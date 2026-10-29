#ifndef BASH_PAPU_PAHERO_H
#define BASH_PAPU_PAHERO_H

/* Functions and data defined in src/overlays/papu/pahero.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/papu/pahero.c */

#include "common.h"
#include "bash/papu_ovl.h"





void PAHeroWinner(s32 arg0);

void PACrateFindFreeSpot(EXTengine *eng);

/* past the end of the overlay, at a fixed address (configs/USA/undefined_syms_manual.txt) */
extern _sfx_handler *PapuStraySfx;
void PAHeroDead(s32 arg0);

s32 PACrateHitHeroes(EXTengine *eng, EXTobject *flags, u32 mask);

void SetPAHeroHit(s32 arg0);

typedef struct { s16 a; s16 b; s16 c; } PapuWeaponVec;
extern PapuWeaponVec PAHeroThrowVecs[];

NAS *PACrateCollide(EXTengine *eng, s32 self, s32 checkHeroes);

void PAHeroHit(u32 arg0);

void PAHeroPickup(s32 arg0);

void PAHeroHold(s32 arg0);

void PAHeroCarry(s32 arg0);

void PAHeroThrow(s32 arg0);

void SetPAHeroDead(s32 arg0);

void SetPAHeroWinner(s32 arg0);

void *PapuHeroObjectAhead(s32 idx, s32 mask);

void SetPAHeroThrow(s32 arg0);

void SetPAHeroCarry(u32 arg0);

void SetPAHeroHold(s32 arg0);

void SetPAHeroPickup(s32 arg0);

extern u16 PACrateTickModels[];

/* A hero's sliding/throw stats, by hero_name (HitHooks tables). */
typedef struct PapuStats {
    /* 0x00 */ u16 a;
    /* 0x02 */ u16 b;
    /* 0x04 */ u16 c;
    /* 0x06 */ u16 d;
    /* 0x08 */ u16 e;
    /* 0x0A */ u16 f;
    /* 0x0C */ u16 g;
    /* 0x0E */ u16 h;
    /* 0x10 */ u8 i;
} PapuStats;

/* HitHooks.unk0 / .unk4: PapuStats *[] by hero_name. */
#define PAPU_STATS(t, n) (((PapuStats **)HitHooks.t)[n])

/* Extension 0x800 of the carried objects (PlotPACrate). */
typedef struct PapuSlide {
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
} PapuSlide;

/* PapuMinionSlots entries: pad4[0] engine, pad4[4] motion, pad4[6] held object. */
#define PAPU_SLOT_ENG(i) ((EXTengine *)PapuMinionSlots[i].pad4[0])
#define PAPU_SLOT_DYN(i) ((B5E44Dyn *)PapuMinionSlots[i].pad4[4])
#define PAPU_SLOT_OBJ(i) (PapuMinionSlots[i].pad4[6])


void PlotPACrate(NAS *nas);

#endif /* BASH_PAPU_PAHERO_H */
