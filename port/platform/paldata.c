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
#include "platform.h"

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
   -1, a clear one of the size given; the old ones not named follow in their own order. frames, if not NULL, gives picture i's number of
   frames where it isn't the old one's (0: as it was). Its colours and the rest stay as they were. NULL if it isn't a texture set this can
   do (an animated one names its pictures by number). */
static uint8_t *reorder(const uint8_t *b, uint32_t size, const short *order, int n, const unsigned short *clear, const signed char *frames,
                        uint32_t *out_size) {
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
            if (frames != NULL && frames[j] != 0) {
                put32(out + k + 16, (uint32_t)frames[j]);
            }
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

/* A file's bytes, then zeros to as long as it's to be (its languages' longest), then to the end of its last sector */
static int write_file(SDL_IOStream *to, const uint8_t *bytes, uint32_t size, uint32_t as_long, uint64_t *written) {
    static const uint8_t zeros[SECTOR];
    uint32_t pad = as_long - size + (SECTOR - as_long % SECTOR) % SECTOR;

    if (!write_all(to, bytes, size)) {
        return 0;
    }
    while (pad > 0) {
        uint32_t n = pad < SECTOR ? pad : SECTOR;

        if (!write_all(to, zeros, n)) {
            return 0;
        }
        pad -= n;
    }
    *written += (as_long + SECTOR - 1) / SECTOR * SECTOR;
    return 1;
}

/* Which of PalLanguageFiles a USA file is, or -1 */
static int language_file(int entry) {
    int j;

    for (j = 0; j < PalLanguageFileCount; j++) {
        if (PalLanguageFiles[j].entry == entry) {
            return j;
        }
    }
    return -1;
}

/* A file's longest language: what the game is told it is, so every machine's game has the same, whichever its language */
static uint32_t longest(int j) {
    uint32_t most = PalFiles[PalLanguageFiles[j].entry].size;
    int L;

    for (L = 0; L < PAL_LANGUAGES - 1; L++) {
        most = PalLanguageFiles[j].v[L].size > most ? PalLanguageFiles[j].v[L].size : most;
    }
    return most;
}

/* A string at an offset of a blob of the European code: from the program file, or from the archive (its engine or an overlay) */
static int read_word(PalRead read, void *ctx, const uint8_t *exe, size_t exe_size, const PalString *s, int lang, char *out, size_t room) {
    size_t n;

    if (s->sector < 0) {
        if (exe == NULL || s->offs[lang] >= exe_size) {
            return 0;
        }
        for (n = 0; n + 1 < room && s->offs[lang] + n < exe_size && exe[s->offs[lang] + n] != 0; n++) {
            out[n] = (char)exe[s->offs[lang] + n];
        }
    } else {
        uint8_t buf[512];
        size_t want = s->size - s->offs[lang] < sizeof(buf) ? s->size - s->offs[lang] : sizeof(buf);

        if (s->offs[lang] >= s->size || !read(ctx, (uint64_t)s->sector * SECTOR + s->offs[lang], buf, want)) {
            return 0;
        }
        for (n = 0; n + 1 < room && n < want && buf[n] != 0; n++) {
            out[n] = (char)buf[n];
        }
    }
    out[n] = 0;
    return 1;
}

const char *pal_convert(PalRead read, void *ctx, const uint8_t *exe, size_t exe_size, const char *out, void (*progress)(double done)) {
    const char *problem = NULL;
    uint8_t *buf = malloc(CHUNK), *file = NULL;
    uint32_t crc = 0;
    /* the table at the end: each file's sector and size; each language file's entry and its four others' sectors; their counts, the
       version and the mark */
    size_t table_size = PAL_FILES * 8 + (size_t)PalLanguageFileCount * 20 + 28;
    uint8_t *table = malloc(table_size), *lang = table != NULL ? table + PAL_FILES * 8 : NULL;
    int j, L;
    uint64_t at, written = 0;
    char *temp = NULL;
    SDL_IOStream *to = NULL;
    int e;

    if (buf == NULL || table == NULL) {
        free(buf);
        free(table);
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
        uint32_t size = f->kind == PAL_NONE ? 0 : f->size, made = size, as_long;
        const uint8_t *bytes;
        uint8_t *redone = NULL;

        file = size != 0 ? malloc(size) : NULL;
        if (size != 0 && (file == NULL || !read(ctx, (uint64_t)f->sector * SECTOR, file, size))) {
            problem = file == NULL ? "Out of memory." : "The European disc's data can't be read: the image may be damaged.";
            goto done;
        }
        bytes = file;
        if (f->kind == PAL_FONT || f->kind == PAL_INTERFACE) {
            redone = f->kind == PAL_FONT ? reorder(file, size, PalFontOrder, PAL_FONT_GLYPHS, PalTagSize, NULL, &made)
                                         : reorder(file, size, PalInterfaceOrder, PAL_INTERFACE_PICTURES, PalTagSize, NULL, &made);
            if (redone == NULL) {
                problem = "The European disc's font or interface isn't as this expects.";
                goto done;
            }
            bytes = redone;
        }
        /* a file with a version for each language is as long as its longest, here and in each language's copy (below) */
        as_long = (j = language_file(e)) >= 0 ? longest(j) : made;
        put32(table + e * 8, (uint32_t)(written / SECTOR));
        put32(table + e * 8 + 4, as_long);
        if (!write_file(to, bytes, made, as_long, &written)) {
            free(redone);
            problem = "The data can't be written. Is the disk full?";
            goto done;
        }
        free(redone);
        free(file);
        file = NULL;
        if (progress != NULL && e % 32 == 0) {
            progress(0.5 + 0.5 * e / PAL_FILES);
        }
    }
    /* the other four languages' versions, each as long as its longest, after all of them */
    for (j = 0; j < PalLanguageFileCount; j++) {
        const PalLanguageFile *lf = &PalLanguageFiles[j];

        put32(lang + j * 20, (uint32_t)lf->entry | (uint32_t)lf->kind << 24);
        for (L = 0; L < PAL_LANGUAGES - 1; L++) {
            file = malloc(lf->v[L].size);
            if (file == NULL || !read(ctx, (uint64_t)lf->v[L].sector * SECTOR, file, lf->v[L].size)) {
                problem = file == NULL ? "Out of memory." : "The European disc's data can't be read: the image may be damaged.";
                goto done;
            }
            put32(lang + j * 20 + 4 + L * 4, (uint32_t)(written / SECTOR));
            if (!write_file(to, file, lf->v[L].size, longest(j), &written)) {
                problem = "The data can't be written. Is the disk full?";
                goto done;
            }
            free(file);
            file = NULL;
        }
    }
    /* the words: each one's five languages, English first, one after another, each ending in a zero; one only the program file has, from
       a CRASHBSH.DAT alone, five empty strings (it stays English) */
    {
        uint64_t start = written, size = 0;
        int k;

        for (j = 0; j < PalStringCount; j++) {
            char word[512];
            int have = 1;

            for (L = 0; L < PAL_LANGUAGES && have; L++) {
                have = read_word(read, ctx, exe, exe_size, &PalStrings[j], L, word, sizeof(word));
            }
            for (L = 0; L < PAL_LANGUAGES; L++) {
                if (!have || !read_word(read, ctx, exe, exe_size, &PalStrings[j], L, word, sizeof(word))) {
                    word[0] = 0;
                }
                if (!write_all(to, word, strlen(word) + 1)) {
                    problem = "The data can't be written. Is the disk full?";
                    goto done;
                }
                size += strlen(word) + 1;
            }
        }
        k = (int)((SECTOR - size % SECTOR) % SECTOR);
        while (k-- > 0) {
            if (!write_all(to, "", 1)) {
                problem = "The data can't be written. Is the disk full?";
                goto done;
            }
        }
        written += (size + SECTOR - 1) / SECTOR * SECTOR;
        put32(table + table_size - 28, (uint32_t)(start / SECTOR));
        put32(table + table_size - 24, (uint32_t)size);
    }
    /* their table, then its counts, its version and the mark that says it's there */
    put32(table + table_size - 20, (uint32_t)PalLanguageFileCount);
    put32(table + table_size - 16, PAL_FILES);
    put32(table + table_size - 12, PAL_TABLE_VERSION);
    memcpy(table + table_size - 8, PAL_TABLE_MAGIC, 8);
    if (!write_all(to, table, table_size)) {
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
    free(table);
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
    problem = pal_convert(read_file, io, NULL, 0, out, progress);
    SDL_CloseIO(io);
    return problem;
}

/* Where each picture of a texture set is, up to n of them: how many it has, or -1 if it isn't one */
static int picture_places(const uint8_t *b, uint32_t size, uint32_t *at, int n) {
    uint32_t p, i, images;

    if (size < 0x20 || u32(b) != 8) {
        return -1;
    }
    images = u16(b + 8);
    for (p = 0x0C + u32(b + 0x0C), i = 0; i < images; i++) {
        if (p + 20 > size || p + 20 + (uint32_t)u16(b + p) * u16(b + p + 2) * 2 > size) {
            return -1;
        }
        if ((int)i < n) {
            at[i] = p;
        }
        p += 20 + (uint32_t)u16(b + p) * u16(b + p + 2) * 2;
    }
    return (int)images;
}

uint8_t *pal_put_right(int entry, const uint8_t *bytes, uint32_t size, int language) {
    static uint32_t at[512];
    uint8_t *out;
    int i, had;

    if (entry < 0 || entry >= PAL_FILES || PalFiles[entry].kind != PAL_PICTURES || (had = picture_places(bytes, size, at, 512)) <= 0 ||
        had > 512 || (out = malloc(size)) == NULL) {
        return NULL;
    }
    /* its pictures stay in the European order, which its models (083) name them by: the game's code names them by the USA's, which
       pal_mainmenu_picture gives (prims.c). Each USA picture's frames, where the European has its languages' after them (the player tags),
       and the first of those the language's, the same size, with its colours. */
    memcpy(out, bytes, size);
    for (i = 0; i < PAL_PICTURES_COUNT; i++) {
        int j = PalPicturesOrder[i], from = PalPicturesLanguage[i] < 0 ? -1 : PalPicturesLanguage[i] + language;

        if (j < 0 || j >= had) {
            continue;
        }
        if (PalPicturesFrames[i] != 0) {
            put32(out + at[j] + 16, (uint32_t)PalPicturesFrames[i]);
        }
        if (language > 0 && language < PAL_LANGUAGES && from >= 0 && from < had && u16(bytes + at[from]) == u16(bytes + at[j]) &&
            u16(bytes + at[from] + 2) == u16(bytes + at[j] + 2)) {
            memcpy(out + at[j] + 12, bytes + at[from] + 12, 2);
            memcpy(out + at[j] + 20, bytes + at[from] + 20, (size_t)u16(bytes + at[j]) * u16(bytes + at[j] + 2) * 2);
        }
    }
    return out;
}

int pal_mainmenu_picture(int usa) {
    return archive_from_europe() && usa >= 0 && usa < PAL_PICTURES_COUNT && PalPicturesOrder[usa] >= 0 ? PalPicturesOrder[usa] : usa;
}

int pal_mainmenu_usa(int picture) {
    int i;

    if (!archive_from_europe()) {
        return picture;
    }
    for (i = 0; i < PAL_PICTURES_COUNT; i++) {
        if (PalPicturesOrder[i] == picture) {
            return i;
        }
    }
    return -1;
}

int pal_is_converted(const char *path) {
    SDL_IOStream *io = path != NULL ? SDL_IOFromFile(path, "rb") : NULL;
    char mark[8];
    int is = 0;

    if (io != NULL) {
        Sint64 size = SDL_GetIOSize(io);

        uint8_t version[4];

        /* this version's: one made before the languages is made again */
        is = size >= 16 && SDL_SeekIO(io, size - 12, SDL_IO_SEEK_SET) >= 0 && SDL_ReadIO(io, version, 4) == 4 && SDL_ReadIO(io, mark, 8) == 8 &&
             memcmp(mark, PAL_TABLE_MAGIC, 8) == 0 && u32(version) == PAL_TABLE_VERSION;
        SDL_CloseIO(io);
    }
    return is;
}

int pal_is_file(const char *path) {
    SDL_PathInfo info;

    return path != NULL && SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE && pal_is_size((int64_t)info.size);
}

void pal_font_glyphs(int16_t *lookup, uint8_t *flags) {
    int i;

    if (!archive_from_europe()) {
        return;
    }
    for (i = 0; i < PalGlyphCount; i++) {
        lookup[PalGlyphs[i].page * 256 + PalGlyphs[i].code] = (int16_t)(0x3000 | PalGlyphs[i].glyph);
        flags[PalGlyphs[i].page * 256 + PalGlyphs[i].code] = PalGlyphs[i].flags;
    }
}

int pal_mode_text(int language, unsigned *base, const unsigned short **roots) {
    if (!archive_from_europe() || language < 0 || language >= PAL_LANGUAGES) {
        return 0;
    }
    *base = PalModeTextBase;
    *roots = PalModeTextRoots[language];
    return 1;
}

int pal_mode_text_model(int model) {
    int i;

    for (i = 0; i < PalModeTextModelCount; i++) {
        if (PalModeTextModels[i][0] == model) {
            return PalModeTextModels[i][1];
        }
    }
    return model;
}

int pal_mode_text_glyph(int glyph) {
    int i, n = PAL_FONT_GLYPHS;

    for (i = 0; i < PAL_FONT_GLYPHS; i++) {
        if (PalFontOrder[i] == glyph) {
            return i;
        }
    }
    /* one of the European font's own, after the USA's in the made font, in its order */
    for (i = 0; i < glyph; i++) {
        int j, used = 0;

        for (j = 0; j < PAL_FONT_GLYPHS && !used; j++) {
            used = PalFontOrder[j] == i;
        }
        n += !used;
    }
    return n;
}
