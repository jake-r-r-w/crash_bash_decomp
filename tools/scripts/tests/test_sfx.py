"""Tests for sfx.py: sound packs' samples to WAV and back. Run: tools/dk make test-tools"""
import math
import os
import shutil
import struct
import sys
import tempfile
import unittest
import wave

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import sfx  # noqa: E402

PS = 1


def tone(n, f=440.0, amp=8000):
    return [int(amp * math.sin(2 * math.pi * f * i / 22050)) for i in range(n)]


def vh(sizes, full_table=False):
    """A VAB header: 1 program, its 16 tones, the sample sizes (/8) after slot 0."""
    head = struct.pack("<4sIIIHHHHBBBB4s", b"pBAV", 7, 0, 0, 0xEEEE, PS, 1, len(sizes), 127, 64, 0, 0, b"\xff" * 4)
    table = [0] + sizes + ([0] * (255 - len(sizes)) if full_table else [])
    return head + bytes(128 * 16 + PS * 16 * 32) + struct.pack(f"<{len(table)}H", *table)


def one_shot(pcm):
    """A sample as the game's are: lead block, audio (end flag on the last), terminator block."""
    return sfx.encode(pcm, bytes(16) + bytes((0, 1)) + bytes(14) + bytes((0, 7)) + b"\x77" * 14)


def looped(pcm):
    return sfx.encode(pcm, bytes(16) + bytes((0, 6)) + bytes(14) + bytes((0, 3)) + bytes(14))


def pack_bytes(samples, seqs=(), full_table=False):
    vb = b"".join(samples)
    h = vh([len(s) // 8 for s in samples], full_table)
    parts = [vb, h, *seqs] if seqs else [vb, h, b""]
    ptrs, p = [], 4 * len(parts)
    for part in parts:
        ptrs.append(p)
        p += len(part)
    return struct.pack(f"<{len(ptrs)}I", *ptrs) + b"".join(parts)


SEQ = b"pQES" + bytes(12) + b"\xff\x2f\x00"


class Codec(unittest.TestCase):
    def test_encode_decode_is_close(self):
        pcm = tone(28 * 40)
        back = sfx.decode(sfx.audio_blocks(one_shot(pcm))[0])
        err = sum((a - b) ** 2 for a, b in zip(pcm, back))
        sig = sum(x * x for x in pcm)
        self.assertGreater(10 * math.log10(sig / err), 30)

    def test_flags_follow_the_original(self):
        s = one_shot(tone(28 * 3))
        self.assertEqual([s[k + 1] for k in range(0, len(s), 16)], [0, 0, 0, 1, 7])
        s = looped(tone(28 * 3))
        self.assertEqual([s[k + 1] for k in range(0, len(s), 16)], [0, 6, 2, 3])

    def test_not_a_pack(self):
        for d in (b"", bytes(64), struct.pack("<3I", 12, 8, 100) + bytes(100)):
            with self.assertRaises(sfx.Bad):
                sfx.parse(d)


class Files(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.dir)

    def unpacked(self, data):
        src = os.path.join(self.dir, "x.bin")
        with open(src, "wb") as f:
            f.write(data)
        out = os.path.join(self.dir, "x.sfx")
        sfx.unpack(src, out)
        return out

    def test_effects_pack_round_trip(self):
        d = pack_bytes([one_shot(tone(28 * 5)), looped(tone(28 * 7, 220))])
        out = self.unpacked(d)
        self.assertEqual(sfx.pack(out), d)
        self.assertEqual(sorted(os.listdir(out)), ["bank.vb", "bank.vh", "sample_001.wav", "sample_002.wav", "sfx.json"])

    def test_music_pack_round_trip(self):
        d = pack_bytes([one_shot(tone(28 * 5))], seqs=[SEQ, SEQ + b"\0"], full_table=True)
        out = self.unpacked(d)
        self.assertEqual(sfx.pack(out), d)
        self.assertTrue(os.path.exists(os.path.join(out, "music_1.seq")))

    def test_an_edited_wav_is_encoded_and_sizes_fixed(self):
        d = pack_bytes([one_shot(tone(28 * 5)), one_shot(tone(28 * 5))])
        out = self.unpacked(d)
        with wave.open(os.path.join(out, "sample_001.wav"), "wb") as w:
            w.setnchannels(1)
            w.setsampwidth(2)
            w.setframerate(22050)
            w.writeframes(struct.pack(f"<{28 * 9}h", *tone(28 * 9, 330)))
        vb, vh_, seqs, samples = sfx.parse(sfx.pack(out))
        self.assertEqual([len(s) for s in samples], [16 * 11, 16 * 7])      # lead + 9 + terminator
        self.assertEqual(samples[1], sfx.parse(d)[3][1])                   # the other kept as it was

    def test_stereo_refused(self):
        out = self.unpacked(pack_bytes([one_shot(tone(56))]))
        with wave.open(os.path.join(out, "sample_001.wav"), "wb") as w:
            w.setnchannels(2)
            w.setsampwidth(2)
            w.setframerate(22050)
            w.writeframes(bytes(8))
        with self.assertRaises(sfx.Bad):
            sfx.pack(out)


def seq(events, den=4):
    """A SEQ as the game writes them: header, events, two end-of-track markers."""
    return (b"pQES" + struct.pack(">IH", 1, 384) + (431650).to_bytes(3, "big") + bytes((1, den))
            + events + b"\x00\xff\x2f\x00" * 2)


EVENTS = b"\x00\xb0\x07\x7f\x00\x90\x3c\x40\x60\x3e\x40\x83\x00\x80\x3c\x00"   # running status


class Music(unittest.TestCase):
    def test_seq_mid_seq_is_byte_identical(self):
        for den in (4, 3):                                 # 3 isn't a power of two: kept anyway
            s = seq(EVENTS, den)
            m = sfx.seq_to_mid(s)
            self.assertEqual(m[:4], b"MThd")
            self.assertEqual(sfx.mid_to_seq(m), s)

    def test_a_two_track_midi_is_merged(self):
        def track(ev):
            return b"MTrk" + struct.pack(">I", len(ev)) + ev
        t1 = b"\x00\xff\x51\x03\x07\xa1\x20" + b"\x00\x90\x3c\x40\x60\x80\x3c\x00" + b"\x00\xff\x2f\x00"
        t2 = b"\x30\x91\x40\x40\x60\x81\x40\x00\x00\xff\x2f\x00"
        mid = b"MThd" + struct.pack(">IHHH", 6, 1, 2, 96) + track(t1) + track(t2)
        s = sfx.mid_to_seq(mid)
        self.assertEqual(struct.unpack(">H", s[8:10])[0], 96)
        self.assertEqual(s[10:13], b"\x07\xa1\x20")
        self.assertEqual(s[15:], b"\x00\x90\x3c\x40\x30\x91\x40\x40\x30\x80\x3c\x00\x30\x81\x40\x00"
                         + b"\x00\xff\x2f\x00" * 2)

    def test_an_edited_mid_becomes_the_seq(self):
        d = pack_bytes([one_shot(tone(56))], seqs=[seq(EVENTS)], full_table=True)
        tmp = tempfile.mkdtemp()
        try:
            src = os.path.join(tmp, "x.bin")
            open(src, "wb").write(d)
            out = os.path.join(tmp, "x.sfx")
            sfx.unpack(src, out)
            self.assertEqual(sfx.pack(out), d)
            m = os.path.join(out, "music_0.mid")
            edited = sfx.seq_to_mid(seq(EVENTS.replace(b"\x3e\x40", b"\x40\x40")))
            open(m, "wb").write(edited)
            self.assertEqual(sfx.parse(sfx.pack(out))[2][0], seq(EVENTS.replace(b"\x3e\x40", b"\x40\x40")))
        finally:
            shutil.rmtree(tmp)


if __name__ == "__main__":
    unittest.main()
