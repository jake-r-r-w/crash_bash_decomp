#include "common.h"
#include "bash/ovl5d.h"
#include "bash/bash.h"
#include "bash/xtras.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B3A38);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5d/ovl5d", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B3DD8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B432C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B44B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B46B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B4848);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5d/ovl5d", D_800B3424);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B4AB0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B5E28);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B6138);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B62EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B6DA4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B6F0C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B6FE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B710C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B7358);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B7D20);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B7E74);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B8040);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B8314);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B8578);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotSnowGlint);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotBashPenguin);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B8B00);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B8C9C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitTumbler);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotTumbler);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B94C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B95F0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B97A8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitBash8way);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotBash8way);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitBashHomer);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotBashHomer);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitBashShox);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotBashShox);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotMissileExplode);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitMissileExplode);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BA8A4);

void PlotBoing(u8 *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        func_800250B0((NAS *)arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitBoing);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitBashPenguin);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitSnowGlint);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BAC10);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BAC5C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BAD0C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BAD70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BAE08);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BAE98);

void func_800BAF68(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            D_800C3BAC[j][i].unkA = 0;
            D_800C3BAC[j][i].unk8 = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BAFB4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BAFDC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BB100);

s32 func_800BB1B4(u8 *arg0) {
    s32 temp_a0;
    s32 temp_v1;
    s32 var_a2;

    var_a2 = 0;
    if ((*(s32*)((u8*)(arg0) + 144)) & 0x40) {
        temp_a0 = 0x800 - (*(s16*)((u8*)(arg0) + 138));
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
        if ((*(s32*)((u8*)(arg0) + 16)) >= 0x801) {
            (*(s32*)((u8*)(arg0) + 16)) = 0x800;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 16)) < -0x800) {
            (*(s32*)((u8*)(arg0) + 16)) = -0x800;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) >= 0x801) {
            (*(s32*)((u8*)(arg0) + 24)) = 0x800;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < -0x800) {
            (*(s32*)((u8*)(arg0) + 24)) = -0x800;
block_17:
            var_a2 = 1;
        }
    }
    return var_a2;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BB2BC);

void func_800BB360(void) {
}

void func_800BB368(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BB370);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BB3D0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BB4AC);
