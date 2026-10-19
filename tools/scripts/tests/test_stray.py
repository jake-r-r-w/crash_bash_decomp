"""Tests for the gate's check that a match edits no other function (decomp.py stray_edits).
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

OLD = """s32 func_A(s32 x) {
    if (func_B(x) != 0) {
        return 1;
    }
    goto done;
done:
    return 0;
}

s32 func_B(s32 x) {
    return x;
}

INCLUDE_ASM("asm/nonmatchings/x", func_C);
"""

C = """s32 func_C(void) {
    return 2;
}
"""


def match_c(text):
    return text.replace('INCLUDE_ASM("asm/nonmatchings/x", func_C);\n', C)


class StrayEdits(unittest.TestCase):
    def test_plain_match_passes(self):
        self.assertEqual(decomp.stray_edits(OLD, match_c(OLD), {"func_C"}), [])

    def test_edit_to_another_function_is_flagged(self):
        # the ovl60 case: `goto done` -> `return 1` in a matched neighbour
        new = match_c(OLD).replace("    goto done;\n", "    return 1;\n")
        self.assertEqual(decomp.stray_edits(OLD, new, {"func_C"}), ["func_A"])

    def test_fixing_a_call_to_the_match_passes(self):
        old = OLD.replace("return x;", "return func_C();")
        new = match_c(old).replace("return func_C();", "return func_C() + 0;")
        self.assertEqual(decomp.stray_edits(old, new, {"func_C"}), [])

    def test_definition_after_a_call_is_found(self):
        # func_B is called (indented, `if (func_B(x) != 0) {`) before it is defined
        new = match_c(OLD).replace("return x;", "return x + 1;")
        self.assertEqual(decomp.stray_edits(OLD, new, {"func_C"}), ["func_B"])


if __name__ == "__main__":
    unittest.main()
