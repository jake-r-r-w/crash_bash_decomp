"""Tests for the permuter's faster setup: decomp.py's file_decls (the file's own declarations in
base.c) and prune_context (only the declarations the function needs), and permuter_fast.py
(the compile pipeline's Python filters in-process). Run: tools/dk make test-tools

Byte-identical output is checked on real units by the run itself (permuter_fast compares its
first compile with bash's; permute_run compiles the pruned base against the full one)."""
import os
import subprocess
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, ".."))
import decomp  # noqa: E402

HAVE_PERMUTER = os.path.exists(decomp.PERMUTER)
if HAVE_PERMUTER:
    import permuter_fast  # noqa: E402

FILE = """#include "common.h"
#include "bash/x.h"

extern s32 D_80001000;
#define TWICE(x) ((x) * 2)

static inline s32 helper(s32 a) {
    return a + 1;
}

s32 other(s32 a) {
    if (a) {
        return 1;
    }
    return 0;
}

void kr(a) s32 a; {
    D_80001000 = a;
}

#ifdef NON_MATCHING
void target(s32 a) {
    D_80001000 = TWICE(helper(a));
}
#else
INCLUDE_ASM("asm/nonmatchings/x", target);
#endif
"""


class FileDecls(unittest.TestCase):
    def setUp(self):
        self.out = decomp.file_decls(FILE, "target")

    def test_keeps_externs_and_macros(self):
        self.assertIn("extern s32 D_80001000;", self.out)
        self.assertIn("#define TWICE(x)", self.out)

    def test_other_definitions_become_prototypes(self):
        self.assertIn("s32 other(s32 a);", self.out)
        self.assertNotIn("return 1;", self.out)

    def test_inline_helpers_stay_whole(self):
        self.assertIn("return a + 1;", self.out)

    def test_kr_definition_becomes_an_empty_prototype(self):
        self.assertIn("void kr();", self.out)

    def test_includes_and_the_open_ifdef_go(self):
        self.assertNotIn("#include", self.out)
        self.assertNotIn("#ifdef NON_MATCHING", self.out)
        self.assertNotIn("target", self.out)


BASE = """#pragma _permuter latedefine start
#pragma _permuter define gte_ldv0(r0) __asm__ volatile ( "lwc2 $0, 0( %0 )" : : "r"( r0 ) )
#pragma _permuter define gte_rtps() __asm__ volatile ( "nop;" )
void gte_ldv0();
void gte_rtps();
#pragma _permuter latedefine end
typedef int s32;
typedef short s16;
typedef struct Unused { s32 a; } Unused;
enum Kind { KIND_A, KIND_B };
struct Inner { s16 x; };
typedef struct Outer { struct Inner in; s32 n[KIND_B + 1]; } Outer;
extern Outer D_80001000;
extern s32 D_80002000;
s32 unused_fn(Unused *u);
static inline s32 helper(Outer *o) { return o->in.x; }
void target(s32 a) {
    PERM_RANDOMIZE(
    D_80001000.n[0] = helper(&D_80001000) + a;
    )
    gte_ldv0(&a);
}
"""


@unittest.skipUnless(HAVE_PERMUTER, "decomp-permuter not installed (run in tools/dk)")
class PruneContext(unittest.TestCase):
    def setUp(self):
        self.text, self.ctx = decomp.prune_context(BASE, "target")

    def test_keeps_the_closure(self):
        for name in ("Outer", "struct Inner", "KIND_B", "D_80001000", "helper", "s16"):
            self.assertIn(name, self.ctx)

    def test_drops_the_rest(self):
        for name in ("Unused", "D_80002000", "unused_fn"):
            self.assertNotIn(name, self.ctx)

    def test_function_stays_verbatim(self):
        self.assertTrue(self.text.endswith(BASE[BASE.index("void target"):]))

    def test_latedefine_keeps_only_used_macros(self):
        self.assertIn("define gte_ldv0", self.text)
        self.assertNotIn("gte_rtps", self.text)
        self.assertIn("latedefine start", self.text)

    def test_no_used_macro_no_block(self):
        text, _ = decomp.prune_context(BASE.replace("    gte_ldv0(&a);\n", ""), "target")
        self.assertNotIn("#pragma", text)


@unittest.skipUnless(HAVE_PERMUTER, "decomp-permuter not installed (run in tools/dk)")
class Fast(unittest.TestCase):
    def test_parse_reads_the_generated_pipeline(self):
        path = os.path.join(HERE, "_compile.sh")
        with open(path, "w") as f:
            f.write('#!/bin/bash\nset -eo pipefail\ncd /work\nmipsel-linux-gnu-cpp -P -undef -DPSX "$1" | '
                    "/opt/gcc/2.8.1-psx/cc1 -quiet -O2 | python3 tools/scripts/dmpsx.py | "
                    'mipsel-linux-gnu-as -Iinclude -o "$3"\n')
        try:
            cwd, stages, cpp = permuter_fast.parse(path)
        finally:
            os.remove(path)
        self.assertEqual(cwd, "/work")
        self.assertEqual([k for k, _, _ in stages], ["exe", "exe", "py", "exe"])
        self.assertIn("PSX", cpp)

    def test_parse_refuses_other_shell(self):
        path = os.path.join(HERE, "_compile.sh")
        with open(path, "w") as f:
            f.write('cc1 "$1" > /tmp/x.s && as -o "$3" /tmp/x.s\n')
        try:
            self.assertIsNone(permuter_fast.parse(path))
        finally:
            os.remove(path)

    def test_cpp_noop(self):
        names = {"PSX", "GVAL"} | permuter_fast.CPP_BUILTINS
        self.assertTrue(permuter_fast.cpp_noop("int f(void) {\n  return 1;\n}\n", names))
        self.assertFalse(permuter_fast.cpp_noop("#define A 1\nint f(void) { return A; }\n", names))
        self.assertFalse(permuter_fast.cpp_noop("int f(void) { return PSX; }\n", names))
        self.assertFalse(permuter_fast.cpp_noop("int f(void) { return __LINE__; }\n", names))
        self.assertFalse(permuter_fast.cpp_noop("int f(void) { /* c */ return 1; }\n", names))

    def test_relocs_compare_by_address(self):
        # the target's D_8005A677 and our D_8005A614 + 0x63 (a struct field) link the same
        lo, hi = "R_MIPS_LO16\tD_8005A614", "R_MIPS_HI16\tD_8005A614"
        r = permuter_fast.mips_reloc
        self.assertEqual(r(lo, "", "D_8005A614", "0x63"), r("R_MIPS_LO16\tD_8005A677", "", "D_8005A677", "0"))
        self.assertEqual(r(hi, "", "D_8005A614", "0"), r("R_MIPS_HI16\tD_8005A677", "", "D_8005A677", "0"))
        self.assertEqual(r(lo, "", "D_8005A614", "0x63"), "%lo(0xa677)")
        self.assertEqual(r(hi, "", "D_8005A614", "0"), "%hi(0x8006)")
        self.assertNotEqual(r(lo, "", "D_8005A614", "0x64"), "%lo(0xa677)")

    def test_relocs_by_name_need_the_symbol_file(self):
        r = permuter_fast.mips_reloc
        old = dict(permuter_fast._symbols)
        try:
            permuter_fast._symbols.clear()
            self.assertEqual(r("R_MIPS_LO16\tHeroes", "", "Heroes", "4"), "%lo(Heroes+4)")
            permuter_fast._symbols["Heroes"] = 0x80051000
            self.assertEqual(r("R_MIPS_LO16\tHeroes", "", "Heroes", "4"), "%lo(0x1004)")
        finally:
            permuter_fast._symbols.clear()
            permuter_fast._symbols.update(old)

    def test_in_process_filter_matches_the_script(self):
        script = os.path.join(HERE, "..", "dmpsx.py")
        asm = "\t#APP\n\t.word 0x0000007f\n\t#NO_APP\n\tjr $31\n"
        want = subprocess.run([sys.executable, script], input=asm, capture_output=True, text=True).stdout
        got = permuter_fast.run_py(permuter_fast.load(os.path.abspath(script)), [script], asm.encode(), False)
        self.assertEqual(got.decode(), want)


if __name__ == "__main__":
    unittest.main()
