#ifndef BASH_PANEL_H
#define BASH_PANEL_H

/* Functions and data defined in src/overlays/engine/panel.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/panel.c */

#include "common.h"
#include "bash/engine.h"

extern s32 HeroesOutPanel[];

typedef struct DigitEnt {
    u16 cmd;
    u16 arg;
    s32 flags;
} DigitEnt;

typedef struct DigitSrc {
    u16 v;
    u16 pad;
} DigitSrc;

extern DigitEnt CompletionDigits[4];
extern DigitSrc CompletionDigitSrc[];

void CheckHeroesAllOut(void);

void PanelCountCompletion(void);

s32 CalcElapsedTime(void);

void StartPanelCountdown(void);

void SetPanelHeroIconB(u16 arg0, s16 arg1);

void Clock_Off(void);

void InitTrophyCups(void);

s32 StillTottingUpScore(void);

void InitScore2Icons(void);

void InitScore1Icons(void);

void InitCompletionCounter(void);

void MakeSubBar(EngineBox *p, s16 x, s16 y, u32 a, u32 b);

void Init_Clock(s32 arg0, s32 arg1, s32 arg2);

void InitScore3Icons(void);

void Set_Clock(u16 time, u16 flags);

void InitSharedScoreIcons(void);

void Init_Panel(void);

void InitBossHealthBar(void);

void SetPanelHeroIconA(u16 arg0, s16 arg1, s16 arg2);

/* PANEL.CPP's engines: Init_Panel sets up all seven, 0x9C bytes each, which
 * run to Clock_XYZ (Jampack records five of its smaller ones). The icons use
 * 0..3, the countdown 4 (InitCountdown); 5 and 6 are the two InitBossHealthBar
 * builds, which RenderPause draws. */
extern EXTengine PanelTab[7];
extern subengine IconParts[4][7];
extern u16 PanelHeroOrder[4];
extern s16 IconPos4[][2];
extern s16 IconPos2[][2];
extern u16 IconImage[];
typedef struct { s16 x, y; } EngXY;
extern EngXY PanelIconOffset[4];

void InitIcons4(void);

extern subengine *countdown_eng;
extern subengine counter_sub;
extern subengine PauseVolSubA;
extern subengine PauseVolSubB;

void InitCountdown(void);
typedef struct PanelPrim {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 pad10[0x44];
    void *unk54;
    u8 pad58[0x10];
    s16 unk68, pad6A;
    union {
        void *ptr;
        struct {
            s16 unk6C, unk6E;
        } s;
    } u6C;
    s16 unk70, pad72, unk74, unk76, unk78, pad7A, unk7C, unk7E, unk80, pad82, unk84, unk86, unk88, pad8A;
    s32 unk8C, unk90, unk94, unk98;
    u8 pad9C[0xC];
} PanelPrim; /* 0xA8 */
extern SWORD_XY HealthIconPos[4];
extern u16 *pHeroHealth[4];

void InitIcons2(void);

void InitPowerIcons(void);

void UpdatePanel(void);
void UpdateBigScore(void);
void UpdateHealthIcons(void);
void UpdatePowerIcons(void);
void UpdateTankWeaponIcons(void);
void UpdateScoreIcons(void);
void Cups_Update(void);
extern s32 GameCheats;
extern u16 HeroLastHarm[4];
s32 FntPrint();

void InitHealthIcons(void);

void ClearHealthIcons(void);

typedef struct { void *a; void *b; s32 *c; } EngPtr3;
extern EngPtr3 TankHudWeapons[4];

void SetupCompletionDigits(void);

void InitBossHealthIcons(void);

void InitTankWeaponIcons(void);

void UpdateOutMessage(void);

s16 FindTeamLeader(void);

s32 PlaceFlashSub(s32 *p, s32 a1, s32 a2, s32 a3, s32 a4);
extern char *left_score;
extern char *right_score;
extern EXTengine BigScore1;
extern EXTengine BigScore2;
extern subengine BigScoreParts1;
extern subengine BigScoreParts2;
extern u16 BigScore_scale;
extern u16 BigScore_teamA;
extern u16 BigScore_teamB;
extern u16 BigScore_Adelay;
extern u16 BigScore_Bdelay;

void InitBigScore(void);

void InitPowerupIcons(s32 arg0, s32 arg1);

/* GameInfo as seen by Setup_panel. */
typedef struct EngineSlotView {
    /* 0x00 */ u8 unk0[0x48];
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ u8 unk4C[0x58 - 0x4C];
    /* 0x58 */ s8 unk58[0x14];
    /* 0x6C */ s32 unk6C;
} EngineSlotView;

void Setup_panel(void);

/* GameInfo as seen by Draw_Clock. */
typedef struct EngineClockView {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x10 - 0x08];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14[0x21 - 0x14];
    /* 0x21 */ s8 unk21;
} EngineClockView;

void Draw_Clock(void);

/* GameInfo as seen by Cups_Update. */
typedef struct EngModeD9DC {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x10 - 0x08];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 unk18[0x30 - 0x18];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 unk36[0x42 - 0x36];
    /* 0x42 */ s16 unk42;
    /* 0x44 */ u8 unk44[0x5C - 0x44];
    /* 0x5C */ u8 unk5C[4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61[0x6C - 0x61];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70[0x9F - 0x70];
    /* 0x9F */ u8 unk9F[4];
    /* 0xA3 */ u8 unkA3[0xA7 - 0xA3];
    /* 0xA7 */ u8 unkA7[4];
    /* 0xAB */ u8 unkAB[0xCC - 0xAB];
    /* 0xCC */ u8 unkCC[0xE4 - 0xCC];
    /* 0xE4 */ u8 unkE4[0x105 - 0xE4];
    /* 0x105 */ u8 unk105[0x126 - 0x105];
    /* 0x126 */ u8 unk126[0x147 - 0x126];
    /* 0x147 */ u8 unk147[0x168 - 0x147];
    /* 0x168 */ u8 unk168[0x21];
} EngModeD9DC;
#define MODEV ((EngModeD9DC *)GameInfo)

extern s16 CupWinner;
extern u16 D_8009AF38[7];
extern s32 CupsDirty;


#endif /* BASH_PANEL_H */
