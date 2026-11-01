/* The game's fixed-point geometry as plain functions, with the PS1's rules (saturation, the 32-bit middle values, the division's rounding)
 * and its matrix functions' own order of multiplies and shifts, so results match bit for bit. */
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include "maths.h"

/* The sine and cosine table, { sin, cos } for 4096 angles, and the normalising table. Both are exactly these formulas: checked against
 * the tables in a retail exe. */
short rcossin_tbl[4096 * 2];
static short InvSqrtTbl[192];

/* How a triangle's three depths make its place in the order: a third, in 12 bits */
#define ORDER_SCALE 0x155

static int16_t Rot[3][3];
static int32_t Trans[3];
static int32_t CentreX, CentreY, Distance = 1000;

static int32_t saturate(int32_t v) {
    return v < -0x8000 ? -0x8000 : v > 0x7FFF ? 0x7FFF : v;
}

/* (x, y, z) through the rotation, and the translation if there's one to add: the 32-bit results, and each saturated to 16 bits. */
static void rotate(const int32_t *t, int x, int y, int z, int shift12, int32_t *mac, int32_t *ir) {
    int i;

    for (i = 0; i < 3; i++) {
        int64_t s = (t != NULL) ? ((int64_t)t[i] << 12) : 0;

        s += (int64_t)Rot[i][0] * x + (int64_t)Rot[i][1] * y + (int64_t)Rot[i][2] * z;
        mac[i] = (int32_t)(s >> (shift12 * 12));
        ir[i] = saturate(mac[i]);
    }
}

/* distance / z by the hardware's table and Newton steps, or saturated. */
static uint32_t perspective_divide(uint32_t h, uint32_t z) {
    static uint8_t unr[257];
    static int have;
    uint32_t n, d, u;
    int lz;

    if (!have) {
        int i;

        for (i = 0; i < 257; i++) {
            int v = ((0x40000 / (i + 0x100) + 1) / 2) - 0x101;

            unr[i] = v < 0 ? 0 : v;
        }
        have = 1;
    }
    if (h >= z * 2) {
        return 0x1FFFF;
    }
    lz = 0;
    while (((z << lz) & 0x8000) == 0) {
        lz++;
    }
    n = h << lz;
    d = z << lz;
    u = unr[(d - 0x7FC0) >> 7] + 0x101;
    d = (0x2000080 - d * u) >> 8;
    d = (0x0000080 + d * u) >> 8;
    n = (uint32_t)(((uint64_t)n * d + 0x8000) >> 16);
    return n > 0x1FFFF ? 0x1FFFF : n;
}

void view_set_centre(int x, int y) {
    CentreX = x << 16;
    CentreY = y << 16;
}

void view_set_distance(int distance) {
    Distance = distance & 0xFFFF;
}

void view_set_rotation(const void *matrix) {
    const MATRIX *a = matrix;
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            Rot[i][j] = a->m[i][j];
        }
    }
}

void view_set_translation(const void *matrix) {
    const MATRIX *a = matrix;

    Trans[0] = a->t[0];
    Trans[1] = a->t[1];
    Trans[2] = a->t[2];
}

void view_set_translation_vector(const void *vector) {
    const VECTOR *a = vector;

    Trans[0] = a->vx;
    Trans[1] = a->vy;
    Trans[2] = a->vz;
}

void project_point(const void *point, ViewPoint *out) {
    const SVECTOR *v = point;
    int32_t mac[3], ir[3], z;
    int64_t sx, sy;
    uint32_t n;
    float d;

    rotate(Trans, v->vx, v->vy, v->vz, 1, mac, ir);
    z = mac[2];
    out->sz = z < 0 ? 0 : z > 0xFFFF ? 0xFFFF : z;
    n = perspective_divide((uint32_t)Distance, out->sz);
    sx = (int64_t)n * ir[0] + CentreX;
    sy = (int64_t)n * ir[1] + CentreY;
    {
        int32_t x = (int32_t)(sx >> 16);
        int32_t y = (int32_t)(sy >> 16);

        out->sx = x < -0x400 ? -0x400 : x > 0x3FF ? 0x3FF : x;
        out->sy = y < -0x400 ? -0x400 : y > 0x3FF ? 0x3FF : y;
    }
    out->view[0] = ir[0];
    out->view[1] = ir[1];
    out->view[2] = ir[2];
    d = z < 1 ? 1.0f : (float)z;
    out->x = (float)CentreX / 65536.0f + (float)Distance * (float)ir[0] / d;
    out->y = (float)CentreY / 65536.0f + (float)Distance * (float)ir[1] / d;
    out->w = d;
}

void point_store_xy(const ViewPoint *p, void *xy) {
    *(uint32_t *)xy = (uint16_t)p->sx | ((uint32_t)(uint16_t)p->sy << 16);
}

void project_xy(const void *point, void *xy) {
    ViewPoint p;

    project_point(point, &p);
    point_store_xy(&p, xy);
}

void project_xyz(const void *point, void *xy, void *depth) {
    ViewPoint p;

    project_point(point, &p);
    point_store_xy(&p, xy);
    *(uint32_t *)depth = p.sz;
}

void project_view_xy(const void *point, VECTOR *view, void *xy) {
    ViewPoint p;

    project_point(point, &p);
    view->vx = p.view[0];
    view->vy = p.view[1];
    view->vz = p.view[2];
    point_store_xy(&p, xy);
}

void project_three(const void *points, void *xys, uint32_t *depths) {
    const SVECTOR *v = points;
    uint32_t *out = xys;
    int i;

    for (i = 0; i < 3; i++) {
        ViewPoint p;

        project_point(&v[i], &p);
        point_store_xy(&p, &out[i]);
        depths[i] = p.sz;
    }
}

uint32_t points_order(const ViewPoint *a, const ViewPoint *b, const ViewPoint *c) {
    int32_t v = (ORDER_SCALE * ((int32_t)a->sz + b->sz + c->sz)) >> 12;

    return v < 0 ? 0 : v > 0xFFFF ? 0xFFFF : v;
}

/* From the exact positions they're drawn with: rounded to the PS1's screen, a thin triangle of a small model has no area and would be
   dropped, leaving a hole. */
int32_t points_facing(const ViewPoint *a, const ViewPoint *b, const ViewPoint *c) {
    float area = (b->x - a->x) * (c->y - a->y) - (c->x - a->x) * (b->y - a->y);

    return area > 0 ? (area < 1 ? 1 : (int32_t)area) : area < 0 ? (area > -1 ? -1 : (int32_t)area) : 0;
}

void view_transform(const void *point, VECTOR *out) {
    const SVECTOR *v = point;
    int32_t mac[3], ir[3];

    rotate(Trans, v->vx, v->vy, v->vz, 1, mac, ir);
    out->vx = ir[0];
    out->vy = ir[1];
    out->vz = ir[2];
}

/* from + (target - from) * amount, in the 32-bit and saturated steps the fade has always taken */
static void towards(const int32_t from[3], const int32_t target[3], int amount, int32_t *mac, int32_t *ir) {
    int i;

    for (i = 0; i < 3; i++) {
        ir[i] = saturate((int32_t)((((int64_t)target[i] << 12) - from[i]) >> 12));
    }
    for (i = 0; i < 3; i++) {
        mac[i] = (int32_t)(((int64_t)ir[i] * amount + from[i]) >> 12);
        ir[i] = saturate(mac[i]);
    }
}

uint32_t colour_fade(const void *colour, const int32_t to[3], int amount) {
    const unsigned char *c = colour;
    int32_t from[3], mac[3], ir[3];
    uint32_t out = (uint32_t)c[3] << 24;
    int i;

    for (i = 0; i < 3; i++) {
        from[i] = (int32_t)c[i] << 16;
    }
    towards(from, to, amount, mac, ir);
    for (i = 0; i < 3; i++) {
        int32_t v = mac[i] >> 4;

        v = v < 0 ? 0 : v > 255 ? 255 : v;
        out |= (uint32_t)v << (i * 8);
    }
    return out;
}

void vector_towards(const void *from, int r, int g, int b, int amount, void *out) {
    const SVECTOR *f = from;
    int16_t *o = out;
    int32_t start[3], target[3], mac[3], ir[3];

    start[0] = f->vx * 4096;
    start[1] = f->vy * 4096;
    start[2] = f->vz * 4096;
    target[0] = r;
    target[1] = g;
    target[2] = b;
    towards(start, target, amount, mac, ir);
    o[0] = (int16_t)ir[0];
    o[1] = (int16_t)ir[1];
    o[2] = (int16_t)ir[2];
}

void maths_init(void) {
    int i;

    for (i = 0; i < 4096; i++) {
        double a = i * 3.14159265358979323846 * 2 / 4096;

        rcossin_tbl[i * 2] = (short)floor(sin(a) * 4096 + 0.5);
        rcossin_tbl[i * 2 + 1] = (short)floor(cos(a) * 4096 + 0.5);
    }
    for (i = 0; i < 192; i++) {
        InvSqrtTbl[i] = (short)floor(4096 * sqrt(64.0 / (i + 64)));
    }
}

/* Leading zeros of a non-negative value, leading ones of a negative one. */
static int lzc(int32_t a) {
    uint32_t v = a < 0 ? ~(uint32_t)a : (uint32_t)a;
    int n = 0;

    if (v == 0) {
        return 32;
    }
    while (!(v & 0x80000000u)) {
        v <<= 1;
        n++;
    }
    return n;
}

/* csqrt's CORDIC, six steps with the fourth done twice. */
static int32_t csqrt_steps(int32_t a) {
    int32_t x = a + 0x5D50AD;
    int32_t y = a - 0x5D50AD;
    int i;

    for (i = 1; i < 7; i++) {
        int n = (i == 4) ? 2 : 1;

        while (n-- > 0) {
            int32_t nx, ny;

            if (y >= 0) {
                nx = x - (y >> i);
                ny = y - (x >> i);
            } else {
                nx = x + (y >> i);
                ny = y + (x >> i);
            }
            x = nx;
            y = ny;
        }
    }
    return x;
}

int csqrt(int a) {
    int d, n;
    int32_t x, r;

    if (a == 0) {
        return 0;
    }
    d = 8 - lzc(a);
    if (d >= 0) {
        n = d >> 1;
        x = a >> (n * 2);
    } else {
        n = (d >> 1) + 1;
        x = (int32_t)((uint32_t)a << (-(n * 2)));
    }
    n -= 6;
    r = csqrt_steps(x);
    return n < 0 ? r >> -n : (int32_t)((uint32_t)r << n);
}

#define MUL12(a, b) ((int32_t)((uint32_t)(a) * (uint32_t)(b)) >> 12)

static void angle(int a, int32_t *s, int32_t *c) {
    a = (short)a;
    if (a < 0) {
        a = -a & 0xFFF;
        *s = -rcossin_tbl[a * 2];
    } else {
        a &= 0xFFF;
        *s = rcossin_tbl[a * 2];
    }
    *c = rcossin_tbl[a * 2 + 1];
}

MATRIX *RotMatrix(SVECTOR *r, MATRIX *m) {
    int32_t sx, cx, sy, cy, sz, cz, t;

    angle(r->vx, &sx, &cx);
    angle(r->vy, &sy, &cy);
    angle(r->vz, &sz, &cz);
    m->m[0][2] = sy;
    m->m[1][2] = (int32_t)(0u - (uint32_t)cy * (uint32_t)sx) >> 12;
    m->m[2][2] = MUL12(cy, cx);
    m->m[0][0] = MUL12(cz, cy);
    m->m[0][1] = (int32_t)(0u - (uint32_t)sz * (uint32_t)cy) >> 12;
    t = MUL12(cz, -sy);
    m->m[1][0] = MUL12(sz, cx) - MUL12(t, sx);
    m->m[2][0] = MUL12(sz, sx) + MUL12(t, cx);
    t = MUL12(sz, -sy);
    m->m[1][1] = MUL12(cz, cx) + MUL12(t, sx);
    m->m[2][1] = MUL12(cz, sx) - MUL12(t, cx);
    return m;
}

MATRIX *RotMatrixYXZ(SVECTOR *r, MATRIX *m) {
    int32_t sx, cx, sy, cy, sz, cz, t;

    angle(r->vx, &sx, &cx);
    angle(r->vy, &sy, &cy);
    angle(r->vz, &sz, &cz);
    m->m[1][2] = -sx;
    m->m[0][2] = MUL12(sy, cx);
    m->m[2][2] = MUL12(cy, cx);
    m->m[1][0] = MUL12(sz, cx);
    m->m[1][1] = MUL12(cz, cx);
    t = MUL12(sy, sx);
    m->m[0][0] = MUL12(t, sz) + MUL12(cy, cz);
    m->m[0][1] = MUL12(t, cz) - MUL12(cy, sz);
    t = MUL12(cy, sx);
    m->m[2][1] = MUL12(t, cz) + MUL12(sy, sz);
    m->m[2][0] = MUL12(t, sz) - MUL12(sy, cz);
    return m;
}

MATRIX *RotMatrixZYX(SVECTOR *r, MATRIX *m) {
    int32_t sx, cx, sy, cy, sz, cz, t;

    angle(r->vx, &sx, &cx);
    angle(r->vy, &sy, &cy);
    angle(r->vz, &sz, &cz);
    m->m[2][0] = -sy;
    m->m[2][1] = MUL12(sx, cy);
    m->m[2][2] = MUL12(cx, cy);
    m->m[0][0] = MUL12(cy, cz);
    m->m[1][0] = MUL12(sz, cy);
    t = MUL12(sx, sy);
    m->m[0][1] = MUL12(t, cz) - MUL12(sz, cx);
    m->m[1][1] = MUL12(t, sz) + MUL12(cx, cz);
    t = MUL12(sy, cx);
    m->m[0][2] = MUL12(t, cz) + MUL12(sx, sz);
    m->m[1][2] = MUL12(t, sz) - MUL12(sx, cz);
    return m;
}

MATRIX *ScaleMatrixL(MATRIX *m, VECTOR *v) {
    int i, j;
    int32_t s[3];

    s[0] = v->vx;
    s[1] = v->vy;
    s[2] = v->vz;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            m->m[i][j] = MUL12(m->m[i][j], s[i]);
        }
    }
    return m;
}

/* Leaves the view's rotation as m0, which the game's drawing has come to count on. */
MATRIX *MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *out) {
    int16_t r[3][3];
    int i, j;

    view_set_rotation(m0);
    for (j = 0; j < 3; j++) {
        int32_t mac[3], ir[3];

        rotate(NULL, m1->m[0][j], m1->m[1][j], m1->m[2][j], 1, mac, ir);
        for (i = 0; i < 3; i++) {
            r[i][j] = (int16_t)ir[i];
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            out->m[i][j] = r[i][j];
        }
    }
    return out;
}

MATRIX *MulMatrix(MATRIX *m0, MATRIX *m1) {
    return MulMatrix0(m0, m1, m0);
}

VECTOR *ApplyMatrixLV(MATRIX *m, VECTOR *in, VECTOR *out) {
    int32_t v[3], hi[3], lo[3], rh[3], mac[3], ir[3];
    int i;

    view_set_rotation(m);
    v[0] = in->vx;
    v[1] = in->vy;
    v[2] = in->vz;
    for (i = 0; i < 3; i++) {
        int32_t a = v[i] < 0 ? -v[i] : v[i];

        hi[i] = a >> 15;
        lo[i] = a & 0x7FFF;
        if (v[i] < 0) {
            hi[i] = -hi[i];
            lo[i] = -lo[i];
        }
    }
    rotate(NULL, (int16_t)hi[0], (int16_t)hi[1], (int16_t)hi[2], 0, rh, ir);
    rotate(NULL, (int16_t)lo[0], (int16_t)lo[1], (int16_t)lo[2], 1, mac, ir);
    out->vx = (int32_t)((uint32_t)rh[0] << 3) + mac[0];
    out->vy = (int32_t)((uint32_t)rh[1] << 3) + mac[1];
    out->vz = (int32_t)((uint32_t)rh[2] << 3) + mac[2];
    return out;
}

/* OP with sf set: the cross product, >> 12, of two vectors by their low halves. */
static void outer12(const int32_t *a, const int32_t *b, int32_t *r) {
    int32_t d0 = (int16_t)a[0], d1 = (int16_t)a[1], d2 = (int16_t)a[2];
    int32_t i0 = (int16_t)b[0], i1 = (int16_t)b[1], i2 = (int16_t)b[2];

    r[0] = (int32_t)(((int64_t)i2 * d1 - (int64_t)i1 * d2) >> 12);
    r[1] = (int32_t)(((int64_t)i0 * d2 - (int64_t)i2 * d0) >> 12);
    r[2] = (int32_t)(((int64_t)i1 * d0 - (int64_t)i0 * d1) >> 12);
}

static void vector_normal(int32_t *v) {
    int32_t i0 = (int16_t)v[0], i1 = (int16_t)v[1], i2 = (int16_t)v[2];
    int32_t l = i0 * i0 + i1 * i1 + i2 * i2;
    int lz = lzc(l) & ~1;
    int sh = (31 - lz) >> 1;
    int32_t x, h;

    if (l == 0) {
        v[0] = v[1] = v[2] = 0;
        return;
    }
    x = lz >= 24 ? (int32_t)((uint32_t)l << (lz - 24)) : l >> (24 - lz);
    h = InvSqrtTbl[x - 64];
    v[0] = (h * i0) >> sh;
    v[1] = (h * i1) >> sh;
    v[2] = (h * i2) >> sh;
}

void MatrixNormal_1(MATRIX *m, MATRIX *n) {
    int32_t a[3], b[3], c[3];
    int i;

    b[0] = m->m[1][0];
    b[1] = m->m[1][1];
    b[2] = m->m[1][2];
    c[0] = m->m[2][0];
    c[1] = m->m[2][1];
    c[2] = m->m[2][2];
    outer12(b, c, a);
    outer12(a, b, c);
    vector_normal(a);
    vector_normal(b);
    vector_normal(c);
    for (i = 0; i < 3; i++) {
        n->m[0][i] = (short)a[i];
        n->m[1][i] = (short)b[i];
        n->m[2][i] = (short)c[i];
    }
}
