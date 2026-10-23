#ifndef BASH_CARD_H
#define BASH_CARD_H

/* Functions and data defined in src/main/card.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/card.c */

#include "common.h"
#include "bash/main.h"

void func_8002C894(void);
s32 func_8002C4B0(u8 arg0);
void func_8002D134(s32 arg0);
void func_8002D15C(void);
void func_8002D254(void);
void func_8002D274(void);
void func_8002D33C(u8 *arg0);
s32 func_8002C4B0(u8 arg0);
void card_save(void);
void func_8002D108(void);
s32 func_8002D094(void);
s32 func_8002D360(u8 *arg0);
s32 func_8002D178(void);
SizeEntry *func_8002D1D4(void);
void func_8002D294(void);
void func_8002D394(u8 *arg0, s16 *arg1);
void func_8002C5BC(void);
void func_8002C97C(void);

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
    r2 = func_8002C4B0(0x20);
    fill = ((r2 & 0xFF) << 8) | ((u32)(func_8002C4B0(0x20) & 0xFFFF) >> 8);
    do {
        if (done == 0) {
            c = src[i];
            i++;
            if (c >= 0x20U) {
                r = func_8002C4B0(c);
                dst[n] = ((r & 0xFF) << 8) | ((u32)(func_8002C4B0(c) & 0xFFFF) >> 8);
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
            if (strcmp(D_8004CDAC, (char *)p) == 0) {
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
            D_800657C8[i].b[0] = 0;
            D_800657C8[i].b[1] = 0;
            D_800657C8[i].b[2] = 0;
            D_800657C8[i].b[3] = 0;
            D_800657C8[i].b[4] = 0;
            D_800657C8[i].b[5] = 0;
            D_800657C8[i].b[6] = 0;
            D_800657C8[i].b[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = 0x850;
    *(s32 *)(D_800655C8 + new_var) = 1;
    i = 7;
    p = D_800655C8 + i;
    do {
        p[0x854] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)(D_800655C8 + new_var);
    q = (s32 *)(D_800655C8 + 0x200);
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
            D_800657C8[i].b[0] = 0;
            D_800657C8[i].b[1] = 0;
            D_800657C8[i].b[2] = 0;
            D_800657C8[i].b[3] = 0;
            D_800657C8[i].b[4] = 0;
            D_800657C8[i].b[5] = 0;
            D_800657C8[i].b[6] = 0;
            D_800657C8[i].b[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = 0x850;
    *(s32 *)(D_800655C8 + new_var) = 1;
    i = 7;
    p = D_800655C8 + i;
    do {
        p[0x854] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)(D_800655C8 + new_var);
    q = (s32 *)(D_800655C8 + 0x200);
    sum = 0;
    while (q < end) {
        sum += *q++;
    }
    D_80065E24 = sum;
}
static inline void McClearSave(void) {
    s32 *q;
    s32 i;

    q = (s32 *)D_800655C8;
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
