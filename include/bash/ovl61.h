#ifndef BASH_OVL61_H
#define BASH_OVL61_H

/* Functions and data defined in the ovl61 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl61/ovl61.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/engine.h"

extern HeroInfo Heroes[4];

typedef struct Ovl61Vel {
    u8 pad0[0x10];
    s32 x;
    s32 pad14;
    s32 z;
} Ovl61Vel;

typedef struct Ovl61Dir {
    u8 pad0[0x16];
    s16 dir;
} Ovl61Dir;

extern s32 D_800C271C;
extern s32 D_800C2720;
extern s32 D_800C2724;
extern s32 D_800C2728;
extern u16 D_800C1C88;
extern u16 D_800C1F14;
extern HeroInfo Heroes[4];
extern s32 D_800C1C70;
extern u16 D_800C1F14;

void Plot_ArkLaserWall(NAS *nas);

typedef struct {
    s16 x;
    s16 z;
    s16 r;
} Ovl61Circle;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[0x6E];
    s16 radius;
    u8 pad8C[4];
    u32 flags;
} Ovl61Obj;

extern Ovl61Circle *D_800C1A7C;

s32 func_800B4EFC(Ovl61Obj *arg0);

void func_800B84B8(void);
void func_800BDD30(s32 arg0);

void func_800C0448(s32 arg0);

void func_800C04B8(s32 arg0);

s32 func_800C05B8(s32 arg0);

void func_800C069C(s32 arg0);

s32 func_800C096C(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2);

s32 func_800BFD9C(u8 *arg0);

u8 *func_800C01B8(void);

void func_800C04FC(s32 arg0);

s32 func_800C0A74(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2);

void func_800C0B7C(void);

void func_800C0B84(void);

void func_800C0B8C(void);

extern char func_800B36CC[];
extern char func_800B3B20[];
extern char func_800B40AC[];
extern char func_800B9728[];

s32 func_800C0624(s32 arg0);

void func_800C0B94(void);

#endif /* BASH_OVL61_H */
