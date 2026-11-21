/* A hero pack's models of the kinds where the game's heroes ride or hold something (Pogo, Ballistix, Tank, Crash Dash, Splash Dash,
 * the mallet, the dino and the kegs games; docs/HERO_PACKS.md): Crash's model of that kind followed frame by frame, as Pinstripe's were
 * made (heroes/pinstripe.c, now in the git history, whose tables of where Crash and his prop are in each kind's model are copied
 * here), but with the pack's own skeleton:
 * - Crash measured in each frame: his hips, chest and shoulders (his body's turn), each arm's shoulder, elbow and hand, his feet, and
 *   where he rides astride his legs' hips, knees and ankles, all in the prop's own frame;
 * - the pack's hips on his, its trunk turned as his is, each arm reaching (two-bone) for his hand (on the grip, where the kind has grips;
 *   else where his is from his shoulder, for its size), its legs as his where he rides astride, its feet on the pegs or the floor where he
 *   stands, else sat; and in a tank's hatch or a race cart all of it below its hips hidden;
 * - the prop (the stick, the car's handles, the cart, the fish, the mallet, the dino, the keg) from Crash's model, as it moves with him,
 *   or with the pack's hand where he carries it.
 * The moves are Crash's of that kind, a frame a tick. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "bash/main.h"
#include "bash/modelbank.h"
#include "bash/herodef.h"
#include "bash/heromake.h"
#include "bash/packtemplate.h"
#include "../../platform/heropack.h"

#define FOLLOW_PART_MAX 128
#define FOLLOW_CORNERS 1024
#define FOLLOW_MOVES 8
#define FOLLOW_CUT 122.0f   /* (Pinstripe's: a kind's tables say there's a cut, in a tank's hatch and a race cart) */
#define GUNMETAL 0x80746E   /* what a pack hero rides painted, as Pinstripe's is: a blue-grey from its colours */
#define PF_MAX_VERTS 4096

static V3 v3(float x, float y, float z) {
    V3 v;

    v.x = x, v.y = y, v.z = z;
    return v;
}
static V3 vadd(V3 a, V3 b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
static V3 vsub(V3 a, V3 b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }
static V3 vmul(V3 a, float s) { return v3(a.x * s, a.y * s, a.z * s); }
static float vdot(V3 a, V3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static V3 vcross(V3 a, V3 b) { return v3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static float vlen(V3 a) { return sqrtf(vdot(a, a)); }
static V3 vnorm(V3 a) {
    float l = vlen(a);

    return l > 1e-6f ? vmul(a, 1 / l) : v3(0, 1, 0);
}

typedef struct Follow Follow;
typedef struct {
    V3 hip[2], knee[2], ankle[2];
    float r;
} FollowLegs;
typedef struct {
    s32 bank;                /* Crash's model of the kind, */
    s32 kind;                /* the kind (KIND_), */
    const char *what;        /* and what it's called in the log */
    const char *const *moves;
    s32 nmoves;
    /* the prop: a strip of Crash's model that's the prop's, by its corners as he first moves (at) or its place in the model (strip, from
     * 0); NULL: none, he holds nothing */
    s32 (*prop)(const V3 *at, s32 n, s32 textured, s32 strip);
    s32 (*frame)(V3 p);      /* of the prop's corners, those of its frame, fitted each frame; NULL: the model's own frame */
    s32 (*grip)(V3 p);       /* its grips' (each side's by its x); NULL: none, his arms as Crash's */
    s32 (*peg)(V3 p);        /* its pegs', under his feet; NULL as he sits */
    s32 (*hips)(V3 p);       /* Crash's corners of his hips, */
    s32 (*chest)(V3 p);      /* his chest, */
    s32 (*foot)(V3 p);       /* and his feet (each side's by its x; NULL as he sits) */
    V3 shoulder[2], elbow[2]; /* his joints as he first moves, his left first, */
    float shoulder_r, elbow_r; /* how far round them their corners are, */
    float hand_r, hand_back; /* and his hands: his corners within hand_r of a grip (or of hand, without grips), in front of hand_back (z) */
    V3 hand[2];
    float scale;             /* his size on it */
    float lean;              /* his body leant forward over it, radians */
    float squat;             /* his hips lower than he stands (his rig's units), standing */
    float tube;              /* the prop's length above its pegs against Crash's (1 as it is) */
    float grip_in;           /* his hand's hole this far out along a grip's bar from its grip's middle (Crash's fist covers its end) */
    float cut;               /* all of him below this in his rig hidden (in a tank's hatch, a race cart), or 0 */
    s32 across_chest;        /* his body's line across from his chest's two halves, not his shoulders (which go with his arms) */
    void (*paint)(Follow *c); /* the prop's colours (its faces' and images') made his, or NULL */
    s32 stretch;             /* his body stretched up and squashed down as Crash's is (his chest from his hips) */
    const FollowLegs *legs;  /* his legs as Crash's go, from where Crash's are (riding astride); NULL: on its pegs, or sat */
    s32 reach;               /* his hands where Crash's are from his shoulders (for his size), his arms stretching as far as Crash's do,
                              * not only pointing Crash's way */
    s32 stand;               /* stood on the floor as Crash stands, his feet as far off their places as Crash's go (foot) */
    s32 carry;               /* the prop in his hand (1 his left, 2 his right) and moved with it, as big against him as against Crash:
                              * its handle (its grip corners' line) through his hand's hole, or, without grips, where it is against
                              * Crash's hand (held round it); 0 none */
    s32 mesh;                /* the mesh of Crash's model his moves move (the kegs game's: the second, with his keg) */
    s32 carry_kept;          /* what he carries kept Crash's size (a keg, as big as the game's others), where it'd be for his size
                              * against his shoulder, his hand reaching round it as Crash's does */
    s32 lit;                 /* its LIGHT on a second mesh, charred (Polar Push's lightning, as ride_make's bear has it) */
} FollowKind;

struct Follow {
    const FollowKind *kind;
    GamePart prop;
    s32 anim[FOLLOW_MOVES];
    V3 ref[FOLLOW_CORNERS];
    s16 frame[FOLLOW_PART_MAX], hips[FOLLOW_PART_MAX], chest[FOLLOW_PART_MAX], half[2][FOLLOW_PART_MAX], nhalf[2], shoulder[2][FOLLOW_PART_MAX];
    s16 feet[2][FOLLOW_PART_MAX], elbow[2][FOLLOW_PART_MAX], hand[2][FOLLOW_PART_MAX], grips[2][FOLLOW_PART_MAX];
    s16 leg[3][2][FOLLOW_PART_MAX];
    s32 nleg[3][2];
    s32 nframe, nhips, nchest, nshoulder[2], nfeet[2], nelbow[2], nhand[2], ngrips[2];
    MadeAnim anims[FOLLOW_MOVES];
    float was[3][3];
};

typedef struct {
    const FollowKind *kind;
    const V3 *ref;
    s32 at, strip;
} PropPick;

static s32 keep_all(void *ctx, const V3 *rest, s32 n, s32 textured) {
    (void)ctx, (void)rest, (void)n, (void)textured;
    return 1;
}

static s32 keep_prop(void *ctx, const V3 *rest, s32 n, s32 textured) {
    PropPick *pick = ctx;
    const V3 *p = pick->ref + pick->at;

    (void)rest;
    pick->at += n;
    return pick->kind->prop(p, n, textured, pick->strip++);
}

static V3 mid_of(const V3 *at, const s16 *ids, s32 n) {
    V3 c = { 0, 0, 0 };
    s32 i;

    for (i = 0; i < n; i++) {
        c = vadd(c, at[ids[i]]);
    }
    return n > 0 ? vmul(c, 1.0f / n) : c;
}

/* How Crash's body lies, his corners at `at`: a frame (its columns across, up and forward) of his spine, hips to chest, and his shoulders'
 * (or his chest's halves') line across it */
static void body_lie(const Follow *c, const V3 *at, float m[3][3]) {
    V3 up = vnorm(vsub(mid_of(at, c->chest, c->nchest), mid_of(at, c->hips, c->nhips)));
    V3 across = c->kind->across_chest ? vsub(mid_of(at, c->half[1], c->nhalf[1]), mid_of(at, c->half[0], c->nhalf[0]))
                                      : vsub(mid_of(at, c->shoulder[1], c->nshoulder[1]), mid_of(at, c->shoulder[0], c->nshoulder[0]));
    V3 fwd;

    across = vnorm(vsub(across, vmul(up, vdot(across, up))));
    fwd = vcross(across, up);
    m[0][0] = across.x, m[0][1] = up.x, m[0][2] = fwd.x;
    m[1][0] = across.y, m[1][1] = up.y, m[1][2] = fwd.y;
    m[2][0] = across.z, m[2][1] = up.z, m[2][2] = fwd.z;
}

/* Where a two-bone limb's middle joint goes, from root reaching for target (bones l1 then l2), bent toward hint */
static V3 two_bone(V3 root, V3 target, float l1, float l2, V3 hint) {
    V3 d = vsub(target, root), n;
    float dist = vlen(d), a, h;

    dist = dist < fabsf(l1 - l2) + 0.01f ? fabsf(l1 - l2) + 0.01f : dist > l1 + l2 - 0.01f ? l1 + l2 - 0.01f : dist;
    d = vnorm(d);
    a = (l1 * l1 - l2 * l2 + dist * dist) / (2 * dist);
    h = sqrtf(fmaxf(0, l1 * l1 - a * a));
    n = vsub(hint, vmul(d, vdot(hint, d)));
    n = vlen(n) > 1e-4f ? vnorm(n) : v3(0, 0, -1);
    return vadd(vadd(root, vmul(d, a)), vmul(n, h));
}

#define STICK_LOW -76.0f
#define GRIP_TOP -276.0f
#define GRIP_LOW -230.0f
#define GRIP_BACK -60.0f
#define GRIP_FRONT -105.0f
#define GRIP_HALF 76.0f
#define TIP_HALF 15.0f
#define TIP_TOP -45.0f
#define PEG_ASIDE 30.0f
#define POGO_HIPS_TOP -190.0f
#define POGO_HIPS_LOW -155.0f
#define POGO_MIDDLE 30.0f
#define POGO_CHEST_TOP -330.0f
#define POGO_CHEST_LOW -255.0f
#define POGO_CHEST_HALF 40.0f
#define POGO_CHEST_FRONT -80.0f
#define POGO_FEET_TOP -100.0f
#define POGO_HAND_R 45.0f
#define POGO_HAND_BACK -55.0f
#define POGO_METAL 0.45f /* the stick's blues as greys this much of their brightness */
static const char *const PogoNames[] = { "BREATHE", "JUMP", "DAZED", "WIN", "TAZING" };

static s32 pogo_prop(const V3 *p, s32 n, s32 textured, s32 strip) {
    s32 k, low = 0, grip = 1;

    (void)strip;
    if (textured) {
        return 0;
    }
    for (k = 0; k < n; k++) {
        low |= p[k].y > STICK_LOW;
        grip &= p[k].y >= GRIP_TOP && p[k].y <= GRIP_LOW && p[k].z >= GRIP_FRONT && p[k].z <= GRIP_BACK && fabsf(p[k].x) <= GRIP_HALF;
    }
    return low || grip;
}
static s32 pogo_frame(V3 p) {
    return !(fabsf(p.x) < TIP_HALF && p.y > TIP_TOP);
}
static s32 pogo_grip(V3 p) {
    return p.y >= GRIP_TOP && p.y <= GRIP_LOW && p.z >= GRIP_FRONT && p.z <= GRIP_BACK && fabsf(p.x) > 10;
}
static s32 pogo_peg(V3 p) {
    return p.y > STICK_LOW && fabsf(p.x) > PEG_ASIDE;
}
static s32 pogo_hips(V3 p) {
    return p.y > POGO_HIPS_TOP && p.y < POGO_HIPS_LOW && fabsf(p.x) < POGO_MIDDLE;
}
static s32 pogo_chest(V3 p) {
    return p.y > POGO_CHEST_TOP && p.y < POGO_CHEST_LOW && fabsf(p.x) < POGO_CHEST_HALF && p.z > POGO_CHEST_FRONT;
}
static s32 pogo_foot(V3 p) {
    return p.y > POGO_FEET_TOP && fabsf(p.x) > POGO_MIDDLE;
}

/* The stick's blues (not its handle's white, nor its tip's chrome) as greys, POGO_METAL of their brightness */
static void stick_metal(Follow *c) {
    s32 i, m, q, tip;

    for (i = 0; i < c->prop.nfaces; i++) {
        MadeFace *f = &c->prop.faces[i];

        tip = 1;
        for (q = 0; q < 3; q++) {
            const V3 *p = &c->ref[c->prop.corner_of[f->v[q]]];

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
}

static const FollowKind PogoKind = {
    DB_CharsPogoCrash, KIND_POGO, "pogo stick", PogoNames, 5,
    pogo_prop, pogo_frame, pogo_grip, pogo_peg, pogo_hips, pogo_chest, pogo_foot,
    { { -50, -302, 10 }, { 50, -302, 10 } }, { { -126, -297, -25 }, { 126, -300, -27 } }, 20.0f, 25.0f,
    POGO_HAND_R, POGO_HAND_BACK, { { 0, 0, 0 }, { 0, 0, 0 } },
    0.8f, 0.25f, 12.0f, 1.3f, 0, 0, 0, stick_metal,
};

/* -- in a Ballistix car --
 * Ballistix draws a car round each hero (its vehicle model's meshes 4 and 1) and the hero's own model in it (models\chars\ball\), the
 * four of them round the arena, so each is seen from a side of its own. The car's handles are the heroes' models' own: two levers, one
 * each side (Crash's textured strips all CAR_LEVER_X or more out to the sides), a grip bar out from the top of each, which rock as he
 * steers (BANK). He holds their grip bars' inner ends (their corners no more than CAR_GRIP_X out, above CAR_GRIP_LOW), sat as Crash sits
 * (his hips on Crash's: Crash's corners below CAR_HIPS_TOP within CAR_MIDDLE across; his chest from CAR_CHEST_TOP to CAR_CHEST_LOW,
 * behind CAR_CHEST_BACK),
 * his legs folded in the car under him; he lets go with a hand as Crash does to fidget, wave and cheer. */
#define CAR_LEVER_X 210.0f
#define CAR_GRIP_X 260.0f
#define CAR_GRIP_LOW -165.0f
#define CAR_HIPS_TOP -150.0f
#define CAR_MIDDLE 50.0f
#define CAR_CHEST_TOP -250.0f
#define CAR_CHEST_LOW -160.0f
#define CAR_CHEST_BACK 50.0f /* behind his jaw */
#define CAR_HAND_IN 20.0f    /* his fist this far out along a grip bar from its inner end */
static const char *const CarNames[] = { "BREATHE", "BANK", "IDLE_A", "TAUNT_A", "DIE", "WIN" };

static s32 car_prop(const V3 *p, s32 n, s32 textured, s32 strip) {
    s32 k;

    (void)strip;
    for (k = 0; k < n; k++) {
        if (fabsf(p[k].x) < CAR_LEVER_X) {
            return 0;
        }
    }
    return textured;
}
static s32 car_grip(V3 p) {
    return fabsf(p.x) <= CAR_GRIP_X && p.y < CAR_GRIP_LOW;
}
static s32 car_hips(V3 p) {
    return p.y > CAR_HIPS_TOP && fabsf(p.x) < CAR_MIDDLE;
}
static s32 car_chest(V3 p) {
    return p.y > CAR_CHEST_TOP && p.y < CAR_CHEST_LOW && fabsf(p.x) < CAR_MIDDLE && p.z > CAR_CHEST_BACK;
}

static const FollowKind CarKind = {
    DB_CharsBallCrash, KIND_BALL, "car", CarNames, 6,
    car_prop, NULL, car_grip, NULL, car_hips, car_chest, NULL,
    { { -65, -226, 150 }, { 65, -226, 150 } }, { { -172, -208, 192 }, { 172, -208, 192 } }, 25.0f, 30.0f,
    70.0f, 1e9f, { { 0, 0, 0 }, { 0, 0, 0 } },
    1.0f, 0.1f, 0, 1, CAR_HAND_IN, 0, 0, NULL,
};

/* -- in a tank --
 * Tank draws each hero's tank round it (its hull and turret) and the hero's own model stood in the turret's hatch (models\chars\tank\),
 * as big as on foot: Crash's is him from the waist up and nothing else, his hands on the hatch's rim. He stands in it as Crash does:
 * his hips on Crash's (Crash's corners at the bottom, below TANK_HIPS_TOP within TANK_MIDDLE across, behind TANK_HIPS_FRONT: the
 * hatch; his hands rest on his belly in front, and a set with them rose out of the hatch as he raised his arms), his chest from
 * TANK_CHEST_TOP to TANK_CHEST_LOW behind TANK_CHEST_FRONT (not his face), his body and arms as Crash's, who drops mines over the side, is thrown back as he's hit, slumps
 * as he loses and cheers as he wins; and all of him below his hips (FOLLOW_CUT) hidden inside, as the others have nothing
 * below the hatch. */
#define TANK_HIPS_TOP -320.0f
#define TANK_HIPS_FRONT -30.0f
#define TANK_MIDDLE 40.0f
#define TANK_CHEST_TOP -410.0f
#define TANK_CHEST_LOW -330.0f
#define TANK_CHEST_FRONT -50.0f
static const char *const TankNames[] = { "BREATHE", "IDLE1", "MINE", "HIT", "DIE", "WIN" };

static s32 tank_hips(V3 p) {
    return p.y > TANK_HIPS_TOP && fabsf(p.x) < TANK_MIDDLE && p.z > TANK_HIPS_FRONT;
}
static s32 tank_chest(V3 p) {
    return p.y > TANK_CHEST_TOP && p.y < TANK_CHEST_LOW && fabsf(p.x) < TANK_MIDDLE && p.z > TANK_CHEST_FRONT;
}

static const FollowKind TankKind = {
    DB_CharsTankCrash, KIND_TANK, "tank", TankNames, 6,
    NULL, NULL, NULL, NULL, tank_hips, tank_chest, NULL,
    { { -52, -425, 20 }, { 52, -425, 20 } }, { { -88, -345, 15 }, { 88, -345, 15 } }, 30.0f, 25.0f,
    30.0f, 1e9f, { { -50, -312, -45 }, { 50, -312, -45 } },
    1.0f, 0, 0, 1, 0, FOLLOW_CUT, 1, NULL,
};

/* -- in a race cart (Crash Dash) --
 * On Crash Dash's tracks (but the water's) each hero's model is it in its cart (models\chars\race\: all the carts the same, each a
 * colour of its own, Crash's blue, Coco's pink, Cortex's yellow, Brio's green, Tiny's dark purple, the others orange, Ripper Roo's
 * red). Crash's is him from the waist up, sat in its cockpit, and the cart: its strips with a corner where he isn't (below his seat,
 * RACE_SEAT, or further out across than his elbows, RACE_OUT). Its frame is the cockpit's rim (its corners within RACE_RIM_HALF of
 * RACE_RIM_Y, but the jets' behind RACE_RIM_BACK), which goes forward and back as a boost stretches the cart and down as it drops off
 * the track. Crash holds the front of the cockpit by its dashboard, so he does: his hands RACE_HAND_IN in from the rim's front corners
 * (those further out than RACE_OUT, from RACE_GRIP_FRONT to RACE_GRIP_BACK along). He sits in Crash's seat (his hips on Crash's
 * bottom, Crash's corners below RACE_HIPS_TOP within RACE_MIDDLE across; his chest from RACE_CHEST_TOP to RACE_CHEST_LOW within
 * RACE_CHEST_HALF across, behind his jaw, RACE_CHEST_FRONT), as big as on foot as Cortex is in his, and all of him below his hips is
 * hidden in the cart (FOLLOW_CUT), as Crash's model has nothing there. His cart is gunmetal (GUNMETAL, from its blues, RACE_FROM, as
 * his pogo stick is), its jets' lights yellow as every cart's are. */
#define RACE_SEAT -64.0f
#define RACE_OUT 104.0f
#define RACE_RIM_Y -108.0f
#define RACE_RIM_HALF 4.0f
#define RACE_RIM_BACK 170.0f
#define RACE_GRIP_FRONT -60.0f
#define RACE_GRIP_BACK -30.0f
#define RACE_HAND_IN 52.0f
#define RACE_HIPS_TOP -80.0f
#define RACE_MIDDLE 30.0f
#define RACE_CHEST_TOP -235.0f
#define RACE_CHEST_LOW -140.0f
#define RACE_CHEST_HALF 46.0f
#define RACE_CHEST_FRONT -30.0f
#define RACE_FROM 220.0f
static const char *const RaceNames[] = { "BREATHE", "RUN", "HIT", "FALL", "WIN" };

static s32 race_prop(const V3 *p, s32 n, s32 textured, s32 strip) {
    s32 k;

    (void)textured;
    (void)strip;
    for (k = 0; k < n; k++) {
        if (p[k].y > RACE_SEAT || fabsf(p[k].x) > RACE_OUT) {
            return 1;
        }
    }
    return 0;
}
static s32 race_frame(V3 p) {
    return fabsf(p.y - RACE_RIM_Y) < RACE_RIM_HALF && p.z < RACE_RIM_BACK;
}
static s32 race_grip(V3 p) {
    return fabsf(p.y - RACE_RIM_Y) < RACE_RIM_HALF && fabsf(p.x) > RACE_OUT && p.z > RACE_GRIP_FRONT && p.z < RACE_GRIP_BACK;
}
static s32 race_hips(V3 p) {
    return p.y > RACE_HIPS_TOP && fabsf(p.x) < RACE_MIDDLE;
}
static s32 race_chest(V3 p) {
    return p.y > RACE_CHEST_TOP && p.y < RACE_CHEST_LOW && fabsf(p.x) < RACE_CHEST_HALF && p.z > RACE_CHEST_FRONT;
}

static void cart_gunmetal(Follow *c) {
    hm_paint(c->prop.faces, c->prop.nfaces, c->prop.images, c->prop.nimages, RACE_FROM, GUNMETAL);
}

static const FollowKind RaceKind = {
    DB_CharsRaceCrash, KIND_RACE, "race cart", RaceNames, 5,
    race_prop, race_frame, race_grip, NULL, race_hips, race_chest, NULL,
    { { -48, -205, 18 }, { 48, -205, 18 } }, { { -90, -115, 35 }, { 90, -115, 35 } }, 25.0f, 25.0f,
    80.0f, -5.0f, { { 0, 0, 0 }, { 0, 0, 0 } },
    1.0f, 0.1f, 0, 1, -RACE_HAND_IN, FOLLOW_CUT, 1, cart_gunmetal, 1,
};

/* -- riding a fish (Splash Dash) --
 * On Crash Dash's water track each hero's model is it riding a fish (models\chars\splash\: Crash's and Coco's a pink dolphin, Cortex's
 * and Brio's a grey shark, Tiny's and Kong's an orca, Dingodile's and Rilla Roo's a blue fish, Ripper Roo's red). Crash's dolphin is
 * his model's strips from FISH_STRIP on (it's after him in the model), painted gunmetal from its pinks (FISH_FROM), as his cart is.
 * Its frame is its middle (its corners from FISH_FRONT to FISH_BACK along), which bobs as it swims, leaps out of the water and flips
 * over as he wins, and spins round as it dives. Crash sits astride it, lies along it holding its head as it swims, and goes up and over
 * with it as it flips; Pinstripe goes as he does, all of him: his hips on Crash's (Crash's corners below FISH_HIPS_TOP within
 * FISH_MIDDLE across, behind FISH_HIPS_FRONT), his chest from FISH_CHEST_TOP to FISH_CHEST_LOW within FISH_CHEST_HALF across, behind
 * his jaw (FISH_CHEST_FRONT), his arms as Crash's (his hands by where they are) and his legs as Crash's (FishLegs), at 0.8 of his
 * size, as on the bear. */
#define FISH_STRIP 39
#define FISH_FRONT -120.0f
#define FISH_BACK 160.0f
#define FISH_FROM 315.0f
#define FISH_HIPS_TOP -130.0f
#define FISH_MIDDLE 40.0f
#define FISH_HIPS_FRONT 50.0f
#define FISH_CHEST_TOP -260.0f
#define FISH_CHEST_LOW -175.0f
#define FISH_CHEST_HALF 55.0f
#define FISH_CHEST_FRONT 30.0f
static const char *const FishNames[] = { "BREATHE", "SWIM", "TRANS", "WIN", "FALL" };
static const FollowLegs FishLegs = {
    { { -45, -100, 85 }, { 45, -100, 85 } }, { { -95, -60, 100 }, { 95, -60, 100 } }, { { -95, -8, 75 }, { 95, -8, 75 } }, 30.0f,
};

static s32 fish_prop(const V3 *p, s32 n, s32 textured, s32 strip) {
    (void)p;
    (void)n;
    (void)textured;
    return strip >= FISH_STRIP;
}
static s32 fish_frame(V3 p) {
    return p.z > FISH_FRONT && p.z < FISH_BACK;
}
static s32 fish_hips(V3 p) {
    return p.y > FISH_HIPS_TOP && fabsf(p.x) < FISH_MIDDLE && p.z > FISH_HIPS_FRONT;
}
static s32 fish_chest(V3 p) {
    return p.y > FISH_CHEST_TOP && p.y < FISH_CHEST_LOW && fabsf(p.x) < FISH_CHEST_HALF && p.z > FISH_CHEST_FRONT;
}

static void fish_gunmetal(Follow *c) {
    hm_paint(c->prop.faces, c->prop.nfaces, c->prop.images, c->prop.nimages, FISH_FROM, GUNMETAL);
}

static const FollowKind FishKind = {
    DB_CharsSplashCrash, KIND_SPLASH, "fish", FishNames, 5,
    fish_prop, fish_frame, NULL, NULL, fish_hips, fish_chest, NULL,
    { { -55, -250, 95 }, { 55, -250, 95 } }, { { -70, -160, 70 }, { 70, -160, 70 } }, 25.0f, 25.0f,
    45.0f, 20.0f, { { -47, -145, -33 }, { 45, -146, -26 } },
    0.8f, 0, 0, 1, 0, 0, 1, fish_gunmetal, 0, &FishLegs, 1, 0, 0,
};

/* -- with a mallet (Mallet Mash) --
 * Each hero's model in the mallet game is it with its big mallet (models\chars\mallet\: six moves, BREATHE, MEDIUM as it runs, HIT
 * knocked down, ATTACK the swing, WIN, and #4a7, which Crash's hasn't: his stand-in for it is his BREATHE). Crash carries his over his
 * shoulder in his left hand and swings it with that hand alone: the mallet is his model's strips from MALLET_STRIP on (after him in the
 * model), its handle its corners below its head (below MALLET_HEAD). Pinstripe stands, runs, swings, falls and cheers as Crash does,
 * as big as on foot: stood on the floor, his feet as far off their places as Crash's go (Crash's corners below MALLET_FEET_TOP more
 * than MALLET_MIDDLE aside), his hips Crash's (from MALLET_HIPS_TOP to MALLET_HIPS_LOW within MALLET_MIDDLE across), his chest from
 * MALLET_CHEST_TOP to MALLET_CHEST_LOW within MALLET_CHEST_HALF across, his hands where Crash's are (for his size), and his left round
 * the mallet's handle, which goes with it, as big against him as Crash's is against Crash. */
#define MALLET_STRIP 39
#define MALLET_HEAD -340.0f
#define MALLET_FEET_TOP -50.0f
#define MALLET_MIDDLE 30.0f
#define MALLET_HIPS_TOP -180.0f
#define MALLET_HIPS_LOW -115.0f
#define MALLET_CHEST_TOP -260.0f
#define MALLET_CHEST_LOW -215.0f
#define MALLET_CHEST_HALF 55.0f
static const char *const MalletNames[] = { "BREATHE", "MEDIUM", "ATTACK", "HIT", "WIN" };

static s32 mallet_prop(const V3 *p, s32 n, s32 textured, s32 strip) {
    (void)p;
    (void)n;
    (void)textured;
    return strip >= MALLET_STRIP;
}
static s32 mallet_grip(V3 p) {
    return p.y > MALLET_HEAD;
}
static s32 mallet_hips(V3 p) {
    return p.y > MALLET_HIPS_TOP && p.y < MALLET_HIPS_LOW && fabsf(p.x) < MALLET_MIDDLE;
}
static s32 mallet_chest(V3 p) {
    return p.y > MALLET_CHEST_TOP && p.y < MALLET_CHEST_LOW && fabsf(p.x) < MALLET_CHEST_HALF;
}
static s32 mallet_foot(V3 p) {
    return p.y > MALLET_FEET_TOP && fabsf(p.x) > MALLET_MIDDLE;
}

static const FollowKind MalletKind = {
    DB_CharsMalletCrash, KIND_MALLET, "mallet", MalletNames, 5,
    mallet_prop, NULL, mallet_grip, NULL, mallet_hips, mallet_chest, mallet_foot,
    { { -54, -292, 15 }, { 54, -292, 15 } }, { { -112, -222, -8 }, { 115, -225, 30 } }, 45.0f, 45.0f,
    45.0f, 1e9f, { { -160, -232, -85 }, { 115, -136, -15 } },
    1.0f, 0, 0, 1, 0, 0, 1, NULL, 0, NULL, 1, 1, 1,
};

/* -- riding a dino (the gem game) --
 * In the gem game each hero rides a small dino of its own colour (models\chars\dino\: Crash's blue, Coco's pink, Cortex's yellow, Brio's
 * teal, Tiny's dark purple, Kong's and Dingodile's red, Rilla Roo's orange, Ripper Roo's red). Crash's is his model's first
 * DINO_CREST strips (a crest down its middle) and all from DINO_STRIP on (its body, head, legs and tail), painted gunmetal from its
 * blues (DINO_FROM; its tan belly kept). Its frame is its back under him (its corners above DINO_BACK_LOW, from DINO_FRONT to DINO_BACK
 * along), which bobs as it runs, rears as it skids and jolts as it shoots. Crash sits astride it, stands up on it as it skids and leaps
 * up off it as he wins; Pinstripe does as he does, all of him, at 0.8 of his size as on the fish: his hips on Crash's (Crash's corners
 * from DINO_HIPS_TOP to DINO_HIPS_LOW within DINO_MIDDLE across, behind DINO_HIPS_FRONT), his chest from DINO_CHEST_TOP to
 * DINO_CHEST_LOW within DINO_CHEST_HALF, behind DINO_CHEST_FRONT, his hands where Crash's are and his legs as Crash's (DinoLegs). */
#define DINO_CREST 5
#define DINO_STRIP 44
#define DINO_FROM 215.0f
#define DINO_BACK_LOW -250.0f
#define DINO_FRONT -100.0f
#define DINO_BACK 150.0f
#define DINO_HIPS_TOP -415.0f
#define DINO_HIPS_LOW -350.0f
#define DINO_MIDDLE 32.0f
#define DINO_HIPS_FRONT -45.0f
#define DINO_CHEST_TOP -500.0f
#define DINO_CHEST_LOW -450.0f
#define DINO_CHEST_HALF 45.0f
#define DINO_CHEST_FRONT -60.0f
static const char *const DinoNames[] = { "BREATHE", "RUN", "SLIDE", "WIN", "RECOIL" };
static const FollowLegs DinoLegs = {
    { { -45, -395, -15 }, { 45, -395, -15 } }, { { -100, -355, -50 }, { 100, -355, -50 } },
    { { -110, -330, -110 }, { 110, -330, -110 } }, 30.0f,
};

static s32 dino_prop(const V3 *p, s32 n, s32 textured, s32 strip) {
    (void)p;
    (void)n;
    (void)textured;
    return strip < DINO_CREST || strip >= DINO_STRIP;
}
static s32 dino_frame(V3 p) {
    return p.y < DINO_BACK_LOW && p.z > DINO_FRONT && p.z < DINO_BACK;
}
static s32 dino_hips(V3 p) {
    return p.y > DINO_HIPS_TOP && p.y < DINO_HIPS_LOW && fabsf(p.x) < DINO_MIDDLE && p.z > DINO_HIPS_FRONT;
}
static s32 dino_chest(V3 p) {
    return p.y > DINO_CHEST_TOP && p.y < DINO_CHEST_LOW && fabsf(p.x) < DINO_CHEST_HALF && p.z > DINO_CHEST_FRONT;
}

static void dino_gunmetal(Follow *c) {
    hm_paint(c->prop.faces, c->prop.nfaces, c->prop.images, c->prop.nimages, DINO_FROM, GUNMETAL);
}

static const FollowKind DinoKind = {
    DB_CharsDinoCrash, KIND_DINO, "dino", DinoNames, 5,
    dino_prop, dino_frame, NULL, NULL, dino_hips, dino_chest, NULL,
    { { -55, -545, 0 }, { 55, -545, 0 } }, { { -65, -452, -30 }, { 65, -452, -30 } }, 35.0f, 30.0f,
    45.0f, -40.0f, { { -63, -408, -125 }, { 60, -396, -82 } },
    0.8f, 0, 0, 1, 0, 0, 1, dino_gunmetal, 0, &DinoLegs, 1, 0, 0,
};

/* -- with a keg (Keg Kaboom) --
 * Each hero's model in the kegs game is it with the keg it carries all game (models\chars\kegs\: five moves, BREATHE, MEDIUM as it runs,
 * JUMP up, falling and landing, HIT knocked down, and WIN). Crash's has two meshes, the second with his keg, and it's that one his
 * moves move (KEG_MESH; the first is never drawn): the keg is its strips from KEG_STRIP on. He carries it under his left arm, his hand
 * round its far side, in every move. Pinstripe does as Crash does, as with the mallet (stood on the floor, his hands where Crash's are;
 * Crash stands the same in both games), the keg under his left arm as Crash's is, at its own size (as big against him as against Crash
 * it was half as big again as the game's other kegs), his hand round it where Crash's is. */
#define KEG_MESH 1
#define KEG_STRIP 39
static const char *const KegNames[] = { "BREATHE", "MEDIUM", "JUMP", "WIN", "HIT" };

static s32 keg_prop(const V3 *p, s32 n, s32 textured, s32 strip) {
    (void)p;
    (void)n;
    (void)textured;
    return strip >= KEG_STRIP;
}

static const FollowKind KegKind = {
    DB_CharsKegsCrash, KIND_KEGS, "keg", KegNames, 5,
    keg_prop, NULL, NULL, NULL, mallet_hips, mallet_chest, mallet_foot,
    { { -54, -292, 15 }, { 54, -292, 15 } }, { { -155, -294, 16 }, { 114, -226, 28 } }, 45.0f, 35.0f,
    45.0f, 1e9f, { { -221, -241, 18 }, { 100, -152, -12 } },
    1.0f, 0, 0, 1, 0, 0, 1, NULL, 0, NULL, 1, 1, 1, KEG_MESH, 1,
};

/* --- the pack, posed as Crash is --- */

typedef struct {
    float m[3][3];
} Mat;

static V3 mat_apply(const Mat *a, V3 v) {
    return v3(a->m[0][0] * v.x + a->m[0][1] * v.y + a->m[0][2] * v.z, a->m[1][0] * v.x + a->m[1][1] * v.y + a->m[1][2] * v.z,
              a->m[2][0] * v.x + a->m[2][1] * v.y + a->m[2][2] * v.z);
}

static Mat mat_mul(const Mat *a, const Mat *b) {
    Mat r;
    s32 i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            r.m[i][j] = a->m[i][0] * b->m[0][j] + a->m[i][1] * b->m[1][j] + a->m[i][2] * b->m[2][j];
        }
    }
    return r;
}

/* the shortest turn from direction a to b, as a matrix */
static Mat mat_arc(V3 a, V3 b) {
    V3 x = vnorm(a), y = vnorm(b), ax = vcross(x, y);
    float c = vdot(x, y), s = vlen(ax), t;
    Mat r;

    if (s < 1e-6f) {
        memset(&r, 0, sizeof(r));
        r.m[0][0] = r.m[1][1] = r.m[2][2] = 1;
        if (c < 0) {
            r.m[0][0] = r.m[2][2] = -1;
        }
        return r;
    }
    ax = vmul(ax, 1 / s);
    t = 1 - c;
    r.m[0][0] = t * ax.x * ax.x + c, r.m[0][1] = t * ax.x * ax.y - s * ax.z, r.m[0][2] = t * ax.x * ax.z + s * ax.y;
    r.m[1][0] = t * ax.x * ax.y + s * ax.z, r.m[1][1] = t * ax.y * ax.y + c, r.m[1][2] = t * ax.y * ax.z - s * ax.x;
    r.m[2][0] = t * ax.x * ax.z - s * ax.y, r.m[2][1] = t * ax.y * ax.z + s * ax.x, r.m[2][2] = t * ax.z * ax.z + c;
    return r;
}

enum { PB_HIPS, PB_CHEST, PB_UPPER_L, PB_FORE_L, PB_HAND_L, PB_UPPER_R, PB_FORE_R, PB_HAND_R, PB_THIGH_L, PB_SHIN_L, PB_FOOT_L, PB_THIGH_R,
       PB_SHIN_R, PB_FOOT_R, PB_COUNT };
static const char *const PbNames[PB_COUNT] = { "hips", "chest", "upperarm_l", "forearm_l", "hand_l", "upperarm_r", "forearm_r", "hand_r",
                                               "thigh_l", "shin_l", "foot_l", "thigh_r", "shin_r", "foot_r" };

typedef struct {
    Follow c;
    const PackModel *pm;
    s32 n;                       /* its vertices */
    V3 rest[PF_MAX_VERTS];       /* as it rests, in Crash Bash's units and axes, at its size on the prop about its hips */
    V3 bone[PACK_MAX_BONES];     /* and its bones' places */
    s32 pb[PB_COUNT];            /* its bones the posing moves */
    s8 lower[PACK_MAX_BONES];    /* a bone of its legs (hidden below a cut) */
    s8 role[PACK_MAX_BONES];     /* which of PB_ a bone of its is, else -1 (it goes with its trunk, or the posed bone it hangs from) */
    Mat across;                  /* its trunk's frame as it rests (body_lie's columns) */
    float hip_height, crash_hip_height, sole;
    float k, ground, scale;      /* its rest pose to the game's units (k, ground), and its size on the prop */
    s32 own[FOLLOW_MOVES];       /* each move's clip of its own (KIND/MOVE), which it plays in place of following, or -1 */
    V3 feet_ref[2];
} Packed;

static Packed ThePack;

/* its trunk's frame as it rests: up its spine, across its shoulders, forward */
static Mat pack_frame(const Packed *p) {
    V3 up = vnorm(vsub(p->bone[p->pb[PB_CHEST]], p->bone[p->pb[PB_HIPS]]));
    V3 across = vsub(p->bone[p->pb[PB_UPPER_L]], p->bone[p->pb[PB_UPPER_R]]), fwd;
    Mat m;

    across = vnorm(vsub(across, vmul(up, vdot(across, up))));
    fwd = vcross(across, up);
    m.m[0][0] = across.x, m.m[0][1] = up.x, m.m[0][2] = fwd.x;
    m.m[1][0] = across.y, m.m[1][1] = up.y, m.m[1][2] = fwd.y;
    m.m[2][0] = across.z, m.m[2][1] = up.z, m.m[2][2] = fwd.z;
    return m;
}

/* whether a and b are the same in their first n characters, big or small (a shorter one ends at its 0) */
static s32 same_ci(const char *a, const char *b, size_t n) {
    size_t i;

    for (i = 0; i < n; i++) {
        char x = (char)(a[i] >= 'a' && a[i] <= 'z' ? a[i] - 32 : a[i]), y = (char)(b[i] >= 'a' && b[i] <= 'z' ? b[i] - 32 : b[i]);

        if (x != y) {
            return 0;
        }
        if (x == 0) {
            return 1;
        }
    }
    return 1;
}

static void bear_fill_at(const Packed *p, const Moved *mv, const V3 *L, V3 *out);

/* A move of its own clip (KIND/MOVE in its glTF) in place of the one it follows: its skeleton posed by that clip, across the length of
 * the move it's in place of, its hips on the followed hero's hips, at its size on the prop; the prop still as the followed hero's
 * move has it, or in its own hand where it carries it (docs/HERO_PACKS.md, Templates) */
static void own_frame(Packed *p, s32 a, float f, const Moved *mv, V3 hips_at, const V3 *L, V3 *out) {
    static PackTrs local[PACK_MAX_BONES];
    static float world[PACK_MAX_BONES][16], skinned[PF_MAX_VERTS * 3];
    const Follow *c = &p->c;
    const PackClip *clip = &p->pm->clips[p->own[a]];
    s32 frames = c->anims[a].frames, k, hb = p->pb[PB_HIPS];
    float t = frames > 1 ? clip->length * f / (float)(frames - 1) : 0;
    V3 hips;

    pack_pose_local(p->pm, p->own[a], t, local);
    pack_pose_world(p->pm, local, world);
    pack_skin(p->pm, (const float(*)[16])world, skinned);
    hips = v3(world[hb][12] * p->k, -(world[hb][13] - p->ground) * p->k, -world[hb][14] * p->k);
    for (k = 0; k < p->n; k++) {
        V3 at = v3(skinned[k * 3] * p->k, -(skinned[k * 3 + 1] - p->ground) * p->k, -skinned[k * 3 + 2] * p->k);

        out[k] = hm_moved(mv, vadd(hips_at, vmul(vsub(at, hips), p->scale)));
    }
    if (c->kind->carry) {
        s32 ci = c->kind->carry - 1, hand = p->pb[ci == 1 ? PB_HAND_R : PB_HAND_L];
        V3 Hd = mid_of(L, c->hand[ci], c->nhand[ci]);
        V3 at = v3(world[hand][12] * p->k, -(world[hand][13] - p->ground) * p->k, -world[hand][14] * p->k);

        at = vadd(hips_at, vmul(vsub(at, hips), p->scale));
        for (k = 0; k < c->prop.nvert; k++) {
            out[p->n + k] = hm_moved(mv, vadd(at, vsub(L[c->prop.corner_of[k]], Hd)));
        }
        return;
    }
    for (k = 0; k < c->prop.nvert; k++) {
        out[p->n + k] = hm_moved(mv, L[c->prop.corner_of[k]]);
    }
    bear_fill_at(p, mv, L, out);
}

static void pack_frame_at(void *ctx, s32 a, s32 f, V3 *out);

static void frame_at(void *ctx, s32 a, float f, V3 *out) {
    static V3 F[FOLLOW_CORNERS], L[FOLLOW_CORNERS];
    static Mat Q[PACK_MAX_BONES];
    static V3 P[PACK_MAX_BONES];
    Packed *p = ctx;
    Follow *c = &p->c;
    const FollowKind *fk = c->kind;
    float now[3][3];
    Mat body, R, rest_t;
    Moved mv;
    V3 H, hips_at, up_world = { 0, -1, 0 };
    s32 i, j, k, side;

    if (!hm_model_at(&c->prop, c->anim[a], f, F)) {
        memcpy(F, c->ref, sizeof(V3) * (size_t)c->prop.corners);
    }
    if (fk->frame != NULL) {
        hm_fit(c->ref, F, c->frame, c->nframe, &mv);
        for (k = 0; k < c->prop.corners; k++) {
            L[k] = vadd(mv.from, hm_unturned(&mv, vsub(F[k], mv.to)));
        }
    } else {
        memset(&mv, 0, sizeof(mv));
        mv.m[0][0] = mv.m[1][1] = mv.m[2][2] = 1;
        memcpy(L, F, sizeof(V3) * (size_t)c->prop.corners);
    }
    /* its trunk turned as Crash's is, its hips on his (stood, as much higher as its legs are longer) */
    body_lie(c, L, now);
    memcpy(body.m, now, sizeof(now));
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            rest_t.m[i][j] = p->across.m[j][i];
        }
    }
    R = mat_mul(&body, &rest_t);
    H = mid_of(L, c->hips, c->nhips);
    hips_at = H;
    if (fk->peg != NULL || fk->stand) {
        hips_at = vadd(H, vmul(up_world, p->hip_height - p->crash_hip_height));
    }
    if (p->own[a] >= 0) {
        own_frame(p, a, f, &mv, hips_at, L, out);
        return;
    }
    for (k = 0; k < p->pm->nbones; k++) {
        Q[k] = R;
        P[k] = vadd(hips_at, mat_apply(&R, vsub(p->bone[k], p->bone[p->pb[PB_HIPS]])));
    }
    /* each arm reaching for Crash's hand: on its grip where he holds one, else where his is from his shoulder for its size; its elbow
     * bent his way. Crash's x > 0 side is the pack's left (glTF's +x, which stays +x in Crash Bash's axes) */
    for (side = 0; side < 2; side++) {
        s32 ci = side == 0 ? 1 : 0, up = p->pb[side ? PB_UPPER_R : PB_UPPER_L], fo = p->pb[side ? PB_FORE_R : PB_FORE_L];
        s32 ha = p->pb[side ? PB_HAND_R : PB_HAND_L];
        V3 S = mid_of(L, c->shoulder[ci], c->nshoulder[ci]), E = mid_of(L, c->elbow[ci], c->nelbow[ci]), Hd = mid_of(L, c->hand[ci], c->nhand[ci]);
        V3 sh = P[up], target, elbow, hand, hint;
        float l1 = vlen(vsub(p->bone[fo], p->bone[up])), l2 = vlen(vsub(p->bone[ha], p->bone[fo]));
        float crash_arm = vlen(vsub(E, S)) + vlen(vsub(Hd, E));

        if (fk->grip != NULL && !fk->carry) {
            target = Hd;
        } else {
            target = vadd(sh, vmul(vsub(Hd, S), crash_arm > 1 ? (l1 + l2) / crash_arm : 1));
        }
        hint = vsub(E, vmul(vadd(S, Hd), 0.5f));
        elbow = two_bone(sh, target, l1, l2, hint);
        hand = vadd(elbow, vmul(vnorm(vsub(target, elbow)), l2));
        Q[up] = mat_arc(vsub(p->bone[fo], p->bone[up]), vsub(elbow, sh));
        P[up] = sh;
        Q[fo] = mat_arc(vsub(p->bone[ha], p->bone[fo]), vsub(hand, elbow));
        P[fo] = elbow;
        Q[ha] = Q[fo];
        P[ha] = hand;
    }
    /* its legs: as Crash's astride, on the pegs or the floor stood, else sat */
    for (side = 0; side < 2; side++) {
        s32 ci = side == 0 ? 1 : 0, th = p->pb[side ? PB_THIGH_R : PB_THIGH_L], sh = p->pb[side ? PB_SHIN_R : PB_SHIN_L];
        s32 ft = p->pb[side ? PB_FOOT_R : PB_FOOT_L];
        V3 hip = P[th], knee, ankle, target, hint;
        float l1 = vlen(vsub(p->bone[sh], p->bone[th])), l2 = vlen(vsub(p->bone[ft], p->bone[sh]));

        if (fk->legs != NULL) {
            V3 hc = mid_of(L, c->leg[0][ci], c->nleg[0][ci]), kc = mid_of(L, c->leg[1][ci], c->nleg[1][ci]);
            V3 ac = mid_of(L, c->leg[2][ci], c->nleg[2][ci]);
            float crash_leg = vlen(vsub(kc, hc)) + vlen(vsub(ac, kc));

            target = vadd(hip, vmul(vsub(ac, hc), crash_leg > 1 ? (l1 + l2) / crash_leg : 1));
            hint = vsub(kc, vmul(vadd(hc, ac), 0.5f));
        } else if (fk->peg != NULL || fk->stand) {
            /* its feet where Crash's are (on the pegs, or the floor), over his shoes, which are the prop's */
            target = c->nfeet[ci] > 0 ? mid_of(L, c->feet[ci], c->nfeet[ci])
                                      : vadd(hips_at, vsub(p->bone[ft], p->bone[p->pb[PB_HIPS]]));
            target.y -= (p->sole - p->bone[ft].y) * 0.5f; /* its ankle above its foot's middle */
            hint = mat_apply(&R, v3(0, 0, -1));
        } else {
            /* sat: thighs forward, shins down */
            V3 thigh = vnorm(mat_apply(&R, v3(0, 0.25f, -1))), shin = vnorm(mat_apply(&R, v3(0, 1, -0.1f)));

            target = vadd(vadd(hip, vmul(thigh, l1)), vmul(shin, l2));
            hint = thigh;
        }
        knee = two_bone(hip, target, l1, l2, hint);
        ankle = vadd(knee, vmul(vnorm(vsub(target, knee)), l2));
        Q[th] = mat_arc(vsub(p->bone[sh], p->bone[th]), vsub(knee, hip));
        P[th] = hip;
        Q[sh] = mat_arc(vsub(p->bone[ft], p->bone[sh]), vsub(ankle, knee));
        P[sh] = knee;
        Q[ft] = Q[sh];
        P[ft] = ankle;
    }
    /* a bone not posed goes as the posed one it hangs from does (a finger with its hand, a toe with its foot), else with its trunk */
    for (k = 0; k < p->pm->nbones; k++) {
        s32 b = k;

        if (p->role[k] >= 0) {
            continue;
        }
        while (b >= 0 && p->role[b] < 0) {
            b = p->pm->bones[b].parent;
        }
        if (b >= 0 && b != p->pb[PB_HIPS] && b != p->pb[PB_CHEST]) {
            Q[k] = Q[b];
            P[k] = vadd(P[b], mat_apply(&Q[b], vsub(p->bone[k], p->bone[b])));
        }
    }
    /* the mesh with its bones */
    for (k = 0; k < p->n; k++) {
        V3 s = { 0, 0, 0 };
        float lower = 0;

        for (j = 0; j < 4; j++) {
            s32 b = p->pm->joint[k * 4 + j];
            float w = p->pm->weight[k * 4 + j];

            if (w <= 0) {
                continue;
            }
            s = vadd(s, vmul(vadd(P[b], mat_apply(&Q[b], vsub(p->rest[k], p->bone[b]))), w));
            lower += p->lower[b] ? w : 0;
        }
        /* in a tank's hatch or a race cart, its legs hidden at one point just below its hips */
        if (fk->cut > 0 && lower > 0.5f) {
            s = vadd(hips_at, v3(0, 8, 0));
        }
        out[k] = hm_moved(&mv, s);
    }
    /* the prop, as it moves; or in the pack's hand, where Crash carries it */
    if (fk->carry) {
        s32 ci = fk->carry - 1, side = ci == 1 ? 0 : 1;
        V3 Hd = mid_of(L, c->hand[ci], c->nhand[ci]), at = P[p->pb[side ? PB_HAND_R : PB_HAND_L]];

        for (k = 0; k < c->prop.nvert; k++) {
            out[p->n + k] = hm_moved(&mv, vadd(at, vsub(L[c->prop.corner_of[k]], Hd)));
        }
        return;
    }
    for (k = 0; k < c->prop.nvert; k++) {
        out[p->n + k] = hm_moved(&mv, L[c->prop.corner_of[k]]);
    }
    bear_fill_at(p, &mv, L, out);
}

static const FollowKind *const PackKinds[] = { &PogoKind, &CarKind, &TankKind, &RaceKind, &FishKind, &MalletKind, &DinoKind, &KegKind };

/* -- kinds only a template's map fills in (docs/HERO_PACKS.md, Templates) --
 * On foot, as the crate games have their heroes (models\chars\crate2\: the same moves for all eight), stood as the template stands, at
 * the pack's own size; and riding the polar bear (models\chars\polar2\, as ride_make's is), its LIGHT on a charred second mesh.
 * Crash's on foot is a map too (his tables never had it); his bear is still ride_make's. Their parts' tests are the map's: these are
 * only there to say a kind has hips, feet, a prop, legs. */
static s32 map_any(V3 p);
static s32 map_any_prop(const V3 *at, s32 n, s32 textured, s32 strip);
#define FootNames PackFootMoves
static const FollowLegs AnyLegs;
static const FollowKind FootKind = {
    DB_CharsCrate2Crash, KIND_FOOT, "feet", FootNames, 5,
    NULL, NULL, NULL, NULL, map_any, map_any, map_any,
    { { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 } }, 25.0f, 25.0f,
    25.0f, 1e9f, { { 0, 0, 0 }, { 0, 0, 0 } },
    1.0f, 0, 0, 1, 0, 0, 1, NULL, 0, NULL, 1, 1, 0,
};
static const char *const PolarNames[] = { "BREATHE", "FALL", "RUN", "PUSH", "TAUNT", "LIGHT", "WIN" };
static const FollowKind PolarKind = {
    DB_CharsPolar2Crash, KIND_POLAR, "polar bear", PolarNames, 7,
    map_any_prop, map_any, NULL, NULL, map_any, map_any, NULL,
    { { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 } }, 25.0f, 25.0f,
    25.0f, 1e9f, { { 0, 0, 0 }, { 0, 0, 0 } },
    0.8f, 0, 0, 1, 0, 0, 1, NULL, 0, &AnyLegs, 1, 0, 0, 0, 0, 1,
};

/* --- following another hero (a template; docs/HERO_PACKS.md, Templates) ---
 * Crash's kind as it is, but that hero's model of it, its parts found by its map's markers (packtemplates.c): a joint's corners
 * within the map's r of its marker, the prop the strips away from its body, what it rides or holds as that hero's (not painted), and
 * the pack at that hero's size on it. */
#define CRASH_HEIGHT 452.0f /* Crash standing, whose kinds' sizes are for him */
static const TemplateMap *Map;
static FollowKind MapKind;
static FollowLegs MapLegs;

static s32 map_has(TmplPoint m) {
    return m.x != 0 || m.y != 0 || m.z != 0;
}
static V3 map_v(TmplPoint m) {
    return v3(m.x, m.y, m.z);
}
/* Each marker's reach, as map_fit finds it: hips, chest, each foot, grip and peg */
enum { MR_HIPS, MR_CHEST, MR_FOOT, MR_GRIP = MR_FOOT + 2, MR_PEG = MR_GRIP + 2, MR_COUNT = MR_PEG + 2 };
static float MapReach[MR_COUNT];
static s32 map_near(V3 p, TmplPoint m, s32 which) {
    return map_has(m) && vlen(vsub(p, map_v(m))) < MapReach[which];
}
static s32 map_in(V3 p, const TmplBox *b) {
    float x = b->sym ? fabsf(p.x) : p.x;

    return (b->lo.x != 0 || b->hi.x != 0 || b->lo.y != 0 || b->hi.y != 0) && x >= b->lo.x && x <= b->hi.x && p.y >= b->lo.y
           && p.y <= b->hi.y && p.z >= b->lo.z && p.z <= b->hi.z;
}
static s32 map_in_any(V3 p) {
    s32 i;

    for (i = 0; i < TMPL_BOXES && !map_in(p, &Map->prop[i]); i++) {
    }
    return i < TMPL_BOXES;
}
/* a strip mostly in the prop's boxes; for the polar bear, any strip in its box, its faces sorted after (bear_cut) */
static s32 map_prop(const V3 *at, s32 n, s32 textured, s32 strip) {
    s32 k, in = 0;

    (void)textured, (void)strip;
    for (k = 0; k < n; k++) {
        in += map_in_any(at[k]);
    }
    return MapKind.lit ? in > 0 : in * 2 > n;
}

/* --- the polar bear out of its rider's model ---
 * A hero's polar2 model is the hero and its bear in one mesh, and the bear is the same bear in each (Crash's, ride_make's, too), its
 * fur pale and greyish, its nose, eyes and soles black; a rider's strips run into it. So the bear is the faces with all three corners
 * in the map's box whose colour (the texel at the face's middle times its corners' colours, as the PlayStation draws it) is its fur's:
 * warm (not bluer than its red or green, nor greener than its red, its red at most BEAR_RED over its green) and its channels within
 * BEAR_GREY of each other. Black (darker than BEAR_BLACK) is a rider's shoes as often as the bear's nose, eyes and soles, so a black
 * face is the bear's only below BEAR_BLACK_TOP, its back's height, and where it's drawn as they are: untextured (its nose) or in an
 * image its pale fur has too (its eyes and soles). Then the fill (heromake.h) closes it where its rider covered it, fitted to it as
 * Crash's (HmCrashFill) is to his: BearBoxMid and BearBoxHalf are his bear's box by this rule (its body, above BEAR_BODY), and its
 * fur lit as much brighter or darker than his as its corners' colours are than BEAR_LIGHT (Coco's bear's, whose box is his). */
#define BEAR_GREY 0.36f
#define BEAR_RED 0.3f
#define BEAR_BLACK 30.0f
#define BEAR_BLACK_TOP -170.0f
#define BEAR_BODY -60.0f
#define BEAR_LIGHT 0.85f
static const V3 BearBoxMid = { 0, -136.5f, -43.5f }, BearBoxHalf = { 83, 73.5f, 197.5f };
static HmFill BearFill;
static s32 BearFills; /* its corners in the model being made, after the bear's (0: none) */

static void face_colour(const GamePart *g, const MadeFace *f, float rgb[3]) {
    float t[3] = { 128, 128, 128 };
    s32 j;

    if (f->image >= 0 && f->image < g->nimages && g->images[f->image].pixels != NULL) {
        const MadeImage *im = &g->images[f->image];
        s32 u = (f->uv[0][0] + f->uv[1][0] + f->uv[2][0]) / 3, v = (f->uv[0][1] + f->uv[1][1] + f->uv[2][1]) / 3;
        s32 row = im->units * 2, i;

        if (v >= 0 && v < im->h && u >= 0 && u < row * (im->bpp8 ? 1 : 2)) {
            u16 w;

            i = im->bpp8 ? im->pixels[v * row + u] : (im->pixels[v * row + u / 2] >> ((u & 1) * 4)) & 15;
            w = im->clut[i];
            t[0] = (w & 31) * 255 / 31.0f;
            t[1] = (w >> 5 & 31) * 255 / 31.0f;
            t[2] = (w >> 10 & 31) * 255 / 31.0f;
        }
    }
    for (j = 0; j < 3; j++) {
        float c = (f->rgb[0][0][j] + f->rgb[0][1][j] + f->rgb[0][2][j]) / 3.0f * t[j] / 128;

        rgb[j] = c > 255 ? 255 : c;
    }
}

/* 1 its fur's colour, 2 black (the bear's only by how it's drawn), 0 neither */
static s32 bear_fur(const float c[3]) {
    float hi = c[0] > c[1] ? c[0] : c[1], lo = c[0] < c[1] ? c[0] : c[1];

    hi = c[2] > hi ? c[2] : hi;
    lo = c[2] < lo ? c[2] : lo;
    if (!(c[0] + 8 >= c[2] && c[1] + 2 >= c[2] && c[1] <= c[0] + 10 && c[0] - c[1] <= BEAR_RED * c[0] && hi - lo <= BEAR_GREY * hi + 8)) {
        return 0;
    }
    return hi < BEAR_BLACK ? 2 : 1;
}

/* The bear's faces kept, on the corners they use, and its fill made */
static void bear_cut(Follow *c) {
    static s16 new_of[FOLLOW_CORNERS], was[FOLLOW_CORNERS];
    static s8 fur[MADE_MAX_FACES];
    s8 fur_image[PART_MAX_IMAGES];
    GamePart *g = &c->prop;
    V3 lo = { 1e9f, 1e9f, 1e9f }, hi = { -1e9f, -1e9f, -1e9f };
    HmFillShape sh;
    float light = 0;
    s32 i, j, n = 0, nv = 0, body = 0;

    for (i = 0; i < g->nvert; i++) {
        new_of[i] = -1;
        was[i] = g->corner_of[i];
    }
    memset(g->in_part, 0, (size_t)g->corners);
    memset(fur_image, 0, sizeof(fur_image));
    for (i = 0; i < g->nfaces && i < MADE_MAX_FACES; i++) {
        const MadeFace *f = &g->faces[i];
        float rgb[3];
        s32 in = 1;

        for (j = 0; j < 3; j++) {
            in &= map_in_any(c->ref[was[f->v[j]]]);
        }
        face_colour(g, f, rgb);
        fur[i] = (s8)(in ? bear_fur(rgb) : 0);
        if (fur[i] == 1 && f->image >= 0 && f->image < PART_MAX_IMAGES) {
            fur_image[f->image] = 1;
        }
    }
    for (i = 0; i < g->nfaces && i < MADE_MAX_FACES; i++) {
        MadeFace f = g->faces[i];
        float y = (c->ref[was[f.v[0]]].y + c->ref[was[f.v[1]]].y + c->ref[was[f.v[2]]].y) / 3;

        if (fur[i] == 0
            || (fur[i] == 2 && (y < BEAR_BLACK_TOP || (f.image >= 0 && (f.image >= PART_MAX_IMAGES || !fur_image[f.image]))))) {
            continue;
        }
        for (j = 0; j < 3; j++) {
            s32 o = f.v[j];

            if (new_of[o] < 0) {
                V3 q = c->ref[was[o]];

                new_of[o] = (s16)nv;
                g->corner_of[nv++] = was[o];
                g->in_part[was[o]] = 1;
                if (q.y < BEAR_BODY) {
                    lo = v3(q.x < lo.x ? q.x : lo.x, q.y < lo.y ? q.y : lo.y, q.z < lo.z ? q.z : lo.z);
                    hi = v3(q.x > hi.x ? q.x : hi.x, q.y > hi.y ? q.y : hi.y, q.z > hi.z ? q.z : hi.z);
                    body++;
                }
            }
            f.v[j] = new_of[o];
            light += (f.rgb[0][j][0] + f.rgb[0][j][1] + f.rgb[0][j][2]) / (3 * 128.0f);
        }
        g->faces[n++] = f;
    }
    g->nfaces = n;
    g->nvert = nv;
    BearFills = 0;
    BearFill.nvert = BearFill.nfaces = 0;
    if (n == 0 || body < 8) {
        return;
    }
    sh.mid = v3((lo.x + hi.x) / 2 + HmCrashFill.mid.x - BearBoxMid.x, (lo.y + hi.y) / 2 + HmCrashFill.mid.y - BearBoxMid.y,
                (lo.z + hi.z) / 2 + HmCrashFill.mid.z - BearBoxMid.z);
    sh.half = v3((hi.x - lo.x) / 2 * HmCrashFill.half.x / BearBoxHalf.x, (hi.y - lo.y) / 2 * HmCrashFill.half.y / BearBoxHalf.y,
                 (hi.z - lo.z) / 2 * HmCrashFill.half.z / BearBoxHalf.z);
    sh.legs = sh.mid.y + (HmCrashFill.legs - HmCrashFill.mid.y) * sh.half.y / HmCrashFill.half.y;
    sh.light = light / (3 * n) / BEAR_LIGHT;
    sh.under = 1;
    hm_fill_make(&BearFill, &sh, c->ref, g->corner_of, g->nvert, g->faces, g->nfaces);
    printf("hero packs: the polar bear %d faces, its box (%.0f %.0f %.0f) to (%.0f %.0f %.0f), lit %.2f, filled with %d\n", n, lo.x,
           lo.y, lo.z, hi.x, hi.y, hi.z, light / (3 * n), BearFill.nfaces);
}

/* Its fill's corners in a frame, after the bear's, as the bear's L have moved */
static void bear_fill_at(const Packed *p, const Moved *mv, const V3 *L, V3 *out) {
    const Follow *c = &p->c;
    s32 k;

    for (k = 0; k < BearFills; k++) {
        out[p->n + c->prop.nvert + k] = hm_moved(mv, hm_fill_at(&BearFill, k, c->ref, L, c->prop.corner_of));
    }
}
static s32 map_frame(V3 p) {
    (void)p;
    return 1;
}
static s32 map_any(V3 p) {
    (void)p;
    return 1;
}
static s32 map_any_prop(const V3 *at, s32 n, s32 textured, s32 strip) {
    (void)at, (void)n, (void)textured, (void)strip;
    return 1;
}
static s32 map_grip(V3 p) {
    return map_near(p, Map->grip[p.x > 0], MR_GRIP + (p.x > 0));
}
static s32 map_peg(V3 p) {
    return map_near(p, Map->peg[p.x > 0], MR_PEG + (p.x > 0));
}
static s32 map_hips(V3 p) {
    return map_near(p, Map->hips, MR_HIPS);
}
static s32 map_chest(V3 p) {
    return map_near(p, Map->chest, MR_CHEST);
}
static s32 map_foot(V3 p) {
    return map_near(p, Map->foot[p.x > 0], MR_FOOT + (p.x > 0));
}

/* A marker's reach on the model: out to its MAP_NEAREST nearest corners (of its body, or of the prop for a grip or a peg), at least
 * the map's r and at most MAP_FARTHEST times it (on_prop 2: of either). A marker inside a big body still finds the corners round it. */
#define MAP_NEAREST 6
#define MAP_FARTHEST 5.0f
static float map_reach(const Follow *c, TmplPoint m, s32 on_prop, s32 side) {
    float best[MAP_NEAREST];
    s32 k, i, n = 0;

    if (!map_has(m)) {
        return 0;
    }
    for (k = 0; k < c->prop.corners; k++) {
        float d;

        if ((on_prop != 2 && (c->kind->prop != NULL && c->prop.in_part[k]) != on_prop) || (side < 0 && c->ref[k].x > -10) || (side > 0 && c->ref[k].x < 10)) {
            continue;
        }
        d = vlen(vsub(c->ref[k], map_v(m)));
        if (n < MAP_NEAREST) {
            i = n++;
        } else if (d >= best[MAP_NEAREST - 1]) {
            continue;
        } else {
            i = MAP_NEAREST - 1;
        }
        for (; i > 0 && best[i - 1] > d; i--) {
            best[i] = best[i - 1];
        }
        best[i] = d;
    }
    if (n == 0) {
        return Map->r;
    }
    return fminf(fmaxf(best[n - 1] + 1, Map->r), Map->r * MAP_FARTHEST);
}

/* each marker's reach on the model as it first moves, before its parts are found by them */
static void map_fit(Follow *c) {
    const TemplateMap *m = Map;
    s32 i;

    MapReach[MR_HIPS] = map_reach(c, m->hips, 0, 0);
    /* the chest's corners either side of its middle too, for its line across (FollowKind.across_chest) */
    MapReach[MR_CHEST] = fmaxf(map_reach(c, m->chest, 0, 0), fmaxf(map_reach(c, m->chest, 0, -1), map_reach(c, m->chest, 0, 1)));
    MapKind.shoulder_r = MapKind.elbow_r = MapKind.hand_r = MapLegs.r = m->r;
    for (i = 0; i < 2; i++) {
        MapReach[MR_FOOT + i] = map_reach(c, m->foot[i], 0, 0);
        MapReach[MR_GRIP + i] = map_reach(c, m->grip[i], 1, 0);
        MapReach[MR_PEG + i] = map_reach(c, m->peg[i], 1, 0);
        MapKind.shoulder_r = fmaxf(MapKind.shoulder_r, map_reach(c, m->shoulder[i], 0, 0));
        MapKind.elbow_r = fmaxf(MapKind.elbow_r, map_reach(c, m->elbow[i], 0, 0));
        /* (a hand on what it holds, or where a model has none, as Coco in her cart: the corners there, of the prop too) */
        MapKind.hand_r = fmaxf(MapKind.hand_r, map_reach(c, m->hand[i], 2, 0));
        MapLegs.r = fmaxf(MapLegs.r, fmaxf(map_reach(c, m->hip[i], 0, 0), fmaxf(map_reach(c, m->knee[i], 0, 0), map_reach(c, m->ankle[i], 0, 0))));
    }
}

/* Crash's kind made the map's hero's: NULL if it can't be (no map, or a part Crash's kind needs that the map hasn't) */
static const FollowKind *map_kind(const FollowKind *crash, s32 hero) {
    const TemplateMap *m = pack_template_map(hero, crash->kind);
    s32 i;

    if (m == NULL || !map_has(m->hips) || !map_has(m->chest)) {
        return NULL;
    }
    Map = m;
    MapKind = *crash;
    MapKind.bank = m->bank;
    /* a map with no prop boxes has none: its model is the hero alone, what it rides drawn by the game (Cortex's hover pogo) */
    for (i = 0; i < TMPL_BOXES && !map_has(m->prop[i].lo) && !map_has(m->prop[i].hi); i++) {
    }
    MapKind.prop = crash->prop != NULL && i < TMPL_BOXES ? map_prop : NULL;
    MapKind.frame = MapKind.prop != NULL && crash->frame != NULL ? map_frame : NULL;
    MapKind.grip = MapKind.prop != NULL && crash->grip != NULL && map_has(m->grip[0]) ? map_grip : NULL;
    MapKind.peg = MapKind.prop != NULL && crash->peg != NULL && map_has(m->peg[0]) ? map_peg : NULL;
    MapKind.hips = map_hips;
    MapKind.chest = map_chest;
    MapKind.foot = crash->foot != NULL && map_has(m->foot[0]) ? map_foot : NULL;
    for (i = 0; i < 2; i++) {
        MapKind.shoulder[i] = map_v(m->shoulder[i]);
        MapKind.elbow[i] = map_v(m->elbow[i]);
        MapKind.hand[i] = map_v(m->hand[i]);
        MapLegs.hip[i] = map_v(m->hip[i]);
        MapLegs.knee[i] = map_v(m->knee[i]);
        MapLegs.ankle[i] = map_v(m->ankle[i]);
    }
    MapKind.shoulder_r = MapKind.elbow_r = m->r;
    MapKind.hand_r = m->r * 1.3f;
    MapKind.hand_back = 1e9f;
    MapLegs.r = m->r;
    MapKind.legs = crash->legs != NULL ? &MapLegs : NULL;
    MapKind.paint = NULL;
    MapKind.mesh = m->mesh1 > 0 ? m->mesh1 - 1 : crash->mesh;
    MapKind.carry = crash->carry != 0 && m->carry != 0 ? m->carry : crash->carry;
    MapKind.scale = crash->kind == KIND_FOOT ? 1 : crash->scale * PackTemplateHeight[hero] / CRASH_HEIGHT * (m->scale > 0 ? m->scale : 1);
    return &MapKind;
}

static s32 pack_follow_one(Packed *p, const FollowKind *fk, const MadeFace *faces, s32 nfaces, const MadeImage *images, s32 nimages,
                           HeroMade *out) {
    Follow *c = &p->c;
    MadeFace *all = NULL;
    MadeImage *imgs = NULL;
    MadeModel mm;
    float top = 0, sole = -1e9f;
    s32 i, k, ok = 0, nfill = 0;

    memset(c, 0, sizeof(*c));
    c->kind = fk;
    if (fk->prop == NULL) {
        if (!hm_game_part_of(fk->bank, fk->mesh, keep_all, NULL, &c->prop)) {
            return 0;
        }
    } else {
        GamePart whole;
        PropPick pick;
        s32 got;

        if (!hm_game_part_of(fk->bank, fk->mesh, keep_all, NULL, &whole)) {
            return 0;
        }
        got = whole.corners <= FOLLOW_CORNERS && (i = hm_model_anim(&whole, fk->moves[0], NULL)) >= 0 && hm_model_at(&whole, i, 0, c->ref);
        hm_part_free(&whole);
        pick.kind = fk;
        pick.ref = c->ref;
        pick.at = pick.strip = 0;
        if (!got || !hm_game_part_of(fk->bank, fk->mesh, keep_prop, &pick, &c->prop)) {
            return 0;
        }
    }
    if (c->prop.corners > FOLLOW_CORNERS || fk->nmoves > FOLLOW_MOVES) {
        goto done;
    }
    for (i = 0; i < fk->nmoves; i++) {
        if ((c->anim[i] = hm_model_anim(&c->prop, fk->moves[i], &c->anims[i].frames)) < 0) {
            goto done;
        }
        c->anims[i].name = fk->moves[i];
        c->anims[i].mesh = 0;
        c->anims[i].step = 1;
        /* a clip of its own for this kind's move, KIND/MOVE (heropack.c's kind names) */
        p->own[i] = -1;
        for (k = 0; k < p->pm->nclips && p->own[i] < 0; k++) {
            const char *name = p->pm->clips[k].name, *kn = pack_kind_name(fk->kind);
            size_t kl = strlen(kn);

            if (same_ci(name, kn, kl) && name[kl] == '/' && same_ci(name + kl + 1, fk->moves[i], strlen(fk->moves[i]) + 1)) {
                p->own[i] = k;
            }
        }
    }
    if (!hm_model_at(&c->prop, c->anim[0], 0, c->ref)) {
        goto done;
    }
    if (fk == &MapKind) {
        map_fit(c);
    }
    BearFills = 0;
    if (fk->lit) {
        bear_cut(c);
    }
    /* Crash's parts, by where they are as he first moves (as Pinstripe's are found) */
    for (k = 0; k < c->prop.corners; k++) {
        V3 q = c->ref[k];

        if (fk->prop != NULL && c->prop.in_part[k]) {
            if (fk->frame != NULL && fk->frame(q) && c->nframe < FOLLOW_PART_MAX) {
                c->frame[c->nframe++] = (s16)k;
            }
            if (fk->grip != NULL && fk->grip(q)) {
                s32 s = fk->carry ? 0 : q.x > 0;

                if (c->ngrips[s] < FOLLOW_PART_MAX) {
                    c->grips[s][c->ngrips[s]++] = (s16)k;
                }
            }
            continue;
        }
        if (fk->hips(q) && c->nhips < FOLLOW_PART_MAX) {
            c->hips[c->nhips++] = (s16)k;
        }
        if (fk->chest(q) && c->nchest < FOLLOW_PART_MAX) {
            c->chest[c->nchest++] = (s16)k;
            if (fabsf(q.x) > 10 && c->nhalf[q.x > 0] < FOLLOW_PART_MAX) {
                c->half[q.x > 0][c->nhalf[q.x > 0]++] = (s16)k;
            }
        }
        for (i = 0; i < 2; i++) {
            if (vlen(vsub(q, fk->shoulder[i])) < fk->shoulder_r && c->nshoulder[i] < FOLLOW_PART_MAX) {
                c->shoulder[i][c->nshoulder[i]++] = (s16)k;
            }
            if (vlen(vsub(q, fk->elbow[i])) < fk->elbow_r && c->nelbow[i] < FOLLOW_PART_MAX) {
                c->elbow[i][c->nelbow[i]++] = (s16)k;
            }
        }
        for (i = 0; i < 2 && fk->legs != NULL; i++) {
            const V3 *joint[3] = { fk->legs->hip, fk->legs->knee, fk->legs->ankle };
            s32 j;

            for (j = 0; j < 3; j++) {
                if (vlen(vsub(q, joint[j][i])) < fk->legs->r && c->nleg[j][i] < FOLLOW_PART_MAX) {
                    c->leg[j][i][c->nleg[j][i]++] = (s16)k;
                }
            }
        }
        if (fk->foot != NULL && fk->foot(q) && c->nfeet[q.x > 0] < FOLLOW_PART_MAX) {
            c->feet[q.x > 0][c->nfeet[q.x > 0]++] = (s16)k;
        }
        top = q.y < top ? q.y : top;
        sole = q.y > sole ? q.y : sole;
    }
    for (k = 0; k < c->prop.corners; k++) {
        V3 q = c->ref[k];

        for (i = 0; i < 2 && (fk == &MapKind || !(fk->prop != NULL && c->prop.in_part[k])); i++) {
            V3 at = fk->hand[i];

            /* (a template's hands are where its map says, whatever they hold) */
            if (fk->grip != NULL && !fk->carry && c->ngrips[i] > 0 && fk != &MapKind) {
                at = mid_of(c->ref, c->grips[i], c->ngrips[i]);
            }
            if (vlen(vsub(q, at)) < fk->hand_r && q.z < fk->hand_back && c->nhand[i] < FOLLOW_PART_MAX) {
                c->hand[i][c->nhand[i]++] = (s16)k;
            }
        }
    }
    if (c->nhips < 4 || c->nchest < 4 || c->nshoulder[0] < 2 || c->nshoulder[1] < 2 || c->nelbow[0] < 2 || c->nelbow[1] < 2
        || c->nhand[0] < 2 || c->nhand[1] < 2 || (fk->frame != NULL && c->nframe < 8 && fk != &MapKind)) {
        /* (each part's corners: a template's map is checked by this) */
        printf("hero packs: the hero followed isn't with its %s as it was (corners: hips %d, chest %d, shoulders %d %d, elbows %d %d, hands "
               "%d %d, frame %d; prop %d of %d)\n", fk->what, c->nhips, c->nchest, c->nshoulder[0], c->nshoulder[1], c->nelbow[0],
               c->nelbow[1], c->nhand[0], c->nhand[1], c->nframe, c->prop.corners, c->prop.corners);
        goto done;
    }
    /* a template's prop too small to fit a frame to (Dingodile's thin pogo stick): the model's own frame */
    if (fk == &MapKind && fk->frame != NULL && c->nframe < 8) {
        MapKind.frame = NULL;
    }
    /* where his feet first are, and his hips' height above them */
    for (i = 0; i < 2; i++) {
        p->feet_ref[i] = c->nfeet[i] > 0 ? mid_of(c->ref, c->feet[i], c->nfeet[i]) : v3(0, sole, 0);
    }
    p->crash_hip_height = vmul(vadd(p->feet_ref[0], p->feet_ref[1]), 0.5f).y - mid_of(c->ref, c->hips, c->nhips).y;
    if (fk->prop == NULL) {
        /* none: its frames are the pack's alone (the part's corners still measure the hero) */
        made_images_free(c->prop.images, c->prop.nimages);
        c->prop.nfaces = c->prop.nimages = c->prop.nvert = 0;
    }
    if (out == NULL) {
        /* only made ready, for its frames (pack_follow_foot): its part kept till pack_follow_foot_end */
        return 1;
    }
    if (fk->paint != NULL) {
        fk->paint(c);
    }
    nfill = fk->lit && nfaces + c->prop.nfaces + BearFill.nfaces <= MADE_MAX_FACES ? BearFill.nfaces : 0;
    BearFills = nfill > 0 ? BearFill.nvert : 0;
    all = malloc(sizeof(MadeFace) * (size_t)(nfaces + c->prop.nfaces + nfill));
    imgs = malloc(sizeof(MadeImage) * (size_t)(nimages + c->prop.nimages + 1));
    if (all == NULL || imgs == NULL || nfaces + c->prop.nfaces > MADE_MAX_FACES) {
        goto done;
    }
    for (k = 0; k < nfill; k++) {
        MadeFace *f = &all[nfaces + c->prop.nfaces + k];

        *f = BearFill.faces[k];
        for (i = 0; i < 3; i++) {
            f->v[i] += p->n + c->prop.nvert;
        }
    }
    memcpy(all, faces, sizeof(MadeFace) * (size_t)nfaces);
    for (k = 0; k < c->prop.nfaces; k++) {
        MadeFace *f = &all[nfaces + k];

        *f = c->prop.faces[k];
        for (i = 0; i < 3; i++) {
            f->v[i] += p->n;
        }
        f->image = f->image >= 0 ? f->image + nimages : -1;
    }
    memcpy(imgs, images, sizeof(MadeImage) * (size_t)nimages);
    memcpy(imgs + nimages, c->prop.images, sizeof(MadeImage) * (size_t)c->prop.nimages);
    memset(&mm, 0, sizeof(mm));
    mm.faces = all;
    mm.nfaces = nfaces + c->prop.nfaces + nfill;
    mm.meshes = 1;
    mm.images = imgs;
    mm.nimages = nimages + c->prop.nimages;
    mm.anims = c->anims;
    mm.nanims = fk->nmoves;
    mm.nvert = p->n + c->prop.nvert + BearFills;
    mm.rest_anim = 0;
    mm.frame = pack_frame_at;
    mm.ctx = p;
    if (fk->lit) {
        /* the second mesh the same faces charred, its LIGHT on it (as ride_make's) */
        for (k = 0; k < mm.nfaces; k++) {
            for (i = 0; i < 9; i++) {
                all[k].rgb[1][i / 3][i % 3] = (u8)(all[k].rgb[0][i / 3][i % 3] * 0.3f);
            }
        }
        for (i = 0; i < fk->nmoves; i++) {
            c->anims[i].mesh = strcmp(fk->moves[i], "LIGHT") == 0;
        }
        mm.meshes = 2;
        mm.shared_colours = mm.nfaces * 3 * 2 > 0x1FFF;
    }
    ok = made_write(&mm, &out->tex[fk->kind], &out->tex_size[fk->kind], &out->mdl[fk->kind], &out->mdl_size[fk->kind]);
done:
    free(all);
    free(imgs);
    hm_part_free(&c->prop);
    return ok;
}

/* Its models of each kind where the game's heroes ride or hold something, followed as Crash's are (the top of this file): its rest pose
 * from its pack (pm, in glTF's axes), k its units to Crash Bash's on foot, and its feet's height there (ground) */
static void pack_frame_at(void *ctx, s32 a, s32 f, V3 *out) {
    frame_at(ctx, a, (float)f, out);
}

/* The pack's rest pose and the bones the posing moves, found by their names: 0 if it hasn't them */
static PackTrs BindLocal[PACK_MAX_BONES];
static float BindWorld[PACK_MAX_BONES][16], BindSkinned[PF_MAX_VERTS * 3];
static s32 pack_bind(Packed *p, const char *id, const PackModel *pm, float k, float ground) {
    float (*world)[16] = BindWorld;
    s32 b, i;

    if (pm->nvert > PF_MAX_VERTS) {
        return 0;
    }
    for (i = 0; i < PB_COUNT; i++) {
        p->pb[i] = -1;
        for (b = 0; b < pm->nbones; b++) {
            const char *a = pm->bones[b].name, *n = PbNames[i];

            while (*a != 0 && *n != 0 && (*a | 32) == (*n | 32)) {
                a++, n++;
            }
            if (*a == 0 && *n == 0) {
                p->pb[i] = b;
            }
        }
        if (p->pb[i] < 0) {
            printf("hero packs: %s has no %s bone, so it plays on foot where others ride\n", id, PbNames[i]);
            return 0;
        }
    }
    p->pm = pm;
    p->k = k;
    p->ground = ground;
    p->n = pm->nvert;
    pack_pose_local(pm, -1, 0, BindLocal);
    pack_pose_world(pm, BindLocal, world);
    pack_skin(pm, (const float(*)[16])world, BindSkinned);
    return 1;
}

/* The pack as it rests, at its size on a kind, about its hips, and its bones' roles there */
static void pack_prep(Packed *p, const FollowKind *fk) {
    const PackModel *pm = p->pm;
    float (*world)[16] = BindWorld, *skinned = BindSkinned, k = p->k, ground = p->ground;
    s32 b, i, v;
    V3 hips;

    p->scale = fk->scale;
    hips = v3(world[p->pb[PB_HIPS]][12] * k, -(world[p->pb[PB_HIPS]][13] - ground) * k, -world[p->pb[PB_HIPS]][14] * k);

    /* in Crash Bash's axes and units at its size on this, about its hips */
    for (v = 0; v < pm->nvert; v++) {
        V3 at = v3(skinned[v * 3] * k, -(skinned[v * 3 + 1] - ground) * k, -skinned[v * 3 + 2] * k);

        p->rest[v] = vadd(hips, vmul(vsub(at, hips), fk->scale));
    }
    for (b = 0; b < pm->nbones; b++) {
        V3 at = v3(world[b][12] * k, -(world[b][13] - ground) * k, -world[b][14] * k);
        const char *n = pm->bones[b].name;
        char c0 = (char)(n[0] | 32), c1 = (char)(n[0] != 0 ? n[1] | 32 : 0);

        p->bone[b] = vadd(hips, vmul(vsub(at, hips), fk->scale));
        p->lower[b] = (s8)((c0 == 't' && c1 == 'h') || (c0 == 's' && c1 == 'h') || (c0 == 'f' && c1 == 'o'));
        p->role[b] = -1;
    }
    for (i = 0; i < PB_COUNT; i++) {
        p->role[p->pb[i]] = (s8)i;
    }
    p->sole = -1e9f;
    for (v = 0; v < pm->nvert; v++) {
        p->sole = p->rest[v].y > p->sole ? p->rest[v].y : p->sole;
    }
    p->across = pack_frame(p);
    p->hip_height = (p->bone[p->pb[PB_FOOT_L]].y + p->bone[p->pb[PB_FOOT_R]].y) / 2 - p->bone[p->pb[PB_HIPS]].y;
}

void pack_follow_all(const char *id, const PackModel *pm, float k, float ground, const MadeFace *faces, s32 nfaces, const MadeImage *images,
                     s32 nimages, const s32 *templates, HeroMade *out) {
    Packed *p = &ThePack;
    s32 made = 0;
    size_t kind;

    if (!pack_bind(p, id, pm, k, ground)) {
        return;
    }
    /* each kind Crash's tables have, then the polar bear where its template isn't Crash (his is ride_make's) */
    for (kind = 0; kind <= sizeof(PackKinds) / sizeof(PackKinds[0]); kind++) {
        const FollowKind *fk = kind < sizeof(PackKinds) / sizeof(PackKinds[0]) ? PackKinds[kind] : &PolarKind;
        s32 hero = templates != NULL ? templates[fk->kind] : 1;

        if (fk == &PolarKind && hero <= 1) {
            continue;
        }
        /* its template's kind (another hero's, by its map), else Crash's */
        if (hero > 1 && map_kind(fk, hero) != NULL) {
            fk = &MapKind;
            printf("hero packs: %s follows hero %d %s\n", id, hero, fk->what);
        } else if (hero > 1) {
            printf("hero packs: %s follows Crash %s: there's no map of hero %d's model of it yet\n", id, fk->what, hero);
            if (fk == &PolarKind) {
                continue;
            }
        }
        pack_prep(p, fk);
        if (pack_follow_one(p, fk, faces, nfaces, images, nimages, out)) {
            made++;
        } else {
            printf("hero packs: %s has no model %s\n", id, fk->what);
        }
    }
    printf("hero packs: %s rides and holds things in %d of the games' kinds\n", id, made);
}

/* On foot, as its template stands (packheroes.c's model on foot): made ready to give frames, the template's moves on foot, and let go */
s32 pack_follow_foot(const char *id, const PackModel *pm, float k, float ground, s32 hero) {
    Packed *p = &ThePack;

    if (!pack_bind(p, id, pm, k, ground) || map_kind(&FootKind, hero) == NULL) {
        return 0;
    }
    pack_prep(p, &MapKind);
    if (!pack_follow_one(p, &MapKind, NULL, 0, NULL, 0, NULL)) {
        hm_part_free(&p->c.prop);
        return 0;
    }
    printf("hero packs: %s stands as hero %d does\n", id, hero);
    return 1;
}

/* Its moves on foot (FootNames), each with its frames, or 0 frames if the template's model hasn't it */
s32 pack_follow_foot_moves(const char *const **names, s32 *frames) {
    s32 i;

    for (i = 0; i < 5; i++) {
        frames[i] = ThePack.c.anims[i].frames;
    }
    *names = FootNames;
    return 5;
}

void pack_follow_foot_frame(s32 move, float f, V3 *out) {
    frame_at(&ThePack, move, f, out);
}

void pack_follow_foot_end(void) {
    hm_part_free(&ThePack.c.prop);
}
