"""Tests for decomp.py sched's dump reading (schedx.py). Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import schedx  # noqa: E402

# trimmed from func_800B345C's cc1 -dS dump (2026-10-19)
SCHED = """;; Function f

;;	 -- basic block number 0 from 4 to 121 --
;; ready list initially:
;; 121 90 11

;; insn[  11]: priority =    1, ref_count =    0
;; insn[  90]: priority =    1, ref_count =    0
;; insn[ 121]: priority =    7, ref_count =    0
;; ready list at T-1: 121 (7) 90 (1) 11 (1), now 121 90 11
;; ready list at T-2: 90 (1) 11 (1), now 90 11
;; blocking insn 66 for 1 cycles
;; ready list at T-3: 11 (1), now 11

(insn 11 66 87 (set (reg/v:SI 81)
        (const_int 0)) 172 {movsi_internal2} (nil)
    (nil))

(insn 90 99 102 (set (reg/v:SI 83)
        (lo_sum:SI (reg:SI 130)
            (symbol_ref:SI ("D_800CA544")))) 162 {low} (nil)
    (nil))

(insn 121 119 0 (set (mem/s:SI (plus:SI (reg:SI 30 $fp)
                (const_int 24)) 0)
        (reg:SI 149)) 172 {movsi_internal2} (nil)
    (nil))
"""

LREG = """;; Function f
Register 80 used 7 times across 136 insns; crosses 1 call.
Register 81 used 4 times across 30 insns; crosses 1 call.
Register 82 used 6 times across 46 insns; crosses 1 call; pointer.
"""
GREG = """;; Function f
;; 3 regs to allocate: 81 82 80
;; Register dispositions:
80 in 19  81 in 17  82 in 18
"""


class Sched(unittest.TestCase):
    def test_patterns(self):
        p = schedx.insns(SCHED)
        self.assertEqual(p[11], "r81 = 0")
        self.assertEqual(p[90], "r83 = r130+%lo(D_800CA544)")
        self.assertEqual(p[121], "SI[$fp+24] = r149")

    def test_reasons(self):
        (block, order), = schedx.schedule(SCHED)
        self.assertEqual([u for u, _, _ in order], [11, 90, 121])   # final order: forward
        why = {u: w for u, _, w in order}
        self.assertEqual(why[121], "priority 7 > 1 of 90")
        self.assertEqual(why[90], "tie with 11: source order (the later insn goes first)")
        self.assertIn("only insn ready (blocked on latency: 66)", why[11])

    def test_allocation(self):
        out = schedx.allocation(LREG, GREG)
        self.assertEqual([p for p, *_ in out], [81, 82, 80])
        p, refs, live, pri, hard = out[0]
        self.assertEqual((refs, live, hard), (4, 30, 17))
        self.assertAlmostEqual(pri, 2 * 4 / 30)

    def test_report_names_variables(self):
        text = schedx.report({"lreg": LREG, "greg": GREG, "sched": SCHED}, "f", {"i": "s1", "h": "s2"})
        self.assertIn("-> s1  (i)", text)
        self.assertIn("r81 = 0", text)


if __name__ == "__main__":
    unittest.main()
