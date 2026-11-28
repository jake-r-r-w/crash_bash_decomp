/* The game's archive, CRASHBSH.DAT: a flat run of 2 KB sectors from sector 0, and every read is finished by the time it returns.
 *
 * Mods can put files of their own in place of the archive's (mods.c). A replacement can be bigger than the file it replaces, so the files
 * after it are moved on: the game's table of where its files are (DatIndex) and how much of the archive each group of them is
 * (FileGroups, read in one go) are laid out again, and a read finds each sector where it is now, in the archive or in a mod's file. With
 * nothing replaced, the tables are the game's own and a read is a plain read. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "paldata.h"
#include "platform.h"
#include "settings.h"

#define SECTOR_BYTES 2048
#define SECTORS(bytes) (((bytes) + SECTOR_BYTES - 1) / SECTOR_BYTES)

/* The game's own tables, as main.c has them */
typedef struct {
    int32_t sector, size;
} ArchiveEntry;
typedef struct {
    int32_t first, count, size;
} ArchiveGroup;
extern ArchiveEntry DatIndex[];
extern int32_t DatIndexCount;
extern ArchiveGroup FileGroups[];
extern int32_t FileGroupCount;

static SDL_IOStream *Dat;
static int Converted; /* the data was made from the European disc (paldata.c) */
/* And its languages: for each file with a version for each, where the French's, German's, Italian's and Spanish's are in the file (its
   own sector, as it was made), or 0; and the one this machine reads (settings.h, language: never the game's own, which stays English) */
static int32_t (*LangSector)[PAL_LANGUAGES - 1];
static uint8_t *LangKind; /* PAL_TEXT (read in its language) or PAL_SOUND (read in English: audio.c plays its language) */
static int Languages;
/* Files the archive has otherwise than the game reads them (paldata.c, pal_put_right), by entry, or NULL: as the archive has them, and
   as the game reads them in the language they were last made in */
static uint8_t **PutRightStored, **PutRight;
static int NumPutRight, PutRightLanguage = -1;

/* What the tables were before anything was replaced, and the replacements by entry */
static ArchiveEntry *Original;
static int32_t *OriginalGroupSize;
static uint8_t **Replaced;
static int32_t *ReplacedSize;
static int NumReplaced;

/* A CRASHBSH.DAT made from the European disc (paldata.c) has the USA's files in another place each, and ends with their table: it's
   the game's table from here on, before anything else reads it (main.c opens the archive first), and each group's size changes by as
   much as the span of its files does. */
static uint32_t le32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

/* The words in the five languages (paldata.c), and this machine's for each English one */
static char *Words;
static const char *(*WordSet)[PAL_LANGUAGES];
static char (*WordLetters)[64];
static int NumWords;

static void letters_of(const char *s, char *out, size_t room) {
    size_t n = 0;

    for (; *s != 0 && n + 1 < room; s++) {
        char c = *s >= 'a' && *s <= 'z' ? (char)(*s - 'a' + 'A') : *s;

        if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            out[n++] = c;
        }
    }
    out[n] = 0;
}

static void read_words(uint32_t sector, uint32_t size) {
    uint32_t i, n = 0;
    char *p;

    if (size == 0 || (Words = malloc(size + 1)) == NULL) {
        return;
    }
    if (SDL_SeekIO(Dat, (Sint64)sector * SECTOR_BYTES, SDL_IO_SEEK_SET) < 0 || SDL_ReadIO(Dat, Words, size) != size) {
        free(Words);
        Words = NULL;
        return;
    }
    Words[size] = 0;
    for (i = 0; i < size; i++) {
        n += Words[i] == 0;
    }
    NumWords = (int)(n / PAL_LANGUAGES);
    WordSet = calloc((size_t)NumWords + 1, sizeof(*WordSet));
    WordLetters = calloc((size_t)NumWords + 1, sizeof(*WordLetters));
    if (WordSet == NULL || WordLetters == NULL) {
        NumWords = 0;
        return;
    }
    for (p = Words, i = 0; i < (uint32_t)NumWords * PAL_LANGUAGES; i++) {
        WordSet[i / PAL_LANGUAGES][i % PAL_LANGUAGES] = p;
        p += strlen(p) + 1;
    }
    for (i = 0; i < (uint32_t)NumWords; i++) {
        letters_of(WordSet[i][0], WordLetters[i], sizeof(WordLetters[i]));
    }
}

/* A converted archive's files whose pictures are in the European order, in the USA's (PAL_PICTURES): the same size, so nothing moves */
static void put_right(int32_t count) {
    int32_t e;

    for (e = 0; e < count && e < PAL_FILES; e++) {
        uint8_t *bytes;

        if ((PalFiles[e].kind != PAL_PICTURES && PalFiles[e].kind != PAL_INTERFACE) || DatIndex[e].size <= 0) {
            continue;
        }
        bytes = malloc((size_t)DatIndex[e].size);
        if (bytes == NULL || SDL_SeekIO(Dat, (Sint64)DatIndex[e].sector * SECTOR_BYTES, SDL_IO_SEEK_SET) < 0 ||
            SDL_ReadIO(Dat, bytes, (size_t)DatIndex[e].size) != (size_t)DatIndex[e].size) {
            free(bytes);
            continue;
        }
        if (PutRight == NULL && ((PutRight = calloc((size_t)count, sizeof(uint8_t *))) == NULL ||
                                 (PutRightStored = calloc((size_t)count, sizeof(uint8_t *))) == NULL)) {
            free(bytes);
            return;
        }
        /* the main menu's pictures put right; an interface as it is in English, the language's otherwise (pal_put_right) */
        PutRight[e] = pal_put_right(e, bytes, (uint32_t)DatIndex[e].size, 0);
        if (PalFiles[e].kind == PAL_PICTURES) {
            printf("data: file %03X's pictures %s\n", (unsigned)e, PutRight[e] != NULL ? "put right" : "aren't as this expects: left");
            if (PutRight[e] == NULL) {
                free(bytes);
                continue;
            }
        }
        PutRightStored[e] = bytes;
        NumPutRight++;
    }
    PutRightLanguage = 0;
}

static int language(void);

/* One of them as the game reads it now, in this machine's language */
static const uint8_t *put_right_now(int32_t i) {
    int32_t e;

    if (PutRightLanguage != language()) {
        PutRightLanguage = language();
        for (e = 0; e < DatIndexCount; e++) {
            uint8_t *made;

            if (PutRightStored[e] != NULL) {
                made = pal_put_right(e, PutRightStored[e], (uint32_t)(Original != NULL ? Original[e].size : DatIndex[e].size), PutRightLanguage);
                if (made != NULL || PalFiles[e].kind == PAL_INTERFACE) {
                    free(PutRight[e]);
                    PutRight[e] = made;
                }
            }
        }
    }
    /* none (an interface in English): the file as the archive has it */
    return PutRight[i] != NULL ? PutRight[i] : PutRightStored[i];
}

static void read_own_table(void) {
    Sint64 size = SDL_GetIOSize(Dat);
    uint8_t foot[28], *table, *lang;
    uint32_t words_sector = 0, words_size = 0;
    int32_t footer;
    int64_t *was;
    int32_t count, langs = 0, version, i, L;
    Sint64 bytes;

    if (size < 28 || SDL_SeekIO(Dat, size - 28, SDL_IO_SEEK_SET) < 0 || SDL_ReadIO(Dat, foot, 28) != 28 ||
        memcmp(foot + 20, PAL_TABLE_MAGIC, 8) != 0) {
        return;
    }
    /* version 1: the files' table, its count; 2: the languages' table and its count before them; 3: where the words are, before that */
    version = (int32_t)le32(foot + 16);
    count = (int32_t)le32(foot + 12);
    footer = version == 1 ? 16 : version == 2 ? 20 : 28;
    if (version >= 2 && version <= 3) {
        langs = (int32_t)le32(foot + 8);
    } else if (version != 1) {
        return;
    }
    if (version == 3) {
        words_sector = le32(foot);
        words_size = le32(foot + 4);
    }
    bytes = (Sint64)count * 8 + (Sint64)langs * 20;
    if (count <= 0 || count > DatIndexCount || langs < 0 || langs > count || bytes + 20 > size) {
        return;
    }
    table = malloc((size_t)bytes);
    was = malloc(sizeof(int64_t) * (size_t)FileGroupCount);
    if (table == NULL || was == NULL || SDL_SeekIO(Dat, size - footer - bytes, SDL_IO_SEEK_SET) < 0 ||
        SDL_ReadIO(Dat, table, (size_t)bytes) != (size_t)bytes) {
        free(table);
        free(was);
        return;
    }
    lang = table + (size_t)count * 8;
    if (langs > 0 && (LangSector = calloc((size_t)DatIndexCount, sizeof(*LangSector))) != NULL &&
        (LangKind = calloc((size_t)DatIndexCount, 1)) != NULL) {
        for (i = 0; i < langs; i++) {
            int32_t entry = (int32_t)(le32(lang + i * 20) & 0xFFFFFF);

            if (entry >= 0 && entry < count) {
                LangKind[entry] = (uint8_t)(le32(lang + i * 20) >> 24);
            }

            for (L = 0; entry >= 0 && entry < count && L < PAL_LANGUAGES - 1; L++) {
                LangSector[entry][L] = (int32_t)le32(lang + i * 20 + 4 + L * 4);
            }
        }
        Languages = 1;
    }
    for (i = 0; i < FileGroupCount; i++) {
        const ArchiveGroup *g = &FileGroups[i];

        was[i] = g->count > 0 && g->first + g->count <= count
                     ? (int64_t)(DatIndex[g->first + g->count - 1].sector + SECTORS(DatIndex[g->first + g->count - 1].size) - DatIndex[g->first].sector)
                     : -1;
    }
    for (i = 0; i < count; i++) {
        const uint8_t *p = table + i * 8;

        DatIndex[i].sector = (int32_t)(p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24);
        DatIndex[i].size = (int32_t)(p[4] | p[5] << 8 | p[6] << 16 | (uint32_t)p[7] << 24);
    }
    for (i = 0; i < FileGroupCount; i++) {
        const ArchiveGroup *g = &FileGroups[i];

        if (was[i] >= 0) {
            int64_t now = DatIndex[g->first + g->count - 1].sector + SECTORS(DatIndex[g->first + g->count - 1].size) - DatIndex[g->first].sector;

            FileGroups[i].size = (int32_t)(FileGroups[i].size + (now - was[i]) * SECTOR_BYTES);
        }
    }
    free(table);
    free(was);
    read_words(words_sector, words_size);
    put_right(count);
    Converted = 1;
    printf("data: made from the European disc, its own table of %d files%s\n", (int)count, Languages ? ", and its five languages" : "");
}

/* Strings of the game's that want the second of two words with the same English (the start line's PRESS ü BUTTON TO START: a battle's,
   then a tournament's, pal_table.mjs) */
static const char *Seconds[8];

void pal_text_second(const char *english) {
    int i;

    for (i = 0; i < 8 && Seconds[i] != NULL && Seconds[i] != english; i++) {
    }
    if (i < 8) {
        Seconds[i] = english;
    }
}

const char *pal_text(const char *english) {
    char key[64];
    int L = settings()->language, i, skip = 0;

    if (NumWords == 0 || english == NULL || L <= 0 || L >= PAL_LANGUAGES || !Languages) {
        return english;
    }
    for (i = 0; i < 8 && Seconds[i] != NULL; i++) {
        skip |= Seconds[i] == english;
    }
    for (i = 0; i < NumWords; i++) {
        if (strcmp(WordSet[i][0], english) == 0 && skip-- <= 0) {
            return WordSet[i][L][0] != 0 ? WordSet[i][L] : english;
        }
    }
    /* the European English is spaced and stopped a little otherwise: by its letters */
    letters_of(english, key, sizeof(key));
    for (i = 0; key[0] != 0 && i < NumWords; i++) {
        if (strcmp(WordLetters[i], key) == 0) {
            return WordSet[i][L][0] != 0 ? WordSet[i][L] : english;
        }
    }
    /* and the USA's with PRESS before the European English (PRESS LEFT - RIGHT TO CHANGE ARENA: LEFT - RIGHT TO CHANGE ARENA) */
    for (i = 0; strncmp(key, "PRESS", 5) == 0 && key[5] != 0 && i < NumWords; i++) {
        if (strcmp(WordLetters[i], key + 5) == 0) {
            return WordSet[i][L][0] != 0 ? WordSet[i][L] : english;
        }
    }
    /* and with BATTLE after it: the USA's PRESS ü BUTTON TO START BATTLE is the European's battle start line, the first of its two */
    if (strlen(key) > 6 && strcmp(key + strlen(key) - 6, "BATTLE") == 0) {
        key[strlen(key) - 6] = 0;
        for (i = 0; i < NumWords; i++) {
            if (strcmp(WordLetters[i], key) == 0) {
                return WordSet[i][L][0] != 0 ? WordSet[i][L] : english;
            }
        }
    }
    return english;
}

static void read_dat(int32_t sector, int32_t count, uint8_t *buf);

uint8_t *archive_language_copy(int entry, int *size) {
    int L = language();
    int32_t n;
    uint8_t *buf;

    if (L == 0 || LangSector == NULL || LangKind == NULL || entry < 0 || entry >= DatIndexCount || LangKind[entry] == 0 ||
        LangSector[entry][L - 1] == 0 || (Replaced != NULL && Replaced[entry] != NULL)) {
        return NULL;
    }
    n = Original != NULL ? Original[entry].size : DatIndex[entry].size;
    if (n <= 0 || (buf = malloc((size_t)SECTORS(n) * SECTOR_BYTES)) == NULL) {
        return NULL;
    }
    read_dat(LangSector[entry][L - 1], SECTORS(n), buf);
    *size = n;
    return buf;
}

int archive_languages(void) {
    archive_open();
    return Languages;
}

static int language(void);

int archive_language(void) {
    return language();
}

/* The language this machine reads the files of: 0 English, 1 French, 2 German, 3 Italian, 4 Spanish (settings.h) */
static int language(void) {
    int l = settings()->language;

    return Languages && l > 0 && l < PAL_LANGUAGES ? l : 0;
}

int archive_from_europe(void) {
    archive_open();
    return Converted;
}

void archive_open(void) {
    if (Dat != NULL) {
        return;
    }
    if (platform_data_path() != NULL) {
        Dat = SDL_IOFromFile(platform_data_path(), "rb");
    }
    if (Dat == NULL) {
        platform_fatal("Can't find the game's data.\n\nCopy CRASHBSH.DAT from your own copy of Crash Bash (USA or Europe) into the same folder as this program. It's in the CRASHBSH folder on the disc.");
    }
    read_own_table();
}

static void read_dat(int32_t sector, int32_t count, uint8_t *buf) {
    size_t want = (size_t)count * SECTOR_BYTES;
    size_t got;

    archive_open();
    SDL_SeekIO(Dat, (Sint64)sector * SECTOR_BYTES, SDL_IO_SEEK_SET);
    got = SDL_ReadIO(Dat, buf, want);
    if (got < want) {
        memset(buf + got, 0, want - got);
    }
}

/* The entry whose place a sector is in now: the last that starts at or before it, or -1 before the first. */
static int32_t entry_at(int32_t sector) {
    int32_t lo = 0, hi = DatIndexCount - 1, found = -1;

    while (lo <= hi) {
        int32_t mid = (lo + hi) / 2;

        if (DatIndex[mid].sector <= sector) {
            found = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return found;
}

static int LogReads;

void archive_log_reads(void) {
    LogReads = 1;
}

/* The files a read is of, named as --dump-files names them: a group's, or one's, or part of one's. */
static void log_read(int32_t sector, int32_t count) {
    int32_t i = entry_at(sector), last = entry_at(sector + count - 1);

    printf("files read:");
    for (i = i < 0 ? 0 : i; i <= last; i++) {
        if (DatIndex[i].size > 0) {
            printf(" %03X", (unsigned)i);
        }
    }
    printf(" at vblank %d\n", platform_vblanks());
}

/* What's past the end of the file reads as zeros. */
void archive_read(int sector, int count, void *buf) {
    uint8_t *out = buf;

    if (LogReads) {
        log_read(sector, count);
    }
    /* every file as the game has it, in English: a language's copies are only read for what's drawn and heard (archive_language_copy),
       so the game is the same in any. (The mode text was read in the language once: its layout differs by language, and a game in
       French went otherwise, 2026-11-26.) */
    if (NumReplaced == 0 && NumPutRight == 0) {
        read_dat(sector, count, out);
        return;
    }
    while (count > 0) {
        int32_t i = entry_at(sector);
        int32_t start = i < 0 ? 0 : DatIndex[i].sector;
        int32_t next = i + 1 < DatIndexCount ? DatIndex[i + 1].sector : INT32_MAX;
        int32_t run = next - sector < count ? next - sector : count;
        int32_t off = sector - start;

        if (i >= 0 && Replaced != NULL && Replaced[i] != NULL) {
            int64_t from = (int64_t)off * SECTOR_BYTES, have = ReplacedSize[i] - from;
            size_t want = (size_t)run * SECTOR_BYTES, copy = have <= 0 ? 0 : (size_t)have < want ? (size_t)have : want;

            if (copy != 0) {
                memcpy(out, Replaced[i] + from, copy);
            }
            memset(out + copy, 0, want - copy);
        } else if (i >= 0 && PutRightStored != NULL && PutRightStored[i] != NULL) {
            /* one the archive has otherwise: as the game reads it, as long */
            int64_t from = (int64_t)off * SECTOR_BYTES, have = (Original != NULL ? Original[i].size : DatIndex[i].size) - from;
            size_t want = (size_t)run * SECTOR_BYTES, copy = have <= 0 ? 0 : (size_t)have < want ? (size_t)have : want;

            if (copy != 0) {
                memcpy(out, put_right_now(i) + from, copy);
            }
            memset(out + copy, 0, want - copy);
        } else {
            /* the game's own file, or the archive before its first: where it was */
            read_dat(i < 0 ? sector : (Original != NULL ? Original[i].sector : DatIndex[i].sector) + off, run, out);
        }
        out += (size_t)run * SECTOR_BYTES;
        sector += run;
        count -= run;
    }
}

static int keep_originals(void) {
    int32_t i;

    if (Original != NULL) {
        return 1;
    }
    Original = malloc(sizeof(ArchiveEntry) * (size_t)DatIndexCount);
    OriginalGroupSize = malloc(sizeof(int32_t) * (size_t)FileGroupCount);
    Replaced = calloc((size_t)DatIndexCount, sizeof(uint8_t *));
    ReplacedSize = calloc((size_t)DatIndexCount, sizeof(int32_t));
    if (Original == NULL || OriginalGroupSize == NULL || Replaced == NULL || ReplacedSize == NULL) {
        return 0;
    }
    memcpy(Original, DatIndex, sizeof(ArchiveEntry) * (size_t)DatIndexCount);
    for (i = 0; i < FileGroupCount; i++) {
        OriginalGroupSize[i] = FileGroups[i].size;
    }
    return 1;
}

int archive_entries(void) {
    return DatIndexCount;
}

int archive_entry_size(int entry) {
    if (entry < 0 || entry >= DatIndexCount) {
        return -1;
    }
    return Original != NULL ? Original[entry].size : DatIndex[entry].size;
}

void archive_replace(int entry, uint8_t *bytes, int size) {
    if (entry < 0 || entry >= DatIndexCount || !keep_originals()) {
        free(bytes);
        return;
    }
    NumReplaced -= Replaced[entry] != NULL;
    free(Replaced[entry]);
    Replaced[entry] = bytes;
    ReplacedSize[entry] = bytes != NULL ? size : 0;
    NumReplaced += bytes != NULL;
}

void archive_replace_none(void) {
    int32_t i;

    for (i = 0; Replaced != NULL && i < DatIndexCount; i++) {
        archive_replace(i, NULL, 0);
    }
}

/* Where every file is now. A replacement that needs more sectors than were there before the next file moves every later one on by
 * that much; one that needs fewer leaves its old place's end empty. A group's size changes by as much as the span of its files does. */
void archive_lay_out(void) {
    int32_t i, shift = 0;

    if (Original == NULL) {
        return;
    }
    for (i = 0; i < DatIndexCount; i++) {
        DatIndex[i].sector = Original[i].sector + shift;
        DatIndex[i].size = Replaced[i] != NULL ? ReplacedSize[i] : Original[i].size;
        if (Replaced[i] != NULL) {
            int32_t room = i + 1 < DatIndexCount ? Original[i + 1].sector - Original[i].sector : SECTORS(Original[i].size);
            int32_t need = SECTORS(ReplacedSize[i]);

            if (need > room) {
                shift += need - room;
            }
        }
    }
    for (i = 0; i < FileGroupCount; i++) {
        const ArchiveGroup *g = &FileGroups[i];
        int32_t first = g->first, last = g->first + g->count - 1;

        FileGroups[i].size = OriginalGroupSize[i];
        if (g->count > 0 && first >= 0 && last < DatIndexCount) {
            int64_t was = (int64_t)(Original[last].sector + SECTORS(Original[last].size) - Original[first].sector) * SECTOR_BYTES;
            int64_t now = (int64_t)(DatIndex[last].sector + SECTORS(DatIndex[last].size) - DatIndex[first].sector) * SECTOR_BYTES;

            FileGroups[i].size = (int32_t)(OriginalGroupSize[i] + now - was);
        }
    }
}

/* The game's own bytes of an entry, as the archive has them, to free: for --dump-files. */
uint8_t *archive_own_file(int entry, int *size) {
    const ArchiveEntry *e;
    uint8_t *buf;

    if (entry < 0 || entry >= DatIndexCount) {
        return NULL;
    }
    e = Original != NULL ? &Original[entry] : &DatIndex[entry];
    buf = malloc((size_t)SECTORS(e->size) * SECTOR_BYTES + 1);
    if (buf != NULL) {
        read_dat(e->sector, SECTORS(e->size), buf);
        if (PutRightStored != NULL && PutRightStored[entry] != NULL) {
            memcpy(buf, put_right_now(entry), (size_t)e->size);
        }
        *size = e->size;
    }
    return buf;
}

void archive_entry_now(int entry, int *sector, int *size) {
    archive_open();
    *sector = entry >= 0 && entry < DatIndexCount ? DatIndex[entry].sector : 0;
    *size = entry >= 0 && entry < DatIndexCount ? DatIndex[entry].size : 0;
}
