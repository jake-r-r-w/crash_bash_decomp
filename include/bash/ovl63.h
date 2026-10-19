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

void func_800B5360(s32 *arg0);

void func_800B7D94(void);

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

void func_800C5204(EXTheromove *a, EXTheromove *b);

extern HeroInfo Heroes[4];
void SetHeroAction(HeroInfo *hero, s32 action);
s32 func_8008CEB0(u8 *arg0, u8 *arg1);

s32 func_800C4F30(u8 *arg0);

#endif /* BASH_OVL63_H */
