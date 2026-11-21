/* The standard moves (kit.h), for any model on the standard skeleton, whatever pose it rests in.
 *
 * The body's bones (hips to head, the jaw, ears and tail) turn from how they rest. The limbs are aimed instead: each move says where
 * an upper arm or thigh points, as turns from hanging straight down (x: forward with less than 0; z: out from the body), and where its
 * lower half points from there; and each bone is turned from its own rest direction to that. So a model made with its arms out in a T,
 * hanging, or held up begging moves the same. A raised arm goes no higher than the model's arm_raise (its markers say: a short-armed
 * hero with a big head can't reach over it). Distances are for a hero 0.955 tall, scaled to the model. */
#include <math.h>
#include <string.h>
#include "kit.h"

#define DEG 0.0174532925f

typedef float Q[4];

static void q_axis(Q q, float x, float y, float z, float deg) {
    float h = deg * DEG / 2, s = sinf(h), n = sqrtf(x * x + y * y + z * z);

    q[0] = x / n * s;
    q[1] = y / n * s;
    q[2] = z / n * s;
    q[3] = cosf(h);
}

static void q_mul(const Q a, const Q b, Q out) {
    Q r;

    r[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
    r[1] = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
    r[2] = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];
    r[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
    memcpy(out, r, sizeof(r));
}

static void q_inv(const Q a, Q out) {
    out[0] = -a[0];
    out[1] = -a[1];
    out[2] = -a[2];
    out[3] = a[3];
}

static void q_rot(const Q q, const float v[3], float out[3]) {
    Q p = { v[0], v[1], v[2], 0 }, c, r;

    q_inv(q, c);
    q_mul(q, p, r);
    q_mul(r, c, r);
    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
}

/* turns about x (pitch: + tips its end back), y (yaw: + to his left), z (roll), z first */
static void euler(Q q, float x, float y, float z) {
    Q a, b, c;

    q_axis(a, 0, 1, 0, y);
    q_axis(b, 1, 0, 0, x);
    q_axis(c, 0, 0, 1, z);
    q_mul(a, b, q);
    q_mul(q, c, q);
}

/* the shortest turn from direction a to b */
static void q_arc(const float a[3], const float b[3], Q out) {
    float la = sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]), lb = sqrtf(b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);
    float x[3] = { a[0] / la, a[1] / la, a[2] / la }, y[3] = { b[0] / lb, b[1] / lb, b[2] / lb };
    float d = x[0] * y[0] + x[1] * y[1] + x[2] * y[2], n;

    if (d < -0.9999f) {
        /* opposite: about any axis across */
        float ax[3] = { 0, -x[2], x[1] };

        if (fabsf(x[0]) > 0.9f) {
            ax[0] = -x[2], ax[1] = 0, ax[2] = x[0];
        }
        q_axis(out, ax[0], ax[1], ax[2], 180);
        return;
    }
    out[0] = x[1] * y[2] - x[2] * y[1];
    out[1] = x[2] * y[0] - x[0] * y[2];
    out[2] = x[0] * y[1] - x[1] * y[0];
    out[3] = 1 + d;
    n = sqrtf(out[0] * out[0] + out[1] * out[1] + out[2] * out[2] + out[3] * out[3]);
    for (int i = 0; i < 4; i++) {
        out[i] /= n;
    }
}

static float smooth(float a, float b, float x) {
    float t = (x - a) / (b - a);

    t = t < 0 ? 0 : t > 1 ? 1 : t;
    return t * t * (3 - 2 * t);
}

static float wave(float t, float period, float phase) {
    return sinf(2 * 3.14159265f * (t / period + phase));
}

/* --- a pose: turns of the body's bones, aims of the limbs --- */

typedef struct {
    float body[KB_COUNT][3]; /* euler turns from rest, for the body's bones */
    int turned[KB_COUNT];
    float arm[2][3];         /* each arm's upper: forward (x) and out (z); and its lower: forward from the upper (x) */
    float leg[2][3];         /* each leg's thigh forward, its shin from the thigh, its foot from the shin */
    float hips[3];           /* the hips' move */
} Pose;

static void turn(Pose *p, int b, float x, float y, float z) {
    p->body[b][0] = x;
    p->body[b][1] = y;
    p->body[b][2] = z;
    p->turned[b] = 1;
}

/* the ears swinging behind the head's moves, the tail wagging, the jaw panting */
static void ears(Pose *p, float t, float bob, float period) {
    turn(p, KB_EAR_L1, 0, 0, -6 - 10 * wave(t, period, -0.15f) * bob);
    turn(p, KB_EAR_R1, 0, 0, 6 + 10 * wave(t, period, -0.15f) * bob);
    turn(p, KB_EAR_L2, 8 * wave(t, period, -0.3f) * bob, 0, -8 * wave(t, period, -0.3f) * bob);
    turn(p, KB_EAR_R2, 8 * wave(t, period, -0.3f) * bob, 0, 8 * wave(t, period, -0.3f) * bob);
}

static void wag(Pose *p, float t, float period, float deg, float lift) {
    turn(p, KB_TAIL1, -lift, deg * wave(t, period, 0), 0);
    turn(p, KB_TAIL2, -lift * 0.5f, deg * 0.8f * wave(t, period, -0.12f), 0);
    turn(p, KB_TAIL3, 0, deg * 0.6f * wave(t, period, -0.24f), 0);
}

static void pant(Pose *p, float t, float period, float open) {
    turn(p, KB_JAW, open * 0.5f + open * 0.5f * (0.5f + 0.5f * wave(t, period, 0)), 0, 0);
}

static void arms(Pose *p, float lx, float lo, float lf, float rx, float ro, float rf) {
    p->arm[0][0] = lx, p->arm[0][1] = lo, p->arm[0][2] = lf;
    p->arm[1][0] = rx, p->arm[1][1] = ro, p->arm[1][2] = rf;
}

static void legs(Pose *p, float lt, float ls, float lf, float rt, float rs, float rf) {
    p->leg[0][0] = lt, p->leg[0][1] = ls, p->leg[0][2] = lf;
    p->leg[1][0] = rt, p->leg[1][1] = rs, p->leg[1][2] = rf;
}

const KitMove KitMoves[] = {
    { "BREATHE", 2.0f }, { "RUN", 0.5f }, { "JUMP", 1.2f }, { "ATTACK", 0.53f }, { "WIN", 2.0f },
    { "HIT", 0.8f }, { "TAUNT", 1.5f }, { "PICKUP", 0.5f }, { "HOLD_SLOW", 0.7f }, { "HOLD_THROW", 0.5f },
};
const int KitMoveCount = sizeof(KitMoves) / sizeof(KitMoves[0]);

static void moves(int m, float t, Pose *p) {
    float s, k;

    memset(p, 0, sizeof(*p));
    arms(p, -10, 10, -10, -10, 10, -10);
    switch (m) {
    case 0: /* standing, panting happily, tail wagging */
        p->hips[1] = 0.003f * wave(t, 1, 0);
        turn(p, KB_SPINE, 2 * wave(t, 1, 0), 0, 0);
        turn(p, KB_CHEST, 1.5f * wave(t, 1, 0.1f), 0, 1.5f * wave(t, 2, 0));
        turn(p, KB_HEAD, -3 + 3 * wave(t, 2, 0.2f), 6 * wave(t, 2, 0), 4 * wave(t, 2, 0.25f));
        pant(p, t, 0.4f, 16);
        ears(p, t, 0.5f, 1);
        wag(p, t, 0.4f, 28, 20);
        arms(p, -10 + 3 * wave(t, 2, 0), 10, -15, -10 - 3 * wave(t, 2, 0), 10, -15);
        break;
    case 1: /* running */
        s = wave(t, 0.5f, 0);
        p->hips[1] = 0.018f * fabsf(wave(t, 0.5f, 0.25f));
        turn(p, KB_HIPS, 0, 6 * s, 0);
        turn(p, KB_SPINE, 12, -4 * s, 0);
        turn(p, KB_CHEST, 4, -4 * s, 0);
        turn(p, KB_HEAD, -14, 4 * s, 0);
        pant(p, t, 0.25f, 20);
        ears(p, t, 1.6f, 0.25f);
        wag(p, t, 0.25f, 18, 35);
        arms(p, -40 * s, 18, -50 - 20 * fmaxf(0, s), 40 * s, 18, -50 - 20 * fmaxf(0, -s));
        legs(p, -45 * s, 50 * fmaxf(0, s), -10 * s, 45 * s, 50 * fmaxf(0, -s), 10 * s);
        break;
    case 2: { /* a crouch, a leap, tucked in the air, and down */
        float crouch = smooth(0, 0.12f, t) * (1 - smooth(0.12f, 0.22f, t)) + smooth(0.85f, 0.95f, t) * (1 - smooth(0.95f, 1.2f, t));
        float stretch = smooth(0.12f, 0.25f, t) * (1 - smooth(0.3f, 0.45f, t)), tuck = smooth(0.3f, 0.45f, t) * (1 - smooth(0.7f, 0.85f, t));

        p->hips[1] = -0.05f * crouch;
        turn(p, KB_HIPS, -10 * tuck, 0, 0);
        turn(p, KB_SPINE, 20 * crouch - 10 * stretch, 0, 0);
        turn(p, KB_CHEST, 8 * crouch, 0, 0);
        turn(p, KB_HEAD, -15 * stretch + 10 * crouch, 0, 0);
        pant(p, t, 0.3f, 10 + 14 * stretch);
        ears(p, t, 1.2f, 0.6f);
        wag(p, t, 0.3f, 20, 30 + 30 * stretch);
        arms(p, -150 * stretch - 60 * tuck + 20 * crouch, 25 * stretch, -40 * tuck, -150 * stretch - 60 * tuck + 20 * crouch, 25 * stretch,
             -40 * tuck);
        legs(p, -55 * crouch - 70 * tuck, 90 * crouch + 100 * tuck, -30 * crouch, -55 * crouch - 70 * tuck, 90 * crouch + 100 * tuck,
             -30 * crouch);
        break;
    }
    case 3: /* the attack: round and round twice, arms flung out */
        k = t / 0.53f;
        p->hips[1] = 0.02f * sinf(3.14159265f * k);
        turn(p, KB_HIPS, 0, 720 * smooth(0, 1, k), 0);
        turn(p, KB_SPINE, 6, 0, 8 * sinf(3.14159265f * k));
        turn(p, KB_HEAD, -8, -20 * sinf(3.14159265f * k), 0);
        pant(p, t, 0.2f, 22);
        ears(p, t, 2, 0.26f);
        wag(p, t, 0.2f, 10, 40);
        arms(p, -20, 14 + 70 * sinf(3.14159265f * k), -20, -20, 14 + 70 * sinf(3.14159265f * k), -20);
        legs(p, -20 * wave(t, 0.26f, 0), 30, 0, 20 * wave(t, 0.26f, 0), 30, 0);
        break;
    case 4: { /* a win: hopping round, arms up, tail going */
        float hop = fabsf(sinf(2 * 3.14159265f * t));

        p->hips[1] = 0.06f * hop;
        turn(p, KB_HIPS, 0, t < 1 ? 360 * smooth(0, 1, t) : 0, 0);
        turn(p, KB_SPINE, -6 * hop, 0, 0);
        turn(p, KB_HEAD, -20, 10 * wave(t, 1, 0), 8 * wave(t, 0.5f, 0));
        pant(p, t, 0.25f, 24);
        ears(p, t, 1.6f, 0.5f);
        wag(p, t, 0.18f, 35, 40);
        arms(p, -160 + 15 * wave(t, 0.5f, 0), 36, -20, -160 - 15 * wave(t, 0.5f, 0), 36, -20);
        legs(p, -30 * hop, 50 * hop, 0, -30 * hop, 50 * hop, 0);
        break;
    }
    case 5: /* hit: thrown back, then back on its feet */
        k = smooth(0, 0.15f, t) * (1 - smooth(0.4f, 0.8f, t));
        p->hips[1] = -0.03f * k;
        p->hips[2] = -0.04f * k;
        turn(p, KB_SPINE, -28 * k, 0, 6 * k);
        turn(p, KB_CHEST, -10 * k, 0, 0);
        turn(p, KB_HEAD, -25 * k, 0, -10 * k);
        pant(p, t, 0.8f, 28 * k);
        ears(p, t, 2.5f * k, 0.3f);
        wag(p, t, 0.8f, 5, 10 - 30 * k);
        arms(p, -100 * k, 10 + 50 * k, -30 * k, -100 * k, 10 + 50 * k, -30 * k);
        legs(p, -40 * k, 50 * k, 0, -20 * k, 30 * k, 0);
        break;
    case 6: { /* a taunt: its back to them, wiggling, then a bark over its shoulder */
        float tn = smooth(0, 0.2f, t) * (1 - smooth(1.3f, 1.5f, t));
        float bark = t > 0.8f && t < 1.3f ? fmaxf(0, sinf(2 * 3.14159265f * (t - 0.8f) / 0.25f)) : 0;

        turn(p, KB_HIPS, 0, 160 * tn + 14 * wave(t, 0.25f, 0) * tn, 10 * wave(t, 0.25f, 0) * tn);
        turn(p, KB_SPINE, 18 * tn, 0, -6 * wave(t, 0.25f, 0) * tn);
        turn(p, KB_CHEST, 0, -40 * tn, 0);
        turn(p, KB_HEAD, -15 * bark, -70 * tn, 0);
        turn(p, KB_JAW, 8 + 26 * bark, 0, 0);
        ears(p, t, 1.2f, 0.25f);
        wag(p, t, 0.12f, 40, 40);
        arms(p, -20, 10, -40, -20, 10, -40);
        legs(p, -10 * tn, 20 * tn, 0, -10 * tn, 20 * tn, 0);
        break;
    }
    case 7: { /* bending to pick up a crate, and up with it */
        float down = smooth(0, 0.25f, t) * (1 - smooth(0.25f, 0.5f, t)), up = smooth(0.25f, 0.5f, t);

        p->hips[1] = -0.04f * down;
        turn(p, KB_SPINE, 35 * down, 0, 0);
        turn(p, KB_CHEST, 20 * down, 0, 0);
        turn(p, KB_HEAD, -20 * down, 0, 0);
        pant(p, t, 0.5f, 10);
        ears(p, t, 0.8f, 0.5f);
        wag(p, t, 0.25f, 20, 30);
        arms(p, -70 * down - 165 * up, 14, -20 * down - 30 * up, -70 * down - 165 * up, 14, -20 * down - 30 * up);
        legs(p, -50 * down, 80 * down, -20 * down, -50 * down, 80 * down, -20 * down);
        break;
    }
    case 8: /* walking with a crate held up */
        s = wave(t, 0.7f, 0);
        p->hips[1] = 0.012f * fabsf(wave(t, 0.7f, 0.25f));
        turn(p, KB_SPINE, -4, -3 * s, 0);
        turn(p, KB_HEAD, -6, 0, 0);
        pant(p, t, 0.35f, 18);
        ears(p, t, 1, 0.35f);
        wag(p, t, 0.35f, 15, 30);
        arms(p, -165, 14, -30, -165, 14, -30);
        legs(p, -30 * s, 40 * fmaxf(0, s), 0, 30 * s, 40 * fmaxf(0, -s), 0);
        break;
    case 9: { /* throwing it */
        float back = smooth(0, 0.2f, t) * (1 - smooth(0.2f, 0.3f, t)), fling = smooth(0.2f, 0.32f, t) * (1 - smooth(0.35f, 0.5f, t));

        turn(p, KB_SPINE, -14 * back + 20 * fling, 0, 0);
        turn(p, KB_HEAD, -10 * back, 0, 0);
        turn(p, KB_JAW, 10 + 20 * fling, 0, 0);
        ears(p, t, 1.4f, 0.4f);
        wag(p, t, 0.25f, 15, 30);
        arms(p, -165 - 20 * back + 90 * fling, 14, -30 * back, -165 - 20 * back + 90 * fling, 14, -30 * back);
        legs(p, -15 * fling, 20 * fling, 0, 10 * fling, 0, 0);
        break;
    }
    }
}

/* --- the pose as turns of the model's own bones --- */

static void dir_of(const KitModel *m, int b, float out[3]) {
    for (int i = 0; i < 3; i++) {
        out[i] = m->bones[b].end[i] - m->bones[b].at[i];
    }
}

/* a limb's two bones aimed: upper by (x forward, z out) from hanging, lower x further, in the frame of `frame` (the world turn of the
 * bone they hang from); the local turns into rot, and the lower's world turn into lower_w */
static void aim(const KitModel *m, int upper, int lower, const Q frame, float x, float out, float lx, int side, float rot[KB_COUNT][4],
                Q lower_w) {
    static const float Down[3] = { 0, -1, 0 };
    float t1[3], t2[3], rest[3];
    Q a, b, up_w, inv;

    euler(a, x, 0, side * out);
    euler(b, lx, 0, 0);
    q_rot(a, Down, t1);
    q_mul(a, b, b);
    q_rot(b, Down, t2);
    q_rot(frame, t1, t1);
    q_rot(frame, t2, t2);
    dir_of(m, upper, rest);
    q_arc(rest, t1, up_w);
    q_inv(frame, inv);
    q_mul(inv, up_w, rot[upper]);
    dir_of(m, lower, rest);
    q_arc(rest, t2, lower_w);
    q_inv(up_w, inv);
    q_mul(inv, lower_w, rot[lower]);
}

void kit_pose(const KitModel *model, int m, float t, float rot[KB_COUNT][4], float hips[3]) {
    float scale = (model->hi[1] - model->lo[1]) / 0.955f;
    Q world[KB_COUNT], lw;
    Pose p;
    int b, side;

    moves(m, t, &p);
    for (b = 0; b < KB_COUNT; b++) {
        rot[b][0] = rot[b][1] = rot[b][2] = 0;
        rot[b][3] = 1;
        if (p.turned[b]) {
            euler(rot[b], p.body[b][0], p.body[b][1], p.body[b][2]);
        }
    }
    for (b = 0; b < 3; b++) {
        hips[b] = p.hips[b] * scale;
    }
    /* the body's bones' world turns, down to the chest */
    memcpy(world[KB_HIPS], rot[KB_HIPS], sizeof(Q));
    q_mul(world[KB_HIPS], rot[KB_SPINE], world[KB_SPINE]);
    q_mul(world[KB_SPINE], rot[KB_CHEST], world[KB_CHEST]);
    for (side = 0; side < 2; side++) {
        float up = fmaxf(p.arm[side][0], -model->arm_raise);
        int u = side ? KB_UPPERARM_R : KB_UPPERARM_L, f = side ? KB_FOREARM_R : KB_FOREARM_L, h = side ? KB_HAND_R : KB_HAND_L;
        int th = side ? KB_THIGH_R : KB_THIGH_L, sh = side ? KB_SHIN_R : KB_SHIN_L, ft = side ? KB_FOOT_R : KB_FOOT_L;

        aim(model, u, f, world[KB_CHEST], up, p.arm[side][1], p.arm[side][2], side ? -1 : 1, rot, lw);
        rot[h][0] = rot[h][1] = rot[h][2] = 0, rot[h][3] = 1;
        aim(model, th, sh, world[KB_HIPS], p.leg[side][0], 3, p.leg[side][1], side ? -1 : 1, rot, lw);
        /* the foot as it rests against its shin, turned up or down */
        euler(rot[ft], p.leg[side][2], 0, 0);
        {
            /* undo the shin's turn from rest, so the foot stays flat as the leg bends, then its own */
            Q rest_w, inv;

            q_mul(world[KB_HIPS], rot[th], rest_w);
            q_mul(rest_w, rot[sh], rest_w);
            q_inv(rest_w, inv);
            q_mul(inv, world[KB_HIPS], inv);
            q_mul(inv, rot[ft], rot[ft]);
        }
    }
}
