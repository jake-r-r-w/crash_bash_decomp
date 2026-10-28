#!/usr/bin/env python3
"""Mode text (CRASHBSH.DAT overlays/text/*.bin): its strings to a text file and back.

Runs inside the toolchain container:
    tools/dk python3 tools/scripts/txt.py unpack assets/CRASHBSH/08D.bin   -> 08D.txt/
    tools/dk python3 tools/scripts/txt.py pack assets/CRASHBSH/08D.txt OUT
`dat.py extract` unpacks them, and `make disc` packs them.

Each file (docs/ASSETS.md) is a u32 (the file's number: 7 to 18), then the mode's strings (zero-
terminated, each starting on a 4-byte boundary), then data that points at them by absolute
address. So a string can change but not move or grow: it has its slot, from its start to the
4-byte boundary after its terminator. <id>.txt/ holds data.bin (the file as it is) and text.txt
(one line per string: its offset, then the text; \\n and \\r are the bytes 0A and 0D, the game's
line breaks (0D is followed by two digits), \\xNN any other byte (FC-FF look like button icons),
\\\\ a backslash). pack writes each changed line into its slot, zero-padded, and refuses one too
long; an unchanged one keeps its slot as it is (four files hold, after a terminator, the tail of
a string cut short in development: "...SCORE TO ZERO", then "9IN THE ALLOTTED TIME").

The data is the mode's menus: panel records (PlaceRec: type, x, y, z, scale, ..., +0x14 a pointer
to its text or image, ...) in lists ending at a type below 2, reached through pointer tables.
menus.json lists each one with its records' fields; a data pointer is @OFFSET (a string, its
text shown beside it) or &OFFSET (other data). pack writes back the fields you change (x has a
flag: 0x8000 set centres it); lists and records can't be added or removed.
"""
import json
import os
import re
import struct
import sys

DATA = "data.bin"
TEXT = "text.txt"
MENUS = "menus.json"
CHARS = set(range(0x20, 0x7F)) | {0x0A, 0x0D} | set(range(0xF0, 0x100))
LINE = re.compile(r"^@([0-9A-F]{4}): (.*)$")


class Bad(Exception):
    pass


def strings(d):
    """[(offset, bytes, slot size)] of the strings after the first u32."""
    out, p = [], 4
    while p < len(d):
        e = d.find(b"\0", p)
        if e <= p or any(b not in CHARS for b in d[p:e]):
            break
        nxt = (e + 4) & ~3
        out.append((p, d[p:e], min(nxt, len(d)) - p))
        while d[nxt:nxt + 4] == bytes(4):        # zero words between strings stay as they are
            nxt += 4
        p = nxt
    return out


def parse(d):
    if len(d) < 8 or struct.unpack_from("<I", d)[0] > 0xFFFF:
        raise Bad("no header")
    s = strings(d)
    if len(s) < 3 or sum(len(t) for _, t, _ in s) < 32:
        raise Bad("not a text file")
    return s


def escape(b):
    out = []
    for c in b:
        out.append("\\n" if c == 0x0A else "\\r" if c == 0x0D else "\\\\" if c == 0x5C
                   else chr(c) if 0x20 <= c < 0x7F else f"\\x{c:02X}")
    return "".join(out)


def unescape(s, where):
    out, i = bytearray(), 0
    while i < len(s):
        c = s[i]
        if c != "\\":
            if not 0x20 <= ord(c) < 0x7F:
                raise Bad(f"{where}: {c!r} isn't in the game's character set (ASCII; \\xNN for others)")
            out.append(ord(c))
            i += 1
            continue
        nxt = s[i + 1:i + 2]
        if nxt in ("n", "r", "\\"):
            out.append({"n": 0x0A, "r": 0x0D, "\\": 0x5C}[nxt])
            i += 2
        elif nxt == "x" and re.fullmatch(r"[0-9A-Fa-f]{2}", s[i + 2:i + 4]):
            out.append(int(s[i + 2:i + 4], 16))
            i += 4
        else:
            raise Bad(f"{where}: bad escape {s[i:i + 4]!r}")
    return bytes(out)


# ---- the menus: the data after the strings is panel records (PlaceRec, include/bash/main.h)
# in lists ending at a type below 2, reached through pointer tables (the engine's
# PauseMenuTexts gives each mode's pause table's offset); the file loads at ModeTextBuffer.

BASE = 0x800AEC60
REC = 0x24
REC_FIELDS = [("type", 0, "i"), ("x", 4, "h"), ("y", 6, "h"), ("z", 8, "h"), ("scale", 0xA, "h"),
              ("unkC", 0xC, "h"), ("unkE", 0xE, "h"), ("unk10", 0x10, "h"), ("pad12", 0x12, "h"),
              ("data", 0x14, "ref"), ("unk18", 0x18, "i"), ("unk1C", 0x1C, "h"), ("unk1E", 0x1E, "h"),
              ("unk20", 0x20, "i")]


def panel_lists(d):
    """[(start, [record offsets])]: every pointer target in the data that holds records of types
    2..0x20 ending at one below 2 (ApplyPlaceRecs' end)."""
    s = strings(d)
    send = s[-1][0] + s[-1][2] if s else 4
    targets = set()
    for p in range(send, len(d) - 3, 4):
        x = struct.unpack_from("<I", d, p)[0]
        if BASE + send <= x < BASE + len(d):
            targets.add(x - BASE)
    out = []
    for t in sorted(targets):
        k, recs = t, []
        while k + REC <= len(d):
            ty = struct.unpack_from("<i", d, k)[0]
            if ty < 2:
                break
            if ty > 0x20:
                recs = None
                break
            recs.append(k)
            k += REC
        if recs and k + REC <= len(d):
            out.append((t, recs))
    return out


def _ref(d, x, starts):
    o = x - BASE
    if 0 <= o < len(d):
        return ("@%04X" if o in starts else "&%04X") % o
    return x


def _unref(v, where):
    if isinstance(v, str):
        m = re.fullmatch(r"[@&]([0-9A-Fa-f]{1,4})", v)
        if not m:
            raise Bad(f"{where}: a pointer is @OFFSET (a string) or &OFFSET (data), not {v!r}")
        return BASE + int(m.group(1), 16)
    return v


def menus(d):
    """The panel lists as JSON-ready dicts: each record's fields, its data pointer as @offset (a
    string, with its text alongside) or &offset (other data)."""
    starts = {o: t for o, t, _ in strings(d)}
    out = []
    for t, recs in panel_lists(d):
        rs = []
        for k in recs:
            r = {}
            for name, off, fmt in REC_FIELDS:
                if fmt == "ref":
                    v = _ref(d, struct.unpack_from("<I", d, k + off)[0], starts)
                    r[name] = v
                    if isinstance(v, str) and v[0] == "@":
                        r["text"] = escape(starts[int(v[1:], 16)])
                else:
                    r[name] = struct.unpack_from("<" + fmt, d, k + off)[0]
            rs.append(r)
        out.append({"at": "%04X" % t, "records": rs})
    return out


def write_menus(d, ms):
    """Write the edited record fields into d (records can be shared by lists: only a field that
    differs from the file's is written; "text" is for reading, edit text.txt)."""
    lists = panel_lists(bytes(d))
    if [m["at"] for m in ms] != ["%04X" % t for t, _ in lists]:
        raise Bad(f"{MENUS}: the lists don't match the file's (they can't be added, removed or moved)")
    for (t, recs), m in zip(lists, ms):
        if len(m["records"]) != len(recs):
            raise Bad(f"{MENUS}: list {m['at']}: records can't be added or removed")
        for k, r in zip(recs, m["records"]):
            for name, off, fmt in REC_FIELDS:
                f = "I" if fmt == "ref" else fmt
                v = _unref(r[name], f"{MENUS}: list {m['at']}") if fmt == "ref" else r[name]
                if v != struct.unpack_from("<" + f, d, k + off)[0]:
                    struct.pack_into("<" + f, d, k + off, v)


def unpack(src, dest):
    d = open(src, "rb").read()
    s = parse(d)
    os.makedirs(dest, exist_ok=True)
    with open(os.path.join(dest, DATA), "wb") as f:
        f.write(d)
    with open(os.path.join(dest, MENUS), "w") as f:
        json.dump(menus(d), f, indent=1)
        f.write("\n")
    with open(os.path.join(dest, TEXT), "w", newline="\n") as f:
        f.write(f"# {os.path.basename(src)}: one string a line. Keep the @offset; a string may not\n"
                "# be longer than it was by more than its padding (pack says).\n")
        for off, t, _ in s:
            f.write(f"@{off:04X}: {escape(t)}\n")


def pack(src):
    d = bytearray(open(os.path.join(src, DATA), "rb").read())
    slots = {off: size for off, _, size in strings(bytes(d))}
    seen = set()
    for n, line in enumerate(open(os.path.join(src, TEXT), newline="\n"), 1):
        line = line.rstrip("\n")
        if not line or line.startswith("#"):
            continue
        m = LINE.match(line)
        if not m:
            raise Bad(f"{TEXT}:{n}: expected '@OFFSET: text'")
        off = int(m.group(1), 16)
        if off not in slots:
            raise Bad(f"{TEXT}:{n}: no string starts at @{off:04X}")
        b = unescape(m.group(2), f"{TEXT}:{n}")
        if len(b) + 1 > slots[off]:
            raise Bad(f"{TEXT}:{n}: {len(b)} bytes, the slot holds {slots[off] - 1}")
        if d[off:off + len(b)] != b or d[off + len(b)]:
            # changed: zero the rest of the slot. An unchanged one keeps what follows its
            # terminator: some hold the tail of a string cut short in development
            d[off:off + slots[off]] = b + bytes(slots[off] - len(b))
        seen.add(off)
    if seen != set(slots):
        raise Bad(f"{TEXT}: missing strings at " + ", ".join(f"@{o:04X}" for o in sorted(set(slots) - seen)))
    mp = os.path.join(src, MENUS)
    if os.path.exists(mp):
        write_menus(d, json.load(open(mp)))
    return bytes(d)


def main():
    if len(sys.argv) < 3 or sys.argv[1] not in ("unpack", "pack"):
        sys.exit(__doc__.split("\n\n")[1])
    if sys.argv[1] == "unpack":
        src = sys.argv[2]
        unpack(src, os.path.splitext(src)[0] + ".txt")
    else:
        with open(sys.argv[3], "wb") as fh:
            fh.write(pack(sys.argv[2]))


if __name__ == "__main__":
    main()
