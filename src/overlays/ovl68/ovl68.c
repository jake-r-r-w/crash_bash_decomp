#include "common.h"
#include "bash/ovl68.h"
#include "bash/chase.h"
#include "bash/pogo.h"
#include "bash/bash.h"
#include "bash/xtras.h"

void func_800B3330(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B3338);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B3488);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl68/ovl68", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B37C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B3EFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B4048);

void func_800B4444(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    u8 *g;
    u8 *h;
    NAS **p;

    i = 0;
    p = D_800BC424;
loop:
    if (i < 8) {
        if (*p != 0) {
            j = k + 1;
            k = j;
        }
        p++;
        i++;
        goto loop;
    }
    g = D_80052A54;
    if (*(s32 *)(g + 0x1544) != 0) {
        func_80029100(g + 0x1548);
    }
    n = 3;
    do {
        n--;
    } while (n >= 0);
    h = D_80053F8C;
    func_80029100(h + 0x10);
    ((void (*)(s32 *))func_800B3330)(D_800BC240);
}

void func_800B44C8(void) {
    HeroInfo *h;
    s32 i;
    s32 best;
    s32 max;

    max = 0;
    best = -1;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (!(h->hero_status & 0xFA00) && max < h->hero_health) {
            max = h->hero_health;
            best = i;
        }
    }
    if (best != -1) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00) && i != best) {
                func_80084E04(i, 0, -1, 0x1000, 0);
            }
        }
    }
}

s32 func_800B459C(s32 *p) {
    s32 d;
    s32 a;
    if (p[4] < -D_800BC44C || D_800BC44C < p[4] || p[6] < -D_800BC44C || D_800BC44C < p[6]) {
        d = (csqrt(p[4] * p[4] + p[6] * p[6]) >> 6) - D_800BC448;
        if (d > 0) {
            a = get_angle(p[4], p[6]);
            p[4] -= (d * D_80068BD4[(a - 0x400) & 0xFFF].unk2) >> 12;
            p[6] -= (d * D_80068BD4[a & 0xFFF].unk2) >> 12;
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B46BC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B48E4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B4B34);

s32 func_800B51C8(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_move->jump_max != 0) {
        if (func_80025BBC(h->hero_eng) != 0) {
            h->hero_move->jump_max = 0;
            h->hero_sub->unk08 = 0;
        } else {
            if (h->hero_ctrl & 8) {
                return 0;
            }
            if (D_8005A5D8 < h->hero_eng->unk10.y + h->hero_sub->unk08) {
                if (D_8005B5B8 == 0x7FFFFFFF) {
                    SetHeroAction((HeroInfo *)arg0, 0x79);
                    return 1;
                }
                h->hero_eng->unk10.y = D_8005A5D8;
                h->hero_sub->unk08 = 0;
            }
        }
    } else {
        func_800863D0(arg0);
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B52C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B5CD0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B5E84);

void func_800B6168(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *p;
    s32 idx;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    SetHeroSeq((HeroInfo *)arg0, 0x65);
    idx = 0;
    if (D_8005A614[arg0 + 0x5C] == 0) {
        idx = h->hero_name;
    }
    h->hero_move->jump_vel = D_8009D6E8.unk10[idx].vel;
    h->hero_move->jump_max = D_8009D6E8.unk10[idx].max;
    h->hero_move->jump_acc = D_8009D6E8.unk10[idx].acc;
    h->hero_move->jump_dec = D_8009D6E8.unk10[idx].dec;
    h->hero_move->jump_dir = -1;
}

void func_800B627C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *e;
    EXTengine *p;

    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    h->hero_move->jump_vel = 8;
    h->hero_move->jump_max = 0x70;
    h->hero_move->jump_acc = 0xA;
    h->hero_move->jump_dec = 0;
    h->hero_move->jump_dir = 0;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq((HeroInfo *)arg0, 0x66);
}

void func_800B6350(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *e;

    if (func_80025BBC(h->hero_eng) != 0) {
        SetHeroAction((HeroInfo *)arg0, 0x7B);
    } else if (!(h->hero_ctrl & 8)) {
        e = h->hero_eng;
        if (D_8005A5D8 < e->unk10.y + h->hero_sub->unk08) {
            if (D_8005B5B8 == 0x7FFFFFFF) {
                SetHeroAction((HeroInfo *)arg0, 0x79);
            } else {
                e->unk10.y = D_8005A5D8;
                h->hero_sub->unk08 = 0;
            }
        }
    }
    func_800B52C8(arg0);
    func_8008502C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B643C);

void func_800B65B0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *o;
    EXTengine *t;
    u8 *x;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    h->hero_move->jump_max = 0;
    h->hero_sub->unk08 = 0;
    SetHeroSeq((HeroInfo *)arg0, 0x67);
    if (D_8005AA9C != NULL) {
        e = h->hero_eng;
        o = D_8005AA9C->Engine;
        t = e->platform;
        if (t != NULL) {
            t->platform = NULL;
            e->platform = NULL;
        }
        t = o->platform;
        if (t != NULL) {
            t->platform = NULL;
            o->platform = NULL;
        }
        e->platform = o;
        o->platform = e;
        x = FindExtension(D_8005AA9C, 0x1000);
        if (x != NULL && (*(u16 *)(x + 0x1C) & 8)) {
            *(u16 *)(x + 0x1E) |= 8;
        }
    }
}

static inline s32 Chk66A4_A(s32 arg0) {
    s32 ret;
    u16 st;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x75);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x74);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk66A4_B(s32 arg0) {
    s32 ret;

    if (D_80051338[arg0].unk54 & 0x4000) {
        SetHeroAction((HeroInfo *)arg0, 0x78);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk66A4_C(s32 arg0) {
    s32 ret;

    if (Heroes[arg0].hero_move->forward_speed != 0) {
        SetHeroAction((HeroInfo *)arg0, 0x72);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void func_800B66A4(s32 arg0) {
    if (!Chk66A4_A(arg0)) {
        if (!Chk66A4_B(arg0)) {
            if (!Chk66A4_C(arg0)) {
                func_800B52C8(arg0);
                func_8008502C(arg0);
                func_800B51C8(arg0);
            }
        }
    }
}

void func_800B67D8(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    func_80027F7C(arg0, 0xA, 0xF0);
    func_800BA694(arg0);
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 0;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq((HeroInfo *)arg0, 0x68);
    PlaySfx(D_800BB95C[h->hero_name - 1][Random(4)], 0, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
}

void func_800B68B8(s32 arg0) {
    u8 *g;

    if (func_8008502C() == 8 && ((g = D_8005A614), (*(u16 *)(g + 0x32) & 0x80)) && g[arg0 + 0x5C] == 0 &&
        !(*(s32 *)(g + 4) & 2)) {
        HeroInfo *base = Heroes;
        HeroInfo *h = &base[arg0];

        h->hero_immunity = 0;
        h->hero_hit += 0x28;
        SetHeroSeq((HeroInfo *)arg0, 0x61);
        if (D_800BC2D0[arg0][0] == 0) {
            SetHeroAction((HeroInfo *)arg0, 0x76);
        } else {
            SetHeroAction((HeroInfo *)arg0, 0x71);
        }
    }
    func_800B51C8(arg0);
}

void func_800B69AC(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 5;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->unk08 = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    SetHeroSeq(arg0, 0x61);
    h->hero_hit = 0x3C;
    h->hero_status |= 0x2000;
}

void func_800B6AAC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base = Heroes;

    h = base + arg0;
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
    D_800BC2D0[arg0][0] = 0;
    func_800898EC(arg0);
}

void func_800B6B84(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x75);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x74);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret != 0) {
        base = Heroes;
        h = &base[arg0];
        if ((u32)(h->hero_name - 1) < 8) {
            h->hero_sub->unk6C = (s32)(D_80052A54 + D_8009D6EC[*(volatile u16 *)&h->hero_name] * 0x38);
        } else {
            h->hero_sub->unk6C = 0;
        }
    } else {
        func_80089BE8(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6C84);

void func_800B6E84(s32 arg0) {
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6F6C);

void func_800B722C(s32 arg0) {
    HeroInfo *h;
    EXTengine *eng;
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
    func_8008502C(arg0);
    eng = Heroes[arg0].hero_eng;
    eng->unk5C = 1;
    if (eng->unk2C.x > 0x12C) {
        eng->unk2C.x -= 0x12C;
        eng->unk2C.y -= 0x12C;
        eng->unk2C.z -= 0x12C;
    } else {
        eng->unk2C.x = 3;
        eng->unk2C.y = 3;
        eng->unk2C.z = 3;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotBarrelGenerator);

void InitBarrelCH(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CoalFlameExt *ext;
    s32 i;
    s32 n;

    ext = AddExtension(nas, 0x44, 0x10000001);
    nas->FUX = &ext->ext;
    ((s32 *)ext)[0x30 / 4] = 1;
    ((s32 *)ext)[0x40 / 4] = -0x17;
    ext->unk1C = 0x3E8;
    eng = nas->Engine;
    sub = eng->sub;
    eng->unk2C.x = 0x3E8;
    eng->unk2C.y = 0x3E8;
    eng->unk2C.z = 0x3E8;
    eng->unk5C = 1;
    eng->unk10.y = 0;
    sub->unk6C = D_80053F8C;
    n = ++D_800BC450;
    if ((n & 0x1F) == 0) {
        sub->unk74 = 0x2016;
    } else if ((n & 7) == 0) {
        sub->unk74 = 0x2017;
    } else {
        sub->unk74 = 0x2001;
    }
    nas->UserInfo.status = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (D_800BC424[i] == NULL) {
                D_800BC424[i] = nas;
                break;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotBarrelCH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B7948);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B7B80);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B7DF0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCoalFlame);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCircleFire);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotEgg);

void PlotCoalFlameSFX(CoalSfxObj *obj) {
    CoalSfxInner *in = obj->unk54;

    if (D_800BC46C[obj->idx] > 0) {
        if (D_800BC47C[obj->idx] == 0) {
            PlaySfx(D_800BB9FC[Random(4)], &in->voice, &in->pos, 0x1000, 0x1E00, 0);
        } else {
            D_800BC46C[obj->idx]--;
            if (D_800BC46C[obj->idx] <= 0) {
                if (in->voice != 0) {
                    StopSfx(in->voice);
                }
                D_800BC48C[obj->idx] = 0;
            }
        }
    }
    D_800BC47C[obj->idx] = D_800BC46C[obj->idx];
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotPUGenerator_CH);

void InitPickupCH(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    PickupCHExt *ext;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    nas->UserInfo.user2 = 0;
    nas->UserInfo.user1 = 5;
    ext = AddExtension(nas, 0x44, 0x10000006);
    ext->unk14 = 0;
    eng = nas->Engine;
    nas->FUX = (EXT *)ext;
    eng->unk5D = 1;
    e = AddEngine(nas, 1);
    if (e != NULL) {
        e->unk10.x = eng->unk10.x;
        e->unk10.y = 0;
        e->unk10.z = eng->unk10.z;
        e->sub->unk51 = 2;
        e->sub->unk6C = D_80052F5C;
        e->sub->unk74 = 0x2001;
        e->sub->flags |= 0x40000041;
        e->sub->unk76 = 0x800;
        e->sub->unk50 = 1;
        e->sub->unk52 = 1;
    }
    PlaySfx(0x3FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotPickupCH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B94FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B9A50);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B9ED8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA220);

void func_800BA5A0(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            func_800B94FC(arg0, (u16)arg1);
        } else if (f & 0x20) {
            func_800B9A50(arg0, (u16)arg1);
        } else if (f & 0x40) {
            func_800B9ED8(arg0, (u16)arg1);
        } else {
            func_800BA220(arg0, (u16)arg1);
        }
    }
}

void func_800BA630(NAS *nas) {
    s32 i;

    for (i = 0; i < 1; i++) {
        if (D_800BC420[i] == nas) {
            D_800BC420[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitPUGenerator_CH(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x258;
}

void func_800BA694(s32 arg0) {
    EXTengine *p = Heroes[arg0].hero_powerup;
    subengine *s = NULL;

    if (p != NULL) {
        s = p->sub;
    }
    if (s != NULL) {
        p->flags &= 0xFFFF7FFF;
    }
    D_800BC2D0[arg0][0x14] = 0;
}

void func_800BA70C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *p = h->hero_powerup;
    EXTengine *e;

    if (p != NULL) {
        e = h->hero_eng;
        p->unk10 = e->unk10;
        p->unk5E = 1;
        p->unk10.y -= 0x300;
        p->unk1E = p->unk1E + 0x38;
    }
}

void func_800BA784(s32 arg0, s32 arg1) {
    EXTengine *p;
    subengine *sub;

    sub = NULL;
    func_800BA694(arg0);
    p = Heroes[arg0].hero_powerup;
    if (p != NULL) {
        sub = p->sub;
    }
    if (arg1 < 3) {
        if (arg1 > 0) {
            if (sub != NULL) {
                sub->unk6C = D_80052F5C;
                sub->unk74 = 0x200C;
                sub->flags |= 0x8000;
                p->flags |= 0x8000;
            }
            D_800BC2D0[arg0][0x14] = arg1;
        }
    }
}

void InitCoalFlameSFX(CoalFlameObj *arg0) {
    arg0->unk54->unkC = 0;
}

void InitEgg(NAS *nas) {
    EggExt *ext;
    EXTengine *eng;
    subengine *sub;
    s32 scene;

    ext = AddExtension(nas, 0x44, 0x10000005);
    nas->FUX = &ext->ext;
    ext->unk30 = 8;
    ext->unk10 = -0x55;
    ext->unk14 = -0xA0;
    eng = nas->Engine;
    nas->UserInfo.user2 = 0x78;
    sub = eng->sub;
    nas->UserInfo.status = eng->unk10.x;
    eng->unk10.x = 0x600;
    eng->unk10.y = 0x12C;
    eng->unk10.z = 0xA60;
    eng->unk1E = (nas->UserInfo.status - 0x400) & 0xFFF;
    eng->unk5E = 1;
    scene = *D_80058AD0;
    sub->unk74 = 0x4080;
    sub->unk68 = 0x3E7;
    sub->unk6C = (void *)scene;
    sub->flags |= 0x2000000;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitCircleFire(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CircleFireExt *ext;

    ext = AddExtension(nas, 0x44, 0x10000004);
    nas->FUX = &ext->ext;
    ext->unk30 = 1;
    eng = nas->Engine;
    nas->UserInfo.status = eng->unk10.x;
    eng->unk10.x = (D_80068BD4[((u16)nas->UserInfo.status - 0x400) & 0xFFF].unk2 * 9) >> 4;
    eng->unk10.z = (D_80068BD4[nas->UserInfo.status & 0xFFF].unk2 * 9) >> 4;
    sub = eng->sub;
    eng->unk1E = (nas->UserInfo.status - 0x400) & 0xFFF;
    eng->unk5E = 1;
    eng->unk10.y = 0;
    sub->unk6C = D_80053F8C;
    sub->unk74 = 0x4000;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitCoalFlame(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CoalFlameExt *ext;

    eng = nas->Engine;
    nas->UserInfo.user2 = 0xF;
    ext = AddExtension(nas, 0x44, 0x10000003);
    nas->FUX = &ext->ext;
    ext->unk1C = -0xD;
    sub = eng->sub;
    sub->unk6C = D_80053F8C;
    sub->unk74 = 0x200E;
    eng->unk5C = 1;
    eng->unk2C.x = 0x1000;
    eng->unk2C.y = 0x1000;
    eng->unk2C.z = 0x1000;
    eng->unk5E = 1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotCoal(CoalNAS *nas) {
    CoalEng *eng = nas->eng;

    if (nas->unk5E == 5) {
        eng->unk5E = 1;
        eng->unk1C += 0x44;
        nas->unk60 += 0xC;
        eng->y += nas->unk60;
        if (eng->y > 0) {
            eng->y = 0;
            eng->sub->unk74 = 0x2002;
            eng->unk1C = 0;
            eng->unk5E = 1;
            nas->unk5E = 0;
        }
    }
    nas->unk5C--;
    if (nas->unk5C == 0) {
        s32 xi = (eng->x + 0xA00) >> 7;
        s32 zi = (eng->z + 0xA00) >> 7;
        s32 idx = zi * 40 + xi;
        D_800BC4E8[idx] = 0;
        KillNAS((NAS *)nas);
    }
}

void InitCoal(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    nas->FUX = AddExtension(nas, 0x44, 0x10000002);
    eng = nas->Engine;
    eng->unk5D = 1;
    nas->UserInfo.flags = 5;
    nas->UserInfo.user1 = 0;
    sub = eng->sub;
    sub->unk6C = D_80053F8C;
    sub->unk74 = 0x200F;
    nas->UserInfo.status = 0x3C;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800BAC30(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (D_800BC424[i] == nas) {
            D_800BC424[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitBarrelGenerator(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x3C;
}

void func_800BAC98(s32 arg0) {
    HeroInfo *hero;
    HeroInfo *base;

    base = Heroes;
    hero = base + arg0;
    hero->hero_frozen = 5;
    SetHeroAction((HeroInfo *)arg0, 0x77);
    if (hero->hero_hit != 0) {
        hero->hero_hit--;
    } else {
        SetHeroAction((HeroInfo *)arg0, 0x77);
    }
    func_8008502C(arg0);
}

void func_800BAD1C(s32 arg0) {
    HeroInfo *hero;
    HeroInfo *base;
    s32 y;

    base = Heroes;
    hero = base + arg0;
    hero->hero_frozen = 2;
    y = hero->hero_eng->unk10.y + hero->hero_sub->unk08;
    if (D_8005A5D8 + 0x180 < y) {
        SetHeroAction((HeroInfo *)arg0, 0x77);
    }
    func_800B52C8(arg0);
    func_8008502C(arg0);
}

void func_800BADA8(s32 arg0) {
    func_800B52C8();
    func_8008502C(arg0);
}

void func_800BADD4(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x64);
}

void func_800BAE20(void) {
}

void func_800BAE28(void) {
}

void func_800BAE30(s32 arg0) {
    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x61);
}

s32 func_800BAE84(void) {
    return 0;
}

s32 func_800BAE8C(void) {
    return 1;
}

s32 func_800BAE94(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x75;
    if (!(st & 0x200)) {
        act = 0x74;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_800BAF00(s32 arg0) {
    if (!(D_80051338[arg0].unk54 & 0x4000)) {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, 0x78);
    return 1;
}

s32 func_800BAF58(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction((HeroInfo *)arg0, 0x71);
        return 1;
    }
    return 0;
}

s32 func_800BAFB4(s32 arg0)
{
  if (Heroes[arg0].hero_move->forward_speed != 0)
  {
    SetHeroAction((HeroInfo *)arg0, 0x72);
    return 1;
  }
  return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB010);

void func_800BB134(s32 arg0)
{
  HeroInfo *h;
  HeroInfo *new_var;
  if (D_800BC444 == 0)
  {
    new_var = Heroes;
    h = new_var;
    h += arg0;
    if ((h->hero_immunity == 0) && (!(h->hero_status & 0xFA00)))
    {
      SetHeroAction((HeroInfo *) arg0, (u16) D_8009E0D8);
      func_80027F7C(arg0, 0xA, 0xF0);
    }
  }
}

s32 func_800BB1C0(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) continue;
        if (h->hero_status & 0xC000) continue;
        d = x - h->hero_eng->unk10.x;
        if (d < -range || range < d) continue;
        d = z - h->hero_eng->unk10.z;
        if (-range < d && d < range) return i;
    }
    return -1;
}

s32 func_800BB260(s32 arg0) {
    u32 m = D_80051338[arg0].unk54 & 0x2000;
    return m != 0;
}

void func_800BB294(s32 *p, s32 arg1) {
    s32 d = (csqrt(p[4] * p[4] + p[6] * p[6]) >> 6) - arg1;
    s32 a;
    if (d > 0) {
        a = get_angle(p[4], p[6]);
        p[4] -= (d * D_80068BD4[(a - 0x400) & 0xFFF].unk2) >> 12;
        p[6] -= (d * D_80068BD4[a & 0xFFF].unk2) >> 12;
    }
}

void func_800BB36C(void) {
}

void func_800BB374(void) {
}

void func_800BB37C(void) {
}

void func_800BB384(void) {
    D_8005AA70[0] = (s32)func_800B3EFC;
    D_8005AA70[1] = (s32)func_800B4048;
    D_8005AA70[2] = (s32)func_800B4048;
    D_8005AA70[3] = (s32)func_800B4444;
    D_8005AA70[4] = (s32)func_800BB37C;
    D_8005AA70[6] = (s32)func_800BB374;
    D_8005AA70[5] = (s32)func_800BB36C;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[10] = (s32)func_800B44C8;
}

void func_800BB3F0(s32 x, s32 y) {
    s32 xi = (x + 0xA00) >> 7;
    s32 yi = (y + 0xA00) >> 7;
    s32 idx = yi * 40 + xi;
    D_800BC4E8[idx] = 0;
}

s32 func_800BB428(s32 x, s32 z) {
    s32 xi;
    s32 zi;
    s32 idx;

    if ((u32)(x + 0x9FF) >= 0x13FF || z >= 0xA00 || z < -0x9FF) {
        return 0;
    }
    xi = (x + 0xA00) >> 7;
    zi = (z + 0xA00) >> 7;
    idx = zi * 40 + xi;
    return D_800BC4E8[idx];
}

void func_800BB488(CoalGridObj *obj) {
    CoalPosInner *p = obj->pos;
    s32 x = p->x;
    s32 z = p->z;
    s32 idx;
    CoalGridObj *old;

    x += 0xA00;
    z += 0xA00;
    x >>= 7;
    z >>= 7;
    idx = z * 40 + x;
    old = D_800BC4E8[idx];

    D_800BC4E8[idx] = obj;
    if (old != NULL) {
        KillNAS((NAS *)old);
    }
}

s32 func_800BB4F8(s32 x, s32 z, s32 range, s32 skip) {
    s32 i;
    s32 neg;
    s32 dx;
    s32 dz;
    HeroInfo *h;
    EXTengine *eng;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (i == skip || h->hero_name == 9 || (h->hero_status & 0xC000)) {
            continue;
        }
        eng = h->hero_eng;
        dx = x - eng->unk10.x;
        if (dx < -range || range < dx) {
            continue;
        }
        dz = z - eng->unk10.z;
        if (-range < dz && dz < range) {
            return i;
        }
    }
    return -1;
}

s32 func_800BB5A0(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) continue;
        if (h->hero_status & 0xC000) continue;
        d = x - h->hero_eng->unk10.x;
        if (d < -range || range < d) continue;
        d = z - h->hero_eng->unk10.z;
        if (-range < d && d < range) return i;
    }
    return -1;
}

void func_800BB640(s32 arg0) {
    u8 *temp_v0;

    temp_v0 = (arg0 * 0x30) + D_800BC210;
    (*(s32*)((u8*)(temp_v0) + 0)) = 1;
    (*(s32*)((u8*)(temp_v0) + 4)) = 1;
    (*(s32*)((u8*)(temp_v0) + 8)) = 0;
    (*(s32*)((u8*)(temp_v0) + 24)) = 0;
    (*(s32*)((u8*)(temp_v0) + 28)) = 0;
    (*(s32*)((u8*)(temp_v0) + 32)) = 0;
    (*(s32*)((u8*)(temp_v0) + 36)) = 0;
    (*(s32*)((u8*)(temp_v0) + 40)) = 0x8000;
    (*(s32*)((u8*)(temp_v0) + 44)) = 0x8000;
}

NAS *func_800BB688(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    NAS *nas;
    EXTengine *eng;

    for (i = 0; i < 1; i++) {
        nas = D_800BC420[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (z - eng->unk10.z) < range) {
                return nas;
            }
        }
    }
    return NULL;
}

void func_800BB6FC(s32 arg0, s32 arg1, s32 arg2, EXTengine *arg3, Ovl68Out *arg4) {
    s32 ang;
    s32 n;

    ang = get_angle(arg3->unk10.x - arg1, arg3->unk10.z - arg2);
    arg4->unk0 = 2;
    n = D_800BC2D0[arg0][17];
    arg4->unk4 = (n / 2 + 0x400) / n;
    arg4->unk8 = *(u32 *)((u8 *)D_80051244 + ((ang >> 7) & 0x1C));
    if (D_8009D908 == 0) {
        Random(4);
    }
}

s32 func_800BB7E0(Ovl68Obj *a, Ovl68Pos *b) {
    Ovl68Pos *p = a->pos;
    s32 dx = b->x - p->x;
    s32 dz;
    dx = dx * dx;
    dz = b->z - p->z;
    dz = dz * dz;
    return csqrt(dx + dz) >> 6;
}
