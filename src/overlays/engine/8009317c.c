#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/8009317c.h"
#include "bash/hero.h"
#include "bash/shell.h"

s32 func_8009317C(void) {
    EngineGameView *g;
    u8 *p;
    s32 **tab = D_8009F954;
    s32 **rp;

    g = (EngineGameView *)D_8005A614;
    rp =&tab[*(s32 *)(D_8005A614 + 0x2C)];
    p = (u8 *)(*rp)[D_8004DDD0[g->mode * 3] - 0x101];
    if (p != NULL) {
        func_800134FC(p, D_80098598);
    }
    p = (u8 *)D_8009F8E4[D_8004DDD0[g->mode * 3] - 0x101];
    if ((s32)p != D_8009F968) {
        func_800134FC(p, D_80098594);
        D_8009F968 = (s32)p;
    }
    return (s32)(D_80098594 + *(s32 *)(p + 4));
}

void func_80093270(void) {
    TrigEntry *t;
    WobbleObj *o;
    s32 f;
    s32 i;

    if (D_800A0158 >= 2 && (o = D_800A0290) != NULL) {
        t = D_80068BD4;
        o->unk51 = 0;
        o->unk12 += 0x20;
        f = D_8005B690;
        o->unk10 = *(s32 *)&t[(((f * 70) & 0xFFF) - 0x400) & 0xFFF] >> 20;
        o->unk12 = *(s32 *)&t[(((f * 140) & 0xFFF) - 0x400) & 0xFFF] >> 21;
        o->unk14 = (t[(((f * 70) & 0xFFF) - 0x400) & 0xFFF].unk2 +
                    t[(((f * 100) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 5;
        o->unk4 = *(s32 *)&t[(((f * 35) & 0xFFF) - 0x400) & 0xFFF] >> 23;
        o->unk8 = (t[(((f * 45) & 0xFFF) - 0x400) & 0xFFF].unk2 +
                   t[(((f * 145) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 5;
        o->unk52 = 1;
        o = o->unk5C;
        if (o != NULL) {
            o->unk14 += 0x17;
            i = (-f * 10) & 0xFFF;
            o->unk10 = *(s32 *)&t[(i - 0x400) & 0xFFF] >> 20;
            o->unk12 = *(s32 *)&t[i] >> 20;
            o->unk52 = 1;
        }
    }
}

void func_8009342C(void) {
    u8 *p;
    u8 *b;

    if (D_800A0158 >= 2) {
        if (D_800A02C0 == 0 && *(s32 *)((b = D_80052A54) + 0x2CAC) != 0) {
            if (D_80064C93 != -1) {
                PlaySfx(0x3C5, NULL, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x3C6, NULL, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x3C7, NULL, NULL, 0x1000, 0x1E00, 0);
                p = b + 0x2CA0;
                D_800A02C4 = 6;
                func_80095258(p);
                D_800A02C0 += 1;
                func_8001FE80(p, 0, &D_8009F9D4, &D_8009F9F4, 8, NULL);
            }
        }
        if (D_800A02C4 != 0) {
            D_800A02C4 -= 1;
            if (D_800A02C4 == 0) {
                PlaySfx(0x3C8, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        func_800220E8();
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_80093588);

static inline void func_800936C4_heap(Heap *heap, s32 p) {
    s32 size;

    size = 0x801FF000;
    size -= p;
    func_80011BF8(heap, (u8 *)p, size);
    func_80012E54();
}

void func_800936C4(void) {
    s32 start;
    EngineCfg *cfg;

    func_8002AC70();
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    func_800936C4_heap(&D_8004E0F0, (s32)D_800D7144);
    start = func_8009317C() + 7;
    start &= ~7;
    func_800936C4_heap(&D_8004E0F0, start);
    func_800235BC(&D_80098F48);
    cfg = (EngineCfg *)D_8005A5E4;
    if (cfg == NULL) {
        cfg = &D_8009F96C;
    }
    func_800274FC((s32 *)cfg);
    ((void (*)(u8 *))func_800276A4)(D_800637A8 == 0 ? D_8005B72C : D_8005B744);
    D_8005A614[0x60] = 0;
    D_8005A614[0x61] = 0;
    func_80028774(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    func_80017164(cfg->pools);
    func_800170D4();
    func_800222FC();
    func_8001D5D0();
    D_800A0158 = 0;
    if (((D5A614View *)D_8005A614)->mode == 0x28 && D_800590CC != 0) {
        ((ModeMgr *)D_8009F658)->flags = 0x1D;
        ((ModeMgr *)D_8009F658)->next = (GameModeDef *)D_800B9524;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_80093854);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_800939FC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_80093BE8);

void func_8009400C(void) {
    struct E { s32 a, b; u8 pad[0x30]; };
    struct G { struct E e[0x1C]; s32 x, y; } *g;
    struct E *e;
    u16 i = D_800A00D0[D_8005A640];
    g = (void *)D_80052A54;
    e = &g->e[i];
    g->x = e->a;
    g->y = e->b;
}

void func_80094058(void) {
    func_8001E854(D_800A015C);
}

void func_8009407C(void) {
    if (D_800A0158 >= 2) {
        func_8001E898(D_800A015C);
    }
}

void func_800940B0(s32 arg0, s32 arg1) {
    s32 i;
    u8 *p = D_8009FA14;

    for (i = 0; i < 0x60; i++) {
        if (arg0 == *(s32 *)(p + 4)) {
            func_800129D0(p, arg1);
        }
        p += 0x10;
    }
}

void func_80094120(void) {
    func_80022030();
    func_80016F54(D_800556F4);
}

void func_8009414C(void) {
    if (D_800A0158 >= 2) {
        D_8004E0D8 += 1;
        func_80018B08(D_8009F9AC, D_80051640);
        if (D_800A02C0 != 0) {
            func_8009440C();
        }
        func_80022070();
    }
}

void func_800941BC(void) {
    func_80014388(D_80051640);
    D_800A02C0 = 0;
    D_800A02C4 = 0;
}

void func_800941EC(void) {
    func_8001D174(D_800A0224);
    func_80016F54(D_800555A4);
}

void func_8009421C(void) {
    if (D_800A0158 >= 2) {
        func_80018B08((GfxViewport *)D_8009F8BC, (GfxCamera *)D_800A016C);
        D_800569AC = *(s32 *)D_8009F644;
        func_8001C298((EXTengine *)D_800A0224);
    }
}

void func_80094274(void) {
    EXTengine *eng = (EXTengine *)D_800A0224;
    subengine *sub;
    u16 *p;

    *eng = EngineTemplate;
    eng->unk5D = 0;
    eng->unk10.x = 0;
    eng->unk10.y = 0;
    eng->flags |= 0x10000000;
    func_80018F3C(eng, 2);
    sub = eng->sub;
    p = D_8009F994;
    if (sub != NULL) {
        do {
            sub->unk6C = D_800555A4;
            sub->flags = 0x01008000;
            sub->unk74 = *p;
            sub->unk04 = 0;
            sub->unk08 = 0;
            sub->unk0C = 0;
            sub = sub->next;
            p++;
            sub->unk51 = 0;
        } while (sub != NULL);
    }
}

void func_80094364(void) {
    s32 *a = D_800A00F0;
    s32 *p;

    if (D_8005A650 >= 0) {
        p = (s32 *)(D_80052A54 + D_8005A650 * 0x38);
    } else {
        p = NULL;
    }
    func_8001DEF4(a, p);
}

void func_800943B8(void) {
    if (D_8005A648 == 0x28 && D_800590CC == 0) {
        func_80016CA0(D_800556F4);
    } else {
        func_80016CA0(D_800555A4);
    }
}

void func_8009440C(void) {
    s32 h = *(s32 *)(D_80056998.cam->unk18);
    subengine *e = &D_800A0C74;
    EngineMarker *m;
    EngineMarkerPos *p;
    s32 i;
    s32 s;

    for (i = 0; i < D_800A0C70; i++) {
        m = &D_800A0C10[i];
        p = &D_800A02D8[m->idx];
        *e = D_8005A830;
        e->unk74 = m->unk0;
        e->unk6C = m->unk8;
        e->unk51 = 0;
        s = D_8004E0D8;
        e->unk10 = (s * p->x) >> 12;
        e->unk12 = (s * p->y) >> 12;
        e->unk14 = (s * p->z) >> 12;
        e->flags = 0x03008000;
        e->unk68 = p->unkC;
        gte_SetGeomScreen(h);
        func_80019F1C((u8 *)e, (u8 *)&D_80056998.cam->view, 0x03008000);
        gte_SetGeomScreen(*(s32 *)(D_80056998.cam->unk18));
    }
}

void func_800945C0(EngineStartArgs *arg0) {
    s32 rate;

    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    rate = 3;
    D_8004E0E0 = rate;
    D_80058B64 = 0x1E0000 / (60 / rate);
    D_80058B64 = arg0->unk8 * rate / 60;
    func_80014388(D_80051640);
    D_800A0D30 = 0;
    D_800A0D38 = 1;
    D_800A0C70 = 0;
    ((EngineStartArgs *)&D_800A0D2C)->unk0 = arg0->unk0;
    D_800A0D1C = arg0->unk4;
    D_800A0D20[0] = 0;
    D_800A0D24[0] = 0;
    func_8002232C();
    D_800644C0[0].id = -1;
    D_800644C0[0].handle = 0;
    D_800644C0[0].head = NULL;
    D_800644C0[1].id = -1;
    D_800644C0[1].handle = 0;
    D_800644C0[1].head = NULL;
    D_800A0D28 = 0;
    D_800654B0 = 0;
    D_800654B4 = 0;
}

void func_80094754(void) {
    EngineCfg *cfg;

    func_8002AC70();
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    D_800A0D3C = 0;
    func_8002232C();
    D_8009F968 = 0;
    func_80011BF8(&D_8004E0F0, D_80098594, 0x801FF000 - (s32)D_80098594);
    func_80012E54();
    cfg = &D_800A0314;
    func_800274FC((s32 *)cfg);
    ((void (*)(u8 *))func_800276A4)(D_800637A8 == 0 ? D_8005B72C : D_8005B744);
    func_80028774(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    func_80017164(cfg->pools);
    func_800170D4();
    func_8001E984(0x20, 0x80);
    func_800945C0((EngineStartArgs *)D_800A0C0C);
}

void func_80094860(void) {
    VabSlot *slot;
    VabSlot *base;
    s32 n;
    s32 ok;

    if (D_800A0D1C == 0) {
        return;
    }
    if (D_800654B4 != 0) {
        if (((EngineVabCue *)D_800A0D1C)->vab == 0) {
            ok = 1;
        } else {
            ok = ~D_800644C0[D_800A0D20[0]].id != 0;
        }
        if (ok) {
            if (((EngineVabCue *)D_800A0D1C)->vab == 0) {
                return;
            }
            PlaySfx(((EngineVabCue *)D_800A0D1C)->sfx, NULL, NULL, 0x1000, 0x1E00, 0);
            D_800654B4 -= 1;
            n = (D_800A0D20[0] + 1) & 1;
            D_800A0D20[0] = n;
            D_800654B0 = ((EngineVabCue *)D_800A0D1C)->unk8;
            if (D_800644C0[n].id != -1) {
                func_8002BCB0((s32 *)&D_800644C0[n]);
            }
            D_800A0D1C += sizeof(EngineVabCue);
        }
    }
    if (((EngineVabCue *)D_800A0D1C)->vab != 0) {
        base = D_800644C0;
        slot = &base[D_800A0D20[0]];
        if (slot->head == NULL && slot->handle == 0 && func_80012FFC() == 0) {
            func_8002BA10(((EngineVabCue *)D_800A0D1C)->vab, &base[D_800A0D20[0]]);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_80094A28);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_80094B38);

void func_80094D88(s32 arg0, s32 arg1) {
    D_800A0C08 = arg1;
    D_800A0C0C = arg0;
    func_8001E588(D_8009F644, 0x200, 0);
    D_8009F65C = D_800A0BF4;
}

void func_80094DD0(void) {
    func_8002BA44();
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    func_80094F60();
    func_80022030();
    func_8002BA44();
    func_8002B96C();
    if (D_800644C0[0].id != -1) {
        func_8002BCB0((s32 *)&D_800644C0[0]);
    }
    if (D_800644C0[1].id != -1) {
        func_8002BCB0((s32 *)&D_800644C0[1]);
    }
    func_8002232C();
    func_8002726C();
    D_800A0C70 = 0;
    D_800A0D3C += 1;
    D_8004E0E0 = 2;
}

void func_80094EB8(void) {
    func_80018B08(&D_800A0BCC, (GfxCamera *)D_80051640);
    D_80056998.modelDp = *(s32 *)D_8009F644;
    func_80022070();
    D_80056998.modelDp = 0x1000 - (((0x1000 - D_80058B60) * (0x1000 - D_80056998.modelDp)) >> 12);
    func_8009440C();
}

s32 func_80094F30(void) {
    s32 var_v0;
    u8 *temp_v1;

    temp_v1 = (*(s32*)((u8*)(D_800A0D2C) + 4));
    var_v0 = 0;
    if (temp_v1 != NULL) {
        var_v0 = (*(s32*)((u8*)(temp_v1) + 12)) != 0;
    }
    return var_v0;
}

void func_80094F60(void) {
    if (((*(s32*)((u8*)(D_800A0D2C) + 4)) != D_800A0D30) && (D_800A0D30 != 0)) {
        func_80016F54(D_800A0D30);
    }
}

void func_80094FA8(void) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(D_800A0D2C) + 4));
    if (temp_a0 != 0) {
        func_8001612C(temp_a0, 0);
    }
}

void func_80094FE0(void) {
    if (D_800A0C08 != 0) {
        func_80092AF4(D_800A0C08, 0x200);
        D_800A0D3C += 1;
    }
}

void func_80095020(void) {
    func_8002BA44();
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    func_80094F60();
    func_80022030();
    func_8002BA44();
    func_8002B96C();
    if (D_800644C0[0].id != -1) {
        func_8002BCB0((s32 *)&D_800644C0[0]);
    }
    if (D_800644C0[1].id != -1) {
        func_8002BCB0((s32 *)&D_800644C0[1]);
    }
    func_8002232C();
    func_8002726C();
    D_800A0C70 = 0;
    D_800A0D3C += 1;
}

s32 func_800950FC(void) {
    if (D_800A0D1C == 0) {
        return 1;
    }
    if (*(s32 *)D_800A0D1C == 0) {
        return 1;
    }
    return ~D_800644C0[D_800A0D20[0]].id != 0;
}

void func_80095160(EngineStartArgs *arg0) {
    D_800A0D1C = arg0->unk4;
    D_800A0D20[0] = 0;
    D_800A0D24[0] = 0;
    func_8002232C();
    D_800644C0[0].id = -1;
    D_800644C0[0].handle = 0;
    D_800644C0[0].head = NULL;
    D_800644C0[1].id = -1;
    D_800644C0[1].handle = 0;
    D_800644C0[1].head = NULL;
    D_800A0D28 = 0;
    D_800654B0 = 0;
    D_800654B4 = 0;
}

s32 func_800951D0(s32 arg0) {
    s32 i;
    EngineKeyed16 *e = D_800A0B60;
    u8 *g;

    for (i = 0, g = D_8005A614; i < 4; e++, i++) {
        if (arg0 == e->key) {
            s16 m = *(s16 *)(g + 0x44);
            if (m == 1) {
                return e->v[0];
            }
            if (m == 3) {
                return e->v[2];
            }
            return e->v[1];
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_80095258);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_800952F8);

void func_80095A04(s32 arg0) {
    EXTengine *eng;

    eng = &EnginePool[arg0];
    func_8001D334(eng->sub);
    eng->flags &= 0xFFFF7FFF;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_80095A60);

void func_80095BEC(s32 arg0, s32 *recs) {
    EXTengine *eng;
    subengine *subs;
    s32 *rec;
    s32 i;
    s32 n;

    if (recs != NULL) {
        eng = &EnginePool[arg0];
        *eng = EngineTemplate;
        rec = recs;
        i = 0;
        subs = D_800A13F4[arg0];
        while (*rec >= 2) {
            rec += 9;
            i++;
        }
        n = i;
        func_8001D334(eng->sub);
        *eng = EngineTemplate;
        eng->unk5C = 1;
        eng->unk5D = 0;
        eng->flags |= 0x10000000;
        if (n > 0) {
            func_8001D3B0(eng, n, subs);
            func_800952F8(eng->sub, recs);
        }
    }
}

/* Counts the 0x24-byte records before the first whose leading word is
 * below 2. */
s32 func_80095D88(s32 *rec) {
    s32 n;

    n = 0;
    while (*rec >= 2) {
        rec += 9;
        n++;
    }
    return n;
}

void func_80095DBC(void) {
    s32 i;

    for (i = 0; i < 9; i++) {
        EnginePool[i] = EngineTemplate;
    }
}

EXTengine *GetEngineSlot(s32 arg0) {
    return &EnginePool[arg0];
}

void func_80095E5C(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;
    u32 flags;
    u32 n;
    u32 m;

    p = *(u8 **)(arg0 + 0x64);
    flags = *(u32 *)(p + 0x20);
    if (flags & 1) {
        *(u16 *)(arg0 + 0x10) += *(u16 *)(p + 0xC);
    }
    if (flags & 2) {
        *(u16 *)(arg0 + 0x12) += *(u16 *)(p + 0xE);
    }
    if (flags & 4) {
        *(u16 *)(arg0 + 0x14) += *(u16 *)(p + 0x10);
    }
    if (flags & 8) {
        n = *(u32 *)(p + 0x18);
        if (n != 0) {
            m = D_8005B690 % n + 1;
            *(u16 *)(arg0 + 0x74) = *(u16 *)(p + 0x1E) + (*(u16 *)(p + 0x18) - m);
        }
    }
    *(u16 *)(arg0 + 0x52) = 1;
    func_80019F1C(arg0, arg1, arg2);
}

s32 func_80095F48(s32 *p, s32 a1, s32 a2, s32 a3, s32 a4) {
    if (p == NULL) {
        return 0;
    }
    p[0] = 0x10008000;
    p[0x54 / 4] = (s32)D_8001C448;
    p[1] = a1;
    p[2] = a2;
    p[0x6C / 4] = a3;
    p[0x7C / 4] = a4;
    return p[0x5C / 4];
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009317c", func_80095F88);
