#ifndef BASH_OVL63_H
#define BASH_OVL63_H

/* Functions and data defined in the ovl63 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl63/ovl63.c */

#include "common.h"
#include "bash/nas.h"

extern HeroInfo Heroes[4];

void PlotRaceSmokeBoostWater(NAS *nas);
void PlotRaceGreenSmokeBoost(NAS *nas);
void PlotRaceGreenSmokePoof(NAS *nas);
void PlotRaceSmokePoof(NAS *nas);
s32 func_800C4D1C(u8 *arg0, u8 *arg1);
void InitRaceTag(u8 *arg0);
s32 func_800C5014(s32 arg0);
void PlotRaceSmokeBoost(NAS *nas);
void func_800C53BC(s32 arg0);
s32 func_800C5AF8(void);
s32 func_800C5B7C(void);
void func_800C5B84(void);
void func_800C5B8C(void);
s32 func_800C5D0C(s32 arg0);
void func_800C5D8C(void);

extern s32 D_800C6930;
extern s32 D_800C6D20;
extern ScriptInit D_800C6E2C;
extern ScriptInit D_800C6C8C;
extern s32 D_80054F84;
extern HeroInfo Heroes[4];
extern ScriptInit D_800C6D74;
extern ScriptInit D_800C6DD0;
extern u8 D_800546C4[];
extern s32 D_800C6BAC;
extern u8 D_80052F94[];
extern char D_800B3400[];

void func_800B5360(s32 player);
u32 func_800B4EDC(s32 player);

/* A hero's FUX as func_800B5360 reads it: a 2D velocity at 0x13C. */
typedef struct Ovl63VelFux {
    /* 0x000 */ u8 pad0[0x13C];
    /* 0x13C */ s32 vx;
    /* 0x140 */ s32 vz;
} Ovl63VelFux;

/* A hero's FUX as func_800BEBC0 reads it: held item, timer, velocity. */
typedef struct Ovl63BumpFux {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ s32 item;
    /* 0x01C */ u8 pad1C[0x10];
    /* 0x02C */ s32 timer;
    /* 0x030 */ u8 pad30[0x10C];
    /* 0x13C */ s32 vx;
    /* 0x140 */ s32 vz;
} Ovl63BumpFux;
void func_800BE914(s32 a, s32 b);

/* Extension 0x20000000 as func_800B4708 drives it. */
typedef struct EXTovl63ai {
    /* 0x00 */ s32 unk0[5];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ void (*unk18)(s32 player);
} EXTovl63ai;
extern s32 D_800C6980[];
void func_800B4708();
void func_800B372C(s32 arg0);
void func_800B3890(s32 arg0);
void func_800B3BC0(s32 arg0);
s32 func_800B406C(s32 arg0);
s32 func_800B41DC(s32 arg0);
s32 func_800B442C(s32 arg0);

void func_800B7D94(void);

void func_800B4A4C();
void func_800B8890();
void func_800B8D68();

void PlotRaceFire(NAS *nas);

/* A value func_800B5FFC eases toward its target by a fixed step. */
typedef struct Ovl63Ease {
    /* 0x00 */ s32 cur;
    /* 0x04 */ s32 target;
    /* 0x08 */ s16 step;     /* used while target != 0 */
    /* 0x0A */ s16 stepZero; /* used while target == 0 */
    /* 0x0C */ u8 padC[0x20 - 0xC];
} Ovl63Ease;

/* The offsets func_800B5FFC eases and adds (>> 8) to a position. */
typedef struct Ovl63EaseCam {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Ovl63Ease x;
    /* 0x40 */ Ovl63Ease z;
} Ovl63EaseCam;

/* Shield extension, AddExtension(nas, 0x20, 0x10000000). */
typedef struct EXTraceshield {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14[2];
    /* 0x1C */ s32 unk1C;
} EXTraceshield;

/* A hero's FUX as func_800B406C reads it: a mode word at 0x18. */
typedef struct Ovl63Fux18 {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 mode;
} Ovl63Fux18;

/* A hero's FUX as func_800C57B4 clears it. */
typedef struct Ovl63HeroFux {
    /* 0x00 */ u8 unk0[0x8C];
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ s32 unk90;
} Ovl63HeroFux;

/* A hero's FUX as func_800BB794 clears it: a word at 0x40. */
typedef struct Ovl63Fux40 {
    /* 0x00 */ u8 unk0[0x40];
    /* 0x40 */ s32 unk40;
} Ovl63Fux40;

/* A hero's FUX as func_800BC2D0 clears it: a word at 0x138. */
typedef struct Ovl63Fux138 {
    /* 0x000 */ u8 unk0[0x138];
    /* 0x138 */ s32 unk138;
} Ovl63Fux138;

/* Per-hero run speed rows: 3 u16 per hero_name. */
extern u16 *D_8009D6F4;

/* A hero's FUX as func_800C5470 reads it: a count at 0x3C. */
typedef struct Ovl63FuxCount {
    /* 0x00 */ u8 unk0[0x3C];
    /* 0x3C */ s32 unk3C;
} Ovl63FuxCount;
/* A hero's FUX as func_800C56D4 fills it. */
typedef struct Ovl63SpinFux {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
} Ovl63SpinFux;

/* A hero's FUX as func_800BC6C8 counts it down: a timer at 0x24. */
typedef struct Ovl63FuxTimer {
    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ s32 unk24;
} Ovl63FuxTimer;

/* Hero FUX pending-spawn flags, as seen by func_800BC984. */
typedef struct Ovl63SpawnFux {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ s32 unk18;
    /* 0x01C */ u8 pad1C[0x130 - 0x1C];
    /* 0x130 */ s32 unk130;
} Ovl63SpawnFux;

/* Extension 0x10000000 of the NAS spawned by func_800BC984. */
typedef struct Ovl63SpawnExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} Ovl63SpawnExt;

s32 func_800BC984(s32 player);

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
/* Rippled extension, AddExtension(nas, 0x1C, 0x10000000). */
typedef struct EXTrippled {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 scale;
    /* 0x10 */ s32 speed;
    /* 0x14 */ s32 max;
    /* 0x18 */ s32 unk18;
} EXTrippled;
typedef struct Ovl63Bouncer {
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
} Ovl63Bouncer;

typedef struct Ovl63BounceObj {
    s32 unk0[5];
    s32 pos;      /* 0x14 */
} Ovl63BounceObj;

void func_800C5204(EXTheromove *a, EXTheromove *b);

extern HeroInfo Heroes[4];
void SetHeroAction(HeroInfo *hero, s32 action);
s32 func_8008C4C8(s32 *arg0, s32 arg1);
s32 func_8008CEB0(u8 *arg0, u8 *arg1);

s32 func_800C467C(Ovl63BounceObj *obj, Ovl63Bouncer *b);

s32 func_800C4F30(u8 *arg0);

void func_800C5F54(void);
/* D_8005A614 as seen by func_800B8D68. */
typedef struct Ovl63GameView {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14[0x30 - 0x14];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 unk34;
} Ovl63GameView;
#define G63 ((Ovl63GameView *)D_8005A614)
/* D_8005A614 as seen by func_800BF0F8. */
typedef struct Ovl63GameView2 {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x30 - 0x8];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 unk36[0x5C - 0x36];
    /* 0x5C */ u8 unk5C[4];
    /* 0x60 */ u8 unk60;
} Ovl63GameView2;
#define G63B ((Ovl63GameView2 *)D_8005A614)

/* One of four random voice sfx (func_800BBA04; as SFX4 in ovl65.h). */
#define OVL63_SFX4(a, b, c, d)                              \
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

void func_8007EB1C(s32 arg0, s32 arg1, s32 arg2);
void func_8007B7FC(u32 a, u32 b);
extern s32 D_800C6BA4;
extern s32 D_800C6BA8;

/* A hero's race FUX, AddExtension(nas, 0x174, 0x10000000) in func_800B8890. */
typedef struct Ovl63RaceFux {
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
} Ovl63RaceFux;

/* Extension 0x20000000 as func_800B8890 sets it up. */
typedef struct Ovl63AiExt {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ void (*unk18)(s32 player);
    /* 0x1C */ s32 unk1C;
} Ovl63AiExt;

/* hero->FUX as reset by func_800BAD54. */
typedef struct Ovl63ResetFux {
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
} Ovl63ResetFux;

extern s32 D_800C692C;
extern s32 D_800C695C;

/* Extension 0x10000000 as PlotDac drives it. */
typedef struct EXTovl63dac {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 state;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 angle;
    /* 0x18 */ s32 timer;
    /* 0x1C */ s32 height;
    /* 0x20 */ EXTengine *shadow;
    /* 0x24 */ s32 player;
} EXTovl63dac;

typedef struct Ovl63Obj74 {
    /* 0x00 */ u8 unk0[0x74];
    /* 0x74 */ s16 unk74;
} Ovl63Obj74;
extern Ovl63Obj74 *D_800C69B4;
extern s32 D_800C69B8;
extern s32 D_800C69BC;
extern s32 D_800C6BB0;
extern u16 D_800AD12C;

/* Spinning object func_800B5D14 updates: rotation at 0x10, its matrix at 0x30. */
typedef struct Ovl63SpinObj {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ SVECTOR rot;
    /* 0x18 */ u8 unk18[0x30 - 0x18];
    /* 0x30 */ MATRIX mat;
} Ovl63SpinObj;
extern Ovl63SpinObj *D_800C69A4;
extern s32 D_800CA608;
extern s32 D_800C69AC;
extern u8 D_800C6B70[];
s32 func_800B5A70();

/* A step of a func_800B5A70 pattern (0xC bytes). */
typedef struct Ovl63PatStep {
    /* 0x00 */ s32 hold;
    /* 0x04 */ s32 delay; /* -1: never */
    /* 0x08 */ s32 unk8;
} Ovl63PatStep;

/* A func_800B5A70 pattern: its steps and its length. */
typedef struct Ovl63Pattern {
    /* 0x00 */ Ovl63PatStep *steps;
    /* 0x04 */ s32 length;
} Ovl63Pattern;

/* One of the six pistons func_800B5A70 moves (0x1C bytes). */
typedef struct Ovl63Piston {
    /* 0x00 */ s32 pos;
    /* 0x04 */ s32 state;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 timer;
    /* 0x14 */ s32 hold;
    /* 0x18 */ s32 active;
} Ovl63Piston;
extern Ovl63Piston D_800CA55C[6];
extern s32 D_800C69A8;
extern s32 D_800C6BA0;

void func_800B5D14(void);
void func_800B6474(void);
void func_800C04EC(void);
void func_800C0CB4(void);
/* PadState (D_80051338) as func_800C554C reads it: d-pad bits 0xF0 at 0x50. */
typedef struct Ovl63Pad {
    /* 0x00 */ u8 unk0[0x50];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl63Pad;
/* Race missile extension (0x78 bytes, id 0x10000000). */
typedef struct RaceMissileExt {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x20 - 0x14];
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
    /* 0x6C */ u8 pad6C[0x78 - 0x6C];
} RaceMissileExt;

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

/* Pickup spawn slots func_800C0CB4 picks from: an angle and an in-use flag. */
typedef struct Ovl63PickUpSlot {
    /* 0x00 */ s32 angle;
    /* 0x04 */ s32 used;
} Ovl63PickUpSlot;
extern Ovl63PickUpSlot D_800CA64C[16];
extern s32 D_800C6CB4;

/* A hero's FUX as func_800C0CB4 reads it: a state at 0x18. */
typedef struct Ovl63FuxState {
    /* 0x00 */ u8 unk0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x24 - 0x1C];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x58 - 0x28];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[0x130 - 0x5C];
    /* 0x130 */ s32 unk130;
} Ovl63FuxState;
/* Race volcano extension (0x15C bytes, id 0x10000000). */
typedef struct RaceVolcanoExt {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ u8 pad14[0x18 - 0x14];
    /* 0x018 */ EXTengine *eng[3];
    /* 0x024 */ u8 pad24[0x144 - 0x24];
    /* 0x144 */ s32 unk144[3];
    /* 0x150 */ s32 unk150[3];
} RaceVolcanoExt;

extern u8 D_80052F5C[];

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
s32 func_8008C5C4(s32 *from, s32 *to, void *arc, s32 *pos);

extern NAS *D_800C6920;
NAS *func_800C5C64(s32 arg0);

void InitRaceMissile(NAS *m);

extern ScriptInit D_800C6D14;
extern s32 D_800C6924;

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

extern ScriptInit D_800C6ECC;
void InitDac(NAS *m);

void InitRaceSmokeBoostWater(NAS *nas);

void InitRaceGreenSmokeBoost(NAS *nas);

extern ScriptInit D_800C6F28;

void func_800B9E50(Quat *q);

void InitRaceGreenSmokePoof(NAS *nas);

void InitRaceSmokePoof(NAS *nas);

void InitRaceFire(NAS *nas);

void InitTNTCrate(NAS *m);

s32 func_800C5334(s32 arg0);

s32 func_800C5400(s32 arg0);

s32 func_800C554C(s32 arg0);

void func_800C55FC(s32 player);

void func_800C5918(s32 arg0);

void func_800C5B08(s32 *pos, s32 *polar);

void func_800C5B94(void);

void func_800C5BEC(void);

NAS *func_800C5DAC(s32 arg0);
/* D_8009D87C (declared in bash/8007ee2c.h, which this unit does not include). */
typedef struct Ovl63Odds {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Ovl63Odds;

extern Ovl63Odds D_8009D87C[];

s32 func_800C5640(u8 *arg0, u32 arg1, s32 arg2);

s32 func_800C5E6C(s32 arg0, s32 arg1);
/* Glob extension, AddExtension(nas, 0x1C, 0x10000000). */
typedef struct EXTglob {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ ScriptCursor *unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} EXTglob;

extern ScriptInit D_800C6E80;

void InitGlob(NAS *m);

s32 func_800C55A4(s32 arg0);

s32 func_800C5A50(s32 player);
/* A scene object in the list at D_80058AD0 + 0x1C (func_800B5F04). */
typedef struct Ovl63SceneObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ SLONG_XYZ pos;
    /* 0x10 */ u8 pad10[2];
    /* 0x12 */ s16 angle; /* set by func_800B5778 */
    /* 0x14 */ u8 pad14[0x51 - 0x14];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 pad52[0x5C - 0x52];
    /* 0x5C */ struct Ovl63SceneObj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ struct Ovl63SceneModel *model;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ u16 index;
} Ovl63SceneObj;

/* An Ovl63SceneObj's model as func_800B5778 reads it. */
typedef struct Ovl63SceneModel {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 *unkC; /* passed to func_80015A18 */
} Ovl63SceneModel;
extern s32 D_800CA604;

/* A collision normal (8 bytes, copied whole) read by func_800B6DBC. */
typedef struct Ovl63Norm {
    s16 x, y, z, pad;
} Ovl63Norm;

s32 func_800B66C4(SLONG_XYZ *pos, s16 *tri);

/* D_80058AD0 as seen by func_800B5F04. */
typedef struct Ovl63Scene {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Ovl63SceneObj *objs;
} Ovl63Scene;

/* A barrel (0x6C bytes, 128 in D_800C6F34). */
typedef struct Ovl63Barrel {
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
    /* 0x54 */ Ovl63SceneObj *obj;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ SLONG_XYZ pos;
    /* 0x68 */ s32 unk68;
} Ovl63Barrel;

extern Ovl63Barrel D_800C6F34[128];
extern s32 D_800C66F0;

/* A hero's NAS.FUX as func_800BE7B4 sees it (velocities). */
typedef struct Ovl63Fux {
    /* 0x000 */ u8 pad0[0x13C];
    /* 0x13C */ s32 unk13C;
    /* 0x140 */ s32 unk140;
} Ovl63Fux;

void func_800BE7B4(s32 player, Ovl63Barrel *b);

void func_800B5F04(void);

s32 func_800BCD1C(s32 arg0);

void func_800C57B4(s32 player);
/* D_8005A614 as seen by func_800C310C. */
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
#define MODE63 ((D5A614View63 *)D_8005A614)

extern u16 D_800AD12C;
extern HeroInfo Heroes[4];

void func_800BC7C4(s32 arg0);

s32 func_800C310C(SLONG_XYZ *pos);

s32 func_800C50B0(u8 *arg0, s32 arg1);

void func_800C5854(s32 *arg0, s32 *arg1);

extern s32 D_800C66CC;
extern s32 D_800C66D0;
extern s32 D_800C66D4;
extern s32 D_800C66D8;
extern s32 D_800C66DC;
extern s32 D_800C66E0;
extern s32 D_800C66E4;
extern s32 D_800C66E8;
extern s32 D_800CA534[];

typedef struct {
    s16 a;
    s16 b;
    s16 c;
} Ovl63Short3;
extern Ovl63Short3 D_800C6BB4[8];

s32 func_800B35CC(s32 arg0);

void func_800B8F04(s32 arg0);

void func_800BCDAC(s32 player);

s32 func_800BF0F8(u8 *arg0, u32 arg1);

void func_800C5154(s32 player);

s32 func_800C5978(s32 player);

/* Ripple extension (id 0x10000000) as PlotRippled reads it. */
typedef struct EXTripple {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 scale;
    /* 0x10 */ s32 step;
    /* 0x14 */ s32 max;
    /* 0x18 */ u16 fade;
} EXTripple;

void func_800B5528(s32 player, SLONG_XYZ *pos);

void func_800B5660(s32 arg0);
/* Six object pointers walked by func_800B372C and others. */
extern s32 D_800CA544[];

s32 func_800B82FC(s32 obj, SLONG_XYZ *a, s32 b, SLONG_XYZ *c);

void func_800B5FFC(Ovl63EaseCam *c, s32 *pos);

void func_800BB794(s32 player);

s32 func_800BCC20(s32 arg0);

void InitRacePickUp(NAS *m);

void InitDinoEgg(NAS *m);

s32 func_800C4E54(s32 a, s32 b);

s32 func_800C5470(s32 arg0);

void func_800C56D4(s32 player);

/* Track glob extension, AddExtension(nas, 0x8C, 0x10000000). */
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

extern ScriptInit D_800C6EA4;
extern u8 *D_800C66EC;
extern s32 D_800C6EB0;

void func_800B372C(s32 player);

void func_800B48EC(s32 player);

s32 func_800B6108(Ovl63Barrel *b);

void func_800BC2D0(s32 player);

void func_800BC6C8(s32 arg0);

void InitRaceVolcano(NAS *nas);

void InitTrackGlob(NAS *m);

void func_800C44B0(void);

/* A hero's FUX as func_800BC4B0 reads it: a jump phase at 0x138. */
typedef struct Ovl63JumpFux {
    /* 0x000 */ u8 pad0[0x138];
    /* 0x138 */ s32 phase;
} Ovl63JumpFux;

/* 4.12 fixed-point product rounded to nearest (func_800BA004). */
static inline s32 Ovl63FixMul(s32 a, s32 b) {
    return ((a * b) >= 0 ? (a * b) + 0x800 : (a * b) - 0x800) >> 12;
}

/* Inline copy of func_800C5A50: steps the jump, true once above D_800C6930. */
/* Inline copy of func_800C5A50: steps a hero's jump and reports a landing
 * above the floor (D_800C6930), as func_800BC0A4 inlines it. */
static inline s32 Ovl63JumpAbove(s32 player) {
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
        if (move->jump_vel > 0 && y > D_800C6930) {
            return 1;
        }
    }
    return 0;
}

s32 func_800B96B0(s32 arg0, s32 arg1);

void func_800BA004(VECTOR *ang, Quat *q);

void func_800BB878(s32 player);

void func_800BC0A4(s32 arg0);

void func_800BC4B0(s32 player);
extern s32 D_800CA544[];

typedef struct Ovl63BoxHalf {
    s16 v[3];
} Ovl63BoxHalf;

typedef struct Ovl63Box {
    Ovl63BoxHalf lo;
    Ovl63BoxHalf hi;
} Ovl63Box;

/* Sets ang to a's rotation and passes v through (func_800B82FC). */
static inline s32 Ovl63BoxPushRot(SVECTOR *ang, u16 *a, s32 v) {
    ang->vx = a[8];
    ang->vy = a[9];
    ang->vz = a[10];
    return v;
}

s32 func_800B82FC(s32 arg0, SLONG_XYZ *arg1, s32 arg2, SLONG_XYZ *arg3);

/* Spawns the 0x1605 puff at v (func_800B6474). */
static inline void Ovl63SpawnPuff(SLONG_XYZ *v, s32 kind, void *anim) {
    NAS *nas;
    s32 *ext;

    nas = SpawnNASById(v->x, v->y, v->z, 0x1605, 0x11);
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

s32 func_800B345C(s32 player);

void func_800BAD54(s32 player);

/* A hero's FUX as PlotRaceTag counts it down. */
typedef struct Ovl63TagFux {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10[0x8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x10];
    /* 0x2C */ s32 unk2C;
} Ovl63TagFux;

/* The 0x10000000 extension of the 0x1605 tag spawned by PlotRaceTag. */
typedef struct Ovl63TagExt {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10[0x8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 unk24[0x4];
    /* 0x28 */ s32 unk28;
} Ovl63TagExt;

extern s32 D_800C6928;

s32 func_800B4D64(s32 *arg0, s32 arg1, s32 *arg2);

s32 func_800B62F4(Ovl63Barrel *self);

void func_800C0F5C(void);

void PlotRaceTag(NAS *m);

void PlotBarrelRoll(NAS *nas);

void InitRaceSmokeBoost(NAS *nas);

/* A hero's FUX as func_800C0218 counts laps in it. */
typedef struct Ovl63LapFux {
    /* 0x000 */ u8 pad0[0x30];
    /* 0x030 */ s32 cross;
    /* 0x034 */ s32 laps;
    /* 0x038 */ u8 pad38[0x148 - 0x38];
    /* 0x148 */ s32 started;
    /* 0x14C */ u8 pad14C[0x170 - 0x14C];
    /* 0x170 */ s32 side;
} Ovl63LapFux;

extern s32 D_8005A618;
extern void (*D_8009D74C)(s32, s32);

void func_800B5778(void);

void func_800C0218(s32 player);

void PlotGlob(NAS *m);

/* A hero's FUX as PlotRaceShield drives it: flags, the shield NAS, velocity. */
typedef struct Ovl63ShieldFux {
    /* 0x000 */ u8 pad0[0x130];
    /* 0x130 */ s32 unk130;
    /* 0x134 */ NAS *shield;
    /* 0x138 */ s32 unk138;
    /* 0x13C */ s32 unk13C;
    /* 0x140 */ s32 unk140;
} Ovl63ShieldFux;

/* Extension 0x10000000 of the effect NAS PlotRaceShield spawns. */
typedef struct Ovl63ShieldFxExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
} Ovl63ShieldFxExt;

void PlotRaceShield(NAS *nas);

/* D_8009D6E8 (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct Ovl63Hooks {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ void *unkC;
    /* 0x10 */ void *unk10;
    /* 0x14 */ void *unk14;
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ void *unk2C;
    /* 0x30 */ void *unk30;
    /* 0x34 */ void *unk34;
    /* 0x38 */ void *unk38;
    /* 0x3C */ void *unk3C;
    /* 0x40 */ void *unk40;
    /* 0x44 */ void *unk44;
    /* 0x48 */ void *unk48;
    /* 0x4C */ void *unk4C;
    /* 0x50 */ void *unk50;
    /* 0x54 */ void *unk54;
    /* 0x58 */ void *unk58;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
} Ovl63Hooks;
extern Ovl63Hooks D_8009D6E8;
extern void *D_8009D8F4;
extern void *D_8009D8F8;
extern void *D_8009D8FC;
extern void *D_8009D900;
extern void *D_8009D904;
extern s32 D_80051658;

/* Tables func_800B4A4C hooks into the engine. */
extern u8 D_800C68F8[], D_800C690C[], D_800C6960[], D_8008B29C[], D_8008B418[];
extern u8 D_800C63FC[], D_800C6410[], D_800C6424[], D_800C6430[], D_800C6448[], D_800C645C[];
extern u8 D_800C64B0[], D_800C64C4[], D_800C64D8[], D_800C64E4[], D_800C64FC[], D_800C6510[];
extern u8 D_800C6564[], D_800C6578[], D_800C658C[], D_800C6598[], D_800C65B0[], D_800C65C4[];
extern u8 D_800C6618[], D_800C662C[], D_800C6640[], D_800C664C[], D_800C6664[], D_800C6678[];
s32 func_800B6DBC(SLONG_XYZ *arg0, s32 *arg1);

void func_800B8FD8();
void func_800B980C();
void func_800BBA04(s32 player);

void func_800BD99C();
void func_800BCE88();

s32 func_800BEBC0(s32 player);

void PlotRacePickUp(NAS *m);

void PlotTNTCrate(NAS *nas);

void PlotTrackGlob(NAS *nas);

void PlotDac(NAS *nas);

#endif /* BASH_OVL63_H */
