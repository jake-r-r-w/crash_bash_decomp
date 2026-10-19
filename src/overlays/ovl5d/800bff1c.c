#include "common.h"
#include "bash/ovl5d.h"
#include "bash/ovl5d_800bff1c.h"
#include "bash/bash.h"
#include "bash/xtras.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800BFF1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C013C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C02D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C05A8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C07F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C09F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C0BAC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C0DE4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C0F14);

void func_800C1190(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
    }
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C1278);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C14D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C159C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C1848);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C1A30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C284C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C2B60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C2C70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C2DAC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C2E58);

void func_800C2EC8(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x11);
}

void func_800C2F14(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x10);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C2F60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C2FB0);
