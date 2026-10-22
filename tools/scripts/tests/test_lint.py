"""Tests for the validation gate's lint (decomp.py lint_function): only C, so every inline asm
spelling is refused, in the body or in a macro or static inline it uses, and K&R definitions are
linted too. Run: tools/dk make test-tools"""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

ASM = "inline asm (only C is allowed)"


def lint(body, name="f", head="void f(void)\n"):
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as fh:
        fh.write(f"int g;\n{head}{{\n{body}\n}}\n")
    try:
        return decomp.lint_function(fh.name, name)
    finally:
        os.unlink(fh.name)


class AsmSpellings(unittest.TestCase):
    def test_every_asm_is_an_error(self):
        for s in ('asm("nop");', '__asm("nop");', '__asm__("nop");', 'asm volatile("nop");',
                  '__asm__ __volatile__("nop");', 'asm __volatile__("nop");', '__asm volatile ("nop");',
                  'asm("");', '__asm__ __volatile__("");', 'asm("" ::: "memory");',
                  '__asm__("" : "=r"(g) : "0"(g));',
                  'register int x asm("$20");', 'register int x __asm("$20");', 'register int x __asm__("$20");'):
            self.assertIn(ASM, lint(s), s)

    def test_plain_c_is_clean(self):
        """Code that only steers the compiler is still C."""
        for s in ('g = 1; /* asm("nop") in a comment */', "int x; do { x = g; } while (0); g = x;",
                  "int *h; h++; h--;", "if (g) { g = 1; } else { g = 1; }"):
            self.assertEqual(lint(s), [], s)

    def test_m2c_leftovers(self):
        self.assertIn("m2c macro", lint("g = M2C_ERROR(1);"))


class Helpers(unittest.TestCase):
    """Moving asm into a macro or static inline doesn't make it C; the SDK's gte_ macros are the
    one exception (GTE instructions C can't express)."""

    def lint_with(self, header, body):
        d = tempfile.mkdtemp()
        with open(os.path.join(d, "h.h"), "w") as fh:
            fh.write(header)
        src = os.path.join(d, "f.c")
        with open(src, "w") as fh:
            fh.write(f'#include "h.h"\nint g;\nvoid f(void)\n{{\n{body}\n}}\n')
        return decomp.lint_function(src, "f")

    def test_asm_in_a_macro(self):
        h = "#define BARRIER() __asm__ volatile (\"\")\n#define USE() do { BARRIER(); } while (0)\n"
        self.assertTrue(any(s.startswith(ASM + " in BARRIER") for s in self.lint_with(h, "USE();")))

    def test_asm_in_an_inline(self):
        h = "static inline int Get(void) {\n    register int x asm(\"$16\");\n    x = g;\n    return x;\n}\n"
        self.assertTrue(any(s.startswith(ASM + " in Get") for s in self.lint_with(h, "g = Get();")))

    def test_gte_macros_and_plain_macros_are_clean(self):
        h = ("#define gte_ldv0(r0) __asm__ volatile (\"lwc2 $0, 0( %0 )\" : : \"r\"(r0))\n"
             "#define SET(b, v) do { (b) = (v); } while (0)\n")
        self.assertEqual(self.lint_with(h, "gte_ldv0(&g); SET(g, 1);"), [])


class KandR(unittest.TestCase):
    def test_definition_found(self):
        text = "void f(void);\nvoid f(a, b)\n    u8 *a;\n    s32 b;\n{\n    *a = b;\n}\n"
        self.assertEqual(decomp.extract_function(text, "f").strip(), "void f(a, b)\n    u8 *a;\n    s32 b;\n{\n    *a = b;\n}")

    def test_prototype_and_call_are_not_definitions(self):
        self.assertIsNone(decomp.func_def_regex("f").search("void f(void);\nint x;\n{\n"))
        self.assertIsNone(decomp.func_def_regex("f").search("void g(void) {\n    f(1);\n    x = 2;\n}\n"))

    def test_linted(self):
        self.assertIn(ASM, lint('asm("nop");', head="void f(a)\nu8 *a;\n"))


if __name__ == "__main__":
    unittest.main()
