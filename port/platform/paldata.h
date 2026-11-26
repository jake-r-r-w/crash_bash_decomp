/* The European disc (SCES-02834): its CRASHBSH.DAT made into one the USA game reads (paldata.c; docs/PORT.md, The European disc). */
#ifndef PALDATA_H
#define PALDATA_H

#include <stddef.h>
#include <stdint.h>

#define PAL_FILES 992              /* the USA archive's files */
#define PAL_FONT_GLYPHS 109        /* the USA font's */
#define PAL_INTERFACE_PICTURES 46  /* the USA interface's */

/* what's done with a file: copied as it is; the font's or the interface's pictures put in the USA's order; or none (the USA has a file
   the European disc hasn't, that the game never reads) */
enum { PAL_COPY, PAL_FONT, PAL_INTERFACE, PAL_NONE };

typedef struct PalFile {
    uint32_t sector, size; /* in the European CRASHBSH.DAT */
    uint8_t kind;
} PalFile;

/* paltable.c, made by port/pal_table.mjs */
extern const unsigned PalDatSize, PalDatCrc;
extern const PalFile PalFiles[PAL_FILES];
extern const short PalFontOrder[PAL_FONT_GLYPHS];
extern const short PalInterfaceOrder[PAL_INTERFACE_PICTURES];
extern const unsigned short PalTagSize[2];

/* Reads size bytes at byte at of the European CRASHBSH.DAT: 0 if it can't */
typedef int (*PalRead)(void *ctx, uint64_t at, void *buf, size_t size);

/* Whether a CRASHBSH.DAT this size is the European disc's */
int pal_is_size(int64_t size);
/* The European CRASHBSH.DAT read by read, made into out: the USA's files in the USA's order, then their table (archive.c reads it).
   NULL, or what's wrong: another European disc's, or it can't be read or written. progress, if not NULL, from 0 to 1 */
const char *pal_convert(PalRead read, void *ctx, const char *out, void (*progress)(double done));
/* The same, from a CRASHBSH.DAT file */
const char *pal_convert_file(const char *dat, const char *out, void (*progress)(double done));

/* Whether a file is one made by pal_convert, or a European CRASHBSH.DAT as it is on the disc */
int pal_is_converted(const char *path);
int pal_is_file(const char *path);

/* A converted file ends with its table: the last bytes, PAL_TABLE_MAGIC, after the table's size */
#define PAL_TABLE_MAGIC "CBUSAIDX"

#endif
