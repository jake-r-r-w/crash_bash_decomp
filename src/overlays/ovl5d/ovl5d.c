#include "common.h"
#include "bash/ovl5d.h"
#include "bash/bash.h"
#include "bash/xtras.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B3A38);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5d/ovl5d", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B3DD8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B432C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B44B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B46B8);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B6138);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B62EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B6DA4);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B6FE0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B710C);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B7E74);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B8040);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B8B00);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B8C9C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitTumbler);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotTumbler);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B94C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800B95F0);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", PlotBash8way);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", InitBashHomer);

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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/ovl5d", func_800BAFDC);

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
