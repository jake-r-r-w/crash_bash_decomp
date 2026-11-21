/* Heroes of their own, from the mods that are on (bash/bonusheroes.h's pack_heroes_scan; docs/HERO_PACKS.md): each a hero pack,
 * heroes/<id>/ in a mod, as the hero tool writes it. Its hero.json is its HeroDef, and its hero.glb its models, made here as a hero's
 * file in main/heroes/ makes them from its disc (made_write): its mesh skinned to its skeleton for each frame of each of its clips, in
 * Crash Bash's units and axes, its textures as the game's indexed images.
 *
 * What it gives the game's play is whole numbers from hero.json (its height, base, plays-like): its mesh, made in floats, only changes
 * how it's drawn, so a machine's own rounding never reaches the game's state. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "bash/extraheroes.h"
#include "bash/bonusheroes.h"
#include "bash/heromake.h"
#include "../../platform/heropack.h"
#include "../../platform/mods.h"

#define PACK_TICKS 30.0f        /* the game's ticks a second: a clip's second is this many of its frames, times its subframes */
#define PACK_JSON_BYTES (256 * 1024)
#define PACK_GLB_BYTES (32 * 1024 * 1024)
#define PACK_CARD_BYTES (4 * 1024 * 1024)
#define PACK_MAX_ANIMS 64
#define PACK_MAX_VERTS 4096     /* extraheroes.c's MORPH_VERTS: a frame's corners as they're drawn */
#define PACK_IMAGE_MAX 256      /* an image's texels across or down at most: a corner's place in it is a byte */
/* How a pack's faces are lit as it stands, its light baked in its colours as the game's heroes' are: wrapped, so the side away from the
 * light still shades (Pinstripe's, docs/EXTRA_HEROES.md: the game's heroes are bright and soft) */
static const V3 PackLight = { -0.35f, -0.75f, -0.55f };
#define PACK_LIGHT_BASE 0.8f
#define PACK_LIGHT_GAIN 0.5f

typedef struct {
    PackInfo info;
    HeroDef def;
    Stand stands[NUM_KINDS][PACK_MAX_STANDS + 1];
    Voice voices[HERO_SOUNDS + 1];
    u32 sig;       /* its mod's and its own files, hashed: a change makes it again */
    s32 index;     /* its place among the mods' heroes (mods_hero_file) */
    u32 *card;
    s32 card_w, card_h;
} PackSlot;

static PackSlot Slots[EXTRA_PACKS];

static s32 make_pack(s32 slot, HeroMade *out);
static s32 make_pack1(HeroMade *out) { return make_pack(0, out); }
static s32 make_pack2(HeroMade *out) { return make_pack(1, out); }
static s32 make_pack3(HeroMade *out) { return make_pack(2, out); }
static s32 make_pack4(HeroMade *out) { return make_pack(3, out); }
static s32 (*const PackMakes[EXTRA_PACKS])(HeroMade *) = { make_pack1, make_pack2, make_pack3, make_pack4 };

static u32 fnv(u32 h, const void *p, size_t n) {
    const u8 *b = p;
    size_t i;

    for (i = 0; i < n; i++) {
        h = (h ^ b[i]) * 16777619u;
    }
    return h;
}

/* Two names the same, whatever their case (of the first n letters) */
static s32 same_n(const char *a, const char *b, size_t n) {
    size_t i;

    for (i = 0; i < n; i++) {
        char x = a[i] >= 'a' && a[i] <= 'z' ? (char)(a[i] - 32) : a[i], y = b[i] >= 'a' && b[i] <= 'z' ? (char)(b[i] - 32) : b[i];

        if (x != y) {
            return 0;
        }
        if (x == 0) {
            return 1;
        }
    }
    return 1;
}

static s32 same(const char *a, const char *b) {
    return same_n(a, b, (size_t)-1);
}

/* --- the slots --- */

static void slot_def(PackSlot *s, s32 slot) {
    HeroDef *d = &s->def;
    s32 k, i, n;

    memset(d, 0, sizeof(*d));
    d->name = s->info.name;
    d->id = s->info.id;
    d->disc = NULL;
    d->prefer = s->info.prefer;
    for (k = 0; k < NUM_KINDS; k++) {
        const PackKind *pk = &s->info.kinds[k];

        for (i = 0; i < pk->nstands; i++) {
            s->stands[k][i].from = pk->stands[i].from;
            s->stands[k][i].to = pk->stands[i].to;
            s->stands[k][i].pace = pk->stands[i].pace == 1 ? LOOP : pk->stands[i].pace == 2 ? ONCE : STRETCH;
        }
        s->stands[k][i].from = NULL;
        s->stands[k][i].to = NULL;
        s->stands[k][i].pace = STRETCH;
        d->kinds[k].stands = s->stands[k];
        d->kinds[k].otherwise = pk->otherwise[0] != 0 ? pk->otherwise : "BREATHE";
        d->kinds[k].keep_lift = pk->keep_lift;
        d->kinds[k].like = pk->like;
        d->kinds[k].subframes = pk->subframes;
    }
    /* its base's sounds where it says so, none where it says none; its own (voice_NN.wav) are its base's until they're read */
    for (k = 0, n = 0; k < HERO_SOUNDS; k++) {
        if (s->info.voices[k] != 0) {
            s->voices[n].offset = k;
            s->voices[n++].voice = VOICE_BASE;
        }
    }
    s->voices[n].offset = -1;
    s->voices[n].voice = VOICE_NONE;
    d->voices = s->voices;
    d->face = s->info.face;
    d->face_turn = s->info.face_turn;
    d->make = PackMakes[slot];
}

void pack_heroes_scan(void) {
    s32 n = mods_heroes_count(), slot;

    for (slot = 0; slot < EXTRA_PACKS; slot++) {
        PackSlot *s = &Slots[slot];
        s32 x = EXTRA_PACK1 + slot;
        u8 *json = NULL, *glb = NULL;
        size_t json_size = 0, glb_size = 0;
        u32 sig = 0;
        char problem[64];

        if (slot < n) {
            const char *mod = mods_hero_mod(slot), *id = mods_hero_id(slot);

            json = mods_hero_file(slot, "hero.json", PACK_JSON_BYTES, &json_size);
            glb = mods_hero_file(slot, "hero.glb", PACK_GLB_BYTES, &glb_size);
            sig = fnv(2166136261u, mod, strlen(mod) + 1);
            sig = fnv(sig, id, strlen(id) + 1);
            sig = json != NULL ? fnv(sig, json, json_size) : sig;
            sig = glb != NULL ? fnv(fnv(sig, &glb_size, sizeof(glb_size)), glb, glb_size < 65536 ? glb_size : 65536) : sig;
        }
        free(glb);
        if (sig == s->sig && (sig == 0 || HeroDefs[x] != NULL)) {
            free(json);
            continue;
        }
        bonus_heroes_forget(x);
        HeroDefs[x] = NULL;
        free(s->card);
        memset(s, 0, sizeof(*s));
        s->sig = sig;
        if (slot >= n) {
            free(json);
            continue;
        }
        if (json == NULL || !pack_info_read((const char *)json, json_size, &s->info, problem, sizeof(problem))) {
            printf("hero packs: %s, in %s: %s\n", mods_hero_id(slot), mods_hero_mod(slot), json == NULL ? "NO HERO.JSON" : problem);
            free(json);
            continue;
        }
        free(json);
        /* its folder's name is its id, whatever hero.json says */
        snprintf(s->info.id, sizeof(s->info.id), "%s", mods_hero_id(slot));
        s->index = slot;
        slot_def(s, slot);
        HeroDefs[x] = &s->def;
        printf("hero packs: %s (%s), in %s, in slot %d\n", s->info.id, s->info.name, mods_hero_mod(slot), slot + 1);
    }
}

/* --- its textures, as the game's images: 8 bits a texel, a palette of 256 with 0 clear --- */

typedef struct {
    s32 lo, hi;    /* its colours, in Colours */
    s32 count;
} Box;

static u16 Colours[32768];
static u32 Counts[32768];
static s16 PaletteOf[32768];

static s32 axis_of(u16 c, s32 axis) {
    return c >> (axis * 5) & 31;
}

static s32 by_axis;
static s32 cmp_axis(const void *a, const void *b) {
    return axis_of(*(const u16 *)a, by_axis) - axis_of(*(const u16 *)b, by_axis);
}

/* The colours of an image's opaque texels (15 bits each) into at most `max` (median cut), each colour's place in the palette into
 * PaletteOf and the palette's colours into out; how many */
static s32 palette_of(u16 *out, s32 max) {
    static Box boxes[256];
    s32 ncol = 0, nbox = 1, c, i, b;

    for (c = 0; c < 32768; c++) {
        if (Counts[c] != 0) {
            Colours[ncol++] = (u16)c;
        }
    }
    if (ncol == 0) {
        return 0;
    }
    boxes[0].lo = 0;
    boxes[0].hi = ncol;
    while (nbox < max && nbox < ncol) {
        s32 best = -1, best_axis = 0;
        u32 best_score = 0;

        /* the box with the most texels times its widest spread */
        for (b = 0; b < nbox; b++) {
            s32 lo[3] = { 31, 31, 31 }, hi[3] = { 0, 0, 0 }, a, wide = 0, wide_axis = 0;
            u32 texels = 0;

            if (boxes[b].hi - boxes[b].lo < 2) {
                continue;
            }
            for (i = boxes[b].lo; i < boxes[b].hi; i++) {
                for (a = 0; a < 3; a++) {
                    s32 v = axis_of(Colours[i], a);

                    lo[a] = v < lo[a] ? v : lo[a];
                    hi[a] = v > hi[a] ? v : hi[a];
                }
                texels += Counts[Colours[i]];
            }
            for (a = 0; a < 3; a++) {
                if (hi[a] - lo[a] > wide) {
                    wide = hi[a] - lo[a];
                    wide_axis = a;
                }
            }
            if (wide > 0 && texels * (u32)wide > best_score) {
                best_score = texels * (u32)wide;
                best = b;
                best_axis = wide_axis;
            }
        }
        if (best < 0) {
            break;
        }
        {
            Box *bx = &boxes[best];
            u32 total = 0, half = 0;

            by_axis = best_axis;
            qsort(&Colours[bx->lo], (size_t)(bx->hi - bx->lo), sizeof(u16), cmp_axis);
            for (i = bx->lo; i < bx->hi; i++) {
                total += Counts[Colours[i]];
            }
            for (i = bx->lo; i < bx->hi - 1 && half + Counts[Colours[i]] <= total / 2; i++) {
                half += Counts[Colours[i]];
            }
            i = i == bx->lo ? bx->lo + 1 : i;
            boxes[nbox].lo = i;
            boxes[nbox].hi = bx->hi;
            bx->hi = i;
            nbox++;
        }
    }
    for (b = 0; b < nbox; b++) {
        double sum[3] = { 0, 0, 0 }, n = 0;
        s32 a;

        for (i = boxes[b].lo; i < boxes[b].hi; i++) {
            for (a = 0; a < 3; a++) {
                sum[a] += (double)axis_of(Colours[i], a) * Counts[Colours[i]];
            }
            n += Counts[Colours[i]];
            PaletteOf[Colours[i]] = (s16)b;
        }
        out[b] = 0;
        for (a = 0; a < 3; a++) {
            out[b] |= (u16)((s32)(sum[a] / (n > 0 ? n : 1) + 0.5) << (a * 5));
        }
    }
    return nbox;
}

/* An image, made a power of two no bigger than PACK_IMAGE_MAX each way (a box filter), into a made image */
static s32 made_image(const PackImage *src, MadeImage *im) {
    s32 w = 8, h = 8, x, y, n, i;
    u8 *texels;
    u16 pal[255];

    memset(im, 0, sizeof(*im));
    if (src->rgba == NULL || src->w <= 0 || src->h <= 0) {
        return 0;
    }
    while (w * 2 <= src->w && w < PACK_IMAGE_MAX) {
        w *= 2;
    }
    while (h * 2 <= src->h && h < PACK_IMAGE_MAX) {
        h *= 2;
    }
    texels = malloc((size_t)w * h * 4);
    im->pixels = malloc((size_t)w * h);
    if (texels == NULL || im->pixels == NULL) {
        free(texels);
        free(im->pixels);
        im->pixels = NULL;
        return 0;
    }
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            s32 x0 = x * src->w / w, x1 = (x + 1) * src->w / w, y0 = y * src->h / h, y1 = (y + 1) * src->h / h, sx, sy, c;
            u32 sum[4] = { 0, 0, 0, 0 }, cnt = 0;

            x1 = x1 > x0 ? x1 : x0 + 1;
            y1 = y1 > y0 ? y1 : y0 + 1;
            for (sy = y0; sy < y1 && sy < src->h; sy++) {
                for (sx = x0; sx < x1 && sx < src->w; sx++) {
                    for (c = 0; c < 4; c++) {
                        sum[c] += src->rgba[((size_t)sy * src->w + sx) * 4 + c];
                    }
                    cnt++;
                }
            }
            for (c = 0; c < 4; c++) {
                texels[((size_t)y * w + x) * 4 + c] = (u8)(cnt != 0 ? sum[c] / cnt : 0);
            }
        }
    }
    memset(Counts, 0, sizeof(Counts));
    for (i = 0; i < w * h; i++) {
        const u8 *t = &texels[i * 4];

        if (t[3] >= 128) {
            Counts[t[0] >> 3 | (t[1] >> 3) << 5 | (t[2] >> 3) << 10]++;
        }
    }
    n = palette_of(pal, 255);
    for (i = 0; i < w * h; i++) {
        const u8 *t = &texels[i * 4];

        im->pixels[i] = t[3] >= 128 ? (u8)(1 + PaletteOf[t[0] >> 3 | (t[1] >> 3) << 5 | (t[2] >> 3) << 10]) : 0;
    }
    free(texels);
    im->bpp8 = 1;
    im->units = w / 2;
    im->h = h;
    im->colours = 256;
    /* 0 clear; the rest opaque, black too (the top bit set) */
    im->clut[0] = 0;
    for (i = 0; i < n; i++) {
        im->clut[1 + i] = (u16)(0x8000 | pal[i]);
    }
    return 1;
}

/* --- its models --- */

typedef struct {
    const PackModel *pm;
    s32 clip[PACK_MAX_ANIMS]; /* each animation's clip, -1 its rest pose */
    float length[PACK_MAX_ANIMS];
    s32 frames[PACK_MAX_ANIMS];
    float k, ground;          /* glTF's metres to Crash Bash's units, and its feet's height as it rests */
} Foot;

static Foot TheFoot;

/* Frame f of animation `anim`: the clip's pose at its time, skinned, then turned into Crash Bash's axes (y down, facing -z: glTF faces
 * +z, so it's turned about x, which keeps its faces' winding) and units, its feet at 0 */
static void foot_frame(void *ctx, s32 anim, s32 f, V3 *out) {
    static PackTrs local[PACK_MAX_BONES];
    static float world[PACK_MAX_BONES][16], skinned[PACK_MAX_VERTS * 3];
    const Foot *ft = ctx;
    const PackModel *pm = ft->pm;
    float t = ft->frames[anim] > 1 ? ft->length[anim] * (float)f / (float)(ft->frames[anim] - 1) : 0;
    s32 v;

    pack_pose_local(pm, ft->clip[anim], t, local);
    pack_pose_world(pm, local, world);
    pack_skin(pm, (const float(*)[16])world, skinned);
    for (v = 0; v < pm->nvert; v++) {
        out[v].x = skinned[v * 3] * ft->k;
        out[v].y = -(skinned[v * 3 + 1] - ft->ground) * ft->k;
        out[v].z = -skinned[v * 3 + 2] * ft->k;
    }
}

/* A clip's animation name for a kind: KIND/MOVE, or for on foot MOVE alone too; NULL if it's another kind's */
static const char *clip_move(const char *clip, s32 kind) {
    const char *slash = strchr(clip, '/');
    const char *kname = pack_kind_name(kind);

    if (slash == NULL) {
        return kind == KIND_FOOT ? clip : NULL;
    }
    return (size_t)(slash - clip) == strlen(kname) && same_n(clip, kname, strlen(kname)) ? slash + 1 : NULL;
}

static s32 make_pack(s32 slot, HeroMade *out) {
    static PackModel pm;
    static MadeFace faces[MADE_MAX_FACES];
    static MadeImage images[PACK_MAX_IMAGES];
    static MadeAnim anims[PACK_MAX_ANIMS];
    static char names[PACK_MAX_ANIMS][PACK_ANIM_NAME];
    static float rest[PACK_MAX_VERTS * 3];
    static V3 normal[PACK_MAX_VERTS];
    static PackTrs local[PACK_MAX_BONES];
    static float world[PACK_MAX_BONES][16];
    PackSlot *s = &Slots[slot];
    s32 image_of[PACK_MAX_IMAGES], nimages = 0, nanims = 0, rest_anim = -1, i, j, c, ok = 0;
    float lo = 1e9f, hi = -1e9f;
    size_t size = 0;
    u8 *glb = mods_hero_file(s->index, "hero.glb", PACK_GLB_BYTES, &size);
    char problem[64];
    MadeModel mm;

    if (glb == NULL) {
        printf("hero packs: %s has no hero.glb\n", s->info.id);
        return 0;
    }
    if (!pack_model_read(glb, size, &pm, problem, sizeof(problem))) {
        printf("hero packs: %s: %s\n", s->info.id, problem);
        free(glb);
        return 0;
    }
    free(glb);
    if (pm.nvert > PACK_MAX_VERTS || pm.ntri > MADE_MAX_FACES) {
        printf("hero packs: %s has %d vertices and %d triangles: the game draws %d and %d at most\n", s->info.id, pm.nvert, pm.ntri,
               PACK_MAX_VERTS, MADE_MAX_FACES);
        goto done;
    }

    /* how tall it stands: its rest pose's feet to its top, made its height */
    pack_pose_local(&pm, -1, 0, local);
    pack_pose_world(&pm, local, world);
    pack_skin(&pm, (const float(*)[16])world, rest);
    for (i = 0; i < pm.nvert; i++) {
        lo = rest[i * 3 + 1] < lo ? rest[i * 3 + 1] : lo;
        hi = rest[i * 3 + 1] > hi ? rest[i * 3 + 1] : hi;
    }
    if (hi - lo < 1e-4f) {
        printf("hero packs: %s has no height\n", s->info.id);
        goto done;
    }
    TheFoot.pm = &pm;
    TheFoot.k = (float)s->info.height / (hi - lo);
    TheFoot.ground = lo;

    /* its images */
    for (i = 0; i < pm.nimages; i++) {
        image_of[i] = nimages < MADE_MAX_IMAGES && made_image(&pm.images[i], &images[nimages]) ? nimages++ : -1;
    }

    /* its animations on foot: its clips for it, at their own length, and BREATHE as it rests if it has no clip of that */
    for (i = 0; i < pm.nclips && nanims < PACK_MAX_ANIMS; i++) {
        const char *move = clip_move(pm.clips[i].name, KIND_FOOT);
        s32 sub = s->info.kinds[KIND_FOOT].subframes, frames;

        if (move == NULL || move[0] == 0) {
            continue;
        }
        for (j = 0; j < nanims && !same(names[j], move); j++) {
        }
        if (j < nanims) {
            continue;
        }
        for (c = 0; move[c] != 0 && c + 1 < PACK_ANIM_NAME; c++) {
            names[nanims][c] = (char)(move[c] >= 'a' && move[c] <= 'z' ? move[c] - 'a' + 'A' : move[c]);
        }
        names[nanims][c] = 0;
        frames = (s32)(pm.clips[i].length * PACK_TICKS * sub + 0.5f) + 1;
        frames = frames < 2 ? 2 : frames > MADE_MAX_FRAMES ? MADE_MAX_FRAMES : frames;
        TheFoot.clip[nanims] = i;
        TheFoot.length[nanims] = pm.clips[i].length;
        TheFoot.frames[nanims] = frames;
        anims[nanims].name = names[nanims];
        anims[nanims].frames = frames;
        anims[nanims].mesh = 0;
        anims[nanims].step = sub;
        rest_anim = strcmp(names[nanims], "BREATHE") == 0 ? nanims : rest_anim;
        nanims++;
    }
    if (rest_anim < 0 && nanims < PACK_MAX_ANIMS) {
        strcpy(names[nanims], "BREATHE");
        TheFoot.clip[nanims] = -1;
        TheFoot.length[nanims] = 0;
        TheFoot.frames[nanims] = 2;
        anims[nanims].name = names[nanims];
        anims[nanims].frames = 2;
        anims[nanims].mesh = 0;
        anims[nanims].step = 1;
        rest_anim = nanims++;
    }

    /* its faces, each corner's colour lit as it rests (its normals the mean of its faces' at each place, across its seams) */
    {
        static V3 game[PACK_MAX_VERTS];

        foot_frame(&TheFoot, rest_anim, 0, game);
        memset(normal, 0, sizeof(normal));
        for (i = 0; i < pm.ntri; i++) {
            const V3 *a = &game[pm.tri[i * 3]], *b = &game[pm.tri[i * 3 + 1]], *d = &game[pm.tri[i * 3 + 2]];
            V3 e1 = { b->x - a->x, b->y - a->y, b->z - a->z }, e2 = { d->x - a->x, d->y - a->y, d->z - a->z };
            V3 n = { e1.y * e2.z - e1.z * e2.y, e1.z * e2.x - e1.x * e2.z, e1.x * e2.y - e1.y * e2.x };

            for (j = 0; j < 3; j++) {
                V3 *to = &normal[pm.tri[i * 3 + j]];

                to->x += n.x;
                to->y += n.y;
                to->z += n.z;
            }
        }
        for (i = 0; i < pm.nvert; i++) {
            V3 sum = normal[i];

            for (j = 0; j < pm.nvert; j++) {
                if (j != i && fabsf(game[j].x - game[i].x) < 0.5f && fabsf(game[j].y - game[i].y) < 0.5f
                    && fabsf(game[j].z - game[i].z) < 0.5f) {
                    sum.x += normal[j].x;
                    sum.y += normal[j].y;
                    sum.z += normal[j].z;
                }
            }
            rest[i * 3] = sum.x;
            rest[i * 3 + 1] = sum.y;
            rest[i * 3 + 2] = sum.z;
        }
    }
    for (i = 0; i < pm.ntri; i++) {
        MadeFace *f = &faces[i];
        s32 im = pm.tri_image[i] >= 0 && pm.tri_image[i] < pm.nimages ? image_of[pm.tri_image[i]] : -1;

        memset(f, 0, sizeof(*f));
        f->image = im;
        f->blend = pm.tri_blend[i] ? 0x8000 : 0;
        for (j = 0; j < 3; j++) {
            s32 v = pm.tri[i * 3 + j];
            const float *nn = &rest[v * 3];
            float len = sqrtf(nn[0] * nn[0] + nn[1] * nn[1] + nn[2] * nn[2]), ll = sqrtf(PackLight.x * PackLight.x
                          + PackLight.y * PackLight.y + PackLight.z * PackLight.z), lit = 0;

            f->v[j] = v;
            if (im >= 0) {
                float w = (float)(images[im].units * 2), h = (float)images[im].h, u = pm.uv[v * 2] * w, t = pm.uv[v * 2 + 1] * h;

                f->uv[j][0] = (u8)(u < 0 ? 0 : u > 255 ? 255 : u > w - 1 ? w - 1 : u + 0.5f);
                f->uv[j][1] = (u8)(t < 0 ? 0 : t > 255 ? 255 : t > h - 1 ? h - 1 : t + 0.5f);
            }
            if (len > 0) {
                lit = (nn[0] * PackLight.x + nn[1] * PackLight.y + nn[2] * PackLight.z) / (len * ll);
            }
            lit = pm.tri_unlit[i] ? 1 : PACK_LIGHT_BASE + PACK_LIGHT_GAIN * ((1 + lit) / 2) * ((1 + lit) / 2);
            for (c = 0; c < 3; c++) {
                float value = pm.rgba[v * 4 + c] * 128 * lit;

                f->rgb[0][j][c] = f->rgb[1][j][c] = (u8)(value > 255 ? 255 : value < 0 ? 0 : value);
            }
        }
    }

    memset(&mm, 0, sizeof(mm));
    mm.faces = faces;
    mm.nfaces = pm.ntri;
    mm.meshes = 1;
    mm.images = images;
    mm.nimages = nimages;
    mm.anims = anims;
    mm.nanims = nanims;
    mm.nvert = pm.nvert;
    mm.rest_anim = rest_anim;
    mm.frame = foot_frame;
    mm.ctx = &TheFoot;
    if (!made_write(&mm, &out->tex[KIND_FOOT], &out->tex_size[KIND_FOOT], &out->mdl[KIND_FOOT], &out->mdl_size[KIND_FOOT])) {
        printf("hero packs: %s's model on foot can't be written\n", s->info.id);
        goto done;
    }
    /* on the tournament's podium: standing, its win as it cheers, and its hit as it loses, if it has them */
    {
        WarpMove podium[4] = {
            { "WIN_BREATHE", rest_anim, 0, 1 }, { "WIN", rest_anim, 0, 1 }, { "LOSE_BREATHE", rest_anim, 0, 1 }, { "LOSE", rest_anim, 0, 1 },
        };

        for (i = 0; i < nanims; i++) {
            podium[1].anim = strcmp(names[i], "WIN") == 0 ? i : podium[1].anim;
            podium[3].anim = strcmp(names[i], "HIT") == 0 || strcmp(names[i], "DIE") == 0 ? i : podium[3].anim;
        }
        if (!hm_warp_make(&mm, podium, 4, s->info.prefer, &out->tex[KIND_WARP], &out->tex_size[KIND_WARP], &out->mdl[KIND_WARP],
                          &out->mdl_size[KIND_WARP])) {
            printf("hero packs: %s has no podium model\n", s->info.id);
        }
    }
    /* its card */
    {
        u8 *png = mods_hero_file(s->index, "card.png", PACK_CARD_BYTES, &size);
        int w = 0, h = 0;

        if (png != NULL && (s->card = pack_png_read(png, size, &w, &h)) != NULL) {
            s->card_w = w;
            s->card_h = h;
            out->card = s->card;
            out->card_w = w;
            out->card_h = h;
            out->card_head = h;
        }
        free(png);
    }
    printf("hero packs: %s made, %d triangles, %d vertices, %d bones, %d clips, %d textures%s\n", s->info.id, pm.ntri, pm.nvert,
           pm.nbones, nanims, nimages, out->card != NULL ? ", its card" : "");
    ok = 1;
done:
    made_images_free(images, nimages);
    pack_model_free(&pm);
    return ok;
}
