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
    int template_hero;              /* the original hero whose model of this kind it follows, its moves and what it rides or holds
                                     * (1 Crash to 8 Rilla Roo; PackInfo.template_hero if 0): docs/HERO_PACKS.md, Templates. -1
                                     * ("own"): none, its own moves there, though it says a template (on foot) */
    int no_fit;                     /* "fit": false: not drawn in to sit in a tank's hatch or a race cart (its own adjustments fit it,
                                     * hiding bones: the user's word, 2026-11-22) */
} PackKind;

/* A change to its pose in a game's move, on top of what the game makes of it there (hero.json's "adjust", made with the hero tool's
 * animation tools: docs/HERO_PACKS.md, Adjusting its moves; platform/heroadjust.c applies them). Its turns and moves are about the
 * hero's own axes as it stands at rest, in the game's: x across (to its left), y down, z to its back. Several at a frame add up. */
#define PACK_MAX_ADJUSTS 1024 /* (Bobby has 704: his ears flop in every jump; 256 till 2026-11-23) */
#define PACK_ADJUST_NAME 32
typedef struct {
    int kind;                       /* PACK_KINDS' */
    char move[PACK_ADJUST_NAME];    /* the move of its model of that kind, as the game made it (the tool's list), or "*" every one */
    char bone[PACK_ADJUST_NAME];    /* its bone, by name (hips, chest, upperarm_l ...: the hero tool's) */
    int at;                         /* the frame of that move it's at, or -1 for every frame */
    int until;                      /* held there to this frame, all of it from at to until ("until"; at or before at: at alone) */
    int spread;                     /* the frames either side it fades out over (0: those frames alone) */
    int ease;                       /* how it fades in and out there, PACK_EASE_ ("ease"; 0 smooth) */
    int blend;                      /* blended into its bone's next key in the move by its ease, not faded out ("blend": true) */
    float turn[3];                  /* that bone and all on it turned about those axes (degrees, a turn's axis times its angle) */
    float reach[3];                 /* moved (the game's units): a hand or foot, its arm or leg bent to it; the hips, all of it; any
                                     * other bone, it and all on it */
    float grow[3];                  /* that bone and all on it made bigger along those axes, as a share more ("scale" less 1: 0 none) */
} PackAdjust;

typedef struct {
    char name[PACK_NAME];      /* in the game's capitals */
    char id[PACK_NAME];
    int format;                /* hero.json's format, 1 */
    int prefer;                /* the original hero it borrows if nobody's playing that one, 1 Crash to 8 Rilla Roo */
    int height;                /* standing, in the game's units (Crash 452, most about 650) */
    int template_hero;         /* the original hero it follows in every kind ("template"; 1 Crash, the first, to 8 Rilla Roo), each move
                                * of its own clips (KIND/MOVE) in that move's place */
    int template_said;         /* whether hero.json says so: only then does it follow it on foot too (a pack from before keeps its own) */
    PackKind kinds[PACK_KINDS];
    int voices[PACK_SOUNDS];   /* each of its base's sound offsets: PACK_VOICE_ (its own sound for a job, its base's, none, or not said) */
    char face[PACK_ANIM_NAME]; /* the clip its face is drawn from if it has no card */
    float face_turn;
    float face_head;           /* the share of its model, from the top, that's its head, for that face (0: the game's own share) */
    PackAdjust adjust[PACK_MAX_ADJUSTS]; /* its moves adjusted ("adjust") */
    int nadjust;
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

/* How a change fades in and out over its spread (PackAdjust.ease; the user's word, 2026-11-22: "the transion effect should be
 * chooseable"): smooth (a cosine, the first), steady, slow at its far end, slow into its frames, none (all of it there and none either
 * side), past it and back, bouncing, wobbling. hero.json's names for them, and a name's number (-1 none). */
enum { PACK_EASE_SMOOTH, PACK_EASE_LINEAR, PACK_EASE_SLOW_START, PACK_EASE_SLOW_FINISH, PACK_EASE_SUDDEN, PACK_EASE_OVERSHOOT,
       PACK_EASE_BOUNCE, PACK_EASE_WOBBLE, PACK_EASES };
const char *pack_ease_name(int ease);
int pack_ease_named(const char *name);

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

/* A file of moves adjusted alone, {"adjust": [...]} as hero.json has them: how many of most it has, into out; -1 (and why) if it isn't
 * one (the hero tool's --adjust) */
int pack_adjusts_read(const char *text, size_t size, PackAdjust *out, int most, char *problem, size_t problem_size);

/* A PNG (a card) as w x h pixels, red | green << 8 | blue << 16 | alpha << 24, to free; NULL if it isn't one */
uint32_t *pack_png_read(const uint8_t *png, size_t size, int *w, int *h);

/* Its card (cw x ch, as pack_png_read gives) as the game shows it (extraheroes.c), here so the hero tool's preview is the same:
 * pack_card_face draws it into the top left fw x fh of a picture stride wide: across it but inset of its width each side, a card row
 * rows texels tall from oy, and none of it from row stop on; each pixel the mean of the card's under it (4 x 4 samples).
 * pack_select_picture: a select's portrait, fw x fh, its top left pw x ph a gradient from yellow to orange with face (fw wide, or NULL)
 * over it, the rest clear. pack_card_select: the card in a portrait w x h texels (the select's 64 x 64), pack_card_panel: on the panel,
 * the face over its health bar (64 x 32): the top of the card, unsquashed */
#define PACK_SELECT_ROWS 38      /* of a portrait's 64 rows, those the game's own use, and of its 64 columns: the rest is clear, and */
#define PACK_SELECT_COLS 56      /* the frame's border shows there */
#define PACK_SELECT_INSET 0.05f  /* a card's room each side, so its ears clear the frame */
#define PACK_TEXEL_TALL 1.6f     /* how much taller than wide the select and the panel show a texel (512 x 240 on 4:3; widescreen too) */
void pack_card_face(const uint32_t *card, int cw, int ch, uint32_t *out, int stride, int fw, int fh, float inset, float rows, float oy, int stop);
void pack_select_picture(uint32_t *out, int fw, int fh, int pw, int ph, const uint32_t *face);
void pack_card_select(const uint32_t *card, int cw, int ch, uint32_t *out, int w, int h);
void pack_card_panel(const uint32_t *card, int cw, int ch, uint32_t *out, int w, int h);

/* 4x4 column-major helpers, for posing a skeleton by hand */
void pack_trs_matrix(const PackTrs *p, float out[16]);
void pack_mat_mul(const float a[16], const float b[16], float out[16]);
void pack_mat_point(const float m[16], const float p[3], float out[3]);
int pack_mat_invert(const float m[16], float out[16]);

#endif
