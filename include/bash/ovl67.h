#ifndef BASH_OVL67_H
#define BASH_OVL67_H

/* Functions and data defined in the ovl67 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl67/ovl67.c */

#include "common.h"
#include "bash/nas.h"

s32 func_800BB05C(u8 *arg0);
u8 *func_800BB624(void);
void func_800BBD18(void);

extern u16 D_800C417A;

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl67_800bbdb8.h"

#endif /* BASH_OVL67_H */
