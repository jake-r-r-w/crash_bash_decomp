#include "common.h"
#include "bash/main.h"
#include "bash/ovl66.h"
#include "bash/bonk.h"
#include "bash/pogo.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B33A4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B347C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B34F8);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl66/ovl66", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", PlotBonkStool);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B3D28);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", InitBonkCrate);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", PlotBonkCrate);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B4454);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B47C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B4910);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B4BC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B5050);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B54D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B5820);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B5BA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B60FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B61B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B61F8);

void func_800B6238(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            func_800B4BC8(arg0, (u16)arg1);
        } else if (f & 0x20) {
            func_800B5050(arg0, (u16)arg1);
        } else if (f & 0x40) {
            func_800B54D8(arg0, (u16)arg1);
        } else {
            func_800B5820(arg0, (u16)arg1);
        }
    }
}

s32 func_800B62C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_a0;
    s32 temp_a1;

    temp_a0 = arg2 - arg0;
    temp_a1 = arg3 - arg1;
    return (temp_a0 * temp_a0) + (temp_a1 * temp_a1);
}

void PlotBonkFlag(NAS *nas) {
    func_800250B0(nas);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", InitBonkFlag);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", PlotBonkShockwave);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", InitBonkShockwave);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B64B4);

void func_800B6560(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6568);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", InitBonkStool);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B66C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6774);

void func_800B67C4(void) {
    func_800B34F8();
    func_800B6560();
    func_800B6774();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B67F4);

void func_800B6868(s32 arg0) {
    func_8007B7FC(arg0 & 0xFFFF, 0x81D0);
}

void func_800B688C(void) {
    if (!(D_8005A646 & 0xE0)) {
        func_800B6868(0x130);
    }
}

void func_800B68BC(void) {
    if (D_8005A646 & 0x20) {
        func_800B6868(0x130);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B68EC);

void func_800B69A0(void) {
    if (D_8005A646 & 0x80) {
        func_800B6868(0x130);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B69D0);

void func_800B6A1C(void) {
    D_8005A5D8 = -0x10;
    func_8007EB1C(0x140, 0x28, 0);
    func_8007B7FC(0x130, 0x81D0);
    func_800B69D0();
    D_800B9EEC = 0x10;
    func_800B6774();
    func_800B69A0();
    func_800B68EC();
    func_800B68BC();
    func_800B688C();
    func_800B9264();
    func_800B347C();
    func_800B6568();
}

void func_800B6AAC(void) {
    D_8005AA70[0] = (s32)func_800B33A4;
    D_8005AA70[1] = (s32)func_800B6A1C;
    D_8005AA70[2] = (s32)func_800B6A1C;
    D_8005AA70[3] = (s32)func_800B67C4;
    D_8005AA70[5] = (s32)func_800B6AFC;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
}

void func_800B6AF4(void) {
}

void func_800B6AFC(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6B04);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6CD8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6E1C);
