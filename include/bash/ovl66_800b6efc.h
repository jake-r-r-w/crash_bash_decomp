#ifndef BASH_OVL66_800B6EFC_H
#define BASH_OVL66_800B6EFC_H

/* Functions and data defined in src/overlays/ovl66/800b6efc.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl66/800b6efc.c */

#include "common.h"
#include "bash/ovl66.h"

extern HeroInfo Heroes[4];

void func_800B8A30(s32 arg0);
typedef struct Ovl66Slot {
    /* 0x0 */ s32 f0;
    /* 0x4 */ s32 f4;
    /* 0x8 */ s32 f8;
    /* 0xC */ s32 fC;
} Ovl66Slot;

extern Ovl66Slot D_800B9F4C[];
extern HeroInfo Heroes[4];

void func_800B8AC8(s32 arg0);

void func_800B9144(s32 player);
void func_800B61F8(s32 player);

void func_800B8EF0(void);

void func_800B91BC(s32 arg0);

void func_800B9264(void);

#endif /* BASH_OVL66_800B6EFC_H */
