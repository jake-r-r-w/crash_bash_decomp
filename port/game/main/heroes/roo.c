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

/* His sounds for his base's (docs/EXTRA_HEROES.md, A hero's sounds): the announcer still names Rilla Roo, as near a name as there is;
 * he laughs as he taunts and wins, yelps only as he's hurt, gives his jump's call as he jumps and lunges, thuds as he lands, and throws
 * with Crash's throw (his game has none of his own). Picking up a crate, carrying it and pushing he's quiet. */
static const Voice RooVoices[] = {
    { 0, VOICE_BASE }, { 1, VOICE_LAUGH }, { 2, VOICE_YELP }, { 3, VOICE_YELP }, { 4, VOICE_YELP }, { 5, VOICE_YELP },
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

const HeroDef HeroRoo = {
    "RIPPER ROO", "roo", "SCUS-94900", Kanga,
    { { RooStands, "BREATHE", ROO_KEEP_LIFT, 0 }, { RooRides, "BREATHE", 0x1000, Crash } },
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

static V3 seated(s32 k) {
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
        r = hm_turn_x(hm_turn_z(q, hip, side * SEAT_SPREAD), hip, -SEAT_FLEX);
        knee = hm_turn_x(hm_turn_z(knee, hip, side * SEAT_SPREAD), hip, -SEAT_FLEX);
        shin = hm_turn_x(hm_turn_z(r, knee, -side * SEAT_SPREAD * SEAT_KNEE_OUT), knee, SEAT_KNEE_BACK);
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

/* What's loose on him as he rides: his tongue, the faces of his model that are its reds and pinks, more of it the further it's out of
 * his mouth (FLOP_TONGUE_ROOT to FLOP_TONGUE_TIP forward, standing); and his ears, out to the sides of his head above FLOP_EAR_LOW,
 * more the further out (FLOP_EAR_ROOT to FLOP_EAR_TIP), FLOP_EAR of the tongue's. They swing on a spring at his head as the bear
 * throws it about: FLOP_PERIOD frames a swing, FLOP_DAMPING of what would stop it, taking FLOP_GAIN of the throws, FLOP_MAX at most. */
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
static float FlopShare[MAX_VERTS];

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
#define CRASH_CRATE_VOICES 0xD4 /* his throw is Crash's: the archive's sfx/vox_crate/crash.sfx, Crash's voice in Crate Crush, */
#define CRASH_THROW 0x10        /* its sound for his throw, the sixteenth of his block (from sound 1) */
static u8 *RooSound[NUM_VOICES];
static HeroSound RooThrow;
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
        rider.steady = RIDE_STEADY;
        rider.match_bank = DB_CharsPolar2Rilla; /* his base's: its moves then go a frame a tick */
        rider.faces = faces;
        rider.nfaces = own_tris;
        rider.images = images;
        rider.nimages = nimage;
        rider.charred = RIDE_CHARRED;
        rider.own = ride_own;
        flop_setup(&model);
        rider.flop = FlopShare;
        rider.flop_period = FLOP_PERIOD;
        rider.flop_damping = FLOP_DAMPING;
        rider.flop_gain = FLOP_GAIN;
        rider.flop_max = FLOP_MAX;
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
    printf("bonus: Ripper Roo from Crash Bandicoot, %d triangles, %d textures, %d bytes%s%s%s%s%s\n", ntri, nimage, out->mdl_size[KIND_FOOT],
           out->mdl[KIND_POLAR] != NULL ? ", riding a bear" : "", out->card != NULL ? ", his hero card" : "",
           out->sound[VOICE_LAUGH].adpcm != NULL ? ", his laugh" : "", out->sound[VOICE_THUD].adpcm != NULL ? ", his landing" : "",
           out->sound[VOICE_THROW].adpcm != NULL ? ", Crash's throw" : "");
    ok = 1;
done:
    made_images_free(images, nimage);
    free(level.pages);
    return ok;
}
