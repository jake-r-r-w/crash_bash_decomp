#ifndef BASH_BONUSHEROES_H
#define BASH_BONUSHEROES_H

/* Bonus heroes (main/bonusheroes.c, PC only): extra heroes' models from the player's own copies of other Crash games, made into Crash
 * Bash's and put in the archive, so an extra hero (extraheroes.c) can look as it did there. Without the disc, it's Crash Bash's own. */

#include "common.h"

/* As the archive's laid out (platform/mods.c, mods_apply_files): the bonus models' files in their entries, for the discs there are. */
void bonus_heroes_files(void);
/* Whether an extra hero (EXTRA_) has its bonus model, and which bank it's in. */
s32 bonus_heroes_have(s32 extra);
s32 bonus_heroes_bank(s32 extra);
/* Its model file, whole, to free. */
u8 *bonus_heroes_model(s32 extra);
/* Its face from its game, if that has one: w x h pixels (red | green << 8 | blue << 16 | alpha << 24), each stretch times as tall as
 * it's wide as its game showed it; NULL for none. */
const u32 *bonus_heroes_card(s32 extra, s32 *w, s32 *h, float *stretch);
/* Its voice from its game, Roo's laugh: as it's picked, taunts and wins. Again, one that's going starts again; else it finishes. */
void bonus_heroes_cheer(s32 extra, s32 again);

#endif
