#ifndef BASH_OVL66_800B6EFC_H
#define BASH_OVL66_800B6EFC_H

/* Functions and data defined in src/overlays/ovl66/800b6efc.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl66/800b6efc.c */

#include "common.h"
#include "bash/ovl66.h"

typedef struct Ovl66PadButtons {
    /* 0x00 */ u8 unk0[0x50];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl66PadButtons;

extern HeroInfo Heroes[4];

void func_800B8A30(s32 arg0);
typedef struct Ovl66Slot {
    /* 0x0 */ s32 f0;
    /* 0x4 */ s32 f4;
    /* 0x8 */ s32 f8;
    /* 0xC */ s32 fC;
} Ovl66Slot;

extern Ovl66Slot D_800B9F4C[];
extern u16 D_800AD12C;
extern HeroInfo Heroes[4];

extern s32 D_800B9F1C[];

extern void (*D_8009D73C)();
extern s32 (*D_8009D728)(s32);

void func_800B72FC(s32 idx);

s32 func_800B8714(s32 arg0);

void func_800B876C(s32 arg0);

s32 func_800B8814(s32 arg0);

void func_800B886C(s32 idx);

void func_800B8918(s32 arg0);

s32 func_800B89D8(s32 arg0);

void func_800B8AC8(s32 arg0);

void func_800B8AF4(s32 arg0);

void func_800B8B3C(s32 arg0);

void func_800B8BA0(s32 arg0);

void func_800B8C3C(s32 arg0);

void func_800B8CB4(s32 arg0);

void func_800B8D3C(s32 arg0);

s32 func_800B8D88(s32 arg0);

s32 func_800B8E08(s32 arg0);

void func_800B8EF8(s32 arg0);

void func_800B9144(s32 player);
void func_800B61F8(s32 player);

void func_800B8EF0(void);

void func_800B91BC(s32 arg0);

void func_800B9264(void);

#endif /* BASH_OVL66_800B6EFC_H */
