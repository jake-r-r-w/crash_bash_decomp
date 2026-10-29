"""hidden.py: the final test's nothing-hidden scan."""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import hidden  # noqa: E402


class Source(unittest.TestCase):
    def kinds(self, path, text):
        return [k for _, k, _ in hidden.scan_source(path, text)]

    def test_finds_each_kind(self):
        text = ('INCLUDE_ASM("asm/x", func);\nINCLUDE_RODATA("asm/x", D);\n.incbin "x.bin"\n'
                'asm("nop");\n#ifdef NON_MATCHING\nx = func_80012345();\ny = D_8005AA04;\n')
        self.assertEqual(self.kinds("src/main/a.c", text),
                         ["asm include", "asm include", ".incbin", "inline asm", "parked draft",
                          "address name", "address name"])

    def test_clean_code_passes(self):
        self.assertEqual(self.kinds("src/main/a.c", "s32 x = GetEngineSlot(0)->flags;\n/* a NON_MATCHING note */\n"), [])

    def test_allowed_lines(self):
        self.assertEqual(self.kinds("src/main/prims.c", 'register s32 xy asm("$20");\n'), [])
        self.assertEqual(self.kinds("include/bash/main.h", '#define gte_ldv0_w(a, b) __asm__ volatile ("mtc2")\n'), [])
        # the same lines anywhere else fail
        self.assertEqual(self.kinds("src/main/main.c", 'register s32 xy asm("$20");\n'), ["inline asm"])


class Maps(unittest.TestCase):
    def test_non_source_objects(self):
        m = ("LOAD build/src/main/main.c.o\nLOAD build/psyq/libapi/c81.o\n"
             "LOAD build/asm/header.s.o\nLOAD build/asm/bin/overlays/engine/engine_bss.bin.o\n"
             "LOAD linkers/undefined_syms_auto.txt\n")
        self.assertEqual(hidden.scan_map(m), ["build/asm/header.s.o", "build/asm/bin/overlays/engine/engine_bss.bin.o"])

    def test_exceptions_have_reasons(self):
        for o, why in hidden.EXCEPTIONS.items():
            self.assertTrue(o.startswith("build/") and len(why) > 20, o)


if __name__ == "__main__":
    unittest.main()
