#ifndef BASH_RACE_OVL_H
#define BASH_RACE_OVL_H

/* Functions and data defined in the race overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/race/race.c */

#include "common.h"
#include "bash/nas.h"

extern HeroInfo Heroes[4];

void PlotRaceSmokeBoostWater(NAS *nas);
void PlotRaceGreenSmokeBoost(NAS *nas);
void PlotRaceGreenSmokePoof(NAS *nas);
void PlotRaceSmokePoof(NAS *nas);
s32 RaceDist2(u8 *arg0, u8 *arg1);
void InitRaceTag(u8 *arg0);
s32 RaceToCell(s32 arg0);
void PlotRaceSmokeBoost(NAS *nas);
void RaceHeroWinner(s32 arg0);
s32 RaceFloorY(void);
s32 RaceAlwaysZero(void);
void FirstTriggersRACE(void);
void RaceRestartLevel(void);
s32 RacePadAngle(s32 arg0);
void Race_HeroBot(void);

extern s32 RaceFloorLevel;
extern s32 RaceTagTime;
extern ScriptInit RaceGreenSmokeBoostScriptInit;
extern ScriptInit RaceSmokeBoostScriptInit;
extern HeroInfo Heroes[4];
extern ScriptInit RaceSmokePoofScriptInit;
extern ScriptInit RaceGreenSmokePoofScriptInit;
extern s32 RaceBarrelRollTimer;
extern const char RaceMsgPlayerAngle[];

void RaceAI_SteerAhead(s32 player);
u32 RaceAI_FollowTrack(s32 player);

/* A hero's FUX as RaceAI_SteerAhead reads it: a 2D velocity at 0x13C. */
typedef struct RaceVelFux {
    /* 0x000 */ u8 pad0[0x13C];
    /* 0x13C */ s32 vx;
    /* 0x140 */ s32 vz;
} RaceVelFux;

/* A hero's FUX as RaceHeroCollide reads it: held item, timer, velocity. */
typedef struct RaceBumpFux {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ s32 item;
    /* 0x01C */ u8 pad1C[0x10];
    /* 0x02C */ s32 timer;
    /* 0x030 */ u8 pad30[0x10C];
    /* 0x13C */ s32 vx;
    /* 0x140 */ s32 vz;
} RaceBumpFux;
void RaceHeroesBump(s32 a, s32 b);

/* Extension 0x20000000 as RaceAI_Think drives it. */
typedef struct EXTraceai {
    /* 0x00 */ s32 unk0[5];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ void (*unk18)(s32 player);
} EXTraceai;
extern s32 RaceAISpeeds[];
void RaceAI_Think();
void RaceAI_SpinnerBrake(s32 arg0);
void RaceAI_AvoidBarrels(s32 arg0);
void RaceAI_AvoidObjects(s32 arg0);
s32 RaceAI_UseCloseWeapon(s32 arg0);
s32 RaceAI_UseAimedWeapon(s32 arg0);
s32 RaceAI_SteerAtVictim(s32 arg0);

void InitialiseRACEFirst();
void InitialiseRACE();
void UpdateRACE();

void PlotRaceFire(NAS *nas);

/* A value RaceEaseCamStep eases toward its target by a fixed step. */
typedef struct RaceEase {
    /* 0x00 */ s32 cur;
    /* 0x04 */ s32 target;
    /* 0x08 */ s16 step;     /* used while target != 0 */
    /* 0x0A */ s16 stepZero; /* used while target == 0 */
    /* 0x0C */ u8 padC[0x20 - 0xC];
} RaceEase;

/* The offsets RaceEaseCamStep eases and adds (>> 8) to a position. */
typedef struct RaceEaseCam {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ RaceEase x;
    /* 0x40 */ RaceEase z;
} RaceEaseCam;

/* Shield extension, AddExt(nas, 0x20, 0x10000000). */
typedef struct EXTraceshield {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14[2];
    /* 0x1C */ s32 unk1C;
} EXTraceshield;

/* A hero's FUX as RaceAI_UseCloseWeapon reads it: a mode word at 0x18. */
typedef struct RaceFux18 {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 mode;
} RaceFux18;

/* A hero's FUX as SetRaceHeroIdle clears it. */
typedef struct RaceHeroFux {
    /* 0x00 */ u8 unk0[0x8C];
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ s32 unk90;
} RaceHeroFux;

/* A hero's FUX as SetRaceHeroRun clears it: a word at 0x40. */
typedef struct RaceFux40 {
    /* 0x00 */ u8 unk0[0x40];
    /* 0x40 */ s32 unk40;
} RaceFux40;

/* A hero's FUX as SetRaceHeroJump clears it: a word at 0x138. */
typedef struct RaceFux138 {
    /* 0x000 */ u8 unk0[0x138];
    /* 0x138 */ s32 unk138;
} RaceFux138;

/* Per-hero run speed rows: 3 u16 per hero_name. */

/* A hero's FUX as RaceHeroBreatheFire reads it: a count at 0x3C. */
typedef struct RaceFuxCount {
    /* 0x00 */ u8 unk0[0x3C];
    /* 0x3C */ s32 unk3C;
} RaceFuxCount;
/* A hero's FUX as SetRaceHeroOiled fills it. */
typedef struct RaceSpinFux {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
} RaceSpinFux;

/* A hero's FUX as RaceHeroOiled counts it down: a timer at 0x24. */
typedef struct RaceFuxTimer {
    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ s32 unk24;
} RaceFuxTimer;

/* Hero FUX pending-spawn flags, as seen by RaceHeroUsePickup. */
typedef struct RaceSpawnFux {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ s32 unk18;
    /* 0x01C */ u8 pad1C[0x130 - 0x1C];
    /* 0x130 */ s32 unk130;
} RaceSpawnFux;

/* Extension 0x10000000 of the NAS spawned by RaceHeroUsePickup. */
typedef struct RaceSpawnExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} RaceSpawnExt;

s32 RaceHeroUsePickup(s32 player);

void InitRaceShield(NAS *nas);
typedef struct EXTracefire {
    /* 0x00 */ s32 unk0[4];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} EXTracefire;
/* Inner ripple extension (id 0x10000000). */
typedef struct EXTinnerripple {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 scale;
    /* 0x10 */ s32 speed;
    /* 0x14 */ s32 max;
    /* 0x18 */ s16 unk18;
} EXTinnerripple;
/* Rippled extension, AddExt(nas, 0x1C, 0x10000000). */
typedef struct EXTrippled {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 scale;
    /* 0x10 */ s32 speed;
    /* 0x14 */ s32 max;
    /* 0x18 */ s32 unk18;
} EXTrippled;
typedef struct RaceBouncer {
    s32 unk0;
    s32 vel;      /* 0x04 */
    s32 unk8;
    s32 unkC;
    s32 accel;    /* 0x10 */
    s32 unk14;
    s32 unk18;
    s32 limit;    /* 0x1C */
    s32 damp;     /* 0x20 */
    s32 flags;    /* 0x24 */
    s32 rebound;  /* 0x28 */
} RaceBouncer;

typedef struct RaceBounceObj {
    s32 unk0[5];
    s32 pos;      /* 0x14 */
} RaceBounceObj;

void RaceMovesSwap(EXTheromove *a, EXTheromove *b);

extern HeroInfo Heroes[4];
void SetHeroAction(s32 player, s32 action);
s32 PointInZone(s32 *arg0, s32 arg1);
s32 DistXZSq(u8 *arg0, u8 *arg1);

s32 RaceBounceObject(RaceBounceObj *obj, RaceBouncer *b);

s32 RaceDazeHeroNear(u8 *arg0);

void RACESetUp(void);
/* GameInfo as seen by UpdateRACE. */
typedef struct RaceGameView {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14[0x30 - 0x14];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 unk34;
} RaceGameView;
#define G63 ((RaceGameView *)GameInfo)
/* GameInfo as seen by RaceMissileHitHeroes. */
typedef struct RaceGameView2 {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x30 - 0x8];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 unk36[0x5C - 0x36];
    /* 0x5C */ u8 unk5C[4];
    /* 0x60 */ u8 unk60;
} RaceGameView2;
#define G63B ((RaceGameView2 *)GameInfo)

/* One of four random voice sfx (SetRaceHeroDazed; as SFX4 in dino.h). */
#define RACE_SFX4(a, b, c, d)                              \
    r = Random(4);                                          \
    if (r == 0) {                                           \
        PlaySfx(a, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }                                                       \
    if (r == 1) {                                           \
        PlaySfx(b, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }                                                       \
    if (r == 2) {                                           \
        PlaySfx(c, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }                                                       \
    if (r == 3) {                                           \
        PlaySfx(d, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }

void Init_Clock(s32 arg0, s32 arg1, s32 arg2);
void Set_Clock(u16 time, u16 flags);
extern s32 RacePickupTimer;
extern s32 RaceUnused1;

/* A hero's race FUX, AddExt(nas, 0x174, 0x10000000) in InitialiseRACE. */
typedef struct RaceFux {
    /* 0x000 */ s32 unk0[3];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ s32 unk18;
    /* 0x01C */ EXTengine *unk1C;
    /* 0x020 */ s32 unk20;
    /* 0x024 */ s32 unk24;
    /* 0x028 */ s32 unk28;
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ s32 unk30;
    /* 0x034 */ s32 unk34;
    /* 0x038 */ s32 unk38;
    /* 0x03C */ s32 unk3C;
    /* 0x040 */ s32 unk40;
    /* 0x044 */ s32 unk44;
    /* 0x048 */ s32 unk48;
    /* 0x04C */ s32 unk4C;
    /* 0x050 */ s32 unk50;
    /* 0x054 */ s32 unk54;
    /* 0x058 */ s32 unk58;
    /* 0x05C */ u8 pad5C[0x130 - 0x5C];
    /* 0x130 */ s32 unk130;
    /* 0x134 */ s32 unk134;
    /* 0x138 */ s32 unk138;
    /* 0x13C */ s32 unk13C;
    /* 0x140 */ s32 unk140;
    /* 0x144 */ s32 unk144;
    /* 0x148 */ s32 unk148;
    /* 0x14C */ s32 unk14C;
    /* 0x150 */ s32 unk150;
    /* 0x154 */ s32 unk154;
    /* 0x158 */ s32 unk158;
    /* 0x15C */ s32 unk15C;
    /* 0x160 */ s32 unk160;
    /* 0x164 */ s32 unk164;
    /* 0x168 */ s32 unk168;
    /* 0x16C */ s32 unk16C;
    /* 0x170 */ s32 unk170;
} RaceFux;

/* Extension 0x20000000 as InitialiseRACE sets it up. */
typedef struct RaceAiExt {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ void (*unk18)(s32 player);
    /* 0x1C */ s32 unk1C;
} RaceAiExt;

/* hero->FUX as reset by SetRaceHeroOffTrack. */
typedef struct RaceResetFux {
    /* 0x000 */ u8 unk0[0x18];
    /* 0x018 */ s32 unk18;
    /* 0x01C */ u8 unk1C[0x24 - 0x1C];
    /* 0x024 */ s32 unk24;
    /* 0x028 */ u8 unk28[0x54 - 0x28];
    /* 0x054 */ s32 unk54;
    /* 0x058 */ u8 unk58[0x60 - 0x58];
    /* 0x060 */ s32 unk60;
    /* 0x064 */ u8 unk64[0x6C - 0x64];
    /* 0x06C */ s32 unk6C;
    /* 0x070 */ u8 unk70[0x78 - 0x70];
    /* 0x078 */ s32 unk78;
    /* 0x07C */ s32 unk7C;
    /* 0x080 */ s32 unk80;
    /* 0x084 */ u8 unk84[0x88 - 0x84];
    /* 0x088 */ s32 unk88;
    /* 0x08C */ u8 unk8C[0x90 - 0x8C];
    /* 0x090 */ s32 unk90;
    /* 0x094 */ u8 unk94[0x128 - 0x94];
    /* 0x128 */ _sfx_handler *unk128;
    /* 0x12C */ u8 unk12C[0x144 - 0x12C];
    /* 0x144 */ s32 unk144;
    /* 0x148 */ u8 unk148[0x168 - 0x148];
    /* 0x168 */ s32 unk168;
} RaceResetFux;

/* hero->FUX as RaceHeroOffTrack (the dive) reads it. */
typedef struct RaceDiveFux {
    /* 0x000 */ u8 unk0[0x48];
    /* 0x048 */ s32 unk48;
    /* 0x04C */ u8 unk4C[0x54 - 0x4C];
    /* 0x054 */ s32 unk54;
    /* 0x058 */ s32 unk58;
    /* 0x05C */ RaceBouncer bounce;
    /* 0x088 */ s32 unk88;
    /* 0x08C */ s32 unk8C;
    /* 0x090 */ s32 unk90;
    /* 0x094 */ u8 unk94[0x13C - 0x94];
    /* 0x13C */ s32 unk13C;
    /* 0x140 */ s32 unk140;
    /* 0x144 */ s32 unk144;
} RaceDiveFux;

/* hero->FUX as RaceHeroIdle (the swim) reads it. */
typedef struct RaceSwimFux {
    /* 0x000 */ u8 unk0[0x24];
    /* 0x024 */ s32 unk24;
    /* 0x028 */ u8 unk28[0x48 - 0x28];
    /* 0x048 */ s32 unk48;
    /* 0x04C */ s32 unk4C;
    /* 0x050 */ u8 unk50[0x58 - 0x50];
    /* 0x058 */ s32 unk58;
    /* 0x05C */ u8 unk5C[0x8C - 0x5C];
    /* 0x08C */ s32 unk8C;
    /* 0x090 */ s32 unk90;
    /* 0x094 */ u8 unk94[0xA4 - 0x94];
    /* 0x0A4 */ SLONG_XYZ unkA4;
    /* 0x0B0 */ u8 unkB0[0x128 - 0xB0];
    /* 0x128 */ _sfx_handler *unk128;
    /* 0x12C */ u8 unk12C[0x168 - 0x12C];
    /* 0x168 */ s32 unk168;
    /* 0x16C */ s32 unk16C;
} RaceSwimFux;

extern s32 RaceTimer;
extern s32 RaceOver;

/* Extension 0x10000000 as PlotDac drives it. */
typedef struct EXTracedac {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 state;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 angle;
    /* 0x18 */ s32 timer;
    /* 0x1C */ s32 height;
    /* 0x20 */ EXTengine *shadow;
    /* 0x24 */ s32 player;
} EXTracedac;

typedef struct RaceObj74 {
    /* 0x00 */ u8 unk0[0x74];
    /* 0x74 */ s16 unk74;
} RaceObj74;
extern RaceObj74 *RaceGlowObj;
extern s32 RaceGlowPhase;
extern s32 RaceGlowMode;
extern s32 RaceStartDelay;
extern u16 CameraMode;

/* Spinning object MaintainCentre updates: rotation at 0x10, its matrix at 0x30. */
typedef struct RaceSpinObj {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ SVECTOR rot;
    /* 0x18 */ u8 unk18[0x30 - 0x18];
    /* 0x30 */ MATRIX mat;
} RaceSpinObj;
extern RaceSpinObj *RaceSpinner;
extern s32 RaceSpinDelay;
extern s32 RacePatternIdx;
s32 RacePistonPattern();

/* A step of a RacePistonPattern pattern (0xC bytes). */
typedef struct RacePatStep {
    /* 0x00 */ s32 hold;
    /* 0x04 */ s32 delay; /* -1: never */
    /* 0x08 */ s32 unk8;
} RacePatStep;

/* A RacePistonPattern pattern: its steps and its length. */
typedef struct RacePattern {
    /* 0x00 */ RacePatStep *steps;
    /* 0x04 */ s32 length;
} RacePattern;
extern RacePattern RacePistonPatterns[];

/* One of the six pistons RacePistonPattern moves (0x1C bytes). */
typedef struct RacePiston {
    /* 0x00 */ s32 pos;
    /* 0x04 */ s32 state;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 timer;
    /* 0x14 */ s32 hold;
    /* 0x18 */ s32 active;
} RacePiston;
extern RacePiston RacePistons[6];
extern s32 RacePatternTime;
extern s32 RacePistonLimit;

void MaintainCentre(void);
void RaceUpdateBarrels(void);
void RacePickupMaster(void);
void RacePickupRespawn(void);
/* PadState (Pads) as RaceHeroTestIdle reads it: d-pad bits 0xF0 at 0x50. */
typedef struct RacePad {
    /* 0x00 */ u8 unk0[0x50];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} RacePad;
/* Race missile extension (0x78 bytes, id 0x10000000). */
typedef struct RaceMissileExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x34 - 0x24];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[0x40 - 0x38];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[0x4C - 0x44];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ EXTengine *unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ SLONG_XYZ pos;
} RaceMissileExt;

/* An x/z vector as PlotRaceMissile keeps it on the stack (16 bytes). */
typedef struct RaceVec4 {
    s32 x;
    s32 y;
    s32 z;
    s32 pad;
} RaceVec4;

/* A wall segment of a scene collision object (RaceTrackCollide; dino's DinoSeg). */
typedef struct RaceSeg {
    /* 0x00 */ SLONG_XYZ p0;
    /* 0x0C */ SLONG_XYZ p1;
    /* 0x18 */ s32 len;
    /* 0x1C */ s32 ang;
    /* 0x20 */ u8 pad20[0x18];
} RaceSeg;

typedef struct RaceSegObj {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 count;
    /* 0x3C */ u8 pad3C[0xC];
    /* 0x48 */ RaceSeg segs[1];
} RaceSegObj;

/* MapScene as RaceTrackCollide reads it. */
typedef struct RaceSegScene {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ u8 *data;
} RaceSegScene;

s32 RaceTrackCollide(SLONG_XYZ *pos, RaceVec4 *out, s32 radius, s32 idx, s32 flags);

/* Heading a reflected off a wall of normal angle b (inline copy of RaceReflectAngle, PlotDinoEgg). */
static inline s32 RaceBounce(s32 a, s32 b) {
    s32 n[4];
    s32 d[4];
    s32 r[4];
    s32 dot;

    n[0] = rcossin_tbl[b & 0xFFF].unk2;
    n[2] = rcossin_tbl[(b - 0x400) & 0xFFF].unk2;
    d[0] = -rcossin_tbl[(a - 0x400) & 0xFFF].unk2;
    d[2] = -rcossin_tbl[a & 0xFFF].unk2;
    dot = (d[0] * n[0] >> 12) + (d[2] * n[2] >> 12);
    r[0] = d[0] - (dot * n[0] >> 11);
    r[2] = d[2] - (dot * n[2] >> 11);
    return get_angle(r[0], r[2]) + 0x800;
}

/* RaceBounce written out on the caller's own n/d/r arrays (PlotDinoEgg). */
#define RACE_BOUNCE(out, n, d, r, ang, wall)                   \
    {                                                           \
        s32 b_ = (wall);                                        \
        s32 a_ = (ang);                                         \
        s32 dot_;                                               \
                                                                \
        n[0] = rcossin_tbl[b_ & 0xFFF].unk2;                     \
        n[2] = rcossin_tbl[(b_ - 0x400) & 0xFFF].unk2;           \
        d[0] = -rcossin_tbl[(a_ - 0x400) & 0xFFF].unk2;          \
        d[2] = -rcossin_tbl[a_ & 0xFFF].unk2;                    \
        dot_ = (d[0] * n[0] >> 12) + (d[2] * n[2] >> 12);       \
        r[0] = d[0] - (dot_ * n[0] >> 11);                      \
        r[2] = d[2] - (dot_ * n[2] >> 11);                      \
        (out) = get_angle(r[0], r[2]) + 0x800;                  \
    }

/* First hero within reach of pos, knocked into action 0x5D, or -1 (inline copy of RaceDazeHeroNear). */
static inline s32 RaceHitHero(u8 *pos) {
    HeroInfo *hero;
    u32 i;

    i = 0;
    hero = Heroes;
loop:
    if (i < 4) {
        s32 dist;

        do {
            dist = DistXZSq(pos, (u8 *)hero->hero_eng + 0x10);
        } while (0);
        if (dist < 0x6184) {
            if (hero->hero_mode != 0x5D) {
                SetHeroAction(i, 0x5D);
            }
            return i;
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

/* out[0] = v's x/z distance from the origin, out[1] its angle (PlotRaceMissile). */
static inline void RaceToPolar(SLONG_XYZ *v, s32 *out) {
    out[0] = csqrt(v->x * v->x + v->z * v->z) >> 6;
    out[1] = get_angle(v->x, v->z) & 0xFFF;
}

/* Race pick-up extension (0x50 bytes, id 0x10000000). */
typedef struct RacePickUpExt {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x1C - 0x14];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 pad20[0x28 - 0x20];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[0x38 - 0x34];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ EXTengine *unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
} RacePickUpExt;

/* Pickup spawn slots RacePickupRespawn picks from: an angle and an in-use flag. */
typedef struct RacePickUpSlot {
    /* 0x00 */ s32 angle;
    /* 0x04 */ s32 used;
} RacePickUpSlot;
extern RacePickUpSlot RacePickupSlots[16];
extern s32 RacePickupHeroCount;
extern s32 RacePickupCount;

/* A hero's FUX as RacePickupMaster reads it: a held pick-up and its engine. */
typedef struct RaceFuxHeld {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ EXTengine *unk1C;
} RaceFuxHeld;

/* A hero's FUX as RacePickupRespawn reads it: a state at 0x18. */
typedef struct RaceFuxState {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x24 - 0x1C];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x58 - 0x28];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[0x130 - 0x5C];
    /* 0x130 */ s32 unk130;
} RaceFuxState;
/* A volcano rock's arc, passed to ArcUpdate (0x54 bytes). */
typedef struct RaceVolcArc {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x14 - 0x8];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x34 - 0x1C];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ u8 pad3C[0x44 - 0x3C];
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad48[0x54 - 0x48];
} RaceVolcArc;

/* Race volcano extension (0x15C bytes, id 0x10000000). */
typedef struct RaceVolcanoExt {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ EXTengine *eng[3];
    /* 0x024 */ SLONG_XYZ vel[3];
    /* 0x048 */ RaceVolcArc arc[3];
    /* 0x144 */ NAS *unk144[3];
    /* 0x150 */ s32 unk150[3];
} RaceVolcanoExt;

/* Squared distance between a and b (PlotRaceVolcano). */
static inline s32 RaceDistSq(SLONG_XYZ *a, SLONG_XYZ *b) {
    s32 dx;
    s32 dz;
    s32 dy;

    dx = a->x - b->x;
    dz = a->z - b->z;
    dy = a->y - b->y;
    return dx * dx + dz * dz + dy * dy;
}

/* Spawns the 0x1605 puff at v and returns it (PlotRaceVolcano). */
static inline NAS *RaceSpawnPuffRet(SLONG_XYZ *v, s32 kind, s32 mode) {
    NAS *nas;
    s32 *ext;

    nas = CreateNAS(v->x, v->y, v->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = kind;
            ext[6] = 0x100;
            ext[8] = mode;
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}



/* Barrel roll extension (0x80 bytes, id 0x10000000). */
typedef struct BarrelRollExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x20 - 0x14];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x40 - 0x28];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad48[0x50 - 0x48];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x60 - 0x54];
    /* 0x60 */ SLONG_XYZ pos;
    /* 0x6C */ SLONG_XYZ vel;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ EXTengine *shadow;
} BarrelRollExt;

void InitBarrelRoll(NAS *m);
s32 ArcUpdate(s32 *from, s32 *to, void *arc, s32 *pos);

extern NAS *RacePickups;
NAS *RaceNearestPickup(s32 arg0);

void InitRaceMissile(NAS *m);

extern ScriptInit DinoEggScriptInit;
extern s32 RaceEggCount;

/* Dac extension (0x28 bytes, id 0x10000000). */
typedef struct DacExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x18 - 0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ EXTengine *shadow;
    /* 0x24 */ s32 unk24;
} DacExt;

extern ScriptInit DacScriptInit;
void InitDac(NAS *m);

void InitRaceSmokeBoostWater(NAS *nas);

void InitRaceGreenSmokeBoost(NAS *nas);

extern ScriptInit RaceSmokeBoostWaterScriptInit;

void RaceQuatNormalize(Quat *q);

void InitRaceGreenSmokePoof(NAS *nas);

void InitRaceSmokePoof(NAS *nas);

void InitRaceFire(NAS *nas);

void InitTNTCrate(NAS *m);

s32 RaceTrackAngle(s32 arg0);

s32 TestRaceHeroWon(s32 arg0);

s32 RaceHeroTestIdle(s32 arg0);

void SetRaceHeroKick(s32 player);

void RaceInitHero(s32 arg0);

void RacePolarOf(s32 *pos, s32 *polar);

void RaceSpawnBarrelRolls(void);

void RaceCycleModel(void);

NAS *RacePickupAhead(s32 arg0);
/* AiPlayerSkill (declared in bash/pause.h, which this unit does not include). */
typedef struct RaceOdds {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} RaceOdds;

extern RaceOdds AiPlayerSkill[];

s32 RaceOtherHeroNear(u8 *arg0, u32 arg1, s32 arg2);

s32 RaceAI_TargetShare(s32 arg0, s32 arg1);
/* Glob extension, AddExt(nas, 0x1C, 0x10000000). */
typedef struct EXTglob {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ ScriptCursor *unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} EXTglob;

extern ScriptInit GlobScriptInit;

void InitGlob(NAS *m);

s32 RaceHeroTestRun(s32 arg0);

s32 RaceJumpUp(s32 player);
/* A scene object in the list at MapScene + 0x1C (RaceInitBarrels). */
typedef struct RaceSceneObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ SLONG_XYZ pos;
    /* 0x10 */ u8 pad10[2];
    /* 0x12 */ s16 angle; /* set by RaceInitSpinners */
    /* 0x14 */ u8 pad14[0x51 - 0x14];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 pad52[0x5C - 0x52];
    /* 0x5C */ struct RaceSceneObj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ struct RaceSceneModel *model;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ u16 index;
} RaceSceneObj;

/* An RaceSceneObj's model as RaceInitSpinners reads it. */
typedef struct RaceSceneModel {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 *unkC; /* passed to entity_address */
} RaceSceneModel;
extern s32 RaceSpinTimer;

/* A collision normal (8 bytes, copied whole) read by RaceFloorHeight. */
typedef struct RaceNorm {
    s16 x, y, z, pad;
} RaceNorm;

s32 RaceTriHeight(SLONG_XYZ *pos, s16 *tri);

/* MapScene as seen by RaceInitBarrels. */
typedef struct RaceScene {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ RaceSceneObj *objs;
} RaceScene;

/* A barrel (0x6C bytes, 128 in RaceBarrels). */
typedef struct RaceBarrelView {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ u8 pad2C[0x40 - 0x2C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ u8 pad4C[2];
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ u8 pad50[4];
    /* 0x54 */ RaceSceneObj *obj;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ SLONG_XYZ pos;
    /* 0x68 */ s32 unk68;
} RaceBarrelView;

extern RaceBarrelView RaceBarrels[128];
extern s32 RaceBarrelCount;

/* A hero's NAS.FUX as RaceBarrelBump sees it (velocities). */
typedef struct RaceFuxView {
    /* 0x000 */ u8 pad0[0x13C];
    /* 0x13C */ s32 unk13C;
    /* 0x140 */ s32 unk140;
    /* 0x144 */ u8 pad144[0x164 - 0x144];
    /* 0x164 */ s32 unk164; /* bounce factor, 8.8 (0x100 at setup) */
} RaceFuxView;

void RaceBarrelBump(s32 player, RaceBarrelView *b);

void RaceInitBarrels(void);

s32 RaceHeroTestOil(s32 arg0);

void SetRaceHeroIdle(s32 player);
/* GameInfo as seen by RaceExplodeHitHeroes. */
typedef struct D5A614View63 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x30 - 0x8];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 flags;
    /* 0x34 */ u8 unk34[0x5C - 0x34];
    /* 0x5C */ u8 unk5C[4];
    /* 0x60 */ u8 unk60;
} D5A614View63;
#define MODE63 ((D5A614View63 *)GameInfo)

extern u16 CameraMode;
extern HeroInfo Heroes[4];

void RaceHeroKick(s32 arg0);

s32 RaceExplodeHitHeroes(SLONG_XYZ *pos);

s32 RaceHeroNear(u8 *arg0, s32 arg1);

void RaceAxisAngleToQuat(s32 *arg0, s32 *arg1);

extern s32 RaceAccel;
extern s32 RaceMoveTune1;
extern s32 RaceMoveTune2;
extern s32 RaceDecel;
extern s32 RaceMoveTune3;
extern s32 RaceMoveTune4;
extern s32 RaceJumpVel;
extern s32 RaceMoveTune5;
extern s32 RaceSteer[];
/* boost tables indexed by odds - 0x16 (RacePreMoveHero) */
extern s32 RaceSteerSpeeds[];
extern s32 RaceSteerAngles[];

typedef struct {
    s16 a;
    s16 b;
    s16 c;
} RaceShort3;
extern RaceShort3 RaceJumpTable[8];

s32 RaceAI_PickVictim(s32 arg0);

void RaceInitHeroMove(s32 arg0);

void SetRaceHeroWinner(s32 player);

s32 RaceMissileHitHeroes(u8 *arg0, u32 arg1);

void RaceFireMissile(s32 player);

s32 RaceFallToFloor(s32 player);

/* Ripple extension (id 0x10000000) as PlotRippled reads it. */
typedef struct EXTripple {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 scale;
    /* 0x10 */ s32 step;
    /* 0x14 */ s32 max;
    /* 0x18 */ u16 fade;
} EXTripple;

void RaceAI_SteerTo(s32 player, SLONG_XYZ *pos);

void RaceAI_ChasePickup(s32 arg0);
/* Six object pointers walked by RaceAI_SpinnerBrake and others. */
extern s32 RaceSpinnerBoxes[];

s32 XZBoxToSphere(s32 obj, SLONG_XYZ *a, s32 b, SLONG_XYZ *c);

void RaceEaseCamStep(RaceEaseCam *c, s32 *pos);

void SetRaceHeroRun(s32 player);

s32 RaceHeroTestJump(s32 arg0);

void InitRacePickUp(NAS *m);

void InitDinoEgg(NAS *m);

s32 RaceReflectAngle(s32 a, s32 b);

s32 RaceHeroBreatheFire(s32 arg0);

void SetRaceHeroOiled(s32 player);

/* Track glob extension, AddExt(nas, 0x8C, 0x10000000). */
typedef struct EXTtrackglob {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x54 - 0x2C];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ u8 pad58[0x64 - 0x58];
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ SLONG_XYZ pos;
    /* 0x7C */ ScriptCursor *cursor;
    /* 0x80 */ EXTengine *eng;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
} EXTtrackglob;

extern ScriptInit TrackGlobScriptInit;
extern u8 *RaceObjects;
extern s32 RaceTrackGlobCount;

void RaceAI_SpinnerBrake(s32 player);

void RaceInitHeroAngle(s32 player);

s32 RaceBarrelHitHero(RaceBarrelView *b);

void SetRaceHeroJump(s32 player);

void RaceHeroOiled(s32 arg0);

void InitRaceVolcano(NAS *nas);

void InitTrackGlob(NAS *m);

void RaceResetJumps(void);

/* A hero's FUX as RaceHeroJump reads it: a jump phase at 0x138. */
typedef struct RaceJumpFux {
    /* 0x000 */ u8 pad0[0x138];
    /* 0x138 */ s32 phase;
} RaceJumpFux;

/* 4.12 fixed-point product rounded to nearest (RaceEulerToQuat). */
static inline s32 RaceFixMul(s32 a, s32 b) {
    return ((a * b) >= 0 ? (a * b) + 0x800 : (a * b) - 0x800) >> 12;
}

/* Inline copy of RaceJumpUp: steps the jump, true once above RaceFloorLevel. */
/* Inline copy of RaceJumpUp: steps a hero's jump and reports a landing
 * above the floor (RaceFloorLevel), as RaceHeroDazed inlines it. */
static inline s32 RaceJumpAbove(s32 player) {
    EXTheromove *move;
    EXTengine *eng;
    s32 y;

    move = Heroes[player].hero_move;
    eng = Heroes[player].hero_eng;
    if (move != NULL && eng != NULL) {
        if (move->jump_vel < 0) {
            move->jump_vel += move->jump_acc;
        } else {
            move->jump_vel += move->jump_dec;
        }
        y = eng->unk10.y += move->jump_vel;
        if (move->jump_vel > 0 && y > RaceFloorLevel) {
            return 1;
        }
    }
    return 0;
}

s32 RaceKeepOnTrack(s32 arg0, s32 arg1);

void RaceEulerToQuat(VECTOR *ang, Quat *q);

void RaceHeroRun(s32 player);

void RaceHeroDazed(s32 arg0);

void RaceHeroJump(s32 player);
extern s32 RaceSpinnerBoxes[];

typedef struct RaceBoxHalf {
    s16 v[3];
} RaceBoxHalf;

typedef struct RaceBox {
    RaceBoxHalf lo;
    RaceBoxHalf hi;
} RaceBox;

/* Sets ang to a's rotation and passes v through (XZBoxToSphere). */
static inline s32 RaceBoxPushRot(SVECTOR *ang, u16 *a, s32 v) {
    ang->vx = a[8];
    ang->vy = a[9];
    ang->vz = a[10];
    return v;
}

s32 XZBoxToSphere(s32 arg0, SLONG_XYZ *arg1, s32 arg2, SLONG_XYZ *arg3);

/* Spawns the 0x1605 puff at v (RaceUpdateBarrels). */
static inline void RaceSpawnPuff(SLONG_XYZ *v, s32 kind, void *anim) {
    NAS *nas;
    s32 *ext;

    nas = CreateNAS(v->x, v->y, v->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = kind;
            ext[6] = 0x100;
            if (anim != NULL) {
                ext[8] = 5;
                nas->Engine->sub->unk6C = anim;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
    }
}

s32 RaceAI_DodgeSpinners(s32 player);

void SetRaceHeroOffTrack(s32 player);

/* A hero's FUX as PlotRaceTag counts it down. */
typedef struct RaceTagFux {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10[0x8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x10];
    /* 0x2C */ s32 unk2C;
} RaceTagFux;

/* The 0x10000000 extension of the 0x1605 tag spawned by PlotRaceTag. */
typedef struct RaceTagExt {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10[0x8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 unk24[0x4];
    /* 0x28 */ s32 unk28;
} RaceTagExt;

extern s32 RaceTagTimer;

s32 RaceFindNode(s32 *arg0, s32 arg1, s32 *arg2);

s32 RaceBarrelSpaceFree(RaceBarrelView *self);

void RaceInitStub(void);

void PlotRaceTag(NAS *m);

void PlotBarrelRoll(NAS *nas);

void InitRaceSmokeBoost(NAS *nas);

/* A hero's FUX as RaceCountLaps counts laps in it. */
typedef struct RaceLapFux {
    /* 0x000 */ u8 pad0[0x30];
    /* 0x030 */ s32 cross;
    /* 0x034 */ s32 laps;
    /* 0x038 */ u8 pad38[0x148 - 0x38];
    /* 0x148 */ s32 started;
    /* 0x14C */ u8 pad14C[0x170 - 0x14C];
    /* 0x170 */ s32 side;
} RaceLapFux;

extern s32 GameCheats;

void RaceInitSpinners(void);

void RaceCountLaps(s32 player);

void PlotGlob(NAS *m);

/* A hero's FUX as PlotRaceShield drives it: flags, the shield NAS, velocity. */
typedef struct RaceShieldFux {
    /* 0x000 */ u8 pad0[0x130];
    /* 0x130 */ s32 unk130;
    /* 0x134 */ NAS *shield;
    /* 0x138 */ s32 unk138;
    /* 0x13C */ s32 unk13C;
    /* 0x140 */ s32 unk140;
} RaceShieldFux;

/* Extension 0x10000000 of the effect NAS PlotRaceShield spawns. */
typedef struct RaceShieldFxExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
} RaceShieldFxExt;

void PlotRaceShield(NAS *nas);

/* HeroHooks (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct RaceHooks {
    /* 0x00 */ s32 InitHero;
    /* 0x04 */ void *HeroModels;
    /* 0x08 */ void *unk8;
    /* 0x0C */ u16 *RunSpeeds;
    /* 0x10 */ void *JumpEnts;
    /* 0x14 */ void *VictorySeqs;
    /* 0x18 */ void *CreateShadow;
    /* 0x1C */ void *UpdateShadow;
    /* 0x20 */ void *PreMoveHero;
    /* 0x24 */ void *MoveHero;
    /* 0x28 */ void *PostMoveHero;
    /* 0x2C */ void *HeroFacing;
    /* 0x30 */ void *HeroSetIdle;
    /* 0x34 */ void *HeroTouched;
    /* 0x38 */ void *HeroBot;
    /* 0x3C */ void *TestAttack;
    /* 0x40 */ void *TestRun;
    /* 0x44 */ void *TestJump;
    /* 0x48 */ void *TestKick;
    /* 0x4C */ void *Hero2ObjectCollision;
    /* 0x50 */ void *HeroGrabObject;
    /* 0x54 */ void *HeroKickCollision;
    /* 0x58 */ void *HeroCarryCollision;
    /* 0x5C */ void *HeroExtraEffects;
    /* 0x60 */ void *HeroesTouched;
    /* 0x64 */ void (*ChooseWinner)(s32, s32);
    /* 0x68 */ void *KeepInBounds;
} RaceHooks;
extern RaceHooks HeroHooks;
extern void *AiBotTweakTable;
extern void *AiHumanTweakTable;
extern void *AiSkillTable;
extern void *AiTeamSkillTable;
extern void *AiHumanRoundTweakTable;

/* Tables InitialiseRACEFirst hooks into the engine. */
void ModelCreateShadow(s32 idx);
void ModelUpdateShadow(s32 idx);
extern s32 RaceVictorySeqs[]; /* {name, id} pairs: the victory sequences (the engine's hook 0x14) */
s32 RaceFloorHeight(SLONG_XYZ *arg0, s32 *arg1);

void RacePreMoveHero();
void RaceMoveHero(s32 player);
void RaceHeroOffTrack(s32 player);

void SetRaceHeroDazed(s32 player);

void RaceChooseWinner(s32 arg0, u16 arg1);
void RaceChooseWinnerTeam();

/* hero->FUX as RaceChooseWinner (round result) reads it. */
typedef struct RaceResultFux {
    /* 0x000 */ u8 unk0[0x128];
    /* 0x128 */ _sfx_handler *unk128;
    /* 0x12C */ u8 unk12C[0x148 - 0x12C];
    /* 0x148 */ s32 unk148;
} RaceResultFux;

/* GameInfo as seen by RaceChooseWinner (round result). */
typedef struct RaceResultView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32[0x48 - 0x32];
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ u8 unk4C[0x60 - 0x4C];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62[0x6C - 0x62];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70[0xA7 - 0x70];
    /* 0xA7 */ u8 unkA7[4];
} RaceResultView;
#define RV ((RaceResultView *)GameInfo)

/* RaceChooseWinner: a drawn round, counting with the caller's i. */
#define RACE_DRAW_I(g, i, arg1)                                            \
    (g)->unk60 = 0;                                                         \
    (g)->unk30 |= (arg1);                                                   \
    for (i = 0; i < 4; i++) {                                               \
        Heroes[i].hero_status |= 0x100;                                     \
    }

/* RaceChooseWinner: team w wins by total, re-reading the team indices. */
#define RACE_TEAM_WIN_BY(g, hb, w, l)                                      \
    (g)->unk30 |= 0x800;                                                    \
    (g)->unk60 = hb[w].hero_name;                                           \
    (g)->unk61 = hb[hb[w].hero_teammate].hero_name;                         \
    hb[w].hero_status |= 0x200;                                             \
    hb[hb[w].hero_teammate].hero_status |= 0x200;                           \
    hb[l].hero_status |= 0x400;                                             \
    hb[hb[l].hero_teammate].hero_status |= 0x400;                           \
    hb[l].hero_status |= 0x8000;                                            \
    hb[hb[l].hero_teammate].hero_status |= 0x8000;                          \
    hb[l].hero_shadow->sub->flags &= ~0x8000;                               \
    hb[hb[l].hero_teammate].hero_shadow->sub->flags &= ~0x8000;

/* RaceChooseWinner: hero p's facing, printed (RaceTrackAngle's sum unfolded). */
static inline s32 RaceFacing(s32 p) {
    EXTengine *eng;
    s32 ang;

    eng = Heroes[p].hero_eng;
    ang = get_angle(eng->unk10.x, eng->unk10.z) & 0xFFF;
    ang -= 0x52C;
    ang = 0x1000 - ang;
    ang &= 0xFFF;
    printf(RaceMsgPlayerAngle, p, ang);
    return ang;
}


/* RaceChooseWinnerTeam: a drawn round (every hero gets 0x100). */
#define RACE_DRAW(k, arg1)                                 \
    {                                                       \
        s32 k;                                              \
        GameInfo[0x60] = 0;                               \
        *(u16 *)(GameInfo + 0x30) |= (arg1);              \
        for (k = 0; k < 4; k++) {                           \
            Heroes[k].hero_status |= 0x100;                 \
        }                                                   \
    }

/* RaceChooseWinnerTeam: team w/wt wins, l/lt lose and lose their shadows. */
#define RACE_TEAM_WIN(w, wt, l, lt)                        \
    GameInfo[0x60] = Heroes[w].hero_name;                 \
    GameInfo[0x61] = Heroes[wt].hero_name;                \
    Heroes[w].hero_status |= 0x200;                         \
    Heroes[wt].hero_status |= 0x200;                        \
    Heroes[l].hero_status |= 0x400;                         \
    Heroes[lt].hero_status |= 0x400;                        \
    Heroes[l].hero_status |= 0x8000;                        \
    Heroes[l].hero_shadow->sub->flags &= ~0x8000;           \
    Heroes[lt].hero_status |= 0x8000;                       \
    Heroes[lt].hero_shadow->sub->flags &= ~0x8000;

void RaceHeroIdle(s32 player);

s32 RaceHeroCollide(s32 player);

void PlotRaceMissile(NAS *m);

void PlotRacePickUp(NAS *m);

void PlotTNTCrate(NAS *nas);

void PlotDinoEgg(NAS *m);

void PlotRaceVolcano(NAS *nas);

void PlotTrackGlob(NAS *nas);

void PlotDac(NAS *nas);

extern const char RaceMsgError[];

/* RaceTrackCollideB: wall segments of a scene collision object (as dino's func_800B9020). */
typedef struct RaceVec {
    s32 vx, vy, vz;
} RaceVec;

/* The scene's table of objects: a count, then each object's offset from its own entry. */
typedef struct RaceSegTable {
    s32 count;
    s32 offs[1];
} RaceSegTable;

/* A local copy of a segment's two ends (16-byte stride). */
typedef struct RaceSegEnds {
    RaceVec p0;
    s32 pad0;
    RaceVec p1;
    s32 pad1;
} RaceSegEnds;

typedef struct RaceRot {
    u8 pad0[0x12];
    s16 ang; /* 0x12 */
} RaceRot;

/* The blend between two collision objects RaceTrackCollideB takes. */
typedef struct RaceSegBlend {
    s32 t;          /* 0x0 */
    s32 unk4;
    s32 idx;        /* 0x8 */
    RaceRot *rot;  /* 0xC */
} RaceSegBlend;

s32 RaceTrackCollideB(VECTOR *pos, RaceVec *out, s32 radius, s32 idx, s32 flags, RaceSegBlend *bl);

s32 RaceTrackCollideC(VECTOR *pos, RaceVec *out, s32 radius, s32 idx, s32 flags, SVECTOR *rot);

#endif /* BASH_RACE_OVL_H */

/* Race's AI tweak tables and hero models (halfwords) */
extern u16 RaceDashAiSkill[];
extern u16 RaceDashAiSkillAlt[];
extern u16 RaceDashAiTeamSkill[];
extern u16 RaceDashAiHumanRoundTweak[];
extern u16 RaceDashAiBotTweak[];
extern u16 RaceDashAiHumanTweak[];
extern u16 RaceToxicAiSkill[];
extern u16 RaceToxicAiSkillAlt[];
extern u16 RaceToxicAiTeamSkill[];
extern u16 RaceToxicAiHumanRoundTweak[];
extern u16 RaceToxicAiBotTweak[];
extern u16 RaceToxicAiHumanTweak[];
extern u16 RaceLavaAiSkill[];
extern u16 RaceLavaAiSkillAlt[];
extern u16 RaceLavaAiTeamSkill[];
extern u16 RaceLavaAiHumanRoundTweak[];
extern u16 RaceLavaAiBotTweak[];
extern u16 RaceLavaAiHumanTweak[];
extern u16 RaceWaterAiSkill[];
extern u16 RaceWaterAiSkillAlt[];
extern u16 RaceWaterAiTeamSkill[];
extern u16 RaceWaterAiHumanRoundTweak[];
extern u16 RaceWaterAiBotTweak[];
extern u16 RaceWaterAiHumanTweak[];
extern u16 RaceHeroModels[];
extern u16 RaceHeroModelsWater[];
