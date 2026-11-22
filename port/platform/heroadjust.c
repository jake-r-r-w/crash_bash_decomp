/* A hero pack's posed skeleton adjusted, and its mesh skinned to it (heroadjust.h) */
#include <math.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "heroadjust.h"

/* --- 3-vectors and row-major 3x3s --- */

static void m_apply(const float m[9], const float v[3], float out[3]) {
    float r[3] = { m[0] * v[0] + m[1] * v[1] + m[2] * v[2], m[3] * v[0] + m[4] * v[1] + m[5] * v[2], m[6] * v[0] + m[7] * v[1] + m[8] * v[2] };

    memcpy(out, r, sizeof(r));
}

static void m_mul(const float a[9], const float b[9], float out[9]) {
    float r[9];
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            r[i * 3 + j] = a[i * 3] * b[j] + a[i * 3 + 1] * b[3 + j] + a[i * 3 + 2] * b[6 + j];
        }
    }
    memcpy(out, r, sizeof(r));
}

/* the turn by angle (radians) about a unit axis */
static void m_axis(const float ax[3], float angle, float out[9]) {
    float c = cosf(angle), s = sinf(angle), t = 1 - c, x = ax[0], y = ax[1], z = ax[2];

    out[0] = t * x * x + c, out[1] = t * x * y - s * z, out[2] = t * x * z + s * y;
    out[3] = t * x * y + s * z, out[4] = t * y * y + c, out[5] = t * y * z - s * x;
    out[6] = t * x * z - s * y, out[7] = t * y * z + s * x, out[8] = t * z * z + c;
}

static float v_len(const float v[3]) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/* the shortest turn from direction a to b */
static void m_arc(const float a[3], const float b[3], float out[9]) {
    float la = v_len(a), lb = v_len(b), x[3], y[3], ax[3], c, s;
    int i;

    memset(out, 0, sizeof(float) * 9);
    out[0] = out[4] = out[8] = 1;
    if (la < 1e-6f || lb < 1e-6f) {
        return;
    }
    for (i = 0; i < 3; i++) {
        x[i] = a[i] / la, y[i] = b[i] / lb;
    }
    ax[0] = x[1] * y[2] - x[2] * y[1], ax[1] = x[2] * y[0] - x[0] * y[2], ax[2] = x[0] * y[1] - x[1] * y[0];
    c = x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
    s = v_len(ax);
    if (s < 1e-6f) {
        return;
    }
    for (i = 0; i < 3; i++) {
        ax[i] /= s;
    }
    m_axis(ax, atan2f(s, c), out);
}

/* where a two-bone limb's middle joint goes, from root reaching for target (bones l1 then l2), bent towards hint (packfollow.c's) */
static void two_bone(const float root[3], const float target[3], float l1, float l2, const float hint[3], float out[3]) {
    float d[3], n[3], dist, a, h, hd;
    int i;

    for (i = 0; i < 3; i++) {
        d[i] = target[i] - root[i];
    }
    dist = v_len(d);
    for (i = 0; i < 3 && dist > 1e-6f; i++) {
        d[i] /= dist;
    }
    dist = dist < fabsf(l1 - l2) + 0.01f ? fabsf(l1 - l2) + 0.01f : dist > l1 + l2 - 0.01f ? l1 + l2 - 0.01f : dist;
    a = (l1 * l1 - l2 * l2 + dist * dist) / (2 * dist);
    h = sqrtf(fmaxf(0, l1 * l1 - a * a));
    hd = hint[0] * d[0] + hint[1] * d[1] + hint[2] * d[2];
    for (i = 0; i < 3; i++) {
        n[i] = hint[i] - d[i] * hd;
    }
    hd = v_len(n);
    if (hd > 1e-4f) {
        for (i = 0; i < 3; i++) {
            n[i] /= hd;
        }
    } else {
        n[0] = 0, n[1] = 0, n[2] = -1;
    }
    for (i = 0; i < 3; i++) {
        out[i] = root[i] + d[i] * a + n[i] * h;
    }
}

/* --- the adjustments --- */

int pack_adjust_for(const PackAdjust *a, int kind, const char *move) {
    return a->kind == kind && move != NULL && (strcmp(a->move, "*") == 0 || SDL_strcasecmp(a->move, move) == 0);
}

float pack_adjust_share(const PackAdjust *a, int frame) {
    int d;

    if (a->at < 0) {
        return 1;
    }
    d = frame > a->at ? frame - a->at : a->at - frame;
    if (a->spread <= 0 || d > a->spread) {
        return d == 0 ? 1.0f : 0.0f;
    }
    return 0.5f + 0.5f * cosf(3.14159265f * (float)d / (float)(a->spread + 1));
}

int pack_bone_named(const PackBones *bones, const char *name) {
    int b;

    for (b = 0; b < bones->nbones; b++) {
        if (bones->name[b] != NULL && SDL_strcasecmp(bones->name[b], name) == 0) {
            return b;
        }
    }
    return -1;
}

/* whether bone d is under (or is) bone b */
static int under(const PackBones *bones, int d, int b) {
    int n;

    for (n = 0; d >= 0 && n < PACK_MAX_BONES; n++, d = bones->parent[d]) {
        if (d == b) {
            return 1;
        }
    }
    return 0;
}

void pack_turn_under(const PackBones *bones, int bone, const float M[9], float (*P)[3], float (*Q)[9]) {
    float pivot[3], d[3];
    int k, i;

    memcpy(pivot, P[bone], sizeof(pivot));
    for (k = 0; k < bones->nbones; k++) {
        if (!under(bones, k, bone)) {
            continue;
        }
        for (i = 0; i < 3; i++) {
            d[i] = P[k][i] - pivot[i];
        }
        m_apply(M, d, d);
        for (i = 0; i < 3; i++) {
            P[k][i] = pivot[i] + d[i];
        }
        m_mul(M, Q[k], Q[k]);
    }
}

int pack_adjust_pose(const PackAdjust *adj, int nadj, int kind, const char *move, int frame, const PackBones *bones,
                     const float trunk[9], float (*P)[3], float (*Q)[9]) {
    static float turn[PACK_MAX_BONES][3], reach[PACK_MAX_BONES][3];
    static int depth[PACK_MAX_BONES];
    int i, b, k, any = 0, deepest = 0;

    memset(turn, 0, sizeof(turn));
    memset(reach, 0, sizeof(reach));
    for (i = 0; i < nadj; i++) {
        float w;

        if (!pack_adjust_for(&adj[i], kind, move) || (w = pack_adjust_share(&adj[i], frame)) <= 0
            || (b = pack_bone_named(bones, adj[i].bone)) < 0) {
            continue;
        }
        for (k = 0; k < 3; k++) {
            turn[b][k] += adj[i].turn[k] * w;
            reach[b][k] += adj[i].reach[k] * w;
        }
        any = 1;
    }
    if (!any) {
        return 0;
    }
    for (b = 0; b < bones->nbones; b++) {
        int p = bones->parent[b];

        for (depth[b] = 0; p >= 0 && depth[b] < PACK_MAX_BONES; p = bones->parent[p]) {
            depth[b]++;
        }
        deepest = depth[b] > deepest ? depth[b] : deepest;
    }
    /* the hips' reach (and a root's): all of it moved */
    for (b = 0; b < bones->nbones; b++) {
        float d[3];

        if ((bones->parent[b] >= 0 && SDL_strcasecmp(bones->name[b], "hips") != 0) || v_len(reach[b]) <= 0) {
            continue;
        }
        m_apply(trunk, reach[b], d);
        for (k = 0; k < bones->nbones; k++) {
            for (i = 0; i < 3; i++) {
                P[k][i] += d[i];
            }
        }
    }
    /* each bone's turn, those nearest the hips first: its turn's axis is the hero's own (trunk), so it's trunk * turn * trunk' */
    for (i = 0; i <= deepest; i++) {
        for (b = 0; b < bones->nbones; b++) {
            float len = v_len(turn[b]), ax[3], M[9];

            if (depth[b] != i || len <= 1e-4f) {
                continue;
            }
            for (k = 0; k < 3; k++) {
                ax[k] = turn[b][k] / len;
            }
            m_apply(trunk, ax, ax);
            m_axis(ax, len * 3.14159265f / 180, M);
            pack_turn_under(bones, b, M, P, Q);
        }
    }
    /* each hand's and foot's reach (any bone two down from another): its arm or leg bent to reach there */
    for (b = 0; b < bones->nbones; b++) {
        int mid = bones->parent[b], root = mid >= 0 ? bones->parent[mid] : -1;
        float target[3], hint[3], l1, l2, d1[3], d2[3], knee[3], M[9];

        if (root < 0 || v_len(reach[b]) <= 0 || SDL_strcasecmp(bones->name[b], "hips") == 0) {
            continue;
        }
        m_apply(trunk, reach[b], target);
        for (k = 0; k < 3; k++) {
            target[k] += P[b][k];
            d1[k] = P[mid][k] - P[root][k];
            d2[k] = P[b][k] - P[mid][k];
            hint[k] = P[mid][k] - (P[root][k] + P[b][k]) / 2;
        }
        l1 = v_len(d1), l2 = v_len(d2);
        if (l1 < 1e-4f || l2 < 1e-4f) {
            continue;
        }
        if (v_len(hint) < 1e-3f) {
            float back[3] = { 0, 0, -1 };

            m_apply(trunk, back, hint);
        }
        two_bone(P[root], target, l1, l2, hint, knee);
        for (k = 0; k < 3; k++) {
            d2[k] = knee[k] - P[root][k];
        }
        m_arc(d1, d2, M);
        pack_turn_under(bones, root, M, P, Q);
        for (k = 0; k < 3; k++) {
            d1[k] = P[b][k] - P[mid][k];
            d2[k] = target[k] - P[mid][k];
        }
        m_arc(d1, d2, M);
        pack_turn_under(bones, mid, M, P, Q);
    }
    return 1;
}

/* --- the mesh --- */

void pack_skin_posed(const PackSkin *s, const float (*P)[3], const float (*Q)[9], const float hips_at[3], int cut, float *out) {
    int v, j, i;

    for (v = 0; v < s->nvert; v++) {
        float at[3] = { 0, 0, 0 }, lower = 0, tail = 0, arm = 0, d[3];

        for (j = 0; j < 4; j++) {
            int b = s->joint[v * 4 + j];
            float w = s->weight[v * 4 + j];

            if (w <= 0) {
                continue;
            }
            for (i = 0; i < 3; i++) {
                d[i] = s->rest[v * 3 + i] - s->bone[b * 3 + i];
            }
            m_apply(Q[b], d, d);
            for (i = 0; i < 3; i++) {
                at[i] += (P[b][i] + d[i]) * w;
            }
            lower += s->part[b] == 1 ? w : 0;
            tail += s->part[b] == 2 ? w : 0;
            arm += s->part[b] == 3 ? w : 0;
        }
        if (cut && tail > 0.5f) {
            at[0] = hips_at[0], at[1] = hips_at[1] + 8, at[2] = hips_at[2];
        } else if (cut && (lower > 0.5f || (arm < 0.5f && at[1] > hips_at[1] + 4))) {
            float dx = at[0] - hips_at[0], dz = at[2] - hips_at[2], dd = sqrtf(dx * dx + dz * dz);

            if (dd > PACK_HATCH_HALF) {
                at[0] = hips_at[0] + dx * PACK_HATCH_HALF / dd;
                at[2] = hips_at[2] + dz * PACK_HATCH_HALF / dd;
            }
            at[1] = at[1] > hips_at[1] + 4 ? at[1] : hips_at[1] + 4;
        }
        memcpy(&out[v * 3], at, sizeof(at));
    }
}
