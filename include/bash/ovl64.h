#ifndef BASH_OVL64_H
#define BASH_OVL64_H

/* Functions and data defined in the ovl64 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl64/ovl64.c */

#include "common.h"
#include "bash/nas.h"

void func_800B3428();

void func_800B359C();

s32 func_800B8E4C(void);

s32 func_800B8E54(void);

void func_800B8E5C(void);

void func_800B9038(void);

void func_800B9040(void);

void func_800B9048(void);

void func_800B9088(void);

void func_800BB37C(void);

void func_800BB52C(s32 arg0);
extern void (*D_8009D73C)();

extern s32 D_8005AA70[11];

void func_800B9BF8(s32 arg0);
extern s32 D_8005A5D8;
extern HeroInfo Heroes[4];
void func_80025F44(EXTengine *eng);
s32 func_80085890();
s32 func_8008A200(s32 arg0);
void func_8008A810(void);

#endif /* BASH_OVL64_H */
