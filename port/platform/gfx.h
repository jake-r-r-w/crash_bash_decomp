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
enum { GFX_OPAQUE = -1, GFX_HALF = 0, GFX_ADD = 1, GFX_SUB = 2, GFX_ADD_QUARTER = 3, GFX_STATE_BLEND = 4 };

typedef struct GfxTex {
    float u0, v0, us, vs; /* where it is in the atlas: origin and scale per texel */
} GfxTex;

int gfx_init(void *window, int width, int height, int vsync);
void gfx_quit(void);

void gfx_begin_frame(void);
void gfx_end_frame(void); /* draws what was added, and shows it */
void gfx_set_clear(uint8_t r, uint8_t g, uint8_t b);

/* The screen units vertices arrive in, and a clip rect in those units for what follows. */
void gfx_set_screen(int width, int height);
void gfx_set_clip(int x, int y, int w, int h);

/* A triangle, with key the sort order: higher draws first. tex NULL is untextured. */
void gfx_tri(int key, const GfxVert v[3], const GfxTex *tex, int blend);
/* v[0] v[1] v[2] v[3] in PS1 quad order (Z), as two triangles. */
void gfx_quad(int key, const GfxVert v[4], const GfxTex *tex, int blend);

/* The blend for what draws after this at its place in the order and uses GFX_STATE_BLEND, as a DR_TPAGE sets it. */
void gfx_state_blend(int key, int blend);

/* A texture from the game's data: bpp 4 or 8 with a palette of u16 colours, or 16. Decoded on first use. */
const GfxTex *gfx_texture(const void *pixels, int width_texels, int height, int bpp, const void *palette);
/* The game freed or changed the memory in this range: forget anything decoded from it. */
void gfx_forget_textures(const void *start, const void *end);

/* Saves the next frame as a PNG. */
void gfx_screenshot(const char *path);

#endif
