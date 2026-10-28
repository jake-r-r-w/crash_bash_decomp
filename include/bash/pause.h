#ifndef BASH_PAUSE_H
#define BASH_PAUSE_H

/* Functions and data defined in src/overlays/engine/pause.c: PAUSE.CPP (Jampack's line
 * records; docs/PROTOTYPES.md). Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/pause.c */

#include "common.h"
#include "bash/engine.h"

/* Hook HurtHeroesInRadius calls with its own arguments after the hero loop. */
/* The part of a NAS's FUX extension HurtHeroesInRadius flags. */
typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} FuxFlags;
/* Hero engine scale tables, 12 steps (HeroSpin). */
extern u16 HeroSquashCycle[];
extern u16 HeroSpinScale[];
typedef struct {
    s32 a;
    u16 b;
    u16 pad;
} D_8009D6FC_t;
/* Per-hero run speed rows: 3 u16 per hero_name (DefPreMoveHero). */
extern s16 HeroAnimHeroA;
extern u16 HeroAnimSeqA;
extern s16 HeroAnimHeroB;
extern u16 HeroAnimSeqB;






















void RenderPause(void);

void StartWinSequence(s32 arg0);


extern s16 HeroAnimHeroA;
extern u16 HeroAnimSeqA;
extern s16 HeroAnimHeroB;
extern u16 HeroAnimSeqB;




s32 PauseChooseYesNo(s32 pad);





void SetPauseY(s32 arg0);

void RequestQuit(void);





void Unpause(void);

s16 PauseRecValue(void);











void SetDefaultHeroHooks(void);

void RenderPause(void);


s32 PauseExitPressed(void);








void PauseRecPrev(void);

s32 PauseRecNext(void);


















extern s32 HeroOrders[16][4];



/* HeroTakeHit: harm tables. */
extern u16 HitDamage[][4];
extern u16 HitFalloff[][9];
extern u16 HitScale[];

extern s16 WinSeqTimer;
extern s16 WinSeqStep;
extern s16 WinSeqHero;
extern s16 WinSeqDelay;
extern s8 HeroSide[];
extern s32 WinCounterSize;
extern s32 WinCounterState;
extern s32 WinCounterAnim;
extern s32 PauseDelay;
/* GameInfo as seen by StartWinSequence. */
typedef struct EngineHintView {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ s32 flags;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 unk18[0x32 - 0x18];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0x5C - 0x36];
    /* 0x5C */ u8 unk5C[4];
    /* 0x60 */ u8 unk60[0x68 - 0x60];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 unk6C[0x76 - 0x6C];
    /* 0x76 */ u8 shown[1];
} EngineHintView;

s32 PauseChooseUpDown(s32 pad, s32 max);

s32 ProcessPauseUD(s32 pad, s32 max);

void AnimateWinCounter(void);

void SetPauseRecs(s32 arg0, s32 kind, s32 flag);












void RecordWin(void);











extern u16 HeroIdleSfx[];



/* ballistic arc state used by ArcUpdate */
typedef struct {
    /* 0x00 */ s32 gravity;
    /* 0x04 */ s32 started;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 vy0;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 vy;
    /* 0x18 */ s32 floor;
    /* 0x1C */ s32 unk1C[2];
    /* 0x24 */ s32 angle;
    /* 0x28 */ s32 unk28[3];
    /* 0x34 */ s32 vx;
    /* 0x38 */ s32 vz;
    /* 0x3C */ s32 unk3C[2];
    /* 0x44 */ s32 speed;
} Arc;

s32 csqrt(s32 x);



extern u16 D_8009E214[];
extern u16 D_8009E224[];




/* HeroBecomeAngel: a 14-byte record per hero, 3 rows of 4 heroes. */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
} D_8009E0E8_t;
extern D_8009E0E8_t AngelSparkles[3][4];

void UpdateWinCounter(s32 arg0);





/* HeroShoot: per weapon-timer step, an s16 triple. */
extern s16 ThrowArcs[][3];

/* PlotFallParticles: one falling particle, drawn as a quad between two points */
typedef struct {
    SVECTOR top;  /* 0x0 */
    SVECTOR bot;  /* 0x8 */
    s32 angle;    /* 0x10 */
    s32 base;     /* 0x14 */
} FallParticle;

typedef struct {
    FallParticle *parts; /* 0x0 */
    POLY_FT4 *prims[2];  /* 0x4: indexed by FrameCount & 1 */
    s32 count;           /* 0xC */
} FallParticles;


/* GameInfo as seen by DrawPauseMenu. */
typedef struct EngineMsgView {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u32 flags;
    /* 0x14 */ u8 unk14[0x26 - 0x14];
    /* 0x26 */ u8 numPlayers;
    /* 0x27 */ u8 unk27[0x2C - 0x27];
    /* 0x2C */ s32 level;
    /* 0x30 */ u8 unk30[2];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 mode;
} EngineMsgView;
#define MSGV ((EngineMsgView *)GameInfo)
typedef struct EngineLevelInfo {
    /* 0x00 */ u8 unk0[0xBC];
    /* 0xBC */ u8 *unkBC;
    /* 0xC0 */ u8 *unkC0;
} EngineLevelInfo;
/* UpdateFxEmitter: script FX emitter state */
typedef struct FxEmitter {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x18];
    /* 0x20 */ s32 life;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 live;
    /* 0x30 */ EmitPart *parts;
    /* 0x34 */ s32 *rates;
} FxEmitter;
typedef struct FxOwner {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ s32 time;
    /* 0x018 */ u8 pad18[0x3A0];
    /* 0x3B8 */ u8 *fx;
} FxOwner;
extern EngineLevelInfo *LangMessages[];
extern HudEntry PauseMenuExitArena[];
extern HudEntry PauseMenuQuit[];
extern HudEntry PauseMenuChangeArena[];
extern HudEntry PauseMenuShort[];
extern HudEntry PauseRules1[];
extern HudEntry PauseRules0[];
extern HudEntry PauseConfirmMenus[];
extern HudEntry PauseConfirmExit[];
extern HudEntry PauseOptionsMenu[];
extern HudEntry PauseSoundMenu[];
extern HudEntry PauseControllerMenu[];
extern HudEntry PauseEndMenu[];
extern HudEntry PauseEndWarpMenu[];
extern HudEntry PauseEndAdventureMenu[];
extern HudEntry PauseBlankA[];
extern HudEntry PauseBlankB[];
extern HudEntry PauseDemoLabel[];
extern s32 PauseLastChoice;
extern s16 PauseMenuY;
extern s16 PauseToggles[4];
extern s32 D_8009BD30;
extern u16 StatsPageWanted;
extern u16 StatsPageShown;
extern s32 StatsHasPage1;
extern s32 StatsHasPage3;
extern s32 StatsHasBonus;
void DrawAdventureStats(s32 arg0);
/* GameInfo as seen by DrawAdventureStats. */
typedef struct ScoreView {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x10 - 0x8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 unk14[0x32 - 0x14];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u8 unk34[0x9F - 0x34];
    /* 0x9F */ u8 bonus[4];
    /* 0xA3 */ u8 unkA3[0xCC - 0xA3];
    /* 0xCC */ u8 score[4];
    /* 0xD0 */ u8 unkD0[0xE0 - 0xD0];
    /* 0xE0 */ s32 unkE0;
    /* 0xE4 */ u8 got0[0x21];
    /* 0x105 */ u8 got2[0x21];
    /* 0x126 */ u8 got1[0x21];
    /* 0x147 */ u8 got3[0x21];
} ScoreView;
#define SCV ((ScoreView *)GameInfo)
/* Two-digit text fields (tens, units) of the score panel. */
extern char PauseDigits[2];
extern char PauseDigits2[2];
extern char PauseDigits3[2];
extern char PauseDigits4[2];
extern char PauseDigits5[2];
extern s32 D_8009AF60[];
extern s32 D_8009B0C8[];
extern s32 D_8009B230[];
extern s32 D_8009B3E0[];
extern s32 D_8009B590[];
extern s32 D_8009B788[];
extern s32 D_8009B980[];
extern s32 D_8009BB30[];
/* GameInfo as seen by DrawHudEntries. */
typedef struct HudGameView {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u32 state;
    /* 0x08 */ u8 unk8[0x10 - 0x8];
    /* 0x10 */ u32 flags;
    /* 0x14 */ u8 unk14[0x2C - 0x14];
    /* 0x2C */ s32 level;
    /* 0x30 */ u8 unk30[2];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 mode;
} HudGameView;
/* PauseMenuPanel as seen by DrawHudEntries. */
typedef struct HudBox {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} HudBox;
/* PanelTab as seen by DrawHudEntries. */
typedef struct HudEngView {
    /* 0x000 */ u8 pad0[0x31C];
    /* 0x31C */ s32 x0;
    /* 0x320 */ s32 y0;
    /* 0x324 */ u8 pad324[0x32A - 0x324];
    /* 0x32A */ u16 rot0;
    /* 0x32C */ u8 pad32C[0x36A - 0x32C];
    /* 0x36A */ s16 on0;
    /* 0x36C */ u8 pad36C[0x3B8 - 0x36C];
    /* 0x3B8 */ s32 x1;
    /* 0x3BC */ s32 y1;
    /* 0x3C0 */ u8 pad3C0[0x3C6 - 0x3C0];
    /* 0x3C6 */ u16 rot1;
    /* 0x3C8 */ u8 pad3C8[0x406 - 0x3C8];
    /* 0x406 */ s16 on1;
} HudEngView;
#define HUDG ((HudGameView *)GameInfo)
/* GameInfo as seen by DefChooseWinner. */
typedef struct WinnerView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 flags;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0x48 - 0x36];
    /* 0x48 */ s16 team0;
    /* 0x4A */ s16 team1;
    /* 0x4C */ u8 unk4C[0x5C - 0x4C];
    /* 0x5C */ u8 out[4];
    /* 0x60 */ u8 winner;
    /* 0x61 */ u8 unk61[0x6C - 0x61];
    /* 0x6C */ s32 teams;
} WinnerView;
#define WINV ((WinnerView *)GameInfo)
extern s32 HeroesInPlay;
/* GameInfo as seen by ProcessHeroes. */
typedef struct HeroLoopView {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 state;
    /* 0x08 */ u8 unk8[0x10 - 0x8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 unk14[0x30 - 0x14];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0x5C - 0x36];
    /* 0x5C */ u8 out[4];
} HeroLoopView;
#define HLV ((HeroLoopView *)GameInfo)
#define HPADS ((PadButtons *)Pads)
extern s32 HeroesSidelined;
extern u16 FinalHeroesTempo;
extern u16 HeroLastHarm[4];
/* GameInfo as seen by UpdatePause (the pause menu). */
typedef struct PauseView {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 state;
    /* 0x08 */ u8 unk8[0x10 - 0x8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 unk18[0x26 - 0x18];
    /* 0x26 */ u8 numPlayers;
    /* 0x27 */ u8 unk27[0x30 - 0x27];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0x68 - 0x36];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 unk6C[0x74 - 0x6C];
    /* 0x74 */ u8 unk74;
} PauseView;
#define PSV ((PauseView *)GameInfo)
extern u16 D_8009AF38[];
#define HUDBOX ((HudBox *)PauseMenuPanel)
#define HUDENG ((HudEngView *)PanelTab)
extern s32 D_8009AF00;
extern s16 D_8009AE98[][8];
void DrawSpriteTriple();
void DrawHudEntries(HudEntry *e, s32 arg1);

void DrawPauseMenu(void);

void UpdatePauseSlide(s32 arg0);







/* InitialiseHeroes: hero setup. */
extern s32 HeroesInPlay;
extern s32 HeroesSidelined;
extern u16 HeroEntities[];

/* GameInfo as seen by InitialiseHeroes. */
typedef struct HeroSetupView {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14[0x30 - 0x14];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32[0x50 - 0x32];
    /* 0x50 */ u8 unk50[8];
    /* 0x58 */ s8 unk58[8];
} HeroSetupView;
#define HSV ((HeroSetupView *)GameInfo)


/* A particle PlotSparks scatters (0x18 bytes). */
typedef struct {
    s16 x, y, z;
    s16 pad6;
    s16 x2, y2, z2;
    s16 padE;
    s32 ang;
    s32 base_x;
} Spark;

/* A POLY_FT4 (0x28 bytes). */
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u16 uv0, clut;
    s16 x1, y1;
    u16 uv1, tpage;
    s16 x2, y2;
    u16 uv2, pad1;
    s16 x3, y3;
    u16 uv3, pad2;
} SparkPoly;

/* The texture PlotSparks puts on its polys. */
typedef struct {
    u8 pad[0x24];
    u16 tpage, clut, uv0, uv1, uv2, uv3;
} SparkTex;

typedef struct {
    Spark *sparks;
    SparkPoly *polys0;
    SparkPoly *polys1;
    s32 count;
} SparkSet;

typedef struct {
    u8 pad[0x6C];
    SparkSet set;
} SparkObj;



/* A script keyframe (0x50 bytes): get_script_track + 0x24 starts the array (InitScriptFX, PlotScriptFX). */
typedef struct ScriptKey {
    /* 0x00 */ s32 frame;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 x, y, z;
    /* 0x18 */ s32 rx, ry, rz;
    /* 0x24 */ Quat quat;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ s32 sx, sy, sz;
    /* 0x4C */ s32 fade;
} ScriptKey;
extern s32 ScriptFxOn;


#endif /* BASH_PAUSE_H */
