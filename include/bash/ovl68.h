#ifndef BASH_OVL68_H
#define BASH_OVL68_H

/* Functions and data defined in the ovl68 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl68/ovl68.c */

#include "common.h"
#include "bash/nas.h"

typedef struct { s16 unk0; s16 unk2; } Ovl68Trig;
extern Ovl68Trig D_80068BD4[];

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

typedef struct CircleFireExt {
    EXT ext;
    u8 pad0C[0x24];
    s32 unk30;
    u8 pad34[0x10];
} CircleFireExt;

extern u8 D_80053F8C[];
extern s32 *D_80058AD0;

typedef struct EggExt {
    EXT ext;
    u8 pad0C[4];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x18];
    s32 unk30;
    u8 pad34[0x10];
} EggExt;
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

extern u32 D_80051244[];
extern s32 D_8009D908;

typedef struct Ovl68Out {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ovl68Out;
typedef struct CoalSfxInner {
    u8 pad0[0x10];
    SLONG_XYZ pos;
    u8 pad1C[0x94 - 0x10 - sizeof(SLONG_XYZ)];
    _sfx_handler *voice;
} CoalSfxInner;

typedef struct CoalSfxObj {
    u8 pad0[0x54];
    CoalSfxInner *unk54;
    u8 pad58[0xC];
    s32 idx;
} CoalSfxObj;

extern s32 D_800BC46C[];
extern s32 D_800BC47C[];
extern s32 D_800BC48C[];
extern s32 D_800BB9FC[];
void PlotCoalFlameSFX(CoalSfxObj *obj);

void func_800BB6FC(s32 arg0, s32 arg1, s32 arg2, EXTengine *arg3, Ovl68Out *arg4);

typedef struct CoalSub {
    u8 pad0[0x74];
    u16 unk74;
} CoalSub;

typedef struct CoalEng {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    u16 unk1C;
    u8 pad1E[0x5E - 0x1E];
    u16 unk5E;
    u8 pad60[0x6C - 0x60];
    CoalSub *sub;
} CoalEng;

typedef struct CoalNAS {
    u8 pad0[0x54];
    CoalEng *eng;
    u8 pad58[4];
    u16 unk5C;
    u16 unk5E;
    s32 unk60;
} CoalNAS;

void PlotCoal(CoalNAS *nas);
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
extern u8 D_80052F5C[];
extern s32 D_800BC4E8[];
extern NAS *D_800BC420[];
extern NAS *D_800BC424[8];
extern NAS *D_800BC424[];
extern s32 D_800BC450;
NAS *func_800BB688(s32 x, s32 z, s32 range);
extern s32 D_800BC4E8[];
extern s32 D_800BC444;
extern s32 D_800BB95C[][4];
void func_800BA694(s32 arg0);
extern s16 D_8009E0D8;

void func_800BA5A0(s32 arg0, s32 arg1);
void func_800B94FC(s32 arg0, u16 arg1);
void func_800B9A50(s32 arg0, u16 arg1);
void func_800B9ED8(s32 arg0, u16 arg1);
void func_800BA220(s32 arg0, u16 arg1);
extern u8 D_8005A614[];

/* Hero jump parameters, 8 bytes per hero (D_8009D6E8.unk10). */
typedef struct Ovl68JumpEnt {
    u16 vel, max, acc, dec;
} Ovl68JumpEnt;

/* D_8009D6E8 (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct Ovl68Hooks {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Ovl68JumpEnt *unk10;
    /* 0x14 */ u8 pad14[0x58];
} Ovl68Hooks;
extern Ovl68Hooks D_8009D6E8;

void func_800B6E84(s32 arg0);
extern s32 D_8005A5D8;
extern s32 D_8005B5B8;
extern HeroInfo Heroes[4];
void func_80025F44(EXTengine *eng);
void func_80084E04(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);

s32 func_80085890();
s32 func_8008A200(s32 arg0);
void func_8008A810(void);

void func_800B44C8(void);

s32 func_800B51C8(s32 arg0);

void func_800B6168(s32 arg0);

void func_800B627C(s32 arg0);

void func_800B6350(s32 arg0);

void func_800B67D8(s32 arg0);

void func_800B68B8(s32 arg0);

void func_800B69AC(s32 arg0);

void func_800B6AAC(s32 arg0);

void func_800B722C(s32 arg0);

void InitBarrelCH(NAS *nas);

void func_800BA630(NAS *nas);

void InitPUGenerator_CH(NAS *nas);

void func_800BA70C(s32 arg0);

void func_800BA784(s32 arg0, s32 arg1);

void InitCoalFlameSFX(CoalFlameObj *arg0);

void InitEgg(NAS *nas);

void InitCircleFire(NAS *nas);

void InitCoalFlame(NAS *nas);

void InitCoal(NAS *nas);

void func_800BAC30(NAS *nas);

void InitBarrelGenerator(NAS *nas);

void func_800BAC98(s32 arg0);

void func_800BAD1C(s32 arg0);

void func_800BADD4(s32 arg0);

void func_800BAE30(s32 arg0);

s32 func_800BAE94(s32 arg0);

s32 func_800BAF00(s32 arg0);

s32 func_800BAF58(s32 arg0);

s32 func_800BAFB4(s32 arg0);

void func_800BB134(s32 arg0);

s32 func_800BB1C0(s32 x, s32 z, s32 range);

s32 func_800BB260(s32 arg0);

void func_800BB294(s32 *p, s32 arg1);

void func_800BB384(void);

void func_800BB3F0(s32 x, s32 y);

s32 func_800BB428(s32 x, s32 z);

s32 func_800BB4F8(s32 x, s32 z, s32 range, s32 skip);

s32 func_800BB5A0(s32 x, s32 z, s32 range);

#endif /* BASH_OVL68_H */
