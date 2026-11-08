/* Pinstripe Potoroo (bash/herodef.h), from Crash Bandicoot (USA, SCUS-94900): the second extra hero, made as Ripper Roo is
 * (heroes/roo.c, docs/EXTRA_HEROES.md).
 *
 * Level 8, his boss fight in his office, has his model (Pin1G) and 36 animations, all of him with his tommy gun: standing (PinSV, one
 * frame), firing in three stances (PiG1V kneeling, PiG2V crouched and laughing, PiG3V in a stride), sweeping his fire down and up
 * (PiE1-9V), stepping and diving aside (PiL1-6V, PiR1-6V), thrown back as he's hit (PiF1V, PiF4V), looking about (PiF2V, PiF3V) and
 * throwing the gun up (PiA1-3V). His upper body hardly bends in them: his arms stay on the gun and turn with his chest.
 *
 * At the user's word he has the gun only as he attacks and wins. The rest of the time it's away, its faces folded into his hands (its
 * front grip into his left, his palms closed over where the grips went), and his arms are his own: a rig of his (each arm two bones,
 * each leg two and its shoe) hangs them at his sides as he stands, pumps them as he jogs and puts them up for a crate. His jog, jump,
 * holding and throwing a crate, and his breathing (bobbing at the knees) are made by it, moving as much as Crash Bash's heroes' do.
 * His attack is his stride's fire (PiG3V), the gun coming out into his hands and away again, its muzzle flashing; he's thrown back as
 * he's hit (PiF1V) and knocked down (PiF4V), his arms flung out by his rig, laughs crouched as he taunts (PiG2V, his arms down), and as
 * he wins shoots his gun about laughing, as in that fight (PiG2V, the gun out), each of those at his still's size (PIN_SOURCE_SCALE)
 * with his still's eyes. He's lit brighter and softer than his game's colours are, and his head's bigger, as Crash Bash's heroes look
 * (PinLight, HEAD_SCALE). Riding a polar bear he's sat astride Crash's, his hands on its neck (ride_bear), and thrown about at the
 * ice's edge as Crash is (ride_own). His sounds are his boss
 * fight's: his laugh and his tommy gun. His hero card is the face that fight shows by his health. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "bash/main.h"
#include "bash/hero.h"
#include "bash/modelbank.h"
#include "bash/heromake.h"

/* --- how he plays ---
 * His base is Cortex, if nobody else is playing him: a villain as he is, and about his size. On foot he jogs where the others run,
 * slide or swim, holds a crate up as they do, fires his gun where they attack or kick, laughs where they taunt, shoots his gun about
 * laughing as he wins, is thrown back as he's hit and knocked down where they die.
 *
 * Whoever's place he has (Cortex's, or another's if someone has Cortex), its moves are its own scripts (extraheroes.c, SeqNames), so
 * some go by them: his attack's all of any hero's (Crash's and Coco's spin is a still mesh, the others' animations of their own), and
 * so are his hit, taunt, picking up a crate and throwing it; his win's all of either win (a round's, which some heroes play their taunt
 * for, and the match's), round and round; his jump's three parts are the three sequences a jump is (up, falling and landing, his JUMP
 * split as Cortex's is), so he lands as a hero does after a throw (Crash, Coco) and doesn't jump again. */
static const Stand PinStands[] = {
    { "MEDIUM", "WALK" }, { "RUN", "WALK" }, { "SKATE", "WALK" }, { "SLIDE", "WALK" }, { "SWIM", "WALK" }, { "FALL", "WALK" },
    { "PUSH", "WALK" }, { "TRANS", "WALK" }, { "BARGE", "WALK" },
    { "BANK", "HOLD" }, { "HOLD_SLOW", "HOLDWALK" }, { "HOLD_THROW", "THROW" },
    { "KICK", "ATTACK" }, { "FIRE1", "ATTACK" }, { "FIRE2", "ATTACK" }, { "FIRE3", "ATTACK" },
    { "WIN", "WIN", LOOP }, { "WIN2", "WIN", LOOP },
    { "TAUNT_A", "TAUNT" }, { "MINE", "TAUNT" }, { "TAZING", "TAUNT" }, { "LIGHT", "TAUNT" },
    { "RECOIL", "HIT" }, { "FLIP", "HIT", ONCE }, { "DAZED", "DIE", ONCE },
    { "@JUMP", "JUMP:0-7", ONCE }, { "@FALL", "JUMP:8-12", ONCE }, { "@LAND", "JUMP:13-19", ONCE },
    { "@ATTACK", "ATTACK" }, { "@HIT", "HIT" }, { "@TAUNT_A", "TAUNT" }, { "@WIN", "WIN", LOOP }, { "@VICTORY", "WIN", LOOP },
    { "@BA_PICKUP", "PICKUP" }, { "@BA_HOLD_THROW", "THROW" }, { "@PA_PICKUP", "PICKUP" }, { "@PA_HOLD_THROW", "THROW" },
    { NULL, NULL, STRETCH },
};

/* His sounds for his base's (docs/EXTRA_HEROES.md, A hero's sounds): the game has no name for him, so as he wins the announcer's name
 * is the start of his laugh, then "wins!", and he laughs as he cheers; he laughs as he taunts, yelps as he's hurt (his laugh's first
 * "ha", higher), fires his tommy gun as he attacks, thuds as he lands and throws with Crash's throw (his game has none of his own).
 * Jumping, picking up a crate, carrying it and pushing he's quiet. */
static const Voice PinVoices[] = {
    { 0, VOICE_NAME }, { 1, VOICE_LAUGH }, { 2, VOICE_YELP }, { 3, VOICE_YELP }, { 4, VOICE_YELP }, { 5, VOICE_YELP },
    { 6, VOICE_LAUGH },
    /* Crate Crush's */
    { 9, VOICE_NONE }, { 10, VOICE_NONE }, { 11, VOICE_NONE }, { 12, VOICE_THUD }, { 13, VOICE_FIRE }, { 14, VOICE_NONE },
    { 15, VOICE_THROW }, { 16, VOICE_NONE },
    /* Polar Push's */
    { 19, VOICE_NONE }, { 20, VOICE_NONE }, { 21, VOICE_NONE }, { 22, VOICE_NONE }, { 23, VOICE_NONE }, { 24, VOICE_LAUGH },
    { 25, VOICE_NONE },
    { -1, 0 },
};

static s32 pin_make(HeroMade *out);

/* His model's frames to each of his base's, blended between (as Roo's, roo.c): his moves are as long as Cortex's, and another hero's
 * longer ones would otherwise hold some of his frames for two of theirs, stepping (at 120 frames a second, slowly) */
#define PIN_SUBFRAMES 4

/* Riding a polar bear his model has all of Polar Push's moves by name; his push is one push, as long as the polar attack's sequence
 * (Cortex's plays his push's middle three times, his jetpack, which on Crash's bear was a push lunging three times; once at its own
 * speed it would end with him still sliding as the game pushes) */
static const Stand PinRides[] = {
    { "@POLAR_ATTACK", "PUSH", STRETCH },
    { NULL, NULL, STRETCH },
};
/* On his pogo stick his model has Pogo's moves by name, Crash's; his win (a flip) and his tazing at their own speed, as his base's are
 * 87 and 4 ticks long and Crash's 38 and 15 */
static const Stand PinPogo[] = {
    { "WIN", "WIN", LOOP },
    { "TAZING", "TAZING", LOOP },
    { NULL, NULL, STRETCH },
};

/* Each kind of game plays as on foot until he has a model of it; on his bear he's as heavy and as quick as Crash on his */
const HeroDef HeroPinstripe = {
    "PINSTRIPE", "pinstripe", "SCUS-94900", Cortex,
    { { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES }, { PinRides, "BREATHE", 0x1000, Crash, 1 },
      { PinPogo, "BREATHE", 0x1000, 0, 1 }, { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES },
      { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES }, { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES },
      { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES }, { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES },
      { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES }, { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES } },
    PinVoices, "BREATHE", 0, pin_make,
};

/* --- how he's made --- */

#define PIN_LEVEL "S0/S0000008.NSF"
#define PIN_HEIGHT 640.0f /* standing, in Crash Bash's units: a little taller than Roo (620), as the user found him too small at Cortex's
                           * height (540); Crate Crush's heroes stand 452 (Crash) to 748 (Kong), most about 650 */
#define PIN_UNIT 4096.0f  /* c1_frame_verts' units to his game's own */
#define PIN_SOURCE_SCALE 1.2f /* his boss fight's moves against its still (PinSV), which his rig is made from: every rigid piece of him
                               * (his shoes, his hands, the gun) is 1.2 times as big in each of them, and they're drawn at its size */
#define PIN_PI 3.14159265f
#define MAX_TRIS 1024
#define MAX_VERTS 640     /* his 441 and the gun's own copies */

/* His light, in his colours as he stands (c1_model_faces), for Crash Bash's look: its heroes' colours are bright and soft, their shading
 * mostly in their textures, a corner's colour no more than about a quarter either way from the rest of its colour's. Lit as Roo is (all
 * flat on the side away from the light: 42% of his faces were one colour) he was darker and duller than them, so his light wraps round,
 * brighter, and goes from 0.8 of his colours away from it to 1.3 toward it. */
static const C1Light PinLight = { { -0.35f, -0.75f, -0.55f }, 0.8f, 0.5f, 1 };

/* The animations his model has, each from one of his game's (src, its frames resampled to the made one's) or made by the rig from his
 * standing pose (src NULL); as long as his base's of the same name (hm_bank_frames of Cortex's, `base`) or `frames` ticks, each tick
 * PIN_SUBFRAMES of his frames (and no more than MADE_MAX_FRAMES) */
typedef struct {
    const char *name, *src, *base;
    s32 frames;
} Anim;
enum { A_BREATHE, A_WALK, A_JUMP, A_ATTACK, A_HIT, A_DIE, A_TAUNT, A_WIN, A_HOLD, A_HOLDWALK, A_PICKUP, A_THROW, PIN_ANIMS };
static const Anim PinAnims[PIN_ANIMS] = {
    { "BREATHE", NULL, "BREATHE", 32 }, { "WALK", NULL, "MEDIUM", 22 }, { "JUMP", NULL, "JUMP", 20 },
    { "ATTACK", "PiG3V", "ATTACK", 19 }, { "HIT", "PiF1V", "HIT", 26 }, { "DIE", "PiF4V", "DIE", 21 },
    { "TAUNT", "PiG2V", "TAUNT_A", 40 }, { "WIN", "PiG2V", NULL, 24 }, { "HOLD", NULL, "HOLD", 24 },
    { "HOLDWALK", NULL, "HOLD_SLOW", 24 }, { "PICKUP", NULL, "PICKUP", 9 }, { "THROW", NULL, "HOLD_THROW", 13 },
};

/* --- his rig ---
 * Worked in his game's axes and units, y up and him facing +z (c1_frame_verts' over PIN_UNIT, y and z the other way round), from his
 * standing pose. His model is seven pieces (triangles joined by their corners): his body (with his arms and head), the gun with his
 * right hand on it, his legs, his left hand (with the gun's front grip in it), his shoes and the gun's muzzle flash. The gun's faces
 * are those of the gun's piece and his left hand's that aren't a hand's colours (its skin and the green cuff): the gun and both its
 * grips. They're drawn on copies of their corners of their own (after his), so the gun can be folded away into his hands, each grip
 * into the hand holding it, and the hands left as they are. */
static V3 Rest[MAX_VERTS];     /* standing, in the rig */
static u8 PieceOf[MAX_VERTS];  /* PIECE_ */
enum { PIECE_BODY, PIECE_GUN, PIECE_LEGS, PIECE_LEFT_HAND, PIECE_SHOE, PIECE_FLASH, PIECE_NONE };
static u8 HandOf[MAX_VERTS];   /* 1 his right hand's, 2 his left's (its skin and cuff), else 0 */
static s32 Verts;              /* his own */
static s16 CopyOf[MAX_VERTS];  /* each of his vertices' copy (after his), or -1 */
static s16 CopyFrom[MAX_VERTS]; /* and each copy's vertex of his */
static u8 GunTri[MAX_TRIS];    /* the gun's triangles and the flash's, drawn on the copies */
static u8 CopyFlash[MAX_VERTS];
static u8 CopyHand[MAX_VERTS]; /* the hand each copy folds into: 1 the right, 2 the left */
static s32 Copies;

/* Each arm: its shoulder, elbow and wrist standing (his game's units), the vertices of his body within ARM_RADIUS of the line through
 * them (and its hand's, and what it holds: the gun and its flash the right, the front grip the left), each one's share of the forearm
 * (blended over ARM_ELBOW about the elbow along the arm) and how much it moves at all (less into the shoulder, ARM_SHOULDER along the
 * arm from it, and none above ARM_TOP). */
#define ARM_RADIUS 17.0f
#define ARM_ELBOW 8.0f
#define ARM_SHOULDER_IN -4.0f
#define ARM_SHOULDER 10.0f
#define ARM_TOP_FULL 220.0f
#define ARM_TOP 232.0f
#define ARM_VERTS 256
typedef struct {
    float side;
    V3 joint[3];
    float len[2];
    s16 verts[ARM_VERTS];
    float fore[ARM_VERTS], weight[ARM_VERTS];
    s32 n;
} Arm;
static Arm Arms[2] = {
    { -1, { { -42, 212, 0 }, { -42, 182, 18 }, { -27, 160, 66 } } },
    { 1, { { 44, 214, -12 }, { 53, 166, -18 }, { 40, 160, 10 } } },
};

/* Each leg: its hip, knee and ankle, its vertices (of his legs on its side, and its shoe), each one's share of the shin (blended over
 * LEG_KNEE about the knee) and whether it's the shoe's. */
#define LEG_KNEE 10.0f
typedef struct {
    float side;
    V3 joint[3];
    s16 verts[64];
    float shin[64];
    u8 shoe[64];
    s32 n;
} Leg;
static Leg Legs[2] = {
    { -1, { { -16, 122, -3 }, { -19, 73, 0 }, { -23, 25, -3 } } },
    { 1, { { 16, 122, -3 }, { 19, 73, 0 }, { 23, 25, -3 } } },
};

/* His chest, which his arms turn with in his game's animations: his body's vertices within CHEST_HALF of his middle across, between
 * CHEST_LOW and CHEST_HIGH; and his upper body, all above his hips, which bends and rises as he breathes, walks and jumps */
#define CHEST_HALF 25.0f
#define CHEST_LOW 150.0f
#define CHEST_HIGH 222.0f
#define WAIST_Y 125.0f
#define WAIST_BLEND 20.0f
static s16 Chest[128];
static s32 ChestN;

/* His eyes: three layers his game drew in that order, each over the last whatever their depths: his eyeballs (yellow), the slits of
 * his pupils on them (5, 5, 11) and his lids over both (their colour flagged 0x10). Each of its frames' vertices is quantised by itself,
 * so in every one some corner of a layer pokes through the one over it, and a depth test showed those as slivers round his eyes,
 * changing as he moved. His still's lids are lifted forward EYE_LIFT_LID of his game's units (his height is about 290; drawn back to
 * front as his game drew them, his head looks the same), and each frame from his game has his eyes from his still, moved as its own
 * eyes move (src_at). His pupils were slits of a few polygons drawn on his eyeballs' own corners, tiny and broken up beside Crash
 * Bash's heroes' eyes, which are their textures' (the user's word: "all the other heroes use a texture"): so his eyeballs are painted
 * (eye_paint), each eye an image of its own made here, of nobody's art, a pupil in the middle and its glint, laid flat over the eye
 * as he faces, and the pupils' polygons are left out. */
#define EYE_LIFT_LID 2.0f
#define EYE_MAX 96
#define EYE_TEX_W 32 /* the eye's image, texels (4 bits each) */
#define EYE_TEX_H 16
static u8 EyeLayer[MAX_VERTS]; /* each vertex's layers, of the triangles it's a corner of: 1 eyeball, 2 pupil, 4 lid */
static s16 EyeIds[EYE_MAX];
static s32 EyeN;
static u8 EyeTri[MAX_TRIS];    /* each triangle's layer, the same */

/* His head, HEAD_SCALE as big as his game's, as Crash Bash's heroes' heads are big for their bodies (the user's pick, 2026-11-06): all
 * of his body's piece above his neck (blended over NECK_BLEND about NECK_Y, where his body's narrowest under his head), grown about
 * the middle of his neck (its corners NECK_BLEND under NECK_Y) in each frame, so it grows where his neck is however he's moving. */
#define HEAD_SCALE 1.2f
#define NECK_Y 230.0f
#define NECK_BLEND 6.0f
#define NECK_MAX 32
static float HeadGrow[MAX_VERTS]; /* each vertex's share of HEAD_SCALE (0 none, 1 all of it) */
static s16 Neck[NECK_MAX];
static s32 NeckN;

/* His palms: his hands close round the gun's grips, so with the gun away each palm is a hole where its grip went in (the game draws
 * only a face's front, and you'd see through the hand). Each is closed with a cap, the loop of edges where its hand's faces meet the
 * grip's fanned to the loop's middle, its colours from the faces beside it, on copies of its corners of their own: they fold away into
 * that middle as the gun comes out, its grip filling the palm again. */
#define MAX_CAPS 2
#define CAP_MAX 32
typedef struct {
    s16 loop[CAP_MAX]; /* its corners round the hole, in order */
    s32 n, base;       /* their count, and its first vertex: their copies, then its middle */
} Cap;
static Cap Caps[MAX_CAPS];
static s32 NCaps, CapVerts;

static V3 v3(float x, float y, float z) {
    V3 v;

    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}
static V3 vadd(V3 a, V3 b) {
    return v3(a.x + b.x, a.y + b.y, a.z + b.z);
}
static V3 vsub(V3 a, V3 b) {
    return v3(a.x - b.x, a.y - b.y, a.z - b.z);
}
static V3 vmul(V3 a, float s) {
    return v3(a.x * s, a.y * s, a.z * s);
}
static float vdot(V3 a, V3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
static V3 vcross(V3 a, V3 b) {
    return v3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
static float vlen(V3 a) {
    return sqrtf(vdot(a, a));
}
static V3 vnorm(V3 a) {
    float l = vlen(a);

    return l > 0 ? vmul(a, 1 / l) : a;
}

/* A turn, as its axis (a unit) and angle */
typedef struct {
    V3 axis;
    float angle;
} Turn;
static Turn turn_from(V3 a, V3 b) {
    Turn t;
    V3 c = vcross(a, b);
    float s = vlen(c);

    t.axis = s > 1e-6f ? vmul(c, 1 / s) : v3(1, 0, 0);
    t.angle = s > 1e-6f ? atan2f(s, vdot(a, b)) : 0;
    return t;
}
static V3 turned(const Turn *t, V3 v) {
    float c = cosf(t->angle), s = sinf(t->angle);

    return vadd(vadd(vmul(v, c), vmul(vcross(t->axis, v), s)), vmul(t->axis, vdot(t->axis, v) * (1 - c)));
}

static V3 to_rig(V3 p) {
    return v3(p.x / PIN_UNIT, -p.y / PIN_UNIT, -p.z / PIN_UNIT);
}
static V3 from_rig(V3 r) {
    return v3(r.x * PIN_UNIT, -r.y * PIN_UNIT, -r.z * PIN_UNIT);
}

static float seg_closest(V3 p, V3 a, V3 b, float *u) {
    V3 ab = vsub(b, a);
    float t = vdot(vsub(p, a), ab) / vdot(ab, ab);

    t = t < 0 ? 0 : t > 1 ? 1 : t;
    *u = t;
    return vlen(vsub(p, vadd(a, vmul(ab, t))));
}

/* His pieces, the gun's faces and the rig's shares, from his model's triangles and his standing pose */
static s32 rig_setup(const C1Entry *model, s32 nvert) {
    static s32 up[MAX_VERTS], size[MAX_VERTS], piece_root[8];
    const u8 *info = model->item[0], *polys = model->item[1];
    s32 ntri = (s32)hm_rd32(info), nslot = (s32)hm_rd32(info + 16), i, j, k, a;

    for (k = 0; k < nvert; k++) {
        up[k] = k;
        size[k] = 0;
        PieceOf[k] = PIECE_NONE;
        HandOf[k] = 0;
    }
#define ROOT(x) \
    do { \
        while (up[x] != x) { \
            up[x] = up[up[x]]; \
            x = up[x]; \
        } \
    } while (0)
    for (i = 0; i < ntri && (i + 1) * 8 <= model->size[1]; i++) {
        s32 v[3];

        for (j = 0; j < 3; j++) {
            v[j] = hm_rd16(polys + i * 8 + j * 2) / 6;
            if (v[j] >= nvert) {
                return 0;
            }
        }
        for (j = 1; j < 3; j++) {
            s32 x = v[0], y = v[j];

            ROOT(x);
            ROOT(y);
            up[x] = y;
        }
    }
    for (k = 0; k < nvert; k++) {
        a = k;
        ROOT(a);
        size[a]++;
    }
    /* the pieces by size: body 241 vertices, gun and hand 66, legs 50, left hand 29, shoes 22 and 22, flash 10 */
    for (i = 0; i < 7; i++) {
        s32 best = -1;

        for (k = 0; k < nvert; k++) {
            if (up[k] == k && size[k] > 0 && (best < 0 || size[k] > size[best])) {
                for (j = 0; j < i && piece_root[j] != k; j++) {
                }
                if (j == i) {
                    best = k;
                }
            }
        }
        if (best < 0) {
            return 0;
        }
        piece_root[i] = best;
    }
    if (size[piece_root[0]] < 200 || size[piece_root[1]] < 40 || size[piece_root[6]] < 4) {
        printf("bonus: Pinstripe's model isn't as his game has it (pieces of %d, %d, %d)\n", size[piece_root[0]], size[piece_root[1]],
               size[piece_root[6]]);
        return 0;
    }
    for (k = 0; k < nvert; k++) {
        static const u8 Kinds[7] = { PIECE_BODY, PIECE_GUN, PIECE_LEGS, PIECE_LEFT_HAND, PIECE_SHOE, PIECE_SHOE, PIECE_FLASH };

        a = k;
        ROOT(a);
        for (i = 0; i < 7 && piece_root[i] != a; i++) {
        }
        PieceOf[k] = i < 7 ? Kinds[i] : PIECE_NONE;
    }
#undef ROOT
    /* the gun's faces, its grips' and the flash's: copies of their corners of their own; the hands', their skin and their cuffs, as they
     * are */
    Copies = 0;
    for (k = 0; k < nvert; k++) {
        CopyOf[k] = -1;
    }
    for (i = 0; i < ntri && i < MAX_TRIS; i++) {
        const u8 *p = polys + i * 8, *s;
        s32 slot = hm_rd16(p + 6) & 0x7FFF, v0 = hm_rd16(p) / 6, piece = PieceOf[v0], hand, held;

        GunTri[i] = 0;
        if (slot >= nslot || (piece != PIECE_GUN && piece != PIECE_LEFT_HAND && piece != PIECE_FLASH)) {
            continue;
        }
        s = info + 20 + slot * 4;
        held = piece == PIECE_LEFT_HAND ? 2 : 1;
        hand = piece != PIECE_FLASH && ((s[0] == 89 && s[1] == 35 && s[2] == 0) || (s[0] == 0 && s[1] == 107 && s[2] == 0));
        GunTri[i] = (u8)!hand;
        for (j = 0; j < 3; j++) {
            s32 v = hm_rd16(p + j * 2) / 6;

            if (hand) {
                HandOf[v] = (u8)held;
            } else if (CopyOf[v] < 0 && nvert + Copies < MAX_VERTS) {
                CopyOf[v] = (s16)(nvert + Copies);
                CopyFrom[Copies] = (s16)v;
                CopyFlash[Copies] = piece == PIECE_FLASH;
                CopyHand[Copies] = (u8)held;
                Copies++;
            }
        }
    }
    /* the arms */
    for (a = 0; a < 2; a++) {
        Arm *arm = &Arms[a];
        float l0, l1;

        arm->len[0] = l0 = vlen(vsub(arm->joint[1], arm->joint[0]));
        arm->len[1] = l1 = vlen(vsub(arm->joint[2], arm->joint[1]));
        arm->n = 0;
        for (k = 0; k < nvert && arm->n < ARM_VERTS; k++) {
            V3 p = Rest[k];
            float u0, u1, d0, d1, s;

            if (PieceOf[k] == PIECE_BODY) {
                d0 = seg_closest(p, arm->joint[0], arm->joint[1], &u0);
                d1 = seg_closest(p, arm->joint[1], arm->joint[2], &u1);
                if ((d0 < d1 ? d0 : d1) > ARM_RADIUS || p.x * arm->side < 12) {
                    continue;
                }
                s = d0 <= d1 ? u0 * l0 : l0 + u1 * l1;
                arm->verts[arm->n] = (s16)k;
                arm->fore[arm->n] = hm_smooth(l0 - ARM_ELBOW, l0 + ARM_ELBOW, s);
                arm->weight[arm->n] = hm_smooth(ARM_SHOULDER_IN, ARM_SHOULDER, s) * hm_smooth(ARM_TOP, ARM_TOP_FULL, p.y);
                arm->n++;
            } else if ((a == 0 && PieceOf[k] == PIECE_LEFT_HAND) || (a == 1 && (PieceOf[k] == PIECE_GUN || PieceOf[k] == PIECE_FLASH))) {
                arm->verts[arm->n] = (s16)k;
                arm->fore[arm->n] = 1;
                arm->weight[arm->n] = 1;
                arm->n++;
            }
        }
    }
    /* the legs, by side */
    for (a = 0; a < 2; a++) {
        Leg *leg = &Legs[a];

        leg->n = 0;
        for (k = 0; k < nvert && leg->n < 64; k++) {
            if ((PieceOf[k] != PIECE_LEGS && PieceOf[k] != PIECE_SHOE) || (Rest[k].x < 0 ? -1.0f : 1.0f) != leg->side) {
                continue;
            }
            leg->verts[leg->n] = (s16)k;
            leg->shoe[leg->n] = PieceOf[k] == PIECE_SHOE;
            leg->shin[leg->n] = hm_smooth(leg->joint[1].y + LEG_KNEE, leg->joint[1].y - LEG_KNEE, Rest[k].y);
            leg->n++;
        }
    }
    /* his chest */
    ChestN = 0;
    for (k = 0; k < nvert && ChestN < 128; k++) {
        if (PieceOf[k] == PIECE_BODY && fabsf(Rest[k].x) < CHEST_HALF && Rest[k].y > CHEST_LOW && Rest[k].y < CHEST_HIGH) {
            Chest[ChestN++] = (s16)k;
        }
    }
    /* his eyes' layers, by their colours */
    for (k = 0; k < nvert; k++) {
        EyeLayer[k] = 0;
    }
    for (i = 0; i < ntri && i < MAX_TRIS; i++) {
        const u8 *p = polys + i * 8, *s;
        s32 slot = hm_rd16(p + 6) & 0x7FFF, layer = 0;

        EyeTri[i] = 0;
        if (slot >= nslot || PieceOf[hm_rd16(p) / 6] != PIECE_BODY) {
            continue;
        }
        s = info + 20 + slot * 4;
        layer = s[0] == 255 && s[1] == 255 && s[2] == 0 ? 1 : s[0] == 5 && s[1] == 5 && s[2] == 11 ? 2 : (s[3] & 0x10) ? 4 : 0;
        for (j = 0; j < 3; j++) {
            EyeLayer[hm_rd16(p + j * 2) / 6] |= (u8)layer;
        }
        EyeTri[i] = (u8)layer;
    }
    EyeN = 0;
    for (k = 0; k < nvert && EyeN < EYE_MAX; k++) {
        if (EyeLayer[k]) {
            EyeIds[EyeN++] = (s16)k;
        }
    }
    /* his head, and his neck */
    NeckN = 0;
    for (k = 0; k < nvert; k++) {
        HeadGrow[k] = PieceOf[k] == PIECE_BODY ? hm_smooth(NECK_Y - NECK_BLEND, NECK_Y + NECK_BLEND, Rest[k].y) : 0;
        if (PieceOf[k] == PIECE_BODY && Rest[k].y >= NECK_Y - NECK_BLEND && Rest[k].y < NECK_Y && NeckN < NECK_MAX) {
            Neck[NeckN++] = (s16)k;
        }
    }
    return Arms[0].n > 20 && Arms[1].n > 20 && Legs[0].n > 20 && Legs[1].n > 20 && ChestN >= 4 && NeckN >= 3;
}

/* His head grown about the middle of his neck as frame r has it (above, HEAD_SCALE) */
static void grow_head(V3 *r) {
    V3 neck = { 0, 0, 0 };
    s32 k;

    for (k = 0; k < NeckN; k++) {
        neck = vadd(neck, r[Neck[k]]);
    }
    neck = vmul(neck, 1.0f / NeckN);
    for (k = 0; k < Verts; k++) {
        if (HeadGrow[k] > 0) {
            r[k] = vadd(neck, vmul(vsub(r[k], neck), 1 + (HEAD_SCALE - 1) * HeadGrow[k]));
        }
    }
}

/* His still's lids lifted forward (above, EyeLayer) */
static void eye_lift(void) {
    s32 k;

    for (k = 0; k < EyeN; k++) {
        if (EyeLayer[EyeIds[k]] & 4) {
            Rest[EyeIds[k]].z += EYE_LIFT_LID;
        }
    }
}

/* His eye's image (above): yellow, deeper toward its edge and under his lid, a black pupil in its middle a little taller than wide
 * (an eye's shape on him is about 2.3 times as wide as it's tall), blended into the yellow at its edge, and a white glint at its top
 * left; four bits a texel. 0 if there's no memory for it. */
#define RGB15(r, g, b) ((u16)(((r) >> 3) | ((g) >> 3) << 5 | ((b) >> 3) << 10))
enum { EYE_YELLOW = 1, EYE_DEEP, EYE_SHADE, EYE_RIM1, EYE_RIM2, EYE_PUPIL, EYE_GLINT, EYE_GLINT_RIM };
#define EYE_PUPIL_W 3.4f /* half the pupil's width and height, texels */
#define EYE_PUPIL_H 5.2f
#define EYE_GLINT_R 1.1f
static s32 eye_image(MadeImage *im) {
    static const u16 Clut[16] = {
        0x8000, RGB15(255, 228, 40), RGB15(240, 190, 20), RGB15(200, 140, 10), RGB15(170, 152, 27), RGB15(85, 76, 13),
        RGB15(12, 10, 8), RGB15(255, 255, 240), RGB15(130, 130, 125),
    };
    float cx = EYE_TEX_W / 2.0f, cy = EYE_TEX_H / 2.0f + 0.5f, gx = cx - 1.4f, gy = cy - 2.2f;
    s32 u, v, k;

    memset(im, 0, sizeof(*im));
    im->units = EYE_TEX_W / 4;
    im->h = EYE_TEX_H;
    im->bpp8 = 0;
    im->colours = 16;
    for (k = 0; k < 16; k++) {
        im->clut[k] = Clut[k] != 0 ? Clut[k] : Clut[EYE_YELLOW];
    }
    im->clut[0] = 0x8000;
    if ((im->pixels = calloc((size_t)im->units * 2 * EYE_TEX_H, 1)) == NULL) {
        return 0;
    }
    for (v = 0; v < EYE_TEX_H; v++) {
        for (u = 0; u < EYE_TEX_W; u++) {
            float pupil = 0, glint = 0, e = ((u + 0.5f - cx) / cx) * ((u + 0.5f - cx) / cx) + ((v + 0.5f - cy) / cy) * ((v + 0.5f - cy) / cy);
            s32 sx, sy, c;

            /* the pupil's and glint's share of it, by 4 x 4 points in it */
            for (sy = 0; sy < 4; sy++) {
                for (sx = 0; sx < 4; sx++) {
                    float x = u + (sx + 0.5f) / 4, y = v + (sy + 0.5f) / 4;
                    float px = (x - cx) / EYE_PUPIL_W, py = (y - cy) / EYE_PUPIL_H, qx = x - gx, qy = y - gy;

                    pupil += (px * px + py * py <= 1) / 16.0f;
                    glint += (qx * qx + qy * qy <= EYE_GLINT_R * EYE_GLINT_R) / 16.0f;
                }
            }
            c = v < 3 ? EYE_SHADE : e > 0.75f ? EYE_DEEP : EYE_YELLOW;
            c = glint > 0.6f ? EYE_GLINT : glint > 0.2f ? EYE_GLINT_RIM : pupil > 0.85f ? EYE_PUPIL : pupil > 0.5f ? EYE_RIM2
                : pupil > 0.15f ? EYE_RIM1 : c;
            im->pixels[v * im->units * 2 + u / 2] |= (u8)(c << (u & 1) * 4);
        }
    }
    return 1;
}

/* His eyeballs painted with it (above), each face's corners placed on it by where they are from the middle of their eye's extent in
 * his still as he faces, both eyes at one size (his are 14 and 8.5 of his game's units across, and each would have its own pupil's
 * shape), untinted; and his pupils' faces left out (all their corners at one). Its image is the last of his. */
static void eye_paint(MadeFace *faces, s32 ntri, MadeImage *images, s32 *nimages) {
    V3 lo[2], hi[2], mid[2];
    float half_w = 0, half_h = 0;
    s32 i, j, k, side;

    if (*nimages >= MADE_MAX_IMAGES || !eye_image(&images[*nimages])) {
        return;
    }
    for (side = 0; side < 2; side++) {
        lo[side] = v3(1e9f, 1e9f, 1e9f);
        hi[side] = v3(-1e9f, -1e9f, -1e9f);
    }
    for (k = 0; k < EyeN; k++) {
        V3 p = Rest[EyeIds[k]];

        side = p.x > 0;
        if (EyeLayer[EyeIds[k]] & 1) {
            lo[side] = v3(p.x < lo[side].x ? p.x : lo[side].x, p.y < lo[side].y ? p.y : lo[side].y, 0);
            hi[side] = v3(p.x > hi[side].x ? p.x : hi[side].x, p.y > hi[side].y ? p.y : hi[side].y, 0);
        }
    }
    for (side = 0; side < 2; side++) {
        half_w = (hi[side].x - lo[side].x) / 2 > half_w ? (hi[side].x - lo[side].x) / 2 : half_w;
        half_h = (hi[side].y - lo[side].y) / 2 > half_h ? (hi[side].y - lo[side].y) / 2 : half_h;
    }
    /* each eye's middle where its yellow is, as he faces: its faces' middles by their areas (its corners' extent leans to its long
     * pointed corner, and its pupil there looked aside) */
    for (side = 0; side < 2; side++) {
        float area = 0;

        mid[side] = vmul(vadd(lo[side], hi[side]), 0.5f);
        for (i = 0; i < ntri && i < MAX_TRIS; i++) {
            V3 a = Rest[faces[i].v[0]], b = Rest[faces[i].v[1]], c = Rest[faces[i].v[2]];
            float s = fabsf((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) / 2;

            if (EyeTri[i] != 1 || (a.x + b.x + c.x > 0) != side) {
                continue;
            }
            if (area == 0) {
                mid[side] = v3(0, 0, 0);
            }
            mid[side] = vadd(mid[side], vmul(v3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, 0), s));
            area += s;
        }
        if (area > 0) {
            mid[side] = vmul(mid[side], 1 / area);
        }
    }
    for (i = 0; i < ntri && i < MAX_TRIS; i++) {
        if (EyeTri[i] == 2) {
            faces[i].v[1] = faces[i].v[2] = faces[i].v[0];
            continue;
        }
        if (EyeTri[i] != 1) {
            continue;
        }
        side = Rest[faces[i].v[0]].x + Rest[faces[i].v[1]].x + Rest[faces[i].v[2]].x > 0;
        if (half_w <= 0 || half_h <= 0) {
            continue;
        }
        faces[i].image = *nimages;
        for (j = 0; j < 3; j++) {
            V3 p = Rest[faces[i].v[j]];
            float u = 0.5f + (p.x - mid[side].x) / (2 * half_w), v = 0.5f - (p.y - mid[side].y) / (2 * half_h);

            u = u < 0 ? 0 : u > 1 ? 1 : u;
            v = v < 0 ? 0 : v > 1 ? 1 : v;

            faces[i].uv[j][0] = (u8)(u * (EYE_TEX_W - 1) + 0.5f);
            faces[i].uv[j][1] = (u8)(v * (EYE_TEX_H - 1) + 0.5f);
            for (k = 0; k < 3; k++) {
                faces[i].rgb[0][j][k] = faces[i].rgb[1][j][k] = 128;
            }
        }
    }
    (*nimages)++;
}

/* An arm put along `up` (its upper arm) and `fore` (its forearm), directions in his rig, its vertices from where they stand moved to
 * there, and then as `chest` moves them (his chest's move from standing to the frame), if not NULL */
static void pose_arm(V3 *r, const Arm *arm, V3 up, V3 fore, const Moved *chest) {
    V3 q0 = arm->joint[0], q1 = vadd(q0, vmul(vnorm(up), arm->len[0]));
    Turn t0 = turn_from(vnorm(vsub(arm->joint[1], arm->joint[0])), vnorm(up));
    Turn t1 = turn_from(turned(&t0, vnorm(vsub(arm->joint[2], arm->joint[1]))), vnorm(fore));
    s32 i;

    for (i = 0; i < arm->n; i++) {
        s32 k = arm->verts[i];
        V3 p = Rest[k], a = vadd(q0, turned(&t0, vsub(p, arm->joint[0])));
        V3 b = vadd(q1, turned(&t1, turned(&t0, vsub(p, arm->joint[1]))));
        V3 q = hm_mix(a, b, arm->fore[i]);

        q = hm_mix(p, q, arm->weight[i]);
        r[k] = chest != NULL ? hm_moved(chest, q) : q;
    }
}

/* The arms' poses: each arm's upper arm and forearm directions (x by its side), for both */
typedef struct {
    V3 up, fore;
} ArmPose;
static const ArmPose ArmsDown = { { 0.12f, -1, -0.05f }, { 0.04f, -1, 0.18f } };
static const ArmPose ArmsUp = { { 0.45f, 0.88f, 0.1f }, { -0.3f, 0.95f, 0.05f } };
static const ArmPose ArmsForward = { { 0.15f, 0.2f, 1 }, { 0.05f, 0.1f, 1 } };

static V3 sided(V3 v, float side) {
    return v3(v.x * side, v.y, v.z);
}

/* both arms between two poses, swung forward (+) or back by `swing` radians about their shoulders (the second arm the other way, as he
 * walks) and by `pitch` both together */
static void pose_arms(V3 *r, const ArmPose *a, const ArmPose *b, float t, float swing, float pitch, const Moved *chest) {
    s32 i;

    for (i = 0; i < 2; i++) {
        float s = Arms[i].side, w = (i == 0 ? swing : -swing) + pitch;
        V3 up = hm_mix(sided(a->up, s), sided(b->up, s), t), fore = hm_mix(sided(a->fore, s), sided(b->fore, s), t);
        V3 o = { 0, 0, 0 };

        up = hm_turn_x(up, o, -w);
        fore = hm_turn_x(fore, o, -w * 1.3f);
        pose_arm(r, &Arms[i], up, fore, chest);
    }
}

/* A leg: its thigh turned forward by `hip` radians at the hip, its shin back from that by `knee` at the knee, and its shoe by `foot`
 * more at the ankle; each from where it stands, about its own joints */
static void pose_leg(V3 *r, const Leg *leg, float hip, float knee, float foot) {
    V3 knee_at = hm_turn_x(leg->joint[1], leg->joint[0], -hip);
    V3 ankle_at = vadd(knee_at, hm_turn_x(vsub(leg->joint[2], leg->joint[1]), v3(0, 0, 0), -(hip - knee)));
    s32 i;

    for (i = 0; i < leg->n; i++) {
        s32 k = leg->verts[i];
        V3 p = Rest[k], thigh = hm_turn_x(p, leg->joint[0], -hip);
        V3 shin = vadd(knee_at, hm_turn_x(vsub(p, leg->joint[1]), v3(0, 0, 0), -(hip - knee)));

        if (leg->shoe[i]) {
            r[k] = vadd(ankle_at, hm_turn_x(vsub(p, leg->joint[2]), v3(0, 0, 0), -(hip - knee + foot)));
        } else {
            r[k] = hm_mix(thigh, shin, leg->shin[i]);
        }
    }
}

/* His upper body (all above his waist, blended over WAIST_BLEND): leaned forward by `lean` radians about his waist (back if it's below
 * 0) and lifted `lift`; and his chest risen `breath` more (as he breathes), with his arms and hands. (It leaned back for a lean above
 * 0 till 2026-11-08, so his pickup leaned back as he knelt and his jog a little back; the user had them put right.) */
static void pose_body(V3 *r, float lean, float lift, float breath) {
    V3 waist = { 0, WAIST_Y, 0 };
    s32 k;

    for (k = 0; k < Verts; k++) {
        float w;

        if (PieceOf[k] == PIECE_LEGS || PieceOf[k] == PIECE_SHOE) {
            continue;
        }
        w = hm_smooth(WAIST_Y - WAIST_BLEND, WAIST_Y + WAIST_BLEND, Rest[k].y);
        r[k] = hm_turn_x(r[k], waist, lean * w);
        r[k].y += lift * w + breath * hm_smooth(WAIST_Y + 25, 200, Rest[k].y);
    }
}

/* His upper body (all above his waist, blended over WAIST_BLEND) turned `turn` radians about the upright through his waist, his
 * shoulders swinging round as he walks */
static void pose_turn(V3 *r, float turn) {
    V3 waist = { 0, WAIST_Y, 0 };
    s32 k;

    for (k = 0; k < Verts; k++) {
        if (PieceOf[k] != PIECE_LEGS && PieceOf[k] != PIECE_SHOE) {
            r[k] = hm_turn_y(r[k], waist, turn * hm_smooth(WAIST_Y - WAIST_BLEND, WAIST_Y + WAIST_BLEND, Rest[k].y));
        }
    }
}

/* All of him moved by d */
static void pose_shift(V3 *r, V3 d) {
    s32 k;

    for (k = 0; k < Verts; k++) {
        r[k] = vadd(r[k], d);
    }
}

/* Both knees bent `bend` radians, his thighs forward half that and his shoes flat, and all of him down so his feet stay on the floor */
static void pose_knees(V3 *r, float bend) {
    float thigh = vlen(vsub(Legs[0].joint[1], Legs[0].joint[0])), shin = vlen(vsub(Legs[0].joint[2], Legs[0].joint[1]));
    s32 i;

    for (i = 0; i < 2; i++) {
        pose_leg(r, &Legs[i], bend / 2, bend, bend / 2);
    }
    pose_shift(r, v3(0, -(thigh + shin) * (1 - cosf(bend / 2)), 0));
}

/* --- his moves --- */

static const C1Entry *Src[PIN_ANIMS];
static s32 Scale[3];
static s32 Frames[PIN_ANIMS];

/* A frame of one of his game's animations, at t of its frames (blended between two), in the rig and at its size (PIN_SOURCE_SCALE,
 * about his feet: the rig's origin is on the floor between them); its count, 0 if it can't */
static s32 src_at(const C1Entry *a, float t, V3 *r) {
    static V3 fa[MAX_VERTS], fb[MAX_VERTS];
    s32 f0, f1, n, m, k;
    float u;

    t = t < 0 ? 0 : t > a->count - 1 ? (float)(a->count - 1) : t;
    f0 = (s32)floorf(t);
    f1 = f0 + 1 < a->count ? f0 + 1 : f0;
    u = t - f0;
    n = c1_frame_verts(a->item[f0], a->size[f0], Scale, fa, NULL, MAX_VERTS);
    m = c1_frame_verts(a->item[f1], a->size[f1], Scale, fb, NULL, MAX_VERTS);
    if (n != Verts || m != Verts) {
        return 0;
    }
    for (k = 0; k < n; k++) {
        r[k] = vmul(to_rig(hm_mix(fa[k], fb[k], u)), 1 / PIN_SOURCE_SCALE);
    }
    /* his eyes his still's, in order, moved as these are */
    if (EyeN >= 3) {
        Moved mv;

        hm_fit(Rest, r, EyeIds, EyeN, &mv);
        for (k = 0; k < EyeN; k++) {
            r[EyeIds[k]] = hm_moved(&mv, Rest[EyeIds[k]]);
        }
    }
    return n;
}

/* His chest's move from standing to a frame */
static void chest_move(const V3 *r, Moved *mv) {
    hm_fit(Rest, r, Chest, ChestN, mv);
}

/* The gun's, its grips' and the flash's copies: out of his hands (`gun` 1, as big as it is, 0 folded away, each part into the middle
 * of the hand holding it) and the flash showing or not (only with the gun all out), as the frame `r` has them */
static void place_gun(V3 *r, float gun, s32 flash) {
    V3 grip[3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
    s32 k, n[3] = { 0, 0, 0 };

    for (k = 0; k < Verts; k++) {
        grip[HandOf[k]] = vadd(grip[HandOf[k]], r[k]);
        n[HandOf[k]]++;
    }
    for (k = 1; k < 3; k++) {
        grip[k] = n[k] > 0 ? vmul(grip[k], 1.0f / n[k]) : grip[k];
    }
    for (k = 0; k < Copies; k++) {
        float s = CopyFlash[k] ? (flash && gun >= 1 ? 1.0f : 0.0f) : gun;
        V3 g = grip[CopyHand[k]];

        r[Verts + k] = vadd(g, vmul(vsub(r[CopyFrom[k]], g), s));
    }
    /* the palms' caps, as the gun isn't there */
    for (k = 0; k < NCaps; k++) {
        const Cap *c = &Caps[k];
        V3 mid = { 0, 0, 0 };
        s32 j;

        for (j = 0; j < c->n; j++) {
            mid = vadd(mid, r[c->loop[j]]);
        }
        mid = vmul(mid, 1.0f / c->n);
        for (j = 0; j < c->n; j++) {
            r[c->base + j] = gun <= 0 ? r[c->loop[j]] : vadd(mid, vmul(vsub(r[c->loop[j]], mid), 1 - (gun < 1 ? gun : 1)));
        }
        r[c->base + c->n] = mid;
    }
}

/* The palms' caps (above, Caps) as faces after his: from each hand's own faces (its skin and cuff), the edges only one of them has,
 * chained into loops; the loops mostly on the grip's corners are its palm's (the others its wrist's, inside his sleeve). Each edge's
 * face is the one beside it, and the cap's face there goes the other way along it, so it's drawn from the same side. */
static void add_caps(MadeFace *faces, s32 *ntri) {
    static s16 next[MAX_VERTS], face_of[MAX_VERTS];
    static u8 seen[MAX_VERTS];
    s32 hand, i, j, k, a;

    NCaps = 0;
    CapVerts = 0;
    for (hand = 1; hand <= 2; hand++) {
        for (k = 0; k < Verts; k++) {
            next[k] = -1;
            seen[k] = 0;
        }
        for (i = 0; i < *ntri; i++) {
            if (GunTri[i] || faces[i].v[0] >= Verts || HandOf[faces[i].v[0]] != hand) {
                continue;
            }
            for (j = 0; j < 3; j++) {
                s32 x = faces[i].v[j], y = faces[i].v[(j + 1) % 3], shared = 0, o, e;

                /* by its corners either way round: the right hand has two faces twice over */
                for (o = 0; o < *ntri && !shared; o++) {
                    if (o == i || GunTri[o] || faces[o].v[0] >= Verts || HandOf[faces[o].v[0]] != hand) {
                        continue;
                    }
                    for (e = 0; e < 3; e++) {
                        s32 p = faces[o].v[e], q = faces[o].v[(e + 1) % 3];

                        shared |= (p == x && q == y) || (p == y && q == x);
                    }
                }
                if (!shared) {
                    next[x] = (s16)y;
                    face_of[x] = (s16)i;
                }
            }
        }
        for (a = 0; a < Verts && NCaps < MAX_CAPS; a++) {
            Cap *c = &Caps[NCaps];
            s32 v, on_grip = 0;

            if (next[a] < 0 || seen[a]) {
                continue;
            }
            c->n = 0;
            for (v = a; v >= 0 && !seen[v] && c->n < CAP_MAX; v = next[v]) {
                seen[v] = 1;
                c->loop[c->n++] = (s16)v;
                on_grip += CopyOf[v] >= 0;
            }
            if (c->n < 3 || v != a || on_grip * 2 < c->n || Verts + Copies + CapVerts + c->n + 1 > MAX_VERTS
                || *ntri + c->n > MAX_TRIS) {
                continue;
            }
            c->base = Verts + Copies + CapVerts;
            CapVerts += c->n + 1;
            {
                s32 sum[2][3] = { { 0, 0, 0 }, { 0, 0, 0 } }, first = *ntri, m, ch;

                for (j = 0; j < c->n; j++) {
                    const MadeFace *by = &faces[face_of[c->loop[j]]];
                    MadeFace *f = &faces[(*ntri)++];
                    s32 ja = 0, jb = 0, q;

                    for (q = 0; q < 3; q++) {
                        ja = by->v[q] == c->loop[j] ? q : ja;
                        jb = by->v[q] == c->loop[(j + 1) % c->n] ? q : jb;
                    }
                    memset(f, 0, sizeof(*f));
                    f->v[0] = c->base + (j + 1) % c->n;
                    f->v[1] = c->base + j;
                    f->v[2] = c->base + c->n;
                    f->image = -1;
                    f->blend = by->blend;
                    for (m = 0; m < 2; m++) {
                        for (ch = 0; ch < 3; ch++) {
                            f->rgb[m][0][ch] = by->rgb[m][jb][ch];
                            f->rgb[m][1][ch] = by->rgb[m][ja][ch];
                            sum[m][ch] += by->rgb[m][ja][ch];
                        }
                    }
                }
                /* the middle the loop's colour */
                for (j = first; j < *ntri; j++) {
                    for (m = 0; m < 2; m++) {
                        for (ch = 0; ch < 3; ch++) {
                            faces[j].rgb[m][2][ch] = (u8)(sum[m][ch] / c->n);
                        }
                    }
                }
            }
            NCaps++;
        }
    }
}

/* Standing as he breathes (made), at t of the way round, with the bounce Crash Bash's heroes stand with (their idles move a corner 2 to
 * 4% of their height on average; his had moved 0.8%, and looked stiff by them): his knees bending from IDLE_KNEE to IDLE_KNEE +
 * IDLE_BOUNCE and back, his chest rising IDLE_BREATH, nodding IDLE_NOD, his shoulders turning IDLE_TURN and his arms swaying IDLE_ARM */
#define IDLE_KNEE 0.18f
#define IDLE_BOUNCE 0.5f
#define IDLE_BREATH 3.0f
#define IDLE_NOD 0.035f
#define IDLE_TURN 0.05f
#define IDLE_ARM 0.08f
static void stand(V3 *r, float t, const ArmPose *arms) {
    float a = 2 * PIN_PI * t, s = sinf(a), down = (1 - cosf(a)) / 2;

    memcpy(r, Rest, sizeof(V3) * Verts);
    pose_arms(r, arms, arms, 0, IDLE_ARM * s, 0, NULL);
    pose_body(r, IDLE_NOD * down, 0, IDLE_BREATH * s);
    pose_turn(r, IDLE_TURN * s);
    pose_knees(r, IDLE_KNEE + IDLE_BOUNCE * down);
}

/* His jog, two steps a go round (made), as Crash Bash's heroes run (their runs move a corner 11 to 43% of their height on average;
 * his walk had moved 8%): legs swinging WALK_HIP forward and back, the one coming forward bent WALK_KNEE at the knee, the body sinking
 * as the legs spread so the foot on the ground stays there and bobbing WALK_BOB, leaning WALK_LEAN forward, his shoulders turning
 * WALK_TURN against his hips, his elbows bent and his arms pumping WALK_ARM against his legs (or up, holding a crate, and steadier) */
#define WALK_HIP 0.55f
#define WALK_KNEE 1.1f
#define WALK_LEAN 0.11f
#define WALK_ARM 0.48f
#define WALK_BOB 7.0f
#define WALK_TURN 0.14f
static const ArmPose ArmsJog = { { 0.16f, -1, -0.12f }, { 0.06f, -0.8f, 0.6f } };
static void walk(V3 *r, float t, s32 holding) {
    float a = 2 * PIN_PI * t, leg_len = Legs[0].joint[0].y - Legs[0].joint[2].y;
    s32 i;

    memcpy(r, Rest, sizeof(V3) * Verts);
    if (holding) {
        pose_arms(r, &ArmsUp, &ArmsUp, 0, 0, 0, NULL);
    } else {
        pose_arms(r, &ArmsJog, &ArmsJog, 0, -WALK_ARM * sinf(a), 0, NULL);
    }
    pose_body(r, WALK_LEAN, -WALK_BOB * (holding ? 0.5f : 1) * sinf(a) * sinf(a), 0);
    pose_turn(r, WALK_TURN * (holding ? 0.3f : 1) * sinf(a));
    for (i = 0; i < 2; i++) {
        /* the first leg forward as the first arm (on its side) goes back */
        float ph = i == 0 ? a : a + PIN_PI, hip = WALK_HIP * sinf(ph), swing = cosf(ph) > 0 ? cosf(ph) : 0;

        float knee = WALK_KNEE * swing * swing;

        pose_leg(r, &Legs[i], hip, knee, (knee - hip) * 0.8f - 0.15f * swing);
    }
    /* sunk as far as a straight leg at its angle comes up short */
    pose_shift(r, v3(0, -leg_len * (1 - cosf(WALK_HIP * sinf(a))), 0));
}

/* His jump (made), in the three parts a jump's three sequences play (PinStands' @JUMP, @FALL and @LAND, as Cortex's JUMP is split: its
 * frames 0 to 7 of 20, 8 to 12 and 13 to 19), the last frame of each held for as long as the game takes: a crouch and up off his feet,
 * legs tucking up and arms out as he rises (the game lifts him); his legs reaching down for the floor as he falls; and landing, down
 * into a crouch and up again, standing by its end */
static void jump(V3 *r, float t) {
    float crouch = hm_smooth(0, 0.15f, t) * hm_smooth(0.32f, 0.2f, t) + hm_smooth(0.64f, 0.72f, t) * hm_smooth(0.92f, 0.78f, t);
    float tuck = hm_smooth(0.2f, 0.36f, t) * hm_smooth(0.6f, 0.45f, t), arms = hm_smooth(0.12f, 0.3f, t) * hm_smooth(0.82f, 0.64f, t);
    s32 i;

    memcpy(r, Rest, sizeof(V3) * Verts);
    pose_arms(r, &ArmsDown, &ArmsUp, arms * 0.55f, 0, -0.5f * crouch, NULL);
    pose_body(r, 0.25f * crouch + 0.1f * tuck, 0, 0);
    for (i = 0; i < 2; i++) {
        pose_leg(r, &Legs[i], 0.7f * crouch + (i == 0 ? 1.0f : 0.8f) * tuck, 1.4f * crouch + 1.6f * tuck, -0.6f * crouch);
    }
    pose_shift(r, v3(0, -32 * crouch, 0));
}

/* Picking up a crate (made): down on his knees reaching for it, then up holding it over his head */
static void pickup(V3 *r, float t) {
    float down = hm_smooth(0, 0.35f, t) * hm_smooth(1.0f, 0.55f, t), lift = hm_smooth(0.45f, 1.0f, t);
    s32 i;

    memcpy(r, Rest, sizeof(V3) * Verts);
    pose_arms(r, &ArmsForward, &ArmsUp, lift, 0, -0.6f * down, NULL);
    pose_body(r, 0.5f * down, 0, 0);
    for (i = 0; i < 2; i++) {
        pose_leg(r, &Legs[i], 0.8f * down, 1.5f * down, -0.7f * down);
    }
    pose_shift(r, v3(0, -40 * down, 0));
}

/* Throwing it (made): arms from up over his head forward and down, leaning into it, then back to standing */
static void throw_crate(V3 *r, float t) {
    float fwd = hm_smooth(0.1f, 0.45f, t), back = hm_smooth(0.6f, 1.0f, t);

    memcpy(r, Rest, sizeof(V3) * Verts);
    if (back <= 0) {
        pose_arms(r, &ArmsUp, &ArmsForward, fwd, 0, 0, NULL);
    } else {
        pose_arms(r, &ArmsForward, &ArmsDown, back, 0, 0, NULL);
    }
    pose_body(r, 0.25f * fwd * (1 - back), 0, 0);
}

/* His attack: his stride's fire, a frame of it a tick from ATTACK_FROM, his hands coming up from standing to the gun over ATTACK_DRAW
 * ticks as it grows into them, its muzzle flashing as his game's does while it's all out, and back and away again over the last
 * ATTACK_DRAW. The gun's held as he stands too (his arms carry it), so it comes out of his hands wherever they are. */
#define ATTACK_FROM 2.0f
#define ATTACK_DRAW 3.0f
static void attack(V3 *r, s32 f, s32 frames) {
    static V3 s[MAX_VERTS];
    float tick = (float)f / PIN_SUBFRAMES, ticks = (float)frames / PIN_SUBFRAMES;
    float in = hm_smooth(0, ATTACK_DRAW, tick), out = hm_smooth(ticks - 1, ticks - 1 - ATTACK_DRAW, tick), w = in < out ? in : out;
    s32 k;

    stand(s, 0, &ArmsDown);
    src_at(Src[A_ATTACK], ATTACK_FROM + tick, r);
    for (k = 0; k < Verts; k++) {
        r[k] = hm_mix(s[k], r[k], w);
    }
    place_gun(r, w, 1);
}

/* His win: his boss fight's laughing spray (PiG2V), a frame of it a tick round and round, the gun out all the while and its muzzle
 * flashing as it does there (a burst every third frame, the flash's own piece growing), shooting it about as he does in that fight */
static void win(V3 *r, s32 f) {
    static V3 a[MAX_VERTS], b[MAX_VERTS];
    const C1Entry *src = Src[A_WIN];
    float tick = (float)f / PIN_SUBFRAMES;
    s32 f0 = (s32)tick % src->count, f1 = (f0 + 1) % src->count, k;

    src_at(src, (float)f0, a);
    src_at(src, (float)f1, b);
    for (k = 0; k < Verts; k++) {
        r[k] = hm_mix(a[k], b[k], tick - (s32)tick);
    }
    place_gun(r, 1, 1);
}

/* One of his game's, resampled to `frames`, the gun away; his arms down (as his chest moves) if `arms_down` */
enum { ARMS_GAME, ARMS_DOWN, ARMS_HURT };

/* As he's hurt his game has his arms on the gun, so they're his rig's: flung out and up as the blow jolts him (from `from` to `peak`
 * of his game's frames, held to `hold`) and down by `end`, hanging as he recovers or goes down, moving as his chest does */
typedef struct {
    float from, peak, hold, end;
} Fling;
static const ArmPose ArmsFlung = { { 0.85f, 0.05f, -0.4f }, { 0.6f, 0.5f, -0.25f } };
static const Fling HitFling = { 0.5f, 3, 6, 11 }, DieFling = { 0, 2, 4, 9 };

/* One of his game's, resampled to `frames`, the gun away; his arms as his game has them, down, or hurt (as his chest moves) */
static void from_game(V3 *r, s32 j, s32 f, s32 frames, s32 arms) {
    const C1Entry *a = Src[j];
    float t = frames > 1 ? (float)f * (a->count - 1) / (frames - 1) : 0;
    Moved chest;

    src_at(a, t, r);
    if (arms == ARMS_DOWN) {
        chest_move(r, &chest);
        pose_arms(r, &ArmsDown, &ArmsDown, 0, 0, 0, &chest);
    } else if (arms == ARMS_HURT) {
        const Fling *fl = j == A_DIE ? &DieFling : &HitFling;

        chest_move(r, &chest);
        pose_arms(r, &ArmsDown, &ArmsFlung, hm_smooth(fl->from, fl->peak, t) * hm_smooth(fl->end, fl->hold, t), 0, 0, &chest);
    }
}

/* Frame f of animation j, in the rig: his vertices and the gun's copies */
static void pose(s32 j, s32 f, V3 *r) {
    s32 n = Frames[j];
    float t = n > 0 ? (float)f / n : 0;

    switch (j) {
    case A_WALK:
        walk(r, t, 0);
        break;
    case A_HOLDWALK:
        walk(r, t, 1);
        break;
    case A_JUMP:
        jump(r, n > 1 ? (float)f / (n - 1) : 0);
        break;
    case A_HOLD:
        stand(r, t, &ArmsUp);
        break;
    case A_PICKUP:
        pickup(r, n > 1 ? (float)f / (n - 1) : 0);
        break;
    case A_THROW:
        throw_crate(r, n > 1 ? (float)f / (n - 1) : 0);
        break;
    case A_ATTACK:
        attack(r, f, n);
        return; /* its own gun */
    case A_HIT:
    case A_DIE:
        from_game(r, j, f, n, ARMS_HURT);
        break;
    case A_WIN:
        win(r, f);
        return; /* its own gun */
    case A_TAUNT:
        from_game(r, j, f, n, ARMS_DOWN);
        break;
    default:
        stand(r, t, &ArmsDown);
        break;
    }
    place_gun(r, 0, 0);
}

typedef struct {
    float k, ground; /* his rig's units to Crash Bash's, and how far down his feet are then */
} FootCtx;

static void foot_frame(void *ctx, s32 j, s32 f, V3 *out) {
    static V3 r[MAX_VERTS];
    const FootCtx *c = ctx;
    s32 k;

    pose(j, f, r);
    grow_head(r);
    for (k = 0; k < Verts + Copies + CapVerts; k++) {
        V3 p = from_rig(r[k]);

        out[k].x = p.x * c->k;
        out[k].y = p.y * c->k - c->ground;
        out[k].z = p.z * c->k;
    }
}

/* --- riding a polar bear (Polar Push) ---
 * Sat astride Crash's bear (ride_make, bonusheroes.c, which moves him with its back and leans him as Crash leans): each thigh out
 * over its side RIDE_SPREAD and forward RIDE_FLEX at his hip, his shins bent RIDE_KNEE back down its flanks, his toes tipped
 * RIDE_FOOT down, his hips RIDE_HIP_UP over its back where Crash sits; sat up a little back (RIDE_LEAN: as the user approved him,
 * from when pose_body leaned back for this forward lean), his hands forward on its shoulders (ArmsRide), the gun away. As Roo, he's as
 * heavy and as quick as Crash on it, his moves as long as his base's. */
#define RIDE_SPREAD 0.75f
#define RIDE_FLEX 1.3f
#define RIDE_KNEE 2.1f
#define RIDE_FOOT 0.35f
#define RIDE_LEAN -0.14f
#define RIDE_HIP_UP 4.0f /* Crash Bash's units */
#define RIDE_Z 4.0f
#define RIDE_LEAN_WAIST 0.8f
#define RIDE_LEAN_NECK 0.2f
#define RIDE_STEADY 0.35f /* of Crash's tilt from upright, the share he keeps back from: he's taller than Crash */
#define RIDE_CHARRED 0.35f /* his colours' share when a bomb's charred him */
#define RIDE_SCALE 0.8f /* his size on it, about his hips: the bear is Crash's, and his legs reached the floor beside it */
static const ArmPose ArmsRide = { { 0.3f, -0.72f, 0.6f }, { 0.04f, -0.88f, 0.46f } };

/* A leg astride (made): out to its side `spread` radians and forward `flex` at the hip, its shin back `knee` at the knee, its shoe
 * tipped `foot` more at the ankle */
static void pose_astride(V3 *r, const Leg *leg, float spread, float flex, float knee, float foot) {
    V3 o = { 0, 0, 0 };
    s32 i;

    for (i = 0; i < leg->n; i++) {
        s32 k = leg->verts[i];
        V3 p = Rest[k], shin;

        if (leg->shoe[i]) {
            p = vadd(leg->joint[2], hm_turn_x(vsub(p, leg->joint[2]), o, -foot));
        }
        shin = vadd(leg->joint[1], hm_turn_x(vsub(p, leg->joint[1]), o, knee));
        p = leg->shoe[i] ? shin : hm_mix(p, shin, leg->shin[i]);
        p = hm_turn_z(p, leg->joint[0], leg->side * spread);
        r[k] = hm_turn_x(p, leg->joint[0], -flex);
    }
}

/* Sat on the bear, in his rig: all of him, the gun's copies, his palms' caps and his head as foot_frame has them */
static void seated(V3 *r) {
    s32 i;

    memcpy(r, Rest, sizeof(V3) * Verts);
    pose_arms(r, &ArmsRide, &ArmsRide, 0, 0, 0, NULL);
    pose_body(r, RIDE_LEAN, 0, 0);
    for (i = 0; i < 2; i++) {
        pose_astride(r, &Legs[i], RIDE_SPREAD, RIDE_FLEX, RIDE_KNEE, RIDE_FOOT);
    }
    place_gun(r, 0, 0);
    grow_head(r);
}

/* The vertex of his whose place in him a vertex has: itself, a copy's of the gun, or a palm cap's first corner */
static s32 own_vertex(s32 k) {
    s32 c;

    if (k < Verts) {
        return k;
    }
    if (k < Verts + Copies) {
        return CopyFrom[k - Verts];
    }
    for (c = 0; c < NCaps; c++) {
        if (k >= Caps[c].base && k <= Caps[c].base + Caps[c].n) {
            return Caps[c].loop[0];
        }
    }
    return 0;
}

/* Where he's sat on the bear's back (ride_bear): his units to Crash Bash's, the top of its back and its middle */
static struct {
    float k, back_y, seat_z;
    s32 n;
} RideAt;

/* A point of him in his rig as he sits in Crash Bash's: RIDE_SCALE of him, about the middle of his hips, his hips on its back */
static V3 to_seat(V3 r) {
    V3 hips = { 0, Legs[0].joint[0].y, Legs[0].joint[0].z }, p;
    float hip = Legs[0].joint[0].y;

    p = from_rig(vadd(hips, vmul(vsub(r, hips), RIDE_SCALE)));
    return v3(p.x * RideAt.k, p.y * RideAt.k + hip * PIN_UNIT * RideAt.k - RIDE_HIP_UP + RideAt.back_y,
              p.z * RideAt.k + RideAt.seat_z + RIDE_Z);
}

/* A direction in Crash Bash's axes in his rig's (y and z the other way round), and a turn of Crash Bash's on one in his rig */
static V3 rig_dir(V3 d) {
    return v3(d.x, -d.y, -d.z);
}
static V3 bash_turned(const float m[3][3], V3 v) {
    V3 b = rig_dir(v);

    return rig_dir(v3(m[0][0] * b.x + m[0][1] * b.y + m[0][2] * b.z, m[1][0] * b.x + m[1][1] * b.y + m[1][2] * b.z,
                      m[2][0] * b.x + m[2][1] * b.y + m[2][2] * b.z));
}

/* Thrown about at the ice's edge (FALL: the teeter, then the drop), he goes as Crash does on his bear (ride_crash_body): each of his
 * arms turned from how he sits as Crash's turns from how Crash sat, and then, as Crash's goes further (a turn of FOLLOW_FROM to
 * FOLLOW_ALL radians), more and more the very way Crash's goes, so he flails his arms out and up as Crash does; his body above his
 * waist turned as Crash's body turns, back and over to his left; his legs astride still; and all of him moved as Crash's hips move,
 * aside and back off the bear's back as he hangs on, and above it as it drops away from under him. Without Crash's body, as he sits. */
#define FOLLOW_FROM 0.3f
#define FOLLOW_ALL 1.1f
static void ride_own(void *ctx, s32 anim, s32 f, s32 frames, V3 *verts) {
    static V3 r[MAX_VERTS];
    V3 waist = { 0, WAIST_Y, 0 };
    CrashBody cb;
    s32 i, j, k;

    (void)ctx;
    (void)frames;
    if (anim != RIDE_FALL || !ride_crash_body(anim, f, &cb)) {
        return;
    }
    memcpy(r, Rest, sizeof(V3) * Verts);
    for (i = 0; i < 2; i++) {
        V3 own[2], dir[2];
        Turn t[2];
        float w = 0;

        own[0] = sided(ArmsRide.up, Arms[i].side);
        own[1] = sided(ArmsRide.fore, Arms[i].side);
        for (j = 0; j < 2; j++) {
            t[j] = turn_from(vnorm(rig_dir(cb.arm_was[i][j])), vnorm(rig_dir(cb.arm[i][j])));
            w = fmaxf(w, hm_smooth(FOLLOW_FROM, FOLLOW_ALL, t[j].angle));
        }
        for (j = 0; j < 2; j++) {
            dir[j] = vnorm(hm_mix(turned(&t[j], vnorm(own[j])), vnorm(rig_dir(cb.arm[i][j])), w));
        }
        pose_arm(r, &Arms[i], dir[0], dir[1], NULL);
    }
    pose_body(r, RIDE_LEAN, 0, 0);
    for (k = 0; k < Verts; k++) {
        if (PieceOf[k] != PIECE_LEGS && PieceOf[k] != PIECE_SHOE) {
            r[k] = hm_mix(r[k], vadd(waist, bash_turned(cb.body, vsub(r[k], waist))),
                          hm_smooth(WAIST_Y - WAIST_BLEND, WAIST_Y + WAIST_BLEND, Rest[k].y));
        }
    }
    for (i = 0; i < 2; i++) {
        pose_astride(r, &Legs[i], RIDE_SPREAD, RIDE_FLEX, RIDE_KNEE, RIDE_FOOT);
    }
    place_gun(r, 0, 0);
    grow_head(r);
    for (k = 0; k < RideAt.n; k++) {
        verts[k] = vadd(to_seat(r[k]), cb.hips);
    }
}

/* His bear model: sat on its back, his faces and images; 0 if there's no bear or it couldn't */
static s32 ride_bear(const FootCtx *foot, const MadeFace *faces, s32 nfaces, const MadeImage *images, s32 nimages, HeroMade *out) {
    static V3 r[MAX_VERTS], seat[MAX_VERTS];
    static float waist_share[MAX_VERTS], neck_share[MAX_VERTS];
    float back_y, seat_z, s = PIN_UNIT * foot->k, hip = Legs[0].joint[0].y;
    s32 n = Verts + Copies + CapVerts, k;
    Rider rider;
    V3 w, nk;

    if (!ride_seat(&back_y, &seat_z)) {
        return 0;
    }
    RideAt.k = foot->k;
    RideAt.back_y = back_y;
    RideAt.seat_z = seat_z;
    RideAt.n = n;
    seated(r);
    for (k = 0; k < n; k++) {
        s32 own = own_vertex(k);

        seat[k] = to_seat(r[k]);
        waist_share[k] = hm_smooth(WAIST_Y - WAIST_BLEND, WAIST_Y + WAIST_BLEND, Rest[own].y);
        neck_share[k] = hm_smooth(NECK_Y - NECK_BLEND, NECK_Y + NECK_BLEND, Rest[own].y);
    }
    w = from_rig(v3(0, hip + (WAIST_Y - hip) * RIDE_SCALE, 0));
    nk = from_rig(v3(0, hip + (NECK_Y - hip) * RIDE_SCALE, 0));
    memset(&rider, 0, sizeof(rider));
    rider.seat = seat;
    rider.nvert = n;
    rider.waist_share = waist_share;
    rider.neck_share = neck_share;
    rider.waist = v3(w.x * foot->k, w.y * foot->k + hip * s - RIDE_HIP_UP + back_y, w.z * foot->k + seat_z + RIDE_Z);
    rider.neck = v3(nk.x * foot->k, nk.y * foot->k + hip * s - RIDE_HIP_UP + back_y, nk.z * foot->k + seat_z + RIDE_Z);
    rider.lean_waist = RIDE_LEAN_WAIST;
    rider.lean_neck = RIDE_LEAN_NECK;
    rider.steady = RIDE_STEADY;
    rider.match_bank = DB_CharsPolar2Cortex; /* his base's: its moves then go a frame a tick */
    rider.faces = faces;
    rider.nfaces = nfaces;
    rider.images = images;
    rider.nimages = nimages;
    rider.charred = RIDE_CHARRED;
    rider.own = ride_own;
    rider.follow = 1 << RIDE_FALL;
    return ride_make(&rider, &out->tex[KIND_POLAR], &out->tex_size[KIND_POLAR], &out->mdl[KIND_POLAR], &out->mdl_size[KIND_POLAR]);
}

/* --- on a pogo stick (Pogo) ---
 * Pogo's heroes each bounce on a thing of their own, in a model of their own with it (models\chars\pogo\): Crash and Coco on pogo
 * sticks, Cortex and Brio on their jetpacks, Tiny and Kong on springs. He bounces on Crash's stick, as he rides as Crash does (the user,
 * of his bear: not with Cortex's jetpack). His pogo model is that stick, as it moves in Crash's (its strips keep_stick picks: the ones
 * that reach down to its pegs, and its handle's grips in front of Crash), its tube POGO_TUBE as long above its pegs so its handle's
 * at his waist, its blues gunmetal (stick_metal: dark, as his tommy gun is, against his red suit); and him on it at POGO_SCALE of his
 * size, his feet on its pegs and his hands on its grips (pose_leg_to, pose_arm_to: each limb's two bones put to reach them). He
 * bounces as Crash does on it, in the stick's own frame (its frame, pegs and grips fitted each frame; its tip slides in and out): his
 * hips and feet moved as Crash's are (`follow` as far, for his height against Crash's; POGO_SQUAT lower than he stands), so he springs
 * up off the pegs at the top of a bounce as Crash does, holding on, his limbs stretching to the grips and pegs as Crash's do when
 * they're out of reach (two_bone); his body above his waist leaning POGO_LEAN forward over it and turned as Crash's turns, his spine
 * and shoulders; and a hand Crash lets go with (he punches the air as he wins) going as Crash's arm goes. Pogo's five moves are
 * Crash's, each as long: the bounce, the hop, dazed, the win and the tazing; the win and the tazing at their own speed (PinPogo), as his
 * base's are 87 and 4 ticks long. */
#define STICK_LOW -76.0f      /* a strip of the stick's reaches below this (Crash Bash's axes, y down): its tube, frame and pegs; */
#define GRIP_TOP -276.0f      /* and its grips are all within this box, in front of Crash (whose hands are on them, and higher) */
#define GRIP_LOW -230.0f
#define GRIP_BACK -60.0f
#define GRIP_FRONT -105.0f
#define GRIP_HALF 76.0f
#define TIP_HALF 15.0f        /* its tip, which slides in and out: its corners within this of its middle across, below TIP_TOP */
#define TIP_TOP -45.0f
#define PEG_ASIDE 30.0f       /* its pegs: its corners below STICK_LOW and more than this aside */
#define POGO_HIPS_TOP -190.0f /* Crash on it: his hips, his corners from POGO_HIPS_TOP down to POGO_HIPS_LOW within POGO_MIDDLE aside; */
#define POGO_HIPS_LOW -155.0f
#define POGO_MIDDLE 30.0f
#define POGO_CHEST_TOP -330.0f /* his chest, from POGO_CHEST_TOP to POGO_CHEST_LOW within POGO_CHEST_HALF, behind POGO_CHEST_FRONT (his */
#define POGO_CHEST_LOW -255.0f /* hands are in front, on the grips); and his shoulders, within POGO_SHOULDER_R of PogoShoulders */
#define POGO_CHEST_HALF 40.0f
#define POGO_CHEST_FRONT -80.0f
#define POGO_SHOULDER_R 20.0f
#define POGO_FEET_TOP -100.0f  /* and his feet, his corners below this more than POGO_MIDDLE aside: they leave the pegs as he springs; */
#define POGO_ELBOW_R 25.0f     /* his elbows, within POGO_ELBOW_R of PogoElbows; and his hands, within POGO_HAND_R of the grips and */
#define POGO_HAND_R 45.0f      /* in front of POGO_HAND_BACK. As Crash wins he lets go with a hand and punches the air with it, so a */
#define POGO_HAND_BACK -55.0f  /* hand of his goes as Crash's goes off its grip, from POGO_OFF_FROM further from it to all by POGO_OFF_ALL */
#define POGO_OFF_FROM 10.0f
#define POGO_OFF_ALL 40.0f
static const V3 PogoShoulders[2] = { { -50, -302, 10 }, { 50, -302, 10 } };
static const V3 PogoElbows[2] = { { -126, -297, -25 }, { 126, -300, -27 } };
#define POGO_SCALE 0.8f
#define POGO_TUBE 1.3f
#define POGO_SQUAT 12.0f /* his rig's units */
#define POGO_LEAN 0.25f
#define POGO_METAL 0.45f  /* the stick's blues as greys this much of their brightness */
#define POGO_PART_MAX 128
#define POGO_CORNERS 1024
#define POGO_MOVES 5
static const char *const PogoNames[POGO_MOVES] = { "BREATHE", "JUMP", "DAZED", "WIN", "TAZING" };

typedef struct {
    GamePart stick;              /* Crash's pogo model, the stick its part: all its corners as it moves (hm_model_at) */
    s32 anim[POGO_MOVES];
    V3 ref[POGO_CORNERS];        /* its corners as Crash first bounces (BREATHE's first frame) */
    s16 frame[POGO_PART_MAX], hips[POGO_PART_MAX], chest[POGO_PART_MAX], shoulder[2][POGO_PART_MAX], feet[2][POGO_PART_MAX];
    s16 elbow[2][POGO_PART_MAX], hand[2][POGO_PART_MAX];
    s32 nframe, nhips, nchest, nshoulder[2], nfeet[2], nelbow[2], nhand[2];
    V3 grip_at[2];               /* the grips' middles as it rests (Crash Bash's axes), */
    float hand_off[2];           /* and how far Crash's hands' are from them then */
    float peg_y;                 /* the top of its pegs, which its tube's lengthened from */
    float k;                     /* Crash Bash's units to a unit of his rig, at POGO_SCALE */
    float follow;                /* his height on it against Crash's: how much further he moves than Crash does on it */
    V3 origin;                   /* where his rig's floor point is on the stick as it rests */
    V3 grip[2], ankle[2], palm[2]; /* in his rig: each hand's target on its grip, each ankle's on its peg, and each palm as he stands */
    float was[3][3];             /* how Crash's body lies as he first bounces (body_lie) */
    MadeAnim anims[POGO_MOVES];
} PogoCtx;
static PogoCtx Pogo;

/* The stick's strips, by where their corners are as Crash first bounces (the model's own pose is another: his arms out): `ref`, all
 * its corners then, and `at`, the strip's first, counted on as the strips go by */
typedef struct {
    const V3 *ref;
    s32 at;
} StickPick;

static s32 keep_all(void *ctx, const V3 *rest, s32 n, s32 textured) {
    (void)ctx;
    (void)rest;
    (void)n;
    (void)textured;
    return 1;
}

static s32 keep_stick(void *ctx, const V3 *rest, s32 n, s32 textured) {
    StickPick *pick = ctx;
    const V3 *p = pick->ref + pick->at;
    s32 k, low = 0, grip = 1;

    (void)rest;
    pick->at += n;
    if (textured) {
        return 0;
    }
    for (k = 0; k < n; k++) {
        low |= p[k].y > STICK_LOW;
        grip &= p[k].y >= GRIP_TOP && p[k].y <= GRIP_LOW && p[k].z >= GRIP_FRONT && p[k].z <= GRIP_BACK && fabsf(p[k].x) <= GRIP_HALF;
    }
    return low || grip;
}

static V3 mid_of(const V3 *at, const s16 *ids, s32 n) {
    V3 c = { 0, 0, 0 };
    s32 i;

    for (i = 0; i < n; i++) {
        c = vadd(c, at[ids[i]]);
    }
    return n > 0 ? vmul(c, 1.0f / n) : c;
}

/* How Crash's body lies, his corners at `at`: a frame (its columns across, up and forward) of his spine, hips to chest, and his
 * shoulders' line across it */
static void body_lie(const PogoCtx *c, const V3 *at, float m[3][3]) {
    V3 up = vnorm(vsub(mid_of(at, c->chest, c->nchest), mid_of(at, c->hips, c->nhips)));
    V3 across = vsub(mid_of(at, c->shoulder[1], c->nshoulder[1]), mid_of(at, c->shoulder[0], c->nshoulder[0])), fwd;

    across = vnorm(vsub(across, vmul(up, vdot(across, up))));
    fwd = vcross(across, up);
    m[0][0] = across.x, m[0][1] = up.x, m[0][2] = fwd.x;
    m[1][0] = across.y, m[1][1] = up.y, m[1][2] = fwd.y;
    m[2][0] = across.z, m[2][1] = up.z, m[2][2] = fwd.z;
}

/* A turn of Crash Bash's undone on a direction in his rig */
static V3 bash_unturned(const float m[3][3], V3 v) {
    V3 b = rig_dir(v);

    return rig_dir(v3(m[0][0] * b.x + m[1][0] * b.y + m[2][0] * b.z, m[0][1] * b.x + m[1][1] * b.y + m[2][1] * b.z,
                      m[0][2] * b.x + m[1][2] * b.y + m[2][2] * b.z));
}

/* Where a two-bone limb's middle joint goes, from `root` reaching for `target` (bones l1 then l2), bent toward `hint`; and, if it's out
 * of reach, how much longer both bones are to reach it (`stretch`, else 1), as Crash's limbs stretch as he springs off his stick */
static V3 two_bone(V3 root, V3 target, float l1, float l2, V3 hint, float *stretch) {
    V3 d = vsub(target, root), n;
    float dist = vlen(d), a, h;

    *stretch = dist > l1 + l2 ? dist / (l1 + l2) : 1;
    l1 *= *stretch;
    l2 *= *stretch;
    dist = dist < fabsf(l1 - l2) + 0.01f ? fabsf(l1 - l2) + 0.01f : dist > l1 + l2 - 0.01f ? l1 + l2 - 0.01f : dist;
    d = vnorm(d);
    a = (l1 * l1 - l2 * l2 + dist * dist) / (2 * dist);
    h = sqrtf(fmaxf(0, l1 * l1 - a * a));
    n = vsub(hint, vmul(d, vdot(hint, d)));
    n = vlen(n) > 1e-4f ? vnorm(n) : v3(0, 0, 1);
    return vadd(vadd(root, vmul(d, a)), vmul(n, h));
}

/* p, a point of a bone from `from` along `dir`, its length along it `stretch` as long */
static V3 stretched(V3 p, V3 from, V3 dir, float stretch) {
    return vadd(p, vmul(dir, vdot(vsub(p, from), dir) * (stretch - 1)));
}

/* An arm put so its palm (`palm` as he stands) is at `target`, its elbow bent toward `hint`; its shoulder where it stands */
static void pose_arm_to(V3 *r, const Arm *arm, V3 palm, V3 target, V3 hint) {
    V3 s = arm->joint[0], e0 = arm->joint[1], elbow, up, fore;
    float st;
    Turn t0, t1;
    s32 i;

    elbow = two_bone(s, target, arm->len[0], vlen(vsub(palm, e0)), hint, &st);
    up = vnorm(vsub(elbow, s));
    fore = vnorm(vsub(target, elbow));
    t0 = turn_from(vnorm(vsub(e0, s)), up);
    t1 = turn_from(turned(&t0, vnorm(vsub(palm, e0))), fore);
    for (i = 0; i < arm->n; i++) {
        s32 k = arm->verts[i];
        V3 p = Rest[k], a = stretched(vadd(s, turned(&t0, vsub(p, s))), s, up, st);
        V3 b = stretched(vadd(elbow, turned(&t1, turned(&t0, vsub(p, e0)))), elbow, fore, st);

        r[k] = hm_mix(p, hm_mix(a, b, arm->fore[i]), arm->weight[i]);
    }
}

/* A leg from its hip at `hip` to its ankle at `ankle`, its knee bent toward `hint`, its shoe flat as he stands */
static void pose_leg_to(V3 *r, const Leg *leg, V3 hip, V3 ankle, V3 hint) {
    V3 h0 = leg->joint[0], k0 = leg->joint[1], a0 = leg->joint[2], knee, thigh, shin;
    float st;
    Turn t0, t1;
    s32 i;

    knee = two_bone(hip, ankle, vlen(vsub(k0, h0)), vlen(vsub(a0, k0)), hint, &st);
    thigh = vnorm(vsub(knee, hip));
    shin = vnorm(vsub(ankle, knee));
    t0 = turn_from(vnorm(vsub(k0, h0)), thigh);
    t1 = turn_from(turned(&t0, vnorm(vsub(a0, k0))), shin);
    for (i = 0; i < leg->n; i++) {
        s32 k = leg->verts[i];
        V3 p = Rest[k];

        if (leg->shoe[i]) {
            r[k] = vadd(ankle, vsub(p, a0));
        } else {
            r[k] = hm_mix(stretched(vadd(hip, turned(&t0, vsub(p, h0))), hip, thigh, st),
                          stretched(vadd(knee, turned(&t1, turned(&t0, vsub(p, k0)))), knee, shin, st), leg->shin[i]);
        }
    }
}

/* A point of his rig on the stick as it rests, in Crash Bash's units and axes, and back */
static V3 rig_on_stick(const PogoCtx *c, V3 r) {
    return vadd(c->origin, v3(r.x * c->k, -r.y * c->k, -r.z * c->k));
}
static V3 stick_in_rig(const PogoCtx *c, V3 p) {
    return v3((p.x - c->origin.x) / c->k, -(p.y - c->origin.y) / c->k, -(p.z - c->origin.z) / c->k);
}

/* Crash's body on the stick for a frame, in its own frame (Crash Bash's axes): how far his hips and feet have moved, his body's turn
 * (M), and each arm: how far off its grip its hand's gone (0 to 1) and its upper arm and forearm as they'd be with his body unturned */
typedef struct {
    V3 hips, feet[2];
    float M[3][3];
    float off[2];
    V3 arm[2][2];
} PogoBody;

/* His pose on the stick (in his rig) as Crash's is (b) */
static void pogo_pose(const PogoCtx *c, const PogoBody *b, V3 *r) {
    V3 waist = { 0, WAIST_Y, 0 }, o = { 0, 0, 0 }, move = vmul(rig_dir(b->hips), c->follow / c->k);
    s32 i, k;

    move.y -= POGO_SQUAT;
    memcpy(r, Rest, sizeof(V3) * Verts);
    /* each hand to its grip: the grip taken back by how his body will be moved, turned and leant, and the arm put to it there; or, as
     * far as Crash's has let go of his, where Crash's arm would put it (his upper arm and forearm as Crash's, leant back as he'll lean) */
    for (i = 0; i < 2; i++) {
        V3 t = vsub(c->grip[i], move), up = hm_turn_x(rig_dir(b->arm[i][0]), o, -POGO_LEAN), fore = hm_turn_x(rig_dir(b->arm[i][1]), o, -POGO_LEAN);
        V3 own = vadd(Arms[i].joint[0], vadd(vmul(up, Arms[i].len[0]), vmul(fore, vlen(vsub(c->palm[i], Arms[i].joint[1])))));

        t = vadd(waist, bash_unturned(b->M, vsub(t, waist)));
        t = hm_turn_x(t, waist, -POGO_LEAN);
        pose_arm_to(r, &Arms[i], c->palm[i], hm_mix(t, own, b->off[i]), vnorm(hm_mix(v3(Arms[i].side, -0.3f, -0.5f), up, b->off[i])));
    }
    pose_body(r, POGO_LEAN, 0, 0);
    for (k = 0; k < Verts; k++) {
        if (PieceOf[k] != PIECE_LEGS && PieceOf[k] != PIECE_SHOE) {
            r[k] = hm_mix(r[k], vadd(waist, bash_turned(b->M, vsub(r[k], waist))),
                          hm_smooth(WAIST_Y - WAIST_BLEND, WAIST_Y + WAIST_BLEND, Rest[k].y));
            r[k] = vadd(r[k], move);
        }
    }
    for (i = 0; i < 2; i++) {
        pose_leg_to(r, &Legs[i], vadd(Legs[i].joint[0], move), vadd(c->ankle[i], vmul(rig_dir(b->feet[i]), c->follow / c->k)),
                    v3(Legs[i].side * 0.3f, 0, 1));
    }
    place_gun(r, 0, 0);
    grow_head(r);
}

static void pogo_frame(void *ctx, s32 a, s32 f, V3 *out) {
    static V3 F[POGO_CORNERS], L[POGO_CORNERS], r[MAX_VERTS];
    const PogoCtx *c = ctx;
    float now[3][3];
    s32 n = Verts + Copies + CapVerts, i, j, k;
    PogoBody b;
    Moved mv;

    if (!hm_model_at(&c->stick, c->anim[a], (float)f, F)) {
        memcpy(F, c->ref, sizeof(V3) * (size_t)c->stick.corners);
    }
    /* the stick's frame (its tip slides), and all of it in it */
    hm_fit(c->ref, F, c->frame, c->nframe, &mv);
    for (k = 0; k < c->stick.corners; k++) {
        L[k] = vadd(mv.from, hm_unturned(&mv, vsub(F[k], mv.to)));
    }
    body_lie(c, L, now);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            b.M[i][j] = now[i][0] * c->was[j][0] + now[i][1] * c->was[j][1] + now[i][2] * c->was[j][2];
        }
    }
    b.hips = vsub(mid_of(L, c->hips, c->nhips), mid_of(c->ref, c->hips, c->nhips));
    for (i = 0; i < 2; i++) {
        V3 s = mid_of(L, c->shoulder[i], c->nshoulder[i]), e = mid_of(L, c->elbow[i], c->nelbow[i]), h = mid_of(L, c->hand[i], c->nhand[i]);
        V3 u = vsub(e, s), w = vsub(h, e);

        b.feet[i] = vsub(mid_of(L, c->feet[i], c->nfeet[i]), mid_of(c->ref, c->feet[i], c->nfeet[i]));
        b.off[i] = hm_smooth(POGO_OFF_FROM, POGO_OFF_ALL, vlen(vsub(h, c->grip_at[i])) - c->hand_off[i]);
        /* unturned by his body's turn (its transpose) */
        b.arm[i][0] = vnorm(v3(b.M[0][0] * u.x + b.M[1][0] * u.y + b.M[2][0] * u.z, b.M[0][1] * u.x + b.M[1][1] * u.y + b.M[2][1] * u.z,
                               b.M[0][2] * u.x + b.M[1][2] * u.y + b.M[2][2] * u.z));
        b.arm[i][1] = vnorm(v3(b.M[0][0] * w.x + b.M[1][0] * w.y + b.M[2][0] * w.z, b.M[0][1] * w.x + b.M[1][1] * w.y + b.M[2][1] * w.z,
                               b.M[0][2] * w.x + b.M[1][2] * w.y + b.M[2][2] * w.z));
    }
    pogo_pose(c, &b, r);
    for (k = 0; k < n; k++) {
        out[k] = hm_moved(&mv, rig_on_stick(c, r[k]));
    }
    for (k = 0; k < c->stick.nvert; k++) {
        V3 p = L[c->stick.corner_of[k]];

        if (p.y < c->peg_y) {
            p.y = c->peg_y + (p.y - c->peg_y) * POGO_TUBE;
        }
        out[n + k] = hm_moved(&mv, p);
    }
}

/* The stick's blues (not its handle's white, nor its tip's chrome) as greys, POGO_METAL of their brightness */
static void stick_metal(const PogoCtx *c, MadeFace *f) {
    s32 m, q, tip = 1;

    for (q = 0; q < 3; q++) {
        const V3 *p = &c->ref[c->stick.corner_of[f->v[q]]];

        tip &= fabsf(p->x) < TIP_HALF && p->y > TIP_TOP;
    }
    for (m = 0; m < 2 && !tip; m++) {
        for (q = 0; q < 3; q++) {
            u8 *rgb = f->rgb[m][q];
            s32 v = rgb[0] > rgb[1] ? (rgb[0] > rgb[2] ? rgb[0] : rgb[2]) : (rgb[1] > rgb[2] ? rgb[1] : rgb[2]);

            if (rgb[2] >= rgb[0] + 8) {
                rgb[0] = rgb[1] = rgb[2] = (u8)(v * POGO_METAL);
            }
        }
    }
}

/* His pogo model: him (his faces and images, on his own vertices) on Crash's stick; 0 if there's no stick or it couldn't */
static s32 pogo_make(const FootCtx *foot, const MadeFace *faces, s32 nfaces, const MadeImage *images, s32 nimages, HeroMade *out) {
    PogoCtx *c = &Pogo;
    MadeFace *all = NULL;
    MadeImage *imgs = NULL;
    MadeModel mm;
    V3 g[2] = { { 0, 0, 0 }, { 0, 0, 0 } }, pg[2] = { { 0, 0, 0 }, { 0, 0, 0 } }, pm[2] = { { 0, 0, 0 }, { 0, 0, 0 } };
    s32 ng[2] = { 0, 0 }, npg[2] = { 0, 0 }, npm[2] = { 0, 0 }, n = Verts + Copies + CapVerts, i, k, ok = 0;
    float top = 0, sole = -1e9f;

    memset(c, 0, sizeof(*c));
    /* all of Crash's model first, for where its corners are as he first bounces, then its stick by those */
    {
        GamePart whole;
        StickPick pick;
        s32 got;

        if (!hm_game_part(DB_CharsPogoCrash, keep_all, NULL, &whole)) {
            printf("bonus: Pogo's Crash isn't in the archive\n");
            return 0;
        }
        got = whole.corners <= POGO_CORNERS && (i = hm_model_anim(&whole, PogoNames[0], NULL)) >= 0 && hm_model_at(&whole, i, 0, c->ref);
        hm_part_free(&whole);
        pick.ref = c->ref;
        pick.at = 0;
        if (!got || !hm_game_part(DB_CharsPogoCrash, keep_stick, &pick, &c->stick)) {
            printf("bonus: Pogo's Crash has no stick\n");
            return 0;
        }
    }
    if (c->stick.corners > POGO_CORNERS) {
        goto done;
    }
    for (i = 0; i < POGO_MOVES; i++) {
        if ((c->anim[i] = hm_model_anim(&c->stick, PogoNames[i], &c->anims[i].frames)) < 0) {
            goto done;
        }
        c->anims[i].name = PogoNames[i];
        c->anims[i].mesh = 0;
        c->anims[i].step = 1;
    }
    if (!hm_model_at(&c->stick, c->anim[0], 0, c->ref)) {
        goto done;
    }
    /* the stick's frame, grips and pegs, and Crash's hips, chest and shoulders */
    c->peg_y = 0;
    for (k = 0; k < c->stick.corners; k++) {
        V3 p = c->ref[k];

        if (c->stick.in_part[k]) {
            if (!(fabsf(p.x) < TIP_HALF && p.y > TIP_TOP) && c->nframe < POGO_PART_MAX) {
                c->frame[c->nframe++] = (s16)k;
            }
            if (p.y >= GRIP_TOP && p.y <= GRIP_LOW && p.z >= GRIP_FRONT && p.z <= GRIP_BACK && fabsf(p.x) > 10) {
                g[p.x > 0] = vadd(g[p.x > 0], p);
                ng[p.x > 0]++;
            }
            if (p.y > STICK_LOW && fabsf(p.x) > PEG_ASIDE) {
                pg[p.x > 0] = vadd(pg[p.x > 0], p);
                npg[p.x > 0]++;
                c->peg_y = c->peg_y == 0 || p.y < c->peg_y ? p.y : c->peg_y;
            }
            continue;
        }
        if (p.y > POGO_HIPS_TOP && p.y < POGO_HIPS_LOW && fabsf(p.x) < POGO_MIDDLE && c->nhips < POGO_PART_MAX) {
            c->hips[c->nhips++] = (s16)k;
        }
        if (p.y > POGO_CHEST_TOP && p.y < POGO_CHEST_LOW && fabsf(p.x) < POGO_CHEST_HALF && p.z > POGO_CHEST_FRONT
            && c->nchest < POGO_PART_MAX) {
            c->chest[c->nchest++] = (s16)k;
        }
        for (i = 0; i < 2; i++) {
            if (vlen(vsub(p, PogoShoulders[i])) < POGO_SHOULDER_R && c->nshoulder[i] < POGO_PART_MAX) {
                c->shoulder[i][c->nshoulder[i]++] = (s16)k;
            }
            if (vlen(vsub(p, PogoElbows[i])) < POGO_ELBOW_R && c->nelbow[i] < POGO_PART_MAX) {
                c->elbow[i][c->nelbow[i]++] = (s16)k;
            }
        }
        if (p.y > POGO_FEET_TOP && fabsf(p.x) > POGO_MIDDLE && c->nfeet[p.x > 0] < POGO_PART_MAX) {
            c->feet[p.x > 0][c->nfeet[p.x > 0]++] = (s16)k;
        }
        top = p.y < top ? p.y : top;
        sole = p.y > sole ? p.y : sole;
    }
    /* his hands, at the grips */
    for (i = 0; i < 2; i++) {
        c->grip_at[i] = ng[i] > 0 ? vmul(g[i], 1.0f / ng[i]) : g[i];
    }
    for (k = 0; k < c->stick.corners; k++) {
        V3 p = c->ref[k];

        for (i = 0; i < 2 && !c->stick.in_part[k]; i++) {
            if (vlen(vsub(p, c->grip_at[i])) < POGO_HAND_R && p.z < POGO_HAND_BACK && c->nhand[i] < POGO_PART_MAX) {
                c->hand[i][c->nhand[i]++] = (s16)k;
            }
        }
    }
    if (c->nframe < 8 || c->nhips < 4 || c->nchest < 4 || c->nshoulder[0] < 2 || c->nshoulder[1] < 2 || c->nfeet[0] < 2
        || c->nfeet[1] < 2 || c->nelbow[0] < 2 || c->nelbow[1] < 2 || c->nhand[0] < 2 || c->nhand[1] < 2 || ng[0] == 0 || ng[1] == 0
        || npg[0] == 0 || npg[1] == 0 || sole <= top) {
        printf("bonus: Pogo's Crash isn't on his stick as he was (%d %d %d %d %d %d %d %d %d %d %d, grips %d %d, pegs %d %d)\n",
               c->nframe, c->nhips, c->nchest, c->nshoulder[0], c->nshoulder[1], c->nfeet[0], c->nfeet[1], c->nelbow[0], c->nelbow[1],
               c->nhand[0], c->nhand[1], ng[0], ng[1], npg[0], npg[1]);
        goto done;
    }
    for (i = 0; i < 2; i++) {
        c->hand_off[i] = vlen(vsub(mid_of(c->ref, c->hand[i], c->nhand[i]), c->grip_at[i]));
    }
    c->follow = POGO_SCALE * PIN_HEIGHT / (sole - top);
    body_lie(c, c->ref, c->was);
    /* him on it: his rig's floor point between its pegs, on their top */
    c->k = PIN_UNIT * foot->k * POGO_SCALE;
    pg[0] = vmul(pg[0], 1.0f / npg[0]);
    pg[1] = vmul(pg[1], 1.0f / npg[1]);
    c->origin = v3(0, c->peg_y, (pg[0].z + pg[1].z) / 2);
    for (k = 0; k < Verts; k++) {
        if (HandOf[k] != 0) {
            pm[HandOf[k] == 1] = vadd(pm[HandOf[k] == 1], Rest[k]);
            npm[HandOf[k] == 1]++;
        }
    }
    for (i = 0; i < 2; i++) {
        V3 grip = c->grip_at[i];

        grip.y = c->peg_y + (grip.y - c->peg_y) * POGO_TUBE;
        c->grip[i] = stick_in_rig(c, grip);
        c->palm[i] = npm[i] > 0 ? vmul(pm[i], 1.0f / npm[i]) : Arms[i].joint[2];
        c->ankle[i] = v3(stick_in_rig(c, pg[i]).x, Legs[i].joint[2].y, Legs[i].joint[2].z);
    }
    /* his faces and images, then the stick's */
    all = malloc(sizeof(MadeFace) * (size_t)(nfaces + c->stick.nfaces));
    imgs = malloc(sizeof(MadeImage) * (size_t)(nimages + c->stick.nimages + 1));
    if (all == NULL || imgs == NULL || nfaces + c->stick.nfaces > MADE_MAX_FACES) {
        goto done;
    }
    memcpy(all, faces, sizeof(MadeFace) * (size_t)nfaces);
    for (k = 0; k < c->stick.nfaces; k++) {
        MadeFace *f = &all[nfaces + k];

        *f = c->stick.faces[k];
        stick_metal(c, f);
        for (i = 0; i < 3; i++) {
            f->v[i] += n;
        }
        f->image = f->image >= 0 ? f->image + nimages : -1;
    }
    memcpy(imgs, images, sizeof(MadeImage) * (size_t)nimages);
    memcpy(imgs + nimages, c->stick.images, sizeof(MadeImage) * (size_t)c->stick.nimages);
    memset(&mm, 0, sizeof(mm));
    mm.faces = all;
    mm.nfaces = nfaces + c->stick.nfaces;
    mm.meshes = 1;
    mm.images = imgs;
    mm.nimages = nimages + c->stick.nimages;
    mm.anims = c->anims;
    mm.nanims = POGO_MOVES;
    mm.nvert = n + c->stick.nvert;
    mm.rest_anim = 0;
    mm.frame = pogo_frame;
    mm.ctx = c;
    ok = made_write(&mm, &out->tex[KIND_POGO], &out->tex_size[KIND_POGO], &out->mdl[KIND_POGO], &out->mdl_size[KIND_POGO]);
    printf("bonus: Pinstripe on Crash's pogo stick, its %d faces, %s\n", c->stick.nfaces, ok ? "made" : "not made");
done:
    free(all);
    free(imgs);
    hm_part_free(&c->stick);
    return ok;
}

/* --- his sounds and card ---
 * His boss fight's object (PinsC) plays his laugh (laf8A) and his tommy gun (MGn8A), at 11025 Hz. His game has no hurt voice or name
 * for him: his yelp is his laugh's first "ha" (PIN_YELP_BYTES, to where it falls quiet), higher, and his name as he wins its first
 * three (PIN_NAME_BYTES), done before the announcer's "wins!" (0.77 s on at the soonest). His landing is Crash's thud in that level
 * (ld10A), and his throw Crash's in Crash Bash (sfx/vox_crate/crash.sfx). */
#define PIN_PITCH 0x400
#define PIN_YELP_BYTES (40 * 16)
#define PIN_YELP_PITCH 0x480
#define PIN_NAME_BYTES (170 * 16)
#define CRASH_CRATE_VOICES 0xD4
#define CRASH_THROW 0x10
#define CARD_W 64
#define CARD_H 32
#define CARD_ASPECT 0.625f /* its pixels as wide as his game shows them against their height, on its 512 x 240 screen */
static u8 *PinSound[NUM_VOICES];
static s32 PinSoundBytes[NUM_VOICES];
static HeroSound PinThrow;
static u32 PinCard[CARD_W * CARD_H];

static void make_sounds(const C1Level *l, HeroMade *out) {
    static const struct {
        s32 voice;
        const char *entry;
    } Sounds[] = { { VOICE_LAUGH, "laf8A" }, { VOICE_FIRE, "MGn8A" }, { VOICE_THUD, "ld10A" } };
    s32 i, size;

    for (i = 0; i < (s32)(sizeof(Sounds) / sizeof(Sounds[0])); i++) {
        s32 v = Sounds[i].voice;

        if (PinSound[v] == NULL) {
            size = 0;
            PinSound[v] = c1_sound(l, Sounds[i].entry, &size);
            PinSoundBytes[v] = PinSound[v] != NULL ? size : 0;
        }
        out->sound[v].adpcm = PinSound[v];
        out->sound[v].bytes = PinSoundBytes[v];
        out->sound[v].pitch = PIN_PITCH;
    }
    out->sound[VOICE_NAME] = out->sound[VOICE_LAUGH];
    out->sound[VOICE_NAME].bytes = out->sound[VOICE_LAUGH].bytes < PIN_NAME_BYTES ? out->sound[VOICE_LAUGH].bytes : PIN_NAME_BYTES;
    out->sound[VOICE_YELP] = out->sound[VOICE_LAUGH];
    out->sound[VOICE_YELP].bytes = out->sound[VOICE_LAUGH].bytes < PIN_YELP_BYTES ? out->sound[VOICE_LAUGH].bytes : PIN_YELP_BYTES;
    out->sound[VOICE_YELP].pitch = PIN_YELP_PITCH;
    if (PinThrow.adpcm == NULL) {
        hm_game_sound(CRASH_CRATE_VOICES, CRASH_THROW, &PinThrow);
    }
    out->sound[VOICE_THROW] = PinThrow;
}

/* --- making him --- */

static s32 pin_make(HeroMade *out) {
    static V3 verts[MAX_VERTS], normals[MAX_VERTS];
    static MadeFace faces[MAX_TRIS];
    static MadeImage images[MADE_MAX_IMAGES];
    static MadeAnim made_anims[PIN_ANIMS];
    static C1Entry anims[PIN_ANIMS], rest_anim;
    DiscImage *disc;
    C1Level level;
    C1Entry model;
    FootCtx foot;
    MadeModel mm;
    s32 nvert, ntri = 0, nimage = 0, i, j, k, ok = 0;
    float lo = 1e9f, hi = -1e9f;

    disc = disc_image_find(HeroPinstripe.disc);
    if (disc == NULL) {
        return 0;
    }
    if (!c1_level_read(disc, PIN_LEVEL, &level)) {
        printf("bonus: Crash Bandicoot's Pinstripe level can't be read\n");
        disc_image_close(disc);
        return 0;
    }
    disc_image_close(disc);
    if (!c1_entry_find(&level, "Pin1G", &model) || model.type != 2 || model.count < 2 || !c1_entry_find(&level, "PinSV", &rest_anim)
        || rest_anim.type != 1 || rest_anim.count <= 0) {
        printf("bonus: Crash Bandicoot's disc has no Pinstripe\n");
        goto done;
    }
    for (i = 0; i < PIN_ANIMS; i++) {
        Src[i] = NULL;
        if (PinAnims[i].src != NULL) {
            if (!c1_entry_find(&level, PinAnims[i].src, &anims[i]) || anims[i].type != 1 || anims[i].count <= 0) {
                printf("bonus: Pinstripe's %s is missing\n", PinAnims[i].src);
                goto done;
            }
            Src[i] = &anims[i];
        }
        Frames[i] = PinAnims[i].base != NULL ? hm_bank_frames(DB_CharsCrateCortex, PinAnims[i].base) : 0;
        Frames[i] = (Frames[i] > 0 ? Frames[i] : PinAnims[i].frames) * PIN_SUBFRAMES;
        Frames[i] = Frames[i] > MADE_MAX_FRAMES ? MADE_MAX_FRAMES : Frames[i];
    }
    c1_model_scale(&model, Scale);
    nvert = c1_frame_verts(rest_anim.item[0], rest_anim.size[0], Scale, verts, normals, MAX_VERTS);
    if (nvert == 0 || nvert > MAX_VERTS - 128) {
        goto done;
    }
    Verts = nvert;
    for (k = 0; k < nvert; k++) {
        Rest[k] = to_rig(verts[k]);
        lo = verts[k].y < lo ? verts[k].y : lo;
        hi = verts[k].y > hi ? verts[k].y : hi;
    }
    if (!rig_setup(&model, nvert)) {
        printf("bonus: Pinstripe can't be rigged\n");
        goto done;
    }
    eye_lift();
    /* as tall as PIN_HEIGHT from his feet to the top of his head (y down: hi his feet) */
    foot.k = PIN_HEIGHT / (hi - lo);
    foot.ground = hi * foot.k;

    /* his triangles, their colours lit as he stands, and their textures; the gun's and the flash's on their own copies */
    if (!c1_model_faces(&level, &model, normals, nvert, faces, MAX_TRIS, &ntri, images, &nimage, &PinLight)) {
        goto done;
    }
    for (i = 0; i < ntri && i < MAX_TRIS; i++) {
        if (!GunTri[i]) {
            continue;
        }
        for (j = 0; j < 3; j++) {
            s32 v = faces[i].v[j];

            if (CopyOf[v] >= 0) {
                faces[i].v[j] = CopyOf[v];
            }
        }
    }
    add_caps(faces, &ntri);
    eye_paint(faces, ntri, images, &nimage);
    for (j = 0; j < PIN_ANIMS; j++) {
        made_anims[j].name = PinAnims[j].name;
        made_anims[j].frames = Frames[j];
        made_anims[j].mesh = 0;
        made_anims[j].step = PIN_SUBFRAMES;
    }
    memset(&mm, 0, sizeof(mm));
    mm.faces = faces;
    mm.nfaces = ntri;
    mm.meshes = 1;
    mm.images = images;
    mm.nimages = nimage;
    mm.anims = made_anims;
    mm.nanims = PIN_ANIMS;
    mm.nvert = nvert + Copies + CapVerts;
    mm.rest_anim = A_BREATHE;
    mm.frame = foot_frame;
    mm.ctx = &foot;
    if (!made_write(&mm, &out->tex[KIND_FOOT], &out->tex_size[KIND_FOOT], &out->mdl[KIND_FOOT], &out->mdl_size[KIND_FOOT])) {
        goto done;
    }
    if (!ride_bear(&foot, faces, ntri, images, nimage, out)) {
        printf("bonus: Pinstripe has no polar bear to ride\n");
    }
    if (!pogo_make(&foot, faces, ntri, images, nimage, out)) {
        printf("bonus: Pinstripe has no pogo stick\n");
    }
    if (c1_sprite(&level, "ShadC", 5, CARD_W, CARD_H, PinCard)) {
        out->card = PinCard;
        out->card_w = CARD_W;
        out->card_h = CARD_H;
        out->card_head = CARD_H;
        out->card_aspect = CARD_ASPECT;
    }
    make_sounds(&level, out);
    printf("bonus: Pinstripe from Crash Bandicoot, %d triangles, %d textures, %d bytes, the gun on %d corners of its own, %d palms closed"
           "%s%s%s\n", ntri, nimage, out->mdl_size[KIND_FOOT], Copies, NCaps, out->card != NULL ? ", his hero card" : "",
           out->sound[VOICE_LAUGH].adpcm != NULL ? ", his laugh" : "", out->sound[VOICE_FIRE].adpcm != NULL ? ", his tommy gun" : "");
    ok = 1;
done:
    made_images_free(images, nimage);
    free(level.pages);
    return ok;
}
