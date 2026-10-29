"""raw_offsets.py: the byte-offset reads the progress check misses."""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import raw_offsets  # noqa: E402


class RawLines(unittest.TestCase):
    def test_finds_and_skips(self):
        body = ("x = *(s32 *)(p + 0x18);\nt[0x274 / 4] = f;\nq = (s32 *)(base + 0x200);\n"
                "y = p->field;\nz = *(s32 *)(p + 4);  /* matching: kept */\n")
        self.assertEqual(raw_offsets.raw_lines(body),
                         ["x = *(s32 *)(p + 0x18);", "t[0x274 / 4] = f;", "q = (s32 *)(base + 0x200);"])


if __name__ == "__main__":
    unittest.main()
