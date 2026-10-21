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
