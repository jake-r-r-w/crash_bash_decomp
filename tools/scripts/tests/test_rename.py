"""Tests for decomp.py rename's choice of asm files: a slot overlay's own only.
Run: tools/dk make test-tools"""
import os
import sys
import unittest
from unittest import mock

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

FOUND = ["asm/nonmatchings/overlays/bash/bash/func_800B4848.s",
         "asm/nonmatchings/overlays/titles/titles/func_800B4848.s",
         "asm/nonmatchings/overlays/engine/80083708/func_800B4848.s",
         "asm/nonmatchings/main/80017164/func_800B4848.s"]


class AsmFiles(unittest.TestCase):
    def test_a_slot_overlay_renames_only_its_own(self):
        with mock.patch.object(decomp.glob, "glob", return_value=FOUND):
            self.assertEqual(decomp.asm_files_of("titles", "func_800B4848"), [FOUND[1]])

    def test_the_exe_and_engine_never_take_a_slot_overlays(self):
        with mock.patch.object(decomp.glob, "glob", return_value=FOUND):
            self.assertEqual(decomp.asm_files_of("", "func_800B4848"), FOUND[2:])
            self.assertEqual(decomp.asm_files_of("engine", "func_800B4848"), FOUND[2:])


if __name__ == "__main__":
    unittest.main()
