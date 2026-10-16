"""Tests for regions.py, the block-by-block scorer behind `decomp.py regions`.
Run: tools/dk make test-tools

The point of the scorer is that a difference stays in its own block: one extra instruction
early on must not turn every later block into a diff, as the positional count of `try` does."""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import regions  # noqa: E402

V0, V1, A0, A1, RA = 2, 3, 4, 5, 31


def addu(rd, rs, rt):
    return (rs << 21) | (rt << 16) | (rd << 11) | 0x21


def lw(rt, off, base):
    return (35 << 26) | (base << 21) | (rt << 16) | (off & 0xFFFF)


def beq(rs, rt, off):
    return (4 << 26) | (rs << 21) | (rt << 16) | (off & 0xFFFF)


JR_RA = (RA << 21) | 8
NOP = 0

# three blocks: [0,4) ends in a branch to 6, [4,6) falls through, [6,9) returns
TARGET = [lw(V0, 0, A0), addu(V1, V0, A1), beq(V1, 0, 3), NOP,
          addu(V0, V0, V0), addu(V0, V0, V1),
          lw(V1, 4, A0), JR_RA, addu(V0, V0, V1)]


def score(cw, tw=TARGET):
    return regions.score(tw, cw, [""] * len(tw), {}, vram=None)


def statuses(res):
    return [b["status"] for b in res["blocks"]]


class Regions(unittest.TestCase):
    def test_blocks(self):
        self.assertEqual(regions.blocks_of(TARGET), [(0, 4), (4, 6), (6, 9)])

    def test_identical(self):
        res = score(list(TARGET))
        self.assertEqual(statuses(res), ["ok", "ok", "ok"])
        self.assertEqual((res["exact"], res["len"], res["extra"]), (9, 9, []))

    def test_register_difference_stays_in_its_block(self):
        cw = list(TARGET)
        cw[4] = addu(V1, V0, V0)
        self.assertEqual(statuses(score(cw)), ["ok", "operands", "ok"])

    def test_inserted_instruction_stays_in_its_block(self):
        # an extra instruction in the middle block; the branch is moved to keep its target
        cw = TARGET[:2] + [beq(V1, 0, 4), NOP, addu(V0, V0, V0), addu(A0, A0, A0),
                           addu(V0, V0, V1)] + TARGET[6:]
        res = score(cw)
        self.assertEqual(statuses(res), ["ok", "differ", "ok"])
        self.assertEqual(res["extra"], [5])

    def test_branch_to_the_wrong_place(self):
        cw = list(TARGET)
        cw[2] = beq(V1, 0, 2)      # lands on index 5 instead of 6
        self.assertEqual(statuses(score(cw))[0], "operands")

    def test_relocated_field_is_masked(self):
        tw = [(15 << 26) | (V0 << 16) | 0x8006, JR_RA, NOP]      # lui v0, %hi(sym)
        cw = [(15 << 26) | (V0 << 16), JR_RA, NOP]
        res = regions.score(tw, cw, ["lui $v0, %hi(sym)", "jr $ra", "nop"], {0: ("R_MIPS_HI16", "sym")})
        self.assertEqual(statuses(res), ["ok"])


def kinds(cw, tw=TARGET):
    rows, counts = regions.diff_rows(tw, cw, [""] * len(tw), {}, score(cw, tw))
    return [r[0] for r in rows], counts


class DiffRows(unittest.TestCase):
    """`try`'s aligned diff: each row's kind, so one extra instruction is one `extra` row."""

    def test_identical(self):
        self.assertEqual(kinds(list(TARGET))[1], {"same": 9})

    def test_register(self):
        cw = list(TARGET)
        cw[4] = addu(V1, V0, V0)
        self.assertEqual(kinds(cw)[1], {"same": 8, "register": 1})

    def test_operand(self):
        cw = list(TARGET)
        cw[6] = lw(V1, 8, A0)                 # the offset, not a register
        self.assertEqual(kinds(cw)[1], {"same": 8, "operand": 1})

    def test_inserted_instruction_is_one_extra_row(self):
        cw = TARGET[:2] + [beq(V1, 0, 4), NOP, addu(V0, V0, V0), addu(A0, A0, A0),
                           addu(V0, V0, V1)] + TARGET[6:]
        rows, counts = kinds(cw)
        self.assertEqual(counts.get("extra"), 1)
        self.assertEqual(rows.index("extra"), 5)

    def test_swapped_statements_are_moved(self):
        tw = [lw(V0, 0, A0), lw(V1, 4, A0), addu(V0, V0, V1), JR_RA, NOP]
        cw = [lw(V1, 4, A0), lw(V0, 0, A0), addu(V0, V0, V1), JR_RA, NOP]
        self.assertEqual(kinds(cw, tw)[1], {"moved": 2, "same": 4})


if __name__ == "__main__":
    unittest.main()
