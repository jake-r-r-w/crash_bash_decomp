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

typedef struct SetupCam {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x1C];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[0x10];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ u8 pad5A[0xA];
    /* 0x64 */ s32 sx, sy, sz;
    /* 0x70 */ u8 pad70[0x24];
    /* 0x94 */ u8 unk94;
    /* 0x95 */ u8 unk95;
} SetupCam;

typedef struct MixObj {
    /* 0x00 */ u8 pad00[0x20];
    /* 0x20 */ s32 a;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ s32 b;
    /* 0x34 */ u8 pad34[0x1A];
    /* 0x4E */ s16 w;
} MixObj;

extern SetupCam D_800C7614;
extern u8 D_8009F644[];
extern u8 D_8009F6F4[];

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
typedef struct LaserPos {
    s32 x;
    s32 y;
    s32 z;
} LaserPos;

typedef struct LaserTgt {
    /* 0x00 */ u8 pad00[0x10];
    /* 0x10 */ LaserPos pos;
} LaserTgt;

typedef struct LaserNode {
    /* 0x00 */ u8 pad00[0x20];
    /* 0x20 */ struct LaserNode *next;
    /* 0x24 */ u8 pad24[0x30];
    /* 0x54 */ LaserTgt *tgt;
} LaserNode;

typedef struct LaserExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 unk28;
} LaserExt;

extern u8 *D_800C449C;
extern NAS *D_800C4490;
extern NAS *D_800C4494;
extern s32 D_800C4478;
extern void (*D_8009F8B4)(void);
extern s32 D_800C4498;
extern s32 D_800C45F0;

typedef struct MineModel {
    /* 0x00 */ u8 pad00[0x6C];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ s16 unk74;
} MineModel;

typedef struct MineObj {
    /* 0x00 */ u8 pad00[0x1C];
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ u8 pad22[0x3C];
    /* 0x5E */ s16 unk5E;
    /* 0x60 */ u8 pad60[0xC];
    /* 0x6C */ MineModel *model;
} MineObj;

typedef struct MissileObj {
    /* 0x00 */ u8 pad00[0x5D];
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 pad5E[0xE];
    /* 0x6C */ MineModel *model;
} MissileObj;

typedef struct MissileHolder {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ MissileObj *obj;
} MissileHolder;

typedef struct MissileExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} MissileExt;

typedef struct MineHolder {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ MineObj *obj;
} MineHolder;
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

void InitNitroMine(MineHolder *arg0);

void func_800C2BB0(void);

void func_800C2C08(s32 arg0);

void func_800C2C8C(s32 arg0);

void func_800C2DD8(s32 arg0);

void func_800C2E60(s32 arg0);

void func_800C2EF8(s32 arg0);

void func_800C2F78(MixObj *arg0, MixObj *arg1);

void PlotDummyCamera(NAS *nas);

s32 func_800C365C(void);

typedef struct BoxHalf {
    s16 v[3];
} BoxHalf;

typedef struct Box2 {
    BoxHalf lo;
    BoxHalf hi;
} Box2;

typedef struct AngObj {
    /* 0x00 */ char pad0[0x10];
    /* 0x10 */ u16 ax;
    /* 0x12 */ u16 ay;
    /* 0x14 */ char pad14[0x3D];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ char pad54[0x20];
    /* 0x74 */ u16 state;
} AngObj;

extern char D_800B336C[];

void func_800BD584(AngObj *o);

void InitNitroMissile(MissileHolder *arg0);

s32 func_800C323C(LaserPos p, s32 r);
typedef struct BossScene {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad04[0x28];
    /* 0x2C */ u8 *unk2C;
} BossScene;

typedef struct BossExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ ScriptCursor *cursor;
    /* 0x10 */ KeyBlock *keys;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
} BossExt;

extern s16 D_8009D6E2;
extern ScriptInit D_800C446C;
extern s32 D_800C4530;

void func_800BE8C0(s32 arg0);

void func_800BF7C8(s32 arg0);

void InitNitroBoss(NAS *nas);

s32 func_800C3544(s32 *arg0, s32 *arg1);

#endif /* BASH_OVL67_800BBDB8_H */
