#ifndef BASH_OVL62_H
#define BASH_OVL62_H

/* Functions and data defined in the ovl62 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl62/ovl62.c */

#include "common.h"
#include "bash/nas.h"

void func_800CAD0C(void);
void func_800CAD14(void);
void func_800CAD1C(void);
s32 func_800CAFF0(void);
void func_800CB124(void);
void func_800CB12C(void);
void func_800CB6B4(void);
void func_800CB6BC(void);
s32 func_800CB6C4(s32 arg0);
s32 func_800CB744(u8 *arg0, u8 *arg1);
u32 func_800CBA28(u8 *arg0);
s32 func_800CBA5C(void);
void func_800CF9EC(s32 player);
s32 func_800D2D2C();
void func_800D3170(void);
void func_800D34F0(void);
void func_800D34F8(void);
void func_800D3500(void);
void func_800D3508(void);

extern NAS *D_800D5294;

typedef struct Ext10000000 {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 unkC;
} Ext10000000;

typedef struct Ext20 {
    /* 0x00 */ u8 pad[0x20];
    /* 0x20 */ s32 val;
} Ext20;

extern HeroInfo Heroes[4];

/* Head of a list linked through +0x20 (counted by func_800C9B5C). */
extern u8 *D_800D5454;
extern HeroInfo Heroes[4];
extern s32 D_8005A680[];

s32 func_800C9B5C(void);

void func_800BFD38();

typedef struct GravityObject {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 target;
} GravityObject;

void PlotGravityObject(GravityObject *obj);
/* A hero's FUX extension as func_800B9714 sets it up. */
typedef struct Ovl62Fux {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ SLONG_XYZ vel;
    /* 0x18 */ SLONG_XYZ pos;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x30 - 0x28];
    /* 0x30 */ s32 state;
    /* 0x34 */ u8 pad34[0x44 - 0x34];
    /* 0x44 */ void *func;
} Ovl62Fux;

extern s32 D_800D4E0C[];
void func_800B7520();
s32 func_800B9714(s32 player);

s32 func_800CD118(s32 *arg0, s32 arg1);

void func_800D2EB0(s32 player);

void func_800D325C(s32 arg0);

/* Bomb maker extension (0x1C bytes, id 0x10000000). */
typedef struct BombMakerExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 timer;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} BombMakerExt;

/* The NAS that owns the bomb maker. */
extern NAS *D_800D52B4;

s32 func_800C9C98(u8 *arg0);

NAS *func_800CB5F4(void);

void func_800D3018(s32 player);

#endif /* BASH_OVL62_H */
