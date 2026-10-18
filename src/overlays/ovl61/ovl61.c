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

s32 func_800B4EFC(Ovl61Obj *arg0) {
    Ovl61Circle *base = D_800C1A7C;
    Ovl61Circle *c;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 x;
    s32 lim;
    s32 z;

    for (i = 0; i < 4; i++) {
        c = &base[i];
        if (arg0->flags & 0x40) {
            r = c->r + arg0->radius;
        } else {
            r = c->r + 0x80;
        }
        dx = c->x;
        x = arg0->x;
        dx = x - dx;
        z = arg0->z;
        dz = z - c->z;
        if (dx * dx + dz * dz < r * r) {
            return 1;
        }
    }
    lim = 0x900;
    if (x > lim || x < -lim) {
        return 1;
    }
    if (z > lim) {
        return 1;
    }
    return z < -lim;
}

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

s32 func_800B84B8(s32 arg0) {
    s32 sum;
    s32 i;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

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

s32 func_800BFD14(Ovl61Vel *v) {
    return v->pad14 - ((D_8005A5D8 - (v->z * D_80068BD4[(D_800C1C68 - 0x400) & 0xFFF].unk2 >> 12)) +
                       (v->x * D_80068BD4[(D_800C1C6C - 0x400) & 0xFFF].unk2 >> 12));
}

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

void Init_ArkCameraFlash(NAS *arg0) {
    u8 *eng = (u8*)(*(s32*)((u8*)(arg0) + 84));
    u8 *sub;
    if (eng != NULL) {
        s32 val;
        sub = (u8*)(*(s32*)(eng + 108));
        val = *(s32*)D_80058AD0;
        (*(u16*)(sub + 116)) = 0x2017;
        (*(u8*)(sub + 80)) = 1;
        (*(u8*)(sub + 81)) = 0;
        (*(s32*)(sub + 108)) = val;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS(arg0);
    }
}

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

void func_800C02F8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800B62F4();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x1D);
    h->hero_weapon_timer = 0x96;
    h->hero_attack_counter = 0;
    PlaySfx(D_800C18E8[arg0], &h->hero_eng->sfx, NULL, 0x1000, 0x1E00, 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C039C);

void func_800C0448(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_frozen = 5;
    if (h->hero_hit != 0) {
        h->hero_hit--;
    } else {
        SetHeroAction((HeroInfo *)arg0, 0x25);
    }
    func_8008502C(arg0);
}

void func_800C04B8(s32 arg0) {
    Heroes[arg0].hero_frozen = 5;
    func_8008502C();
}

void func_800C04FC(s32 arg0) {
    func_800B62F4();
    SetHeroSeq(arg0, 0x1A);
}

void func_800C052C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    func_800B62F4(arg0);
    SetHeroSeq((HeroInfo *)arg0, 0x19);
    h->hero_idle_count = Random(0x96) + 0xB4;
}

s32 func_800C05B8(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x22;
    if (!(st & 0x200)) {
        act = 0x23;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_800C0624(s32 arg0) {
    HeroInfo *base = Heroes;
    u16 st = base[arg0].hero_status;
    s32 act;

    if (st & 0x200) {
        act = 0x22;
    } else if (st & 0x2000) {
        act = 0x24;
    } else if (st & 0x400) {
        act = 0x23;
    } else {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

void func_800C069C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *shadow = h->hero_shadow;

    shadow->unk10 = h->hero_eng->unk10;
    shadow->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(shadow);
    shadow->unk1E = h->hero_eng->unk1E;
    shadow->unk5E = 1;
    if (h->hero_status & 0xF800) {
        shadow->sub->flags &= ~0x8000;
    }
}

extern char D_8005453C[];

void func_800C0760(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    s->sub->unk6C = D_8005453C;
    s->sub->unk74 = 0x2002;
    s->unk10 = h->hero_eng->unk10;
    s->unk5D = 0;
    s->flags |= 0x8041;
    s->sub->unk76 = 0x200;
    s->sub->flags |= 0x42000000;
    s->sub->unk68 = -0x10;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800C0804);

s32 func_800C08C8(Ovl61Obj *arg0) {
    Ovl61Circle *base = D_800C1A7C;
    Ovl61Circle *c;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;

    for (i = 0; i < 4; i++) {
        c = &base[i];
        if (arg0->flags & 0x40) {
            r = c->r + arg0->radius;
        } else {
            r = c->r + 0x80;
        }
        dx = arg0->x - c->x;
        dz = arg0->z - c->z;
        if (dx * dx + dz * dz < r * r) {
            return 1;
        }
    }
    return 0;
}

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

s32 func_800C0A74(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2) {
    s32 lim;
    s32 x;
    s32 z;
    s32 d;

    lim = 0xA00;
    if (arg2 != 0) {
        lim = 0xB80;
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
            if (x > 0x600) {
                arg0->x = 0x600;
            }
            if (arg0->x < -0x600) {
                arg0->x = -0x600;
            }
        } else {
            if (z > 0x600) {
                arg0->z = 0x600;
            }
            if (arg0->z < -0x600) {
                arg0->z = -0x600;
            }
        }
    }
    return 0;
}

void func_800C0B7C(void) {
}

void func_800C0B84(void) {
}

void func_800C0B8C(void) {
}

void func_800C0B94(void) {
    D_8005AA70[0] = (s32)func_800B36CC;
    D_8005AA70[1] = (s32)func_800B3B20;
    D_8005AA70[2] = (s32)func_800B3B20;
    D_8005AA70[3] = (s32)func_800B40AC;
    D_8005AA70[6] = (s32)func_800C0B7C;
    D_8005AA70[5] = (s32)func_800C0B84;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[9] = (s32)func_800BFD9C;
    D_8005AA70[10] = (s32)func_800B9728;
}
