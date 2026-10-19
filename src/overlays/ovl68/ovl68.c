#include "common.h"
#include "bash/ovl68.h"
#include "bash/chase.h"
#include "bash/pogo.h"

void func_800B3330(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B3338);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B3488);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl68/ovl68", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B37C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B3EFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B4048);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B4444);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B44C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B459C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B46BC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B48E4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B4B34);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B51C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B52C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B5CD0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B5E84);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6168);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B627C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6350);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B643C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B65B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B66A4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B67D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B68B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B69AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6AAC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6B84);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6C84);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6E84);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6F6C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B722C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotBarrelGenerator);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitBarrelCH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotBarrelCH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B7948);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B7B80);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B7DF0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCoalFlame);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCircleFire);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotEgg);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCoalFlameSFX);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotPUGenerator_CH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitPickupCH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotPickupCH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B94FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B9A50);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B9ED8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA220);

void func_800BA5A0(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            func_800B94FC(arg0, (u16)arg1);
        } else if (f & 0x20) {
            func_800B9A50(arg0, (u16)arg1);
        } else if (f & 0x40) {
            func_800B9ED8(arg0, (u16)arg1);
        } else {
            func_800BA220(arg0, (u16)arg1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA630);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitPUGenerator_CH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA694);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA70C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA784);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitCoalFlameSFX);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitEgg);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitCircleFire);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitCoalFlame);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCoal);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitCoal);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAC30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitBarrelGenerator);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAC98);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAD1C);

void func_800BADA8(s32 arg0) {
    func_800B52C8();
    func_8008502C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BADD4);

void func_800BAE20(void) {
}

void func_800BAE28(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAE30);

s32 func_800BAE84(void) {
    return 0;
}

s32 func_800BAE8C(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAE94);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAF00);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAF58);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAFB4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB010);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB134);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB1C0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB260);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB294);

void func_800BB36C(void) {
}

void func_800BB374(void) {
}

void func_800BB37C(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB384);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB3F0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB428);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB488);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB4F8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB5A0);

void func_800BB640(s32 arg0) {
    u8 *temp_v0;

    temp_v0 = (arg0 * 0x30) + D_800BC210;
    (*(s32*)((u8*)(temp_v0) + 0)) = 1;
    (*(s32*)((u8*)(temp_v0) + 4)) = 1;
    (*(s32*)((u8*)(temp_v0) + 8)) = 0;
    (*(s32*)((u8*)(temp_v0) + 24)) = 0;
    (*(s32*)((u8*)(temp_v0) + 28)) = 0;
    (*(s32*)((u8*)(temp_v0) + 32)) = 0;
    (*(s32*)((u8*)(temp_v0) + 36)) = 0;
    (*(s32*)((u8*)(temp_v0) + 40)) = 0x8000;
    (*(s32*)((u8*)(temp_v0) + 44)) = 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB688);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB6FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB7E0);
