#ifndef BASH_PACKTEMPLATE_H
#define BASH_PACKTEMPLATE_H

/* A hero pack's template (PC only; docs/HERO_PACKS.md, Templates): the original hero whose model of each kind a pack's hero follows,
 * its moves and what it rides or holds. Crash's kinds are packfollow.c's own tables; each other hero's are maps of markers on its
 * model of that kind (main/packtemplates.c), each joint a point in its first frame, the corners near it that joint's. */

#include "common.h"

typedef struct {
    float x, y, z; /* Crash Bash's units and axes, y down, as the model first moves (its first move's first frame) */
} TmplPoint;

/* A box of the model, lo to hi; sym: its x is either side (lo.x to hi.x out from the middle, left or right) */
typedef struct {
    TmplPoint lo, hi;
    s32 sym;
} TmplBox;
#define TMPL_BOXES 4

/* Each pair is its left first (x below 0). A point left at 0, 0, 0 isn't there. */
typedef struct {
    s32 hero;      /* 2 Coco to 8 Rilla Roo (heropack.c's numbers) */
    s32 kind;      /* KIND_ */
    s32 bank;      /* its model of the kind (DB_Chars...) */
    float r;       /* a joint's corners are those within r of its marker */
    TmplPoint hips, chest, head, shoulder[2], elbow[2], hand[2], hip[2], knee[2], ankle[2], foot[2];
    TmplPoint grip[2], peg[2]; /* on what it rides or holds, where Crash's kind has them */
    TmplBox prop[TMPL_BOXES]; /* what it rides or holds: the strips with most of their corners in one of these */
    float scale;   /* the pack's size on it against Crash's kind's for this hero's height (PackTemplateHeight), 1 as it is */
    s32 mesh1;     /* the mesh of the model it follows, plus 1 (0: Crash's kind's: a hero's model may have fewer) */
    s32 carry;     /* the hand it carries its prop in, 1 its left, 2 its right (0: Crash's kind's: Cortex has his keg on his right) */
} TemplateMap;

/* Each original hero's height on foot (its model of it, crate2), 1 Crash to 8 Rilla Roo */
extern const float PackTemplateHeight[9];
/* The map of a hero's model of a kind, or NULL if there's none yet (the pack follows Crash's there) */
const TemplateMap *pack_template_map(s32 hero, s32 kind);

#endif
