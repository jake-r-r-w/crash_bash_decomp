#include "common.h"
#include "bash/ovl68.h"
#include "bash/chase.h"
#include "bash/pogo.h"
#include "bash/bash.h"
#include "bash/xtras.h"

void func_800B3330(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B3338);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B3488);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl68/ovl68", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B37C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B3EFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B4048);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B4444);

void func_800B44C8(void) {
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B459C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B46BC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B48E4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B4B34);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B51C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B52C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B5CD0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B5E84);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6168);

void func_800B627C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *e;
    EXTengine *p;

    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    h->hero_move->jump_vel = 8;
    h->hero_move->jump_max = 0x70;
    h->hero_move->jump_acc = 0xA;
    h->hero_move->jump_dec = 0;
    h->hero_move->jump_dir = 0;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq((HeroInfo *)arg0, 0x66);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6350);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B643C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B65B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B66A4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B67D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B68B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B69AC);

void func_800B6AAC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base = Heroes;

    h = base + arg0;
    h->hero_hit = 0;
    h->hero_immunity = 0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 2;
    h->hero_health = 0;
    h->hero_status |= 0x2000;
    h->hero_powerup->flags = 0;
    h->hero_fx->flags = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    D_800BC2D0[arg0][0] = 0;
    func_800898EC(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6B84);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6C84);

void func_800B6E84(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
    }
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B6F6C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B722C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotBarrelGenerator);

void InitBarrelCH(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CoalFlameExt *ext;
    s32 i;
    s32 n;

    ext = AddExtension(nas, 0x44, 0x10000001);
    nas->FUX = &ext->ext;
    ((s32 *)ext)[0x30 / 4] = 1;
    ((s32 *)ext)[0x40 / 4] = -0x17;
    ext->unk1C = 0x3E8;
    eng = nas->Engine;
    sub = eng->sub;
    eng->unk2C.x = 0x3E8;
    eng->unk2C.y = 0x3E8;
    eng->unk2C.z = 0x3E8;
    eng->unk5C = 1;
    eng->unk10.y = 0;
    sub->unk6C = D_80053F8C;
    n = ++D_800BC450;
    if ((n & 0x1F) == 0) {
        sub->unk74 = 0x2016;
    } else if ((n & 7) == 0) {
        sub->unk74 = 0x2017;
    } else {
        sub->unk74 = 0x2001;
    }
    nas->UserInfo.status = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (D_800BC424[i] == NULL) {
                D_800BC424[i] = nas;
                break;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotBarrelCH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B7948);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B7B80);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B7DF0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCoalFlame);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCircleFire);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotEgg);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCoalFlameSFX);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotPUGenerator_CH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitPickupCH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotPickupCH);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B94FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B9A50);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B9ED8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA220);

void func_800BA5A0(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            func_800B94FC(arg0, (u16)arg1);
        } else if (f & 0x20) {
            func_800B9A50(arg0, (u16)arg1);
        } else if (f & 0x40) {
            func_800B9ED8(arg0, (u16)arg1);
        } else {
            func_800BA220(arg0, (u16)arg1);
        }
    }
}

void func_800BA630(NAS *nas) {
    s32 i;

    for (i = 0; i < 1; i++) {
        if (D_800BC420[i] == nas) {
            D_800BC420[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitPUGenerator_CH(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x258;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA694);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA70C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BA784);

void InitCoalFlameSFX(CoalFlameObj *arg0) {
    arg0->unk54->unkC = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitEgg);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", InitCircleFire);

void InitCoalFlame(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CoalFlameExt *ext;

    eng = nas->Engine;
    nas->UserInfo.user2 = 0xF;
    ext = AddExtension(nas, 0x44, 0x10000003);
    nas->FUX = &ext->ext;
    ext->unk1C = -0xD;
    sub = eng->sub;
    sub->unk6C = D_80053F8C;
    sub->unk74 = 0x200E;
    eng->unk5C = 1;
    eng->unk2C.x = 0x1000;
    eng->unk2C.y = 0x1000;
    eng->unk2C.z = 0x1000;
    eng->unk5E = 1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", PlotCoal);

void InitCoal(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    nas->FUX = AddExtension(nas, 0x44, 0x10000002);
    eng = nas->Engine;
    eng->unk5D = 1;
    nas->UserInfo.flags = 5;
    nas->UserInfo.user1 = 0;
    sub = eng->sub;
    sub->unk6C = D_80053F8C;
    sub->unk74 = 0x200F;
    nas->UserInfo.status = 0x3C;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800BAC30(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (D_800BC424[i] == nas) {
            D_800BC424[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitBarrelGenerator(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x3C;
}

void func_800BAC98(s32 arg0) {
    HeroInfo *hero;
    HeroInfo *base;

    base = Heroes;
    hero = base + arg0;
    hero->hero_frozen = 5;
    SetHeroAction((HeroInfo *)arg0, 0x77);
    if (hero->hero_hit != 0) {
        hero->hero_hit--;
    } else {
        SetHeroAction((HeroInfo *)arg0, 0x77);
    }
    func_8008502C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAD1C);

void func_800BADA8(s32 arg0) {
    func_800B52C8();
    func_8008502C(arg0);
}

void func_800BADD4(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x64);
}

void func_800BAE20(void) {
}

void func_800BAE28(void) {
}

void func_800BAE30(s32 arg0) {
    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x61);
}

s32 func_800BAE84(void) {
    return 0;
}

s32 func_800BAE8C(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAE94);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAF00);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAF58);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BAFB4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB010);

void func_800BB134(s32 arg0)
{
  HeroInfo *h;
  HeroInfo *new_var;
  if (D_800BC444 == 0)
  {
    new_var = Heroes;
    h = new_var;
    h += arg0;
    if ((h->hero_immunity == 0) && (!(h->hero_status & 0xFA00)))
    {
      SetHeroAction((HeroInfo *) arg0, (u16) D_8009E0D8);
      func_80027F7C(arg0, 0xA, 0xF0);
    }
  }
}

s32 func_800BB1C0(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) continue;
        if (h->hero_status & 0xC000) continue;
        d = x - h->hero_eng->unk10.x;
        if (d < -range || range < d) continue;
        d = z - h->hero_eng->unk10.z;
        if (-range < d && d < range) return i;
    }
    return -1;
}

s32 func_800BB260(s32 arg0) {
    u32 m = D_80051338[arg0].unk54 & 0x2000;
    return m != 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB294);

void func_800BB36C(void) {
}

void func_800BB374(void) {
}

void func_800BB37C(void) {
}

void func_800BB384(void) {
    D_8005AA70[0] = (s32)func_800B3EFC;
    D_8005AA70[1] = (s32)func_800B4048;
    D_8005AA70[2] = (s32)func_800B4048;
    D_8005AA70[3] = (s32)func_800B4444;
    D_8005AA70[4] = (s32)func_800BB37C;
    D_8005AA70[6] = (s32)func_800BB374;
    D_8005AA70[5] = (s32)func_800BB36C;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[10] = (s32)func_800B44C8;
}

void func_800BB3F0(s32 x, s32 y) {
    s32 xi = (x + 0xA00) >> 7;
    s32 yi = (y + 0xA00) >> 7;
    s32 idx = yi * 40 + xi;
    D_800BC4E8[idx] = 0;
}

s32 func_800BB428(s32 x, s32 z) {
    s32 xi;
    s32 zi;
    s32 idx;

    if ((u32)(x + 0x9FF) >= 0x13FF || z >= 0xA00 || z < -0x9FF) {
        return 0;
    }
    xi = (x + 0xA00) >> 7;
    zi = (z + 0xA00) >> 7;
    idx = zi * 40 + xi;
    return D_800BC4E8[idx];
}

void func_800BB488(CoalGridObj *obj) {
    CoalPosInner *p = obj->pos;
    s32 x = p->x;
    s32 z = p->z;
    s32 idx;
    CoalGridObj *old;

    x += 0xA00;
    z += 0xA00;
    x >>= 7;
    z >>= 7;
    idx = z * 40 + x;
    old = D_800BC4E8[idx];

    D_800BC4E8[idx] = obj;
    if (old != NULL) {
        KillNAS((NAS *)old);
    }
}

s32 func_800BB4F8(s32 x, s32 z, s32 range, s32 skip) {
    s32 i;
    s32 neg;
    s32 dx;
    s32 dz;
    HeroInfo *h;
    EXTengine *eng;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (i == skip || h->hero_name == 9 || (h->hero_status & 0xC000)) {
            continue;
        }
        eng = h->hero_eng;
        dx = x - eng->unk10.x;
        if (dx < -range || range < dx) {
            continue;
        }
        dz = z - eng->unk10.z;
        if (-range < dz && dz < range) {
            return i;
        }
    }
    return -1;
}

s32 func_800BB5A0(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) continue;
        if (h->hero_status & 0xC000) continue;
        d = x - h->hero_eng->unk10.x;
        if (d < -range || range < d) continue;
        d = z - h->hero_eng->unk10.z;
        if (-range < d && d < range) return i;
    }
    return -1;
}

void func_800BB640(s32 arg0) {
    u8 *temp_v0;

    temp_v0 = (arg0 * 0x30) + D_800BC210;
    (*(s32*)((u8*)(temp_v0) + 0)) = 1;
    (*(s32*)((u8*)(temp_v0) + 4)) = 1;
    (*(s32*)((u8*)(temp_v0) + 8)) = 0;
    (*(s32*)((u8*)(temp_v0) + 24)) = 0;
    (*(s32*)((u8*)(temp_v0) + 28)) = 0;
    (*(s32*)((u8*)(temp_v0) + 32)) = 0;
    (*(s32*)((u8*)(temp_v0) + 36)) = 0;
    (*(s32*)((u8*)(temp_v0) + 40)) = 0x8000;
    (*(s32*)((u8*)(temp_v0) + 44)) = 0x8000;
}

NAS *func_800BB688(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    NAS *nas;
    EXTengine *eng;

    for (i = 0; i < 1; i++) {
        nas = D_800BC420[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (z - eng->unk10.z) < range) {
                return nas;
            }
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB6FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800BB7E0);
