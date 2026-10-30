/* The PS1's GPU as a software rasteriser (ROADMAP-B 6.2): 1 MB of VRAM and the GP0 command stream, so
 * the game's primitives, its VRAM copies and its texture tricks work as they do on the console. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "gpu_raster.h"

uint16_t Vram[VRAM_H][VRAM_W];

static struct {
    int clip_x0, clip_y0, clip_x1, clip_y1;
    int ofs_x, ofs_y;
    int page_x, page_y, abr, depth;
    int dither;
    int win_mx, win_my, win_ox, win_oy;
    int set_mask, check_mask;
} S;

static const int8_t DitherTbl[4][4] = {
    { -4, 0, -3, 1 },
    { 2, -2, 3, -1 },
    { -3, 1, -4, 0 },
    { 3, -1, 2, -2 },
};

typedef struct {
    int x, y;
    int r, g, b;
    int u, v;
} Vtx;

static void set_tpage(uint32_t w) {
    S.page_x = (w & 0xF) * 64;
    S.page_y = ((w >> 4) & 1) * 256 + ((w >> 11) & 1) * 512;
    S.abr = (w >> 5) & 3;
    S.depth = (w >> 7) & 3;
}

static int sext11(int v) {
    return (int)((uint32_t)v << 21) >> 21;
}

/* One pixel: the mask test, blending, the mask bit. */
static inline void put_pixel(int x, int y, uint16_t c, int semi) {
    uint16_t *d = &Vram[y][x];
    uint16_t bg = *d;

    if (S.check_mask && (bg & 0x8000)) {
        return;
    }
    if (semi) {
        int br = bg & 31, bgn = (bg >> 5) & 31, bb = (bg >> 10) & 31;
        int fr = c & 31, fg = (c >> 5) & 31, fb = (c >> 10) & 31;
        int r, g, b;

        switch (S.abr) {
        case 0:
            r = (br + fr) >> 1;
            g = (bgn + fg) >> 1;
            b = (bb + fb) >> 1;
            break;
        case 1:
            r = br + fr;
            g = bgn + fg;
            b = bb + fb;
            break;
        case 2:
            r = br - fr;
            g = bgn - fg;
            b = bb - fb;
            break;
        default:
            r = br + (fr >> 2);
            g = bgn + (fg >> 2);
            b = bb + (fb >> 2);
            break;
        }
        r = r < 0 ? 0 : r > 31 ? 31 : r;
        g = g < 0 ? 0 : g > 31 ? 31 : g;
        b = b < 0 ? 0 : b > 31 ? 31 : b;
        c = (c & 0x8000) | r | (g << 5) | (b << 10);
    }
    *d = c | (S.set_mask ? 0x8000 : 0);
}

static inline uint16_t texel(int u, int v, int clut_x, int clut_y) {
    u = (u & ~(S.win_mx * 8)) | ((S.win_ox & S.win_mx) * 8);
    v = (v & ~(S.win_my * 8)) | ((S.win_oy & S.win_my) * 8);
    switch (S.depth) {
    case 0: {
        int w = Vram[(S.page_y + v) & 511][(S.page_x + (u >> 2)) & 1023];

        return Vram[clut_y & 511][(clut_x + ((w >> ((u & 3) * 4)) & 15)) & 1023];
    }
    case 1: {
        int w = Vram[(S.page_y + v) & 511][(S.page_x + (u >> 1)) & 1023];

        return Vram[clut_y & 511][(clut_x + ((w >> ((u & 1) * 8)) & 255)) & 1023];
    }
    default:
        return Vram[(S.page_y + v) & 511][(S.page_x + u) & 1023];
    }
}

/* A texel or flat colour through the vertex colour, the dither pattern and down to 5 bits. */
static inline uint16_t shade(uint16_t tex, int r, int g, int b, int x, int y, int dither) {
    int cr, cg, cb;

    cr = ((tex & 31) * r) >> 4;
    cg = (((tex >> 5) & 31) * g) >> 4;
    cb = (((tex >> 10) & 31) * b) >> 4;
    if (dither) {
        int d = DitherTbl[y & 3][x & 3];

        cr += d;
        cg += d;
        cb += d;
    }
    cr = cr < 0 ? 0 : cr > 255 ? 255 : cr;
    cg = cg < 0 ? 0 : cg > 255 ? 255 : cg;
    cb = cb < 0 ? 0 : cb > 255 ? 255 : cb;
    return (tex & 0x8000) | (cr >> 3) | ((cg >> 3) << 5) | ((cb >> 3) << 10);
}

static inline uint16_t flat(int r, int g, int b, int x, int y, int dither) {
    if (dither) {
        int d = DitherTbl[y & 3][x & 3];

        r += d;
        g += d;
        b += d;
        r = r < 0 ? 0 : r > 255 ? 255 : r;
        g = g < 0 ? 0 : g > 255 ? 255 : g;
        b = b < 0 ? 0 : b > 255 ? 255 : b;
    }
    return (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10);
}

/* floor(a / b) for b > 0 */
static int64_t fdiv(int64_t a, int64_t b) {
    int64_t q = a / b;

    if ((a % b) != 0 && a < 0) {
        q--;
    }
    return q;
}

/* x on the edge a-b at row y, rounded up: a pixel's centre on the left edge is in, on the right edge
 * out, which is the hardware's top-left rule. */
static inline int edge_x(const Vtx *a, const Vtx *b, int y) {
    int64_t dy = b->y - a->y;

    return (int)fdiv((int64_t)(b->x - a->x) * (y - a->y) + dy - 1, dy) + a->x;
}

static void triangle(const Vtx *v0, const Vtx *v1, const Vtx *v2, int textured, int gouraud, int semi, int raw,
                     int clut_x, int clut_y) {
    const Vtx *t;
    const Vtx *a = v0, *b = v1, *c = v2;
    int64_t area;
    int64_t drx = 0, dry = 0, dgx = 0, dgy = 0, dbx = 0, dby = 0, dux = 0, duy = 0, dvx = 0, dvy = 0;
    int minx, maxx, y, ytop, ybot;
    int dither;

    area = (int64_t)(v1->x - v0->x) * (v2->y - v0->y) - (int64_t)(v2->x - v0->x) * (v1->y - v0->y);
    if (area == 0) {
        return;
    }
    minx = v0->x < v1->x ? v0->x : v1->x;
    minx = minx < v2->x ? minx : v2->x;
    maxx = v0->x > v1->x ? v0->x : v1->x;
    maxx = maxx > v2->x ? maxx : v2->x;
    if (a->y > b->y) { t = a; a = b; b = t; }
    if (b->y > c->y) { t = b; b = c; c = t; }
    if (a->y > b->y) { t = a; a = b; b = t; }
    if (maxx - minx >= 1024 || c->y - a->y >= 512) {
        return;
    }
#define GRAD(f, dfx, dfy) \
    dfx = (((int64_t)(v1->f - v0->f) * (v2->y - v0->y) - (int64_t)(v2->f - v0->f) * (v1->y - v0->y)) << 16) / area; \
    dfy = (((int64_t)(v2->f - v0->f) * (v1->x - v0->x) - (int64_t)(v1->f - v0->f) * (v2->x - v0->x)) << 16) / area
    if (gouraud) {
        GRAD(r, drx, dry);
        GRAD(g, dgx, dgy);
        GRAD(b, dbx, dby);
    }
    if (textured) {
        GRAD(u, dux, duy);
        GRAD(v, dvx, dvy);
    }
#undef GRAD
    dither = S.dither && (gouraud || (textured && !raw));
    ytop = a->y < S.clip_y0 ? S.clip_y0 : a->y;
    ybot = c->y > S.clip_y1 + 1 ? S.clip_y1 + 1 : c->y;
    for (y = ytop; y < ybot; y++) {
        int xa, xb, x, xend;
        int64_t fr, fg, fb, fu, fv;

        xa = edge_x(a, c, y);
        xb = (y < b->y) ? edge_x(a, b, y) : edge_x(b, c, y);
        if (xa > xb) {
            int k = xa;

            xa = xb;
            xb = k;
        }
        xend = xb > S.clip_x1 + 1 ? S.clip_x1 + 1 : xb;
        x = xa < S.clip_x0 ? S.clip_x0 : xa;
        if (x >= xend) {
            continue;
        }
#define START(f, dfx, dfy) \
    (((int64_t)v0->f << 16) + (dfx) * (x - v0->x) + (dfy) * (y - v0->y) + 0x8000)
        fr = START(r, drx, dry);
        fg = START(g, dgx, dgy);
        fb = START(b, dbx, dby);
        fu = START(u, dux, duy);
        fv = START(v, dvx, dvy);
#undef START
        for (; x < xend; x++) {
            if (textured) {
                uint16_t c16 = texel((int)(fu >> 16) & 255, (int)(fv >> 16) & 255, clut_x, clut_y);

                if (c16 != 0) {
                    if (!raw) {
                        c16 = shade(c16, (int)(fr >> 16), (int)(fg >> 16), (int)(fb >> 16), x, y, dither);
                    }
                    put_pixel(x, y, c16, semi && (c16 & 0x8000));
                }
                fu += dux;
                fv += dvx;
            } else {
                put_pixel(x, y, flat((int)(fr >> 16), (int)(fg >> 16), (int)(fb >> 16), x, y, dither), semi);
            }
            fr += drx;
            fg += dgx;
            fb += dbx;
        }
    }
}

static void polygon(const uint32_t *p, uint32_t code) {
    int quad = (code >> 3) & 1, gouraud = (code >> 4) & 1, textured = (code >> 2) & 1;
    int semi = (code >> 1) & 1, raw = code & 1;
    int n = quad ? 4 : 3;
    Vtx v[4];
    int i;
    int clut_x = 0, clut_y = 0;
    int r = p[0] & 255, g = (p[0] >> 8) & 255, b = (p[0] >> 16) & 255;

    p++;
    for (i = 0; i < n; i++) {
        if (gouraud && i > 0) {
            r = *p & 255;
            g = (*p >> 8) & 255;
            b = (*p >> 16) & 255;
            p++;
        }
        v[i].r = r;
        v[i].g = g;
        v[i].b = b;
        v[i].x = sext11(*p & 0xFFFF) + S.ofs_x;
        v[i].y = sext11(*p >> 16) + S.ofs_y;
        v[i].x = sext11(v[i].x);
        v[i].y = sext11(v[i].y);
        p++;
        v[i].u = 0;
        v[i].v = 0;
        if (textured) {
            v[i].u = *p & 255;
            v[i].v = (*p >> 8) & 255;
            if (i == 0) {
                clut_x = ((*p >> 16) & 63) * 16;
                clut_y = (*p >> 22) & 511;
            } else if (i == 1) {
                set_tpage(*p >> 16);
            }
            p++;
        }
    }
    if (!gouraud && !textured) {
        /* flat: no gradients needed, the colour goes through START's base */
    }
    if (!gouraud) {
        /* the same colour everywhere: the gradients stay 0 */
    }
    triangle(&v[0], &v[1], &v[2], textured, gouraud, semi, raw, clut_x, clut_y);
    if (quad) {
        triangle(&v[1], &v[2], &v[3], textured, gouraud, semi, raw, clut_x, clut_y);
    }
}

static void rectangle(const uint32_t *p, uint32_t code) {
    int size = (code >> 3) & 3, textured = (code >> 2) & 1, semi = (code >> 1) & 1, raw = code & 1;
    int r = p[0] & 255, g = (p[0] >> 8) & 255, b = (p[0] >> 16) & 255;
    int x0, y0, u0 = 0, v0 = 0, w, h;
    int clut_x = 0, clut_y = 0;
    int x, y;

    p++;
    x0 = sext11(sext11(*p & 0xFFFF) + S.ofs_x);
    y0 = sext11(sext11(*p >> 16) + S.ofs_y);
    p++;
    if (textured) {
        u0 = *p & 255;
        v0 = (*p >> 8) & 255;
        clut_x = ((*p >> 16) & 63) * 16;
        clut_y = (*p >> 22) & 511;
        p++;
    }
    switch (size) {
    case 0:
        w = *p & 0xFFFF;
        h = *p >> 16;
        break;
    case 1:
        w = h = 1;
        break;
    case 2:
        w = h = 8;
        break;
    default:
        w = h = 16;
        break;
    }
    for (y = 0; y < h; y++) {
        int py = y0 + y;

        if (py < S.clip_y0 || py > S.clip_y1) {
            continue;
        }
        for (x = 0; x < w; x++) {
            int px = x0 + x;

            if (px < S.clip_x0 || px > S.clip_x1) {
                continue;
            }
            if (textured) {
                uint16_t c = texel((u0 + x) & 255, (v0 + y) & 255, clut_x, clut_y);

                if (c != 0) {
                    if (!raw) {
                        c = shade(c, r, g, b, px, py, 0);
                    }
                    put_pixel(px, py, c, semi && (c & 0x8000));
                }
            } else {
                put_pixel(px, py, flat(r, g, b, px, py, 0), semi);
            }
        }
    }
}

static void line(int x0, int y0, int c0, int x1, int y1, int c1, int semi, int gouraud) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int n = dx > dy ? dx : dy;
    int i;

    if (dx >= 1024 || dy >= 512) {
        return;
    }
    for (i = 0; i <= n; i++) {
        int x = n ? x0 + (x1 - x0) * i / n : x0;
        int y = n ? y0 + (y1 - y0) * i / n : y0;
        int r = c0 & 255, g = (c0 >> 8) & 255, b = (c0 >> 16) & 255;

        if (gouraud && n) {
            r += (((c1 & 255) - r) * i) / n;
            g += ((((c1 >> 8) & 255) - g) * i) / n;
            b += ((((c1 >> 16) & 255) - b) * i) / n;
        }
        if (x >= S.clip_x0 && x <= S.clip_x1 && y >= S.clip_y0 && y <= S.clip_y1) {
            put_pixel(x, y, flat(r, g, b, x, y, S.dither && gouraud), semi);
        }
    }
}

void gpu_fill(int x, int y, int w, int h, uint32_t rgb) {
    uint16_t c = ((rgb & 255) >> 3) | ((((rgb >> 8) & 255) >> 3) << 5) | ((((rgb >> 16) & 255) >> 3) << 10);
    int i, j;

    for (j = 0; j < h; j++) {
        uint16_t *row = Vram[(y + j) & 511];

        for (i = 0; i < w; i++) {
            row[(x + i) & 1023] = c;
        }
    }
}

void gpu_load_image(int x, int y, int w, int h, const uint16_t *p) {
    int i, j;

    for (j = 0; j < h; j++) {
        uint16_t *row = Vram[(y + j) & 511];

        for (i = 0; i < w; i++) {
            row[(x + i) & 1023] = *p++;
        }
    }
}

void gpu_store_image(int x, int y, int w, int h, uint16_t *p) {
    int i, j;

    for (j = 0; j < h; j++) {
        const uint16_t *row = Vram[(y + j) & 511];

        for (i = 0; i < w; i++) {
            *p++ = row[(x + i) & 1023];
        }
    }
}

void gpu_move_image(int sx, int sy, int dx, int dy, int w, int h) {
    static uint16_t line_buf[VRAM_W];
    int j, i;

    /* row by row in the order that leaves an overlapping copy as the hardware's would, which reads a
     * row into its FIFO before it writes it */
    for (j = 0; j < h; j++) {
        const uint16_t *s = Vram[(sy + j) & 511];
        uint16_t *d = Vram[(dy + j) & 511];

        for (i = 0; i < w; i++) {
            line_buf[i] = s[(sx + i) & 1023];
        }
        for (i = 0; i < w; i++) {
            uint16_t *q = &d[(dx + i) & 1023];

            if (!(S.check_mask && (*q & 0x8000))) {
                *q = line_buf[i] | (S.set_mask ? 0x8000 : 0);
            }
        }
    }
}

void gpu_set_draw_env(int x0, int y0, int x1, int y1, int ofx, int ofy, uint32_t tpage_word, uint32_t win) {
    S.clip_x0 = x0;
    S.clip_y0 = y0;
    S.clip_x1 = x1;
    S.clip_y1 = y1;
    S.ofs_x = sext11(ofx);
    S.ofs_y = sext11(ofy);
    set_tpage(tpage_word);
    S.dither = (tpage_word >> 9) & 1;
    S.win_mx = win & 31;
    S.win_my = (win >> 5) & 31;
    S.win_ox = (win >> 10) & 31;
    S.win_oy = (win >> 15) & 31;
}

/* The number of words the command at p takes, with the variable-length ones read from the packet. */
static int command_size(const uint32_t *p, int left) {
    uint32_t code = p[0] >> 24;

    if (code >= 0x20 && code < 0x40) {
        int n = (code & 8) ? 4 : 3;
        int size = 1 + n * ((code & 4) ? 2 : 1);

        if (code & 0x10) {
            size += n - 1;
        }
        return size;
    }
    if (code >= 0x40 && code < 0x60) {
        int gouraud = (code >> 4) & 1;

        if (code & 8) {
            int i = 1;

            while (i < left && (p[i] & 0xF000F000u) != 0x50005000u) {
                i++;
            }
            return i + 1;
        }
        return gouraud ? 4 : 3;
    }
    if (code >= 0x60 && code < 0x80) {
        return 2 + ((code & 4) ? 1 : 0) + (((code >> 3) & 3) == 0 ? 1 : 0);
    }
    if (code >= 0x80 && code < 0xA0) {
        return 4;
    }
    if (code >= 0xA0 && code < 0xC0) {
        int w = p[2] & 0xFFFF, h = p[2] >> 16;

        return 3 + (w * h + 1) / 2;
    }
    if (code >= 0xC0 && code < 0xE0) {
        return 3;
    }
    if (code == 0x02) {
        return 3;
    }
    return 1;
}

/* Runs the words of one packet as the GPU's FIFO would see them: often one command, but DR_ENV and
 * friends hold several. */
void gpu_run_packet(const uint32_t *p, int len) {
    while (len > 0) {
        uint32_t w = p[0];
        uint32_t code = w >> 24;
        int size = command_size(p, len);

        if (size > len) {
            return;
        }
        if (code >= 0x20 && code < 0x40) {
            polygon(p, code);
        } else if (code >= 0x40 && code < 0x60) {
            int semi = (code >> 1) & 1, gouraud = (code >> 4) & 1;
            int x0, y0, c0 = w, i = 1;

            x0 = sext11(p[i] & 0xFFFF) + S.ofs_x;
            y0 = sext11(p[i] >> 16) + S.ofs_y;
            i++;
            for (;;) {
                int c1 = c0, x1, y1;

                if (i >= size || (p[i] & 0xF000F000u) == 0x50005000u) {
                    break;
                }
                if (gouraud) {
                    c1 = p[i++];
                }
                if (i >= size) {
                    break;
                }
                x1 = sext11(p[i] & 0xFFFF) + S.ofs_x;
                y1 = sext11(p[i] >> 16) + S.ofs_y;
                i++;
                line(x0, y0, c0, x1, y1, c1, semi, gouraud);
                x0 = x1;
                y0 = y1;
                c0 = c1;
            }
        } else if (code >= 0x60 && code < 0x80) {
            rectangle(p, code);
        } else if (code >= 0x80 && code < 0xA0) {
            gpu_move_image(p[1] & 0x3FF, (p[1] >> 16) & 0x1FF, p[2] & 0x3FF, (p[2] >> 16) & 0x1FF,
                           ((p[3] & 0xFFFF) - 1) % 1024 + 1, (((p[3] >> 16)) - 1) % 512 + 1);
        } else if (code >= 0xA0 && code < 0xC0) {
            gpu_load_image(p[1] & 0x3FF, (p[1] >> 16) & 0x1FF, p[2] & 0xFFFF, p[2] >> 16, (const uint16_t *)&p[3]);
        } else if (code == 0x02) {
            /* the fill rounds x and w to 16 pixels, and ignores the clip and the mask */
            gpu_fill(p[1] & 0x3F0, (p[1] >> 16) & 0x1FF, ((p[2] & 0x3FF) + 15) & ~15, (p[2] >> 16) & 0x1FF, w & 0xFFFFFF);
        } else if (code == 0xE1) {
            set_tpage(w);
            S.dither = (w >> 9) & 1;
        } else if (code == 0xE2) {
            S.win_mx = w & 31;
            S.win_my = (w >> 5) & 31;
            S.win_ox = (w >> 10) & 31;
            S.win_oy = (w >> 15) & 31;
        } else if (code == 0xE3) {
            S.clip_x0 = w & 0x3FF;
            S.clip_y0 = (w >> 10) & 0x1FF;
        } else if (code == 0xE4) {
            S.clip_x1 = w & 0x3FF;
            S.clip_y1 = (w >> 10) & 0x1FF;
        } else if (code == 0xE5) {
            S.ofs_x = sext11(w & 0x7FF);
            S.ofs_y = sext11((w >> 11) & 0x7FF);
        } else if (code == 0xE6) {
            S.set_mask = w & 1;
            S.check_mask = (w >> 1) & 1;
        }
        p += size;
        len -= size;
    }
}
