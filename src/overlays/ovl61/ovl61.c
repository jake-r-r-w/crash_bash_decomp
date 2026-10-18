#include "common.h"
#include "bash/main.h"
#include "bash/ovl61.h"
#include "bash/arkenoid.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B3528);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B36CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B3B20);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B40AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B42F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B453C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4880);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4AE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4D9C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4EFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4FC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B5238);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B558C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B58E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B5C30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B5EC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B60FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B62F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B649C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B66D0);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl61/ovl61", D_800B32B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B68C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B69E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B6AE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B6BE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B6CF0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B6FE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B70CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7450);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7518);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7688);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B77F8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B79F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7B60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7C60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B808C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B84B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B85DC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B8964);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl61/ovl61", D_800B3300);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B8A68);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B9728);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B97D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B99E0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", InitBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", PlotBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", CalcArkInfluence);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", FreeGrabbedObject);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", AlignGrabBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Plot_N_Gin);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BC0D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BC564);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BCA30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_ArkDeadWall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_NGArkDeadWall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BD43C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BD510);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BD770);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Plot_Ark_Pickup);

void func_800BDD30(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_pu_timer != 0) {
        h->hero_pu_timer--;
    }
    if (h->hero_pu_action_timer != 0) {
        h->hero_pu_action_timer--;
    }
    if (h->hero_pu_lock != 0) {
        h->hero_pu_lock--;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Plot_RepulseRing);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_ArkLaserWall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BE1C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BE4D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", ArkHeroRad2ObjectCollision);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", ArkHeroRad2ObjectRepel);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", ArkHeroRad2ObjectAttract);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BF640);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BFA24);

void Plot_ArkLaserWall(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    if (D_8005A644 & 0x801) {
        D_800C1F14 &= ~nas->UserInfo.flags;
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    if (eng != NULL) {
        if (D_800C1C70 == 0) {
            D_800C1F14 &= ~nas->UserInfo.flags;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x5B8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            KillNAS(nas);
            return;
        }
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_RepulseRing);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BFD14);

s32 func_800BFD9C(u8 *arg0) {
    return (*(s32*)((u8*)(arg0) + 20)) - D_8005A5D8;
}

void Plot_ArkDeadWall(NAS *arg0) {
    EXTengine *eng;
    subengine *sub;

    if (D_800C1C88 == 0) {
        D_800C1F14 &= ~arg0->UserInfo.flags;
        KillNAS(arg0);
        return;
    }
    eng = arg0->Engine;
    if (eng != NULL) {
        if (arg0->UserInfo.user1 != 0) {
            arg0->UserInfo.user1 = arg0->UserInfo.user1 - 1;
        } else {
            eng->flags |= 0x8000;
        }
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub->unk74 = (D_8004E0D8 & 7) + 0x2001;
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Plot_ArkCameraFlash);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_ArkCameraFlash);

void Plot_seaweed(NAS *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        func_800250B0(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_seaweed);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_N_Gin);

u8 *func_800C01B8(void) {
    s32 var_v0;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *var_v1;

    var_v1 = D_8005AA38;
    var_v0 = 0;
    if (var_v1 != NULL) {
        do {
            var_v1 = (*(s32*)((u8*)(var_v1) + 32));
            var_v0 += 1;
        } while (var_v1 != NULL);
    }
    if (var_v0 >= 9) {
        return NULL;
    }
    temp_v0 = SpawnNASById(0x1000, 0x1000, 0x1000, 0x1501, 0x11);
    if ((temp_v0 != NULL) && ((*(s32*)((u8*)(temp_v0) + 84)) != 0)) {
        temp_v0_2 = (*(s32*)((u8*)(temp_v0) + 88));
        if (temp_v0_2 != NULL) {
            (*(s16*)((u8*)(temp_v0_2) + 26)) = 0;
        }
    }
    return temp_v0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C0250);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C02F8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C039C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C0448);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C04B8);

void func_800C04FC(s32 arg0) {
    func_800B62F4();
    SetHeroSeq(arg0, 0x1A);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C052C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C05B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C0624);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C069C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C0760);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C0804);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C08C8);

s32 func_800C096C(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2) {
    s32 lim;
    s32 x;
    s32 z;
    s32 d;

    lim = 0xA00;
    if (arg2 != 0) {
        lim = 0x1000;
    }
    x = arg0->x;
    if (x > lim) {
        arg1->dir = D_800C271C;
        return 1;
    }
    if (x < -lim) {
        arg1->dir = D_800C2720;
        return 1;
    }
    z = arg0->z;
    if (z > lim) {
        arg1->dir = D_800C2724;
        return 1;
    }
    if (z < -lim) {
        arg1->dir = D_800C2728;
        return 1;
    }
    if (arg2 != 0) {
        d = arg1->dir;
        if (d == D_800C2724 || d == D_800C2728) {
            if (x > 0x680) {
                arg0->x = 0x680;
            }
            if (arg0->x < -0x680) {
                arg0->x = -0x680;
            }
        } else {
            if (z > 0x680) {
                arg0->z = 0x680;
            }
            if (arg0->z < -0x680) {
                arg0->z = -0x680;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C0A74);

void func_800C0B7C(void) {
}

void func_800C0B84(void) {
}

void func_800C0B8C(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C0B94);
