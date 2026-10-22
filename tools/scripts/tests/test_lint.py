"""Tests for the validation gate's lint (decomp.py lint_function): every inline asm spelling is
refused or needs --hack, and K&R definitions are linted too. Run: tools/dk make test-tools"""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402


def lint(body, name="f", head="void f(void)\n"):
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as fh:
        fh.write(f"int g;\n{head}{{\n{body}\n}}\n")
    try:
        return decomp.lint_function(fh.name, name)
    finally:
        os.unlink(fh.name)


class AsmSpellings(unittest.TestCase):
    def test_instructions_are_errors(self):
        for s in ('asm("nop");', '__asm("nop");', '__asm__("nop");', 'asm volatile("nop");',
                  '__asm__ __volatile__("nop");', 'asm __volatile__("nop");', '__asm volatile ("nop");'):
            self.assertIn("inline asm with instructions", lint(s)[0], s)

    def test_empty_asm_is_a_hack(self):
        for s in ('asm("");', '__asm__ __volatile__("");', 'asm("" ::: "memory");',
                  '__asm__("" : "=r"(g) : "0"(g));'):
            errs, hacks = lint(s)
            self.assertEqual(errs, [], s)
            self.assertIn("empty asm barrier", hacks, s)

    def test_register_pins_are_hacks(self):
        for s in ('register int x asm("$20");', 'register int x __asm("$20");', 'register int x __asm__("$20");'):
            errs, hacks = lint(s)
            self.assertEqual(errs, [], s)
            self.assertIn("register pin", hacks, s)

    def test_plain_c_is_clean(self):
        self.assertEqual(lint("g = 1; /* asm(\"nop\") in a comment */"), ([], []))


class NoopSteps(unittest.TestCase):
    """func_800B345C (2026-10-19) matched with `h++; h--;` and `p++; p++; p--;`."""

    def test_pairs_are_hacks(self):
        for s in ("int *h; h++; h--;", "int *h; h--;\n    h++;", "int *p; p++; p++; p--;"):
            self.assertIn("no-op ++/-- pair", lint(s)[1], s)

    def test_real_steps_are_clean(self):
        for s in ("int *p, i; p++; i++;", "int *p; p++; p++;", "int i; i--; g = i; i++;"):
            self.assertEqual(lint(s), ([], []), s)


class WriteOnlyLocals(unittest.TestCase):
    """func_800B4444 (2026-10-21) matched with `if (*p != 0) { j = k + 1; k = j; }`: j and k are
    only read to compute each other, so every store is dead."""

    def test_a_cycle_is_a_hack(self):
        s = "int *p, i, j, k;\n    for (i = 0; i < 8; i++) {\n        if (*p != 0) {\n            j = k + 1;\n            k = j;\n        }\n        p++;\n    }"
        self.assertIn("dead store", lint(s)[1])

    def test_used_or_effectful_values_are_clean(self):
        for s in ("int j, k; j = g; k = j; g = k;",           # k reaches a global
                  "int j, k; j = g + 1; k = j; if (k) g = 0;",  # k is tested
                  "int j; j = g;",                             # a lone unused store
                  "int j, k; j = f2(); k = j;"):              # a call: the store has an effect
            self.assertNotIn("dead store", lint(s)[1], s)

    def test_nested_declarations_dont_hide_earlier_reads(self):
        s = "int *p, *q;\n    p = (int *)g;\n    q = p + 1;\n    if (p[0] == 2) { int k; k = 0; g = k; }\n    g = (int)q;"
        self.assertNotIn("dead store", lint(s)[1])


class OverwrittenStores(unittest.TestCase):
    """Roo_FindNewSquare (2026-10-21) matched with `z = D_800C5BEC;` overwritten before any read."""

    def test_overwritten_before_a_read_is_a_hack(self):
        for s in ("int z; z = g;\n    g = 1;\n    z = g + 2;\n    g = z;",
                  "int z; z = g;\n    switch (g) { case 1: g = 2; break; }\n    z = 3;\n    g = z;",
                  "int i, z; z = g;\n    for (i = 0; i < 4; i++) { if (i) break; }\n    z = 3;\n    g = z;"):
            self.assertIn("dead store", lint(s)[1], s)

    def test_a_field_of_the_same_name_is_not_a_read(self):
        """func_8001AF2C: `in = &D_800569CC.in; in = &box->center;`."""
        s = "int *in;\n\n    in = &s.in;\n    in = &g;\n    g = *in;"
        self.assertIn("dead store", lint(s, head="struct T { int in; } s;\nvoid f(void)\n")[1])

    def test_live_first_stores_are_clean(self):
        for s in ("int z; z = g;\n    z = z >> 2;\n    g = z;",               # the second store reads it
                  "int z; z = g;\n    if (g) { g = z; }\n    z = 3;\n    g = z;",  # read between
                  "int z; z = g;\n    if (g) z = 3;\n    g = z;",            # the second store is conditional
                  "int z; z = f2();\n    z = 3;\n    g = z;",                 # a call
                  "int i, z; for (i = 0; i < 4; i++) { z = g; if (i) break; z = 3; g = z; }\n    g = z;"):
            self.assertNotIn("dead store", lint(s)[1], s)


class SameBranches(unittest.TestCase):
    """PlotHelicopterTA (2026-10-21) matched with the same test in both arms of `if (i)`."""

    def test_identical_arms_are_a_hack(self):
        for s in ("if (g) { g = 1; } else { g = 1; }", "if (g)\n        g = 1;\n    else\n        g = 1;"):
            self.assertIn("identical if/else arms", lint(s)[1], s)

    def test_different_arms_are_clean(self):
        for s in ("if (g) { g = 1; } else { g = 2; }", "if (g) { g = 1; } else if (g) { g = 1; }",
                  "if (g) { } else { }"):
            self.assertNotIn("identical if/else arms", lint(s)[1], s)


class DoWhileZero(unittest.TestCase):
    """func_800BE158's checkpoint (2026-10-22) matched only with `do { do { dx = -1; } while (0); }
    while (0);` around each case's assignment; 176 such wrappers were in matched C, unflagged."""

    def test_wrappers_are_hacks(self):
        for s in ("int x; do { x = g; } while (0); g = x;",
                  "int x; do {\n        do {\n            x = -1;\n        } while (0);\n    } while (0);\n    g = x;",
                  "do { if (g) { g = 1; } } while(0);"):
            self.assertIn("do { } while (0) wrapper", lint(s)[1], s)

    def test_real_loops_are_clean(self):
        for s in ("int i = 0; do { i++; } while (i < 4); g = i;", "while (0) { g = 1; }",
                  "int i; for (i = 0; i < 4; i++) { do { g++; } while (g < 2); }"):
            self.assertNotIn("do { } while (0) wrapper", lint(s)[1], s)


class Helpers(unittest.TestCase):
    """PlotBalloon (2026-10-22) moved its do { } while (0) into header macros (OVL64_*) to pass
    the lint: a macro or static inline the function uses is linted too."""

    def lint_with(self, header, body):
        d = tempfile.mkdtemp()
        with open(os.path.join(d, "h.h"), "w") as fh:
            fh.write(header)
        src = os.path.join(d, "f.c")
        with open(src, "w") as fh:
            fh.write(f'#include "h.h"\nint g;\nvoid f(void)\n{{\n{body}\n}}\n')
        return decomp.lint_function(src, "f")[1]

    def test_macro_do_while_is_a_hack(self):
        h = "#define POP(x)          \\\n    do {                \\\n        if (x) {        \\\n            for (;;) { if (g) { g = 0; } } \\\n        }               \\\n    } while (0)\n"
        self.assertTrue(any(s.startswith("do { } while (0) wrapper in POP") for s in self.lint_with(h, "POP(g);")))

    def test_inline_and_nested_macros(self):
        h = ("static inline int Get(void) {\n    int x;\n    do { x = g; } while (0);\n    return x;\n}\n"
             "#define WRAP() (Get() + 1)\n")
        self.assertTrue(any(s.startswith("do { } while (0) wrapper in Get") for s in self.lint_with(h, "g = WRAP();")))

    def test_accepted_idiom_is_clean(self):
        """OVL66_SET_CLOSEST (2026-10-22): a macro the human accepted is not a hack anywhere."""
        h = "#define SET(b, v) do { (b) = (v); } while (0)\n"
        d = tempfile.mkdtemp()
        with open(os.path.join(d, "h.h"), "w") as fh:
            fh.write(h)
        src = os.path.join(d, "f.c")
        with open(src, "w") as fh:
            fh.write('#include "h.h"\nint g;\nvoid f(void)\n{\n    SET(g, 1);\n}\n')
        body = decomp.extract_function(open(src).read(), "f")
        self.assertTrue(decomp.helper_hacks(src, body, idioms={}))
        self.assertEqual(decomp.helper_hacks(src, body, idioms={"SET": "why"}), [])

    def test_unused_or_clean_helpers_are_clean(self):
        h = "#define POP(x) do { x = 0; } while (0)\n#define ADD(x) ((x) + 1)\n"
        self.assertEqual(self.lint_with(h, "g = ADD(g);"), [])

    def test_deep_nesting_in_a_body(self):
        s = "do {\n        for (;;) { if (g) { g = 0; } else { g = 1; } }\n    } while (0);"
        self.assertIn("do { } while (0) wrapper", lint(s)[1])


class KandR(unittest.TestCase):
    def test_definition_found(self):
        text = "void f(void);\nvoid f(a, b)\n    u8 *a;\n    s32 b;\n{\n    *a = b;\n}\n"
        self.assertEqual(decomp.extract_function(text, "f").strip(), "void f(a, b)\n    u8 *a;\n    s32 b;\n{\n    *a = b;\n}")

    def test_prototype_and_call_are_not_definitions(self):
        self.assertIsNone(decomp.func_def_regex("f").search("void f(void);\nint x;\n{\n"))
        self.assertIsNone(decomp.func_def_regex("f").search("void g(void) {\n    f(1);\n    x = 2;\n}\n"))

    def test_linted(self):
        self.assertIn("inline asm with instructions", lint('asm("nop");', head="void f(a)\nu8 *a;\n")[0])


if __name__ == "__main__":
    unittest.main()
