#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/levelload.h"
#include "bash/hero.h"
#include "bash/shell.h"

s32 func_8009317C(void) {
    EngineGameView *g;
    u8 *p;
    s32 **tab = D_8009F954;
    s32 **rp;

    g = (EngineGameView *)GameInfo;
    rp =&tab[*(s32 *)(GameInfo + 0x2C)];
    p = (u8 *)(*rp)[ModeTypes[g->mode * 3] - 0x101];
    if (p != NULL) {
        func_800134FC(p, D_80098598);
    }
    p = (u8 *)D_8009F8E4[ModeTypes[g->mode * 3] - 0x101];
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
        t = TrigTable;
        o->unk51 = 0;
        o->unk12 += 0x20;
        f = FrameCount;
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
        if (D_800A02C0 == 0 && *(s32 *)((b = HeroModelBank) + 0x2CAC) != 0) {
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

    p = D_800A001C[GameMode];
    if (p != NULL) {
        func_80012AC4(p);
        while (func_80012FFC() != 0) {
            func_8001231C();
            func_80010AE8(&MainHeap);
            func_8002BAE8();
        }
    }
    func_80017070();
    if (GameMode == 0x28 && FirstBootDone == 0) {
        func_80016CA0(D_800556F4);
    } else {
        func_80016CA0(LoadingModel);
    }
    RenderHook = func_80093038;
    cam_init(MainCamera);
    cam_init((u8 *)&MainGfxCamera);
    cam_init((u8 *)&HudCamera);
    MainGfxCamera.z = 0x2000;
    cam_init((u8 *)&LoadingCamera);
    *(s32 *)((u8 *)&LoadingCamera + 8) = 0xA24;
    LoadingCamera.z = 0;
    q = (s32 *)&GameViewport;
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
        func_80010AE8(&MainHeap);
        func_8002BAE8();
    }
    func_800936C4_heap(&MainHeap, (s32)D_800D7144);
    start = func_8009317C() + 7;
    start &= ~7;
    func_800936C4_heap(&MainHeap, start);
    func_800235BC(&D_80098F48);
    cfg = (EngineCfg *)D_8005A5E4;
    if (cfg == NULL) {
        cfg = &D_8009F96C;
    }
    func_800274FC((s32 *)cfg);
    ((void (*)(u8 *))func_800276A4)(D_800637A8 == 0 ? vmode_512_ntsc : D_8005B744);
    GameInfo[0x60] = 0;
    GameInfo[0x61] = 0;
    func_80028774(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    func_80017164(cfg->pools);
    func_800170D4();
    func_800222FC();
    draw_init();
    D_800A0158 = 0;
    if (((D5A614View *)GameInfo)->mode == 0x28 && FirstBootDone != 0) {
        ProcessControl.flags = 0x1D;
        ProcessControl.next_process = &RunGame;
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
    View939FC *a = (View939FC *)GameInfo;

    i = 0;
    arg = D_8005A5E8[a->unk8].unk4;
    {
        s32 n = D_800A00D0[a->unk2C];
        u8 *b = HeroModelBank;
        Blk38 *t = &((Blk38 *)b)[n];
        ((Blk38 *)b)[28].head.w0 = t->head.w0;
        base = b;
        ((Blk38 *)base)[28].head.w4 = t->head.w4;
    }
    g = a;
    tbl = &HeroHooks;
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
  tg = GameInfo;
  idx = ((View939FC *) tg)->unk8;
  g = (View939FC *) tg;
  tg = (u8 *) D_8005A5E8;
  arg = *((s32 *) (((u8 *) (&((EngineRec32 *) tg)[idx])) + 4));
  t = HeroModelBank;
  h = &HeroHooks;
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
      func_8001612C(HeroModelBank + (v * 0x38), 0);
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
      func_8001612C(HeroModelBank + (v * 0x38), 0);
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
        func_8001E610(&LoadingControl, NULL);
    }
    func_8002BA44();
    func_80011410(&MainHeap);
    if (func_80012FFC() != 0 || MainHeap.deferred != NULL) {
        return;
    }
    switch (D_800A0158) {
    case 0:
        if (GameMode == 0x28 && FirstBootDone == 0) {
            func_8001E934(&LoadingControl, &LoadingProcess2);
        } else {
            func_8001E934(&LoadingControl, &LoadingProcess);
        }
        func_8001E934(&MenuProcessControl, NULL);
        func_80095DBC();
        func_80093588();
        g0 = (EngStateView *)GameInfo;
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
        g = (EngStateView *)GameInfo;
        g->unk30 = 0;
        g->unk10 = 0;
        func_80023158(D_8005AA50);
        func_8001D640(D_8005AA54);
        func_8001E984(0x20, 0x20);
        process_init(&LoadingControl);
        InitialiseNasSystem();
        func_8007A8C4();
        if (g->mode != 0x28 || FirstBootDone == 0) {
            func_800229FC(g->mode);
        }
        if (GameMode == 0x28 && FirstBootDone == 0) {
            func_8002BBB4();
            func_8002BD0C((s32)D_8004E998);
        }
        set_fade(&ScreenFade, -0x300, 0);
        D_800A0158 = 2;
        break;
    case 6:
        if (GameMode != 0x28 || FirstBootDone == 0) {
            func_80022140(GameMode, ModeTypes[GameMode * 3]);
        }
        load_initial_dbases();
        a = map;
        if (D_8005A650 >= 0) {
            p = (s32 *)(HeroModelBank + D_8005A650 * 0x38);
        } else {
            p = NULL;
        }
        func_8001DEF4(a, p);
        D_800A0158 = 4;
        break;
    case 2:
        if (GameMode == 0x28) {
            func_8002C284(1);
        } else {
            func_8002C284(0);
        }
    case 8:
        func_80093854();
        D_800A0158 = 6;
        break;
    case 5:
        func_80022140(GameMode, ModeTypes[GameMode * 3]);
        D_800A0158 = 3;
        break;
    case 3:
        a = map;
        if (D_8005A650 >= 0) {
            p = (s32 *)(HeroModelBank + D_8005A650 * 0x38);
        } else {
            p = NULL;
        }
        func_8001DEF4(a, p);
        load_initial_dbases();
        D_800A0158 = 4;
        break;
    case 4:
        ((EngColor *)&GameViewport)->code = ((EngColorSrc *)MapScene)->col->code;
        c = ((EngColorSrc *)MapScene)->col;
        ((EngColor *)&GameViewport)->rgb = c->rgb;
        func_8007ABC4();
        func_8007A808();
        D_800A0158 = 7;
        break;
    case 7:
        func_80092AF4(&RetryProcess, 0x300);
        if (GameMode != 0x28) {
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
    u16 i = D_800A00D0[GameLanguage];
    g = (void *)HeroModelBank;
    e = &g->e[i];
    g->x = e->a;
    g->y = e->b;
}

void func_80094058(void) {
    process_quit(&LoadingControl);
}

void func_8009407C(void) {
    if (D_800A0158 >= 2) {
        func_8001E898(&LoadingControl);
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
        GameFrameCount += 1;
        func_80018B08(D_8009F9AC, MainCamera);
        if (D_800A02C0 != 0) {
            func_8009440C();
        }
        scripts_render();
    }
}

void func_800941BC(void) {
    cam_init(MainCamera);
    D_800A02C0 = 0;
    D_800A02C4 = 0;
}

void func_800941EC(void) {
    func_8001D174(D_800A0224);
    func_80016F54(LoadingModel);
}

void func_8009421C(void) {
    if (D_800A0158 >= 2) {
        func_80018B08(&LoadingViewport, &LoadingCamera);
        FadeAmount = ScreenFade.amount;
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
            sub->unk6C = LoadingModel;
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
        p = (s32 *)(HeroModelBank + D_8005A650 * 0x38);
    } else {
        p = NULL;
    }
    func_8001DEF4(a, p);
}

void func_800943B8(void) {
    if (GameMode == 0x28 && FirstBootDone == 0) {
        func_80016CA0(D_800556F4);
    } else {
        func_80016CA0(LoadingModel);
    }
}

void func_8009440C(void) {
    s32 h = *(s32 *)(RenderState.cam->unk18);
    subengine *e = &D_800A0C74;
    EngineMarker *m;
    EngineMarkerPos *p;
    s32 i;
    s32 s;

    for (i = 0; i < D_800A0C70; i++) {
        m = &D_800A0C10[i];
        p = &D_800A02D8[m->idx];
        *e = SubEngineTemplate;
        e->unk74 = m->unk0;
        e->unk6C = m->unk8;
        e->unk51 = 0;
        s = GameFrameCount;
        e->unk10 = (s * p->x) >> 12;
        e->unk12 = (s * p->y) >> 12;
        e->unk14 = (s * p->z) >> 12;
        e->flags = 0x03008000;
        e->unk68 = p->unkC;
        gte_SetGeomScreen(h);
        func_80019F1C((u8 *)e, (u8 *)&RenderState.cam->view, 0x03008000);
        gte_SetGeomScreen(*(s32 *)(RenderState.cam->unk18));
    }
}

void func_800945C0(EngineStartArgs *arg0) {
    s32 rate;

    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&MainHeap);
        func_8002BAE8();
    }
    rate = 3;
    D_8004E0E0 = rate;
    D_80058B64 = 0x1E0000 / (60 / rate);
    D_80058B64 = arg0->unk8 * rate / 60;
    cam_init(MainCamera);
    D_800A0D30 = 0;
    D_800A0D38 = 1;
    D_800A0C70 = 0;
    ((EngineStartArgs *)&D_800A0D2C)->unk0 = arg0->unk0;
    D_800A0D1C = arg0->unk4;
    D_800A0D20[0] = 0;
    D_800A0D24[0] = 0;
    func_8002232C();
    SfxBanks[0].id = -1;
    SfxBanks[0].handle = 0;
    SfxBanks[0].head = NULL;
    SfxBanks[1].id = -1;
    SfxBanks[1].handle = 0;
    SfxBanks[1].head = NULL;
    D_800A0D28 = 0;
    D_800654B0 = 0;
    D_800654B4 = 0;
}

void func_80094754(void) {
    EngineCfg *cfg;

    func_8002AC70();
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&MainHeap);
        func_8002BAE8();
    }
    D_800A0D3C = 0;
    func_8002232C();
    D_8009F968 = 0;
    func_80011BF8(&MainHeap, D_80098594, 0x801FF000 - (s32)D_80098594);
    func_80012E54();
    cfg = &D_800A0314;
    func_800274FC((s32 *)cfg);
    ((void (*)(u8 *))func_800276A4)(D_800637A8 == 0 ? vmode_512_ntsc : D_8005B744);
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
            ok = ~SfxBanks[D_800A0D20[0]].id != 0;
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
            if (SfxBanks[n].id != -1) {
                func_8002BCB0((s32 *)&SfxBanks[n]);
            }
            D_800A0D1C += sizeof(EngineVabCue);
        }
    }
    if (((EngineVabCue *)D_800A0D1C)->vab != 0) {
        base = SfxBanks;
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
    D_800A0BCC.isbg = ((MarkCtx *)D_800A0D2C)->unkC;
}

void func_80094B38(void) {
    MarkObj *o;
    MarkObj *obj;
    s32 ok;

    GameFrameCount += 1;
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
                            ok = ~SfxBanks[D_800A0D20[0]].id != 0;
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
                        set_fade(&ScreenFade, -0x800, 0);
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
    if ((Pads[0].pressed & 0x4000) && D_800A0D3C == 0 && D_800A0C08 != 0) {
        func_80092AF4(D_800A0C08, 0x200);
        D_800A0D3C += 1;
    }
}

void func_80094D88(s32 arg0, process_info *arg1) {
    D_800A0C08 = arg1;
    D_800A0C0C = arg0;
    set_fade(&ScreenFade, 0x200, 0);
    ProcessControl.next_process = &D_800A0BF4;
}

void func_80094DD0(void) {
    func_8002BA44();
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&MainHeap);
        func_8002BAE8();
    }
    func_80094F60();
    func_80022030();
    func_8002BA44();
    func_8002B96C();
    if (SfxBanks[0].id != -1) {
        func_8002BCB0((s32 *)&SfxBanks[0]);
    }
    if (SfxBanks[1].id != -1) {
        func_8002BCB0((s32 *)&SfxBanks[1]);
    }
    func_8002232C();
    func_8002726C();
    D_800A0C70 = 0;
    D_800A0D3C += 1;
    D_8004E0E0 = 2;
}

void func_80094EB8(void) {
    func_80018B08(&D_800A0BCC, (GfxCamera *)MainCamera);
    RenderState.modelDp = ScreenFade.amount;
    scripts_render();
    RenderState.modelDp = 0x1000 - (((0x1000 - D_80058B60) * (0x1000 - RenderState.modelDp)) >> 12);
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
        func_80010AE8(&MainHeap);
        func_8002BAE8();
    }
    func_80094F60();
    func_80022030();
    func_8002BA44();
    func_8002B96C();
    if (SfxBanks[0].id != -1) {
        func_8002BCB0((s32 *)&SfxBanks[0]);
    }
    if (SfxBanks[1].id != -1) {
        func_8002BCB0((s32 *)&SfxBanks[1]);
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
    return ~SfxBanks[D_800A0D20[0]].id != 0;
}

void func_80095160(EngineStartArgs *arg0) {
    D_800A0D1C = arg0->unk4;
    D_800A0D20[0] = 0;
    D_800A0D24[0] = 0;
    func_8002232C();
    SfxBanks[0].id = -1;
    SfxBanks[0].handle = 0;
    SfxBanks[0].head = NULL;
    SfxBanks[1].id = -1;
    SfxBanks[1].handle = 0;
    SfxBanks[1].head = NULL;
    D_800A0D28 = 0;
    D_800654B0 = 0;
    D_800654B4 = 0;
}

s32 func_800951D0(s32 arg0) {
    s32 i;
    EngineKeyed16 *e = D_800A0B60;
    u8 *g;

    for (i = 0, g = GameInfo; i < 4; e++, i++) {
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
                skip = ((PlaceGameView *)GameInfo)->unk6C == 0;
                break;
            case 0x10:
                skip = ((PlaceGameView *)GameInfo)->unk6C;
                break;
            case 0x30:
                skip = ((PlaceGameView *)GameInfo)->unk10 ^ 1;
                skip &= 1;
                break;
            case 0x40:
                skip = ((PlaceGameView *)GameInfo)->unk10 & 1;
                break;
            case 0x60:
                skip = 0;
                if (!(((PlaceGameView *)GameInfo)->unk10 & 1) ||
                    !(((PlaceGameView *)GameInfo)->unk32 & 0x10)) {
                    skip = 1;
                }
                break;
            case 0x50:
                skip = 0;
                if (((PlaceGameView *)GameInfo)->unk10 & 1) {
                    skip = (u32)skip < (((PlaceGameView *)GameInfo)->unk32 & 0x10);
                }
                break;
            case 0x70:
                skip = 0;
                if (!(((PlaceGameView *)GameInfo)->unk10 & 1) ||
                    !(((PlaceGameView *)GameInfo)->unk32 & 0xE0)) {
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
                                ((s32 *)rec->unk14)[((PlaceGameView *)GameInfo)->unk2C],
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
                              ((s32 *)rec->unk14)[((PlaceGameView *)GameInfo)->unk2C],
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
                s->unk6C = (s32)(HeroModelBank + rec->unk1C * 0x38);
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
                s->unk6C = (s32)InterfaceModel;
                {
                    u16 *tbl = IconImage;
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
                s->unk6C = (s32)(HeroModelBank + rec->unk1C * 0x38);
                s->unk74 = rec->unk1E;
            }
            break;
        case 5:
            if (skip == 0) {
                s->flags |= 0x10008000;
                s->x = rec->x;
                s->y = rec->y;
                s->z = rec->z;
                sub_play_script((obj_dbase *)(HeroModelBank + rec->unk1C * 0x38), rec->unk1E, (subengine *)s, rec->unk20);
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
            m = FrameCount % n + 1;
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
        a2 = a2 * 0x280 / ScreenRect->w;
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

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */
extern u8 D_8004E578[];
extern u8 D_8004E580[];
extern u8 D_8004E588[];
extern u8 D_8004E590[];
extern u8 D_8004E598[];
extern u8 D_8004E5A0[];
extern u8 D_8004E5A8[];
extern u8 D_8004E5B0[];
extern u8 D_8004E5B8[];
extern u8 D_8004E5C0[];
extern u8 D_8004E5C8[];
extern u8 D_8004E5D0[];
extern u8 D_8004E7C0[];
extern u8 D_8004E7C8[];
extern u8 D_8004E7E0[];
extern u8 D_8004E7E8[];
extern u8 D_8004E7F0[];
extern u8 D_8004E7F8[];
extern u8 D_8004E800[];
extern u8 D_8004E808[];
extern u8 D_8004E810[];
extern u8 D_8004E818[];
extern u8 D_8004E8B8[];
extern u8 D_8004E8C0[];
extern u8 D_8004E8C8[];
extern u8 D_8004E8D0[];
extern u8 D_8004E8D8[];
extern u8 D_8004E8E0[];
extern u8 D_8004E8E8[];
extern u8 D_8004E8F0[];
extern u8 D_8004E8F8[];
extern u8 D_8004E900[];
extern u8 D_8004E908[];
extern u8 D_8004E910[];
extern u8 D_8004E918[];
extern u8 D_8004E920[];
extern u8 D_8004E928[];
extern u8 D_8004E930[];
extern u8 D_8004E938[];
extern u8 D_8004E940[];
extern u8 D_8004E948[];
extern u8 D_8004E950[];
extern u8 D_8004E958[];
extern u8 D_8004E988[];
extern u8 D_8004F048[];
extern u8 D_8004F770[];
extern u8 D_8004F778[];
extern u8 D_8004F780[];
extern u8 D_8004F788[];
extern u8 D_8004F790[];
extern u8 D_8004F798[];
extern u8 D_8004F7A0[];
extern u8 D_8004F7A8[];
extern u8 D_8004F7B0[];
extern u8 D_8004F7B8[];
extern u8 D_8004F7C0[];
extern u8 D_8004F7C8[];
extern u8 D_8004F7D0[];
extern u8 D_8004FA38[];
extern u8 D_8004FA40[];
extern u8 D_8004FA58[];
extern u8 D_800501C0[];
extern u8 D_80055144[];
extern u8 D_80055AE4[];
extern u8 D_80055B1C[];
extern u8 D_80055B54[];
extern u8 D_80055B8C[];
extern u8 D_80055BC4[];
extern u8 D_80055BFC[];
extern u8 D_80055C34[];
extern u8 D_80055C6C[];
extern u8 D_80055CA4[];
extern u8 D_80055D14[];
extern u8 D_80055D4C[];
extern u8 D_80055D84[];
extern u8 D_80055DBC[];
extern u8 D_80055DF4[];
extern u8 D_80055E2C[];
extern u8 D_80055E64[];
extern u8 D_80055E9C[];
extern u8 D_80055ED4[];
extern u8 D_80055F0C[];
extern u8 D_80055F44[];
extern u8 D_80055F7C[];
extern u8 D_80055FB4[];
extern u8 D_80055FEC[];
extern u8 D_80056024[];
extern u8 D_8005605C[];
extern u8 D_80056094[];
extern u8 D_800560CC[];
extern u8 D_80056104[];
extern u8 D_8005613C[];
extern u8 D_80056174[];
extern u8 D_800561AC[];
extern u8 D_800561E4[];
extern u8 D_8005621C[];
extern u8 D_80056254[];
extern u8 D_8005628C[];
extern u8 D_800562C4[];
extern u8 D_800562FC[];
extern u8 D_80056334[];
extern u8 D_8005636C[];
extern u8 D_800563A4[];
extern u8 D_800563DC[];
extern u8 D_80056414[];
extern u8 D_80056484[];
extern u8 D_800564BC[];
extern u8 D_800564F4[];
extern u8 D_8005652C[];
extern u8 D_80056564[];
extern u8 D_8005659C[];
extern u8 D_800565D4[];
extern u8 D_8005660C[];
extern u8 D_80056644[];
extern u8 D_8005667C[];
extern u8 D_800566B4[];
extern u8 D_800566EC[];
extern u8 D_80056724[];
extern u8 D_8005675C[];
extern u8 D_80056794[];
extern u8 D_800567CC[];
extern u8 D_80056804[];
extern u8 D_80063BB8[];
extern u8 D_80063EC8[];
extern u8 D_80063ED8[];
extern u8 file_group_BALLS[];
extern u8 file_group_BASHMODE20[];
extern u8 file_group_BEACHBALL[];
extern u8 file_group_BONK[];
extern u8 file_group_CHASE[];
extern u8 file_group_DASH[];
extern u8 file_group_DINO[];
extern u8 file_group_DRAINS[];
extern u8 file_group_HUB[];
extern u8 file_group_JUNGLE[];
extern u8 file_group_LAVA[];
extern u8 file_group_N2O[];
extern u8 file_group_NGINBALL[];
extern u8 file_group_PAPU[];
extern u8 file_group_POGOAGOGO[];
extern u8 file_group_POGOBOUNCER[];
extern u8 file_group_POGOLOCO[];
extern u8 file_group_POGOPAINTER[];
extern u8 file_group_POLARBSS[];
extern u8 file_group_POLARCUT[];
extern u8 file_group_POLARDAY[];
extern u8 file_group_POLAREND[];
extern u8 file_group_POLARNGT[];
extern u8 file_group_RING[];
extern u8 file_group_SKYBALL[];
extern u8 file_group_SNOW[];
extern u8 file_group_SPACE[];
extern u8 file_group_TANKBOSS[];
extern u8 file_group_TANKDESERT[];
extern u8 file_group_TANKJUNGLE[];
extern u8 file_group_TANKLAST[];
extern u8 file_group_TANKMETAL[];
extern u8 file_group_TITLES[];
extern u8 file_group_TOXIC[];
extern u8 file_group_WATER[];

GfxViewport LoadingViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0xF0, 0, 0x200, -0x100, 0, 0, 0, 1 };
s32 D_8009F8E4[14] = {
    (s32)&D_8004F7A0, (s32)&D_8004F798, (s32)&D_8004F790, (s32)&D_8004F7D0, (s32)&D_8004F7C8,
    (s32)&D_8004F7C0, (s32)&D_8004F7B8, (s32)&D_8004F7B0, (s32)&D_8004F048, (s32)&D_8004F7A8,
    (s32)&D_8004F788, (s32)&D_8004F778, (s32)&D_8004F770, (s32)&D_8004F780,
};
s32 D_8009F91C[14] = {
    0, 0, (s32)&D_8004E580, (s32)&D_8004E5C8, (s32)&D_8004E5A0, (s32)&D_8004E578,
    (s32)&D_8004E598, (s32)&D_8004E5D0, (s32)&D_8004E590, (s32)&D_8004E588, (s32)&D_8004E5B0,
    (s32)&D_8004E5A8, (s32)&D_8004E5B8, (s32)&D_8004E5C0,
};
s32 *D_8009F954[5] = {
    (void *)&D_8009F91C, (void *)&D_8009F91C, (void *)&D_8009F91C, (void *)&D_8009F91C,
    (void *)&D_8009F91C,
};
s32 D_8009F968 = 0;
EngineCfg D_8009F96C = {
    { 0, 0x20, 0, 0, 0x10, 0, 0, 0, 0, 1, 0, 0 }, { 0x80, 0x200, 0x20 }, (void *)&D_80063EC8,
    (void *)&D_80063BB8, 0x20, 0x400,
};
u16 D_8009F994[2] = { 0x2001, 0x2002 };
process_info LoadingProcess = { func_80094274, func_80093270, func_8009421C, func_800941EC, 0 };
u8 D_8009F9AC[40] = {
    0, 0, 0, 0, 0x80, 2, 0xE0, 1, 0, 0, 0, 0, 0x80, 2, 0xE0, 1, 0x40, 1, 0, 0, 0xF0, 0, 0, 0,
    0, 2, 0, 0, 0xFF, 0xD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
Blk32 D_8009F9D4 = { { 0x1000, 0, 0x1000, 0, 0x1000, 0, 0, 0 } };
CamVec3 D_8009F9F4 = {0};
process_info LoadingProcess2 = { func_800941BC, func_8009342C, func_8009414C, func_80094120, 0 };
u8 D_8009FA14[1516] = {
    0x10, 6, 5, 0x80, 0x8C, 0x2A, 5, 0x80, 0, 0, 5, 0x80, 0xF8, 0xFF, 4, 0x80, 4, 6, 5, 0x80,
    0xC4, 0x2A, 5, 0x80, 0xF0, 0xFF, 4, 0x80, 0xE8, 0xFF, 4, 0x80, 0xF8, 5, 5, 0x80, 0xFC,
    0x2A, 5, 0x80, 0xE0, 0xFF, 4, 0x80, 0xD8, 0xFF, 4, 0x80, 0xEC, 5, 5, 0x80, 0x34, 0x2B, 5,
    0x80, 0xD0, 0xFF, 4, 0x80, 0xC8, 0xFF, 4, 0x80, 0xD4, 5, 5, 0x80, 0xDC, 0x2B, 5, 0x80,
    0xB0, 0xFF, 4, 0x80, 0xA8, 0xFF, 4, 0x80, 0xC8, 5, 5, 0x80, 0x14, 0x2C, 5, 0x80, 0xA0,
    0xFF, 4, 0x80, 0x98, 0xFF, 4, 0x80, 0xE0, 5, 5, 0x80, 0x6C, 0x2B, 5, 0x80, 0xC0, 0xFF, 4,
    0x80, 0xB8, 0xFF, 4, 0x80, 0xBC, 5, 5, 0x80, 0xA4, 0x2B, 5, 0x80, 0x90, 0xFF, 4, 0x80,
    0x88, 0xFF, 4, 0x80, 0xB0, 5, 5, 0x80, 0x4C, 0x2C, 5, 0x80, 0x80, 0xFF, 4, 0x80, 0x78,
    0xFF, 4, 0x80, 0xA4, 5, 5, 0x80, 0x84, 0x2C, 5, 0x80, 0x70, 0xFF, 4, 0x80, 0x68, 0xFF, 4,
    0x80, 0x98, 5, 5, 0x80, 0xBC, 0x2C, 5, 0x80, 0x60, 0xFF, 4, 0x80, 0x58, 0xFF, 4, 0x80,
    0x8C, 5, 5, 0x80, 0xF4, 0x2C, 5, 0x80, 0x50, 0xFF, 4, 0x80, 0x48, 0xFF, 4, 0x80, 0x68, 5,
    5, 0x80, 0x64, 0x2D, 5, 0x80, 0x20, 0xFF, 4, 0x80, 0x18, 0xFF, 4, 0x80, 0x5C, 5, 5, 0x80,
    0xD4, 0x2D, 5, 0x80, 0x10, 0xFF, 4, 0x80, 8, 0xFF, 4, 0x80, 0x80, 5, 5, 0x80, 0x2C, 0x2D,
    5, 0x80, 0x40, 0xFF, 4, 0x80, 0x38, 0xFF, 4, 0x80, 0x74, 5, 5, 0x80, 0x9C, 0x2D, 5, 0x80,
    0x30, 0xFF, 4, 0x80, 0x28, 0xFF, 4, 0x80, 0x50, 5, 5, 0x80, 0xC, 0x2E, 5, 0x80, 0, 0xFF, 4,
    0x80, 0xF8, 0xFE, 4, 0x80, 0x44, 5, 5, 0x80, 0x44, 0x2E, 5, 0x80, 0xF0, 0xFE, 4, 0x80,
    0xE8, 0xFE, 4, 0x80, 0x38, 5, 5, 0x80, 0x7C, 0x2E, 5, 0x80, 0xE0, 0xFE, 4, 0x80, 0xD8,
    0xFE, 4, 0x80, 0x14, 5, 5, 0x80, 0x24, 0x2F, 5, 0x80, 0xB0, 0xFE, 4, 0x80, 0xA8, 0xFE, 4,
    0x80, 0x20, 5, 5, 0x80, 0xEC, 0x2E, 5, 0x80, 0xC0, 0xFE, 4, 0x80, 0xB8, 0xFE, 4, 0x80,
    0xFC, 4, 5, 0x80, 0x44, 0x4A, 5, 0x80, 0x90, 0xFE, 4, 0x80, 0x88, 0xFE, 4, 0x80, 0x2C, 5,
    5, 0x80, 0xB4, 0x2E, 5, 0x80, 0xD0, 0xFE, 4, 0x80, 0xC8, 0xFE, 4, 0x80, 8, 5, 5, 0x80,
    0xD4, 0x49, 5, 0x80, 0xA0, 0xFE, 4, 0x80, 0x98, 0xFE, 4, 0x80, 0xF0, 4, 5, 0x80, 0xDC,
    0x47, 5, 0x80, 0x80, 0xFE, 4, 0x80, 0x78, 0xFE, 4, 0x80, 0xE4, 4, 5, 0x80, 0x14, 0x48, 5,
    0x80, 0x70, 0xFE, 4, 0x80, 0x68, 0xFE, 4, 0x80, 0xD8, 4, 5, 0x80, 0x4C, 0x48, 5, 0x80,
    0x60, 0xFE, 4, 0x80, 0x58, 0xFE, 4, 0x80, 0xB4, 4, 5, 0x80, 0xF4, 0x48, 5, 0x80, 0x30,
    0xFE, 4, 0x80, 0x28, 0xFE, 4, 0x80, 0xC0, 4, 5, 0x80, 0xBC, 0x48, 5, 0x80, 0x40, 0xFE, 4,
    0x80, 0x38, 0xFE, 4, 0x80, 0x9C, 4, 5, 0x80, 0xB4, 0x4A, 5, 0x80, 0x10, 0xFE, 4, 0x80, 8,
    0xFE, 4, 0x80, 0xCC, 4, 5, 0x80, 0x84, 0x48, 5, 0x80, 0x50, 0xFE, 4, 0x80, 0x48, 0xFE, 4,
    0x80, 0xA8, 4, 5, 0x80, 0xEC, 0x4A, 5, 0x80, 0x20, 0xFE, 4, 0x80, 0x18, 0xFE, 4, 0x80,
    0x90, 4, 5, 0x80, 0xCC, 0x4B, 5, 0x80, 0, 0xFE, 4, 0x80, 0xF8, 0xFD, 4, 0x80, 0x84, 4, 5,
    0x80, 4, 0x4C, 5, 0x80, 0xF0, 0xFD, 4, 0x80, 0xE8, 0xFD, 4, 0x80, 0x78, 4, 5, 0x80, 0x3C,
    0x4C, 5, 0x80, 0xE0, 0xFD, 4, 0x80, 0xD8, 0xFD, 4, 0x80, 0x54, 4, 5, 0x80, 0xE4, 0x4C, 5,
    0x80, 0xB0, 0xFD, 4, 0x80, 0xA8, 0xFD, 4, 0x80, 0x60, 4, 5, 0x80, 0xAC, 0x4C, 5, 0x80,
    0xC0, 0xFD, 4, 0x80, 0xB8, 0xFD, 4, 0x80, 0x48, 4, 5, 0x80, 0x1C, 0x4D, 5, 0x80, 0xA0,
    0xFD, 4, 0x80, 0x98, 0xFD, 4, 0x80, 0x6C, 4, 5, 0x80, 0x74, 0x4C, 5, 0x80, 0xD0, 0xFD, 4,
    0x80, 0xC8, 0xFD, 4, 0x80, 0x3C, 4, 5, 0x80, 0x54, 0x4D, 5, 0x80, 0x90, 0xFD, 4, 0x80,
    0x88, 0xFD, 4, 0x80, 0x30, 4, 5, 0x80, 0x44, 0x43, 5, 0x80, 0x80, 0xFD, 4, 0x80, 0x78,
    0xFD, 4, 0x80, 0x24, 4, 5, 0x80, 0x7C, 0x43, 5, 0x80, 0x70, 0xFD, 4, 0x80, 0x68, 0xFD, 4,
    0x80, 0x18, 4, 5, 0x80, 0xB4, 0x43, 5, 0x80, 0x60, 0xFD, 4, 0x80, 0x58, 0xFD, 4, 0x80, 0xC,
    4, 5, 0x80, 0xEC, 0x43, 5, 0x80, 0x50, 0xFD, 4, 0x80, 0x48, 0xFD, 4, 0x80, 0, 4, 5, 0x80,
    0x24, 0x44, 5, 0x80, 0x40, 0xFD, 4, 0x80, 0x38, 0xFD, 4, 0x80, 0xF4, 3, 5, 0x80, 0x5C,
    0x44, 5, 0x80, 0x30, 0xFD, 4, 0x80, 0x28, 0xFD, 4, 0x80, 0xE8, 3, 5, 0x80, 0x94, 0x44, 5,
    0x80, 0x20, 0xFD, 4, 0x80, 0x18, 0xFD, 4, 0x80, 0xDC, 3, 5, 0x80, 0xCC, 0x44, 5, 0x80,
    0x10, 0xFD, 4, 0x80, 8, 0xFD, 4, 0x80, 0xD0, 3, 5, 0x80, 0xE4, 0x37, 5, 0x80, 0, 0xFD, 4,
    0x80, 0xF8, 0xFC, 4, 0x80, 0xC4, 3, 5, 0x80, 0x1C, 0x38, 5, 0x80, 0xF0, 0xFC, 4, 0x80,
    0xE8, 0xFC, 4, 0x80, 0xB8, 3, 5, 0x80, 0x54, 0x38, 5, 0x80, 0xE0, 0xFC, 4, 0x80, 0xD8,
    0xFC, 4, 0x80, 0xA0, 3, 5, 0x80, 0xC4, 0x38, 5, 0x80, 0xC0, 0xFC, 4, 0x80, 0xB8, 0xFC, 4,
    0x80, 0x94, 3, 5, 0x80, 0xFC, 0x38, 5, 0x80, 0xB0, 0xFC, 4, 0x80, 0xA8, 0xFC, 4, 0x80,
    0x7C, 3, 5, 0x80, 0x6C, 0x39, 5, 0x80, 0x90, 0xFC, 4, 0x80, 0x88, 0xFC, 4, 0x80, 0xAC, 3,
    5, 0x80, 0x8C, 0x38, 5, 0x80, 0xD0, 0xFC, 4, 0x80, 0xC8, 0xFC, 4, 0x80, 0x88, 3, 5, 0x80,
    0x34, 0x39, 5, 0x80, 0xA0, 0xFC, 4, 0x80, 0x98, 0xFC, 4, 0x80, 0x70, 3, 5, 0x80, 0xC4,
    0x3F, 5, 0x80, 0x80, 0xFC, 4, 0x80, 0x78, 0xFC, 4, 0x80, 0x64, 3, 5, 0x80, 0xFC, 0x3F, 5,
    0x80, 0x70, 0xFC, 4, 0x80, 0x68, 0xFC, 4, 0x80, 0x58, 3, 5, 0x80, 0x34, 0x40, 5, 0x80,
    0x60, 0xFC, 4, 0x80, 0x58, 0xFC, 4, 0x80, 0x40, 3, 5, 0x80, 0xA4, 0x40, 5, 0x80, 0x40,
    0xFC, 4, 0x80, 0x38, 0xFC, 4, 0x80, 0x34, 3, 5, 0x80, 0xDC, 0x40, 5, 0x80, 0x30, 0xFC, 4,
    0x80, 0x28, 0xFC, 4, 0x80, 0x1C, 3, 5, 0x80, 0x4C, 0x41, 5, 0x80, 0x10, 0xFC, 4, 0x80, 8,
    0xFC, 4, 0x80, 0x4C, 3, 5, 0x80, 0x6C, 0x40, 5, 0x80, 0x50, 0xFC, 4, 0x80, 0x48, 0xFC, 4,
    0x80, 0x28, 3, 5, 0x80, 0x14, 0x41, 5, 0x80, 0x20, 0xFC, 4, 0x80, 0x18, 0xFC, 4, 0x80,
    0x10, 3, 5, 0x80, 0xB4, 0x3C, 5, 0x80, 0, 0xFC, 4, 0x80, 0xF8, 0xFB, 4, 0x80, 4, 3, 5,
    0x80, 0xEC, 0x3C, 5, 0x80, 0xF0, 0xFB, 4, 0x80, 0xE8, 0xFB, 4, 0x80, 0xF8, 2, 5, 0x80,
    0x24, 0x3D, 5, 0x80, 0xE0, 0xFB, 4, 0x80, 0xD8, 0xFB, 4, 0x80, 0xEC, 2, 5, 0x80, 0x5C,
    0x3D, 5, 0x80, 0xD0, 0xFB, 4, 0x80, 0xC8, 0xFB, 4, 0x80, 0xE0, 2, 5, 0x80, 0x94, 0x3D, 5,
    0x80, 0xC0, 0xFB, 4, 0x80, 0xB8, 0xFB, 4, 0x80, 0xD4, 2, 5, 0x80, 0xCC, 0x3D, 5, 0x80,
    0xB0, 0xFB, 4, 0x80, 0xA8, 0xFB, 4, 0x80, 0xC8, 2, 5, 0x80, 4, 0x3E, 5, 0x80, 0xA0, 0xFB,
    4, 0x80, 0x98, 0xFB, 4, 0x80, 0xBC, 2, 5, 0x80, 0x3C, 0x3E, 5, 0x80, 0x90, 0xFB, 4, 0x80,
    0x88, 0xFB, 4, 0x80, 0xB0, 2, 5, 0x80, 0x3C, 0x53, 5, 0x80, 0x80, 0xFB, 4, 0x80, 0x78,
    0xFB, 4, 0x80, 0x80, 2, 5, 0x80, 0x1C, 0x54, 5, 0x80, 0x40, 0xFB, 4, 0x80, 0x38, 0xFB, 4,
    0x80, 0xA4, 2, 5, 0x80, 0x74, 0x53, 5, 0x80, 0x70, 0xFB, 4, 0x80, 0x68, 0xFB, 4, 0x80,
    0x74, 2, 5, 0x80, 0x8C, 0x54, 5, 0x80, 0x30, 0xFB, 4, 0x80, 0x28, 0xFB, 4, 0x80, 0x8C, 2,
    5, 0x80, 0xE4, 0x53, 5, 0x80, 0x50, 0xFB, 4, 0x80, 0x48, 0xFB, 4, 0x80, 0x68, 2, 5, 0x80,
    0x54, 0x54, 5, 0x80, 0x20, 0xFB, 4, 0x80, 0x18, 0xFB, 4, 0x80, 0x98, 2, 5, 0x80, 0xAC,
    0x53, 5, 0x80, 0x60, 0xFB, 4, 0x80, 0x58, 0xFB, 4, 0x80, 0x5C, 2, 5, 0x80, 0xC4, 0x54, 5,
    0x80, 0x10, 0xFB, 4, 0x80, 8, 0xFB, 4, 0x80, 0x50, 2, 5, 0x80, 0xC4, 0x4D, 5, 0x80, 0,
    0xFB, 4, 0x80, 0xF8, 0xFA, 4, 0x80, 8, 2, 5, 0x80, 0xBC, 0x4F, 5, 0x80, 0xA0, 0xFA, 4,
    0x80, 0x98, 0xFA, 4, 0x80, 0x44, 2, 5, 0x80, 0xFC, 0x4D, 5, 0x80, 0xF0, 0xFA, 4, 0x80,
    0xE8, 0xFA, 4, 0x80, 0x20, 2, 5, 0x80, 0xA4, 0x4E, 5, 0x80, 0xC0, 0xFA, 4, 0x80, 0xB8,
    0xFA, 4, 0x80, 0x2C, 2, 5, 0x80, 0x6C, 0x4E, 5, 0x80, 0xD0, 0xFA, 4, 0x80, 0xC8, 0xFA, 4,
    0x80, 0xFC, 1, 5, 0x80, 0xF4, 0x4F, 5, 0x80, 0x90, 0xFA, 4, 0x80, 0x88, 0xFA, 4, 0x80,
    0x38, 2, 5, 0x80, 0x34, 0x4E, 5, 0x80, 0xE0, 0xFA, 4, 0x80, 0xD8, 0xFA, 4, 0x80, 0x14, 2,
    5, 0x80, 0x2C, 0x50, 5, 0x80, 0xB0, 0xFA, 4, 0x80, 0xA8, 0xFA, 4, 0x80, 0xF0, 1, 5, 0x80,
    0x64, 0x50, 5, 0x80, 0x80, 0xFA, 4, 0x80, 0x78, 0xFA, 4, 0x80, 0xB4, 1, 5, 0x80, 0xB4,
    0x51, 5, 0x80, 0x30, 0xFA, 4, 0x80, 0x28, 0xFA, 4, 0x80, 0xE4, 1, 5, 0x80, 0x9C, 0x50, 5,
    0x80, 0x70, 0xFA, 4, 0x80, 0x68, 0xFA, 4, 0x80, 0x9C, 1, 5, 0x80, 0x7C, 0x51, 5, 0x80,
    0x10, 0xFA, 4, 0x80, 8, 0xFA, 4, 0x80, 0xCC, 1, 5, 0x80, 0xC, 0x51, 5, 0x80, 0x50, 0xFA, 4,
    0x80, 0x48, 0xFA, 4, 0x80, 0xA8, 1, 5, 0x80, 0xEC, 0x51, 5, 0x80, 0x20, 0xFA, 4, 0x80,
    0x18, 0xFA, 4, 0x80, 0xD8, 1, 5, 0x80, 0xD4, 0x50, 5, 0x80, 0x60, 0xFA, 4, 0x80,
};
s32 D_800A0000[5] = { (s32)&D_8004FA58, (s32)&D_800501C0, (s32)&D_80055144, (s32)&D_8004FA40, (s32)&D_8004FA38 };
s32 D_800A0014[2] = {0};
u8 *D_800A001C[45] = {
    (void *)&file_group_JUNGLE, (void *)&file_group_SPACE, (void *)&file_group_SNOW,
    (void *)&file_group_DRAINS, (void *)&file_group_PAPU, (void *)&file_group_TANKLAST,
    (void *)&file_group_TANKDESERT, (void *)&file_group_TANKJUNGLE,
    (void *)&file_group_TANKMETAL, (void *)&file_group_TANKBOSS,
    (void *)&file_group_POGOPAINTER, (void *)&file_group_POGOAGOGO,
    (void *)&file_group_POGOLOCO, (void *)&file_group_POGOBOUNCER, (void *)&file_group_BALLS,
    (void *)&file_group_BEACHBALL, (void *)&file_group_NGINBALL, (void *)&file_group_SKYBALL,
    (void *)&file_group_POLARDAY, (void *)&file_group_POLARNGT, (void *)&file_group_POLARCUT,
    (void *)&file_group_POLAREND, (void *)&file_group_POLARBSS, (void *)&file_group_DASH,
    (void *)&file_group_TOXIC, (void *)&file_group_WATER, (void *)&file_group_LAVA,
    (void *)&file_group_RING, (void *)&file_group_DINO, (void *)&file_group_BONK,
    (void *)&file_group_CHASE, (void *)&file_group_N2O, (void *)&file_group_BASHMODE20,
    (void *)&file_group_HUB, (void *)&file_group_HUB, (void *)&file_group_HUB,
    (void *)&file_group_HUB, (void *)&file_group_HUB, (void *)&file_group_HUB,
    (void *)&file_group_HUB, (void *)&file_group_TITLES, (void *)&file_group_TITLES,
    (void *)&file_group_TITLES, (void *)&D_800A0014, (void *)&file_group_POLARDAY,
};
u16 D_800A00D0[6] = { 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0 };
process_info D_800A00DC = { func_800936C4, func_80093BE8, func_8009407C, func_80094058, 0 };
s32 map[26] = {0};
s32 D_800A0158 = 0;
process_control_info LoadingControl = {0};
GfxCamera LoadingCamera = {0};
u8 D_800A0224[108] = {0};
WobbleObj *D_800A0290 = 0;
s32 D_800A0294[11] = {0};
s32 D_800A02C0 = 0;
s32 D_800A02C4 = 0;
s32 D_800A02C8[4] = {0};
EngineMarkerPos D_800A02D8[3] = {
    { 0x7FB, 0x1FC3, 0xFA0, 0x7D0, { 0, 0, 0, 0, 0, 0 } },
    { 0x1FC3, 0xFA0, 0x7E4, 0x5DC, { 0, 0, 0, 0, 0, 0 } },
    { 0, 0, 0, 0x3E8, { 0, 0, 0, 0, 0, 0 } },
};
EngineCfg D_800A0314 = {
    { 0, 0x10, 0, 0, 0x10, 0, 0, 0, 0, 0x10, 0, 0 }, { 0x80, 0x200, 0x20 },
    (void *)&D_80063ED8, (void *)&D_80063BB8, 0x10, 0x400,
};
s32 D_800A033C[52] = {
    1, (s32)&D_80056024, 0, 1, 1, (s32)&D_8005605C, 0, 1, 1, (s32)&D_80056094, 0, 1, 1,
    (s32)&D_800560CC, 0, 1, 1, (s32)&D_80056104, 0, 1, 1, (s32)&D_8005613C, 0, 1, 1,
    (s32)&D_80056174, 0, 1, 1, (s32)&D_800561AC, 0, 1, 1, (s32)&D_800561E4, 0, 1, 1,
    (s32)&D_8005621C, 0, 1, 1, (s32)&D_80056254, 0, 1, 1, (s32)&D_8005628C, 0, 1, 0, 0, 0, 0,
};
s32 D_800A040C[15] = {
    (s32)&D_8004E940, 0x780, 0x4AD, (s32)&D_8004E938, 0x781, 0x4AD, (s32)&D_8004E930, 0x782,
    0x4AD, (s32)&D_8004E928, 0x783, 0x4AD, 0, 0, 0,
};
s32 D_800A0448[14] = { 1, (s32)&D_800564F4, 0, 1, 0, 0, 0, 0, (s32)&D_8004E988, 0x76C, 0x4AD, 0, 0, 0 };
s32 D_800A0480[40] = {
    1, (s32)&D_800562C4, 0, 1, 1, (s32)&D_800562FC, 0, 1, 1, (s32)&D_80056334, 0, 1, 1,
    (s32)&D_8005636C, 0, 1, 1, (s32)&D_800563A4, 0, 1, 1, (s32)&D_800563DC, 0, 1, 1,
    (s32)&D_80056414, 0, 1, 1, (s32)&D_80056484, 0, 1, 1, (s32)&D_800564BC, 0, 1, 0, 0, 0, 0,
};
s32 D_800A0520[20] = {
    (s32)&D_8004E920, 0x78A, 0x4AD, (s32)&D_8004E918, 0x78B, 0x4AD, (s32)&D_8004E910, 0x78C,
    0x4AD, 0, 0, 0, 1, (s32)&D_8005652C, 0, 0, 0, 0, 0, 0,
};
s32 D_800A0570[8] = { 1, (s32)&D_80056564, 0, 1, 0, 0, 0, 0 };
s32 D_800A0590[20] = {
    1, (s32)&D_80055F44, 0, 1, 1, (s32)&D_80055F7C, 0, 1, 1, (s32)&D_80055FB4, 0, 1, 1,
    (s32)&D_80055FEC, 0, 1, 0, 0, 0, 0,
};
s32 D_800A05E0[12] = {
    (s32)&D_8004E958, 0x776, 0x4AD, (s32)&D_8004E950, 0x777, 0x4AD, (s32)&D_8004E948, 0x778,
    0x4AD, 0, 0, 0,
};
s32 D_800A0610[16] = { 1, (s32)&D_8005659C, 0, 1, 0, 0, 0, 0, 1, (s32)&D_8005659C, 0, 1, 0, 0, 0, 0 };
s32 D_800A0650[8] = { 1, (s32)&D_800565D4, 0, 1, 0, 0, 0, 0 };
s32 D_800A0670[80] = {
    1, (s32)&D_80055AE4, 0, 0, 1, (s32)&D_80055B1C, 0, 0, 1, (s32)&D_80055B54, 0, 0, 1,
    (s32)&D_80055B8C, 0, 0, 1, (s32)&D_80055BC4, 0, 0, 1, (s32)&D_80055BFC, 0, 0, 1,
    (s32)&D_80055C34, 0, 0, 1, (s32)&D_80055C6C, 0, 0, 1, (s32)&D_80055CA4, 0, 0, 1,
    (s32)&D_80055D14, 0, 0, 1, (s32)&D_80055D4C, 0, 0, 1, (s32)&D_80055D84, 0, 0, 1,
    (s32)&D_80055DBC, 0, 0, 1, (s32)&D_80055DF4, 0, 0, 1, (s32)&D_80055E2C, 0, 0, 1,
    (s32)&D_80055E64, 0, 0, 1, (s32)&D_80055E9C, 0, 0, 1, (s32)&D_80055ED4, 0, 0, 1,
    (s32)&D_80055F0C, 0, 0, 0, 0, 0, 0,
};
s32 D_800A07B0[42] = {
    (s32)&D_8004E908, 0x794, 0x255, (s32)&D_8004E900, 0x795, 0x255, (s32)&D_8004E8F8, 0x796,
    0x255, (s32)&D_8004E8F0, 0x797, 0x255, (s32)&D_8004E8E8, 0x798, 0x255, (s32)&D_8004E8E0,
    0x799, 0x255, (s32)&D_8004E8D8, 0x79A, 0x255, (s32)&D_8004E8D0, 0x79B, 0x255,
    (s32)&D_8004E8C8, 0x79C, 0x255, (s32)&D_8004E8C0, 0x79D, 0x255, (s32)&D_8004E8B8, 0x79E,
    0x255, 0, 0, 0, (s32)&D_800A0670, (s32)&D_800A07B0, 0x1DFC40, (s32)&D_800A0650,
    (s32)&D_800A07B0, 0x1E0000,
};
MsgStep D_800A0858[2] = {
    { (void *)&D_800A033C, (void *)&D_800A040C, 0x1E0000 },
    { (void *)&D_800A0448, (void *)&D_800A07B0, 0x1E0000 },
};
MsgStep D_800A0870[2] = {
    { (void *)&D_800A0480, (void *)&D_800A0520, 0x1E0000 },
    { (void *)&D_800A0480, (void *)&D_800A07B0, 0x1E0000 },
};
MsgStep D_800A0888[4] = {
    { (void *)&D_800A0590, (void *)&D_800A05E0, 0x1E0000 },
    { (void *)&D_800A0570, (void *)&D_800A07B0, 0x1E0000 },
    { (void *)&D_800A0610, (void *)&D_800A07B0, 0x1E0000 },
    { (void *)&D_800A0610, (void *)&D_800A07B0, 0x1E0000 },
};
s32 D_800A08B8[6] = { (s32)&D_8004E810, 0x3DD, 0, 0, 0, 0 };
s32 D_800A08D0[6] = { (s32)&D_8004E818, 0x3DA, 0, 0, 0, 0 };
s32 D_800A08E8[6] = { (s32)&D_8004E800, 0x3E3, 0, 0, 0, 0 };
s32 D_800A0900[6] = { (s32)&D_8004E808, 0x3E0, 0, 0, 0, 0 };
s32 D_800A0918[8] = { 1, (s32)&D_8005660C, 0, 0, 0, 0, 0, 0 };
s32 D_800A0938[3] = { (s32)&D_800A0918, (s32)&D_800A08B8, 0x1E0000 };
s32 D_800A0944[8] = { 1, (s32)&D_80056644, 0, 0, 0, 0, 0, 0 };
s32 D_800A0964[3] = { (s32)&D_800A0944, (s32)&D_800A08D0, 0x1E0000 };
s32 D_800A0970[8] = { 1, (s32)&D_800566B4, 0, 0, 0, 0, 0, 0 };
s32 D_800A0990[3] = { (s32)&D_800A0970, (s32)&D_800A0900, 0x1E0000 };
s32 D_800A099C[8] = { 1, (s32)&D_8005667C, 0, 0, 0, 0, 0, 0 };
s32 D_800A09BC[3] = { (s32)&D_800A099C, (s32)&D_800A08E8, 0x1E0000 };
s32 D_800A09C8[6] = { (s32)&D_8004E7F0, 0x3DC, 0, 0, 0, 0 };
s32 D_800A09E0[6] = { (s32)&D_8004E7F8, 0x3D9, 0, 0, 0, 0 };
s32 D_800A09F8[6] = { (s32)&D_8004E7E0, 0x3E2, 0, 0, 0, 0 };
s32 D_800A0A10[6] = { (s32)&D_8004E7E8, 0x3DF, 0, 0, 0, 0 };
s32 D_800A0A28[8] = { 1, (s32)&D_800566EC, 0, 0, 0, 0, 0, 0 };
s32 D_800A0A48[3] = { (s32)&D_800A0A28, (s32)&D_800A09C8, 0x1E0000 };
s32 D_800A0A54[8] = { 1, (s32)&D_80056724, 0, 0, 0, 0, 0, 0 };
s32 D_800A0A74[3] = { (s32)&D_800A0A54, (s32)&D_800A09E0, 0x1E0000 };
s32 D_800A0A80[8] = { 1, (s32)&D_80056794, 0, 0, 0, 0, 0, 0 };
s32 D_800A0AA0[3] = { (s32)&D_800A0A80, (s32)&D_800A0A10, 0x1E0000 };
s32 D_800A0AAC[8] = { 1, (s32)&D_8005675C, 0, 0, 0, 0, 0, 0 };
s32 D_800A0ACC[3] = { (s32)&D_800A0AAC, (s32)&D_800A09F8, 0x1E0000 };
s32 D_800A0AD8[6] = { (s32)&D_8004E7C0, 0x3E4, 0, 0, 0, 0 };
s32 D_800A0AF0[6] = { (s32)&D_8004E7C8, 0x3E1, 0, 0, 0, 0 };
s32 D_800A0B08[8] = { 1, (s32)&D_80056804, 0, 0, 0, 0, 0, 0 };
s32 D_800A0B28[3] = { (s32)&D_800A0B08, (s32)&D_800A0AF0, 0x1E0000 };
s32 D_800A0B34[8] = { 1, (s32)&D_800567CC, 0, 0, 0, 0, 0, 0 };
s32 D_800A0B54[3] = { (s32)&D_800A0B34, (s32)&D_800A0AD8, 0x1E0000 };
EngineKeyed16 D_800A0B60[4] = {
    { 4, { (s32)&D_800A0A74, (s32)&D_800A0964, (s32)&D_800A0A74 } },
    { 0x16, { (s32)&D_800A0A48, (s32)&D_800A0938, (s32)&D_800A0A48 } },
    { 0x1F, { (s32)&D_800A0ACC, (s32)&D_800A09BC, (s32)&D_800A0B54 } },
    { 9, { (s32)&D_800A0AA0, (s32)&D_800A0990, (s32)&D_800A0B28 } },
};
Blk32 D_800A0BA0 = { { 0x1000, 0, 0x1000, 0, 0x1000, 0, 0, 0 } };
CamVec3 D_800A0BC0 = {0};
GfxViewport D_800A0BCC = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0xF0, 0x200, 0xDFF, 0, 0, 0, 0, 1 };
process_info D_800A0BF4 = { func_80094754, func_80094B38, func_80094EB8, func_80094DD0, 0 };
process_info *D_800A0C08 = 0;
s32 D_800A0C0C = 0;
EngineMarker D_800A0C10[8] = {0};
s32 D_800A0C70 = 0;
subengine D_800A0C74 = {0};
s32 D_800A0D1C = 0;
s32 D_800A0D20[1] = {0};
s32 D_800A0D24[1] = {0};
s32 D_800A0D28 = 0;
void *D_800A0D2C = 0;
s32 D_800A0D30 = 0;
u8 *D_800A0D34 = 0;
s32 D_800A0D38 = 0;
s32 D_800A0D3C = 0;
s32 D_800A0D40[1] = {0};
Tmpl1C D_800A0D44[11] = {
    { 0x8001, 0, 0x8021, 0x8080, 4, 0, 4, 0, 0x180, 0 },
    { 0x8001, 0, 0x8021, 0x8080, 4, 0, 4, 0, 0x100, 0 },
    { 0x8000, 0, 0x8000, 0xC0C0, 4, 0, 4, 0, 0x180, 0 },
    { 0x8001, 0, 0x8000, 0xC0C0, 4, 0, 4, 0, 0x100, 0 },
    { 0x8000, 0, 0x8000, 0xC0C0, 4, 0, 4, 0, 0x100, 0 },
    { 0x8000, 0xFFFFFF, 0, 0xC0C0, 4, 0, 4, 0, 0x184, 0 },
    { 0x8001, 0, 0x8000, 0xC0C0, 4, 0, 4, 0, 0x184, 0 },
    { 0x8001, 0x808000, 0x8000, 0x808000, 4, 0, 4, 0, 0x180, 0 },
    { 0x8001, 0x80, 0x8000, 0x80, 4, 0, 4, 0, 0x180, 0 },
    { 0x8000, 0, 0x8000, 0xFFC000, 4, 0, 4, 0, 0x100, 0 },
    { 0x8000, 0, 0x8000, 0xFF, 4, 0, 4, 0, 0x100, 0 },
};
EXTengine EnginePool[9] = {0};
subengine D_800A13F4[9][32] = {0};
