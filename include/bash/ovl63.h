#ifndef BASH_OVL63_H
#define BASH_OVL63_H

/* Functions and data defined in the ovl63 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl63/ovl63.c */

#include "common.h"
#include "bash/nas.h"

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

void InitRaceShield(NAS *nas);
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

void InitRaceMissile(NAS *m);

void InitRaceSmokeBoostWater(NAS *nas);

void InitRaceGreenSmokeBoost(NAS *nas);

extern ScriptInit D_800C6F28;

s32 func_800C5400(s32 arg0);

void func_800C5B08(s32 *pos, s32 *polar);

#endif /* BASH_OVL63_H */
