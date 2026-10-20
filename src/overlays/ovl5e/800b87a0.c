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

void func_800BA4C0(s32 arg0) {
    HeroInfo *b = Heroes;
    HeroInfo *h = &b[arg0];

    if (h->hero_pu != 0x12) {
        h->hero_hit = 0x1E;
    }
    h->hero_move->velocity_enable = 0;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    PlaySfx(D_8009E080[h->hero_name][Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
    SetHeroSeq((HeroInfo *)arg0, 9);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BA5CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BA7D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BA958);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BABA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/800b87a0", func_800BADE8);

void func_800BAFD4(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base2;

    if (Heroes[arg0].hero_pu == 0x14) {
        if (D_800C4780 != NULL) {
            StopSfx(D_800C4780);
        }
    }
    base2 = Heroes;
    h = base2 + arg0;
    h->hero_hit = 0;
    h->hero_immunity = 0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 2;
    h->hero_health = 0;
    h->hero_status |= 0x2000;
    h->hero_powerup->flags = 0;
    h->hero_fx->flags = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    func_800898EC(arg0);
}

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

void *func_800BC588(s32 idx, s32 mask) {
    NAS *n;
    EXTengine *he;
    EXTengine *e;
    EXT *fux;
    s32 hx, hz, dx, dz, lim, m;

    n = D_8005AA24;
    m = mask;
    lim = 0x10000;
    he = Heroes[idx].hero_eng;
    hx = he->unk10.x - (D_80068BD4[(he->unk1E - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    hz = he->unk10.z - (D_80068BD4[he->unk1E & 0xFFF].unk2 * 1024 >> 14);
    for (; n != NULL; n = n->Object.Child) {
        fux = n->FUX;
        if (fux == NULL) continue;
        if (!(*(u16 *)((u8 *)fux + 0x1C) & m)) continue;
        e = n->Engine;
        if (e == NULL) continue;
        dx = e->unk10.x - hx;
        dz = e->unk10.z - hz;
        if (dx * dx + dz * dz < lim) return fux;
    }
    return 0;
}

void func_800BC698(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    if (func_8008ABF8(arg0) != 0) {
        base = Heroes;
        h = base + arg0;
        if ((u16)(h->hero_name - 1) < 8) {
            h->hero_sub->unk6C = D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
        } else {
            h->hero_sub->unk6C = NULL;
        }
    } else {
        func_80089BE8(arg0);
    }
}

void func_800BC744(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0x17);
    if (h->hero_object != NULL) {
        h->hero_object->can_do |= 0x20;
    }
    h->hero_weapon_timer = 0;
}

void func_800BC7B4(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x16);
}

void func_800BC800(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x15);
}

void func_800BC84C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0x14);
    h->hero_weapon_timer = 0;
}
