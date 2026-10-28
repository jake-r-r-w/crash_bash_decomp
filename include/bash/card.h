#ifndef BASH_CARD_H
#define BASH_CARD_H

/* Functions and data defined in src/main/card.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/card.c */

#include "common.h"
#include "bash/main.h"

void card_init(void);
s32 ascii_to_sjis(u8 arg0);
void card_request(s32 arg0);
void card_reset(void);
void card_stop(void);
void card_poll(void);
void card_clear_slot(u8 *arg0);
s32 ascii_to_sjis(u8 arg0);
void card_save(void);
void card_format(void);
s32 card_saving(void);
s32 card_checksum(u8 *arg0);
s32 card_count_files(void);
SizeEntry *card_find_file(void);
void card_clear_slots(void);
void card_sjis_title(u8 *arg0, s16 *arg1);
void card_make_header(void);
void card_update(void);

static inline void McSetTitle(u8 *src, s16 *dst) {
    s16 fill;
    s32 i;
    s32 n;
    s32 done;
    u8 c;
    s32 r;
    s32 r2;

    i = 0;
    done = 0;
    n = 0;
    r2 = ascii_to_sjis(0x20);
    fill = ((r2 & 0xFF) << 8) | ((u32)(ascii_to_sjis(0x20) & 0xFFFF) >> 8);
    do {
        if (done == 0) {
            c = src[i];
            i++;
            if (c >= 0x20U) {
                r = ascii_to_sjis(c);
                dst[n] = ((r & 0xFF) << 8) | ((u32)(ascii_to_sjis(c) & 0xFFFF) >> 8);
            } else {
                done = 1;
            }
            if (done != 0) {
                goto fillit;
            }
        } else {
fillit:
            dst[n] = fill;
        }
        n++;
    } while (n < 0x20);
}
static inline SizeEntry *McFindSave(void) {
    SizeEntry *p;
    s32 i;

    i = D_800675D0;
    p = D_800675D4;
    i--;
    if (i != -1) {
        do {
            if (strcmp(CardFileName, (char *)p) == 0) {
                return p;
            }
            i--;
            p++;
        } while (i != -1);
    }
    return NULL;
}
static inline void McResetSave(void) {
    u8 *p;
    s32 i;
    s32 *q;
    s32 *end;
    s32 sum;
    int new_var;

    i = 0;
    for (;;) {
        if (i < 4) {
            CardSlots[i].b[0] = 0;
            CardSlots[i].b[1] = 0;
            CardSlots[i].b[2] = 0;
            CardSlots[i].b[3] = 0;
            CardSlots[i].b[4] = 0;
            CardSlots[i].b[5] = 0;
            CardSlots[i].b[6] = 0;
            CardSlots[i].b[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = 0x850;
    *(s32 *)(CardBuffer + new_var) = 1;
    i = 7;
    p = CardBuffer + i;
    do {
        p[0x854] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)(CardBuffer + new_var);
    q = (s32 *)(CardBuffer + 0x200);
    i = 0;
    while (q < end) {
        i += *q++;
    }
    D_80065E24 = i;
}
static inline void McResetSaveSum(void) {
    u8 *p;
    s32 i;
    s32 *q;
    s32 *end;
    s32 sum;
    int new_var;

    i = 0;
    for (;;) {
        if (i < 4) {
            CardSlots[i].b[0] = 0;
            CardSlots[i].b[1] = 0;
            CardSlots[i].b[2] = 0;
            CardSlots[i].b[3] = 0;
            CardSlots[i].b[4] = 0;
            CardSlots[i].b[5] = 0;
            CardSlots[i].b[6] = 0;
            CardSlots[i].b[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = 0x850;
    *(s32 *)(CardBuffer + new_var) = 1;
    i = 7;
    p = CardBuffer + i;
    do {
        p[0x854] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)(CardBuffer + new_var);
    q = (s32 *)(CardBuffer + 0x200);
    sum = 0;
    while (q < end) {
        sum += *q++;
    }
    D_80065E24 = sum;
}
static inline void McClearSave(void) {
    s32 *q;
    s32 i;

    q = (s32 *)CardBuffer;
    i = 0x7FF;
    do {
        *q = 0;
        i--;
        q++;
    } while (i != -1);
}
static inline s32 McFreeBytes(void) {
    SizeEntry *p;
    s32 i;
    s32 sum;

    i = D_800675D0;
    if (i >= 0xF) {
        return 0;
    }
    p = D_800675D4;
    i--;
    sum = 0;
    if (i != -1) {
        do {
            sum += p->size;
            p++;
            i--;
        } while (i != -1);
    }
    return 0x1E000 - sum;
}

#endif /* BASH_CARD_H */
