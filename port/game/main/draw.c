#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "../../platform/gfx.h"
#include "bash/extraheroes.h"

/* The game's drawing on the native renderer. image and palette are ids of the game's own data, and a uv is in the texture's own texels. */

#define MAX_IMAGES 0x4000
#define MAX_PALETTES 0x10000

typedef struct {
    obj_textureset *set;
    s32 index;
} ImageReg;

typedef struct {
    obj_textureset *set;
    s32 index;
} PaletteReg;

/* Which tile's on the other side of each edge of a tile, found as they're drawn: the art's cut into tiles, across meshes too, and filtering
   has to blend across the cuts or they show. */
typedef struct {
    u16 image, palette;
} TileJoin;

#define JOIN_NONE 0
#define JOIN_MANY 0xFFFF
static TileJoin Joins[MAX_IMAGES][4];

static ImageReg Images[MAX_IMAGES];
static PaletteReg Palettes[MAX_PALETTES];
static s32 NextImage = 1, NextPalette = 1;

u16 tex_image_id(obj_textureset *set, s32 index) {
    s32 i;

    for (i = 0; i < MAX_IMAGES; i++) {
        s32 id = NextImage;
        NextImage = id + 1 >= MAX_IMAGES ? 1 : id + 1;
        if (Images[id].set == NULL) {
            Images[id].set = set;
            Images[id].index = index;
            return (u16)id;
        }
    }
    return 0;
}

u16 tex_palette_id(obj_textureset *set, s32 index) {
    s32 i;

    for (i = 0; i < MAX_PALETTES; i++) {
        s32 id = NextPalette;
        NextPalette = id + 1 >= MAX_PALETTES ? 1 : id + 1;
        if (Palettes[id].set == NULL) {
            Palettes[id].set = set;
            Palettes[id].index = index;
            return (u16)id;
        }
    }
    return 0;
}

/* The heroes' and bosses' icons, which the icons setting makes sharp (gfx_texture_icon): the frames of the panel's file the panel gives each hero
   (IconImage), the hero select's portraits, two of each hero, and whatever the panel's icon shows, which is how a boss's comes, from its own file
   (draw_mark_icon). Only drawn as a flat sprite (draw_face_quad): what's drawn on a model isn't an icon. */
#define NUM_ICON_IMAGES 10
#define HERO_PORTRAITS_FIRST 0x67
#define HERO_PORTRAITS_LAST 0x74
#define HERO_PORTRAITS_MORE 0x7B /* and the one after it */
#define MAX_MARKED_ICONS 16
/* in a texture's id, so its sharp copy is another texture than it */
#define ICON_ID 0x5348415250494331ULL

static struct {
    const obj_textureset *set;
    s32 index;
} MarkedIcons[MAX_MARKED_ICONS];
static s32 NumMarkedIcons;
static s32 DrawingSprite;

static s32 is_icon(const obj_textureset *ts, s32 index) {
    s32 i;

    if (ts == &HeroModelBank[DB_Interface].textures) {
        for (i = 0; i < NUM_ICON_IMAGES; i++) {
            if (IconImage[i] != 0 && (IconImage[i] & 0xFFF) == index) {
                return 1;
            }
        }
        return 0;
    }
    if (ts == &HeroModelBank[DB_MainmenuModels].textures) {
        return (index >= HERO_PORTRAITS_FIRST && index <= HERO_PORTRAITS_LAST) || index == HERO_PORTRAITS_MORE || index == HERO_PORTRAITS_MORE + 1;
    }
    for (i = 0; i < NumMarkedIcons; i++) {
        if (MarkedIcons[i].set == ts && MarkedIcons[i].index == index) {
            return 1;
        }
    }
    return 0;
}

void draw_mark_icon(const struct subengine *sub) {
    const obj_textureset *ts;
    s32 i;

    if (sub == NULL || sub->u6C.dbase == NULL || (sub->entity & 0xF000) != 0x3000) {
        return;
    }
    ts = &sub->u6C.dbase->textures;
    for (i = 0; i < NumMarkedIcons; i++) {
        if (MarkedIcons[i].set == ts && MarkedIcons[i].index == (sub->entity & 0xFFF)) {
            return;
        }
    }
    if (NumMarkedIcons < MAX_MARKED_ICONS) {
        MarkedIcons[NumMarkedIcons].set = ts;
        MarkedIcons[NumMarkedIcons].index = sub->entity & 0xFFF;
        NumMarkedIcons++;
    }
}

void tex_release_ids(obj_textureset *set) {
    s32 i;

    /* another file can come into the same place */
    for (i = 0; i < NumMarkedIcons;) {
        if (MarkedIcons[i].set == set) {
            MarkedIcons[i] = MarkedIcons[--NumMarkedIcons];
        } else {
            i++;
        }
    }
    for (i = 0; i < MAX_IMAGES; i++) {
        if (Images[i].set == set) {
            Images[i].set = NULL;
            memset(Joins[i], 0, sizeof(Joins[i]));
        }
    }
    for (i = 0; i < MAX_PALETTES; i++) {
        if (Palettes[i].set == set) {
            Palettes[i].set = NULL;
        }
    }
    gfx_forget_textures();
}

static void join_tile(s32 image, s32 side, s32 other, s32 palette) {
    TileJoin *j = &Joins[image & (MAX_IMAGES - 1)][side];

    if (j->image == JOIN_NONE) {
        j->image = (u16)other;
        j->palette = (u16)palette;
    } else if (j->image != other || j->palette != palette) {
        j->image = JOIN_MANY;
    }
}

/* The edges of the frame's faces that lie along a side of their tile, by where they are on the screen. */
#define EDGE_SLOTS 8192

typedef struct {
    s32 frame;
    s32 a[2], b[2];
    u16 image, palette;
    s16 along[2];
    s32 side;
} EdgeSeen;

static EdgeSeen Edges[EDGE_SLOTS];

/* Two faces with an edge in the same place, each on opposite sides of its tile and the same way along it: those tiles are next to each other. */
static void note_tile_edge(const DrawFace *f, const obj_texture *tile, const ViewPoint *p, const ViewPoint *q, u16 uv0, u16 uv1) {
    s32 u0 = uv0 & 0xFF, v0 = uv0 >> 8, u1 = uv1 & 0xFF, v1 = uv1 >> 8;
    s32 side, along0, along1;
    s32 a[2], b[2];
    u32 hash;
    s32 i, tries;

    if (u0 == u1 && (u0 == 0 || u0 >= tile->w - 1)) {
        side = u0 == 0 ? GFX_LEFT : GFX_RIGHT;
        along0 = v0;
        along1 = v1;
    } else if (v0 == v1 && (v0 == 0 || v0 >= tile->h - 1)) {
        side = v0 == 0 ? GFX_TOP : GFX_BOTTOM;
        along0 = u0;
        along1 = u1;
    } else {
        return;
    }
    a[0] = (s32)(p->x * 4 + 0.5f);
    a[1] = (s32)(p->y * 4 + 0.5f);
    b[0] = (s32)(q->x * 4 + 0.5f);
    b[1] = (s32)(q->y * 4 + 0.5f);
    /* the same way round from both faces */
    if (a[0] > b[0] || (a[0] == b[0] && a[1] > b[1])) {
        s32 x = a[0], y = a[1], was = along0;

        a[0] = b[0];
        a[1] = b[1];
        b[0] = x;
        b[1] = y;
        along0 = along1;
        along1 = was;
    }
    hash = ((u32)a[0] * 73856093u) ^ ((u32)a[1] * 19349663u) ^ ((u32)b[0] * 83492791u) ^ ((u32)b[1] * 50331653u);
    for (i = hash & (EDGE_SLOTS - 1), tries = 0; tries < 8; i = (i + 1) & (EDGE_SLOTS - 1), tries++) {
        EdgeSeen *s = &Edges[i];

        if (s->frame != FrameCount + 1) {
            s->frame = FrameCount + 1;
            s->a[0] = a[0];
            s->a[1] = a[1];
            s->b[0] = b[0];
            s->b[1] = b[1];
            s->image = f->image;
            s->palette = f->palette;
            s->along[0] = (s16)along0;
            s->along[1] = (s16)along1;
            s->side = side;
            return;
        }
        if (s->a[0] == a[0] && s->a[1] == a[1] && s->b[0] == b[0] && s->b[1] == b[1]) {
            if ((s->side ^ 1) == side && s->along[0] == along0 && s->along[1] == along1) {
                join_tile(s->image, s->side, f->image, f->palette);
                join_tile(f->image, side, s->image, s->palette);
            }
            return;
        }
    }
}

static void note_tile_edges(const DrawFace *f, const ViewPoint *q, s32 corners) {
    const ImageReg *im = &Images[f->image & (MAX_IMAGES - 1)];
    const obj_texture *tile;

    if (im->set == NULL || im->set->tex == NULL) {
        return;
    }
    tile = &im->set->tex[im->index];
    /* a sheet of colour swatches: its faces aren't pictures */
    if (tile->clut == 0x7FFF) {
        return;
    }
    if (corners == 3) {
        note_tile_edge(f, tile, &q[0], &q[1], f->uv[0], f->uv[1]);
        note_tile_edge(f, tile, &q[1], &q[2], f->uv[1], f->uv[2]);
        note_tile_edge(f, tile, &q[2], &q[0], f->uv[2], f->uv[0]);
    } else {
        note_tile_edge(f, tile, &q[0], &q[1], f->uv[0], f->uv[1]);
        note_tile_edge(f, tile, &q[1], &q[3], f->uv[1], f->uv[3]);
        note_tile_edge(f, tile, &q[3], &q[2], f->uv[3], f->uv[2]);
        note_tile_edge(f, tile, &q[2], &q[0], f->uv[2], f->uv[0]);
    }
}

/* What a texture is, whichever address its file has moved to: its image, palette and frame. */
#define TEX_ID(image, palette, pix) (((unsigned long long)(image) << 48) | ((unsigned long long)(palette) << 32) | (u32)(pix))

/* A scrolled texture: its rows rotated, with each position its own copy on the atlas. */
static const GfxTex *scrolled_texture(const obj_texture *t, int image, int palette, const u8 *base, const u8 *pal) {
    static u8 Rows[256 * 256];
    int bpp = t->flag ? 8 : 4;
    int row = t->w * bpp / 8;
    int top = t->scrollRow % t->h;
    const u8 *src = base + t->pix;

    if (row * t->h > (int)sizeof(Rows)) {
        return gfx_texture_plain(TEX_ID(image, palette, t->pix), src, t->w, t->h, bpp, pal);
    }
    memcpy(Rows, src + top * row, (size_t)((t->h - top) * row));
    memcpy(Rows + (t->h - top) * row, src, (size_t)(top * row));
    /* one that wraps round onto itself joins as it stands now */
    if (gfx_smooth()) {
        const TileJoin *j = Joins[image & (MAX_IMAGES - 1)];
        GfxEdge edges[4];
        s32 i, any = 0;

        memset(edges, 0, sizeof(edges));
        for (i = 0; i < 4; i++) {
            if (j[i].image == image) {
                edges[i].pixels = Rows;
                edges[i].palette = pal;
                edges[i].w = t->w;
                edges[i].h = t->h;
                edges[i].bpp = bpp;
                any = 1;
            }
        }
        if (any) {
            return gfx_texture_scrolled(TEX_ID(image, palette, t->pix + top * row) ^ 0x4A6F696E65640000ULL, Rows, src, top, t->w, t->h, bpp, pal, edges);
        }
    }
    return gfx_texture_scrolled(TEX_ID(image, palette, t->pix + top * row), Rows, src, top, t->w, t->h, bpp, pal, NULL);
}

/* The game's text, and the panel's digits: small hand-placed pixels that upscaling or filtering would spoil, so they stay as they are. */
#define PANEL_DIGITS_FIRST 0x09
#define PANEL_DIGITS_LAST 0x20

static s32 is_text(const obj_textureset *ts, s32 index) {
    return ts == &sysfont_dbase->textures ||
           (ts == &HeroModelBank[DB_Interface].textures && index >= PANEL_DIGITS_FIRST && index <= PANEL_DIGITS_LAST);
}

static const GfxTex *tex_of(u16 image, u16 palette) {
    ImageReg *im = &Images[image & (MAX_IMAGES - 1)];
    obj_textureset *ts = im->set;
    obj_texture *t;
    const void *pal = NULL;
    const u8 *base;

    if (ts == NULL || ts->tex == NULL || ts->base == NULL) {
        return NULL;
    }
    t = &ts->tex[im->index];
    base = (const u8 *)ts->base;
    /* PC: an extra hero's face on the panel, in its base's place (extraheroes.c) */
    if (ts == &HeroModelBank[DB_Interface].textures) {
        const GfxTex *face = extra_hero_face(im->index, t->w, t->h);

        if (face != NULL) {
            return face;
        }
    }
    if (palette != 0 && Palettes[palette].set != NULL && Palettes[palette].set->ent != NULL) {
        pal = (const u8 *)Palettes[palette].set->base + Palettes[palette].set->ent[Palettes[palette].index].off;
    }
    if (t->scrollRow != 0) {
        return scrolled_texture(t, image, palette, base, pal);
    }
    /* before text, as the panel's file has its eighth hero's icon among its digits */
    if (DrawingSprite && gfx_sharp_icons() && is_icon(ts, im->index)) {
        return gfx_texture_icon(TEX_ID(image, palette, t->pix) ^ ICON_ID, base + t->pix, t->w, t->h, t->flag ? 8 : 4, pal);
    }
    if (is_text(ts, im->index)) {
        return gfx_texture_sharp(TEX_ID(image, palette, t->pix), base + t->pix, t->w, t->h, t->flag ? 8 : 4, pal);
    }
    if (gfx_smooth()) {
        const TileJoin *j = Joins[image & (MAX_IMAGES - 1)];

        if (j[0].image | j[1].image | j[2].image | j[3].image) {
            GfxEdge edges[4];
            s32 i, any = 0;

            memset(edges, 0, sizeof(edges));
            for (i = 0; i < 4; i++) {
                const ImageReg *other = j[i].image != JOIN_NONE && j[i].image != JOIN_MANY ? &Images[j[i].image & (MAX_IMAGES - 1)] : NULL;
                const PaletteReg *p = &Palettes[j[i].palette];

                if (other != NULL && other->set != NULL && other->set->tex != NULL && other->set->base != NULL && p->set != NULL && p->set->ent != NULL) {
                    const obj_texture *o = &other->set->tex[other->index];

                    if (o->scrollRow == 0) {
                        edges[i].pixels = (const u8 *)other->set->base + o->pix;
                        edges[i].palette = (const u8 *)p->set->base + p->set->ent[p->index].off;
                        edges[i].w = o->w;
                        edges[i].h = o->h;
                        edges[i].bpp = o->flag ? 8 : 4;
                        any = 1;
                    }
                }
            }
            if (any) {
                /* more joins can turn up, and it's a new texture then */
                unsigned long long which = 0;

                for (i = 0; i < 4; i++) {
                    which = which * 0x10001ULL + j[i].image;
                }
                return gfx_texture_joined(TEX_ID(image, palette, t->pix) ^ (which * 0x9E3779B97F4A7C15ULL), base + t->pix, t->w, t->h, t->flag ? 8 : 4, pal, edges);
            }
        }
    }
    return gfx_texture(TEX_ID(image, palette, t->pix), base + t->pix, t->w, t->h, t->flag ? 8 : 4, pal);
}

/* A texture's texel nearest the colour of all of its texels, for a far floor that fades to its plain shade (main/widesand.c). Whether it found one. */
s32 draw_texture_plain(const obj_textureset *ts, s32 index, s32 *u, s32 *v) {
    const obj_texture *t;
    const u16 *pal;
    const u8 *px;
    s32 row, x, y, n = 0, best = 0x7FFFFFFF;
    s32 sum[3] = { 0, 0, 0 };

    if (ts == NULL || ts->tex == NULL || ts->base == NULL) {
        return 0;
    }
    t = &ts->tex[index];
    if (t->scrollRow != 0 || t->info.palette == 0 || Palettes[t->info.palette].set == NULL || Palettes[t->info.palette].set->ent == NULL) {
        return 0;
    }
    pal = (const u16 *)((const u8 *)Palettes[t->info.palette].set->base + Palettes[t->info.palette].set->ent[Palettes[t->info.palette].index].off);
    px = (const u8 *)ts->base + t->pix;
    row = t->flag ? (t->w + 1) & ~1 : (t->w + 3) / 4 * 2;
    /* twice over: the average, then the texel nearest it, leaving out clear ones */
    for (n = 0; n < 2; n++) {
        s32 count = 0;

        for (y = 0; y < t->h; y++) {
            for (x = 0; x < t->w; x++) {
                u16 c = pal[t->flag ? px[y * row + x] : (px[y * row + x / 2] >> ((x & 1) * 4)) & 15];
                s32 r = c & 31, g = (c >> 5) & 31, b = (c >> 10) & 31;

                if (c == 0) {
                    continue;
                }
                if (n == 0) {
                    sum[0] += r;
                    sum[1] += g;
                    sum[2] += b;
                    count++;
                } else {
                    s32 dr = r * 256 - sum[0], dg = g * 256 - sum[1], db = b * 256 - sum[2];
                    s32 d = dr / 16 * (dr / 16) + dg / 16 * (dg / 16) + db / 16 * (db / 16);

                    if (d < best) {
                        best = d;
                        *u = x;
                        *v = y;
                    }
                }
            }
        }
        if (n == 0) {
            if (count == 0) {
                return 0;
            }
            /* in 256ths */
            sum[0] = sum[0] * 256 / count;
            sum[1] = sum[1] * 256 / count;
            sum[2] = sum[2] * 256 / count;
        }
    }
    return 1;
}

/* The picture on the screen, until a scene or a viewport's background is drawn over it. */
static u8 *BackPixels;
static s32 BackW, BackH;

/* A wide view's side of the picture, when its edge is one colour all the way down, as a white logo's is: that edge is carried on across the side
   (draw_back). With more at its edge than that, the side is black, as round any 4:3 picture. */
static s32 back_side_flat(s32 right) {
    const u16 *px = (const u16 *)BackPixels;
    s32 x = right ? BackW - 1 : 0;
    s32 y;

    if (px == NULL || BackW <= 0 || BackH <= 0) {
        return 0;
    }
    for (y = 1; y < BackH; y++) {
        if ((px[y * BackW + x] & 0x7FFF) != (px[x] & 0x7FFF)) {
            return 0;
        }
    }
    return 1;
}

/* The one place everything goes for now, when it isn't -1 (draw_clipped_begin). */
static s32 FixedKey = -1;

static s32 key_of(s32 layer) {
    if (FixedKey >= 0) {
        return FixedKey;
    }
    return layer >= 0 && layer < 0x1000 ? layer : -1;
}

static u32 colour_of(u32 word) {
    return (word & 0xFFFFFF) | 0xFF000000;
}

static s32 blend_of(const DrawFace *f) {
    return (f->colour[0] & DRAW_BLENDS) ? f->blend : GFX_OPAQUE;
}

/* What's drawing, to name its triangles (gfx_mark). */
static uint64_t OwnerId;
static s32 OwnerPart;

static uint64_t OwnerMesh;

#define PART_ID(owner, part) (((owner) * 31) + (uint64_t)(part) + 1 + OwnerMesh * 0xC2B2AE3D27D4EB4FULL)

/* What's drawn next is a new part of what's drawing. */
static void mark_next_part(void) {
    OwnerPart++;
    gfx_mark(OwnerId != 0 ? PART_ID(OwnerId, OwnerPart) : 0, 0);
}

/* Behind every view of the frame. */
#define KEY_BACK 0x2000

static void fill_rect(s32 key, s32 x, s32 y, s32 w, s32 h, u32 colour) {
    GfxVert v[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        v[i].x = (float)(x + ((i & 1) ? w : 0));
        v[i].y = (float)(y + ((i & 2) ? h : 0));
        v[i].w = 1;
        v[i].u = 0;
        v[i].v = 0;
        v[i].rgba = colour_of(colour);
    }
    gfx_quad(key, v, NULL, GFX_OPAQUE);
}

void draw_viewport(s32 layer, s32 x, s32 y, s32 w, s32 h, s32 fill, u32 colour) {
    s32 key = key_of(layer);
    s32 whole = x <= 0 && x + w >= ScreenRect->w;
    s32 extra = whole ? gfx_view_extra() : 0;

    if (key < 0) {
        return;
    }
    gfx_mark(0, 0);
    gfx_set_clip(x, y, w, h);
    if (fill) {
        BackPixels = NULL;
        fill_rect(key + 1, x - extra, y, w + extra * 2, h, colour);
    } else if (extra > 0) {
        /* A level that fills all of a 4:3 screen doesn't clear what's behind it, so past its sides, where it can leave gaps, old pictures would stay, and what's blended there would build up. */
        fill_rect(KEY_BACK, x - extra, y, extra, h, 0);
        fill_rect(KEY_BACK, x + w, y, extra, h, 0);
    }
}

void draw_face_quad(s32 layer, const DrawFace *f, const DVECTOR xy[4]) {
    s32 key = key_of(layer);
    const GfxTex *t;
    GfxVert v[4];
    s32 i;

    if (key < 0) {
        return;
    }
    mark_next_part();
    DrawingSprite = 1;
    t = tex_of(f->image, f->palette);
    DrawingSprite = 0;
    if (t != NULL) {
        for (i = 0; i < 4; i++) {
            v[i].x = xy[i].vx;
            v[i].y = xy[i].vy;
            v[i].w = 1;
            v[i].rgba = colour_of(f->colour[i]);
            v[i].u = (float)(f->uv[i] & 0xFF);
            v[i].v = (float)(f->uv[i] >> 8);
        }
        gfx_quad(key, v, t, blend_of(f));
    }
}

void draw_plain_quad(s32 layer, const DVECTOR xy[4], const u32 colour[4], s32 blend) {
    s32 key = key_of(layer);
    GfxVert v[4];
    s32 i;

    if (key < 0) {
        return;
    }
    mark_next_part();
    for (i = 0; i < 4; i++) {
        v[i].x = xy[i].vx;
        v[i].y = xy[i].vy;
        v[i].w = 1;
        v[i].rgba = colour_of(colour[i]);
        v[i].u = 0;
        v[i].v = 0;
    }
    gfx_quad(key, v, NULL, blend);
}

/* As a thin quad. */
void draw_line(s32 layer, DVECTOR a, DVECTOR b, u32 colour_a, u32 colour_b) {
    s32 key = key_of(layer);
    GfxVert v[4];
    s32 i;

    if (key < 0) {
        return;
    }
    mark_next_part();
    for (i = 0; i < 4; i++) {
        v[i].x = (float)((i & 1) ? b.vx : a.vx);
        v[i].y = (float)((i & 1) ? b.vy : a.vy) + ((i & 2) ? 1 : 0);
        v[i].w = 1;
        v[i].u = 0;
        v[i].v = 0;
        v[i].rgba = colour_of((i & 1) ? colour_b : colour_a);
    }
    gfx_quad(key, v, NULL, GFX_OPAQUE);
}

/* Named by what it shows too: a subengine given another model or text is a new thing, not this one moved. */
void draw_owner(const struct subengine *sub) {
    OwnerId = sub != NULL ? (uint64_t)(uintptr_t)sub ^ ((uint64_t)(uintptr_t)sub->u6C.ptr * 0x9E3779B97F4A7C15ULL) : 0;
    OwnerPart = 0;
    OwnerMesh = 0;
}

/* A model draws its faces as one part, named by its mesh: frames of an animation share one, and move;
   another mesh of the same file is another thing. */
void draw_owner_part(const void *mesh) {
    OwnerPart++;
    OwnerMesh = (uint64_t)(uintptr_t)mesh;
}

/* Real depth for what's drawn next, in the current view's own range: views draw in the order of their place in the table. */
void draw_depth(s32 on) {
    gfx_set_depth(on ? 1 + key_of(RenderState.layer) * GFX_DEPTH_LAYERS / 0x1000 : 0);
}

/* What's drawn next, sorted among itself by real depth, all at one place in the frame's order and cut to a rectangle from the top
   left. */
static int ClipBefore;

void draw_clipped_begin(s32 key, s32 w, s32 h) {
    ClipBefore = gfx_current_clip();
    gfx_set_clip(0, 0, w, h);
    FixedKey = key;
    draw_depth(1);
}

/* What's drawn next goes behind everything else in its view, sorted among itself by real depth. */
void draw_behind(s32 on) {
    FixedKey = on ? RenderState.layer + RenderState.otLen - 1 : -1;
    gfx_set_depth(on ? GFX_DEPTH_LAYERS : 0);
}

void draw_clipped_end(void) {
    draw_depth(0);
    FixedKey = -1;
    gfx_use_clip(ClipBefore);
}

/* Whether three projected positions are all behind the camera, which gives them a depth of 1 and positions
   that mean nothing: as the view turns, those can sweep across the screen as one huge triangle. */
static s32 behind_camera(const ViewPoint q[3]) {
    return q[0].w <= 1 && q[1].w <= 1 && q[2].w <= 1;
}

static void mesh_vert(GfxVert *v, const ViewPoint *q, u32 colour, u16 uv) {
    v->x = q->x;
    v->y = q->y;
    v->w = q->w;
    v->u = uv & 0xFF;
    v->v = uv >> 8;
    v->rgba = colour_of(colour);
}

/* Which of the faces of what's drawn next, by their place in its mesh, as the first and last of each run: only these, or all but these. */
static const s16 (*FaceList)[2];
static s32 FaceCount, FaceKeep;

void draw_face_list(const s16 (*faces)[2], s32 count, s32 keep) {
    FaceList = faces;
    FaceCount = count;
    FaceKeep = keep;
}

static s32 face_left_out(s32 face) {
    s32 i;

    for (i = 0; i < FaceCount; i++) {
        if (face >= FaceList[i][0] && face <= FaceList[i][1]) {
            return !FaceKeep;
        }
    }
    return FaceCount != 0 && FaceKeep;
}

/* What's drawn next is a mirror image, so its faces wind the other way. */
static s32 Mirrored;

void draw_mirrored(s32 on) {
    Mirrored = on;
}

static void face_sides(s32 *wind, s32 *hidden) {
    if (Mirrored && *wind != 0) {
        *wind = -*wind;
        *hidden = !*hidden;
    }
}

void draw_mesh_tri(s32 layer, const DrawFace *f, const ViewPoint q[3], s32 face, s32 wind, s32 hidden) {
    s32 key = key_of(layer);
    const GfxTex *t;
    GfxVert v[3];
    s32 i;

    face_sides(&wind, &hidden);
    if (key < 0 || (hidden && !gfx_stepping()) || face_left_out(face)) {
        return;
    }
    t = tex_of(f->image, f->palette);
    if (t == NULL) {
        return;
    }
    BackPixels = NULL;
    if (behind_camera(q)) {
        return;
    }
    for (i = 0; i < 3; i++) {
        mesh_vert(&v[i], &q[i], f->colour[i], f->uv[i]);
    }
    if (gfx_smooth()) {
        note_tile_edges(f, q, 3);
    }
    gfx_mark(OwnerId != 0 ? PART_ID(OwnerId, OwnerPart) : 0, face);
    gfx_face_tri(key, v, t, blend_of(f), wind, hidden);
}

void draw_mesh_tri_plain(s32 layer, const DrawFace *f, const ViewPoint q[3], s32 face, s32 wind, s32 hidden) {
    s32 key = key_of(layer);
    GfxVert v[3];
    s32 i;

    face_sides(&wind, &hidden);
    if (key < 0 || (hidden && !gfx_stepping()) || face_left_out(face)) {
        return;
    }
    BackPixels = NULL;
    if (behind_camera(q)) {
        return;
    }
    for (i = 0; i < 3; i++) {
        mesh_vert(&v[i], &q[i], f->colour[i], 0);
    }
    gfx_mark(OwnerId != 0 ? PART_ID(OwnerId, OwnerPart) : 0, face);
    gfx_face_tri(key, v, NULL, blend_of(f), wind, hidden);
}

void draw_mesh_quad(s32 layer, const DrawFace *f, const ViewPoint q[4]) {
    s32 key = key_of(layer);
    const GfxTex *t;
    GfxVert v[4];
    s32 i;

    if (key < 0) {
        return;
    }
    t = tex_of(f->image, f->palette);
    if (t == NULL) {
        return;
    }
    BackPixels = NULL;
    for (i = 0; i < 4; i++) {
        /* a corner behind the camera has no position that means anything */
        if (q[i].w <= 1) {
            return;
        }
        mesh_vert(&v[i], &q[i], f->colour[i], f->uv[i]);
    }
    if (gfx_smooth()) {
        note_tile_edges(f, q, 4);
    }
    mark_next_part();
    gfx_quad(key, v, t, blend_of(f));
}

/* The picture from x0 to x1 on the screen, of its columns u0 to u1. */
static void back_quad(s32 key, const GfxTex *t, float x0, float x1, float u0, float u1) {
    GfxVert v[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        v[i].x = (i & 1) ? x1 : x0;
        v[i].y = (i & 2) ? (float)ScreenRect->h : 0;
        v[i].w = 1;
        v[i].u = (i & 1) ? u1 : u0;
        v[i].v = (i & 2) ? (float)BackH : 0;
        v[i].rgba = 0xFF808080;
    }
    gfx_quad(key, v, t, GFX_OPAQUE);
}

static void draw_back(void) {
    if (BackPixels != NULL && ScreenRect != NULL) {
        const GfxTex *t = gfx_texture(0, BackPixels, BackW, BackH, 16, NULL);
        float extra = (float)gfx_view_extra();

        if (t == NULL) {
            return;
        }
        back_quad(KEY_BACK, t, 0, (float)ScreenRect->w, 0, (float)BackW);
        /* its edge column, from the middle of it so it's that column alone, over the black a viewport puts there and behind all the rest */
        if (extra > 0 && back_side_flat(0)) {
            back_quad(KEY_BACK - 1, t, -extra, 0, 0.5f, 0.5f);
        }
        if (extra > 0 && back_side_flat(1)) {
            back_quad(KEY_BACK - 1, t, (float)ScreenRect->w, (float)ScreenRect->w + extra, (float)BackW - 0.5f, (float)BackW - 0.5f);
        }
    }
}

void draw_picture(const void *pixels) {
    static u8 *keep;
    static s32 keepSize;
    s32 size = ScreenRect->w * ScreenRect->h * 2;

    if (size > keepSize) {
        keep = realloc(keep, size);
        keepSize = size;
    }
    memcpy(keep, pixels, size);
    BackPixels = keep;
    BackW = ScreenRect->w;
    BackH = ScreenRect->h;
}

void draw_clear(void) {
    BackPixels = NULL;
    gfx_clear(0, 0, 0);
}

void draw_end_frame(void) {
    draw_back();
    if (ScreenRect != NULL) {
        gfx_set_screen(ScreenRect->w, ScreenRect->h);
    }
    gfx_end_frame();
}
