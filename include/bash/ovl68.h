#ifndef BASH_OVL68_H
#define BASH_OVL68_H

/* Functions and data defined in the ovl68 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl68/ovl68.c */

#include "common.h"
#include "bash/nas.h"

void func_800B3330(void);
void func_800BADA8(s32 arg0);
void func_800BAE20(void);
void func_800BAE28(void);
s32 func_800BAE84(void);
s32 func_800BAE8C(void);
void func_800BB36C(void);
void func_800BB374(void);
void func_800BB37C(void);
void func_800BB640(s32 arg0);

extern u8 D_800BC210[];

#endif /* BASH_OVL68_H */
