/* A level's floor carried on past where it stops, for a wide view (main/widesand.c). */
#ifndef BASH_WIDESAND_H
#define BASH_WIDESAND_H

#include "common.h"
#include "bash/main.h"

typedef struct WideSand {
    s16 piece;            /* the map placement the floor's in */
    s16 image_a, image_b; /* its tiles: the sand has the first */
    u32 rim_colour;       /* what it darkens to by the rim, as the floor does towards its own */
    u32 colour;           /* if it isn't 0, what it has a little way out from the floor's edge, rather than what that edge has: a floor that's shaded
                             dark beside something at its edge, as a road, would carry that shade all the way out */
    s16 plain_far;        /* if it isn't 0, from about this far out from the floor's edge its tile's detail fades away, to the tile's middle texel: a far
                             textured floor seen side on, as the sides of a very wide view see it, is only stripes, as the texels can't all be shown */
} WideSand;

/* Makes the sand for a level's file if it isn't made, out to the rim, a loop of x, z and the height it's to have there. Whether there's any. */
s32 wide_sand_ready(const map_enviroment *scene, s32 file, const WideSand *sand, const s16 (*rim)[3], s32 rim_points);
/* With the view of the piece it goes with. */
void wide_sand_draw(const map_enviroment *scene, const WideSand *sand, subengine *owner);
/* Its height at a place, in the piece's own coordinates. Whether it reaches there. */
s32 wide_sand_height(float x, float z, float *y);

#endif
