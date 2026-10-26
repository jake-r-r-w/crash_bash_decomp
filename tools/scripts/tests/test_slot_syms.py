"""Tests for slot_syms.py: which overlay symbols the engine's link gets.
Run: tools/dk make test-tools"""
import os
import sys
import unittest
from unittest import mock

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import slot_syms  # noqa: E402

ELFS = {
    "tank.elf": {"InitShell": 0x800C7A00, "PlotShell": 0x800BB5C8, "Local": 0x800B4000},
    "pogo.elf": {"InitPogo": 0x800B5000, "Shared": 0x800B6000},
    "arkenoid.elf": {"Shared": 0x800B6000},
}


class Resolve(unittest.TestCase):
    def run_resolve(self, names, elfs=ELFS):
        with mock.patch.object(slot_syms, "defined", side_effect=lambda e: elfs[e]):
            return slot_syms.resolve(set(names), list(elfs))

    def test_only_the_names_asked_for(self):
        self.assertEqual(self.run_resolve({"InitShell", "PlotShell", "NullProc"}),
                         [("InitShell", 0x800C7A00), ("PlotShell", 0x800BB5C8)])

    def test_the_same_address_in_two_overlays_is_fine(self):
        self.assertEqual(self.run_resolve({"Shared"}), [("Shared", 0x800B6000)])

    def test_one_name_at_two_addresses_is_an_error(self):
        clash = dict(ELFS, **{"arkenoid.elf": {"Shared": 0x800B7000}})
        with self.assertRaises(SystemExit):
            self.run_resolve({"Shared"}, clash)


if __name__ == "__main__":
    unittest.main()
