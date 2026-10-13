#include "common.h"
#include "bash/main.h"
#include "bash/8002e0f0.h"

INCLUDE_ASM("asm/nonmatchings/main/8002e0f0", func_8002E0F0);

INCLUDE_ASM("asm/nonmatchings/main/8002e0f0", func_8002E25C);

INCLUDE_ASM("asm/nonmatchings/main/8002e0f0", func_8002E3D0);

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

INCLUDE_RODATA("asm/nonmatchings/main/8002e0f0", D_8004CE5C);

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
