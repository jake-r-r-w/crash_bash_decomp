#ifndef BASH_OVL61_H
#define BASH_OVL61_H

/* Functions and data defined in the ovl61 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl61/ovl61.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/engine.h"

extern HeroInfo Heroes[4];
extern char D_80054504[];

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

typedef struct Ovl61Spawn {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[2];
    s16 dir;
} Ovl61Spawn;

extern s32 *D_800C19C0[][3];
extern s32 D_800C1C68;
extern s32 D_800C1C6C;
extern u16 D_800C18E8[];
extern u16 D_800C18E0[];
extern s16 D_800C1F0C;
extern s32 D_800C271C;
extern s32 D_800C2720;
extern s32 D_800C2724;
extern s32 D_800C2728;
extern u16 D_800C1C88;
extern u16 D_800C1F14;
extern HeroInfo Heroes[4];
extern s32 D_800C1C70;
extern u16 D_800C1F14;

extern u16 D_8009D754;
typedef struct Ovl61Cnt {
    u16 n;
} Ovl61Cnt;

extern Ovl61Cnt D_8009E25C;

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

typedef struct Ovl61Gin {
    u8 pad0[0x14];
    s16 unk14;
    u8 pad16[4];
    s16 unk1A;
    s16 unk1C;
    u8 pad1E[4];
    s16 unk22;
} Ovl61Gin;

extern char D_80054574[];
extern ScriptInit D_800C14A8;
extern EXTengine *D_800C1B28;
extern Ovl61Gin *D_800C1B2C;
typedef struct Ovl61Node {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[4];
    struct Ovl61Node *next;
    u8 pad24[0x30];
    struct Ovl61Node *target;
} Ovl61Node;

extern Ovl61Circle *D_800C1A7C;
extern Ovl61Circle D_800C1A44[];
extern ScriptInit D_800C1554;

s32 func_800B4EFC(Ovl61Obj *arg0);

s32 func_800B84B8(s32 arg0);
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
void func_800B9728(void);

void func_800B69E8(s32 arg0);

void func_800B79F4(s32 arg0);

s32 func_800BFD14(Ovl61Vel *v);

s32 func_800C0250(s32 arg0, s32 arg1);

void func_800C02F8(s32 arg0);

void func_800C039C(s32 arg0);

void func_800C052C(s32 arg0);

s32 func_800C0624(s32 arg0);

void func_800C0760(s32 arg0);

s32 func_800C08C8(Ovl61Obj *arg0);

void func_800C0B94(void);

typedef struct {
    s32 pad0;
    s32 f4;
    s32 f8;
    s32 fC;
    s32 f10;
    s32 f14;
    s16 f18;
    s16 f1A;
} Ovl61Wall;

extern Ovl61Wall D_800C1E78[];
extern u16 D_800C139C[];
extern _sfx_handler *D_800C1BC4;

void func_800B6AE0(s32 arg0);

void func_800B6BE8(s32 arg0);

void func_800B6FE0(s32 arg0);

void func_800B7450(s32 arg0);

s32 func_800B8964(s32 arg0);

void func_800B99E0(void);

void func_800BD43C(u32 arg0);

s32 func_800C0804(s32 arg0, s16 arg1, s32 arg2, s16 arg3);

#endif /* BASH_OVL61_H */
