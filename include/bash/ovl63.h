#ifndef BASH_OVL63_H
#define BASH_OVL63_H

/* Functions and data defined in the ovl63 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl63/ovl63.c */

#include "common.h"
#include "bash/nas.h"

void func_800C4618(void);
void func_800C4BB0(void);
void func_800C4C14(void);
void func_800C4C98(void);
s32 func_800C4D1C(u8 *arg0, u8 *arg1);
void func_800C4D70(u8 *arg0);
s32 func_800C5014(s32 arg0);
void func_800C5028(void);
void func_800C53BC(s32 arg0);
s32 func_800C5AF8(void);
s32 func_800C5B7C(void);
void func_800C5B84(void);
void func_800C5B8C(void);
s32 func_800C5D0C(s32 arg0);
void func_800C5D8C(void);

extern s32 D_800C6930;
extern s32 D_800C6D20;

void func_800B7D94(void);
void func_800B5360(s32 *arg0);

#endif /* BASH_OVL63_H */
