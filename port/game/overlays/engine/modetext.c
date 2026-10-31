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

static s32 is_text_byte(u8 c) {
    return (c >= 0x20 && c < 0x7F) || c == 0x0A || c == 0x0D || c >= 0xF0;
}

static u32 word_at(const u8 *d, s32 off) {
    return d[off] | (d[off + 1] << 8) | (d[off + 2] << 16) | ((u32)d[off + 3] << 24);
}

static s16 half_at(const u8 *d, s32 off) {
    return (s16)(d[off] | (d[off + 1] << 8));
}

/* where the strings end */
static s32 strings_end(const u8 *d, s32 len) {
    s32 p = 4;
    s32 end = 4;

    while (p < len) {
        s32 e = p;
        s32 next;
        while (e < len && d[e] != 0 && is_text_byte(d[e])) {
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

/* Translates the file just loaded into ModeTextBuffer. Returns 0 if it doesn't fit. */
s32 ModeTextTranslate(s32 len) {
    static u8 is_target[MODE_TEXT_MAX / 4];
    static u8 is_record[MODE_TEXT_MAX / 4];
    static u32 moved[MODE_TEXT_MAX / 4];
    const u8 *d = ModeTextBuffer;
    u32 out = 0;
    s32 send;
    s32 p;
    s32 i;

    ModeTextLists = 0;
    ModeTextRecords = 0;
    for (i = 0; i < MODE_TEXT_MAX / 4; i++) {
        is_target[i] = 0;
        is_record[i] = 0;
        moved[i] = 0xFFFFFFFF;
    }
    if (len > MODE_TEXT_MAX) {
        return 0;
    }
    send = strings_end(d, len);
    for (p = send; p + 4 <= len; p += 4) {
        u32 x = word_at(d, p);
        if (x >= MODE_TEXT_BASE + send && x < MODE_TEXT_BASE + len && !(x & 3)) {
            is_target[(x - MODE_TEXT_BASE) / 4] = 1;
        }
    }
    /* the panel lists: every target holding records of types 2..0x20, ending at one below 2 */
    for (p = send; p + 4 <= len; p += 4) {
        s32 k = p;
        s32 n = 0;
        if (!is_target[p / 4]) {
            continue;
        }
        while (k + MODE_TEXT_REC <= len) {
            s32 type = (s32)word_at(d, k);
            if (type < 2) {
                break;
            }
            if (type > 0x20) {
                n = -1;
                break;
            }
            n++;
            k += MODE_TEXT_REC;
        }
        if (n > 0 && k + MODE_TEXT_REC <= len) {
            s32 j;
            ModeTextLists++;
            for (j = 0; j <= n; j++) {
                s32 r = p + j * MODE_TEXT_REC;
                if (j < n && !is_record[r / 4]) {
                    ModeTextRecords++;
                }
                is_record[r / 4] = 1;
            }
        }
    }
    /* lay it out: where each word goes */
    for (p = send; p + 4 <= len;) {
        moved[p / 4] = out;
        if (is_record[p / 4]) {
            out += sizeof(PlaceRec);
            p += MODE_TEXT_REC;
        } else {
            out += sizeof(void *);
            p += 4;
        }
    }
    if (out > sizeof(ModeTextData)) {
        return 0;
    }

#define MODE_TEXT_PTR(x)                                                                                             \
    ((x) >= MODE_TEXT_BASE && (x) < MODE_TEXT_BASE + len                                                             \
         ? ((x) - MODE_TEXT_BASE < (u32)send || moved[((x) - MODE_TEXT_BASE) / 4] == 0xFFFFFFFF                      \
                ? (void *)(ModeTextBuffer + ((x) - MODE_TEXT_BASE))                                                  \
                : (void *)(ModeTextData + moved[((x) - MODE_TEXT_BASE) / 4]))                                        \
         : (void *)(iptr)(x))

    for (p = send; p + 4 <= len;) {
        u8 *to = ModeTextData + moved[p / 4];
        if (is_record[p / 4]) {
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
        s32 off = PauseMenuOffsets[i];
        if (off != 0 && off >= send && off + 4 <= len && moved[off / 4] != 0xFFFFFFFF) {
            PauseMenuTexts[i] = (PlaceRec ***)(ModeTextData + moved[off / 4]);
        } else {
            PauseMenuTexts[i] = (PlaceRec ***)ModeTextEmpty;
        }
    }
    return 1;
}
