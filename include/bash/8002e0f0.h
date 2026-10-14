#ifndef BASH_8002E0F0_H
#define BASH_8002E0F0_H

/* Functions and data defined in src/main/8002e0f0.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/8002e0f0.c */

#include "common.h"
#include "bash/nas.h"

/* The debug text printer's cursor (func_8002E25C). */
typedef struct TextPos {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 w; /* width of the last glyph drawn */
    /* 0x06 */ s16 unk6;
} TextPos;

extern s32 D_8006EA24; /* nonzero: ASCII from the BIOS ROM font, else Kanji (Krom2RawAdd) */

void func_8002E0F0(void);

void func_8002E25C(u8 *str, TextPos *pos, s32 arg2);
void func_8002E3D0(TextPos *pos, u8 *glyph, s32 arg2, s32 arg3);
s32 func_8002E584(u8 *glyph);
void func_8002E618(void);

#endif /* BASH_8002E0F0_H */
