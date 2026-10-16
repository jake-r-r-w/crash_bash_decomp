"""Tests for decomp.py port's parts: reading an old diff and splitting what it added into whole
declarations. Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

DIFF = """diff --git a/include/bash/engine.h b/include/bash/engine.h
--- a/include/bash/engine.h
+++ b/include/bash/engine.h
@@ -85,7 +85,12 @@ extern s32 D_8009AE3C;
+extern u16 D_800A00D0[];
+/* a helper */
+static inline void helper(s32 e) {
+    D_800A00D0[0] = e;
+}extern u8 D_800A015C[];
@@ -345,7 +360,9 @@ void func_8009342C(void);
+    /* 0x04 */ s32 unk4;
diff --git a/src/overlays/engine/engine.c b/src/overlays/engine/engine.c
--- a/src/overlays/engine/engine.c
+++ b/src/overlays/engine/engine.c
@@ -1194,7 +1194,9 @@ void func_800936C4(void) {
-INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093854);
+void func_80093854(void) {
+    helper(1);
+}
"""


class Port(unittest.TestCase):
    def test_diff_parts(self):
        body, added = decomp.diff_parts(DIFF, {"func_80093854"})
        self.assertEqual(body, "void func_80093854(void) {\n    helper(1);\n}")
        self.assertIn("extern u16 D_800A00D0[];", added)
        self.assertNotIn("void func_80093854(void) {", added)

    def test_units_split_a_glued_brace(self):
        _, added = decomp.diff_parts(DIFF, {"func_80093854"})
        units = decomp.decl_units(added)
        self.assertEqual([u[0] for u in units],
                         ["extern u16 D_800A00D0[];", "/* a helper */", "extern u8 D_800A015C[];", "    /* 0x04 */ s32 unk4;"])
        self.assertEqual(units[1][-1], "}")

    def test_unit_name(self):
        self.assertEqual(decomp.unit_name(["typedef struct A {", "    s16 x;", "} A;"]), "A")
        self.assertEqual(decomp.unit_name(["struct B {", "    s32 y;", "};"]), "B")


if __name__ == "__main__":
    unittest.main()
