"""Tests for slot_refs.py. Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import shift  # noqa: E402
import slot_refs  # noqa: E402


class Attribute(unittest.TestCase):
    def test_a_unique_address_is_that_overlays(self):
        done, left = slot_refs.attribute([(0x80099000, 0x800BB384)], {0x800BB384: [("chase", "CHASESetUp")]})
        self.assertEqual((done, left), ({0x80099000: ("chase", "CHASESetUp")}, []))

    def test_the_mode_table_takes_only_a_file_group(self):
        a = shift.MODE_TABLE + 4 * 0x1E
        syms = {0x800BB8EC: [("chase", "file_group_CHASE"), ("n2o", "N2OHeroTestIdle")]}
        done, _ = slot_refs.attribute([(a, 0x800BB8EC)], syms)
        self.assertEqual(done[a], ("chase", "file_group_CHASE"))

    def test_a_neighbour_settles_a_shared_address(self):
        # a world record: its SetUp is chase's alone, so the PreMap word beside it is chase's too
        syms = {0x800BB90C: [("chase", "CHASE_PREMAP"), ("bash", "BashThing")], 0x800BBA1C: [("chase", "chase_shell_info")]}
        done, left = slot_refs.attribute([(0x80099100, 0x800BB90C), (0x80099104, 0x800BBA1C)], syms)
        self.assertEqual(done[0x80099100], ("chase", "CHASE_PREMAP"))
        self.assertEqual(left, [])

    def test_a_sequence_column_goes_with_its_game(self):
        tables = [0x1000, 0x2000]
        syms = {0x5000: [("dino", "CRA_seq_dinowin")], 0x6000: [("dino", "COC_seq_dinowin"), ("hub", "Other")]}
        done, _ = slot_refs.attribute([(0x1000 + 4 * 86, 0x5000), (0x2000 + 4 * 86, 0x6000)], syms, tables)
        self.assertEqual(done[0x2000 + 4 * 86], ("dino", "COC_seq_dinowin"))

    def test_the_slot_id_word_is_no_overlays(self):
        done, left = slot_refs.attribute([(0x80098594, shift.SLOT_LO)], {shift.SLOT_LO: [("tank", "TankOverlayId")]})
        self.assertEqual((done, left), ({}, []))


if __name__ == "__main__":
    unittest.main()
