/* The geometry types and matrix functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBGTE_H
#define PC_LIBGTE_H

#include <pc_types.h>

typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short vx, vy; } DVECTOR;
typedef struct { u_char r, g, b, cd; } CVECTOR;

void InitGeom(void);
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
