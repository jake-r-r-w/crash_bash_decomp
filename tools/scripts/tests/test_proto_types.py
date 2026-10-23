"""Tests for proto_types.py: g++ 2.x parameter types and which ones replace the C's.
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import proto_types  # noqa: E402


class Demangle(unittest.TestCase):
    def test_builtins_and_unsigned(self):
        self.assertEqual(proto_types.demangle("CreateNAS__FiiiUsUs"), ["s32", "s32", "s32", "u16", "u16"])
        self.assertEqual(proto_types.demangle("init_file__Fv"), [])

    def test_classes_and_repeats(self):
        self.assertEqual(proto_types.demangle("InsertNasTag__FP7NAStypeT0"), ["NAS *", "NAS *"])
        self.assertEqual(proto_types.demangle("line_intersect_zy__FP6VECTORN40"), ["VECTOR *"] * 5)

    def test_unknown_class_is_none(self):
        # G4xy_w: a class passed by value (not mapped), then int and unsigned long
        self.assertEqual(proto_types.demangle("texture_draw__FP11obj_textureG4xy_wiUl"),
                         ["obj_texture *", None, "s32", "u32"])
        self.assertEqual(proto_types.demangle("tga_convert__FP10tga_headerll"), [None, "s32", "s32"])

    def test_function_pointer_unsupported(self):
        with self.assertRaises(proto_types.Unsupported):
            proto_types.demangle("asload_permanent_callback__FP4filePFPvPv_vPv")


class Decl(unittest.TestCase):
    def test_prototypes_not_calls(self):
        text = ("void set_fade(u8 *arg0, s32 arg1, s32 arg2);\n"
                "struct obj_dbase *f(void);\n"
                "void g(void) {\n    set_fade(D_8009F644, 0x200, 0);\n    x = set_fade(p, 1, 2);\n}\n")
        found = [m.group(0) for m in proto_types.decl_rx("set_fade").finditer(text)]
        self.assertEqual(found, ["void set_fade(u8 *arg0, s32 arg1, s32 arg2);"])
        self.assertEqual([m.group(1) for m in proto_types.decl_rx("f").finditer(text)], ["struct obj_dbase *"])


class Better(unittest.TestCase):
    def test_rules(self):
        self.assertTrue(proto_types.better("u8 *", "NAS *"))          # generic pointer -> known type
        self.assertTrue(proto_types.better("s32", "u32"))             # integer for integer
        self.assertTrue(proto_types.better("u8 *", "char *"))
        self.assertFalse(proto_types.better("EngineBox *", "subengine *"))   # the repo's type is specific
        self.assertFalse(proto_types.better("POLY_GT4 *", "void *"))
        self.assertFalse(proto_types.better("u8 *", None))


if __name__ == "__main__":
    unittest.main()
