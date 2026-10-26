"""Tests for model_slots.py. Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import model_slots  # noqa: E402


class Lists(unittest.TestCase):
    def test_an_index_symbol_is_its_entry(self):
        self.assertEqual(model_slots.entry("D_8004E110"), 0)
        self.assertEqual(model_slots.entry("D_8004ECB8"), (0x8004ECB8 - 0x8004E110) // 8)
        self.assertIsNone(model_slots.entry("D_8004E0F0"))
        self.assertIsNone(model_slots.entry("FontModel"))

    def test_a_file_list_reads_as_slot_triples_up_to_its_zeros(self):
        body = "(s32)&FontModel, (s32)&D_8004ECB8, (s32)&D_8004ECB0, 0, 0, 0, (s32)&D_800500A0"
        self.assertEqual(model_slots.triples(body),
                         [("FontModel", model_slots.entry("D_8004ECB8"), model_slots.entry("D_8004ECB0"))])


if __name__ == "__main__":
    unittest.main()
