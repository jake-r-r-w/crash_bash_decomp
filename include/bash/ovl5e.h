#ifndef BASH_OVL5E_H
#define BASH_OVL5E_H

/* Functions and data defined in the ovl5e overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5e/ovl5e.c */

#include "common.h"
#include "bash/nas.h"

typedef struct PapuEntry {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s32 unk8;
    /* 0xC */ s32 unkC;
} PapuEntry;

extern PapuEntry D_800BE648[20];
typedef struct Ovl5eTile {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ovl5eTile;

typedef struct Ovl5eCell {
    Ovl5eTile *tile;
    s32 pad[4];
} Ovl5eCell;

extern Ovl5eCell D_800BD948[8][8];

s32 func_800B7670(u8 *arg0);

void func_800B7778(void);

void func_800B790C(s32 arg0);

void func_800B7D94(void);

void func_800B7D9C(void);

void func_800B7DA4(void);

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl5e_800b87a0.h"

s32 func_800B7558(EXTengine *eng);

void func_800B7B10(void);

#endif /* BASH_OVL5E_H */
