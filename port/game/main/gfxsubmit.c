#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "../../platform/gfx.h"

/* The game's drawing calls on the native renderer. The game still fills in its prim structs, but they're only data here, read as
 * they're submitted: tpage and clut hold texture and palette ids, and a uv is in the texture's own texels. There's no VRAM.
 * Meshes take their positions straight from the GTE, unrounded, so they're real 3D at any resolution. */

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

/* An id's place in a tpage: bits 5 and 6 are the blend. */
#define TPAGE_OF(id) ((((id) & 0x1F)) | (((id) >> 5) << 7))
#define ID_OF_TPAGE(tp) (((tp) & 0x1F) | (((tp) >> 7) << 5))

u16 tex_image_id(obj_textureset *set, s32 index) {
    s32 i;

    for (i = 0; i < MAX_IMAGES; i++) {
        s32 id = NextImage;
        NextImage = id + 1 >= MAX_IMAGES ? 1 : id + 1;
        if (Images[id].set == NULL) {
            Images[id].set = set;
            Images[id].index = index;
            return (u16)TPAGE_OF(id);
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
static const GfxTex *scrolled_texture(const obj_texture *t, int tpage, int clut, const u8 *base, const u8 *pal) {
    static u8 Rows[256 * 256];
    int bpp = t->flag ? 8 : 4;
    int row = t->w * bpp / 8;
    int top = t->scrollRow % t->h;
    const u8 *src = base + t->pix;

    if (row * t->h > (int)sizeof(Rows)) {
        return gfx_texture(TEX_ID(ID_OF_TPAGE(tpage & ~0x60), clut, t->pix), src, t->w, t->h, bpp, pal);
    }
    memcpy(Rows, src + top * row, (size_t)((t->h - top) * row));
    memcpy(Rows + (t->h - top) * row, src, (size_t)(top * row));
    return gfx_texture(TEX_ID(ID_OF_TPAGE(tpage & ~0x60), clut, t->pix + top * row), Rows, t->w, t->h, bpp, pal);
}

static const GfxTex *tex_of(u16 tpage, u16 clut) {
    ImageReg *im = &Images[ID_OF_TPAGE(tpage & ~0x60) & (MAX_IMAGES - 1)];
    obj_textureset *ts = im->set;
    obj_texture *t;
    const void *pal = NULL;
    const u8 *base;

    if (ts == NULL || ts->tex == NULL || ts->base == NULL) {
        return NULL;
    }
    t = &ts->tex[im->index];
    base = (const u8 *)ts->base;
    if (clut != 0 && Palettes[clut].set != NULL && Palettes[clut].set->ent != NULL) {
        pal = (const u8 *)Palettes[clut].set->base + Palettes[clut].set->ent[Palettes[clut].index].off;
    }
    if (t->scrollRow != 0) {
        return scrolled_texture(t, tpage, clut, base, pal);
    }
    return gfx_texture(TEX_ID(ID_OF_TPAGE(tpage & ~0x60), clut, t->pix), base + t->pix, t->w, t->h, t->flag ? 8 : 4, pal);
}

/* The picture on the screen, until a scene or a viewport's background is drawn over it. */
static u8 *BackPixels;

static s32 key_of(void *ot) {
    u32 *o = ot;
    s32 i;

    /* either buffer: the pause text is added between the game's frames, to the table the PS1's GPU is still walking */
    for (i = 0; i < 2; i++) {
        if (o >= DrawBuffers[i].ot && o < DrawBuffers[i].ot + 0x1000) {
            return (s32)(o - DrawBuffers[i].ot);
        }
    }
    return -1;  /* an ordering table of its own, like the hub's: not drawn yet */
}

static u32 colour_of(u32 word) {
    return (word & 0xFFFFFF) | 0xFF000000;
}

/* The GPU commands the game builds, by their first byte. */
#define CMD_NOP 0x00
#define CMD_DRAW_MODE 0xE1
#define CMD_VIEWPORT 0xFF /* not the PS1's: what SetDrawEnv leaves for prim_submit */

/* A DR_ENV, as SetDrawEnv fills it in. */
typedef struct {
    u32 tag;
    u32 cmd;
    RECT clip;
    s16 ofs[2];
    u32 bg; /* r, g, b, and in the top byte whether to fill */
} ViewportCmd;
_Static_assert(sizeof(ViewportCmd) <= sizeof(DR_ENV), "kept inside the game's DR_ENV");

/* A word holding two s16s: a position, or a width and height. */
#define WORD_X(w) ((s16)((w) & 0xFFFF))
#define WORD_Y(w) ((s16)((w) >> 16))

/* The blend of an untextured semi-transparent command: the packet's own draw mode, or else the last DR_TPAGE in draw order. */
static s32 untextured_blend(u32 cmd, s32 packet_blend) {
    if (!(cmd & 0x02000000)) {
        return GFX_OPAQUE;
    }
    return packet_blend != GFX_OPAQUE ? packet_blend : GFX_STATE_BLEND;
}

/* Polygons, 0x20-0x3F: flat or gouraud, 3 or 4 corners, plain or textured. Returns its length in words. */
static s32 draw_poly(s32 key, const u32 *p, s32 packet_blend) {
    u32 cmd = p[0];
    s32 gouraud = cmd & 0x10000000, quad = cmd & 0x08000000, textured = cmd & 0x04000000;
    s32 raw = cmd & 0x01000000;
    s32 corners = quad ? 4 : 3;
    const u32 *w = p;
    u32 colour = colour_of(raw ? 0x808080 : *w);
    u16 clut = 0, tpage = 0;
    GfxVert v[4];
    s32 i;

    w++;
    for (i = 0; i < corners; i++) {
        if (gouraud && i > 0) {
            colour = colour_of(*w++);
        }
        v[i].x = WORD_X(*w);
        v[i].y = WORD_Y(*w);
        w++;
        v[i].w = 1;
        v[i].rgba = colour;
        v[i].u = 0;
        v[i].v = 0;
        if (textured) {
            v[i].u = (float)(*w & 0xFF);
            v[i].v = (float)((*w >> 8) & 0xFF);
            if (i == 0) {
                clut = (u16)(*w >> 16);
            } else if (i == 1) {
                tpage = (u16)(*w >> 16);
            }
            w++;
        }
    }
    if (textured) {
        const GfxTex *t = tex_of(tpage, clut);
        s32 blend = (cmd & 0x02000000) ? (tpage >> 5) & 3 : GFX_OPAQUE;

        if (t != NULL) {
            if (quad) {
                gfx_quad(key, v, t, blend);
            } else {
                gfx_tri(key, v, t, blend);
            }
        }
    } else if (quad) {
        gfx_quad(key, v, NULL, untextured_blend(cmd, packet_blend));
    } else {
        gfx_tri(key, v, NULL, untextured_blend(cmd, packet_blend));
    }
    return (s32)(w - p);
}

/* Lines, 0x40-0x5F, as thin quads. Returns its length in words. */
static s32 draw_line(s32 key, const u32 *p, s32 packet_blend) {
    u32 cmd = p[0];
    s32 gouraud = cmd & 0x10000000;
    const u32 *w = p + 1;
    u32 c0 = colour_of(cmd), c1 = c0;
    s32 x0 = WORD_X(*w), y0 = WORD_Y(*w);
    s32 x1, y1, i;
    GfxVert v[4];

    w++;
    if (gouraud) {
        c1 = colour_of(*w++);
    }
    x1 = WORD_X(*w);
    y1 = WORD_Y(*w);
    w++;
    for (i = 0; i < 4; i++) {
        v[i].x = (float)((i & 1) ? x1 : x0);
        v[i].y = (float)((i & 1) ? y1 : y0) + ((i & 2) ? 1 : 0);
        v[i].w = 1;
        v[i].u = 0;
        v[i].v = 0;
        v[i].rgba = (i & 1) ? c1 : c0;
    }
    gfx_quad(key, v, NULL, untextured_blend(cmd, packet_blend));
    return (s32)(w - p);
}

/* Plain rectangles, 0x60-0x7F. Returns its length in words. */
static s32 draw_rect(s32 key, const u32 *p, s32 packet_blend) {
    static const s16 Sizes[4] = { 0, 1, 8, 16 };
    u32 cmd = p[0];
    s32 size = Sizes[(cmd >> 27) & 3];
    s32 x = WORD_X(p[1]), y = WORD_Y(p[1]);
    s32 wd = size != 0 ? size : WORD_X(p[2]);
    s32 ht = size != 0 ? size : WORD_Y(p[2]);
    GfxVert v[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        v[i].x = (float)(x + ((i & 1) ? wd : 0));
        v[i].y = (float)(y + ((i & 2) ? ht : 0));
        v[i].w = 1;
        v[i].u = 0;
        v[i].v = 0;
        v[i].rgba = colour_of(cmd);
    }
    gfx_quad(key, v, NULL, untextured_blend(cmd, packet_blend));
    return size != 0 ? 2 : 3;
}

/* A viewport: its clip, and its background if it has one, under everything else in its part of the table. */
static void set_viewport(s32 key, const ViewportCmd *c) {
    s32 x = c->clip.x - c->ofs[0], y = c->clip.y - c->ofs[1];
    s32 i;

    gfx_set_clip(x, y, c->clip.w, c->clip.h);
    if (c->bg >> 24) {
        GfxVert v[4];

        BackPixels = NULL;
        for (i = 0; i < 4; i++) {
            v[i].x = (float)(x + ((i & 1) ? c->clip.w : 0));
            v[i].y = (float)(y + ((i & 2) ? c->clip.h : 0));
            v[i].w = 1;
            v[i].u = 0;
            v[i].v = 0;
            v[i].rgba = colour_of(c->bg);
        }
        gfx_quad(key + 1, v, NULL, GFX_OPAQUE);
    }
}

/* What's drawing, to name its triangles (gfx_mark). */
static uint64_t OwnerId;
static s32 OwnerPart;

#define PART_ID(owner, part) (((owner) * 31) + (uint64_t)(part) + 1)

/* Named by what it shows too: a subengine given another model or text is a new thing, not this one moved. */
void draw_owner(const struct subengine *sub) {
    OwnerId = sub != NULL ? (uint64_t)(uintptr_t)sub ^ ((uint64_t)(uintptr_t)sub->u6C.ptr * 0x9E3779B97F4A7C15ULL) : 0;
    OwnerPart = 0;
}

/* A model draws its faces as one part. */
void draw_owner_part(void) {
    OwnerPart++;
}

/* Real depth for what's drawn next, in the current view's own range: views draw in the order of their place in the table. */
void draw_depth(s32 on) {
    gfx_set_depth(on ? 1 + key_of(RenderState.ot) * GFX_DEPTH_LAYERS / 0x1000 : 0);
}

/* Reads a packet the game built, a tag then the GPU commands its length counts, as the PS1's GPU would. */
void prim_submit(void *ot, void *prim) {
    s32 key = key_of(ot);
    const u32 *p = (const u32 *)prim + 1;
    s32 left = ((const u8 *)prim)[3];
    s32 packet_blend = GFX_OPAQUE;

    if (key < 0) {
        return;
    }
    if ((p[0] >> 24) == CMD_VIEWPORT) {
        gfx_mark(0, 0);
        set_viewport(key, prim);
        return;
    }
    OwnerPart++;
    gfx_mark(OwnerId != 0 ? PART_ID(OwnerId, OwnerPart) : 0, 0);
    do {
        u32 cmd = p[0] >> 24;
        s32 words = 1;

        if (cmd >= 0x20 && cmd < 0x40) {
            words = draw_poly(key, p, packet_blend);
        } else if (cmd >= 0x40 && cmd < 0x60) {
            words = draw_line(key, p, packet_blend);
        } else if (cmd >= 0x60 && cmd < 0x80) {
            words = draw_rect(key, p, packet_blend);
        } else if (cmd == CMD_DRAW_MODE) {
            if (left <= 1) {
                gfx_state_blend(key, (p[0] >> 5) & 3); /* a DR_TPAGE alone sets it for what draws after */
            } else {
                packet_blend = (p[0] >> 5) & 3;
            }
        }
        p += words;
        left -= words;
    } while (left > 0);
}

void SetTile1(TILE_1 *p) {
    setlen(p, 2);
    setcode(p, 0x68);
}

/* A textured mesh triangle, at the last three positions the GTE projected. */
void mesh_submit_gt3(void *ot, void *poly, s32 face) {
    POLY_GT3 *p = poly;
    s32 key = key_of(ot);
    const GfxTex *t;
    float q[9];
    GfxVert v[3];
    s32 blend;

    if (key < 0) {
        return;
    }
    t = tex_of(p->tpage, p->clut);
    if (t == NULL) {
        return;
    }
    BackPixels = NULL;
    pc_gte_precise(q);
    v[0].x = q[0]; v[0].y = q[1]; v[0].w = q[2]; v[0].u = p->u0; v[0].v = p->v0; v[0].rgba = colour_of(*(u32 *)&p->r0);
    v[1].x = q[3]; v[1].y = q[4]; v[1].w = q[5]; v[1].u = p->u1; v[1].v = p->v1; v[1].rgba = colour_of(*(u32 *)&p->r1);
    v[2].x = q[6]; v[2].y = q[7]; v[2].w = q[8]; v[2].u = p->u2; v[2].v = p->v2; v[2].rgba = colour_of(*(u32 *)&p->r2);
    blend = (((u8 *)p)[7] & 2) ? (p->tpage >> 5) & 3 : GFX_OPAQUE;
    gfx_mark(OwnerId != 0 ? PART_ID(OwnerId, OwnerPart) : 0, face);
    gfx_tri(key, v, t, blend);
}

/* An untextured mesh triangle: its blend word at +4, then a POLY_G3 at +8. */
void mesh_submit_g3(void *ot, u8 *p, s32 face) {
    s32 key = key_of(ot);
    float q[9];
    GfxVert v[3];
    s32 blend;

    if (key < 0) {
        return;
    }
    BackPixels = NULL;
    pc_gte_precise(q);
    v[0].x = q[0]; v[0].y = q[1]; v[0].w = q[2]; v[0].u = v[0].v = 0; v[0].rgba = colour_of(*(u32 *)(p + 12));
    v[1].x = q[3]; v[1].y = q[4]; v[1].w = q[5]; v[1].u = v[1].v = 0; v[1].rgba = colour_of(*(u32 *)(p + 20));
    v[2].x = q[6]; v[2].y = q[7]; v[2].w = q[8]; v[2].u = v[2].v = 0; v[2].rgba = colour_of(*(u32 *)(p + 28));
    blend = (p[15] & 2) ? (*(u32 *)(p + 4) >> 5) & 3 : GFX_OPAQUE;
    gfx_mark(OwnerId != 0 ? PART_ID(OwnerId, OwnerPart) : 0, face);
    gfx_tri(key, v, NULL, blend);
}

/* A full-screen picture, for the logos: LoadImage to a screen. */
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

s32 LoadImage(RECT *r, u32 *pixels) {
    if (ScreenRect != NULL && r != NULL && r->w == ScreenRect->w && r->h == ScreenRect->h) {
        static u8 *keep;
        static s32 keepSize;
        s32 size = r->w * r->h * 2;

        if (size > keepSize) {
            keep = realloc(keep, size);
            keepSize = size;
        }
        memcpy(keep, pixels, size);
        BackPixels = keep;
        BackW = r->w;
        BackH = r->h;
    }
    return 0;
}

s32 ClearImage(RECT *r, u8 red, u8 green, u8 blue) {
    BackPixels = NULL;
    gfx_clear(red, green, blue);
    return 0;
}

s32 ClearImage2(RECT *r, u8 red, u8 green, u8 blue) {
    return ClearImage(r, red, green, blue);
}

void DrawOTag(u32 *ot) {
    if (ot == &cdb->ot[0xFFF]) {
        draw_back();
        if (ScreenRect != NULL) {
            gfx_set_screen(ScreenRect->w, ScreenRect->h);
        }
        gfx_end_frame();
    }
}

DRAWENV *SetDefDrawEnv(DRAWENV *env, s32 x, s32 y, s32 w, s32 h) {
    env->clip.x = x;
    env->clip.y = y;
    env->clip.w = w;
    env->clip.h = h;
    env->ofs[0] = x;
    env->ofs[1] = y;
    env->isbg = 0;
    env->dfe = 0;
    env->dtd = 0;
    env->r0 = env->g0 = env->b0 = 0;
    return env;
}

/* Keeps what prim_submit needs in the DR_ENV. */
void SetDrawEnv(DR_ENV *dr, DRAWENV *env) {
    ViewportCmd *c = (ViewportCmd *)dr;

    c->cmd = (u32)CMD_VIEWPORT << 24;
    c->clip = env->clip;
    c->ofs[0] = env->ofs[0];
    c->ofs[1] = env->ofs[1];
    c->bg = env->r0 | (env->g0 << 8) | (env->b0 << 16) | ((env->isbg ? 1u : 0u) << 24);
}

DRAWENV *PutDrawEnv(DRAWENV *env) {
    return env;
}
