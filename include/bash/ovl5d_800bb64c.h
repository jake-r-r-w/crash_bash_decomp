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

s32 func_800BB7B8(T_BB7B8 *p);

void InitShieldPickup(NAS *arg0);

void InitBootsPickup(NAS *arg0);

s32 func_800BFB6C(s32 pos, ExtCurve *c);



void func_800BEDE8(s32 arg0, s32 arg1);
extern s32 (*D_8009D750)(EXTengine *);

void func_800BFAC0(EXTengine *eng);

void func_800BFC30(EXTengine *eng);

#endif /* BASH_OVL5D_800BB64C_H */
