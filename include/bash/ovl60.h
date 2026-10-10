#ifndef BASH_OVL60_H
#define BASH_OVL60_H

/* Functions and data defined in the ovl60 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl60/ovl60.c */

#include "common.h"
#include "bash/nas.h"

void func_800C0BF0(s32 arg0, s32 arg1);
s32 func_800C1764(void);
s32 func_800C2660(u8 *arg0);
void func_800C2F50(void);
void func_800C2F84(void);
void func_800C318C(s32 arg0);
void func_800C31B0(void);
void func_800C3388(void);
void func_800C3390(void);
void func_800C34B0(void);
void func_800C34D0(void);
void func_800C34F0(s32 arg0);
void func_800C3510(u8 *arg0);

void func_800C353C();
extern s32 D_800C5BB0;
extern s32 D_800C52E0;
extern s32 D_800C52E4;
extern s32 D_800C52EC;
extern s32 D_800C52F0;
extern s32 D_800C52F8;
extern s32 D_800C52FC;
extern s32 D_800C5BAC;
extern s32 D_800C5BB8;
extern s32 D_800C5BBC;

#endif /* BASH_OVL60_H */
