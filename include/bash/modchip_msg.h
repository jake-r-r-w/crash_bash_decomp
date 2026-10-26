#ifndef BASH_MODCHIP_MSG_H
#define BASH_MODCHIP_MSG_H

/* Functions and data defined in src/main/modchip_msg.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/modchip_msg.c */

#include "common.h"
#include "bash/nas.h"

/* The debug text printer's cursor (func_8002E25C). */
typedef struct TextPos {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 w; /* width of the last glyph drawn */
    /* 0x06 */ s16 unk6;
} TextPos;

/* LoadImage packet for one glyph row pair (func_8002E3D0) */
typedef struct GlyphRow {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 cmd;
    /* 0x08 */ s32 xy;
    /* 0x0C */ s32 wh;
    /* 0x10 */ s16 data[16];
} GlyphRow;

/* the debug text screens, indexed by MsgFontRom */
typedef struct TextScreen {
    /* 0x00 */ u8 rowBytes; /* bytes per glyph row */
    /* 0x01 */ u8 unk1[11];
} TextScreen;

extern TextScreen D_8006789C[];

extern s32 MsgFontRom; /* nonzero: ASCII from the BIOS ROM font, else Kanji (Krom2RawAdd) */

void func_8002E0F0(void);

void func_8002E25C(u8 *str, TextPos *pos, s32 arg2);
void func_8002E3D0(TextPos *pos, u8 *glyph, s32 colour, s32 bits);
void func_8002E618(void);

#endif /* BASH_MODCHIP_MSG_H */
