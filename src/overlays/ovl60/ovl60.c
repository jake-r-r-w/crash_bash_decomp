#include "common.h"
#include "bash/main.h"
#include "bash/ovl60.h"
#include "bash/pogo.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B3534);

void func_800B3808(void) {
    s32 i;
    s16 a;
    HeroInfo *h;
    EXTengine *e;
    subengine *sub;
    Ovl60Slot *slot;
    Ovl60Move *mv;
    EXTheromove *m;

    i = 0;
    h = Heroes;
    mv = D_800C5ADC;
    slot = D_800C535C;
loop:
    if (i < 4) {
        h->hero->Camera.width = 0x500;
        switch (h->hero_name) {
        case 5:
        case 6:
        case 7:
            h->hero->Camera.height = 0x700;
            break;
        case 8:
            h->hero->Camera.height = 0x800;
            break;
        default:
            h->hero->Camera.height = 0x500;
            break;
        }
        func_800C159C(i);
        func_800B8BE4(i, 0);
        slot->unk14 = func_800C14BC(i, 0);
        mv->unk4 = 0;
        e = h->hero_eng;
        if (e != NULL) {
            if (D_8005A680[0] != 0) {
                switch (i) {
                case 0:
                case 1:
                    a = 0xC00;
                    break;
                case 2:
                case 3:
                    a = 0x400;
                    break;
                default:
                    a = 0;
                    break;
                }
            } else {
                switch (i) {
                case 0:
                    a = 0x800;
                    break;
                case 1:
                    a = 0xC00;
                    break;
                case 2:
                    a = 0;
                    break;
                case 3:
                    a = 0x400;
                    break;
                default:
                    a = 0;
                    break;
                }
            }
            e->unk1E = a;
            m = h->hero_move;
            if (m != NULL) {
                m->forward_angle = a;
                m->facing_angle = a;
            }
            e->unk5E = 1;
        }
        e = h->hero_powerup;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                u32 f = sub->flags;

                f &= ~0x8000;
                sub->unk6C = D_80053B9C;
                sub->unk74 = 0x200D;
                sub->flags = f;
                e->flags &= ~0x8000;
            }
        }
        e = h->hero_shadow;
        if (e != NULL) {
            e->unk2C.z = 0x1A00;
            e->unk2C.y = 0x1A00;
            e->unk2C.x = 0x1A00;
            e->unk5E = 1;
        }
        func_800C1004(i);
        h++;
        mv++;
        slot++;
        i++;
        goto loop;
    }
}

void func_800B3A38(void) {
    s32 i;
    s16 v;
    u32 snd;

    if (SCORE60V->flags & 0x40) {
        switch (SCORE60V->mode) {
        case 10:
            for (i = 0; i < 4; i++) {
                v = 60;
                if (SCORE60V->out[i] == 0) {
                    v = 80;
                    if (SCORE60V->teams != 0) {
                        v = 90;
                    }
                }
                Heroes[i].hero_score_displayed = v;
                Heroes[i].hero_score = v;
            }
            snd = 0x100;
            if (D_8005A680[0] != 0) {
                snd = 0x120;
            }
            break;
        case 11:
            for (i = 0; i < 4; i++) {
                v = 150;
                if (SCORE60V->out[i] != 0) {
                    v = 90;
                }
                Heroes[i].hero_score_displayed = v;
                Heroes[i].hero_score = v;
            }
            snd = 0x130;
            break;
        case 12:
            for (i = 0; i < 4; i++) {
                if (SCORE60V->out[i] != 0) {
                    v = 80;
                    if (SCORE60V->teams != 0) {
                        v = 90;
                    }
                } else {
                    v = 120;
                    if (SCORE60V->teams != 0) {
                        v = 135;
                    }
                }
                Heroes[i].hero_score_displayed = v;
                Heroes[i].hero_score = v;
            }
            snd = 0x130;
            break;
        case 13:
            for (i = 0; i < 4; i++) {
                v = 120;
                if (SCORE60V->out[i] != 0) {
                    v = 90;
                }
                Heroes[i].hero_score_displayed = v;
                Heroes[i].hero_score = v;
            }
            snd = 0x130;
            break;
        default:
            goto skip;
        }
        func_8007B7FC(snd, 0x81D0);
    skip:
        func_8007D144();
        if (D_8005A680[0] != 0) {
            D_80099174 |= 0x2000;
        }
    }
}

void func_800B3C44(void) {
    s32 i;
    s32 j;
    Ovl60Cell *p;
    Ovl60Obj *obj;
    u16 *id;
    s16 v;
    Ovl60Cell *base;
    Ovl60Cell *base2;
    u8 *g;
    s32 rowOff;
    s32 off;

    j = 0;
    base = D_800C53DC[0];
    for (rowOff = 0; j < 8; j++) {
        i = 0;
        off = rowOff;
        do {
            Ovl60Cell *c;

            func_800C2620(i, j);
            c = (Ovl60Cell *)(off + (u32)base);
            c->unk4 = NULL;
            *(s16 *)c->unk8 = 0;
            *(s16 *)c->unkC = 0;
            c->hero = NULL;
            c->unkE = 0;
            c->unk10 = 0;
            c->unk18[0] = 0;
            off += sizeof(D_800C53DC[0]);
        } while (++i < 8);
        rowOff += sizeof(Ovl60Cell);
    }
    obj = (Ovl60Obj *)((GfxScene *)D_80058AD0)->objs;
    D_800C5BB4 = NULL;
    if (obj != NULL) {
    base2 = D_800C53DC[0];
    g = D_8005A614;
    do {
        id = (u16 *)func_80015870(obj->model, obj->index);
        if ((u16)(*id - 10) < 78) {
            v = *id;
            j = 7 - (s16)(v % 10);
            i = (s16)(v / 10) - 1;
            p = (Ovl60Cell *)(j * sizeof(Ovl60Cell) + i * sizeof(D_800C53DC[0]) + (u32)base2);
            p->unk4 = (Ovl60CellPos *)obj;
            obj->flags |= 0x20000;
            func_800C3510((u8 *)obj);
        }
        if (*(s16 *)(g + 0x34) == 11 && *(s16 *)id == 100) {
            D_800C5BB4 = obj;
        }
        obj = obj->next;
    } while (obj != NULL);
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            func_800B7C3C(i, j);
        }
    }
}

void func_800B3E34(void) {
    Ovl60Slot *slot;
    s32 i;
    s32 ok;
    u8 *g;

    func_800C2EE0();
    func_800B4230();
    func_800B87DC();
    if (D_8005A644 & 0x801) {
        return;
    }
    i = 0;
    slot = D_800C535C;
loop:
    if (slot->unk4 == 5) {
        goto tick;
    }
    if (slot->unk4 == 6) {
    tick:
        if (--slot->unk6 < 2) {
            func_800C1CF0(i);
        }
    }
    i++;
    slot++;
    if (i < 4) {
        goto loop;
    }
    func_800B9C10();
    func_800B9D64();
    g = D_8005A614;
    switch ((Ovl60Mode)*(s16 *)(g + 0x34)) {
    case OVL60_MODE_12:
        break;
    case OVL60_MODE_10:
        if (*(u16 *)(g + 0x32) & 0x80) {
            switch ((u32)D_80099178 >> 8) {
            case 0x115:
                ok = D_800C52F4 < 3;
                break;
            case 0x50:
                ok = D_800C52F4 < 4;
                break;
            case 0x30:
                ok = D_800C52F4 < 5;
                break;
            case 0x20:
                ok = D_800C52F4 < 6;
                break;
            case 0x10:
                ok = D_800C52F4 < 7;
                break;
            default:
                return;
            }
            if (ok) {
                SpawnNASById(0, 0, 0, 0x1407, 0x11);
            }
        }
        break;
    case OVL60_MODE_11:
        func_800BCB4C(g);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B3FF0);

void func_800B4230(void) {
    s32 x;
    s32 z;
    EXTengine *eng;
    s32 k;
    s32 hx;
    s32 hz;
    EXTengine *he;

    for (hx = 0; hx < 8; hx++) {
        for (hz = 0; hz < 8; hz++) {
            D_800C53DC[hx][hz].unkE = 0;
        }
    }
    for (k = 0; k < 4; k++) {
        he = Heroes[k].hero_eng;
        hx = (he->unk10.x + 0x800) >> 9;
        hz = (he->unk10.z + 0x800) >> 9;
        if ((u32)hx < 8 && (u32)hz < 8) {
            D_800C53DC[hx][hz].unkE = k + 1;
        }
    }
    if (D_800C5BAC != 0) {
        eng = *(EXTengine **)(D_800C5BAC + 0x54);
        if (eng != NULL) {
            x = (eng->unk10.x + 0x800) >> 9;
            z = (eng->unk10.z + 0x800) >> 9;
            if ((u32)x < 8 && z >= 0 && z < 8) {
                D_800C53DC[x][z].unkE = 5;
            }
        }
    }
}

void func_800B4374(s32 arg0, u16 arg1) {
    u8 *g;
    s32 i;
    s32 found;

    switch (D_8005A648) {
    case 10:
    case 12:
        found = -1;
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_mode == 0x3B && (D_800C535C[i].flags & 1)) {
                found = i;
            }
        }
        if (found == -1) {
            break;
        }
        if (D_8005A614[found + 0x5C] != 0) {
            return;
        }
        g = D_8005A614;
        g[0x60] = 0;
        ((D5A614View *)g)->unk30 |= arg1;
        for (i = 0; i < 4; i++) {
            if (g[i + 0x5C] != 0) {
                func_80026980(Heroes[i].hero);
                Heroes[i].hero_status |= 0x400;
            } else if (i != found) {
                Heroes[i].hero_status |= 0x100;
            }
        }
        D_800AD12C = 1;
        return;
    case 11:
    case 13:
        break;
    default:
        return;
    }
    func_800B44F4(arg0, arg1);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B44F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B4A70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B4E50);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B53CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B574C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B5B1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B5D90);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl60/ovl60", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B5EE4);

void func_800B6370(s32 arg0) {
    s32 seq;

    func_800C2810(arg0);
    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    Heroes[arg0].hero_move->jump_ctrl |= 0x22;
    Heroes[arg0].hero_move->jump_ctrl &= 0xEF;
    func_800C159C(arg0);
    if (D_800C535C[arg0].unk4 == 5) {
        SetHeroSeq((HeroInfo *)arg0, 0x2F);
    } else {
        SetHeroSeq((HeroInfo *)arg0, 0x2E);
    }
}

void func_800B6438(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    s32 v;

    if (func_800C2B00(arg0) != 0) {
        return;
    }
    if (func_800C29F4(arg0) != 0) {
        return;
    }
    if (func_800C295C(arg0) != 0) {
        return;
    }
    if (func_800B6C14(arg0) != 0) {
        return;
    }
    if (func_800C2CBC(arg0) != 0) {
        return;
    }
    if (func_800B5D90(arg0) != 0) {
        return;
    }
    switch (func_8008502C(arg0)) {
    case 0x12:
        func_800B8AF8(arg0);
        func_800B8BE4(arg0, 1);
        if (D_800C535C[arg0].unk4 != 5 && Heroes[arg0].hero_aseq == 0x2F) {
            SetHeroSeq((HeroInfo *)arg0, 0x2E);
        }
        break;
    case 0x11:
        func_800B72B0(arg0);
        break;
    case 0x14:
        func_800B741C(arg0);
        break;
    case 0x13:
        func_800B7574(arg0);
        break;
    }
    base = Heroes;
    h = &base[arg0];
    switch (h->hero_name) {
    case 4:
        v = h->hero_sub->unk74 - 0x4080;
        D_800C52E0 = v;
        if (v > 0x10) {
            D_800C52E0 = 0;
        }
        break;
    case 3:
        v = h->hero_sub->unk74 - 0x4000;
        D_800C52E4 = v;
        if (v > 0x10) {
            D_800C52E4 = 0;
        }
        break;
    }
    func_800871E4(arg0);
}

void func_800B662C(s32 arg0) {
    HeroInfo *h;
    EXTengine *eng;
    EXTheromove *mv;
    HeroInfo *base;
    s32 d;

    if (func_800C2B00(arg0) != 0) {
        return;
    }
    if (func_800C29F4(arg0) != 0) {
        return;
    }
    if (func_800C295C(arg0) != 0) {
        return;
    }
    if (func_800B6C14(arg0) != 0) {
        return;
    }
    func_800BC0EC(arg0);
    base = Heroes;
    h = &base[arg0];
    h->hero_frozen = 10;
    if (func_8008502C(arg0) == 0x14) {
        func_800C2674(arg0);
    }
    eng = h->hero_eng;
    if (eng != NULL) {
        mv = h->hero_move;
        if (mv != NULL) {
            d = func_800154A0(eng->unk1E, mv->facing_angle);
            eng->unk5E = 1;
            eng->unk1E += d >> 3;
        }
    }
    if (--D_800C535C[arg0].unk6 <= 0) {
        Heroes[arg0].hero_frozen = 0;
        func_800C2B98(arg0);
        SetHeroAction((HeroInfo *)arg0, 0x39);
    }
    func_800871E4(arg0);
}

void func_800B6790(s32 arg0) {
    EXTengine *eng;
    u8 *g;

    Heroes[arg0].hero_frozen = 2;
    func_800C1CF0();
    if (func_8008A200(arg0) != 0) {
        g = D_8005A614;
        if (*(s32 *)(g + 4) & 0x800) {
            *(u16 *)(g + 0x30) |= 0x201;
        } else {
            *(u16 *)(g + 0x30) |= 1;
        }
    }
    switch (func_8008502C(arg0)) {
    case 0x12:
        func_800B8AF8(arg0);
        func_800B8BE4(arg0, 1);
        break;
    case 0x11:
        func_800B72B0(arg0);
        break;
    case 0x14:
        func_800B741C(arg0);
        break;
    case 0x13:
        func_800B7574(arg0);
        break;
    }
    switch (Heroes[arg0].hero_name) {
    case 4:
        D_800C52E0 = Random(0x10);
        break;
    case 3:
        D_800C52E4 = Random(0x10);
        break;
    }
    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        s32 r = func_80015480(0, eng->unk1E);
        eng->unk5E = 1;
        eng->unk1E += r >> 3;
    }
}

void func_800B6960(s32 arg0) {
    s32 *pos;
    NAS *nas;
    s32 *ext;
    u8 *p;

    func_800C2810(arg0);
    Heroes[arg0].hero_move->velocity_enable = 0;
    Heroes[arg0].hero_frozen = 2;
    func_800C1CF0(arg0);
    func_800C159C(arg0);
    Heroes[arg0].hero_move->jump_vel = 0x30;
    Heroes[arg0].hero_move->jump_max = 0x90;
    Heroes[arg0].hero_move->jump_acc = 0;
    Heroes[arg0].hero_move->jump_dec = 0;
    Heroes[arg0].hero_move->jump_dir = -1;
    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = D_80053B9C;
    nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 0;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
    }
    func_80026980(Heroes[arg0].hero);
    Heroes[arg0].hero_shadow->flags &= ~0x8000;
}

void func_800B6AE0(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *eng;
    s32 *pos;
    NAS *nas;
    s32 *ext;
    u8 *p;

    func_800C1CF0();
    base = Heroes;
    h = &base[arg0];
    eng = h->hero_eng;
    h->hero_frozen = 2;
    if (eng != NULL) {
        eng->unk1E = (eng->unk1E + 0xE3) % 4096;
        eng->unk5E = 1;
        if (((D_8004E0D8 + arg0) & 3) == 0) {
            pos = (s32 *)&eng->unk10;
            p = D_80053B9C;
            nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
            if (nas != NULL) {
                ext = FindExtension(nas, 0x10000000);
                if (ext != NULL) {
                    ext[3] = 2;
                    ext[6] = 0x100;
                    if (p != NULL) {
                        ext[8] = 5;
                        ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
                    } else {
                        ext[8] = 1;
                    }
                    ext[10] = 0;
                    nas->InitDelete(nas);
                }
            }
        }
    }
}

s32 func_800B6C14(s32 arg0) {
    HeroInfo *h;
    s32 x;
    s32 z;
    Ovl60Cell *c;
    HeroInfo *base = Heroes;

    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    if (!(h->hero_status & 0x800) && !(D_800C535C[arg0].flags & 1)) {
        return 0;
    }
    x = (h->hero_eng->unk10.x + 0x800) >> 9;
    z = (h->hero_eng->unk10.z + 0x800) >> 9;
    if (h->hero_mode != 0x3B && h->hero_last_mode != 0x3B) {
        switch (h->hero_name) {
        case 1:
            PlaySfx(3, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 3:
            PlaySfx(0x67, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 2:
            PlaySfx(0x35, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 5:
            PlaySfx(0x12F, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 6:
            PlaySfx(0x161, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 7:
            PlaySfx(0xFD, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 4:
            PlaySfx(0x99, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 8:
            PlaySfx(0xCB, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        }
    }
    SetHeroAction((HeroInfo *)arg0, 0x3B);
    if ((u32)x < 8 && z >= 0 && z < 8) {
        Ovl60Cell *cb = D_800C53DC[0];
        c = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)cb);
        c->unkE = arg0 + 1;
        c->unk10 = arg0 + 1;
    }
    return 1;
}

void func_800B6DF4(s32 arg0) {
    s32 x;
    s32 z;
    Ovl60Cell *c;
    HeroInfo *h;
    HeroInfo *base;

    func_800C2810(arg0);
    if (D_800C535C[arg0].nas != NULL) {
        func_800C2B98(arg0);
    }
    func_800C1CF0(arg0);
    func_80028018(arg0, 0x1E);
    func_800C159C(arg0);
    x = (Heroes[arg0].hero_eng->unk10.x + 0x800) >> 9;
    z = (Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9;
    if ((u32)x < 8 && z >= 0 && z < 8) {
        Ovl60Cell *cb = D_800C53DC[0];
        c = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)cb);
        c->unkE = arg0 + 1;
        c->unk10 = arg0 + 1;
    }
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 10;
    D_800C535C[arg0].unk8[1] = 0;
    SetHeroSeq((HeroInfo *)arg0, 0x31);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B6F34);

void func_800B72B0(s32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    subengine *s;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = D_80053B9C;
    nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 6;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    if (r != NULL) {
        eng = r->Engine;
        if (eng != NULL) {
            u32 a = (u32)(Random(360) << 12) / 360;
            eng->unk5E = 1;
            eng->unk1E = a;
            for (s = eng->sub; s != NULL; s = s->next) {
                if (s->unk74 != 0x2022) {
                    s->flags |= 0x20000000;
                }
            }
        }
    }
}

void func_800B741C(s32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    subengine *s;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = D_80053B9C;
    nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 4;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    if (r != NULL) {
        eng = r->Engine;
        if (eng != NULL) {
            u32 a = (u32)(Random(360) << 12) / 360;
            eng->unk5E = 1;
            eng->unk1E = a;
            for (s = eng->sub; s != NULL; s = s->next) {
                s->flags |= 0x20000000;
            }
        }
    }
}

void func_800B7574(s32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    subengine *s;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = D_80053B9C;
    nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 4;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    if (r != NULL) {
        eng = r->Engine;
        if (eng != NULL) {
            u32 a = (u32)(Random(360) << 12) / 360;
            eng->unk5E = 1;
            eng->unk1E = a;
            for (s = eng->sub; s != NULL; s = s->next) {
                s->flags |= 0x20000000;
            }
        }
    }
}

void func_800B76CC(s32 arg0) {
    VECTOR pos;
    HeroInfo *h;
    EXTengine *pu;
    subengine *sub;
    s32 state;
    s32 dy;
    HeroInfo *base = Heroes;

    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    pu = h->hero_powerup;
    if (pu != NULL) {
        sub = pu->sub;
        if (sub != NULL) {
            state = (u16)D_800C535C[arg0].unk4;
            if (state >= 7) {
                goto clear;
            }
            if (state != 0) {
                if (func_80015780(h->hero_eng, h->hero_sub, 0x80, &pos) != NULL) {
                    pu->unk10 = *(SLONG_XYZ *)&pos;
                    dy = D_800C4484[h->hero_name];
                } else {
                    dy = 0x280;
                    pu->unk10 = h->hero_eng->unk10;
                }
                pu->unk5E = 1;
                pu->unk10.y -= dy;
                pu->unk1E += 0x71;
                if ((u16)D_800C535C[arg0].unk4 != 6 || D_800C535C[arg0].unk6 >= 0x5B ||
                    !((D_8004E0D8 >> 2) & 1)) {
                    sub->flags |= 0x8000;
                    pu->flags |= 0x8000;
                    return;
                }
            }
        clear:
            sub->flags &= ~0x8000;
            pu->flags &= ~0x8000;
        }
    }
}

s32 func_800B788C(s32 arg0) {
    s32 i;
    s32 j;
    s32 count;
    EXTengine *eng;

    count = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (D_800C53DC[i][j].unkA == arg0) {
                func_800C2338(i, j, D_800C53DC[i][j].unkA, 10);
                count++;
            }
        }
    }
    if (count != 0) {
        eng = Heroes[arg0].hero_eng;
        func_800B7990(arg0, (eng->unk10.x + 0x800) >> 9, (eng->unk10.z + 0x800) >> 9, count);
    }
    return count;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B7990);

void func_800B7B1C(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *p;
    Ovl60Cell *base;

    if (D_800C535C[arg2].unk4 == 6) {
        base = D_800C53DC[0];
        p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
        p->unk12 = 10;
        p->unkA = arg2;
        p->unk14 = 0xF0;
        return;
    }
    {
        Ovl60Cell *b2 = D_800C53DC[0];
        p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)b2);
    }
    p->unk12 = 1;
    p->unkA = arg2;
    if (D_8005A648 == 0xB) {
        if (arg0 > 0) {
            func_800B7C3C(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            func_800B7C3C(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            func_800B7C3C(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            func_800B7C3C(arg0, arg1 + 1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B7C3C);

void func_800B8180(s32 arg0, s32 arg1) {
    HeroInfo *h;
    Ovl60Cell *c;
    Ovl60Cell *base;
    s32 *pos;
    NAS *nas;
    s32 *ext;
    s32 i;
    s32 near;
    s32 one;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        near = 0;
        if (abs((arg0 << 9) - (((h->hero_eng->unk10.x + 0x800) >> 9) << 9)) < 0x300) {
            near = abs((arg1 << 9) - (((h->hero_eng->unk10.z + 0x800) >> 9) << 9)) < 0x300;
        }
        if (near && !(h->hero_status & 0x800)) {
            func_800C142C(i);
        }
    }
    base = D_800C53DC[0];
    c = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
    c->unk12 = 1;
    c->unkA = -1;
    c->unk14 = 0;
    pos = &c->unk4->x;
    nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    one = 1;
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = one;
            ext[6] = 0x100;
            ext[8] = one;
            ext[10] = 0;
            nas->InitDelete(nas);
        }
    }
    switch ((u32)Random(4)) {
    case 0:
        PlaySfx(0x3EC, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x3ED, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x3EE, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x3EF, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B83AC);

void func_800B8660(s32 arg0, s32 arg1, s32 dx, s32 dz, const s32 arg4) {
    s32 z;
    s32 x;
    Ovl60Cell *c;
    u8 n;
    s16 t;
    s16 old;
    u8 *g;

    x = arg0;
    z = arg1;
    n = 2;
    t = 6;
    for (;;) {
        if ((u32)x >= 8 || (u32)z >= 8) {
            goto end;
        }
        c = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)D_800C53DC);
        if ((u16)c->unk12 - 5 >= 3u) {
            old = c->unkA;
            c->unk14 = t;
            c->unk12 = 0;
            c->unkA = arg4;
            c->unk16 = old;
            g = D_8005A614;
            if (*(s16 *)(g + 0x34) == 0xB) {
                if (x > 0) {
                    func_800B7C3C(x - 1, z);
                }
                if (x < 7) {
                    func_800B7C3C(x + 1, z);
                }
                if (z > 0) {
                    func_800B7C3C(x, z - 1);
                }
                if (z < 7) {
                    func_800B7C3C(x, z + 1);
                }
            }
            ((Ovl60Cell *)(x * sizeof(D_800C53DC[0]) + z * sizeof(Ovl60Cell) + (u32)D_800C53DC))->unk18[0] = n;
            n++;
            t += 4;
        }
        x += dx;
        z += dz;
    }
end:;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B87DC);

void func_800B8AF8(s32 arg0) {
    switch (Heroes[arg0].hero_name) {
    case 4:
        PlaySfx(0x561, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x560, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 5:
        PlaySfx(0x562, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 6:
        PlaySfx(0x563, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x55E, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x55F, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 7:
        PlaySfx(0x564, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 8:
        PlaySfx(0x565, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

void func_800B8BE4(s32 arg0, s32 arg1) {
    EXTengine *eng;
    Ovl60Cell *c;
    Ovl60Cell *p;
    s32 x;
    s32 z;
    s32 owner;

    eng = Heroes[arg0].hero_eng;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    if ((u32)x >= 8) {
        return;
    }
    if ((u32)z >= 8) {
        return;
    }
    {
        Ovl60Cell *cb = D_800C53DC[0];
        c = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)cb);
    }
    c->unk10 = 0;
    if (arg1 != 0) {
        D_800C5ADC[arg0].unk4 = 1;
    }
    if (c->unkA != arg0) {
        if (arg1 != 0) {
            func_800B8DFC(arg0);
        }
        *(u16 *)c->unkC |= 0x10;
    }
    if (func_800C1EA8(arg0, x, z, arg1) != 0) {
        return;
    }
    if (func_800C1DA4(arg0, x, z, arg1) != 0) {
        return;
    }
    {
        Ovl60Cell *cb = D_800C53DC[0];
        p = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)cb);
    }
    owner = p->unkA;
    if (owner == -1) {
        func_800B7B1C(x, z, arg0);
        func_800B7C3C(x, z);
        func_800C1A18(arg0);
        return;
    }
    if (owner == arg0) {
        return;
    }
    switch ((u16)p->unk12) {
    case 1:
    case 3:
        func_800B7B1C(x, z, arg0);
        func_800B7C3C(x, z);
        func_800C1A18(arg0);
        break;
    case 5:
        func_800B8180(x, z);
        func_800B7C3C(x, z);
        break;
    case 6:
        func_800B83AC(x, z);
        func_800B7C3C(x, z);
        break;
    }
}

void func_800B8DFC(u32 arg0) {
    switch (arg0) {
    case 0:
        PlaySfx(0x55A, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x55B, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x55C, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x55D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

void func_800B8EC8(u32 arg0) {
    switch (arg0) {
    case 0:
        PlaySfx(0x54E, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x54F, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x550, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x551, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

void func_800B8F94(s32 arg0) {
    s32 i;
    s32 j;
    s32 n;
    Ovl60Cell *c;

    n = 0;
    for (i = 0;; i++) {
        if (i >= 8) goto done;
        for (j = 0;; j++) {
            if (j >= 8) goto next;
            c = (Ovl60Cell *)(i * sizeof(D_800C53DC[0]) + j * sizeof(Ovl60Cell) + (u32)D_800C53DC);
            if (c->unkA == arg0) {
                c->unk12 = 1;
                c->unkA = -1;
                c->unk14 = 0;
                func_800B7C3C(i, j);
                n++;
            }
        }
    next:;
    }
done:
    if (n != 0) {
        PlaySfx(0x54B, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void func_800B9080(s32 arg0) {
    NAS *nas;
    s32 i;
    s32 j;
    s32 n;
    u16 st;
    Ovl60Cell *c;

    nas = D_800C5BB0;
    if (nas == NULL) {
        return;
    }
    n = 0;
    if (nas->UserInfo.status < 3) {
        return;
    }
    nas->UserInfo.status = 6;
    i = 0;
    for (;;) {
        j = 0;
        if (i >= 8) {
            goto done;
        }
        for (;;) {
            if (j >= 8) {
                goto next;
            }
            c = (Ovl60Cell *)(j * sizeof(Ovl60Cell) + i * sizeof(D_800C53DC[0]) + (u32)D_800C53DC);
            st = c->unk12;
            if (st == 1 && c->unkA == arg0) {
                c->unk12 = 10;
                c->unkA = arg0;
                c->unk14 = 0xF0;
                func_800B7C3C(i, j);
                if (n == 0) {
                    c->unk18[0] = st;
                } else {
                    c->unk18[0] = 0;
                }
                n++;
            }
            j++;
        }
    next:
        i++;
    }
done:
    if (n != 0) {
        PlaySfx(0x54C, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void func_800B91E8(s32 arg0) {
    s32 i;
    s32 j;
    s32 n;
    Ovl60Cell *c;

    n = 0;
    i = 0;
    for (;;) {
        j = 0;
        if (i >= 8) {
            goto done;
        }
        for (;;) {
            if (j >= 8) {
                goto next;
            }
            c = (Ovl60Cell *)(j * sizeof(Ovl60Cell) + i * sizeof(D_800C53DC[0]) + (u32)D_800C53DC);
            if ((u16)c->unk12 == 10 && c->unkA == arg0) {
                c->unk12 = 1;
                c->unkA = arg0;
                if (((D5A614View *)D_8005A614)->mode == 0xB) {
                    if (i > 0) {
                        func_800B7C3C(i - 1, j);
                    }
                    if (i < 7) {
                        func_800B7C3C(i + 1, j);
                    }
                    if (j > 0) {
                        func_800B7C3C(i, j - 1);
                    }
                    if (j < 7) {
                        func_800B7C3C(i, j + 1);
                    }
                }
                func_800B7C3C(i, j);
                n++;
            }
            j++;
        }
    next:
        i++;
    }
done:
    if (n != 0) {
        PlaySfx(0x54D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B935C);

void func_800B9C10(void) {
    D5A614View *g = (D5A614View *)D_8005A614;
    s32 idx = g->mode - 10;
    s32 x, z;

    if (D_800C52E8 == -1) {
        if (D_800C52EC < D_800C38D8[idx]) {
            D_800C52E8 = Random(70) + 40;
        }
    } else {
        D_800C52E8--;
        if (D_800C52E8 < 5) {
            if (func_800C165C(&x, &z) != 0) {
                if (g->mode == 10) {
                    goto seven;
                }
                if (g->mode == 11) {
                    if (Random(3) == 0) {
                        func_800BA3E0(x, z, 8);
                    } else {
                    seven:
                        func_800BA3E0(x, z, 7);
                    }
                }
            }
            if (D_800C52EC > D_800C38E8[idx]) {
                D_800C52E8 = -1;
            } else {
                D_800C52E8 = Random(70) + 40;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B9D64);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BA018);

s32 func_800BA110(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *c;
    Ovl60Cell *base;
    HeroInfo *h;
    s32 action;
    s32 lock;
    s32 t;

    if ((u32)arg1 >= 8 || (u32)arg2 >= 8) {
        return 0;
    }
    if (func_800C0AFC(arg1, arg2) != 0) {
        return 1;
    }
    base = D_800C53DC[0];
    c = (Ovl60Cell *)(arg2 * sizeof(Ovl60Cell) + arg1 * sizeof(D_800C53DC[0]) + (u32)base);
    if (c->unkE != 0) {
        return 0;
    }
    if (c->unk10 != 0 && c->unk10 != arg0 + 1) {
        return 0;
    }
    {
        Ovl60Cell *b2 = D_800C53DC[0];
        c = (Ovl60Cell *)(arg2 * sizeof(Ovl60Cell) + arg1 * sizeof(D_800C53DC[0]) + (u32)b2);
    }
    h = c->hero;
    if (h != NULL) {
        action = h->hero_pu_action;
        lock = h->hero_pu_lock;
        t = *(s32 *)&h->hero_weapon_timer;
        if (action == 11) {
            return 0;
        }
        if (t != 0) {
            return 0;
        }
        if (action == 10) {
            if (lock < 3) {
                return 0;
            }
        } else if (lock < 3) {
            return 0;
        }
        return 1;
    }
    return (u16)c->unk12 == 7;
}

void func_800BA25C(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *c;
    Ovl60CellPos *pos;
    NAS *nas;
    EXTengine *e;
    Ovl60Cell *base;

    base = D_800C53DC[0];
    c = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
    if (c->hero != NULL || func_800C0AFC(arg0, arg1) != 0) {
        return;
    }
    pos = c->unk4;
    if (pos == NULL) {
        return;
    }
    nas = SpawnNASById(pos->x, D_8005A5D8, pos->z, 0x1402, 0x11);
    if (nas == NULL) {
        return;
    }
    c->hero = (HeroInfo *)nas;
    e = nas->Engine;
    if (e != NULL) {
        nas->UserInfo.flags = arg2;
        switch (arg2) {
        case 1:
            e->sub->unk6C = D_80053B9C;
            e->sub->unk74 = 0x200D;
            break;
        case 2:
            e->sub->unk6C = D_80053B9C;
            e->sub->unk74 = 0x201B;
            break;
        case 4:
            e->sub->unk6C = D_80053B9C;
            e->sub->unk74 = 0x2040;
            break;
        case 5:
            e->sub->unk6C = D_80053B9C;
            e->sub->unk74 = 0x200C;
            break;
        case 3:
            e->sub->unk6C = D_80053B9C;
            e->sub->unk74 = 0x2002;
            break;
        }
    }
    D_800C52F0++;
}

void func_800BA3E0(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *c;
    Ovl60CellPos *pos;
    NAS *nas;
    EXTengine *e;
    Ovl60Cell *base;

    base = D_800C53DC[0];
    c = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
    if (c->hero != NULL || func_800C0AFC(arg0, arg1) != 0) {
        return;
    }
    pos = c->unk4;
    if (pos == NULL) {
        return;
    }
    nas = SpawnNASById(pos->x, D_8005A5D8, pos->z, 0x1401, 0x11);
    if (nas == NULL) {
        return;
    }
    c->hero = (HeroInfo *)nas;
    e = nas->Engine;
    nas->UserInfo.flags = arg2;
    if (e != NULL) {
        switch (arg2) {
        case 7:
            e->sub->unk6C = D_80053B9C;
            e->sub->unk74 = 0x2001;
            nas->UserInfo.user1 = Random(4);
            nas->UserInfo.user2 = 0;
            break;
        case 8:
            e->sub->unk6C = D_80053B9C;
            e->sub->unk74 = 0x200E;
            nas->UserInfo.user1 = Random(4);
            nas->UserInfo.user2 = 0;
            break;
        }
    }
    D_800C52EC++;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BA51C);

void Plot_PogoTazer(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    subengine *sub;
    Ovl60Slot *slot;
    HeroInfo *h;
    s32 n;
    s32 i;
    s32 x;
    s32 z;
    s32 hx;
    s32 hz;
    s32 dir;
    s32 hit;
    Ovl60Slot *slots;
    HeroInfo *heroes;

    eng = nas->Engine;
    n = 0;
    if (eng == NULL) {
        return;
    }
    slots = D_800C535C;
    heroes = Heroes;
    sub = eng->sub;
next_sub:
    i = 0;
    if (sub != NULL) {
        slot = slots;
        h = heroes;
        sub->unk74 = ((D_8004E0D8 + n++) & 7) + 0x2003;
        x = (eng->unk10.x + 0x800) >> 9;
        z = (eng->unk10.z + 0x800) >> 9;
    next_hero:
        if (i < 4) {
            if (slot->nas == nas) {
                goto skip;
            }
            hit = 0;
            if (h->hero_mode == 0x3B) {
                goto skip;
            }
            he = h->hero_eng;
            hx = (he->unk10.x + 0x800) >> 9;
            hz = (he->unk10.z + 0x800) >> 9;
            dir = eng->unk1E >> 10;
            switch (dir) {
            case 0:
                if (hz == z && !(hx < x)) {
                    hit = 1;
                }
                break;
            case 1:
                if (hx == x && !(z < hz)) {
                    hit = 1;
                }
                break;
            case 2:
                if (hz == z && !(x < hx)) {
                    hit = 1;
                }
                break;
            case 3:
                if (hx == x && !(hz < z)) {
                    hit = 1;
                }
                break;
            }
            if (hit) {
                func_800C142C(i);
                if (slot->unk4 == 5 && h->hero_aseq == 0x2F) {
                    SetHeroAction((HeroInfo *)i, 0x39);
                }
                if (slot->unk4 == 6) {
                    func_800B91E8(i);
                }
                slot->unk4 = 0;
            }
        skip:
            slot++;
            h++;
            i++;
            goto next_hero;
        }
        sub = sub->next;
        goto next_sub;
    }
}

void Plot_PogoPickup(NAS *nas) {
    EXTengine *eng;
    s32 dir;
    s32 scale;
    s32 d;
    s32 state;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    state = nas->UserInfo.flags;
    if (state >= 9) {
        return;
    }
    if (state < 7) {
        return;
    }
    dir = nas->UserInfo.user1 & 3;
    switch (nas->UserInfo.status) {
    case 0:
    case 1:
        PlaySfx(0x3FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        eng->unk2C.x = 0x100;
        eng->unk2C.y = 0x100;
        eng->unk2C.z = 0x100;
        eng->flags |= 0x21;
        eng->sub->unk76 = 0x100;
        nas->UserInfo.status = 2;
        eng->unk1E = dir << 10;
        eng->unk5E = 1;
        break;
    case 2:
        scale = eng->unk2C.x;
        scale += 0x100;
        if (scale >= 0x1000) {
            nas->UserInfo.status = 3;
            scale = 0x1000;
            nas->UserInfo.user2 = D_800C38F8[D_800C5BB8];
            eng->flags &= ~0x21;
        }
        eng->unk2C.x = scale;
        eng->unk2C.y = scale;
        eng->unk2C.z = scale;
        eng->sub->unk76 = scale;
        eng->unk5E = 1;
        break;
    case 3:
        if (nas->UserInfo.user2 != 0) {
            nas->UserInfo.user2--;
        }
        d = func_80015480(dir << 10, eng->unk1E) >> 3;
        eng->unk5E = 1;
        eng->unk1E += d;
        if (d < 2) {
            nas->UserInfo.user1 = (nas->UserInfo.user1 + 1) & 3;
        }
        break;
    case 6:
        KillNAS(nas);
        D_800C52EC--;
        break;
    }
}

void Init_PogoPowerup(NAS *nas) {
    EXTengine *e;
    EXTengine *e2;
    EXTmotion *m;
    subengine *sub;

    e = nas->Engine;
    if (e != NULL) {
        e->sub->unk6C = D_80053B9C;
        e->sub->unk74 = 0x200B;
        e->sub->unk68 = -0x10;
        e->sub->flags |= 0x02000000;
        e->unk10.y -= 0x1000;
        m = AddMotion(nas);
        if (m != NULL) {
            m->Ymax = 0xA0;
            m->Ydec = 0xE;
            m->Yacc = 0;
            m->Ydir = 0xFF;
            m->Yvel = 0;
        } else {
            nas->Failure = 1;
        }
        e2 = AddEngine(nas, 1);
        if (e2 != NULL) {
            sub = e2->sub;
            e2->unk10.x = e->unk10.x;
            e2->unk10.y = e->unk10.y;
            e2->unk10.z = e->unk10.z;
            if (sub != NULL) {
                sub->unk6C = D_80053B9C;
                sub->unk74 = 0x2029;
                sub->unk51 = 2;
                sub->unk50 = 1;
                sub->unk76 = 0x800;
                sub->flags |= 0x40008041;
            }
        }
        PlaySfx(0x41F, NULL, &e->unk10, 0x1000, 0x1E00, 0);
    }
    nas->UserInfo.flags = 0;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = 0xF0;
    nas->UserInfo.user2 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Plot_PogoPowerup(NAS *nas) {
    EXTengine *eng;
    EXTengine *e2;
    subengine *sub;
    EXTmotion *mot;
    s32 scale;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    switch (nas->UserInfo.flags) {
    case 1:
    case 2:
    case 4:
        eng->unk5E = 1;
        eng->unk1E += 0x38;
        break;
    case 5:
        break;
    }
    switch (nas->UserInfo.flags) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    default:
        return;
    }
    e2 = eng->Engine;
    if (e2 != NULL) {
        sub = e2->sub;
        e2->unk10.x = eng->unk10.x;
        e2->unk10.z = eng->unk10.z;
        e2->unk10.y = D_8005A5D8;
        if (sub != NULL) {
            scale = 0;
            if (abs(eng->unk10.y - D_8005A5D8) < 0x200) {
                scale = 0x1000 - abs(eng->unk10.y - D_8005A5D8) * 6;
            }
            sub->unk20.z = scale;
            sub->unk20.y = scale;
            sub->unk20.x = scale;
            sub->unk52 = 1;
        }
    }
    switch (nas->UserInfo.status) {
    case 0:
    case 3:
        if (nas->UserInfo.user2 != 0) {
            nas->UserInfo.user2--;
        }
        if (func_8007A26C(nas) == 0) {
            break;
        }
        if (nas->UserInfo.status == 0) {
            nas->UserInfo.user2 = D_800C38F8[D_800C5BB8];
            nas->UserInfo.status = 3;
            PlaySfx(0x420, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        mot = FindExtension(nas, 0x800);
        if (mot != NULL) {
            mot->Ymax = 0x200;
            mot->Ydec = 8;
            mot->Ydir = 0xFF;
            mot->Yacc = 0;
            mot->Yvel = 0x40;
        }
        break;
    case 6:
        KillNAS(nas);
        D_800C52F0--;
        break;
    }
}

void Plot_PogoBanker(NAS *nas) {
    s32 x;
    s32 z;
    EXTengine *eng;
    Ovl60CellPos *pos;
    Ovl60Cell *c;
    s32 scale;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 1;
        }
        break;
    case 1:
        if (func_800C165C(&x, &z) != 0) {
            Ovl60Cell *cb = D_800C53DC[0];
            c = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)cb);
            pos = c->unk4;
            c->hero = (HeroInfo *)nas;
            if (pos != NULL) {
                eng->unk10.x = pos->x;
                eng->unk10.y = D_8005A5D8;
                eng->unk10.z = pos->z;
                eng->unk2C.x = 0;
                eng->unk2C.y = 0;
                eng->unk2C.z = 0;
                eng->flags |= 0x8021;
                eng->unk5E = 1;
                nas->UserInfo.status = 2;
                nas->UserInfo.user2 = D_800C38F8[D_800C5BB8];
                PlaySfx(0x3FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 2:
        scale = eng->unk2C.x + 0x100;
        if (scale >= 0x1000) {
            scale = 0x1000;
            eng->flags &= ~0x21;
            nas->UserInfo.status = 3;
        }
        eng->unk2C.z = scale;
        eng->unk2C.y = scale;
        eng->unk2C.x = scale;
        eng->unk5E = 1;
        break;
    case 3:
        if (nas->UserInfo.user2 != 0) {
            nas->UserInfo.user2--;
        }
        break;
    case 4:
        eng->flags &= ~0x8000;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = Random(0x28) + 0x28;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoMushroom);

void func_800BB724(s32 arg0, s32 arg1) {
    s32 i;
    s32 j;
    Ovl60Cell *c;
    s16 old;
    s16 k;

    for (i = 0;; i++) {
        if (i >= 8) goto done;
        for (j = 0, k = 10;; j++) {
            if (j >= 8) goto next;
            c = (Ovl60Cell *)(i * sizeof(D_800C53DC[0]) + j * sizeof(Ovl60Cell) + (u32)D_800C53DC);
            if (c->unkA == arg1) {
                old = c->unkA;
                c->unk16 = old;
                c->unkA = arg0;
                c->unk14 = k;
                c->unk12 = 0;
                if (D_8005A648 == 0xB) {
                    if (i > 0) {
                        func_800B7C3C(i - 1, j);
                    }
                    if (i < 7) {
                        func_800B7C3C(i + 1, j);
                    }
                    if (j > 0) {
                        func_800B7C3C(i, j - 1);
                    }
                    if (j < 7) {
                        func_800B7C3C(i, j + 1);
                    }
                }
            }
        }
    next:;
    }
done:;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BB840);

s32 func_800BBB08(s32 *arg0, const s32 arg1, s32 arg2) {
    s32 i;
    s32 j;
    s32 n;
    Ovl60Cell *c;
    u16 t;
    s32 k;
    s32 r;

    n = 0;
    for (j = 0;; j++) {
        if (j >= 8) goto done;
        for (i = 0;; i++) {
            r = j * 8;
            k = 10;
            if (i >= 8) goto next;
            if (arg0[r + i] != 0) continue;
            c = (Ovl60Cell *)(i * sizeof(D_800C53DC[0]) + j * sizeof(Ovl60Cell) + (u32)D_800C53DC);
            if (c->unkE != 0) continue;
            t = c->unk12;
            if (t == 6) continue;
            if (t == 5) continue;
            if (t == 7) continue;
            if (arg2 != 0) {
                c->unk14 = k;
                c->unk12 = 0;
                c->unk16 = c->unkA;
                c->unkA = arg1;
                if (D_8005A648 == 0xB) {
                    if (i > 0) {
                        func_800B7C3C(i - 1, j);
                    }
                    if (i < 7) {
                        func_800B7C3C(i + 1, j);
                    }
                    if (j > 0) {
                        func_800B7C3C(i, j - 1);
                    }
                    if (j < 7) {
                        func_800B7C3C(i, j + 1);
                    }
                }
            }
            n++;
        }
    next:;
    }
done:
    return n;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BBC94);

s32 func_800BBF00(s32 *arg0, s32 *arg1) {
    s32 x;
    s32 y;
    s32 i;
    s32 found;
    s32 ok;

    i = 0;
loop:
    x = Random(8);
    y = Random(8);
    found = func_800BC04C(x, y) ^ 1;
    i++;
    if (found && i < 10) {
        goto loop;
    }
    if (i >= 10) {
        found = 0;
    } else {
        *arg0 = x;
        *arg1 = y;
        found = 1;
    }
    if (!found) {
        return 0;
    }
    ok = 1;
    if (*arg0 > 0) {
        ok = func_800C1608(*arg0 - 1, *arg1) == 0;
    }
    if (*arg0 < 7 && func_800C1608(*arg0 + 1, *arg1)) {
        ok = 0;
    }
    if (*arg1 > 0 && func_800C1608(*arg0, *arg1 - 1)) {
        ok = 0;
    }
    if (*arg1 < 7 && func_800C1608(*arg0, *arg1 + 1)) {
        ok = 0;
    }
    return ok;
}

s32 func_800BC04C(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
    s32 s;

    if (p->unkE != 0 || p->unk10 != 0 || p->hero != NULL) {
        return 0;
    }
    s = (u16)p->unk12;
    if (s == 5) {
        return 0;
    }
    if (s == 6) {
        return 0;
    }
    return func_800C0AFC(arg0, arg1) == 0;
}

void func_800BC0EC(s32 arg0) {
    s32 x = Heroes[arg0].hero_eng->unk10.x;
    s32 tx = (((x + 0x800) >> 9) << 9) - 0x700;
    s32 tz = (((Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9) << 9) - 0x700;

    Heroes[arg0].hero_eng->unk10.x = x - (x - tx) / 4;
    Heroes[arg0].hero_eng->unk10.z -= (Heroes[arg0].hero_eng->unk10.z - tz) / 4;
}

void func_800BC184(s32 arg0, s32 arg1) {
    EXTheromove *move;
    EXTengine *eng;
    s32 buttons;
    s32 angle;
    s32 mode;

    func_800BC490(arg0);
    move = Heroes[arg0].hero_move;
    if (move != NULL) {
        buttons = ((Ovl60Pad *)D_80051338)[arg0].unk50;
        angle = move->facing_angle;
        if (buttons & 0xF0) {
            if (buttons & 0x80) {
                angle = 0x400;
            }
            if (buttons & 0x40) {
                angle = 0;
            }
            if (buttons & 0x20) {
                angle = 0xC00;
            }
            if (buttons & 0x10) {
                angle = 0x800;
            }
        }
        mode = 1;
        if (arg1 != 0) {
            mode = 3;
        }
        func_800C1384(arg0, mode, angle, 0x180);
        eng = Heroes[arg0].hero_eng;
        PlaySfx(0x522, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        if (D_800C535C[arg0].unk4 == 5 && Heroes[arg0].hero_aseq == 0x2F) {
            SetHeroAction((HeroInfo *)arg0, 0x39);
        }
        if (D_800C535C[arg0].unk4 == 6) {
            func_800B91E8(arg0);
        }
        D_800C535C[arg0].unk4 = 0;
    }
}

void func_800BC314(s32 arg0, s32 arg1) {
    HeroInfo *h;
    EXTengine *eng;
    s32 x;
    s32 z;
    Ovl60Slot *base;
    Ovl60Slot *slot;
    HeroInfo *heroes;

    heroes = Heroes;
    h = &heroes[arg0];
    eng = h->hero_eng;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    func_800BC490(arg0);
    if (x > 0) {
        func_800C1384(arg0, 2, 0x400, 0x180);
    }
    if (x < 7) {
        func_800C1384(arg0, 2, 0xC00, 0x180);
    }
    if (z > 0) {
        func_800C1384(arg0, 2, 0, 0x180);
    }
    if (z < 7) {
        func_800C1384(arg0, 2, 0x800, 0x180);
    }
    PlaySfx(0x43C, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
    if (D_800C535C[arg0].unk4 == 5 && h->hero_aseq == 0x2F) {
        SetHeroAction((HeroInfo *)arg0, 0x39);
    }
    base = D_800C535C;
    slot = &base[arg0];
    if (slot->unk4 == 6) {
        func_800B91E8(arg0);
    }
    slot->unk4 = 0;
}

void func_800BC490(s32 arg0) {
    switch (Heroes[arg0].hero_name) {
    case 1:
        PlaySfx(0x10, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x74, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x42, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 5:
        PlaySfx(0x13C, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 6:
        PlaySfx(0x16E, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 7:
        PlaySfx(0x10A, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 4:
        PlaySfx(0xA6, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 8:
        PlaySfx(0xD8, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoProjectile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BCB4C);

void func_800BD008(s32 arg0) {
    D_800C5BD0 = arg0;
    switch (arg0) {
    case 1:
        D_800C5300 = 0;
        D_800C5BCC = 8;
        D_800C5BC4 = 8;
        D_800C5BC8 = 8;
        break;
    case 2:
        D_800C5300 = 0;
        D_800C5BCC = 8;
        D_800C5BC4 = 8;
        D_800C5BC8 = 0x10;
        break;
    case 3:
        D_800C5300 = 1;
        D_800C5BCC = 8;
        D_800C5BC4 = 8;
        D_800C5BC8 = 8;
        break;
    case 4:
        D_800C5300 = 1;
        D_800C5BCC = 8;
        D_800C5BC4 = 8;
        D_800C5BC8 = 0x10;
        break;
    case 6:
        D_800C5BCC = 0x18;
        D_800C5BC4 = 0x18;
        D_800C5300 = D_8004E0D8 & 1;
        D_800C5BC8 = (Random(2) + 2) * 8;
        break;
    }
}

void func_800BD10C(s32 arg0) {
    EXTbotai *ai;
    s32 r;

    D5A614View *g = (D5A614View *)D_8005A614;

    if (!(*(s32 *)&g->unkC[4] & 0x04000000)) {
        ai = Heroes[arg0].hero_ai;
        if (ai != NULL) {
            ai->AIpad = 0;
            if (ai->AItimer1 != 0) {
                ai->AItimer1--;
            }
            if (D_800C5ADC[arg0].unk4 != 0 &&
                (u16)(g->mode - 0xB) < 2U) {
                if (ai->AItimer2 != 0) {
                    ai->AItimer2--;
                }
            }
            switch (ai->AImode) {
            case 1:
                r = func_800BDE24(arg0) != 1;
                if (r) {
                    ai->TargetXYZ.x = Random(8);
                    ai->TargetXYZ.z = Random(8);
                    ai->AImode = 3;
                }
                break;
            case 3:
                r = func_800BDE24(arg0) != 1;
                if (r) {
                    func_800BD790(arg0);
                }
                break;
            case 4:
                r = func_800BDE24(arg0) != 1;
                if (r) {
                    ai->AImode = 1;
                    break;
                }
            case 5:
            case 6:
            case 7:
                func_800BD790(arg0);
                break;
            }
            if (ai->AItimer1 == 0 || (D_8005A646 & 0x20)) {
                switch (D_800C535C[arg0].unk4) {
                case 1:
                case 3:
                case 4:
                    r = func_800BD378(arg0);
                    goto check;
                case 2:
                    r = func_800BD520(arg0);
                check:
                    if (r != 0) {
                        ai->AIpad |= 0x8000;
                    }
                    break;
                }
            }
            func_80013C94((u8 *)&D_80051338[arg0], ai->AIpad);
        }
        D_800C5ADC[arg0].unk4 = 0;
    }
}

s32 func_800BD378(s32 hero) {
    u8 *g;
    HeroInfo *p;
    EXTengine *eng;
    s32 mate;
    u32 i;
    s32 x, z, ox, oz;
    s32 dir;
    s32 hit;

    if (D_8005A680[0] != 0) {
        mate = Heroes[hero].hero_teammate;
    } else {
        mate = 1000;
    }
    i = 0;
    eng = Heroes[hero].hero_eng;
    g = D_8005A614;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    dir = Heroes[hero].hero_move->facing_angle >> 10;
    p = Heroes;
loop:
    {
        hit = 0;
        if (i == hero || i == mate) {
            goto next;
        }
        ox = (p->hero_eng->unk10.x + 0x800) >> 9;
        oz = (p->hero_eng->unk10.z + 0x800) >> 9;
        switch (dir) {
        case 0:
            if (oz < z && x == ox) {
                hit = 1;
            }
            break;
        case 1:
            if (ox < x && z == oz) {
                hit = 1;
            }
            break;
        case 2:
            if (z < oz && x == ox) {
                hit = 1;
            }
            break;
        case 3:
            if (x < ox && z == oz) {
                hit = 1;
            }
            break;
        }
        if (hit) {
            if ((*(u16 *)(g + 0x32) & 0x20) && g[i + 0x5C] != 0) {
                return 0;
            }
            return 1;
        }
    }
next:
    i++;
    p++;
    if (i < 4) {
        goto loop;
    }
    return 0;
}

s32 func_800BD520(s32 hero) {
    u8 *g;
    HeroInfo *p;
    EXTengine *eng;
    s32 mate;
    u32 i;
    s32 x, z, ox, oz;

    if (D_8005A680[0] != 0) {
        mate = Heroes[hero].hero_teammate;
        i = 0;
    } else {
        mate = 1000;
        i = 0;
    }
    eng = Heroes[hero].hero_eng;
    g = D_8005A614;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    p = Heroes;
loop:
    if (i == hero) {
        goto next;
    }
    do {
        if (i == mate) {
            goto next;
        }
    } while (0);
    ox = (p->hero_eng->unk10.x + 0x800) >> 9;
    oz = (p->hero_eng->unk10.z + 0x800) >> 9;
    if (x != ox) {
        do {
            if (z != oz) {
                goto next;
            }
        } while (0);
    }
    if (*(u16 *)(g + 0x32) & 0x20) {
        do {
            if (g[i + 0x5C] != 0) {
                return 0;
            }
        } while (0);
    }
    return 1;
next:
    i++;
    p++;
    if (i < 4) {
        goto loop;
    }
    return 0;
}

s32 func_800BD630(s32 hero) {
    u8 *g;
    HeroInfo *p;
    EXTengine *eng;
    s32 mate;
    u32 i;
    s32 x, z;

    if (D_8005A680[0] != 0) {
        mate = Heroes[hero].hero_teammate;
        i = 0;
    } else {
        mate = 1000;
        i = 0;
    }
    eng = Heroes[hero].hero_eng;
    g = D_8005A614;
    p = Heroes;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    for (;;) {
        if (i >= 4) {
            goto end;
        }
        if (i != hero && i != mate) {
            if ((u32)(csqrt(func_800C16FC(x, z, p->hero_eng->unk10.x, p->hero_eng->unk10.z)) >> 6) < 0x381) {
                if ((*(u16 *)(g + 0x32) & 0x20) && g[i + 0x5C] != 0) {
                    return 0;
                }
                return 1;
            }
        }
        p++;
        i++;
    }
end:
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BD790);

s32 func_800BDCB8(s32 arg0, s32 x, s32 z, s32 dx, s32 dz, s32 flags) {
    Ovl60Cell *p;
    HeroInfo *h;
    u8 *g;
    u16 t;
    u16 a;

    x += dx;
    z += dz;
    if ((u32)x >= 8 || (u32)z >= 8) {
        return 0;
    }
    p = &D_800C53DC[x][z];
    if (p == NULL) {
        return 0;
    }
    if (p->unkE != 0) {
        return 0;
    }
    if (p->unk10 != 0 && p->unk10 != arg0 + 1) {
        return 0;
    }
    g = D_8005A614;
    if (g[arg0 + 0x5C] == 0 && !(flags & 1)) {
        goto one;
    }
    t = p->unk12;
    if (t == 5) {
        return 0;
    }
    if (t == 6) {
        return 0;
    }
    h = p->hero;
    if (h != NULL) {
        a = h->hero_pu_action;
        if (a == 0xB) {
            return 0;
        }
        if (*(s16 *)(g + 0x34) != 0xD) {
            goto one;
        }
        if (a == 0xA) {
            return 1;
        }
    }
    if (D_8005A648 == 0xD && (flags & 2) && p->unkA == arg0 && D_800C535C[arg0].unk4 != 6) {
        return 0;
    }
one:
    return 1;
}

s32 func_800BDE24(s32 arg0) {
    s32 x;
    s32 z;
    EXTbotai *ai;

    if ((u16)D_8005A648 - 0xB < 2U) {
        ai = Heroes[arg0].hero_ai;
        if (ai != NULL && ai->AItimer2 == 0) {
            ai->AItimer2 = Random(3) + 3;
            if (func_800C0F08(arg0, ai) != 0) {
                return 1;
            }
            if (func_800BE864(arg0, (Ovl60Goal *)ai) != 0) {
                return 1;
            }
            if (func_800BE6C4(arg0, (Ovl60Goal *)ai) != 0) {
                return 1;
            }
        }
    }
    x = -1;
    z = -1;
    func_800BDF60(arg0, &x, &z);
    if (x != -1) {
        EXTbotai *ai2 = Heroes[arg0].hero_ai;
        if (ai2 != NULL) {
            ai2->TargetXYZ.x = x;
            ai2->AImode = 4;
            ai2->TargetXYZ.z = z;
            return 1;
        }
    }
    return 0;
}

void func_800BDF60(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 hx;
    s32 hz;
    u32 best;
    EXTengine *eng;
    Ovl60Cell *c;
    s32 i;
    s32 j;
    s32 ok;
    s32 dx;
    s32 dz;
    s32 cx;
    s32 cz;
    s32 px;
    s32 pz;
    u32 d;

    best = -1;
    eng = Heroes[arg0].hero_eng;
    hx = (eng->unk10.x + 0x800) >> 9;
    hz = (eng->unk10.z + 0x800) >> 9;
    i = 0;
    while (1) {
        if (i >= 8) {
            break;
        }
        j = 0;
        while (1) {
            if (j >= 8) {
                break;
            }
            if (func_800BA110(arg0, i, j) != 0) {
                HeroInfo *h;
                c = (Ovl60Cell *)(j * sizeof(Ovl60Cell) + i * sizeof(D_800C53DC[0]) + (u32)D_800C53DC);
                h = c->hero;
                ok = 0;
                if (h != NULL) {
                    do {
                        ok = D_800C535C[arg0].unk4 < h->hero_pu_action;
                    } while (0);
                } else if ((u16)c->unk12 != 7 || D_800C535C[arg0].unk4 < 9) {
                    ok = 1;
                }
                if (ok) {
                    cx = (i << 9) - 0x800;
                    px = (hx << 9) - 0x800;
                    cz = (j << 9) - 0x800;
                    pz = (hz << 9) - 0x800;
                    dx = abs(cx - px);
                    dz = abs(cz - pz);
                    d = dx * dx + dz * dz;
                    if (d < best) {
                        best = d;
                        *arg1 = i;
                        *arg2 = j;
                    }
                }
            }
            j++;
        }
        i++;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BE158);

s32 func_800BE6C4(s32 arg0, Ovl60Goal *arg1) {
    Ovl60Move *m2;
    EXTengine *eng;
    Ovl60Move *m;
    Ovl60Move *base;
    Ovl60Move *base2;
    s32 x;
    s32 z;
    s32 odd;
    s32 dir;

    if ((u16)D_8005A648 - 0xB >= 2U) {
        return 0;
    }
    if (func_800C0DE4(arg0) < 9) {
        return 0;
    }
    {
        eng = Heroes[arg0].hero_eng;
        if (eng != NULL) {
            base = D_800C5ADC;
            m = (Ovl60Move *)(arg0 * sizeof(Ovl60Move) + (u32)base);
            x = (eng->unk10.x + 0x800) >> 9;
            z = (eng->unk10.z + 0x800) >> 9;
            odd = D_8004E0D8 & 1;
            m->unkC = 0;
            m->unk10 = 8;
            m->unk0 = odd;
            if (x & 4) {
                if (z & 4) {
                    dir = odd * 2;
                } else if (odd != 0) {
                    dir = 4;
                } else {
                    dir = 6;
                }
            } else if (z & 4) {
                dir = (odd == 0) * 2;
            } else if (odd != 0) {
                dir = 6;
            } else {
                dir = 4;
            }
            m->unk8 = dir;
            base2 = D_800C5ADC;
            m2 = (Ovl60Move *)(arg0 * sizeof(Ovl60Move) + (u32)base2);
            if (m2->unk0 != 0) {
                arg1->unk14 = x + D_800C457C[m2->unk8].x;
                arg1->unk1C = z + D_800C457C[m2->unk8].z;
            } else {
                arg1->unk14 = x + D_800C45BC[m2->unk8].x;
                arg1->unk1C = z + D_800C45BC[m2->unk8].z;
            }
            arg1->unk20 = 5;
            return 1;
        }
    }
    return 0;
}

s32 func_800BE864(s32 hero, Ovl60Goal *goal) {
    EXTengine *eng;
    s32 ox;
    s32 oz;
    s32 best;
    s32 px;
    s32 pz;
    s32 bx;
    s32 bz;
    s32 i;
    s32 j;
    s32 ax;
    s32 cx;
    s32 az;
    s32 cz;
    s32 dx;
    s32 dy;
    s32 d;

    if (D_8005A648 == 0xC) {
        return 0;
    }
    eng = Heroes[hero].hero_eng;
    if (eng == NULL) {
        return 0;
    }
    best = 0x0FFFFFFF;
    ox = (eng->unk10.x + 0x800) >> 9;
    oz = (eng->unk10.z + 0x800) >> 9;
    bx = ox;
    bz = oz;
    px = ox << 9;
    pz = oz << 9;
    for (i = 0; ; i++) {
        if (i >= 8) {
            goto done;
        }
        for (j = 0; ; j++) {
            if (j >= 8) {
                goto next;
            }
            if (func_800BEA34(i, j, hero) != 0) {
                ax = px - 0x800;
                az = pz - 0x800;
                cx = (i << 9) - 0x800;
                cz = (j << 9) - 0x800;
                dx = abs(ax - cx);
                dy = abs(az - cz);
                d = dx * dx + dy * dy;
                if (d < best) {
                    best = d;
                    bx = i;
                    bz = j;
                }
            }
        }
    next:;
    }
done:
    if (bx != ox || bz != oz) {
        goal->unk14 = bx;
        goal->unk1C = bz;
        goal->unk20 = 6;
        return 1;
    }
    return 0;
}

s32 func_800BEA34(s32 x, s32 z, s32 hero) {
    s32 grid[64];
    s32 *g;
    Ovl60Cell *base;
    Ovl60Cell *c;
    s32 i;
    s32 j;

    if (func_800BEB6C(x, z, hero) < 2) {
        return 0;
    }
    base = D_800C53DC[0];
    c = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)base);
    if (c->unkE != 0) {
        return 0;
    }
    if (c->unkA == hero) {
        return 0;
    }
    g = grid;
    for (j = 0; j < 8; j++) {
        for (i = 0; i < 8; i++) {
            s32 *p = (s32 *)((j * 8 + i) * 4 + (u32)g);
            *p = 0;
            if (D_800C53DC[i][j].unkA == hero) {
                *p = 1;
            }
        }
    }
    grid[z * 8 + x] = 1;
    func_800BB840(grid, hero);
    return func_800BBB08(grid, hero, 0) != 0;
}

s32 func_800BEB6C(s32 arg0, s32 arg1, s32 arg2) {
    s32 n;

    n = 0;
    if (arg0 > 0) {
        s32 v = D_800C53DC[arg0 - 1][arg1].unkA;
        n = v == arg2;
    }
    if (arg0 < 7 && D_800C53DC[arg0 + 1][arg1].unkA == arg2) {
        n++;
    }
    if (arg1 > 0 && D_800C53DC[arg0][arg1 - 1].unkA == arg2) {
        n++;
    }
    if (arg1 < 7 && D_800C53DC[arg0][arg1 + 1].unkA == arg2) {
        n++;
    }
    if (arg0 == 0 && D_800C5B2C[arg1] == arg2) {
        n++;
    }
    if (arg0 == 7 && D_800C5B4C[arg1] == arg2) {
        n++;
    }
    if (arg1 == 0 && D_800C5B8C[arg0] == arg2) {
        n++;
    }
    if (arg1 == 7 && D_800C5B6C[arg0] == arg2) {
        n++;
    }
    return n;
}

void Init_PogoRipperRoo(NAS *nas) {
    EXTengine *e;
    EXTengine *e2;
    EXTmotion *m;
    subengine *sub;

    e = nas->Engine;
    if (e != NULL) {
        func_80024E30(nas, &D_800C4680);
        func_8001682C((s32 *)D_80053BD4);
        e->sub->unk6C = D_80053BD4;
        e->sub->unk74 = 0x2001;
        e->unk5D = 0;
        nas->UserInfo.status = 0;
        e->unk10.x = -0x100;
        e->unk10.z = 0x900;
        m = AddMotion(nas);
        if (m != NULL) {
            m->Ymax = 0xA0;
            m->Ydec = 0xE;
            m->Yacc = 0;
            m->Ydir = 0xFF;
            m->Yvel = 0;
        } else {
            nas->Failure = 1;
        }
        e2 = AddEngine(nas, 1);
        if (e2 != NULL) {
            sub = e2->sub;
            e2->unk10.x = e->unk10.x;
            e2->unk10.y = e->unk10.y;
            e2->unk10.z = e->unk10.z;
            if (sub != NULL) {
                sub->unk6C = D_80053B9C;
                sub->unk74 = 0x2029;
                sub->unk51 = 2;
                sub->unk50 = 1;
                sub->unk76 = 0x800;
                sub->flags |= 0x40000041;
            }
        }
    }
    D_800C5308 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
        D_800C5BAC = 0;
    } else {
        D_800C5BAC = (s32)nas;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoRipperRoo);

void Roo_UpdatePosition(EXTengine *eng) {
    u32 x;
    u32 z;
    s32 i;
    s32 j;
    s32 a;

    x = D_800C5BD8;
    z = D_800C5BDC;
    if (x < 8 && z < 8) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (D_800C53DC[i][j].unk10 == 5) {
                    D_800C53DC[i][j].unk10 = 0;
                }
            }
        }
        D_800C53DC[x][z].unk10 = 5;
    }
    if ((u32)(D_800C5304 - 2) < 10) {
        eng->unk10.x += D_800C5BE0;
        eng->unk10.z += D_800C5BE4;
    }
    a = func_800154A0(D_800C5BE8, eng->unk1E);
    eng->unk5E = 1;
    eng->unk1E -= a >> 1;
}

bool Roo_ChangeDirOnEdge(void) {
    if ((u32)(D_800C5BDC + 1) >= 10) {
        if (D_800C5BDC < -1) {
            D_800C5BDC = -1;
        }
        if (D_800C5BDC > 8) {
            D_800C5BDC = 8;
        }
        if (D_800C5BD8 == 8) {
            D_800C5BD8 = 7;
            D_800C5BEC = 1;
        } else if (D_800C5BD8 == -1) {
            D_800C5BD8 = 0;
            D_800C5BEC = 3;
        }
        return 1;
    }
    if (D_800C5BDC == 8 || D_800C5BDC == -1) {
        if (D_800C5BD8 == 8) {
            D_800C5BEC = (D_800C5BDC == 8) ? (D_800C5BEC == 2)
                                           : (D_800C5BEC == 0 ? 1 : 2);
            return 1;
        }
        if (D_800C5BD8 == -1) {
            D_800C5BEC = (D_800C5BDC == 8) ? (D_800C5BEC == 2 ? 3 : 0)
                                           : (D_800C5BEC == 0 ? 3 : 2);
            return 1;
        }
    }
    if ((u32)(D_800C5BD8 + 1) >= 10) {
        if (D_800C5BD8 < -1) {
            D_800C5BD8 = -1;
        }
        if (D_800C5BD8 > 8) {
            D_800C5BD8 = 8;
        }
        if (D_800C5BDC == 8) {
            D_800C5BDC = 7;
            D_800C5BEC = 0;
        } else if (D_800C5BDC == -1) {
            D_800C5BDC = 0;
            D_800C5BEC = 2;
        }
        return 1;
    }
    if (D_800C5BD8 == -1 || D_800C5BD8 == 8) {
        if (D_800C5BDC == 8) {
            D_800C5BEC = (D_800C5BD8 == 8) ? (D_800C5BEC == 2 ? 3 : 0)
                                           : (D_800C5BEC != 3);
            return 1;
        }
        if (D_800C5BDC == -1) {
            D_800C5BEC = (D_800C5BD8 == 8) ? ((D_800C5BEC == 1) * 2)
                                           : (D_800C5BEC == 3 ? 2 : 1);
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Roo_FindNewSquare);

void Roo_GoBoing(EXTengine *eng) {
    switch (Random(6)) {
    case 0:
        PlaySfx(0x53C, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x53D, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x53E, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x53F, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 4:
        PlaySfx(0x540, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 5:
        PlaySfx(0x541, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

void func_800BFFF4(EXTengine *eng) {
    NAS *nas;
    EXTengine *e;
    subengine *sub;
    subengine *s;
    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;
    s32 angle = eng->unk1E;

    nas = SpawnNASById(x, D_8005A5D8 - 0x200, z, 0x1404, 0x11);
    if (nas != NULL) {
        e = nas->Engine;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                nas->UserInfo.user2 = 1;
                nas->UserInfo.flags = 0x502;
                nas->UserInfo.user1 = 0;
                nas->UserInfo.status = angle;
                sub->unk6C = D_80053B9C;
                sub->unk74 = 0x200D;
                func_80018F3C(e, 8);
                for (s = sub->next; s != NULL; s = s->next) {
                    s->unk6C = D_80053B9C;
                    s->flags = (s->flags & ~0x8000) | 0x20000041;
                    s->unk20.z = 0x1000;
                    s->unk20.y = 0x1000;
                    s->unk20.x = 0x1000;
                    s->unk50 = 1;
                    s->unk51 = 0;
                    s->unk04 = 0x90;
                }
                sub->unk74 = 0x200D;
                e->unk1E = (angle + 0xC00) % 0x1000;
                e->unk5E = 1;
                nas->UserInfo.flags |= 1;
            }
        }
    }
}

void Roo_ShootAtHumans(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    s32 x, z, hx, hz;
    s32 i;

    eng = nas->Engine;
    i = 0;
    if (eng == NULL) {
        return;
    }
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    for (; i < 4; i++) {
        if (D_8005A614[i + 0x5C] != 0 || Heroes[i].hero_mode == 0x3B) {
            continue;
        }
        he = Heroes[i].hero_eng;
        hx = (he->unk10.x + 0x800) >> 9;
        hz = (he->unk10.z + 0x800) >> 9;
        if (x != hx && z != hz) {
            continue;
        }
        if (x == hx) {
            if (z < hz) {
                func_800C02AC(eng, 0x800);
            }
            if (hz < z) {
                func_800C02AC(eng, 0);
            }
        }
        if (z == hz) {
            if (x < hx) {
                func_800C02AC(eng, 0xC00);
            }
            if (hx < x) {
                func_800C02AC(eng, 0x400);
            }
        }
    }
}

void func_800C02AC(EXTengine *eng, s32 angle) {
    NAS *nas;
    EXTengine *e;
    subengine *sub;
    subengine *s;

    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;

    nas = SpawnNASById(x, D_8005A5D8 - 0x280, z, 0x1404, 0x11);
    if (nas != NULL) {
        e = nas->Engine;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                nas->UserInfo.user2 = 1;
                nas->UserInfo.flags = 0x502;
                nas->UserInfo.user1 = 0;
                nas->UserInfo.status = angle;
                sub->unk6C = D_80053B9C;
                sub->unk74 = 0x200D;
                func_80018F3C(e, 8);
                for (s = sub->next; s != NULL; s = s->next) {
                    s->unk6C = D_80053B9C;
                    s->flags = (s->flags & ~0x8000) | 0x20000041;
                    s->unk20.z = 0x1000;
                    s->unk20.y = 0x1000;
                    s->unk20.x = 0x1000;
                    s->unk50 = 1;
                    s->unk51 = 0;
                    s->unk04 = 0x90;
                }
                sub->unk74 = 0x200D;
                e->unk1E = (angle + 0xC00) % 0x1000;
                e->unk5E = 1;
                nas->UserInfo.flags |= 1;
            }
        }
    }
    PlaySfx(0x522, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
}

void Init_PogoPadlock(NAS *nas) {
    EXTengine *e;
    EXTengine *e2;
    subengine *sub;

    e = nas->Engine;
    if (e != NULL) {
        e->sub->unk6C = D_80053B9C;
        e->sub->unk74 = 0x2041;
        e->unk2C.z = 0x1000;
        e->unk2C.y = 0x1000;
        e->unk2C.x = 0x1000;
        e->unk5C = 1;
        e->flags &= ~0x8000;
        e2 = AddEngine(nas, 1);
        if (e2 != NULL) {
            sub = e2->sub;
            e2->unk10.x = e->unk10.x;
            e2->unk10.y = D_8005A5D8;
            e2->unk10.z = e->unk10.z;
            e2->flags &= ~0x8000;
            if (sub != NULL) {
                sub->unk74 = 0x2029;
                sub->unk51 = 2;
                sub->unk6C = D_80053B9C;
                sub->unk50 = 1;
                sub->unk76 = 0x800;
                sub->flags |= 0x40008041;
            }
        }
    } else {
        nas->Failure = 1;
    }
    nas->UserInfo.user1 = 0xF0;
    nas->UserInfo.status = 0;
    nas->UserInfo.flags = D_800C530C[0];
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoPadlock);

s32 func_800C0AFC(s32 arg0, s32 arg1) {
    EXTengine *e;

    if (D_800C5BB0 != NULL) {
        e = D_800C5BB0->Engine;
        if (e != NULL) {
            if ((u16)D_800C5BB0->UserInfo.status >= 3 && (e->unk10.x + 0x800) >> 9 == arg0
                && (e->unk10.z + 0x800) >> 9 == arg1) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800C0B70(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *p;

    if ((u32)arg1 >= 8 || (u32)arg2 >= 8) {
        return 0;
    }
    if (D_800C53DC[arg1][arg2].hero != NULL) {
        return 0;
    }
    p = &D_800C53DC[arg1][arg2];
    if (p == NULL) {
        return 0;
    }
    if (p->unkE != 0) {
        return 0;
    }
    return p->unk10 == 0;
}

void func_800C0BF0(s32 arg0, s32 arg1) {
    D_800C5BB0 = SpawnNASById((arg0 << 9) - 0x700, D_8005A5D8, (arg1 << 9) - 0x700, 0x1408, 0x11);
}

void func_800C0C34(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_8008D5D4((arg1 << 9) - 0x700, D_8005A5D8 - 0x100, (arg2 << 9) - 0x800, arg3, 0);
}

bool SquareIsFreeForRoo(s32 arg0, s32 arg1) {
    Ovl60Cell *p;
    u16 t;

    if ((u32)arg0 >= 8 || (u32)arg1 >= 8) {
        return 1;
    }
    p = &D_800C53DC[arg0][arg1];
    if (p == NULL) {
        return 0;
    }
    if (p->unkE != 0) {
        return 0;
    }
    t = p->unk10;
    if (t != 0 && t != 5) {
        return 0;
    }
    return 1;
}

bool Roo_Bounce(NAS *nas) {
    if (++D_800C5304 < 14) {
        return 0;
    }
    D_800C5304 = 0;
    if (nas->Engine != NULL) {
        Roo_GoBoing(nas->Engine);
    }
    return 1;
}

void Roo_UpdateVelocities(EXTengine *eng) {
    s32 dx;
    s32 dz;

    dx = (D_800C5BD8 << 9) - eng->unk10.x - 0x700;
    dz = (D_800C5BDC << 9) - eng->unk10.z - 0x700;
    D_800C5BE0 = dx / 10;
    D_800C5BE4 = dz / 10;
    D_800C5BE8 = get_angle(dx, dz) + 0x800;
}

s32 func_800C0DE4(s32 arg0) {
    s32 i;
    s32 j;
    s32 count;

    count = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (D_800C53DC[i][j].unkA == arg0) {
                count++;
            }
        }
    }
    return count;
}

s32 func_800C0E40(s32 arg0, u32 arg1, u32 arg2) {
    if (arg2 < 8) {
        if (arg1 == -1 && D_800C5B2C[arg2] == arg0) {
            return 1;
        }
        if (arg1 == 8 && D_800C5B4C[arg2] == arg0) {
            return 1;
        }
    }
    if (arg1 < 8) {
        if (arg2 == -1 && D_800C5B8C[arg1] == arg0) {
            return 1;
        }
        if (arg2 == 8 && D_800C5B6C[arg1] == arg0) {
            return 1;
        }
    }
    return 0;
}

s32 func_800C0F08(s32 arg0, EXTbotai *ai)
{
    s32 x;
    s32 z;

    EXTengine *e = Heroes[arg0].hero_eng;
    s32 a = Heroes[arg0].hero_move->facing_angle >> 10;

    if (func_800BE158(arg0, (e->unk10.x + 0x800) >> 9, (e->unk10.z + 0x800) >> 9,
                      a, &x, &z) != 0) {
        ai->AImode = 7;
        ai->TargetXYZ.x = x;
        ai->TargetXYZ.z = z;
        return 1;
    }
    return 0;
}

void func_800C0FAC(s32 i)
{
    EXTbotai *ai;

    ai = Heroes[i].hero_ai;
    if (ai != NULL) {
        ai->AItimer1 = D_800C3A38[D_800C5BBC][0];
    }
}

void func_800C1004(s32 i)
{
    EXTbotai *ai;

    ai = Heroes[i].hero_ai;
    if (ai != NULL) {
        ai->AImode = 1;
        ai->AItimer1 = 0;
        ai->AItimer2 = Random(3) + 3;
    }
}

void func_800C1068(void) {
    s32 i;
    u8 (*p)[8] = D_800C3C14[D_800C5BC0];

    for (i = 0; i < 8; i++) {
        D_800C5B2C[i] = p[0][i];
        D_800C5B6C[i] = p[1][i];
        D_800C5B4C[7 - i] = p[2][i];
        D_800C5B8C[7 - i] = p[3][i];
    }
}

void func_800C10F8(void) {
    D_800C5BC0 = 0;
    D_800C5BC4 = Random(0xC8) + 0xC8;
    D_800C5BD0 = 0;
    D_800C5BCC = 8;
    D_800C5BD4 = 0;
    D_800C5300 = 0;
    if (D_8005A646 & 0x80) {
        D_800C5BC4 = 0x78;
    }
    func_800C1068();
}

void Init_PogoProjectile(NAS *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s16*)((u8*)(arg0) + 94)) = 0;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS((NAS *)arg0);
    }
}

void func_800C11C4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    NAS *nas;
    EXTengine *e;
    subengine *sub;
    subengine *s;

    nas = SpawnNASById(arg1, D_8005A5D8 - arg3, arg2, 0x1404, 0x11);
    if (nas != NULL) {
        e = nas->Engine;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                nas->UserInfo.user1 = 0;
                nas->UserInfo.user2 = arg0;
                nas->UserInfo.status = arg5;
                nas->UserInfo.flags = ((arg4 & 7) << 8) | 2;
                sub->unk6C = D_80053B9C;
                sub->unk74 = 0x200D;
                func_80018F3C(e, 8);
                for (s = sub->next; s != NULL; s = s->next) {
                    s->unk6C = D_80053B9C;
                    s->flags = (s->flags & ~0x8000) | 0x20000041;
                    s->unk20.z = 0x1000;
                    s->unk20.y = 0x1000;
                    s->unk20.x = 0x1000;
                    s->unk50 = 1;
                    s->unk51 = 0;
                    s->unk04 = 0x90;
                }
                switch (arg0) {
                case 1:
                    sub->unk74 = 0x200D;
                    break;
                case 2:
                    sub->unk74 = 0x201B;
                    break;
                case 3:
                    sub->unk74 = 0x2040;
                    break;
                }
                e->unk1E = (arg5 + 0xC00) % 0x1000;
                e->unk5E = 1;
                nas->UserInfo.flags |= 1;
            }
        }
    }
}

void func_800C1384(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    EXTengine *eng;

    eng = Heroes[arg0].hero_eng;
    func_800C11C4(arg1, (((eng->unk10.x + 0x800) >> 9) << 9) - 0x700,
                  (((eng->unk10.z + 0x800) >> 9) << 9) - 0x700, arg3, arg0 & 7, arg2);
}

void func_800C1400(s32 arg0) {
    D_800C535C[arg0].flags |= 1;
}

void func_800C142C(s32 arg0) {
    Heroes[arg0].hero_status |= 0x800;
}

s32 func_800C1460(s32 arg0) {
    s32 i;
    s32 j;
    s32 count;

    count = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (D_800C53DC[i][j].unkA == arg0) {
                count++;
            }
        }
    }
    return count;
}

s32 func_800C14BC(s32 arg0, s32 arg1)
{
    s32 h;
    s32 n;
    s32 r;
    s32 *a = D_800C5074;

    h = Heroes[arg0].hero_name;
    n = a[h] - D_800C5050[h] - D_800C502C[h];
    r = 0x200 / n;
    if (arg1 != 0) {
        if (n & 1) {
            r = (0x200 / (n + 1)) * 2;
        } else {
            r *= 2;
        }
    }
    return r;
}

void func_800C159C(s32 arg0) {
    Heroes[arg0].hero_eng->unk10.x = (((Heroes[arg0].hero_eng->unk10.x + 0x800) >> 9) << 9) - 0x700;
    Heroes[arg0].hero_eng->unk10.z = (((Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9) << 9) - 0x700;
}

s32 func_800C1608(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    HeroInfo *hero = ((Ovl60Cell *)(arg0 * sizeof(D_800C53DC[0]) + arg1 * sizeof(Ovl60Cell) + (u32)base))->hero;

    if (hero != NULL && hero->hero_pu_action == 0xB) {
        return 1;
    }
    return 0;
}

s32 func_800C165C(s32 *arg0, s32 *arg1) {
    s32 x;
    s32 y;
    s32 i;
    s32 ok;

    i = 0;
loop:
    x = Random(8);
    y = Random(8);
    ok = func_800BC04C(x, y) != 1;
    i++;
    if (ok && i < 10) {
        goto loop;
    }
    if (i >= 10) {
        return 0;
    }
    *arg0 = x;
    *arg1 = y;
    return 1;
}

s32 func_800C16FC(x0, y0, x1, y1)
s32 x0;
s32 y0;
s32 x1;
s32 y1;
{
    s32 dx;
    s32 dy;
    s32 a;
    s32 b;

    a = (y0 << 9) - 0x800;
    b = (y1 << 9) - 0x800;
    dx = abs((x0 - x1) << 9);
    dy = abs(a - b);
    return dx * dx + dy * dy;
}

s32 func_800C1764(void) {
    return csqrt(func_800C16FC()) >> 6;
}

s32 func_800C178C(void) {
    D5A614View *g = (D5A614View *)D_8005A614;
    s32 v;

    switch (g->mode) {
    case 10:
        v = 0x303C;
        if (g->flags & 0x80) {
            v = 0x3033;
        }
        return v;
    case 11:
        v = 0x3052;
        if (g->flags & 0x80) {
            v = 0x301B;
        }
        return v;
    case 12:
        v = 0x3035;
        if (g->flags & 0x80) {
            v = 0x303F;
        }
        return v;
    case 13:
        return 0x3000;
    }
    return 0;
}

void func_800C1840(u32 x, u32 z, s32 id, s32 *seen) {
    if (x < 8 && z < 8 && D_800C53DC[x][z].unkA != id && seen[z * 8 + x] == 0) {
        seen[z * 8 + x] = 1;
        func_800C1840(x - 1, z - 1, id, seen);
        func_800C1840(x, z - 1, id, seen);
        func_800C1840(x + 1, z - 1, id, seen);
        func_800C1840(x - 1, z, id, seen);
        func_800C1840(x + 1, z, id, seen);
        func_800C1840(x - 1, z + 1, id, seen);
        func_800C1840(x, z + 1, id, seen);
        func_800C1840(x + 1, z + 1, id, seen);
    }
}

void func_800C19A8(s32 *arg0, s32 arg1) {
    s32 i;
    s32 j;
    s32 *p;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            p = (s32 *)((i * 8 + j) * sizeof(s32) + (u32)arg0);
            *p = 0;
            if (D_800C53DC[j][i].unkA == arg1) {
                *p = 1;
            }
        }
    }
}

void func_800C1A18(s32 arg0) {
    s32 grid[64];

    if (((D5A614View *)D_8005A614)->mode != 10 && ((D5A614View *)D_8005A614)->mode != 13) {
        func_800C19A8(grid, arg0);
        func_800BB840(grid, arg0);
        if (func_800BBB08(grid, arg0, 1) != 0) {
            PlaySfx(0x51D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            if (((D5A614View *)D_8005A614)->mode == 11) {
                func_800B788C(arg0);
            }
        }
    }
}

void Init_PogoMushroom(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053ABC;
        e->sub->unk74 = 0x2007;
        e->flags &= ~0x8000;
    }
    nas->UserInfo.flags = 11;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        D_800C52F4++;
    }
}

void Init_PogoBanker(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053B9C;
        e->sub->unk74 = 0x200B;
        e->flags &= ~0x8000;
    }
    nas->UserInfo.flags = 10;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = Random(60) + 60;
    nas->UserInfo.user2 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Init_PogoPickup(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053B9C;
        e->sub->unk74 = 0x200B;
        e->sub->unk68 = -0x10;
        e->sub->flags |= 0x02000000;
    }
    nas->UserInfo.flags = 0;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = 0xF0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Init_PogoTazer(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053B9C;
        e->sub->unk74 = 0x2005;
        e->unk5D = 2;
    }
    nas->UserInfo.flags = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800C1CF0(arg0)
s32 arg0;
{
    Ovl60Slot *base;
    Ovl60Slot *slot;

    if (D_800C535C[arg0].unk4 == 5 && Heroes[arg0].hero_aseq == 0x2F) {
        SetHeroAction((HeroInfo *)arg0, 0x39);
    }
    base = D_800C535C;
    slot = &base[arg0];
    if (slot->unk4 == 6) {
        func_800B91E8(arg0);
    }
    slot->unk4 = 0;
}

s32 func_800C1DA4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *g = D_8005A614;
    Ovl60Cell *base;
    Ovl60Cell *p;

    if (((D5A614View *)g)->mode != 13) {
        return 0;
    }
    if ((((D5A614View *)g)->flags & 0x80) && g[arg0 + 0x5C] == 0) {
        return 1;
    }
    base = D_800C53DC[0];
    p = (Ovl60Cell *)(arg2 * sizeof(Ovl60Cell) + arg1 * sizeof(D_800C53DC[0]) + (u32)base);
    if ((u16)p->unk12 == 10) {
        if (arg3 != 0) {
            func_800B8EC8(arg0);
        }
        return 1;
    }
    if (p->unkA != arg0) {
        return 0;
    }
    if (*(u16 *)p->unkC & 0x10) {
        return 0;
    }
    func_800B8F94(arg0);
    func_800B7C3C(arg1, arg2);
    return 1;
}

s32 func_800C1EA8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Ovl60Cell *base;
    Ovl60Cell *c;

    if (D_8005A648 == 0xC) {
        base = D_800C53DC[0];
        c = (Ovl60Cell *)(arg2 * sizeof(Ovl60Cell) + arg1 * sizeof(D_800C53DC[0]) + (u32)base);
        if ((u16)c->unk12 == 7) {
            func_800B7B1C(arg1, arg2, arg0);
            if (arg1 > 0) {
                func_800B8660(arg1, arg2, -1, 0, arg0);
            }
            if (arg1 < 7) {
                func_800B8660(arg1, arg2, 1, 0, arg0);
            }
            if (arg2 > 0) {
                func_800B8660(arg1, arg2, 0, -1, arg0);
            }
            if (arg2 < 7) {
                func_800B8660(arg1, arg2, 0, 1, arg0);
            }
            func_800C1A18(arg0);
            if (arg3 != 0) {
                PlaySfx(0x515, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
            return 1;
        }
        return 0;
    }
    return 0;
}

void func_800C2018(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 > 0) {
        func_800B8660(arg0, arg1, -1, 0, arg2);
    }
    if (arg0 < 7) {
        func_800B8660(arg0, arg1, 1, 0, arg2);
    }
    if (arg1 > 0) {
        func_800B8660(arg0, arg1, 0, -1, arg2);
    }
    if (arg1 < 7) {
        func_800B8660(arg0, arg1, 0, 1, arg2);
    }
}

void func_800C20C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    NAS *nas = (NAS *)D_800C53DC[arg0][arg1].hero;
    s32 dir;
    s32 i;
    s32 dx;
    s32 dz;

    if (nas != NULL) {
        if (nas->Engine != NULL) {
            PlaySfx(0x515, NULL, &nas->Engine->unk10, 0x1000, 0x1E00, 0);
        }
        dir = nas->UserInfo.user1 & 3;
        for (i = 0; i < arg3; i++) {
            switch (dir) {
            case 0:
                dx = 0;
                dz = 1;
                break;
            case 1:
                dx = 1;
                dz = 0;
                break;
            case 2:
                dx = 0;
                dz = -1;
                break;
            case 3:
                dx = -1;
                dz = 0;
                break;
            default:
                dx = 0;
                dz = 0;
                break;
            }
            func_800B8660(arg0, arg1, dx, dz, arg2);
            dir ^= 2;
        }
    }
}

void func_800C2204(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if ((u16)D_800C53DC[i][j].unk12 == 6) {
                func_800B83AC(i, j);
            }
        }
    }
}

void func_800C2294(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
    p->unk12 = 4;
    p->unk14 = arg3;
    p->unk16 = arg2;
    p->unkA = arg2;
}

void func_800C22E8(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
    p->unk12 = 2;
    p->unk14 = arg3;
    p->unk16 = arg2;
}

void func_800C2338(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
    p->unk12 = 3;
    p->unk14 = arg3;
    p->unk16 = arg2;
}

void func_800C2388(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
    s16 old = p->unkA;

    p->unkA = arg2;
    p->unk14 = arg3;
    p->unk12 = 0;
    p->unk16 = old;
    if (D_8005A648 == 0xB) {
        if (arg0 > 0) {
            func_800B7C3C(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            func_800B7C3C(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            func_800B7C3C(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            func_800B7C3C(arg0, arg1 + 1);
        }
    }
}

void func_800C244C(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 7;
}

void func_800C2488(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 6;
    p->unk14 = D_800C3998[D_800C5BB8][0];
}

void func_800C24E4(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 5;
    p->unk14 = 0x78;
}

void func_800C2524(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 10;
    p->unkA = arg2;
    p->unk14 = 0xF0;
}

void func_800C2564(s32 arg0, s32 arg1, s16 arg2) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = arg2;
    if (D_8005A648 == 0xB) {
        if (arg0 > 0) {
            func_800B7C3C(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            func_800B7C3C(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            func_800B7C3C(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            func_800B7C3C(arg0, arg1 + 1);
        }
    }
}

void func_800C2620(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
}

s32 func_800C2660(u8 *arg0) {
    return (*(s32*)((u8*)(arg0) + 20)) - D_8005A5D8;
}

void func_800C2674(s32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = D_80053B9C;
    nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 7;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    r->Engine->unk10.y -= 0x40;
}

void func_800C2770(s32 arg0, u32 arg1, u32 arg2) {
    s32 i;
    s32 j;

    if (arg1 < 8 && arg2 < 8) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (D_800C53DC[i][j].unk10 == arg0 + 1) {
                    D_800C53DC[i][j].unk10 = 0;
                }
            }
        }
        D_800C53DC[arg1][arg2].unk10 = arg0 + 1;
    }
}

void func_800C2810(s32 arg0) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (D_800C53DC[i][j].unk10 == arg0 + 1) {
                D_800C53DC[i][j].unk10 = 0;
            }
        }
    }
}

void func_800C286C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *eng;

    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_frozen = 2;
    func_800C1CF0();
    eng = h->hero_eng;
    if (eng != NULL) {
        s32 r = func_80015480(0, eng->unk1E);
        eng->unk5E = 1;
        eng->unk1E += r >> 3;
    }
    func_8008502C(arg0);
}

void func_800C28FC(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800C2810(arg0);
    func_800C1CF0(arg0);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
}

s32 func_800C295C(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x100) {
        if (D_800C535C[arg0].nas != NULL) {
            func_800C2B98(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, 0x3E);
        SetHeroSeq((HeroInfo *)arg0, 0x2E);
        return 1;
    }
    return 0;
}

s32 func_800C29F4(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x400) {
        if (D_800C535C[arg0].nas != NULL) {
            func_800C2B98(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, 0x3D);
        SetHeroSeq((HeroInfo *)arg0, 0x2E);
        return 1;
    }
    return 0;
}

void func_800C2A8C(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800C2810(arg0);
    func_80089FA4(arg0);
    func_800C1CF0(arg0);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
    D_800AD12C = 1;
}

s32 func_800C2B00(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x200) {
        if (D_800C535C[arg0].nas != NULL) {
            func_800C2B98(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, 0x3C);
        SetHeroSeq((HeroInfo *)arg0, 0x32);
        return 1;
    }
    return 0;
}

void func_800C2B98(s32 arg0) {
    Ovl60Slot *base = D_800C535C;
    Ovl60Slot *slot = (Ovl60Slot *)(arg0 * sizeof(Ovl60Slot) + (u32)base);

    if (slot->nas != NULL) {
        KillNAS(slot->nas);
        slot->nas = NULL;
    }
}

void func_800C2BE8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    EXTheromove *move;

    func_800C2810(arg0);
    SetHeroSeq((HeroInfo *)arg0, 0x30);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    move = h->hero_move;
    if (move != NULL) {
        move->velocity_enable = 0;
        move->forward_angle = move->facing_angle;
    }
    h->hero_frozen = 0xA;
    D_800C535C[arg0].unk6 = 0x14;
}

s32 func_800C2C70(s32 arg0) {
    Ovl60Slot *base = D_800C535C;
    Ovl60Slot *slot = (Ovl60Slot *)(arg0 * sizeof(Ovl60Slot) + (u32)base);

    if (slot->nas != NULL) {
        func_800C2B98(arg0);
        return 1;
    }
    return 0;
}

s32 func_800C2CBC(s32 arg0) {
    if (D_80051338[arg0].unk54 & 0x8000) {
        switch (D_800C535C[arg0].unk4) {
        case 1:
            func_800BC184(arg0, 0);
            break;
        case 2:
            func_800BC314(arg0, 0);
            break;
        case 4:
            func_800BC184(arg0, 1);
            break;
        case 3:
            if (func_800BA51C(arg0, 0) != 0) {
                SetHeroAction((HeroInfo *)arg0, 0x3F);
                return 1;
            }
            break;
        }
    }
    return 0;
}

void func_800C2DCC(s32 arg0) {
    Ovl60Slot *slot;

    Heroes[arg0].hero_move->velocity_enable = 0;
    Heroes[arg0].hero_move->jump_ctrl &= 0xFE;
    slot = &D_800C535C[arg0];
    *(s16 *)((u8 *)slot + 0x16) = 0;
    slot->unk18 = (u16)slot->unk4 == 5;
    func_800B8BE4(arg0, 0);
}

void func_800C2E50(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            func_800B4374(arg0, (u16)arg1);
        } else if (f & 0x20) {
            func_800B4E50(arg0, (u16)arg1);
        } else if (f & 0x40) {
            func_800B4A70(arg0, (u16)arg1);
        } else {
            func_800B53CC(arg0, (u16)arg1);
        }
    }
}

void func_800C2EE0(void) {
    s32 i;
    u8 *p;

    for (i = 0; i < 4; i++) {
        p = D_8005A614 + i;
        if (p[0x5C] != 0) {
            D_800C5BB8 = D_8009D87C[i][0];
        }
        if (p[0x5C] == 0) {
            D_800C5BBC = D_8009D87C[i][2];
        }
    }
}

void func_800C2F50(void) {
    if (!(D_8005A646 & 0xE0)) {
        func_8007B7FC(0x130, 0x81D0);
    }
}

void func_800C2F84(void) {
    if (D_8005A646 & 0x20) {
        func_8007B7FC(0x130, 0x81D0);
    }
}

void func_800C2FB8(void) {
    u8 *g = D_8005A614;
    s32 i;

    if (*(u16 *)(g + 0x32) & 0x80) {
        if (*(s16 *)(g + 0x34) == 10) {
            D_800C52F4 = 0;
            for (i = 0; i < 2; i++) {
                SpawnNASById(0, 0, 0, 0x1407, 0x11);
            }
        }
        func_8007B7FC(0x130, 0x81D0);
    }
}

void func_800C3048(void) {
    s32 i;
    s32 n;

    n = 0;
    switch (D_8005A648) {
    case 10:
        n = 2;
        break;
    case 11:
        func_800C10F8();
        break;
    case 12:
        n = 3;
        SpawnNASById(0, 0, 0, 0x1406, 0x11);
        break;
    case 13:
        n = 2;
        func_800C0BF0(4, 4);
        break;
    }
    if (n != 0) {
        for (i = 0; i < n; i++) {
            SpawnNASById(0, 0, 0, 0x1403, 0x11);
        }
    }
}

void func_800C3148(void) {
    ((Ovl60Cam *)D_80051640)->eyeX = 0;
    ((Ovl60Cam *)D_80051640)->eyeZ = 0xE98;
    ((Ovl60Cam *)D_80051640)->rotX = 0x1BE;
    ((Ovl60Cam *)D_80051640)->dist = 0x200;
    ((Ovl60Cam *)D_80051640)->eyeY = 0;
    ((Ovl60Cam *)D_80051640)->rotY = 0;
    ((Ovl60Cam *)D_80051640)->rotZ = 0;
    ((Ovl60Cam *)D_80051640)->atX = 0;
    ((Ovl60Cam *)D_80051640)->atY = 0;
    ((Ovl60Cam *)D_80051640)->atZ = -0x650;
}

void func_800C318C(s32 arg0) {
    func_8007B7FC(arg0 & 0xFFFF, 0x81D0);
}

void func_800C31B0(void) {
    func_8007EB1C(0x140, 0x28, 0);
    D_800C5BB0 = 0;
    D_800C5BAC = 0;
    D_800C52FC = 0;
    D_800C52E0 = 0;
    D_800C52E4 = 0;
    D_8005A5D8 = -0x10;
    D_800C52EC = 0;
    D_800C52F0 = 0;
    D_800C52F8 = 0;
    D_800C5BB8 = 0x10;
    D_800C5BBC = 0x10;
    func_800B3FF0();
    func_800C3148();
    func_800B3C44();
    func_800C3048();
    func_800B3808();
    func_800C2FB8();
    func_800C2F84();
    func_800B3A38();
    func_800C2F50();
}

void func_800C327C(s32 arg0) {
    Ovl60Slot *base = D_800C535C;
    Ovl60Slot *p = (Ovl60Slot *)(arg0 * sizeof(Ovl60Slot) + (u32)base);

    p->flags = 0;
    p->unk4 = 0;
    p->nas = NULL;
    D_800C53CC[arg0] = 5;
    SetHeroAction((HeroInfo *)arg0, 0x39);
    func_800C1004(arg0);
}

void func_800C32E8(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    if (s != NULL) {
        s->sub->unk6C = D_80053B9C;
        s->sub->unk74 = 0x2029;
        s->unk10 = h->hero_eng->unk10;
        s->flags |= 0x8041;
        s->sub->unk76 = 0x800;
        s->sub->flags |= 0x40000000;
    }
}

void func_800C3388(void) {
}

void func_800C3390(void) {
}

void func_800C3398(void) {
    D_8009D6E8.unk0 = (s32)func_800C327C;
    D_8009D6E8.unk44 = &D_8008AB04;
    D_8009D6E8.unk4 = &D_800C52CC;
    D_8009D6E8.unkC = &D_800C44A8;
    D_8009D6E8.unk20 = func_800B574C;
    D_8009D6E8.unk28 = func_800B5B1C;
    D_8009D6E8.unk64 = func_800C2E50;
    D_8009D6E8.unk5C = func_800B76CC;
    D_8009D6E8.unk18 = func_800C32E8;
    D_8009D6E8.unk38 = func_800BD10C;
    func_800B3534();
}

void func_800C3434(void) {
    D_8005AA70[0] = (s32)func_800C3398;
    D_8005AA70[1] = (s32)func_800C31B0;
    D_8005AA70[2] = (s32)func_800C31B0;
    D_8005AA70[3] = (s32)func_800B3E34;
    D_8005AA70[5] = (s32)func_800C3388;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[9] = (s32)func_800C2660;
    func_8001E934(&D_8009F8A4, (s32)D_800C52B8);
}

void func_800C34B0(void) {
    func_800183DC();
}

void func_800C34D0(void) {
    func_80018430(0x80);
}

void func_800C34F0(s32 arg0) {
    func_80018430(arg0 * 2);
}

void func_800C3510(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 84)) = func_800C353C;
    (*(s32*)((u8*)(arg0) + 88)) = D_8001C378;
    (*(s32*)((u8*)(arg0) + 0)) = (s32) ((*(s32*)((u8*)(arg0) + 0)) | 0x01000000);
}

void func_800C353C(ModelObj *obj, void *arg1, s32 flags) {
    POLY_GT4 *p0;
    POLY_GT4 *p1;
    GfxTexInfo *ti;

    func_8001AA48(obj, arg1, flags);
    p0 = obj->slots[0].prim;
    p1 = obj->slots[1].prim;
    if (p0 != NULL && p1 != NULL) {
        if (obj->unk8E != 0) {
            ti = &obj->model->texs.tex[obj->unk8E & 0xFFF].info;
            *(u16 *)&p0->u0 = ti->uv;
            *(u16 *)&p0->u1 = ti->uv1;
            *(u16 *)&p0->u2 = ti->uv2;
            *(u16 *)&p0->u3 = ti->uv3;
            p0->tpage = ti->tpage;
            *(u16 *)&p1->u0 = ti->uv;
            *(u16 *)&p1->u1 = ti->uv1;
            *(u16 *)&p1->u2 = ti->uv2;
            *(u16 *)&p1->u3 = ti->uv3;
            p1->tpage = ti->tpage;
            obj->unk8E = 0;
        }
        if (obj->unk8C != 0) {
            p0->clut = obj->unk8C;
            p1->clut = obj->unk8C;
            obj->unk8C = 0;
        }
    }
}
