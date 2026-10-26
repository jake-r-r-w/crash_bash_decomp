"""Tests for the swarm's header reconcile (headers.py): the four declaration clashes of
2026-10-19, each fixed by keeping the tree's declaration. Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import headers  # noqa: E402


class Names(unittest.TestCase):
    def test_kinds(self):
        n = headers.names
        self.assertEqual(n("extern s32 D_80054F84;\n"), {"D_80054F84"})
        self.assertEqual(n("extern TankUnk74 *D_8009D60C;\n"), {"D_8009D60C"})
        self.assertEqual(n("extern NAS *D_800CB264[8];   /* objects */\n"), {"D_800CB264"})
        self.assertEqual(n("void func_800BC490(s32 arg0);\n"), {"func_800BC490"})
        self.assertEqual(n("typedef struct {\n    s32 x;\n} TankPoint;\n"), {"TankPoint"})
        self.assertEqual(n("typedef struct PogoPad {\n    s32 a;\n} PogoPad;\n"),
                         {"struct PogoPad", "PogoPad"})
        self.assertEqual(n("struct EXTpolarAI {\n    s32 id;\n};\n"), {"struct EXTpolarAI"})
        self.assertEqual(n("typedef void (*Fn)(s32);\n"), {"Fn"})
        self.assertEqual(n("BASH_SIZE_CHECK(EXTbombmaker, 0x1C);\n"), set())


BASE = """#include "common.h"

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ s16 unk6;
} PogoSlot;

extern PogoSlot D_800C535C[];
extern s32 D_80054F84;
void func_800BC490(s32 arg0);
"""


class Reconcile(unittest.TestCase):
    def check(self, new, known=None):
        return headers.reconcile(BASE, new, known or {})

    def test_unchanged_header_is_untouched(self):
        self.assertEqual(self.check(BASE), (BASE, []))

    def test_struct_merged_twice_keeps_the_tree(self):  # func_800BC184: duplicate member unk4
        new = BASE.replace("    /* 0x06 */ s16 unk6;\n",
                           "    /* 0x06 */ s16 unk6;\n    /* 0x04 */ u16 unk4;\n    /* 0x06 */ u8 unk6[0xA];\n")
        text, dropped = self.check(new)
        self.assertEqual(text, BASE)
        self.assertEqual(len(dropped), 1)

    def test_retyped_global_keeps_the_tree(self):  # InitRaceSmokeBoost: conflicting D_80054F84
        new = BASE.replace("extern s32 D_80054F84;\n",
                           "extern s32 D_80054F84;\nextern ScriptInit D_800C6C8C;\nextern GfxModel D_80054F84;\n")
        text, _ = self.check(new)
        self.assertEqual(text, BASE.replace("extern s32 D_80054F84;\n",
                                            "extern s32 D_80054F84;\nextern ScriptInit D_800C6C8C;\n"))

    def test_rewritten_in_place_gets_the_tree_form_back(self):
        new = BASE.replace("extern s32 D_80054F84;\n", "extern GfxModel D_80054F84;\n")
        self.assertEqual(self.check(new)[0], BASE)

    def test_prototype_from_another_header_dropped(self):  # func_800BC184's SetHeroAction(s32, s32)
        new = BASE + "void SetHeroAction(s32 hero, s32 action);\nvoid func_800C1384(s32 a);\n"
        known = {"SetHeroAction": "void SetHeroAction(HeroInfo *hero, s32 action);\n"}
        self.assertEqual(self.check(new, known)[0], BASE + "void func_800C1384(s32 a);\n")

    def test_new_struct_added_twice_kept_once(self):  # func_800C6B9C: TankPoint defined twice
        point = "typedef struct {\n    s32 x;\n    s32 z;\n} TankPoint;\n"
        new = BASE + point + "\n" + point
        text, dropped = self.check(new)
        self.assertEqual(text.count("} TankPoint;"), 1)
        self.assertEqual(len(dropped), 1)

    def test_new_declarations_kept(self):  # func_800C5620's new extension struct
        ext = "typedef struct TankExt1605 {\n    s32 unkC;\n} TankExt1605;\n"
        self.assertEqual(self.check(BASE + ext), (BASE + ext, []))


class Visible(unittest.TestCase):
    HEADS = {
        "include/bash/tank_ovl.h": '#include "common.h"\n#include "bash/nas.h"\n',
        "include/bash/nas.h": "typedef struct NAS NAS;\n",
        "include/bash/engine.h": "typedef struct EngineHooks EngineHooks;\n",
        "include/bash/pogo_ovl.h": "typedef struct PogoHooks PogoHooks;\n",
    }
    TANK = '#include "common.h"\n#include "bash/tank_ovl.h"\n#include "bash/engine.h"\n'
    POGO = '#include "bash/pogo_ovl.h"\n'

    def test_only_headers_its_files_include(self):  # func_800C8A80: a type tank.c never sees
        self.assertEqual(headers.visible("include/bash/tank_ovl.h", [self.TANK, self.POGO], self.HEADS),
                         {"include/bash/nas.h", "include/bash/engine.h"})

    def test_every_including_file_must_see_it(self):
        other = '#include "bash/tank_ovl.h"\n'
        self.assertEqual(headers.visible("include/bash/tank_ovl.h", [self.TANK, other], self.HEADS),
                         {"include/bash/nas.h"})

    def test_header_nobody_includes_sees_nothing(self):
        self.assertEqual(headers.visible("include/bash/pogo_ovl.h", [self.TANK], self.HEADS), set())


if __name__ == "__main__":
    unittest.main()
