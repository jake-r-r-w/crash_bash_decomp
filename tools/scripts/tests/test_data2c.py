"""Tests for data2c.py: struct layouts from header text and how values are written.
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import data2c  # noqa: E402

HDR = """
typedef struct Ambient_data Ambient_data;
struct Ambient_data {
    /* 0x00 */ u16 min_delay;
    /* 0x02 */ u16 max_delay;
    /* 0x08 */ u16 Random[8];   /* the comment's offset isn't trusted: layout is computed */
};
typedef struct { u16 v, pad; } RockTblEnt;
typedef struct Mixed {
    u8 a;
    s32 b;
    s16 c[3];
    NAS *p;
} Mixed;
typedef u16 hero_seq;
"""


class Layouts(unittest.TestCase):
    def setUp(self):
        self.t = data2c.Types([HDR])

    def test_scalars_and_aliases(self):
        self.assertEqual(self.t.layout("s32"), (4, 4))
        self.assertEqual(self.t.layout("hero_seq"), (2, 2))
        self.assertEqual(self.t.layout("anything", ptr=1), (4, 4))

    def test_struct_from_tag_and_typedef(self):
        self.assertEqual(self.t.layout("Ambient_data"), (20, 2))
        self.assertEqual(self.t.layout("RockTblEnt"), (4, 2))

    def test_padding_and_alignment(self):
        ms, kind = self.t.members("Mixed")
        self.assertEqual([(m[0], m[4]) for m in ms], [("a", 0), ("b", 4), ("c", 8), ("p", 16)])
        self.assertEqual(self.t.layout("Mixed"), (20, 4))

    def test_unknown_type(self):
        self.assertIsNone(self.t.layout("NoSuchType"))


class Values(unittest.TestCase):
    def setUp(self):
        c = data2c.Converter.__new__(data2c.Converter)
        c.vram, c.end, c.syms, c.funcs, c.decls = 0x800B32B4, 0x800CB294, {}, {}, {}
        c.symbolic, c.provide, c.externs, c.base_rom = set(), {}, set(), 0
        c.srcs = c.hdrs = ""
        self.c = c

    def word(self, v, signed=True, ptr_ok=True, local=()):
        return self.c.scalar(4, signed, 0, v.to_bytes(4, "little"), ptr_ok, "s32", list(local))

    def test_small_and_negative(self):
        self.assertEqual(self.word(5), "5")
        self.assertEqual(self.word(0x1C), "0x1C")
        self.assertEqual(self.word(0xFFFFFFF0), "-0x10")

    def test_flags_stay_hex(self):
        self.assertEqual(self.word(0x80070000), "(s32)0x80070000")

    def test_a_pointer_into_the_overlay_is_a_symbol(self):
        local = [(0x800C8C14, "D_800C8C14")]
        self.assertEqual(self.word(0x800C8C14, local=local), "(s32)&D_800C8C14")
        self.assertEqual(self.word(0x800C8C1C, local=local), "(s32)(u8 *)&D_800C8C14 + 0x8")

    def test_a_pointer_into_the_exe_needs_a_provide(self):
        self.assertEqual(self.word(0x8004F2A8), "(s32)&D_8004F2A8")
        self.assertEqual(self.c.provide, {"D_8004F2A8": 0x8004F2A8})
        self.assertIn("D_8004F2A8", self.c.externs)

    def test_braces_wrap_long_lists(self):
        short = data2c.Converter._braces(["1", "2"], 0)
        self.assertEqual(short, "{ 1, 2 }")
        long = data2c.Converter._braces([f"0x{i:X}" for i in range(0x1000, 0x1040)], 0)
        self.assertTrue(long.startswith("{\n    0x1000,"))


class FunctionPointers(unittest.TestCase):
    def test_a_function_pointer_field_is_a_pointer(self):
        t = data2c.Types(["typedef struct process_info {\n    void (*init)(void);\n    void (*update)(void);\n"
                          "    u32 *heap;\n} process_info;"])
        self.assertEqual(t.layout("process_info"), (12, 4))
        self.assertEqual([f[1] for f in t.fields("process_info")], [data2c.FN, data2c.FN, "u32"])


class Declarations(unittest.TestCase):
    def test_several_declarators_on_one_line(self):
        d = data2c.declarations(["extern u8 D_800D3510[], D_800D3524[];\nextern hero_seq D_800D3948[], *D_X;\n"
                                 "extern s32 D_A;\n"])
        self.assertEqual(d["D_800D3524"], ("u8", 0, [None]))
        self.assertEqual(d["D_800D3948"], ("hero_seq", 0, [None]))
        self.assertEqual(d["D_X"], ("hero_seq", 1, []))
        self.assertEqual(d["D_A"], ("s32", 0, []))


class WordFields(unittest.TestCase):
    def test_only_4_byte_fields_can_hold_a_pointer(self):
        c = data2c.Converter.__new__(data2c.Converter)
        c.types = data2c.Types(["typedef struct { s32 a; u8 b[8]; NAS *p; s16 h[2]; } Card;"])
        out = set()
        c.word_fields("Card", 0, [2], 0, out)
        self.assertEqual(out, {0, 12, 20, 32})


class Alignment(unittest.TestCase):
    def setUp(self):
        c = data2c.Converter.__new__(data2c.Converter)
        c.decls = {"D_A": ("u16", 0, []), "D_B": ("u16", 0, [None]), "D_C": ("Ambient_data", 0, []),
                   "D_P": ("u8", 1, [])}
        self.c = c

    def test_arrays_and_structs_are_word_aligned(self):
        # mips.h DATA_ALIGNMENT: an array, struct or union is word-aligned; a scalar keeps its own
        self.assertEqual(self.c.align_of("D_A"), 2)
        self.assertEqual(self.c.align_of("D_B"), 4)
        self.assertEqual(self.c.align_of("D_C"), 4)
        self.assertEqual(self.c.align_of("D_P"), 4)
        self.assertEqual(self.c.align_of("D_undeclared"), 4)

    def test_a_gap_before_a_halfword_is_a_rest(self):
        # Bash's u16 D_800C3B34, then 2 zero bytes, then u16 D_800C3B38: the gap can't be padding
        c = data2c.Converter.__new__(data2c.Converter)
        c.vram, c.end, c.syms, c.funcs, c.symbolic, c.provide, c.externs = 0x800B32B4, 0x800CB294, {}, {}, set(), {}, set()
        c.srcs = c.hdrs = ""
        c.decls = {"D_800C3B34": ("u16", 0, [])}
        c.types = data2c.Types([HDR])
        c.bin = bytes(0x10)
        c.rom = lambda va: 0
        out = c.define("D_800C3B34", 0x800C3B34, 4, {"short"}, [], next_align=2)
        self.assertEqual(out, "u16 D_800C3B34 = 0;\ns16 D_800C3B36 = 0;")
        # before a word-aligned object the same 2 bytes are padding
        self.assertEqual(c.define("D_800C3B34", 0x800C3B34, 4, {"short"}, [], next_align=4), "u16 D_800C3B34 = 0;")


class OtherSlots(unittest.TestCase):
    def test_only_another_slot_overlays_files_are_left_out(self):
        slots = ["bash", "bonk", "n2o", "tank", "titles"]

        def other(p):
            return data2c.other_slot(p, "titles", slots)
        self.assertTrue(other("include/bash/bonk_ovl.h"))
        self.assertTrue(other("src/overlays/n2o/800bbdb8.c"))
        self.assertTrue(other("include/bash/n2o_800bbdb8.h"))
        self.assertTrue(other("include/bash/n2o_nox.h"))
        self.assertTrue(other("src/overlays/bash/bash.c"))
        self.assertFalse(other("include/bash/titles_ovl.h"))
        self.assertFalse(other("src/overlays/titles/titles.c"))
        self.assertFalse(other("include/bash/8008e27c.h"))
        self.assertFalse(other("src/overlays/engine/asttable.c"))
        # the leak's headers are shared, not an overlay's own
        self.assertFalse(other("include/bash/tank.h"))
        self.assertFalse(other("include/bash/bash.h"))


if __name__ == "__main__":
    unittest.main()
