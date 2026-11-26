#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"

/* A mode text file is strings, then panel records and pointer tables, with PS1 addresses for pointers (docs/ASSETS.md).
 * The strings stay where they load. The records and tables are rebuilt in ModeTextData with real pointers: each record
 * becomes a PlaceRec, and each table word a pointer, so the tables still index the same way. */
#define MODE_TEXT_BASE 0x800AEC60u
#define MODE_TEXT_MAX 0x4660
#define MODE_TEXT_REC 0x24

u8 ModeTextBuffer[MODE_TEXT_MAX];
_Alignas(16) u8 ModeTextData[0x10000];

/* The most a mode text file can be, as it's read into the buffer in whole sectors: for a mod's file in its place (mode_text_room) */
const s32 ModeTextRoom = MODE_TEXT_MAX / 2048 * 2048;

static void *ModeTextEmpty[12];
PlaceRec ***PauseMenuTexts[45];
s32 ModeTextLists;
s32 ModeTextRecords;

/* Each game mode's pause table, as an offset in its file. 0 for a mode with none. */
static const u16 PauseMenuOffsets[45] = {
    0x3B44, 0x3C1C, 0x3CF4, 0x3DCC, 0x7B4, 0x2C58, 0x2A48, 0x2BA8, 0x2AF8, 0x2CA0, 0x3808, 0x38B8, 0x397C, 0x3A40,
    0x2178, 0x2228, 0x22D8, 0x2374, 0x26B0, 0x2600, 0x2760, 0x2820, 0x2868, 0x1FB4, 0x2078, 0x2200, 0x213C, 0x1850,
    0x1594, 0x1828, 0x17DC, 0x544, 0x3E24, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0,
};

/* PC: the European disc's mode text loads 0x50 later and has its own layout, so its load address and its English roots, for each mode,
   come from its engine (platform/paldata.c; docs/PORT.md, Languages). The game reads the English on every machine. */
int pal_mode_text(int language, unsigned *base, const unsigned short **roots);
int pal_mode_text_model(int model);
int pal_mode_text_glyph(int glyph);
int archive_language(void);
unsigned char *archive_language_copy(int entry, int *size);
extern file *ModeTextFilesLang[14];
#define PC_DAT_FILES 992 /* DatIndex's entries (main.c) */

static s32 is_text_byte(u8 c) {
    return (c >= 0x20 && c < 0x7F) || c == 0x0A || c == 0x0D || c >= 0xF0;
}

/* PC: a language's text, with its accented letters (Latin-1) */
static s32 is_text_byte_accented(u8 c) {
    return (c >= 0x20 && c != 0x7F) || c == 0x0A || c == 0x0D;
}

static u32 word_at(const u8 *d, s32 off) {
    return d[off] | (d[off + 1] << 8) | (d[off + 2] << 16) | ((u32)d[off + 3] << 24);
}

static s16 half_at(const u8 *d, s32 off) {
    return (s16)(d[off] | (d[off + 1] << 8));
}

/* where the strings end */
static s32 strings_end_by(const u8 *d, s32 len, s32 (*text)(u8)) {
    s32 p = 4;
    s32 end = 4;

    while (p < len) {
        s32 e = p;
        s32 next;
        while (e < len && d[e] != 0 && text(d[e])) {
            e++;
        }
        if (e == p || e >= len || d[e] != 0) {
            break;
        }
        end = (e + 4) & ~3;
        next = end;
        if (end > len) {
            end = len;
        }
        while (next + 4 <= len && word_at(d, next) == 0) {
            next += 4;
        }
        p = next;
    }
    return end;
}

static s32 strings_end(const u8 *d, s32 len) {
    return strings_end_by(d, len, is_text_byte);
}

/* PC: the record's format. The USA's is 9 words: type, x|y, z|scale, rx|ry, rz|pad, data, frame, model|player, flags. The European disc's
   is 7: type|flags|x, y|z, scale|rx, ry|rz, data, frame, model|player, its flags a byte (0x80 the USA's 0x800), its model banks numbered
   otherwise and its font's glyphs in its own order (paldata.c; pal_table.mjs checked every field against the USA's English) */
static s32 Europe;
#define REC_LEN (Europe ? 0x1C : MODE_TEXT_REC)
#define REC_DATA (Europe ? 0x10 : 0x14)
/* (a European record's type is its first byte: a pointer, 0x800Axxxx and so on, isn't one, though its first byte can look it, as no x is
   0x8001 to 0x80FF) */
#define REC_TYPE(w) (Europe ? ((w) >> 24 == 0x80 && ((w) >> 16 & 0xFF) != 0 ? 0x7FFF : (s32)((w) & 0xFF)) : (s32)(w))

/* Which words are the start of something pointed at (a target), and which the records of a panel list (types 2 to 0x20, ending at one
   below 2, each MODE_TEXT_REC bytes) are: the records' words, or 1 for each */
static void find_records(const u8 *d, s32 len, u32 base, s32 send, u8 *is_target, u8 *is_record, s32 count) {
    s32 p;

    for (p = 0; p < MODE_TEXT_MAX / 4; p++) {
        is_target[p] = 0;
        is_record[p] = 0;
    }
    for (p = send; p + 4 <= len; p += 4) {
        u32 x = word_at(d, p);
        if (x >= base + send && x < base + len && !(x & 3)) {
            is_target[(x - base) / 4] = 1;
        }
    }
    for (p = send; p + 4 <= len; p += 4) {
        s32 k = p;
        s32 n = 0;
        if (!is_target[p / 4]) {
            continue;
        }
        while (k + REC_LEN <= len) {
            s32 type = REC_TYPE(word_at(d, k));
            if (type < 2) {
                break;
            }
            if (type > 0x20) {
                n = -1;
                break;
            }
            n++;
            k += REC_LEN;
        }
        if (n > 0 && k + REC_LEN <= len) {
            s32 j;
            if (count) {
                ModeTextLists++;
            }
            for (j = 0; j <= n; j++) {
                s32 r = p + j * REC_LEN;
                if (count && j < n && !is_record[r / 4]) {
                    ModeTextRecords++;
                }
                is_record[r / 4] = 1;
            }
        }
    }
}

static u8 is_target[MODE_TEXT_MAX / 4];
static u8 is_record[MODE_TEXT_MAX / 4];
static u32 ModeTextBase = MODE_TEXT_BASE;
static s32 ModeTextSend, ModeTextLen;

/* Translates the file just loaded into ModeTextBuffer. Returns 0 if it doesn't fit. */
s32 ModeTextTranslate(s32 len) {
    static u32 moved[MODE_TEXT_MAX / 4];
    const u8 *d = ModeTextBuffer;
    const unsigned short *roots = NULL;
    unsigned pal_base;
    u32 base = MODE_TEXT_BASE;
    u32 out = 0;
    s32 send;
    s32 p;
    s32 i;

    /* PC: the European disc's, at its own address, with its own roots (the English's, whatever this machine draws) */
    Europe = 0;
    if (pal_mode_text(0, &pal_base, &roots)) {
        base = pal_base;
        Europe = 1;
    }
    ModeTextBase = base;
    ModeTextLists = 0;
    ModeTextRecords = 0;
    for (i = 0; i < MODE_TEXT_MAX / 4; i++) {
        moved[i] = 0xFFFFFFFF;
    }
    if (len > MODE_TEXT_MAX) {
        return 0;
    }
    send = strings_end(d, len);
    ModeTextSend = send;
    ModeTextLen = len;
    find_records(d, len, base, send, is_target, is_record, 1);
    /* lay it out: where each word goes */
    for (p = send; p + 4 <= len;) {
        moved[p / 4] = out;
        if (is_record[p / 4]) {
            out += sizeof(PlaceRec);
            p += REC_LEN;
        } else {
            out += sizeof(void *);
            p += 4;
        }
    }
    if (out > sizeof(ModeTextData)) {
        return 0;
    }

#define MODE_TEXT_PTR(x)                                                                                             \
    ((x) >= base && (x) < base + len                                                                                 \
         ? ((x) - base < (u32)send || moved[((x) - base) / 4] == 0xFFFFFFFF                                          \
                ? (void *)(ModeTextBuffer + ((x) - base))                                                            \
                : (void *)(ModeTextData + moved[((x) - base) / 4]))                                                  \
         : (void *)(iptr)(x))

    for (p = send; p + 4 <= len;) {
        u8 *to = ModeTextData + moved[p / 4];
        if (is_record[p / 4] && Europe) {
            PlaceRec *r = (PlaceRec *)to;
            u32 w0 = word_at(d, p);
            u32 flags = (w0 >> 8) & 0xFF;
            u32 x = word_at(d, p + 0x10);
            s32 player = half_at(d, p + 0x1A);

            r->type = (s32)(w0 & 0xFF);
            r->x = (s16)(w0 >> 16);
            r->y = half_at(d, p + 4);
            r->z = half_at(d, p + 6);
            r->scale = half_at(d, p + 8);
            r->rx = half_at(d, p + 0xA);
            r->ry = half_at(d, p + 0xC);
            r->rz = half_at(d, p + 0xE);
            r->pad12 = 0;
            r->data = MODE_TEXT_PTR(x);
            r->frame = (s32)word_at(d, p + 0x14);
            r->model = (s16)pal_mode_text_model(half_at(d, p + 0x18));
            r->player = (s16)(r->model == DB_Font && (player & 0xF000) == 0x3000 ? 0x3000 | pal_mode_text_glyph(player & 0xFFF) : player);
            r->flags = (s32)((flags & 0x7F) | ((flags & 0x80) << 4));
            p += REC_LEN;
        } else if (is_record[p / 4]) {
            PlaceRec *r = (PlaceRec *)to;
            u32 x = word_at(d, p + 0x14);
            r->type = (s32)word_at(d, p);
            r->x = half_at(d, p + 4);
            r->y = half_at(d, p + 6);
            r->z = half_at(d, p + 8);
            r->scale = half_at(d, p + 0xA);
            r->rx = half_at(d, p + 0xC);
            r->ry = half_at(d, p + 0xE);
            r->rz = half_at(d, p + 0x10);
            r->pad12 = half_at(d, p + 0x12);
            r->data = MODE_TEXT_PTR(x);
            r->frame = (s32)word_at(d, p + 0x18);
            r->model = half_at(d, p + 0x1C);
            r->player = half_at(d, p + 0x1E);
            r->flags = (s32)word_at(d, p + 0x20);
            p += MODE_TEXT_REC;
        } else {
            u32 x = word_at(d, p);
            *(void **)to = MODE_TEXT_PTR(x);
            p += 4;
        }
    }

    for (i = 0; i < 45; i++) {
        s32 off = roots != NULL ? roots[i] : PauseMenuOffsets[i];
        if (off != 0 && off >= send && off + 4 <= len && moved[off / 4] != 0xFFFFFFFF) {
            PauseMenuTexts[i] = (PlaceRec ***)(ModeTextData + moved[off / 4]);
        } else {
            PauseMenuTexts[i] = (PlaceRec ***)ModeTextEmpty;
        }
    }
    return 1;
}

/* PC: the words drawn in this machine's language. The language's copy of the mode text just loaded is read beside the English from each
   mode's root: its 9 sets of pages, each a list of panel lists. A set's strings, the ones its records name, in order and each once, are
   the same in both, though a language lays them out otherwise (more pages, the other records elsewhere): paired one by one, or else page
   by page where those are the same, each English string the game draws has the language's. The game itself reads only the English
   (online play is the same in any language); DrawSysText and SysGetLen draw and measure the language's (ModeTextHeard). A string with no
   partner stays English. */
#define TEXT_MAP_MAX 1024
#define SET_STRINGS 96
static u8 *HeardText;
static s32 HeardLen, HeardSend;
static u8 heard_target[MODE_TEXT_MAX / 4];
static u8 heard_record[MODE_TEXT_MAX / 4];
static const u8 *MapFrom[TEXT_MAP_MAX];
static const u8 *MapTo[TEXT_MAP_MAX];
static s32 MapCount;

static void map_string(const u8 *from, const u8 *to) {
    s32 i;

    for (i = 0; i < MapCount && MapFrom[i] != from; i++) {
    }
    if (i == MapCount && MapCount < TEXT_MAP_MAX) {
        MapFrom[MapCount] = from;
        MapTo[MapCount] = to;
        MapCount++;
    }
}

/* A list's strings (its records' data that's a string), from offset at, after the ones already in out, each once, with how each is drawn
   (its record's frame, the font and colour, and whether it's centred): how many now */
static s32 list_strings(const u8 *d, s32 len, s32 send, s32 at, const u8 **out, s32 *label, s32 n) {
    u32 base = ModeTextBase;
    s32 k, j;

    for (k = 0; k < 40 && at >= send && at + REC_LEN <= len; k++, at += REC_LEN) {
        u32 w0 = word_at(d, at);
        s32 type = REC_TYPE(w0);
        u32 x = word_at(d, at + REC_DATA);
        const u8 *s;

        if (type < 2 || type > 0x20) {
            break;
        }
        if (x < base + 4 || x >= base + (u32)send) {
            continue;
        }
        s = d + (x - base);
        for (j = 0; j < n && strcmp((const char *)out[j], (const char *)s) != 0; j++) {
        }
        if (j == n && n < SET_STRINGS) {
            s32 centred = Europe ? (s16)(w0 >> 16) == -0x8000 : half_at(d, at + 4) == -0x8000;

            label[n] = (s32)(word_at(d, at + REC_DATA + 4) & 0xFF) | centred << 8;
            out[n++] = s;
        }
    }
    return n;
}

/* The list offsets of a set (a table of pointers to lists, ending at 0) */
static s32 set_lists(const u8 *d, s32 len, s32 send, s32 at, s32 *lists) {
    u32 base = ModeTextBase;
    s32 n = 0;

    for (; n < 16 && at >= send && at + 4 <= len; at += 4) {
        u32 x = word_at(d, at);

        if (x < base + (u32)send || x >= base + (u32)len) {
            break;
        }
        lists[n++] = (s32)(x - base);
    }
    return n;
}

/* A set's strings in both, paired in order by how they're drawn: the longest run of the same kinds in the same order (a language can have
   a string more, a second wording, or one fewer), each pair of it */
static void pair_sets(s32 re, s32 rf) {
    const u8 *e = ModeTextBuffer, *f = HeardText;
    u32 base = ModeTextBase;
    s32 k;

    for (k = 0; k < 9; k++) {
        static const u8 *se[SET_STRINGS], *sf[SET_STRINGS];
        static s32 ke[SET_STRINGS], kf[SET_STRINGS];
        static u8 run[SET_STRINGS + 1][SET_STRINGS + 1];
        s32 le[16], lf[16], ne, nf, i, j, n, m;
        u32 xe, xf;

        if (re + k * 4 + 4 > ModeTextLen || rf + k * 4 + 4 > HeardLen) {
            break;
        }
        xe = word_at(e, re + k * 4);
        xf = word_at(f, rf + k * 4);
        if (xe < base + (u32)ModeTextSend || xe >= base + (u32)ModeTextLen || xf < base + (u32)HeardSend || xf >= base + (u32)HeardLen) {
            continue;
        }
        ne = set_lists(e, ModeTextLen, ModeTextSend, (s32)(xe - base), le);
        nf = set_lists(f, HeardLen, HeardSend, (s32)(xf - base), lf);
        for (n = m = i = 0; i < ne; i++) {
            n = list_strings(e, ModeTextLen, ModeTextSend, le[i], se, ke, n);
        }
        for (i = 0; i < nf; i++) {
            m = list_strings(f, HeardLen, HeardSend, lf[i], sf, kf, m);
        }
        /* the longest common run of kinds, from the end, then read from the start */
        for (i = n; i >= 0; i--) {
            for (j = m; j >= 0; j--) {
                if (i == n || j == m) {
                    run[i][j] = 0;
                } else if (ke[i] == kf[j]) {
                    run[i][j] = (u8)(run[i + 1][j + 1] + 1);
                } else {
                    run[i][j] = run[i + 1][j] > run[i][j + 1] ? run[i + 1][j] : run[i][j + 1];
                }
            }
        }
        for (i = j = 0; i < n && j < m;) {
            if (ke[i] == kf[j] && run[i][j] == run[i + 1][j + 1] + 1) {
                map_string(se[i++], sf[j++]);
            } else if (run[i + 1][j] >= run[i][j + 1]) {
                i++;
            } else {
                j++;
            }
        }
    }
}

void ModeTextLanguage(file *p) {
    const unsigned short *roots_en, *roots;
    unsigned base;
    s32 L = archive_language(), entry, size = 0, type = -1, i;

    free(HeardText);
    HeardText = NULL;
    MapCount = 0;
    if (p == NULL || L == 0 || !pal_mode_text(0, &base, &roots_en) || !pal_mode_text(L, &base, &roots)) {
        return;
    }
    entry = p >= DatIndex && p < DatIndex + PC_DAT_FILES ? (s32)(p - DatIndex) : -1;
    if (entry < 0 || (HeardText = archive_language_copy(entry, &size)) == NULL) {
        return;
    }
    HeardLen = size < MODE_TEXT_MAX ? size : MODE_TEXT_MAX;
    HeardSend = strings_end_by(HeardText, HeardLen, is_text_byte_accented);
    find_records(HeardText, HeardLen, base, HeardSend, heard_target, heard_record, 0);
    for (i = 0; i < 14; i++) {
        if (ModeTextFilesLang[i] == p) {
            type = i;
        }
    }
    /* from each root of the modes this file is for */
    for (i = 0; i < 45 && type >= 0; i++) {
        if (ModeTypes[i * 3] - 0x101 == type && roots_en[i] != 0 && roots[i] != 0) {
            pair_sets(roots_en[i], roots[i]);
        }
    }
}

const u8 *ModeTextHeard(const u8 *s) {
    s32 i;

    if (MapCount == 0 || s < ModeTextBuffer || s >= ModeTextBuffer + MODE_TEXT_MAX) {
        return s;
    }
    for (i = 0; i < MapCount; i++) {
        if (MapFrom[i] == s) {
            return MapTo[i];
        }
    }
    return s;
}
