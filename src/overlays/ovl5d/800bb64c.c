#include "common.h"
#include "bash/ovl5d.h"
#include "bash/ovl5d_800bb64c.h"
#include "bash/bash.h"
#include "bash/xtras.h"

void func_800BB64C(T_BB64C_A *a, T_BB64C_B *b) {
    s32 z;
    s32 x;
    s32 sum = 0;
    s32 v = b->unkE;
    s32 d = b->unk10 - b->unk12;
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
    x = a->x;
    mx = sum * D_80068BD4[(a->ang - 0x400) & 0xFFF].unk2;
    mz = sum * D_80068BD4[(u16)a->ang & 0xFFF].unk2;
    z = a->z;
    x -= mx >> 12;
    z -= mz >> 12;
    x &= ~0xFF;
    x += 0x80;
    z &= ~0xFF;
    z += 0x80;
    b->unk16 = x;
    b->unk2A = z;
    b->dir = D_80051244[((u16)a->ang >> 7) & 0x1C];
    x -= a->x;
    z -= a->z;
    sum = csqrt(x * x + z * z) >> 6;
    if (sum > 0) {
        v -= d;
        do {
            sum -= v;
            v -= d;
        } while (sum > 0);
        v += d;
    }
    v -= d >> 1;
    b->unkE = v;
}

s32 func_800BB7B8(T_BB7B8 *p) {
    s32 a, b, c, d;
    b = (p->x >> 8) + 8;
    a = 7 - (p->y >> 8);
    c = (s8)(p->h >> 8);
    d = c << 16;
    if (!(((u16)p->h) >> 16)) {
        return c;
    }
    return a + b + d;
}

void func_800BB7E8(void) {
    s32 i;
    s32 x, z;
    u16 t;
    u8 c;
    u16 *p, *q;
    s32 j;
    NAS *n;
    EXTengine *e;
    T_BB7E8Fux *f;

    i = 12;
    p = D_8009E46C;
    p += 12;
    do {
        *p = 0;
        i--;
        p--;
    } while (i >= 0);
    for (j = 12; j >= 0; j--) {
        D_8009E25C[j] = 0;
    }
    n = D_8005AA24;
    for (;;) {
        if (n == NULL) return;
        e = n->Engine;
        if (e != NULL) {
        f = (T_BB7E8Fux *)n->FUX;
        if (f != NULL) {
            t = f->type;
            x = (e->unk10.x >> 8) + 10;
            z = 9 - (e->unk10.z >> 8);
            if (x >= 20) x = 19;
            if (x < 0) x = 0;
            if (z >= 20) z = 19;
            if (z < 0) z = 0;
            D_8009D90C[z * 20 + x].unk0 = t;
            if ((u16)(f->type - 1) < 12) {
                D_8009E46C[0]++;
                D_8009E46C[f->type]++;
            } else if ((u16)(f->type - 0x11) < 12) {
                D_8009E25C[0]++;
                D_8009E25C[f->type - 0x10]++;
                if ((u16)(f->type - 0x12) < 4) {
                    D_8009E46C[1]++;
                }
            }
        } else {
            c = 0xFF;
            x = (e->unk10.x >> 8) + 10;
            z = 9 - (e->unk10.z >> 8);
            if (x >= 20) x = 19;
            if (x < 0) x = 0;
            if (z >= 20) z = 19;
            if (z < 0) z = 0;
            D_8009D90C[z * 20 + x].unk0 = c;
        }
        }
        n = n->Object.Child;
    }
}

u8 *func_800BBA14(s32 a, s32 b, s32 c, s32 d, s32 e) {
    NAS *nas;
    T_BBD48Obj *obj;
    EXTengine *eng;
    s32 x, z;
    u16 k;
    s32 t;

    nas = SpawnNASById(a, b, c, 0x202, 0x11);
    k = d;
    if (nas != NULL) {
        eng = nas->Engine;
        if (eng != NULL) {
            obj = (T_BBD48Obj *)AddObject(nas);
            if (obj != NULL && AddMotion(nas) != NULL) {
                func_80026AD8((u8 *)nas);
                func_8002694C(nas, 0x300);
                nas->FUX = (EXT *)obj;
                eng->sub->unk6C = D_80052F94;
                eng->unk5D = 1;
                obj->unk14 = d;
                D_8009E46C[obj->unk14]++;
                if ((s16)e >= 0) {
                    obj->unk1A = e;
                } else {
                    obj->unk1A = 6;
                }
                switch (k) {
                case 3:
                    eng->sub->unk74 = 0x2008;
                    break;
                case 1:
                    eng->sub->unk74 = 0x2014;
                    break;
                case 2:
                    eng->sub->unk74 = 0x2014;
                    break;
                case 5:
                    eng->sub->unk74 = 0x2009;
                    obj->unk22 = 0x5A;
                    obj->unk1C |= 0x100;
                    break;
                case 6:
                    eng->sub->unk74 = 0x2001;
                    obj->unk1C |= 0x100;
                    break;
                default:
                    eng->sub->unk74 = 0x2013;
                    break;
                }
                switch ((u16)obj->unk1A) {
                case 3:
                    eng->unk10.y = D_8005A5D8;
                    break;
                case 7:
                    eng->unk2C.x = 0x40;
                    eng->unk2C.y = 0x40;
                    eng->unk2C.z = 0x40;
                    eng->unk5D = 1;
                    eng->unk5C = 1;
                    eng->unk5E = 1;
                    eng->flags = (eng->flags & ~0x8000) | 1;
                    eng->unk10.y = D_8005A5D8;
                    PlaySfx(0x3FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    break;
                case 6:
                    eng->unk5D = 1;
                    eng->unk5C = 1;
                    eng->unk5E = 1;
                    eng->unk2C.y = 0;
                    eng->flags &= ~0x8000;
                    eng->unk10.y = D_8005A5D8;
                    break;
                }
                t = k;
                x = (eng->unk10.x >> 8) + 10;
                z = 9 - (eng->unk10.z >> 8);
                if (x >= 20) x = 19;
                if (x < 0) x = 0;
                if (z >= 20) z = 19;
                if (z < 0) z = 0;
                D_8009D90C[z * 20 + x].unk0 = t;
            }
        }
        if (nas->Failure != 0) {
            KillNAS(nas);
            return NULL;
        }
    }
    return (u8 *)nas;
}

void func_800BBD48(NAS *arg0) {
    EXTengine *eng;
    T_BBD48Obj *obj;
    NAS *nas;
    EXTengine *other;
    s32 again;
    s32 x, z;

    eng = arg0->Engine;
    if (eng != NULL) {
        obj = (T_BBD48Obj *)AddObject(arg0);
        if (obj != NULL) {
            arg0->FUX = (EXT *)obj;
            do {
                again = 0;
                nas = D_8005AA2C;
                eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
                eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
                while (nas != NULL) {
                    if (nas->Type == D_8009E464) {
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
            eng->unk10.y = D_8005A5D8;
            eng->sub->unk6C = D_80052F94;
            eng->unk5D = 1;
            eng->unk5C = 1;
            obj->unk1A = 3;
            if (arg0->trigger != NULL) {
                obj->unk14 = arg0->trigger->InfoStruct->val1;
            }
            D_8009E46C[obj->unk14]++;
            switch (obj->unk14) {
            case 3:
                eng->sub->unk74 = 0x2008;
                break;
            case 1:
                eng->sub->unk74 = 0x2014;
                break;
            case 2:
                eng->sub->unk74 = 0x2014;
                break;
            case 5:
                eng->sub->unk74 = 0x2009;
                obj->unk22 = 0x5A;
                obj->unk1C |= 0x100;
                break;
            case 6:
                eng->sub->unk74 = 0x2001;
                obj->unk1C |= 0x100;
                break;
            default:
                eng->sub->unk74 = 0x2013;
                break;
            }
            x = (eng->unk10.x >> 8) + 10;
            z = 9 - (eng->unk10.z >> 8);
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
            D_8009D90C[z * 20 + x].unk0 = obj->unk14;
            AddMotion(arg0);
            func_80026AD8((u8 *)arg0);
            func_8002694C(arg0, 0x300);
        }
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BC038);

void InitCratePickup(NAS *arg0) {
    EXTengine *eng;
    EXTobject *obj;
    SLONG_XYZ *pos;
    u16 id;
    s32 x, z;
    s32 r;

    eng = arg0->Engine;
    if (eng != NULL) {
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        eng->sub->unk6C = D_80052F5C;
        r = Random(4);
        eng->sub->unk74 = D_800C46E4[r];
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            func_80026AD8((u8 *)arg0);
            func_8002694C(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = r + 0x12;
            obj->countdown = 0x78;
            if (eng->unk10.y == D_8005A5D8) {
                obj->status = 0;
            } else {
                obj->status = 4;
                func_8008DDC0(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->unk10;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj->whatami;
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
        D_8009D90C[z * 0x14 + x].unk0 = id;
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
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0x200D;
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            func_80026AD8((u8 *)arg0);
            func_8002694C(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = 0x12;
            obj->countdown = 0x78;
            if (eng->unk10.y == D_8005A5D8) {
                obj->status = 0;
            } else {
                obj->status = 4;
                func_8008DDC0(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->unk10;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj->whatami;
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
        D_8009D90C[z * 0x14 + x].unk0 = id;
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
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0x200A;
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            func_80026AD8((u8 *)arg0);
            func_8002694C(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = 0x13;
            obj->countdown = 0x78;
            if (eng->unk10.y == D_8005A5D8) {
                obj->status = 0;
            } else {
                obj->status = 4;
                func_8008DDC0(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->unk10;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj->whatami;
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
        D_8009D90C[z * 0x14 + x].unk0 = id;
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
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0x2011;
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            func_80026AD8((u8 *)arg0);
            func_8002694C(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = 0x14;
            obj->countdown = 0x78;
            if (eng->unk10.y == D_8005A5D8) {
                obj->status = 0;
            } else {
                obj->status = 4;
                func_8008DDC0(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->unk10;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj->whatami;
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
        D_8009D90C[z * 0x14 + x].unk0 = id;
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
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0x2009;
        obj = (EXTobject *)AddObject(arg0);
        if (obj != NULL) {
            func_80026AD8((u8 *)arg0);
            func_8002694C(arg0, 0x200);
            arg0->FUX = (EXT *)obj;
            obj->whatami = 0x15;
            obj->countdown = 0x78;
            if (eng->unk10.y == D_8005A5D8) {
                obj->status = 0;
            } else {
                obj->status = 4;
                func_8008DDC0(arg0);
            }
        }
        FetchDefaultColBox(eng, 0x50);
        pos = &eng->unk10;
        x = (pos->x >> 8) + 10;
        z = 9 - (pos->z >> 8);
        id = obj->whatami;
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
        D_8009D90C[z * 0x14 + x].unk0 = id;
        PlaySfx(0x41F, NULL, pos, 0x1000, 0x1E00, 0);
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void func_800BE9D0(NAS *arg0) {
    EXTengine *eng;
    EXTobject *obj;
    HeroInfo *h;

    eng = arg0->Engine;
    if (eng == NULL) {
        return;
    }
    eng->unk5E = 1;
    eng->unk1E += 0x71;
    obj = (EXTobject *)arg0->FUX;
    if (obj == NULL) {
        return;
    }
    switch (obj->status) {
    case 4:
    case 5:
        if (func_8007A26C(arg0) != 0) {
            if (obj->status == 4) {
                PlaySfx(0x420, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
            obj->status = 1;
            obj->can_do |= 0x18;
        }
        return;
    }
    func_8008D6C8(arg0);
    if (obj->will_do & 8) {
        obj->will_do &= ~8;
        if (obj->player != -1 && obj->player < 5) {
            Heroes[obj->player].hero_pu = obj->whatami;
            Heroes[obj->player].hero_pu_timer = 0xB4;
            Heroes[obj->player].hero_powerup->sub->unk74 = eng->sub->unk74;
            switch (obj->whatami) {
            case 0x12:
                Heroes[obj->player].hero_pu_timer = 0x12C;
                Heroes[obj->player].hero_powerup->sub->unk74 = 0x200D;
                PlaySfx(0x41B, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                h = &Heroes[obj->player];
                h->hero_harm_Nas = NULL;
                h->hero_harm_player = -1;
                h->hero_harm_value = 0;
                h->hero_harm_flags = 0;
                break;
            case 0x13:
                Heroes[obj->player].hero_pu_timer = 0xF0;
                Heroes[obj->player].hero_powerup->sub->unk74 = eng->sub->unk74;
                PlaySfx(0x41C, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 0x14:
                Heroes[obj->player].hero_pu_timer = 0xF0;
                Heroes[obj->player].hero_powerup->sub->unk74 = 0x2011;
                PlaySfx(0x452, &D_800C4780, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x41A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            default:
                Heroes[obj->player].hero_pu_timer = 0xF0;
                Heroes[obj->player].hero_powerup->sub->unk74 = eng->sub->unk74;
                PlaySfx(0x41D, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            }
        }
    } else if (obj->countdown != 0) {
        obj->countdown--;
        return;
    }
    SpawnNASById(eng->unk10.x, eng->unk10.y - 0x100, eng->unk10.z, 0x207, 0x11);
    KillNAS(arg0);
}

void func_800BEDE8(s32 arg0, s32 arg1) {
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
                    b->hero_powerup->sub->unk74 = a->hero_powerup->sub->unk74;
                    PlaySfx(0x454, 0, 0, 0x1000, 0x1E00, 0);
                }
                break;
            }
        }
    }
}

void func_800BEF00(s32 arg0) {
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
            case 8:
            default:
                y = -0x280;
                break;
            }
        }
        pu->unk10 = Heroes[arg0].hero_eng->unk10;
        sub->unk08 = y;
        switch (Heroes[arg0].hero_pu) {
        case 0x1B:
            sub->unk08 -= 0x60;
            /* fallthrough */
        case 0x19:
        case 0x1C:
            b2 = Heroes;
            h2 = b2 + arg0;
            if (h2->hero_weapon_count != 0) {
                sub->flags |= 0x8000;
                pu->unk10 = h2->hero_eng->unk10;
                pu->unk1E += 0xAA;
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
                pu->unk10 = h4->hero_eng->unk10;
                do {
                    pu->unk1E += 0xAA;
                } while (0);
                if (D_800C4780 != NULL) {
                    t = h4->hero_pu_timer;
                    if (t >= 0x12C) {
                        t = 0x12B;
                    }
                    func_800228A4(D_800C4780, (0x12C - t) * 0xF00 / 300 + 0x1E00);
                }
            } else {
                if (D_800C4780 != NULL) {
                    StopSfx(D_800C4780);
                }
                h4->hero_pu = 0;
                SetHeroAction((HeroInfo *)arg0, 0x12);
                pu->unk1E = 0;
                sub->unk08 = 0;
            }
            break;
        case 0x12:
            b5 = Heroes;
            h5 = b5 + arg0;
            if (h5->hero_pu_timer != 0) {
                sub->flags |= 0x8000;
                pu->unk10 = h5->hero_eng->unk10;
                pu->unk1E += 0xAA;
                sub->unk0C = sub->unk08 >> 1;
            } else {
                h5->hero_pu = 0;
                pu->unk1E = 0;
                sub->unk04 = 0;
                sub->unk08 = 0;
                sub->unk0C = 0;
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
            pu->unk10 = Heroes[arg0].hero_eng->unk10;
            pu->unk1E += 0xAA;
            break;
        }
        pu->unk5E = 1;
    }
}

s32 func_800BF404(EXTengine *eng, s32 arg1, s32 who, s32 heroes) {
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

    box = func_80025A48(eng, 0x40);
    if (box != NULL) {
        r = box->radius;
        top = eng->unk10.y + box->unk2;
        bot = eng->unk10.y + box->height;
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
                rec = func_80025A48(e, 0x40);
                if (rec == NULL) {
                    continue;
                }
                lo = e->unk10.y + rec->unk2;
                hi = e->unk10.y + rec->height;
                if (bot < lo && hi < top) {
                    dx = e->unk10.x - eng->unk10.x;
                    dz = dx * dx;
                    dx = e->unk10.z - eng->unk10.z;
                    d = dz + dx * dx;
                    rr = rec->radius + r;
                    rr = rr * rr;
                    if (d < rr) {
                        return (s32)Heroes[i].hero;
                    }
                }
            }
        }
        for (nas = D_8005AA2C; nas != NULL; nas = nas->Solid.Child) {
            if (nas->Class & 0x200) {
                e = nas->Engine;
                if (e != NULL && e != eng) {
                    rec = func_80025A48(e, 0x40);
                    if (rec != NULL) {
                        lo = e->unk10.y + rec->unk2;
                        hi = e->unk10.y + rec->height;
                        if (bot < lo && hi < top) {
                            dx = e->unk10.x - eng->unk10.x;
                            dz = dx * dx;
                            dx = e->unk10.z - eng->unk10.z;
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
                                    return (s32)nas;
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

NAS *func_800BF708(EXTengine *eng, s32 who) {
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

    box = func_80025A48(eng, 0x40);
    if (box != NULL) {
        r = box->radius;
        top = eng->unk10.y + box->unk2;
        bot = eng->unk10.y + box->height;
        for (nas = D_8005AA2C; nas != NULL; nas = nas->Solid.Child) {
            if (nas->Class & 0x200) {
                e = nas->Engine;
                if (e != NULL && e != eng) {
                    rec = func_80025A48(e, 0x40);
                    if (rec != NULL) {
                        lo = e->unk10.y + rec->unk2;
                        hi = e->unk10.y + rec->height;
                        if (bot < lo && hi < top) {
                            dx = e->unk10.x - eng->unk10.x;
                            dz = e->unk10.z - eng->unk10.z;
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

s32 func_800BF928(EXTengine *eng, HitFlags *flags, u32 mask) {
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

    rec =func_80025A48(eng, 0x40);
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
            rec2 = func_80025A48(other, 0x40);
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

void func_800BFAC0(EXTengine *eng) {
    PlaySfx(((HitSfxTable *)D_8009E424)->crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
    func_8007A524(2, (s32 *)((u8 *)eng + 0x10), (s32)D_80052F94);
    if ((D_8009D750(eng) ^ 1) != 0) {
        SpawnNASById(((s32 *)eng)[4], ((s32 *)eng)[5], ((s32 *)eng)[6], 0x206, 0x11);
    }
}

s32 func_800BFB6C(s32 pos, ExtCurve *c) {
    s32 sum = 0;
    s32 a = c->unkE;
    s32 b = c->unk18;
    s32 delta = c->unk1A - c->unk1C;
    s32 dir = c->unkC;
    s32 step = c->unk10 - c->unk12;
    s32 max1 = c->unk14;
    s32 max2 = c->unk1E;
    s32 n = 0;

    if (pos < D_8005A5D8) {
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
                delta = c->unk1C;
                dir = 0;
            }
            n++;
        } while (pos < D_8005A5D8);
    }
    c->unk30 = sum;
    c->unk32 = n;
    return sum;
}

void func_800BFC30(EXTengine *eng) {
    NAS *nas;
    EXTengine *other;
    s32 again;

    do {
        again = 0;
        nas = D_8005AA2C;
        eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
        eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
        while (nas != NULL) {
            if (nas->Type == D_8009E464) {
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

void func_800BFD2C(s32 arg0) {
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
    if (h2->hero_health <= 0 && !(D_8005A618 & 2)) {
        h2->hero_status |= 0x2000;
    }
    b3 = Heroes;
    h3 = &b3[arg0];
    if (h3->hero_eng->sfx != NULL) {
        StopSfx(h3->hero_eng->sfx);
    }
    PlaySfx(D_8009E080[h3->hero_name][Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
    SetHeroSeq((HeroInfo *)arg0, 9);
}
