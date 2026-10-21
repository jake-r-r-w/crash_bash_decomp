#ifndef BASH_OVL68_H
#define BASH_OVL68_H

/* Functions and data defined in the ovl68 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl68/ovl68.c */

#include "common.h"
#include "bash/nas.h"

typedef struct { s16 unk0; s16 unk2; } Ovl68Trig;
extern Ovl68Trig D_80068BD4[];

extern NAS *D_8005AA9C;
extern s32 D_8005A5DC;

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

extern u16 *D_8009D6EC;
extern u8 D_80052A54[];
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
typedef struct Ovl68Pos {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
} Ovl68Pos;

typedef struct Ovl68Obj {
    u8 pad0[0x54];
    Ovl68Pos *pos;
} Ovl68Obj;

void func_800B3330(void);
extern s32 D_800BC240[];
void func_80029100(void *arg0);
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

/* InitPickupCH's extension 0x10000006 (0x44 bytes), its NAS.FUX. */
typedef struct PickupCHExt {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 unk18[0x44 - 0x18];
} PickupCHExt;

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
extern s32 D_800BC448;
extern s32 D_800BC44C;
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
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ Ovl68JumpEnt *unk10;
    /* 0x14 */ void *unk14;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ u8 pad28[0x10];
    /* 0x38 */ void *unk38;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ void *unk40;
    /* 0x44 */ void *unk44;
    /* 0x48 */ void *unk48;
    /* 0x4C */ u8 pad4C[8];
    /* 0x54 */ void *unk54;
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ void *unk5C;
    /* 0x60 */ u8 pad60[4];
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
} Ovl68Hooks;
extern Ovl68Hooks D_8009D6E8;
extern void *D_8009D8F4;
extern void *D_8009D8F8;
extern void *D_8009D8FC;
extern void *D_8009D900;
extern void *D_8009D904;
extern s16 D_8009E0E2;
extern s16 D_8009E0E4;
extern s16 D_8009E0E6;
extern s32 D_8005A680;
extern u8 D_800BB928[];
extern u8 D_800BB93C[];
extern u8 D_800BC098[];
extern u8 D_800BC0AC[];
extern u8 D_800BC0C0[];
extern u8 D_800BC0CC[];
extern u8 D_800BC0E4[];
extern u8 D_800BC0F8[];
s32 func_800B37C8();
s32 func_800B46BC();
s32 func_800B48E4();
s32 func_800B4B34();
s32 func_800B52C8();

void func_800B6E84(s32 arg0);
extern s32 D_8005A5D8;
extern s32 D_8005B5B8;
extern HeroInfo Heroes[4];
void func_80025F44(EXTengine *eng);
void func_80084E04(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);

s32 func_80085890();
s32 func_8008A200(s32 arg0);
void func_8008A810(void);

void func_800B3338(s32 arg0, EXTengine *arg1);

void func_800B44C8(void);

s32 func_800B459C(s32 *p);

s32 func_800B51C8(s32 arg0);

void func_800B6168(s32 arg0);

void func_800B627C(s32 arg0);

void func_800B6350(s32 arg0);

void func_800B65B0(s32 arg0);

void func_800B66A4(s32 arg0);

void func_800B67D8(s32 arg0);

void func_800B68B8(s32 arg0);

void func_800B69AC(s32 arg0);

void func_800B6AAC(s32 arg0);

void func_800B6B84(s32 arg0);

void func_800B722C(s32 arg0);

void InitBarrelCH(NAS *nas);

void InitPickupCH(NAS *nas);

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
/* PlotCircleFire: offsets of the 8 fire points around the circle, and the
 * 40x40 grid of fire NASes (128 units a cell, centred on 0). */
typedef struct { s32 x; s32 z; } CircleFireOfs;
extern CircleFireOfs D_800BC1B0[8];
NAS *SpawnNASById(s32 x, s32 y, s32 z, u16 id, u16 flags);
s32 KillNAS(NAS *nas);
typedef struct { u8 pad0[0x2C]; s32 unk2C; } CircleFireFux;
void PlotCircleFire(NAS *nas);

static inline s32 CircleFireIndex(s32 x, s32 z) {
    s32 col = (x + 0xA00) >> 7;
    s32 row = (z + 0xA00) >> 7;

    return row * 40 + col;
}
#define CIRCLEFIRE_CELL(x, z) CircleFireIndex(x, z)

static inline NAS *CircleFireCell(s32 x, s32 z) {
    if (x >= -0x9FF && x < 0xA00 && z >= -0x9FF) {
        return (NAS *)D_800BC4E8[CIRCLEFIRE_CELL(x, z)];
    }
    return NULL;
}

static inline NAS *CircleFireGridCell(s32 *grid, s32 x, s32 z) {
    if (x >= -0x9FF && x < 0xA00 && z >= -0x9FF) {
        return (NAS *)grid[CIRCLEFIRE_CELL(x, z)];
    }
    return NULL;
}

void func_800B643C(s32 arg0);

/* func_800B7B80: coal flame spreading over the circle-fire grid. */
typedef struct { u8 pad0[0x10]; s32 x; s32 pad14; s32 z; } CoalFlamePos;
typedef struct {
    u8 pad0[0x1C]; s32 unk1C; s32 unk20; s32 unk24; s32 unk28; s32 unk2C;
} CoalFlameFux;
typedef struct {
    u8 pad0[0x54]; CoalFlamePos *pos; CoalFlameFux *fux; u16 unk5C;
} CoalFlameCell;
typedef struct {
    u8 pad0[0x10]; s32 x; s32 pad14; s32 z; u8 pad1C[0x10];
    s32 sx; s32 sy; s32 sz; u8 pad38[0x26]; s16 unk5E;
} CoalFlameEng;
void func_800B5CD0(s32 arg0);

void func_800B7B80(CoalFlameEng *eng, CoalFlameFux *fux, s32 id);

void PlotPickupCH(NAS *nas);

s32 func_800BB7E0(Ovl68Obj *a, Ovl68Pos *b);

#endif /* BASH_OVL68_H */
