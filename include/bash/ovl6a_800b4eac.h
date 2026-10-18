#ifndef BASH_OVL6A_800B4EAC_H
#define BASH_OVL6A_800B4EAC_H

/* Functions and data defined in src/overlays/ovl6a/800b4eac.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl6a/800b4eac.c */

#include "common.h"
#include "bash/ovl6a.h"

extern s32 D_800BD5A8;
extern s32 D_800BF260;
extern EXTengine D_800BF100;
extern s32 D_800BF19C;
extern s32 D_800BF1A0;
extern u32 D_800BD5B0;
extern s32 D_800BD5B4;

/* D_8005A614 as this file sees it: per-player byte flags at 0xE4.. */
typedef struct Ovl6aGameView {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ s32 unk4;
    /* 0x008 */ u8 pad8[0x24 - 0x8];
    /* 0x024 */ s16 unk24;
    /* 0x026 */ u8 pad26[0x30 - 0x26];
    /* 0x030 */ u16 unk30;
    /* 0x032 */ u16 unk32;
    /* 0x034 */ u8 pad34[0x4C - 0x34];
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

void func_800B7874(void);

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

void func_800B77B0(NAS *nas);

void func_800B8BC8(void);

void func_800BAAFC(void);

void func_800BAB40(void);

void func_800BB364(void);

void func_800BB38C(void);

void func_800BB584(void);

void func_800BBE1C(void);

void func_800BBF44(SVECTOR *v, s16 *b);

#endif /* BASH_OVL6A_800B4EAC_H */
