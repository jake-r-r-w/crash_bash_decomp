#include "common.h"
#include "bash/main.h"
#include "bash/ovl67.h"
#include "bash/n2o.h"
#include "bash/arkenoid.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B34A4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B361C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B3888);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B39C0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B3C04);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B3CD0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B3E74);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B4138);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B447C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B46DC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B4954);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B4AD8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B4CE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B4E30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B4FA4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B5064);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B5168);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B52CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B5440);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B5550);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B5674);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B5774);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B5838);

void func_800B59A8(s32 arg0) {
    SLONG_XYZ sp18;
    EXTengine *temp_v0;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *temp_s0;
    HeroInfo *temp_s0_2;
    _sfx_handler *temp_a0;
    subengine *temp_v1;

    base = Heroes;
    temp_s0 = &base[arg0];
    temp_s0->hero_hit = 0;
    temp_s0->hero_immunity = 0;
    temp_s0->hero_move->velocity_enable = 0;
    temp_s0->hero_move->jump_max = 0;
    temp_s0->hero_health = 0;
    temp_s0->hero_frozen = 2;
    temp_s0->hero_powerup->flags = 0;
    temp_s0->hero_fx->flags = 0;
    temp_s0->hero_pu = 0;
    temp_s0->hero_pu_timer = 0;
    temp_s0->hero_anim->sub = NULL;
    temp_a0 = temp_s0->hero_powerup->sfx;
    if (temp_a0 != NULL) {
        StopSfx(temp_a0);
    }
    temp_s0->hero_status |= 0x2000;
    temp_v1 = temp_s0->hero_sub;
    temp_v1 = temp_v1->next;
    if (temp_v1 != NULL) {
        temp_v1->flags = 0;
        temp_v1 = temp_v1->next;
        if (temp_v1 != NULL) {
            temp_v1->flags = 0;
        }
    }
    base2 = Heroes;
    temp_s0_2 = &base2[arg0];
    temp_v0 = temp_s0_2->hero_eng;
    sp18 = temp_v0->unk10;
    func_8007A524(0, (s32 *)&sp18, (s32) &D_80053EE4);
    PlaySfx(0x586, NULL, &temp_s0_2->hero_eng->unk10, 0x1000, 0x1E00, 0);
    func_800898EC(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B5B14);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B5BFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", InitN2OBall);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl67/ovl67", D_800B32B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", PlotN2OBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B6E6C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B6F4C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B7244);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B7858);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B7C58);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B8058);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B817C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B83EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B8968);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", PlotN2Omissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", PlotN2Ofirstmissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B90F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B92D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", InitN2ODeadWall);

void func_800B9800(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r) {
    SLONG_XYZ start;
    SLONG_XYZ d;
    SLONG_XYZ dir;
    s32 a;
    s32 b;
    s32 c;
    s32 s;
    s32 t1;
    s32 t2;

    arg1 = (arg1 + 0x800) & 0xFFF;
    start = *pos;
    start.x -= (0x200 * D_80068BD4[(arg1 - 0x400) & 0xFFF].unk2) >> 12;
    start.z -= (0x200 * D_80068BD4[arg1].unk2) >> 12;
    dir.x = start.x - pos->x;
    dir.y = 0;
    dir.z = start.z - pos->z;
    if (dir.x == 0) {
        return;
    }
    d.x = target->x - start.x;
    d.y = 0;
    d.z = target->z - start.z;
    a = (dir.x * dir.x + dir.z * dir.z) >> 4;
    b = ((dir.x * d.x + dir.z * d.z) * 2) >> 4;
    c = (d.x * d.x + d.z * d.z - r * r) >> 4;
    s = b * b - a * c * 4;
    if (s < 0) {
        return;
    }
    s = csqrt(s) >> 6;
    t1 = ((-b + s) << 8) / (a * 2);
    t2 = ((-b - s) << 8) / (a * 2);
    if (t1 < 0 && t2 < 0) {
        return;
    }
    if (t1 < t2) {
        if (t1 < 0) {
            pos->x = start.x - ((dir.x * t2) >> 8);
            pos->y = start.y - ((dir.y * t2) >> 8);
            pos->z = start.z - ((dir.z * t2) >> 8);
        } else {
            pos->x = start.x - ((dir.x * t1) >> 8);
            pos->y = start.y - ((dir.y * t1) >> 8);
            pos->z = start.z - ((dir.z * t1) >> 8);
        }
    } else if (t2 >= 0) {
        pos->x = start.x - ((dir.x * t2) >> 8);
        pos->y = start.y - ((dir.y * t2) >> 8);
        pos->z = start.z - ((dir.z * t2) >> 8);
    } else {
        pos->x = start.x - ((dir.x * t1) >> 8);
        pos->y = start.y - ((dir.y * t1) >> 8);
        pos->z = start.z - ((dir.z * t1) >> 8);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800B9B10);

s32 func_800B9CB8(s32 arg0, EXTengine *unused) {
    SLONG_XYZ tgt;
    SLONG_XYZ nearp;
    SLONG_XYZ farp;
    VECTOR pos;
    EXTengine *me;
    s32 speed;
    s32 top;
    s32 bot;
    s32 rnear;
    s32 rfar;
    HeroInfo *hero;
    Ovl67HeroBox *box;
    NAS *nas;
    EXTengine *eng;
    FlagRecord *rec;
    EXTobject *obj;
    Ovl67Puck *puck;
    s32 r;
    s32 rad;
    s32 d;
    s32 dist;
    s32 y;
    s32 hy;
    s32 x1;
    s32 x2;
    s32 z2;
    s32 dz;
    s32 fa;
    s16 sp;
    TrigEntry *tbl;
    s32 cz;
    s32 fb;
    u16 ang;
    u16 ang2;
    u16 fc;
    u16 fc2;
    Ovl67HeroBox *base;

    if (Heroes[arg0].hero_status & 0xF800) {
        return 0;
    }
    me = Heroes[arg0].hero_eng;
    if (me == NULL) {
        return 0;
    }
    speed = Heroes[arg0].hero_move->forward_speed;
    if (arg0 == 3) {
        speed = -Heroes[3].hero_move->forward_speed;
    }
    if (func_80015780(me, me->sub, 1, &pos) == NULL) {
        *(SLONG_XYZ *)&pos = me->unk10;
    }
    if (func_80025A48(me, 2) != NULL) {
        if (me->unk5E != 0) {
            func_800142D4(&me->unk1C);
        }
        func_8001CD04(me->sub, me->unk3C, &me->unk10);
    }
    hy = Heroes[arg0].hero_eng->unk10.y;
    base = D_800C4190;
    box = &base[arg0];
    top = hy + box->b;
    bot = hy + box->a;
    rnear = box->c;
    rfar = box->d;
    nearp.x = Heroes[arg0].hero_eng->unk10.x + box->x0;
    nearp.y = Heroes[arg0].hero_eng->unk10.y + box->y0;
    nearp.z = Heroes[arg0].hero_eng->unk10.z + box->z0;
    hero = &Heroes[arg0];
    farp.x = hero->hero_eng->unk10.x + box->x1;
    farp.y = hero->hero_eng->unk10.y + box->y1;
    farp.z = hero->hero_eng->unk10.z + box->z1;
    tbl = D_80068BD4;
    for (nas = D_8005AA2C;; nas = nas->Solid.Child) {
        if (nas == NULL) {
            return 0;
        }
        eng = nas->Engine;
        if (eng == NULL || eng == me) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        dz = rec->unk2;
        y = eng->unk10.y;
        x1 = rec->height;
        if (!(top < y + dz)) {
            continue;
        }
        y += x1;
        if (!(y < bot)) {
            continue;
        }
        rad = rec->radius;
        if (func_80015480(hero->hero_move->forward_angle,
                          get_angle(-eng->OldXYZ.x + pos.vx, -eng->OldXYZ.z + pos.vz)) >= 0x38F) {
            r = rnear;
            tgt = nearp;
        } else {
            r = rfar;
            tgt = farp;
        }
        get_angle(eng->unk10.x - pos.vx, eng->unk10.z - pos.vz);
        obj = FindExtension(nas, 0x1000);
        if (obj == NULL) {
            continue;
        }
        rad += r;
        y = eng->unk10.x - tgt.x;
        dist = y * y;
        y = eng->unk10.z - tgt.z;
        dist += y * y;
        if (!(dist < rad * rad)) {
            continue;
        }
        dist = csqrt(dist) >> 6;
        if (!(dist < rad)) {
            continue;
        }
        puck = FindExtension(nas, 0x800);
        if (puck != NULL) {
            r = puck->dir;
        } else {
            r = get_angle(eng->OldXYZ.x - eng->unk10.x, eng->OldXYZ.z - eng->unk10.z);
        }
        func_800B9800(&eng->unk10, r, &tgt, rad + 8);
        r = get_angle(-eng->unk10.x + tgt.x, -eng->unk10.z + tgt.z) & 0xFFF;
        puck = FindExtension(nas, 0x800);
        if (puck == NULL) {
            continue;
        }
        obj->player = arg0;
        rad = puck->dir;
        rad += 0x800;
        rad &= 0xFFF;
        if (obj->whatami == 2 && !(hero->hero_status & 0xFA00) &&
            (arg0 == 0 || (D_8005A680[0] != 0 && arg0 == 1))) {
            if (!(D_8005A618 & 1)) {
                hero->hero_score = 0;
                hero->hero_health = 0;
                ((void (*)(s32, s32, s32))func_80027F7C)(arg0, 0xC, 0xFF);
                SetHeroAction((HeroInfo *)arg0, 0x2E);
            }
            KillNAS(nas);
            return 0;
        }
        PlaySfx(D_800C39B0[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        fa = hero->hero_move->forward_angle & 0xFFF;
        ang = r;
        fc = fa;
        sp = abs(speed);
        x1 = puck->speed * tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
        z2 = puck->speed * tbl[r & 0xFFF].unk2 >> 12;
        dz = sp * tbl[(fc - 0x400) & 0xFFF].unk2 >> 12;
        cz = sp * tbl[fa].unk2 >> 12;
        r = get_angle(x1 + dz, z2 + cz) & 0xFFF;
        d = func_800154A0(rad, r);
        if (abs(d) > 0x400) {
            func_800154A0(rad ^ 0x800, r);
            fb = hero->hero_move->forward_angle & 0xFFF;
            ang2 = r;
            fc2 = fb;
            sp = abs(speed);
            x1 = puck->speed * tbl[(ang2 - 0x400) & 0xFFF].unk2 >> 12;
            z2 = puck->speed * tbl[r].unk2 >> 12;
            dz = sp * tbl[(fc2 - 0x400) & 0xFFF].unk2 >> 12;
            x2 = sp * tbl[fb].unk2 >> 12;
            r = get_angle(x1 + dz, z2 + x2) & 0xFFF;
        } else {
            if (abs(d) > 0x3F0) {
                if (d >= 0) {
                    d = 0x3F0;
                } else {
                    d = -0x3F0;
                }
            }
            r += d;
            r &= 0xFFF;
        }
        puck->dir = r;
        if (speed != 0) {
            puck->speed += abs(speed);
            if (puck->speed > puck->max + 0x40) {
                puck->speed = puck->max + 0x40;
            }
        }
        if (puck->unk32 == 0 && puck->max < 0x90) {
            puck->max += 4;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BA43C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BA6C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BAAA8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BAC30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BAEA8);

void PlotN2ODeadWall(NAS *arg0) {
    EXTengine *eng;
    subengine *sub;

    if (D_8005A644 & 0x801) {
        D_800C438A &= ~arg0->UserInfo.flags;
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

s32 func_800BB05C(u8 *arg0) {
    return (*(s32*)((u8*)(arg0) + 20)) - D_8005A5D8;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB070);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB184);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", PlotN2OmissileExplode);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", InitN2OmissileExplode);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB3A0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", InitN2Ofirstmissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", InitN2Omissile);

void func_800BB5F0(void) {
    volatile s32 x, y, z;
    SpawnNASById(x, y, z, 0x1B01, 0x11);
}

u8 *func_800BB624(void) {
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
    temp_v0 = SpawnNASById(0x1000, 0x1000, 0x1000, 0x1B01, 0x11);
    if ((temp_v0 != NULL) && ((*(s32*)((u8*)(temp_v0) + 84)) != 0)) {
        temp_v0_2 = (*(s32*)((u8*)(temp_v0) + 88));
        if (temp_v0_2 != NULL) {
            (*(s16*)((u8*)(temp_v0_2) + 26)) = 0;
        }
    }
    return temp_v0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB6BC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB72C);

void func_800BB788(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x24);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB7D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB86C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB8EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB944);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BB9B0);

void func_800BBA4C(s32 arg0) {
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BBB10);

s32 func_800BBBB4(s32 arg0, s16 arg1, s32 arg2, s16 arg3) {
    s32 a = arg1 * D_80068BD4[((arg0 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 b = arg1 * D_80068BD4[arg0 & 0xFFF].unk2;
    s32 c = arg3 * D_80068BD4[((arg2 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 d = arg3 * D_80068BD4[arg2 & 0xFFF].unk2;
    s32 x = a >> 12;
    s32 z = b >> 12;
    x += c >> 12;
    z += d >> 12;
    return get_angle(x, z) & 0xFFFF;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BBC78);

void func_800BBD18(void) {
    if (D_800C417A == 1) {
        func_800B92D4();
        func_800B5BFC();
        func_800B7244();
        return;
    }
    func_800BE70C();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/ovl67", func_800BBD68);
