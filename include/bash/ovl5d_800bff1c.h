#ifndef BASH_OVL5D_800BFF1C_H
#define BASH_OVL5D_800BFF1C_H

/* Functions and data defined in src/overlays/ovl5d/800bff1c.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5d/800bff1c.c */

#include "common.h"
#include "bash/ovl5d.h"
#include "bash/main.h"
#include "bash/8007ee2c.h"

void func_800C1190(s32 arg0);

extern HeroInfo Heroes[4];
s32 SetHeroSeq(HeroInfo *hero, s32 seq);

extern u16 *D_8009D6EC;
void func_800C2DAC(s32 arg0);
extern _sfx_handler *D_800C4780;
void func_800C0BAC(s32 arg0);

void func_800C0DE4(s32 arg0);

void func_800C0F14(s32 arg0);

void func_800C1278(s32 arg0);

void func_800C14D4(s32 arg0);

NAS *func_800C159C(s32 player, s32 mask, s32 unused, s32 cone);

void *func_800C2B60(s32 idx, s32 mask);

void func_800C2C70(s32 *a, s32 *b, s16 ma, s32 *c, s32 *d, s16 mb);

void func_800C2E58(s32 arg0);

void func_800C2EC8(s32 arg0);

void func_800C2F14(s32 arg0);

void func_800C2F60(s32 arg0);

s32 func_800C2FB0(s32 arg0);

extern u8 D_800C4790[];
s32 func_8008502C();

void func_800C013C(s32 arg0);

#endif /* BASH_OVL5D_800BFF1C_H */
