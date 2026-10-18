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

void func_800B36CC(void) {
    u8 *g;
    void **p;
    void **p2;
    void **p3;
    void **p4;
    void *t;

    D_8009D6E8.unk4 = D_800C0E70;
    D_8009D6E8.unk0 = (s32)func_800B5238;
    D_8009D6E8.unk18 = func_800C0760;
    D_8009D6E8.unk1C = func_800C069C;
    D_8009D6E8.unk38 = func_800B8A68;
    D_8009D6E8.unk14 = D_800C1054;
    if (D_8005A680[0] != 0) {
        D_800C271C = 1;
        D_800C2720 = 3;
        D_800C2724 = 2;
    } else {
        D_800C271C = 2;
        D_800C2720 = 3;
        D_800C2724 = 1;
    }
    D_800C2728 = 0;
    g = D_8005A614;
    switch (*(s16 *)(g + 0x34)) {
    case 15:
        D_8009D6E8.unk68 = func_800B42F4;
        D_8009D6E8.unk20 = func_800B558C;
        D_8009D6E8.unk24 = func_800B5C30;
        D_800C1C8C = func_800B4880;
        D_800C1C90 = func_800C0A74;
        D_800C1C94 = func_800B4EFC;
        D_8005AA94 = (void *)func_800BFD9C;
        D_8009D8F4 = D_800C0F44;
        D_8009D8F8 = D_800C0F58;
        D_8009D904 = D_800C0F2C;
        p = &D_8009D8FC;
        if (*(s32 *)(g + 0x6C) != 0) {
            t = D_800C0F0C;
        } else {
            t = D_800C0EF8;
        }
        *p = t;
        D_8009D900 = D_800C0F20;
        return;
    case 16:
        D_8009D6E8.unk68 = func_800B42F4;
        D_8009D6E8.unk20 = func_800B558C;
        D_8009D6E8.unk24 = func_800B5C30;
        D_800C1C8C = func_800B4880;
        D_800C1C90 = func_800C096C;
        D_800C1C94 = func_800B4EFC;
        D_8005AA94 = (void *)func_800BFD9C;
        D_8009D8F4 = D_800C0FB8;
        D_8009D8F8 = D_800C0FCC;
        D_8009D904 = D_800C0FA0;
        p2 = &D_8009D8FC;
        if (*(s32 *)(g + 0x6C) != 0) {
            t = D_800C0F80;
        } else {
            t = D_800C0F6C;
        }
        *p2 = t;
        D_8009D900 = D_800C0F94;
        return;
    case 17:
        D_8009D6E8.unk68 = func_800B453C;
        D_8009D6E8.unk20 = func_800B58E8;
        D_8009D6E8.unk24 = func_800B5EC8;
        D_800C1C8C = func_800B4AE8;
        D_800C1C90 = func_800B4D9C;
        D_800C1C94 = func_800C08C8;
        D_8005AA94 = (void *)func_800BFD14;
        D_8009D8F4 = D_800C102C;
        D_8009D8F8 = D_800C1040;
        D_8009D904 = D_800C1014;
        p3 = &D_8009D8FC;
        if (*(s32 *)(g + 0x6C) != 0) {
            t = D_800C0FF4;
        } else {
            t = D_800C0FE0;
        }
        *p3 = t;
        D_8009D900 = D_800C1008;
        return;
    case 0:
    default:
        D_8009D6E8.unk68 = func_800B42F4;
        D_8009D6E8.unk20 = func_800B558C;
        D_8009D6E8.unk24 = func_800B5C30;
        D_800C1C8C = func_800B4880;
        D_800C1C90 = func_800C0A74;
        D_800C1C94 = func_800B4EFC;
        D_8005AA94 = (void *)func_800BFD9C;
        D_8009D8F4 = D_800C0ED0;
        D_8009D8F8 = D_800C0EE4;
        D_8009D904 = D_800C0EB8;
        p4 = &D_8009D8FC;
        if (D_8005A680[0] != 0) {
            t = D_800C0E98;
        } else {
            t = D_800C0E84;
        }
        *p4 = t;
        D_8009D900 = D_800C0EAC;
        return;
    }
}

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

s32 func_800B453C(Ovl61Obj *arg0) {
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 a;

    if (D_8005A644 & 0x801) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if ((Heroes[i].hero_status & 0xFA00) && (void *)Heroes[i].hero_eng == (void *)arg0) {
            return 0;
        }
    }
    a = arg0->dir;
    arg0->x = D_80068BD4[(a - 0x400) & 0xFFF].unk2 * 9 >> 4;
    arg0->z = D_80068BD4[a & 0xFFF].unk2 * 9 >> 4;
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
            a = get_angle(arg0->x, arg0->z);
            arg0->dir = a;
            arg0->x = D_80068BD4[(a - 0x400) & 0xFFF].unk2 * 9 >> 4;
            arg0->z = D_80068BD4[a & 0xFFF].unk2 * 9 >> 4;
            return 1;
        }
    }
    return 0;
}

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

s32 func_800B4AE8(Ovl61Obj *arg0, s32 arg1) {
    SLONG_XYZ v;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 cx;
    s32 ret;
    s32 a;
    s32 t;

    a = ret = -1;
    for (i = 0; i < 4; i++) {
        if (arg0->flags & 0x40) {
            r = D_800C1A7C[i].r + arg0->radius;
        } else {
            r = D_800C1A7C[i].r + 0x80;
        }
        cx = D_800C1A7C[i].x;
        dx = arg0->x - cx;
        dz = arg0->z - D_800C1A7C[i].z;
        if (dx * dx + dz * dz < r * r) {
            v.x = cx;
            v.y = 0;
            v.z = D_800C1A7C[i].z;
            func_800BE1C4((SLONG_XYZ *)&arg0->x, arg1, &v, r);
            a = get_angle(D_800C1A7C[i].x - arg0->x, D_800C1A7C[i].z - arg0->z);
            D_800C1E28[i].state = 3;
            ret = a;
        }
        if (D_800C1F14 != 0) {
            dx = arg0->x;
            dz = arg0->z;
            if (arg0->flags & 0x40) {
                r = 0x800 - arg0->radius;
            } else {
                r = 0x800;
            }
            if (dx > r && (D_800C1F14 & 0x88)) {
                arg0->x = r;
                a = 0x400;
            }
            if (dx < -r && (D_800C1F14 & 0x44)) {
                arg0->x = -r;
                a = 0xC00;
            }
            if (dz > r && (D_800C1F14 & 0x22)) {
                arg0->z = r;
                a = 0;
            }
            if (dz < -r && (D_800C1F14 & 0x11)) {
                arg0->z = -r;
                a = 0x800;
            }
            if (a != -1) {
                if (D_800C1EFE != 0) {
                    t = D_800C1EFE - 1;
                } else {
                    PlaySfx(0x592, NULL, NULL, 0x1000, 0x1E00, 0);
                    t = 3;
                }
                D_800C1EFE = t;
                do {
                    ret = a;
                } while (0);
            }
        }
    }
    return ret;
}

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

s32 func_800B4FC8(s32 *p0, s32 *p1, s32 *q) {
    s32 denom, num, u, t;
    s32 dqz;
    s32 dpx;
    s32 dpz, dqx, ez, ex;
    s32 a, b, c;

    a = p0[0];
    if (a < 0 && (c = p1[0]) < 0 && a < (b = q[0]) && c < b) {
        return 0;
    }
    if ((a = p0[0]) > 0 && (c = p1[0]) > 0 && (b = q[0]) < a && b < c) {
        return 0;
    }
    a = p0[2];
    if (a < 0 && (c = p1[2]) < 0 && a < (b = q[2]) && c < b) {
        return 0;
    }
    if ((a = p0[2]) > 0 && (c = p1[2]) > 0 && (b = q[2]) < a && b < c) {
        return 0;
    }
    dqz = -q[2] >> 4;
    dpx = p0[0] >> 4;
    dqx = -q[0] >> 4;
    dpz = p0[2] >> 4;
    ez = (p1[2] - p0[2]) >> 4;
    ex = (p1[0] - p0[0]) >> 4;
    num = dqz * dpx - dqx * dpz;
    denom = ez * dqx - ex * dqz;
    if (denom > 0) {
        if (denom < num) {
            return 0;
        }
        if (num < 0) {
            return 0;
        }
    } else {
        if (num < denom) {
            return 0;
        }
        if (num > 0) {
            return 0;
        }
    }
    u = ex * dpz - ez * dpx;
    if (denom > 0) {
        if (denom < u) {
            return 0;
        }
        if (u < 0) {
            return 0;
        }
    } else {
        if (u < denom) {
            return 0;
        }
        if (u > 0) {
            return 0;
        }
    }
    if (denom == 0) {
        return 0;
    }
    t = (num << 8) / denom;
    q[0] = p0[0] + (((p1[0] - p0[0]) * t) >> 8);
    q[2] = p0[2] + (((p1[2] - p0[2]) * t) >> 8);
    return 1;
}

void func_800B5238(s32 arg0) {
    HeroInfo *h;
    HeroInfo *c;
    HeroInfo *g;
    HeroInfo *b;
    EXTengine *e;
    subengine *s;
    subengine *n;
    s32 sp;
    s32 y;
    s32 d;
    s32 a;

    c = Heroes;
    h = c + arg0;
    e = h->hero_eng;
    if (arg0 == D_800C2728) {
        h->hero_eng->unk10.x = D_800C18A8[0][0];
        h->hero_eng->unk10.z = D_800C18A8[0][2];
    }
    if (arg0 == D_800C2724) {
        h->hero_eng->unk10.x = D_800C18A8[1][0];
        h->hero_eng->unk10.z = D_800C18A8[1][2];
    }
    if (arg0 == D_800C271C) {
        h->hero_eng->unk10.x = D_800C18A8[2][0];
        h->hero_eng->unk10.z = D_800C18A8[2][2];
    }
    if (arg0 == D_800C2720) {
        h->hero_eng->unk10.x = D_800C18A8[3][0];
        h->hero_eng->unk10.z = D_800C18A8[3][2];
    }
    e->unk5D = 0;
    s = h->hero_sub;
    func_8001D390(e, 2);
    n = s->next;
    if (n != NULL) {
        n->unk6C = s->unk6C;
        n->flags |= 0x8000;
        h->hero_anim->sub = n;
        n = n->next;
        if (n != NULL) {
            n->unk6C = D_8005453C;
            n->unk74 = 0x2001;
            n->unk50 = 1;
            n->unk51 = 0;
            n->flags &= ~0x8000;
        }
    }
    s->unk6C = D_8005453C;
    if (arg0 == D_800C2728) {
        switch (Heroes[arg0].hero_name) {
        case 5:
        case 6:
        case 8:
            s->unk68 = -0x10;
            s->flags |= 0x02000000;
            break;
        }
    }
    D_800C1EE8[arg0] = 0xF;
    s->unk74 = 0x2004;
    func_80025B10(e, 0x50);
    if (e->unk10.x > 0x880) {
        e->unk10.x = 0x880;
    }
    if (e->unk10.x < -0x880) {
        e->unk10.x = -0x880;
    }
    if (e->unk10.z > 0x880) {
        e->unk10.z = 0x880;
    }
    if (e->unk10.z < -0x880) {
        e->unk10.z = -0x880;
    }
    SetHeroAction((HeroInfo *)arg0, 0x1C);
    b = Heroes;
    g = b + arg0;
    a = D_8004E0D8;
    g->hero_weapon_count = 1;
    sp = ((((((a + arg0) << 6) & 0xFFF) - 0x400) & 0xFFF) + D_80068BD4)->unk2;
    y = D_8005A5D8;
    d = (sp * 15 >> 11) - 0x80;
    g->hero_eng->unk10.y = y + d;
}

void func_800B558C(s32 arg0) {
    EXTheromove *m;
    HeroInfo *h;
    HeroInfo *c;
    PadButtons *p;
    PadButtons *pads;
    s32 mask;
    s32 d;
    s32 max;
    s32 acc;
    s32 fast;
    s32 b;
    s32 v;

    max = 0x68;
    acc = 0x22;
    c = Heroes;
    h = c + arg0;
    m = h->hero_move;
    fast = 0;
    if (h->hero_frozen != 0) {
        ((PadButtons *)D_80051338)[arg0].unk50 = 0;
        ((PadButtons *)D_80051338)[arg0].unk54 = 0;
    }
    if ((D_80051338[arg0].unk0 & 1) || h->hero_ai->AImode != 0) {
        if (arg0 == D_800C2724) {
            acc = 0x2A;
        }
        if (((PadButtons *)D_80051338)[arg0].unk50 & 0xF00) {
            max = 0xA0;
            acc = 0x38;
        }
        if (arg0 == D_800C2724 || arg0 == D_800C2728) {
            mask = 0xA0;
        } else {
            mask = 0x50;
        }
        pads = (PadButtons *)D_80051338;
        p = pads + arg0;
        if (p->unk50 & mask) {
            if (m->velocity_enable != 0) {
                d = 0;
                if (((s32 (*)(EXTengine *))D_8009D6E8.unk68)(Heroes[arg0].hero_eng) != 0) {
                    fast = 1;
                }
                if (arg0 == D_800C2724 || arg0 == D_800C2728) {
                    b = p->unk50;
                    if (b & 0x80) {
                        d = -acc;
                    }
                    v = b & 0x20;
                } else {
                    b = p->unk50;
                    if (b & 0x40) {
                        d = -acc;
                    }
                    v = b & 0x10;
                }
                if (v != 0) {
                    d += acc;
                }
                if (fast != 0) {
                    if (d < 0) {
                        d -= max >> 1;
                    }
                    if (d > 0) {
                        d += max >> 1;
                    }
                }
                m->forward_speed += d;
                do {
                    if (m->forward_speed > max) {
                        m->forward_speed = max;
                    }
                    if (m->forward_speed < -max) {
                        m->forward_speed = -max;
                    }
                } while (0);
                if (m->forward_speed == 0) {
                    if (d != 0) {
                        v = 1;
                        if (d < 0) {
                            v = -1;
                        }
                        m->forward_speed = v;
                    }
                }
            }
            D_800C1EF0[arg0] = m->forward_speed;
        } else {
            D_800C1EF0[arg0] = 0;
            if (((s32 (*)(EXTengine *))D_8009D6E8.unk68)(Heroes[arg0].hero_eng) != 0) {
                m->forward_speed = 0;
            }
            if (m->forward_speed > 0) {
                v = m->forward_speed - 0x12;
                m->forward_speed = v;
                if (v >= 0) {
                    goto end;
                }
                m->forward_speed = 0;
            }
            if (m->forward_speed < 0) {
                v = m->forward_speed + 0x12;
                m->forward_speed = v;
                if (v > 0) {
                    m->forward_speed = 0;
                }
            }
        }
    end:
        m->forward_angle = m->facing_angle;
    }
}

void func_800B58E8(s32 arg0) {
    EXTheromove *mv;
    HeroInfo *h;
    PadButtons *p;
    s32 mask;
    s32 d;
    s32 hit;
    s32 step;
    s32 max;
    s32 t;

    hit = 0;
    max = 0x28;
    {
        HeroInfo *hb = Heroes;
        h = &hb[arg0];
    }
    mv = h->hero_move;
    step = 0xD;
    if (h->hero_frozen != 0) {
        HPADS[arg0].unk50 = 0;
        HPADS[arg0].unk54 = 0;
    }
    if ((*(s32 *)HPADS[arg0].unk0 & 1) || h->hero_ai->AImode != 0) {
        if (arg0 == D_800C2724) {
            step = 0x15;
        }
        if (HPADS[arg0].unk50 & 0xF00) {
            max = 0x48;
            step = 0x12;
        }
        mask = (arg0 == D_800C2724 || arg0 == D_800C2728) ? 0xA0 : 0x50;
        {
            PadButtons *b = HPADS;
            p = &b[arg0];
        }
        if (p->unk50 & mask) {
            if (mv->velocity_enable != 0) {
                d = 0;
                if (D_8009D750(Heroes[arg0].hero_eng) != 0) {
                    hit = 1;
                }
                if (arg0 == D_800C2724 || arg0 == D_800C2728) {
                    if (p->unk50 & 0x80) {
                        d = -step;
                    }
                    if (p->unk50 & 0x20) {
                        d += step;
                    }
                } else {
                    if (p->unk50 & 0x40) {
                        d = -step;
                    }
                    if (p->unk50 & 0x10) {
                        d += step;
                    }
                }
                if (hit != 0) {
                    do {
                        if (d < 0) {
                            d -= max >> 1;
                        }
                        if (d > 0) {
                            d += max >> 1;
                        }
                    } while (0);
                }
                mv->forward_speed += d;
                if (mv->forward_speed > max) {
                    mv->forward_speed = max;
                }
                if (mv->forward_speed < -max) {
                    mv->forward_speed = -max;
                }
                if (mv->forward_speed == 0) {
                    mv->forward_speed = (d < 0) ? -1 : 1;
                }
            }
            D_800C1EF0[arg0] = mv->forward_speed;
        } else {
            D_800C1EF0[arg0] = 0;
            if (D_8009D750(Heroes[arg0].hero_eng) != 0) {
                mv->forward_speed = 0;
            }
            if (mv->forward_speed > 0) {
                mv->forward_speed -= 6;
                if (mv->forward_speed < 0) {
                    mv->forward_speed = 0;
                }
            }
            if (mv->forward_speed < 0) {
                mv->forward_speed += 6;
                if (mv->forward_speed > 0) {
                    mv->forward_speed = 0;
                }
            }
        }
    }
}

void func_800B5C30(s32 arg0) {
    HeroInfo *h;
    HeroInfo *g;
    HeroInfo *c;
    HeroInfo *f;
    HeroInfo *b;
    EngineHooks *hooks;
    s32 a1;
    s32 sp;

    c = Heroes;
    g = c + arg0;
    a1 = 0;
    if (!(g->hero_status & 0xFA00)) {
        s32 d;
        s32 y;
        sp = ((((((D_8004E0D8 + arg0) << 6) & 0xFFF) - 0x400) & 0xFFF) + D_80068BD4)->unk2;
        a1 = sp * 17 >> 10;
        y = D_8005A5D8;
        d = (sp * 15 >> 11) - 0x80;
        g->hero_eng->unk10.y = y + d;
    } else {
        g->hero_eng->unk10.y = -0x80;
    }
    hooks = &D_8009D6E8;
    ((void (*)(s32))hooks->unk2C)(arg0);
    f = Heroes;
    g = f + arg0;
    sp = g->hero_move->forward_speed;
    if (sp != 0) {
        if (arg0 == D_800C2724) {
            a1 = sp;
            g->hero_eng->unk10.x += sp;
        }
        if (arg0 == D_800C2728) {
            a1 = -sp;
            g->hero_eng->unk10.x += sp;
        }
        if (arg0 == D_800C2720) {
            a1 = sp;
            g->hero_eng->unk10.z += sp;
        }
        if (arg0 == D_800C271C) {
            a1 = -sp;
            g->hero_eng->unk10.z += sp;
        }
        ((void (*)(EXTengine *))hooks->unk68)(g->hero_eng);
    }
    b = Heroes;
    h = b + arg0;
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

s32 func_800B60FC(s32 arg0) {
    PadButtons *p;
    s32 act;
    s32 t;
    s32 f;

    switch (MODEV->unk34) {
    break;
    case 14:
        if (MODEV->unk32 & 0x80) {
            if (arg0 == 0 || (MODEV->unk6C != 0 && arg0 == 1)) {
                Heroes[arg0].hero_attack_timer = 10;
            }
            goto l17;
        }
    case 17:
l17:
        if (MODEV->unk32 & 0x80) {
            if (arg0 != 0 && (MODEV->unk6C == 0 || arg0 != 1)) {
                if (HPADS[arg0].unk54 & 0x2000) {
                    act = 0x20;
                    goto call;
                }
            }
        }
    case 16:
        {
            PadButtons *b = HPADS;
            p = &b[arg0];
        }
        f = p->unk54 & 0x8000;
        goto chk;
    case 15:
        {
            PadButtons *b = HPADS;
            p = &b[arg0];
        }
        t = p->unk54;
        if (t & 0x4000) {
            act = 0x21;
            goto call;
        }
        f = t & 0x8000;
chk:
        if (f) {
            if (Heroes[arg0].hero_attack_timer == 0) {
                act = 0x1F;
call:
                SetHeroAction((HeroInfo *)arg0, act);
                return 1;
            }
            p->unk4C &= ~0x8000;
        }
        return 0;
    default:
        return 0;
    }
}

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

s32 func_800B649C(arg0)
s32 arg0;
{
  s32 v;
  s32 t;
  u32 u;
  u16 *p;
  u16 *d;
  subengine *s;
  v = D_800C1EF0[arg0];
  if (v == 0)
  {
    d = &D_800C1EE8[arg0];
    do
    {
    }
    while (0);
    p = (u16 *) D_800C13AC;
    p += Heroes[arg0].hero_name * 5;
    if ((*d) <= p[2])
    {
      t = p[2];
    }
    else
    {
      t = p[3];
    }
    if (D_800C1EE8[arg0] == t)
    {
      if (p || Heroes[arg0].hero_name)
      {
        return 1;
      }
      else
      {
        return 1;
      }
    }
  }
  else
  {
    if ((arg0 == D_800C2724) || (arg0 == D_800C2720))
    {
      v = (~v) + 1;
    }
    if (v < 0)
    {
      u = D_800C13AC[Heroes[arg0].hero_name][0];
    }
    else
    {
      u = D_800C13AC[Heroes[arg0].hero_name][1];
    }
    if (D_800C1EE8[arg0] == u)
    {
      goto skip;
    }
  }
  D_800C1EE8[arg0] += 1;
  skip:
  if (D_800C1EE8[arg0] >= 0x1D)
  {
    D_800C1EE8[arg0] = 1;
  }

  s = Heroes[arg0].hero_sub;
  s = s->next;
  if (s != 0)
  {
    s->unk74 = D_800C13AC[Heroes[arg0].hero_name][4] + D_800C1EE8[arg0];
    return 0;
  }
  return 1;
}

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

void func_800B6CF0(s32 arg0) {
    EXTengine *e;
    EXTengine *f;
    s32 v;
    s32 done;

    done = 0;
    if (arg0 == D_800C2724) {
        e = Heroes[arg0].hero_eng;
        v = e->unk10.z;
        if (v > 0x500) {
            e->unk10.z = v - 0x20;
        } else {
            done = 1;
        }
        if (D_8005A680[0] != 0) {
            f = Heroes[arg0].hero_eng;
            if (f->unk10.x < 0) {
                f->unk10.x += 0x20;
            }
        }
    }
    if (arg0 == D_800C2728) {
        e = Heroes[arg0].hero_eng;
        v = e->unk10.z;
        if (v < -0x500) {
            e->unk10.z = v + 0x20;
        } else {
            done = 1;
        }
        if (D_8005A680[0] != 0) {
            f = Heroes[arg0].hero_eng;
            if (f->unk10.x > 0) {
                f->unk10.x -= 0x20;
            }
        }
    }
    if (arg0 == D_800C271C) {
        e = Heroes[arg0].hero_eng;
        v = e->unk10.x;
        if (v > 0x500) {
            e->unk10.x = v - 0x20;
        } else {
            done = 1;
        }
        if (D_8005A680[0] != 0) {
            f = Heroes[arg0].hero_eng;
            if (f->unk10.z < 0) {
                f->unk10.z += 0x20;
            }
        }
    }
    if (arg0 == D_800C2720) {
        e = Heroes[arg0].hero_eng;
        v = e->unk10.x;
        if (v < -0x500) {
            e->unk10.x = v + 0x20;
        } else {
            done = 1;
        }
        if (D_8005A680[0] != 0) {
            f = Heroes[arg0].hero_eng;
            if (f->unk10.z > 0) {
                f->unk10.z -= 0x20;
            }
        }
    }
    if (done != 0) {
        D_800AD12C = 1;
    }
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

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

void func_800B70CC(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    subengine *s;
    Ovl61ModeView *m;
    s32 i;

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
    func_80026980(h->hero);
    h->hero_sub->unk08 = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    SetHeroSeq(arg0, 0x20);
    func_80028018(arg0, 0x3C);
    s = h->hero_sub->next;
    if (s != NULL) {
        s = s->next;
        if (s != NULL) {
            s->flags = 0;
        }
    }
    if (arg0 == D_800C2728) {
        i = 0;
    } else if (arg0 == D_800C2724) {
        i = 1;
    } else if (arg0 == D_800C2720) {
        i = 3;
    } else {
        i = (arg0 == D_800C271C) * 2;
    }
    if (D_800C1D38[i].sub != NULL) {
        D_800C1D38[i].sub->unk74 = D_800C1D38[i].unkE;
    }
    m = (Ovl61ModeView *)D_8005A614;
    switch (m->unk34) {
    case 16:
        SpawnNASById(arg0, D_8005A5D8 - 0x50, -0x20, 0x1506, 0x11);
        break;
    case 17:
        if (m->unk30 & 0x801) {
            D_800C1F14 = 0;
            if (D_800C1C60 != NULL) {
                StopSfx(D_800C1C60);
            }
        } else {
            if (D_800C1D88[i].flags != NULL) {
                *D_800C1D88[i].flags |= 0x8000;
            }
            if (D_800C1C84 == 0) {
                PlaySfx(0x591, &D_800C1C60, NULL, 0x1000, 0x1E00, 0);
                D_800C1C84 = 1;
            }
            if (arg0 == D_800C2728) {
                D_800C1F14 |= 1;
            }
            if (arg0 == D_800C2724) {
                D_800C1F14 |= 2;
            }
            if (arg0 == D_800C2720) {
                D_800C1F14 |= 4;
            }
            if (arg0 == D_800C271C) {
                D_800C1F14 |= 8;
            }
        }
        break;
    default:
        SpawnNASById(arg0, D_8005A5D8 - 0x50, 0, 0x1505, 0x11);
        break;
    }
    Heroes[arg0].hero_hit = 0x3C;
}

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

void InitBall(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    EXTengine *e1;
    EXTobject *obj;
    EXTmotion *mot;
    subengine *sub;
    u8 *ext;
    s32 i;

    eng = nas->Engine;
    if (eng != NULL) {
    eng->sub->flags |= 0x02000000;
    eng->flags &= 0xFFFF7FFF;
    nas->Class |= 0x2000;
    obj = (EXTobject *)AddObject(nas);
    if (obj != NULL) {
        mot = AddMotion(nas);
        if (mot != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x300);
            nas->FUX = (EXT *)obj;
            obj->whatami = 1;
            obj->owner = 0x63;
            obj->status = 0;
            mot->Mass = 0x100;
            mot->Za = 0;
            mot->Zvel = 0;
            mot->Zmax = 0;
            mot->Zacc = 3;
            mot->Zdec = 3;
            mot->Ftime = 0;
        }
    }
    eng->unk10.y = D_8005A5D8 - 0x80;
    eng->unk5D = 2;
    eng->sub->unk6C = D_80054504;
    if (D_8005A648 == 0xF) {
        eng->sub->unk74 = 0x2017;
    } else {
        eng->sub->unk74 = 0x2009;
    }
    func_80026B70((u8 **)&D_8005AA38, (u8 *)nas);
    e1 = AddEngine(nas, 1);
    if (e1 != NULL) {
        sub = e1->sub;
        sub->unk6C = D_80054504;
        if (D_8005A648 == 0xF) {
            sub->unk74 = 0x2016;
        } else {
            sub->unk74 = 0x200B;
        }
        e1->flags |= 0x22000021;
        sub->flags |= 0x22000021;
        e1->flags &= 0xFFFF7FFF;
    }
    e = AddEngine(nas, 1);
    if (e != NULL) {
        sub = e->sub;
        e->unk5D = 0;
        e->unk5C = 0;
        e->flags &= 0xFFFF7FFF;
        sub->unk6C = D_80054504;
        if (D_8005A648 == 0xE) {
            sub->unk74 = 0x200C;
            sub->unk76 = 0xB00;
            sub->flags |= 0x42000021;
            ext = AddExtension(nas, 0x74, 0x10000000);
            if (ext != NULL) {
                i = 7;
                ext += i;
                do {
                    ext[0x6C] = 0;
                    i--;
                    ext--;
                } while (i >= 0);
                func_8001D390(e, 6);
                for (sub = sub->next; sub != NULL; sub = sub->next) {
                    sub->unk51 = 0;
                    sub->unk50 = 1;
                    sub->unk6C = D_80054504;
                    sub->unk74 = 0x200D;
                    sub->flags = (sub->flags & 0xFFFF7FFF) | 0x42000021;
                    sub->unk76 = 0xC00;
                }
            }
        } else {
            sub->unk74 = 0;
        }
    }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", PlotBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", CalcArkInfluence);

void FreeGrabbedObject(u32 arg0, bool arg1) {
    VECTOR sp18;
    HeroInfo *hs;
    HeroInfo *h;
    EXTengine *hero;
    EXTmotion *mot;
    EXTengine *eng;
    NAS *nas;
    EXTobject *obj;
    s32 dir;
    s32 a;
    s32 spd;

    hs = Heroes;
    h = &hs[arg0];
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    hero = h->hero_eng;
    if (hero != NULL) {
        nas = D_8005AA2C;
        if (nas != NULL) {
            do {
                eng = nas->Engine;
                if (eng != NULL) {
                    if (eng != hero) {
                        obj = FindExtension(nas, 0x1000);
                        if (obj != NULL && obj->status == 2 && arg0 == obj->player) {
                            mot = FindExtension(nas, 0x800);
                            if (mot != NULL) {
                                spd = 0x30;
                                if (arg1) {
                                    PlaySfx(D_800C18E8[arg0 + 8], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                                    spd = 0xE0;
                                }
                                mot->Xvel = spd;
                                mot->Xmax = spd;
                                if (func_80015780(hero, hero->sub, 4, &sp18) != NULL) {
                                    dir = get_angle(-eng->unk10.x + sp18.vx, -eng->unk10.z + sp18.vz);
                                } else {
                                    dir = get_angle(-eng->unk10.x + hero->unk10.x, -eng->unk10.z + hero->unk10.z);
                                }
                                a = func_800154A0(hero->unk1E, dir);
                                if (a > 0x220) {
                                    a = 0x220;
                                }
                                if (a < -0x220) {
                                    a = -0x220;
                                }
                                a += hero->unk1E;
                                a &= 0xFFF;
                                *(volatile s16 *)&mot->Xa = a;
                                eng->unk10.x -= mot->Xvel * D_80068BD4[(mot->Xa - 0x400) & 0xFFF].unk2 >> 12;
                                eng->unk10.z -= mot->Xvel * D_80068BD4[(u16)mot->Xa & 0xFFF].unk2 >> 12;
                                obj->status = 1;
                            }
                        }
                    }
                    nas = nas->Solid.Child;
                }
            } while (nas != NULL);
        }
    }
}

void AlignGrabBall(EXTengine *arg0, EXTmotion *arg1, EXTobject *arg2) {
    SLONG_XYZ v;
    VECTOR target;
    HeroInfo *hero;
    EXTengine *o;
    NAS *n;
    void *t;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 rr;
    HeroInfo *base;
    s32 a1;
    s32 a2;
    s32 d;

    base = Heroes;
    hero = &base[arg2->player];
    if (func_80015780(hero->hero_eng, hero->hero_sub, 4, &target) == NULL) {
        *(SLONG_XYZ *)&target = hero->hero_eng->unk10;
    }
    for (i = 0; i < 4; i++) {
        if (arg0->collision_box.flags & 0x40) {
            r = D_800C1A7C[i].r + arg0->collision_box.radius;
        } else {
            r = D_800C1A7C[i].r + 0x80;
        }
        dx = arg0->unk10.x - D_800C1A7C[i].x;
        dz = arg0->unk10.z - D_800C1A7C[i].z;
        if (dx * dx + dz * dz < r * r) {
            a1 = get_angle(-dx, -dz);
            v.x = D_800C1A7C[i].x;
            v.y = 0;
            v.z = D_800C1A7C[i].z;
            func_800BE1C4(&arg0->unk10, a1, &v, r);
            dx = -arg0->unk10.x;
            arg1->Xa = get_angle(dx + target.vx, (dz = -arg0->unk10.z) + target.vz);
        }
    }
    r = 0x80;
    if (arg0->collision_box.flags & 0x40) {
        r = arg0->collision_box.radius;
    }
    r *= 2;
    rr = r * r;
    for (n = D_8005AA2C; n != NULL; n = n->Solid.Child) {
        o = n->Engine;
        if (o != NULL && o != arg0) {
            t = FindExtension(n, 0x1000);
            if (t != NULL && *(u16 *)((u8 *)t + 0x1A) == 2) {
                dx = o->unk10.x - arg0->unk10.x;
                dz = o->unk10.z - arg0->unk10.z;
                if (dx * dx + dz * dz < rr) {
                    a1 = get_angle(target.vx - arg0->unk10.x, target.vz - arg0->unk10.z);
                    dx = -o->unk10.x;
                    a2 = get_angle(dx + target.vx, (dz = -o->unk10.z) + target.vz);
                    d = (0xE3 - func_80015480(a1, a2)) >> 1;
                    if (func_800154A0(a1, a2) < 0) {
                        a1 += d;
                        a2 -= d;
                    } else {
                        a2 += d;
                        a1 -= d;
                    }
                    arg1->Xa = a1;
                    t = FindExtension(n, 0x800);
                    if (t != NULL) {
                        *(s16 *)((u8 *)t + 0x16) = a2;
                    }
                }
            }
        }
    }
}

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

void func_800BD510(void) {
    Ovl61Wall *base;
    Ovl61Wall *w;
    Ovl61WallObj *o;
    s32 t;
    s32 lim;
    Ovl61WallObj *ob;
    s32 two;
    s32 three;
    s32 neg;
    s32 s;
    s32 phase;

    two = 2;
    three = 3;
    neg = -1;
    base = D_800C1E78;
    ob = D_800C1DD8;
    w = base;
    phase = (D_8004E0D8 >> 1) & 7;
    o = ob;
loop:
    {
        if (w->f1A != 0) {
            w->f1A = w->f1A - 1;
        }
        s = w->f18;
        if (s == two) {
            goto c2;
        }
        if (s < 3) {
            if (s == 1) {
                goto c1;
            }
            goto end;
        }
        if (s == three) {
            goto c3;
        }
        goto end;
        {
        c1:
            if (o->sub != NULL) {
                o->sub->unk74 = o->unkE + phase;
            }
            if (w->f1A == 0) {
                do { do { w->f18 = two; } while (0); } while (0);
            }
            if (w->f14 == neg) {
                if (w->pad0 < -0x1500) {
                    w->f14 = 0;
                    w->fC = 0x60;
                    w->f10 = 0;
                }
            } else {
                if (w->f4 > 0) {
                    w->fC = 0x10;
                }
                if (w->pad0 >= -0x12FF) {
                    do { w->f14 = neg; } while (0);
                    w->fC = 0;
                    w->f10 = 0x40;
                }
            }
            goto end;
        c2:
            w->f8 = 0x300;
            w->f14 = 0;
            w->fC = 4;
            w->f10 = 0;
            do { do { w->f18 = three; } while (0); } while (0);
            if (o->sub != NULL) {
                o->sub->unk74 = o->unkA;
            }
            goto end;
        c3:
            if (w->f14 == 0) {
                w->fC += 2;
                if (w->pad0 > 0) {
                    w->f10 = w->fC;
                    w->fC = 0;
                    w->f14 = neg;
                    w->f8 = w->f8 >> 1;
                }
            } else {
                w->f10 += 2;
                if (w->pad0 < 0) {
                    if (w->f8 > 0x40) {
                        w->fC = w->f10;
                        w->f10 = 0;
                        w->f14 = 0;
                        w->f8 = w->f8 >> 1;
                    } else {
                        w->pad0 = 0;
                        w->f4 = 0;
                        w->fC = 0;
                        w->f10 = 0;
                        w->f18 = 0;
                    }
                }
            }
        }
    end:
        w->f4 += w->fC;
        w->f4 -= w->f10;
        lim = w->f8;
        if (lim < w->f4) {
            w->f4 = lim;
        }
        t = -w->f8;
        if (w->f4 < t) {
            w->f4 = t;
        }
        w->pad0 += w->f4;
        w++;
        o++;
    }
    do {
        if ((s32)w < (s32)(base + 4)) {
            goto loop;
        }
    } while (0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BD770);

void Plot_Ark_Pickup(NAS *arg0) {
    EXTobject *fux;
    EXTengine *e;
    HeroInfo *h;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;

    if (arg0->Engine != NULL && (fux = (EXTobject *)arg0->FUX) != NULL) {
        switch (fux->status) {
        case 0:
            fux->status = 1;
            fux->can_do = (fux->can_do & 0xFFD0) | 0x12;
            func_80026AD8((u8 *)arg0);
            fux->player = -1;
            break;
        case 7:
            fux->status = 0;
            break;
        default:
            for (i = 0; i < 4; i++) {
                h = &Heroes[i];
                e = h->hero_eng;
                dx = e->unk10.x - D_800C1A7C[arg0->UserInfo.user1].x;
                dz = e->unk10.z - D_800C1A7C[arg0->UserInfo.user1].z;
                if (e->collision_box.flags & 0x40) {
                    r = D_800C1A7C[arg0->UserInfo.user1].r + e->collision_box.radius;
                } else {
                    r = D_800C1A7C[arg0->UserInfo.user1].r + 0x80;
                }
                r += 0x40;
                if (dx * dx + dz * dz < r * r) {
                    D_8009D754 = i;
                    h->hero_pu = fux->contains;
                    if (fux->contains == 0x12 || fux->contains == 0x17) {
                        SpawnNASById(h->hero_eng->unk10.x, h->hero_eng->unk10.y, h->hero_eng->unk10.z, 0x1503, 0x11);
                    }
                    D_800C1E28[arg0->UserInfo.user1].unk10 = 0;
                    PlaySfx(0x5DA, NULL, NULL, 0x1000, 0x1E00, 0);
                    goto kill;
                }
            }
            break;
        }
    }
    if ((u16)D_800C1B2C->unk1A != 0) {
        arg0->UserInfo.user2 = 0;
    }
    if (arg0->UserInfo.user2 != 0) {
        arg0->UserInfo.user2--;
        return;
    }
    PlaySfx(0x5D8, NULL, NULL, 0x1000, 0x1E00, 0);
    D_800C1E28[arg0->UserInfo.user1].unk10 = 0;
kill:
    if (arg0->Family.Child != NULL) {
        KillNAS(arg0->Family.Child);
    }
    KillNAS(arg0);
}

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

void func_800BE1C4(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r) {
    SLONG_XYZ start;
    SLONG_XYZ d;
    SLONG_XYZ dir;
    s32 a;
    s32 b;
    s32 c;
    s32 s;
    s32 t1;
    s32 t2;

    arg1 = (arg1 + 0x800) & 0xFFF;
    start = *pos;
    start.x -= (0x200 * D_80068BD4[(arg1 - 0x400) & 0xFFF].unk2) >> 12;
    start.z -= (0x200 * D_80068BD4[arg1].unk2) >> 12;
    dir.x = start.x - pos->x;
    dir.y = 0;
    dir.z = start.z - pos->z;
    if (dir.x == 0) {
        return;
    }
    d.x = target->x - start.x;
    d.y = 0;
    d.z = target->z - start.z;
    a = (dir.x * dir.x + dir.z * dir.z) >> 4;
    b = ((dir.x * d.x + dir.z * d.z) * 2) >> 4;
    c = (d.x * d.x + d.z * d.z - r * r) >> 4;
    s = b * b - a * c * 4;
    if (s < 0) {
        return;
    }
    s = csqrt(s) >> 6;
    t1 = ((-b + s) << 8) / (a * 2);
    t2 = ((-b - s) << 8) / (a * 2);
    if (t1 < 0 && t2 < 0) {
        return;
    }
    if (t1 < t2) {
        if (t1 < 0) {
            pos->x = start.x - ((dir.x * t2) >> 8);
            pos->y = start.y - ((dir.y * t2) >> 8);
            pos->z = start.z - ((dir.z * t2) >> 8);
        } else {
            pos->x = start.x - ((dir.x * t1) >> 8);
            pos->y = start.y - ((dir.y * t1) >> 8);
            pos->z = start.z - ((dir.z * t1) >> 8);
        }
    } else if (t2 >= 0) {
        pos->x = start.x - ((dir.x * t2) >> 8);
        pos->y = start.y - ((dir.y * t2) >> 8);
        pos->z = start.z - ((dir.z * t2) >> 8);
    } else {
        pos->x = start.x - ((dir.x * t1) >> 8);
        pos->y = start.y - ((dir.y * t1) >> 8);
        pos->z = start.z - ((dir.z * t1) >> 8);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BE4D4);

bool ArkHeroRad2ObjectCollision(u32 arg0, s32 arg1, bool arg2) {
    EXTengine *hero;
    EXTengine *o;
    FlagRecord *rec;
    u8 *n;
    u8 *t;
    u8 *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
    s32 ang;
    u16 v;
    u16 old;
    bool hit;

    hero = Heroes[arg0].hero_eng;
    hit = 0;
    if (hero != NULL) {
        n = D_8005AA38;
        if (n != NULL) {
            do {
                o = *(EXTengine **)(n + 0x54);
                if (o != NULL && o != hero) {
                    rec = func_80025A48(o, 0x40);
                    if (rec != NULL) {
                        dx = o->unk10.x - hero->unk10.x;
                        dz = o->unk10.z - hero->unk10.z;
                        r = rec->radius;
                        r += arg1;
                        d = dx * dx + dz * dz;
                        if (d < r * r) {
                            d = csqrt(d) >> 6;
                            if (d < r) {
                                t = FindExtension((NAS *)n, 0x1000);
                                if (t != NULL
                                    && (*(u16 *)(t + 0x1A) == 1 || *(u16 *)(t + 0x1A) == 8)
                                    && *(u16 *)(t + 0x14) == 1 && *(s16 *)(t + 0x20) == 0) {
                                    ang = get_angle(-dx, -dz) & 0xFFF;
                                    d = r - d + 1;
                                    o->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                    o->unk10.z -= (d * D_80068BD4[ang].unk2) >> 12;
                                    *(u16 *)(t + 0x16) = arg0;
                                    e = FindExtension((NAS *)n, 0x800);
                                    if (e != NULL) {
                                        hit = 1;
                                        PlaySfx(0x585, NULL, NULL, 0x1000, 0x1E00, 0);
                                        *(s16 *)(t + 0x20) = 4;
                                        *(s16 *)(e + 0x16) = ang;
                                        switch (D_8005A648) {
                                        case 0xF:
                                            *(s16 *)(e + 0xE) = *(u16 *)(e + 0x14) + 0x40;
                                            *(u16 *)(e + 0x14) = 0x40;
                                            break;
                                        case 0x11:
                                            if (arg2) {
                                                o->sub->unk74 = 0x2018;
                                                *(u16 *)(t + 0x14) = 2;
                                                *(s16 *)(e + 0xE) = *(u16 *)(e + 0x14) + 0x40;
                                                *(u16 *)(e + 0x14) = 0x80;
                                            } else {
                                                *(s16 *)(e + 0xE) = *(u16 *)(e + 0x14) + 0x40;
                                                *(u16 *)(e + 0x14) = 0x80;
                                            }
                                            break;
                                        default:
                                            *(s16 *)(e + 0xE) = *(u16 *)(e + 0x14) + 0x40;
                                            *(u16 *)(e + 0x14) = 0x90;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                n = *(u8 **)(n + 0x34);
            } while (n != NULL);
        }
    }
    return hit;
}

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

s32 func_800BFA24(Ovl61Vel *arg0, s32 arg1) {
    s32 i;
    FlagRecord *rec;
    s32 r;
    s32 dz;
    s32 dx;
    EXTengine *eng;
    HeroInfo *hero;
    HeroInfo *base = Heroes;

    for (i = 0; i < 4; i++) {
        hero = &base[i];
        if (hero->hero_status & 0xF800) {
            continue;
        }
        eng = hero->hero_eng;
        if (eng == NULL) {
            continue;
        }
        do {
            rec = func_80025A48(eng, 0x40);
            if (rec == NULL) {
                goto next;
            }
        } while (0);
        dx = eng->unk10.x - arg0->x;
        dz = eng->unk10.z - arg0->z;
        r = rec->radius + (arg1 & 0xFFFF);
        r = r * r;
        if ((dx * dx) + (dz * dz) < r) {
            return i;
        }
    next:;
    }
    return -1;
}

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
