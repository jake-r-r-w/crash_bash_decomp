#ifndef BASH_PAUSE_H
#define BASH_PAUSE_H

/* Functions and data defined in src/overlays/engine/pause.c: PAUSE.CPP (Jampack's line
 * records; docs/PROTOTYPES.md). Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/pause.c */

#include "common.h"
#include "bash/engine.h"

/* Hook func_8008DEC8 calls with its own arguments after the hero loop. */
extern void (*D_8009E494)(s32, s32, s32, s32);
/* The part of a NAS's FUX extension func_8008DEC8 flags. */
typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} FuxFlags;
extern s32 (*D_8009D724)(s32);
extern s32 (*D_8009D738)(s32, s32);
extern s32 (*D_8009D730)(s32);
/* Hero engine scale tables, 12 steps (func_80088794). */
extern u16 D_8009DC38[];
extern u16 D_8009DC50[];
typedef struct {
    s32 a;
    u16 b;
    u16 pad;
} D_8009D6FC_t;
extern D_8009D6FC_t *D_8009D6FC;
/* Per-hero run speed rows: 3 u16 per hero_name (func_800859D4). */
extern u16 *HeroRunSpeeds;
extern s16 D_8009D864;
extern u16 D_8009D866;
extern s16 D_8009D870;
extern u16 D_8009D872;






















void func_80082590(void);

void func_800826F8(s32 arg0);


extern s16 D_8009D864;
extern u16 D_8009D866;
extern s16 D_8009D870;
extern u16 D_8009D872;




s32 func_8007EE2C(s32 pad);





void func_800831C4(s32 arg0);

void func_8008352C(void);





void func_800834E4(void);

s16 func_800831F0(void);











void func_80083550(void);

void func_80082590(void);


s32 func_80083488(void);








void func_80083244(void);

s32 func_800832BC(void);


















extern s32 D_8009D764[16][4];

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} D_8009D87C_t;

extern D_8009D87C_t AiPlayerSkill[];

/* func_80084E04: harm tables. */
extern u16 D_8009D3B8[][4];
extern u16 D_8009D400[][9];
extern u16 D_8009D4A4[];

extern s16 D_8009E0D0;
extern s16 D_8009E0D2;
extern s16 D_8009E0D4;
extern s16 D_8009E0D6;
extern s8 D_8004E0A4[];
extern s32 D_8009BD08;
extern s32 D_8009BD40;
extern s32 D_8009BD44;
extern s32 D_8009BD48;
/* GameInfo as seen by func_800826F8. */
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
extern s16 D_8009ADE6;

s32 func_8007EFC0(s32 pad, s32 max);

s32 func_8007F1A8(s32 pad, s32 max);

void func_80082C34(void);

void func_80083334(s32 arg0, s32 kind, s32 flag);











extern u8 DragonObjectsModel[];

void func_80082AC4(void);









extern s32 (*D_8009D730)(s32);


extern void (*D_8009D71C)(s32);
extern void (*D_8009D740)(s32);
extern u16 D_8009DC2C[];



/* ballistic arc state used by func_8008C5C4 */
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




/* func_800898EC: a 14-byte record per hero, 3 rows of 4 heroes. */
typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
} D_8009E0E8_t;
extern D_8009E0E8_t D_8009E0E8[3][4];
extern u8 AngelModel[];

void func_80082D90(s32 arg0);





/* func_8008900C: per weapon-timer step, an s16 triple. */
extern s16 D_8009DC68[][3];

/* func_8008CA90: one falling particle, drawn as a quad between two points */
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


/* GameInfo as seen by func_80080FBC. */
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
/* GameOptions as seen by func_80080FBC. */
typedef struct EngineOptView {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s8 unkC[4];
} EngineOptView;
#define OPTV ((EngineOptView *)GameOptions)
typedef struct EngineLevelInfo {
    /* 0x00 */ u8 unk0[0xBC];
    /* 0xBC */ u8 *unkBC;
    /* 0xC0 */ u8 *unkC0;
} EngineLevelInfo;
/* func_8008ADA4: script FX emitter state */
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
extern EngineLevelInfo *D_80059748[];
extern s16 D_8005975C[];
extern s16 D_8005981C[];
extern s16 D_800598DC[];
extern s16 D_800599B4[];
extern s16 D_80059A5C[];
extern s16 D_80059A8C[];
extern s16 D_80059ABC[];
extern s16 D_80059C24[];
extern s16 D_80059C9C[];
extern s16 D_80059D2C[];
extern s16 D_80059E4C[];
extern s16 D_8005A14C[];
extern s16 D_8005A1C4[];
extern s16 D_8005A20C[];
extern s16 D_8005A494[];
extern s16 D_8005A4DC[];
extern s16 D_8005A524[];
extern s16 D_8009AC78;
extern s16 D_8009AD20;
extern s32 D_8009AEE8;
extern s16 D_8009BD18;
extern s16 D_8009BD1C[4];
extern s32 D_8009BD30;
extern u16 D_8009BD00;
extern u16 D_8009BD2C;
extern s32 D_8009BD34;
extern s32 D_8009BD38;
extern s32 D_8009BD3C;
void func_80081470(s32 arg0);
/* GameInfo as seen by func_80081470. */
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
extern char D_8009AF48[2];
extern char D_8009AF4C[2];
extern char D_8009AF50[2];
extern char D_8009AF54[2];
extern char D_8009AF58[2];
extern s32 D_8009AF60[];
extern s32 D_8009B0C8[];
extern s32 D_8009B230[];
extern s32 D_8009B3E0[];
extern s32 D_8009B590[];
extern s32 D_8009B788[];
extern s32 D_8009B980[];
extern s32 D_8009BB30[];
/* func_800809A0: one 0x18-byte HUD script entry; the list ends at type -1. */
typedef struct HudEntry {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 idx;
    /* 0x08 */ s32 a;
    /* 0x0C */ s32 b;
    /* 0x10 */ s32 c;
    /* 0x14 */ s32 d;
} HudEntry;
/* GameInfo as seen by func_800809A0. */
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
/* D_8009BCE0 as seen by func_800809A0. */
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
/* PanelTab as seen by func_800809A0. */
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
/* GameInfo as seen by func_80084788. */
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
extern s32 D_8009D758;
/* GameInfo as seen by func_80083E8C. */
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
extern s32 D_8009D75C;
extern u16 D_8009D760;
extern u16 D_8009D6E0;
extern u16 D_8009D524[4];
/* GameInfo as seen by func_8007F314 (the pause menu). */
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
#define HUDBOX ((HudBox *)D_8009BCE0)
#define HUDENG ((HudEngView *)PanelTab)
extern s32 D_8009AF00;
extern s16 D_8009AE98[][8];
void func_800248A0();
void func_800809A0(HudEntry *e, s32 arg1);

void func_80080FBC(void);

void func_80081F64(s32 arg0);







/* func_80083708: hero setup. */
extern s32 D_8009D758;
extern s32 D_8009D75C;
extern u16 D_8009D3A4[];

/* GameInfo as seen by func_80083708. */
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


/* A particle func_8008C824 scatters (0x18 bytes). */
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

/* The texture func_8008C824 puts on its polys. */
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

extern u8 *D_80054944;


/* A script keyframe (0x50 bytes): func_80021DD4 + 0x24 starts the array (InitScriptFX, PlotScriptFX). */
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
extern s32 D_8009E194;


#endif /* BASH_PAUSE_H */
