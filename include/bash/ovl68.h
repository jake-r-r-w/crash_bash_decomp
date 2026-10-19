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

void func_800BA5A0(s32 arg0, s32 arg1);
void func_800B94FC(s32 arg0, u16 arg1);
void func_800B9A50(s32 arg0, u16 arg1);
void func_800B9ED8(s32 arg0, u16 arg1);
void func_800BA220(s32 arg0, u16 arg1);
extern u8 D_8005A614[];

#endif /* BASH_OVL68_H */
