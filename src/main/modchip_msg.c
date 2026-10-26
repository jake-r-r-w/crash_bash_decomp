#include "common.h"
#include "bash/main.h"
#include "bash/modchip_msg.h"

/* the debug text screens, 12 bytes each, indexed by MsgFontRom */
extern u8 D_8006789E[]; /* +2: u8 */
extern u8 D_800678A0[]; /* +4: s16 x */
extern u8 D_800678A2[]; /* +6: s16 y */
extern u8 D_800678A4[]; /* +8: u8 *text */

void func_8002E0F0(void) {
    DRAWENV draw;
    DISPENV disp;
    DR_ENV env;
    u32 clear[3];
    TextPos pos;
    s32 colour;
    s32 i;
    s32 o;
    s32 w;
    s16 x;
    s16 y;

    ResetGraph(1);
    StopCallback();
    ResetGraph(5);
    *(s16 *)0x1F801DAA = 0;
    SetDefDrawEnv(&draw, 0, 0, 320, 240);
    SetDefDispEnv(&disp, 0, 0, 320, 240);
    draw.isbg = 1;
    SetDrawEnv(&env, &draw);
    DrawPrim(&env);
    PutDispEnv(&disp);
    clear[0] = 0x02000000;
    clear[1] = 0xE6000002;
    clear[2] = 0;
    DrawPrim(clear);
    colour = 0xFFFF;
    i = 0;
    w = D_8006789E[MsgFontRom * 12];
    pos.unk6 = 1;
    pos.w = w;
    do {
        o = MsgFontRom * 12;
        x = *(s16 *)(D_800678A0 + o);
        y = *(s16 *)(D_800678A2 + o);
        pos.x = x + i;
        pos.y = y + i;
        i++;
        func_8002E25C(*(u8 **)(D_800678A4 + o), &pos, colour);
        colour = 0x8000;
    } while (i < 2);
    func_8002E618();
    SetDispMask(1);
    exit();
}

void func_8002E25C(u8 *str, TextPos *pos, s32 arg2) {
    s32 len;
    u8 *base;
    s32 startX;
    u8 *glyph;
    s32 w;
    u8 c;

    base = str;
    len = strlen(str);
    startX = pos->x;
    for (; str < base + len; str++) {
        c = *str;
        if (c == '\n') {
            pos->x = startX;
            pos->y += 18;
        } else if (MsgFontRom == 0) {
            u32 hi = *(volatile u8 *)str++;
            glyph = (u8 *)Krom2RawAdd(*(volatile u8 *)str | (hi << 8));
            func_8002E3D0(pos, glyph, arg2, 7);
            pos->x = pos->w + (s16)(pos->x + 1);
        } else if (c == ' ') {
            pos->x += 8;
        } else {
            w = (*(volatile u8 *)str - 0x21) * 15;
            glyph = w + (u8 *)0xBFC7F8DE;
            w = func_8002E584(glyph);
            pos->w = w - ((u32)w >> 16) + 1;
            func_8002E3D0(pos, glyph, arg2, w);
            pos->x = pos->w + (s16)(pos->x + 2);
        }
    }
}

void func_8002E3D0(TextPos *pos, u8 *glyph, s32 colour, s32 bits) {
    GlyphRow p;
    s32 x;
    s32 y;
    s16 *out;
    s16 *dst;
    u16 hi;
    u16 lo;
    s32 bg;
    s32 b;
    s32 i;
    s32 row;

    lo = bits;
    hi = bits >> 16;
    x = pos->x;
    y = pos->y;
    row = 0;
    p.tag = (((pos->w + 1) >> 1) + 3) << 24;
    p.cmd = 0xA0000000;
    p.wh = *(s32 *)&pos->w;
    do {
        out = p.data;
        i = 0;
        if (D_8006789C[MsgFontRom].rowBytes != 0) {
            do {
                for (b = lo; b >= hi; b--) {
                    dst = out++;
                    bg = (u16)b;
                    bg >>= 16;
                    if ((*glyph >> b) & 1) {
                        bg = colour;
                    }
                    *dst = bg;
                }
                i++;
                glyph++;
            } while (i < D_8006789C[MsgFontRom].rowBytes);
        }
        for (i = 0; i < 2; i++) {
            p.xy = *(s32 *)pos;
            DrawPrim(&p);
            pos->x++;
        }
        row++;
        pos->x = x;
        pos->y++;
    } while (row < 15);
    pos->y = y;
}

s32 func_8002E584(u8 *arg0) {
    u16 max;
    u16 min;
    s32 i;
    s32 bit;
    s32 first;
    s32 b;

    max = 0;
    min = 7;
    i = 0xE;
    do {
        first = 1;
        bit = 7;
        b = *arg0;
        do {
            if ((b >> bit) & 1) {
                if (first != 0) {
                    if (max < bit) {
                        max = bit;
                    }
                    if (bit < min) {
                        min = bit;
                    }
                    first = 0;
                } else {
                    if (bit < min) {
                        min = bit;
                    }
                }
            }
            bit--;
        } while (bit >= 0);
        i--;
        arg0++;
    } while (i >= 0);
    return (min << 16) | max;
}

const char ModchipMsg[] = "     SOFTWARE TERMINATED\nCONSOLE MAY HAVE BEEN MODIFIED\n     CALL 1-888-780-7690";
const char ModchipMsgJapanese[] = "\213\255\220\247\217I\227\271\202\265\202\334\202\265\202\275\201B\n\226{\221\314\202\252\211\374\221\242\202\263\202\352\202\304\202\242\202\351\n\202\250\202\273\202\352\202\252\202\240\202\350\202\334\202\267\201B";

void func_8002E618(void) {
    extern u8 D_800678B4[][2];
    extern void DrawPrim(void *);
    s32 prim[6];
    s32 i;
    s32 k;
    s32 j;

    prim[0] = 0x05000000;
    prim[1] = 0x280000FF;
    i = 0;
    do {
        k = 0;
        do {
            s8 d[4][2] = { { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
            j = 0;
            do {
                s32 idx = (j + 2) >> 2;
                s32 x = i + (j & 1);
                prim[j + 2] = ((d[k][1] * D_800678B4[16 - x][idx] + 0x78) << 16) |
                              (d[k][0] * D_800678B4[x][idx] + 0xA0);
                j++;
            } while (j < 4);
            DrawPrim(prim);
            k++;
        } while (k < 4);
        i++;
    } while (i < 16);
    prim[2] = 0x500070;
    prim[3] = 0x480078;
    prim[4] = 0xA800C8;
    prim[5] = 0xA000D0;
    DrawPrim(prim);
}
