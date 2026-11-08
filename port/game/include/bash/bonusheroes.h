#ifndef BASH_BONUSHEROES_H
#define BASH_BONUSHEROES_H

/* Bonus heroes (main/bonusheroes.c, PC only): the extra heroes' models, sounds and pictures, made from the player's own copies of other
 * Crash games by each hero's own file (main/heroes/, bash/herodef.h) and put in the archive, so an extra hero (extraheroes.c) looks and
 * sounds as it did there. Without the disc, there's none. */

#include "common.h"

/* As the archive's laid out (platform/mods.c, mods_apply_files): each hero is made, the first time, and its models' files go in their
 * entries. */
void bonus_heroes_files(void);
/* Whether an extra hero (EXTRA_) was made, and the bank of its model of a kind (KIND_; its model on foot for a kind it has none of). */
s32 bonus_heroes_have(s32 extra);
s32 bonus_heroes_bank(s32 extra, s32 kind);
/* Whether it has a model of its own of a kind (not its model on foot in its place). */
s32 bonus_heroes_made(s32 extra, s32 kind);
/* Its model file of a kind, whole, to free. */
u8 *bonus_heroes_model(s32 extra, s32 kind);
/* Its face from its game, if that has one: w x h pixels (red | green << 8 | blue << 16 | alpha << 24), its head the top `head` rows,
 * what hangs from it (Roo's tongue) the rest; NULL for none. */
const u32 *bonus_heroes_card(s32 extra, s32 *w, s32 *h, s32 *head);
/* How wide its card's pixels are shown against their height (HeroMade.card_aspect), 0 for stretched to fill */
float bonus_heroes_card_aspect(s32 extra);
/* One of its sounds (VOICE_), as it plays in place of its base's: its laugh one at a time, a new one starting it again. */
void bonus_heroes_voice(s32 extra, s32 voice);

#endif
