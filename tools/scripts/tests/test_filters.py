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


class FillBranches(unittest.TestCase):
    """gcc 2.7.2 leaves some conditional branches in reorder mode; ASPSX fills their slots from
    below, else from the target, with an instruction whose register is dead on the other path."""

    def run_fill(self, text):
        return norm(aspsx_fill.fill_all_branches(lines(text)))

    TAIL = """
        $L26:
        j $L27
        li $2,0x00000001
        $L27:
        j $31
        nop  # DEBUG: branch/jump
        .end f"""

    def test_jump_target_slot_copied(self):  # func_8002DE2C: return 1 for arg0 == 10
        out = self.run_fill("""
            beq $4,$2,$L26
            nop  # DEBUG: branch/jump
            addu $3,$3,1
            li $2,0x00000007
            j $L27
            nop  # DEBUG: branch/jump""" + self.TAIL)
        self.assertEqual(out[:2], ["beq $4,$2,$L27", "li $2,0x00000001"])
        self.assertIn("$L26:", out)                         # the jump stays for other entries

    def test_register_read_on_the_fallthrough(self):  # func_8002D4F4: bne $16,$2 then sb $16
        src = """
            bne $16,$2,$L26
            nop  # DEBUG: branch/jump
            sb $16,D_80067884
            $L26:
            j $L27
            li $16,0x00000007
            $L27:
            j $31
            nop  # DEBUG: branch/jump
            .end f"""
        self.assertEqual(self.run_fill(src), norm(lines(src)))

    def test_target_instruction_copied_and_dropped(self):  # func_8002D4F4's $L157 (12 beqs)
        out = self.run_fill("""
            beq $16,$2,$L157
            nop  # DEBUG: branch/jump
            bgez $16,$L13
            li $2,-2
            j $L13
            li $16,-1
            $L157:
            li $2,0x00000001
            $L177:
            sw $2,0($4)
            $L13:
            j $31
            nop  # DEBUG: branch/jump
            .end f""")
        self.assertEqual(out[:2], ["beq $16,$2,$L177", "li $2,0x00000001"])
        self.assertNotIn("$L157:", out)                     # nothing reaches it now
        self.assertEqual(out.count("li $2,0x00000001"), 1)

    def test_from_below_past_a_label(self):  # func_8002D4F4: bgez $16,$L45 then $L175: li $2,-2
        out = self.run_fill("""
            bgez $16,$L45
            nop  # DEBUG: branch/jump
            $L175:
            li $2,-2
            beq $16,$2,$L13
            nop  # DEBUG: branch/jump
            $L45:
            li $2,0x00000001
            $L13:
            j $31
            nop  # DEBUG: branch/jump
            .end f""")
        self.assertEqual(out[:4], ["bgez $16,$L45", "$L175:", "li $2,-2", "beq $16,$2,$L13"])

    def test_a_load_does_not_prove_it_dead(self):  # func_8002E25C: bne $2,$0 then lbu $2
        src = """
            bne $2,$0,$L174
            nop  # DEBUG: branch/jump
            lbu $4,0($18)
            addu $18,$18,1
            lbu $2,0($18)
            j $31
            nop  # DEBUG: branch/jump
            $L174:
            li $2,0x00000020
            j $31
            nop  # DEBUG: branch/jump
            .end f"""
        self.assertEqual(self.run_fill(src), norm(lines(src)))

    def test_live_at_return(self):  # func_8002DE2C: $L187's sll $5 is never written before jr $31
        src = """
            bne $7,$2,$L187
            nop  # DEBUG: branch/jump
            sw $0,0($4)
            j $31
            nop  # DEBUG: branch/jump
            $L187:
            sll $5,$4,2
            sw $5,0($4)
            j $31
            nop  # DEBUG: branch/jump
            .end f"""
        self.assertEqual(self.run_fill(src), norm(lines(src)))

    def test_saved_register_dead_across_a_call(self):  # func_8002D4F4's state 15 (0x750)
        out = self.run_fill("""
            bne $16,$2,$L133
            nop  # DEBUG: branch/jump
            jal f2
            addu $4,$0,$zero
            $L133:
            j $L13
            li $16,0x0000000f
            $L13:
            j $31
            nop  # DEBUG: branch/jump
            .end f""")
        self.assertEqual(out[:2], ["bne $16,$2,$L13", "li $16,0x0000000f"])

    def test_filled_slot_untouched(self):
        src = """
            beq $4,$2,$L26
            addu $3,$3,1""" + self.TAIL
        self.assertEqual(self.run_fill(src), norm(lines(src)))


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
