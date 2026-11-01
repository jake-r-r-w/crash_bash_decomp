/* The game's maths: its fixed-point geometry, as plain functions (maths.c). The rules are the PS1's, saturation and rounding included,
 * as the game's logic is built on the results. */
#ifndef PLATFORM_MATHS_H
#define PLATFORM_MATHS_H

#include <stdint.h>

typedef struct { short m[3][3]; int32_t t[3]; } MATRIX;
typedef struct { int32_t vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short vx, vy; } DVECTOR;
typedef struct { uint8_t r, g, b, cd; } CVECTOR;

/* The view points are projected through. */
void view_set_centre(int x, int y);
void view_set_distance(int distance);
void view_set_rotation(const void *matrix);
void view_set_translation(const void *matrix);
void view_set_translation_vector(const void *vector);

/* A point after projecting: on the PS1's screen, rounded as it was; in the view; and exact, for drawing. */
typedef struct ViewPoint {
    int16_t sx, sy;
    uint16_t sz;
    int32_t view[3];
    float x, y, w;
} ViewPoint;

/* The points are all three shorts, whatever they're called where they come from. */
void project_point(const void *point, ViewPoint *out);
/* Just the screen position, as the two shorts it's stored in. */
void project_xy(const void *point, void *xy);
void project_xyz(const void *point, void *xy, void *depth);
void project_view_xy(const void *point, VECTOR *view, void *xy);
/* Three points in a row, with their positions and depths stored in a row. */
void project_three(const void *points, void *xys, uint32_t *depths);
void point_store_xy(const ViewPoint *p, void *xy);
/* A triangle's place in the drawing order, and which way it faces: positive one way, negative the other. */
uint32_t points_order(const ViewPoint *a, const ViewPoint *b, const ViewPoint *c);
int32_t points_facing(const ViewPoint *a, const ViewPoint *b, const ViewPoint *c);
/* A point turned and moved by the view, not projected. */
void view_transform(const void *point, VECTOR *out);

/* A colour word (r, g, b, its code) faded towards a colour by amount of 0x1000. */
uint32_t colour_fade(const void *colour, const int32_t to[3], int amount);
/* A vector moved towards (r, g, b) by amount of 0x1000. */
void vector_towards(const void *from, int r, int g, int b, int amount, void *out);

void maths_init(void);
int csqrt(int a);
VECTOR *ApplyMatrixLV(MATRIX *m, VECTOR *in, VECTOR *out);
void MatrixNormal_1(MATRIX *m, MATRIX *n);
MATRIX *MulMatrix(MATRIX *m0, MATRIX *m1);
MATRIX *MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *out);
MATRIX *RotMatrix(SVECTOR *r, MATRIX *m);
MATRIX *RotMatrixYXZ(SVECTOR *r, MATRIX *m);
MATRIX *RotMatrixZYX(SVECTOR *r, MATRIX *m);
MATRIX *ScaleMatrixL(MATRIX *m, VECTOR *v);

#endif
