/* Bonus heroes (bash/bonusheroes.h, bash/heromake.h; PC only): the extra heroes' models, sounds and pictures, made from the player's own
 * copies of other Crash games, and what the heroes' own files (main/heroes/) use to make them.
 *
 * Each hero's make (bash/herodef.h) finds its game's disc (disc_image_find), reads what it needs from it, and makes it into a Crash Bash
 * model and texture set of each kind it has (docs/ASSETS.md): on foot, riding a polar bear, on a pogo stick, in a Ballistix car, in a
 * tank, in a race cart, riding a fish, and with a mallet. They go into the archive's entries for
 * that hero (main.c's DatIndex, from 992) as a mod's files do, and the game loads them as it does any model, into banks of their own
 * (DB_BonusRoo, ...). Nothing of them is kept but in memory, and without its disc there's no hero. docs/EXTRA_HEROES.md has how to add
 * one, from Ripper Roo's (heroes/roo.c). */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "bash/main.h"
#include "bash/modelbank.h"
#include "bash/extraheroes.h"
#include "bash/bonusheroes.h"
#include "bash/heromake.h"
#include "../../platform/audio.h"

void archive_replace(int entry, unsigned char *bytes, int size);
unsigned char *archive_own_file(int entry, int *size);
void SDL_free(void *mem);

extern const HeroDef HeroRoo;

const HeroDef *const HeroDefs[NUM_EXTRA] = { NULL, &HeroRoo };

/* Each hero's archive entries and bank for each kind it has (main.c, dbase.c, levelload.c's ModeDbaseLists) */
static const struct {
    s32 tex, mdl, bank;
} Slots[NUM_EXTRA][NUM_KINDS] = {
    { { 0, 0, DB_Empty }, { 0, 0, DB_Empty }, { 0, 0, DB_Empty }, { 0, 0, DB_Empty }, { 0, 0, DB_Empty }, { 0, 0, DB_Empty },
      { 0, 0, DB_Empty }, { 0, 0, DB_Empty } },
    { { 992, 993, DB_BonusRoo }, { 994, 995, DB_BonusRooPolar }, { 996, 997, DB_BonusRooPogo }, { 998, 999, DB_BonusRooBall },
      { 1000, 1001, DB_BonusRooTank }, { 1002, 1003, DB_BonusRooRace }, { 1004, 1005, DB_BonusRooSplash },
      { 1006, 1007, DB_BonusRooMallet } },
};

static HeroMade Made[NUM_EXTRA];
static s32 Tried[NUM_EXTRA], Have[NUM_EXTRA];

/* --- bytes --- */

u32 hm_rd32(const u8 *p) {
    return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24;
}

u16 hm_rd16(const u8 *p) {
    return (u16)(p[0] | p[1] << 8);
}

void hm_wr32(u8 *p, u32 v) {
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
    p[2] = (u8)(v >> 16);
    p[3] = (u8)(v >> 24);
}

void hm_wr16(u8 *p, u32 v) {
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
}

size_t hm_put(Bytes *o, const void *src, size_t n) {
    size_t at = o->n;

    if (o->n + n > o->cap) {
        size_t cap = o->cap ? o->cap : 4096;
        u8 *more;

        while (cap < o->n + n) {
            cap *= 2;
        }
        more = realloc(o->b, cap);
        if (more == NULL) {
            return at;
        }
        o->b = more;
        o->cap = cap;
    }
    if (src != NULL) {
        memcpy(o->b + o->n, src, n);
    } else {
        memset(o->b + o->n, 0, n);
    }
    o->n += n;
    return at;
}

void hm_align(Bytes *o, size_t to) {
    if (o->n % to != 0) {
        hm_put(o, NULL, to - o->n % to);
    }
}

void hm_self(Bytes *o, size_t field, size_t to) {
    hm_wr32(o->b + field, (u32)(s32)((long long)to - (long long)field));
}

/* --- points --- */

float hm_smooth(float a, float b, float x) {
    float t = (x - a) / (b - a);

    t = t < 0 ? 0 : t > 1 ? 1 : t;
    return t * t * (3 - 2 * t);
}

V3 hm_mix(V3 a, V3 b, float t) {
    V3 r;

    r.x = a.x + (b.x - a.x) * t;
    r.y = a.y + (b.y - a.y) * t;
    r.z = a.z + (b.z - a.z) * t;
    return r;
}

V3 hm_turn_x(V3 p, V3 c, float a) {
    float s = sinf(a), k = cosf(a), y = p.y - c.y, z = p.z - c.z;

    p.y = c.y + y * k - z * s;
    p.z = c.z + y * s + z * k;
    return p;
}

V3 hm_turn_y(V3 p, V3 c, float a) {
    float s = sinf(a), k = cosf(a), x = p.x - c.x, z = p.z - c.z;

    p.x = c.x + x * k + z * s;
    p.z = c.z - x * s + z * k;
    return p;
}

V3 hm_turn_z(V3 p, V3 c, float a) {
    float s = sinf(a), k = cosf(a), x = p.x - c.x, y = p.y - c.y;

    p.x = c.x + x * k - y * s;
    p.y = c.y + x * s + y * k;
    return p;
}

/* --- Crash Bandicoot's levels --- */

static s32 decompress(const u8 *in, size_t size, size_t *at, u8 *out) {
    size_t p = *at;
    s32 length, skip, pos = 0, rest;

    if (p + 12 > size) {
        return 0;
    }
    length = (s32)hm_rd32(in + p + 4);
    skip = (s32)hm_rd32(in + p + 8);
    if (length < 0 || length > C1_PAGE_BYTES || skip < 0) {
        return 0;
    }
    p += 12;
    while (pos < length) {
        s32 prefix;

        if (p >= size) {
            return 0;
        }
        prefix = in[p++];
        if (prefix & 0x80) {
            s32 b, seek, span, i;

            if (p >= size) {
                return 0;
            }
            b = in[p++];
            seek = b >> 3 | (prefix & 0x7F) << 5;
            span = (b & 7) == 7 ? 64 : (b & 7) + 3;
            if (seek > pos || seek == 0 || pos + span > C1_PAGE_BYTES) {
                return 0;
            }
            for (i = 0; i < span; i++) {
                out[pos + i] = out[pos - seek + i];
            }
            pos += span;
        } else {
            if (p + (size_t)prefix > size || pos + prefix > C1_PAGE_BYTES) {
                return 0;
            }
            memcpy(out + pos, in + p, (size_t)prefix);
            p += (size_t)prefix;
            pos += prefix;
        }
    }
    p += (size_t)skip;
    rest = C1_PAGE_BYTES - length < C1_PAGE_BYTES - pos ? C1_PAGE_BYTES - length : C1_PAGE_BYTES - pos;
    if (p + (size_t)(C1_PAGE_BYTES - length) > size) {
        return 0;
    }
    memcpy(out + pos, in + p, (size_t)rest);
    *at = p + (size_t)(C1_PAGE_BYTES - length);
    return 1;
}

s32 c1_level_read(DiscImage *disc, const char *path, C1Level *out) {
    size_t size = 0, at = 0;
    u8 *nsf = disc_image_read(disc, path, &size);
    s32 cap = 0;

    memset(out, 0, sizeof(*out));
    if (nsf == NULL) {
        return 0;
    }
    while (at + 12 <= size) {
        u16 magic = hm_rd16(nsf + at);
        u8 *page;

        if (out->count == cap) {
            u8 *more = realloc(out->pages, (size_t)(cap = cap ? cap * 2 : 64) * C1_PAGE_BYTES);

            if (more == NULL) {
                break;
            }
            out->pages = more;
        }
        page = out->pages + (size_t)out->count * C1_PAGE_BYTES;
        if (magic == 0x1234 && at + C1_PAGE_BYTES <= size) {
            memcpy(page, nsf + at, C1_PAGE_BYTES);
            at += C1_PAGE_BYTES;
        } else if (magic != 0x1235 || !decompress(nsf, size, &at, page)) {
            break;
        }
        out->count++;
    }
    SDL_free(nsf);
    return out->count > 0;
}

static u32 eid_of(const char *name) {
    static const char Chars[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_!";
    u32 eid = 1;
    s32 i;

    for (i = 0; i < 5; i++) {
        const char *c = strchr(Chars, name[i]);

        eid |= (u32)(c != NULL ? c - Chars : 0) << (25 - 6 * i);
    }
    return eid;
}

s32 c1_entry_find(const C1Level *l, const char *name, C1Entry *out) {
    u32 eid = eid_of(name);
    s32 p, k, i;

    for (p = 0; p < l->count; p++) {
        const u8 *page = l->pages + (size_t)p * C1_PAGE_BYTES;
        s32 n = (s32)hm_rd32(page + 8);

        /* a page of entries, or of sounds (3) */
        if ((hm_rd16(page + 2) != 0 && hm_rd16(page + 2) != 3) || n < 0 || n > 4096) {
            continue;
        }
        for (k = 0; k < n; k++) {
            u32 s = hm_rd32(page + 16 + k * 4), e = hm_rd32(page + 20 + k * 4);
            const u8 *en = page + s;
            s32 items;

            if (s >= e || e > C1_PAGE_BYTES || e - s < 16 || hm_rd32(en) != 0x100FFFF || hm_rd32(en + 4) != eid) {
                continue;
            }
            items = (s32)hm_rd32(en + 12);
            if (items < 0 || items > 128 || 20 + items * 4 > (s32)(e - s)) {
                return 0;
            }
            out->type = (s32)hm_rd32(en + 8);
            out->count = items;
            for (i = 0; i < items; i++) {
                u32 a = hm_rd32(en + 16 + i * 4), b = hm_rd32(en + 20 + i * 4);

                if (a > b || b > e - s) {
                    return 0;
                }
                out->item[i] = en + a;
                out->size[i] = (s32)(b - a);
            }
            return 1;
        }
    }
    return 0;
}

const u8 *c1_texture_page(const C1Level *l, u32 eid) {
    s32 p;

    for (p = 0; p < l->count; p++) {
        const u8 *page = l->pages + (size_t)p * C1_PAGE_BYTES;

        if (hm_rd16(page + 2) == 1 && hm_rd32(page + 4) == eid) {
            return page;
        }
    }
    return NULL;
}

void c1_model_scale(const C1Entry *model, s32 scale[3]) {
    scale[0] = (s32)hm_rd32(model->item[0] + 4);
    scale[1] = (s32)hm_rd32(model->item[0] + 8);
    scale[2] = (s32)hm_rd32(model->item[0] + 12);
}

s32 c1_frame_verts(const u8 *f, s32 size, const s32 scale[3], V3 *out, V3 *normals, s32 max) {
    s32 n = (s32)hm_rd32(f), k;
    s32 off[3];

    if (n <= 0 || n > max || 56 + n * 6 > size) {
        return 0;
    }
    off[0] = (s32)hm_rd32(f + 8);
    off[1] = (s32)hm_rd32(f + 12);
    off[2] = (s32)hm_rd32(f + 16);
    for (k = 0; k < n; k++) {
        const u8 *v = f + 56 + k * 6;

        out[k].x = (float)(v[0] + off[0] - 128) * scale[0];
        out[k].y = -(float)(v[1] + off[1] - 128) * scale[1];
        out[k].z = -(float)(v[2] + off[2] - 128) * scale[2];
        if (normals != NULL) {
            /* the normal's x is the other way from the position's (CrashEdit's lighting) */
            normals[k].x = -(float)(s8)v[3];
            normals[k].y = -(float)(s8)v[4];
            normals[k].z = -(float)(s8)v[5];
        }
    }
    return n;
}

u8 *c1_sound(const C1Level *l, const char *name, s32 *size) {
    C1Entry e;
    u8 *copy;

    if (!c1_entry_find(l, name, &e) || e.type != 12 || e.count < 1 || e.size[0] < 16 || (copy = malloc((size_t)e.size[0])) == NULL) {
        return NULL;
    }
    memcpy(copy, e.item[0], (size_t)e.size[0]);
    *size = e.size[0];
    return copy;
}

/* An object's item: a run of sprite lists, each a head word (count << 16 | kind, kind 2 the sprites) then the texture page's eid and
 * 8 bytes a sprite: its colour word (the CLUT's x in bits 24-27) and where in the page it is (uvi its size, cm 0 for 4-bit). */
s32 c1_sprite64(const C1Level *l, const char *object, s32 item, u32 out[64 * 64]) {
    C1Entry obj;
    const u8 *it;
    s32 at, f, x, y;

    if (!c1_entry_find(l, object, &obj) || obj.type != 11 || obj.count <= item) {
        return 0;
    }
    it = obj.item[item];
    for (at = 4; at + 12 <= obj.size[item]; at += 4) {
        const u8 *page = c1_texture_page(l, hm_rd32(it + at));
        u32 head = hm_rd32(it + at - 4);

        if (page == NULL || (head & 0xFFFF) != 2) {
            continue;
        }
        for (f = 0; f < (s32)(head >> 16) && at + 12 + f * 8 <= obj.size[item]; f++) {
            u32 col = hm_rd32(it + at + 4 + f * 8), ti = hm_rd32(it + at + 8 + f * 8);
            s32 uvi = (s32)(ti >> 22 & 0x3FF), cm = (s32)(ti >> 20 & 3), seg = (s32)(ti >> 18 & 3), xoffu = (s32)(ti >> 13 & 0x1F);
            s32 cy = (s32)(ti >> 6 & 0x7F), y0 = (s32)(ti & 0x1F) * 4, x0 = 256 * seg + 8 * xoffu, cx = (s32)(col >> 24 & 0xF) * 16;

            if (cm != 0 || 4 << (uvi % 5) != 64 || 4 << (uvi / 5 % 5) != 64 || y0 + 64 > 128) {
                continue;
            }
            for (y = 0; y < 64; y++) {
                for (x = 0; x < 64; x++) {
                    s32 u = x0 + x, i = page[(y0 + y) * 512 + u / 2] >> (u & 1) * 4 & 15;
                    u16 c = hm_rd16(page + cy * 512 + (cx + i) * 2);

                    out[y * 64 + x] = c == 0 ? 0 : 0xFF000000u | (u32)((c & 0x1F) * 255 / 31) | (u32)((c >> 5 & 0x1F) * 255 / 31) << 8
                                                   | (u32)((c >> 10 & 0x1F) * 255 / 31) << 16;
                }
            }
            return 1;
        }
    }
    return 0;
}

/* --- Crash Bandicoot's models into made faces --- */

/* A texture of the model's: a rect of a page, in 4 or 8 bits, and its CLUT's place */
typedef struct {
    u32 page_eid, info;
    s32 clutx;
    s32 x, y, w, h, bpp;
} C1Texture;

s32 c1_model_faces(const C1Level *l, const C1Entry *model, const V3 *normals, s32 nvert, MadeFace *faces, s32 max, s32 *nfaces,
                   MadeImage *images, s32 *nimages) {
    static C1Texture texs[MADE_MAX_IMAGES];
    const u8 *info = model->item[0], *polys = model->item[1];
    const V3 light = { -0.35f, -0.75f, -0.55f };
    float ll = sqrtf(light.x * light.x + light.y * light.y + light.z * light.z);
    s32 ntri = (s32)hm_rd32(info), nslot = (s32)hm_rd32(info + 16), ntex = 0, i, j, k;

    *nfaces = *nimages = 0;
    if (model->count < 2 || ntri <= 0 || ntri > max || model->size[1] < ntri * 8 || model->size[0] < 20 + nslot * 4) {
        return 0;
    }
    for (i = 0; i < ntri; i++) {
        const u8 *p = polys + i * 8, *s;
        MadeFace *t = &faces[i];
        u16 w = hm_rd16(p + 6);
        s32 slot = w & 0x7FFF;
        u8 rgb[3];

        memset(t, 0, sizeof(*t));
        t->v[0] = hm_rd16(p) / 6;
        t->v[1] = hm_rd16(p + 2) / 6;
        t->v[2] = hm_rd16(p + 4) / 6;
        t->image = -1;
        if (t->v[0] >= nvert || t->v[1] >= nvert || t->v[2] >= nvert || slot >= nslot) {
            t->v[0] = t->v[1] = t->v[2] = 0;
            slot = 0;
        }
        s = info + 20 + slot * 4;
        rgb[0] = s[0];
        rgb[1] = s[1];
        rgb[2] = s[2];
        if ((s[3] & 0x80) && 20 + (slot + 3) * 4 <= model->size[0]) {
            u32 page = hm_rd32(s + 4), tinfo = hm_rd32(s + 8);
            s32 uvi = (s32)(tinfo >> 22 & 0x3FF), cm = (s32)(tinfo >> 20 & 3), seg = (s32)(tinfo >> 18 & 3);
            s32 xoffu = (s32)(tinfo >> 13 & 0x1F), yoffu = (s32)(tinfo & 0x1F), wind = uvi / 25 & 31;
            s32 tw = 4 << (uvi % 5), th = 4 << (uvi / 5 % 5), xoff, yoff;

            if (cm <= 1 && c1_texture_page(l, page) != NULL) {
                xoff = (64 << (2 - cm)) * seg + (2 << (2 - cm)) * xoffu;
                yoff = yoffu * 4;
                for (j = 0; j < ntex; j++) {
                    if (texs[j].page_eid == page && texs[j].x == xoff && texs[j].y == yoff && texs[j].w == tw && texs[j].h == th
                        && texs[j].bpp == (cm == 0 ? 4 : 8) && texs[j].clutx == (s[3] & 0xF) && (texs[j].info >> 6 & 0x7F) == (tinfo >> 6 & 0x7F)) {
                        break;
                    }
                }
                if (j == ntex && ntex < MADE_MAX_IMAGES) {
                    texs[ntex].page_eid = page;
                    texs[ntex].info = tinfo;
                    texs[ntex].clutx = s[3] & 0xF;
                    texs[ntex].x = xoff;
                    texs[ntex].y = yoff;
                    texs[ntex].w = tw;
                    texs[ntex].h = th;
                    texs[ntex].bpp = cm == 0 ? 4 : 8;
                    ntex++;
                }
                if (j < ntex) {
                    t->image = j;
                    /* corner a has U1 V1, b U2 V2, c U3 V3 */
                    t->uv[0][0] = (u8)(tw * (0x30FF0C >> wind & 1) > 255 ? 255 : tw * (0x30FF0C >> wind & 1));
                    t->uv[1][0] = (u8)(tw * (0x8799E1 >> wind & 1) > 255 ? 255 : tw * (0x8799E1 >> wind & 1));
                    t->uv[2][0] = (u8)(tw * (0x4B66D2 >> wind & 1) > 255 ? 255 : tw * (0x4B66D2 >> wind & 1));
                    t->uv[0][1] = (u8)(th * (0xF3CC30 >> wind & 1) > 255 ? 255 : th * (0xF3CC30 >> wind & 1));
                    t->uv[1][1] = (u8)(th * (0x9E7186 >> wind & 1) > 255 ? 255 : th * (0x9E7186 >> wind & 1));
                    t->uv[2][1] = (u8)(th * (0x6DB249 >> wind & 1) > 255 ? 255 : th * (0x6DB249 >> wind & 1));
                }
            }
        }
        /* each corner's colour, lit by the light as it stands: the game's own models have their light in their colours */
        for (j = 0; j < 3; j++) {
            const V3 *nn = &normals[t->v[j]];
            float len = sqrtf(nn->x * nn->x + nn->y * nn->y + nn->z * nn->z), lit = 0;

            if (len > 0) {
                lit = (nn->x * light.x + nn->y * light.y + nn->z * light.z) / (len * ll);
            }
            lit = 0.62f + 0.55f * (lit > 0 ? lit : 0);
            for (k = 0; k < 3; k++) {
                float v = rgb[k] * lit;

                t->rgb[0][j][k] = (u8)(v > 255 ? 255 : v);
            }
        }
        memcpy(t->rgb[1], t->rgb[0], sizeof(t->rgb[0]));
    }
    /* the images, out of their pages: each a palette and its rows */
    for (j = 0; j < ntex; j++) {
        const u8 *page = c1_texture_page(l, texs[j].page_eid);
        MadeImage *im = &images[j];
        s32 cy = (s32)(texs[j].info >> 6 & 0x7F), cx = texs[j].clutx * 16, row, y;

        memset(im, 0, sizeof(*im));
        im->bpp8 = texs[j].bpp == 8;
        im->units = im->bpp8 ? texs[j].w / 2 : texs[j].w / 4;
        im->h = texs[j].h;
        im->colours = im->bpp8 ? 256 : 16;
        for (k = 0; k < im->colours; k++) {
            im->clut[k] = cx + k < 256 ? hm_rd16(page + cy * 512 + (cx + k) * 2) : 0;
        }
        row = im->units * 2;
        if ((im->pixels = calloc((size_t)row * (size_t)im->h, 1)) == NULL) {
            made_images_free(images, j);
            return 0;
        }
        for (y = 0; y < im->h; y++) {
            s32 at = (texs[j].y + y) * 512 + (im->bpp8 ? texs[j].x : texs[j].x / 2);

            if (at + row <= C1_PAGE_BYTES) {
                memcpy(im->pixels + (size_t)y * row, page + at, (size_t)row);
            }
        }
    }
    *nfaces = ntri;
    *nimages = ntex;
    return 1;
}

void made_images_free(MadeImage *images, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        free(images[i].pixels);
        images[i].pixels = NULL;
    }
}

/* --- writing a made model --- */

#define INDEX_LIMIT 16384 /* an index list's entries are a vertex << 2 in 16 bits */

static void put_box(Bytes *o, const V3 *v, s32 n) {
    float lo[3] = { 1e9f, 1e9f, 1e9f }, hi[3] = { -1e9f, -1e9f, -1e9f }, r = 0;
    u8 box[0x14];
    s32 i, j;

    for (i = 0; i < n; i++) {
        float p[3];

        p[0] = v[i].x;
        p[1] = v[i].y;
        p[2] = v[i].z;
        for (j = 0; j < 3; j++) {
            lo[j] = p[j] < lo[j] ? p[j] : lo[j];
            hi[j] = p[j] > hi[j] ? p[j] : hi[j];
        }
    }
    for (j = 0; j < 3; j++) {
        float h = (hi[j] - lo[j]) / 2;

        r += h * h;
    }
    /* as the game's own: across from least to most, but down from the bottom to the top */
    hm_wr16(box + 0, (u32)(s16)lo[0]);
    hm_wr16(box + 2, (u32)(s16)hi[1]);
    hm_wr16(box + 4, (u32)(s16)lo[2]);
    hm_wr16(box + 6, (u32)(s16)hi[0]);
    hm_wr16(box + 8, (u32)(s16)lo[1]);
    hm_wr16(box + 10, (u32)(s16)hi[2]);
    hm_wr16(box + 12, (u32)(s16)((lo[0] + hi[0]) / 2));
    hm_wr16(box + 14, (u32)(s16)((lo[1] + hi[1]) / 2));
    hm_wr16(box + 16, (u32)(s16)((lo[2] + hi[2]) / 2));
    hm_wr16(box + 18, (u32)(s16)sqrtf(r));
    hm_put(o, box, sizeof(box));
}

static void write_texture_set(Bytes *tex, const MadeImage *images, s32 nimages) {
    size_t first_image;
    s32 j, k;

    hm_put(tex, NULL, 0x20);
    hm_wr32(tex->b, 8);
    for (j = 0; j < nimages; j++) {
        u8 head[4], c[2];

        hm_wr32(head, (u32)images[j].colours);
        hm_put(tex, head, 4);
        for (k = 0; k < images[j].colours; k++) {
            hm_wr16(c, images[j].clut[k]);
            hm_put(tex, c, 2);
        }
    }
    first_image = tex->n;
    for (j = 0; j < nimages; j++) {
        s32 row = images[j].units * 2;
        u8 head[20];

        memset(head, 0, sizeof(head));
        hm_wr16(head, (u32)images[j].units);
        hm_wr16(head + 2, (u32)images[j].h);
        hm_wr16(head + 6, (u32)((images[j].h & 0xFF) << 8 | (row & 0xFF)));
        hm_wr16(head + 12, (u32)(j << 1 | images[j].bpp8));
        hm_wr32(head + 16, 1);
        hm_put(tex, head, sizeof(head));
        hm_put(tex, images[j].pixels, (size_t)row * (size_t)images[j].h);
    }
    if (nimages == 0) {
        /* none: one clear image, as a set has some */
        u8 head[20], clut[36];

        memset(clut, 0, sizeof(clut));
        hm_wr32(clut, 16);
        hm_put(tex, clut, sizeof(clut));
        first_image = tex->n;
        memset(head, 0, sizeof(head));
        hm_wr16(head, 1);
        hm_wr16(head + 2, 4);
        hm_wr16(head + 6, 4 << 8 | 2);
        hm_wr32(head + 16, 1);
        hm_put(tex, head, sizeof(head));
        hm_put(tex, NULL, 8);
        nimages = 1;
    }
    hm_wr16(tex->b + 8, (u32)nimages);
    hm_wr16(tex->b + 10, (u32)nimages);
    hm_wr32(tex->b + 0x0C, (u32)(first_image - 0x0C));
    hm_wr32(tex->b + 0x10, 0x10);
    hm_align(tex, 4);
    hm_wr32(tex->b + 4, (u32)tex->n);
}

/* An animation's keyframes, every `step` frames (a multiple of its own at_least) and its last: the frames between are blended from the
 * one before to the one after. */
static s32 key_step(s32 frames, s32 nvert, s32 at_least) {
    s32 unit = at_least > 1 ? at_least : 1, step = unit;

    while (((frames - 1) / step + 2) * nvert > INDEX_LIMIT && step < frames) {
        step += unit;
    }
    return step;
}

s32 made_write(const MadeModel *m, u8 **tex_out, s32 *tex_size, u8 **mdl_out, s32 *mdl_size) {
    Bytes tex = { 0 }, mdl = { 0 };
    V3 *fv = malloc(sizeof(V3) * (size_t)(m->nvert > 0 ? m->nvert : 1)), *corners = malloc(sizeof(V3) * (size_t)m->nfaces * 3);
    size_t strips, vblock, uvidx, texs, colidx[2], end, colours, uvs, zeros, count0, entries, kept;
    s32 i, j, k, n, mesh;

    *tex_out = *mdl_out = NULL;
    if (fv == NULL || corners == NULL || m->nfaces <= 0 || m->nvert <= 0 || m->nvert > INDEX_LIMIT / 2 || m->nfaces * 3 > 4096) {
        free(fv);
        free(corners);
        return 0;
    }
    for (j = 0; j < m->nanims; j++) {
        if (m->anims[j].frames > MADE_MAX_FRAMES) {
            printf("bonus: a made model's %s has %d frames, more than a frame's number can name (%d)\n", m->anims[j].name,
                   m->anims[j].frames, MADE_MAX_FRAMES);
            free(fv);
            free(corners);
            return 0;
        }
    }
    write_texture_set(&tex, m->images, m->nimages);

    /* the meshes: a strip a face, all of them sharing all but their colours */
    hm_put(&mdl, NULL, 0x58 + 0x34 * (size_t)m->meshes);
    strips = mdl.n;
    for (i = 0; i < m->nfaces; i++) {
        u8 s[2];

        hm_wr16(s, (u32)(1 << 8 | (m->faces[i].image < 0)));
        hm_put(&mdl, s, 2);
    }
    hm_put(&mdl, "\xFF\xFF", 2);
    hm_align(&mdl, 4);
    vblock = mdl.n;
    m->frame(m->ctx, m->rest_anim >= 0 && m->rest_anim < m->nanims ? m->rest_anim : 0, 0, fv);
    for (i = 0; i < m->nfaces; i++) {
        for (k = 0; k < 3; k++) {
            corners[i * 3 + k] = fv[m->faces[i].v[k]];
        }
    }
    put_box(&mdl, corners, m->nfaces * 3);
    for (i = 0; i < m->nfaces * 3; i++) {
        u8 v[8];

        memset(v, 0, sizeof(v));
        hm_wr16(v, (u32)(s16)corners[i].x);
        hm_wr16(v + 2, (u32)(s16)corners[i].y);
        hm_wr16(v + 4, (u32)(s16)corners[i].z);
        hm_put(&mdl, v, sizeof(v));
    }
    uvidx = mdl.n;
    for (i = 0, n = 0; i < m->nfaces; i++) {
        u8 s[2];

        hm_wr16(s, (u32)(m->faces[i].image >= 0 ? n : 0));
        n += m->faces[i].image >= 0 ? 3 : 0;
        hm_put(&mdl, s, 2);
    }
    hm_align(&mdl, 4);
    texs = mdl.n;
    for (i = 0; i < m->nfaces; i++) {
        u8 s[2];

        hm_wr16(s, (u32)(m->faces[i].image >= 0 ? m->faces[i].image : 0));
        hm_put(&mdl, s, 2);
    }
    hm_align(&mdl, 4);
    for (mesh = 0; mesh < m->meshes; mesh++) {
        colidx[mesh] = mdl.n;
        for (i = 0; i < m->nfaces; i++) {
            u8 s[2];

            hm_wr16(s, (u32)((mesh * m->nfaces + i) * 3 | m->faces[i].blend));
            hm_put(&mdl, s, 2);
        }
        hm_align(&mdl, 4);
    }
    end = mdl.n;
    colours = mdl.n;
    for (mesh = 0; mesh < m->meshes; mesh++) {
        for (i = 0; i < m->nfaces; i++) {
            for (j = 0; j < 3; j++) {
                u8 c[4];

                c[0] = m->faces[i].rgb[mesh][j][0];
                c[1] = m->faces[i].rgb[mesh][j][1];
                c[2] = m->faces[i].rgb[mesh][j][2];
                c[3] = 0;
                hm_put(&mdl, c, 4);
            }
        }
    }
    uvs = mdl.n;
    for (i = 0; i < m->nfaces; i++) {
        for (j = 0; m->faces[i].image >= 0 && j < 3; j++) {
            u8 s[2];

            hm_wr16(s, (u32)(m->faces[i].uv[j][0] | m->faces[i].uv[j][1] << 8));
            hm_put(&mdl, s, 2);
        }
    }
    hm_put(&mdl, NULL, 4);
    hm_align(&mdl, 4);
    zeros = hm_put(&mdl, NULL, 0x18);
    count0 = hm_put(&mdl, NULL, 4);
    entries = hm_put(&mdl, NULL, (size_t)m->nanims * 0x18);
    kept = mdl.n;

    /* the header */
    hm_wr32(mdl.b, 0x0C160029);
    hm_self(&mdl, 0x08, zeros);
    hm_self(&mdl, 0x10, zeros);
    hm_self(&mdl, 0x18, count0);
    hm_self(&mdl, 0x1C, count0);
    hm_self(&mdl, 0x20, colours);
    hm_self(&mdl, 0x24, uvs);
    hm_self(&mdl, 0x28, zeros);
    hm_self(&mdl, 0x2C, zeros);
    hm_self(&mdl, 0x3C, zeros);
    hm_wr32(mdl.b + 0x40, (u32)m->nanims);
    hm_self(&mdl, 0x44, entries);
    hm_self(&mdl, 0x4C, count0);
    hm_wr32(mdl.b + 0x50, (u32)kept);
    hm_wr32(mdl.b + 0x54, (u32)m->meshes);
    for (mesh = 0; mesh < m->meshes; mesh++) {
        size_t h = 0x58 + 0x34 * (size_t)mesh;

        hm_wr16(mdl.b + h + 0x08, (u32)m->nfaces);
        hm_wr16(mdl.b + h + 0x0A, 4);
        hm_self(&mdl, h + 0x10, vblock);
        hm_self(&mdl, h + 0x14, strips);
        hm_self(&mdl, h + 0x18, uvidx);
        hm_self(&mdl, h + 0x1C, texs);
        hm_self(&mdl, h + 0x20, colidx[mesh]);
        hm_self(&mdl, h + 0x24, end);
    }
    hm_wr32(tex.b + 0x14, (u32)kept);

    /* the animations, each in a block of its own from a 2 KB sector on, with room before it for the heap's header as it's split off:
     * its frames' records, an index list a keyframe, and the keyframes' vertices */
    for (j = 0; j < m->nanims; j++) {
        s32 frames = m->anims[j].frames > 0 ? m->anims[j].frames : 1, step = key_step(frames, m->nvert, m->anims[j].step), keys = 0, f, q;
        s32 key_frame[256];
        size_t start, list[256], pool, e = entries + (size_t)j * 0x18;

        for (f = 0; f < frames - 1 && keys < 255; f += step) {
            key_frame[keys++] = f;
        }
        key_frame[keys++] = frames - 1;
        hm_put(&mdl, NULL, 0x20);
        hm_align(&mdl, 2048);
        start = hm_put(&mdl, NULL, 4 + (size_t)frames * 16);
        for (q = 0; q < keys; q++) {
            m->frame(m->ctx, j, key_frame[q], fv);
            for (i = 0; i < m->nfaces; i++) {
                for (k = 0; k < 3; k++) {
                    corners[i * 3 + k] = fv[m->faces[i].v[k]];
                }
            }
            list[q] = mdl.n;
            put_box(&mdl, corners, m->nfaces * 3);
            for (i = 0; i < m->nfaces; i++) {
                for (k = 0; k < 3; k++) {
                    u8 s[2];

                    hm_wr16(s, (u32)((q * m->nvert + m->faces[i].v[k]) << 2));
                    hm_put(&mdl, s, 2);
                }
            }
            hm_align(&mdl, 4);
        }
        for (f = 0, q = 0; f < frames; f++) {
            size_t rec = start + 4 + (size_t)f * 16;

            while (q + 1 < keys && key_frame[q + 1] <= f) {
                q++;
            }
            hm_self(&mdl, rec, list[q]);
            if (f != key_frame[q] && q + 1 < keys) {
                hm_self(&mdl, rec + 4, list[q + 1]);
                hm_wr32(mdl.b + rec + 8, (u32)((f - key_frame[q]) * 0x1000 / (key_frame[q + 1] - key_frame[q])));
            }
        }
        pool = mdl.n;
        hm_self(&mdl, start, pool);
        for (q = 0; q < keys; q++) {
            m->frame(m->ctx, j, key_frame[q], fv);
            for (k = 0; k < m->nvert; k++) {
                u8 v[6];

                hm_wr16(v, (u32)(s16)fv[k].x);
                hm_wr16(v + 2, (u32)(s16)fv[k].y);
                hm_wr16(v + 4, (u32)(s16)fv[k].z);
                hm_put(&mdl, v, sizeof(v));
            }
        }
        hm_align(&mdl, 4);
        hm_wr32(mdl.b + e, (u32)start);
        hm_wr32(mdl.b + e + 4, (u32)mdl.n);
        hm_wr32(mdl.b + e + 8, (u32)frames);
        hm_self(&mdl, e + 0x0C, 0x58 + 0x34 * (size_t)(m->anims[j].mesh < m->meshes ? m->anims[j].mesh : 0));
        hm_wr32(mdl.b + e + 0x10, (u32)string_hash((u8 *)m->anims[j].name));
    }
    free(fv);
    free(corners);
    if (tex.b == NULL || mdl.b == NULL) {
        free(tex.b);
        free(mdl.b);
        return 0;
    }
    *tex_out = tex.b;
    *tex_size = (s32)tex.n;
    *mdl_out = mdl.b;
    *mdl_size = (s32)mdl.n;
    return 1;
}

/* --- Crash Bash's own models, as the archive has them --- */

static const FileEntry *file_anims(const ModelFile *m) {
    return (const FileEntry *)((const u8 *)&m->meshAnims + m->meshAnims);
}

static s32 mesh_corners(const MeshHeader *h) {
    const u16 *s = (const u16 *)((const u8 *)&h->runs + h->runs);
    s32 n = 0;

    for (; *s != 0xFFFF; s++) {
        n += (*s >> 8) + 2;
    }
    return n;
}

/* Frame f of animation n: each of its mesh's corners, blended if it's between two keyframes. The count. */
static s32 file_frame(const u8 *file, s32 n, s32 f, V3 *out, s32 max) {
    const ModelFile *m = (const ModelFile *)file;
    const FileEntry *e;
    const MeshHeader *mesh;
    const s32 *anim, *fr;
    const s16 *pool;
    const u16 *a, *b;
    s32 k, count, blend;

    if (n < 0 || n >= m->nMeshAnims || f < 0 || f >= file_anims(m)[n].frames) {
        return 0;
    }
    e = &file_anims(m)[n];
    anim = (const s32 *)(file + e->start);
    fr = (const s32 *)((const u8 *)anim + 4 + f * 16);
    mesh = (const MeshHeader *)((const u8 *)&e->mesh + e->mesh);
    pool = (const s16 *)(anim[0] != 0 ? (const u8 *)anim + anim[0] : (const u8 *)&m->verts + m->verts);
    a = (const u16 *)((const u8 *)fr + fr[0] + 0x14);
    blend = fr[2];
    b = blend != 0 ? (const u16 *)((const u8 *)&fr[1] + fr[1] + 0x14) : a;
    count = mesh_corners(mesh);
    for (k = 0; k < count && k < max; k++) {
        const s16 *pa = pool + (a[k] >> 2) * 3, *pb = pool + (b[k] >> 2) * 3;

        out[k].x = (float)(pa[0] + ((pb[0] - pa[0]) * blend >> 12));
        out[k].y = (float)(pa[1] + ((pb[1] - pa[1]) * blend >> 12));
        out[k].z = (float)(pa[2] + ((pb[2] - pa[2]) * blend >> 12));
    }
    return count;
}

static s32 file_anim_named(const u8 *file, const char *name) {
    const ModelFile *m = (const ModelFile *)file;
    s32 key = string_hash((u8 *)name), i;

    for (i = 0; i < m->nMeshAnims; i++) {
        if (file_anims(m)[i].hash == key) {
            return i;
        }
    }
    return -1;
}

/* A texture set's images and CLUTs (docs/ASSETS.md, Texture sets) */
typedef struct {
    s32 nimg, nclut;
    const u8 *img[64], *clut[64];
    s32 clut_n[64];
} TexSet;

static s32 tex_set(const u8 *t, s32 size, TexSet *s) {
    s32 i, at;

    memset(s, 0, sizeof(*s));
    if (size < 0x20 || hm_rd32(t) != 8) {
        return 0;
    }
    s->nimg = hm_rd16(t + 8);
    s->nclut = hm_rd16(t + 10);
    if (s->nimg > 64 || s->nclut > 64) {
        return 0;
    }
    for (i = 0, at = 0x10 + (s32)hm_rd32(t + 0x10); i < s->nclut; i++) {
        if (at + 4 > size) {
            return 0;
        }
        s->clut_n[i] = (s32)hm_rd32(t + at);
        s->clut[i] = t + at + 4;
        at += 4 + s->clut_n[i] * 2;
    }
    for (i = 0, at = 0x0C + (s32)hm_rd32(t + 0x0C); i < s->nimg; i++) {
        if (at + 20 > size) {
            return 0;
        }
        s->img[i] = t + at;
        at += 20 + hm_rd16(t + at) * hm_rd16(t + at + 2) * 2;
    }
    return 1;
}

/* The made image for a face whose texture word is t (a mesh's texs: its image, or with bit 15 the set's last image with CLUT t & 0x1FF),
 * into images (n of max made so far), made once: made_img and made_clut keep which each is. -1 if there's none. */
static s32 face_image(MadeImage *images, s32 *n, s32 max, const TexSet *ts, s32 t, s32 *made_img, s32 *made_clut) {
    MadeImage *im;
    s32 i, c, w, h, k, img = (t & 0x8000) ? ts->nimg - 1 : t & 0x1FF, clut = (t & 0x8000) ? t & 0x1FF : -1;

    for (i = 0; i < *n; i++) {
        if (made_img[i] == img && made_clut[i] == clut) {
            return i;
        }
    }
    if (*n == max || img < 0 || img >= ts->nimg) {
        return -1;
    }
    w = hm_rd16(ts->img[img]);
    h = hm_rd16(ts->img[img] + 2);
    c = clut >= 0 ? clut : hm_rd16(ts->img[img] + 12) >> 1;
    if (c < 0 || c >= ts->nclut) {
        return -1;
    }
    im = &images[*n];
    memset(im, 0, sizeof(*im));
    im->units = w;
    im->h = h;
    im->bpp8 = hm_rd16(ts->img[img] + 12) & 1;
    im->colours = ts->clut_n[c] > 256 ? 256 : ts->clut_n[c];
    for (k = 0; k < im->colours; k++) {
        im->clut[k] = hm_rd16(ts->clut[c] + k * 2);
    }
    if ((im->pixels = malloc((size_t)w * h * 2)) == NULL) {
        return -1;
    }
    memcpy(im->pixels, ts->img[img] + 20, (size_t)w * h * 2);
    made_img[*n] = img;
    made_clut[*n] = clut;
    return (*n)++;
}

/* --- part of a model --- */

void hm_part_free(GamePart *p) {
    made_images_free(p->images, p->nimages);
    free(p->mdl);
    free(p->rest);
    free(p->in_part);
    free(p->faces);
    free(p->corner_of);
    memset(p, 0, sizeof(*p));
}

s32 hm_game_part(s32 bank, s32 (*keep)(void *ctx, const V3 *rest, s32 corners, s32 textured), void *ctx, GamePart *out) {
    const obj_dbase *db = &HeroModelBank[bank];
    const ModelFile *m;
    const MeshHeader *h;
    const u16 *runs, *uv_idx, *texs, *cols, *uvs;
    const u8 *colours, *vb;
    u8 *tex = NULL;
    TexSet ts;
    s32 mdl_size = 0, tex_size = 0, made_img[PART_MAX_IMAGES], made_clut[PART_MAX_IMAGES];
    s32 k, s, face, corner, left, t = 0, faces = 0, *vert_of = NULL;

    memset(out, 0, sizeof(*out));
    out->mdl = archive_own_file((s32)(db->file - DatIndex), &mdl_size);
    if (out->mdl == NULL || ((const ModelFile *)out->mdl)->nMeshes < 1) {
        goto fail;
    }
    m = (const ModelFile *)out->mdl;
    h = &m->meshes[0];
    runs = (const u16 *)((const u8 *)&h->runs + h->runs);
    uv_idx = (const u16 *)((const u8 *)&h->uvIdx + h->uvIdx);
    cols = (const u16 *)((const u8 *)&h->colors + h->colors);
    uvs = (const u16 *)((const u8 *)&m->uvs + m->uvs);
    colours = (const u8 *)&m->colors + m->colors;
    vb = (const u8 *)&h->verts + h->verts + 0x14;
    out->corners = mesh_corners(h);
    for (s = 0; runs[s] != 0xFFFF; s++) {
        faces += runs[s] >> 8;
    }
    out->rest = malloc(sizeof(V3) * (size_t)out->corners);
    out->in_part = calloc((size_t)out->corners, 1);
    out->faces = malloc(sizeof(MadeFace) * (size_t)(faces > 0 ? faces : 1));
    out->corner_of = malloc(sizeof(s16) * (size_t)out->corners);
    vert_of = malloc(sizeof(s32) * (size_t)out->corners);
    if (out->rest == NULL || out->in_part == NULL || out->faces == NULL || out->corner_of == NULL || vert_of == NULL) {
        goto fail;
    }
    for (k = 0; k < out->corners; k++) {
        out->rest[k].x = (s16)hm_rd16(vb + k * 8);
        out->rest[k].y = (s16)hm_rd16(vb + k * 8 + 2);
        out->rest[k].z = (s16)hm_rd16(vb + k * 8 + 4);
        vert_of[k] = -1;
    }
    /* its textures, if any face of the part has one */
    if (db->textureFile != NULL && (tex = archive_own_file((s32)(db->textureFile - DatIndex), &tex_size)) != NULL
        && !tex_set(tex, tex_size, &ts)) {
        free(tex);
        tex = NULL;
    }
    texs = (const u16 *)((const u8 *)&h->texs + h->texs);
    left = 0;
    for (s = 0, face = 0, corner = 0; runs[s] != 0xFFFF; corner += (runs[s] >> 8) + 2, s++) {
        s32 n = runs[s] >> 8, flip0 = (runs[s] & 8) != 0, textured = !(runs[s] & 1), take, j;

        take = corner + n + 2 <= out->corners && keep(ctx, out->rest + corner, n + 2, textured);
        for (j = 0; j < n; j++, face++) {
            /* a strip's first face is drawn a, c, b, so one drawn a, b, c in its own strip has its last two the other way */
            s32 order[3], q;
            MadeFace *mf;

            if (--left < 0) {
                t = texs[0];
                texs++;
                left = (t >> 9) & 0x3F;
            }
            if (!take) {
                continue;
            }
            order[0] = 0;
            order[1] = j % 2 == flip0 ? 1 : 2;
            order[2] = j % 2 == flip0 ? 2 : 1;
            mf = &out->faces[out->nfaces++];
            memset(mf, 0, sizeof(*mf));
            mf->image = textured && tex != NULL ? face_image(out->images, &out->nimages, PART_MAX_IMAGES, &ts, t, made_img, made_clut) : -1;
            mf->blend = (u16)(cols[face] & 0xE000);
            for (q = 0; q < 3; q++) {
                s32 at = corner + j + order[q], c = (cols[face] & 0x1FFF) + order[q], w;

                if (vert_of[at] < 0) {
                    out->corner_of[out->nvert] = (s16)at;
                    out->in_part[at] = 1;
                    vert_of[at] = out->nvert++;
                }
                mf->v[q] = vert_of[at];
                w = uvs[uv_idx[face] + order[q]];
                mf->uv[q][0] = (u8)(w & 0xFF);
                mf->uv[q][1] = (u8)(w >> 8);
                memcpy(mf->rgb[0][q], colours + c * 4, 3);
                memcpy(mf->rgb[1][q], colours + c * 4, 3);
            }
        }
    }
    for (k = 0; k < out->nimages; k++) {
        out->image_of[k] = (s16)made_img[k];
    }
    free(vert_of);
    free(tex);
    if (out->nfaces == 0) {
        hm_part_free(out);
        return 0;
    }
    return 1;
fail:
    free(vert_of);
    free(tex);
    hm_part_free(out);
    return 0;
}

s32 hm_model_anim(const GamePart *p, const char *name, s32 *frames) {
    s32 a = p->mdl != NULL ? file_anim_named(p->mdl, name) : -1;

    if (frames != NULL) {
        *frames = a >= 0 ? file_anims((const ModelFile *)p->mdl)[a].frames : 0;
    }
    return a;
}

s32 hm_bank_frames(s32 bank, const char *name) {
    s32 size = 0, a, n = 0;
    u8 *file = archive_own_file((s32)(HeroModelBank[bank].file - DatIndex), &size);

    if (file != NULL && (a = file_anim_named(file, name)) >= 0) {
        n = file_anims((const ModelFile *)file)[a].frames;
    }
    free(file);
    return n;
}

s32 hm_model_at(const GamePart *p, s32 anim, float t, V3 *corners) {
    static V3 *next;
    static s32 next_n;
    s32 n, f0, f1, k;
    float u;

    if (p->mdl == NULL || anim < 0 || anim >= ((const ModelFile *)p->mdl)->nMeshAnims) {
        return 0;
    }
    n = file_anims((const ModelFile *)p->mdl)[anim].frames;
    t = t < 0 ? 0 : t > n - 1 ? (float)(n - 1) : t;
    f0 = (s32)floorf(t);
    f1 = f0 + 1 < n ? f0 + 1 : f0;
    u = t - f0;
    if (file_frame(p->mdl, anim, f0, corners, p->corners) != p->corners) {
        return 0;
    }
    if (u > 0 && f1 != f0) {
        if (next_n < p->corners) {
            free(next);
            next_n = (next = malloc(sizeof(V3) * (size_t)p->corners)) != NULL ? p->corners : 0;
        }
        if (next != NULL && file_frame(p->mdl, anim, f1, next, p->corners) == p->corners) {
            for (k = 0; k < p->corners; k++) {
                corners[k] = hm_mix(corners[k], next[k], u);
            }
        }
    }
    return 1;
}

/* --- painting ---
 * A colour less saturated than PAINT_GREY is a grey, and kept. Of the others, those within PAINT_NEAR of the hue painted from are
 * painted: that hue becomes the paint's own, and the others are PAINT_SPREAD as far from it as they were from that. */
#define PAINT_GREY 0.2f
#define PAINT_NEAR 65.0f
#define PAINT_SPREAD 0.5f

/* A colour's hue (degrees), saturation and value (as its largest part) */
static void colour_hsv(const float *c, float *h, float *s, float *v) {
    float mx = fmaxf(c[0], fmaxf(c[1], c[2])), d = mx - fminf(c[0], fminf(c[1], c[2]));

    *v = mx;
    *s = mx > 0 ? d / mx : 0;
    if (d <= 0) {
        *h = 0;
    } else if (mx == c[0]) {
        *h = 60 * fmodf((c[1] - c[2]) / d + 6, 6);
    } else if (mx == c[1]) {
        *h = 60 * ((c[2] - c[0]) / d + 2);
    } else {
        *h = 60 * ((c[0] - c[1]) / d + 4);
    }
}

/* One colour (any scale) of about hue `from` painted the hue, saturation and value of a paint */
static void paint_colour(float *c, float from, const float *paint) {
    float h, s, v, f, p, q, t;
    s32 i;

    colour_hsv(c, &h, &s, &v);
    h = fmodf(h - from + 540, 360) - 180; /* how far round from `from`, either way */
    if (s < PAINT_GREY || fabsf(h) > PAINT_NEAR) {
        return;
    }
    h = fmodf(paint[0] + h * PAINT_SPREAD + 720, 360) / 60;
    s *= paint[1];
    v *= paint[2];
    i = (s32)h % 6;
    f = h - floorf(h);
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    {
        const float rgb[6][3] = { { v, t, p }, { q, v, p }, { p, v, t }, { p, q, v }, { t, p, v }, { v, p, q } };

        c[0] = rgb[i][0];
        c[1] = rgb[i][1];
        c[2] = rgb[i][2];
    }
}

void hm_paint(MadeFace *faces, s32 nfaces, MadeImage *images, s32 nimages, float from, u32 paint) {
    s32 i, m, q, j, k;
    float c[3], hsv[3];

    c[0] = (float)(paint & 0xFF) / 255;
    c[1] = (float)(paint >> 8 & 0xFF) / 255;
    c[2] = (float)(paint >> 16 & 0xFF) / 255;
    colour_hsv(c, &hsv[0], &hsv[1], &hsv[2]);

    for (i = 0; i < nfaces; i++) {
        for (m = 0; m < 2; m++) {
            for (q = 0; q < 3; q++) {
                for (j = 0; j < 3; j++) {
                    c[j] = faces[i].rgb[m][q][j];
                }
                paint_colour(c, from, hsv);
                for (j = 0; j < 3; j++) {
                    faces[i].rgb[m][q][j] = (u8)(c[j] + 0.5f);
                }
            }
        }
    }
    /* their CLUTs' colours, 5 bits each, the top bit (semi-transparent) kept */
    for (i = 0; i < nimages; i++) {
        for (k = 0; k < images[i].colours; k++) {
            u16 w = images[i].clut[k];

            c[0] = (float)(w & 31);
            c[1] = (float)(w >> 5 & 31);
            c[2] = (float)(w >> 10 & 31);
            paint_colour(c, from, hsv);
            images[i].clut[k] = (u16)((w & 0x8000) | (s32)(c[0] + 0.5f) | (s32)(c[1] + 0.5f) << 5 | (s32)(c[2] + 0.5f) << 10);
        }
    }
}

/* --- the bear ---
 * Crash's polar model (DB_CharsPolar2Crash), as the archive has it: one mesh of Crash and his bear (and a second with the bear charred,
 * as the game draws it once a bomb's hit). The bear is its strips that are textured only with the bear's images (BEAR_IMAGES and the
 * last image with BEAR_CLUT) or not at all, inside its outline, and neither blue (his jeans) nor orange (his skin). */
#define BEAR_CORNERS 1024
#define BEAR_FACES 512
#define BEAR_IMAGES 2   /* images 0 and 1, its fur's */
#define BEAR_CLUT 12    /* and the set's last with this CLUT, its shading */
#define BEAR_HALF 110   /* its outline: this far either side, and below BEAR_TOP */
#define BEAR_TOP -215
#define HIPS_TOP -225   /* Crash's hips: his corners below this and within BEAR_HALF across; his head, above HEAD_BOTTOM */
#define HEAD_BOTTOM -400
#define BACK_BELOW -110 /* its back, which the hero moves with: its corners above this (not its legs), within BACK_HALF across, */
#define BACK_HALF 70    /* from BACK_FRONT to BACK_REAR along (not its head, which nods, nor its tail) */
#define BACK_FRONT -90
#define BACK_REAR 110
#define MAX_BEAR_IMAGES 8

typedef struct {
    s32 made;
    u8 *mdl;
    s32 corners;
    s32 nfaces, nimages, nvert;
    MadeFace faces[BEAR_FACES];
    MadeImage images[MAX_BEAR_IMAGES];
    s16 vert_of[BEAR_CORNERS], corner_of[BEAR_CORNERS];
    s32 nhips, nhead, nback;
    s16 hips[BEAR_CORNERS], head[BEAR_CORNERS], back[BEAR_CORNERS];
    V3 ref[BEAR_CORNERS]; /* his first breathing frame */
    float back_y, seat_z;
    s32 anim[RIDE_ANIMS];
} Bear;

static Bear TheBear;
static s32 BearTried;

static const char *const RideNames[RIDE_ANIMS] = { "FALL", "RUN", "BREATHE", "PUSH", "TAUNT", "LIGHT", "WIN" };

static s32 bear_load(void) {
    static V3 rest[BEAR_CORNERS];
    Bear *b = &TheBear;
    const obj_dbase *db = &HeroModelBank[DB_CharsPolar2Crash];
    const ModelFile *m;
    const MeshHeader *h1, *h2;
    const u16 *runs, *runs2, *uv_idx, *texs, *col1, *col2, *uvs;
    const u8 *colours, *vb;
    u8 *tex;
    TexSet ts;
    s32 mdl_size = 0, tex_size = 0, made_img[MAX_BEAR_IMAGES], made_clut[MAX_BEAR_IMAGES];
    s32 i, k, s, face, corner, left = 0, t = 0, back_n = 0, breathe;
    s8 strip_bear[256];
    s32 strip_of[BEAR_CORNERS];
    float zs = 0;

    if (BearTried) {
        return b->made;
    }
    BearTried = 1;
    memset(b, 0, sizeof(*b));
    b->mdl = archive_own_file((s32)(db->file - DatIndex), &mdl_size);
    tex = archive_own_file((s32)(db->textureFile - DatIndex), &tex_size);
    if (b->mdl == NULL || tex == NULL || !tex_set(tex, tex_size, &ts)) {
        goto fail;
    }
    m = (const ModelFile *)b->mdl;
    if (m->nMeshes < 2) {
        goto fail;
    }
    h1 = &m->meshes[0];
    h2 = &m->meshes[1];
    runs = (const u16 *)((const u8 *)&h1->runs + h1->runs);
    runs2 = (const u16 *)((const u8 *)&h2->runs + h2->runs);
    uv_idx = (const u16 *)((const u8 *)&h1->uvIdx + h1->uvIdx);
    texs = (const u16 *)((const u8 *)&h1->texs + h1->texs);
    col1 = (const u16 *)((const u8 *)&h1->colors + h1->colors);
    col2 = (const u16 *)((const u8 *)&h2->colors + h2->colors);
    uvs = (const u16 *)((const u8 *)&m->uvs + m->uvs);
    colours = (const u8 *)&m->colors + m->colors;
    vb = (const u8 *)&h1->verts + h1->verts + 0x14;
    b->corners = mesh_corners(h1);
    if (b->corners > BEAR_CORNERS || mesh_corners(h2) != b->corners) {
        goto fail;
    }
    for (k = 0; k < b->corners; k++) {
        rest[k].x = (s16)hm_rd16(vb + k * 8);
        rest[k].y = (s16)hm_rd16(vb + k * 8 + 2);
        rest[k].z = (s16)hm_rd16(vb + k * 8 + 4);
    }
    /* which strips are the bear's */
    for (s = 0, face = 0, corner = 0; runs[s] != 0xFFFF && s < 256; corner += (runs[s] >> 8) + 2, s++) {
        s32 n = runs[s] >> 8, ok = 1, j;
        float rgb[3] = { 0, 0, 0 };

        for (j = 0; j < n + 2; j++) {
            strip_of[corner + j] = s;
            if (fabsf(rest[corner + j].x) > BEAR_HALF || rest[corner + j].y < BEAR_TOP) {
                ok = 0;
            }
        }
        for (j = 0; j < n; j++, face++) {
            s32 c = col1[face] & 0x1FFF, q;

            if (--left < 0) {
                t = texs[0];
                texs++;
                left = (t >> 9) & 0x3F;
            }
            if (!(runs[s] & 1) && ((t & 0x8000) ? (t & 0x1FF) != BEAR_CLUT : (t & 0x1FF) >= BEAR_IMAGES)) {
                ok = 0;
            }
            for (q = 0; q < 3; q++) {
                rgb[0] += colours[(c + q) * 4] / (3.0f * n);
                rgb[1] += colours[(c + q) * 4 + 1] / (3.0f * n);
                rgb[2] += colours[(c + q) * 4 + 2] / (3.0f * n);
            }
        }
        /* not blue (his jeans) nor orange (his skin) */
        if (rgb[2] > rgb[0] + 10 || rgb[0] > 1.6f * rgb[1] + 10) {
            ok = 0;
        }
        strip_bear[s] = (s8)ok;
    }
    if (runs[s] != 0xFFFF || memcmp(runs, runs2, (size_t)(s + 1) * 2) != 0) {
        goto fail;
    }
    /* its faces, each a strip of its own on the corners its strip has, drawn the same way round, and the corners they use */
    for (k = 0; k < b->corners; k++) {
        b->vert_of[k] = -1;
    }
    texs = (const u16 *)((const u8 *)&h1->texs + h1->texs);
    left = 0;
    for (s = 0, face = 0, corner = 0; runs[s] != 0xFFFF; corner += (runs[s] >> 8) + 2, s++) {
        s32 n = runs[s] >> 8, flip0 = (runs[s] & 8) != 0, j;

        for (j = 0; j < n; j++, face++) {
            /* the strip's corners in the order this face's made one has them: a strip's first face is drawn a, c, b, so one drawn
             * a, b, c in its own strip has its last two the other way */
            s32 order[3], q;
            MadeFace *mf;

            if (--left < 0) {
                t = texs[0];
                texs++;
                left = (t >> 9) & 0x3F;
            }
            if (!strip_bear[s] || b->nfaces == BEAR_FACES) {
                continue;
            }
            order[0] = 0;
            order[1] = j % 2 == flip0 ? 1 : 2;
            order[2] = j % 2 == flip0 ? 2 : 1;
            mf = &b->faces[b->nfaces++];
            memset(mf, 0, sizeof(*mf));
            mf->image = -1;
            if (!(runs[s] & 1)) {
                mf->image = face_image(b->images, &b->nimages, MAX_BEAR_IMAGES, &ts, t, made_img, made_clut);
            }
            mf->blend = (u16)(col1[face] & 0xE000);
            for (q = 0; q < 3; q++) {
                s32 at = corner + j + order[q], c1 = (col1[face] & 0x1FFF) + order[q], c2 = (col2[face] & 0x1FFF) + order[q], w;

                if (b->vert_of[at] < 0) {
                    b->corner_of[b->nvert] = (s16)at;
                    b->vert_of[at] = (s16)b->nvert++;
                }
                mf->v[q] = b->vert_of[at];
                w = uvs[uv_idx[face] + order[q]];
                mf->uv[q][0] = (u8)(w & 0xFF);
                mf->uv[q][1] = (u8)(w >> 8);
                memcpy(mf->rgb[0][q], colours + c1 * 4, 3);
                memcpy(mf->rgb[1][q], colours + c2 * 4, 3);
            }
        }
    }
    /* his animations, and his parts in his first breathing frame */
    for (i = 0; i < RIDE_ANIMS; i++) {
        if ((b->anim[i] = file_anim_named(b->mdl, RideNames[i])) < 0) {
            goto fail;
        }
    }
    breathe = b->anim[RIDE_BREATHE];
    if (file_frame(b->mdl, breathe, 0, b->ref, BEAR_CORNERS) != b->corners) {
        goto fail;
    }
    for (k = 0; k < b->corners; k++) {
        const V3 *p = &b->ref[k];

        if (strip_bear[strip_of[k]]) {
            if (fabsf(p->x) < 30 && p->z > -40 && p->z < 60) {
                b->back_y = back_n++ == 0 || p->y < b->back_y ? p->y : b->back_y;
            }
            if (p->y < BACK_BELOW && fabsf(p->x) < BACK_HALF && p->z > BACK_FRONT && p->z < BACK_REAR) {
                b->back[b->nback++] = (s16)k;
            }
        } else if (p->y > HIPS_TOP && fabsf(p->x) < BEAR_HALF) {
            b->hips[b->nhips++] = (s16)k;
            zs += p->z;
        } else if (p->y <= HEAD_BOTTOM) {
            b->head[b->nhead++] = (s16)k;
        }
    }
    if (b->nfaces < 64 || b->nhips < 8 || b->nhead < 4 || b->nback < 8 || back_n == 0) {
        goto fail;
    }
    b->seat_z = zs / b->nhips;
    b->made = 1;
    free(tex);
    printf("bonus: the bear from Crash's polar model, %d faces, %d images\n", b->nfaces, b->nimages);
    return 1;
fail:
    printf("bonus: Crash's polar model has no bear to ride\n");
    made_images_free(b->images, b->nimages);
    free(b->mdl);
    free(tex);
    memset(b, 0, sizeof(*b));
    return 0;
}

s32 ride_seat(float *back_y, float *seat_z) {
    if (!bear_load()) {
        return 0;
    }
    *back_y = TheBear.back_y;
    *seat_z = TheBear.seat_z;
    return 1;
}

/* --- riding --- */

typedef struct {
    float m[3][3];
} M3;

static V3 m3_mul(const M3 *r, V3 p) {
    V3 o;

    o.x = r->m[0][0] * p.x + r->m[0][1] * p.y + r->m[0][2] * p.z;
    o.y = r->m[1][0] * p.x + r->m[1][1] * p.y + r->m[1][2] * p.z;
    o.z = r->m[2][0] * p.x + r->m[2][1] * p.y + r->m[2][2] * p.z;
    return o;
}

static V3 m3_mul_t(const M3 *r, V3 p) {
    V3 o;

    o.x = r->m[0][0] * p.x + r->m[1][0] * p.y + r->m[2][0] * p.z;
    o.y = r->m[0][1] * p.x + r->m[1][1] * p.y + r->m[2][1] * p.z;
    o.z = r->m[0][2] * p.x + r->m[1][2] * p.y + r->m[2][2] * p.z;
    return o;
}

static V3 centre_of(const V3 *v, const s16 *ids, s32 n) {
    V3 c = { 0, 0, 0 };
    s32 i;

    for (i = 0; i < n; i++) {
        c.x += v[ids[i]].x / n;
        c.y += v[ids[i]].y / n;
        c.z += v[ids[i]].z / n;
    }
    return c;
}

/* The turn that best takes points A to B about their middles (Horn's: the quaternion is the eigenvector of the largest eigenvalue of a
 * 4 x 4 matrix of their products, found by Jacobi's turns). */
static void fit_turn(const V3 *A, const V3 *B, const s16 *ids, s32 n, M3 *r, V3 *ca, V3 *cb) {
    double S[3][3], N[4][4], V[4][4], w, x, y, z;
    s32 i, j, k, p, q, sweep, best = 0;

    *ca = centre_of(A, ids, n);
    *cb = centre_of(B, ids, n);
    memset(S, 0, sizeof(S));
    for (k = 0; k < n; k++) {
        double a[3], bb[3];

        a[0] = A[ids[k]].x - ca->x;
        a[1] = A[ids[k]].y - ca->y;
        a[2] = A[ids[k]].z - ca->z;
        bb[0] = B[ids[k]].x - cb->x;
        bb[1] = B[ids[k]].y - cb->y;
        bb[2] = B[ids[k]].z - cb->z;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                S[i][j] += a[i] * bb[j];
            }
        }
    }
    N[0][0] = S[0][0] + S[1][1] + S[2][2];
    N[0][1] = N[1][0] = S[1][2] - S[2][1];
    N[0][2] = N[2][0] = S[2][0] - S[0][2];
    N[0][3] = N[3][0] = S[0][1] - S[1][0];
    N[1][1] = S[0][0] - S[1][1] - S[2][2];
    N[1][2] = N[2][1] = S[0][1] + S[1][0];
    N[1][3] = N[3][1] = S[2][0] + S[0][2];
    N[2][2] = -S[0][0] + S[1][1] - S[2][2];
    N[2][3] = N[3][2] = S[1][2] + S[2][1];
    N[3][3] = -S[0][0] - S[1][1] + S[2][2];
    memset(V, 0, sizeof(V));
    for (i = 0; i < 4; i++) {
        V[i][i] = 1;
    }
    for (sweep = 0; sweep < 30; sweep++) {
        for (p = 0; p < 3; p++) {
            for (q = p + 1; q < 4; q++) {
                double th, t, c, s;

                if (fabs(N[p][q]) < 1e-12) {
                    continue;
                }
                th = (N[q][q] - N[p][p]) / (2 * N[p][q]);
                t = (th >= 0 ? 1 : -1) / (fabs(th) + sqrt(th * th + 1));
                c = 1 / sqrt(t * t + 1);
                s = t * c;
                for (k = 0; k < 4; k++) {
                    double akp = N[k][p], akq = N[k][q];

                    N[k][p] = c * akp - s * akq;
                    N[k][q] = s * akp + c * akq;
                }
                for (k = 0; k < 4; k++) {
                    double apk = N[p][k], aqk = N[q][k];

                    N[p][k] = c * apk - s * aqk;
                    N[q][k] = s * apk + c * aqk;
                }
                for (k = 0; k < 4; k++) {
                    double vkp = V[k][p], vkq = V[k][q];

                    V[k][p] = c * vkp - s * vkq;
                    V[k][q] = s * vkp + c * vkq;
                }
            }
        }
    }
    for (k = 1; k < 4; k++) {
        best = N[k][k] > N[best][best] ? k : best;
    }
    w = V[0][best];
    x = V[1][best];
    y = V[2][best];
    z = V[3][best];
    r->m[0][0] = (float)(w * w + x * x - y * y - z * z);
    r->m[0][1] = (float)(2 * (x * y - w * z));
    r->m[0][2] = (float)(2 * (x * z + w * y));
    r->m[1][0] = (float)(2 * (x * y + w * z));
    r->m[1][1] = (float)(w * w - x * x + y * y - z * z);
    r->m[1][2] = (float)(2 * (y * z - w * x));
    r->m[2][0] = (float)(2 * (x * z - w * y));
    r->m[2][1] = (float)(2 * (y * z + w * x));
    r->m[2][2] = (float)(w * w - x * x - y * y + z * z);
}

void hm_fit(const V3 *from, const V3 *to, const s16 *ids, s32 n, Moved *out) {
    M3 r;

    fit_turn(from, to, ids, n, &r, &out->from, &out->to);
    memcpy(out->m, r.m, sizeof(out->m));
}

V3 hm_turned(const Moved *mv, V3 v) {
    M3 r;

    memcpy(r.m, mv->m, sizeof(r.m));
    return m3_mul(&r, v);
}

V3 hm_unturned(const Moved *mv, V3 v) {
    M3 r;

    memcpy(r.m, mv->m, sizeof(r.m));
    return m3_mul_t(&r, v);
}

V3 hm_moved(const Moved *mv, V3 p) {
    V3 d;

    d.x = p.x - mv->from.x;
    d.y = p.y - mv->from.y;
    d.z = p.z - mv->from.z;
    d = hm_turned(mv, d);
    d.x += mv->to.x;
    d.y += mv->to.y;
    d.z += mv->to.z;
    return d;
}

static V3 unit(V3 v) {
    float l = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

    if (l > 0) {
        v.x /= l;
        v.y /= l;
        v.z /= l;
    }
    return v;
}

/* The turn taking direction a to b, as its axis times its angle */
static V3 turn_vector(V3 a, V3 b) {
    V3 ax;
    float sn, ang;

    ax.x = a.y * b.z - a.z * b.y;
    ax.y = a.z * b.x - a.x * b.z;
    ax.z = a.x * b.y - a.y * b.x;
    sn = sqrtf(ax.x * ax.x + ax.y * ax.y + ax.z * ax.z);
    if (sn < 1e-6f) {
        ax.x = ax.y = ax.z = 0;
        return ax;
    }
    ang = atan2f(sn, a.x * b.x + a.y * b.y + a.z * b.z);
    ax.x *= ang / sn;
    ax.y *= ang / sn;
    ax.z *= ang / sn;
    return ax;
}

/* t of a turn given as its axis times its angle */
static void vector_turn(V3 v, float t, M3 *r) {
    float ang = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z), c, s, k;
    V3 ax;

    memset(r, 0, sizeof(*r));
    r->m[0][0] = r->m[1][1] = r->m[2][2] = 1;
    if (ang < 1e-6f) {
        return;
    }
    ax.x = v.x / ang;
    ax.y = v.y / ang;
    ax.z = v.z / ang;
    ang *= t;
    c = cosf(ang);
    s = sinf(ang);
    k = 1 - c;
    r->m[0][0] = c + ax.x * ax.x * k;
    r->m[0][1] = ax.x * ax.y * k - ax.z * s;
    r->m[0][2] = ax.x * ax.z * k + ax.y * s;
    r->m[1][0] = ax.y * ax.x * k + ax.z * s;
    r->m[1][1] = c + ax.y * ax.y * k;
    r->m[1][2] = ax.y * ax.z * k - ax.x * s;
    r->m[2][0] = ax.z * ax.x * k - ax.y * s;
    r->m[2][1] = ax.z * ax.y * k + ax.x * s;
    r->m[2][2] = c + ax.z * ax.z * k;
}

static V3 turn_about(V3 p, V3 c, const M3 *r) {
    V3 d, o;

    d.x = p.x - c.x;
    d.y = p.y - c.y;
    d.z = p.z - c.z;
    d = m3_mul(r, d);
    o.x = c.x + d.x;
    o.y = c.y + d.y;
    o.z = c.z + d.z;
    return o;
}

/* The riding's frames, worked out before the model's written: for each of Polar Push's animations, as long as the hero's are, Crash's
 * frame that each is (the same share of the way through his, blended between the two either side), the bear's back's motion from
 * its first breathing frame, and Crash's lean from his then, in the back's frame (a turn: its axis times its angle), smoothed over
 * RIDE_SMOOTH frames each side (RideSmooth's weights). */
#define RIDE_MAX_FRAMES 128
#define RIDE_SMOOTH 2
static const float RideSmooth[RIDE_SMOOTH * 2 + 1] = { 1, 4, 6, 4, 1 };
static const s8 RideLoops[RIDE_ANIMS] = { 0, 1, 1, 0, 0, 1, 0 }; /* RUN, BREATHE and LIGHT go round and round */

typedef struct {
    M3 back;    /* the bear's back's turn, about its middle in the first breathing frame (from), then to its middle now */
    V3 from, to;
    V3 lean;
    V3 flop;    /* where the swing of what's loose on the hero has got to */
} RideStep;

typedef struct {
    const Rider *r;
    MadeAnim anims[RIDE_ANIMS];
    RideStep steps[RIDE_ANIMS][RIDE_MAX_FRAMES];
} RideCtx;

static RideCtx Ride;

/* Crash's frame f of an animation the hero's has `frames` of: where in his it is, a share of the way through */
static float crash_time(s32 a, s32 f, s32 frames) {
    const Bear *b = &TheBear;
    s32 n = file_anims((const ModelFile *)b->mdl)[b->anim[a]].frames;

    if (RideLoops[a]) {
        return frames > 0 ? (float)f * n / frames : 0;
    }
    return frames > 1 ? (float)f * (n - 1) / (frames - 1) : 0;
}

/* Crash's corners at t of his animation a, blended between the frames either side */
static void crash_at(s32 a, float t, V3 *F) {
    static V3 G[BEAR_CORNERS];
    const Bear *b = &TheBear;
    s32 n = file_anims((const ModelFile *)b->mdl)[b->anim[a]].frames, f0 = (s32)floorf(t), f1, k;
    float u = t - f0;

    if (RideLoops[a]) {
        f0 = (f0 % n + n) % n;
        f1 = (f0 + 1) % n;
    } else {
        f0 = f0 < 0 ? 0 : f0 >= n ? n - 1 : f0;
        f1 = f0 + 1 >= n ? n - 1 : f0 + 1;
    }
    if (file_frame(b->mdl, b->anim[a], f0, F, BEAR_CORNERS) != b->corners) {
        memcpy(F, b->ref, sizeof(V3) * (size_t)b->corners);
        return;
    }
    if (u > 0 && f1 != f0 && file_frame(b->mdl, b->anim[a], f1, G, BEAR_CORNERS) == b->corners) {
        for (k = 0; k < b->corners; k++) {
            F[k] = hm_mix(F[k], G[k], u);
        }
    }
}

static void ride_flop(RideCtx *c, s32 a);

static void ride_steps(RideCtx *c) {
    static V3 F[BEAR_CORNERS];
    static V3 lean[RIDE_MAX_FRAMES];
    const Bear *b = &TheBear;
    V3 hc, hd, spine_ref, spine;
    s32 a, f, d, n;

    hc = centre_of(b->ref, b->hips, b->nhips);
    hd = centre_of(b->ref, b->head, b->nhead);
    spine_ref.x = hd.x - hc.x;
    spine_ref.y = hd.y - hc.y;
    spine_ref.z = hd.z - hc.z;
    spine_ref = unit(spine_ref);
    for (a = 0; a < RIDE_ANIMS; a++) {
        n = c->anims[a].frames;
        /* where Crash's spine points each frame (the way he leans as the world sees it, not the bear), part of the way back toward
         * upright as he first sat, steadier than him */
        for (f = 0; f < n; f++) {
            RideStep *st = &c->steps[a][f];

            crash_at(a, crash_time(a, f, n), F);
            fit_turn(b->ref, F, b->back, b->nback, &st->back, &st->from, &st->to);
            hc = centre_of(F, b->hips, b->nhips);
            hd = centre_of(F, b->head, b->nhead);
            spine.x = hd.x - hc.x;
            spine.y = hd.y - hc.y;
            spine.z = hd.z - hc.z;
            lean[f] = unit(hm_mix(unit(spine), spine_ref, c->r->steady));
        }
        /* that smoothed, so it doesn't jitter, and then as a turn from how he sat in the bear's back's frame: the bear can pitch every
         * frame and he still leans smoothly against it */
        for (f = 0; f < n; f++) {
            V3 sum = { 0, 0, 0 };

            for (d = -RIDE_SMOOTH; d <= RIDE_SMOOTH; d++) {
                s32 g = f + d;

                if (RideLoops[a]) {
                    g = (g % n + n) % n;
                } else {
                    g = g < 0 ? 0 : g >= n ? n - 1 : g;
                }
                sum.x += lean[g].x * RideSmooth[d + RIDE_SMOOTH];
                sum.y += lean[g].y * RideSmooth[d + RIDE_SMOOTH];
                sum.z += lean[g].z * RideSmooth[d + RIDE_SMOOTH];
            }
            c->steps[a][f].lean = turn_vector(spine_ref, unit(m3_mul_t(&c->steps[a][f].back, unit(sum))));
        }
        if (c->r->flop != NULL && c->r->swing.period > 0) {
            ride_flop(c, a);
        }
    }
}

/* The hero's vertices at a step, but for what's loose on it: sat, its own motion, bent at its neck and waist as Crash leans, then
 * moved as the bear's back is */
static void hero_at(const RideCtx *c, s32 a, s32 f, V3 *out) {
    const Rider *r = c->r;
    const RideStep *st = &c->steps[a][f];
    M3 at_waist, at_neck;
    s32 k;

    vector_turn(st->lean, r->lean_waist, &at_waist);
    vector_turn(st->lean, r->lean_neck, &at_neck);
    memcpy(out, r->seat, sizeof(V3) * (size_t)r->nvert);
    if (r->own != NULL) {
        r->own(r->ctx, a, f, c->anims[a].frames, out);
    }
    for (k = 0; k < r->nvert; k++) {
        V3 q = out[k], d;

        q = hm_mix(q, turn_about(q, r->neck, &at_neck), r->neck_share[k]);
        q = hm_mix(q, turn_about(q, r->waist, &at_waist), r->waist_share[k]);
        d.x = q.x - st->from.x;
        d.y = q.y - st->from.y;
        d.z = q.z - st->from.z;
        d = m3_mul(&st->back, d);
        out[k].x = st->to.x + d.x;
        out[k].y = st->to.y + d.y;
        out[k].z = st->to.z + d.z;
    }
}

void hm_flop(const V3 *path, s32 n, s32 loops, const Flop *spring, V3 *out) {
    s32 rounds = loops ? 4 : 1, round, f;
    float K, C;
    V3 x = { 0, 0, 0 }, v = { 0, 0, 0 };

    if (n <= 0 || spring->period <= 0) {
        return;
    }
    K = 4 * 3.14159265f * 3.14159265f / (spring->period * spring->period);
    C = 2 * spring->damping * sqrtf(K);
    for (round = 0; round < rounds; round++) {
        for (f = 0; f < n; f++) {
            s32 last = f - 1, next = f + 1;
            V3 acc = { 0, 0, 0 };
            float len;

            if (loops) {
                last = (last + n) % n;
                next %= n;
            }
            if (last >= 0 && next < n && n > 2) {
                acc.x = path[next].x - 2 * path[f].x + path[last].x;
                acc.y = path[next].y - 2 * path[f].y + path[last].y;
                acc.z = path[next].z - 2 * path[f].z + path[last].z;
            }
            v.x += -K * x.x - C * v.x - spring->gain * acc.x;
            v.y += -K * x.y - C * v.y - spring->gain * acc.y;
            v.z += -K * x.z - C * v.z - spring->gain * acc.z;
            x.x += v.x;
            x.y += v.y;
            x.z += v.z;
            if ((len = sqrtf(x.x * x.x + x.y * x.y + x.z * x.z)) > spring->max) {
                x.x *= spring->max / len;
                x.y *= spring->max / len;
                x.z *= spring->max / len;
                v.x = v.y = v.z = 0;
            }
            if (round == rounds - 1) {
                out[f] = x;
            }
        }
    }
}

/* The swing of what's loose on the rider, each frame of animation a, from its head's path (the middle of what bends at its neck) */
static void ride_flop(RideCtx *c, s32 a) {
    static V3 path[RIDE_MAX_FRAMES], swing[RIDE_MAX_FRAMES];
    const Rider *r = c->r;
    s32 n = c->anims[a].frames, f, k;
    V3 *verts = malloc(sizeof(V3) * (size_t)r->nvert);

    if (verts == NULL) {
        return;
    }
    for (f = 0; f < n; f++) {
        float w = 0;

        hero_at(c, a, f, verts);
        path[f].x = path[f].y = path[f].z = 0;
        for (k = 0; k < r->nvert; k++) {
            path[f].x += verts[k].x * r->neck_share[k];
            path[f].y += verts[k].y * r->neck_share[k];
            path[f].z += verts[k].z * r->neck_share[k];
            w += r->neck_share[k];
        }
        if (w > 0) {
            path[f].x /= w;
            path[f].y /= w;
            path[f].z /= w;
        }
    }
    free(verts);
    hm_flop(path, n, RideLoops[a], &r->swing, swing);
    for (f = 0; f < n; f++) {
        c->steps[a][f].flop = swing[f];
    }
}

static void ride_frame(void *ctx, s32 a, s32 f, V3 *out) {
    static V3 F[BEAR_CORNERS];
    const RideCtx *c = ctx;
    const Rider *r = c->r;
    const Bear *b = &TheBear;
    const RideStep *st = &c->steps[a][f];
    s32 k;

    crash_at(a, crash_time(a, f, c->anims[a].frames), F);
    hero_at(c, a, f, out);
    for (k = 0; r->flop != NULL && k < r->nvert; k++) {
        out[k].x += st->flop.x * r->flop[k];
        out[k].y += st->flop.y * r->flop[k];
        out[k].z += st->flop.z * r->flop[k];
    }
    /* and the bear */
    for (k = 0; k < b->nvert; k++) {
        out[r->nvert + k] = F[b->corner_of[k]];
    }
}

s32 ride_make(const Rider *r, u8 **tex, s32 *tex_size, u8 **mdl, s32 *mdl_size) {
    const Bear *b = &TheBear;
    MadeFace *faces;
    MadeImage images[MADE_MAX_IMAGES + MAX_BEAR_IMAGES];
    MadeModel mm;
    u8 *match = NULL;
    s32 i, j, k, ok, size = 0;

    *tex = *mdl = NULL;
    if (!bear_load() || r->nimages + b->nimages > MADE_MAX_IMAGES + MAX_BEAR_IMAGES
        || (faces = malloc(sizeof(MadeFace) * (size_t)(r->nfaces + b->nfaces))) == NULL) {
        return 0;
    }
    /* its faces, and their charred colours; then the bear's, on its vertices after the hero's */
    for (i = 0; i < r->nfaces; i++) {
        faces[i] = r->faces[i];
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                faces[i].rgb[1][j][k] = (u8)(faces[i].rgb[0][j][k] * r->charred);
            }
        }
    }
    for (i = 0; i < b->nfaces; i++) {
        MadeFace *f = &faces[r->nfaces + i];

        *f = b->faces[i];
        for (j = 0; j < 3; j++) {
            f->v[j] += r->nvert;
        }
        f->image = f->image >= 0 ? f->image + r->nimages : -1;
    }
    memcpy(images, r->images, sizeof(MadeImage) * (size_t)r->nimages);
    memcpy(images + r->nimages, b->images, sizeof(MadeImage) * (size_t)b->nimages);
    /* Polar Push's animations, as long as the model's it matches (or Crash's), each on Crash's mesh for it */
    if (r->match_bank >= 0) {
        match = archive_own_file((s32)(HeroModelBank[r->match_bank].file - DatIndex), &size);
    }
    Ride.r = r;
    memset(Ride.steps, 0, sizeof(Ride.steps));
    for (i = 0; i < RIDE_ANIMS; i++) {
        const FileEntry *e = &file_anims((const ModelFile *)b->mdl)[b->anim[i]];
        const MeshHeader *moved = (const MeshHeader *)((const u8 *)&e->mesh + e->mesh);
        s32 m = match != NULL ? file_anim_named(match, RideNames[i]) : -1;
        s32 frames = m >= 0 ? file_anims((const ModelFile *)match)[m].frames : e->frames;

        Ride.anims[i].name = RideNames[i];
        Ride.anims[i].frames = frames < 1 ? 1 : frames >= RIDE_MAX_FRAMES ? RIDE_MAX_FRAMES - 1 : frames;
        Ride.anims[i].mesh = moved == &((const ModelFile *)b->mdl)->meshes[1] ? 1 : 0;
    }
    free(match);
    ride_steps(&Ride);
    memset(&mm, 0, sizeof(mm));
    mm.faces = faces;
    mm.nfaces = r->nfaces + b->nfaces;
    mm.meshes = 2;
    mm.images = images;
    mm.nimages = r->nimages + b->nimages;
    mm.anims = Ride.anims;
    mm.nanims = RIDE_ANIMS;
    mm.nvert = r->nvert + b->nvert;
    mm.rest_anim = RIDE_BREATHE;
    mm.frame = ride_frame;
    mm.ctx = &Ride;
    ok = made_write(&mm, tex, tex_size, mdl, mdl_size);
    free(faces);
    return ok;
}

/* --- sounds --- */

#define VAB_PROGRAMS 0x20  /* a sound pack's VAB header (its VH): 128 programs of 16 bytes from here, */
#define VAB_TONES 0x820    /* then 16 tones of 32 bytes each of its programs, then a u16 size / 8 each of its samples from slot 1 */

s32 hm_game_sound(s32 entry, s32 id, HeroSound *out) {
    s32 size = 0, nprog, nsample, p, j, k, at, vag = -1, centre = 0, shift = 0;
    u8 *pack = archive_own_file(entry, &size), *copy;
    const u8 *vh, *vb, *sizes;
    u32 from = 0, bytes;
    double pitch;

    memset(out, 0, sizeof(*out));
    if (pack == NULL || size < 8) {
        free(pack);
        return 0;
    }
    /* the pack: a table of offsets, the VB (its samples) first and the VH */
    vb = pack + hm_rd32(pack);
    vh = pack + hm_rd32(pack + 4);
    if (vh + VAB_TONES > pack + size) {
        free(pack);
        return 0;
    }
    nprog = hm_rd16(vh + 0x12);
    nsample = hm_rd16(vh + 0x16);
    sizes = vh + VAB_TONES + nprog * 16 * 32;
    for (p = 0; p < nprog && vag < 0; p++) {
        for (j = 0; j < 16 && j < vh[VAB_PROGRAMS + p * 16]; j++) {
            const u8 *t = vh + VAB_TONES + (p * 16 + j) * 32;

            if ((t[6] | t[7] << 8) == id) {
                vag = hm_rd16(t + 22);
                centre = t[4];
                shift = t[5];
                break;
            }
        }
    }
    if (vag <= 0 || vag > nsample || sizes + (vag + 1) * 2 > pack + size) {
        free(pack);
        return 0;
    }
    for (k = 1; k < vag; k++) {
        from += hm_rd16(sizes + k * 2) * 8u;
    }
    bytes = hm_rd16(sizes + vag * 2) * 8u;
    at = (s32)(vb - pack) + (s32)from;
    if (bytes < 16 || at + (s32)bytes > size || (copy = malloc(bytes)) == NULL) {
        free(pack);
        return 0;
    }
    memcpy(copy, pack + at, bytes);
    free(pack);
    pitch = 4096.0 * pow(2.0, (60 + shift / 128.0 - centre) / 12.0);
    out->adpcm = copy;
    out->bytes = (s32)bytes;
    out->pitch = pitch > 0x3FFF ? 0x3FFF : (u32)pitch;
    return 1;
}

void hm_play(const HeroSound *s, short *voice) {
    s32 vol = 0x1000 * GameOptions.sfx * 0x7F / 409600;
    short v;

    if (s == NULL || s->adpcm == NULL || s->bytes < 16) {
        return;
    }
    if (voice != NULL && audio_sample_playing(*voice, s->adpcm)) {
        audio_voice_off(*voice);
    }
    vol = vol < 0 ? 0 : vol > 0x7F ? 0x7F : vol;
    v = audio_play_sample(s->adpcm, (uint32_t)s->bytes, s->pitch, 0x3FFF * vol / 0x7F, 0x3FFF * vol / 0x7F);
    if (voice != NULL) {
        *voice = v;
    }
}

/* --- the heroes --- */

static void make_hero(s32 extra) {
    const HeroDef *d = HeroDefs[extra];
    HeroMade *m = &Made[extra];

    Tried[extra] = 1;
    memset(m, 0, sizeof(*m));
    if (d == NULL || d->make == NULL || !d->make(m) || m->mdl[KIND_FOOT] == NULL) {
        s32 k;

        for (k = 0; k < NUM_KINDS; k++) {
            free(m->tex[k]);
            free(m->mdl[k]);
        }
        memset(m, 0, sizeof(*m));
        return;
    }
    Have[extra] = 1;
}

void bonus_heroes_files(void) {
    s32 x, k;

    if (!extra_heroes_on()) {
        return;
    }
    for (x = EXTRA_NONE + 1; x < NUM_EXTRA; x++) {
        if (!Tried[x]) {
            make_hero(x);
        }
        for (k = 0; Have[x] && k < NUM_KINDS; k++) {
            u8 *tex, *mdl;

            if (Made[x].mdl[k] == NULL || (tex = malloc((size_t)Made[x].tex_size[k])) == NULL) {
                continue;
            }
            if ((mdl = malloc((size_t)Made[x].mdl_size[k])) == NULL) {
                free(tex);
                continue;
            }
            memcpy(tex, Made[x].tex[k], (size_t)Made[x].tex_size[k]);
            memcpy(mdl, Made[x].mdl[k], (size_t)Made[x].mdl_size[k]);
            archive_replace(Slots[x][k].tex, tex, Made[x].tex_size[k]);
            archive_replace(Slots[x][k].mdl, mdl, Made[x].mdl_size[k]);
        }
    }
}

s32 bonus_heroes_have(s32 extra) {
    return extra > EXTRA_NONE && extra < NUM_EXTRA && Have[extra] && extra_heroes_on();
}

s32 bonus_heroes_bank(s32 extra, s32 kind) {
    if (extra <= EXTRA_NONE || extra >= NUM_EXTRA || kind < 0 || kind >= NUM_KINDS) {
        return DB_Empty;
    }
    /* a kind it has none of: its model on foot */
    return Slots[extra][Have[extra] && Made[extra].mdl[kind] == NULL ? KIND_FOOT : kind].bank;
}

u8 *bonus_heroes_model(s32 extra, s32 kind) {
    u8 *copy;

    if (!bonus_heroes_have(extra) || kind < 0 || kind >= NUM_KINDS) {
        return NULL;
    }
    if (Made[extra].mdl[kind] == NULL) {
        kind = KIND_FOOT;
    }
    if ((copy = malloc((size_t)Made[extra].mdl_size[kind])) != NULL) {
        memcpy(copy, Made[extra].mdl[kind], (size_t)Made[extra].mdl_size[kind]);
    }
    return copy;
}

const u32 *bonus_heroes_card(s32 extra, s32 *w, s32 *h, s32 *head) {
    if (!bonus_heroes_have(extra) || Made[extra].card == NULL) {
        return NULL;
    }
    *w = Made[extra].card_w;
    *h = Made[extra].card_h;
    *head = Made[extra].card_head;
    return Made[extra].card;
}

void bonus_heroes_voice(s32 extra, s32 voice) {
    /* its laugh one at a time, a new one starting it again, as a hero's taunt sound does (its move's 0x800A, on its own sound) */
    static short laughing = -1;

    if (!bonus_heroes_have(extra) || voice <= VOICE_BASE || voice >= NUM_VOICES) {
        return;
    }
    hm_play(&Made[extra].sound[voice], voice == VOICE_LAUGH || voice == VOICE_NAME ? &laughing : NULL);
}
