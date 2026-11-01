/* The game's drawing, as direct calls on the renderer (main/draw.c). A layer is a place in the frame's order, 0 to 0xFFF: the higher are
 * drawn first. Positions are in the screen's own pixels, and a colour is 0xBBGGRR. */
#ifndef BASH_DRAW_H
#define BASH_DRAW_H

#include <stdint.h>
#include "../../../platform/gfx.h"
#include "../../../platform/maths.h"

typedef struct { short x, y, w, h; } RECT;

/* In a face's first colour: it's drawn with its blend. */
#define DRAW_BLENDS 0x02000000

/* How a face looks, which a model keeps from frame to frame. An untextured one uses just its colours and blend. */
typedef struct DrawFace {
    uint32_t colour[4];
    uint16_t uv[4];     /* u | v << 8 */
    uint16_t image;     /* tex_image_id's */
    uint16_t palette;   /* tex_palette_id's */
    int32_t blend;      /* half, add, subtract or add a quarter: GFX_HALF and so on */
} DrawFace;

static inline void draw_face_colour(DrawFace *f, uint32_t colour) {
    f->colour[0] = colour;
    f->colour[1] = colour;
    f->colour[2] = colour;
    f->colour[3] = colour;
}

/* A quad's corners are in Z order: top left, top right, bottom left, bottom right. */
void draw_face_quad(int32_t layer, const DrawFace *f, const DVECTOR xy[4]);
/* blend is GFX_OPAQUE or one of the others. */
void draw_plain_quad(int32_t layer, const DVECTOR xy[4], const uint32_t colour[4], int32_t blend);
void draw_line(int32_t layer, DVECTOR a, DVECTOR b, uint32_t colour_a, uint32_t colour_b);

/* A model's faces, at their exact positions, so they're real 3D at any resolution. face is its place in its model, wind says which way it
   shows (gfx_face_tri), and hidden that it faces away this frame. */
void draw_mesh_tri(int32_t layer, const DrawFace *f, const ViewPoint q[3], int32_t face, int32_t wind, int32_t hidden);
void draw_mesh_tri_plain(int32_t layer, const DrawFace *f, const ViewPoint q[3], int32_t face, int32_t wind, int32_t hidden);
void draw_mesh_quad(int32_t layer, const DrawFace *f, const ViewPoint q[4]);

/* What's drawn next goes behind everything else in its view. */
void draw_behind(int32_t on);

/* What's drawn next is cut to this rectangle, with a background of the colour under it if fill is set. */
void draw_viewport(int32_t layer, int32_t x, int32_t y, int32_t w, int32_t h, int32_t fill, uint32_t colour);

/* A full-screen picture of 16-bit pixels, until a scene or a viewport's background is drawn over it: the logos. */
void draw_picture(const void *pixels);
void draw_clear(void);
void draw_end_frame(void);

#endif
