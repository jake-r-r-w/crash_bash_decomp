#ifndef BASH_EXTRAHEROES_H
#define BASH_EXTRAHEROES_H

/* Extra heroes (main/extraheroes.c, PC only): characters from the player's own copies of other Crash games, as heroes of their own,
 * with a card in the character select when the launcher's EXTRA HEROES is on and the disc is in (docs/ROADMAP-B.md, 7.12). Each one borrows, inside, the number of an
 * original hero nobody's playing, so every table the game keeps by hero still works, and plays that hero's moves; its model takes that
 * hero's place, and each frame of the moves is drawn as its own stand-in for that animation, by name. */

#include "common.h"
#include "bash/main.h"
#include "bash/herolist.h"

/* Each extra hero's number, from bash/herolist.h's list (EXTRA_ROO, ...) */
#define EXTRA_NUMBER(name, def) EXTRA_##name,
enum { EXTRA_NONE, EXTRA_HERO_LIST(EXTRA_NUMBER) NUM_EXTRA };

/* Whether the launcher has them on, or a test run asks for one. */
s32 extra_heroes_on(void);
/* A player's extra hero (EXTRA_), or EXTRA_NONE for one of the original eight. */
s32 extra_hero_of(s32 player);
void extra_hero_pick(s32 player, s32 extra);
/* Its name, in the game's capitals, and its model (HeroModelBank). */
const char *extra_hero_name(s32 extra);
s32 extra_hero_model(s32 extra);
/* A kind of model (herodef.h's KIND_) in words, for the log: "on foot", "riding a polar bear", ... */
const char *extra_hero_kind_name(s32 kind);
/* By its short name, for a test run's --extra-hero (roo), or EXTRA_NONE. */
s32 extra_hero_named(const char *id);
/* Whether it's there to play: its model came from the player's disc of its game (bonusheroes.c). */
s32 extra_hero_available(s32 extra);
/* The original hero it would rather borrow, if nobody's playing that one (HeroDef.prefer). */
s32 extra_hero_prefer(s32 extra);
/* The extra heroes with a card in the character select, in bash/herolist.h's order, into heroes (if not NULL), EXTRA_CARDS_MAX at
 * most; how many. None unless the launcher has them on. */
s32 extra_heroes_cards(s32 *heroes);

/* Once the heroes are known, before their models load: each extra hero's base, and the models with theirs in their bases' places. */
void extra_heroes_assign(void);
void extra_heroes_models(void);
/* Whether a model is an extra hero's: it keeps all its animations, as it stands in for others. */
s32 extra_heroes_model_is(const obj_dbase *db);
/* As the heroes are made, and when the level ends. */
void extra_heroes_start(void);
void extra_heroes_stop(void);

/* A move's model switch (0x8006): the base's own model of a kind is the extra hero's. */
s32 extra_hero_bank(s32 player, s32 bank);
/* The model a subengine's drawn with: an extra hero's own, where the game has given it its base's (Polar Push's, by hero), or as it is. */
const obj_dbase *extra_hero_dbase(const subengine *sub, const obj_dbase *db);
/* The hero whose weight and speeds a player's hero `name` has in this game: an extra hero's kind can be another's (Roo's on his bear,
 * Crash's), and anyone else is themselves. */
s32 extra_hero_like(s32 player, s32 name);
/* Any sound (PlaySfx): 1 if it's one of an extra hero's base's own (its block: its moves', hits', taunt's and cheers', and the announcer
 * naming it as it wins) and the extra hero's own has played instead, or none. */
s32 extra_hero_sound(s32 sfx);
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

/* The character select (titles.c): each of the n heroes' cards has its portrait in portraits[i], and its model loads, while the select's
 * up; what a pick plays on its stand; and its portrait as the subengine that shows it is drawn (draw.c), NULL for any other. */
void extra_heroes_select_enter(const subengine *const *portraits, const s32 *heroes, s32 n);
void extra_heroes_select_leave(void);
void *extra_hero_select_steps(s32 extra);
s32 extra_hero_select_scale(s32 extra);
const struct GfxTex *extra_hero_portrait(const subengine *sub, s32 w, s32 h);
/* The menus after it, which show the heroes' portraits by number (DB_MainmenuModels' image index): an extra hero's in its base's place. */
const struct GfxTex *extra_hero_menu_portrait(s32 index, s32 w, s32 h);

#endif
