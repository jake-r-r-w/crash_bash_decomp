#include "common.h"
#include "bash/main.h"
#include "bash/ovl66.h"
#include "bash/ovl66_800b6efc.h"
#include "bash/bonk.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B6EFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B70C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B717C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B72FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B738C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B747C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B76CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B77FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B7908);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B7B48);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B8550);

s32 func_800B8714(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x100) {
        SetHeroAction((HeroInfo *)arg0, 0x40);
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B876C);

s32 func_800B8814(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x48);
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B886C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B8918);

s32 func_800B89D8(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x47);
        return 1;
    }
    return 0;
}

void func_800B8A30(s32 arg0) {
    HeroInfo *hero;

    if (func_800B8714(arg0) == 0 && func_800B8814(arg0) == 0 && func_800B89D8(arg0) == 0) {
        HeroInfo *base = Heroes;
        hero = base + arg0;
        hero->hero_frozen = 10;
        if (func_8008502C(arg0) == 9) {
            SetHeroAction((HeroInfo *)arg0, 0x40);
            hero->hero_frozen = 0;
        }
    }
}

void func_800B8AC8(s32 arg0) {
    Heroes[arg0].hero_frozen = 0xA;
}

void func_800B8AF4(s32 arg0) {
    Heroes[arg0].hero_frozen = 10;
    SetHeroSeq((HeroInfo *)arg0, 0x37);
}

void func_800B8B3C(s32 arg0) {
    SetHeroSeq((HeroInfo *)arg0, 0x36);
    Heroes[arg0].hero_frozen = 10;
    D_800B9F4C[arg0].f4 = 0;
}

void func_800B8BA0(s32 arg0) {
    HeroInfo *hero;
    HeroInfo *base = Heroes;

    D_800B9F4C[arg0].f0 = 0;
    hero = base + arg0;
    hero->hero_frozen = 10;
    PlaySfx(D_800B9F1C[arg0], NULL, &hero->hero_eng->unk10, 0x1000, 0x1E00, 0);
    SetHeroSeq((HeroInfo *)arg0, 0x35);
}

void func_800B8C3C(s32 arg0) {
    if (Heroes[arg0].hero_move->reactive_speed != 0) {
        D_8009D73C();
    }
    func_8008502C(arg0);
    func_800871E4(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B8CB4);

void func_800B8D3C(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x34);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B8D88);

s32 func_800B8E08(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction((HeroInfo *)arg0, 0x40);
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B8E64);

void func_800B8EF0(void) {
}

void func_800B8EF8(s32 arg0) {
    EXTengine *p = Heroes[arg0].hero_powerup;
    subengine *s = NULL;

    if (p != NULL) {
        s = p->sub;
    }
    if (s != NULL) {
        p->flags &= 0xFFFF7FFF;
    }
    D_800B9F4C[arg0].f8 = 3;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B8F64);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B9054);

void func_800B9144(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *shadow;

    h = base + player;

    ((void (*)(EXTengine *))func_800B9054)(h->hero_eng);
    shadow = h->hero_shadow;
    if (shadow != NULL) {
        shadow->unk10 = h->hero_eng->unk10;
    }
}

void func_800B91BC(s32 arg0) {
    EXTmotion *m;

    SetHeroAction((HeroInfo *)arg0, 0x40);
    D_800B9F4C[arg0].f8 = 3;
    D_800B9F4C[arg0].f0 = 0;
    m = AddMotion(Heroes[arg0].hero);
    if (m != NULL) {
        m->Ymax = 0xA0;
        m->Ydec = 0xE;
        m->Yacc = 0;
        m->Ydir = 0xFF;
        m->Yvel = 0;
    }
    func_800B9144(arg0);
    func_800B61F8(arg0);
}

void func_800B9264(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        func_800B91BC(var_s0);
        var_s0 += 1;
    } while (var_s0 < 4);
}
