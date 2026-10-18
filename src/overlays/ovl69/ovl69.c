#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/ovl69.h"
#include "bash/titles.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B37BC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B39C8);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl69/ovl69", D_800B32B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B3A9C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B3CA8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B3F7C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B42B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B4464);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B4770);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B4848);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B4910);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B4F1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B51C0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B51EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5218);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5244);

void func_800B5324(void) {
}

void func_800B532C(void) {
    func_80095BEC(4, D_800B8F20);
    D_800B9508 = 0;
    D_800B950C = 0;
}

void func_800B5360(s32 *arg0) {
    D_800B9620 = arg0;
    func_8001E848(&D_8009F8A4, *arg0);
}

void func_800B5390(s32 arg0) {
    s32 *temp_v1;
    s32 temp_a1;

    temp_v1 = &D_800B9620[arg0];
    temp_a1 = *temp_v1;
    D_800B9620 = temp_v1;
    if (temp_a1 != 0) {
        func_8001E848(&D_8009F8A4, temp_a1);
    }
}

void func_800B53D4(void) {
    s32 *temp_v0;
    s32 temp_a1;

    temp_v0 = D_800B9620 - 1;
    temp_a1 = *temp_v0;
    D_800B9620 = temp_v0;
    if (temp_a1 != 0) {
        func_8001E848(&D_8009F8A4, temp_a1);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5410);

void func_800B544C(void) {
}

void func_800B5454(void) {
    func_80095BEC(0, D_800B8974);
    D_800B962C = 2;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5484);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B54B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B54F0);

void func_800B556C(void) {
    D_800569AC = *(s32 *)D_8009F644;
    func_80018B08(&D_800B8DD8, &D_800B9538);
    func_8001C2C0((u8 *)GetEngineSlot(0));
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B55B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5614);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B566C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5734);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B57BC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5880);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B58E4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5978);

void func_800B5A40(u8 *cam, s32 arg1) {
    if (arg1 != 0) {
        func_800B5978(cam, 1);
    }
    D_8009F708 = 0xF0;
}

void func_800B5A6C(void) {
}

void func_800B5A74(void) {
    func_800139A8();
}

void func_800B5A94(void) {
    u8 *g = D_8005A614;

    g[0x50] = 9;
    g[0x51] = 9;
    g[0x52] = 9;
    g[0x53] = 9;
    *(u16 *)(g + 0x32) |= 0x200;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5AC0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5B10);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B5CA8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B63F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B64B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B65D0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B6734);

void func_800B6A64(void) {
    EXTengine *eng;

    eng = GetEngineSlot(0);
    eng->unk10.x = -0x140;
    eng->unk10.y = -0xF0;
    func_800B566C();
    func_80018B08(&D_800B8DD8, &D_800B9F1C);
    D_800569AC = *(s32 *)D_8009F644;
    func_8001C2C0((u8 *)GetEngineSlot(2));
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B6ACC);

int func_800B6B88(int a0, int a1, int a2, int a3, int a4) {
    int v;

    if (a0 != a1) {
        if (a0 < a1) {
            if (a2 < 0) {
                a2 += a4;
            } else {
                a2 += a3;
            }
        } else if (a2 > 0) {
            a2 -= a4;
        } else {
            a2 -= a3;
        }
    }
    v = a2;
    if (a2 < 0) {
        v = -a2;
    }
    if (a3 >= v) {
        v = a1 - a0;
        if (v < 0) {
            v = -v;
        }
        if (a3 >= v) {
            a2 = 0;
        }
    }
    return a2;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B6C04);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B6CF4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B6E0C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7458);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B79A8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7B30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7BE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7C68);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7CE8);

void func_800B7D20(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7D28);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7D98);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7DC0);

s32 func_800B7DE8(s32 arg0) {
    return D_800BA324[arg0].unk4;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7E04);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7E54);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7E7C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7EA4);

s32 func_800B7EE4(s32 idx, u8 *base) {
    return (base + idx)[0xE4] != 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B7EF4);

void func_800B81F0(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    func_800B5880();
    if (D_800BAAC8 == 0) {
        if (D_80051380 & 0x80) {
            temp_v0 = D_800BAACC - 1;
            D_800BAACC = temp_v0;
            if (temp_v0 < 0) {
                D_800BAACC = 1;
            }
            PlaySfx(0x385, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (D_80051380 & 0x20) {
            temp_v0_2 = D_800BAACC + 1;
            D_800BAACC = temp_v0_2;
            if (temp_v0_2 >= 2) {
                D_800BAACC = 0;
            }
            PlaySfx(0x385, 0, 0, 0x1000, 0x1E00, 0);
        }
        func_800B7EF4();
        if (D_80051380 & 0x4000) {
            func_800B5410();
            PlaySfx(0x192, 0, 0, 0x1000, 0x1E00, 0);
        }
    }
    if ((D_80051380 & 0x10) && (D_800BAAC8 > 0)) {
        D_800BAAC8 -= 1;
    }
    if ((D_80051380 & 0x40) && (D_800BAAC8 < 0)) {
        D_800BAAC8 += 1;
    }
    if (D_80051380 & 0x1000) {
        PlaySfx(0x384, 0, 0, 0x1000, 0x1E00, 0);
        func_800B53D4();
    }
}

void func_800B83A4(void) {
    func_80095BEC(0, D_800BA754);
    D_800BAAC8 = 0;
    D_800BAACC = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl69/ovl69", func_800B83D8);

void func_800B8470(void) {
    func_800917B4();
}

void func_800B8490(void) {
}

void func_800B8498(void) {
    func_8009246C();
}

void func_800B84B8(void) {
    func_8001C2C0(GetEngineSlot(2));
}

void func_800B84E0(void) {
    func_80095BEC(2, D_800BAAD0);
}
