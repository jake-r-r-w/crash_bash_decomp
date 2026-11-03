/* Makes a texture bigger, as it's first drawn, from the game's own pixels: no AI, nothing to download and nothing to generate first. A
 * corner of a texel takes its neighbours' colour where they meet at a diagonal (the Scale2x rule), so a stepped edge turns into a smoother
 * one. It only ever picks a colour that's already there and never blends, as a texel's alpha says what kind it is: clear, semi-transparent
 * or opaque. */
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "upscale.h"

/* Texels this close count as the same colour, or dithered edges and noisy textures wouldn't change at all. */
#define NEAR_LUMA 48
#define NEAR_CHROMA 12

static int same(uint32_t a, uint32_t b) {
    int r, g, b2, y, u, v;

    if (a == b) {
        return 1;
    }
    if ((a >> 24) != (b >> 24)) {
        return 0;
    }
    if ((a >> 24) == 0) {
        return 1; /* clear is clear, whatever colour it's holding */
    }
    r = (int)(a & 0xFF) - (int)(b & 0xFF);
    g = (int)((a >> 8) & 0xFF) - (int)((b >> 8) & 0xFF);
    b2 = (int)((a >> 16) & 0xFF) - (int)((b >> 16) & 0xFF);
    y = (r * 77 + g * 150 + b2 * 29) >> 8;
    u = (b2 * 128 - r * 43 - g * 85) >> 8;
    v = (r * 128 - g * 107 - b2 * 21) >> 8;
    return abs(y) <= NEAR_LUMA && abs(u) <= NEAR_CHROMA && abs(v) <= NEAR_CHROMA;
}

static void double_it(const uint32_t *in, int w, int h, uint32_t *out) {
    int x, y;

    for (y = 0; y < h; y++) {
        const uint32_t *row = in + y * w;
        const uint32_t *above = y > 0 ? row - w : row;
        const uint32_t *below = y + 1 < h ? row + w : row;
        uint32_t *top = out + y * 2 * (w * 2);
        uint32_t *bottom = top + w * 2;

        for (x = 0; x < w; x++) {
            uint32_t e = row[x];
            uint32_t b = above[x], h2 = below[x];
            uint32_t d = x > 0 ? row[x - 1] : e, f = x + 1 < w ? row[x + 1] : e;
            uint32_t e0 = e, e1 = e, e2 = e, e3 = e;

            if (!same(b, h2) && !same(d, f)) {
                if (same(d, b)) {
                    e0 = d;
                }
                if (same(b, f)) {
                    e1 = f;
                }
                if (same(d, h2)) {
                    e2 = d;
                }
                if (same(h2, f)) {
                    e3 = f;
                }
            }
            top[x * 2] = e0;
            top[x * 2 + 1] = e1;
            bottom[x * 2] = e2;
            bottom[x * 2 + 1] = e3;
        }
    }
}

/* An icon is a small picture shown big, and its soft edges read as blur. So its 4x is a smooth enlargement (Catmull-Rom), made sharp again by taking
   away a blur of it (an unsharp mask). Unlike the 2x above, it makes colours in between, as an icon is a picture rather than flat colours. Its outline
   is where the smooth enlargement is half covered, so that stays crisp, and each texel is still clear, semi-transparent or opaque: the kind of the
   texel it's in, or of the one it's nearest if that one's clear. */
#define SHARP_AMOUNT 3.0f
#define SHARP_SIGMA 1.2f
#define SHARP_RADIUS 4 /* sigma * 3 */
/* below this, a point has too little of the icon to have a colour of its own */
#define SHARP_SOME 0.05f

static float catmull_rom(float t) {
    t = fabsf(t);
    return t < 1 ? 1.5f * t * t * t - 2.5f * t * t + 1 : t < 2 ? -0.5f * t * t * t + 2.5f * t * t - 4 * t + 2 : 0;
}

static uint32_t texel_at(const uint32_t *in, int w, int h, int x, int y) {
    x = x < 0 ? 0 : x >= w ? w - 1 : x;
    y = y < 0 ? 0 : y >= h ? h - 1 : y;
    return in[y * w + x];
}

static int shown(const uint32_t *in, int w, int h, int x, int y) {
    return (texel_at(in, w, h, x, y) >> 24) != 0;
}

/* A square corner of what's shown, as a portrait's: clear on two sides that meet, with both edges carrying on straight from it. The smooth outline would
   round it off, so it keeps its square. A step of a slope isn't one, as the texel beside it along an edge has the other side's step. */
static int square_corner(const uint32_t *in, int w, int h, int x, int y) {
    static const int sides[4][2] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
    int s;

    if (!shown(in, w, h, x, y)) {
        return 0;
    }
    for (s = 0; s < 4; s++) {
        const int *a = sides[s], *b = sides[(s + 1) % 4];

        if (shown(in, w, h, x + a[0], y + a[1]) || shown(in, w, h, x + b[0], y + b[1]) || shown(in, w, h, x + a[0] + b[0], y + a[1] + b[1])) {
            continue;
        }
        /* along a's edge away from the corner, and along b's */
        if (shown(in, w, h, x - b[0], y - b[1]) && !shown(in, w, h, x - b[0] + a[0], y - b[1] + a[1]) &&
            shown(in, w, h, x - a[0], y - a[1]) && !shown(in, w, h, x - a[0] + b[0], y - a[1] + b[1])) {
            return 1;
        }
    }
    return 0;
}

/* a line of count points from n, step apart: along x (step 1) or y (step w) */
static void blur_line(const float *in, float *out, int n, int step, int count, const float *kernel) {
    int i, k, c;

    for (i = 0; i < count; i++) {
        for (c = 0; c < 3; c++) {
            float v = 0;

            for (k = -SHARP_RADIUS; k <= SHARP_RADIUS; k++) {
                int j = i + k < 0 ? 0 : i + k >= count ? count - 1 : i + k;

                v += kernel[k + SHARP_RADIUS] * in[(n + j * step) * 3 + c];
            }
            out[(n + i * step) * 3 + c] = v;
        }
    }
}

void upscale_texels_sharp(const uint32_t *in, int w, int h, uint32_t *out) {
    const int k = UPSCALE_SHARP_FACTOR, W = w * k, H = h * k;
    float *rgb = malloc(sizeof(float) * 3 * (size_t)W * H), *across = malloc(sizeof(float) * 3 * (size_t)W * H), *blur = malloc(sizeof(float) * 3 * (size_t)W * H);
    float *cover = malloc(sizeof(float) * (size_t)W * H);
    float kernel[SHARP_RADIUS * 2 + 1], sum = 0;
    int x, y, i, j, c;

    if (rgb == NULL || across == NULL || blur == NULL || cover == NULL) {
        free(rgb);
        free(across);
        free(blur);
        free(cover);
        upscale_texels(in, w, h, out, k);
        return;
    }
    for (i = -SHARP_RADIUS; i <= SHARP_RADIUS; i++) {
        kernel[i + SHARP_RADIUS] = expf(-(float)(i * i) / (2 * SHARP_SIGMA * SHARP_SIGMA));
        sum += kernel[i + SHARP_RADIUS];
    }
    for (i = 0; i <= SHARP_RADIUS * 2; i++) {
        kernel[i] /= sum;
    }
    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            float sx = (x + 0.5f) / k - 0.5f, sy = (y + 0.5f) / k - 0.5f;
            int ix = (int)floorf(sx), iy = (int)floorf(sy), n = y * W + x;
            float r = 0, g = 0, b = 0, cov = 0;

            for (j = -1; j <= 2; j++) {
                for (i = -1; i <= 2; i++) {
                    uint32_t p = texel_at(in, w, h, ix + i, iy + j);
                    float wt = catmull_rom(sx - (ix + i)) * catmull_rom(sy - (iy + j));

                    if ((p >> 24) != 0) {
                        r += wt * (float)(p & 255);
                        g += wt * (float)((p >> 8) & 255);
                        b += wt * (float)((p >> 16) & 255);
                        cov += wt;
                    }
                }
            }
            cover[n] = cov;
            rgb[n * 3] = cov > SHARP_SOME ? r / cov : 0;
            rgb[n * 3 + 1] = cov > SHARP_SOME ? g / cov : 0;
            rgb[n * 3 + 2] = cov > SHARP_SOME ? b / cov : 0;
        }
    }
    for (y = 0; y < H; y++) {
        blur_line(rgb, across, y * W, 1, W, kernel);
    }
    for (x = 0; x < W; x++) {
        blur_line(across, blur, x, W, H, kernel);
    }
    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            int n = y * W + x;
            uint32_t kind = texel_at(in, w, h, x / k, y / k) >> 24, colour = 0;

            if (cover[n] < 0.5f && !square_corner(in, w, h, x / k, y / k)) {
                out[n] = 0;
                continue;
            }
            if (kind == 0) {
                kind = texel_at(in, w, h, (x + k / 2) / k, (y + k / 2) / k) >> 24;
            }
            for (c = 0; c < 3; c++) {
                float v = rgb[n * 3 + c] + SHARP_AMOUNT * (rgb[n * 3 + c] - blur[n * 3 + c]);

                colour |= (uint32_t)(v < 0 ? 0 : v > 255 ? 255 : (int)(v + 0.5f)) << (c * 8);
            }
            out[n] = colour | ((kind != 0 ? kind : 255) << 24);
        }
    }
    free(rgb);
    free(across);
    free(blur);
    free(cover);
}

void upscale_texels(const uint32_t *in, int w, int h, uint32_t *out, int factor) {
    if (factor == 2) {
        double_it(in, w, h, out);
    } else {
        uint32_t *middle = malloc(sizeof(uint32_t) * (size_t)w * 2 * h * 2);

        if (middle == NULL) {
            return;
        }
        double_it(in, w, h, middle);
        double_it(middle, w * 2, h * 2, out);
        free(middle);
    }
}
