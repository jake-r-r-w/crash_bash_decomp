#ifndef BASH_OVL69_H
#define BASH_OVL69_H

/* Functions and data defined in the ovl69 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl69/ovl69.c */

#include "common.h"
#include "bash/nas.h"

void func_800B5324(void);
void func_800B532C(void);
void func_800B5360(s32 *arg0);
void func_800B5390(s32 arg0);
void func_800B544C(void);
void func_800B5454(void);
void func_800B5A6C(void);
void func_800B5A74(void);
void func_800B7D20(void);
void func_800B81F0(void);
void func_800B83A4(void);
void func_800B8470(void);
void func_800B8490(void);
void func_800B8498(void);
void func_800B84B8(void);
void func_800B84E0(void);

extern u8 D_800BAAD0[];
extern s32 *D_800B9620;
extern u8 D_800B8974[];
extern s32 D_800B962C;
extern u8 D_800B8F20[];
extern s32 D_800B9508;
extern s32 D_800B950C;
extern u8 D_800BA754[];
extern s32 D_800BAAC8;
extern s32 D_800BAACC;

#endif /* BASH_OVL69_H */
