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
        scripts_update();
    }
}

void func_80093588(void) {
    u8 *p;
    s32 *q;

    p = D_800A001C[D_8005A648];
    if (p != NULL) {
        func_80012AC4(p);
        while (func_80012FFC() != 0) {
            func_8001231C();
            func_80010AE8(&D_8004E0F0);
            func_8002BAE8();
        }
    }
    func_80017070();
    if (D_8005A648 == 0x28 && D_800590CC == 0) {
        func_80016CA0(D_800556F4);
    } else {
        func_80016CA0(D_800555A4);
    }
    D_8009F8B4 = func_80093038;
    cam_init(D_80051640);
    cam_init(D_8009F734);
    cam_init((u8 *)&D_8009F7EC);
    *(s32 *)&D_8009F734[0x14] = 0x2000;
    cam_init(D_800A016C);
    *(s32 *)&D_800A016C[8] = 0xA24;
    *(s32 *)&D_800A016C[0x14] = 0;
    q = (s32 *)D_8009F6F4;
    q[5] = 0x15E;
    q[4] = 0x140;
}

static inline void func_800936C4_heap(_mal_area *heap, s32 p) {
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
    draw_init();
    D_800A0158 = 0;
    if (((D5A614View *)D_8005A614)->mode == 0x28 && D_800590CC != 0) {
        D_8009F658.flags = 0x1D;
        D_8009F658.next_process = &D_800B9524;
    }
}

void func_80093854(void) {
    View939FC *g;
    s32 arg;
    s32 i;
    EngineHooks *tbl;
    s32 c;
    u8 *base;
    s32 j;
    s32 e;
    View939FC *a = (View939FC *)D_8005A614;

    i = 0;
    arg = D_8005A5E8[a->unk8].unk4;
    {
        s32 n = D_800A00D0[a->unk2C];
        u8 *b = D_80052A54;
        Blk38 *t = &((Blk38 *)b)[n];
        ((Blk38 *)b)[28].head.w0 = t->head.w0;
        base = b;
        ((Blk38 *)base)[28].head.w4 = t->head.w4;
    }
    g = a;
    tbl = &D_8009D6E8;
loop:
    if (i < 4) {
        do {
            c = ((u8 *)g)[i + 0x50];
            if ((u32)(c - 1) < 8) {
                {
                    u8 *p = D_8009FA14;
                    e = ((u16 *)tbl->unk4)[c] * 0x38 + (s32)base;
                    for (j = 0; j < 0x60; j++) {
                        if (e == *(s32 *)(p + 4)) {
                            func_800129D0(p, arg);
                        }
                        p += 0x10;
                    }
                }
                if (tbl->unk8 != NULL) {
                    u8 *p = D_8009FA14;
                    e = ((u16 *)tbl->unk8)[c] * 0x38 + (s32)base;
                    for (j = 0; j < 0x60; j++) {
                        if (e == *(s32 *)(p + 4)) {
                            func_800129D0(p, arg);
                        }
                        p += 0x10;
                    }
                }
            }
            i++;
        } while (0);
        goto loop;
    }
}

void load_initial_dbases(void)
{
  s32 i;
  View939FC *g;
  s32 arg;
  u8 *b;
  u8 *t;
  u8 *tg;
  s32 idx;
  s32 *src;
  s16 *p;
  s32 v;
  EngineHooks *h;
  u16 *new_var;
  u32 k;

  new_var = D_800A00D0;
  i = 0;
  tg = D_8005A614;
  idx = ((View939FC *) tg)->unk8;
  g = (View939FC *) tg;
  tg = (u8 *) D_8005A5E8;
  arg = *((s32 *) (((u8 *) (&((EngineRec32 *) tg)[idx])) + 4));
  t = D_80052A54;
  h = &D_8009D6E8;
  src = (s32 *) (&((Blk38 *) t)[new_var[g->unk2C]]);
  ((Blk38 *) t)[28].head.w0 = src[0];
  b = t;
  ((Blk38 *) b)[28].head.w4 = src[1];
  do
  {
    k = ((u8 *) g)[i + 0x50];
    if ((k - 1) < 8)
    {
      func_8001612C((u8 *) ((((u16 *) h->unk4)[k] * 0x38) + ((s32) b)), arg);
      if (h->unk8 != 0)
      {
        func_8001612C((u8 *) ((((u16 *) h->unk8)[k] * 0x38) + ((s32) b)), arg);
      }
    }
    i++;
  }
  while (i < 4);

  p = *((s16 **) D_8005A5E0);
  v = *p;
  if (v >= 0)
  {
    do
    {
      func_8001612C(D_80052A54 + (v * 0x38), 0);
      p++;
      v = *p;
    }
    while (v >= 0);
  }
  p = *((s16 **) (((u8 *) (&D_8005A5E8[D_8005A61C])) + 8));
  v = *p;
  if (v >= 0)
  {
    do
    {
      func_8001612C(D_80052A54 + (v * 0x38), 0);
      p++;
      v = *p;
    }
    while (v >= 0);
  }
}


void func_80093BE8(void) {
    EngStateView *g0;
    EngStateView *g;
    EngColor *c;
    s32 *a;
    s32 *p;

    if (D_800A0158 >= 2) {
        func_8001E610(&D_800A015C, NULL);
    }
    func_8002BA44();
    func_80011410(&D_8004E0F0);
    if (func_80012FFC() != 0 || D_8004E0F0.deferred != NULL) {
        return;
    }
    switch (D_800A0158) {
    case 0:
        if (D_8005A648 == 0x28 && D_800590CC == 0) {
            func_8001E934(&D_800A015C, &D_8009FA00);
        } else {
            func_8001E934(&D_800A015C, &D_8009F998);
        }
        func_8001E934(&D_8009F8A4, NULL);
        func_80095DBC();
        func_80093588();
        g0 = (EngStateView *)D_8005A614;
        g0->unk42 = -1;
        g0->unkCC[0] = 0;
        g0->unkCC[1] = 0;
        g0->unkCC[2] = 0;
        g0->unkCC[3] = 0;
        D_800A0158 = 1;
        g0->unk30 &= ~0x200;
        break;
    case 1:
        SetNasDefaults();
        g = (EngStateView *)D_8005A614;
        g->unk30 = 0;
        g->unk10 = 0;
        func_80023158(D_8005AA50);
        func_8001D640(D_8005AA54);
        func_8001E984(0x20, 0x20);
        process_init(&D_800A015C);
        InitialiseNasSystem();
        func_8007A8C4();
        if (g->mode != 0x28 || D_800590CC == 0) {
            func_800229FC(g->mode);
        }
        if (D_8005A648 == 0x28 && D_800590CC == 0) {
            func_8002BBB4();
            func_8002BD0C((s32)D_8004E998);
        }
        set_fade(&D_8009F644, -0x300, 0);
        D_800A0158 = 2;
        break;
    case 6:
        if (D_8005A648 != 0x28 || D_800590CC == 0) {
            func_80022140(D_8005A648, D_8004DDD0[D_8005A648 * 3]);
        }
        load_initial_dbases();
        a = map;
        if (D_8005A650 >= 0) {
            p = (s32 *)(D_80052A54 + D_8005A650 * 0x38);
        } else {
            p = NULL;
        }
        func_8001DEF4(a, p);
        D_800A0158 = 4;
        break;
    case 2:
        if (D_8005A648 == 0x28) {
            func_8002C284(1);
        } else {
            func_8002C284(0);
        }
    case 8:
        func_80093854();
        D_800A0158 = 6;
        break;
    case 5:
        func_80022140(D_8005A648, D_8004DDD0[D_8005A648 * 3]);
        D_800A0158 = 3;
        break;
    case 3:
        a = map;
        if (D_8005A650 >= 0) {
            p = (s32 *)(D_80052A54 + D_8005A650 * 0x38);
        } else {
            p = NULL;
        }
        func_8001DEF4(a, p);
        load_initial_dbases();
        D_800A0158 = 4;
        break;
    case 4:
        ((EngColor *)D_8009F6F4)->code = ((EngColorSrc *)D_80058AD0)->col->code;
        c = ((EngColorSrc *)D_80058AD0)->col;
        ((EngColor *)D_8009F6F4)->rgb = c->rgb;
        func_8007ABC4();
        func_8007A808();
        D_800A0158 = 7;
        break;
    case 7:
        func_80092AF4(&D_8009F720, 0x300);
        if (D_8005A648 != 0x28) {
            func_8002C284(-1);
        }
        D_800A0158 = 9;
        break;
    case 9:
        break;
    }
}

void realign_panel_dbase(void) {
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
    process_quit(&D_800A015C);
}

void func_8009407C(void) {
    if (D_800A0158 >= 2) {
        func_8001E898(&D_800A015C);
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
        scripts_render();
    }
}

void func_800941BC(void) {
    cam_init(D_80051640);
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
        D_800569AC = D_8009F644.amount;
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

void level_load_map(void) {
    s32 *a = map;
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
    cam_init(D_80051640);
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
    sfx_machine_bank_info *slot;
    sfx_machine_bank_info *base;
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
            machine_load_sfx(((EngineVabCue *)D_800A0D1C)->vab, &base[D_800A0D20[0]]);
        }
    }
}

void func_80094A28(void) {
    MarkObj *o;
    MarkSrc *s;
    EngineMarker *m;
    EngineMarker *b;
    s32 i;
    s32 n;
    s32 v;
    s32 t;
    MarkSrcEnt *e;

    func_8001FE80((u8 *)((MarkCtx *)D_800A0D2C)->obj, ((MarkCtx *)D_800A0D2C)->unk8, &D_800A0BA0, &D_800A0BC0, 4, &D_800A0D34);
    D_800A0C70 = 0;
    t = (s32)((MarkCtx *)D_800A0D2C)->obj;
    D_800A0D30 = t;
    o = (MarkObj *)t;
    if (o != NULL) {
        s = o->src;
        if (s != NULL) {
            i = 0;
            if (s->count > 0) {
                do {
                    e = s->ent;
                    if (((s16 *)&e[i])[-1] == 0x64) {
                        b = D_800A0C10;
                        n = D_800A0C70;
                        m = &b[n];
                        m->unk8 = o;
                        *(s32 *)m = i + 0x2001;
                        v = e[i].val;
                        D_800A0C70 = n + 1;
                        m->idx = v;
                    }
                } while (++i < s->count);
            }
        }
    }
    D_800A0BF3 = ((MarkCtx *)D_800A0D2C)->unkC;
}

void func_80094B38(void) {
    MarkObj *o;
    MarkObj *obj;
    s32 ok;

    D_8004E0D8 += 1;
    scripts_update();
    if (D_800A0D2C != NULL) {
        if (D_800A0D34 == NULL) {
            if ((s32)((MarkCtx *)D_800A0D2C)->obj != D_800A0D30 && D_800A0D30 != 0) {
                func_80016F54((u8 *)D_800A0D30);
            }
            if (((MarkCtx *)D_800A0D2C)->obj == NULL && D_800A0D3C == 0) {
                if (D_800A0C08 != 0) {
                    func_80092AF4(D_800A0C08, 0x200);
                    D_800A0D3C += 1;
                }
            } else {
                o = ((MarkCtx *)D_800A0D2C)->obj;
                ok = 0;
                if (o != NULL) {
                    ok = o->src != NULL;
                }
                if (ok) {
                    if (D_800A0D38 != 0) {
                        if (D_800A0D1C == 0 || ((EngineVabCue *)D_800A0D1C)->vab == 0) {
                            ok = 1;
                        } else {
                            ok = ~D_800644C0[D_800A0D20[0]].id != 0;
                        }
                        if (!ok) {
                            goto end;
                        }
                    }
                    func_80094A28();
                    D_800A0D2C = (MarkCtx *)D_800A0D2C + 1;
                    if (D_800A0D38 != 0) {
                        D_800654B0 = 1;
                        D_800A0D38 = 0;
                        D_800A0D28 = 1;
                        set_fade(&D_8009F644, -0x800, 0);
                    }
                }
            }
        }
    end:
        if (D_800A0D3C == 0) {
            obj = ((MarkCtx *)D_800A0D2C)->obj;
            if (obj != NULL) {
                func_8001612C((u8 *)obj, 0);
            }
        }
    }
    func_80094860();
    if ((D_80051380 & 0x4000) && D_800A0D3C == 0 && D_800A0C08 != 0) {
        func_80092AF4(D_800A0C08, 0x200);
        D_800A0D3C += 1;
    }
}

void func_80094D88(s32 arg0, process_info *arg1) {
    D_800A0C08 = arg1;
    D_800A0C0C = arg0;
    set_fade(&D_8009F644, 0x200, 0);
    D_8009F658.next_process = &D_800A0BF4;
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
    D_80056998.modelDp = D_8009F644.amount;
    scripts_render();
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

void func_80095258(u8 *arg0) {
    MarkObj *o = (MarkObj *)arg0;
    MarkSrc *s;
    EngineMarker *m;
    EngineMarker *b;
    MarkSrcEnt *e;
    s32 i;
    s32 n;
    s32 v;

    D_800A0C70 = 0;
    if (o != NULL) {
        s = o->src;
        if (s != NULL) {
            do {
            i = 0;
            if (s->count > 0) {
                do {
                    e = s->ent;
                    if (((s16 *)&e[i])[-1] == 0x64) {
                        b = D_800A0C10;
                        n = D_800A0C70;
                        m = &b[n];
                        m->unk8 = o;
                        *(s32 *)m = i + 0x2001;
                        v = e[i].val;
                        D_800A0C70 = n + 1;
                        m->idx = v;
                    }
                } while (++i < s->count);
            }
            } while (0);
        }
    }
}

void func_800952F8(subengine *sub, s32 *recs) {
    PlaceSub *s;
    PlaceRec *rec;
    s32 skip;

    s = (PlaceSub *)sub;
    rec = (PlaceRec *)recs;
    skip = 0;
    for (;;) {
        if (s == NULL || rec->type < 2) {
            break;
        }
        switch (rec->type) {
        case 13:
            switch (rec->unk20 & 0x8F0) {
            case 0x800:
                skip = 0;
                break;
            case 0x20:
                skip = ((PlaceGameView *)D_8005A614)->unk6C == 0;
                break;
            case 0x10:
                skip = ((PlaceGameView *)D_8005A614)->unk6C;
                break;
            case 0x30:
                skip = ((PlaceGameView *)D_8005A614)->unk10 ^ 1;
                skip &= 1;
                break;
            case 0x40:
                skip = ((PlaceGameView *)D_8005A614)->unk10 & 1;
                break;
            case 0x60:
                skip = 0;
                if (!(((PlaceGameView *)D_8005A614)->unk10 & 1) ||
                    !(((PlaceGameView *)D_8005A614)->unk32 & 0x10)) {
                    skip = 1;
                }
                break;
            case 0x50:
                skip = 0;
                if (((PlaceGameView *)D_8005A614)->unk10 & 1) {
                    skip = (u32)skip < (((PlaceGameView *)D_8005A614)->unk32 & 0x10);
                }
                break;
            case 0x70:
                skip = 0;
                if (!(((PlaceGameView *)D_8005A614)->unk10 & 1) ||
                    !(((PlaceGameView *)D_8005A614)->unk32 & 0xE0)) {
                    skip = 1;
                }
                break;
            }
            break;
        case 3:
        case 11:
        case 14:
            if (skip == 0) {
                if (rec->type == 14) {
                    PlaceSprite(s, rec->x, rec->y,
                                ((s32 *)rec->unk14)[((PlaceGameView *)D_8005A614)->unk2C],
                                rec->unk18);
                } else {
                    PlaceSprite(s, rec->x, rec->y, rec->unk14, rec->unk18);
                }
                if (rec->type == 11) {
                    s->z = 0;
                    s->flags &= 0xEFFFFFFF;
                }
            }
            break;
        case 4:
        case 12:
        case 16:
        case 18:
            if (skip == 0) {
                if (rec->type == 18) {
                    PlaceTmpl(s, rec->x,
                              ((s32 *)rec->unk14)[((PlaceGameView *)D_8005A614)->unk2C],
                              rec->z, rec->scale, rec->unkC);
                } else {
                    PlaceTmpl(s, rec->x, rec->y, rec->z, rec->scale, rec->unkC);
                }
                if (rec->type == 12) {
                    s->flags &= 0xEFFFFFFF;
                    s->unk84 *= 8;
                    s->unk86 *= 8;
                    s->z = rec->unkE;
                }
                if (rec->type == 16) {
                    s->unk68 = 200;
                    s->flags |= 0x2000000;
                }
            }
            break;
        case 6:
        case 7:
        case 15:
        case 17:
            if (skip == 0) {
                s->flags |= 0x8000;
                if (rec->type == 15) {
                    s->flags |= 0x2000000;
                    s->unk68 = 100;
                }
                if (rec->type == 17) {
                    s->unk68 = 90;
                    s->flags |= 0x2000000;
                }
                s->sx = rec->scale;
                s->sy = rec->scale;
                s->sz = rec->scale;
                s->unk50 = 1;
                s->unk51 = 0;
                s->unk52 = 1;
                if (rec->type != 7) {
                    s->flags |= 0x10000000;
                }
                s->x = rec->x;
                s->y = rec->y;
                s->z = rec->z;
                s->unk6C = (s32)(D_80052A54 + rec->unk1C * 0x38);
                s->unk74 = rec->unk1E;
            }
            break;
        case 8:
        case 9:
            if (skip == 0) {
                s->flags |= 0x8000;
                s->sx = rec->scale;
                s->sy = rec->scale;
                s->sz = rec->scale;
                s->unk50 = 1;
                s->unk51 = 0;
                s->unk52 = 1;
                if (rec->type != 7) {
                    s->flags |= 0x10000000;
                }
                s->x = rec->x;
                s->y = rec->y;
                s->z = rec->z;
                s->unk6C = (s32)D_80053074;
                {
                    u16 *tbl = D_80099218;
                    s32 name = Heroes[rec->unk1E].hero_name;
                    s->unk74 = tbl[name];
                }
            }
            break;
        case 10:
            if (skip == 0) {
                s->hook = func_80095E5C;
                s->rec = rec;
                s->flags |= 0x10008000;
                s->x = rec->x;
                s->y = rec->y;
                s->z = rec->z;
                s->unk51 = 0;
                s->unk50 = 1;
                s->sx = s->sy = s->sz = rec->scale;
                s->unk10 = rec->unkC;
                s->unk12 = rec->unkE;
                s->unk14 = rec->unk10;
                s->unk6C = (s32)(D_80052A54 + rec->unk1C * 0x38);
                s->unk74 = rec->unk1E;
            }
            break;
        case 5:
            if (skip == 0) {
                s->flags |= 0x10008000;
                s->x = rec->x;
                s->y = rec->y;
                s->z = rec->z;
                sub_play_script((obj_dbase *)(D_80052A54 + rec->unk1C * 0x38), rec->unk1E, (subengine *)s, rec->unk20);
            }
            break;
        case 2:
            break;
        }
        rec++;
        s = s->next;
    }
}

void func_80095A04(s32 arg0) {
    EXTengine *eng;

    eng = &EnginePool[arg0];
    kill_subs(eng->sub);
    eng->flags &= 0xFFFF7FFF;
}

void func_80095A60(s32 arg0, s32 arg1) {
    EXTengine *eng;
    subengine *subs;
    subengine *sub;
    s32 n;

    eng = &EnginePool[arg0];
    *eng = EngineTemplate;
    subs = D_800A13F4[arg0];
    n = arg1;
    kill_subs(eng->sub);
    *eng = EngineTemplate;
    eng->unk5C = 1;
    eng->unk5D = 0;
    eng->flags |= 0x10000000;
    if (n > 0) {
        func_8001D3B0(eng, n, subs);
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub->flags &= 0xFFFF7FFF;
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

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
        kill_subs(eng->sub);
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

s32 func_80095F88(u8 *p, s16 a1, s32 a2, s16 a3, s32 a4, s32 idx) {
    Tmpl1C *t;
    if (p == NULL) {
        return 0;
    }
    t = &D_800A0D44[idx];
    *(s32 *)p = t->f0 | 0x12008000;
    *(s32 *)(p + 0x54) = (s32)D_8001C690;
    if ((a1 & 0xC000) == 0x8000) {
        a2 = a2 * 0x280 / D_8005B698->w;
    }
    *(s32 *)(p + 0x6C) = t->f0;
    *(s16 *)(p + 0x70) = a1;
    *(s16 *)(p + 0x76) = a2;
    *(s16 *)(p + 0x72) = a3;
    *(s16 *)(p + 0x78) = a4;
    *(s32 *)(p + 0x7C) = t->f4;
    *(u16 *)(p + 0x84) = t->f10;
    *(u16 *)(p + 0x86) = t->f14;
    *(s32 *)(p + 0x80) = t->fC;
    *(s32 *)(p + 0x88) = t->f8;
    *(u16 *)(p + 0x68) = t->f18;
    return *(s32 *)(p + 0x5C);
}
