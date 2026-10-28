#include "common.h"
#include "bash/main.h"
#include "bash/bonk_ovl.h"
#include "bash/bonk_bohero.h"
#include "bash/bonk.h"

/* data, in address order */
s32 BonkShockwaveSfx[4] = { 0x72F, 0x730, 0x731, 0x732 };
s32 BonkBackswingSfx[4] = { 0x719, 0x71A, 0x71B, 0x71C };
s32 BonkSwingSfx[4] = { 0x721, 0x722, 0x723, 0x724 };
s32 BonkSwingHitSfx[4] = { 0x710, 0x711, 0x712, 0x713 };
BonkSlot BonkSlots[4] = {0};

void BonkSpawnShockwave(s32 arg0) {
    VECTOR sp18;
    HeroInfo *h;
    HeroInfo *base;
    EXTengine *pu;
    subengine *sub;
    BonkSlot *sbase;
    NAS *nas;
    BonkSlot *slot;

    base = Heroes;
    h = base + arg0;
    if (sub_find_point(h->hero_eng, h->hero_sub, 1, &sp18) == NULL) {
        *(SLONG_XYZ *)&sp18 = h->hero_eng->unk10;
    }
    nas = CreateNAS(sp18.vx, World_floor_level - 0x80, sp18.vz, 0x1A03, 0x11);
    sbase = BonkSlots;
    slot = sbase + arg0;
    if (slot->f8 == 1) {
        PlaySfx(0x72D, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        if (nas != NULL) {
            func_80028018(arg0, 5);
            nas->UserInfo.user2 = 0x8000;
            BonkMalletHit(&sp18, 0x600, arg0, 0x100);
            pu = h->hero_powerup;
            sub = NULL;
            if (pu != NULL) {
                sub = pu->sub;
            }
            if (sub != NULL) {
                pu->flags &= 0xFFFF7FFF;
            }
            slot->f8 = 3;
        }
    } else {
        BonkMalletHit(&sp18, 0x380, arg0, 0);
    }
    PlaySfx(BonkShockwaveSfx[arg0], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
}

void BonkHeroFacing(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    if (h->hero_frozen == 0 || h->hero_mode == 0x43) {
        if (!(h->hero_ctrl & 1)) {
            h->hero_eng->unk1E += angle_diff(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
        }
        h->hero_ctrl &= 0xFFFE;
        h->hero_eng->unk5E = 1;
    }
}

void BonkPreMoveHero(s32 arg0) {
    EXTheromove *m;

    m = Heroes[arg0].hero_move;
    m->forward_speed = 0;
    if (Heroes[arg0].hero_frozen != 0) {
        ((BonkPadButtons *)Pads)[arg0].unk50 &= 0x8000;
    }
    if ((((BonkPadButtons *)Pads)[arg0].unk0 & 1) || GameInfo[arg0 + 0x5C] != 0) {
        if (((BonkPadButtons *)Pads)[arg0].unk50 & 0xF0) {
            m->forward_angle = ((BonkPadButtons *)Pads)[arg0].unk38 + 0x800;
            if (m->velocity_enable != 0) {
                if (BonkSlots[arg0].f8 == 0) {
                    m->forward_speed = GameInfo[arg0 + 0x5C] != 0 ? BonkSteerB[BonkSkill] : 0x50;
                } else {
                    m->forward_speed = GameInfo[arg0 + 0x5C] != 0 ? BonkSteerA[BonkSkill] : 0x2D;
                }
            }
        } else {
            m->x_max = 0;
            m->z_max = 0;
        }
    }
}

void SetBonkHeroIdle(s32 idx) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + idx;

    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 1;
    SetHeroSeq(idx, 0x33);
    if (h->hero_ctrl & 4) {
        EXTengine *e = h->hero_eng;
        e->unk2C.z = 0x1000;
        e->unk2C.y = 0x1000;
        e->unk2C.x = 0x1000;
        h->hero_ctrl &= 0xFFFB;
    }
}

void BonkHeroIdle(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    if (func_8008ABF8() == 0 && func_80086FCC(arg0) == 0 && BonkHeroTestRun(arg0) == 0) {
        func_8008502C(arg0);
        if (VelocityCalc < 4) {
            if (VelocityCalc >= 2) {
                HeroHooks.HeroKickCollision(arg0);
            }
        }
        base = Heroes;
        h = base + arg0;
        if (h->hero_status & 0x100) {
            if (h->hero_move != NULL) {
                h->hero_move->forward_angle = 0;
                h->hero_move->facing_angle = 0;
            }
            CameraMode = 1;
        }
        func_800871E4(arg0);
    }
}

void BonkHeroRun(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    s32 id;
    s32 a0;

    if (func_8008ABF8() == 0 && func_80086FCC(arg0) == 0) {
        if (BonkStopInline(arg0) == 0) {
            base = Heroes;
            h = base + arg0;
            h->hero_move->velocity_enable = 1;
            HeroHooks.HeroKickCollision(arg0);
            id = func_8008502C(arg0);
            switch (id) {
            case 8:
                a0 = 0x41E;
                if (h->hero_pu != 0x15) {
                    switch (h->hero_name) {
                    case 1: a0 = 0xA; break;
                    case 2: a0 = 0x3C; break;
                    case 3: a0 = 0x6E; break;
                    case 4: a0 = 0xA0; break;
                    case 7: a0 = 0x104; break;
                    case 8: a0 = 0xD2; break;
                    case 5: a0 = 0x136; break;
                    case 6: a0 = 0x168; break;
                    default: goto done;
                    }
                    PlaySfx(a0, NULL, (SLONG_XYZ *)((u8 *)Heroes[arg0].hero_eng + 0x10), 0x1000, 0x1E00, 0);
                } else {
                    PlaySfx(a0, NULL, (SLONG_XYZ *)((u8 *)h->hero_eng + 0x10), 0x1000, 0x1E00, 0);
                }
                break;
            case 9:
                if (h->hero_pu != 0x15) {
                    switch (h->hero_name) {
                    case 1: a0 = 0xB; break;
                    case 2: a0 = 0x3D; break;
                    case 3: a0 = 0x6F; break;
                    case 4: a0 = 0xA1; break;
                    case 7: a0 = 0x105; break;
                    case 8: a0 = 0xD3; break;
                    case 5: a0 = 0x137; break;
                    case 6: a0 = 0x169; break;
                    default: goto done;
                    }
                    PlaySfx(a0, NULL, (SLONG_XYZ *)((u8 *)Heroes[arg0].hero_eng + 0x10), 0x1000, 0x1E00, 0);
                }
                break;
            }
done:
            func_800871E4(arg0);
        }
    }
}

void BonkHeroBackswing(s32 arg0) {
    BonkTarget *t;
    HeroInfo *base;
    HeroInfo *h;

    if (BonkHeroTestIdle(arg0) == 0 && BonkHeroTestLose(arg0) == 0 && BonkHeroTestWin(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        h->hero_frozen = 10;
        if (BonkAI_FindClosest(arg0, &t, 0x306) != 0) {
            BonkPos *p;
            BonkSlots[arg0].f0 = (s32)t;
            p = t->unk4->unk54;
            h->hero_move->forward_angle =
                get_angle(h->hero_eng->unk10.x - p->x, h->hero_eng->unk10.z - p->z);
        }
        if (func_8008502C(arg0) == 8) {
            if (((BonkPadButtons *)Pads)[arg0].unk50 & 0x8000) {
                SetHeroAction(arg0, 0x44);
            } else {
                SetHeroAction(arg0, 0x45);
            }
        }
    }
}

void BonkHeroExtBackswing(s32 arg0) {
    BonkSlot *s;
    BonkSlot *sb;

    if (BonkHeroTestIdle(arg0) == 0 && BonkHeroTestLose(arg0) == 0 && BonkHeroTestWin(arg0) == 0) {
        HeroInfo *base = Heroes;
        (base + arg0)->hero_frozen = 10;
        sb = BonkSlots;
        s = sb + arg0;
        if (s->f4 == 0) {
            if (func_8008502C(arg0) == 8) {
                s->f4 = 1;
            }
            if (s->f4 == 0) {
                return;
            }
        }
        if (!(((BonkPadButtons *)Pads)[arg0].unk50 & 0x8000)) {
            SetHeroAction(arg0, 0x45);
            BonkSlots[arg0].f4 = 0;
        }
    }
}

void BonkHeroSwing(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    s32 act;
    BonkTgt2 *t;

    if (BonkHeroTestIdle(arg0) == 0 && BonkHeroTestLose(arg0) == 0 && BonkHeroTestWin(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        h->hero_frozen = 10;
        act = func_8008502C(arg0);
        if (act == 0xC) {
            PlaySfx(BonkSwingSfx[arg0], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (act == 8) {
            if (h->hero_last_mode == 0x44 || BonkSlots[arg0].f8 == 1 ||
                (GameInfo[arg0 + 0x5C] != 0 && (((D5A614View *)GameInfo)->flags & 0x80))) {
                BonkSpawnShockwave(arg0);
            }
            t = (BonkTgt2 *)BonkSlots[arg0].f0;
            if (t != NULL) {
                switch (t->kind) {
                case 0x100:
                    if (t->obj != NULL && (u16)t->obj->mode >= 0xB) {
                        BonkHitTarget(arg0, 1, t);
                    }
                    break;
                case 0x200:
                    if (t->obj != NULL && (u16)t->obj->mode >= 0xB) {
                        BonkHitObject(arg0, 1, (BonkObj *)t);
                    }
                    break;
                }
            }
            PlaySfx(BonkSwingHitSfx[arg0], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            SetHeroAction(arg0, 0x46);
        }
    }
}

s32 BonkHeroKickCollision(s32 arg0) {
    TrigEntry *tbl;
    VECTOR pos;
    NAS *own;
    EXTengine *obj;
    s32 hit;
    s32 top;
    s32 push;
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
    HeroInfo *other;
    EXTengine *eng;
    EXTengine *e;
    FlagRecord *rec;
    FlagRecord *box;
    NAS *n;
    BonkHitFlags *t;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
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
                    rotscale_update(&eng->unk1C);
                }
                gfx_load_xform(eng->sub, eng->unk3C, &eng->unk10);
                lo2 = eng->unk10.y + box->unk2;
                hi2 = eng->unk10.y + box->height;
                if (lo2 < hi2) {
                    tmp = hi2;
                    hi2 = lo2;
                    lo2 = tmp;
                }
                rad2 = box->radius;
                gte_rotate_point((SVECTOR *)box, &pos);
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
            other = Heroes;
            i = 0;
            self = &Heroes[arg0];
            do {
                tbl = rcossin_tbl;
                if (arg0 != i && !(other[i].hero_status & 0xF800)) {
                    if (self->hero_move->forward_speed != 0) {
                        push = 1;
                    } else {
                        push = other[i].hero_move->forward_speed == 0;
                    }
                    if (VelocityCalc < 4) {
                        if (VelocityCalc >= 2) {
                            push = 1;
                        }
                    }
                    e = other[i].hero_eng;
                    if (e != NULL) {
                        rec = func_80025A48(e, 0x40);
                        if (rec != NULL) {
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
                                            eng->unk10.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->unk10.z -= (d * tbl[ang].unk2) >> 12;
                                            hit = 1;
                                            if (self->hero_pu != 0) {
                                                (*(void (**)(s32, s32))&HeroHooks.pad60)(arg0, i);
                                            }
                                            if (other[i].hero_pu != 0) {
                                                (*(void (**)(s32, s32))&HeroHooks.pad60)(i, arg0);
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
                                            eng->unk10.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->unk10.z -= (d * tbl[ang].unk2) >> 12;
                                        } else {
                                            e->unk10.x += (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            e->unk10.z += (d * tbl[ang].unk2) >> 12;
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
                                            eng->unk10.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->unk10.z -= (d * tbl[ang].unk2) >> 12;
                                        } else {
                                            e->unk10.x += (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            e->unk10.z += (d * tbl[ang].unk2) >> 12;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                i++;
            } while (i < 4);
            own = NULL;
            n = HeadOfSolids;
            if (Heroes[arg0].hero_object != NULL) {
                own = ((NAS **)Heroes[arg0].hero_object)[-1];
            }
            rad = 0x78;
            push = Heroes[arg0].hero_move->forward_speed != 0;
            if (VelocityCalc < 4) {
                if (VelocityCalc >= 2) {
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
                                eng->unk10.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                eng->unk10.z -= (d * rcossin_tbl[ang & 0xFFF].unk2) >> 12;
                            }
                            t = FindExtension(n, 0x1000);
                            if (t != NULL && (t->unk1C & 8)) {
                                t->unk16 = arg0;
                                t->unk1E |= 8;
                            }
                        }
                    }
                }
            }
        }
    }
    return hit;
}

void BonkHeroLoser(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    if (h->hero_eng->unk2C.x != 0) {
        h->hero_eng->unk2C.x -= 0x80;
        h->hero_eng->unk2C.y -= 0x80;
        h->hero_eng->unk2C.z -= 0x80;
        h->hero_eng->unk5E = 1;
        if (h->hero_eng->unk2C.x <= 0) {
            BonkSpawnPuff(&h->hero_eng->unk10, 1, MalletObjectsModel);
            if (Heroes[arg0].hero_shadow != NULL) {
                Heroes[arg0].hero_shadow->flags &= 0xFFFF7FFF;
            }
        }
    }
    Heroes[arg0].hero_frozen = 2;
    func_8008502C(arg0);
}

s32 BonkHeroTestIdle(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x100) {
        SetHeroAction(arg0, 0x40);
        return 1;
    }
    return 0;
}

void SetBonkHeroLoser(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *p = h->hero_powerup;
    subengine *s = NULL;

    if (p != NULL) {
        s = p->sub;
    }
    if (s != NULL) {
        p->flags &= 0xFFFF7FFF;
    }
    BonkSlots[arg0].f8 = 3;
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
    SetHeroSeq(arg0, 0x33);
    func_80026980(h->hero);
}

s32 BonkHeroTestLose(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x400) {
        SetHeroAction(arg0, 0x48);
        return 1;
    }
    return 0;
}

void BonkHeroWinner(s32 idx) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + idx;
    D5A614View *g;

    h->hero_frozen = 2;
    if (h->hero_move != NULL) {
        h->hero_move->forward_angle = 0;
        h->hero_move->facing_angle = 0;
    }
    func_8008502C(idx);
    if (func_8008A200(idx) != 0) {
        g = (D5A614View *)GameInfo;
        if (g->unk4 & 0x800) {
            g->unk30 = g->unk30 | 0x201;
        } else {
            g->unk30 = g->unk30 | 1;
        }
    }
}

void SetBonkHeroWinner(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *p = h->hero_powerup;
    subengine *s = NULL;

    if (p != NULL) {
        s = p->sub;
    }
    if (s != NULL) {
        p->flags &= 0xFFFF7FFF;
    }
    BonkSlots[arg0].f8 = 3;
    func_80089FA4(arg0);
    SetHeroSeq(arg0, 0x39);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
    CameraMode = 1;
}

s32 BonkHeroTestWin(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x200) {
        SetHeroAction(arg0, 0x47);
        return 1;
    }
    return 0;
}

void BonkHeroSwingRecoil(s32 arg0) {
    HeroInfo *hero;

    if (BonkHeroTestIdle(arg0) == 0 && BonkHeroTestLose(arg0) == 0 && BonkHeroTestWin(arg0) == 0) {
        HeroInfo *base = Heroes;
        hero = base + arg0;
        hero->hero_frozen = 10;
        if (func_8008502C(arg0) == 9) {
            SetHeroAction(arg0, 0x40);
            hero->hero_frozen = 0;
        }
    }
}

void SetBonkHeroSwingRecoil(s32 arg0) {
    Heroes[arg0].hero_frozen = 0xA;
}

void SetBonkHeroSwing(s32 arg0) {
    Heroes[arg0].hero_frozen = 10;
    SetHeroSeq(arg0, 0x37);
}

void SetBonkHeroExtBackswing(s32 arg0) {
    SetHeroSeq(arg0, 0x36);
    Heroes[arg0].hero_frozen = 10;
    BonkSlots[arg0].f4 = 0;
}

void SetBonkHeroBackswing(s32 arg0) {
    HeroInfo *hero;
    HeroInfo *base = Heroes;

    BonkSlots[arg0].f0 = 0;
    hero = base + arg0;
    hero->hero_frozen = 10;
    PlaySfx(BonkBackswingSfx[arg0], NULL, &hero->hero_eng->unk10, 0x1000, 0x1E00, 0);
    SetHeroSeq(arg0, 0x35);
}

void BonkHeroHit(s32 arg0) {
    if (Heroes[arg0].hero_move->reactive_speed != 0) {
        HeroHooks.HeroKickCollision();
    }
    func_8008502C(arg0);
    func_800871E4(arg0);
}

void SetBonkHeroHit(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->velocity_enable = 0;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq(arg0, 0x38);
    h->hero_hit = 0x1E;
}

void SetBonkHeroRun(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x34);
}

s32 BonkHeroTestRun(s32 arg0) {
    if (HeroHooks.TestRun(arg0) == 0) {
        if (Heroes[arg0].hero_move->forward_speed == 0) {
            return 0;
        }
        SetHeroAction(arg0, 0x41);
    }
    return 1;
}

s32 BonkHeroToIdle(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction(arg0, 0x40);
        return 1;
    }
    return 0;
}

s32 BonkHeroTestSwing(s32 arg0) {
    if ((((BonkPadButtons *)Pads)[arg0].unk50 & 0x8000) && Heroes[arg0].hero_mode != 0x43) {
        SetHeroAction(arg0, 0x43);
        return 1;
    }
    return 0;
}

void BonkHeroPostMove(void) {
}

void BonkHidePowerup(s32 arg0) {
    EXTengine *p = Heroes[arg0].hero_powerup;
    subengine *s = NULL;

    if (p != NULL) {
        s = p->sub;
    }
    if (s != NULL) {
        p->flags &= 0xFFFF7FFF;
    }
    BonkSlots[arg0].f8 = 3;
}

s32 BonkKeepInBoundsB(EXTengine *eng) {
    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;
    s32 limit;
    s32 ret = 0;
    s32 ang;

    if (eng->collision_box.flags & 0x40) {
        limit = 0x880 - eng->collision_box.radius;
    } else {
        limit = 0x880;
    }
    if (x * x + z * z >= limit * limit) {
        ang = get_angle(x, z);
        eng->unk10.x = limit * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
        eng->unk10.z = limit * rcossin_tbl[ang & 0xFFF].unk2 >> 12;
        ret = 1;
    }
    return ret;
}

s32 BONK_KeepInBounds(EXTengine *eng) {
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
        eng->unk10.x = limit * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
        eng->unk10.z = limit * rcossin_tbl[ang & 0xFFF].unk2 >> 12;
        ret = 1;
    }
    return ret;
}

void BonkHeroSnapShadow(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *shadow;

    h = base + player;

    ((void (*)(EXTengine *))BONK_KeepInBounds)(h->hero_eng);
    shadow = h->hero_shadow;
    if (shadow != NULL) {
        shadow->unk10 = h->hero_eng->unk10;
    }
}

void BonkHeroBounce(s32 arg0) {
    EXTmotion *m;

    SetHeroAction(arg0, 0x40);
    BonkSlots[arg0].f8 = 3;
    BonkSlots[arg0].f0 = 0;
    m = AddMotion(Heroes[arg0].hero);
    if (m != NULL) {
        m->Ymax = 0xA0;
        m->Ydec = 0xE;
        m->Yacc = 0;
        m->Ydir = 0xFF;
        m->Yvel = 0;
    }
    BonkHeroSnapShadow(arg0);
    BonkAI_Reset(arg0);
}

void BonkBounceAll(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        BonkHeroBounce(var_s0);
        var_s0 += 1;
    } while (var_s0 < 4);
}
