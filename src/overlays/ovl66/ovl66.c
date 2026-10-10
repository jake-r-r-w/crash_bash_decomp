#include "common.h"
#include "prototypes.h"
#include "externs.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B33A4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B347C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B34F8);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl66/ovl66", D_800B32B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B3668);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B3D28);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B3F10);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B4038);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6238);

s32 func_800B62C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_a0;
    s32 temp_a1;

    temp_a0 = arg2 - arg0;
    temp_a1 = arg3 - arg1;
    return (temp_a0 * temp_a0) + (temp_a1 * temp_a1);
}

void func_800B62EC(void) {
    func_800250B0();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B630C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6380);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6414);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B64B4);

void func_800B6560(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6568);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B65F0);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6AAC);

void func_800B6AF4(void) {
}

void func_800B6AFC(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6B04);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6CD8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6E1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B6EFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B70C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B717C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B72FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B738C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B747C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B76CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B77FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B7908);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B7B48);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8550);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8714);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B876C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8814);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B886C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8918);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B89D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8A30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8AC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8AF4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8B3C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8BA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8C3C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8CB4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8D3C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8D88);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8E08);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8E64);

void func_800B8EF0(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8EF8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B8F64);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B9054);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B9144);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", func_800B91BC);

void func_800B9264(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        func_800B91BC(var_s0);
        var_s0 += 1;
    } while (var_s0 < 4);
}
