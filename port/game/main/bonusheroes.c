/* Bonus heroes (bash/bonusheroes.h, PC only): extra heroes' models from the player's own copies of other Crash games.
 *
 * A disc is found in the discs folders (disc_image_find), the character's level read from it, and its model and animations made into a
 * Crash Bash model and texture set (docs/ASSETS.md), which go into the archive's entries 992 and 993 (main.c) as a mod's files do: the
 * game loads them as it does any model, into a bank of their own (DB_BonusRoo), and nothing of them is kept but in memory.
 *
 * Ripper Roo, from Crash Bandicoot (USA, SCUS-94900): level 0x17, his boss fight, has his model (RS1nG) and four animations, RS1nV (a big
 * jump), RS2nV (standing), RS3nV (a laugh) and RS4nV (knocked over), which become JUMP, BREATHE, TAUNT and DIE.
 *
 * Crash Bandicoot's formats, as CrashEdit (the cbhacks project) reads them: a level's NSF is 64 KB pages, each raw (magic 0x1234) or
 * compressed (0x1235: runs and repeats for its first `length` bytes, `skip` bytes to pass, the rest raw). A page of entries lists them,
 * and each entry its items; a texture page is 256 x 128 16-bit units of VRAM. An animation's items are its frames: a 56-byte header
 * (vertex count, model, x, y, z offsets, collision) and 6 bytes a vertex (x, y, z unsigned, a normal signed), each at (xyz + offset -
 * 128) * the model's scale. A model's first item has its triangle count, its scale and its colours (4-byte slots: r, g, b, flags) and
 * textures (3 slots: the colour with byte 3's top bit, the texture page, and where in it), its second 8 bytes a triangle: three
 * vertices' byte offsets and its slot. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "bash/main.h"
#include "bash/modelbank.h"
#include "bash/extraheroes.h"
#include "bash/bonusheroes.h"
#include "../../platform/platform.h"
#include "../../platform/audio.h"

void archive_replace(int entry, unsigned char *bytes, int size);
void SDL_free(void *mem);

#define PAGE_BYTES 65536
#define BONUS_TEX 992
#define BONUS_MDL 993
#define ROO_HEIGHT 620.0f   /* standing, in Crash Bash's units: Crate Crush's heroes stand 452 (Crash) to 748 (Kong), most about 650 */
#define MAX_TRIS 2048
#define MAX_VERTS 1024
#define MAX_IMAGES 32
#define FLIP_FACES 0        /* 1: the other way round from Crash Bandicoot's */

static u32 rd32(const u8 *p) {
    return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24;
}

static u16 rd16(const u8 *p) {
    return (u16)(p[0] | p[1] << 8);
}

static void wr32(u8 *p, u32 v) {
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
    p[2] = (u8)(v >> 16);
    p[3] = (u8)(v >> 24);
}

static void wr16(u8 *p, u32 v) {
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
}

/* --- Crash Bandicoot's levels --- */

typedef struct {
    u8 *pages; /* count of PAGE_BYTES */
    s32 count;
} Level;

static s32 decompress(const u8 *in, size_t size, size_t *at, u8 *out) {
    size_t p = *at;
    s32 length, skip, pos = 0, rest;

    if (p + 12 > size) {
        return 0;
    }
    length = (s32)rd32(in + p + 4);
    skip = (s32)rd32(in + p + 8);
    if (length < 0 || length > PAGE_BYTES || skip < 0) {
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
            if (seek > pos || seek == 0 || pos + span > PAGE_BYTES) {
                return 0;
            }
            for (i = 0; i < span; i++) {
                out[pos + i] = out[pos - seek + i];
            }
            pos += span;
        } else {
            if (p + (size_t)prefix > size || pos + prefix > PAGE_BYTES) {
                return 0;
            }
            memcpy(out + pos, in + p, (size_t)prefix);
            p += (size_t)prefix;
            pos += prefix;
        }
    }
    p += (size_t)skip;
    rest = PAGE_BYTES - length < PAGE_BYTES - pos ? PAGE_BYTES - length : PAGE_BYTES - pos;
    if (p + (size_t)(PAGE_BYTES - length) > size) {
        return 0;
    }
    memcpy(out + pos, in + p, (size_t)rest);
    *at = p + (size_t)(PAGE_BYTES - length);
    return 1;
}

static s32 level_read(DiscImage *disc, const char *path, Level *out) {
    size_t size = 0, at = 0;
    u8 *nsf = disc_image_read(disc, path, &size);
    s32 cap = 0;

    memset(out, 0, sizeof(*out));
    if (nsf == NULL) {
        return 0;
    }
    while (at + 12 <= size) {
        u16 magic = rd16(nsf + at);
        u8 *page;

        if (out->count == cap) {
            u8 *more = realloc(out->pages, (size_t)(cap = cap ? cap * 2 : 64) * PAGE_BYTES);

            if (more == NULL) {
                break;
            }
            out->pages = more;
        }
        page = out->pages + (size_t)out->count * PAGE_BYTES;
        if (magic == 0x1234 && at + PAGE_BYTES <= size) {
            memcpy(page, nsf + at, PAGE_BYTES);
            at += PAGE_BYTES;
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

/* An entry: its items, each with its size. */
typedef struct {
    s32 type, count;
    const u8 *item[128];
    s32 size[128];
} Entry;

static s32 entry_find(const Level *l, const char *name, Entry *out) {
    u32 eid = eid_of(name);
    s32 p, k, i;

    for (p = 0; p < l->count; p++) {
        const u8 *page = l->pages + (size_t)p * PAGE_BYTES;
        s32 n = (s32)rd32(page + 8);

        /* a page of entries, or of sounds (3) */
        if ((rd16(page + 2) != 0 && rd16(page + 2) != 3) || n < 0 || n > 4096) {
            continue;
        }
        for (k = 0; k < n; k++) {
            u32 s = rd32(page + 16 + k * 4), e = rd32(page + 20 + k * 4);
            const u8 *en = page + s;
            s32 items;

            if (s >= e || e > PAGE_BYTES || e - s < 16 || rd32(en) != 0x100FFFF || rd32(en + 4) != eid) {
                continue;
            }
            items = (s32)rd32(en + 12);
            if (items < 0 || items > 128 || 20 + items * 4 > (s32)(e - s)) {
                return 0;
            }
            out->type = (s32)rd32(en + 8);
            out->count = items;
            for (i = 0; i < items; i++) {
                u32 a = rd32(en + 16 + i * 4), b = rd32(en + 20 + i * 4);

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

static const u8 *texture_page(const Level *l, u32 eid) {
    s32 p;

    for (p = 0; p < l->count; p++) {
        const u8 *page = l->pages + (size_t)p * PAGE_BYTES;

        if (rd16(page + 2) == 1 && rd32(page + 4) == eid) {
            return page;
        }
    }
    return NULL;
}

/* --- into Crash Bash's --- */

/* A grown buffer of bytes. */
typedef struct {
    u8 *b;
    size_t n, cap;
} Bytes;

static size_t put(Bytes *o, const void *src, size_t n) {
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

static void align(Bytes *o, size_t to) {
    if (o->n % to != 0) {
        put(o, NULL, to - o->n % to);
    }
}

/* A self offset: from the field at `field` to `to`. */
static void self(Bytes *o, size_t field, size_t to) {
    wr32(o->b + field, (u32)(s32)((long long)to - (long long)field));
}

typedef struct {
    float x, y, z;
} V3;

/* A texture of Roo's: a rect of a page, in 4 or 8 bits, with its palette, as a Crash Bash image. */
typedef struct {
    u32 page_eid, info;
    s32 clutx;
    s32 x, y, w, h, bpp;
} Image;

typedef struct {
    s32 a, b, c;      /* vertices */
    s32 slot;
    s32 image;        /* -1 untextured */
    s32 uv[3][2];     /* in the image's texels */
    u8 rgb[3];
} Tri;

typedef struct {
    const char *entry, *name;
} Anim;

static const Anim RooAnims[] = { { "RS1nV", "JUMP" }, { "RS2nV", "BREATHE" }, { "RS3nV", "TAUNT" }, { "RS4nV", "DIE" } };
#define ROO_ANIMS 4
#define ROO_REST 1 /* the one he stands in: his height and his colours' light are from its first frame */

/* A frame's vertices, in Crash Bash's axes (y down, z the other way) before they're scaled. */
static s32 frame_verts(const u8 *f, s32 size, const s32 scale[3], V3 *out, V3 *normals, s32 max) {
    s32 n = (s32)rd32(f), k;
    s32 off[3];

    if (n <= 0 || n > max || 56 + n * 6 > size) {
        return 0;
    }
    off[0] = (s32)rd32(f + 8);
    off[1] = (s32)rd32(f + 12);
    off[2] = (s32)rd32(f + 16);
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

static void put_box(Bytes *o, const V3 *v, s32 n, float k, float ground) {
    float lo[3] = { 1e9f, 1e9f, 1e9f }, hi[3] = { -1e9f, -1e9f, -1e9f }, r = 0;
    u8 box[0x14];
    s32 i, j;

    for (i = 0; i < n; i++) {
        float p[3] = { v[i].x * k, v[i].y * k - ground, v[i].z * k };

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
    wr16(box + 0, (u32)(s16)lo[0]);
    wr16(box + 2, (u32)(s16)hi[1]);
    wr16(box + 4, (u32)(s16)lo[2]);
    wr16(box + 6, (u32)(s16)hi[0]);
    wr16(box + 8, (u32)(s16)lo[1]);
    wr16(box + 10, (u32)(s16)hi[2]);
    wr16(box + 12, (u32)(s16)((lo[0] + hi[0]) / 2));
    wr16(box + 14, (u32)(s16)((lo[1] + hi[1]) / 2));
    wr16(box + 16, (u32)(s16)((lo[2] + hi[2]) / 2));
    wr16(box + 18, (u32)(s16)sqrtf(r));
    put(o, box, sizeof(box));
}

static u8 *RooTex, *RooMdl;
static s32 RooTexSize, RooMdlSize, RooTried, RooMade;

/* His laugh, which his boss fight's object (RRooC) plays: a sound entry (CrznA) in a sound page, PS1 ADPCM at 11025 Hz, as CrashEdit
 * plays that game's */
#define ROO_LAUGH_PITCH 0x400
static u8 *RooLaugh;
static s32 RooLaughSize;

static void make_laugh(const Level *l) {
    Entry e;

    if (RooLaugh != NULL || !entry_find(l, "CrznA", &e) || e.type != 12 || e.count < 1 || e.size[0] < 16) {
        return;
    }
    RooLaugh = malloc((size_t)e.size[0]);
    if (RooLaugh != NULL) {
        memcpy(RooLaugh, e.item[0], (size_t)e.size[0]);
        RooLaughSize = e.size[0];
    }
}

/* His hero card: the face his boss fight shows, whose tongue is his health (ShadC, the fight's panel: a 64 x 64 sprite), as red | green
 * << 8 | blue << 16 | alpha << 24, with the tongue short: his head and the tongue as it leaves his mouth (rows 0 to 41), then the
 * curl at its end (rows 50 to 62), so it hangs over the health bar. */
#define CARD_HEAD 42
#define CARD_TIP 50
#define CARD_ROWS (CARD_HEAD + 63 - CARD_TIP)
static u32 RooCard[64 * CARD_ROWS];
static s32 RooCardMade;

static void make_card(const Level *l) {
    Entry obj;
    const u8 *it;
    s32 at, f, x, y;

    if (!entry_find(l, "ShadC", &obj) || obj.type != 11 || obj.count < 6) {
        return;
    }
    it = obj.item[5];
    for (at = 4; at + 12 <= obj.size[5]; at += 4) {
        const u8 *page = texture_page(l, rd32(it + at));
        u32 head = rd32(it + at - 4);

        if (page == NULL || (head & 0xFFFF) != 2) {
            continue;
        }
        for (f = 0; f < (s32)(head >> 16) && at + 12 + f * 8 <= obj.size[5]; f++) {
            u32 col = rd32(it + at + 4 + f * 8), ti = rd32(it + at + 8 + f * 8);
            s32 uvi = (s32)(ti >> 22 & 0x3FF), cm = (s32)(ti >> 20 & 3), seg = (s32)(ti >> 18 & 3), xoffu = (s32)(ti >> 13 & 0x1F);
            s32 cy = (s32)(ti >> 6 & 0x7F), y0 = (s32)(ti & 0x1F) * 4, x0 = 256 * seg + 8 * xoffu, cx = (s32)(col >> 24 & 0xF) * 16;

            /* the face: the one 64 x 64 4-bit sprite */
            if (cm != 0 || 4 << (uvi % 5) != 64 || 4 << (uvi / 5 % 5) != 64 || y0 + 64 > 128) {
                continue;
            }
            for (y = 0; y < CARD_ROWS; y++) {
                s32 row = y < CARD_HEAD ? y : y - CARD_HEAD + CARD_TIP;

                for (x = 0; x < 64; x++) {
                    s32 u = x0 + x, i = page[(y0 + row) * 512 + u / 2] >> (u & 1) * 4 & 15;
                    u16 c = rd16(page + cy * 512 + (cx + i) * 2);

                    RooCard[y * 64 + x] = c == 0 ? 0 : 0xFF000000u | (u32)((c & 0x1F) * 255 / 31) | (u32)((c >> 5 & 0x1F) * 255 / 31) << 8
                                                   | (u32)((c >> 10 & 0x1F) * 255 / 31) << 16;
                }
            }
            RooCardMade = 1;
            return;
        }
    }
}

static void make_roo(void) {
    static V3 verts[MAX_VERTS], normals[MAX_VERTS];
    static Tri tris[MAX_TRIS];
    static Image images[MAX_IMAGES];
    DiscImage *disc;
    Level level;
    Entry model, anims[ROO_ANIMS];
    Bytes tex = { 0 }, mdl = { 0 };
    const u8 *info, *polys;
    s32 scale[3], ntri, nslot, nvert = 0, nimage = 0, i, j, k, n;
    float lo = 1e9f, hi = -1e9f, kscale, ground;
    size_t mesh, strips, vblock, uvidx, texs, colidx, end, colours, uvs, zeros, count0, entries, kept;

    const V3 light = { -0.35f, -0.75f, -0.55f };

    RooTried = 1;
    disc = disc_image_find("SCUS-94900");
    if (disc == NULL) {
        return;
    }
    if (!level_read(disc, "S1/S0000017.NSF", &level)) {
        printf("bonus: Crash Bandicoot's Ripper Roo level can't be read\n");
        disc_image_close(disc);
        return;
    }
    disc_image_close(disc);
    if (!entry_find(&level, "RS1nG", &model) || model.type != 2 || model.count < 2) {
        printf("bonus: Crash Bandicoot's disc has no Ripper Roo\n");
        goto done;
    }
    for (i = 0; i < ROO_ANIMS; i++) {
        if (!entry_find(&level, RooAnims[i].entry, &anims[i]) || anims[i].type != 1 || anims[i].count <= 0) {
            printf("bonus: Ripper Roo's %s is missing\n", RooAnims[i].entry);
            goto done;
        }
    }
    info = model.item[0];
    polys = model.item[1];
    ntri = (s32)rd32(info);
    scale[0] = (s32)rd32(info + 4);
    scale[1] = (s32)rd32(info + 8);
    scale[2] = (s32)rd32(info + 12);
    nslot = (s32)rd32(info + 16);
    if (ntri <= 0 || ntri > MAX_TRIS || model.size[1] < ntri * 8 || model.size[0] < 20 + nslot * 4) {
        goto done;
    }
    /* how big he is, standing */
    nvert = frame_verts(anims[ROO_REST].item[0], anims[ROO_REST].size[0], scale, verts, normals, MAX_VERTS);
    if (nvert == 0) {
        goto done;
    }
    for (k = 0; k < nvert; k++) {
        lo = verts[k].y < lo ? verts[k].y : lo;
        hi = verts[k].y > hi ? verts[k].y : hi;
    }
    kscale = ROO_HEIGHT / (hi - lo);
    ground = hi * kscale;

    /* his triangles, their colours lit as he stands, and their textures */
    for (i = 0; i < ntri; i++) {
        const u8 *p = polys + i * 8, *s;
        Tri *t = &tris[i];
        u16 w = rd16(p + 6);

        t->a = rd16(p) / 6;
        t->b = rd16(p + 2) / 6;
        t->c = rd16(p + 4) / 6;
        t->slot = w & 0x7FFF;
        t->image = -1;
        if (t->a >= nvert || t->b >= nvert || t->c >= nvert || t->slot >= nslot) {
            t->a = t->b = t->c = 0;
            t->slot = 0;
        }
        s = info + 20 + t->slot * 4;
        t->rgb[0] = s[0];
        t->rgb[1] = s[1];
        t->rgb[2] = s[2];
        if ((s[3] & 0x80) && 20 + (t->slot + 3) * 4 <= model.size[0]) {
            u32 page = rd32(s + 4), tinfo = rd32(s + 8);
            s32 uvi = (s32)(tinfo >> 22 & 0x3FF), cm = (s32)(tinfo >> 20 & 3), seg = (s32)(tinfo >> 18 & 3);
            s32 xoffu = (s32)(tinfo >> 13 & 0x1F), yoffu = (s32)(tinfo & 0x1F), wind = uvi / 25 & 31;
            s32 tw = 4 << (uvi % 5), th = 4 << (uvi / 5 % 5), xoff, yoff;

            if (cm <= 1 && texture_page(&level, page) != NULL) {
                xoff = (64 << (2 - cm)) * seg + (2 << (2 - cm)) * xoffu;
                yoff = yoffu * 4;
                for (j = 0; j < nimage; j++) {
                    if (images[j].page_eid == page && images[j].x == xoff && images[j].y == yoff && images[j].w == tw && images[j].h == th
                        && images[j].bpp == (cm == 0 ? 4 : 8) && images[j].clutx == (s[3] & 0xF) && (images[j].info >> 6 & 0x7F) == (tinfo >> 6 & 0x7F)) {
                        break;
                    }
                }
                if (j == nimage && nimage < MAX_IMAGES) {
                    images[nimage].page_eid = page;
                    images[nimage].info = tinfo;
                    images[nimage].clutx = s[3] & 0xF;
                    images[nimage].x = xoff;
                    images[nimage].y = yoff;
                    images[nimage].w = tw;
                    images[nimage].h = th;
                    images[nimage].bpp = cm == 0 ? 4 : 8;
                    nimage++;
                }
                if (j < nimage) {
                    t->image = j;
                    /* corner a has U1 V1, b U2 V2, c U3 V3 */
                    t->uv[0][0] = tw * (0x30FF0C >> wind & 1);
                    t->uv[1][0] = tw * (0x8799E1 >> wind & 1);
                    t->uv[2][0] = tw * (0x4B66D2 >> wind & 1);
                    t->uv[0][1] = th * (0xF3CC30 >> wind & 1);
                    t->uv[1][1] = th * (0x9E7186 >> wind & 1);
                    t->uv[2][1] = th * (0x6DB249 >> wind & 1);

                }
            }
        }
    }

    /* the texture set: a palette and an image a texture */
    {
        size_t first_image;

        put(&tex, NULL, 0x20);
        wr32(tex.b, 8);
        for (j = 0; j < nimage; j++) {
            const u8 *page = texture_page(&level, images[j].page_eid);
            s32 colours_n = images[j].bpp == 4 ? 16 : 256, cy = (s32)(images[j].info >> 6 & 0x7F), cx = images[j].clutx * 16;
            u8 head[4];

            wr32(head, (u32)colours_n);
            put(&tex, head, 4);
            for (k = 0; k < colours_n; k++) {
                s32 x = cx + k;

                put(&tex, x < 256 ? page + cy * 512 + x * 2 : (const u8 *)"\0\0", 2);
            }
        }
        first_image = tex.n;
        for (j = 0; j < nimage; j++) {
            const u8 *page = texture_page(&level, images[j].page_eid);
            s32 units = images[j].bpp == 4 ? images[j].w / 4 : images[j].w / 2, row = units * 2, y;
            u8 head[20];

            memset(head, 0, sizeof(head));
            wr16(head, (u32)units);
            wr16(head + 2, (u32)images[j].h);
            wr16(head + 6, (u32)((images[j].h & 0xFF) << 8 | (row & 0xFF)));
            wr16(head + 12, (u32)(j << 1 | (images[j].bpp == 8)));
            wr32(head + 16, 1);
            put(&tex, head, sizeof(head));
            for (y = 0; y < images[j].h; y++) {
                s32 at = (images[j].y + y) * 512 + (images[j].bpp == 4 ? images[j].x / 2 : images[j].x);

                put(&tex, at + row <= PAGE_BYTES ? page + at : NULL, (size_t)row);
            }
        }
        if (nimage == 0) {
            /* none: one clear image, as a set has some */
            u8 head[20], clut[36];

            memset(clut, 0, sizeof(clut));
            wr32(clut, 16);
            put(&tex, clut, sizeof(clut));
            first_image = tex.n;
            memset(head, 0, sizeof(head));
            wr16(head, 1);
            wr16(head + 2, 4);
            wr16(head + 6, 4 << 8 | 2);
            wr32(head + 16, 1);
            put(&tex, head, sizeof(head));
            put(&tex, NULL, 8);
            nimage = 1;
        }
        wr16(tex.b + 8, (u32)nimage);
        wr16(tex.b + 10, (u32)nimage);
        wr32(tex.b + 0x0C, (u32)(first_image - 0x0C));
        wr32(tex.b + 0x10, 0x10);
        align(&tex, 4);
        wr32(tex.b + 4, (u32)tex.n);
    }

    /* the model: a mesh of a strip a triangle, and his four animations */
    put(&mdl, NULL, 0x58 + 0x34);
    mesh = 0x58;
    strips = mdl.n;
    for (i = 0; i < ntri; i++) {
        u8 s[2];

        wr16(s, (u32)(1 << 8 | (tris[i].image < 0)));
        put(&mdl, s, 2);
    }
    put(&mdl, "\xFF\xFF", 2);
    align(&mdl, 4);
    vblock = mdl.n;
    {
        static V3 corners[MAX_TRIS * 3];

        for (i = 0; i < ntri; i++) {
            corners[i * 3] = verts[tris[i].a];
            corners[i * 3 + 1] = verts[tris[i].b];
            corners[i * 3 + 2] = verts[tris[i].c];
        }
        put_box(&mdl, corners, ntri * 3, kscale, ground);
        for (i = 0; i < ntri * 3; i++) {
            u8 v[8];

            memset(v, 0, sizeof(v));
            wr16(v, (u32)(s16)(corners[i].x * kscale));
            wr16(v + 2, (u32)(s16)(corners[i].y * kscale - ground));
            wr16(v + 4, (u32)(s16)(corners[i].z * kscale));
            put(&mdl, v, sizeof(v));
        }
    }
    uvidx = mdl.n;
    for (i = 0, n = 0; i < ntri; i++) {
        u8 s[2];

        wr16(s, (u32)(tris[i].image >= 0 ? n : 0));
        n += tris[i].image >= 0 ? 3 : 0;
        put(&mdl, s, 2);
    }
    align(&mdl, 4);
    texs = mdl.n;
    for (i = 0; i < ntri; i++) {
        u8 s[2];

        wr16(s, (u32)(tris[i].image >= 0 ? tris[i].image : 0));
        put(&mdl, s, 2);
    }
    align(&mdl, 4);
    colidx = mdl.n;
    for (i = 0; i < ntri; i++) {
        u8 s[2];

        wr16(s, (u32)(i * 3));
        put(&mdl, s, 2);
    }
    align(&mdl, 4);
    end = mdl.n;
    /* each corner's colour, lit by the light as he stands: the game's own models have their light in their colours */
    colours = mdl.n;
    {
        float ll = sqrtf(light.x * light.x + light.y * light.y + light.z * light.z);

        for (i = 0; i < ntri; i++) {
            const s32 corner[3] = { tris[i].a, tris[i].b, tris[i].c };

            for (j = 0; j < 3; j++) {
                const V3 *nn = &normals[corner[j]];
                float len = sqrtf(nn->x * nn->x + nn->y * nn->y + nn->z * nn->z), lit = 0;
                u8 c[4];

                if (len > 0) {
                    lit = (nn->x * light.x + nn->y * light.y + nn->z * light.z) / (len * ll);
                }
                lit = 0.62f + 0.55f * (lit > 0 ? lit : 0);
                for (k = 0; k < 3; k++) {
                    float v = tris[i].rgb[k] * lit;

                    c[k] = (u8)(v > 255 ? 255 : v);
                }
                c[3] = 0;
                put(&mdl, c, 4);
            }
        }
    }
    uvs = mdl.n;
    for (i = 0; i < ntri; i++) {
        for (j = 0; tris[i].image >= 0 && j < 3; j++) {
            u8 s[2];
            s32 u = tris[i].uv[j][0], v = tris[i].uv[j][1];

            wr16(s, (u32)((u > 255 ? 255 : u) | (v > 255 ? 255 : v) << 8));
            put(&mdl, s, 2);
        }
    }
    put(&mdl, NULL, 4);
    align(&mdl, 4);
    zeros = put(&mdl, NULL, 0x18);
    count0 = put(&mdl, NULL, 4);
    entries = put(&mdl, NULL, (size_t)ROO_ANIMS * 0x18);
    kept = mdl.n;

    /* the header */
    wr32(mdl.b, 0x0C160029);
    self(&mdl, 0x08, zeros);
    self(&mdl, 0x10, zeros);
    self(&mdl, 0x18, count0);
    self(&mdl, 0x1C, count0);
    self(&mdl, 0x20, colours);
    self(&mdl, 0x24, uvs);
    self(&mdl, 0x28, zeros);
    self(&mdl, 0x2C, zeros);
    self(&mdl, 0x3C, zeros);
    wr32(mdl.b + 0x40, ROO_ANIMS);
    self(&mdl, 0x44, entries);
    self(&mdl, 0x4C, count0);
    wr32(mdl.b + 0x50, (u32)kept);
    wr32(mdl.b + 0x54, 1);
    wr16(mdl.b + mesh + 0x08, (u32)ntri);
    wr16(mdl.b + mesh + 0x0A, 4);
    self(&mdl, mesh + 0x10, vblock);
    self(&mdl, mesh + 0x14, strips);
    self(&mdl, mesh + 0x18, uvidx);
    self(&mdl, mesh + 0x1C, texs);
    self(&mdl, mesh + 0x20, colidx);
    self(&mdl, mesh + 0x24, end);
    wr32(tex.b + 0x14, (u32)kept);

    /* the animations, each in a block of its own from a 2 KB sector on, with room before it for the heap's header as it's split off */
    for (j = 0; j < ROO_ANIMS; j++) {
        static V3 fv[MAX_VERTS];
        s32 frames = anims[j].count, f, nv = 0;
        size_t start, list0, pool, e = entries + (size_t)j * 0x18;

        put(&mdl, NULL, 0x20);
        align(&mdl, 2048);
        start = put(&mdl, NULL, 4 + (size_t)frames * 16);
        list0 = mdl.n;
        for (f = 0; f < frames; f++) {
            static V3 corners[MAX_TRIS * 3];
            size_t list;

            nv = frame_verts(anims[j].item[f], anims[j].size[f], scale, fv, NULL, MAX_VERTS);
            for (i = 0; i < ntri; i++) {
                corners[i * 3] = fv[nv > tris[i].a ? tris[i].a : 0];
                corners[i * 3 + 1] = fv[nv > tris[i].b ? tris[i].b : 0];
                corners[i * 3 + 2] = fv[nv > tris[i].c ? tris[i].c : 0];
            }
            list = mdl.n;
            self(&mdl, start + 4 + (size_t)f * 16, list);
            put_box(&mdl, corners, ntri * 3, kscale, ground);
            for (i = 0; i < ntri; i++) {
                const s32 corner[3] = { tris[i].a, tris[i].b, tris[i].c };

                for (k = 0; k < 3; k++) {
                    u8 s[2];
                    s32 index = f * nvert + (corner[k] < nv ? corner[k] : 0);

                    /* the third corner says which way round the face is */
                    wr16(s, (u32)(index << 2 | (k == 2 && FLIP_FACES)));
                    put(&mdl, s, 2);
                }
            }
            align(&mdl, 4);
        }
        (void)list0;
        pool = mdl.n;
        self(&mdl, start, pool);
        for (f = 0; f < frames; f++) {
            nv = frame_verts(anims[j].item[f], anims[j].size[f], scale, fv, NULL, MAX_VERTS);
            for (k = 0; k < nvert; k++) {
                u8 v[6];
                const V3 *p = &fv[k < nv ? k : 0];

                wr16(v, (u32)(s16)(p->x * kscale));
                wr16(v + 2, (u32)(s16)(p->y * kscale - ground));
                wr16(v + 4, (u32)(s16)(p->z * kscale));
                put(&mdl, v, sizeof(v));
            }
        }
        align(&mdl, 4);
        wr32(mdl.b + e, (u32)start);
        wr32(mdl.b + e + 4, (u32)mdl.n);
        wr32(mdl.b + e + 8, (u32)frames);
        self(&mdl, e + 0x0C, mesh);
        wr32(mdl.b + e + 0x10, (u32)string_hash((u8 *)RooAnims[j].name));
    }
    if (tex.b == NULL || mdl.b == NULL || (size_t)ntri * 3 > 4096) {
        goto done;
    }
    RooTex = tex.b;
    RooTexSize = (s32)tex.n;
    RooMdl = mdl.b;
    RooMdlSize = (s32)mdl.n;
    tex.b = mdl.b = NULL;
    RooMade = 1;
    make_card(&level);
    make_laugh(&level);
    printf("bonus: Ripper Roo from Crash Bandicoot, %d triangles, %d textures, %d bytes%s%s\n", ntri, nimage, RooMdlSize,
           RooCardMade ? ", his hero card" : "", RooLaugh != NULL ? ", his laugh" : "");
done:
    free(tex.b);
    free(mdl.b);
    free(level.pages);
}

void bonus_heroes_files(void) {
    u8 *tex, *mdl;

    if (!extra_heroes_on()) {
        return;
    }
    if (!RooTried) {
        make_roo();
    }
    if (!RooMade || (tex = malloc((size_t)RooTexSize)) == NULL || (mdl = malloc((size_t)RooMdlSize)) == NULL) {
        return;
    }
    memcpy(tex, RooTex, (size_t)RooTexSize);
    memcpy(mdl, RooMdl, (size_t)RooMdlSize);
    archive_replace(BONUS_TEX, tex, RooTexSize);
    archive_replace(BONUS_MDL, mdl, RooMdlSize);
}

s32 bonus_heroes_have(s32 extra) {
    return extra == EXTRA_ROO && RooMade && extra_heroes_on();
}

u8 *bonus_heroes_model(s32 extra) {
    u8 *copy = extra == EXTRA_ROO && RooMade ? malloc((size_t)RooMdlSize) : NULL;

    if (copy != NULL) {
        memcpy(copy, RooMdl, (size_t)RooMdlSize);
    }
    return copy;
}

const u32 *bonus_heroes_card(s32 extra, s32 *w, s32 *h, float *stretch) {
    if (!bonus_heroes_have(extra) || !RooCardMade) {
        return NULL;
    }
    *w = 64;
    *h = CARD_ROWS;
    *stretch = 1.0f; /* as its pixels are: at the 1.6 its game showed it at, he looked squeezed in the panel */
    return RooCard;
}

void bonus_heroes_cheer(s32 extra) {
    /* as loud as the game's own effects at the sound option it's at (sound.c, sfx_key_on) */
    s32 vol = 0x1E00 * GameOptions.sfx * 0x7F / 409600;

    if (!bonus_heroes_have(extra) || RooLaugh == NULL) {
        return;
    }
    vol = vol < 0 ? 0 : vol > 0x7F ? 0x7F : vol;
    audio_play_sample(RooLaugh, (uint32_t)RooLaughSize, ROO_LAUGH_PITCH, 0x3FFF * vol / 0x7F, 0x3FFF * vol / 0x7F);
}

s32 bonus_heroes_bank(s32 extra) {
    return extra == EXTRA_ROO ? DB_BonusRoo : DB_Empty;
}
