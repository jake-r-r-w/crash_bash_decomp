#ifndef BASH_PAUSE_H
#define BASH_PAUSE_H

/* Functions and data defined in src/overlays/engine/pause.c: PAUSE.CPP (Jampack's line
 * records; docs/PROTOTYPES.md). Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/pause.c */

#include "common.h"
#include "bash/engine.h"

/* Hook HurtHeroesInRadius calls with its own arguments after the hero loop. */
/* Hero engine scale tables, 12 steps (HeroSpin). */
extern u16 HeroSquashCycle[];
extern u16 HeroSpinScale[];
/* Per-hero run speed rows: 3 u16 per hero_name (DefPreMoveHero). */
extern s16 HeroAnimHeroA;
extern u16 HeroAnimSeqA;
extern s16 HeroAnimHeroB;
extern u16 HeroAnimSeqB;






















void RenderPause(void);

void StartWinSequence(s32 arg0);






s32 PauseChooseYesNo(s32 pad);





void SetPauseY(s32 arg0);

void RequestQuit(void);





void Unpause(void);

s16 PauseRecValue(void);











void SetDefaultHeroHooks(void);



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
extern s32 WinCounterSize;
extern s32 WinCounterState;
extern s32 WinCounterAnim;
extern s32 PauseDelay;

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
    /* 0x08 */ s32 pad8;
    /* 0x0C */ s32 vy0;
    /* 0x10 */ s32 pad10;
    /* 0x14 */ s32 vy;
    /* 0x18 */ s32 floor;
    /* 0x1C */ s32 pad1C[2];
    /* 0x24 */ s32 angle;
    /* 0x28 */ s32 pad28[3];
    /* 0x34 */ s32 vx;
    /* 0x38 */ s32 vz;
    /* 0x3C */ s32 pad3C[2];
    /* 0x44 */ s32 speed;
} Arc;

s32 csqrt(s32 x);



extern u16 HealthPickupSfx[];
extern u16 HealthPickupAmounts[];




/* HeroBecomeAngel: a 14-byte record per hero, 3 rows of 4 heroes. */
typedef struct {
    u16 timer;  /* counted down */
    u16 flag;  /* tested for zero */
    u16 flags;
    s16 speed;  /* outwards, less 3 a frame */
    s16 y;
    s16 yvel;
    s16 scale;  /* grows 0x60 a frame to 0x1000 */
} AngelSparkle;
extern AngelSparkle AngelSparkles[3][4];

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


typedef struct EngineLevelInfo {
    /* 0x00 */ void *pad0[47];
    /* 0xBC */ u8 *musicText;  /* the options' music volume line */
    /* 0xC0 */ u8 *sfxText;  /* the options' sound volume line */
} EngineLevelInfo;
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
extern u16 PauseToggles[4];
extern s32 PauseStatsAllowed;
extern u16 StatsPageWanted;
extern u16 StatsPageShown;
extern s32 StatsHasPage1;
extern s32 StatsHasPage3;
extern s32 StatsHasBonus;
void DrawAdventureStats(s32 arg0);
/* Two-digit text fields (tens, units) of the score panel. */
extern char PauseDigits[4];
extern char PauseDigits2[4];
extern char PauseDigits3[4];
extern char PauseDigits4[4];
extern char PauseDigits5[4];
extern PlaceRec AdvStatsPanel0[];
extern PlaceRec AdvStatsPanel1[];
extern PlaceRec AdvStatsPanel2[];
extern PlaceRec AdvStatsPanel3[];
extern PlaceRec AdvStatsPanel4[];
extern PlaceRec AdvStatsPanel5[];
extern PlaceRec AdvStatsPanel6[];
extern PlaceRec AdvStatsPanel7[];
/* A panel rectangle, draw_panel_rect's record (PauseMenuPanel: the highlight behind the pause
 * menu's selected line, DrawHudEntries). -1 in top hides it. */
typedef struct HudBox {
    /* 0x00 */ u32 flags;     /* 0x8000 drawn */
    /* 0x04 */ s16 x;         /* 0x8000 | n: centred on n (0: the screen) */
    /* 0x06 */ s16 top;
    /* 0x08 */ s16 z;
    /* 0x0A */ s16 width;
    /* 0x0C */ s16 bottom;
    /* 0x0E */ s16 padE;
    /* 0x10 */ s32 colour;
    /* 0x14 */ s32 unk14;
} HudBox;
extern s32 HeroesInPlay;
#define HPADS ((PadState *)Pads)
extern s32 HeroesSidelined;
extern u16 FinalHeroesTempo;
extern u16 LevelKindKeys[7];
#define HUDBOX (&PauseMenuPanel)
extern s32 PauseTextHold;
extern s16 LangHudRows[][8];
void DrawSpriteTriple(s16 x, s16 y, s32 c0, s32 c1, s32 n);
void DrawHudEntries(HudEntry *e, s32 arg1);

void DrawPauseMenu(void);

void UpdatePauseSlide(s32 arg0);







/* InitialiseHeroes: hero setup. */
extern u16 HeroEntities[];



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
    u8 pad[offsetof(obj_texture, info.tpage)];
    u16 tpage, clut, uv0, uv1, uv2, uv3;
} SparkTex;

typedef struct {
    Spark *sparks;
    void *prims[2];     /* two frames' prims: SparkPoly for the engine's, Nox's SparkLine */
    s32 count;
} SparkSet;

typedef struct {
    u8 pad[offsetof(subengine, u6C)];
    SparkSet set;
} SparkObj;



/* A script keyframe (0x50 bytes): get_script_track + 0x24 starts the array (InitScriptFX, PlotScriptFX). */
typedef struct ScriptKey {
    /* 0x00 */ s32 frame;
    /* 0x04 */ s32 pad4;
    /* 0x08 */ s32 pad8;
    /* 0x0C */ s32 x, y, z;
    /* 0x18 */ s32 rx, ry, rz;
    /* 0x24 */ Quat quat;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ s32 sx, sy, sz;
    /* 0x4C */ s32 fade;
} ScriptKey;
extern s32 ScriptFxOn;


#endif /* BASH_PAUSE_H */
