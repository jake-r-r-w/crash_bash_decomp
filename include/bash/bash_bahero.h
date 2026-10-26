#ifndef BASH_BASH_BAHERO_H
#define BASH_BASH_BAHERO_H

/* Functions and data defined in src/overlays/bash/bahero.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/bash/bahero.c */

#include "common.h"
#include "bash/bash_ovl.h"
#include "bash/main.h"
#include "bash/pause.h"
#include "bash/heroes.h"

void BashHeroWinner(s32 arg0);

s32 SetHeroSeq(s32 player, s32 seq);

extern u16 *HeroModelTable;
void BashHeroDead(s32 arg0);
void BashHeroBigWeight(s32 arg0);

void SetBashHeroDead(s32 arg0);

void SetBashHeroWinner(s32 arg0);

void SetBashHeroLoser(s32 arg0);

void BashHeroLoser(s32 arg0);

NAS *BashHero2ObjectCollision(s32 player, s32 mask, s32 unused, s32 cone);

void *BashHeroObjectAhead(s32 idx, s32 mask);

void BashShareMomentum(s32 *a, s32 *b, s16 ma, s32 *c, s32 *d, s16 mb);

void SetBashHeroThrow(s32 arg0);

void SetBashHeroCarry(s32 arg0);

void SetBashHeroHold(s32 arg0);

void SetBashHeroPickup(s32 arg0);

s32 BashSpacePadHook(s32 arg0);

extern u8 BashPickupScales[];
s32 func_8008502C();

void BashHeroHit(s32 arg0);

void BashHeroPickup(s32 arg0);

void BashHeroHold(s32 arg0);

void BashHeroCarry(s32 arg0);

void BashHeroThrow(s32 arg0);

void SetBashHeroBigWeight(s32 arg0);

#endif /* BASH_BASH_BAHERO_H */
