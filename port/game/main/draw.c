#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "../../platform/gfx.h"

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

void tex_release_ids(obj_textureset *set) {
    s32 i;

    for (i = 0; i < MAX_IMAGES; i++) {
        if (Images[i].set == set) {
            Images[i].set = NULL;
        }
    }
    for (i = 0; i < MAX_PALETTES; i++) {
        if (Palettes[i].set == set) {
            Palettes[i].set = NULL;
        }
    }
    gfx_forget_textures();
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
    return gfx_texture_plain(TEX_ID(image, palette, t->pix + top * row), Rows, t->w, t->h, bpp, pal);
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
    if (palette != 0 && Palettes[palette].set != NULL && Palettes[palette].set->ent != NULL) {
        pal = (const u8 *)Palettes[palette].set->base + Palettes[palette].set->ent[Palettes[palette].index].off;
    }
    if (t->scrollRow != 0) {
        return scrolled_texture(t, image, palette, base, pal);
    }
    return gfx_texture(TEX_ID(image, palette, t->pix), base + t->pix, t->w, t->h, t->flag ? 8 : 4, pal);
}

/* The picture on the screen, until a scene or a viewport's background is drawn over it. */
static u8 *BackPixels;

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

void draw_viewport(s32 layer, s32 x, s32 y, s32 w, s32 h, s32 fill, u32 colour) {
    s32 key = key_of(layer);
    s32 i;

    if (key < 0) {
        return;
    }
    gfx_mark(0, 0);
    gfx_set_clip(x, y, w, h);
    if (fill) {
        GfxVert v[4];

        BackPixels = NULL;
        if (x <= 0 && x + w >= ScreenRect->w) {
            x -= gfx_view_extra();
            w += gfx_view_extra() * 2;
        }
        for (i = 0; i < 4; i++) {
            v[i].x = (float)(x + ((i & 1) ? w : 0));
            v[i].y = (float)(y + ((i & 2) ? h : 0));
            v[i].w = 1;
            v[i].u = 0;
            v[i].v = 0;
            v[i].rgba = colour_of(colour);
        }
        gfx_quad(key + 1, v, NULL, GFX_OPAQUE);
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
    t = tex_of(f->image, f->palette);
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

void draw_mesh_tri(s32 layer, const DrawFace *f, const ViewPoint q[3], s32 face, s32 wind, s32 hidden) {
    s32 key = key_of(layer);
    const GfxTex *t;
    GfxVert v[3];
    s32 i;

    if (key < 0 || (hidden && !gfx_stepping())) {
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
    gfx_mark(OwnerId != 0 ? PART_ID(OwnerId, OwnerPart) : 0, face);
    gfx_face_tri(key, v, t, blend_of(f), wind, hidden);
}

void draw_mesh_tri_plain(s32 layer, const DrawFace *f, const ViewPoint q[3], s32 face, s32 wind, s32 hidden) {
    s32 key = key_of(layer);
    GfxVert v[3];
    s32 i;

    if (key < 0 || (hidden && !gfx_stepping())) {
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
    mark_next_part();
    gfx_quad(key, v, t, blend_of(f));
}

static s32 BackW, BackH;

static void draw_back(void) {
    if (BackPixels != NULL && ScreenRect != NULL) {
        const GfxTex *t = gfx_texture(0, BackPixels, BackW, BackH, 16, NULL);
        GfxVert v[4];
        s32 i;

        if (t == NULL) {
            return;
        }
        for (i = 0; i < 4; i++) {
            v[i].x = (i & 1) ? (float)ScreenRect->w : 0;
            v[i].y = (i & 2) ? (float)ScreenRect->h : 0;
            v[i].w = 1;
            v[i].u = (i & 1) ? (float)BackW : 0;
            v[i].v = (i & 2) ? (float)BackH : 0;
            v[i].rgba = 0xFF808080;
        }
        gfx_quad(0x2000, v, t, GFX_OPAQUE);
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
