"""fold_views.py: a folded view's typedef goes, with its comment, whatever follows its name."""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import fold_views  # noqa: E402


class DropTypedef(unittest.TestCase):
    def drop(self, text):
        p = os.path.join(tempfile.mkdtemp(), "x.h")
        with open(p, "w") as f:
            f.write(text)
        fold_views.drop_typedef("V", [p])
        return open(p).read()

    def test_trailing_comment(self):
        self.assertEqual(self.drop("/* a */\ntypedef struct {\n    s32 a;\n} V; /* size 4 */\nint keep;\n"),
                         "int keep;\n")

    def test_plain_and_one_line(self):
        self.assertEqual(self.drop("typedef struct V {\n    s32 a;\n} V;\ntypedef struct { s32 b; } V;\nint k;\n"),
                         "int k;\n")


if __name__ == "__main__":
    unittest.main()
