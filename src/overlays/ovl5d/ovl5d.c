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

void func_800B6F0C(void) {
    HeroInfo *h;
    s32 i;
    s32 best;
    s32 max;

    max = 0;
    best = -1;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (!(h->hero_status & 0xFA00) && max < h->hero_health) {
            max = h->hero_health;
            best = i;
        }
    }
    if (best != -1) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00) && i != best) {
                func_80084E04(i, 0, -1, 0x1000, 0);
            }
        }
    }
}

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

void InitMissileExplode(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        sub->unk6C = &D_80052F5C;
        sub->unk74 = 0x2010;
        sub->unk50 = 1;
        sub->unk51 = 0;
        eng->flags |= 0x21;
        nas->UserInfo.flags = 0;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BA8A4);

void PlotBoing(u8 *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        func_800250B0((NAS *)arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitBoing);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitBashPenguin);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitSnowGlint);

void func_800BAC10(void) {
    u8 *vp = D_8009F6F4;
    func_80093038();
    func_80018B08((GfxViewport *)vp, (GfxCamera *)D_80051640);
    D_800569AC = *(s32 *)D_8009F644;
    func_8009440C();
}

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

void func_800BB2BC(void) {
    s32 i;
    EXTheromove *m;
    for (i = 0; i < 4; i++) {
        m = Heroes[i].hero_move;
        if (m != NULL) {
            m->z_acc = 8;
            m->x_acc = 8;
            m->z_dec = 4;
            m->x_dec = 4;
            if ((OVLG->flags & 0x80) && (i == 0 || (OVLG->p2_active != 0 && i == 1))) {
                m->z_acc = 4;
                m->x_acc = 4;
                m->z_dec = 1;
                m->x_dec = 1;
            }
            m->velocity_calc = 3;
        }
    }
}

void func_800BB360(void) {
}

void func_800BB368(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BB370);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BB3D0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BB4AC);
