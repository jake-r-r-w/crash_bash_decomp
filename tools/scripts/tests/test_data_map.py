"""Tests for the dashboard's data map: C blocks cut at their objects, and which names are real.
Run: tools/dk make test-tools"""
import os
import sys
import unittest
from unittest import mock

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402


def cells(m):
    return list(zip(m["n"], m["s"], m["t"], m["nm"]))


class DataMap(unittest.TestCase):
    def test_a_c_block_is_cut_at_its_objects(self):
        items = [["ovl", "overlays/ovl/ovl", "overlays/ovl/ovl", 24, True, ".data"]]
        with mock.patch.object(decomp, "split_c_block",
                               return_value=[("TankFiles", 16), ("D_800C0010", 8)]):
            m = decomp.page_data_map(items)
        self.assertEqual(m["segs"], [["overlays/ovl/ovl", "ovl", 2]])
        self.assertEqual(cells(m), [("TankFiles", 16, "c", "1"), ("D_800C0010", 8, "c", "0")])

    def test_asm_labels_and_unlabelled_asm_files_are_placeholders(self):
        items = [["ovl", "overlays/ovl/ovl_data", "ovl_data", 8, False, "data"],
                 ["ovl", "overlays/ovl/ovl_data", "D_800C0008", 4, False, "data"],
                 ["ovl", "overlays/ovl/ovl_data", "TankMines", 4, False, "data"]]
        m = decomp.page_data_map(items)
        self.assertEqual(m["nm"], "001")

    def test_a_matched_functions_rodata_is_the_compilers(self):
        items = [["ovl", "overlays/ovl/ovl", "D_800B3928", 16, True, ".rodata"],
                 ["ovl", "overlays/ovl/ovl", "D_800B3938", 16, False, ".rodata"]]
        with mock.patch.object(decomp, "split_c_block", return_value=None):
            m = decomp.page_data_map(items)
        self.assertEqual(m["nm"], "10")

    def test_jump_tables_are_shown_by_their_function(self):
        rs = [(0x30, "jtbl_800B3670", ["PlotTotemBit"]), (0x10, "jtbl_800B3650", ["PlotTotemBit"]),
              (0x50, "jtbl_800B3690", ["PlotMine"]), (0x60, "jtbl_800B36A0", ["A", "B"]),
              (0x70, "D_800B36B0", ["PlotMine"])]
        self.assertEqual(decomp.jump_table_labels(rs), {
            "jtbl_800B3650": "PlotTotemBit switch table 1", "jtbl_800B3670": "PlotTotemBit switch table 2",
            "jtbl_800B3690": "PlotMine switch table"})

    def test_an_objects_bytes_before_its_first_symbol_keep_the_file_name(self):
        with mock.patch.object(decomp, "object_data_syms", return_value={".rodata": [(8, "TankAnimHit")]}), \
                mock.patch.object(decomp.os.path, "exists", return_value=True):
            parts = decomp.split_c_block("overlays/ovl5f/ovl5f", ".rodata", 12, {})
        self.assertEqual(parts, [("ovl5f", 8), ("TankAnimHit", 4)])


if __name__ == "__main__":
    unittest.main()
