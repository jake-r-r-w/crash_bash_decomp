"""Tests for decomp.py units' overlay support: where an overlay's .bss tail starts.
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402


class ZeroTail(unittest.TestCase):
    def test_tail_starts_after_the_last_nonzero_word(self):
        data = b"\x01\0\0\0" + b"\0\0\x02\0" + b"\0" * 12
        self.assertEqual(decomp.zero_tail_start(0x800C0000, data), 0x800C0008)

    def test_a_partly_zero_word_rounds_up(self):
        data = b"\x01\0\0\0" + b"\x05" + b"\0" * 11
        self.assertEqual(decomp.zero_tail_start(0x800C0000, data), 0x800C0008)

    def test_all_zero_data_is_all_tail(self):
        self.assertEqual(decomp.zero_tail_start(0x800C0000, b"\0" * 16), 0x800C0000)

    def test_no_tail(self):
        self.assertEqual(decomp.zero_tail_start(0x800C0000, b"\x01\x02\x03\x04"), 0x800C0004)


if __name__ == "__main__":
    unittest.main()
