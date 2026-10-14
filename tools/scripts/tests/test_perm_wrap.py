"""Tests for decomp.py's perm_wrap, which limits the permuter to the C of the blocks that differ
(`decomp.py permute NAME --differing`). Run: tools/dk make test-tools

A region must be whole statements: a PERM_RANDOMIZE that cuts an if from its else, or a
block from its closing brace, doesn't parse and the permuter run is wasted."""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

BODY = """void f(s32 a) {
    s32 x;
    s32 y = 0;

    x = a + 1;
    if (x > 2) {
        y = x;
        x = 0;
    } else {
        y = 2;
    }
    g(x,
      y);
    return;
}"""
L = BODY.split("\n")


def region(spans):
    out, wrapped = decomp.perm_wrap(BODY, spans)
    return out, [(lo, hi) for lo, hi in wrapped]


class PermWrap(unittest.TestCase):
    def test_a_statement_inside_a_block_stays_alone(self):
        _, w = region([(6, 6)])                     # `y = x;` inside the if
        self.assertEqual(w, [(6, 6)])

    def test_a_block_opened_but_not_closed_widens_down(self):
        _, w = region([(5, 6)])                     # `if (x > 2) {` + `y = x;`
        self.assertEqual(w, [(5, 10)])              # through the else's brace

    def test_else_pulls_in_its_if(self):
        _, w = region([(8, 9)])                     # `} else {` + `y = 2;`
        self.assertEqual(w, [(5, 10)])

    def test_finishes_a_statement_split_over_lines(self):
        _, w = region([(11, 11)])                   # `g(x,`
        self.assertEqual(w, [(11, 12)])

    def test_skips_declarations_and_the_closing_brace(self):
        _, w = region([(0, len(L) - 1)])
        self.assertEqual(w, [(4, 13)])

    def test_merges_overlapping_spans(self):
        _, w = region([(4, 4), (5, 6)])
        self.assertEqual(w, [(4, 10)])

    def test_output_wraps_the_region(self):
        out, _ = region([(4, 4)])
        self.assertIn("PERM_RANDOMIZE(\n    x = a + 1;\n)", out)


if __name__ == "__main__":
    unittest.main()
