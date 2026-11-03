/* Reading the files in a zip (zipread.c), for mods: a zip's files are read as they're needed, stored or deflated, and checked as they
 * go, as a zip can come from anyone. */
#ifndef ZIPREAD_H
#define ZIPREAD_H

#include <stddef.h>
#include <stdint.h>

typedef struct ZipEntry {
    char *name;        /* its path in the zip, with / between folders */
    uint32_t method;   /* 0 stored, 8 deflated: the two it reads */
    uint32_t crc;
    uint32_t packed;   /* bytes in the zip */
    uint32_t size;     /* bytes unpacked */
    uint32_t local;    /* where its local header is */
    int locked;        /* encrypted, which it can't read */
} ZipEntry;

typedef struct ZipFile {
    char *path;
    ZipEntry *entries; /* folders left out */
    int count;
} ZipFile;

/* The zip's list of files, or NULL with *problem saying why. */
ZipFile *zip_open(const char *path, const char **problem);
void zip_close(ZipFile *zip);
/* An entry's bytes, to free, or NULL with *problem saying why: none past limit. */
uint8_t *zip_read(const ZipFile *zip, int entry, size_t limit, size_t *size, const char **problem);

#endif
