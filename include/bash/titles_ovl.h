#ifndef BASH_TITLES_OVL_H
#define BASH_TITLES_OVL_H

/* Functions and data defined in the titles overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/titles/titles.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/levelload.h" /* PlaceRec: the engine's screen placement records */

extern u8 D_8004E568[];
extern void (*D_8009E58C)(void);
extern PlaceRec PlayersMenuPlace[];
extern PlaceRec PlayersMenuAdventurePlace[];
void OptionsQuit(void);
void OptionsInit(void);
void MenuChainStart(process_info **chain);
void MenuChainStep(s32 n);
void BattleOptionsQuit(void);
void BattleOptionsInit(void);
void TitlesInitLevel(void);
void TitlesEmptyHook(void);
void TitlesUpdateLevel(void);
void TitlesEmptyStub(void);
void TeamModeUpdate(void);
void team_mode_init(void);
void TitlesShellScreenRender(void);
void TitlesShellScreenUpdate(void);
void TitlesShellScreenInit(void);
void TitlesExtraScreenRender(void);
void TitlesExtraScreenInit(void);

typedef struct TitlesSub {
    /* 0x000 */ u8 pad0[0x274];
    /* 0x274 */ s32 a;
    /* 0x278 */ u8 pad278[0x31C - 0x278];
    /* 0x31C */ s32 b;
} TitlesSub;

extern process_info D_8009F480;
extern GfxViewport TitlesMenuView;
extern s32 MainMenuCursor;
extern s32 NewLoadMenuCursor;
extern PlaceRec NewLoadMenuPlace[];
extern s32 MainMenuHintTimer;
extern s8 AdventureHeadsSolo[];
extern s8 AdventureHeadsTeam[];
extern PlaceRec MainMenuPlace[];
extern u8 D_800A0840[];
extern process_info D_8009E5C8;
extern u16 TitlesDemoModes[];
void func_80094D88(s32 arg0, process_info *arg1);
extern PlaceRec TitlesExtraPlace[];
extern GfxViewport TitlesMenuView;
extern GfxCamera CharSelectCamera;
extern process_info **MenuChainPos;
extern PlaceRec BattleOptionsPlace[];
extern s32 BattleOptionsState;
extern PlaceRec OptionsPlace[];
extern s32 OptionsCursor;
extern s32 OptionsPage;
/* GameOptions as OptionsUpdate edits it (options menu). */
typedef struct TitlesOpt {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 unkC[4];
} TitlesOpt;
extern PlaceRec OptionsSoundPlace[];
extern PlaceRec OptionsControllerPlace[];
extern PlaceRec TeamModePlace[];
extern s32 TeamModeRow;
extern s32 TeamModePreset;
/* each screen of the front end is a process (process_info); the menu chains list them */
extern process_info PlayersMenu;
extern process_info NewLoadMenu;
extern process_info TitlesLeave;
extern process_info BattleOptions;
extern process_info CharSelect;
extern process_info LevelSelect;
extern process_info TeamMode;

void num_players_render(void);

void CharSelectRender(void);
/* 16-byte slot record of an BattleArena list (LevelSelectUpdate). */
typedef struct {
    s32 type;  /* 0x0: 1 open from the start, 2 unlocked by GameInfo[0xE4/0x168 + idx] */
    s32 idx;   /* 0x4: the game mode (GameMode) */
    void *model; /* 0x8 */
    s32 unkC;  /* 0xC: the model's anim */
} BattleLevel;

/* One battle arena of the level select (BattleArenas, by GameInfo[0x36]): unk0 its
 * BattleLevel list, unk4 how many. */
typedef struct {
    s32 unk0;
    s32 unk4;
} BattleArena;

void LevelSelectInput(void);

extern BattleArena BattleArenas[];
extern s32 LangPressToUnlockLevels[];
extern s32 LangNoMoreUnlocked[];
extern s32 LangInsertCard[];
extern s32 LangLocked[];
extern s32 LangCheckingCard[];

/* Inline forms of the slot getters LevelListModel / LevelListAnim / LevelListText. */
static inline s32 TitlesSlotModelI(s32 a, s32 b) {
    return *(s32 *)(BattleArenas[a].unk0 + (b << 4) + 8);
}

static inline s32 TitlesSlotCI(s32 a, s32 b) {
    return *(s32 *)(BattleArenas[a].unk0 + (b << 4) + 0xC);
}

extern s32 *D_8004DD70[];

static inline s32 TitlesSlotRowI(s32 a, s32 b) {
    s32 base = BattleArenas[a].unk0;
    s32 idx = *(s32 *)((u8 *)base + (b << 4) + 4);
    s32 *row = D_8004DD70[*(s32 *)(GameInfo + 0x2C)];
    return row[idx];
}

/* LevelSelectInput: is the selected slot of BattleArenas available? */
static inline s32 TitlesSlotOn(void) {
    u8 *g = GameInfo;
    BattleLevel *slot;
    s32 type;
    s32 idx;

    slot = (BattleLevel *)((u8 *)BattleArenas[(s8)g[0x36]].unk0 + ((s8)g[0x37] << 4));
    idx = slot->idx;
    type = slot->type;
    if ((*(s32 *)(g + 4) & 0x2000) || type == 1
        || (type == 2 && (g[idx + 0xE4] != 0 || g[idx + 0x168] != 0))) {
        return 1;
    }
    return 0;
}

void TitlesCheckIdle(void);
void AnimateTitlesBackdrop(void);
void MenuChainStart(process_info **chain);

extern s32 MainMenuHints[];
extern process_info OptionsScreen;

extern PlaceRec TitlesBackdropPlace[];
extern process_info MainMenu;
extern process_info **BattleChainResume;
extern process_info **TournamentChainResume;
extern Ext400 TitlesBackdropAnimator;
extern Ext400 TitlesBackdropAtable;

/* A character select card (CharCards, one per hero, 0x28 bytes). */
typedef struct CharCard {
    /* 0x00 */ s32 flags;     /* bit 0: taken */
    /* 0x04 */ u8 hero;       /* hero_name; CpuPickCharacter reads the word */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ s32 x;
    /* 0x0C */ s32 y;
    /* 0x10 */ s32 target;
    /* 0x14 */ s32 model;     /* index into D_80052A54 */
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 anim;      /* the selector's Atable when picked: a CharSelectAnim* */
    /* 0x20 */ s32 sfx;
    /* 0x24 */ u8 *sub;       /* set by CharSelectInit */
} CharCard;

/* the selectors' animations, by hero: a step list and a record pointing at it */
extern s32 CharSelectSteps0[], CharSelectSteps1[], CharSelectSteps2[], CharSelectSteps3[];
extern s32 CharSelectSteps4[], CharSelectSteps5[], CharSelectSteps6[], CharSelectAnim7[];
extern CharCard CharCards[];
extern PlaceRec CharSelectPlace[];
extern PlaceRec CharSelectModelsPlace[];
extern PlaceRec LevelSelectPlace0[];
extern PlaceRec LevelSelectPlace1[];
extern PlaceRec LevelSelectPlace2[];
extern PlaceRec LevelSelectPlace3[];
extern PlaceRec LevelSelectPlace4[];
extern PlaceRec LevelSelectPlace[];
extern s32 LevelSelectBusy;
extern s32 LevelSelectReload;
typedef struct TitlesObj {
    s32 unk0;
    s32 unk4;
    subengine *unk8;
    subengine *unkC;
    subengine *unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    Ext400 ext;
} TitlesObj;

void InitCharSelector(s32 arg0);
void ShuffleCharSelectors(void);
typedef struct {
    s32 pad0[1];
    s32 unk4;
    s32 unk8;
} TitlesDst;
typedef struct {
    s32 pad0[3];
    TitlesDst *dst;
    s32 pad10[8];
    s32 idx;
    s32 pad34[11];
} TitlesSlot;
typedef struct {
    s32 x;
    s32 y;
} TitlesOff;
extern TitlesSlot CharSelectors[];
/* UpdateCharSelector: speed toward tgt, +-lim (+-lim+900 when reversing), 0 when near. */
static inline s32 TitlesSteer(s32 pos, s32 tgt, s32 vx, s32 lim) {
    s32 v;
    s32 a;
    s32 d;

    v = vx;
    if (pos != tgt) {
        if (pos < tgt) {
            if (vx < 0) {
                v = vx + (lim + 900);
            } else {
                v = vx + lim;
            }
        } else {
            v = vx - (lim + 900);
            if (vx <= 0) {
                v = vx - lim;
            }
        }
    }
    a = v;
    if (a < 0) {
        a = -a;
    }
    if (a <= lim) {
        d = tgt - pos;
        if (d < 0) {
            d = -d;
        }
        if (d <= lim) {
            v = 0;
        }
    }
    return v;
}
/* TrigTable read bytewise: hi is the sine's high byte. */
typedef struct {
    s16 unk0;
    s8 lo;
    s8 hi;
} TitlesTrig;
extern s32 CharPickSfx[];
extern s32 CharMoveSfx[];
extern u8 D_80052A54[];
extern TitlesOff CharSelectorOffsets[];
/* GameInfo as seen by ShuffleCharSelectors (unk26 is D_8005A63A). */
typedef struct {
    u8 pad0[0x26];
    u8 unk26;
} TitlesModeView;

void SetupTitlesBackdrop(void);

void TitlesPreInitLevel(void);

int ApproachSpeed(int a0, int a1, int a2, int a3, int a4);

s32 LevelListCount(s32 arg0);

extern s32 *D_8004DD70[];

s32 LevelListText(s32 a, s32 b);

void MoveTeamBar(s32 arg0, s32 arg1);
/* OptionsRender */
#include "bash/main.h"
void func_8008D488(u8 *arg0, s32 arg1);
extern GfxCamera TitlesCamera;
extern GfxViewport TitlesMenuView;
extern GfxViewport TitlesBackdropView;
extern GfxViewport LevelSelectView0;
extern GfxViewport LevelSelectView1;
extern GfxViewport LevelSelectView2;
extern GfxViewport LevelSelectView3;
extern GfxViewport LevelSelectView4;

void OptionsRender(void);

void RunGameQuit(void);

void RunGameRender(void);

void RunGameUpdate(void);

void TitlesLeaveRender(void);

void NewLoadMenuInit(void);

void MainMenuInit(void);

void RenderTitlesBackdrop(void);

void TitlesCamFromPath(u8 *cam, s32 arg1);

void TitlesSetUp(void);

void CharSelectInit(void);

s32 IsLevelUnlocked(s32 arg0);

s32 LevelListName(s32 arg0, s32 arg1);

s32 LevelListModel(s32 arg0, s32 arg1);

extern void (*D_8009E598)(void);

extern void (*D_8009E594)(void);

extern void (*D_8009E590)(void);

extern s8 D_8005A64B;

void NewLoadMenuUpdate(void);

void BattleOptionsRender(void);

void TitlesSetHeroMasks(void);

void RunGameInit(void);

void TitlesLeaveInit(void);

void PlayersMenuInit(void);

void RenderTitlesMenu(void);

void MenuConfirmOrBack(void);

s32 CpuPickCharacter(s32 a0);
void UpdateCharSelector(TitlesSlot *slot);

s8 adventure_mode_head(s32 a0);

s32 UnlockCardLevels(s32 arg0);

void LevelSelectInit(void);

void LevelSelectRender(void);

void LevelSelectQuit(void);

void LevelSelectCursor(s32 arg0);

s32 LevelListAnim(s32 a, s32 b);

s32 LevelListType(s32 a, s32 b);

s32 LevelListLevel(s32 a, s32 b);

s32 AnyCardHasLevel(s32 idx);

extern s8 D_8005A64A;

void LevelSelectUpdate(void);

s32 IsCardLevel(s32 arg0);

/* 0x4C-byte preset record of TeamModePresets, picked by TeamModePreset (TeamModeShowPreset). */
typedef struct {
    s32 hero[4];   /* 0x00: indices into GameInfo (byte at +0x4C) */
    s32 flags[3];  /* 0x10: or-ed into sub[8/9/10].flags */
    s32 val[4];    /* 0x1C: targets for sub[4..7] */
    s32 *tbl;      /* 0x2C: indexed by GameInfo+0x2C */
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
} TitlesPreset;
extern TitlesPreset TeamModePresets[];
/* MoveTeamBar(i, val) inlined: ease sub[4 + i]'s +0x70 toward val. */
#define TITLES_SETBAR(i, val)                                                  \
    {                                                                         \
        s32 v_ = (val);                                                       \
        EXTengine *e_ = GetEngineSlot(0);                                     \
        u8 *q_ = (u8 *)e_->sub + ((i) * 0xA8 + 0x2A0);                        \
        s16 r_ = func_80015284(*(s16 *)(q_ + 0x70), v_, 0x800);               \
        *(s16 *)(q_ + 0x70) = r_;                                             \
        *(s16 *)(q_ + 0x76) = r_ + 0x78;                                      \
        *(s32 *)((u8 *)e_->sub + (i) * 0xA8 + 4) = *(s16 *)(q_ + 0x70) + 0x1A; \
    }
/* GameInfo as seen by TeamModeShowPreset. */
typedef struct {
    u8 pad0[0x2C];
    s32 unk2C;     /* 0x2C */
    u8 pad30[0x48 - 0x30];
    u16 unk48;     /* 0x48 */
    u16 unk4A;     /* 0x4A */
    u8 unk4C[8];   /* 0x4C: per player */
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 pad58[0x6C - 0x58];
    s32 unk6C;     /* 0x6C */
} TitlesPresetView;
#define O69PV ((TitlesPresetView *)GameInfo)
extern u16 TeamModeHeroFrames[];

void TeamModeShowPreset(void);

/* GameInfo as seen by BattleOptionsUpdate (option screen). */
typedef struct {
    u8 pad0[0x2C];
    s32 unk2C;     /* 0x2C */
    u8 pad30[0x32 - 0x30];
    u16 unk32;     /* 0x32 */
    u8 pad34[0x38 - 0x34];
    s16 unk38;     /* 0x38 */
    u16 unk3A;     /* 0x3A */
} TitlesOptView;
#define O69OV ((TitlesOptView *)GameInfo)
extern s32 TextDifficulties[][3];
extern s32 TextBattleKinds[][2];

void MainMenuUpdate(void);

void BattleOptionsUpdate(void);

/* GameInfo as seen by PlayersMenuUpdate (player count menu). */
typedef struct {
    u8 pad0[0x26];
    u8 unk26;      /* 0x26: chosen count */
    u8 unk27;      /* 0x27: mode (D_8005A63B) */
    u8 pad28[0x32 - 0x28];
    u16 unk32;     /* 0x32 */
    u8 pad34[0x54 - 0x34];
    s8 unk54;
    s8 unk55;
    s8 unk56;
    s8 unk57;
    u8 pad58[0x5C - 0x58];
    u8 unk5C[4];   /* 0x5C */
    u8 pad60[0x6C - 0x60];
    s32 unk6C;     /* 0x6C */
} TitlesCountView;
#define O69CV ((TitlesCountView *)GameInfo)
/* PadState (Pads) as seen by PlayersMenuUpdate. */
typedef struct {
    u8 pad0[0x2A];
    s16 unk2A;     /* 0x2A */
    u8 pad2C[0x48 - 0x2C];
    s32 pressed;   /* 0x48 */
    u8 pad4C[0x58 - 0x4C];
} TitlesPad;
#define O69PAD ((TitlesPad *)Pads)
extern u8 D_8005A63B;
void PlayersMenuUpdate(void);

void OptionsUpdate(void);

void MenuChainBack(void);
void MenuChainNext(void);

void CharSelectUpdate(void);

#endif /* BASH_TITLES_OVL_H */
