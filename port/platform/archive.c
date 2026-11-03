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
#include "platform.h"

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

/* What the tables were before anything was replaced, and the replacements by entry */
static ArchiveEntry *Original;
static int32_t *OriginalGroupSize;
static uint8_t **Replaced;
static int32_t *ReplacedSize;
static int NumReplaced;

void archive_open(void) {
    if (Dat != NULL) {
        return;
    }
    if (platform_data_path() != NULL) {
        Dat = SDL_IOFromFile(platform_data_path(), "rb");
    }
    if (Dat == NULL) {
        platform_fatal("Can't find the game's data.\n\nCopy CRASHBSH.DAT from your own copy of Crash Bash (USA) into the same folder as this program. It's in the CRASHBSH folder on the disc.");
    }
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
    if (NumReplaced == 0) {
        read_dat(sector, count, out);
        return;
    }
    while (count > 0) {
        int32_t i = entry_at(sector);
        int32_t start = i < 0 ? 0 : DatIndex[i].sector;
        int32_t next = i + 1 < DatIndexCount ? DatIndex[i + 1].sector : INT32_MAX;
        int32_t run = next - sector < count ? next - sector : count;
        int32_t off = sector - start;

        if (i >= 0 && Replaced[i] != NULL) {
            int64_t from = (int64_t)off * SECTOR_BYTES, have = ReplacedSize[i] - from;
            size_t want = (size_t)run * SECTOR_BYTES, copy = have <= 0 ? 0 : (size_t)have < want ? (size_t)have : want;

            if (copy != 0) {
                memcpy(out, Replaced[i] + from, copy);
            }
            memset(out + copy, 0, want - copy);
        } else {
            /* the game's own file, or the archive before its first: where it was */
            read_dat(i < 0 ? sector : Original[i].sector + off, run, out);
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
        *size = e->size;
    }
    return buf;
}
