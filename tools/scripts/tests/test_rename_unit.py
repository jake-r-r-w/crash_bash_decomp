import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import rename_unit  # noqa: E402


class RenameUnit(unittest.TestCase):
    def test_paths(self):
        self.assertEqual(rename_unit.new_path("include/bash/ovl62.h", "ovl62", "polar"), "include/bash/polar_ovl.h")
        self.assertEqual(rename_unit.new_path("include/bash/ovl5d_800bb64c.h", "ovl5d", "bash"),
                         "include/bash/bash_800bb64c.h")
        self.assertEqual(rename_unit.new_path("src/overlays/ovl62/ovl62_id.c", "ovl62", "polar"),
                         "src/overlays/polar/polar_id.c")
        self.assertEqual(rename_unit.new_path("configs/USA/sym.ovl62.txt", "ovl62", "polar"), "configs/USA/sym.polar.txt")
        self.assertEqual(rename_unit.new_path("include/bash/polar.h", "ovl62", "polar"), "include/bash/polar.h")

    def test_c_names(self):
        t = "Ovl62PolarAi a; Ovl62Cam *c; OVL62_ATTACK; Ovl60_SpawnPuff(); Ovl620x; BASH_OVL62_H"
        self.assertEqual(rename_unit.c_names(t, "ovl62", "Polar"),
                         {"Ovl62PolarAi": "PolarAi", "Ovl62Cam": "PolarCam", "OVL62_ATTACK": "POLAR_ATTACK"})
        # a doubled prefix is dropped only before a capital
        self.assertEqual(rename_unit.c_names("Ovl62Polarity", "ovl62", "Polar"), {"Ovl62Polarity": "PolarPolarity"})

    def test_rewrite(self):
        t = ('#ifndef BASH_OVL62_H\n#include "bash/ovl62.h"\n#include "bash/ovl62_800b5000.h"\n'
             'INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", f);\n'
             '"ovl62:800B3A38" Ovl62Cam BASH_OVL62_800B5000_H ovl621 xovl62\n')
        names = rename_unit.c_names(t, "ovl62", "Polar")
        self.assertEqual(rename_unit.rewrite(t, "ovl62", "polar", names),
                         '#ifndef BASH_POLAR_OVL_H\n#include "bash/polar_ovl.h"\n#include "bash/polar_800b5000.h"\n'
                         'INCLUDE_ASM("asm/nonmatchings/overlays/polar/polar", f);\n'
                         '"polar:800B3A38" PolarCam BASH_POLAR_800B5000_H ovl621 xovl62\n')


if __name__ == "__main__":
    unittest.main()
