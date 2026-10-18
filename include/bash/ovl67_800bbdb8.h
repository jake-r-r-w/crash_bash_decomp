#ifndef BASH_OVL67_800BBDB8_H
#define BASH_OVL67_800BBDB8_H

/* Functions and data defined in src/overlays/ovl67/800bbdb8.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl67/800bbdb8.c */

#include "common.h"
#include "bash/ovl67.h"

typedef struct FollowCam {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ u8 pad18[0x3C];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 pad56;
    /* 0x58 */ s16 unk58;
} FollowCam;

typedef struct MineSrc {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ s32 unk18;
} MineSrc;

typedef struct MineSFX {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ MineSrc *src;
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ s16 unk5C;
    /* 0x5E */ u8 pad5E[2];
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
} MineSFX;
typedef struct LaserSub {
    /* 0x00 */ u8 pad00[0x6C];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ s16 unk74;
} LaserSub;

typedef struct LaserObj {
    /* 0x00 */ u8 pad00[0x1E];
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 pad20[0x3D];
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 pad5E[0x0E];
    /* 0x6C */ LaserSub *sub;
} LaserObj;

typedef struct LaserShoot {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ LaserObj *obj;
    /* 0x58 */ u8 pad58[8];
    /* 0x60 */ s32 unk60;
} LaserShoot;
extern u8 *D_800C449C;
extern s32 D_800C4478;
extern s32 D_800C4480;
extern s32 D_800C4488;
extern s32 D_800C448C;
extern s32 D_800C44FC;
extern s32 D_800C4500;
extern s32 D_800C4504;
extern s32 D_800C4508;
extern ScriptInit D_800C44F0;

void func_800BC6B8(void);

void InitMineSFX(MineSFX *arg0);

void InitLaserShoot(LaserShoot *arg0);

void func_800C2904(s32 *arg0, s32 flag, s32 *out);

s32 func_800C2D10(s32 arg0);

void func_800C2D80(s32 arg0);

void func_800C2E58(void);

s32 func_800C2F70(void);

void InitDummyCamera(NAS *nas);

void func_800C3184(void);

void func_800C3234(void);

void func_800C3370(void);

void func_800C3440(void);

void func_800C36D4(void);

void func_800C36DC(void);

void PlotNitroSmokeBoost(NAS *nas);

void func_800C318C(s32 arg0);

void InitNitroSmokeBoost(NAS *nas);
extern s32 D_800C447C;

void func_800C27D4(void);

void func_800C2BB0(void);

void func_800C2C08(s32 arg0);

void func_800C2C8C(s32 arg0);

void func_800C2EF8(s32 arg0);

s32 func_800C365C(void);

#endif /* BASH_OVL67_800BBDB8_H */
