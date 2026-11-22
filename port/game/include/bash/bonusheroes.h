#ifndef BASH_BONUSHEROES_H
#define BASH_BONUSHEROES_H

/* Bonus heroes (main/bonusheroes.c, PC only): the extra heroes' models, sounds and pictures, made from the hero packs of the mods that
 * are on (main/packheroes.c, docs/HERO_PACKS.md) and put in the archive. */

#include "common.h"

/* As the archive's laid out (platform/mods.c, mods_apply_files): each hero is made, the first time, and its models' files go in their
 * entries. */
void bonus_heroes_files(void);
/* Whether an extra hero (EXTRA_) was made, and the bank of its model of a kind (KIND_; its model on foot for a kind it has none of). */
s32 bonus_heroes_have(s32 extra);
s32 bonus_heroes_bank(s32 extra, s32 kind);
/* Whether it has a model of its own of a kind (not its model on foot in its place). */
s32 bonus_heroes_made(s32 extra, s32 kind);
/* Its model file of a kind, whole, to free; and its texture file. */
u8 *bonus_heroes_model(s32 extra, s32 kind);
u8 *bonus_heroes_texture(s32 extra, s32 kind, s32 *size);
/* Its face from its game, if that has one: w x h pixels (red | green << 8 | blue << 16 | alpha << 24), its head the top `head` rows,
 * what hangs from it (Roo's tongue) the rest; NULL for none. */
const u32 *bonus_heroes_card(s32 extra, s32 *w, s32 *h, s32 *head);
/* How wide its card's pixels are shown against their height (HeroMade.card_aspect), 0 for stretched to fill */
float bonus_heroes_card_aspect(s32 extra);
/* One of its sounds (VOICE_), as it plays in place of its base's: its laugh one at a time, a new one starting it again. */
void bonus_heroes_voice(s32 extra, s32 voice);
/* What was made of it let go, to be made again (a pack's slot when the packs in it change). */
void bonus_heroes_forget(s32 extra);

/* The heroes of the mods that are on (main/packheroes.c, docs/HERO_PACKS.md) into the pack slots (EXTRA_PACK1 on), each made from
 * its pack as bonus_heroes_files makes it: done again each time it's called, and a slot whose pack changed made again. */
void pack_heroes_scan(void);

/* What's made written to FOLDER/<hero id>_<KIND>.hpv as it's made, for the hero tool's preview (--dump-hero; docs/HERO_PACKS.md, The
 * preview): every model of every kind, its faces, images and every frame, as herotool/src/preview.c reads them. */
void bonus_heroes_dump_to(const char *folder);
/* That folder, or NULL if there's none (packfollow.c writes each kind's poses there too: <id>_<KIND>.pose) */
const char *bonus_heroes_dump_folder(void);
/* And what the game draws with a hero beside its own model, the Ballistix car and the tank: FOLDER/<id>_<KIND>_ride<N>.hpv */
void bonus_heroes_dump_rides(const char *id);

#endif
