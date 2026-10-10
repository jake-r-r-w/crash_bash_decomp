#ifndef BASH_OVL64_H
#define BASH_OVL64_H

/* Functions and data defined in the ovl64 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl64/ovl64.c */

#include "common.h"
#include "bash/nas.h"

s32 func_800B8E4C(void);
s32 func_800B8E54(void);
void func_800B8E5C(void);
void func_800B9038(void);
void func_800B9040(void);
void func_800B9048(void);

#endif /* BASH_OVL64_H */
