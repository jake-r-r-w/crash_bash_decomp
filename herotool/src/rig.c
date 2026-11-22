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
    2.5f, 2.5f, 2.5f, 1.5f, 1.6f, 1, 1, 1, 1, 1, 1, 1.2f, 1, 1, 1.2f, 1, 1, 1, 1.6f, 1.6f, 1.6f, 1.6f, 1, 1, 1,
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

/* What hangs off a hero (its ears, jaw and tail): each part's bones (first to last) and the bone it hangs from */
static const struct {
    int first, last, from;
} Hanging[] = {
    { KB_EAR_L1, KB_EAR_L2, KB_HEAD }, { KB_EAR_R1, KB_EAR_R2, KB_HEAD }, { KB_JAW, KB_JAW, KB_HEAD }, { KB_TAIL1, KB_TAIL3, KB_HIPS },
};

/* which of those parts a bone's in, or -1 */
static int hanging(int b) {
    int i;

    for (i = 0; i < (int)(sizeof(Hanging) / sizeof(Hanging[0])); i++) {
        if (b >= Hanging[i].first && b <= Hanging[i].last) {
            return i;
        }
    }
    return -1;
}

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
    /* what hangs off it (an ear, the jaw, the tail) goes only with its own bones and the one it hangs from, and the rest of it never
     * with those: its second nearest bone over the surface, and the smoothing, gave an ear's lower half its chest and arms, so its ears
     * went with its shoulders (the user's word, 2026-11-22) */
    for (v = 0; v < m->nvert; v++) {
        int top = 0, part, in, from;
        float sum = 0;

        if (same[v] != v) {
            continue;
        }
        for (b = 1; b < KB_COUNT; b++) {
            top = W[v].w[b] > W[v].w[top] ? b : top;
        }
        part = hanging(top);
        for (b = 0; b < KB_COUNT; b++) {
            in = hanging(b);
            from = in >= 0 ? Hanging[in].from : -1;
            /* a part's corner: its part and what it hangs from; any other corner: no part's bones, but where it's what they hang from */
            if (part >= 0 ? !(in == part || b == Hanging[part].from) : (in >= 0 && top != from)) {
                W[v].w[b] = 0;
            }
            sum += W[v].w[b];
        }
        for (b = 0; b < KB_COUNT && sum > 0; b++) {
            W[v].w[b] /= sum;
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

/* --- a model's own rig, by its bones' names --- */

/* Each standard bone's names in the rigs people make (Mixamo's, VRM's, Blender's and Rigify's, Unity's humanoid), as compared: small
 * letters, digits only, anything before a ':' dropped ("mixamorig:LeftArm" is "leftarm", "upper_arm.L" "upperarml"). The first a model
 * has is the one. */
static const char *const Aliases[KB_COUNT][8] = {
    { "hips", "pelvis", "hip" },
    { "spine", "spine01", "abdomen" },
    { "spine2", "upperchest", "chest", "spine02", "spine1", "spine003", "spine002" },
    { "neck", "neck01", "neck1" },
    { "head" },
    { "leftarm", "upperarml", "leftupperarm", "upperarmleft", "luparm", "lupperarm", "armupperl" },
    { "leftforearm", "forearml", "leftlowerarm", "lowerarml", "lforearm", "lowerarmleft", "armlowerl" },
    { "lefthand", "handl", "lhand", "handleft" },
    { "rightarm", "upperarmr", "rightupperarm", "upperarmright", "ruparm", "rupperarm", "armupperr" },
    { "rightforearm", "forearmr", "rightlowerarm", "lowerarmr", "rforearm", "lowerarmright", "armlowerr" },
    { "righthand", "handr", "rhand", "handright" },
    { "leftupleg", "thighl", "leftupperleg", "upperlegl", "lthigh", "upperlegleft", "legupperl" },
    { "leftleg", "shinl", "leftlowerleg", "lowerlegl", "calfl", "lshin", "lowerlegleft", "leglowerl" },
    { "leftfoot", "footl", "lfoot", "footleft" },
    { "rightupleg", "thighr", "rightupperleg", "upperlegr", "rthigh", "upperlegright", "legupperr" },
    { "rightleg", "shinr", "rightlowerleg", "lowerlegr", "calfr", "rshin", "lowerlegright", "leglowerr" },
    { "rightfoot", "footr", "rfoot", "footright" },
    { "jaw", "jaw01", "lowerjaw" },
    { "earl1", "earl", "leftear", "ear1l", "earleft" },
    { "earl2", "ear2l", "leftear2", "earl001" },
    { "earr1", "earr", "rightear", "ear1r", "earright" },
    { "earr2", "ear2r", "rightear2", "earr001" },
    { "tail1", "tail", "tail01", "tail0" },
    { "tail2", "tail02", "tail001" },
    { "tail3", "tail03", "tail002" },
};

static void plain_name(const char *name, char *out, size_t size) {
    const char *at = strrchr(name, ':');
    size_t n = 0;

    for (at = at != NULL ? at + 1 : name; *at != 0 && n + 1 < size; at++) {
        char c = (char)SDL_tolower((unsigned char)*at);

        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            out[n++] = c;
        }
    }
    out[n] = 0;
}

/* whether bone c carries on from b (the bone after it in its chain), not one that branches off it */
static int carries_on(int b, int c) {
    return KitBoneParent[c] == b && c != KB_UPPERARM_L && c != KB_UPPERARM_R && c != KB_JAW && c != KB_EAR_L1 && c != KB_EAR_R1
        && c != KB_THIGH_L && c != KB_THIGH_R && c != KB_TAIL1;
}

/* each bone's end: the start of the one after it, else on along its parent's line by half (a hand past its forearm), else up a little */
static void ends(KitModel *m) {
    int b, c, i;

    for (b = 0; b < KB_COUNT; b++) {
        int par = KitBoneParent[b];

        if (!m->bones[b].have) {
            continue;
        }
        for (c = 0; c < KB_COUNT && !(m->bones[c].have && carries_on(b, c)); c++) {
        }
        if (c < KB_COUNT) {
            memcpy(m->bones[b].end, m->bones[c].at, sizeof(m->bones[b].end));
        } else if (par >= 0 && b != KB_HEAD) {
            for (i = 0; i < 3; i++) {
                m->bones[b].end[i] = m->bones[b].at[i] + (m->bones[b].at[i] - m->bones[par].at[i]) * 0.5f;
            }
        } else {
            memcpy(m->bones[b].end, m->bones[b].at, sizeof(m->bones[b].end));
            m->bones[b].end[1] += (m->hi[1] - m->lo[1]) * 0.1f;
        }
    }
}

int kit_map_rig(KitModel *m, char *why, size_t why_size) {
    int map[PACK_MAX_BONES], b, j, k, v;
    char plain[PACK_MAX_BONES][PACK_ANIM_NAME];

    if (!m->skinned) {
        snprintf(why, why_size, "the model has no rig of its own: give it markers (--markers), where its joints are");
        return 0;
    }
    for (j = 0; j < m->nsrc; j++) {
        plain_name(m->src_name[j], plain[j], sizeof(plain[j]));
        map[j] = -1;
    }
    for (b = 0; b < KB_COUNT; b++) {
        m->bones[b].have = 0;
        for (k = 0; k < 8 && Aliases[b][k] != NULL && !m->bones[b].have; k++) {
            for (j = 0; j < m->nsrc; j++) {
                if (map[j] < 0 && strcmp(plain[j], Aliases[b][k]) == 0) {
                    map[j] = b;
                    m->src_of[b] = j;
                    m->bones[b].have = 1;
                    memcpy(m->bones[b].at, m->src_at[j], sizeof(m->bones[b].at));
                    break;
                }
            }
        }
        if (!m->bones[b].have && b < KB_NEEDED) {
            snprintf(why, why_size,
                     "its rig has no bone the tool knows as the %s (it knows Mixamo's, VRM's, Blender's and Unity's names): rename it, or "
                     "give markers (--markers)",
                     KitBoneNames[b]);
            return 0;
        }
    }
    ends(m);
    /* each vertex's weights onto the standard bones: a bone of its rig that isn't one (a finger, a twist) gives its weight to the
     * nearest one it hangs from */
    for (v = 0; v < m->nvert; v++) {
        float w[KB_COUNT] = { 0 }, sum = 0;
        int took[KB_COUNT] = { 0 };

        for (k = 0; k < 4; k++) {
            int s = m->src_joint[v][k];

            if (m->src_weight[v][k] <= 0) {
                continue;
            }
            while (s >= 0 && s < m->nsrc && map[s] < 0) {
                s = m->src_parent[s];
            }
            w[s >= 0 && s < m->nsrc ? map[s] : KB_HIPS] += m->src_weight[v][k];
        }
        for (k = 0; k < 4; k++) {
            int best = -1;

            for (b = 0; b < KB_COUNT; b++) {
                if (!took[b] && w[b] > 0 && (best < 0 || w[b] > w[best])) {
                    best = b;
                }
            }
            m->joint[v][k] = (uint8_t)(best < 0 ? 0 : best);
            m->weight[v][k] = best < 0 ? 0 : w[best];
            if (best >= 0) {
                took[best] = 1;
                sum += w[best];
            }
        }
        for (k = 0; k < 4 && sum > 0; k++) {
            m->weight[v][k] /= sum;
        }
        if (sum <= 0) {
            m->joint[v][0] = KB_HIPS;
            m->weight[v][0] = 1;
        }
    }
    for (b = 0, k = 0; b < KB_COUNT; b++) {
        k += m->bones[b].have;
    }
    printf("herotool: its own rig, %d bones, mapped onto %d of the standard skeleton's, its weights kept\n", m->nsrc, k);
    return 1;
}

/* --- markers: saved, and a first guess --- */

int kit_save_markers(const KitModel *m, const char *path) {
    SDL_IOStream *io = SDL_IOFromFile(path, "w");
    int b, first = 1;

    if (io == NULL) {
        return 0;
    }
    SDL_IOprintf(io, "{\n");
    for (b = 0; b < KB_COUNT; b++) {
        const KitBone *k = &m->bones[b];

        if (!k->have) {
            continue;
        }
        SDL_IOprintf(io, "%s  \"%s\": [%.4f, %.4f, %.4f], \"%s_tip\": [%.4f, %.4f, %.4f]", first ? "" : ",\n", KitBoneNames[b], k->at[0],
                     k->at[1], k->at[2], KitBoneNames[b], k->end[0], k->end[1], k->end[2]);
        first = 0;
    }
    SDL_IOprintf(io, ",\n  \"arm_raise\": %.0f\n}\n", m->arm_raise);
    SDL_CloseIO(io);
    return 1;
}

/* the middle of the model across and through at a height (its corners within a slab there), for a joint down its middle */
static void middle_at(const KitModel *m, float y, float slab, float *x, float *z) {
    float sx = 0, sz = 0;
    int n = 0, v;

    for (v = 0; v < m->nvert; v++) {
        if (fabsf(m->pos[v][1] - y) < slab) {
            sx += m->pos[v][0];
            sz += m->pos[v][2];
            n++;
        }
    }
    *x = n > 0 ? sx / n : (m->lo[0] + m->hi[0]) / 2;
    *z = n > 0 ? sz / n : (m->lo[2] + m->hi[2]) / 2;
}

void kit_guess_markers(KitModel *m) {
    float h = m->hi[1] - m->lo[1], y0 = m->lo[1], w = m->hi[0] - m->lo[0], cx, cz, x, z;
    int tpose = w > 0.75f * h, b, s;
    /* a standing humanoid's joints, as shares of its height up from its feet, and out from its middle */
    static const struct {
        int bone;
        float y, x, end_y, end_x, end_z;
    } Body[] = {
        { KB_HIPS, 0.50f, 0, 0.60f, 0, 0 }, { KB_SPINE, 0.60f, 0, 0.70f, 0, 0 }, { KB_CHEST, 0.70f, 0, 0.82f, 0, 0 },
        { KB_NECK, 0.82f, 0, 0.87f, 0, 0 }, { KB_HEAD, 0.87f, 0, 1.0f, 0, 0 },
    };

    memset(m->bones, 0, sizeof(m->bones));
    m->arm_raise = 160;
    for (b = 0; b < (int)(sizeof(Body) / sizeof(Body[0])); b++) {
        KitBone *k = &m->bones[Body[b].bone];

        middle_at(m, y0 + Body[b].y * h, h * 0.04f, &x, &z);
        k->at[0] = x, k->at[1] = y0 + Body[b].y * h, k->at[2] = z;
        middle_at(m, y0 + Body[b].end_y * h, h * 0.04f, &x, &z);
        k->end[0] = x, k->end[1] = y0 + Body[b].end_y * h, k->end[2] = z;
        k->have = 1;
    }
    middle_at(m, y0 + 0.8f * h, h * 0.04f, &cx, &cz);
    for (s = 0; s < 2; s++) {
        float side = s == 0 ? 1.0f : -1.0f;
        int up = s ? KB_UPPERARM_R : KB_UPPERARM_L, fo = s ? KB_FOREARM_R : KB_FOREARM_L, ha = s ? KB_HAND_R : KB_HAND_L;
        int th = s ? KB_THIGH_R : KB_THIGH_L, sh = s ? KB_SHIN_R : KB_SHIN_L, ft = s ? KB_FOOT_R : KB_FOOT_L;
        float sy = y0 + 0.80f * h, ax = 0.11f * h;
        /* the arms out in a T, or hanging */
        float elbow[3] = { cx + side * (tpose ? 0.28f : 0.15f) * h, tpose ? sy : y0 + 0.64f * h, cz };
        float hand[3] = { cx + side * (tpose ? 0.42f : 0.16f) * h, tpose ? sy : y0 + 0.50f * h, cz };
        float tip[3] = { cx + side * (tpose ? 0.50f : 0.16f) * h, tpose ? sy : y0 + 0.42f * h, cz };
        float hip[3], knee[3], ankle[3];

        m->bones[up].at[0] = cx + side * ax, m->bones[up].at[1] = sy, m->bones[up].at[2] = cz;
        memcpy(m->bones[up].end, elbow, sizeof(elbow));
        memcpy(m->bones[fo].at, elbow, sizeof(elbow));
        memcpy(m->bones[fo].end, hand, sizeof(hand));
        memcpy(m->bones[ha].at, hand, sizeof(hand));
        memcpy(m->bones[ha].end, tip, sizeof(tip));
        middle_at(m, y0 + 0.48f * h, h * 0.03f, &x, &z);
        hip[0] = x + side * 0.06f * h, hip[1] = y0 + 0.48f * h, hip[2] = z;
        knee[0] = x + side * 0.065f * h, knee[1] = y0 + 0.27f * h, knee[2] = z;
        ankle[0] = x + side * 0.065f * h, ankle[1] = y0 + 0.05f * h, ankle[2] = z;
        memcpy(m->bones[th].at, hip, sizeof(hip));
        memcpy(m->bones[th].end, knee, sizeof(knee));
        memcpy(m->bones[sh].at, knee, sizeof(knee));
        memcpy(m->bones[sh].end, ankle, sizeof(ankle));
        memcpy(m->bones[ft].at, ankle, sizeof(ankle));
        m->bones[ft].end[0] = ankle[0], m->bones[ft].end[1] = y0, m->bones[ft].end[2] = ankle[2] + 0.08f * h;
        m->bones[up].have = m->bones[fo].have = m->bones[ha].have = 1;
        m->bones[th].have = m->bones[sh].have = m->bones[ft].have = 1;
    }
}

/* --- a model's own clips, on the standard skeleton --- */

/* the turn in a matrix, its scale taken out (column-major, as glTF's) */
static void mat_quat(const float m[16], float q[4]) {
    float r[3][3], tr, s;
    int c, i;

    for (c = 0; c < 3; c++) {
        float len = sqrtf(m[c * 4] * m[c * 4] + m[c * 4 + 1] * m[c * 4 + 1] + m[c * 4 + 2] * m[c * 4 + 2]);

        for (i = 0; i < 3; i++) {
            r[i][c] = len > 0 ? m[c * 4 + i] / len : (i == c);
        }
    }
    tr = r[0][0] + r[1][1] + r[2][2];
    if (tr > 0) {
        s = sqrtf(tr + 1) * 2;
        q[3] = s / 4, q[0] = (r[2][1] - r[1][2]) / s, q[1] = (r[0][2] - r[2][0]) / s, q[2] = (r[1][0] - r[0][1]) / s;
    } else if (r[0][0] > r[1][1] && r[0][0] > r[2][2]) {
        s = sqrtf(1 + r[0][0] - r[1][1] - r[2][2]) * 2;
        q[3] = (r[2][1] - r[1][2]) / s, q[0] = s / 4, q[1] = (r[0][1] + r[1][0]) / s, q[2] = (r[0][2] + r[2][0]) / s;
    } else if (r[1][1] > r[2][2]) {
        s = sqrtf(1 + r[1][1] - r[0][0] - r[2][2]) * 2;
        q[3] = (r[0][2] - r[2][0]) / s, q[0] = (r[0][1] + r[1][0]) / s, q[1] = s / 4, q[2] = (r[1][2] + r[2][1]) / s;
    } else {
        s = sqrtf(1 + r[2][2] - r[0][0] - r[1][1]) * 2;
        q[3] = (r[1][0] - r[0][1]) / s, q[0] = (r[0][2] + r[2][0]) / s, q[1] = (r[1][2] + r[2][1]) / s, q[2] = s / 4;
    }
}

static void qmul4(const float a[4], const float b[4], float o[4]) {
    float r[4] = { a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1], a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0],
                   a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3], a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2] };

    memcpy(o, r, sizeof(r));
}

/* Each standard bone turns in the world as its bone in the model's rig turns from its rest (the standard bones rest unturned at
 * its joints): so its local turn is its parent's world turn undone, then its own. A standard bone the rig hasn't turns with its
 * parent. */
int kit_clip_pose(const KitModel *m, int clip, float t, float rot[KB_COUNT][4], float hips[3]) {
    static PackTrs local[PACK_MAX_BONES];
    static float world[PACK_MAX_BONES][16];
    float d[KB_COUNT][4];
    int b, i;

    if (m->src == NULL || clip < 0 || clip >= m->src->nclips || m->src_of[KB_HIPS] < 0) {
        return 0;
    }
    pack_pose_local(m->src, clip, t, local);
    pack_pose_world(m->src, local, world);
    for (b = 0; b < KB_COUNT; b++) {
        int j = m->src_of[b], p = KitBoneParent[b];
        float qw[4], qr[4];

        if (j < 0) {
            if (p >= 0) {
                memcpy(d[b], d[p], sizeof(d[b]));
            } else {
                d[b][0] = d[b][1] = d[b][2] = 0, d[b][3] = 1;
            }
        } else {
            mat_quat(world[j], qw);
            mat_quat(m->src->bones[j].rest_world, qr);
            qr[0] = -qr[0], qr[1] = -qr[1], qr[2] = -qr[2];
            qmul4(qw, qr, d[b]);
        }
        if (p < 0) {
            memcpy(rot[b], d[b], sizeof(rot[b]));
        } else {
            float inv[4] = { -d[p][0], -d[p][1], -d[p][2], d[p][3] };

            qmul4(inv, d[b], rot[b]);
        }
    }
    for (i = 0; i < 3; i++) {
        hips[i] = world[m->src_of[KB_HIPS]][12 + i] - m->src->bones[m->src_of[KB_HIPS]].rest_world[12 + i];
    }
    return 1;
}
