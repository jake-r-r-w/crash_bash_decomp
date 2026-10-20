#include "common.h"
#include "bash/main.h"
#include "bash/ovl66.h"
#include "bash/ovl66_800b6efc.h"
#include "bash/bonk.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B6EFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B70C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B717C);

void func_800B72FC(s32 idx) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + idx;

    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)idx, 0x33);
    if (h->hero_ctrl & 4) {
        EXTengine *e = h->hero_eng;
        e->unk2C.z = 0x1000;
        e->unk2C.y = 0x1000;
        e->unk2C.x = 0x1000;
        h->hero_ctrl &= 0xFFFB;
    }
}

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

void func_800B876C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *p = h->hero_powerup;
    subengine *s = NULL;

    if (p != NULL) {
        s = p->sub;
    }
    if (s != NULL) {
        p->flags &= 0xFFFF7FFF;
    }
    D_800B9F4C[arg0].f8 = 3;
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
    SetHeroSeq((HeroInfo *)arg0, 0x33);
    func_80026980(h->hero);
}

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

void func_800B8CB4(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->velocity_enable = 0;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq((HeroInfo *)arg0, 0x38);
    h->hero_hit = 0x1E;
}

void func_800B8D3C(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x34);
}

s32 func_800B8D88(s32 arg0) {
    if (D_8009D728(arg0) == 0) {
        if (Heroes[arg0].hero_move->forward_speed == 0) {
            return 0;
        }
        SetHeroAction((HeroInfo *)arg0, 0x41);
    }
    return 1;
}

s32 func_800B8E08(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction((HeroInfo *)arg0, 0x40);
        return 1;
    }
    return 0;
}

s32 func_800B8E64(s32 arg0) {
    if ((((Ovl66PadButtons *)D_80051338)[arg0].unk50 & 0x8000) && Heroes[arg0].hero_mode != 0x43) {
        SetHeroAction((HeroInfo *)arg0, 0x43);
        return 1;
    }
    return 0;
}

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

s32 func_800B9054(EXTengine *eng) {
    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;
    s32 limit;
    s32 ret = 0;
    s32 ang;

    if (eng->collision_box.flags & 0x40) {
        limit = 0x980 - eng->collision_box.radius;
    } else {
        limit = 0x980;
    }
    if (x * x + z * z >= limit * limit) {
        ang = get_angle(x, z);
        eng->unk10.x = limit * D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> 12;
        eng->unk10.z = limit * D_80068BD4[ang & 0xFFF].unk2 >> 12;
        ret = 1;
    }
    return ret;
}

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
