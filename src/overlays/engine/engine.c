#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/hero.h"
#include "bash/shell.h"

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80078CA4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80078DD4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007906C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80079260);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800793E8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80079554);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007976C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800798A4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A164);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A26C);

NAS *func_8007A464(s32 arg0, s32 *arg1, s32 arg2) {
    NAS *nas;
    s32 *ext;

    nas = SpawnNASById(arg1[0], arg1[1], arg1[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = arg0;
            ext[6] = 0x100;
            if (arg2 != 0) {
                ext[8] = 4;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = arg2;
            } else {
                ext[8] = 0;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A524);

s32 func_8007A5E8(u8 *arg0) {
    s32 sum = 0;
    s32 i = 0;
    do {
        sum += (arg0 + i)[0xE4];
        i++;
    } while (i < 0x21);
    return sum;
}

void func_8007A614(s32 arg0) {
    u8 *p = D_8005A614;
    func_8007A164(D_800986FC[arg0 & 0xFFFF], p);
}

void func_8007A650(s32 arg0) {
    func_8007A164(D_800986FC[arg0 & 0xFFFF]);
}

void func_8007A688(s32 arg0) {
    func_8007A164(arg0 & 0xFFFF, D_8005A614);
}

void func_8007A6B0(s32 arg0) {
    func_8007A164(arg0 & 0xFFFF);
}

void func_8007A6D0(s32 arg0) {
    func_8007A164(arg0 & 0xFFFF, D_8005A614);
}

void func_8007A6F8(void) {
    func_8002617C();
}

void func_8007A718(void) {
    NAS *nas;
    EXTengine *eng;

    for (nas = NASActiveHead; nas != NULL; nas = nas->Tag.Child) {
        eng = nas->Engine;
        if (eng != NULL && (nas->NASid & 1)) {
            do {
                if (!(nas->Class & 0x2000)) {
                    eng->OldXYZ = eng->unk10;
                }
                func_8001C298(eng);
                eng = eng->Engine;
            } while (eng != NULL);
        }
    }
}

void func_8007A7C0(void) {
    func_80011410(&D_8004E0F0);
    func_80079554();
    D_8005AA58 = 4;
    func_8007AF3C();
    D_8005AA58 = 0;
    func_80081F64(0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A808);

void func_8007A8C4(void) {
    EngineGameView *g = (EngineGameView *)D_8005A614;

    g->flags &= 0xF4FF;
    func_80083550();
    func_8008688C();
    if (D_8005AA70[0] != 0) {
        ((void (*)(void))D_8005AA70[0])();
    }
}

void func_8007A91C(void) {
    if (D_8009871C != 0) {
        D_8009871C = D_8009871C - 1;
    } else {
        u8 *p = D_80098718;
        p += Random(8) * 2;
        PlaySfx(*(u16*)(p + 8), 0, 0, 0x1000, 0x1E00, 0);
        D_8009871C = (*(u16*)((u8*)D_80098718) + Random(*(u16*)((u8*)D_80098718 + 2) - *(u16*)((u8*)D_80098718))) * 30;
    }
}

void func_8007A9D0(u8 *arg0) {
    D_80098718 = arg0;
    D_8009871C = 0;
    PlaySfx((*(u16*)((u8*)(arg0) + 4)), 0, 0, 0x1000, 0x1E00, 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007AA18);

void func_8007AAF0(void) {
    D_8005AA58 = 4;
    func_8007AF3C();
    D_8005AA58 = 0;
}

void func_8007AB20(void) {
}

void func_8007AB28(void) {
    D_8005AA58 = 0;
    if (D_8005AA80 != NULL) {
        D_8005AA80();
    }
    func_80086814();
    func_80026FCC();
}

void func_8007AB6C(void) {
    func_8007A6F8();
}

void func_8007AB8C(void) {
    D_8005AA58 = 0;
    ASTTable = EngineASTTable;
    ASTCount = func_8007AC00();
}

void func_8007ABC4(void) {
    D_8005AA58 = 0;
    D_8005AA48 = 0;
    func_80024AD8();
    func_80026ECC();
    func_80026D9C();
}

int func_8007AC00(void) {
    unsigned int i;

    for (i = 0; i < 0xAE; i++) {
    }
    return i;
}
