"""Tests for decomp.py idiom (idiom.py): instruction patterns with placeholders, and the -g line
map it reads the C lines from. Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402
import idiom  # noqa: E402

CODE = [idiom.insn(t) for t in [
    "andi $v0, $t0, 0x3",
    "addu $a3, $v0, $zero",
    "sll $v0, $a3, 3",
    "subu $v0, $v0, $a3",
    "lbu $v0, 0x5C($v0)",
    "lui $v0, %hi(D_8005A614)",
    "addiu $t0, $v0, %lo(D_8005A614)",
    "sw $a2, %lo(D_8009D6F4)($v0)",
    "addiu $a0, $zero, 0x40",
    "lui $a1, (0x51EB851F >> 16)",
]]


def hits(pat, code=CODE, gap=8):
    return [(s, e, b) for s, e, b in idiom.find(idiom.pattern(pat), code, gap)]


class TestPattern(unittest.TestCase):
    def test_canonical_forms(self):
        self.assertEqual(idiom.insn("addu $a3, $v0, $zero"), ("move", ["$a3", "$v0"]))
        self.assertEqual(idiom.insn("addu $a3, $zero, $v0"), ("move", ["$a3", "$v0"]))
        self.assertEqual(idiom.insn("addiu $a0, $zero, 0x40"), ("li", ["$a0", "0x40"]))
        self.assertEqual(idiom.insn("lbu $v0, 0x5C($v0)"), ("lbu", ["$v0", "0x5C", "@$v0"]))
        self.assertEqual(idiom.insn("sw $a2, %lo(D_1)($v0)"), ("sw", ["$a2", "%lo(D_1)", "@$v0"]))
        self.assertEqual(idiom.insn("lui $v0, %hi(D_1)"), ("lui", ["$v0", "%hi(D_1)"]))

    def test_copy_then_shift_binds_registers_and_values(self):
        (s, e, b), = hits("move A, B; sll B, A, N")
        self.assertEqual((s, e), (1, 3))
        self.assertEqual(b, {"A": "$a3", "B": "$v0", "N": "3"})

    def test_placeholders_bound_to_registers_differ(self):
        self.assertEqual(hits("sll A, B, 3; subu A, A, A"), [])
        self.assertEqual(len(hits("sll A, B, 3; subu A, A, B")), 1)

    def test_the_written_form_does_not_matter(self):
        self.assertEqual(len(hits("addu A, B, $zero; sll B, A, 3")), 1)
        self.assertEqual(len(hits("move a3, v0")), 1)
        self.assertEqual(len(hits("li A, 64")), 1)

    def test_memory_operands_and_globs(self):
        self.assertEqual(hits("lbu A, N(A)")[0][2], {"A": "$v0", "N": "0x5C"})
        self.assertEqual(len(hits("lui A, %hi(D_8005*); addiu B, A, %lo(*)")), 1)
        self.assertEqual(len(hits("sw *, %lo(*)(A)")), 1)
        self.assertEqual(hits("lbu A, B"), [])            # a memory operand is two operands

    def test_gaps_and_wildcards(self):
        self.assertEqual(hits("andi A, *, 3; ...; lbu A, *(A)")[0][:2], (0, 5))
        self.assertEqual(hits("andi A, *, 3; ...; lbu A, *(A)", gap=2), [])
        self.assertEqual(len(hits("* $v0, $a3, 3")), 1)

    def test_parenthesized_immediates_are_one_operand(self):
        self.assertEqual(hits("lui A, *")[-1][2], {"A": "$a1"})

    def test_bad_patterns(self):
        with self.assertRaises(ValueError):
            idiom.pattern("...; sll A, B, 3")


OBJDUMP = """
Disassembly of section .text:

00000000 <f>:
f():
/work/src/x.c:10
   0:	27bdffe8 	addiu	sp,sp,-24
   4:	afb00010 	sw	s0,16(sp)

00000008 <LM5>:
/work/src/x.c:12
   8:	00102080 	sll	a0,s0,0x2

0000000c <LM6>:
/work/include/h.h:3
   c:	00902021 	addu	a0,a0,s0

00000010 <g>:
g():
/work/src/x.c:20
  10:	03e00008 	jr	ra
"""


class TestLines(unittest.TestCase):
    def test_line_labels_are_not_functions(self):
        out = decomp.objdump_lines(OBJDUMP, "x.c")
        self.assertEqual(out, {"f": {0: 10, 1: 10, 2: 12, 3: None}, "g": {0: 20}})


if __name__ == "__main__":
    unittest.main()
