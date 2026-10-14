#include "common.h"
#include "bash/8002d488.h"

extern u8 D_BFC7FF52;
extern s32 D_80067894;
extern s32 D_8006EA24;

void func_8002D488(void) {
    switch (D_BFC7FF52) {
    case 0x41:
        D_80067894 = 1;
        D_8006EA24 = 1;
        return;
    case 0x45:
        D_80067894 = 0;
        D_8006EA24 = 2;
        return;
    case 0x4A:
    default:
        D_80067894 = 1;
        D_8006EA24 = 0;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8002d488", func_8002D4F4);

INCLUDE_ASM("asm/nonmatchings/main/8002d488", func_8002DE2C);

void func_8002E000(s32 arg0) {
    s32 i;
    s32 off;

    i = 0;
    *D_80067874 = 1;
    *D_80067880 = 7;
    do {
        *(s32 *)0 = i;
        i++;
    } while (i < 4);
    *D_80067874 = 1;
    *D_8006787C = 0x18;
    *D_80067874 = 0;
    off = arg0 * 4;
    i = 0;
    if (D_8006783C[off + 1] != 0) {
        do {
            *D_8006787C = D_80067884[i];
            i++;
        } while (i < D_8006783C[off + 1]);
    }
    *D_80067874 = 0;
    *D_80067878 = D_8006783C[arg0 * 4];
}
