#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/gameflow.h"
#include "bash/hero.h"
#include "bash/shell.h"

void PlayHitEffect(s32 *arg0, HitInfo *arg1, s32 arg2) {
    s32 id;
    s32 obj;

    obj = ((s32 *)arg1)[-1];
    id = arg2 != 0 ? arg1->unk16 : -1;
    switch (arg1->unk14) {
    case 5:
    case 7:
        PlaySfx(HitSfx.hit[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(1, arg0, (s32)&HeroModelBank[DB_CrateObjects]);
        HurtHeroesInRadius(obj, 0x280, id, 0x208, 0xA0);
        break;
    case 6:
        PlaySfx(HitSfx.hit6[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(0, arg0, (s32)&HeroModelBank[DB_CrateObjects]);
        HurtHeroesInRadius(obj, 0x280, id, 0x208, 0);
        break;
    case 2:
        PlaySfx(HitSfx.crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(2, arg0, (s32)&HeroModelBank[DB_CrateObjects]);
        break;
    case 3:
        PlaySfx(HitSfx.crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(4, arg0, (s32)&HeroModelBank[DB_CrateObjects]);
        HurtHeroesInRadius(obj, 0x280, id, 0x203, 0xA0);
        break;
    default:
        PlaySfx(HitSfx.other[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(3, arg0, (s32)&HeroModelBank[DB_CrateObjects]);
        HurtHeroesInRadius(obj, 0x280, id, 0x205, 0xA0);
        break;
    }
    if (HitHooks.unk8 != NULL) {
        HitHooks.unk8(arg0[0], arg0[1], arg0[2], 0, id, arg1->unk14);
    }
}

void InitCrateExplode(NAS *arg0) {
    EXTengine *e = arg0->Engine;

    if (e != NULL) {
        PlaySfx(HitSfx.crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(2, (s32 *)&e->unk10, &HeroModelBank[DB_CrateObjects]);
    }
    KillNAS(arg0);
}

void UpdateIntro(void) {
    IntroStep *e;
    IntroStep *tbl;
    s32 type;
    u8 *a;
    u8 *b;

    GameFrameCount++;
    if ((Pads[0].held & 0x8C08) == 0x8C08) {
        shell_run_spyro_demo();
    }
    scripts_update();
    if (IntroSfxDelay != 0) {
        if (--IntroSfxDelay == 0) {
            PlaySfx(IntroNextSfx, &IntroSfxHandle, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if (IntroScriptHandle != 0) {
        return;
    }
    if (IntroDbase != 0) {
        dbase_release((u8 *)IntroDbase);
        IntroDbase = 0;
    }
    if (IntroStepIndex == 1 && IntroStepTimer == IntroSteps[0].delay) {
        modchip_step(0);
    }
    if (IntroStepTimer < 0) {
        return;
    }
    if (--IntroStepTimer > 0) {
        return;
    }
    if (loader_pending() != 0) {
        IntroStepTimer = 1;
        return;
    }
    if ((u32)IntroStepIndex >= 4) {
        ProcessControl.flags = 0x12;
        ProcessControl.next_process = &LevelLoadProcess;
        IntroStepTimer = -1;
        return;
    }
    tbl = IntroSteps;
    e = &tbl[IntroStepIndex];
    type = e->type;
    switch (type) {
    case 1:
        IntroStepTimer = e->delay;
        a = (u8 *)load_file_permanent(e->pic);
        b = tga_convert(a, ScreenRect->w, ScreenRect->h);
        mal_free(a, &MainHeap);
        LoadImage(ScreenRect, (u32 *)b);
        LoadImage(ScreenRect + 1, (u32 *)b);
        DrawSync(0);
        mal_free(b, &MainHeap);
        if (IntroSteps[IntroStepIndex].sfx[0] != -1) {
            PlaySfx(IntroSteps[IntroStepIndex].sfx[0], &IntroSfx[0], NULL, 0x1000, 0x1E00, 0);
            PlaySfx(IntroSteps[IntroStepIndex].sfx[1], &IntroSfx[1], NULL, 0x1000, 0x1E00, 0);
            PlaySfx(IntroSteps[IntroStepIndex].sfx[2], &IntroSfx[2], NULL, 0x1000, 0x1E00, 0);
            IntroSfxDelay = 6;
            IntroNextSfx = IntroSteps[IntroStepIndex].sfxNext;
        }
        break;
    case 2:
        IntroStepTimer = e->delay;
        SetupSceneMarks(e->model);
        ScriptTimeStep = 0x10000;
        VSyncRate = type;
        IntroScript = play_script((obj_dbase *)IntroSteps[IntroStepIndex].model, IntroSteps[IntroStepIndex].unkC,
                                   &IntroScriptRot, &IntroScriptPos, 4, (script_handler **)&IntroScriptHandle);
        if (IntroSteps[IntroStepIndex].sfx[0] != -1) {
            PlaySfx(IntroSteps[IntroStepIndex].sfx[0], &IntroSfx[0], NULL, 0x1000, 0x1E00, 0);
            PlaySfx(IntroSteps[IntroStepIndex].sfx[1], &IntroSfx[1], NULL, 0x1000, 0x1E00, 0);
            PlaySfx(IntroSteps[IntroStepIndex].sfx[2], &IntroSfx[2], NULL, 0x1000, 0x1E00, 0);
            IntroSfxDelay = 0x16;
            IntroNextSfx = IntroSteps[IntroStepIndex].sfxNext;
        }
        if (IntroSteps[IntroStepIndex].unk10 != -1) {
            music_request(IntroSteps[IntroStepIndex].unk10);
        }
        if (IntroDbase != 0) {
            dbase_release((u8 *)IntroDbase);
        }
        IntroDbase = (s32)IntroSteps[IntroStepIndex].model;
        break;
    }
    IntroStepIndex++;
    if (IntroSteps[IntroStepIndex].type == 2) {
        dbase_asload_with_anims(IntroSteps[IntroStepIndex].model, 0);
    }
}

void QuitIntro(void) {
    s32 f;
    RECT *a;
    RECT *r;

    if (IntroDbase != 0) {
        dbase_release((u8 *)IntroDbase);
    }
    scripts_release();
    subpool_free();
    f = FrameCount;
    a = &ScreenRect[f & 1];
    r = &ScreenRect[(f + 1) & 1];
    MoveImage2(a, r->x, r->y);
}

void RenderIntro(void) {
    gfx_set_viewport(IntroViewport, &MainCamera);
    if (IntroDbase != 0) {
        DrawSceneMarks();
    }
    scripts_render();
}

void InitIntro(void) {
    IntroStepIndex = 0;
    IntroStepTimer = 0;
    IntroScriptHandle = 0;
    IntroDbase = 0;
    init_scripts(0x20, 0x80);
    IntroNextSfx = -1;
    IntroSfxDelay = 0;
    subpool_init(0x200);
    cam_init(&MainCamera);
    FirstBootDone = 1;
}

void LoadScreenImage(void) {
    u8 *a;
    u8 *b;
    s32 load_file_permanent();

    a = (u8 *)load_file_permanent();
    b = tga_convert(a, ScreenRect->w, ScreenRect->h);
    mal_free(a, &MainHeap);
    LoadImage(ScreenRect, (u32 *)b);
    LoadImage(ScreenRect + 1, (u32 *)b);
    DrawSync(0);
    mal_free(b, &MainHeap);
}

void CopyScreen(void) {
    s32 f = FrameCount;
    RECT *a = &ScreenRect[f & 1];
    RECT *r = &ScreenRect[(f + 1) & 1];

    MoveImage2(a, r->x, r->y);
}

void HandleLevelExit(void) {
    GameState *g = (GameState *)GameInfo;
    ScriptScene *msg;
    ScriptScene *res;
    u16 flags;
    u16 w;

    if (!(g->exitFlags & 0x8000)) {
        return;
    }
    g->heroes[0] = g->heroChoice[0];
    g->heroes[1] = g->heroChoice[1];
    g->heroes[2] = g->heroChoice[2];
    g->heroes[3] = g->heroChoice[3];
    g->teammates[0] = g->teammateChoice[0];
    g->teammates[1] = g->teammateChoice[1];
    g->teammates[2] = g->teammateChoice[2];
    g->teammates[3] = g->teammateChoice[3];
    if (g->levelFlags & 0x20) {
        PickCpuHeroes();
    }
    flags = g->exitFlags;
    if ((flags & 0x8008) == 0x8008) {
        g->mode = NextGameMode;
        res = PickModeVariant((s16)NextGameMode);
        if (res == 0) {
            ProcessControl.next_process = &LevelLoadProcess;
            return;
        }
        GotoScriptScene(res, &LevelLoadProcess);
        return;
    }
    w = ModeTypes[g->mode * 3];
    if (w == 0x101) {
        return;
    }
    if (w == 0x102) {
        if (g->mode == 0x2B) {
            g->mode = 0x28;
            ProcessControl.next_process = &LevelLoadProcess;
            return;
        }
        if (flags == 0x8002) {
            g->mode = WarpRoomMode();
        } else {
            g->mode = NextGameMode;
        }
        ProcessControl.next_process = &LevelLoadProcess;
        return;
    }
    if (flags & 0x200) {
        ProcessControl.next_process = &RetryProcess;
        return;
    }
    switch (g->mode) {
    case 0x1F:
        if (g->adventure.unkE4[4][31] != 0 && (flags & 1)) {
            if (g->teamSide[0] == 3) {
                g->mode = 0x20;
                GotoScriptScene(LevelExitMsgsC, &LevelLoadProcess);
            } else if (g->teamSide[0] == 1) {
                g->mode = 0x25;
                GotoScriptScene(LevelExitMsgsA, &LevelLoadProcess);
            } else {
                g->mode = 0x25;
                GotoScriptScene(LevelExitMsgsB, &LevelLoadProcess);
            }
            return;
        }
        GameMode = WarpRoomMode();
        return;
    case 0x20:
        g->mode = 0x25;
        if (HeroSide[g->winner[0]] == 1) {
            msg = LevelExitMsgsA;
        } else {
            msg = LevelExitMsgsB;
        }
        GotoScriptScene(msg, &LevelLoadProcess);
        return;
    }
    g = (GameState *)GameInfo;
    switch (*(u8 *)&g->levelFlags) {
    case 2:
        if (g->exitFlags & 1) {
            g->mode = CupNextArena();
        } else {
            g->mode = 0x2A;
        }
        break;
    case 1:
        g->mode = 0x29;
        break;
    default:
        g->mode = WarpRoomMode();
        break;
    }
    ProcessControl.next_process = &LevelLoadProcess;
}

s32 WarpRoomMode(void) {
    return WarpRooms[GameWarpRoom].levnum;
}

void ReturnToWarpRoom(void) {
    NextGameMode = WarpRoomMode();
    GameExitFlags = 0x8008;
    HandleLevelExit();
}

void UpdateExitDelay(void) {
    GameState *g = GAME;

    if (g->exitFlags & 0x8000) {
        if (ProcessControl.next_process == NULL) {
            if (g->exitDelay != 0) {
                g->exitDelay -= 1;
                if (g->exitDelay != 0) {
                    return;
                }
            }
            GotoProcess(&LevelLoadProcess, 0x200);
            if (!(g->exitFlags & 0x200)) {
                music_request(-1);
            }
        }
    }
}

void ExitToTitles(void) {
    NextGameMode = 0x28;
    GameExitFlags = 0x8008;
}

void AnimateCupMenu(void) {
    CupRound *p;
    CupDef *base;
    CupDef *t;
    subengine *sub;
    TrigEntry *trig;
    s32 n;
    s32 i;
    u32 mask;

    i = 0;
    trig = rcossin_tbl;
    base = CupTable;
    t = base + GameCup;
    n = t->roundCount;
    p = t->rounds;
loop:
    if (i < n) {
        sub = GetEngineSlot(i + 3)->sub;
        sub->u6C.dbase = (void *)p->unk8;
        sub->entity = p->unkC;
        if (p->unk0 == 1 || (p->unk0 == 2 && LevelUnlockedIn(p->mode, GameInfo) != 0)) {
            sub->flags |= 0x8000;
        } else {
            sub->flags &= ~0x8000;
        }
        p++;
        sub->unk10 = 0x7D;
        sub->unk12 = 0;
        sub->unk14 = 0;
        sub->unk04 = trig[(((GameFrameCount * 20 + (i << 10)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 128 >> 12;
        sub->unk08 = (trig[(((GameFrameCount * 70 + (i << 9)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
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

s32 CountSavedWins(s32 arg0) {
    s32 cnt;
    s32 i;
    u8 *tab;

    tab = (u8 *)&CardSlots[0].state;
    cnt = 0;
    i = 0;
    for (;;) {
    loop:
        if (i >= 7) {
            break;
        }
        if (CupTable[i].unk0 == 3 && !EngineTable_AllDone(&CupTable[i]) &&
            EngineTable_Reachable(&CupTable[i], tab)) {
            cnt++;
            if (arg0 != 0) {
                EngineTable_MarkDone(&CupTable[i]);
            }
        }
        i++;
    }
    return cnt;
}

void UpdateCardMessage(void) {
    subengine *s0;
    subengine *s1;
    s32 *tab;

    s0 = GetEngineSlot(0)->sub;
    s1 = &s0[6];
    if (CardStatus & 2) {
        if (CardStatus & 0x40) {
            s1->u6C.text = (char *)LangCheckingUnlocked[GameLanguage];
            s0[6].flags |= 0x8000;
            return;
        } else if (CardStatus & 0x10) {
            if (CountSavedWins(0) != 0) {
                s1->u6C.text = (char *)LangUnlockWithCard[GameLanguage];
                s0[6].flags |= 0x8000;
                if (Pads[0].pressed & 0x2000) {
                    PlaySfx(0x396, NULL, NULL, 0x1000, 0x1E00, 0);
                    CountSavedWins(1);
                }
                return;
            }
            s1->u6C.text = (char *)LangThereAreNoMoreUnlockedLevels[GameLanguage];
            s0[6].flags |= 0x8000;
            return;
        } else {
            goto def;
        }
    } else {
def:
        tab = LangInsertCardForUnlocked;
    }
    s1->u6C.text = (char *)tab[GameLanguage];
    s0[6].flags |= 0x8000;
}

void UpdateCupMenu(void) {
    u8 *g;
    u8 *p;
    subengine *sub;
    s32 i;

    AnimateTitlesBackdrop();
    AnimateCupMenu();
    if (CupMenuCompact != 0) {
        CupMenuCompact--;
        prims_empty_back();
        mal_compact_step(&MainHeap);
    }
    if (Pads[0].pressed & 0x80) {
        EngineWorldPrev();
    }
    if (Pads[0].pressed & 0x20) {
        EngineWorldNext();
    }
    if (Pads[0].pressed & 0x1000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
    if (EngineTable_Unlocked(GAME->cup) && (Pads[0].pressed & 0x4000)) {
        g = GameInfo;
        ((GameState *)g)->levelFlags = (((GameState *)g)->levelFlags & 0xFF00) | 2;
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        g[0x29] = 0;
        NextGameMode = CupTable[g[0x28]].rounds->mode;
        MenuChainNext();
        p = GameInfo;
        for (i = 0; i < 4; i++) {
            p[i + 0x9F] = 0;
            p[i + 0xA3] = 0;
        }
    }
    GetEngineSlot(0)->sub->u6C.dbase = (void *)EngineCurWorldName();
    if (GAME->cup != 0) {
        GetEngineSlot(0)->sub[8].flags |= 0x8000;
    } else {
        GetEngineSlot(0)->sub[8].flags &= ~0x8000;
    }
    if (GameCup < 6) {
        GetEngineSlot(0)->sub[9].flags |= 0x8000;
    } else {
        GetEngineSlot(0)->sub[9].flags &= ~0x8000;
    }
    if (EngineTable_Unlocked(GAME->cup)) {
        GetEngineSlot(0)->sub[7].flags |= 0x8000;
        GetEngineSlot(0)->sub[4].flags &= ~0x8000;
        GetEngineSlot(0)->sub[3].flags &= ~0x8000;
        GetEngineSlot(0)->sub[5].flags &= ~0x8000;
        GetEngineSlot(0)->sub[6].flags &= ~0x8000;
        return;
    }
    GetEngineSlot(0)->sub[7].flags &= ~0x8000;
    GetEngineSlot(0)->sub[3].flags |= 0x8000;
    sub = &GetEngineSlot(0)->sub[4];
    sub->flags |= 0x02008000;
    sub->unk68 = -0x80;
    sub->u6C.dbase = &HeroModelBank[DB_MainmenuModels];
    sub->entity = 0x2006;
    sub->unk12 = FrameCount * 50;
    sub->unk10 = 0;
    sub->unk14 = rcossin_tbl[(((GameFrameCount * 100) & 0xFFF) - 0x400) & 0xFFF].unk2 * 25 >> 9;
    sub->unk52 = 1;
    GetEngineSlot(0)->sub[5].flags |= 0x8000;
    UpdateCardMessage();
}

void RenderCupMenu(void) {
    gfx_set_viewport(&CupViewport0, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(3));
    gfx_set_viewport(&CupViewport1, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(4));
    gfx_set_viewport(&CupViewport2, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(5));
    gfx_set_viewport(&CupViewport3, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(6));
    RenderTitlesBackdropAndMenu();
    gfx_set_viewport(&CupMenuViewport, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(2));
}

void QuitCupMenu(void) {
}

s32 CountCupTrophies(CupDef *cup) {
    s32 i;
    s32 j;
    s32 cnt;
    s32 obj;
    u8 *p;
    u8 *tab;

    i = 0;
    tab = (u8 *)&CardSlots[0].state;
    for (;;) {
        if (i >= cup->roundCount) {
            goto done;
        }
        obj = cup->rounds[i].mode;
        if (LevelUnlockedIn(obj, GameInfo) == 0) {
            cnt = 0;
            j = cnt;
            p = tab;
            do {
                if (LevelUnlockedIn(obj, p) != 0) {
                    cnt++;
                }
                j++;
                p += 0x194;
            } while (j < 4);
            if (cnt == 0) {
                return 0;
            }
        }
        i++;
    }
done:
    return 1;
}

void AwardCupTrophies(CupDef *cup) {
    s32 i;
    s32 n = cup->roundCount;

    for (i = 0; i < n; i++) {
        CupRound *e = cup->rounds;
        GAME->adventure.unkE4[0][e[i].mode] = 1;
        n = cup->roundCount;
    }
}

void CupMenuNext(void) {
    GameState *g = GAME;
    s32 n;

    CupMenuCompact = 4;
    n = g->cup;
    if (n < 6) {
        n++;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g->cup = n;
    }
}

void CupMenuPrev(void) {
    GameState *g = GAME;
    s32 n;

    CupMenuCompact = 4;
    n = g->cup;
    if (n > 0) {
        n--;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g->cup = n;
    }
}

s32 CupUnlocked(s32 arg0) {
    CupDef *base = CupTable;
    CupDef *t;
    CupRound *p;
    s32 n;
    s32 i;

    t = base + arg0;
    if (t->unk0 == 1) {
        return 1;
    }
    if (t->unk0 == 2) {
        return 0;
    }
    n = t->roundCount;
    p = t->rounds;
    for (i = 0; i < n; i++, p++) {
        if (!LevelUnlockedIn(p->mode, GameInfo)) {
            return 0;
        }
    }
    return 1;
}

s32 CupCurrentModeType(void) {
    u16 *wt = ModeTypes;
    CupDef *tab = CupTable;
    GameState *g = GAME;
    CupRound *e = tab[g->cup].rounds;
    s32 idx = e[g->cupRound].mode;
    s32 **tbl;
    s32 w = wt[idx * 3] - 0x101;
    s32 *t;
    tbl = (s32 **)LangModeTypeTexts;
    t = tbl[g->language];
    return t[w];
}

s32 CupNextArena(void) {
    if (GAME->cupRound < CupTable[GAME->cup].roundCount - 1) {
        GAME->cupRound = GAME->cupRound + 1;
        return CupTable[GAME->cup].rounds[GAME->cupRound].mode;
    }
    return 0x2B;
}

s32 CupCurrentArena(void) {
    return CupTable[GAME->cup].rounds[GAME->cupRound].mode;
}

void InitCupMenu(void) {
    CupMenuCompact = 0;
    PlaceEngineSlot(0, (s32 *)CupMenuLabels);
    PlaceEngineSlot(2, CupMenuPanel);
    PlaceEngineSlot(3, (s32 *)CupSlotPanel);
    PlaceEngineSlot(4, (s32 *)CupSlotPanel);
    PlaceEngineSlot(5, (s32 *)CupSlotPanel);
    PlaceEngineSlot(6, (s32 *)CupSlotPanel);
    GameCup = 0;
}

/* WarpRoomIndex, inlined into its callers */
static inline s32 WarpRoomFind(s32 arg0) {
    s32 i;
    hub_info *p;

    for (i = 0, p = WarpRooms; (u32)i < 10; i++, p++) {
        if (p->levnum == arg0) {
            return i;
        }
    }
    return -1;
}

s32 WarpRoomBestLevel(void) {
    s32 i;
    s32 j;
    s32 best;
    hub_info *t;

    best = -1;
    i = WarpRoomFind(GameMode);
    if (i < 0) {
        t = 0;
    } else {
        t = &WarpRooms[i];
    }
    if (t == NULL) {
        return best;
    }
    for (j = 0; j < t->num_warps; j++) {
        if (ModeRequirementMet(t->warp[j].levnum) != 0 && best < j) {
            best = j;
        }
    }
    return best;
}

hub_info *WarpRoomOfMode(s32 arg0) {
    s32 i = WarpRoomFind(arg0);

    if (i < 0) {
        return NULL;
    }
    return &WarpRooms[i];
}

s32 WarpRoomIndex(s32 arg0) {
    s32 i;
    hub_info *p;

    for (i = 0, p = WarpRooms; (u32)i < 10; i++, p++) {
        if (p->levnum == arg0) {
            return i;
        }
    }
    return -1;
}

s32 GatewayMode(void) {
    HubTableEntry *p;

    if (GameGateway < 0 || (p = HubGateways[GameGateway].table, p->gate_type != 2)) {
        return -1;
    }
    return p->level;
}

void ResetWarpRooms(void) {
    s32 i;
    HubTableEntry *e;
    hub_info *t = WarpRooms;

    for (i = 9; i != -1; i--) {
        e = t->table;
        while (e->gateway != 0) {
            e->status &= ~8;
            e++;
        }
        t++;
    }
    HubHeroRing.unk0 = 0;
    HubHeroRing.angle = 0;
    HubHeroRing.radius = 0x400;
}

void ShowCardSlot(GameState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    EXTengine *e;
    s32 n;
    s32 d;
    s32 r;

    PlaceEngineSlot(arg1 + 4, CardSlotPanel);
    e = GetEngineSlot(arg1 + 4);
    e->unk10.x = arg3;
    e->unk10.y = arg4;
    e->flags |= 0x10000000;
    if (arg2 != 0) {
        e->sub[3].entity = 0;
        e->sub[4].entity = 0;
        e->sub[5].entity = 0;
        e->sub[6].entity = 0;
        e->sub[2].u6C.dbase = NULL;
        e->sub[1].u6C.text = LangEmpty[GameLanguage];
        e->sub[1].unk04 = 0x8096;
        e->sub[1].unk08 = 0x1E;
        return;
    }
    e->sub[3].entity = IconImage[arg0->heroChoice[0]];
    e->sub[4].entity = IconImage[arg0->heroChoice[1]];
    e->sub[5].entity = IconImage[arg0->heroChoice[2]];
    e->sub[6].entity = IconImage[arg0->heroChoice[3]];
    e->sub[1].u6C.text = arg0->name;
    e->sub[1].unk04 = 0xC;
    e->sub[1].unk08 = 2;
    n = count_completion((u8 *)arg0);
    PercentText[arg1][0] = n / 100 + '0';
    n %= 100;
    d = n / 10;
    r = n - d * 10;
    d += '0';
    r += '0';
    PercentText[arg1][1] = d;
    PercentText[arg1][2] = r;
    PercentText[arg1][3] = '%';
    PercentText[arg1][4] = 0;
    e->sub[2].u6C.text = PercentText[arg1];
}

void ShowCardSlotStats(GameState *arg0, s32 arg1) {
    EXTengine *e;

    e = GetEngineSlot(3);
    e->unk10.x = 0x28;
    e->unk10.y = 0xDA;
    e->flags |= 0x10000000;
    if (arg1 != 0) {
        EngineSetSlot3Digits(3, 3);
        return;
    }
    itoa_div(CardStatsText, CountTrophies(arg0), 10);
    e->sub[2].u6C.text = CardStatsText;
    itoa_div(CardStatsText + 3, EngineSum147(arg0), 10);
    e->sub[6].u6C.text = CardStatsText + 3;
    itoa_div(CardStatsText + 6, EngineSum126(arg0), 10);
    e->sub[4].u6C.text = CardStatsText + 6;
    itoa_div(CardStatsText + 9, EngineCount105(arg0), 10);
    e->sub[8].u6C.text = CardStatsText + 9;
    itoa_div_end(CardStatsText + 12, EngineExtra105(arg0), 10);
    e->sub[10].u6C.text = CardStatsText + 12;
    switch (arg0->adventure.unkE0) {
    case 1:
        EngineSetSlot3Digits(3, 1);
        break;
    case 2:
        EngineSetSlot3Digits(3, 2);
        break;
    default:
        EngineSetSlot3Digits(3, 0);
        break;
    }
}

void UpdateCardPrompt(void) {
    PadState *pad;
    s32 idx;
    EXTengine *e;
    GameState *g;
    u8 *src;
    u8 *p;

    RenderState.modelDp = ScreenFade.amount;
    switch (CardPromptStep) {
    case 0:
        pad = Pads;
        if (pad->pressed & 0x1000) {
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
            CardPromptRetry = 1;
            GotoProcess(&RetryProcess, 0x200);
            {
                GameState *q = GAME;
                q->exitFlags |= 0x200;
            }
        }
        if (CardStatus & 2) {
            if (CardStatus & 0x40) {
                e = GetEngineSlot(1);
                e->sub[1].u6C.text = LangMenuCheckingCard[(g = GAME)->language];
                GetEngineSlot(1)->sub->u6C.text = LangLoadGame[g->language];
                eng_render_subs(GetEngineSlot(1));
                return;
            } else if (CardStatus & 0x10) {
                UpdateCardSlots();
                idx = CardSlotIndex * 0x194;
                p = (u8 *)CardSlots;
                if (*(s32 *)(idx + p) != 0
                    && (pad->pressed & 0x4000)) {
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                    music_request(-1);
                    src = CardSlotIndex * 0x194 + p;
                    {
                        GameState *q = GAME;
                        q->heroChoice[0] = src[0];
                        q->heroChoice[1] = src[1];
                        q->heroChoice[2] = src[2];
                        q->heroChoice[3] = src[3];
                        q->heroOut[0] = src[4];
                        q->heroOut[1] = src[5];
                        q->heroOut[2] = src[6];
                        q->heroOut[3] = src[7];
                    }
                    *GAME = *(GameState *)(src + 8);
                    {
                        GameOptionSet *d = &GameOptions;
                        GameState *s = GAME;
                        *d = *(GameOptionSet *)s->adventure.unkD0;
                    }
                    ShowCardSlots();
                    GotoProcess(&LevelLoadProcess, 0x200);
                    CardPromptStep = 3;
                }
                return;
            } else {
                if (CardStatus & 4) {
                    GetEngineSlot(1)->sub[1].u6C.text = LangCardUnformatted[GameLanguage];
                } else {
                    GetEngineSlot(1)->sub[1].u6C.text = LangNoCrashBashData[GameLanguage];
                }
                GetEngineSlot(1)->sub->u6C.text = LangLoadGame[GameLanguage];
                eng_render_subs(GetEngineSlot(1));
                return;
            }
        }
        e = GetEngineSlot(1);
        {
            GameState *q;
            e->sub[1].u6C.text = LangNoCard[(q = GAME)->language];
            GetEngineSlot(1)->sub->u6C.text = LangLoadGame[q->language];
        }
        eng_render_subs(GetEngineSlot(1));
        break;
    case 1:
        if (loader_pending() == 0) {
            GameState *q = GAME;
            CardPromptStep = 2;
            q->heroes[0] = q->heroChoice[0];
        }
        break;
    case 2:
        if (loader_pending() == 0) {
            GameState *q;
            GotoProcess(&RetryProcess, 0x200);
            q = GAME;
            CardPromptStep = 3;
            q->exitFlags |= 0x200;
        }
        break;
    }
}

const char MenuTournamentSelect[] = "TOURNAMENT SELECT";

const char MenuLocked[] = "LOCKED";

const char MenuPressButtonToSelect[] = "PRESS \374 BUTTON TO SELECT";

const char MenuInsertCardForUnlocked[] = "INSERT A MEMORY CARD\nWITH CRASH BASH DATA\nTO CHECK FOR \nUNLOCKED LEVELS";

const char MenuArrowLeft[] = "<";

const char MenuArrowRight[] = ">";

const char MenuUnlockWithCard[] = "PRESS \375BUTTON TO\nUNLOCK LOCKED LEVELS\nUSING MEMORY CARD IN\nMEMORY CARD SLOT 1";

const char MenuCheckingUnlocked[] = "CHECKING MEMORY CARD\nIN MEMORY CARD SLOT 1\nFOR UNLOCKED\nLEVELS";

const char MenuThereAreNoMoreUnlockedLevels[] = "THERE ARE NO MORE\nUNLOCKED LEVELS\nON THIS MEMORY CARD\n";

const char MenuConfirmOverwrite[] = "\01508CONFIRM OVERWRITE\n\nPRESS \377 TO SAVE\01525PRESS \376 TO EXIT\01517WITHOUT SAVING";

const char MenuPressButtonToOverwrite[] = "PRESS \374 BUTTON TO OVERWRITE";

const char MenuPressButtonToLoad[] = "PRESS \374 BUTTON TO LOAD";

const char MenuPressButtonToSave[] = "PRESS \374 BUTTON TO SAVE";

const char MenuPressButtonToExitWithoutSaving[] = "PRESS \376 BUTTON TO EXIT WITHOUT SAVING";

const char MenuSelectAGameSave[] = "SELECT A GAME SAVE";

const char MenuSelectGameToLoad[] = "SELECT GAME TO LOAD";

const char MenuCheckingCard[] = "\nCHECKING MEMORY CARD\nIN MEMORY CARD SLOT 1\nPLEASE WAIT";

const char MenuCardUnformatted[] = "WARNING\01530MEMORY CARD IS UNFORMATTED\nINSERT FORMATTED MEMORY CARD\nINTO MEMORY CARD SLOT 1\n\01535PRESS \376 BUTTON TO EXIT";

const char MenuNoCrashBashData[] = "WARNING\01525THERE IS NO CRASH BASH\nDATA ON THIS MEMORY CARD\nPLEASE INSERT ANOTHER\nMEMORY CARD\n\nPRESS \376 BUTTON TO EXIT";

const char MenuLoadGame[] = "LOAD GAME";

const char MenuNoCard[] = "WARNING\01530THERE IS NO MEMORY CARD\nIN MEMORY CARD SLOT 1\nPLEASE INSERT A MEMORY CARD\n\01535PRESS \376 BUTTON TO EXIT";

const char MenuFormatCard[] = "WARNING\01525MEMORY CARD IS UNFORMATTED\nDO YOU WANT TO FORMAT ?\01525PRESS \377 BUTTON TO FORMAT\nPRESS \376 BUTTON TO EXIT\nWITHOUT FORMATTING";

const char MenuSaveGame[] = "SAVE GAME";

const char MenuCardFull[] = "WARNING\01520THE MEMORY CARD IN MEMORY\01516CARD SLOT 1 HAS\01516INSUFFICIENT SPACE\01520PLEASE INSERT A MEMORY CARD\01516CONTAINING AT LEAST\015161 FREE BLOCK\01520PRESS \376 BUTTON TO EXIT";

const char MenuNoCard2[] = "WARNING\01530THERE IS NO MEMORY CARD\nIN MEMORY CARD SLOT 1\nPLEASE INSERT A MEMORY CARD\01530PRESS \376 BUTTON TO\nEXIT WITHOUT SAVING";

const char MenuSaving[] = "\01508SAVING\n\nPLEASE DO NOT REMOVE\nTHE MEMORY CARD";

const char MenuSaveCompleted[] = "\01508SAVE COMPLETED\n\nPRESS \374 BUTTON\nTO CONTINUE";

const char MenuSaveFailedPressButtonToExit[] = "\nSAVE FAILED\n\nPRESS \376 BUTTON TO EXIT";

const char MenuFormatting[] = "\01508FORMATTING\n\nPLEASE DO NOT REMOVE\nTHE MEMORY CARD\n";

const char MenuPressButtonToExitWithoutLoading[] = "PRESS \376 BUTTON TO EXIT WITHOUT LOADING";

const char MenuEmpty[] = "EMPTY";

const char Menu01[] = "01";

const char Menu02[] = "02";

const char Menu03[] = "03";

const char Menu04[] = "04";

const char Menu05[] = "05";

const char MenuNameWidth[] = "WWWWWWWW";

const char Menu200Percent[] = "200%";

const char MenuCardCorrupt[] = "WARNING\nTHE MEMORY CARD IN\nMEMORY CARD SLOT 1\nIS CORRUPT. PLEASE INSERT\nANOTHER MEMORY CARD\nPRESS \376 BUTTON TO EXIT\nWITHOUT SAVING";
const char MenuEmpty2[] = "";

void UpdateSaveMenu(void) {
    s32 f;

    RenderState.modelDp = ScreenFade.amount;
    switch (SaveMenuState) {
    case 0:
        f = CardStatus;
        if (f & 2) {
            if (f & 0xC) {
                if (f & 4) {
                    SAVEMENU_SET_ANIM(LangFormatCard);
                    if (Pads[0].pressed & 0x8000) {
                        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                        card_format();
                        SaveMenuState = 6;
                    }
                } else {
                    SaveMenuState = 8;
                }
            } else if (f & 0x40) {
                SAVEMENU_SET_ANIM(LangMenuCheckingCard);
            } else if (f & 0x30) {
                if (!(f & 0x10)) {
                    card_clear_slots();
                }
                UpdateCardSlots();
                if (Pads[0].pressed & 0x4000) {
                    s32 idx = CardSlotIndex * 0x194;
                    u8 *p = (u8 *)CardSlots;
                    if (*(s32 *)(idx + p) == 0) {
                        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                        if (HubSaveMenuHooks.unk0 != NULL) {
                            HubSaveMenuHooks.unk0();
                        }
                        SaveMenuState = 3;
                    } else {
                        SaveMenuState = 7;
                    }
                }
            } else {
                SAVEMENU_SET_ANIM(LangCardFull);
            }
        } else {
            SAVEMENU_SET_ANIM(LangNoCard2);
        }
        if (Pads[0].pressed & 0x1000) {
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
            GotoProcess(&RetryProcess, 0x200);
            {
                GameState *q = GAME;
                SaveMenuState = 4;
                q->exitFlags |= 0x200;
            }
        }
        break;
    case 7:
        if (CardStatus & 2) {
            SAVEMENU_SET_ANIM(LangConfirmOverwrite);
            f = Pads[0].pressed;
            if (f & 0x8000) {
                PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                if (HubSaveMenuHooks.unk0 != NULL) {
                    HubSaveMenuHooks.unk0();
                }
                SaveMenuState = 3;
            } else if (f & 0x1000) {
                SaveMenuState = 0;
            }
            break;
        }
        SaveMenuState = 0;
        break;
    case 1:
        f = CardStatus;
        if (f & 0x100) {
            SaveMenuState = 2;
        } else if (!(f & 0x80)) {
            SaveMenuState = 5;
        }
        SAVEMENU_SET_ANIM(LangSaving);
        break;
    case 5:
        SAVEMENU_SET_ANIM(LangSaveCompleted);
        if (Pads[0].pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            GotoProcess(&RetryProcess, 0x200);
            SaveMenuState = 4;
            GAME->exitFlags |= 0x200;
        }
        break;
    case 2:
        SAVEMENU_SET_ANIM(LangSaveFailedPressButtonToExit);
        if (Pads[0].pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            card_reset();
            RefreshCardSlots();
            SaveMenuState = 0;
        }
        break;
    case 3:
        f = CardStatus;
        if (!(f & 2) || !(f & 0x30)) {
            SaveMenuState = 0;
            if (HubSaveMenuHooks.unkC != NULL) {
                HubSaveMenuHooks.unkC();
            }
            PlaceEngineSlot(1, &CardPromptPanel);
        } else {
            if (HubSaveMenuHooks.unk4 != NULL) {
                HubSaveMenuHooks.unk4();
            }
            if (HubSaveMenuHooks.unk8 != NULL) {
                HubSaveMenuHooks.unk8();
            }
            switch (HubSaveChoice) {
            case 1: {
                CardSlot *dst = &CardSlots[CardSlotIndex];
                GameState *q = GAME;
                dst->summary[0] = q->heroChoice[0];
                dst->summary[1] = q->heroChoice[1];
                dst->summary[2] = q->heroChoice[2];
                dst->summary[3] = q->heroChoice[3];
                dst->summary[4] = q->heroOut[0];
                dst->summary[5] = q->heroOut[1];
                dst->summary[6] = q->heroOut[2];
                dst->summary[7] = q->heroOut[3];
                *(GameOptionSet *)q->adventure.unkD0 = GameOptions;
                dst->state = *q;
                ShowCardSlots();
                card_save();
                SaveMenuState = 1;
                if (HubSaveMenuHooks.unkC != NULL) {
                    HubSaveMenuHooks.unkC();
                }
                PlaceEngineSlot(1, &CardPromptPanel);
                break;
            }
            case 2:
                GotoProcess(&RetryProcess, 0x200);
                SaveMenuState = 4;
                {
                    GameState *q = GAME;
                    q->exitFlags |= 0x200;
                }
                if (HubSaveMenuHooks.unkC != NULL) {
                    HubSaveMenuHooks.unkC();
                }
                break;
            }
            break;
        }
    case 6:
        f = CardStatus;
        if (f & 0x100) {
            SaveMenuState = 2;
        } else if (f & 0x80) {
            SAVEMENU_SET_ANIM(LangFormatting);
        } else {
            SaveMenuState = 0;
        }
        break;
    case 8:
        if (!(CardStatus & 0xE)) {
            SaveMenuState = 0;
        }
        SAVEMENU_SET_ANIM(SaveMenuAnims);
        break;
    }
    if ((SaveMenuState == 2 || SaveMenuState == 8) && (Pads[0].pressed & 0x1000)) {
        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
        if (SaveMenuState == 2) {
            card_reset();
        }
        GotoProcess(&RetryProcess, 0x200);
        SaveMenuState = 4;
        {
            GameState *q = GAME;
            q->exitFlags |= 0x200;
        }
    }
}

void RenderSaveMenu(void) {
    EXTengine *e;
    subengine *s;
    s32 v;

    gfx_set_viewport(&SaveBackViewport, &SaveBackCamera);
    e = GetEngineSlot(8);
    s = e->sub;
    s->unk14 = FrameCount * 20;
    s->unk12 = 0x293;
    s->unk50 = 1;
    s->unk20.z = 0x2800;
    s->unk20.x = 0x2800;
    s->unk52 = 1;
    s->unk51 = 0;
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(e);
    gfx_set_viewport(&SaveViewport, &SaveCamera);
    switch (SaveMenuStep) {
    case 0:
        if (Pads[0].pressed & 0xF0) {
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            v = 1;
            if (SaveMenuYesNo == 1) {
                v = 2;
            }
            SaveMenuYesNo = v;
        }
        if (Pads[0].pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            if (SaveMenuYesNo == 1) {
                SaveMenuState = 0;
                SaveMenuStep = SaveMenuYesNo;
            } else {
                CardPromptStep = 0;
                SaveMenuStep = 2;
            }
        }
        break;
    case 1:
        UpdateSaveMenu();
        break;
    case 2:
        UpdateCardPrompt();
        break;
    }
}

void UpdateCardSlots(void) {
    PadState *pad;
    EXTengine *e;
    EXTengine *e2;

    if (CardStatus & 2) {
        u8 *c = CardBuffer;  /* the image's sum (CardSaveSum) by the buffer: one instruction less */
        if (*(s32 *)(c + 0x85C) != CardBufferStamp) {
            RefreshCardSlots();
            CardBufferStamp = *(s32 *)(c + 0x85C);
        }
    }
    pad = Pads;
    if (pad->pressed & 0x10) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex - 2);
    }
    if (pad->pressed & 0x40) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex + 2);
    }
    if (pad->pressed & 0x80) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex - 1);
    }
    if (pad->pressed & 0x20) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex + 1);
    }
    CardSlotIndex &= 3;
    e = GetEngineSlot(CardSlotIndex | 4);
    e2 = GetEngineSlot(2);
    e2->unk10.y = e->unk10.y;
    e2->unk10.x = e->unk10.x;
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(4));
    eng_render_subs(GetEngineSlot(5));
    eng_render_subs(GetEngineSlot(6));
    eng_render_subs(GetEngineSlot(7));
    eng_render_subs(GetEngineSlot(3));
    eng_render_subs(GetEngineSlot(2));
    switch (SaveMenuStep) {
    case 2:
        {
            s32 idx;
            u8 *p;
            GameState *g;
            idx = CardSlotIndex * 0x194;
            p = (u8 *)CardSlots;
            if (*(s32 *)(idx + p) == 0) {
                GetEngineSlot(3)->sub[13].u6C.dbase = 0;
            } else {
                GetEngineSlot(3)->sub[13].u6C.text = LangPressButtonToLoad[GameLanguage];
            }
            GetEngineSlot(3)->sub[12].u6C.text = LangSelectGameToLoad[(g = GAME)->language];
            GetEngineSlot(3)->sub[14].u6C.text = LangPressButtonToExitWithoutLoading[g->language];
        }
        break;
    case 1:
        {
            s32 idx;
            u8 *p;
            GameState *g;
            idx = CardSlotIndex * 0x194;
            p = (u8 *)CardSlots;
            if (*(s32 *)(idx + p) == 0) {
                GetEngineSlot(3)->sub[13].u6C.text = LangSaveOrExit[GameLanguage];
            } else {
                GetEngineSlot(3)->sub[13].u6C.text = LangPressButtonToOverwrite[GameLanguage];
            }
            GetEngineSlot(3)->sub[12].u6C.text = LangSelectAGameSave[(g = GAME)->language];
            GetEngineSlot(3)->sub[14].u6C.text = LangPressButtonToExitWithoutSaving[g->language];
        }
        break;
    }
}

void ShowCardSlots(void) {
    CardSlot *p = CardSlots;
    GameState *pad = &p->state;

    /* a slot is empty when its summary's first word is 0 */
    ShowCardSlot(pad, 0, *(s32 *)CardSlots[0].summary == 0, 0x28, 0x1E);
    ShowCardSlot(&p[1].state, 1, *(s32 *)p[1].summary == 0, 0x140, 0x1E);
    ShowCardSlot(&p[2].state, 2, *(s32 *)p[2].summary == 0, 0x28, 0x7C);
    ShowCardSlot(&p[3].state, 3, *(s32 *)p[3].summary == 0, 0x140, 0x7C);
    PlaceEngineSlot(3, &CardSlotsPanel);
    ShowCardSlotStats((GameState *)((u8 *)pad + CardSlotIndex * sizeof(CardSlot)), *(s32 *)p[CardSlotIndex].summary == 0);
}

void InitSaveMenu(void) {
    subengine *sub;

    CardSlotIndex = 0;
    CardBufferStamp = 0x4D2;
    ShowSaveScreen();
    FontPage = 0;
    SaveMenuStep = 0;
    SaveMenuYesNo = 1;
    PlaceEngineSlot(8, &SaveMenuPanel);
    sub = GetEngineSlot(8)->sub;
    if (GameMode == 0x28) {
        sub->entity = 0x2007;
        sub->u6C.dbase = &HeroModelBank[DB_MainmenuModels];
    } else {
        sub->entity = 0x201F;
        sub->u6C.dbase = &HeroModelBank[DB_AdventureItems];
    }
    PlaceEngineSlot(1, &CardPromptPanel);
    SaveMenuStage = 0;
    SaveMenuState = 0;
    SaveMenuStep = 1;
}

void InitCardPromptMenu(void) {
    subengine *sub;

    CardSlotIndex = 0;
    CardBufferStamp = 0x4D2;
    ShowSaveScreen();
    FontPage = 0;
    SaveMenuStep = 0;
    SaveMenuYesNo = 1;
    PlaceEngineSlot(8, &SaveMenuPanel);
    sub = GetEngineSlot(8)->sub;
    if (GameMode == 0x28) {
        sub->entity = 0x2007;
        sub->u6C.dbase = &HeroModelBank[DB_MainmenuModels];
    } else {
        sub->entity = 0x201F;
        sub->u6C.dbase = &HeroModelBank[DB_AdventureItems];
    }
    PlaceEngineSlot(1, &CardPromptPanel);
    CardPromptStep = 0;
    SaveMenuStep = 2;
}

void ShowSaveScreen(void) {
    EXTengine *eng;

    ShowCardSlots();
    PlaceEngineSlot(2, &SaveScreenPanel);
    GetEngineSlot(2)->unk10.x = 0x28;
    GetEngineSlot(2)->unk10.y = 0x1E;
    eng = GetEngineSlot(2);
    eng->flags |= 0x10000000;
    cam_init(&SaveCamera);
    SaveCamera.eye.z = 0x5DC;
    cam_init(&SaveBackCamera);
    SaveBackCamera.eye.z = 0x400;
    SaveBackCamera.dist = 0x190;
    set_fade(&ScreenFade, -0x200, 0);
}

void RefreshCardSlots(void) {
    EXTengine *eng;

    ShowCardSlots();
    PlaceEngineSlot(2, &SaveScreenPanel);
    GetEngineSlot(2)->unk10.x = 0x28;
    GetEngineSlot(2)->unk10.y = 0x1E;
    eng = GetEngineSlot(2);
    eng->flags |= 0x10000000;
}

void RestoreHeroChoice(u8 *src) {
    GameState *g = GAME;

    g->heroChoice[0] = src[0];
    g->heroChoice[1] = src[1];
    g->heroChoice[2] = src[2];
    g->heroChoice[3] = src[3];
    g->heroOut[0] = src[4];
    g->heroOut[1] = src[5];
    g->heroOut[2] = src[6];
    g->heroOut[3] = src[7];
    *GAME = *(GameState *)(src + 8);
    {
        GameOptionSet *d = &GameOptions;
        GameState *s = GAME;
        *d = *(GameOptionSet *)s->adventure.unkD0;
    }
    ShowCardSlots();
}

void SaveHeroChoice(u8 *dst) {
    u8 *g = GameInfo;

    dst[0] = g[0x4C];
    dst[1] = g[0x4D];
    dst[2] = g[0x4E];
    dst[3] = g[0x4F];
    dst[4] = g[0x5C];
    dst[5] = g[0x5D];
    dst[6] = g[0x5E];
    dst[7] = g[0x5F];
    *(GameOptionSet *)(g + 0xD0) = GameOptions;
    *(GameState *)(dst + 8) = *(GameState *)g;
    ShowCardSlots();
}

s32 CardSlotEmpty(s32 *arg0) {
    return *arg0 == 0;
}

void UpdateSaveScreen(void) {
}

void ClearEngineSlots(void) {
    KillEngineSlot(2);
    KillEngineSlot(3);
    KillEngineSlot(4);
    KillEngineSlot(5);
    KillEngineSlot(6);
    KillEngineSlot(7);
    KillEngineSlot(8);
    prims_empty_back();
    prims_empty();
    primpool_free();
}

void InitCardMenu(void) {
    subengine *sub;

    CardSlotIndex = 0;
    CardBufferStamp = 0x4D2;
    ShowSaveScreen();
    FontPage = 0;
    SaveMenuStep = 0;
    SaveMenuYesNo = 1;
    PlaceEngineSlot(8, &SaveMenuPanel);
    sub = GetEngineSlot(8)->sub;
    if (GameMode == 0x28) {
        sub->entity = 0x2007;
        sub->u6C.dbase = &HeroModelBank[DB_MainmenuModels];
    } else {
        sub->entity = 0x201F;
        sub->u6C.dbase = &HeroModelBank[DB_AdventureItems];
    }
    PlaceEngineSlot(1, &CardPromptPanel);
}

s32 GameExtra105(u8 *arg0) {
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

s32 GameCount105(u8 *arg0) {
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

s32 GameSum126(u8 *arg0) {
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

s32 GameSum147(u8 *arg0) {
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

void GameSetSlot3Digits(s32 arg0) {
    s32 *src;
    s32 *p;
    s32 i;
    s32 *base;

    src = (s32 *)(arg0 * 4);
    src = (s32 *)((s32)src + arg0);
    src = (s32 *)((s32)src * 4);
    src = (s32 *)((s32)src + (s32)Slot3DigitSrc);
    base = (s32 *)GetEngineSlot(3)->sub;
    p = base + 0xA8 / 4;
    i = 0;
    do {
        if (*src == -1) {
            p[0] &= ~0x8000;
            p[0x2A] &= ~0x8000;
        } else {
            p[0] |= 0x8000;
            p[0x2A] |= 0x8000;
            p[1] = *src;
            p[0x2B] = *src;
        }
        p += 0x54;
        i++;
        src++;
    } while (i < 5);
}

void InitGameState(GameState *arg0) {
    s32 i;
    u8 t;
    u8 *p;
    s16 j;
    s16 k;

    arg0->mode = 0x28;
    arg0->arena = 0;
    arg0->arenaLevel = 0;
    arg0->gateway = -1;
    arg0->returnToHub = 0;
    arg0->arena = 0;
    arg0->arenaLevel = 0;
    k = ' ';
    for (i = 7, p = (u8 *)arg0 + i; i >= 0; i--, p--) {  /* unk18[i] loses the stepped pointer p */
        p[0x18] = k;
    }
    t = 0x3C;
    do {
        j = 1;
    } while (0);
    arg0->levelFlags = 0x14;
    arg0->unk3E = 0x200;
    arg0->unk20 = 0;
    arg0->adventure.unkE0 = 0;
    arg0->levelRecIndex = 0;
    arg0->frames = 0;
    arg0->aiLevel = j;
    arg0->exitDelay = 0;
    arg0->exitFlags = 0;
    arg0->pause = 0;
    if (VideoMode == 1) {
        t = 0x32;
    }
    arg0->unk21 = t;
    arg0->heroChoice[0] = j;
    arg0->battleWins = 3;
    arg0->heroChoice[1] = 3;
    arg0->heroChoice[2] = 5;
    arg0->heroChoice[3] = 7;
    arg0->teamSide[0] = 1;
    arg0->teammateChoice[0] = -1;
    arg0->teammateChoice[1] = -1;
    arg0->teammateChoice[2] = -1;
    arg0->teammateChoice[3] = -1;
    arg0->demoTimer = 0;
    arg0->demo = 0;
    arg0->winner[0] = 0;
    arg0->winner[1] = 0;
    arg0->heroOut[0] = 0;
    arg0->heroOut[1] = 0;
    arg0->heroOut[2] = 0;
    arg0->heroOut[3] = 0;
    arg0->teamSide[1] = 2;
    arg0->teamLeader[0] = 0;
    arg0->teamLeader[1] = 2;
    arg0->heroes[0] = arg0->heroChoice[0];
    arg0->heroes[1] = arg0->heroChoice[1];
    arg0->heroes[2] = arg0->heroChoice[2];
    arg0->heroes[3] = arg0->heroChoice[3];
    arg0->teammates[0] = arg0->teammateChoice[0];
    arg0->teammates[1] = arg0->teammateChoice[1];
    arg0->adventure.unk74 = 0;
    arg0->adventure.unk75 = 0;
    arg0->teammates[2] = arg0->teammateChoice[2];
    arg0->teammates[3] = arg0->teammateChoice[3];
    for (i = 0; i < 0x21; i++) {
        arg0->adventure.unkAB[i] = 0;
        arg0->adventure.unk76[i] = 0;
        arg0->adventure.unkE4[0][i] = 0;
        arg0->adventure.unkE4[2][i] = 0;
        arg0->adventure.unkE4[1][i] = 0;
        arg0->adventure.unkE4[3][i] = 0;
        arg0->adventure.unkE4[4][i] = 0;
    }
    t = 1;
    arg0->adventure.unkA7[2] = 2;
    arg0->adventure.unkA7[3] = 3;
    arg0->adventure.unk9B[0] = 0;
    arg0->adventure.unk9B[1] = 0;
    arg0->adventure.unk9B[2] = 0;
    arg0->adventure.unk9B[3] = 0;
    arg0->adventure.unkCC[0] = 0;
    arg0->adventure.unkCC[1] = 0;
    arg0->adventure.unkCC[2] = 0;
    arg0->adventure.unkCC[3] = 0;
    arg0->adventure.unk9F[0] = 0;
    arg0->adventure.unk9F[1] = 0;
    arg0->adventure.unk9F[2] = 0;
    arg0->adventure.unk9F[3] = 0;
    arg0->adventure.unkA7[0] = 0;
    arg0->adventure.unkA7[1] = t;
    arg0->players = t;
    arg0->maxPlayers = 0;
    arg0->cupLeader = -1;
    arg0->teams = 0;
    arg0->adventure.unk70 = 0;
    arg0->language = GraphicsMode;
    *(GameOptionSet *)arg0->adventure.unkD0 = GameOptions;
}

void InitGraphics(void) {
    EngineCfg *cfg = &DefaultLevelConfig;
    GameState *g;

    AllocOtBuffers((s32 *)cfg);
    ((void (*)(u8 *))ResetScreen)((u8 *)(VideoMode == 0 ? &vmode_512_ntsc : &vmode_512_pal));
    g = GAME;
    g->cheats = 0x80000000;
    vram_init(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    prims_init(cfg->pools);
    dbase_init_all();
    ResetEnginePool();
    GraphicsMode = 0;
    IdleFrames = 0;
    core_process = 0;
    GameOptions.music = 0x46;
    GameOptions.sfx = 0x64;
    GameOptions.stereo = 1;
    GameOptions.vibrate[0] = 1;
    GameOptions.vibrate[1] = 1;
    GameOptions.vibrate[2] = 1;
    GameOptions.vibrate[3] = 1;
    InitGameState(g);
    sound_init();
}

void StartGame(void) {
    ProcessControl.process = NULL;
    process_control_init(&ProcessControl, NULL);
    InitGraphics();
    ScreenFade.amount = 0x1000;
    ScreenFade.maxed = 3;
    ScreenFade.speed = 0x100;
    SetDispMask(1);
    FirstBootDone = 1;
    GotoProcess(&LevelLoadProcess, 0x200);
    fade_update(&ScreenFade);
    process_control_update(&ProcessControl, &ScreenFade);
    process_control_render(&ProcessControl);
    if (ProcessControl.process != NULL) {
        if (ProcessControl.process->quit != NULL) {
            ProcessControl.process->quit();
        }
    }
}

void GotoProcessFade(process_info *arg0) {
    if (ProcessControl.next_process != arg0) {
        ((void (*)(fade_info *))set_fade)(&ScreenFade);
        ProcessControl.next_process = arg0;
    }
}

void GotoProcess(process_info *process, s32 speed) {
    process_control_info *p = &ProcessControl;

    if (p->next_process != process) {
        set_fade(&ScreenFade, speed, 0);
        p->next_process = process;
    }
}

void QuitProcess(void) {
    if (ProcessControl.process != NULL) {
        if (ProcessControl.process->quit != NULL) {
            ProcessControl.process->quit();
        }
    }
}

void RenderProcess(void) {
    process_control_render(&ProcessControl);
}

void UpdateProcess(void) {
    fade_update(&ScreenFade);
    process_control_update(&ProcessControl, &ScreenFade);
}

void RestartGame(void) {
    ProcessControl.process = NULL;
    process_control_init(&ProcessControl, NULL);
    InitGraphics();
    ScreenFade.amount = 0x1000;
    ScreenFade.maxed = 3;
    ScreenFade.speed = 0x100;
    SetDispMask(1);
    FirstBootDone = 1;
    GotoProcess(&LevelLoadProcess, 0x200);
}

void ResetGameState(void) {
    InitGameState(GameInfo);
}

void InitGameOptions(void) {
    GameOptionSet *p;
    GameOptions.music = 0x46;
    p = &GameOptions;
    p->sfx = 0x64;
    p->stereo = 1;
    p->vibrate[0] = 1;
    p->vibrate[1] = 1;
    p->vibrate[2] = 1;
    p->vibrate[3] = 1;
}

void InitRetry(void) {
    GameState *g;

    g = (GameState *)GameInfo;
    g->exitFlags &= 0x7FF8;
    set_fade(&ScreenFade, -0x300, 0);
    GameSceneFade = 0;
    if (g->mode == 0x24 && g->adventure.unkE0 <= 0) {
        g->adventure.unkE0 = 1;
    }
    g = (GameState *)GameInfo;
    if (CountTrophies(g) >= 0x19 && g->adventure.unkE0 < 2) {
        g->adventure.unkE0 = 2;
    }
    prims_empty_back();
    mal_compact(&MainHeap);
    FirstBootDone = 0;
    if (GameExitFlags & 0x200) {
        InitialiseLevel();
    } else {
        music_request(1);
    }
    EngineBeginMode(1);
}

void QuitGame(void) {
    scripts_kill_all();
    QuitLevel();
    sfx_delete_all();
    process_quit(&MenuProcessControl);
    prims_empty();
    primpool_free();
    HandleLevelExit();
}

void RenderGame(void) {
    RenderHook();
    RenderState.modelDp = ScreenFade.amount;
    RenderPause();
    process_control_render(&MenuProcessControl);
}

void UpdateGame(void) {
    if (UpdatePause() != 0) {
        GameState *g;
        GameFrameCount++;
        g = GAME;
        g->frames += 1;
        UpdateLevel();
        ((void (*)(void *, u8 *))map_update)(MapScene, (u8 *)&MainCamera.at.x);
    } else {
        RenderLevel();
    }
    MainGfxCamera.at.z = 0;
    MainGfxCamera.eye.z = 0x800;
    HudCamera.at.z = 0;
    HudCamera.eye.z = 0x40C;
    UpdateScreenLabels();
    scripts_update();
    process_control_update(&MenuProcessControl, &ScreenFade);
    UpdateExitDelay();
}

void SetReturnGateway(void) {
    GAME->gateway = 0xA;
    GAME->returnToHub = 1;
}

void UpdateSecretLevel(void) {
    GameState *a = (GameState *)GameInfo;
    GameState *g;

    if (a->mode == 0x24 && a->adventure.unkE0 <= 0) {
        a->adventure.unkE0 = 1;
    }
    g = (GameState *)GameInfo;
    if (CountTrophies(g) >= 0x19 && g->adventure.unkE0 < 2) {
        g->adventure.unkE0 = 2;
    }
}

void RenderGameScene(void) {
    gfx_set_viewport(&GameViewport, &MainCamera);
    RenderState.modelDp = 0x1000 - ((0x1000 - GameSceneFade) * (0x1000 - ScreenFade.amount) / 4096);
    PrimDepthTest = 0;
    GameSceneCounts[0] = 0;
    GameSceneCounts[1] = 0;
    GameSceneCounts[2] = 0;
    scripts_render();
    RenderAllNas();
    draw_map_scene(MapScene);
    gfx_set_viewport(&MainViewport, &MainGfxCamera);
    RenderState.modelDp = ScreenFade.amount;
    DrawLevelHud();
    RenderScreenLabels();
}

void SetGameViewport(void) {
    gfx_set_viewport(&GameViewport, &MainCamera);
    RenderState.modelDp = 0x1000 - ((0x1000 - GameSceneFade) * (0x1000 - ScreenFade.amount) / 4096);
}

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */
extern ScreenMode vmode_256_ntsc;
extern ScreenMode vmode_320_ntsc;
extern ScreenMode vmode_384_ntsc;
extern ScreenMode vmode_640_ntsc;

HitSfxTable HitSfx = {
    { 0x3EC, 0x3ED, 0x3EE, 0x3EF },
    { 0x3F1, 0x3F1, 0x3F3, 0x3F4 },
    { 0x3E8, 0x3E8, 0x3EA, 0x3EB },
    { 0x3F5, 0x3F5, 0x3F7, 0x3F8 },
};
s32 CrateAstType = 0;
s32 CrateAstType2 = 0;
u16 CrateCounts[14] = {0};
HitCallbacks HitHooks = {0};
u8 IntroViewport[40] = {
    0, 0, 0, 0, 0x80, 2, 0xE0, 1, 0, 0, 0, 0, 0x80, 2, 0xE0, 1, 0x40, 1, 0, 0, 0xF0, 0, 0, 0,
    0, 2, 0, 0, 0xFF, 0xD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
IntroStep IntroSteps[4] = {
    { 1, (void *)&DatIndex[2 * 139], 0, 0, -1, { 0x3B6, 0x3B7, 0x3B8 }, 0x3B9, 0x5A },
    { 1, (void *)&DatIndex[2 * 138], 0, 0, -1, { 0x3BB, 0x3BC, 0x3BD }, 0x3BE, 0x8C },
    { 2, 0, (void *)&HeroModelBank[DB_CutsceneIntroEurocom], 0, -1, { 0x3C0, 0x3C1, 0x3C2 }, 0x3C3, 0x14 },
    { 2, 0, (void *)&HeroModelBank[DB_CutsceneIntroLogo2], 0, 1, { 0x3C5, 0x3C6, 0x3C7 }, 0x3C8, 0x14 },
};
Blk32 IntroScriptRot = { { 0x1000, 0, 0x1000, 0, 0x1000, 0, 0, 0 } };
CamVec3 IntroScriptPos = {0};
process_info IntroProcess = { InitIntro, UpdateIntro, RenderIntro, QuitIntro, 0 };
s32 IntroUnused = 0;
s32 IntroStepIndex = 0;
s32 IntroStepTimer = 0;
script_handler *IntroScript = 0;
s32 IntroNextSfx = 0;
s32 IntroSfxDelay = 0;
_sfx_handler *IntroSfx[3] = {0};
_sfx_handler *IntroSfxHandle = 0;
s32 WarpReturnProcess[5] = { (s32)ReturnToWarpRoom, 0, 0, 0, 0 };
s32 NextGameMode = 0;
CupRound Cup0Arenas[4] = {
    { 1, 0, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2005 },
    { 1, 1, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2006 },
    { 1, 2, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2007 },
    { 1, 3, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2008 },
};
CupRound Cup1Arenas[4] = {
    { 1, 0x13, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2005 },
    { 1, 0x12, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2006 },
    { 1, 0x14, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2007 },
    { 1, 0x15, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2008 },
};
CupRound Cup2Arenas[4] = {
    { 1, 0xA, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2001 },
    { 1, 0xB, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2002 },
    { 1, 0xC, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2003 },
    { 1, 0xD, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2001 },
};
CupRound Cup3Arenas[4] = {
    { 1, 0xE, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2001 },
    { 1, 0xF, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2002 },
    { 1, 0x10, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2003 },
    { 1, 0x11, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2004 },
};
CupRound Cup4Arenas[4] = {
    { 2, 6, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2009 },
    { 2, 8, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x200A },
    { 2, 7, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x200B },
    { 2, 5, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x200C },
};
CupRound Cup5Arenas[4] = {
    { 2, 0x17, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2009 },
    { 2, 0x18, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200A },
    { 2, 0x1A, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200B },
    { 2, 0x19, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200C },
};
CupRound Cup6Arenas[4] = {
    { 2, 0x1B, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200D },
    { 2, 0x1C, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200E },
    { 2, 0x1D, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200F },
    { 2, 0x1E, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2010 },
};
CupDef CupTable[7] = {
    { 1, Cup0Arenas, 4 }, { 1, Cup1Arenas, 4 },
    { 1, Cup2Arenas, 4 }, { 1, Cup3Arenas, 4 },
    { 3, Cup4Arenas, 4 }, { 3, Cup5Arenas, 4 },
    { 3, Cup6Arenas, 4 },
};
u8 CupMenuPanel[180] = {
    4, 0, 0, 0, 0x32, 0, 0x36, 1, 0x58, 0, 0xD0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0x4A, 1, 0x4E, 2, 0x58, 0, 0xD0, 0, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0x32, 0, 0x36, 1,
    0xE0, 0, 0x58, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 0, 0, 0, 0x4A, 1, 0x4E, 2, 0xE0, 0, 0x58, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 LangTournamentSelect[1] = { (s32)&MenuTournamentSelect };
s32 LangMenuLocked[1] = { (s32)&MenuLocked };
s32 LangMenuPressButtonToSelect[1] = { (s32)&MenuPressButtonToSelect };
TextPanelItem CupMenuLabels[12] = {
    {
        { 3, 0, 0, 0, 0, 0x80, 0x72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0,
        { 2, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 0xE, 0, 0, 0, 0, 0x80, 0x38, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        (void *)&LangTournamentSelect, { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 4, 0, 0, 0, 0, 0x80, 0x58, 2, 0x36, 0xFF, 0x5A, 0xFF, 1, 0, 0, 0, 0, 0, 0, 0 }, 0,
        { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 4, 0, 0, 0, 0, 0x80, 0x90, 1, 0x92, 0xFF, 0x5A, 0, 4, 0, 0, 0, 0, 0, 0, 0 }, 0,
        { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 6, 0, 0, 0, 0x60, 0xFF, 0x2E, 0, 0, 0, 0x66, 6, 0, 0, 0, 0, 0, 0, 0, 0 }, 0,
        { 0, 0, 0, 0, 0xC7, 0 }, 0x2006, { 0, 0, 0, 0 },
    },
    {
        { 0xE, 0, 0, 0, 0x32, 0x80, 0x9A, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        (void *)&LangMenuLocked, { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 3, 0, 0, 0, 0x32, 0x80, 0xD8, 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        (void *)&MenuInsertCardForUnlocked, { 4, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 0xE, 0, 0, 0, 0, 0x80, 0xA5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        (void *)&LangMenuPressButtonToSelect, { 4, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 3, 0, 0, 0, 0xF7, 0xFE, 0x72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        (void *)&MenuArrowLeft, { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 3, 0, 0, 0, 0xF0, 0, 0x72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        (void *)&MenuArrowRight, { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 4, 0, 0, 0, 0, 0x80, 0x58, 2, 0x9A, 0, 0xC0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, 0,
        { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0,
        { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
};
s32 LangUnlockWithCard[1] = { (s32)&MenuUnlockWithCard };
s32 LangInsertCardForUnlocked[1] = { (s32)&MenuInsertCardForUnlocked };
s32 LangCheckingUnlocked[1] = { (s32)&MenuCheckingUnlocked };
s32 LangThereAreNoMoreUnlockedLevels[1] = { (s32)&MenuThereAreNoMoreUnlockedLevels };
GfxViewport CupMenuViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0, 0, 0x800, 0x7FF, -0x80, 0, 0, 0, 1 };
GfxViewport CupViewport0 = { 0x32, 0x58, 0x136, 0xD0, 0, 0, 0x280, 0x1E0, 0xB4, 0xF8, 0x200, 0x17F, 0, 0, 0, 0, 0 };
GfxViewport CupViewport1 = { 0x14A, 0x58, 0x24E, 0xD0, 0, 0, 0x280, 0x1E0, 0x1CC, 0xF8, 0x380, 0x17F, 0, 0, 0, 0, 0 };
GfxViewport CupViewport2 = { 0x32, 0xE0, 0x136, 0x158, 0, 0, 0x280, 0x1E0, 0xB4, 0x180, 0x500, 0x17F, 0, 0, 0, 0, 0 };
GfxViewport CupViewport3 = { 0x14A, 0xE0, 0x24E, 0x158, 0, 0, 0x280, 0x1E0, 0x1CC, 0x180, 0x680, 0x17F, 0, 0, 0, 0, 0 };
TextPanelItem CupSlotPanel[2] = {
    {
        { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xCC, 0xC, 0, 0, 0, 0, 0, 0, 0, 0 }, 0,
        { 0, 0, 0, 0, 0xC8, 0 }, 0x2005, { 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0,
        { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 },
    },
};
process_info TournamentSelect = { InitCupMenu, UpdateCupMenu, RenderCupMenu, QuitCupMenu, 0 };
s32 CupMenuCompact = 0;
HubTableEntry WarpRoomBossItems[4] = {
    { 1, 2, 0xA, 3 },
    { 2, 2, 0x13, 3 },
    { 3, 2, 0xB, 3 },
    { 0, 0, 0, 0 },
};
hub_warp_info WarpRoom1Doors[4] = { { 0x2010, 0x22 }, { 0x2012, 0x23 }, { 0x2014, 0x24 }, { 0x2016, 0x25 } };
HubTableEntry WarpRoom1Items[8] = {
    { 1, 2, 0xE, 3 },
    { 2, 2, 0x13, 3 },
    { 3, 2, 0xA, 3 },
    { 4, 2, 0, 3 },
    { 5, 2, 4, 3 },
    { 0xB, 5, 0, 3 },
    { 0xC, 4, 0, 3 },
    { 0, 0, 0, 0 },
};
hub_warp_info WarpRoom2Doors[4] = { { 0x200F, 0x21 }, { 0x2013, 0x23 }, { 0x2015, 0x24 }, { 0x2017, 0x25 } };
HubTableEntry WarpRoom2Items[9] = {
    { 1, 2, 0xF, 3 },
    { 2, 2, 0x12, 3 },
    { 3, 2, 0xB, 3 },
    { 4, 2, 1, 3 },
    { 5, 2, 6, 3 },
    { 6, 2, 0x16, 3 },
    { 0xB, 5, 0, 3 },
    { 0xC, 4, 0, 3 },
    { 0, 0, 0, 0 },
};
hub_warp_info WarpRoom3Doors[4] = { { 0x200E, 0x21 }, { 0x2010, 0x22 }, { 0x2014, 0x24 }, { 0x2016, 0x25 } };
HubTableEntry WarpRoom3Items[10] = {
    { 1, 2, 0x10, 3 },
    { 2, 2, 0x14, 3 },
    { 3, 2, 0xC, 3 },
    { 4, 2, 2, 3 },
    { 5, 2, 8, 3 },
    { 6, 2, 0x17, 3 },
    { 7, 2, 9, 3 },
    { 0xB, 5, 0, 3 },
    { 0xC, 4, 0, 3 },
    { 0, 0, 0, 0 },
};
hub_warp_info WarpRoom4Doors[4] = { { 0x200F, 0x21 }, { 0x2011, 0x22 }, { 0x2013, 0x23 }, { 0x2017, 0x25 } };
HubTableEntry WarpRoom4Items[11] = {
    { 1, 2, 0x11, 3 },
    { 2, 2, 0x15, 3 },
    { 3, 2, 0xD, 3 },
    { 4, 2, 3, 3 },
    { 5, 2, 7, 3 },
    { 6, 2, 0x18, 3 },
    { 7, 2, 0x1B, 3 },
    { 8, 2, 0x1F, 3 },
    { 0xB, 5, 0, 3 },
    { 0xC, 4, 0, 3 },
    { 0, 0, 0, 0 },
};
hub_warp_info WarpRoom5Doors[4] = { { 0x200E, 0x21 }, { 0x2010, 0x22 }, { 0x2012, 0x23 }, { 0x2014, 0x24 } };
HubTableEntry WarpRoom5Items[9] = {
    { 1, 2, 0x19, 3 },
    { 2, 2, 0x1C, 3 },
    { 3, 2, 0x1D, 3 },
    { 4, 2, 5, 3 },
    { 5, 2, 0x1E, 3 },
    { 6, 2, 0x1A, 3 },
    { 0xB, 5, 0, 3 },
    { 0xC, 4, 0, 3 },
    { 0, 0, 0, 0 },
};
hub_info WarpRooms[10] = {
    { 0x21, WarpRoom1Items, 4, WarpRoom1Doors, 0x5021 },
    { 0x22, WarpRoom2Items, 4, WarpRoom2Doors, 0x502D },
    { 0x23, WarpRoom3Items, 4, WarpRoom3Doors, 0x502E },
    { 0x24, WarpRoom4Items, 4, WarpRoom4Doors, 0x502A },
    { 0x25, WarpRoom5Items, 4, WarpRoom5Doors, 0x502D },
    { 0x21, WarpRoom1Items, 0, NULL, 0 },
    { 0x21, WarpRoom1Items, 0, NULL, 0 },
    { 0x21, WarpRoom1Items, 0, NULL, 0 },
    { 0x26, WarpRoomBossItems, 0, NULL, 0 },
    { 0x27, WarpRoomBossItems, 0, NULL, 0 },
};
HubRing HubHeroRing = {0};
s32 CardPromptPanel = 3;
s32 CardPromptItems[44] = {  /* CardPromptPanel's records */
    0x328000, 0, 0, 0, 0, 0, 0, 0, 3, 0x788000, 0, 0, 0, 0, 0, 0, 0, 4, 0x1B88000, 0x19A0072,
    0, 0, 0, 0, 0, 0, 4, 0x2808000, 0x5A0028, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 SaveScreenPanel = 4;
s32 SaveScreenItems[17] =  /* SaveScreenPanel's records */ { 0x1180000, 0x5A0000, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
void *LangConfirmOverwrite[1] = { (void *)&MenuConfirmOverwrite };
void *LangPressButtonToOverwrite[1] = { (void *)&MenuPressButtonToOverwrite };
void *LangPressButtonToLoad[1] = { (void *)&MenuPressButtonToLoad };
void *LangSaveOrExit[2] = { (void *)&MenuPressButtonToSave, (void *)&MenuPressButtonToExitWithoutSaving };
void *LangSelectAGameSave[1] = { (void *)&MenuSelectAGameSave };
void *LangPressButtonToExitWithoutSaving[1] = { (void *)&MenuPressButtonToExitWithoutSaving };
void *LangSelectGameToLoad[1] = { (void *)&MenuSelectGameToLoad };
void *LangMenuCheckingCard[1] = { (void *)&MenuCheckingCard };
void *LangCardUnformatted[1] = { (void *)&MenuCardUnformatted };
void *LangNoCrashBashData[1] = { (void *)&MenuNoCrashBashData };
void *LangLoadGame[1] = { (void *)&MenuLoadGame };
void *LangNoCard[1] = { (void *)&MenuNoCard };
void *LangFormatCard[1] = { (void *)&MenuFormatCard };
void *LangSaveGame[1] = { (void *)&MenuSaveGame };
void *LangCardFull[1] = { (void *)&MenuCardFull };
void *LangNoCard2[1] = { (void *)&MenuNoCard2 };
void *LangSaving[1] = { (void *)&MenuSaving };
void *LangSaveCompleted[1] = { (void *)&MenuSaveCompleted };
void *LangSaveFailedPressButtonToExit[1] = { (void *)&MenuSaveFailedPressButtonToExit };
void *LangFormatting[1] = { (void *)&MenuFormatting };
void *LangPressButtonToExitWithoutLoading[1] = { (void *)&MenuPressButtonToExitWithoutLoading };
void *LangEmpty[1] = { (void *)&MenuEmpty };
s32 CardSlotsPanel = 4;
s32 CardSlotsItems[143] = {  /* CardSlotsPanel's records */
    0x2300000, 0x500000, 6, 0, 0, 0, 0, 0, 0xA, 0x260050, 0xB330000, 0x280000, 0, 0, 0,
    0x2022001C, 2, 3, 0x320050, 0, 0, 0, (s32)&Menu01, 4, 0, 0, 0xA, 0x2600B4, 0xB330000,
    0x280000, 0, 0, 0, 0x201F001C, 2, 3, 0x3200B4, 0, 0, 0, (s32)&Menu02, 4, 0, 0, 0xA,
    0x260118, 0xB330000, 0x280000, 0, 0, 0, 0x201E001C, 2, 3, 0x320118, 0, 0, 0, (s32)&Menu03,
    4, 0, 0, 0xA, 0x26017C, 0xB330000, 0x280000, 0, 0, 0, 0x2021001C, 2, 3, 0x32017C, 0, 0, 0,
    (s32)&Menu04, 4, 0, 0, 0xA, 0x2601E0, 0xB330000, 0x280000, 0, 0, 0, 0x2020001C, 2, 3,
    0x3201E0, 0, 0, 0, (s32)&Menu05, 4, 0, 0, 4, 0x2300000, 0xCE0056, 6, 0, 0, 0, 0, 0, 3,
    0x608118, 0, 0, 0, 0, 0, 0, 0, 3, 0x8A8118, 0, 0, 0, 0, 4, 0, 0, 3, 0xAC8118, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 Slot3DigitSrc[4][5] = {
    { 0xB4, 0x118, 0x17C, -1, -1 }, { 0x82, 0xE6, 0x14A, 0x1AE, -1 },
    { 0x50, 0xB4, 0x118, 0x17C, 0x1E0 }, { -1, -1, -1, -1, -1 },
};
s32 CardSlotPanel[72] = {
    4, 0x1180000, 0x5A0000, 6, 0, 0, 0, 0, 0, 3, 0x2000C, 0, 0, 0, (s32)&MenuNameWidth, 0, 0,
    0, 3, 0x200BC, 0, 0, 0, (s32)&Menu200Percent, 0, 0, 0, 6, 0x24000C, 0x10000000, 0, 0, 0, 0,
    0x3003001C, 0, 6, 0x240048, 0x10000000, 0, 0, 0, 0, 0x3003001C, 0, 6, 0x240084, 0x10000000,
    0, 0, 0, 0, 0x3003001C, 0, 6, 0x2400C0, 0x10000000, 0, 0, 0, 0, 0x3003001C, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};
s32 CardSlotIndex = 0;
GfxViewport SaveViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0, 0, 0, 0xDFF, 0, 0, 0, 0x10, 1 };
GfxViewport SaveBackViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0xF0, 0x200, 0xD00, -0x80, 0, 0, 0, 1 };
s32 SaveMenuPanel = 6;
s32 SaveMenuItems[17] =  /* SaveMenuPanel's records */ { 0xFF9C, 0x100004C8, 0, 0, 0, 0, 0x200700C7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
void *SaveMenuAnims[6] = {
    (void *)&MenuCardCorrupt, (void *)InitCardMenu, (void *)UpdateSaveScreen,
    (void *)RenderSaveMenu, (void *)ClearEngineSlots, 0,
};
process_info SaveScreenProcess = { InitSaveMenu, UpdateSaveScreen, RenderSaveMenu, ClearEngineSlots, 0 };
s32 SaveScreenProcess2[5] = { (s32)InitCardPromptMenu, (s32)UpdateSaveScreen, (s32)RenderSaveMenu, (s32)ClearEngineSlots, 0 };
s32 SaveMenuStage = 0;
/* the results screen's number texts, one buffer: +0, +3, +6, +9 three digits each, +12 the last */
char CardStatsText[16] = {0};
char PercentText[4][5] = {0};
s32 CardBufferStamp = 0;
camera_info_tag SaveCamera = {0};
camera_info_tag SaveBackCamera = {0};
s32 SaveMenuStep = 0;
s32 SaveMenuYesNo = 0;
s32 CardPromptStep = 0;
s32 CardPromptRetry = 0;
s32 SaveMenuState = 0;
fade_info ScreenFade = {0};
s32 IdleFrames = 0;
process_control_info ProcessControl = {0};
s32 ScreenModesByWidth[5] = { (s32)&vmode_640_ntsc, (s32)&vmode_512_ntsc, (s32)&vmode_384_ntsc, (s32)&vmode_320_ntsc, (s32)&vmode_256_ntsc };
GfxViewport MainViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0, 0, 0, 0x200, -0x100, 0xF0, 0xF0, 0xF0, 0 };
GfxViewport HudViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0, 0, 0, 0xDAC, 0, 0xF0, 0xF0, 0xF0, 0 };
s32 ScreenSizes[10] = { 0xF00140, 0x1E00280, 0xF00140, 0x1E00280, 0x1E0, 0x168, 0x200, 0x800, 0, 0x1000000 };
GfxViewport GameViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0x15E, 0x200, 0xDFF, 0, 8, 0x30, 0x10, 0 };
s32 HubSetupFlag[1] = {0};
process_info RetryProcess = { InitRetry, UpdateGame, RenderGame, QuitGame, 0 };
camera_info_tag MainGfxCamera = {0};
camera_info_tag HudCamera = {0};
process_control_info MenuProcessControl = {0};
void (*RenderHook)(void) = 0;
s32 GameSceneFade = 0;
