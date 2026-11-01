/* The native renderer: what the game's drawing code calls (docs/PORT.md). Triangles go in with a sort key, the ordering the
 * game's buckets give them, and are drawn back to front through SDL3's GPU API. Textures are decoded from the game's own pixels
 * and palettes into one atlas. Positions are real: x and y already divided into the game's screen units, with the depth kept
 * as w, so a triangle stays perspective correct at any resolution. */
#ifndef GFX_H
#define GFX_H

#include <stdint.h>

typedef struct GfxVert {
    float x, y;    /* in screen units: the game's 640 by 480-ish space, set by gfx_set_screen */
    float w;       /* depth for perspective, 1 for 2D */
    float u, v;    /* in the texture's own texels */
    uint32_t rgba; /* 0x80 is full brightness when textured, as on the PS1 */
} GfxVert;

/* Blends, as the PS1's semi-transparency modes. */
enum { GFX_OPAQUE = -1, GFX_HALF = 0, GFX_ADD = 1, GFX_SUB = 2, GFX_ADD_QUARTER = 3 };

typedef struct GfxTex {
    float u0, v0, us, vs; /* where it is in the atlas: origin and scale per texel */
    int sharp;            /* always as it was: not upscaled, and not filtered */
} GfxTex;

/* What the renderer's asked for, from the player's settings. */
typedef struct GfxConfig {
    int width, height; /* what it draws at. 0 x 0: as sharp as the window can show */
    int vsync;
    int fps;           /* 30 draws once for each of the game's frames; higher draws between them too, through gfx_present */
    int antialias;     /* samples a pixel: 1 is off */
    int smooth;        /* textures filtered, rather than their square texels */
    int wide;          /* a wider window shows more of the world, rather than bars at the sides */
    int upscale;       /* textures made this many times bigger (upscale.c): 1 is off, or 2 or 4 */
    const char *pack;  /* the texture pack in use (texpack.c), or empty */
} GfxConfig;

int gfx_init(void *window, const GfxConfig *config);
void gfx_quit(void);

void gfx_begin_frame(void);
void gfx_end_frame(void);
void gfx_draw_every(int frames); /* a test run: draw one frame in this many */ /* what was added is the game's new frame */
void gfx_present(void);   /* draws and shows the next step towards it, at frame rates above the game's */
/* The picture stays from frame to frame, as a PS1's screen does, until the game clears it. */
void gfx_clear(uint8_t r, uint8_t g, uint8_t b);

/* The screen units vertices arrive in, and a clip rect in those units for what follows. */
void gfx_set_screen(int width, int height);
void gfx_set_clip(int x, int y, int w, int h);
/* The clip in use, to go back to it. */
int gfx_current_clip(void);
void gfx_use_clip(int clip);

/* Whether the next opaque triangles test and write real depth, between themselves, on top of the order by key. 0 is no;
 * otherwise a layer from 1 to GFX_DEPTH_LAYERS: a lower layer is always in front of a higher one, whatever their depths. */
#define GFX_DEPTH_LAYERS 64
void gfx_set_depth(int layer);
/* Names the next triangles, counting up from face, so the same ones can be found in the game's next frame and drawn moving
 * between the two. An object of 0 has no name, and just appears. */
void gfx_mark(uint64_t object, int face);

/* A triangle, with key the sort order: higher draws first. tex NULL is untextured. */
void gfx_tri(int key, const GfxVert v[3], const GfxTex *tex, int blend);
/* A model's face, which shows only one way round: wind 1 when its area is positive, -1 when it isn't, 0 either way.
 * hidden says it faces away in this frame. It still comes, because the pictures between two frames decide for
 * themselves: a face dropped at once would leave a hole where its neighbours haven't arrived yet. */
void gfx_face_tri(int key, const GfxVert v[3], const GfxTex *tex, int blend, int wind, int hidden);
/* Whether pictures are drawn between the game's frames. */
int gfx_stepping(void);
/* The window has a new size: the picture's redrawn at it, if that's what the settings ask. */
void gfx_window_resized(void);
/* v[0] v[1] v[2] v[3] in PS1 quad order (Z), as two triangles. */
void gfx_quad(int key, const GfxVert v[4], const GfxTex *tex, int blend);

/* A texture that isn't upscaled, for one that's made again as it moves. Otherwise as gfx_texture. */
const GfxTex *gfx_texture_plain(uint64_t id, const void *pixels, int width_texels, int height, int bpp, const void *palette);
/* A texture with its rows rotated by top, so made again as it moves: source is how it is at rest, which a texture pack replaces. */
const GfxTex *gfx_texture_scrolled(uint64_t id, const void *pixels, const void *source, int top, int width_texels, int height, int bpp, const void *palette);
/* One that's never upscaled or filtered, whatever the settings: text, whose small hand-placed pixels those would spoil. */
const GfxTex *gfx_texture_sharp(uint64_t id, const void *pixels, int width_texels, int height, int bpp, const void *palette);

/* A texture from the game's data: bpp 4 or 8 with a palette of u16 colours, or 16. Decoded on first use. id says which texture
 * this is, to find it again without looking at its pixels, until gfx_forget_textures; 0 for one that has no lasting id. */
/* Changed while running. */
void gfx_configure(const GfxConfig *config);

/* How far past each side of the game's screen the picture goes, in its units: 0 at 4:3. */
int gfx_view_extra(void);

const GfxTex *gfx_texture(uint64_t id, const void *pixels, int width_texels, int height, int bpp, const void *palette);
/* The ids are no longer good. What's decoded stays, for when the same pixels come back. */
void gfx_forget_textures(void);

/* Saves the next frame as a PNG. */
void gfx_screenshot(const char *path);

#endif
