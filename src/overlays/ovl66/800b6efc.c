#include "common.h"
#include "bash/main.h"
#include "bash/ovl66.h"
#include "bash/ovl66_800b6efc.h"
#include "bash/bonk.h"

void func_800B6EFC(s32 arg0) {
    VECTOR sp18;
    HeroInfo *h;
    HeroInfo *base;
    EXTengine *pu;
    subengine *sub;
    Ovl66Slot *sbase;
    NAS *nas;
    Ovl66Slot *slot;

    base = Heroes;
    h = base + arg0;
    if (func_80015780(h->hero_eng, h->hero_sub, 1, &sp18) == NULL) {
        *(SLONG_XYZ *)&sp18 = h->hero_eng->unk10;
    }
    nas = SpawnNASById(sp18.vx, D_8005A5D8 - 0x80, sp18.vz, 0x1A03, 0x11);
    sbase = D_800B9F4C;
    slot = sbase + arg0;
    if (slot->f8 == 1) {
        PlaySfx(0x72D, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        if (nas != NULL) {
            func_80028018(arg0, 5);
            nas->UserInfo.user2 = 0x8000;
            func_800B4910(&sp18, 0x600, arg0, 0x100);
            pu = h->hero_powerup;
            sub = NULL;
            if (pu != NULL) {
                sub = pu->sub;
            }
            if (sub != NULL) {
                pu->flags &= 0xFFFF7FFF;
            }
            slot->f8 = 3;
        }
    } else {
        func_800B4910(&sp18, 0x380, arg0, 0);
    }
    PlaySfx(D_800B9F0C[arg0], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
}

void func_800B70C4(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    if (h->hero_frozen == 0 || h->hero_mode == 0x43) {
        if (!(h->hero_ctrl & 1)) {
            h->hero_eng->unk1E += func_800154A0(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
        }
        h->hero_ctrl &= 0xFFFE;
        h->hero_eng->unk5E = 1;
    }
}

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

void func_800B738C(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    if (func_8008ABF8() == 0 && func_80086FCC(arg0) == 0 && func_800B8D88(arg0) == 0) {
        func_8008502C(arg0);
        if (D_8005A5DC < 4) {
            if (D_8005A5DC >= 2) {
                D_8009D73C(arg0);
            }
        }
        base = Heroes;
        h = base + arg0;
        if (h->hero_status & 0x100) {
            if (h->hero_move != NULL) {
                h->hero_move->forward_angle = 0;
                h->hero_move->facing_angle = 0;
            }
            D_800AD12C = 1;
        }
        func_800871E4(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B747C);

void func_800B76CC(s32 arg0) {
    Ovl66Target *t;
    HeroInfo *base;
    HeroInfo *h;

    if (func_800B8714(arg0) == 0 && func_800B8814(arg0) == 0 && func_800B89D8(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        h->hero_frozen = 10;
        if (func_800B4454(arg0, &t, 0x306) != 0) {
            Ovl66Pos *p;
            D_800B9F4C[arg0].f0 = (s32)t;
            p = t->unk4->unk54;
            h->hero_move->forward_angle =
                get_angle(h->hero_eng->unk10.x - p->x, h->hero_eng->unk10.z - p->z);
        }
        if (func_8008502C(arg0) == 8) {
            if (((Ovl66PadButtons *)D_80051338)[arg0].unk50 & 0x8000) {
                SetHeroAction((HeroInfo *)arg0, 0x44);
            } else {
                SetHeroAction((HeroInfo *)arg0, 0x45);
            }
        }
    }
}

void func_800B77FC(s32 arg0) {
    Ovl66Slot *s;
    Ovl66Slot *sb;

    if (func_800B8714(arg0) == 0 && func_800B8814(arg0) == 0 && func_800B89D8(arg0) == 0) {
        HeroInfo *base = Heroes;
        (base + arg0)->hero_frozen = 10;
        sb = D_800B9F4C;
        s = sb + arg0;
        if (s->f4 == 0) {
            if (func_8008502C(arg0) == 8) {
                s->f4 = 1;
            }
            if (s->f4 == 0) {
                return;
            }
        }
        if (!(((Ovl66PadButtons *)D_80051338)[arg0].unk50 & 0x8000)) {
            SetHeroAction((HeroInfo *)arg0, 0x45);
            D_800B9F4C[arg0].f4 = 0;
        }
    }
}

void func_800B7908(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    s32 act;
    Ovl66Tgt2 *t;

    if (func_800B8714(arg0) == 0 && func_800B8814(arg0) == 0 && func_800B89D8(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        h->hero_frozen = 10;
        act = func_8008502C(arg0);
        if (act == 0xC) {
            PlaySfx(D_800B9F2C[arg0], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (act == 8) {
            if (h->hero_last_mode == 0x44 || D_800B9F4C[arg0].f8 == 1 ||
                (D_8005A614[arg0 + 0x5C] != 0 && (((D5A614View *)D_8005A614)->flags & 0x80))) {
                func_800B6EFC(arg0);
            }
            t = (Ovl66Tgt2 *)D_800B9F4C[arg0].f0;
            if (t != NULL) {
                switch (t->kind) {
                case 0x100:
                    if (t->obj != NULL && (u16)t->obj->mode >= 0xB) {
                        func_800B47C8(arg0, 1, t);
                    }
                    break;
                case 0x200:
                    if (t->obj != NULL && (u16)t->obj->mode >= 0xB) {
                        func_800B64B4(arg0, 1, (Ovl66Obj *)t);
                    }
                    break;
                }
            }
            PlaySfx(D_800B9F3C[arg0], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            SetHeroAction((HeroInfo *)arg0, 0x46);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/800b6efc", func_800B7B48);

void func_800B8550(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    if (h->hero_eng->unk2C.x != 0) {
        h->hero_eng->unk2C.x -= 0x80;
        h->hero_eng->unk2C.y -= 0x80;
        h->hero_eng->unk2C.z -= 0x80;
        h->hero_eng->unk5E = 1;
        if (h->hero_eng->unk2C.x <= 0) {
            Ovl66SpawnPuff(&h->hero_eng->unk10, 1, D_80053C7C);
            if (Heroes[arg0].hero_shadow != NULL) {
                Heroes[arg0].hero_shadow->flags &= 0xFFFF7FFF;
            }
        }
    }
    Heroes[arg0].hero_frozen = 2;
    func_8008502C(arg0);
}

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

void func_800B886C(s32 idx) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + idx;
    D5A614View *g;

    h->hero_frozen = 2;
    if (h->hero_move != NULL) {
        h->hero_move->forward_angle = 0;
        h->hero_move->facing_angle = 0;
    }
    func_8008502C(idx);
    if (func_8008A200(idx) != 0) {
        g = (D5A614View *)D_8005A614;
        if (g->unk4 & 0x800) {
            g->unk30 = g->unk30 | 0x201;
        } else {
            g->unk30 = g->unk30 | 1;
        }
    }
}

void func_800B8918(s32 arg0) {
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
    func_80089FA4(arg0);
    SetHeroSeq((HeroInfo *)arg0, 0x39);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
    D_800AD12C = 1;
}

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

s32 func_800B8F64(EXTengine *eng) {
    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;
    s32 limit;
    s32 ret = 0;
    s32 ang;

    if (eng->collision_box.flags & 0x40) {
        limit = 0x880 - eng->collision_box.radius;
    } else {
        limit = 0x880;
    }
    if (x * x + z * z >= limit * limit) {
        ang = get_angle(x, z);
        eng->unk10.x = limit * D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> 12;
        eng->unk10.z = limit * D_80068BD4[ang & 0xFFF].unk2 >> 12;
        ret = 1;
    }
    return ret;
}

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
