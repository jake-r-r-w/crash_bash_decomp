"""Tests for decomp.py jtrace's dump reading and report (jtrace.py). Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import jtrace  # noqa: E402

# func_8007F314's case 0 exit from cc1 -dg (2026-10-23): the ret = 0 copy after the
# flags & 0x7000 test, its jump to the switch end, and the label after it
GREG = """;; Function g

(insn 3 2 5 (set (reg:SI 2 v0)
        (const_int 1)) 172 {movsi_internal2} (nil)
    (nil))

;; Function f

(jump_insn 463 461 467 (set (pc)
        (if_then_else (eq (reg:SI 2 v0)
                (const_int 0 [0x0]))
            (label_ref 472)
            (pc))) 164 {branch_zero} (nil)
    (expr_list:REG_DEAD (reg:SI 2 v0)
        (nil)))

(insn 467 463 469 (set (reg/v:SI 19 s3)
        (const_int 0 [0x0])) 172 {movsi_internal2} (nil)
    (nil))

(jump_insn 469 467 470 (set (pc)
        (label_ref 3269)) 237 {jump} (nil)
    (nil))

(barrier 470 469 472)

(code_label 472 470 474 61 "" "")

(call_insn 481 479 485 (parallel[
            (set (reg:SI 2 v0)
                (call (mem:SI (symbol_ref:SI ("func_8007EFC0")))
                    (const_int 16 [0x10])))
            (clobber (reg:SI 31 ra))
        ] ) -1 (nil)
    (nil)
    (nil))
"""


class TestInsns(unittest.TestCase):
    def test_summaries(self):
        n = jtrace.insns(GREG, "f")
        self.assertEqual(n[467], "(set s3 0)")
        self.assertEqual(n[469], "(set (pc) L3269)")
        self.assertEqual(n[463], "(set (pc) (if_then_else (eq v0 0) L472 (pc)))")
        self.assertEqual(n[472], "label $L61")
        self.assertEqual(n[470], "barrier")
        self.assertIn("func_8007EFC0", n[481])

    def test_other_function_left_out(self):
        self.assertNotIn(3, jtrace.insns(GREG, "f"))
        self.assertEqual(jtrace.insns(GREG, "g")[3], "(set v0 1)")
        self.assertEqual(jtrace.insns(GREG, "h"), {})


class TestReport(unittest.TestCase):
    RUNS = [[("fcj", "J469", "L3269", 1, None),
             ("fcj", "J469", "J1614", 2, ("i467", "i1612")),
             ("cross", "J469", "i467", "i1612"),
             ("redirect", "J469", "L3269", "L3843")]]

    def test_full(self):
        r = jtrace.report(self.RUNS, jtrace.insns(GREG, "f"))
        self.assertIn("jump2 run 1: 1 cross-jumps", r)
        self.assertIn("fcj J469 [(set (pc) L3269)] vs L3269 min 1 -> no", r)
        self.assertIn("-> i467 i1612", r)
        self.assertIn("CROSS J469 [(set (pc) L3269)]: delete from i467 [(set s3 0)], keep from i1612", r)
        self.assertIn("redirect J469: L3269 -> L3843", r)

    def test_cross_only(self):
        r = jtrace.report(self.RUNS, {}, cross_only=True)
        self.assertNotIn("fcj", r)
        self.assertNotIn("redirect", r)
        self.assertIn("CROSS J469: delete from i467, keep from i1612", r)


if __name__ == "__main__":
    unittest.main()
