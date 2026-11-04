#ifndef BASH_HEROMAKE_H
#define BASH_HEROMAKE_H

/* Making an extra hero (PC only; bonusheroes.c): what a hero's file (main/heroes/) uses to read its game's disc, and to make what it
 * finds there into Crash Bash's models, on foot and riding a polar bear. docs/EXTRA_HEROES.md shows Ripper Roo's use of each part. */

#include "common.h"
#include "bash/herodef.h"
#include "../../../platform/platform.h"

/* --- bytes --- */
u32 hm_rd32(const u8 *p);
u16 hm_rd16(const u8 *p);
void hm_wr32(u8 *p, u32 v);
void hm_wr16(u8 *p, u32 v);

/* A grown buffer of bytes: put returns where it put them (src NULL: zeros) */
typedef struct {
    u8 *b;
    size_t n, cap;
} Bytes;
size_t hm_put(Bytes *o, const void *src, size_t n);
void hm_align(Bytes *o, size_t to);
/* a self offset: from the field at `field` to `to` */
void hm_self(Bytes *o, size_t field, size_t to);

/* --- a point, and turning one about another --- */
typedef struct {
    float x, y, z;
} V3;
float hm_smooth(float a, float b, float x); /* 0 at a, 1 at b (either way round), smoothly between */
V3 hm_mix(V3 a, V3 b, float t);
V3 hm_turn_x(V3 p, V3 c, float a); /* a radians about the axis through c along x, y or z */
V3 hm_turn_y(V3 p, V3 c, float a);
V3 hm_turn_z(V3 p, V3 c, float a);

/* --- Crash Bandicoot (1996)'s levels, as CrashEdit (the cbhacks project) reads them ---
 * A level's NSF is 64 KB pages, each raw (magic 0x1234) or compressed (0x1235: runs and repeats for its first `length` bytes, `skip`
 * bytes to pass, the rest raw). A page of entries lists them, and each entry its items; a texture page is 256 x 128 16-bit units of
 * VRAM. An animation's items are its frames: a 56-byte header (vertex count, model, x, y, z offsets, collision) and 6 bytes a vertex
 * (x, y, z unsigned, a normal signed), each at (xyz + offset - 128) * the model's scale. A model's first item has its triangle count,
 * its scale and its colours (4-byte slots: r, g, b, flags) and textures (3 slots: the colour with byte 3's top bit, the texture page,
 * and where in it), its second 8 bytes a triangle: three vertices' byte offsets and its slot. */
#define C1_PAGE_BYTES 65536
typedef struct {
    u8 *pages; /* count of C1_PAGE_BYTES */
    s32 count;
} C1Level;
typedef struct {
    s32 type, count;
    const u8 *item[128];
    s32 size[128];
} C1Entry;
s32 c1_level_read(DiscImage *disc, const char *path, C1Level *out); /* free(out->pages) after */
s32 c1_entry_find(const C1Level *l, const char *name, C1Entry *out);
const u8 *c1_texture_page(const C1Level *l, u32 eid);
/* A model's scale (its first item's), and a frame's vertices in Crash Bash's axes (y down, z the other way), not scaled to its units
 * yet; with their normals, if `normals` isn't NULL. The count, or 0. */
void c1_model_scale(const C1Entry *model, s32 scale[3]);
s32 c1_frame_verts(const u8 *frame, s32 size, const s32 scale[3], V3 *out, V3 *normals, s32 max);
/* A sound entry's ADPCM (11025 Hz, PS1's), to free, and its size. */
u8 *c1_sound(const C1Level *l, const char *name, s32 *size);
/* The 64 x 64 sprite an object's item `item` shows first (its HUD face, for one), into out; 0 if there's none. */
s32 c1_sprite64(const C1Level *l, const char *object, s32 item, u32 out[64 * 64]);

/* --- a made model: its faces, images and animations, written as Crash Bash's (docs/ASSETS.md) --- */
#define MADE_MAX_IMAGES 32
/* The most frames an animation can have: the games name a frame 0x4000 | animation << 7 | frame, so a frame past 127 would be another
 * animation's (or none, and the model isn't drawn). made_write refuses a longer one. */
#define MADE_MAX_FRAMES 128
typedef struct {
    s32 v[3];          /* its corners: vertices of each frame */
    s32 image;         /* in the made texture set, -1 untextured */
    u8 uv[3][2];       /* each corner's place in its image, texels */
    u8 rgb[2][3][3];   /* each corner's colour, in the first mesh and the second (charred) */
    u16 blend;         /* its colour index's top bits: 0x8000 semi-transparent, bits 13 and 14 the blend mode */
} MadeFace;
typedef struct {
    s32 units, h, bpp8; /* its width in VRAM's 16-bit units, its height, and 8 bits a texel (else 4) */
    u8 *pixels;         /* units * 2 bytes a row, to free */
    s32 colours;        /* its CLUT's, 16 or 256 */
    u16 clut[256];
} MadeImage;
typedef struct {
    const char *name;
    s32 frames;
    s32 mesh; /* the one it moves: 0, or 1 the second */
    s32 step; /* at least this many frames from one keyframe to the next (0 or 1: any), the frames between blended */
} MadeAnim;
typedef struct {
    MadeFace *faces;
    s32 nfaces;
    s32 meshes; /* 1, or 2 with the second's colours */
    MadeImage *images;
    s32 nimages;
    const MadeAnim *anims;
    s32 nanims;
    s32 nvert;     /* a frame's vertices */
    s32 rest_anim; /* the animation whose first frame is the mesh's own pose: its box, and its height */
    /* Frame f of animation `anim`: nvert vertices, in Crash Bash's units and axes (y down, its feet at 0). */
    void (*frame)(void *ctx, s32 anim, s32 f, V3 *out);
    void *ctx;
} MadeModel;
/* The model and its texture set, to free; 0 if it couldn't. An animation whose frames' vertices are more than an index can reach is
 * written as keyframes with the frames between blended. */
s32 made_write(const MadeModel *m, u8 **tex, s32 *tex_size, u8 **mdl, s32 *mdl_size);
void made_images_free(MadeImage *images, s32 n);

/* A Crash Bandicoot model's triangles as made faces, lit as they'd stand by normals (frame `normals` of its standing animation), and
 * their textures as made images; 0 if it couldn't. */
s32 c1_model_faces(const C1Level *l, const C1Entry *model, const V3 *normals, s32 nvert, MadeFace *faces, s32 max, s32 *nfaces,
                   MadeImage *images, s32 *nimages);

/* --- part of one of the game's own models, as the player's archive has it ---
 * The strips of a model bank's first mesh that `keep` picks, from where each one's corners are as the model rests (Crash Bash's axes,
 * y down) and whether it's textured: their faces as made faces, each on its strip's own corners and drawn the same way round, their
 * images, and where its corners are in any frame of the model's animations (hm_model_at, then corner_of). */
#define PART_MAX_IMAGES 8
typedef struct {
    u8 *mdl;          /* the model's file */
    s32 corners;      /* its first mesh's corners, */
    V3 *rest;         /* where each is as it rests, */
    u8 *in_part;      /* and which are the part's */
    MadeFace *faces;  /* the part's faces, on its own vertices */
    s32 nfaces;
    MadeImage images[PART_MAX_IMAGES];
    s16 image_of[PART_MAX_IMAGES]; /* each one's in the model's texture set (the set's last for the one any model's faces share) */
    s32 nimages;
    s16 *corner_of;   /* each of its vertices' corner */
    s32 nvert;
} GamePart;
s32 hm_game_part(s32 bank, s32 (*keep)(void *ctx, const V3 *rest, s32 corners, s32 textured), void *ctx, GamePart *out);
void hm_part_free(GamePart *p);
/* The model's animation by name, and its frames; -1 if it has none. */
s32 hm_model_anim(const GamePart *p, const char *name, s32 *frames);
/* The frames of a model bank's animation by name, as the archive has it: 0 if it has none (a made animation as long as its base's
 * plays a frame a tick). */
s32 hm_bank_frames(s32 bank, const char *name);
/* All its first mesh's corners at t of animation `anim`, between the frames either side (t is kept within its frames). 0 if it can't. */
s32 hm_model_at(const GamePart *p, s32 anim, float t, V3 *corners);
/* Faces and their images painted another colour, `paint` (red | green << 8 | blue << 16): their warm colours (reds, oranges and
 * yellows) turned to its hue, as strong as it is of theirs, and as bright of their own shade, a little of how far apart they were kept;
 * greys and the cold colours as they are. For a part of the game's own models made a hero's own colour (the cart Roo has is Rilla
 * Roo's orange one, painted his). */
void hm_paint(MadeFace *faces, s32 nfaces, MadeImage *images, s32 nimages, u32 paint);

/* --- what's loose on a hero (a tongue, ears) ---
 * Its swing: a damped spring at the hero's head, thrown about by the head's accelerations from frame to frame. The frames a swing
 * takes, its damping (a share of what would stop it swinging at all), how much of the head's throw it takes, and the most it swings
 * (Crash Bash's units). */
typedef struct {
    float period, damping, gain, max;
} Flop;
/* The swing at each of an animation's n frames, from its head's path through them: from rest as it starts, or, in one that loops,
 * settled into the loop's own (a few times round it first). */
void hm_flop(const V3 *path, s32 n, s32 loops, const Flop *spring, V3 *out);

/* --- riding a polar bear (Polar Push) ---
 * The games' riders sit on their bears in one model; an extra hero's rider model is Crash's bear, from the player's archive as the game
 * has it, with the hero sat on its back: it moves as the bear's back does, bent as Crash leans on his, hips to head (a share of it,
 * smoothed over a few frames, so a tall hero's head doesn't swing with every bob of his), and over that any motion of its own. The
 * animations are Polar Push's, as long as another hero's are (match_bank, the hero's base's: its moves then go a frame a tick), Crash's
 * resampled to them. */
enum { RIDE_FALL, RIDE_RUN, RIDE_BREATHE, RIDE_PUSH, RIDE_TAUNT, RIDE_LIGHT, RIDE_WIN, RIDE_ANIMS };
typedef struct {
    /* the hero sat on the bear's back, as for its first breathing frame, in Crash Bash's units and axes: its vertices, and each one's
     * share of a bend at its waist and at its neck, at those points */
    const V3 *seat;
    s32 nvert;
    const float *waist_share, *neck_share;
    V3 waist, neck;
    float lean_waist, lean_neck; /* of Crash's lean, the shares bent at each */
    float steady;                /* and of his tilt from upright, the share the hero keeps back from: a tall one's head swings further */
    s32 match_bank;              /* the model whose animations' lengths its own have (HeroModelBank), or -1 for Crash's */
    /* its faces (their vertices its own) and images; a face's second colours are made from its first, `charred` of them */
    const MadeFace *faces;
    s32 nfaces;
    const MadeImage *images;
    s32 nimages;
    float charred;
    /* any motion of its own, before the riding's, for frame f of RIDE_ animation `anim` (of `frames`); may be NULL */
    void (*own)(void *ctx, s32 anim, s32 f, s32 frames, V3 *verts);
    void *ctx;
    /* what's loose on it (a tongue, ears): each vertex's share of their swing, 1 at their ends, or NULL for none; and the swing, at
     * its head (the middle of what bends at its neck) */
    const float *flop;
    Flop swing;
} Rider;
/* Where the hero sits: the top of the bear's back under its middle, and that middle (Crash Bash's axes). 0 if there's no bear. */
s32 ride_seat(float *back_y, float *seat_z);
s32 ride_make(const Rider *r, u8 **tex, s32 *tex_size, u8 **mdl, s32 *mdl_size);

/* One of the game's own sounds, out of a sound pack in the archive (docs/ASSETS.md, Sound packs): the sample its tone for sound `id`
 * plays (copied, kept), at the pitch the game plays it (PlaySfx's note, 60, against the tone's centre and shift). 0 if there's none. */
s32 hm_game_sound(s32 entry, s32 id, HeroSound *out);
/* PS1 ADPCM played from memory, as loud as the game's own effects (sound.c, sfx_key_on); voice: the one that played it last stops
 * first (NULL: any number at once). */
void hm_play(const HeroSound *s, short *voice);

#endif
