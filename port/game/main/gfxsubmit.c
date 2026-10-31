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
    return gfx_texture(base + t->pix, t->w, t->h, t->flag ? 8 : 4, pal);
}

/* The picture on the screen, until a scene or a viewport's background is drawn over it. */
static u8 *BackPixels;

static s32 key_of(void *ot) {
    u32 *o = ot;

    if (o >= cdb->ot && o < cdb->ot + 0x1000) {
        return (s32)(o - cdb->ot);
    }
    return -1;  /* an ordering table of its own, like the hub's: not drawn yet */
}

static u32 colour_of(u32 word) {
    return (word & 0xFFFFFF) | 0xFF000000;
}

/* Reads one of the PS1's drawing prims. */
void prim_submit(void *ot, void *prim) {
    s32 key = key_of(ot);
    u32 *w = prim;
    u32 code;

    if (key < 0) {
        return;
    }
    code = ((u8 *)prim)[7];
    if ((code & 0xE0) == 0x20) {
        s32 gouraud = code & 0x10, quad = code & 8, textured = code & 4, semi = code & 2, raw = code & 1;
        s32 n = quad ? 4 : 3;
        s32 i;
        u32 *p = w + 1;
        u32 c = colour_of(*p++);
        u16 clut = 0, tpage = 0;
        GfxVert v[4];

        if (raw) {
            c = 0xFF808080;
        }
        for (i = 0; i < n; i++) {
            if (gouraud && i > 0) {
                c = colour_of(*p++);
            }
            v[i].x = (s16)(*p & 0xFFFF);
            v[i].y = (s16)(*p >> 16);
            p++;
            v[i].w = 1;
            v[i].rgba = c;
            v[i].u = 0;
            v[i].v = 0;
            if (textured) {
                v[i].u = (float)(*p & 0xFF);
                v[i].v = (float)((*p >> 8) & 0xFF);
                if (i == 0) {
                    clut = (u16)(*p >> 16);
                } else if (i == 1) {
                    tpage = (u16)(*p >> 16);
                }
                p++;
            }
        }
        if (textured) {
            const GfxTex *t = tex_of(tpage, clut);
            s32 blend = semi ? (tpage >> 5) & 3 : GFX_OPAQUE;
            if (t == NULL) {
                return;
            }
            if (quad) {
                gfx_quad(key, v, t, blend);
            } else {
                gfx_tri(key, v, t, blend);
            }
        } else {
            s32 blend = semi ? GFX_STATE_BLEND : GFX_OPAQUE;
            if (quad) {
                gfx_quad(key, v, NULL, blend);
            } else {
                gfx_tri(key, v, NULL, blend);
            }
        }
    } else if ((code & 0xE0) == 0x60) {
        /* rectangles: TILE and SPRT */
        s32 semi = code & 2, textured = code & 4;
        s32 size = (code >> 3) & 3;
        u32 *p = w + 1;
        u32 c = colour_of(*p++);
        s32 x, y, wd, ht;
        GfxVert v[4];
        s32 i;
        u32 uvw = 0;

        x = (s16)(*p & 0xFFFF);
        y = (s16)(*p >> 16);
        p++;
        if (textured) {
            uvw = *p++;
        }
        if (size == 0) {
            wd = (s16)(*p & 0xFFFF);
            ht = (s16)(*p >> 16);
        } else {
            wd = ht = size == 1 ? 1 : size == 2 ? 8 : 16;
        }
        for (i = 0; i < 4; i++) {
            v[i].x = (float)(x + ((i & 1) ? wd : 0));
            v[i].y = (float)(y + ((i & 2) ? ht : 0));
            v[i].w = 1;
            v[i].rgba = c;
            v[i].u = (float)((uvw & 0xFF) + ((i & 1) ? wd : 0));
            v[i].v = (float)(((uvw >> 8) & 0xFF) + ((i & 2) ? ht : 0));
        }
        if (textured) {
            return;  /* SPRT's texture comes from a DR_TPAGE the game doesn't use for it yet */
        }
        gfx_quad(key, v, NULL, semi ? GFX_STATE_BLEND : GFX_OPAQUE);
    } else if ((code & 0xE0) == 0x40) {
        /* lines, as thin quads */
        s32 gouraud = code & 0x10, semi = code & 2;
        u32 *p = w + 1;
        u32 c0 = colour_of(*p++), c1;
        s32 x0, y0, x1, y1;
        GfxVert v[4];

        x0 = (s16)(*p & 0xFFFF);
        y0 = (s16)(*p >> 16);
        p++;
        c1 = gouraud ? colour_of(*p++) : c0;
        x1 = (s16)(*p & 0xFFFF);
        y1 = (s16)(*p >> 16);
        v[0].x = (float)x0; v[0].y = (float)y0; v[0].rgba = c0;
        v[1].x = (float)x1; v[1].y = (float)y1; v[1].rgba = c1;
        v[2].x = (float)x0; v[2].y = (float)y0 + 1; v[2].rgba = c0;
        v[3].x = (float)x1; v[3].y = (float)y1 + 1; v[3].rgba = c1;
        v[0].w = v[1].w = v[2].w = v[3].w = 1;
        v[0].u = v[0].v = v[1].u = v[1].v = v[2].u = v[2].v = v[3].u = v[3].v = 0;
        gfx_quad(key, v, NULL, semi ? GFX_STATE_BLEND : GFX_OPAQUE);
    } else if (code == 0xE1) {
        /* DR_TPAGE: the blend for untextured semi-transparent prims */
        gfx_state_blend(key, (w[1] >> 5) & 3);
    } else if (code == 0xE3) {
        /* SetDrawEnv's: a viewport's clip, and maybe its background */
        s32 cx = (s16)(w[2] & 0xFFFF), cy = (s16)(w[2] >> 16);
        s32 cw = (s16)(w[3] & 0xFFFF), ch = (s16)(w[3] >> 16);
        s32 ox = (s16)(w[4] & 0xFFFF), oy = (s16)(w[4] >> 16);

        gfx_set_clip(cx - ox, cy - oy, cw, ch);
        if (w[5] & 0x01000000) {
            GfxVert v[4];

            BackPixels = NULL;
            s32 i;

            for (i = 0; i < 4; i++) {
                v[i].x = (float)(cx - ox + ((i & 1) ? cw : 0));
                v[i].y = (float)(cy - oy + ((i & 2) ? ch : 0));
                v[i].w = 1;
                v[i].u = v[i].v = 0;
                v[i].rgba = colour_of(w[5]);
            }
            gfx_quad(key + 1, v, NULL, GFX_OPAQUE);
        }
    }
}

/* A textured mesh triangle, at the last three positions the GTE projected. */
void mesh_submit_gt3(void *ot, void *poly) {
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
    gfx_tri(key, v, t, blend);
}

/* An untextured mesh triangle: its blend word at +4, then a POLY_G3 at +8. */
void mesh_submit_g3(void *ot, u8 *p) {
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
    gfx_tri(key, v, NULL, blend);
}

/* A full-screen picture, for the logos: LoadImage to a screen. */
static s32 BackW, BackH;

static void draw_back(void) {
    if (BackPixels != NULL && ScreenRect != NULL) {
        const GfxTex *t = gfx_texture(BackPixels, BackW, BackH, 16, NULL);
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
        gfx_forget_textures(NULL, NULL);
        BackPixels = keep;
        BackW = r->w;
        BackH = r->h;
    }
    return 0;
}

s32 ClearImage(RECT *r, u8 red, u8 green, u8 blue) {
    BackPixels = NULL;
    gfx_set_clear(red, green, blue);
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
    u32 *w = (u32 *)dr;

    w[1] = 0xE3000000;
    w[2] = (env->clip.x & 0xFFFF) | ((u32)env->clip.y << 16);
    w[3] = (env->clip.w & 0xFFFF) | ((u32)env->clip.h << 16);
    w[4] = (env->ofs[0] & 0xFFFF) | ((u32)env->ofs[1] << 16);
    w[5] = env->r0 | (env->g0 << 8) | (env->b0 << 16) | ((env->isbg ? 1u : 0u) << 24);
}

DRAWENV *PutDrawEnv(DRAWENV *env) {
    return env;
}
