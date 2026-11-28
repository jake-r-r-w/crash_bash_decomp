/* Hero packs (heropack.h; docs/HERO_PACKS.md): a hero's hero.json, and its hero.glb (cgltf reads it), into plain C for the game to make
 * its models from (game/main/packheroes.c). Nothing here is the game's state: what a pack gives the game's play is whole numbers in its
 * hero.json, and its mesh only changes how it's drawn. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"
#include "heropack.h"

static void say(char *problem, size_t size, const char *what) {
    if (problem != NULL && size != 0) {
        SDL_strlcpy(problem, what, size);
    }
}

/* --- a little JSON: a tree of values --- */

enum { J_NULL, J_FALSE, J_TRUE, J_NUMBER, J_STRING, J_ARRAY, J_OBJECT };

typedef struct JVal JVal;
struct JVal {
    int type;
    double number;
    char *string;   /* a string's, or an object member's key */
    JVal *first;    /* an array's or object's first */
    JVal *next;     /* the next in the array or object it's in */
    char *key;
};

typedef struct {
    const char *at, *end;
    int depth;
} JText;

static void jfree(JVal *v) {
    while (v != NULL) {
        JVal *next = v->next;

        jfree(v->first);
        free(v->string);
        free(v->key);
        free(v);
        v = next;
    }
}

static void jspace(JText *t) {
    while (t->at < t->end && (*t->at == ' ' || *t->at == '\t' || *t->at == '\r' || *t->at == '\n')) {
        t->at++;
    }
}

static char *jstring(JText *t) {
    size_t cap = 16, n = 0;
    char *s;

    if (t->at >= t->end || *t->at != '"' || (s = malloc(cap)) == NULL) {
        return NULL;
    }
    for (t->at++; t->at < t->end && *t->at != '"'; t->at++) {
        char c = *t->at;

        if (c == '\\' && t->at + 1 < t->end) {
            t->at++;
            c = *t->at == 'n' ? '\n' : *t->at == 't' ? '\t' : *t->at == 'u' ? '?' : *t->at;
            if (*t->at == 'u') {
                t->at = t->at + 4 < t->end ? t->at + 4 : t->end - 1;
            }
        }
        if (n + 2 > cap) {
            char *more = realloc(s, cap *= 2);

            if (more == NULL) {
                free(s);
                return NULL;
            }
            s = more;
        }
        s[n++] = c;
    }
    s[n] = 0;
    if (t->at >= t->end) {
        free(s);
        return NULL;
    }
    t->at++;
    return s;
}

static JVal *jvalue(JText *t) {
    JVal *v;

    jspace(t);
    if (t->at >= t->end || t->depth > 32 || (v = calloc(1, sizeof(JVal))) == NULL) {
        return NULL;
    }
    if (*t->at == '{' || *t->at == '[') {
        int object = *t->at == '{';
        JVal **tail = &v->first;

        v->type = object ? J_OBJECT : J_ARRAY;
        t->at++;
        t->depth++;
        jspace(t);
        if (t->at < t->end && *t->at == (object ? '}' : ']')) {
            t->at++;
            t->depth--;
            return v;
        }
        for (;;) {
            char *key = NULL;
            JVal *item;

            jspace(t);
            if (object) {
                if ((key = jstring(t)) == NULL) {
                    break;
                }
                jspace(t);
                if (t->at >= t->end || *t->at++ != ':') {
                    free(key);
                    break;
                }
            }
            if ((item = jvalue(t)) == NULL) {
                free(key);
                break;
            }
            item->key = key;
            *tail = item;
            tail = &item->next;
            jspace(t);
            if (t->at < t->end && *t->at == ',') {
                t->at++;
                continue;
            }
            if (t->at < t->end && *t->at == (object ? '}' : ']')) {
                t->at++;
                t->depth--;
                return v;
            }
            break;
        }
        jfree(v);
        return NULL;
    }
    if (*t->at == '"') {
        v->type = J_STRING;
        if ((v->string = jstring(t)) == NULL) {
            free(v);
            return NULL;
        }
        return v;
    }
    if (t->end - t->at >= 4 && memcmp(t->at, "true", 4) == 0) {
        v->type = J_TRUE;
        t->at += 4;
        return v;
    }
    if (t->end - t->at >= 5 && memcmp(t->at, "false", 5) == 0) {
        v->type = J_FALSE;
        t->at += 5;
        return v;
    }
    if (t->end - t->at >= 4 && memcmp(t->at, "null", 4) == 0) {
        v->type = J_NULL;
        t->at += 4;
        return v;
    }
    {
        char num[64];
        size_t n = 0;

        while (t->at < t->end && n + 1 < sizeof(num) && strchr("+-0123456789.eE", *t->at) != NULL) {
            num[n++] = *t->at++;
        }
        num[n] = 0;
        if (n == 0) {
            free(v);
            return NULL;
        }
        v->type = J_NUMBER;
        v->number = SDL_strtod(num, NULL);
    }
    return v;
}

static const JVal *jget(const JVal *object, const char *key) {
    const JVal *v;

    if (object == NULL || object->type != J_OBJECT) {
        return NULL;
    }
    for (v = object->first; v != NULL; v = v->next) {
        if (v->key != NULL && SDL_strcasecmp(v->key, key) == 0) {
            return v;
        }
    }
    return NULL;
}

static const char *jstr(const JVal *v) {
    return v != NULL && v->type == J_STRING ? v->string : NULL;
}

static int jint(const JVal *v, int otherwise) {
    return v != NULL && v->type == J_NUMBER ? (int)floor(v->number + 0.5) : otherwise;
}

static const JVal *jat(const JVal *array, int i) {
    const JVal *v;

    if (array == NULL || array->type != J_ARRAY) {
        return NULL;
    }
    for (v = array->first; v != NULL && i > 0; v = v->next, i--) {
    }
    return v;
}

/* --- hero.json --- */

const char *const PackVoiceJobs[PACK_VOICE_JOBS] = { "name", "laugh", "yelp", "jump", "land", "thud", "throw", "fire" };
static const char *const KindNames[PACK_KINDS] = { "foot", "polar", "pogo", "ball", "tank", "race", "splash", "mallet", "dino", "kegs", "warp" };
static const char *const HeroNames[] = { "", "crash", "coco", "cortex", "brio", "tiny", "kong", "dingodile", "rilla" };

static const char *const EaseNames[PACK_EASES] = { "smooth", "linear", "slow-start", "slow-finish", "sudden", "overshoot", "bounce",
                                                   "wobble" };

const char *pack_ease_name(int ease) {
    return ease >= 0 && ease < PACK_EASES ? EaseNames[ease] : EaseNames[0];
}

int pack_ease_named(const char *name) {
    int e;

    for (e = 0; name != NULL && e < PACK_EASES; e++) {
        if (SDL_strcasecmp(name, EaseNames[e]) == 0) {
            return e;
        }
    }
    return -1;
}

const char *pack_kind_name(int kind) {
    return kind >= 0 && kind < PACK_KINDS ? KindNames[kind] : "";
}

int pack_kind_named(const char *name) {
    int k;

    for (k = 0; name != NULL && k < PACK_KINDS; k++) {
        if (SDL_strcasecmp(name, KindNames[k]) == 0) {
            return k;
        }
    }
    return -1;
}

/* An original hero by name ("crash" ... "rilla"), or number; 0 for none */
static int hero_number(const JVal *v) {
    const char *s = jstr(v);
    int h;

    if (s == NULL) {
        h = jint(v, 0);
        return h >= 0 && h <= 8 ? h : 0;
    }
    for (h = 1; h <= 8; h++) {
        if (SDL_strcasecmp(s, HeroNames[h]) == 0) {
            return h;
        }
    }
    return 0;
}

static void copy_name(char *out, size_t size, const char *s, int capitals) {
    size_t i;

    SDL_strlcpy(out, s != NULL ? s : "", size);
    for (i = 0; capitals && out[i] != 0; i++) {
        out[i] = (char)SDL_toupper((unsigned char)out[i]);
    }
}

/* A list of a pack's moves adjusted, its "adjust" (hero.json's, or a file of its own: pack_adjusts_read): how many of most, into out */
static int read_adjusts(const JVal *list, PackAdjust *out, int most) {
    int n = 0;

    /* each: [{"kind", "move", "bone", "at" (or none: every frame), "until" (held to that frame), "spread", "ease", "blend",
       "turn": [x, y, z], "reach": [x, y, z], "scale": [x, y, z]}] */
    {
        const JVal *a;
        int i;

        for (i = 0; (a = jat(list, i)) != NULL && n < most; i++) {
            PackAdjust *d = &out[n];
            const JVal *turn = jget(a, "turn"), *reach = jget(a, "reach"), *scale = jget(a, "scale");
            const char *move = jstr(jget(a, "move")), *bone = jstr(jget(a, "bone"));
            int j;

            memset(d, 0, sizeof(*d));
            d->kind = pack_kind_named(jstr(jget(a, "kind")));
            if (d->kind < 0 || move == NULL || bone == NULL) {
                continue;
            }
            SDL_strlcpy(d->move, move, sizeof(d->move));
            SDL_strlcpy(d->bone, bone, sizeof(d->bone));
            d->at = jint(jget(a, "at"), -1);
            d->at = d->at < -1 ? -1 : d->at;
            d->until = jint(jget(a, "until"), d->at);
            d->until = d->until < d->at ? d->at : d->until > 100000 ? 100000 : d->until;
            d->spread = jint(jget(a, "spread"), 0);
            d->spread = d->spread < 0 ? 0 : d->spread > 200 ? 200 : d->spread;
            d->ease = pack_ease_named(jstr(jget(a, "ease")));
            d->ease = d->ease < 0 ? PACK_EASE_SMOOTH : d->ease;
            d->blend = jget(a, "blend") != NULL && jget(a, "blend")->type == J_TRUE;
            for (j = 0; j < 3; j++) {
                const JVal *t = jat(turn, j), *r = jat(reach, j);

                d->turn[j] = t != NULL && t->type == J_NUMBER ? (float)t->number : 0;
                d->reach[j] = r != NULL && r->type == J_NUMBER ? (float)r->number : 0;
                d->turn[j] = d->turn[j] < -360 ? -360 : d->turn[j] > 360 ? 360 : d->turn[j];
                d->reach[j] = d->reach[j] < -1000 ? -1000 : d->reach[j] > 1000 ? 1000 : d->reach[j];
                {
                    const JVal *g = jat(scale, j);
                    float s = g != NULL && g->type == J_NUMBER ? (float)g->number : 1;

                    s = s < 0 ? 0 : s > 10 ? 10 : s; /* (0: hidden, all of it at its joint) */
                    d->grow[j] = s - 1;
                }
            }
            n++;
        }
    }
    return n;
}

int pack_adjusts_read(const char *text, size_t size, PackAdjust *out, int most, char *problem, size_t problem_size) {
    JText t;
    JVal *root;
    int n;

    t.at = text;
    t.end = text + size;
    t.depth = 0;
    if (size >= 3 && (unsigned char)text[0] == 0xEF && (unsigned char)text[1] == 0xBB && (unsigned char)text[2] == 0xBF) {
        t.at += 3;
    }
    root = jvalue(&t);
    if (root == NULL || root->type != J_OBJECT || jget(root, "adjust") == NULL) {
        jfree(root);
        say(problem, problem_size, "IT ISN'T JSON WITH AN ADJUST LIST");
        return -1;
    }
    n = read_adjusts(jget(root, "adjust"), out, most);
    jfree(root);
    return n;
}

int pack_info_read(const char *text, size_t size, PackInfo *out, char *problem, size_t problem_size) {
    JText t;
    JVal *root;
    const JVal *kinds, *voices, *v;
    int k;

    memset(out, 0, sizeof(*out));
    t.at = text;
    t.end = text + size;
    t.depth = 0;
    if (size >= 3 && (unsigned char)text[0] == 0xEF && (unsigned char)text[1] == 0xBB && (unsigned char)text[2] == 0xBF) {
        t.at += 3;
    }
    root = jvalue(&t);
    if (root == NULL || root->type != J_OBJECT) {
        jfree(root);
        say(problem, problem_size, "ITS HERO.JSON ISN'T JSON");
        return 0;
    }
    out->format = jint(jget(root, "format"), 1);
    if (out->format != 1) {
        jfree(root);
        say(problem, problem_size, "ITS HERO.JSON IS FOR A NEWER GAME");
        return 0;
    }
    copy_name(out->id, sizeof(out->id), jstr(jget(root, "id")), 0);
    copy_name(out->name, sizeof(out->name), jstr(jget(root, "name")), 1);
    if (out->name[0] == 0) {
        copy_name(out->name, sizeof(out->name), out->id, 1);
    }
    out->prefer = hero_number(jget(root, "prefer"));
    if (out->prefer == 0) {
        out->prefer = 1;
    }
    out->template_hero = hero_number(jget(root, "template"));
    out->template_said = out->template_hero != 0;
    if (out->template_hero == 0) {
        out->template_hero = 1;
    }
    out->height = jint(jget(root, "height"), 600);
    out->height = out->height < 200 ? 200 : out->height > 900 ? 900 : out->height;
    copy_name(out->face, sizeof(out->face), jstr(jget(root, "face")), 0);
    if (out->face[0] == 0) {
        SDL_strlcpy(out->face, "BREATHE", sizeof(out->face));
    }
    v = jget(root, "face_turn");
    out->face_turn = v != NULL && v->type == J_NUMBER ? (float)v->number : 0;
    v = jget(root, "face_head");
    out->face_head = v != NULL && v->type == J_NUMBER && v->number > 0.05 && v->number <= 1 ? (float)v->number : 0;
    for (k = 0; k < PACK_KINDS; k++) {
        out->kinds[k].keep_lift = 0x1000;
        out->kinds[k].subframes = 1;
    }
    kinds = jget(root, "kinds");
    for (k = 0; k < PACK_KINDS; k++) {
        const JVal *kind = jget(kinds, KindNames[k]), *stands = jget(kind, "stands"), *s;
        PackKind *pk = &out->kinds[k];
        int i;

        if (kind == NULL) {
            continue;
        }
        pk->keep_lift = jint(jget(kind, "keep_lift"), 0x1000);
        pk->keep_lift = pk->keep_lift < 0 ? 0 : pk->keep_lift > 0x1000 ? 0x1000 : pk->keep_lift;
        pk->like = hero_number(jget(kind, "like"));
        pk->template_hero = hero_number(jget(kind, "template"));
        if (jstr(jget(kind, "template")) != NULL && SDL_strcasecmp(jstr(jget(kind, "template")), "own") == 0) {
            pk->template_hero = -1;
        }
        pk->no_fit = jget(kind, "fit") != NULL && jget(kind, "fit")->type == J_FALSE;
        pk->subframes = jint(jget(kind, "subframes"), 1);
        pk->subframes = pk->subframes < 1 ? 1 : pk->subframes > 4 ? 4 : pk->subframes;
        copy_name(pk->otherwise, sizeof(pk->otherwise), jstr(jget(kind, "otherwise")), 0);
        for (i = 0; (s = jat(stands, i)) != NULL && pk->nstands < PACK_MAX_STANDS; i++) {
            const char *from = jstr(jat(s, 0)), *to = jstr(jat(s, 1)), *pace = jstr(jat(s, 2));
            PackStand *st = &pk->stands[pk->nstands];

            if (from == NULL || to == NULL) {
                continue;
            }
            SDL_strlcpy(st->from, from, sizeof(st->from));
            SDL_strlcpy(st->to, to, sizeof(st->to));
            st->pace = pace == NULL ? 0 : SDL_strcasecmp(pace, "loop") == 0 ? 1 : SDL_strcasecmp(pace, "once") == 0 ? 2 : 0;
            pk->nstands++;
        }
    }
    out->nadjust = read_adjusts(jget(root, "adjust"), out->adjust, PACK_MAX_ADJUSTS);
    /* every offset not said, but those it says */
    for (k = 0; k < PACK_SOUNDS; k++) {
        out->voices[k] = PACK_VOICE_UNSAID;
    }
    voices = jget(root, "voices");
    for (v = voices != NULL && voices->type == J_OBJECT ? voices->first : NULL; v != NULL; v = v->next) {
        int offset = (int)SDL_strtol(v->key, NULL, 10);
        const char *what = jstr(v);

        if (offset < 0 || offset >= PACK_SOUNDS || what == NULL) {
            continue;
        }
        out->voices[offset] = SDL_strcasecmp(what, "none") == 0 ? PACK_VOICE_NONE : PACK_VOICE_BASE;
        for (int j = 0; j < PACK_VOICE_JOBS; j++) {
            if (SDL_strcasecmp(what, PackVoiceJobs[j]) == 0) {
                out->voices[offset] = PACK_VOICE_JOB + j;
            }
        }
    }
    jfree(root);
    if (out->id[0] == 0) {
        say(problem, problem_size, "ITS HERO.JSON HAS NO ID");
        return 0;
    }
    return 1;
}

/* --- matrices: 4x4, column-major --- */

void pack_mat_mul(const float a[16], const float b[16], float out[16]) {
    float r[16];
    int i, j, k;

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            float s = 0;

            for (k = 0; k < 4; k++) {
                s += a[k * 4 + i] * b[j * 4 + k];
            }
            r[j * 4 + i] = s;
        }
    }
    memcpy(out, r, sizeof(r));
}

void pack_mat_point(const float m[16], const float p[3], float out[3]) {
    float r[3];
    int i;

    for (i = 0; i < 3; i++) {
        r[i] = m[i] * p[0] + m[4 + i] * p[1] + m[8 + i] * p[2] + m[12 + i];
    }
    memcpy(out, r, sizeof(r));
}

int pack_mat_invert(const float m[16], float out[16]) {
    float inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    if (fabsf(det) < 1e-12f) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        out[i] = inv[i] / det;
    }
    return 1;
}

void pack_trs_matrix(const PackTrs *p, float out[16]) {
    float x = p->r[0], y = p->r[1], z = p->r[2], w = p->r[3];

    out[0] = (1 - 2 * (y * y + z * z)) * p->s[0];
    out[1] = (2 * (x * y + z * w)) * p->s[0];
    out[2] = (2 * (x * z - y * w)) * p->s[0];
    out[3] = 0;
    out[4] = (2 * (x * y - z * w)) * p->s[1];
    out[5] = (1 - 2 * (x * x + z * z)) * p->s[1];
    out[6] = (2 * (y * z + x * w)) * p->s[1];
    out[7] = 0;
    out[8] = (2 * (x * z + y * w)) * p->s[2];
    out[9] = (2 * (y * z - x * w)) * p->s[2];
    out[10] = (1 - 2 * (x * x + y * y)) * p->s[2];
    out[11] = 0;
    out[12] = p->t[0];
    out[13] = p->t[1];
    out[14] = p->t[2];
    out[15] = 1;
}

/* A node's matrix as translation, rotation and scale (no shear) */
static void matrix_trs(const float m[16], PackTrs *p) {
    float r[9], tr;
    int i;

    p->t[0] = m[12];
    p->t[1] = m[13];
    p->t[2] = m[14];
    for (i = 0; i < 3; i++) {
        p->s[i] = sqrtf(m[i * 4] * m[i * 4] + m[i * 4 + 1] * m[i * 4 + 1] + m[i * 4 + 2] * m[i * 4 + 2]);
        if (p->s[i] < 1e-8f) {
            p->s[i] = 1e-8f;
        }
        r[i * 3] = m[i * 4] / p->s[i];
        r[i * 3 + 1] = m[i * 4 + 1] / p->s[i];
        r[i * 3 + 2] = m[i * 4 + 2] / p->s[i];
    }
    /* r is column-major: r[c * 3 + row] */
    tr = r[0] + r[4] + r[8];
    if (tr > 0) {
        float s = sqrtf(tr + 1) * 2;

        p->r[3] = s / 4;
        p->r[0] = (r[5] - r[7]) / s;
        p->r[1] = (r[6] - r[2]) / s;
        p->r[2] = (r[1] - r[3]) / s;
    } else if (r[0] > r[4] && r[0] > r[8]) {
        float s = sqrtf(1 + r[0] - r[4] - r[8]) * 2;

        p->r[3] = (r[5] - r[7]) / s;
        p->r[0] = s / 4;
        p->r[1] = (r[3] + r[1]) / s;
        p->r[2] = (r[6] + r[2]) / s;
    } else if (r[4] > r[8]) {
        float s = sqrtf(1 + r[4] - r[0] - r[8]) * 2;

        p->r[3] = (r[6] - r[2]) / s;
        p->r[0] = (r[3] + r[1]) / s;
        p->r[1] = s / 4;
        p->r[2] = (r[7] + r[5]) / s;
    } else {
        float s = sqrtf(1 + r[8] - r[0] - r[4]) * 2;

        p->r[3] = (r[1] - r[3]) / s;
        p->r[0] = (r[6] + r[2]) / s;
        p->r[1] = (r[7] + r[5]) / s;
        p->r[2] = s / 4;
    }
}

/* --- hero.glb --- */

static void *grow(void *p, size_t bytes) {
    void *q = realloc(p, bytes);

    if (q == NULL) {
        free(p);
    }
    return q;
}

static int read_image(const cgltf_image *img, PackImage *out) {
    const cgltf_buffer_view *bv = img->buffer_view;
    SDL_Surface *loaded, *rgba;
    SDL_IOStream *io;

    memset(out, 0, sizeof(*out));
    if (bv == NULL || bv->buffer == NULL || bv->buffer->data == NULL) {
        return 0;
    }
    io = SDL_IOFromConstMem((const uint8_t *)bv->buffer->data + bv->offset, bv->size);
    loaded = io != NULL ? SDL_LoadPNG_IO(io, true) : NULL;
    if (loaded == NULL) {
        return 0;
    }
    rgba = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(loaded);
    if (rgba == NULL) {
        return 0;
    }
    out->w = rgba->w;
    out->h = rgba->h;
    out->rgba = malloc((size_t)out->w * out->h * 4);
    if (out->rgba != NULL) {
        for (int y = 0; y < out->h; y++) {
            memcpy(out->rgba + (size_t)y * out->w * 4, (const uint8_t *)rgba->pixels + (size_t)y * rgba->pitch, (size_t)out->w * 4);
        }
    }
    SDL_DestroySurface(rgba);
    return out->rgba != NULL;
}

static int add_primitive(PackModel *m, const cgltf_data *d, const cgltf_node *node, const cgltf_primitive *prim) {
    const cgltf_accessor *pos = NULL, *uv = NULL, *col = NULL, *joints = NULL, *weights = NULL;
    const cgltf_skin *skin = node->skin;
    int base = m->nvert, n, nidx, i, j, image = -1, blend = 0, unlit = 0;
    float factor[4] = { 1, 1, 1, 1 }, node_world[16];
    size_t a;

    if (prim->type != cgltf_primitive_type_triangles) {
        return 1;
    }
    for (a = 0; a < prim->attributes_count; a++) {
        const cgltf_attribute *at = &prim->attributes[a];

        if (at->type == cgltf_attribute_type_position) {
            pos = at->data;
        } else if (at->type == cgltf_attribute_type_texcoord && at->index == 0) {
            uv = at->data;
        } else if (at->type == cgltf_attribute_type_color && at->index == 0) {
            col = at->data;
        } else if (at->type == cgltf_attribute_type_joints && at->index == 0) {
            joints = at->data;
        } else if (at->type == cgltf_attribute_type_weights && at->index == 0) {
            weights = at->data;
        }
    }
    if (pos == NULL) {
        return 1;
    }
    if (prim->material != NULL) {
        const cgltf_material *mat = prim->material;

        /* A model in the older specular-glossiness form (Sketchfab's exports, among others) keeps
           its colour and picture in the extension's diffuse; the metallic-roughness part, when it
           has one too, is only the fallback for readers without the extension. */
        if (mat->has_pbr_specular_glossiness || mat->has_pbr_metallic_roughness) {
            const cgltf_texture *tex = mat->has_pbr_specular_glossiness
                ? mat->pbr_specular_glossiness.diffuse_texture.texture
                : mat->pbr_metallic_roughness.base_color_texture.texture;

            memcpy(factor, mat->has_pbr_specular_glossiness ? mat->pbr_specular_glossiness.diffuse_factor
                                                            : mat->pbr_metallic_roughness.base_color_factor,
                   sizeof(factor));
            if (tex != NULL && tex->image != NULL) {
                image = (int)(tex->image - d->images);
                image = image < m->nimages ? image : -1;
            }
        }
        blend = mat->alpha_mode == cgltf_alpha_mode_blend;
        unlit = mat->unlit;
    }
    n = (int)pos->count;
    m->pos = grow(m->pos, sizeof(float) * 3 * (size_t)(base + n));
    m->uv = grow(m->uv, sizeof(float) * 2 * (size_t)(base + n));
    m->rgba = grow(m->rgba, sizeof(float) * 4 * (size_t)(base + n));
    m->joint = grow(m->joint, 4 * (size_t)(base + n));
    m->weight = grow(m->weight, sizeof(float) * 4 * (size_t)(base + n));
    if (m->pos == NULL || m->uv == NULL || m->rgba == NULL || m->joint == NULL || m->weight == NULL) {
        return 0;
    }
    cgltf_node_transform_world(node, node_world);
    for (i = 0; i < n; i++) {
        float p[3], *out = &m->pos[(base + i) * 3], c[4] = { 1, 1, 1, 1 };
        uint8_t *jo = &m->joint[(base + i) * 4];
        float *we = &m->weight[(base + i) * 4];

        cgltf_accessor_read_float(pos, (cgltf_size)i, p, 3);
        m->uv[(base + i) * 2] = m->uv[(base + i) * 2 + 1] = 0;
        if (uv != NULL) {
            cgltf_accessor_read_float(uv, (cgltf_size)i, &m->uv[(base + i) * 2], 2);
        }
        if (col != NULL) {
            cgltf_accessor_read_float(col, (cgltf_size)i, c, col->type == cgltf_type_vec4 ? 4 : 3);
        }
        for (j = 0; j < 4; j++) {
            m->rgba[(base + i) * 4 + j] = c[j] * factor[j];
            jo[j] = 0;
            we[j] = 0;
        }
        if (skin != NULL && joints != NULL && weights != NULL) {
            cgltf_uint ji[4] = { 0, 0, 0, 0 };
            float wi[4] = { 0, 0, 0, 0 }, sum = 0, rest[3] = { 0, 0, 0 };

            cgltf_accessor_read_uint(joints, (cgltf_size)i, ji, 4);
            cgltf_accessor_read_float(weights, (cgltf_size)i, wi, 4);
            for (j = 0; j < 4; j++) {
                float bind[16], to_rest[16], q[3];
                int bone;

                if (wi[j] <= 0 || ji[j] >= skin->joints_count) {
                    continue;
                }
                bone = (int)(skin->joints[ji[j]] - d->nodes);
                /* where the skin's bind puts it, as the bones rest */
                if (skin->inverse_bind_matrices != NULL) {
                    cgltf_accessor_read_float(skin->inverse_bind_matrices, ji[j], bind, 16);
                } else {
                    pack_mat_invert(m->bones[bone].rest_world, bind);
                }
                pack_mat_mul(m->bones[bone].rest_world, bind, to_rest);
                pack_mat_point(to_rest, p, q);
                rest[0] += q[0] * wi[j];
                rest[1] += q[1] * wi[j];
                rest[2] += q[2] * wi[j];
                jo[j] = (uint8_t)bone;
                we[j] = wi[j];
                sum += wi[j];
            }
            if (sum > 0) {
                for (j = 0; j < 4; j++) {
                    we[j] /= sum;
                }
                out[0] = rest[0] / sum;
                out[1] = rest[1] / sum;
                out[2] = rest[2] / sum;
                continue;
            }
        }
        /* not skinned: it goes with its node */
        pack_mat_point(node_world, p, out);
        jo[0] = (uint8_t)(node - d->nodes);
        we[0] = 1;
    }
    nidx = prim->indices != NULL ? (int)prim->indices->count : n;
    nidx -= nidx % 3;
    m->tri = grow(m->tri, sizeof(int) * (size_t)(m->ntri * 3 + nidx));
    m->tri_image = grow(m->tri_image, sizeof(int) * (size_t)(m->ntri + nidx / 3));
    m->tri_blend = grow(m->tri_blend, (size_t)(m->ntri + nidx / 3));
    m->tri_unlit = grow(m->tri_unlit, (size_t)(m->ntri + nidx / 3));
    if (m->tri == NULL || m->tri_image == NULL || m->tri_blend == NULL || m->tri_unlit == NULL) {
        return 0;
    }
    for (i = 0; i < nidx; i += 3) {
        for (j = 0; j < 3; j++) {
            int v = prim->indices != NULL ? (int)cgltf_accessor_read_index(prim->indices, (cgltf_size)(i + j)) : i + j;

            m->tri[m->ntri * 3 + j] = base + (v < n ? v : 0);
        }
        m->tri_image[m->ntri] = image;
        m->tri_blend[m->ntri] = (uint8_t)blend;
        m->tri_unlit[m->ntri++] = (uint8_t)unlit;
    }
    m->nvert = base + n;
    return 1;
}

static int add_clip(PackModel *m, const cgltf_data *d, const cgltf_animation *an) {
    PackClip *c = &m->clips[m->nclips];
    size_t i;

    memset(c, 0, sizeof(*c));
    SDL_strlcpy(c->name, an->name != NULL ? an->name : "", sizeof(c->name));
    c->tracks = calloc(an->channels_count != 0 ? an->channels_count : 1, sizeof(PackTrack));
    if (c->tracks == NULL) {
        return 0;
    }
    for (i = 0; i < an->channels_count; i++) {
        const cgltf_animation_channel *ch = &an->channels[i];
        const cgltf_animation_sampler *s = ch->sampler;
        PackTrack *t = &c->tracks[c->ntracks];
        int width, path, k, cubic;
        float *all;

        if (ch->target_node == NULL || s == NULL || s->input == NULL || s->output == NULL) {
            continue;
        }
        path = ch->target_path == cgltf_animation_path_type_translation ? 0
             : ch->target_path == cgltf_animation_path_type_rotation    ? 1
             : ch->target_path == cgltf_animation_path_type_scale       ? 2
                                                                         : -1;
        if (path < 0) {
            continue;
        }
        width = path == 1 ? 4 : 3;
        cubic = s->interpolation == cgltf_interpolation_type_cubic_spline;
        t->bone = (int)(ch->target_node - d->nodes);
        t->path = path;
        t->step = s->interpolation == cgltf_interpolation_type_step;
        t->n = (int)s->input->count;
        t->time = malloc(sizeof(float) * (size_t)t->n);
        t->value = malloc(sizeof(float) * (size_t)t->n * width);
        all = malloc(sizeof(float) * (size_t)t->n * width * (cubic ? 3 : 1));
        if (t->time == NULL || t->value == NULL || all == NULL) {
            free(t->time);
            free(t->value);
            free(all);
            return 0;
        }
        cgltf_accessor_unpack_floats(s->input, t->time, (cgltf_size)t->n);
        cgltf_accessor_unpack_floats(s->output, all, (cgltf_size)t->n * width * (cubic ? 3 : 1));
        /* a cubic spline's keys are in-tangent, value, out-tangent: its values only, blended in a line */
        for (k = 0; k < t->n; k++) {
            memcpy(&t->value[k * width], &all[(k * (cubic ? 3 : 1) + (cubic ? 1 : 0)) * width], sizeof(float) * width);
            c->length = t->time[k] > c->length ? t->time[k] : c->length;
        }
        free(all);
        c->ntracks++;
    }
    m->nclips++;
    return 1;
}

int pack_model_read(const uint8_t *glb, size_t size, PackModel *out, char *problem, size_t problem_size) {
    cgltf_options opts;
    cgltf_data *d = NULL;
    size_t i, p;

    memset(out, 0, sizeof(*out));
    memset(&opts, 0, sizeof(opts));
    if (cgltf_parse(&opts, glb, size, &d) != cgltf_result_success || cgltf_load_buffers(&opts, d, NULL) != cgltf_result_success) {
        cgltf_free(d);
        say(problem, problem_size, "ITS HERO.GLB ISN'T GLTF");
        return 0;
    }
    if (d->nodes_count > PACK_MAX_BONES) {
        cgltf_free(d);
        say(problem, problem_size, "ITS HERO.GLB HAS TOO MANY NODES");
        return 0;
    }
    /* every node is a bone: as it rests, and where that is */
    out->nbones = (int)d->nodes_count;
    for (i = 0; i < d->nodes_count; i++) {
        const cgltf_node *n = &d->nodes[i];
        PackBone *b = &out->bones[i];

        SDL_strlcpy(b->name, n->name != NULL ? n->name : "", sizeof(b->name));
        b->parent = n->parent != NULL ? (int)(n->parent - d->nodes) : -1;
        if (n->has_matrix) {
            matrix_trs(n->matrix, &b->rest);
        } else {
            b->rest.t[0] = n->has_translation ? n->translation[0] : 0;
            b->rest.t[1] = n->has_translation ? n->translation[1] : 0;
            b->rest.t[2] = n->has_translation ? n->translation[2] : 0;
            b->rest.r[0] = n->has_rotation ? n->rotation[0] : 0;
            b->rest.r[1] = n->has_rotation ? n->rotation[1] : 0;
            b->rest.r[2] = n->has_rotation ? n->rotation[2] : 0;
            b->rest.r[3] = n->has_rotation ? n->rotation[3] : 1;
            b->rest.s[0] = n->has_scale ? n->scale[0] : 1;
            b->rest.s[1] = n->has_scale ? n->scale[1] : 1;
            b->rest.s[2] = n->has_scale ? n->scale[2] : 1;
        }
        cgltf_node_transform_world(n, b->rest_world);
        pack_mat_invert(b->rest_world, b->rest_inv);
    }
    for (i = 0; i < d->images_count && out->nimages < PACK_MAX_IMAGES; i++) {
        read_image(&d->images[i], &out->images[out->nimages++]);
    }
    for (i = 0; i < d->nodes_count; i++) {
        const cgltf_mesh *mesh = d->nodes[i].mesh;

        for (p = 0; mesh != NULL && p < mesh->primitives_count; p++) {
            if (!add_primitive(out, d, &d->nodes[i], &mesh->primitives[p])) {
                cgltf_free(d);
                pack_model_free(out);
                say(problem, problem_size, "NO MEMORY FOR ITS MESH");
                return 0;
            }
        }
    }
    for (i = 0; i < d->animations_count && out->nclips < PACK_MAX_CLIPS; i++) {
        if (!add_clip(out, d, &d->animations[i])) {
            cgltf_free(d);
            pack_model_free(out);
            say(problem, problem_size, "NO MEMORY FOR ITS CLIPS");
            return 0;
        }
    }
    cgltf_free(d);
    if (out->ntri == 0) {
        pack_model_free(out);
        say(problem, problem_size, "ITS HERO.GLB HAS NO MESH");
        return 0;
    }
    return 1;
}

void pack_model_free(PackModel *m) {
    int i, k;

    free(m->pos);
    free(m->uv);
    free(m->rgba);
    free(m->joint);
    free(m->weight);
    free(m->tri);
    free(m->tri_image);
    free(m->tri_blend);
    free(m->tri_unlit);
    for (i = 0; i < m->nclips; i++) {
        for (k = 0; k < m->clips[i].ntracks; k++) {
            free(m->clips[i].tracks[k].time);
            free(m->clips[i].tracks[k].value);
        }
        free(m->clips[i].tracks);
    }
    for (i = 0; i < m->nimages; i++) {
        free(m->images[i].rgba);
    }
    memset(m, 0, sizeof(*m));
}

int pack_clip_find(const PackModel *m, const char *name) {
    int i;

    for (i = 0; name != NULL && i < m->nclips; i++) {
        if (SDL_strcasecmp(m->clips[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* --- posing --- */

static void sample(const PackTrack *t, float time, float *out) {
    int width = t->path == 1 ? 4 : 3, k, j;
    float u;

    if (t->n == 0) {
        return;
    }
    if (time <= t->time[0] || t->n == 1) {
        memcpy(out, t->value, sizeof(float) * width);
        return;
    }
    if (time >= t->time[t->n - 1]) {
        memcpy(out, &t->value[(t->n - 1) * width], sizeof(float) * width);
        return;
    }
    for (k = 0; k + 1 < t->n && t->time[k + 1] < time; k++) {
    }
    u = t->step || t->time[k + 1] <= t->time[k] ? 0 : (time - t->time[k]) / (t->time[k + 1] - t->time[k]);
    if (t->path == 1) {
        const float *a = &t->value[k * 4], *b = &t->value[(k + 1) * 4];
        float dot = a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3], sign = dot < 0 ? -1.0f : 1.0f, len = 0;

        /* blended along the short way round, then made a unit again */
        for (j = 0; j < 4; j++) {
            out[j] = a[j] * (1 - u) + b[j] * sign * u;
            len += out[j] * out[j];
        }
        len = len > 0 ? 1 / sqrtf(len) : 1;
        for (j = 0; j < 4; j++) {
            out[j] *= len;
        }
        return;
    }
    for (j = 0; j < 3; j++) {
        out[j] = t->value[k * 3 + j] * (1 - u) + t->value[(k + 1) * 3 + j] * u;
    }
}

void pack_pose_local(const PackModel *m, int clip, float t, PackTrs *local) {
    int b, k;

    for (b = 0; b < m->nbones; b++) {
        local[b] = m->bones[b].rest;
    }
    if (clip < 0 || clip >= m->nclips) {
        return;
    }
    for (k = 0; k < m->clips[clip].ntracks; k++) {
        const PackTrack *tr = &m->clips[clip].tracks[k];
        PackTrs *l;

        if (tr->bone < 0 || tr->bone >= m->nbones) {
            continue;
        }
        l = &local[tr->bone];
        sample(tr, t, tr->path == 0 ? l->t : tr->path == 1 ? l->r : l->s);
    }
}

void pack_pose_world(const PackModel *m, const PackTrs *local, float (*world)[16]) {
    int b, done = 0, pass;
    unsigned char ready[PACK_MAX_BONES];

    memset(ready, 0, sizeof(ready));
    /* parents before their children, whatever order the nodes are in */
    for (pass = 0; pass < m->nbones && done < m->nbones; pass++) {
        for (b = 0; b < m->nbones; b++) {
            float mine[16];
            int parent = m->bones[b].parent;

            if (ready[b] || (parent >= 0 && !ready[parent])) {
                continue;
            }
            pack_trs_matrix(&local[b], mine);
            if (parent >= 0) {
                pack_mat_mul(world[parent], mine, world[b]);
            } else {
                memcpy(world[b], mine, sizeof(mine));
            }
            ready[b] = 1;
            done++;
        }
    }
}

void pack_skin(const PackModel *m, const float (*world)[16], float *out) {
    static float skin[PACK_MAX_BONES][16];
    int b, v, j;

    for (b = 0; b < m->nbones; b++) {
        pack_mat_mul(world[b], m->bones[b].rest_inv, skin[b]);
    }
    for (v = 0; v < m->nvert; v++) {
        float sum[3] = { 0, 0, 0 };

        for (j = 0; j < 4; j++) {
            float w = m->weight[v * 4 + j], q[3];

            if (w <= 0) {
                continue;
            }
            pack_mat_point(skin[m->joint[v * 4 + j]], &m->pos[v * 3], q);
            sum[0] += q[0] * w;
            sum[1] += q[1] * w;
            sum[2] += q[2] * w;
        }
        out[v * 3] = sum[0];
        out[v * 3 + 1] = sum[1];
        out[v * 3 + 2] = sum[2];
    }
}

void pack_card_face(const uint32_t *card, int cw, int ch, uint32_t *out, int stride, int fw, int fh, float inset, float rows, float oy, int stop) {
    float cols = fw * (1 - 2 * inset) / cw, ox = fw * inset;
    int px, py, i, j;

    for (py = 0; py < fh; py++) {
        for (px = 0; px < fw; px++) {
            uint32_t r = 0, g = 0, b = 0, a = 0;

            for (j = 0; j < 4; j++) {
                for (i = 0; i < 4; i++) {
                    int x = (int)floorf((px + (i + 0.5f) / 4 - ox) / cols), y = (int)floorf((py + (j + 0.5f) / 4 - oy) / rows);
                    uint32_t c = x >= 0 && y >= 0 && x < cw && y < ch && py < stop ? card[y * cw + x] : 0, ca = c >> 24;

                    r += (c & 0xFF) * ca;
                    g += (c >> 8 & 0xFF) * ca;
                    b += (c >> 16 & 0xFF) * ca;
                    a += ca;
                }
            }
            out[py * stride + px] = a == 0 ? 0 : (r / a) | (g / a) << 8 | (b / a) << 16 | (a / 16) << 24;
        }
    }
}

void pack_select_picture(uint32_t *out, int fw, int fh, int pw, int ph, const uint32_t *face) {
    int x, y, k;

    for (y = 0; y < fh; y++) {
        uint32_t bg[3];

        bg[0] = 255;
        bg[1] = (uint32_t)(224 - 104 * y / (ph > 0 ? ph : 1));
        bg[2] = (uint32_t)(72 - 56 * y / (ph > 0 ? ph : 1));
        for (x = 0; x < fw; x++) {
            uint32_t c = y < ph && x < pw && face != NULL ? face[y * fw + x] : 0, a = c >> 24, mixed = 0xFF000000u;

            for (k = 0; k < 3; k++) {
                mixed |= ((c >> (k * 8) & 0xFF) * a + bg[k] * (255 - a)) / 255 << (k * 8);
            }
            out[y * fw + x] = y < ph && x < pw ? mixed : 0;
        }
    }
}

/* across the part of the portrait the game's own use (but its inset), and down all of it */
void pack_card_select(const uint32_t *card, int cw, int ch, uint32_t *out, int w, int h) {
    int ph = h > PACK_SELECT_ROWS ? PACK_SELECT_ROWS : h, pw = w > PACK_SELECT_COLS ? PACK_SELECT_COLS : w;
    uint32_t *face = (uint32_t *)calloc((size_t)w * h, sizeof(uint32_t));

    if (face == NULL) {
        memset(out, 0, sizeof(uint32_t) * (size_t)w * h);
        return;
    }
    pack_card_face(card, cw, ch, face, w, pw, ph, PACK_SELECT_INSET, (float)ph / ch, 0, ph);
    pack_select_picture(out, w, h, pw, ph, face);
    free(face);
}

/* right across, the top of it (head) 24 texels tall as the game's own faces are, from 0.6 down, and on below that to row 26, the
   health bar's top, so it goes on behind the bar (64 x 32's; as much more for a bigger one). head: as many of its rows as are as tall
   against its width as those 24 rows show against the 64 columns (each PACK_TEXEL_TALL), so it isn't squashed (the user's word,
   2026-11-23: "Yes, change it": the whole card had been squeezed into them) */
void pack_card_panel(const uint32_t *card, int cw, int ch, uint32_t *out, int w, int h) {
    float rows = 24.0f * h / 32, head = cw * rows * PACK_TEXEL_TALL / w;

    pack_card_face(card, cw, ch, out, w, w, h, 0, rows / head, 0.6f * h / 32, 26 * h / 32);
}

uint32_t *pack_png_read(const uint8_t *png, size_t size, int *w, int *h) {
    PackImage im;
    cgltf_image fake;
    cgltf_buffer_view bv;
    cgltf_buffer buf;

    memset(&fake, 0, sizeof(fake));
    memset(&bv, 0, sizeof(bv));
    memset(&buf, 0, sizeof(buf));
    buf.data = (void *)png;
    buf.size = size;
    bv.buffer = &buf;
    bv.size = size;
    fake.buffer_view = &bv;
    if (!read_image(&fake, &im)) {
        return NULL;
    }
    *w = im.w;
    *h = im.h;
    /* RGBA bytes are red | green << 8 | blue << 16 | alpha << 24 as a little-endian word */
    return (uint32_t *)im.rgba;
}

/* --- its sounds --- */

int16_t *pack_wav_read(const uint8_t *wav, size_t size, int *frames) {
    SDL_AudioSpec spec, want = { SDL_AUDIO_S16LE, 1, PACK_SOUND_RATE };
    Uint8 *buf = NULL, *out = NULL;
    Uint32 len = 0;
    int out_len = 0;

    if (!SDL_LoadWAV_IO(SDL_IOFromConstMem(wav, size), true, &spec, &buf, &len)) {
        return NULL;
    }
    if (!SDL_ConvertAudioSamples(&spec, buf, (int)len, &want, &out, &out_len)) {
        SDL_free(buf);
        return NULL;
    }
    SDL_free(buf);
    *frames = out_len / 2;
    return (int16_t *)out;
}

void pack_samples_free(int16_t *pcm) {
    SDL_free(pcm);
}

/* PS1 ADPCM, as the game's player decodes it (platform/audio.c): 16-byte blocks of 28 samples, each block's filter and shift the
 * ones that come nearest */
static const int Filter0[5] = { 0, 60, 115, 98, 122 }, Filter1[5] = { 0, 0, -52, -55, -60 };

static int encode_block(const int16_t *x, int filter, int shift, int *s1, int *s2, uint8_t *out, long long *err) {
    int i, p1 = *s1, p2 = *s2;
    long long e = 0;

    for (i = 0; i < 28; i++) {
        int pred = (p1 * Filter0[filter] + p2 * Filter1[filter] + 32) >> 6, step = 1 << (12 - shift), d = x[i] - pred, q, dec;

        q = d >= 0 ? (d + step / 2) / step : -((-d + step / 2) / step);
        q = q < -8 ? -8 : q > 7 ? 7 : q;
        dec = (int)((uint32_t)(q & 15) << 28) >> (16 + shift);
        dec += pred;
        dec = dec < -32768 ? -32768 : dec > 32767 ? 32767 : dec;
        e += (long long)(x[i] - dec) * (x[i] - dec);
        p2 = p1;
        p1 = dec;
        if (out != NULL) {
            out[2 + i / 2] |= (uint8_t)((q & 15) << ((i & 1) * 4));
        }
    }
    if (out != NULL) {
        *s1 = p1;
        *s2 = p2;
    }
    *err = e;
    return 1;
}

uint8_t *pack_adpcm_encode(const int16_t *pcm, int frames, int *bytes) {
    int blocks = (frames + 27) / 28, b, s1 = 0, s2 = 0;
    uint8_t *out = calloc((size_t)(blocks > 0 ? blocks : 1), 16);

    if (out == NULL) {
        return NULL;
    }
    for (b = 0; b < blocks; b++) {
        int16_t x[28];
        int i, f, s, best_f = 0, best_s = 12;
        long long best = -1, e;

        for (i = 0; i < 28; i++) {
            x[i] = b * 28 + i < frames ? pcm[b * 28 + i] : 0;
        }
        for (f = 0; f < 5; f++) {
            for (s = 0; s <= 12; s++) {
                encode_block(x, f, s, &s1, &s2, NULL, &e);
                if (best < 0 || e < best) {
                    best = e;
                    best_f = f;
                    best_s = s;
                }
            }
        }
        out[b * 16] = (uint8_t)(best_s | best_f << 4);
        out[b * 16 + 1] = (uint8_t)(b == blocks - 1 ? 1 : 0); /* the last: stop after it */
        encode_block(x, best_f, best_s, &s1, &s2, &out[b * 16], &e);
    }
    *bytes = blocks * 16;
    return out;
}
