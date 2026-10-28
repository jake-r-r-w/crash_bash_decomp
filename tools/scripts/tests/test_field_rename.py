"""Tests for field_rename.py: only the renamed struct's uses change.
Run: tools/dk make test-tools"""
import os
import shutil
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import field_rename  # noqa: E402

HDR = """typedef struct A { int dbase; int other; } A;
typedef struct B { int unk6C; } B;
"""
SRC = """#include "t.h"
int f(A *a, B *b) {
    a->unk6C = 1;
    b->unk6C = 2;
    return a->other + b->unk6C;
}
"""


@unittest.skipUnless(os.path.exists(field_rename.CC1[0]), "needs the container's cc1")
class FieldRenameTest(unittest.TestCase):
    def test_renames_only_the_struct_named(self):
        d = tempfile.mkdtemp()
        cwd = os.getcwd()
        try:
            os.makedirs(os.path.join(d, "include"))
            with open(os.path.join(d, "include", "t.h"), "w") as f:
                f.write(HDR)
            with open(os.path.join(d, "t.c"), "w") as f:
                f.write(SRC)
            os.chdir(d)
            sys.argv = ["field_rename.py", "unk6C", "dbase", "--files", "t.c"]
            self.assertEqual(field_rename.main(), 0)
            out = open("t.c").read()
            self.assertIn("a->dbase = 1;", out)
            self.assertIn("b->unk6C = 2;", out)
            self.assertIn("b->unk6C;", out)
        finally:
            os.chdir(cwd)
            shutil.rmtree(d)


if __name__ == "__main__":
    unittest.main()
