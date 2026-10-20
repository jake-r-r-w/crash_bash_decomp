"""Tests for finding a parked function's C under #ifdef NON_MATCHING (decomp.py parked_c), which
sched, regions and vars compile with -DNON_MATCHING. Run: tools/dk make test-tools"""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

SRC = """INCLUDE_ASM("asm/nonmatchings/x", func_A);

#ifdef NON_MATCHING
s32 func_B(s32 x) {
#if 0
    x++;
#else
    x--;
#endif
    return x;
}
#else
INCLUDE_ASM("asm/nonmatchings/x", func_B);
#endif

#ifndef NON_MATCHING
void func_C(void) {
}
#endif
"""


class Parked(unittest.TestCase):
    def test_blocks_keep_nested_ifs(self):
        (c, asm), = decomp.non_matching_blocks(SRC)
        self.assertIn("x--;", c)
        self.assertIn("#endif\n    return x;", c)
        self.assertEqual(asm.strip(), 'INCLUDE_ASM("asm/nonmatchings/x", func_B);')

    def test_parked_c(self):
        with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as f:
            f.write(SRC)
        try:
            self.assertTrue(decomp.parked_c(f.name, "func_B"))
            self.assertFalse(decomp.parked_c(f.name, "func_A"))      # INCLUDE_ASM only
            self.assertFalse(decomp.parked_c(f.name, "func_C"))      # C, not parked
        finally:
            os.unlink(f.name)


if __name__ == "__main__":
    unittest.main()
