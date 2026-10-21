#include "common.h"
#include "bash/main.h"
#include "bash/ovl67.h"
#include "bash/n2o.h"
#include "bash/arkenoid.h"

void func_800B34A4(void) {
    D_800C45F0 = 0;
    if (!(D_8005A644 & 0x200)) {
        D_800C417A = 0;
    }
    if (D_800C417A == 1) {
        D_8009D6E8.unk20 = (void *)func_800B447C;
        D_8009D6E8.unk68 = (void *)func_800B3888;
        D_8009D6E8.unk24 = (void *)func_800B46DC;
        D_8009D6E8.unk18 = (void *)func_800BBB10;
        D_8009D6E8.unk1C = (void *)func_800BBA4C;
    } else {
        D_8009D6E8.unk20 = (void *)func_800BEB68;
        D_8009D6E8.unk24 = (void *)func_800BF0AC;
        D_8009D6E8.unk68 = (void *)func_800C2F70;
        D_8009D6E8.unk2C = (void *)func_800C2EF8;
        D_8009D6E8.unk18 = (void *)func_800C36DC;
        D_8009D6E8.unk1C = (void *)func_800C36D4;
        D_800C4498 = 0;
    }
    D_8009D6E8.unk0 = (s32)func_800B3E74;
    D_8009D6E8.unk4 = D_800C403C;
    D_8009D6E8.unk14 = D_800C3774;
    D_8009E0D8 = 0x2A;
    D_8009E0E2 = 0x2E;
    D_8005AA94 = (void *)func_800BB05C;
    OVG->unk32 |= 0x100;
    if (OVG->unk6C == 0) {
        OVG->unk51 = 9;
    }
    OVG->unk52 = 9;
    OVG->unk53 = 9;
    D_8009D8FC = OVG->unk6C != 0 ? D_800C3E70 : D_800C3E30;
}

void func_800B361C(void) {
    HeroInfo *h;
    EXTheromove *mv;
    s32 i;

    i = 0;
    D_8005A5D8 = 0x2800;
    D_8005AA38 = NULL;
    D_8005AA3C = 0;
    D_800C4370 = 0;
    D_800C4372 = 0;
    D_800C4374 = 50;
    D_800C4080 = 0;
    D_800C4376 = 0;
    D_800C437A = 0;
    D_800C4378 = 0;
    D_800C4380 = 0;
    D_800C4382 = 50;
    D_800C437C = 0;
    D_800C437E = 0;
    D_800C43DC = 0;
    D_800C438A = 0;
    D_800C438C = 0;
    D_800C4390 = 0;
    D_800C4394 = 0;
    D_800C4384 = 6;
    ((Ovl67SceneRef *)D_80058AD0)->unk10->unk27 = 1;
    ((Ovl67Puck *)D_8005A614)->unk32 &= ~0x800;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        mv = h->hero_move;
        h->hero_eng->unk1E = 0x800;
        mv->facing_angle = 0x800;
        mv->forward_angle = 0x800;
        if (D_800C417A == 1) {
            func_80026980(h->hero);
        }
        D_800C418C = D_800C37DC;
    }
    func_8001682C((s32 *)D_80054504);
    func_8001682C((s32 *)(D_80054504 - 0x1500));
    func_8001682C((s32 *)(D_80054504 - 0x658));
    func_8001682C((s32 *)(D_80054504 - 0x620));
    func_8007E9FC(0x5C, 0x3088);
    D_800C4398.unk0 = 0;
    D_800C4398.unk22 = i + 10;
    D_800C4398.unk4 = 0;
    D_800C4398.unk8 = 0;
    D_800C4398.unkC = 0;
    D_800C4398.unk10 = 0;
    D_800C4398.unk14 = 0;
    D_800C4398.unk18 = 0;
    D_800C4398.unk1C = 0;
    D_800C4398.unk20 = 0;
    D_800C4398.unk24 = 0;
    D_800C4398.unk26 = 0xFF;
    D_800C4398.unk28 = 0;
    D_800C4398.unk2A = 0;
    D_800C4398.unk2C = 0;
    D_800C4398.unk2E = 0;
    D_800C4398.unk30 = 0;
    D_800C4398.unk32 = 0;
    D_800C4398.unk34 = 0;
    D_800C4398.unk38 = 0;
    D_800C4398.unk3C = 0;
    func_800BB070(D_800C417A);
    if (D_800C417A != 1) {
        D_800C4084 = SpawnNASById(0, -0x200, 0xA00, 0x1C02, 0x11);
        func_800C349C();
    } else {
        func_800B6F4C();
    }
    func_800B90F4();
}

s32 func_800B3888(Ovl67Obj *arg0) {
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    Ovl67Circle *c;

    for (i = 0; i < 4; i++) {
        c = (Ovl67Circle *)(i * 6 + (s32)D_800C418C);
        dx = arg0->x - c->x;
        dz = arg0->z - c->z;
        if (arg0->flags & 0x40) {
            r = c->r + arg0->radius;
        } else {
            r = c->r + 0x80;
        }
        if (dx * dx + dz * dz < r * r) {
            arg0->x = D_800C418C[i].x - ((r * D_80068BD4[(get_angle(-dx, -dz) - 0x400) & 0xFFF].unk2) >> 12);
            return 1;
        }
    }
    return 0;
}

s32 func_800B39C0(Ovl67Obj *arg0, s32 arg1) {
    Ovl67Circle *base;
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

    base = D_800C418C;
    ret = 0;
    for (i = 0; i < 4; i++) {
        if (arg0->flags & 0x40) {
            r = base[i].r + arg0->radius;
        } else {
            r = base[i].r + 0x80;
        }
        cx = D_800C418C[i].x;
        x = arg0->x;
        dx = x - cx;
        cz = D_800C418C[i].z;
        z = arg0->z;
        dz = z - cz;
        if (dx * dx + dz * dz < r * r) {
            goto found;
        }
    }
    ret = -1;
    if (D_800C438A == 0) {
        goto fail;
    }
    dx = x;
    dz = z;
    if (arg0->flags & 0x40) {
        r = 0x800 - arg0->radius;
    } else {
        r = 0x800;
    }
    if (dx > r && (D_800C438A & 0x88)) {
        arg0->x = r;
        ret = 0x400;
    }
    if (dx < -r && (D_800C438A & 0x44)) {
        arg0->x = -r;
        do { ret = 0xC00; } while (0);
    }
    if (dz > r && (D_800C438A & 0x22)) {
        arg0->z = r;
        ret = 0;
    }
    if (dz < -r && (D_800C438A & 0x11)) {
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
    v.z = D_800C418C[i].z;
    func_800B9800((SLONG_XYZ *)&arg0->x, arg1, &v, r);
    return get_angle(D_800C418C[i].x - arg0->x, D_800C418C[i].z - arg0->z);
fail:
    return -1;
}

s32 func_800B3C04(Ovl67Obj *arg0) {
    Ovl67Circle *base = D_800C418C;
    Ovl67Circle *c;
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

void func_800B3CD0(s32 arg0) {
    VECTOR sp10;
    VECTOR *v;
    EXTengine *e = (&Heroes[arg0])->hero_eng;
    FlagRecord *r;
    Ovl67HeroBox *b;
    Ovl67HeroBox *base;
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
        base = D_800C4190;
        b = (Ovl67HeroBox *)(arg0 << 2);
        b = (Ovl67HeroBox *)((s32)b + arg0);
        b = (Ovl67HeroBox *)((s32)b << 3);
        b = (Ovl67HeroBox *)((s32)base + (s32)b);
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
        base = D_800C4190;
        b = (Ovl67HeroBox *)(arg0 << 2);
        b = (Ovl67HeroBox *)((s32)b + arg0);
        b = (Ovl67HeroBox *)((s32)b << 3);
        t = r->radius;
        b = (Ovl67HeroBox *)((s32)base + (s32)b);
        b->d = t;
        func_80015758((SVECTOR *)r, v);
        b->x1 = sp10.vx - (&Heroes[arg0])->hero_eng->unk10.x;
        b->y1 = sp10.vy - (&Heroes[arg0])->hero_eng->unk10.y;
        b->z1 = sp10.vz - (&Heroes[arg0])->hero_eng->unk10.z;
    }
}

void func_800B3E74(s32 i) {
    HeroInfo *h;
    HeroInfo *hb;
    HeroInfo *hb2;
    HeroInfo *h2;
    s32 one;
    EXTengine *eng = Heroes[i].hero_eng;
    subengine *sub;
    subengine *n;
    s32 k;
    void *scene;

    Heroes[i].hero_health = 0xFF;
    switch (i) {
    case 0:
        if (D_8005A680[0] != 0) {
            Heroes[0].hero_eng->unk10.x = D_800C4050[1][0];
            Heroes[0].hero_eng->unk10.z = D_800C4050[1][2];
        } else {
            Heroes[0].hero_eng->unk10.x = D_800C4050[0][0];
            Heroes[0].hero_eng->unk10.z = D_800C4050[0][2];
        }
        break;
    case 1:
        Heroes[i].hero_eng->unk10.x = D_800C4050[2][0];
        Heroes[i].hero_eng->unk10.z = D_800C4050[2][2];
        break;
    }
    k = 2;
    hb = Heroes;
    h = hb + i;
    eng->unk5D = 0;
    sub = h->hero_sub;
    func_8001D390(eng, k);
    n = sub->next;
    if (n != NULL) {
        n->unk6C = sub->unk6C;
        n->flags |= 0x8000;
        h->hero_anim->sub = n;
        n = n->next;
        if (n != NULL) {
            n->unk6C = D_80053EE4;
            n->unk74 = 0x2005;
            n->unk50 = 1;
            n->unk51 = 0;
            n->flags &= ~0x8000;
        }
    }
    k = 1;
    if (D_800C417A == k) {
        ((void (*)(s32, s32))func_800B4138)(i, k);
        return;
    }
    D_800C417C[i] = 0xF;
    sub->unk6C = *(void **)D_80058AD0;
    sub->unk74 = 0x4000;
    if ((u32)(i - 2) < 2) {
        sub->flags &= ~0x8000;
    }
    if (D_8005A680[0] == 0 && i == k) {
        sub->flags &= ~0x8000;
    }
    sub->unk50 = 1;
    sub->unk51 = 0;
    sub->unk20.x = 0x1201;
    sub->unk20.y = 0x1201;
    sub->unk20.z = 0x1201;
    one = 1;
    sub->unk52 = one;
    func_80025B10(eng, 0x50);
    hb2 = Heroes;
    h2 = hb2 + i;
    func_80026B70((u8 **)&D_800C4080, (u8 *)h2->hero);
    SetHeroAction((HeroInfo *)i, 0x2F);
    h2->hero_eng->unk10.y = -0x480;
    h2->hero_weapon_count = one;
    if (i == 0) {
        func_800C3440();
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B4138);

void func_800B447C(s32 arg0) {
    EXTheromove *m;
    HeroInfo *h;
    HeroInfo *c;
    Ovl67Pad *p;
    Ovl67Pad *pads;
    s32 d;
    s32 max;
    s32 acc;
    s32 fast;
    s32 b;
    s32 v;

    c = Heroes;
    h = c + arg0;
    m = h->hero_move;
    h->hero_eng->flags |= 0x8000;
    pads = (Ovl67Pad *)D_80051338;
    p = pads + arg0;
    max = 0x68;
    acc = 0x22;
    fast = 0;
    if ((p->unk0 & 1) || h->hero_ai->AImode != 0) {
        if (p->unk50 & 0xF00) {
            max = 0xA0;
            acc = 0x38;
        }
        if (p->unk50 & 0xA0) {
            if (m->velocity_enable != 0) {
                d = 0;
                if (D_8009D750(h->hero_eng) != 0) {
                    fast = 1;
                }
                b = p->unk50;
                if (b & 0x80) {
                    d = -acc;
                }
                if (b & 0x20) {
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
                if (m->forward_speed > max) {
                    m->forward_speed = max;
                }
                if (m->forward_speed < -max) {
                    m->forward_speed = -max;
                }
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
            D_800C4184[arg0] = m->forward_speed;
        } else {
            D_800C4184[arg0] = 0;
            if (D_8009D750(h->hero_eng) != 0) {
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

void func_800B46DC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *g;
    HeroInfo *f;
    s32 a1;
    s32 sp;
    s32 t;

    HeroInfo *c;
    HeroInfo *b;
    HeroInfo *e;

    c = Heroes;
    g = c + arg0;
    a1 = 0;
    if (!(g->hero_status & 0xFA00)) {
        s32 y;
        s32 d;
        t = ((D_8004E0D8 + arg0) << 6) & 0xFFF;
        a1 = D_80068BD4[(t - 0x400) & 0xFFF].unk2 * 17 >> 10;
        y = D_8005A5D8;
        d = (D_80068BD4[(t * 2 - 0x400) & 0xFFF].unk2 * 15 >> 11) - 0x80;
        g->hero_eng->unk10.y = y + d;
    } else {
        g->hero_eng->unk10.y = D_8005A5D8 - 0x80;
    }
    ((void (*)(s32))D_8009D6E8.unk2C)(arg0);
    b = Heroes;
    f = b + arg0;
    sp = f->hero_move->forward_speed;
    if (sp != 0) {
        a1 = -sp;
        f->hero_eng->unk10.x += sp;
    }
    sp = func_800154A0(f->hero_eng->unk20, a1);
    {
        EXTengine *e2 = f->hero_eng;
        e2->unk20 = (u16)e2->unk20 + (sp >> 2);
        func_800B9CB8(arg0, e2);
    }
    if (func_800B9B10(arg0) != 0) {
        if ((u16)D_800C4394 == 0) {
            PlaySfx(0x28A, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        D_800C4394 = 4;
    } else if ((u16)D_800C4394 != 0) {
        D_800C4394 = (u16)D_800C4394 - 1;
    }
    e = Heroes;
    h = e + arg0;
    h->hero_eng->unk5E = 1;
    if (h->hero_mode == 0x26) {
        h->hero_eng->unk10.y = D_8005A5D8 - 0x80;
        return;
    }
    if (!(h->hero_status & 0xF800)) {
        if (h->hero_attack_timer != 0) {
            h->hero_sub->unk74 = 0x2001;
            return;
        }
        h->hero_sub->unk74 = 0x2003;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B4954);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B4AD8);

void func_800B4CE0(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    s32 z;
    s32 s;
    s32 n;

    if (func_800B4AD8() != 0) {
        func_8008502C(arg0);
    }
    base = Heroes;
    h = &base[arg0];
    if (h->hero_eng->unk10.z < -0xA00) {
        h->hero_eng->unk10.z += 0x20;
        z = h->hero_eng->unk10.z;
        s = z + 0xA00;
        n = -(s >> 6);
        s >>= 3;
        if (s > 0x40) {
            s = 0x40;
        }
        if (s != 0) {
            h->hero_eng->unk10.z = z - s;
        } else {
            h->hero_eng->unk10.z = -0xA00;
        }
        n = 0x1E - n;
        if (n <= 0) {
            n = 1;
        }
        if (n > 0x1E) {
            n = 0x1E;
        }
        Heroes[arg0].hero_sub->unk74 = n + 0x3FFF;
    } else {
        if (D_800C43D8 != NULL) {
            *D_800C43D8 &= 0xFFFF7FFF;
        }
        h->hero_eng->unk10.z = -0xA00;
        SetHeroAction((HeroInfo *)arg0, 0x27);
        h->hero_sub->unk74 = 0x2003;
    }
}

void func_800B4E30(s32 arg0) {
    s32 ret;
    u16 st;
    Ovl67PadFlags *pad;
    HeroInfo *h;
    HeroInfo *hb;
    s32 mode;

    if (func_800B4AD8() != 0) {
        func_8008502C(arg0);
    }
    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x2C);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction((HeroInfo *)arg0, 0x2D);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        Ovl67PadFlags *base = (Ovl67PadFlags *)D_80051338;
        pad = &base[arg0];
        if (pad->unk54 & 0x8000) {
            if (Heroes[arg0].hero_attack_timer != 0) {
                pad->unk4C &= ~0x8000;
            } else {
                SetHeroAction((HeroInfo *)arg0, 0x2B);
            }
        }
        hb = Heroes;
        h = hb + arg0;
        if (h->hero_move->forward_speed != 0 &&
            ((mode = h->hero_mode, (s32)mode < 0x2C) == 0 || (s32)mode < 0x2A)) {
            SetHeroAction((HeroInfo *)arg0, 0x28);
        }
    }
}

void func_800B4FA4(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    subengine *s;

    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    func_800B4954(arg0);
    SetHeroAction((HeroInfo *)arg0, 0x27);
    h->hero_idle_count = Random(0x96) + 0xB4;
    s = h->hero_sub;
    s = s->next;
    if (s != NULL) {
        s = s->next;
        if (s != NULL) {
            s->flags &= 0xFFFF7FFF;
        }
    }
}

void func_800B5064(s32 arg0) {
    s32 r;
    u16 st;
    Ovl67PadFlags *pad;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x2C);
        r = 1;
    } else if (st & 0x2000) {
        SetHeroAction((HeroInfo *)arg0, 0x2D);
        r = 1;
    } else {
        r = 0;
    }
    if (r == 0) {
        Ovl67PadFlags *base = (Ovl67PadFlags *)D_80051338;
        pad = &base[arg0];
        if (pad->unk54 & 0x8000) {
            if (Heroes[arg0].hero_attack_timer != 0) {
                pad->unk4C &= ~0x8000;
            } else {
                SetHeroAction((HeroInfo *)arg0, 0x2B);
            }
        }
        func_8008502C(arg0);
    }
}

void func_800B5168(s32 arg0) {
    s32 ret;
    u16 st;
    Ovl67PadFlags *pad;
    HeroInfo *h;
    HeroInfo *hb;
    s32 mode;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x2C);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction((HeroInfo *)arg0, 0x2D);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        Ovl67PadFlags *base = (Ovl67PadFlags *)D_80051338;
        pad = &base[arg0];
        if (pad->unk54 & 0x8000) {
            if (Heroes[arg0].hero_attack_timer != 0) {
                pad->unk4C &= ~0x8000;
            } else {
                SetHeroAction((HeroInfo *)arg0, 0x2B);
            }
        }
        hb = Heroes;
        h = hb + arg0;
        if (h->hero_move->forward_speed != 0 &&
            ((mode = h->hero_mode, (s32)mode < 0x2C) == 0 || (s32)mode < 0x2A)) {
            SetHeroAction((HeroInfo *)arg0, 0x28);
        }
        func_8008502C(arg0);
    }
}

void func_800B52CC(s32 arg0) {
    s32 ret;
    u16 st;
    Ovl67PadFlags *pad;
    HeroInfo *h;
    HeroInfo *hb;
    s32 mode;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x2C);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction((HeroInfo *)arg0, 0x2D);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        Ovl67PadFlags *base = (Ovl67PadFlags *)D_80051338;
        pad = &base[arg0];
        if (pad->unk54 & 0x8000) {
            if (Heroes[arg0].hero_attack_timer != 0) {
                pad->unk4C &= ~0x8000;
            } else {
                SetHeroAction((HeroInfo *)arg0, 0x2B);
            }
        }
        hb = Heroes;
        h = hb + arg0;
        if (h->hero_move->forward_speed == 0 &&
            ((mode = h->hero_mode, (s32)mode < 0x2C) == 0 || (s32)mode < 0x2A)) {
            SetHeroAction((HeroInfo *)arg0, 0x27);
        }
        if (func_800B4AD8(arg0) != 0) {
            func_8008502C(arg0);
        }
    }
}

void func_800B5440(s32 arg0) {
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
    D_8005A689 = 0;
}

void func_800B5550(s32 arg0) {
    EXTengine *e;
    s32 flag;
    s32 t;

    e = Heroes[arg0].hero_eng;
    t = e->unk10.z;
    flag = 0;
    if (t < -0x500) {
        e->unk10.z = t + 0x20;
    } else {
        flag = 1;
    }
    if (D_8005A680[0] != 0) {
        if (arg0 == 0) {
            e = D_8009D530;
            t = e->unk10.x;
            if (t >= -0x27F) {
                e->unk10.x = t - 0x40;
            }
        } else {
            e = Heroes[arg0].hero_eng;
            t = e->unk10.x;
            if (t < 0x280) {
                e->unk10.x = t + 0x40;
            }
        }
    }
    if (flag != 0) {
        D_800AD12C = 1;
    }
    func_80085890(arg0, flag);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

void func_800B5674(s32 arg0) {
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
    func_80026980(h->hero);
    h->hero_sub->unk08 = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    SetHeroSeq(arg0, 0x29);
    h->hero_hit = 0x3C;
}

void func_800B5774(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800B4954();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x25);
    h->hero_attack_delay += 0x1E;
    if (h->hero_attack_delay > 0x5A) {
        h->hero_attack_delay = 0x5A;
    }
    h->hero_attack_counter = 0;
    h->hero_attack_timer = h->hero_attack_delay;
    PlaySfx(D_800C39A0[arg0], NULL, NULL, 0x1000, 0x1E00, 0);
}

void func_800B5838(s32 arg0) {
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
            s->unk74 = 0x2005;
            s->unk20.x = scale & 0xFFFF;
            s->unk20.y = scale & 0xFFFF;
            s->unk20.z = scale & 0xFFFF;
            s->unk76 = ang;
            s->unk52 = 1;
            s->flags |= 0x40008021;
        }
    }
    if (func_800BA43C(arg0, (scale & 0xFFFF) >> 3, 0)) {
        Heroes[arg0].hero_attack_timer = 0;
    }
    base2 = Heroes;
    base2[arg0].hero_attack_counter++;
    if (base2[arg0].hero_attack_counter >= 4) {
        SetHeroAction((HeroInfo *)arg0, 0x27);
        if (s != 0) {
            s->flags &= 0xFFFF7FFF;
        }
    }
    if (func_800B4AD8(arg0) != 0) {
        func_8008502C(arg0);
    }
}

void func_800B59A8(s32 arg0) {
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
    func_8007A524(0, (s32 *)&sp18, (s32) &D_80053EE4);
    PlaySfx(0x586, NULL, &temp_s0_2->hero_eng->unk10, 0x1000, 0x1E00, 0);
    func_800898EC(arg0);
}

void func_800B5B14(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x2C);
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

void func_800B5BFC(void) {
    s32 n;
    Ovl67Node *p;

    if (D_8005A644 & 0x801) {
        return;
    }
    if (D_800C4374 != 0) {
        D_800C4374--;
        return;
    }
    D_800C4370++;
    n = (u16)((u16)D_800C4370 / 240);
    n++;
    if (n >= 5) {
        n = 4;
    }
    p = D_8005AA38;
    D_800C4372 = 0;
    if (p != NULL) {
        do {
            p = p->next;
            D_800C4372++;
        } while (p != NULL);
    }
    if ((u16)D_800C4372 < n) {
        func_800BB624();
        D_800C4374 = 30;
    }
}

void InitN2OBall(NAS *nas) {
    EXTengine *eng;
    EXTengine *e1;
    EXTobject *obj;
    EXTmotion *mot;
    subengine *sub;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->sub->flags |= 0x02000000;
        eng->flags &= 0xFFFF7FFF;
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
            }
        }
        eng->unk5D = 2;
        eng->sub->unk6C = D_80054504;
        eng->sub->unk74 = 0x2009;
        func_80026B70((u8 **)&D_8005AA38, (u8 *)nas);
        e1 = AddEngine(nas, 1);
        if (e1 != NULL) {
            sub = e1->sub;
            sub->unk6C = D_80054504;
            sub->unk74 = 0x200B;
            e1->flags |= 0x22000021;
            sub->flags |= 0x22000021;
            e1->flags &= 0xFFFF7FFF;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl67/ovl67", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", PlotN2OBall);

void func_800B6E6C(Ovl67Blend *p) {
    s32 x1;
    s32 z1;
    s16 vel;
    s32 speed;

    speed = p->speed_b;
    if (speed != 0) {
        u32 da = p->dir;
        u32 db = p->dir_b;
        s32 a, b, c, d;
        vel = p->speed_a;
        a = vel * D_80068BD4[(da - 0x400) & 0xFFF].unk2;
        b = vel * D_80068BD4[da & 0xFFF].unk2;
        c = speed * D_80068BD4[(db - 0x400) & 0xFFF].unk2;
        d = speed * D_80068BD4[db & 0xFFF].unk2;
        x1 = a >> 12;
        z1 = b >> 12;
        x1 += c >> 12;
        z1 += d >> 12;
        p->dir = get_angle(x1, z1) & 0xFFF;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B6F4C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B7244);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B7858);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B7C58);

s32 func_800B8058(s32 arg0) {
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

s32 func_800B817C(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, s32 arg5) {
    Ovl67Pos buf;
    s32 a;
    Ovl67Node *t;
    Ovl67Node *n;
    Ovl67Node *best;
    HeroInfo *h;
    HeroInfo *h2;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;

    arg2 *= arg2;
    if (Heroes[arg0].hero_attack_timer != 0) {
        goto ret0;
    }
    best = NULL;
    n = D_8005AA38;
    if (n != NULL) {
        h = &Heroes[arg0];
    loop:
        do {
            t = n->target;
            if (t != NULL) {
                dx = h->hero_eng->unk10.x - t->x;
                dz = h->hero_eng->unk10.z - t->z;
                if (dx * dx + dz * dz < arg2 && n->unk58 != NULL && n->unk58->unk14 != 2) {
                    a = get_angle(dx, dz);
                    d = func_800154A0(a, h->hero_move->forward_angle);
                    if (arg1 == -1) {
                        if ((s16)arg3 < d && d < (s16)arg4) {
                            best = n;
                        }
                    } else if (FindExtension((NAS *)n, 0x800) != NULL) {
                        r = func_800B7C58(arg1, (Ovl67Pos *)&t->x, &buf, (u16)a);
                        best = n;
                        if (r != 0) {
                            goto found;
                        }
                    }
                }
            }
        } while (0);
        do {
            n = n->next;
        } while (0);
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
        if ((func_800B7C58(arg1, (Ovl67Pos *)&t->x, &buf, (u16)a) ^ 1) == 0) {
            goto ret0;
        }
    }
found:
    return 1;
ret0:
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B83EC);

void func_800B8968(s32 a) {
    s16 (*tab)[3] = (s16 (*)[3])D_800C37DC;
    s32 arg0 = a & 0xFFFF;
    NAS *n;
    Ovl67Slot *base;
    s32 x = tab[arg0][0];
    s32 z = tab[arg0][1];
    Ovl67Slot *s;
    s32 y;

    base = D_800C42D0;
    s = &base[arg0];
    y = s->src->y;

    if (s->kind != 3) {
        n = s->nas;
        if (n != NULL) {
            return;
        }
        n = SpawnNASById(x, y, z, 0x1B03, 0x11);
    } else {
        n = s->nas;
        if (n != NULL) {
            n->UserInfo.flags = 1;
            return;
        }
        n = SpawnNASById(x, y, z, 0x1B02, 0x11);
    }
    if (n != NULL) {
        if (n->Failure == 0) {
            s->nas = n;
        }
        n->UserInfo.user1 = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", PlotN2Omissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", PlotN2Ofirstmissile);

void func_800B90F4(void) {
    s32 i;
    Ovl67Slot *s;
    Ovl67Ent *e;
    s16 *t;
    u32 k;

    for (i = 0; i < 4; i++) {
        D_800C4230[i].ent = NULL;
        D_800C4230[i].f4 = 0;
        D_800C4230[i].f8 = 0;
        D_800C4230[i].f12 = 0;
        D_800C4230[i].fC = 0;
        D_800C4230[i].f12 = 0;
        D_800C4230[i].fC = 0;
        D_800C4230[i].f16 = 0;
        D_800C4230[i].f18 = 0;
        D_800C4230[i].f1A = 0;
        D_800C4230[i].f1C = 0;
        D_800C4230[i].f20 = 0;
        D_800C4230[i].f24 = 0;
        D_800C42D0[i].src = NULL;
        D_800C42D0[i].unk4 = 0;
        D_800C42D0[i].nas = NULL;
        D_800C42D0[i].f12 = 0;
        D_800C42D0[i].f0C = 0;
        D_800C42D0[i].f12 = 0;
        D_800C42D0[i].f0C = 0;
        D_800C42D0[i].f16 = 0;
        D_800C42D0[i].f18 = 0;
        D_800C42D0[i].kind = 3;
        D_800C42D0[i].f1C = 0;
        D_800C42D0[i].f20 = 0;
        D_800C42D0[i].f24 = 0;
    }
    e = (Ovl67Ent *)((GfxScene *)D_80058AD0)->objs;
    D_800C43D8 = NULL;
    for (; e != NULL; e = e->next) {
        t = (s16 *)func_80015870(e->key, e->id);
        if (t[1] == 0x63) {
            k = t[0] - 10;
            if (k < 4) {
                D_800C4230[k].ent = e;
                D_800C4230[k].fC = e->id;
            }
            k = t[0] - 10;
            if (k < 4) {
                s = &D_800C42D0[k];
                s->src = (void *)e;
                s->f1C = e->y;
                if (!(k & 1)) {
                    e->y += 0x200;
                    s->kind = 0;
                }
                s->f20 = e->y;
            }
            if (t[0] == 0x61) {
                D_800C43D8 = (u32 *)e;
            }
            if (t[0] == 0x62) {
                e->flags = 0;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B92D4);

void InitN2ODeadWall(NAS *nas)
{
    EXTengine *eng;
    subengine *sub;
    u32 dir;
    s32 off;

    if (D_8005A644 & 0x801) {
        D_800C438A &= ~nas->UserInfo.flags;
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    if (eng != 0) {
        dir = eng->unk10.x;
        eng->flags &= 0xFFFF7FFF;
        if (dir == 3) {
            eng->unk10.x = -0x600;
            eng->unk10.z = 0x880;
            eng->unk1C = 0x38E;
            eng->unk1E = 0;
            nas->UserInfo.flags = 2;
            D_800C438A |= 2;
        }
        if (dir < 2) {
            eng->unk10.x = -0x600;
            eng->unk10.z = -0x880;
            eng->unk1C = 0x200;
            eng->unk1E = 0;
            nas->UserInfo.flags = 1;
            D_800C438A |= 1;
        }
        if (dir == 4) {
            eng->unk10.x = -0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = dir;
            D_800C438A |= 4;
        }
        if (dir == 5) {
            eng->unk10.x = 0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = 8;
            D_800C438A |= 8;
        }
        eng->unk5D = 2;
        eng->unk5E = 1;
        if (D_800C438C == 0) {
            PlaySfx(0x591, &eng->sfx, 0, 0x1000, 0x1E00, 0);
        }
        D_800C438C = 1;
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
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800B9800(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r) {
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

s32 func_800B9B10(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *t;
    EXTengine *eng;
    EXTengine *teng;
    s32 mate;
    s32 dx;
    s32 dz;
    s32 r;

    base = Heroes;
    h = &base[arg0];
    if (!(h->hero_status & 0xFA00) && D_8005A680[0] != 0) {
        mate = h->hero_teammate;
        t = &base[mate];
        if (!(t->hero_status & 0xFA00)) {
            eng = h->hero_eng;
            if (eng != NULL && (r = h->hero_move->forward_speed) != 0) {
                teng = t->hero_eng;
                dx = eng->unk10.x - teng->unk10.x;
                dz = eng->unk10.z - teng->unk10.z;
                if (eng->collision_box.flags & 0x40) {
                    r = eng->collision_box.radius;
                } else {
                    r = 0x100;
                }
                r *= 2;
                r += 0x90;
                if (dx * dx + dz * dz < r * r) {
                    eng->unk10.x = Heroes[mate].hero_eng->unk10.x - ((r * D_80068BD4[(get_angle(-dx, -dz) - 0x400) & 0xFFF].unk2) >> 12);
                    return 1;
                }
                return 0;
            }
        }
    }
    return 0;
}

s32 func_800B9CB8(s32 arg0, EXTengine *unused) {
    SLONG_XYZ tgt;
    SLONG_XYZ nearp;
    SLONG_XYZ farp;
    VECTOR pos;
    EXTengine *me;
    s32 speed;
    s32 top;
    s32 bot;
    s32 rnear;
    s32 rfar;
    HeroInfo *hero;
    Ovl67HeroBox *box;
    NAS *nas;
    EXTengine *eng;
    FlagRecord *rec;
    EXTobject *obj;
    Ovl67Puck *puck;
    s32 r;
    s32 rad;
    s32 d;
    s32 dist;
    s32 y;
    s32 hy;
    s32 x1;
    s32 x2;
    s32 z2;
    s32 dz;
    s32 fa;
    s16 sp;
    TrigEntry *tbl;
    s32 cz;
    s32 fb;
    u16 ang;
    u16 ang2;
    u16 fc;
    u16 fc2;
    Ovl67HeroBox *base;

    if (Heroes[arg0].hero_status & 0xF800) {
        return 0;
    }
    me = Heroes[arg0].hero_eng;
    if (me == NULL) {
        return 0;
    }
    speed = Heroes[arg0].hero_move->forward_speed;
    if (arg0 == 3) {
        speed = -Heroes[3].hero_move->forward_speed;
    }
    if (func_80015780(me, me->sub, 1, &pos) == NULL) {
        *(SLONG_XYZ *)&pos = me->unk10;
    }
    if (func_80025A48(me, 2) != NULL) {
        if (me->unk5E != 0) {
            func_800142D4(&me->unk1C);
        }
        func_8001CD04(me->sub, me->unk3C, &me->unk10);
    }
    hy = Heroes[arg0].hero_eng->unk10.y;
    base = D_800C4190;
    box = &base[arg0];
    top = hy + box->b;
    bot = hy + box->a;
    rnear = box->c;
    rfar = box->d;
    nearp.x = Heroes[arg0].hero_eng->unk10.x + box->x0;
    nearp.y = Heroes[arg0].hero_eng->unk10.y + box->y0;
    nearp.z = Heroes[arg0].hero_eng->unk10.z + box->z0;
    hero = &Heroes[arg0];
    farp.x = hero->hero_eng->unk10.x + box->x1;
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
        } else {
            r = rfar;
            tgt = farp;
        }
        get_angle(eng->unk10.x - pos.vx, eng->unk10.z - pos.vz);
        obj = FindExtension(nas, 0x1000);
        if (obj == NULL) {
            continue;
        }
        rad += r;
        y = eng->unk10.x - tgt.x;
        dist = y * y;
        y = eng->unk10.z - tgt.z;
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
        func_800B9800(&eng->unk10, r, &tgt, rad + 8);
        r = get_angle(-eng->unk10.x + tgt.x, -eng->unk10.z + tgt.z) & 0xFFF;
        puck = FindExtension(nas, 0x800);
        if (puck == NULL) {
            continue;
        }
        obj->player = arg0;
        rad = puck->dir;
        rad += 0x800;
        rad &= 0xFFF;
        if (obj->whatami == 2 && !(hero->hero_status & 0xFA00) &&
            (arg0 == 0 || (D_8005A680[0] != 0 && arg0 == 1))) {
            if (!(D_8005A618 & 1)) {
                hero->hero_score = 0;
                hero->hero_health = 0;
                ((void (*)(s32, s32, s32))func_80027F7C)(arg0, 0xC, 0xFF);
                SetHeroAction((HeroInfo *)arg0, 0x2E);
            }
            KillNAS(nas);
            return 0;
        }
        PlaySfx(D_800C39B0[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        fa = hero->hero_move->forward_angle & 0xFFF;
        ang = r;
        fc = fa;
        sp = abs(speed);
        x1 = puck->speed * tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
        z2 = puck->speed * tbl[r & 0xFFF].unk2 >> 12;
        dz = sp * tbl[(fc - 0x400) & 0xFFF].unk2 >> 12;
        cz = sp * tbl[fa].unk2 >> 12;
        r = get_angle(x1 + dz, z2 + cz) & 0xFFF;
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

s32 func_800BA43C(u32 arg0, s32 arg1, bool arg2) {
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
    s32 hit;

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
                                    *(u16 *)(t + 0x1C) |= 8;
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
                                        *(s16 *)(e + 0xE) = *(u16 *)(e + 0x14) + 0x40;
                                        *(u16 *)(e + 0x14) = 0x90;
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BA6C4);

NAS *func_800BAAA8(void) {
    SLONG_XYZ sp10;
    s32 r;
    EXTengine *me;
    EXTengine *eng;
    FlagRecord *rec;
    NAS *nas;
    EXTengine *ext;
    s32 r0;
    s32 dx;
    s32 dz;

    me = D_800C439C;
    if (me != NULL) {
        rec = func_80025A48(me, 0x40);
        if (rec != NULL) {
            if (me->unk5E != 0) {
                func_800142D4(&me->unk1C);
            }
            func_8001CD04(me->sub, me->unk3C, &me->unk10);
            r0 = rec->radius;
            func_80015758((SVECTOR *)rec, (VECTOR *)&sp10);
            for (nas = D_8005AA2C; nas != NULL; nas = nas->Solid.Child) {
                eng = nas->Engine;
                if (eng != NULL && eng != me) {
                    FlagRecord *rec2;
                    do { rec2 = func_80025A48(eng, 0x40); } while (0);
                    if (rec2 != NULL) {
                        r = rec2->radius;
                        ext = FindExtension(nas, 0x1000);
                        if (ext != NULL) {
                            r += r0;
                            if (ext->unk1C & 8) {
                                dx = eng->unk10.x - sp10.x;
                                dz = eng->unk10.z - sp10.z;
                                if (r * r > dx * dx + dz * dz && FindExtension(nas, 0x800) != NULL) {
                                    ext->unk1E = 0x100;
                                    return nas;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

s32 func_800BAC30(x, y, z, size, mask)
    s32 x;
    s32 y;
    s32 z;
    u16 size;
    u16 mask;
{
    s32 ret;
    u32 i;
    EXTengine *eng;
    NAS *nas;
    EXTobject *obj;
    FlagRecord *rec;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 rr;
    s32 dist;

    ret = -1;
    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_status & 0xFA00) {
            continue;
        }
        eng = Heroes[i].hero_eng;
        if (eng == NULL) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        top = eng->unk10.y + rec->unk2 + size;
        bot = eng->unk10.y + rec->height - size;
        if (y < top && bot < y) {
            dx = eng->unk10.x - x;
            dist = dx * dx;
            dz = eng->unk10.z - z;
            dist += dz * dz;
            rr = rec->radius + size;
            rr *= rr;
            if (dist < rr) {
                ret = i;
            }
        }
    }
    for (nas = D_8005AA2C; nas != NULL; nas = nas->Solid.Child) {
        if (!(nas->Class & 0x200)) {
            continue;
        }
        eng = nas->Engine;
        if (eng == NULL) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        top = eng->unk10.y + rec->unk2 + size;
        bot = eng->unk10.y + rec->height - size;
        if (y < top && bot < y) {
            dx = eng->unk10.x - x;
            dist = dx * dx;
            dz = eng->unk10.z - z;
            dist += dz * dz;
            rr = rec->radius + size;
            rr *= rr;
            if (dist < rr) {
                obj = FindExtension(nas, 0x1000);
                if (obj != NULL && (obj->can_do & mask)) {
                    obj->player = -1;
                    obj->will_do |= mask;
                }
                if (ret != -1) {
                    ret = -1;
                }
            }
        }
    }
    return ret;
}

s32 func_800BAEA8(Ovl67Vel *arg0, s32 arg1) {
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

void PlotN2ODeadWall(NAS *arg0) {
    EXTengine *eng;
    subengine *sub;

    if (D_8005A644 & 0x801) {
        D_800C438A &= ~arg0->UserInfo.flags;
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

s32 func_800BB05C(u8 *arg0) {
    return (*(s32*)((u8*)(arg0) + 20)) - D_8005A5D8;
}

void func_800BB070(u16 arg0) {
    Ovl67FlagNode *n = ((Ovl67SceneRef *)D_80058AD0)->unk1C;
    s16 *p;

    if (arg0) {
        while (n != NULL) {
            p = (s16 *)func_80015870(n->unk6C, n->unk74);
            if (p[1] == 0x63) {
                if (p[0] != 0x62) {
                    n->flags |= 0x8000;
                } else {
                    n->flags &= ~0x8000;
                }
            } else {
                n->flags &= ~0x8000;
            }
            n = n->next;
        }
    } else {
        while (n != NULL) {
            p = (s16 *)func_80015870(n->unk6C, n->unk74);
            if (p[1] == 0x63) {
                n->flags &= ~0x8000;
            } else {
                n->flags |= 0x8000;
            }
            n = n->next;
        }
    }
}

void func_800BB184(void) {
    s32 *t;
    s32 z;
    CamState *cam;
    EXTengine *eng;
    s32 d;

    t = func_8001E48C(0x63);
    eng = Heroes[0].hero_eng;
    z = eng->unk10.z;
    if (D_8005A680[0] != 0) {
        if (Heroes[0].hero_status & 0xFA00) {
            z = Heroes[1].hero_eng->unk10.z;
        }
        if (Heroes[1].hero_status & 0xFA00) {
            z = eng->unk10.z;
        }
    }
    if ((!(Heroes[0].hero_status & 0xFA00) || !(Heroes[1].hero_status & 0xFA00)) && t != NULL) {
        cam = (CamState *)D_80051640;
        cam->unk0 = 0;
        cam->unk4 = 0;
        cam->unk8 = func_800153B4(t + 0x13, t + 0xC, cam->rot);
        cam->pos = *(CamVec3 *)(t + 0xC);
        cam->unk18 = 0x200;
        d = -0xA00 - cam->pos.v[2];
        cam->pos.v[2] = z - d;
    }
}

void PlotN2OmissileExplode(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s16 *p;
    s16 (*tab)[3];

    eng = nas->Engine;
    if (eng != NULL) {
        tab = D_800C4150;
        p = tab[nas->UserInfo.flags];
        sub = eng->sub;
        if (p[0] != 0) {
            sub->unk20.x = p[0];
            sub->unk20.y = p[1];
            sub->unk20.x = p[2];
            sub->unk52 = 1;
            nas->UserInfo.flags++;
            return;
        }
        KillNAS(nas);
    }
}

void InitN2OmissileExplode(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        sub->unk6C = &D_80053EAC;
        sub->unk74 = 0x2005;
        sub->unk50 = 1;
        sub->unk51 = 0;
        eng->flags |= 0x21;
        nas->UserInfo.flags = 0;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800BB3A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    NAS *nas;

    nas = SpawnNASById(arg0, arg1 - 0x80, arg2, 0x1B04, 0x11);
    if (nas != NULL) {
        nas->UserInfo.user1 = arg3;
        PlaySfx(D_800C39DC[Random(4)], NULL, &nas->Engine->unk10, 0x1000, 0x1E00, 0);
    }
}

void InitN2Ofirstmissile(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    EXTmotion *mot;
    s32 ang;
    s32 d;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        eng->unk5D = 0;
        sub->unk6C = &D_80053EAC;
        sub->unk74 = 0x2004;
        sub->unk51 = 0;
        sub->unk68 = 0x100;
        sub->flags |= 0x02000000;
        mot = AddMotion(nas);
        if (mot != NULL) {
            mot->Xvel = -0x28;
            mot->Xmax = 0x100;
            mot->Xacc = 8;
            mot->Xdec = 0;
        }
        d = D_8009D530->unk10.x;
        ang = get_angle(eng->unk10.x - d, eng->unk10.z - d);
        nas->UserInfo.user2 = ang;
        eng->unk1E = ang - 0x400;
        eng->boomshanka = 1;
        nas->UserInfo.flags = 0;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitN2Omissile(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    EXTmotion *mot;
    s32 ang;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        eng->unk5D = 0;
        sub->unk6C = &D_80053EAC;
        sub->unk74 = 0x2004;
        sub->unk51 = 0;
        sub->flags |= 0x02000000;
        mot = AddMotion(nas);
        if (mot != NULL) {
            mot->Xvel = -0x28;
            mot->Xmax = 0x100;
            mot->Xacc = 8;
            mot->Xdec = 0;
        }
        ang = get_angle(eng->unk10.x - ((s32 *)D_8009D530)[4], eng->unk10.z - ((s32 *)D_8009D530)[4]);
        nas->UserInfo.user2 = ang;
        eng->unk1E = ang - 0x400;
        eng->boomshanka = 1;
        eng->unk10.y += 0x200;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800BB5F0(void) {
    volatile s32 x, y, z;
    SpawnNASById(x, y, z, 0x1B01, 0x11);
}

u8 *func_800BB624(void) {
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
    temp_v0 = SpawnNASById(0x1000, 0x1000, 0x1000, 0x1B01, 0x11);
    if ((temp_v0 != NULL) && ((*(s32*)((u8*)(temp_v0) + 84)) != 0)) {
        temp_v0_2 = (*(s32*)((u8*)(temp_v0) + 88));
        if (temp_v0_2 != NULL) {
            (*(s16*)((u8*)(temp_v0_2) + 26)) = 0;
        }
    }
    return temp_v0;
}

void func_800BB6BC(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_frozen = 5;
    if (h->hero_hit != 0) {
        h->hero_hit--;
    } else {
        SetHeroAction((HeroInfo *)arg0, 0x2E);
    }
    func_8008502C(arg0);
}

void func_800BB72C(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    func_800B4954();
    SetHeroSeq((HeroInfo *)arg0, 0x22);
}

void func_800BB788(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x24);
}

void func_800BB7D4(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->velocity_enable = 1;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    func_800B4954(arg0);
    SetHeroSeq((HeroInfo *)arg0, 0x21);
    h->hero_idle_count = Random(0x96) + 0xB4;
}

void func_800BB86C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    h->hero_move->velocity_enable = 0;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    func_800B4954(arg0);
    SetHeroSeq((HeroInfo *)arg0, 0x21);
}

s32 func_800BB8EC(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x2C);
        return 1;
    }
    return 0;
}

s32 func_800BB944(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x2C;
    if (!(st & 0x200)) {
        act = 0x2D;
        if (!(st & 0x2000)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_800BB9B0(s32 arg0) {
    PadStick *base = (PadStick *)D_80051338;
    PadStick *pad = &base[arg0];

    if (pad->dirPressed & 0x8000) {
        if (Heroes[arg0].hero_attack_timer == 0) {
            SetHeroAction((HeroInfo *)arg0, 0x2B);
            return 1;
        }
        pad->dirHeld &= 0xFFFF7FFF;
    }
    return 0;
}

void func_800BBA4C(s32 arg0) {
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

void func_800BBB10(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    s->sub->unk6C = D_80053EE4;
    s->sub->unk74 = 0x2006;
    s->unk10 = h->hero_eng->unk10;
    s->unk5D = 0;
    s->flags |= 0x8041;
    s->sub->unk76 = 0x200;
    s->sub->flags |= 0x42000000;
    s->sub->unk68 = -0x10;
}

s32 func_800BBBB4(s32 arg0, s16 arg1, s32 arg2, s16 arg3) {
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

s32 func_800BBC78(Ovl67Vel *arg0, Ovl67Out *arg1, s32 arg2) {
    s32 lim;
    s32 x;
    s32 z;

    if (arg2 != 0) {
        lim = 0x1200;
    } else {
        lim = 0xB00;
    }
    x = arg0->x;
    if (x > lim) {
        arg1->dir = -1;
        return 1;
    }
    if (x < -lim) {
        arg1->dir = -1;
        return 1;
    }
    z = arg0->z;
    if (z > lim) {
        arg1->dir = 3;
        return 1;
    }
    if (z < -lim) {
        arg1->dir = 0;
        return 1;
    }
    if (arg2 != 0 && z < -0xA00) {
        if (x > 0x600) {
            arg0->x = 0x600;
        }
        if (arg0->x < -0x600) {
            arg0->x = -0x600;
        }
    }
    return 0;
}

void func_800BBD18(void) {
    if (D_800C417A == 1) {
        func_800B92D4();
        func_800B5BFC();
        func_800B7244();
        return;
    }
    func_800BE70C();
}

void func_800BBD68(void) {
    D_8005AA70[0] = (s32)func_800B34A4;
    D_8005AA70[1] = (s32)func_800B361C;
    D_8005AA70[2] = (s32)func_800B361C;
    D_8005AA70[9] = (s32)func_800BB05C;
    D_8005AA70[6] = 0;
    D_8005AA70[5] = 0;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[3] = (s32)func_800BBD18;
}
