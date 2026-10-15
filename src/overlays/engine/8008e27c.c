#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/8008e27c.h"
#include "bash/hero.h"
#include "bash/shell.h"

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_8008E27C);

void InitCrateExplode(NAS *arg0) {
    EXTengine *e = arg0->Engine;
    u8 *t;

    if (e != NULL) {
        t = (u8 *)D_8009E424;
        PlaySfx(*(s32 *)(t + Random(4) * 4 + 0x30), NULL, NULL, 0x1000, 0x1E00, 0);
        func_8007A524(2, (u8 *)e + 0x10, D_80052F94);
    }
    KillNAS(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_8008E5BC);

void func_8008EAC8(void) {
    s32 f;
    RECT *a;
    RECT *r;

    if (D_800AEC5C != 0) {
        func_80016F54((u8 *)D_800AEC5C);
    }
    func_80021FB0();
    func_8001D604();
    f = D_8005B690;
    a = &D_8005B698[f & 1];
    r = &D_8005B698[(f + 1) & 1];
    MoveImage2(a, r->x, r->y);
}

void func_8008EB3C(void) {
    func_80018B08(D_8009E498, D_80051640);
    if (D_800AEC5C != 0) {
        func_8009440C();
    }
    func_80022070();
}

void func_8008EB8C(void) {
    D_8009E5A4 = 0;
    D_8009E5A8 = 0;
    D_800AEC58 = 0;
    D_800AEC5C = 0;
    func_8001E984(0x20, 0x80);
    D_8009E5B0 = -1;
    D_8009E5B4 = 0;
    func_8001D640(0x200);
    func_80014388(D_80051640);
    D_800590CC = 1;
}

void func_8008EC00(void) {
    u8 *a;
    u8 *b;
    s32 func_800133FC();

    a = (u8 *)func_800133FC();
    b = func_800189F4(a, D_8005B698->w, D_8005B698->h);
    func_800110A8(a, &D_8004E0F0);
    LoadImage(D_8005B698, (u32 *)b);
    LoadImage(D_8005B698 + 1, (u32 *)b);
    DrawSync(0);
    func_800110A8(b, &D_8004E0F0);
}

void func_8008EC94(void) {
    s32 f = D_8005B690;
    RECT *a = &D_8005B698[f & 1];
    RECT *r = &D_8005B698[(f + 1) & 1];

    MoveImage2(a, r->x, r->y);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_8008ECE4);

s32 func_8008EF70(void) {
    return D_8009EDD4[D_8005A676][0];
}

void func_8008EF9C(void) {
    D_8009E5DC = func_8008EF70();
    D_8005A644 = 0x8008;
    func_8008ECE4();
}

void func_8008EFD4(void) {
    u8 *g = D_8005A614;

    if (*(u16 *)(g + 0x30) & 0x8000) {
        if (D_8009F65C == 0) {
            if (*(u16 *)(g + 0x24) != 0) {
                *(u16 *)(g + 0x24) -= 1;
                if (*(u16 *)(g + 0x24) != 0) {
                    return;
                }
            }
            func_80092AF4((s32)D_800A00DC, 0x200);
            if (!(*(u16 *)(g + 0x30) & 0x200)) {
                func_8002C284(-1);
            }
        }
    }
}

void func_8008F068(void) {
    D_8009E5DC = 0x28;
    D_8005A644 = 0x8008;
}

void func_8008F084(void) {
    EngineEntry16Head *p;
    EngineTable12 *base;
    EngineTable12 *t;
    subengine *sub;
    TrigEntry *trig;
    s32 n;
    s32 i;
    u32 mask;

    i = 0;
    trig = D_80068BD4;
    base = D_8009E7A0;
    t = base + (u8)D_8005A63C;
    n = t->unk8;
    p = t->unk4;
loop:
    if (i < n) {
        sub = GetEngineSlot(i + 3)->sub;
        sub->unk6C = (void *)p->unk8;
        sub->unk74 = p->unkC;
        if (p->unk0 == 1 || (p->unk0 == 2 && func_800B7EE4(p->unk4, D_8005A614) != 0)) {
            sub->flags |= 0x8000;
        } else {
            sub->flags &= ~0x8000;
        }
        p++;
        sub->unk10 = 0x7D;
        sub->unk12 = 0;
        sub->unk14 = 0;
        sub->unk04 = trig[(((D_8004E0D8 * 20 + (i << 10)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 128 >> 12;
        sub->unk08 = (trig[(((D_8004E0D8 * 70 + (i << 9)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
        i++;
        goto loop;
    }
    i = n;
    mask = ~0x8000;
loop2:
    if (i < 4) {
        GetEngineSlot(i + 3)->sub->flags &= mask;
        i++;
        goto loop2;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_8008F248);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_8008F41C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_8008F554);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_8008FB48);

void func_8008FC58(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_8008FC60);

void func_8008FD40(void *arg0) {
    s32 i;
    s32 n = *(s32 *)((u8 *)arg0 + 8);

    for (i = 0; i < n; i++) {
        u8 *e = *(u8 **)((u8 *)arg0 + 4);
        D_8005A614[*(s32 *)(e + i * 16 + 4) + 0xE4] = 1;
        n = *(s32 *)((u8 *)arg0 + 8);
    }
}

void func_8008FD94(void) {
    u8 *g = D_8005A614;
    s32 n;

    D_8009EB98 = 4;
    n = g[0x28];
    if (n < 6) {
        n++;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g[0x28] = n;
    }
}

void func_8008FE00(void) {
    u8 *g = D_8005A614;
    s32 n;

    D_8009EB98 = 4;
    n = g[0x28];
    if (n > 0) {
        n--;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g[0x28] = n;
    }
}

s32 func_8008FE68(s32 arg0) {
    EngineTable12 *base = D_8009E7A0;
    EngineTable12 *t;
    EngineEntry16Head *p;
    s32 n;
    s32 i;

    t = base + arg0;
    if (t->unk0 == 1) {
        return 1;
    }
    if (t->unk0 == 2) {
        return 0;
    }
    n = t->unk8;
    p = t->unk4;
    for (i = 0; i < n; i++, p++) {
        if (!func_800B7EE4(p->unk4, D_8005A614)) {
            return 0;
        }
    }
    return 1;
}

s32 func_8008FF10(void) {
    u16 *wt = D_8004DDD0;
    EngineTable12 *tab = D_8009E7A0;
    u8 *g = D_8005A614;
    EngineEntry16Head *e = tab[g[0x28]].unk4;
    s32 idx = e[g[0x29]].unk4;
    s32 **tbl;
    s32 w = wt[idx * 3] - 0x101;
    s32 *t;
    tbl = (s32 **)D_8004DDBC;
    t = tbl[*(s32 *)(g + 0x2C)];
    return *(s32 *)((u8 *)t + w * 4);
}

s32 func_8008FF94(void) {
    if (D_8005A614[0x29] < D_8009E7A0[D_8005A614[0x28]].unk8 - 1) {
        D_8005A614[0x29] = D_8005A614[0x29] + 1;
        return D_8009E7A0[D_8005A614[0x28]].unk4[D_8005A614[0x29]].unk4;
    }
    return 0x2B;
}

s32 func_80090008(void) {
    return D_8009E7A0[D_8005A614[0x28]].unk4[D_8005A614[0x29]].unk4;
}

void func_80090048(void) {
    D_8009EB98 = 0;
    func_80095BEC(0, D_8009E8B4);
    func_80095BEC(2, D_8009E7F4);
    func_80095BEC(3, D_8009EB3C);
    func_80095BEC(4, D_8009EB3C);
    func_80095BEC(5, D_8009EB3C);
    func_80095BEC(6, D_8009EB3C);
    D_8005A63C = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_800900CC);

static inline s32 func_800901B4_find(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = D_8009EDD4[0]; (u32)i < 10; i++, p += 5) {
        if (*p == arg0) {
            return i;
        }
    }
    return -1;
}

s32 *func_800901B4(s32 arg0) {
    s32 i = func_800901B4_find(arg0);

    if (i < 0) {
        return 0;
    }
    return D_8009EDD4[i];
}

s32 func_8009020C(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = D_8009EDD4[0]; (u32)i < 10; i++, p += 5) {
        if (*p == arg0) {
            return i;
        }
    }
    return -1;
}

s32 func_80090248(void) {
    EngSub90248 *p;

    if (D_8005A677 < 0 || (p = D_800BCCF4[D_8005A677].unk14, p->unk2 != 2)) {
        return -1;
    }
    return p->unk4;
}

void func_8009029C(void) {
    s32 i;
    EngEntry *e;
    EngTable *t = (EngTable *)D_8009EDD4;

    for (i = 9; i != -1; i--) {
        e = t->entries;
        while (e->a != 0) {
            e->flags &= ~8;
            e++;
        }
        t++;
    }
    D_8009EE9C.unk0 = 0;
    D_8009EE9C.unk4 = 0;
    D_8009EE9C.unk8 = 0x400;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_80090310);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_80090564);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_80090A10);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE47C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE490);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE498);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE4B4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE4FC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE500);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE504);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE554);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE594);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE5CC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE618);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE634);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE64C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE664);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE68C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE6A0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE6B4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE6EC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE760);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE7D4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE7E0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE850);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE8D4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE8E0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AE988);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEA04);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEA34);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEA64);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEA8C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEAC4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEAEC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEAF4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEAF8);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEAFC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEB00);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEB04);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEB08);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEB14);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8008e27c", D_800AEB1C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_80090E20);

void func_800917B4(void) {
    EXTengine *e;
    subengine *s;
    s32 v;

    func_80018B08(&D_8009F3E4, &D_8009F578);
    e = GetEngineSlot(8);
    s = e->sub;
    s->unk14 = D_8005B690 * 20;
    s->unk12 = 0x293;
    s->unk50 = 1;
    s->unk20.z = 0x2800;
    s->unk20.x = 0x2800;
    s->unk52 = 1;
    s->unk51 = 0;
    D_800569AC = *(s32 *)D_8009F644;
    func_8001C2C0((u8 *)e);
    func_80018B08(&D_8009F3BC, &D_8009F4C0);
    switch (D_8009F630) {
    case 0:
        if (D_80051380 & 0xF0) {
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            v = 1;
            if (D_8009F634 == 1) {
                v = 2;
            }
            D_8009F634 = v;
        }
        if (D_80051380 & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            if (D_8009F634 == 1) {
                D_8009F640 = 0;
                D_8009F630 = D_8009F634;
            } else {
                D_8009F638 = 0;
                D_8009F630 = 2;
            }
        }
        break;
    case 1:
        func_80090E20();
        break;
    case 2:
        func_80090A10();
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_8009196C);

void func_80091E58(void) {
    u8 *p = (u8 *)D_800657C8;
    u8 *pad = p + 8;

    func_80090310(pad, 0,*(s32 *)D_800657C8 == 0, 0x28, 0x1E);
    func_80090310(p + 0x19C, 1, *(s32 *)(p + 0x194) == 0, 0x140, 0x1E);
    func_80090310(p + 0x330, 2, *(s32 *)(p + 0x328) == 0, 0x28, 0x7C);
    func_80090310(p + 0x4C4, 3, *(s32 *)(p + 0x4BC) == 0, 0x140, 0x7C);
    func_80095BEC(3, &D_8009F008);
    func_80090564(pad + D_8009F3B8 * 0x194,*(s32 *)(p + D_8009F3B8 * 0x194) == 0);
}

void func_80091F54(void) {
    subengine *sub;

    D_8009F3B8 = 0;
    D_8009F4BC = 0x4D2;
    func_800920DC();
    D_8005B2B4 = 0;
    D_8009F630 = 0;
    D_8009F634 = 1;
    func_80095BEC(8, &D_8009F40C);
    sub = GetEngineSlot(8)->sub;
    if (D_8005A648 == 0x28) {
        sub->unk74 = 0x2007;
        sub->unk6C = D_800555DC;
    } else {
        sub->unk74 = 0x201F;
        sub->unk6C = D_800558EC;
    }
    func_80095BEC(1, &D_8009EEB0);
    D_8009F494 = 0;
    D_8009F640 = 0;
    D_8009F630 = 1;
}

void func_8009201C(void) {
    subengine *sub;

    D_8009F3B8 = 0;
    D_8009F4BC = 0x4D2;
    func_800920DC();
    D_8005B2B4 = 0;
    D_8009F630 = 0;
    D_8009F634 = 1;
    func_80095BEC(8, &D_8009F40C);
    sub = GetEngineSlot(8)->sub;
    if (D_8005A648 == 0x28) {
        sub->unk74 = 0x2007;
        sub->unk6C = D_800555DC;
    } else {
        sub->unk74 = 0x201F;
        sub->unk6C = D_800558EC;
    }
    func_80095BEC(1, &D_8009EEB0);
    D_8009F638 = 0;
    D_8009F630 = 2;
}

void func_800920DC(void) {
    EXTengine *eng;

    func_80091E58();
    func_80095BEC(2, &D_8009EF64);
    GetEngineSlot(2)->unk10.x = 0x28;
    GetEngineSlot(2)->unk10.y = 0x1E;
    eng = GetEngineSlot(2);
    eng->flags |= 0x10000000;
    func_80014388((u8 *)&D_8009F4C0);
    *(s32 *)((u8 *)&D_8009F4C0 + 8) = 0x5DC;
    func_80014388((u8 *)&D_8009F578);
    *(s32 *)((u8 *)&D_8009F578 + 8) = 0x400;
    *(s32 *)((u8 *)&D_8009F578 + 0x18) = 0x190;
    func_8001E588(D_8009F644, -0x200, 0);
}

void func_80092198(void) {
    EXTengine *eng;

    func_80091E58();
    func_80095BEC(2, &D_8009EF64);
    GetEngineSlot(2)->unk10.x = 0x28;
    GetEngineSlot(2)->unk10.y = 0x1E;
    eng = GetEngineSlot(2);
    eng->flags |= 0x10000000;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_80092200);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_800922FC);

s32 func_800923F0(s32 *arg0) {
    return *arg0 == 0;
}

void func_800923FC(void) {
}

void func_80092404(void) {
    func_80095A04(2);
    func_80095A04(3);
    func_80095A04(4);
    func_80095A04(5);
    func_80095A04(6);
    func_80095A04(7);
    func_80095A04(8);
    func_80017640();
    func_8001888C();
    func_800183DC();
}

void func_8009246C(void) {
    subengine *sub;

    D_8009F3B8 = 0;
    D_8009F4BC = 0x4D2;
    func_800920DC();
    D_8005B2B4 = 0;
    D_8009F630 = 0;
    D_8009F634 = 1;
    func_80095BEC(8, &D_8009F40C);
    sub = GetEngineSlot(8)->sub;
    if (D_8005A648 == 0x28) {
        sub->unk74 = 0x2007;
        sub->unk6C = D_800555DC;
    } else {
        sub->unk74 = 0x201F;
        sub->unk6C = D_800558EC;
    }
    func_80095BEC(1, &D_8009EEB0);
}

s32 func_8009251C(u8 *arg0) {
    u8 *p;
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 0x21; i++) {
        p = arg0 + i;
        if (p[0x105] >= 2) {
            n += p[0x105] - 1;
        }
    }
    return n;
}

s32 func_80092560(u8 *arg0) {
    s32 n;
    s32 i;

    n = 0;
    i = 0;
    do {
        if ((arg0 + i)[0x105] != 0) {
            n++;
        }
        i++;
    } while (i < 0x21);
    return n;
}

s32 func_80092598(u8 *arg0) {
    s32 n;
    s32 i;

    n = 0;
    i = 0;
    do {
        n += (arg0 + i)[0x126];
        i++;
    } while (i < 0x21);
    return n;
}

s32 func_800925C4(u8 *arg0) {
    s32 n;
    s32 i;

    n = 0;
    i = 0;
    do {
        n += (arg0 + i)[0x147];
        i++;
    } while (i < 0x21);
    return n;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_800925F0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_800926CC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_800928E8);

void func_800929D8(void) {
    *(s32 *)D_8009F658 = 0;
    func_8001E934(D_8009F658, 0);
    func_800928E8();
    *(s32 *)D_8009F644 = 0x1000;
    *(s32 *)(D_8009F644 + 8) = 3;
    *(s32 *)(D_8009F644 + 4) = 0x100;
    SetDispMask(1);
    D_800590CC = 1;
    func_80092AF4((s32)D_800A00DC, 0x200);
    func_8001E598(D_8009F644);
    func_8001E610(D_8009F658, D_8009F644);
    func_8001E898(D_8009F658);
    if (((ModeMgr *)D_8009F658)->cur != NULL) {
        if (((ModeMgr *)D_8009F658)->cur->exit != NULL) {
            ((ModeMgr *)D_8009F658)->cur->exit();
        }
    }
}

void func_80092AA8(GameModeDef *arg0) {
    if (((ModeMgr *)D_8009F658)->next != arg0) {
        ((void (*)(u8 *))func_8001E588)(D_8009F644);
        ((ModeMgr *)D_8009F658)->next = arg0;
    }
}

void func_80092AF4(s32 arg0, s32 arg1) {
    u8 *p = D_8009F658;

    if (*(s32 *)(p + 4) != arg0) {
        func_8001E588(D_8009F644, arg1, 0);
        *(s32 *)(p + 4) = arg0;
    }
}

void func_80092B40(void) {
    if (((ModeMgr *)D_8009F658)->cur != NULL) {
        if (((ModeMgr *)D_8009F658)->cur->exit != NULL) {
            ((ModeMgr *)D_8009F658)->cur->exit();
        }
    }
}

void func_80092B7C(void) {
    func_8001E898(D_8009F658);
}

void func_80092BA0(void) {
    func_8001E598(D_8009F644);
    func_8001E610(D_8009F658, D_8009F644);
}

void func_80092BDC(void) {
    *(s32 *)D_8009F658 = 0;
    func_8001E934(D_8009F658, 0);
    func_800928E8();
    *(s32 *)D_8009F644 = 0x1000;
    *(s32 *)(D_8009F644 + 8) = 3;
    *(s32 *)(D_8009F644 + 4) = 0x100;
    SetDispMask(1);
    D_800590CC = 1;
    func_80092AF4((s32)D_800A00DC, 0x200);
}

void func_80092C54(void) {
    func_800926CC(D_8005A614);
}

void func_80092C78(void) {
    s8 *p;
    *(s32 *)D_8005A604 = 0x46;
    p = D_8005A604;
    *(s32 *)(p + 4) = 0x64;
    *(s32 *)(p + 8) = 1;
    p[0xC] = 1;
    p[0xD] = 1;
    p[0xE] = 1;
    p[0xF] = 1;
}

void func_80092CAC(void) {
    EngineGameView *g;

    g = (EngineGameView *)D_8005A614;
    g->unk30 &= 0x7FF8;
    func_8001E588(D_8009F644, -0x300, 0);
    D_8009F8B8 = 0;
    if (g->mode == 0x24 && g->unkE0 <= 0) {
        g->unkE0 = 1;
    }
    g = (EngineGameView *)D_8005A614;
    if (func_8007A5E8((u8 *)g) >= 0x19 && g->unkE0 < 2) {
        g->unkE0 = 2;
    }
    func_80017640();
    func_80010AE8(&D_8004E0F0);
    D_800590CC = 0;
    if (D_8005A644 & 0x200) {
        func_80079260();
    } else {
        func_8002C284(1);
    }
    EngineBeginMode(1);
}

void func_80092E40(void) {
    func_80022030();
    func_8007AB28();
    func_80022504();
    func_8001E854(&D_8009F8A4);
    func_8001888C();
    func_800183DC();
    func_8008ECE4();
}

void func_80092E94(void) {
    D_8009F8B4();
    D_800569AC = *(s32 *)D_8009F644;
    func_80082590();
    func_8001E898(&D_8009F8A4);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_80092EDC);

void func_80092F9C(void) {
    D_8005A614[0x63] = 0xA;
    *(s32 *)(D_8005A614 + 0x64) = 1;
}

void func_80092FB8(void) {
    EngineGameView *a = (EngineGameView *)D_8005A614;
    EngineGameView *g;

    if (a->mode == 0x24 && a->unkE0 <= 0) {
        a->unkE0 = 1;
    }
    g = (EngineGameView *)D_8005A614;
    if (func_8007A5E8((u8 *)g) >= 0x19 && g->unkE0 < 2) {
        g->unkE0 = 2;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8008e27c", func_80093038);

void func_80093110(void) {
    func_80018B08((GfxViewport *)D_8009F6F4, (GfxCamera *)D_80051640);
    D_80056998.modelDp = 0x1000 - ((0x1000 - D_8009F8B8) * (0x1000 - *(s32 *)D_8009F644) / 4096);
}
