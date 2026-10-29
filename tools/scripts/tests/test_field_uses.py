"""field_uses.py: a struct's members and the probe rename stay inside its body."""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import field_uses  # noqa: E402

H = """typedef struct A {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[4];
} A;
typedef struct {
    /* 0x00 */ s32 unk0;   /* B's */
    /* 0x04 */ s16 unk4[2];
} B;
"""


class Uses(unittest.TestCase):
    def test_members_and_probe(self):
        d = os.path.join(os.path.dirname(__file__), "tmp_field_uses")
        os.makedirs(d, exist_ok=True)
        with open(os.path.join(d, "x.h"), "w") as f:
            f.write(H)
        h, s, e = field_uses.find_struct("B", d)
        self.assertEqual([n for n, _ in field_uses.members(H[s:e])], ["unk0", "unk4"])
        out = field_uses.probe(H, s, e, ["unk0", "unk4"])
        self.assertIn("s32 unk0;\n    /* 0x04 */ u8 pad4", out)          # A untouched
        self.assertIn("s32 unk0__use;   /* B's */", out)
        self.assertIn("s16 unk4__use[2];", out)
        os.remove(os.path.join(d, "x.h"))
        os.rmdir(d)


if __name__ == "__main__":
    unittest.main()
