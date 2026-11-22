/* A hero pack's posed skeleton adjusted (hero.json's "adjust", PackAdjust: docs/HERO_PACKS.md, Adjusting its moves) and its mesh
 * skinned to it, in the game's axes and units (y down). The game makes a pack's models with these (game/main/packfollow.c), and the
 * hero tool draws its preview with them (herotool/src/gui.cpp), so what the tool shows as it's changed is what the game makes.
 *
 * A pose is each bone's joint (P) and its turn from its rest (Q, row-major 3x3: row r, column c at Q[r * 3 + c]). */
#ifndef HEROADJUST_H
#define HEROADJUST_H

#include "heropack.h"

#ifdef __cplusplus
extern "C" {
#endif

/* How far either side of its hips what's below a tank's hatch is drawn in to (the game's units), so it stays inside the tank */
#define PACK_HATCH_HALF 55.0f

/* A skeleton: its bones' names and parents (-1 a root) */
typedef struct {
    int nbones;
    const char *name[PACK_MAX_BONES];
    int parent[PACK_MAX_BONES];
} PackBones;

/* Whether an adjustment is for a kind's move (its name in any case; "*" every move of the kind) */
int pack_adjust_for(const PackAdjust *a, int kind, const char *move);
/* How much of it there is at a frame of its move: all of it at its own frame, fading out over its spread; all of it at every frame
 * if it's at none */
float pack_adjust_share(const PackAdjust *a, int frame);
/* A bone by name (any case), or -1 */
int pack_bone_named(const PackBones *bones, const char *name);
/* The adjustments for a kind's move applied at a frame of it, to a pose (P, Q), the hero's trunk turned by trunk from its rest (its
 * hips' turn before them): first the hips' reach (all of it moved), then each bone's turn, those nearest the hips first (each one
 * with all on it, about its joint), then each hand's and foot's reach (its arm or leg bent to it). 0 if none were there. */
int pack_adjust_pose(const PackAdjust *adj, int nadj, int kind, const char *move, int frame, const PackBones *bones,
                     const float trunk[9], float (*P)[3], float (*Q)[9]);
/* A bone and all on it turned by M (row-major) about where its joint is */
void pack_turn_under(const PackBones *bones, int bone, const float M[9], float (*P)[3], float (*Q)[9]);

/* Its mesh as it rests at its size on what it rides (the game's axes and units), and what bends it */
typedef struct {
    int nvert;
    const float *rest;          /* 3 a vertex */
    const float *bone;          /* 3 a bone: its joint as it rests */
    const unsigned char *joint; /* 4 a vertex: its bones, */
    const float *weight;        /* and how much */
    const signed char *part;    /* each bone's part: 1 a leg's, 2 the tail's, 3 an arm's, 0 its body's (where it sits in something) */
} PackSkin;
/* Its mesh skinned to a pose (out, 3 a vertex). cut: it sits in a tank's hatch or a race cart, its tail folded into its hips (hips_at)
 * and its legs and all of it below its hips (but its arms) drawn in to PACK_HATCH_HALF of them where they are, down inside what it
 * sits in (the user's word, 2026-11-22). */
void pack_skin_posed(const PackSkin *s, const float (*P)[3], const float (*Q)[9], const float hips_at[3], int cut, float *out);

#ifdef __cplusplus
}
#endif

#endif
