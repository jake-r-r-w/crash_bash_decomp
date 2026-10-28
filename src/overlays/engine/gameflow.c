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
        PlaySfx(HitSfx[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(1, arg0, (s32)&HeroModelBank[DB_CrateObjects]);
        HurtHeroesInRadius(obj, 0x280, id, 0x208, 0xA0);
        break;
    case 6:
        PlaySfx(((HitSfxTable *)HitSfx)->hit6[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(0, arg0, (s32)&HeroModelBank[DB_CrateObjects]);
        HurtHeroesInRadius(obj, 0x280, id, 0x208, 0);
        break;
    case 2:
        PlaySfx(((HitSfxTable *)HitSfx)->crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(2, arg0, (s32)&HeroModelBank[DB_CrateObjects]);
        break;
    case 3:
        PlaySfx(((HitSfxTable *)HitSfx)->crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(4, arg0, (s32)&HeroModelBank[DB_CrateObjects]);
        HurtHeroesInRadius(obj, 0x280, id, 0x203, 0xA0);
        break;
    default:
        PlaySfx(((HitSfxTable *)HitSfx)->other[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
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
        PlaySfx(((HitSfxTable *)HitSfx)->crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        SpawnModelFx(2, (u8 *)e + 0x10, &HeroModelBank[DB_CrateObjects]);
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
    if (D_800AEC58 != 0) {
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
                                   &IntroScriptRot, &IntroScriptPos, 4, (script_handler **)&D_800AEC58);
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
    gfx_set_viewport(D_8009E498, MainCamera);
    if (IntroDbase != 0) {
        DrawSceneMarks();
    }
    scripts_render();
}

void InitIntro(void) {
    IntroStepIndex = 0;
    IntroStepTimer = 0;
    D_800AEC58 = 0;
    IntroDbase = 0;
    init_scripts(0x20, 0x80);
    IntroNextSfx = -1;
    IntroSfxDelay = 0;
    subpool_init(0x200);
    cam_init((GfxCamera *)MainCamera);
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
    EngineModeView *g = (EngineModeView *)GameInfo;
    s32 msg;
    s32 res;
    u16 flags;
    u16 w;

    if (!(g->unk30 & 0x8000)) {
        return;
    }
    g->unk50[0] = g->unk4C[0];
    g->unk50[1] = g->unk4C[1];
    g->unk50[2] = g->unk4C[2];
    g->unk50[3] = g->unk4C[3];
    g->unk58[0] = g->unk54[0];
    g->unk58[1] = g->unk54[1];
    g->unk58[2] = g->unk54[2];
    g->unk58[3] = g->unk54[3];
    if (g->unk32 & 0x20) {
        PickCpuHeroes();
    }
    flags = g->unk30;
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
        if (g->unk187 != 0 && (flags & 1)) {
            if (g->unk44 == 3) {
                g->mode = 0x20;
                GotoScriptScene((s32)D_800A0888, &LevelLoadProcess);
            } else if (g->unk44 == 1) {
                g->mode = 0x25;
                GotoScriptScene((s32)D_800A0858, &LevelLoadProcess);
            } else {
                g->mode = 0x25;
                GotoScriptScene((s32)D_800A0870, &LevelLoadProcess);
            }
            return;
        }
        GameMode = WarpRoomMode();
        return;
    case 0x20:
        g->mode = 0x25;
        if (HeroSide[g->unk60] == 1) {
            msg = (s32)D_800A0858;
        } else {
            msg = (s32)D_800A0870;
        }
        GotoScriptScene(msg, &LevelLoadProcess);
        return;
    }
    g = (EngineModeView *)GameInfo;
    switch (*(u8 *)&g->unk32) {
    case 2:
        if (g->unk30 & 1) {
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
    return WarpRooms[GameWarpRoom][0];
}

void ReturnToWarpRoom(void) {
    NextGameMode = WarpRoomMode();
    GameExitFlags = 0x8008;
    HandleLevelExit();
}

void UpdateExitDelay(void) {
    u8 *g = GameInfo;

    if (*(u16 *)(g + 0x30) & 0x8000) {
        if (ProcessControl.next_process == NULL) {
            if (*(u16 *)(g + 0x24) != 0) {
                *(u16 *)(g + 0x24) -= 1;
                if (*(u16 *)(g + 0x24) != 0) {
                    return;
                }
            }
            GotoProcess(&LevelLoadProcess, 0x200);
            if (!(*(u16 *)(g + 0x30) & 0x200)) {
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
    EngineEntry16Head *p;
    EngineTable12 *base;
    EngineTable12 *t;
    subengine *sub;
    TrigEntry *trig;
    s32 n;
    s32 i;
    u32 mask;

    i = 0;
    trig = rcossin_tbl;
    base = CupTable;
    t = base + (u8)CupMenuAnim;
    n = t->unk8;
    p = t->unk4;
loop:
    if (i < n) {
        sub = GetEngineSlot(i + 3)->sub;
        sub->u6C.dbase = (void *)p->unk8;
        sub->entity = p->unkC;
        if (p->unk0 == 1 || (p->unk0 == 2 && LevelUnlockedIn(p->unk4, GameInfo) != 0)) {
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

    tab = CardSlots[0].unk8;
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
    u8 *s0;
    Sub3F0 *s1;
    s32 *tab;

    s0 = (u8 *)GetEngineSlot(0)->sub;
    s1 = (Sub3F0 *)(s0 + 0x3F0);
    if (CardStatus & 2) {
        if (CardStatus & 0x40) {
            s1->f6C = LangCheckingUnlocked[GameLanguage];
            *(s32 *)(s0 + 0x3F0) |= 0x8000;
            return;
        } else if (CardStatus & 0x10) {
            if (CountSavedWins(0) != 0) {
                s1->f6C = LangUnlockWithCard[GameLanguage];
                *(s32 *)(s0 + 0x3F0) |= 0x8000;
                if (Pads[0].pressed & 0x2000) {
                    PlaySfx(0x396, NULL, NULL, 0x1000, 0x1E00, 0);
                    CountSavedWins(1);
                }
                return;
            }
            s1->f6C = LangThereAreNoMoreUnlockedLevels[GameLanguage];
            *(s32 *)(s0 + 0x3F0) |= 0x8000;
            return;
        } else {
            goto def;
        }
    } else {
def:
        tab = LangInsertCardForUnlocked;
    }
    s1->f6C = tab[GameLanguage];
    *(s32 *)(s0 + 0x3F0) |= 0x8000;
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
    if (EngineTable_Unlocked(GameInfo[0x28]) && (Pads[0].pressed & 0x4000)) {
        g = GameInfo;
        ((EngineModeView *)g)->unk32 = (((EngineModeView *)g)->unk32 & 0xFF00) | 2;
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        g[0x29] = 0;
        NextGameMode = CupTable[g[0x28]].unk4->unk4;
        MenuChainNext();
        p = GameInfo;
        for (i = 0; i < 4; i++) {
            p[i + 0x9F] = 0;
            p[i + 0xA3] = 0;
        }
    }
    GetEngineSlot(0)->sub->u6C.dbase = (void *)EngineCurWorldName();
    if (GameInfo[0x28] != 0) {
        GetEngineSlot(0)->sub[8].flags |= 0x8000;
    } else {
        GetEngineSlot(0)->sub[8].flags &= ~0x8000;
    }
    if ((u8)CupMenuAnim < 6) {
        GetEngineSlot(0)->sub[9].flags |= 0x8000;
    } else {
        GetEngineSlot(0)->sub[9].flags &= ~0x8000;
    }
    if (EngineTable_Unlocked(GameInfo[0x28])) {
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
    eng_render_subs((u8 *)GetEngineSlot(3));
    gfx_set_viewport(&CupViewport1, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(4));
    gfx_set_viewport(&CupViewport2, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(5));
    gfx_set_viewport(&CupViewport3, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(6));
    RenderTitlesBackdropAndMenu();
    gfx_set_viewport(&CupMenuViewport, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(2));
}

void QuitCupMenu(void) {
}

s32 CountCupTrophies(void *arg0) {
    s32 i;
    s32 j;
    s32 cnt;
    s32 obj;
    u8 *p;
    u8 *tab;

    i = 0;
    tab = CardSlots[0].unk8;
    for (;;) {
        if (i >= *(s32 *)((u8 *)arg0 + 8)) {
            goto done;
        }
        obj = *(s32 *)((u8 *)(i * 16 + *(s32 *)((u8 *)arg0 + 4)) + 4);
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

void AwardCupTrophies(void *arg0) {
    s32 i;
    s32 n = *(s32 *)((u8 *)arg0 + 8);

    for (i = 0; i < n; i++) {
        u8 *e = *(u8 **)((u8 *)arg0 + 4);
        GameInfo[*(s32 *)(e + i * 16 + 4) + 0xE4] = 1;
        n = *(s32 *)((u8 *)arg0 + 8);
    }
}

void CupMenuNext(void) {
    u8 *g = GameInfo;
    s32 n;

    CupMenuCompact = 4;
    n = g[0x28];
    if (n < 6) {
        n++;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g[0x28] = n;
    }
}

void CupMenuPrev(void) {
    u8 *g = GameInfo;
    s32 n;

    CupMenuCompact = 4;
    n = g[0x28];
    if (n > 0) {
        n--;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g[0x28] = n;
    }
}

s32 CupUnlocked(s32 arg0) {
    EngineTable12 *base = CupTable;
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
        if (!LevelUnlockedIn(p->unk4, GameInfo)) {
            return 0;
        }
    }
    return 1;
}

s32 CupCurrentModeType(void) {
    u16 *wt = ModeTypes;
    EngineTable12 *tab = CupTable;
    u8 *g = GameInfo;
    EngineEntry16Head *e = tab[g[0x28]].unk4;
    s32 idx = e[g[0x29]].unk4;
    s32 **tbl;
    s32 w = wt[idx * 3] - 0x101;
    s32 *t;
    tbl = (s32 **)LangModeTypeTexts;
    t = tbl[*(s32 *)(g + 0x2C)];
    return *(s32 *)((u8 *)t + w * 4);
}

s32 CupNextArena(void) {
    if (GameInfo[0x29] < CupTable[GameInfo[0x28]].unk8 - 1) {
        GameInfo[0x29] = GameInfo[0x29] + 1;
        return CupTable[GameInfo[0x28]].unk4[GameInfo[0x29]].unk4;
    }
    return 0x2B;
}

s32 CupCurrentArena(void) {
    return CupTable[GameInfo[0x28]].unk4[GameInfo[0x29]].unk4;
}

void InitCupMenu(void) {
    CupMenuCompact = 0;
    PlaceEngineSlot(0, (s32 *)D_8009E8B4);
    PlaceEngineSlot(2, D_8009E7F4);
    PlaceEngineSlot(3, (s32 *)CupSlotPanel);
    PlaceEngineSlot(4, (s32 *)CupSlotPanel);
    PlaceEngineSlot(5, (s32 *)CupSlotPanel);
    PlaceEngineSlot(6, (s32 *)CupSlotPanel);
    CupMenuAnim = 0;
}

static inline s32 func_800900CC_find(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = WarpRooms[0]; (u32)i < 10; i++, p += 5) {
        if (*p == arg0) {
            return i;
        }
    }
    return -1;
}

s32 WarpRoomBestLevel(void) {
    s32 i;
    s32 j;
    s32 best;
    s32 *t;

    best = -1;
    i = func_800900CC_find(GameMode);
    if (i < 0) {
        t = 0;
    } else {
        t = WarpRooms[i];
    }
    if (t == 0) {
        return best;
    }
    for (j = 0; j < t[2]; j++) {
        if (((s32(*)(s32))ModeRequirementMet)(((u16 *)(t[3] + j * 8))[2]) != 0 && best < j) {
            best = j;
        }
    }
    return best;
}

static inline s32 func_800901B4_find(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = WarpRooms[0]; (u32)i < 10; i++, p += 5) {
        if (*p == arg0) {
            return i;
        }
    }
    return -1;
}

s32 *WarpRoomOfMode(s32 arg0) {
    s32 i = func_800901B4_find(arg0);

    if (i < 0) {
        return 0;
    }
    return WarpRooms[i];
}

s32 WarpRoomIndex(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = WarpRooms[0]; (u32)i < 10; i++, p += 5) {
        if (*p == arg0) {
            return i;
        }
    }
    return -1;
}

s32 GatewayMode(void) {
    EngSub90248 *p;

    if (GameGateway < 0 || (p = HubGateways[GameGateway].unk14, p->unk2 != 2)) {
        return -1;
    }
    return p->unk4;
}

void ResetWarpRooms(void) {
    s32 i;
    EngEntry *e;
    EngTable *t = (EngTable *)WarpRooms;

    for (i = 9; i != -1; i--) {
        e = t->entries;
        while (e->a != 0) {
            e->flags &= ~8;
            e++;
        }
        t++;
    }
    HubHeroRing.unk0 = 0;
    HubHeroRing.angle = 0;
    HubHeroRing.radius = 0x400;
}

void ShowCardSlot(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    EXTengine *e;
    s32 n;
    s32 d;
    s32 r;

    PlaceEngineSlot(arg1 + 4, D_8009F298);
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
    e->sub[3].entity = IconImage[arg0[0x4C]];
    e->sub[4].entity = IconImage[arg0[0x4D]];
    e->sub[5].entity = IconImage[arg0[0x4E]];
    e->sub[6].entity = IconImage[arg0[0x4F]];
    e->sub[1].u6C.dbase = arg0 + 0x18;
    e->sub[1].unk04 = 0xC;
    e->sub[1].unk08 = 2;
    n = count_completion(arg0);
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

void ShowCardSlotStats(u8 *arg0, s32 arg1) {
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
    switch (*(s32 *)(arg0 + 0xE0)) {
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
    u8 *g;
    u8 *src;
    u8 *p;

    RenderState.modelDp = ScreenFade.amount;
    switch (CardPromptStep) {
    case 0:
        pad = Pads;
        if (*(s32 *)((u8 *)pad + 0x48) & 0x1000) {
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009F63C = 1;
            GotoProcess(&RetryProcess, 0x200);
            {
                u8 *q = GameInfo;
                *(u16 *)(q + 0x30) |= 0x200;
            }
        }
        if (CardStatus & 2) {
            if (CardStatus & 0x40) {
                e = GetEngineSlot(1);
                e->sub[1].u6C.text = LangMenuCheckingCard[*(s32 *)((g = GameInfo) + 0x2C)];
                GetEngineSlot(1)->sub->u6C.text = LangLoadGame[*(s32 *)(g + 0x2C)];
                eng_render_subs((u8 *)GetEngineSlot(1));
                return;
            } else if (CardStatus & 0x10) {
                UpdateCardSlots();
                idx = CardSlotIndex * 0x194;
                p = (u8 *)CardSlots;
                if (*(s32 *)(idx + p) != 0
                    && (*(s32 *)((u8 *)pad + 0x48) & 0x4000)) {
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                    music_request(-1);
                    src = CardSlotIndex * 0x194 + p;
                    {
                        u8 *q = GameInfo;
                        q[0x4C] = src[0];
                        q[0x4D] = src[1];
                        q[0x4E] = src[2];
                        q[0x4F] = src[3];
                        q[0x5C] = src[4];
                        q[0x5D] = src[5];
                        q[0x5E] = src[6];
                        q[0x5F] = src[7];
                    }
                    *(Blk18C *)GameInfo = *(Blk18C *)(src + 8);
                    {
                        Blk12p4 *d = (Blk12p4 *)&GameOptions;
                        u8 *s = GameInfo;
                        *d = *(Blk12p4 *)(s + 0xD0);
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
                eng_render_subs((u8 *)GetEngineSlot(1));
                return;
            }
        }
        e = GetEngineSlot(1);
        {
            u8 *q;
            e->sub[1].u6C.text = LangNoCard[*(s32 *)((q = GameInfo) + 0x2C)];
            GetEngineSlot(1)->sub->u6C.text = LangLoadGame[*(s32 *)(q + 0x2C)];
        }
        eng_render_subs((u8 *)GetEngineSlot(1));
        break;
    case 1:
        if (loader_pending() == 0) {
            u8 *q = GameInfo;
            CardPromptStep = 2;
            q[0x50] = q[0x4C];
        }
        break;
    case 2:
        if (loader_pending() == 0) {
            u8 *q;
            GotoProcess(&RetryProcess, 0x200);
            q = GameInfo;
            CardPromptStep = 3;
            *(u16 *)(q + 0x30) |= 0x200;
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
                u8 *q = GameInfo;
                SaveMenuState = 4;
                *(u16 *)(q + 0x30) |= 0x200;
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
            *(u16 *)(GameInfo + 0x30) |= 0x200;
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
                D_800657C8_Slot *dst = &CardSlots[CardSlotIndex];
                u8 *q = GameInfo;
                dst->b[0] = q[0x4C];
                dst->b[1] = q[0x4D];
                dst->b[2] = q[0x4E];
                dst->b[3] = q[0x4F];
                dst->b[4] = q[0x5C];
                dst->b[5] = q[0x5D];
                dst->b[6] = q[0x5E];
                dst->b[7] = q[0x5F];
                *(Blk12p4 *)(q + 0xD0) = *(Blk12p4 *)&GameOptions;
                *(Blk18C *)dst->unk8 = *(Blk18C *)q;
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
                    u8 *q = GameInfo;
                    *(u16 *)(q + 0x30) |= 0x200;
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
        SAVEMENU_SET_ANIM(D_8009F454);
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
            u8 *q = GameInfo;
            *(u16 *)(q + 0x30) |= 0x200;
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
    eng_render_subs((u8 *)e);
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
        u8 *c = CardBuffer;
        if (*(s32 *)(c + 0x85C) != CardBufferStamp) {
            RefreshCardSlots();
            CardBufferStamp = *(s32 *)(c + 0x85C);
        }
    }
    pad = Pads;
    if (*(s32 *)((u8 *)pad + 0x48) & 0x10) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex - 2);
    }
    if (*(s32 *)((u8 *)pad + 0x48) & 0x40) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex + 2);
    }
    if (*(s32 *)((u8 *)pad + 0x48) & 0x80) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex - 1);
    }
    if (*(s32 *)((u8 *)pad + 0x48) & 0x20) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex + 1);
    }
    CardSlotIndex &= 3;
    e = GetEngineSlot(CardSlotIndex | 4);
    e2 = GetEngineSlot(2);
    e2->unk10.y = e->unk10.y;
    e2->unk10.x = e->unk10.x;
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(4));
    eng_render_subs((u8 *)GetEngineSlot(5));
    eng_render_subs((u8 *)GetEngineSlot(6));
    eng_render_subs((u8 *)GetEngineSlot(7));
    eng_render_subs((u8 *)GetEngineSlot(3));
    eng_render_subs((u8 *)GetEngineSlot(2));
    switch (SaveMenuStep) {
    case 2:
        {
            s32 idx;
            u8 *p;
            u8 *g;
            idx = CardSlotIndex * 0x194;
            p = (u8 *)CardSlots;
            if (*(s32 *)(idx + p) == 0) {
                GetEngineSlot(3)->sub[13].u6C.dbase = 0;
            } else {
                GetEngineSlot(3)->sub[13].u6C.text = LangPressButtonToLoad[GameLanguage];
            }
            GetEngineSlot(3)->sub[12].u6C.text = LangSelectGameToLoad[*(s32 *)((g = GameInfo) + 0x2C)];
            GetEngineSlot(3)->sub[14].u6C.text = LangPressButtonToExitWithoutLoading[*(s32 *)(g + 0x2C)];
        }
        break;
    case 1:
        {
            s32 idx;
            u8 *p;
            u8 *g;
            idx = CardSlotIndex * 0x194;
            p = (u8 *)CardSlots;
            if (*(s32 *)(idx + p) == 0) {
                GetEngineSlot(3)->sub[13].u6C.text = D_8009EFB8[GameLanguage];
            } else {
                GetEngineSlot(3)->sub[13].u6C.text = LangPressButtonToOverwrite[GameLanguage];
            }
            GetEngineSlot(3)->sub[12].u6C.text = LangSelectAGameSave[*(s32 *)((g = GameInfo) + 0x2C)];
            GetEngineSlot(3)->sub[14].u6C.text = LangPressButtonToExitWithoutSaving[*(s32 *)(g + 0x2C)];
        }
        break;
    }
}

void ShowCardSlots(void) {
    u8 *p = (u8 *)CardSlots;
    u8 *pad = p + 8;

    ShowCardSlot(pad, 0,*(s32 *)CardSlots == 0, 0x28, 0x1E);
    ShowCardSlot(p + 0x19C, 1, *(s32 *)(p + 0x194) == 0, 0x140, 0x1E);
    ShowCardSlot(p + 0x330, 2, *(s32 *)(p + 0x328) == 0, 0x28, 0x7C);
    ShowCardSlot(p + 0x4C4, 3, *(s32 *)(p + 0x4BC) == 0, 0x140, 0x7C);
    PlaceEngineSlot(3, &D_8009F008);
    ShowCardSlotStats(pad + CardSlotIndex * 0x194,*(s32 *)(p + CardSlotIndex * 0x194) == 0);
}

void InitSaveMenu(void) {
    subengine *sub;

    CardSlotIndex = 0;
    CardBufferStamp = 0x4D2;
    ShowSaveScreen();
    FontPage = 0;
    SaveMenuStep = 0;
    SaveMenuYesNo = 1;
    PlaceEngineSlot(8, &D_8009F40C);
    sub = GetEngineSlot(8)->sub;
    if (GameMode == 0x28) {
        sub->entity = 0x2007;
        sub->u6C.dbase = &HeroModelBank[DB_MainmenuModels];
    } else {
        sub->entity = 0x201F;
        sub->u6C.dbase = &HeroModelBank[DB_AdventureItems];
    }
    PlaceEngineSlot(1, &CardPromptPanel);
    D_8009F494 = 0;
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
    PlaceEngineSlot(8, &D_8009F40C);
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
    PlaceEngineSlot(2, &D_8009EF64);
    GetEngineSlot(2)->unk10.x = 0x28;
    GetEngineSlot(2)->unk10.y = 0x1E;
    eng = GetEngineSlot(2);
    eng->flags |= 0x10000000;
    cam_init(&SaveCamera);
    SaveCamera.unk8 = 0x5DC;
    cam_init(&SaveBackCamera);
    SaveBackCamera.unk8 = 0x400;
    SaveBackCamera.unk18 = 0x190;
    set_fade(&ScreenFade, -0x200, 0);
}

void RefreshCardSlots(void) {
    EXTengine *eng;

    ShowCardSlots();
    PlaceEngineSlot(2, &D_8009EF64);
    GetEngineSlot(2)->unk10.x = 0x28;
    GetEngineSlot(2)->unk10.y = 0x1E;
    eng = GetEngineSlot(2);
    eng->flags |= 0x10000000;
}

void RestoreHeroChoice(u8 *src) {
    u8 *g = GameInfo;

    g[0x4C] = src[0];
    g[0x4D] = src[1];
    g[0x4E] = src[2];
    g[0x4F] = src[3];
    g[0x5C] = src[4];
    g[0x5D] = src[5];
    g[0x5E] = src[6];
    g[0x5F] = src[7];
    *(Blk18C *)GameInfo = *(Blk18C *)(src + 8);
    {
        Blk12p4 *d = (Blk12p4 *)&GameOptions;
        u8 *s = GameInfo;
        *d = *(Blk12p4 *)(s + 0xD0);
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
    *(Blk16 *)(g + 0xD0) = *(Blk16 *)&GameOptions;
    *(Blk18C *)(dst + 8) = *(Blk18C *)g;
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
    PlaceEngineSlot(8, &D_8009F40C);
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
    src = (s32 *)((s32)src + (s32)D_8009F248);
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

    arg0->unk34 = 0x28;
    arg0->unk36 = 0;
    arg0->unk37 = 0;
    arg0->unk63 = -1;
    arg0->unk64 = 0;
    arg0->unk36 = 0;
    arg0->unk37 = 0;
    k = ' ';
    for (i = 7, p = (u8 *)arg0 + i; i >= 0; i--, p--) {
        p[0x18] = k;
    }
    t = 0x3C;
    do {
        j = 1;
    } while (0);
    arg0->unk32 = 0x14;
    arg0->unk3E = 0x200;
    arg0->unk20 = 0;
    arg0->unkE0 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unk38 = j;
    arg0->unk24 = 0;
    arg0->unk30 = 0;
    arg0->unk10 = 0;
    if (VideoMode == 1) {
        t = 0x32;
    }
    arg0->unk21 = t;
    arg0->unk4C = j;
    arg0->unk3A = 3;
    arg0->unk4D[0] = 3;
    arg0->unk4D[1] = 5;
    arg0->unk4D[2] = 7;
    arg0->unk44 = 1;
    arg0->unk54[0] = -1;
    arg0->unk54[1] = -1;
    arg0->unk54[2] = -1;
    arg0->unk54[3] = -1;
    arg0->unk14 = 0;
    arg0->unk68 = 0;
    arg0->unk60 = 0;
    arg0->unk61 = 0;
    arg0->unk5C[0] = 0;
    arg0->unk5C[1] = 0;
    arg0->unk5C[2] = 0;
    arg0->unk5C[3] = 0;
    arg0->unk46 = 2;
    arg0->unk48 = 0;
    arg0->unk4A = 2;
    arg0->unk50 = arg0->unk4C;
    arg0->unk51[0] = arg0->unk4D[0];
    arg0->unk51[1] = arg0->unk4D[1];
    arg0->unk51[2] = arg0->unk4D[2];
    arg0->unk58[0] = arg0->unk54[0];
    arg0->unk58[1] = arg0->unk54[1];
    arg0->unk74 = 0;
    arg0->unk75 = 0;
    arg0->unk58[2] = arg0->unk54[2];
    arg0->unk58[3] = arg0->unk54[3];
    for (i = 0; i < 0x21; i++) {
        arg0->unkAB[i] = 0;
        arg0->unk76[i] = 0;
        arg0->unkE4[i] = 0;
        arg0->unk126[i] = 0;
        arg0->unk105[i] = 0;
        arg0->unk147[i] = 0;
        arg0->unk168[i] = 0;
    }
    t = 1;
    arg0->unkA9 = 2;
    arg0->unkAA = 3;
    arg0->unk9B[0] = 0;
    arg0->unk9B[1] = 0;
    arg0->unk9B[2] = 0;
    arg0->unk9B[3] = 0;
    arg0->unkCC[0] = 0;
    arg0->unkCC[1] = 0;
    arg0->unkCC[2] = 0;
    arg0->unkCC[3] = 0;
    arg0->unk9B[4] = 0;
    arg0->unk9B[5] = 0;
    arg0->unk9B[6] = 0;
    arg0->unk9B[7] = 0;
    arg0->unkA7 = 0;
    arg0->unkA8 = t;
    arg0->unk26 = t;
    arg0->unk27 = 0;
    arg0->unk42 = -1;
    arg0->unk6C = 0;
    arg0->unk70 = 0;
    arg0->unk2C = D_8004E0D4;
    arg0->unkD0 = *(Blk12p4 *)&GameOptions;
}

void InitGraphics(void) {
    EngineCfg *cfg = &D_8009F96C;
    u8 *g;

    AllocOtBuffers((s32 *)cfg);
    ((void (*)(u8 *))ResetScreen)(VideoMode == 0 ? vmode_512_ntsc : D_8005B744);
    g = GameInfo;
    *(s32 *)(g + 4) = 0x80000000;
    vram_init(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    prims_init(cfg->pools);
    dbase_init_all();
    ResetEnginePool();
    D_8004E0D4 = 0;
    D_8009F654 = 0;
    core_process = 0;
    GameOptions.music = 0x46;
    GameOptions.sfx = 0x64;
    GameOptions.stereo = 1;
    GameOptions.vibrate[0] = 1;
    GameOptions.vibrate[1] = 1;
    GameOptions.vibrate[2] = 1;
    GameOptions.vibrate[3] = 1;
    InitGameState((GameState *)g);
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
    EngineGameView *g;

    g = (EngineGameView *)GameInfo;
    g->unk30 &= 0x7FF8;
    set_fade(&ScreenFade, -0x300, 0);
    D_8009F8B8 = 0;
    if (g->mode == 0x24 && g->unkE0 <= 0) {
        g->unkE0 = 1;
    }
    g = (EngineGameView *)GameInfo;
    if (CountTrophies((u8 *)g) >= 0x19 && g->unkE0 < 2) {
        g->unkE0 = 2;
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
        u8 *g;
        GameFrameCount++;
        g = GameInfo;
        *(s32 *)(g + 0xC) += 1;
        UpdateLevel();
        ((void (*)(void *, u8 *))map_update)(MapScene, D_8005164C);
    } else {
        RenderLevel();
    }
    MainGfxCamera.z = 0;
    MainGfxCamera.unk8 = 0x800;
    HudCamera.z = 0;
    HudCamera.unk8 = 0x40C;
    UpdateScreenLabels();
    scripts_update();
    process_control_update(&MenuProcessControl, &ScreenFade);
    UpdateExitDelay();
}

void SetReturnGateway(void) {
    GameInfo[0x63] = 0xA;
    *(s32 *)(GameInfo + 0x64) = 1;
}

void UpdateSecretLevel(void) {
    EngineGameView *a = (EngineGameView *)GameInfo;
    EngineGameView *g;

    if (a->mode == 0x24 && a->unkE0 <= 0) {
        a->unkE0 = 1;
    }
    g = (EngineGameView *)GameInfo;
    if (CountTrophies((u8 *)g) >= 0x19 && g->unkE0 < 2) {
        g->unkE0 = 2;
    }
}

void RenderGameScene(void) {
    gfx_set_viewport(&GameViewport, (GfxCamera *)MainCamera);
    RenderState.modelDp = 0x1000 - ((0x1000 - D_8009F8B8) * (0x1000 - ScreenFade.amount) / 4096);
    D_80056878 = 0;
    D_8005687C[0] = 0;
    D_8005687C[1] = 0;
    D_8005687C[2] = 0;
    scripts_render();
    RenderAllNas();
    draw_map_scene((GfxScene *)MapScene);
    gfx_set_viewport(&MainViewport, &MainGfxCamera);
    RenderState.modelDp = ScreenFade.amount;
    DrawLevelHud();
    RenderScreenLabels();
}

void SetGameViewport(void) {
    gfx_set_viewport(&GameViewport, (GfxCamera *)MainCamera);
    RenderState.modelDp = 0x1000 - ((0x1000 - D_8009F8B8) * (0x1000 - ScreenFade.amount) / 4096);
}

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */
extern u8 D_8005B69C[];
extern u8 D_8005B6CC[];
extern u8 D_8005B6FC[];
extern u8 D_8005B75C[];

s32 HitSfx[16] = {
    0x3EC, 0x3ED, 0x3EE, 0x3EF, 0x3F1, 0x3F1, 0x3F3, 0x3F4, 0x3E8, 0x3E8, 0x3EA, 0x3EB, 0x3F5,
    0x3F5, 0x3F7, 0x3F8,
};
s32 CrateAstType = 0;
s32 CrateAstType2 = 0;
u16 CrateCounts[14] = {0};
HitCallbacks HitHooks = {0};
u8 D_8009E498[40] = {
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
s32 IntroProcess[6] = { (s32)InitIntro, (s32)UpdateIntro, (s32)RenderIntro, (s32)QuitIntro, 0, 0 };
s32 IntroStepIndex = 0;
s32 IntroStepTimer = 0;
script_handler *IntroScript = 0;
s32 IntroNextSfx = 0;
s32 IntroSfxDelay = 0;
_sfx_handler *IntroSfx[3] = {0};
_sfx_handler *IntroSfxHandle = 0;
s32 WarpReturnProcess[5] = { (s32)ReturnToWarpRoom, 0, 0, 0, 0 };
s32 NextGameMode = 0;
s32 D_8009E5E0[16] = {
    1, 0, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2005, 1, 1, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2006, 1, 2, (s32)&HeroModelBank[DB_MainmenuArenaModels2],
    0x2007, 1, 3, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2008,
};
s32 D_8009E620[16] = {
    1, 0x13, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2005, 1, 0x12, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2006, 1, 0x14,
    (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2007, 1, 0x15, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2008,
};
s32 D_8009E660[16] = {
    1, 0xA, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2001, 1, 0xB, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2002, 1, 0xC,
    (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2003, 1, 0xD, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2001,
};
s32 D_8009E6A0[16] = {
    1, 0xE, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2001, 1, 0xF, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2002, 1, 0x10,
    (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2003, 1, 0x11, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2004,
};
s32 D_8009E6E0[16] = {
    2, 6, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x2009, 2, 8, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x200A, 2, 7, (s32)&HeroModelBank[DB_MainmenuArenaModels1],
    0x200B, 2, 5, (s32)&HeroModelBank[DB_MainmenuArenaModels1], 0x200C,
};
s32 D_8009E720[16] = {
    2, 0x17, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2009, 2, 0x18, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200A, 2, 0x1A,
    (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200B, 2, 0x19, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200C,
};
s32 D_8009E760[16] = {
    2, 0x1B, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200D, 2, 0x1C, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200E, 2, 0x1D,
    (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x200F, 2, 0x1E, (s32)&HeroModelBank[DB_MainmenuArenaModels2], 0x2010,
};
EngineTable12 CupTable[7] = {
    { 1, (void *)&D_8009E5E0, 4 }, { 1, (void *)&D_8009E620, 4 },
    { 1, (void *)&D_8009E660, 4 }, { 1, (void *)&D_8009E6A0, 4 },
    { 3, (void *)&D_8009E6E0, 4 }, { 3, (void *)&D_8009E720, 4 },
    { 3, (void *)&D_8009E760, 4 },
};
u8 D_8009E7F4[180] = {
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
EngRec24 D_8009E8B4[12] = {
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
EngRec24 CupSlotPanel[2] = {
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
s32 D_8009EB9C[8] = { 0x20001, 0x3000A, 0x20002, 0x30013, 0x20003, 0x3000B, 0, 0 };
s32 D_8009EBBC[8] = { 0x2010, 0x22, 0x2012, 0x23, 0x2014, 0x24, 0x2016, 0x25 };
s32 D_8009EBDC[16] = {
    0x20001, 0x3000E, 0x20002, 0x30013, 0x20003, 0x3000A, 0x20004, 0x30000, 0x20005, 0x30004,
    0x5000B, 0x30000, 0x4000C, 0x30000, 0, 0,
};
s32 D_8009EC1C[8] = { 0x200F, 0x21, 0x2013, 0x23, 0x2015, 0x24, 0x2017, 0x25 };
s32 D_8009EC3C[18] = {
    0x20001, 0x3000F, 0x20002, 0x30012, 0x20003, 0x3000B, 0x20004, 0x30001, 0x20005, 0x30006,
    0x20006, 0x30016, 0x5000B, 0x30000, 0x4000C, 0x30000, 0, 0,
};
s32 D_8009EC84[8] = { 0x200E, 0x21, 0x2010, 0x22, 0x2014, 0x24, 0x2016, 0x25 };
s32 D_8009ECA4[20] = {
    0x20001, 0x30010, 0x20002, 0x30014, 0x20003, 0x3000C, 0x20004, 0x30002, 0x20005, 0x30008,
    0x20006, 0x30017, 0x20007, 0x30009, 0x5000B, 0x30000, 0x4000C, 0x30000, 0, 0,
};
s32 D_8009ECF4[8] = { 0x200F, 0x21, 0x2011, 0x22, 0x2013, 0x23, 0x2017, 0x25 };
s32 D_8009ED14[22] = {
    0x20001, 0x30011, 0x20002, 0x30015, 0x20003, 0x3000D, 0x20004, 0x30003, 0x20005, 0x30007,
    0x20006, 0x30018, 0x20007, 0x3001B, 0x20008, 0x3001F, 0x5000B, 0x30000, 0x4000C, 0x30000,
    0, 0,
};
s32 D_8009ED6C[8] = { 0x200E, 0x21, 0x2010, 0x22, 0x2012, 0x23, 0x2014, 0x24 };
s32 D_8009ED8C[18] = {
    0x20001, 0x30019, 0x20002, 0x3001C, 0x20003, 0x3001D, 0x20004, 0x30005, 0x20005, 0x3001E,
    0x20006, 0x3001A, 0x5000B, 0x30000, 0x4000C, 0x30000, 0, 0,
};
s32 WarpRooms[10][5] = {
    { 0x21, (s32)&D_8009EBDC, 4, (s32)&D_8009EBBC, 0x5021 },
    { 0x22, (s32)&D_8009EC3C, 4, (s32)&D_8009EC1C, 0x502D },
    { 0x23, (s32)&D_8009ECA4, 4, (s32)&D_8009EC84, 0x502E },
    { 0x24, (s32)&D_8009ED14, 4, (s32)&D_8009ECF4, 0x502A },
    { 0x25, (s32)&D_8009ED8C, 4, (s32)&D_8009ED6C, 0x502D },
    { 0x21, (s32)&D_8009EBDC, 0, 0, 0 }, { 0x21, (s32)&D_8009EBDC, 0, 0, 0 },
    { 0x21, (s32)&D_8009EBDC, 0, 0, 0 }, { 0x26, (s32)&D_8009EB9C, 0, 0, 0 },
    { 0x27, (s32)&D_8009EB9C, 0, 0, 0 },
};
HubRing HubHeroRing = {0};
s32 CardPromptPanel = 3;
s32 D_8009EEB4[44] = {
    0x328000, 0, 0, 0, 0, 0, 0, 0, 3, 0x788000, 0, 0, 0, 0, 0, 0, 0, 4, 0x1B88000, 0x19A0072,
    0, 0, 0, 0, 0, 0, 4, 0x2808000, 0x5A0028, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_8009EF64 = 4;
s32 D_8009EF68[17] = { 0x1180000, 0x5A0000, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
void *LangConfirmOverwrite[1] = { (void *)&MenuConfirmOverwrite };
void *LangPressButtonToOverwrite[1] = { (void *)&MenuPressButtonToOverwrite };
void *LangPressButtonToLoad[1] = { (void *)&MenuPressButtonToLoad };
void *D_8009EFB8[2] = { (void *)&MenuPressButtonToSave, (void *)&MenuPressButtonToExitWithoutSaving };
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
s32 D_8009F008 = 4;
s32 D_8009F00C[143] = {
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
s32 D_8009F248[4][5] = {
    { 0xB4, 0x118, 0x17C, -1, -1 }, { 0x82, 0xE6, 0x14A, 0x1AE, -1 },
    { 0x50, 0xB4, 0x118, 0x17C, 0x1E0 }, { -1, -1, -1, -1, -1 },
};
s32 D_8009F298[72] = {
    4, 0x1180000, 0x5A0000, 6, 0, 0, 0, 0, 0, 3, 0x2000C, 0, 0, 0, (s32)&MenuNameWidth, 0, 0,
    0, 3, 0x200BC, 0, 0, 0, (s32)&Menu200Percent, 0, 0, 0, 6, 0x24000C, 0x10000000, 0, 0, 0, 0,
    0x3003001C, 0, 6, 0x240048, 0x10000000, 0, 0, 0, 0, 0x3003001C, 0, 6, 0x240084, 0x10000000,
    0, 0, 0, 0, 0x3003001C, 0, 6, 0x2400C0, 0x10000000, 0, 0, 0, 0, 0x3003001C, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};
s32 CardSlotIndex = 0;
GfxViewport SaveViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0, 0, 0, 0xDFF, 0, 0, 0, 0x10, 1 };
GfxViewport SaveBackViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0xF0, 0x200, 0xD00, -0x80, 0, 0, 0, 1 };
s32 D_8009F40C = 6;
s32 D_8009F410[17] = { 0xFF9C, 0x100004C8, 0, 0, 0, 0, 0x200700C7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
void *D_8009F454[6] = {
    (void *)&MenuCardCorrupt, (void *)InitCardMenu, (void *)UpdateSaveScreen,
    (void *)RenderSaveMenu, (void *)ClearEngineSlots, 0,
};
process_info SaveScreenProcess = { InitSaveMenu, UpdateSaveScreen, RenderSaveMenu, ClearEngineSlots, 0 };
s32 SaveScreenProcess2[5] = { (s32)InitCardPromptMenu, (s32)UpdateSaveScreen, (s32)RenderSaveMenu, (s32)ClearEngineSlots, 0 };
s32 D_8009F494 = 0;
/* the results screen's number texts, one buffer: +0, +3, +6, +9 three digits each, +12 the last */
char CardStatsText[16] = {0};
char PercentText[4][5] = {0};
s32 CardBufferStamp = 0;
GfxCamera SaveCamera = {0};
GfxCamera SaveBackCamera = {0};
s32 SaveMenuStep = 0;
s32 SaveMenuYesNo = 0;
s32 CardPromptStep = 0;
s32 D_8009F63C = 0;
s32 SaveMenuState = 0;
fade_info ScreenFade = {0};
s32 D_8009F654 = 0;
process_control_info ProcessControl = {0};
s32 D_8009F668[5] = { (s32)&D_8005B75C, (s32)&vmode_512_ntsc, (s32)&D_8005B6FC, (s32)&D_8005B6CC, (s32)&D_8005B69C };
GfxViewport MainViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0, 0, 0, 0x200, -0x100, 0xF0, 0xF0, 0xF0, 0 };
GfxViewport HudViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0, 0, 0, 0xDAC, 0, 0xF0, 0xF0, 0xF0, 0 };
s32 D_8009F6CC[10] = { 0xF00140, 0x1E00280, 0xF00140, 0x1E00280, 0x1E0, 0x168, 0x200, 0x800, 0, 0x1000000 };
GfxViewport GameViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0x15E, 0x200, 0xDFF, 0, 8, 0x30, 0x10, 0 };
s32 D_8009F71C[1] = {0};
process_info RetryProcess = { InitRetry, UpdateGame, RenderGame, QuitGame, 0 };
GfxCamera MainGfxCamera = {0};
GfxCamera HudCamera = {0};
process_control_info MenuProcessControl = {0};
void (*RenderHook)(void) = 0;
s32 D_8009F8B8 = 0;
