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

extern u16 D_800BE898[];

/* A hero's sliding/throw stats, by hero_name (D_8009E488 tables). */
typedef struct Ovl5eStats {
    /* 0x00 */ u16 a;
    /* 0x02 */ u16 b;
    /* 0x04 */ u16 c;
    /* 0x06 */ u16 d;
    /* 0x08 */ u16 e;
    /* 0x0A */ u16 f;
    /* 0x0C */ u16 g;
    /* 0x0E */ u16 h;
    /* 0x10 */ u8 i;
} Ovl5eStats;

/* D_8009E488.unk0 / .unk4: Ovl5eStats *[] by hero_name. */
#define OVL5E_STATS(t, n) (((Ovl5eStats **)D_8009E488.t)[n])

/* Extension 0x800 of the carried objects (func_800B87A0). */
typedef struct Ovl5eSlide {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 dir;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ u8 pad20[0xA];
    /* 0x2A */ s16 unk2A;
} Ovl5eSlide;

/* D_800BD70C entries: pad4[0] engine, pad4[4] motion, pad4[6] held object. */
#define OVL5E_SLOT_ENG(i) ((EXTengine *)D_800BD70C[i].pad4[0])
#define OVL5E_SLOT_DYN(i) ((B5E44Dyn *)D_800BD70C[i].pad4[4])
#define OVL5E_SLOT_OBJ(i) (D_800BD70C[i].pad4[6])

extern s32 D_8009E2B4[];

void func_800B87A0(NAS *nas);

#endif /* BASH_OVL5E_800B87A0_H */
