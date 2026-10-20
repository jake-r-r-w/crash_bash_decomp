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
typedef struct {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 y;
    s16 pad1C;
    s16 h;
} T_B8080;

typedef struct Slot40 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 pad4[15];
} Slot40;

extern Slot40 D_800BD70C[6];

typedef struct Ovl5eKey {
    /* 0x00 */ u16 a;
    /* 0x02 */ u16 b;
    /* 0x04 */ u16 pad4;
    /* 0x06 */ u16 c;
    /* 0x08 */ u16 pad8[2];
    /* 0x0C */ u16 d;
    /* 0x0E */ u16 e;
    /* 0x10 */ u16 f;
    /* 0x12 */ u16 g;
} Ovl5eKey;

extern Ovl5eKey *D_800BD008[];
extern Ovl5eKey *D_800BE82C;
extern s16 D_800BE894;

void func_800B7600(s32 arg0);
typedef struct Ovl5eStep {
    s32 a;
    s32 b;
    s32 c;
    s16 d;
    s16 pad;
} Ovl5eStep;

extern Ovl5eStep *D_800BD118[];
void func_800B4EE8(s32 a, s32 b, s32 c, u16 d);

s32 func_800B7670(u8 *arg0);

void func_800B7778(void);

void func_800B790C(s32 arg0);

s32 func_800B794C(void);

void func_800B35AC(void);
void func_800B36D4(void);
void func_800B7D94(void);

void func_800B7D9C(void);

void func_800B7DA4(void);

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl5e_800b87a0.h"

s32 func_800B7558(EXTengine *eng);

void func_800B7894(u16 arg0);

void InitPapuColumn(NAS *nas);

void func_800B7B10(void);

void func_800B7DE4(void);

s32 func_800B8080(T_B8080 *p);

#endif /* BASH_OVL5E_H */
