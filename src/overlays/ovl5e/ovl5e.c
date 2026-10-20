#include "common.h"
#include "bash/main.h"
#include "bash/ovl5e.h"
#include "bash/papu.h"

void func_800B35AC(void) {
    EngineGameView *g;
    u8 *v;
    void **p;

    D_8009D6E8.unk68 = func_800B7670;
    D_8009D6E8.unk4C = func_800BB3F0;
    D_8009D6E8.unk50 = func_800BB69C;
    D_8009D6E8.unk54 = func_800BB86C;
    D_8009D6E8.unk58 = func_800BC274;
    func_800B7E38();
    D_8005AA94 = func_800B7558;
    D_8009D6E8.unk28 = func_800B790C;
    g = (EngineGameView *)D_8005A614;
    g->flags |= 0x900;
    if (*(s32 *)(D_8005A614 + 0x6C) == 0) {
        D_8005A614[0x51] = 9;
    }
    D_8005A614[0x52] = 9;
    D_8005A614[0x53] = 9;
    D_8009E0D8 = 0x15;
    D_8009E0DA = 0x16;
    D_8009E0DC = 0x17;
    D_8009E0DE = 0x18;
    D_8009E0E0 = 0x19;
    D_8009E0E2 = 0x1A;
    D_8009E0E4 = 0x1B;
    p = &D_8009D8FC;
    if (*(s32 *)(D_8005A614 + 0x6C) != 0) {
        v = D_800BC8DC;
    } else {
        v = D_800BC89C;
    }
    *p = v;
}

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

void InitPapuBoss(NAS *nas) {
    EXTengine *eng;
    ScriptCursor *cur;

    eng = nas->Engine;
    if (eng != NULL) {
        func_80086868();
        eng->sub->unk6C = D_800533F4;
        eng->sub->unk74 = 0x2001;
        eng->unk5C = 1;
        eng->unk5E = 1;
        func_80025B10(eng, 0x40);
        cur = func_80024E30(nas, &D_800BD3AC);
        func_800250B0(nas);
        D_800BE88E = 0;
        D_800BE890 = 0;
        D_800BE894 = 0;
        func_80026A04((u8 *)nas);
        D_800BD88C.nas = nas;
        D_800BD88C.eng = eng;
        D_800BD88C.sub = eng->sub;
        D_800BD88C.unk22 = 3;
        D_800BD88C.cursor = cur;
        D_800BD88C.unkC = 0;
        D_800BD88C.unk10 = 0;
        D_800BD88C.unk14 = 0;
        D_800BD88C.unk1C = 0;
        D_800BD88C.unk20 = 0;
        D_800BD88C.unk24 = 0;
        D_800BD88C.unk26 = 0x14;
        D_800BD88C.unk28 = 0;
        D_800BD88C.unk2A = 0;
        D_800BD88C.unk2C = 0;
        D_800BD88C.unk2E = 0;
        D_800BD88C.unk30 = 0;
        D_800BD88C.unk32 = 0;
        D_800BD88C.unk34 = 0;
        D_800BD88C.unk38 = 0;
        D_800BD88C.unk3C = 0;
    }
    if (nas->Failure != 0) {
        printf("boss failure!!!\n");
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", PlotPapuBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4B9C);

s32 func_800B4D70(Ovl5ePos *arg0) {
    s32 best;
    s32 bestScore;
    s32 i;
    s32 j;
    s32 r;
    s32 dx;
    s32 dz;
    s32 score;

    best = Random(5);
    bestScore = 0x90000;
    r = (0x20 - D_8009D88E) * 16;
    for (i = 0; i < 5; i++) {
        dx = D_800BD500[i].x - arg0->x;
        dz = D_800BD500[i].z - arg0->z;
        dx += Random(r);
        dz += Random(r);
        score = dx * dx + dz * dz;
        for (j = 0; j < 4; j++) {
            if (!(Heroes[i].hero_status & 0xFA00)) {
                EXTengine *e = Heroes[i].hero_eng;
                dx = D_800BD500[i].x - e->unk10.x;
                dz = D_800BD500[i].x - e->unk10.z;
                if (dx * dx + dz * dz <= 0x8FFFF) {
                    score = 0;
                }
            }
        }
        if (bestScore < score) {
            bestScore = score;
            best = i;
        }
    }
    return best;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B4EE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", PlotPapuMinion);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B5E44);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6008);

s32 func_800B61A0(EXTengine *eng) {
    VECTOR pos;
    FlagRecord *box;
    FlagRecord *rec;
    s32 i;
    s32 r;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 d;
    s32 lo;
    s32 hi;

    box = func_80025A48(eng, 0x400);
    if (box != NULL) {
        if (eng->unk5E != 0) {
            func_800142D4(&eng->unk1C);
        }
        func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
        r = box->radius;
        top = eng->unk10.y + box->unk2;
        bot = eng->unk10.y + box->height;
        func_80015758((SVECTOR *)box, &pos);
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_status & 0xF800) {
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
            lo = eng->unk10.y + rec->unk2;
            hi = eng->unk10.y + rec->height;
            if (bot < lo && hi < top) {
                dx = eng->unk10.x - pos.vx;
                dz = eng->unk10.z - pos.vz;
                d = dx * dx + dz * dz;
                if (d < (rec->radius + r) * (rec->radius + r)) {
                    func_80084E04(i, NULL, -1, 0x8008, 0x446);
                }
            }
        }
    }
    return -1;
}

s32 func_800B6334(EXTengine *eng, u32 flag) {
    VECTOR pos;
    FlagRecord *box;
    FlagRecord *rec;
    s32 i;
    s32 r;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 d;
    s32 lo;
    s32 hi;

    box = func_80025A48(eng, flag);
    if (box == NULL) {
        return -1;
    }
    if (eng->unk5E != 0) {
        func_800142D4(&eng->unk1C);
    }
    func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
    r = box->radius;
    top = eng->unk10.y + box->unk2;
    bot = eng->unk10.y + box->height;
    func_80015758((SVECTOR *)box, &pos);
    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_status & 0xF800) {
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
        lo = eng->unk10.y + rec->unk2;
        hi = eng->unk10.y + rec->height;
        if (bot < lo && hi < top) {
            dx = eng->unk10.x - pos.vx;
            dz = eng->unk10.z - pos.vz;
            d = dx * dx + dz * dz;
            if (d < (rec->radius + r) * (rec->radius + r)) {
                return i;
            }
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B64AC);

NAS *func_800B668C(EXTengine *eng, s32 mask) {
    VECTOR pos;
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
    if (eng->unk5E != 0) {
        func_800142D4(&eng->unk1C);
    }
    func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
    r = box->radius;
    top = eng->unk10.y + box->unk2;
    bot = eng->unk10.y + box->height;
    func_80015758((SVECTOR *)box, &pos);
    for (nas = D_8005AA2C; nas != NULL; nas = nas->Solid.Child) {
        if (nas->Class & 0x200) {
            e = nas->Engine;
            if (e != NULL && e != eng) {
                rec = func_80025A48(e, 0x40);
                if (rec != NULL) {
                    lo = e->unk10.y + rec->unk2;
                    hi = e->unk10.y + rec->height;
                    if (bot < lo && hi < top) {
                        dx = e->unk10.x - pos.vx;
                        dz = e->unk10.z - pos.vz;
                        rr = rec->radius + r;
                        if (dx * dx + dz * dz < rr * rr) {
                            obj = FindExtension(nas, 0x1000);
                            if (obj != NULL && (obj->can_do & mask)) {
                                obj->player = 3;
                                obj->will_do |= mask;
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6860);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5e/ovl5e", func_800B6D14);

void func_800B6FF8(void) {
    Ovl5eKey *k;
    s32 first;
    s32 v;
    s32 t;

    first = 1;
    if (D_800BE82C != NULL && D_800BE82C->b != 0) {
        k = D_800BE82C;
        do {
            if (k->g != 0) {
                t = k->f;
                if (t != 0) {
                    v = t - 1;
                } else {
                    if (first) {
                        PlaySfx(0x1F6, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                    first = 0;
                    if (k->g != 0) {
                        D_800BD948[(s16)k->d][(s16)k->e].unkE = 1;
                    }
                    k->d += k->pad8[0];
                    k->e += k->pad8[1];
                    if (k->d >= 7U) {
                        k->g = 0;
                    }
                    if ((u16)(k->e - 1) >= 7U) {
                        k->g = 0;
                    }
                    v = k->pad4;
                }
                k->f = v;
            }
            k++;
        } while (k->b != 0);
    }
}

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

void func_800B7BE4(void)
{
  s32 i;
  s32 *p;
  s32 a;
  s32 b;
  s32 type;
  s32 n;
  short t;
  n = D_800BD8E4;
  for (i = 0;; i++)
  {
    if (i >= n)
    {
      goto done;
    }
    p = (s32 *) func_800B3E80(D_800BD8E2);
    if (p != 0)
    {
      a = p[0];
      b = p[1];
      t = D_800BD908[D_800BD8E0];
      D_800BD8E0 = (D_800BD8E0 + 1) & 0x3F;
      type = t;
      if ((type == 6) && (D_8009E46C[6] >= 4))
      {
        type = 4;
      }
      func_800B82DC(a, 0, b, type, -1);
    }
  }

  done:
  ;

  ;
}

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

void func_800B7F14(T_B7F14_A *a, T_B7F14_B *b) {
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

void func_800B8508(NAS *arg0) {
    EXTengine *eng;
    Ovl5eObj *obj;
    NAS *nas;
    EXTengine *other;
    s32 again;
    s32 x, z;

    eng = arg0->Engine;
    if (eng != NULL) {
        obj = (Ovl5eObj *)AddObject(arg0);
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
            case 4:
                eng->sub->unk74 = 0x2013;
                break;
            case 5:
                eng->sub->unk74 = 0x2009;
                obj->unk22 = 0x5A;
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
