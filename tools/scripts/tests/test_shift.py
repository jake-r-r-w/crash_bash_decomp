"""Tests for the shift build: the padded linker script, and the DAT laid out again around a grown entry.
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import dat  # noqa: E402
import shift  # noqa: E402

LD = """    .ovl5f 0x800B32B4 : AT(ovl5f_ROM_START) SUBALIGN(2)
    {
        build/asm/data/overlays/ovl5f/ovl5f_id.rodata.s.o(.rodata);
        build/src/overlays/ovl5f/ovl5f.c.o(.rodata);
        build/src/overlays/ovl5f/ovl5f.c.o(.text);
        build/asm/data/overlays/ovl5f/ovl5f_id.rodata.s.o(.data);
        build/src/overlays/ovl5f/ovl5f.c.o(.data);
        build/src/overlays/ovl5f/ovl5f.c.o(.bss);
    }
"""


class PadScript(unittest.TestCase):
    def test_pads_before_each_c_section_and_not_the_id(self):
        t = shift.pad_script(LD, "ovl5f", 0x40)
        lines = [l.strip() for l in t.splitlines()]
        for sect in (".rodata", ".text", ".data"):
            i = lines.index(f"build/src/overlays/ovl5f/ovl5f.c.o({sect});")
            self.assertTrue(lines[i - 1].startswith(". += 0x40;"), sect)
        i = lines.index("build/asm/data/overlays/ovl5f/ovl5f_id.rodata.s.o(.rodata);")
        self.assertFalse(lines[i - 1].startswith(". +="))
        self.assertEqual(t.count(". += 0x40;"), 3)

    def test_a_script_without_c_data_is_refused(self):
        with self.assertRaises(ValueError):
            shift.pad_script(LD.replace("ovl5f.c.o(.data)", "ovl5f_data.data.s.o(.data)"), "ovl5f", 0x40)


class Relayout(unittest.TestCase):
    S = dat.SECTOR

    def test_a_grown_entry_that_fits_its_sectors_moves_nothing(self):
        new, extra = dat.relayout([(0, 100), (2, 3000), (5, 10)], {1: 3 * self.S - 1})
        self.assertEqual((new, extra), ([(0, 100), (2, 3 * self.S - 1), (5, 10)], 0))

    def test_entries_after_a_grown_one_move_down_in_order(self):
        new, extra = dat.relayout([(0, 100), (2, 3000), (4, 10)], {1: 2 * self.S + 1})
        self.assertEqual((new, extra), ([(0, 100), (2, 2 * self.S + 1), (5, 10)], 1))

    def test_the_relaid_dat_keeps_every_entry_and_the_filler(self):
        old = [(0, 3), (1, 5)]
        files = [b"AAA", b"B" * (self.S + 10)]
        filler = b"x" * (self.S - 3) + b"y" * (self.S - 5)
        new, extra = dat.relayout(old, {1: len(files[1])}, 2 * self.S)
        out = dat.join_relaid(files, filler, old, new, 2 * self.S, (2 + extra) * self.S)
        self.assertEqual(out[:3], b"AAA")
        self.assertEqual(out[3:self.S], b"x" * (self.S - 3))
        self.assertEqual(out[self.S:2 * self.S + 10], files[1])
        self.assertEqual(len(out), 3 * self.S)


if __name__ == "__main__":
    unittest.main()
