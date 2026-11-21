/* A hero pack (platform/heropack.c; docs/HERO_PACKS.md): a hero of its own in a mod, heroes/<id>/, as the hero tool writes it. Its
 * hero.json, read into a PackInfo, says how it plays; its hero.glb, read into a PackModel, is its mesh, skeleton, clips and textures.
 * The game makes its models of every kind from them (game/main/packheroes.c). */
#ifndef HEROPACK_H
#define HEROPACK_H

#include <stddef.h>
#include <stdint.h>

#define PACK_KINDS 11      /* the game's kinds of hero model, bash/herodef.h's KIND_ */
#define PACK_SOUNDS 26     /* a base's sound offsets, bash/herodef.h's HERO_SOUNDS */
#define PACK_NAME 32
#define PACK_ANIM_NAME 48
#define PACK_MAX_STANDS 48
#define PACK_MAX_BONES 128
#define PACK_MAX_IMAGES 8
#define PACK_MAX_CLIPS 128

/* --- hero.json --- */

/* A stand-in: for its base's animation `from`, its own `to` (bash/herodef.h's Stand), at a pace: 0 stretched over its base's, 1 round
 * and round at its own speed, 2 once at its own speed */
typedef struct {
    char from[PACK_ANIM_NAME], to[PACK_ANIM_NAME];
    int pace;
} PackStand;

typedef struct {
    PackStand stands[PACK_MAX_STANDS];
    int nstands;
    char otherwise[PACK_ANIM_NAME]; /* its stand-in for any other of its base's: empty for BREATHE */
    int keep_lift;                  /* of a frame's lift off the ground, the share drawn (0x1000 all) */
    int like;                       /* the original hero (1 Crash to 8 Rilla Roo) whose weight and speeds it has, 0 its base's */
    int subframes;                  /* its frames to each tick of its clips' (1 to 4) */
} PackKind;

typedef struct {
    char name[PACK_NAME];      /* in the game's capitals */
    char id[PACK_NAME];
    int format;                /* hero.json's format, 1 */
    int prefer;                /* the original hero it borrows if nobody's playing that one, 1 Crash to 8 Rilla Roo */
    int height;                /* standing, in the game's units (Crash 452, most about 650) */
    PackKind kinds[PACK_KINDS];
    int voices[PACK_SOUNDS];   /* each of its base's sound offsets: PACK_VOICE_ (its own sound for a job, its base's, none, or not said) */
    char face[PACK_ANIM_NAME]; /* the clip its face is drawn from if it has no card */
    float face_turn;
    float face_head;           /* the share of its model, from the top, that's its head, for that face (0: the game's own share) */
} PackInfo;

/* Its sounds: a file for each job the game has a hero's sound do (voice_<job>.wav, PackVoiceJobs), and for each of its base's 26
 * sound offsets which plays: hero.json's "voices", {"offset": "job", "base" or "none"}; an offset it doesn't name plays the job
 * it does for the game's own heroes, if the pack has that file, else its base's */
enum { PACK_VOICE_UNSAID = -1, PACK_VOICE_NONE, PACK_VOICE_BASE, PACK_VOICE_JOB };
#define PACK_VOICE_JOBS 8
extern const char *const PackVoiceJobs[PACK_VOICE_JOBS]; /* name, laugh, yelp, jump, land, thud, throw, fire */
#define PACK_SOUND_RATE 22050
/* A WAV as mono 16-bit samples at PACK_SOUND_RATE (pack_samples_free them), and how many; NULL if it isn't one */
int16_t *pack_wav_read(const uint8_t *wav, size_t size, int *frames);
/* Samples as PS1 ADPCM, as the game plays its own (to free), and its bytes */
uint8_t *pack_adpcm_encode(const int16_t *pcm, int frames, int *bytes);
void pack_samples_free(int16_t *pcm);

/* hero.json's text into out; 0 and why (in capitals) if it isn't one. */
int pack_info_read(const char *text, size_t size, PackInfo *out, char *problem, size_t problem_size);
/* A kind's name in hero.json ("foot", "polar", ...), and its number from one; -1 if it isn't one */
const char *pack_kind_name(int kind);
int pack_kind_named(const char *name);

/* --- hero.glb --- */

/* A bone's place relative to its parent: translation, rotation (a quaternion, x y z w) and scale */
typedef struct {
    float t[3], r[4], s[3];
} PackTrs;

typedef struct {
    char name[PACK_ANIM_NAME];
    int parent;          /* -1 for a root */
    PackTrs rest;
    float rest_world[16]; /* where it is as the mesh rests (column-major, as glTF's), */
    float rest_inv[16];   /* and the inverse */
} PackBone;

typedef struct {
    int bone, path;      /* path 0 translation, 1 rotation, 2 scale */
    int step;            /* held from key to key, not blended */
    int n;               /* keys */
    float *time, *value; /* n times, and n values of 3 or 4 floats */
} PackTrack;

typedef struct {
    char name[PACK_ANIM_NAME]; /* KIND/MOVE, or MOVE alone for on foot */
    float length;              /* seconds */
    PackTrack *tracks;
    int ntracks;
} PackClip;

typedef struct {
    int w, h;
    uint8_t *rgba; /* w * h * 4 */
} PackImage;

typedef struct {
    int nvert;
    float *pos;        /* 3 a vertex, as the mesh rests, in glTF's axes (y up, metres, facing +z) */
    float *uv;         /* 2 a vertex */
    float *rgba;       /* 4 a vertex: its colour times its material's */
    uint8_t *joint;    /* 4 a vertex: the bones it goes with, */
    float *weight;     /* and how much (summing to 1) */
    int ntri;
    int *tri;          /* 3 a triangle */
    int *tri_image;    /* each one's image, -1 none */
    uint8_t *tri_blend; /* each one's drawn blended (its material's alpha mode BLEND) */
    uint8_t *tri_unlit; /* or unlit (KHR_materials_unlit) */
    PackBone bones[PACK_MAX_BONES];
    int nbones;
    PackClip clips[PACK_MAX_CLIPS];
    int nclips;
    PackImage images[PACK_MAX_IMAGES];
    int nimages;
} PackModel;

/* A GLB's bytes into out (pack_model_free after); 0 and why if it can't. Its images are PNGs. */
int pack_model_read(const uint8_t *glb, size_t size, PackModel *out, char *problem, size_t problem_size);
void pack_model_free(PackModel *m);
/* A clip by name (any case), or -1 */
int pack_clip_find(const PackModel *m, const char *name);
/* Each bone's place relative to its parent at t seconds into clip `clip` (-1 the rest pose): its tracks' keys blended, a bone with none
 * as it rests */
void pack_pose_local(const PackModel *m, int clip, float t, PackTrs *local);
/* Those places into where each bone is (world, nbones matrices of 16) */
void pack_pose_world(const PackModel *m, const PackTrs *local, float (*world)[16]);
/* The mesh's vertices where those bones put them (3 a vertex, glTF's axes) */
void pack_skin(const PackModel *m, const float (*world)[16], float *out);

/* A PNG (a card) as w x h pixels, red | green << 8 | blue << 16 | alpha << 24, to free; NULL if it isn't one */
uint32_t *pack_png_read(const uint8_t *png, size_t size, int *w, int *h);

/* 4x4 column-major helpers, for posing a skeleton by hand */
void pack_trs_matrix(const PackTrs *p, float out[16]);
void pack_mat_mul(const float a[16], const float b[16], float out[16]);
void pack_mat_point(const float m[16], const float p[3], float out[3]);
int pack_mat_invert(const float m[16], float out[16]);

#endif
