#include "common.h"
#include "bash/main.h"

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010000);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010158);

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004C56C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800101E0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010278);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800102F8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010354);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010394);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010410);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010488);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800107C8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001097C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010AE8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010C70);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010E8C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800110A8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800112A4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011410);

void func_80011474(void) {
    func_80011410(D_8004E0F0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011498);

s32 func_8001151C(u8 *arg0) {
    return ~(*(s32*)((u8*)(arg0) + -20)) == 0;
}

void func_80011530(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + -20)) = -1;
    (*(s16*)((u8*)(arg0) + -24)) = 4;
}

void func_80011544(u8 *arg0, u8 **arg1) {
    u8 **temp_v1;
    u8 *temp_a2;

    temp_a2 = arg0 - 0x18;
    temp_v1 = (*(s32*)((u8*)(temp_a2) + 4));
    if (temp_v1 != (u8 **)-1) {
        *temp_v1 = NULL;
    }
    (*(s16*)((u8*)(arg0) + -24)) = 0;
    (*(s32*)((u8*)(temp_a2) + 4)) = arg1;
    *arg1 = arg0;
}

s16 func_8001156C(u8 *arg0) {
    return (*(s16*)((u8*)(arg0) + -24));
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011578);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800115D8);

u8 *func_80011654(s32 arg0, u8 *arg1) {
    u16 *var_a0_2;
    u8 *temp_v1;
    u8 *var_a0;
    u8 *var_v1;

    func_80010AE8(arg1);
    var_v1 = func_80010C70(arg0, arg1);
    var_a0 = var_v1 + 0x18;
    if (var_v1 == NULL) {
        temp_v1 = (*(s32*)((u8*)(arg1) + 12));
        var_a0_2 = NULL;
        if ((temp_v1 != NULL) && ((var_a0_2 = temp_v1 + (((*(s32*)((u8*)(temp_v1) + 8)) * 4) + 0x18), ((*(s32*)((u8*)(var_a0_2) + 4)) == -1)) || ((*(s32*)((u8*)(var_a0_2) + 8)) == 0))) {
            var_a0_2 = NULL;
        }
        if ((var_a0_2 != NULL) && (*var_a0_2 & 2)) {
            var_a0_2 = NULL;
        }
        func_800107C8(var_a0_2, arg1);
        var_v1 = func_80010C70(arg0, arg1);
        var_a0 = var_v1 + 0x18;
        if (var_v1 == NULL) {
            func_80011474();
            var_a0 = NULL;
        } else {
            goto block_10;
        }
    } else {
block_10:
        (*(s32*)((u8*)(var_v1) + 4)) = -1;
        (*(u16*)((u8*)(var_v1) + 0)) = 0;
    }
    return var_a0;
}

u8 *func_80011748(s32 arg0, u8 *arg1) {
    u16 *var_a0_2;
    u8 *temp_v1;
    u8 *var_a0;
    u8 *var_v0;

    var_a0 = func_80010C70();
    var_v0 = var_a0 + 0x18;
    if (var_a0 == NULL) {
        temp_v1 = (*(s32*)((u8*)(arg1) + 12));
        var_a0_2 = NULL;
        if ((temp_v1 != NULL) && ((var_a0_2 = temp_v1 + (((*(s32*)((u8*)(temp_v1) + 8)) * 4) + 0x18), ((*(s32*)((u8*)(var_a0_2) + 4)) == -1)) || ((*(s32*)((u8*)(var_a0_2) + 8)) == 0))) {
            var_a0_2 = NULL;
        }
        if ((var_a0_2 != NULL) && (*var_a0_2 & 2)) {
            var_a0_2 = NULL;
        }
        func_800107C8(var_a0_2, arg1);
        var_a0 = func_80010C70(arg0, arg1);
        var_v0 = var_a0 + 0x18;
        if (var_a0 == NULL) {
            func_80011474();
            return NULL;
        }
        goto block_10;
    }
block_10:
    (*(s32*)((u8*)(var_a0) + 4)) = -1;
    (*(u16*)((u8*)(var_a0) + 0)) = 0;
    return var_v0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011828);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011878);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800118BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800119D4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011AE8);

void func_80011B74(u8 *arg0, u8 *arg1) {
    u8 *temp_v1;
    u8 *temp_v1_2;

    temp_v1 = (*(s32*)((u8*)(arg0) + 16));
    if (temp_v1 != NULL) {
        (*(s32*)((u8*)(temp_v1) + 20)) = (u8 *) (*(s32*)((u8*)(arg0) + 20));
    } else {
        (*(s32*)((u8*)(arg1) + 8)) = (u8 *) (*(s32*)((u8*)(arg0) + 20));
    }
    temp_v1_2 = (*(s32*)((u8*)(arg0) + 20));
    if (temp_v1_2 != NULL) {
        (*(s32*)((u8*)(temp_v1_2) + 16)) = (u8 *) (*(s32*)((u8*)(arg0) + 16));
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011BC0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011BF8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011C5C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011C98);

void func_80011D00(s32 arg0) {
    func_800107C8(arg0 - 0x18, D_8004E0F0);
}

void func_80011D28(u8 *arg0) {
    u16 *var_a0;
    u8 *temp_v1;

    temp_v1 = (*(s32*)((u8*)(arg0) + 12));
    var_a0 = NULL;
    if ((temp_v1 != NULL) && ((var_a0 = temp_v1 + (((*(s32*)((u8*)(temp_v1) + 8)) * 4) + 0x18), ((*(s32*)((u8*)(var_a0) + 4)) == -1)) || ((*(s32*)((u8*)(var_a0) + 8)) == 0))) {
        var_a0 = NULL;
    }
    if ((var_a0 != NULL) && (*var_a0 & 2)) {
        var_a0 = NULL;
    }
    func_800107C8(var_a0, arg0);
}

u8 *func_80011DB4(u8 *arg0) {
    u16 temp_v0;
    u8 *var_a0;
    u8 *var_s0;
    u8 *var_s1;

    var_s0 = NULL;
    var_a0 = (*(s32*)((u8*)(arg0) + 16));
    var_s1 = NULL;
    if (var_a0 != NULL) {
        do {
            temp_v0 = (*(u16*)((u8*)(var_a0) + 2)) - 1;
            (*(u16*)((u8*)(var_a0) + 2)) = temp_v0;
            if ((s16) temp_v0 <= 0) {
                if (var_s0 != NULL) {
                    (*(s32*)((u8*)(var_s0) + 20)) = (u8 *) (*(s32*)((u8*)(var_a0) + 20));
                } else {
                    (*(s32*)((u8*)(arg0) + 16)) = (u8 *) (*(s32*)((u8*)(var_a0) + 20));
                }
                func_800110A8(var_a0 + 0x18, D_8004E0F0);
                var_a0 = NULL;
                if (var_s0 != NULL) {
                    var_a0 = (*(s32*)((u8*)(var_s0) + 20));
                }
            } else {
                var_s1 += (s16) temp_v0;
                var_s0 = var_a0;
                var_a0 = (*(s32*)((u8*)(var_a0) + 20));
            }
        } while (var_a0 != NULL);
    }
    return var_s1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011E80);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011EF0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011F74);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011FC8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800120E8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800121F8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001231C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001251C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800125B4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012678);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012840);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800129D0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012AC4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012BC8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012CD4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012D80);

void func_80012DDC(s32 arg0) {
    do {

    } while (func_8001251C(arg0) != 0);
}

void func_80012E0C(void) {
}

void func_80012E14(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = D_80051240;
    D_80051240 = arg0;
    (*(s32*)((u8*)(arg0) + 8)) = temp_v1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012E28);

void func_80012E54(void) {
    u8 *p;
    s32 i;

    p = D_80050F40;
    D_80051240 = p;
    for (i = 62; i != -1; i--) {
        *(u8 **)(p + 8) = p + 0xC;
        p += 0xC;
    }
    *(u8 **)(p + 8) = NULL;
}

void func_80012E88(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012E90);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012F04);

void func_80012F90(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = D_80050F3C;
    D_80050F3C = arg0;
    (*(s32*)((u8*)(arg0) + 28)) = temp_v1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012FA4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012FFC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013034);

u8 *func_80013110(u8 *arg0) {
    u8 *temp_a1;
    u8 *temp_v0;
    u8 *temp_v1;

    temp_a1 = D_80050F3C;
    if (temp_a1 != NULL) {
        temp_v1 = (*(s32*)((u8*)(temp_a1) + 28));
        (*(s32*)((u8*)(temp_a1) + 28)) = NULL;
        (*(s32*)((u8*)(temp_a1) + 32)) = (u8 *) (*(s32*)((u8*)(arg0) + 16));
        temp_v0 = (*(s32*)((u8*)(arg0) + 16));
        D_80050F3C = temp_v1;
        if (temp_v0 != NULL) {
            (*(s32*)((u8*)(temp_v0) + 28)) = temp_a1;
        } else {
            (*(s32*)((u8*)(arg0) + 12)) = temp_a1;
        }
        (*(s32*)((u8*)(arg0) + 16)) = temp_a1;
    }
    return temp_a1;
}

s32 func_80013160(u8 *arg0) {
    return (*(s32*)((u8*)(arg0) + 12));
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001316C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800131B0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013238);

void func_8001325C(void) {
    u8 *p;
    s32 i;

    p = D_8005063C;
    D_80050F3C = p;
    for (i = 62; i != -1; i--) {
        *(u8 **)(p + 0x1C) = p + 0x24;
        p += 0x24;
    }
    *(u8 **)(p + 0x1C) = NULL;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013290);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013334);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800133C8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800133FC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013488);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800134FC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800135E4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001361C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013650);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013688);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800136BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800138A4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800139A8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013B0C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013C24);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013C78);

void func_80013C94(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 48)) = arg1;
    (*(s32*)((u8*)(arg0) + 44)) = 1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013CA4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013CFC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013D54);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013F44);

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004C83C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800142D4);

void func_80014388(u8 *arg0) {
    D_800517B0 = 0;
    (*(s32*)((u8*)(arg0) + 100)) = 0x1000;
    (*(s32*)((u8*)(arg0) + 104)) = 0x1000;
    (*(s32*)((u8*)(arg0) + 108)) = 0x1000;
    (*(s32*)((u8*)(arg0) + 8)) = 0x300;
    (*(s32*)((u8*)(arg0) + 24)) = 0x140;
    (*(s16*)((u8*)(arg0) + 84)) = 0;
    (*(s16*)((u8*)(arg0) + 86)) = 0;
    (*(s16*)((u8*)(arg0) + 88)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
    (*(s32*)((u8*)(arg0) + 16)) = 0;
    (*(s32*)((u8*)(arg0) + 20)) = 0;
    (*(s8*)((u8*)(arg0) + 149)) = 0;
    (*(s8*)((u8*)(arg0) + 148)) = 1;
    (*(s32*)((u8*)(arg0) + 0)) = 0;
    (*(s32*)((u8*)(arg0) + 4)) = 0;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 60)) = 0;
    (*(s32*)((u8*)(arg0) + 56)) = 0;
    (*(s32*)((u8*)(arg0) + 64)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800143F0);

void func_8001452C(s32 arg0, s32 arg1) {
    D_800517B0 = arg1;
    D_800517B4 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80014540);

/* Angle of (x, z) in 0x1000 units per turn, from the 0x800-entry arctangent table */
s32 get_angle(s32 x, s32 z) {
    if (x == 0 && z == 0) {
        return 0;
    }
    if (x == 0x80000000) {
        x = 0x80000001;
    }
    if (z == 0x80000000) {
        z = 0x80000001;
    }
    if (x >= 0) {
        if (z >= 0) {
            if (x < z) {
                return AtanTable[(x << 11) / z];
            }
            return 0x400 - AtanTable[(z << 11) / x];
        }
        z = -z;
        if (x >= z) {
            return AtanTable[(z << 11) / x] + 0x400;
        }
        return 0x800 - AtanTable[(x << 11) / z];
    }
    x = -x;
    if (z < 0) {
        z = -z;
        if (x < z) {
            return AtanTable[(x << 11) / z] + 0x800;
        }
        return 0xC00 - AtanTable[(z << 11) / x];
    }
    if (x >= z) {
        return AtanTable[(z << 11) / x] + 0xC00;
    }
    return 0x1000 - AtanTable[(x << 11) / z];
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001492C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800150E8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001517C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015210);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015284);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800152A8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015304);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001534C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001538C);

s32 *func_800153A8(void) {
    return D_800527C4;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800153B4);

s32 func_80015480(s32 arg0, s32 arg1) {
    s32 var_a0;

    var_a0 = (arg1 - arg0) & 0xFFF;
    if (var_a0 >= 0x801) {
        var_a0 = 0x1000 - var_a0;
    }
    return var_a0;
}

s32 func_800154A0(s32 arg0, s32 arg1) {
    s32 var_a0;

    var_a0 = (arg1 - arg0) & 0xFFF;
    if (var_a0 >= 0x801) {
        var_a0 -= 0x1000;
    }
    return var_a0;
}

void func_800154C0(u16 *arg0, s32 arg1) {
    s32 i;

    for (i = arg1 - 1; i != -1; i--) {
        *arg0++ &= 0x7FFF;
    }
}

void func_800154EC(u16 *arg0, s32 arg1) {
    s32 i;

    for (i = arg1 - 1; i != -1; i--) {
        *arg0++ |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015518);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015554);

s32 Random(s32 n) {
    if (n < 2) {
        return 0;
    }
    RandomSeedA += RandomSeedB;
    RandomSeedB += RandomSeedA + ((u32)RandomSeedA >> 31);
    return (u32)RandomSeedA % n;
}

void SetRandomSeed(s32 arg0, s32 arg1) {
    RandomSeedA = arg0;
    RandomSeedB = arg1;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", func_800155FC);
#else
void func_800155FC(void) {
    RandomSeedA += RandomSeedB;
    RandomSeedB += RandomSeedA + ((u32)RandomSeedA >> 31);
}
#endif

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001562C);

void func_80015758(SVECTOR *v, VECTOR *out) {
    gte_ldv0(v);
    gte_rt();
    gte_stlvl(out);
}

FlagPoint *func_80015780(EXTengine *eng, subengine *sub, u16 flag, VECTOR *out) {
    PointList *list = func_8001562C(sub);
    FlagPoint *pts;
    FlagPoint *pt;
    FlagPoint *p;
    s32 i;
    s32 n;

    pt = list->pts;
    if (list == NULL) {
        return NULL;
    }
    n = list->count;
    pts = pt;
    for (i = 0;;) {
        if (--n == -1) {
            goto none;
        }
        p = &pts[i];
        if (p->flags & flag) {
            if (eng->unk5E != 0) {
                func_800142D4(&eng->unk1C);
            }
            func_8001CD04(sub, eng->unk3C, &eng->unk10);
            gte_ldv0(p);
            gte_rt();
            gte_stlvl(out);
            return pt;
        }
        i++;
        pt++;
    }
none:
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015870);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800158E4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015984);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800159C4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015A18);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015A78);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015AB0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015AD4);

void func_80015C1C(PointSprites *ps, MATRIX *m) {
    PointSpriteSet *set = &ps->set;
    PointSpriteSrc *src = set->src;
    TILE_1 *prim = set->prims[D_8005B690 & 1];
    s32 n;

    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
    n = set->count;
    for (;;) {
        if (n == 0) {
            return;
        }
        gte_ldv0(&src->v);
        gte_rtps();
        gte_stsxy(&prim->x0);
        src++;
        n--;
        addPrim(D_80056998.ot, prim);
        prim++;
    }
}
void func_80015D08(u8 *arg0) {
    u8 *temp_s2;

    temp_s2 = arg0 + 0x6C;
    func_80011578((*(s32*)((u8*)(temp_s2) + 4)), D_8004E0F0, 2);
    func_80011578((*(s32*)((u8*)(temp_s2) + 8)), D_8004E0F0, 2);
    func_800110A8((*(s32*)((u8*)(arg0) + 108)), D_8004E0F0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015D70);

s32 func_80015E94(u8 *arg0, u8 *arg1) {
    u8 *temp_s3;

    temp_s3 = arg1 + 4;
    (*(s32*)((u8*)(arg1) + 4)) = 0;
    (*(s32*)((u8*)(arg1) + 8)) = 0;
    (*(s32*)((u8*)(arg1) + 0)) = 0;
    (*(s32*)((u8*)(arg1) + 12)) = 0;
    func_800115D8((*(s32*)((u8*)(arg0) + 4)), temp_s3, D_8004E0F0, 2);
    if ((*(s32*)((u8*)(arg1) + 4)) != 0) {
        func_800115D8((*(s32*)((u8*)(arg0) + 4)), arg1 + 8, D_8004E0F0, 3);
        if ((*(s32*)((u8*)(arg1) + 8)) != 0) {
            func_800115D8((*(s32*)((u8*)(arg0) + 8)), arg1, D_8004E0F0, 3);
            (*(s32*)((u8*)(arg1) + 12)) = (s32) (*(s32*)((u8*)(arg0) + 0));
            return (*(s32*)((u8*)(arg1) + 0));
        }
        func_800110A8(temp_s3, D_8004E0F0);
        goto block_4;
    }
block_4:
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015F60);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800160C4);

s32 func_800160F8(u8 *arg0, s32 arg1) {
    s32 temp_a0;

    if (arg0 != NULL) {
        temp_a0 = (*(s32*)((u8*)(arg0) + 24));
        if (temp_a0 != 0) {
            return temp_a0 + ((arg1 & 0xFFF) * 0x38);
        }
    }
    return 0;
}

void func_8001612C(u8 *arg0, s32 arg1) {
    if (((*(s32*)((u8*)(arg0) + 8)) == 0) && ((*(s32*)((u8*)(arg0) + 12)) == 0)) {
        (*(s32*)((u8*)(arg0) + 52)) = arg1;
        (*(s32*)((u8*)(arg0) + 8)) = func_80013650((*(s32*)((u8*)(arg0) + 4)), func_80016188, arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016188);

void func_80016234(u8 *arg0, u8 *arg1) {
    func_80011544(arg0, arg1 + 0xC);
    func_80015F60(arg0, (*(s32*)((u8*)(arg1) + 52)));
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016274);

void func_800162E4(u8 *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_s0;

    temp_v0 = func_80012E28();
    (*(s32*)((u8*)(arg0) + 20)) = temp_v0;
    temp_s0 = (*(s32*)((u8*)(arg0) + 4)) - (*(s32*)((u8*)(arg0) + 0));
    func_800131B0(temp_s0);
    func_800115D8(temp_s0, temp_v0, D_8004E0F0, 0);
    memcpy(*temp_v0, arg1 + (*(s32*)((u8*)(arg0) + 0)), temp_s0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001636C);

void func_800163E0(u8 *arg0, s32 arg1) {
    s32 temp_a1;
    u8 *temp_s0;
    u8 *temp_v1;

    temp_v1 = (*(s32*)((u8*)(arg0) + 12));
    temp_s0 = temp_v1 + ((*(s32*)((u8*)(temp_v1) + 60)) + 0x3C) + ((arg1 * 0x10) + 4);
    if (((*(s32*)((u8*)(temp_s0) + 8)) == 0) && ((*(s32*)((u8*)(temp_s0) + 12)) == 0)) {
        temp_a1 = (*(s32*)((u8*)(temp_s0) + 0));
        (*(s32*)((u8*)(temp_s0) + 12)) = func_800133C8((*(s32*)((u8*)(arg0) + 0)), temp_a1, temp_s0 + 8, (*(s32*)((u8*)(temp_s0) + 4)) - temp_a1);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016450);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016478);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016558);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800165F8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800166FC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800167A8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001682C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016928);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016A20);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016AD4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016B48);

u8 *func_80016C18(s32 *arg0, u8 *arg1) {
    s32 temp_a1;
    s32 temp_v0_2;
    u8 *temp_v0;
    u8 *var_v0;

    var_v0 = (*(s32*)((u8*)(arg1) + 20));
    if (var_v0 == NULL) {
        temp_v0 = func_80012E28();
        if (temp_v0 != NULL) {
            temp_a1 = (*(s32*)((u8*)(arg1) + 0));
            (*(s32*)((u8*)(arg1) + 20)) = temp_v0;
            temp_v0_2 = func_800133C8(*arg0, temp_a1, temp_v0, (*(s32*)((u8*)(arg1) + 4)) - temp_a1);
            (*(s32*)((u8*)(temp_v0) + 4)) = temp_v0_2;
            if (temp_v0_2 != 0) {
                return temp_v0;
            }
            func_80012E14(temp_v0);
            goto block_5;
        }
block_5:
        var_v0 = NULL;
        /* Duplicate return node #6. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016CA0);

void func_80016D44(u8 *arg0) {
    if (((*(s32*)((u8*)(arg0) + 8)) == 0) && ((*(s32*)((u8*)(arg0) + 12)) == 0)) {
        (*(s32*)((u8*)(arg0) + 8)) = func_80013650((*(s32*)((u8*)(arg0) + 4)), func_80016D98, arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016D98);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016E5C);

void func_80016EF4(u8 *arg0) {
    s32 temp_s0;

    temp_s0 = arg0 + 0x10;
    func_8002A5A4(temp_s0);
    func_80029560(temp_s0);
    (*(s32*)((u8*)(arg0) + 32)) = 0;
}

void func_80016F34(u8 *arg0, u8 *arg1) {
    func_80011544(arg0, arg1 + 0xC);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016F54);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001702C);

void func_80017070(void) {
    func_80016CA0(D_80055684);
}

void func_80017094(u8 *arg0, s32 arg1, s32 arg2) {
    s32 temp_s0;

    temp_s0 = arg0 + 0x10;
    (*(s32*)((u8*)(arg0) + 12)) = arg1;
    (*(s32*)((u8*)(arg0) + 32)) = arg2;
    func_8002A5A4(temp_s0, arg2);
    func_80029560(temp_s0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800170D4);

void func_80017138(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
    (*(s32*)((u8*)(arg0) + 52)) = 0;
    func_8002AA8C(arg0 + 0x10);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017164);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017388);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017640);

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", func_800177F4);
#else
void func_800177F4(RelTable *face, POLY_GT4 *poly, s32 arg2, s32 mode) {
    u16 *idx;
    u32 *colors;
    u32 *c;
    s32 semi = mode & 1;
    s32 abr = mode & 0x60;
    u32 code;
    u16 tpage;

    gte_ldfcdir(D_80056998.farColor[0], D_80056998.farColor[1], D_80056998.farColor[2]);
    gte_lddp(D_80056998.dp);
    idx = (u16 *)((u8 *)&face->table + face->table);
    colors = (u32 *)((u8 *)&D_80056998.unk0->colors->table + D_80056998.unk0->colors->table);
    c = &colors[idx[0] & 0x1FFF];
    gte_ldrgb(c);
    gte_dpcs();
    gte_strgb(&poly->r0);
    c++;
    gte_ldrgb(c);
    gte_dpcs();
    gte_strgb(&poly->r1);
    c++;
    gte_ldrgb(c);
    gte_dpcs();
    gte_strgb(&poly->r2);
    setPolyGT4(poly);
    if (idx[0] & 0x8000) {
        code = *(u32 *)&poly->r0 | 0x02000000;
        tpage = (poly->tpage & ~0x60) | 0x20;
    } else {
        if (semi) {
            code = *(u32 *)&poly->r0 | 0x02000000;
            tpage = poly->tpage;
        } else {
            code = *(u32 *)&poly->r0 & ~0x02000000;
            tpage = poly->tpage;
        }
        tpage = (tpage & ~0x60) | abr;
    }
    *(u32 *)&poly->r0 = code; /* code byte: semi-transparency bit */
    poly->tpage = tpage;
    gte_ldrgb(&colors[idx[1] & 0x1FFF]);
    gte_dpcs();
    gte_strgb(&poly->r3);
}
#endif

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001795C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017B08);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017D90);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017EE8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800180BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001828C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001830C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80018388);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800183B0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800183DC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80018430);

void func_800184D4(u8 **arg0) {
    u8 *temp_a0;

    temp_a0 = *arg0;
    if (temp_a0 != NULL) {
        (*(s16*)((u8*)(temp_a0) + 86)) = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800184F0);

s32 *func_80018570(u8 *arg0) {
    s32 **temp_a1;
    s32 *temp_v1;

    if ((arg0 == NULL) || (temp_v1 = (*(s32*)((u8*)(arg0) + 12)), (temp_v1 == NULL))) {
        return NULL;
    }
    temp_a1 = (*(s32*)((u8*)(arg0) + 16));
    (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
    if (temp_a1 != NULL) {
        *temp_a1 = *temp_v1;
        (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
        if (*temp_v1 == NULL) {
            (*(s32*)((u8*)(arg0) + 8)) = temp_a1;
        }
    } else {
        (*(s32*)((u8*)(arg0) + 4)) = (s32 *) *temp_v1;
        (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
        *temp_v1 = NULL;
    }
    return temp_v1;
}

s32 *func_800185F8(u8 *arg0) {
    s32 **temp_a1;
    s32 *temp_v1;

    if ((arg0 == NULL) || (temp_v1 = (*(s32*)((u8*)(arg0) + 12)), (temp_v1 == NULL))) {
        return NULL;
    }
    temp_a1 = (*(s32*)((u8*)(arg0) + 16));
    (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
    if (temp_a1 != NULL) {
        *temp_a1 = *temp_v1;
        (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
        if (*temp_v1 == NULL) {
            (*(s32*)((u8*)(arg0) + 8)) = temp_a1;
        }
    } else {
        if ((*(s32*)((u8*)(arg0) + 8)) == temp_v1) {
            (*(s32*)((u8*)(arg0) + 8)) = NULL;
        }
        (*(s32*)((u8*)(arg0) + 4)) = (s32 *) *temp_v1;
        (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
        *temp_v1 = NULL;
    }
    return temp_v1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80018694);

void func_80018728(s32 arg0, s32 arg1) {
    func_80017EE8();
    func_80017D90(arg0, arg1);
}

void func_80018764(s32 *arg0) {
    s32 *temp_v1;
    s32 *var_a0;
    s32 *var_s0;

    var_s0 = arg0;
    var_a0 = NULL;
    if (arg0 != NULL) {
        do {
            func_800110A8((*(s32*)((u8*)(var_s0) + 4)), D_8004E0F0);
            var_a0 = var_s0;
            var_s0 = (*(s32*)((u8*)(var_a0) + 0));
        } while (var_s0 != NULL);
    }
    temp_v1 = D_80056858;
    D_80056858 = arg0;
    *var_a0 = temp_v1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800187D4);

void func_8001888C(void) {
    u8 *var_s0;

    DrawSync(0);
    printf("empty prims\n");
    if (D_8005B698 != 0) {
        func_8002775C(D_8005B790);
        func_8002775C(D_8005B790 + 0x400C);
    }
    var_s0 = D_80056864;
    if (var_s0 != NULL) {
        do {
            func_800187D4(var_s0 + 0x18, (*(u16*)((u8*)(var_s0) + 84)));
            func_800187D4(var_s0 + 4, (*(u16*)((u8*)(var_s0) + 84)));
            func_800187D4(var_s0 + 0x40, (*(u16*)((u8*)(var_s0) + 84)));
            func_800187D4(var_s0 + 0x2C, (*(u16*)((u8*)(var_s0) + 84)));
            var_s0 = (*(s32*)((u8*)(var_s0) + 0));
        } while (var_s0 != NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80018938);

void func_800189E0(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 4)) = 0;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
    (*(s32*)((u8*)(arg0) + 16)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800189F4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80018B08);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80018F3C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80019094);

/* Draws the triangle strips of a model: `runs` holds {mode, count} byte
 * pairs ended by count 0xFF, `v` the vertices (vz's high half carries
 * flags: 0x10000 flips the winding test, 0x20000 skips it). Mode bit 0
 * selects primitives whose SXY go at +0x10/+0x18/+0x20 and whose length is
 * preset; otherwise each is a POLY_GT3. The three register variables are
 * the original's: only they give s4/s3/s2 with s0/s1 unused. */
void func_800193A8(POLY_GT3 *poly, SVECTOR *v, u8 *runs) {
    u32 otz;
    s32 opz;
    GfxState *gs;
    s32 n;
    s32 mode;
    register s32 xy asm("$20");
    register s32 z asm("$19");
    register s32 flags asm("$18");

    gs = &D_80056998;
    xy = ((s32 *)v)[0];
    z = ((s32 *)v)[1];
    for (;;) {
        if (runs[1] == 0xFF) {
            return;
        }
        gte_ldv0_w(xy, z);
        v++;
        n = runs[1];
        gte_rtps_b();
        xy = ((s32 *)v)[0];
        z = ((s32 *)v)[1];
        gte_ldv0_w(xy, z);
        v++;
        mode = *runs;
        gte_rtps_b();
        xy = ((s32 *)v)[0];
        z = ((s32 *)v)[1];
        if (mode & 1) {
            for (;;) {
                if (n-- <= 0) {
                    goto next;
                }
                gte_ldv0_w(xy, z);
                v++;
                flags = z;
                gte_rtps_b();
                xy = ((s32 *)v)[0];
                z = ((s32 *)v)[1];
                gte_avsz3_b();
                gte_stotz(&otz);
                if (otz != 0) {
                    otz = (otz + gs->otzAdd) >> 1;
                    if (otz < gs->otLen) {
                        if (!(flags & 0x20000)) {
                            gte_nclip_b();
                            gte_stopz(&opz);
                            if (flags & 0x10000) {
                                if (opz > 0) {
                                    goto skip;
                                }
                            } else if (opz <= 0) {
                                goto skip;
                            }
                        }
                        addPrim(gs->ot + otz, poly);
                        gte_stsxy3_10(poly);
                    }
                }
            skip:
                poly++;
            }
        } else {
            while (n-- > 0) {
                gte_ldv0_w(xy, z);
                v++;
                flags = z;
                gte_rtps_b();
                xy = ((s32 *)v)[0];
                z = ((s32 *)v)[1];
                gte_avsz3_b();
                gte_stotz(&otz);
                otz = (otz + gs->otzAdd) >> 1;
                if (otz < gs->otLen) {
                    if (!(flags & 0x20000)) {
                        gte_nclip_b();
                        gte_stopz(&opz);
                        if (flags & 0x10000) {
                            if (opz > 0) {
                                poly++;
                                continue;
                            }
                        } else if (opz <= 0) {
                            poly++;
                            continue;
                        }
                    }
                    poly->tag = 0x09000000 | (gs->ot[otz] & 0xFFFFFF);
                    setaddr(gs->ot + otz, poly);
                    gte_stsxy3_gt3(poly);
                }
                poly++;
            }
        }
    next:
        runs += 2;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001965C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80019A60);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80019F1C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001A0D8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001A43C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001A6D4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001A82C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001AA48);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001ACC0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001AF2C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001B6D4);

void func_8001C008(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc) {
    SVECTOR *dst = D_80056AC8;
    u8 *run = (u8 *)&model->runs + model->runs;
    s32 r, g, b;
    s32 i;

    gte_lddp(p);
    r = fc->vx;
    g = fc->vy;
    b = fc->vz;
    for (; run[1] != 0xFF; run += 2) {
        i = run[1] + 2;
        while (i-- > 0) {
            gte_ldsv(src);
            gte_ldfcdir(r, g, b);
            fc++;
            dst->pad = src->pad;
            src++;
            gte_intpl();
            r = fc->vx;
            g = fc->vy;
            b = fc->vz;
            gte_stsv(dst);
            dst++;
        }
    }
}

void func_8001C0F0(s32 p, MorphModel *model, u16 *ia, u16 *ib, s16 (*verts)[3]) {
    SVECTOR *dst = D_80056AC8;
    u8 *run;
    u32 a;
    u32 b;
    s16 *va;
    s16 *vb;
    s32 flag;
    s32 i, n;
    s32 r, g, bl;

    gte_lddp(p);
    a = *ia;
    b = *ib;
    run = (u8 *)(model->runs + (s32)model) + 0x15;
    for (;;) {
        n = *run;
        if (n == 0xFF) {
            return;
        }
        i = n + 2;
        while (i-- > 0) {
            va = verts[a >> 2];
            vb = verts[b >> 2];
            gte_ldsv(va);
            flag = b & 3;
            r = vb[0];
            g = vb[1];
            bl = vb[2];
            gte_ldfcdir(r, g, bl);
            ia++;
            ib++;
            gte_intpl_b();
            b = *ib;
            a = *ia;
            gte_stsv(dst);
            dst->pad = flag;
            dst++;
        }
        run += 2;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001C1E0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001C270);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001C298);

void func_8001C2C0(u8 *arg0) {
    s32 (*temp_v0)(u8 *, s32, s32);
    s32 temp_s1;
    s32 temp_s2;
    u8 *var_s0;

    var_s0 = (*(s32*)((u8*)(arg0) + 108));
    D_8005699C = arg0;
    if (var_s0 != NULL) {
        temp_s1 = (*(s32*)((u8*)(arg0) + 12));
        if (temp_s1 & 0x8000) {
            temp_s2 = func_8001ACC0(arg0 + 0xC);
loop_3:
            if (var_s0 != NULL) {
                temp_v0 = (*(s32*)((u8*)(var_s0) + 84));
                if (temp_v0 != NULL) {
                    temp_v0(var_s0, temp_s2, temp_s1);
                }
                var_s0 = (*(s32*)((u8*)(var_s0) + 92));
                goto loop_3;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001C34C);

void func_8001C378(u8 *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;

    temp_a0 = (*(s32*)((u8*)(arg0) + 144));
    if (temp_a0 != 0) {
        func_80018388(temp_a0);
        (*(s32*)((u8*)(arg0) + 144)) = 0;
    }
    temp_a0_2 = (*(s32*)((u8*)(arg0) + 156));
    if (temp_a0_2 != 0) {
        func_80018388(temp_a0_2);
        (*(s32*)((u8*)(arg0) + 156)) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001C3D0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001C448);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001C584);

void func_8001C690(NAS *nas, u8 *arg1) {
    GteSetup s;
    u32 hidden;

    if (nas->flags & 0x8000) {
        hidden = nas->flags & 0x10000000;
        if (nas->flags & 0x02000000) {
            D_800569DC = nas->unk68;
        } else {
            D_800569DC = 0;
        }
        if (!(nas->flags & 0x10000000)) {
            D_80056998.unk84 = D_80056998.unkCC;
            D_80056998.unk88 = D_80056998.unk10C;
            func_8001965C(nas, arg1, arg1 + 0x14, D_80056998.unk28, &s);
            gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
            if (s.trans != NULL) {
                gte_SetTransVector(s.trans);
            }
            gte_SetRotMatrix(s.rot);
        }
        func_8001A6D4(&nas->ast, hidden);
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", func_8001C7FC);
#else
s32 func_8001C7FC(NAS *nas, u8 *arg1, u32 flags) {
    GteSetup s;
    u32 f;

    if (nas->flags & 0x8000) {
        f = flags | nas->flags;
        if (nas->flags & 0x02000000) {
            D_800569DC = nas->unk68;
        } else {
            D_800569DC = 0;
        }
        if (!(nas->flags & 0x10000000)) {
            D_80056998.unk84 = D_80056998.unkCC;
            D_80056998.unk88 = D_80056998.unk10C;
            func_8001965C(nas, arg1, arg1 + 0x14, D_80056998.unk28, &s);
            gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
            if (s.trans != NULL) {
                gte_SetTransVector(s.trans);
            }
            gte_SetRotMatrix(s.rot);
        }
        f &= ~0x10000000;
        return func_8001A0D8(&nas->ast, f | nas->flags);
    }
}
#endif

u32 func_8001C974(NAS *nas, u8 *arg1, u32 flags) {
    GteSetup s;

    flags |= nas->flags;
    if (nas->flags & 0x02000000) {
        D_800569DC = nas->unk68;
    } else {
        D_800569DC = 0;
    }
    if (!(nas->flags & 0x10000000)) {
        D_80056998.unk84 = D_80056998.unkCC;
        D_80056998.unk88 = D_80056998.unk10C;
        func_8001965C(nas, arg1, arg1 + 0x14, D_80056998.unk28, &s);
        gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
        if (s.trans != NULL) {
            gte_SetTransVector(s.trans);
        }
        gte_SetRotMatrix(s.rot);
    }
    flags &= ~0x10000000;
    return flags | nas->flags;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001CAD4);

void func_8001CBD4(u32 arg0) {
    func_8002A2EC((arg0 >> 0x10) & 0xFF, (arg0 >> 8) & 0xFF, arg0 & 0xFF);
}

void func_8001CC08(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001CC10);

void func_8001CD04(void *arg0, void *arg1, void *arg2) {
    GteSetup s;

    D_80056998.unk84 = D_80056998.unkCC;
    D_80056998.unk88 = D_80056998.unk10C;
    func_8001965C(arg0, arg1, arg2, D_80056998.unk28, &s);
    gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
    if (s.trans != NULL) {
        gte_SetTransVector(s.trans);
    }
    gte_SetRotMatrix(s.rot);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001CE00);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001CEB4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001CF50);

void *func_8001CFE8(MorphModel *model, void *arg1, s32 arg2) {
    void *p;

    p = func_80019094(model, arg2);
    if (p != NULL) {
        func_800193A8(p, arg1, (u8 *)&model->runs + model->runs);
    }
    return p;
}

void func_8001D048(SVECTOR *v, POLY_GT4 *poly) {
    u32 otz;
    s32 xy;
    s32 z;

    gte_ldv0_w(((s32 *)v)[0], ((s32 *)v)[1]);
    v++;
    gte_rtps();
    gte_ldv0_w(((s32 *)v)[0], ((s32 *)v)[1]);
    v++;
    gte_rtps();
    gte_ldv0_w(((s32 *)v)[0], ((s32 *)v)[1]);
    v++;
    gte_rtps();
    xy = ((s32 *)v)[0];
    z = ((s32 *)v)[1];
    gte_stsxy3_gt4(poly);
    gte_ldv0_w(xy, z);
    gte_rtps();
    gte_stsxy(&poly->x3);
    gte_avsz3();
    gte_stotz(&otz);
    otz = (otz + D_80056998.otzAdd) >> 1;
    if (otz < D_80056998.otLen) {
        addPrim(D_80056998.ot + otz, poly);
    }
}

void func_8001D174(EXTengine *eng) {
    void (*release)(subengine *);
    s32 voice;
    subengine *next;
    subengine *first;
    subengine *last;
    subengine *freeList;
    subengine *sub;

    voice = eng->unk94;
    if (voice != 0) {
        StopSfx(voice);
    }
    first = eng->sub;
    last = eng->subLast;
    sub = first;
    if (first != NULL) {
        do {
            release = sub->release;
            next = sub->next;
            if (release != NULL) {
                release(sub);
            }
            sub->unk54 = 0;
            sub->release = NULL;
            sub->flags = 0;
            sub = next;
        } while (sub != NULL);
        freeList = D_80058ACC;
        D_80058ACC = first;
        last->next = freeList;
    }
    eng->sub = NULL;
    eng->subCount = 0;
}

void func_8001D220(EXTengine *eng) {
    void (*release)(subengine *);
    subengine *next;
    subengine *first;
    subengine *last;
    subengine *freeList;
    subengine *sub;

    first = eng->sub;
    last = eng->subLast;
    sub = first;
    if (first != NULL) {
        do {
            release = sub->release;
            next = sub->next;
            if (release != NULL) {
                release(sub);
            }
            sub->unk54 = 0;
            sub->release = NULL;
            sub->flags = 0;
            sub = next;
        } while (sub != NULL);
        freeList = D_80058ACC;
        D_80058ACC = first;
        last->next = freeList;
    }
    eng->sub = NULL;
    eng->subCount = 0;
}

void func_8001D2B4(subengine *first, subengine *last) {
    void (*release)(subengine *);
    subengine *next;
    subengine *freeList;
    subengine *sub;

    if (first != NULL) {
        sub = first;
        do {
            release = sub->release;
            next = sub->next;
            if (release != NULL) {
                release(sub);
            }
            sub->unk54 = 0;
            sub->release = NULL;
            sub->flags = 0;
            sub = next;
        } while (sub != NULL);
        freeList = D_80058ACC;
        D_80058ACC = first;
        last->next = freeList;
    }
}

void func_8001D334(subengine *first) {
    void (*release)(subengine *);
    subengine *next;
    subengine *sub;

    sub = first;
    if (sub != NULL) {
        do {
            release = sub->release;
            next = sub->next;
            if (release != NULL) {
                release(sub);
            }
            sub->unk54 = 0;
            sub->release = NULL;
            sub->flags = 0;
            sub = next;
        } while (sub != NULL);
    }
}

s32 func_8001D390(EXTengine *eng, s32 n) {
    return func_80018F3C(eng, n);
}

void func_8001D3B0(EXTengine *eng, s32 n, subengine *sub) {
    if (eng != NULL) {
        eng->sub = sub;
        eng->subCount = n;
    }
    *sub = D_8005A830;
    n--;
    sub->prev = NULL;
    while (n != 0) {
        sub->next = sub + 1;
        sub++;
        *sub = D_8005A830;
        n--;
        sub->prev = sub - 1;
    }
    sub->next = NULL;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001D48C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001D5D0);

void func_8001D604(void) {
    if (D_80058AC8 != 0) {
        func_800110A8(D_80058AC8, D_8004E0F0);
        D_80058AC8 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001D640);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001D6B4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001D894);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001DAF8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001DCD4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001DD20);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001DD50);

void func_8001DDB4(s32 *arg0) {
    func_80029100(*arg0 + 0x10);
}

void func_8001DDD8(u8 *arg0) {
    func_80016F54((*(s32*)((u8*)(arg0) + 0)));
    func_800110A8((*(s32*)((u8*)(arg0) + 28)), D_8004E0F0);
    (*(s32*)((u8*)(arg0) + 28)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001DE18);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001DEF4);

void func_8001DF74(u8 *arg0, u8 **arg1) {
    u8 *temp_s0;
    u8 *temp_s1;

    temp_s1 = *arg1;
    func_80011544(arg0, temp_s1 + 0x20);
    temp_s0 = temp_s1 + 0x10;
    func_8002A5A4(temp_s0, (*(s32*)((u8*)(temp_s0) + 16)));
    func_80029560(temp_s0);
    func_8002A35C(temp_s0);
    (*(s32*)((u8*)(temp_s1) + 8)) = func_80013034((*(s32*)((u8*)(temp_s1) + 0)), 0, (*(s32*)((u8*)(temp_s1) + 36)), func_8001DFF8, arg1, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001DFF8);

void func_8001E054(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 0)) = arg1;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    D_80058AD0 = arg0;
    func_8001DE18();
    (*(s32*)((u8*)(arg0) + 28)) = 0;
    func_8001D6B4(arg0);
    func_8001682C(arg1);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E0A8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E21C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E41C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E48C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E51C);

void func_8001E588(u8 *arg0, s32 arg1, s32 arg2) {
    (*(s32*)((u8*)(arg0) + 4)) = arg1;
    (*(s32*)((u8*)(arg0) + 12)) = arg2;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E598);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E610);

void func_8001E824(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 12)) = 2;
    (*(s32*)((u8*)(arg0) + 4)) = (s32) (*(s32*)((u8*)(arg0) + 8));
}

void func_8001E838(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 4)) = arg1;
    (*(s32*)((u8*)(arg0) + 12)) = 1;
}

void func_8001E848(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 4)) = arg1;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E854);

void func_8001E898(u8 *arg0) {
    s32 (*temp_v1)();
    u8 *temp_v0;

    if (arg0 != NULL) {
        temp_v0 = (*(s32*)((u8*)(arg0) + 0));
        if (temp_v0 != NULL) {
            temp_v1 = (*(s32*)((u8*)(temp_v0) + 8));
            if ((temp_v1 != NULL) && !((*(s32*)((u8*)(arg0) + 12)) & 4)) {
                temp_v1();
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E8F0);

void func_8001E934(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 0)) = arg1;
    (*(s32*)((u8*)(arg0) + 4)) = 0;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
}

void func_8001E948(void) {
    func_8001888C();
    do {

    } while (func_80011DB4(D_8004E0F0) != 0);
    func_80010AE8(D_8004E0F0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E984);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001EAA4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001EDFC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001F0D4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001F4F8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001F604);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001F828);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001FCDC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001FE80);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020064);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800201BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020344);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020680);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020B44);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020BA8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020C04);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020C4C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020CC4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020DF8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020E9C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020F5C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021054);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800210F8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002120C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021238);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002128C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021330);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800213A0);

void func_8002141C(u8 *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;

    temp_a0 = (*(s32*)((u8*)(arg0) + 12));
    if (temp_a0 != 0) {
        func_8001D2B4(temp_a0, (*(s32*)((u8*)(arg0) + 16)));
        (*(s32*)((u8*)(arg0) + 12)) = 0;
    }
    temp_a0_2 = (*(s32*)((u8*)(arg0) + 20));
    if (temp_a0_2 != 0) {
        func_800110A8(temp_a0_2, D_8004E0F0);
        (*(s32*)((u8*)(arg0) + 20)) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002147C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021604);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021708);

void func_80021770(u8 *arg0, s32 *arg1) {
    func_80019F1C(arg0 + 8, *arg1, (*(s32*)((u8*)(arg0) + 8)));
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021798);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021878);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800218DC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021940);

void func_80021990(u8 *arg0, s32 *arg1) {
    func_80019F1C(arg0 + 8, *arg1, (*(s32*)((u8*)(arg0) + 8)));
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800219B8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021A1C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021AF0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021B88);

void func_80021C20(u8 *arg0, u8 *arg1, u8 *arg2, s32 arg3, s32 arg4) {
    (*(s32*)((u8*)(arg2) + 0)) = func_80015304((*(s32*)((u8*)(arg0) + 0)), (*(s32*)((u8*)(arg1) + 0)), arg3, arg4);
    (*(s32*)((u8*)(arg2) + 4)) = func_80015304((*(s32*)((u8*)(arg0) + 4)), (*(s32*)((u8*)(arg1) + 4)), arg3, arg4);
    (*(s32*)((u8*)(arg2) + 8)) = func_80015304((*(s32*)((u8*)(arg0) + 8)), (*(s32*)((u8*)(arg1) + 8)), arg3, arg4);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021CB8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021D1C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021DD4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021E44);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021E60);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021E98);

void func_80021ED8(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = D_80058B70;
    D_80058B70 = arg0;
    (*(s32*)((u8*)(arg0) + 180)) = temp_v1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021EEC);

void func_80021F14(u8 *arg0) {
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *temp_v1_3;

    temp_v1 = (*(s32*)((u8*)(arg0) + 112));
    if (temp_v1 != NULL) {
        (*(s32*)((u8*)(temp_v1) + 116)) = (u8 *) (*(s32*)((u8*)(arg0) + 116));
    } else {
        D_80058B78 = (*(s32*)((u8*)(arg0) + 116));
    }
    temp_v1_2 = (*(s32*)((u8*)(arg0) + 116));
    if (temp_v1_2 != NULL) {
        (*(s32*)((u8*)(temp_v1_2) + 112)) = (u8 *) (*(s32*)((u8*)(arg0) + 112));
    }
    temp_v1_3 = D_80058B74;
    D_80058B74 = arg0;
    (*(s32*)((u8*)(arg0) + 116)) = temp_v1_3;
}

u8 *func_80021F6C(void) {
    u8 *temp_v0;
    u8 *temp_v1;

    temp_v1 = D_80058B74;
    if (temp_v1 != NULL) {
        D_80058B74 = (*(s32*)((u8*)(temp_v1) + 116));
        if (D_80058B78 != NULL) {
            (*(s32*)((u8*)(D_80058B78) + 112)) = temp_v1;
        }
        temp_v0 = D_80058B78;
        (*(s32*)((u8*)(temp_v1) + 112)) = 0;
        D_80058B78 = temp_v1;
        (*(s32*)((u8*)(temp_v1) + 116)) = temp_v0;
    }
    return temp_v1;
}

void func_80021FB0(void) {
    u8 *temp_s0;
    u8 *var_a0;

    var_a0 = D_80058B78;
    if (var_a0 != NULL) {
        do {
            temp_s0 = (*(s32*)((u8*)(var_a0) + 116));
            func_80020F5C(var_a0);
            var_a0 = temp_s0;
        } while (var_a0 != NULL);
    }
    if (D_80058B6C != 0) {
        func_800110A8(D_80058B6C, D_8004E0F0);
    }
    if (D_80058B68 != 0) {
        func_800110A8(D_80058B68, D_8004E0F0);
    }
}

void func_80022030(void) {
    u8 *temp_s0;
    u8 *var_a0;

    var_a0 = D_80058B78;
    if (var_a0 != NULL) {
        do {
            temp_s0 = (*(s32*)((u8*)(var_a0) + 116));
            func_80020F5C(var_a0);
            var_a0 = temp_s0;
        } while (var_a0 != NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80022070);

void func_800220E8(void) {
    u8 *temp_s1;
    u8 *var_s0;

    var_s0 = D_80058B78;
    if (var_s0 != NULL) {
        do {
            temp_s1 = (*(s32*)((u8*)(var_s0) + 116));
            if (func_80020064(var_s0) == 0) {
                func_80021F14(var_s0);
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80022140);

void func_800222BC(void) {
    func_8003347C();
}

void func_800222DC(void) {
    func_8003346C();
}

void func_800222FC(void) {
    SsInit();
    func_8002C060();
    func_80022984();
}

void func_8002232C(void) {
    func_8002BBEC();
    SsInit();
    func_8002C44C();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002235C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80022504);

void StopSfx(SfxVoice *voice) {
    if (voice != NULL) {
        if (voice->owner != NULL) {
            *voice->owner = 0;
        }
        func_8002B8F4(&voice->unk2C);
        if (voice->next != NULL) {
            voice->next->prev = voice->prev;
        }
        if (voice->prev != NULL) {
            voice->prev->next = voice->next;
        } else {
            SfxVoicePool.active = voice->next;
        }
        voice->next = SfxVoicePool.free;
        SfxVoicePool.free = voice;
    }
}

SfxVoice *PlaySfx(s32 id, SfxVoice **handle, SLONG_XYZ *pos, s32 arg3, s32 arg4, s32 flags) {
    SfxVoice *voice;
    SfxVoice *v;

    voice = NULL;
    if (id & 0x8000) {
        func_80027A74();
    } else {
        if (SfxTable[id][1] == -1) {
            return NULL;
        }
        v = SfxVoicePool.free;
        if (v != NULL) {
            SfxVoicePool.free = v->next;
            v->next = SfxVoicePool.active;
            v->prev = NULL;
            if (SfxVoicePool.active != NULL) {
                SfxVoicePool.active->prev = v;
            }
            voice = v;
            voice->owner = NULL;
            voice->unk28 = -1;
            voice->unk2C = -1;
        }
        if (voice != NULL) {
            voice->next = SfxVoicePool.active;
            voice->prev = NULL;
            SfxVoicePool.active = voice;
            voice->id = id;
            voice->unk20 = arg3;
            voice->unk24 = arg4;
            voice->flags = flags;
            voice->unk28 = -1;
            if (handle != NULL) {
                if (*handle != NULL) {
                    StopSfx(*handle);
                }
                *handle = voice;
                voice->owner = handle;
                voice->pos = pos;
            } else if (pos != NULL) {
                voice->posCopy = *pos;
                voice->pos = &voice->posCopy;
            } else {
                voice->pos = NULL;
            }
        }
    }
    return voice;
}

void func_800227C0(void) {
    u8 *var_v1;

    var_v1 = SfxActiveVoices;
    if (var_v1 != NULL) {
        do {
            (*(s32*)((u8*)(var_v1) + 28)) = (s32) ((*(s32*)((u8*)(var_v1) + 28)) | 4);
            var_v1 = (*(s32*)((u8*)(var_v1) + 24));
        } while (var_v1 != NULL);
    }
}

void func_800227FC(void) {
    u8 *var_v1;

    var_v1 = SfxActiveVoices;
    if (var_v1 != NULL) {
        do {
            (*(s32*)((u8*)(var_v1) + 28)) = (s32) ((*(s32*)((u8*)(var_v1) + 28)) | 2);
            var_v1 = (*(s32*)((u8*)(var_v1) + 24));
        } while (var_v1 != NULL);
    }
}

void func_80022838(void) {
    u8 *temp_s1;
    u8 *var_s0;

    func_8002AEE0();
    var_s0 = SfxActiveVoices;
    if (var_s0 != NULL) {
        do {
            temp_s1 = (*(s32*)((u8*)(var_s0) + 24));
            if (func_8002235C(var_s0) != 1) {
                StopSfx(var_s0);
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}

void func_800228A4(u8 *arg0, s32 arg1) {
    if (arg0 != NULL) {
        (*(s32*)((u8*)(arg0) + 36)) = arg1;
        (*(s32*)((u8*)(arg0) + 28)) = (s32) ((*(s32*)((u8*)(arg0) + 28)) | 1);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800228C4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80022924);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80022984);

void func_800229D4(void) {
    func_8002BBB4();
    func_80022984();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800229FC);

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", SpawnNASById);
#else
NAS *SpawnNASById(s32 x, s32 y, s32 z, u16 id, u16 flags) {
    NAS *nas;
    NAS *free;
    EXTengine *eng;
    s32 i;
    s32 r;
    s32 want;
    u16 ast;

    nas = NULL;
    want = id;
    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == want) {
            r = i;
            goto found;
        }
    }
    r = 0;
found:
    ast = r;
    if (ast != 0 && ast < ASTCount) {
        nas = NULL;
        free = NASFreeList;
        if (free != NULL) {
            nas = free->next;
            NASFreeList = nas;
            if (nas != NULL) {
                nas->prev = NULL;
            }
            nas = free;
        }
        if (nas != NULL) {
            *nas = NASTemplate;
            nas->flags = 1;
            if (NASActiveTail != NULL) {
                NASActiveTail->next = nas;
                nas->prev = NASActiveTail;
                NASActiveTail = nas;
            } else {
                NASActiveTail = nas;
                NASActiveHead = nas;
            }
            nas->ast = ast;
            nas->Init = ASTTable[ast].Init;
            nas->Plot = ASTTable[ast].Plot;
            if (flags & 1) {
                eng = AddEngine(nas, 1);
                if (eng != NULL) {
                    eng->unk10.x = x;
                    eng->unk10.y = y;
                    eng->unk10.z = z;
                } else {
                    if (flags & 0x10) {
                        KillNAS(nas);
                    }
                    return NULL;
                }
            }
            nas->Init(nas);
            if (nas->flags == 0) {
                nas = NULL;
            }
        }
    }
    return nas;
}
#endif

INCLUDE_ASM("asm/nonmatchings/main/main", func_80022C3C);

INCLUDE_ASM("asm/nonmatchings/main/main", SpawnNASFromRecord);

s32 KillNAS(NAS *nas) {
    EXTengine *eng;

    if (!(nas->flags & 1)) {
        return 1;
    }
    if (nas->spawn != NULL && (nas->spawn->flags & 0x10)) {
        return 0;
    }
    if (nas->unk68 & 4) {
        nas->Init(nas);
    }
    if (nas->spawn != NULL) {
        nas->spawn->flags &= ~1;
    }
    eng = nas->eng;
    for (;;) {
        if (eng == NULL) {
            goto freed;
        }
        if (eng->link != NULL) {
            eng->link->link = NULL;
        }
        eng->link = NULL;
        func_8001C270(eng);
        eng = eng->next;
    }
freed:
    func_80026430(nas);
    func_80026BF4((u8 *)nas);
    func_80026AFC(nas);
    func_80026980(nas);
    func_80026A54(nas);
    func_800268BC(nas);
    if (nas->prev != NULL) {
        nas->prev->next = nas->next;
        if (nas->next != NULL) {
            nas->next->prev = nas->prev;
        } else {
            NASActiveTail = nas->prev;
        }
    }
    nas->flags = 0;
    if (NASFreeList != NULL) {
        NASFreeList->prev = nas;
        nas->next = NASFreeList;
        NASFreeList = nas;
    } else {
        NASFreeList = nas;
        nas->next = NULL;
    }
    nas->prev = NULL;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80023158);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002321C);

INCLUDE_ASM("asm/nonmatchings/main/main", AllocExtension);

INCLUDE_ASM("asm/nonmatchings/main/main", FreeExtension);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800235BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800237BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80023A90);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024008);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024214);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800243A0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800246D0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800248A0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024AD8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024E30);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024FA4);

s16 func_800250B0(NAS *nas) {
    EXTengine *eng;
    EXTanim *anim;
    subengine *sub;
    s32 id;
    s32 kind;
    s32 want;

    if ((nas->flags & 0x500) == 0x500 && (eng = nas->eng) != NULL) {
        id = 0x400;
        want = 0;
        for (anim = nas->ext; anim != NULL; anim = anim->Extend.next) {
            if (anim->EXTid & id) {
                if (want == 0) {
                    goto found;
                }
                if ((anim->EXTid & 0xFF) == want) {
                    goto found;
                }
            }
        }
        anim = NULL;
    found:
        if (anim != NULL) {
            kind = anim->EXTid & 0xFF;
            if (anim->eng != NULL) {
                eng = anim->eng;
            }
            sub = anim->sub;
            if (sub == NULL) {
                sub = eng->sub;
            }
            switch (kind) {
            case 1:
                return func_800251A8(nas, eng, sub, anim);
            case 2:
                return func_80025534(nas, eng, sub, anim);
            }
        }
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800251A8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80025534);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002578C);

FlagRecord *func_80025A48(EXTengine *eng, u32 flag) {
    PointList *list;
    FlagRecord *rec;
    s32 n;

    if (eng->rec.flags & flag) {
        return &eng->rec;
    }
    if (eng->sub != NULL && (list = func_8001562C(eng->sub)) != NULL) {
        n = list->nRecs;
        rec = (FlagRecord *)&list->pts[list->count];
        for (; n != 0; n--, rec++) {
            if (rec->flags & flag) {
                return rec;
            }
        }
    }
    return NULL;
}

void func_80025AE4(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6) {
    (*(s16*)((u8*)(arg0) + 132)) = arg1;
    (*(s16*)((u8*)(arg0) + 134)) = arg2;
    (*(s16*)((u8*)(arg0) + 136)) = arg3;
    (*(s16*)((u8*)(arg0) + 142)) = 0;
    (*(s16*)((u8*)(arg0) + 138)) = (s16) arg4;
    (*(s16*)((u8*)(arg0) + 140)) = (s16) arg5;
    (*(s32*)((u8*)(arg0) + 144)) = arg6;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80025B10);

s32 func_80025BBC(EXTengine *eng) {
    s32 dy;

    func_80032F3C(eng->unk10.y - eng->unk64);
    func_80032F3C(eng->unk10.x - eng->unk60);
    func_80032F3C(eng->unk10.z - eng->unk68);
    if (D_8005AA94 != NULL) {
        dy = D_8005AA94(eng);
    } else {
        dy = func_80025E00(eng, eng->unk10.y - D_8005A5D8);
    }
    D_8005B5B8 = dy;
    if (dy == 0x7FFFFFFF) {
        return 0;
    }
    if (dy < 0) {
        return 0;
    }
    eng->unk10.y -= dy;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80025C80);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80025D40);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80025E00);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80025F44);

void func_80025F8C(void) {
}

void func_80025F94(void) {
}

void func_80025F9C(void) {
}

void func_80025FA4(void) {
}

void func_80025FAC(EXTanim *anim, s16 n) {
    switch (anim->EXTid & 0xFF) {
    case 1:
        if (anim->seqs != NULL) {
            anim->seqId = n;
            anim->seq = anim->seqs[n];
        }
        anim->unk1E = 0;
        anim->unk1C = -1;
        anim->unk28 = 0;
        break;
    case 2:
        if (anim->seqs != NULL) {
            anim->seqId = n;
            anim->seq = anim->seqs[n];
        }
        anim->unk1E = -1;
        anim->unk1C = -1;
        anim->unk28 = 0;
        break;
    default:
        anim->unk28 = 0;
        break;
    }
    anim->unk2A = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026048);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002617C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026210);

void func_80026308(void) {
    func_80023A90();
    D_8005B2B4 = 0;
    D_8005B63C = D_80055684;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002633C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800263D8);

void func_80026418(void) {
}

void func_80026420(void) {
}

void func_80026428(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026430);

void func_800264B4(void) {
    func_800110A8(D_8006EA00, D_8004E0F0);
    D_8006EA00 = 0;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", FindExtension);
#else
void *FindExtension(NAS *nas, s32 id) {
    EXTheader *ext;
    s32 sub;

    sub = id & 0xFF;
    id &= ~0xFF;
    for (ext = nas->ext; ext != NULL; ext = ext->Extend.next) {
        if (ext->EXTid & id) {
            if (sub == 0) {
                return ext;
            }
            if ((ext->EXTid & 0xFF) == sub) {
                return ext;
            }
        }
    }
    return NULL;
}
#endif

EXTmotion *AddMotion(NAS *nas) {
    EXTmotion *mot;

    mot = AllocExtension(nas, 0x34, 0x800, &D_8005A9CC);
    if (mot != NULL) {
        nas->flags |= 0x800;
        *mot = D_8005A9CC;
    }
    return mot;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800265DC);

INCLUDE_ASM("asm/nonmatchings/main/main", AddObject);

EXTengine *AddEngine(NAS *nas, s32 id) {
    EXTengine *eng;
    EXTengine *prev;
    EXTengine *ext;

    eng = AllocExtension(nas, 0x9C, id | 0x100, &EngineTemplate);
    if (eng != NULL) {
        nas->flags |= 0x100;
        *eng = EngineTemplate;
        if (func_8001D390(eng, 1) != 0) {
            prev = nas->eng;
            if (prev == NULL) {
                nas->eng = eng;
            } else {
                for (ext = prev->Extend.next; ext != NULL; ext = ext->Extend.next) {
                    if (ext->EXTid & 0x100) {
                        prev->next = ext;
                        prev = ext;
                    }
                }
            }
        } else {
            FreeExtension(eng);
            eng = NULL;
        }
    }
    return eng;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026858);

void *AddExtension(void *owner, s32 size, s32 id) {
    return AllocExtension(owner, size, id, NULL);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800268BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002694C);

void func_80026980(NAS *nas) {
    NAS **head = (NAS **)&D_8005AA34;

    if (nas->prev38 != NULL || nas->next38 != NULL) {
        if (nas->prev38 != NULL) {
            nas->prev38->next38 = nas->next38;
            if (nas->next38 != NULL) {
                nas->next38->prev38 = nas->prev38;
            }
        } else {
            *head = nas->next38;
            nas->next38->prev38 = NULL;
        }
        nas->prev38 = NULL;
        nas->next38 = NULL;
    } else if (*head == nas) {
        *head = NULL;
    }
}

void func_80026A04(u8 *arg0) {
    if (D_8005AA34 != NULL) {
        (*(s32*)((u8*)(arg0) + 60)) = (u8 *) D_8005AA34;
        (*(s32*)((u8*)(D_8005AA34) + 56)) = arg0;
    }
    D_8005AA34 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026A28);

void func_80026A54(NAS *nas) {
    NAS **head = (NAS **)&D_8005AA24;

    if (nas->prev28 != NULL || nas->next28 != NULL) {
        if (nas->prev28 != NULL) {
            nas->prev28->next28 = nas->next28;
            if (nas->next28 != NULL) {
                nas->next28->prev28 = nas->prev28;
            }
        } else {
            *head = nas->next28;
            nas->next28->prev28 = NULL;
        }
        nas->prev28 = NULL;
        nas->next28 = NULL;
    } else if (*head == nas) {
        *head = NULL;
    }
}

void func_80026AD8(u8 *arg0) {
    if (D_8005AA24 != NULL) {
        (*(s32*)((u8*)(arg0) + 44)) = (u8 *) D_8005AA24;
        (*(s32*)((u8*)(D_8005AA24) + 40)) = arg0;
    }
    D_8005AA24 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026AFC);

void func_80026B70(u8 **arg0, u8 *arg1) {
    u8 *temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 != NULL) {
        (*(s32*)((u8*)(arg1) + 32)) = temp_v0;
        (*(s32*)((u8*)(temp_v0) + 28)) = arg1;
    }
    *arg0 = arg1;
    (*(s32*)((u8*)(arg1) + 36)) = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026B94);

void func_80026BF4(u8 *arg0) {
    u8 *temp_v1;
    u8 *temp_v1_2;

    temp_v1 = (*(s32*)((u8*)(arg0) + 20));
    if (temp_v1 != NULL) {
        (*(s32*)((u8*)(temp_v1) + 24)) = (u8 *) (*(s32*)((u8*)(arg0) + 24));
    }
    temp_v1_2 = (*(s32*)((u8*)(arg0) + 24));
    if (temp_v1_2 != NULL) {
        (*(s32*)((u8*)(temp_v1_2) + 20)) = (u8 *) (*(s32*)((u8*)(arg0) + 20));
    }
    (*(s32*)((u8*)(arg0) + 20)) = NULL;
    (*(s32*)((u8*)(arg0) + 24)) = NULL;
}

void func_80026C38(u8 *arg0, u8 *arg1) {
    u8 *var_a0;

    var_a0 = arg0;
    if ((*(s32*)((u8*)(var_a0) + 24)) != 0) {
        do {
            var_a0 = (*(s32*)((u8*)(var_a0) + 24));
        } while ((*(s32*)((u8*)(var_a0) + 24)) != 0);
    }
    (*(s32*)((u8*)(var_a0) + 24)) = arg1;
    (*(s32*)((u8*)(arg1) + 20)) = var_a0;
}

void func_80026C6C(u8 *arg0, u8 *arg1) {
    u8 *temp_v0;

    temp_v0 = (*(s32*)((u8*)(arg1) + 12));
    if (temp_v0 != NULL) {
        (*(s32*)((u8*)(temp_v0) + 16)) = arg0;
        (*(s32*)((u8*)(arg0) + 12)) = temp_v0;
        (*(s32*)((u8*)(arg0) + 16)) = arg1;
        (*(s32*)((u8*)(arg1) + 12)) = arg0;
    }
}

void func_80026C94(u8 *arg0, u8 *arg1) {
    u8 *temp_a2;
    u8 *temp_v1;
    u8 *temp_v1_2;

    temp_a2 = (*(s32*)((u8*)(arg1) + 12));
    if (temp_a2 != NULL) {
        temp_v1 = (*(s32*)((u8*)(arg0) + 12));
        if (temp_v1 != NULL) {
            (*(s32*)((u8*)(temp_v1) + 16)) = (u8 *) (*(s32*)((u8*)(arg0) + 16));
            temp_v1_2 = (*(s32*)((u8*)(arg0) + 16));
            if (temp_v1_2 != NULL) {
                (*(s32*)((u8*)(temp_v1_2) + 12)) = (u8 *) (*(s32*)((u8*)(arg0) + 12));
            } else {
                NASActiveTail = (*(s32*)((u8*)(arg0) + 12));
            }
        }
        (*(s32*)((u8*)(temp_a2) + 16)) = arg0;
        (*(s32*)((u8*)(arg0) + 12)) = temp_a2;
        (*(s32*)((u8*)(arg0) + 16)) = arg1;
        (*(s32*)((u8*)(arg1) + 12)) = arg0;
    }
}

void func_80026D00(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026D08);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026D9C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026E2C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026ECC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026F48);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026FCC);

void func_80027040(s32 arg0) {
    u8 *temp_s0;
    u8 *var_a0;

    var_a0 = NASActiveHead;
    if (var_a0 != NULL) {
        do {
            temp_s0 = (*(s32*)((u8*)(var_a0) + 16));
            if (((*(s32*)((u8*)(var_a0) + 0)) & 1) && ((*(u16*)((u8*)(var_a0) + 108)) == (arg0 & 0xFFFF))) {
                KillNAS(var_a0);
            }
            var_a0 = temp_s0;
        } while (var_a0 != NULL);
    }
}

void func_800270B0(NAS *nas) {
    if (nas->spawn != NULL) {
        nas->spawn->flags |= 0x8000;
    }
    KillNAS(nas);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800270F0);

s32 func_8002718C(void) {
    __main();
    ResetCallback();
    ResetGraph(0);
    func_8002D488();
    SetGraphDebug(0);
    InitGeom();
    SsInit();
    D_800637A8 = 0;
    func_80027F00();
    func_8003351C(D_800637A8);
    DrawSync(0);
    ClearImage2(&D_8005B640, 0, 0, 0);
    DrawSync(0);
    func_8002AABC();
    func_80028C94();
    func_80010158();
    func_80027908();
    func_800153A8();
    func_8002AB44();
    StopCallback();
    PadStopCom();
    SetDispMask(0);
    ResetGraph(3);
    return 0;
}

void func_8002726C(void) {
    DrawSync(0);
    ClearImage2(&D_8005B640, 0, 0, 0);
    DrawSync(0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800272AC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027408);

void func_8002759C(void) {
    RECT *temp_s0;

    temp_s0 = D_8005B698;
    if (temp_s0 != 0) {
        DrawSync(0);
        ClearImage(&temp_s0[1], 0, 0, 0);
        ClearImage(&temp_s0[0], 0, 0, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800275F8);

void func_800276A4(void) {
    func_800275F8();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800276C4);

void func_8002775C(u8 *arg0) {
    ClearOTagR((u_long *)arg0, 0x1000);
    *(s32 *)(arg0 + 0x4008) = *(s32 *)(arg0 + 0x4000);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027790);

void func_800278BC(s32 *arg0, s32 arg1) {
    CdIntToPos(*arg0 + arg1, &D_8006EA14);
    CdControlF(CdlSetloc, (u8 *)&D_8006EA14);
    CdControlF(CdlSeekL, (u8 *)&D_8006EA14);
}

void func_80027908(void) {
    if (D_800637B4 > 0) {
        D_800637B4 = CdReadSync(0, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027944);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800279A4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027A30);

void func_80027A74(void) {
    func_80012FFC();
}

void func_80027A94(void) {
}

void func_80027A9C(void) {
    if (D_80063A7C >= 0x1F) {
        D_80063A7C = 0x1E;
    }
    func_80027A30(0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027AD4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027D90);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027F00);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027F7C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028018);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800280C0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028158);

void func_80028184(u8 *arg0, s32 arg1, s8 arg2) {
    (*(s8*)((u8*)(arg0) + 5)) = arg2;
    (*(s16*)((u8*)(arg0) + 10)) = (s16) (arg1 + 1);
    (*(u16*)((u8*)(arg0) + 6)) = (u16) ((*(u16*)((u8*)(arg0) + 6)) | 1);
}

void func_800281A0(u8 *arg0, s32 arg1) {
    (*(s8*)((u8*)(arg0) + 4)) = 1;
    (*(s16*)((u8*)(arg0) + 8)) = (s16) (arg1 + 1);
    (*(u16*)((u8*)(arg0) + 6)) = (u16) ((*(u16*)((u8*)(arg0) + 6)) | 1);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800281C0);

void func_800282D8(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 4)) = 0;
    (*(s32*)((u8*)(arg0) + 0)) = 0;
    (*(s32*)((u8*)(arg0) + 24)) = 1;
    (*(s32*)((u8*)(arg0) + 28)) = 0;
    (*(s16*)((u8*)(arg0) + 34)) = 0;
    (*(s16*)((u8*)(arg0) + 40)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800282F8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028420);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002854C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028774);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028994);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028A00);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028A28);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028AE4);

s32 func_80028BC8(u8 **arg0) {
    u8 *p;
    s32 n;

    n = 0;
    for (p = *arg0; p != NULL; p = *(u8 **)(p + 4)) {
        n++;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028BF0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028C94);

void func_80028D38(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028D40);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028F28);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029100);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002926C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029560);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029790);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002992C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029D28);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029F9C);

void func_8002A2DC(void) {
}

void func_8002A2E4(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A2EC);

void func_8002A35C(u8 *arg0) {
    u8 *temp_a0;
    u8 *temp_v0;

    if ((*(s32*)((u8*)(arg0) + 24)) != 0) {
        temp_a0 = (*(s32*)((u8*)(arg0) + 16));
        temp_v0 = func_800112A4(temp_a0, (*(s32*)((u8*)(temp_a0) + 24)), D_8004E0F0);
        (*(s32*)((u8*)(arg0) + 16)) = temp_v0;
        func_80011544(temp_v0, arg0 + 0x18);
        (*(s32*)((u8*)(arg0) + 16)) = NULL;
        return;
    }
    func_80011578((*(s32*)((u8*)(arg0) + 16)), D_8004E0F0, 2);
    (*(s32*)((u8*)(arg0) + 16)) = NULL;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A3D0);

void func_8002A4B8(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A4C0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A5A4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A650);

void func_8002A67C(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A684);

void func_8002A740(u8 *arg0) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(arg0) + 4));
    if (temp_a0 != 0) {
        func_8002854C(temp_a0);
        (*(s32*)((u8*)(arg0) + 4)) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A77C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A830);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A84C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A9AC);

void func_8002AA50(u8 *arg0) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(arg0) + 32));
    if (temp_a0 != 0) {
        func_80028AE4(temp_a0);
        (*(s32*)((u8*)(arg0) + 32)) = 0;
    }
}

void func_8002AA8C(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 0)) = 0;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
    (*(s32*)((u8*)(arg0) + 16)) = 0;
}

void func_8002AAA0(void) {
    D_80064414 = 0;
}

void func_8002AAAC(void) {
    D_80064414 = 1;
}

void func_8002AABC(void) {
    D_8006557C = 0;
    SsInit();
    SsSetTableSize(D_800654CC, 1, 1);
    SpuReserveReverbWorkArea(1);
    SsUtSetReverbType(7);
    SsUtSetReverbDepth(100, 0);
    func_800369FC();
    func_8002AAA0();
    D_800643F8 = (void (*)())SsSetTickCallback(func_8002ABA8);
    SsSetTickMode(1);
    SsStart();
}

void func_8002AB44(void) {
    if (D_800643F8 != 0) {
        func_8002C428();
        SsUtFlush();
        SsEnd();
        SsSetTickCallback(D_800643F8);
        func_800366FC();
        VSync(3);
        D_800643F8 = 0;
    }
}

void func_8002ABA8(void) {
    s32 temp_v0;

    D_800654AC += 1;
    if (D_800654B0 > 0) {
        temp_v0 = D_800654B0 - 1;
        D_800654B0 = temp_v0;
        if (temp_v0 == 0) {
            D_800654B4 += 1;
        }
    }
    if (D_80064414 == 0) {
        func_8002B4B8();
    }
}

void func_8002AC18(void) {
    func_8002C0E0();
    func_800227C0();
}

void func_8002AC40(void) {
    func_8002C11C();
    func_800227FC();
    func_80027A9C();
}

void func_8002AC70(void) {
    func_8002C428();
    func_80022504();
    func_80027A9C();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002ACA0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002ADD4);

void func_8002AEE0(void) {
    MATRIX *m;

    if (D_800654C0 != NULL || D_800654C4 != NULL) {
        if (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
            D_800654C4->id = D_800654C8;
            func_800112A4(D_800654C0, D_800654C0->head, D_8004E0F0);
            func_8002ACA0(D_800654C4->id, 0);
            D_800654C0 = NULL;
            D_800654C4 = NULL;
        }
    }
    SpuGetAllKeysStatus(D_800644A8);
    func_80014540(D_80051640, D_8005B698);
    m = (MATRIX *)(D_80051640 + 0x74);
    gte_SetRotMatrix(m);
    gte_SetGeomScreen(*(s32 *)(D_80051640 + 0x18));
    gte_SetTransMatrix(m);
    gte_SetGeomOffset(D_8005B698->w / 2, 175);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B020);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B210);

void func_8002B4B8(void) {
    SsSeqCalledTbyT();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B4D8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B52C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B5B0);

s32 func_8002B6D0(VECTOR *pos, View *view) {
    VECTOR *out = &D_800569CC.out;
    s32 sxy;
    s32 *psxy = &sxy;
    SVECTOR *in = &D_800569CC.in;

    in->vx = pos->vx - view->pos.vx;
    in->vy = pos->vy - view->pos.vy;
    in->vz = pos->vz - view->pos.vz;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(psxy);
    return func_8002B020(out, D_80064478);
}

void func_8002B760(VECTOR *pos, View *view, VECTOR *out, s32 *sxy) {
    D_800569CC.in.vx = pos->vx - view->pos.vx;
    D_800569CC.in.vy = pos->vy - view->pos.vy;
    D_800569CC.in.vz = pos->vz - view->pos.vz;
    gte_ldv0(&D_800569CC.in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(sxy);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B7C8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B8F4);

void func_8002B96C(void) {
    SpuIsTransferCompleted(1);
    D_800654B8 = 0;
    func_800373FC(0);
    SsUtAllKeyOff(0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B9A4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BA10);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BA44);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BAE8);

s32 func_8002BB80(void) {
    if ((D_800654C0 != 0) || (D_800654C4 != 0)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BBB4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BBEC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BCB0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BD0C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BD68);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BD98);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BEA8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C060);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C090);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C0E0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C11C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C158);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C18C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C1C0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C248);

void func_8002C284(s32 arg0) {
    D_8006559C = arg0;
}

void func_8002C290(void) {
    D_800655AC = 0;
}

void func_8002C29C(void) {
    D_800655AC = 1;
}

void func_8002C2AC(void) {
    D_800655A8 += 1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C2C4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C394);

void func_8002C3F0(s32 arg0, s32 *arg1) {
    *arg1 = arg0;
    func_8002C2C4(arg1);
    D_800655A8 = 0;
    D_800655AC = 0;
    D_800655B0 = 0;
}

void func_8002C428(void) {
    func_8002C090(&D_8006557C);
}

void func_8002C44C(void) {
    if (D_8006557C != 0) {
        func_800110A8(D_8006557C, D_8004E0F0);
        D_8006557C = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C488);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C4B0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C5BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C894);

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004CD8C);

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004CDAC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C97C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D094);

void func_8002D0C8(void) {
    s32 flags;
    s32 on;

    flags = D_800655C0;
    if ((flags & 2) && (flags & 0x30)) {
        on = TRUE;
        D_800675C8 = on;
        D_800655C4 = on;
        D_800655C0 = flags | 0x80;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D108);

void func_8002D134(s32 arg0) {
    D_800675C8 = 1;
    D_800655C4 = arg0;
    D_800655C0 |= 0x80;
}

void func_8002D15C(void) {
    D_800655C4 = 0;
    D_800675CC = 0;
    D_800655C0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D178);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D1D4);

void func_8002D254(void) {
    MemCardStop();
}

void func_8002D274(void) {
    func_8002C97C();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D294);

void func_8002D33C(u8 *arg0) {
    (*(s8*)((u8*)(arg0) + 0)) = 0;
    (*(s8*)((u8*)(arg0) + 1)) = 0;
    (*(s8*)((u8*)(arg0) + 2)) = 0;
    (*(s8*)((u8*)(arg0) + 3)) = 0;
    (*(s8*)((u8*)(arg0) + 4)) = 0;
    (*(s8*)((u8*)(arg0) + 5)) = 0;
    (*(s8*)((u8*)(arg0) + 6)) = 0;
    (*(s8*)((u8*)(arg0) + 7)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D360);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D394);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D488);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D4F4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002DE2C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E000);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E0F0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E25C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E3D0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E584);

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004CE5C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E618);
