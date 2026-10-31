/* libcd on PC (ROADMAP-B 6.2): sectors come from a CRASHBSH.DAT file, a flat run of 2 KB sectors from
 * sector 0, and every read is finished by the time it returns. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcd.h>
#include "../platform/platform.h"

static FILE *Dat;
static long DatSize;
static int ReadPos;

static void open_dat(void) {
    const char *path;

    if (Dat != NULL) {
        return;
    }
    path = platform_data_path();
    Dat = fopen(path, "rb");
    if (Dat == NULL) {
        fprintf(stderr, "Can't open the game's data, %s (docs/PORT.md, Running it)\n", path);
        exit(1);
    }
    fseek(Dat, 0, SEEK_END);
    DatSize = ftell(Dat);
}

int CdInit(void) {
    open_dat();
    return 1;
}

int CdStatus(void) {
    return 0x02;
}

int CdDiskReady(int mode) {
    return 2;
}

int CdSync(int mode, u_char *result) {
    return 2;
}

int CdControl(u_char com, u_char *param, u_char *result) {
    if (com == CdlSetloc) {
        ReadPos = CdPosToInt((CdlLOC *)param);
    }
    return 1;
}

int CdControlF(u_char com, u_char *param) {
    return CdControl(com, param, NULL);
}

static int bcd(int v) {
    return ((v / 10) << 4) | (v % 10);
}

static int unbcd(int v) {
    return (v >> 4) * 10 + (v & 15);
}

CdlLOC *CdIntToPos(int i, CdlLOC *p) {
    i += 150;
    p->sector = bcd(i % 75);
    i /= 75;
    p->second = bcd(i % 60);
    p->minute = bcd(i / 60);
    p->track = 0;
    return p;
}

int CdPosToInt(CdlLOC *p) {
    return (unbcd(p->minute) * 60 + unbcd(p->second)) * 75 + unbcd(p->sector) - 150;
}

/* The game looks for one file, the archive, which starts at sector 0 here. */
CdlFILE *CdSearchFile(CdlFILE *fp, char *name) {
    open_dat();
    CdIntToPos(0, &fp->pos);
    fp->size = (u_long)DatSize;
    strncpy(fp->name, "CRASHBSH.DAT;1", sizeof(fp->name));
    return fp;
}

int CdRead(int sectors, u_long *buf, int mode) {
    size_t got;

    open_dat();
    fseek(Dat, (long)ReadPos * 2048, SEEK_SET);
    got = fread(buf, 1, (size_t)sectors * 2048, Dat);
    if (got < (size_t)sectors * 2048) {
        memset((char *)buf + got, 0, (size_t)sectors * 2048 - got);
    }
    ReadPos += sectors;
    return 1;
}

int CdReadSync(int mode, u_char *result) {
    return 0;
}
