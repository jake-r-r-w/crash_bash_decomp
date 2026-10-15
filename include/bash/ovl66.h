#ifndef BASH_OVL66_H
#define BASH_OVL66_H

/* Functions and data defined in the ovl66 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl66/ovl66.c */

#include "common.h"
#include "bash/nas.h"

s32 func_800B62C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void PlotBonkFlag(NAS *nas);
void func_800B6560(void);
void func_800B67C4(void);
void func_800B6868(s32 arg0);
void func_800B688C(void);
void func_800B68BC(void);
void func_800B69A0(void);
void func_800B6A1C(void);
void func_800B6AF4(void);
void func_800B6AFC(void);

extern s32 D_800B9EEC;

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl66_800b6efc.h"

#endif /* BASH_OVL66_H */
