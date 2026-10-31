#include "../../../platform/settings.h"
#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/levelload.h"
#include "bash/hero.h"
#include "bash/shell.h"

/* Loads the mode's text (into ModeTextBuffer) and its minigame overlay (into the slot, unless it
 * is there already); returns where the overlay ends, the heap's new start. */
u8 *LoadModeOverlay(void) {
    GameState *g;
    file *p;
    file ***tab = ModeTextTables;
    file ***rp;

    g = (GameState *)GameInfo;
    rp =&tab[GAME->language];
    p = (*rp)[ModeTypes[g->mode * 3] - 0x101];
    if (p != NULL) {
        load_file_direct(p, ModeTextLoadAddr);
        ModeTextTranslate(p->size);
    }
    p = ModeOverlayFiles[ModeTypes[g->mode * 3] - 0x101];
    if (p != ModeOverlayLoaded) {
        overlay_load(p);
        ModeOverlayLoaded = p;
    }
    return HEAP_START;
}

void AnimateLoadingIcon(void) {
    TrigEntry *t;
    subengine *o;
    s32 f;
    s32 i;

    if (LevelLoadStage >= 2 && (o = LoadingIconEngine.sub) != NULL) {
        t = rcossin_tbl;
        o->xform.order = 0;
        o->xform.rot.vy += 0x20;
        f = FrameCount;
        o->xform.rot.vx = *(s32 *)&t[(((f * 70) & 0xFFF) - 0x400) & 0xFFF] >> 20;
        o->xform.rot.vy = *(s32 *)&t[(((f * 140) & 0xFFF) - 0x400) & 0xFFF] >> 21;
        o->xform.rot.vz = (t[(((f * 70) & 0xFFF) - 0x400) & 0xFFF].unk2 +
                    t[(((f * 100) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 5;
        o->XYZ.x = *(s32 *)&t[(((f * 35) & 0xFFF) - 0x400) & 0xFFF] >> 23;
        o->XYZ.y = (t[(((f * 45) & 0xFFF) - 0x400) & 0xFFF].unk2 +
                   t[(((f * 145) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 5;
        o->xform.dirty = 1;
        o = o->next;
        if (o != NULL) {
            o->xform.rot.vz += 0x17;
            i = (-f * 10) & 0xFFF;
            o->xform.rot.vx = *(s32 *)&t[(i - 0x400) & 0xFFF] >> 20;
            o->xform.rot.vy = *(s32 *)&t[i] >> 20;
            o->xform.dirty = 1;
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
static inline void InitHeapToRamTop(_mal_area *heap) {
    mal_init(heap, HEAP_START, HEAP_END - HEAP_START);
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
    InitHeapToRamTop(&MainHeap);
    LoadModeOverlay();
    InitHeapToRamTop(&MainHeap);
    SetupLevelSpecs(DefaultArenaDef);
    cfg = (EngineCfg *)LevelConfig;
    if (cfg == NULL) {
        cfg = &DefaultLevelConfig;
    }
    AllocOtBuffers((s32 *)cfg);
    ((void (*)(u8 *))ResetScreen)((u8 *)(VideoMode == 0 ? &vmode_512_ntsc : &vmode_512_pal));
    GAME->winner[0] = 0;
    GAME->winner[1] = 0;
    vram_init(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    prims_init(cfg->pools);
    dbase_init_all();
    sound_init();
    draw_init();
    LevelLoadStage = 0;
    if (((GameState *)GameInfo)->mode == 0x28 && FirstBootDone != 0) {
        ProcessControl.flags = 0x1D;
        ProcessControl.next_process = &RunGame;
    }
}

void QueueHeroModels(void) {
    GameState *g;
    void **arg;
    s32 i;
    EngineHooks *tbl;
    s32 c;
    u8 *base;
    s32 j;
    obj_dbase *e;
    GameState *a = (GameState *)GameInfo;

    i = 0;
    arg = (void **)LevelRecs[a->levelRecIndex].Anims;
    {
        s32 n = LangPanelModels[a->language];
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
                    ModeDbase *p = ModeDbaseLists;
                    e = &((obj_dbase *)base)[((u16 *)tbl->HeroModels)[c]];
                    for (j = 0; j < 0x60; j++) {
                        if (e == p->dbase) {
                            queue_dbase_load((u8 *)p, arg);
                        }
                        p++;
                    }
                }
                if (tbl->flag != NULL) {
                    ModeDbase *p = ModeDbaseLists;
                    e = &((obj_dbase *)base)[((u16 *)tbl->flag)[c]];
                    for (j = 0; j < 0x60; j++) {
                        if (e == p->dbase) {
                            queue_dbase_load((u8 *)p, arg);
                        }
                        p++;
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
  GameState *g;
  char **arg;
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
  idx = ((GameState *) tg)->levelRecIndex;
  g = (GameState *) tg;
  tg = (u8 *) LevelRecs;
  arg = ((EngineRec32 *) tg)[idx].Anims;
  t = (u8 *)HeroModelBank;
  h = &HeroHooks;
  src = (s32 *) (&((obj_dbase *) t)[new_var[g->language]]);
  ((obj_dbase *) t)[DB_Interface].file = ((obj_dbase *) src)->file;
  b = t;
  ((obj_dbase *) b)[DB_Interface].textureFile = ((obj_dbase *) src)->textureFile;
  do
  {
    k = ((u8 *) g)[i + 0x50];
    if ((k - 1) < 8)
    {
      dbase_asload_with_anims(&((obj_dbase *) b)[((u16 *) h->HeroModels)[k]], (void **)arg);
      if (h->flag != 0)
      {
        dbase_asload_with_anims(&((obj_dbase *) b)[((u16 *) h->flag)[k]], (void **)arg);
      }
    }
    i++;
  }
  while (i < 4);

  p = LevelTriggerData->types;
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
  p = LevelRecs[LevelRecIndex].LevelDB;
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


static void LevelLoadStep(void) {
    GameState *g0;
    GameState *g;
    map_map *c;
    s32 *a;
    s32 *p;

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
        g0 = (GameState *)GameInfo;
        g0->cupLeader = -1;
        g0->adventure.roundPoints[0] = 0;
        g0->adventure.roundPoints[1] = 0;
        g0->adventure.roundPoints[2] = 0;
        g0->adventure.roundPoints[3] = 0;
        LevelLoadStage = 1;
        g0->exitFlags &= ~0x200;
        break;
    case 1:
        SetNasDefaults();
        g = (GameState *)GameInfo;
        g->exitFlags = 0;
        g->pause = 0;
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
            sfx_bank_load(&DatIndex[273]);
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
        GameViewport.isbg = MapScene->map->clear;
        c = MapScene->map;
        GameViewport.bg = c->clear_colour;
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

void UpdateLevelLoad(void) {
    if (settings()->skip_loading) {
        /* every stage now, with no frames in between to show a loading screen in */
        while (LevelLoadStage != 9) {
            while (loader_pending() != 0 || MainHeap.deferred != NULL) {
                loader_update();
                mal_update(&MainHeap);
            }
            LevelLoadStep();
        }
        return;
    }
    if (LevelLoadStage >= 2) {
        process_control_update(&LoadingControl, NULL);
    }
    vab_transfer_wait();
    mal_count_free(&MainHeap);
    if (loader_pending() != 0 || MainHeap.deferred != NULL) {
        return;
    }
    LevelLoadStep();
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
    if (LevelLoadStage >= 2 && !settings()->skip_loading) {
        process_control_render(&LoadingControl);
    }
}

void LoadModeDbases(obj_dbase *arg0, void **arg1) {
    s32 i;
    ModeDbase *p = ModeDbaseLists;

    for (i = 0; i < 0x60; i++) {
        if (arg0 == p->dbase) {
            queue_dbase_load((u8 *)p, arg1);
        }
        p++;
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
    eng->xform.order = 0;
    eng->XYZ.x = 0;
    eng->XYZ.y = 0;
    eng->flags |= 0x10000000;
    eng_add_subs(eng, 2);
    sub = eng->sub;
    p = LoadingEntities;
    if (sub != NULL) {
        do {
            sub->u6C.dbase = &HeroModelBank[DB_Loading];
            sub->flags = 0x01008000;
            sub->entity = *p;
            sub->XYZ.x = 0;
            sub->XYZ.y = 0;
            sub->XYZ.z = 0;
            sub = sub->next;
            p++;
            if (sub != NULL) sub->xform.order = 0;
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
        e->entity = m->entity;
        e->u6C.dbase = m->dbase;
        e->xform.order = 0;
        s = GameFrameCount;
        e->xform.rot.vx = (s * p->x) >> 12;
        e->xform.rot.vy = (s * p->y) >> 12;
        e->xform.rot.vz = (s * p->z) >> 12;
        e->flags = 0x03008000;
        e->otz = p->otz;
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
    ModeOverlayLoaded = 0;
    mal_init(&MainHeap, HEAP_START, HEAP_END - HEAP_START);
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
            VabCueTimer = SceneVabCue->timer;
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

    play_script((obj_dbase *)SceneMark->obj, SceneMark->script, &SceneScriptRot, &SceneScriptPos, 4, &SceneScript);
    SceneMarkCount = 0;
    SceneOldDbase = (obj_dbase *)SceneMark->obj;
    o = SceneMark->obj;
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
                        m->dbase = o;
                        *(s32 *)m = i + 0x2001;
                        v = e[i].val;
                        SceneMarkCount = n + 1;
                        m->idx = v;
                    }
                } while (++i < s->count);
            }
        }
    }
    SceneViewport.isbg = SceneMark->isbg;
}

void UpdateScriptScene(void) {
    MarkObj *o;
    MarkObj *obj;
    s32 ok;

    GameFrameCount += 1;
    scripts_update();
    if (SceneMark != NULL) {
        if (SceneScript == NULL) {
            if ((obj_dbase *)SceneMark->obj != SceneOldDbase && SceneOldDbase != 0) {
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
    if (((obj_dbase *)SceneMark->obj != SceneOldDbase) && (SceneOldDbase != 0)) {
        dbase_release(SceneOldDbase);
    }
}

void LoadSceneModel(void) {
    obj_dbase *temp_a0;

    temp_a0 = (obj_dbase *)SceneMark->obj;
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
    GameState *g;

    for (i = 0, g = GAME; i < 4; e++, i++) {
        if (arg0 == e->mode) {
            s16 m = g->teamSide[0];
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
                        m->dbase = o;
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

void ApplyPlaceRecs(subengine *sub, PlaceRec *recs) {
    PlaceSub *s;
    PlaceRec *rec;
    s32 skip;

    s = (PlaceSub *)sub;
    rec = recs;
    skip = 0;
    for (;;) {
        if (s == NULL || rec->type < 2) {
            break;
        }
        switch (rec->type) {
        case 13:
            switch (rec->flags & 0x8F0) {
            case 0x800:
                skip = 0;
                break;
            case 0x20:
                skip = ((GameState *)GameInfo)->teams == 0;
                break;
            case 0x10:
                skip = ((GameState *)GameInfo)->teams;
                break;
            case 0x30:
                skip = ((GameState *)GameInfo)->pause ^ 1;
                skip &= 1;
                break;
            case 0x40:
                skip = ((GameState *)GameInfo)->pause & 1;
                break;
            case 0x60:
                skip = 0;
                if (!(((GameState *)GameInfo)->pause & 1) ||
                    !(((GameState *)GameInfo)->levelFlags & 0x10)) {
                    skip = 1;
                }
                break;
            case 0x50:
                skip = 0;
                if (((GameState *)GameInfo)->pause & 1) {
                    skip = (u32)skip < (((GameState *)GameInfo)->levelFlags & 0x10);
                }
                break;
            case 0x70:
                skip = 0;
                if (!(((GameState *)GameInfo)->pause & 1) ||
                    !(((GameState *)GameInfo)->levelFlags & 0xE0)) {
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
                                ((void **)rec->data)[((GameState *)GameInfo)->language],
                                rec->frame);
                } else {
                    PlaceSprite(s, rec->x, rec->y, rec->data, rec->frame);
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
                              ((s32 *)rec->data)[((GameState *)GameInfo)->language],
                              rec->z, rec->scale, rec->rx);
                } else {
                    PlaceTmpl(s, rec->x, rec->y, rec->z, rec->scale, rec->rx);
                }
                if (rec->type == 12) {
                    s->flags &= 0xEFFFFFFF;
                    s->w *= 8;
                    s->h *= 8;
                    s->z = rec->ry;
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
                s->unk6C = &HeroModelBank[rec->model];
                s->unk74 = rec->player;
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
                s->unk6C = &HeroModelBank[DB_Interface];
                {
                    u16 *tbl = IconImage;
                    s32 name = Heroes[rec->player].hero_name;
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
                s->rx = rec->rx;
                s->ry = rec->ry;
                s->rz = rec->rz;
                s->unk6C = &HeroModelBank[rec->model];
                s->unk74 = rec->player;
            }
            break;
        case 5:
            if (skip == 0) {
                s->flags |= 0x10008000;
                s->x = rec->x;
                s->y = rec->y;
                s->z = rec->z;
                sub_play_script((obj_dbase *)(&HeroModelBank[rec->model]), rec->player, (subengine *)s, rec->flags);
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
    eng->xform.hasScale = 1;
    eng->xform.order = 0;
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

void PlaceEngineSlot(s32 arg0, PlaceRec *recs) {
    EXTengine *eng;
    subengine *subs;
    PlaceRec *rec;
    s32 i;
    s32 n;

    if (recs != NULL) {
        eng = &EnginePool[arg0];
        *eng = EngineTemplate;
        rec = recs;
        i = 0;
        subs = EngineSlotSubs[arg0];
        while (rec->type >= 2) {
            rec++;
            i++;
        }
        n = i;
        kill_subs(eng->sub);
        *eng = EngineTemplate;
        eng->xform.hasScale = 1;
        eng->xform.order = 0;
        eng->flags |= 0x10000000;
        if (n > 0) {
            subs_init_array(eng, n, subs);
            ApplyPlaceRecs(eng->sub, recs);
        }
    }
}

/* Counts the records before the first whose type is below 2. */
s32 CountPlaceRecs(PlaceRec *rec) {
    s32 n = 0;

    while (rec[n].type >= 2) {
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

void draw_animated_sub(PlaceSub *arg0, u8 *arg1, s32 arg2) {
    PlaceRec *p;
    u32 flags;
    u32 n;
    u32 m;

    p = arg0->rec;
    flags = p->flags;
    if (flags & 1) {
        arg0->rx += p->rx;
    }
    if (flags & 2) {
        arg0->ry += p->ry;
    }
    if (flags & 4) {
        arg0->rz += p->rz;
    }
    if (flags & 8) {
        n = p->frame;
        if (n != 0) {
            m = FrameCount % n + 1;
            arg0->unk74 = p->player + ((u16)p->frame - m);
        }
    }
    arg0->unk52 = 1;
    draw_object((u8 *)arg0, arg1, arg2);
}

void *PlaceFlashSub(PlaceSub *p, s32 a1, s32 a2, void *a3, s32 a4) {
    if (p == NULL) {
        return 0;
    }
    p->flags = 0x10008000;
    p->hook = posdst_flash;
    p->x = a1;
    p->y = a2;
    p->unk6C = a3;
    p->unk7C = a4;
    return p->next;
}

void *PlaceTextSub(PlaceSub *p, s16 a1, s32 a2, s16 a3, s32 a4, s32 idx) {
    sub_string_info *t;
    if (p == NULL) {
        return 0;
    }
    t = &TextTemplates[idx];
    p->flags = t->f0 | 0x12008000;
    p->hook = draw_nas_model;
    if ((a1 & 0xC000) == 0x8000) {
        a2 = a2 * 0x280 / ScreenRect->w;
    }
    p->unk6C = (void *)(iptr)t->f0;
    p->unk70 = a1;
    p->unk76 = a2;
    p->unk72 = a3;
    p->unk78 = a4;
    p->unk7C = t->f4;
    p->w = t->f10;
    p->h = t->f14;
    p->unk80 = t->fC;
    p->unk88 = t->f8;
    p->unk68 = t->f18;
    return p->next;
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
/* each mode type's minigame overlay (DAT entries 487 and 716..728), from mode type 0x101 */
file *ModeOverlayFiles[14] = {
    &DatIndex[722], &DatIndex[721], &DatIndex[720], &DatIndex[728], &DatIndex[727],
    &DatIndex[726], &DatIndex[725], &DatIndex[724], &DatIndex[487], &DatIndex[723],
    &DatIndex[719], &DatIndex[717], &DatIndex[716], &DatIndex[718],
};
file *ModeTextFilesLang[14] = {  /* ModeTextTables' per-language file list */
    0, 0, &DatIndex[142], &DatIndex[151], &DatIndex[146], &DatIndex[141],
    &DatIndex[145], &DatIndex[152], &DatIndex[144], &DatIndex[143], &DatIndex[148],
    &DatIndex[147], &DatIndex[149], &DatIndex[150],
};
file **ModeTextTables[5] = {
    (void *)&ModeTextFilesLang, (void *)&ModeTextFilesLang, (void *)&ModeTextFilesLang, (void *)&ModeTextFilesLang,
    (void *)&ModeTextFilesLang,
};
file *ModeOverlayLoaded = 0;
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
/* the models each mode loads (LoadModeDbases, QueueHeroModels; the last is the adventure stats') */
ModeDbase ModeDbaseLists[96] = {
    { &FileGroups[128], &HeroModelBank[DB_CharsCrateCrash], &DatIndex[990], &DatIndex[989] },
    { &FileGroups[127], &HeroModelBank[DB_CharsCrateCoco], &DatIndex[988], &DatIndex[987] },
    { &FileGroups[126], &HeroModelBank[DB_CharsCrateCortex], &DatIndex[986], &DatIndex[985] },
    { &FileGroups[125], &HeroModelBank[DB_CharsCrateBrio], &DatIndex[984], &DatIndex[983] },
    { &FileGroups[123], &HeroModelBank[DB_CharsCrateDingo], &DatIndex[980], &DatIndex[979] },
    { &FileGroups[122], &HeroModelBank[DB_CharsCrateRilla], &DatIndex[978], &DatIndex[977] },
    { &FileGroups[124], &HeroModelBank[DB_CharsCrateTiny], &DatIndex[982], &DatIndex[981] },
    { &FileGroups[121], &HeroModelBank[DB_CharsCrateKong], &DatIndex[976], &DatIndex[975] },
    { &FileGroups[120], &HeroModelBank[DB_CharsPogoCrash], &DatIndex[974], &DatIndex[973] },
    { &FileGroups[119], &HeroModelBank[DB_CharsPogoCoco], &DatIndex[972], &DatIndex[971] },
    { &FileGroups[118], &HeroModelBank[DB_CharsPogoCortex], &DatIndex[970], &DatIndex[969] },
    { &FileGroups[117], &HeroModelBank[DB_CharsPogoBrio], &DatIndex[968], &DatIndex[967] },
    { &FileGroups[114], &HeroModelBank[DB_CharsPogoDingo], &DatIndex[962], &DatIndex[961] },
    { &FileGroups[113], &HeroModelBank[DB_CharsPogoRilla], &DatIndex[960], &DatIndex[959] },
    { &FileGroups[116], &HeroModelBank[DB_CharsPogoTiny], &DatIndex[966], &DatIndex[965] },
    { &FileGroups[115], &HeroModelBank[DB_CharsPogoKong], &DatIndex[964], &DatIndex[963] },
    { &FileGroups[112], &HeroModelBank[DB_CharsPolar2Crash], &DatIndex[958], &DatIndex[957] },
    { &FileGroups[111], &HeroModelBank[DB_CharsPolar2Coco], &DatIndex[956], &DatIndex[955] },
    { &FileGroups[110], &HeroModelBank[DB_CharsPolar2Cortex], &DatIndex[954], &DatIndex[953] },
    { &FileGroups[107], &HeroModelBank[DB_CharsPolar2Brio], &DatIndex[948], &DatIndex[947] },
    { &FileGroups[108], &HeroModelBank[DB_CharsPolar2Dingo], &DatIndex[950], &DatIndex[949] },
    { &FileGroups[105], &HeroModelBank[DB_CharsPolar2Rilla], &DatIndex[944], &DatIndex[943] },
    { &FileGroups[109], &HeroModelBank[DB_CharsPolar2Tiny], &DatIndex[952], &DatIndex[951] },
    { &FileGroups[106], &HeroModelBank[DB_CharsPolar2Kong], &DatIndex[946], &DatIndex[945] },
    { &FileGroups[104], &HeroModelBank[DB_CharsCrate2Crash], &DatIndex[942], &DatIndex[941] },
    { &FileGroups[103], &HeroModelBank[DB_CharsCrate2Coco], &DatIndex[940], &DatIndex[939] },
    { &FileGroups[102], &HeroModelBank[DB_CharsCrate2Cortex], &DatIndex[938], &DatIndex[937] },
    { &FileGroups[99], &HeroModelBank[DB_CharsCrate2Brio], &DatIndex[932], &DatIndex[931] },
    { &FileGroups[100], &HeroModelBank[DB_CharsCrate2Dingo], &DatIndex[934], &DatIndex[933] },
    { &FileGroups[97], &HeroModelBank[DB_CharsCrate2Rilla], &DatIndex[928], &DatIndex[927] },
    { &FileGroups[101], &HeroModelBank[DB_CharsCrate2Tiny], &DatIndex[936], &DatIndex[935] },
    { &FileGroups[98], &HeroModelBank[DB_CharsCrate2Kong], &DatIndex[930], &DatIndex[929] },
    { &FileGroups[96], &HeroModelBank[DB_CharsPolarCrash], &DatIndex[926], &DatIndex[925] },
    { &FileGroups[95], &HeroModelBank[DB_CharsPolarCoco], &DatIndex[924], &DatIndex[923] },
    { &FileGroups[94], &HeroModelBank[DB_CharsPolarCortex], &DatIndex[922], &DatIndex[921] },
    { &FileGroups[91], &HeroModelBank[DB_CharsPolarBrio], &DatIndex[916], &DatIndex[915] },
    { &FileGroups[92], &HeroModelBank[DB_CharsPolarDingo], &DatIndex[918], &DatIndex[917] },
    { &FileGroups[90], &HeroModelBank[DB_CharsPolarRilla], &DatIndex[914], &DatIndex[913] },
    { &FileGroups[93], &HeroModelBank[DB_CharsPolarTiny], &DatIndex[920], &DatIndex[919] },
    { &FileGroups[89], &HeroModelBank[DB_CharsPolarKong], &DatIndex[912], &DatIndex[911] },
    { &FileGroups[88], &HeroModelBank[DB_CharsBallCrash], &DatIndex[910], &DatIndex[909] },
    { &FileGroups[87], &HeroModelBank[DB_CharsBallCoco], &DatIndex[908], &DatIndex[907] },
    { &FileGroups[86], &HeroModelBank[DB_CharsBallCortex], &DatIndex[906], &DatIndex[905] },
    { &FileGroups[85], &HeroModelBank[DB_CharsBallBrio], &DatIndex[904], &DatIndex[903] },
    { &FileGroups[84], &HeroModelBank[DB_CharsBallDingo], &DatIndex[902], &DatIndex[901] },
    { &FileGroups[83], &HeroModelBank[DB_CharsBallRilla], &DatIndex[900], &DatIndex[899] },
    { &FileGroups[82], &HeroModelBank[DB_CharsBallTiny], &DatIndex[898], &DatIndex[897] },
    { &FileGroups[81], &HeroModelBank[DB_CharsBallKong], &DatIndex[896], &DatIndex[895] },
    { &FileGroups[80], &HeroModelBank[DB_CharsTankCrash], &DatIndex[894], &DatIndex[893] },
    { &FileGroups[79], &HeroModelBank[DB_CharsTankCoco], &DatIndex[892], &DatIndex[891] },
    { &FileGroups[78], &HeroModelBank[DB_CharsTankCortex], &DatIndex[890], &DatIndex[889] },
    { &FileGroups[76], &HeroModelBank[DB_CharsTankBrio], &DatIndex[886], &DatIndex[885] },
    { &FileGroups[75], &HeroModelBank[DB_CharsTankDingo], &DatIndex[884], &DatIndex[883] },
    { &FileGroups[73], &HeroModelBank[DB_CharsTankRilla], &DatIndex[880], &DatIndex[879] },
    { &FileGroups[77], &HeroModelBank[DB_CharsTankTiny], &DatIndex[888], &DatIndex[887] },
    { &FileGroups[74], &HeroModelBank[DB_CharsTankKong], &DatIndex[882], &DatIndex[881] },
    { &FileGroups[72], &HeroModelBank[DB_CharsKegsCrash], &DatIndex[878], &DatIndex[877] },
    { &FileGroups[71], &HeroModelBank[DB_CharsKegsCoco], &DatIndex[876], &DatIndex[875] },
    { &FileGroups[70], &HeroModelBank[DB_CharsKegsCortex], &DatIndex[874], &DatIndex[873] },
    { &FileGroups[68], &HeroModelBank[DB_CharsKegsBrio], &DatIndex[870], &DatIndex[869] },
    { &FileGroups[67], &HeroModelBank[DB_CharsKegsDingo], &DatIndex[868], &DatIndex[867] },
    { &FileGroups[65], &HeroModelBank[DB_CharsKegsRilla], &DatIndex[864], &DatIndex[863] },
    { &FileGroups[69], &HeroModelBank[DB_CharsKegsTiny], &DatIndex[872], &DatIndex[871] },
    { &FileGroups[66], &HeroModelBank[DB_CharsKegsKong], &DatIndex[866], &DatIndex[865] },
    { &FileGroups[64], &HeroModelBank[DB_CharsMalletCrash], &DatIndex[862], &DatIndex[861] },
    { &FileGroups[63], &HeroModelBank[DB_CharsMalletCoco], &DatIndex[860], &DatIndex[859] },
    { &FileGroups[62], &HeroModelBank[DB_CharsMalletCortex], &DatIndex[858], &DatIndex[857] },
    { &FileGroups[61], &HeroModelBank[DB_CharsMalletBrio], &DatIndex[856], &DatIndex[855] },
    { &FileGroups[60], &HeroModelBank[DB_CharsMalletDingo], &DatIndex[854], &DatIndex[853] },
    { &FileGroups[59], &HeroModelBank[DB_CharsMalletRilla], &DatIndex[852], &DatIndex[851] },
    { &FileGroups[58], &HeroModelBank[DB_CharsMalletTiny], &DatIndex[850], &DatIndex[849] },
    { &FileGroups[57], &HeroModelBank[DB_CharsMalletKong], &DatIndex[848], &DatIndex[847] },
    { &FileGroups[56], &HeroModelBank[DB_CharsDinoCrash], &DatIndex[846], &DatIndex[845] },
    { &FileGroups[52], &HeroModelBank[DB_CharsDinoCoco], &DatIndex[838], &DatIndex[837] },
    { &FileGroups[55], &HeroModelBank[DB_CharsDinoCortex], &DatIndex[844], &DatIndex[843] },
    { &FileGroups[51], &HeroModelBank[DB_CharsDinoBrio], &DatIndex[836], &DatIndex[835] },
    { &FileGroups[53], &HeroModelBank[DB_CharsDinoDingo], &DatIndex[840], &DatIndex[839] },
    { &FileGroups[50], &HeroModelBank[DB_CharsDinoRilla], &DatIndex[834], &DatIndex[833] },
    { &FileGroups[54], &HeroModelBank[DB_CharsDinoTiny], &DatIndex[842], &DatIndex[841] },
    { &FileGroups[49], &HeroModelBank[DB_CharsDinoKong], &DatIndex[832], &DatIndex[831] },
    { &FileGroups[48], &HeroModelBank[DB_CharsRaceCrash], &DatIndex[830], &DatIndex[829] },
    { &FileGroups[42], &HeroModelBank[DB_CharsRaceCoco], &DatIndex[818], &DatIndex[817] },
    { &FileGroups[47], &HeroModelBank[DB_CharsRaceCortex], &DatIndex[828], &DatIndex[827] },
    { &FileGroups[44], &HeroModelBank[DB_CharsRaceBrio], &DatIndex[822], &DatIndex[821] },
    { &FileGroups[45], &HeroModelBank[DB_CharsRaceDingo], &DatIndex[824], &DatIndex[823] },
    { &FileGroups[41], &HeroModelBank[DB_CharsRaceRilla], &DatIndex[816], &DatIndex[815] },
    { &FileGroups[46], &HeroModelBank[DB_CharsRaceTiny], &DatIndex[826], &DatIndex[825] },
    { &FileGroups[43], &HeroModelBank[DB_CharsRaceKong], &DatIndex[820], &DatIndex[819] },
    { &FileGroups[40], &HeroModelBank[DB_CharsSplashCrash], &DatIndex[814], &DatIndex[813] },
    { &FileGroups[35], &HeroModelBank[DB_CharsSplashCoco], &DatIndex[804], &DatIndex[803] },
    { &FileGroups[39], &HeroModelBank[DB_CharsSplashCortex], &DatIndex[812], &DatIndex[811] },
    { &FileGroups[33], &HeroModelBank[DB_CharsSplashBrio], &DatIndex[800], &DatIndex[799] },
    { &FileGroups[37], &HeroModelBank[DB_CharsSplashDingo], &DatIndex[808], &DatIndex[807] },
    { &FileGroups[34], &HeroModelBank[DB_CharsSplashRilla], &DatIndex[802], &DatIndex[801] },
    { &FileGroups[38], &HeroModelBank[DB_CharsSplashTiny], &DatIndex[810], &DatIndex[809] },
    { &FileGroups[36], &HeroModelBank[DB_CharsSplashKong], &DatIndex[806], &DatIndex[805] },
};
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
MarkCtx ExitAMarks[13] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingGoodShot3], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot8], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot7], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot6], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingGoodShot2], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot5], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot4], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot3], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot2], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingEvilShot1], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingGoodShot1], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntro], 0, 1 },
    { 0 },
};
EngineVabCue ExitAAudio[5] = {
    { &DatIndex[262], 0x780, 0x4AD },
    { &DatIndex[261], 0x781, 0x4AD },
    { &DatIndex[260], 0x782, 0x4AD },
    { &DatIndex[259], 0x783, 0x4AD },
    { 0 },
};
MarkCtx SceneTextGoodwinsMarks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneTextGoodwins], 0, 1 },
    { 0 },
};
EngineVabCue SceneTextGoodwinsAudio[2] = {
    { &DatIndex[271], 0x76C, 0x4AD },
    { 0 },
};
MarkCtx ExitBMarks[10] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingEvilShot8], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot11], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot10], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingEvilShot7], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingEvilShot6], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot9], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingEvilShot5], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingEvilShot3], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelEndingEvilShot2], 0, 1 },
    { 0 },
};
EngineVabCue ExitBAudio[4] = {
    { &DatIndex[258], 0x78A, 0x4AD },
    { &DatIndex[257], 0x78B, 0x4AD },
    { &DatIndex[256], 0x78C, 0x4AD },
    { 0 },
};
MarkCtx SceneTextEvilwinsMarks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneTextEvilwins], 0, 0 },
    { 0 },
};
MarkCtx SceneTextMixteamwinsMarks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneTextMixteamwins], 0, 1 },
    { 0 },
};
MarkCtx ExitCMarks[5] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevel4], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevel3], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevel2], 0, 1 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevel], 0, 1 },
    { 0 },
};
EngineVabCue ExitCAudio[4] = {
    { &DatIndex[265], 0x776, 0x4AD },
    { &DatIndex[264], 0x777, 0x4AD },
    { &DatIndex[263], 0x778, 0x4AD },
    { 0 },
};
MarkCtx SceneTextCreditsMarks[4] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneTextCredits], 0, 1 },
    { 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneTextCredits], 0, 1 },
    { 0 },
};
MarkCtx SceneTextIntromovieMarks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneTextIntromovie], 0, 1 },
    { 0 },
};
MarkCtx IntroMovieMarks[20] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneIntroLogo], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot19], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot18], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot17], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot16], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot15], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot14], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroCrashplain], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroCortexlab], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroShotWelcome3], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroShotWelcome2], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroShotWelcome], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamevil4], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamevil3], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot13], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamevil2], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelShot12], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamevil], 0, 0 },
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneLevelIntroShotGroupTeamgood], 0, 0 },
    { 0 },
};
EngineVabCue SceneSharedAudio[12] = {
    { &DatIndex[255], 0x794, 0x255 },
    { &DatIndex[254], 0x795, 0x255 },
    { &DatIndex[253], 0x796, 0x255 },
    { &DatIndex[252], 0x797, 0x255 },
    { &DatIndex[251], 0x798, 0x255 },
    { &DatIndex[250], 0x799, 0x255 },
    { &DatIndex[249], 0x79A, 0x255 },
    { &DatIndex[248], 0x79B, 0x255 },
    { &DatIndex[247], 0x79C, 0x255 },
    { &DatIndex[246], 0x79D, 0x255 },
    { &DatIndex[245], 0x79E, 0x255 },
    { 0 },
};
ScriptScene IntroMovieMsgs[2] = {
    { IntroMovieMarks, SceneSharedAudio, 0x1DFC40 },
    { SceneTextIntromovieMarks, SceneSharedAudio, 0x1E0000 },
};
ScriptScene LevelExitMsgsA[2] = {
    { ExitAMarks, ExitAAudio, 0x1E0000 },
    { SceneTextGoodwinsMarks, SceneSharedAudio, 0x1E0000 },
};
ScriptScene LevelExitMsgsB[2] = {
    { ExitBMarks, ExitBAudio, 0x1E0000 },
    { ExitBMarks, SceneSharedAudio, 0x1E0000 },
};
ScriptScene LevelExitMsgsC[4] = {
    { ExitCMarks, ExitCAudio, 0x1E0000 },
    { SceneTextMixteamwinsMarks, SceneSharedAudio, 0x1E0000 },
    { SceneTextCreditsMarks, SceneSharedAudio, 0x1E0000 },
    { SceneTextCreditsMarks, SceneSharedAudio, 0x1E0000 },
};
EngineVabCue SceneUkaData4Audio[2] = {
    { &DatIndex[224], 0x3DD, 0 },
    { 0 },
};
EngineVabCue SceneUkaData3Audio[2] = {
    { &DatIndex[225], 0x3DA, 0 },
    { 0 },
};
EngineVabCue SceneUkaData2Audio[2] = {
    { &DatIndex[222], 0x3E3, 0 },
    { 0 },
};
EngineVabCue SceneUkaDataAudio[2] = {
    { &DatIndex[223], 0x3E0, 0 },
    { 0 },
};
MarkCtx SceneUkaData4Marks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneUkaData4], 0, 0 },
    { 0 },
};
ScriptScene SceneUkaData4 = { SceneUkaData4Marks, SceneUkaData4Audio, 0x1E0000 };
MarkCtx SceneUkaData3Marks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneUkaData3], 0, 0 },
    { 0 },
};
ScriptScene SceneUkaData3 = { SceneUkaData3Marks, SceneUkaData3Audio, 0x1E0000 };
MarkCtx SceneUkaDataMarks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneUkaData], 0, 0 },
    { 0 },
};
ScriptScene SceneUkaData = { SceneUkaDataMarks, SceneUkaDataAudio, 0x1E0000 };
MarkCtx SceneUkaData2Marks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneUkaData2], 0, 0 },
    { 0 },
};
ScriptScene SceneUkaData2 = { SceneUkaData2Marks, SceneUkaData2Audio, 0x1E0000 };
EngineVabCue SceneAkuData4Audio[2] = {
    { &DatIndex[220], 0x3DC, 0 },
    { 0 },
};
EngineVabCue SceneAkuData3Audio[2] = {
    { &DatIndex[221], 0x3D9, 0 },
    { 0 },
};
EngineVabCue SceneAkuData2Audio[2] = {
    { &DatIndex[218], 0x3E2, 0 },
    { 0 },
};
EngineVabCue SceneAkuDataAudio[2] = {
    { &DatIndex[219], 0x3DF, 0 },
    { 0 },
};
MarkCtx SceneAkuData4Marks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneAkuData4], 0, 0 },
    { 0 },
};
ScriptScene SceneAkuData4 = { SceneAkuData4Marks, SceneAkuData4Audio, 0x1E0000 };
MarkCtx SceneAkuData3Marks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneAkuData3], 0, 0 },
    { 0 },
};
ScriptScene SceneAkuData3 = { SceneAkuData3Marks, SceneAkuData3Audio, 0x1E0000 };
MarkCtx SceneAkuDataMarks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneAkuData], 0, 0 },
    { 0 },
};
ScriptScene SceneAkuData = { SceneAkuDataMarks, SceneAkuDataAudio, 0x1E0000 };
MarkCtx SceneAkuData2Marks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneAkuData2], 0, 0 },
    { 0 },
};
ScriptScene SceneAkuData2 = { SceneAkuData2Marks, SceneAkuData2Audio, 0x1E0000 };
EngineVabCue SceneAkuUkaData2Audio[2] = {
    { &DatIndex[214], 0x3E4, 0 },
    { 0 },
};
EngineVabCue SceneAkuUkaDataAudio[2] = {
    { &DatIndex[215], 0x3E1, 0 },
    { 0 },
};
MarkCtx SceneAkuUkaDataMarks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneAkuUkaData], 0, 0 },
    { 0 },
};
ScriptScene SceneAkuUkaData = { SceneAkuUkaDataMarks, SceneAkuUkaDataAudio, 0x1E0000 };
MarkCtx SceneAkuUkaData2Marks[2] = {
    { 1, (MarkObj *)&HeroModelBank[DB_CutsceneAkuUkaData2], 0, 0 },
    { 0 },
};
ScriptScene SceneAkuUkaData2 = { SceneAkuUkaData2Marks, SceneAkuUkaData2Audio, 0x1E0000 };
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
obj_dbase *SceneOldDbase = 0;
script_handler *SceneScript = 0;
s32 SceneStarting = 0;
s32 SceneStep = 0;
s32 SceneUnused[1] = {0};
sub_string_info TextTemplates[11] = {
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
