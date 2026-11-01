/* Texture packs: a folder of PNGs that replace the game's textures, each named after the 16 hex digits of the hash of what it replaces. The
 * hash is of the texture's own pixels and palette, so it doesn't depend on where or when it's loaded. The port ships none. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "texpack.h"

#define MAX_FILE_BYTES (64 * 1024 * 1024)
#define MAX_SIDE 2048

static char Folder[1024];
static uint64_t *Hashes;
static int NumHashes;
static char DumpFolder[1024];

static int by_hash(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;

    return x < y ? -1 : x > y;
}

/* 16 hex digits, then .png */
static int hash_of_name(const char *name, uint64_t *out) {
    uint64_t v = 0;
    int i;

    if (strlen(name) != 20 || SDL_strcasecmp(name + 16, ".png") != 0) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        char c = name[i];
        int d = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;

        if (d < 0) {
            return 0;
        }
        v = v << 4 | (uint64_t)d;
    }
    *out = v;
    return 1;
}

static int is_folder(const char *path) {
    SDL_PathInfo info;

    return SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY;
}

/* Where packs are kept: next to the program, in the user's own folder, and here. */
static int pack_roots(char roots[3][1024]) {
    const char *base = SDL_GetBasePath();
    char *pref = SDL_GetPrefPath("CrashBash", "CrashBash");
    int n = 0;

    if (base != NULL) {
        SDL_snprintf(roots[n++], 1024, "%spacks/", base);
    }
    if (pref != NULL) {
        SDL_snprintf(roots[n++], 1024, "%spacks/", pref);
        SDL_free(pref);
    }
    SDL_snprintf(roots[n++], 1024, "packs/");
    return n;
}

void texpack_select(const char *name) {
    char roots[3][1024];
    char **names;
    int count = 0, i, r, n;

    free(Hashes);
    Hashes = NULL;
    NumHashes = 0;
    Folder[0] = 0;
    if (name == NULL || name[0] == 0) {
        return;
    }
    if (strchr(name, '/') != NULL || strchr(name, '\\') != NULL) {
        char path[1024];

        SDL_snprintf(path, sizeof(path), "%s/", name);
        if (is_folder(path)) {
            SDL_strlcpy(Folder, path, sizeof(Folder));
        }
    } else {
        n = pack_roots(roots);
        for (r = 0; r < n && Folder[0] == 0; r++) {
            char path[1024];

            SDL_snprintf(path, sizeof(path), "%s%s/", roots[r], name);
            if (is_folder(path)) {
                SDL_strlcpy(Folder, path, sizeof(Folder));
            }
        }
    }
    if (Folder[0] == 0) {
        fprintf(stderr, "texture pack: no %s\n", name);
        return;
    }
    names = SDL_GlobDirectory(Folder, "*.png", SDL_GLOB_CASEINSENSITIVE, &count);
    Hashes = count != 0 ? malloc(sizeof(uint64_t) * (size_t)count) : NULL;
    for (i = 0; Hashes != NULL && i < count; i++) {
        uint64_t h;

        if (hash_of_name(names[i], &h)) {
            Hashes[NumHashes++] = h;
        }
    }
    SDL_free(names);
    printf("texture pack %s: %d textures\n", Folder, NumHashes);
    if (NumHashes == 0) {
        Folder[0] = 0;
        return;
    }
    qsort(Hashes, (size_t)NumHashes, sizeof(uint64_t), by_hash);
}

int texpack_active(void) {
    return Folder[0] != 0;
}

int texpack_has(uint64_t hash) {
    return NumHashes != 0 && bsearch(&hash, Hashes, (size_t)NumHashes, sizeof(uint64_t), by_hash) != NULL;
}

void texpack_drop(uint64_t hash) {
    uint64_t *at = NumHashes != 0 ? bsearch(&hash, Hashes, (size_t)NumHashes, sizeof(uint64_t), by_hash) : NULL;

    if (at != NULL) {
        memmove(at, at + 1, sizeof(uint64_t) * (size_t)(Hashes + NumHashes - at - 1));
        NumHashes--;
    }
}

/* Packs come from anyone, and SDL's PNG loader is meant for files you trust, so a file is sized up from its header first: its size and its
   dimensions, as what a file unpacks to is what could do harm. */
static int looks_fine(const char *path) {
    static const uint8_t Signature[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
    uint8_t head[24];
    SDL_IOStream *io = SDL_IOFromFile(path, "rb");
    Sint64 size;
    uint32_t w, h;
    int ok;

    if (io == NULL) {
        return 0;
    }
    size = SDL_GetIOSize(io);
    ok = size > 0 && size <= MAX_FILE_BYTES && SDL_ReadIO(io, head, sizeof(head)) == sizeof(head);
    SDL_CloseIO(io);
    if (!ok || memcmp(head, Signature, sizeof(Signature)) != 0 || memcmp(head + 12, "IHDR", 4) != 0) {
        return 0;
    }
    w = (uint32_t)head[16] << 24 | (uint32_t)head[17] << 16 | (uint32_t)head[18] << 8 | head[19];
    h = (uint32_t)head[20] << 24 | (uint32_t)head[21] << 16 | (uint32_t)head[22] << 8 | head[23];
    return w >= 1 && h >= 1 && w <= MAX_SIDE && h <= MAX_SIDE;
}

uint32_t *texpack_load(uint64_t hash, int *width, int *height) {
    char path[1200];
    SDL_Surface *loaded, *rgba;
    uint32_t *out = NULL;

    SDL_snprintf(path, sizeof(path), "%s%016llx.png", Folder, (unsigned long long)hash);
    if (!looks_fine(path)) {
        /* it may have been named in capitals, on a system that minds */
        SDL_snprintf(path, sizeof(path), "%s%016llX.png", Folder, (unsigned long long)hash);
        if (!looks_fine(path)) {
            fprintf(stderr, "texture pack: %016llx isn't a PNG it can use, up to %d x %d\n", (unsigned long long)hash, MAX_SIDE, MAX_SIDE);
            return NULL;
        }
    }
    loaded = SDL_LoadPNG(path);
    if (loaded == NULL) {
        fprintf(stderr, "texture pack: %s: %s\n", path, SDL_GetError());
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

void texpack_dump(uint64_t hash, const uint32_t *texels, int w, int h, int pitch) {
    char path[1200];
    SDL_Surface *s;

    SDL_snprintf(path, sizeof(path), "%s%016llx.png", DumpFolder, (unsigned long long)hash);
    if (SDL_GetPathInfo(path, NULL)) {
        return;
    }
    s = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_ABGR8888, (void *)texels, pitch);
    if (s != NULL) {
        SDL_SavePNG(s, path);
        SDL_DestroySurface(s);
    }
}

int texpack_list(char (*names)[TEXPACK_NAME], int max) {
    char roots[3][1024];
    int n = pack_roots(roots), r, count = 0;

    for (r = 0; r < n; r++) {
        int found = 0, i;
        char **dirs = SDL_GlobDirectory(roots[r], "*", 0, &found);

        for (i = 0; dirs != NULL && i < found && count < max; i++) {
            char path[1200];
            int have = 0, j;

            SDL_snprintf(path, sizeof(path), "%s%s", roots[r], dirs[i]);
            for (j = 0; j < count; j++) {
                have |= SDL_strcasecmp(names[j], dirs[i]) == 0;
            }
            if (!have && is_folder(path) && strlen(dirs[i]) < TEXPACK_NAME) {
                SDL_strlcpy(names[count++], dirs[i], TEXPACK_NAME);
            }
        }
        SDL_free(dirs);
    }
    return count;
}
