#include "common.h"
#include "bash/ovl5d.h"
#include "bash/bash.h"
#include "bash/xtras.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B3A38);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5d/ovl5d", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B3DD8);

void func_800B432C(void) {
    s32 i;
    s16 id;
    s16 *p;

    func_80086F78();
    func_800BB7E8();
    func_80086D88();
    if (D_8009D908 != 0) {
        if (D_800C3B40 == -1) {
            for (i = 0; i < 4; i++) {
                D_8009D87C[i].unk4 = 0x10;
                D_8009D87C[i].unk0 = 0x1A;
            }
            p = &D_800C3B40;
            *p = func_800B6DA4(0x63);
            id = *p;
            if (id != -1 && id < 4) {
                D_8009D87C[id].unk4 = 0x30;
                D_8009D87C[id].unk0 = 8;
            } else {
                D_800C3B40 = -1;
            }
        } else if (Heroes[D_800C3B40].hero_status & 0xFA00) {
            D_800C3B40 = -1;
        }
    }
    switch (D_8005A648) {
    case 0:
    case 3:
        func_8007A91C();
        func_800B7358();
        break;
    case 1:
        func_800BAD70();
        func_800B8578();
        func_800B7358();
        break;
    case 2:
        func_800B8B00();
        func_800B7358();
        break;
    case 32:
        func_800B7358();
        break;
    }
}

void func_800B44B0(s32 arg0) {
    EXTengine *e = Heroes[arg0].hero_eng;
    EXTheromove *m = Heroes[arg0].hero_move;
    s32 step;

    D_8009D714();
    func_800860D4(arg0);
    if (m->velocity_calc == 3) {
        if (m->x_vel != m->x_max) {
            if (m->x_max != 0) {
                step = m->x_acc;
            } else {
                step = m->x_dec;
            }
            if (Heroes[arg0].hero_eng->unk10.y < D_8005A5D8) {
                step = 0x80;
            }
            if (m->x_vel < m->x_max) {
                m->x_vel += step;
                if (m->x_vel > m->x_max) {
                    m->x_vel = m->x_max;
                }
            } else {
                m->x_vel -= step;
                if (m->x_vel < m->x_max) {
                    m->x_vel = m->x_max;
                }
            }
        }
        if (m->z_vel != m->z_max) {
            if (m->z_max != 0) {
                step = m->z_acc;
            } else {
                step = m->z_dec;
            }
            if (Heroes[arg0].hero_eng->unk10.y < D_8005A5D8) {
                step = 0x80;
            }
            if (m->z_vel < m->z_max) {
                m->z_vel += step;
                if (m->z_vel > m->z_max) {
                    m->z_vel = m->z_max;
                }
            } else {
                m->z_vel -= step;
                if (m->z_vel < m->z_max) {
                    m->z_vel = m->z_max;
                }
            }
        }
        e->unk10.z += m->z_vel >> 2;
        e->unk10.x += m->x_vel >> 2;
    }
}

s32 func_800B46B8(s32 arg0) {
    s32 i;
    s32 dx;
    s32 dz;

    for (i = 0; i < 4; i++) {
        if (i != arg0 && !(Heroes[i].hero_status & 0xFA00) && Heroes[i].hero_pu == 0x14) {
            dx = Heroes[arg0].hero_eng->unk10.x - Heroes[i].hero_eng->unk10.x;
            dz = Heroes[arg0].hero_eng->unk10.z - Heroes[i].hero_eng->unk10.z;
            if (dx * dx + dz * dz < 0x100000) {
                return get_angle(dx, dz);
            }
        }
    }
    if (D_800C46B4 != NULL && D_800C46B4->status == 3) {
        dx = D_800C46B0->unk10.x - Heroes[arg0].hero_eng->unk10.x;
        dz = D_800C46B0->unk10.z - Heroes[arg0].hero_eng->unk10.z;
        if (dx * dx + dz * dz < 0x240000) {
            return get_angle(-dx, -dz);
        }
    }
    return 0;
}

void func_800B4848(s32 arg0, s32 arg1, s32 arg2, Ovl5dAiState *ai, s32 prev) {
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *o;
    s32 i;
    s32 best;
    s32 min;
    s32 dx;
    s32 dz;
    s32 d;

    base = Heroes;
    h = base + arg0;
    if (h->hero_weapon_count == 0 || h->hero_attack_timer != 0) {
        return;
    }
    best = -1;
    switch (h->hero_pu) {
    case 0x1C:
        min = 0x51000;
        for (i = 0; i < 4; i++) {
            o = &Heroes[i];
            if (arg0 != i && i != arg1 && !(o->hero_status & 0xFA00)) {
                dx = base[arg0].hero_eng->unk10.x - o->hero_eng->unk10.x;
                dz = dx * dx;
                dx = base[arg0].hero_eng->unk10.z - o->hero_eng->unk10.z;
                d = dz + dx * dx;
                if (d < min) {
                    best = i;
                    min = d;
                }
            }
        }
        break;
    case 0x1B:
        min = 0x800;
        goto loop2;
    case 0 ... 0x1A:
    default:
        min = 0x800;
    loop2:
        for (i = 0; i < 4; i++) {
            if (i != arg0 && i != arg1) {
                d = func_80015480(arg2, get_angle(Heroes[i].hero_eng->unk10.x - Heroes[arg0].hero_eng->unk10.x,
                                                  Heroes[i].hero_eng->unk10.z - Heroes[arg0].hero_eng->unk10.z));
                if (d < min) {
                    min = d;
                    best = i;
                }
            }
        }
        break;
    }
    if (best == prev && prev != -1) {
        ai->target = -1;
        ai->unk20 = 1;
        ai->flags |= 0x8000;
    } else if (best != -1 && (D_8005A680[0] == 0 || best != arg1)) {
        ai->target = -1;
        ai->unk20 = 1;
        ai->flags |= 0x8000;
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5d/ovl5d", D_800B3424);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B4AB0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B5E28);

s32 func_800B6138(s32 arg0) {
    EXTbotai *ai;
    u16 w[3];
    s32 i;
    s32 l;
    s32 r;
    s32 a;
    s32 b;
    u16 *p;
    s32 ff;
    s32 fe;

    ai = Heroes[arg0].hero_ai;
    i = 0;
    ff = 0xFF;
    fe = 0xFE;
    p = w;
    for (; i < 3; i++) {
        *p = 0;
        a = ai->AIforward_left[i];
        b = ai->AIforward_right[i];
        l = a & 0x3F;
        r = b & 0x3F;
        if (a == ff || b == ff) {
            *p = ff;
        }
        if (a == fe || b == fe) {
            *p = fe;
        }
        if (l >= 6) {
            if (l < 8) {
                goto setl;
            }
            if (l < 0x16) {
                if (l >= 0x14) {
                setl:
                    *p = ff;
                }
            }
        }
        if (r >= 6) {
            if (r < 8) {
                goto setr;
            }
            if (r < 0x16) {
                if (r >= 0x14) {
                setr:
                    *p = ff;
                }
            }
        }
        p++;
    }
    if (w[0] != 0) {
        if (Heroes[arg0].hero_object != NULL) {
            Heroes[arg0].hero_move->velocity_enable = 0;
            ai->AImode = 0xD;
            return 1;
        }
        if (w[1] != 0 || w[2] != 0) {
            Heroes[arg0].hero_move->forward_angle += 0x31C;
            ai->AIaborted_mode = ai->AImode;
            ai->AImode = 5;
            ai->AIsleep = 0x1E;
            ai->TargetPlayer = -1;
            ai->pTargetNas = NULL;
            return 1;
        }
        Heroes[arg0].hero_move->velocity_enable = 1;
        ai->AIpad |= 0x4000;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B62EC);

s32 func_800B6DA4(s32 arg0) {
    s32 i;
    s32 sum;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
        }
    }
    if (D_800C46B4 != NULL && (u32)arg0 < 4 && D_800C46B4->status == 1) {
        if (Random(10) == 0) {
            return 6;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

void func_800B6F0C(void) {
    HeroInfo *h;
    s32 i;
    s32 best;
    s32 max;

    max = 0;
    best = -1;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (!(h->hero_status & 0xFA00) && max < h->hero_health) {
            max = h->hero_health;
            best = i;
        }
    }
    if (best != -1) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00) && i != best) {
                func_80084E04(i, 0, -1, 0x1000, 0);
            }
        }
    }
}

s32 func_800B6FE0(s32 arg0) {
    HeroInfo *base;
    HeroInfo *o;
    HeroInfo *h;
    EXTengine *a;
    EXTengine *b;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 min;

    u8 *g;
    s32 mate;
    s32 r;

    mate = Heroes[arg0].hero_teammate;
    min = 0x7FFFFF;
    i = 0;
    g = D_8005A614;
    base = Heroes;
    h = &Heroes[arg0];
    for (i = 0; (u16)i < 4; i++) {
        if ((u16)i != arg0 && (*(s32 *)(g + 0x6C) == 0 || (u16)i != mate)) {
            o = (HeroInfo *)((u16)i * sizeof(HeroInfo) + (u32)base);
            if (!(o->hero_status & 0xFA00)) {
                a = h->hero_eng;
                b = o->hero_eng;
                dx = a->unk10.x - b->unk10.x;
                dz = a->unk10.z - b->unk10.z;
                d = dx * dx + dz * dz;
                if (d < min) {
                    min = d;
                }
            }
        }
    }
    if (min != 0x7FFFFF) {
        r = csqrt(min) >> 6;
    } else {
        r = 0;
    }
    return r;
}

void func_800B710C(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4, u16 arg5,
                   u16 arg6, u16 arg7, u16 arg8) {
    s32 i;
    s32 k;
    s32 b;
    u16 c;
    u8 t;

    D_800C3B94[3] = arg1 * arg6 / 16 + 1;
    D_800C3B94[2] = arg2 * arg6 / 16 + 1;
    D_800C3B94[1] = arg3 * arg6 / 16 + 1;
    D_800C3B94[0] = arg3 * arg6 / 16 + 1;
    D_800C3B88 = arg0;
    D_800C3B8C[3] = arg1;
    D_800C3B8C[2] = arg2;
    D_800C3B8C[0] = D_800C3B8C[1] = arg3;
    arg4 *= 4;
    arg5 *= 4;
    c = arg6 * 4;
    arg7 *= 4;
    arg8 *= 4;
    k = 0;
    for (i = 0; i < arg4; i++) {
        D_800C3B44[k++] = 4;
    }
    for (i = 0; i < arg5; i++) {
        D_800C3B44[k++] = 5;
    }
    for (i = 0; i < c; i++) {
        D_800C3B44[k++] = 6;
    }
    for (i = 0; i < arg7; i++) {
        D_800C3B44[k++] = 1;
    }
    for (i = 0; i < arg8; i++) {
        D_800C3B44[k++] = 2;
    }
    for (i = 0; i < 600; i++) {
        k = Random(0x40);
        b = Random(0x40);
        t = D_800C3B44[k];
        D_800C3B44[k] = D_800C3B44[b];
        D_800C3B44[b] = t;
    }
    D_800C3B84 = Random(0x20) + 0x14;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B7358);

void func_800B7D20(u16 arg0) {
    s32 i;
    Ovl5dPair *p;
    s32 t;
    s32 c;
    s32 a;
    s32 b;

    for (i = 0;; i++) {
        if (i >= arg0) {
            return;
        }
        p = func_800B8040(D_800C3B86);
        if (p != NULL) {
            a = p->unk0;
            b = p->unk4;
            c = D_800C3B44[D_800C3B84];
            D_800C3B84 = (D_800C3B84 + 1) & 0x3F;
            t = c;
            if (t == 6) {
                if (!(OVLG->flags & 0x80) || OVLG->mode != 1) {
                    if (D_8009E46C[6] >= D_800C3B94[D_8009D758 - 1]) {
                        t = 4;
                    }
                }
            }
            if (OVLG->mode == 3 && t == 4) {
                t = 3;
            }
            func_800BBA14(a, 0, b, t, -1);
        }
    }
}

void func_800B7E74(void) {
    s32 i;
    s32 j;
    s32 x;
    s32 z;
    s32 n;
    s32 tx;
    s32 tz;

    if (D_8005A648 == 3) {
        for (i = 0; i < 16; i++) {
            z = i * 256 - 0x780;
            for (j = 0; j < 16; j++) {
                x = j * 256 - 0x780;
                if ((u32)(i - 2) >= 12) {
                    z = Random(11) * 256 - 0x580;
                }
                if ((u32)(j - 2) >= 12) {
                    x = Random(11) * 256 - 0x580;
                }
                D_800C3EAC[i][j].x = x;
                D_800C3EAC[i][j].z = z;
            }
        }
        n = 0;
    } else {
        for (i = 0; i < 16; i++) {
            z = i * 256 - 0x780;
            for (j = 0; j < 16; j++) {
                x = j * 256 - 0x780;
                D_800C3EAC[i][j].x = x;
                D_800C3EAC[i][j].z = z;
            }
        }
        n = 0;
    }
    do {
        i = Random(16);
        j = Random(16);
        tx = Random(16);
        tz = Random(16);
        x = D_800C3EAC[i][j].x;
        z = D_800C3EAC[i][j].z;
        D_800C3EAC[i][j].x = D_800C3EAC[tx][tz].x;
        D_800C3EAC[i][j].z = D_800C3EAC[tx][tz].z;
        D_800C3EAC[tx][tz].x = x;
        D_800C3EAC[tx][tz].z = z;
        n++;
    } while (n < 0x258);
}

Ovl5dPair *func_800B8040(u8 *arg0) {
    Ovl5dGridCell *best;
    Ovl5dGridCell *cell;
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

    cell = D_800C3EAC[0];
    bestn = 9;
    tries = 9;
    i = *(u16 *)arg0;
    best = cell = &cell[i];
    besti = i;
    i++;
    i &= 0xFF;
    x = (cell->x >> 8) + 10;
    z = 9 - (cell->z >> 8);
    do {
        while (D_8009D90C[z * 20 + x].unk0 != 0) {
            if (i == *(u16 *)arg0) {
                return NULL;
            }
            cell = D_800C3EAC[0];
            cell = &cell[i];
            i++;
            i &= 0xFF;
            x = (cell->x >> 8) + 10;
            z = 9 - (cell->z >> 8);
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
            return (Ovl5dPair *)cell;
        }
        if (n < bestn) {
            bestn = n;
            best = cell;
            besti = i;
        }
        if (i == *(u16 *)arg0) {
            return NULL;
        }
        cell = D_800C3EAC[0];
        cell = &cell[i];
        i++;
        i &= 0xFF;
        x = (cell->x >> 8) + 10;
        z = 9 - (cell->z >> 8);
        if (--tries == 0xFFFF) {
            break;
        }
    } while (1);
    *(u16 *)arg0 = besti;
    return (Ovl5dPair *)best;
}

void func_800B8314(void) {
    s32 j;
    s32 i;
    Ovl5dCell *p;
    Ovl5dObj *obj;
    u16 *id;
    s16 v;
    Ovl5dCell *base;
    Ovl5dCell *base2;
    s32 x;
    s32 z;
    s32 off;
    s32 colOff;

    z = 0;
    x = 0;
    obj = (Ovl5dObj *)((GfxScene *)D_80058AD0)->objs;
    func_80018430(200);
    if (obj != NULL) {
        base = D_800C3BAC[0];
        do {
            id = (u16 *)func_80015870(obj->model, obj->index);
            if ((u16)(*id - 10) < 78) {
                v = *id;
                i = 7 - (s16)(v % 10);
                j = (s16)(v / 10) - 1;
                p = (Ovl5dCell *)(i * sizeof(Ovl5dCell) + j * sizeof(D_800C3BAC[0]) + (u32)base);
                p->obj = obj;
                func_8001C3D0((u8 *)obj);
                obj->flags |= 0x1000000;
            }
            if (*(s16 *)id == 10) {
                x = obj->x;
                z = obj->z;
            }
            obj = obj->next;
        } while (obj != NULL);
    }
    if (x | z) {
        obj = (Ovl5dObj *)((GfxScene *)D_80058AD0)->objs;
        if (obj != NULL) {
            do {
                if (*(s16 *)func_80015870(obj->model, obj->index) == 5) {
                    j = obj->x - x;
                    i = obj->z - z;
                    j = (j + 0x80) >> 9;
                    i = ((i + 0x80) >> 9) + 7;
                    obj->flags = (obj->flags | 0x1000000) & ~0x8000;
                    D_800C3BAC[j][i].obj2 = obj;
                }
                obj = obj->next;
            } while (obj != NULL);
        }
    }
    i = 0;
    base2 = D_800C3BAC[0];
    for (colOff = 0; i < 8; i++, colOff += sizeof(Ovl5dCell)) {
        for (j = 0, off = colOff; j < 8; j++, off += sizeof(D_800C3BAC[0])) {
            p = (Ovl5dCell *)(off + (u32)base2);
            obj = p->obj;
            if (obj != NULL) {
                p->unkA = obj->index;
                p->unk8 = 0;
            } else {
                p->unkA = 0;
                p->unk8 |= 0x100;
            }
        }
    }
}

void func_800B8578(void) {
    s32 i;
    s32 j;
    Ovl5dObj *obj;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            obj = D_800C3BAC[i][j].obj2;
            if (obj != NULL) {
                obj->flags &= ~0x8000;
                if (D_800C3BAC[i][j].unkA != 0) {
                    if (i > 0 && D_800C3BAC[i - 1][j].unkA == 0) {
                        obj->flags |= 0x01008000;
                    }
                    if (i < 7 && D_800C3BAC[i + 1][j].unkA == 0) {
                        obj->flags |= 0x01008000;
                    }
                    if (j > 0 && D_800C3BAC[i][j - 1].unkA == 0) {
                        obj->flags |= 0x01008000;
                    }
                }
            }
        }
    }
}

void PlotSnowGlint(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    SnowGlint *g;
    s32 i;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        g = FindExtension(nas, 0x20000002);
        if (g != NULL && sub != NULL) {
            i = 0;
            do {
                if (g->on[i] == 0) {
                    if (g->timer[i] != 0) {
                        g->timer[i]--;
                        sub->flags &= 0xFFFF7FFF;
                    } else {
                        sub->unk04 = Random(0xF00) - 0x780;
                        sub->unk0C = Random(0xF00) - 0x780;
                        sub->flags |= 0x8000;
                        g->timer[i] = Random(3);
                        g->on[i] = 1;
                    }
                } else {
                    if (g->timer[i] != 0) {
                        g->timer[i]--;
                    } else {
                        g->timer[i] = Random(0xF);
                        g->on[i] = 0;
                    }
                }
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotBashPenguin);

void func_800B8B00(void) {
    s32 lo, hi;
    u32 x, z;
    u32 xhi, zhi;

    lo = 8;
    hi = 12;
    for (x = lo; (s32)x < hi; x++) {
        for (z = lo; (s32)z < hi; z++) {
            D_8009D90C[z * 20 + x].unk0 = 0xF0;
        }
    }
    x = ((D_800C46B0->unk10.x - 0x80) >> 8) + 10;
    z = 9 - ((D_800C46B0->unk10.z - 0x80) >> 8);
    xhi = ((D_800C46B0->unk10.x + 0x80) >> 8) + 10;
    zhi = 9 - ((D_800C46B0->unk10.z + 0x80) >> 8);
    if (x < 20) {
        if (z < 20) {
            D_8009D90C[z * 20 + x].unk0 |= 0x40;
        }
        if (zhi < 20) {
            D_8009D90C[zhi * 20 + x].unk0 |= 0x40;
        }
    }
    if (xhi < 20) {
        if (z < 20) {
            D_8009D90C[z * 20 + xhi].unk0 |= 0x40;
        }
        if (zhi < 20) {
            D_8009D90C[zhi * 20 + xhi].unk0 |= 0x40;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B8C9C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitTumbler);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotTumbler);

void func_800B94C4(s32 arg0, u16 arg1) {
    HeroInfo *h2;
    HeroInfo *base2;
    (&Heroes[arg0])->hero_pu = arg1;
    (&Heroes[arg0])->hero_weapon_count = 1;
    (&Heroes[arg0])->hero_status |= 0x10;
    switch (arg1) {
    case 25:
        (&Heroes[arg0])->hero_powerup->sub->unk74 = 0x200B;
        (&Heroes[arg0])->hero_weapon_count = 2;
        break;
    case 27:
        (&Heroes[arg0])->hero_powerup->sub->unk74 = 0x200F;
        (&Heroes[arg0])->hero_weapon_count = 3;
        break;
    default:
        base2 = Heroes;
        h2 = base2 + arg0;
        h2->hero_powerup->sub->unk74 = 0x200C;
        h2->hero_weapon_count = 1;
        break;
    }
    switch (arg1) {
    case 25:
        PlaySfx(0x438, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    case 27:
        PlaySfx(0x437, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    default:
        PlaySfx(0x439, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    }
}

s32 func_800B95F0(s32 arg0)
{
    HeroInfo *h;
    PadButtons *p;
    NAS *n;
    u16 pu;

    HeroInfo *ht;
    PadButtons *pt;

    ht = Heroes;
    h = &ht[arg0];
    if (h->hero_weapon_count != 0) {
        pt = (PadButtons *)D_80051338;
        p = &pt[arg0];
        if (p->unk50 & 0x8000) {
            if (h->hero_attack_timer != 0) {
                p->unk4C &= ~0x8000;
            } else {
            D_8009D754 = arg0;
            switch (h->hero_pu) {
            case 0x19:
                n = SpawnNASById(h->hero_eng->unk10.x, h->hero_eng->unk10.y, h->hero_eng->unk10.z, 0x1103, 0x11);
                if (n != NULL) {
                    n->UserInfo.user1 = arg0;
                    h->hero_attack_timer = 0x14;
                    h->hero_weapon_timer = 0x1E;
                    h->hero_weapon_count -= 1;
                }
                break;
            case 0x1B:
                n = SpawnNASById(h->hero_eng->unk10.x, h->hero_eng->unk10.y, h->hero_eng->unk10.z, 0x1104, 0x11);
                if (n != NULL) {
                    n->UserInfo.user1 = arg0;
                    h->hero_attack_timer = 0x14;
                    h->hero_weapon_timer = 0x1E;
                    h->hero_weapon_count -= 1;
                }
                break;
            case 0x1C:
                n = SpawnNASById(h->hero_eng->unk10.x, h->hero_eng->unk10.y, h->hero_eng->unk10.z, 0x1105, 0x11);
                if (n != NULL) {
                    n->UserInfo.user1 = arg0;
                    h->hero_attack_timer = 0x28;
                    h->hero_weapon_timer = 0x3C;
                    h->hero_weapon_count -= 1;
                }
                break;
            default:
                return 0;
            }
            }
        } else {
            return 0;
        }
    }
    return 0;
}

s32 func_800B97A8(x, y, z, size, skip, mask)
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

void InitBash8way(NAS *nas) {
    EXTengine *eng;
    EXTmotion *mot;
    subengine *sub;

    eng = nas->Engine;
    if (eng != NULL) {
        nas->UserInfo.user1 = D_8009D754;
        nas->UserInfo.user2 = Heroes[D_8009D754].hero_move->facing_angle;
        func_8001D390(eng, 7);
        sub = eng->sub;
        eng->unk5D = 0;
        for (; sub != NULL; sub = sub->next) {
            sub->unk51 = 0;
            sub->unk6C = D_80052F5C;
            sub->unk74 = 0x200E;
        }
        mot = AddMotion(nas);
        if (mot != NULL) {
            mot->Xvel = 0x20;
            mot->Xmax = 0xA0;
            mot->Xacc = 8;
            mot->Ymax = 0x80;
            mot->Xdec = 0;
            mot->Yvel = 0;
            mot->Yacc = 1;
            mot->Ydec = 0;
            mot->Ydir = 0;
        }
        eng->unk10.y -= 0xE0;
        PlaySfx(0x43C, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBash8way(NAS *nas) {
    EXTengine *eng;
    EXTmotion *mot;
    subengine *sub;
    s32 dead;
    s32 ang;
    s32 a;
    s32 speed;
    s32 t;
    s32 x, y, z;
    s32 kind;

    eng = nas->Engine;
    if (eng != NULL) {
        dead = 1;
        kind = 0x19;
        mot = FindExtension(nas, 0x800);
        if (mot != NULL) {
            mot->Xvel += mot->Xacc;
            if (mot->Xvel > mot->Xmax) {
                mot->Xvel = mot->Xmax;
            }
            mot->Yvel += mot->Yacc;
            if (mot->Yvel > mot->Ymax) {
                mot->Yvel = mot->Ymax;
            }
            ang = nas->UserInfo.user2;
            sub = eng->sub;
            speed = mot->Xvel;
            for (; sub != NULL; ang += 0x200, sub = sub->next, ang &= 0xFFF) {
                if (sub->flags != 0) {
                    a = (ang - 0x400) & 0xFFF;
                    sub->unk04 -= (speed * D_80068BD4[a].unk2) >> 12;
                    sub->unk0C -= (speed * D_80068BD4[ang & 0xFFF].unk2) >> 12;
                    t = sub->unk08 + mot->Yvel;
                    sub->unk08 = t;
                    x = eng->unk10.x + sub->unk04;
                    y = eng->unk10.y + t;
                    z = eng->unk10.z + sub->unk0C;
                    dead = 0;
                    sub->unk12 = a;
                    if (y >= D_8005A5D8) {
                        func_800BA8A4(x, D_8005A5D8, z, nas->UserInfo.user1, kind);
                        sub->flags = 0;
                    }
                    if (func_800B97A8(x, y, z, 0x20, nas->UserInfo.user1, 0xA) != -1) {
                        func_800BA8A4(x, D_8005A5D8, z, nas->UserInfo.user1, kind);
                        sub->flags = 0;
                    }
                    sub->unk52 = 1;
                }
            }
        }
        if (dead) {
            KillNAS(nas);
        }
    }
}

void InitBashHomer(NAS *nas) {
    EXTengine *eng;
    EXTmotion *mot;
    subengine *sub;
    HeroInfo *h;
    s32 i, best, bestd, d, u1;

    eng = nas->Engine;
    if (eng != NULL) {
        h = Heroes;
        nas->UserInfo.user1 = D_8009D754;
        nas->UserInfo.user2 = Heroes[D_8009D754].hero_move->facing_angle;
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0x200F;
        func_8001D390(eng, 1);
        eng->unk5D = 0;
        eng->unk5C = 1;
        eng->flags |= 0x21;
        sub = eng->sub;
        sub = sub->next;
        if (sub != NULL) {
            sub->unk20.x = 0x4000;
            sub->unk20.y = 0x4000;
            sub->unk20.z = 0x4000;
            sub->unk50 = 1;
            sub->unk51 = 0;
            sub->unk6C = D_80052F5C;
            sub->unk74 = 0x2010;
            sub->flags |= 0x21;
            sub = sub->next;
            sub->unk52 = 1;
        }
        mot = AddMotion(nas);
        if (mot != NULL) {
            mot->Xvel = 0x20;
            mot->Xmax = 0xA0;
            mot->Xacc = 8;
            mot->Ymax = 0x80;
            mot->Xdec = 0;
            mot->Yvel = 0;
            mot->Yacc = 1;
            mot->Ydec = 0;
            mot->Ydir = 0;
        }
        PlaySfx(0x43B, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        best = -1;
        bestd = 0x800;
        for (i = 0; i < 4; i++) {
            u1 = nas->UserInfo.user1;
            if (i != u1 && (OVLG->p2_active == 0 || Heroes[i].hero_teammate != u1)) {
                d = func_80015480(nas->UserInfo.user2,
                                  get_angle(eng->unk10.x - Heroes[i].hero_eng->unk10.x,
                                            eng->unk10.z - Heroes[i].hero_eng->unk10.z));
                if (d < bestd) {
                    bestd = d;
                    best = i;
                }
            }
        }
        nas->UserInfo.status = best;
        nas->UserInfo.flags = 0x64;
        eng->unk10.y -= 0x180;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotBashHomer);

extern u16 D_8009D754;

void InitBashShox(NAS *nas) {
    EXTengine *eng;
    BashShoxExt *x;
    subengine *sub;
    s32 i;

    eng = nas->Engine;
    if (eng != NULL) {
        nas->UserInfo.user1 = D_8009D754;
        x = AddExtension(nas, 0x68, 0x20000000);
        if (x != NULL) {
            nas->FUX = (void *)x;
            func_8001D390(eng, 6);
            sub = eng->sub;
            eng->unk5D = 0;
            eng->unk5C = 1;
            eng->flags |= 0x21;
            if (sub != NULL) {
                i = 0;
                do {
                    sub->unk51 = 0;
                    sub->unk50 = 1;
                    sub->unk6C = D_80052F94;
                    sub->unk74 = 0x2015;
                    sub->flags = (sub->flags & 0xFFFF7FFF) | 0x21;
                    sub->unk08 = -0x100;
                    x->a[i] = i * 5;
                    x->b[i] = 0xC;
                    x->c[i] = 0x1000;
                    sub = sub->next;
                    i++;
                } while (sub != NULL);
            }
            PlaySfx(0x43A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotBashShox);

void PlotMissileExplode(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s16 *row;
    s16 (*tbl)[3];
    s32 z;
    u16 idx;

    eng = nas->Engine;
    if (eng != NULL) {
        idx = nas->UserInfo.flags;
        tbl = D_800C3B08;
        row = tbl[idx];
        sub = eng->sub;
        if (row[0] != 0) {
            sub->unk20.x = row[0];
            sub->unk20.y = row[1];
            z = row[2];
            sub->unk52 = 1;
            sub->unk20.x = z;
            nas->UserInfo.flags++;
            if (idx == 1) {
                func_8008DEC8((s32)nas, 0x280, nas->UserInfo.user1, nas->UserInfo.user2, 0);
            }
        } else {
            KillNAS(nas);
        }
    }
}

void InitMissileExplode(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        sub->unk6C = &D_80052F5C;
        sub->unk74 = 0x2010;
        sub->unk50 = 1;
        sub->unk51 = 0;
        eng->flags |= 0x21;
        nas->UserInfo.flags = 0;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800BA8A4(s32 x, s32 y, s32 z, s32 arg3, s32 arg4) {
    NAS *n;

    n = SpawnNASById(x, y - 0x80, z, 0x1106, 0x11);
    if (n != NULL) {
        n->UserInfo.user1 = arg3;
        if (arg4 != 0x19) {
            if (arg4 != 0x1B) {
                return;
            }
        } else if (Random(4) != 0) {
            goto set;
        }
        PlaySfx(D_8009E424[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
set:
        n->UserInfo.user2 = 0x208;
    }
}

void PlotBoing(u8 *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        func_800250B0((NAS *)arg0);
    }
}

void InitBoing(NAS *nas) {
    EXTengine *eng;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->unk10.y -= 0x100;
        eng->sub->unk6C = *(void **)D_80058AD0;
        eng->unk5D = 1;
        eng->unk5C = 1;
        func_80024E30(nas, &D_800C3AFC);
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitBashPenguin(NAS *nas) {
    EXTengine *eng;
    EXTobject *obj;

    eng = nas->Engine;
    if (eng != NULL) {
        D_800C46B0 = eng;
        func_80024E30(nas, &D_800C3668);
        eng->sub->unk6C = (s32)D_8005303C;
        eng->sub->unk74 = 0x2001;
        obj = (EXTobject *)AddObject(nas);
        if (obj != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
            D_800C46B4 = obj;
            nas->FUX = (EXT *)obj;
            obj->whatami = 0xE;
            obj->can_do |= 2;
        }
        eng->unk5D = 1;
        func_80025B10(eng, 0x50);
        func_800B8B00();
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitSnowGlint(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    SnowGlint *g;
    s32 i;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->sub->unk6C = *(s32 *)D_80058AD0;
        func_8001D390(eng, 9);
        sub = nas->Engine->sub;
        g = AddExtension(nas, 0x34, 0x20000002);
        if (g != NULL && sub != NULL) {
            i = 0;
            do {
                s32 v = *(s32 *)D_80058AD0;
                sub->unk74 = 0x2001;
                sub->flags = (sub->flags | 0x21) & 0xFFFF7FFF;
                sub->unk6C = v;
                g->on[i] = 0;
                g->timer[i] = Random(0xF);
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800BAC10(void) {
    u8 *vp = D_8009F6F4;
    func_80093038();
    func_80018B08((GfxViewport *)vp, (GfxCamera *)D_80051640);
    D_800569AC = *(s32 *)D_8009F644;
    func_8009440C();
}

s32 func_800BAC5C(EXTengine *eng) {
    s32 x = (eng->unk10.x + 0x800) >> 9;
    s32 z = (eng->unk10.z + 0x800) >> 9;
    s32 dy = eng->unk10.y - D_8005A5D8;

    if ((u32)x >= 8 || (u32)z >= 8) {
        return dy;
    }
    if (D_800C3BAC[x][z].unk8 & 0x100) {
        return 0x7FFFFFFF;
    }
    return func_80025E00(eng, dy);
}

u16 func_800BAD0C(s32 arg0, s32 arg1) {
    s32 x = (arg0 + 0x800) >> 9;
    s32 y = (arg1 + 0x800) >> 9;

    if ((u32)x >= 8) {
        return 0;
    }
    if ((u32)y >= 8) {
        return 0;
    }
    return D_800C3BAC[x][y].unkA;
}

void func_800BAD70(void) {
    s32 i, j;

    for (j = 2; j < 18; j++) {
        for (i = 2; i < 18; i++) {
            if (D_800C3BAC[(j - 2) >> 1][7 - ((i - 2) >> 1)].unkA == 0) {
                D_8009D90C[i * 20 + j].unk0 = 0xFF;
            }
        }
    }
}

void func_800BAE08(s32 arg0) {
    EXTengine *eng = Heroes[arg0].hero_eng;
    Ovl5dCell *base = D_800C3BAC[0];
    s32 xi = (eng->unk10.x + 0x800) >> 9;
    s32 zi = (eng->unk10.z + 0x800) >> 9;
    Ovl5dCell *cell = (Ovl5dCell *)((char *)base + (zi * 12 + xi * 0x60));
    Ovl5dObj *obj = cell->obj;

    if (obj != NULL) {
        obj->flags &= 0xFFFF7FFF;
        cell->unkA = 0;
    }
}

void func_800BAE98(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    Ovl5dCell *c;
    s32 x;
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801) && !(*(s32 *)(g + 4) & 0x1000) &&
        arg1 == D_8005A5D8 && arg5 < 8 && arg5 >= 5) {
        arg0 = (arg0 + 0x800) >> 9;
        arg2 = (arg2 + 0x800) >> 9;
        if ((u32)arg0 < 8 && (u32)arg2 < 8) {
            if (((Ovl5dCell *)((char *)D_800C3BAC + arg2 * 12 + arg0 * 0x60))->obj != NULL) {
                *(s32 *)((Ovl5dCell *)((char *)D_800C3BAC + arg2 * 12 + arg0 * 0x60))->obj &= 0xFFFF7FFF;
                ((Ovl5dCell *)((char *)D_800C3BAC + arg2 * 12 + arg0 * 0x60))->unkA = 0;
                ((Ovl5dCell *)((char *)D_800C3BAC + arg2 * 12 + arg0 * 0x60))->unk8 |= 0x100;
            }
        }
    }
}

void func_800BAF68(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            D_800C3BAC[j][i].unkA = 0;
            D_800C3BAC[j][i].unk8 = 0;
        }
    }
}

void func_800BAFB4(void) {
    func_800B7D20(D_800C3B88);
}

u8 *func_800BAFDC(s32 x, s32 unused, s32 z, u16 flag) {
    s32 step;
    s32 neg;
    s32 nx;
    s32 nz;
    s32 zi;
    s32 xi;
    u8 *res;

    step = 1;
    xi = (x >> 8) + 10;
    zi = 9 - (z >> 8);

    do {
        if (D_8009D90C[zi * 20 + xi].unk0 != 0) {
            xi = (x >> 8) + 10;
            zi = 9 - (z >> 8);
            if (flag != 0) {
                zi += step;
            } else {
                xi += step;
            }
            neg = ~step;
            step = neg + 1;
            if (step > 0) {
                step = neg + 2;
            }
            if (step >= 9) {
                step = 0;
            }
        } else {
            step = 0;
        }
    } while (step != 0);
    xi = xi - 10;
    xi <<= 8;
    zi = 19 - zi;
    zi = zi - 10;
    nz = zi << 8;
    if ((u32)(xi + 0x700) > 0xE00U) {
        xi = x;
    }
    if ((u32)(nz + 0x700) > 0xE00U) {
        nz = z;
    }
    res = func_800BBA14(xi, 0, nz, 2, -1);
    if (res != NULL) {
        func_80026B70(&D_800C478C, res);
    }
    return res;
}

s32 func_800BB100(s32 arg0) {
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

s32 func_800BB1B4(u8 *arg0) {
    s32 temp_a0;
    s32 temp_v1;
    s32 var_a2;

    var_a2 = 0;
    if ((*(s32*)((u8*)(arg0) + 144)) & 0x40) {
        temp_a0 = 0x800 - (*(s16*)((u8*)(arg0) + 138));
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
        if ((*(s32*)((u8*)(arg0) + 16)) >= 0x801) {
            (*(s32*)((u8*)(arg0) + 16)) = 0x800;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 16)) < -0x800) {
            (*(s32*)((u8*)(arg0) + 16)) = -0x800;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) >= 0x801) {
            (*(s32*)((u8*)(arg0) + 24)) = 0x800;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < -0x800) {
            (*(s32*)((u8*)(arg0) + 24)) = -0x800;
block_17:
            var_a2 = 1;
        }
    }
    return var_a2;
}

void func_800BB2BC(void) {
    s32 i;
    EXTheromove *m;
    for (i = 0; i < 4; i++) {
        m = Heroes[i].hero_move;
        if (m != NULL) {
            m->z_acc = 8;
            m->x_acc = 8;
            m->z_dec = 4;
            m->x_dec = 4;
            if ((OVLG->flags & 0x80) && (i == 0 || (OVLG->p2_active != 0 && i == 1))) {
                m->z_acc = 4;
                m->x_acc = 4;
                m->z_dec = 1;
                m->x_dec = 1;
            }
            m->velocity_calc = 3;
        }
    }
}

void func_800BB360(void) {
}

void func_800BB368(void) {
}

void func_800BB370(void) {
    D_8005AA70[0] = (s32)func_800B3A38;
    D_8005AA70[1] = (s32)func_800B3DD8;
    D_8005AA70[2] = (s32)func_800B3DD8;
    D_8005AA70[3] = (s32)func_800B432C;
    D_8005AA70[5] = (s32)func_800BB360;
    D_8005AA70[9] = (s32)func_800BAC5C;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[10] = (s32)func_800B6F0C;
}

void func_800BB3D0(void) {
    D_8009E464 = Ovl5dASTIndex(0x201);
    D_8009E468 = Ovl5dASTIndex(0x202);
    D_8009E488.unk4 = D_8009E3B4;
    D_8009E488.unk8 = NULL;
    D_8009E488.unkC = 0;
    D_8009E488.unk0 = D_8009E3DC;
}

void func_800BB4AC(Ovl5dTumbler *arg0, Ovl5dTumbleMove *arg1, s32 arg2) {
    s32 y;
    s32 vel;
    s32 v2;
    s32 sum;
    s32 d1;
    s32 d2;
    s32 max;
    s32 max2;
    s32 steps;
    s32 dir;
    s32 dx;
    s32 dz;
    s32 ang;
    s32 dist;

    dist = arg0->y;
    if (dist >= D_8005A5D8) {
        dist = D_8005A5D8 - 1;
    }
    y = dist;
    sum = 0;
    vel = arg1->vel;
    v2 = arg1->v2;
    dir = arg1->dir;
    d1 = arg1->acc - arg1->dec;
    d2 = arg1->acc2 - arg1->dec2;
    max = arg1->max;
    max2 = arg1->max2;
    steps = 0;
    while (y < D_8005A5D8) {
        vel += d1;
        if (vel < 0) {
            vel = 0;
        }
        if (vel > max) {
            vel = max;
        }
        v2 += d2;
        sum += vel;
        if (v2 < 0) {
            v2 = 0;
        }
        if (v2 > max2) {
            v2 = max2;
        }
        if (dir != 0) {
            y -= v2;
        } else {
            y += v2;
        }
        if (v2 == 0) {
            d2 = arg1->dec2;
            dir = 0;
        }
        steps++;
    }
    arg1->sum = sum;
    arg1->steps = steps;
    dx = arg0->x - Heroes[arg2].hero_eng->unk10.x;
    dz = arg0->z - Heroes[arg2].hero_eng->unk10.z;
    ang = get_angle(dx, dz);
    dist = csqrt(dx * dx + dz * dz);
    arg0->angle = ang;
    arg1->vel = (u32)(dist >> 6) / arg1->steps;
}
