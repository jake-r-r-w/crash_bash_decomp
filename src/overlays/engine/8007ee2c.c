#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/8007ee2c.h"
#include "bash/hero.h"
#include "bash/shell.h"

s32 func_8007EE2C(s32 pad) {
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x80) && D_8009AEEC != 0) {
        D_8009AEEC = 0;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x20) && D_8009AEEC == 0) {
        D_8009AEEC = 1;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadButtons *)D_80051338)[pad].held & 9) != 9 &&
        (((PadButtons *)D_80051338)[pad].pressed & 8)) {
        s32 *p = (s32 *)D_8005A614;
        D_8009AEF0 = 0;
        D_8009AEEC = 0;
        p[4] &= 0x04000010;
        func_8002AC18();
        return 0;
    }
    return ((PadButtons *)D_80051338)[pad].pressed & 0x4000;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8007EFC0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8007F1A8);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDB0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDBC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDCC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDE0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDF8);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADE10);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADE20);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADE38);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8007F314);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800809A0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80080FBC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80081470);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80081F64);

void func_80082590(void) {
    GfxState *gs;
    u8 *e;

    if (D_8005A624 & 0x7000) {
        gs = &D_80056998;
        *(s32 *)&gs->unk28[0] = 0;
        *(s32 *)&gs->unk28[4] = 0;
        if (D_8005A624 & 0x4000) {
            e = D_8009BCE0;
            if (*(s16 *)(e + 0xC) != -1) {
                func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)D_8009F644;
                func_8001A6D4(e, 0x10000000);
            }
        }
    } else if (D_8005A624 & 1) {
        gs = &D_80056998;
        *(s32 *)&gs->unk28[0] = 0;
        *(s32 *)&gs->unk28[4] = 0;
        if (!(D_8005A624 & 0x40)) {
            e = D_8009BCE0;
            if (*(s16 *)(e + 0xC) != -1) {
                func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)D_8009F644;
                func_8001A6D4(e, 0x10000000);
            }
        }
        func_80018B08(&D_8009F6A4, &D_8009F7EC);
        func_8001C2C0((u8 *)GetEngineSlot(0));
        func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
        func_8001C2C0((u8 *)GetEngineSlot(4));
        func_8001C298(&D_8009959C);
        func_8001C298(&D_8009959C + 1);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800826F8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80082AC4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80082C34);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80082D90);

void func_800831C4(s32 arg0) {
    GetEngineSlot(0)->unk10.y = arg0;
}

s16 func_800831F0(void) {
    EngineRec24 *p = D_8009BD0C;

    if (p != NULL) {
        do {
            if (p->kind != 0) {
                if (p->kind == 1) {
                    return p->val;
                }
            } else {
                return 0;
            }
            p++;
        } while (p != NULL);
    }
    return 0;
}

void func_80083244(void) {
    EngineRec24 *rec;
    s32 n;

    if (D_8009BD10 != NULL) {
        if (D_8009BD14 != 0) {
            n = D_8009BD14 - 1;
            rec = D_8009BD10[n];
            D_8009BD14 = n;
            D_8009BD0C = rec;
            func_80095BEC(0, &rec->kind);
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

s32 func_800832BC(void) {
    EngineRec24 *rec;

    if (D_8009BD10 != NULL) {
        rec = D_8009BD10[D_8009BD14 + 1];
        if (rec != NULL) {
            D_8009BD14 = D_8009BD14 + 1;
            D_8009BD0C = rec;
            func_80095BEC(0, &rec->kind);
            return 1;
        }
    }
    D_8009BD10 = NULL;
    D_8009BD0C = NULL;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80083334);

s32 func_80083488(void) {
    PadButtons *base = (PadButtons *)D_80051338;
    PadButtons *pad = &base[D_8009AEF0];
    if ((pad->held & 9) != 9) {
        if (pad->pressed & 0x4008) {
            return 1;
        }
    }
    return 0;
}

void func_800834E4(void) {
    s32 *p = (s32 *)D_8005A614;
    D_8009AEF0 = 0;
    D_8009AEEC = 0;
    p[4] &= 0x04000010;
    func_8002AC18();
}

void func_8008352C(void) {
    u8 *p = D_8005A614;
    s32 a = *(s32 *)(p + 0x10);
    u16 b = *(u16 *)(p + 0x30);
    *(s32 *)(p + 0x10) = a | 0x10;
    *(u16 *)(p + 0x30) = b | 0x8002;
}

void func_80083550(void) {
    D_8009D6E8.unk0 = 0;
    D_8009D6E8.unk4 = D_8009D2E4;
    D_8009D6E8.unkC = D_8009D318;
    D_8009D6E8.unk10 = D_8009D354;
    D_8009D6E8.unk14 = D_8009D2F8;
    D_8009D6E8.unk20 = func_800859D4;
    D_8009D6E8.unk24 = func_80085D2C;
    D_8009D6E8.unk2C = func_80085C84;
    D_8009D6E8.unk30 = func_8008AA40;
    D_8009D6E8.unk34 = func_8008A9F0;
    D_8009D6E8.unk38 = func_8008680C;
    D_8009D6E8.unk3C = func_8008AB48;
    D_8009D6E8.unk40 = func_8008AB48;
    D_8009D6E8.unk4C = func_80086724;
    D_8009D6E8.unk50 = func_8008671C;
    D_8009D6E8.unk54 = func_80086714;
    D_8009D6E8.unk64 = func_80084788;
    D_8009D6E8.unk60 = func_80086704;
    D_8009D6E8.unk8 = 0;
    D_8009D6E8.unk18 = 0;
    D_8009D6E8.unk1C = 0;
    D_8009D6E8.unk28 = 0;
    D_8009D6E8.unk44 = 0;
    D_8009D6E8.unk48 = 0;
    D_8009D6E8.unk5C = 0;
    D_8009D6E8.unk58 = 0;
    D_8009D6E8.unk68 = func_8008670C;
    D_8009D8F4 = D_8009D894;
    D_8009D8F8 = D_8009D8A8;
    D_8009D8FC = D_8009D8BC;
    D_8009D904 = D_8009D8DC;
    D_8009D900 = D_8009D8D0;
    D_8009D908 = 0;
    D_8009E0D8 = 0xD;
    D_8009E0DA = 0xE;
    D_8009E0DC = 0xF;
    D_8009E0DE = 0x10;
    D_8009E0E0 = 0x11;
    D_8009E0E2 = 0x13;
    D_8009E0E4 = 0x14;
    D_8009E0E6 = 0xC;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80083708);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF60);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF68);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF70);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF78);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF80);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF88);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF90);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF98);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADFA4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADFAC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80083E8C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80084788);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80084E04);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008502C);

s32 SetHeroSeq(HeroInfo *hero, s32 seq) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + (s32)hero;
    u16 name = h->hero_name;

    if ((u32)(name - 1) < 8) {
        hero_seq **t = HeroSeqTables[name];

        h->hero_aseq = seq;
        h->hero_anim->Atable = t[seq];
        h->hero_anim->Aspeed = -1;
        h->hero_anim->Aindex = 0;
        h->hero_anim->Masterspeed = *(u16 *)h->hero_anim->Atable;
        h->hero_anim->Seq_count = 0;
        return func_8008502C(hero, seq);
    }
    return 0;
}

s32 func_80085584(s32 hero, u16 name, s32 seq) {
    HeroInfo *h;

    if ((u16)(name - 1) < 8) {
        hero_seq **t = HeroSeqTables[name];

        HeroInfo *base = Heroes;

        h = base + hero;
        h->hero_aseq = seq;
        h->hero_anim->Atable = t[seq];
        h->hero_anim->Aspeed = -1;
        h->hero_anim->Aindex = 0;
        h->hero_anim->Masterspeed = *(u16 *)h->hero_anim->Atable;
        h->hero_anim->Seq_count = 0;
        return func_8008502C(hero);
    }
    return 0;
}

void func_80085650(void) {
    s32 i;
    s32 j;
    s32 off;
    s32 k;
    s32 tab;
    s32 m;
    s32 base;
    s32 *p;
    s32 *q;
    s32 t;

    i = 0;
    tab = (s32)D_8009D764;
    off = 0;
    do {
        j = 0;
        m = off;
        do {
            *(s32 *)(m + tab) = j;
            j++;
            m += 4;
        } while (j < 4);
        j = 0;
        base = off;
        k = off;
        do {
            q = (s32 *)((Random(4) * 4 + base) + tab);
            p = (s32 *)(k + tab);
            t = *p;
            *p = *q;
            *q = t;
            k += 4;
        } while (++j < 4);
        i++;
        off += 16;
    } while (i < 16);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80085718);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80085890);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800859D4);

void func_80085C84(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    if (h->hero_frozen == 0) {
        if (!(h->hero_ctrl & 1)) {
            h->hero_eng->unk1E += func_800154A0(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
        }
        h->hero_ctrl &= 0xFFFE;
        h->hero_eng->unk5E = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80085D2C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800860D4);

s32 func_800863D0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    s32 dist;
    s32 dy;

    int new_var;
    s32 dz;
    s32 far;

    dist = abs(h->hero_eng->unk10.x - h->hero_eng->OldXYZ.x);
    dz = abs(h->hero_eng->unk10.z - h->hero_eng->OldXYZ.z);
    new_var = 0x40;
    dist = (dist + new_var) + dz;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy == 0x7FFFFFFF) {
        h->hero_eng->unk10 = h->hero_eng->OldXYZ;
        return 0;
    }
    if (dy < 0) {
        dist = ~dist + 1;
        far = dy < dist;
    } else {
        far = dist < dy;
    }
    if (far) {
        return 0;
    }
    h->hero_eng->unk10.y -= dy;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800864CC);

s32 func_800866DC(s32 *arg0) {
    s32 buf[4];

    *(Vec3W *)(buf + 1) = *(Vec3W *)(arg0 + 4);
    return 0;
}

void func_80086704(void) {
}

s32 func_8008670C(void) {
    return 0;
}

s32 func_80086714(void) {
    return 0;
}

s32 func_8008671C(void) {
    return 0;
}

s32 func_80086724(void) {
    return 0;
}

void func_8008672C(s32 idx) {
    HeroInfo *h;
    EXTengine *s;
    EXTengine *e;

    if (D_8009D704 != NULL) {
        D_8009D704();
    } else {
        HeroInfo *base = Heroes;
        h = base + idx;
        s = h->hero_shadow;
        e = h->hero_eng;
        s->unk10 = e->unk10;
        if (h->hero_status & 0xA000) {
            s->sub->flags &= 0xFFFF7FFF;
        }
        s->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(s);
    }
}

void func_800867EC(void) {
    func_80085D2C(1);
}

void func_8008680C(void) {
}

void func_80086814(void) {
    if (D_8009D86C != 0) {
        func_80016478(D_8009D86C, D_8009D868);
    }
    if (D_8009D878 != 0) {
        func_80016478(D_8009D878, D_8009D874);
    }
}

void func_80086868(void) {
    D_8009D6DC[0] = 0;
    D_8009D6DC[1] = 0;
    D_8009D6DC[2] = 0;
    D_8009D6DC[3] = 20;
    D_8009D6DC[4] = 0;
}

void func_8008688C(void) {
    D_800AD12C = D_8005A5E8[D_8005A61C].unkC;
}

void SetHeroAction(HeroInfo *hero, s32 action) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + (s32)hero;
    u16 old = h->hero_mode;

    h->hero_mode = action;
    h->hero_last_mode = old;
    HeroActionTable[action].fn();
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80086918);

s32 func_80086C94(s32 arg0) {
    s32 v;
    u8 *g;
    s32 idx;

    if (arg0 < 0) {
        return -1;
    }
    g = D_8005A614;
    idx = *(s16 *)(g + 0x42);
    if (idx == 0 ||*(s32 *)(g + 0x6C) != 0 || !(*(u16 *)(g + 0x32) & 0x10)) {
        return -1;
    }
    {
        u8 t = g[*(s16 *)(g + 0x34) + 0xAB];

        if (t == 0) {
            arg0 = arg0 / 2;
        }
        if (t == 1) {
            v = arg0 * 3;
            arg0 = v / 4;
        }
        if (D_8005A614[*(s16 *)(D_8005A614 + 0x34) + 0xAB] == 3) {
            arg0 *= 2;
        }
        idx = *(s16 *)(D_8005A614 + 0x42);
        if (idx != -1) {
            D_8009D87C[idx].unk4 = arg0;
        }
        if (arg0 <= 0) {
            arg0 = 1;
        }
        return arg0;
    }
    return -1;
}

void func_80086D88(void) {
    s32 i;
    HeroInfo *hero;
    EXTengine *eng;
    s32 flag;
    s32 px;
    s32 x;
    s32 z;
    s32 xlo;
    s32 xhi;
    s32 zlo;
    s32 zhi;

    for (i = 0; i < 4; i++) {
        hero = &Heroes[i];
        if ((u32)(hero->hero_name - 1) < 8) {
            eng = hero->hero_eng;
            flag = (i << 5) | 0x80;
            px = eng->unk10.x >> 8;
            x = px + 10;
            z = 9 - (eng->unk10.z >> 8);
            if (x < 0) {
                continue;
            }
            if (x >= 20) {
                continue;
            }
            if (z < 0) {
                continue;
            }
            if (z < 20) {
                xhi = x;
                xlo = xhi;
                zhi = z;
                zlo = zhi;
                if (xhi > 0) {
                    xlo = px + 9;
                }
                if (xhi < 19) {
                    xhi = px + 11;
                }
                if (zhi > 0) {
                    zlo = zhi - 1;
                }
                if (zhi < 19) {
                    zhi++;
                }
                D_8009D90C[z * 20 + x].unk0 |= flag;
                D_8009D90C[zlo * 20 + x].unk0 |= flag;
                D_8009D90C[zhi * 20 + x].unk0 |= flag;
                D_8009D90C[z * 20 + xlo].unk0 |= flag;
                D_8009D90C[zlo * 20 + xlo].unk0 |= flag;
                D_8009D90C[zhi * 20 + xlo].unk0 |= flag;
                D_8009D90C[z * 20 + xhi].unk0 |= flag;
                D_8009D90C[zlo * 20 + xhi].unk0 |= flag;
                D_8009D90C[zhi * 20 + xhi].unk0 |= flag;
            }
        }
    }
}

void func_80086F4C(void) {
    s32 i;
    s32 j;

    i = 0;
loop:
    j = 0;
    if (i < 20) {
inner:
        if (j < 20) {
            j++;
            goto inner;
        }
        i++;
        goto loop;
    }
}

void func_80086F78(void) {
    s32 i = 399;
    D_8009D90C_t *p = D_8009D90C;

    for (p += 399; i >= 0; i--) {
        p->unk0 = 0;
        p--;
    }
}

void func_80086FA0(void) {
    s32 i;
    D_8009D90C_t *p;

    for (i = 0, p = D_8009D90C; i < 400; i++) {
        p->unk1 = 0;
        p->unk0 = 0;
        p++;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80086FCC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800871E4);

void func_80087428(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;

    h = base + arg0;

    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 1;
    D_8009D718();
    SetHeroSeq((HeroInfo *)arg0, 1);
    h->hero->Camera.look_ahead = 0x400;
    if (h->hero_ctrl & 4) {
        e = h->hero_eng;
        e->unk2C.z = 0x1000;
        e->unk2C.y = 0x1000;
        e->unk2C.x = 0x1000;
        h->hero_ctrl &= 0xFFFB;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800874EC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008795C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80087B8C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80087FC0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800880E0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800881E0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800882DC);

void func_80088450(s32 arg0) {
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
    SetHeroSeq((HeroInfo *)arg0, 8);
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

void func_80088544(s32 arg0) {
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E4);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E6);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0 && func_80086FCC(arg0) == 0) {
        s32 ret2;
        if (D_8009D72C != NULL) {
            ret2 = D_8009D72C(arg0);
        } else if (((PadButtons *)D_80051338)[arg0].unk54 & 0x4000) {
            SetHeroAction((HeroInfo *)arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (D_8009D728(arg0) != 0) {
                ret = 1;
            } else {
                HeroInfo *base = Heroes;
                HeroInfo *hero = &base[arg0];
                if (hero->hero_move->forward_speed != 0) {
                    if (hero->hero_mode == 0xF || hero->hero_mode == 0x17) {
                        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0DE);
                    } else {
                        SetHeroAction((HeroInfo *)arg0, 3);
                    }
                    ret = 1;
                } else {
                    ret = 0;
                }
            }
            if (ret == 0) {
                D_8009D73C(arg0);
                func_8008502C(arg0);
                func_800871E4(arg0);
            }
        }
    }
}

void func_80088708(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];
    NAS *nas;

    hero->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
    nas = hero->hero;
    nas->Camera.look_ahead = nas->Camera.width;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80088794);

void func_80088BAC(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80088C28);

void func_80088F78(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
    if (hero->hero_name == 3) {
        hero->hero_weapon_timer = -2;
    } else {
        hero->hero_weapon_timer = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008900C);

void func_800894E0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008955C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_800898EC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80089BE8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_80089FA4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008A200);

void func_8008A810(void) {
    u8 *p = D_8005A614;

    if (*(s32 *)(p + 4) & 0x800) {
        *(u16 *)(p + 0x30) |= 0x201;
        return;
    }
    *(u16 *)(p + 0x30) |= 1;
}

void func_8008A854(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    s32 sum;

    h->hero_frozen = 2;
    sum = *(s32 *)((u8 *)h->hero_eng + 0x14) + *(s32 *)((u8 *)h->hero_sub + 8);
    if (D_8005A5D8 + 0x180 < sum) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E2);
    }
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

void func_8008A8F0(s32 arg0) {
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008A924);

void func_8008A99C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 2);
}

void func_8008A9F0(s32 arg0) {
    Heroes[arg0].hero_idle_count = Random(0x258) + 0x12C;
}

void func_8008AA40(s32 arg0) {
    Heroes[arg0].hero_idle_count = Random(0x96) + 0x5A;
}

void func_8008AA90(void) {
    func_8008502C();
}

void func_8008AAB0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0);
}

s32 func_8008AB04(s32 arg0) {
    PadState *base = D_80051338;
    PadState *pad = &base[arg0];

    *(s32 *)((u8 *)pad + 0x50) &= ~0x4000;
    *(s32 *)((u8 *)pad + 0x54) &= ~0x4000;
    return 0;
}

s32 func_8008AB48(void) {
    return 0;
}

s32 func_8008AB50(s32 arg0) {
    u16 temp_v1;
    u32 m;

    temp_v1 = Heroes[arg0].hero_status;

    if (temp_v1 & 0x200) {
        return 1;
    }
    m = temp_v1 & 0x400;
    return m != 0;
}

s32 func_8008AB94(s32 arg0) {
    s32 ret;

    if (!(Heroes[arg0].hero_status & 0x2000)) {
        ret = 0;
    } else {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E2);
        ret = 1;
    }
    return ret;
}

s32 func_8008ABF8(s32 arg0) {
    u16 temp_v1;
    u16 act;

    temp_v1 = Heroes[arg0].hero_status;
    if (temp_v1 & 0x200) {
        act = D_8009E0E4;
    } else if (temp_v1 & 0x400) {
        act = D_8009E0E6;
    } else {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_8008AC74(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    u16 mode;
    s32 a1;

    h = base + arg0;
    if (h->hero_move->forward_speed != 0) {
        return 0;
    }
    mode = h->hero_mode;
    if (mode == 0x10 || mode == 0x18) {
        a1 = (u16)D_8009E0DC;
    } else {
        a1 = 0;
    }
    SetHeroAction((HeroInfo *)arg0, a1);
    return 1;
}

s32 func_8008ACF0(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 mode;
    s32 a1;

    if (D_8009D728(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        if (h->hero_move->forward_speed != 0) {
            mode = h->hero_mode;
            if (mode == 0xF || mode == 0x17) {
                SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0DE);
            } else {
                SetHeroAction((HeroInfo *)arg0, 3);
            }
            return 1;
        }
        return 0;
    }
    return 1;
}

void func_8008AD9C(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008ADA4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008B29C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008B418);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", InitScriptFX);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", PlotScriptFX);

void PlotSmokePoof(NAS *m) {
    s32 w = m->UserInfo.user1;
    EXTengine **u;
    EXTengine *e;
    s32 x;
    s32 z;

    if (w != 0) {
        u = (EXTengine **)w;
        if (D_8005A648 != 0x1C) {
            m->Engine->unk10.x += (u[1]->unk10.x - m->Engine->unk10.x) >> 4;
            z = (u[1]->unk10.z - (e = m->Engine)->unk10.z) >> 4;
            x = e->unk10.z;
        } else {
            m->Engine->unk10.x += (u[1]->unk10.x - m->Engine->unk10.x) / 6;
            z = (u[1]->unk10.z - (e = m->Engine)->unk10.z) / 6;
            x = e->unk10.z;
        }
        x += z;
        e->unk10.z = x;
        m->Engine->unk10.y = u[1]->unk10.y - ((u[1]->unk2C.x * 0x1A4) >> 12);
        m->Engine->unk2C.x = u[1]->unk2C.x;
        m->Engine->unk2C.y = u[1]->unk2C.y;
        m->Engine->unk2C.z = u[1]->unk2C.z;
    }
    func_800250B0(m);
}

s32 func_8008C4C8(s32 *arg0, s32 arg1) {
    s32 v[3];
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 ang;

    base = *(u8 **)((u8 *)D_80058AD0 + 0x10);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        n = *(s32 *)(p + 0x38);
        t = D_80068BD4;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = *(s32 *)(a3 + 0x64);
            v[0] = arg0[0] - *(s32 *)(a2 - 8);
            v[2] = arg0[2] - *(s32 *)a2;
            ang = -ang;
            a3 += 0x38;
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                return 0;
            }
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008C5C4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008C824);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008CA90);

void PlotWeatherNas(u8 *arg0) {
    if (*(s32 *)(arg0 + 0x60) == 0) {
        s32 v = *(s32 *)(arg0 + 0x64);
        switch (v) {
        case 0:
            func_80015D70(D_8009E200, *(u8 **)(*(u8 **)(arg0 + 0x54) + 0x6C));
            *(s32 *)(arg0 + 0x60) = 1;
            break;
        case 1:
            *(s32 *)(arg0 + 0x60) = v;
            break;
        }
    }
}

void InitWeatherNas(u8 *arg0) {
    *(s32 *)(arg0 + 0x60) = 0;
    *(s32 *)(arg0 + 0x64) = 0;
    **(s32 **)(*(u8 **)(arg0 + 0x54) + 0x6C) = 0x20008000;
    if (D_8005A648 == 0x1F) {
        func_80015D70(D_800C45DC, *(u8 **)(*(u8 **)(arg0 + 0x54) + 0x6C));
    }
}

void func_8008CE10(void) {
}

void func_8008CE18(void) {
}

FlagPoint *func_8008CE20(subengine *sub, s32 idx, s32 mask) {
    PointList *list = func_8001562C(sub);
    u8 *p;
    s32 i;
    s32 n;

    if (list != NULL) {
        n = list->nRecs;
        p = (u8 *)list->pts;
        i = 0;
        if (--n != -1) {
            do {
                if (p != NULL && i == idx && (((u32 *)p)[3] & 0xC0000000) == mask) {
                    return (FlagPoint *)p;
                }
                i++;
                p += 0x10;
            } while (--n != -1);
        }
    }
    return NULL;
}

s32 func_8008CEB0(u8 *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = (*(s32*)((u8*)(arg0) + 0)) - (*(s32*)((u8*)(arg1) + 0));
    temp_v0_2 = (*(s32*)((u8*)(arg0) + 8)) - (*(s32*)((u8*)(arg1) + 8));
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
}

void PlotParticleEffect(void) {
}

void InitParticleEffect(void) {
}

void InitSmokePoof(NAS *nas) {
    func_80024E30(nas, &D_8009E1E8);
    nas->Engine->sub->unk6C = D_800546C4;
    nas->Engine->sub->flags |= 0x40000041;
    nas->UserInfo.user1 = 0;
    if (((EngineGameView *)D_8005A614)->mode == 0x1C) {
        nas->Engine->sub->flags = 0x40008021;
    }
    if (((EngineGameView *)D_8005A614)->mode == 0x17) {
        nas->Engine->unk2C.x = 0x800;
        nas->Engine->unk2C.y = 0x800;
        nas->Engine->unk2C.z = 0x800;
    }
}

void func_8008CFBC(EXTengine *eng) {
    NAS *nas;
    EXTscriptdata *sd;
    threaddata *t;
    s32 i;

    nas = ((NAS **)eng)[-1];
    if (nas != NULL) {
        sd = FindExtension(nas, 0x10000000);
        if (sd != NULL) {
            i = 0;
            while (1) {
                if (i >= sd->threadcount) {
                    break;
                }
                t = &sd->threads[i];
                if (t->threadinfo != NULL) {
                    func_800110A8(t->threadinfo, &D_8004E0F0);
                }
                if (t->threadcontrol != NULL) {
                    func_800110A8(t->threadcontrol, &D_8004E0F0);
                }
                i++;
            }
            func_8001D174(eng);
        }
    }
}

void func_8008D088(u8 *arg0, u8 *arg1, EmitPart *arg2, s32 *arg3, u8 *arg4) {
    s32 life;
    s32 t;
    s32 u;

    func_8001F604(arg0, arg2, arg1);
    life = *(s32 *)(arg1 + 0x18);
    if (life == 0) {
        life = *(s32 *)(arg4 + 0x20);
    }
    t = *(s32 *)(arg1 + 0x58);
    if (t != 0) {
        arg3[0] = 0x1000 / t;
        *(s16 *)(arg0 + 0x76) = 0x1000;
    }
    u = *(s32 *)(arg1 + 0x5C);
    if (u != 0) {
        arg3[1] = 0x1000 / (life - u);
    }
    t = *(s32 *)(arg1 + 0x60);
    if (t != 0) {
        arg3[2] = 0x1000 / t;
        *(s32 *)(arg0 + 0x28) = 0;
        *(s32 *)(arg0 + 0x24) = 0;
        *(s32 *)(arg0 + 0x20) = 0;
    }
    u = *(s32 *)(arg1 + 0x64);
    if (u != 0) {
        if (life != u) {
            arg3[3] = 0x1000 / (life - u);
        } else {
            arg3[3] = 0;
        }
    }
    if (*(s32 *)(arg1 + 0x68) != 0) {
        *(s16 *)(arg0 + 0x14) = Random(0x1000);
    }
    *(s32 *)(arg4 + 0x2C) += 1;
}

u8 *func_8008D24C(u8 *arg0, s32 *arg1) {
    u8 *r;

    r = func_80021DD4(arg0, arg1[0x24 / 4], arg1[1]);
    if (r != NULL) {
        return r + 0x24;
    }
    return NULL;
}

u8 *func_8008D284(u8 *arg0, u8 *arg1) {
    u8 *p = func_80021DD4(arg0, *(s32 *)(arg1 + 0x24), *(s32 *)(arg1 + 4));
    if (p != NULL) {
        return p + 0x14;
    }
    return NULL;
}

void PlotFloatyScore(NAS *nas) {
    EXTengine *eng = nas->Engine;
    s32 s;
    s32 one;
    s32 ang;
    s32 y;

    one = 1;
    if (eng != NULL) {
        switch (nas->UserInfo.status) {
        case 0:
            if (nas->UserInfo.user1 <= 0 || --nas->UserInfo.user1 <= 0) {
                nas->UserInfo.status = one;
                nas->UserInfo.user1 = 0;
            }
            break;
        case 1:
            eng->unk10.y -= 0x20;
            if (nas->UserInfo.user2 - (D_8006BBD6[0] * 3 >> 7) >= eng->unk10.y) {
                nas->UserInfo.status = 2;
                nas->UserInfo.flags = 0x1E;
            }
            break;
        case 2:
            ang = ++nas->UserInfo.user1;
            y = nas->UserInfo.user2;
            ang = D_80068BD4[((ang << 7) & 0xFFF) - 0x400 & 0xFFF].unk2;
            eng->unk5E = one;
            eng->unk10.y = y - (ang * 3 >> 7);
            s = eng->unk2C.x + 0x40;
            eng->unk2C.z = s;
            eng->unk2C.y = s;
            eng->unk2C.x = s;
            if (--nas->UserInfo.flags == 0) {
                nas->UserInfo.status = 3;
                nas->UserInfo.flags = 0x3C;
                nas->UserInfo.user1 = 6;
            }
            break;
        case 3:
            eng->unk10.y -= nas->UserInfo.user1;
            eng->unk5E = one;
            eng->unk2C.x = eng->unk2C.y = eng->unk2C.z -= (eng->unk2C.x - 0x800) >> 5;
            nas->UserInfo.user1 += 3;
            if (--nas->UserInfo.flags == 0) {
                KillNAS(nas);
            }
            break;
        }
    }
}

void func_8008D488(u8 *arg0, s32 arg1) {
    s32 a;
    s32 b;

    if (arg1 != 0) {
        arg0[4] = 0x25;
        arg0[3] = (arg1 % 10) + 0x30;
        a = ((arg1 / 10) % 10) + 0x30;
        arg0[2] = a;
        b = (((arg1 / 10) / 10) % 10) + 0x30;
        arg0[1] = b;
        b &= 0xFF;
        if (b == 0x30) {
            s32 c = a & 0xFF;
            do { } while (0);
            arg0[1] = 0x20;
            if (b == c) {
                arg0[2] = 0x20;
            }
        }
    } else {
        arg0[1] = 0x20;
        arg0[2] = 0x4F;
        arg0[3] = 0x46;
        arg0[4] = 0x46;
    }
}

void InitFloatyScore(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = (*(s32*)((u8*)(arg0) + 84));
    if (temp_v1 != NULL) {
        (*(s16*)((u8*)(arg0) + 94)) = 0x1E;
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = (s32) ((*(s32*)((u8*)(temp_v1) + 20)) - 0x1A0);
        (*(s8*)((u8*)(temp_v1) + 93)) = 2;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS((NAS *)arg0);
    }
}

void func_8008D5D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    EXTengine *eng;
    NAS *nas;
    subengine *sub;
    s32 off;

    if (arg3 >= 0x64) {
        arg3 = 0x63;
    }
    nas = SpawnNASById(arg0, arg1, arg2, 0x1409, 0x11);
    if (nas != NULL) {
        eng = nas->Engine;
        nas->UserInfo.user1 = arg4;
        if (eng != NULL) {
            off = 0;
            if (arg3 >= 10) {
                func_80018F3C(eng, 1);
            }
            sub = eng->sub;
            if (sub != NULL) {
                while (sub != NULL) {
                    sub->unk6C = D_80053074;
                    sub->unk04 = off;
                    sub->unk74 = arg3 % 10 + 0x200E;
                    sub = sub->next;
                    arg3 /= 10;
                    if (sub != NULL) {
                        off -= 0xB4;
                    }
                }
            }
        } else {
            KillNAS(nas);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008D6C8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", InitHealth);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", PlotHealth);

void PlotBigPoof(NAS *nas) {
    func_800250B0(nas);
}

void InitBigPoof(NAS *nas) {
    EXTengine *eng = nas->Engine;

    if (eng != NULL) {
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0;
        func_80024E30(nas, &D_8009E250);
        eng->flags |= 0x21;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_8008DDC0(NAS *nas) {
    EXTmotion *mot;
    EXTengine *eng;
    EXTengine *src;

    mot = AddMotion(nas);
    if (mot != NULL) {
        mot->Ymax = 0xA0;
        mot->Ydec = 0xE;
        mot->Yacc = 0;
        mot->Ydir = 0xFF;
        mot->Yvel = 0;
    }
    eng = AddEngine(nas, 1);
    if (eng != NULL) {
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0x2001;
        src = nas->Engine;
        eng->unk10 = src->unk10;
        eng->flags |= 0x8041;
        eng->sub->unk76 = 0x800;
        eng->sub->flags |= 0x40000000;
    }
}

s32 func_8008DE8C(void) {
    return 0;
}

s32 func_8008DE94(s32 arg0) {
    return (Heroes[arg0].hero_status & 0xFA00) == 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8008DEC8);
