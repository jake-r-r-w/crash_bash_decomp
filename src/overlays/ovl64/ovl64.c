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

void func_800B382C(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 y;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    Ovl64Cell *p;
    Ovl64Cell *base;
    s32 t0;
    s32 t1;

    i = 0;
    base = D_800BBE10[0];
    y = 0x500;
    for (; i < 4; i++) {
        j = 0;
        d = i << 7;
        for (; j < 16; j++) {
            t0 = j << 8;
            p = (Ovl64Cell *)(d + (s32)base);
            p->x = t0;
            p->y = y;
            d += 8;
        }
        y += 0x140;
    }
    for (k = 0; k < 300; k++) {
        i = Random(4);
        j = Random(16);
        c = Random(4);
        d = Random(16);
        t0 = D_800BBE10[i][j].x;
        t1 = D_800BBE10[i][j].y;
        D_800BBE10[i][j].x = D_800BBE10[c][d].x;
        D_800BBE10[i][j].y = D_800BBE10[c][d].y;
        D_800BBE10[c][d].x = t0;
        D_800BBE10[c][d].y = t1;
    }
}

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

s32 func_800B8C74(s32 arg0) {
    s32 flip = arg0 ^ 1;
    s32 best = 0x7FFFFF;
    u16 i = 0;
    u8 *g = D_8005A614;
    HeroInfo *base = Heroes;
    HeroInfo *me = base + arg0;
    HeroInfo *h;
    s32 dx;
    s32 dz;
    s32 d;
    EXTengine *a;
    EXTengine *b;

    for (; i < 4; i++) {
        if (i == arg0) {
            continue;
        }
        if (*(volatile s32 *)(g + 0x6C) != 0 && i == flip) {
            continue;
        }
        h = (HeroInfo *)(i * sizeof(HeroInfo) + (s32)base);
        if (h->hero_status & 0xFA00) {
            continue;
        }
        a = me->hero_eng;
        b = h->hero_eng;
        dx = a->unk10.x - b->unk10.x;
        dz = a->unk10.z - b->unk10.z;
        d = dx * dx + dz * dz;
        if (d < best) {
            best = d;
        }
    }
    return best != 0x7FFFFF ? csqrt(best) >> 6 : 0;
}

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

void func_800B8E64(u16 arg0, u16 arg1) {
    s32 i;
    Ovl64Pair *p;

    for (i = 0; i < arg0; i++) {
        p = func_800B4F7C(&D_800BBD5E);
        if (p != NULL) {
            func_800B51A0(p->a, p->b, arg1, -1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B8EE0);

s32 func_800B8F48(EXTengine *eng) {
    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;
    s32 limit;
    s32 ret = 0;
    s32 ang;

    if (eng->collision_box.flags & 0x40) {
        limit = 0x980 - eng->collision_box.radius;
    } else {
        limit = 0x980;
    }
    if (x * x + z * z >= limit * limit) {
        ang = get_angle(x, z);
        eng->unk10.x = limit * D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> 12;
        eng->unk10.z = limit * D_80068BD4[ang & 0xFFF].unk2 >> 12;
        ret = 1;
    }
    return ret;
}

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

void func_800B92BC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    D_800BC05C[arg0] = 0;
    base = Heroes;
    h = base + arg0;
    h->hero_hit = 0x1E;
    h->hero_pu_timer = 0;
    h->hero_move->velocity_enable = 0;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq((HeroInfo *)arg0, 0x2B);
}

void func_800B937C(s32 arg0) {
    HeroInfo *h;
    HeroInfo *h2;
    u16 *flag;
    u16 *fbase;
    s32 r;
    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_move->reactive_speed != 0) {
        D_8009D73C();
    }
    r = func_8008502C(arg0);
    if (r != 8) {
        if (r == 9) {
            PlaySfx(D_800BC054[arg0], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        }
    } else {
        D_800BC05C[arg0] = 1;
        if (h->hero_health <= 0 && !(D_8005A618 & 2)) {
            h->hero_immunity = 0;
            h->hero_hit += 0x28;
            SetHeroSeq((HeroInfo *)arg0, 0x2D);
        }
        Heroes[arg0].hero_health = 0x14;
    }
    fbase = D_800BC05C;
    flag = fbase + arg0;
    if (*flag != 0) {
        HeroInfo *base2 = Heroes;

        h2 = base2 + arg0;
        if (abs(((s32 (*)(EXTengine *))func_80025F44)(h2->hero_eng)) < 0x20) {
            *flag = 0;
            switch (h2->hero_name) {
            case 1:
                PlaySfx(0x11, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x43, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x75, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 4:
                PlaySfx(0xA7, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 7:
                PlaySfx(0x10B, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 8:
                PlaySfx(0xD9, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 5:
                PlaySfx(0x13D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 6:
                PlaySfx(0x16F, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            }
        }
    }
    if (D_8009D740 != NULL) {
        D_8009D740(arg0);
    }
    func_800871E4(arg0);
}

void func_800B9608(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *p;
    s32 idx;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    SetHeroSeq((HeroInfo *)arg0, 0x2C);
    idx = 0;
    if (D_8005A614[arg0 + 0x5C] == 0) {
        idx = h->hero_name;
    }
    h->hero_move->jump_vel = D_8009D6E8.unk10[idx].vel;
    h->hero_move->jump_max = D_8009D6E8.unk10[idx].max;
    h->hero_move->jump_acc = D_8009D6E8.unk10[idx].acc;
    h->hero_move->jump_dec = D_8009D6E8.unk10[idx].dec;
    h->hero_move->jump_dir = -1;
}

void func_800B971C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *eng;

    if (func_80025BBC(h->hero_eng) != 0) {
        SetHeroAction((HeroInfo *)arg0, 7);
    } else {
        eng = h->hero_eng;
        if (D_8005A5D8 < eng->unk10.y + h->hero_sub->unk08) {
            if (D_8005B5B8 == 0x7FFFFFFF) {
                SetHeroAction((HeroInfo *)arg0, 6);
            } else {
                eng->unk10.y = D_8005A5D8;
                h->hero_sub->unk08 = 0;
            }
        }
    }
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

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

void func_800B98B8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *base2;
    HeroInfo *h2;
    u16 st;
    s32 ret;

    base = Heroes;
    h = &base[arg0];
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    st = h->hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x36);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x37);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret != 0) {
        base2 = Heroes;
        h2 = &base2[arg0];
        if ((u32)(h2->hero_name - 1) < 8) {
            h2->hero_sub->unk6C = D_80052A54 + D_8009D6EC[*(volatile u16 *)&h2->hero_name] * 0x38;
        } else {
            h2->hero_sub->unk6C = 0;
        }
    } else {
        func_80089BE8(arg0);
    }
}

void func_800B9A04(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    HeroInfo *b2;
    s32 dy;

    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 1;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->unk6C = D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
    }
    {
        HeroInfo *h2;
        b2 = Heroes;
        h2 = b2 + arg0;
        h2->hero_sub->unk08 = 0;
        h2->hero_pu = 0;
        h2->hero_pu_timer = 0;
    }
    func_80085718(arg0, 0);
    SetHeroSeq((HeroInfo *)arg0, 0xC);
    func_80089FA4(arg0);
    D_800AD12C = 1;
}

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

void func_800B9CE0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    HeroInfo *base2;
    HeroInfo *h2;
    s32 dy;

    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 8;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->unk6C = (s32)D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
    }
    base2 = Heroes;
    h2 = base2 + arg0;
    h2->hero_eng->unk5C = 1;
    h2->hero_eng->unk2C.x = 0x1000;
    h2->hero_eng->unk2C.y = 0x1000;
    h2->hero_eng->unk2C.z = 0x1000;
    h2->hero_sub->unk08 = 0;
    h2->hero_pu = 0;
    h2->hero_pu_timer = 0;
    SetHeroSeq(arg0, 1);
    func_80026980(h2->hero);
}

void func_800B9EEC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base = Heroes;
    s32 dy;

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
    func_8008502C(arg0);
    {
        HeroInfo *b2 = Heroes;
        h = b2 + arg0;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    if (D_8005A644 & 0x800) {
        EXTengine *e = h->hero_eng;
        s32 v = e->unk2C.x;
        v -= 0x60;
        if (v > 0x300) {
            e->unk2C.x = v;
            h->hero_eng->unk2C.y = v;
            h->hero_eng->unk2C.z = v;
        } else {
            func_8007A524(D_800BB89C[arg0], &e->unk10.x, (s32)&D_80052FCC);
            h->hero_status |= 0x8000;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BA098);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BA344);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BA514);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BAF1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BB230);

void func_800BB37C(EXTengine *eng, Ovl64HitMask *m, u32 flag, s32 hero) {
    FlagRecord *r;
    FlagRecord *r2;
    HeroInfo *h;
    EXTengine *e2;
    s32 top, bot, dx, dz, rr, y, lo, hi;
    s32 rad;

    r = func_80025A48(eng, 0x40);
    if (r != NULL) {
        top = eng->unk10.y + r->unk2;
        rad = r->radius;
        h = &Heroes[hero];
        bot = eng->unk10.y + r->height;
        if (!(h->hero_status & 0xF800)) {
            e2 = h->hero_eng;
            if (e2 != NULL) {
                r2 = func_80025A48(e2, 0x40);
                if (r2 != NULL) {
                    lo = e2->unk10.y + r2->unk2;
                    hi = e2->unk10.y + r2->height;
                    if (bot < lo && hi < top) {
                        dx = e2->unk10.x - eng->unk10.x;
                        dz = e2->unk10.z - eng->unk10.z;
                        rr = r2->radius + rad;
                        rr *= rr;
                        if (dx * dx + dz * dz < rr && m != NULL && (m->mask & flag)) {
                            m->hit |= flag;
                        }
                    }
                }
            }
        }
    }
}

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
