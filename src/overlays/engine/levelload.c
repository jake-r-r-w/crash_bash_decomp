#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/levelload.h"
#include "bash/hero.h"
#include "bash/shell.h"

s32 LoadModeText(void) {
    EngineGameView *g;
    u8 *p;
    s32 **tab = ModeTextTables;
    s32 **rp;

    g = (EngineGameView *)GameInfo;
    rp =&tab[*(s32 *)(GameInfo + 0x2C)];
    p = (u8 *)(*rp)[ModeTypes[g->mode * 3] - 0x101];
    if (p != NULL) {
        load_file_direct(p, ModeTextLoadAddr);
    }
    p = (u8 *)ModeTextFiles[ModeTypes[g->mode * 3] - 0x101];
    if ((s32)p != ModeTextLoaded) {
        load_file_direct(p, SlotBase);
        ModeTextLoaded = (s32)p;
    }
    return (s32)(SlotBase + *(s32 *)(p + 4));
}

void AnimateLoadingIcon(void) {
    TrigEntry *t;
    subengine *o;
    s32 f;
    s32 i;

    if (LevelLoadStage >= 2 && (o = LoadingIconEngine.sub) != NULL) {
        t = rcossin_tbl;
        o->unk51 = 0;
        o->unk12 += 0x20;
        f = FrameCount;
        o->unk10 = *(s32 *)&t[(((f * 70) & 0xFFF) - 0x400) & 0xFFF] >> 20;
        o->unk12 = *(s32 *)&t[(((f * 140) & 0xFFF) - 0x400) & 0xFFF] >> 21;
        o->unk14 = (t[(((f * 70) & 0xFFF) - 0x400) & 0xFFF].unk2 +
                    t[(((f * 100) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 5;
        o->unk04 = *(s32 *)&t[(((f * 35) & 0xFFF) - 0x400) & 0xFFF] >> 23;
        o->unk08 = (t[(((f * 45) & 0xFFF) - 0x400) & 0xFFF].unk2 +
                   t[(((f * 145) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 5;
        o->unk52 = 1;
        o = o->next;
        if (o != NULL) {
            o->unk14 += 0x17;
            i = (-f * 10) & 0xFFF;
            o->unk10 = *(s32 *)&t[(i - 0x400) & 0xFFF] >> 20;
            o->unk12 = *(s32 *)&t[i] >> 20;
            o->unk52 = 1;
        }
    }
}

void UpdateLoadingScene(void) {
    u8 *p;
    u8 *b;  /* the bank as bytes: an obj_dbase * here changes the branches */

    if (LevelLoadStage >= 2) {
        if (LoadingScenePlaying == 0 && (b = (u8 *)HeroModelBank, ((obj_dbase *)b)[DB_CutsceneIntroLogo2].data != NULL)) {
            if (SfxTable[0x3C5][1] != -1) {
                PlaySfx(0x3C5, NULL, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x3C6, NULL, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x3C7, NULL, NULL, 0x1000, 0x1E00, 0);
                p = (u8 *)&((obj_dbase *)b)[DB_CutsceneIntroLogo2];
                LoadingSceneDelay = 6;
                SetupSceneMarks(p);
                LoadingScenePlaying += 1;
                play_script((obj_dbase *)p, 0, &LoadingScriptRot, &LoadingScriptPos, 8, NULL);
            }
        }
        if (LoadingSceneDelay != 0) {
            LoadingSceneDelay -= 1;
            if (LoadingSceneDelay == 0) {
                PlaySfx(0x3C8, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        scripts_update();
    }
}

void LoadModeFiles(void) {
    u8 *p;
    s32 *q;

    p = ModeFileGroups[GameMode];
    if (p != NULL) {
        queue_group_load(p);
        while (loader_pending() != 0) {
            loader_update();
            mal_compact(&MainHeap);
            vab_transfer_check();
        }
    }
    load_font();
    if (GameMode == 0x28 && FirstBootDone == 0) {
        dbase_load_textures_wait(&HeroModelBank[DB_CutsceneIntroLogo2]);
    } else {
        dbase_load_textures_wait(&HeroModelBank[DB_Loading]);
    }
    RenderHook = RenderGameScene;
    cam_init(&MainCamera);
    cam_init(&MainGfxCamera);
    cam_init(&HudCamera);
    MainGfxCamera.at.z = 0x2000;
    cam_init(&LoadingCamera);
    LoadingCamera.eye.z = 0xA24;
    LoadingCamera.at.z = 0;
    q = (s32 *)&GameViewport;
    q[5] = 0x15E;
    q[4] = 0x140;
}

/* the heap from p to the top of RAM below the stack (0x801FF000) */
static inline void InitHeapToRamTop(_mal_area *heap, s32 p) {
    s32 size;

    size = 0x801FF000;
    size -= p;
    mal_init(heap, (u8 *)p, size);
    smallnode_init();
}

void InitLevelLoad(void) {
    s32 start;
    EngineCfg *cfg;

    sound_stop();
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    InitHeapToRamTop(&MainHeap, (s32)SlotHeapStart);
    start = LoadModeText() + 7;
    start &= ~7;
    InitHeapToRamTop(&MainHeap, start);
    SetupLevelSpecs(&DefaultArenaDef);
    cfg = (EngineCfg *)LevelConfig;
    if (cfg == NULL) {
        cfg = &DefaultLevelConfig;
    }
    AllocOtBuffers((s32 *)cfg);
    ((void (*)(u8 *))ResetScreen)((u8 *)(VideoMode == 0 ? &vmode_512_ntsc : &vmode_512_pal));
    GameInfo[0x60] = 0;
    GameInfo[0x61] = 0;
    vram_init(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    prims_init(cfg->pools);
    dbase_init_all();
    sound_init();
    draw_init();
    LevelLoadStage = 0;
    if (((D5A614View *)GameInfo)->mode == 0x28 && FirstBootDone != 0) {
        ProcessControl.flags = 0x1D;
        ProcessControl.next_process = &RunGame;
    }
}

void QueueHeroModels(void) {
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
    arg = LevelRecs[a->unk8].unk4;
    {
        s32 n = LangPanelModels[a->unk2C];
        obj_dbase *b = HeroModelBank;
        obj_dbase *t = &b[n];
        b[DB_Interface].file = t->file;  /* the language's panel models */
        base = (u8 *)b;
        ((obj_dbase *)base)[DB_Interface].textureFile = t->textureFile;
    }
    g = a;
    tbl = &HeroHooks;
loop:
    if (i < 4) {
        do {
            c = ((u8 *)g)[i + 0x50];
            if ((u32)(c - 1) < 8) {
                {
                    u8 *p = ModeDbaseLists;
                    e = ((u16 *)tbl->HeroModels)[c] * 0x38 + (s32)base;
                    for (j = 0; j < 0x60; j++) {
                        if (e == *(s32 *)(p + 4)) {
                            queue_dbase_load(p, arg);
                        }
                        p += 0x10;
                    }
                }
                if (tbl->unk8 != NULL) {
                    u8 *p = ModeDbaseLists;
                    e = ((u16 *)tbl->unk8)[c] * 0x38 + (s32)base;
                    for (j = 0; j < 0x60; j++) {
                        if (e == *(s32 *)(p + 4)) {
                            queue_dbase_load(p, arg);
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

  new_var = LangPanelModels;
  i = 0;
  tg = GameInfo;
  idx = ((View939FC *) tg)->unk8;
  g = (View939FC *) tg;
  tg = (u8 *) LevelRecs;
  arg = *((s32 *) (((u8 *) (&((EngineRec32 *) tg)[idx])) + 4));
  t = (u8 *)HeroModelBank;
  h = &HeroHooks;
  src = (s32 *) (&((obj_dbase *) t)[new_var[g->unk2C]]);
  ((obj_dbase *) t)[DB_Interface].file = src[0];
  b = t;
  ((obj_dbase *) b)[DB_Interface].textureFile = src[1];
  do
  {
    k = ((u8 *) g)[i + 0x50];
    if ((k - 1) < 8)
    {
      dbase_asload_with_anims((obj_dbase *) ((((u16 *) h->HeroModels)[k] * 0x38) + ((s32) b)), arg);
      if (h->unk8 != 0)
      {
        dbase_asload_with_anims((obj_dbase *) ((((u16 *) h->unk8)[k] * 0x38) + ((s32) b)), arg);
      }
    }
    i++;
  }
  while (i < 4);

  p = *((s16 **) LevelTriggerData);
  v = *p;
  if (v >= 0)
  {
    do
    {
      dbase_asload_with_anims(&HeroModelBank[v], 0);
      p++;
      v = *p;
    }
    while (v >= 0);
  }
  p = *((s16 **) (((u8 *) (&LevelRecs[LevelRecIndex])) + 8));
  v = *p;
  if (v >= 0)
  {
    do
    {
      dbase_asload_with_anims(&HeroModelBank[v], 0);
      p++;
      v = *p;
    }
    while (v >= 0);
  }
}


void UpdateLevelLoad(void) {
    EngStateView *g0;
    EngStateView *g;
    map_map *c;
    s32 *a;
    s32 *p;

    if (LevelLoadStage >= 2) {
        process_control_update(&LoadingControl, NULL);
    }
    vab_transfer_wait();
    mal_count_free(&MainHeap);
    if (loader_pending() != 0 || MainHeap.deferred != NULL) {
        return;
    }
    switch (LevelLoadStage) {
    case 0:
        if (GameMode == 0x28 && FirstBootDone == 0) {
            process_control_init(&LoadingControl, &LoadingProcess2);
        } else {
            process_control_init(&LoadingControl, &LoadingProcess);
        }
        process_control_init(&MenuProcessControl, NULL);
        ResetEnginePool();
        LoadModeFiles();
        g0 = (EngStateView *)GameInfo;
        g0->unk42 = -1;
        g0->unkCC[0] = 0;
        g0->unkCC[1] = 0;
        g0->unkCC[2] = 0;
        g0->unkCC[3] = 0;
        LevelLoadStage = 1;
        g0->unk30 &= ~0x200;
        break;
    case 1:
        SetNasDefaults();
        g = (EngStateView *)GameInfo;
        g->unk30 = 0;
        g->unk10 = 0;
        InitNasHeap(NasHeapSize);
        subpool_init(SubPoolCount);
        init_scripts(0x20, 0x20);
        process_init(&LoadingControl);
        InitialiseNasSystem();
        PreInitLevel();
        if (g->mode != 0x28 || FirstBootDone == 0) {
            music_load_track(g->mode);
        }
        if (GameMode == 0x28 && FirstBootDone == 0) {
            sfx_table_clear();
            sfx_bank_load((s32)&DatIndex[2 * 273]);
        }
        set_fade(&ScreenFade, -0x300, 0);
        LevelLoadStage = 2;
        break;
    case 6:
        if (GameMode != 0x28 || FirstBootDone == 0) {
            music_play_level(GameMode, ModeTypes[GameMode * 3]);
        }
        load_initial_dbases();
        a = map;
        if (LevelLoadModel >= 0) {
            p = (s32 *)(&HeroModelBank[LevelLoadModel]);
        } else {
            p = NULL;
        }
        mapscene_init(a, p);
        LevelLoadStage = 4;
        break;
    case 2:
        if (GameMode == 0x28) {
            music_request(1);
        } else {
            music_request(0);
        }
    case 8:
        QueueHeroModels();
        LevelLoadStage = 6;
        break;
    case 5:
        music_play_level(GameMode, ModeTypes[GameMode * 3]);
        LevelLoadStage = 3;
        break;
    case 3:
        a = map;
        if (LevelLoadModel >= 0) {
            p = (s32 *)(&HeroModelBank[LevelLoadModel]);
        } else {
            p = NULL;
        }
        mapscene_init(a, p);
        load_initial_dbases();
        LevelLoadStage = 4;
        break;
    case 4:
        ((EngColor *)&GameViewport)->code = MapScene->map->clear;
        c = MapScene->map;
        ((EngColor *)&GameViewport)->rgb = *(EngRgb *)&c->clear_colour;
        ClearLevelNas();
        InitialiseEngine();
        LevelLoadStage = 7;
        break;
    case 7:
        GotoProcess(&RetryProcess, 0x300);
        if (GameMode != 0x28) {
            music_request(-1);
        }
        LevelLoadStage = 9;
        break;
    case 9:
        break;
    }
}

void realign_panel_dbase(void) {
    struct E { s32 a, b; u8 pad[0x30]; };
    struct G { struct E e[0x1C]; s32 x, y; } *g;
    struct E *e;
    u16 i = LangPanelModels[GameLanguage];
    g = (void *)HeroModelBank;
    e = &g->e[i];
    g->x = e->a;
    g->y = e->b;
}

void QuitLevelLoad(void) {
    process_quit(&LoadingControl);
}

void RenderLevelLoad(void) {
    if (LevelLoadStage >= 2) {
        process_control_render(&LoadingControl);
    }
}

void LoadModeDbases(s32 arg0, s32 arg1) {
    s32 i;
    u8 *p = ModeDbaseLists;

    for (i = 0; i < 0x60; i++) {
        if (arg0 == *(s32 *)(p + 4)) {
            queue_dbase_load(p, arg1);
        }
        p += 0x10;
    }
}

void QuitLoadingScene(void) {
    scripts_kill_all();
    dbase_release(&HeroModelBank[DB_CutsceneIntroLogo2]);
}

void RenderLoadingScene(void) {
    if (LevelLoadStage >= 2) {
        GameFrameCount += 1;
        gfx_set_viewport(LoadingSceneViewport, &MainCamera);
        if (LoadingScenePlaying != 0) {
            DrawSceneMarks();
        }
        scripts_render();
    }
}

void InitLoadingCamera(void) {
    cam_init(&MainCamera);
    LoadingScenePlaying = 0;
    LoadingSceneDelay = 0;
}

void QuitLoadingScreen(void) {
    eng_release_subs_sfx(&LoadingIconEngine);
    dbase_release(&HeroModelBank[DB_Loading]);
}

void RenderLoadingScreen(void) {
    if (LevelLoadStage >= 2) {
        gfx_set_viewport(&LoadingViewport, &LoadingCamera);
        RenderState.modelDp = ScreenFade.amount;
        eng_render(&LoadingIconEngine);
    }
}

void InitLoadingScreen(void) {
    EXTengine *eng = &LoadingIconEngine;
    subengine *sub;
    u16 *p;

    *eng = EngineTemplate;
    eng->unk5D = 0;
    eng->unk10.x = 0;
    eng->unk10.y = 0;
    eng->flags |= 0x10000000;
    eng_add_subs(eng, 2);
    sub = eng->sub;
    p = LoadingEntities;
    if (sub != NULL) {
        do {
            sub->u6C.dbase = &HeroModelBank[DB_Loading];
            sub->flags = 0x01008000;
            sub->entity = *p;
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

    if (LevelLoadModel >= 0) {
        p = (s32 *)(&HeroModelBank[LevelLoadModel]);
    } else {
        p = NULL;
    }
    mapscene_init(a, p);
}

void LoadLoadingModel(void) {
    if (GameMode == 0x28 && FirstBootDone == 0) {
        dbase_load_textures_wait(&HeroModelBank[DB_CutsceneIntroLogo2]);
    } else {
        dbase_load_textures_wait(&HeroModelBank[DB_Loading]);
    }
}

void DrawSceneMarks(void) {
    s32 h = RenderState.cam->dist;
    subengine *e = &SceneMarkSub;
    EngineMarker *m;
    EngineMarkerPos *p;
    s32 i;
    s32 s;

    for (i = 0; i < SceneMarkCount; i++) {
        m = &SceneMarks[i];
        p = &SceneMarkPos[m->idx];
        *e = SubEngineTemplate;
        e->entity = m->unk0;
        e->u6C.dbase = m->unk8;
        e->unk51 = 0;
        s = GameFrameCount;
        e->unk10 = (s * p->x) >> 12;
        e->unk12 = (s * p->y) >> 12;
        e->unk14 = (s * p->z) >> 12;
        e->flags = 0x03008000;
        e->unk68 = p->unkC;
        gte_SetGeomScreen(h);
        draw_object((u8 *)e, (u8 *)&RenderState.cam->view, 0x03008000);
        gte_SetGeomScreen(RenderState.cam->dist);
    }
}

void SetupScriptScene(ScriptScene *arg0) {
    s32 rate;

    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    rate = 3;
    VSyncRate = rate;
    ScriptTimeStep = 0x1E0000 / (60 / rate);
    ScriptTimeStep = arg0->rate * rate / 60;
    cam_init(&MainCamera);
    SceneOldDbase = 0;
    SceneStarting = 1;
    SceneMarkCount = 0;
    ((ScriptScene *)&SceneMark)->marks = arg0->marks;  /* as a field store: a plain one is scheduled apart */
    SceneVabCue = arg0->cues;
    SceneVabSlot[0] = 0;
    SceneSoundHandle[0] = 0;
    sound_reinit();
    SfxBanks[0].id = -1;
    SfxBanks[0].handle = 0;
    SfxBanks[0].head = NULL;
    SfxBanks[1].id = -1;
    SfxBanks[1].handle = 0;
    SfxBanks[1].head = NULL;
    SceneStarted = 0;
    VabCueTimer = 0;
    VabCuePending = 0;
}

void InitScriptScene(void) {
    EngineCfg *cfg;

    sound_stop();
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    SceneStep = 0;
    sound_reinit();
    ModeTextLoaded = 0;
    mal_init(&MainHeap, SlotBase, 0x801FF000 - (s32)SlotBase);
    smallnode_init();
    cfg = &SceneLevelConfig;
    AllocOtBuffers((s32 *)cfg);
    ((void (*)(u8 *))ResetScreen)((u8 *)(VideoMode == 0 ? &vmode_512_ntsc : &vmode_512_pal));
    vram_init(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    prims_init(cfg->pools);
    dbase_init_all();
    init_scripts(0x20, 0x80);
    SetupScriptScene(SceneArgs);
}

void UpdateSceneVabCues(void) {
    sfx_machine_bank_info *slot;
    sfx_machine_bank_info *base;
    s32 n;
    s32 ok;

    if (SceneVabCue == 0) {
        return;
    }
    if (VabCuePending != 0) {
        if (SceneVabCue->vab == 0) {
            ok = 1;
        } else {
            ok = ~SfxBanks[SceneVabSlot[0]].id != 0;
        }
        if (ok) {
            if (SceneVabCue->vab == 0) {
                return;
            }
            PlaySfx(SceneVabCue->sfx, NULL, NULL, 0x1000, 0x1E00, 0);
            VabCuePending -= 1;
            n = (SceneVabSlot[0] + 1) & 1;
            SceneVabSlot[0] = n;
            VabCueTimer = SceneVabCue->unk8;
            if (SfxBanks[n].id != -1) {
                sfx_bank_close((s32 *)&SfxBanks[n]);
            }
            SceneVabCue++;
        }
    }
    if (SceneVabCue->vab != 0) {
        base = SfxBanks;
        slot = &base[SceneVabSlot[0]];
        if (slot->head == NULL && slot->handle == 0 && loader_pending() == 0) {
            machine_load_sfx(SceneVabCue->vab, &base[SceneVabSlot[0]]);
        }
    }
}

void StartSceneScript(void) {
    MarkObj *o;
    MarkSrc *s;
    EngineMarker *m;
    EngineMarker *b;
    s32 i;
    s32 n;
    s32 v;
    s32 t;
    MarkSrcEnt *e;

    play_script((obj_dbase *)SceneMark->obj, SceneMark->unk8, &SceneScriptRot, &SceneScriptPos, 4, &SceneScript);
    SceneMarkCount = 0;
    t = (s32)SceneMark->obj;
    SceneOldDbase = t;
    o = (MarkObj *)t;
    if (o != NULL) {
        s = o->src;
        if (s != NULL) {
            i = 0;
            if (s->count > 0) {
                do {
                    e = s->ent;
                    if (((s16 *)&e[i])[-1] == 0x64) {
                        b = SceneMarks;
                        n = SceneMarkCount;
                        m = &b[n];
                        m->unk8 = o;
                        *(s32 *)m = i + 0x2001;
                        v = e[i].val;
                        SceneMarkCount = n + 1;
                        m->idx = v;
                    }
                } while (++i < s->count);
            }
        }
    }
    SceneViewport.isbg = SceneMark->unkC;
}

void UpdateScriptScene(void) {
    MarkObj *o;
    MarkObj *obj;
    s32 ok;

    GameFrameCount += 1;
    scripts_update();
    if (SceneMark != NULL) {
        if (SceneScript == NULL) {
            if ((s32)SceneMark->obj != SceneOldDbase && SceneOldDbase != 0) {
                dbase_release((u8 *)SceneOldDbase);
            }
            if (SceneMark->obj == NULL && SceneStep == 0) {
                if (SceneSkipProcess != 0) {
                    GotoProcess(SceneSkipProcess, 0x200);
                    SceneStep += 1;
                }
            } else {
                o = SceneMark->obj;
                ok = 0;
                if (o != NULL) {
                    ok = o->src != NULL;
                }
                if (ok) {
                    if (SceneStarting != 0) {
                        if (SceneVabCue == 0 || SceneVabCue->vab == 0) {
                            ok = 1;
                        } else {
                            ok = ~SfxBanks[SceneVabSlot[0]].id != 0;
                        }
                        if (!ok) {
                            goto end;
                        }
                    }
                    StartSceneScript();
                    SceneMark = SceneMark + 1;
                    if (SceneStarting != 0) {
                        VabCueTimer = 1;
                        SceneStarting = 0;
                        SceneStarted = 1;
                        set_fade(&ScreenFade, -0x800, 0);
                    }
                }
            }
        }
    end:
        if (SceneStep == 0) {
            obj = SceneMark->obj;
            if (obj != NULL) {
                dbase_asload_with_anims((obj_dbase *)obj, 0);
            }
        }
    }
    UpdateSceneVabCues();
    if ((Pads[0].pressed & 0x4000) && SceneStep == 0 && SceneSkipProcess != 0) {
        GotoProcess(SceneSkipProcess, 0x200);
        SceneStep += 1;
    }
}

void GotoScriptScene(ScriptScene *arg0, process_info *arg1) {
    SceneSkipProcess = arg1;
    SceneArgs = arg0;
    set_fade(&ScreenFade, 0x200, 0);
    ProcessControl.next_process = &ScriptSceneProcess;
}

void QuitScriptScene(void) {
    vab_transfer_wait();
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    ReleaseOldSceneModel();
    scripts_kill_all();
    vab_transfer_wait();
    sfx_all_off();
    if (SfxBanks[0].id != -1) {
        sfx_bank_close((s32 *)&SfxBanks[0]);
    }
    if (SfxBanks[1].id != -1) {
        sfx_bank_close((s32 *)&SfxBanks[1]);
    }
    sound_reinit();
    ClearVram();
    SceneMarkCount = 0;
    SceneStep += 1;
    VSyncRate = 2;
}

void RenderScriptScene(void) {
    gfx_set_viewport(&SceneViewport, &MainCamera);
    RenderState.modelDp = ScreenFade.amount;
    scripts_render();
    RenderState.modelDp = 0x1000 - (((0x1000 - ScriptFade) * (0x1000 - RenderState.modelDp)) >> 12);
    DrawSceneMarks();
}

s32 SceneModelLoaded(void) {
    s32 loaded;
    MarkObj *obj;

    obj = SceneMark->obj;
    loaded = 0;
    if (obj != NULL) {
        loaded = obj->src != NULL;
    }
    return loaded;
}

void ReleaseOldSceneModel(void) {
    if ((((s32)SceneMark->obj) != SceneOldDbase) && (SceneOldDbase != 0)) {
        dbase_release(SceneOldDbase);
    }
}

void LoadSceneModel(void) {
    s32 temp_a0;

    temp_a0 = ((s32)SceneMark->obj);
    if (temp_a0 != 0) {
        dbase_asload_with_anims(temp_a0, 0);
    }
}

void SceneNextProcess(void) {
    if (SceneSkipProcess != 0) {
        GotoProcess(SceneSkipProcess, 0x200);
        SceneStep += 1;
    }
}

void UnloadScene(void) {
    vab_transfer_wait();
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    ReleaseOldSceneModel();
    scripts_kill_all();
    vab_transfer_wait();
    sfx_all_off();
    if (SfxBanks[0].id != -1) {
        sfx_bank_close((s32 *)&SfxBanks[0]);
    }
    if (SfxBanks[1].id != -1) {
        sfx_bank_close((s32 *)&SfxBanks[1]);
    }
    sound_reinit();
    ClearVram();
    SceneMarkCount = 0;
    SceneStep += 1;
}

s32 SceneVabReady(void) {
    if (SceneVabCue == 0) {
        return 1;
    }
    if (*(s32 *)SceneVabCue == 0) {
        return 1;
    }
    return ~SfxBanks[SceneVabSlot[0]].id != 0;
}

void SceneSoundInit(ScriptScene *arg0) {
    SceneVabCue = arg0->cues;
    SceneVabSlot[0] = 0;
    SceneSoundHandle[0] = 0;
    sound_reinit();
    SfxBanks[0].id = -1;
    SfxBanks[0].handle = 0;
    SfxBanks[0].head = NULL;
    SfxBanks[1].id = -1;
    SfxBanks[1].handle = 0;
    SfxBanks[1].head = NULL;
    SceneStarted = 0;
    VabCueTimer = 0;
    VabCuePending = 0;
}

ScriptScene *PickModeVariant(s32 arg0) {
    s32 i;
    ModeScenes *e = ModeVariants;
    u8 *g;

    for (i = 0, g = GameInfo; i < 4; e++, i++) {
        if (arg0 == e->mode) {
            s16 m = *(s16 *)(g + 0x44);
            if (m == 1) {
                return e->scene[0];
            }
            if (m == 3) {
                return e->scene[2];
            }
            return e->scene[1];
        }
    }
    return 0;
}

void SetupSceneMarks(u8 *arg0) {
    MarkObj *o = (MarkObj *)arg0;
    MarkSrc *s;
    EngineMarker *m;
    EngineMarker *b;
    MarkSrcEnt *e;
    s32 i;
    s32 n;
    s32 v;

    SceneMarkCount = 0;
    if (o != NULL) {
        s = o->src;
        if (s != NULL) {
            do {
            i = 0;
            if (s->count > 0) {
                do {
                    e = s->ent;
                    if (((s16 *)&e[i])[-1] == 0x64) {
                        b = SceneMarks;
                        n = SceneMarkCount;
                        m = &b[n];
                        m->unk8 = o;
                        *(s32 *)m = i + 0x2001;
                        v = e[i].val;
                        SceneMarkCount = n + 1;
                        m->idx = v;
                    }
                } while (++i < s->count);
            }
            } while (0);
        }
    }
}

void ApplyPlaceRecs(subengine *sub, s32 *recs) {
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
                s->unk6C = (s32)(&HeroModelBank[rec->unk1C]);
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
                s->unk6C = (s32)&HeroModelBank[DB_Interface];
                {
                    u16 *tbl = IconImage;
                    s32 name = Heroes[rec->unk1E].hero_name;
                    s->unk74 = tbl[name];
                }
            }
            break;
        case 10:
            if (skip == 0) {
                s->hook = draw_animated_sub;
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
                s->unk6C = (s32)(&HeroModelBank[rec->unk1C]);
                s->unk74 = rec->unk1E;
            }
            break;
        case 5:
            if (skip == 0) {
                s->flags |= 0x10008000;
                s->x = rec->x;
                s->y = rec->y;
                s->z = rec->z;
                sub_play_script((obj_dbase *)(&HeroModelBank[rec->unk1C]), rec->unk1E, (subengine *)s, rec->unk20);
            }
            break;
        case 2:
            break;
        }
        rec++;
        s = s->next;
    }
}

void KillEngineSlot(s32 arg0) {
    EXTengine *eng;

    eng = &EnginePool[arg0];
    kill_subs(eng->sub);
    eng->flags &= 0xFFFF7FFF;
}

void InitEngineSlot(s32 arg0, s32 arg1) {
    EXTengine *eng;
    subengine *subs;
    subengine *sub;
    s32 n;

    eng = &EnginePool[arg0];
    *eng = EngineTemplate;
    subs = EngineSlotSubs[arg0];
    n = arg1;
    kill_subs(eng->sub);
    *eng = EngineTemplate;
    eng->unk5C = 1;
    eng->unk5D = 0;
    eng->flags |= 0x10000000;
    if (n > 0) {
        subs_init_array(eng, n, subs);
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub->flags &= 0xFFFF7FFF;
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

void PlaceEngineSlot(s32 arg0, s32 *recs) {
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
        subs = EngineSlotSubs[arg0];
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
            subs_init_array(eng, n, subs);
            ApplyPlaceRecs(eng->sub, recs);
        }
    }
}

/* Counts the 0x24-byte records before the first whose leading word is
 * below 2. */
s32 CountPlaceRecs(s32 *rec) {
    s32 n;

    n = 0;
    while (*rec >= 2) {
        rec += 9;
        n++;
    }
    return n;
}

void ResetEnginePool(void) {
    s32 i;

    for (i = 0; i < 9; i++) {
        EnginePool[i] = EngineTemplate;
    }
}

EXTengine *GetEngineSlot(s32 arg0) {
    return &EnginePool[arg0];
}

void draw_animated_sub(u8 *arg0, u8 *arg1, s32 arg2) {
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
    draw_object(arg0, arg1, arg2);
}

s32 PlaceFlashSub(s32 *p, s32 a1, s32 a2, s32 a3, s32 a4) {
    if (p == NULL) {
        return 0;
    }
    p[0] = 0x10008000;
    p[0x54 / 4] = (s32)posdst_flash;
    p[1] = a1;
    p[2] = a2;
    p[0x6C / 4] = a3;
    p[0x7C / 4] = a4;
    return p[0x5C / 4];
}

s32 PlaceTextSub(u8 *p, s16 a1, s32 a2, s16 a3, s32 a4, s32 idx) {
    Tmpl1C *t;
    if (p == NULL) {
        return 0;
    }
    t = &TextTemplates[idx];
    *(s32 *)p = t->f0 | 0x12008000;
    *(s32 *)(p + 0x54) = (s32)draw_nas_model;
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
extern s32 VramNodePool[];
extern u8 VramAreas[];
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
s32 ModeTextFiles[14] = {
    (s32)&DatIndex[2 * 722], (s32)&DatIndex[2 * 721], (s32)&DatIndex[2 * 720], (s32)&DatIndex[2 * 728], (s32)&DatIndex[2 * 727],
    (s32)&DatIndex[2 * 726], (s32)&DatIndex[2 * 725], (s32)&DatIndex[2 * 724], (s32)&DatIndex[2 * 487], (s32)&DatIndex[2 * 723],
    (s32)&DatIndex[2 * 719], (s32)&DatIndex[2 * 717], (s32)&DatIndex[2 * 716], (s32)&DatIndex[2 * 718],
};
s32 ModeTextFilesLang[14] = {  /* ModeTextTables' per-language file list */
    0, 0, (s32)&DatIndex[2 * 142], (s32)&DatIndex[2 * 151], (s32)&DatIndex[2 * 146], (s32)&DatIndex[2 * 141],
    (s32)&DatIndex[2 * 145], (s32)&DatIndex[2 * 152], (s32)&DatIndex[2 * 144], (s32)&DatIndex[2 * 143], (s32)&DatIndex[2 * 148],
    (s32)&DatIndex[2 * 147], (s32)&DatIndex[2 * 149], (s32)&DatIndex[2 * 150],
};
s32 *ModeTextTables[5] = {
    (void *)&ModeTextFilesLang, (void *)&ModeTextFilesLang, (void *)&ModeTextFilesLang, (void *)&ModeTextFilesLang,
    (void *)&ModeTextFilesLang,
};
s32 ModeTextLoaded = 0;
EngineCfg DefaultLevelConfig = {
    { 0, 0x20, 0, 0, 0x10, 0, 0, 0, 0, 1, 0, 0 }, { 0x80, 0x200, 0x20 }, (void *)&VramAreas,
    (void *)&VramNodePool, 0x20, 0x400,
};
u16 LoadingEntities[2] = { 0x2001, 0x2002 };
process_info LoadingProcess = { InitLoadingScreen, AnimateLoadingIcon, RenderLoadingScreen, QuitLoadingScreen, 0 };
u8 LoadingSceneViewport[40] = {
    0, 0, 0, 0, 0x80, 2, 0xE0, 1, 0, 0, 0, 0, 0x80, 2, 0xE0, 1, 0x40, 1, 0, 0, 0xF0, 0, 0, 0,
    0, 2, 0, 0, 0xFF, 0xD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
Blk32 LoadingScriptRot = { { 0x1000, 0, 0x1000, 0, 0x1000, 0, 0, 0 } };
CamVec3 LoadingScriptPos = {0};
process_info LoadingProcess2 = { InitLoadingCamera, UpdateLoadingScene, RenderLoadingScene, QuitLoadingScene, 0 };
u8 ModeDbaseLists[1516] = {
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
s32 AdvStatsFiles[5] =  /* DrawAdventureStats' and the save menu's loads */ { (s32)&DatIndex[2 * 809], (s32)&FileGroups[36], (s32)&HeroModelBank[DB_CharsSplashKong], (s32)&DatIndex[2 * 806], (s32)&DatIndex[2 * 805] };
s32 FileGroupNone[2] = {0};
u8 *ModeFileGroups[45] = {
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
    (void *)&file_group_TITLES, (void *)&FileGroupNone, (void *)&file_group_POLARDAY,
};
u16 LangPanelModels[6] = { 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0 };
process_info LevelLoadProcess = { InitLevelLoad, UpdateLevelLoad, RenderLevelLoad, QuitLevelLoad, 0 };
s32 map[26] = {0};
s32 LevelLoadStage = 0;
process_control_info LoadingControl = {0};
camera_info_tag LoadingCamera = {0};
EXTengine LoadingIconEngine = {0};  /* the loading screen's icon (InitLoadingScreen) */
s32 LoadingScenePlaying = 0;
s32 LoadingSceneDelay = 0;
s32 LoadingUnused[4] = {0};
EngineMarkerPos SceneMarkPos[3] = {
    { 0x7FB, 0x1FC3, 0xFA0, 0x7D0, { 0, 0, 0, 0, 0, 0 } },
    { 0x1FC3, 0xFA0, 0x7E4, 0x5DC, { 0, 0, 0, 0, 0, 0 } },
    { 0, 0, 0, 0x3E8, { 0, 0, 0, 0, 0, 0 } },
};
EngineCfg SceneLevelConfig = {
    { 0, 0x10, 0, 0, 0x10, 0, 0, 0, 0, 0x10, 0, 0 }, { 0x80, 0x200, 0x20 },
    (void *)(VramAreas + 0x10), (void *)&VramNodePool, 0x10, 0x400,
};
s32 ExitAMarks[52] = {
    1, (s32)&HeroModelBank[DB_CutsceneLevelEndingGoodShot3], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot8], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot7], 0, 1, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelShot6], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelEndingGoodShot2], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot5], 0, 1, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelShot4], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot3], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot2], 0, 1, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelEndingEvilShot1], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelEndingGoodShot1], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelIntro], 0, 1, 0, 0, 0, 0,
};
s32 ExitAAudio[15] = {
    (s32)&DatIndex[2 * 262], 0x780, 0x4AD, (s32)&DatIndex[2 * 261], 0x781, 0x4AD, (s32)&DatIndex[2 * 260], 0x782,
    0x4AD, (s32)&DatIndex[2 * 259], 0x783, 0x4AD, 0, 0, 0,
};
s32 SceneTextGoodwinsMarks[14] = { 1, (s32)&HeroModelBank[DB_CutsceneTextGoodwins], 0, 1, 0, 0, 0, 0, (s32)&DatIndex[2 * 271], 0x76C, 0x4AD, 0, 0, 0 };
s32 ExitBMarks[40] = {
    1, (s32)&HeroModelBank[DB_CutsceneLevelEndingEvilShot8], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot11], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot10], 0, 1, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelEndingEvilShot7], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelEndingEvilShot6], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot9], 0, 1, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelEndingEvilShot5], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelEndingEvilShot3], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevelEndingEvilShot2], 0, 1, 0, 0, 0, 0,
};
s32 ExitBAudio[20] = {
    (s32)&DatIndex[2 * 258], 0x78A, 0x4AD, (s32)&DatIndex[2 * 257], 0x78B, 0x4AD, (s32)&DatIndex[2 * 256], 0x78C,
    0x4AD, 0, 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneTextEvilwins], 0, 0, 0, 0, 0, 0,
};
s32 SceneTextMixteamwinsMarks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneTextMixteamwins], 0, 1, 0, 0, 0, 0 };
s32 ExitCMarks[20] = {
    1, (s32)&HeroModelBank[DB_CutsceneLevel4], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevel3], 0, 1, 1, (s32)&HeroModelBank[DB_CutsceneLevel2], 0, 1, 1,
    (s32)&HeroModelBank[DB_CutsceneLevel], 0, 1, 0, 0, 0, 0,
};
s32 ExitCAudio[12] = {
    (s32)&DatIndex[2 * 265], 0x776, 0x4AD, (s32)&DatIndex[2 * 264], 0x777, 0x4AD, (s32)&DatIndex[2 * 263], 0x778,
    0x4AD, 0, 0, 0,
};
s32 SceneTextCreditsMarks[16] = { 1, (s32)&HeroModelBank[DB_CutsceneTextCredits], 0, 1, 0, 0, 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneTextCredits], 0, 1, 0, 0, 0, 0 };
s32 SceneTextIntromovieMarks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneTextIntromovie], 0, 1, 0, 0, 0, 0 };
s32 IntroMovieMarks[80] = {
    1, (s32)&HeroModelBank[DB_CutsceneIntroLogo], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot19], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot18], 0, 0, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelShot17], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot16], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot15], 0, 0, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelShot14], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelIntroCrashplain], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelIntroCortexlab], 0, 0, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelIntroShotWelcome3], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelIntroShotWelcome2], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelIntroShotWelcome], 0, 0, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamevil4], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamevil3], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot13], 0, 0, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamevil2], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelShot12], 0, 0, 1, (s32)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamevil], 0, 0, 1,
    (s32)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamgood], 0, 0, 0, 0, 0, 0,
};
s32 SceneSharedAudio[36] = {
    (s32)&DatIndex[2 * 255], 0x794, 0x255, (s32)&DatIndex[2 * 254], 0x795, 0x255, (s32)&DatIndex[2 * 253], 0x796,
    0x255, (s32)&DatIndex[2 * 252], 0x797, 0x255, (s32)&DatIndex[2 * 251], 0x798, 0x255, (s32)&DatIndex[2 * 250],
    0x799, 0x255, (s32)&DatIndex[2 * 249], 0x79A, 0x255, (s32)&DatIndex[2 * 248], 0x79B, 0x255,
    (s32)&DatIndex[2 * 247], 0x79C, 0x255, (s32)&DatIndex[2 * 246], 0x79D, 0x255, (s32)&DatIndex[2 * 245], 0x79E,
    0x255, 0, 0, 0,
};
ScriptScene IntroMovieMsgs[2] = {
    { (void *)&IntroMovieMarks, (void *)&SceneSharedAudio, 0x1DFC40 },
    { (void *)&SceneTextIntromovieMarks, (void *)&SceneSharedAudio, 0x1E0000 },
};
ScriptScene LevelExitMsgsA[2] = {
    { (void *)&ExitAMarks, (void *)&ExitAAudio, 0x1E0000 },
    { (void *)&SceneTextGoodwinsMarks, (void *)&SceneSharedAudio, 0x1E0000 },
};
ScriptScene LevelExitMsgsB[2] = {
    { (void *)&ExitBMarks, (void *)&ExitBAudio, 0x1E0000 },
    { (void *)&ExitBMarks, (void *)&SceneSharedAudio, 0x1E0000 },
};
ScriptScene LevelExitMsgsC[4] = {
    { (void *)&ExitCMarks, (void *)&ExitCAudio, 0x1E0000 },
    { (void *)&SceneTextMixteamwinsMarks, (void *)&SceneSharedAudio, 0x1E0000 },
    { (void *)&SceneTextCreditsMarks, (void *)&SceneSharedAudio, 0x1E0000 },
    { (void *)&SceneTextCreditsMarks, (void *)&SceneSharedAudio, 0x1E0000 },
};
s32 SceneUkaData4Audio[6] = { (s32)&DatIndex[2 * 224], 0x3DD, 0, 0, 0, 0 };
s32 SceneUkaData3Audio[6] = { (s32)&DatIndex[2 * 225], 0x3DA, 0, 0, 0, 0 };
s32 SceneUkaData2Audio[6] = { (s32)&DatIndex[2 * 222], 0x3E3, 0, 0, 0, 0 };
s32 SceneUkaDataAudio[6] = { (s32)&DatIndex[2 * 223], 0x3E0, 0, 0, 0, 0 };
s32 SceneUkaData4Marks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneUkaData4], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneUkaData4 = { (void *)&SceneUkaData4Marks, (void *)&SceneUkaData4Audio, 0x1E0000 };
s32 SceneUkaData3Marks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneUkaData3], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneUkaData3 = { (void *)&SceneUkaData3Marks, (void *)&SceneUkaData3Audio, 0x1E0000 };
s32 SceneUkaDataMarks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneUkaData], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneUkaData = { (void *)&SceneUkaDataMarks, (void *)&SceneUkaDataAudio, 0x1E0000 };
s32 SceneUkaData2Marks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneUkaData2], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneUkaData2 = { (void *)&SceneUkaData2Marks, (void *)&SceneUkaData2Audio, 0x1E0000 };
s32 SceneAkuData4Audio[6] = { (s32)&DatIndex[2 * 220], 0x3DC, 0, 0, 0, 0 };
s32 SceneAkuData3Audio[6] = { (s32)&DatIndex[2 * 221], 0x3D9, 0, 0, 0, 0 };
s32 SceneAkuData2Audio[6] = { (s32)&DatIndex[2 * 218], 0x3E2, 0, 0, 0, 0 };
s32 SceneAkuDataAudio[6] = { (s32)&DatIndex[2 * 219], 0x3DF, 0, 0, 0, 0 };
s32 SceneAkuData4Marks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneAkuData4], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneAkuData4 = { (void *)&SceneAkuData4Marks, (void *)&SceneAkuData4Audio, 0x1E0000 };
s32 SceneAkuData3Marks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneAkuData3], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneAkuData3 = { (void *)&SceneAkuData3Marks, (void *)&SceneAkuData3Audio, 0x1E0000 };
s32 SceneAkuDataMarks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneAkuData], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneAkuData = { (void *)&SceneAkuDataMarks, (void *)&SceneAkuDataAudio, 0x1E0000 };
s32 SceneAkuData2Marks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneAkuData2], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneAkuData2 = { (void *)&SceneAkuData2Marks, (void *)&SceneAkuData2Audio, 0x1E0000 };
s32 SceneAkuUkaData2Audio[6] = { (s32)&DatIndex[2 * 214], 0x3E4, 0, 0, 0, 0 };
s32 SceneAkuUkaDataAudio[6] = { (s32)&DatIndex[2 * 215], 0x3E1, 0, 0, 0, 0 };
s32 SceneAkuUkaDataMarks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneAkuUkaData], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneAkuUkaData = { (void *)&SceneAkuUkaDataMarks, (void *)&SceneAkuUkaDataAudio, 0x1E0000 };
s32 SceneAkuUkaData2Marks[8] = { 1, (s32)&HeroModelBank[DB_CutsceneAkuUkaData2], 0, 0, 0, 0, 0, 0 };
ScriptScene SceneAkuUkaData2 = { (void *)&SceneAkuUkaData2Marks, (void *)&SceneAkuUkaData2Audio, 0x1E0000 };
ModeScenes ModeVariants[4] = {
    { 4, { &SceneAkuData3, &SceneUkaData3, &SceneAkuData3 } },
    { 0x16, { &SceneAkuData4, &SceneUkaData4, &SceneAkuData4 } },
    { 0x1F, { &SceneAkuData2, &SceneUkaData2, &SceneAkuUkaData2 } },
    { 9, { &SceneAkuData, &SceneUkaData, &SceneAkuUkaData } },
};
Blk32 SceneScriptRot = { { 0x1000, 0, 0x1000, 0, 0x1000, 0, 0, 0 } };
CamVec3 SceneScriptPos = {0};
GfxViewport SceneViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0xF0, 0x200, 0xDFF, 0, 0, 0, 0, 1 };
process_info ScriptSceneProcess = { InitScriptScene, UpdateScriptScene, RenderScriptScene, QuitScriptScene, 0 };
process_info *SceneSkipProcess = 0;
ScriptScene *SceneArgs = 0;
EngineMarker SceneMarks[8] = {0};
s32 SceneMarkCount = 0;
subengine SceneMarkSub = {0};
EngineVabCue *SceneVabCue = 0;
s32 SceneVabSlot[1] = {0};
s32 SceneSoundHandle[1] = {0};
s32 SceneStarted = 0;
MarkCtx *SceneMark = 0;
s32 SceneOldDbase = 0;
script_handler *SceneScript = 0;
s32 SceneStarting = 0;
s32 SceneStep = 0;
s32 SceneUnused[1] = {0};
Tmpl1C TextTemplates[11] = {
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
subengine EngineSlotSubs[9][32] = {0};
