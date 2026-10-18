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

#endif /* BASH_OVL67_H */
