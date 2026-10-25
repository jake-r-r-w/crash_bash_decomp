"""Tests for decomp.py protos (proto_find.py). Run: tools/dk make test-tools"""
import os
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import proto_find  # noqa: E402

JR_RA, NOP = 0x03E00008, 0
ADDIU_SP = 0x27BDFFE8           # addiu sp,sp,-24
LW = 0x8C820010                 # lw v0,16(a0)
SW = 0xAC820010                 # sw v0,16(a0)
BEQ = 0x10400003                # beq v0,zero,+3
JAL = 0x0C000000                # jal 0


def func(*body):
    return [ADDIU_SP, *body, JR_RA, NOP]


def pack(ws):
    return struct.pack(f"<{len(ws)}I", *ws)


class TestSplit(unittest.TestCase):
    def test_functions_skip_padding(self):
        ws = func(LW) + [0, 0] + func(SW, SW)
        self.assertEqual(proto_find.functions(ws), [(0, 4), (6, 11)])

    def test_tokens_ignore_registers(self):
        self.assertEqual(proto_find.token(0x8C820010), proto_find.token(0x8CA40008))
        self.assertNotEqual(proto_find.token(LW), proto_find.token(SW))


class TestRank(unittest.TestCase):
    def test_closest_first(self):
        target = func(LW, BEQ, JAL, SW)
        ws = func(SW, SW, SW, SW) + func(LW, BEQ, JAL, SW) + func(LW, BEQ, SW, SW)
        best = proto_find.rank(target, ws, 0x80010000, top=2)
        self.assertEqual(best[0], (1.0, 0x80010000 + 7 * 4, 7 * 4))
        self.assertEqual(best[1][1], 0x80010000 + 14 * 4)
        self.assertLess(best[1][0], 1.0)

    def test_delay_slot_nop_left_out_on_both_sides(self):
        target = func(LW, BEQ, JAL, SW)[:-1]          # a size that stops before the nop
        best = proto_find.rank(target, func(SW, SW) + func(LW, BEQ, JAL, SW), 0x80010000, top=1)
        self.assertEqual(best[0][0], 1.0)


SCAN = """# Some_Demo

- Exe: `files/SLUS_000.01`, load `0x80010000`, `0x1000` bytes
- DAT: `files/CRASHBSH/CRASHBSH.DAT`, 16384 bytes; index at exe `0x80010010`, 4 entries
- Engine: entry `0x1` (sector `0x2`, `0x800` bytes), load `0x80071764` (115 jal targets on function starts); cut to `engine_1.bin`
- Slot overlays: 2 entries load at `0x800B0000` (`0x2`, `0x3`)
"""


class TestDiscs(unittest.TestCase):
    def make(self, root):
        d = os.path.join(root, "Some_Demo")
        os.makedirs(os.path.join(d, "files", "CRASHBSH"))
        with open(os.path.join(d, "scan.md"), "w") as f:
            f.write(SCAN)
        target = func(LW, BEQ, JAL, SW)
        with open(os.path.join(d, "engine_1.bin"), "wb") as f:
            f.write(pack(target))
        # entries 2 and 3 are the slots: 3 holds the target after one other function
        slot2, slot3 = pack(func(SW, SW, SW, SW)), pack(func(SW, SW) + target)
        index = [(0, 0x10), (1, 0x10), (2, len(slot2)), (4, len(slot3))]
        exe = bytearray(0x800) + bytearray(0x10) + b"".join(struct.pack("<II", *e) for e in index)
        with open(os.path.join(d, "files", "SLUS_000.01"), "wb") as f:
            f.write(exe)
        dat = bytearray(8 * 2048)
        dat[2 * 2048:2 * 2048 + len(slot2)] = slot2
        dat[4 * 2048:4 * 2048 + len(slot3)] = slot3
        with open(os.path.join(d, "files", "CRASHBSH", "CRASHBSH.DAT"), "wb") as f:
            f.write(dat)
        return d, target

    def test_scan_md(self):
        with tempfile.TemporaryDirectory() as root:
            d, target = self.make(root)
            (dd, info), = proto_find.discs(root)
            self.assertEqual(dd, d)
            self.assertEqual(info["main"], (os.path.join(d, "files/SLUS_000.01"), 0x80010000))
            self.assertEqual(info["engine"], (os.path.join(d, "engine_1.bin"), 0x80071764))
            self.assertEqual(info["slot"][3:], (0x80010010, 4, 0x800B0000, [2, 3]))

    def test_engine(self):
        with tempfile.TemporaryDirectory() as root:
            _, target = self.make(root)
            out = proto_find.report(target, "engine", root)
            self.assertIn("1.000 at 0x80071764, 28 B (engine_1.bin +0x0)", out)

    def test_slot_entries_read_through_the_index(self):
        with tempfile.TemporaryDirectory() as root:
            _, target = self.make(root)
            out = proto_find.report(target, "slot", root)
            self.assertIn("1.000 at 0x800B0014, 28 B (entry 0x3 +0x14)", out)

    def test_missing_code(self):
        with tempfile.TemporaryDirectory() as root:
            d, target = self.make(root)
            os.remove(os.path.join(d, "engine_1.bin"))
            self.assertIn("no engine code", proto_find.report(target, "engine", root))

    def test_copy_in_one_disc(self):
        with tempfile.TemporaryDirectory() as root:
            _, target = self.make(root)
            d, ws, addr = proto_find.copy_in(target, "slot", "Demo", root)
            self.assertEqual((ws, addr), (target, 0x800B0014))
            self.assertIsNone(proto_find.copy_in(target, "slot", "Nowhere", root))


LUI = 0x3C028006                # lui v0,0x8006
ADDIU = 0x24451234              # addiu a1,v0,0x1234
LH = 0x84A30034                 # lh v1,52(a1)


class TestDiff(unittest.TestCase):
    """protos --diff: copies are aligned by opcode and register, immediates set aside."""

    def test_decode(self):
        self.assertEqual(proto_find.fmt_word(LH), "lh v1,0x34(a1)")
        self.assertEqual(proto_find.fmt_word(ADDIU), "addiu a1,v0,0x1234")
        self.assertEqual(proto_find.fmt_word(0x00031400), "sll v0,v1,0x10")
        self.assertEqual(proto_find.fmt_word(NOP), "nop")

    def test_immediates_ignored_registers_not(self):
        self.assertEqual(proto_find.key(LW), proto_find.key(0x8C820014))   # lw v0,20(a0)
        self.assertNotEqual(proto_find.key(LW), proto_find.key(0x8C830010))  # lw v1,16(a0)

    def test_hunks(self):
        a = [LUI, ADDIU, LH, JR_RA, NOP]
        b = [LUI, 0x24455678, LH, JR_RA, NOP]        # only the addiu's immediate differs
        self.assertEqual(proto_find.diff_lines(a, b)[-1].split(";")[0], "0 hunk(s)")
        c = [LUI, LH, JR_RA, NOP]                     # the copy drops the addiu
        out = proto_find.diff_lines(a, c)
        self.assertIn("--- delete retail 0x0004-0x0008 (1)  copy 0x0004-0x0004 (0)", out)
        self.assertIn("  -       4  addiu a1,v0,0x1234", out)

    def test_named_left_side(self):
        """--file puts a test build where retail was; the hunks name that side."""
        out = proto_find.diff_lines([LUI, ADDIU, LH], [LUI, LH], a_name="yours")
        self.assertIn("--- delete yours 0x0004-0x0008 (1)  copy 0x0004-0x0004 (0)", out)

    def test_word_lines_keep_immediates(self):
        """--words prints the copy itself, which the hunks only show around differences."""
        self.assertEqual(proto_find.word_lines([LUI, ADDIU]),
                         ["      0  3c028006  lui v0,0x8006",
                          "      4  24451234  addiu a1,v0,0x1234"])


if __name__ == "__main__":
    unittest.main()
