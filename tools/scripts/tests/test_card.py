"""Tests for card.py: the memory-card save. Run: tools/dk make test-tools"""
import json
import os
import shutil
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import card  # noqa: E402


def save():
    """A save: the header, slot 0 in use (summary and a few GameState words), the flags and sum."""
    s = bytearray(card.BLOCK)
    s[0:4] = b"SC\x12\x01"
    title = "ＣＲＡＳＨ　ＢＡＳＨ".encode("shift_jis")
    s[4:4 + len(title)] = title
    s[card.SLOT_AT:card.SLOT_AT + 8] = bytes([1, 3, 6, 8, 0, 1, 1, 1])
    struct.pack_into("<i", s, card.SLOT_AT + 8 + 0xC, 1234)            # GameState.frames
    struct.pack_into("<I", s, 0x850, 1)
    struct.pack_into("<I", s, card.SUM_AT, card.checksum(s))
    return bytes(s)


def image(sv):
    """A 128 KB card with the save in block 2."""
    c = bytearray(16 * card.BLOCK)
    c[0:2] = b"MC"
    f = 2 * card.FRAME
    struct.pack_into("<II", c, f, 0x51, card.BLOCK)
    c[f + 0x0A:f + 0x0A + len(card.NAME)] = card.NAME
    c[2 * card.BLOCK:3 * card.BLOCK] = sv
    return bytes(c)


class Card(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.mkdtemp()
        self.src = os.path.join(self.dir, "x.srm")
        with open(self.src, "wb") as f:
            f.write(image(save()))
        self.out = os.path.join(self.dir, "x.card")
        card.unpack(self.src, self.out)

    def tearDown(self):
        shutil.rmtree(self.dir)

    def test_read(self):
        m = json.load(open(os.path.join(self.out, card.JSON)))
        self.assertEqual((m["title_ascii"], m["sum_ok"], m["flags"][0]), ("CRASH BASH", True, 1))
        self.assertEqual(m["slots"][0]["summary"], [1, 3, 6, 8, 0, 1, 1, 1])
        self.assertEqual(m["slots"][0]["state"]["frames"], 1234)

    def test_round_trip_and_an_edit_fixes_the_sum(self):
        self.assertEqual(card.pack(self.out, self.src), image(save()))
        p = os.path.join(self.out, card.JSON)
        m = json.load(open(p))
        m["slots"][0]["state"]["frames"] = 99
        json.dump(m, open(p, "w"))
        sv = card.pack(self.out, self.src)[2 * card.BLOCK:3 * card.BLOCK]
        self.assertEqual(struct.unpack_from("<i", sv, card.SLOT_AT + 8 + 0xC)[0], 99)
        self.assertEqual(struct.unpack_from("<I", sv, card.SUM_AT)[0], card.checksum(sv))

    def test_no_save_refused(self):
        with self.assertRaises(card.Bad):
            card.find(bytes(16 * card.BLOCK))


if __name__ == "__main__":
    unittest.main()
