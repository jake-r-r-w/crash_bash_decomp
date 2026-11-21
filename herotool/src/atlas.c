/* A model's texture laid out for the game (kit.h). The game keeps a corner's place in its image as whole texels (a byte each way), so
 * each corner moves up to half a texel; with a texture's pieces up against each other, as modelling programs pack them, a corner
 * moved so shows its neighbour's colour (flecks of another colour along every seam). So every piece (triangles joined by their
 * corners, in one image) gets a place of its own in one KIT_TEX x KIT_TEX image, as big as they can all be at once, KIT_PAD texels clear
 * all round filled with its own edge's colours. Triangles with no texture get a white piece, so their corners' colours show as they
 * are. Nothing of the model changes but where its corners are in its texture. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "kit.h"

typedef struct {
    int image;               /* its source image, -1 the white */
    float x0, y0, x1, y1;    /* where it is in its image, in texels */
    int ox, oy, pw, ph;      /* where it goes, and its size with its border */
    int first, count;        /* its triangles, in Order */
} Piece;

static int *Order;

static int by_height(const void *a, const void *b) {
    const Piece *p = a, *q = b;
    float d = (q->y1 - q->y0) - (p->y1 - p->y0);

    return d > 0 ? 1 : d < 0 ? -1 : 0;
}

/* its pieces placed on shelves, tallest first, at k texels a source texel: whether they all fit */
static int place(Piece *p, int n, float k) {
    int x = 0, y = 0, row = 0, i;

    for (i = 0; i < n; i++) {
        int pw = (int)ceilf((p[i].x1 - p[i].x0) * k) + 1 + 2 * KIT_PAD, ph = (int)ceilf((p[i].y1 - p[i].y0) * k) + 1 + 2 * KIT_PAD;

        if (p[i].image < 0) {
            pw = ph = 4 + 2 * KIT_PAD;
        }
        if (pw > KIT_TEX) {
            return 0;
        }
        if (x + pw > KIT_TEX) {
            x = 0;
            y += row;
            row = 0;
        }
        p[i].ox = x;
        p[i].oy = y;
        p[i].pw = pw;
        p[i].ph = ph;
        x += pw;
        row = ph > row ? ph : row;
    }
    return y + row <= KIT_TEX;
}

static int root(int *parent, int a) {
    while (parent[a] != a) {
        a = parent[a] = parent[parent[a]];
    }
    return a;
}

/* whether texel (x, y) of image im is under one of a piece's triangles, a texel's margin round them */
static int covered(const KitModel *m, const Piece *p, const PackImage *im, int x, int y) {
    int t, j;

    for (t = p->first; t < p->first + p->count; t++) {
        const int *tri = m->tri[Order[t]];
        float q[3][2], d, a, b, e, px = x + 0.5f, py = y + 0.5f;

        for (j = 0; j < 3; j++) {
            q[j][0] = m->uv[tri[j]][0] * im->w;
            q[j][1] = m->uv[tri[j]][1] * im->h;
        }
        d = (q[1][0] - q[0][0]) * (q[2][1] - q[0][1]) - (q[2][0] - q[0][0]) * (q[1][1] - q[0][1]);
        if (fabsf(d) < 1e-9f) {
            continue;
        }
        e = 1.0f / fmaxf(1.0f, sqrtf(fabsf(d)));
        a = ((q[1][0] - px) * (q[2][1] - py) - (q[2][0] - px) * (q[1][1] - py)) / d;
        b = ((q[2][0] - px) * (q[0][1] - py) - (q[0][0] - px) * (q[2][1] - py)) / d;
        if (a >= -e && b >= -e && 1 - a - b >= -e) {
            return 1;
        }
    }
    return 0;
}

int kit_atlas(KitModel *m, char *why, size_t why_size) {
    int *parent = malloc(sizeof(int) * m->nvert), *slot = malloc(sizeof(int) * m->nvert), *pieceof = malloc(sizeof(int) * m->ntri);
    Piece *pieces = calloc(m->ntri, sizeof(Piece));
    float *rgb = calloc(KIT_TEX * KIT_TEX * 3, sizeof(float)), lo = 1e-4f, hi = 64, k;
    uint8_t *have = calloc(KIT_TEX * KIT_TEX, 1), *done = calloc(m->nvert, 1);
    int npieces = 0, i, j, t, pass, at;
    SDL_Surface *s;
    SDL_IOStream *io;

    Order = malloc(sizeof(int) * m->ntri);
    if (parent == NULL || slot == NULL || pieceof == NULL || pieces == NULL || rgb == NULL || have == NULL || done == NULL || Order == NULL) {
        snprintf(why, why_size, "no memory");
        return 0;
    }
    /* the pieces: triangles of one image joined by their corners */
    for (i = 0; i < m->nvert; i++) {
        parent[i] = i;
        slot[i] = -1;
    }
    for (t = 0; t < m->ntri; t++) {
        parent[root(parent, m->tri[t][1])] = root(parent, m->tri[t][0]);
        parent[root(parent, m->tri[t][2])] = root(parent, m->tri[t][0]);
    }
    for (t = 0; t < m->ntri; t++) {
        int r = root(parent, m->tri[t][0]), im = m->tri_image[t] >= 0 && m->tri_image[t] < m->nimages ? m->tri_image[t] : -1;

        /* the untextured all share one white piece */
        if (im < 0) {
            r = m->nvert;
        }
        for (j = 0; j < npieces && !(pieces[j].image == im && (im < 0 || slot[r < m->nvert ? r : 0] == j)); j++) {
        }
        if (j == npieces) {
            pieces[j].image = im;
            pieces[j].x0 = pieces[j].y0 = 1e30f;
            pieces[j].x1 = pieces[j].y1 = -1e30f;
            npieces++;
            if (r < m->nvert) {
                slot[r] = j;
            }
        }
        pieceof[t] = j;
        pieces[j].count++;
        for (i = 0; i < 3 && im >= 0; i++) {
            float u = m->uv[m->tri[t][i]][0] * m->images[im].w, v = m->uv[m->tri[t][i]][1] * m->images[im].h;

            pieces[j].x0 = fminf(pieces[j].x0, u);
            pieces[j].x1 = fmaxf(pieces[j].x1, u);
            pieces[j].y0 = fminf(pieces[j].y0, v);
            pieces[j].y1 = fmaxf(pieces[j].y1, v);
        }
    }
    /* each piece's triangles together, in Order */
    for (j = 0, at = 0; j < npieces; j++) {
        pieces[j].first = at;
        at += pieces[j].count;
        pieces[j].count = 0;
    }
    for (t = 0; t < m->ntri; t++) {
        Piece *p = &pieces[pieceof[t]];

        Order[p->first + p->count++] = t;
    }
    /* as big as they all fit */
    qsort(pieces, npieces, sizeof(Piece), by_height);
    for (i = 0; i < 40; i++) {
        float mid = (lo + hi) / 2;

        if (place(pieces, npieces, mid)) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    k = lo;
    if (!place(pieces, npieces, k)) {
        snprintf(why, why_size, "the texture's %d pieces don't fit in %d x %d", npieces, KIT_TEX, KIT_TEX);
        return 0;
    }
    /* each piece's texels: the mean of the source texels under each that its own triangles cover; and its corners where it now is */
    for (j = 0; j < npieces; j++) {
        Piece *p = &pieces[j];
        const PackImage *im = p->image >= 0 ? &m->images[p->image] : NULL;
        int tx, ty;

        for (ty = p->oy; ty < p->oy + p->ph; ty++) {
            for (tx = p->ox; tx < p->ox + p->pw; tx++) {
                float s[3] = { 0, 0, 0 }, sx0, sy0;
                int n = 0, sx, sy, c;

                if (im == NULL) {
                    s[0] = s[1] = s[2] = 255;
                    n = 1;
                } else {
                    sx0 = p->x0 + (tx - p->ox - KIT_PAD) / k;
                    sy0 = p->y0 + (ty - p->oy - KIT_PAD) / k;
                    for (sy = (int)floorf(sy0); sy < (int)ceilf(sy0 + 1 / k); sy++) {
                        for (sx = (int)floorf(sx0); sx < (int)ceilf(sx0 + 1 / k); sx++) {
                            if (sx < 0 || sy < 0 || sx >= im->w || sy >= im->h || !covered(m, p, im, sx, sy)) {
                                continue;
                            }
                            for (c = 0; c < 3; c++) {
                                s[c] += im->rgba[((size_t)sy * im->w + sx) * 4 + c];
                            }
                            n++;
                        }
                    }
                }
                if (n > 0) {
                    for (c = 0; c < 3; c++) {
                        rgb[(ty * KIT_TEX + tx) * 3 + c] = s[c] / n;
                    }
                    have[ty * KIT_TEX + tx] = 1;
                }
            }
        }
        for (t = p->first; t < p->first + p->count; t++) {
            for (i = 0; i < 3; i++) {
                int v = m->tri[Order[t]][i];

                if (done[v]) {
                    continue;
                }
                done[v] = 1;
                if (im == NULL) {
                    m->uv[v][0] = (p->ox + KIT_PAD + 2.0f) / KIT_TEX;
                    m->uv[v][1] = (p->oy + KIT_PAD + 2.0f) / KIT_TEX;
                } else {
                    m->uv[v][0] = (p->ox + KIT_PAD + (m->uv[v][0] * im->w - p->x0) * k) / KIT_TEX;
                    m->uv[v][1] = (p->oy + KIT_PAD + (m->uv[v][1] * im->h - p->y0) * k) / KIT_TEX;
                }
            }
        }
    }
    /* the borders, and what's between, filled from their neighbours */
    for (pass = 0; pass < KIT_TEX; pass++) {
        uint8_t *next = malloc(KIT_TEX * KIT_TEX);
        int any = 0, x, y, c;

        memcpy(next, have, KIT_TEX * KIT_TEX);
        for (y = 0; y < KIT_TEX; y++) {
            for (x = 0; x < KIT_TEX; x++) {
                static const int dx[4] = { 1, -1, 0, 0 }, dy[4] = { 0, 0, 1, -1 };
                float s[3] = { 0, 0, 0 };
                int n = 0, d;

                if (have[y * KIT_TEX + x]) {
                    continue;
                }
                for (d = 0; d < 4; d++) {
                    int nx = x + dx[d], ny = y + dy[d];

                    if (nx < 0 || ny < 0 || nx >= KIT_TEX || ny >= KIT_TEX || !have[ny * KIT_TEX + nx]) {
                        continue;
                    }
                    for (c = 0; c < 3; c++) {
                        s[c] += rgb[(ny * KIT_TEX + nx) * 3 + c];
                    }
                    n++;
                }
                if (n > 0) {
                    for (c = 0; c < 3; c++) {
                        rgb[(y * KIT_TEX + x) * 3 + c] = s[c] / n;
                    }
                    next[y * KIT_TEX + x] = 1;
                    any = 1;
                }
            }
        }
        memcpy(have, next, KIT_TEX * KIT_TEX);
        free(next);
        if (!any) {
            break;
        }
    }
    m->atlas = malloc(KIT_TEX * KIT_TEX * 3);
    for (i = 0; i < KIT_TEX * KIT_TEX * 3; i++) {
        m->atlas[i] = (uint8_t)fminf(255, fmaxf(0, rgb[i] + 0.5f));
    }
    /* as a PNG */
    s = SDL_CreateSurfaceFrom(KIT_TEX, KIT_TEX, SDL_PIXELFORMAT_RGB24, m->atlas, KIT_TEX * 3);
    io = SDL_IOFromDynamicMem();
    if (s == NULL || io == NULL || !SDL_SavePNG_IO(s, io, false)) {
        snprintf(why, why_size, "can't write its texture: %s", SDL_GetError());
        return 0;
    }
    m->png_size = (size_t)SDL_TellIO(io);
    m->png = SDL_malloc(m->png_size);
    SDL_SeekIO(io, 0, SDL_IO_SEEK_SET);
    SDL_ReadIO(io, m->png, m->png_size);
    SDL_CloseIO(io);
    SDL_DestroySurface(s);
    printf("herotool: its texture's %d pieces laid out in %d x %d, at %.2f of the size they'd be shrunk to whole\n", npieces, KIT_TEX,
           KIT_TEX, m->nimages > 0 ? k * m->images[0].w / KIT_TEX : 1.0f);
    free(parent);
    free(slot);
    free(pieceof);
    free(pieces);
    free(rgb);
    free(have);
    free(done);
    free(Order);
    return 1;
}
