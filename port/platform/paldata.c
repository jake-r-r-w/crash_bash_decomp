/* The European disc (SCES-02834) for the port, which plays the USA game's C (docs/PORT.md, The European disc).
 *
 * The USA game reads CRASHBSH.DAT through the USA disc's table of its files. The European disc's has the same files in its own order,
 * 844 of the 977 the port reads byte for byte, the rest its own versions (a texture redrawn, a model with a few numbers changed), and
 * more besides: five languages' text and speech. paltable.c, made from both discs by port/pal_table.mjs, says where each USA file is in
 * it. This writes a CRASHBSH.DAT of those files in the USA's order, the English where there are five, with their table at its end, which
 * archive.c reads in place of the USA's. Two of them the code picks pictures from by number, so their pictures are put in the USA's
 * order: the font, which has the accented letters too, and the interface, which hasn't the USA's four player tags (those are clear).
 * Nothing of either game is in the port: only where things are. The whole European file is checked first, by its CRC, so another
 * pressing's isn't made into something wrong. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "paldata.h"

#define SECTOR 2048
#define CHUNK (1024 * 1024)

int pal_is_size(int64_t size) {
    return size == (int64_t)PalDatSize;
}

static uint16_t u16(const uint8_t *p) {
    return (uint16_t)(p[0] | p[1] << 8);
}

static uint32_t u32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

static void put32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

/* A texture set's pictures in another order (docs/ASSETS.md, Texture sets): picture i of the new one is the old one's order[i], or, for
   -1, a clear one of the size given; the old ones not named follow in their own order. Its colours and the rest stay as they were. NULL
   if it isn't a texture set this can do (an animated one names its pictures by number). */
static uint8_t *reorder(const uint8_t *b, uint32_t size, const short *order, int n, const unsigned short *clear, uint32_t *out_size) {
    uint32_t images, start, at, i, k, total;
    uint32_t *from, *len;
    uint8_t *used, *out;
    int j;

    if (size < 0x20 || u32(b) != 8 || u32(b + 4) != size || u32(b + 0x18) != 0) {
        return NULL;
    }
    images = u16(b + 8);
    start = 0x0C + u32(b + 0x0C);
    from = calloc(images + 1, sizeof(uint32_t));
    len = calloc(images + 1, sizeof(uint32_t));
    used = calloc(images + 1, 1);
    if (from == NULL || len == NULL || used == NULL) {
        goto fail;
    }
    for (at = start, i = 0; i < images; i++) {
        if (at + 20 > size) {
            goto fail;
        }
        from[i] = at;
        len[i] = 20 + (uint32_t)u16(b + at) * u16(b + at + 2) * 2;
        if (at + len[i] > size) {
            goto fail;
        }
        at += len[i];
    }
    /* the new size: the head, each picture named, the others, and what follows them */
    total = start + (size - at);
    for (j = 0; j < n; j++) {
        if (order[j] >= (int)images) {
            goto fail;
        }
        total += order[j] < 0 ? 20 + (uint32_t)clear[0] * clear[1] * 2 : len[order[j]];
        if (order[j] >= 0) {
            used[order[j]] = 1;
        }
    }
    for (i = 0; i < images; i++) {
        total += used[i] ? 0 : len[i];
    }
    out = malloc(total);
    if (out == NULL) {
        goto fail;
    }
    memcpy(out, b, start);
    k = start;
    for (j = 0; j < n; j++) {
        if (order[j] >= 0) {
            memcpy(out + k, b + from[order[j]], len[order[j]]);
            k += len[order[j]];
        } else {
            /* a clear one: the first picture's head (its colours and depth), this size, every texel the colour 0, which is clear */
            uint32_t px = (uint32_t)clear[0] * clear[1] * 2;

            memcpy(out + k, b + from[0], 20);
            out[k] = (uint8_t)clear[0];
            out[k + 1] = (uint8_t)(clear[0] >> 8);
            out[k + 2] = (uint8_t)clear[1];
            out[k + 3] = (uint8_t)(clear[1] >> 8);
            memset(out + k + 20, 0, px);
            k += 20 + px;
        }
    }
    for (i = 0; i < images; i++) {
        if (!used[i]) {
            memcpy(out + k, b + from[i], len[i]);
            k += len[i];
        }
    }
    memcpy(out + k, b + at, size - at);
    k += size - at;
    put32(out + 4, k);
    {
        uint32_t count = (uint32_t)n;

        for (i = 0; i < images; i++) {
            count += !used[i];
        }
        out[8] = (uint8_t)count;
        out[9] = (uint8_t)(count >> 8);
    }
    free(from);
    free(len);
    free(used);
    *out_size = k;
    return out;
fail:
    free(from);
    free(len);
    free(used);
    return NULL;
}

static int write_all(SDL_IOStream *to, const void *buf, size_t size) {
    return size == 0 || SDL_WriteIO(to, buf, size) == size;
}

const char *pal_convert(PalRead read, void *ctx, const char *out, void (*progress)(double done)) {
    const char *problem = NULL;
    uint8_t *buf = malloc(CHUNK), *file = NULL, zeros[SECTOR] = { 0 };
    uint32_t crc = 0;
    uint8_t table[PAL_FILES * 8 + 16];
    uint64_t at, written = 0;
    char *temp = NULL;
    SDL_IOStream *to = NULL;
    int e;

    if (buf == NULL) {
        return "Out of memory.";
    }
    /* the whole European file, by its CRC: the one paltable.c was made from */
    for (at = 0; at < PalDatSize; at += CHUNK) {
        size_t want = PalDatSize - at < CHUNK ? (size_t)(PalDatSize - at) : CHUNK;

        if (!read(ctx, at, buf, want)) {
            problem = "The European disc's data can't be read: the image may be damaged.";
            goto done;
        }
        crc = SDL_crc32(crc, buf, want);
        if (progress != NULL) {
            progress(0.5 * (double)at / PalDatSize);
        }
    }
    if (crc != PalDatCrc) {
        problem = "That European disc isn't the one this knows (SCES-02834): its data isn't the same. It may be damaged, or another pressing.";
        goto done;
    }
    if (SDL_asprintf(&temp, "%s.part", out) < 0) {
        temp = NULL;
    }
    to = temp != NULL ? SDL_IOFromFile(temp, "wb") : NULL;
    if (to == NULL) {
        problem = "The data can't be written. Is the disk full?";
        goto done;
    }
    /* each USA file, from the European one, a sector apart as in the game's own */
    for (e = 0; e < PAL_FILES; e++) {
        const PalFile *f = &PalFiles[e];
        uint32_t size = f->kind == PAL_NONE ? 0 : f->size, made = size;
        const uint8_t *bytes;
        uint8_t *redone = NULL;

        file = size != 0 ? malloc(size) : NULL;
        if (size != 0 && (file == NULL || !read(ctx, (uint64_t)f->sector * SECTOR, file, size))) {
            problem = file == NULL ? "Out of memory." : "The European disc's data can't be read: the image may be damaged.";
            goto done;
        }
        bytes = file;
        if (f->kind == PAL_FONT || f->kind == PAL_INTERFACE) {
            redone = f->kind == PAL_FONT ? reorder(file, size, PalFontOrder, PAL_FONT_GLYPHS, PalTagSize, &made)
                                         : reorder(file, size, PalInterfaceOrder, PAL_INTERFACE_PICTURES, PalTagSize, &made);
            if (redone == NULL) {
                problem = "The European disc's font or interface isn't as this expects.";
                goto done;
            }
            bytes = redone;
        }
        put32(table + e * 8, (uint32_t)(written / SECTOR));
        put32(table + e * 8 + 4, made);
        if (!write_all(to, bytes, made) || !write_all(to, zeros, (SECTOR - made % SECTOR) % SECTOR)) {
            free(redone);
            problem = "The data can't be written. Is the disk full?";
            goto done;
        }
        written += (made + SECTOR - 1) / SECTOR * SECTOR;
        free(redone);
        free(file);
        file = NULL;
        if (progress != NULL && e % 32 == 0) {
            progress(0.5 + 0.5 * e / PAL_FILES);
        }
    }
    /* their table, then its size and the mark that says it's there */
    put32(table + PAL_FILES * 8, PAL_FILES);
    put32(table + PAL_FILES * 8 + 4, 1);
    memcpy(table + PAL_FILES * 8 + 8, PAL_TABLE_MAGIC, 8);
    if (!write_all(to, table, sizeof(table))) {
        problem = "The data can't be written. Is the disk full?";
        goto done;
    }
    if (!SDL_CloseIO(to)) {
        problem = "The data can't be written. Is the disk full?";
    }
    to = NULL;
    if (problem == NULL && !SDL_RenamePath(temp, out)) {
        problem = "The data can't be put in place.";
    }
    if (problem == NULL && progress != NULL) {
        progress(1);
    }
done:
    if (to != NULL) {
        SDL_CloseIO(to);
    }
    if (temp != NULL) {
        if (problem != NULL) {
            SDL_RemovePath(temp);
        }
        SDL_free(temp);
    }
    free(file);
    free(buf);
    return problem;
}

static int read_file(void *ctx, uint64_t at, void *buf, size_t size) {
    SDL_IOStream *io = ctx;

    return SDL_SeekIO(io, (Sint64)at, SDL_IO_SEEK_SET) == (Sint64)at && SDL_ReadIO(io, buf, size) == size;
}

const char *pal_convert_file(const char *dat, const char *out, void (*progress)(double done)) {
    SDL_IOStream *io = SDL_IOFromFile(dat, "rb");
    const char *problem;

    if (io == NULL) {
        return "The European disc's CRASHBSH.DAT can't be opened.";
    }
    problem = pal_convert(read_file, io, out, progress);
    SDL_CloseIO(io);
    return problem;
}

int pal_is_converted(const char *path) {
    SDL_IOStream *io = path != NULL ? SDL_IOFromFile(path, "rb") : NULL;
    char mark[8];
    int is = 0;

    if (io != NULL) {
        Sint64 size = SDL_GetIOSize(io);

        is = size >= 16 && SDL_SeekIO(io, size - 8, SDL_IO_SEEK_SET) >= 0 && SDL_ReadIO(io, mark, 8) == 8 && memcmp(mark, PAL_TABLE_MAGIC, 8) == 0;
        SDL_CloseIO(io);
    }
    return is;
}

int pal_is_file(const char *path) {
    SDL_PathInfo info;

    return path != NULL && SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE && pal_is_size((int64_t)info.size);
}
