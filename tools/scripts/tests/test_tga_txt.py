"""Tests for tga.py (title screens) and txt.py (mode text). Run: tools/dk make test-tools"""
import os
import shutil
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import tga  # noqa: E402
import txt  # noqa: E402
from PIL import Image  # noqa: E402

FOOTER = bytes(8) + b"TRUEVISION-XFILE.\0"


def screen(px, w=3, h=2):
    return struct.pack("<BBBHHBHHHHBB", 0, 0, 2, 0, 0, 0, 0, 0, w, h, 16, 1) + struct.pack(f"<{w * h}H", *px) + FOOTER


def text_file():
    body = b"OBJECTIVE\0\0\0" + b"WIN BY\r09BEING FIRST\0\0\0\0" +b"ZERO\x009IN\0\0\0\0" +b"\xff\0\0\0" + b"EXIT\0\0\0\0"
    return struct.pack("<I", 9) + body + bytes(4) + struct.pack("<I", 0x800AEC64)


class Base(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.dir)

    def unpacked(self, mod, data):
        src = os.path.join(self.dir, "x.bin")
        with open(src, "wb") as f:
            f.write(data)
        out = os.path.join(self.dir, "x.out")
        mod.unpack(src, out)
        return out


class Screens(Base):
    PX = [0x7C00, 0x03E0, 0x001F, 0x0000, 0x7FFF, 0x1234]   # bottom row first: red green blue / ...

    def test_round_trip_and_orientation(self):
        out = self.unpacked(tga, screen(self.PX))
        self.assertEqual(tga.pack(out), screen(self.PX))
        im = Image.open(os.path.join(out, tga.PNG))
        self.assertEqual(im.getpixel((0, 1)), (248, 0, 0))     # the file's first pixel is bottom left
        self.assertEqual(im.getpixel((1, 0)), (248, 248, 248))

    def test_attribute_bit_or_other_tgas_refused(self):
        with self.assertRaises(tga.Bad):
            tga.parse(screen([0x8000] * 6))
        with self.assertRaises(tga.Bad):
            tga.parse(screen(self.PX)[:2] + b"\x0a" + screen(self.PX)[3:])   # RLE


class Text(Base):
    def test_round_trip_keeps_a_cut_strings_tail(self):
        out = self.unpacked(txt, text_file())
        self.assertEqual(txt.pack(out), text_file())
        lines = open(os.path.join(out, txt.TEXT)).read().splitlines()
        self.assertIn("@0004: OBJECTIVE", lines)
        self.assertIn("@0010: WIN BY\\r09BEING FIRST", lines)
        self.assertIn("@0034: \\xFF", lines)

    def edit(self, out, old, new):
        p = os.path.join(out, txt.TEXT)
        t = open(p).read()
        self.assertIn(old, t)
        open(p, "w").write(t.replace(old, new))

    def test_an_edit_is_zero_padded_in_its_slot(self):
        out = self.unpacked(txt, text_file())
        self.edit(out, "@0028: ZERO", "@0028: NIL")
        d = txt.pack(out)
        self.assertEqual(d[0x28:0x30], b"NIL" + bytes(5))          # the cut tail goes with the edit
        self.assertEqual(d[0x30:], text_file()[0x30:])

    def test_too_long_or_bad_escape_refused(self):
        out = self.unpacked(txt, text_file())
        self.edit(out, "@0004: OBJECTIVE", "@0004: OBJECTIVES!!")
        with self.assertRaises(txt.Bad):
            txt.pack(out)
        out = self.unpacked(txt, text_file())
        self.edit(out, "@0038: EXIT", "@0038: \\q")
        with self.assertRaises(txt.Bad):
            txt.pack(out)


if __name__ == "__main__":
    unittest.main()
