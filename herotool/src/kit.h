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
/* write.c: the pack, heroes/<id>/ in a mod folder, with its mod.json */
typedef struct {
    const char *id, *name, *prefer, *mod_name, *author;
    int height;
    float face_head;
    const char *voice[PACK_VOICE_JOBS]; /* each job's sound (PackVoiceJobs: name, laugh, yelp, ...), a WAV, or NULL for its base's */
    const char *card;                   /* its card in the character select, a PNG, or NULL for its face drawn from it */
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
/* gui.cpp: the window, a model to open, the mods folder to write to and the game to test in (each NULL for none, or beside it) */
int kit_gui(const char *model, const char *mods, const char *game);

#ifdef __cplusplus
}
#endif

#endif
