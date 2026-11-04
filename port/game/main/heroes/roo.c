/* Ripper Roo (bash/herodef.h), from Crash Bandicoot (USA, SCUS-94900): the extra hero the others follow. docs/EXTRA_HEROES.md goes
 * through this file a part at a time, for making another.
 *
 * Level 0x17, his boss fight, has his model (RS1nG) and four animations, RS1nV (a big jump), RS2nV (standing), RS3nV (a laugh) and RS4nV
 * (knocked over), which become JUMP, BREATHE, TAUNT and DIE; two more are made for him from them, a walk, his legs stepping (WALK), and
 * a spin attack, his legs out as he spins in a blue tornado (SPIN). Riding a polar bear, he's sat astride Crash's and moves as Crash
 * does on it (bonusheroes.c, ride_make), swaying as he stands and laughing as he taunts and wins. His sounds are his boss fight's: his
 * laugh, his jump and his landings. His hero card is the face that fight shows, whose tongue is his health. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "bash/main.h"
#include "bash/hero.h"
#include "bash/heromake.h"

/* --- how he plays ---
 * His base is Rilla Roo, Kanga inside, if nobody else is playing her. On foot he stands still holding a crate (BANK), walks where the
 * others run, walk carrying, slide or swim, spins where they attack or kick, laughs where they throw, taunt or win, and falls over when
 * he's hurt. His JUMP is his boss fight's, which crouches into a tall stance first (frames 0 to 12), so he jumps from 13. Riding a bear,
 * his model has every one of Polar Push's animations by name, and he's as strong and as quick on it as Crash is. */
static const Stand RooStands[] = {
    { "JUMP", "JUMP:13-27" }, { "MEDIUM", "WALK" }, { "RUN", "WALK" }, { "SKATE", "WALK" }, { "SLIDE", "WALK" },
    { "SWIM", "WALK" }, { "FLIP", "WALK" }, { "FALL", "WALK" }, { "PUSH", "WALK" }, { "TRANS", "WALK" },
    { "HOLD_SLOW", "WALK" }, { "BARGE", "WALK" }, { "BANK", "BREATHE" }, { "ATTACK", "SPIN" }, { "KICK", "SPIN" },
    { "WIN", "TAUNT", LOOP }, { "WIN2", "TAUNT", LOOP },
    { "PICKUP", "TAUNT" }, { "HOLD_THROW", "TAUNT" }, { "TAUNT", "TAUNT" }, { "TAUNT_A", "TAUNT" },
    { "FIRE1", "TAUNT" }, { "FIRE2", "TAUNT" }, { "FIRE3", "TAUNT" }, { "MINE", "TAUNT" }, { "RECOIL", "TAUNT" },
    { "TAZING", "TAUNT" }, { "LIGHT", "TAUNT" },
    { "HIT", "DIE", ONCE }, { "DAZED", "DIE", ONCE }, { "DIE", "DIE", ONCE },
    { NULL, NULL, STRETCH },
};
static const Stand RooRides[] = {
    { NULL, NULL, STRETCH },
};

/* His sounds for his base's (docs/EXTRA_HEROES.md, A hero's sounds): the announcer still names Rilla Roo, as near a name as there is;
 * he laughs as he taunts and wins, yelps as he's hurt, grunts and throws, gives his jump's call as he jumps and lunges, and thuds as he
 * lands. Carrying a crate he's quiet. */
static const Voice RooVoices[] = {
    { 0, VOICE_BASE }, { 1, VOICE_LAUGH }, { 2, VOICE_YELP }, { 3, VOICE_YELP }, { 4, VOICE_YELP }, { 5, VOICE_YELP },
    { 6, VOICE_LAUGH },
    /* Crate Crush's */
    { 9, VOICE_NONE }, { 10, VOICE_NONE }, { 11, VOICE_JUMP }, { 12, VOICE_THUD }, { 13, VOICE_JUMP }, { 14, VOICE_YELP },
    { 15, VOICE_YELP }, { 16, VOICE_YELP },
    /* Polar Push's */
    { 19, VOICE_YELP }, { 20, VOICE_YELP }, { 21, VOICE_JUMP }, { 22, VOICE_YELP }, { 23, VOICE_YELP }, { 24, VOICE_LAUGH },
    { 25, VOICE_YELP },
    { -1, 0 },
};

static s32 roo_make(HeroMade *out);

const HeroDef HeroRoo = {
    "RIPPER ROO", "roo", "SCUS-94900", Kanga,
    { { RooStands, "BREATHE", 0x500, 0 }, { RooRides, "BREATHE", 0x1000, Crash } },
    RooVoices, "BREATHE", 0, roo_make,
};

/* --- how he's made --- */

#define ROO_LEVEL "S1/S0000017.NSF"
#define ROO_HEIGHT 620.0f /* standing, in Crash Bash's units: Crate Crush's heroes stand 452 (Crash) to 748 (Kong), most about 650 */
#define MAX_TRIS 2048
#define MAX_VERTS 1024

typedef struct {
    const char *entry, *name;
    s32 made; /* the frames of one made for him from that entry's (rig_setup), or 0 for the entry's own */
} Anim;

static const Anim RooAnims[] = {
    { "RS1nV", "JUMP", 0 }, { "RS2nV", "BREATHE", 0 }, { "RS3nV", "TAUNT", 0 }, { "RS4nV", "DIE", 0 }, { "RS2nV", "WALK", 20 },
    { "RS1nV", "SPIN", 15 },
};
#define ROO_ANIMS 6
#define ROO_REST 1  /* the one he stands in: his height and his colours' light are from its first frame */
#define ROO_LAUGH 2
#define ROO_WALK 4  /* made for him, as his game has no walk (he only ever jumps in his boss fight) and no attack */
#define ROO_SPIN 5

/* His standing animation (ROO_REST) as his boss fight's, but with only ROO_SWAY of its rocking: each frame's vertices are drawn in
 * toward the middle of its frames, each vertex's mean over them. */
#define ROO_SWAY 0.6f
static V3 SwayMiddle[MAX_VERTS];
static s32 SwayVerts;

static void sway_middle(const C1Entry *a, const s32 scale[3]) {
    static V3 fv[MAX_VERTS];
    s32 f, k, n;

    SwayVerts = 0;
    memset(SwayMiddle, 0, sizeof(SwayMiddle));
    for (f = 0; f < a->count; f++) {
        n = c1_frame_verts(a->item[f], a->size[f], scale, fv, NULL, MAX_VERTS);
        if (n == 0 || (f > 0 && n != SwayVerts)) {
            SwayVerts = 0;
            return;
        }
        SwayVerts = n;
        for (k = 0; k < n; k++) {
            SwayMiddle[k].x += fv[k].x / a->count;
            SwayMiddle[k].y += fv[k].y / a->count;
            SwayMiddle[k].z += fv[k].z / a->count;
        }
    }
}

/* --- his made animations ---
 * His game has no walk and no attack, so they're made for him from his own frames (ROO_WALK, ROO_SPIN), worked in his game's axes
 * and units, y up and him facing +z: c1_frame_verts' over ROO_UNIT, y and z the other way round. Both start from his standing pose,
 * SwayMiddle. WALK steps: each foot goes back flat along the floor, then up, its knee out in front, and forward again, the body
 * bobbing, leaning over the leg it's on and turning against the legs. SPIN, his attack: he throws his legs out to the sides as his
 * jump does (its frame SPIN_OUT, the splits), spins round twice in a hop in a blue tornado, and lands as he stood. */
#define ROO_UNIT 4096.0f
#define ROO_PI 3.14159265f

/* His hips, and how much of a leg each vertex is: all of it below LEG_FULL, none above LEG_TOP (the bottom of his straitjacket goes
 * with both), and none within LEG_MID of his middle across, all from LEG_SIDE out. His tail, behind him and down to the floor
 * (TAIL_), is his body's. A leg leans from its hip (HIP_Y) to the top of its foot (FOOT_Y), and the foot under that stays flat. */
#define HIP_Y 58.0f
#define HIP_Z 4.0f
#define FOOT_Y 12.0f
#define LEG_TOP 70.0f
#define LEG_FULL 42.0f
#define LEG_MID 5.0f
#define LEG_SIDE 16.0f
#define TAIL_X 14.0f
#define TAIL_Y 30.0f
#define TAIL_Z -15.0f

/* A stride, as each leg goes through it: on the ground for its first STEP_STANCE, its foot going back steadily from STEP_REACH in
 * front of its hip to STEP_REACH behind, then up STEP_LIFT with its knee STEP_KNEE_OUT in front, and forward again. Two strides an
 * animation, a step 5 ticks of his base's 20-tick run: his legs are short, and so many steps keep his feet up with the floor as he
 * goes (Tiny's, a stride a run, go back as fast as he moves). The body bobs, leans over the leg it's on, and turns against the legs. */
#define STEP_STRIDES 2
#define STEP_STANCE 0.6f
#define STEP_REACH 44.0f
#define STEP_LIFT 22.0f
#define STEP_KNEE_OUT 26.0f
#define STEP_BOB 4.0f
#define STEP_ROLL 0.10f
#define STEP_TWIST 0.10f

/* The spin: his jump's frame with his legs out, the turns, the hop (extraheroes.c draws a third of a frame's lift, keep_lift), the
 * frames he takes to throw his legs out and bring them in, and the one he's landed by: the base's attack shows only its first 14. */
#define SPIN_OUT 14
#define SPIN_TURNS 2
#define SPIN_LIFT 150.0f
#define SPIN_OPEN 3
#define SPIN_LAND 13
#define SPIN_BODY 70.0f /* he spins round the middle of what's above this */

/* The tornado round his spin, as Crash's and Coco's spins have (their models' second mesh), but blue and glowing: streaks of faces,
 * drawn adding their colour (a face's colour index, bit 15 and blend mode 1), rising round him from his legs to his head, wide at
 * the bottom and narrower up, each bright at its front and fading behind, whirling round faster than he turns. They're vertices of
 * his mesh like the rest, all at one point (so nothing to draw) in any other frame, and open out and fold away with his legs. */
#define TORN_STREAKS 8
#define TORN_SEGS 14
#define TORN_VERTS (TORN_STREAKS * (TORN_SEGS + 1) * 2)
#define TORN_TRIS (TORN_STREAKS * TORN_SEGS * 4) /* two a quad, each side */
#define TORN_STEP 24.0f    /* how much higher each streak starts */
#define TORN_RISE 46.0f    /* and rises along itself */
#define TORN_HALF 7.0f     /* half its width */
#define TORN_BOTTOM 126.0f /* the radius at the floor, and at TORN_TOP */
#define TORN_RADIUS_TOP 66.0f
#define TORN_TOP 210.0f
#define TORN_SPAN 3.6f     /* the way round a streak goes (radians), the way it turns a frame, and how far round each starts from the last */
#define TORN_SPEED 1.75f
#define TORN_PHASE 2.2f
#define TORN_RAMP 1.6f     /* how a streak's brightness rises to its front */
static const u8 TornRgb[3] = { 60, 150, 255 };

static V3 Rest[MAX_VERTS], LegsOut[MAX_VERTS];
static float LegShare[MAX_VERTS];
static u8 LegOf[MAX_VERTS]; /* 0 his left (-x), 1 his right */
static u8 TailOf[MAX_VERTS];
static s32 RigVerts;
static float SpinX, SpinZ;

static V3 to_rig(V3 p) {
    V3 r;

    r.x = p.x / ROO_UNIT;
    r.y = -p.y / ROO_UNIT;
    r.z = -p.z / ROO_UNIT;
    return r;
}

static V3 from_rig(V3 r) {
    V3 p;

    p.x = r.x * ROO_UNIT;
    p.y = -r.y * ROO_UNIT;
    p.z = -r.z * ROO_UNIT;
    return p;
}

/* His standing pose (sway_middle first), each vertex's share of a leg and its knee, and his jump's legs out, on the same floor. */
static void rig_setup(const C1Entry *jump, const s32 scale[3]) {
    static V3 fv[MAX_VERTS];
    float low = 1e9f, out_low = 1e9f, sx = 0, sz = 0;
    s32 k, n, f, body = 0;

    RigVerts = 0;
    if (SwayVerts == 0 || jump->count <= 0) {
        return;
    }
    for (k = 0; k < SwayVerts; k++) {
        V3 p = to_rig(SwayMiddle[k]);
        s32 tail = p.z < TAIL_Z && fabsf(p.x) < TAIL_X && p.y < TAIL_Y;

        Rest[k] = p;
        LegOf[k] = p.x < 0 ? 0 : 1;
        TailOf[k] = (u8)tail;
        LegShare[k] = tail ? 0 : hm_smooth(LEG_TOP, LEG_FULL, p.y) * hm_smooth(LEG_MID, LEG_SIDE, fabsf(p.x));
        low = p.y < low ? p.y : low;
        if (p.y > SPIN_BODY) {
            sx += p.x;
            sz += p.z;
            body++;
        }
    }
    SpinX = body > 0 ? sx / body : 0;
    SpinZ = body > 0 ? sz / body : 0;
    f = jump->count > SPIN_OUT ? SPIN_OUT : jump->count - 1;
    n = c1_frame_verts(jump->item[f], jump->size[f], scale, fv, NULL, MAX_VERTS);
    for (k = 0; k < SwayVerts; k++) {
        LegsOut[k] = k < n ? to_rig(fv[k]) : Rest[k];
        out_low = LegsOut[k].y < out_low ? LegsOut[k].y : out_low;
    }
    for (k = 0; k < SwayVerts; k++) {
        LegsOut[k].y += low - out_low;
    }
    RigVerts = SwayVerts;
}

/* Where a leg is at p (0 to 1) through its stride: its foot's way in front of its hip, and how far up it is (0 on the ground to 1). */
static void stride(float p, float *z, float *up) {
    if (p < STEP_STANCE) {
        *z = STEP_REACH * (1 - 2 * p / STEP_STANCE);
        *up = 0;
        return;
    }
    p = (p - STEP_STANCE) / (1 - STEP_STANCE);
    *z = STEP_REACH * (2 * hm_smooth(0, 1, p) - 1);
    *up = sinf(ROO_PI * p);
}

static s32 walk_verts(s32 f, V3 *out) {
    float t = (float)STEP_STRIDES * f / RooAnims[ROO_WALK].made, z[2], up[2], bob, roll, twist;
    V3 pelvis = { 0, HIP_Y, HIP_Z };
    s32 k;

    /* his right leg half a stride behind his left */
    stride(t - floorf(t), &z[0], &up[0]);
    stride(t + 0.5f - floorf(t + 0.5f), &z[1], &up[1]);
    bob = STEP_BOB * (up[0] + up[1] - 0.5f);
    roll = STEP_ROLL * (up[0] - up[1]);
    twist = STEP_TWIST * (z[0] - z[1]) / (2 * STEP_REACH);
    for (k = 0; k < RigVerts; k++) {
        V3 p = Rest[k], q, body;
        s32 leg = LegOf[k];
        float w = LegShare[k], d;

        body = hm_turn_y(hm_turn_z(p, pelvis, roll), pelvis, -twist);
        body.y += bob;
        q = hm_mix(p, body, 1 - w);
        if (w > 0) {
            /* how far down the leg, 0 at the hip to 1 at the foot and all of the foot */
            d = (HIP_Y - p.y) / (HIP_Y - FOOT_Y);
            d = d < 0 ? 0 : d > 1 ? 1 : d;
            q.y += STEP_LIFT * up[leg] * d * w;
            q.z += (z[leg] * d + STEP_KNEE_OUT * up[leg] * sinf(ROO_PI * d)) * w;
        }
        out[k] = from_rig(q);
    }
    return RigVerts;
}

/* The tornado's vertices after his n, from frame f of his spin, open of the way out and up off the ground: each streak a strip of
 * TORN_SEGS + 1 pairs, its back end first; or, shut, all at one point inside him. */
static s32 add_tornado(V3 *out, s32 n, s32 f, float open, float up) {
    s32 k, s, at = n;

    for (k = 0; k < TORN_STREAKS; k++) {
        for (s = 0; s <= TORN_SEGS; s++) {
            float u = (float)s / TORN_SEGS, a = k * TORN_PHASE + TORN_SPEED * f + TORN_SPAN * u, y = k * TORN_STEP + TORN_RISE * u;
            float r = (TORN_BOTTOM + (TORN_RADIUS_TOP - TORN_BOTTOM) * y / TORN_TOP) * open, h = TORN_HALF * open;
            V3 lo, hi;

            if (open <= 0) {
                lo.x = hi.x = SpinX;
                lo.y = hi.y = SPIN_BODY + 30;
                lo.z = hi.z = SpinZ;
            } else {
                lo.x = hi.x = SpinX + r * sinf(a);
                lo.z = hi.z = SpinZ + r * cosf(a);
                lo.y = y - h + up;
                hi.y = y + h + up;
            }
            out[at++] = from_rig(lo);
            out[at++] = from_rig(hi);
        }
    }
    return at;
}

static s32 spin_verts(s32 f, V3 *out) {
    float t = f >= SPIN_LAND ? 1 : (float)f / SPIN_LAND;
    float open = fminf(hm_smooth(0, SPIN_OPEN, (float)f), hm_smooth(SPIN_LAND, SPIN_LAND - SPIN_OPEN, (float)f));
    float turn = 2 * ROO_PI * SPIN_TURNS * hm_smooth(0, 1, t), up = SPIN_LIFT * sinf(ROO_PI * t);
    V3 axis = { SpinX, 0, SpinZ };
    s32 k;

    for (k = 0; k < RigVerts; k++) {
        V3 q = hm_turn_y(hm_mix(Rest[k], LegsOut[k], open), axis, turn);

        q.y += up;
        out[k] = from_rig(q);
    }
    return add_tornado(out, RigVerts, f, open, up);
}

/* His animation j's frames, and its frame f as c1_frame_verts gives it, then the tornado's (shut but in his spin): his standing one with
 * less sway, the made ones made. */
static s32 anim_frames(const C1Entry *a, s32 j) {
    return RooAnims[j].made > 0 && RigVerts > 0 ? RooAnims[j].made : a->count;
}

static s32 anim_verts(const C1Entry *a, s32 j, s32 f, const s32 scale[3], V3 *out, V3 *normals) {
    s32 n, k;

    if (RooAnims[j].made > 0 && RigVerts > 0) {
        return j == ROO_SPIN ? spin_verts(f, out) : add_tornado(out, walk_verts(f, out), 0, 0, 0);
    }
    f = f < a->count ? f : a->count - 1;
    n = c1_frame_verts(a->item[f], a->size[f], scale, out, normals, MAX_VERTS - TORN_VERTS);
    if (j == ROO_REST && n > 0 && n == SwayVerts) {
        for (k = 0; k < n; k++) {
            out[k].x = SwayMiddle[k].x + (out[k].x - SwayMiddle[k].x) * ROO_SWAY;
            out[k].y = SwayMiddle[k].y + (out[k].y - SwayMiddle[k].y) * ROO_SWAY;
            out[k].z = SwayMiddle[k].z + (out[k].z - SwayMiddle[k].z) * ROO_SWAY;
        }
    }
    return n > 0 && RigVerts > 0 ? add_tornado(out, n, 0, 0, 0) : n;
}

/* --- sat on a polar bear ---
 * Astride it: each thigh swung forward from its hip (SEAT_FLEX) and out over the bear's side (SEAT_SPREAD), the shin hung back toward
 * straight down from the knee (SEAT_KNEE_OUT of the spread, and SEAT_KNEE_BACK under), each long foot turned at its ankle to point
 * forward (SEAT_FOOT_TURN), and the tail swung up behind about its root to lie along the bear's back (SEAT_TAIL_UP). His hips are
 * SEAT_HIP_UP above the top of its back, SEAT_Z behind where Crash sits. As Crash leans, hips to head, he bends at his waist and neck
 * (RIDE_WAIST and RIDE_NECK, each a share of it), each vertex as much as it's above those, smoothly over RIDE_*_BLEND. */
#define SEAT_HIP_X 12.0f
#define SEAT_FLEX 0.4f
#define SEAT_SPREAD 0.9f
#define SEAT_KNEE_Y 30.0f
#define SEAT_KNEE_OUT 0.8f
#define SEAT_KNEE_BACK 0.3f
#define SEAT_ANKLE_X 22.0f
#define SEAT_ANKLE_Y 6.0f
#define SEAT_FOOT_TURN 1.2f
#define SEAT_TAIL_Y 30.0f
#define SEAT_TAIL_Z -14.0f
#define SEAT_TAIL_UP 1.25f
#define SEAT_HIP_UP 45.0f
#define SEAT_Z 28.0f
#define RIDE_WAIST 92.0f
#define RIDE_WAIST_BLEND 22.0f
#define RIDE_NECK 145.0f
#define RIDE_NECK_BLEND 12.0f
#define RIDE_LEAN_WAIST 0.6f
#define RIDE_LEAN_NECK 0.4f
#define RIDE_CHARRED 0.35f /* his colours' share when a bomb's charred him */

static V3 seated(s32 k) {
    V3 q = Rest[k];

    if (LegShare[k] > 0) {
        float side = Rest[k].x < 0 ? -1.0f : 1.0f, foot = hm_smooth(SEAT_ANKLE_Y + 4, SEAT_ANKLE_Y - 4, Rest[k].y);
        float below = hm_smooth(SEAT_KNEE_Y + 8, SEAT_KNEE_Y - 8, Rest[k].y);
        V3 hip = { side * SEAT_HIP_X, HIP_Y, HIP_Z }, ankle = { side * SEAT_ANKLE_X, SEAT_ANKLE_Y, 0 };
        V3 knee = { side * SEAT_HIP_X, SEAT_KNEE_Y, HIP_Z }, r, shin;

        if (foot > 0) {
            q = hm_mix(q, hm_turn_y(q, ankle, -side * SEAT_FOOT_TURN), foot);
        }
        /* the thigh, from the hip; then the shin, from where the knee's gone */
        r = hm_turn_x(hm_turn_z(q, hip, side * SEAT_SPREAD), hip, -SEAT_FLEX);
        knee = hm_turn_x(hm_turn_z(knee, hip, side * SEAT_SPREAD), hip, -SEAT_FLEX);
        shin = hm_turn_x(hm_turn_z(r, knee, -side * SEAT_SPREAD * SEAT_KNEE_OUT), knee, SEAT_KNEE_BACK);
        r = hm_mix(r, shin, below);
        q = hm_mix(q, r, LegShare[k]);
    }
    if (TailOf[k]) {
        V3 root = { 0, SEAT_TAIL_Y, SEAT_TAIL_Z };

        q = hm_turn_x(q, root, SEAT_TAIL_UP);
    }
    return q;
}

/* What riding needs of his making: his frames for his own motion over it */
static struct {
    const C1Entry *anims;
    s32 scale[3];
    float k;
    const float *share;
} RideOwn;

/* Over the riding: his standing sway as he breathes, and his laugh as he taunts and wins, round and round, his body above his waist */
static void ride_own(void *ctx, s32 anim, s32 f, s32 frames, V3 *verts) {
    static V3 fv[MAX_VERTS];
    const C1Entry *a;
    float amount = 1;
    s32 g, n, k;

    if (anim == RIDE_BREATHE) {
        a = &RideOwn.anims[ROO_REST];
        g = frames > 0 ? f * a->count / frames : 0;
        amount = ROO_SWAY;
    } else if (anim == RIDE_TAUNT || anim == RIDE_WIN) {
        a = &RideOwn.anims[ROO_LAUGH];
        g = a->count > 0 ? f % a->count : 0;
    } else {
        return;
    }
    n = c1_frame_verts(a->item[g], a->size[g], RideOwn.scale, fv, NULL, MAX_VERTS);
    for (k = 0; k < n && k < RigVerts && k < SwayVerts; k++) {
        float w = RideOwn.share[k] * amount * RideOwn.k;

        verts[k].x += (fv[k].x - SwayMiddle[k].x) * w;
        verts[k].y += (fv[k].y - SwayMiddle[k].y) * w;
        verts[k].z += (fv[k].z - SwayMiddle[k].z) * w;
    }
}

/* --- making him --- */

typedef struct {
    const C1Entry *anims;
    s32 scale[3];
    float k, ground; /* his units to Crash Bash's, and how far down his feet are then */
} FootCtx;

static void foot_frame(void *ctx, s32 j, s32 f, V3 *out) {
    static V3 fv[MAX_VERTS];
    const FootCtx *c = ctx;
    s32 n = anim_verts(&c->anims[j], j, f, c->scale, fv, NULL), k, total = RigVerts > 0 ? RigVerts + TORN_VERTS : n;

    for (k = 0; k < total; k++) {
        const V3 *p = &fv[k < n ? k : 0];

        out[k].x = p->x * c->k;
        out[k].y = p->y * c->k - c->ground;
        out[k].z = p->z * c->k;
    }
}

/* His sounds, which his boss fight's object (RRooC) plays: sound entries in sound pages, at 11025 Hz. His laugh (CrznA), his jump's
 * call (rjpnA), and his landings, light (ld30A) and heavy (ld00A). His game has no hurt voice for him, so his yelp is his laugh's first
 * "ha", its first ROO_YELP_BYTES (0.22 s, where it falls quiet), a little higher. */
#define ROO_PITCH 0x400
#define ROO_YELP_BYTES (86 * 16)
#define ROO_YELP_PITCH 0x480
#define CARD_HEAD 42 /* his hero card: his head and the tongue as it leaves his mouth; then the tongue hanging down and curling at its end */
static u8 *RooSound[NUM_VOICES];
static s32 RooSoundBytes[NUM_VOICES];
static u32 RooCard[64 * 64];

static void make_sounds(const C1Level *l, HeroMade *out) {
    static const struct {
        s32 voice;
        const char *entry;
    } Sounds[] = { { VOICE_LAUGH, "CrznA" }, { VOICE_JUMP, "rjpnA" }, { VOICE_LAND, "ld30A" }, { VOICE_THUD, "ld00A" } };
    s32 i, size;

    for (i = 0; i < (s32)(sizeof(Sounds) / sizeof(Sounds[0])); i++) {
        s32 v = Sounds[i].voice;

        if (RooSound[v] == NULL) {
            size = 0;
            RooSound[v] = c1_sound(l, Sounds[i].entry, &size);
            RooSoundBytes[v] = RooSound[v] != NULL ? size : 0;
        }
        out->sound[v].adpcm = RooSound[v];
        out->sound[v].bytes = RooSoundBytes[v];
        out->sound[v].pitch = ROO_PITCH;
    }
    out->sound[VOICE_YELP] = out->sound[VOICE_LAUGH];
    out->sound[VOICE_YELP].bytes = out->sound[VOICE_LAUGH].bytes < ROO_YELP_BYTES ? out->sound[VOICE_LAUGH].bytes : ROO_YELP_BYTES;
    out->sound[VOICE_YELP].pitch = ROO_YELP_PITCH;
}

static s32 roo_make(HeroMade *out) {
    static V3 verts[MAX_VERTS], normals[MAX_VERTS], seat[MAX_VERTS];
    static MadeFace faces[MAX_TRIS];
    static MadeImage images[MADE_MAX_IMAGES];
    static MadeAnim made_anims[ROO_ANIMS];
    static float waist_share[MAX_VERTS], neck_share[MAX_VERTS];
    DiscImage *disc;
    C1Level level;
    C1Entry model, anims[ROO_ANIMS];
    FootCtx foot;
    MadeModel mm;
    Rider rider;
    s32 nvert, ntri = 0, own_tris, nimage = 0, i, j, k, ok = 0;
    float lo = 1e9f, hi = -1e9f, back_y, seat_z;

    disc = disc_image_find(HeroRoo.disc);
    if (disc == NULL) {
        return 0;
    }
    if (!c1_level_read(disc, ROO_LEVEL, &level)) {
        printf("bonus: Crash Bandicoot's Ripper Roo level can't be read\n");
        disc_image_close(disc);
        return 0;
    }
    disc_image_close(disc);
    if (!c1_entry_find(&level, "RS1nG", &model) || model.type != 2 || model.count < 2) {
        printf("bonus: Crash Bandicoot's disc has no Ripper Roo\n");
        goto done;
    }
    for (i = 0; i < ROO_ANIMS; i++) {
        if (!c1_entry_find(&level, RooAnims[i].entry, &anims[i]) || anims[i].type != 1 || anims[i].count <= 0) {
            printf("bonus: Ripper Roo's %s is missing\n", RooAnims[i].entry);
            goto done;
        }
    }
    c1_model_scale(&model, foot.scale);
    foot.anims = anims;
    /* how big he is, standing, and his rig */
    sway_middle(&anims[ROO_REST], foot.scale);
    rig_setup(&anims[ROO_SPIN], foot.scale);
    nvert = anim_verts(&anims[ROO_REST], ROO_REST, 0, foot.scale, verts, normals);
    if (nvert == 0) {
        goto done;
    }
    for (k = 0; k < nvert; k++) {
        lo = verts[k].y < lo ? verts[k].y : lo;
        hi = verts[k].y > hi ? verts[k].y : hi;
    }
    foot.k = ROO_HEIGHT / (hi - lo);
    foot.ground = hi * foot.k;

    /* his triangles, their colours lit as he stands, and their textures; then the tornado's, on its vertices after his (add_tornado):
     * each quad two faces a side, so it shows from in and out, each corner bright as far as it is to its streak's front */
    if (!c1_model_faces(&level, &model, normals, nvert, faces, MAX_TRIS - TORN_TRIS, &ntri, images, &nimage)) {
        goto done;
    }
    own_tris = ntri;
    if (RigVerts > 0 && nvert == RigVerts + TORN_VERTS) {
        s32 streak, seg, q, c;

        for (streak = 0; streak < TORN_STREAKS; streak++) {
            for (seg = 0; seg < TORN_SEGS; seg++) {
                /* this pair's low and high, then the next pair's */
                s32 v0 = RigVerts + (streak * (TORN_SEGS + 1) + seg) * 2;
                const s32 quad[4][3] = {
                    { v0, v0 + 2, v0 + 3 }, { v0, v0 + 3, v0 + 1 }, { v0, v0 + 3, v0 + 2 }, { v0, v0 + 1, v0 + 3 },
                };

                for (q = 0; q < 4; q++) {
                    MadeFace *t = &faces[ntri++];

                    memset(t, 0, sizeof(*t));
                    t->image = -1;
                    /* added, semi-transparent (bit 15) in blend mode 1 */
                    t->blend = 0x8000 | 1 << 13;
                    for (j = 0; j < 3; j++) {
                        float b = powf((float)((quad[q][j] - RigVerts) / 2 % (TORN_SEGS + 1)) / TORN_SEGS, TORN_RAMP);

                        t->v[j] = quad[q][j];
                        for (c = 0; c < 3; c++) {
                            t->rgb[0][j][c] = t->rgb[1][j][c] = (u8)(TornRgb[c] * b);
                        }
                    }
                }
            }
        }
    }

    /* on foot */
    for (j = 0; j < ROO_ANIMS; j++) {
        made_anims[j].name = RooAnims[j].name;
        made_anims[j].frames = anim_frames(&anims[j], j);
        made_anims[j].mesh = 0;
    }
    memset(&mm, 0, sizeof(mm));
    mm.faces = faces;
    mm.nfaces = ntri;
    mm.meshes = 1;
    mm.images = images;
    mm.nimages = nimage;
    mm.anims = made_anims;
    mm.nanims = ROO_ANIMS;
    mm.nvert = nvert;
    mm.rest_anim = ROO_REST;
    mm.frame = foot_frame;
    mm.ctx = &foot;
    if (!made_write(&mm, &out->tex[KIND_FOOT], &out->tex_size[KIND_FOOT], &out->mdl[KIND_FOOT], &out->mdl_size[KIND_FOOT])) {
        goto done;
    }

    /* riding a polar bear: sat on its back as he stands, his own faces (not the tornado's) */
    if (RigVerts > 0 && ride_seat(&back_y, &seat_z)) {
        float s = ROO_UNIT * foot.k;
        V3 waist = { 0, RIDE_WAIST, 0 }, neck = { 0, RIDE_NECK, 0 };

        for (k = 0; k < RigVerts; k++) {
            V3 p = from_rig(seated(k));

            seat[k].x = p.x * foot.k;
            seat[k].y = p.y * foot.k + HIP_Y * s - SEAT_HIP_UP + back_y;
            seat[k].z = p.z * foot.k + seat_z + SEAT_Z;
            waist_share[k] = hm_smooth(RIDE_WAIST - RIDE_WAIST_BLEND, RIDE_WAIST + RIDE_WAIST_BLEND, Rest[k].y);
            neck_share[k] = hm_smooth(RIDE_NECK - RIDE_NECK_BLEND, RIDE_NECK + RIDE_NECK_BLEND, Rest[k].y);
        }
        waist = from_rig(waist);
        neck = from_rig(neck);
        memset(&rider, 0, sizeof(rider));
        rider.seat = seat;
        rider.nvert = RigVerts;
        rider.waist_share = waist_share;
        rider.neck_share = neck_share;
        rider.waist.x = waist.x * foot.k;
        rider.waist.y = waist.y * foot.k + HIP_Y * s - SEAT_HIP_UP + back_y;
        rider.waist.z = waist.z * foot.k + seat_z + SEAT_Z;
        rider.neck.x = neck.x * foot.k;
        rider.neck.y = neck.y * foot.k + HIP_Y * s - SEAT_HIP_UP + back_y;
        rider.neck.z = neck.z * foot.k + seat_z + SEAT_Z;
        rider.lean_waist = RIDE_LEAN_WAIST;
        rider.lean_neck = RIDE_LEAN_NECK;
        rider.faces = faces;
        rider.nfaces = own_tris;
        rider.images = images;
        rider.nimages = nimage;
        rider.charred = RIDE_CHARRED;
        rider.own = ride_own;
        RideOwn.anims = anims;
        memcpy(RideOwn.scale, foot.scale, sizeof(RideOwn.scale));
        RideOwn.k = foot.k;
        RideOwn.share = waist_share;
        if (!ride_make(&rider, &out->tex[KIND_POLAR], &out->tex_size[KIND_POLAR], &out->mdl[KIND_POLAR], &out->mdl_size[KIND_POLAR])) {
            printf("bonus: Ripper Roo has no polar bear to ride\n");
        }
    }
    if (c1_sprite64(&level, "ShadC", 5, RooCard)) {
        out->card = RooCard;
        out->card_w = out->card_h = 64;
        out->card_head = CARD_HEAD;
    }
    make_sounds(&level, out);
    printf("bonus: Ripper Roo from Crash Bandicoot, %d triangles, %d textures, %d bytes%s%s%s%s\n", ntri, nimage, out->mdl_size[KIND_FOOT],
           out->mdl[KIND_POLAR] != NULL ? ", riding a bear" : "", out->card != NULL ? ", his hero card" : "",
           out->sound[VOICE_LAUGH].adpcm != NULL ? ", his laugh" : "", out->sound[VOICE_THUD].adpcm != NULL ? ", his landing" : "");
    ok = 1;
done:
    made_images_free(images, nimage);
    free(level.pages);
    return ok;
}
