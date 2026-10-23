#ifndef BASH_OVL5E_800B87A0_H
#define BASH_OVL5E_800B87A0_H

/* Functions and data defined in src/overlays/ovl5e/800b87a0.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5e/800b87a0.c */

#include "common.h"
#include "bash/ovl5e.h"



typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} Ovl5eHitFlags;

/* Extension 0x1000 checked by func_800BC274. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[2];
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} Ovl5ePushExt;

void func_800BB308(s32 arg0);
extern HeroInfo Heroes[4];
s32 func_80085890();
s32 func_8008A200(s32 arg0);
void func_8008A810(void);

extern u16 D_8009E080[][4];
void func_800BA3C4(EXTengine *eng);

extern _sfx_handler *D_800C4780;
void func_800898EC(s32 arg0);
void func_800BC698(s32 arg0);
extern u16 *D_8009D6EC;
s32 func_8008ABF8(s32 arg0);

s32 func_800BA22C(EXTengine *eng, Ovl5eHitFlags *flags, u32 mask);

void func_800BA4C0(s32 arg0);

typedef struct { s16 a; s16 b; s16 c; } Ovl5eWeaponVec;
extern Ovl5eWeaponVec D_800BE8AC[];

NAS *func_800B9E74(EXTengine *eng, s32 self, s32 checkHeroes);

void PAHeroHit(u32 arg0);

void func_800BA7D8(s32 arg0);

void func_800BA958(s32 arg0);

void func_800BABA0(s32 arg0);

void func_800BADE8(s32 arg0);

void func_800BAFD4(s32 arg0);

void func_800BB0D4(s32 arg0);

void *func_800BC588(s32 idx, s32 mask);

void func_800BC744(s32 arg0);

void SetPAHeroCarry(u32 arg0);

void func_800BC800(s32 arg0);

void func_800BC84C(s32 arg0);

#endif /* BASH_OVL5E_800B87A0_H */
