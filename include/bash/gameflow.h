#ifndef BASH_GAMEFLOW_H
#define BASH_GAMEFLOW_H

/* Functions and data defined in src/overlays/engine/gameflow.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/gameflow.c */

#include "common.h"
#include "bash/engine.h"

extern s32 D_8005687C[];
s32 UpdatePause(void);
void UpdateLevel(void);
void UpdateScreenLabels(void);
extern u8 D_8005164C[];
extern GfxViewport D_8009EA74;
extern GfxViewport D_8009EA9C;
extern GfxViewport D_8009EAC4;
extern GfxViewport D_8009EAEC;
extern GfxViewport D_8009EB14;
extern GfxCamera TitlesCamera;
void RenderTitlesBackdropAndMenu(void);

typedef struct { s32 w[4]; } Blk16;
typedef struct { Blk16 a[0x18]; s32 b[3]; } Blk18C;
typedef struct { s32 w[3]; } Blk12;
typedef struct { Blk12 a; s32 b; } Blk12p4;

/* game state block at GameInfo, reset by InitGameState */
typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 unk18[8];
    u8 unk20;
    u8 unk21;
    u8 pad22[2];
    s16 unk24;
    u8 unk26;
    u8 unk27;
    u8 pad28[4];
    s32 unk2C;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    u8 unk36;
    u8 unk37;
    s16 unk38;
    s16 unk3A;
    u8 pad3C[2];
    s16 unk3E;
    u8 pad40[2];
    s16 unk42;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s8 unk4C;
    u8 unk4D[3];
    s8 unk50;
    u8 unk51[3];
    s8 unk54[4];
    s8 unk58[4];
    u8 unk5C[4];
    u8 unk60;
    u8 unk61;
    u8 pad62;
    s8 unk63;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    u8 unk74;
    u8 unk75;
    u8 unk76[0x21];
    u8 pad97[4];
    u8 unk9B[8];
    u8 padA3[4];
    u8 unkA7;
    u8 unkA8;
    u8 unkA9;
    u8 unkAA;
    u8 unkAB[0x21];
    u8 unkCC[4];
    Blk12p4 unkD0;
    s32 unkE0;
    u8 unkE4[0x21];
    u8 unk105[0x21];
    u8 unk126[0x21];
    u8 unk147[0x21];
    u8 unk168[0x21];
} GameState;

typedef struct { u8 pad[0x6C]; s32 f6C; } Sub3F0;
extern s32 D_8009EA64[];
extern s32 D_8009EA68[];
extern s32 D_8009EA6C[];
extern s32 D_8009EA70[];
s32 CountSavedWins(s32 arg0);

/* Inlined copies used by CountSavedWins. */
static inline s32 EngineTable_AllDone(EngineTable12 *t) {
    EngineEntry16Head *p;
    s32 n;
    s32 i;

    n = t->unk8;
    p = t->unk4;
    for (i = 0; i < n; i++, p++) {
        if (!LevelUnlockedIn(p->unk4, GameInfo)) {
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

static inline void EngineTable_MarkDone(void *arg0) {
    s32 i;

    for (i = 0; i < ((EngineTable12 *)arg0)->unk8; i++) {
        GameInfo[((EngineTable12 *)arg0)->unk4[i].unk4 + 0xE4] = 1;
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

    D_8009EB98 = 4;
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

    D_8009EB98 = 4;
    n = g[0x28];
    if (n > 0) {
        n--;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g[0x28] = n;
    }
}

/* CupUnlocked's body, inlined by UpdateCupMenu. */
static inline s32 EngineTable_Unlocked(s32 arg0) {
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

/* CupCurrentModeType's body, inlined by UpdateCupMenu. */
static inline s32 EngineCurWorldName(void) {
    u16 *wt = ModeTypes;
    EngineTable12 *tab = CupTable;
    u8 *g = GameInfo;
    EngineEntry16Head *e = tab[g[0x28]].unk4;
    s32 idx = e[g[0x29]].unk4;
    s32 **tbl;
    s32 w = wt[idx * 3] - 0x101;
    s32 *t;
    tbl = (s32 **)D_8004DDBC;
    t = tbl[*(s32 *)(g + 0x2C)];
    return *(s32 *)((u8 *)t + w * 4);
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

void ShowCardSlot(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 D_8009F298[];
extern void *D_8009F004[];
extern char D_8009F4A8[][5];
void ShowCardSlotStats(u8 *arg0, s32 arg1);
extern char CardStatsText[];

extern s32 D_8009F248[][5];
EXTengine *GetEngineSlot(s32 arg0);

/* GameSum147's body, inlined by ShowCardSlotStats. */
static inline s32 EngineSum147(u8 *arg0) {
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

/* GameSum126's body, inlined by ShowCardSlotStats. */
static inline s32 EngineSum126(u8 *arg0) {
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

/* GameCount105's body, inlined by ShowCardSlotStats. */
static inline s32 EngineCount105(u8 *arg0) {
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

/* GameExtra105's body, inlined by ShowCardSlotStats. */
static inline s32 EngineExtra105(u8 *arg0) {
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

/* GameSetSlot3Digits's body, inlined by ShowCardSlotStats. */
static inline void EngineSetSlot3Digits(s32 slot, s32 arg0) {
    s32 *src;
    s32 *p;
    s32 i;
    s32 *base;
    s32 *tbl;

    tbl = (s32 *)D_8009F248 + arg0 * 5;
    base = (s32 *)GetEngineSlot(slot)->sub;
    src = tbl;
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

void ShowCardSlotStats(u8 *arg0, s32 arg1);

void ShowSaveScreen(void);

void UpdateCardPrompt(void);

void UpdateSaveMenu(void);

void HandleLevelExit(void);
/* GameInfo as seen by HandleLevelExit. */
typedef struct EngineModeView {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 pad36[0x44 - 0x36];
    /* 0x44 */ s16 unk44;
    /* 0x46 */ u8 pad46[0x4C - 0x46];
    /* 0x4C */ u8 unk4C[4];
    /* 0x50 */ u8 unk50[4];
    /* 0x54 */ u8 unk54[4];
    /* 0x58 */ u8 unk58[4];
    /* 0x5C */ u8 pad5C[0x60 - 0x5C];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 pad61[0x187 - 0x61];
    /* 0x187 */ u8 unk187;
} EngineModeView;
extern s8 D_8004E0A4[];
extern process_info RetryProcess;
extern void *D_8009EFCC[];
extern void *D_8009EFD0[];
extern void *D_8009EFD4[];
extern void *D_8009EFD8[];
extern void *D_8009EFDC[];
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
extern void *D_8009EFB0[];
extern void *D_8009EFB4[];
extern void *D_8009EFB8[];
extern void *D_8009EFC0[];
extern void *D_8009EFC4[];
extern void *D_8009EFC8[];
extern void *D_8009F000[];
extern void *D_8009EFAC[];
extern void *D_8009EFE0[];
extern void *D_8009EFE4[];
extern void *D_8009EFE8[];
extern void *D_8009EFEC[];
extern void *D_8009EFF0[];
extern void *D_8009EFF4[];
extern void *D_8009EFF8[];
extern void *D_8009EFFC[];
extern void *D_8009F454[];
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
        e_->sub[1].unk6C = (tbl)[*(s32 *)((g_ = GameInfo) + 0x2C)];          \
        GetEngineSlot(1)->sub->unk6C = D_8009EFE4[*(s32 *)(g_ + 0x2C)];        \
        eng_render_subs((u8 *)GetEngineSlot(1));                                 \
    }
extern s32 D_8009F63C;
void UpdateCardSlots(void);
/* A step of the ending's messages (GotoScriptScene): two pointers and a time. */
typedef struct MsgStep {
    void *a;
    void *b;
    s32 time;
} MsgStep;
extern MsgStep D_800A0858[];
extern MsgStep D_800A0870[];
extern MsgStep D_800A0888[];

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

s32 *WarpRoomOfMode(s32 arg0);

void RenderSaveMenu(void);

s32 GameExtra105(u8 *arg0);

void QuitProcess(void);

void AwardCupTrophies(void *arg0);

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

void LoadScreenImage(void);

void UpdateExitDelay(void);

s32 CupUnlocked(s32 arg0);

void InitCardMenu(void);

void InitCardPromptMenu(void);

extern s32 D_8009F248[][5];

void UpdateCardMessage(void);

void RenderCupMenu(void);

s32 CountCupTrophies(void *arg0);

s32 WarpRoomBestLevel(void);

void RestoreHeroChoice(u8 *src);

void SaveHeroChoice(u8 *dst);

void GameSetSlot3Digits(s32 arg0);

void InitGraphics(void);

void UpdateGame(void);

typedef struct {
    void *unk0;
    void *unk4;
    void (*unk8)(s32, s32, s32, s32, s32, s32);
    void (*unkC)(s32, s32, s32, s32);   /* Papu: PapuExploRange */
} HitCallbacks;
extern HitCallbacks HitHooks;

typedef struct {
    u8 pad0[0x14];
    u16 unk14;
    s16 unk16;
} HitInfo;
typedef struct {
    s32 hit[4];
    s32 hit6[4];
    s32 other[4];
    s32 crate[4];
} HitSfxTable;
/* Intro/cutscene step script run by UpdateIntro. */
typedef struct {
    /* 0x00 */ s32 type;   /* 1: picture, 2: model */
    /* 0x04 */ s32 *pic;
    /* 0x08 */ u8 *model;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 sfx[3];
    /* 0x20 */ s32 sfxNext;
    /* 0x24 */ s32 delay;
} IntroStep;
extern IntroStep IntroSteps[];
extern Blk32 D_8009E560;
extern CamVec3 D_8009E580;
extern u8 *D_8009E5AC;
extern _sfx_handler *IntroSfx[3];
extern _sfx_handler *D_8009E5C4;
void UpdateIntro(void);
void HurtHeroesInRadius(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void PlayHitEffect(s32 *arg0, HitInfo *arg1, s32 arg2);

void UpdateCupMenu(void);

void InitGameState(GameState *arg0);

#endif /* BASH_GAMEFLOW_H */
