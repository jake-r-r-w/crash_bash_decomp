#ifndef BASH_OVL68_H
#define BASH_OVL68_H

/* Functions and data defined in the ovl68 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl68/ovl68.c */

#include "common.h"
#include "bash/nas.h"

typedef struct CoalFlameInner {
    s32 pad0[3];
    s32 unkC;
} CoalFlameInner;

typedef struct CoalFlameObj {
    u8 pad0[0x54];
    CoalFlameInner *unk54;
} CoalFlameObj;

typedef struct CoalFlameExt {
    EXT ext;
    u8 pad0C[0x10];
    s32 unk1C;
    u8 pad20[0x24];
} CoalFlameExt;

extern u8 D_80053F8C[];
/* PadState (D_80051338) as func_800BB260 reads it: flags at 0x54. */
typedef struct Ovl68Pad {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ s32 unk54;
} Ovl68Pad;
extern Ovl68Pad D_80051338[];

typedef struct CoalPosInner {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 z;
} CoalPosInner;

typedef struct CoalGridObj {
    u8 pad0[0x54];
    CoalPosInner *pos;
} CoalGridObj;


void func_800BB488(CoalGridObj *obj);
void func_800B3330(void);
void func_800BADA8(s32 arg0);
void func_800BAE20(void);
void func_800BAE28(void);
s32 func_800BAE84(void);
s32 func_800BAE8C(void);
void func_800BB36C(void);
void func_800BB374(void);
void func_800BB37C(void);
void func_800BB640(s32 arg0);
extern s32 D_8005AA70[11];
void func_800B3EFC(void);
void func_800B4048(void);
void func_800B4444(void);

extern u8 D_800BC210[];
extern s32 D_800BC2D0[][21];
extern s32 D_800BC4E8[];
extern NAS *D_800BC420[];
extern NAS *D_800BC424[8];
extern NAS *D_800BC424[];
extern s32 D_800BC450;
NAS *func_800BB688(s32 x, s32 z, s32 range);
extern s32 D_800BC4E8[];
extern s32 D_800BC444;
extern s16 D_8009E0D8;

void func_800BA5A0(s32 arg0, s32 arg1);
void func_800B94FC(s32 arg0, u16 arg1);
void func_800B9A50(s32 arg0, u16 arg1);
void func_800B9ED8(s32 arg0, u16 arg1);
void func_800BA220(s32 arg0, u16 arg1);
extern u8 D_8005A614[];

void func_800B6E84(s32 arg0);
extern s32 D_8005A5D8;
extern HeroInfo Heroes[4];
void func_80025F44(EXTengine *eng);
void func_80084E04(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);

s32 func_80085890();
s32 func_8008A200(s32 arg0);
void func_8008A810(void);

void func_800B44C8(void);

void func_800B627C(s32 arg0);

void func_800B6AAC(s32 arg0);

void InitBarrelCH(NAS *nas);

void func_800BA630(NAS *nas);

void InitPUGenerator_CH(NAS *nas);

void InitCoalFlameSFX(CoalFlameObj *arg0);

void InitCoalFlame(NAS *nas);

void InitCoal(NAS *nas);

void func_800BAC30(NAS *nas);

void InitBarrelGenerator(NAS *nas);

void func_800BAC98(s32 arg0);

void func_800BADD4(s32 arg0);

void func_800BAE30(s32 arg0);

void func_800BB134(s32 arg0);

s32 func_800BB1C0(s32 x, s32 z, s32 range);

s32 func_800BB260(s32 arg0);

void func_800BB384(void);

void func_800BB3F0(s32 x, s32 y);

s32 func_800BB428(s32 x, s32 z);

s32 func_800BB4F8(s32 x, s32 z, s32 range, s32 skip);

s32 func_800BB5A0(s32 x, s32 z, s32 range);

#endif /* BASH_OVL68_H */
