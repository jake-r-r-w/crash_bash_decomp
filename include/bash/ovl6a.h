#ifndef BASH_OVL6A_H
#define BASH_OVL6A_H

/* Functions and data defined in the ovl6a overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl6a/ovl6a.c */

#include "common.h"
#include "bash/nas.h"

s32 *func_800B4D8C(void);
void func_800B4DE8(void);
void func_800B4EAC(void);
void func_800B86AC(void);
void func_800B875C(void);
void func_800B88B8(void);

extern u8 D_800C7294[];
extern s32 D_80098714;
extern s32 D_800BCED4;
extern s32 D_800BCED8;
extern s32 D_800BCEE0;
extern subengine *D_800BCEE8;

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl6a_800b4eac.h"

void func_800B4694(void);

int func_800B4D98(unsigned char *s);

void func_800B4E1C(void);

#endif /* BASH_OVL6A_H */
