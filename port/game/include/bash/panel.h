#ifndef BASH_PANEL_H
#define BASH_PANEL_H

/* Functions and data defined in src/overlays/engine/panel.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/panel.c */

#include "common.h"
#include "bash/engine.h"

extern PlaceRec HeroesOutPanel[];

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
extern DigitSrc CompletionDigitSrc[4][4];

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

void MakeSubBar(PanelPrim *p, s16 x, s16 y, u32 a, u32 b);

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

void InitPowerupIcons(s16 *arg0, s16 *arg1);


void Setup_panel(void);


void Draw_Clock(void);


extern s16 CupWinner;
extern s32 CupsDirty;


#endif /* BASH_PANEL_H */
