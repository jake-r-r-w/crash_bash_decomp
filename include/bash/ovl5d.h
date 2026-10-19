#ifndef BASH_OVL5D_H
#define BASH_OVL5D_H

/* Functions and data defined in the ovl5d overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5d/ovl5d.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"
#include "bash/engine.h"

typedef struct Ovl5dCell {
    s16 unk0[4];
    s16 unk8;
    s16 unkA;
} Ovl5dCell;

extern Ovl5dCell D_800C3BAC[8][8];

void func_800B62EC(void);

void func_800B7D20(void);

void PlotBoing(u8 *arg0);

s32 func_800BB1B4(u8 *arg0);

void func_800BB360(void);

void func_800BB368(void);

extern u8 D_80052F5C[];

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl5d_800bb64c.h"
#include "bash/ovl5d_800bff1c.h"

void InitMissileExplode(NAS *nas);

void func_800BAC10(void);

void func_800BAF68(void);

#endif /* BASH_OVL5D_H */
