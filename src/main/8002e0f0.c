#include "common.h"
#include "bash/main.h"

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

INCLUDE_ASM("asm/nonmatchings/main/8002e0f0", func_8002E618);
