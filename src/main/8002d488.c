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

INCLUDE_ASM("asm/nonmatchings/main/8002d488", func_8002E000);
