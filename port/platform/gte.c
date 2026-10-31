/* The GTE and libgte as plain C (ROADMAP-B 6.2): the registers and operations the game uses, with the
 * hardware's fixed-point rules (saturation, the 32-bit MACs, the division's rounding), and libgte's
 * matrix functions with their original order of multiplies and shifts, so results match bit for bit. */
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <libgte.h>
#include <inline_c.h>

/* libgte's sine and cosine table, { sin, cos } for 4096 angles, and VectorNormal's table. Both are
 * exactly these formulas: checked against the tables in a retail exe. */
short rcossin_tbl[4096 * 2];
static short InvSqrtTbl[192];

static struct {
    int16_t v[3][3];
    uint32_t rgbc, code_otz;
    int32_t otz, ir0, ir[3];
    int16_t sx[3], sy[3];
    uint16_t sz[4];
    uint32_t rgb[3];
    int32_t mac0, mac[3];
    int16_t r[3][3];
    int32_t tr[3];
    int32_t fc[3];
    int32_t ofx, ofy, h, dqa, dqb, zsf3, zsf4;
} G;

static int32_t lm_ir(int32_t v, int lm) {
    int32_t lo = lm ? 0 : -0x8000;

    return v < lo ? lo : v > 0x7FFF ? 0x7FFF : v;
}

static void set_mac_ir(int i, int64_t v, int sf, int lm) {
    G.mac[i] = (int32_t)(v >> (sf * 12));
    G.ir[i] = lm_ir(G.mac[i], lm);
}

/* MVMVA with the rotation matrix, no translation or TR, on a vector */
static void mvmva(const int32_t *t, int v0, int v1, int v2, int sf) {
    int i;

    for (i = 0; i < 3; i++) {
        int64_t s = (t != NULL) ? ((int64_t)t[i] << 12) : 0;

        s += (int64_t)G.r[i][0] * v0 + (int64_t)G.r[i][1] * v1 + (int64_t)G.r[i][2] * v2;
        set_mac_ir(i, s, sf, 0);
    }
}

/* The division of RTPS: H / SZ3 by the hardware's table and Newton steps, or saturated. */
static uint32_t rtp_divide(uint32_t h, uint32_t z) {
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

static float PreciseX[3], PreciseY[3], PreciseW[3];

/* The last three positions projected, unrounded: x, y and depth each. */
void pc_gte_precise(float out[9]) {
    int i;

    for (i = 0; i < 3; i++) {
        out[i * 3] = PreciseX[i];
        out[i * 3 + 1] = PreciseY[i];
        out[i * 3 + 2] = PreciseW[i];
    }
}

static void rtp(int i, int last) {
    int64_t sx, sy, sz;
    int32_t z;
    uint32_t n;

    mvmva(G.tr, G.v[i][0], G.v[i][1], G.v[i][2], 1);
    z = G.mac[2];
    G.sz[0] = G.sz[1];
    G.sz[1] = G.sz[2];
    G.sz[2] = G.sz[3];
    G.sz[3] = z < 0 ? 0 : z > 0xFFFF ? 0xFFFF : z;
    n = rtp_divide((uint32_t)G.h, G.sz[3]);
    sx = (int64_t)n * G.ir[0] + G.ofx;
    sy = (int64_t)n * G.ir[1] + G.ofy;
    sz = (int64_t)n * G.dqa + G.dqb;
    PreciseX[0] = PreciseX[1];
    PreciseY[0] = PreciseY[1];
    PreciseW[0] = PreciseW[1];
    PreciseX[1] = PreciseX[2];
    PreciseY[1] = PreciseY[2];
    PreciseW[1] = PreciseW[2];
    {
        float d = z < 1 ? 1.0f : (float)z;

        PreciseX[2] = (float)G.ofx / 65536.0f + (float)G.h * (float)G.ir[0] / d;
        PreciseY[2] = (float)G.ofy / 65536.0f + (float)G.h * (float)G.ir[1] / d;
        PreciseW[2] = d;
    }
    G.sx[0] = G.sx[1];
    G.sy[0] = G.sy[1];
    G.sx[1] = G.sx[2];
    G.sy[1] = G.sy[2];
    {
        int32_t x = (int32_t)(sx >> 16);
        int32_t y = (int32_t)(sy >> 16);

        G.sx[2] = x < -0x400 ? -0x400 : x > 0x3FF ? 0x3FF : x;
        G.sy[2] = y < -0x400 ? -0x400 : y > 0x3FF ? 0x3FF : y;
    }
    G.mac0 = (int32_t)sy;
    if (last) {
        int32_t d;

        G.mac0 = (int32_t)sz;
        d = (int32_t)(sz >> 12);
        G.ir0 = d < 0 ? 0 : d > 0x1000 ? 0x1000 : d;
    }
}

static void push_rgb(void) {
    int i;
    uint32_t c = 0;

    for (i = 0; i < 3; i++) {
        int32_t v = G.mac[i] >> 4;

        v = v < 0 ? 0 : v > 255 ? 255 : v;
        c |= (uint32_t)v << (i * 8);
    }
    G.rgb[0] = G.rgb[1];
    G.rgb[1] = G.rgb[2];
    G.rgb[2] = c | (G.rgbc & 0xFF000000u);
}

/* [MAC] = MAC + (FC - MAC) * IR0 */
static void interpolate(int32_t m0, int32_t m1, int32_t m2) {
    int32_t m[3];
    int i;

    m[0] = m0;
    m[1] = m1;
    m[2] = m2;
    for (i = 0; i < 3; i++) {
        set_mac_ir(i, ((int64_t)G.fc[i] << 12) - m[i], 1, 0);
    }
    for (i = 0; i < 3; i++) {
        set_mac_ir(i, (int64_t)G.ir[i] * G.ir0 + m[i], 1, 0);
    }
}

void pc_gte_SetGeomOffset(int x, int y) {
    G.ofx = x << 16;
    G.ofy = y << 16;
}
void pc_gte_SetGeomScreen(int h) {
    G.h = h & 0xFFFF;
}
void pc_gte_SetRotMatrix(const void *m) {
    const MATRIX *a = m;
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            G.r[i][j] = a->m[i][j];
        }
    }
}
void pc_gte_SetTransMatrix(const void *m) {
    const MATRIX *a = m;

    G.tr[0] = a->t[0];
    G.tr[1] = a->t[1];
    G.tr[2] = a->t[2];
}
void pc_gte_SetTransVector(const void *v) {
    const VECTOR *a = v;

    G.tr[0] = a->vx;
    G.tr[1] = a->vy;
    G.tr[2] = a->vz;
}
void pc_gte_ldv0(const void *v) {
    const SVECTOR *a = v;

    G.v[0][0] = a->vx;
    G.v[0][1] = a->vy;
    G.v[0][2] = a->vz;
}
void pc_gte_ldv0_words(int xy, int z) {
    G.v[0][0] = (int16_t)xy;
    G.v[0][1] = (int16_t)(xy >> 16);
    G.v[0][2] = (int16_t)z;
}
void pc_gte_ldv3c(const void *v) {
    const SVECTOR *a = v;
    int i;

    for (i = 0; i < 3; i++) {
        G.v[i][0] = a[i].vx;
        G.v[i][1] = a[i].vy;
        G.v[i][2] = a[i].vz;
    }
}
void pc_gte_ldsv(const void *v) {
    const SVECTOR *a = v;

    G.ir[0] = a->vx;
    G.ir[1] = a->vy;
    G.ir[2] = a->vz;
}
void pc_gte_ldrgb(const void *c) {
    const unsigned char *a = c;

    G.rgbc = a[0] | (a[1] << 8) | (a[2] << 16) | ((uint32_t)a[3] << 24);
}
void pc_gte_lddp(int p) {
    G.ir0 = (int16_t)p;
}
void pc_gte_ldfcdir(int r, int g, int b) {
    G.fc[0] = r;
    G.fc[1] = g;
    G.fc[2] = b;
}
void pc_gte_rtps(void) {
    rtp(0, 1);
}
void pc_gte_rtpt(void) {
    rtp(0, 0);
    rtp(1, 0);
    rtp(2, 1);
}
void pc_gte_rt(void) {
    mvmva(G.tr, G.v[0][0], G.v[0][1], G.v[0][2], 1);
}
void pc_gte_nclip(void) {
    int64_t a = (int64_t)G.sx[0] * G.sy[1] + (int64_t)G.sx[1] * G.sy[2] + (int64_t)G.sx[2] * G.sy[0];
    int64_t b = (int64_t)G.sx[0] * G.sy[2] + (int64_t)G.sx[1] * G.sy[0] + (int64_t)G.sx[2] * G.sy[1];

    G.mac0 = (int32_t)(a - b);
}
void pc_gte_avsz3(void) {
    int32_t v;

    G.mac0 = G.zsf3 * ((int32_t)G.sz[1] + G.sz[2] + G.sz[3]);
    v = G.mac0 >> 12;
    G.otz = v < 0 ? 0 : v > 0xFFFF ? 0xFFFF : v;
}
void pc_gte_dpcs(void) {
    interpolate((int32_t)(G.rgbc & 0xFF) << 16, (int32_t)((G.rgbc >> 8) & 0xFF) << 16,
                (int32_t)((G.rgbc >> 16) & 0xFF) << 16);
    push_rgb();
}
void pc_gte_intpl(void) {
    interpolate(G.ir[0] << 12, G.ir[1] << 12, G.ir[2] << 12);
    push_rgb();
}
static uint32_t sxy(int i) {
    return (uint16_t)G.sx[i] | ((uint32_t)(uint16_t)G.sy[i] << 16);
}
void pc_gte_stsxy(void *p) {
    *(uint32_t *)p = sxy(2);
}
void pc_gte_stsxy2(void *p) {
    *(uint32_t *)p = sxy(2);
}
void pc_gte_stsxy3c(void *p) {
    uint32_t *a = p;

    a[0] = sxy(0);
    a[1] = sxy(1);
    a[2] = sxy(2);
}
void pc_gte_stsxy3(void *p0, void *p1, void *p2) {
    *(uint32_t *)p0 = sxy(0);
    *(uint32_t *)p1 = sxy(1);
    *(uint32_t *)p2 = sxy(2);
}
void pc_gte_stsz(void *p) {
    *(uint32_t *)p = G.sz[3];
}
void pc_gte_stsz3c(void *p) {
    uint32_t *a = p;

    a[0] = G.sz[1];
    a[1] = G.sz[2];
    a[2] = G.sz[3];
}
void pc_gte_stotz(void *p) {
    *(uint32_t *)p = G.otz;
}
void pc_gte_stopz(void *p) {
    *(int32_t *)p = G.mac0;
}
void pc_gte_stlvl(void *p) {
    int32_t *a = p;

    a[0] = G.ir[0];
    a[1] = G.ir[1];
    a[2] = G.ir[2];
}
void pc_gte_strgb(void *p) {
    *(uint32_t *)p = G.rgb[2];
}
void pc_gte_stsv(void *p) {
    int16_t *a = p;

    a[0] = (int16_t)G.ir[0];
    a[1] = (int16_t)G.ir[1];
    a[2] = (int16_t)G.ir[2];
}

/* libgte */

void InitGeom(void) {
    int i;

    for (i = 0; i < 4096; i++) {
        double a = i * 3.14159265358979323846 * 2 / 4096;

        rcossin_tbl[i * 2] = (short)floor(sin(a) * 4096 + 0.5);
        rcossin_tbl[i * 2 + 1] = (short)floor(cos(a) * 4096 + 0.5);
    }
    for (i = 0; i < 192; i++) {
        InvSqrtTbl[i] = (short)floor(4096 * sqrt(64.0 / (i + 64)));
    }
    G.zsf3 = 0x155;
    G.zsf4 = 0x100;
    G.h = 1000;
    G.dqa = (int16_t)0xEF9E;
    G.dqb = 0x1400000;
    G.ofx = 0;
    G.ofy = 0;
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

/* Multiplies with the GTE's rotation matrix, which it overwrites, as libgte does. */
MATRIX *MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *out) {
    int16_t r[3][3];
    int i, j;

    pc_gte_SetRotMatrix(m0);
    for (j = 0; j < 3; j++) {
        mvmva(NULL, m1->m[0][j], m1->m[1][j], m1->m[2][j], 1);
        G.v[0][0] = m1->m[0][j];
        G.v[0][1] = m1->m[1][j];
        G.v[0][2] = m1->m[2][j];
        for (i = 0; i < 3; i++) {
            r[i][j] = (int16_t)G.ir[i];
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
    int32_t v[3], hi[3], lo[3], rh[3];
    int i;

    pc_gte_SetRotMatrix(m);
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
    mvmva(NULL, (int16_t)hi[0], (int16_t)hi[1], (int16_t)hi[2], 0);
    for (i = 0; i < 3; i++) {
        rh[i] = G.mac[i];
    }
    mvmva(NULL, (int16_t)lo[0], (int16_t)lo[1], (int16_t)lo[2], 1);
    out->vx = (int32_t)((uint32_t)rh[0] << 3) + G.mac[0];
    out->vy = (int32_t)((uint32_t)rh[1] << 3) + G.mac[1];
    out->vz = (int32_t)((uint32_t)rh[2] << 3) + G.mac[2];
    return out;
}

/* OP with sf set: the cross product, >> 12, of two vectors whose low halves are what the GTE sees. */
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
