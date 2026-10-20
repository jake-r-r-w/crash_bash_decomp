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

s32 func_800B7558(EXTengine *eng) {
    s32 y;
    s32 ix;
    s32 iz;
    Ovl5eTile *t;

    y = eng->unk10.y - D_8005A5D8;
    ix = (eng->unk10.x + 0x700) >> 9;
    iz = (eng->unk10.z + 0x700) >> 9;
    if ((u32)ix >= 7U || (u32)iz >= 7U) {
        return y - 0x800;
    }
    t = D_800BD948[ix][iz].tile;
    if (t != NULL) {
        y -= t->unk8;
    }
    return func_80025E00(eng, y);
}

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

s32 func_800B794C(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (D_800BD70C[i].unk0 == 0) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", PlotPapuColumn);

void InitPapuColumn(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = *(GfxModel **)D_80058AD0;
        e->sub->unk74 = 0x200C;
        e->unk5D = 0;
        e->unk5C = 1;
    }
    nas->UserInfo.user1 = 1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800B7B10(void) {
    s32 i;
    PapuEntry *p;
    s16 one;

    i = 0;
    one = 1;
    for (p = D_800BE648; i < 20; i++, p++) {
        if (p->unk0 != 0) {
            p->unk6 = one;
        }
    }
}

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

void func_800B7DE4(void) {
    D_8005AA70[0] = (s32)func_800B35AC;
    D_8005AA70[1] = (s32)func_800B36D4;
    D_8005AA70[2] = (s32)func_800B36D4;
    D_8005AA70[3] = (s32)func_800B7DA4;
    D_8005AA70[5] = (s32)func_800B7D94;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[9] = (s32)func_800B7558;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7E38);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7F14);

s32 func_800B8080(T_B8080 *p) {
    s32 a, b, c, d;
    b = (p->x >> 8) + 8;
    a = 7 - (p->y >> 8);
    c = (s8)(p->h >> 8);
    d = c << 16;
    if (!(((u16)p->h) >> 16)) {
        return c;
    }
    return a + b + d;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B80B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B82DC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B8508);
