#ifndef BASH_BONK_BOHERO_H
#define BASH_BONK_BOHERO_H

/* Functions and data defined in src/overlays/bonk/bohero.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/bonk/bohero.c */

#include "common.h"
#include "bash/bonk_ovl.h"


extern s32 BonkSteerB[];
extern s32 BonkSteerA[];

void BonkHeroSwingRecoil(s32 arg0);
typedef struct BonkSlot {
    /* 0x0 */ void *f0;
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
    /* 0x54 */ BonkPos *pos;
} BonkInner;

typedef struct BonkTarget {
    /* 0x00 */ s32 pad0;
    /* 0x04 */ BonkInner *inner;
} BonkTarget;

extern BonkSlot BonkSlots[];

extern s32 BonkBackswingSfx[];
extern s32 BonkShockwaveSfx[];
void BonkMalletHit(VECTOR *pos, s32 a, s32 b, s32 c);
extern s32 BonkSwingSfx[];
extern s32 BonkSwingHitSfx[];

void BonkHitTarget(s32 arg0, s32 arg1, BonkTgt2 *t);
void BonkHeroSwing(s32 arg0);


void SetBonkHeroIdle(s32 idx);

void BonkHeroIdle(s32 arg0);

void BonkHeroBackswing(s32 arg0);

void BonkHeroExtBackswing(s32 arg0);

s32 BonkHeroTestIdle(s32 arg0);

void SetBonkHeroLoser(s32 arg0);

s32 BonkHeroTestLose(s32 arg0);

void BonkHeroWinner(s32 idx);

void SetBonkHeroWinner(s32 arg0);

s32 BonkHeroTestWin(s32 arg0);

void SetBonkHeroSwingRecoil(s32 arg0);

void SetBonkHeroSwing(s32 arg0);

void SetBonkHeroExtBackswing(s32 arg0);

void SetBonkHeroBackswing(s32 arg0);

void BonkHeroHit(s32 arg0);

void SetBonkHeroHit(s32 arg0);

void SetBonkHeroRun(s32 arg0);

s32 BonkHeroTestRun(s32 arg0);

s32 BonkHeroToIdle(s32 arg0);

void BonkHidePowerup(s32 arg0);

s32 BonkKeepInBoundsB(EXTengine *eng);

void BonkHeroSnapShadow(s32 player);
void BonkAI_Reset(s32 player);

void BonkHeroPostMove(void);

void BonkHeroBounce(s32 arg0);

void BonkBounceAll(void);

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
                nas->Engine->sub->u6C.dbase = anim;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
    }
}

void BonkSpawnShockwave(s32 arg0);

void BonkHeroLoser(s32 arg0);

/* Inline copy of BonkHeroToIdle. */
static inline s32 BonkStopInline(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction(arg0, 0x40);
        return 1;
    }
    return 0;
}

void BonkHeroRun(s32 arg0);


#endif /* BASH_BONK_BOHERO_H */
