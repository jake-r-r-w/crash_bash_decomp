#ifndef BASH_BONK_800B6EFC_H
#define BASH_BONK_800B6EFC_H

/* Functions and data defined in src/overlays/bonk/800b6efc.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/bonk/800b6efc.c */

#include "common.h"
#include "bash/bonk_ovl.h"

typedef struct BonkPadButtons {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4[0x38 - 0x04];
    /* 0x38 */ u16 unk38;
    /* 0x3A */ u8 unk3A[0x50 - 0x3A];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} BonkPadButtons;

extern HeroInfo Heroes[4];
extern s32 D_800B94E0[];
extern s32 D_800B9440[];

void func_800B8A30(s32 arg0);
typedef struct BonkSlot {
    /* 0x0 */ s32 f0;
    /* 0x4 */ s32 f4;
    /* 0x8 */ s32 f8;
    /* 0xC */ s32 fC;
} BonkSlot;

typedef struct BonkPos {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 y;
    /* 0x18 */ s32 z;
} BonkPos;

typedef struct BonkInner {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ BonkPos *unk54;
} BonkInner;

typedef struct BonkTarget {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ BonkInner *unk4;
} BonkTarget;

extern BonkSlot D_800B9F4C[];
extern u16 D_800AD12C;
extern HeroInfo Heroes[4];

extern s32 D_800B9F1C[];
extern s32 D_800B9F0C[];
void func_800B4910(VECTOR *pos, s32 a, s32 b, s32 c);
extern s32 D_800B9F2C[];
extern s32 D_800B9F3C[];

void func_800B47C8(s32 arg0, s32 arg1, BonkTgt2 *t);
void func_800B6EFC();
void func_800B7908(s32 arg0);

extern void (*D_8009D73C)();
extern s32 (*D_8009D728)(s32);
extern u16 D_800AD12C;

void func_800B72FC(s32 idx);

void func_800B738C(s32 arg0);

void func_800B76CC(s32 arg0);

void func_800B77FC(s32 arg0);

s32 func_800B8714(s32 arg0);

void func_800B876C(s32 arg0);

s32 func_800B8814(s32 arg0);

void func_800B886C(s32 idx);

void func_800B8918(s32 arg0);

s32 func_800B89D8(s32 arg0);

void func_800B8AC8(s32 arg0);

void func_800B8AF4(s32 arg0);

void func_800B8B3C(s32 arg0);

void func_800B8BA0(s32 arg0);

void func_800B8C3C(s32 arg0);

void func_800B8CB4(s32 arg0);

void func_800B8D3C(s32 arg0);

s32 func_800B8D88(s32 arg0);

s32 func_800B8E08(s32 arg0);

void func_800B8EF8(s32 arg0);

s32 func_800B8F64(EXTengine *eng);

void func_800B9144(s32 player);
void func_800B61F8(s32 player);

void func_800B8EF0(void);

void func_800B91BC(s32 arg0);

void func_800B9264(void);

/* Spawns the 0x1605 puff at v. */
static inline void BonkSpawnPuff(SLONG_XYZ *v, s32 kind, void *anim) {
    NAS *nas;
    s32 *ext;

    nas = CreateNAS(v->x, v->y, v->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = kind;
            ext[6] = 0x100;
            if (anim != NULL) {
                ext[8] = 5;
                nas->Engine->sub->unk6C = anim;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
    }
}

void func_800B6EFC(s32 arg0);

void func_800B8550(s32 arg0);

/* Inline copy of func_800B8E08. */
static inline s32 BonkStopInline(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction(arg0, 0x40);
        return 1;
    }
    return 0;
}

void func_800B747C(s32 arg0);

/* Extension 0x1000 flags set by func_800B7B48. */
typedef struct BonkHitFlags {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} BonkHitFlags;

#endif /* BASH_BONK_800B6EFC_H */
