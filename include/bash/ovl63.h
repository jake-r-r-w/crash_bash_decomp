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
extern s32 D_80054F84;
extern HeroInfo Heroes[4];
extern ScriptInit D_800C6D74;
extern ScriptInit D_800C6DD0;
extern u8 D_800546C4[];
extern s32 D_800C6BAC;
extern u8 D_80052F94[];
extern char D_800B3400[];

void func_800B5360(s32 *arg0);

void func_800B7D94(void);

void func_800B4A4C();
void func_800B8890();
void func_800B8D68();

void PlotRaceFire(NAS *nas);

/* Shield extension, AddExtension(nas, 0x20, 0x10000000). */
typedef struct EXTraceshield {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14[2];
    /* 0x1C */ s32 unk1C;
} EXTraceshield;

/* A hero's FUX as func_800C57B4 clears it. */
typedef struct Ovl63HeroFux {
    /* 0x00 */ u8 unk0[0x8C];
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ s32 unk90;
} Ovl63HeroFux;

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

extern s32 D_800C692C;
extern s32 D_800C695C;
typedef struct Ovl63Obj74 {
    /* 0x00 */ u8 unk0[0x74];
    /* 0x74 */ s16 unk74;
} Ovl63Obj74;
extern Ovl63Obj74 *D_800C69B4;
extern s32 D_800C69B8;
extern s32 D_800C69BC;
extern s32 D_800C6BB0;
extern u16 D_800AD12C;

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

extern u8 D_80052F5C[];

extern NAS *D_800C6920;
NAS *func_800C5C64(s32 arg0);

void InitRaceMissile(NAS *m);

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
    /* 0x10 */ u8 pad10[0x51 - 0x10];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 pad52[0x5C - 0x52];
    /* 0x5C */ struct Ovl63SceneObj *next;
    /* 0x60 */ u8 pad60[0x74 - 0x60];
    /* 0x74 */ u16 index;
} Ovl63SceneObj;

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
    /* 0x68 */ u8 pad68[4];
} Ovl63Barrel;

extern Ovl63Barrel D_800C6F34[128];
extern s32 D_800C66F0;

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

s32 func_800B35CC(s32 arg0);

void func_800B8F04(s32 arg0);

void func_800BCDAC(s32 player);

s32 func_800BF0F8(u8 *arg0, u32 arg1);

void func_800C5154(s32 player);

s32 func_800C5978(s32 player);

#endif /* BASH_OVL63_H */
