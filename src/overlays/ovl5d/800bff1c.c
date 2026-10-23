#include "common.h"
#include "bash/ovl5d.h"
#include "bash/ovl5d_800bff1c.h"
#include "bash/bash.h"
#include "bash/xtras.h"
#include "bash/papu.h"

void func_800BFF1C(s32 arg0) {
    HeroInfo *b = Heroes;
    HeroInfo *h = &b[arg0];
    s32 sfx;

    if (h->hero_move->reactive_speed != 0) {
        D_8009D73C();
    }
    switch (func_8008502C(arg0)) {
    case 8:
        switch (h->hero_name) {
        case 1:
            sfx = 0xD;
            goto play8;
        case 2:
            sfx = 0x3F;
            goto play8;
        case 7:
            sfx = 0x107;
            goto play8;
        case 8:
            sfx = 0xD5;
            goto play8;
        case 5:
            sfx = 0x139;
            goto play8;
        case 6:
            sfx = 0x16B;
        play8:
            PlaySfx(sfx, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (Heroes[arg0].hero_health <= 0 && !(D_8005A618 & 2)) {
            Heroes[arg0].hero_immunity = 0;
            Heroes[arg0].hero_status |= 0x2000;
            Heroes[arg0].hero_hit += 0x28;
            SetHeroSeq((HeroInfo *)arg0, 0x13);
        }
        break;
    case 9:
        switch (h->hero_name) {
        case 3:
            sfx = 0x71;
            break;
        case 4:
            sfx = 0xA3;
            break;
        default:
            goto done;
        }
        PlaySfx(sfx, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
done:
    if (D_8009D740 != NULL) {
        D_8009D740(arg0);
    }
    func_800871E4(arg0);
}

void func_800C013C(s32 arg0) {
    VECTOR pos;
    HeroInfo *h;
    subengine *s;
    HeroInfo *b;
    s16 t;

    func_8008502C();
    b = Heroes;
    h = &b[arg0];
    s = h->hero_fx->sub;
    if (s != NULL && func_80015780(h->hero_eng, h->hero_sub, 0x40, &pos) != NULL) {
        h->hero_fx->unk10 = h->hero_eng->unk10;
        t = h->hero_weapon_timer;
        if (t >= 0) {
            if (D_800C4790[t * 3] == 0) {
                goto clear;
            }
            s->unk20.x = D_800C4790[t * 3];
            s->unk20.y = D_800C4790[t * 3 + 1];
            s->unk20.x = D_800C4790[t * 3 + 2];
            s->unk04 = pos.vx - h->hero_eng->unk10.x;
            s->unk08 = pos.vy - h->hero_eng->unk10.y;
            s->unk0C = pos.vz - h->hero_eng->unk10.z;
            s->flags |= 0x8000;
            s->unk52 = 1;
            h->hero_weapon_timer++;
        } else {
            h->hero_weapon_timer++;
        }
        goto done;
clear:
        s->flags &= ~0x8000;
    }
done:
    func_800871E4(arg0);
    D_8009D73C(arg0);
}

void func_800C02D4(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    subengine *s;
    HeroInfo *self;
    HeroInfo *base2;
    EXTobject *o;
    s32 v;
    u32 m;
    VECTOR pos;
    int new_var;

    if (func_8008AB50(arg0) != 0) {
        goto action;
    }
    base = Heroes;
    h = &base[arg0];
    o = h->hero_object;
    if (o == NULL) {
        goto none;
    }
    if (o->status != 2) {
        h->hero_object = NULL;
        if (h->hero_eng->sfx != NULL) {
            StopSfx(h->hero_eng->sfx);
        }
        goto rest;
    }
    if (o->can_do & 4) {
        if (D_8005A614[arg0 + 0x5C] != 0) {
            m = ((PadButtons *)D_80051338)[arg0].unk50 & 0x2000;
        } else {
            m = ((PadButtons *)D_80051338)[arg0].unk54 & 0x2000;
        }
        if (m != 0) {
        action:
            SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E0);
            return;
        }
    }
    goto rest;
none:
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    s = h->hero_fx->sub;
    if (s != NULL) {
        s->flags &= ~0x8000;
    }
    SetHeroAction((HeroInfo *)arg0, 0);
    return;
rest:
    if (func_8008ACF0(arg0) == 0) {
        D_8009D73C(arg0);
        func_8008502C(arg0);
        base2 = Heroes;
        self = &base2[arg0];
        s = self->hero_fx->sub;
        if (s != NULL && func_80015780(self->hero_eng, self->hero_sub, 0x40, &pos) != NULL) {
            self->hero_fx->unk10 = self->hero_eng->unk10;
            v = (D_80068BD4[(((D_8004E0D8 << 8) & 0xFFF) - 0x400) & 0xFFF].unk2 * 512 >> 12) + 0x1000;
            s->unk20.y = s->unk20.x = v;
            s->unk04 = pos.vx - self->hero_eng->unk10.x;
            s->unk08 = pos.vy - self->hero_eng->unk10.y;
            new_var = pos.vz - self->hero_eng->unk10.z;
            s->flags |= 0x8000;
            s->unk52 = 1;
            s->unk0C = new_var;
        }
        func_800871E4(arg0);
    }
}

void func_800C05A8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    subengine *s;
    HeroInfo *self;
    HeroInfo *base2;
    EXTobject *o;
    s32 v;
    VECTOR pos;
    int new_var;

    if (func_8008AB50(arg0) != 0) {
        goto action;
    }
    base = Heroes;
    h = &base[arg0];
    o = h->hero_object;
    if (o == NULL) {
        goto none;
    }
    if (o->status != 2) {
        h->hero_object = NULL;
        if (h->hero_eng->sfx != NULL) {
            StopSfx(h->hero_eng->sfx);
        }
        goto rest;
    }
    if ((o->can_do & 4) && (((PadButtons *)D_80051338)[arg0].unk50 & 0x2000)) {
    action:
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E0);
        return;
    }
    goto rest;
none:
    SetHeroAction((HeroInfo *)arg0, 0);
    return;
rest:
    if (func_8008AC74(arg0) == 0) {
        D_8009D73C(arg0);
        func_8008502C(arg0);
        base2 = Heroes;
        self = &base2[arg0];
        s = self->hero_fx->sub;
        if (s != NULL && func_80015780(self->hero_eng, self->hero_sub, 0x40, &pos) != NULL) {
            self->hero_fx->unk10 = self->hero_eng->unk10;
            v = (D_80068BD4[(((D_8004E0D8 << 8) & 0xFFF) - 0x400) & 0xFFF].unk2 * 512 >> 12) + 0x1000;
            s->unk20.y = s->unk20.x = v;
            s->unk04 = pos.vx - self->hero_eng->unk10.x;
            s->unk08 = pos.vy - self->hero_eng->unk10.y;
            new_var = pos.vz - self->hero_eng->unk10.z;
            s->flags |= 0x8000;
            s->unk52 = 1;
            s->unk0C = new_var;
        }
        func_800871E4(arg0);
    }
}

void func_800C07F4(s32 arg0) {
    VECTOR pos;
    HeroInfo *h;
    subengine *s;
    s16 *w;
    HeroInfo *b;
    HeroInfo *h0;
    HeroInfo *b0;
    s16 (*tbl)[3];
    s16 t;

    if (func_8008502C() == 0x13) {
        b0 = Heroes;
        h0 = &b0[arg0];
        if (h0->hero_object != NULL) {
            h0->hero_object->will_do |= 4;
            h0->hero_object = NULL;
            if (h0->hero_eng->sfx != NULL) {
                StopSfx(h0->hero_eng->sfx);
            }
        }
    }
    b = Heroes;
    h = &b[arg0];
    s = h->hero_fx->sub;
    if (s != NULL && func_80015780(h->hero_eng, h->hero_sub, 0x40, &pos) != NULL) {
        h->hero_fx->unk10 = h->hero_eng->unk10;
        t = h->hero_weapon_timer;
        if (t >= 0) {
            tbl = D_8009DC68;
            w = tbl[t];
            if (w[0] == 0) {
                goto clear;
            }
            s->unk20.x = w[0];
            s->unk20.y = w[1];
            s->unk20.x = w[2];
            s->unk04 = pos.vx - h->hero_eng->unk10.x;
            s->unk08 = pos.vy - h->hero_eng->unk10.y;
            s->unk0C = pos.vz - h->hero_eng->unk10.z;
            s->flags |= 0x8000;
            s->unk52 = 1;
            h->hero_weapon_timer++;
        } else {
            h->hero_weapon_timer++;
        }
        goto done;
clear:
        s->flags &= ~0x8000;
    }
done:
    func_800871E4(arg0);
    D_8009D73C(arg0);
}

void func_800C09F4(s32 arg0) {
    HeroInfo *h;
    EXTengine *p;
    HeroInfo *base = Heroes;

    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_move->velocity_calc = 9;
    h->hero_move->z_vel = 0;
    h->hero_move->x_vel = 0;
    h->hero_move->z_max = 0;
    h->hero_move->x_max = 0;
    h->hero_frozen = 0x3C;
    h->hero_status |= 0x2800;
    SetHeroSeq((HeroInfo *)arg0, 1);
    p = h->hero_powerup;
    p->unk10 = h->hero_eng->unk10;
    p->sub->unk74 = 0x2002;
    p->flags |= 0x8000;
    p->unk10.x = (p->unk10.x & ~0xFF) + 0x80;
    p->unk10.z = (p->unk10.z & ~0xFF) + 0x80;
    p->unk10.y -= 0x1000;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (D_800C4780 != NULL) {
        StopSfx(D_800C4780);
    }
    PlaySfx(0x453, NULL, NULL, 0x1000, 0x1E00, 0);
}

void func_800C0BAC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    EXTengine *pu;
    s32 y;

    if (func_8008ABF8(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        pu = h->hero_powerup;
        if ((u16)h->hero_frozen >= 3) {
            y = pu->unk10.y;
            if (y < World_floor_level) {
                pu->unk10.y = y + 0x100;
                if (y + 0x100 >= World_floor_level) {
                    pu->unk10.y = World_floor_level;
                    if (h->hero_powerup->sfx != NULL) {
                        StopSfx(h->hero_powerup->sfx);
                    }
                    if (D_800C4780 != NULL) {
                        StopSfx(D_800C4780);
                    }
                    PlaySfx(0x455, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                Heroes[arg0].hero_frozen = 0x1E;
            }
            if (abs(World_floor_level - pu->unk10.y) < 0x100) {
                Heroes[arg0].hero_eng->unk2C.y = 0x100;
                Heroes[arg0].hero_eng->unk10.y = World_floor_level;
            }
        } else {
            SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E2);
            if (h->hero_powerup->sfx != NULL) {
                StopSfx(h->hero_powerup->sfx);
            }
            if (D_800C4780 != NULL) {
                StopSfx(D_800C4780);
            }
            pu->flags &= 0xFFFF7FFF;
            h->hero_eng->unk2C.y = 0x1000;
        }
        pu->unk10.x = Heroes[arg0].hero_eng->unk10.x;
        pu->unk10.z = Heroes[arg0].hero_eng->unk10.z;
        Heroes[arg0].hero_eng->unk5E = 1;
        func_8008502C(arg0);
    }
}

void func_800C0DE4(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base2;

    if (Heroes[arg0].hero_pu == 0x14) {
        if (D_800C4780 != NULL) {
            StopSfx(D_800C4780);
        }
    }
    base2 = Heroes;
    h = base2 + arg0;
    h->hero_hit = 0;
    h->hero_immunity = 0;
    h->hero_move->z_vel = 0;
    h->hero_move->x_vel = 0;
    h->hero_move->z_max = 0;
    h->hero_move->x_max = 0;
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

void func_800C0F14(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h2;
    s32 dy;

    if (Heroes[arg0].hero_pu == 0x14 && D_800C4780 != NULL) {
        StopSfx(D_800C4780);
    }
    base = Heroes;
    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_move->z_vel = 0;
    h->hero_move->x_vel = 0;
    h->hero_move->z_max = 0;
    h->hero_move->x_max = 0;
    h->hero_frozen = 1;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_sub->flags &= ~0x21;
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
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    base2 = Heroes;
    h2 = base2 + arg0;
    h2->hero_sub->unk08 = 0;
    h2->hero_pu = 0;
    h2->hero_pu_timer = 0;
    func_80085718(arg0, 0);
    SetHeroSeq((HeroInfo *)arg0, 0xC);
    func_80089FA4(arg0);
}

void func_800C1190(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

void func_800C1278(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h2;
    s32 dy;

    if (Heroes[arg0].hero_pu == 0x14 && D_800C4780 != NULL) {
        StopSfx(D_800C4780);
    }
    base = Heroes;
    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_move->z_vel = 0;
    h->hero_move->x_vel = 0;
    h->hero_move->z_max = 0;
    h->hero_move->x_max = 0;
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
    h->hero_sub->unk6C = D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    base2 = Heroes;
    h2 = base2 + arg0;
    h2->hero_sub->unk08 = 0;
    h2->hero_pu = 0;
    h2->hero_pu_timer = 0;
    SetHeroSeq((HeroInfo *)arg0, 1);
    func_80026980(h2->hero);
}

void func_800C14D4(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    func_8008502C(arg0);
}

NAS *func_800C159C(player, mask, unused, cone)
    s32 player;
    s32 mask;
    s32 unused;
    u16 cone;
{
    VECTOR pos;
    s32 top;
    s32 bot;
    HeroInfo *heroes;
    HeroInfo *h;
    EXTengine *me;
    EXTengine *eng;
    FlagRecord *rec;
    FlagRecord *rec2;
    EXTobject *obj;
    NAS *nas;
    NAS *self;
    s32 rad;
    s32 lo;
    s32 hi;
    s32 dx;
    s32 dz;
    s32 r;

    heroes = Heroes;
    h = &heroes[player];
    me = h->hero_eng;
    if (me == NULL) {
        return NULL;
    }
    rec = func_80025A48(me, 0x400);
    if (rec == NULL) {
        return NULL;
    }
    if (me->unk5E != 0) {
        func_800142D4(&me->unk1C);
    }
    func_8001CD04(me->sub, me->unk3C, &me->unk10);
    bot = me->unk10.y + rec->unk2;
    top = me->unk10.y + rec->height;
    rad = rec->radius;
    func_80015758((SVECTOR *)rec, &pos);
    nas = D_8005AA2C;
    self = NULL;
    if (h->hero_object != NULL) {
        self = ((NAS **)h->hero_object)[-1];
    }
    for (; nas != NULL; nas = nas->Solid.Child) {
        if (!(nas->Class & 0x200) || nas == self) {
            continue;
        }
        eng = nas->Engine;
        if (eng == NULL || eng == me) {
            continue;
        }
        rec2 = func_80025A48(eng, 0x40);
        if (rec2 == NULL) {
            continue;
        }
        lo = eng->unk10.y + rec2->unk2;
        hi = eng->unk10.y + rec2->height;
        if (top < lo && hi < bot) {
            dx = eng->unk10.x - pos.vx;
            dz = eng->unk10.z - pos.vz;
            r = rec2->radius + rad;
            if (dx * dx + dz * dz < r * r) {
                obj = FindExtension(nas, 0x1000);
                if (obj != NULL && (obj->can_do & mask)) {
                    if (cone == 0) {
                        do {
                            obj->player = player;
                            obj->will_do |= mask;
                            return nas;
                        } while (0);
                    } else if (func_80015480(Heroes[player].hero_move->forward_angle,
                                             get_angle(eng->unk10.x - me->unk10.x, eng->unk10.z - me->unk10.z) ^ 0x800) < cone) {
                        do {
                            obj->player = player;
                            obj->will_do |= mask;
                            return nas;
                        } while (0);
                    }
                }
            }
        }
    }
    return NULL;
}

NAS *func_800C1848(idx, cone)
    s32 idx;
    u16 cone;
{
    NAS *n;
    HeroInfo *heroes;
    HeroInfo *h;
    EXTengine *he;
    EXTengine *e;
    u8 *fux;
    u16 ang;
    s32 hx, hz, dx, dz;
    s32 a;
    s32 lim = 0x10000;

    heroes = Heroes;
    n = D_8005AA24;
    h = &heroes[idx];
    ang = h->hero_move->facing_angle;
    he = h->hero_eng;
    hx = he->unk10.x - (D_80068BD4[(ang - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    hz = he->unk10.z - (D_80068BD4[ang & 0xFFF].unk2 * 1024 >> 14);
    for (; n != NULL; n = n->Object.Child) {
        fux = (u8 *)n->FUX;
        if (fux == NULL) continue;
        if (!(*(u16 *)(fux + 0x1C) & 1)) continue;
        e = n->Engine;
        if (e == NULL) continue;
        if (e->platform != NULL) continue;
        dx = e->unk10.x - hx;
        dz = e->unk10.z - hz;
        if (dx * dx + dz * dz >= lim) continue;
        if (cone != 0) {
            a = get_angle(e->unk10.x - he->unk10.x, e->unk10.z - he->unk10.z) ^ 0x800;
            if (func_80015480(Heroes[idx].hero_move->forward_angle, a) < cone) {
                *(s16 *)(fux + 0x16) = idx;
                *(u16 *)(fux + 0x1E) |= 1;
                Heroes[idx].hero_move->forward_angle = a;
                return n;
            }
        } else {
            *(s16 *)(fux + 0x16) = idx;
            *(u16 *)(fux + 0x1E) |= 1;
            return n;
        }
    }
    return NULL;
}

static inline void func_800C1A30_bounce(s32 *a, s32 *b, s16 ma, s32 *c, s32 *d, s16 mb) {
    s32 total;
    s32 r;
    s32 q;
    s32 dd;
    s32 x;
    s32 y;

    if (ma > 0 && mb > 0) {
        x = *a;
        y = *c;
        dd = -((x - y) << 8);
        q = (dd * mb + ((ma * x + mb * y) << 8)) / (total = mb + ma);
        r = q - dd;
        *a = q >> 8;
        *c = r >> 8;
        x = *b;
        y = *d;
        dd = -((x - y) << 8);
        q = (dd * mb + ((ma * x + mb * y) << 8)) / total;
        r = q - dd;
        *b = q >> 8;
        *d = r >> 8;
    }
}

#define BOUNCE(m1, m2) \
    func_800C1A30_bounce(&(m1)->x_vel, &(m1)->z_vel, (m1)->mass, &(m2)->x_vel, &(m2)->z_vel, (m2)->mass)

s32 func_800C1A30(s32 arg0) {
    VECTOR pos;
    NAS *own;
    EXTengine *obj;
    s32 hit;
    s32 bot;
    s32 top;
    s32 rad;
    s32 lo;
    s32 hi;
    s32 rad2;
    s32 hi2;
    s32 lo2;
    s32 rad3;
    s32 lo3;
    s32 hi3;
    u32 i;
    HeroInfo *self;
    HeroInfo *heroes;
    EXTengine *eng;
    EXTengine *e;
    FlagRecord *rec;
    FlagRecord *box;
    NAS *n;
    EXTobject *t;
    s32 push;
    s32 dx;
    s32 r;
    s32 d;
    s32 dz;
    s32 rr;
    s32 ang;
    s32 a;
    s32 tmp;

    hit = 0;
    lo2 = 0;
    hi2 = 0;
    rad2 = 0;
    hi3 = 0;
    lo3 = 0;
    rad3 = 0;
    eng = Heroes[arg0].hero_eng;
    obj = NULL;
    if (eng != NULL) {
        box = func_80025A48(eng, 0x40);
        if (box != NULL) {
            rad = box->radius;
            hi = eng->unk10.y + box->unk2;
            lo = eng->unk10.y + box->height;
            box = func_80025A48(eng, 1);
            if (box != NULL) {
                if (eng->unk5E != 0) {
                    func_800142D4(&eng->unk1C);
                }
                func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
                lo2 = eng->unk10.y + box->unk2;
                hi2 = eng->unk10.y + box->height;
                if (lo2 < hi2) {
                    tmp = hi2;
                    hi2 = lo2;
                    lo2 = tmp;
                }
                rad2 = box->radius;
                func_80015758((SVECTOR *)box, &pos);
            }
            if (Heroes[arg0].hero_object != NULL) {
                n = ((NAS **)Heroes[arg0].hero_object)[-1];
                obj = n->Engine;
                if (obj != NULL) {
                    rad3 = 0x80;
                    hi3 = obj->unk10.y;
                    lo3 = hi3 - 0x100;
                }
            }
            heroes = Heroes;
            i = 0;
            self = &heroes[arg0];
            for (; ; i++) {
                s32 unused;

                if (i >= 4) {
                    break;
                }
                if (arg0 == i || (Heroes[i].hero_status & 0xF800)) {
                    continue;
                }
                if (self->hero_move->forward_speed != 0) {
                    push = 1;
                } else {
                    push = Heroes[i].hero_move->forward_speed == 0;
                }
                if (D_8005A5DC < 4) {
                    if (D_8005A5DC >= 2) {
                        push = 1;
                    }
                }
                e = Heroes[i].hero_eng;
                if (e == NULL) {
                    continue;
                }
                rec = func_80025A48(e, 0x40);
                if (rec == NULL) {
                    continue;
                }
                bot = e->unk10.y + rec->unk2;
                top = e->unk10.y + rec->height;
                if (push != 0) {
                    r = rec->radius;
                    r += rad;
                    rr = r * r;
                    if (lo < bot && top < hi) {
                        dx = e->unk10.x - eng->unk10.x;
                        dz = e->unk10.z - eng->unk10.z;
                        d = dx * dx + dz * dz;
                        if (d < rr) {
                            d = csqrt(d) >> 6;
                            if (d < r) {
                                ang = get_angle(dx, dz) & 0xFFF;
                                d = r - d + 1;
                                eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                eng->unk10.z -= (d * D_80068BD4[ang].unk2) >> 12;
                                hit = 1;
                                if (self->hero_pu != 0) {
                                    (*(void (**)(s32, s32))&D_8009D6E8.unk60)(arg0, i);
                                }
                                if (Heroes[i].hero_pu != 0) {
                                    (*(void (**)(s32, s32))&D_8009D6E8.unk60)(i, arg0);
                                }
                                if (D_8005A5DC < 4) {
                                    if (D_8005A5DC >= 2) {
                                        BOUNCE(self->hero_move, Heroes[i].hero_move);
                                    }
                                }
                            }
                        }
                    }
                }
                if (rad2 != 0 && hi2 < bot && top < lo2) {
                    dx = e->unk10.x - pos.vx;
                    dz = e->unk10.z - pos.vz;
                    r = rec->radius;
                    r += rad2;
                    d = dx * dx + dz * dz;
                    rr = r * r;
                    if (d < rr) {
                        d = csqrt(d) >> 6;
                        if (d < r) {
                            hit = 1;
                            ang = get_angle(dx, dz) & 0xFFF;
                            d = r - d + 1;
                            if (push != 0) {
                                eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                eng->unk10.z -= (d * D_80068BD4[ang].unk2) >> 12;
                            } else {
                                e->unk10.x += (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                e->unk10.z += (d * D_80068BD4[ang].unk2) >> 12;
                            }
                            if (D_8005A5DC < 4) {
                                if (D_8005A5DC >= 2) {
                                    BOUNCE(self->hero_move, Heroes[i].hero_move);
                                }
                            }
                        }
                    }
                }
                if (rad3 != 0 && lo3 < bot && top < hi3) {
                    dx = e->unk10.x - obj->unk10.x;
                    dz = e->unk10.z - obj->unk10.z;
                    r = rec->radius;
                    r += rad3;
                    d = dx * dx + dz * dz;
                    rr = r * r;
                    if (d < rr) {
                        d = csqrt(d) >> 6;
                        if (d < r) {
                            hit = 1;
                            ang = get_angle(dx, dz) & 0xFFF;
                            d = r - d + 1;
                            if (push != 0) {
                                eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                eng->unk10.z -= (d * D_80068BD4[ang].unk2) >> 12;
                            } else {
                                e->unk10.x += (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                e->unk10.z += (d * D_80068BD4[ang].unk2) >> 12;
                            }
                            if (D_8005A5DC < 4) {
                                if (D_8005A5DC >= 2) {
                                    BOUNCE(self->hero_move, Heroes[i].hero_move);
                                }
                            }
                        }
                    }
                }
            }
            own = NULL;
            n = D_8005AA2C;
            if (Heroes[arg0].hero_object != NULL) {
                own = ((NAS **)Heroes[arg0].hero_object)[-1];
            }
            rad = 0x78;
            push = Heroes[arg0].hero_move->forward_speed != 0;
            if (D_8005A5DC < 4) {
                if (D_8005A5DC >= 2) {
                    push = 1;
                }
            }
            for (; n != NULL; n = n->Solid.Child) {
                if (!(n->Class & 0x200)) continue;
                if (n == own) continue;
                e = n->Engine;
                if (e == NULL) continue;
                if (e == eng) continue;
                rec = func_80025A48(e, 0x40);
                if (rec == NULL) continue;
                bot = e->unk10.y + rec->unk2;
                top = e->unk10.y + rec->height;
                if (lo < bot && top < hi) {
                    dx = e->unk10.x - eng->unk10.x;
                    dz = e->unk10.z - eng->unk10.z;
                    r = rec->radius;
                    r += rad;
                    d = dx * dx + dz * dz;
                    rr = r * r;
                    if (d < rr) {
                        d = csqrt(d) >> 6;
                        if (d < r) {
                            hit = 1;
                            a = get_angle(dx, dz);
                            ang = a & 0xFFF;
                            if (!(a & 0xFF)) {
                                ang -= 8 - Random(0x10);
                            }
                            d = r - d + 1;
                            if (push != 0) {
                                eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                eng->unk10.z -= (d * D_80068BD4[ang & 0xFFF].unk2) >> 12;
                            }
                            t = FindExtension(n, 0x1000);
                            if (t != NULL && (t->can_do & 8)) {
                                t->player = arg0;
                                t->will_do |= 8;
                            }
                        }
                    }
                }
            }
        }
    }
    return hit;
}

s32 func_800C284C(s32 idx) {
    VECTOR v;
    subengine *sub;
    s32 hit;
    s32 top;
    s32 bot;
    EXTengine *eng;
    EXTengine *other;
    FlagRecord *box;
    FlagRecord *rec;
    NAS *nas;
    NAS *self;
    EXTobject *ext;
    s32 dx;
    s32 dz;
    s32 r;
    s32 d;
    s32 ang;
    s32 lo;
    s32 hi;

    eng = Heroes[idx].hero_eng;
    hit = 0;
    if (eng != NULL) {
        box = func_80025A48(eng, 0x40);
        if (box != NULL) {
            bot = eng->unk10.y + box->unk2;
            top = eng->unk10.y + box->height;
            box = func_80025A48(eng, 1);
            if (box != NULL) {
                if (eng->unk5E != 0) {
                    func_800142D4(&eng->unk1C);
                    sub = eng->sub;
                    eng->sub = sub;
                    func_8001CD04(sub, eng->unk3C, &eng->unk10);
                } else {
                    sub = eng->sub;
                    eng->sub = sub;
                    func_8001CD04(sub, eng->unk3C, &eng->unk10);
                }
                func_80015758((SVECTOR *)box, &v);
            }
            nas = D_8005AA2C;
            self = NULL;
            if (Heroes[idx].hero_object != NULL) {
                self = ((NAS **)Heroes[idx].hero_object)[-1];
            }
            if (nas != NULL) {
                do {
                    if ((nas->Class & 0x200) && nas != self) {
                        other = nas->Engine;
                        if (other != NULL && other != eng) {
                            ext = FindExtension(nas, 0x1000);
                            if (ext != NULL && (ext->status == 1 || ext->status == 0xB)) {
                                rec = func_80025A48(other, 0x40);
                                if (rec != NULL) {
                                    lo = other->unk10.y + rec->unk2;
                                    hi = other->unk10.y + rec->height;
                                    if (top < lo && hi < bot) {
                                        dx = other->unk10.x - eng->unk10.x;
                                        dz = other->unk10.z - eng->unk10.z;
                                        r = rec->radius;
                                        r += 0x78;
                                        d = dx * dx + dz * dz;
                                        if (d < r * r) {
                                            d = csqrt(d) >> 6;
                                            if (d < r) {
                                                ang = get_angle(dx, dz) & 0xFFF;
                                                d = r - d + 1;
                                                eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                eng->unk10.z -= (d * D_80068BD4[ang].unk2) >> 12;
                                                hit = 1;
                                                if (ext->can_do & 8) {
                                                    ext->player = idx;
                                                    ext->will_do |= 8;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    nas = nas->Solid.Child;
                } while (nas != NULL);
            }
        }
    }
    return hit;
}

void *func_800C2B60(s32 idx, s32 mask) {
    NAS *n;
    EXTengine *he;
    EXTengine *e;
    EXT *fux;
    s32 hx, hz, dx, dz, lim, m;

    n = D_8005AA24;
    m = mask;
    lim = 0x10000;
    he = Heroes[idx].hero_eng;
    hx = he->unk10.x - (D_80068BD4[(he->unk1E - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    hz = he->unk10.z - (D_80068BD4[he->unk1E & 0xFFF].unk2 * 1024 >> 14);
    for (; n != NULL; n = n->Object.Child) {
        fux = n->FUX;
        if (fux == NULL) continue;
        if (!(*(u16 *)((u8 *)fux + 0x1C) & m)) continue;
        e = n->Engine;
        if (e == NULL) continue;
        dx = e->unk10.x - hx;
        dz = e->unk10.z - hz;
        if (dx * dx + dz * dz < lim) return fux;
    }
    return 0;
}

void func_800C2C70(s32 *a, s32 *b, s16 ma, s32 *c, s32 *d, s16 mb) {
    s32 total;
    s32 r;
    s32 q;
    s32 dd;
    s32 x;
    s32 y;

    if (ma > 0 && mb > 0) {
        x = *a;
        y = *c;
        dd = -((x - y) << 8);
        q = (dd * mb + ((ma * x + mb * y) << 8)) / (total = mb + ma);
        r = q - dd;
        *a = q >> 8;
        *c = r >> 8;
        x = *b;
        y = *d;
        dd = -((x - y) << 8);
        q = (dd * mb + ((ma * x + mb * y) << 8)) / total;
        r = q - dd;
        *b = q >> 8;
        *d = r >> 8;
    }
}

void func_800C2DAC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    if (func_8008ABF8(arg0) != 0) {
        base = Heroes;
        h = base + arg0;
        if ((u16)(h->hero_name - 1) < 8) {
            h->hero_sub->unk6C = D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
        } else {
            h->hero_sub->unk6C = NULL;
        }
    } else {
        HeroGhost(arg0);
    }
}

void func_800C2E58(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0x12);
    if (h->hero_object != NULL) {
        h->hero_object->can_do |= 0x20;
    }
    h->hero_weapon_timer = 0;
}

void func_800C2EC8(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x11);
}

void func_800C2F14(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x10);
}

void func_800C2F60(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xF);
    h->hero_weapon_timer = 0;
}

s32 func_800C2FB0(s32 arg0) {
    u32 m;

    if (D_8005A614[arg0 + 0x5C] != 0) {
        return 0;
    }
    m = D_80051338[arg0].unk54 & 0x2000;
    return m != 0;
}
