"""Tests for tex.py: texture sets to PNGs and back. Run: tools/dk make test-tools"""
import os
import shutil
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import tex  # noqa: E402
from PIL import Image  # noqa: E402

PRE = struct.pack("<4I", 0x10, 0x1234, 0, 0)


def fields(w, h, clut, eight=False):
    return [w, h, 0, 0x2010, 0, (clut << 1 | eight) & 0xFFFF, 1]


def sample(tail=b""):
    """Two CLUTs (16 and 256 colours), a 4-bit image on CLUT 0, an 8-bit one on CLUT 1, and one
    with a negative CLUT index."""
    cluts = [[0, 0x8000] + list(range(1, 15)), list(range(0x7F00, 0x8000))]
    images = [(fields(2, 3, 0), bytes(range(12))),
              (fields(2, 2, 1, eight=True), bytes(range(200, 208))),
              (fields(1, 1, -1), b"\x21\x43")]
    return tex.build(PRE, cluts, images, tail)


class Format(unittest.TestCase):
    def test_parse_reads_back_what_build_writes(self):
        d = sample(b"\x08\0\0\0" + bytes(4))
        pre, cluts, images, tail = tex.parse(d)
        self.assertEqual((pre, len(cluts), len(images), tail), (PRE, 2, 3, b"\x08\0\0\0" + bytes(4)))
        self.assertEqual(struct.unpack_from("<IIHHI", d), (8, len(d), 3, 2, 0x20 + 4 + 32 + 4 + 512 - 0x0C))

    def test_not_a_set(self):
        for d in (b"", struct.pack("<II", 8, 99) + bytes(24), b"\x0c" + bytes(40)):
            with self.assertRaises(tex.Bad):
                tex.parse(d)

    def test_mode_is_depth_and_clut(self):
        self.assertEqual(tex.mode(fields(1, 1, 3)), (4, 3))
        self.assertEqual(tex.mode(fields(1, 1, 5, eight=True)), (8, 5))
        self.assertEqual(tex.mode(fields(1, 1, -1)), (4, -1))

    def test_colours_round_trip(self):
        for c in (0, 0x8000, 0x7FFF, 0xFFFF, 0x1234, 0x8421):
            self.assertEqual(tex.from_rgba(*tex.to_rgba(c)), c)

    def test_pixels_round_trip(self):
        px = bytes(range(256))
        for depth in (4, 8):
            self.assertEqual(tex.from_indices(tex.to_indices(px, depth), depth), px)
        self.assertEqual(tex.to_indices(b"\x21", 4), b"\x01\x02")   # low nibble is the left pixel


class Files(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.mkdtemp()
        self.bin = os.path.join(self.dir, "000.bin")
        self.out = os.path.join(self.dir, "000.tex")
        with open(self.bin, "wb") as f:
            f.write(sample(b"tail"))
        tex.unpack(self.bin, self.out)

    def tearDown(self):
        shutil.rmtree(self.dir)

    def test_unpack_pack_is_byte_identical(self):
        self.assertEqual(tex.pack(self.out), sample(b"tail"))

    def test_pngs_have_the_depths_and_palettes(self):
        a = Image.open(os.path.join(self.out, "img_000.png"))
        b = Image.open(os.path.join(self.out, "img_001.png"))
        self.assertEqual((a.mode, a.size, b.size), ("P", (8, 3), (4, 2)))
        self.assertEqual(tuple(a.getpalette()[3:6]), (0, 0, 0))        # 0x8000: black, STP set
        self.assertEqual(tuple(a.getpalette()[6:9]), (8, 0, 0))        # 1: r = 1 << 3

    def test_an_edited_pixel_is_packed(self):
        p = os.path.join(self.out, "img_000.png")
        im = Image.open(p)
        im.putpixel((0, 0), 15)
        im.save(p)
        self.assertEqual(tex.parse(tex.pack(self.out))[2][0][1][0], 0x0F)   # was 0x00: left pixel 15

    def test_too_many_colours_or_wrong_size_refused(self):
        p = os.path.join(self.out, "img_000.png")
        im = Image.frombytes("P", (8, 3), bytes([20]) * 24)   # index 20 in a 4-bit image
        im.putpalette(list(range(256)) * 3)
        im.save(p)
        with self.assertRaises(tex.Bad):
            tex.pack(self.out)
        Image.new("P", (9, 3)).save(p)
        with self.assertRaises(tex.Bad):
            tex.pack(self.out)


if __name__ == "__main__":
    unittest.main()
