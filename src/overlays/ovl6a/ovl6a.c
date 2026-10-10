#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/ovl6a.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B4694);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B478C);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/ovl6a", D_800B32B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B488C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B4C94);

s32 *func_800B4D8C(void) {
    return D_800C7294;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B4D98);

void func_800B4DE8(void) {
    if (D_8005A688 != 0) {
        func_800B488C();
    }
    func_800B85A8();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B4E1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B4EAC);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/ovl6a", D_800B330C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B5158);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B551C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B5710);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B6224);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B64C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B694C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B6A3C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B6E20);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/ovl6a", D_800B3618);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B6FD4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B77B0);

void func_800B7874(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B787C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B7988);

void func_800B7A78(void) {
    D_800BCEE8 = func_8001DCD4(1, 0x14);
    D_800BCED8 = 0;
    D_800BCEE0 = -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B7AB4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B7B38);

void func_800B7B7C(void) {
    func_80095A04(1);
    func_80095A04(2);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B7BA4);

void func_800B7C78(void) {
    func_800B64C8();
}

void func_800B7C98(void) {
    D_800BCED4 = -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B7CA8);

void func_800B7D7C(void) {
    D_800BCED4 = -1;
}

void func_800B7D8C(void) {
    func_80095A04(1);
    func_80095A04(2);
}

void func_800B7DB4(u8 *arg0, s32 arg1) {
    u8 *temp_a2;

    temp_a2 = (*(s32*)((u8*)(arg0) + 84));
    if (temp_a2 != NULL) {
        if (arg1 != 0) {
            (*(s32*)((u8*)(temp_a2) + 12)) = (s32) ((*(s32*)((u8*)(temp_a2) + 12)) | 0x8000);
            (*(s16*)((u8*)(arg0) + 92)) = 1;
            return;
        }
        (*(s32*)((u8*)(temp_a2) + 12)) = (s32) ((*(s32*)((u8*)(temp_a2) + 12)) & 0xFFFF7FFF);
        (*(s16*)((u8*)(arg0) + 92)) = 0;
    }
}

void func_800B7E04(void) {
}

void func_800B7E0C(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B7E14);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B7F24);

s32 func_800B7F74(u8 *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = (*(s32*)((u8*)(arg1) + 16)) - (*(s32*)((u8*)(arg0) + 16));
    temp_v0_2 = (*(s32*)((u8*)(arg1) + 24)) - (*(s32*)((u8*)(arg0) + 24));
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B7FAC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B7FD0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8098);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8148);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8174);

s32 func_800B821C(void) {
    return 0;
}

s32 func_800B8224(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B822C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8284);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8324);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B838C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B83D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8474);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B84CC);

void func_800B8588(void) {
    func_8009440C();
}

void func_800B85A8(void) {
    if (D_8005A688 == 0) {
        func_800B551C();
        func_800B6A3C();
        func_800139A8();
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B85E4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B86AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8708);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/ovl6a", D_800B37EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B875C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B88B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8AA4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8BC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8CA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B8F50);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B92A0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B93B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B9538);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B96F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B9B0C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B9D1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B9E90);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BA1B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BA374);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BA7BC);

void func_800BA804(void) {
    s32 temp_a0;
    u8 *temp_v0;

    temp_v0 = GetEngineSlot(6);
    if (temp_v0 != NULL) {
        temp_a0 = (*(s32*)((u8*)(temp_v0) + 20));
        if (temp_a0 < 0x21C) {
            (*(s32*)((u8*)(temp_v0) + 20)) = (s32) (temp_a0 + 5);
        }
    }
}

void func_800BA848(void) {
    s32 temp_a0;
    u8 *temp_v0;

    temp_v0 = GetEngineSlot(6);
    if (temp_v0 != NULL) {
        temp_a0 = (*(s32*)((u8*)(temp_v0) + 20));
        if (temp_a0 >= 0x155) {
            (*(s32*)((u8*)(temp_v0) + 20)) = (s32) (temp_a0 - 5);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BA88C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BA8EC);

void func_800BA978(u8 *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = 0;
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s16*)((u8*)(arg0) + 94)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS();
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BA9C8);

void func_800BAAFC(void) {
    func_8009440C();
    func_80018B08(D_8009F67C, D_8009F734);
    func_8001C2C0(GetEngineSlot(6));
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BAB40);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/ovl6a", D_800B3894);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BABEC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BB038);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BB280);

void func_800BB364(void) {
    func_80095A04(0);
    func_80095A04(1);
}

void func_800BB38C(void) {
    func_800BABEC();
    if (D_80051380 & 9) {
        D_8009F65C = D_8009F46C;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BB3D0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BB4AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BB584);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BB5F0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BBC64);

void func_800BBE1C(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BBE24);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800BBF44);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/ovl6a", D_800B3928);
