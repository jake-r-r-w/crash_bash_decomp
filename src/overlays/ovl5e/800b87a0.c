#include "common.h"
#include "bash/main.h"
#include "bash/ovl5e.h"
#include "bash/ovl5e_800b87a0.h"
#include "bash/papu.h"
#include "bash/bash.h"
#include "bash/xtras.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800B87A0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800B9E74);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BA22C);

void func_800BA3C4(EXTengine *eng) {
    NAS *nas;
    EXTengine *other;
    s32 again;

    do {
        again = 0;
        nas = D_8005AA2C;
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        while (nas != NULL) {
            if (nas->Type == D_8009E464) {
                other = nas->Engine;
                if (other != NULL && other->unk10.x == eng->unk10.x && other->unk10.z == eng->unk10.z) {
                    eng->unk10.x = Random(0xC00) - 0x600;
                    eng->unk10.z = Random(0xC00) - 0x600;
                    nas = NULL;
                    again = 1;
                }
            }
            if (nas != NULL) {
                nas = nas->Solid.Child;
            }
        }
    } while (again);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BA4C0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BA5CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BA7D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BA958);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BABA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BADE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BAFD4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BB0D4);

void func_800BB308(s32 arg0) {
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BB3F0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BB69C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BB86C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BC274);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BC588);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BC698);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BC744);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BC7B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BC800);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BC84C);
