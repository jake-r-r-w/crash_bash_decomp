/* Textures in place of the game's, from mods (mods.c): PNGs, each named after the 16 hex digits of the hash of what it replaces. The hash
 * is of the texture's own pixels and palette, so it doesn't depend on where or when it's loaded. And saving the game's own as they're
 * drawn, named that way, for making them. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "mods.h"
#include "texpack.h"

#define MAX_SIDE 2048

static char DumpFolder[1024];

int texpack_active(void) {
    return mods_textures_on();
}

int texpack_has(uint64_t hash) {
    return mods_texture_has(hash);
}

void texpack_drop(uint64_t hash) {
    mods_texture_drop(hash);
}

unsigned texpack_generation(void) {
    return mods_textures_generation();
}

/* Mods come from anyone, and SDL's PNG loader is meant for files you trust, so a file is sized up from its header first: its
   dimensions, as what a file unpacks to is what could do harm. mods.c has already limited its size. */
static int looks_fine(const uint8_t *bytes, size_t size) {
    static const uint8_t Signature[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
    uint32_t w, h;

    if (size < 24 || memcmp(bytes, Signature, sizeof(Signature)) != 0 || memcmp(bytes + 12, "IHDR", 4) != 0) {
        return 0;
    }
    w = (uint32_t)bytes[16] << 24 | (uint32_t)bytes[17] << 16 | (uint32_t)bytes[18] << 8 | bytes[19];
    h = (uint32_t)bytes[20] << 24 | (uint32_t)bytes[21] << 16 | (uint32_t)bytes[22] << 8 | bytes[23];
    return w >= 1 && h >= 1 && w <= MAX_SIDE && h <= MAX_SIDE;
}

uint32_t *texpack_load(uint64_t hash, int *width, int *height) {
    size_t size = 0;
    uint8_t *bytes = mods_texture_read(hash, &size);
    SDL_IOStream *io;
    SDL_Surface *loaded, *rgba;
    uint32_t *out = NULL;

    if (bytes == NULL) {
        return NULL;
    }
    if (!looks_fine(bytes, size)) {
        fprintf(stderr, "texture %016llx isn't a PNG it can use, up to %d x %d\n", (unsigned long long)hash, MAX_SIDE, MAX_SIDE);
        free(bytes);
        return NULL;
    }
    io = SDL_IOFromConstMem(bytes, size);
    loaded = io != NULL ? SDL_LoadPNG_IO(io, true) : NULL;
    free(bytes);
    if (loaded == NULL) {
        fprintf(stderr, "texture %016llx: %s\n", (unsigned long long)hash, SDL_GetError());
        return NULL;
    }
    rgba = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_ABGR8888);
    SDL_DestroySurface(loaded);
    if (rgba == NULL || rgba->w > MAX_SIDE || rgba->h > MAX_SIDE) {
        SDL_DestroySurface(rgba);
        return NULL;
    }
    out = malloc(sizeof(uint32_t) * (size_t)rgba->w * rgba->h);
    if (out != NULL) {
        int x, y;

        for (y = 0; y < rgba->h; y++) {
            const uint32_t *row = (const uint32_t *)((const uint8_t *)rgba->pixels + (size_t)y * rgba->pitch);

            for (x = 0; x < rgba->w; x++) {
                /* a texel's alpha says what kind it is, which is all the drawing understands: clear, semi-transparent or opaque */
                uint32_t a = row[x] >> 24;

                out[y * rgba->w + x] = (row[x] & 0xFFFFFF) | ((a < 64 ? 0u : a < 192 ? 128u : 255u) << 24);
            }
        }
        *width = rgba->w;
        *height = rgba->h;
    }
    SDL_DestroySurface(rgba);
    return out;
}

void texpack_dump_to(const char *folder) {
    SDL_CreateDirectory(folder);
    SDL_snprintf(DumpFolder, sizeof(DumpFolder), "%s/", folder);
}

int texpack_dumping(void) {
    return DumpFolder[0] != 0;
}

void texpack_dump(uint64_t hash, const uint32_t *texels, int w, int h, int pitch, int top) {
    char path[1200];
    SDL_Surface *s;
    uint32_t *rest;
    int y;

    SDL_snprintf(path, sizeof(path), "%s%016llx.png", DumpFolder, (unsigned long long)hash);
    if (SDL_GetPathInfo(path, NULL)) {
        return;
    }
    rest = malloc(sizeof(uint32_t) * (size_t)w * h);
    if (rest == NULL) {
        return;
    }
    for (y = 0; y < h; y++) {
        memcpy(rest + (y + top) % h * w, (const uint8_t *)texels + (size_t)y * pitch, (size_t)w * 4);
    }
    s = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_ABGR8888, rest, w * 4);
    if (s != NULL) {
        SDL_SavePNG(s, path);
        SDL_DestroySurface(s);
    }
    free(rest);
}
