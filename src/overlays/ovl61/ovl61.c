#include "common.h"
#include "bash/main.h"
#include "bash/ovl61.h"
#include "bash/arkenoid.h"

void func_800B3528(s32 arg0) {
    VECTOR sp10;
    VECTOR *v;
    HeroInfo *h;
    EXTengine *e = (&Heroes[arg0])->hero_eng;
    FlagRecord *r;
    Ovl61HeroBox *b;
    Ovl61HeroBox *base;
    s32 t;

    if (e->unk5E != 0) {
        func_800142D4(&e->unk1C);
    }
    func_8001CD04(e->sub, e->unk3C, &e->unk10);
    do {
        r = func_80025A48(e, 1);
        if (r == NULL) goto next;
    } while (0);
    {
        base = D_800C1A88;
        b = (Ovl61HeroBox *)(arg0 << 2);
        b = (Ovl61HeroBox *)((s32)b + arg0);
        b = (Ovl61HeroBox *)((s32)b << 3);
        b = (Ovl61HeroBox *)((s32)base + (s32)b);
        t = r->unk2;
        b->a = t;
        b->b = r->height;
        b->c = r->radius;
        func_80015758((SVECTOR *)r, &sp10);
        b->x0 = sp10.vx - (&Heroes[arg0])->hero_eng->unk10.x;
        b->y0 = sp10.vy - (&Heroes[arg0])->hero_eng->unk10.y;
        b->z0 = sp10.vz - (&Heroes[arg0])->hero_eng->unk10.z;
    }
next:
    r = func_80025A48(e, 2);
    v = &sp10;
    if (r != NULL) {
        base = D_800C1A88;
        b = (Ovl61HeroBox *)(arg0 << 2);
        b = (Ovl61HeroBox *)((s32)b + arg0);
        b = (Ovl61HeroBox *)((s32)b << 3);
        t = r->radius;
        b = (Ovl61HeroBox *)((s32)base + (s32)b);
        b->d = t;
        func_80015758((SVECTOR *)r, v);
        b->x1 = sp10.vx - (&Heroes[arg0])->hero_eng->unk10.x;
        b->y1 = sp10.vy - (&Heroes[arg0])->hero_eng->unk10.y;
        b->z1 = sp10.vz - (&Heroes[arg0])->hero_eng->unk10.z;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B36CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B3B20);

void func_800B40AC(void) {
    s32 i;
    s32 best;
    s32 k;
    u8 *ao;
    u16 *c2;
    NAS *n;
    Ovl61FuxType *f;
    u16 *cnt;

    func_80086F78();
    if ((D_8005A644 & 0x801) && D_800C1C88 != 0) {
        D_800C1C88 = D_800C1C88 - 1;
    }
    i = 12;
    if (D_8009D908 != 0) {
        if (D_800C1C80 == -1) {
            best = 99;
            for (i = 0; i < 4; i++) {
                D_8009D87C[i].unk4 = 4;
                D_8009D87C[i].unk0 = 32;
                if (!(Heroes[i].hero_status & 0xFA00) && Heroes[i].hero_score < best) {
                    best = Heroes[i].hero_score;
                    D_800C1C80 = i;
                }
            }
            if (D_800C1C80 != -1 && D_800C1C80 < 4) {
                D_8009D87C[D_800C1C80].unk4 = 32;
                D_8009D87C[D_800C1C80].unk0 = 0;
            }
            i = 12;
        }
    }
    {
        u16 *p = (u16 *)&D_8009E25C;
        p += 12;
        do {
            *p = 0;
            i--;
            p--;
        } while (i >= 0);
    }
    for (n = D_8005AA24; n != NULL; n = n->Object.Child) {
        f = (Ovl61FuxType *)n->FUX;
        if (f != NULL && (u16)(f->type - 0x11) < 0xC) {
            D_8009E25C.n++;
            k = f->type - 0x10;
            cnt = (u16 *)&D_8009E25C;
            cnt[k]++;
            if ((u16)(f->type - 0x12) < 4) {
                c2 = D_8009E46C;
                c2[1]++;
            }
        }
    }
    func_800BC0D8();
    func_800BCA30();
    if (D_8005A648 != 0x10) {
        if (D_8005A648 >= 0x11) {
            if (D_8005A648 == 0x11) {
                func_800BD510();
            }
        }
    }
    ao = (u8 *)D_80054504;
    ao += 0x10;
    func_80029100((AnimOwner *)ao);
}

s32 func_800B42F4(Ovl61Obj *arg0) {
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 a;

    for (i = 0; i < 4; i++) {
        dx = arg0->x - D_800C1A7C[i].x;
        dz = arg0->z - D_800C1A7C[i].z;
        if (arg0->flags & 0x40) {
            r = D_800C1A7C[i].r + arg0->radius;
        } else {
            r = D_800C1A7C[i].r + 0x80;
        }
        if (dx * dx + dz * dz < r * r) {
            a = get_angle(-dx, -dz);
            if ((void *)arg0 != (void *)Heroes[D_800C2720].hero_eng &&
                (void *)arg0 != (void *)Heroes[D_800C271C].hero_eng) {
                arg0->x = D_800C1A7C[i].x - (r * D_80068BD4[(a - 0x400) & 0xFFF].unk2 >> 12);
            }
            if ((void *)arg0 != (void *)Heroes[D_800C2724].hero_eng &&
                (void *)arg0 != (void *)Heroes[D_800C2728].hero_eng) {
                arg0->z = D_800C1A7C[i].z - (r * D_80068BD4[a & 0xFFF].unk2 >> 12);
            }
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B453C);

s32 func_800B4880(Ovl61Obj *arg0, s32 arg1) {
    Ovl61Circle *c;
    Ovl61Circle *base;
    SLONG_XYZ v;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 x;
    s32 z;
    s32 ret;
    s32 cx;
    s32 cz;

    for (i = 0; i < 4; i++) {
        if (arg0->flags & 0x40) {
            r = D_800C1A7C[i].r + arg0->radius;
        } else {
            r = D_800C1A7C[i].r + 0x80;
        }
        base = D_800C1A7C;
        c = &base[i];
        cx = c->x;
        x = arg0->x;
        dx = x - cx;
        cz = c->z;
        z = arg0->z;
        dz = z - cz;
        if (dx * dx + dz * dz < r * r) {
            goto found;
        }
    }
    ret = -1;
    if (D_800C1F14 == 0) {
        goto fail;
    }
    dx = x;
    dz = z;
    if (arg0->flags & 0x40) {
        r = 0x800 - arg0->radius;
    } else {
        r = 0x800;
    }
    if (dx > r && (D_800C1F14 & 0x88)) {
        arg0->x = r;
        ret = 0x400;
    }
    if (dx < -r && (D_800C1F14 & 0x44)) {
        arg0->x = -r;
        ret = 0xC00;
    }
    if (dz > r && (D_800C1F14 & 0x22)) {
        arg0->z = r;
        ret = 0;
    }
    if (dz < -r && (D_800C1F14 & 0x11)) {
        arg0->z = -r;
        ret = 0x800;
    }
    if (ret != -1) {
        PlaySfx(0x592, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    return ret;
found:
    v.x = cx;
    v.y = 0;
    v.z = base[i].z;
    func_800BE1C4((SLONG_XYZ *)&arg0->x, arg1, &v, r);
    ret = get_angle(D_800C1A7C[i].x - arg0->x, D_800C1A7C[i].z - arg0->z);
    D_800C1E28[i].state = 3;
    return ret;
fail:
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4AE8);

s32 func_800B4D9C(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2) {
    s32 lim = 0x980;
    s32 x, z;
    s32 a;

    if (arg2 != 0) {
        lim = 0xC00;
    }
    x = arg0->x;
    z = arg0->z;
    if (x * x + z * z > lim * lim) {
        a = get_angle(-x, -z) + 0x200;
        a &= 0xC00;
        if (a == 0x400) {
            arg1->dir = D_800C2720;
            return 1;
        } else if (a == 0x800) {
            arg1->dir = D_800C2724;
            return 1;
        } else if (a == 0xC00) {
            arg1->dir = D_800C271C;
            return 1;
        } else {
            arg1->dir = D_800C2728;
            return 1;
        }
    }
    if (arg2 != 0) {
        s16 d = arg1->dir;
        if (d == D_800C2724 || d == D_800C2728) {
            if (x >= 0x601) {
                arg0->x = 0x600;
            }
            if (arg0->x < -0x600) {
                arg0->x = -0x600;
            }
        } else {
            if (z >= 0x601) {
                arg0->z = 0x600;
            }
            if (arg0->z < -0x600) {
                arg0->z = -0x600;
            }
        }
    }
    return 0;
}

s32 func_800B4EFC(Ovl61Obj *arg0) {
    Ovl61Circle *base = D_800C1A7C;
    Ovl61Circle *c;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 x;
    s32 lim;
    s32 z;

    for (i = 0; i < 4; i++) {
        c = &base[i];
        if (arg0->flags & 0x40) {
            r = c->r + arg0->radius;
        } else {
            r = c->r + 0x80;
        }
        dx = c->x;
        x = arg0->x;
        dx = x - dx;
        z = arg0->z;
        dz = z - c->z;
        if (dx * dx + dz * dz < r * r) {
            return 1;
        }
    }
    lim = 0x900;
    if (x > lim || x < -lim) {
        return 1;
    }
    if (z > lim) {
        return 1;
    }
    return z < -lim;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4FC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B5238);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B558C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B58E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B5C30);

void func_800B5EC8(s32 arg0) {
    HeroInfo *h;
    HeroInfo *g;
    HeroInfo *b;
    HeroInfo *c;
    EXTengine *e;
    s32 a0;
    s32 a1;
    s32 sp;
    s32 y;
    s32 r;

    r =((s32 (*)(EXTengine *))func_80025F44)(Heroes[arg0].hero_eng);
    e = Heroes[arg0].hero_eng;
    y = e->unk10.y - r;
    if (!(Heroes[arg0].hero_status & 0xFA00)) {
        s32 t;
        s32 d;
        a0 = ((D_8004E0D8 + arg0) << 6) & 0xFFF;
        t = D_80068BD4[(a0 - 0x400) & 0xFFF].unk2;
        a1 = t * 17 >> 10;
        d = (t * 15 >> 11) - 0x80;
        e->unk10.y = y + d;
    } else {
        e->unk10.y = y - 0x80;
        a1 = 0;
    }
    c = Heroes;
    g = c + arg0;
    sp = g->hero_move->forward_speed;
    a0 = g->hero_eng->unk1E;
    if (sp != 0) {
        if (arg0 == D_800C2724) {
            a0 += sp;
            a1 = sp;
        }
        if (arg0 == D_800C2728) {
            a0 -= sp;
            a1 = -sp;
        }
        if (arg0 == D_800C2720) {
            a0 += sp;
            a1 = sp;
        }
        if (arg0 == D_800C271C) {
            a0 -= sp;
            a1 = -sp;
        }
    }
    b = Heroes;
    h = b + arg0;
    a0 &= 0xFFF;
    h->hero_eng->unk1E = a0;
    h->hero_move->forward_angle = a0;
    sp = func_800154A0(h->hero_eng->unk20, a1);
    {
        EXTengine *e2 = h->hero_eng;
        e2->unk20 = (u16)e2->unk20 + (sp >> 2);
        func_800BE4D4(arg0, e2);
    }
    h->hero_eng->unk5E = 1;
    if (!(h->hero_status & 0xF800)) {
        if (h->hero_mode == 0x21) {
            h->hero_sub->unk74 = 0x2006;
            return;
        }
        if (h->hero_attack_timer != 0) {
            h->hero_sub->unk74 = 0x2006;
            return;
        }
        h->hero_sub->unk74 = 0x2004;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B60FC);

void func_800B62F4(arg0)
s32 arg0;
{
    u16 *p;
    u16 *b;
    s32 v;
    u16 t;

    v = D_800C1EF0[arg0];
    if (arg0 == D_800C2724 || arg0 == D_800C2720) {
        v = ~v + 1;
    }
    if (v < 0) {
        t = D_800C13AC[Heroes[arg0].hero_name][2];
    } else {
        t = D_800C13AC[Heroes[arg0].hero_name][3];
    }
    b = D_800C1EE8;
    p = b + arg0;
    if ((u32)(abs(*p - D_800C13AC[Heroes[arg0].hero_name][2]) - 2) >= 0x19U) {
        *p = t;
    }
    if ((u32)(abs(*p - D_800C13AC[Heroes[arg0].hero_name][3]) - 2) >= 0x19U) {
        *p = t;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B649C);

static inline s32 HeroCheckPad54(s32 arg0) {
    s32 ret;

    if (Heroes[arg0].hero_status & 0xFA00) {
        return 0;
    }
    if (((PadButtons *)D_80051338)[arg0].unk54 & 0x1000) {
        SetHeroAction((HeroInfo *)arg0, 0x1E);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 HeroCheckStatus2600(s32 arg0) {
    u16 st;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x22);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction((HeroInfo *)arg0, 0x24);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void func_800B66D0(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *base2;
    HeroInfo *h2;
    u16 mode;

    if (HeroCheckPad54(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        if (h->hero_aseq == 0x19) {
            if (h->hero_idle_count != 0) {
                h->hero_idle_count = h->hero_idle_count - 1;
            } else {
                h->hero_idle_count = Random(0x258) + 0x12C;
                SetHeroSeq((HeroInfo *)arg0, 0x1B);
            }
        }
        if (func_800B649C(arg0) != 0) {
            func_8008502C(arg0);
        }
        if (HeroCheckStatus2600(arg0) == 0) {
            func_800B60FC(arg0);
            base2 = Heroes;
            h2 = base2 + arg0;
            if (h2->hero_move->forward_speed != 0) {
                mode = h2->hero_mode;
                if (mode != 0x1F && mode != 0x21) {
                    SetHeroAction((HeroInfo *)arg0, 0x1D);
                }
            }
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl61/ovl61", D_800B32B4);

void func_800B68C8(s32 arg0) {
    s32 id;
    SLONG_XYZ *pos;
    HeroInfo *base;

    SetHeroSeq((HeroInfo *)arg0, 0x1C);
    if (Random(3) == 0 || D_8005A614[arg0 + 0x5C] == 0) {
        static const void *const tbl[] = {&&l1, &&l2, &&l3, &&l4, &&l5, &&l6, &&l7, &&l8};
        u32 c;

        c = Heroes[arg0].hero_name - 1;
        if (c < 8) {
            goto *tbl[c];
        }
        return;
    l1:
        PlaySfx(7, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        return;
    l2:
        PlaySfx(0x39, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        return;
    l3:
        PlaySfx(0x6B, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        return;
    l4:
        PlaySfx(0x9D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        return;
    l5:
        PlaySfx(0x133, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        return;
    l6:
        PlaySfx(0x165, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        return;
    l7:
        PlaySfx(0x101, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        return;
    l8:
        PlaySfx(0xCF, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void func_800B69E8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    u16 mode;
    s32 act;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x22);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction((HeroInfo *)arg0, 0x24);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        func_800B60FC(arg0);
        base = Heroes;
        h = &base[arg0];
        if (h->hero_move->forward_speed != 0) {
            mode = h->hero_mode;
            if (mode != 0x1F && mode != 0x21) {
                SetHeroAction((HeroInfo *)arg0, 0x1D);
            }
        }
        func_8008502C(arg0);
    }
}

void func_800B6AE0(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    u16 mode;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x22);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction((HeroInfo *)arg0, 0x24);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        func_800B60FC(arg0);
        base = Heroes;
        h = &base[arg0];
        if (h->hero_move->forward_speed == 0) {
            mode = h->hero_mode;
            if (mode != 0x1F && mode != 0x21) {
                SetHeroAction((HeroInfo *)arg0, 0x1C);
            }
        }
        if (func_800B649C(arg0) != 0) {
            func_8008502C(arg0);
        }
    }
}

void func_800B6BE8(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 1;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    func_80026A04((u8 *)h->hero);
    h->hero_sub->unk08 = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    func_80085718(arg0, 0);
    func_80089FA4(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B6CF0);

void func_800B6FE0(s32 arg0) {
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
    SetHeroSeq(arg0, 0x19);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B70CC);

void func_800B7450(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800B62F4();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x1D);
    h->hero_attack_delay += 0x1E;
    if (h->hero_attack_delay > 0x5A) {
        h->hero_attack_delay = 0x5A;
    }
    h->hero_attack_counter = 0;
    h->hero_attack_timer = h->hero_attack_delay;
    PlaySfx(D_800C18E0[arg0], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
}

void func_800B7518(s32 arg0) {
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h;
    subengine *s;
    subengine *sub;
    u16 cnt;
    s32 scale;
    s32 ang;

    s = 0;
    base = Heroes;
    h = &base[arg0];
    cnt = h->hero_attack_counter;
    sub = h->hero_sub;
    scale = (cnt * 0x600) + 0x1000;
    ang = (cnt << 9) + 0x800;
    sub = sub->next;
    if (sub != 0) {
        s = sub->next;
        if (s != 0) {
            s->unk74 = 0x2001;
            s->unk20.x = scale & 0xFFFF;
            s->unk20.y = scale & 0xFFFF;
            s->unk20.z = scale & 0xFFFF;
            s->unk76 = ang;
            s->unk52 = 1;
            s->flags |= 0x40008021;
        }
    }
    if (ArkHeroRad2ObjectCollision(arg0, (scale & 0xFFFF) >> 3, 0)) {
        Heroes[arg0].hero_attack_timer = 0;
    }
    base2 = Heroes;
    base2[arg0].hero_attack_counter++;
    if (base2[arg0].hero_attack_counter >= 4) {
        SetHeroAction((HeroInfo *)arg0, 0x1C);
        if (s != 0) {
            s->flags &= 0xFFFF7FFF;
        }
    }
    if (func_800B649C(arg0) != 0) {
        func_8008502C(arg0);
    }
}

void func_800B7688(s32 arg0) {
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h;
    subengine *a;
    subengine *s;
    u16 cnt;
    s32 v;
    s32 r;

    s = NULL;
    base = Heroes;
    cnt = base[arg0].hero_attack_counter;
    a = base[arg0].hero_sub;
    v = cnt * 0x600 + 0x1000;
    r = (cnt << 9) + 0x800;
    a = a->next;
    if (a != NULL) {
        s = a->next;
        if (s != NULL) {
            s->unk74 = 0x2007;
            s->unk20.x = v & 0xFFFF;
            s->unk20.y = v & 0xFFFF;
            s->unk20.z = v & 0xFFFF;
            s->unk76 = r;
            s->unk52 = 1;
            s->flags |= 0x40008021;
        }
    }
    if (ArkHeroRad2ObjectCollision(arg0, (v & 0xFFFF) >> 3, 1) != 0) {
        Heroes[arg0].hero_attack_timer = 0;
    }
    base2 = Heroes;
    h = &base2[arg0];
    cnt = h->hero_attack_counter + 1;
    h->hero_attack_counter = cnt;
    if (cnt >= 4) {
        SetHeroAction((HeroInfo *)arg0, 0x1C);
        if (s != NULL) {
            s->flags &= 0xFFFF7FFF;
        }
    }
    if (func_800B649C(arg0) != 0) {
        func_8008502C(arg0);
    }
}

void func_800B77F8(s32 arg0) {
    subengine *a;
    subengine *s;
    HeroInfo *h;
    HeroInfo *b;
    s32 t;
    s32 f;
    s16 w;

    s = NULL;
    a = Heroes[arg0].hero_sub;
    a = a->next;
    if (a != NULL) {
        s = a->next;
        if (s != NULL) {
            s->unk74 = 0x2003;
            s->flags |= 0x40008021;
            t = D_80068BD4[(((D_8004E0D8 << 7) & 0xFFF) - 0x400) & 0xFFF].unk2 * 1024 >> 13;
            s->unk52 = 1;
            s->unk20.x = t + 0x1000;
            s->unk20.y = t + 0x1000;
            s->unk20.z = t + 0x1000;
            s->unk76 = 0xA00 - t;
        }
    }
    b = Heroes;
    h = b + arg0;
    h->hero_attack_counter += 1;
    if (h->hero_weapon_timer != 0) {
        w = h->hero_weapon_timer - 1;
        h->hero_weapon_timer = w;
        if (w < 0x91) {
            if (!(HPADS[arg0].unk50 & 0x4000)) {
                f = 1;
                if (HLV->out[arg0] != 0) {
                    f = h->hero_ai->AItimer3 < 1U;
                }
                if (f != 0) {
                    FreeGrabbedObject(arg0, 1);
                    goto act;
                }
            } else {
                h->hero_ai->AItimer3 = 2;
            }
        }
    } else {
        FreeGrabbedObject(arg0, 0);
act:
        SetHeroAction((HeroInfo *)arg0, 0x1C);
        if (s != NULL) {
            s->flags &= 0xFFFF7FFF;
        }
    }
    if (func_800B649C(arg0) != 0) {
        func_8008502C(arg0);
    }
}

void func_800B79F4(s32 arg0) {
    SLONG_XYZ sp18;
    EXTengine *temp_v0;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *temp_s0;
    HeroInfo *temp_s0_2;
    _sfx_handler *temp_a0;
    subengine *temp_v1;

    base = Heroes;
    temp_s0 = &base[arg0];
    temp_s0->hero_hit = 0;
    temp_s0->hero_immunity = 0;
    temp_s0->hero_move->velocity_enable = 0;
    temp_s0->hero_move->jump_max = 0;
    temp_s0->hero_health = 0;
    temp_s0->hero_frozen = 2;
    temp_s0->hero_powerup->flags = 0;
    temp_s0->hero_fx->flags = 0;
    temp_s0->hero_pu = 0;
    temp_s0->hero_pu_timer = 0;
    temp_s0->hero_anim->sub = NULL;
    temp_a0 = temp_s0->hero_powerup->sfx;
    if (temp_a0 != NULL) {
        StopSfx(temp_a0);
    }
    temp_s0->hero_status |= 0x2000;
    temp_v1 = temp_s0->hero_sub;
    temp_v1 = temp_v1->next;
    if (temp_v1 != NULL) {
        temp_v1->flags = 0;
        temp_v1 = temp_v1->next;
        if (temp_v1 != NULL) {
            temp_v1->flags = 0;
        }
    }
    base2 = Heroes;
    temp_s0_2 = &base2[arg0];
    temp_v0 = temp_s0_2->hero_eng;
    sp18 = temp_v0->unk10;
    func_8007A524(0, (s32 *)&sp18, (s32) &D_80054504);
    PlaySfx(0x586, NULL, &temp_s0_2->hero_eng->unk10, 0x1000, 0x1E00, 0);
    func_800898EC(arg0);
}

void func_800B7B60(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x22);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret != 0) {
        base = Heroes;
        h = &base[arg0];
        if ((u32)(h->hero_name - 1) < 8) {
            h->hero_sub->unk6C = D_80052A54 + D_8009D6EC[*(volatile u16 *)&h->hero_name] * 0x38;
        } else {
            h->hero_sub->unk6C = NULL;
        }
    } else {
        func_80089BE8(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7C60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B808C);

s32 func_800B84B8(s32 arg0) {
    s32 sum;
    s32 i;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B85DC);

s32 func_800B8964(s32 arg0) {
    Ovl61Node *n;
    Ovl61Node *t;
    Ovl61Node *e;
    HeroInfo *h;
    s32 dx;
    s32 dz;
    s32 lim;
    HeroInfo *base;

    if (D_8005A648 == 0xF) {
        lim = 0x64000;
        n = D_8005AA38;
        if (n != NULL) {
            base = Heroes;
            h = &base[arg0];
            do {
                t = n->target;
                if (t != NULL) {
                    e = (Ovl61Node *)h->hero_eng;
                    dx = e->x - t->x;
                    dz = e->z - t->z;
                    if (dx * dx + dz * dz < lim) {
                        if (func_80015480(get_angle(dx, dz), h->hero_move->forward_angle) < 0x2AA) {
                            return 1;
                        }
                    }
                }
                n = n->next;
            } while (n != NULL);
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl61/ovl61", D_800B3300);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B8A68);

void func_800B9728(void) {
    s32 best = 0;
    s32 idx = -1;
    s32 i;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if ((h->hero_status & 0xFA00) == 0 && best < h->hero_health) {
            best = h->hero_score;
            idx = i;
        }
    }
    if (idx != -1) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if ((h->hero_status & 0xFA00) == 0 && i != idx) {
                h->hero_score = 0;
                h->hero_health = 0;
                h->hero_status |= 0x2000;
            }
        }
    }
}

NAS *func_800B97D8(NAS *arg0) {
    NAS *nas;
    Ovl61Eng *src;
    Ovl61Eng *eng;
    Ovl61Fux *fux;
    Ovl61Mot *dst;
    Ovl61Mot *srcm;
    s32 dir;

    nas = NULL;
    if (arg0 != NULL) {
        src = (Ovl61Eng *)arg0->Engine;
        if (src != NULL) {
            nas = SpawnNASById(src->x, src->pad14, src->z, 0x1501, 0x11);
            if (nas != NULL) {
                eng = (Ovl61Eng *)nas->Engine;
                if (eng != NULL) {
                    eng->flags |= 0x8000;
                    if (eng->sub != NULL) {
                        eng->sub->flags |= 0x8000;
                    }
                    fux = (Ovl61Fux *)nas->FUX;
                    if (fux != NULL) {
                        fux->flag = 1;
                        dst = FindExtension(nas, 0x800);
                        if (dst != NULL) {
                            srcm = FindExtension(arg0, 0x800);
                            if (srcm != NULL) {
                                dst->b = 4;
                                dst->c = 1;
                                dst->a = srcm->a;
                                dst->lim = srcm->lim;
                                dir = srcm->dir;
                                srcm->dir = (dir + 0x2AA) & 0xFFF;
                                src->x -= D_80068BD4[(srcm->dir - 0x400) & 0xFFF].unk2 * 128 >> 12;
                                src->z -= D_80068BD4[srcm->dir & 0xFFF].unk2 * 128 >> 12;
                                if (dst->lim < 0x50) {
                                    dst->lim = 0x50;
                                }
                                if (dst->a < dst->lim) {
                                    dst->a = dst->lim;
                                }
                                dst->dir = (dir - 0x2AA) & 0xFFF;
                                eng->x -= D_80068BD4[(dst->dir - 0x400) & 0xFFF].unk2 * 128 >> 12;
                                eng->z -= D_80068BD4[dst->dir & 0xFFF].unk2 * 128 >> 12;
                            }
                        }
                    }
                }
            }
        }
    }
    return nas;
}

void func_800B99E0(void) {
    NAS *nas;
    EXTengine *eng;
    EXTobject *fux;
    EXTmotion *mot;
    s32 dir;
    s32 n;

    nas = D_8005AA38;
    n = 0;
    if (nas != NULL) {
        do {
            nas = *(NAS **)((u8 *)nas + 0x20);
            n++;
        } while (nas != NULL);
    }
    if (n < 8) {
        nas = SpawnNASById(((Ovl61Spawn *)D_800C1B28)->x, ((Ovl61Spawn *)D_800C1B28)->y, ((Ovl61Spawn *)D_800C1B28)->z, 0x1501, 0x11);
        if (nas != NULL) {
            eng = nas->Engine;
            if (eng != NULL) {
                eng->flags |= 0x8000;
                if (eng->Engine != NULL) {
                    eng->Engine->flags |= 0x8000;
                }
                fux = (EXTobject *)nas->FUX;
                if (fux != NULL) {
                    fux->status = 1;
                    fux->can_do = 0x40;
                    mot = FindExtension(nas, 0x800);
                    if (mot != NULL) {
                        mot->Xacc = 4;
                        mot->Xdec = 1;
                        mot->Xvel = 0x80;
                        mot->Xmax = 0x80;
                        dir = ((Ovl61Spawn *)D_800C1B28)->dir;
                        mot->Xa = dir;
                        eng->unk10.x = D_80068BD4[(dir - 0x400) & 0xFFF].unk2 * 13 >> 7;
                        eng->unk10.z = D_80068BD4[dir & 0xFFF].unk2 * 13 >> 7;
                    }
                }
                PlaySfx(0x5C1, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", InitBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", PlotBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", CalcArkInfluence);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", FreeGrabbedObject);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", AlignGrabBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Plot_N_Gin);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BC0D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BC564);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BCA30);

void Init_ArkDeadWall(NAS *nas)
{
    EXTengine *eng;
    subengine *sub;
    s32 dir;
    s32 off;

    eng = nas->Engine;
    if (eng != 0) {
        dir = eng->unk10.x;
        eng->flags &= 0xFFFF7FFF;
        if (dir == D_800C2724) {
            eng->unk10.x = -0x600;
            eng->unk10.z = 0x880;
            eng->unk1C = 0x38E;
            eng->unk1E = 0;
            nas->UserInfo.flags = 2;
            D_800C1F14 |= 2;
        }
        if (dir == D_800C2728) {
            eng->unk10.x = -0x600;
            eng->unk10.z = -0x880;
            eng->unk1C = 0x200;
            eng->unk1E = 0;
            nas->UserInfo.flags = 1;
            D_800C1F14 |= 1;
        }
        if (dir == D_800C2720) {
            eng->unk10.x = -0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = 4;
            D_800C1F14 |= 4;
        }
        if (dir == D_800C271C) {
            eng->unk10.x = 0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = 8;
            D_800C1F14 |= 8;
        }
        eng->unk5D = 2;
        eng->unk5E = 1;
        if (D_800C1C84 == 0) {
            PlaySfx(0x591, &eng->sfx, 0, 0x1000, 0x1E00, 0);
        }
        D_800C1C84 = 1;
        func_80018F3C(eng, 5);
        sub = eng->sub;
        off = 0;
        if (sub != 0) {
            do {
                sub->unk6C = D_80054504;
                sub->unk74 = 0;
                sub->unk04 = off;
                sub->flags |= 0x21;
                sub = sub->next;
                off += 0x200;
            } while (sub != 0);
        }
    }
    nas->UserInfo.user1 = 4;
    nas->UserInfo.user2 = 0x1E;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Init_NGArkDeadWall(NAS *arg0) {
    EXTengine *eng;
    subengine *sub;
    s32 x;
    s32 off;

    eng = arg0->Engine;
    if (eng != NULL) {
        eng->flags &= 0xFFFF7FFF;
        x = eng->unk10.x;
        if (x == D_800C2724) {
            eng->unk10.x = -0x700;
            eng->unk10.z = 0x880;
            eng->unk1C = 0x38E;
            eng->unk1E = 0;
            arg0->UserInfo.flags = 2;
            D_800C1F14 |= 2;
        }
        if (x == D_800C2728) {
            eng->unk10.x = -0x700;
            eng->unk10.z = -0x880;
            eng->unk1C = 0x200;
            eng->unk1E = 0;
            arg0->UserInfo.flags = 1;
            D_800C1F14 |= 1;
        }
        if (x == D_800C2720) {
            eng->unk10.x = -0x880;
            eng->unk10.z = -0x700;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            arg0->UserInfo.flags = 4;
            D_800C1F14 |= 4;
        }
        if (x == D_800C271C) {
            eng->unk10.x = 0x880;
            eng->unk10.z = -0x700;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            arg0->UserInfo.flags = 8;
            D_800C1F14 |= 8;
        }
        eng->unk5D = 2;
        eng->unk5E = 1;
        if (D_800C1C84 == 0) {
            PlaySfx(0x591, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
        }
        D_800C1C84 = 1;
        func_80018F3C(eng, 6);
        sub = eng->sub;
        off = 0;
        if (sub != NULL) {
            do {
                sub->unk6C = D_80054504;
                sub->unk74 = 0;
                sub->unk04 = off;
                sub->flags |= 0x21;
                sub = sub->next;
                off += 0x200;
            } while (sub != NULL);
        }
    }
    arg0->UserInfo.user1 = 4;
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void func_800BD43C(u32 arg0) {
    Ovl61Wall *base;
    Ovl61Wall *w;
    s32 r = Random(0x1E);

    base = D_800C1E78;
    w = &base[arg0 & 0xFFFF];
    w->f1A = r + 0x3C;
    w->f18 = 1;
    w->f10 = 0xA0;
    w->f8 = 0x300;
    w->f4 = 0;
    w->fC = 0;
    w->f14 = -1;
    PlaySfx(0x5C9, &D_800C1BC4, NULL, 0x1000, 0x1E00, 0);
    PlaySfx(D_800C139C[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BD510);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BD770);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Plot_Ark_Pickup);

void func_800BDD30(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_pu_timer != 0) {
        h->hero_pu_timer--;
    }
    if (h->hero_pu_action_timer != 0) {
        h->hero_pu_action_timer--;
    }
    if (h->hero_pu_lock != 0) {
        h->hero_pu_lock--;
    }
}

void Plot_RepulseRing(NAS *arg0) {
    EXTengine *e;
    HeroInfo *h;
    subengine *s;
    s32 t;
    HeroInfo *base;

    base = Heroes;
    h = &base[arg0->UserInfo.user1];
    if (h->hero_status & 0xFA00) {
        KillNAS(arg0);
        return;
    }
    e = arg0->Engine;
    if (e == NULL) {
        return;
    }
    e->unk10 = h->hero_eng->unk10;
    e->unk1E = base[arg0->UserInfo.user1].hero_eng->unk1E;
    e->unk1C = base[arg0->UserInfo.user1].hero_eng->unk1C;
    e->unk20 = base[arg0->UserInfo.user1].hero_eng->unk20;
    s = e->sub;
    s->flags |= 0x40008000;
    t = D_80068BD4[(((D_8004E0D8 << 7) & 0xFFF) - 0x400) & 0xFFF].unk2 * 1024 >> 12;
    s->unk52 = 1;
    s->unk20.x = t + 0x1200;
    s->unk20.y = t + 0x1200;
    s->unk20.z = t + 0x1200;
    s->unk76 = t + 0x600;
    e->unk5E = 1;
    ArkHeroRad2ObjectRepel(arg0->UserInfo.user1, 0x800);
    D_8009E25C.n += 1;
    if (arg0->UserInfo.user2 != 0) {
        arg0->UserInfo.user2 = arg0->UserInfo.user2 - 1;
        return;
    }
    base[arg0->UserInfo.user1].hero_pu = 0;
    PlaySfx(0x5D8, NULL, NULL, 0x1000, 0x1E00, 0);
    KillNAS(arg0);
}

void Init_ArkLaserWall(NAS *nas)
{
    EXTengine *eng;
    subengine *sub;
    s32 dir;
    s32 off;

    if (D_8005A644 & 0x801) {
        D_800C1F14 &= ~nas->UserInfo.flags;
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    if (eng != 0) {
        dir = eng->unk10.x;
        if (dir == D_800C2724) {
            eng->unk10.x = -0x600;
            eng->unk10.z = 0x880;
            eng->unk1C = 0x71;
            eng->unk1E = 0;
            nas->UserInfo.flags = 0x20;
            D_800C1F14 |= 0x20;
        }
        if (dir == D_800C2728) {
            eng->unk10.x = -0x600;
            eng->unk10.z = -0x880;
            eng->unk1C = 0x200;
            eng->unk1E = 0;
            nas->UserInfo.flags = 0x10;
            D_800C1F14 |= 0x10;
        }
        if (dir == D_800C2720) {
            eng->unk10.x = -0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = 0x40;
            D_800C1F14 |= 0x40;
        }
        if (dir == D_800C271C) {
            eng->unk10.x = 0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = 0x80;
            D_800C1F14 |= 0x80;
        }
        eng->unk5D = 2;
        eng->unk5E = 1;
        PlaySfx(0x5B7, &eng->sfx, 0, 0x1000, 0x1E00, 0);
        func_80018F3C(eng, 5);
        sub = eng->sub;
        off = 0;
        if (sub != 0) {
            do {
                sub->unk6C = *(s32 *)D_80058AD0;
                sub->flags |= 0x21;
                sub->unk74 = 0x2019;
                sub->unk04 = off;
                sub = sub->next;
                off += 0x200;
            } while (sub != 0);
        }
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BE1C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BE4D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", ArkHeroRad2ObjectCollision);

void ArkHeroRad2ObjectRepel(u32 arg0, s32 arg1) {
    SLONG_XYZ sp10;
    EXTengine *eng;
    FlagRecord *rec;
    u8 *n;
    u8 *t;
    u8 *o;
    u8 *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 q;
    s32 r;
    s32 lim;
    s32 ang;

    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        rec = func_80025A48(eng, 1);
        if (rec != NULL) {
            if (eng->unk5E != 0) {
                func_800142D4(&eng->unk1C);
            }
            func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
            func_80015758((SVECTOR *)rec, (VECTOR *)&sp10);
        } else {
            sp10 = eng->unk10;
        }
        r = arg1 + 0x59;
        lim = r * r;
        n = D_8005AA38;
        if (n != NULL) {
            do {
                t = *(u8 **)(n + 0x58);
                if (t != NULL && *(u16 *)(t + 0x1A) == 1) {
                    o = *(u8 **)(n + 0x54);
                    if (o != NULL) {
                        dx = *(s32 *)(o + 0x10) - sp10.x;
                        dz = *(s32 *)(o + 0x18) - sp10.z;
                        d = dx * dx + dz * dz;
                        if (d < lim) {
                            d = csqrt(d) >> 6;
                            ang = get_angle(-dx, -dz) & 0xFFF;
                            d = r - d;                            e = FindExtension((NAS *)n, 0x800);
                            if (e != NULL) {
                                *(s16 *)(e + 0x2A) = ang;
                                *(s16 *)(e + 0x22) = d >> 4;
                            }
                        }
                    }
                }
                n = *(u8 **)(n + 0x20);
            } while (n != NULL);
        }
    }
}

void ArkHeroRad2ObjectAttract(u32 arg0, s32 arg1) {
    SLONG_XYZ sp10;
    EXTengine *eng;
    FlagRecord *rec;
    u8 *n;
    u8 *o;
    u8 *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
    s32 lim;
    s32 ang;
    s32 a;

    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        rec = func_80025A48(eng, 2);
        if (rec != NULL) {
            if (eng->unk5E != 0) {
                func_800142D4(&eng->unk1C);
            }
            func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
            func_80015758((SVECTOR *)rec, (VECTOR *)&sp10);
        } else {
            sp10 = eng->unk10;
        }
        r = arg1 + 0x59;
        lim = r * r;
        n = D_8005AA38;
        if (n != NULL) {
            do {
                o = *(u8 **)(n + 0x54);
                if (o != NULL) {
                    dx = *(s32 *)(o + 0x10) - sp10.x;
                    dz = *(s32 *)(o + 0x18) - sp10.z;
                    d = dx * dx + dz * dz;
                    if (d < lim) {
                        e = FindExtension((NAS *)n, 0x800);
                        if (e != NULL) {
                            ang = get_angle(dx, dz) & 0xFFF;
                            a = func_80015480(ang, *(s16 *)(e + 0x16));
                            if (a < 0x300) {
                                s32 s = csqrt(d);
                                d = s >> 6;
                                if (a < (s >> 8)) {
                                    d = r - d;
                                    *(s16 *)(e + 0x2A) = ang;
                                    *(s16 *)(e + 0x22) = d >> 4;
                                }
                            }
                        }
                    }
                }
                n = *(u8 **)(n + 0x20);
            } while (n != NULL);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BF640);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BFA24);

void Plot_ArkLaserWall(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    if (D_8005A644 & 0x801) {
        D_800C1F14 &= ~nas->UserInfo.flags;
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    if (eng != NULL) {
        if (D_800C1C70 == 0) {
            D_800C1F14 &= ~nas->UserInfo.flags;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x5B8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            KillNAS(nas);
            return;
        }
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

void Init_RepulseRing(NAS *nas)
{
  EXTengine *eng;
  char new_var;
  subengine *sub;
  eng = nas->Engine;
  if (eng != 0)
  {
    nas->UserInfo.user1 = D_8009D754;
    eng->unk5D = 0;
    eng->unk5C = 1;
    sub = eng->sub;
    eng->flags |= 0x21;
    sub->unk51 = (new_var = 0);
    sub->unk6C = &D_80054504;
    sub->unk74 = 0x2014;
    sub->unk50 = 1;
    sub->unk08 = 0x80;
    sub->flags = (sub->flags & 0xFFFF7FFF) | 0x21;
    PlaySfx(0x5D6, 0, 0, 0x1000, 0x1E00, new_var);
    PlaySfx(0x5D7, &eng->sfx, 0, 0x1000, 0x1E00, 0);
    nas->UserInfo.user2 = 0xF0;
    D_8009E25C.n += 1;
  }
  if (nas->Failure != 0)
  {
    KillNAS(nas);
  }
}

s32 func_800BFD14(Ovl61Vel *v) {
    return v->pad14 - ((D_8005A5D8 - (v->z * D_80068BD4[(D_800C1C68 - 0x400) & 0xFFF].unk2 >> 12)) +
                       (v->x * D_80068BD4[(D_800C1C6C - 0x400) & 0xFFF].unk2 >> 12));
}

s32 func_800BFD9C(u8 *arg0) {
    return (*(s32*)((u8*)(arg0) + 20)) - D_8005A5D8;
}

void Plot_ArkDeadWall(NAS *arg0) {
    EXTengine *eng;
    subengine *sub;

    if (D_800C1C88 == 0) {
        D_800C1F14 &= ~arg0->UserInfo.flags;
        KillNAS(arg0);
        return;
    }
    eng = arg0->Engine;
    if (eng != NULL) {
        if (arg0->UserInfo.user1 != 0) {
            arg0->UserInfo.user1 = arg0->UserInfo.user1 - 1;
        } else {
            eng->flags |= 0x8000;
        }
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub->unk74 = (D_8004E0D8 & 7) + 0x2001;
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

void Plot_ArkCameraFlash(NAS *arg0)
{
  long new_var;
  EXTengine *eng = arg0->Engine;
  if (eng != 0)
  {
    Ovl61Circle *base = D_800C1A44;
    subengine *sub = eng->sub;
    Ovl61Circle *c = &base[arg0->UserInfo.user1];
    if (c->x != 0)
    {
      sub->unk20.x = c->x;
      sub->unk20.y = c->z;
      new_var = c->r;
      sub->flags |= 0x8000;
      sub->unk52 = 1;
      sub->unk20.x = new_var;
      arg0->UserInfo.user1 += 1;
    }
    else
    {
      KillNAS(arg0);
    }
  }
}

void Init_ArkCameraFlash(NAS *arg0) {
    u8 *eng = (u8*)(*(s32*)((u8*)(arg0) + 84));
    u8 *sub;
    if (eng != NULL) {
        s32 val;
        sub = (u8*)(*(s32*)(eng + 108));
        val = *(s32*)D_80058AD0;
        (*(u16*)(sub + 116)) = 0x2017;
        (*(u8*)(sub + 80)) = 1;
        (*(u8*)(sub + 81)) = 0;
        (*(s32*)(sub + 108)) = val;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS(arg0);
    }
}

void Plot_seaweed(NAS *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        func_800250B0(arg0);
    }
}

void Init_seaweed(NAS *arg0) {
    EXTengine *eng = arg0->Engine;
    if (eng != NULL) {
        ScriptCursor *cur;
        eng->sub->unk6C = *(s32*)D_80058AD0;
        cur = func_80024E30(arg0, &D_800C1554);
        if (cur != NULL) {
            u8 *trig = (u8*)arg0->trigger;
            if (trig != NULL) {
                u8 *info = (u8*)*(s32*)(trig + 16);
                s32 n = *(s32*)(info + 4);
                ((void (*)(EXTanimator*, s32))func_80025FAC)((EXTanimator*)cur, n);
                if (n < 2) {
                    func_800250B0(arg0);
                    *(u16*)((u8*)cur + 30) += Random(8);
                }
                eng->sub->flags |= 0x02000000;
                eng->sub->unk68 = *(u16*)(info + 8);
            }
        }
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void Init_N_Gin(NAS *nas) {
    EXTengine *eng;
    Ovl61Gin *obj;

    eng = nas->Engine;
    if (eng != NULL) {
        func_80024E30(nas, &D_800C14A8);
        D_800C1B28 = eng;
        eng->sub->unk6C = D_80054574;
        eng->sub->unk74 = 0x2001;
        D_800C1B2C = obj = (Ovl61Gin *)AddObject(nas);
        if (obj != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
            nas->FUX = (EXT *)obj;
            obj->unk14 = 0xE;
            obj->unk1A = 0;
            obj->unk22 = 0x186;
            obj->unk1C = 0;
        }
        eng->unk10.y = -0x1000;
        nas->Class &= 0xFDFF;
        eng->unk5D = 1;
        eng->flags &= 0xFFFF7FFF;
        func_80025B10(eng, 0x50);
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

u8 *func_800C01B8(void) {
    s32 var_v0;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *var_v1;

    var_v1 = D_8005AA38;
    var_v0 = 0;
    if (var_v1 != NULL) {
        do {
            var_v1 = (*(s32*)((u8*)(var_v1) + 32));
            var_v0 += 1;
        } while (var_v1 != NULL);
    }
    if (var_v0 >= 9) {
        return NULL;
    }
    temp_v0 = SpawnNASById(0x1000, 0x1000, 0x1000, 0x1501, 0x11);
    if ((temp_v0 != NULL) && ((*(s32*)((u8*)(temp_v0) + 84)) != 0)) {
        temp_v0_2 = (*(s32*)((u8*)(temp_v0) + 88));
        if (temp_v0_2 != NULL) {
            (*(s16*)((u8*)(temp_v0_2) + 26)) = 0;
        }
    }
    return temp_v0;
}

s32 func_800C0250(s32 arg0, s32 arg1) {
    s32 row;

    row = arg0 == D_800C2728;
    if (arg0 == D_800C2720) {
        row = 2;
    }
    if (arg0 == D_800C271C) {
        row = 3;
    }
    if (arg1 == *D_800C19C0[row][0]) {
        return 0;
    }
    if (arg1 == *D_800C19C0[row][1]) {
        return 1;
    }
    if (arg1 == *D_800C19C0[row][2]) {
        return 2;
    }
    return 3;
}

void func_800C02F8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800B62F4();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x1D);
    h->hero_weapon_timer = 0x96;
    h->hero_attack_counter = 0;
    PlaySfx(D_800C18E8[arg0], &h->hero_eng->sfx, NULL, 0x1000, 0x1E00, 0);
}

void func_800C039C(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800B62F4();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x1D);
    D_800C1F0C = Random(0x3C) + 0x3C;
    h->hero_attack_counter = 0;
    PlaySfx(D_800C18E0[arg0], NULL, NULL, 0x1000, 0x1E00, 0);
}

void func_800C0448(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_frozen = 5;
    if (h->hero_hit != 0) {
        h->hero_hit--;
    } else {
        SetHeroAction((HeroInfo *)arg0, 0x25);
    }
    func_8008502C(arg0);
}

void func_800C04B8(s32 arg0) {
    Heroes[arg0].hero_frozen = 5;
    func_8008502C();
}

void func_800C04FC(s32 arg0) {
    func_800B62F4();
    SetHeroSeq(arg0, 0x1A);
}

void func_800C052C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    func_800B62F4(arg0);
    SetHeroSeq((HeroInfo *)arg0, 0x19);
    h->hero_idle_count = Random(0x96) + 0xB4;
}

s32 func_800C05B8(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x22;
    if (!(st & 0x200)) {
        act = 0x23;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_800C0624(s32 arg0) {
    HeroInfo *base = Heroes;
    u16 st = base[arg0].hero_status;
    s32 act;

    if (st & 0x200) {
        act = 0x22;
    } else if (st & 0x2000) {
        act = 0x24;
    } else if (st & 0x400) {
        act = 0x23;
    } else {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

void func_800C069C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *shadow = h->hero_shadow;

    shadow->unk10 = h->hero_eng->unk10;
    shadow->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(shadow);
    shadow->unk1E = h->hero_eng->unk1E;
    shadow->unk5E = 1;
    if (h->hero_status & 0xF800) {
        shadow->sub->flags &= ~0x8000;
    }
}

extern char D_8005453C[];

void func_800C0760(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    s->sub->unk6C = D_8005453C;
    s->sub->unk74 = 0x2002;
    s->unk10 = h->hero_eng->unk10;
    s->unk5D = 0;
    s->flags |= 0x8041;
    s->sub->unk76 = 0x200;
    s->sub->flags |= 0x42000000;
    s->sub->unk68 = -0x10;
}

s32 func_800C0804(s32 arg0, s16 arg1, s32 arg2, s16 arg3) {
    s32 a = arg1 * D_80068BD4[((arg0 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 b = arg1 * D_80068BD4[arg0 & 0xFFF].unk2;
    s32 c = arg3 * D_80068BD4[((arg2 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 d = arg3 * D_80068BD4[arg2 & 0xFFF].unk2;
    s32 x = a >> 12;
    s32 z = b >> 12;
    x += c >> 12;
    z += d >> 12;
    return get_angle(x, z) & 0xFFFF;
}

s32 func_800C08C8(Ovl61Obj *arg0) {
    Ovl61Circle *base = D_800C1A7C;
    Ovl61Circle *c;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;

    for (i = 0; i < 4; i++) {
        c = &base[i];
        if (arg0->flags & 0x40) {
            r = c->r + arg0->radius;
        } else {
            r = c->r + 0x80;
        }
        dx = arg0->x - c->x;
        dz = arg0->z - c->z;
        if (dx * dx + dz * dz < r * r) {
            return 1;
        }
    }
    return 0;
}

s32 func_800C096C(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2) {
    s32 lim;
    s32 x;
    s32 z;
    s32 d;

    lim = 0xA00;
    if (arg2 != 0) {
        lim = 0x1000;
    }
    x = arg0->x;
    if (x > lim) {
        arg1->dir = D_800C271C;
        return 1;
    }
    if (x < -lim) {
        arg1->dir = D_800C2720;
        return 1;
    }
    z = arg0->z;
    if (z > lim) {
        arg1->dir = D_800C2724;
        return 1;
    }
    if (z < -lim) {
        arg1->dir = D_800C2728;
        return 1;
    }
    if (arg2 != 0) {
        d = arg1->dir;
        if (d == D_800C2724 || d == D_800C2728) {
            if (x > 0x680) {
                arg0->x = 0x680;
            }
            if (arg0->x < -0x680) {
                arg0->x = -0x680;
            }
        } else {
            if (z > 0x680) {
                arg0->z = 0x680;
            }
            if (arg0->z < -0x680) {
                arg0->z = -0x680;
            }
        }
    }
    return 0;
}

s32 func_800C0A74(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2) {
    s32 lim;
    s32 x;
    s32 z;
    s32 d;

    lim = 0xA00;
    if (arg2 != 0) {
        lim = 0xB80;
    }
    x = arg0->x;
    if (x > lim) {
        arg1->dir = D_800C271C;
        return 1;
    }
    if (x < -lim) {
        arg1->dir = D_800C2720;
        return 1;
    }
    z = arg0->z;
    if (z > lim) {
        arg1->dir = D_800C2724;
        return 1;
    }
    if (z < -lim) {
        arg1->dir = D_800C2728;
        return 1;
    }
    if (arg2 != 0) {
        d = arg1->dir;
        if (d == D_800C2724 || d == D_800C2728) {
            if (x > 0x600) {
                arg0->x = 0x600;
            }
            if (arg0->x < -0x600) {
                arg0->x = -0x600;
            }
        } else {
            if (z > 0x600) {
                arg0->z = 0x600;
            }
            if (arg0->z < -0x600) {
                arg0->z = -0x600;
            }
        }
    }
    return 0;
}

void func_800C0B7C(void) {
}

void func_800C0B84(void) {
}

void func_800C0B8C(void) {
}

void func_800C0B94(void) {
    D_8005AA70[0] = (s32)func_800B36CC;
    D_8005AA70[1] = (s32)func_800B3B20;
    D_8005AA70[2] = (s32)func_800B3B20;
    D_8005AA70[3] = (s32)func_800B40AC;
    D_8005AA70[6] = (s32)func_800C0B7C;
    D_8005AA70[5] = (s32)func_800C0B84;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[9] = (s32)func_800BFD9C;
    D_8005AA70[10] = (s32)func_800B9728;
}
