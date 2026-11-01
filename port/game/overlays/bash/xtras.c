#include "common.h"
#include "bash/engine.h"
#include "bash/bash_ovl.h"
#include "bash/bash_xtras.h"
#include "bash/bash.h"
#include "bash/xtras.h"

/* data, in address order */
u16 BashTickingCrateModels[4] = { 0x2010, 0x2011, 0x2012, 0 };
u16 BashDrainsEdgeCrates[18] = {
    0x19, 0x1B, 0x1C, 0x19, 0x1C, 0x1B, 0x1C, 0x19, 0x1B, 0x1C, 0x1B, 0x19, 0x1B, 0x1C, 0x19,
    0x1B, 0x19, 0x1C,
};
u16 BashPickupModels[4] = { 0x200D, 0x200A, 0x2011, 0x2009 };
EXTengine BashSfxEngine = {0};
u16 BashDrainsEdgePos = 0;
u16 BashPowerupTimer = 0;
u8 *BashCrateList = 0;

void CrateAimAtGrid(EXTengine *a, EXTmotion *b) {
    s32 z;
    s32 x;
    s32 sum = 0;
    s32 v = b->Xvel;
    s32 d = b->Xacc - b->Xdec;
    s32 mx, mz;

    if (v != 0) {
        do {
            v += d;
            if (v < 0) {
                v = 0;
            }
            sum += v;
        } while (v != 0);
    }
    x = a->XYZ.x;
    mx = sum * sin_cos_table[(a->xform.rot.vy - 0x400) & 0xFFF].unk2;
    mz = sum * sin_cos_table[(u16)a->xform.rot.vy & 0xFFF].unk2;
    z = a->XYZ.z;
    x -= mx >> 12;
    z -= mz >> 12;
    x &= ~0xFF;
    x += 0x80;
    z &= ~0xFF;
    z += 0x80;
    b->Xa = x;
    b->Za = z;
    b->flags = *(u8 *)FILE_AT(DirPadBits, ((u16)a->xform.rot.vy >> 7) & 0x1C);
    x -= a->XYZ.x;
    z -= a->XYZ.z;
    sum = int_sqrt(x * x + z * z) >> 6;
    if (sum > 0) {
        v -= d;
        do {
            sum -= v;
            v -= d;
        } while (sum > 0);
        v += d;
    }
    v -= d >> 1;
    b->Xvel = v;
}

s32 BashGridKey(EXTengine *p) {
    s32 a, b, c, d;
    b = (p->XYZ.x >> 8) + 8;
    a = 7 - (p->XYZ.z >> 8);
    c = (s8)(p->xform.rot.vy >> 8);
    d = c << 16;
    if (!(((u16)p->xform.rot.vy) >> 16)) {
        return c;
    }
    return a + b + d;
}

void BashCountObjects(void) {
    s32 i;
    s32 x, z;
    u16 t;
    u8 c;
    u16 *p;
    s32 j;
    NAS *n;
    EXTengine *e;
    T_BB7E8Fux *f;

    i = 12;
    p = CrateCounts;
    p += 12;
    do {
        *p = 0;
        i--;
        p--;
    } while (i >= 0);
    for (j = 12; j >= 0; j--) {
        PickupCounts[j] = 0;
    }
    n = HeadOfObjects;
    for (;;) {
        if (n == NULL) return;
        e = n->Engine;
        if (e != NULL) {
        f = (T_BB7E8Fux *)n->FUX;
        if (f != NULL) {
            t = f->type;
            x = (e->XYZ.x >> 8) + 10;
            z = 9 - (e->XYZ.z >> 8);
            if (x >= 20) x = 19;
            if (x < 0) x = 0;
            if (z >= 20) z = 19;
            if (z < 0) z = 0;
            ArenaGrid[z * 20 + x].flags = t;
            if ((u16)(f->type - 1) < 12) {
                CrateCounts[0]++;
                CrateCounts[f->type]++;
            } else if ((u16)(f->type - 0x11) < 12) {
                PickupCounts[0]++;
                PickupCounts[f->type - 0x10]++;
                if ((u16)(f->type - 0x12) < 4) {
                    CrateCounts[1]++;
                }
            }
        } else {
            c = 0xFF;
            x = (e->XYZ.x >> 8) + 10;
            z = 9 - (e->XYZ.z >> 8);
            if (x >= 20) x = 19;
            if (x < 0) x = 0;
            if (z >= 20) z = 19;
            if (z < 0) z = 0;
            ArenaGrid[z * 20 + x].flags = c;
        }
        }
        n = n->Object.Child;
    }
}

u8 *CreateCrate(s32 a, s32 b, s32 c, s32 d, s32 e) {
    NAS *nas;
    EXTobject *obj;
    EXTengine *eng;
    s32 x, z;
    u16 k;
    s32 t;

    nas = CreateNAS(a, b, c, 0x202, 0x11);
    k = d;
    if (nas != NULL) {
        eng = nas->Engine;
        if (eng != NULL) {
            obj = (EXTobject *)AddObject(nas);
            if (obj != NULL && AddMotion(nas) != NULL) {
                LinkObject(nas);
                AddSolid(nas, 0x300);
                nas->FUX = (EXT *)obj;
                eng->sub->u6C.dbase = &HeroModelBank[DB_CrateObjects];
                eng->xform.order = 1;
                obj->whatami = d;
                CrateCounts[obj->whatami]++;
                if ((s16)e >= 0) {
                    obj->status = e;
                } else {
                    obj->status = 6;
                }
                switch (k) {
                case 3:
                    eng->sub->entity = 0x2008;
                    break;
                case 1:
                    eng->sub->entity = 0x2014;
                    break;
                case 2:
                    eng->sub->entity = 0x2014;
                    break;
                case 5:
                    eng->sub->entity = 0x2009;
                    obj->countdown = 0x5A;
                    obj->can_do |= 0x100;
                    break;
                case 6:
                    eng->sub->entity = 0x2001;
                    obj->can_do |= 0x100;
                    break;
                default:
                    eng->sub->entity = 0x2013;
                    break;
                }
                switch ((u16)obj->status) {
                case 3:
                    eng->XYZ.y = World_floor_level;
                    break;
                case 7:
                    eng->xform.scale.vx = 0x40;
                    eng->xform.scale.vy = 0x40;
                    eng->xform.scale.vz = 0x40;
                    eng->xform.order = 1;
                    eng->xform.hasScale = 1;
                    eng->xform.dirty = 1;
                    eng->flags = (eng->flags & ~0x8000) | 1;
                    eng->XYZ.y = World_floor_level;
                    PlaySfx(0x3FA, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                    break;
                case 6:
                    eng->xform.order = 1;
                    eng->xform.hasScale = 1;
                    eng->xform.dirty = 1;
                    eng->xform.scale.vy = 0;
                    eng->flags &= ~0x8000;
                    eng->XYZ.y = World_floor_level;
                    break;
                }
                t = k;
                x = (eng->XYZ.x >> 8) + 10;
                z = 9 - (eng->XYZ.z >> 8);
                if (x >= 20) x = 19;
                if (x < 0) x = 0;
                if (z >= 20) z = 19;
                if (z < 0) z = 0;
                ArenaGrid[z * 20 + x].flags = t;
            }
        }
        if (nas->Failure != 0) {
            KillNAS(nas);
            return NULL;
        }
    }
    return (u8 *)nas;
}

void InitCrate(NAS *arg0) {
    EXTengine *eng;
    EXTobject *obj;
    NAS *nas;
    EXTengine *other;
    s32 again;
    s32 x, z;

    eng = arg0->Engine;
    if (eng != NULL) {
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            arg0->FUX = (EXT *)obj;
            do {
                again = 0;
                nas = HeadOfSolids;
                eng->XYZ.x = (eng->XYZ.x & ~0xFF) + 0x80;
                eng->XYZ.z = (eng->XYZ.z & ~0xFF) + 0x80;
                while (nas != NULL) {
                    if (nas->Type == CrateAstType) {
                        other = nas->Engine;
                        if (other != NULL && other->XYZ.x == eng->XYZ.x && other->XYZ.z == eng->XYZ.z) {
                            eng->XYZ.x = Random(0xC00) - 0x600;
                            eng->XYZ.z = Random(0xC00) - 0x600;
                            nas = NULL;
                            again = 1;
                        }
                    }
                    if (nas != NULL) {
                        nas = nas->Solid.Child;
                    }
                }
            } while (again);
            eng->XYZ.y = World_floor_level;
            eng->sub->u6C.dbase = &HeroModelBank[DB_CrateObjects];
            eng->xform.order = 1;
            eng->xform.hasScale = 1;
            obj->status = 3;
            if (arg0->trigger != NULL) {
                obj->whatami = arg0->trigger->InfoStruct->val1;
            }
            CrateCounts[obj->whatami]++;
            switch (obj->whatami) {
            case 3:
                eng->sub->entity = 0x2008;
                break;
            case 1:
                eng->sub->entity = 0x2014;
                break;
            case 2:
                eng->sub->entity = 0x2014;
                break;
            case 5:
                eng->sub->entity = 0x2009;
                obj->countdown = 0x5A;
                obj->can_do |= 0x100;
                break;
            case 6:
                eng->sub->entity = 0x2001;
                obj->can_do |= 0x100;
                break;
            default:
                eng->sub->entity = 0x2013;
                break;
            }
            x = (eng->XYZ.x >> 8) + 10;
            z = 9 - (eng->XYZ.z >> 8);
            if (x >= 20) {
                x = 19;
            }
            if (x < 0) {
                x = 0;
            }
            if (z >= 20) {
                z = 19;
            }
            if (z < 0) {
                z = 0;
            }
            ArenaGrid[z * 20 + x].flags = obj->whatami;
            AddMotion(arg0);
            LinkObject(arg0);
            AddSolid(arg0, 0x300);
        }
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void PlotCrate(NAS *arg0) {
    VECTOR a;
    VECTOR b;
    EXTengine *eng;
    EXTobject *fux;
    EXTmotion *ext;
    EXTengine *he;
    EXTengine *ho;
    EXTengine *hd;
    HeroInfo *hb;
    NAS *n;
    s32 type;
    s32 dx, dy, dz;
    s32 y;
    s32 v;
    s32 who;
    s32 ang;
    s32 done;
    u16 t;

    eng = arg0->Engine;
    if (eng == NULL) {
        return;
    }
    fux = (EXTobject *)arg0->FUX;
    if (fux == NULL) {
        return;
    }
    type = fux->whatami;
    ext = FindExtension(arg0, 0x800);
    if (ext == NULL) {
        return;
    }
    switch (fux->status) {
    case 0:
        fux->status = 1;
        fux->can_do &= 0xFFD4;
        switch (type) {
        case 6:
            fux->can_do |= 0xA;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 7:
            fux->can_do |= 3;
            break;
        }
        eng->xform.dirty = 1;
        eng->xform.rot.vy = 0;
        BC038_SNAP(eng);
        fux->player = -1;
        FetchDefaultColBox(eng, 0x50);
        break;
    case 2:
    case 0x13:
        if (fux->player != -1 && fux->player < 5) {
            he = Heroes[fux->player].hero_eng;
            if (he != NULL) {
                if (sub_find_point(he, he->sub, 0x10, &a) != NULL) {
                    if (sub_find_point(he, he->sub, 0x20, &b) != NULL) {
                        dx = a.vx - b.vx;
                        dy = b.vy - a.vy;
                        dz = a.vz - b.vz;
                        eng->xform.rot.vy = get_angle(dx, dz) - 0x200;
                        ang = 0x271 - get_angle(int_sqrt(dx * dx + dz * dz) >> 6, dy);
                        eng->xform.rot.vx = ang;
                        eng->XYZ.y = b.vy - (sin_cos_table[(ang - 0x400) & 0xFFF].unk2 * 128 >> 12);
                        dx = a.vx - dx / 2;
                        dz = a.vz - dz / 2;
                        eng->xform.rot.vz = 0;
                        eng->XYZ.x = dx;
                        eng->XYZ.z = dz;
                    }
                } else if (sub_find_point(he, he->sub, 1, &b) != NULL) {
                    eng->XYZ = *(SLONG_XYZ *)&b;
                    eng->xform.rot.vy = he->xform.rot.vy;
                } else {
                    fux->status = 4;
                }
            }
        if (fux->will_do & 2) {
            /* the original cleared player first, so the hero holding it kept a pointer to a dead crate, and wrote to it when he was hit */
            BC038Release(fux);
            fux->player = -1;
            switch (type) {
            case 3:
            case 4:
                BC038Release(fux);
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                KillTriggerNas(arg0);
                return;
            case 5:
            case 6:
            case 7:
                BC038Release(fux);
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                KillTriggerNas(arg0);
                return;
            case 1:
                BC038Release(fux);
                CrateBreak(eng, fux);
                KillTriggerNas(arg0);
                return;
            case 2:
                BC038Release(fux);
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                fux->status = 0xE;
                return;
            default:
                BC038_STOP(ext);
                break;
            }
        }
        if ((fux->can_do & 0x20) && CrateHitSolids(eng, fux->player) != NULL) {
            switch (type) {
            case 2:
                BC038Release(fux);
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                fux->status = 0xE;
                return;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                BC038Release(fux);
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                KillTriggerNas(arg0);
                return;
            case 1:
                BC038Release(fux);
                CrateBreak(eng, fux);
                KillTriggerNas(arg0);
                return;
            default:
                ext->Xdec = 0;
                ext->Xacc = 0;
                ext->Xmax = 0;
                ext->Xvel = 0;
                break;
            }
        }
        if (fux->will_do & 4) {
            hb = Heroes;
            fux->can_do &= 0xFFDB;
            fux->will_do &= 0xFFFB;
            eng->xform.rot.vy = hb[fux->player].hero_move->facing_angle;
            eng->xform.rot.vx = 0;
            eng->xform.rot.vz = 0;
            ext->Xvel = BC038_STATB(hb, hit_shapes, fux->player)->speed;
            ext->Xmax = BC038_STATB(hb, hit_shapes, fux->player)->max;
            ext->Xacc = BC038_STATB(hb, hit_shapes, fux->player)->accel;
            ext->Xdec = BC038_STATB(hb, hit_shapes, fux->player)->decel;
            ext->Yvel = BC038_STATB(hb, hit_shapes, fux->player)->yvel;
            ext->Ymax = BC038_STATB(hb, hit_shapes, fux->player)->ymax;
            ext->Yacc = BC038_STATB(hb, hit_shapes, fux->player)->yacc;
            ext->Ydec = BC038_STATB(hb, hit_shapes, fux->player)->ydec;
            ext->Ydir = BC038_STATB(hb, hit_shapes, fux->player)->ydir;
            fux->status = 8;
        }
        } else {
            fux->status = 4;
        }
        eng->xform.dirty = 1;
        break;
    case 6:
        eng->xform.rot.vy = 0;
        v = eng->xform.scale.vy;
        v += 0x200;
        BC038_SNAP(eng);
        if (v >= 0x1000) {
            v = 0x1000;
            fux->status = 0;
        }
        eng->xform.scale.vy = v;
        eng->xform.dirty = 1;
        eng->flags |= 0x8000;
        break;
    case 0x10:
        eng->xform.rot.vy = 0;
        v = eng->xform.scale.vy;
        v -= 0x200;
        BC038_SNAP(eng);
        if (v < 0x41) {
            KillTriggerNas(arg0);
            return;
        }
        eng->xform.scale.vy = v;
        eng->xform.dirty = 1;
        break;
    case 7:
        v = eng->xform.scale.vx;
        v += 0x100;
        if (v >= 0x1000) {
            v = 0x1000;
            fux->status = 0;
            eng->flags &= ~0x21;
        }
        eng->xform.scale.vx = v;
        eng->xform.scale.vy = v;
        eng->xform.scale.vz = v;
        eng->sub->scale = v;
        eng->xform.dirty = 1;
        eng->flags |= 0x8000;
        break;
    case 4:
        ext->Yvel = 0x50;
        ext->Ymax = 0x50;
        ext->Ydec = 6;
        ext->Ydir = 0xFF;
        ext->Yacc = 0;
        eng->xform.rot.vx = 0;
        eng->xform.rot.vz = 0;
        fux->status = 8;
        break;
    case 5:
        BC038_SNAP(eng);
        ext->Ymax = 0x50;
        ext->Ydec = 6;
        ext->Ydir = 0xFF;
        ext->Yvel = 0;
        ext->Yacc = 0;
        eng->xform.rot.vx = 0;
        eng->xform.rot.vz = 0;
        fux->status = 8;
        break;
    case 8:
        if (eng->XYZ.y > World_floor_level) {
            eng->flags |= 0x02000000;
            eng->sub->otz = 0x190;
            eng->sub->flags |= 0x02000000;
            eng->sub->otz = 0x190;
        }
        if (GravitateNas(arg0) != 0) {
            eng->XYZ.y += 0x20;
            CrateCollide(eng, fux, fux->player, 0);
            eng->XYZ.y -= 0x20;
            switch (type) {
            case 3:
            case 4:
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                KillTriggerNas(arg0);
                return;
            case 5:
            case 6:
            case 7:
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                KillTriggerNas(arg0);
                return;
            case 1:
                CrateBreak(eng, fux);
                KillTriggerNas(arg0);
                return;
            case 2:
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                if (HeroHooks.KeepInBounds(eng) != 0) {
                    KillTriggerNas(arg0);
                    return;
                }
                fux->status = 0xE;
                return;
            case 18:
            case 19:
            case 20:
            case 21:
                PlaySfx(0x420, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
            default:
                fux->status = 0;
                break;
            }
        }
        if (fux->will_do & 2) {
            fux->player = 6;
            PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
            KillTriggerNas(arg0);
            return;
        }
        if (CrateCollide(eng, 0, fux->player, 1) != 0) {
            switch (type) {
            case 3:
            case 4:
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                KillTriggerNas(arg0);
                return;
            case 5:
            case 6:
            case 7:
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                KillTriggerNas(arg0);
                return;
            case 1:
                CrateBreak(eng, fux);
                KillTriggerNas(arg0);
                return;
            case 2:
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                fux->status = 0xE;
                return;
            default:
                BC038_STOP(ext);
                break;
            }
        }
        break;
    case 9:
        switch (type) {
        case 1:
            BC038Release(fux);
            PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
            KillTriggerNas(arg0);
            return;
        case 2:
            BC038Release(fux);
            if (OVLG->levelFlags & 0x80) {
                who = fux->player;
                if (who == 0 || (OVLG->teams != 0 && who == 1)) {
                    Heroes[who].hero_health = 2;
                    fux->whatami = 6;
                    PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 0);
                    return;
                }
            }
            PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
            fux->status = 0xC;
            return;
        case 6:
            BC038Release(fux);
            PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 0);
            KillTriggerNas(arg0);
            return;
        case 5:
            fux->countdown = 0x5A;
            fux->whatami = 7;
            PlaySfx(0x3F0, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
            v = fux->player;
            fux->status = 0xB;
            fux->timer = 2;
            if ((u32)v >= 5) {
                v = 0;
            }
            ext->Xvel = BC038_STAT(hit_shapes2, v)->speed;
            ext->Xmax = BC038_STAT(hit_shapes2, v)->max;
            ext->Xacc = BC038_STAT(hit_shapes2, v)->accel;
            ext->Xdec = BC038_STAT(hit_shapes2, v)->decel;
            BashGridKey((EXTengine *)eng);
            CrateAimAtGrid((EXTengine *)eng, (EXTmotion *)ext);
            break;
        case 3:
        case 4:
        case 7:
            PlaySfx(0x3F9, &eng->sfx, &eng->XYZ, 0x1000, 0x1E00, 0);
            v = fux->player;
            fux->status = 0xB;
            fux->timer = 2;
            if ((u32)v >= 5) {
                v = 0;
            }
            ext->Xvel = BC038_STAT(hit_shapes2, v)->speed;
            ext->Xmax = BC038_STAT(hit_shapes2, v)->max;
            ext->Xacc = BC038_STAT(hit_shapes2, v)->accel;
            ext->Xdec = BC038_STAT(hit_shapes2, v)->decel;
            BashGridKey((EXTengine *)eng);
            CrateAimAtGrid((EXTengine *)eng, (EXTmotion *)ext);
            break;
        default:
            fux->status = 0;
            break;
        }
        break;
    case 3:
        fux->status = 0;
        break;
    case 0xB:
        ext->Xvel += ext->Xacc - ext->Xdec;
        if (ext->Xvel < 0) {
            ext->Xvel = 0;
        }
        if (ext->Xvel > ext->Xmax) {
            ext->Xvel = ext->Xmax;
        }
        eng->XYZ.x -= (ext->Xvel * sin_cos_table[(eng->xform.rot.vy - 0x400) & 0xFFF].unk2) >> 12;
        eng->XYZ.z -= (ext->Xvel * sin_cos_table[(u16)eng->xform.rot.vy & 0xFFF].unk2) >> 12;
        switch (ext->flags & 0xF0) {
        case 0x10:
        case 0x30:
        case 0x90:
            if (ext->Za >= eng->XYZ.z) {
                ext->Xvel = 0;
            }
            break;
        case 0x80:
            if (eng->XYZ.x >= ext->Xa) {
                ext->Xvel = 0;
            }
            break;
        case 0x20:
            if (ext->Xa >= eng->XYZ.x) {
                ext->Xvel = 0;
            }
            break;
        default:
            if (eng->XYZ.z >= ext->Za) {
                ext->Xvel = 0;
            }
            break;
        }
        if (ext->Xvel == 0) {
            eng->xform.dirty = 1;
            eng->xform.rot.vy = 0;
            BC038_SNAP(eng);
            fux->status = 0;
        }
        if (fux->timer != 0) {
            fux->timer--;
        }
        if (CrateCollide(eng, 0, fux->player, 1) != 0) {
            BC038_STOP(ext);
            if (fux->timer == 0) {
                switch (type) {
                case 3:
                case 4:
                    BC038Release(fux);
                    PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                    KillTriggerNas(arg0);
                    return;
                case 5:
                case 7:
                    BC038Release(fux);
                    PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                    KillTriggerNas(arg0);
                    return;
                }
            } else if (type < 5) {
                if (type >= 3) {
                    BC038Release(fux);
                    PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                    KillTriggerNas(arg0);
                    return;
                }
            }
        }
        if (HeroHooks.KeepInBounds(eng) != 0) {
            switch (type) {
            case 3:
            case 4:
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
                KillTriggerNas(arg0);
                return;
            case 6:
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 0);
                KillTriggerNas(arg0);
                return;
            case 5:
            case 7:
            default:
                eng->xform.dirty = 1;
                eng->xform.rot.vy = 0;
                BC038_SNAP(eng);
                fux->status = 0;
                BC038_STOP(ext);
                break;
            }
        }
        break;
    case 0xA:
        if (fux->countdown != 0) {
            fux->countdown--;
            break;
        }
        BC038Release(fux);
        PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 1);
        KillTriggerNas(arg0);
        return;
    case 0xC:
        fux->countdown = 0x78;
        fux->can_do &= 0xFFF4;
        BC038_SNAP(eng);
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        ext->Yvel = 0x60;
        ext->Ymax = 0xF0;
        ext->Yacc = 0;
        ext->Ydec = 4;
        ext->Ydir = 0xFF;
        eng->xform.rot.vx = 0;
        eng->xform.rot.vz = 0;
        switch (fux->contains) {
        case 0x19:
            eng->sub->entity = 0x200B;
            fux->status = 0xD;
            break;
        case 0x1B:
            eng->sub->entity = 0x200F;
            fux->status = 0xD;
            break;
        case 0x1C:
            eng->sub->entity = 0x200C;
            fux->status = 0xD;
            break;
        case 0x11:
            eng->sub->entity = Random(0xE) + 0x2018;
            eng->xform.order = 4;
            fux->status = 0xD;
            UnlinkObject(arg0);
            RemoveSolid(arg0);
            break;
        default:
            KillTriggerNas(arg0);
            return;
        }
        break;
    case 0xD:
        if (HeroHooks.KeepInBounds(eng) != 0) {
            KillTriggerNas(arg0);
            return;
        }
        done = 0;
        if (fux->player != -1 && ext->Ydir == 0) {
            fux->can_do |= 8;
            if (sub_find_point(Heroes[fux->player].hero_eng, Heroes[fux->player].hero_sub, 0x80, &a) != NULL) {
                dx = eng->XYZ.x - a.vx;
                dy = eng->XYZ.y - a.vy;
                dz = eng->XYZ.z - a.vz;
            } else {
                hd = Heroes[fux->player].hero_eng;
                dx = eng->XYZ.x - hd->XYZ.x;
                y = eng->XYZ.y + 0x180;
                dy = y - hd->XYZ.y;
                dz = eng->XYZ.z - hd->XYZ.z;
            }
            dx >>= 2;
            dy >>= 2;
            dz >>= 2;
            eng->XYZ.x -= dx;
            eng->XYZ.y -= dy;
            eng->XYZ.z -= dz;
            dx >>= 1;
            dz >>= 1;
            if (dx == 0 && dz == 0) {
                done = 1;
            }
            CrateHitHeroes(eng, (EXTobject *)fux, 8);
            if (fux->will_do & 8) {
                done = 1;
            }
        }
        if (GravitateNas(arg0) != 0) {
            done = 1;
        }
        if (done) {
            switch (fux->contains) {
            case 0x19:
            case 0x1B:
            case 0x1C:
                if (fux->player != -1 && fux->player < 5) {
                    BashGivePowerup(fux->player, fux->contains);
                    BashPowerupTimer = (Random(6) + 2) * 30;
                }
                KillTriggerNas(arg0);
                n = (NAS *)BashCrateList;
                while (n != NULL) {
                    fux = (EXTobject *)n->FUX;
                    if (fux != NULL) {
                        switch (fux->status) {
                        case 0:
                        case 1:
                            fux->status = 0x10;
                            break;
                        }
                        n = n->Group.Child;
                    } else {
                        KillTriggerNas(n);
                        n = (NAS *)BashCrateList;
                    }
                }
                return;
            case 0x11:
                if (fux->player != -1 && fux->player < 5) {
                    Heroes[fux->player].hero_health++;
                    if (Heroes[fux->player].hero_health >= 0x15) {
                        Heroes[fux->player].hero_health = 0x14;
                    }
                    PlaySfx(0x1C2, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                }
                KillTriggerNas(arg0);
                return;
            }
        }
        eng->xform.dirty = 1;
        eng->xform.rot.vy += 0x71;
        break;
    case 0xE:
        fux->countdown = 0x96;
        fux->can_do &= 0xFFF4;
        BC038_SNAP(eng);
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        ext->Yvel = 0x80;
        ext->Ymax = 0x50;
        ext->Ydec = 4;
        ext->Yacc = 0;
        ext->Ydir = 0xFF;
        eng->xform.rot.vx = 0;
        eng->xform.rot.vz = 0;
        switch (fux->contains) {
        case 0x19:
            eng->sub->entity = 0x200B;
            fux->status = 0xF;
            break;
        case 0x1B:
            eng->sub->entity = 0x200F;
            fux->status = 0xF;
            break;
        case 0x1C:
            eng->sub->entity = 0x200C;
            fux->status = 0xF;
            break;
        case 0x11:
            arg0->UserInfo.user2 = Random(0x1C);
            eng->sub->entity = (arg0->UserInfo.user2 >> 1) % 14 + 0x2018;
            eng->xform.dirty = 1;
            eng->xform.rot.vy = 0;
            fux->status = 0xF;
            break;
        default:
            KillTriggerNas(arg0);
            return;
        }
        break;
    case 0xF:
        if (HeroHooks.KeepInBounds(eng) != 0) {
            KillTriggerNas(arg0);
            return;
        }
        GravitateNas(arg0);
        if (fux->countdown == 0) {
            KillTriggerNas(arg0);
            return;
        }
        fux->countdown--;
        if (fux->contains == 0x11) {
            eng->xform.rot.vy = 0;
            arg0->UserInfo.user1++;
            eng->sub->entity = (arg0->UserInfo.user2 >> 1) % 14 + 0x2018;
        } else {
            eng->xform.rot.vy += 0x71;
        }
        if (ext->Ydir == 0) {
            fux->can_do |= 8;
        }
        CrateHitHeroes(eng, (EXTobject *)fux, 8);
        if ((fux->will_do & 8) && fux->player != -1 && fux->player < 5) {
            switch (fux->contains) {
            case 0x19:
            case 0x1B:
            case 0x1C:
                BashGivePowerup(fux->player, fux->contains);
                BashPowerupTimer = (Random(6) + 2) * 30;
                KillTriggerNas(arg0);
                n = (NAS *)BashCrateList;
                while (n != NULL) {
                    fux = (EXTobject *)n->FUX;
                    if (fux != NULL) {
                        switch (fux->status) {
                        case 0:
                        case 1:
                            fux->status = 0x10;
                            break;
                        }
                        n = n->Group.Child;
                    } else {
                        KillTriggerNas(n);
                        n = (NAS *)BashCrateList;
                    }
                }
                return;
            case 0x11:
                Heroes[fux->player].hero_health++;
                if (Heroes[fux->player].hero_health >= 0x15) {
                    Heroes[fux->player].hero_health = 0x14;
                }
                PlaySfx(0x1C2, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                KillTriggerNas(arg0);
                return;
            }
        }
        eng->xform.dirty = 1;
        break;
    case 0x63:
        if (GravitateNas(arg0) != 0) {
            KillTriggerNas(arg0);
            return;
        }
        v = eng->xform.scale.vx;
        v -= 0x100;
        if (v < 0x101) {
            KillTriggerNas(arg0);
            return;
        }
        eng->xform.scale.vx = v;
        eng->xform.scale.vy = v;
        eng->xform.scale.vz = v;
        eng->sub->scale = v;
        eng->xform.dirty = 1;
        break;
    default:
        if (fux->will_do & 0x100) {
            fux->status = 0xA;
            fux->countdown = 0xA;
            fux->will_do &= 0xFEFC;
            fux->can_do &= 0xFEFC;
        }
        if ((fux->will_do & 1) && !(fux->will_do & 2)) {
            if (fux->player != -1 && fux->player < 5) {
                fux->status = 2;
                fux->can_do |= 4;
                Heroes[fux->player].hero_object = (EXTobject *)fux;
                eng->xform.rot.vy = Heroes[fux->player].hero_move->facing_angle;
                fux->will_do &= 0xFFFC;
                fux->can_do = (fux->can_do & 0xFFFE) | 6;
                if (type == 5) {
                    fux->countdown = 0x5A;
                    if ((OVLG->levelFlags & 0x80) && OVLG->mode == 1) {
                        if (fux->player == 0 || (OVLG->teams != 0 && fux->player == OVLG->mode)) {
                            fux->countdown = 0x1E;
                            fux->can_do &= 0xFFFB;
                        }
                    }
                    fux->whatami = 7;
                    PlaySfx(0x3F0, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                }
            }
        }
        if (fux->will_do & 2) {
            switch (type) {
            case 6:
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 0);
                KillTriggerNas(arg0);
                return;
            case 5:
                if (fux->player == -1) {
                    fux->countdown = 0x5A;
                    fux->whatami = 7;
                    PlaySfx(0x3F0, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                    goto clr;
                }
                break;
            default:
                if (fux->player == -1) {
                    goto clr;
                }
                break;
            }
            if (fux->player >= 5) {
                dx = eng->XYZ.x;
                dz = -eng->XYZ.z;
            } else {
                ho = Heroes[fux->player].hero_eng;
                dx = eng->XYZ.x - ho->XYZ.x;
                dz = ho->XYZ.z - eng->XYZ.z;
                PlaySfx(PlayerHitSfx[fux->player], NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
            }
            ang = get_angle(-dx, dz);
            eng->xform.rot.vy = (ang + 0x100) & 0xE00;
            fux->status = 9;
            fux->will_do &= 0xFFFC;
            fux->can_do &= 0xFFFC;
            goto hit8;
        clr:
            fux->will_do &= 0xFFFC;
        }
    hit8:
        if ((fux->will_do & 8) && type == 6) {
            PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 0);
            KillTriggerNas(arg0);
            return;
        }
        if (eng->platform != NULL) {
            if (type == 5) {
                fux->countdown = 0x5A;
                fux->whatami = 7;
                PlaySfx(0x3F0, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
            } else if (type == 6) {
                PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 0);
                KillTriggerNas(arg0);
                return;
            }
        }
        break;
    }
    t = fux->status;
    if ((t == 1 || t == 0xB) && FollowContour(eng) != 0) {
        if (fux->status == 1) {
            fux->status = 4;
        } else {
            fux->status = 5;
        }
    }
    if (type == 7) {
        if (fux->countdown != 0) {
            fux->countdown--;
            eng->sub->entity = BashTickingCrateModels[fux->countdown / 30];
            if (fux->countdown % 30 == 0) {
                PlaySfx(0x3F0, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
            }
        } else {
            BC038Release(fux);
            PlayHitEffect(&eng->XYZ.x, (HitInfo *)fux, 0);
            KillTriggerNas(arg0);
        }
    }
}

void InitCratePickup(NAS *arg0) {
    EXTengine *eng;
    EXTobject *obj;
    SLONG_XYZ *pos;
    u16 id;
    s32 x, z;
    s32 r;

    eng = arg0->Engine;
    if (eng != NULL) {
        eng->XYZ.x = (eng->XYZ.x & ~0xFF) + 0x80;
        eng->XYZ.z = (eng->XYZ.z & ~0xFF) + 0x80;
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        r = Random(4);
        eng->sub->entity = BashPickupModels[r];
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            LinkObject(arg0);
            AddSolid(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = r + 0x12;
            obj->countdown = 0x78;
            if (eng->XYZ.y == World_floor_level) {
                obj->status = 0;
            } else {
                obj->status = 4;
                AddPowerupFloat(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->XYZ;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj != NULL ? obj->whatami : 0;
        if (x >= 0x14) {
            x = 0x13;
        }
        if (x < 0) {
            x = 0;
        }
        if (z >= 0x14) {
            z = 0x13;
        }
        if (z < 0) {
            z = 0;
        }
        ArenaGrid[z * 0x14 + x].flags = id;
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void InitShieldPickup(NAS *arg0) {
    EXTengine *eng;
    EXTobject *obj;
    SLONG_XYZ *pos;
    u16 id;
    s32 x, z;

    eng = arg0->Engine;
    if (eng != NULL) {
        eng->XYZ.x = (eng->XYZ.x & ~0xFF) + 0x80;
        eng->XYZ.z = (eng->XYZ.z & ~0xFF) + 0x80;
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        eng->sub->entity = 0x200D;
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            LinkObject(arg0);
            AddSolid(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = 0x12;
            obj->countdown = 0x78;
            if (eng->XYZ.y == World_floor_level) {
                obj->status = 0;
            } else {
                obj->status = 4;
                AddPowerupFloat(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->XYZ;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj != NULL ? obj->whatami : 0;
        if (x >= 0x14) {
            x = 0x13;
        }
        if (x < 0) {
            x = 0;
        }
        if (z >= 0x14) {
            z = 0x13;
        }
        if (z < 0) {
            z = 0;
        }
        ArenaGrid[z * 0x14 + x].flags = id;
        PlaySfx(0x41F, NULL, pos, 0x1000, 0x1E00, 0);
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void InitBootsPickup(NAS *arg0) {
    EXTengine *eng;
    EXTobject *obj;
    SLONG_XYZ *pos;
    u16 id;
    s32 x, z;

    eng = arg0->Engine;
    if (eng != NULL) {
        eng->XYZ.x = (eng->XYZ.x & ~0xFF) + 0x80;
        eng->XYZ.z = (eng->XYZ.z & ~0xFF) + 0x80;
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        eng->sub->entity = 0x200A;
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            LinkObject(arg0);
            AddSolid(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = 0x13;
            obj->countdown = 0x78;
            if (eng->XYZ.y == World_floor_level) {
                obj->status = 0;
            } else {
                obj->status = 4;
                AddPowerupFloat(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->XYZ;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj != NULL ? obj->whatami : 0;
        if (x >= 0x14) {
            x = 0x13;
        }
        if (x < 0) {
            x = 0;
        }
        if (z >= 0x14) {
            z = 0x13;
        }
        if (z < 0) {
            z = 0;
        }
        ArenaGrid[z * 0x14 + x].flags = id;
        PlaySfx(0x41F, NULL, pos, 0x1000, 0x1E00, 0);
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void InitAnvilPickup(NAS *arg0) {
    EXTengine *eng;
    EXTobject *obj;
    SLONG_XYZ *pos;
    u16 id;
    s32 x, z;

    eng = arg0->Engine;
    if (eng != NULL) {
        eng->XYZ.x = (eng->XYZ.x & ~0xFF) + 0x80;
        eng->XYZ.z = (eng->XYZ.z & ~0xFF) + 0x80;
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        eng->sub->entity = 0x2011;
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            LinkObject(arg0);
            AddSolid(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = 0x14;
            obj->countdown = 0x78;
            if (eng->XYZ.y == World_floor_level) {
                obj->status = 0;
            } else {
                obj->status = 4;
                AddPowerupFloat(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->XYZ;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj != NULL ? obj->whatami : 0;
        if (x >= 0x14) {
            x = 0x13;
        }
        if (x < 0) {
            x = 0;
        }
        if (z >= 0x14) {
            z = 0x13;
        }
        if (z < 0) {
            z = 0;
        }
        ArenaGrid[z * 0x14 + x].flags = id;
        PlaySfx(0x41F, NULL, pos, 0x1000, 0x1E00, 0);
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void InitSlowmoPickup(NAS *arg0) {
    EXTengine *eng;
    EXTobject *obj;
    SLONG_XYZ *pos;
    u16 id;
    s32 x, z;

    eng = arg0->Engine;
    if (eng != NULL) {
        eng->XYZ.x = (eng->XYZ.x & ~0xFF) + 0x80;
        eng->XYZ.z = (eng->XYZ.z & ~0xFF) + 0x80;
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        eng->sub->entity = 0x2009;
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            LinkObject(arg0);
            AddSolid(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = 0x15;
            obj->countdown = 0x78;
            if (eng->XYZ.y == World_floor_level) {
                obj->status = 0;
            } else {
                obj->status = 4;
                AddPowerupFloat(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->XYZ;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj != NULL ? obj->whatami : 0;
        if (x >= 0x14) {
            x = 0x13;
        }
        if (x < 0) {
            x = 0;
        }
        if (z >= 0x14) {
            z = 0x13;
        }
        if (z < 0) {
            z = 0;
        }
        ArenaGrid[z * 0x14 + x].flags = id;
        PlaySfx(0x41F, NULL, pos, 0x1000, 0x1E00, 0);
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void PlotPickup(NAS *arg0) {
    EXTengine *eng;
    EXTobject *obj;
    HeroInfo *h;

    eng = arg0->Engine;
    if (eng == NULL) {
        return;
    }
    eng->xform.dirty = 1;
    eng->xform.rot.vy += 0x71;
    obj = (EXTobject *)arg0->FUX;
    if (obj == NULL) {
        return;
    }
    switch (obj->status) {
    case 4:
    case 5:
        if (GravitateNas(arg0) != 0) {
            if (obj->status == 4) {
                PlaySfx(0x420, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
            }
            obj->status = 1;
            obj->can_do |= 0x18;
        }
        return;
    }
    UpdateBounceMotion(arg0);
    if (obj->will_do & 8) {
        obj->will_do &= ~8;
        if (obj->player != -1 && obj->player < 5) {
            Heroes[obj->player].hero_pu = obj->whatami;
            Heroes[obj->player].hero_pu_timer = 0xB4;
            Heroes[obj->player].hero_powerup->sub->entity = eng->sub->entity;
            switch (obj->whatami) {
            case 0x12:
                Heroes[obj->player].hero_pu_timer = 0x12C;
                Heroes[obj->player].hero_powerup->sub->entity = 0x200D;
                PlaySfx(0x41B, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                h = &Heroes[obj->player];
                h->hero_harm_Nas = NULL;
                h->hero_harm_player = -1;
                h->hero_harm_value = 0;
                h->hero_harm_flags = 0;
                break;
            case 0x13:
                Heroes[obj->player].hero_pu_timer = 0xF0;
                Heroes[obj->player].hero_powerup->sub->entity = eng->sub->entity;
                PlaySfx(0x41C, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            case 0x14:
                Heroes[obj->player].hero_pu_timer = 0xF0;
                Heroes[obj->player].hero_powerup->sub->entity = 0x2011;
                PlaySfx(0x452, &BashSfxEngine.sfx, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x41A, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            default:
                Heroes[obj->player].hero_pu_timer = 0xF0;
                Heroes[obj->player].hero_powerup->sub->entity = eng->sub->entity;
                PlaySfx(0x41D, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            }
        }
    } else if (obj->countdown != 0) {
        obj->countdown--;
        return;
    }
    CreateNAS(eng->XYZ.x, eng->XYZ.y - 0x100, eng->XYZ.z, 0x207, 0x11);
    KillNAS(arg0);
}

void BashHeroesTouched(s32 arg0, s32 arg1) {
    HeroInfo *base = Heroes;
    HeroInfo *b = base + arg1;
    HeroInfo *a;

    if ((b->hero_immunity | b->hero_hit) == 0) {
        a = base + arg0;
        if (!(a->hero_status & 0xFA00) && !(b->hero_status & 0xFA00)) {
            switch (a->hero_pu) {
            case 0x19:
            case 0x1E:
                break;
            case 0x14:
                if (a->hero_pu_lock == 0) {
                    b->hero_pu_timer = a->hero_pu_timer + 10;
                    b->hero_pu = a->hero_pu;
                    b->hero_pu_lock = 0x1E;
                    a->hero_pu = 0x1E;
                    a->hero_pu_timer = 0;
                    b->hero_powerup->sub->entity = a->hero_powerup->sub->entity;
                    PlaySfx(0x454, 0, 0, 0x1000, 0x1E00, 0);
                }
                break;
            }
        }
    }
}

void BashHero_ExtraEffects(s32 arg0) {
    HeroInfo *h;
    EXTengine *pu;
    subengine *sub;
    VECTOR pos;
    s32 y;
    s32 t;
    HeroInfo *base;
    s32 top;
    HeroInfo *h2;
    HeroInfo *b2;
    HeroInfo *h4;
    HeroInfo *h5;
    HeroInfo *h6;
    HeroInfo *b4;
    HeroInfo *b5;
    HeroInfo *b6;

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
        if (sub_find_point(h->hero_eng, h->hero_sub, 0x80, &pos) != NULL) {
            top = pos.vy - 0x40;
            y = top - h->hero_eng->XYZ.y;
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
            case 8:
            default:
                y = -0x280;
                break;
            }
        }
        pu->XYZ = Heroes[arg0].hero_eng->XYZ;
        sub->XYZ.y = y;
        switch (Heroes[arg0].hero_pu) {
        case 0x1B:
            sub->XYZ.y -= 0x60;
            /* fallthrough */
        case 0x19:
        case 0x1C:
            b2 = Heroes;
            h2 = b2 + arg0;
            if (h2->hero_weapon_count != 0) {
                sub->flags |= 0x8000;
                pu->XYZ = h2->hero_eng->XYZ;
                pu->xform.rot.vy += 0xAA;
            } else {
                h2->hero_pu = 0x1E;
                h2->hero_pu_timer = 0;
                h2->hero_status &= 0xFFEF;
                if (pu->sfx != NULL) {
                    StopSfx(pu->sfx);
                }
            }
            break;
        case 0x1E:
            Heroes[arg0].hero_pu = 0;
            Heroes[arg0].hero_status &= 0xFFEF;
            sub->flags &= ~0x8000;
            break;
        case 0x14:
            b4 = Heroes;
            h4 = b4 + arg0;
            if (h4->hero_pu_timer != 0) {
                sub->flags |= 0x8000;
                pu->XYZ = h4->hero_eng->XYZ;
                do {
                    pu->xform.rot.vy += 0xAA;
                } while (0);
                if (BashSfxEngine.sfx != NULL) {
                    t = h4->hero_pu_timer;
                    if (t >= 0x12C) {
                        t = 0x12B;
                    }
                    sfx_set_pitch(BashSfxEngine.sfx, (0x12C - t) * 0xF00 / 300 + 0x1E00);
                }
            } else {
                if (BashSfxEngine.sfx != NULL) {
                    StopSfx(BashSfxEngine.sfx);
                }
                h4->hero_pu = 0;
                SetHeroAction(arg0, 0x12);
                pu->xform.rot.vy = 0;
                sub->XYZ.y = 0;
            }
            break;
        case 0x12:
            b5 = Heroes;
            h5 = b5 + arg0;
            if (h5->hero_pu_timer != 0) {
                sub->flags |= 0x8000;
                pu->XYZ = h5->hero_eng->XYZ;
                pu->xform.rot.vy += 0xAA;
                sub->XYZ.z = sub->XYZ.y >> 1;
            } else {
                h5->hero_pu = 0;
                pu->xform.rot.vy = 0;
                sub->XYZ.x = 0;
                sub->XYZ.y = 0;
                sub->XYZ.z = 0;
                sub->flags &= ~0x8000;
                PlaySfx(0x421, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            break;
        default:
            b6 = Heroes;
            h6 = b6 + arg0;
            if (h6->hero_pu_timer != 0) {
                sub->flags |= 0x8000;
            } else {
                h6->hero_pu = 0;
                sub->flags &= ~0x8000;
            }
            pu->XYZ = Heroes[arg0].hero_eng->XYZ;
            pu->xform.rot.vy += 0xAA;
            break;
        }
        pu->xform.dirty = 1;
    }
}

void *CrateCollide(EXTengine *eng, void *arg1, s32 who, s32 heroes) {
    s32 d;
    FlagRecord *box;
    FlagRecord *rec;
    EXTengine *e;
    EXTobject *obj;
    NAS *nas;
    s32 i;
    s32 r;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 rr;
    s32 lo;
    s32 hi;

    box = FindCollisionBox(eng, 0x40);
    if (box != NULL) {
        r = box->radius;
        top = eng->XYZ.y + box->dy;
        bot = eng->XYZ.y + box->height;
        if (heroes != 0) {
            for (i = 0; i < 4; i++) {
                if (who == i) {
                    continue;
                }
                if (Heroes[i].hero_status & 0xF800) {
                    continue;
                }
                e = Heroes[i].hero_eng;
                if (e == NULL) {
                    continue;
                }
                rec = FindCollisionBox(e, 0x40);
                if (rec == NULL) {
                    continue;
                }
                lo = e->XYZ.y + rec->dy;
                hi = e->XYZ.y + rec->height;
                if (bot < lo && hi < top) {
                    dx = e->XYZ.x - eng->XYZ.x;
                    dz = dx * dx;
                    dx = e->XYZ.z - eng->XYZ.z;
                    d = dz + dx * dx;
                    rr = rec->radius + r;
                    rr = rr * rr;
                    if (d < rr) {
                        return Heroes[i].hero;
                    }
                }
            }
        }
        for (nas = HeadOfSolids; nas != NULL; nas = nas->Solid.Child) {
            if (nas->Class & 0x200) {
                e = nas->Engine;
                if (e != NULL && e != eng) {
                    rec = FindCollisionBox(e, 0x40);
                    if (rec != NULL) {
                        lo = e->XYZ.y + rec->dy;
                        hi = e->XYZ.y + rec->height;
                        if (bot < lo && hi < top) {
                            dx = e->XYZ.x - eng->XYZ.x;
                            dz = dx * dx;
                            dx = e->XYZ.z - eng->XYZ.z;
                            d = dz + dx * dx;
                            rr = rec->radius + r;
                            rr = rr * rr;
                            if (d < rr) {
                                obj = FindExtension(nas, 0x1000);
                                if (obj != NULL && (obj->whatami < 0xE || obj->whatami != who)) {
                                    if (obj->can_do & 8) {
                                        obj->player = -1;
                                        obj->will_do |= 8;
                                    }
                                    if (obj->can_do & 2) {
                                        obj->will_do |= 2;
                                        if (obj->status != 2) {
                                            obj->player = -1;
                                        }
                                    }
                                    if (obj->can_do & 0x10) {
                                        return 0;
                                    }
                                    return nas;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

NAS *CrateHitSolids(EXTengine *eng, s32 who) {
    FlagRecord *box;
    FlagRecord *rec;
    EXTengine *e;
    EXTobject *obj;
    NAS *nas;
    s32 r;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 rr;
    s32 lo;
    s32 hi;

    box = FindCollisionBox(eng, 0x40);
    if (box != NULL) {
        r = box->radius;
        top = eng->XYZ.y + box->dy;
        bot = eng->XYZ.y + box->height;
        for (nas = HeadOfSolids; nas != NULL; nas = nas->Solid.Child) {
            if (nas->Class & 0x200) {
                e = nas->Engine;
                if (e != NULL && e != eng) {
                    rec = FindCollisionBox(e, 0x40);
                    if (rec != NULL) {
                        lo = e->XYZ.y + rec->dy;
                        hi = e->XYZ.y + rec->height;
                        if (bot < lo && hi < top) {
                            dx = e->XYZ.x - eng->XYZ.x;
                            dz = e->XYZ.z - eng->XYZ.z;
                            rr = rec->radius + r;
                            rr = rr * rr;
                            if (dx * dx + dz * dz < rr) {
                                obj = FindExtension(nas, 0x1000);
                                if (obj != NULL && (obj->whatami < 0xE || obj->whatami != who) && obj->status != 1) {
                                    if (obj->can_do & 8) {
                                        obj->player = -1;
                                        obj->will_do |= 8;
                                    }
                                    if (obj->can_do & 2) {
                                        obj->will_do |= 2;
                                        if (obj->status != 2) {
                                            obj->player = -1;
                                        }
                                    }
                                    if (obj->can_do & 0x10) {
                                        return NULL;
                                    }
                                    return nas;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

s32 CrateHitHeroes(EXTengine *eng, EXTobject *flags, u32 mask) {
    s32 sum;
    FlagRecord *rec;
    FlagRecord *rec2;
    EXTengine *other;
    HeroInfo *h;
    s32 i;
    s32 top;
    s32 bottom;
    s32 dx;
    s32 dz;
    s32 r;
    s32 lo;
    s32 hi;

    rec =FindCollisionBox(eng, 0x40);
    i = 0;
    if (rec != NULL) {
        r = rec->radius;
        bottom = eng->XYZ.y + rec->dy;
        top = eng->XYZ.y + rec->height;
        for (; i < 4; i++) {
            h = &Heroes[i];
            if (h->hero_status & 0xF800) {
                continue;
            }
            other = h->hero_eng;
            if (other == NULL) {
                continue;
            }
            rec2 = FindCollisionBox(other, 0x40);
            if (rec2 == NULL) {
                continue;
            }
            lo = other->XYZ.y + rec2->dy;
            hi = other->XYZ.y + rec2->height;
            if (top < lo && hi < bottom) {
                dx = other->XYZ.x - eng->XYZ.x;
                dz = other->XYZ.z - eng->XYZ.z;
                sum = rec2->radius + r;
                sum = sum * sum;
                if (dx * dx + dz * dz < sum) {
                    if (flags != NULL) {
                        if (flags->can_do & mask) {
                            flags->player = i;
                            flags->will_do |= mask;
                        }
                    }
                    return i;
                }
            }
        }
    }
    return -1;
}

void CrateBreak(EXTengine *eng, EXTobject *fux) {
    PlaySfx(HitSfx.crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
    SpawnModelFx(2, (s32 *)&eng->XYZ, &HeroModelBank[DB_CrateObjects]);
    if ((HeroHooks.KeepInBounds(eng) ^ 1) != 0) {
        CreateNAS(eng->XYZ.x, eng->XYZ.y, eng->XYZ.z, 0x206, 0x11);
    }
}

s32 CrateFallTime(s32 pos, EXTmotion *c) {
    s32 sum = 0;
    s32 a = c->Xvel;
    s32 b = c->Yvel;
    s32 delta = c->Yacc - c->Ydec;
    s32 dir = c->Ydir;
    s32 step = c->Xacc - c->Xdec;
    s32 max1 = c->Xmax;
    s32 max2 = c->Ymax;
    s32 n = 0;

    if (pos < World_floor_level) {
        do {
            a += step;
            if (a < 0) {
                a = 0;
            }
            if (a > max1) {
                a = max1;
            }
            sum += a;
            b += delta;
            if (b < 0) {
                b = 0;
            }
            if (b > max2) {
                b = max2;
            }
            if (dir != 0) {
                pos -= b;
            } else {
                pos += b;
            }
            if (b == 0) {
                delta = c->Ydec;
                dir = 0;
            }
            n++;
        } while (pos < World_floor_level);
    }
    c->Fdist = sum;
    c->Ftime = n;
    return sum;
}

void CrateFindFreeSpot(EXTengine *eng) {
    NAS *nas;
    EXTengine *other;
    s32 again;

    do {
        again = 0;
        nas = HeadOfSolids;
        eng->XYZ.x = (eng->XYZ.x & ~0xFF) + 0x80;
        eng->XYZ.z = (eng->XYZ.z & ~0xFF) + 0x80;
        while (nas != NULL) {
            if (nas->Type == CrateAstType) {
                other = nas->Engine;
                if (other != NULL && other->XYZ.x == eng->XYZ.x && other->XYZ.z == eng->XYZ.z) {
                    eng->XYZ.x = Random(0xC00) - 0x600;
                    eng->XYZ.z = Random(0xC00) - 0x600;
                    nas = NULL;
                    again = 1;
                }
            }
            if (nas != NULL) {
                nas = nas->Solid.Child;
            }
        }
    } while (again);
}

void SetBashHeroHit(s32 arg0) {
    HeroInfo *b = Heroes;
    HeroInfo *h = &b[arg0];
    HeroInfo *b2;
    HeroInfo *b3;
    HeroInfo *h2;
    HeroInfo *h3;

    if (h->hero_pu != 0x12) {
        h->hero_hit = 0x1E;
    }
    h->hero_move->velocity_enable = 0;
    switch (h->hero_pu) {
    case 0x13:
        h->hero_pu_timer = 0;
    case 0x19:
    case 0x1B:
    case 0x1C:
        Heroes[arg0].hero_weapon_count = 0;
    }
    b2 = Heroes;
    h2 = &b2[arg0];
    if (h2->hero_object != NULL) {
        h2->hero_object->player = h2->hero_harm_player;
        h2->hero_object->status = 4;
        h2->hero_object = NULL;
    }
    if (h2->hero_health <= 0 && !(GameCheats & 2)) {
        h2->hero_status |= 0x2000;
    }
    b3 = Heroes;
    h3 = &b3[arg0];
    if (h3->hero_eng->sfx != NULL) {
        StopSfx(h3->hero_eng->sfx);
    }
    PlaySfx(HeroHitSfx[h3->hero_name][Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
    SetHeroSeq(arg0, 9);
}
