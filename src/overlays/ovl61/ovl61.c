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

void AR_PreInitLevel(void) {
    u8 *g;
    void **p;
    void **p2;
    void **p3;
    void **p4;
    void *t;

    D_8009D6E8.unk4 = D_800C0E70;
    D_8009D6E8.unk0 = (s32)AR_InitHero;
    D_8009D6E8.unk18 = AR_InitShadow;
    D_8009D6E8.unk1C = AR_UpdateShadow;
    D_8009D6E8.unk38 = Ark_HeroBot;
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
        D_8009D6E8.unk68 = AR_KeepInBounds;
        D_8009D6E8.unk20 = AR_PreMoveHero;
        D_8009D6E8.unk24 = AR_MoveHero;
        D_800C1C8C = func_800B4880;
        D_800C1C90 = func_800C0A74;
        D_800C1C94 = AR_BA_TestLaunchBounds;
        D_8005AA94 = (void *)GetArkContour;
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
        D_8009D6E8.unk68 = AR_KeepInBounds;
        D_8009D6E8.unk20 = AR_PreMoveHero;
        D_8009D6E8.unk24 = AR_MoveHero;
        D_800C1C8C = func_800B4880;
        D_800C1C90 = func_800C096C;
        D_800C1C94 = AR_BA_TestLaunchBounds;
        D_8005AA94 = (void *)GetArkContour;
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
        D_8009D6E8.unk68 = AR_PI_KeepInBounds;
        D_8009D6E8.unk20 = AR_PI_PreMoveHero;
        D_8009D6E8.unk24 = AR_PI_MoveHero;
        D_800C1C8C = func_800B4AE8;
        D_800C1C90 = AR_PI_TestOutOfBounds;
        D_800C1C94 = AR_PI_TestLaunchBounds;
        D_8005AA94 = (void *)GetArkPIContour;
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
        D_8009D6E8.unk68 = AR_KeepInBounds;
        D_8009D6E8.unk20 = AR_PreMoveHero;
        D_8009D6E8.unk24 = AR_MoveHero;
        D_800C1C8C = func_800B4880;
        D_800C1C90 = func_800C0A74;
        D_800C1C94 = AR_BA_TestLaunchBounds;
        D_8005AA94 = (void *)GetArkContour;
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

void AR_InitLevel(void) {
    Ovl61ModeView *m;
    Ovl61ModeView *m2;
    HeroInfo *h;
    EXTengine *e;
    EXTheromove *mv;
    s32 i;
    s32 a;

    D_8005A5D8 = 0;
    D_8005AA38 = 0;
    D_8005AA3C = 0;
    D_800C1F02 = 0;
    D_800C1F04 = 0;
    D_800C1F06 = 0;
    D_800C1F08 = 0;
    D_800C1F0A = 0xB4;
    D_800C1F0C = 0xB4;
    D_800C1F14 = 0;
    D_800C1F16 = -1;
    D_800C1B28 = 0;
    D_800C1B2C = 0;
    D_800C1C84 = 0;
    D_800C2718 = Random(15);
    D_800C1C70 = 150;
    D_800C1C74 = Random(18);
    D_800C1C78 = 270;
    D_800C1C7C = Random(18);
    D_800C1C80 = -1;
    D_800C1EF8 = 0;
    D_800C1EFA = 0;
    D_800C1EFC = 10;
    D_800C1F00 = 0;
    D_800C1B30 = EngineTemplate;
    D_800C1BCC = EngineTemplate;
    D_800C1C88 = 30;
    D_800C1EFE = 0;

    for (i = 0, m = (Ovl61ModeView *)D_8005A614; i < 4; i++) {
        h = &Heroes[i];
        a = get_angle(h->hero_eng->unk10.x, h->hero_eng->unk10.z) + 0x7F;
        mv = h->hero_move;
        e = h->hero_eng;
        a &= 0xC00;
        e->unk1E = a;
        mv->facing_angle = a;
        mv->forward_angle = a;
        func_80026980(h->hero);
        switch (m->unk34) {
        case 15:
            D_800C1A7C = D_800C10F4;
            D_800C1A80 = D_800C110C;
            h->hero_score = 12;
            h->hero_health = 12;
            D_800C1A84 = D_800C1124;
            if (m->unk32 & 0x40) {
                if (m->unk6C != 0) {
                    Heroes[0].hero_score = 18;
                    Heroes[1].hero_score = 0;
                    Heroes[2].hero_score = 30;
                    Heroes[3].hero_score = 0;
                } else {
                    Heroes[0].hero_score = 9;
                    Heroes[1].hero_score = 15;
                    Heroes[2].hero_score = 15;
                    Heroes[3].hero_score = 15;
                }
            }
            break;
        case 16:
            D_800C1A7C = D_800C1154;
            D_800C1A80 = D_800C116C;
            h->hero_score = 20;
            h->hero_health = 20;
            D_800C1A84 = D_800C1184;
            if (m->unk32 & 0x40) {
                if (m->unk6C != 0) {
                    Heroes[0].hero_score = 30;
                    Heroes[1].hero_score = 0;
                    Heroes[2].hero_score = 40;
                    Heroes[3].hero_score = 0;
                } else {
                    Heroes[0].hero_score = 15;
                    Heroes[1].hero_score = 20;
                    Heroes[2].hero_score = 20;
                    Heroes[3].hero_score = 20;
                }
            }
            break;
        case 17:
            D_800C1A7C = D_800C11B4;
            D_800C1A80 = D_800C11CC;
            D_800C1A84 = D_800C11E4;
            if (m->unk32 & 0x80) {
                h->hero_score = 10;
                h->hero_health = 10;
            } else {
                h->hero_score = 20;
                h->hero_health = 20;
            }
            if (m->unk32 & 0x40) {
                if (m->unk6C != 0) {
                    Heroes[0].hero_score = 18;
                    Heroes[1].hero_score = 0;
                    Heroes[2].hero_score = 25;
                    Heroes[3].hero_score = 0;
                } else {
                    Heroes[0].hero_score = 9;
                    Heroes[1].hero_score = 15;
                    Heroes[2].hero_score = 15;
                    Heroes[3].hero_score = 15;
                }
            }
            break;
        case 0:
        default:
            D_800C1A7C = D_800C1094;
            D_800C1A80 = D_800C10AC;
            h->hero_score = 15;
            h->hero_health = 15;
            D_800C1A84 = D_800C10C4;
            if (m->unk32 & 0x40) {
                if (m->unk6C != 0) {
                    Heroes[0].hero_score = Heroes[0].hero_score_displayed = 18;
                    Heroes[1].hero_score = Heroes[1].hero_score_displayed = 0;
                    Heroes[2].hero_score = Heroes[2].hero_score_displayed = 30;
                    Heroes[3].hero_score = Heroes[3].hero_score_displayed = 0;
                } else {
                    Heroes[0].hero_score = Heroes[0].hero_score_displayed = 9;
                    Heroes[1].hero_score = Heroes[1].hero_score_displayed = 15;
                    Heroes[2].hero_score = Heroes[2].hero_score_displayed = 15;
                    Heroes[3].hero_score = Heroes[3].hero_score_displayed = 15;
                }
            }
            break;
        }
        Heroes[0].hero_score_displayed = Heroes[0].hero_score;
        Heroes[1].hero_score_displayed = Heroes[1].hero_score;
        Heroes[2].hero_score_displayed = Heroes[2].hero_score;
        Heroes[3].hero_score_displayed = Heroes[3].hero_score;
        func_800B3528(i);
    }
    m2 = (Ovl61ModeView *)D_8005A614;
    if ((m2->unk32 & 0x40) && m2->unk6C != 0) {
        func_8007D144();
    }
    switch (D_8005A648) {
    case 0x10:
        SpawnNASById(0, D_8005A5D8, 0, 0x1507, 0x11);
        break;
    case 0x11:
        PlaySfx(0x5C8, &D_800C1BC4, 0, 0x1000, 0x1E00, 0);
        break;
    }
    func_800BC564();
}

void AR_UpdateLevel(void) {
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

bool AR_KeepInBounds(EXTengine *arg0) {
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 a;

    for (i = 0; i < 4; i++) {
        dx = arg0->unk10.x - D_800C1A7C[i].x;
        dz = arg0->unk10.z - D_800C1A7C[i].z;
        if (arg0->collision_box.flags & 0x40) {
            r = D_800C1A7C[i].r + arg0->collision_box.radius;
        } else {
            r = D_800C1A7C[i].r + 0x80;
        }
        if (dx * dx + dz * dz < r * r) {
            a = get_angle(-dx, -dz);
            if ((void *)arg0 != (void *)Heroes[D_800C2720].hero_eng &&
                (void *)arg0 != (void *)Heroes[D_800C271C].hero_eng) {
                arg0->unk10.x = D_800C1A7C[i].x - (r * D_80068BD4[(a - 0x400) & 0xFFF].unk2 >> 12);
            }
            if ((void *)arg0 != (void *)Heroes[D_800C2724].hero_eng &&
                (void *)arg0 != (void *)Heroes[D_800C2728].hero_eng) {
                arg0->unk10.z = D_800C1A7C[i].z - (r * D_80068BD4[a & 0xFFF].unk2 >> 12);
            }
            return 1;
        }
    }
    return 0;
}

bool AR_PI_KeepInBounds(EXTengine *arg0) {
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
    a = arg0->unk1E;
    arg0->unk10.x = D_80068BD4[(a - 0x400) & 0xFFF].unk2 * 9 >> 4;
    arg0->unk10.z = D_80068BD4[a & 0xFFF].unk2 * 9 >> 4;
    for (i = 0; i < 4; i++) {
        dx = arg0->unk10.x - D_800C1A7C[i].x;
        dz = arg0->unk10.z - D_800C1A7C[i].z;
        if (arg0->collision_box.flags & 0x40) {
            r = D_800C1A7C[i].r + arg0->collision_box.radius;
        } else {
            r = D_800C1A7C[i].r + 0x80;
        }
        if (dx * dx + dz * dz < r * r) {
            a = get_angle(-dx, -dz);
            if ((void *)arg0 != (void *)Heroes[D_800C2720].hero_eng &&
                (void *)arg0 != (void *)Heroes[D_800C271C].hero_eng) {
                arg0->unk10.x = D_800C1A7C[i].x - (r * D_80068BD4[(a - 0x400) & 0xFFF].unk2 >> 12);
            }
            if ((void *)arg0 != (void *)Heroes[D_800C2724].hero_eng &&
                (void *)arg0 != (void *)Heroes[D_800C2728].hero_eng) {
                arg0->unk10.z = D_800C1A7C[i].z - (r * D_80068BD4[a & 0xFFF].unk2 >> 12);
            }
            a = get_angle(arg0->unk10.x, arg0->unk10.z);
            arg0->unk1E = a;
            arg0->unk10.x = D_80068BD4[(a - 0x400) & 0xFFF].unk2 * 9 >> 4;
            arg0->unk10.z = D_80068BD4[a & 0xFFF].unk2 * 9 >> 4;
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

s32 func_800B4AE8(EXTengine *arg0, s32 arg1) {
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
        if (arg0->collision_box.flags & 0x40) {
            r = D_800C1A7C[i].r + arg0->collision_box.radius;
        } else {
            r = D_800C1A7C[i].r + 0x80;
        }
        cx = D_800C1A7C[i].x;
        dx = arg0->unk10.x - cx;
        dz = arg0->unk10.z - D_800C1A7C[i].z;
        if (dx * dx + dz * dz < r * r) {
            v.x = cx;
            v.y = 0;
            v.z = D_800C1A7C[i].z;
            func_800BE1C4(&arg0->unk10, arg1, &v, r);
            a = get_angle(D_800C1A7C[i].x - arg0->unk10.x, D_800C1A7C[i].z - arg0->unk10.z);
            D_800C1E28[i].state = 3;
            ret = a;
        }
        if (D_800C1F14 != 0) {
            dx = arg0->unk10.x;
            dz = arg0->unk10.z;
            if (arg0->collision_box.flags & 0x40) {
                r = 0x800 - arg0->collision_box.radius;
            } else {
                r = 0x800;
            }
            if (dx > r && (D_800C1F14 & 0x88)) {
                arg0->unk10.x = r;
                a = 0x400;
            }
            if (dx < -r && (D_800C1F14 & 0x44)) {
                arg0->unk10.x = -r;
                a = 0xC00;
            }
            if (dz > r && (D_800C1F14 & 0x22)) {
                arg0->unk10.z = r;
                a = 0;
            }
            if (dz < -r && (D_800C1F14 & 0x11)) {
                arg0->unk10.z = -r;
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

bool AR_PI_TestOutOfBounds(EXTengine *arg0, EXTobject *arg1, bool arg2) {
    s32 lim = 0x980;
    s32 x, z;
    s32 a;

    if (arg2 != 0) {
        lim = 0xC00;
    }
    x = arg0->unk10.x;
    z = arg0->unk10.z;
    if (x * x + z * z > lim * lim) {
        a = get_angle(-x, -z) + 0x200;
        a &= 0xC00;
        if (a == 0x400) {
            arg1->player = D_800C2720;
            return 1;
        } else if (a == 0x800) {
            arg1->player = D_800C2724;
            return 1;
        } else if (a == 0xC00) {
            arg1->player = D_800C271C;
            return 1;
        } else {
            arg1->player = D_800C2728;
            return 1;
        }
    }
    if (arg2 != 0) {
        s16 d = arg1->player;
        if (d == D_800C2724 || d == D_800C2728) {
            if (x >= 0x601) {
                arg0->unk10.x = 0x600;
            }
            if (arg0->unk10.x < -0x600) {
                arg0->unk10.x = -0x600;
            }
        } else {
            if (z >= 0x601) {
                arg0->unk10.z = 0x600;
            }
            if (arg0->unk10.z < -0x600) {
                arg0->unk10.z = -0x600;
            }
        }
    }
    return 0;
}

bool AR_BA_TestLaunchBounds(EXTengine *arg0, EXTobject *obj) {
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
        if (arg0->collision_box.flags & 0x40) {
            r = c->r + arg0->collision_box.radius;
        } else {
            r = c->r + 0x80;
        }
        dx = c->x;
        x = arg0->unk10.x;
        dx = x - dx;
        z = arg0->unk10.z;
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

void AR_InitHero(u32 arg0) {
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

void AR_PreMoveHero(u32 arg0) {
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

void AR_PI_PreMoveHero(u32 arg0) {
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

void AR_MoveHero(u32 arg0) {
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

void AR_PI_MoveHero(u32 arg0) {
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

void ArkHeroIdle(u32 arg0) {
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

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl61/ovl61", D_800B32B8);

void SetArkHeroTaunt(u32 arg0) {
    s32 id;
    SLONG_XYZ *pos;
    HeroInfo *base;

    SetHeroSeq((HeroInfo *)arg0, 0x1C);
    if (Random(3) == 0 || D_8005A614[arg0 + 0x5C] == 0) {
        switch (Heroes[arg0].hero_name) {
        case 1:
            PlaySfx(7, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 2:
            PlaySfx(0x39, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 3:
            PlaySfx(0x6B, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 4:
            PlaySfx(0x9D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 5:
            PlaySfx(0x133, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 6:
            PlaySfx(0x165, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 7:
            PlaySfx(0x101, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 8:
            PlaySfx(0xCF, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        }
    }
}

void ArkHeroTaunt(u32 arg0) {
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

void ArkHeroMove(u32 arg0) {
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

void SetArkHeroWinner(u32 arg0) {
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

void ArkHeroWinner(u32 arg0) {
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

void SetArkHeroLose(u32 arg0) {
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

void SetArkHeroDie(u32 arg0) {
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

void SetArkHeroKick(u32 arg0) {
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

void ArkHeroKick(u32 arg0) {
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

void ArkHeroRedKick(u32 arg0) {
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

void ArkHeroGrab(u32 arg0) {
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

void SetArkHeroDead(u32 arg0) {
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

void ArkHeroDead(u32 arg0) {
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

s32 func_800B7C60(s32 arg0, Ovl61Pos *from, Ovl61Pos *to, s32 angle) {
    s32 i;
    s32 n;
    u16 ang;
    s16 x0;
    s16 z0;
    s16 x1;
    s16 z1;
    s32 dz;
    s32 ox;
    s32 oz;
    s32 dx;
    s32 ez;
    s32 ex;
    s32 num;
    s32 den;
    s32 num2;
    s32 t;

    if (arg0 == D_800C2728) {
        i = 0;
    } else if (arg0 == D_800C2724) {
        i = 1;
    } else if (arg0 == D_800C2720) {
        i = 3;
    } else {
        i = (arg0 == D_800C271C) * 2;
    }
    n = 0x20 - D_8009D87C[arg0].unk0;
    n -= D_800C1F00;
    x0 = D_800C1900[i].x0;
    z0 = D_800C1900[i].z0;
    x1 = D_800C1900[i].x1;
    z1 = D_800C1900[i].z1;
    if (n < 0) {
        n = 0;
    }
    n = D_800C1980[n];
    /* The empty loops raise angle's ref count so it gets s1 (hack). */
    do {
        do {
            do {
                do {
                    ang = angle + (Random(n) - n);
                } while (0);
            } while (0);
        } while (0);
    } while (0);
    to->x = from->x - D_80068BD4[(ang - 0x400) & 0xFFF].unk2;
    to->z = from->z - D_80068BD4[ang & 0xFFF].unk2;
    if (x0 < from->x && x1 < from->x && x0 < to->x && x1 < to->x) {
        return 0;
    }
    if (from->x < x0 && from->x < x1 && to->x < x0 && to->x < x1) {
        return 0;
    }
    if (z0 < from->z && z1 < from->z && z0 < to->z && z1 < to->z) {
        return 0;
    }
    if (from->z < z0 && from->z < z1 && to->z < z0 && to->z < z1) {
        return 0;
    }
    dz = (from->z - to->z) >> 4;
    ox = (x0 - from->x) >> 4;
    oz = (z0 - from->z) >> 4;
    dx = (from->x - to->x) >> 4;
    ez = (z1 - z0) >> 4;
    ex = (x1 - x0) >> 4;
    num = dz * ox - dx * oz;
    den = ez * dx - ex * dz;
    if (den > 0) {
        if (den < num || num < 0) {
            return 0;
        }
    } else {
        if (num < den || num > 0) {
            return 0;
        }
    }
    num2 = ex * oz - ez * ox;
    if (den > 0) {
        if (den < num2 || num2 < 0) {
            return 0;
        }
    } else {
        if (num2 < den || num2 > 0) {
            return 0;
        }
    }
    if (den == 0) {
        return 0;
    }
    t = (num << 8) / den;
    to->x = x0 + ((x1 - x0) * t >> 8);
    to->z = z0 + ((z1 - z0) * t >> 8);
    return 1;
}

s32 func_800B808C(s32 arg0, Ovl61Pos *from, Ovl61Pos *to, s32 angle) {
    s32 i;
    s32 n;
    u16 ang;
    s16 x0;
    s16 z0;
    s16 x1;
    s16 z1;
    s32 dz;
    s32 ox;
    s32 oz;
    s32 dx;
    s32 ez;
    s32 ex;
    s32 num;
    s32 den;
    s32 num2;
    s32 t;

    if (arg0 == D_800C2728) {
        i = 0;
    } else if (arg0 == D_800C2724) {
        i = 1;
    } else if (arg0 == D_800C2720) {
        i = 3;
    } else {
        i = (arg0 == D_800C271C) * 2;
    }
    n = 0x20 - D_8009D87C[arg0].unk0;
    n -= D_800C1F00;
    x0 = D_800C1940[i].x0;
    z0 = D_800C1940[i].z0;
    x1 = D_800C1940[i].x1;
    z1 = D_800C1940[i].z1;
    if (n < 0) {
        n = 0;
    }
    n = D_800C1980[n];
    /* The empty loops raise angle's ref count so it gets s1 (hack). */
    do {
        do {
            do {
                do {
                    ang = angle + (Random(n) - n);
                } while (0);
            } while (0);
        } while (0);
    } while (0);
    to->x = from->x - D_80068BD4[(ang - 0x400) & 0xFFF].unk2;
    to->z = from->z - D_80068BD4[ang & 0xFFF].unk2;
    if (x0 < from->x && x1 < from->x && x0 < to->x && x1 < to->x) {
        return 0;
    }
    if (from->x < x0 && from->x < x1 && to->x < x0 && to->x < x1) {
        return 0;
    }
    if (z0 < from->z && z1 < from->z && z0 < to->z && z1 < to->z) {
        return 0;
    }
    if (from->z < z0 && from->z < z1 && to->z < z0 && to->z < z1) {
        return 0;
    }
    dz = (from->z - to->z) >> 4;
    ox = (x0 - from->x) >> 4;
    oz = (z0 - from->z) >> 4;
    dx = (from->x - to->x) >> 4;
    ez = (z1 - z0) >> 4;
    ex = (x1 - x0) >> 4;
    num = dz * ox - dx * oz;
    den = ez * dx - ex * dz;
    if (den > 0) {
        if (den < num || num < 0) {
            return 0;
        }
    } else {
        if (num < den || num > 0) {
            return 0;
        }
    }
    num2 = ex * oz - ez * ox;
    if (den > 0) {
        if (den < num2 || num2 < 0) {
            return 0;
        }
    } else {
        if (num2 < den || num2 > 0) {
            return 0;
        }
    }
    if (den == 0) {
        return 0;
    }
    t = (num << 8) / den;
    to->x = x0 + ((x1 - x0) * t >> 8);
    to->z = z0 + ((z1 - z0) * t >> 8);
    return 1;
}

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

s32 func_800B85DC(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, s32 arg5) {
    Ovl61Pos buf;
    Ovl61Node *n;
    Ovl61Node *t;
    Ovl61Node *best;
    HeroInfo *h;
    HeroInfo *h1;
    HeroInfo *h2;
    s32 dx;
    s32 dz;
    s32 a;
    s32 d;
    s32 r;

    arg2 *= arg2;
    if (MODEV->unk34 < 0xE) {
        goto ret0;
    }
    if (MODEV->unk34 >= 0x11) {
        if (MODEV->unk34 != 0x11) {
            return 0;
        }
        if (MODEV->unk32 & 0x80) {
            n = D_8005AA38;
            if (n != NULL) {
                HeroInfo *base = Heroes;

                h1 = &base[arg0];
                do {
                    t = n->target;
                    if (t != NULL) {
                        dx = h1->hero_eng->unk10.x - t->x;
                        dz = h1->hero_eng->unk10.z - t->z;
                        if (dx * dx + dz * dz < arg2 && n->unk58 != NULL && n->unk58->unk14 == 2) {
                            goto set_ai;
                        }
                    }
                    n = n->next;
                } while (n != NULL);
            }
        }
    }
    if (Heroes[arg0].hero_attack_timer != 0) {
        return 0;
    }
    best = NULL;
    n = D_8005AA38;
    if (n != NULL) {
        h = &Heroes[arg0];
    loop:
        /* the loop notes weight these refs for global-alloc as a loop's would, without loop.c
           hoisting the (s16) casts out as it does from a real loop */
        do {
            t = n->target;
            if (t != NULL) {
                dx = h->hero_eng->unk10.x - t->x;
                dz = h->hero_eng->unk10.z - t->z;
                if (dx * dx + dz * dz < arg2 && n->unk58 != NULL && n->unk58->unk14 != 2) {
                    a = get_angle(dx, dz);
                    d = func_800154A0(a, h->hero_move->forward_angle);
                    if (arg1 == -1) {
                        if ((s16)arg3 < d) {
                            if (d < (s16)arg4) {
                                best = n;
                            }
                        }
                    } else if (FindExtension((NAS *)n, 0x800) != NULL) {
                        r = func_800B808C(arg1, (Ovl61Pos *)&t->x, &buf, (u16)a);
                        best = n;
                        if (r != 0) {
                            goto found;
                        }
                    }
                }
            }
        } while (0);
        n = n->next;
        if (n != NULL) {
            goto loop;
        }
    }
    if (best == NULL) {
        return 0;
    }
    if (arg5 == 0) {
        goto ret0;
    }
    if (D_8005A680[0] != 0) {
        HeroInfo *base = Heroes;

        h2 = &base[arg0];
        arg1 = h2->hero_teammate;
        t = best->target;
        a = get_angle(h2->hero_eng->unk10.x - t->x, h2->hero_eng->unk10.z - t->z);
        if (FindExtension((NAS *)best, 0x800) == NULL) {
            goto ret0;
        }
        if ((func_800B808C(arg1, (Ovl61Pos *)&t->x, &buf, (u16)a) ^ 1) == 0) {
            goto ret0;
        }
    }
found:
    return 1;
set_ai:
    h1->hero_ai->AIpad |= 0x2000;
    return 0;
ret0:
    return 0;
}

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

void Ark_HeroBot(u32 arg0) {
    s32 unused[2];
    Ovl61Pos best;
    Ovl61Pos pos;
    s32 dist;
    s32 ang;
    s32 mate;
    EXTbotai *ai;
    Ovl61AiNode *n;
    Ovl61AiNode *node;
    EXTengine *e;
    EXTengine *obj;
    EXTengine *eng;
    Ovl61AiExt *x;
    Ovl61AiExt *ext;
    s32 min;
    s32 target;
    s32 row;
    s32 col;
    s32 k;
    Ovl61Pos *at;
    s32 range;
    s32 spread;
    s32 dx;
    s32 d;
    s32 r;
    s32 flag;
    s32 v;
    PadState *pad;
    D_8009D87C_t *sk;

    if (D_8005A624 & 0x04000000) {
        return;
    }
    ai = Heroes[arg0].hero_ai;
    if (ai->AIinitial_delay != 0) {
        ai->AIinitial_delay--;
    }
    if (ai->AIsleep != 0) {
        ai->AIsleep--;
    }
    if (ai->AItimer1 != 0) {
        ai->AItimer1--;
    }
    if (ai->AItimer2 != 0) {
        ai->AItimer2--;
    }
    if (ai->AItimer3 != 0) {
        ai->AItimer3--;
    }
    ai->AIpad = 0;
    mate = Heroes[arg0].hero_teammate;
    if (Heroes[arg0].hero_mode == (u16)D_8009E0D8 || Heroes[arg0].hero_mode == (u16)D_8009E0E2) {
        ai->AIsleep = 5;
        ai->AImode = 0x14;
    }
    min = 0x7FFFFF;
    n = D_8005AA38;
    if (ai->TargetPlayer == -1) {
        ai->TargetPlayer = func_800B84B8(arg0);
    }
    target = ai->TargetPlayer;
    k = (0x20 - D_8009D87C[arg0].unk0) << 4;
    for (; n != NULL; n = n->next) {
        e = n->eng;
        if (e != NULL && (ext = (Ovl61AiExt *)n->fux) != NULL && ext->unk1A == 1) {
            x = FindExtension((NAS *)n, 0x800);
            if (x != NULL && func_800B7C60(arg0, (Ovl61Pos *)&e->unk10, &pos, x->unk16) != 0) {
                dx = (e->unk10.x + k) - (pos.x + k);
                row = e->unk10.z - pos.z;
                dx = dx * dx;
                d = dx + row * row;
                if (d < min) {
                    min = d;
                    best = pos;
                }
            }
        }
    }
    if (target != -1) {
        row = arg0 == D_800C2728;
        if (arg0 == D_800C2720) {
            row = 2;
        }
        if (arg0 == D_800C271C) {
            row = 3;
        }
        if (target == *D_800C19C0[row][0]) {
            col = 0;
        } else if (target == *D_800C19C0[row][1]) {
            col = 1;
        } else if (target == *D_800C19C0[row][2]) {
            col = 2;
        } else {
            col = 3;
        }
        v = D_800C19F0[col];
        if (arg0 == D_800C2724) {
            best.x -= v;
        }
        if (arg0 == D_800C2728) {
            best.x += v;
        }
        if (arg0 == D_800C2720) {
            best.z -= v;
        }
        if (arg0 == D_800C271C) {
            best.x -= v;
        }
    }
    if (D_8005A648 == 0x10) {
        k = (0x20 - D_8009D87C[arg0].unk0) << 5;
        if (k <= 0) {
            k = 1;
        }
        for (node = D_8005AA24; node != NULL; node = node->child) {
            obj = node->eng;
            if (obj != NULL && (ext = (Ovl61AiExt *)node->fux) != NULL &&
                ((Ovl61AiFux *)ext)->unk24 == 0x12) {
                eng = Heroes[arg0].hero_eng;
                dx = eng->unk10.x - obj->unk10.x;
                dx += k;
                row = eng->unk10.z - obj->unk10.z;
                row += k;
                d = dx * dx + row * row;
                if (d < min) {
                    min = d;
                    best = *(Ovl61Pos *)&obj->unk10;
                }
            }
        }
    }
    if (min != 0x7FFFFF) {
        ang = get_angle(best.x - Heroes[arg0].hero_eng->unk10.x,
                        best.z - Heroes[arg0].hero_eng->unk10.z);
        dist = csqrt(min) >> 6;
    } else {
        dist = 0;
        ang = get_angle(ai->TargetXYZ.x - Heroes[arg0].hero_eng->unk10.x,
                        ai->TargetXYZ.z - Heroes[arg0].hero_eng->unk10.z);
    }
    if (ai->AItimer2 != 0) {
        if (func_800B8964(arg0) != 0) {
            ai->AIpad |= 0x4000;
        } else {
            ai->AItimer2 = 0;
        }
        if (ai->AItimer2 < 0x1E) {
            for (node = D_8005AA38; node != NULL; node = node->next) {
                ext = FindExtension((NAS *)node, 0x1000);
                if (ext != NULL && ext->unk1A == 2 && arg0 == (s16)ext->unk16) {
                    target = ai->TargetPlayer;
                    if (target != -1) {
                        obj = node->eng;
                        if (obj != NULL) {
                            x = FindExtension((NAS *)node, 0x800);
                            if (x != NULL) {
                                at = (Ovl61Pos *)&obj->unk10;
                                range = x->unk16;
                                if (func_800B7C60(target, at, &pos, range) != 0) {
                                    ai->AItimer2 = 0;
                                    ai->TargetPlayer = -1;
                                    ai->AIpad &= ~0x4000;
                                } else if ((target = D_8004E0D8 & 3) != arg0 && target != mate &&
                                           func_800B7C60(target, at, &pos, range) != 0) {
                                    ai->AItimer2 = 0;
                                    ai->TargetPlayer = -1;
                                    ai->AIpad &= ~0x4000;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    switch (ai->AImode) {
    case 3:
        k = D_8009D87C[arg0].unk0 * 0x10;
        sk = &D_8009D87C[arg0];
        k -= D_800C1F00;
        if (k <= 0) {
            k = 1;
        }
        spread = Random(k) + 0x100;
        k += spread;
        if (D_8005A648 == 0xF) {
            if (ai->AItimer2 == 0) {
                target = ai->TargetPlayer;
                flag = 0;
                if (target != -1) {
                    if (ai->AItimer1 == 0) {
                        if ((s16)Random(0x18) > sk->unk0) {
                            ai->AItimer1 = ((0x20 - sk->unk0) >> 1) + 0x40;
                            if (ai->AItimer1 == 0) {
                                ai->AItimer1 = 1;
                            }
                        } else {
                            flag = 1;
                        }
                    }
                    if (D_8009D908 == 0) {
                        flag = 1;
                    }
                    if (((s32 (*)(s32, s32, s32, s32, s32, s32))func_800B85DC)(arg0, target, k, -0x200, 0x200, 0) != 0) {
                        ai->TargetPlayer = -1;
                        ai->AIpad |= 0x8000;
                    } else {
                        if (ai->AItimer1 != 0) {
                            ai->AItimer1--;
                        }
                        if (func_800B8964(arg0) != 0) {
                            ai->AItimer2 = 0x5A;
                            ai->AIpad |= 0x4000;
                        } else if (((s32 (*)(s32, s32, s32, s32, s32, s32))func_800B85DC)(arg0, target, k, -0x200, 0x200, flag) != 0) {
                            ai->TargetPlayer = -1;
                            ai->AIpad |= 0x8000;
                        }
                    }
                } else {
                    if (ai->AItimer1 != 0) {
                        ai->AItimer1--;
                    }
                    if (func_800B8964(arg0) != 0) {
                        ai->AItimer2 = 0x5A;
                        ai->AIpad |= 0x4000;
                    }
                }
            }
        } else {
            target = ai->TargetPlayer;
            flag = 0;
            if (target != -1) {
                if (ai->AItimer1 == 0) {
                    if ((s16)Random(0x20) > sk->unk0) {
                        ai->AItimer1 = ((0x20 - sk->unk0) >> 1) + 0x20;
                        if (ai->AItimer1 == 0) {
                            ai->AItimer1 = 1;
                        }
                    } else {
                        flag = 1;
                    }
                }
                if (D_8009D908 == 0) {
                    flag = 1;
                }
                if (((s32 (*)(s32, s32, s32, s32, s32, s32))func_800B85DC)(arg0, target, k, -0x200, 0x200, flag) != 0) {
                    if ((D_8005A646 & 0x80) && (u16)D_800C1F0C == 0) {
                        ai->AIpad |= 0x2000;
                    } else {
                        ai->TargetPlayer = -1;
                        ai->AIpad |= 0x8000;
                    }
                } else if (ai->AItimer1 != 0) {
                    ai->AItimer1--;
                }
            }
        }
        if (dist != 0) {
            eng = Heroes[arg0].hero_eng;
            dx = eng->unk10.x - best.x;
            r = eng->unk10.z - best.z;
            if (arg0 == D_800C2724 || arg0 == D_800C2728) {
                v = abs(dx);
            } else {
                v = abs(r);
            }
            v += (D_8009D87C[arg0].unk0 + D_800C1F00) << 5;
            if (abs(v) > 0x280) {
                if (dist < v) {
                    ai->AIpad |= 0xF00;
                }
                ai->AIang = ang;
                ai->AIpad |= D_80051244[(ang >> 9) & 7];
            }
        } else {
            ai->AImode = 1;
        }
        break;
    case 4:
        if (dist != 0) {
            goto abort;
        }
        eng = Heroes[arg0].hero_eng;
        dx = eng->unk10.x - ai->TargetXYZ.x;
        row = eng->unk10.z - ai->TargetXYZ.z;
        d = csqrt(dx * dx + row * row) >> 6;
        if (d < 0x80) {
            ai->AIsleep = Random(0x3C) + 0x1E;
            ai->AImode = 5;
        } else {
            ai->AIang = ang;
            ai->AIpad |= D_80051244[(ang >> 9) & 7];
        }
        break;
    case 21:
        if (dist != 0) {
            goto abort;
        }
        if (Heroes[arg0].hero_mode != 0x1E) {
            ai->AImode = 1;
        }
        break;
    case 5:
        if (dist == 0) {
            if (ai->AIsleep == 0) {
                ai->AImode = 0x15;
                ai->AIpad |= 0x1000;
            }
            break;
        }
    abort:
        ai->AImode = 3;
        ai->AIaborted_mode = 0;
        break;
    case 20:
        break;
    default:
        ai->TargetXYZ = Heroes[arg0].hero_eng->unk10;
        if (arg0 == D_800C2724 || arg0 == D_800C2728) {
            ai->TargetXYZ.x = Random(0x400) - 0x200;
        } else {
            ai->TargetXYZ.z = Random(0x400) - 0x200;
        }
        ai->AImode = 4;
        break;
    }
    pad = &D_80051338[arg0];
    func_80013C94((u8 *)pad, ai->AIpad);
    *(s32 *)((u8 *)pad + 0x38) = ai->AIang;
}

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

/* The side (0-3) a player defends. */
static inline s32 ArkPlayerSide(s32 player) {
    if (player == D_800C2728) {
        return 0;
    }
    if (player == D_800C2724) {
        return 1;
    }
    if (player == D_800C2720) {
        return 3;
    }
    return (player == D_800C271C) * 2;
}

void PlotBall(NAS *nas) {
    EXTengine *eng;
    EXTobject *obj;
    EXTmotion *mot;
    EXTengine *e2;
    subengine *sub;
    Ovl61Trail *trail;
    s32 hide;
    s32 done;
    s16 k;
    s32 ang;
    s16 d;
    s32 dx;
    s32 dz;
    s32 y;
    s32 dist;
    s32 sc;
    s32 i;
    s32 idx;
    s32 yv;
    s16 shade;
    s16 pl;

    hide = 0;
    eng = nas->Engine;
    done = 0;
    if (eng == NULL) {
        return;
    }
    eng->OldXYZ = eng->unk10;
    obj = (EXTobject *)nas->FUX;
    if (obj == NULL) {
        return;
    }
    mot = FindExtension(nas, 0x800);
    if (mot != NULL) {
        if (mot->Ftime != 0) {
            mot->Ftime--;
        }
        if (mot->Xvel < mot->Xmax) {
            mot->Xvel += mot->Xacc;
            if (mot->Xvel > mot->Xmax) {
                mot->Xvel = mot->Xmax;
            }
        }
        if (mot->Xvel > mot->Xmax) {
            mot->Xvel -= mot->Xdec;
            if (mot->Xvel < mot->Xmax) {
                mot->Xvel = mot->Xmax;
            }
        }
        eng->unk10.x -= (mot->Xvel * D_80068BD4[(mot->Xa - 0x400) & 0xFFF].unk2) >> 12;
        eng->unk10.z -= (mot->Xvel * D_80068BD4[mot->Xa & 0xFFF].unk2) >> 12;
        if (obj->timer != 0) {
            obj->timer--;
        }
        switch (obj->status) {
        case 0:
            hide = 1;
            if ((((Ovl61BallView *)D_8005A614)->unk10 & 0x04000000) ||
                (((Ovl61BallView *)D_8005A614)->unk30 & 0x1000)) {
                mot->Xvel = 0;
                mot->Xacc = 0;
                mot->Xdec = 0;
                eng->unk10.x = 0x2000;
                eng->unk10.y = 0x2000;
                eng->unk10.z = 0x2000;
                eng->flags &= ~0x8000;
                break;
            }
            if ((u16)D_800C1F16 < 4) {
                idx = D_800C1214[D_800C1F16][Random(2)];
            } else {
                idx = Random(4);
            }
            if (D_800C1CE8[idx].unk12 == 0) {
                D_800C1F16 = -1;
                eng->unk10 = ((SLONG_XYZ *)D_800C1A84)[idx];
                D_800C1CE8[idx].unk12 = 0x1F;
                obj->countdown = 0xF;
                obj->status = 12;
            }
            break;
        case 12:
            hide = 1;
            if (obj->countdown != 0) {
                obj->countdown--;
                break;
            }
            e2 = eng->Engine;
            eng->flags |= 0x8000;
            if (e2 != NULL) {
                e2->flags |= 0x8000;
            }
            ang = get_angle(eng->unk10.x, eng->unk10.z);
            mot->Xa = ang;
            mot->Za = ang;
            ang = Random(0x1C7);
            mot->Xacc = 4;
            mot->Zvel = 0;
            mot->Xdec = hide;
            mot->Za = (mot->Za + ((u16)ang - 0xE3)) & 0xFFF;
            obj->timer = 0x14;
            PlaySfx(0x5A0, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
            switch (((Ovl61BallView *)D_8005A614)->unk34) {
            case 17:
                mot->Yvel = 0x40;
                mot->Ymax = 0x50;
                mot->Yacc = 6;
                mot->Ydec = 0;
                mot->Ydir = 0;
                mot->Xvel = 0x50;
                obj->status = 6;
                mot->Xmax = 0x50;
                break;
            case 16:
                mot->Ymax = 0x60;
                mot->Yacc = 3;
                mot->Xvel = 0x20;
                mot->Yvel = 0x10;
                mot->Ydec = 0;
                mot->Ydir = 0;
                obj->status = 6;
                mot->Xmax = 0x50;
                break;
            case 15:
                eng->unk10.y = D_8005A5D8 - 0x100;
                mot->Ymax = 0x60;
                mot->Ydir = 0xFF;
                mot->Yvel = 0x30;
                mot->Yacc = 0;
                mot->Ydec = 6;
                mot->Xvel = 0x20;
                obj->status = 6;
                mot->Xmax = 0x30;
                break;
            default:
                eng->unk10.y = D_8005A5D8 - 0x100;
                mot->Yvel = 0x30;
                mot->Ymax = 0x60;
                mot->Ydir = 0xFF;
                mot->Yacc = 0;
                mot->Ydec = 6;
                mot->Xvel = 0x20;
                obj->status = 6;
                mot->Xmax = 0x50;
                break;
            }
            obj->player = -1;
            obj->owner = -1;
            break;
        case 6:
            if (((s32 (*)(EXTengine *, EXTobject *))D_800C1C94)(eng, obj) != 0) {
                hide = 1;
            } else {
                mot->Xa = mot->Za;
                mot->Za = 0;
                obj->status = 8;
            }
        case 8:
            mot->Yvel += mot->Yacc - mot->Ydec;
            if (mot->Yvel < 0) {
                mot->Yvel = 0;
            }
            if (mot->Yvel > mot->Ymax) {
                mot->Yvel = mot->Ymax;
            }
            if (mot->Ydir != 0) {
                eng->unk10.y -= mot->Yvel;
            } else {
                eng->unk10.y += mot->Yvel;
            }
            if (mot->Yvel == 0) {
                mot->Yacc = mot->Ydec;
                mot->Ydec = 0;
                mot->Ydir = 0;
            }
            if (mot->Ydir != 0) {
                break;
            }
            eng->unk10.y += 0x60;
            if (func_80025BBC(eng) != 0) {
                obj->can_do |= 0x400;
                if (nas->UserInfo.flags == 0) {
                    PlaySfx(D_800C1378[Random(4)], &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
                    nas->UserInfo.flags |= 1;
                }
                if (((Ovl61BallView *)D_8005A614)->unk34 == 0x10) {
                    mot->Yvel = mot->Yvel >> 2;
                } else {
                    mot->Yvel = mot->Yvel >> 1;
                }
                mot->Ydec = mot->Yacc;
                mot->Yacc = 0;
                mot->Ydir = 0xFF;
                if (mot->Yvel < 8) {
                    obj->status = 1;
                    obj->can_do |= 0x400;
                }
            }
            if (obj->can_do & 0x400) {
                d = ((s16 (*)(EXTengine *, s32))D_800C1C8C)(eng, mot->Xa);
                if (d != -1) {
                    k = func_800154A0(((u16)mot->Xa + 0x800) & 0xFFF, d);
                    if (abs(k) < 8) {
                        k = Random(0x10) + 8;
                    }
                    mot->Xa = (d + k) & 0xFFF;
                    mot->Zmax = 0;
                    PlaySfx(D_800C1388[Random(6)], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (((s32 (*)(EXTengine *, EXTobject *, s32))D_800C1C90)(eng, obj, 0) != 0) {
                    obj->countdown = 4;
                    switch (((Ovl61BallView *)D_8005A614)->unk34) {
                    case 17:
                        obj->status = 0x63;
                        break;
                    case 15:
                    case 16:
                        obj->status = 4;
                        mot->Yvel = 0x80;
                        mot->Ymax = 0x60;
                        mot->Yacc = 8;
                        mot->Ydec = 0;
                        break;
                    default:
                        obj->status = 0x11;
                        break;
                    }
                    i = ArkPlayerSide(obj->player);
                    BLINK_D38[i].unk12 = 4;
                    BLINK_D38[i].unk10 = 0x20;
                    break;
                }
            }
            eng->unk10.y -= 0x60;
            break;
        case 3:
            if (((s32 (*)(EXTengine *, EXTobject *))D_800C1C94)(eng, obj) != 0) {
                hide = 1;
            } else {
                mot->Xa = mot->Za;
                mot->Za = 0;
                PlaySfx(D_800C1378[Random(4)], &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
                obj->status = 1;
            }
            eng->unk10.y = eng->unk10.y - ((s32 (*)(EXTengine *))func_80025F44)(eng) - 0x60;
            break;
        case 1:
            d = ((s16 (*)(EXTengine *, s32))D_800C1C8C)(eng, mot->Xa);
            if (d != -1) {
                k = func_800154A0(((u16)mot->Xa + 0x800) & 0xFFF, d);
                if (abs(k) < 8) {
                    k = Random(0x10) + 8;
                }
                mot->Xa = (d + k) & 0xFFF;
                mot->Zmax = 0;
                PlaySfx(D_800C1388[Random(6)], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
            if (((s32 (*)(EXTengine *, EXTobject *, s32))D_800C1C90)(eng, obj, 0) != 0) {
                obj->countdown = 8;
                switch (((Ovl61BallView *)D_8005A614)->unk34) {
                case 17:
                    obj->countdown = 0x10;
                    obj->status = 0x63;
                    break;
                case 15:
                case 16:
                    obj->status = 4;
                    mot->Yvel = 0x80;
                    mot->Ymax = 0x60;
                    mot->Yacc = 8;
                    mot->Ydec = 0;
                    break;
                default:
                    obj->status = 0x11;
                    break;
                }
                i = ArkPlayerSide(obj->player);
                BLINK_D38[i].unk12 = 4;
                BLINK_D38[i].unk10 = 0x20;
                break;
            }
            CalcArkInfluence(mot);
            eng->unk10.y = eng->unk10.y - ((s32 (*)(EXTengine *))func_80025F44)(eng) - 0x60;
            break;
        case 0x63:
            if (obj->countdown != 0) {
                obj->countdown--;
            } else {
                done = 1;
            }
            if ((((s32 (*)(EXTengine *, EXTobject *, s32))D_800C1C90)(eng, obj, 0) ^ 1) != 0) {
                obj->status = 1;
                break;
            }
            mot->Xa = get_angle(-eng->unk10.x, -eng->unk10.z);
            if (((s32 (*)(EXTengine *, EXTobject *, s32))D_800C1C90)(eng, obj, 1) != 0 || done != 0) {
                obj->status = 4;
                mot->Yvel = 0x20;
                mot->Ymax = 0x60;
                mot->Yacc = 6;
                mot->Ydec = 0;
            }
            eng->unk10.y = eng->unk10.y - ((s32 (*)(EXTengine *))func_80025F44)(eng) - 0x60;
            break;
        case 0x11:
            if (obj->countdown != 0) {
                if (--obj->countdown == 0) {
                    eng->flags |= 0x40000021;
                    eng->sub->flags |= 0x40000021;
                    eng->sub->unk76 = 0x400;
                    e2 = eng->Engine;
                    if (e2 != NULL) {
                        e2->flags |= 0x40000000;
                        e2->sub->flags |= 0x40000000;
                        e2->sub->unk76 = 0x400;
                    }
                }
            } else {
                eng->sub->unk76 += 0x200;
                e2 = eng->Engine;
                hide = 1;
                if (e2 != NULL) {
                    e2->sub->unk76 += 0x200;
                    if ((s16)e2->sub->unk76 >= 0x1000) {
                        e2->sub->unk76 = 0xFFF;
                        done = hide;
                    }
                }
            }
            if (((s32 (*)(EXTengine *, EXTobject *, s32))D_800C1C90)(eng, obj, 1) != 0 || done != 0) {
                obj->status = 0x12;
            }
            eng->unk10.y = eng->unk10.y - ((s32 (*)(EXTengine *))func_80025F44)(eng) - 0x60;
            break;
        case 4:
            if (obj->countdown != 0) {
                if (--obj->countdown == 0) {
                    eng->flags |= 0x40000021;
                    eng->sub->flags |= 0x40000021;
                    eng->sub->unk76 = 0x400;
                    eng->sub->unk68 = 0x100;
                    e2 = eng->Engine;
                    if (e2 != NULL) {
                        e2->flags |= 0x40000000;
                        e2->sub->flags |= 0x40000000;
                        e2->sub->unk76 = 0x400;
                    }
                }
            } else {
                eng->sub->unk76 += 0x200;
                e2 = eng->Engine;
                hide = 1;
                if (e2 != NULL) {
                    e2->sub->unk76 += 0x200;
                    if ((s16)e2->sub->unk76 >= 0x1000) {
                        e2->sub->unk76 = 0xFFF;
                        done = hide;
                    }
                }
            }
            if (((Ovl61BallView *)D_8005A614)->unk34 != 17) {
                mot->Xvel >>= 1;
                mot->Xmax >>= 1;
            }
            mot->Yvel += mot->Yacc - mot->Ydec;
            if (mot->Yvel < 0) {
                mot->Yvel = 0;
            }
            if (mot->Yvel > mot->Ymax) {
                mot->Yvel = mot->Ymax;
            }
            yv = mot->Yvel;
            eng->unk10.y += yv;
            if (((s32 (*)(EXTengine *, EXTobject *, s32, s32))D_800C1C90)(eng, obj, 1, yv) != 0 ||
                done != 0) {
                obj->status = 0x12;
            }
            break;
        case 2:
            obj->can_do |= 0x80;
            if ((u16)obj->player >= 4) {
                mot->Xvel = 0x30;
                mot->Xmax = 0x30;
                obj->status = 1;
                break;
            }
            AlignGrabBall(eng, mot, obj);
            break;
        case 0x12:
            if (obj->whatami != 2) {
                obj->can_do &= 0xFF7F;
                pl = obj->player;
                if (!(Heroes[pl].hero_status & 0xFA00)) {
                    PlaySfx(D_800C13A4[pl], NULL, NULL, 0x1000, 0x1E00, 0);
                    pl = obj->player;
                    if (pl != -1) {
                        D_800C1F16 = ArkPlayerSide(pl);
                    }
                    if ((((Ovl61BallView *)D_8005A614)->unk4 & 0x80000001) != 0x80000001) {
                        if ((((Ovl61BallView *)D_8005A614)->unk32 & 0x40) &&
                            ((Ovl61BallView *)D_8005A614)->unk6C != 0) {
                            if (((Ovl61BallView *)D_8005A614)->unk5C[obj->player] != 0) {
                                obj->player = ((Ovl61BallView *)D_8005A614)->unk4A;
                            } else {
                                obj->player = ((Ovl61BallView *)D_8005A614)->unk48;
                            }
                            if (Heroes[obj->player].hero_score != 0) {
                                Heroes[obj->player].hero_score--;
                                Heroes[obj->player].hero_score_displayed = Heroes[obj->player].hero_score;
                            }
                        } else if (Heroes[obj->player].hero_score != 0) {
                            Heroes[obj->player].hero_score--;
                            Heroes[obj->player].hero_health--;
                            Heroes[obj->player].hero_score_displayed = Heroes[obj->player].hero_score;
                        }
                        if ((((Ovl61BallView *)D_8005A614)->unk4 & 0x80000002) != 0x80000002 &&
                            Heroes[obj->player].hero_score == 0) {
                            if (obj->player == D_800C1C80) {
                                D_800C1C80 = -1;
                            }
                            Heroes[obj->player].hero_status |= 0x2000;
                            if (((Ovl61BallView *)D_8005A614)->unk32 & 0x40) {
                                pl = Heroes[obj->player].hero_teammate;
                                if (pl != -1) {
                                    Heroes[pl].hero_status |= 0x2000;
                                }
                            }
                        }
                    }
                }
            }
            KillNAS(nas);
            return;
        }
        mot->Zvel = 0;
    }
    dx = eng->unk10.x - eng->OldXYZ.x;
    dz = eng->unk10.z - eng->OldXYZ.z;
    eng->unk1E += (((get_angle(dx, dz) & 0xFFF) - eng->unk1E) << 20) >> 21;
    dist = csqrt(dx * dx + dz * dz) >> 5;
    eng->unk5E = 1;
    dx = ((eng->unk10.x + 0x800) >> 9) + 1;
    dz = ((eng->unk10.z + 0x800) >> 9) + 1;
    eng->unk1C -= dist;
    if (dx >= 10) {
        dx = 9;
    }
    if (dx < 0) {
        dx = 0;
    }
    if (dz >= 10) {
        dz = 9;
    }
    if (dz < 0) {
        dz = 0;
    }
    shade = D_800C1224[9 - dz][dx];
    switch (obj->status) {
    case 4:
    case 0x12:
        shade = 0x200;
        break;
    case 0x63:
    case 2:
        break;
    default:
        func_800BF640(eng, (Ovl61FuxF *)obj);
        break;
    }
    eng->sub->unk68 = shade;
    e2 = eng->Engine;
    if (e2 == NULL) {
        return;
    }
    e2->unk10 = eng->unk10;
    e2->sub->unk68 = shade;
    /* D_8005AA70 is the level's LevSpec: [9] is GetContour */
    y = eng->unk10.y - ((s32 (*)(EXTengine *))D_8005AA70[9])(eng);
    e2 = e2->Engine;
    if (e2 == NULL) {
        return;
    }
    e2->unk10 = eng->unk10;
    e2->unk10.y = y;
    e2->sub->unk68 = shade;
    sub = e2->sub;
    if (hide != 0) {
        e2->flags &= ~0x8000;
    } else {
        e2->flags |= 0x8000;
    }
    sub->unk08 = y - eng->unk10.y;
    trail = FindExtension(nas, 0x10000000);
    if (trail == NULL) {
        return;
    }
    k = 0;
    sub = sub->next;
    sc = 0x1000;
    while (sub != NULL) {
        if (trail->on[k] != 0) {
            sub->flags |= 0x8000;
        } else {
            sub->flags &= ~0x8000;
        }
        sub->unk04 = trail->pos[k].x - e2->unk10.x;
        sub->unk08 = trail->pos[k].y - (e2->unk10.y + e2->sub->unk08);
        sub->unk0C = trail->pos[k].z - e2->unk10.z;
        sub->unk20.z = sc;
        sub->unk20.x = sc;
        sc -= 0x180;
        sub->unk52 = 1;
        if (sc < 0x100) {
            sc = 0x100;
        }
        sub->unk76 = 0x1000 - (sc >> 1);
        sub = sub->next;
        k++;
    }
    for (k = 7; k > 0; k--) {
        trail->pos[k].x = trail->pos[k - 1].x;
        trail->pos[k].y = trail->pos[k - 1].y;
        trail->pos[k].z = trail->pos[k - 1].z;
        trail->on[k] = trail->on[k - 1];
    }
    if (hide == 0 && (nas->UserInfo.flags & 1)) {
        trail->on[0] = 1;
        trail->pos[0] = e2->unk10;
    }
}

void CalcArkInfluence(EXTmotion *m) {
    s32 sx;
    s32 sz;
    s32 ax;
    s32 az;
    s32 speed;
    s32 angle;
    s32 t;
    s32 x1;
    s32 z1;
    s16 vel;
    s32 scale;

    if (D_8005A648 == 0x11) {
        sx = abs((D_80068BD4[(D_800C1C68 - 0x400) & 0xFFF].unk2 * 5) >> 3);
        sz = abs((D_80068BD4[(D_800C1C6C - 0x400) & 0xFFF].unk2 * 5) >> 3);
        az = 0;
        ax = (D_800C1C68 < 0) ? 0x800 : 0;
        if (D_800C1C68 > 0) {
            ax = 0;
        }
        if (D_800C1C6C < 0) {
            az = 0x400;
        }
        if (D_800C1C6C > 0) {
            az = 0xC00;
        }
        scale = (s16)sx;
        x1 = (scale * D_80068BD4[(ax - 0x400) & 0xFFF].unk2) >> 12;
        z1 = (scale * D_80068BD4[ax].unk2) >> 12;
        scale = (s16)sz;
        t = (scale * D_80068BD4[(az - 0x400) & 0xFFF].unk2) >> 12;
        x1 += t;
        t = (scale * D_80068BD4[az].unk2) >> 12;
        angle = get_angle(x1, z1 + t);
        m->Zvel = (sx + sz) >> 6;
        m->Za = angle;
    }
    speed = m->Zvel;
    if (speed != 0) {
        vel = m->Xvel;
        x1 = (vel * D_80068BD4[((u16)m->Xa - 0x400) & 0xFFF].unk2) >> 12;
        z1 = (vel * D_80068BD4[(u16)m->Xa & 0xFFF].unk2) >> 12;
        t = (speed * D_80068BD4[((u16)m->Za - 0x400) & 0xFFF].unk2) >> 12;
        x1 += t;
        t = (speed * D_80068BD4[(u16)m->Za & 0xFFF].unk2) >> 12;
        m->Xa = get_angle(x1, z1 + t) & 0xFFF;
    }
}

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

void Plot_N_Gin(NAS *arg0) {
    EXTengine *eng;
    Ovl61Gin *gin;
    s32 i;
    u32 s;

    eng = arg0->Engine;
    if (eng == NULL) {
        return;
    }
    if (FindExtension(arg0, 0x400) != NULL && (gin = D_800C1B2C) != NULL) {
        if (D_8005A644 & 0x801) {
            if ((u16)gin->unk1A != 0) {
                gin->unk1A = 5;
                if (eng->sfx != NULL) {
                    StopSfx(eng->sfx);
                }
            }
            func_800250B0(arg0);
        }
        s = (u16)gin->unk1A;
        switch (s) {
        case 1:
            eng->unk10.y = D_8005A5D8;
            gin->unk1A = 2;
            gin->unk22 = 0xA;
            gin->unk1C |= 0x90;
            PlaySfx(0x5BE, NULL, NULL, 0x1000, 0x1E00, 0);
            func_8007A524(0, &eng->unk10.x, (s32)D_80054574);
            eng->sub->unk74 = 0x2001;
            break;
        case 2:
            eng->unk1E += 0x20;
            if (gin->unk22 != 0) {
                gin->unk22--;
            } else {
                arg0->Class |= 0x200;
                eng->flags |= 0x8000;
                gin->unk1A = 3;
                PlaySfx(0x5BF, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
                gin->unk22 = 0x5A;
                gin->unk20 = 0x10;
            }
            func_800250B0(arg0);
            eng->unk5E = 1;
            return;
        case 3:
            eng->unk1E += gin->unk20;
            gin->unk20 += 4;
            func_800250B0(arg0);
            if (gin->unk22 != 0) {
                gin->unk22--;
                break;
            }
            eng->sub->unk74 = 0x2006;
            gin->unk1A = 4;
            gin->unk22 = 0x96;
            gin->unk20 = Random(0xA) + 0xA;
            if (((Ovl61GinView *)D_8005A614)->unk32 == 0) {
                eng->unk5E = 1;
                return;
            }
            if (((Ovl61GinView *)D_8005A614)->unk6C != 0) {
                gin->unk22 = 0xD2;
            }
            break;
        case 4:
            eng->unk1E += 0x1C7;
            if (((Ovl61GinView *)D_8005A614)->unk32 & 0x80) {
                if (gin->unk20 != 0) {
                    gin->unk20--;
                } else {
                    for (i = 0; i < 4; i++) {
                        if ((i == 0 || (((Ovl61GinView *)D_8005A614)->unk6C != 0 && i == 1)) && !(Heroes[i].hero_status & 0xFA00)) {
                            if (func_80015480(eng->unk1E, Heroes[i].hero_move->facing_angle & 0xFFF) < 0xE3) {
                                func_800B99E0();
                                gin->unk20 = Random(0xB) + 6;
                            } else {
                                gin->unk20 = Random(5);
                            }
                        }
                    }
                }
            } else {
                if (gin->unk20 != 0) {
                    gin->unk20--;
                } else {
                    func_800B99E0();
                    gin->unk20 = 0xF;
                }
            }
            if (gin->unk22 != 0) {
                gin->unk22--;
            } else {
                gin->unk1A = 5;
            }
            break;
        case 5:
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x5C0, NULL, NULL, 0x1000, 0x1E00, 0);
            func_8007A524(0, &eng->unk10.x, (s32)D_80054574);
            gin->unk1C = 0;
            eng->unk10.y = -0x1000;
            eng->flags &= 0xFFFF7FFF;
            D_800C1EFC = 0x19;
            arg0->Class &= 0xFDFF;
            gin->unk1A = 0;
            gin->unk22 = (Random(0xF) + 8) * 0x1E;
            break;
        case 0:
        default:
            gin->unk1C = 0;
            eng->unk10.y = -0x1000;
            arg0->Class &= 0xFDFF;
            eng->flags &= 0xFFFF7FFF;
            if (((Ovl61ModeView *)D_8005A614)->unk30 & 0x801) {
                eng->unk5E = 1;
                return;
            }
            if (gin->unk22 != 0) {
                gin->unk22--;
            } else {
                gin->unk1A = 1;
            }
        }
    }
end:
    eng->unk5E = 1;
}

void func_800BC0D8(void) {
    Ovl61GameView *g;
    Ovl61Node *n;
    NAS *nas;
    s32 gin;
    s32 cnt;
    s32 i;
    s32 lim;
    s32 x;
    s32 y;
    s32 hero;
    s32 total;

    g = (Ovl61GameView *)D_8005A614;
    if (g->flags & 0x801) {
        return;
    }
    gin = 0;
    if (D_800C1B2C != NULL) {
        gin = (u16)D_800C1B2C->unk1A != 0;
    }
    if (g->mode == 0x10 && D_8009E25C.n == 0) {
        if (D_800C1F0A != 0) {
            D_800C1F0A--;
        } else {
            for (i = cnt = 0; i < 4; i++) {
                if (Heroes[i].hero_pu != 0) {
                    cnt++;
                }
            }
            if (cnt == 0) {
                func_800BD770();
                D_800C1F0A = Random(0x5A) + 0x1E;
            }
        }
    }
    if (D_8005A648 == 0xE && Random(0xF) == 0) {
        x = Random(0x1800) - 0xC00;
        y = -(Random(0x200) + 0x80);
        SpawnNASById(x, y, Random(0x800) + 0x1000, 0x1504, 0x11);
    }
    if (D_800C1EFC != 0) {
        D_800C1EFC--;
    } else {
        D_800C1EF8++;
        lim = (u16)(D_800C1EF8 / 300);
        lim++;
        if (D_8005A648 == 0xF) {
            if (lim >= 5) {
                lim = 4;
            }
        } else if (lim >= 6) {
            lim = 5;
        }
        if (gin) {
            lim = 0;
        }
        n = D_8005AA38;
        D_800C1EFA = 0;
        while (n != NULL) {
            n = n->next;
            D_800C1EFA++;
        }
        if (D_800C1EFA < lim) {
            total = 0;
            for (n = D_8005AA38; n != NULL; n = n->next) {
                total++;
            }
            if (total < 9) {
                nas = SpawnNASById(0x1000, 0x1000, 0x1000, 0x1501, 0x11);
                if (nas != NULL && nas->Engine != NULL && nas->FUX != NULL) {
                    ((Ovl61Fux *)nas->FUX)->flag = 0;
                }
            }
            D_800C1EFC = 0x1E;
        }
    }
    D_800C1F00 = D_800C1328[D_800C1EFA];
    if (O61GV->mode == 0xF && (O61GV->pad_buttons & 0x80)) {
        if (D_800C1C70 != 0) {
            D_800C1C70--;
        } else {
            if (++D_800C1C74 >= 0x12) {
                D_800C1C74 = 0;
            }
            if (O61GV->unk6C != 0) {
                hero = D_800C1A20[D_800C1C74];
            } else {
                hero = D_800C19FC[D_800C1C74];
            }
            if (!(Heroes[hero].hero_status & 0xFA00)) {
                SpawnNASById(hero, 0, 0, 0x1509, 0x11);
            }
            D_800C1C70 = Random(6) * 0x1E + 0x5A;
        }
    }
    if (O61GV->mode == 0x11) {
        if (D_800C1F0C != 0) {
            D_800C1F0C--;
        }
        if (D_800C1C78 != 0) {
            D_800C1C78--;
        } else {
            if (++D_800C1C7C >= 0x12) {
                D_800C1C7C = 0;
            }
            if (O61GV->unk6C != 0) {
                i = D_800C1A20[D_800C1C7C];
            } else {
                i = D_800C19FC[D_800C1C7C];
            }
            func_800BD43C((u16)i);
            D_800C1C78 = Random(9) * 0x1E + 0x10E;
        }
    }
}

#define LINK(x) ((Ovl61Link *)(x))

void func_800BC564(void) {
    s32 i;
    Ovl61ScnObj *obj;
    s16 *p;
    s32 t;
    s32 t60;
    u32 k;

    for (i = 9; i >= 0; i--) {
        D_800C1C98[i] = 0;
    }
    for (i = 9; i >= 0; i--) {
        D_800C1CC0[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        D_800C1CE8[i].a = 0;
        D_800C1CE8[i].unkA = 0;
        D_800C1CE8[i].unk12 = 0;
        D_800C1CE8[i].unk10 = 0;
        LINK(D_800C1E28)[i].a = 0;
        LINK(D_800C1E28)[i].b = 0;
        LINK(D_800C1E28)[i].unkE = 0;
        LINK(D_800C1E28)[i].unk8 = 0;
        LINK(D_800C1E28)[i].unkE = 0;
        LINK(D_800C1E28)[i].unk8 = 0;
        LINK(D_800C1E28)[i].unk12 = 0;
        LINK(D_800C1D38)[i].a = 0;
        LINK(D_800C1D38)[i].b = 0;
        LINK(D_800C1D38)[i].unkE = 0;
        LINK(D_800C1D38)[i].unk8 = 0;
        LINK(D_800C1D38)[i].unkE = 0;
        LINK(D_800C1D38)[i].unk8 = 0;
        LINK(D_800C1D38)[i].unk12 = 0;
        LINK(D_800C1D88)[i].a = 0;
        LINK(D_800C1D88)[i].b = 0;
        LINK(D_800C1D88)[i].unkE = 0;
        LINK(D_800C1D88)[i].unk8 = 0;
        LINK(D_800C1D88)[i].unkE = 0;
        LINK(D_800C1D88)[i].unk8 = 0;
        LINK(D_800C1D88)[i].unk12 = 0;
        LINK(D_800C1DD8)[i].a = 0;
        LINK(D_800C1DD8)[i].b = 0;
        LINK(D_800C1DD8)[i].unkE = 0;
        LINK(D_800C1DD8)[i].unk8 = 0;
        LINK(D_800C1DD8)[i].unkE = 0;
        LINK(D_800C1DD8)[i].unk8 = 0;
        LINK(D_800C1DD8)[i].unk12 = 0;
        D_800C1E78[i].pad0 = 0;
        D_800C1E78[i].f4 = 0;
        D_800C1E78[i].f8 = 0x60;
        D_800C1E78[i].fC = 0;
        D_800C1E78[i].f10 = 0;
        D_800C1E78[i].f14 = 0;
        D_800C1E78[i].f18 = 0;
        D_800C1E78[i].f1A = 0;
    }
    for (obj = (Ovl61ScnObj *)((GfxScene *)D_80058AD0)->objs; obj != 0; obj = obj->next) {
        p = (s16 *)func_80015870(obj->unk6C, obj->unk74);
        k = *p;
        if (k - 1 < 9) {
            D_800C1C98[k] = obj;
        }
        k = *p - 10;
        if (k < 4) {
            D_800C1CE8[k].a = obj;
            D_800C1CE8[k].unkA = obj->unk74;
            obj->unk68 = 0x44;
            obj->flags |= 0x2000000;
        }
        k = *p - 20;
        if (k < 4) {
            LINK(D_800C1E28)[k].a = obj;
            LINK(D_800C1E28)[k].unk8 = obj->unk74;
            switch (((Ovl61ModeView *)D_8005A614)->unk34) {
            case 14:
                LINK(D_800C1E28)[k].unkA = D_800C1410[k];
                break;
            case 16:
                LINK(D_800C1E28)[k].unkA = D_800C1430[k];
                break;
            }
        }
        t = *p;
        k = t - 30;
        if (k < 4) {
            LINK(D_800C1E28)[k].b = obj;
            LINK(D_800C1E28)[k].unkC = obj->unk74;
            switch (((Ovl61ModeView *)D_8005A614)->unk34) {
            case 14:
                LINK(D_800C1E28)[k].unkE = D_800C1410[t - 26];
                break;
            case 16:
                LINK(D_800C1E28)[k].unkE = D_800C1430[t - 26];
                break;
            }
        }
        k = *p - 50;
        if (k < 4) {
            LINK(D_800C1D38)[k].a = obj;
            LINK(D_800C1D38)[k].unk8 = obj->unk74;
            LINK(D_800C1D38)[k].unkA = D_800C1420[k];
        }
        t60 = *p;
        k = t60 - 60;
        if (k < 4) {
            LINK(D_800C1D38)[k].b = obj;
            LINK(D_800C1D38)[k].unkC = obj->unk74;
            LINK(D_800C1D38)[k].unkE = D_800C1420[t60 - 56];
        }
        k = *p - 80;
        if (k < 4) {
            LINK(D_800C1DD8)[k].a = obj;
            LINK(D_800C1DD8)[k].unkA = obj->unk74;
            LINK(D_800C1DD8)[k].unkE = D_800C1464[k][0];
        }
        k = *p - 90;
        if (k < 4) {
            LINK(D_800C1D88)[k].a = obj;
            LINK(D_800C1D88)[k].unkA = obj->unk74;
            obj->flags &= ~0x8000;
        }
        k = *p - 100;
        if (k < 10) {
            D_800C1CC0[k] = obj;
        }
    }
}

void func_800BCA30(void)
{
    s32 a;
    s32 i;
    s32 j;
    Ovl61ScnObj *o;
    Ovl61ScnObj *p;
    Ovl61Spr **sp;
    Ovl61Spr *s;

    a = (D_8004E0D8 << 5) & 0xFFF;
    if (D_8005A648 == 0x11) {
        D_800C1C6C = ((D_80068BD4[(a * 2 - 0x400) & 0xFFF].unk2 + D_80068BD4[a].unk2) * 11) >> 12;
        D_800C1C68 = (D_80068BD4[(a - 0x400) & 0xFFF].unk2 * 5) >> 10;
        D_8005A5D8 = (D_80068BD4[a].unk2 * 3) >> 7;
    }
    i = 0;
    do {
        if (D_800C1E78[i].pad0 != 0) {
            a = get_angle(0xA00, D_800C1E78[i].pad0 >> 4) - 0x400;
            switch (i) {
            case 1:
                D_800C1C6C += a;
                D_800C1C68 += a;
                break;
            case 2:
                D_800C1C6C -= a;
                D_800C1C68 += a;
                break;
            case 3:
                D_800C1C6C -= a;
                D_800C1C68 -= a;
                break;
            default:
                D_800C1C6C += a;
                D_800C1C68 -= a;
                break;
            }
        }
        i++;
    } while (i < 4);

    for (j = 0; j < 4; j++) {
        if (D_800C1CE8[j].unk12 != 0) {
            D_800C1CE8[j].unk12--;
        }
        if (BLINK_E28[j].unk12 != 0) {
            BLINK_E28[j].unk12--;
        }
        if (BLINK_D38[j].unk12 != 0) {
            BLINK_D38[j].unk12--;
        }
        if ((p = D_800C1CE8[j].a) != NULL && ((Ovl61ModeView *)D_8005A614)->unk34 == 14) {
            if (D_800C1CE8[j].unk12 != 0) {
                D_800C1CE8[j].unk10++;
                p->unk74 = D_800C1CE8[j].unkA + (u16)(((D_800C1CE8[j].unk10 >> 2) & 3) + 1);
            } else {
                p->unk74 = D_800C1CE8[j].unkA;
                D_800C1CE8[j].unk10 = 0;
            }
        }
        if ((p = BLINK_E28[j].a) != NULL) {
            switch (((Ovl61ModeView *)D_8005A614)->unk34) {
            case 14:
                if (BLINK_E28[j].unk12 != 0) {
                    p->unk74 = BLINK_E28[j].unkA;
                    if (BLINK_E28[j].b != NULL) {
                        BLINK_E28[j].b->unk74 = BLINK_E28[j].unkE;
                    }
                } else {
                    p->unk74 = BLINK_E28[j].unk8;
                    if (BLINK_E28[j].b != NULL) {
                        BLINK_E28[j].b->unk74 = BLINK_E28[j].unkC;
                    }
                }
                break;
            case 16:
                if (BLINK_E28[j].unk10 == 1) {
                    p->unk74 = BLINK_E28[j].unkA;
                    BLINK_E28[j].b->unk74 = BLINK_E28[j].unkE;
                } else {
                    p->unk74 = BLINK_E28[j].unk8;
                    BLINK_E28[j].b->unk74 = BLINK_E28[j].unkC;
                }
                break;
            }
        }
        if ((o = BLINK_D38[j].a) != NULL && BLINK_D38[j].unk12 == 0) {
            if (BLINK_D38[j].unk10 != 0 && (--BLINK_D38[j].unk10 & 4)) {
                o->unk74 = BLINK_D38[j].unkA;
            } else {
                BLINK_D38[j].a->unk74 = BLINK_D38[j].unk8;
            }
        }
    }

    if (D_8005A648 == 0x11) {
        if (D_800C1C88 == 0) {
            D_800C1F14 = 0;
        }
        for (i = 1; i < 10; i++) {
            sp = (Ovl61Spr **)&D_800C1C98[i];
            if (*sp != NULL) {
                (*sp)->unk51 = 2;
                (*sp)->unk10 = D_800C1C68;
                (*sp)->unk14 = D_800C1C6C;
                (*sp)->unk8 = D_8005A5D8;
                (*sp)->unk52 = 1;
            }
        }
        for (i = 0; i < 10; i++) {
            if (D_800C1CC0[i] != NULL) {
                ((Ovl61Rise *)D_800C1CC0[i])->unk4 += D_800C1348[i].step;
                if (D_800C1348[i].max < ((Ovl61Rise *)D_800C1CC0[i])->unk4) {
                    ((Ovl61Rise *)D_800C1CC0[i])->unk4 = D_800C1348[i].reset;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            s = (Ovl61Spr *)D_800C1D88[i].flags;
            if (s != NULL) {
                if (D_800C1C88 == 0) {
                    s->flags &= ~0x8000;
                }
                ((Ovl61Spr *)D_800C1D88[i].flags)->unk51 = 2;
                ((Ovl61Spr *)D_800C1D88[i].flags)->unk10 = D_800C1C68;
                ((Ovl61Spr *)D_800C1D88[i].flags)->unk14 = D_800C1C6C;
                ((Ovl61Spr *)D_800C1D88[i].flags)->unk8 = D_8005A5D8;
                ((Ovl61Spr *)D_800C1D88[i].flags)->unk52 = 1;
            }
            if (D_800C1DD8[i].sub != NULL) {
                ((Ovl61Spr *)D_800C1DD8[i].sub)->unk51 = 2;
                ((Ovl61Spr *)D_800C1DD8[i].sub)->unk10 = D_800C1C68;
                ((Ovl61Spr *)D_800C1DD8[i].sub)->unk14 = D_800C1C6C;
                ((Ovl61Spr *)D_800C1DD8[i].sub)->unk8 = D_8005A5D8;
                ((Ovl61Spr *)D_800C1DD8[i].sub)->unk52 = 1;
            }
        }
    }
}

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

void func_800BD770(void)
{
  SLONG_XYZ pos;
  HeroInfo *base;
  HeroInfo *h1;
  HeroInfo *h2;
  EXTengine *eng;
  s32 s;
  Ovl61Spark *obj;
  NAS *nas;
  NAS *tgt;
  s32 i;
  s32 k;
  i = 0;
  if (((u16) D_800C1B2C->unk1A) != 0)
  {
    return;
  }
  base = Heroes;
  h1 = &base[D_800C2728];
  h2 = &base[D_800C2720];
  for (;;)
  {
    if (i >= 5)
    {
      return;
    }
    D_800C2718++;
    s = D_800C1440[D_800C2718];
    i++;
    switch (s)
    {
      case 0:
        if ((h1->hero_status & 0xFA00) && (h2->hero_status & 0xFA00))
      {
        do
        {
          s = -1;
        }
        while (0);
      }
        break;

      case 1:
        if ((h1->hero_status & 0xFA00) && (base[D_800C271C].hero_status & 0xFA00))
      {
        do
        {
          s = -1;
        }
        while (0);
      }
        break;

      case 2:
        if ((base[D_800C2724].hero_status & 0xFA00) && (h2->hero_status & 0xFA00))
      {
        do
        {
          s = -1;
        }
        while (0);
      }
        break;

      case 3:
        if ((base[D_800C2724].hero_status & 0xFA00) && (base[D_800C271C].hero_status & 0xFA00))
      {
        do
        {
          s = -1;
        }
        while (0);
      }
        break;

      default:
        D_800C2718 = 0;
        break;

    }

    if (s >= 0)
    {
      nas = SpawnNASById(D_800C1A7C[s].x, 0, D_800C1A7C[s].z, 0x1502, 0x11);
      if (nas == 0)
      {
        return;
      }
      eng = nas->Engine;
      if (eng != 0)
      {

        k = 0x12;
        Random(8);
        obj = (Ovl61Spark *) AddObject(nas);
        if (obj != 0)
        {
          nas->FUX = (EXT *) obj;
          obj->unk14 = k;
          obj->unk1A = 7;
        }

        eng->flags = 0;
        D_8009E25C.n++;
        obj->unk24 = k;
        nas->UserInfo.user1 = s;
        nas->UserInfo.user2 = 0xB4;
      }
      break;
    }
  }

  pos = D_8005B2B8[s + 4].pos;
  D_800C1E28[nas->UserInfo.user1].unk10 = 1;
  tgt = func_8007A464(1, (s32 *) (&pos), *((s32 *) D_80058AD0));
  if (tgt != 0)
  {
    func_80026C38((u8 *) nas, (u8 *) tgt);
  }
  if (nas->Failure != 0)
  {
    tgt = nas->Family.Child;
    if (tgt != 0)
    {
      KillNAS(tgt);
    }
    KillNAS(nas);
  }
}

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

s32 func_800BE4D4(s32 arg0, EXTengine *unused) {
    SLONG_XYZ tgt;
    SLONG_XYZ nearp;
    SLONG_XYZ farp;
    VECTOR pos;
    SVECTOR spot;
    EXTengine *me;
    s32 speed;
    FlagRecord *grab;
    s32 top;
    s32 bot;
    s32 rnear;
    s32 rfar;
    HeroInfo *hero;
    Ovl61HeroBox *box;
    NAS *nas;
    EXTengine *eng;
    FlagRecord *rec;
    FlagRecord *fr;
    EXTobject *obj;
    Ovl61Puck *puck;
    s32 r;
    s32 rad;
    s32 back;
    s32 d;
    s32 dist;
    s32 y;
    s32 x1;
    s32 z1;
    s32 x2;
    s32 z2;
    s32 dz;
    s32 fa;
    s16 sp;
    s32 idx;
    TrigEntry *tbl;
    s32 cz;
    s32 fb;
    u16 ang;
    u16 ang2;
    u16 fc;
    u16 fc2;
    Ovl61HeroBox *base;

    if (Heroes[arg0].hero_status & 0xF800) {
        return 0;
    }
    me = Heroes[arg0].hero_eng;
    if (me == NULL) {
        return 0;
    }
    speed = Heroes[arg0].hero_move->forward_speed;
    if (arg0 == D_800C2720) {
        speed = -speed;
    }
    if (arg0 == D_800C271C) {
        speed = Heroes[arg0].hero_move->forward_speed;
    }
    if (arg0 == D_800C2724) {
        speed = -Heroes[arg0].hero_move->forward_speed;
    }
    if (func_80015780(me, me->sub, 1, &pos) == NULL) {
        *(SLONG_XYZ *)&pos = me->unk10;
    }
    fr = func_80025A48(me, 2);
    if (fr != NULL) {
        if (me->unk5E != 0) {
            func_800142D4(&me->unk1C);
        }
        func_8001CD04(me->sub, me->unk3C, &me->unk10);
        grab = fr;
    } else {
        grab = NULL;
    }
    base = D_800C1A88;
    box = &base[arg0];
    top = box->b;
    bot = box->a;
    rnear = box->c;
    rfar = box->d;
    nearp.x = Heroes[arg0].hero_eng->unk10.x + box->x0;
    nearp.y = Heroes[arg0].hero_eng->unk10.y + box->y0;
    nearp.z = Heroes[arg0].hero_eng->unk10.z + box->z0;
    farp.x = Heroes[arg0].hero_eng->unk10.x + box->x1;
    hero = &Heroes[arg0];
    farp.y = hero->hero_eng->unk10.y + box->y1;
    farp.z = hero->hero_eng->unk10.z + box->z1;
    tbl = D_80068BD4;
    for (nas = D_8005AA2C;; nas = nas->Solid.Child) {
        if (nas == NULL) {
            return 0;
        }
        eng = nas->Engine;
        if (eng == NULL || eng == me) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        dz = rec->unk2;
        y = eng->unk10.y;
        x1 = rec->height;
        if (!(top < y + dz)) {
            continue;
        }
        y += x1;
        if (!(y < bot)) {
            continue;
        }
        rad = rec->radius;
        if (func_80015480(hero->hero_move->forward_angle,
                          get_angle(-eng->OldXYZ.x + pos.vx, -eng->OldXYZ.z + pos.vz)) >= 0x38F) {
            r = rnear;
            tgt = nearp;
            back = 0;
        } else {
            r = rfar;
            tgt = farp;
            back = 1;
            if (hero->hero_mode == 0x21) {
                r += 0x80;
            }
        }
        get_angle(eng->unk10.x - pos.vx, eng->unk10.z - pos.vz);
        obj = FindExtension(nas, 0x1000);
        if (obj == NULL) {
            continue;
        }
        if (obj->status == 2) {
            if (arg0 == obj->player) {
                puck = FindExtension(nas, 0x800);
                if (puck != NULL && grab != NULL) {
                    rad += rfar;
                    r = func_800154A0(me->unk1E, puck->dir);
                    if (r > 0x220) {
                        r = 0x220;
                    }
                    if (r < -0x220) {
                        r = -0x220;
                    }
                    r &= 0xFFF;
                    spot.vx = ((Ovl61Spot *)grab)->x - (rad * tbl[(r - 0x400) & 0xFFF].unk2 >> 12);
                    z1 = rad * tbl[r].unk2 >> 12;
                    spot.vz = ((Ovl61Spot *)grab)->z - z1;
                    spot.vy = 0;
                    func_80015758(&spot, (VECTOR *)&eng->unk10);
                }
            }
            continue;
        }
        y = eng->unk10.x - tgt.x;
        dist = y * y;
        y = eng->unk10.z - tgt.z;
        rad += r;
        dist += y * y;
        if (!(dist < rad * rad)) {
            continue;
        }
        dist = csqrt(dist) >> 6;
        if (!(dist < rad)) {
            continue;
        }
        puck = FindExtension(nas, 0x800);
        if (puck != NULL) {
            r = puck->dir;
        } else {
            r = get_angle(eng->OldXYZ.x - eng->unk10.x, eng->OldXYZ.z - eng->unk10.z);
        }
        func_800BE1C4(&eng->unk10, r, &tgt, rad + 8);
        r = get_angle(-eng->unk10.x + tgt.x, -eng->unk10.z + tgt.z) & 0xFFF;
        puck = FindExtension(nas, 0x800);
        if (puck == NULL) {
            continue;
        }
        obj->player = arg0;
        if (hero->hero_mode != 0x21) {
            back = 0;
        }
        if (back) {
            r = get_angle(-eng->unk10.x + farp.x, -eng->unk10.z + farp.z) & 0xFFF;
            puck->dir = r;
            puck->speed = 0;
            puck->max = 0;
            obj->status = 2;
            PlaySfx(D_800C18E8[arg0 + 4], NULL, &me->unk10, 0x1000, 0x1E00, 0);
            continue;
        }
        rad = puck->dir;
        rad += 0x800;
        rad &= 0xFFF;
        if (obj->whatami == 2) {
            if (!(hero->hero_status & 0xFA00) &&
                (arg0 == 0 || (D_8005A680[0] != 0 && arg0 == 1))) {
                hero->hero_score = 0;
                if (arg0 == D_800C2728) {
                    idx = 0;
                } else if (arg0 == D_800C2724) {
                    idx = 1;
                } else if (arg0 == D_800C2720) {
                    idx = 3;
                } else {
                    idx = (arg0 == D_800C271C) * 2;
                }
                if (D_800C1D88[idx].flags != NULL) {
                    *D_800C1D88[idx].flags |= 0x8000;
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
                ((void (*)(s32, s32, s32))func_80027F7C)(arg0, 0xC, 0xFF);
                SetHeroAction((HeroInfo *)arg0, 0x25);
                return 0;
            }
        } else {
            PlaySfx(D_800C1380[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
            fa = hero->hero_move->forward_angle & 0xFFF;
            ang = r;
            fc = fa;
            sp = abs(speed);
            x1 = puck->speed * tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
            z2 = puck->speed * tbl[(u16)r].unk2 >> 12;
            dz = sp * tbl[(fc - 0x400) & 0xFFF].unk2 >> 12;
            cz = sp * tbl[fa].unk2 >> 12;
            r = (u16)get_angle(x1 + dz, z2 + cz);
        }
        r &= 0xFFF;
        d = func_800154A0(rad, r);
        if (abs(d) > 0x400) {
            func_800154A0(rad ^ 0x800, r);
            fb = hero->hero_move->forward_angle & 0xFFF;
            ang2 = r;
            fc2 = fb;
            sp = abs(speed);
            x1 = puck->speed * tbl[(ang2 - 0x400) & 0xFFF].unk2 >> 12;
            z2 = puck->speed * tbl[r].unk2 >> 12;
            dz = sp * tbl[(fc2 - 0x400) & 0xFFF].unk2 >> 12;
            x2 = sp * tbl[fb].unk2 >> 12;
            r = get_angle(x1 + dz, z2 + x2) & 0xFFF;
        } else {
            if (abs(d) > 0x3F0) {
                if (d >= 0) {
                    d = 0x3F0;
                } else {
                    d = -0x3F0;
                }
            }
            r += d;
            r &= 0xFFF;
        }
        puck->dir = r;
        if (speed != 0) {
            puck->speed += abs(speed);
            if (puck->speed > puck->max + 0x40) {
                puck->speed = puck->max + 0x40;
            }
        }
        if (puck->unk32 == 0 && puck->max < 0x90) {
            puck->max += 4;
        }
    }
    return 0;
}

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

s32 func_800BF640(EXTengine *eng, Ovl61FuxF *self) {
    FlagRecord *box;
    FlagRecord *rec;
    EXTengine *other;
    NAS *it;
    Ovl61Mot *mine;
    Ovl61Mot *m;
    Ovl61FuxF *fux;
    s32 hit;
    s32 rad;
    s32 bot;
    s32 top;
    s32 lo;
    s32 hi;
    s32 dx;
    s32 dz;
    s32 r;
    s32 d;
    s32 ang;
    s32 avg;

    hit = 0;
    box = func_80025A48(eng, 0x40);
    if (box != NULL) {
        top = eng->unk10.y + box->unk2;
        bot = eng->unk10.y + box->height;
        rad = box->radius;
        mine = FindExtension(((NAS **)eng)[-1], 0x800);
        it = D_8005AA2C;
        if (it != NULL) {
            do {
                if (it->Class & 0x200) {
                    other = it->Engine;
                    if (other != NULL && other != eng) {
                        rec = func_80025A48(other, 0x40);
                        if (rec != NULL) {
                            lo = other->unk10.y + rec->unk2;
                            hi = other->unk10.y + rec->height;
                            if (bot < lo && hi < top) {
                                dx = other->unk10.x - eng->unk10.x;
                                dz = other->unk10.z - eng->unk10.z;
                                r = rec->radius;
                                r += rad;
                                if ((d = dx * dx + dz * dz) < r * r) {
                                    d = csqrt(d) >> 6;
                                    do {
                                    if (d < r) {
                                        ang = get_angle(-dx, -dz) & 0xFFF;
                                        d = r - d + 1;
                                        fux = (Ovl61FuxF *)it->FUX;
                                        if (fux != NULL) {
                                            if (self->flags & 0x80) {
                                                m = FindExtension(it, 0x800);
                                                if (m != NULL) {
                                                    other->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                    other->unk10.z -= (d * D_80068BD4[ang].unk2) >> 12;
                                                    ang += 0x800;
                                                    ang &= 0xFFF;
                                                    ang += func_800154A0((m->dir + 0x800) & 0xFFF, ang);
                                                    ang &= 0xFFF;
                                                    m->dir = ang;
                                                    PlaySfx(0x580, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                                                }
                                            } else {
                                                if (r / 2 < d) {
                                                    d = r;
                                                }
                                                eng->unk10.x += (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                eng->unk10.z += (d * D_80068BD4[ang].unk2) >> 12;
                                                if (mine != NULL) {
                                                    m = FindExtension(it, 0x800);
                                                    if (m != NULL) {
                                                        if (!(fux->flags & 0x80)) {
                                                            m->dir = ang;
                                                            dx = (mine->a + m->a) / 2;
                                                            m->a = dx;
                                                            mine->a = dx;
                                                        }
                                                        PlaySfx(0x580, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                                                    }
                                                    ang += 0x800;
                                                    ang &= 0xFFF;
                                                    ang += func_800154A0((mine->dir + 0x800) & 0xFFF, ang);
                                                    ang &= 0xFFF;
                                                    mine->dir = ang;
                                                }
                                            }
                                            hit = 1;
                                        }
                                    }
                                    } while (0);
                                }
                            }
                        }
                    }
                }
                it = it->Solid.Child;
            } while (it != NULL);
        }
    }
    return hit;
}

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

s32 GetArkPIContour(EXTengine *v) {
    return v->unk10.y - ((D_8005A5D8 - (v->unk10.z * D_80068BD4[(D_800C1C68 - 0x400) & 0xFFF].unk2 >> 12)) +
                       (v->unk10.x * D_80068BD4[(D_800C1C6C - 0x400) & 0xFFF].unk2 >> 12));
}

s32 GetArkContour(EXTengine *arg0) {
    return arg0->unk10.y - D_8005A5D8;
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

void SetArkHeroGrab(u32 arg0) {
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

void SetArkHeroRedKick(u32 arg0) {
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

void ArkHeroDie(u32 arg0) {
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

void ArkHeroLose(u32 arg0) {
    Heroes[arg0].hero_frozen = 5;
    func_8008502C();
}

void SetArkHeroMove(u32 arg0) {
    func_800B62F4();
    SetHeroSeq(arg0, 0x1A);
}

void SetArkHeroIdle(u32 arg0) {
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

void AR_UpdateShadow(u32 arg0) {
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

void AR_InitShadow(u32 arg0) {
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

bool AR_PI_TestLaunchBounds(EXTengine *arg0, EXTobject *obj) {
    Ovl61Circle *base = D_800C1A7C;
    Ovl61Circle *c;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;

    for (i = 0; i < 4; i++) {
        c = &base[i];
        if (arg0->collision_box.flags & 0x40) {
            r = c->r + arg0->collision_box.radius;
        } else {
            r = c->r + 0x80;
        }
        dx = arg0->unk10.x - c->x;
        dz = arg0->unk10.z - c->z;
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

void AR_EarliestTriggers(void) {
}

void func_800C0B8C(void) {
}

void AR_SetUp(void) {
    D_8005AA70[0] = (s32)AR_PreInitLevel;
    D_8005AA70[1] = (s32)AR_InitLevel;
    D_8005AA70[2] = (s32)AR_InitLevel;
    D_8005AA70[3] = (s32)AR_UpdateLevel;
    D_8005AA70[6] = (s32)func_800C0B7C;
    D_8005AA70[5] = (s32)AR_EarliestTriggers;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[9] = (s32)GetArkContour;
    D_8005AA70[10] = (s32)func_800B9728;
}
