"""unk_fields.py: a field's uses sorted into none, write and read."""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import unk_fields  # noqa: E402


class Kind(unittest.TestCase):
    def test_kinds(self):
        self.assertEqual(unk_fields.kind("unk8", [])[0], "none")
        self.assertEqual(unk_fields.kind("unk8", ["p->unk8 = 0;", "q.unk8[2] = 1;"])[0], "write")
        self.assertEqual(unk_fields.kind("unk8", ["p->unk8 = q->unk8;"])[0], "read")
        self.assertEqual(unk_fields.kind("unk8", ["p->unk8++;"])[0], "read")
        self.assertEqual(unk_fields.kind("unk8", ["if (p->unk8 == 2) {"])[0], "read")


if __name__ == "__main__":
    unittest.main()
