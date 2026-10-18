#include "common.h"
#include "bash/main.h"
#include "bash/ovl5e.h"
#include "bash/papu.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B35AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B36D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B38FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B3B08);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B3D18);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B3E80);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4154);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4204);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B42C0);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5e/ovl5e", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", InitPapuBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", PlotPapuBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4B9C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4D70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4EE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", PlotPapuMinion);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B5E44);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6008);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B61A0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6334);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B64AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B668C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6860);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6D14);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6FF8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7134);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B744C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7558);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7600);

s32 func_800B7670(u8 *arg0) {
    s32 temp_a0;
    s32 temp_v1;
    s32 var_a2;

    var_a2 = 0;
    if ((*(s32*)((u8*)(arg0) + 144)) & 0x40) {
        temp_a0 = 0x6A0 - (*(s16*)((u8*)(arg0) + 138));
        if (temp_a0 < (*(s32*)((u8*)(arg0) + 16))) {
            (*(s32*)((u8*)(arg0) + 16)) = temp_a0;
            var_a2 = 1;
        }
        temp_v1 = -temp_a0;
        if ((*(s32*)((u8*)(arg0) + 16)) < temp_v1) {
            (*(s32*)((u8*)(arg0) + 16)) = temp_v1;
            var_a2 = 1;
        }
        if (temp_a0 < (*(s32*)((u8*)(arg0) + 24))) {
            (*(s32*)((u8*)(arg0) + 24)) = temp_a0;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < temp_v1) {
            (*(s32*)((u8*)(arg0) + 24)) = temp_v1;
            goto block_17;
        }
    } else {
        if ((*(s32*)((u8*)(arg0) + 16)) >= 0x601) {
            (*(s32*)((u8*)(arg0) + 16)) = 0x600;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 16)) < -0x600) {
            (*(s32*)((u8*)(arg0) + 16)) = -0x600;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) >= 0x601) {
            (*(s32*)((u8*)(arg0) + 24)) = 0x600;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < -0x600) {
            (*(s32*)((u8*)(arg0) + 24)) = -0x600;
block_17:
            var_a2 = 1;
        }
    }
    return var_a2;
}

void func_800B7778(void) {
    u8 *temp_v1;
    u8 *var_a0;

    var_a0 = D_8005AA24;
    if (var_a0 != NULL) {
        do {
            temp_v1 = (*(s32*)((u8*)(var_a0) + 88));
            if (temp_v1 != NULL) {
                if ((*(u16*)((u8*)(temp_v1) + 26)) == 1) {
                    (*(u16*)((u8*)(temp_v1) + 26)) = 4U;
                } else {
                    (*(u16*)((u8*)(temp_v1) + 26)) = 5U;
                }
            }
            var_a0 = (*(s32*)((u8*)(var_a0) + 44));
        } while (var_a0 != NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B77D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7894);

void func_800B790C(s32 arg0) {
    if (func_800B744C() != 0) {
        func_80084E04(arg0, 0, -1, 3, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B794C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", PlotPapuColumn);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", InitPapuColumn);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7B10);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7B4C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7BE4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7CBC);

void func_800B7D94(void) {
}

void func_800B7D9C(void) {
}

void func_800B7DA4(void) {
    func_80086F78();
    func_800B80B0();
    func_80086D88();
    func_800B3B08();
    func_800B7134();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7DE4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7E38);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7F14);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B8080);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B80B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B82DC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B8508);
