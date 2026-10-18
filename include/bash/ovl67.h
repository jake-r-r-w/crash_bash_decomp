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

extern u16 D_800C417A;
extern u16 D_800C438A;

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
extern s32 D_8005A618;
extern s32 D_8005A680[];

s32 func_800B9CB8(s32 arg0, EXTengine *unused);

void PlotN2ODeadWall(NAS *arg0);

void func_800BB5F0(void);

void func_800BB788(s32 arg0);

#endif /* BASH_OVL67_H */
