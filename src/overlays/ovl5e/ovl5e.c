#include "common.h"
#include "bash/main.h"
#include "bash/ovl5e.h"
#include "bash/papu.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B35AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B36D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B38FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B3B08);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B3D18);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B3E80);

void func_800B4154(u16 arg0) {
    s32 i;
    PapuEntry *p;

    for (i = 0; i < 20; i++) {
        p = &D_800BE648[i];
        if (p->unk0 != 0) {
            s16 v = D_800BD6F0[arg0][i];
            p->unkE = 4;
            p->unkA = v;
            p->unkC = 0x60;
            if (abs(p->unkA) >= p->unkC) {
                p->unkE = 0;
            }
        }
    }
}

void func_800B4204(void) {
    PapuEntry *e;
    Ovl5eTile *t;
    s32 i;
    s32 v;
    s32 a;
    s32 c;

    e = D_800BE648;
    i = 0;
    do {
        e = &D_800BE648[i];
        if ((u16)e->unk6 != 0) {
            t = (Ovl5eTile *)e->unk0;
            v = e->unkA;
            c = e->unkC;
            v += e->unkE;
            if (c < v) {
                v = c;
            }
            e->unkA = v;
            a = t->unk8 + v;
            if (a < -0xE00) {
                e->unk6 = 0;
                t->unk0 &= 0xFFFF7FFF;
            }
            if (e->unk8 < a) {
                a = e->unk8;
                e->unkA = 0;
                e->unkC = 0;
                e->unkE = 0;
                e->unk6 = 0;
            }
            t->unk8 = a;
        }
        i++;
    } while (i < 20);
}

void func_800B42C0(void) {
    s32 i;
    Slot40 *p;

    for (i = 0; i < 6; i++) {
        p = &D_800BD70C[i];
        p->unk0 = 0;
        p->pad4[0] = 0;
        p->pad4[1] = 0;
        p->pad4[2] = 0;
        p->pad4[3] = 0;
        p->pad4[4] = 0;
        p->pad4[5] = 0;
        p->pad4[6] = 0;
        p->unk20 = 0;
        p->unk22 = i + 0x14;
        p->unk24 = 0;
        p->unk26 = 0x14;
        p->unk28 = 0;
        p->unk2A = 0;
        p->unk2C = 0;
        p->unk2E = 0;
        p->unk30 = 0;
        p->unk32 = 0;
        p->unk34[0] = 0;
        p->unk34[1] = 0;
        p->unk34[2] = 0;
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5e/ovl5e", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", InitPapuBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", PlotPapuBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4B9C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4D70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4EE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", PlotPapuMinion);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B5E44);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6008);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B61A0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6334);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B64AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B668C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6860);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6D14);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6FF8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7134);

s32 func_800B744C(s32 arg0) {
    EXTengine *eng;
    s32 ix;
    s32 iz;
    s32 y;
    Ovl5eTile *t;

    eng = Heroes[arg0].hero_eng;
    ix = (eng->unk10.x + 0x700) >> 9;
    iz = (eng->unk10.z + 0x700) >> 9;
    if ((u32)ix < 7U && (u32)iz < 7U) {
        y = eng->unk10.y - D_8005A5D8;
        t = D_800BD948[ix][iz].tile;
        if (t != NULL) {
            y -= t->unk8;
        }
        if (abs(y) < 0x20 && (D_800BD948[ix][iz + 1].unkA & 0x100)) {
            return 1;
        }
    }
    return 0;
}

s32 func_800B7558(EXTengine *eng) {
    s32 y;
    s32 ix;
    s32 iz;
    Ovl5eTile *t;

    y = eng->unk10.y - D_8005A5D8;
    ix = (eng->unk10.x + 0x700) >> 9;
    iz = (eng->unk10.z + 0x700) >> 9;
    if ((u32)ix >= 7U || (u32)iz >= 7U) {
        return y - 0x800;
    }
    t = D_800BD948[ix][iz].tile;
    if (t != NULL) {
        y -= t->unk8;
    }
    return func_80025E00(eng, y);
}

void func_800B7600(s32 arg0) {
    Ovl5eKey *src;
    Ovl5eKey *dst;

    src = D_800BD008[arg0];
    D_800BE82C = src;
    if (src->b != 0) {
        dst = src;
        do {
            dst->d = src->a;
            dst->e = dst->b;
            dst->f = dst->c;
            dst->g = 1;
            dst++;
            src++;
        } while (dst->b != 0);
    }
    D_800BE894 = 2;
}

s32 func_800B7670(u8 *arg0) {
    s32 temp_a0;
    s32 temp_v1;
    s32 var_a2;

    var_a2 = 0;
    if ((*(s32*)((u8*)(arg0) + 144)) & 0x40) {
        temp_a0 = 0x6A0 - (*(s16*)((u8*)(arg0) + 138));
        if (temp_a0 < (*(s32*)((u8*)(arg0) + 16))) {
            (*(s32*)((u8*)(arg0) + 16)) = temp_a0;
            var_a2 = 1;
        }
        temp_v1 = -temp_a0;
        if ((*(s32*)((u8*)(arg0) + 16)) < temp_v1) {
            (*(s32*)((u8*)(arg0) + 16)) = temp_v1;
            var_a2 = 1;
        }
        if (temp_a0 < (*(s32*)((u8*)(arg0) + 24))) {
            (*(s32*)((u8*)(arg0) + 24)) = temp_a0;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < temp_v1) {
            (*(s32*)((u8*)(arg0) + 24)) = temp_v1;
            goto block_17;
        }
    } else {
        if ((*(s32*)((u8*)(arg0) + 16)) >= 0x601) {
            (*(s32*)((u8*)(arg0) + 16)) = 0x600;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 16)) < -0x600) {
            (*(s32*)((u8*)(arg0) + 16)) = -0x600;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) >= 0x601) {
            (*(s32*)((u8*)(arg0) + 24)) = 0x600;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < -0x600) {
            (*(s32*)((u8*)(arg0) + 24)) = -0x600;
block_17:
            var_a2 = 1;
        }
    }
    return var_a2;
}

void func_800B7778(void) {
    u8 *temp_v1;
    u8 *var_a0;

    var_a0 = D_8005AA24;
    if (var_a0 != NULL) {
        do {
            temp_v1 = (*(s32*)((u8*)(var_a0) + 88));
            if (temp_v1 != NULL) {
                if ((*(u16*)((u8*)(temp_v1) + 26)) == 1) {
                    (*(u16*)((u8*)(temp_v1) + 26)) = 4U;
                } else {
                    (*(u16*)((u8*)(temp_v1) + 26)) = 5U;
                }
            }
            var_a0 = (*(s32*)((u8*)(var_a0) + 44));
        } while (var_a0 != NULL);
    }
}

s32 func_800B77D8(void) {
    s32 a;

    if (*D_8005A680 != 0) {
        a = Random(2);
        if (Heroes[a].hero_status & 0xFA00) {
            a = Heroes[a].hero_teammate;
        }
        if (a == -1) {
            return -1;
        }
    } else {
        a = 0;
    }
    if (Heroes[a].hero_status & 0xFA00) {
        a = -1;
    }
    return a;
}

void func_800B7894(u16 arg0) {
    Ovl5eStep *s = D_800BD118[arg0];

    while (s->d >= 0) {
        func_800B4EE8(s->a, s->b, s->c, s->d);
        s++;
    }
}

void func_800B790C(s32 arg0) {
    if (func_800B744C(arg0) != 0) {
        func_80084E04(arg0, 0, -1, 3, 0);
    }
}

s32 func_800B794C(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (D_800BD70C[i].unk0 == 0) {
            return i;
        }
    }
    return -1;
}

void PlotPapuColumn(NAS *nas) {
    EXTengine *e = nas->Engine;
    EXTengine *other;
    s32 lim;
    s32 d;
    s32 cur;

    if (e != NULL && nas->UserInfo.user1 != 0) {
        cur = e->unk10.y + 0x140;
        e->unk10.y = cur;
        lim = D_8005B2EC;
        if (nas->Family.Parent != NULL) {
            other = nas->Family.Parent->Engine;
            if (other != NULL) {
                lim = other->unk10.y;
                d = lim - cur;
                if (d < 0) {
                    d = 0;
                }
                if (d < 0x200) {
                    other->unk2C.y = d * 8;
                    D_8009D6DC[2] |= 0x2000;
                }
            }
        }
        if (lim < e->unk10.y) {
            PlaySfx(0x20C, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009D6DC[3] = 0;
            D_8009D6DC[2] |= 0x8000;
            e->unk10.y = lim;
            nas->UserInfo.user1 = 0;
        }
        e->unk5E = 1;
    }
}

void InitPapuColumn(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = *(GfxModel **)D_80058AD0;
        e->sub->unk74 = 0x200C;
        e->unk5D = 0;
        e->unk5C = 1;
    }
    nas->UserInfo.user1 = 1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800B7B10(void) {
    s32 i;
    PapuEntry *p;
    s16 one;

    i = 0;
    one = 1;
    for (p = D_800BE648; i < 20; i++, p++) {
        if (p->unk0 != 0) {
            p->unk6 = one;
        }
    }
}

void func_800B7B4C(void) {
    s32 i, j;

    for (j = 2; j < 18; j++) {
        for (i = 2; i < 18; i++) {
            if (D_800BD948[(j - 2) >> 1][7 - ((i - 2) >> 1)].unkA == 0) {
                D_8009D90C[i * 20 + j].unk0 = 0xFF;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7BE4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7CBC);

void func_800B7D94(void) {
}

void func_800B7D9C(void) {
}

void func_800B7DA4(void) {
    func_80086F78();
    func_800B80B0();
    func_80086D88();
    func_800B3B08();
    func_800B7134();
}

void func_800B7DE4(void) {
    D_8005AA70[0] = (s32)func_800B35AC;
    D_8005AA70[1] = (s32)func_800B36D4;
    D_8005AA70[2] = (s32)func_800B36D4;
    D_8005AA70[3] = (s32)func_800B7DA4;
    D_8005AA70[5] = (s32)func_800B7D94;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[9] = (s32)func_800B7558;
}

void func_800B7E38(void) {
    D_8009E464 = Ovl5eASTIndex(0x203);
    D_8009E468 = Ovl5eASTIndex(0x204);
    D_8009E488.unk4 = D_8009E3B4;
    D_8009E488.unk8 = NULL;
    D_8009E488.unkC = 0;
    D_8009E488.unk0 = D_8009E3DC;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B7F14);

s32 func_800B8080(T_B8080 *p) {
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B80B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B82DC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B8508);
