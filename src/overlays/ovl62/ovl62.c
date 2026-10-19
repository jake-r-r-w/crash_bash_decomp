#include "common.h"
#include "bash/main.h"
#include "bash/ovl62.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B3740);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B38A0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B3AA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B4094);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B4260);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl62/ovl62", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B44B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B4A1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitPolarBomb);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotPolarBomb);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotBombGFX);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B5570);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B5684);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B5788);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B5910);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl62/ovl62", D_800B3358);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B5AC4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B5EBC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B60B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B6258);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B649C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B6708);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B6D74);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B6EB4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B6FEC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B7224);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B7520);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B767C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B782C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B7A58);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B7B88);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B7CDC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B7F70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B889C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B90C0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B9398);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B94B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B9600);

s32 func_800B9714(s32 player) {
    Ovl62Fux *fux;
    Ovl62Fux *ext;
    Ovl62Fux *f;
    HeroInfo *h;
    SLONG_XYZ v;
    s32 r;
    s32 spd;

    fux = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return 0;
    }
    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL && ext->unk24 == 1) {
        return 0;
    }
    if (Random(0x1000) >= D_800D4E0C[Heroes[player].hero_name + 0x2F]) {
        return 0;
    }
    h = &Heroes[player];
    r = Random(0x1000);
    f = (Ovl62Fux *)h->hero->FUX;
    spd = 0x708;
    if (f != NULL) {
        v.x = (D_80068BD4[r & 0xFFF].unk2 * spd) >> 12;
        v.z = (D_80068BD4[(r - 0x400) & 0xFFF].unk2 * spd) >> 12;
        f->vel = v;
        f->pos = h->hero_eng->unk10;
    }
    fux->state = 9;
    fux->func = func_800B7520;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B98AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B9994);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B9B6C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B9D48);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BAFA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BB810);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BBF2C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BCC9C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BCF08);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BD058);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitTransmuterBeam);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotTransmuterBeam);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotWeightDeath);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BFD38);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BFEE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotScratchMaster);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitIceBerg);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C0888);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl62/ovl62", D_800B3498);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C0E94);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C0FE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitUkaPolarMask);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotUkaPolarMask);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C2ECC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitPolarWalrus);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotPolarWalrus);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitWalrusFish);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotWalrusFish);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotSmokeTrail);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitSimpleExplosion);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotDingoTail);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotBrioPack);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitPackEffect);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitPolarBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotPolarBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C5CD0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C5DCC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C5EEC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitBearminator);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotBearminator);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotPolarMissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotBigMissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C7ED4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitSniper);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotSniper);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotSniperMissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C9230);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitSniperMissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitBigMissile);

void PlotBossTransmuter(NAS *m) {
}

void InitBossTransmuter(NAS *m) {
}

s32 func_800C9B5C(void) {
    u8 *p;
    s32 n;

    n = 0;
    for (p = D_800D5454; p != NULL; p = *(u8 **)(p + 0x20)) {
        n++;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitPolarMissile);

s32 func_800C9C98(u8 *arg0) {
    HeroInfo *hero;
    u32 i;
    u32 n;

    n = 1;
    if (D_8005A680[0] != 0) {
        n = 2;
    }
    i = 0;
    hero = Heroes;
loop:
    if (i < n) {
        if (!(hero->hero_mode & 0x2000) &&
            func_8008CEB0(arg0, (u8 *)hero->hero_eng + 0x10) < 0x7BC4) {
            if (hero->hero_mode != 0x4B) {
                SetHeroAction((HeroInfo *)i, 0x4B);
            }
            PlaySfx(0x43D, NULL, NULL, 0x1000, 0x1E00, 0);
            return i;
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C9D74);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C9EC4);

void PlotBearCam(NAS *m) {
}

void InitBearCam(NAS *m) {
}

void PlotRockProjectile(NAS *m) {
}

void InitRockProjectile(NAS *m) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C9F70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotBearBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitBearBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotPackEffect);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotCortexPack);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitCortexPack);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitBrioPack);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitDingoTail);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotSimpleExplosion);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitSmokeTrail);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotBearDeath);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitBearDeath);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CA6E8);

void PlotBreathMaster(NAS *m) {
}

void InitBreathMaster(NAS *m) {
}

void PlotSnowMaster(NAS *m) {
}

void InitSnowMaster(NAS *m) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CA770);

void PlotIceBerg(NAS *arg0) {
    u8 *temp_a1;
    u8 *temp_v1;

    temp_a1 = (*(s32*)((u8*)(arg0) + 84));
    (*(s32*)((u8*)(temp_a1) + 16)) = (s32) ((*(s32*)((u8*)(temp_a1) + 16)) + ((s32) (*(s32*)((u8*)(arg0) + 100)) >> 8));
    temp_v1 = (*(s32*)((u8*)(arg0) + 84));
    if ((*(s32*)((u8*)(temp_v1) + 16)) >= 0x1AF5) {
        (*(s32*)((u8*)(temp_v1) + 16)) = -0x1C84;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitScratchMaster);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CA930);

void PlotGravityObject(GravityObject *obj) {
    s32 target = obj->target;

    if (target != 0) {
        func_800BFD38(obj, target);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitGravityObject);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitWeightDeath);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CAC20);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CACC0);

void func_800CAD0C(void) {
}

void func_800CAD14(void) {
}

void func_800CAD1C(void) {
    u8 *var_v1;

    var_v1 = (*(s32*)((u8*)(D_80058AD0) + 28));
    if (var_v1 != NULL) {
        do {
            (*(s32*)((u8*)(var_v1) + 0)) = (s32) ((*(s32*)((u8*)(var_v1) + 0)) | 0x01000000);
            var_v1 = (*(s32*)((u8*)(var_v1) + 92));
        } while (var_v1 != NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CAD60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CAE10);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CAEB0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CAF94);

s32 func_800CAFF0(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CAFF8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB0A4);

void func_800CB124(void) {
}

void func_800CB12C(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB134);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB178);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB284);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB2C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB314);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB368);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB41C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB508);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB56C);

NAS *func_800CB5F4(void) {
    NAS *nas;
    Ext10000000 *ext;

    nas = D_800D5294;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->unkC == 0) {
            return nas;
        }
        nas = nas->Group.Child;
        goto loop;
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CB650);

void func_800CB6B4(void) {
}

void func_800CB6BC(void) {
}

s32 func_800CB6C4(s32 arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (arg0 & 0x80) {
        var_a1 = 0xC00;
    }
    if (arg0 & 0x20) {
        var_a1 = 0x400;
    }
    if (arg0 & 0x10) {
        var_a1 = 0;
    }
    if (arg0 & 0x40) {
        var_a1 = 0x800;
    }
    if ((arg0 & 0x90) == 0x90) {
        var_a1 = 0xE00;
    }
    if ((arg0 & 0xC0) == 0xC0) {
        var_a1 = 0xA00;
    }
    if ((arg0 & 0x30) == 0x30) {
        var_a1 = 0x200;
    }
    if ((arg0 & 0x60) == 0x60) {
        var_a1 = 0x600;
    }
    return var_a1;
}

s32 func_800CB744(u8 *arg0, u8 *arg1) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a2;
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = (*(s32*)((u8*)(arg0) + 0));
    temp_a0 = (*(s32*)((u8*)(arg1) + 0));
    var_v1 = (temp_a0 < temp_v0) << 7;
    if (temp_v0 < temp_a0) {
        var_v1 |= 0x20;
    }
    temp_a2 = (*(s32*)((u8*)(arg0) + 8));
    temp_a0_2 = (*(s32*)((u8*)(arg1) + 8));
    if (temp_a0_2 < temp_a2) {
        var_v1 |= 0x40;
    }
    if (temp_a2 < temp_a0_2) {
        var_v1 |= 0x10;
    }
    return var_v1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitBombGFX);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotBombMaker);

void InitBombMaker(NAS *m) {
    BombMakerExt *ext;

    ext = AddExtension(m, 0x1C, 0x10000000);
    if (ext != NULL) {
        ext->timer = 0xB4;
        ext->unkC = 0;
        ext->unk18 = 0;
        D_800D52B4 = m;
    }
}

u32 func_800CBA28(u8 *arg0) {
    return (u32) ((get_angle((*(s32*)((u8*)(arg0) + 0)), (*(s32*)((u8*)(arg0) + 8))) - 0x7D) & 0xFFF) >> 8;
}

s32 func_800CBA5C(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CBA64);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CBAA4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CBB8C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CBBE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CC024);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CC2BC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CC3B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CCCB8);

s32 func_800CD118(s32 *arg0, s32 arg1) {
    s32 v[3];
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 ang;

    base = *(u8 **)((u8 *)D_80058AD0 + 0x10);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        n = *(s32 *)(p + 0x38);
        t = D_80068BD4;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = *(s32 *)(a3 + 0x64);
            v[0] = arg0[0] - *(s32 *)(a2 - 8);
            v[2] = arg0[2] - *(s32 *)a2;
            ang = -ang;
            a3 += 0x38;
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                return 0;
            }
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CD214);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CD560);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CD594);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CDA54);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CDBD0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CDE8C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CE664);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CED58);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CF6F8);

void func_800CF9EC(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;

    if (func_800D2D2C() == 0) {
        base = Heroes;
        hero = base + player;
        if (hero->hero_move != NULL) {
            hero->hero_move->z_dir = 0;
        }
        hero->hero_eng->sub->unk68 = 0;
        if (hero->hero_name == 1) {
            hero->hero_eng->sub->unk74 = 0x2002;
        }
        if (hero->hero_name == 5) {
            hero->hero_eng->sub->unk74 = 0x2002;
        }
        if (hero->hero_name == 3) {
            hero->hero_eng->sub->unk74 = 0x2002;
        }
        if (hero->hero_name == 2) {
            hero->hero_eng->sub->unk74 = 0x2002;
        }
        if (hero->hero_name == 4) {
            hero->hero_eng->sub->unk74 = 0x2002;
        }
        if (hero->hero_name == 7) {
            hero->hero_eng->sub->unk74 = 0x2003;
        }
        if (hero->hero_name == 6) {
            hero->hero_eng->sub->unk74 = 0x2002;
        }
        if (hero->hero_name == 8) {
            hero->hero_eng->sub->unk74 = 0x2002;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CFB7C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CFC58);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CFF44);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D0060);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D034C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D0470);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D05A8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D0650);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D0750);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D0808);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D0A6C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D10A0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D1188);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D14CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D17E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D191C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D1C14);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D1DB4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D2044);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D2248);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D2334);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D257C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D2B58);

void func_800D2C38(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = base + player;
    EXTheromove *move;

    if (FindExtension(hero->hero, 0x20000000) != NULL) {
        move = hero->hero_move;
        if (move != NULL) {
            move->z_vel = 0;
            move->x_vel = 0;
            move->z_max = 0;
        }
        SetHeroAction((HeroInfo *)player, 0x50);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D2CB8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D2D2C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D2D9C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D2E00);

void func_800D2EB0(s32 player) {
    s32 *ext = FindExtension(Heroes[player].hero, 0x20000000);

    if (ext != NULL) {
        if (ext[9] == 0) {
            SetHeroAction((HeroInfo *)player, 0x49);
        }
        if (ext[9] == 1) {
            SetHeroAction((HeroInfo *)player, 0x54);
        }
        func_8008502C(player);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D2F48);

void func_800D3018(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    EXTheromove *move = info->hero_move;

    if (move != NULL) {
        move->x_max = 0;
        move->z_max = 0;
        move->x_vel = 0;
        move->z_vel = 0;
    }
    info->hero_frozen = 4;
    func_800D2D2C();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D307C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D30C0);

void func_800D3170(void) {
    if (D_8005A648 != 0x16) {
        func_800D2D2C();
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D31A0);

void func_800D325C(s32 arg0) {
    Ext20 *ext = FindExtension(Heroes[arg0].hero, 0x20000000);

    if (ext != NULL) {
        ext->val += 4;
        if (ext->val > 0x100) {
            ext->val = 0x100;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D32CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D3394);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D3460);

void func_800D34F0(void) {
}

void func_800D34F8(void) {
}

void func_800D3500(void) {
}

void func_800D3508(void) {
}
