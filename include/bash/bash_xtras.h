#ifndef BASH_BASH_XTRAS_H
#define BASH_BASH_XTRAS_H

/* Functions and data defined in src/overlays/bash/xtras.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/bash/xtras.c */

#include "common.h"
#include "bash/bash_ovl.h"
#include "bash/main.h"
#include "bash/engine.h"

typedef struct {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 y;
    s16 pad1C;
    s16 h;
} T_BB7B8;


typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 pad16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
} ExtCurve;

typedef struct {
    u8 pad0[0x14];
    u16 type;
} T_BB7E8Fux;

s32 BashGridKey(T_BB7B8 *p);

extern u16 BashPickupModels[];

void InitCratePickup(NAS *arg0);

void InitShieldPickup(NAS *arg0);

void InitBootsPickup(NAS *arg0);

s32 CrateFallTime(s32 pos, ExtCurve *c);




void BashHeroesTouched(s32 arg0, s32 arg1);

void CrateFindFreeSpot(EXTengine *eng);


void InitAnvilPickup(NAS *arg0);

void InitSlowmoPickup(NAS *arg0);

s32 CrateHitHeroes(EXTengine *eng, EXTobject *flags, u32 mask);

void SetBashHeroHit(s32 arg0);


typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 pad14;
    s32 z;
    s16 pad1C;
    s16 ang;
} T_BB64C_A;

typedef struct {
    u8 pad0[0xD];
    u8 dir;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 pad14;
    s16 unk16;
    u8 pad18[0x12];
    s16 unk2A;
} T_BB64C_B;

void CrateAimAtGrid(T_BB64C_A *a, T_BB64C_B *b);

NAS *CrateHitSolids(EXTengine *eng, s32 who);


void InitCrate(NAS *arg0);

void PlotPickup(NAS *arg0);


/* PlotCrate: its extension 0x800 (CrateAimAtGrid's T_BB64C_B). */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 flag;  /* tested for zero */
    /* 0x0D */ u8 dir;
    /* 0x0E */ s16 speed;
    /* 0x10 */ s16 accel;
    /* 0x12 */ s16 decel;
    /* 0x14 */ s16 max;
    /* 0x16 */ s16 tx;
    /* 0x18 */ u16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ u8 pad20[0xA];
    /* 0x2A */ s16 tz;
} T_BC038Ext;

/* PlotCrate: entries of HitHooks.unk0/unk4, by hero_name. */
typedef struct {
    /* 0x00 */ s16 speed;
    /* 0x02 */ s16 max;
    /* 0x04 */ s16 accel;
    /* 0x06 */ s16 decel;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
} T_BC038Stats;

/* PlotCrate: frees the carrying hero's hold on the crate (cf. papu's PapuReleaseObj). */
static inline void BC038Release(EXTobject *fux) {
    s32 w = fux->player;
    HeroInfo *b;
    HeroInfo *h;

    if (w != -1 && w < 5) {
        b = Heroes;
        h = &b[w];
        if (h->hero_object == (EXTobject *)fux) {
            h->hero_object = NULL;
            if (h->hero_eng->sfx != NULL) {
                StopSfx(h->hero_eng->sfx);
            }
        }
    }
}

#define BC038_SNAP(eng)                                     \
    (eng)->XYZ.x = ((eng)->XYZ.x & ~0xFF) + 0x80;       \
    (eng)->XYZ.z = ((eng)->XYZ.z & ~0xFF) + 0x80

#define BC038_STOP(ext)                                     \
    (ext)->decel = 0;                                       \
    (ext)->accel = 0;                                       \
    (ext)->max = 0;                                         \
    (ext)->speed = 0

#define BC038_STATB(base, tbl, w) (((T_BC038Stats **)HitHooks.tbl)[(base)[w].hero_name])
#define BC038_STAT(tbl, w) BC038_STATB(Heroes, tbl, w)

/* The crate breaks (PlotCrate passes eng and fux). fux goes unused: a1 is
 * never read, in every disc's copy. */
void CrateBreak(EXTengine *eng, EXTobject *fux);

extern u16 BashTickingCrateModels[];
void PlotCrate(NAS *arg0);

/* data defined in xtras.c */
extern u16 BashDrainsEdgeCrates[];
extern EXTengine BashSfxEngine;
extern u16 BashDrainsEdgePos;
extern u16 BashPowerupTimer;
extern u8 *BashCrateList;

#endif /* BASH_BASH_XTRAS_H */
