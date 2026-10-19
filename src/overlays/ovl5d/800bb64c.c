#include "common.h"
#include "bash/ovl5d.h"
#include "bash/ovl5d_800bb64c.h"
#include "bash/bash.h"
#include "bash/xtras.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BB64C);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BB7E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BBA14);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BBD48);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BC038);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", InitCratePickup);

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
        func_80025B10(eng, 0x50);
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", InitBootsPickup);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", InitAnvilPickup);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", InitSlowmoPickup);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BE9D0);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BEF00);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BF404);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BF708);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BF928);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bb64c", func_800BFD2C);
