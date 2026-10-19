#ifndef BASH_OVL62_H
#define BASH_OVL62_H

/* Functions and data defined in the ovl62 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl62/ovl62.c */

#include "common.h"
#include "bash/nas.h"

extern u8 D_800546C4[];

void func_800B5AC4();
void func_800BAFA0();
void func_800BB810();
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

/* D_80051640 as the camera func_800CACC0 sets up. */
typedef struct Ovl62Cam {
    /* 0x00 */ CamXYZ eye;
    /* 0x0C */ CamXYZ at;
    /* 0x18 */ s32 dist;
    /* 0x1C */ u8 pad1C[0x54 - 0x1C];
    /* 0x54 */ s16 rotX;
    /* 0x56 */ s16 rotY;
    /* 0x58 */ s16 rotZ;
} Ovl62Cam;

typedef struct Ext10000000 {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 unkC;
} Ext10000000;

typedef struct Ext20 {
    /* 0x00 */ u8 pad[0x20];
    /* 0x20 */ s32 val;
    /* 0x24 */ s32 state;
} Ext20;

/* PadState (D_80051338) as func_800D1C14 and func_800D307C use it: the
 * buttons at 0x50, a word at 0x54. */
typedef struct Ovl62Pad {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl62Pad;

void func_800D1C14(s32 player);

extern HeroInfo Heroes[4];

/* Extension 0x10000000 made by InitSimpleExplosion: a velocity per sub-engine. */
typedef struct ExtSimpleExplosion {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ SLONG_XYZ vel[20];
    /* 0x104 */ s32 unk104;
} ExtSimpleExplosion;

extern u8 D_800546C4[];

/* The object PlotBrioPack follows (its NAS.UserInfo.user1). */
typedef struct BrioAnim {
    /* 0x00 */ u8 pad0[0x1E];
    /* 0x1E */ s16 frame;
} BrioAnim;

typedef struct BrioOwner {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ EXTengine *engine;
    /* 0x08 */ u8 pad8[0x1C - 0x8];
    /* 0x1C */ BrioAnim *anim;
    /* 0x20 */ u8 pad20[0x2C - 0x20];
    /* 0x2C */ u16 type;
} BrioOwner;

/* Head of a list linked through +0x20 (counted by func_800C9B5C). */
extern u8 *D_800D5454;
extern HeroInfo Heroes[4];
extern s32 D_8005A680[];

s32 func_800C9B5C(void);

void func_800BFD38();

/* A gravity object's motion block (GravityObject.target). */
typedef struct GravityMotion {
    /* 0x00 */ u8 pad0[0xD];
    /* 0x0D */ u8 flags;
    /* 0x0E */ s16 vx;
    /* 0x10 */ s16 ax;
    /* 0x12 */ u8 pad12[0x16 - 0x12];
    /* 0x16 */ s16 rx;
    /* 0x18 */ s16 vy;
    /* 0x1A */ s16 ay;
    /* 0x1C */ u8 pad1C[0x20 - 0x1C];
    /* 0x20 */ s16 ry;
    /* 0x22 */ s16 vz;
    /* 0x24 */ s16 az;
    /* 0x26 */ u8 pad26[0x2A - 0x26];
    /* 0x2A */ s16 rz;
    /* 0x2C */ u8 pad2C[0x2E - 0x2C];
    /* 0x2E */ s16 floor;
} GravityMotion;

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
    /* 0x28 */ s32 target;
    /* 0x2C */ u8 pad2C[0x30 - 0x2C];
    /* 0x30 */ s32 state;
    /* 0x34 */ u8 pad34[0x3C - 0x34];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0x44 - 0x40];
    /* 0x44 */ void *func;
    /* 0x48 */ u8 pad48[0x5C - 0x48];
    /* 0x5C */ s32 timer;
} Ovl62Fux;

extern s32 D_800D4E0C[];
void func_800B7520();
void func_800B767C();
extern s32 D_800D5FEC[];

/* Inlined into func_800B9994. */
static inline void Ovl62StartChase(s32 player) {
    Ovl62Fux *f;
    s32 j;
    s32 n;

    f = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (f == NULL) {
        return;
    }
    for (n = j = 0; j < 4; j++) {
        if (Heroes[j].hero_status & 0x2000) {
            n++;
        }
        f->state = 4;
        f->func = func_800CB12C;
        if (n >= 3) {
            return;
        }
    }
    f->unk3C = 1;
    f->unk24 = Random(0x3C) + 5;
}
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

/* D_8009D87C (declared in bash/8007ee2c.h, which this unit does not include). */
typedef struct Ovl62Odds {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Ovl62Odds;

extern Ovl62Odds D_8009D87C[];
extern s32 D_800D52E0[];

s32 func_8008CEB0(u8 *arg0, u8 *arg1);
s32 func_800B7B88(s32 arg0);
s32 func_800BCF08(s32 arg0, s32 arg1);
s32 func_800D05A8(s32 arg0);

/* The NAS that owns the bomb maker. */
extern NAS *D_800D52B4;

s32 func_800C9C98(u8 *arg0);

NAS *func_800CB5F4(void);

void func_800D2C38(s32 player);

void func_800D2F48(s32 player);

void func_800D3018(s32 player);

extern s32 D_800D5FEC[4];

void func_800B60B8(s32 player);

void func_800B767C(s32 player);

s32 func_800B9994(s32 player);

s32 func_800C9EC4(EXTengine *eng, s32 player);

s32 func_800C9F70(void);

void func_800CACC0(void);

void func_800CB134(s32 player);

s32 func_800CB284(void);
s32 func_8008502C();

s32 func_800CB2C8(s32 arg0);

void func_800CB508(s32 player);

void func_800CB650(s32 player);

void func_800CBA64(void);

s32 func_800CD560(s32 arg0);

s32 func_800D0650(s32 player);

s32 func_800D0750(s32 player);

s32 func_800D10A0(s32 player);

void func_800D307C(s32 player);

#endif /* BASH_OVL62_H */
