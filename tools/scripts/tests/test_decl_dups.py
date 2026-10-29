"""Tests for decl_dups.py: repeated declarations in the headers. Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decl_dups  # noqa: E402

ROOT = os.path.join(os.path.dirname(__file__), "..", "..", "..")


class Repeats(unittest.TestCase):
    def test_declarations(self):
        t = "void f(s32 a);\nextern s32 G[4];\ntypedef struct S S;\nstatic inline void g(void) {\n}\n"
        self.assertEqual(decl_dups.declarations(t), [("f", 1), ("G", 2)])

    def test_repeat_in_a_header_and_of_the_leak(self):
        hs = {"x/pogo_ovl.h": "void A(void);\nvoid B(s32);\nvoid A();\n",
              "x/pogo.h": "void B(s32 x);\n",
              "x/tank_ovl.h": "void A(void);\n"}
        self.assertEqual(decl_dups.repeats(hs), [("x/pogo_ovl.h", 2, "B", "declared in the leaked pogo.h"),
                                                 ("x/pogo_ovl.h", 3, "A", "already declared at line 1")])
        self.assertEqual(decl_dups.views(hs), {"A": ["pogo_ovl.h", "tank_ovl.h"], "B": ["pogo.h", "pogo_ovl.h"]})

    def test_the_repo_has_none(self):
        hs = decl_dups.load(os.path.join(ROOT, "include", "bash"))
        self.assertTrue(hs)
        self.assertEqual(decl_dups.repeats(hs), [])

    def test_the_repo_has_no_views(self):
        hs = decl_dups.load(os.path.join(ROOT, "include", "bash"))
        self.assertEqual(decl_dups.views(hs, decl_dups.not_views(hs, os.path.join(ROOT, "src"))), {})


if __name__ == "__main__":
    unittest.main()
