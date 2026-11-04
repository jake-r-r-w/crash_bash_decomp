/* Ripper Roo (bash/herodef.h), from Crash Bandicoot (USA, SCUS-94900): the extra hero the others follow. docs/EXTRA_HEROES.md goes
 * through this file a part at a time, for making another.
 *
 * Level 0x17, his boss fight, has his model (RS1nG) and four animations, RS1nV (a big jump), RS2nV (standing), RS3nV (a laugh) and RS4nV
 * (knocked over), which become JUMP, BREATHE, TAUNT and DIE; two more are made for him from them, a walk of small bouncy hops (WALK),
 * and a spin attack, his legs out as he spins in a blue tornado (SPIN). Riding a polar bear, he's sat astride Crash's and moves as Crash
 * does on it (bonusheroes.c, ride_make), swaying as he stands and laughing as he taunts and wins. His sounds are his boss fight's: his
 * laugh, his jump and his landings. His hero card is the face that fight shows, whose tongue is his health. */
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
 * His base is Rilla Roo, Kanga inside, if nobody else is playing her. On foot he stands still holding a crate (BANK), hops where the
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
/* On his cane his model has Pogo's moves by name, as long as his base's; his tazing is the boss's attack, at its own speed */
/* In a car his model has Ballistix's moves by name, in a tank Tank's, and in a race cart Crash Dash's; he sways in it as he drives at
 * his own pace */
static const Stand RooCar[] = {
    { NULL, NULL, STRETCH },
};
static const Stand RooTank[] = {
    { NULL, NULL, STRETCH },
};
static const Stand RooRace[] = {
    { "BREATHE", "BREATHE", LOOP },
    { NULL, NULL, STRETCH },
};
/* Riding a fish his model has Splash Dash's moves by name */
static const Stand RooSplash[] = {
    { NULL, NULL, STRETCH },
};
static const Stand RooPogo[] = {
    { "TAZING", "TAZING", LOOP },
    { NULL, NULL, STRETCH },
};

/* His sounds for his base's (docs/EXTRA_HEROES.md, A hero's sounds): as he wins the announcer names him "Rilla Roo", the nearest name the
 * game has (RILLA_NAME), whoever's place he has (with Coco's he "won" as Coco), then "wins!", and he laughs as he cheers; he laughs as
 * he taunts, yelps only as he's hurt, gives his jump's call as he jumps and lunges, thuds as he lands, and throws with Crash's throw
 * (his game has none of his own). Picking up a crate, carrying it and pushing he's quiet. */
static const Voice RooVoices[] = {
    { 0, VOICE_NAME }, { 1, VOICE_LAUGH }, { 2, VOICE_YELP }, { 3, VOICE_YELP }, { 4, VOICE_YELP }, { 5, VOICE_YELP },
    { 6, VOICE_LAUGH },
    /* Crate Crush's */
    { 9, VOICE_NONE }, { 10, VOICE_NONE }, { 11, VOICE_JUMP }, { 12, VOICE_THUD }, { 13, VOICE_JUMP }, { 14, VOICE_NONE },
    { 15, VOICE_THROW }, { 16, VOICE_NONE },
    /* Polar Push's */
    { 19, VOICE_NONE }, { 20, VOICE_NONE }, { 21, VOICE_JUMP }, { 22, VOICE_NONE }, { 23, VOICE_NONE }, { 24, VOICE_LAUGH },
    { 25, VOICE_NONE },
    { -1, 0 },
};

static s32 roo_make(HeroMade *out);

#define ROO_KEEP_LIFT 0x500 /* on foot, the share of a frame's lift off the floor that's drawn: his own jump is a crate or more high */
#define ROO_SUBFRAMES 4     /* on foot, his model's frames to his game's, blended between: his base's moves are longer than his, and
                             * without them he'd hold some of his frames for two of theirs, stepping (at 120 frames a second, slowly) */

const HeroDef HeroRoo = {
    "RIPPER ROO", "roo", "SCUS-94900", Kanga,
    { { RooStands, "BREATHE", ROO_KEEP_LIFT, 0, ROO_SUBFRAMES }, { RooRides, "BREATHE", 0x1000, Crash, 1 },
      { RooPogo, "BREATHE", 0x1000, 0, ROO_SUBFRAMES }, { RooCar, "BREATHE", 0x1000, 0, 1 },
      { RooTank, "BREATHE", 0x1000, Dingodile, 1 }, { RooRace, "BREATHE", 0x1000, 0, 1 }, { RooSplash, "SWIM", 0x1000, 0, 1 } },
    /* in a car, a tank, a race cart and on a fish a frame of his to each of his base's (VEHICLE_PER) */
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
#define ROO_DIE 3
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
 * SwayMiddle. WALK hops, small and bouncy, as a kangaroo goes. SPIN, his attack: he throws his legs out to the sides as his
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

/* His walk, a kangaroo's: small hops on both feet together, HOP_HOPS an animation (a hop every 10 ticks of his base's 20-tick run).
 * Each hop his feet are on the floor for HOP_CONTACT of it, going back flat under him from HOP_REACH in front to HOP_REACH behind as
 * he goes over them, his body sinking HOP_CROUCH as he lands and springing up again; then he's in the air, as high as HOP_SEEN looks
 * once less of his lift is drawn (ROO_KEEP_LIFT), his feet tucked HOP_TUCK up under him and swinging forward to land, his knees out
 * in front (HOP_KNEE), and his body tipped HOP_LEAN forward. */
#define HOP_HOPS 2
#define HOP_CONTACT 0.3f
#define HOP_REACH 22.0f
#define HOP_CROUCH 6.0f
#define HOP_SEEN 55.0f
#define HOP_TUCK 10.0f
#define HOP_KNEE 12.0f
#define HOP_LEAN 0.12f
static float HopLift; /* HOP_SEEN in his units, before less of it's drawn: roo_make works it out */

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

static s32 hop_verts(s32 f, V3 *out) {
    float t = (float)HOP_HOPS * f / RooAnims[ROO_WALK].made, p = t - floorf(t), up = 0, crouch = 0, feet, tuck = 0, lean = 0;
    V3 pelvis = { 0, HIP_Y, HIP_Z };
    s32 k;

    if (p < HOP_CONTACT) {
        /* on the floor: his feet going back under him as he goes over them, his body sinking and springing up */
        float c = p / HOP_CONTACT;

        feet = HOP_REACH * (1 - 2 * c);
        crouch = HOP_CROUCH * sinf(ROO_PI * c);
    } else {
        /* in the air: up and down again, his feet swinging forward from behind to land, tucked up under him on the way */
        float a = (p - HOP_CONTACT) / (1 - HOP_CONTACT), arc = sinf(ROO_PI * a);

        up = HopLift * arc;
        feet = HOP_REACH * (2 * hm_smooth(0, 1, a) - 1);
        tuck = HOP_TUCK * arc;
        lean = HOP_LEAN * arc;
    }
    for (k = 0; k < RigVerts; k++) {
        V3 r = Rest[k], q, body;
        float w = LegShare[k], d;

        body = hm_turn_x(r, pelvis, lean);
        body.y += up - crouch;
        r.y += up - crouch;
        q = hm_mix(r, body, 1 - w);
        if (w > 0) {
            /* how far down the leg, 0 at the hip to 1 at the foot and all of the foot: the feet stay on the floor as he sinks */
            d = (HIP_Y - Rest[k].y) / (HIP_Y - FOOT_Y);
            d = d < 0 ? 0 : d > 1 ? 1 : d;
            q.y += (crouch + tuck) * d * w;
            q.z += (feet * d + HOP_KNEE * (tuck / HOP_TUCK) * sinf(ROO_PI * d)) * w;
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
        return j == ROO_SPIN ? spin_verts(f, out) : add_tornado(out, hop_verts(f, out), 0, 0, 0);
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
 * Astride it as on a horse: each thigh swung forward from its hip (SEAT_FLEX) and out over the bear's side (SEAT_SPREAD), the knee
 * bent so the shin goes back under it (SEAT_KNEE_BACK) and toward straight down (SEAT_KNEE_OUT of the spread), the long feet poking
 * out at the sides as he stands on them, tipped a little down at the ankle (SEAT_FOOT_DROOP), and the tail tucked away into its root,
 * down inside the bear (SEAT_TAIL_), as behind him it would only cut through the bear's rump. His hips are SEAT_HIP_UP above the top
 * of its back, low enough that he sits down
 * into its fur, SEAT_Z behind where Crash sits. He moves as the bear's back does, and as Crash leans on his, hips to head, he bends at
 * his waist and neck (RIDE_WAIST and RIDE_NECK, a share of it at each), each vertex as much as it's above those, smoothly over
 * RIDE_*_BLEND, keeping RIDE_STEADY back toward upright. His tongue and ears swing as his head's thrown about (FLOP_). */
#define SEAT_HIP_X 12.0f
#define SEAT_FLEX 0.7f
#define SEAT_SPREAD 0.9f
#define SEAT_KNEE_Y 30.0f
#define SEAT_KNEE_OUT 0.8f
#define SEAT_KNEE_BACK 0.9f
#define SEAT_ANKLE_X 22.0f
#define SEAT_ANKLE_Y 6.0f
#define SEAT_FOOT_DROOP 0.3f
#define SEAT_TAIL_Y 30.0f
#define SEAT_TAIL_Z -14.0f
#define SEAT_HIP_UP 5.0f
#define SEAT_Z 5.0f
#define RIDE_WAIST 92.0f
#define RIDE_WAIST_BLEND 22.0f
#define RIDE_NECK 145.0f
#define RIDE_NECK_BLEND 12.0f
#define RIDE_LEAN_WAIST 0.8f
#define RIDE_LEAN_NECK 0.2f
#define RIDE_STEADY 0.3f /* of Crash's tilt from upright, the share he keeps back from, steadier as he's taller */
#define RIDE_CHARRED 0.35f /* his colours' share when a bomb's charred him */

/* Astride: how far his thighs go out over its sides (spread), and how far of that his shins come back in under them (knee_out) */
static V3 astride(s32 k, float spread, float knee_out) {
    V3 q = Rest[k];

    if (LegShare[k] > 0) {
        float side = Rest[k].x < 0 ? -1.0f : 1.0f, foot = hm_smooth(SEAT_ANKLE_Y + 4, SEAT_ANKLE_Y - 4, Rest[k].y);
        float below = hm_smooth(SEAT_KNEE_Y + 8, SEAT_KNEE_Y - 8, Rest[k].y);
        V3 hip = { side * SEAT_HIP_X, HIP_Y, HIP_Z }, ankle = { side * SEAT_ANKLE_X, SEAT_ANKLE_Y, 0 };
        V3 knee = { side * SEAT_HIP_X, SEAT_KNEE_Y, HIP_Z }, r, shin;

        /* the foot, out at his side, tipped down a little */
        if (foot > 0) {
            q = hm_mix(q, hm_turn_z(q, ankle, -side * SEAT_FOOT_DROOP), foot);
        }
        /* the thigh, from the hip; then the shin, from where the knee's gone */
        r = hm_turn_x(hm_turn_z(q, hip, side * spread), hip, -SEAT_FLEX);
        knee = hm_turn_x(hm_turn_z(knee, hip, side * spread), hip, -SEAT_FLEX);
        shin = hm_turn_x(hm_turn_z(r, knee, -side * spread * knee_out), knee, SEAT_KNEE_BACK);
        r = hm_mix(r, shin, below);
        q = hm_mix(q, r, LegShare[k]);
    }
    if (TailOf[k]) {
        /* all of it at its root: its faces have no size there, and the ones joining it to him go into the bear */
        q.x = 0;
        q.y = SEAT_TAIL_Y;
        q.z = SEAT_TAIL_Z;
    }
    return q;
}

/* On the bear */
static V3 seated(s32 k) {
    return astride(k, SEAT_SPREAD, SEAT_KNEE_OUT);
}

/* What's loose on him: his tongue, the faces of his model that are its reds and pinks, more of it the further it's out of his mouth
 * (FLOP_TONGUE_ROOT to FLOP_TONGUE_TIP forward, standing); and his ears, out to the sides of his head above FLOP_EAR_LOW, more the
 * further out (FLOP_EAR_ROOT to FLOP_EAR_TIP), FLOP_EAR of the tongue's. They swing on a spring at his head as it's thrown about, as he
 * hops, jumps, spins and laughs and as the bear bounces him (RooSwing): FLOP_PERIOD frames a swing, FLOP_DAMPING of what would stop
 * it, taking FLOP_GAIN of the throws, FLOP_MAX at most. */
#define FLOP_TONGUE_ROOT 10.0f
#define FLOP_TONGUE_TIP 38.0f
#define FLOP_EAR_LOW 178.0f
#define FLOP_EAR_ROOT 28.0f
#define FLOP_EAR_TIP 58.0f
#define FLOP_EAR 0.8f
#define FLOP_PERIOD 9.0f
#define FLOP_DAMPING 0.25f
#define FLOP_GAIN 1.0f
#define FLOP_MAX 60.0f
static const Flop RooSwing = { FLOP_PERIOD, FLOP_DAMPING, FLOP_GAIN, FLOP_MAX };
static float FlopShare[MAX_VERTS];
static float TongueShare[MAX_VERTS]; /* of it, his tongue's: none at its root, all at its tip */

static void flop_setup(const C1Entry *model) {
    static u8 red[MAX_VERTS];
    const u8 *info = model->item[0], *polys = model->item[1];
    s32 ntri = (s32)hm_rd32(info), nslot = (s32)hm_rd32(info + 16), i, j, k;

    memset(red, 0, sizeof(red));
    for (i = 0; i < ntri && (i + 1) * 8 <= model->size[1]; i++) {
        const u8 *p = polys + i * 8, *s;
        s32 slot = hm_rd16(p + 6) & 0x7FFF;

        if (slot >= nslot || 20 + (slot + 1) * 4 > model->size[0]) {
            continue;
        }
        s = info + 20 + slot * 4;
        if (s[0] > 100 && s[0] > 1.5f * s[1] && s[0] > 1.5f * s[2]) {
            for (j = 0; j < 3; j++) {
                s32 v = hm_rd16(p + j * 2) / 6;

                if (v < MAX_VERTS) {
                    red[v] = 1;
                }
            }
        }
    }
    for (k = 0; k < RigVerts; k++) {
        float t = red[k] ? hm_smooth(FLOP_TONGUE_ROOT, FLOP_TONGUE_TIP, Rest[k].z) : 0;
        float e = Rest[k].y > FLOP_EAR_LOW ? FLOP_EAR * hm_smooth(FLOP_EAR_ROOT, FLOP_EAR_TIP, fabsf(Rest[k].x)) : 0;

        FlopShare[k] = t > e ? t : e;
        TongueShare[k] = t;
    }
}

/* What riding needs of his making: his frames for his own motion over it */
static struct {
    const C1Entry *anims;
    s32 scale[3];
    float k;
    const float *share;
} RideOwn;

/* Over the riding, his body above his waist: his standing sway as he breathes (RIDE_SWAY of it, the same share of the way round),
 * and his laugh as he taunts and wins (RIDE_LAUGH of it, at its own speed, round and round), each between two of its frames, and
 * the laugh coming in and going out over RIDE_FADE frames, so neither starts or stops with a jump. */
#define RIDE_SWAY 0.12f
#define RIDE_LAUGH 0.2f
#define RIDE_FADE 4.0f

static void ride_own(void *ctx, s32 anim, s32 f, s32 frames, V3 *verts) {
    static V3 fv[MAX_VERTS], gv[MAX_VERTS];
    const C1Entry *a;
    float amount, t, u;
    s32 g0, g1, n, m, k;

    if (anim == RIDE_BREATHE) {
        a = &RideOwn.anims[ROO_REST];
        t = frames > 0 ? (float)f * a->count / frames : 0;
        amount = RIDE_SWAY;
    } else if (anim == RIDE_TAUNT || anim == RIDE_WIN) {
        a = &RideOwn.anims[ROO_LAUGH];
        t = (float)f;
        amount = RIDE_LAUGH * fminf(hm_smooth(0, RIDE_FADE, (float)f), hm_smooth((float)(frames - 1), frames - 1 - RIDE_FADE, (float)f));
    } else {
        return;
    }
    if (a->count <= 0 || amount <= 0) {
        return;
    }
    g0 = (s32)floorf(t);
    u = t - g0;
    g0 %= a->count;
    g1 = (g0 + 1) % a->count;
    n = c1_frame_verts(a->item[g0], a->size[g0], RideOwn.scale, fv, NULL, MAX_VERTS);
    m = c1_frame_verts(a->item[g1], a->size[g1], RideOwn.scale, gv, NULL, MAX_VERTS);
    for (k = 0; k < n && k < m && k < RigVerts && k < SwayVerts; k++) {
        float w = RideOwn.share[k] * amount * RideOwn.k;

        verts[k].x += (fv[k].x + (gv[k].x - fv[k].x) * u - SwayMiddle[k].x) * w;
        verts[k].y += (fv[k].y + (gv[k].y - fv[k].y) * u - SwayMiddle[k].y) * w;
        verts[k].z += (fv[k].z + (gv[k].z - fv[k].z) * u - SwayMiddle[k].z) * w;
    }
}

/* --- making him --- */

typedef struct {
    const C1Entry *anims;
    s32 scale[3];
    float k, ground; /* his units to Crash Bash's, and how far down his feet are then */
} FootCtx;

/* On foot, the swing of what's loose on him through each of his animations' frames (his game's), from his head's path (his vertices
 * above FOOT_HEAD_Y, standing) as it's seen: with the lift a frame's drawn without taken off it (ROO_KEEP_LIFT). Round and round in
 * the ones that go round (FootLoops: his breathing, his laugh, his walk). */
#define FOOT_HEAD_Y 150.0f
#define MAX_OWN_FRAMES 32 /* his animations' frames (his jump's 28 the most) */
static const u8 FootLoops[ROO_ANIMS] = { 0, 1, 1, 0, 1, 0 };
static V3 FootSwing[ROO_ANIMS][MAX_OWN_FRAMES];

static void foot_swing(const FootCtx *c) {
    static V3 fv[MAX_VERTS], path[MAX_OWN_FRAMES];
    float keep = (float)ROO_KEEP_LIFT / 0x1000;
    s32 j, f, k, n, nv;

    memset(FootSwing, 0, sizeof(FootSwing));
    for (j = 0; j < ROO_ANIMS; j++) {
        n = anim_frames(&c->anims[j], j);
        n = n > MAX_OWN_FRAMES ? MAX_OWN_FRAMES : n;
        for (f = 0; f < n; f++) {
            V3 sum = { 0, 0, 0 };
            float w = 0, low = -1e9f;

            nv = anim_verts(&c->anims[j], j, f, c->scale, fv, NULL);
            for (k = 0; k < nv && k < RigVerts; k++) {
                float y = fv[k].y * c->k - c->ground;

                low = y > low ? y : low;
                if (Rest[k].y > FOOT_HEAD_Y) {
                    sum.x += fv[k].x * c->k;
                    sum.y += y;
                    sum.z += fv[k].z * c->k;
                    w++;
                }
            }
            if (w > 0) {
                sum.x /= w;
                sum.y /= w;
                sum.z /= w;
            }
            /* drawn lower by part of its lift off the floor (extraheroes.c, extra_hero_shape) */
            if (low < 0) {
                sum.y += -low * (1 - keep);
            }
            path[f] = sum;
        }
        hm_flop(path, n, FootLoops[j], &RooSwing, FootSwing[j]);
    }
}

static void foot_frame(void *ctx, s32 j, s32 f, V3 *out) {
    static V3 fv[MAX_VERTS];
    const FootCtx *c = ctx;
    s32 own = f / ROO_SUBFRAMES, n = anim_verts(&c->anims[j], j, own, c->scale, fv, NULL), k;
    s32 total = RigVerts > 0 ? RigVerts + TORN_VERTS : n;
    V3 swing = own < MAX_OWN_FRAMES ? FootSwing[j][own] : FootSwing[j][MAX_OWN_FRAMES - 1];

    for (k = 0; k < total; k++) {
        const V3 *p = &fv[k < n ? k : 0];
        float w = k < RigVerts ? FlopShare[k] : 0;

        out[k].x = p->x * c->k + swing.x * w;
        out[k].y = p->y * c->k - c->ground + swing.y * w;
        out[k].z = p->z * c->k + swing.z * w;
    }
}

/* --- on his cane (Pogo) ---
 * Pogo's heroes bounce on pogo sticks, a model each with its stick (models\chars\pogo\). Where Ripper Roo is the arena's boss
 * (models\arena\pogo_epl\roo2.mdl, DB_Roo, from the player's archive) he bounces on a stick of his own: a cane that telescopes,
 * squeezed short between his feet as he lands, shooting out to throw him high, and left below as he flies, to land on again. Roo's
 * pogo model is that cane, moving as it does there (the boss model's strips all within CANE_HALF of its middle across and below
 * CANE_TOP, as it rests), and Roo on it, going up and down as the boss does (the middle of the boss's body, his corners above BOSS_BODY
 * as he rests, from where it is at his bounce's bottom, his BREATHE's first frame): his feet on the cane's top (perched) as it squeezes
 * and shoots out, and once the boss goes higher than it, flying above it, his legs thrown out as in his own jump's splits (LegsOut,
 * from POGO_OPEN_FROM above the cane, all the way from POGO_OPEN_FULL) and in again to land on it; his tongue and ears swinging. Pogo's
 * moves (PogoMoves): his bounce, the boss's; his hop to the next square, the boss's, eased from and back to his bounce's bottom over
 * POGO_EASE of it at each end, as his base's hop lands on it; dazed, he staggers through his knocked-over animation's frames before he
 * falls (to POGO_DAZE_LAST), stood on the squeezed cane; as he wins, he laughs through a bounce; and his tazing is the boss's attack,
 * the cane flipped up and fired. */
#define CANE_HALF 16.0f
#define CANE_TOP -400.0f
#define BOSS_BODY -300.0f
#define POGO_OPEN_FROM 15.0f
#define POGO_OPEN_FULL 140.0f
#define POGO_FOOT_TURN 1.0f
#define POGO_ANKLE_X 8.0f
#define POGO_EASE 0.25f
#define POGO_DAZE_LAST 10
#define POGO_MAX_TICKS 40
#define POGO_MAX_CORNERS 1024

enum { POSE_BOUNCE, POSE_LAUGH, POSE_DAZED };
typedef struct {
    const char *name; /* Pogo's (its heroes' pogo models') */
    const char *boss; /* the boss's it moves as, */
    s32 from, to;     /* from and to which of its frames */
    s32 own_speed;    /* at the boss's speed, a frame of his a tick (a LOOP stand-in); else as long as his base's */
    s32 loops;
    s32 ease;
    s32 pose;
} PogoMove;

static const PogoMove PogoMoves[] = {
    { "BREATHE", "BREATHE", 0, 13, 0, 1, 0, POSE_BOUNCE }, { "JUMP", "JUMP", 0, 13, 0, 0, 1, POSE_BOUNCE },
    { "DAZED", "BREATHE", 0, 0, 0, 0, 0, POSE_DAZED },     { "WIN", "BREATHE", 0, 13, 0, 0, 0, POSE_LAUGH },
    { "TAZING", "ATTACK", 0, 20, 1, 0, 0, POSE_BOUNCE },
};
#define POGO_MOVES 5

typedef struct {
    const FootCtx *foot;
    GamePart cane;
    s32 boss[POGO_MOVES], bottom; /* the boss's animations, and his BREATHE */
    float bottom_y;               /* the middle of his body at his bounce's bottom */
    MadeAnim anims[POGO_MOVES];
    V3 swing[POGO_MOVES][POGO_MAX_TICKS];
} PogoCtx;

static PogoCtx Pogo;
static V3 PogoOut[MAX_VERTS]; /* LegsOut with his body as high as he stands */

static s32 keep_cane(void *ctx, const V3 *rest, s32 n, s32 textured) {
    s32 k;

    (void)ctx;
    (void)textured;
    for (k = 0; k < n; k++) {
        if (fabsf(rest[k].x) > CANE_HALF || rest[k].y < CANE_TOP) {
            return 0;
        }
    }
    return 1;
}

/* The middle of the boss's body (not the cane) in his corners */
static float boss_body_y(const PogoCtx *c, const V3 *corners) {
    float sum = 0;
    s32 k, n = 0;

    for (k = 0; k < c->cane.corners; k++) {
        if (!c->cane.in_part[k] && c->cane.rest[k].y < BOSS_BODY) {
            sum += corners[k].y;
            n++;
        }
    }
    return n > 0 ? sum / n : 0;
}

/* How high the cane's top is off the floor */
static float cane_top(const PogoCtx *c, const V3 *corners) {
    float top = 0;
    s32 k;

    for (k = 0; k < c->cane.nvert; k++) {
        float y = -corners[c->cane.corner_of[k]].y;

        top = k == 0 || y > top ? y : top;
    }
    return top;
}

/* A vertex of his (in his rig) stood on the cane, `amount` of the way: each foot turned in about its ankle toward forward
 * (POGO_FOOT_TURN) and each leg drawn in, more of it further down, till the ankles are POGO_ANKLE_X across, so his long feet,
 * splayed out as he stands, go side by side over the crook */
static V3 perched(V3 q, s32 k, float amount) {
    if (LegShare[k] > 0 && amount > 0) {
        float side = Rest[k].x < 0 ? -1.0f : 1.0f, foot = hm_smooth(FOOT_Y + 4, FOOT_Y - 2, Rest[k].y) * amount;
        float d = (HIP_Y - Rest[k].y) / (HIP_Y - SEAT_ANKLE_Y);
        V3 ankle = { side * SEAT_ANKLE_X, 0, 0 };

        d = d < 0 ? 0 : d > 1 ? 1 : d;
        if (foot > 0) {
            q = hm_turn_y(q, ankle, -side * POGO_FOOT_TURN * foot);
        }
        q.x -= side * (SEAT_ANKLE_X - POGO_ANKLE_X) * d * LegShare[k] * amount;
    }
    return q;
}

/* Frame f of move a, but for the swing: Roo (RigVerts) and the boss's corners */
static void pogo_pose(const PogoCtx *c, s32 a, s32 f, V3 *roo, V3 *corners) {
    static V3 fv[MAX_VERTS], posed[MAX_VERTS], bottom[POGO_MAX_CORNERS];
    const PogoMove *m = &PogoMoves[a];
    const FootCtx *foot = c->foot;
    s32 frames = c->anims[a].frames, k, n = 0, g;
    float phase, t, lift, ease = 0, top, feet, open, sole;

    if (m->own_speed) {
        t = m->from + (float)f / ROO_SUBFRAMES;
        t = t > m->to ? (float)m->to : t;
        phase = m->to > m->from ? (t - m->from) / (m->to - m->from) : 0;
    } else {
        phase = m->loops ? (float)f / frames : frames > ROO_SUBFRAMES ? (float)f / (frames - ROO_SUBFRAMES) : 0;
        phase = phase > 1 ? 1 : phase;
        t = m->from + (m->to - m->from) * phase;
    }
    if (!hm_model_at(&c->cane, c->boss[a], t, corners)) {
        hm_model_at(&c->cane, c->bottom, 0, corners);
    }
    lift = c->bottom_y - boss_body_y(c, corners);
    if (m->ease) {
        ease = 1 - fminf(hm_smooth(0, POGO_EASE, phase), hm_smooth(1, 1 - POGO_EASE, phase));
        if (ease > 0 && hm_model_at(&c->cane, c->bottom, 0, bottom)) {
            for (k = 0; k < c->cane.corners; k++) {
                corners[k] = hm_mix(corners[k], bottom[k], ease);
            }
            lift *= 1 - ease;
        }
    }
    /* his feet on the cane's top, or as high as the boss goes once he's above it */
    top = cane_top(c, corners);
    feet = lift > top ? lift : top;
    open = hm_smooth(POGO_OPEN_FROM, POGO_OPEN_FULL, feet - top);
    switch (m->pose) {
    case POSE_LAUGH:
        g = f / ROO_SUBFRAMES % (foot->anims[ROO_LAUGH].count > 0 ? foot->anims[ROO_LAUGH].count : 1);
        n = anim_verts(&foot->anims[ROO_LAUGH], ROO_LAUGH, g, foot->scale, fv, NULL);
        break;
    case POSE_DAZED:
        g = (s32)(phase * POGO_DAZE_LAST + 0.5f);
        n = anim_verts(&foot->anims[ROO_DIE], ROO_DIE, g, foot->scale, fv, NULL);
        break;
    default:
        n = 0;
    }
    /* his soles where he stands, or a frame of his own game's own lowest: either way on the cane's top as he's on it */
    sole = foot->ground;
    for (k = 0; k < RigVerts; k++) {
        if (n > 0) {
            posed[k] = from_rig(perched(to_rig(fv[k < n ? k : 0]), k, 1));
            sole = k == 0 || posed[k].y * foot->k > sole ? posed[k].y * foot->k : sole;
        } else {
            posed[k] = from_rig(perched(hm_mix(Rest[k], PogoOut[k], open), k, 1 - open));
        }
    }
    for (k = 0; k < RigVerts; k++) {
        roo[k].x = posed[k].x * foot->k;
        roo[k].y = posed[k].y * foot->k - sole - feet;
        roo[k].z = posed[k].z * foot->k;
    }
}

/* The swing of his tongue and ears through each move, a tick at a time (every ROO_SUBFRAMES of its frames), from his head's path */
static void pogo_swing(PogoCtx *c) {
    static V3 roo[MAX_VERTS], corners[POGO_MAX_CORNERS], path[POGO_MAX_TICKS];
    s32 a, i, k, ticks;

    memset(c->swing, 0, sizeof(c->swing));
    for (a = 0; a < POGO_MOVES; a++) {
        ticks = (c->anims[a].frames + ROO_SUBFRAMES - 1) / ROO_SUBFRAMES;
        ticks = ticks > POGO_MAX_TICKS ? POGO_MAX_TICKS : ticks;
        for (i = 0; i < ticks; i++) {
            V3 sum = { 0, 0, 0 };
            float w = 0;

            pogo_pose(c, a, i * ROO_SUBFRAMES, roo, corners);
            for (k = 0; k < RigVerts; k++) {
                if (Rest[k].y > FOOT_HEAD_Y) {
                    sum.x += roo[k].x;
                    sum.y += roo[k].y;
                    sum.z += roo[k].z;
                    w++;
                }
            }
            if (w > 0) {
                sum.x /= w;
                sum.y /= w;
                sum.z /= w;
            }
            path[i] = sum;
        }
        hm_flop(path, ticks, PogoMoves[a].loops, &RooSwing, c->swing[a]);
    }
}

static void pogo_frame(void *ctx, s32 a, s32 f, V3 *out) {
    static V3 corners[POGO_MAX_CORNERS];
    const PogoCtx *c = ctx;
    s32 ticks = (c->anims[a].frames + ROO_SUBFRAMES - 1) / ROO_SUBFRAMES, i0, i1, k;
    float s = (float)f / ROO_SUBFRAMES, u;
    V3 swing;

    ticks = ticks > POGO_MAX_TICKS ? POGO_MAX_TICKS : ticks;
    pogo_pose(c, a, f, out, corners);
    i0 = (s32)floorf(s);
    u = s - i0;
    i0 = i0 < ticks ? i0 : ticks - 1;
    i1 = i0 + 1 < ticks ? i0 + 1 : PogoMoves[a].loops ? 0 : i0;
    swing = hm_mix(c->swing[a][i0], c->swing[a][i1], u);
    for (k = 0; k < RigVerts; k++) {
        out[k].x += swing.x * FlopShare[k];
        out[k].y += swing.y * FlopShare[k];
        out[k].z += swing.z * FlopShare[k];
    }
    for (k = 0; k < c->cane.nvert; k++) {
        out[RigVerts + k] = corners[c->cane.corner_of[k]];
    }
}

/* His pogo model: his own faces (own_tris of them, on his RigVerts) and images, then the cane's */
static s32 pogo_make(const FootCtx *foot, const MadeFace *faces, s32 own_tris, const MadeImage *images, s32 nimage, HeroMade *out) {
    static V3 corners[POGO_MAX_CORNERS];
    PogoCtx *c = &Pogo;
    MadeFace *all = NULL;
    MadeImage *imgs = NULL;
    MadeModel mm;
    s32 a, k, j, n, ok = 0;
    float raise = 0, rest_y = 0, out_y = 0;

    memset(c, 0, sizeof(*c));
    c->foot = foot;
    if (RigVerts == 0 || !hm_game_part(DB_Roo, keep_cane, NULL, &c->cane)) {
        printf("bonus: Pogo's Ripper Roo has no cane\n");
        return 0;
    }
    if (c->cane.corners > POGO_MAX_CORNERS || (c->bottom = hm_model_anim(&c->cane, "BREATHE", NULL)) < 0
        || !hm_model_at(&c->cane, c->bottom, 0, corners)) {
        goto done;
    }
    c->bottom_y = boss_body_y(c, corners);
    for (a = 0; a < POGO_MOVES; a++) {
        const PogoMove *m = &PogoMoves[a];

        if ((c->boss[a] = hm_model_anim(&c->cane, m->boss, NULL)) < 0) {
            goto done;
        }
        /* as long as his base's, each frame of it ROO_SUBFRAMES of his; or at the boss's speed */
        n = m->own_speed ? m->to - m->from + 1 : hm_bank_frames(DB_CharsPogoRilla, m->name);
        n = n > 0 ? n : m->to - m->from + 1;
        n = n > POGO_MAX_TICKS ? POGO_MAX_TICKS : n;
        c->anims[a].name = m->name;
        c->anims[a].frames = n * ROO_SUBFRAMES;
        c->anims[a].mesh = 0;
        c->anims[a].step = ROO_SUBFRAMES;
    }
    /* his legs-out pose with his body (all but his legs and tail) where it is as he stands */
    for (k = 0, n = 0; k < RigVerts; k++) {
        if (LegShare[k] <= 0 && !TailOf[k]) {
            rest_y += Rest[k].y;
            out_y += LegsOut[k].y;
            n++;
        }
    }
    raise = n > 0 ? (rest_y - out_y) / n : 0;
    for (k = 0; k < RigVerts; k++) {
        PogoOut[k] = LegsOut[k];
        PogoOut[k].y += raise;
    }
    pogo_swing(c);
    all = malloc(sizeof(MadeFace) * (size_t)(own_tris + c->cane.nfaces));
    imgs = malloc(sizeof(MadeImage) * (size_t)(nimage + c->cane.nimages + 1));
    if (all == NULL || imgs == NULL) {
        goto done;
    }
    memcpy(all, faces, sizeof(MadeFace) * (size_t)own_tris);
    for (k = 0; k < c->cane.nfaces; k++) {
        MadeFace *f = &all[own_tris + k];

        *f = c->cane.faces[k];
        for (j = 0; j < 3; j++) {
            f->v[j] += RigVerts;
        }
        f->image = f->image >= 0 ? f->image + nimage : -1;
    }
    memcpy(imgs, images, sizeof(MadeImage) * (size_t)nimage);
    memcpy(imgs + nimage, c->cane.images, sizeof(MadeImage) * (size_t)c->cane.nimages);
    memset(&mm, 0, sizeof(mm));
    mm.faces = all;
    mm.nfaces = own_tris + c->cane.nfaces;
    mm.meshes = 1;
    mm.images = imgs;
    mm.nimages = nimage + c->cane.nimages;
    mm.anims = c->anims;
    mm.nanims = POGO_MOVES;
    mm.nvert = RigVerts + c->cane.nvert;
    mm.rest_anim = 0;
    mm.frame = pogo_frame;
    mm.ctx = c;
    ok = made_write(&mm, &out->tex[KIND_POGO], &out->tex_size[KIND_POGO], &out->mdl[KIND_POGO], &out->mdl_size[KIND_POGO]);
done:
    if (!ok) {
        printf("bonus: Ripper Roo can't be put on his cane\n");
    }
    free(all);
    free(imgs);
    hm_part_free(&c->cane);
    return ok;
}

/* --- in a Ballistix car ---
 * Ballistix draws a car round each hero (its vehicle model's meshes 4 and 1) and the hero's own model in it (models\chars\ball\), the
 * four of them round the arena, so each is seen from a side of its own: from behind, the front and either side. The car's handles are
 * the heroes' models' own (all the same: Rilla Roo's, her strips all CAR_HANDLE_X or more out to the sides, no higher than
 * CAR_HANDLE_TOP and within CAR_HANDLE_Z of the middle along), so his model has them too, rocking as hers do as she steers, though he
 * can't hold them. Roo sits in it as big as the others, CAR_SCALE of his size about his hips, which are CAR_HIPS above its floor and
 * CAR_Z back in its seat: his thighs
 * forward (CAR_FLEX), his shins down (CAR_KNEE_BACK), his long feet turned in under him as on his cane, his tail tucked into its root,
 * all of that inside the car and his straitjacket and head above it. With no hands for its handles, he drives it by smacking his head
 * down on its side, the way he goes (BANK: CAR_LEAN at his waist and CAR_NOD more at his neck, all of it from CAR_SMACK of the way
 * there); he sways as he breathes (CAR_SWAY of his standing animation's), laughs as he fidgets, taunts and wins (CAR_FIDGET, CAR_LAUGH
 * and all of it, bouncing CAR_HOP in his seat as he wins), and slumps back as he loses (CAR_SLUMP, his head dropping CAR_DROOP). His
 * tongue and ears swing, but as he drives: its frames are the way he leans, not time. */
#define CAR_HANDLE_X 210.0f
#define CAR_HANDLE_TOP -250.0f
#define CAR_HANDLE_Z 30.0f
#define CAR_SCALE 1.25f
#define CAR_HIPS 40.0f
#define CAR_Z 170.0f
#define CAR_FLEX 1.45f
#define CAR_KNEE_Y 30.0f
#define CAR_KNEE_BACK 1.45f
#define CAR_LEAN 0.8f
#define CAR_NOD 1.18f
#define CAR_SMACK 0.75f
#define CAR_SWAY 0.3f
#define CAR_FIDGET 0.4f
#define CAR_LAUGH 0.6f
#define CAR_HOP 40.0f
#define CAR_SLUMP 0.55f
#define CAR_DROOP 0.5f
#define CAR_FADE 4.0f
#define CAR_MAX_TICKS 96 /* his base's longest move (Rilla Roo's IDLE_A is 75 frames) */
/* In a car or a tank, his model's frames to each of his base's: one, as his moves there are worked out for each frame of theirs (they
 * don't step as his game's own frames stretched over theirs would), and an animation can't be longer than MADE_MAX_FRAMES */
#define VEHICLE_PER 1

enum { CAR_BREATHE, CAR_BANK, CAR_IDLE, CAR_TAUNT, CAR_DIE, CAR_WIN, CAR_MOVES };
static const char *const CarNames[CAR_MOVES] = { "BREATHE", "BANK", "IDLE_A", "TAUNT_A", "DIE", "WIN" };

typedef struct {
    const FootCtx *foot;
    MadeAnim anims[CAR_MOVES];
    V3 swing[CAR_MOVES][CAR_MAX_TICKS];
    GamePart handles;
    s32 handle_anim[CAR_MOVES], handle_rest; /* Rilla Roo's animations they move with, by the same names, and her BREATHE */
    V3 *handle_at;                            /* where their corners are */
} CarCtx;

static CarCtx Car;

static s32 keep_handles(void *ctx, const V3 *rest, s32 n, s32 textured) {
    s32 k;

    (void)ctx;
    (void)textured;
    for (k = 0; k < n; k++) {
        if (fabsf(rest[k].x) < CAR_HANDLE_X || rest[k].y < CAR_HANDLE_TOP || fabsf(rest[k].z) > CAR_HANDLE_Z) {
            return 0;
        }
    }
    return 1;
}

/* A vertex of his (in his rig) sat in the car, legs and tail; above his waist he's as he stands */
static V3 car_seated(V3 q, s32 k) {
    if (LegShare[k] > 0) {
        float side = Rest[k].x < 0 ? -1.0f : 1.0f, foot = hm_smooth(FOOT_Y + 4, FOOT_Y - 2, Rest[k].y);
        float below = hm_smooth(CAR_KNEE_Y + 8, CAR_KNEE_Y - 8, Rest[k].y);
        V3 hip = { side * SEAT_HIP_X, HIP_Y, HIP_Z }, knee = { side * SEAT_HIP_X, CAR_KNEE_Y, HIP_Z }, ankle = { side * SEAT_ANKLE_X, 0, 0 };
        V3 r, shin;

        if (foot > 0) {
            q = hm_turn_y(q, ankle, -side * POGO_FOOT_TURN * foot);
        }
        r = hm_turn_x(q, hip, -CAR_FLEX);
        knee = hm_turn_x(knee, hip, -CAR_FLEX);
        shin = hm_turn_x(r, knee, CAR_KNEE_BACK);
        r = hm_mix(r, shin, below);
        q = hm_mix(q, r, LegShare[k]);
    }
    if (TailOf[k]) {
        q.x = 0;
        q.y = SEAT_TAIL_Y;
        q.z = SEAT_TAIL_Z;
    }
    return q;
}

/* The way he leans in the car's BANK frame g of n (its base's): none at its first, middle and last, his head down on one side by a
 * quarter of the way and on the other by three quarters, as the base's leans (the game holds frame 9 or 23 of 28 as he goes one way or
 * the other, and comes back upright as he stops) */
static float car_bank(float g, s32 n) {
    float half = n / 2.0f, s;

    if (g <= half) {
        s = -sinf(ROO_PI * g / half);
    } else {
        s = n - 1 > half ? sinf(ROO_PI * (g - half) / (n - 1 - half)) : 0;
    }
    s /= CAR_SMACK;
    return s > 1 ? 1 : s < -1 ? -1 : s;
}

/* Add `amount` of his own animation a's motion above his waist at t of his frames (round and round), blended between two of them,
 * to his rig's vertices */
static void car_motion(const FootCtx *foot, s32 a, float t, float amount, V3 *q) {
    static V3 fv[MAX_VERTS], gv[MAX_VERTS];
    const C1Entry *e = &foot->anims[a];
    s32 g0, g1, n, m, k;
    float u;

    if (e->count <= 0 || amount <= 0 || SwayVerts == 0) {
        return;
    }
    g0 = (s32)floorf(t);
    u = t - g0;
    g0 = (g0 % e->count + e->count) % e->count;
    g1 = (g0 + 1) % e->count;
    n = c1_frame_verts(e->item[g0], e->size[g0], foot->scale, fv, NULL, MAX_VERTS);
    m = c1_frame_verts(e->item[g1], e->size[g1], foot->scale, gv, NULL, MAX_VERTS);
    for (k = 0; k < n && k < m && k < RigVerts && k < SwayVerts; k++) {
        float w = hm_smooth(RIDE_WAIST - RIDE_WAIST_BLEND, RIDE_WAIST + RIDE_WAIST_BLEND, Rest[k].y) * amount;
        V3 d = to_rig(hm_mix(fv[k], gv[k], u)), s = to_rig(SwayMiddle[k]);

        q[k].x += (d.x - s.x) * w;
        q[k].y += (d.y - s.y) * w;
        q[k].z += (d.z - s.z) * w;
    }
}

/* Frame f of move a, but for the swing: his vertices in Crash Bash's units, in the car */
static void car_pose(const CarCtx *c, s32 a, s32 f, V3 *out) {
    static V3 q[MAX_VERTS];
    const FootCtx *foot = c->foot;
    s32 frames = c->anims[a].frames, k;
    float g = (float)f / VEHICLE_PER, ticks = (float)frames / VEHICLE_PER, lean = 0, slump = 0, hop = 0, fade, s;
    V3 waist = { 0, RIDE_WAIST, 0 }, neck = { 0, RIDE_NECK, 0 };

    for (k = 0; k < RigVerts; k++) {
        q[k] = car_seated(Rest[k], k);
    }
    fade = fminf(hm_smooth(0, CAR_FADE, g), hm_smooth(ticks - 1, ticks - 1 - CAR_FADE, g));
    switch (a) {
    case CAR_BREATHE:
        car_motion(foot, ROO_REST, g * foot->anims[ROO_REST].count / (ticks > 0 ? ticks : 1), CAR_SWAY, q);
        break;
    case CAR_BANK:
        lean = car_bank(g, (s32)(ticks + 0.5f));
        break;
    case CAR_IDLE:
        car_motion(foot, ROO_LAUGH, g, CAR_FIDGET * fade, q);
        break;
    case CAR_TAUNT:
        car_motion(foot, ROO_LAUGH, g, CAR_LAUGH * fade, q);
        break;
    case CAR_WIN:
        car_motion(foot, ROO_LAUGH, g, fade, q);
        hop = CAR_HOP * fabsf(sinf(ROO_PI * g / 8)) * fade;
        break;
    case CAR_DIE:
        slump = hm_smooth(0, ticks * 0.5f, g);
        break;
    }
    for (k = 0; k < RigVerts; k++) {
        float up = hm_smooth(RIDE_WAIST - RIDE_WAIST_BLEND, RIDE_WAIST + RIDE_WAIST_BLEND, Rest[k].y);
        float head = hm_smooth(RIDE_NECK - RIDE_NECK_BLEND, RIDE_NECK + RIDE_NECK_BLEND, Rest[k].y);
        V3 p = q[k];

        if (up > 0 && lean != 0) {
            /* down on the car's side, his head the most */
            V3 at_neck = hm_turn_z(neck, waist, -lean * CAR_LEAN);

            p = hm_mix(p, hm_turn_z(p, waist, -lean * CAR_LEAN), up);
            p = hm_mix(p, hm_turn_z(p, at_neck, -lean * CAR_NOD), head);
        }
        if (up > 0 && slump > 0) {
            /* back in his seat, his head dropping */
            V3 at_neck = hm_turn_x(neck, waist, -slump * CAR_SLUMP);

            p = hm_mix(p, hm_turn_x(p, waist, -slump * CAR_SLUMP), up);
            p = hm_mix(p, hm_turn_x(p, at_neck, slump * CAR_DROOP), head);
        }
        s = ROO_UNIT * foot->k * CAR_SCALE;
        p.y += hop / s * up;
        out[k].x = p.x * s;
        out[k].y = -(p.y - HIP_Y) * s - CAR_HIPS;
        out[k].z = -(p.z - HIP_Z) * s + CAR_Z;
    }
}

/* The swing of his tongue and ears through each move but BANK, a tick at a time, from his head's path */
static void car_swing(CarCtx *c) {
    static V3 v[MAX_VERTS], path[CAR_MAX_TICKS];
    s32 a, i, k, ticks;

    memset(c->swing, 0, sizeof(c->swing));
    for (a = 0; a < CAR_MOVES; a++) {
        if (a == CAR_BANK) {
            continue;
        }
        ticks = (c->anims[a].frames + VEHICLE_PER - 1) / VEHICLE_PER;
        ticks = ticks > CAR_MAX_TICKS ? CAR_MAX_TICKS : ticks;
        for (i = 0; i < ticks; i++) {
            V3 sum = { 0, 0, 0 };
            float w = 0;

            car_pose(c, a, i * VEHICLE_PER, v);
            for (k = 0; k < RigVerts; k++) {
                if (Rest[k].y > FOOT_HEAD_Y) {
                    sum.x += v[k].x;
                    sum.y += v[k].y;
                    sum.z += v[k].z;
                    w++;
                }
            }
            if (w > 0) {
                sum.x /= w;
                sum.y /= w;
                sum.z /= w;
            }
            path[i] = sum;
        }
        hm_flop(path, ticks, a == CAR_BREATHE, &RooSwing, c->swing[a]);
    }
}

static void car_frame(void *ctx, s32 a, s32 f, V3 *out) {
    const CarCtx *c = ctx;
    s32 ticks = (c->anims[a].frames + VEHICLE_PER - 1) / VEHICLE_PER, i0, i1, k;
    float s = (float)f / VEHICLE_PER, u;
    V3 swing;

    ticks = ticks > CAR_MAX_TICKS ? CAR_MAX_TICKS : ticks;
    car_pose(c, a, f, out);
    i0 = (s32)floorf(s);
    u = s - i0;
    i0 = i0 < ticks ? i0 : ticks - 1;
    i1 = i0 + 1 < ticks ? i0 + 1 : a == CAR_BREATHE ? 0 : i0;
    swing = hm_mix(c->swing[a][i0], c->swing[a][i1], u);
    for (k = 0; k < RigVerts; k++) {
        out[k].x += swing.x * FlopShare[k];
        out[k].y += swing.y * FlopShare[k];
        out[k].z += swing.z * FlopShare[k];
    }
    /* the handles, as they move in Rilla Roo's same move at the same frame of hers (they rock as she steers) */
    if (c->handle_at != NULL) {
        s32 h = c->handle_anim[a] >= 0 ? c->handle_anim[a] : c->handle_rest;

        if (!hm_model_at(&c->handles, h, h == c->handle_anim[a] ? s : 0, c->handle_at)) {
            hm_model_at(&c->handles, c->handle_rest, 0, c->handle_at);
        }
        for (k = 0; k < c->handles.nvert; k++) {
            out[RigVerts + k] = c->handle_at[c->handles.corner_of[k]];
        }
    }
}

/* His car model: his own faces (own_tris of them) and images, and the handles; Ballistix's moves as long as his base's */
static s32 car_make(const FootCtx *foot, const MadeFace *faces, s32 own_tris, const MadeImage *images, s32 nimage, HeroMade *out) {
    CarCtx *c = &Car;
    MadeFace *all = NULL;
    MadeImage *imgs = NULL;
    MadeModel mm;
    s32 a, n, k, j, ok = 0, breathe, nh = 0, nhi = 0;

    memset(c, 0, sizeof(*c));
    c->foot = foot;
    for (a = 0; a < CAR_MOVES; a++) {
        n = hm_bank_frames(DB_CharsBallRilla, CarNames[a]);
        n = n > 0 ? n : 20;
        n = n > CAR_MAX_TICKS ? CAR_MAX_TICKS : n;
        c->anims[a].name = CarNames[a];
        c->anims[a].frames = n * VEHICLE_PER;
        c->anims[a].mesh = 0;
        c->anims[a].step = VEHICLE_PER;
    }
    /* the handles, moving as Rilla Roo's do */
    if (hm_game_part(DB_CharsBallRilla, keep_handles, NULL, &c->handles)) {
        if ((breathe = hm_model_anim(&c->handles, "BREATHE", NULL)) >= 0
            && (c->handle_at = malloc(sizeof(V3) * (size_t)c->handles.corners)) != NULL
            && hm_model_at(&c->handles, breathe, 0, c->handle_at)) {
            c->handle_rest = breathe;
            for (a = 0; a < CAR_MOVES; a++) {
                c->handle_anim[a] = hm_model_anim(&c->handles, CarNames[a], NULL);
            }
            nh = c->handles.nfaces;
            nhi = c->handles.nimages;
        } else {
            free(c->handle_at);
            c->handle_at = NULL;
        }
    }
    if (nh == 0) {
        printf("bonus: Rilla Roo's car has no handles\n");
    }
    car_swing(c);
    all = malloc(sizeof(MadeFace) * (size_t)(own_tris + nh));
    imgs = malloc(sizeof(MadeImage) * (size_t)(nimage + nhi + 1));
    if (all == NULL || imgs == NULL) {
        goto done;
    }
    memcpy(all, faces, sizeof(MadeFace) * (size_t)own_tris);
    for (k = 0; k < nh; k++) {
        MadeFace *f = &all[own_tris + k];

        *f = c->handles.faces[k];
        for (j = 0; j < 3; j++) {
            f->v[j] += RigVerts;
        }
        f->image = f->image >= 0 ? f->image + nimage : -1;
    }
    memcpy(imgs, images, sizeof(MadeImage) * (size_t)nimage);
    memcpy(imgs + nimage, c->handles.images, sizeof(MadeImage) * (size_t)nhi);
    memset(&mm, 0, sizeof(mm));
    mm.faces = all;
    mm.nfaces = own_tris + nh;
    mm.meshes = 1;
    mm.images = imgs;
    mm.nimages = nimage + nhi;
    mm.anims = c->anims;
    mm.nanims = CAR_MOVES;
    mm.nvert = RigVerts + (nh > 0 ? c->handles.nvert : 0);
    mm.rest_anim = CAR_BREATHE;
    mm.frame = car_frame;
    mm.ctx = c;
    ok = made_write(&mm, &out->tex[KIND_BALL], &out->tex_size[KIND_BALL], &out->mdl[KIND_BALL], &out->mdl_size[KIND_BALL]);
done:
    if (!ok) {
        printf("bonus: Ripper Roo can't be put in a car\n");
    }
    free(all);
    free(imgs);
    free(c->handle_at);
    c->handle_at = NULL;
    hm_part_free(&c->handles);
    return ok;
}

/* --- in a tank (Tank) ---
 * Tank draws each hero's tank round it (the hull and its weapon's turret, from the tank objects model, or the swamp's hover tank) and
 * the hero's own model stood in the turret's hatch (models\chars\tank\), as big as on foot, from the waist up out of it. Roo stands in
 * it as on foot, his rig's TANK_CUT (just below his straitjacket) at the turret's top, TANK_HATCH up; what of him is below that, his
 * legs, feet and tail, isn't drawn (it's all at TANK_HIDDEN, inside), as the others' models have nothing below the hatch either; and
 * nothing of him lower than TANK_FLOOR, inside the hull. His moves (TankNames, each as long as Rilla Roo's): he
 * sways as he breathes (TANK_SWAY of his standing animation's); looks about as he waits (IDLE1: his head turning TANK_LOOK each way,
 * laughing TANK_FIDGET of his laugh); with no hands to drop a mine he ducks into the hatch for it and pops up again (MINE: TANK_DUCK
 * down, springing up TANK_POP past where he stood); he's thrown back as he's hit (HIT: TANK_RECOIL at his waist, wobbling upright
 * again); slumps down over the turret as he loses, folded over its front (DIE: TANK_SLUMP at his waist, TANK_DROOP more at his neck,
 * TANK_SINK down onto it), his head hanging over the edge; and laughs
 * as he wins, bouncing TANK_HOP in the hatch. His tongue and ears swing. He plays as Dingodile does (his kind's like): the
 * flamethrower and its speed, whoever's place he has (Rilla Roo's has the same). */
#define TANK_CUT 64.0f
#define TANK_HATCH 301.0f
#define TANK_FLOOR 20.0f
#define TANK_HIDDEN 30.0f
#define TANK_SWAY 0.3f
#define TANK_LOOK 0.6f
#define TANK_FIDGET 0.3f
#define TANK_DUCK 200.0f
#define TANK_POP 30.0f
#define TANK_RECOIL 0.5f
#define TANK_SLUMP 1.45f
#define TANK_DROOP 0.8f
#define TANK_SINK 25.0f
#define TANK_HOP 40.0f
#define TANK_FADE 4.0f
#define TANK_MAX_TICKS 96

enum { TANK_BREATHE, TANK_IDLE, TANK_MINE, TANK_HIT, TANK_DIE, TANK_WIN, TANK_MOVES };
static const char *const TankNames[TANK_MOVES] = { "BREATHE", "IDLE1", "MINE", "HIT", "DIE", "WIN" };

typedef struct {
    const FootCtx *foot;
    MadeAnim anims[TANK_MOVES];
    V3 swing[TANK_MOVES][TANK_MAX_TICKS];
} TankCtx;

static TankCtx Tank;

/* Frame f of move a, but for the swing: his vertices in Crash Bash's units, in the hatch */
static void tank_pose(const TankCtx *c, s32 a, s32 f, V3 *out) {
    static V3 q[MAX_VERTS];
    const FootCtx *foot = c->foot;
    s32 frames = c->anims[a].frames, k;
    float g = (float)f / VEHICLE_PER, ticks = (float)frames / VEHICLE_PER, last = ticks > 1 ? ticks - 1 : 1;
    float t = g / last, fade, look = 0, back = 0, slump = 0, lift = 0, s = ROO_UNIT * foot->k;
    V3 waist = { 0, RIDE_WAIST, 0 }, neck = { 0, RIDE_NECK, 0 };

    for (k = 0; k < RigVerts; k++) {
        /* what of him is below the hatch, his legs, feet and tail, isn't drawn: all of it at one point inside the turret, so its
         * faces have no size (his long feet would poke out through the hull) */
        if (TailOf[k] || Rest[k].y < TANK_CUT) {
            q[k].x = 0;
            q[k].y = TANK_HIDDEN;
            q[k].z = 0;
        } else {
            q[k] = Rest[k];
        }
    }
    fade = fminf(hm_smooth(0, TANK_FADE, g), hm_smooth(last, last - TANK_FADE, g));
    switch (a) {
    case TANK_BREATHE:
        car_motion(foot, ROO_REST, g * foot->anims[ROO_REST].count / (ticks > 0 ? ticks : 1), TANK_SWAY, q);
        break;
    case TANK_IDLE:
        car_motion(foot, ROO_LAUGH, g, TANK_FIDGET * fade, q);
        look = TANK_LOOK * sinf(2 * ROO_PI * t) * fade;
        break;
    case TANK_MINE:
        /* down quickly, a moment there, and up again past where he stood, settling */
        lift = -TANK_DUCK * fminf(hm_smooth(0, 0.25f, t), hm_smooth(0.7f, 0.45f, t)) + TANK_POP * sinf(ROO_PI * hm_smooth(0.6f, 1, t));
        break;
    case TANK_HIT:
        /* thrown back, and wobbling upright again */
        back = TANK_RECOIL * hm_smooth(0, 2, g) * expf(-3 * t) * cosf(3 * ROO_PI * t);
        break;
    case TANK_DIE:
        slump = hm_smooth(0, last * 0.5f, g);
        lift = -TANK_SINK * slump;
        break;
    case TANK_WIN:
        car_motion(foot, ROO_LAUGH, g, fade, q);
        lift = TANK_HOP * fabsf(sinf(ROO_PI * g / 8)) * fade;
        break;
    }
    for (k = 0; k < RigVerts; k++) {
        float up = hm_smooth(RIDE_WAIST - RIDE_WAIST_BLEND, RIDE_WAIST + RIDE_WAIST_BLEND, Rest[k].y);
        float head = hm_smooth(RIDE_NECK - RIDE_NECK_BLEND, RIDE_NECK + RIDE_NECK_BLEND, Rest[k].y);
        V3 p = q[k];

        if (head > 0 && look != 0) {
            p = hm_mix(p, hm_turn_y(p, neck, look), head);
        }
        if (up > 0 && back != 0) {
            p = hm_mix(p, hm_turn_x(p, waist, -back), up);
        }
        if (up > 0 && slump > 0) {
            /* forward at his waist, his head hanging */
            V3 at_neck = hm_turn_x(neck, waist, slump * TANK_SLUMP);

            p = hm_mix(p, hm_turn_x(p, waist, slump * TANK_SLUMP), up);
            p = hm_mix(p, hm_turn_x(p, at_neck, slump * TANK_DROOP), head);
        }
        out[k].x = p.x * s;
        out[k].y = -(p.y - TANK_CUT) * s - TANK_HATCH - lift;
        out[k].z = -p.z * s;
        /* nothing through the hull's floor: what of his legs would go through it lies on it, inside */
        if (out[k].y > -TANK_FLOOR) {
            out[k].y = -TANK_FLOOR;
        }
    }
}

/* The swing of his tongue and ears through each move, a tick at a time, from his head's path */
static void tank_swing(TankCtx *c) {
    static V3 v[MAX_VERTS], path[TANK_MAX_TICKS];
    s32 a, i, k, ticks;

    memset(c->swing, 0, sizeof(c->swing));
    for (a = 0; a < TANK_MOVES; a++) {
        ticks = (c->anims[a].frames + VEHICLE_PER - 1) / VEHICLE_PER;
        ticks = ticks > TANK_MAX_TICKS ? TANK_MAX_TICKS : ticks;
        for (i = 0; i < ticks; i++) {
            V3 sum = { 0, 0, 0 };
            float w = 0;

            tank_pose(c, a, i * VEHICLE_PER, v);
            for (k = 0; k < RigVerts; k++) {
                if (Rest[k].y > FOOT_HEAD_Y) {
                    sum.x += v[k].x;
                    sum.y += v[k].y;
                    sum.z += v[k].z;
                    w++;
                }
            }
            if (w > 0) {
                sum.x /= w;
                sum.y /= w;
                sum.z /= w;
            }
            path[i] = sum;
        }
        hm_flop(path, ticks, a == TANK_BREATHE, &RooSwing, c->swing[a]);
    }
}

static void tank_frame(void *ctx, s32 a, s32 f, V3 *out) {
    const TankCtx *c = ctx;
    s32 ticks = (c->anims[a].frames + VEHICLE_PER - 1) / VEHICLE_PER, i0, i1, k;
    float s = (float)f / VEHICLE_PER, u;
    V3 swing;

    ticks = ticks > TANK_MAX_TICKS ? TANK_MAX_TICKS : ticks;
    tank_pose(c, a, f, out);
    i0 = (s32)floorf(s);
    u = s - i0;
    i0 = i0 < ticks ? i0 : ticks - 1;
    i1 = i0 + 1 < ticks ? i0 + 1 : a == TANK_BREATHE ? 0 : i0;
    swing = hm_mix(c->swing[a][i0], c->swing[a][i1], u);
    for (k = 0; k < RigVerts; k++) {
        out[k].x += swing.x * FlopShare[k];
        out[k].y += swing.y * FlopShare[k];
        out[k].z += swing.z * FlopShare[k];
    }
}

/* His tank model: his own faces (own_tris of them) and images, Tank's moves as long as his base's */
static s32 tank_make(const FootCtx *foot, const MadeFace *faces, s32 own_tris, const MadeImage *images, s32 nimage, HeroMade *out) {
    TankCtx *c = &Tank;
    MadeModel mm;
    s32 a, n;

    memset(c, 0, sizeof(*c));
    c->foot = foot;
    for (a = 0; a < TANK_MOVES; a++) {
        n = hm_bank_frames(DB_CharsTankRilla, TankNames[a]);
        n = n > 0 ? n : 20;
        n = n > TANK_MAX_TICKS ? TANK_MAX_TICKS : n;
        c->anims[a].name = TankNames[a];
        c->anims[a].frames = n * VEHICLE_PER;
        c->anims[a].mesh = 0;
        c->anims[a].step = VEHICLE_PER;
    }
    tank_swing(c);
    memset(&mm, 0, sizeof(mm));
    mm.faces = faces;
    mm.nfaces = own_tris;
    mm.meshes = 1;
    mm.images = images;
    mm.nimages = nimage;
    mm.anims = c->anims;
    mm.nanims = TANK_MOVES;
    mm.nvert = RigVerts;
    mm.rest_anim = TANK_BREATHE;
    mm.frame = tank_frame;
    mm.ctx = c;
    if (!made_write(&mm, &out->tex[KIND_TANK], &out->tex_size[KIND_TANK], &out->mdl[KIND_TANK], &out->mdl_size[KIND_TANK])) {
        printf("bonus: Ripper Roo can't be put in a tank\n");
        return 0;
    }
    return 1;
}

/* --- in a race cart (Crash Dash) ---
 * Crash Dash's heroes drive carts round its tracks (but the water's, where they ride fish), each hero's cart in its own model with it
 * (models\chars\race\, five moves: BREATHE as it drives, RUN as a boost stretches the cart out and back, HIT as it's knocked dazed, FALL
 * as it drops off the track, and WIN). Roo's model has Rilla Roo's cart, her model's strips all lower than DASH_CART_TOP as it rests
 * (she's above it), moving as hers does in the same move and frame. Each hero's cart is a colour of its own (Crash's blue, Coco's pink,
 * Cortex's yellow, Brio's green, Tiny's dark purple, the others orange) and she may be racing too, so his is painted DASH_PAINT, red
 * (his fur's blue was Crash's, and he blended into it), but for its lights (DASH_LIGHTS), yellow as every cart's are. He sits in it as
 * big as the biggest of the others, DASH_SCALE of his size about his hips, which are DASH_HIPS
 * up and DASH_Z back in it, against its back, his legs as in a Ballistix car. He's thrown about as she is in it, by how far the middle of
 * her body goes from where it is as she drives (back as it boosts, up as it's knocked, down as it falls); sways as he drives (DASH_SWAY
 * of his standing animation's, at his own pace: his BREATHE is DASH_BREATHE_TICKS long, round and round); laughs as he boosts
 * (DASH_WHEE, thrown DASH_BACK back into his seat); his head goes round and round as he's dazed (DASH_DAZE at his neck, slowing); he
 * leans back as he falls (DASH_BACK); and laughs as he wins, bouncing DASH_HOP in his seat. His tongue and ears swing. */
#define DASH_CART_TOP -220.0f
#define DASH_PAINT 0x2020E0 /* red | green << 8 | blue << 16 */
#define DASH_FROM 45.0f     /* its oranges and yellows (hm_paint) */
#define DASH_LIGHTS 10      /* her texture set's image for its lights */
#define DASH_SCALE 0.8f
#define DASH_HIPS 95.0f
#define DASH_Z 45.0f
#define DASH_SWAY 0.3f
#define DASH_BREATHE_TICKS 24
#define DASH_WHEE 0.6f
#define DASH_BACK 0.35f
#define DASH_DAZE 0.3f
#define DASH_HOP 40.0f
#define DASH_FADE 4.0f
#define DASH_MAX_TICKS 96

enum { DASH_BREATHE, DASH_RUN, DASH_HIT, DASH_FALL, DASH_WIN, DASH_MOVES };
static const char *const DashNames[DASH_MOVES] = { "BREATHE", "RUN", "HIT", "FALL", "WIN" };

typedef struct {
    const FootCtx *foot;
    MadeAnim anims[DASH_MOVES];
    V3 swing[DASH_MOVES][DASH_MAX_TICKS];
    GamePart cart;
    s32 cart_anim[DASH_MOVES], cart_rest; /* Rilla Roo's animations it moves with, by the same names, and her BREATHE */
    V3 *cart_at;                           /* where her model's corners are */
    V3 rider;                              /* the middle of her body as she drives */
} DashCtx;

static DashCtx Dash;

static s32 keep_cart(void *ctx, const V3 *rest, s32 n, s32 textured) {
    s32 k;

    (void)ctx;
    (void)textured;
    for (k = 0; k < n; k++) {
        if (rest[k].y < DASH_CART_TOP) {
            return 0;
        }
    }
    return 1;
}

/* Rilla Roo's model in his move a at its frame f (hers the same, but his own BREATHE: her cart's still in hers): its corners into
 * c->cart_at, and how far the middle of her body is from where it is as she drives */
static V3 dash_at(const DashCtx *c, s32 a, s32 f) {
    V3 d = { 0, 0, 0 };
    s32 h = a != DASH_BREATHE && c->cart_anim[a] >= 0 ? c->cart_anim[a] : c->cart_rest, k, n = 0;

    if (c->cart_at == NULL || !hm_model_at(&c->cart, h, h == c->cart_rest ? 0 : (float)f / VEHICLE_PER, c->cart_at)) {
        return d;
    }
    for (k = 0; k < c->cart.corners; k++) {
        if (!c->cart.in_part[k]) {
            d.x += c->cart_at[k].x;
            d.y += c->cart_at[k].y;
            d.z += c->cart_at[k].z;
            n++;
        }
    }
    if (n > 0) {
        d.x = d.x / n - c->rider.x;
        d.y = d.y / n - c->rider.y;
        d.z = d.z / n - c->rider.z;
    }
    return d;
}

/* Frame f of move a, but for the swing: his vertices in Crash Bash's units, in the cart (and her model's corners there, in c->cart_at) */
static void dash_pose(const DashCtx *c, s32 a, s32 f, V3 *out) {
    static V3 q[MAX_VERTS];
    const FootCtx *foot = c->foot;
    s32 frames = c->anims[a].frames, k;
    float g = (float)f / VEHICLE_PER, ticks = (float)frames / VEHICLE_PER, last = ticks > 1 ? ticks - 1 : 1;
    float t = g / last, fade, back = 0, daze = 0, spin = 0, hop = 0, s = ROO_UNIT * foot->k * DASH_SCALE;
    V3 waist = { 0, RIDE_WAIST, 0 }, neck = { 0, RIDE_NECK, 0 }, thrown = dash_at(c, a, f);

    for (k = 0; k < RigVerts; k++) {
        q[k] = car_seated(Rest[k], k);
    }
    fade = fminf(hm_smooth(0, DASH_FADE, g), hm_smooth(last, last - DASH_FADE, g));
    switch (a) {
    case DASH_BREATHE:
        car_motion(foot, ROO_REST, g * foot->anims[ROO_REST].count / (ticks > 0 ? ticks : 1), DASH_SWAY, q);
        break;
    case DASH_RUN:
        car_motion(foot, ROO_LAUGH, g, DASH_WHEE * fade, q);
        back = DASH_BACK * sinf(ROO_PI * t);
        break;
    case DASH_HIT:
        /* round and round, slowing */
        daze = DASH_DAZE * hm_smooth(0, 3, g) * (1 - 0.5f * t);
        spin = 2 * ROO_PI * 2.5f * (t - 0.25f * t * t);
        break;
    case DASH_FALL:
        back = DASH_BACK * hm_smooth(0, 6, g);
        break;
    case DASH_WIN:
        car_motion(foot, ROO_LAUGH, g, fade, q);
        hop = DASH_HOP * fabsf(sinf(ROO_PI * g / 8)) * fade;
        thrown.x = thrown.y = thrown.z = 0;
        break;
    }
    for (k = 0; k < RigVerts; k++) {
        float up = hm_smooth(RIDE_WAIST - RIDE_WAIST_BLEND, RIDE_WAIST + RIDE_WAIST_BLEND, Rest[k].y);
        float head = hm_smooth(RIDE_NECK - RIDE_NECK_BLEND, RIDE_NECK + RIDE_NECK_BLEND, Rest[k].y);
        V3 p = q[k];

        if (up > 0 && back != 0) {
            p = hm_mix(p, hm_turn_x(p, waist, -back), up);
        }
        if (head > 0 && daze != 0) {
            /* his head nodding round in a circle */
            p = hm_mix(p, hm_turn_x(hm_turn_z(p, neck, daze * sinf(spin)), neck, daze * cosf(spin)), head);
        }
        p.y += hop / s * up;
        out[k].x = p.x * s + thrown.x;
        out[k].y = -(p.y - HIP_Y) * s - DASH_HIPS + thrown.y;
        out[k].z = -(p.z - HIP_Z) * s + DASH_Z + thrown.z;
    }
}

/* The swing of his tongue and ears through each move, a tick at a time, from his head's path */
static void dash_swing(DashCtx *c) {
    static V3 v[MAX_VERTS], path[DASH_MAX_TICKS];
    s32 a, i, k, ticks;

    memset(c->swing, 0, sizeof(c->swing));
    for (a = 0; a < DASH_MOVES; a++) {
        ticks = (c->anims[a].frames + VEHICLE_PER - 1) / VEHICLE_PER;
        ticks = ticks > DASH_MAX_TICKS ? DASH_MAX_TICKS : ticks;
        for (i = 0; i < ticks; i++) {
            V3 sum = { 0, 0, 0 };
            float w = 0;

            dash_pose(c, a, i * VEHICLE_PER, v);
            for (k = 0; k < RigVerts; k++) {
                if (Rest[k].y > FOOT_HEAD_Y) {
                    sum.x += v[k].x;
                    sum.y += v[k].y;
                    sum.z += v[k].z;
                    w++;
                }
            }
            if (w > 0) {
                sum.x /= w;
                sum.y /= w;
                sum.z /= w;
            }
            path[i] = sum;
        }
        hm_flop(path, ticks, a == DASH_BREATHE, &RooSwing, c->swing[a]);
    }
}

static void dash_frame(void *ctx, s32 a, s32 f, V3 *out) {
    const DashCtx *c = ctx;
    s32 ticks = (c->anims[a].frames + VEHICLE_PER - 1) / VEHICLE_PER, i0, i1, k;
    float s = (float)f / VEHICLE_PER, u;
    V3 swing;

    ticks = ticks > DASH_MAX_TICKS ? DASH_MAX_TICKS : ticks;
    dash_pose(c, a, f, out);
    i0 = (s32)floorf(s);
    u = s - i0;
    i0 = i0 < ticks ? i0 : ticks - 1;
    i1 = i0 + 1 < ticks ? i0 + 1 : a == DASH_BREATHE ? 0 : i0;
    swing = hm_mix(c->swing[a][i0], c->swing[a][i1], u);
    for (k = 0; k < RigVerts; k++) {
        out[k].x += swing.x * FlopShare[k];
        out[k].y += swing.y * FlopShare[k];
        out[k].z += swing.z * FlopShare[k];
    }
    /* the cart, where dash_pose found it */
    for (k = 0; k < c->cart.nvert; k++) {
        out[RigVerts + k] = c->cart_at[c->cart.corner_of[k]];
    }
}

/* His race model: his own faces (own_tris of them) and images, and the cart; Crash Dash's moves as long as Rilla Roo's, but BREATHE */
static s32 dash_make(const FootCtx *foot, const MadeFace *faces, s32 own_tris, const MadeImage *images, s32 nimage, HeroMade *out) {
    DashCtx *c = &Dash;
    MadeFace *all = NULL;
    MadeImage *imgs = NULL;
    MadeModel mm;
    s32 a, n, k, j, ok = 0, breathe;
    V3 sum = { 0, 0, 0 };

    memset(c, 0, sizeof(*c));
    c->foot = foot;
    for (a = 0; a < DASH_MOVES; a++) {
        n = a == DASH_BREATHE ? DASH_BREATHE_TICKS : hm_bank_frames(DB_CharsRaceRilla, DashNames[a]);
        n = n > 0 ? n : 20;
        n = n > DASH_MAX_TICKS ? DASH_MAX_TICKS : n;
        c->anims[a].name = DashNames[a];
        c->anims[a].frames = n * VEHICLE_PER;
        c->anims[a].mesh = 0;
        c->anims[a].step = VEHICLE_PER;
    }
    /* the cart, moving as Rilla Roo's does, painted his blue */
    if (!hm_game_part(DB_CharsRaceRilla, keep_cart, NULL, &c->cart) || (breathe = hm_model_anim(&c->cart, "BREATHE", NULL)) < 0
        || (c->cart_at = malloc(sizeof(V3) * (size_t)c->cart.corners)) == NULL || !hm_model_at(&c->cart, breathe, 0, c->cart_at)) {
        printf("bonus: Ripper Roo has no race cart (Rilla Roo's)\n");
        goto done;
    }
    c->cart_rest = breathe;
    for (a = 0; a < DASH_MOVES; a++) {
        c->cart_anim[a] = hm_model_anim(&c->cart, DashNames[a], NULL);
    }
    for (k = 0, n = 0; k < c->cart.corners; k++) {
        if (!c->cart.in_part[k]) {
            sum.x += c->cart_at[k].x;
            sum.y += c->cart_at[k].y;
            sum.z += c->cart_at[k].z;
            n++;
        }
    }
    if (n > 0) {
        c->rider.x = sum.x / n;
        c->rider.y = sum.y / n;
        c->rider.z = sum.z / n;
    }
    for (k = 0; k < c->cart.nfaces; k++) {
        if (c->cart.faces[k].image < 0 || c->cart.image_of[c->cart.faces[k].image] != DASH_LIGHTS) {
            hm_paint(&c->cart.faces[k], 1, NULL, 0, DASH_FROM, DASH_PAINT);
        }
    }
    for (k = 0; k < c->cart.nimages; k++) {
        if (c->cart.image_of[k] != DASH_LIGHTS) {
            hm_paint(NULL, 0, &c->cart.images[k], 1, DASH_FROM, DASH_PAINT);
        }
    }
    dash_swing(c);
    all = malloc(sizeof(MadeFace) * (size_t)(own_tris + c->cart.nfaces));
    imgs = malloc(sizeof(MadeImage) * (size_t)(nimage + c->cart.nimages + 1));
    if (all == NULL || imgs == NULL) {
        goto done;
    }
    memcpy(all, faces, sizeof(MadeFace) * (size_t)own_tris);
    for (k = 0; k < c->cart.nfaces; k++) {
        MadeFace *f = &all[own_tris + k];

        *f = c->cart.faces[k];
        for (j = 0; j < 3; j++) {
            f->v[j] += RigVerts;
        }
        f->image = f->image >= 0 ? f->image + nimage : -1;
    }
    memcpy(imgs, images, sizeof(MadeImage) * (size_t)nimage);
    memcpy(imgs + nimage, c->cart.images, sizeof(MadeImage) * (size_t)c->cart.nimages);
    memset(&mm, 0, sizeof(mm));
    mm.faces = all;
    mm.nfaces = own_tris + c->cart.nfaces;
    mm.meshes = 1;
    mm.images = imgs;
    mm.nimages = nimage + c->cart.nimages;
    mm.anims = c->anims;
    mm.nanims = DASH_MOVES;
    mm.nvert = RigVerts + c->cart.nvert;
    mm.rest_anim = DASH_BREATHE;
    mm.frame = dash_frame;
    mm.ctx = c;
    ok = made_write(&mm, &out->tex[KIND_RACE], &out->tex_size[KIND_RACE], &out->mdl[KIND_RACE], &out->mdl_size[KIND_RACE]);
done:
    if (!ok) {
        printf("bonus: Ripper Roo can't be put in a race cart\n");
    }
    free(all);
    free(imgs);
    free(c->cart_at);
    c->cart_at = NULL;
    hm_part_free(&c->cart);
    return ok;
}

/* --- riding a fish (Splash Dash) ---
 * Crash Dash's water track has its heroes riding fish, each hero's in its own model with it (models\chars\splash\: Crash's and Coco's
 * a pink dolphin, Cortex's and Brio's a grey shark, Tiny's and Kong's an orca, Dingodile's and Rilla Roo's a blue fish), five moves:
 * BREATHE sat up on it, SWIM lying along it holding its head (as it swims, and as it waits too), TRANS sitting up and down again as it
 * sets off, FALL as it dives, and WIN sat up, waving. Roo's model has Rilla Roo's fish, her model's first FISH_STRIPS strips (it's
 * before her in it), moving as hers does in the same move and frame; painted his red, as his cart is (its blues, FISH_FROM), as he's
 * blue too. He sits astride it as on a polar bear, his legs less spread (FISH_SPREAD, his shins FISH_KNEE_OUT of it back in) as it's
 * narrower, FISH_SCALE of his size, his hips FISH_HIP_UP above the top of its back FISH_SEAT_Z along it, as it rests, and moves as its
 * back does there (a fit of its corners above FISH_BACK_Y within FISH_BACK_Z of his seat along it). With no hands, he holds on with his
 * tongue (at the user's word): out of his mouth to the top of its head (its corner nearest FISH_HEAD_Y and FISH_HEAD_Z as it rests,
 * FISH_REIN_UP over it) as a rein, all of it but its root (FISH_REIN_ROOT of the way out) on the way there. He sways as he sits up
 * (BREATHE); crouches forward FISH_CROUCH at his waist as it swims, rocking FISH_STROKE with its strokes; leans into the crouch as it
 * sets off (TRANS); is thrown back FISH_THROWN as it dives; and laughs as he wins, bouncing FISH_HOP. His ears swing. */
#define FISH_STRIPS 16
#define FISH_FROM 197.0f
#define FISH_SCALE 1.1f
#define FISH_SPREAD 0.7f
#define FISH_KNEE_OUT 0.9f
#define FISH_HIP_UP 5.0f
#define FISH_SEAT_Z 80.0f
#define FISH_SEAT_NEAR 40.0f
#define FISH_BACK_Y -40.0f
#define FISH_BACK_Z 220.0f
#define FISH_CROUCH 0.4f
#define FISH_STROKE 0.06f
#define FISH_THROWN 0.5f
#define FISH_HOP 30.0f
#define FISH_HEAD_Y -110.0f
#define FISH_HEAD_Z -300.0f
#define FISH_REIN_UP 8.0f
#define FISH_REIN_ROOT 0.3f
#define FISH_SWAY 0.3f
#define FISH_FADE 4.0f
#define FISH_MAX_TICKS 96
#define FISH_MAX_IDS 256

enum { FISH_BREATHE, FISH_FALL, FISH_SWIM, FISH_TRANS, FISH_WIN, FISH_MOVES };
static const char *const FishNames[FISH_MOVES] = { "BREATHE", "FALL", "SWIM", "TRANS", "WIN" };

typedef struct {
    const FootCtx *foot;
    MadeAnim anims[FISH_MOVES];
    V3 swing[FISH_MOVES][FISH_MAX_TICKS];
    GamePart fish;
    s32 fish_anim[FISH_MOVES], fish_rest; /* Rilla Roo's animations it moves with, by the same names, and her BREATHE */
    V3 *rest_at, *at;                     /* her model's corners as it rests, and in a frame */
    s16 back[FISH_MAX_IDS];
    s32 nback, head;                      /* its back's corners by his seat, and the one his tongue holds */
    float seat_y;                         /* the top of its back at his seat */
} FishCtx;

static FishCtx Fish;

static s32 keep_fish(void *ctx, const V3 *rest, s32 n, s32 textured) {
    s32 *strips = ctx;

    (void)rest;
    (void)n;
    (void)textured;
    return (*strips)++ < FISH_STRIPS;
}

/* Rilla Roo's model in move a at its frame f: its corners into c->at, and how its back's moved from as it rests */
static void fish_at(const FishCtx *c, s32 a, s32 f, Moved *back) {
    s32 h = c->fish_anim[a] >= 0 ? c->fish_anim[a] : c->fish_rest;

    if (!hm_model_at(&c->fish, h, (float)f / VEHICLE_PER, c->at)) {
        memcpy(c->at, c->rest_at, sizeof(V3) * (size_t)c->fish.corners);
    }
    hm_fit(c->rest_at, c->at, c->back, c->nback, back);
}

/* Frame f of move a, but for the swing and the rein: his vertices in Crash Bash's units, on the fish (and her model's corners there,
 * in c->at) */
static void fish_pose(const FishCtx *c, s32 a, s32 f, V3 *out) {
    static V3 q[MAX_VERTS];
    const FootCtx *foot = c->foot;
    s32 frames = c->anims[a].frames, k;
    float g = (float)f / VEHICLE_PER, ticks = (float)frames / VEHICLE_PER, last = ticks > 1 ? ticks - 1 : 1;
    float t = g / last, fade, lean = 0, hop = 0, s = ROO_UNIT * foot->k * FISH_SCALE;
    V3 waist = { 0, RIDE_WAIST, 0 };
    Moved back;

    fish_at(c, a, f, &back);
    for (k = 0; k < RigVerts; k++) {
        q[k] = astride(k, FISH_SPREAD, FISH_KNEE_OUT);
    }
    fade = fminf(hm_smooth(0, FISH_FADE, g), hm_smooth(last, last - FISH_FADE, g));
    switch (a) {
    case FISH_BREATHE:
        car_motion(foot, ROO_REST, g * foot->anims[ROO_REST].count / (ticks > 0 ? ticks : 1), FISH_SWAY, q);
        break;
    case FISH_SWIM:
        lean = FISH_CROUCH + FISH_STROKE * sinf(2 * ROO_PI * g / (ticks > 0 ? ticks : 1));
        break;
    case FISH_TRANS:
        lean = FISH_CROUCH * hm_smooth(0, 1, t);
        break;
    case FISH_FALL:
        lean = -FISH_THROWN * hm_smooth(0, 6, g);
        break;
    case FISH_WIN:
        car_motion(foot, ROO_LAUGH, g, fade, q);
        hop = FISH_HOP * fabsf(sinf(ROO_PI * g / 8)) * fade;
        break;
    }
    for (k = 0; k < RigVerts; k++) {
        float up = hm_smooth(RIDE_WAIST - RIDE_WAIST_BLEND, RIDE_WAIST + RIDE_WAIST_BLEND, Rest[k].y);
        V3 p = q[k], o;

        if (up > 0 && lean != 0) {
            p = hm_mix(p, hm_turn_x(p, waist, lean), up);
        }
        p.y += hop / s * up;
        o.x = p.x * s;
        o.y = -(p.y - HIP_Y) * s + c->seat_y - FISH_HIP_UP;
        o.z = -(p.z - HIP_Z) * s + FISH_SEAT_Z;
        out[k] = hm_moved(&back, o);
    }
}

/* His tongue as a rein, from his mouth to the top of its head: each of its vertices on the way there as far as it's out of his mouth
 * (but its root, coming out of it), the tongue's own shape about the way kept */
static void fish_rein(const FishCtx *c, V3 *out) {
    V3 root = { 0, 0, 0 }, tip = { 0, 0, 0 }, to = c->at[c->head], u, w, e;
    float nr = 0, nt = 0, l, l2, sn, cs;
    s32 k;

    to.y -= FISH_REIN_UP;
    for (k = 0; k < RigVerts; k++) {
        if (TongueShare[k] > 0 && TongueShare[k] < 0.25f) {
            root.x += out[k].x;
            root.y += out[k].y;
            root.z += out[k].z;
            nr++;
        } else if (TongueShare[k] > 0.85f) {
            tip.x += out[k].x;
            tip.y += out[k].y;
            tip.z += out[k].z;
            nt++;
        }
    }
    if (nr == 0 || nt == 0) {
        return;
    }
    root.x /= nr;
    root.y /= nr;
    root.z /= nr;
    u.x = tip.x / nt - root.x;
    u.y = tip.y / nt - root.y;
    u.z = tip.z / nt - root.z;
    w.x = to.x - root.x;
    w.y = to.y - root.y;
    w.z = to.z - root.z;
    l = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
    l2 = sqrtf(w.x * w.x + w.y * w.y + w.z * w.z);
    if (l < 1 || l2 < 1) {
        return;
    }
    u.x /= l;
    u.y /= l;
    u.z /= l;
    w.x /= l2;
    w.y /= l2;
    w.z /= l2;
    /* the turn from the way it goes to the way to its head: about e, by the angle whose sine and cosine are sn and cs */
    e.x = u.y * w.z - u.z * w.y;
    e.y = u.z * w.x - u.x * w.z;
    e.z = u.x * w.y - u.y * w.x;
    sn = sqrtf(e.x * e.x + e.y * e.y + e.z * e.z);
    cs = u.x * w.x + u.y * w.y + u.z * w.z;
    if (sn > 1e-6f) {
        e.x /= sn;
        e.y /= sn;
        e.z /= sn;
    }
    for (k = 0; k < RigVerts; k++) {
        float along, ed, share = hm_smooth(0, FISH_REIN_ROOT, TongueShare[k]);
        V3 v, a, p;

        if (TongueShare[k] <= 0) {
            continue;
        }
        v.x = out[k].x - root.x;
        v.y = out[k].y - root.y;
        v.z = out[k].z - root.z;
        along = v.x * u.x + v.y * u.y + v.z * u.z;
        a.x = v.x - along * u.x;
        a.y = v.y - along * u.y;
        a.z = v.z - along * u.z;
        if (sn > 1e-6f) {
            /* Rodrigues': a turned about e */
            V3 ea = { e.y * a.z - e.z * a.y, e.z * a.x - e.x * a.z, e.x * a.y - e.y * a.x };

            ed = e.x * a.x + e.y * a.y + e.z * a.z;
            a.x = a.x * cs + ea.x * sn + e.x * ed * (1 - cs);
            a.y = a.y * cs + ea.y * sn + e.y * ed * (1 - cs);
            a.z = a.z * cs + ea.z * sn + e.z * ed * (1 - cs);
        }
        along *= l2 / l;
        p.x = root.x + along * w.x + a.x;
        p.y = root.y + along * w.y + a.y;
        p.z = root.z + along * w.z + a.z;
        out[k] = hm_mix(out[k], p, share);
    }
}

/* The swing of his ears through each move, a tick at a time, from his head's path */
static void fish_swing(FishCtx *c) {
    static V3 v[MAX_VERTS], path[FISH_MAX_TICKS];
    s32 a, i, k, ticks;

    memset(c->swing, 0, sizeof(c->swing));
    for (a = 0; a < FISH_MOVES; a++) {
        ticks = (c->anims[a].frames + VEHICLE_PER - 1) / VEHICLE_PER;
        ticks = ticks > FISH_MAX_TICKS ? FISH_MAX_TICKS : ticks;
        for (i = 0; i < ticks; i++) {
            V3 sum = { 0, 0, 0 };
            float w = 0;

            fish_pose(c, a, i * VEHICLE_PER, v);
            for (k = 0; k < RigVerts; k++) {
                if (Rest[k].y > FOOT_HEAD_Y) {
                    sum.x += v[k].x;
                    sum.y += v[k].y;
                    sum.z += v[k].z;
                    w++;
                }
            }
            if (w > 0) {
                sum.x /= w;
                sum.y /= w;
                sum.z /= w;
            }
            path[i] = sum;
        }
        hm_flop(path, ticks, a == FISH_SWIM || a == FISH_BREATHE, &RooSwing, c->swing[a]);
    }
}

static void fish_frame(void *ctx, s32 a, s32 f, V3 *out) {
    const FishCtx *c = ctx;
    s32 ticks = (c->anims[a].frames + VEHICLE_PER - 1) / VEHICLE_PER, i0, i1, k;
    float s = (float)f / VEHICLE_PER, u;
    V3 swing;

    ticks = ticks > FISH_MAX_TICKS ? FISH_MAX_TICKS : ticks;
    fish_pose(c, a, f, out);
    i0 = (s32)floorf(s);
    u = s - i0;
    i0 = i0 < ticks ? i0 : ticks - 1;
    i1 = i0 + 1 < ticks ? i0 + 1 : a == FISH_SWIM || a == FISH_BREATHE ? 0 : i0;
    swing = hm_mix(c->swing[a][i0], c->swing[a][i1], u);
    for (k = 0; k < RigVerts; k++) {
        float w = TongueShare[k] > 0 ? 0 : FlopShare[k]; /* his ears; his tongue's the rein */

        out[k].x += swing.x * w;
        out[k].y += swing.y * w;
        out[k].z += swing.z * w;
    }
    fish_rein(c, out);
    /* the fish, where fish_pose found it */
    for (k = 0; k < c->fish.nvert; k++) {
        out[RigVerts + k] = c->at[c->fish.corner_of[k]];
    }
}

/* His fish model: his own faces (own_tris of them) and images, and the fish; Splash Dash's moves as long as Rilla Roo's */
static s32 fish_make(const FootCtx *foot, const MadeFace *faces, s32 own_tris, const MadeImage *images, s32 nimage, HeroMade *out) {
    FishCtx *c = &Fish;
    MadeFace *all = NULL;
    MadeImage *imgs = NULL;
    MadeModel mm;
    s32 a, n, k, j, ok = 0, strips = 0;
    float best = 1e30f;

    memset(c, 0, sizeof(*c));
    c->foot = foot;
    for (a = 0; a < FISH_MOVES; a++) {
        n = hm_bank_frames(DB_CharsSplashRilla, FishNames[a]);
        n = n > 0 ? n : 20;
        n = n > FISH_MAX_TICKS ? FISH_MAX_TICKS : n;
        c->anims[a].name = FishNames[a];
        c->anims[a].frames = n * VEHICLE_PER;
        c->anims[a].mesh = 0;
        c->anims[a].step = VEHICLE_PER;
    }
    /* the fish, moving as Rilla Roo's does, painted his red */
    if (!hm_game_part(DB_CharsSplashRilla, keep_fish, &strips, &c->fish)
        || (c->fish_rest = hm_model_anim(&c->fish, "BREATHE", NULL)) < 0
        || (c->rest_at = malloc(sizeof(V3) * (size_t)c->fish.corners)) == NULL
        || (c->at = malloc(sizeof(V3) * (size_t)c->fish.corners)) == NULL || !hm_model_at(&c->fish, c->fish_rest, 0, c->rest_at)) {
        printf("bonus: Ripper Roo has no fish (Rilla Roo's)\n");
        goto done;
    }
    for (a = 0; a < FISH_MOVES; a++) {
        c->fish_anim[a] = hm_model_anim(&c->fish, FishNames[a], NULL);
    }
    /* its back by his seat, the top of it there, and the corner his tongue holds */
    c->seat_y = 1e30f;
    for (k = 0; k < c->fish.corners; k++) {
        const V3 *p = &c->rest_at[k];
        float d;

        if (!c->fish.in_part[k]) {
            continue;
        }
        if (p->y < FISH_BACK_Y && fabsf(p->z - FISH_SEAT_Z) < FISH_BACK_Z && c->nback < FISH_MAX_IDS) {
            c->back[c->nback++] = (s16)k;
        }
        if (fabsf(p->x) < FISH_SEAT_NEAR && fabsf(p->z - FISH_SEAT_Z) < FISH_SEAT_NEAR && p->y < c->seat_y) {
            c->seat_y = p->y;
        }
        d = p->x * p->x + (p->y - FISH_HEAD_Y) * (p->y - FISH_HEAD_Y) + (p->z - FISH_HEAD_Z) * (p->z - FISH_HEAD_Z);
        if (d < best) {
            best = d;
            c->head = k;
        }
    }
    if (c->nback < 3 || c->seat_y > 1e29f) {
        printf("bonus: Rilla Roo's fish isn't as it was (%d corners of its back)\n", c->nback);
        goto done;
    }
    hm_paint(c->fish.faces, c->fish.nfaces, c->fish.images, c->fish.nimages, FISH_FROM, DASH_PAINT);
    fish_swing(c);
    all = malloc(sizeof(MadeFace) * (size_t)(own_tris + c->fish.nfaces));
    imgs = malloc(sizeof(MadeImage) * (size_t)(nimage + c->fish.nimages + 1));
    if (all == NULL || imgs == NULL) {
        goto done;
    }
    memcpy(all, faces, sizeof(MadeFace) * (size_t)own_tris);
    for (k = 0; k < c->fish.nfaces; k++) {
        MadeFace *f = &all[own_tris + k];

        *f = c->fish.faces[k];
        for (j = 0; j < 3; j++) {
            f->v[j] += RigVerts;
        }
        f->image = f->image >= 0 ? f->image + nimage : -1;
    }
    memcpy(imgs, images, sizeof(MadeImage) * (size_t)nimage);
    memcpy(imgs + nimage, c->fish.images, sizeof(MadeImage) * (size_t)c->fish.nimages);
    memset(&mm, 0, sizeof(mm));
    mm.faces = all;
    mm.nfaces = own_tris + c->fish.nfaces;
    mm.meshes = 1;
    mm.images = imgs;
    mm.nimages = nimage + c->fish.nimages;
    mm.anims = c->anims;
    mm.nanims = FISH_MOVES;
    mm.nvert = RigVerts + c->fish.nvert;
    mm.rest_anim = FISH_BREATHE;
    mm.frame = fish_frame;
    mm.ctx = c;
    ok = made_write(&mm, &out->tex[KIND_SPLASH], &out->tex_size[KIND_SPLASH], &out->mdl[KIND_SPLASH], &out->mdl_size[KIND_SPLASH]);
done:
    if (!ok) {
        printf("bonus: Ripper Roo can't be put on a fish\n");
    }
    free(all);
    free(imgs);
    free(c->rest_at);
    free(c->at);
    c->rest_at = c->at = NULL;
    hm_part_free(&c->fish);
    return ok;
}

/* His sounds, which his boss fight's object (RRooC) plays: sound entries in sound pages, at 11025 Hz. His laugh (CrznA), his jump's
 * call (rjpnA), and his landings, light (ld30A) and heavy (ld00A). His game has no hurt voice for him, so his yelp is his laugh's first
 * "ha", its first ROO_YELP_BYTES (0.22 s, where it falls quiet), a little higher. */
#define ROO_PITCH 0x400
#define ROO_YELP_BYTES (86 * 16)
#define ROO_YELP_PITCH 0x480
#define CARD_HEAD 42 /* his hero card: his head and the tongue as it leaves his mouth; then the tongue hanging down and curling at its end */
#define CRASH_CRATE_VOICES 0xD4 /* his throw is Crash's: the archive's sfx/vox_crate/crash.sfx, Crash's voice in Crate Crush, */
#define CRASH_THROW 0x10        /* its sound for his throw, the sixteenth of his block (from sound 1) */
#define RILLA_COMMON_VOICES 0xB8 /* his name as he wins is the announcer's "Rilla Roo", the nearest the game has: sfx/vox_common/rilla.sfx, */
#define RILLA_NAME 0xC9          /* the first of her block; whoever's place he has, as with another's he "won" as that hero */
#define ROO_NAME_BYTES (260 * 16) /* and if it hasn't got it, his laugh's first 0.65 s, its long "ha" and four short ones, to a quiet gap
                                   * in it, done before the announcer's "wins!" (0.77 s on at the soonest) */
static u8 *RooSound[NUM_VOICES];
static HeroSound RooThrow, RooName;
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
    if (RooName.adpcm == NULL) {
        hm_game_sound(RILLA_COMMON_VOICES, RILLA_NAME, &RooName);
    }
    if (RooName.adpcm != NULL) {
        out->sound[VOICE_NAME] = RooName;
    } else {
        out->sound[VOICE_NAME] = out->sound[VOICE_LAUGH];
        out->sound[VOICE_NAME].bytes = out->sound[VOICE_LAUGH].bytes < ROO_NAME_BYTES ? out->sound[VOICE_LAUGH].bytes : ROO_NAME_BYTES;
    }
    out->sound[VOICE_YELP] = out->sound[VOICE_LAUGH];
    out->sound[VOICE_YELP].bytes = out->sound[VOICE_LAUGH].bytes < ROO_YELP_BYTES ? out->sound[VOICE_LAUGH].bytes : ROO_YELP_BYTES;
    out->sound[VOICE_YELP].pitch = ROO_YELP_PITCH;
    if (RooThrow.adpcm == NULL) {
        hm_game_sound(CRASH_CRATE_VOICES, CRASH_THROW, &RooThrow);
    }
    out->sound[VOICE_THROW] = RooThrow;
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
    HopLift = HOP_SEEN / ((float)ROO_KEEP_LIFT / 0x1000) / (ROO_UNIT * foot.k);
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

    /* on foot, his tongue and ears swinging */
    if (RigVerts > 0) {
        flop_setup(&model);
        foot_swing(&foot);
    }
    for (j = 0; j < ROO_ANIMS; j++) {
        made_anims[j].name = RooAnims[j].name;
        /* ROO_SUBFRAMES to each of his game's, its frames the keyframes: the last is his game's last */
        made_anims[j].frames = (anim_frames(&anims[j], j) - 1) * ROO_SUBFRAMES + 1;
        made_anims[j].mesh = 0;
        made_anims[j].step = ROO_SUBFRAMES;
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
        rider.steady = RIDE_STEADY;
        rider.match_bank = DB_CharsPolar2Rilla; /* his base's: its moves then go a frame a tick */
        rider.faces = faces;
        rider.nfaces = own_tris;
        rider.images = images;
        rider.nimages = nimage;
        rider.charred = RIDE_CHARRED;
        rider.own = ride_own;
        rider.flop = FlopShare;
        rider.swing = RooSwing;
        RideOwn.anims = anims;
        memcpy(RideOwn.scale, foot.scale, sizeof(RideOwn.scale));
        RideOwn.k = foot.k;
        RideOwn.share = waist_share;
        if (!ride_make(&rider, &out->tex[KIND_POLAR], &out->tex_size[KIND_POLAR], &out->mdl[KIND_POLAR], &out->mdl_size[KIND_POLAR])) {
            printf("bonus: Ripper Roo has no polar bear to ride\n");
        }
    }
    /* on his cane (Pogo), in a car (Ballistix), in a tank, in a race cart (Crash Dash) and on a fish (Splash Dash): his own faces, as
     * riding */
    if (RigVerts > 0) {
        pogo_make(&foot, faces, own_tris, images, nimage, out);
        car_make(&foot, faces, own_tris, images, nimage, out);
        tank_make(&foot, faces, own_tris, images, nimage, out);
        dash_make(&foot, faces, own_tris, images, nimage, out);
        fish_make(&foot, faces, own_tris, images, nimage, out);
    }
    if (c1_sprite64(&level, "ShadC", 5, RooCard)) {
        out->card = RooCard;
        out->card_w = out->card_h = 64;
        out->card_head = CARD_HEAD;
    }
    make_sounds(&level, out);
    printf("bonus: Ripper Roo from Crash Bandicoot, %d triangles, %d textures, %d bytes%s%s%s%s%s%s%s%s%s%s%s\n", ntri, nimage,
           out->mdl_size[KIND_FOOT], out->mdl[KIND_POLAR] != NULL ? ", riding a bear" : "",
           out->mdl[KIND_POGO] != NULL ? ", on his cane" : "", out->mdl[KIND_BALL] != NULL ? ", in a car" : "",
           out->mdl[KIND_TANK] != NULL ? ", in a tank" : "", out->mdl[KIND_RACE] != NULL ? ", in a race cart" : "",
           out->mdl[KIND_SPLASH] != NULL ? ", riding a fish" : "", out->card != NULL ? ", his hero card" : "",
           out->sound[VOICE_LAUGH].adpcm != NULL ? ", his laugh" : "", out->sound[VOICE_THUD].adpcm != NULL ? ", his landing" : "",
           out->sound[VOICE_THROW].adpcm != NULL ? ", Crash's throw" : "", RooName.adpcm != NULL ? ", named Rilla Roo" : "");
    ok = 1;
done:
    made_images_free(images, nimage);
    free(level.pages);
    return ok;
}
