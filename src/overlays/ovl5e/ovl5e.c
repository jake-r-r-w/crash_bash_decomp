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

void func_800B36D4(void) {
    EXTheromove *move;
    s32 i;
    s16 angle;

    D_8005A5D8 = 0;
    D_800BD8DC = 0;
    D_800BD8DE = 0;
    *(s16 *)D_800BD8E2 = 0;
    D_800BD708 = 0;
    D_800BD8D4 = 0;
    D_800BD8D8 = 0;
    D_800BE828 = 0;
    D_800BE82A = 0;
    D_800BD8CC = 0;
    D_800BD8CE = 0;
    D_800BD8D0 = (Random(10) + 10) * 30;
    D_800BE788 = EngineTemplate;
    func_800B3D18();
    func_80086D88();
    for (i = 0; i < 4; i++) {
        angle = get_angle(Heroes[i].hero_eng->unk10.x, Heroes[i].hero_eng->unk10.z);
        move = Heroes[i].hero_move;
        Heroes[i].hero_eng->unk1E = angle;
        move->facing_angle = angle;
        move->forward_angle = angle;
        Heroes[i].hero_ctrl |= 8;
        if (D_8005A614[0x51] == 9) {
            Heroes[i].hero_shadow->flags = 0;
        }
    }
    func_8007EB04();
    func_8007E9FC(0x2C, 0x300A);
    func_8007C6EC();
    func_800B42C0();
    func_800B6D14();
    SpawnNASById(D_8005B2B8[2].pos.x, D_8005B2B8[2].pos.y, D_8005B2B8[2].pos.z,
                 0x1201, 0x11);
    func_800B38FC(0, 0, 0, 0, 0xC, 4, 0, 0, 0);
    func_800B7BE4();
    D_8009E494 = (void (*)(s32, s32, s32, s32))func_800B5E44;
    D_800BE830 = 0;
    D_800BE832 = 0;
}

void func_800B38FC(u16 arg0, s32 a1, s32 a2, s32 a3, u16 n4, u16 n5, u16 n6, u16 n7, u16 n8) {
    s32 t;
    s32 i;
    s32 j;
    s32 k;
    u16 *p;
    u16 *e;
    u8 *a;
    u8 *b;
    u8 tmp;
    s32 x;

    x = arg0 * n6;
    p = D_800BD8F0;
    t = x / 16;
    k = t + 1;
    p[3] = k;
    p[2] = k;
    p[1] = k;
    do {
    } while (0);
    n4 *= 4;
    n5 *= 4;
    n6 *= 4;
    n7 *= 4;
    n8 *= 4;
    do {
    } while (0);
    i = 0;
    D_800BD8E4 = arg0;
    e = &D_800BD8E8;
    do {
    } while (0);
    p[0] = k;
    e[3] = arg0;
    e[2] = arg0;
    e[1] = arg0;
    D_800BD8E8 = arg0;
    do {
    } while (0);
    for (j = i; j < n4; j++) {
        D_800BD908[i++] = 4;
    }
    for (j = 0; j < n5; j++) {
        D_800BD908[i++] = 5;
    }
    for (j = 0; j < n6; j++) {
        D_800BD908[i++] = 6;
    }
    for (j = 0; j < n7; j++) {
        D_800BD908[i++] = 1;
    }
    x = n8;
    for (j = 0; j < x; j++) {
        D_800BD908[i++] = 2;
    }
    j = 0;
    do {
        i = Random(0x40);
        k = Random(0x40);
        a = &D_800BD908[i];
        b = &D_800BD908[k];
        tmp = *a;
        j++;
        *a = *b;
        *b = tmp;
    } while (j < 0x258);
    D_800BD8E0 = Random(0x20) + 0x14;
}

void func_800B3B08(void) {
    Ovl5eGridPos *p;
    s32 a1;
    s32 y;

    if (D_8005A644 & 0x801) {
        return;
    }
    if (D_800BD8DC == 0) {
        if (D_8009E46C[0] < D_800BD8E8) {
            D_800BD8DC = Random(15) + 15;
            func_800B7CBC(1);
        } else {
            D_800BD8DC = Random(40) + 5;
        }
    } else {
        D_800BD8DC = D_800BD8DC - 1;
    }
    a1 = D_800BC95C[D_8005A6C3];
    if (a1 != 0) {
        if (D_800BD8D0 != 0) {
            D_800BD8D0 -= 1;
        }
    } else {
        D_800BD8D0 = 99;
        D_800BD8CC = 0;
    }
    if (D_800BD8CC != 0) {
        if (D_800BD8CE != 0) {
            D_800BD8CE -= 1;
            return;
        }
        p = &D_800BDE48[0][0];
        p += Random(256);
        a1 = p->x;
        a1 >>= 8;
        a1 += 10;
        y = 9 - (p->y >> 8);
        if (D_8009D90C[y * 20 + a1].unk0 < 16) {
            SpawnNASById(p->x, -0x800, p->y, 0x20C, 0x11);
            D_800BD8CC -= 1;
            D_800BD8CE = Random(4) + 2;
        }
    } else if (D_800BD8D0 == 0) {
        if (D_8009E25C[0] < a1) {
            D_800BD8CC = a1;
            D_800BD8D0 = (Random(10) + 10) * 30;
            return;
        }
        D_800BD8D0 = 60;
    }
}

void func_800B3D18(void) {
    s32 in, x, y;
    s32 j, i, n, k;
    Ovl5eGridPos *a, *b;

    i = 0;
    do {
        y = (i << 8) - 0x780;
        j = 0;
        do {
            x = (j << 8) - 0x780;
            if ((u32)(i - 3) >= 8U) {
                y = (Random(9) << 8) - 0x480;
            }
            if ((u32)(j - 4) >= 8U) {
                x = (Random(9) << 8) - 0x380;
            }
            D_800BDE48[i][j].x = x;
            D_800BDE48[i][j].y = y;
        } while (++j < 16);
    } while (++i < 16);
    for (n = 0; n < 600; n++) {
        i = Random(16);
        j = Random(16);
        in = Random(16);
        k = Random(16);
        a = (Ovl5eGridPos *)((u8 *)D_800BDE48 + (j * 8 + i * 128));
        b = (Ovl5eGridPos *)((u8 *)D_800BDE48 + (k * 8 + in * 128));
        x = a->x;
        y = a->y;
        a->x = b->x;
        a->y = b->y;
        b->x = x;
        b->y = y;
    }
}

Ovl5eMsg *func_800B3E80(char *arg0) {
    Ovl5eGridPos *best;
    Ovl5eGridPos *cell;
    u16 bestn;
    u16 tries;
    u16 besti;
    s32 i;
    s32 x;
    s32 z;
    s32 xlo;
    s32 xhi;
    s32 zlo;
    s32 zhi;
    u16 n;

    cell = D_800BDE48[0];
    bestn = 9;
    tries = 9;
    i = *(u16 *)arg0;
    best = cell = &cell[i];
    besti = i;
    i++;
    i &= 0xFF;
    x = (cell->x >> 8) + 10;
    z = 9 - (cell->y >> 8);
    do {
        while (D_8009D90C[z * 20 + x].unk0 != 0) {
            if (i == *(u16 *)arg0) {
                return NULL;
            }
            cell = D_800BDE48[0];
            cell = &cell[i];
            i++;
            i &= 0xFF;
            x = (cell->x >> 8) + 10;
            z = 9 - (cell->y >> 8);
        }
        xhi = x;
        xlo = x;
        zhi = z;
        zlo = z;
        if (x > 0) {
            xlo = x - 1;
        }
        if (x < 19) {
            xhi = x + 1;
        }
        if (z > 0) {
            zlo = z - 1;
        }
        if (z < 19) {
            zhi = z + 1;
        }
        n = D_8009D90C[zlo * 20 + xlo].unk0 != 0;
        if (D_8009D90C[zlo * 20 + x].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[zlo * 20 + xhi].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[z * 20 + xlo].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[z * 20 + xhi].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[zhi * 20 + xlo].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[zhi * 20 + x].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[zhi * 20 + xhi].unk0 != 0) {
            n++;
        }
        if (n == 0) {
            *(u16 *)arg0 = i;
            return (Ovl5eMsg *)cell;
        }
        if (n < bestn) {
            bestn = n;
            best = cell;
            besti = i;
        }
        if (i == *(u16 *)arg0) {
            return NULL;
        }
        cell = D_800BDE48[0];
        cell = &cell[i];
        i++;
        i &= 0xFF;
        x = (cell->x >> 8) + 10;
        z = 9 - (cell->y >> 8);
        if (--tries == 0xFFFF) {
            break;
        }
    } while (1);
    *(u16 *)arg0 = besti;
    return (Ovl5eMsg *)best;
}

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

void PlotPapuBoss(NAS *nas) {
    s32 frame;
    EXTanimator *anim;
    EXTengine *eng;
    s16 *p;
    s32 v;
    s32 i;
    s32 j;
    s32 empty;
    NAS *o;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    anim = FindExtension(nas, 0x400);
    frame = func_800250B0(nas);
    if (D_8009D6E0 & 0x200) {
        if (D_800BE88E != 0x1E) {
            D_800BE88E = 0x1E;
            func_80025FAC(anim, 0x1C);
        }
    }
    switch (D_800BE88E) {
    case 7:
        if (frame == 8) {
            D_800BE88E = 0;
        } else if (frame == 0xC) {
            PlaySfx(0x1F8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 8:
        switch (frame) {
        case 0xC:
            PlaySfx(0x1F8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        case 0xD:
            func_800B7894(D_800BE834);
            break;
        case 8:
            D_800BE88E = 0xB;
            break;
        }
        break;
    case 6:
        func_800B61A0(eng);
        if (frame == 8) {
            goto f8;
        }
        if (frame == 0xC) {
            PlaySfx(0x1F5, NULL, NULL, 0x1000, 0x1E00, 0);
            v = 0xB0;
            i = 0;
            p = D_800BE838;
            D_800BE894 = 1;
            D_800BE838[0] = 0x80;
            D_800BE888 = 0x300;
            for (; i < 0x27; i++) {
                if (v <= 0) {
                    break;
                }
                *p = v;
                v -= 0x10;
                p++;
            }
            func_800B7778();
            for (j = 0; j < 0x14; j++) {
                if (D_800BE648[j].unk0 != 0) {
                    D_800BE648[j].unk6 = 1;
                }
            }
        }
        break;
    f8:
        D_800BD8D8 = 1;
        D_800BE88E = 0;
        break;
    case 10:
        if (!(D_8005A624 & 0x04000000)) {
            D_800BE88E = 0;
        }
        break;
    case 12:
        if (D_800BD8B4 != 0) {
            D_800BE890 = D_800BE88E;
            if (D_800BD708 != 0) {
                D_800BE88E = 9;
                D_8009D6DC[3] -= 5;
                if (D_8009D6DC[3] < 0) {
                    D_8009D6DC[3] = 0;
                }
                func_80025FAC(anim, 0x1E);
            }
        }
        if (frame == 0xC) {
            PlaySfx(0x1F8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (D_800BE82A != 0) {
            D_800BE82A--;
        } else {
            D_800BE88E = 0;
        }
        break;
    case 11:
        empty = 1;
        for (i = 0; i < 6; i++) {
            if (D_800BD70C[i].unk0 != 0) {
                empty = 0;
            }
        }
        if (empty) {
            D_800BE88E = 0;
        }
        if (frame == 8) {
            empty = D_800BD2F4[Random(8)];
            ((void (*)(EXTanimator *, s32))func_80025FAC)(anim, empty);
        }
        break;
    case 9:
        D_800BD708 = 0;
    case 1:
        if (frame == 8) {
            D_800BE88E = 0;
        }
        break;
    case 20:
        if (frame == 0xC) {
            o = SpawnNASById(eng->unk10.x, eng->unk10.y - 0x800, eng->unk10.z, 0x1203, 0x11);
            if (o != NULL) {
                func_80026C38((u8 *)nas, (u8 *)o);
            }
        }
        break;
    case 30:
        break;
    default:
        D_800BE832 = D_800BD130[D_800BE830];
        D_800BE830++;
        D_800BE834 = D_800BD130[D_800BE830];
        D_800BE830++;
        if (D_800BE832 == 4) {
            if (D_800BD708 != 0) {
                D_800BE830 = D_800BE834 * 2;
                D_800BE832 = D_800BD130[D_800BE830];
                D_800BE830++;
                D_800BE834 = D_800BD130[D_800BE830];
                D_800BE830++;
            } else {
                D_800BE832 = D_800BD130[D_800BE830];
                D_800BE830++;
                D_800BE834 = D_800BD130[D_800BE830];
                D_800BE830++;
            }
        }
        switch (D_800BE832) {
        case 1:
            D_800BE88E = 0xA;
            func_80025FAC(anim, 0);
            break;
        case 9:
            D_800BD8D4 = 0;
            D_800BE88E = 0xC;
            D_800BD708 = 1;
            D_800BE82A = D_800BC91C[D_8005A6C3];
            if (D_800BE824 != NULL) {
                D_800BE824->flags |= 0x8000;
            }
            func_80025FAC(anim, 0x21);
            break;
        case 2:
            D_800BD708 = 0;
            if (D_800BE824 != NULL) {
                D_800BE824->flags &= ~0x8000;
            }
            D_800BE88E = 6;
            D_800BD8E8 = (&D_800BD8E8)[1] = 0;
            func_80025FAC(anim, 3);
            func_800B4154(D_800BE834);
            break;
        case 3:
            func_800B7600(D_800BE834);
            D_800BE88E = 1;
            func_80025FAC(anim, 0xA);
            break;
        case 5:
            D_800BD8DC = 0x1E;
            D_800BD8E8 = (&D_800BD8E8)[1] = 6;
            break;
        case 6:
            D_800BD8D4 = 1;
            func_800B42C0();
            D_800BE88E = 8;
            func_80025FAC(anim, 0x16);
            break;
        case 7:
            D_800BD8D4 = 1;
            func_800B42C0();
            D_800BE88E = 8;
            func_80025FAC(anim, 0x16);
            break;
        case 8:
            D_800BD8D4 = 1;
            D_800BE88E = 0xB;
            func_80025FAC(anim, 0);
            break;
        case 10:
            if (D_8009D6E0 & 0x200) {
                D_800BE88E = 0x1E;
                func_80025FAC(anim, 0x1C);
            } else {
                D_800BE88E = 0x14;
                func_80025FAC(anim, 0x19);
            }
            break;
        default:
            D_800BE88E = 1;
            func_80025FAC(anim, 0);
            break;
        }
        break;
    }
    eng->unk5E = 1;
    D_800BD8B4 = 0;
}

NAS *func_800B4B9C(s32 arg0) {
    NAS *best;
    NAS *n;
    s32 bestScore;
    s32 px;
    s32 pz;
    s32 r;
    s32 h;
    EXTengine *e;
    s32 dx;
    s32 dz;
    s32 score;
    s32 mul;
    s32 cnt;
    s32 i;
    T_B4B9CFux *f;
    NAS *o;

    best = NULL;
    bestScore = 0x7FFFFF;
    n = D_8005AA24;
    h = D_8009D88E;
    px = D_800BD70C[arg0].unk34[0];
    pz = px;
    if (n != NULL) {
        r = (0x20 - h) * 16;
        do {
            e = n->Engine;
            if (e != NULL && e->platform == NULL) {
                f = (T_B4B9CFux *)n->FUX;
                if (f != NULL && ((mul = 1, f->type == 4) || (f->type == 5 && (mul = 5))) && f->status == 1) {
                    dx = pz - e->unk10.x;
                    dz = px - e->unk10.z;
                    dx += Random(r);
                    dz += Random(r);
                    score = csqrt(dx * dx + dz * dz) >> 6;
                    score *= mul;
                    cnt = 0;
                    for (i = 0; i < 6; i++) {
                        o = (NAS *)D_800BD70C[i].unk0;
                        if (o != NULL && o->FUX != NULL && ((T_B4B9CFux *)o->FUX)->owner == n) {
                            cnt += 2;
                        }
                    }
                    if (cnt != 0) {
                        score <<= cnt;
                    }
                    if (score < bestScore) {
                        best = n;
                        bestScore = score;
                    }
                }
            }
            n = n->Object.Child;
        } while (n != NULL);
    }
    return best;
}

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

NAS *func_800B4EE8(s32 a, s32 b, s32 c, u16 d)
{
  NAS *nas;
  u8 *new_var;
  EXTengine *eng;
  Ovl5eObj *obj;
  Slot40 *s;
  s32 i;
  nas = SpawnNASById(a, b, c, 0x1202, 0x11);
  if (nas != 0)
  {
    eng = nas->Engine;
    if (eng != 0)
    {
      eng->sub->unk6C = &D_8005342C;
      eng->unk5D = 0;
      eng->sub->unk74 = 0x2001;
      eng->flags &= 0xFFFF7FFF;
      func_80025B10(eng, 0x50);
      for (i = 0; i < 6; i++)
      {
        if (D_800BD70C[i].unk0 == 0)
        {
          goto found;
        }
      }

      i = -1;
      found:
      if (i != (-1))
      {
        nas->UserInfo.user1 = i;
        new_var = (u8 *) D_800BD70C;
        s = (Slot40 *) (new_var + (i << 6));
        s->unk0 = (s32) nas;
        s->pad4[0] = (s32) eng;
        s->pad4[1] = (s32) eng->sub;
        s->pad4[4] = (s32) AddMotion(nas);
        *((SLONG_XYZ *) s->unk34) = eng->unk10;
        s->unk26 = 0xA;
        obj = (Ovl5eObj *) AddObject(nas);
        if (obj != 0)
        {
          nas->FUX = (EXT *) obj;
          obj->unk1A = 1;
          obj->unk22 = d;
          obj->unk14 = s->unk22;
          func_80026AD8((u8 *) nas);
          func_8002694C(nas, 0x200);
        }
        s->pad4[5] = (s32) func_80024E30(nas, &D_800BD4F4);
      }
      else
      {
        KillNAS(nas);
        return 0;
      }

    }
    if (nas->Failure != 0)
    {
      KillNAS(nas);
    }
  }
  return nas;
}

void PlotPapuMinion(NAS *nas)
{
  EXTengine *target_eng;
  s32 slot;
  s32 level;
  EXTengine *eng;
  MinionMove *move;
  EXTanimator *anim;
  MinionFux *fux;
  MinionFux *other;
  NAS *target;
  Slot40 *s;
  s32 dx;
  s32 dz;
  s32 i;
  s32 hit;
  s32 dist;
  s32 angle;
  s32 pick;
  s32 who;
  s32 frame;
  s32 step;
  u16 a;

  slot = nas->UserInfo.user1;
  eng = nas->Engine;
  level = D_8009D88E;
  if (eng == NULL) {
    return;
  }
  target_eng = NULL;
  dist = -1;
  angle = 0;
  move = FindExtension(nas, 0x800);
  if (move == NULL) {
    return;
  }
  anim = FindExtension(nas, 0x400);
  if (anim == NULL) {
    return;
  }
  fux = (MinionFux *)nas->FUX;
  if (fux == NULL) {
    return;
  }
  target = fux->target;
  hit = 0;
  if (target != NULL) {
    for (i = 0; i < 4; i++) {
      if (Heroes[i].hero == target) {
        if (!(Heroes[i].hero_status & 0xFA00)) {
          hit = 1;
        }
        break;
      }
    }
    if (i == 4) {
      pick = (s32)target->FUX;
      if (pick != 0 && ((MinionFux *)pick)->state == 1) {
        hit = 1;
      }
      if (target->Engine->platform != NULL) {
        hit = 0;
      }
    }
    if (hit) {
      target_eng = target->Engine;
      dx = target_eng->unk10.x - eng->unk10.x;
      dz = target_eng->unk10.z - eng->unk10.z;
      angle = get_angle(-dx, -dz);
      dist = csqrt(dx * dx + dz * dz) >> 6;
    }
  }
  if (!hit) {
    fux->target = NULL;
  }
  frame = func_800250B0(nas);
  switch (fux->state) {
  case 5:
    move->speed = 0;
    eng->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(eng);
    break;
  case 1:
    move->speed = 0;
    if (fux->unk22 == 0) {
      PlaySfx(0x1FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
      func_8007A524(0, &eng->unk10.x, (s32)D_8005342C);
      fux->unk22 = 5;
      fux->state = 2;
    } else {
      fux->unk22--;
    }
    break;
  case 2:
    move->speed = 0;
    if (fux->unk22 != 0) {
      fux->unk22--;
      break;
    }
    eng->flags |= 0x8021;
    fux->state = 3;
    move->unk1E = 0x50;
    move->unk18 = 0;
    move->unk1A = 6;
    move->unk1C = 0;
    move->unkC = 0;
    func_80025FAC(anim, 3);
    break;
  case 3:
    move->speed = 0;
    if (func_8007A26C(nas) != 0) {
      fux->unk1C |= 0x102;
      eng->flags &= ~0x21;
      fux->state = 4;
      func_80025FAC(anim, 4);
    }
    break;
  case 4:
    move->speed = 0;
    if (frame == 8) {
      fux->state = 0;
    }
    break;
  case 6:
    dx = ((s32 (*)(EXTengine *))func_80025F44)(eng);
    i = (s16)((s16)(((eng->unk1E -= 0x155) & 0xFFF) / 341) % 12);
    eng->unk10.y -= dx;
    if (frame == 8) {
      fux->state = 0;
      if (eng->sfx != NULL) {
        StopSfx(eng->sfx);
      }
      eng->unk5C = 0;
      eng->unk2C.x = 0x1000;
      eng->unk2C.y = 0x1000;
      eng->unk2C.z = 0x1000;
    } else {
      eng->unk5C = 1;
      eng->unk2C.x = D_8009DC38[i];
      eng->unk2C.y = D_8009DC50[i];
      eng->unk2C.z = D_8009DC38[i];
    }
    who = func_800B6334(eng, 0x400);
    if (who != -1) {
      if ((Heroes[who].hero_immunity | Heroes[who].hero_hit) == 0) {
        Heroes[who].hero_move->reactive_angle = angle;
        Heroes[who].hero_move->reactive_speed = 0x30;
        func_80084E04(who, NULL, -1, 0x8005, 0x446);
      }
    }
    func_800B64AC(eng, 2, 0x400, (u16)nas->UserInfo.user1);
    if (target_eng != NULL) {
      move->angle = angle;
    }
    move->speed = 0x20;
    break;
  case 7:
    eng->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(eng);
    hit = (s32)func_800B6008(slot, 0x180);
    if (hit != 0) {
      for (i = 0; i < 4; i++) {
        if (!(Heroes[i].hero_status & 0xFA00) &&
            func_80015480(eng->unk1E, get_angle(eng->unk10.x - Heroes[i].hero_eng->unk10.x,
                                                eng->unk10.z - Heroes[i].hero_eng->unk10.z)) < level * 4 + 0x1C0) {
          func_80025FAC(anim, 0x10);
          fux->state = 6;
          break;
        }
      }
      do {
        if (fux->state != 6) {
          other = (MinionFux *)((NAS *)hit)->FUX;
          if (other != NULL) {
            other->unk1E |= 1;
            other->unk16 = D_800BD70C[slot].unk22;
            fux->target = NULL;
            fux->state = 8;
            func_80025FAC(anim, 7);
          }
        }
      } while (0);
    } else if (target_eng == NULL) {
      fux->state = 0;
      func_80025FAC(anim, 0);
    } else {
      move->angle = angle;
      move->speed = 0x20;
    }
    break;
  case 8:
    eng->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(eng);
    move->speed = 0;
    if (D_800BD70C[slot].pad4[6] != 0) {
      if (frame == 8) {
        if (((MinionFux *)D_800BD70C[slot].pad4[6])->unk14 == 5) {
          fux->unk20 = 0x5A - Random(level + 0xA);
        } else {
          fux->unk20 = Random(level * 3 + 0x96) + 0xB4;
        }
        pick = 0;
        if (*D_8005A680 != 0) {
          pick = Random(2);
          if (Heroes[pick].hero_status & 0xFA00) {
            pick = Heroes[pick].hero_teammate;
          }
          if (pick == -1) {
            who = -1;
            goto have;
          }
        }
        if (Heroes[pick].hero_status & 0xFA00) {
          pick = -1;
        }
        who = pick;
      have:
        if (who != -1) {
          fux->target = Heroes[who].hero;
          fux->state = 9;
          func_80025FAC(anim, 9);
        }
      }
    } else {
      fux->unk1E |= 2;
    }
    break;
  case 9:
    eng->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(eng);
    if (D_800BD70C[slot].pad4[6] == 0) {
      fux->unk1E |= 2;
      break;
    }
    hit = (s32)((NAS **)D_800BD70C[slot].pad4[6])[-1];
    if (hit == 0 || ((NAS *)hit)->Engine->platform != NULL) {
      fux->target = NULL;
      fux->state = 0xA;
      func_80025FAC(anim, 0xA);
    }
    if (fux->unk20 != 0) {
      fux->unk20--;
      if (frame == 8 && dist < 0x600 &&
          func_80015480(angle, eng->unk1E) < (0x20 - level) * 4 + 0x40) {
        fux->target = NULL;
        fux->state = 0xA;
        func_80025FAC(anim, 0xA);
      }
      if (target_eng != NULL) {
        move->angle = angle;
        move->speed = 0x18;
        break;
      }
      fux->target = NULL;
      fux->state = 0xA;
      func_80025FAC(anim, 0xA);
      break;
    }
    fux->target = NULL;
    fux->state = 0xA;
    func_80025FAC(anim, 0xA);
    break;
  case 13:
    eng->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(eng);
    dx = D_800BD500[nas->UserInfo.user2].x - eng->unk10.x;
    dz = D_800BD500[nas->UserInfo.user2].z - eng->unk10.z;
    angle = get_angle(-dx, -dz);
    dist = csqrt(dx * dx + dz * dz) >> 6;
    if (dist < 0x200) {
      fux->state = 0;
    }
    move->angle = angle;
    move->speed = 0x20;
    break;
  case 10:
    eng->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(eng);
    move->speed = 0;
    if (frame == 8) {
      fux->state = 0;
    } else if (frame == 0x10) {
      if (D_800BD70C[slot].pad4[6] != 0) {
        ((MinionFux *)D_800BD70C[slot].pad4[6])->unk1E |= 4;
      }
      D_800BD70C[slot].pad4[6] = 0;
    }
    break;
  case 12:
    eng->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(eng);
    move->speed = 0;
    pick = (s32)D_800BD70C[slot].pad4[6];
    if (pick != 0) {
      ((MinionFux *)pick)->unk16 = fux->unk16;
    }
    D_800BD70C[slot].pad4[6] = 0;
    fux->state = 0xB;
    fux->unk1C &= 0xFEFD;
    fux->unk1E &= 0xFEFD;
    D_800BD70C[slot].unk26 = 0;
    func_80025FAC(anim, 0xD);
    break;
  case 11:
    eng->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(eng);
    move->speed = 0;
    D_800BD70C[slot].pad4[6] = 0;
    if (frame != 8) {
      if (frame == 9 && D_800BD70C[slot].unk26 != 0) {
        fux->state = 0;
      }
      break;
    }
    D_800BD70C[slot].unk0 = 0;
    PlaySfx(0x1FB, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    func_8007A524(0, &eng->unk10.x, (s32)D_8005342C);
    KillNAS(nas);
    return;
  default:
    eng->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(eng);
    fux->unk1C |= 0x102;
    move->speed = 0;
    move->push = 0;
    fux->target = func_800B4B9C(slot);
    if (fux->target != NULL) {
      func_80025FAC(anim, 1);
      fux->state = 7;
    } else {
      nas->UserInfo.user2 = func_800B4D70((Ovl5ePos *)eng);
      func_80025FAC(anim, 1);
      fux->state = 0xD;
    }
    break;
  }
  if (fux->state != 6) {
    s32 turn = func_800154A0(eng->unk1E, move->angle) >> 2;
    if (turn != 0) {
      eng->unk1E += turn;
    } else {
      eng->unk1E = move->angle;
    }
  }
  eng->unk10.x -= (move->speed * D_80068BD4[(move->angle - 0x400) & 0xFFF].unk2) >> 12;
  eng->unk10.z -= (move->speed * D_80068BD4[(u16)move->angle & 0xFFF].unk2) >> 12;
  if (move->push != 0) {
    eng->unk10.x -= (move->push * D_80068BD4[(move->push_angle - 0x400) & 0xFFF].unk2) >> 12;
    eng->unk10.z -= (move->push * D_80068BD4[(u16)move->push_angle & 0xFFF].unk2) >> 12;
    move->push -= 4;
    if ((s16)move->push < 0) {
      move->push = 0;
    }
  }
  func_800B6860(slot);
  func_800B7670((u8 *)eng);
  if (fux->unk1E & 0x102) {
    if (D_800BD70C[slot].pad4[6] != 0) {
      ((MinionFux *)D_800BD70C[slot].pad4[6])->unk16 = fux->unk16;
    }
    D_800BD70C[slot].pad4[6] = 0;
    move->push = 0x40;
    move->push_angle = (move->angle + 0x800) & 0xFFF;
    fux->state = 0xB;
    fux->unk1C &= 0xFEFD;
    fux->unk1E &= 0xFEFD;
    PlaySfx(D_800BD700[Random(4)], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    func_80025FAC(anim, 0xD);
  }
  eng->unk5E = 1;
}

void func_800B5E44(B5E44Owner *arg0, s32 arg1, s32 arg2)
{
  B5E44Pos *me;
  s32 i;
  s32 px;
  s32 pz;
  s32 dx;
  s32 dz;
  s32 d;
  B5E44Pos *new_var;
  s32 ang;
  B5E44Slot *s;
  B5E44Flag *f;
  arg2 -= 0x14;
  if (((u32) arg2) >= 7U)
  {
    arg2 = -1;
  }
  me = arg0->pos;
  if (me != 0)
  {
    px = me->x;
    pz = me->z;
    for (i = 0; i < 4; i++)
    {
      s = &((B5E44Slot *) D_800BD70C)[i];
      if ((arg2 != i) && ((&((B5E44Slot *) D_800BD70C)[i])->obj != 0))
      {
        f = s->obj->flag;
        if ((f != 0) && (f->flags & 2))
        {
          dx = s->pos->x - px;
          new_var = s->pos;
          dz = new_var->z - pz;
          d = csqrt((dx * dx) + (dz * dz)) >> 6;
          if (d < arg1)
          {
            if (d < 0x80)
            {
              ang = s->dyn->unk16;
            }
            else
            {
              ang = get_angle(dx, dz);
            }
            s->dyn->unk2A = ang ^ 0x800;
            d = (arg1 - d) >> 3;
            (&((B5E44Slot *) D_800BD70C)[i])->dyn->unk22 = d;
            (&((B5E44Slot *) D_800BD70C)[i])->dyn->unk16 = (&((B5E44Slot *) D_800BD70C)[i])->dyn->unk2A + 0x800;
            PlaySfx(D_800BD700[Random(4)], 0, (SLONG_XYZ *) (&me->x), 0x1000, 0x1E00, 0);
            f->unk1A = 0xC;
          }
        }
      }
    }

  }
}

void *func_800B6008(s32 arg0, s32 arg1) {
    s32 ang;
    u8 *n = D_8005AA24;
    s32 lim = 0x10000;
    Slot40 *b = D_800BD70C;
    Slot40 *s = &b[arg0];
    u8 *o = (u8*)s->pad4[0];
    u8 *a = (u8*)s->pad4[4];
    s32 px = *(s32*)(o + 0x10) - (D_80068BD4[(*(s16*)(a + 0x16) - 0x400) & 0xFFF].unk2 * 256 >> 12);
    s32 pz = *(s32*)(o + 0x18) - (D_80068BD4[*(u16*)(a + 0x16) & 0xFFF].unk2 * 256 >> 12);
    Slot40 *q;

    if (n != NULL) {
        ang = arg1 & 0xFFFF;
        q = s;
        do {
            u8 *f = *(u8**)(n + 0x58);
            if (f != NULL && (*(u16*)(f + 0x1C) & 1)) {
                u8 *e = *(u8**)(n + 0x54);
                if (e != NULL && *(s32*)(e + 0x80) == 0) {
                    s32 ex = *(s32*)(e + 0x10);
                    s32 ez = *(s32*)(e + 0x18);
                    s32 dx = ex - px;
                    s32 dz = ez - pz;
                    if (dx * dx + dz * dz < lim) {
                        if (ang == 0) {
                            return n;
                        }
                        if (func_80015480(*(s16*)((u8*)q->pad4[4] + 0x16),
                                          get_angle(ex - *(s32*)(o + 0x10), ez - *(s32*)(o + 0x18)) ^ 0x800) < ang) {
                            return n;
                        }
                    }
                }
            }
            n = *(u8**)(n + 0x2C);
        } while (n != NULL);
    }
    return NULL;
}

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

NAS *func_800B64AC(EXTengine *eng, u32 mask, s32 unused, s32 add) {
    VECTOR pos;
    FlagRecord *new_var;
    FlagRecord *box;
    FlagRecord *rec;
    NAS *n;
    u8 *x;
    s32 r;
    s32 top;
    s32 bot;
    s32 dx;
    s32 lo;
    EXTengine *o;
    s32 hi;

    box = func_80025A48(eng, 0x400);
    if (box != NULL) {
        if (eng->unk5E != 0) {
            func_800142D4(&eng->unk1C);
        }
        func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
        r = box->radius;
        new_var = box;
        top = eng->unk10.y + new_var->unk2;
        bot = eng->unk10.y + new_var->height;
        func_80015758((SVECTOR *)new_var, &pos);
        for (n = D_8005AA2C; n != NULL; n = n->Solid.Child) {
            if (!(n->Class & 0x200)) {
                continue;
            }
            o = n->Engine;
            if (o == NULL || o == eng) {
                continue;
            }
            rec = func_80025A48(o, 0x40);
            if (rec == NULL) {
                continue;
            }
            lo = o->unk10.y + rec->unk2;
            hi = o->unk10.y + rec->height;
            if (bot < lo && hi < top) {
                dx = o->unk10.x - pos.vx;
                unused = o->unk10.z - pos.vz;
                if (dx * dx + unused * unused < (rec->radius + r) * (rec->radius + r)) {
                    x = FindExtension(n, 0x1000);
                    if (x != NULL && (*(u16 *)(x + 0x1C) & mask)) {
                        *(s16 *)(x + 0x16) = add + 0x14;
                        *(u16 *)(x + 0x1E) |= mask;
                        return n;
                    }
                }
            }
        }
    }
    return NULL;
}

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

s32 func_800B6860(u32 idx) {
    s32 hit;
    s32 bot;
    s32 top;
    EXTengine *eng;
    EXTengine *e;
    FlagRecord *box;
    FlagRecord *rec;
    HeroInfo *h;
    NAS *nas;
    NAS *self;
    EXTobject *obj;
    s32 r;
    s32 can;
    s32 rr;
    s32 rr2;
    s32 dx;
    s32 dz;
    s32 d2;
    s32 d;
    s32 ang;
    s32 lo;
    s32 hi;
    s32 *p;
    s32 i;
    Slot40 *slot;

    hit = 0;
    if (idx >= 6) {
        return 0;
    }
    eng = (EXTengine *)D_800BD70C[idx].pad4[0];
    if (eng != NULL && (box = func_80025A48(eng, 0x40)) != NULL) {
    r = box->radius;
    top = eng->unk10.y + box->unk2;
    bot = eng->unk10.y + box->height;
    for (i = 0; i < 4; i++) {
        if (!(Heroes[i].hero_status & 0xF800)) {
            if (((s16 *)D_800BD70C[idx].pad4[4])[7] == 0) {
                can = Heroes[i].hero_move->forward_speed == 0;
            } else {
                can = 1;
            }
            e = Heroes[i].hero_eng;
            if (e != NULL) {
                rec = func_80025A48(e, 0x40);
                if (rec != NULL) {
                    lo = e->unk10.y + rec->unk2;
                    hi = e->unk10.y + rec->height;
                    if (can) {
                        rr = rec->radius;
                        rr += r;
                        rr2 = rr * rr;
                        if (bot < lo && hi < top) {
                            dx = e->unk10.x - eng->unk10.x;
                            dz = e->unk10.z - eng->unk10.z;
                            d2 = dx * dx + dz * dz;
                            if (d2 < rr2) {
                                d2 = csqrt(d2) >> 6;
                                if (d2 < rr) {
                                    ang = get_angle(dx, dz) & 0xFFF;
                                    d2 = rr - d2 + 1;
                                    eng->unk10.x -= d2 * D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> 12;
                                    hit = 1;
                                    eng->unk10.z -= d2 * D_80068BD4[ang].unk2 >> 12;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    nas = D_8005AA2C;
    p = (s32 *)D_800BD70C[idx].pad4[6];
    slot = &D_800BD70C[idx];
    self = NULL;
    if (p != NULL) {
        self = (NAS *)p[-1];
    }
    can = ((s16 *)slot->pad4[4])[7] != 0;
    r = 0x78;
    for (; nas != NULL; nas = nas->Solid.Child) {
        if ((nas->Class & 0x200) && nas != self) {
            e = nas->Engine;
            if (e != NULL && e != eng) {
                rec = func_80025A48(e, 0x40);
                if (rec != NULL) {
                    lo = e->unk10.y + rec->unk2;
                    hi = e->unk10.y + rec->height;
                    if (bot < lo && hi < top) {
                        dx = e->unk10.x - eng->unk10.x;
                        dz = e->unk10.z - eng->unk10.z;
                        rr = rec->radius;
                        rr += r;
                        d2 = dx * dx + dz * dz;
                        rr2 = rr * rr;
                        if (d2 < rr2) {
                            d2 = csqrt(d2) >> 6;
                            if (d2 < rr) {
                                hit = 1;
                                ang = get_angle(dx, dz) & 0xFFF;
                                if (!(ang & 0xFF)) {
                                    ang += Random(0x10) - 8;
                                }
                                d2 = rr - d2 + 1;
                                if (can) {
                                    eng->unk10.x -= d2 * D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> 12;
                                    eng->unk10.z -= d2 * D_80068BD4[ang & 0xFFF].unk2 >> 12;
                                }
                                obj = FindExtension(nas, 0x1000);
                                if (obj != NULL && (obj->can_do & 8)) {
                                    obj->player = idx;
                                    obj->will_do |= 8;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    }
    return hit;
}

void func_800B6D14(void) {
    Ovl5eSceneObj *obj;
    u16 *id;
    PapuEntry *p;
    s32 i;
    s32 j;
    s32 dx;
    s32 dz;

    obj = *(Ovl5eSceneObj **)((u8 *)D_80058AD0 + 0x1C);
    func_80018430(0xC8);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            D_800BD948[j][i].tile = NULL;
        }
    }
    for (j = 0; j < 6; j++) {
        D_800BE648[j].unk0 = 0;
        D_800BE648[j].unk6 = 0;
        D_800BE648[j].unkA = 0;
        D_800BE648[j].unkC = 0;
        D_800BE648[j].unkE = 0;
        D_800BE648[j].unk8 = 0;
    }
    D_800BE824 = NULL;
    while (obj != NULL) {
        id = (u16 *)func_80015870(obj->model, obj->index);
        if ((u32)(*id - 10) < 78) {
            i = 7 - (s16)((s16)*id % 10);
            j = (s16)((s16)*id / 10) - 1;
            D_800BD948[j][i].tile = (Ovl5eTile *)obj;
            func_8001C3D0((u8 *)obj);
            obj->flags |= 0x1000000;
        }
        j = *(s16 *)id - 100;
        if ((u32)j < 20) {
            p = &D_800BE648[j];
            p->unk0 = (s32)obj;
            p->unk8 = obj->unk8;
        }
        if (j == 50) {
            D_800BE824 = obj;
            obj->unk50 = 1;
            obj->unk51 = 0;
            obj->flags &= ~0x8000;
        }
        obj = obj->next;
    }
    for (j = 39; j >= 0; j--) {
        D_800BE838[j] = 0;
    }
    D_800BE888 = 0;
    D_800BE88A = 0;
    D_800BE88C = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            obj = (Ovl5eSceneObj *)D_800BD948[j][i].tile;
            if (obj != NULL) {
                D_800BD948[j][i].unkA = 0;
                D_800BD948[j][i].unk10 = obj->index;
                D_800BD948[j][i].pad8 = 0;
                D_800BD948[j][i].unk12 = obj->unk8;
                D_800BD948[j][i].unkE = 0;
                dx = obj->x;
                dz = obj->z - 0x600;
                D_800BD948[j][i].unkC = csqrt(dx * dx + dz * dz) >> 6;
            }
        }
    }
}

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

void func_800B7134(void) {
    s32 i;
    s32 j;
    s32 m;
    u32 t;
    u32 a;
    s32 w;
    u32 u;
    u32 k;
    Ovl5eTile *tile;
    u16 *tb;
    TrigEntry *trig;
    u16 *seq;

    if (D_800BE824 != NULL) {
        if (D_800BD8D4 == 0) {
            if (D_800BE828 != 0) {
                D_800BE828 -= 0x140;
                if (D_800BE828 < 0) {
                    D_800BE828 = 0;
                }
                if (D_800BE828 < 0x200 && D_800BE81C != NULL) {
                    StopSfx(D_800BE81C);
                }
                D_800BE824->unk24 = D_800BE828;
            } else {
                D_800BE824->flags &= ~0x8000;
            }
        } else {
            if (D_800BE828 == 0) {
                PlaySfx(0x20D, &D_800BE81C, NULL, 0x1000, 0x1E00, 0);
            }
            if (D_800BE828 < 0x1000) {
                D_800BE828 += 0x80;
                if (D_800BE828 > 0x1000) {
                    D_800BE828 = 0x1000;
                }
                D_800BE824->unk24 = D_800BE828;
            }
            D_800BE824->flags |= 0x8000;
        }
        D_800BE824->unk52 = 1;
    }
    func_800B6FF8();
    func_800B4204();
    m = (u16)D_800BE894;
    if (m < 3 && m != 0) {
        if (D_800BE838[0] != 0) {
            D_800BE838[0] -= 2;
        }
        j = 0x27;
        do {
            D_800BE838[j] = D_800BE838[j - 1];
            j--;
        } while (j > 0);
        D_800BE888 += 0x20;
        t = D_800BE888;
        for (i = 1; i < 8; i++) {
            j = 0;
            tb = D_800BE838;
            trig = D_80068BD4;
            seq = D_800BD1B0;
            for (; j < 7; j++) {
                tile = D_800BD948[j][i].tile;
                if (tile != NULL) {
                    a = D_800BD948[j][i].unkC;
                    u = t + a;
                    a >>= 7;
                    w = tb[a];
                    tile->unk8 = D_800BD948[j][i].unk12;
                    k = ((u & 0xFFF) - 0x400) & 0xFFF;
                    tile->unk8 += (w * trig[k].unk2) >> 12;
                    if (D_800BD948[j][i].unkE != 0) {
                        D_800BD948[j][i].unkA |= 0x100;
                        tile->unk74 = seq[D_800BD948[j][i].unkE];
                        D_800BD948[j][i].unkE++;
                        if (seq[D_800BD948[j][i].unkE] == 0) {
                            D_800BD948[j][i].unkE = 0;
                        }
                    } else {
                        tile->unk74 = D_800BD948[j][i].unk10;
                        D_800BD948[j][i].unkA &= ~0x100;
                    }
                }
            }
            /* Dead stores (flow deletes them): a hack. Setting the table bases twice per pass
               stops loop.c moving them out of this loop; the target rebuilds each one per i. */
            tb = NULL;
            trig = NULL;
            seq = NULL;
        }
    }
}

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

void func_800B7CBC(u16 arg0) {
    s32 i;
    Ovl5eMsg *m;
    s32 v;
    short t;
    int a;
    s32 b;

    for (i = 0;; i++) {
        if (i >= (arg0 & 0xFFFF)) {
            return;
        }
        m = func_800B3E80(D_800BD8E2);
        if (m != NULL) {
            a = m->unk0;
            b = m->unk4;
            t = D_800BD908[D_800BD8E0];
            D_800BD8E0 = (D_800BD8E0 + 1) & 0x3F;
            v = t;
            if (v == 6 && D_8009E46C[6] >= 4) {
                v = 4;
            }
            func_800B82DC(a, 0, b, v, -1);        }
    }
}

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

void func_800B80B0(void) {
    s32 i;
    s32 x, z;
    u16 t;
    u8 c;
    u16 *p;
    s32 j;
    NAS *n;
    EXTengine *e;
    T_B80B0Fux *f;

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
        f = (T_B80B0Fux *)n->FUX;
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

NAS *func_800B82DC(s32 a, s32 b, s32 c, s32 d, s32 e) {
    NAS *nas;
    Ovl5eObj *obj;
    EXTengine *eng;
    s32 x, z;

    u16 k;
    SLONG_XYZ *p;
    s32 t;

    nas = SpawnNASById(a, b, c, 0x204, 0x11);
    k = d;
    if (nas != NULL) {
        eng = nas->Engine;
        if (eng != NULL) {
            obj = (Ovl5eObj *)AddObject(nas);
            if (obj != NULL && AddMotion(nas) != NULL) {
                func_80026AD8((u8 *)nas);
                func_8002694C(nas, 0x300);
                nas->FUX = (EXT *)obj;
                eng->sub->unk6C = D_80052F94;
                eng->unk5D = 1;
                obj->unk14 = d;
                D_8009E46C[obj->unk14]++;
                obj->unk1A = 7;
                switch (k) {
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
                eng->unk2C.x = 0x40;
                eng->unk2C.y = 0x40;
                eng->unk2C.z = 0x40;
                eng->unk5D = 1;
                eng->unk5C = 1;
                eng->unk5E = 1;
                eng->flags |= 1;
                eng->unk10.y = D_8005A5D8;
                p = &eng->unk10;
                PlaySfx(0x3FA, NULL, p, 0x1000, 0x1E00, 0);
                t = k;
                x = (eng->unk10.x >> 8) + 10;
                z = 9 - (p->z >> 8);
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
    return nas;
}

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
