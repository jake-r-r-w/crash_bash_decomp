#ifndef BASH_OVL6A_800B4EAC_H
#define BASH_OVL6A_800B4EAC_H

/* Functions and data defined in src/overlays/ovl6a/800b4eac.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl6a/800b4eac.c */

#include "common.h"
#include "bash/ovl6a.h"

typedef struct Ovl6aSpawnExt {
    u8 unk0[0xC];
    s32 *unkC;
    u8 unk10[0x34 - 0x10];
    s32 unk34;
} Ovl6aSpawnExt;
extern s32 D_800BD5A8;
void func_800BBE24();

extern s32 D_800BF260;
extern s32 D_800BCEF0;
extern EXTengine D_800BF100;
extern s32 D_800BF19C;
extern s32 D_800BF1A0;
extern s32 D_800BF1A4;
typedef struct Ovl6aEntry12 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ovl6aEntry12;
extern Ovl6aEntry12 D_800BD7E0[];
typedef struct PodiumFx {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x52 - 0x16];
    /* 0x52 */ s16 unk52;
} PodiumFx;
typedef struct TrophyInner {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[0x5E - 0x38];
    /* 0x5E */ u16 unk5E;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ PodiumFx *fx;
} TrophyInner;
typedef struct PodiumTrophy {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ TrophyInner *inner;
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ u16 state;
} PodiumTrophy;
#define PODIUM_SIN(k) D_80068BD4[((((D_8005B690 * (k)) & 0xFFF) - 0x400) & 0xFFF)].unk2
extern u32 D_800BD5B0;
extern u8 D_80055924[];
extern u8 D_8005595C[];
extern u8 D_80055994[];
extern u8 D_800559CC[];
extern u8 D_80055A04[];
extern u8 D_80055A3C[];
extern u8 D_80055A74[];
extern u8 D_80055AAC[];
void func_800B8AA4(void);
extern s32 D_800BD5B4;
extern s32 D_800BD598[];
extern u8 D_800BD5B8[];
extern u8 D_800BDB14[];
extern u8 D_800BF1A8[];

/* D_8005A614 as this file sees it: per-player byte flags at 0xE4.. */
typedef struct Ovl6aGameView {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ u8 padC[0x24 - 0xC];
    /* 0x024 */ s16 unk24;
    /* 0x026 */ u8 pad26[0x30 - 0x26];
    /* 0x030 */ u16 unk30;
    /* 0x032 */ u16 unk32;
    /* 0x034 */ u8 pad34[0x3C - 0x34];
    /* 0x03C */ u16 unk3C;
    /* 0x03E */ u8 pad3E[0x4C - 0x3E];
    /* 0x04C */ u8 unk4C;
    /* 0x04D */ u8 pad4D[3];
    /* 0x050 */ u8 unk50;
    /* 0x051 */ u8 unk51;
    /* 0x052 */ u8 unk52;
    /* 0x053 */ u8 unk53;
    /* 0x054 */ u8 pad54[0xE0 - 0x54];
    /* 0x0E0 */ s32 unkE0;
    /* 0x0E4 */ u8 unkE4[0x21];
    /* 0x105 */ u8 unk105[0x21];
    /* 0x126 */ u8 unk126[0x21];
    /* 0x147 */ u8 unk147[0x21];
} Ovl6aGameView;
typedef struct KeyedPair {
    s32 key;
    s32 val[1];
} KeyedPair;
extern KeyedPair D_800BCB54[];
/* A POLY_FT4 as func_800B9E90 fills it. */
typedef struct Ovl6aFt4 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0, g0, b0, code;
    /* 0x08 */ s16 x0, y0;
    /* 0x0C */ u16 uv0;
    /* 0x0E */ u16 clut;
    /* 0x10 */ s16 x1, y1;
    /* 0x14 */ u16 uv1;
    /* 0x16 */ u16 tpage;
    /* 0x18 */ s16 x2, y2;
    /* 0x1C */ u16 uv2;
    /* 0x1E */ u16 pad2;
    /* 0x20 */ s16 x3, y3;
    /* 0x24 */ u16 uv3;
    /* 0x26 */ u16 pad3;
} Ovl6aFt4;

/* One row of 8 particles func_800B9E90 sets up (0x140 bytes). */
typedef struct Ovl6aSparkRow {
    /* 0x000 */ SVECTOR rot[8];
    /* 0x040 */ s32 unk40[8];
    /* 0x060 */ u8 pad60[0x40];
    /* 0x0A0 */ SVECTOR unkA0[8];
    /* 0x0E0 */ SVECTOR unkE0[8];
    /* 0x120 */ s32 unk120;
    /* 0x124 */ s32 unk124;
    /* 0x128 */ s32 unk128;
    /* 0x12C */ s32 unk12C;
    /* 0x130 */ s32 unk130;
    /* 0x134 */ s32 unk134;
    /* 0x138 */ s32 unk138;
    /* 0x13C */ s32 unk13C;
} Ovl6aSparkRow;

typedef struct Ovl6aSparkSet {
    /* 0x0 */ Ovl6aSparkRow *rows;
    /* 0x4 */ Ovl6aFt4 *src;
    /* 0x8 */ Ovl6aFt4 *dst;
    /* 0xC */ s32 count;
} Ovl6aSparkSet;

typedef struct Ovl6aSparks {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ Ovl6aSparkSet set;
} Ovl6aSparks;

/* A texture entry: the POLY_FT4 fields func_800B9E90 copies. */
typedef struct Ovl6aTex {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ u16 tpage;
    /* 0x26 */ u16 clut;
    /* 0x28 */ u16 uv0;
    /* 0x2A */ u16 uv1;
    /* 0x2C */ u16 uv2;
    /* 0x2E */ u16 uv3;
} Ovl6aTex;

void func_800B9E90(Ovl6aSparks *arg0);

extern s32 D_800BD390;
extern s32 D_800BD394;
extern s32 D_800BCEF4;
extern u16 D_8009EEA0;
extern EXTengine *D_8009D530;
extern CamEase D_800BCBAC;

typedef struct DistPos {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
} DistPos;
extern EngRec24 D_800BCA20[];
extern s32 D_800BCEFC;

void func_800B6224(s32 arg0);

typedef struct PodiumChild {
    u8 pad0[0xC];
    s32 flags;
    u8 pad10[0x1C];
    s32 f2C;
    s32 f30;
    s32 f34;
    u8 pad38[0x24];
    u8 f5C;
    u8 pad5D;
    s16 f5E;
} PodiumChild;

typedef struct PodiumObj {
    u8 pad0[0x54];
    PodiumChild *child;
    u8 pad58[4];
    u16 f5C;
} PodiumObj;

extern PodiumObj *D_800BD594;
extern char D_8005572C;

typedef struct Ovl6aSpawnEnt {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 z;
    /* 0x8 */ u8 kind;
    /* 0x9 */ u8 flags;
} Ovl6aSpawnEnt;
extern NAS *D_800BD584[];
extern u16 D_800BD1F0[];
extern s32 D_800BD200[];

void func_800B7874(void);

typedef struct FxInner {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0xA];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x24];
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad54[0x18];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ s16 unk74;
} FxInner;

typedef struct FxMid {
    u8 pad0[0x6C];
    FxInner *inner;
} FxMid;

typedef struct FxObj {
    u8 pad0[0x54];
    FxMid *mid;
    u8 pad58[8];
    s32 unk60;
} FxObj;

void func_800B7988(s32 *arg0, s32 flag, s32 *out);

void func_800B7A78(void);

s32 func_800B7AB4(s32 arg0);

u16 func_800B7B38(void);

void func_800B7B7C(void);

void func_800B7BA4(void);

void func_800B7C78(void);

void func_800B7C98(void);

void func_800B7CA8(void);

void func_800B7D7C(void);

void func_800B7D8C(void);

void func_800B7DB4(u8 *arg0, s32 arg1);

void PlotGatewayTrophy(void);

void InitGatewayTrophy(void);

s32 func_800B7F24(DistPos *a, DistPos *b);

s32 func_800B7F74(u8 *arg0, u8 *arg1);

void func_800B7FAC(s32 arg0);

void func_800B7FD0(u8 *cam, s32 arg1);

void func_800B8148(s32 arg0);

s32 func_800B821C(void);

s32 func_800B8224(void);

void func_800B822C(s32 arg0);
void func_800B5710(s32 arg0);

void func_800B8284(void);

void func_800B8324(s32 arg0);

void func_800B838C(s32 arg0);

void func_800B83D4(s32 arg0);

void func_800B8474(s32 arg0);

s32 func_800B84CC(void);

void func_800B8588(void);

void func_800B85A8(void);

void func_800B85E4(void);

s32 func_800B8708(s32 arg0);

void func_800B8CA0(void);

void func_800B8F50(void);

void PlotPodiumOdo(NAS *arg0);

void func_800BA7BC(NAS *arg0);

void func_800BA804(void);

void func_800BA848(void);

void func_800BA88C(void);

void InitPodiumTrophy(NAS *arg0);

void InitPodiumOdo(u8 *arg0);

typedef struct ScriptExt40 {
    /* 0x00 */ u8 pad00[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[8];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ ScriptCursor *cursor;
    /* 0x30 */ u8 pad30[8];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} ScriptExt40;

extern ScriptInit D_800BCCE8;

s32 func_800B694C(void);

void func_800B77B0(NAS *nas);

void func_800B787C(FxObj *arg0);

void func_800B7E14(s32 *arg0, s32 arg1);

s32 func_800B8174(s32 arg0);

void func_800B8BC8(void);

void func_800BAAFC(void);

void func_800BAB40(void);

void func_800BB280(void);

void func_800BB364(void);

void func_800BB38C(void);

void func_800BB3D0(s8 arg0);
typedef struct SubInit {
    s16 a;
    s16 b;
    s32 c;
    s32 pad[2];
} SubInit;

typedef struct SubInitList {
    SubInit *items;
    s32 count;
} SubInitList;

extern subengine D_800BDB58[];

void func_800BB4AC(SubInitList *list);

void func_800BB584(void);

void func_800BBE1C(void);

typedef struct Ovl6aRenderSub {
    u8 padC[0xC];
    s32 unkC;
} Ovl6aRenderSub;

typedef struct Ovl6aRenderObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x68 - 0x4];
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ Ovl6aRenderSub *sub;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ u16 unk74;
} Ovl6aRenderObj;

extern u8 D_800BF264[];
extern u8 D_800C7264[];
void func_800B5158(void);

void func_800BB5F0(Ovl6aRenderObj *obj, s16 *out);
void func_800BBC64(s16 *a, void *b, s32 c, s32 d);
void func_800BBE24(Ovl6aRenderObj *obj, void *arg1);

void func_800BBF44(SVECTOR *v, s16 *b);

typedef struct PodiumInner {
    u8 pad0[0x6C];
    u8 *script;
} PodiumInner;

typedef struct PodiumMid {
    u8 pad0[0xC];
    u32 flags;
    u8 pad10[0x5C];
    PodiumInner *inner;
} PodiumMid;

typedef struct PodiumScriptObj {
    u8 pad0[0x54];
    PodiumMid *mid;
    u8 pad58[0xC];
    s32 unk64;
} PodiumScriptObj;

extern ScriptInit D_800BCF68;
extern ScriptInit D_800BCFB8;
extern ScriptInit D_800BD008;
extern ScriptInit D_800BD070;
extern ScriptInit D_800BD0D8;
extern ScriptInit D_800BD134;
extern ScriptInit D_800BD18C;
extern ScriptInit D_800BD1E4;

void func_800B93B4(s32 arg0, s32 arg1);
/* D_8005A614 as seen by func_800B6A3C. */
typedef struct Ovl6aSelView {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u8 pad36[0x62 - 0x36];
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
} Ovl6aSelView;
/* 8-byte entries of D_8009EDD4[i][3]. */
typedef struct Ovl6aSelEntry {
    /* 0x0 */ u16 unk0;
    /* 0x2 */ u16 unk2;
    /* 0x4 */ u16 unk4;
    /* 0x6 */ u16 unk6;
} Ovl6aSelEntry;
extern u8 D_800BCEDC;
extern s32 D_800BCEE4;

void func_800B6A3C(void);

void func_800B6E20(void);

void PlotPodiumTrophy(PodiumTrophy *arg0);

void func_800BA9C8(s32 arg0, Ovl6aSpawnEnt *arg1);

#endif /* BASH_OVL6A_800B4EAC_H */
