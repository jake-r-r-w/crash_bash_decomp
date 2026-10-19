#include "common.h"
#include "bash/main.h"
#include "bash/ovl63.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B345C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B35CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B372C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B3890);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B3BC0);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl63/ovl63", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B3E60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B406C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B41DC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B442C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B4708);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B48EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B4A4C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B4D64);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B4EDC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5360);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5528);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5660);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5778);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5A70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5D14);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5F04);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5FFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B6108);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B62F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B6474);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B66C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B6DBC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B7190);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B76A4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B7D94);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl63/ovl63", D_800B3310);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B82FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B8890);

void func_800B8D68(void) {
    D_800C692C++;
    if (G63->unk30 & 0x800) {
        D_800C695C = 1;
    }
    if (G63->unk30 & 1) {
        D_800C695C = 1;
    }
    if (G63->unk10 == 0) {
        D_800C6BB0--;
    }
    if (D_800C695C == 0) {
        D_800AD12C = 0;
    } else {
        D_800AD12C = 1;
    }
    func_800C04EC();
    if (G63->unk34 != 0x19) {
        func_800C0CB4();
    }
    if (G63->unk34 == 0x18) {
        func_800B6474();
    }
    if (G63->unk34 == 0x19) {
        func_800B5D14();
    }
    if (G63->unk34 == 0x1A && !(G63->unk32 & 0x80)) {
        D_800C69B4->unk74 = (D_800C69B8 >> 8) + 0x4000;
        if (D_800C69BC == 0) {
            D_800C69B8 += 0x80;
            if (D_800C69B8 >= 0x6000) {
                D_800C69B8 = 0;
            }
        }
        if (D_800C69BC == 1) {
            D_800C69B8 -= 0x19C;
            if (D_800C69B8 <= 0) {
                D_800C69B8 = 0;
                D_800C69BC = 0;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B8F04);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B8FD8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B96B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B980C);

void func_800B9E50(Quat *q) {
    s32 n;
    s32 sw;
    s32 sz;
    s32 sx;
    s32 sy;

    sw = q->v[3] * q->v[3];
    sz = q->v[2] * q->v[2];
    sx = q->v[0] * q->v[0];
    sy = q->v[1] * q->v[1];
    n = ((sx >= 0 ? sx + 0x800 : sx - 0x800) >> 12)
      + ((q->v[1] * q->v[1] >= 0 ? sy + 0x800 : sy - 0x800) >> 12)
      + ((q->v[2] * q->v[2] >= 0 ? sz + 0x800 : sz - 0x800) >> 12)
      + ((q->v[3] * q->v[3] >= 0 ? sw + 0x800 : sw - 0x800) >> 12);
    q->v[0] = (q->v[0] << 12) / n;
    q->v[1] = (q->v[1] << 12) / n;
    q->v[2] = (q->v[2] << 12) / n;
    q->v[3] = (q->v[3] << 12) / n;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BA004);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BA278);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BAD54);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BAFC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BB794);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BB878);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BBA04);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BC0A4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BC2D0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BC4B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BC6C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BC7C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BC984);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BCC20);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BCD1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BCDAC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BCE88);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BD99C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BE7B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BE914);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BEBC0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BF0F8);

void InitRaceMissile(NAS *m) {
    RaceMissileExt *ext;
    EXTengine *e;

    ext = AddExtension(m, 0x78, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk74 = 0x2011;
        m->Engine->sub->unk6C = D_80052F5C + 0x2028;
        m->Engine->unk5D = 2;
        m->Engine->unk5C = 1;
        m->Engine->sub->unk51 = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk64 = 0x8C;
        ext->unk34 = 0x3000;
        ext->unk40 = 0x700;
        ext->unk54 = 4;
        ext->unk10 = 0;
        ext->unk20 = 0;
        ext->unk60 = 0;
        ext->unk4C = 0;
        ext->unk50 = 0x100;
        ext->unk5C = NULL;
        ext->unk68 = 0;
        ext->unk58 = ext->unk34;
        e = AddEngine(m, 2);
        if (e != NULL) {
            ext->unk5C = e;
            e->sub->unk6C = D_80052F5C;
            e->sub->unk74 = 0x2001;
            e->unk5C = 1;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
        }
        PlaySfx(0x522, &m->Engine->sfx, &m->Engine->unk10, 0x1000, 0x1E00, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRaceMissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitRacePickUp);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl63/ovl63", D_800B3400);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRacePickUp);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C0218);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C04EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C0CB4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C0F5C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotTNTCrate);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitDinoEgg);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotDinoEgg);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRaceShield);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRaceTag);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitRaceVolcano);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRaceVolcano);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotGlob);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C310C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitBarrelRoll);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotBarrelRoll);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitTrackGlob);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotTrackGlob);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitDac);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotDac);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C44B0);

void PlotRaceSmokeBoostWater(NAS *nas) {
    func_800250B0(nas);
}

void InitRaceSmokeBoostWater(NAS *nas) {
    func_80024E30(nas, &D_800C6F28);
    nas->Engine->sub->unk6C = &D_80054F84;
    nas->UserInfo.user1 = 0;
}

s32 func_800C467C(Ovl63BounceObj *obj, Ovl63Bouncer *b) {
    s32 pos;

    b->vel += b->accel;
    pos = obj->pos += b->vel >> 8;
    if (b->limit < pos) {
        obj->pos -= abs(pos - b->limit);
        if (b->flags & 1) {
            b->vel = -b->vel;
        }
        if (b->flags & 2) {
            b->vel = -((b->vel * b->damp) >> 8);
        }
        if (b->flags & 4) {
            b->vel = -b->rebound;
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitGlob);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotInnerRipple);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitInnerRipple);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRippled);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitRippled);

void PlotRaceGreenSmokeBoost(NAS *nas) {
    func_800250B0(nas);
}

void InitRaceGreenSmokeBoost(NAS *nas) {
    func_80024E30(nas, &D_800C6E2C);
    nas->Engine->sub->unk6C = &D_80054F84;
    nas->UserInfo.user1 = 0;
}

void PlotRaceGreenSmokePoof(NAS *nas) {
    func_800250B0(nas);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitRaceGreenSmokePoof);

void PlotRaceSmokePoof(NAS *nas) {
    func_800250B0(nas);
}

void InitRaceSmokePoof(NAS *nas) {
    func_80024E30(nas, &D_800C6D74);
    nas->Engine->sub->unk6C = D_800546C4;
    nas->Engine->sub->flags |= 0x21;
    nas->UserInfo.user1 = 0;
}

s32 func_800C4D1C(u8 *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    temp_v0 = (*(s32*)((u8*)(arg0) + 0)) - (*(s32*)((u8*)(arg1) + 0));
    temp_v0_2 = (*(s32*)((u8*)(arg0) + 8)) - (*(s32*)((u8*)(arg1) + 8));
    temp_v0_3 = (*(s32*)((u8*)(arg0) + 4)) - (*(s32*)((u8*)(arg1) + 4));
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2) + (temp_v0_3 * temp_v0_3);
}

void InitRaceTag(u8 *arg0) {
    D_800C6D20 = 0x12C;
    PlaySfx(0x452, (*(s32*)((u8*)(arg0) + 84)) + 0x94, 0, 0x1000, 0x1E00, 0);
}

void InitRaceShield(NAS *nas) {
    EXTraceshield *ext;

    ext = AddExtension(nas, 0x20, 0x10000000);
    if (ext != NULL) {
        ext->unk10 = 0xE;
        ext->unkC = 0;
        ext->unk1C = 0x100;
    }
}

void PlotRaceFire(NAS *nas) {
    FindExtension(nas, 0x10000000);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitRaceFire);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C4E54);

s32 func_800C4F30(u8 *arg0) {
    HeroInfo *hero;
    u32 i;

    i = 0;
    hero = Heroes;
loop:
    if (i < 4) {
        if (func_8008CEB0(arg0, (u8 *)hero->hero_eng + 0x10) < 0x6184) {
            if (hero->hero_mode != 0x5D) {
                SetHeroAction((HeroInfo *)i, 0x5D);
            }
            return i;
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

void InitTNTCrate(NAS *m) {
    m->Engine->unk5D = 2;
    m->Engine->unk5C = 1;
    m->Engine->sub->unk74 = 0x2009;
    m->Engine->sub->unk6C = (s32)D_80052F94;
    m->UserInfo.status = 0;
    m->UserInfo.user1 = 0;
    m->UserInfo.user2 = 0;
    m->UserInfo.flags = 0;
}

s32 func_800C5014(s32 arg0) {
    s32 var_a0;

    var_a0 = arg0;
    if (var_a0 < 0) {
        var_a0 += 0x1FF;
    }
    return var_a0 >> 9;
}

void PlotRaceSmokeBoost(NAS *nas) {
    func_800250B0(nas);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", InitRaceSmokeBoost);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C50B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C5154);

void func_800C5204(EXTheromove *a, EXTheromove *b) {
    s32 ma;
    s32 mb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;

    ma = a->mass;
    mb = b->mass;
    if (ma > 0 && mb > 0) {
        d = -((a->x_vel - b->x_vel) << 8);
        q = (d * mb + ((ma * a->x_vel + mb * b->x_vel) << 8)) / (total = mb + ma);
        r = q - d;
        a->x_vel = q >> 8;
        b->x_vel = r >> 8;
        d = -((a->z_vel - b->z_vel) << 8);
        q = (d * mb + ((ma * a->z_vel + mb * b->z_vel) << 8)) / total;
        r = q - d;
        a->z_vel = q >> 8;
        b->z_vel = r >> 8;
    }
}

s32 func_800C5334(s32 arg0) {
    EXTengine *eng;
    s32 angle;

    eng = Heroes[arg0].hero_eng;
    angle = get_angle(eng->unk10.x, eng->unk10.z) & 0xFFF;
    angle = 0x152C - angle;
    angle &= 0xFFF;
    printf(D_800B3400, arg0, angle);
    return angle;
}

void func_800C53BC(s32 arg0) {
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

s32 func_800C5400(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    if (hero->hero_status & 0x200) {
        if (hero->hero_mode != 0x62) {
            SetHeroAction((HeroInfo *)arg0, 0x62);
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C5470);

s32 func_800C554C(s32 arg0) {
    if (((Ovl63Pad *)D_80051338)[arg0].unk50 & 0xF0) {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, 0x5B);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C55A4);

void func_800C55FC(s32 player) {
    EXTraceshield *ext;

    ext = (EXTraceshield *)Heroes[player].hero->FUX;
    if (ext != NULL) {
        ext->unk14[0] = 8;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C5640);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C56D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C57B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C5854);

void func_800C5918(s32 arg0) {
    SetHeroAction((HeroInfo *)arg0, 0x5B);
    Heroes[arg0].hero_eng->unk1E = 0;
    Heroes[arg0].hero_eng->unk5E = 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C5978);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C5A50);

s32 func_800C5AF8(void) {
    return D_800C6930;
}

void func_800C5B08(s32 *pos, s32 *polar) {
    polar[0] = csqrt(pos[0] * pos[0] + pos[2] * pos[2]) >> 6;
    polar[1] = get_angle(pos[0], pos[2]) & 0xFFF;
}

s32 func_800C5B7C(void) {
    return 0;
}

void func_800C5B84(void) {
}

void func_800C5B8C(void) {
}

void func_800C5B94(void) {
    if (D_800C6BAC-- < 0) {
        SpawnNASById(0, 0, 0, 0x170C, 0x11);
        D_800C6BAC = 0xB4;
    }
}

void func_800C5BEC(void) {
    D_800C69B4->unk74 = (D_800C69B8 >> 8) + 0x4000;
    if (D_800C69BC == 0) {
        D_800C69B8 += 0x80;
        if (D_800C69B8 >= 0x6000) {
            D_800C69B8 = 0;
        }
    }
    if (D_800C69BC == 1) {
        D_800C69B8 -= 0x19C;
        if (D_800C69B8 <= 0) {
            D_800C69B8 = 0;
            D_800C69BC = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C5C64);

s32 func_800C5D0C(s32 arg0) {
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

void func_800C5D8C(void) {
    func_800B4708();
}

NAS *func_800C5DAC(s32 arg0) {
    NAS *nas;
    HeroInfo *base;
    HeroInfo *hero;
    s32 angle;

    nas = func_800C5C64(arg0);
    if (nas != NULL) {
        base = Heroes;
        hero = base + arg0;
        if (func_8008CEB0((u8 *)&nas->Engine->unk10, (u8 *)&hero->hero_eng->unk10) <= 0x270FFF) {
            angle = get_angle(nas->Engine->unk10.x, nas->Engine->unk10.z) & 0xFFF;
            if ((get_angle(hero->hero_eng->unk10.x, hero->hero_eng->unk10.z) & 0xFFF) < angle) {
                return nas;
            }
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C5E6C);

void func_800C5F54(void) {
    D_8005AA70[0] = (s32)func_800B4A4C;
    D_8005AA70[1] = (s32)func_800B8890;
    D_8005AA70[2] = (s32)func_800B8890;
    D_8005AA70[3] = (s32)func_800B8D68;
    D_8005AA70[5] = (s32)func_800C5B84;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
}
