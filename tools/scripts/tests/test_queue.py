"""Tests for decomp.py's target picking and function lookup (what `next`, `queue` and the swarm
lease). Run: tools/dk make test-tools"""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

PROJ = {"lib_segments": ["^psyq/"]}


def fn(seg, name, size, fp=None):
    return {"seg": seg, "name": name, "orig": name, "size": size, "fp": fp}


class PickTargets(unittest.TestCase):
    def setUp(self):
        self.funcs = {
            "80010000": fn("main/main", "func_80010000", 400),
            "80020000": fn("main/main", "func_80020000", 40),
            "80030000": fn("main/main", "func_80030000", 200, fp="t"),
            "ovl5d:800B3A38": fn("overlays/ovl5d/ovl5d", "func_800B3A38", 200, fp="t"),
            "80040000": fn("main/main", "func_80040000", 8),
            "80050000": fn("psyq/libgte", "func_80050000", 12),
        }
        self.stat = {k: "todo" for k in self.funcs}
        self.stat["80040000"] = "matched"

    def keys(self, st=None, seg=None, count=10):
        return [k for k, _ in decomp.pick_targets(self.funcs, self.stat, st or {}, PROJ, seg, count)]

    def test_order_and_scope(self):
        # matched and library code are left out; cost = size / unmatched identical copies
        self.assertEqual(self.keys(), ["80020000", "80030000", "ovl5d:800B3A38", "80010000"])

    def test_seg_prefix(self):
        self.assertEqual(self.keys(seg="overlays/"), ["ovl5d:800B3A38"])
        self.assertEqual(self.keys(seg="psyq/"), ["80050000"])

    def test_in_progress_first_and_tried_last(self):
        st = {"80010000": {"status": "in_progress"}, "80020000": {"tries": 3}}
        self.stat["80010000"] = "in_progress"
        self.assertEqual(self.keys(st), ["80010000", "80030000", "ovl5d:800B3A38", "80020000"])

    def test_auto_diff_is_the_work(self):
        self.assertEqual(self.keys({"80010000": {"auto_diff": 2}})[0], "80010000")

    def test_count(self):
        self.assertEqual(len(self.keys(count=2)), 2)


class FindFunc(unittest.TestCase):
    """Slot overlays all load at 0x800B32B4, so their splat names repeat across overlays."""

    def setUp(self):
        self.funcs = {
            "8002D4F4": dict(fn("main/main", "func_8002D4F4", 40), src="src/main/main.c"),
            "ovl5d:800B3A38": dict(fn("overlays/ovl5d/ovl5d", "func_800B3A38", 40), src="src/overlays/ovl5d/ovl5d.c"),
            "ovl60:800B3A38": dict(fn("overlays/ovl60/ovl60", "func_800B3A38", 40), src="src/overlays/ovl60/ovl60.c"),
        }
        self.removed = {}
        self.saved = decomp.sym_names, decomp.matched_in_worktree, decomp.die
        decomp.sym_names = lambda: {}
        decomp.matched_in_worktree = lambda path="src": self.removed.get(path, set())

        def die(msg, code=2):
            raise SystemExit(msg)
        decomp.die = die

    def tearDown(self):
        decomp.sym_names, decomp.matched_in_worktree, decomp.die = self.saved

    def find(self, name):
        return decomp.find_func(self.funcs, name)[0]

    def test_name_key_and_address(self):
        self.assertEqual(self.find("func_8002D4F4"), "8002D4F4")
        self.assertEqual(self.find("0x8002D4F4"), "8002D4F4")
        self.assertEqual(self.find("ovl60:800B3A38"), "ovl60:800B3A38")
        self.assertEqual(self.find("overlays/ovl5d/ovl5d/func_800B3A38"), "ovl5d:800B3A38")

    def test_shared_name_is_ambiguous(self):
        with self.assertRaisesRegex(SystemExit, "ambiguous.*ovl5d:800B3A38"):
            self.find("func_800B3A38")
        with self.assertRaisesRegex(SystemExit, "ambiguous"):
            self.find("0x800B3A38")

    def test_shared_name_being_worked_on(self):
        self.removed["src/overlays/ovl60/ovl60.c"] = {"func_800B3A38"}
        self.assertEqual(self.find("func_800B3A38"), "ovl60:800B3A38")

    def test_did_you_mean(self):
        with self.assertRaisesRegex(SystemExit, "did you mean func_8002D4F4"):
            self.find("func_8002D4F5")


class Declarations(unittest.TestCase):
    def test_decl_name_function_pointers(self):
        self.assertEqual(decomp.decl_name("extern s32 (*D_8009D728)(s32);"), "D_8009D728")
        self.assertEqual(decomp.decl_name("extern void (*D_800A0000[4])(void);"), "D_800A0000")
        self.assertEqual(decomp.decl_name("void func_80010000(s32 (*cb)(void));"), "func_80010000")
        self.assertEqual(decomp.decl_name("extern u8 D_80030000[16];"), "D_80030000")

    def test_copy_decls(self):
        """A copy gets the declarations its original's file sees and its own file doesn't (the
        ovl64:800BB52C case: an implicit call to a function pointer)."""
        cwd = os.getcwd()
        with tempfile.TemporaryDirectory() as d:
            os.chdir(d)
            try:
                for p, text in {
                    "include/common.h": "",
                    "include/bash/eng.h": "/* owner: src/eng.c */\ntypedef struct S {\n    s32 g;\n} S;\n"
                                          "extern void (*D_8009D73C)();\nvoid func_8008502C(s32);\n",
                    "include/bash/ovl.h": "/* owner: src/ovl.c */\nvoid func_8008502C(s32);\n",
                    "src/eng.c": '#include "common.h"\n#include "bash/eng.h"\n',
                    "src/ovl.c": '#include "common.h"\n#include "bash/ovl.h"\n',
                }.items():
                    os.makedirs(os.path.dirname(p), exist_ok=True)
                    with open(p, "w") as fh:
                        fh.write(text)
                ftext = "void f(s32 g) {\n    D_8009D73C(g);\n    func_8008502C(g);\n}\n"
                self.assertEqual(decomp.copy_decls(ftext, "src/eng.c", "src/ovl.c", ["include/common.h"], []),
                                 ["extern void (*D_8009D73C)();"])
            finally:
                os.chdir(cwd)


if __name__ == "__main__":
    unittest.main()
