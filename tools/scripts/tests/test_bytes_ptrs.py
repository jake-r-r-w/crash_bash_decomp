"""Tests for bytes_ptrs.py: aligned words of a byte or halfword array equal to a symbol.
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import bytes_ptrs  # noqa: E402

SYMS = {0x8004DD84: "ModeTypeNames", 0x80010410: "shell_start_engine"}


class BytesPtrsTest(unittest.TestCase):
    def test_bytes(self):
        vals = [0x84, 0xDD, 4, 0x80, 0, 0, 0, 0, 0x84, 0xDD, 4, 0x80]
        hits = bytes_ptrs.scan(vals, 1, 0x8004E000, SYMS)
        self.assertEqual([(o, s) for o, w, s in hits], [(0, "ModeTypeNames"), (8, "ModeTypeNames")])

    def test_alignment_follows_the_address(self):
        # the array starts 2 bytes before a word boundary: the word is at offset 2
        vals = [0, 0, 0x10, 4, 1, 0x80]
        hits = bytes_ptrs.scan(vals, 1, 0x80050002, SYMS)
        self.assertEqual([(o, s) for o, w, s in hits], [(2, "shell_start_engine")])

    def test_halfwords(self):
        hits = bytes_ptrs.scan([0x410, 0x8001, 5, 6], 2, 0x80050000, SYMS)
        self.assertEqual([s for o, w, s in hits], ["shell_start_engine"])

    def test_numbers_that_are_not_symbols(self):
        self.assertEqual(bytes_ptrs.scan([0, 0, 7, 0x80], 1, 0x80050000, SYMS), [])

    def test_inside_an_object(self):
        objs = [(0x8004E110, 0x1F00, "DatIndex")]
        hits = bytes_ptrs.scan([0x8004E120, 0x8004E110, 0x80060000], 4, 0x80050000, {0x8004E110: "DatIndex"}, objects=objs)
        self.assertEqual([s for o, w, s in hits], ["DatIndex+0x10", "DatIndex"])

    def test_values(self):
        self.assertEqual(bytes_ptrs.values("{ 1, -2, 0x10 }, { 'a', 0 }"), [1, -2, 16, 97, 0])
        self.assertIsNone(bytes_ptrs.values("1, Foo, 2"))


if __name__ == "__main__":
    unittest.main()
