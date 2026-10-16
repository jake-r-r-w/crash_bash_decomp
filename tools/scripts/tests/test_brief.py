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


if __name__ == "__main__":
    unittest.main()
