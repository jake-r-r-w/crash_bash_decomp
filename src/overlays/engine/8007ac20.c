#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/8007ac20.h"
#include "bash/hero.h"
#include "bash/shell.h"

void func_8007AC20(void) {
    EngineSlotView *v;
    s32 i;

    D_80099174 = (D_80099174 | 0x80000000) & 0xFFFE2FFF;
    v = (EngineSlotView *)D_8005A614;
    if (v->unk6C != 0) {
        i = v->unk48;
        D_800991D8[0] = i;
        D_800991D8[1] = i;
        i = v->unk58[i];
        if (i != -1) {
            D_800991D8[1] = i;
        }
        i = v->unk4A;
        D_800991D8[2] = i;
        D_800991D8[3] = i;
        i = v->unk58[i];
        if (i != -1) {
            D_800991D8[3] = i;
        }
    } else {
        D_800991D8[0] = 0;
        D_800991D8[1] = 1;
        D_800991D8[2] = 2;
        D_800991D8[3] = 3;
    }
    func_8007EAC0();
    if (D_8004DDD0[D_8005A648 * 3] != 0x101 && D_8004DDD0[D_8005A648 * 3] != 0x102) {
        switch (D_8005A648) {
        case 14:
        case 15:
        case 16:
        case 17:
            func_8007BFA8();
            func_8007D0BC();
            func_8007EB68();
            func_8007E874();
            break;
        case 10:
        case 11:
        case 12:
            func_8007BFA8();
            func_8007CFAC();
            func_8007EB68();
            break;
        case 13:
            func_8007BFA8();
            func_8007CFAC();
            func_8007EB68();
            break;
        case 29:
            func_8007BFA8();
            func_8007CFAC();
            func_8007EB68();
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            func_8007BFA8();
            func_8007C338();
            func_8007EB68();
            func_8007E874();
            func_8007D208();
            break;
        case 9:
            func_8007BFA8();
            func_8007C338();
            func_8007EB68();
            func_8007D208();
            break;
        case 30:
            func_8007BFA8();
            func_8007CFAC();
            func_8007EB68();
            func_8007E874();
            break;
        case 0:
        case 1:
        case 2:
        case 3:
            func_8007BFA8();
            func_8007C338();
            func_8007EB68();
            func_8007E874();
            break;
        case 4:
            func_8007BFA8();
            func_8007C338();
            func_8007EB68();
            break;
        case 32:
            func_8007C170();
            func_8007C338();
            func_8007EB68();
            break;
        case 23:
        case 24:
        case 25:
        case 26:
            func_8007BFA8();
            func_8007EB68();
            func_8007D034();
            func_8007CC80();
            break;
        case 28:
            func_8007BFA8();
            func_8007CFAC();
            func_8007EB68();
            break;
        case 27:
            func_8007BFA8();
            func_8007CFAC();
            func_8007EB68();
            break;
        case 22:
            func_8007BFA8();
            func_8007CC80();
            func_8007EB68();
            break;
        case 18:
        case 19:
        case 20:
        case 21:
            func_8007BFA8();
            func_8007CC80();
            func_8007EB68();
            func_8007E874();
            break;
        case 31:
            func_8007BFA8();
            func_8007C338();
            func_8007EB68();
            break;
        default:
            func_8007BFA8();
            func_8007EB68();
            break;
        }
    }
    func_8007E7F4();
    D_8009BD10 = 0;
    D_8009BD0C = 0;
    D_8009BD14 = 0;
}

void func_8007AF3C(void) {
    u16 *t;

    if (D_80099174 & 8) {
        func_8007CA70();
    }
    if (D_80099174 & 0x10) {
        func_8007CE50();
    }
    if (D_80099174 & 0x700) {
        func_8007D4D0();
    }
    if (D_80099174 & 0x1000) {
        func_8007B330();
    }
    if (D_80099174 & 0x8000) {
        func_8007D5EC();
    }
    if (D_80099174 & 0x100000) {
        func_8007D2F4();
    }
    func_8007D9DC();
    if (D_8005A618 & 0x400) {
        FntPrint("   HEALTH %2d   HEALTH %2d              HEALTH %2d    HEALTH %2d\n",
                 Heroes[0].hero_health, Heroes[1].hero_health, Heroes[2].hero_health,
                 Heroes[3].hero_health);
        t = D_8009D524;
        FntPrint("   LAST %d.%2d   LAST %d.%2d              LAST %d.%2d    LAST %d.%2d\n",
                 t[0] >> 8, (t[0] & 0xFF) * 100 / 256, t[1] >> 8, (t[1] & 0xFF) * 100 / 256,
                 t[2] >> 8, (t[2] & 0xFF) * 100 / 256, t[3] >> 8, (t[3] & 0xFF) * 100 / 256);
    }
}

void func_8007B120(void) {
    D_8009A97C = EngineTemplate;
    D_8009A97C.flags = 0x10008000;
    D_8009A97C.unk10.x = 0x82;
    D_8009A97C.unk10.y = 0x8C;
    D_8009A97C.unk10.z = 0;
    func_8001D3B0(&D_8009A97C, 1, &D_8009AAB4);
    func_80095F48((s32 *)&D_8009AAB4, -0x30, 0, D_80099200, 0);
    D_8009A97C.unk5D = 0;
    D_8009A97C.unk5C = 1;
    D_8009AAB4.flags &= ~0x10000000;
    D_8009AA18 = EngineTemplate;
    D_8009AA18.flags = 0x10008000;
    D_8009AA18.unk10.x = 0x1E0;
    D_8009AA18.unk10.y = 0x8C;
    D_8009AA18.unk10.z = 0;
    func_8001D3B0(&D_8009AA18, 1, &D_8009AB5C);
    func_80095F48((s32 *)&D_8009AB5C, -0x30, 0, D_80099204, 0);
    D_8009AA18.unk5C = 1;
    D_8009AA18.unk5D = 0;
    D_8009AE7C = 0x10;
    D_8009AE7E = 0;
    D_8009AE80 = 0;
    D_8009AE82 = 0;
    D_8009AE84 = 0;
    D_8009AB5C.flags &= ~0x10000000;
    D_80099174 |= 0x1000;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007B330);

void func_8007B7FC(u32 a, u32 b) {
    u32 v = a;
    u32 w = b;
    s32 i;
    s32 d;
    u32 t;

    if ((v & 0xFFFF) == 0xFFFF) {
        v = (u32)D_80099178 >> 8;
    }
    if ((w & 0xFFFF) == 0xFFFF) {
        w = D_8009917C;
    }
    D_80099178 = (v & 0xFFFF) << 8;
    t = w | 0x400;
    D_8009917C = t;
    D_8009AD54[0].c = 0x10008000;
    D_8009AD54[0].a = 0x3013;
    D_8009AD54[0].b = D_80099180[0].a;
    for (i = 1; i < 4; i++) {
        d = v & 0xF;
        if (d >= 0xA) {
            d = 0;
        }
        v = (v & 0xFFFF) >> 4;
        D_8009AD54[i].a = d + 0x3009;
        D_8009AD54[i].c = 0x10008000;
        D_8009AD54[i].b = D_80099180[i].a;
    }
}

void func_8007B8D4(void) {
    EngineClockView *v;
    u32 s;
    u32 t;
    u32 e;
    s32 sec;
    s32 f;
    s32 min;
    s32 n;
    s32 i;
    s32 d;
    s32 fl;
    GpuTextCmd *p;
    HeroInfo *h;

    if (!(D_80099174 & 1)) {
        return;
    }
    if (D_8005A644 & 0x9800) {
        return;
    }
    s = (u32)D_80099178 >> 8;
    if (D_8009917C & 0x400) {
        if (D_80099178 == 0x2000) {
            func_8002C1C0(0x6E, 0xF0);
        }
    }
    if (D_8009917C & 0x200) {
        if (func_8007E894() != 0) {
            s32 k = 100;

            for (d = 3; d >= 0; d--) {
                Heroes[d].hero_frozen = k;
            }
        } else {
            n = 1;
            if (D_8005A618 & 0x800) {
                n = 0x201;
            }
            D_8009D74C(-1, n);
            D_8009917C = 0x8000;
        }
    }
    if ((D_8009917C & 0x100) && s == 0) {
        PlaySfx(0x19C, NULL, NULL, 0x1000, 0x1E00, 0);
        if (func_8007E894() != 0) {
            D_80099178 = 0;
            D_8009917C = 0x8200;
        } else {
            n = 1;
            if (D_8005A618 & 0x800) {
                n = 0x201;
            }
            D_8009D74C(-1, n);
            D_80099178 = 0;
            D_8009917C = 0x8000;
        }
    }
    v = (EngineClockView *)D_8005A614;
    if (!(v->unk4 & 0x40) && !(v->unk10 & 0x04000000)) {
        if (D_8009917C & 0x20) {
            u32 bf;
            u32 bs;
            u32 bm;

            sec = (u32)D_80099178 >> 8;
            min = (u32)D_80099178 >> 16;
            bf = *(u8 *)&D_80099178;
            f = (bf >> 4) * 10 + (bf & 0xF);
            bs = (u8)sec;
            sec = (bs >> 4) * 10 + (bs & 0xF);
            bm = (u8)min;
            min = (bm >> 4) * 10 + (bm & 0xF);
            f += 2;
            if (v->unk21 < f) {
                sec++;
                f -= v->unk21;
            }
            if (sec >= 60) {
                min++;
                sec -= 60;
            }
            if (min >= 60) {
                min -= 60;
            }
            e = (f / 10) * 16 + f % 10;
            sec = (sec / 10) * 16 + sec % 10;
            min = (min / 10) * 16 + min % 10;
            D_80099178 = ((min & 0xFF) << 16) + ((sec & 0xFF) << 8) + (e & 0xFF);
        }
        if (D_8009917C & 0x40) {
            s32 min;
            u32 bf;
            u32 bs;
            u32 m;

            sec = (u32)D_80099178 >> 8;
            min = (u32)D_80099178 >> 16;
            bf = *(u8 *)&D_80099178;
            f = (bf >> 4) * 10 + (bf & 0xF);
            bs = (u8)sec;
            sec = (bs >> 4) * 10 + (bs & 0xF);
            m = (u8)min;
            min = (m >> 4) * 10 + (m & 0xF);
            f -= 2;
            if (f < 0) {
                sec--;
                f += ((EngineClockView *)D_8005A614)->unk21;
            }
            if (sec < 0) {
                min--;
                sec += 60;
            }
            if (min < 0) {
                min += 60;
            }
            e = (f / 10) * 16 + f % 10;
            sec = (sec / 10) * 16 + sec % 10;
            min = (min / 10) * 16 + min % 10;
            D_80099178 = ((min & 0xFF) << 16) + ((sec & 0xFF) << 8) + (e & 0xFF);
        }
    }
    if (D_8009917C & 0x80) {
        if (s < 6) {
            if ((u16)D_8009AE76 != s) {
                PlaySfx(0x19B, NULL, NULL, 0x1000, 0x1E00, 0);
                D_8009AE76 = s;
            }
        } else {
            D_8009AE76 = 99;
        }
    }
    fl = D_8009917C;
    do {
        if (!(fl & 0x10)) {
            goto join;
        }
        p = D_8009AD54;
        t = p->c & ~0x8000;
        p->c = t;
    } while (0);
    if (!(fl & 2) && (u32)(D_80099178 & 0x70) < 0x1D) {
        do {
            t |= 0x8000;
            D_8009AD54->c = t;
            break;
        } while (1);
    }
join:
    f = 4;
    if (D_8009917C & 4) {
        f = 3;
    }
    for (i = 1; i < f; i++) {
        d = s & 0xF;
        if (d >= 10) {
            d = 0;
        }
        D_8009AD54[i].a = d + 0x3009;
        s >>= 4;
    }
}

void func_8007BE84(void) {
    DigitEnt *e;
    s32 v;
    s32 n;
    s32 i;

    v = D_8004E0D0;
    D_800996E0[0].flags = 0x10008000;
    D_800996E0[0].cmd = 0x3014;
    for (i = 1; i < 4; i++) {
        n = v & 0xF;
        if (n >= 10) {
            n = 0;
        }
        v >>= 4;
        D_800996E0[i].cmd = n + 0x3009;
        D_800996E0[i].flags = 0x10008000;
    }
    n = 3;
    i = 3;
    do {
        e = &D_800996E0[i];
        if (e->cmd == 0x3009) {
            n--;
            e->flags &= 0xFFFF7FFF;
        } else {
            i = 0;
        }
        i--;
    } while (i >= 2);
    i = 1;
    for (; i < 4; i++) {
        D_800996E0[i].arg = *(u16 *)((u8 *)D_80099198 + n * 16 + i * 4);
    }
    D_800996E0[0].arg = *(u16 *)(n * 16 + (u8 *)D_80099198);
}

void func_8007BFA8(void) {
    s32 i;
    s32 j;
    subengine *sub;

    for (i = 0; i < 4; i++) {
        ((EXTengine **)&D_8009970C)[i] = &D_80099290[i];
        sub = D_8009971C[i];
        func_8001D3B0(((EXTengine **)&D_8009970C)[i], 7, sub);
        for (j = 0; j < 7; j++) {
            sub->unk6C = D_80053074;
            sub->flags = 0x10000000;
            sub++;
        }
        sub = D_8009971C[i];
        sub->unk6C = D_80053074;
        ((EXTengine **)&D_8009970C)[i]->flags = 0x10008000;
        sub->flags = 0x10008000;
        ((EXTengine **)&D_8009970C)[i]->unk10.x = D_800991E0[D_800991D8[i]][0];
        ((EXTengine **)&D_8009970C)[i]->unk10.y = D_800991E0[D_800991D8[i]][1];
        ((EXTengine **)&D_8009970C)[i]->unk10.z = 0;
        sub->unk74 = D_80099218[Heroes[i].hero_name];
        sub = &D_8009971C[i][1];
        sub->unk6C = D_80053074;
        sub->flags = 0;
        sub->unk04 = D_80099240[i].x;
        sub->unk08 = D_80099240[i].y;
        sub->unk0C = 0;
        sub->unk74 = 0x3026;
    }
    D_80099174 |= 2;
}

void func_8007C170(void) {
    s32 i;
    s32 j;
    subengine *sub;

    for (i = 0; i < 2; i++) {
        ((EXTengine **)&D_8009970C)[i] = &D_80099290[i];
        sub = D_8009971C[i];
        func_8001D3B0(((EXTengine **)&D_8009970C)[i], 7, sub);
        for (j = 0; j < 7; j++) {
            sub->unk6C = D_80053074;
            sub->flags = 0x10000000;
            sub++;
        }
        sub = D_8009971C[i];
        sub->unk6C = D_80053074;
        ((EXTengine **)&D_8009970C)[i]->flags = 0x10008000;
        sub->flags = 0x10008000;
        ((EXTengine **)&D_8009970C)[i]->unk10.x = D_800991F0[D_800991D8[i]][0];
        ((EXTengine **)&D_8009970C)[i]->unk10.y = D_800991F0[D_800991D8[i]][1];
        ((EXTengine **)&D_8009970C)[i]->unk10.z = 0;
        sub->unk74 = D_80099218[Heroes[i].hero_name];
        sub = &D_8009971C[i][1];
        sub->unk6C = D_80053074;
        sub->flags = 0;
        sub->unk04 = D_80099240[i].x;
        sub->unk08 = D_80099240[i].y;
        sub->unk0C = 0;
        sub->unk74 = 0x3026;
    }
    D_80099174 |= 2;
}

void func_8007C338(void) {
    s32 i;
    s16 x0;
    s16 x1;
    s32 c0;
    s32 c1;
    s32 off;
    PanelPrim *b;
    PanelPrim *p;
    PanelPrim *q;
    s32 col;
    s32 yb;
    u8 *tim;
    u8 *tex;
    s16 y0;
    s16 y1;
    s32 code;
    s16 n;

    i = 0;
    tim = D_80052F5C;
    y0 = 0x37;
    c0 = 0x3FFF7F;
    c1 = 0xF4F2F;
    tex = D_8001C7FC;
    y1 = 0x43;
    code = 0x12000000;
    n = 8;
    while (1) {
        if (i >= 4) {
            break;
        }
        col = 0;
        D_8009AE88[i] = col;
        if (Heroes[i].hero_name != 9) {
            D_8009AE88[i] = (s32)&Heroes[i].hero_health;
            p = D_8009986C[i];
            p->u6C.ptr = tim;
            p->unk0 = 0x10008000;
            p->unk4 = D_80099250[i].a + 0x10;
            yb = D_80099250[i].b;
            p->unk74 = 0x2018;
            off = i * sizeof(D_8009986C[0]);
            b = (PanelPrim *)(off + (u8 *)&D_8009986C[0][1]);
            p->unkC = 0;
            p->unk8 = yb + 0x10;
            x0 = D_80099250[i].a + 0x1C;
            x1 = D_80099250[i].a + 0x5C;
            b->unk54 = tex;
            b->u6C.s.unk6E = y0;
            b->unk70 = 0;
            b->unk76 = y0;
            b->unk78 = 0;
            b->unk7E = y1;
            b->unk80 = 0;
            b->unk86 = y1;
            b->unk88 = 0;
            b->unk8C = c0;
            b->unk90 = c0;
            b->unk94 = c1;
            b->unk98 = c1;
            b->unk0 = code;
            b->unk68 = n;
            b->u6C.s.unk6C = x0;
            b->unk74 = x1;
            b->unk7C = x0;
            b->unk84 = x1;
            x0 = D_80099250[i].a + 0x1C;
            x1 = D_80099250[i].a + 0x5C;
            q = &D_8009986C[i][2];
            q->unk54 = tex;
            q->u6C.s.unk6E = y0;
            q->u6C.s.unk6C = x0;
            q->unk70 = 0;
            q->unk74 = x1;
            q->unk84 = x1;
            col = 0xBF;
            q->unk76 = y0;
            q->unk78 = 0;
            q->unk7C = x0;
            q->unk7E = y1;
            q->unk80 = 0;
            q->unk86 = y1;
            q->unk88 = 0;
            q->unk8C = col;
            q->unk90 = col;
            q->unk94 = col;
            q->unk98 = col;
            q->unk0 = code;
            q->unk68 = n;
        }
        i++;
    }
    D_80099174 |= 8;
}

void func_8007C514(void) {
    s32 i;
    s16 x0;
    s16 x1;
    s32 c0;
    s32 c1;
    s32 off;
    PanelPrim *b;
    PanelPrim *p;
    PanelPrim *q;
    s32 col;
    s32 yb;
    u8 *tim;
    u8 *tex;
    s16 y0;
    s16 y1;
    s32 code;
    s16 n;

    i = 0;
    tim = D_80052F5C;
    y0 = 0x37;
    c0 = 0x3FFF7F;
    c1 = 0xF4F2F;
    tex = D_8001C7FC;
    y1 = 0x43;
    code = 0x12000000;
    n = 8;
    while (1) {
        if (i >= 4) {
            break;
        }
        col = 0;
        D_8009AE88[i] = col;
        if (Heroes[i].hero_name != 9) {
            D_8009AE88[i] = (s32)&Heroes[i].hero_health;
            p = D_8009986C[i];
            p->u6C.ptr = tim;
            p->unk0 = 0x10008000;
            p->unk4 = D_80099250[i].a;
            yb = D_80099250[i].b;
            p->unk74 = 0x2018;
            off = i * sizeof(D_8009986C[0]);
            b = (PanelPrim *)(off + (u8 *)&D_8009986C[0][1]);
            p->unkC = 0;
            p->unk8 = yb + 0x10;
            x0 = D_80099250[i].a + 8;
            x1 = D_80099250[i].a + 0x48;
            b->unk54 = tex;
            b->u6C.s.unk6E = y0;
            b->unk70 = 0;
            b->unk76 = y0;
            b->unk78 = 0;
            b->unk7E = y1;
            b->unk80 = 0;
            b->unk86 = y1;
            b->unk88 = 0;
            b->unk8C = c0;
            b->unk90 = c0;
            b->unk94 = c1;
            b->unk98 = c1;
            b->unk0 = code;
            b->unk68 = n;
            b->u6C.s.unk6C = x0;
            b->unk74 = x1;
            b->unk7C = x0;
            b->unk84 = x1;
            x0 = D_80099250[i].a + 8;
            x1 = D_80099250[i].a + 0x48;
            q = &D_8009986C[i][2];
            q->unk54 = tex;
            q->u6C.s.unk6E = y0;
            q->u6C.s.unk6C = x0;
            q->unk70 = 0;
            q->unk74 = x1;
            q->unk84 = x1;
            col = 0xBF;
            q->unk76 = y0;
            q->unk78 = 0;
            q->unk7C = x0;
            q->unk7E = y1;
            q->unk80 = 0;
            q->unk86 = y1;
            q->unk88 = 0;
            q->unk8C = col;
            q->unk90 = col;
            q->unk94 = col;
            q->unk98 = col;
            q->unk0 = code;
            q->unk68 = n;
        }
        i++;
    }
    D_80099174 |= 8;
}

void func_8007C6EC(void) {
    PanelPrim *p;
    PanelPrim *b;
    PanelPrim *q;
    s32 x0;
    s16 x1;
    s32 y0;
    s32 y1;
    S16Pair *s;
    s32 c0;
    s32 c1;
    s32 code;
    u8 *tex;
    u8 i;

    i = 3;
    D_8009AE88[3] = (s32)&D_8009D6DC[3];
    p = D_8009986C[3];
    b = p + 1;
    p->u6C.ptr = D_80052F5C;
    p->unk0 = 0x10008000;
    p->unk74 = 0x2018;
    c0 = 0x3FFF7F;
    c1 = 0xF4F2F;
    tex = D_8001C7FC;
    y1 = 0x43;
    y0 = 0x37;
    code = 0x12000000;
    p->unkC = 0;
    b->unk8C = c0;
    b->unk90 = c0;
    b->unk54 = tex;
    b->u6C.s.unk6E = y0;
    b->unk70 = 0;
    b->unk76 = y0;
    b->unk78 = 0;
    b->unk7E = y1;
    b->unk80 = 0;
    b->unk86 = y1;
    b->unk88 = 0;
    b->unk94 = c1;
    b->unk98 = c1;
    p[1].unk0 = code;
    b->unk68 = 8;
    s = &D_80099250[i];
    q = p + 2;
    q->unk54 = tex;
    q->u6C.s.unk6E = y0;
    q->unk70 = 0;
    p->unk4 = s->a;
    p->unk8 = s->b + 0x10;
    x0 = (u16)s->a + 8;
    x1 = (u16)s->a + 0x48;
    b->u6C.s.unk6C = x0;
    b->unk74 = x1;
    b->unk7C = x0;
    b->unk84 = x1;
    q->u6C.s.unk6C = x0;
    q->unk74 = x1;
    q->unk76 = y0;
    q->unk7C = x0;
    q->unk84 = x1;
    q->unk78 = 0;
    q->unk7E = y1;
    q->unk80 = 0;
    q->unk86 = y1;
    q->unk88 = 0;
    q->unk8C = 0xBF;
    q->unk90 = 0xBF;
    q->unk94 = 0xBF;
    q->unk98 = 0xBF;
    q->unk0 = code;
    q->unk68 = 8;
    D_80099174 |= 8;
}

void func_8007C830(s32 arg0, s32 arg1) {
    s16 x0;
    s16 x1;
    s32 yb;
    PanelPrim *b;
    PanelPrim *p;
    PanelPrim *q;
    s32 col;
    u8 *tim;
    s16 y0;
    s16 y1;
    s32 code;
    s16 n;

    tim = D_80052F5C;

    D_8009AE88[2] = arg0;
    p = D_8009986C[2];
    p->u6C.ptr = tim;
    p->unk0 = 0x10008000;
    y0 = 0x37;
    y1 = 0x43;
    code = 0x12000000;
    n = 8;
    p->unk4 = D_80099250[2].a;
    yb = D_80099250[2].b;
    p->unkC = 0;
    p->unk74 = 0x2018;
    p->unk8 = yb + 0x10;
    b = p + 1;
    x0 = (u16)D_80099250[2].a + 8;
    x1 = (u16)D_80099250[2].a + 0x48;
    b->unk54 = D_8001C7FC;
    b->u6C.s.unk6C = x0;
    b->u6C.s.unk6E = y0;
    b->unk70 = 0;
    b->unk74 = x1;
    b->unk76 = y0;
    b->unk78 = 0;
    b->unk7C = x0;
    b->unk7E = y1;
    b->unk80 = 0;
    b->unk84 = x1;
    b->unk86 = y1;
    b->unk88 = 0;
    b->unk8C = 0x3FFF7F;
    b->unk90 = 0x3FFF7F;
    b->unk94 = 0xF4F2F;
    b->unk98 = 0xF4F2F;
    p[1].unk0 = code;
    b->unk68 = n;
    q = b + 1;
    q->unk54 = D_8001C7FC;
    q->u6C.s.unk6C = x0;
    q->u6C.s.unk6E = y0;
    q->unk70 = 0;
    q->unk74 = x1;
    q->unk7C = x0;
    col = 0xBF;
    q->unk76 = y0;
    q->unk78 = 0;
    q->unk7E = y1;
    q->unk80 = 0;
    q->unk84 = x1;
    q->unk86 = y1;
    q->unk88 = 0;
    q->unk8C = col;
    q->unk90 = col;
    q->unk94 = col;
    q->unk98 = col;
    p[2].unk0 = code;
    q->unk68 = n;

    D_8009AE88[3] = arg1;
    p = p + 7;
    p->u6C.ptr = tim;
    p->unk0 = 0x10008000;
    p->unk4 = D_80099250[3].a;
    yb = D_80099250[3].b;
    p->unkC = 0;
    p->unk74 = 0x2018;
    p->unk8 = yb + 0x10;
    x0 = (u16)D_80099250[3].a + 8;
    x1 = (u16)D_80099250[3].a + 0x48;
    {
    PanelPrim *q;
    q = b + 7;
    q->unk54 = D_8001C7FC;
    q->u6C.s.unk6C = x0;
    q->u6C.s.unk6E = y0;
    q->unk70 = 0;
    q->unk74 = x1;
    q->unk76 = y0;
    q->unk78 = 0;
    q->unk7C = x0;
    q->unk7E = y1;
    q->unk80 = 0;
    q->unk84 = x1;
    q->unk86 = y1;
    q->unk88 = 0;
    q->unk8C = 0x3FFF7F;
    q->unk90 = 0x3FFF7F;
    q->unk94 = 0xF4F2F;
    q->unk98 = 0xF4F2F;
    b[7].unk0 = code;
    q->unk68 = n;
    }
    {
    PanelPrim *q;
    q = b + 8;
    q->unk54 = D_8001C7FC;
    q->u6C.s.unk6C = x0;
    q->u6C.s.unk6E = y0;
    q->unk70 = 0;
    q->unk74 = x1;
    q->unk76 = y0;
    q->unk78 = 0;
    q->unk7C = x0;
    q->unk7E = y1;
    q->unk80 = 0;
    q->unk84 = x1;
    q->unk86 = y1;
    q->unk88 = 0;
    q->unk8C = col;
    q->unk90 = col;
    q->unk94 = col;
    q->unk98 = col;
    b[8].unk0 = code;
    q->unk68 = n;
    }
    D_80099174 |= 8;
}

void func_8007CA70(void) {
    s32 i;
    s32 v;
    s32 sum;
    u8 *p;
    s32 t;
    subengine (*e)[7];
    s32 c;

    e = D_8009971C;
    c = D_8004E0D8;
    for (i = 0; i < 4; i++) {
        t = c >> 1;
        v = 0;
        if (D_8009AE88[i] != 0) {
            switch (((EngineGameView *)D_8005A614)->mode) {
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                e[i][2].unk74 = (t + i * 4) % 14 + 0x2018;
                v = (u16)(*(u16 *)D_8009AE88[i] / 3);
                break;
            case 0x1F:
                e[i][2].unk74 = (t + i * 4) % 14 + 0x2018;
                v = *(u16 *)D_8009AE88[i] >> 3;
                break;
            default:
                e[i][2].unk74 = (t + i * 4) % 14 + 0x2018;
                v = *(u16 *)D_8009AE88[i] * 31 / 20;
                break;
            }
        }
        if (v >= 0x20) {
            v = 0x1F;
        }
        p = D_80099914 + i * 0x498;
        sum = v * 2 + *(s16 *)(p + 0x6C);
        *(s16 *)(p + 0x74) = sum;
        *(s16 *)(p + 0x84) = sum;
        if (v != 0) {
            *(s32 *)p |= 0x8000;
        } else {
            *(s32 *)p &= 0xFFFF7FFF;
        }
        p = D_800999BC + i * 0x498;
        *(s16 *)(p + 0x6C) = sum;
        *(s16 *)(p + 0x7C) = sum;
        if (v < 0x1F) {
            *(s32 *)p |= 0x8000;
        } else {
            *(s32 *)p &= 0xFFFF7FFF;
        }
    }
}

void func_8007CC80(void) {
    s32 i;
    s16 x0;
    s16 x1;
    s32 c0;
    s32 c1;
    s32 off;
    PanelPrim *b;
    PanelPrim *p;
    PanelPrim *q;
    s32 col;
    s32 yb;
    u8 *tim;
    u8 *tex;
    s16 y0;
    s16 y1;
    s32 code;
    s16 n;

    i = 0;
    tim = D_80053074;
    y0 = 0x37;
    c0 = 0x3FFF7F;
    c1 = 0xF4F2F;
    tex = D_8001C7FC;
    y1 = 0x43;
    code = 0x12000000;
    n = 8;
    while (1) {
        if (i >= 4) {
            break;
        }
        col = 0;
        D_8009AE88[i] = col;
        if (Heroes[i].hero_name != 9) {
            p = D_8009986C[i];
            p->u6C.ptr = tim;
            p->unk0 = 0x10008000;
            p->unk4 = D_80099250[i].a - 0xE;
            yb = D_80099250[i].b;
            p->unk74 = 0x3021;
            off = i * sizeof(D_8009986C[0]);
            b = (PanelPrim *)(off + (u8 *)&D_8009986C[0][1]);
            p->unkC = 0;
            p->unk8 = yb - 0x10;
            x0 = D_80099250[3].a + 0x10;
            x1 = D_80099250[3].a + 0x50;
            b->unk54 = tex;
            b->u6C.s.unk6E = y0;
            b->unk70 = 0;
            b->unk76 = y0;
            b->unk78 = 0;
            b->unk7E = y1;
            b->unk80 = 0;
            b->unk86 = y1;
            b->unk88 = 0;
            b->unk8C = c0;
            b->unk90 = c0;
            b->unk94 = c1;
            b->unk98 = c1;
            b->unk0 = code;
            b->unk68 = n;
            b->u6C.s.unk6C = x0;
            b->unk74 = x1;
            b->unk7C = x0;
            b->unk84 = x1;
            x0 = D_80099250[3].a + 0x10;
            x1 = D_80099250[3].a + 0x50;
            q = &D_8009986C[i][2];
            q->unk54 = tex;
            q->u6C.s.unk6E = y0;
            q->unk70 = 0;
            q->u6C.s.unk6C = x0;
            q->unk74 = x1;
            q->unk84 = x1;
            col = 0xBF;
            q->unk76 = y0;
            q->unk78 = 0;
            q->unk7C = x0;
            q->unk7E = y1;
            q->unk80 = 0;
            q->unk86 = y1;
            q->unk88 = 0;
            q->unk8C = col;
            q->unk90 = col;
            q->unk94 = col;
            q->unk98 = col;
            q->unk0 = code;
            q->unk68 = n;
        }
        i++;
    }
    D_80099174 |= 0x10;
}

void func_8007CE50(void) {
    s32 i;
    s32 off;
    HeroInfo *h;
    s32 v;
    s32 sum;
    u8 *e;
    u8 *p;

    i = 0;
    off = i;
    h = Heroes;
loop:
    if (i < 4) {
        if (h->hero_name != 9) {
            if (D_8005A648 < 0x12) {
                v = 0;
            } else {
                if (D_8005A648 >= 0x17) {
                    if (D_8005A648 >= 0x1B) {
                        v = 0;
                    } else {
                        e = FindExtension(h->hero, 0x10000000);
                        if (e != NULL) {
                            v = *(s32 *)(e + 0x58) >> 10;
                        } else {
                            v = 0;
                        }
                    }
                } else {
                    e = FindExtension(h->hero, 0x20000000);
                    if (e != NULL) {
                        v = *(s32 *)(e + 0x20) >> 2;
                    } else {
                        v = 0;
                    }
                }
            }
            if (v >= 0x40) {
                v = 0x3F;
            }
            p = D_80099914 + off;
            sum = v + *(s16 *)(p + 0x6C);
            *(s16 *)(p + 0x74) = sum;
            *(s16 *)(p + 0x84) = sum;
            if (v != 0) {
                *(s32 *)p |= 0x8000;
            } else {
                *(s32 *)p &= 0xFFFF7FFF;
            }
            p = D_800999BC + off;
            *(s16 *)(p + 0x6C) = sum;
            *(s16 *)(p + 0x7C) = sum;
            if (v < 0x3F) {
                *(s32 *)p |= 0x8000;
            } else {
                *(s32 *)p &= 0xFFFF7FFF;
            }
        }
        off += 0x498;
        h++;
        i++;
        goto loop;
    }
}

void func_8007CFAC(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            D_8009AD74[i].a = 0;
            D_8009AD74[i].b = D_80099260[i].a;
            D_8009AD74[i].c = D_80099260[i].b;
            D_8009AD74[i].d = 1;
        } else {
            D_8009AD74[i].d = 0;
        }
    }
    D_80099174 |= 0x400;
}

void func_8007D034(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            D_8009AD74[i].a = 0;
            D_8009AD74[i].b = D_80099270[i].a;
            D_8009AD74[i].c = D_80099270[i].b;
            D_8009AD74[i].d = 1;
        } else {
            D_8009AD74[i].d = 0;
        }
    }
    D_80099174 |= 0x200;
}

void func_8007D0BC(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            D_8009AD74[i].a = 0;
            D_8009AD74[i].b = D_80099280[i].a;
            D_8009AD74[i].c = D_80099280[i].b;
            D_8009AD74[i].d = 1;
        } else {
            D_8009AD74[i].d = 0;
        }
    }
    D_80099174 |= 0x200;
}

void func_8007D144(void) {
    s32 t;
    u8 *g = D_8005A614;

    if (*(s32 *)(g + 0x6C) != 0) {
        t = *(s16 *)(g + 0x48);
        D_8009AD74[t].d = 1;
        D_8009AD74[t].b += 0x46;
        t = Heroes[t].hero_teammate;
        if (t != -1) {
            D_8009AD74[t].d = 0;
        }
        t = *(s16 *)(g + 0x4A);
        D_8009AD74[t].d = 1;
        D_8009AD74[t].b += 0x46;
        t = Heroes[t].hero_teammate;
        if (t != -1) {
            D_8009AD74[t].d = 0;
        }
    }
}

void func_8007D208(void) {
    s32 i;
    s32 off;
    PanelPrim *p;

    i = 0;
    while (1) {
        if (i >= 4) {
            break;
        }
        D_8009AE40[i].a = NULL;
        D_8009AE40[i].b = NULL;
        if (Heroes[i].hero_name != 9) {
            D_8009AE40[i].a = &D_800CAD5C[i];
            off = i * sizeof(D_80099A64[0]);
            p = (PanelPrim *)(off + (u8 *)&D_80099A64[0][0]);
            p->unk4 = 0x2E;
            p->unk8 = 0x38;
            p->u6C.ptr = D_8005365C;
            p->unk0 = 0x10000000;
            p->unkC = 0;
            p->unk74 = 0x3029;
            p = (PanelPrim *)(off + (u8 *)&D_80099A64[0][1]);
            D_8009AE40[i].b = &D_800CAD5C[i].unk4;
            p->unk4 = 0xA;
            p->unk8 = 0x39;
            p->u6C.ptr = D_8005365C;
            p->unk0 = 0x10000000;
            p->unkC = 0;
            p->unk74 = 0x302B;
        }
        i++;
    }
    D_80099174 |= 0x100000;
}

void func_8007D2F4(void) {
    s32 i;
    PanelPrim *p;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            p = &D_80099A64[i][0];
            switch (*(s32 *)D_8009AE40[i].a) {
            case 3:
                p->unk0 |= 0x8000;
                switch (*(s32 *)D_8009AE40[i].b) {
                case 1:
                    p->unk74 = 0x3029;
                    break;
                case 2:
                    p->unk74 = 0x302D;
                    break;
                default:
                    p->unk74 = 0x302C;
                    break;
                }
                break;
            case 2:
                p->unk0 |= 0x8000;
                switch (*(s32 *)D_8009AE40[i].b) {
                case 0:
                    p->unk0 &= ~0x8000;
                    break;
                case 1:
                    p->unk74 = 0x3030;
                    break;
                case 2:
                    p->unk74 = 0x302F;
                    break;
                default:
                    p->unk74 = 0x302E;
                    break;
                }
                break;
            default:
                p->unk0 &= ~0x8000;
                break;
            }
            p = &D_80099A64[i][1];
            switch (*D_8009AE40[i].c) {
            case 0:
                p->unk0 &= ~0x8000;
                break;
            case 1:
                p->unk74 = 0x302B;
                p->unk0 |= 0x8000;
                break;
            case 2:
                p->unk74 = 0x302A;
                p->unk0 |= 0x8000;
                break;
            default:
                p->unk74 = 0x3028;
                p->unk0 |= 0x8000;
                break;
            }
        }
    }
}

void func_8007D4D0(void) {
    U16Quad *q = D_8009AD74;
    U16Quad *end = q;
    HeroInfo *base = Heroes;
    HeroInfo *h;
    int i = 0;

    do {
        h = base + i;
        if (h->hero_score >= 0x3E8) {
            h->hero_score = 0x3E7;
        }
        if (h->hero_score < 0) {
            h->hero_score = 0;
        }
        if (!(D_80099174 & 0x1000)) {
            if (h->hero_score != h->hero_score_displayed &&
                (!(D_8004E0D8 & 3) || (D_8009917C & 0x200))) {
                if (h->hero_score_displayed < h->hero_score) {
                    h->hero_score_displayed++;
                } else {
                    h->hero_score_displayed--;
                }
            }
        }
        if ((D_80099174 & 0x2000) && h->hero_teammate != -1) {
            q->a = h->hero_score_displayed + base[h->hero_teammate].hero_score_displayed;
        } else {
            q->a = h->hero_score_displayed;
        }
        q++;
        i++;
    } while ((s32)q < (s32)end + 0x20);
}

void func_8007D5EC(void) {
    EngineGameView *g = (EngineGameView *)D_8005A614;
    EXTengine *e;

    if (!(g->unk30 & 0x8801) && *(s32 *)(D_8005A614 + 0x68) == 0 && D_8009D908 != 0) {
        if (D_8009AE78 != 0) {
            D_8009AE78 -= 1;
            return;
        }
        e = GetEngineSlot(5);
        func_80095BEC(5, D_8005A554);
        PlaySfx(0x194, NULL, NULL, 0x1000, 0x1E00, 0);
        e->unk10.y = 0x1F4;
        e->flags |= 0x10008000;
        D_80099174 = (D_80099174 & 0xFFFF7FFF) | 0x4000;
    }
}

void func_8007D6D0(void) {
    EXTengine *e;
    s32 i;
    EngineGameView *g;

    g =(EngineGameView *)D_8005A614;
    if (*(s32 *)((u8 *)g + 0x10) & 1) {
        return;
    }
    e = GetEngineSlot(5);
    if (D_80099174 & 0x10000) {
        if (e->unk10.y < 0x258) {
            e->unk10.y += 0x20;
        } else {
            D_80099174 &= ~0x4000;
        }
        return;
    }
    if (g->unk30 & 0x8801) {
        D_80099174 |= 0x10000;
        return;
    }
    if (e->unk10.y < 0x17D) {
        e->unk10.y = 0x17C;
    } else {
        e->unk10.y -= 0x10;
    }
    i = 0;
    for (;;) {
        if (i >= 4) {
            return;
        }
        if (D_8005A614[i + 0x5C] == 0 && (((PadStick *)D_80051338)[i].pressed & 0x4000)) {
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
            if (D_8005AA70[10] != 0) {
                ((void (*)(void))D_8005AA70[10])();
            }
            D_80099174 |= 0x10000;
        }
        i++;
    }
}

s16 func_8007D850(void) {
    u8 *g = D_8005A614;
    s16 r = -1;
    s16 i;
    s16 t;
    HeroInfo *h;
    HeroInfo *h2;

    if (*(s32 *)(g + 0x6C) != 0 && *(s16 *)(g + 0x34) != 0x20) {
        HeroInfo *H = Heroes;
        s16 a = *(s16 *)(g + 0x48);
        s16 b;
        h = &H[a];
        t = h->hero_teammate;
        if (t == -1) {
            t = *(u16 *)(g + 0x48);
        }
        if (g[0x60] == h->hero_name || g[0x60] == Heroes[t].hero_name) {
            return a;
        }
        b = *(s16 *)(g + 0x4A);
        h2 = &Heroes[b];
        t = h2->hero_teammate;
        if (t == -1) {
            t = *(u16 *)(g + 0x4A);
        }
        if (g[0x60] == h2->hero_name || g[0x60] == Heroes[t].hero_name) {
            return b;
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (D_8005A674 == Heroes[i].hero_name) {
                r = i;
            }
        }
    }
    return r;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ac20", D_800ADD3C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ac20", D_800ADD40);

void func_8007D9DC(void) {
    s32 key;
    u16 *p;
    s32 mode;
    s32 i;
    s32 k;
    s32 n;
    s32 used;
    s32 slot;
    s16 r;
    s32 t;
    u8 v;

    if (D_8009AE3C != 0) {
        D_8009AE3C--;
    }
    if (!(D_8005A646 & 0x100)) {
        s32 j;
        j = 0;
        key = D_8005A646 & 0xFF;
        p = D_8009AF38;
        do {
            if (key == *p) {
                mode = j;
                goto found;
            }
            j++;
            p++;
        } while (j < 7);
        mode = 0;
    } else {
        mode = 7;
    }
found:
    switch ((u16)D_8009AE70) {
    case 0:
        if ((MODEV->unk30 & 1) && !(MODEV->unk30 & 0x8000)) {
            MODEV->unk14 = 0;
            MODEV->unk10 |= 0x4000000;
            if (MODEV->unk60 == 0) {
                func_8002C1C0(100, 5);
                func_8002C29C();
                func_80082AC4();
                D_8009AE72 = -1;
                D_8009AE70 = 0xC;
                D_8009AE3C = 0x78;
            } else if (MODEV->unk32 & 0x100) {
                D_8009AE72 = func_8007D850();
                slot = MODEV->unk34;
                if (D_8009AE72 != -1) {
                    MODEV->unk168[slot] = 1;
                    func_80092F9C();
                }
                D_8009AE70 = 0x10;
                D_8009AE3C = 0x78;
            } else {
                if (MODEV->unk34 == 0x20) {
                    D_8009AE72 = func_8007D850();
                    if (D_8009AE72 != -1) {
                        MODEV->unkCC[D_8009AE72]++;
                        func_80098284();
                    }
                } else {
                    D_8009AE72 = func_8007D850();
                    if (D_8009AE72 != -1) {
                        MODEV->unkCC[D_8009AE72]++;
                        r = D_8009AE72;
                        t = Heroes[r].hero_teammate;
                        if (t != -1) {
                            MODEV->unkCC[t] = MODEV->unkCC[r];
                        }
                        func_80098284();
                    }
                }
                D_8009AE70 = 1;
                D_8009AE3C = 2;
            }
        }
        break;
    case 1:
        if (D_8009AE3C == 0) {
            D_8009AE70 = 3;
            D_8009AE3C = 0x5A;
        }
        break;
    case 3:
        if (D_8009AE3C != 0) {
            break;
        }
        D_8009AE70 = 0xD;
        D_8009AE3C = 0x1E;
        if (MODEV->unk42 == -1) {
            MODEV->unk42 = D_8009AE72;
        } else if (MODEV->unkCC[D_8009AE72] > MODEV->unkCC[MODEV->unk42]) {
            MODEV->unk42 = D_8009AE72;
        }
        printf("mode%x\n", mode);
        switch (mode) {
        case 2:
            if (MODEV->unkCC[D_8009AE72] < D_8009AE74) {
                break;
            }
            if ((MODEV->unk4 & 0x800) || (MODEV->unk30 & 0x200)) {
                MODEV->unkCC[D_8009AE72] = D_8009AE74;
                t = Heroes[D_8009AE72].hero_teammate;
                if (t != -1) {
                    MODEV->unkCC[t] = MODEV->unkCC[D_8009AE72];
                }
                break;
            }
            if (MODEV->unk6C != 0) {
                MODEV->unkA7[2] = (D_8009AE72 & 2) ? 0 : 2;
                MODEV->unkA7[0] = D_8009AE72 & 2;
                MODEV->unkA7[1] = D_8009AE72 ^ 1;
                MODEV->unkA7[3] = (D_8009AE72 & 2) ? 1 : 3;
            } else {
                n = 0;
                k = 3;
                used = 0;
                while (1) {
                    if (k < 0) {
                        break;
                    }
                    i = 0;
                    while (1) {
                        if (i >= 4) {
                            break;
                        }
                        if (MODEV->unkCC[i] == k && !((used >> i) & 1)) {
                            MODEV->unkA7[n] = i;
                            used |= 1 << i;
                            n++;
                        }
                        i++;
                    }
                    k--;
                }
            }
            D_8009AE70 = 5;
            slot = MODEV->unk34;
            if (MODEV->unk5C[D_8009AE72] == 0) {
                MODEV->unkE4[slot] = 1;
            }
            if (MODEV->unkE4[slot] != 0) {
                MODEV->unkAB[slot] = 0;
            } else {
                v = ++MODEV->unkAB[slot];
                if (v >= 4) {
                    MODEV->unkAB[slot] = 3;
                }
            }
            break;
        case 3:
            if (MODEV->unkCC[0] < D_8009AE74) {
                break;
            }
            slot = MODEV->unk34;
            if (MODEV->unk5C[D_8009AE72] == 0) {
                MODEV->unk105[slot] = 1;
            }
            D_8009AE70 = 6;
            break;
        case 4:
            if (MODEV->unkCC[0] < D_8009AE74) {
                break;
            }
            slot = MODEV->unk34;
            if (MODEV->unk5C[D_8009AE72] == 0) {
                MODEV->unk105[slot] = 2;
            }
            D_8009AE70 = 6;
            break;
        case 5:
            if (MODEV->unkCC[0] == 0) {
                break;
            }
            slot = MODEV->unk34;
            if (MODEV->unk5C[D_8009AE72] == 0) {
                MODEV->unk126[slot] = 1;
            }
            D_8009AE70 = 7;
            break;
        case 6:
            if (MODEV->unkCC[0] == 0) {
                break;
            }
            slot = MODEV->unk34;
            if (MODEV->unk5C[D_8009AE72] == 0) {
                MODEV->unk147[slot] = 1;
            }
            D_8009AE70 = 8;
            break;
        case 0:
        case 1:
            if (MODEV->unkCC[D_8009AE72] < D_8009AE74) {
                break;
            }
            if ((MODEV->unk4 & 0x800) || (MODEV->unk30 & 0x200)) {
                MODEV->unkCC[D_8009AE72] = D_8009AE74;
                t = Heroes[D_8009AE72].hero_teammate;
                if (t != -1) {
                    MODEV->unkCC[t] = MODEV->unkCC[D_8009AE72];
                }
                break;
            }
            if (MODEV->unk6C != 0) {
                MODEV->unkA7[2] = (D_8009AE72 & 2) ? 0 : 2;
                MODEV->unkA7[0] = D_8009AE72 & 2;
                MODEV->unkA7[1] = D_8009AE72 ^ 1;
                MODEV->unkA7[3] = (D_8009AE72 & 2) ? 1 : 3;
            } else {
                n = 0;
                k = 3;
                used = 0;
                while (1) {
                    if (k < 0) {
                        break;
                    }
                    i = 0;
                    while (1) {
                        if (i >= 4) {
                            break;
                        }
                        if (MODEV->unkCC[i] == k && !((used >> i) & 1)) {
                            MODEV->unkA7[n] = i;
                            used |= 1 << i;
                            n++;
                        }
                        i++;
                    }
                    k--;
                }
            }
            if (mode == 0) {
                D_8009AE70 = 4;
            } else {
                D_8009AE70 = 0xB;
                MODEV->unk9F[0] += MODEV->unkCC[0];
                MODEV->unk9F[1] += MODEV->unkCC[1];
                MODEV->unk9F[2] += MODEV->unkCC[2];
                MODEV->unk9F[3] += MODEV->unkCC[3];
            }
            break;
        }
        break;
    case 5:
        if (MODEV->unk5C[D_8009AE72] != 0) {
            D_800ADBCC = 1;
            D_8009AEEC = 0;
            D_8009AE3C = 900;
            MODEV->unk10 |= 0x21;
            PlaySfx(0x19E, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009AE70 = 9;
            break;
        }
        if (MODEV->unk34 != 0x20) {
            D_800ADBCC = 1;
            D_8009AE3C = 0x78;
        }
        D_8009AE70 = 0xB;
        break;
    case 6:
        D_800ADBCC = 1;
        D_8009AE3C = 0x78;
        D_8009AE70 = 0xB;
        break;
    case 8:
        D_800ADBCC = 1;
        D_8009AE3C = 0x78;
        D_8009AE70 = 0xB;
        break;
    case 7:
        D_800ADBCC = 1;
        D_8009AE3C = 0x78;
        D_8009AE70 = 0xB;
        break;
    case 17:
        D_8009AE3C = 0x1E;
        D_8009AE70 = 0xB;
        break;
    case 4:
        D_800ADBCC = 1;
        D_8009AEEC = 0;
        D_8009AE3C = 900;
        MODEV->unk10 |= 0x201;
        PlaySfx(0x19E, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009AE70 = 0xA;
        break;
    case 9:
        for (i = 0; i < 4; i++) {
            if (HUDPLAYERS[i]->unk10.y >= -0xFF) {
                HUDPLAYERS[i]->unk10.y -= 8;
            }
        }
        if (D_800996D4[1] >= -0xFF) {
            D_800996D4[1] -= 8;
        }
        if (D_8009A97C.unk10.y >= -0xFF) {
            D_8009A97C.unk10.y -= 8;
        }
        if (D_8009AA18.unk10.y >= -0xFF) {
            D_8009AA18.unk10.y -= 8;
        }
        if (D_8009AE3C == 0) {
            if (D_8009AEEC == 0) {
                MODEV->unk42 = -1;
                MODEV->unkCC[0] = 0;
                MODEV->unkCC[1] = 0;
                MODEV->unkCC[2] = 0;
                MODEV->unkCC[3] = 0;
                D_8009AE70 = 0xD;
            } else {
                func_80098278();
                func_8008352C();
                MODEV->unk10 &= ~1;
            }
        }
        D_8009AE3C++;
        break;
    case 10:
        for (i = 0; i < 4; i++) {
            if (HUDPLAYERS[i]->unk10.y >= -0xFF) {
                HUDPLAYERS[i]->unk10.y -= 8;
            }
        }
        if (D_800996D4[1] >= -0xFF) {
            D_800996D4[1] -= 8;
        }
        if (D_8009A97C.unk10.y >= -0xFF) {
            D_8009A97C.unk10.y -= 8;
        }
        if (D_8009AA18.unk10.y >= -0xFF) {
            D_8009AA18.unk10.y -= 8;
        }
        if (D_8009AE3C == 0) {
            switch (D_8009AEEC) {
            case 0:
                MODEV->unk42 = -1;
                MODEV->unkCC[0] = 0;
                MODEV->unkCC[1] = 0;
                MODEV->unkCC[2] = 0;
                MODEV->unkCC[3] = 0;
                D_8009AE70 = 0xD;
                MODEV->unk10 &= ~1;
                break;
            case 1:
                MODEV->unk10 |= 0x40;
                break;
            case 3:
                D_8009E5DC = 0x28;
                MODEV->unk30 = 0x8008;
                MODEV->unk10 &= ~0x21;
                break;
            case 2:
            default:
                func_80098278();
                func_8008352C();
                MODEV->unk10 &= ~1;
                break;
            }
        }
        D_8009AE3C++;
        break;
    case 12:
        {
            s32 ok = func_8008A200(-1) ^ 1;
            if (ok) {
                func_80082D90(0);
                break;
            }
        }
        switch (mode) {
        case 3:
        case 4:
        case 5:
        case 6:
            D_8009AEEC = 0;
            D_8009AE3C = 900;
            MODEV->unk10 |= 0x401;
            PlaySfx(0x19E, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009AE70 = 0xF;
            break;
        case 7:
            D_8009AEEC = 0;
            D_8009AE3C = 900;
            MODEV->unk10 |= 0x401;
            PlaySfx(0x19E, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009AE70 = 0xF;
            func_80080FBC();
            break;
        default:
            D_8009AE70 = 0xE;
            break;
        }
        break;
    case 15:
        if (D_8009AE3C == 0) {
            if (D_8009AEEC == 0) {
                MODEV->unk42 = -1;
                MODEV->unkCC[0] = 0;
                MODEV->unkCC[1] = 0;
                MODEV->unkCC[2] = 0;
                MODEV->unkCC[3] = 0;
                D_8009AE70 = 0xD;
            } else {
                func_80098278();
                func_8008352C();
                MODEV->unk10 &= ~1;
            }
        }
        D_8009AE3C++;
    case 14:
        func_80082D90(0);
    case 13:
        if (D_8009AE3C != 0) {
            break;
        }
        MODEV->unk30 |= 0x8200;
        func_80098278();
        func_8002C290();
    case 11:
    case 16:
        if (D_8009AE3C == 0) {
            func_80098278();
            func_8008352C();
        }
        break;
    }
}

void func_8007E7A4(void) {
    func_80010000(D_8005A614);
}

s32 func_8007E7C8(void) {
    s32 var_v0;

    var_v0 = D_8005A620 - D_8009BD04;
    if (var_v0 < 0) {
        var_v0 = 0;
    }
    return var_v0;
}

void func_8007E7F4(void) {
    u8 *p;
    u16 f;

    D_8009AE70 = 0;
    D_8009AE3C = 0;
    D_8009AE74 = 3;
    p = D_8005A614;
    f = *(u16 *)(p + 0x32);
    if (f & 3) {
        D_8009AE74 = *(u16 *)(p + 0x3A);
    }
    if (f & 0x20) {
        if (f & 8) {
            D_8009AE74 = 3;
        } else {
            D_8009AE74 = 2;
        }
    }
    if (D_8005A648 == 0x20) {
        D_8009AE74 = 3;
    }
}

void func_8007E874(void) {
    D_8009AE78 = 0xF0;
    D_80099174 |= 0x8000;
}

s32 func_8007E894(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_score != Heroes[i].hero_score_displayed) {
            return 1;
        }
    }
    return 0;
}

void func_8007E8D0(EngineBox *p, s16 x, s16 y, s32 a, s32 b) {
    p->unk54 = D_8001C7FC;
    p->unk6E = y;
    p->unk76 = y;
    p->unk74 = x + 0x40;
    p->unk84 = x + 0x40;
    p->unk6C = x;
    p->unk70 = 0;
    p->unk78 = 0;
    p->unk7C = x;
    p->unk7E = y + 0xC;
    p->unk80 = 0;
    p->unk86 = y + 0xC;
    p->unk88 = 0;
    p->unk8C = a;
    p->unk90 = a;
    p->unk0 = 0x12000000;
    p->unk68 = 8;
    p->unk94 = b;
    p->unk98 = b;
}

void func_8007E93C(u16 arg0, s16 arg1, s16 arg2) {
    if (D_80099174 & 2) {
        void *p = &D_80052A54[arg0 * 0x38];
        subengine *s = D_8009A04C;
        s->unk6C = p;
        s->unk74 = arg1;
        D_8009970C.a[4] = D_80099208[4];
        D_8009970C.a[5] = D_80099208[5];
        D_8009970C.a[6] = 0;
        s = (subengine *)((u8 *)s + 0x498);
        s->unk6C = p;
        s->unk74 = arg2;
        D_8009970C.b[4] = D_80099208[6];
        D_8009970C.b[5] = D_80099208[7];
        D_8009970C.b[6] = 0;
    }
}

void func_8007E9FC(u16 arg0, s16 arg1) {
    if (D_80099174 & 2) {
        D_8009A4E4.unk6C = &D_80052A54[arg0 * 0x38];
        D_8009A4E4.unk74 = arg1;
        D_8009970C.b[4] = D_80099208[2];
        D_8009970C.b[5] = D_80099208[3];
        D_8009970C.b[6] = 0;
        if (D_8005A680[0] == 0) {
            D_8009970C.c[4] = D_80099208[0];
            D_8009970C.c[5] = D_80099208[1];
            D_8009970C.c[6] = 0;
        }
    }
}

void func_8007EAC0(void) {
    D_8004E0D0 = 0;
    D_80099194 = 0;
    D_80099700[0] = 0x138;
    D_80099700[1] = 0x20;
    D_80099700[2] = 0;
    D_80099174 |= 0x20000;
}

void func_8007EB04(void) {
    D_80099174 &= ~1;
}

void func_8007EB1C(s32 arg0, s32 arg1, s32 arg2) {
    D_80099178 = 0;
    D_8009917C = 0;
    D_800996D4[0] = arg0 - 8;
    D_800996D4[1] = arg1 - 8;
    D_800996D4[2] = arg2;
    D_8009AE76 = 0x63;
    D_80099174 |= 1;
}

void func_8007EB68(void) {
    subengine *p;
    subengine *d;

    d = &D_80099500;
    D_80099170 = d;
    d->unk0C = 0x8000;
    *(s32 *)&d->unk10 = 0;
    *(s32 *)&d->unk14 = 0;
    d->unk18 = 0;
    d->unk6C = &D_8009AD94;
    p = &D_8009AD94;
    if (p != NULL) {
        *p = D_8005A830;
        p->unk6C = D_80053074;
        p->unk74 = 0x2004;
        p->flags = 0x11000000;
        p->unk04 = 0x140;
        p->unk08 = 0xB4;
        p->unk50 = 1;
        p->unk0C = 0;
        p->unk20.x = 0xA00;
        p->unk20.y = 0xA00;
        p->unk20.z = 0xA00;
        p->unk51 = 0;
        p->next = NULL;
        p->prev = NULL;
    }
}

void func_8007EC48(void) {
    s32 i;

    D_80099174 = 0;
    do {
        for (i = 0; i < 7; i++) {
            do {
            } while (0);
            D_80099290[i] = EngineTemplate;
            D_80099290[i].flags = 0;
        }
        ((s32 *)&D_8009970C)[i] = 0;
        i++;
    } while (i < 4);
    func_8007ED0C();
}

void func_8007ED0C(void) {
    EXTengine *e;

    e = &D_8009959C;
    func_8001D3B0(e, 1, &D_8009AC04);
    e->flags |= 0x10008000;
    e->sub->flags |= 0x10008000;
    e->sub->unk6C = D_80053074;
    e->sub->unk74 = 0x2022;
    e->unk5D = 0;
    e->unk5C = 1;
    e->unk2C.x = 0xE00;
    e->unk2C.y = 0xE00;
    e->unk2C.z = 0xE00;
    e++;
    func_8001D3B0(e, 1, &D_8009ACAC);
    e->flags |= 0x10008000;
    e->sub->flags |= 0x10008000;
    e->sub->unk6C = D_80053074;
    e->sub->unk74 = 0x2022;
    e->unk5D = 0;
    e->unk5C = 1;
    e->unk2C.x = 0xE00;
    e->unk2C.y = 0xE00;
    e->unk2C.z = 0xE00;
}
