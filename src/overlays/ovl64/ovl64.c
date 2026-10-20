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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B3938);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B6C50);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B8B5C);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800B9804);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BB560);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BB5F0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BB648);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl64/ovl64", func_800BB6B4);
