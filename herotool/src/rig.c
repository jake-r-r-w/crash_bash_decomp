/* A model's skeleton and weights (kit.h).
 *
 * The skeleton is the standard one (KitBoneNames), placed from markers: a JSON file of where each bone starts in the model, by name,
 * and where a bone at a chain's end ends ("head_tip", "hand_l_tip", ...), in the model's own units, as the hero tool's window lets
 * them be dragged. The first 17 bones are needed; a jaw, ears and a tail are the model's if it has them. "arm_raise" (degrees) says
 * how high its arms can go before its paws are in its head (160 by default: straight up).
 *
 * Each corner's bones, for any model (no rules about where its parts are): its side's limbs and ears and the bones down its middle are
 * all it can have (a corner of the left half never goes with the right arm); each place on its surface starts with the bone it's
 * nearest, and every place goes with the two it's nearest over the surface from those (so an ear hanging beside a chest goes with the
 * ear, as the surface reaches it only through the ear's root), smoothed a little, and every copy of a corner at one place (the
 * texture's seams) the same, or they'd come apart as it moves. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "kit.h"

const char *const KitBoneNames[KB_COUNT] = {
    "hips", "spine", "chest", "neck", "head",
    "upperarm_l", "forearm_l", "hand_l", "upperarm_r", "forearm_r", "hand_r",
    "thigh_l", "shin_l", "foot_l", "thigh_r", "shin_r", "foot_r",
    "jaw", "ear_l1", "ear_l2", "ear_r1", "ear_r2", "tail1", "tail2", "tail3",
};
const int KitBoneParent[KB_COUNT] = {
    -1, KB_HIPS, KB_SPINE, KB_CHEST, KB_NECK,
    KB_CHEST, KB_UPPERARM_L, KB_FOREARM_L, KB_CHEST, KB_UPPERARM_R, KB_FOREARM_R,
    KB_HIPS, KB_THIGH_L, KB_SHIN_L, KB_HIPS, KB_THIGH_R, KB_SHIN_R,
    KB_HEAD, KB_HEAD, KB_EAR_L1, KB_HEAD, KB_EAR_R1, KB_HIPS, KB_TAIL1, KB_TAIL2,
};
/* which side a bone's on: 1 his left (+x), -1 his right, 0 down his middle */
static const int Side[KB_COUNT] = {
    0, 0, 0, 0, 0, 1, 1, 1, -1, -1, -1, 1, 1, 1, -1, -1, -1, 0, 1, 1, -1, -1, 0, 0, 0,
};

/* How thick a bone of each kind is against a limb's: the trunk is wide round its bones, which run down its middle, and a head round
 * its; so where a corner is nearest straight, a trunk bone's distance counts for this much of a limb's (a chest's skin is further from
 * the chest's bone than from a jaw beside it, but it's the chest's) */
static const float Wide[KB_COUNT] = {
    2.5f, 2.5f, 2.5f, 1.5f, 1.6f, 1, 1, 1, 1, 1, 1, 1.2f, 1, 1, 1.2f, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

/* "name": [x, y, z] in the JSON, into out; whether it's there */
static int find_point(const char *text, const char *name, float out[3]) {
    char key[64];
    const char *at;

    snprintf(key, sizeof(key), "\"%s\"", name);
    if ((at = strstr(text, key)) == NULL || (at = strchr(at + strlen(key), '[')) == NULL) {
        return 0;
    }
    return sscanf(at + 1, "%f , %f , %f", &out[0], &out[1], &out[2]) == 3;
}

int kit_markers(KitModel *m, const char *path, char *why, size_t why_size) {
    char *text = SDL_LoadFile(path, NULL), key[64];
    const char *at;
    int b, c;

    if (text == NULL) {
        snprintf(why, why_size, "can't read the markers, %s", path);
        return 0;
    }
    for (b = 0; b < KB_COUNT; b++) {
        m->bones[b].have = find_point(text, KitBoneNames[b], m->bones[b].at);
        if (!m->bones[b].have && b < KB_NEEDED) {
            snprintf(why, why_size, "the markers have no %s: it needs %s to %s at least", KitBoneNames[b], KitBoneNames[0],
                     KitBoneNames[KB_NEEDED - 1]);
            SDL_free(text);
            return 0;
        }
    }
    /* each bone's end: its tip if the markers give one, else its first child's start, else a little up from it */
    for (b = 0; b < KB_COUNT; b++) {
        if (!m->bones[b].have) {
            continue;
        }
        snprintf(key, sizeof(key), "%s_tip", KitBoneNames[b]);
        if (find_point(text, key, m->bones[b].end)) {
            continue;
        }
        for (c = 0; c < KB_COUNT && !(m->bones[c].have && KitBoneParent[c] == b); c++) {
        }
        if (c < KB_COUNT && b != KB_CHEST && b != KB_HIPS && b != KB_HEAD) {
            memcpy(m->bones[b].end, m->bones[c].at, sizeof(m->bones[b].end));
        } else if (b == KB_CHEST && m->bones[KB_NECK].have) {
            memcpy(m->bones[b].end, m->bones[KB_NECK].at, sizeof(m->bones[b].end));
        } else if (b == KB_HIPS) {
            memcpy(m->bones[b].end, m->bones[KB_SPINE].at, sizeof(m->bones[b].end));
        } else {
            memcpy(m->bones[b].end, m->bones[b].at, sizeof(m->bones[b].end));
            m->bones[b].end[1] += (m->hi[1] - m->lo[1]) * 0.1f;
        }
    }
    if ((at = strstr(text, "\"arm_raise\"")) != NULL && (at = strchr(at, ':')) != NULL) {
        m->arm_raise = (float)SDL_strtod(at + 1, NULL);
    }
    SDL_free(text);
    return 1;
}

static float seg_dist(const float p[3], const float a[3], const float b[3]) {
    float ab[3], ap[3], t, d = 0, q;
    int i;

    for (i = 0; i < 3; i++) {
        ab[i] = b[i] - a[i];
        ap[i] = p[i] - a[i];
    }
    t = ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2];
    t = t > 0 ? (ap[0] * ab[0] + ap[1] * ab[1] + ap[2] * ab[2]) / t : 0;
    t = t < 0 ? 0 : t > 1 ? 1 : t;
    for (i = 0; i < 3; i++) {
        q = p[i] - (a[i] + ab[i] * t);
        d += q * q;
    }
    return sqrtf(d);
}

typedef struct {
    float w[KB_COUNT];
} Weights;

void kit_weigh(KitModel *m) {
    float height = m->hi[1] - m->lo[1], middle = (m->hi[0] - m->lo[0]) * 0.02f, eps = height * 0.01f;
    Weights *W = calloc(m->nvert, sizeof(Weights)), *next = calloc(m->nvert, sizeof(Weights));
    int *same = malloc(sizeof(int) * m->nvert), *nearest = malloc(sizeof(int) * m->nvert);
    float *geo = malloc(sizeof(float) * (size_t)m->nvert * KB_COUNT), *dist = malloc(sizeof(float) * m->nvert);
    uint8_t *done = malloc(m->nvert);
    int *nn = calloc(m->nvert, sizeof(int)), (*nb)[24] = calloc(m->nvert, sizeof(*nb));
    int v, b, pass, i, j, seeds[KB_COUNT] = { 0 };

    /* the bones a corner can go with, its side's and its middle's */
#define CAN(v, b) (m->bones[b].have && (Side[b] == 0 || (Side[b] > 0 ? m->pos[v][0] > -middle : m->pos[v][0] < middle)))
    /* its surface: each place once (its copies across the texture's seams), and the places next to it */
    for (v = 0; v < m->nvert; v++) {
        same[v] = v;
        for (j = 0; j < v; j++) {
            if (fabsf(m->pos[j][0] - m->pos[v][0]) < 1e-5f * height && fabsf(m->pos[j][1] - m->pos[v][1]) < 1e-5f * height
                && fabsf(m->pos[j][2] - m->pos[v][2]) < 1e-5f * height) {
                same[v] = same[j];
                break;
            }
        }
    }
    for (i = 0; i < m->ntri; i++) {
        for (j = 0; j < 3; j++) {
            int a = same[m->tri[i][j]], c = same[m->tri[i][(j + 1) % 3]], k;

            for (k = 0; k < nn[a] && nb[a][k] != c; k++) {
            }
            if (k == nn[a] && nn[a] < 24) {
                nb[a][nn[a]++] = c;
            }
            for (k = 0; k < nn[c] && nb[c][k] != a; k++) {
            }
            if (k == nn[c] && nn[c] < 24) {
                nb[c][nn[c]++] = a;
            }
        }
    }
    /* each place's nearest bone, straight, for its thickness */
    for (v = 0; v < m->nvert; v++) {
        float best = 1e30f;

        nearest[v] = KB_HIPS;
        for (b = 0; same[v] == v && b < KB_COUNT; b++) {
            float d;

            if (CAN(v, b) && (d = seg_dist(m->pos[v], m->bones[b].at, m->bones[b].end) / Wide[b]) < best) {
                best = d;
                nearest[v] = b;
            }
        }
    }
    /* how far each place is from each bone over the surface, from the places nearest it (Dijkstra: few places, so the plain way) */
    for (b = 0; b < KB_COUNT; b++) {
        for (v = 0; v < m->nvert; v++) {
            dist[v] = same[v] == v && nearest[v] == b ? 0 : 1e30f;
            done[v] = same[v] != v;
            seeds[b] += same[v] == v && nearest[v] == b;
        }
        for (;;) {
            int at = -1;

            for (v = 0; v < m->nvert; v++) {
                if (!done[v] && dist[v] < 1e29f && (at < 0 || dist[v] < dist[at])) {
                    at = v;
                }
            }
            if (at < 0) {
                break;
            }
            done[at] = 1;
            for (j = 0; j < nn[at]; j++) {
                int u = nb[at][j];
                float e = sqrtf((m->pos[u][0] - m->pos[at][0]) * (m->pos[u][0] - m->pos[at][0])
                                + (m->pos[u][1] - m->pos[at][1]) * (m->pos[u][1] - m->pos[at][1])
                                + (m->pos[u][2] - m->pos[at][2]) * (m->pos[u][2] - m->pos[at][2]));

                if (dist[at] + e < dist[u]) {
                    dist[u] = dist[at] + e;
                }
            }
        }
        for (v = 0; v < m->nvert; v++) {
            geo[(size_t)v * KB_COUNT + b] = CAN(v, b) && seeds[b] > 0 ? dist[v] : 1e30f;
        }
    }
    /* each place with the two it's nearest over the surface: all its own bone's where it's nearest, sharing at the joins */
    for (v = 0; v < m->nvert; v++) {
        int b1 = -1, b2 = -1;
        float g1 = 1e30f, g2 = 1e30f, w1, w2;

        if (same[v] != v) {
            continue;
        }
        for (b = 0; b < KB_COUNT; b++) {
            float g = geo[(size_t)v * KB_COUNT + b];

            if (g < g1) {
                g2 = g1, b2 = b1, g1 = g, b1 = b;
            } else if (g < g2) {
                g2 = g, b2 = b;
            }
        }
        if (b1 < 0) {
            W[v].w[KB_HIPS] = 1;
            continue;
        }
        w1 = 1 / ((g1 + eps) * (g1 + eps));
        w2 = b2 >= 0 && g2 < 1e29f ? 1 / ((g2 + eps) * (g2 + eps)) : 0;
        W[v].w[b1] = w1 / (w1 + w2);
        if (b2 >= 0 && w2 > 0) {
            W[v].w[b2] = w2 / (w1 + w2);
        }
    }
    /* smoothed over the surface a little */
    for (pass = 0; pass < 2; pass++) {
        for (v = 0; v < m->nvert; v++) {
            if (same[v] != v) {
                continue;
            }
            for (b = 0; b < KB_COUNT; b++) {
                float s = 0;

                for (j = 0; j < nn[v]; j++) {
                    s += W[nb[v][j]].w[b];
                }
                next[v].w[b] = nn[v] > 0 ? W[v].w[b] * 0.5f + s * 0.5f / nn[v] : W[v].w[b];
            }
        }
        for (v = 0; v < m->nvert; v++) {
            if (same[v] == v) {
                W[v] = next[v];
            }
        }
    }
    /* the four biggest of each place's, for each of its copies */
    for (v = 0; v < m->nvert; v++) {
        const Weights *w = &W[same[v]];
        int took[KB_COUNT] = { 0 };
        float sum = 0;

        for (i = 0; i < 4; i++) {
            int best = -1;

            for (b = 0; b < KB_COUNT; b++) {
                if (!took[b] && w->w[b] > 0 && (best < 0 || w->w[b] > w->w[best])) {
                    best = b;
                }
            }
            m->joint[v][i] = (uint8_t)(best < 0 ? 0 : best);
            m->weight[v][i] = best < 0 ? 0 : w->w[best];
            if (best >= 0) {
                took[best] = 1;
                sum += m->weight[v][i];
            }
        }
        for (i = 0; i < 4 && sum > 0; i++) {
            m->weight[v][i] /= sum;
        }
    }
#undef CAN
    for (b = 0; b < KB_COUNT; b++) {
        if (m->bones[b].have && seeds[b] == 0) {
            printf("herotool: no corner is nearest its %s: check its marker\n", KitBoneNames[b]);
        }
    }
    free(W);
    free(next);
    free(nearest);
    free(same);
    free(geo);
    free(dist);
    free(done);
    free(nn);
    free(nb);
}
