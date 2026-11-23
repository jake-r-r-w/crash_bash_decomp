/* The pack (kit.h; docs/HERO_PACKS.md): heroes/<id>/hero.glb and hero.json in a mod folder, with its mod.json. The GLB is the model's
 * mesh as it rests, its texture as laid out for the game, the standard skeleton (each bone unturned at its place) with each corner's
 * bones, and the moves, a pose each 30th of a second. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "kit.h"

typedef struct {
    uint8_t *b;
    size_t n, cap;
} Buf;

/* bytes, from a multiple of 4: where */
static size_t put(Buf *o, const void *p, size_t n) {
    size_t pad = (4 - o->n % 4) % 4, at;

    if (o->n + pad + n > o->cap) {
        o->cap = (o->n + pad + n) * 2 + 64;
        o->b = realloc(o->b, o->cap);
    }
    memset(o->b + o->n, 0, pad);
    o->n += pad;
    at = o->n;
    if (n > 0) {
        memcpy(o->b + at, p, n);
    }
    o->n += n;
    return at;
}

/* the JSON, grown as it's written */
typedef struct {
    char *s;
    size_t n, cap;
} Text;

static void say(Text *t, const char *fmt, ...) {
    va_list ap;
    int need;

    va_start(ap, fmt);
    need = SDL_vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (t->n + need + 1 > t->cap) {
        t->cap = (t->n + need + 1) * 2;
        t->s = realloc(t->s, t->cap);
    }
    va_start(ap, fmt);
    SDL_vsnprintf(t->s + t->n, need + 1, fmt, ap);
    va_end(ap);
    t->n += need;
}

/* a buffer view and an accessor of it */
static int views, accessors;
static Text Views, Accessors;

static int add(Buf *bin, const void *p, size_t bytes, int ctype, int count, const char *type, const char *minmax) {
    size_t at = put(bin, p, bytes);

    say(&Views, "%s{\"buffer\":0,\"byteOffset\":%zu,\"byteLength\":%zu}", views ? "," : "", at, bytes);
    say(&Accessors, "%s{\"bufferView\":%d,\"componentType\":%d,\"count\":%d,\"type\":\"%s\"%s}", accessors ? "," : "", views, ctype, count,
        type, minmax != NULL ? minmax : "");
    views++;
    return accessors++;
}

/* the model's own clip for a move, by its name, or foot/ and its name (any case); -1 none */
static int own_clip(const KitModel *m, const char *move) {
    int c;

    for (c = 0; m->src != NULL && m->src_of[KB_HIPS] >= 0 && c < m->src->nclips; c++) {
        const char *n = m->src->clips[c].name;

        if (SDL_strcasecmp(n, move) == 0 || (SDL_strncasecmp(n, "foot/", 5) == 0 && SDL_strcasecmp(n + 5, move) == 0)) {
            return c;
        }
    }
    return -1;
}

/* whether a name is one of the pack's moves copied from its template's (KitPack.baked) */
static int baked_named(const KitPack *p, const char *name) {
    int i;

    for (i = 0; i < p->nbaked; i++) {
        if (SDL_strcasecmp(p->baked[i].name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

static int write_file(const char *path, const void *p, size_t n) {
    SDL_IOStream *io = SDL_IOFromFile(path, "wb");
    int ok = io != NULL && SDL_WriteIO(io, p, n) == n;

    if (io != NULL) {
        SDL_CloseIO(io);
    }
    return ok;
}

int kit_write(const KitModel *m, const KitPack *p, const char *out, char *why, size_t why_size) {
    Buf bin = { 0 };
    Text json = { 0 }, anims = { 0 };
    int slot[KB_COUNT], nb = 0, b, i, k, mv;
    int pos_a, uv_a, col_a, joint_a, weight_a, idx_a, ibm_a, image_view;
    char path[1024], mm[256];
    float (*rgba)[4] = malloc(sizeof(float) * 4 * m->nvert);
    uint16_t *joints = malloc(sizeof(uint16_t) * 4 * m->nvert), *idx = malloc(sizeof(uint16_t) * 3 * m->ntri);
    float *ibm = malloc(sizeof(float) * 16 * KB_COUNT);
    uint8_t head[12], th[8], dh[8];
    SDL_IOStream *io;

    views = accessors = 0;
    memset(&Views, 0, sizeof(Views));
    memset(&Accessors, 0, sizeof(Accessors));
    for (b = 0; b < KB_COUNT; b++) {
        slot[b] = m->bones[b].have ? nb++ : -1;
    }
    /* the mesh */
    snprintf(mm, sizeof(mm), ",\"min\":[%g,%g,%g],\"max\":[%g,%g,%g]", m->lo[0], m->lo[1], m->lo[2], m->hi[0], m->hi[1], m->hi[2]);
    pos_a = add(&bin, m->pos, sizeof(float) * 3 * m->nvert, 5126, m->nvert, "VEC3", mm);
    uv_a = add(&bin, m->uv, sizeof(float) * 2 * m->nvert, 5126, m->nvert, "VEC2", NULL);
    for (i = 0; i < m->nvert; i++) {
        for (k = 0; k < 4; k++) {
            rgba[i][k] = m->rgba[i][k];
            joints[i * 4 + k] = (uint16_t)(m->weight[i][k] > 0 && slot[m->joint[i][k]] >= 0 ? slot[m->joint[i][k]] : 0);
        }
    }
    col_a = add(&bin, rgba, sizeof(float) * 4 * m->nvert, 5126, m->nvert, "VEC4", NULL);
    joint_a = add(&bin, joints, sizeof(uint16_t) * 4 * m->nvert, 5123, m->nvert, "VEC4", NULL);
    weight_a = add(&bin, m->weight, sizeof(float) * 4 * m->nvert, 5126, m->nvert, "VEC4", NULL);
    for (i = 0; i < m->ntri; i++) {
        for (k = 0; k < 3; k++) {
            idx[i * 3 + k] = (uint16_t)m->tri[i][k];
        }
    }
    idx_a = add(&bin, idx, sizeof(uint16_t) * 3 * m->ntri, 5123, m->ntri * 3, "SCALAR", NULL);
    image_view = views;
    say(&Views, ",{\"buffer\":0,\"byteOffset\":%zu,\"byteLength\":%zu}", put(&bin, m->png, m->png_size), m->png_size);
    views++;
    /* each bone rests unturned, so its inverse bind is a move back from where it is */
    for (b = 0, k = 0; b < KB_COUNT; b++) {
        if (slot[b] < 0) {
            continue;
        }
        float mat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -m->bones[b].at[0], -m->bones[b].at[1], -m->bones[b].at[2], 1 };

        memcpy(&ibm[k++ * 16], mat, sizeof(mat));
    }
    ibm_a = add(&bin, ibm, sizeof(float) * 16 * nb, 5126, nb, "MAT4", NULL);
    /* the moves: the standard ones, each the model's own clip of that name if it has one (a rigged model's), then its other clips */
    int nmoves = 0, own = 0, nown = 0;
    char ownname[KIT_OWN_MOST][48];
    int ownstd[KIT_OWN_MOST];

    /* its own moves in the games' places: KIND/MOVE=OURS, OURS one of the standard moves */
    for (b = 0; b < KIT_OWN_MOST && p->own[b] != NULL; b++) {
        const char *eq = strchr(p->own[b], '=');
        int s;

        for (s = 0; eq != NULL && s < KitMoveCount && SDL_strcasecmp(eq + 1, KitMoves[s].name) != 0; s++) {
        }
        if (eq == NULL || s >= KitMoveCount || (size_t)(eq - p->own[b]) >= sizeof(ownname[0])) {
            snprintf(why, why_size, "%s isn't GAME/MOVE=ONE OF ITS MOVES", p->own[b]);
            return 0;
        }
        memcpy(ownname[nown], p->own[b], (size_t)(eq - p->own[b]));
        ownname[nown][eq - p->own[b]] = 0;
        ownstd[nown++] = s;
    }
    /* a move's name that's one of its moves copied from its template's (those are written last, in place of any other of that name) */
#define BAKED(name) baked_named(p, name)
    for (mv = 0; mv < KitMoveCount + (m->src != NULL ? m->src->nclips : 0) + nown + p->nbaked && nmoves < 64; mv++) {
        const char *mname;
        float mlen;
        int clip = -1, f, time_a, channels = 0, frames, std = mv < KitMoveCount ? mv : 0;
        const KitBaked *bk = NULL;

        if (mv >= KitMoveCount + (m->src != NULL ? m->src->nclips : 0) + nown) {
            bk = &p->baked[mv - KitMoveCount - (m->src != NULL ? m->src->nclips : 0) - nown];
            mname = bk->name;
            mlen = bk->length;
        } else if (mv >= KitMoveCount + (m->src != NULL ? m->src->nclips : 0)) {
            int o = mv - KitMoveCount - (m->src != NULL ? m->src->nclips : 0);

            std = ownstd[o];
            mname = ownname[o];
            if (BAKED(mname)) {
                continue;
            }
            mlen = KitMoves[std].length;
            clip = own_clip(m, KitMoves[std].name);
            if (clip >= 0) {
                mlen = m->src->clips[clip].length;
            }
        } else if (mv < KitMoveCount) {
            mname = KitMoves[mv].name;
            mlen = KitMoves[mv].length;
            clip = own_clip(m, mname);
            if (clip >= 0) {
                mlen = m->src->clips[clip].length;
            }
        } else {
            int c = mv - KitMoveCount, k;

            mname = m->src->clips[c].name;
            mlen = m->src->clips[c].length;
            for (k = 0; k < KitMoveCount && own_clip(m, KitMoves[k].name) != c; k++) {
            }
            if (k < KitMoveCount || m->src_of[KB_HIPS] < 0 || mlen <= 0 || mname[0] == 0) {
                continue;
            }
            for (k = 0; k < KIT_DROP_MOST && p->drop[k] != NULL && SDL_strcasecmp(p->drop[k], mname) != 0; k++) {
            }
            if ((k < KIT_DROP_MOST && p->drop[k] != NULL) || BAKED(mname)) {
                continue;
            }
            clip = c;
        }
        own += clip >= 0 || bk != NULL;
        frames = bk != NULL ? bk->frames : (int)(mlen * KIT_FPS + 0.5f) + 1;
        frames = frames < 2 ? 2 : frames;
        float *times = malloc(sizeof(float) * frames), (*rot)[KB_COUNT][4] = malloc(sizeof(*rot) * frames), (*hips)[3] = malloc(sizeof(*hips) * frames);
        Text ch = { 0 }, sm = { 0 };
        char tm[96];

        for (f = 0; f < frames; f++) {
            times[f] = mlen * f / (frames - 1);
            if (bk != NULL) {
                memcpy(rot[f], bk->rot[f < bk->frames ? f : bk->frames - 1], sizeof(rot[f]));
                memcpy(hips[f], bk->hips[f < bk->frames ? f : bk->frames - 1], sizeof(hips[f]));
            } else if (clip < 0 || !kit_clip_pose(m, clip, times[f], rot[f], hips[f])) {
                kit_pose(m, std, times[f], rot[f], hips[f]);
            }
        }
        snprintf(tm, sizeof(tm), ",\"min\":[0],\"max\":[%g]", mlen);
        time_a = add(&bin, times, sizeof(float) * frames, 5126, frames, "SCALAR", tm);
        for (b = 0; b < KB_COUNT; b++) {
            float *v = malloc(sizeof(float) * 4 * frames);
            int o;

            if (slot[b] < 0) {
                free(v);
                continue;
            }
            for (f = 0; f < frames; f++) {
                memcpy(&v[f * 4], rot[f][b], sizeof(float) * 4);
            }
            o = add(&bin, v, sizeof(float) * 4 * frames, 5126, frames, "VEC4", NULL);
            say(&sm, "%s{\"input\":%d,\"output\":%d,\"interpolation\":\"LINEAR\"}", channels ? "," : "", time_a, o);
            say(&ch, "%s{\"sampler\":%d,\"target\":{\"node\":%d,\"path\":\"rotation\"}}", channels ? "," : "", channels, slot[b]);
            channels++;
            if (b == KB_HIPS) {
                for (f = 0; f < frames; f++) {
                    v[f * 3] = m->bones[b].at[0] + hips[f][0];
                    v[f * 3 + 1] = m->bones[b].at[1] + hips[f][1];
                    v[f * 3 + 2] = m->bones[b].at[2] + hips[f][2];
                }
                o = add(&bin, v, sizeof(float) * 3 * frames, 5126, frames, "VEC3", NULL);
                say(&sm, ",{\"input\":%d,\"output\":%d,\"interpolation\":\"LINEAR\"}", time_a, o);
                say(&ch, ",{\"sampler\":%d,\"target\":{\"node\":%d,\"path\":\"translation\"}}", channels, slot[b]);
                channels++;
            }
            free(v);
        }
        say(&anims, "%s{\"name\":\"%s\",\"channels\":[%s],\"samplers\":[%s]}", nmoves ? "," : "", mname, ch.s, sm.s);
        nmoves++;
        free(ch.s);
        free(sm.s);
        free(times);
        free(rot);
        free(hips);
    }
    /* the JSON: nodes (the bones, then the mesh's), its skin, material, texture and the moves */
    say(&json, "{\"asset\":{\"version\":\"2.0\",\"generator\":\"herotool\"},\"scene\":0,\"scenes\":[{\"nodes\":[%d,%d]}],\"nodes\":[",
        slot[KB_HIPS], nb);
    for (b = 0; b < KB_COUNT; b++) {
        int par = KitBoneParent[b], c, first = 1;
        const float *at = m->bones[b].at, *pa = par >= 0 ? m->bones[par].at : NULL;

        if (slot[b] < 0) {
            continue;
        }
        say(&json, "%s{\"name\":\"%s\",\"translation\":[%g,%g,%g]", slot[b] ? "," : "", KitBoneNames[b], at[0] - (pa ? pa[0] : 0),
            at[1] - (pa ? pa[1] : 0), at[2] - (pa ? pa[2] : 0));
        for (c = 0; c < KB_COUNT; c++) {
            if (slot[c] >= 0 && KitBoneParent[c] == b) {
                say(&json, "%s%d", first ? ",\"children\":[" : ",", slot[c]);
                first = 0;
            }
        }
        say(&json, "%s}", first ? "" : "]");
    }
    say(&json, ",{\"name\":\"%s\",\"mesh\":0,\"skin\":0}],", p->id);
    say(&json, "\"meshes\":[{\"primitives\":[{\"attributes\":{\"POSITION\":%d,\"TEXCOORD_0\":%d,\"COLOR_0\":%d,\"JOINTS_0\":%d,"
               "\"WEIGHTS_0\":%d},\"indices\":%d,\"material\":0}]}],",
        pos_a, uv_a, col_a, joint_a, weight_a, idx_a);
    say(&json, "\"materials\":[{\"pbrMetallicRoughness\":{\"baseColorTexture\":{\"index\":0},\"metallicFactor\":0,\"roughnessFactor\":1}}],"
               "\"textures\":[{\"source\":0,\"sampler\":0}],\"samplers\":[{\"magFilter\":9728,\"minFilter\":9728}],"
               "\"images\":[{\"bufferView\":%d,\"mimeType\":\"image/png\"}],",
        image_view);
    say(&json, "\"skins\":[{\"joints\":[");
    for (b = 0; b < nb; b++) {
        say(&json, "%s%d", b ? "," : "", b);
    }
    say(&json, "],\"inverseBindMatrices\":%d,\"skeleton\":%d}],\"animations\":[%s],", ibm_a, slot[KB_HIPS], anims.s);
    say(&json, "\"buffers\":[{\"byteLength\":%zu}],\"bufferViews\":[%s],\"accessors\":[%s]}", (bin.n + 3) & ~(size_t)3, Views.s, Accessors.s);
    while (json.n % 4) {
        say(&json, " ");
    }
    put(&bin, NULL, 0);
    /* the folders, the GLB, hero.json and mod.json */
    snprintf(path, sizeof(path), "%s/heroes/%s", out, p->id);
    if (!SDL_CreateDirectory(path)) {
        snprintf(why, why_size, "can't make %s", path);
        return 0;
    }
    snprintf(path, sizeof(path), "%s/heroes/%s/hero.glb", out, p->id);
    io = SDL_IOFromFile(path, "wb");
    if (io == NULL) {
        snprintf(why, why_size, "can't write %s", path);
        return 0;
    }
    memcpy(head, "glTF", 4);
    {
        uint32_t total = (uint32_t)(12 + 8 + json.n + 8 + bin.n);

        memcpy(head + 4, &(uint32_t){ 2 }, 4);
        memcpy(head + 8, &total, 4);
        memcpy(th, &(uint32_t){ (uint32_t)json.n }, 4);
        memcpy(th + 4, "JSON", 4);
        memcpy(dh, &(uint32_t){ (uint32_t)bin.n }, 4);
        memcpy(dh + 4, "BIN\0", 4);
        SDL_WriteIO(io, head, 12);
        SDL_WriteIO(io, th, 8);
        SDL_WriteIO(io, json.s, json.n);
        SDL_WriteIO(io, dh, 8);
        SDL_WriteIO(io, bin.b, bin.n);
    }
    SDL_CloseIO(io);
    {
        Text hero = { 0 };

        say(&hero, "{\n  \"format\": 1,\n  \"id\": \"%s\",\n  \"name\": \"%s\",\n  \"prefer\": \"%s\",\n  \"height\": %d,\n  \"face\": \"BREATHE\",\n"
                   "  \"face_turn\": 20,\n",
            p->id, p->name, p->prefer, p->height);
        /* said, Crash too: a pack that says its template follows it on foot as well (one that doesn't keeps its own moves there) */
        if (p->template_hero != NULL) {
            say(&hero, "  \"template\": \"%s\",\n", p->template_hero);
        }
        if (p->face_head > 0) {
            say(&hero, "  \"face_head\": %g,\n", p->face_head);
        }
        if (p->arm_lift > 0 && p->arm_lift < 180) {
            say(&hero, "  \"arm_lift\": %d,\n", p->arm_lift);
        }
        say(&hero, "  \"kinds\": {\n    \"foot\": {\n      \"subframes\": 2,\n      \"otherwise\": \"BREATHE\",\n      \"stands\": [\n"
                   "        [\"MEDIUM\", \"RUN\"], [\"SKATE\", \"RUN\"], [\"SLIDE\", \"RUN\"], [\"SWIM\", \"RUN\"], [\"FLIP\", \"JUMP\"],\n"
                   "        [\"FALL\", \"JUMP\"], [\"PUSH\", \"RUN\"], [\"TRANS\", \"RUN\"], [\"BARGE\", \"RUN\"], [\"BANK\", \"BREATHE\"],\n"
                   "        [\"KICK\", \"ATTACK\"], [\"WIN\", \"WIN\", \"loop\"], [\"WIN2\", \"WIN\", \"loop\"], [\"TAUNT_A\", \"TAUNT\"],\n"
                   "        [\"FIRE1\", \"HOLD_THROW\"], [\"FIRE2\", \"HOLD_THROW\"], [\"FIRE3\", \"HOLD_THROW\"], [\"MINE\", \"PICKUP\"],\n"
                   "        [\"RECOIL\", \"HIT\"], [\"TAZING\", \"ATTACK\"], [\"LIGHT\", \"TAUNT\"],\n"
                   "        [\"HIT\", \"HIT\", \"once\"], [\"DAZED\", \"HIT\", \"once\"], [\"DIE\", \"HIT\", \"once\"]\n      ]");
        if (p->kind_template[0] != NULL) {
            say(&hero, ",\n      \"template\": \"%s\"", p->kind_template[0]);
        }
        say(&hero, "\n    }");
        /* each other game whose ride or prop is another hero's than its template's, or that it isn't drawn in to sit in */
        for (int k = 1; k < PACK_KINDS; k++) {
            if (p->kind_template[k] != NULL && p->no_fit[k]) {
                say(&hero, ",\n    \"%s\": { \"template\": \"%s\", \"fit\": false }", pack_kind_name(k), p->kind_template[k]);
            } else if (p->kind_template[k] != NULL) {
                say(&hero, ",\n    \"%s\": { \"template\": \"%s\" }", pack_kind_name(k), p->kind_template[k]);
            } else if (p->no_fit[k]) {
                say(&hero, ",\n    \"%s\": { \"fit\": false }", pack_kind_name(k));
            }
        }
        say(&hero, "\n  }");
        /* its moves adjusted (the animation tools): each with a turn or a reach */
        {
            int said = 0;

            for (int i = 0; i < p->nadjust; i++) {
                const PackAdjust *a = &p->adjust[i];
                int turn = a->turn[0] != 0 || a->turn[1] != 0 || a->turn[2] != 0, reach = a->reach[0] != 0 || a->reach[1] != 0 || a->reach[2] != 0;
                int grow = a->grow[0] != 0 || a->grow[1] != 0 || a->grow[2] != 0;

                if (!turn && !reach && !grow) {
                    continue;
                }
                say(&hero, "%s\n    { \"kind\": \"%s\", \"move\": \"%s\", \"bone\": \"%s\"", said++ ? "," : ",\n  \"adjust\": [",
                    pack_kind_name(a->kind), a->move, a->bone);
                if (a->at >= 0) {
                    say(&hero, ", \"at\": %d", a->at);
                    if (a->until > a->at) {
                        say(&hero, ", \"until\": %d", a->until);
                    }
                    say(&hero, ", \"spread\": %d", a->spread);
                    if (a->ease != PACK_EASE_SMOOTH) {
                        say(&hero, ", \"ease\": \"%s\"", pack_ease_name(a->ease));
                    }
                    if (a->blend) {
                        say(&hero, ", \"blend\": true");
                    }
                }
                if (turn) {
                    /* (+ 0: a value turned the other way from 0 written 0, not -0) */
                    say(&hero, ", \"turn\": [%.4g, %.4g, %.4g]", a->turn[0] + 0.0f, a->turn[1] + 0.0f, a->turn[2] + 0.0f);
                }
                if (reach) {
                    say(&hero, ", \"reach\": [%.4g, %.4g, %.4g]", a->reach[0] + 0.0f, a->reach[1] + 0.0f, a->reach[2] + 0.0f);
                }
                if (grow) {
                    say(&hero, ", \"scale\": [%.4g, %.4g, %.4g]", 1 + a->grow[0], 1 + a->grow[1], 1 + a->grow[2]);
                }
                say(&hero, " }");
            }
            if (said) {
                say(&hero, "\n  ]");
            }
        }
        say(&hero, "\n}\n");
        snprintf(path, sizeof(path), "%s/heroes/%s/hero.json", out, p->id);
        if (!write_file(path, hero.s, hero.n)) {
            snprintf(why, why_size, "can't write %s", path);
            return 0;
        }
        free(hero.s);
        hero = (Text){ 0 };
        say(&hero, "{\n  \"name\": \"%s\",\n  \"author\": \"%s\",\n  \"version\": \"1.0\",\n  \"description\": \"%s as a hero of their own\"\n}\n",
            p->mod_name, p->author, p->mod_name);
        snprintf(path, sizeof(path), "%s/mod.json", out);
        write_file(path, hero.s, hero.n);
        free(hero.s);
    }
    /* its card and sounds, each checked the game can read it */
    if (p->card != NULL) {
        size_t n;
        void *png = SDL_LoadFile(p->card, &n);
        SDL_Surface *s = png != NULL ? SDL_LoadPNG_IO(SDL_IOFromConstMem(png, n), true) : NULL;

        snprintf(path, sizeof(path), "%s/heroes/%s/card.png", out, p->id);
        if (s == NULL || !write_file(path, png, n)) {
            snprintf(why, why_size, "its card %s isn't a PNG", p->card);
            SDL_free(png);
            return 0;
        }
        SDL_DestroySurface(s);
        SDL_free(png);
    }
    for (i = 0; i < PACK_VOICE_JOBS; i++) {
        size_t n;
        void *wav;
        SDL_AudioSpec spec;
        Uint8 *buf;
        Uint32 len;

        if (p->voice[i] == NULL) {
            continue;
        }
        wav = SDL_LoadFile(p->voice[i], &n);
        if (wav == NULL || !SDL_LoadWAV_IO(SDL_IOFromConstMem(wav, n), true, &spec, &buf, &len)) {
            snprintf(why, why_size, "its %s sound %s isn't a WAV", PackVoiceJobs[i], p->voice[i]);
            SDL_free(wav);
            return 0;
        }
        SDL_free(buf);
        snprintf(path, sizeof(path), "%s/heroes/%s/voice_%s.wav", out, p->id, PackVoiceJobs[i]);
        write_file(path, wav, n);
        SDL_free(wav);
        printf("herotool: its %s sound, %.2f s\n", PackVoiceJobs[i], (double)len / (SDL_AUDIO_FRAMESIZE(spec) * spec.freq));
    }
    printf("herotool: wrote %s/heroes/%s: %d bones, %d moves (%d its own clips), %zu bytes\n", out, p->id, nb, nmoves, own,
           12 + 8 + json.n + 8 + bin.n);
    free(json.s);
    free(anims.s);
    free(bin.b);
    free(Views.s);
    free(Accessors.s);
    free(rgba);
    free(joints);
    free(idx);
    free(ibm);
    return 1;
}
