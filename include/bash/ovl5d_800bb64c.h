#ifndef BASH_OVL5D_800BB64C_H
#define BASH_OVL5D_800BB64C_H

/* Functions and data defined in src/overlays/ovl5d/800bb64c.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5d/800bb64c.c */

#include "common.h"
#include "bash/ovl5d.h"
#include "bash/main.h"
#include "bash/engine.h"

extern s32 D_8009E464;
typedef struct {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 y;
    s16 pad1C;
    s16 h;
} T_BB7B8;


typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
} ExtCurve;

extern u16 D_8009E25C[13];
typedef struct {
    u8 pad0[0x14];
    u16 type;
} T_BB7E8Fux;

s32 func_800BB7B8(T_BB7B8 *p);
void func_800BB7E8(void);

extern u16 D_800C46E4[];

void InitCratePickup(NAS *arg0);

void InitShieldPickup(NAS *arg0);

void InitBootsPickup(NAS *arg0);

s32 func_800BFB6C(s32 pos, ExtCurve *c);

typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} HitFlags;



void func_800BEDE8(s32 arg0, s32 arg1);
extern s32 (*D_8009D750)(EXTengine *);

void func_800BFAC0(EXTengine *eng);

void func_800BFC30(EXTengine *eng);

extern u16 D_8009E080[][4];

void InitAnvilPickup(NAS *arg0);

void InitSlowmoPickup(NAS *arg0);

s32 func_800BF928(EXTengine *eng, HitFlags *flags, u32 mask);

void func_800BFD2C(s32 arg0);

extern u8 D_80051244[];

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 pad14;
    s32 z;
    s16 pad1C;
    s16 ang;
} T_BB64C_A;

typedef struct {
    u8 pad0[0xD];
    u8 dir;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 pad14;
    s16 unk16;
    u8 pad18[0x12];
    s16 unk2A;
} T_BB64C_B;

void func_800BB64C(T_BB64C_A *a, T_BB64C_B *b);

NAS *func_800BF708(EXTengine *eng, s32 who);

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u8 pad16[0x1A - 0x16];
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u8 pad1E[0x22 - 0x1E];
    /* 0x22 */ s16 unk22;
} T_BBD48Obj;

void func_800BBD48(NAS *arg0);

void func_800BE9D0(NAS *arg0);

#endif /* BASH_OVL5D_800BB64C_H */
