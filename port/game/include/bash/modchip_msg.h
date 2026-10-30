#ifndef BASH_MODCHIP_MSG_H
#define BASH_MODCHIP_MSG_H

/* Functions and data defined in src/ps1/modchip_msg.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/ps1/modchip_msg.c */

#include "common.h"
#include "bash/nas.h"

/* The BIOS ROM's 8x15 ASCII font, from '!' (msg_print, when the kanji ROM isn't used). */
#define ROM_ASCII_FONT ((u8 *)0xBFC7F8DE)

/* The debug text printer's cursor (msg_print). */
typedef struct TextPos {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 w; /* width of the last glyph drawn */
    /* 0x06 */ s16 unk6;
} TextPos;

/* LoadImage packet for one glyph row pair (msg_draw_glyph) */
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
    /* 0x01 */ u8 pad1;
    /* 0x02 */ u8 w;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 x;
    /* 0x06 */ s16 y;
    /* 0x08 */ u8 *text;
} TextScreen;

extern TextScreen ModchipMsgScreens[];

extern s32 MsgFontRom; /* nonzero: ASCII from the BIOS ROM font, else Kanji (Krom2RawAdd) */

void modchip_show_warning(void);

void msg_print(u8 *str, TextPos *pos, s32 arg2);
void msg_draw_glyph(TextPos *pos, u8 *glyph, s32 colour, s32 bits);
void msg_draw_emblem(void);

#endif /* BASH_MODCHIP_MSG_H */
