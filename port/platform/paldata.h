/* The European disc (SCES-02834): its CRASHBSH.DAT made into one the USA game reads (paldata.c; docs/PORT.md, The European disc). */
#ifndef PALDATA_H
#define PALDATA_H

#include <stddef.h>
#include <stdint.h>

#define PAL_FILES 992              /* the USA archive's files */
#define PAL_FONT_GLYPHS 109        /* the USA font's */
#define PAL_INTERFACE_PICTURES 46  /* the USA interface's */
#define PAL_PICTURES_COUNT 126     /* the USA main menu's (082) */
#define PAL_MODES 45               /* the game's modes, each with its pause and rules menus' root in its mode text */

/* what's done with a file: copied as it is; the font's or the interface's pictures put in the USA's order; or none (the USA has a file
   the European disc hasn't, that the game never reads); or copied as it is, and its pictures put in the USA's order as it's read (the
   main menu's, PalPicturesOrder: archive.c, so data made before that is put right too) */
enum { PAL_COPY, PAL_FONT, PAL_INTERFACE, PAL_NONE, PAL_PICTURES };

typedef struct PalFile {
    uint32_t sector, size; /* in the European CRASHBSH.DAT */
    uint8_t kind;
} PalFile;

/* The languages, as the game's code numbers them: a setting of each machine's own (settings.h), never the game's */
#define PAL_LANGUAGES 5 /* English, French, German, Italian, Spanish */

/* A USA file with a version for each language: the French's, German's, Italian's and Spanish's place and size in the European
   CRASHBSH.DAT (the English is PalFiles') */
/* A language file's kind: text the game reads in its language (the archive's reads give it), or a sound pack, which the game reads in
   English, so every machine's game has the same voices and their timing, and the mixer plays its language's alongside (audio.c) */
/* PAL_WORDS: the interface (the pause frame's PAUSED and RULES pictures), drawn in the language (archive.c, pal_put_right) */
enum { PAL_TEXT = 1, PAL_SOUND = 2, PAL_WORDS = 3 };

typedef struct PalLanguageFile {
    uint16_t entry;
    uint8_t kind;
    struct {
        uint32_t sector, size;
    } v[PAL_LANGUAGES - 1];
} PalLanguageFile;

/* A letter of the European font's that the USA's hasn't: page (0 the big font, 1 the small), Latin-1 code, glyph in the made font */
typedef struct PalGlyph {
    uint8_t page, code;
    uint8_t flags; /* the European InitSysFont's: 4, its accent above (nascalls.c's DrawSysText draws it higher) */
    uint16_t glyph;
} PalGlyph;

/* One of the game's words in the five languages, where the European code has it: the engine's or overlay's sector and size in the
   European CRASHBSH.DAT (-1: the disc's program file, which a disc image has and a CRASHBSH.DAT alone hasn't), and each language's
   offset in it, English first */
typedef struct PalString {
    int32_t sector;
    uint32_t size;
    uint32_t offs[PAL_LANGUAGES];
} PalString;

/* paltable.c, made by port/pal_table.mjs */
extern const PalString PalStrings[];
extern const int PalStringCount;
extern const PalLanguageFile PalLanguageFiles[];
extern const int PalLanguageFileCount;
extern const PalGlyph PalGlyphs[];
extern const int PalGlyphCount;
extern const unsigned PalDatSize, PalDatCrc;
extern const PalFile PalFiles[PAL_FILES];
extern const short PalFontOrder[PAL_FONT_GLYPHS];
extern const short PalInterfaceOrder[PAL_INTERFACE_PICTURES];
extern const unsigned short PalTagSize[2];
extern const short PalPicturesOrder[PAL_PICTURES_COUNT];
extern const signed char PalPicturesFrames[PAL_PICTURES_COUNT];
extern const short PalPicturesLanguage[PAL_PICTURES_COUNT];
/* The European mode text's load address (its pointers' base), and each language's root for each mode in its files (0: none) */
extern const unsigned PalModeTextBase;
extern const unsigned short PalModeTextRoots[PAL_LANGUAGES][PAL_MODES];
/* the model banks its records name that the USA numbers otherwise: European, USA */
extern const short PalModeTextModels[][2];
extern const int PalModeTextModelCount;

/* The mode text as the European data has it, for modetext.c: 1 and its base and a language's roots (0 English, the game's own), or 0 for
   the USA's */
int pal_mode_text(int language, unsigned *base, const unsigned short **roots);
/* A European mode text record's model bank as the USA numbers it, and a European font glyph as the made font has it (paldata.c) */
int pal_mode_text_model(int model);
int pal_mode_text_glyph(int glyph);

/* Reads size bytes at byte at of the European CRASHBSH.DAT: 0 if it can't */
typedef int (*PalRead)(void *ctx, uint64_t at, void *buf, size_t size);

/* Whether a CRASHBSH.DAT this size is the European disc's */
int pal_is_size(int64_t size);
/* The European CRASHBSH.DAT read by read, made into out: the USA's files in the USA's order, then their table (archive.c reads it).
   NULL, or what's wrong: another European disc's, or it can't be read or written. progress, if not NULL, from 0 to 1 */
const char *pal_convert(PalRead read, void *ctx, const uint8_t *exe, size_t exe_size, const char *out, void (*progress)(double done));
/* The same, from a CRASHBSH.DAT file (no program file: the words that are only in it stay English) */
const char *pal_convert_file(const char *dat, const char *out, void (*progress)(double done));

/* A file of a converted archive as the game reads it in a language (the machine's: only drawn), if it isn't as the archive has it
   (PAL_PICTURES): a copy as long, to free, or NULL */
uint8_t *pal_put_right(int entry, const uint8_t *bytes, uint32_t size, int language);
/* The main menu's pictures (082), which the European data has in its own order: a USA number of the game's code as the European's, and
   back (-1: none); themselves with the USA's data */
int pal_mainmenu_picture(int usa);
int pal_mainmenu_usa(int picture);

/* Whether a file is one made by pal_convert, or a European CRASHBSH.DAT as it is on the disc */
int pal_is_converted(const char *path);
int pal_is_file(const char *path);

/* The font's letters past the USA's, put in its tables (SysFontLookUp and SysFontFlags: 2 pages of 256) when the data is the European
   disc's */
void pal_font_glyphs(int16_t *lookup, uint8_t *flags);

/* A converted file ends with its table: the last bytes, PAL_TABLE_MAGIC, after the table's size; version 2 has the languages' too, and
   3 where the words are (archive.c) */
#define PAL_TABLE_VERSION 3 /* 3: the words too */
#define PAL_TABLE_MAGIC "CBUSAIDX"

#endif
