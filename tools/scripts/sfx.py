#!/usr/bin/env python3
"""Sound packs (CRASHBSH.DAT .sfx entries): samples to WAV and back; banks and music as Sony's files.

Runs inside the toolchain container:
    tools/dk python3 tools/scripts/sfx.py unpack assets/CRASHBSH/099.bin   -> 099.sfx/
    tools/dk python3 tools/scripts/sfx.py pack assets/CRASHBSH/099.sfx OUT
`dat.py extract` unpacks them, and `make disc` packs them.

A pack (docs/ASSETS.md; CTR-tools reads it the same way, CTRFramework/Code/bash/BashSfx.cs) is a
table of u32 offsets (VB, VH, each SEQ, the last running to the end; the first is the table's
size; an effects-only pack has one empty SEQ, its offset the file's end), then the VB
(SPU ADPCM samples, back to back), the VH (a VAB header, its sample-size table trimmed to the
samples it has in the effects-only packs) and the SEQs (Sony's MIDI-like sequences, the music).

<id>.sfx/ holds bank.vh and bank.vb (the VAB as it is: PS1 sound tools open the pair),
music_N.seq (the sequences as they are; none in an effects-only pack), sample_NNN.wav (each sample decoded: 16-bit mono, written
at a nominal 22050 Hz, which pack ignores) and sfx.json. pack takes each sample's original bytes
when its WAV still decodes the same, and encodes the WAV to ADPCM when it changed (looping or
one-shot as the original was), then fixes the VH's size table and the offsets. Music:
music_N.mid is each SEQ as a MIDI file (format 0; the SEQ's header as tempo and time-signature
events). An edited one (any MIDI a sequencer saves; tracks are merged) becomes the SEQ; while it
is as unpack wrote it, music_N.seq is used as it is. A pack may come
out smaller than the original but not bigger (dat.py refuses).
"""
import hashlib
import json
import os
import struct
import sys
import wave

JSON = "sfx.json"
RATE = 22050
FILTERS = [(0, 0), (60, 0), (115, -52), (98, -55), (122, -60)]


class Bad(Exception):
    pass


def parse(d):
    """(VB, VH, [SEQ], [sample bytes]) of a pack."""
    if len(d) < 16:
        raise Bad("short")
    n = struct.unpack_from("<I", d)[0] // 4
    if not 3 <= n <= 8 or n * 4 > len(d):
        raise Bad("no offset table")
    ptrs = list(struct.unpack_from(f"<{n}I", d)) + [len(d)]
    if ptrs[0] != n * 4 or ptrs != sorted(ptrs):
        raise Bad("offsets out of order")
    vb, vh, seqs = d[ptrs[0]:ptrs[1]], d[ptrs[1]:ptrs[2]], [d[a:b] for a, b in zip(ptrs[2:], ptrs[3:])]
    if vh[:4] != b"pBAV" or any(s and s[:4] != b"pQES" for s in seqs):
        raise Bad("not a VAB (and SEQs)")
    sizes = vag_sizes(vh)
    if sum(sizes) * 8 != len(vb):
        raise Bad("the samples don't fill the VB")
    samples, p = [], 0
    for s in sizes:
        samples.append(vb[p:p + s * 8])
        p += s * 8
    return vb, vh, seqs, samples


def size_table(vh):
    """(offset of the VAG size table in vh, number of samples): u16 sizes / 8, slot 0 unused."""
    ps, _, vs = struct.unpack_from("<HHH", vh, 0x12)
    at = 0x20 + 128 * 16 + ps * 16 * 32
    if at + (vs + 1) * 2 > len(vh):
        raise Bad("VH too short for its size table")
    return at, vs


def vag_sizes(vh):
    at, vs = size_table(vh)
    return list(struct.unpack_from(f"<{vs}H", vh, at + 2))


# ---- SPU ADPCM: 16-byte blocks, 28 samples; byte 0 shift | filter << 4, byte 1 flags

def decode(blocks):
    out, s1, s2 = [], 0, 0
    for k in range(0, len(blocks), 16):
        shift, filt = blocks[k] & 15, min(blocks[k] >> 4, 4)
        f0, f1 = FILTERS[filt]
        for b in blocks[k + 2:k + 16]:
            for nib in (b & 15, b >> 4):
                t = (nib - 16 if nib & 8 else nib) << 12 >> shift
                t += (s1 * f0 + s2 * f1 + 32) >> 6
                t = max(-32768, min(32767, t))
                s2, s1 = s1, t
                out.append(t)
    return out


def encode_block(xs, s1, s2, flags):
    """The block (filter and shift with the least error) for 28 samples, and the new history."""
    best = None
    for filt, (f0, f1) in enumerate(FILTERS):
        for shift in range(13):
            a, b, err, nibs = s1, s2, 0, []
            for x in xs:
                pred = (a * f0 + b * f1 + 32) >> 6
                step = 1 << (12 - shift)
                q = max(-8, min(7, round((x - pred) / step)))
                y = max(-32768, min(32767, (q << 12 >> shift) + pred))
                err += (x - y) ** 2
                b, a = a, y
                nibs.append(q & 15)
                if best and err >= best[0]:
                    break
            else:
                if best is None or err < best[0]:
                    best = (err, filt, shift, nibs, a, b)
    _, filt, shift, nibs, a, b = best
    body = bytes(nibs[i] | nibs[i + 1] << 4 for i in range(0, 28, 2))
    return bytes((shift | filt << 4, flags)) + body, a, b


def audio_blocks(raw):
    """The blocks a sample's audio is in: after the silent lead block, up to the end or loop-end."""
    flags = [raw[k + 1] for k in range(0, len(raw), 16)]
    end = next((i for i, f in enumerate(flags) if f & 1), len(flags) - 1)
    return raw[16:16 * (end + 1)], flags


def encode(pcm, original):
    """A sample's bytes from PCM, shaped like the original: lead block, audio, and a one-shot's
    end flag and terminator block, or a loop's start, middle and end flags."""
    _, flags = audio_blocks(original)
    loop = 3 in flags or 2 in flags
    pcm = list(pcm) + [0] * (-len(pcm) % 28 or (0 if pcm else 28))
    n = len(pcm) // 28
    out, s1, s2 = [bytes(16)], 0, 0
    for i in range(n):
        if loop:
            f = 6 if i == 0 else 3 if i == n - 1 else 2
        else:
            f = 1 if i == n - 1 else 0
        blk, s1, s2 = encode_block(pcm[i * 28:i * 28 + 28], s1, s2, f)
        out.append(blk)
    if not loop:
        out.append(original[-16:])                   # the terminator block (flag 7), as it was
    return b"".join(out)


# ---- SEQ <-> MIDI. A SEQ: "pQES", u32 version, then big-endian u16 ticks per quarter note, u24
# microseconds per quarter note, u8 time-signature numerator and denominator; then MIDI events.

def varlen(n):
    out = [n & 0x7F]
    n >>= 7
    while n:
        out.append(0x80 | (n & 0x7F))
        n >>= 7
    return bytes(reversed(out))


def read_varlen(d, i):
    n = 0
    while True:
        b = d[i]
        i += 1
        n = n << 7 | (b & 0x7F)
        if not b & 0x80:
            return n, i


def seq_to_mid(seq):
    if seq[:4] != b"pQES" or len(seq) < 15:
        raise Bad("not a SEQ")
    ppqn, tempo, num, den = struct.unpack(">H", seq[8:10])[0], seq[10:13], seq[13], seq[14]
    power = den.bit_length() - 1 if den and den & (den - 1) == 0 else 2
    track = b"\x00\xff\x51\x03" + tempo + bytes((0, 0xFF, 0x58, 4, num, power, 24, 8))
    if 1 << power != den:                            # not a power of two (one song has 3): MIDI
        track += b"\x00\xff\x7f\x05pQES" + bytes((den,))   # can't say it; a sequencer event keeps it
    track += seq[15:]
    return b"MThd" + struct.pack(">IHHH", 6, 0, 1, ppqn) + b"MTrk" + struct.pack(">I", len(track)) + track


def midi_events(mid):
    """(ticks per quarter, [(absolute tick, order, event bytes)]) of every track, merged."""
    if mid[:4] != b"MThd":
        raise Bad("not a MIDI file")
    hlen, _, ntrk, div = struct.unpack(">IHHH", mid[4:14])
    if div & 0x8000:
        raise Bad("SMPTE time division isn't supported")
    i, events, order = 8 + hlen, [], 0
    for _ in range(ntrk):
        if mid[i:i + 4] != b"MTrk":
            raise Bad("bad track")
        n = struct.unpack(">I", mid[i + 4:i + 8])[0]
        j, end, tick, status = i + 8, i + 8 + n, 0, None
        while j < end:
            dt, j = read_varlen(mid, j)
            tick += dt
            b = mid[j]
            if b == 0xFF:
                kind = mid[j + 1]
                ln, k = read_varlen(mid, j + 2)
                ev = mid[j:k + ln]
                j = k + ln
                if kind == 0x2F:
                    break
            elif b in (0xF0, 0xF7):
                ln, k = read_varlen(mid, j + 1)
                j = k + ln
                continue                             # sysex: a SEQ has none
            else:
                if b & 0x80:
                    status, j = b, j + 1
                if status is None:
                    raise Bad("running status with no status")
                size = 1 if status >> 4 in (0xC, 0xD) else 2
                ev = bytes((status,)) + mid[j:j + size]
                j += size
            events.append((tick, order, ev))
            order += 1
        i = end
    events.sort()
    return div, events


def mid_to_seq(mid):
    """A SEQ from any MIDI file (tracks merged; tempo changes kept, other meta events and sysex
    dropped), with running status as the game's SEQs have it."""
    div, events = midi_events(mid)
    tempo, sig = (500000).to_bytes(3, "big"), (4, 4)
    for tick, _, ev in events:
        if tick:
            break
        if ev[:2] == b"\xff\x51":
            tempo = ev[3:6]
        elif ev[:2] == b"\xff\x58":
            sig = (ev[3], 1 << ev[4])
        elif ev[:7] == b"\xff\x7f\x05pQES":
            sig = (sig[0], ev[7])
    out, last, status = bytearray(), 0, None
    for tick, _, ev in events:
        if ev[0] == 0xFF:
            if ev[1] != 0x51 or not tick:            # tempo changes stay; the rest is MIDI-only
                continue
            out += varlen(tick - last) + ev
            status = None
        else:
            out += varlen(tick - last) + (ev[1:] if ev[0] == status else ev)
            status = ev[0]
        last = tick
    out += b"\x00\xff\x2f\x00" * 2                   # the game's SEQs all end with two
    return b"pQES" + struct.pack(">IH", 1, div) + tempo + bytes(sig) + bytes(out)


def write_wav(path, pcm):
    with wave.open(path, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(RATE)
        w.writeframes(struct.pack(f"<{len(pcm)}h", *pcm))


def read_wav(path):
    with wave.open(path, "rb") as w:
        if w.getnchannels() != 1 or w.getsampwidth() != 2:
            raise Bad(f"{path}: save it as 16-bit mono WAV")
        data = w.readframes(w.getnframes())
    return list(struct.unpack(f"<{len(data) // 2}h", data))


def unpack(src, dest):
    vb, vh, seqs, samples = parse(open(src, "rb").read())
    os.makedirs(dest, exist_ok=True)
    for name, data in (("bank.vb", vb), ("bank.vh", vh)):
        with open(os.path.join(dest, name), "wb") as f:
            f.write(data)
    meta = {"rate": RATE, "seqs": len(seqs), "samples": [], "mid_sha1": {}}
    for i, s in enumerate(seqs):
        if s:                                        # an effects-only pack's empty one: no file
            with open(os.path.join(dest, f"music_{i}.seq"), "wb") as f:
                f.write(s)
            mid = seq_to_mid(s)
            with open(os.path.join(dest, f"music_{i}.mid"), "wb") as f:
                f.write(mid)
            meta["mid_sha1"][str(i)] = hashlib.sha1(mid).hexdigest()
    for i, raw in enumerate(samples, 1):
        name = f"sample_{i:03d}.wav"
        blocks, flags = audio_blocks(raw)
        write_wav(os.path.join(dest, name), decode(blocks))
        meta["samples"].append({"file": name, "loop": 3 in flags or 2 in flags})
    with open(os.path.join(dest, JSON), "w") as f:
        json.dump(meta, f, indent=1)
        f.write("\n")


def pack(src):
    meta = json.load(open(os.path.join(src, JSON)))
    vh = bytearray(open(os.path.join(src, "bank.vh"), "rb").read())
    vb = open(os.path.join(src, "bank.vb"), "rb").read()
    seqs = []
    for i in range(meta["seqs"]):
        q, m = os.path.join(src, f"music_{i}.seq"), os.path.join(src, f"music_{i}.mid")
        known = meta.get("mid_sha1", {}).get(str(i))
        if os.path.exists(m) and known and hashlib.sha1(open(m, "rb").read()).hexdigest() != known:
            seqs.append(mid_to_seq(open(m, "rb").read()))       # the MIDI was edited
        else:
            seqs.append(open(q, "rb").read() if os.path.exists(q) else b"")
    originals, p = [], 0
    for s in vag_sizes(bytes(vh)):
        originals.append(vb[p:p + s * 8])
        p += s * 8
    if len(originals) != len(meta["samples"]):
        raise Bad(f"bank.vh has {len(originals)} samples, {JSON} {len(meta['samples'])}")
    out = []
    for e, raw in zip(meta["samples"], originals):
        pcm = read_wav(os.path.join(src, e["file"]))
        out.append(raw if pcm == decode(audio_blocks(raw)[0]) else encode(pcm, raw))
    at, vs = size_table(bytes(vh))
    for i, s in enumerate(out, 1):
        if len(s) // 8 > 0xFFFF:
            raise Bad(f"{meta['samples'][i - 1]['file']} is too long")
        struct.pack_into("<H", vh, at + 2 * i, len(s) // 8)
    vb = b"".join(out)
    parts = [vb, bytes(vh), *seqs]
    ptrs, p = [], 4 * len(parts)
    for part in parts:
        ptrs.append(p)
        p += len(part)
    return struct.pack(f"<{len(ptrs)}I", *ptrs) + b"".join(parts)


def main():
    if len(sys.argv) < 3 or sys.argv[1] not in ("unpack", "pack"):
        sys.exit(__doc__.split("\n\n")[1])
    if sys.argv[1] == "unpack":
        src = sys.argv[2]
        unpack(src, os.path.splitext(src)[0] + ".sfx")
    else:
        with open(sys.argv[3], "wb") as fh:
            fh.write(pack(sys.argv[2]))


if __name__ == "__main__":
    main()
