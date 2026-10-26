"""Tests for the shift build: the padded linker script, and the DAT laid out again around a grown entry.
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import dat  # noqa: E402
import shift  # noqa: E402

LD = """    .tank 0x800B32B4 : AT(tank_ROM_START) SUBALIGN(2)
    {
        build/asm/data/overlays/tank/tank_id.rodata.s.o(.rodata);
        build/src/overlays/tank/tank.c.o(.rodata);
        build/src/overlays/tank/tank.c.o(.text);
        build/asm/data/overlays/tank/tank_id.rodata.s.o(.data);
        build/src/overlays/tank/tank.c.o(.data);
        build/src/overlays/tank/tank.c.o(.bss);
    }
"""


class Neighbours(unittest.TestCase):
    def test_a_shared_address_beside_the_overlays_words_is_taken_in_a_chain(self):
        # Bash's per-mode file groups: 0x1C, 0x20 are only Bash's; 0x24 and 0x28 are addresses
        # another overlay also has, and 0x28 is beside 0x24 only
        done = [(0x1C, "jungle"), (0x20, "space")]
        maybe = [(0x24, "snow"), (0x28, "drains"), (0x40, "elsewhere")]
        left = shift.take_neighbours(done, maybe)
        self.assertEqual(left, [(0x40, "elsewhere")])
        self.assertEqual([m[1] for m in done], ["jungle", "space", "snow", "drains"])


class Columns(unittest.TestCase):
    def test_a_shared_address_in_a_taken_sequence_column_is_taken(self):
        # two characters' tables at 0x1000 and 0x2000; id 86 is taken for the first, so the
        # second's id-86 slot is the overlay's too; an id nothing took stays left
        tables = [0x1000, 0x2000]
        done = [(0x1000 + 4 * 86, 0, "CRA_seq_dinowin")]
        maybe = [(0x2000 + 4 * 86, 0, "COC_seq_dinowin"), (0x2000 + 4 * 87, 0, "other"), (0x3000, 0, "outside")]
        left = shift.take_columns(done, maybe, tables)
        self.assertEqual([m[2] for m in left], ["other", "outside"])
        self.assertIn((0x2000 + 4 * 86, 0, "COC_seq_dinowin"), done)


class PadScript(unittest.TestCase):
    def test_pads_before_each_c_section_and_not_the_id(self):
        t = shift.pad_script(LD, "tank", 0x40)
        lines = [l.strip() for l in t.splitlines()]
        for sect in (".rodata", ".text", ".data"):
            i = lines.index(f"build/src/overlays/tank/tank.c.o({sect});")
            self.assertTrue(lines[i - 1].startswith(". += 0x40;"), sect)
        i = lines.index("build/asm/data/overlays/tank/tank_id.rodata.s.o(.rodata);")
        self.assertFalse(lines[i - 1].startswith(". +="))
        self.assertEqual(t.count(". += 0x40;"), 3)

    def test_the_id_unit_in_c_stays_first(self):
        t = shift.pad_script(LD.replace("build/asm/data/overlays/tank/tank_id.rodata.s.o",
                                        "build/src/overlays/tank/tank_id.c.o"), "tank", 0x40)
        lines = [l.strip() for l in t.splitlines()]
        i = lines.index("build/src/overlays/tank/tank_id.c.o(.rodata);")
        self.assertFalse(lines[i - 1].startswith(". +="))
        self.assertTrue(lines[i + 1].startswith(". += 0x40;"))

    def test_a_script_without_c_data_is_refused(self):
        with self.assertRaises(ValueError):
            shift.pad_script(LD.replace("tank.c.o(.data)", "tank_data.data.s.o(.data)"), "tank", 0x40)


class Pointers(unittest.TestCase):
    def test_an_unrelocated_slot_address_is_reported(self):
        data = (0x10).to_bytes(4, "little") + (0x800B9E50).to_bytes(4, "little") + (0x800C0000).to_bytes(4, "little")
        # +0 a relocated offset, +4 a pointer written as a number, +8 a relocated pointer's addend
        self.assertEqual(shift.unmoved(data, {0, 8}), [4])

    def test_objdump_output_is_read(self):
        dump = ("\nContents of section .data:\n 0000 10000000 509e0b80 00000000 00000000  ....\n")
        self.assertEqual(bytes(shift.section_words(dump))[4:8], (0x800B9E50).to_bytes(4, "little"))
        rel = ("RELOCATION RECORDS FOR [.data]:\nOFFSET   TYPE              VALUE\n"
               "00000000 R_MIPS_32         .rodata\n00000008 R_MIPS_32         TankAnims\n")
        self.assertEqual(shift.reloc_offsets(rel), {".data": {0, 8}})


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
