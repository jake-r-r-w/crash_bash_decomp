#include "common.h"
#include "bash/main.h"
#include "bash/ovl62.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

s32 func_800B3740(s32 arg0) {
    s32 start;
    s32 r;
    s32 i;
    s32 j;
    s32 k;
    s32 total;
    s32 result;
    s32 v;
    s32 neg;

    start = Random(4);
    r = Random(0x1000);
    neg = -1;
    i = start;
    while (1) {
        if (i >= start + 4) {
            break;
        }
        if ((i & 3) == arg0) {
            goto next;
        }
        j = i & 3;
        result = 0;
        total = 0;
        if (Heroes[j].hero_status & 0x2000) {
            v = -1;
        } else {
            for (k = 0; k < 4; k++) {
                if (k != arg0 && !(Heroes[k].hero_status & 0x2000)) {
                    total += D_8009D87C[k].unk4 << 7;
                }
            }
            if (total != 0) {
                result = (D_8009D87C[j].unk4 << 19) / total;
            }
            v = result;
        }
        if (r < v) {
            return neg = i & 3;
        }
    next:
        i++;
    }
    return neg;
}

void func_800B38A0(void) {
    s32 i;
    SceneObj *obj;

    i = 0;
    D_8009E1F8 = 0;
    D_8009E1F4 = 0;
    D_800D5298 = NULL;
    D_800D529C = NULL;
    D_800D5FC8 = 0;
    obj =((const Ovl62Scene *)D_80058AD0)->objs;
    D_800D529C = NULL;
    D_800D52A0 = NULL;
    D_80058AF4.vx = 0;
    D_80058AF4.vz = 0;
    do {
        Heroes[i].hero_eng->flags |= 0x200000;
        i++;
    } while (i < 4);
    while (1) {
        if (obj == NULL) {
            break;
        }
        if (obj->flags & 0x8000) {
            if (obj->model->colors != NULL) {
                func_80015A18(obj->index, (u8 *)obj->model->colors);
                if (((Ovl62ModeView *)D_8005A614)->mode == 0x12) {
                    if (!(((Ovl62ModeView *)D_8005A614)->flags & 0x80)) {
                        if (obj->index == 0x5001 || obj->index == 0x5004) {
                            obj->flags |= 0x200000;
                        }
                    } else {
                        if (obj->index == 0x5001 || obj->index == 0x5004) {
                            obj->flags |= 0x200000;
                        }
                        if (obj->index == 0x5005) {
                            D_800D529C = obj;
                        }
                        if (obj->index == 0x5003) {
                            D_800D52A0 = obj;
                            D_800D5FC4 = obj->unk8;
                        }
                    }
                }
                if (((Ovl62ModeView *)D_8005A614)->mode == 0x16 &&obj->index == 0x5003) {
                    D_800D5298 = (subengine *)obj;
                    obj->flags |= 0x200000;
                }
            }
        }
        obj = obj->next;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B3AA0);

s32 func_800B4094(s32 *arg0, s32 arg1) {
    MATRIX m;
    SVECTOR rot;
    VECTOR v;
    VECTOR *vp;
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 cnt;
    s32 ry;

    base = *(u8 **)((u8 *)D_80058AD0 + 0x10);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        cnt = 0;
        vp = &v;
        a2 = p + 0x50;
        a3 = p;
        ry = ((((get_angle(arg0[0], arg0[2]) - 0x7D) & 0xFFF) >> 8) + 1 & 0xF) << 8;
        t = D_80068BD4;
        n = *(s32 *)(p + 0x38);
        for (;;) {
            s32 ang;

            if (i >= n) {
                break;
            }
            ang = *(s32 *)(a3 + 0x64) - ry;
            v.vx = *(s32 *)(a2 - 8);
            v.vz = *(s32 *)a2;
            v.vy = 0;
            rot.vx = 0;
            rot.vy = ry;
            rot.vz = 0;
            RotMatrix(&rot, &m);
            ApplyMatrixLV(&m, vp, vp);
            v.vx = arg0[0] - v.vx;
            v.vz = arg0[2] - v.vz;
            ang = -ang;
            if (((v.vx * t[ang & 0xFFF].unk2) >> 12) - ((v.vz * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                cnt++;
            }
            a3 += 0x38;
            i++;
            a2 += 0x38;
        }
        if (cnt == n) {
            return 0;
        }
    }
    return 1;
}

s32 func_800B4260(s32 *arg0, s32 arg1) {
    s32 v[3];
    Ovl62Link *l;
    u32 sec;
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 ang;

    if (D_8005A648 == 0x14 && D_800D5254 != NULL) {
        l = D_800D5254->unk58;
        if (l != NULL) {
            sec = (u32)((get_angle(arg0[0], arg0[2]) - 0x7D) & 0xFFF) >> 8;
            if (l->sectors[sec].state == 1) {
                arg1 = 1;
            }
            if (l->sectors[sec].state == 3 && l->sectors[sec].val >= 0x72 && !func_800B4094(arg0, 2)) {
                return 0;
            }
            if (l->sectors[sec].state == 2 && l->sectors[sec].val >= 0x72 && !func_800B4094(arg0, 3)) {
                return 0;
            }
            if (l->sectors[sec].state == 4 && l->sectors[sec].val >= 0x72) {
                if (!func_800B4094(arg0, 2)) {
                    return 0;
                }
                if (!func_800B4094(arg0, 3)) {
                    return 0;
                }
            }
        }
    }
    base = *(u8 **)((u8 *)D_80058AD0 + 0x10);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    i = 0;
    if (p != NULL) {
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
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                return 0;
            }
            a3 += 0x38;
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl62/ovl62", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B44B4);

s32 func_800B4A1C(NAS *nas)
{
  s32 *ext;
  HeroInfo *hero;
  u32 i;
  ext = FindExtension(nas, 0x10000000);
  i = 0;
  if (ext == 0)
  {
    return 0;
  }
  hero = Heroes;
  loop:
  if (i < 4)
  {
 do { if (!(hero->hero_status & 0x2000)) { s32 dx; s32 dz; EXTengine *a; EXTengine *b; a = nas->Engine; b = hero->hero_eng; dx = a->unk10.x - b->unk10.x; dz = a->unk10.z - b->unk10.z; if (((dx * dx) + (dz * dz)) > 0x15F8F) { goto next; } if (FindExtension(hero->hero, 0x20000000) != 0) { ext[5] = i; } return 1; } next: hero++; i++; } while (0);
    goto loop;
  }

  return 0;
}

void InitPolarBomb(NAS *m) {
    PolarBombExt *ext;
    EXTengine *e;

    ext = AddExtension(m, 0x84, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk74 = 0x201A;
        m->Engine->sub->unk6C = D_800546C4;
        m->Engine->unk5D = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk30 = 2;
        ext->unk74 = 0x56;
        ext->unk34 = 0;
        ext->unk48 = 0;
        ext->unk10 = 0;
        ext->unkC = 0;
        m->FUX = NULL;
        e = AddEngine(m, 2);
        if (e != NULL) {
            e->sub->unk6C = D_800546C4;
            e->sub->unk74 = 0x202F;
            e->Engine->unk5C = 1;
            e->Engine->unk5D = 0;
            e->Engine->unk2C.x = 0xC00;
            e->Engine->unk2C.y = 0xC00;
            e->Engine->unk2C.z = 0xC00;
            e->unk10 = m->Engine->unk10;
            e->flags |= 0x8041;
            e->sub->unk76 = 0x200;
            e->sub->flags |= 0x40000000;
            m->FUX = (EXT *)e;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotPolarBomb);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotBombGFX);

void func_800B5570(s32 player) {
    HeroInfo *base;
    HeroInfo *h;

    base = Heroes;
    h = &base[player];
    if (h->hero_frozen == 0) {
        if (!(h->hero_ctrl & 1)) {
            if (h->hero_mode != 0x52) {
                h->hero_eng->unk1E += func_800154A0(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
            } else {
                h->hero_eng->unk1E += func_800154A0(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 5;
            }
        }
        Heroes[player].hero_ctrl &= ~1;
        Heroes[player].hero_eng->unk5E = 1;
    }
}

void func_800B5684(s32 player) {
    EXTheromove *move = Heroes[player].hero_move;
    s32 *ext;

    if (move != NULL) {
        switch (D_8005A614[player + 0x50]) {
        case 1:
            move->mass = 0x100;
            break;
        case 2:
            move->mass = 0x100;
            break;
        case 3:
            move->mass = 0x100;
            break;
        case 4:
            move->mass = 0x100;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            move->mass = 0x180;
            break;
        }
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL && ext[9] == 1) {
            move->mass -= 0x46;
        }
    }
}

void func_800B5788(s32 arg0) {
    EXTengine *e = Heroes[arg0].hero_eng;
    EXTheromove *m = Heroes[arg0].hero_move;
    s32 step;

    D_8009D714();
    func_800860D4(arg0);
    if (m->velocity_calc == 3) {
        if (m->x_vel != m->x_max) {
            if (m->x_max != 0) {
                step = m->x_acc;
            } else {
                step = m->x_dec;
            }
            if (m->x_vel < m->x_max) {
                m->x_vel += step;
                if (m->x_vel > m->x_max) {
                    m->x_vel = m->x_max;
                }
            } else {
                m->x_vel -= step;
                if (m->x_vel < m->x_max) {
                    m->x_vel = m->x_max;
                }
            }
        }
        if (m->z_vel != m->z_max) {
            if (m->z_max != 0) {
                step = m->z_acc;
            } else {
                step = m->z_dec;
            }
            if (m->z_vel < m->z_max) {
                m->z_vel += step;
                if (m->z_vel > m->z_max) {
                    m->z_vel = m->z_max;
                }
            } else {
                m->z_vel -= step;
                if (m->z_vel < m->z_max) {
                    m->z_vel = m->z_max;
                }
            }
        }
        e->unk10.z += m->z_vel;
        e->unk10.x += m->x_vel;
    }
}

void func_800B5910(void) {
    SLONG_XYZ pos;
    HeroInfo *hero;
    NAS *nas;
    ExtSpawn5910 *ext;
    u8 *tex;
    s32 i;
    s32 start;
    SLONG_XYZ *p;
    s32 n;
    u16 status;

    i = Random(4);
    start = i;
    while (1) {
        if (i >= start + 4) {
            break;
        }
        hero = &Heroes[i & 3];
        status = hero->hero_status;
        if (status & 0x2000) {
            goto next;
        }
        if (status & 0x200) {
            goto next;
        }
        if (D_800D4D08 != 0) {
            hero->hero_status = status | 0x2000;
            pos.x = hero->hero_eng->unk10.x;
            pos.y = hero->hero_eng->unk10.y - 0x80;
            pos.z = hero->hero_eng->unk10.z;
            tex = D_800546C4;
            p = &pos;
            nas = SpawnNASById(pos.x, p->y, p->z, 0x1605, 0x11);
            n = 5;
            if (nas != NULL) {
                ext = FindExtension(nas, 0x10000000);
                if (ext != NULL) {
                    ext->unkC = n;
                    ext->unk18 = 0x100;
                    if (tex != NULL) {
                        ext->unk20 = n;
                        nas->Engine->sub->unk6C = tex;
                    } else {
                        ext->unk20 = 1;
                    }
                    ext->unk28 = 0;
                    nas->InitDelete(nas);
                }
            }
            do {
                SetHeroAction((HeroInfo *)(i & 3), 0x50);
            } while (0);
        } else {
            D_800D4D08 = 1;
            do {
                SetHeroAction((HeroInfo *)(i & 3), 0x58);
            } while (0);
        }
    next:
        i++;
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl62/ovl62", D_800B3358);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B5AC4);

void func_800B5EBC(player)
s32 player;
{
    HeroInfo *h;
    HeroInfo *h2;
    Ovl62Fux *fux;
    u32 pad;
    u16 ang;
    u16 t;
    EXTbotai *ai;
    s32 off;
    HeroInfo *base;

    base = Heroes;
    off = player * sizeof(HeroInfo);
    h = (HeroInfo *)(off + (u8 *)base);
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux == NULL) {
        return;
    }
    h->hero_ai->AIpad |= Ovl62PadToward(&fux->pos, &fux->vel);
    if (h->hero_frozen == 0) {
        ((Ovl62PadAng *)D_80051338)[player].buttons = h->hero_ai->AIpad;
    }
    ai = h->hero_ai;
    t = ai->AItimer1--;
    if (t == 0) {
        pad = h->hero_ai->AIpad;
        ang = 0;
        if (pad & 0x80) {
            ang = 0xC00;
        }
        if (pad & 0x20) {
            ang = 0x400;
        }
        if (pad & 0x10) {
            ang = 0;
        }
        if (pad & 0x40) {
            ang = 0x800;
        }
        if ((pad & 0x90) == 0x90) {
            ang = 0xE00;
        }
        if ((pad & 0xC0) == 0xC0) {
            ang = 0xA00;
        }
        if ((pad & 0x30) == 0x30) {
            ang = 0x200;
        }
        if ((pad & 0x60) == 0x60) {
            ang = 0x600;
        }
        h2 = (HeroInfo *)(off + (s32)base);
        h2->hero_ai->AIang = ang;
        h2->hero_ai->AItimer1 = fux->unk3C;
    }
    if (Heroes[player].hero_frozen == 0) {
        ((Ovl62PadAng *)D_80051338)[player].angle = Heroes[player].hero_ai->AIang;
    }
}

void func_800B60B8(s32 player) {
    HeroInfo *base;
    HeroInfo *h;
    Ovl62Fux *fux;
    Ext20 *ext;
    s32 t;

    base = Heroes;
    h = base + player;
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (func_800BCF08(player, fux->target) != 0) {
        return;
    }
    func_8008CEB0((u8 *)&base[fux->target].hero_eng->unk10, (u8 *)&h->hero_eng->unk10);
    if (func_800B7B88(player) == 0) {
        return;
    }
    ext = FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        return;
    }
    if (ext->val < func_800D05A8(player)) {
        return;
    }
    if (h->hero_move == NULL) {
        return;
    }
    if (fux->timer-- > 0) {
        return;
    }
    if (h->hero_ai->AIpad & 0x8000) {
        return;
    }
    if (D_8009D87C[player].unk0 > 0x20) {
        D_8009D87C[player].unk0 = 0x20;
    }
    t = D_800D52E0[D_8009D87C[player].unk0];
    h->hero_ai->AIpad |= 0x8000;
    fux->timer = 0x40 - t;
}

void func_800B6258(s32 player) {
    s32 dist;
    HeroInfo *base;
    HeroInfo *h;
    Ext20 *ext;
    s32 target;

    base = Heroes;
    h = base + player;
    ext = FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        return;
    }
    if (h->hero_mode != 0x4E) {
        return;
    }
    target = func_800CB178(player, &dist);
    if (target == -1 && h->hero_anim->Aindex == 0x1E) {
        ext->unk14 = 8;
        return;
    }
    if (dist <= 0x3FFFF && func_800B7A58(player, target) != 0) {
        Ovl62RollOdds(player, ext);
    }
    if (Heroes[player].hero_anim->Aindex == 0x1E) {
        Ovl62RollOdds(player, ext);
    }
}

void func_800B649C(s32 player) {
    Ovl62Fux *fux;
    s32 i;
    s32 n;
    SLONG_XYZ pos;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    HeroInfo *p;
    s32 best;
    s32 min;

    fux = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    for (n = i = 0, p = Heroes; i < 4; i++, p++) {
        if (p->hero_status & 0x2000) {
            n++;
        }
        fux->state = 5;
        if (n >= 3) {
            return;
        }
    }
    fux->target = Random(4);
    n = 0;
    fux->unk3C = 1;
    for (;;) {
        if (func_800BCF08(player, fux->target) != 0) {
            n++;
            fux->target = (fux->target + 1) & 3;
        }
        if (fux->target != player && !(Heroes[fux->target].hero_status & 0x2000)) {
            break;
        }
        fux->target = (fux->target + 1) & 3;
        n++;
        if (((Ovl62GameView *)D_8005A614)->unk6C != 0 && n >= 3) {
            fux->state = 5;
            fux->func = func_800CB6B4;
            return;
        }
    }
    if (!(fux->unk34 & 2)) {
        return;
    }
    best = -1;
    i = 0;
    base = Heroes;
    me = &base[player];
    h = base;
loop:
    do {
        if (i < 4) {
            min = 0x07FFFFFF;
            if (i != player && func_800BCF08(player, i) == 0 && !(h->hero_status & 0x2000)) {
                pos = h->hero_eng->unk10;
                if (func_8008CEB0((u8 *)&pos, (u8 *)&me->hero_eng->unk10) < min) {
                    best = i;
                }
            }
            h++;
            i++;
            goto loop;
        }
    } while (0);
    if (best == -1) {
        fux->state = 5;
        fux->func = func_800CB6B4;
        return;
    }
    fux->target = best;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B6708);

void func_800B6D74(s32 player) {
    HeroInfo *base;
    HeroInfo *h;
    Ovl62Fux *fux;
    s32 dist;
    s32 t;

    if (func_800B7CDC() != 0) {
        return;
    }
    if (D_8005A648 == 0x15) {
        if (func_800CB56C(player) != 0) {
            t = func_800CB178(player, &dist);
            if (t != -1) {
                func_800B6EB4(player, t);
            }
        }
    }
    base = Heroes;
    h = &base[player];
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux != NULL) {
        fux->unk3C = 1;
        if (D_800D5254 != NULL) {
            fux->vel = D_800D5254->unk54->unk10;
        }
        fux->pos = h->hero_eng->unk10;
        if (fux->unk24-- < 0) {
            fux->state = 5;
            fux->func = func_800CB6B4;
            fux->unk34 &= ~1;
        }
        func_800B60B8(player);
        func_800B5EBC(player);
    }
}

s32 func_800B6EB4(s32 player, s32 target) {
    HeroInfo *base;
    HeroInfo *h;
    Ovl62Fux *fux;
    EXTengine *a;
    EXTengine *b;
    s32 dx;
    s32 dz;

    base = Heroes;
    h = &base[player];
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux != NULL) {
        if (func_800BCF08(player, target) != 0) {
            return 0;
        }
        a = h->hero_eng;
        b = base[target].hero_eng;
        dx = a->unk10.x - b->unk10.x;
        dz = a->unk10.z - b->unk10.z;
        if (dx * dx + dz * dz > 0x3D08FF) {
            return 0;
        }
        if (fux->unk54 < 0x1F) {
            return 0;
        }
        if (func_800B7A58(player, target) != 0) {
            h->hero_ai->AIpad |= 0x2000;
            return 1;
        }
    }
    return 0;
}

void func_800B6FEC(s32 player) {
    Ovl62Fux *fux;
    NAS *nas;
    s32 r;

    if (func_800B7CDC() != 0) {
        return;
    }
    fux = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (Ovl62HasBomb2(player)) {
        func_800B649C(player);
        fux->unk24 = Random(0x78) + 0x3C;
        r = func_800B3740(player);
        if (r == -1) {
            fux->unk34 |= 2;
        } else {
            fux->target = r;
            fux->unk34 &= ~2;
        }
        fux->state = 1;
        fux->func = func_800B6708;
        return;
    }
    nas = Ovl62FreeBomb();
    if (nas != NULL) {
        fux->unk3C = 1;
        fux->vel = nas->Engine->unk10;
        fux->pos = Heroes[player].hero_eng->unk10;
        if (fux->unk24-- < 0) {
            fux->state = 5;
            fux->func = func_800CB6B4;
        }
        func_800B60B8(player);
        func_800B5EBC(player);
        return;
    }
    fux->state = 5;
    fux->func = func_800CB6B4;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B7224);

void func_800B7520(s32 player) {
    HeroInfo *h;
    Ovl62Fux *fux;
    s32 dx;
    s32 dz;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux != NULL) {
        if (func_800B7CDC() != 0) {
            return;
        }
        fux->pos = h->hero_eng->unk10;
        dx = fux->vel.x - fux->pos.x;
        dz = fux->vel.z - fux->pos.z;
        if (dx * dx + dz * dz > 0x24000) {
            func_800B5EBC(player);
        } else {
            fux->unk24 = Random(0x78);
            fux->state = 0xA;
            fux->func = func_800B7224;
        }
    }
    if (D_8005A648 == 0x12 && (D_800D5FEC[player] & 1)) {
        fux->state = 0xB;
        func_800CB134(player);
        fux->func = func_800B90C0;
    }
}

void func_800B767C(s32 player) {
    Ovl62Fux *fux;
    HeroInfo *h;
    s32 i;
    s32 n;

    fux = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    i = 0;
    n = 0;
    for (; i < 4; i++) {
        if (Heroes[i].hero_status & 0x2000) {
            n++;
        }
        fux->state = 5;
        fux->func = func_800CB6B4;
        if (n >= 3) {
            return;
        }
    }
    if (Heroes[fux->target].hero_status & 0x2000) {
        fux->state = 4;
        fux->func = func_800CB12C;
    }
    if (fux->unk24-- < 0) {
        fux->state = 5;
        fux->func = func_800CB6B4;
    }
    h = &Heroes[fux->target];
    if (h->hero_mode == 0x4E) {
        fux->state = 5;
        fux->func = func_800CB6B4;
        return;
    }
    fux->vel = h->hero_eng->unk10;
    fux->pos = Heroes[player].hero_eng->unk10;
    func_800B5EBC();
}

void func_800B782C(s32 player) {
    s32 dist;
    HeroInfo *base;
    Ovl62Fux *fux;
    EXTengine *eng;
    EXTengine *e;
    s32 r;
    s32 dx;
    s32 dz;

    if (func_800B7CDC() != 0) {
        return;
    }
    base = Heroes;
    fux = (Ovl62Fux *)base[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    eng = base[player].hero_eng;
    if (eng == NULL) {
        return;
    }
    if (D_8005A648 == 0x15) {
        if (Ovl62HasBomb(player)) {
            r = func_800CB178(player, &dist);
            if (r != -1) {
                func_800B6EB4(player, r);
            }
        }
    }
    fux->unk3C = 1;
    if (D_800D5254 != NULL) {
        fux->vel = D_800D5254->unk54->unk10;
    }
    fux->pos = Heroes[player].hero_eng->unk10;
    e = D_800D5254->unk54;
    dx = eng->unk10.x - e->unk10.x;
    dz = eng->unk10.z - e->unk10.z;
    if (dx * dx + dz * dz < 0x19000) {
        fux->state = 5;
        fux->func = func_800CB6B4;
    }
    if (D_800D5FFC == 1) {
        fux->state = 5;
        fux->func = func_800CB6B4;
    }
    do {
        func_800B5EBC(player);
    } while (0);
}

s32 func_800B7A58(s32 player, s32 target) {
    VECTOR vel;
    VECTOR out;
    VECTOR pos;
    SVECTOR d;
    SVECTOR pad;
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *a;
    EXTengine *b;
    s32 ang;
    s32 ret;

    base = Heroes;
    h = &base[player];
    if (h->hero->FUX == NULL) {
        goto ret0;
    }
    a = h->hero_eng;
    ang = a->unk1E;
    b = base[target].hero_eng;
    vel.vx = D_800D4E04 * D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> 12;
    vel.vz = D_800D4E04 * D_80068BD4[ang & 0xFFF].unk2 >> 12;
    pos.vx = 0;
    pos.vz = 0;
    d.vx = b->unk10.x - a->unk10.x;
    d.vz = b->unk10.z - a->unk10.z;
    d.vy = 0;
    d.pad = 200;
    ret = 1;
    if (func_800BFEE0(&pos, &vel, &d, &out) == ret) {
        return ret;
    }
ret0:
    return 0;
}

s32 func_800B7B88(s32 self) {
    SLONG_XYZ pos;
    s32 dist;
    s32 *dp;
    s32 best;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    s32 i;
    s32 d;
    s32 min;
    u16 st;

    dp = &dist;
    best = -1;
    min = 0x07FFFFFF;
    i = 0;
    base = Heroes;
    me = &base[self];
    h = base;
loop:
    if (i < 4) {
        if (i != self && !((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
            pos = h->hero_eng->unk10;
            d = func_8008CEB0((u8 *)&pos, (u8 *)&me->hero_eng->unk10);
            if (d < min) {
                min = d;
                best = i;
            }
        }
        h++;
        i++;
        goto loop;
    }
    *dp = min;
    if (best != -1) {
        st = 0;
        if (func_800BCF08(self, best) == 0) {
            if (dist <= 0x3FFFF) {
                st = 1;
                d = func_800B7A58(self, best);
                if (d != 0) {
                    return st;
                }
            }
        }
    }
    return 0;
}

s32 func_800B7CDC(s32 self) {
    SLONG_XYZ pos;
    s32 dist;
    s32 *dp;
    s32 best;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    s32 i;
    s32 d;
    s32 min;
    u16 st;
    s32 target;

    if (D_8005A680[0] == 0) {
        return 0;
    }
    dp = &dist;
    best = -1;
    min = 0x07FFFFFF;
    i = 0;
    base = Heroes;
    me = &base[self];
    h = base;
loop:
    do {
        if (i < 4) {
            if (i != self && !((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
                pos = h->hero_eng->unk10;
                d = func_8008CEB0((u8 *)&pos, (u8 *)&me->hero_eng->unk10);
                if (d < min) {
                    min = d;
                    best = i;
                }
            }
            h++;
            i++;
            goto loop;
        }
    } while (0);
    target = best;
    *dp = min;
    if (target == -1) {
        goto ret0;
    }
    d = func_800BCF08(self, target) != 1;
    if (d) {
        return 0;
    }
    if (dist > 0x7FFFF) {
        return 0;
    }
    if (func_800B7A58(self, target) == 0) {
        goto ret0;
    }
    if (func_800B7A58(target, self) == 0) {
        goto ret0;
    }
    Ovl62Grab(self);
    Ovl62Grab(target);
    return 1;
ret0:
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B7F70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B889C);

void func_800B90C0(player)
s32 player;
{
    s32 dist;
    HeroInfo *base;
    HeroInfo *h;
    Ovl62Fux *fux;
    EXTengine *eng;
    s32 r;
    s32 dx;
    s32 dz;

    if (func_800B7CDC(player) != 0) {
        return;
    }
    fux = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (D_8005A648 == 0x15) {
        if (Ovl62HasBomb(player)) {
            r = Ovl62Closest(player, &dist);
            if (r != -1) {
                func_800B6EB4(player, r);
            }
        }
    }
    base = Heroes;
    h = &base[player];
    eng = h->hero_eng;
    if (eng == NULL) {
        return;
    }
    fux->unk3C = 1;
    fux->vel = *(SLONG_XYZ *)&D_800D4DD4;
    fux->pos = h->hero_eng->unk10;
    dx = eng->unk10.x - D_800D4DD4.vx;
    dz = eng->unk10.z - D_800D4DD4.vz;
    if (dx * dx + dz * dz < 0x19000) {
        fux->state = 5;
        fux->func = func_800CB6B4;
    }
    if (fux->unk24-- <= 0) {
        fux->state = 5;
        fux->func = func_800CB6B4;
    }
    func_800B5EBC(player);
}

s32 func_800B9398(s32 player) {
    s32 mode;
    Ovl62Fux *fux;
    HeroInfo *h;
    Ovl62Link *l;
    s32 r;
    HeroInfo *base;

    if (D_8005A648 != 0x14) {
        if ((mode = D_800D5FFC) != 2) {
            return 0;
        }
        base = Heroes;
        h = &base[player];
        fux = (Ovl62Fux *)h->hero->FUX;
        if (fux != NULL && D_800D5254 != NULL) {
            if (D_8009D908 != 0) {
                fux->state = mode;
                fux->func = func_800B782C;
                return 1;
            }
            l = D_800D5254->unk58;
            if (l == NULL) {
                goto fail;
            }
            if (l->unk6C == player) {
                return 0;
            }
            r = Random(0x1000);
            fux->flags |= 1;
            if (r < D_800D4E0C[h->hero_name + 0x17]) {
                fux->state = mode;
                fux->func = func_800B782C;
                return 1;
            }
        }
    }
fail:
    return 0;
}

s32 func_800B94B8(s32 player) {
    Ovl62Fux *fux;
    Ovl62Fux *ext;
    HeroInfo *h;
    s32 r;
    s32 t;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux == NULL) {
        return 0;
    }
    r = Random(0x1000);
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL && ext->unk24 == 1) {
        return 0;
    }
    if (r >= D_800D4E0C[Heroes[player].hero_name - 1]) {
        return 0;
    }
    func_800B649C(player);
    fux->unk24 = Random(0x78) + 0x3C;
    t = func_800B3740(player);
    if (t == -1) {
        fux->unk34 |= 2;
    } else {
        fux->target = t;
        fux->unk34 &= ~2;
    }
    fux->state = 1;
    fux->func = func_800B6708;
    return 1;
}

s32 func_800B9600(s32 player) {
    Ovl62Fux *fux;
    Ovl62Fux *f;
    HeroInfo *h;
    s32 r;
    HeroInfo *base;

    if (D_8005A648 != 0x14) {
        base = Heroes;
        h = &base[player];
        fux = (Ovl62Fux *)h->hero->FUX;
        if (fux != NULL) {
            r = Random(0x1000);
            if (((s32(*)(s32, s32))func_800CAF94)(8, player) != 0) {
                r = 0x7FFF;
            }
            if (r < D_800D4E0C[h->hero_name + 0x27]) {
                f = (Ovl62Fux *)h->hero->FUX;
                if (f != NULL) {
                    f->unk24 = Random(0x168) + 0x5A;
                }
                fux->state = 8;
                fux->func = func_800B6D74;
                return 1;
            }
            return 0;
        }
        return 0;
    }
    return 0;
}

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

s32 func_800B98AC(s32 player) {
    Ovl62Fux *fux;
    Ovl62Fux *f;
    HeroInfo *h;

    HeroInfo *base;

    if (D_8005A648 == 0x15) {
        base = Heroes;
        h = &base[player];
        fux = (Ovl62Fux *)h->hero->FUX;
        if (fux != NULL && Random(0x1000) < D_800D4E0C[h->hero_name + 0x3F]) {
            f = (Ovl62Fux *)h->hero->FUX;
            if (f != NULL) {
                f->unk24 = Random(0x168) + 0x5A;
            }
            fux->state = 0xD;
            fux->func = func_800B6FEC;
            return 1;
        }
        return 0;
    }
    return 0;
}

s32 func_800B9994(s32 player) {
    Ovl62Fux *fux;
    HeroInfo *h;
    s32 start;
    s32 i;
    HeroInfo *base;

    base = Heroes;
    h = base + player;
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux == NULL) {
        return 0;
    }
    start = Random(3);
    if (Random(0x1000) < D_800D4E0C[h->hero_name + 0x1F]) {
        for (i = start; ; i++) {
            s32 t;

            if (i >= start + 4) {
                break;
            }
            t = i & 3;
            if (t != player && !(Heroes[t].hero_status & 0x2000) && (D_800D5FEC[t] & 1)) {
                fux->target = t;
                Ovl62StartChase(player);
                fux->state = 7;
                fux->func = func_800B767C;
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800B9B6C(s32 hero) {
    s16 lead;
    s16 other;
    s32 mate;
    s32 r;

    r = Random(0x1000);
    lead = ((Ovl62TeamView *)D_8005A614)->team0;
    if (hero == lead || (mate = Heroes[lead].hero_teammate, hero == mate)) {
        if (r < 0x800) {
            other = ((Ovl62TeamView *)D_8005A614)->team1;
            if (!(Heroes[other].hero_status & 0x2000)) {
                return other;
            }
            mate = Heroes[other].hero_teammate;
            if (Heroes[mate].hero_status & 0x2000) {
                return -1;
            }
            return mate;
        } else {
            lead = ((Ovl62TeamView *)D_8005A614)->team1;
            mate = Heroes[lead].hero_teammate;
            if (!(Heroes[mate].hero_status & 0x2000)) {
                return mate;
            }
            if (!(Heroes[lead].hero_status & 0x2000)) {
                return lead;
            }
            return -1;
        }
    } else if (r < 0x800) {
        if (!(Heroes[lead].hero_status & 0x2000)) {
            return lead;
        }
        if (Heroes[mate].hero_status & 0x2000) {
            return -1;
        }
        return mate;
    } else {
        if (!(Heroes[mate].hero_status & 0x2000)) {
            return mate;
        }
        if (!(Heroes[lead].hero_status & 0x2000)) {
            return lead;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800B9D48);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BAFA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BB810);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BBF2C);

s32 func_800BCC9C(s32 player) {
    HeroInfo *h;
    Ovl62Fux *ext;
    Ovl62Fux *fux;
    u16 name;
    HeroInfo *base;
    s32 state;

    if (D_800D4E08 < 0xD2) {
        goto fail;
    }
    name = Heroes[player].hero_name;
    if (name != 4 && name != 1 && name != 7 && name != 3 && name != 2) {
        return 0;
    }
    base = Heroes;
    h = &base[player];
    ext = (Ovl62Fux *)FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        goto fail;
    }
    if (ext->unk24 != 0) {
        return 0;
    }
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux == NULL) {
        goto fail;
    }
    state = 6;
    if (fux->state != state && fux->state != 9) {
        return 0;
    }
    if (h->hero_mode != 0x49) {
        return 0;
    }
    if (fux->unk50 != 0) {
        return 0;
    }
    if (h->hero_name == 1) {
        h->hero_anim->Atable = D_800D3BB4;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 2) {
        h->hero_anim->Atable = D_800D3C74;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 7) {
        h->hero_anim->Atable = D_800D4174;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 8) {
        h->hero_anim->Atable = D_800D4B64;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 5) {
        h->hero_anim->Atable = D_800D4410;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == state) {
        h->hero_anim->Atable = D_800D4A50;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 3) {
        h->hero_anim->Atable = D_800D3EDC;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 4) {
        h->hero_anim->Atable = D_800D4820;
        h->hero_anim->Aindex = 1;
    }
    fux->unk50 = 1;
    return 1;
fail:
    return 0;
}

s32 func_800BCF08(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (*(s32 *)(g + 0x6C) != 0) {
        if (Heroes[arg1].hero_status & 0x2000) {
            return 0;
        }
        if (arg0 == *(s16 *)(g + 0x48) && arg1 == Heroes[arg0].hero_teammate) {
            return 1;
        }
        if (arg1 == D_8005A65C[0] && arg0 == Heroes[arg1].hero_teammate) {
            return 1;
        }
        if (arg0 == D_8005A65C[1] && arg1 == Heroes[arg0].hero_teammate) {
            return 1;
        }
        if (arg1 == D_8005A65C[1] && arg0 == Heroes[arg1].hero_teammate) {
            return 1;
        }
    }
    return 0;
}

void func_800BD058(s32 arg0, s32 arg1, s32 arg2) {
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *eng;
    EXTengine *shadow;

    switch (arg1) {
    case 0:
        if (arg2 == 0) {
            break;
        }
        goto common;
    case 1:
        if (arg2 != arg1) {
        common:
            base = Heroes;
            h = &base[arg0];
            eng = h->hero_eng;
            shadow = h->hero_shadow;
            if (eng != NULL && shadow != NULL) {
                eng->unk2C.x = 0x1000;
                eng->unk2C.y = 0x1000;
                eng->unk2C.z = 0x1000;
                shadow->unk2C.x = 0x1000;
                shadow->unk2C.y = 0x1000;
                shadow->unk2C.z = 0x1000;
                func_800B5684(arg0);
                D_800D6924[arg0] = D_8009D6F4[h->hero_name * 3];
            }
        }
        break;
    case 2:
        if (arg2 != arg1 && Heroes[arg0].hero_mode != 0x49) {
            SetHeroAction((HeroInfo *)arg0, 0x49);
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitTransmuterBeam);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotTransmuterBeam);

void PlotWeightDeath(NAS *m) {
    WeightDeathExt *ext;
    HeroInfo *h;
    HeroInfo *base;
    HeroInfo *p;
    HeroInfo *q;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (ext->unk24-- >= 0) {
        return;
    }
    m->Engine->unk10.y += ext->unk10;
    ext->unk10 += ext->unk14;
    if (ext->unkC < m->Engine->unk10.y) {
        m->Engine->unk10.y -= abs(m->Engine->unk10.y - ext->unkC);
        if (++ext->unk18 < 5) {
            ext->unk10 = -ext->unk10 >> 3;
            if (ext->unk18 == 1) {
                PlaySfx(0x455, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
    }
    if (ext->unk20 > 0x3C) {
        HeroInfo *b = Heroes;

        q = &b[ext->unk1C];
        if (!(q->hero_status & 0x200)) {
            q->hero_status |= 0x2000;
        }
        m->Engine->sub->flags &= ~0x8000;
    }
    if (ext->unk20 > 0x78) {
        base = Heroes;
        p = &base[ext->unk1C];
        if (!(p->hero_status & 0x200)) {
            p->hero_status |= 0x2000;
            base[ext->unk1C].hero_status |= 0x8000;
        }
    } else {
        ext->unk20++;
    }
    if (ext->unk20 > 0xF) {
        base = Heroes;
        h = &base[ext->unk1C];
        if (!(h->hero_status & 0x200)) {
            h->hero_eng->flags &= ~0x8000;
            base[ext->unk1C].hero_shadow->flags &= ~0x8000;
        }
    }
}

void func_800BFD38(NAS *nas, GravityMotion *m) {
    EXTengine *eng;
    s32 y;
    s32 floor;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->unk10.x += m->vx;
        eng->unk10.y += m->vy;
        eng->unk10.z += m->vz;
        m->vx += m->ax;
        m->vy += m->ay;
        m->vz += m->az;
        if (m->flags & 4) {
            nas->Engine->unk1C += m->rx;
        }
        if (m->flags & 8) {
            nas->Engine->unk1E += m->ry;
        }
        if (m->flags & 0x10) {
            nas->Engine->unk20 += m->rz;
        }
        nas->Engine->unk5E = 1;
        floor = m->floor;
        y = eng->unk10.y;
        if (y >= floor) {
            eng->unk10.y -= abs(y - floor);
            if (m->flags & 1) {
                m->vy = -m->vy >> 1;
            }
            if (m->flags & 2) {
                func_800270B0(nas);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800BFEE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotScratchMaster);

void InitIceBerg(NAS *m) {
    MapTriggerInfo *info;

    if (m->trigger != NULL) {
        info = m->trigger->InfoStruct;
        m->UserInfo.user1 = info->val1;
        m->UserInfo.user2 = info->val2;
        m->Engine->sub->unk6C = D_800546C4;
        if (D_8005A648 == 0x12) {
            if (m->UserInfo.user1 == 0) {
                m->Engine->sub->unk74 = 0x200D;
            }
            if (m->UserInfo.user1 == 1) {
                m->Engine->sub->unk74 = 0x200E;
            }
            if (m->UserInfo.user1 == 2) {
                m->Engine->sub->unk74 = 0x200F;
            }
        } else {
            if (m->UserInfo.user1 == 0) {
                m->Engine->sub->unk74 = 0x2010;
            }
            if (m->UserInfo.user1 == 1) {
                m->Engine->sub->unk74 = 0x2011;
            }
            if (m->UserInfo.user1 == 2) {
                m->Engine->sub->unk74 = 0x2012;
            }
        }
        m->Engine->sub->flags |= 0x02000000;
        m->Engine->sub->unk51 = 0;
        if (m->UserInfo.user1 == 1) {
            m->Engine->sub->unk12 = 0x800;
            m->Engine->sub->unk68 = 0x200;
        }
        m->Engine->sub->unk68 = 0x200;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C0888);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl62/ovl62", D_800B3498);

void func_800C0E94(void) {
    s32 i;
    SceneObj *obj;
    SceneObj *scan;
    s32 n;
    VECTOR v;
    VECTOR *vp;

    scan = *(SceneObj **)((u8 *)D_80058AD0 + 0x1C);
    for (i = 15; i >= 0; i--) {
        D_800D6190[i] = NULL;
    }
    n = 0;
    while (scan != NULL) {
        if ((scan->flags & 0x8000) && scan->index == 0x4000) {
            D_800D6190[n++] = scan;
        }
        scan = scan->next;
    }
    i = 0;
    while (1) {
        static const VECTOR up = { 0, 0, 0x1000, 0 };

        if (i >= 16) {
            break;
        }
        v = up;
        vp = &v;
        obj = D_800D6190[i];
        ApplyMatrixLV((MATRIX *)((u8 *)obj + 0x30), vp, vp);
        i++;
        D_800D6150[(((get_angle(v.vx, vp->vz) + 1) & 0xFFF) >> 8) - 1 & 0xF] = (Ovl62RiserObj *)obj;
    }
}

#define UKA_SEG_SET(j, st, a) \
    { m->seg[j].state = (st); m->seg[j].angle = (a); m->seg[j].t = 0; m->seg[j].count++; }

void func_800C0FE8(s32 i, UkaMaskSet *m) {
    s32 next;
    s32 prev;

    next = (i + 1) & 0xF;
    prev = (i - 1) & 0xF;
    if (m->seg[i].count == 0) {
        UKA_SEG_SET(i, 1, 0x4000);
        switch (m->seg[next].state) {
        case 0:
            UKA_SEG_SET(next, 2, 0x4300);
            break;
        case 3:
            UKA_SEG_SET(next, 4, 0x4200);
            break;
        }
        switch (m->seg[prev].state) {
        case 0:
            UKA_SEG_SET(prev, 3, 0x4380);
            break;
        case 2:
            UKA_SEG_SET(prev, 4, 0x4100);
            break;
        }
    } else if (m->seg[i].count == 1) {
        switch (m->seg[i].state) {
        case 3:
            UKA_SEG_SET(i, 1, 0x4280);
            break;
        case 2:
            UKA_SEG_SET(i, 1, 0x4180);
            break;
        }
        switch (m->seg[prev].state) {
        case 0:
            UKA_SEG_SET(prev, 3, 0x4380);
            break;
        case 2:
            UKA_SEG_SET(prev, 4, 0x4100);
            break;
        }
        switch (m->seg[next].state) {
        case 0:
            UKA_SEG_SET(next, 2, 0x4300);
            break;
        case 3:
            UKA_SEG_SET(next, 4, 0x4200);
            break;
        }
    } else if (m->seg[i].count == 2) {
        if (m->seg[i].state == 4) {
            UKA_SEG_SET(i, 1, 0x4080);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitUkaPolarMask);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotUkaPolarMask);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C2ECC);

void InitPolarWalrus(NAS *m) {
    PolarWalrusExt *ext;
    PolarWalrusExt2 *e2;

    D_800D5494 = (Ovl62Prox *)m;
    m->Engine->flags |= 0x200000;
    ext = AddExtension(m, 0x94, 0x10000000);
    if (ext != NULL) {
        ext->cursor = func_80024E30(m, &D_800D5630);
        m->FUX = (EXT *)ext;
        m->Engine->sub->unk6C = D_8005476C;
        m->Engine->sub->unk74 = 0x4000;
        m->UserInfo.status = 0;
        func_80026B70(&D_800D5454, (u8 *)m);
        m->Engine->unk10.x = (D_80068BD4[(D_800D563C - 0x400) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.z = (D_80068BD4[D_800D563C & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.y = 0x400;
        m->Engine->unk1E = get_angle(m->Engine->unk10.x, m->Engine->unk10.z) & 0xFFF;
        m->Engine->sub->flags &= ~0x8000;
        ext->state = 0;
        m->Engine->unk5C = 1;
        m->Engine->unk5D = 0;
        m->Engine->unk2C.x = 0x1F40;
        m->Engine->unk2C.y = 0x1F40;
        m->Engine->unk2C.z = 0x1F40;
        m->Engine->unk5E = 1;
        m->Engine->sub->unk68 = 0x200;
        ext->arc[0] = 0x10;
        ext->unk58 = 0x28;
        ext->arc[1] = 0;
        ext->arc[6] = 0;
        ext->from = *(SLONG_XYZ *)&m->Engine->unk10;
        ext->to.x = (D_80068BD4[(D_800D563C - 0x400) & 0xFFF].unk2 * 0x7D) >> 8;
        ext->to.y = 0;
        ext->to.z = (D_80068BD4[D_800D563C & 0xFFF].unk2 * 0x7D) >> 8;
        m->Engine->sub->flags |= 0x02000000;
        ext->timer = Random(8) * 0x1E + 0x10E;
        ext->unk90 = 0;
        m->UserInfo.user1 = 0;
    }
    e2 = AddExtension(m, 0x54, 0x20000000);
    if (e2 != NULL) {
        e2->unk4E = 0x400;
        e2->unk20 = 0;
        e2->unk40 = 0;
        e2->unk4C = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotPolarWalrus);

void InitWalrusFish(NAS *m) {
    WalrusFishExt *ext;

    ext = AddExtension(m, 0x88, 0x10000000);
    if (ext != NULL) {
        ext->cursor = func_80024E30(m, &D_800D5690);
        m->Engine->flags |= 0x200000;
        m->Engine->unk10.x = (D_80068BD4[(D_800D563C - 0x400) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.z = (D_80068BD4[D_800D563C & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.y = 0x400;
        m->Engine->sub->unk6C = D_8005476C;
        m->Engine->sub->unk74 = 0x2002;
        m->Engine->unk5C = 1;
        m->Engine->unk5D = 0;
        m->Engine->unk2C.x = 0x1F40;
        m->Engine->unk2C.y = 0x1F40;
        m->Engine->unk2C.z = 0x1F40;
        ext->state = 0;
        m->Engine->sub->unk68 = 0x200;
        ext->arc[0] = 0x10;
        ext->unk54 = 0x28;
        ext->arc[1] = 0;
        ext->arc[6] = 0;
        ext->from = *(SLONG_XYZ *)&m->Engine->unk10;
        ext->to.x = (D_80068BD4[(D_800D563C - 0x400) & 0xFFF].unk2 * 0x7D) >> 8;
        ext->to.y = 0;
        ext->to.z = (D_80068BD4[D_800D563C & 0xFFF].unk2 * 0x7D) >> 8;
        m->Engine->sub->flags |= 0x02000000;
        ext->spin = 0x80;
        ext->timer = 0;
        PlaySfx(0x478, NULL, NULL, 0x1000, 0x1E00, 0);
        m->Engine->unk5E = 1;
    }
}

void PlotWalrusFish(NAS *m) {
    WalrusFishExt *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    m->Engine->unk1E += ext->spin;
    m->Engine->unk1C += 0x100;
    m->Engine->unk5E = 1;
    switch (ext->state) {
    case 0:
        if (++ext->timer == 0xF) {
            m->Engine->sub->unk68 = 0;
        }
        if (func_8008C5C4(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->unk10.x) != 0) {
            ext->state = 2;
            PlaySfx(0x479, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 2:
        func_800250B0(m);
        m->Engine->unk10.x += ext->vx;
        m->Engine->unk10.z += ext->vz;
        ext->spin >>= 1;
        m->Engine->unk1C >>= 1;
        if (ext->vx < 0) {
            if (++ext->vx <= 0) {
                goto skip_x;
            }
            ext->vx = 0;
        }
        if (ext->vx > 0) {
            if (--ext->vx < 0) {
                ext->vx = 0;
            }
        }
    skip_x:
        if (ext->vz < 0) {
            if (++ext->vz <= 0) {
                goto skip_z;
            }
            ext->vz = 0;
        }
        if (ext->vz > 0) {
            if (--ext->vz < 0) {
                ext->vz = 0;
            }
        }
    skip_z:
        break;
    }
}

void PlotSmokeTrail(NAS *m) {
    ExtSmokeTrail *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (ext->flags & 1) {
        m->Engine->unk10.y -= ext->rise;
        m->Engine->unk10.x = ext->baseX + ((ext->amp * D_80068BD4[(ext->angle - 0x400) & 0xFFF].unk2) >> 12);
        ext->amp -= ext->ampDecay;
        if (ext->amp < 0) {
            ext->amp = 0;
        }
        m->Engine->sub->unk74 = 0x202E;
        ext->angle += 0x80;
    }
    switch (m->UserInfo.status) {
    case 0:
        m->Engine->sub->unk76 -= ext->fadeIn;
        if ((s16)m->Engine->sub->unk76 < 0) {
            m->Engine->sub->unk76 = 0;
            m->UserInfo.status = 1;
        }
        break;
    case 1:
        m->Engine->sub->unk76 += ext->fadeOut;
        m->Engine->unk2C.x -= ext->shrink;
        m->Engine->unk2C.y -= ext->shrink;
        m->Engine->unk2C.z -= ext->shrink;
        if ((s16)m->Engine->sub->unk76 >= 0xDAD) {
            KillNAS(m);
            return;
        }
        break;
    }
    m->Engine->unk5E = 1;
}

void InitSimpleExplosion(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = AddExtension(m, 0x108, 0x10000000);
    if (ext != NULL) {
        sub = m->Engine->sub;
        func_80018F3C(m->Engine, 0x10);
        if (sub != NULL) {
            i = 0;
            do {
                sub->unk74 = 0x2034;
                sub->unk6C = D_800546C4;
                ext->vel[i].x = Random(0x4C) - Random(0x4C);
                ext->vel[i].z = Random(0x4C) - Random(0x4C);
                ext->vel[i].y = Random(0x4C) - Random(0x4C);
                sub->flags = 0x40008041;
                sub->unk50 = 1;
                sub->unk51 = 0;
                sub->unk20.z = 0x1000;
                sub->unk20.y = 0x1000;
                sub->unk20.x = 0x1000;
                ext->unk104 = 0;
                sub->unk76 = 0;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
    }
}

void PlotDingoTail(NAS *m) {
    BrioOwner *owner;
    s32 user;

    user = m->UserInfo.user1;
    if (user != 0) {
        owner = (BrioOwner *)user;
        if (owner->type != 0x51) {
            KillNAS(m);
            return;
        }
        m->Engine->unk1C += 0x200;
        m->Engine->unk5E = 1;
        m->Engine->unk10 = owner->engine->unk10;
        m->Engine->unk1E = owner->engine->unk1E - 0x400;
        m->Engine->unk2C.x = owner->engine->unk2C.x;
        m->Engine->unk2C.y = owner->engine->unk2C.y;
        m->Engine->unk2C.z = owner->engine->unk2C.z;
        if (owner->subtype == 7) {
            m->Engine->unk10.y = owner->engine->unk10.y - ((owner->engine->unk2C.x * 0x208) >> 12);
        } else {
            m->Engine->unk10.y = owner->engine->unk10.y - ((owner->engine->unk2C.x * 0x1A4) >> 12);
        }
        if (owner->anim->frame >= 0xD) {
            m->Engine->sub->unk76 += 0x400;
        }
    }
}

void PlotBrioPack(NAS *m) {
    BrioOwner *owner;
    NAS *nas;
    s32 user;

    user = m->UserInfo.user1;
    if (user != 0) {
        owner = (BrioOwner *)user;
        if (owner->type != 0x51) {
            KillNAS(m);
            return;
        }
        if (owner->anim->frame == 0xC) {
            nas = SpawnNASById(owner->engine->unk10.x,
                               owner->engine->unk10.y - ((owner->engine->unk2C.x * 0x1A4) >> 12),
                               owner->engine->unk10.z + 0x40, 0x1613, 0x11);
            if (nas != NULL) {
                nas->UserInfo.user1 = (s32)owner;
            }
        }
        if (owner->anim->frame >= 0xA) {
            m->Engine->unk10 = owner->engine->unk10;
            m->Engine->unk1E = owner->engine->unk1E;
            m->Engine->unk2C.x = owner->engine->unk2C.x;
            m->Engine->unk2C.y = owner->engine->unk2C.y;
            m->Engine->unk2C.z = owner->engine->unk2C.z;
            m->Engine->unk10.y = owner->engine->unk10.y - ((owner->engine->unk2C.x * 0x1A4) >> 12);
        }
        if (owner->anim->frame >= 0x18) {
            m->Engine->sub->unk76 += 0x400;
        }
    }
    m->Engine->unk5E = 1;
}

void InitPackEffect(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = AddExtension(m, 0x108, 0x10000000);
    if (ext != NULL) {
        sub = m->Engine->sub;
        func_80018F3C(m->Engine, 0xC);
        if (sub != NULL) {
            i = 0;
            do {
                sub->unk74 = 0x2033;
                sub->unk6C = D_800546C4;
                ext->vel[i].x = Random(0x38) - Random(0x38);
                ext->vel[i].z = Random(0x38) - Random(0x38);
                ext->vel[i].y = Random(0x38) - Random(0x38);
                sub->flags = 0x40008021;
                sub->unk50 = 1;
                sub->unk51 = 0;
                sub->unk20.z = 0x1000;
                sub->unk20.y = 0x1000;
                sub->unk20.x = 0x1000;
                ext->unk104 = 0;
                sub->unk76 = 0;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
    }
}

void InitPolarBoss(NAS *nas) {
    PolarBossExt *ext = AddExtension(nas, 0x94, 0x10000000);
    NAS *s;
    Ext20 *e;

    if (ext != NULL) {
        func_80026A04((u8 *)nas);
        func_80086868();
        D_800D5DC4 = nas;
        ext->cursor = func_80024E30(nas, &D_800D5A34);
        func_80025FAC((EXTanimator *)ext->cursor, 0);
        func_800250B0(nas);
        ext->unk84 = SpawnNASById(nas->Engine->unk10.x, nas->Engine->unk10.y, nas->Engine->unk10.z, 0x1622, 0x11);
        s = SpawnNASById(nas->Engine->unk10.x, 0, nas->Engine->unk10.z, 0x1620, 0x11);
        if (s != NULL) {
            ext->unk88 = s;
            func_80026A04((u8 *)s);
        }
        ext->unk90 = 0;
        nas->Engine->sub->unk6C = *(void **)D_80058AD0;
        nas->Engine->sub->unk68 = 0x300;
        nas->Engine->sub->flags |= 0x02000000;
        nas->Engine->unk5D = 0;
        D_8009D6E2 = 0x14;
        e = FindExtension(D_8009D670, 0x20000000);
        if (e != NULL) {
            e->val = 0xFF;
        }
        ext->state = 5;
        ext->unk38 = 0x12;
        ext->unk3C = 4;
        ext->unk3E = 4;
        ext->unk5C = 4;
        ext->unk5E = 4;
        ext->unk34 = 0;
        ext->unk58 = 0;
        ext->unk54 = 0;
        ext->unk68 = 0;
        ext->ang = 0;
        ext->unk70 = 1;
        ext->unk74 = 0;
        ext->phase = 0;
        ext->unk80 = 0;
        ext->unk8C = 0xC;
        PlaySfx(0x226, &nas->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
        nas->Engine->OldXYZ.x = (D_80068BD4[(((ext->ang + 0x20) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3) >> 2;
        nas->Engine->OldXYZ.z = ((D_80068BD4[(ext->ang + 0x20) & 0xFFF].unk2 * 0x400) >> 12) + 0x2000;
        get_angle(nas->Engine->OldXYZ.x - nas->Engine->unk10.x, nas->Engine->OldXYZ.z - nas->Engine->unk10.z);
        nas->Engine->unk1E = 0x800;
        nas->Engine->unk5E = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotPolarBoss);

s32 func_800C5CD0(s32 *arg0, s32 arg1) {
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
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < -0x80) {
                return 0;
            }
            a3 += 0x38;
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}

s32 func_800C5DCC(NAS *m) {
    if (D_800D5298->unk74 == 0x2008) {
        if (func_800C5CD0((s32 *)&m->Engine->unk10, 1)) {
            return 1;
        }
        if (func_800C5CD0((s32 *)&m->Engine->unk10, 0)) {
            return 0;
        }
    }
    if (D_800D5298->unk74 == 0x2009) {
        if (func_800C5CD0((s32 *)&m->Engine->unk10, 2)) {
            return 1;
        }
        if (func_800C5CD0((s32 *)&m->Engine->unk10, 0)) {
            return 0;
        }
    }
    if (D_800D5298->unk74 == 0x200A) {
        if (func_800C5CD0((s32 *)&m->Engine->unk10, 3)) {
            return 0;
        }
    }
    if (D_800D5298->unk74 == 0x5003) {
        if (func_800C5CD0((s32 *)&m->Engine->unk10, 0)) {
            return 0;
        }
    }
    return 1;
}

void func_800C5EEC(Ovl62EaseCam *c, s32 *pos) {
    s32 step;

    if (c->x.cur != c->x.target) {
        if (c->x.target != 0) {
            step = c->x.step;
        } else {
            step = c->x.stepZero;
        }
        if (c->x.cur < c->x.target) {
            c->x.cur += step;
            if (c->x.cur > c->x.target) {
                c->x.cur = c->x.target;
            }
        } else {
            c->x.cur -= step;
            if (c->x.cur < c->x.target) {
                c->x.cur = c->x.target;
            }
        }
    }
    if (c->z.cur != c->z.target) {
        if (c->z.target != 0) {
            step = c->z.step;
        } else {
            step = c->z.stepZero;
        }
        if (c->z.cur < c->z.target) {
            c->z.cur += step;
            if (c->z.cur > c->z.target) {
                c->z.cur = c->z.target;
            }
        } else {
            c->z.cur -= step;
            if (c->z.cur < c->z.target) {
                c->z.cur = c->z.target;
            }
        }
    }
    pos[0] -= c->x.cur;
    pos[2] -= c->z.cur;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitBearminator);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotBearminator);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotPolarMissile);

void PlotBigMissile(NAS *m) {
    BigMissileArcExt *ext;
    PolarBossExt *boss;
    ExtSpawn5910 *sext;
    NAS *nas;
    NAS *res;
    SLONG_XYZ *pos;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (ext->state != 0) {
        return;
    }
    if (func_8008C5C4(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->unk10.x) == 0) {
        return;
    }
    m->Engine->unk10.y = 0x20;
    pos = &m->Engine->unk10;
    nas = SpawnNASById(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        sext = FindExtension(nas, 0x10000000);
        if (sext != NULL) {
            sext->unk18 = 0x100;
            sext->unkC = 0;
            sext->unk20 = 1;
            sext->unk28 = 0;
            nas->InitDelete(nas);
        }
        res = nas;
    } else {
        res = NULL;
    }
    if (res != NULL) {
        res->Engine->flags |= 0x200000;
    }
    boss = FindExtension(D_800D5448, 0x10000000);
    if (boss != NULL) {
        boss->unk70 = 1;
        if (boss->phase == 2 && D_800D5298 != NULL) {
            D_800D5298->unk74 = 0x2008;
        }
        if (boss->phase == 4 && D_800D5298 != NULL) {
            D_800D5298->unk74 = 0x2009;
        }
        if (boss->phase == 6 && D_800D5298 != NULL) {
            D_800D5298->unk74 = 0x200A;
        }
        if (boss->phase == 8 && D_800D5298 != NULL) {
            D_800D5298->flags &= ~0x8000;
            if (!(Heroes[0].hero_mode & 0x2000)) {
                SetHeroAction((HeroInfo *)0, 0x4E);
            }
            if (D_8005A680[0] != 0 && !(Heroes[1].hero_mode & 0x2000)) {
                SetHeroAction((HeroInfo *)1, 0x4E);
            }
        }
        boss->state = 5;
    }
    PlaySfx(0x22F, NULL, NULL, 0x1000, 0x1E00, 0);
    KillNAS(m);
}

void func_800C7ED4(SniperAim *a, s32 *out) {
    s32 step;

    if (a->x.cur != a->x.target) {
        if (a->x.target != 0) {
            step = a->x.step;
        } else {
            step = a->x.stepZero;
        }
        if (a->x.cur < a->x.target) {
            a->x.cur += step;
            if (a->x.target < a->x.cur) {
                a->x.cur = a->x.target;
            }
        } else {
            a->x.cur -= step;
            if (a->x.cur < a->x.target) {
                a->x.cur = a->x.target;
            }
        }
    }
    if (a->y.cur != a->y.target) {
        if (a->y.target != 0) {
            step = a->y.step;
        } else {
            step = a->y.stepZero;
        }
        if (a->y.cur < a->y.target) {
            a->y.cur += step;
            if (a->y.target < a->y.cur) {
                a->y.cur = a->y.target;
            }
        } else {
            a->y.cur -= step;
            if (a->y.cur < a->y.target) {
                a->y.cur = a->y.target;
            }
        }
    }
    out[0] -= a->x.cur;
    out[1] -= a->y.cur;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", InitSniper);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotSniper);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", PlotSniperMissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800C9230);

void InitSniperMissile(NAS *m) {
    SniperMissileExt *ext;

    ext = AddExtension(m, 0x38, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk74 = 0x2006;
        m->Engine->sub->unk6C = D_80054A0C;
        ext->unkC = 0;
        m->Engine->unk5C = 1;
        m->Engine->unk2C.x = 0x2AF8;
        m->Engine->unk2C.y = 0x2AF8;
        m->Engine->unk2C.z = 0x2AF8;
        m->Engine->unk5D = 2;
        ext->unk30 = 0;
        ext->unk34 = 0;
        PlaySfx(0x522, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
    }
}

void InitBigMissile(NAS *m) {
    BigMissileExt *ext;

    ext = AddExtension(m, 0x7C, 0x10000000);
    if (ext != NULL) {
        m->Engine->unk5D = 2;
        m->Engine->unk5C = 1;
        m->Engine->sub->unk74 = 0x201A;
        m->Engine->sub->unk6C = D_800546C4;
        m->Engine->unk2C.x = 0x2000;
        m->Engine->unk2C.y = 0x2000;
        m->Engine->unk2C.z = 0x2000;
        m->Engine->unk5E = 1;
        ext->unkC = 0x10;
        ext->unk50 = 0x96;
        ext->unk10 = 0;
        ext->unk24 = D_8009D530[0]->unk10.y;
        ext->unk78 = 0;
        ext->pos = m->Engine->unk10;
        PlaySfx(0x22D, NULL, NULL, 0x1000, 0x1E00, 0);
        PlaySfx(0x22B, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

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

void InitPolarMissile(NAS *m) {
    PolarMissileExt *ext;

    ext = AddExtension(m, 0x70, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk74 = 0x2006;
        m->Engine->sub->unk6C = D_80054A0C;
        m->Engine->unk5D = 2;
        m->Engine->unk5C = 1;
        m->Engine->sub->unk51 = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk5C = 0x8C;
        ext->unk2C = 0x3000;
        ext->unk38 = 0x700;
        ext->unk4C = 4;
        ext->unk10 = 0;
        ext->unk18 = 0;
        ext->unk58 = 0;
        ext->unk44 = 0;
        ext->unk48 = 0x100;
        ext->unk54 = 0;
        ext->unk60 = 0;
        ext->unk50 = ext->unk2C;
        PlaySfx(0x522, &m->Engine->sfx, &m->Engine->unk10, 0x1000, 0x1E00, 0);
    }
}

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

void func_800C9D74(NAS *m) {
    u8 *p;
    EXTengine *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 ang;
    TrigEntry *t;

    p = D_800D5454;
    t = D_80068BD4;
loop:
    if (p != NULL) {
        e = *(EXTengine **)(p + 0x54);
        if (e != NULL && e != m->Engine) {
            dx = e->unk10.x - m->Engine->unk10.x;
            dz = e->unk10.z - m->Engine->unk10.z;
            if (dx * dx + dz * dz < 0x3D090) {
                d = 0x7D00 - csqrt(dx * dx + dz * dz);
                ang = get_angle(dx, dz) & 0xFFF;
                m->Engine->unk10.x -= ((d + 2) * t[(ang - 0x400) & 0xFFF].unk2) >> 18;
                m->Engine->unk10.z -= ((d + 2) * t[ang].unk2) >> 18;
            }
        }
        p = *(u8 **)(p + 0x20);
        goto loop;
    }
}

s32 func_800C9EC4(EXTengine *eng, s32 player) {
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *e;
    s32 dx;
    s32 dz;

    base = Heroes;
    h = base + player;
    if (h->hero_mode != 0x4E) {
        e = h->hero_eng;
        dx = eng->unk10.x - e->unk10.x;
        dz = eng->unk10.z - e->unk10.z;
        if (dx * dx + dz * dz < 0x40000) {
            return 1;
        }
    }
    return 0;
}

void PlotBearCam(NAS *m) {
}

void InitBearCam(NAS *m) {
}

void PlotRockProjectile(NAS *m) {
}

void InitRockProjectile(NAS *m) {
}

s32 func_800C9F70(void) {
    u8 *g = D_8005A614;
    s32 k;

    if (g[0xC1] < 10) {
        return 0;
    }
    k = 0xB8;
    if (g[0xC1] >= 30) {
        return 0xE66;
    }
    return (g[0xC1] - 9) * k;
}

void PlotBearBoss(NAS *m) {
    if (FindExtension(m, 0x10000000) != NULL) {
        m->Engine->unk5E = 1;
        func_800250B0(m);
    }
}

void InitBearBoss(NAS *m) {
    Ext10000000 *ext;

    ext = AddExtension(m, 0x10, 0x10000000);
    if (ext != NULL) {
        m->Engine->unk5D = 0;
        m->Engine->sub->unk68 = 0x300;
        m->Engine->sub->flags |= 0x02000000;
        m->Engine->sub->unk6C = *(void **)D_80058AD0;
        ext->unkC = (s32)func_80024E30(m, &D_800D5DB8);
        func_80025FAC((EXTanimator *)ext->unkC, 0);
        func_800250B0(m);
    }
}

void PlotPackEffect(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        sub = m->Engine->sub;
        ext->unk104++;
        if (sub != NULL) {
            i = 0;
            do {
                sub->unk04 += ext->vel[i].x;
                sub->unk08 += ext->vel[i].z;
                sub->unk0C += ext->vel[i].y;
                sub->unk76 += 0x100;
                sub->unk20.x -= 0x80;
                sub->unk20.y -= 0x80;
                sub->unk20.z -= 0x80;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
        if (ext->unk104 == 0x2D) {
            KillNAS(m);
        }
    }
}

void PlotCortexPack(NAS *m) {
    m->Engine->unk5E = 1;
}

void InitCortexPack(NAS *m) {
    m->Engine->sub->unk6C = D_80052EEC;
    m->Engine->sub->unk74 = 0x2003;
    m->Engine->unk5D = 2;
    m->Engine->unk5C = 1;
    m->Engine->sub->flags |= 0x40000021;
    m->Engine->sub->unk76 = 0;
}

void InitBrioPack(NAS *m) {
    m->Engine->flags |= 0x200000;
    m->Engine->sub->unk6C = D_800546C4;
    m->Engine->sub->unk74 = 0x2031;
    m->Engine->unk5D = 2;
    m->Engine->unk5C = 1;
    m->Engine->sub->flags |= 0x40000021;
    m->Engine->sub->unk76 = 0;
    m->UserInfo.user2 = 0;
    m->UserInfo.user1 = 0;
}

void InitDingoTail(NAS *m) {
    m->Engine->flags |= 0x200000;
    m->Engine->sub->unk6C = (s32)D_80052EEC;
    m->Engine->sub->unk74 = 0x2003;
    m->Engine->unk5D = 2;
    m->Engine->unk5C = 1;
    m->Engine->sub->flags |= 0x40000021;
    m->Engine->sub->unk76 = 0;
}

void PlotSimpleExplosion(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        sub = m->Engine->sub;
        ext->unk104++;
        if (sub != NULL) {
            i = 0;
            do {
                sub->unk04 += ext->vel[i].x;
                sub->unk08 += ext->vel[i].z;
                sub->unk0C += ext->vel[i].y;
                sub->unk76 += 0x100;
                sub->unk20.x -= 0x80;
                sub->unk20.y -= 0x80;
                sub->unk20.z -= 0x80;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
        if (ext->unk104 == 0x2D) {
            KillNAS(m);
        }
    }
}

void InitSmokeTrail(NAS *m) {
    ExtSmokeTrail *ext;

    ext = AddExtension(m, 0x30, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk6C = D_800546C4;
        m->Engine->sub->unk74 = 0x2020;
        m->Engine->sub->flags |= 0x40000041;
        m->Engine->sub->unk76 = 0x1000;
        m->UserInfo.status = 0;
        ext->fadeIn = 0x400;
        ext->fadeOut = 0xC8;
        ext->shrink = 0x60;
        ext->flags = 0;
        ext->baseX = m->Engine->unk10.x;
        m->Engine->unk5C = 1;
        m->Engine->unk5D = 0;
        ext->angle = Random(0x1000);
    }
}

void PlotBearDeath(NAS *m) {
    BearDeathExt *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        func_800250B0(m);
        m->Engine->unk1C += 0x100;
        m->Engine->unk5E = 1;
        if (ext->unk40 > 0) {
            m->Engine->sub->unk68 = 0x200;
        }
        if (++ext->unk80 == 0x1F) {
            func_80026980(m);
        }
        if (--ext->unk84 < 0) {
            SpawnNASById(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1612, 0x11);
            ext->unk84 = Random(7) + 1;
        }
        if (func_8008C5C4(&ext->from.x, &ext->to.x, &ext->unk2C, &m->Engine->unk10.x) != 0) {
            KillNAS(m);
        }
    }
}

void InitBearDeath(NAS *m) {
    BearDeathExt *ext;

    ext = AddExtension(m, 0x88, 0x10000000);
    if (ext != NULL) {
        func_80024E30(m, &D_800D56D4);
        m->Engine->sub->unk6C = *(s32 *)D_80058AD0;
        m->Engine->unk5D = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk2C = 2;
        ext->unk70 = 0x32;
        ext->unk44 = 0x100;
        ext->unk30 = 0;
        ext->unk80 = 0;
        ext->unk84 = 5;
        func_80026A04((u8 *)m);
    }
}

void func_800CA6E8(Ovl62RiserSet *arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (arg0->slot[i].active != 0 && arg0->slot[i].height < 0x74) {
            arg0->slot[i].height += 4;
            D_800D6150[i]->y = arg0->slot[i].base + arg0->slot[i].height;
        }
    }
}

void PlotBreathMaster(NAS *m) {
}

void InitBreathMaster(NAS *m) {
}

void PlotSnowMaster(NAS *m) {
}

void InitSnowMaster(NAS *m) {
}

void func_800CA770(void) {
    s32 i;
    SceneObj *obj;
    SceneObj **out;

    obj = *(SceneObj **)((u8 *)D_80058AD0 + 0x1C);
    for (i = 79; i >= 0; i--) {
        D_800D6010[i] = NULL;
    }
    i = 0;
    out = D_800D6010;
    while (1) {
        if (obj == NULL) {
            break;
        }
        if (obj->flags & 0x8000) {
            if (obj->model->colors != NULL) {
                u8 *p = func_80015A18(obj->index, (u8 *)obj->model->colors);
                if (p != NULL && *(s32 *)(p + 0x28) != 0) {
                    *out++ = obj;
                    i++;
                }
            }
            obj = obj->next;
        }
    }
    printf(D_800B3498, i - 1);
}

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

void InitScratchMaster(NAS *m) {
    EXTengine *eng;
    subengine *sub;

    if (func_80018F3C(m->Engine, 0x10) != 0) {
        eng = m->Engine;
        sub = eng->sub;
        eng->flags |= 0x200000;
        m->UserInfo.user1 = 0;
        m->UserInfo.user2 = 0;
        while (sub != NULL) {
            sub->flags = 0x40000000;
            sub->unk6C = D_800546C4;
            sub->unk74 = 0x2002;
            sub->unk51 = 2;
            sub->unk14 = 0;
            sub->unk12 = 0;
            sub->unk10 = 0;
            sub->unk76 = 0x800;
            sub = sub->next;
        }
    }
}

void func_800CA930(EXTheromove *a, EXTheromove *b) {
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

void PlotGravityObject(GravityObject *obj) {
    s32 target = obj->target;

    if (target != 0) {
        func_800BFD38(obj, target);
    }
}

void InitGravityObject(NAS *m) {
    GravityMotion *g;
    s32 t;

    g = AddExtension(m, 0x34, 0x10000000);
    if (g != NULL) {
        m->Engine->sub->unk74 = 0x200C;
        m->Engine->sub->unk6C = D_800546C4;
        m->Engine->sub->flags |= 0x02000000;
        g->vx = (D_80068BD4[0xE00].unk2 * 27) >> 11;
        t = D_80068BD4[0x200].unk2;
        g->vy = -0x54;
        g->ax = 0;
        g->az = 0;
        g->ay = 2;
        g->flags = 0;
        g->vz = (t * 27) >> 11;
        g->floor = *(u16 *)&m->Engine->unk10.y + 0x400;
        m->FUX = (EXT *)g;
    }
}

void InitWeightDeath(NAS *m) {
    WeightDeathExt *ext;

    ext = AddExtension(m, 0x28, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk6C = D_800546C4;
        m->Engine->sub->unk74 = 0x2003;
        ext->unk10 = 0x40;
        ext->unk14 = 8;
        ext->unk18 = 0;
        ext->unk20 = 0;
        ext->unk24 = 0;
        PlaySfx(0x453, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

s32 func_800CAC20(s32 i) {
    u8 *p;
    EXTengine *eng;
    EXTengine *e;
    HeroInfo *base;

    p = D_800D5454;
    base = Heroes;
    eng = base[i].hero_eng;
    for (; p != NULL; p = *(u8 **)(p + 0x20)) {
        e = *(EXTengine **)(p + 0x54);
        if (e != NULL && e != eng) {
            if (func_800C2ECC(e, 1, base[i].hero_eng, 0) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

void func_800CACC0(void) {
    ((Ovl62Cam *)D_80051640)->at.x = -0x40;
    ((Ovl62Cam *)D_80051640)->at.y = -0x340;
    ((Ovl62Cam *)D_80051640)->at.z = -0x8E0;
    ((Ovl62Cam *)D_80051640)->rotX = 0xC6;
    ((Ovl62Cam *)D_80051640)->rotY = 0;
    ((Ovl62Cam *)D_80051640)->rotZ = 0;
    ((Ovl62Cam *)D_80051640)->eye.x = 0;
    ((Ovl62Cam *)D_80051640)->eye.z = 0x7FF;
    ((Ovl62Cam *)D_80051640)->eye.y = 0;
    ((Ovl62Cam *)D_80051640)->dist = 0x10F;
}

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

void func_800CAD60(s32 player) {
    Ovl62Fux *fux;
    HeroInfo *base;
    HeroInfo *h;

    base = Heroes;
    h = base + player;
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux != NULL) {
        h->hero_ai->AIpad = 0;
        func_800B9D48(player);
        ((void (*)(s32))fux->func)(player);
        func_80013C94((u8 *)&D_80051338[player], h->hero_ai->AIpad);
    }
}

void func_800CAE10(s32 player, s32 target) {
    Ovl62Fux *fux;
    s32 r;

    fux = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (func_800BCF08(player, target) == 0) {
        return;
    }
    r = func_800B9B6C(player);
    if (r == -1) {
        fux->state = 5;
        fux->func = func_800CB6B4;
        return;
    }
    fux->target = r;
}

void func_800CAEB0(s32 player, s32 idx) {
    Ovl62Prox *p;
    Ovl62Mode *m;
    EXTengine *a;
    EXTengine *b;
    s32 dx;
    s32 dz;
    s32 mode;

    if (D_8005A648 == 0x12) {
        p = D_800D5494;
        if (p != NULL) {
            m = p->unk58;
            if (m != NULL) {
                a = p->eng;
                b = Heroes[player].hero_eng;
                dx = a->unk10.x - b->unk10.x;
                dz = a->unk10.z - b->unk10.z;
                if (dx * dx + dz * dz <= 0xB0643) {
                    mode = m->mode;
                    if (mode == 1 || mode == 5) {
                        D_800D5FEC[idx] |= 0x100;
                    }
                }
            }
        }
    }
}

s32 func_800CAF94(s32 state) {
    Ovl62Fux *f;
    s32 i;
    s32 n;

    HeroInfo *h;

    n = 0;
    i = 0;
    h = Heroes;
loop:
    if (i < 4) {
        f = (Ovl62Fux *)h->hero->FUX;
        if (f != NULL && f->state == state) {
            n++;
        }
        h++;
        i++;
        goto loop;
    }
    return n;
}

s32 func_800CAFF0(void) {
    return 0;
}

s32 func_800CAFF8(s32 player) {
    Ovl62Fux *fux;
    HeroInfo *h;
    HeroInfo *base;

    base = Heroes;
    h = base + player;
    fux = (Ovl62Fux *)h->hero->FUX;
    if (fux == NULL) {
        return 0;
    }
    if (Random(0x1000) >= D_800D4E0C[h->hero_name + 7]) {
        return 0;
    }
    fux->state = 6;
    fux->func = func_800B7F70;
    return 1;
}

void func_800CB0A4(void) {
    Ovl62Link *l;

    if (D_800D5254 != NULL) {
        l = D_800D5254->unk58;
        if (l != NULL) {
            if (D_800D5FEC[6] == 0 && l->unk10 != -1) {
                D_800D5FEC[6] = 1;
            }
            if (D_800D5FEC[6] == -1 && l->unk10 == D_800D5FEC[6]) {
                D_800D5FEC[6] = 0;
            }
        }
    }
}

void func_800CB124(void) {
}

void func_800CB12C(void) {
}

void func_800CB134(s32 player) {
    Ovl62Fux *f;

    f = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (f != NULL) {
        f->unk24 = 0x5A;
    }
}

s32 func_800CB178(s32 self, s32 *dist) {
    SLONG_XYZ pos;
    s32 best;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    s32 i;
    s32 d;
    s32 min;
    u16 st;

    best = -1;
    min = 0x07FFFFFF;
    i = 0;
    base = Heroes;
    me = &base[self];
    h = base;
loop:
    if (i < 4) {
        if (i != self &&!((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
            pos = h->hero_eng->unk10;
            d = func_8008CEB0((u8 *)&pos, (u8 *)&me->hero_eng->unk10);
            if (d < min) {
                min = d;
                do {
                    best = i;
                } while (0);
            }
        }
        do {
            h++;
        } while (0);
        i++;
        goto loop;
    }
    *dist = min;
    return best;
}

s32 func_800CB284(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_800D5FEC[i] & 8) {
            return 1;
        }
        if (D_800D5FEC[i] & 0x20) {
            return 1;
        }
    }
    return 1;
}

s32 func_800CB2C8(s32 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (i == arg0 || (D_800D5FEC[i] & 4)) {
            return 0;
        }
        if (D_800D5FEC[i] & 2) {
            return 0;
        }
    }
    return 1;
}

void func_800CB314(s32 player) {
    Ovl62Fux *fux;

    fux = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (fux != NULL) {
        fux->unk2C = 0;
        fux->unk3C = 0;
        fux->unk34 |= 1;
    }
}

void func_800CB368(s32 player) {
    Ovl62Fux *f;
    s32 j;
    s32 n;

    f = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (f == NULL) {
        return;
    }
    for (n = j = 0; j < 4; j++) {
        if (Heroes[j].hero_status & 0x2000) {
            n++;
        }
        f->state = 4;
        f->func = func_800CB12C;
        if (n >= 3) {
            return;
        }
    }
    f->unk3C = 1;
    f->unk24 = Random(0x3C) + 5;
}

void func_800CB41C(s32 player) {
    Ovl62Fux *f;
    HeroInfo *h;
    HeroInfo *base;
    SLONG_XYZ v;
    s32 r;
    s32 spd;

    r = Random(0x1000);
    base = Heroes;
    h = &base[player];
    f = (Ovl62Fux *)h->hero->FUX;
    spd = 0x708;
    if (f != NULL) {
        v.x = (D_80068BD4[r & 0xFFF].unk2 * spd) >> 12;
        v.z = (D_80068BD4[(r - 0x400) & 0xFFF].unk2 * spd) >> 12;
        f->vel = v;
        f->pos = h->hero_eng->unk10;
    }
}

void func_800CB508(s32 player) {
    Ovl62Fux *fux;

    fux = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (fux != NULL) {
        fux->unk24 = Random(0x168) + 0x5A;
    }
}

s32 func_800CB56C(s32 arg0) {
    NAS *nas;
    BombMakerExt *ext;
    s32 one;

    nas = D_800D5294;
    one = 1;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->unkC == one && ext->unk14 == arg0) {
            return 1;
        }
        nas = nas->Group.Child;
        goto loop;
    }
    return 0;
}

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

void func_800CB650(s32 player) {
    Ovl62Fux *fux;

    fux = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (fux != NULL) {
        fux->unk24 = Random(0x168) + 0x5A;
    }
}

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

void InitBombGFX(NAS *m) {
    BombGFXExt *ext;
    EXTengine *e;

    ext = AddExtension(m, 0x1C, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk74 = 0x2018;
        m->Engine->sub->unk6C = D_800546C4;
        ext->unkC = 0;
        m->Engine->unk5C = 1;
        func_80026B70((u8 **)&D_800D5294, (u8 *)m);
        m->FUX = NULL;
        e = AddEngine(m, 2);
        if (e != NULL) {
            e->sub->unk6C = D_800546C4;
            e->sub->unk74 = 0x202F;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->flags |= 0x8041;
            e->sub->unk76 = 0x200;
            e->sub->flags |= 0x40000000;
            m->FUX = (EXT *)e;
        }
    }
}

void PlotBombMaker(NAS *m) {
    BombMakerExt *ext;
    s32 x;
    s32 z;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        if (ext->unk18 < 0) {
            ext->unk18 = 0;
        }
        switch (ext->unkC) {
        case 0:
            if (ext->unk18 <= 0) {
                if (ext->timer-- < 0 && D_800D4DEC == 0) {
                    ext->unkC = 2;
                    ext->timer = 0x5A;
                    ext->unk18++;
                }
            }
            break;
        case 2:
            x = Random(0x800) - Random(0x800);
            z = Random(0x800);
            SpawnNASById(x, 0, z - Random(0x800), 0x160F, 0x11);
            PlaySfx(0x3FA, NULL, NULL, 0x1000, 0x1E00, 0);
            ext->unkC = 0;
            break;
        }
    }
}

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

void func_800CBA64(void) {
    D_8005AA70[0] = (s32)func_800B5AC4;
    D_8005AA70[1] = (s32)func_800BAFA0;
    D_8005AA70[2] = (s32)func_800BAFA0;
    D_8005AA70[3] = (s32)func_800BB810;
    D_8005AA70[6] = (s32)func_800CAD14;
}

s32 func_800CBAA4(s32 arg0, s32 arg1) {
    s32 total;
    s32 result;
    s32 i;

    result = 0;
    total = 0;
    if (Heroes[arg1].hero_status & 0x2000) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        if (i != arg0 && !(Heroes[i].hero_status & 0x2000)) {
            total += D_8009D87C[i].unk4 << 7;
        }
    }
    if (total != 0) {
        result = (D_8009D87C[arg1].unk4 << 19) / total;
    }
    return result;
}

void func_800CBB8C(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_ai->AImode != 0) {
            D_800D5FB0[i] = 0x1000;
        } else {
            D_800D5FB0[i] = 0x2000;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CBBE8);

void func_800CC024(s32 player, EXTheromove *other) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[player];
    EXTheromove *move = hero->hero_move;
    s32 m1 = move->mass;
    s32 m2 = other->mass;
    s32 k = 0x100;
    s32 v1;
    s32 v2;
    s32 d;
    s32 a;
    s32 b;

    if (m1 > 0 && m2 > 0) {
        if (hero->hero_mode == 0x51 && hero->hero_eng->unk2C.x > 0xFA0) {
            m1 += 0x220;
        }
        v1 = move->x_vel;
        v2 = other->x_vel;
        d = -(k * (v1 - v2));
        a = d * m2 + ((m1 * v1 + m2 * v2) << 8);
        a /= m2 + m1;
        b = a - d;
        if (a < 0) {
            if (a < -0x9600) {
                a = -0x9600;
            }
        } else if (a > 0x9600) {
            a = 0x9600;
        }
        if (b < 0) {
            if (b < -0x9600) {
                b = -0x9600;
            }
        } else if (b > 0x9600) {
            b = 0x9600;
        }
        move->x_vel = -(a >> 8);
        other->x_vel = -(b >> 8);
        v1 = move->z_vel;
        v2 = other->z_vel;
        d = -(k * (v1 - v2));
        a = d * m2 + ((m1 * v1 + m2 * v2) << 8);
        a /= m2 + m1;
        b = a - d;
        if (a < 0) {
            if (a < -0x9600) {
                a = -0x9600;
            }
        } else if (a > 0x9600) {
            a = 0x9600;
        }
        if (b < 0) {
            if (b < -0x9600) {
                b = -0x9600;
            }
        } else if (b > 0x9600) {
            b = 0x9600;
        }
        move->z_vel = -(a >> 8);
        other->z_vel = -(b >> 8);
    }
}

void func_800CC2BC(s32 player) {
    u8 *g = D_8005A614;
    HeroInfo *hero;
    EXTengine *shadow;
    EXTengine *eng;
    s32 *p;

    if (*(s16 *)(g + 0x34) != 0x15 && g[player + 0x50] != 9) {
        HeroInfo *base = Heroes;
        s32 *tbl;

        hero = &base[player];
        shadow = hero->hero_shadow;
        if (shadow != NULL) {
            eng = hero->hero_eng;
            shadow->flags = 0x8000;
            shadow->unk5C = 1;
            if (eng != NULL) {
                shadow->sub->unk6C = D_80054654;
                tbl = D_800D6934;
                p = &tbl[player];
                *p = 0x4000;
                shadow->sub->unk74 = *p;
                shadow->sub->unk68 = 0x80;
                shadow->sub->flags |= 0x02000000;
                shadow->unk10 = eng->unk10;
                shadow->unk5D = 0;
                shadow->sub->flags = 0x8000;
                shadow->unk5E = 1;
            }
        }
    }
}

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

s32 func_800CD560(s32 arg0) {
    Heroes[arg0].hero_move->HitBy = -1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CD594);

void func_800CDA54(s32 player) {
    Ext20 *ext;
    Ext20 *ext2;

    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL) {
        ext->val += 4;
        if (ext->val > 0x100) {
            ext->val = 0x100;
        }
    }
    func_800BBF2C(player);
    func_8008502C(player);
    ext2 = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext2 != NULL && (((Ovl62Pad *)D_80051338)[player].unk50 & 0xF0)) {
        if (ext2->state == 0) {
            SetHeroAction((HeroInfo *)player, 0x4A);
        }
        if (ext2->state == 1) {
            SetHeroAction((HeroInfo *)player, 0x55);
        }
    }
    func_800D0650(player);
    func_800D10A0(player);
    func_800CD214(player);
    func_800C0888(&Heroes[player].hero_eng->unk10, player);
    func_800D2D2C(player);
}

void func_800CDBD0(s32 player) {
    HeroInfo *h;
    HeroInfo *base;
    Ext20 *ext;
    Ext20 *ext1;

    ext1 = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext1 != NULL) {
        ext1->val += 4;
        if (ext1->val > 0x100) {
            ext1->val = 0x100;
        }
    }
    func_800BBF2C(player);
    base = Heroes;
    h = &base[player];
    if (h->hero_anim->Aindex == 4) {
        if (h->hero_move->mass > 0x50) {
            if (player == 0) {
                PlaySfx(0x49C, &base[0].hero_eng->sfx, &base[0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
            if (player == 1) {
                PlaySfx(0x49D, &base[1].hero_eng->sfx, &base[1].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
            if (player == 2) {
                PlaySfx(0x49E, &base[2].hero_eng->sfx, &base[2].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
            if (player == 3) {
                PlaySfx(0x49F, &base[3].hero_eng->sfx, &base[3].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
        } else {
            PlaySfx(0x4A0, &h->hero_eng->sfx, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        }
    }
    func_8008502C(player);
    if (!(((Ovl62Pad *)D_80051338)[player].unk50 & 0xF0)) {
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL) {
            if (ext->state == 0) {
                SetHeroAction((HeroInfo *)player, 0x49);
            }
            if (ext->state == 1) {
                SetHeroAction((HeroInfo *)player, 0x54);
            }
        }
    }
    func_800D0650(player);
    func_800D10A0(player);
    func_800CD214(player);
    func_800C0888(&Heroes[player].hero_eng->unk10, player);
    if (func_800D2D2C(player) != 0) {
        if (Heroes[player].hero_eng->sfx != NULL) {
            StopSfx(Heroes[player].hero_eng->sfx);
        }
    }
}

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

void func_800CFB7C(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;
    EXTheromove *move;

    if (func_800D2D2C() == 0) {
        base = Heroes;
        hero = &base[player];
        move = hero->hero_move;
        if (move != NULL) {
            ((Ovl62Pad *)D_80051338)[player].unk50 = 0;
            ((Ovl62Pad *)D_80051338)[player].unk54 = 0;
            func_800BBF2C(player);
            func_800CD214(player);
            func_800C0888(&hero->hero_eng->unk10, player);
            hero->hero_frozen = 2;
            hero->hero_eng->unk1E += 0x10;
            hero->hero_eng->unk5E = 1;
            move->x_max = 0;
            move->z_max = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800CFC58);

void func_800CFF44(s32 player) {
    HeroInfo *hero;
    EXTheromove *move;
    HeroInfo *base;
    s32 angle;

    base = Heroes;
    hero = &base[player];
    move = hero->hero_move;
    if (move != NULL) {
        angle = (get_angle(hero->hero_eng->unk10.x - D_800D4DD4.vx,
                           hero->hero_eng->unk10.z - D_800D4DD4.vz) + 0x800) & 0xFFF;
        move->forward_angle = angle + 0x800;
        move->x_max = 0;
        move->z_max = 0;
        move->x_vel = (D_80068BD4[(angle - 0x400) & 0xFFF].unk2 * 0x25) >> 11;
        move->z_vel = (D_80068BD4[angle].unk2 * 0x25) >> 11;
        hero->hero_eng->unk10.x += move->x_vel;
        hero->hero_eng->unk10.z += move->z_vel;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D0060);

void func_800D034C(s32 player) {
    Ext20 *ext;
    EXTheromove *m;

    Heroes[player].hero_eng->sub->flags &= 0xFFBFFFFF;
    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL) {
        if (ext->state == 0) {
            SetHeroAction((HeroInfo *)player, 0x49);
        }
        if (ext->state == 1) {
            SetHeroAction((HeroInfo *)player, 0x54);
        }
    }
    D_800D6934[player] = 0x4000;
    Heroes[player].hero_eng->sub->unk68 = 0;
    m = Heroes[player].hero_move;
    if (m != NULL) {
        m->z_dir = 0;
        m->x_max = 0;
        m->z_max = 0;
        m->x_vel = 0;
        m->z_vel = 0;
    }
}

void func_800D0470(s32 player) {
    HeroInfo *base;
    HeroInfo *h;

    base = Heroes;
    h = &base[player];
    if (h->hero_shadow != NULL) {
        h->hero_shadow->sub->flags &= ~0x8000;
    }
    if (D_800D4D08 == 0) {
        PlaySfx(0x45D, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    func_80026980(h->hero);
    h->hero_eng->sub->flags &= ~0x8000;
    if (!(h->hero_status & 0x200)) {
        h->hero_status |= 0x8000;
        if (!(h->hero_status & 0x200)) {
            h->hero_status |= 0x2000;
        }
    }
    if (D_8005A648 == 0x15) {
        FindExtension(Heroes[player].hero, 0x20000000);
    }
}

s32 func_800D05A8(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    s32 *ext = FindExtension(info->hero, 0x20000000);

    if (ext != NULL) {
        if (ext[9] != 0) {
            return 0x80;
        }
        switch (info->hero_name) {
        case 3:
        case 4:
            return 0x100;
        case 5:
        case 6:
            return 0xB4;
        case 7:
        case 8:
        default:
            return 0x80;
        }
    }
    return 0x80;
}

s32 func_800D0650(s32 player) {
    HeroInfo *base;
    HeroInfo *h;
    Ext20 *ext;
    Ovl62Pad *pads;
    Ovl62Pad *pad;

    base = Heroes;
    h = base + player;
    ext = FindExtension(h->hero, 0x20000000);

    if (ext != NULL && h->hero_move != NULL && ext->val >= func_800D05A8(player)) {
        pads = (Ovl62Pad *)D_80051338;
        pad = pads + player;
        if (pad->unk54 & 0x8000) {
            if (h->hero_mode != 0x51) {
                SetHeroAction((HeroInfo *)player, 0x51);
            }
            return 1;
        } else {
            pad->unk50 &= ~0x8000;
            return 0;
        }
    }
    return 0;
}

s32 func_800D0750(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    s32 *ext = FindExtension(info->hero, 0x20000000);

    if (ext != NULL) {
        if (ext[9] != 0) {
            return 0x46;
        }
        switch (info->hero_name) {
        case 7:
        case 8:
            return 0x4B;
        case 5:
        case 6:
            return 0x69;
        case 3:
        case 4:
            return 0x82;
        default:
            return 0x5A;
        }
    }
    return 0;
}

void func_800D0808(s32 player) {
    HeroInfo *h;
    HeroInfo *h2;
    EXTheromove *move;
    Ext20 *ext;
    HeroInfo *base;
    HeroInfo *base2;

    base = Heroes;
    h = &base[player];
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL && func_800D2D2C(player) == 0) {
        if (ext->state == 1) {
            switch (h->hero_name) {
            case 1:
                PlaySfx(0x1A, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x4C, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x80, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 4:
                PlaySfx(0xB0, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 7:
                PlaySfx(0x114, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 8:
                PlaySfx(0xE2, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 5:
                PlaySfx(0x146, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 6:
                PlaySfx(0x178, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            }
        } else {
            switch (h->hero_name) {
            case 1:
                PlaySfx(0x16, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x48, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x7A, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 4:
                PlaySfx(0xAC, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 7:
                PlaySfx(0x110, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 8:
                PlaySfx(0xDE, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 5:
                PlaySfx(0x142, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 6:
                PlaySfx(0x174, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            }
        }
        base2 = Heroes;
        h2 = base2 + player;
        move = h2->hero_move;
        if (move != NULL) {
            if (ext->state == 0) {
                SetHeroSeq((HeroInfo *)player, 0x3E);
            }
            if (ext->state == 1) {
                SetHeroSeq((HeroInfo *)player, 0x45);
            }
            D_800D6934[player] = 0x4080;
            h2->hero_eng->sub->unk68 = 0;
            ext->val -= func_800D05A8(player);
            move->x_vel = 0;
            move->z_vel = 0;
            move->x_max = 0;
            move->z_max = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D0A6C);

s32 func_800D10A0(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    s32 *ext = FindExtension(info->hero, 0x20000000);
    Ovl62Pad *pads;
    Ovl62Pad *pad;

    if (ext != NULL) {
        if (ext[10] == 0) {
            return 0;
        }
        if (info->hero_move != NULL) {
            pads = (Ovl62Pad *)D_80051338;
            pad = pads + player;
            if (pad->unk54 & 0x2000) {
                if (info->hero_mode != 0x53) {
                    SetHeroAction((HeroInfo *)player, 0x53);
                }
                return 1;
            }
            pad->unk50 &= ~0x2000;
            return 0;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D1188);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D14CC);

void func_800D17E8(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    EXTheromove *move;
    Ext20 *ext;
    s32 speed;

    move = info->hero_move;
    if (move != NULL && func_800D2D2C() == 0) {
        speed = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
        ext = FindExtension(info->hero, 0x20000000);
        if (ext != NULL) {
            ext->val += 4;
            if (ext->val > 0x100) {
                ext->val = 0x100;
            }
        }
        func_800BBF2C(player);
        Heroes[player].hero_eng->unk1C += speed;
        func_800CD214(player);
        func_800C0888(&Heroes[player].hero_eng->unk10, player);
        Heroes[player].hero_eng->unk5E = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D191C);

void func_800D1C14(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    Ext20 *ext;
    Ext20 *ext1;
    Ext20 *ext2;

    ext1 = FindExtension(info->hero, 0x20000000);
    if (ext1 != NULL) {
        ext1->state = 1;
    }
    if (func_800D2D2C(player) == 0) {
        ext2 = FindExtension(info->hero, 0x20000000);
        if (ext2 != NULL) {
            ext2->val += 4;
            if (ext2->val > 0x100) {
                ext2->val = 0x100;
            }
        }
        func_800BBF2C(player);
        func_8008502C(player);
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL && (((Ovl62Pad *)D_80051338)[player].unk50 & 0xF0)) {
            if (ext->state == 0) {
                SetHeroAction((HeroInfo *)player, 0x4A);
            }
            if (ext->state == 1) {
                SetHeroAction((HeroInfo *)player, 0x55);
            }
        }
        func_800D0650(player);
        func_800D10A0(player);
        func_800CD214(player);
        func_800C0888(&Heroes[player].hero_eng->unk10, player);
    }
}

void func_800D1DB4(s32 player) {
    HeroInfo *h;
    HeroInfo *base;
    Ext20 *ext;
    Ext20 *ext1;

    if (func_800D2D2C() == 0) {
        ext1 = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext1 != NULL) {
            ext1->val += 4;
            if (ext1->val > 0x100) {
                ext1->val = 0x100;
            }
        }
        func_800BBF2C(player);
        base = Heroes;
        h = &base[player];
        if (h->hero_anim->Aindex == 4) {
            if (h->hero_move->mass != 0x50) {
                if (player == 0) {
                    PlaySfx(0x49C, &base[0].hero_eng->sfx, &base[0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (player == 1) {
                    PlaySfx(0x49D, &base[1].hero_eng->sfx, &base[1].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (player == 2) {
                    PlaySfx(0x49E, &base[2].hero_eng->sfx, &base[2].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (player == 3) {
                    PlaySfx(0x49F, &base[3].hero_eng->sfx, &base[3].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
            } else {
                PlaySfx(0x4A0, &h->hero_eng->sfx, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
        func_8008502C(player);
        if (!(((Ovl62Pad *)D_80051338)[player].unk50 & 0xF0)) {
            ext = FindExtension(Heroes[player].hero, 0x20000000);
            if (ext != NULL) {
                if (ext->state == 0) {
                    SetHeroAction((HeroInfo *)player, 0x49);
                }
                if (ext->state == 1) {
                    SetHeroAction((HeroInfo *)player, 0x54);
                }
            }
        }
        func_800D0650(player);
        func_800D10A0(player);
        func_800CD214(player);
        func_800C0888(&Heroes[player].hero_eng->unk10, player);
    }
}

void func_800D2044(s32 player) {
    HeroInfo *h;
    Ext20 *ext;
    HeroInfo *base = Heroes;

    h = &base[player];
    if (h->hero_name == 1) {
        h->hero_eng->sub->unk6C = D_800547DC;
    }
    if (h->hero_name == 2) {
        h->hero_eng->sub->unk6C = D_80054814;
    }
    if (h->hero_name == 5) {
        h->hero_eng->sub->unk6C = D_80054884;
    }
    if (h->hero_name == 3) {
        h->hero_eng->sub->unk6C = D_8005484C;
    }
    if (h->hero_name == 7) {
        h->hero_eng->sub->unk6C = D_800548BC;
    }
    if (h->hero_name == 4) {
        h->hero_eng->sub->unk6C = D_800548F4;
    }
    if (h->hero_name == 6) {
        h->hero_eng->sub->unk6C = D_80054AEC;
    }
    if (h->hero_name == 8) {
        h->hero_eng->sub->unk6C = D_80054AB4;
    }
    h->hero_eng->sub->flags &= 0xFFBFFFFF;
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
        if (ext->state == 0) {
            ext->state = 1;
        } else if (!(h->hero_status & 0x200)) {
            h->hero_status |= 0x2000;
        }
        SetHeroSeq((HeroInfo *)player, 0x3F);
        func_80028018(player, 0xA);
        ((void (*)(s32, s32, s32))func_80027F7C)(player, 0xA, 0xFF);
    }
}

void func_800D2248(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    s32 *ext;
    u16 status;
    EXTengine *eng;

    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
        ext[0x9C / 4] = 0;
        ext[0x98 / 4] = 0;
        h->hero_eng->unk5D = 2;
        status = h->hero_status;
        if (!(status & 0x200)) {
            h->hero_status = status | 0x2000;
        }
        h->hero_eng->sub->flags |= 0x8000;
        h->hero_eng->sub->unk6C = D_800546C4;
        h->hero_eng->sub->unk74 = 0x202D;
        eng = h->hero_eng;
        SpawnNASById(eng->unk10.x + 0x80, eng->unk10.y - 0x200, eng->unk10.z, 0x1613, 0x11);
    }
}

void func_800D2334(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + player;
    s32 *ext;
    s32 *p;
    EXTheromove *move;
    EXTengine *eng;
    NAS *nas;
    s32 z;
    s32 y;
    s32 x;
    s32 t;

    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
        move = h->hero_move;
        if (move != NULL) {
            move->z_vel = 0;
            move->x_vel = 0;
            move->z_max = 0;
        }
        eng = h->hero_eng;
        t = ext[0x9C / 4] + 1;
        x = eng->unk10.x;
        y = eng->unk10.y;
        z = eng->unk10.z;
        ext[0x9C / 4] = t;
        if (t == 4) {
            SpawnNASById(x - 0x80, y - 0x100, z, 0x1613, 0x11);
            SpawnNASById(x, y - 0x100, z, 0x1614, 0x11);
        }
        if (ext[0x9C / 4] == 2) {
            SpawnNASById(x + 0x80, y - 0x180, z, 0x1613, 0x11);
        }
        if (ext[0x9C / 4] == 0x3C) {
            SetHeroAction((HeroInfo *)player, 0x50);
        }
        if (--ext[0x98 / 4] < 0) {
            ext[0x98 / 4] = Random(7) + 1;
            nas = SpawnNASById(x + 0x40, y - 0x40, z, 0x1612, 0x11);
            if (nas != NULL) {
                p = FindExtension(nas, 0x10000000);
                if (p != NULL) {
                    p[0x1C / 4] = 0xA;
                    p[0xC / 4] = 0x400;
                    p[0x10 / 4] = 0x50;
                    p[0x14 / 4] = -0x40;
                    p[0x24 / 4] = 0x20;
                    p[0x28 / 4] = 1;
                    p[0x18 / 4] |= 1;
                }
            }
            nas = SpawnNASById(x - 0x40, y - 0x40, z, 0x1612, 0x11);
            if (nas != NULL) {
                p = FindExtension(nas, 0x10000000);
                if (p != NULL) {
                    p[0x1C / 4] = 0xA;
                    p[0xC / 4] = 0x258;
                    p[0x10 / 4] = 0x50;
                    p[0x14 / 4] = -0x40;
                    p[0x24 / 4] = 0x20;
                    p[0x28 / 4] = 1;
                    p[0x18 / 4] |= 1;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl62/ovl62", func_800D257C);

void func_800D2B58(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = base + player;
    EXTheromove *move;

    hero->hero_eng->unk5E = 1;
    hero->hero_eng->unk2C.x = 0x1000;
    hero->hero_eng->unk2C.y = 0x1000;
    hero->hero_eng->unk2C.z = 0x1000;
    move = hero->hero_move;
    if (move != NULL) {
        hero->hero_eng->unk1C = 0;
        hero->hero_eng->unk5E = 1;
        func_80085890(player);
        move = hero->hero_move;
        hero->hero_frozen = 0x14;
        if (move != NULL) {
            move->x_max = 0;
            move->z_max = 0;
            move->x_vel = 0;
            move->z_vel = 0;
        }
        if (func_8008A200(player) != 0) {
            func_8008A810();
        }
        func_8008502C(player);
    }
}

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

void func_800D2CB8(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    u16 status;

    if (FindExtension(h->hero, 0x20000000) != NULL) {
        h->hero_eng->unk5D = 2;
        status = h->hero_status;
        if (!(status & 0x200)) {
            h->hero_status = status | 0x2000;
        }
    }
}

s32 func_800D2D2C(arg0)
s32 arg0;
{
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    if (hero->hero_status & 0x200) {
        if (hero->hero_mode != 0x58) {
            SetHeroAction((HeroInfo *)arg0, 0x58);
        }
        return 1;
    }
    return 0;
}

void func_800D2D9C(s32 player) {
    func_8008502C(player);
    func_800CD214(player);
    func_800BBF2C(player);
    func_800C0888(&Heroes[player].hero_eng->unk10, player);
}

void func_800D2E00(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;

    base = Heroes;
    hero = base + player;
    if (hero->hero_eng->sfx != NULL) {
        StopSfx(hero->hero_eng->sfx);
    }
    if (hero->hero_move != NULL) {
        hero->hero_move->z_dir = 0;
    }
    SetHeroSeq((HeroInfo *)player, 0x43);
    D_800D6934[player] = 0x4100;
    hero->hero_eng->sub->unk68 = 0;
}

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

void func_800D2F48(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    s32 *ext = FindExtension(info->hero, 0x20000000);

    if (ext != NULL && func_800D2D2C(player) == 0 && info->hero_move != NULL) {
        func_800BBF2C(player);
        func_800D0A6C(player);
        ext[4] -= 0xF;
        if (ext[4] < 0) {
            ext[4] = 0;
        }
        func_8008502C(player);
        func_800CD214(player);
        func_800C0888(&info->hero_eng->unk10, player);
    }
}

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

void func_800D307C(s32 player) {
    PadState *base;
    Ovl62Pad *pad;

    base = D_80051338;
    pad = (Ovl62Pad *)(base + player);
    pad->unk50 = 0;
    pad->unk54 = 0;
    func_8008502C(player);
}

void func_800D30C0(s32 player) {
    u8 *g = D_8005A614;
    EXTheromove *mv;

    if (*(s16 *)(g + 0x34) != 0x16) {
        PadState *b = D_80051338;
        PadState *p = &b[player];
        *(s32 *)((u8 *)p + 0x50) = 0;
        *(s32 *)((u8 *)p + 0x54) = 0;
    }
    mv = Heroes[player].hero_move;
    if (mv != NULL) {
        mv->z_dir = 0;
        mv->x_max = 0;
        mv->z_max = 0;
        mv->x_vel = 0;
        mv->z_vel = 0;
    }
    if (*(s16 *)(g + 0x34) != 0x16) {
        func_800D2D2C(player);
    }
}

void func_800D3170(void) {
    if (D_8005A648 != 0x16) {
        func_800D2D2C();
    }
}

void func_800D31A0(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;

    if (func_800D2D2C() == 0) {
        base = Heroes;
        hero = base + player;
        if (hero->hero_move != NULL) {
            hero->hero_move->z_dir = 0;
        }
        if (hero->hero_eng->sfx != NULL) {
            StopSfx(hero->hero_eng->sfx);
        }
        SetHeroSeq((HeroInfo *)player, 0x3B);
        D_800D6934[player] = 0x4100;
        hero->hero_eng->sub->unk68 = 0;
    }
}

void func_800D325C(s32 arg0) {
    Ext20 *ext = FindExtension(Heroes[arg0].hero, 0x20000000);

    if (ext != NULL) {
        ext->val += 4;
        if (ext->val > 0x100) {
            ext->val = 0x100;
        }
    }
}

s32 func_800D32CC(s32 player) {
    Ext20 *ext;

    if (((Ovl62Pad *)D_80051338)[player].unk50 & 0xF0) {
        return 0;
    }
    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL) {
        if (ext->state == 0) {
            SetHeroAction((HeroInfo *)player, 0x49);
        }
        if (ext->state == 1) {
            SetHeroAction((HeroInfo *)player, 0x54);
        }
    }
    return 1;
}

s32 func_800D3394(s32 player) {
    Ext20 *ext;

    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL) {
        if (!(((Ovl62Pad *)D_80051338)[player].unk50 & 0xF0)) {
            return 0;
        }
        if (ext->state == 0) {
            SetHeroAction((HeroInfo *)player, 0x4A);
        }
        if (ext->state == 1) {
            SetHeroAction((HeroInfo *)player, 0x55);
        }
        return 1;
    }
    return 0;
}

void func_800D3460(s32 arg0) {
    HeroInfo *base = Heroes;
    s16 two;
    HeroInfo *h;
    EXTheromove *m;

    h = base + arg0;
    if ((m = h->hero_move) != NULL) {
        do {
            two = 2;
        } while (0);
        m->x_dec = 1;
        m->z_dec = 1;
        m->HitBy = -1;
        m->x_acc = two;
        m->z_acc = two;
        m->velocity_calc = 3;
        D_800D6924[arg0] = D_8009D6F4[h->hero_name * 3];
    }
}

void func_800D34F0(void) {
}

void func_800D34F8(void) {
}

void func_800D3500(void) {
}

void func_800D3508(void) {
}
