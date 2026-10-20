#ifndef BASH_OVL5E_H
#define BASH_OVL5E_H

/* Functions and data defined in the ovl5e overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5e/ovl5e.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/engine.h"

typedef struct Ovl5eSpot {
    s32 x;
    s32 y;
    s32 z;
} Ovl5eSpot;

extern Ovl5eSpot D_800BD500[];
extern s16 D_8009D88E;

typedef struct Ovl5ePos {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 z;
} Ovl5ePos;

typedef struct PapuEntry {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
    /* 0xC */ s16 unkC;
    /* 0xE */ s16 unkE;
} PapuEntry;

extern s16 *D_800BD6F0[];

extern PapuEntry D_800BE648[20];

/* The object extension (AddObject) func_800B8508 fills. */
typedef struct Ovl5eObj {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u8 pad16[0x1A - 0x16];
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u8 pad1E[0x22 - 0x1E];
    /* 0x22 */ s16 unk22;
} Ovl5eObj;
extern s32 D_8009E464;
extern u16 D_8009E46C[];
typedef struct Ovl5eTile {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Ovl5eTile;

typedef struct Ovl5eCell {
    Ovl5eTile *tile;
    s32 pad4;
    s16 pad8;
    u16 unkA;
    s32 padC;
    s32 pad10;
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
    /* 0x04 */ s32 pad4[7];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34[3];
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

extern s32 D_8009E468;
extern u8 D_8009E3B4[];
extern u8 D_8009E3DC[];

/* Index of the ASTTable entry with this id, or 0. */
static inline s32 Ovl5eASTIndex(s32 id) {
    s32 i;

    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == id) {
            return i;
        }
    }
    return 0;
}

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl5e_800b87a0.h"

void func_800B42C0(void);

s32 func_800B7558(EXTengine *eng);

void func_800B7894(u16 arg0);

void InitPapuColumn(NAS *nas);

void func_800B7B10(void);

void func_800B7B4C(void);

void func_800B7DE4(void);

s32 func_800B8080(T_B8080 *p);

extern s32 D_8005B2EC;
extern s16 D_8009D6DC[];

void func_800B4154(u16 arg0);

void func_800B4204(void);

s32 func_800B744C(s32 arg0);

s32 func_800B77D8(void);

void PlotPapuColumn(NAS *nas);

void func_800B7E38(void);
extern u16 D_8009E46C[];
extern u16 D_800BD8E0;
extern u16 D_800BD8E4;
extern char D_800BD8E2[];
extern u8 D_800BD908[];

void func_800B7BE4(void);

void func_800B8508(NAS *arg0);

void func_800BB3F0(void);
void func_800BB69C(void);
void func_800BB86C(void);
void func_800BC274(void);
extern u8 D_800BC89C[];
extern u8 D_800BC8DC[];

s32 func_800B4D70(Ovl5ePos *arg0);

#endif /* BASH_OVL5E_H */
