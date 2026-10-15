#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/8007ac20.h"
#include "bash/hero.h"
#include "bash/shell.h"

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007AC20);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007B120);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007B8D4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007BE84);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007C338);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007C514);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007C6EC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007C830);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007CA70);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007CE50);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007D208);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007D2F4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007D4D0);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007D6D0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007D850);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ac20", D_800ADD3C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ac20", D_800ADD40);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007D9DC);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ac20", func_8007ED0C);
