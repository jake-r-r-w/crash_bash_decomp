#ifndef BASH_GAMEFLOW_H
#define BASH_GAMEFLOW_H

/* Functions and data defined in src/overlays/engine/gameflow.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/gameflow.c */

#include "common.h"
#include "bash/engine.h"

extern s32 GameSceneCounts[];
s32 UpdatePause(void);
void UpdateLevel(void);
void UpdateScreenLabels(void);
extern GfxViewport CupMenuViewport;
extern GfxViewport CupViewport0;
extern GfxViewport CupViewport1;
extern GfxViewport CupViewport2;
extern GfxViewport CupViewport3;
extern camera_info_tag TitlesCamera;
void RenderTitlesBackdropAndMenu(void);



extern const char *LangUnlockWithCard[];
extern const char *LangInsertCardForUnlocked[];
extern const char *LangCheckingUnlocked[];
extern const char *LangThereAreNoMoreUnlockedLevels[];
s32 CountSavedWins(s32 arg0);

/* Inlined copies used by CountSavedWins. */
static inline s32 EngineTable_AllDone(CupDef *t) {
    CupRound *p;
    s32 n;
    s32 i;

    n = t->roundCount;
    p = t->rounds;
    for (i = 0; i < n; i++, p++) {
        if (!LevelUnlockedIn(p->mode, GameInfo)) {
            return 0;
        }
    }
    return 1;
}

static inline s32 EngineTable_Reachable(void *arg0, u8 *tab) {
    s32 i;
    s32 j;
    s32 cnt;
    s32 obj;
    u8 *p;

    i = 0;
    for (;;) {
        if (i >= *(s32 *)((u8 *)arg0 + 8)) {
            goto done;
        }
        obj = ((CupDef *)arg0)->rounds[i].mode;
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

static inline void EngineTable_MarkDone(void *arg0) {
    s32 i;

    for (i = 0; i < ((CupDef *)arg0)->roundCount; i++) {
        GameInfo[((CupDef *)arg0)->rounds[i].mode + 0xE4] = 1;
    }
}

void RenderIntro(void);

void InitIntro(void);

void AnimateCupMenu(void);

void AnimateTitlesBackdrop(void);
void MenuChainBack(void);
void MenuChainNext(void);

/* CupMenuNext's body, inlined by UpdateCupMenu. */
static inline void EngineWorldNext(void) {
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

/* CupMenuPrev's body, inlined by UpdateCupMenu. */
static inline void EngineWorldPrev(void) {
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

/* CupUnlocked's body, inlined by UpdateCupMenu. */
static inline s32 EngineTable_Unlocked(s32 arg0) {
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

/* CupCurrentModeType's body, inlined by UpdateCupMenu. */
static inline void *EngineCurWorldName(void) {
    u16 *wt = ModeTypes;
    CupDef *tab = CupTable;
    u8 *g = GameInfo;
    CupRound *e = tab[g[0x28]].rounds;
    s32 idx = e[g[0x29]].mode;
    void ***tbl;
    s32 w = wt[idx * 3] - 0x101;
    void **t;
    tbl = LangModeTypeTexts;
    t = tbl[*(s32 *)(g + 0x2C)];
    return t[w];
}

void QuitCupMenu(void);

void InitCupMenu(void);

void InitSaveMenu(void);

s32 CardSlotEmpty(s32 *arg0);

void UpdateSaveScreen(void);

void ClearEngineSlots(void);

void StartGame(void);

void RenderProcess(void);

void UpdateProcess(void);

void ResetGameState(void);

void QuitGame(void);

void ShowCardSlots(void);

void ShowCardSlot(GameState *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern PlaceRec CardSlotPanel[];
extern void *LangEmpty[];
extern char PercentText[][5];
void ShowCardSlotStats(GameState *arg0, s32 arg1);
extern char CardStatsText[];

extern s32 Slot3DigitSrc[][5];

/* GameSum147's body, inlined by ShowCardSlotStats. */
static inline s32 EngineSum147(GameState *arg0) {
    s32 n;
    s32 i;

    n = 0;
    i = 0;
    do {
        n += arg0->adventure.won[3][i];
        i++;
    } while (i < 0x21);
    return n;
}

/* GameSum126's body, inlined by ShowCardSlotStats. */
static inline s32 EngineSum126(GameState *arg0) {
    s32 n;
    s32 i;

    n = 0;
    i = 0;
    do {
        n += arg0->adventure.won[2][i];
        i++;
    } while (i < 0x21);
    return n;
}

/* GameCount105's body, inlined by ShowCardSlotStats. */
static inline s32 EngineCount105(GameState *arg0) {
    s32 n;
    s32 i;

    n = 0;
    i = 0;
    do {
        if (arg0->adventure.won[1][i] != 0) {
            n++;
        }
        i++;
    } while (i < 0x21);
    return n;
}

/* GameExtra105's body, inlined by ShowCardSlotStats. */
static inline s32 EngineExtra105(GameState *arg0) {
    u8 *p;
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 0x21; i++) {
        p = (u8 *)arg0 + i;  /* a byte walk: &adventure.won[1][i] adds an instruction */
        if (p[0x105] >= 2) {
            n += p[0x105] - 1;
        }
    }
    return n;
}

/* GameSetSlot3Digits's body, inlined by ShowCardSlotStats. */
static inline void EngineSetSlot3Digits(s32 slot, s32 arg0) {
    s32 *src;
    s32 *p;
    s32 i;
    s32 *base;
    s32 *tbl;

    tbl = (s32 *)Slot3DigitSrc + arg0 * 5;
    base = (s32 *)GetEngineSlot(slot)->sub;
    src = tbl;
    p = base + sizeof(subengine) / 4;
    i = 0;
    do {
        if (*src == -1) {
            p[0] &= ~0x8000;
            p[sizeof(subengine) / 4] &= ~0x8000;
        } else {
            p[0] |= 0x8000;
            p[sizeof(subengine) / 4] |= 0x8000;
            p[1] = *src;
            p[0x2B] = *src;
        }
        p += 0x54;
        i++;
        src++;
    } while (i < 5);
}


void ShowSaveScreen(void);

void UpdateCardPrompt(void);

void UpdateSaveMenu(void);

void HandleLevelExit(void);
extern s8 HeroSide[];
extern process_info RetryProcess;
extern void *LangMenuCheckingCard[];
extern void *LangCardUnformatted[];
extern void *LangNoCrashBashData[];
extern void *LangLoadGame[];
extern void *LangNoCard[];
/* UpdateCardSlots: move the save slot cursor to slot (n) & 3 and redraw it. */
#define SELECT_SLOT(n)                                \
    {                                                 \
        s32 i_;                                       \
        u8 *p_;                                       \
        s32 off_;                                     \
        s32 b_;                                       \
        u8 *q_;                                       \
        u8 *a_;                                       \
        i_ = (n) & 3;                                 \
        off_ = i_ * 0x194;                            \
        p_ = (u8 *)CardSlots;                        \
        b_ = *(s32 *)(off_ + p_) == 0;                \
        q_ = p_ + 8;                                  \
        a_ = q_ + off_;                               \
        CardSlotIndex = i_;                              \
        ShowCardSlotStats(a_, b_);                        \
    }
extern void *LangPressButtonToOverwrite[];
extern void *LangPressButtonToLoad[];
extern void *LangSaveOrExit[];
extern void *LangSelectAGameSave[];
extern void *LangPressButtonToExitWithoutSaving[];
extern void *LangSelectGameToLoad[];
extern void *LangPressButtonToExitWithoutLoading[];
extern void *LangConfirmOverwrite[];
extern void *LangFormatCard[];
extern void *LangSaveGame[];
extern void *LangCardFull[];
extern void *LangNoCard2[];
extern void *LangSaving[];
extern void *LangSaveCompleted[];
extern void *LangSaveFailedPressButtonToExit[];
extern void *LangFormatting[];
extern void *SaveMenuAnims[];
/* Optional callbacks used by the save menu (UpdateSaveMenu). */
typedef struct {
    /* 0x0 */ void (*unk0)(void);
    /* 0x4 */ void (*unk4)(void);
    /* 0x8 */ void (*unk8)(void);
    /* 0xC */ void (*unkC)(void);
} SaveMenuHooks;
extern SaveMenuHooks HubSaveMenuHooks;
extern s32 HubSaveChoice;
/* UpdateSaveMenu: show slot 1's menu frames from tbl for the current mode. */
#define SAVEMENU_SET_ANIM(tbl)                                                 \
    {                                                                          \
        EXTengine *e_;                                                         \
        u8 *g_;                                                                \
        e_ = GetEngineSlot(1);                                                 \
        e_->sub[1].u6C.ptr = (tbl)[*(s32 *)((g_ = GameInfo) + 0x2C)];          \
        GetEngineSlot(1)->sub->u6C.ptr = LangSaveGame[*(s32 *)(g_ + 0x2C)];        \
        eng_render_subs(GetEngineSlot(1));                                 \
    }
extern s32 CardPromptRetry;
void UpdateCardSlots(void);
extern ScriptScene LevelExitMsgsA[];
extern ScriptScene LevelExitMsgsB[];
extern ScriptScene LevelExitMsgsC[];

s32 WarpRoomMode(void);

void ReturnToWarpRoom(void);

void ExitToTitles(void);

void SetReturnGateway(void);

void CopyScreen(void);

s32 GameCount105(u8 *arg0);

void ResetWarpRooms(void);

void QuitIntro(void);

s32 WarpRoomIndex(s32 arg0);

void InitRetry(void);

s32 GameSum126(u8 *arg0);

s32 GameSum147(u8 *arg0);

void InitGameOptions(void);

void RenderGameScene(void);

void SetGameViewport(void);

s32 CupCurrentArena(void);

hub_info *WarpRoomOfMode(s32 arg0);

void RenderSaveMenu(void);

s32 GameExtra105(u8 *arg0);

void QuitProcess(void);

void AwardCupTrophies(CupDef *cup);

s32 GatewayMode(void);

void GotoProcessFade(process_info *arg0);

void GotoProcess(process_info *process, s32 speed);

void RenderGame(void);

void CupMenuNext(void);

void CupMenuPrev(void);

s32 CupCurrentModeType(void);

s32 CupNextArena(void);

void RefreshCardSlots(void);

void RestartGame(void);

void UpdateSecretLevel(void);

void InitCrateExplode(NAS *arg0);

void LoadScreenImage(file *file);

void UpdateExitDelay(void);

s32 CupUnlocked(s32 arg0);

void InitCardMenu(void);

void InitCardPromptMenu(void);


void UpdateCardMessage(void);

void RenderCupMenu(void);

s32 CountCupTrophies(CupDef *cup);

s32 WarpRoomBestLevel(void);

void RestoreHeroChoice(CardSlot *src);

void SaveHeroChoice(CardSlot *dst);

void GameSetSlot3Digits(s32 arg0);

void InitGraphics(void);

void UpdateGame(void);

/* What a game hooks into the hit code (HitHooks): its hit shape and stat tables (Bash's and Papu's
 * crates; Bash's snow arena puts its kick speeds in hit_shapes2), a tile breaker and an
 * explosion's reach. */
typedef struct {
    void *hit_shapes2;
    void *hit_shapes;
    void (*break_tile)(s32, s32, s32, s32, s32, s32);   /* Bash's space arena: BashSpaceBreakTile */
    void (*explo_range)(void *, s32, s32, s32);           /* Papu: PapuExploRange */
} HitCallbacks;
extern HitCallbacks HitHooks;

typedef struct {
    u8 pad0[offsetof(EXTobject, whatami)];
    u16 state;  /* switched on */
    s16 player;  /* the hitter, -1 for none */
} HitInfo;
/* Intro/cutscene step script run by UpdateIntro. */
typedef struct {
    /* 0x00 */ s32 type;   /* 1: picture, 2: model */
    /* 0x04 */ file *pic;
    /* 0x08 */ u8 *model;
    /* 0x0C */ s32 script;  /* play_script's script */
    /* 0x10 */ s32 music;  /* music_request's track, -1 for none */
    /* 0x14 */ s32 sfx[3];
    /* 0x20 */ s32 sfxNext;
    /* 0x24 */ s32 delay;
} IntroStep;
extern IntroStep IntroSteps[];
extern Blk32 IntroScriptRot;
extern CamVec3 IntroScriptPos;
extern script_handler *IntroScript;
extern _sfx_handler *IntroSfx[3];
extern _sfx_handler *IntroSfxHandle;
void UpdateIntro(void);
void HurtHeroesInRadius(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void PlayHitEffect(s32 *arg0, HitInfo *arg1, s32 arg2);

void UpdateCupMenu(void);

void InitGameState(GameState *arg0);

#endif /* BASH_GAMEFLOW_H */
