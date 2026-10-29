"""retype.py: a raw word read that becomes a pointer field is reported when code adds to it."""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import retype  # noqa: E402

LV = {0x54: [("Engine", 4, False, "ptr")], 0x5C: [("count", 4, True, "")]}


class TestRetype(unittest.TestCase):
    def rewrite(self, body):
        ptrs = set()
        out = retype.rewrite(body, "a", "NAS", LV, ptrs=ptrs, log=lambda *x: None)
        return out, ptrs

    def test_pointer_field_offset_is_left(self):
        out, ptrs = self.rewrite("f((*(s32*)((u8*)(a) + 84)) + 0x94);\n")
        self.assertEqual(out, "f((a->Engine) + 0x94);\n")
        self.assertEqual(ptrs, {"Engine"})
        self.assertEqual(retype.leftovers(out, "a", ptrs), ["f((a->Engine) + 0x94);"])

    def test_number_field_offset_is_fine(self):
        out, ptrs = self.rewrite("f((*(s32*)((u8*)(a) + 92)) + 1);\n")
        self.assertEqual(out, "f((a->count) + 1);\n")
        self.assertEqual(retype.leftovers(out, "a", ptrs), [])


if __name__ == "__main__":
    unittest.main()
