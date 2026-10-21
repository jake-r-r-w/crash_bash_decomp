#include "common.h"
#include "bash/ovl5d.h"
#include "bash/ovl5d_800bff1c.h"
#include "bash/bash.h"
#include "bash/xtras.h"
#include "bash/papu.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800BFF1C);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C02D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C05A8);

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
            if (y < D_8005A5D8) {
                pu->unk10.y = y + 0x100;
                if (y + 0x100 >= D_8005A5D8) {
                    pu->unk10.y = D_8005A5D8;
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
            if (abs(D_8005A5D8 - pu->unk10.y) < 0x100) {
                Heroes[arg0].hero_eng->unk2C.y = 0x100;
                Heroes[arg0].hero_eng->unk10.y = D_8005A5D8;
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
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
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
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
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
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C1A30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C284C);

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
        func_80089BE8(arg0);
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
