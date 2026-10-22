#include "common.h"
#include "bash/main.h"
#include "bash/ovl60.h"
#include "bash/pogo.h"

void func_800B3534(void) {
    D_8009D8F4 = &D_800C36DC;
    switch (((Ovl60ModeView *)D_8005A614)->mode) {
    case 10:
        D_8009D8FC = ((Ovl60ModeView *)D_8005A614)->teams != 0 ? &D_800C3658 : &D_800C3644;
        D_8009D904 = D_8005A680[0] != 0 ? &D_800C369C : &D_800C3684;
        D_8009D8F8 = D_8005A680[0] != 0 ? &D_800C36C8 : &D_800C36B4;
        D_8009D900 = &D_800C366C;
        break;
    case 11:
        D_8009D8FC = ((Ovl60ModeView *)D_8005A614)->teams != 0 ? &D_800C3704 : &D_800C36F0;
        D_8009D904 = D_8005A680[0] != 0 ? &D_800C3748 : &D_800C3730;
        D_8009D8F8 = D_8005A680[0] != 0 ? &D_800C3774 : &D_800C3760;
        D_8009D900 = &D_800C3718;
        break;
    case 12:
        D_8009D8FC = ((Ovl60ModeView *)D_8005A614)->teams != 0 ? &D_800C379C : &D_800C3788;
        D_8009D904 = D_8005A680[0] != 0 ? &D_800C37E0 : &D_800C37C8;
        D_8009D8F8 = D_8005A680[0] != 0 ? &D_800C380C : &D_800C37F8;
        D_8009D900 = &D_800C37B0;
        break;
    case 13:
        D_8009D8FC = ((Ovl60ModeView *)D_8005A614)->teams != 0 ? &D_800C3834 : &D_800C3820;
        D_8009D904 = D_8005A680[0] != 0 ? &D_800C3878 : &D_800C3860;
        D_8009D8F8 = D_8005A680[0] != 0 ? &D_800C38A4 : &D_800C3890;
        D_8009D900 = &D_800C3848;
        break;
    }
}

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
        Player_SnapToTile(i);
        Set_Tile_Under_Player(i, 0);
        slot->unk14 = Player_CalcForwardSpeed(i, 0);
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
            Tile_Update(i, j);
        }
    }
}

void UpdatePOGO(void) {
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

void func_800B3FF0(void) {
    Ovl60ModeView *g;
    Ovl60ModeView *h;
    Ovl60ModeEntry *e;
    s32 i;

    e = NULL;
    switch (((Ovl60ModeView *)D_8005A614)->mode) {
    case 10:
        if (((Ovl60ModeView *)D_8005A614)->teams != 0) {
            e = &D_800C36C8;
        } else {
            e = &D_800C36B4;
        }
        break;
    case 11:
        if (((Ovl60ModeView *)D_8005A614)->teams != 0) {
            e = &D_800C3774;
        } else {
            e = &D_800C3760;
        }
        break;
    case 12:
        if (((Ovl60ModeView *)D_8005A614)->teams != 0) {
            e = &D_800C380C;
        } else {
            e = &D_800C37F8;
        }
        break;
    case 13:
        if (((Ovl60ModeView *)D_8005A614)->teams != 0) {
            e = &D_800C38A4;
        } else {
            e = &D_800C3890;
        }
        break;
    }
    if (e != NULL) {
        g = (Ovl60ModeView *)D_8005A614;
        switch (g->mode) {
        case 10:
            e->unkE = D_800C3678[g->unk38];
            e->unk10 = D_800C3678[g->unk38 + 3];
            break;
        case 11:
            e->unkE = D_800C3724[g->unk38];
            e->unk10 = D_800C3724[g->unk38 + 3];
            break;
        case 12:
            e->unkE = D_800C37BC[g->unk38];
            e->unk10 = D_800C37BC[g->unk38 + 3];
            break;
        case 13:
            e->unkE = D_800C3854[g->unk38];
            e->unk10 = D_800C3854[g->unk38 + 3];
            break;
        }
        g = (Ovl60ModeView *)D_8005A614;
        if ((g->flags & 1) || (g->flags & 2)) {
            i = 0;
            h = g;
            for (; i < 4; i++) {
                if (((u8 *)h)[i + 0x5C] == 0) {
                    D_8009D87C[i][2] = *((h->flags & 2) ? &e->unk10 : &e->unkE);
                }
            }
        }
    }
    func_800C2EE0();
}

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

void func_800B44F4(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    u8 *g;

    if (arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (i = 0; i < 4; i++) {
                if (!((taken >> i) & 1)) {
                    if (Heroes[i].hero_score > Heroes[best].hero_score || best == -1) {
                        best = i;
                    }
                }
            }
            D_8005A614[j + 0xA7] = best;
            taken |= 1 << best;
        }
        g = D_8005A614;
        if (*(s32 *)(g + 0x6C) == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g[g[0xA7] + 0x5C] != 0
                || (hb = Heroes, a = &hb[g[0xA7]], a->hero_score == hb[g[0xA8]].hero_score)) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
                D_800AD12C = 1;
            } else {
                D_8005A614[0x60] = a->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (D_8005A614[0xA7] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x400;
                    }
                }
            }
        } else {
            s32 ia;
            s32 ib;
            s32 ita;
            s32 itb;
            s32 sa;
            s32 sb;

            func_8007B120();
            D_800AD12C = 1;
            ia = *(s16 *)(g + 0x48);
            ib = *(s16 *)(g + 0x4A);
            ita = Heroes[ia].hero_teammate;
            itb = Heroes[ib].hero_teammate;
            sa = Heroes[ia].hero_score + Heroes[ita].hero_score;
            sb = Heroes[ib].hero_score + Heroes[itb].hero_score;
            if (sa == sb) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if ((sb < sa && g[ia + 0x5C] == 0) || (sa < sb && g[ib + 0x5C] == 0)) {
                u8 *w = D_8005A614;

                *(u16 *)(w + 0x30) |= 0x800;
                if (w[ia + 0x5C] == 0) {
                    w[0x60] = Heroes[ia].hero_name;
                    w[0x61] = Heroes[ita].hero_name;
                    Heroes[ia].hero_status |= 0x200;
                    Heroes[ita].hero_status |= 0x200;
                    Heroes[ib].hero_status |= 0x400;
                    Heroes[itb].hero_status |= 0x400;
                } else {
                    w[0x60] = Heroes[ib].hero_name;
                    w[0x61] = Heroes[itb].hero_name;
                    Heroes[ib].hero_status |= 0x200;
                    Heroes[itb].hero_status |= 0x200;
                    Heroes[ia].hero_status |= 0x400;
                    Heroes[ita].hero_status |= 0x400;
                }
            } else {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            }
        }
    }
}

void func_800B4A70(s32 arg0, u16 arg1) {
    Ovl60TeamView *g;
    Ovl60TeamView *p;
    Ovl60TeamView *q;
    s32 sa;
    s32 sb;
    s32 found;
    s32 done;
    s32 i;

    found = -1;
    done = 0;
    g = (Ovl60TeamView *)D_8005A614;
    if (g->teams != 0) {
        sa = Ovl60TeamScore(g->team0);
        sb = Ovl60TeamScore(g->team1);
        if (sa == 0) {
            do {
                if (g->out[g->team0] != 0) {
                    done = 1;
                } else {
                    OVL60_TEAM_LOST(g, g->team0, i, done);
                }
            } while (0);
        } else if (sb == 0) {
            if (g->out[g->team1] == 0) {
                OVL60_TEAM_LOST_LATE(g, g->team1, i, 0);
            } else {
                done = 1;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_score == 0) {
                found = i;
            }
        }
        if (found != -1) {
            p = (Ovl60TeamView *)D_8005A614;
            if (p->out[found] == 0) {
                OVL60_TEAM_LOST(p, found, i, 0);
            } else {
                done = 1;
            }
        }
    }
    if (done || arg0 == -1) {
        i = 0;
        q = (Ovl60TeamView *)D_8005A614;
        p = q;
        q->winner = 0;
        q->unk30 |= arg1;
        D_8009917C |= 0x200;
        for (; i < 4; i++) {
            if (D_8005A614[i + 0x5C] != 0) {
                Heroes[i].hero_status |= 0x400;
                func_80026980(Heroes[i].hero);
            } else {
                Heroes[i].hero_status |= 0x100;
            }
        }
        D_800AD12C = 1;
    }
}

void func_800B4E50(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    u8 *g;

    if (arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (i = 0; i < 4; i++) {
                if (!((taken >> i) & 1)) {
                    if (Heroes[i].hero_score > Heroes[best].hero_score || best == -1) {
                        best = i;
                    }
                }
            }
            D_8005A614[j + 0xA7] = best;
            taken |= 1 << best;
        }
        g = D_8005A614;
        if (*(s32 *)(g + 0x6C) == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g[g[0xA7] + 0x5C] != 0
                || (hb = Heroes, a = &hb[g[0xA7]], a->hero_score == hb[g[0xA8]].hero_score)) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
                D_800AD12C = 1;
            } else {
                D_8005A614[0x60] = a->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (D_8005A614[0xA7] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x400;
                    }
                }
            }
        } else {
            s32 ia;
            s32 ib;
            s32 ita;
            s32 itb;
            s32 sa;
            s32 sb;

            func_8007B120();
            D_800AD12C = 1;
            ia = *(s16 *)(g + 0x48);
            ib = *(s16 *)(g + 0x4A);
            ita = Heroes[ia].hero_teammate;
            itb = Heroes[ib].hero_teammate;
            sa = Heroes[ia].hero_score + Heroes[ita].hero_score;
            sb = Heroes[ib].hero_score + Heroes[itb].hero_score;
            if (sa == sb) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if ((sb < sa && g[ia + 0x5C] == 0) || (sa < sb && g[ib + 0x5C] == 0)) {
                u8 *w = D_8005A614;

                *(u16 *)(w + 0x30) |= 0x800;
                if (w[ia + 0x5C] == 0) {
                    w[0x60] = Heroes[ia].hero_name;
                    w[0x61] = Heroes[ita].hero_name;
                    Heroes[ia].hero_status |= 0x200;
                    Heroes[ita].hero_status |= 0x200;
                    Heroes[ib].hero_status |= 0x400;
                    Heroes[itb].hero_status |= 0x400;
                } else {
                    w[0x60] = Heroes[ib].hero_name;
                    w[0x61] = Heroes[itb].hero_name;
                    Heroes[ib].hero_status |= 0x200;
                    Heroes[itb].hero_status |= 0x200;
                    Heroes[ia].hero_status |= 0x400;
                    Heroes[ita].hero_status |= 0x400;
                }
            } else {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            }
        }
    }
}

void func_800B53CC(s32 arg0, u16 arg1) {
    Ovl60RankView *g;
    Ovl60RankView *p;
    HeroInfo *hb;
    HeroInfo *h;
    HeroInfo *top;
    HeroInfo *ta;
    HeroInfo *b;
    HeroInfo *am;
    HeroInfo *bm;
    s32 used;
    s32 i;
    s32 k;
    s32 n;
    s32 j;
    s32 best;
    s32 sa;
    s32 sb;

    if (arg0 != -1) {
        return;
    }
    used = 0;
    for (i = 0; i < 4; i++) {
        best = -1;
        for (j = 0; j < 4; j++) {
            if (!((used >> j) & 1) && (Heroes[j].hero_score > Heroes[best].hero_score || best == -1)) {
                best = j;
            }
        }
        RANK60V->order[i] = best;
        used |= 1 << best;
    }
    g = RANK60V;
    if (g->teams == 0) {
        h = Heroes;
        top = &h[g->order[0]];
        if (top->hero_score == h[g->order[1]].hero_score) {
            g->winner[0] = 0;
            g->unk30 |= arg1;
            for (k = 0; k < 4; k++) {
                Heroes[k].hero_status |= 0x100;
            }
            return;
        }
        p = g;
        p->winner[0] = top->hero_name;
        p->unk30 |= 0x800;
        for (n = 0; n < 4; n++) {
            if (p->order[0] == n) {
                h[n].hero_status |= 0x200;
            } else {
                h[n].hero_status |= 0x400;
            }
        }
        return;
    }
    func_8007B120();
    hb = Heroes;
    ta = &hb[g->team[0]];
    b = &hb[g->team[1]];
    am = &hb[ta->hero_teammate];
    bm = &hb[b->hero_teammate];
    sa = ta->hero_score + am->hero_score;
    sb = b->hero_score + bm->hero_score;
    if (sa == sb) {
        g->winner[0] = 0;
        g->unk30 |= arg1;
        for (k = 0; k < 4; k++) {
            Heroes[k].hero_status |= 0x100;
        }
        return;
    }
    g->unk30 |= 0x800;
    if (sb < sa) {
        g->winner[0] = ta->hero_name;
        g->winner[1] = am->hero_name;
        ta->hero_status |= 0x200;
        am->hero_status |= 0x200;
        b->hero_status |= 0x400;
        bm->hero_status |= 0x400;
    } else {
        RANK60V->winner[0] = b->hero_name;
        RANK60V->winner[1] = bm->hero_name;
        b->hero_status |= 0x200;
        bm->hero_status |= 0x200;
        ta->hero_status |= 0x400;
        am->hero_status |= 0x400;
    }
}

void PreMovePOGO(u32 arg0) {
    HeroInfo *hb;
    HeroInfo *h;
    EXTheromove *move;
    PadState *pb;
    PadState *pad;
    s32 x;
    s32 z;
    s32 bits;
    s32 speed;

    hb = Heroes;
    h = &hb[arg0];
    x = (h->hero_eng->unk10.x + 0x800) >> 9;
    z = (h->hero_eng->unk10.z + 0x800) >> 9;
    if (Pickup_IsClaimable(arg0, x, z) != 0) {
        func_800B935C(x, z, arg0);
    }
    move = h->hero_move;
    move->forward_speed = 0;
    if (h->hero_frozen == 0) {
    pb = D_80051338;
    pad = &pb[arg0];
    if ((pad->unk0 & 1) || D_8005A614[arg0 + 0x5C] != 0) {
        bits = ((Ovl60Pad *)pad)->unk50;
        if (D_800C535C[arg0].unk4 == 5 && !(bits & 0xF0)) {
            switch ((u16)move->forward_angle >> 10) {
            case 0:
                bits |= 0x40;
                break;
            case 1:
                bits |= 0x80;
                break;
            case 2:
                bits |= 0x10;
                break;
            case 3:
                bits |= 0x20;
                break;
            }
        }
        if ((bits & 0xF0) && move->velocity_enable != 0) {
            speed = Player_CalcForwardSpeed(arg0, D_800C535C[arg0].unk4 == 5);
            Player_RemoveReserve(arg0);
            if (((bits & 0x80) && x > 0) || ((bits & 0x20) && x < 7)
                || ((bits & 0x10) && z < 7) || ((bits & 0x40) && z > 0)) {
                if ((bits & 0x80) && PogoAI_IsSquareOKForPlayer(arg0, x, z, -1, 0, 0) != 0) {
                    func_800C2770(arg0, x - 1, z);
                    move->forward_angle = 0x400;
                    move->forward_speed = speed;
                    D_800C535C[arg0].unk8[1] = 0;
                }
                if ((bits & 0x20) && PogoAI_IsSquareOKForPlayer(arg0, x, z, 1, 0, 0) != 0) {
                    func_800C2770(arg0, x + 1, z);
                    move->forward_angle = 0xC00;
                    move->forward_speed = speed;
                    D_800C535C[arg0].unk8[1] = 0;
                }
                if ((bits & 0x10) && PogoAI_IsSquareOKForPlayer(arg0, x, z, 0, 1, 0) != 0) {
                    func_800C2770(arg0, x, z + 1);
                    move->forward_angle = 0x800;
                    move->forward_speed = speed;
                    D_800C535C[arg0].unk8[1] = 0;
                }
                if ((bits & 0x40) && PogoAI_IsSquareOKForPlayer(arg0, x, z, 0, -1, 0) != 0) {
                    func_800C2770(arg0, x, z - 1);
                    move->forward_angle = 0;
                    move->forward_speed = speed;
                    D_800C535C[arg0].unk8[1] = 0;
                }
            }
        }
    }
    move->facing_angle = move->forward_angle;
    } else {
        Player_RemoveReserve(arg0);
    }
}

void PostMovePOGO(u32 i) {
    VECTOR v;
    subengine *sub;
    s32 name;
    s32 n;

    name = Heroes[i].hero_name;
    if ((u32)(name - 3) < 2) {
        sub = Heroes[i].hero_fx->sub;
        if (sub != NULL && func_80015780(Heroes[i].hero_eng, Heroes[i].hero_sub, 0x200, &v) != NULL) {
            sub->unk6C = D_80053B9C;
            sub->unk50 = 1;
            sub->flags |= 0x8000;
            Heroes[i].hero_fx->unk5D = 1;
            Heroes[i].hero_fx->unk10 = *(SLONG_XYZ *)&v;
            Heroes[i].hero_fx->unk1E = Heroes[i].hero_eng->unk1E;
            Heroes[i].hero_fx->unk5E = 1;
            if (Heroes[i].hero_mode == 0x3A) {
                D_800C53CC[i] += 5;
                if (D_800C53CC[i] > 45) {
                    D_800C53CC[i] = 45;
                }
            } else {
                D_800C53CC[i] -= 2;
                if (D_800C53CC[i] < 5) {
                    D_800C53CC[i] = 5;
                }
            }
            Heroes[i].hero_fx->unk1C = (D_800C53CC[i] << 12) / 360;
            switch (name) {
            case 3:
                n = D_800C52E4;
                sub->unk74 = 0x2021;
                break;
            case 4:
                n = D_800C52E0;
                sub->unk74 = 0x2021;
                break;
            default:
                sub->unk74 = 0;
                n = 0;
                break;
            }
            if (n > 20) {
                n = 0;
            }
            if (n != 20 && n != 0) {
                n = Random(16);
            }
            sub->unk50 = 1;
            sub->unk20.x = 0x1000;
            sub->unk20.z = 0x1000;
            sub->unk51 = 0;
            sub->unk20.y = D_800C44D8[n];
            sub->unk52 = 1;
        }
    }
}

bool TestHeroPogo(u32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTheromove *m = h->hero_move;
    EXTengine *e;
    s32 angle;
    s32 t;
    s32 u;
    s16 sx;
    s16 sz;
    s32 x;
    s32 z;

    if (m->jump_ctrl & 2) {
        if (m->forward_speed != 0) {
            angle = m->forward_angle;
            e = h->hero_eng;
            sx = D_80068BD4[(angle - 0x400) & 0xFFF].unk2;
            sz = D_80068BD4[angle & 0xFFF].unk2;
            t = e->unk10.x;
            t -= sx * 512 >> 12;
            t += 0x800;
            x = t >> 9;
            u = e->unk10.z - (sz * 512 >> 12);
            u += 0x800;
            z = u >> 9;
            if ((u32)x >= 8 || z < 0 || z >= 8) {
                Heroes[arg0].hero_move->forward_speed = 0;
                return 0;
            }
            if (PogoAI_IsSquareOKForPlayer(arg0, x, z, 0, 0, 0) != 0) {
                SetHeroAction((HeroInfo *)arg0, 0x3A);
                h->hero_move->forward_speed = 0;
                return 1;
            }
            h->hero_move->forward_speed = 0;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl60/ovl60", D_800B32B8);

void HeroPogo(u32 arg0) {
    HeroInfo *h;
    HeroInfo *hero;
    HeroInfo *hb;
    HeroInfo *hb2;
    s32 *tbl;
    Ovl60Slot *slot;
    Ovl60Slot *sb;
    EXTengine *shadow;
    s32 n;
    s32 max;
    s32 name;

    hb = Heroes;
    h = &hb[arg0];
    if (h->hero_name == 4) {
        D_800C52E0 = 0;
    }
    if (h->hero_name == 3) {
        D_800C52E4 = 0;
    }
    if (TestHeroPogoDazed(arg0) != 0) {
        return;
    }
    if (TestHeroPogoShoot(arg0) != 0) {
        return;
    }
    if (h->hero_move->jump_ctrl & 0x40) {
        h->hero_move->forward_speed = Player_CalcForwardSpeed(arg0, D_800C535C[arg0].unk18);
    } else {
        h->hero_move->forward_speed = 0;
    }
    sb = D_800C535C;
    slot = &sb[arg0];
    if (slot->unk18 != 0) {
        slot->unk16++;
    }
    tbl = D_800C5074;
    slot->unk16++;
    n = (u16)slot->unk16;
    hb2 = Heroes;
    hero = &hb2[arg0];
    max = tbl[hero->hero_name];
    if (n < 0) {
        n = 0;
    }
    if (n >= max) {
        n = max;
    }
    shadow = hero->hero_shadow;
    if (shadow != NULL) {
        shadow->unk2C.x = shadow->unk2C.y = shadow->unk2C.z = (abs(max / 2 - n) << 8) + 0xC00;
        shadow->unk5E = 1;
    }
    if (n <= D_800C502C[hero->hero_name] || max - D_800C5050[hero->hero_name] < n) {
        hero->hero_move->jump_ctrl &= 0xBF;
    } else {
        hero->hero_move->jump_ctrl |= 0x40;
    }
    if ((u16)D_800C535C[arg0].unk16 >= max) {
        Player_SnapToTile(arg0);
        PlayBoingNoise(arg0);
        Set_Tile_Under_Player(arg0, 1);
        Heroes[arg0].hero_move->velocity_enable = 0;
        Heroes[arg0].hero_move->forward_speed = 0;
        Heroes[arg0].hero_move->jump_max = 0;
        if (TestHeroPogoWinner(arg0) != 0) {
            return;
        }
        if (TestHeroPogoLoser(arg0) != 0) {
            return;
        }
        if (TestHeroPogoTied(arg0) != 0) {
            return;
        }
        SetHeroAction((HeroInfo *)arg0, 0x39);
    }
    switch (Heroes[arg0].hero_name) {
    case 1:
        n = D_800C4758[n];
        if (n == 0x4111) {
            AddSomeDust(arg0);
        }
        break;
    case 2:
        n = D_800C4878[n];
        if (n == 0x408E) {
            AddSomeDust(arg0);
        }
        break;
    case 5:
        n = D_800C4E40[n];
        if (n == 0x418E) {
            AddSomeDust(arg0);
        }
        break;
    case 6:
        n = D_800C4F3C[n];
        if (n == 0x410E) {
            AddSomeDust(arg0);
        }
        break;
    case 8:
        n = D_800C4CF4[n];
        if (n == 0x4102) {
            AddDingoSmoke(arg0);
        }
        break;
    case 7:
        n = D_800C4BA0[n];
        if (n == 0x4082) {
            AddDingoSmoke(arg0);
        }
        break;
    case 4:
        D_800C52E0 = n;
        n = D_800C49DC[n];
        if (n == 0x4105) {
            AddBrioSmoke(arg0);
        }
        break;
    case 3:
        D_800C52E4 = n;
        n = D_800C4AB0[n];
        if (n == 0x4085) {
            AddBrioSmoke(arg0);
        }
        break;
    }
    Heroes[arg0].hero_eng->sub->unk74 = n;
}

void SetHeroPogoIdle(u32 arg0) {
    s32 seq;

    Player_RemoveReserve(arg0);
    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    Heroes[arg0].hero_move->jump_ctrl |= 0x22;
    Heroes[arg0].hero_move->jump_ctrl &= 0xEF;
    Player_SnapToTile(arg0);
    if (D_800C535C[arg0].unk4 == 5) {
        SetHeroSeq((HeroInfo *)arg0, 0x2F);
    } else {
        SetHeroSeq((HeroInfo *)arg0, 0x2E);
    }
}

void HeroPogoIdle(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    s32 v;

    if (TestHeroPogoWinner(arg0) != 0) {
        return;
    }
    if (TestHeroPogoLoser(arg0) != 0) {
        return;
    }
    if (TestHeroPogoTied(arg0) != 0) {
        return;
    }
    if (TestHeroPogoDazed(arg0) != 0) {
        return;
    }
    if (TestHeroPogoShoot(arg0) != 0) {
        return;
    }
    if (TestHeroPogo(arg0) != 0) {
        return;
    }
    switch (func_8008502C(arg0)) {
    case 0x12:
        PlayBoingNoise(arg0);
        Set_Tile_Under_Player(arg0, 1);
        if (D_800C535C[arg0].unk4 != 5 && Heroes[arg0].hero_aseq == 0x2F) {
            SetHeroSeq((HeroInfo *)arg0, 0x2E);
        }
        break;
    case 0x11:
        AddDingoSmoke(arg0);
        break;
    case 0x14:
        AddSomeDust(arg0);
        break;
    case 0x13:
        AddBrioSmoke(arg0);
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

void HeroPogoTazing(u32 arg0) {
    HeroInfo *h;
    EXTengine *eng;
    EXTheromove *mv;
    HeroInfo *base;
    s32 d;

    if (TestHeroPogoWinner(arg0) != 0) {
        return;
    }
    if (TestHeroPogoLoser(arg0) != 0) {
        return;
    }
    if (TestHeroPogoTied(arg0) != 0) {
        return;
    }
    if (TestHeroPogoDazed(arg0) != 0) {
        return;
    }
    func_800BC0EC(arg0);
    base = Heroes;
    h = &base[arg0];
    h->hero_frozen = 10;
    if (func_8008502C(arg0) == 0x14) {
        AddTazerRing(arg0);
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
        HeroPogoTazingCancel(arg0);
        SetHeroAction((HeroInfo *)arg0, 0x39);
    }
    func_800871E4(arg0);
}

void HeroPogoWinner(u32 arg0) {
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
        PlayBoingNoise(arg0);
        Set_Tile_Under_Player(arg0, 1);
        break;
    case 0x11:
        AddDingoSmoke(arg0);
        break;
    case 0x14:
        AddSomeDust(arg0);
        break;
    case 0x13:
        AddBrioSmoke(arg0);
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

void SetHeroPogoLoser(u32 arg0) {
    s32 *pos;
    NAS *nas;
    s32 *ext;
    u8 *p;

    Player_RemoveReserve(arg0);
    Heroes[arg0].hero_move->velocity_enable = 0;
    Heroes[arg0].hero_frozen = 2;
    func_800C1CF0(arg0);
    Player_SnapToTile(arg0);
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

void HeroPogoLoser(u32 arg0) {
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

bool TestHeroPogoDazed(u32 arg0) {
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

void SetHeroPogoDazed(u32 arg0) {
    s32 x;
    s32 z;
    Ovl60Cell *c;
    HeroInfo *h;
    HeroInfo *base;

    Player_RemoveReserve(arg0);
    if (D_800C535C[arg0].nas != NULL) {
        HeroPogoTazingCancel(arg0);
    }
    func_800C1CF0(arg0);
    func_80028018(arg0, 0x1E);
    Player_SnapToTile(arg0);
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

void HeroPogoDazed(u32 arg0) {
    s32 x;
    s32 z;
    Ovl60Cell *c;
    HeroInfo *h;
    HeroInfo *base;
    Ovl60Slot *s;
    EXTengine *eng;

    x = (Heroes[arg0].hero_eng->unk10.x + 0x800) >> 9;
    z = (Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9;
    if ((u32)x < 8 && z >= 0 && z < 8) {
        Ovl60Cell *cb = D_800C53DC[0];
        c = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)cb);
        c->unkE = arg0 + 1;
        c->unk10 = arg0 + 1;
    }
    if (Ovl60_StatusAction(arg0, 0x200, 0x3C, 0x32)) {
        return;
    }
    if (Ovl60_StatusAction(arg0, 0x400, 0x3D, 0x2E)) {
        return;
    }
    if (Ovl60_StatusAction(arg0, 0x100, 0x3E, 0x2E)) {
        return;
    }
    base = Heroes;
    h = &base[arg0];
    h->hero_frozen = 10;
    s = &D_800C535C[arg0];
    if (!(s->flags & 1)) {
        s->unk8[1]++;
    }
    if ((u32)s->unk8[1] > 0x32) {
        h->hero_status &= ~0x800;
        SetHeroAction((HeroInfo *)arg0, 0x39);
    }
    if (s->flags & 1) {
        eng = h->hero_eng;
        if (eng != NULL) {
            s32 r = func_80015480(0, eng->unk1E);
            eng->unk5E = 1;
            eng->unk1E += r >> 3;
        }
    }
    func_800871E4(arg0);
    func_8008502C(arg0);
    switch (Heroes[arg0].hero_name) {
    case 4:
        if (D_800C52E0 == 0) {
            D_800C52E0 = 0x14;
        } else {
            D_800C52E0 = 0;
        }
        break;
    case 3:
        if (D_800C52E4 == 0) {
            D_800C52E4 = 0x14;
        } else {
            D_800C52E4 = 0;
        }
        break;
    }
}

void AddDingoSmoke(u32 arg0) {
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

void AddSomeDust(u32 arg0) {
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

void AddBrioSmoke(u32 arg0) {
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

s32 Bank_Tiles(s32 arg0) {
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
        Handle_Score(arg0, (eng->unk10.x + 0x800) >> 9, (eng->unk10.z + 0x800) >> 9, count);
    }
    return count;
}

void Handle_Score(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Ovl60ScoreView *g = SCORE60V;
    s32 score;

    if (g->unk30 & 0x801) {
        return;
    }
    if (arg3 != 0) {
        CreateFloatyPogoScore(arg0, arg1, arg2, arg3);
    }
    if (g->flags & 0x40) {
        if (g->teams != 0) {
            score = Heroes[arg0].hero_score;
            if (score < arg3) {
                Heroes[arg0].hero_score = 0;
                Heroes[Heroes[arg0].hero_teammate].hero_score -= arg3 - score;
                return;
            }
        }
        Heroes[arg0].hero_score -= arg3;
    } else if ((g->flags & 0x80) && g->mode == 0xB) {
        if (Heroes[arg0].hero_score < arg3) {
            Heroes[arg0].hero_score = arg3;
        }
    } else {
        Heroes[arg0].hero_score += arg3;
    }
}

void Tile_Claim(s32 arg0, s32 arg1, s32 arg2) {
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
            Tile_Update(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            Tile_Update(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            Tile_Update(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            Tile_Update(arg0, arg1 + 1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Tile_Update);

void Tile_FireTnt(s32 arg0, s32 arg1) {
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
            Player_SetDazed(i);
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

void Tile_FireNitro(s32 arg0, s32 arg1) {
    HeroInfo *h;
    Ovl60Cell *c;
    Ovl60Cell *base;
    s32 *pos;
    NAS *nas;
    s32 *ext;
    s32 i;
    s32 near;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        near = 0;
        if (abs((arg0 << 9) - (((h->hero_eng->unk10.x + 0x800) >> 9) << 9)) < 0x300) {
            near = abs((arg1 << 9) - (((h->hero_eng->unk10.z + 0x800) >> 9) << 9)) < 0x300;
        }
        if (near && !(h->hero_status & 0x800)) {
            if (((Ovl60ModeView *)D_8005A614)->mode == 0xC
                && (((Ovl60ModeView *)D_8005A614)->flags & 0x80)
                && ((Ovl60ModeView *)D_8005A614)->out[i] == 0) {
                func_800C1400(i);
                PlaySfx(0x529, NULL, (SLONG_XYZ *)&h->hero_eng->unk10, 0x1000, 0x1E00, 0);
            } else {
                Player_SetDazed(i);
            }
        }
    }
    base = D_800C53DC[0];
    c = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
    c->unk12 = 1;
    c->unkA = -1;
    c->unk14 = 0;
    Tile_Update(arg0, arg1);
    pos = &c->unk4->x;
    nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 2;
            ext[6] = 0x100;
            ext[8] = 1;
            ext[10] = 0;
            nas->InitDelete(nas);
        }
    }
    switch ((u32)Random(4)) {
    case 0:
        PlaySfx(0x3F1, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x3F2, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x3F3, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x3F4, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    }
}

void ZapInDir(s32 arg0, s32 arg1, s32 dx, s32 dz, const s32 arg4) {
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
                    Tile_Update(x - 1, z);
                }
                if (x < 7) {
                    Tile_Update(x + 1, z);
                }
                if (z > 0) {
                    Tile_Update(x, z - 1);
                }
                if (z < 7) {
                    Tile_Update(x, z + 1);
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

void func_800B87DC(void) {
    s32 off;
    Ovl60Cell *base;
    s32 j;
    s32 i;
    s32 vol;
    s32 rowOff;
    s32 sfx;
    s16 t;

    i = 0;
    base = D_800C53DC[0];
    vol = 0x1E00;
    rowOff = 0;
loop_i:
    j = 0;
    if (i < 8) {
        off = rowOff;
loop_j:
        if (j < 8) {
            if (D_8005A648 == 13) {
                if ((*(u16 *)CELL_AT(off)->unkC & 0x10) && CELL_AT(off)->unkE == 0) {
                    *(u16 *)CELL_AT(off)->unkC &= ~0x10;
                }
            }
            switch ((u16)CELL_AT(off)->unk12) {
            case 5:
                t = --CELL_AT(off)->unk14;
                if (CELL_AT(off)->unk18[0] != 0) {
                    switch (t) {
                    case 10:
                    case 20:
                    case 30:
                    case 60:
                    case 90:
                        do {
                            do {
                                PlaySfx(0x3F0, NULL, (SLONG_XYZ *)&CELL_AT(off)->unk4->x, 0x1000, vol, 0);
                            } while (0);
                        } while (0);
                        break;
                    }
                }
                if (CELL_AT(off)->unk14 < 2) {
                    Tile_FireTnt(i, j);
                }
                Tile_Update(i, j);
                break;
            case 6:
                if (--CELL_AT(off)->unk14 < 2) {
                    Tile_FireNitro(i, j);
                }
                break;
            case 0:
                do {
                if (--CELL_AT(off)->unk14 < 2) {
                    CELL_AT(off)->unk12 = 1;
                    switch (CELL_AT(off)->unk18[0]) {
                    case 2:
                        sfx = 0x516;
                        goto play;
                    case 3:
                        sfx = 0x517;
                        goto play;
                    case 4:
                        sfx = 0x518;
                        goto play;
                    case 5:
                        sfx = 0x519;
                        goto play;
                    case 6:
                        sfx = 0x51A;
                        goto play;
                    case 7:
                        sfx = 0x51B;
                        goto play;
                    case 8:
                        sfx = 0x51C;
                    play:
                        do {
                            do {
                                PlaySfx(sfx, NULL, (SLONG_XYZ *)&CELL_AT(off)->unk4->x, 0x1000, vol, 0);
                            } while (0);
                        } while (0);
                        break;
                    }
                    CELL_AT(off)->unk18[0] = 0;
                }
                } while (0);
                Tile_Update(i, j);
                break;
            case 2:
                do {
                    if (--CELL_AT(off)->unk14 < 2) {
                        CELL_AT(off)->unk12 = 3;
                        CELL_AT(off)->unk14 = 10;
                    }
                } while (0);
                Tile_Update(i, j);
                break;
            case 3:
                do {
                    if (--CELL_AT(off)->unk14 < 2) {
                        CELL_AT(off)->unk12 = 1;
                        CELL_AT(off)->unkA = -1;
                        CELL_AT(off)->unk14 = 0;
                    }
                } while (0);
                Tile_Update(i, j);
                break;
            case 1:
            case 4:
            case 7:
            case 8:
            case 9:
            case 10:
                break;
            }
            off += sizeof(Ovl60Cell);
            j++;
            goto loop_j;
        }
        rowOff += sizeof(D_800C53DC[0]);
        i++;
        goto loop_i;
    }
}

void PlayBoingNoise(u32 arg0) {
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

void Set_Tile_Under_Player(u32 arg0, bool arg1) {
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
            PlaySquareChangeNoise(arg0);
        }
        *(u16 *)c->unkC |= 0x10;
    }
    if (SetTile_PogoThree(arg0, x, z, arg1) != 0) {
        return;
    }
    if (SetTile_PogoFour(arg0, x, z, arg1) != 0) {
        return;
    }
    {
        Ovl60Cell *cb = D_800C53DC[0];
        p = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)cb);
    }
    owner = p->unkA;
    if (owner == -1) {
        Tile_Claim(x, z, arg0);
        Tile_Update(x, z);
        Tiles_CheckForIslands(arg0);
        return;
    }
    if (owner == arg0) {
        return;
    }
    switch ((u16)p->unk12) {
    case 1:
    case 3:
        Tile_Claim(x, z, arg0);
        Tile_Update(x, z);
        Tiles_CheckForIslands(arg0);
        break;
    case 5:
        Tile_FireTnt(x, z);
        Tile_Update(x, z);
        break;
    case 6:
        Tile_FireNitro(x, z);
        Tile_Update(x, z);
        break;
    }
}

void PlaySquareChangeNoise(u32 arg0) {
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

void PlaySquareLockedNoise(u32 arg0) {
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

void PogoFour_PlayerHasStoodOnOwnTile(s32 arg0) {
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
                Tile_Update(i, j);
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

void PogoFour_FreezeMyTiles(s32 arg0) {
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
                Tile_Update(i, j);
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

void PogoFour_UnfreezeMyTiles(s32 arg0) {
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
                        Tile_Update(i - 1, j);
                    }
                    if (i < 7) {
                        Tile_Update(i + 1, j);
                    }
                    if (j > 0) {
                        Tile_Update(i, j - 1);
                    }
                    if (j < 7) {
                        Tile_Update(i, j + 1);
                    }
                }
                Tile_Update(i, j);
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

void func_800B9D64(void) {
    D5A614View *g = (D5A614View *)D_8005A614;
    s32 idx = g->mode - 10;
    s32 x, z;
    u32 r;
    D5A614View *g2;

    if (D_800C52EA == -1) {
        if (g->mode == 13 && (g->flags & 0x80)) {
            if (D_800C52F0 < 4) {
                D_800C52EA = Random(60) + 60;
            }
        } else if (D_800C52F0 < D_800C38B8[idx]) {
            goto rearm;
        }
    } else {
        D_800C52EA--;
        if (D_800C52EA < 2) {
            if (func_800C165C(&x, &z) != 0) {
                switch (g->mode) {
                case 10:
                    if (Random(3) != 0) {
                        func_800BA25C(x, z, 1);
                    } else {
                        goto five;
                    }
                    break;
                case 11:
                    r = Random(5);
                    if (r == 0) {
                        func_800BA25C(x, z, 3);
                    } else if (r < 3) {
                        goto five;
                    } else {
                        func_800BA25C(x, z, 2);
                    }
                    break;
                case 12:
                    switch (Random(2)) {
                    case 0:
                        func_800BA25C(x, z, 1);
                        break;
                    case 1:
                        goto five;
                    }
                    break;
                case 13:
                    if (g->flags & 0x80) {
                        func_800BA25C(x, z, 4);
                    } else if (Random(2) == 0) {
                        func_800BA25C(x, z, 4);
                    } else {
                    five:
                        func_800BA25C(x, z, 5);
                    }
                    break;
                }
            }
            g2 = (D5A614View *)D_8005A614;
            if (g2->mode == 13 && (g2->flags & 0x80)) {
                if (D_800C52F0 >= 7) {
                    D_800C52EA = -1;
                } else {
                    goto rearm;
                }
            } else if (D_800C52F0 > D_800C38C8[idx]) {
                D_800C52EA = -1;
            } else {
            rearm:
                D_800C52EA = Random(60) + 60;
            }
        }
    }
}

bool Pickup_IsClaimable(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *c;
    Ovl60Cell *base;
    HeroInfo *h;
    s32 lock;

    if ((u32)arg1 < 8 && (u32)arg2 < 8) {
        if (func_800C0AFC(arg1, arg2) != 0) {
            return 1;
        }
        base = D_800C53DC[0];
        c = (Ovl60Cell *)(arg2 * sizeof(Ovl60Cell) + arg1 * sizeof(D_800C53DC[0]) + (u32)base);
        h = c->hero;
        if (h == NULL) {
            return 0;
        }
        lock = h->hero_pu_lock;
        if (h->hero_pu_action == 10) {
            if (lock < 3) {
                return 0;
            }
        } else if (lock < 3) {
            return 0;
        }
        if (D_800C53DC[arg1][arg2].unk10 == 0 || D_800C53DC[arg1][arg2].unk10 == arg0 + 1) {
            goto ok;
        }
    }
    return 0;
ok:
    return 1;
}

bool Pickup_IsClaimableByAI(s32 arg0, s32 arg1, s32 arg2) {
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

s32 Fire_Tazer(s32 arg0, s32 arg1) {
    EXTheromove *mv;
    EXTengine *eng;
    EXTengine *e;
    NAS *nas;
    subengine *sub;
    s32 pos[3];
    s32 x;
    s32 z;
    s32 n;
    s32 dx;
    s32 dz;
    s32 t;

    mv = Heroes[arg0].hero_move;
    n = 0;
    if (mv != NULL) {
        dz = 0;
        eng = Heroes[arg0].hero_eng;
        x = (eng->unk10.x + 0x800) >> 9;
        z = (eng->unk10.z + 0x800) >> 9;
        dx = 0;
        switch (mv->facing_angle >> 10) {
        case 0:
            n = z;
            break;
        case 1:
            n = x;
            break;
        case 2:
            n = 7 - z;
            break;
        case 3:
            n = 7 - x;
            break;
        }
        if (n > 0) {
            goto spawn;
        }
    }
    return n;
spawn:
    x = Heroes[arg0].hero_eng->unk10.x;
    z = Heroes[arg0].hero_eng->unk10.z;
    *(SLONG_XYZ *)pos = Heroes[arg0].hero_eng->unk10;
    pos[1] -= 0x80;
    switch (mv->facing_angle >> 10) {
    case 0:
        z -= 0x100;
        dz = -0x200;
        pos[2] -= 0x100;
        break;
    case 1:
        x -= 0x100;
        dx = -0x200;
        pos[0] -= 0x100;
        break;
    case 2:
        z += 0x100;
        dz = 0x200;
        pos[2] += 0x100;
        break;
    case 3:
        x += 0x100;
        dx = 0x200;
        pos[0] += 0x100;
        break;
    }
    if (D_800C535C[arg0].nas != NULL) {
        return n;
    }
    nas = SpawnNASById(x, D_8005A5D8 - 0x40, z, 0x1405, 0x11);
    if (nas == NULL) {
        return n;
    }
    e = nas->Engine;
    if (e == NULL) {
        return n;
    }
    e->unk1E = (mv->facing_angle + 0x400) % 0x1000;
    if ((mv->facing_angle >> 10) & 1) {
        e->unk1C = 0x400;
    }
    e->unk5E = 1;
    if (n >= 2) {
        nas->UserInfo.flags = func_80018F3C(e, n - 1);
    }
    sub = e->sub;
    t = 0;
    while (sub != NULL) {
        pos[0] += dx;
        pos[2] += dz;
        sub->unk6C = D_80053B9C;
        sub->unk74 = 0x2005;
        sub->unk04 = t;
        sub->flags |= 0x21;
        sub = sub->next;
        t += 0x200;
    }
    Ovl60_SpawnPuff(pos, 8, D_80053B9C);
    do {
        do {
            D_800C535C[arg0].nas = nas;
            PlaySfx(0x520, NULL, (SLONG_XYZ *)&Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        } while (0);
    } while (0);
    if (D_800C535C[arg0].unk4 == 5 && Heroes[arg0].hero_aseq == 0x2F) {
        SetHeroAction((HeroInfo *)arg0, 0x39);
    }
    if (D_800C535C[arg0].unk4 == 6) {
        PogoFour_UnfreezeMyTiles(arg0);
    }
    D_800C535C[arg0].unk4 = 0;
    return n;
}

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
                Player_SetDazed(i);
                if (slot->unk4 == 5 && h->hero_aseq == 0x2F) {
                    SetHeroAction((HeroInfo *)i, 0x39);
                }
                if (slot->unk4 == 6) {
                    PogoFour_UnfreezeMyTiles(i);
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

void Plot_PogoMushroom(NAS *nas) {
    s32 x;
    s32 z;
    EXTengine *eng;
    Ovl60CellPos *pos;
    Ovl60Cell *c;
    s32 cx;
    s32 cz;
    s32 scale;
    s32 i;
    s32 j;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        if (--nas->UserInfo.user1 > 0) {
            break;
        }
        if (D_8005A644 & 0x801) {
            break;
        }
        if (func_800BBF00(&x, &z) == 0) {
            break;
        }
        pos = D_800C53DC[x][z].unk4;
        if (pos == NULL) {
            break;
        }
        eng->unk10.x = pos->x;
        eng->unk10.y = D_8005A5D8;
        eng->unk10.z = pos->z;
        Ovl60_SpawnPuff((s32 *)&eng->unk10, 0, D_80053ABC);
        nas->UserInfo.status = 6;
        nas->UserInfo.user1 = 0x12;
        break;
    case 6:
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 1;
        }
        break;
    case 1:
        cx = (eng->unk10.x + 0x800) >> 9;
        cz = (eng->unk10.z + 0x800) >> 9;
        nas->UserInfo.status = 0;
        if (func_800BC04C(cx, cz) != 0) {
            Ovl60Cell *cb = D_800C53DC[0];
            c = (Ovl60Cell *)(cz * sizeof(Ovl60Cell) + cx * sizeof(D_800C53DC[0]) + (u32)cb);
            c->hero = (HeroInfo *)nas;
            if (c->unk4 != NULL) {
                eng->unk2C.x = 0x1000;
                eng->unk2C.z = 0x1000;
                eng->unk2C.y = 0;
                eng->unk5E = 1;
                eng->flags |= 0x8000;
                nas->UserInfo.status = 2;
                nas->UserInfo.user1 = 0;
                PlaySfx(0x528, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 2:
        scale = D_800C45FC[nas->UserInfo.user1];
        if (++nas->UserInfo.user1 >= 0x10) {
            scale = 0x1000;
            nas->UserInfo.user1 = Random(0x28) + 0x28;
            nas->UserInfo.status = 3;
        }
        eng->unk2C.y = scale;
        eng->unk5E = 1;
        break;
    case 3:
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 5;
        }
        break;
    case 5:
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (D_800C53DC[i][j].hero == (HeroInfo *)nas) {
                    D_800C53DC[i][j].hero = NULL;
                }
            }
        }
        Ovl60_SpawnPuff((s32 *)&eng->unk10, 1, D_80053B9C);
        eng->flags &= ~0x8000;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = Random(0x14) + 0x1E;
        break;
    }
}

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
                        Tile_Update(i - 1, j);
                    }
                    if (i < 7) {
                        Tile_Update(i + 1, j);
                    }
                    if (j > 0) {
                        Tile_Update(i, j - 1);
                    }
                    if (j < 7) {
                        Tile_Update(i, j + 1);
                    }
                }
            }
        }
    next:;
    }
done:;
}

void Tiles_FloodFillIslandMap(s32 *seen, s32 id) {
    s32 i;

    if (D_8005A648 == 0xB) {
        for (i = 0; i < 8; i++) {
            if (D_800C5B2C[i] != id) {
                func_800C1840(0, i, id, seen);
            } else {
                if (i > 0 && D_800C5B2C[i - 1] != id) {
                    func_800C1840(0, i, id, seen);
                }
                if (i < 7 && D_800C5B2C[i + 1] != id) {
                    func_800C1840(0, i, id, seen);
                }
            }
            if (D_800C5B4C[i] != id) {
                func_800C1840(7, i, id, seen);
            } else {
                if (i > 0 && D_800C5B4C[i - 1] != id) {
                    func_800C1840(7, i, id, seen);
                }
                if (i < 7 && D_800C5B4C[i + 1] != id) {
                    func_800C1840(7, i, id, seen);
                }
            }
            if (D_800C5B6C[i] != id) {
                func_800C1840(i, 7, id, seen);
            } else {
                if (i > 0 && D_800C5B6C[i - 1] != id) {
                    func_800C1840(i, 7, id, seen);
                }
                if (i < 7 && D_800C5B6C[i + 1] != id) {
                    func_800C1840(i, 7, id, seen);
                }
            }
            if (D_800C5B8C[i] != id) {
                func_800C1840(i, 0, id, seen);
            } else {
                if (i > 0 && D_800C5B8C[i - 1] != id) {
                    func_800C1840(i, 0, id, seen);
                }
                if (i < 7 && D_800C5B8C[i + 1] != id) {
                    func_800C1840(i, 0, id, seen);
                }
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            func_800C1840(0, i, id, seen);
            func_800C1840(7, i, id, seen);
            func_800C1840(i, 0, id, seen);
            func_800C1840(i, 7, id, seen);
        }
    }
}

s32 Tiles_CountUnflagged(s32 *arg0, const s32 arg1, s32 arg2) {
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
                        Tile_Update(i - 1, j);
                    }
                    if (i < 7) {
                        Tile_Update(i + 1, j);
                    }
                    if (j > 0) {
                        Tile_Update(i, j - 1);
                    }
                    if (j < 7) {
                        Tile_Update(i, j + 1);
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

s32 func_800BBC94(s32 x, s32 z) {
    D5A614View *g = (D5A614View *)D_8005A614;
    s32 id;
    s32 mask;
    s32 n;
    s32 r;

    if (g->mode == 11) {
        do {
            id = D_800C53DC[x][z].unkA;
        } while (0);
        mask = 0;
        if (id != -1) {
            if (x > 0) {
                n = D_800C53DC[x - 1][z].unkA;
                mask = n == id;
            } else if (D_800C5B2C[z] == id) {
                mask = 1;
            }
            if (x < 7) {
                if (D_800C53DC[x + 1][z].unkA == id) {
                    mask |= 4;
                }
            } else if (D_800C5B4C[z] == id) {
                mask |= 4;
            }
            if (z > 0) {
                if (D_800C53DC[x][z - 1].unkA == id) {
                    mask |= 2;
                }
            } else if (D_800C5B8C[x] == id) {
                mask |= 2;
            }
            if (z < 7) {
                if (D_800C53DC[x][z + 1].unkA == id) {
                    mask |= 8;
                }
            } else if (D_800C5B6C[x] == id) {
                mask |= 8;
            }
        }
        if (D_8005A646 & 0x80) {
            return mask + 0x301B;
        }
        return mask + 0x3052;
    }
    switch (g->mode) {
    case 12:
        r = 0x304E;
        if (g->flags & 0x80) {
            r = 0x3058;
        }
        return r;
    case 13:
        r = 0x3026;
        if (g->flags & 0x80) {
            r = 0x3044;
        }
        return r;
    default:
        r = 0x303C;
        if (D_8005A646 & 0x80) {
            r = 0x3033;
        }
        return r;
    }
}

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

void Fire_Rocket(s32 arg0, s32 arg1) {
    EXTheromove *move;
    EXTengine *eng;
    s32 buttons;
    s32 angle;
    s32 mode;

    PlayThrowNoise(arg0);
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
        Fire_RocketInDir(arg0, mode, angle, 0x180);
        eng = Heroes[arg0].hero_eng;
        PlaySfx(0x522, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        if (D_800C535C[arg0].unk4 == 5 && Heroes[arg0].hero_aseq == 0x2F) {
            SetHeroAction((HeroInfo *)arg0, 0x39);
        }
        if (D_800C535C[arg0].unk4 == 6) {
            PogoFour_UnfreezeMyTiles(arg0);
        }
        D_800C535C[arg0].unk4 = 0;
    }
}

void Fire_RocketFourWays(s32 arg0, s32 arg1) {
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
    PlayThrowNoise(arg0);
    if (x > 0) {
        Fire_RocketInDir(arg0, 2, 0x400, 0x180);
    }
    if (x < 7) {
        Fire_RocketInDir(arg0, 2, 0xC00, 0x180);
    }
    if (z > 0) {
        Fire_RocketInDir(arg0, 2, 0, 0x180);
    }
    if (z < 7) {
        Fire_RocketInDir(arg0, 2, 0x800, 0x180);
    }
    PlaySfx(0x43C, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
    if (D_800C535C[arg0].unk4 == 5 && h->hero_aseq == 0x2F) {
        SetHeroAction((HeroInfo *)arg0, 0x39);
    }
    base = D_800C535C;
    slot = &base[arg0];
    if (slot->unk4 == 6) {
        PogoFour_UnfreezeMyTiles(arg0);
    }
    slot->unk4 = 0;
}

void PlayThrowNoise(u32 arg0) {
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

void Plot_PogoProjectile(NAS *arg0) {
    EXTengine *e;
    subengine *first;
    subengine *sub;
    HeroInfo *h;
    Ovl60Slot *base;
    Ovl60Slot *slot;
    EXTengine *he;
    NAS *cell;
    NAS *nas;
    s32 *ext;
    u8 *p;
    s32 *pos;
    s32 i;
    s32 j;
    s32 n;
    s32 done;
    s32 k;
    s32 off;
    s32 hit;
    s32 lim;
    s32 px;
    s32 pz;
    s32 me;
    s32 dx;
    s32 dz;
    s32 x;
    s32 z;
    u16 f;

    e = arg0->Engine;
    if (e == NULL || (first = e->sub) == NULL) {
        return;
    }
    if (arg0->UserInfo.flags & 2) {
        e->unk10.x -= (D_80068BD4[(arg0->UserInfo.status - 0x400) & 0xFFF].unk2 * 9) >> 8;
        e->unk10.z -= (D_80068BD4[arg0->UserInfo.status & 0xFFF].unk2 * 9) >> 8;
    }
    j = 0;
    n = 0;
    done = 0;
    sub = first->next;
    for (;;) {
        if (sub == NULL || done) {
            break;
        }
        if (sub->flags & 0x8000) {
            if (((s32)arg0->UserInfo.user1 >> j) & 1) {
                k = sub->unk20.x;
                if (arg0->UserInfo.flags & 2) {
                    sub->unk04 += 0x90;
                }
                k -= 0x100;
                if (k <= 0) {
                    k = 0;
                    sub->flags &= ~0x8000;
                }
                n++;
                sub->unk20.z = k;
                sub->unk20.y = k;
                sub->unk20.x = k;
                sub->unk52 = 1;
            }
            if (sub->flags & 0x8000) {
                goto next;
            }
        }
        if (!(((s32)arg0->UserInfo.user1 >> j) & 1)) {
            done = 1;
        } else {
        next:
            sub = sub->next;
            j++;
        }
    }
    if (arg0->UserInfo.flags & 2) {
        k = (arg0->UserInfo.flags >> 4) & 3;
        hit = 0;
        if (D_8004E0D8 & 1) {
            k++;
        }
        if (k & 2) {
            if (sub != NULL) {
                sub->unk74 = 0x2018;
                sub->flags |= 0x8000;
                arg0->UserInfo.user1 |= 1 << j;
            }
            k = 0;
        }
        i = 0;
        base = D_800C535C;
        h = Heroes;
        slot = base;
        off = i;
        f = (arg0->UserInfo.flags & 0xFFCF) | (k << 4);
        me = (f >> 8) & 7;
        arg0->UserInfo.flags = f;
    heroes:
        if (i < 4) {
            if (i != me) {
                he = h->hero_eng;
                if (he != NULL) {
                    dx = he->unk10.x - e->unk10.x;
                    dz = he->unk10.z - e->unk10.z;
                    if ((csqrt(dx * dx + dz * dz) >> 6) < 0xA0) {
                        hit = 1;
                        if (!(h->hero_status & 0x800)) {
                            if (arg0->UserInfo.user2 == 3) {
                                do {
                                    func_800BB724(me, i);
                                    PlaySfx(0x552, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
                                } while (0);
                            }
                            do {
                                do {
                                    if (((Ovl60Slot *)(off + (u32)base))->unk4 == 5 && h->hero_aseq == 0x2F) {
                                        SetHeroAction((HeroInfo *)i, 0x39);
                                    }
                                } while (0);
                            } while (0);
                            do {
                                if (slot->unk4 == 6) {
                                    PogoFour_UnfreezeMyTiles(i);
                                }
                            } while (0);
                            slot->unk4 = 0;
                            h->hero_status |= 0x800;
                        }
                    }
                }
            }
            h++;
            do {
                slot++;
            } while (0);
            off += sizeof(Ovl60Slot);
            i++;
            goto heroes;
        }
        if (D_8005A648 == 0xA) {
            x = (e->unk10.x + 0x800) >> 9;
            z = (e->unk10.z + 0x800) >> 9;
            if ((u32)x < 8 && z >= 0 && z < 8) {
                cell = (NAS *)D_800C53DC[x][z].hero;
                if (cell != NULL && cell->UserInfo.flags == 0xB) {
                    cell->UserInfo.status = 5;
                    hit = 1;
                }
            }
        }
        if (!hit) {
            px = e->unk10.x;
            pz = e->unk10.z;
            lim = 0x800;
            if (px < -0xA00 || px > lim || pz < -0xA00 || pz > lim) {
                hit = 1;
            }
            if (!hit) {
                return;
            }
        }
        pos = (s32 *)&e->unk10;
        p = D_80053B9C;
        nas = SpawnNASById(e->unk10.x, pos[1], pos[2], 0x1605, 0x11);
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
        PlaySfx(0x521, NULL, &e->unk10, 0x1000, 0x1E00, 0);
        arg0->UserInfo.flags &= 0xFFFC;
        first->flags &= ~0x8000;
    } else if (arg0->UserInfo.user1 != 0 && n == 0) {
        KillNAS(arg0);
    }
}

void func_800BCB4C(void) {
    s32 buf[0x40];
    u8 *row;
    s32 *pos;
    s32 i;
    s32 j;
    HeroInfo *h;
    Ovl60ModeView *g;

    if (D_800C5BB4 == NULL) {
        return;
    }
    if (D_800C5BC4 != 0) {
        D_800C5BC4--;
    }
    if (D_800C5BD0 == 0) {
        switch ((u32)D_80099178 >> 8) {
        case 0x16:
        case 0x17:
        case 0x31:
        case 0x32:
        case 0x46:
        case 0x47:
        case 0x101:
        case 0x102:
        case 0x116:
        case 0x117:
            if (((u32)D_80099178 >> 8) != D_800C5BD4) {
                PlaySfx(0x532, NULL, NULL, 0x1000, 0x1E00, 0);
                D_800C5BD4 = (u32)D_80099178 >> 8;
            }
            break;
        case 0x15:
        case 0x30:
        case 0x45:
        case 0x100:
        case 0x115:
            D_800C5BD0 = Random(4) + 1;
            PlaySfx(0x534, NULL, NULL, 0x1000, 0x1E00, 0);
            func_800BD008(D_800C5BD0);
            break;
        }
        return;
    }
    if (D_800C5BC4 != 0) {
        return;
    }
    if (D_800C5300 != 0) {
        if (--D_800C5BC0 < 0) {
            D_800C5BC0 = 0x1F;
        }
    } else {
        if (++D_800C5BC0 >= 0x20) {
            D_800C5BC0 = 0;
        }
    }
    if (D_8005A646 & 0x80) {
        D_800C5BB4->index = D_800C3B94[D_800C5BC0];
    } else {
        D_800C5BB4->index = D_800C3B14[D_800C5BC0];
    }
    j = 0;
    row = D_800C3C14[D_800C5BC0][0];
    for (; j < 8; j++) {
        D_800C5B2C[j] = row[j];
        D_800C5B6C[j] = row[j + 8];
        D_800C5B4C[7 - j] = row[j + 0x10];
        D_800C5B8C[7 - j] = row[j + 0x18];
    }
    if (D_800C5300 != 0) {
        pos = D_800C4284[D_800C5BC0];
    } else {
        pos = D_800C4084[D_800C5BC0];
    }
    Tile_Update(D_800C4014[pos[0]][0], D_800C4014[pos[0]][1]);
    Tile_Update(D_800C4014[pos[1]][0], D_800C4014[pos[1]][1]);
    Tile_Update(D_800C4014[pos[2]][0], D_800C4014[pos[2]][1]);
    Tile_Update(D_800C4014[pos[3]][0], D_800C4014[pos[3]][1]);
    i = 0;
    g = (Ovl60ModeView *)D_8005A614;
    h = Heroes;
loop:
    if (i < 4) {
        if (g->mode != 10 && g->mode != 13) {
            Tiles_MakeUpIslandMap(buf, i);
            Tiles_FloodFillIslandMap(buf, i);
            if (Tiles_CountUnflagged(buf, i, 1)) {
                PlaySfx(0x51D, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
                if (g->mode == 11) {
                    Bank_Tiles(i);
                }
            }
        }
        h++;
        i++;
        goto loop;
    }
    if (--D_800C5BC8 <= 0) {
        if (!(D_8005A646 & 0x80)) {
            D_800C5BC4 = Random(0xC8) + 0xC8;
        }
        D_800C5BD0 = 0;
    } else {
        D_800C5BC4 = D_800C5BCC;
    }
    PlaySfx(0x533, NULL, NULL, 0x1000, 0x1E00, 0);
}

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

void PogoAI_Poll(u32 arg0) {
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
                    r = PogoAI_CanPlayerShootAnyone(arg0);
                    goto check;
                case 2:
                    r = PogoAI_CanPlayerShootAnyoneFourWays(arg0);
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

bool PogoAI_CanPlayerShootAnyone(u32 hero) {
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

bool PogoAI_CanPlayerShootAnyoneFourWays(u32 hero) {
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

bool PogoAI_CanPlayerShockAnyone(u32 hero) {
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
            if ((u32)(csqrt(PogoSquaredDistanceToDestination(x, z, p->hero_eng->unk10.x, p->hero_eng->unk10.z)) >> 6) < 0x381) {
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

bool PogoAI_IsSquareOKForPlayer(u32 arg0, s32 x, s32 z, s32 dx, s32 dz, u32 flags) {
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
    PogoAI_ScanForClosestPickup(arg0, &x, &z);
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

void PogoAI_ScanForClosestPickup(u32 arg0, s32 *arg1, s32 *arg2) {
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
            if (Pickup_IsClaimableByAI(arg0, i, j) != 0) {
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

#ifdef NON_MATCHING
bool PogoAI_RadialSearch(u32 i, s32 x, s32 z, s32 angle, s32 *outX, s32 *outZ) {
    s32 dx;
    s32 dz;
    s32 side0, side1, side2, side3;
    s32 off0, off1, off2, off3, off4, off5;
    s32 f0, f1, f2;
    s32 miss;
    s32 px, pz;
    s32 sb;

    dz = dx = 0;
    side0 = 0;
    side1 = 0;
    side2 = 0;
    side3 = 0;
    switch (angle) {
    case 2:
        dz = 1;
        side0 = -1;
        side1 = 0;
        side2 = dz;
        break;
    case 0:
        dz = -1;
        side0 = 1;
        side1 = 0;
        side2 = dz;
        break;
    case 1:
        dx = -1;
        side1 = dx;
        side0 = 0;
        side2 = 0;
        side3 = 1;
        break;
    case 3:
        dx = 1;
        side1 = dx;
        side0 = 0;
        side2 = 0;
        side3 = -1;
        break;
    }
    f0 = 0;
    f1 = 0;
    f2 = 0;
    off0 = side3;
    off1 = side2;
    off2 = side1;
    off3 = side0;
    off4 = dz;
    off5 = dx;
    for (;;) {
        miss = 0;
        px = x + off5;
        pz = z + off4;
        if (px >= -1 && px < 9 && pz >= -1 && pz < 9) {
            if (px >= 0 && px < 8 && pz >= 0 && pz < 8) {
                if (!f0 && D_800C53DC[px][pz].unkA != i) {
                    f0 = 1;
                }
                if (f0 && D_800C53DC[px][pz].unkA == i) {
                    goto found;
                }
            } else if (((Ovl60ModeView *)D_8005A614)->mode == 11 && f0 &&
                       func_800C0E40(i, px, pz)) {
                goto found;
            }
        } else {
            miss++;
        }
        px = x + off3;
        pz = z + off2;
        if (px >= -1 && px < 9 && pz >= -1 && pz < 9) {
            if (px >= 0 && px < 8 && pz >= 0 && pz < 8) {
                if (!f1 && D_800C53DC[px][pz].unkA != i) {
                    f1 = 1;
                }
                if (f1 && D_800C53DC[px][pz].unkA == i) {
                    goto found;
                }
            } else if (((Ovl60ModeView *)D_8005A614)->mode == 11 && f1 &&
                       func_800C0E40(i, px, pz)) {
                goto found;
            }
        } else {
            miss++;
        }
        px = x + off1;
        pz = z + off0;
        if (px >= -1 && px < 9 && pz >= -1 && pz < 9) {
            if (px >= 0 && px < 8 && pz >= 0 && pz < 8) {
                if (!f2 && D_800C53DC[px][pz].unkA != i) {
                    f2 = 1;
                }
                if (f2 && D_800C53DC[px][pz].unkA == i) {
                    goto found;
                }
            } else if (((Ovl60ModeView *)D_8005A614)->mode == 11 && f2 &&
                       func_800C0E40(i, px, pz)) {
                goto found;
            }
        } else {
            miss++;
        }
        if (miss >= 3) {
            return 0;
        }
        off0 += side3;
        off1 += side2;
        off2 += side1;
        off3 += side0;
        off4 += dz;
        off5 += dx;
    }
found:
    if (px < 0) {
        px = 0;
    }
    if (px >= 8) {
        px = 7;
    }
    if (pz < 0) {
        pz = 0;
    }
    if (pz >= 8) {
        pz = 7;
    }
    *outX = px;
    *outZ = pz;
    return D_800C53DC[px][pz].unkE == 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", PogoAI_RadialSearch);
#endif

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
    if (PogoAI_CountMyTiles(arg0) < 9) {
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
            if (PogoAI_ClaimingThisTileWillEncircle(i, j, hero) != 0) {
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

bool PogoAI_ClaimingThisTileWillEncircle(s32 x, s32 z, s32 hero) {
    s32 grid[64];
    s32 *g;
    Ovl60Cell *base;
    Ovl60Cell *c;
    s32 i;
    s32 j;

    if (PogoAI_CountNeighbours(x, z, hero) < 2) {
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
    Tiles_FloodFillIslandMap(grid, hero);
    return Tiles_CountUnflagged(grid, hero, 0) != 0;
}

s32 PogoAI_CountNeighbours(s32 arg0, s32 arg1, s32 arg2) {
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

bool Roo_FindNewSquare(EXTengine *eng) {
    s32 count;
    s32 ok;
    s32 x;
    s32 z;
    s32 v;
    s32 *pz;
    s32 *px;

    count = 0;
    do {
        ok = 1;
        count++;
        D_800C5BD8 = (eng->unk10.x + 0x800) >> 9;
        D_800C5BDC = (eng->unk10.z + 0x800) >> 9;
        z = D_800C5BEC;
        px = &D_800C5BD8;
        if (count > 5) {
            D_800C5BEC = Random(4);
            count = 0;
        }
        switch (D_800C5BEC) {
        case 0:
            D_800C5BDC -= 2;
            D_800C5BD8 -= 1 - Random(3);
            break;
        case 2:
            D_800C5BDC += 2;
            D_800C5BD8 -= 1 - Random(3);
            break;
        case 3:
            D_800C5BD8 += 2;
            D_800C5BDC -= 1 - Random(3);
            break;
        case 1:
            D_800C5BD8 -= 2;
            D_800C5BDC -= 1 - Random(3);
            break;
        }
        x = D_800C5BD8;
        if ((u32)x >= 8) {
            D_800C5BEC = (D_8004E0D8 & 1) ? 0 : 2;
            if ((u32)(x + 1) >= 10) {
                pz = &D_800C5BDC;
                v = *pz;
                if (v < 4) {
                    v++;
                } else {
                    v--;
                }
                *pz = v;
                if (D_800C5BD8 < -1) {
                    D_800C5BD8 = -1;
                }
                if (D_800C5BD8 > 8) {
                    D_800C5BD8 = 8;
                }
            }
            return 1;
        }
        z = D_800C5BDC;
        if ((u32)z >= 8) {
            D_800C5BEC = (D_8004E0D8 & 1) ? 1 : 3;
            if ((u32)(z + 1) >= 10) {
                if (x < 4) {
                    x++;
                } else {
                    x--;
                }
                *px = x;
                if (D_800C5BDC < -1) {
                    D_800C5BDC = -1;
                }
                if (D_800C5BDC > 8) {
                    D_800C5BDC = 8;
                }
            }
            return 1;
        }
        if ((SquareIsFreeForRoo(x, z) ^ 1) != 0) {
            ok = 0;
        }
        if (D_800C53DC[D_800C5BD8][D_800C5BDC].hero != NULL) {
            ok = 0;
        }
    } while (!ok);
    return 0;
}

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

void CreateFloatyPogoScore(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
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

s32 PogoAI_CountMyTiles(u32 arg0) {
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

    if (PogoAI_RadialSearch(arg0, (e->unk10.x + 0x800) >> 9, (e->unk10.z + 0x800) >> 9,
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

void Create_PogoProjectile(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
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

void Fire_RocketInDir(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    EXTengine *eng;

    eng = Heroes[arg0].hero_eng;
    Create_PogoProjectile(arg1, (((eng->unk10.x + 0x800) >> 9) << 9) - 0x700,
                  (((eng->unk10.z + 0x800) >> 9) << 9) - 0x700, arg3, arg0 & 7, arg2);
}

void func_800C1400(s32 arg0) {
    D_800C535C[arg0].flags |= 1;
}

void Player_SetDazed(u32 arg0) {
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

s32 Player_CalcForwardSpeed(u32 arg0, s32 arg1)
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

void Player_SnapToTile(u32 arg0) {
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

u32 PogoSquaredDistanceToDestination(x0, y0, x1, y1)
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

u32 PogoDistanceToDestination(s32 x0, s32 y0, s32 x1, s32 y1) {
    return csqrt(PogoSquaredDistanceToDestination(x0, y0, x1, y1)) >> 6;
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

void Tiles_MakeUpIslandMap(s32 *arg0, s32 arg1) {
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

void Tiles_CheckForIslands(s32 arg0) {
    s32 grid[64];

    if (((D5A614View *)D_8005A614)->mode != 10 && ((D5A614View *)D_8005A614)->mode != 13) {
        Tiles_MakeUpIslandMap(grid, arg0);
        Tiles_FloodFillIslandMap(grid, arg0);
        if (Tiles_CountUnflagged(grid, arg0, 1) != 0) {
            PlaySfx(0x51D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            if (((D5A614View *)D_8005A614)->mode == 11) {
                Bank_Tiles(arg0);
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
        PogoFour_UnfreezeMyTiles(arg0);
    }
    slot->unk4 = 0;
}

bool SetTile_PogoFour(s32 arg0, s32 arg1, s32 arg2, bool arg3) {
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
            PlaySquareLockedNoise(arg0);
        }
        return 1;
    }
    if (p->unkA != arg0) {
        return 0;
    }
    if (*(u16 *)p->unkC & 0x10) {
        return 0;
    }
    PogoFour_PlayerHasStoodOnOwnTile(arg0);
    Tile_Update(arg1, arg2);
    return 1;
}

bool SetTile_PogoThree(s32 arg0, s32 arg1, s32 arg2, bool arg3) {
    Ovl60Cell *base;
    Ovl60Cell *c;

    if (D_8005A648 == 0xC) {
        base = D_800C53DC[0];
        c = (Ovl60Cell *)(arg2 * sizeof(Ovl60Cell) + arg1 * sizeof(D_800C53DC[0]) + (u32)base);
        if ((u16)c->unk12 == 7) {
            Tile_Claim(arg1, arg2, arg0);
            if (arg1 > 0) {
                ZapInDir(arg1, arg2, -1, 0, arg0);
            }
            if (arg1 < 7) {
                ZapInDir(arg1, arg2, 1, 0, arg0);
            }
            if (arg2 > 0) {
                ZapInDir(arg1, arg2, 0, -1, arg0);
            }
            if (arg2 < 7) {
                ZapInDir(arg1, arg2, 0, 1, arg0);
            }
            Tiles_CheckForIslands(arg0);
            if (arg3 != 0) {
                PlaySfx(0x515, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
            return 1;
        }
        return 0;
    }
    return 0;
}

void ZapFourWays(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 > 0) {
        ZapInDir(arg0, arg1, -1, 0, arg2);
    }
    if (arg0 < 7) {
        ZapInDir(arg0, arg1, 1, 0, arg2);
    }
    if (arg1 > 0) {
        ZapInDir(arg0, arg1, 0, -1, arg2);
    }
    if (arg1 < 7) {
        ZapInDir(arg0, arg1, 0, 1, arg2);
    }
}

void ZapArrow(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
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
            ZapInDir(arg0, arg1, dx, dz, arg2);
            dir ^= 2;
        }
    }
}

void Tile_FireAllNitros(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if ((u16)D_800C53DC[i][j].unk12 == 6) {
                Tile_FireNitro(i, j);
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

void Tile_DelayedSet(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
    s16 old = p->unkA;

    p->unkA = arg2;
    p->unk14 = arg3;
    p->unk12 = 0;
    p->unk16 = old;
    if (D_8005A648 == 0xB) {
        if (arg0 > 0) {
            Tile_Update(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            Tile_Update(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            Tile_Update(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            Tile_Update(arg0, arg1 + 1);
        }
    }
}

void Tile_SetFourWay(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 7;
}

void Tile_SetNitro(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 6;
    p->unk14 = D_800C3998[D_800C5BB8][0];
}

void Tile_SetTnt(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 5;
    p->unk14 = 0x78;
}

void Tile_SetLocked(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 10;
    p->unkA = arg2;
    p->unk14 = 0xF0;
}

void Tile_Set(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = arg2;
    if (D_8005A648 == 0xB) {
        if (arg0 > 0) {
            Tile_Update(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            Tile_Update(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            Tile_Update(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            Tile_Update(arg0, arg1 + 1);
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

s32 GetPogoContour(EXTengine *arg0) {
    return arg0->unk10.y - D_8005A5D8;
}

void AddTazerRing(u32 arg0) {
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

void Player_RemoveReserve(u32 arg0) {
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

void HeroPogoTied(u32 arg0) {
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

void SetHeroPogoTied(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    Player_RemoveReserve(arg0);
    func_800C1CF0(arg0);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
}

bool TestHeroPogoTied(u32 arg0) {
    if (Heroes[arg0].hero_status & 0x100) {
        if (D_800C535C[arg0].nas != NULL) {
            HeroPogoTazingCancel(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, 0x3E);
        SetHeroSeq((HeroInfo *)arg0, 0x2E);
        return 1;
    }
    return 0;
}

bool TestHeroPogoLoser(u32 arg0) {
    if (Heroes[arg0].hero_status & 0x400) {
        if (D_800C535C[arg0].nas != NULL) {
            HeroPogoTazingCancel(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, 0x3D);
        SetHeroSeq((HeroInfo *)arg0, 0x2E);
        return 1;
    }
    return 0;
}

void SetHeroPogoWinner(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    Player_RemoveReserve(arg0);
    func_80089FA4(arg0);
    func_800C1CF0(arg0);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
    D_800AD12C = 1;
}

bool TestHeroPogoWinner(u32 arg0) {
    if (Heroes[arg0].hero_status & 0x200) {
        if (D_800C535C[arg0].nas != NULL) {
            HeroPogoTazingCancel(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, 0x3C);
        SetHeroSeq((HeroInfo *)arg0, 0x32);
        return 1;
    }
    return 0;
}

void HeroPogoTazingCancel(u32 arg0) {
    Ovl60Slot *base = D_800C535C;
    Ovl60Slot *slot = (Ovl60Slot *)(arg0 * sizeof(Ovl60Slot) + (u32)base);

    if (slot->nas != NULL) {
        KillNAS(slot->nas);
        slot->nas = NULL;
    }
}

void SetHeroPogoTazing(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    EXTheromove *move;

    Player_RemoveReserve(arg0);
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

bool TestHeroPogoShootCancel(u32 arg0) {
    Ovl60Slot *base = D_800C535C;
    Ovl60Slot *slot = (Ovl60Slot *)(arg0 * sizeof(Ovl60Slot) + (u32)base);

    if (slot->nas != NULL) {
        HeroPogoTazingCancel(arg0);
        return 1;
    }
    return 0;
}

bool TestHeroPogoShoot(u32 arg0) {
    if (D_80051338[arg0].unk54 & 0x8000) {
        switch (D_800C535C[arg0].unk4) {
        case 1:
            Fire_Rocket(arg0, 0);
            break;
        case 2:
            Fire_RocketFourWays(arg0, 0);
            break;
        case 4:
            Fire_Rocket(arg0, 1);
            break;
        case 3:
            if (Fire_Tazer(arg0, 0) != 0) {
                SetHeroAction((HeroInfo *)arg0, 0x3F);
                return 1;
            }
            break;
        }
    }
    return 0;
}

void SetHeroPogo(u32 arg0) {
    Ovl60Slot *slot;

    Heroes[arg0].hero_move->velocity_enable = 0;
    Heroes[arg0].hero_move->jump_ctrl &= 0xFE;
    slot = &D_800C535C[arg0];
    *(s16 *)((u8 *)slot + 0x16) = 0;
    slot->unk18 = (u16)slot->unk4 == 5;
    Set_Tile_Under_Player(arg0, 0);
}

void PogoHeroChooseWinner(s32 arg0, u16 arg1) {
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

void InitialisePOGO(void) {
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

void POGO_InitHero(u32 arg0) {
    Ovl60Slot *base = D_800C535C;
    Ovl60Slot *p = (Ovl60Slot *)(arg0 * sizeof(Ovl60Slot) + (u32)base);

    p->flags = 0;
    p->unk4 = 0;
    p->nas = NULL;
    D_800C53CC[arg0] = 5;
    SetHeroAction((HeroInfo *)arg0, 0x39);
    func_800C1004(arg0);
}

void PogoCreateShadow(u32 arg0) {
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

void FirstTriggersPOGO(void) {
}

void func_800C3390(void) {
}

void InitialisePOGOFirst(void) {
    D_8009D6E8.unk0 = (s32)POGO_InitHero;
    D_8009D6E8.unk44 = &D_8008AB04;
    D_8009D6E8.unk4 = &D_800C52CC;
    D_8009D6E8.unkC = &D_800C44A8;
    D_8009D6E8.unk20 = PreMovePOGO;
    D_8009D6E8.unk28 = PostMovePOGO;
    D_8009D6E8.unk64 = PogoHeroChooseWinner;
    D_8009D6E8.unk5C = func_800B76CC;
    D_8009D6E8.unk18 = PogoCreateShadow;
    D_8009D6E8.unk38 = PogoAI_Poll;
    func_800B3534();
}

void POGOSetUp(void) {
    D_8005AA70[0] = (s32)InitialisePOGOFirst;
    D_8005AA70[1] = (s32)InitialisePOGO;
    D_8005AA70[2] = (s32)InitialisePOGO;
    D_8005AA70[3] = (s32)UpdatePOGO;
    D_8005AA70[5] = (s32)FirstTriggersPOGO;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[9] = (s32)GetPogoContour;
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
