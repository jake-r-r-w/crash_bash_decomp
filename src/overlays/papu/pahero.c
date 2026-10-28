#include "common.h"
#include "bash/main.h"
#include "bash/papu_ovl.h"
#include "bash/papu_pahero.h"
#include "bash/papu.h"
#include "bash/bash.h"
#include "bash/xtras.h"

/* data, in address order */
u16 PACrateTickModels[10] = { 0x2010, 0x2011, 0x2012, 0x2012, 0x2012, 0x2012, 0x2012, 0x2012, 0x2012, 0 };
PapuWeaponVec PAHeroThrowVecs[22] = {
    { 0x5000, 0x3000, 0x5000 }, { 0x3800, 0x3000, 0x3800 }, { 0x1800, 0x2000, 0x1800 },
    { 0x1000, 0x1000, 0x1000 }, { 0x800, 0xA00, 0x800 }, { 0xA00, 0xA00, 0xA00 },
    { 0x1000, 0x800, 0x1000 }, { 0xA00, 0xA00, 0xA00 }, { 0x800, 0x1000, 0x800 },
    { 0xA00, 0xA00, 0xA00 }, { 0x1000, 0x800, 0x1000 }, { 0xA00, 0xA00, 0xA00 },
    { 0x800, 0x1000, 0x800 }, { 0xA00, 0xA00, 0xA00 }, { 0x1000, 0x800, 0x1000 },
    { 0xA00, 0xA00, 0xA00 }, { 0x800, 0x1000, 0x800 }, { 0xA00, 0xA00, 0xA00 },
    { 0x1000, 0x800, 0x1000 }, { 0xA00, 0xA00, 0xA00 }, { 0x800, 0x1000, 0x800 },
    { 0xA00, 0xA00, 0xA00 },
};
s16 PapuUnused7[2] = {0};
/* two zero bytes nothing reads, the overlay file's last (retail's is 0xB682 bytes long) */
u16 PapuUnusedTail = 0;

/* Drop obj from the hero or slot that holds it (PlotPACrate). */
static inline void PapuReleaseObj(EXTobject *obj, u16 p) {
    HeroInfo *h;
    HeroInfo *b;
    s32 i = (s16)p;

    if (i < 0x14) {
        if (i != -1 && i < 5) {
            b = Heroes;
            h = &b[i];
            if (h->hero_object == obj) {
                h->hero_object = NULL;
                if (h->hero_eng->sfx != NULL) {
                    StopSfx(h->hero_eng->sfx);
                }
            }
        }
    } else {
        i -= 0x14;
        if ((s16)p < 6) {
            if (PAPU_SLOT_OBJ(i) == (s32)obj) {
                PAPU_SLOT_OBJ(i) = 0;
            }
        }
    }
}

void PlotPACrate(NAS *nas) {
    VECTOR a;
    VECTOR b;
    EXTengine *eng;
    EXTobject *obj;
    PapuSlide *slide;
    EXTengine *other;
    EXTengine *tgt;
    NAS *hit;
    s32 kind;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 ang;
    s32 s;
    s32 c;
    s16 p;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    obj = nas->FUX;
    if (obj == NULL) {
        return;
    }
    kind = obj->whatami;
    slide = FindExtension(nas, 0x800);
    if (slide == NULL) {
        return;
    }
    switch (obj->status) {
    case 0:
        obj->status = 1;
        obj->can_do &= 0xFFD4;
        if (kind >= 4 && (kind < 6 || kind == 7)) {
            obj->can_do |= 3;
        }
        eng->unk5E = 1;
        eng->unk1E = 0;
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        obj->player = -1;
        FetchDefaultColBox(eng, 0x50);
        break;
    case 2:
        p = obj->player;
        if (p != -1 && p < 5) {
            other = Heroes[p].hero_eng;
            if (other != NULL) {
                if (sub_find_point(other, other->sub, 0x10, &a) != NULL) {
                    if (sub_find_point(other, other->sub, 0x20, &b) != NULL) {
                        dx = a.vx - b.vx;
                        dy = b.vy - a.vy;
                        dz = a.vz - b.vz;
                        eng->unk1E = get_angle(dx, dz) - 0x200;
                        ang = 0x271 - get_angle(csqrt(dx * dx + dz * dz) >> 6, dy);
                        eng->unk1C = ang;
                        eng->unk10.y = b.vy - (rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * 128 >> 12);
                        dx = a.vx - dx / 2;
                        dz = a.vz - dz / 2;
                        eng->unk20 = 0;
                        eng->unk10.x = dx;
                        eng->unk10.z = dz;
                    }
                } else if (sub_find_point(other, other->sub, 1, &b) != NULL) {
                    eng->unk10 = *(SLONG_XYZ *)&b;
                    eng->unk1E = other->unk1E;
                } else {
                    obj->status = 4;
                }
            }
            if (obj->will_do & 4) {
                obj->can_do &= 0xFFDB;
                obj->will_do &= 0xFFFB;
                eng->unk1E = Heroes[obj->player].hero_move->facing_angle;
                eng->unk1C = 0;
                eng->unk20 = 0;
                slide->unkE = PAPU_STATS(unk4, Heroes[obj->player].hero_name)->a;
                slide->unk14 = PAPU_STATS(unk4, Heroes[obj->player].hero_name)->b;
                slide->unk10 = PAPU_STATS(unk4, Heroes[obj->player].hero_name)->c;
                slide->unk12 = PAPU_STATS(unk4, Heroes[obj->player].hero_name)->d;
                slide->unk18 = PAPU_STATS(unk4, Heroes[obj->player].hero_name)->e;
                slide->unk1E = PAPU_STATS(unk4, Heroes[obj->player].hero_name)->f;
                slide->unk1A = PAPU_STATS(unk4, Heroes[obj->player].hero_name)->g;
                slide->unk1C = PAPU_STATS(unk4, Heroes[obj->player].hero_name)->h;
                slide->unkC = PAPU_STATS(unk4, Heroes[obj->player].hero_name)->i;
                obj->status = 8;
            }
        } else {
            obj->status = 4;
        }
        eng->unk5E = 1;
        break;
    case 0x13:
        if ((u32)((u16)obj->player - 0x14) < 6) {
            if (PAPU_SLOT_OBJ(obj->player - 0x14) != (s32)obj && !(obj->will_do & 4)) {
                obj->status = 4;
                break;
            }
            other = PAPU_SLOT_ENG(obj->player - 0x14);
            if (other != NULL) {
                if (sub_find_point(other, other->sub, 0x10, &a) != NULL) {
                    if (sub_find_point(other, other->sub, 0x20, &b) != NULL) {
                        dx = a.vx - b.vx;
                        dy = b.vy - a.vy;
                        dz = a.vz - b.vz;
                        eng->unk1E = get_angle(dx, dz) - 0x200;
                        ang = 0x271 - get_angle(csqrt(dx * dx + dz * dz) >> 6, dy);
                        eng->unk1C = ang;
                        eng->unk10.y = b.vy - (rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * 128 >> 12);
                        dx = a.vx - dx / 2;
                        dz = a.vz - dz / 2;
                        eng->unk20 = 0;
                        eng->unk10.x = dx;
                        eng->unk10.z = dz;
                    }
                } else if (sub_find_point(other, other->sub, 1, &b) != NULL) {
                    eng->unk10 = *(SLONG_XYZ *)&b;
                    eng->unk1E = other->unk1E;
                } else {
                    obj->status = 4;
                }
            } else {
                obj->status = 4;
            }
            if (obj->will_do & 4) {
                obj->can_do &= 0xFFDB;
                obj->will_do &= 0xFFFB;
                eng->unk1E = other->unk1E;
                eng->unk1C = 0;
                eng->unk20 = 0;
                slide->unkE = PAPU_STATS(unk4, 1)->a;
                slide->unk14 = PAPU_STATS(unk4, 1)->b;
                slide->unk10 = PAPU_STATS(unk4, 1)->c;
                slide->unk12 = PAPU_STATS(unk4, 1)->d;
                slide->unk18 = PAPU_STATS(unk4, 1)->e;
                slide->unk1E = PAPU_STATS(unk4, 1)->f;
                slide->unk1A = PAPU_STATS(unk4, 1)->g;
                slide->unk1C = PAPU_STATS(unk4, 1)->h;
                slide->unkC = PAPU_STATS(unk4, 1)->i;
                obj->status = 8;
            }
        } else {
            obj->status = 4;
        }
        eng->unk5E = 1;
        break;
    case 7:
        s = eng->unk2C.x;
        s += 0x100;
        if (s >= 0x1000) {
            s = 0x1000;
            obj->status = 0;
            eng->flags &= ~0x21;
        }
        eng->unk2C.x = s;
        eng->unk2C.y = s;
        eng->unk2C.z = s;
        eng->sub->unk76 = s;
        eng->unk5E = 1;
        break;
    case 4:
        slide->unk18 = 0x50;
        slide->unk1E = 0x50;
        slide->unk1C = 6;
        slide->unkC = 0xFF;
        slide->unk1A = 0;
        eng->unk1C = 0;
        eng->unk20 = 0;
        obj->status = 8;
        break;
    case 5:
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        slide->unk1E = 0x50;
        slide->unk1C = 6;
        slide->unkC = 0xFF;
        slide->unk18 = 0;
        slide->unk1A = 0;
        eng->unk1C = 0;
        eng->unk20 = 0;
        obj->status = 8;
        break;
    case 8:
        if (eng->unk10.y > World_floor_level) {
            eng->flags |= 0x02000000;
            eng->sub->unk68 = 0x190;
            eng->sub->flags |= 0x02000000;
            eng->sub->unk68 = 0x190;
        }
        if (GravitateNas(nas) != 0) {
            eng->unk10.y += 0x20;
            PACrateCollide(eng, obj->player, 0);
            eng->unk10.y -= 0x20;
            switch (kind) {
            case 4:
                PlayHitEffect(&eng->unk10.x, (HitInfo *)obj, 1);
                KillTriggerNas(nas);
                return;
            case 5:
            case 7:
                PlayHitEffect(&eng->unk10.x, (HitInfo *)obj, 1);
                KillTriggerNas(nas);
                return;
            default:
                obj->status = 0;
                break;
            }
        }
        hit = PACrateCollide(eng, obj->player, 1);
        if (hit != NULL) {
            if (hit == PapuBoss.nas) {
                PapuBoss.unk28 = 1;
            }
            switch (kind) {
            case 4:
                PlayHitEffect(&eng->unk10.x, (HitInfo *)obj, 1);
                KillTriggerNas(nas);
                return;
            case 5:
            case 7:
                PlayHitEffect(&eng->unk10.x, (HitInfo *)obj, 1);
                KillTriggerNas(nas);
                return;
            default:
                slide->unk12 = 0;
                slide->unk10 = 0;
                slide->unk14 = 0;
                slide->unkE = 0;
                break;
            }
        }
        break;
    case 9:
        switch (kind) {
        case 5:
            obj->countdown = 0x5A;
            obj->whatami = 7;
            PlaySfx(0x3F0, NULL, (SLONG_XYZ *)&eng->unk10, 0x1000, 0x1E00, 0);
            c = obj->player;
            obj->status = 0xB;
            obj->timer = 2;
            if ((u32)c >= 5) {
                c = 0;
            }
            slide->unkE = PAPU_STATS(unk0, Heroes[c].hero_name)->a;
            slide->unk14 = PAPU_STATS(unk0, Heroes[c].hero_name)->b;
            slide->unk10 = PAPU_STATS(unk0, Heroes[c].hero_name)->c;
            slide->unk12 = PAPU_STATS(unk0, Heroes[c].hero_name)->d;
            PapuGridKey((T_B8080 *)eng);
            PACrateAimAtGrid((T_B7F14_A *)eng, (T_B7F14_B *)slide);
            break;
        case 4:
        case 7:
            PlaySfx(0x3F9, &eng->sfx, (SLONG_XYZ *)&eng->unk10, 0x1000, 0x1E00, 0);
            c = obj->player;
            obj->status = 0xB;
            obj->timer = 2;
            if ((u32)c >= 5) {
                c = 0;
            }
            slide->unkE = PAPU_STATS(unk0, Heroes[c].hero_name)->a;
            slide->unk14 = PAPU_STATS(unk0, Heroes[c].hero_name)->b;
            slide->unk10 = PAPU_STATS(unk0, Heroes[c].hero_name)->c;
            slide->unk12 = PAPU_STATS(unk0, Heroes[c].hero_name)->d;
            PapuGridKey((T_B8080 *)eng);
            PACrateAimAtGrid((T_B7F14_A *)eng, (T_B7F14_B *)slide);
            break;
        default:
            obj->status = 0;
            break;
        }
        break;
    case 0xB:
        slide->unkE += slide->unk10 - slide->unk12;
        if (slide->unkE < 0) {
            slide->unkE = 0;
        }
        if (slide->unkE > slide->unk14) {
            slide->unkE = slide->unk14;
        }
        eng->unk10.x -= (slide->unkE * rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2) >> 12;
        eng->unk10.z -= (slide->unkE * rcossin_tbl[eng->unk1E & 0xFFF].unk2) >> 12;
        switch (slide->dir & 0xF0) {
        case 0x10:
        case 0x30:
        case 0x90:
            if (slide->unk2A >= eng->unk10.z) {
                slide->unkE = 0;
            }
            break;
        case 0x80:
            if (eng->unk10.x >= slide->unk16) {
                slide->unkE = 0;
            }
            break;
        case 0x20:
            if (slide->unk16 >= eng->unk10.x) {
                slide->unkE = 0;
            }
            break;
        default:
            if (eng->unk10.z >= slide->unk2A) {
                slide->unkE = 0;
            }
            break;
        }
        if (slide->unkE == 0) {
            eng->unk5E = 1;
            eng->unk1E = 0;
            eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
            eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
            obj->status = 0;
        }
        if (obj->timer != 0) {
            obj->timer--;
        }
        if (PACrateCollide(eng, obj->player, 1) != NULL) {
            slide->unk12 = 0;
            slide->unk10 = 0;
            slide->unk14 = 0;
            slide->unkE = 0;
            if (obj->timer == 0) {
                switch (kind) {
                case 4:
                    PapuReleaseObj(obj, obj->player);
                    PlayHitEffect(&eng->unk10.x, (HitInfo *)obj, 1);
                    KillTriggerNas(nas);
                    return;
                case 5:
                case 7:
                    PapuReleaseObj(obj, obj->player);
                    PlayHitEffect(&eng->unk10.x, (HitInfo *)obj, 1);
                    KillTriggerNas(nas);
                    return;
                }
            }
        }
        if (FollowContour(eng) != 0) {
            slide->unkE = 0x40;
            slide->unk14 = 0x40;
            slide->unk1E = 0x50;
            slide->unk1A = 6;
            slide->unk10 = 0;
            slide->unk12 = 0;
            slide->unk18 = 0;
            slide->unk1C = 0;
            slide->unkC = 0;
            eng->unk1C = 0;
            eng->unk20 = 0;
            obj->status = 8;
        }
        break;
    case 0xA:
        if (obj->countdown != 0) {
            obj->countdown--;
            break;
        }
        PapuReleaseObj(obj, obj->player);
        PlayHitEffect(&eng->unk10.x, (HitInfo *)obj, 1);
        KillTriggerNas(nas);
        return;
    case 0x63:
        if (GravitateNas(nas) == 0) {
            s = eng->unk2C.x;
            s -= 0x100;
            if (s > 0x100) {
                eng->unk2C.x = s;
                eng->unk2C.y = s;
                eng->unk2C.z = s;
                eng->sub->unk76 = s;
                eng->unk5E = 1;
                break;
            }
        }
        KillTriggerNas(nas);
        return;
    default:
        if (obj->will_do & 0x100) {
            obj->status = 0xA;
            obj->countdown = 0xA;
            obj->will_do &= 0xFEFC;
            obj->can_do &= 0xFEFC;
        }
        if (obj->will_do & 1) {
            p = obj->player;
            if (p != -1 && p < 5) {
                obj->status = 2;
                if (kind == 5) {
                    obj->countdown = 0x5A;
                    obj->whatami = 7;
                    PlaySfx(0x3F0, NULL, (SLONG_XYZ *)&eng->unk10, 0x1000, 0x1E00, 0);
                }
                Heroes[obj->player].hero_object = obj;
                eng->unk1E = Heroes[obj->player].hero_move->facing_angle;
                obj->will_do &= 0xFFFC;
                obj->can_do = (obj->can_do & 0xFFFC) | 4;
            } else if ((u32)((u16)obj->player - 0x14) >= 6) {
                obj->will_do &= 0xFFFE;
            } else {
                obj->status = 0x13;
                if (kind == 5) {
                    obj->countdown = 0x5A;
                    obj->whatami = 7;
                    PlaySfx(0x3F0, NULL, (SLONG_XYZ *)&eng->unk10, 0x1000, 0x1E00, 0);
                }
                PAPU_SLOT_OBJ(obj->player - 0x14) = (s32)obj;
                eng->unk1E = PAPU_SLOT_DYN(obj->player - 0x14)->unk16;
                obj->will_do &= 0xFFFC;
                obj->can_do = (obj->can_do & 0xFFFC) | 4;
            }
        }
        if (obj->will_do & 2) {
            if (kind == 5 && obj->player == -1) {
                obj->countdown = 0x5A;
                obj->whatami = 7;
                PlaySfx(0x3F0, NULL, (SLONG_XYZ *)&eng->unk10, 0x1000, 0x1E00, 0);
                obj->will_do &= 0xFFFC;
            } else if (obj->player != -1) {
                p = obj->player;
                if (p >= 5) {
                    if ((u32)(p - 0x14) >= 7) {
                        dx = eng->unk10.x;
                        dz = -eng->unk10.z;
                    } else {
                        tgt = PAPU_SLOT_ENG(p - 0x14);
                        dx = eng->unk10.x - tgt->unk10.x;
                        dz = tgt->unk10.z - eng->unk10.z;
                    }
                } else {
                    tgt = Heroes[p].hero_eng;
                    dx = eng->unk10.x - tgt->unk10.x;
                    dz = tgt->unk10.z - eng->unk10.z;
                    PlaySfx(PlayerHitSfx[p], NULL, (SLONG_XYZ *)&eng->unk10, 0x1000, 0x1E00, 0);
                }
                eng->unk1E = (get_angle(-dx, dz) + 0x100) & 0xE00;
                obj->status = 9;
                obj->will_do &= 0xFFFC;
                obj->can_do &= 0xFFFC;
            } else {
                obj->will_do &= 0xFFFC;
            }
        }
        if (eng->platform != NULL && kind == 5) {
            obj->countdown = 0x5A;
            obj->whatami = 7;
            PlaySfx(0x3F0, NULL, (SLONG_XYZ *)&eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    }
    if ((obj->status == 1 || obj->status == 0xB) && FollowContour(eng) != 0) {
        if (obj->status == 1) {
            obj->status = 4;
        } else {
            obj->status = 5;
        }
    }
    if (kind == 7) {
        if (obj->countdown != 0) {
            obj->countdown--;
            eng->sub->entity = PACrateTickModels[(s16)(obj->countdown / 30)];
            if ((s16)(obj->countdown % 30) == 0) {
                PlaySfx(0x3F0, NULL, (SLONG_XYZ *)&eng->unk10, 0x1000, 0x1E00, 0);
            }
        } else {
            PapuReleaseObj(obj, obj->player);
            PlayHitEffect(&eng->unk10.x, (HitInfo *)obj, 0);
            KillTriggerNas(nas);
        }
    }
}

NAS *PACrateCollide(EXTengine *eng, s32 self, s32 checkHeroes) {
    FlagRecord *rec;
    FlagRecord *rec2;
    EXTengine *other;
    HeroInfo *h;
    NAS *nas;
    PapuHitFlags *flags;
    s32 i;
    s32 r;
    s32 top;
    s32 bottom;
    s32 dx;
    s32 dz;
    s32 sum;
    s32 r2;
    s32 lo;
    s32 hi;

    rec =FindCollisionBox(eng, 0x40);
    if (rec == NULL) {
        return NULL;
    }
    r = rec->radius;
    bottom = eng->unk10.y + rec->unk2;
    top = eng->unk10.y + rec->height;
    if (checkHeroes != 0) {
        for (i = 0; i < 4; i++) {
            if (self == i) {
                continue;
            }
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
            lo = other->unk10.y + rec2->unk2;
            hi = other->unk10.y + rec2->height;
            if (top < lo && hi < bottom) {
                dx = other->unk10.x;
                dx -= eng->unk10.x;
                dz = other->unk10.z - eng->unk10.z;
                lo = rec2->radius;
                lo += r;
                lo *= lo;
                if (dx * dx + dz * dz < lo) {
                    return h->hero;
                }
            }
        }
    }
    other = PapuBoss.eng;
    if (other != NULL) {
        rec2 = FindCollisionBox(other, 0x40);
        if (rec2 != NULL) {
            lo = other->unk10.y + rec2->unk2;
            hi = other->unk10.y + rec2->height;
            if (top < lo && hi < bottom) {
                lo = rec2->radius;
                if (PapuBossFlag == 0) {
                    lo += 0x100;
                }
                dx = other->unk10.x;
                dx -= eng->unk10.x;
                dz = other->unk10.z - eng->unk10.z;
                lo += r;
                lo *= lo;
                if (dx * dx + dz * dz < lo) {
                    return PapuBoss.nas;
                }
            }
        }
    }
    for (nas = HeadOfSolids; nas != NULL; nas = nas->Solid.Child) {
        if (!(nas->Class & 0x200)) {
            continue;
        }
        other = nas->Engine;
        if (other == NULL || other == eng) {
            continue;
        }
        rec2 = FindCollisionBox(other, 0x40);
        if (rec2 == NULL) {
            continue;
        }
        lo = other->unk10.y + rec2->unk2;
        hi = other->unk10.y + rec2->height;
        if (top < lo && hi < bottom) {
            dx = other->unk10.x;
            dx -= eng->unk10.x;
            dz = other->unk10.z - eng->unk10.z;
            lo = rec2->radius;
            lo += r;
            lo *= lo;
            if (dx * dx + dz * dz < lo) {
                flags = FindExtension(nas, 0x1000);
                if (flags != NULL) {
                    if (flags->unk14 >= 0xE && flags->unk14 == self) {
                        continue;
                    }
                    if (flags->unk1C & 8) {
                        flags->unk16 = -1;
                        flags->unk1E |= 8;
                    }
                    if (flags->unk1C & 2) {
                        flags->unk16 = -1;
                        flags->unk1E |= 2;
                    }
                }
                if (flags->unk1C & 0x10) {
                    return NULL;
                }
                return nas;
            }
        }
    }
    return NULL;
}

s32 PACrateHitHeroes(EXTengine *eng, PapuHitFlags *flags, u32 mask) {
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

    rec = FindCollisionBox(eng, 0x40);
    i = 0;
    if (rec != NULL) {
        r = rec->radius;
        bottom = eng->unk10.y + rec->unk2;
        top = eng->unk10.y + rec->height;
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
            lo = other->unk10.y + rec2->unk2;
            hi = other->unk10.y + rec2->height;
            if (top < lo && hi < bottom) {
                dx = other->unk10.x - eng->unk10.x;
                dz = other->unk10.z - eng->unk10.z;
                sum = rec2->radius + r;
                sum = sum * sum;
                if (dx * dx + dz * dz < sum) {
                    if (flags != NULL) {
                        if (flags->unk1C & mask) {
                            flags->unk16 = i;
                            flags->unk1E |= mask;
                        }
                    }
                    return i;
                }
            }
        }
    }
    return -1;
}

void PACrateFindFreeSpot(EXTengine *eng) {
    NAS *nas;
    EXTengine *other;
    s32 again;

    do {
        again = 0;
        nas = HeadOfSolids;
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        while (nas != NULL) {
            if (nas->Type == CrateAstType) {
                other = nas->Engine;
                if (other != NULL && other->unk10.x == eng->unk10.x && other->unk10.z == eng->unk10.z) {
                    eng->unk10.x = Random(0xC00) - 0x600;
                    eng->unk10.z = Random(0xC00) - 0x600;
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

void SetPAHeroHit(s32 arg0) {
    HeroInfo *b = Heroes;
    HeroInfo *h = &b[arg0];

    if (h->hero_pu != 0x12) {
        h->hero_hit = 0x1E;
    }
    h->hero_move->velocity_enable = 0;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    PlaySfx(HeroHitSfx[h->hero_name][Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
    SetHeroSeq(arg0, 9);
}

void PAHeroHit(u32 arg0) {
    HeroInfo *b = Heroes;
    HeroInfo *h = &b[arg0];
    s32 sfx;

    if (h->hero_move->reactive_speed != 0) {
        HeroHooks.HeroKickCollision();
    }
    switch (ProcessHeroAnim(arg0)) {
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
        if (Heroes[arg0].hero_health <= 0 && !(GameCheats & 2)) {
            Heroes[arg0].hero_immunity = 0;
            Heroes[arg0].hero_hit += 0x28;
            SetHeroSeq(arg0, 0x18);
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
    HeroHooks.HeroKickCollision(arg0);
    HeroTestFall(arg0);
}

void PAHeroPickup(s32 arg0) {
    VECTOR pos;
    HeroInfo *h;
    subengine *s;
    PapuWeaponVec *w;
    HeroInfo *b;
    s16 t;
    PapuWeaponVec *tbl;

    ProcessHeroAnim();
    b = Heroes;
    h = &b[arg0];
    s = h->hero_fx->sub;
    if (s != NULL && sub_find_point(h->hero_eng, h->hero_sub, 0x40, &pos) != NULL) {
        h->hero_fx->unk10 = h->hero_eng->unk10;
        t = h->hero_weapon_timer;
        if (t >= 0) {
            tbl = PAHeroThrowVecs;
            w = &tbl[t];
            if (w->a == 0) {
                goto clear;
            }
            s->unk20.x = w->a;
            s->unk20.y = w->b;
            s->unk20.x = w->c;
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
    HeroTestFall(arg0);
}

void PAHeroHold(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    subengine *s;
    HeroInfo *self;
    HeroInfo *base2;
    EXTobject *o;
    s32 v;
    TrigEntry *te;
    VECTOR pos;
    int new_var;

    if (HeroCheckResult(arg0) == 0) {
        base = Heroes;
        h = &base[arg0];
        o = h->hero_object;
        if (o != NULL) {
        if (o->status != 2) {
            h->hero_object = NULL;
            if (h->hero_eng->sfx != NULL) {
                StopSfx(h->hero_eng->sfx);
            }
            goto rest;
        }
        if ((o->can_do & 4) && (((PadButtons *)Pads)[arg0].unk50 & 0x2000)) {
            SetHeroAction(arg0, (u16)HeroActThrow);
            return;
        }
        goto rest;
        }
        SetHeroAction(arg0, 0);
        return;
    rest:
        {
        if (HeroTestMove(arg0) == 0) {
            HeroHooks.HeroKickCollision(arg0);
            ProcessHeroAnim(arg0);
            base2 = Heroes;
            self = &base2[arg0];
            s = self->hero_fx->sub;
            if (s != NULL && sub_find_point(self->hero_eng, self->hero_sub, 0x40, &pos) != NULL) {
                self->hero_fx->unk10 = self->hero_eng->unk10;
                v = (rcossin_tbl[(((GameFrameCount << 8) & 0xFFF) - 0x400) & 0xFFF].unk2 * 512 >> 12) + 0x1000;
                s->unk20.y = s->unk20.x = v;
                s->unk04 = pos.vx - self->hero_eng->unk10.x;
                s->unk08 = pos.vy - self->hero_eng->unk10.y;
                new_var = pos.vz - self->hero_eng->unk10.z;
                s->flags |= 0x8000;
                s->unk52 = 1;
                s->unk0C = new_var;
            }
            HeroTestFall(arg0);
        }
        }
    }
}

void PAHeroCarry(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    subengine *s;
    HeroInfo *self;
    HeroInfo *base2;
    EXTobject *o;
    s32 v;
    TrigEntry *te;
    VECTOR pos;
    int new_var;

    if (HeroCheckResult(arg0) == 0) {
        base = Heroes;
        h = &base[arg0];
        o = h->hero_object;
        if (o != NULL) {
        if (o->status != 2) {
            h->hero_object = NULL;
            if (h->hero_eng->sfx != NULL) {
                StopSfx(h->hero_eng->sfx);
            }
            goto rest;
        }
        if ((o->can_do & 4) && (((PadButtons *)Pads)[arg0].unk50 & 0x2000)) {
            SetHeroAction(arg0, (u16)HeroActThrow);
            return;
        }
        goto rest;
        }
        SetHeroAction(arg0, 0);
        return;
    rest:
        {
        if (HeroTestStop(arg0) == 0) {
            HeroHooks.HeroKickCollision(arg0);
            ProcessHeroAnim(arg0);
            base2 = Heroes;
            self = &base2[arg0];
            s = self->hero_fx->sub;
            if (s != NULL && sub_find_point(self->hero_eng, self->hero_sub, 0x40, &pos) != NULL) {
                self->hero_fx->unk10 = self->hero_eng->unk10;
                v = (rcossin_tbl[(((GameFrameCount << 8) & 0xFFF) - 0x400) & 0xFFF].unk2 * 512 >> 12) + 0x1000;
                s->unk20.y = s->unk20.x = v;
                s->unk04 = pos.vx - self->hero_eng->unk10.x;
                s->unk08 = pos.vy - self->hero_eng->unk10.y;
                new_var = pos.vz - self->hero_eng->unk10.z;
                s->flags |= 0x8000;
                s->unk52 = 1;
                s->unk0C = new_var;
            }
            HeroTestFall(arg0);
        }
        }
    }
}

void PAHeroThrow(s32 arg0) {
    VECTOR pos;
    HeroInfo *h;
    subengine *s;
    s16 *w;
    HeroInfo *b;
    HeroInfo *h0;
    HeroInfo *b0;
    s16 (*tbl)[3];
    s16 t;

    if (ProcessHeroAnim() == 0x13) {
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
    if (s != NULL && sub_find_point(h->hero_eng, h->hero_sub, 0x40, &pos) != NULL) {
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
    HeroTestFall(arg0);
}

void SetPAHeroDead(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base2;

    if (Heroes[arg0].hero_pu == 0x14) {
        if (PapuStraySfx != NULL) {
            StopSfx(PapuStraySfx);
        }
    }
    base2 = Heroes;
    h = base2 + arg0;
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
    HeroBecomeAngel(arg0);
}

void SetPAHeroWinner(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h2;
    s32 dy;

    if (Heroes[arg0].hero_pu == 0x14 && PapuStraySfx != NULL) {
        StopSfx(PapuStraySfx);
    }
    base = Heroes;
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
    h->hero_sub->u6C.dbase = &HeroModelBank[HeroHooks.HeroModels[h->hero_name]];
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))ApplyContour)(h->hero_eng);
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
    RequestHeroAnim(arg0, 0);
    SetHeroSeq(arg0, 0xC);
    StartHeroVictory(arg0);
}

void PAHeroWinner(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))ApplyContour)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    CheckHeroAnim(arg0);
    if (UpdateHeroVictory(arg0) != 0) {
        FinishLevel();
    }
    ProcessHeroAnim(arg0);
}

/* The HeroHooks.Hero2ObjectCollision hook: claim the first object in reach of player's
 * cylinder that can take mask (within cone of the hero's heading, 0: any).
 * Each arm claims in its own do-while (0); jump2 merges the copies. */
NAS *PapuHero2ObjectCollision(player, mask, unused, cone)
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
    rec = FindCollisionBox(me, 0x400);
    if (rec == NULL) {
        return NULL;
    }
    if (me->unk5E != 0) {
        rotscale_update(&me->unk1C);
    }
    gfx_load_xform(me->sub, me->unk3C, &me->unk10);
    bot = me->unk10.y + rec->unk2;
    top = me->unk10.y + rec->height;
    rad = rec->radius;
    gte_rotate_point((SVECTOR *)rec, &pos);
    nas = HeadOfSolids;
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
        rec2 = FindCollisionBox(eng, 0x40);
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
                    } else if (angle_diff_abs(Heroes[player].hero_move->forward_angle,
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

NAS *PapuHeroGrabObject(idx, arg1)
    s32 idx;
    u16 arg1;
{
    NAS *n;
    s32 idx2 = idx;
    s32 lim = 0x10000;
    EXTengine *he;
    EXTengine *e;
    u8 *fux;
    u16 ang;
    s32 hx, hz, dx, dz;

    n = HeadOfObjects;
    ang = Heroes[idx2].hero_move->facing_angle;
    he = Heroes[idx2].hero_eng;
    hx = he->unk10.x - (rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    hz = he->unk10.z - (rcossin_tbl[ang & 0xFFF].unk2 * 1024 >> 14);
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
        if (arg1 == 0 ||
            angle_diff_abs(Heroes[idx2].hero_move->forward_angle,
                          get_angle(e->unk10.x - he->unk10.x, e->unk10.z - he->unk10.z) ^ 0x800) < arg1) {
            do {
                *(s16 *)(fux + 0x16) = idx;
                *(u16 *)(fux + 0x1E) |= 1;
                return n;
            } while (0);
        }
    }
    return NULL;
}

s32 PapuHeroKickCollision(s32 arg0) {
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
    PapuHitFlags *t;
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
        box = FindCollisionBox(eng, 0x40);
        if (box != NULL) {
            rad = box->radius;
            hi = eng->unk10.y + box->unk2;
            lo = eng->unk10.y + box->height;
            box = FindCollisionBox(eng, 1);
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
                        rec = FindCollisionBox(e, 0x40);
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
                                                (*(void (**)(s32, s32))&HeroHooks.HeroesTouched)(arg0, i);
                                            }
                                            if (other[i].hero_pu != 0) {
                                                (*(void (**)(s32, s32))&HeroHooks.HeroesTouched)(i, arg0);
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
                rec = FindCollisionBox(e, 0x40);
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

s32 PapuHeroCarryCollision(s32 idx) {
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
    PapuPushExt *ext;
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
        box = FindCollisionBox(eng, 0x40);
        if (box != NULL) {
            bot = eng->unk10.y + box->unk2;
            top = eng->unk10.y + box->height;
            box = FindCollisionBox(eng, 1);
            if (box != NULL) {
                if (eng->unk5E != 0) {
                    rotscale_update(&eng->unk1C);
                    sub = eng->sub;
                    eng->sub = sub;
                    gfx_load_xform(sub, eng->unk3C, &eng->unk10);
                } else {
                    sub = eng->sub;
                    eng->sub = sub;
                    gfx_load_xform(sub, eng->unk3C, &eng->unk10);
                }
                gte_rotate_point((SVECTOR *)box, &v);
            }
            nas = HeadOfSolids;
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
                            if (ext != NULL && (ext->unk1A == 1 || ext->unk1A == 0xB)) {
                                rec = FindCollisionBox(other, 0x40);
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
                                                eng->unk10.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                eng->unk10.z -= (d * rcossin_tbl[ang].unk2) >> 12;
                                                hit = 1;
                                                if (ext->unk1C & 8) {
                                                    ext->unk16 = idx;
                                                    ext->unk1E |= 8;
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

void *PapuHeroObjectAhead(s32 idx, s32 mask) {
    NAS *n;
    EXTengine *he;
    EXTengine *e;
    EXT *fux;
    s32 hx, hz, dx, dz, lim, m;

    n = HeadOfObjects;
    m = mask;
    lim = 0x10000;
    he = Heroes[idx].hero_eng;
    hx = he->unk10.x - (rcossin_tbl[(he->unk1E - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    hz = he->unk10.z - (rcossin_tbl[he->unk1E & 0xFFF].unk2 * 1024 >> 14);
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

void PAHeroDead(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    if (HeroCheckResult(arg0) != 0) {
        base = Heroes;
        h = base + arg0;
        if ((u16)(h->hero_name - 1) < 8) {
            h->hero_sub->u6C.dbase = &HeroModelBank[HeroHooks.HeroModels[h->hero_name]];
        } else {
            h->hero_sub->u6C.dbase = NULL;
        }
    } else {
        HeroGhost(arg0);
    }
}

void SetPAHeroThrow(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    h->hero_move->velocity_enable = 0;
    SetHeroSeq(arg0, 0x17);
    if (h->hero_object != NULL) {
        h->hero_object->can_do |= 0x20;
    }
    h->hero_weapon_timer = 0;
}

void SetPAHeroCarry(u32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x16);
}

void SetPAHeroHold(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x15);
}

void SetPAHeroPickup(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    h->hero_move->velocity_enable = 0;
    SetHeroSeq(arg0, 0x14);
    h->hero_weapon_timer = 0;
}
