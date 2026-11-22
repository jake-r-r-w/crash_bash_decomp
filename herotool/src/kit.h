/* The hero tool's work on one model (docs/HERO_PACKS.md, The hero tool): read, checked against the game's limits (never changed: a
 * model over them is refused), its texture laid out for the game, a skeleton fitted and its corners weighted to it, moves made, and
 * the pack written. The game reads the pack with the same reader the tool reads models with (port/platform/heropack.c). */
#ifndef HEROTOOL_KIT_H
#define HEROTOOL_KIT_H

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "heropack.h"

/* The game's own limits on a hero (port/game/include/bash/heromake.h, MADE_MAX_FACES; extraheroes.c's MORPH_VERTS) */
#define KIT_MAX_TRIS 1365
#define KIT_MAX_VERTS 4096
#define KIT_TEX 256        /* the most texels a texture is across and down: a corner's place in it is a byte */
#define KIT_PAD 2          /* texels of each texture piece's own colours round it */
#define KIT_FPS 30         /* a move's poses a second */

/* The standard skeleton: the bones every pack has (KB_HIPS to KB_FOOT_R) and those it may (a jaw, ears, a tail) */
enum {
    KB_HIPS, KB_SPINE, KB_CHEST, KB_NECK, KB_HEAD,
    KB_UPPERARM_L, KB_FOREARM_L, KB_HAND_L, KB_UPPERARM_R, KB_FOREARM_R, KB_HAND_R,
    KB_THIGH_L, KB_SHIN_L, KB_FOOT_L, KB_THIGH_R, KB_SHIN_R, KB_FOOT_R,
    KB_JAW, KB_EAR_L1, KB_EAR_L2, KB_EAR_R1, KB_EAR_R2, KB_TAIL1, KB_TAIL2, KB_TAIL3,
    KB_COUNT
};
#define KB_NEEDED (KB_FOOT_R + 1)
extern const char *const KitBoneNames[KB_COUNT];
extern const int KitBoneParent[KB_COUNT];

typedef struct {
    float at[3];  /* where it starts, as the model rests */
    float end[3]; /* where it ends (its child's start, or its tip) */
    int have;
} KitBone;

typedef struct {
    /* the mesh, as the model rests, in its own axes (glTF's: y up, facing +z) and units */
    int nvert, ntri;
    float (*pos)[3], (*uv)[2], (*rgba)[4];
    int *image;           /* each vertex's image (its triangles'), -1 none */
    int (*tri)[3];
    int *tri_image;
    PackImage images[PACK_MAX_IMAGES];
    int nimages;
    float lo[3], hi[3];   /* its box */
    /* the skeleton and each vertex's bones */
    KitBone bones[KB_COUNT];
    uint8_t (*joint)[4];
    float (*weight)[4];
    /* the texture, laid out for the game: KIT_TEX x KIT_TEX RGB, and its PNG */
    uint8_t *atlas;
    uint8_t *png;
    size_t png_size;
    /* how high it may raise its arms: degrees from hanging (160 all the way up; less for short arms and a big head) */
    float arm_raise;
    /* its own rig, if it has one (a skinned mesh): its bones' names, parents and places as it rests, and each vertex's bones in it */
    int nsrc;
    char src_name[PACK_MAX_BONES][PACK_ANIM_NAME];
    int src_parent[PACK_MAX_BONES];
    float src_at[PACK_MAX_BONES][3];
    uint8_t (*src_joint)[4];
    float (*src_weight)[4];
    int skinned;
    /* the model as read, for its own clips (kit_clip_pose), and each standard bone's bone in its rig (-1 none) once mapped */
    PackModel *src;
    int src_of[KB_COUNT];
} KitModel;

/* model.c: the model read from its GLB, and checked; 0 and why (said to the person) if it can't be used */
int kit_read(const char *path, KitModel *m, char *why, size_t why_size);
void kit_free(KitModel *m);
/* atlas.c: its texture laid out again, every piece with a border of its own colours, the corners' places moved with them */
int kit_atlas(KitModel *m, char *why, size_t why_size);
/* rig.c: the skeleton from markers (a JSON file of the bones' places, KitBoneNames; docs/HERO_PACKS.md), and each corner's bones */
int kit_markers(KitModel *m, const char *path, char *why, size_t why_size);
void kit_weigh(KitModel *m);
/* rig.c: its own rig, if it has one, mapped onto the standard skeleton by its bones' names (Mixamo's, VRM's, Blender's and the like),
 * its own weights kept; 0 and why if it has none, or not the bones the standard skeleton needs */
int kit_map_rig(KitModel *m, char *why, size_t why_size);
/* moves.c: the standard moves, for any model on the standard skeleton */
typedef struct {
    const char *name;
    float length;  /* seconds */
} KitMove;
extern const KitMove KitMoves[];
extern const int KitMoveCount;
/* move m's pose at t seconds: each bone's turn from its rest (a quaternion, x y z w) and the hips' move (model units) */
void kit_pose(const KitModel *model, int m, float t, float rot[KB_COUNT][4], float hips[3]);
/* rig.c: a model's own clip, as turns of the standard bones from their rest (as kit_pose gives) and the hips' move: for a rigged
 * model, its rig mapped (kit_map_rig) */
int kit_clip_pose(const KitModel *m, int clip, float t, float rot[KB_COUNT][4], float hips[3]);
/* The games' moves where a pack follows its template (port/game/main/packfollow.c's kinds: the same names for every hero), which its
   own moves can take the place of: each kind's name (heropack.c's) and its moves, NULL ended */
#define KIT_OWN_MOST 16
#define KIT_DROP_MOST 64
typedef struct {
    const char *kind;
    const char *moves[8];
} KitGameMoves;
extern const KitGameMoves KitGames[];
extern const int KitGameCount;
/* A move of its own made from what the game made of its template's (the animation tools: a move of its template's is copied as it is
   when it's first changed, so a change of template doesn't change it): a clip named KIND/MOVE (foot/MOVE on foot), frames as the game
   plays them 30 a second, each frame's turns of the standard bones and the hips' move (as kit_pose gives them) */
typedef struct {
    char name[48];
    int frames;
    float length;
    const float (*rot)[KB_COUNT][4];
    const float (*hips)[3];
} KitBaked;
/* write.c: the pack, heroes/<id>/ in a mod folder, with its mod.json */
typedef struct {
    const char *id, *name, *prefer, *mod_name, *author;
    const char *template_hero;          /* the original hero it follows in every game, its moves and what it rides or holds (NULL Crash) */
    const char *kind_template[PACK_KINDS]; /* one game's own in place of that (heropack.c's kinds: the pogo stick or a jetpack), NULL as it */
    int no_fit[PACK_KINDS];               /* a game's "fit": false (not drawn in to sit in a tank's hatch or a race cart) */
    /* its own moves in a game in place of its template's: "KIND/MOVE=OURS", one of the standard moves (KitMoves) written as that game's
       move (a clip named KIND/MOVE, which the game plays in place of following there); NULL ends the list */
    const char *own[KIT_OWN_MOST + 1];
    /* the model's own clips not to write (a GAME/MOVE clip of a pack loaded to change, its move now the template's or another), NULL
       ended */
    const char *drop[KIT_DROP_MOST + 1];
    int height;
    float face_head;
    const char *voice[PACK_VOICE_JOBS]; /* each job's sound (PackVoiceJobs: name, laugh, yelp, ...), a WAV, or NULL for its base's */
    const char *card;                   /* its card in the character select, a PNG, or NULL for its face drawn from it */
    const PackAdjust *adjust;           /* its moves adjusted in the games (hero.json's "adjust": the animation tools) */
    int nadjust;
    const KitBaked *baked;              /* moves of its own copied from its template's (the animation tools), written as clips */
    int nbaked;
} KitPack;
int kit_write(const KitModel *m, const KitPack *p, const char *out, char *why, size_t why_size);
/* rig.c: the markers as a JSON file kit_markers reads, and a first guess at them from the model's shape (a standing humanoid's
 * proportions, in its box), for the window to start from */
int kit_save_markers(const KitModel *m, const char *path);
void kit_guess_markers(KitModel *m);
/* preview.c: what the game made of a pack, a kind's model as --dump-hero writes it (port/game/main/bonusheroes.c, made_dump): its
 * images, its faces (corners, image or -1, each corner's texel and colour, semi-transparent) and each move's frames, nvert vertices a
 * frame in the game's units and axes (y down). The file is little-endian, so it's read as it lies. */
typedef struct {
    int w, h;
    uint8_t *rgba;
} KitMadeImage;
typedef struct {
    int v[3], image;
    uint8_t uv[3][2], rgb[3][3], semi;
} KitMadeFace;
typedef struct {
    char name[32];
    int frames;
    int16_t *verts;
} KitMadeAnim;
typedef struct {
    KitMadeImage *images;
    int nimages;
    KitMadeFace *faces;
    int nfaces, nvert;
    KitMadeAnim *anims;
    int nanims;
} KitMade;
int kit_made_read(const char *path, KitMade *m);
void kit_made_free(KitMade *m);
/* pose.c: what a kind's model is made of, as --dump-hero writes it beside it (port/game/main/packfollow.c, pose_dump_begin): its
 * skeleton, its mesh as it rests at its size there and each frame's pose before its adjustments, so the animation tools can make it
 * again as they're changed (port/platform/heroadjust.c, as the game does) */
#define KIT_POSE_HEAD 24 /* a frame's floats before its bones': hips 3, trunk 9, its move A 9 and b 3 (out = A at + b) */
typedef struct {
    char name[32];
    int frames;
    int own;     /* the pack's own move (its clip), not its template's followed */
    float *pose; /* frames x (KIT_POSE_HEAD + bones x 12): each bone's joint (3) and turn (9, row-major) after the head */
} KitPoseAnim;
typedef struct {
    int nbones, nvert, nprop, hand, cut;
    char (*names)[48];
    int *parent;
    float *bone;          /* 3 a bone, its joint as it rests */
    signed char *part;    /* each bone's part: 1 a leg's, 2 the tail's, 3 an arm's */
    float *rest;          /* 3 a vertex */
    unsigned char *joint; /* 4 a vertex */
    float *weight;        /* 4 a vertex */
    KitPoseAnim *anims;
    int nanims;
} KitPose;
int kit_pose_read(const char *path, KitPose *p);
void kit_pose_free(KitPose *p);
/* gui.cpp: the window, a model to open, the mods folder to write to and the game to test in (each NULL for none, or beside it) */
int kit_gui(const char *model, const char *mods, const char *game);

#ifdef __cplusplus
}
#endif

#endif
