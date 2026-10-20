#include "common.h"
#include "bash/ovl64.h"
#include "bash/spin.h"
#include "bash/hero.h"
#include "bash/shell.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/bash.h"
#include "bash/xtras.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B3428);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B359C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B382C);

void func_800B3938(void) {
    s32 i;
    Ovl64Obj *obj;
    s16 *p;

    D_800BC010 = 0;
    D_800BC014 = 0;
    D_800BC01C = 1;
    D_800BC018 = 0;
    D_800BC01A = 0;
    obj = ((Ovl64Scene *)D_80058AD0)->objs;
    for (i = 9; i >= 0; i--) {
        D_800BC020[i] = NULL;
    }
    for (; obj != NULL; obj = obj->next) {
        p = (s16 *)func_80015870(obj->model, obj->index);
        i = *p - 10;
        if ((u32)i < 10) {
            D_800BC020[i] = obj;
            obj->unk51 = 0;
        }
        i = *p - 20;
        if ((u32)i < 10) {
            D_800BBCB0[i].obj = obj;
            obj->unk51 = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B3A3C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B3EDC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B4144);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B4970);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl64/ovl64", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B4BA4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B4DC4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B4F7C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B51A0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", PlotHazardType);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B5D3C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", PlotBalloon);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B697C);

void func_800B6C50(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *pu;
    subengine *sub;
    VECTOR pos;
    s32 y;
    s32 top;

    base = Heroes;
    h = base + arg0;
    if (h->hero_pu_timer != 0) {
        h->hero_pu_timer--;
    }
    if (h->hero_pu_lock != 0) {
        h->hero_pu_lock--;
    }
    if (h->hero_pu != 0) {
        pu = h->hero_powerup;
        sub = pu->sub;
        if (func_80015780(h->hero_eng, h->hero_sub, 0x80, &pos) != NULL) {
            top = pos.vy - 0x40;
            y = top - h->hero_eng->unk10.y;
        } else {
            switch (h->hero_name) {
            case 1:
            case 2:
                y = -0x180;
                break;
            case 3:
            case 4:
                y = -0x200;
                break;
            case 5:
            case 6:
            case 7:
            default:
                y = -0x280;
                break;
            }
        }
        pu->unk10 = Heroes[arg0].hero_eng->unk10;
        sub->unk08 = y;
        if (Heroes[arg0].hero_pu != 0x13) {
            if (Heroes[arg0].hero_pu == 0x17 && D_800BBD58 > 30) {
                D_800BBD58 = 30;
            }
        }
        if (Heroes[arg0].hero_weapon_count == 0) {
            Heroes[arg0].hero_status &= 0xFFEF;
        }
        if (Heroes[arg0].hero_pu_timer != 0) {
            sub->flags |= 0x8000;
        } else {
            Heroes[arg0].hero_pu = 0;
            sub->flags &= ~0x8000;
            if (pu->sfx != NULL) {
                StopSfx(pu->sfx);
            }
        }
        pu->unk10 = Heroes[arg0].hero_eng->unk10;
        pu->unk5E = 1;
        pu->unk1E += 0xAA;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B6EC0);

s32 func_800B727C(x, y, z, size, skip, mask)
    s32 x;
    s32 y;
    s32 z;
    u16 size;
    s32 skip;
    u16 mask;
{
    s32 ret;
    u32 i;
    EXTengine *eng;
    NAS *nas;
    EXTobject *obj;
    FlagRecord *rec;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 rr;
    s32 dist;

    ret = -1;
    for (i = 0; i < 4; i++) {
        if (skip == i) {
            continue;
        }
        if (Heroes[i].hero_status & 0xFA00) {
            continue;
        }
        eng = Heroes[i].hero_eng;
        if (eng == NULL) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        top = eng->unk10.y + rec->unk2 + size;
        bot = eng->unk10.y + rec->height - size;
        if (y < top && bot < y) {
            dx = eng->unk10.x - x;
            dist = dx * dx;
            dz = eng->unk10.z - z;
            dist += dz * dz;
            rr = rec->radius + size;
            rr *= rr;
            if (dist < rr) {
                ret = i;
            }
        }
    }
    for (nas = D_8005AA2C; nas != NULL; nas = nas->Solid.Child) {
        if (!(nas->Class & 0x200)) {
            continue;
        }
        eng = nas->Engine;
        if (eng == NULL) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        top = eng->unk10.y + rec->unk2 + size;
        bot = eng->unk10.y + rec->height - size;
        if (y < top && bot < y) {
            dx = eng->unk10.x - x;
            dist = dx * dx;
            dz = eng->unk10.z - z;
            dist += dz * dz;
            rr = rec->radius + size;
            rr *= rr;
            if (dist < rr) {
                obj = FindExtension(nas, 0x1000);
                if (obj != NULL && (obj->can_do & mask)) {
                    obj->player = skip;
                    obj->will_do |= mask;
                }
                if (ret != -1) {
                    ret = -1;
                }
            }
        }
    }
    return ret;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl64/ovl64", D_800B337C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B7508);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B8294);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B85A4);

s32 func_800B8B5C(s32 arg0) {
    s32 other;
    s32 sum;
    s32 i;
    s32 r;

    other = -1;
    if (D_8005A680 != 0) {
        other = arg0 ^ 1;
    }
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != other && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i][2];
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != other && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i][2];
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B8C74);

s32 func_800B8D98(s32 arg0) {
    EXTbotai *ai = Heroes[arg0].hero_ai;
    s32 i;
    s32 l;
    s32 r;

    for (i = 0; i < 3; i++) {
        l = ai->AIforward_left[i] & 0x3F;
        r = ai->AIforward_right[i] & 0x3F;
        if (ai->AIforward_left[i] == 0xFF || ai->AIforward_right[i] == 0xFF) {
        ret1:
            return 1;
        }
        if (l >= 6) {
            if (l < 8) {
                goto ret1;
            }
            if (l < 0x16) {
                if (l >= 0x14) {
                    goto ret1;
                }
            }
        }
        if (r >= 6) {
            if (r < 8) {
                goto ret1;
            }
            if (r < 0x16) {
                if (r >= 0x14) {
                    goto ret1;
                }
            }
        }
    }
    return 0;
}

s32 func_800B8E4C(void) {
    return 0;
}

s32 func_800B8E54(void) {
    return 0;
}

void func_800B8E5C(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B8E64);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B8EE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B8F48);

void func_800B9038(void) {
}

void func_800B9040(void) {
}

void func_800B9048(void) {
    func_80086F78();
    func_800B4BA4();
    func_80086D88();
    func_800B4970();
    func_800B3A3C();
}

void func_800B9088(void) {
    D_8005AA70[0] = (s32)func_800B3428;
    D_8005AA70[1] = (s32)func_800B359C;
    D_8005AA70[2] = (s32)func_800B359C;
    D_8005AA70[3] = (s32)func_800B9048;
    D_8005AA70[5] = (s32)func_800B9038;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B90D0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B92BC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B937C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B9608);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B971C);

void func_800B9804(s32 arg0) {
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
    func_800898EC(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B98B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B9A04);

void func_800B9BF8(s32 arg0) {
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B9CE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B9EEC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BA098);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BA344);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BA514);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BAF1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BB230);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BB37C);

void func_800BB4FC(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
}

void func_800BB52C(s32 arg0) {
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

void func_800BB560(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *shadow = h->hero_shadow;
    EXTengine *eng;

    shadow->sub->unk6C = &D_80052FCC;
    shadow->sub->unk74 = D_800BC04C[arg0];
    eng = h->hero_eng;
    shadow->unk10 = eng->unk10;
    shadow->flags |= 0x8021;
    shadow->sub->unk76 = 0xE00;
}

s32 func_800BB5F0(s32 arg0) {
    if (!(D_80051338[arg0].unk54 & 0x4000)) {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, 0x34);
    return 1;
}

s32 func_800BB648(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x36;
    if (!(st & 0x200)) {
        act = 0x37;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_800BB6B4(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x36;
    if (!(st & 0x200)) {
        act = 0x37;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}
