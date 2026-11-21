/* The preview of what the game makes of a pack (docs/HERO_PACKS.md, The preview): the game, started with --dump-hero, writes each kind's
 * model as it's made (port/game/main/bonusheroes.c, made_dump), its faces, images and every frame of every move, and this reads one. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kit.h"

static int rd(FILE *f, void *to, size_t n) {
    return fread(to, 1, n, f) == n;
}

static int rd32(FILE *f, uint32_t *v) {
    uint8_t b[4];

    if (!rd(f, b, 4)) {
        return 0;
    }
    *v = b[0] | b[1] << 8 | b[2] << 16 | (uint32_t)b[3] << 24;
    return 1;
}

static int rd16(FILE *f, uint32_t *v) {
    uint8_t b[2];

    if (!rd(f, b, 2)) {
        return 0;
    }
    *v = b[0] | b[1] << 8;
    return 1;
}

void kit_made_free(KitMade *m) {
    int i;

    for (i = 0; i < m->nimages; i++) {
        free(m->images[i].rgba);
    }
    for (i = 0; i < m->nanims; i++) {
        free(m->anims[i].verts);
    }
    free(m->images);
    free(m->faces);
    free(m->anims);
    memset(m, 0, sizeof(*m));
}

int kit_made_read(const char *path, KitMade *m) {
    FILE *f = fopen(path, "rb");
    char magic[4];
    uint32_t n, i, k, w, h, v;
    int ok = 0;

    memset(m, 0, sizeof(*m));
    if (f == NULL) {
        return 0;
    }
    if (!rd(f, magic, 4) || memcmp(magic, "HPV1", 4) != 0 || !rd32(f, &n) || n > 64) {
        goto done;
    }
    m->images = calloc(n ? n : 1, sizeof(KitMadeImage));
    m->nimages = (int)n;
    for (i = 0; i < n; i++) {
        if (!rd16(f, &w) || !rd16(f, &h) || w == 0 || h == 0 || w > 1024 || h > 1024) {
            goto done;
        }
        m->images[i].w = (int)w;
        m->images[i].h = (int)h;
        m->images[i].rgba = malloc((size_t)w * h * 4);
        if (m->images[i].rgba == NULL || !rd(f, m->images[i].rgba, (size_t)w * h * 4)) {
            goto done;
        }
    }
    if (!rd32(f, &n) || n > 16384) {
        goto done;
    }
    m->faces = calloc(n ? n : 1, sizeof(KitMadeFace));
    m->nfaces = (int)n;
    for (i = 0; i < n; i++) {
        KitMadeFace *fc = &m->faces[i];
        uint8_t b[16];

        for (k = 0; k < 3; k++) {
            if (!rd32(f, &v)) {
                goto done;
            }
            fc->v[k] = (int)v;
        }
        if (!rd32(f, &v) || !rd(f, b, 16)) {
            goto done;
        }
        fc->image = (int32_t)v;
        for (k = 0; k < 3; k++) {
            fc->uv[k][0] = b[k * 2];
            fc->uv[k][1] = b[k * 2 + 1];
        }
        memcpy(fc->rgb, b + 6, 9);
        fc->semi = b[15];
    }
    if (!rd32(f, &v) || v > 16384 || !rd32(f, &n) || n > 128) {
        goto done;
    }
    m->nvert = (int)v;
    m->anims = calloc(n ? n : 1, sizeof(KitMadeAnim));
    m->nanims = (int)n;
    for (i = 0; i < n; i++) {
        KitMadeAnim *a = &m->anims[i];
        size_t count;

        if (!rd(f, a->name, 32) || !rd32(f, &v) || v == 0 || v > 4096) {
            goto done;
        }
        a->name[31] = 0;
        a->frames = (int)v;
        count = (size_t)a->frames * m->nvert * 3;
        a->verts = malloc(count * sizeof(int16_t));
        if (a->verts == NULL || !rd(f, a->verts, count * sizeof(int16_t))) {
            goto done;
        }
    }
    for (i = 0; i < (uint32_t)m->nfaces; i++) {
        for (k = 0; k < 3; k++) {
            if (m->faces[i].v[k] < 0 || m->faces[i].v[k] >= m->nvert) {
                goto done;
            }
        }
    }
    ok = 1;
done:
    fclose(f);
    if (!ok) {
        kit_made_free(m);
    }
    return ok;
}
