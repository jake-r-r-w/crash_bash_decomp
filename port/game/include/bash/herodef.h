#ifndef BASH_HERODEF_H
#define BASH_HERODEF_H

/* An extra hero, as its own file in main/heroes/ defines it (PC only): how it plays (extraheroes.c), and how it's made from the player's
 * own copy of its game into Crash Bash's models, sounds and picture (its make, with bash/heromake.h's help). Ripper Roo (heroes/roo.c)
 * is the one the others follow: docs/EXTRA_HEROES.md goes through him, a part at a time. */

#include "common.h"

/* The kinds of model a hero has, one for each kind the games' own heroes have: on foot (Crate Crush, and the games that have no model of
 * their own for it), and riding a polar bear (Polar Push). Each loads where the games load their heroes' of that kind. */
enum { KIND_FOOT, KIND_POLAR, NUM_KINDS };

/* How a stand-in's frames go as its base's animation plays: the same share of the way through (STRETCH), or at the stand-in's own speed,
 * a frame a tick from when the base's started, round and round (LOOP) or once, its last frame then held (ONCE). */
enum { STRETCH, LOOP, ONCE };

/* For its base's animation `from`, its own `to`: a list of names, the first its model has, and a name can have a range of its frames
 * (NAME:first-last). */
typedef struct {
    const char *from, *to;
    s32 pace;
} Stand;

/* Its sounds. Each original hero's are a block of sound ids of its own, from the one the announcer names it with, 50 apart, and the
 * games play the ones for its moves from the first HERO_SOUNDS of it (docs/EXTRA_HEROES.md has the list; the last of a block is a menu's
 * in one); an extra hero's base's are all its own instead, by offset in the block: VOICE_BASE keeps the base's (the announcer's name),
 * and one it has none for (VOICE_NONE, and any offset not listed) is quiet. */
enum { VOICE_NONE, VOICE_BASE, VOICE_LAUGH, VOICE_YELP, VOICE_JUMP, VOICE_LAND, VOICE_THUD, VOICE_THROW, NUM_VOICES };
#define HERO_SOUNDS 26
typedef struct {
    s32 offset, voice;
} Voice;

/* How it plays in a kind of game */
typedef struct {
    const Stand *stands;   /* its stand-ins for its base's animations (ended by from NULL); one its model has by the same name is its own */
    const char *otherwise; /* its stand-in for any other */
    s32 keep_lift;         /* of a frame's lift off the ground, the share it's drawn with: 0x1000 all */
    s32 like;              /* the original hero whose weight and speeds it has, 0 for its base's */
} HeroKind;

/* A sound of its own, as its game has it: PS1 ADPCM (16-byte blocks of 28 samples), played at pitch (0x1000 is 44100 Hz). */
typedef struct {
    const u8 *adpcm;
    s32 bytes;
    u32 pitch;
} HeroSound;

/* What its make makes, all kept in memory: a model and texture set of each kind it has (docs/ASSETS.md; NULL for none), its sounds, and
 * its face from its game if it has one (w x h, red | green << 8 | blue << 16 | alpha << 24: its head the top `card_head` rows). */
typedef struct {
    u8 *tex[NUM_KINDS], *mdl[NUM_KINDS];
    s32 tex_size[NUM_KINDS], mdl_size[NUM_KINDS];
    HeroSound sound[NUM_VOICES];
    const u32 *card;
    s32 card_w, card_h, card_head;
} HeroMade;

typedef struct HeroDef {
    const char *name;    /* in the game's capitals */
    const char *id;      /* for a test run's --extra-hero */
    const char *disc;    /* its game's disc, by its serial (disc_image_find) */
    s32 prefer;          /* the original hero it borrows, if nobody's playing that one */
    HeroKind kinds[NUM_KINDS];
    const Voice *voices; /* ended by offset -1 */
    const char *face;    /* the animation its face on the panel and the select is drawn from (frame 0), if it has no card */
    float face_turn;     /* and how far round it's turned, in degrees */
    /* Makes it from its game's disc into out; 0 if it can't (and out's left empty). */
    s32 (*make)(HeroMade *out);
} HeroDef;

/* The heroes there are (bonusheroes.c), by EXTRA_ number: [0] is none. */
extern const HeroDef *const HeroDefs[];

#endif
