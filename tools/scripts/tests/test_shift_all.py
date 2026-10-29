"""Tests for shift_all.py's pure parts: the scripts it rewrites and the addresses it moves.
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import shift_all  # noqa: E402

SCRIPT = """    .main 0x80010000 : AT(main_ROM_START) SUBALIGN(4)
    {
        build/asm/main/psyq/2mbyte.s.o(.text);
        build/src/main/main.c.o(.text);
        build/src/main/dbase.c.o(.text);
        build/src/main/main.c.o(.rodata);
        build/src/main/main.c.o(.data);
    }
    psyq_libgpu_sys_bss 0x8006EA28 (NOLOAD) : SUBALIGN(1) { build/psyq/libgpu/sys.o(.bss) }
    memcpy = 0x8003308C;
    _gp = 0x8006E9EC;
"""


class Scripts(unittest.TestCase):
    def test_provide_keeps_definitions_and_overrides(self):
        t = shift_all.provide("a = 0x80010000;\nPROVIDE(b = 0x80020000);\n_gp = 0x8006E9EC;\n", {"b": 0x80030000})
        self.assertEqual(t, "PROVIDE(a = 0x80010000);\nb = 0x80030000;\n_gp = 0x8006E9EC;\n")

    def test_retail_names(self):
        self.assertEqual(shift_all.retail_names([]), {})

    def test_pad_before_the_first_c_input_of_each_section(self):
        t = shift_all.pad_first(SCRIPT, r"build/src/main/\w+\.c\.o", 0x40)
        lines = [l.strip() for l in t.splitlines()]
        for sect in (".text", ".rodata", ".data"):
            i = lines.index(f"build/src/main/main.c.o({sect});")
            self.assertEqual(lines[i - 1], ". += 0x40;   /* shift_all.py */")
        self.assertEqual(t.count(". += 0x40"), 3)                    # only the first of each
        self.assertNotIn(". += 0x40;   /* shift_all.py */\n        build/asm", t)
        with self.assertRaises(ValueError):
            shift_all.pad_first("nothing here", r"build/src/main/\w+\.c\.o", 0x40)


class Moving(unittest.TestCase):
    def build(self, pads=(0x40, 0x40, 0x40)):
        b = shift_all.Build.__new__(shift_all.Build)
        b.pads = {"main": pads[0], "engine": pads[1]}
        b.pad_slot = pads[2]
        b.engine_base = shift_all.ENGINE_BASE + 3 * pads[0]
        b.slot_base = shift_all.SLOT_BASE + 3 * pads[0] + 3 * pads[1]
        b.slot_heap = shift_all.SLOT_HEAP + b.slot_base - shift_all.SLOT_BASE + 3 * pads[2]
        b.slots = ["tank"]
        b._points = {"main": [0x80010000, 0x8004C56C, 0x8004CEFC], "engine": [0x80078CA4, 0x80090000, 0x800A0000],
                     "tank": [0x800B32B8, 0x800B3B24, 0x800C0000]}
        return b

    def test_moved_counts_the_pads_at_or_below(self):
        b = self.build()
        self.assertEqual(b.moved("main", 0x80010000), 0x80010040)    # the first pad is before it
        self.assertEqual(b.moved("main", 0x8004C56C), 0x8004C5EC)
        self.assertEqual(b.moved("main", 0x80078000), 0x800780C0)    # past all three: the .bss
        self.assertEqual(b.moved("engine", 0x80078C90), 0x80078D50)  # the entry table: the base only
        self.assertEqual(b.moved("engine", 0x800AEC60), 0x800AEDE0)  # ModeTextBuffer, in the .bss
        self.assertEqual(b.moved("tank", 0x800B32B4), 0x800B3434)    # the id word: the slot's base

    def test_one_binary_at_a_time(self):
        b = self.build((0, 0x40, 0))
        self.assertEqual(b.moved("main", 0x80078000), 0x80078000)
        self.assertEqual(b.engine_base, shift_all.ENGINE_BASE)
        self.assertEqual(b.slot_base, shift_all.SLOT_BASE + 0xC0)

    def test_regions(self):
        r = shift_all.Build.region
        self.assertEqual([r(0x80020000), r(0x80080000), r(0x800C0000), r(0x801F0000)], ["main", "engine", "slot", None])


class Startup(unittest.TestCase):
    def test_startup_names_what_it_uses(self):
        for name in ("StartupRa", "HeapStart", "_stacksize", "ExportsInit", "InitHeap", "machine_init"):
            self.assertIn(name, shift_all.STARTUP)
        self.assertIn(".word   0x0000004D", shift_all.STARTUP)


if __name__ == "__main__":
    unittest.main()
