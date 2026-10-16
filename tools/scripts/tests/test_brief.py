"""Tests for what `next` prints: the compact asm listing (decomp.py asm_listing).
Run: tools/dk make test-tools"""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

ASM = r""".section .rodata
.align 2
dlabel jtbl_8004C894
    /* 3D094 8004C894 18430180 */ .word .L8001000C
    /* 3D098 8004C898 2C430180 */ .word .L80010010
enddlabel jtbl_8004C894
dlabel D_8004C8A0
    /* 3D0A0 8004C8A0 */ .asciz "HELLO"
    /* 48454C4C4F000000 */
enddlabel D_8004C8A0

.section .text
glabel func_80010000
    /* 800 80010000 D0FFBD27 */  addiu      $sp, $sp, -0x30
    /* 804 80010004 00000000 */  nop
    /* 808 80010008 00000000 */  nop
  .L8001000C:
    /* 80C 8001000C 00000000 */  nop
  .L80010010:
    /* 810 80010010 0800E003 */  jr         $ra
    /* 814 80010014 00000000 */   nop
endlabel func_80010000
"""


class AsmListing(unittest.TestCase):
    def listing(self, limit=50, start=0):
        with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as fh:
            fh.write(ASM)
        try:
            return decomp.asm_listing(fh.name, limit, start, "func_80010000")
        finally:
            os.unlink(fh.name)

    def test_offsets_labels_rodata(self):
        text, rodata = self.listing()
        self.assertEqual(rodata, ["jtbl_8004C894", "D_8004C8A0"])
        self.assertEqual(text[0], "     0  addiu $sp, $sp, -0x30")
        self.assertIn("  .L8001000C: (+0xc)", text)
        self.assertIn("    10  jr $ra", text)
        self.assertIn("  jtbl_8004C894: jump table, 2 cases -> +0xc, +0x10", text)
        self.assertIn('  D_8004C8A0: .asciz "HELLO"', text)

    def test_cut_and_from(self):
        text, _ = self.listing(limit=2)
        self.assertEqual(text[2], "    ... 6 more lines (decomp.py asm func_80010000 --from 0x8)")
        text, _ = self.listing(start=0x10)
        self.assertEqual(text[0], "  .L80010010: (+0x10)")


class StallNote(unittest.TestCase):
    def test_no_better_in_n_tries(self):
        cwd = os.getcwd()
        with tempfile.TemporaryDirectory() as d:
            os.chdir(d)
            try:
                e = {"size": 128}                      # 5 tries without progress (4 + 128 // 128)
                scores = [(50, 2, 40), (60, 3, 45)] + [(55, 2, 41)] * 5
                notes = []
                for i, (paired, ok, exact) in enumerate(scores, 1):
                    decomp.LAST_SCORE["k"] = (paired, ok, exact, 100, 10)
                    notes.append(decomp.stall_note("k", e, i))
                self.assertEqual(notes[:6], [None] * 6)
                self.assertRegex(notes[6], r"no better in 5 tries: try 2 was the closest, paired 60/100")
            finally:
                os.chdir(cwd)


class SplitPoints(unittest.TestCase):
    JR_RA, NOP = 0x03E00008, 0
    FRAME = 0x27BDFFE8                     # addiu $sp, $sp, -0x18

    def test_second_function_after_jr(self):
        words = [self.FRAME, self.NOP, self.JR_RA, self.NOP, self.NOP, self.FRAME, self.JR_RA, self.NOP]
        self.assertEqual(decomp.split_points(words, []), [(5, "a stack frame set up after a jump")])

    def test_code_a_branch_reaches_is_not_a_split(self):
        beq_to_4 = (4 << 26) | 1                 # beq $zero, $zero, +1: from 2 lands on 4
        words = [self.NOP, self.NOP, beq_to_4, self.NOP, self.JR_RA, self.NOP, 0x24020001, self.JR_RA, self.NOP]
        self.assertEqual(decomp.split_points(words, [6]), [])
        self.assertEqual(decomp.split_points(words, []), [(6, "code after a jump that nothing reaches")])


if __name__ == "__main__":
    unittest.main()
