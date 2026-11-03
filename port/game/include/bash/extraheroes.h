#ifndef BASH_EXTRAHEROES_H
#define BASH_EXTRAHEROES_H

/* Extra heroes (main/extraheroes.c, PC only): characters the game has but never let anyone play, as heroes of their own, with a card
 * in the character select when the launcher's EXTRA HEROES is on (docs/ROADMAP-B.md, 7.12). Each one borrows, inside, the number of an
 * original hero nobody's playing, so every table the game keeps by hero still works, and plays that hero's moves; its model takes that
 * hero's place, and each frame of the moves is drawn as its own stand-in for that animation, by name. */

#include "common.h"
#include "bash/main.h"

enum { EXTRA_NONE, EXTRA_ROO, NUM_EXTRA };

/* Whether the launcher has them on, or a test run asks for one. */
s32 extra_heroes_on(void);
/* A player's extra hero (EXTRA_), or EXTRA_NONE for one of the original eight. */
s32 extra_hero_of(s32 player);
void extra_hero_pick(s32 player, s32 extra);
/* Its name, in the game's capitals, and its model (HeroModelBank). */
const char *extra_hero_name(s32 extra);
s32 extra_hero_model(s32 extra);
/* By its short name, for a test run's --extra-hero (roo), or EXTRA_NONE. */
s32 extra_hero_named(const char *id);

/* Once the heroes are known, before their models load: each extra hero's base, and the models with theirs in their bases' places. */
void extra_heroes_assign(void);
void extra_heroes_models(void);
/* Whether a model is an extra hero's: it keeps all its animations, as it stands in for others. */
s32 extra_heroes_model_is(const obj_dbase *db);
/* As the heroes are made, and when the level ends. */
void extra_heroes_start(void);
void extra_heroes_stop(void);

/* A move's model switch (0x8006): the base's own model is the extra hero's. */
s32 extra_hero_bank(s32 player, s32 bank);
/* A frame as it's drawn: for an extra hero's own subengine, its stand-in's; anything else as it is. */
s32 extra_hero_frame(const subengine *sub, s32 entity);
/* A frame's vertices, once morphed (draw_object_model): an extra hero's lower, for less of a hop. */
void extra_hero_shape(const subengine *sub, s32 entity, const MeshHeader *mesh, SVECTOR *verts);
/* The points the game finds on an extra hero's frame (sub_point_list): the base's, with what it holds on its head. 0 for anything else. */
s32 extra_hero_points(const subengine *sub, PointList **list);
/* An animation's name hash as an extra hero's model has it: its own stand-in for one it hasn't. */
s32 extra_hero_anim_key(const obj_dbase *db, s32 key);
/* The panel's face of an extra hero's base (DB_Interface's image index, w x h texels), as the extra hero's own; NULL for any other. */
const struct GfxTex *extra_hero_face(s32 index, s32 w, s32 h);

#endif
