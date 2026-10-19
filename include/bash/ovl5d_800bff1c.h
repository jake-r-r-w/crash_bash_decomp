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
void func_800C0DE4(s32 arg0);

void func_800C1278(s32 arg0);

void func_800C14D4(s32 arg0);

void func_800C2E58(void);

void func_800C2EC8(s32 arg0);

void func_800C2F14(s32 arg0);

void func_800C2F60(s32 arg0);

#endif /* BASH_OVL5D_800BFF1C_H */
