"""splat_quiet.py: drop the expected lost-symbol notices, keep real ones."""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import splat_quiet  # noqa: E402

NOTICE = [
    "",
    "Error: Unable to determine a segment for the following user-declared symbols.",
    "  Try specifying the segment they belong to with 'segment:segment_name' in your symbol_addrs file.",
    "  user attribute instead.",
    "    GameInfo (Vram: 0x8005A614). Suspected segments: None",
    "    Typo (Vram: 0x800B9999). Suspected segments: None",
    "",
    "Split 19 KB (64.05%) in defined segments",
]


class Quiet(unittest.TestCase):
    def test_drops_only_foreign_symbols(self):
        out = splat_quiet.quiet(NOTICE, {"GameInfo"})
        self.assertIn("    Typo (Vram: 0x800B9999). Suspected segments: None", out)
        self.assertNotIn("    GameInfo (Vram: 0x8005A614). Suspected segments: None", out)
        self.assertIn(NOTICE[1], out)

    def test_drops_the_whole_notice_when_all_are_foreign(self):
        out = splat_quiet.quiet(NOTICE, {"GameInfo", "Typo"})
        self.assertEqual(out, ["", "", "Split 19 KB (64.05%) in defined segments"])

    def test_drops_the_known_alias(self):
        lines = ["WARNING: The user declared symbol 'EngineEntry' (Vram 0x80078C90, size None, segment 'None')",
                 "  overlaps with the previously defined 'HeapStart' (Vram 0x80078C90, size None, segment 'None')",
                 "WARNING: The user declared symbol 'A' (Vram 0x1, size None, segment 'None')",
                 "  overlaps with the previously defined 'B' (Vram 0x1, size None, segment 'None')"]
        self.assertEqual(splat_quiet.quiet(lines, set()), lines[2:])


if __name__ == "__main__":
    unittest.main()
