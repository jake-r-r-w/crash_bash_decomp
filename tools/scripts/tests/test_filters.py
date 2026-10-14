"""Tests for the compile pipeline's assembly filters: dmpsx.py (GTE placeholder words) and
aspsx_fill.py (ASPSX's delay-slot filling and load expansion). Run: tools/dk make test-tools

Each case is a small before/after taken from what cc1/maspsx emit and what the retail code has,
so a change to a filter that breaks a known rule fails here instead of as a diff somewhere."""
import io
import os
import sys
import unittest
from contextlib import redirect_stdout

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import aspsx_fill  # noqa: E402
import dmpsx  # noqa: E402


def lines(text):
    return [l.strip() for l in text.strip().split("\n")]


def norm(out):
    """Output lines as the pipeline writes them (an entry may hold several, joined by \\n)."""
    return [l.strip().replace("\t", " ") for l in "\n".join(out).split("\n") if l.strip()]


class LoadViaDest(unittest.TestCase):
    """maspsx expands `lX rD,sym(rI)` through $at; ASPSX (like GNU as) uses rD unless rD == rI."""

    def test_dest_differs_from_index(self):  # func_8002E0F0: lhu $a0,D_800678A2($v1)
        out = aspsx_fill.load_via_dest(lines("""
            .set noat
            lui $at,%hi(D_800678A2)
            addu $at,$at,$3
            lhu $4,%lo(D_800678A2)($at)
            .set at"""))
        self.assertEqual(norm(out)[1:4], ["lui $4,%hi(D_800678A2)", "addu $4,$4,$3",
                                          "lhu $4,%lo(D_800678A2)($4)"])

    def test_dest_is_index_keeps_at(self):
        src = lines("""
            lui $at,%hi(D_8006789E)
            addu $at,$at,$2
            lbu $2,%lo(D_8006789E)($at)""")
        self.assertEqual(aspsx_fill.load_via_dest(src), src)

    def test_every_load_width(self):
        for op in ("lw", "lh", "lhu", "lb", "lbu"):
            out = aspsx_fill.load_via_dest(lines(f"""
                lui $at,%hi(D)
                addu $at,$at,$5
                {op} $2,%lo(D)($at)"""))
            self.assertEqual(norm(out)[2], f"{op} $2,%lo(D)($2)", op)

    def test_stores_keep_at(self):  # a store has no destination register to borrow
        src = lines("""
            lui $at,%hi(D)
            addu $at,$at,$3
            sw $2,%lo(D)($at)""")
        self.assertEqual(aspsx_fill.load_via_dest(src), src)

    def test_other_symbol_untouched(self):
        src = lines("""
            lui $at,%hi(A)
            addu $at,$at,$3
            lw $2,%lo(B)($at)""")
        self.assertEqual(aspsx_fill.load_via_dest(src), src)


class FillJumpSlots(unittest.TestCase):
    """gcc 2.7.2 leaves j/jr in reorder mode; ASPSX moves the instruction before it into the slot."""

    def test_moves_previous_instruction(self):
        out = aspsx_fill.fill(lines("""
            addiu $2,$2,1
            j $31
            nop  # DEBUG: branch/jump"""))
        self.assertEqual(norm(out), ["j $31", "addiu $2,$2,1"])

    def test_symbol_store_moves_second_half(self):
        out = aspsx_fill.fill(lines("""
            sw $2,D_8006EA24
            j $L1
            nop  # DEBUG: branch/jump"""))
        self.assertEqual(norm(out), [".set noat", "lui $at,%hi(D_8006EA24)", "j $L1",
                                     "sw $2,%lo(D_8006EA24)($at)", ".set at"])

    def test_load_is_not_moved(self):
        src = lines("""
            lw $2,0($4)
            j $31
            nop  # DEBUG: branch/jump""")
        self.assertEqual(aspsx_fill.fill(src), src)

    def test_jump_target_register_not_overwritten(self):
        src = lines("""
            move $31,$2
            jr $31
            nop  # DEBUG: branch/jump""")
        self.assertEqual(aspsx_fill.fill(src), src)


class Dmpsx(unittest.TestCase):
    """inline_c.h placeholder words -> the real cop2 encodings (documented GTE opcodes)."""

    def run_filter(self, text):
        old = sys.stdin
        sys.stdin = io.StringIO(text)
        buf = io.StringIO()
        try:
            with redirect_stdout(buf):
                dmpsx.main()
        finally:
            sys.stdin = old
        return buf.getvalue()

    def test_known_commands(self):
        for word, op in [("0x0000007f", 0x4A180001),   # gte_rtps  -> RTPS
                         ("0x000000bf", 0x4A280030),   # gte_rtpt  -> RTPT
                         ("0x0000117f", 0x4B400006),   # gte_nclip -> NCLIP
                         ("0x000011bf", 0x4B58002D)]:  # gte_avsz3 -> AVSZ3
            out = self.run_filter(f"#APP\n\t.word {word}\n#NO_APP\n")
            self.assertIn(f".word 0x{op:08X}", out, word)

    def test_only_inside_inline_asm(self):
        text = "\t.word 0x0000007f\n"
        self.assertEqual(self.run_filter(text), text)

    def test_ordinary_words_untouched(self):
        text = "#APP\n\t.word 0x12345678\n#NO_APP\n"
        self.assertEqual(self.run_filter(text), text)


if __name__ == "__main__":
    unittest.main()
