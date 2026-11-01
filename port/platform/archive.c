/* The game's archive, CRASHBSH.DAT: a flat run of 2 KB sectors from sector 0, and every read is finished by the time it returns. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform.h"

#define SECTOR_BYTES 2048

static FILE *Dat;

void archive_open(void) {
    if (Dat != NULL) {
        return;
    }
    Dat = fopen(platform_data_path(), "rb");
    if (Dat == NULL) {
        platform_fatal("Can't find the game's data.\n\nCopy CRASHBSH.DAT from your own copy of Crash Bash (USA) into the same folder as this program. It's in the CRASHBSH folder on the disc.");
    }
}

/* What's past the end of the file reads as zeros. */
void archive_read(int sector, int count, void *buf) {
    size_t want = (size_t)count * SECTOR_BYTES;
    size_t got;

    archive_open();
    fseek(Dat, (long)sector * SECTOR_BYTES, SEEK_SET);
    got = fread(buf, 1, want, Dat);
    if (got < want) {
        memset((char *)buf + got, 0, want - got);
    }
}
