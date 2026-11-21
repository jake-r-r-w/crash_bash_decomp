/* Reading a model (kit.h): the game's own reader, then the game's limits checked. A model over them is refused, never changed: the
 * person who made it decides what to take out. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "kit.h"

static void say(char *why, size_t size, const char *fmt, int a, int b) {
    snprintf(why, size, fmt, a, b);
}

int kit_read(const char *path, KitModel *m, char *why, size_t why_size) {
    static PackModel pm;
    size_t size = 0;
    void *glb = SDL_LoadFile(path, &size);
    char problem[64];
    int i, j;

    memset(m, 0, sizeof(*m));
    m->arm_raise = 160;
    if (glb == NULL) {
        snprintf(why, why_size, "can't read %s", path);
        return 0;
    }
    if (!pack_model_read(glb, size, &pm, problem, sizeof(problem))) {
        SDL_free(glb);
        snprintf(why, why_size, "%s isn't a model the game can read: %s", path, problem);
        return 0;
    }
    SDL_free(glb);
    if (pm.ntri > KIT_MAX_TRIS) {
        say(why, why_size, "the model has %d triangles, and the game draws a hero of %d at most: take some out in your modelling program "
            "(or export it with fewer) and try again", pm.ntri, KIT_MAX_TRIS);
        pack_model_free(&pm);
        return 0;
    }
    if (pm.nvert > KIT_MAX_VERTS) {
        say(why, why_size, "the model has %d vertices, and the game draws a hero of %d at most", pm.nvert, KIT_MAX_VERTS);
        pack_model_free(&pm);
        return 0;
    }
    m->nvert = pm.nvert;
    m->ntri = pm.ntri;
    m->pos = malloc(sizeof(*m->pos) * pm.nvert);
    m->uv = malloc(sizeof(*m->uv) * pm.nvert);
    m->rgba = malloc(sizeof(*m->rgba) * pm.nvert);
    m->image = malloc(sizeof(int) * pm.nvert);
    m->tri = malloc(sizeof(*m->tri) * pm.ntri);
    m->tri_image = malloc(sizeof(int) * pm.ntri);
    m->joint = calloc(pm.nvert, sizeof(*m->joint));
    m->weight = calloc(pm.nvert, sizeof(*m->weight));
    if (m->pos == NULL || m->uv == NULL || m->rgba == NULL || m->image == NULL || m->tri == NULL || m->tri_image == NULL
        || m->joint == NULL || m->weight == NULL) {
        snprintf(why, why_size, "no memory");
        pack_model_free(&pm);
        kit_free(m);
        return 0;
    }
    for (j = 0; j < 3; j++) {
        m->lo[j] = 1e30f;
        m->hi[j] = -1e30f;
    }
    for (i = 0; i < pm.nvert; i++) {
        memcpy(m->pos[i], &pm.pos[i * 3], sizeof(m->pos[i]));
        memcpy(m->uv[i], &pm.uv[i * 2], sizeof(m->uv[i]));
        memcpy(m->rgba[i], &pm.rgba[i * 4], sizeof(m->rgba[i]));
        m->image[i] = -1;
        for (j = 0; j < 3; j++) {
            m->lo[j] = m->pos[i][j] < m->lo[j] ? m->pos[i][j] : m->lo[j];
            m->hi[j] = m->pos[i][j] > m->hi[j] ? m->pos[i][j] : m->hi[j];
        }
    }
    for (i = 0; i < pm.ntri; i++) {
        memcpy(m->tri[i], &pm.tri[i * 3], sizeof(m->tri[i]));
        m->tri_image[i] = pm.tri_image[i];
        for (j = 0; j < 3; j++) {
            m->image[m->tri[i][j]] = pm.tri_image[i];
        }
    }
    /* its images are the tool's now */
    m->nimages = pm.nimages;
    for (i = 0; i < pm.nimages; i++) {
        m->images[i] = pm.images[i];
        pm.images[i].rgba = NULL;
    }
    pm.nimages = 0;
    pack_model_free(&pm);
    printf("herotool: %s: %d triangles, %d vertices, %d textures, %.3f tall\n", path, m->ntri, m->nvert, m->nimages, m->hi[1] - m->lo[1]);
    return 1;
}

void kit_free(KitModel *m) {
    int i;

    free(m->pos);
    free(m->uv);
    free(m->rgba);
    free(m->image);
    free(m->tri);
    free(m->tri_image);
    free(m->joint);
    free(m->weight);
    free(m->atlas);
    SDL_free(m->png);
    for (i = 0; i < m->nimages; i++) {
        free(m->images[i].rgba);
    }
    memset(m, 0, sizeof(*m));
}
