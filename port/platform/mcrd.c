/* libmcrd on PC (ROADMAP-B 6.2): the memory card in slot 1 as a file holding a raw 128 KB card (the
#include <stdint.h>
typedef unsigned int u32;
 * form tools/scripts/card.py and emulators read), with every command finished at once. */
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libmcrd.h>
#include "../platform/platform.h"

#define CARD_SIZE 0x20000
#define BLOCK 0x2000
#define FRAME 0x80

static unsigned char Card[CARD_SIZE];
static int Loaded;
static int Seen;
static long LastCmd, LastResult;
static int Pending;

static void finish(int cmd, int result) {
    LastCmd = cmd;
    LastResult = result;
    Pending = 1;
}

static void frame_checksum(unsigned char *f) {
    int i;
    unsigned char x = 0;

    for (i = 0; i < FRAME - 1; i++) {
        x ^= f[i];
    }
    f[FRAME - 1] = x;
}

static void format_card(void) {
    int i;

    memset(Card, 0, CARD_SIZE);
    Card[0] = 'M';
    Card[1] = 'C';
    frame_checksum(Card);
    for (i = 1; i < 16; i++) {
        unsigned char *d = Card + i * FRAME;

        d[0] = 0xA0;
        d[8] = 0xFF;
        d[9] = 0xFF;
        frame_checksum(d);
    }
    for (i = 16; i < 36; i++) {
        unsigned char *d = Card + i * FRAME;

        d[0] = 0xFF;
        d[1] = 0xFF;
        d[2] = 0xFF;
        d[3] = 0xFF;
        d[8] = 0xFF;
        d[9] = 0xFF;
        frame_checksum(d);
    }
    Card[0x7F * FRAME] = 'M';
    Card[0x7F * FRAME + 1] = 'C';
    frame_checksum(Card + 0x7F * FRAME);
}

static void save_card(void) {
    FILE *f = fopen(platform_card_path(), "wb");

    if (f != NULL) {
        fwrite(Card, 1, CARD_SIZE, f);
        fclose(f);
    }
}

static void load_card(void) {
    FILE *f;

    if (Loaded) {
        return;
    }
    Loaded = 1;
    f = fopen(platform_card_path(), "rb");
    if (f == NULL || fread(Card, 1, CARD_SIZE, f) != CARD_SIZE || Card[0] != 'M' || Card[1] != 'C') {
        format_card();
    }
    if (f != NULL) {
        fclose(f);
    }
}

/* The directory frame of a file, by name; 0: none */
static int find_file(const char *name) {
    int i;

    for (i = 1; i < 16; i++) {
        unsigned char *d = Card + i * FRAME;

        if (d[0] == 0x51 && strncmp((const char *)d + 10, name, 20) == 0) {
            return i;
        }
    }
    return 0;
}

void MemCardInit(int val) {
}
void MemCardStart(void) {
}
void MemCardStop(void) {
}

int MemCardExist(int chan) {
    load_card();
    if (chan != 0) {
        finish(0, 1);
        return 1;
    }
    finish(0, Seen ? 0 : 3);
    Seen = 1;
    return 1;
}

int MemCardAccept(int chan) {
    load_card();
    finish(1, chan != 0 ? 1 : 0);
    return 1;
}

int MemCardFormat(int chan) {
    load_card();
    format_card();
    save_card();
    return 0;
}

int MemCardSync(int mode, int *cmds, int *rslt) {
    if (!Pending) {
        return -1;
    }
    Pending = 0;
    if (cmds != NULL) {
        *cmds = LastCmd;
    }
    if (rslt != NULL) {
        *rslt = LastResult;
    }
    return 1;
}

int MemCardCreateFile(int chan, char *file, int blocks) {
    int i;

    load_card();
    if (find_file(file) != 0) {
        return 2;
    }
    for (i = 1; i < 16; i++) {
        unsigned char *d = Card + i * FRAME;

        if (d[0] == 0xA0) {
            uint32_t size = BLOCK;

            memset(d, 0, FRAME);
            d[0] = 0x51;
            memcpy(d + 4, &size, 4);
            d[8] = 0xFF;
            d[9] = 0xFF;
            strncpy((char *)d + 10, file, 20);
            frame_checksum(d);
            save_card();
            return 0;
        }
    }
    return 1;
}

int MemCardGetDirentry(int chan, char *name, struct DIRENTRY *dir, int *files, int ofs, int max) {
    int i, n = 0;

    load_card();
    for (i = 1; i < 16 && n < max; i++) {
        unsigned char *d = Card + i * FRAME;

        if (d[0] == 0x51) {
            uint32_t size;

            memset(&dir[n], 0, sizeof(dir[n]));
            memcpy(&size, d + 4, 4);
            strncpy(dir[n].name, (const char *)d + 10, sizeof(dir[n].name));
            dir[n].size = (long)size;
            n++;
        }
    }
    *files = n;
    return 0;
}

int MemCardReadFile(int chan, char *file, u32 *adrs, int ofs, int bytes) {
    int i;

    load_card();
    i = find_file(file);
    if (i == 0) {
        finish(2, 1);
        return 0;
    }
    memcpy(adrs, Card + i * BLOCK + ofs, (size_t)bytes);
    finish(2, 0);
    return 0;
}

int MemCardWriteFile(int chan, char *file, u32 *adrs, int ofs, int bytes) {
    int i;

    load_card();
    i = find_file(file);
    if (i == 0) {
        finish(3, 1);
        return 0;
    }
    memcpy(Card + i * BLOCK + ofs, adrs, (size_t)bytes);
    save_card();
    finish(3, 0);
    return 0;
}
