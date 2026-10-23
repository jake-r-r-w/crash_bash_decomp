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


class TestDiscs(unittest.TestCase):
    def test_scan_md(self):
        with tempfile.TemporaryDirectory() as root:
            d = os.path.join(root, "Some_Demo")
            os.makedirs(d)
            with open(os.path.join(d, "scan.md"), "w") as f:
                f.write("# Some_Demo\n\n- Exe: `files/SLUS_000.01`, load `0x80010000`, `0x1000` bytes\n"
                        "- Engine: entry `0x1E5` (sector `0x4E28`, `0x4F158` bytes), load `0x80071764` "
                        "(115 jal targets on function starts); cut to `engine_1E5.bin`\n")
            (dd, bins), = proto_find.discs(root)
            self.assertEqual(dd, d)
            self.assertEqual(bins["main"], (os.path.join(d, "files/SLUS_000.01"), 0x80010000, 0x800))
            self.assertEqual(bins["engine"], (os.path.join(d, "engine_1E5.bin"), 0x80071764, 0))
            ws = func(LW, BEQ, JAL, SW)
            with open(os.path.join(d, "engine_1E5.bin"), "wb") as f:
                f.write(struct.pack(f"<{len(ws)}I", *ws))
            out = proto_find.report(ws, "engine", root)
            self.assertIn("1.000 at 0x80071764, 28 B (file 0x0)", out)
            self.assertIn("no main binary", proto_find.report(ws, "main", root))


if __name__ == "__main__":
    unittest.main()
