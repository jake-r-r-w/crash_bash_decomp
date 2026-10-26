import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import rename_file  # noqa: E402

SEG, NEW_SEG = "overlays/bash/800bb64c", "overlays/bash/xtras"
HDR, NEW_HDR = "include/bash/bash_800bb64c.h", "include/bash/bash_xtras.h"


class RenameFile(unittest.TestCase):
    def test_header_name(self):
        self.assertEqual(rename_file.new_header(HDR, "800bb64c", "xtras"), NEW_HDR)
        self.assertEqual(rename_file.new_header("include/bash/80083708.h", "80083708", "hero"),
                         "include/bash/hero.h")

    def test_paths_includes_and_guard(self):
        t = ('#ifndef BASH_BASH_800BB64C_H\n#include "bash/bash_800bb64c.h"\n'
             '      - [0x8398, c, overlays/bash/800bb64c]   # proven after 21\n'
             ' * owner: src/overlays/bash/800bb64c.c */\n"seg": "overlays/bash/800bb64c",\n'
             'build/src/overlays/bash/800bb64c.c.o: CC_VER\n'
             '"asm": "asm/nonmatchings/overlays/bash/800bb64c/CreateCrate.s"\n'
             '/* data defined in 800bb64c.c */ func_800BB64C overlays/bash/800bb64cx\n')
        self.assertEqual(rename_file.rewrite(t, SEG, NEW_SEG, HDR, NEW_HDR, True),
                         '#ifndef BASH_BASH_XTRAS_H\n#include "bash/bash_xtras.h"\n'
                         '      - [0x8398, c, overlays/bash/xtras]   # proven after 21\n'
                         ' * owner: src/overlays/bash/xtras.c */\n"seg": "overlays/bash/xtras",\n'
                         'build/src/overlays/bash/xtras.c.o: CC_VER\n'
                         '"asm": "asm/nonmatchings/overlays/bash/xtras/CreateCrate.s"\n'
                         '/* data defined in xtras.c */ func_800BB64C overlays/bash/800bb64cx\n')

    def test_a_bare_file_name_outside_its_directory_is_left(self):
        t = "the split at 800bb64c.c (papu's 800b87a0.c)"
        self.assertEqual(rename_file.rewrite(t, SEG, NEW_SEG, HDR, NEW_HDR, False), t)


if __name__ == "__main__":
    unittest.main()
