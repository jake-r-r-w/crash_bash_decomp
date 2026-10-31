/* Translates every mode text file named on the command line. The retail files hold 161 lists and 2,058 records
 * (docs/ASSETS.md). Then each game mode's pause table is walked as the game walks it, down to its records and their text. */
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"

extern u8 ModeTextBuffer[];
extern s32 ModeTextLists, ModeTextRecords;
s32 ModeTextTranslate(s32 len);
extern PlaceRec ***PauseMenuTexts[45];
extern u8 ModeTextData[];

unsigned char HeapArena[8];

static int in_buffer(const void *p) {
    return (const u8 *)p >= ModeTextBuffer && (const u8 *)p < ModeTextBuffer + 0x4660;
}

static int in_data(const void *p) {
    return (const u8 *)p >= ModeTextData && (const u8 *)p < ModeTextData + 0x10000 - 0x100;
}

/* a list's records: sensible types, up to one below 2, and any string they name is text. Returns how many, or -1. */
static int walk_list(PlaceRec *r) {
    int n = 0;

    for (; n < 200; n++, r++) {
        if (!in_data(r)) {
            return -1;
        }
        if (r->type < 2) {
            return n;
        }
        if (r->type > 0x20) {
            return -1;
        }
        if (r->type == 3 && in_buffer(r->data)) {
            const u8 *s = r->data;
            if (*s < 0x20) {
                return -1;
            }
        }
    }
    return -1;
}

/* is this table, in this file, a pause table? It leads to lists through zero-separated sets, with no pointers wild. */
static int walk_table(PlaceRec ***t, int *recs) {
    int sets = 0;
    int k;

    if (!in_data(t)) {
        return 0;
    }
    for (k = 0; k < 9; k++) {
        PlaceRec **set = t[k];
        int n;
        if (set == NULL) {
            continue;
        }
        if (!in_data(set)) {
            return -1;
        }
        for (n = 0; n < 32 && set[n] != NULL; n++) {
            int m;
            if (!in_data(set[n]) || !in_data(&set[n + 1])) {
                return -1;
            }
            m = walk_list(set[n]);
            if (m < 0) {
                return -1;
            }
            *recs += m;
        }
        sets++;
    }
    return sets;
}

int main(int argc, char **argv) {
    static int mode_ok[45];
    s32 lists = 0, records = 0;
    int reached = 0, recs = 0, modes = 0;
    int i, j;

    for (i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        size_t n;
        if (f == NULL) {
            printf("can't open %s\n", argv[i]);
            return 1;
        }
        n = fread(ModeTextBuffer, 1, 0x4660, f);
        fclose(f);
        if (!ModeTextTranslate((s32)n)) {
            printf("%s doesn't fit\n", argv[i]);
            return 1;
        }
        lists += ModeTextLists;
        records += ModeTextRecords;
        for (j = 0; j < 33; j++) {
            int r = 0;
            int sets = walk_table(PauseMenuTexts[j], &r);
            if (sets > 0 && r > 0 && !mode_ok[j]) {
                mode_ok[j] = 1;
                reached += sets;
                recs += r;
            }
        }
    }
    for (j = 0; j < 33; j++) {
        modes += mode_ok[j];
        if (!mode_ok[j]) {
            printf("mode %d has no pause table in any file\n", j);
        }
    }
    printf("%d lists, %d records; %d of 33 modes' pause tables walk, %d sets, %d records\n", lists, records, modes, reached, recs);
    if (lists != 161 || records != 2058 || modes != 33) {
        return 1;
    }
    return 0;
}
