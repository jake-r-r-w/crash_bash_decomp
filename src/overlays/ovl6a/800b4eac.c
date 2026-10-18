#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/ovl6a.h"
#include "bash/ovl6a_800b4eac.h"
#include "bash/hub.h"
#include "bash/n2o.h"
#include "bash/titles.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B4EAC);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B330C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B5158);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B551C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B5710);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B6224);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B64C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B694C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B6A3C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B6E20);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B3618);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B6FD4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B77B0);

void func_800B7874(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B787C);

void func_800B7988(s32 *arg0, s32 flag, s32 *out) {
    s32 *p = arg0;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 *q;

    if (p[5] & 0x2000) {
        p -= 7;
    }
    dx = p[0] - p[7];
    q = p + 7;
    dz = p[2] - q[2];
    dy = p[1] - q[1];
    dist = csqrt(dx * dx + dz * dz) >> 6;
    out[1] = get_angle(dx, dz) + (flag << 11);
    if (flag != 0) {
        out[0] = get_angle(dy, dist);
        return;
    }
    out[0] = -get_angle(dy, dist);
}

void func_800B7A78(void) {
    D_800BCEE8 = func_8001DCD4(1, 0x14);
    D_800BCED8 = 0;
    D_800BCEE0 = -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B7AB4);

u16 func_800B7B38(void) {
    if (D_8005A677 >= 0) {
        return D_800BCCF4[D_8005A677].unk14->unk2;
    }
    return 0;
}

void func_800B7B7C(void) {
    func_80095A04(1);
    func_80095A04(2);
}

void func_800B7BA4(void) {
    EXTengine *e;

    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = *(s32 *)D_8009F644;
    if (D_800BCED4 >= 0) {
        if (!(D_8005A624 & 1)) {
            e = GetEngineSlot(1);
            e->flags |= 0x8000;
            e->unk10.y = 0x1D6 - (D_800BF260 >> 5);
            func_8001C2C0((u8 *)GetEngineSlot(1));
        }
    }
    if (D_800BF260 != 0 && !(D_8005A624 & 1)) {
        func_8001C2C0((u8 *)GetEngineSlot(2));
    }
}

void func_800B7C78(void) {
    func_800B64C8();
}

void func_800B7C98(void) {
    D_800BCED4 = -1;
}

void func_800B7CA8(void) {
    EXTengine *e;

    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = *(s32 *)D_8009F644;
    if (D_800BCED4 >= 0) {
        if (!(D_8005A624 & 1)) {
            e = GetEngineSlot(1);
            e->flags |= 0x8000;
            e->unk10.y = 0x1D6 - (D_800BF260 >> 5);
            func_8001C2C0((u8 *)GetEngineSlot(1));
        }
    }
    if (D_800BF260 != 0 && !(D_8005A624 & 1)) {
        func_8001C2C0((u8 *)GetEngineSlot(2));
    }
}

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

void PlotGatewayTrophy(void) {
}

void InitGatewayTrophy(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B7E14);

s32 func_800B7F24(DistPos *a, DistPos *b) {
    s32 dx = b->x - a->x;
    s32 dz = b->z - a->z;
    return csqrt(dx * dx + dz * dz) >> 6;
}

s32 func_800B7F74(u8 *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = (*(s32*)((u8*)(arg1) + 16)) - (*(s32*)((u8*)(arg0) + 16));
    temp_v0_2 = (*(s32*)((u8*)(arg1) + 24)) - (*(s32*)((u8*)(arg0) + 24));
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
}

void func_800B7FAC(s32 arg0) {
    u8 *p;
    D_8009E5DC = arg0;
    p = D_8005A614;
    *(u16 *)(p + 0x30) = 0x8008;
    *(u16 *)(p + 0x24) = 0xA;
}

void func_800B7FD0(u8 *cam, s32 arg1) {
    u8 *o = (u8 *)func_8001E48C(arg1);
    s32 *p;
    s32 t;

    if (o != NULL) {
        *(s16 *)(cam + 0x54) = (get_angle(*(s32 *)(o + 0x58) - *(s32 *)(o + 0x3C),
                                          *(s32 *)(o + 0x50) - *(s32 *)(o + 0x34)) - 0x400) & 0xFFF;
        *(s16 *)(cam + 0x56) = 0;
        *(s16 *)(cam + 0x58) = 0;
        *(s32 *)(cam + 0x18) = (((s32)(*(u16 *)(o + 0x40) << 16)) >> 18) + 0x200;
        do {
            *(s32 *)(cam + 0xC) = *(s32 *)(o + 0x30);
        } while (0);
        p = (s32 *)(o + 0x30);
        t = p[1];
        *(s32 *)(cam + 0x10) = t;
        t = p[2];
        *(s32 *)(cam + 0x14) = t;
        *(s32 *)(cam + 4) = -p[1];
        *(s32 *)cam = 0;
        *(s32 *)(cam + 8) = *(s32 *)(o + 0x58);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B8098);

void func_800B8148(s32 arg0) {
    Heroes[arg0].hero_idle_count = 0xF;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B8174);

s32 func_800B821C(void) {
    return 0;
}

s32 func_800B8224(void) {
    return 0;
}

void func_800B822C(s32 arg0) {
    if (Heroes[arg0].hero->UserInfo.user1 != 0) {
        D_800BCEF4 += 1;
        return;
    }
    D_800BCEF4 = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B8284);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B8324);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B838C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B83D4);

void func_800B8474(s32 arg0) {
    u8 *o;

    func_800B83D4();
    o = (u8 *)func_8001E48C(2);
    if (o != NULL) {
        D_8009EE9C.unkC = *(s32 *)(o + 0x58);
        D_8009EE9C.unk10 = *(s32 *)(o + 0x74);
    }
    func_800B5710(arg0);
}

s32 func_800B84CC(void) {
    EngRec90248 *p;
    s32 i;
    s32 best;
    s32 bestDist;
    s32 d;

    best = -1;
    bestDist = 0x1000;
    for (i = 0; i < 12; i++) {
        p = &D_800BCCF4[i];
        if (p->unk14 != NULL && (p->unk14->unk6 & 2)) {
            d = func_80015480(p->unk10, D_8009EE9C.unk4);
            if (d < bestDist) {
                bestDist = d;
                if ((u32)d < 0x81U) {
                    best = i;
                }
            }
        }
    }
    return best;
}

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B85E4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B86AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B8708);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B37EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B875C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B88B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B8AA4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B8BC8);

void func_800B8CA0(void) {
    s32 r;
    s32 x;

    if (D_8005A624 == 0 && D_800BD390-- < 0) {
        D_800BD394 = Random(4);
        r = Random(6);
        if (r == 0) {
            PlaySfx(0x355, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x356, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x357, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 3) {
            PlaySfx(0x358, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 4) {
            PlaySfx(0x359, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 5) {
            PlaySfx(0x35A, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        r = Random(6);
        if (r == 0) {
            PlaySfx(0x35B, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x35C, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x35D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 3) {
            PlaySfx(0x35E, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 4) {
            PlaySfx(0x35F, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 5) {
            PlaySfx(0x360, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        x = Random(10000) - 5000;
        SpawnNASById(x, Random(10000) - 5000, 0xE10, 0x1628, 0x11);
        D_800BD390 = Random(10) + 5;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B8F50);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", PlotPodiumOdo);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B93B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", PlotPodiumTrophy);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B96F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B9B0C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B9D1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B9E90);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BA1B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BA374);

void func_800BA7BC(NAS *arg0) {
    if (D_8005A624 == 0) {
        if (arg0->UserInfo.user1++ >= 0x1D) {
            KillNAS(arg0);
        }
    }
}

void func_800BA804(void) {
    s32 temp_a0;
    EXTengine *eng;

    eng = GetEngineSlot(6);
    if (eng != NULL) {
        temp_a0 = eng->unk10.y;
        if (temp_a0 < 0x21C) {
            eng->unk10.y = temp_a0 + 5;
        }
    }
}

void func_800BA848(void) {
    s32 temp_a0;
    EXTengine *eng;

    eng = GetEngineSlot(6);
    if (eng != NULL) {
        temp_a0 = eng->unk10.y;
        if (temp_a0 >= 0x155) {
            eng->unk10.y = temp_a0 - 5;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BA88C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", InitPodiumTrophy);

void InitPodiumOdo(u8 *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = 0;
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s16*)((u8*)(arg0) + 94)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS((NAS *)arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BA9C8);

void func_800BAAFC(void) {
    func_8009440C();
    func_80018B08(D_8009F67C, D_8009F734);
    func_8001C2C0(GetEngineSlot(6));
}

void func_800BAB40(void) {
    s32 i;
    s32 any = 0;

    for (i = 0; i < 4; i++) {
        if (((PadButtons *)D_80051338)[i].unk54 & 0x4000) {
            any = 1;
        }
    }
    if (any != 0 && D_800BD5B4 != 0 && D_800BD5B0 < 4U) {
        D_800BD5B0 = 4;
    }
    if (D_800BD5B0 == 0x64) {
        ((Ovl6aGameView *)D_8005A614)->unk24 = 0;
        ((Ovl6aGameView *)D_8005A614)->unk30 = 0x8002;
    }
    func_800B8F50();
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B3894);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BABEC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BB038);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BB280);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BB3D0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BB4AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BB584);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BB5F0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BBC64);

void func_800BBE1C(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BBE24);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BBF44);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B3928);
