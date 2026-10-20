"""Tests for decomp.py sched's dump reading (schedx.py). Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import schedx  # noqa: E402

# func_800BC274's block 5 from cc1 -dS (2026-10-20): the a0 load and the a1 add before
# jal func_8001CD04 tie, and with one memory insn in the block luid decides
SCHED = """;; Function f

;;	 -- basic block number 5 from 91 to 111 --
;; ready list initially:
;; 111

;; insn[  98]: priority =    1, ref_count =    1
;; insn[ 100]: priority =    1, ref_count =    1
;; insn[ 102]: priority =    1, ref_count =    1
;; insn[ 104]: priority =    2, ref_count =    3
;; insn[ 107]: priority =    2, ref_count =    1
;; insn[ 109]: priority =    2, ref_count =    1
;; insn[ 111]: priority = 2147483436, ref_count =    0
;; ready list at T-1: 111 (7fffff2c), now 111
;; ready list at T-2: 107 (2) 109 (2), now 109 107
;; ready list at T-3: 107 (2), now 107
;; ready list at T-4: 104 (2), now 104
;; ready list at T-5: 100 (1) 102 (7f000001), now 102 100
;; launching 98 before 102 with no stalls at T-6
;; ready list at T-6: 100 (1) 98 (1), now 100 98
;; ready list at T-7: 98 (1), now 98
;; total time = 7

(insn 98 96 100 (set (reg:SI 4 a0)
        (mem/s:SI (plus:SI (reg/v:SI 84)
                (const_int 108)))) 172 {movsi_internal2} (nil)
    (nil))

(insn 100 98 102 (set (reg:SI 5 a1)
        (plus:SI (reg/v:SI 84)
            (const_int 60))) 3 {addsi3_internal} (nil)
    (nil))

(insn 102 100 104 (set (reg:SI 6 a2)
        (plus:SI (reg/v:SI 84)
            (const_int 16))) 3 {addsi3_internal} (nil)
    (nil))

(call_insn 104 102 107 (parallel[
            (call (mem:SI (symbol_ref:SI ("func_8001CD04")))
                (const_int 16))
            (clobber (reg:SI 31 ra))
        ] ) 340 {call_internal1} (insn_list 98 (insn_list 100 (insn_list 102 (nil))))
    (expr_list:REG_DEAD (reg:SI 4 a0)
        (nil))
    (expr_list (use (reg:SI 4 a0))
        (nil)))

(insn 107 104 109 (set (reg:SI 4 a0)
        (reg/v:SI 86)) 172 {movsi_internal2} (insn_list:REG_DEP_ANTI 104 (nil))
    (nil))

(insn 109 107 111 (set (reg:SI 5 a1)
        (plus:SI (reg:SI 30 $fp)
            (const_int 16))) 3 {addsi3_internal} (insn_list:REG_DEP_ANTI 104 (nil))
    (nil))

(call_insn 111 109 113 (parallel[
            (call (mem:SI (symbol_ref:SI ("func_80015758")))
                (const_int 16))
            (clobber (reg:SI 31 ra))
        ] ) 340 {call_internal1} (insn_list:REG_DEP_ANTI 104 (insn_list 107 (insn_list 109 (nil))))
    (nil)
    (nil))
"""

# the end of func_800BC274's block 6 (trimmed): two loads in the block, so at T-2 load 118
# takes the place luid gave 121
HAZARD = """;; Function f

;;	 -- basic block number 6 from 113 to 139 --
;; ready list initially:
;; 139

;; insn[ 115]: priority =    1, ref_count =    1
;; insn[ 118]: priority =    1, ref_count =    1
;; insn[ 121]: priority =    1, ref_count =    1
;; insn[ 137]: priority =    1, ref_count =    1
;; insn[ 139]: priority = 2147483436, ref_count =    0
;; ready list at T-1: 139 (7fffff2c), now 139
;; ready list at T-2: 118 (1) 121 (1), now 121 118
;; insn 118 has a greater potential hazard, now 118 121
;; launching 137 before 118 with no stalls at T-3
;; ready list at T-3: 121 (1) 115 (7f000001) 137 (7f000001), now 137 115 121
;; ready list at T-4: 115 (7f000001) 121 (1), now 115 121
;; ready list at T-5: 121 (1), now 121
;; total time = 5

(insn 121 113 115 (set (reg/v:SI 89)
        (const_int 0)) 172 {movsi_internal2} (nil)
    (nil))

(insn 115 121 137 (set (reg:SI 122)
        (high:SI (symbol_ref:SI ("D_8005AA2C")))) 161 {high} (nil)
    (nil))

(insn 137 115 118 (set (reg:SI 132)
        (mem/s:SI (plus:SI (reg:SI 131)
                (const_int 36)))) 172 {movsi_internal2} (nil)
    (nil))

(insn 118 137 139 (set (reg/v:SI 88)
        (mem:SI (lo_sum:SI (reg:SI 122)
                (symbol_ref:SI ("D_8005AA2C"))))) 172 {movsi_internal2} (insn_list 115 (nil))
    (nil))

(jump_insn 139 118 141 (set (pc)
        (if_then_else (eq:SI (reg:SI 132)
                (const_int 0))
            (label_ref 160)
            (pc))) 243 {branch_zero} (insn_list:REG_DEP_ANTI 118 (insn_list:REG_DEP_ANTI 121 (insn_list 137 (nil))))
    (nil))
"""

# func_8001231C's block 12 (main.c, trimmed): at T-3 every insn is blocked, so at T-4 the last
# insn scheduled is still the store 115, which uses load 113's result: 113 is class 1
CLASS = """;; Function f

;;	 -- basic block number 12 from 106 to 132 --
;; ready list initially:
;; 132

;; insn[ 113]: priority =    2, ref_count =    2
;; insn[ 115]: priority =    3, ref_count =    2
;; insn[ 120]: priority =    1, ref_count =    2
;; insn[ 132]: priority = 2147483513, ref_count =    0
;; ready list at T-1: 132 (7fffff79), now 132
;; ready list at T-2: 115 (3), now 115
;; launching 120 before 115 with no stalls at T-3
;; ready list at T-3: 120 (7f000001)
;; blocking insn 120 for 1 cycles
;; launching 120 before 115 with no stalls at T-4
;; launching 113 before 115 with no stalls at T-4
;; ready list at T-4: 120 (7f000001) 113 (7f000001), now 120 113
;; ready list at T-5: 113 (7f000001), now 113
;; total time = 5

(insn 113 106 115 (set (reg:SI 98)
        (mem/s:SI (plus:SI (reg/v:SI 81)
                (const_int 28)))) 172 {movsi_internal2} (nil)
    (nil))

(insn 115 113 120 (set (mem/s:SI (plus:SI (reg/v:SI 80)
                (const_int 12)))
        (reg:SI 98)) 172 {movsi_internal2} (insn_list 113 (nil))
    (nil))

(insn 120 115 132 (set (reg:SI 101)
        (mem:SI (lo_sum:SI (reg:SI 100)
                (symbol_ref:SI ("D_80050F3C"))))) 172 {movsi_internal2} (nil)
    (nil))

(jump_insn 132 120 133 (set (pc)
        (label_ref 140)) 250 {jump} (nil)
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


def reasons(dump):
    (block, info, order), = schedx.schedule(dump)
    return info, [u for u, _, _ in order], {u: w for u, _, w in order}


class Sched(unittest.TestCase):
    def test_insns(self):
        p = schedx.insns(SCHED)
        self.assertEqual(p[98].pat, "$a0 = SI[r84+108]")
        self.assertEqual(p[104].pat, "call(SI[func_8001CD04], 16)")
        self.assertEqual((p[98].unit, p[100].unit, p[104].unit), ("load", None, None))
        self.assertEqual(p[104].links, [("data", 98), ("data", 100), ("data", 102)])
        self.assertEqual(p[107].links, [("REG_DEP_ANTI", 104)])
        self.assertEqual(schedx.insns(CLASS)[115].unit, "store")

    def test_luid_tie_with_one_memory_insn(self):
        info, order, why = reasons(SCHED)
        self.assertEqual(info, {"insns": 7, "mem": 1, "mul": 0})
        self.assertEqual(order, [98, 100, 102, 104, 107, 109, 111])   # final order: forward
        self.assertEqual(why[100], "tie with 98 at priority 1: luid, 100 is later in the RTL so it stays "
                                   "after (only 1 memory insn in the block: no potential hazard)")
        self.assertEqual(why[102], "birthing priority 7f000001 > 1 of 100")
        self.assertEqual(why[111], "block end (tail priority)")
        self.assertEqual(why[98], "only insn ready")

    def test_potential_hazard(self):
        info, order, why = reasons(HAZARD)
        self.assertEqual(info["mem"], 2)
        self.assertEqual(order, [121, 115, 137, 118, 139])
        self.assertTrue(why[118].startswith("greater potential hazard than 121, the rank order's pick "
                                            "(load over ALU"), why[118])
        self.assertIn("its potential hazard is greater too: load over ALU", why[137])
        # 115 vs 121 at T-4: no insn joined, so the order is T-3's sort's
        self.assertEqual(why[115], "birthing priority 7f000001 > 1 of 121")

    def test_class_after_a_blocked_step(self):
        info, order, why = reasons(CLASS)
        self.assertEqual(order, [113, 120, 115, 132])
        self.assertEqual(why[120], "class: 115 uses 113's result, latency 2 (class 1 < 3; luid agrees)")

    def test_order_kept_from_an_earlier_sort(self):
        st = schedx.steps(HAZARD.split("basic block number 6")[1])
        self.assertEqual([s.new for s in st], [[], [118, 121], [115, 137], [], []])
        self.assertEqual((st[1].pre, st[1].now, st[1].hazard), ([121, 118], [118, 121], 118))

    def test_priorities_are_hex(self):  # sched.c: fprintf (" %d (%x)", uid, priority)
        sched = SCHED.replace("107 (2) 109 (2), now 109 107", "107 (2) 109 (1c), now 109 107")
        self.assertEqual(reasons(sched)[2][109], "priority 28 > 2 of 107")

    def test_allocation(self):
        out = schedx.allocation(LREG, GREG)
        self.assertEqual([p for p, *_ in out], [81, 82, 80])
        p, refs, live, pri, hard = out[0]
        self.assertEqual((refs, live, hard), (4, 30, 17))
        self.assertAlmostEqual(pri, 2 * 4 / 30)

    def test_report(self):
        text = schedx.report({"lreg": LREG, "greg": GREG, "sched": SCHED}, "f", {"i": "s1", "h": "s2"})
        self.assertIn("-> s1  (i)", text)
        self.assertIn("block 5: 7 insns, 1 memory insn\n", text)
        self.assertRegex(text, r"98 T-7 +\$a0 = SI\[r84\+108\] +ld +only insn ready")


if __name__ == "__main__":
    unittest.main()
