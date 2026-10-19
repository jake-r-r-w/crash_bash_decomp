#include "common.h"
#include "bash/ovl5d.h"
#include "bash/ovl5d_800bb64c.h"
#include "bash/bash.h"
#include "bash/xtras.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BB64C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BB7B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BB7E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BBA14);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BBD48);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BC038);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", InitCratePickup);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", InitShieldPickup);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", InitBootsPickup);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", InitAnvilPickup);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", InitSlowmoPickup);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BE9D0);

void func_800BEDE8(s32 arg0, s32 arg1) {
    HeroInfo *base = Heroes;
    HeroInfo *b = base + arg1;
    HeroInfo *a;

    if ((b->hero_immunity | b->hero_hit) == 0) {
        a = base + arg0;
        if (!(a->hero_status & 0xFA00) && !(b->hero_status & 0xFA00)) {
            switch (a->hero_pu) {
            case 0x19:
            case 0x1E:
                break;
            case 0x14:
                if (a->hero_pu_lock == 0) {
                    b->hero_pu_timer = a->hero_pu_timer + 10;
                    b->hero_pu = a->hero_pu;
                    b->hero_pu_lock = 0x1E;
                    a->hero_pu = 0x1E;
                    a->hero_pu_timer = 0;
                    b->hero_powerup->sub->unk74 = a->hero_powerup->sub->unk74;
                    PlaySfx(0x454, 0, 0, 0x1000, 0x1E00, 0);
                }
                break;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BEF00);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BF404);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BF708);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BF928);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BFAC0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BFB6C);

void func_800BFC30(EXTengine *eng) {
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BFD2C);
