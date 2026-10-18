#ifndef BASH_OVL61_H
#define BASH_OVL61_H

/* Functions and data defined in the ovl61 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl61/ovl61.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/engine.h"
#include "bash/8007ee2c.h"

extern HeroInfo Heroes[4];
extern char D_80054504[];

typedef struct Ovl61HeroBox {
    s32 a, b, c, d;
    s32 x0, y0, z0;
    s32 x1, y1, z1;
} Ovl61HeroBox;

extern Ovl61HeroBox D_800C1A88[];

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
extern u16 *D_8009D6EC;
extern s32 D_800C1C68;
extern s32 D_800C1C6C;
extern u16 D_800C18E8[];
extern u16 D_800C18E0[];
extern s16 D_800C1F0C;
extern s32 D_800C271C;
extern s32 D_800C2720;
extern s32 D_800C2724;
extern s32 D_800C1C84;
extern s32 D_800C2728;
extern s32 D_800C1C84;
extern u16 D_800C1C88;
extern u16 D_800C1F14;
extern HeroInfo Heroes[4];
extern s32 D_800C1C70;
extern u16 D_800C13AC[][5];
extern u16 D_800C1EE8[];
extern s16 D_800C1EF0[];
extern s32 D_800C18A8[4][3];
extern char D_8005453C[];
extern u16 D_800C1F14;

extern u16 D_8009D754;
typedef struct Ovl61Cnt {
    u16 n;
} Ovl61Cnt;

extern Ovl61Cnt D_8009E25C;
extern s32 D_800C1C80;
extern u16 D_8009E46C[];

typedef struct Ovl61FuxType {
    u8 pad0[0x14];
    u16 type;
} Ovl61FuxType;

void func_800BC0D8(void);
void func_800BCA30(void);
void func_800BD510(void);

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
    u8 pad1C[2];
    s16 dir;
    u8 pad20[0x6A];
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
    u8 pad1E[2];
    s16 unk20;
    s16 unk22;
} Ovl61Gin;

/* D_8005A614 as seen by Plot_N_Gin. */
typedef struct {
    u8 pad0[0x32];
    u16 unk32;
    u8 pad34[0x6C - 0x34];
    s32 unk6C;
} Ovl61GinView;

extern s16 D_800C1EFC;

extern char D_80054574[];
extern ScriptInit D_800C14A8;
extern EXTengine *D_800C1B28;
extern s32 (*D_8009D750)(EXTengine *);
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

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
    s16 state;
} Ovl61Slot;

extern Ovl61Slot D_800C1E28[];

void func_800BE1C4(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r);

s32 func_800B4880(Ovl61Obj *arg0, s32 arg1);

extern u16 D_800C1EFE;

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

void func_800B36CC(void);
void func_800B8A68();
extern u8 D_800C0E70[];
extern u8 D_800C1054[];
extern void *D_800C1C8C;
extern void *D_800C1C90;
extern void *D_800C1C94;
extern u8 D_800C0E84[];
extern u8 D_800C0E98[];
extern u8 D_800C0EAC[];
extern u8 D_800C0EB8[];
extern u8 D_800C0ED0[];
extern u8 D_800C0EE4[];
extern u8 D_800C0EF8[];
extern u8 D_800C0F0C[];
extern u8 D_800C0F20[];
extern u8 D_800C0F2C[];
extern u8 D_800C0F44[];
extern u8 D_800C0F58[];
extern u8 D_800C0F6C[];
extern u8 D_800C0F80[];
extern u8 D_800C0F94[];
extern u8 D_800C0FA0[];
extern u8 D_800C0FB8[];
extern u8 D_800C0FCC[];
extern u8 D_800C0FE0[];
extern u8 D_800C0FF4[];
extern u8 D_800C1008[];
extern u8 D_800C1014[];
extern u8 D_800C102C[];
extern u8 D_800C1040[];
extern char func_800B3B20[];
void func_800B40AC(void);
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
    u16 f18;
    u16 f1A;
} Ovl61Wall;

extern Ovl61Wall D_800C1E78[];

typedef struct {
    u8 pad0[0x74];
    u16 unk74;
} Ovl61WallSub;

typedef struct {
    Ovl61WallSub *sub;
    u8 pad4[0xA - 0x4];
    u16 unkA;
    u8 padC[0xE - 0xC];
    u16 unkE;
    u8 pad10[0x14 - 0x10];
} Ovl61WallObj;

extern Ovl61WallObj D_800C1DD8[];

typedef struct {
    u32 *flags;
    Ovl61WallSub *sub;
    u8 pad8[0xE - 0x8];
    u16 unkE;
    u8 pad10[0x14 - 0x10];
} Ovl61Door;

extern Ovl61Door D_800C1D38[];
extern Ovl61Door D_800C1D88[];
extern _sfx_handler *D_800C1C60;

typedef struct {
    u8 pad0[0x30];
    u16 unk30;
    u8 pad32[2];
    s16 unk34;
} Ovl61ModeView;
extern u16 D_800C139C[];
extern _sfx_handler *D_800C1BC4;

typedef struct Ovl61Eng {
    u8 pad0[0xC];
    u32 flags;
    s32 x;
    s32 pad14;
    s32 z;
    u8 pad1C[0x7C - 0x1C];
    struct Ovl61Eng *sub;
} Ovl61Eng;

typedef struct Ovl61Fux {
    u8 pad0[0x1A];
    s16 flag;
} Ovl61Fux;

typedef struct Ovl61Mot {
    u8 pad0[0xE];
    s16 a;
    s16 b;
    s16 c;
    s16 lim;
    s16 dir;
} Ovl61Mot;

void func_800B3528(s32 arg0);

s32 func_800B42F4(Ovl61Obj *arg0);

s32 func_800B453C(Ovl61Obj *arg0);

s32 func_800B4AE8(Ovl61Obj *arg0, s32 arg1);

s32 func_800B4D9C(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2);

s32 func_800B4FC8(s32 *p0, s32 *p1, s32 *q);

void func_800B5238(s32 arg0);

void func_800B558C(s32 arg0);

void func_800B58E8(s32 arg0);

void func_800B5C30(s32 arg0);

void func_800B5EC8(s32 arg0);

s32 func_800B60FC(s32 arg0);

void func_800B66D0(s32 arg0);

void func_800B68C8(s32 arg0);

void func_800B6AE0(s32 arg0);

void func_800B6BE8(s32 arg0);

void func_800B6CF0(s32 arg0);

void func_800B6FE0(s32 arg0);

void func_800B70CC(s32 arg0);

void func_800B7450(s32 arg0);

void func_800B7518(s32 arg0);

void func_800B7688(s32 arg0);

void func_800B77F8(s32 arg0);

void func_800B7B60(s32 arg0);

s32 func_800B8964(s32 arg0);

NAS *func_800B97D8(NAS *arg0);

void func_800B99E0(void);

void func_800BD43C(u32 arg0);

s32 func_800BFA24(Ovl61Vel *arg0, s32 arg1);

s32 func_800C0804(s32 arg0, s16 arg1, s32 arg2, s16 arg3);

/* func_800B7C60: a player's edge segment (x0,z0)-(x1,z1) and aim spread. */
typedef struct Ovl61Pos {
    s32 x;
    s32 y;
    s32 z;
} Ovl61Pos;
typedef struct Ovl61Edge {
    s16 x0;
    s16 pad2;
    s16 z0;
    s16 pad6;
    s16 x1;
    s16 padA;
    s16 z1;
    s16 padE;
} Ovl61Edge;
extern Ovl61Edge D_800C1900[4];
extern u16 D_800C1980[];
extern s16 D_800C1F00;

s32 func_800B7C60(s32 arg0, Ovl61Pos *from, Ovl61Pos *to, s32 angle);

s32 func_800B808C(s32 arg0, Ovl61Pos *from, Ovl61Pos *to, s32 angle);
extern Ovl61Edge D_800C1940[4];

#endif /* BASH_OVL61_H */
