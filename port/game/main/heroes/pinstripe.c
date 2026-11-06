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
 * each leg two and its shoe) hangs them at his sides as he stands, swings them as he walks and puts them up for a crate. His walk,
 * jump, holding and throwing a crate, and his breathing are made by it. His attack is his stride's fire (PiG3V), the gun coming out
 * into his hands and away again, its muzzle flashing; he's thrown back as he's hit (PiF1V) and knocked down (PiF4V), laughs crouched
 * as he taunts (PiG2V, his arms down), and as he wins shoots his gun about laughing, as in that fight (PiG2V, the gun out), each of
 * those at his still's size (PIN_SOURCE_SCALE). His sounds are his boss fight's: his laugh and his tommy gun. His hero card is the
 * face that fight shows by his health. */
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
 * His base is Cortex, if nobody else is playing him: a villain as he is, and about his size. On foot he walks where the others run,
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

/* Each kind of game plays as on foot until he has a model of it */
const HeroDef HeroPinstripe = {
    "PINSTRIPE", "pinstripe", "SCUS-94900", Cortex,
    { { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES }, { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES },
      { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES }, { PinStands, "BREATHE", 0x1000, 0, PIN_SUBFRAMES },
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
    return Arms[0].n > 20 && Arms[1].n > 20 && Legs[0].n > 20 && Legs[1].n > 20 && ChestN >= 4;
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

/* His upper body (all above his waist, blended over WAIST_BLEND): leaned forward by `lean` radians about his waist and lifted `lift`;
 * and his chest risen `breath` more (as he breathes), with his arms and hands */
static void pose_body(V3 *r, float lean, float lift, float breath) {
    V3 waist = { 0, WAIST_Y, 0 };
    s32 k;

    for (k = 0; k < Verts; k++) {
        float w;

        if (PieceOf[k] == PIECE_LEGS || PieceOf[k] == PIECE_SHOE) {
            continue;
        }
        w = hm_smooth(WAIST_Y - WAIST_BLEND, WAIST_Y + WAIST_BLEND, Rest[k].y);
        r[k] = hm_turn_x(r[k], waist, -lean * w);
        r[k].y += lift * w + breath * hm_smooth(WAIST_Y + 25, 200, Rest[k].y);
    }
}

/* All of him moved by d */
static void pose_shift(V3 *r, V3 d) {
    s32 k;

    for (k = 0; k < Verts; k++) {
        r[k] = vadd(r[k], d);
    }
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

/* Standing as he breathes, arms down (made): at t of the way round */
static void stand(V3 *r, float t, const ArmPose *arms) {
    memcpy(r, Rest, sizeof(V3) * Verts);
    pose_arms(r, arms, arms, 0, 0.04f * sinf(2 * PIN_PI * t), 0, NULL);
    pose_body(r, 0.015f * sinf(2 * PIN_PI * t), 0, 1.2f * sinf(2 * PIN_PI * t));
}

/* His walk, two steps a go round (made): legs swinging WALK_HIP forward and back, the one coming forward bent WALK_KNEE at the knee,
 * the body sinking as the legs spread so the foot on the ground stays there, leaning WALK_LEAN forward, arms swinging WALK_ARM against
 * the legs (or up, holding a crate) */
#define WALK_HIP 0.42f
#define WALK_KNEE 0.75f
#define WALK_LEAN 0.06f
#define WALK_ARM 0.32f
#define WALK_BOB 3.0f
static void walk(V3 *r, float t, s32 holding) {
    float a = 2 * PIN_PI * t, leg_len = Legs[0].joint[0].y - Legs[0].joint[2].y;
    s32 i;

    memcpy(r, Rest, sizeof(V3) * Verts);
    if (holding) {
        pose_arms(r, &ArmsUp, &ArmsUp, 0, 0, 0, NULL);
    } else {
        pose_arms(r, &ArmsDown, &ArmsDown, 0, -WALK_ARM * sinf(a), 0, NULL);
    }
    pose_body(r, WALK_LEAN, -WALK_BOB * sinf(a) * sinf(a), 0);
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
static void from_game(V3 *r, s32 j, s32 f, s32 frames, s32 arms_down) {
    const C1Entry *a = Src[j];
    float t = frames > 1 ? (float)f * (a->count - 1) / (frames - 1) : 0;
    Moved chest;

    src_at(a, t, r);
    if (arms_down) {
        chest_move(r, &chest);
        pose_arms(r, &ArmsDown, &ArmsDown, 0, 0, 0, &chest);
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
        from_game(r, j, f, n, 0);
        break;
    case A_WIN:
        win(r, f);
        return; /* its own gun */
    case A_TAUNT:
        from_game(r, j, f, n, 1);
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
    for (k = 0; k < Verts + Copies + CapVerts; k++) {
        V3 p = from_rig(r[k]);

        out[k].x = p.x * c->k;
        out[k].y = p.y * c->k - c->ground;
        out[k].z = p.z * c->k;
    }
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
    /* as tall as PIN_HEIGHT from his feet to the top of his head (y down: hi his feet) */
    foot.k = PIN_HEIGHT / (hi - lo);
    foot.ground = hi * foot.k;

    /* his triangles, their colours lit as he stands, and their textures; the gun's and the flash's on their own copies */
    if (!c1_model_faces(&level, &model, normals, nvert, faces, MAX_TRIS, &ntri, images, &nimage)) {
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
