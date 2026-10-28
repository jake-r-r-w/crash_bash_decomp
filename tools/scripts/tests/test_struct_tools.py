"""Tests for struct_layout.py, retype.py, fold_views.py and diff_funcs.py (their pure parts).
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import diff_funcs  # noqa: E402
import fold_views  # noqa: E402
import retype  # noqa: E402
import struct_layout as sl  # noqa: E402

# cc1 -g output for: typedef struct { s32 a; s16 b; u8 c[2]; struct In in; } Out; struct In { u16 x, y; };
SDB = """
\t.def\tIn;\t.scl\t10;\t.type\t0x8;\t.size\t4;\t.endef
\t.def\tx;\t.val\t0;\t.scl\t8;\t.type\t0xd;\t.endef
\t.def\ty;\t.val\t2;\t.scl\t8;\t.type\t0xd;\t.endef
\t.def\t.eos;\t.val\t4;\t.scl\t102;\t.tag\tIn;\t.size\t4;\t.endef
\t.def\t.0fake;\t.scl\t10;\t.type\t0x8;\t.size\t12;\t.endef
\t.def\ta;\t.val\t0;\t.scl\t8;\t.type\t0x4;\t.endef
\t.def\tb;\t.val\t4;\t.scl\t8;\t.type\t0x3;\t.endef
\t.def\tc;\t.val\t6;\t.scl\t8;\t.type\t0x3c;\t.dim\t2;\t.size\t2;\t.endef
\t.def\tin;\t.val\t8;\t.scl\t8;\t.tag\tIn;\t.size\t4;\t.type\t0x8;\t.endef
\t.def\t.eos;\t.val\t12;\t.scl\t102;\t.tag\t.0fake;\t.size\t12;\t.endef
\t.def\tOut;\t.scl\t13;\t.tag\t.0fake;\t.size\t12;\t.type\t0x8;\t.endef
"""


class StructLayoutTest(unittest.TestCase):
    def setUp(self):
        self.structs, self.typedefs = sl.parse_sdb(SDB)

    def test_parse(self):
        self.assertEqual(self.typedefs["Out"], ".0fake")
        self.assertEqual([m[0] for m in self.structs[".0fake"]], ["a", "b", "c", "in"])
        self.assertEqual(self.structs["In"][1][1], 2)

    def test_leaves_flatten_arrays_and_nested(self):
        lv = sl.leaves("Out", self.structs, self.typedefs)
        self.assertEqual(lv[0][0][:3], ("a", 4, True))
        self.assertEqual(lv[4][0][:3], ("b", 2, True))
        self.assertEqual(lv[7][0][:3], ("c[1]", 1, False))
        self.assertEqual(lv[8][0][:3], ("in.x", 2, False))
        self.assertEqual(lv[10][0][0], "in.y")
        self.assertEqual(sl.leaves("struct In", self.structs, self.typedefs)[2][0][0], "y")


class RetypeTest(unittest.TestCase):
    def setUp(self):
        s, t = sl.parse_sdb(SDB)
        self.lv = sl.leaves("Out", s, t)

    def test_fields_by_offset_and_width(self):
        body = "void f(u8 *p) {\n    *(s16 *)(p + 4) = *(s32 *)p;\n    q = *(u16 *)(p + 0xA);\n}\n"
        out = retype.rewrite(body, "p", "Out", self.lv, log=lambda m: None)
        self.assertIn("void f(Out *p)", out)
        self.assertIn("p->b = p->a;", out)
        self.assertIn("q = p->in.y;", out)

    def test_narrow_read_of_a_wider_field(self):
        out = retype.rewrite("void f(u8 *p) {\n    q = *(u8 *)p;\n}\n", "p", "Out", self.lv, log=lambda m: None)
        self.assertIn("q = *(u8 *)&p->a;", out)

    def test_unmapped_and_leftovers(self):
        msgs = []
        out = retype.rewrite("void f(u8 *p) {\n    q = *(s32 *)(p + 0x40);\n    r = p + 4;\n}\n", "p", "Out", self.lv, log=msgs.append)
        self.assertTrue(any("unmapped" in m for m in msgs))
        # both are scaled by the new type now: the unmapped access and the arithmetic
        self.assertEqual(retype.leftovers(out, "p"), ["q = *(s32 *)(p + 0x40);", "r = p + 4;"])

    def test_assignment_gets_a_cast(self):
        out = retype.rewrite("void f(void) {\n    u8 *p;\n    p = base + n * 12;\n}\n", "p", "Out", self.lv, log=lambda m: None)
        self.assertIn("p = (Out *)(base + n * 12);", out)


class FoldViewsTest(unittest.TestCase):
    def setUp(self):
        s, t = sl.parse_sdb(SDB)
        self.cl = sl.leaves("Out", s, t)
        self.names = {"a", "b", "c", "in"}

    def test_map_and_traps(self):
        view = [("unk0", 0, 0, "0x4", "", ""), ("b", 8, 0, "0xd", "", ""), ("h", 0, 0, "0x3", "", ""),
                ("pad2", 2, 2, "0x3c", "2", ""), ("unk4", 4, 0, "0x3", "", "")]
        m, traps, un = fold_views.map_view(view, self.cl, self.names)
        self.assertEqual(m["unk0"], "a")
        self.assertEqual(m["unk4"], "b")
        self.assertEqual(m["b"], "in.x")   # named like a canon member elsewhere: a trap
        self.assertIn("b", traps)
        self.assertEqual([u[0] for u in un], ["h"])  # a halfword at a word: no member

    def test_rounds_split_conflicts(self):
        r = fold_views.rounds_of({"V1": {"x": "a"}, "V2": {"x": "b"}, "V3": {"y": "c"}})
        self.assertEqual([x["views"] for x in r], [["V1", "V3"], ["V2"]])


class DiffFuncsTest(unittest.TestCase):
    def test_differing(self):
        funcs = {"1": {"bin": "disc/x.bin", "rom": 0, "size": 4, "name": "f"},
                 "2": {"bin": "disc/x.bin", "rom": 4, "size": 4, "name": "g"},
                 "3": {"seg": "main/main", "rom": 0, "size": 4, "name": "m"}}
        self.assertEqual(diff_funcs.differing(funcs, b"\0" * 4 + b"\1" * 4, b"\0" * 8, "disc/x.bin"), [(4, "g")])
        self.assertEqual(diff_funcs.bin_of(funcs["3"]), diff_funcs.MAIN)


if __name__ == "__main__":
    unittest.main()
