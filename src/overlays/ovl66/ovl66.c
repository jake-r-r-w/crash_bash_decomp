#include "common.h"
#include "bash/main.h"
#include "bash/ovl66.h"
#include "bash/bonk.h"
#include "bash/pogo.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

void func_800B33A4() {
    D_8009D6E8.unk4 = D_800B9DF8;
    D_8009D6E8.unk2C = func_800B70C4;
    D_8009D6E8.unk68 = func_800B9054;
    D_8009D6E8.unk20 = func_800B717C;
    D_8009D6E8.unk28 = func_800B8EF0;
    D_8009D6E8.unk54 = func_800B7B48;
    D_8009D6E8.unk3C = func_800B8E64;
    D_8009D6E8.unk48 = D_8008AB48;
    D_8009D6E8.unk44 = D_8008AB04;
    D_8009D6E8.unk5C = func_800B6E1C;
    D_8009D6E8.unk64 = func_800B6238;
    D_8009D6E8.unk38 = func_800B5BA0;
    D_8009E0D8 = 0x42;
    D_8009E0E4 = 0x47;
    D_8009E0E6 = 0x48;
    func_800B67F4();
}

void func_800B347C(void) {
    s32 i;

    D_800B9EF0 = 0;
    D_800B9EFC = 0;
    D_800B9F04 = 0;
    D_800B9F00 = D_800B9E0C;
    for (i = 0; i < 10; i++) {
        D_800B9E50[i].unk0 = 0x100;
        D_800B9E50[i].unk4 = 0;
        D_800B9E50[i].unk8 = 0;
    }
    D_800B9EF4 = 0;
    D_800B9EF8 = D_800B9634 * 30;
}

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

int func_800B61B0(int a0, int *out)
{
    int v;

    if (func_800B4454(a0, &v, 0x301) != 0) {
        *out = v;
        return 1;
    }
    return 0;
}

void func_800B61F8(s32 arg0) {
    EXTbotai *ai = Heroes[arg0].hero_ai;

    if (ai != NULL) {
        ai->AImode = 1;
        ai->AItimer1 = 0;
    }
}

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

void InitBonkFlag(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        func_80024E30(nas, &D_800B9CE4);
        e->sub->unk6C = D_80053C0C;
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBonkShockwave(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 v;

    if (e != NULL) {
        s32 a = nas->UserInfo.user1;
        s32 b = nas->UserInfo.user2;

        if (a >= b) {
            KillNAS(nas);
        } else {
            s32 t = a - 0x800;

            nas->UserInfo.user1 = a + ((b - t) >> 1);
        }
        e->unk1E += 0x71;
        v = nas->UserInfo.user1;
        e->unk5E = 1;
        e->unk2C.z = v;
        e->unk2C.x = v;
    }
}

void InitBonkShockwave(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053C7C;
        e->sub->unk74 = 0x2014;
        e->unk5C = 1;
        e->unk2C.y = 0x1000;
        e->unk5D = 0;
        e->unk2C.z = 0;
        e->unk2C.x = 0;
        e->unk5E = 1;
        e->flags |= 0x8021;
        nas->UserInfo.user1 = 0;
        nas->UserInfo.user2 = 0x4000;
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800B64B4(s32 arg0, s32 arg1, Ovl66Obj *obj) {
    Ovl66Pad *p = obj->pad;

    if (p != NULL) {
        func_800B6CD8(arg0, Random(2));
        p->mode = 4;
        PlaySfx(D_800B9E40[Random(4)], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void func_800B6560(void) {
}

void func_800B6568(void) {
    s32 i;

    D_800B9F08 = 0;
    for (i = 0; i < 3; i++) {
        D_800B9EC8[i].a = 0x200;
        D_800B9EC8[i].b = 0;
        D_800B9EC8[i].c = 0;
    }
    for (i = 0; i < 2; i++) {
        SpawnNASById(0, 0, 0, 0x1A04, 0x11);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl66/ovl66", InitBonkStool);

void func_800B66C8(void) {
    NAS *n;

    if (D_800B9EF0 < 10) {
        n = SpawnNASById(0, D_8005A5D8 - 0x2000, 0, 0x1A01, 0x11);
        if (n != NULL) {
            n->UserInfo.user1 = Random(10) + D_800B9EF0 * 5;
            n->UserInfo.user2 = D_800B9EF0;
            D_800B9E50[D_800B9EF0].unk4 = (s32)n;
            D_800B9EF0 += 1;
        }
    }
}

void func_800B6774(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_8005A614[i + 0x5C] != 0) {
            D_800B9EEC = D_8009D87C[i][0];
        }
    }
}

void func_800B67C4(void) {
    func_800B34F8();
    func_800B6560();
    func_800B6774();
}

void func_800B67F4(void) {
    D_8009D904 = D_8005A680[0] != 0 ? D_800B92E8 : D_800B92D0;
    D_8009D8FC = D_8005A680[0] != 0 ? D_800B92B0 : D_800B929C;
    D_8009D900 = D_800B92C4;
}

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

void func_800B68EC(void) {
    s32 i;
    s16 v;
    u8 *g = D_8005A614;

    u8 *h;

    if (*(u16 *)(g + 0x32) & 0x40) {
        i = 0;
        h = g;
        for (; i < 4; i++) {
            v = 0x3C;
            if (h[i + 0x5C] == 0) {
                v = 0x50;
                if (*(s32 *)(h + 0x6C) != 0) {
                    v = 0x5A;
                }
            }
            Heroes[i].hero_score_displayed = v;
            Heroes[i].hero_score = v;
        }
        func_800B6868(0x130);
        func_8007D144();
        if (D_8005A680[0] != 0) {
            D_80099174 |= 0x2000;
        }
    }
}

void func_800B69A0(void) {
    if (D_8005A646 & 0x80) {
        func_800B6868(0x130);
    }
}

void func_800B69D0(void) {
    Ovl66Cam *cam = (Ovl66Cam *)D_80051640;
    cam->unk0 = 0;
    cam->unk8 = 0xE98;
    cam->unk54 = 0x192;
    cam->unk18 = 0x200;
    cam->unkC = 0x60;
    cam->unk10 = 0x20;
    cam->unk4 = 0;
    cam->unk56 = 0;
    cam->unk58 = 0;
    cam->unk14 = -0x660;
}

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
