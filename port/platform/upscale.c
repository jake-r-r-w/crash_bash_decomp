/* Makes a texture bigger, as it's first drawn, from the game's own pixels: no AI, nothing to download and nothing to generate first. A
 * corner of a texel takes its neighbours' colour where they meet at a diagonal (the Scale2x rule), so a stepped edge turns into a smoother
 * one. It only ever picks a colour that's already there and never blends, as a texel's alpha says what kind it is: clear, semi-transparent
 * or opaque. */
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
