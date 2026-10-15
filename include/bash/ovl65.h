#ifndef BASH_OVL65_H
#define BASH_OVL65_H

/* Functions and data defined in the ovl65 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl65/ovl65.c */

#include "common.h"
#include "bash/nas.h"

s32 func_800B4C70(s32 *arg0, s32 arg1);

void func_800BCA18(HeroInfo *hero);

s32 func_800BCDF0(u8 *arg0);

void func_800BCE6C(void);

void func_800BCE74(void);

void func_800BCF44(s32 arg0);

void func_800BD08C(void);

void func_800BD1FC(void);

s32 func_800BD408(s32 arg0);

s32 func_800BD488(u8 *arg0, u8 *arg1);

#endif /* BASH_OVL65_H */
