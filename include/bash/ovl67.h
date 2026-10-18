#ifndef BASH_OVL67_H
#define BASH_OVL67_H

/* Functions and data defined in the ovl67 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl67/ovl67.c */

#include "common.h"
#include "bash/nas.h"

s32 func_800BB05C(u8 *arg0);
u8 *func_800BB624(void);
void func_800BBD18(void);
void func_800B34A4(void);
void func_800B361C(void);

extern u16 *D_8009D6EC;

extern u8 D_80053EAC[];
extern EXTengine *D_8009D530;
extern u16 D_800C39A0[];
extern s16 D_800C4150[][3];
extern u16 D_800C417A;
extern u16 D_800C438A;
extern s16 D_800C4184[];
extern s32 (*D_8009D750)(EXTengine *);

/* PadState (D_80051338) as func_800B447C reads it: the buttons at 0x50. */
typedef struct Ovl67Pad {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x50 - 0x04];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x58 - 0x54];
} Ovl67Pad;

/* PadState as func_800B5064 reads it: flags at 0x4C and 0x54. */
typedef struct Ovl67PadFlags {
    /* 0x00 */ u8 pad0[0x4C];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl67PadFlags;

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl67_800bbdb8.h"

void func_800BBA4C(s32 arg0);
extern HeroInfo Heroes[4];

void func_800B59A8(s32 arg0);
extern char D_80053EE4[];
s32 csqrt(s32 x);

NAS *func_8007A524(s32 arg0, s32 *arg1, s32 arg2);
void func_800898EC(s32 arg0);

void func_800B9800(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r);

s32 func_800BBBB4(s32 arg0, s16 arg1, s32 arg2, s16 arg3);

/* Each player's reach box for hitting the ball (func_800B9CB8): the y range
 * relative to the hero, the near and far radii, and the near and far points. */
typedef struct Ovl67HeroBox {
    s32 a, b, c, d;
    s32 x0, y0, z0;
    s32 x1, y1, z1;
} Ovl67HeroBox;

extern Ovl67HeroBox D_800C4190[];

/* Extension 0x800 of a ball (func_800B9CB8), as in ovl61's Ovl61Puck. */
typedef struct Ovl67Puck {
    u8 pad0[0xE];
    s16 speed;
    u8 pad10[4];
    s16 max;
    s16 dir;
    u8 pad18[0x32 - 0x18];
    u16 unk32;
} Ovl67Puck;

extern u16 D_800C39B0[];
extern u16 D_800C39DC[];
extern s32 D_8005A618;
extern s32 D_8005A680[];

s32 func_800B9CB8(s32 arg0, EXTengine *unused);

typedef struct Ovl67Vel {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
} Ovl67Vel;

typedef struct Ovl67Out {
    u8 pad0[0x16];
    s16 dir;
} Ovl67Out;

extern u16 D_800C39B0[];
extern s32 D_8005A618;
extern s32 D_8005A680[];

s32 func_800B9CB8(s32 arg0, EXTengine *unused);

void func_800B447C(s32 arg0);

s32 func_800BAEA8(Ovl67Vel *arg0, s32 arg1);

void PlotN2ODeadWall(NAS *arg0);

void func_800BB5F0(void);

void func_800BB72C(s32 arg0);

void func_800BB788(s32 arg0);
/* Round state cleared by func_800B361C. */
typedef struct Ovl67Round {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} Ovl67Round;

typedef struct Ovl67Scene {
    u8 pad0[0x27];
    u8 unk27;
} Ovl67Scene;

typedef struct Ovl67FlagNode {
    u32 flags;
    u8 pad4[0x58];
    struct Ovl67FlagNode *next;
    u8 pad60[0xC];
    u8 *unk6C;
    u8 pad70[4];
    u16 unk74;
} Ovl67FlagNode;

typedef struct Ovl67SceneRef {
    u8 pad0[0x10];
    Ovl67Scene *unk10;
    u8 pad14[8];
    Ovl67FlagNode *unk1C;
} Ovl67SceneRef;

extern Ovl67Round D_800C4398;
extern s32 D_8005AA3C;
extern s32 D_800C4080;
extern NAS *D_800C4084;
typedef struct {
    s16 x;
    s16 z;
    s16 r;
} Ovl67Circle;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[0x6E];
    s16 radius;
    u8 pad8C[4];
    u32 flags;
} Ovl67Obj;

extern Ovl67Circle *D_800C418C;
extern u8 D_800C37DC[];

typedef struct Ovl67Slot {
    struct { u8 pad0[8]; s32 y; } *src;
    u8 pad4[4];
    NAS *nas;
    u8 padC[0xE];
    u16 kind;
    u8 pad1C[0xC];
} Ovl67Slot;
extern Ovl67Slot D_800C42D0[];
extern char D_80054504[];
extern s16 D_800C4370;
extern s16 D_800C4372;
extern s16 D_800C4374;
extern s16 D_800C4376;
extern s16 D_800C4378;
extern s16 D_800C437A;
extern s16 D_800C437C;
extern s16 D_800C437E;
extern s16 D_800C4380;
extern s16 D_800C4382;
extern s16 D_800C4384;
extern s32 D_800C438C;
extern s32 D_800C4390;
extern s16 D_800C4394;
extern s16 D_800C43DC;
void func_800B6F4C(void);
void func_800B90F4(void);
void func_800BB070(u16 arg0);
void InitN2OmissileExplode(NAS *nas);

void func_800C349C(void);

s32 func_800BB8EC(s32 arg0);

void func_800BBD68(void);
/* A ball in the D_8005AA38 list (func_800B817C), as in ovl61's Ovl61Node. */
typedef struct Ovl67Node {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[4];
    struct Ovl67Node *next;
    u8 pad24[0x30];
    struct Ovl67Node *target;
    struct {
        u8 pad0[0x14];
        u16 unk14;
    } *unk58;
} Ovl67Node;

typedef struct Ovl67Pos {
    s32 x;
    s32 y;
    s32 z;
} Ovl67Pos;

s32 func_800B3C04(Ovl67Obj *arg0);

void func_800B4FA4(s32 arg0);

void func_800B5064(s32 arg0);

void func_800B5674(s32 arg0);

void func_800B5774(s32 arg0);

void func_800B5B14(s32 arg0);

void func_800B5BFC(void);

s32 func_800B7C58(s32 arg0, Ovl67Pos *from, Ovl67Pos *to, s32 angle);

s32 func_800B817C(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, s32 arg5);

void func_800B8968(s32 a);

void func_800BB184(void);

void PlotN2OmissileExplode(NAS *nas);

void func_800BB3A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void InitN2Ofirstmissile(NAS *nas);

void InitN2Omissile(NAS *nas);

void func_800BB6BC(s32 arg0);

void func_800BB7D4(s32 arg0);

void func_800BB86C(s32 arg0);

s32 func_800BB944(s32 arg0);

s32 func_800BB9B0(s32 arg0);

void func_800BBB10(s32 arg0);

s32 func_800BBC78(Ovl67Vel *arg0, Ovl67Out *arg1, s32 arg2);

extern s8 D_8005A689;

void func_800B5440(s32 arg0);

#endif /* BASH_OVL67_H */
