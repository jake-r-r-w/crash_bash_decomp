"""Tests for decomp.py try --file: the test file is compiled with the settings of the unit that
defines the function (per-unit Makefile overrides included), not with project defaults.
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

CFG = {"CC_VER": "2.8.1-psx", "OPT_FLAGS": "-O2", "EXTRA_CC1FLAGS": "-fno-builtin", "GVAL": "0"}


class UnitPipeline(unittest.TestCase):
    def test_c_path_is_the_input(self):
        cmd = decomp.unit_pipeline("src/overlays/ovl63/ovl63.c", "build/t.o", dict(CFG),
                                   c_path="scratch/t.c")
        cpp = cmd.split("|")[0]
        self.assertTrue(cpp.rstrip().endswith("scratch/t.c"), cpp)
        self.assertNotIn("ovl63.c", cmd)

    def test_without_c_path_the_unit_is_the_input(self):
        cmd = decomp.unit_pipeline("src/overlays/ovl63/ovl63.c", "build/t.o", dict(CFG))
        self.assertTrue(cmd.split("|")[0].rstrip().endswith("src/overlays/ovl63/ovl63.c"))

    @unittest.skipUnless(os.path.exists("Makefile"), "needs the repo's Makefile (run from the root)")
    def test_the_units_overrides_apply(self):
        """main/8002d488 is a gcc 2.7.2 unit (Makefile): its settings follow the test file."""
        cmd = decomp.unit_pipeline("src/main/8002d488.c", "build/t.o", dict(CFG), c_path="t.c")
        self.assertIn("2.7.2-psx/cc1", cmd)
        self.assertIn("-fno-rerun-cse-after-loop", cmd)


if __name__ == "__main__":
    unittest.main()
