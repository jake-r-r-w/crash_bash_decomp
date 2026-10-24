#include "common.h"
#include "bash/ovl64.h"
#include "bash/spin.h"
#include "bash/hero.h"
#include "bash/shell.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/bash.h"
#include "bash/xtras.h"

void func_800B3428() {
    void *p;
    void **q;

    D_8009D6E8.unk20 = func_800B90D0;
    D_8009D6E8.unk68 = func_800B8F48;
    D_8009D6E8.unk44 = func_800BB5F0;
    D_8009D6E8.unk3C = func_800B8E54;
    D_8009D6E8.unkC = D_800BB80C;
    D_8009D6E8.unk38 = func_800B7508;
    D_8009D6E8.unk64 = func_800B4144;
    D_8009D6E8.unk4C = func_800BA098;
    D_8009D6E8.unk50 = func_800BA344;
    D_8009D6E8.unk54 = func_800BA514;
    D_8009D6E8.unk58 = func_800BAF1C;
    D_8009D6E8.unk5C = func_800B6C50;
    D_8009D6E8.unk60 = func_800B8E5C;
    D_8009D6E8.unk18 = func_800BB560;
    D_8009D6E8.unk10 = (Ovl64JumpEnt *)D_800BB7BC;
    D_8009E0E2 = 0x38;
    D_8009E0E4 = 0x36;
    D_8009E0E6 = 0x37;
    D_8009E0D8 = 0x33;
    func_800B8EE0();
    D_8009D8F4 = D_800BB76C;
    D_8009D8F8 = D_800BB780;
    D_8009D904 = D_800BB754;
    q = &D_8009D8FC;
    if (D_8005A680 != 0) {
        p = D_800BB734;
    } else {
        p = D_800BB720;
    }
    *q = p;
    D_8009D900 = D_800BB748;
}

void func_800B359C() {
    s32 i;
    EXTheromove *m;
    s16 a;

    if (D_8005A646 & 0x40) {
        Init_Clock(0x140, 0x28, 0);
        Set_Clock(0x125, 0x81D0);
    } else {
        Init_Clock(0x140, 0x28, 0);
        Set_Clock(0x130, 0x81D0);
    }
    World_floor_level = -0x10;
    D_800BBD52 = 0x1F4;
    D_800BBD54 = 0x12C;
    D_800BBD56 = 0x190;
    D_800BBD58 = 0x1E;
    D_800BBD5A = 0;
    D_800BBD5C = 0;
    D_800BBD5E = 0;
    D_800BBD62 = 0;
    D_800BBD64 = 0;
    D_800BBD68 = 0;
    D_800BBD6C = EngineTemplate;
    for (i = 0; i < 4; i++) {
        a = (get_angle(Heroes[i].hero_eng->unk10.x, Heroes[i].hero_eng->unk10.z) + 0x800) & 0xFFF;
        m = Heroes[i].hero_move;
        Heroes[i].hero_eng->unk1E = a;
        m->facing_angle = a;
        m->forward_angle = a;
        Heroes[i].hero->Camera.width = 0;
        Heroes[i].hero->Camera.look_ahead = 0;
        if (((Ovl64Game *)D_8005A614)->cpu[i] != 0) {
            Heroes[i].hero_move->run_walk = 2;
        }
    }
    if (((Ovl64Game *)D_8005A614)->flags & 0x80) {
        func_8001682C(&D_80053004);
    }
    if (((Ovl64Game *)D_8005A614)->flags & 0x40) {
        if (((Ovl64Game *)D_8005A614)->teams != 0) {
            Heroes[0].hero_score = 0x28;
            Heroes[1].hero_score = 0;
            Heroes[2].hero_score = 0x1E;
            Heroes[3].hero_score = 0;
            InitSharedScoreIcons();
        } else {
            Heroes[0].hero_score = 0x1E;
            Heroes[1].hero_score = 0x19;
            Heroes[2].hero_score = 0x19;
            Heroes[3].hero_score = 0x19;
        }
    }
    func_800B382C();
    func_80086D88();
    func_800B3938();
    PlaySfx(0x5EC, &D_800BBE00, NULL, 0x1000, 0x1E00, 0);
}

void func_800B382C(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 y;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    Ovl64Cell *p;
    Ovl64Cell *base;
    s32 t0;
    s32 t1;

    i = 0;
    base = D_800BBE10[0];
    y = 0x500;
    for (; i < 4; i++) {
        j = 0;
        d = i << 7;
        for (; j < 16; j++) {
            t0 = j << 8;
            p = (Ovl64Cell *)(d + (s32)base);
            p->x = t0;
            p->y = y;
            d += 8;
        }
        y += 0x140;
    }
    for (k = 0; k < 300; k++) {
        i = Random(4);
        j = Random(16);
        c = Random(4);
        d = Random(16);
        t0 = D_800BBE10[i][j].x;
        t1 = D_800BBE10[i][j].y;
        D_800BBE10[i][j].x = D_800BBE10[c][d].x;
        D_800BBE10[i][j].y = D_800BBE10[c][d].y;
        D_800BBE10[c][d].x = t0;
        D_800BBE10[c][d].y = t1;
    }
}

void func_800B3938(void) {
    s32 i;
    Ovl64Obj *obj;
    s16 *p;

    D_800BC010 = 0;
    D_800BC014 = 0;
    D_800BC01C = 1;
    D_800BC018 = 0;
    D_800BC01A = 0;
    obj = ((Ovl64Scene *)D_80058AD0)->objs;
    for (i = 9; i >= 0; i--) {
        D_800BC020[i] = NULL;
    }
    for (; obj != NULL; obj = obj->next) {
        p = (s16 *)func_80015870(obj->model, obj->index);
        i = *p - 10;
        if ((u32)i < 10) {
            D_800BC020[i] = obj;
            obj->unk51 = 0;
        }
        i = *p - 20;
        if ((u32)i < 10) {
            D_800BBCB0[i].obj = obj;
            obj->unk51 = 0;
        }
    }
}

void func_800B3A3C(void) {
    Ovl64Obj *obj;
    HeroInfo *h;
    EXTengine *eng;
    EXTengine *e;
    NAS *n;
    Ovl64Fux *f;
    s32 i;
    s32 x;
    s32 z;
    s32 d;
    s32 a;

    if (D_8005A644 & 0x801) {
        D_800BC01C = 0;
    }
    a = D_800BC018 - D_800BC01C * 13;
    if (a != 0) {
        if (a < 0) {
            D_800BC018++;
        } else {
            D_800BC018--;
        }
    }
    a = D_800BC01A - D_800BC01C * 24;
    if (a != 0) {
        if (a < 0) {
            D_800BC01A++;
        } else {
            D_800BC01A--;
        }
    }
    D_800BC010 += D_800BC018;
    D_800BC014 -= D_800BC01A;
    obj = D_800BC020[0];
    if (obj != NULL) {
        obj->dirty = 1;
        obj->ry = D_800BC010;
    }
    obj = D_800BC020[1];
    if (obj != NULL) {
        obj->dirty = 1;
        obj->ry = D_800BC010;
    }
    for (i = 0; i < 10; i++) {
        obj = D_800BBCB0[i].obj;
        if (obj != NULL) {
            if (D_800BBCB0[i].fixed[0] != 0) {
                obj->rx = D_800BBCB0[i].fixed[0];
            } else {
                obj->rx = (u32)(D_800BC010 * D_800BBCB0[i].rate[0]) >> 4;
            }
            if (D_800BBCB0[i].fixed[1] != 0) {
                obj->ry = D_800BBCB0[i].fixed[1];
            } else {
                obj->ry = (u32)(D_800BC010 * D_800BBCB0[i].rate[1]) >> 4;
            }
            if (D_800BBCB0[i].fixed[2] != 0) {
                obj->rz = D_800BBCB0[i].fixed[2];
            } else {
                obj->rz = (u32)(D_800BC010 * D_800BBCB0[i].rate[2]) >> 4;
            }
            obj->dirty = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_status & 0xFA00) continue;
        eng = h->hero_eng;
        if (eng->unk10.y < World_floor_level) continue;
        if (h->hero_pu == 0x13) continue;
        x = eng->unk10.x;
        z = eng->unk10.z;
        d = x * x + z * z;
        if (d > 0x640000) continue;
        if (d <= 0xC3FFF) continue;
        d = (csqrt(d) + 0x20) >> 6;
        a = get_angle(x, z) + D_800BC018;
        a &= 0xFFF;
        h->hero_eng->unk10.x = d * D_80068BD4[(a - 0x400) & 0xFFF].unk2 >> 12;
        h->hero_eng->unk10.z = d * D_80068BD4[a].unk2 >> 12;
        h->hero_move->forward_angle += D_800BC018;
        h->hero_move->forward_angle &= 0xFFF;
    }
    for (n = D_8005AA24; n != NULL; n = n->Object.Child) {
        f = (Ovl64Fux *)n->FUX;
        if (f == NULL) continue;
        e = n->Engine;
        if (e == NULL) continue;
        x = e->unk10.x;
        z = e->unk10.z;
        d = x * x + z * z;
        if (d > 0x640000) continue;
        if (e->unk10.y < World_floor_level) continue;
        if (d <= 0xC3FFF) continue;
        d = f->dist;
        a = f->angle;
        a += D_800BC018;
        a &= 0xFFF;
        f->angle = a;
        e->unk10.x = d * D_80068BD4[(a - 0x400) & 0xFFF].unk2 >> 12;
        e->unk10.z = d * D_80068BD4[a].unk2 >> 12;
        e->unk1E = (e->unk1E + D_800BC018) & 0xFFF;
        e->unk5E = 1;
    }
}

s32 func_800B3EDC(void) {
    Ovl64Game *g;
    HeroInfo *h;
    s32 j;
    s32 i;
    Ovl64Game *gt;
    s32 a;
    s32 b;
    s32 mate;
    s32 score;
    s32 team0;
    s32 team1;
    s32 mate1;
    s32 score1;
    u8 *p;
    s32 k;
    s32 m;

    j = 3;
    p = D_8005A614 + j;
    do {
        p[0xA7] = j;
        j--;
        p--;
    } while (j >= 0);
    i = 0;
    g = (Ovl64Game *)D_8005A614;
    h = Heroes;
    while (1) {
        if (!(i < 4)) break;
        j = 0;
        while (1) {
            if (!(j < 3)) break;
            if (g->flags & 0x40) {
                if (h[g->rank[j]].hero_score > h[((u8 *)((k = j + 1) + (s32)g))[0xA7]].hero_score) {
                    a = g->rank[j];
                    g->rank[j] = ((u8 *)(k + (s32)g))[0xA7];
                    ((u8 *)(k + (s32)g))[0xA7] = a;
                }
            } else {
                if (h[g->rank[j]].hero_score < h[((u8 *)((m = j + 1) + (s32)g))[0xA7]].hero_score) {
                    a = g->rank[j];
                    g->rank[j] = ((u8 *)(m + (s32)g))[0xA7];
                    ((u8 *)(m + (s32)g))[0xA7] = a;
                }
            }
            j++;
        }
        i++;
    }
    gt = (Ovl64Game *)D_8005A614;
    if (gt->teams != 0) {
        team0 = gt->team[0];
        mate = Heroes[team0].hero_teammate;
        score = Heroes[team0].hero_score;
        D_800BBE08 = score;
        if (mate != 0) {
            D_800BBE08 = score + Heroes[mate].hero_score;
        }
        team1 = gt->team[1];
        mate1 = Heroes[team1].hero_teammate;
        score1 = Heroes[team1].hero_score;
        D_800BBE0C = score1;
        if (mate1 != 0) {
            D_800BBE0C = score1 + Heroes[mate1].hero_score;
        }
        if (gt->flags & 0x40) {
            if (D_800BBE08 < D_800BBE0C) {
            ra:
                return team0;
            }
        rb:
            return team1;
        }
        if (D_800BBE08 > D_800BBE0C) {
            goto ra;
        }
        goto rb;
    }
    return gt->rank[0];
}

void func_800B4144(s32 arg0, u16 arg1) {
    s32 found;
    Ovl64Game *g;
    Ovl64Game *game;
    HeroInfo *hb;
    HeroInfo *heroes;
    HeroInfo *h;
    Ovl64Game *p;
    s32 i;
    s32 mate;

    if (OVL64_GAME->state & 0x801) {
        return;
    }
    found = 0;
    if (OVL64_GAME->flags & 0x80) {
        for (i = 0; i < 4; i++) {
            if (!(Heroes[i].hero_status & 0xFA00) && (i == 0 || (OVL64_GAME->teams != 0 && i == 1))) {
                found++;
            }
        }
        if (found == 0) {
            OVL64_DRAW(OVL64_GAME, arg1, i);
            return;
        }
    }
    if (OVL64_GAME->flags & 0x40) {
        if (arg0 == -1) {
            OVL64_DRAW(OVL64_GAME, arg1, i);
            return;
        }
        found = -1;
        if (OVL64_GAME->teams != 0) {
            if (Heroes[OVL64_GAME->team[0]].hero_score == 0) {
                found = OVL64_GAME->team[0];
            }
            if (Heroes[OVL64_GAME->team[1]].hero_score == 0) {
                found = OVL64_GAME->team[1];
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (Heroes[i].hero_score == 0) {
                    found = i;
                }
            }
        }
        if (found != -1) {
            if (found == 0 || (OVL64_GAME->teams != 0 && found == 1)) {
                Heroes[found].hero_status |= 0x200;
                if (OVL64_GAME->teams != 0 && (mate = Heroes[found].hero_teammate) != -1) {
                    Heroes[mate].hero_status |= 0x200;
                }
                OVL64_GAME->winner[0] = Heroes[found].hero_name;
                OVL64_GAME->state |= arg1;
                for (i = 0; i < 4; i++) {
                    if (!(Heroes[i].hero_status & 0xFA00)) {
                        Heroes[i].hero_status |= 0x400;
                    }
                }
                return;
            }
            OVL64_DRAW(OVL64_GAME, arg1, i);
            return;
        }
    }
    if (arg0 != -1) {
        return;
    }
    found = func_800B3EDC();
    g = (Ovl64Game *)D_8005A614;
    if ((g->flags & 0xE0) && found != 0) {
        if (g->teams != 0) {
            if (found == 1) {
                goto decide;
            }
            InitBigScore();
        }
        OVL64_DRAW(g, arg1, i);
        return;
    }
decide:
    game = (Ovl64Game *)D_8005A614;
    if (game->teams != 0) {
        InitBigScore();
        if (D_800BBE08 == D_800BBE0C) {
            game->winner[0] = 0;
            game->state |= arg1;
            for (i = 0; i < 4; i++) {
                Heroes[i].hero_status |= 0x100;
            }
            return;
        }
        game->state |= 0x800;
        if (found == game->team[0]) {
            hb = Heroes;
            h = &hb[found];
            mate = h->hero_teammate;
            if (h->hero_status & 0xFA00) {
                game->winner[0] = mate;
                hb[mate].hero_status |= 0x200;
            } else if (hb[mate].hero_status & 0xFA00) {
                game->winner[0] = h->hero_name;
                h->hero_status |= 0x200;
            } else {
                game->winner[0] = h->hero_name;
                h->hero_status |= 0x200;
                game->winner[1] = hb[mate].hero_name;
                hb[mate].hero_status |= 0x200;
            }
            Heroes[OVL64_GAME->team[1]].hero_status |= 0x400;
            mate = Heroes[OVL64_GAME->team[1]].hero_teammate;
            if (mate != 0) {
                Heroes[mate].hero_status |= 0x400;
            }
        } else {
            game->winner[0] = Heroes[game->team[1]].hero_name;
            Heroes[game->team[1]].hero_status |= 0x200;
            mate = Heroes[game->team[1]].hero_teammate;
            if (mate != 0) {
                game->winner[1] = Heroes[mate].hero_name;
                Heroes[mate].hero_status |= 0x200;
            }
            Heroes[game->team[0]].hero_status |= 0x400;
            mate = Heroes[game->team[0]].hero_teammate;
            if (mate != 0) {
                Heroes[mate].hero_status |= 0x400;
            }
        }
    } else {
        heroes = Heroes;
        h = &heroes[game->rank[0]];
        if (h->hero_score == heroes[game->rank[1]].hero_score) {
            game->winner[0] = 0;
            game->state |= arg1;
            for (i = 0; i < 4; i++) {
                Heroes[i].hero_status |= 0x100;
            }
            return;
        }
        p = game;
        p->winner[0] = h->hero_name;
        p->state |= 0x800;
        for (i = 0; i < 4; i++) {
            if (p->rank[0] == i) {
                heroes[i].hero_status |= 0x200;
            } else {
                heroes[i].hero_status |= 0x400;
            }
        }
    }
}

void func_800B4970(void) {
    s32 i;
    s32 hero;
    s32 max;
    s32 d;

    if (D_8005A644 & 0x801) {
        return;
    }
    if (D_800BBD64 != 0) {
        D_800BBD64--;
    }
    hero = -1;
    max = 8;
    i = 0;
    do {
        if (Heroes[i].hero_pu == 0x17) {
            hero = i;
            if (D_800BBD58 >= 0x1F) {
                D_800BBD58 = 0x1E;
            }
            max = 10;
        }
        i++;
    } while (i < 4);
    if (D_8009E46C[2] == 0 && hero == -1) {
        if (D_800BBD54 != 0) {
            D_800BBD54--;
        } else {
            func_800B8E64(1, 2);
            D_800BBD54 = Random(0x3C) + 0x5A;
        }
    }
    if (D_8009E46C[12] == 0) {
        if (D_800BBD56 != 0) {
            D_800BBD56--;
        } else {
            func_800B4DC4(1, 0xC);
            D_800BBD54 = Random(0x3C) + 0x5A;
        }
    }
    D_800BBD5C = D_8009E46C[8] + D_8009E46C[9] + D_8009E46C[10] + D_8009E46C[11];
    if (D_800BBD58 != 0) {
        D_800BBD58--;
    } else if (D_800BBD5C < max) {
        d = max - D_800BBD5C;
        if (d < 4) {
            i = Random(d) + 1;
            D_800BBD58 = Random(0x14) + 0x1E;
        } else {
            i = 4;
            D_800BBD58 = Random(0xA) + 0x14;
        }
        if (hero != -1) {
            func_800B4DC4(i, D_800BB88C[hero]);
        } else {
            func_800B4DC4(i, -1);
        }
    } else {
        D_800BBD58 = Random(0x1E) + 0x3C;
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl64/ovl64", D_800B32B8);

void func_800B4BA4(void) {
    s32 i;
    u16 *p;
    s32 j;
    NAS *n;
    EXTengine *e;
    Ovl64Fux *f;
    s32 t;
    s32 x0, x1, z0, z1;

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
    for (n = D_8005AA24; n != NULL; n = n->Object.Child) {
        e = n->Engine;
        if (e == NULL) continue;
        f = (Ovl64Fux *)n->FUX;
        if (f == NULL) continue;
        t = f->kind;
        x0 = ((e->unk10.x - 0x80) >> 8) + 10;
        x1 = ((e->unk10.x + 0x80) >> 8) + 10;
        z0 = 9 - ((e->unk10.z - 0x80) >> 8);
        z1 = 9 - ((e->unk10.z + 0x80) >> 8);
        if ((u32)x0 < 20) {
            if ((u32)z0 < 20) {
                D_8009D90C[z0 * 20 + x0].unk0 |= t;
            }
            if ((u32)z1 < 20) {
                D_8009D90C[z1 * 20 + x0].unk0 |= t;
            }
        }
        if ((u32)x1 < 20) {
            if ((u32)z0 < 20) {
                D_8009D90C[z0 * 20 + x1].unk0 |= t;
            }
            if ((u32)z1 < 20) {
                D_8009D90C[z1 * 20 + x1].unk0 |= t;
            }
        }
        if ((u16)(f->kind - 1) < 12) {
            switch (f->sub) {
            case 0:
            case 1:
            case 3:
            case 6:
            case 7:
                D_8009E46C[0]++;
                D_8009E46C[f->kind]++;
                break;
            }
        }
    }
}

void func_800B4DC4(u16 count, s16 arg1) {
    Ovl64Pair *p;
    s32 a;
    s32 b;
    s32 i;
    s16 id;
    u16 kind;
    u16 idx;
    s32 n;
    s32 t;
    s32 r;

    id = arg1;
    if (arg1 > 0) {
        p = func_800B4F7C(&D_800BBD5E);
        if (p != NULL) {
            func_800B5D3C(p->a, p->b, id, -1);
        }
    } else if (count != 0) {
        i = 0;
        do {
            p = func_800B4F7C(&D_800BBD5E);
            if (p == NULL) {
                continue;
            }
            a = p->a;
            idx = Ovl64NextSlot();
            t = D_800BB88C[idx];
            b = p->b;
            kind = D_8009E46C[t];
            id = t;
            if (kind >= 2) {
                continue;
            }
            if (((Ovl64Game *)D_8005A614)->cpu[idx] == 0
                && (n = D_8009D87C[idx][0], n++, (u32)Clock_Time >> 8 < 0x120)) {
                if (kind == 0) {
                    r = (u32)Random(n) < 9;
                } else {
                    r = (u32)Random(n) < 7;
                }
                if (!r) {
                    break;
                }
            }
            func_800B5D3C(a, b, id, -1);
        } while (++i < count);
    }
}

Ovl64Pair *func_800B4F7C(u16 *head) {
    Ovl64Cell *p;
    Ovl64Cell *bestp;
    s32 i;
    u16 besti;
    u16 best;
    u16 tries;
    u16 n;
    s32 x;
    s32 z;
    s32 a;
    s32 b;

    p = D_800BBE10[0];
    best = 27;
    tries = 9;
    i = *head;
    bestp = p = p + i;
    besti = i++;
    i &= 63;
    x = (p->x >> 8) + 10;
    z = 9 - (p->y >> 8);
    do {
        while (D_8009D90C[z * 20 + x].unk0 != 0) {
            if (i == *head) {
                goto fail;
            }
            p = D_800BBE10[0];
            p += i++;
            x = (p->x >> 8) + 10;
            z = 9 - (p->y >> 8);
            i &= 63;
        }
        n = 0;
        for (a = x - 3; a < x + 3; a++) {
            for (b = z - 3; b < z + 3; b++) {
                if ((u32)a < 20 && b >= 0 && b < 20 && D_8009D90C[b * 20 + a].unk0 != 0) {
                    n++;
                }
            }
        }
        if (n == 0) {
            *head = i;
            return (Ovl64Pair *)p;
        }
        if (n < best) {
            best = n;
            bestp = p;
            besti = i;
        }
        if (i == *head) {
        fail:
            return NULL;
        }
        p = D_800BBE10[0];
        p += i++;
        i &= 63;
        x = (p->x >> 8) + 10;
        z = 9 - (p->y >> 8);
    } while (tries-- != 0);
    *head = besti;
    return (Ovl64Pair *)bestp;
}

NAS *func_800B51A0(s32 a, s32 b, u16 c, s16 d)
{
  NAS *nas;
  EXTengine *eng;
  Ovl64Fux *fux;
  s32 x;
  Ovl64Fux *new_var;
  s32 z;
  u16 mask;
  x = (b * D_80068BD4[(a - 0x400) & 0xFFF].unk2) >> 12;
  z = (b * D_80068BD4[a & 0xFFF].unk2) >> 12;
  nas = CreateNAS(x, World_floor_level, z, 0x1801, 0x11);
  if (nas != 0)
  {
    eng = nas->Engine;
    if (eng != 0)
    {
      fux = (Ovl64Fux *) AddObject(nas);
      if (fux != 0)
      {
        fux->angle = a;
        fux->dist = b;
        if (AddMotion(nas) != 0)
        {
          func_80026AD8((u8 *) nas);
          func_8002694C(nas, 0x300);
          nas->FUX = (EXT *) fux;
          eng->sub->unk6C = D_80052F94;
          eng->unk5D = 1;
          fux->kind = c;
          D_8009E46C[fux->kind]++;
          if (d >= 0)
          {
            fux->sub = d;
          }
          else
          {
            fux->sub = 6;
          }
          switch (c)
          {
            case 2:
              eng->sub->unk74 = 0x2014;
              fux->state = D_800BB888[Random(2)];
              break;

            case 5:
            {

              eng->sub->unk74 = 0x2009;
            }
              z = 0x5A;
              fux->timer = z;
              new_var = fux;
              mask = new_var->mask;
              goto set_mask;


            case 6:
            {

              eng->sub->unk74 = 0x2001;
            }
              mask = fux->mask;
              set_mask:
            fux->mask = mask | 0x100;

              break;


            case 4:

            default:
              eng->sub->unk74 = 0x2013;
              break;

          }

          switch (fux->sub)
          {
            case 3:
              eng->unk10.y = World_floor_level;
              break;

            case 7:
              eng->unk2C.x = 0x40;
              eng->unk2C.y = 0x40;
              eng->unk2C.z = 0x40;
              eng->unk5D = 1;
              eng->unk5C = 1;
              eng->unk5E = 1;
              eng->flags |= 1;
              eng->unk10.y = World_floor_level;
              PlaySfx(0x3FA, 0, &eng->unk10, 0x1000, 0x1E00, 0);
              break;

            case 6:
              eng->unk2C.y = 0x40;
              eng->unk5D = 1;
              eng->unk5C = 1;
              eng->unk5E = 1;
              eng->unk10.y = World_floor_level;
              PlaySfx(0x3FA, 0, &eng->unk10, 0x1000, 0x1E00, 0);
              break;

          }

        }
      }
    }
    if (nas->Failure != 0)
    {
      KillNAS(nas);
      return 0;
    }
  }
  return nas;
}

void PlotHazardType(NAS *nas) {
    EXTengine *eng;
    Ovl64Fux *fux;
    s32 kind;
    Ovl64Glow *glow;
    s32 v;
    s16 owner;
    HeroInfo *h;
    HeroInfo *hb;
    s32 a;
    s32 b;
    s16 w;
    s16 o;
    u16 t;
    u32 r;
    s32 done;
    VECTOR pos;
    EXTengine *he;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    D_8009D750(eng);
    fux = (Ovl64Fux *)nas->FUX;
    if (fux == NULL) {
        return;
    }
    kind = fux->kind;
    glow = FindExtension(nas, 0x800);
    if (glow == NULL) {
        return;
    }
    switch (fux->sub) {
    case 0:
        fux->sub = 1;
        fux->mask = (fux->mask & 0xFFD0) | 0xA;
        eng->unk5E = 1;
        fux->owner = -1;
        FetchDefaultColBox(eng, 0x50);
        break;
    case 6:
        v = eng->unk2C.y;
        v += 0x200;
        eng->unk1E = 0;
        if (v >= 0x1000) {
            v = 0x1000;
            fux->sub = 0;
        }
        eng->unk2C.y = v;
        eng->unk5E = 1;
        break;
    case 7:
        v = eng->unk2C.x;
        v += 0x100;
        if (v >= 0x1000) {
            v = 0x1000;
            fux->sub = 0;
            eng->flags &= ~0x21;
        }
        eng->unk2C.x = v;
        eng->unk2C.y = v;
        eng->unk2C.z = v;
        eng->sub->unk76 = v;
        eng->unk5E = 1;
        break;
    case 9:
        switch (kind) {
        case 2:
            Ovl64DropHazard(fux);
            func_8008E27C((s32 *)&eng->unk10, fux, 0);
            fux->sub = 0xC;
            break;
        case 6:
            Ovl64DropHazard(fux);
            func_8008E27C((s32 *)&eng->unk10, fux, 0);
            func_800270B0(nas);
            break;
        case 5:
            fux->timer = 0x5A;
            fux->kind = 7;
            PlaySfx(0x3F0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            break;
        default:
            fux->sub = 0;
            break;
        }
        break;
    case 3:
        fux->sub = 0;
        break;
    case 12:
        fux->timer = 0x78;
        fux->mask &= 0xFFF4;
        eng->sub->unk6C = D_80052FCC;
        glow->unk18 = 0x60;
        glow->unk1E = 0xF0;
        glow->unk1C = 4;
        glow->unk1A = 0;
        glow->unkC = 0xFF;
        eng->unk1C = 0;
        eng->unk20 = 0;
        owner = fux->owner;
        if (owner != -1) {
            a = D_8009D87C[owner][0];
            b = 0x20 - a;
            if (D_8005A614[owner + 0x5C] != 0) {
                w = a;
                o = b;
            } else {
                w = b;
                o = a;
            }
            t = w + o;
            if (t == 0) {
                return;
            }
            r = (u16)Random(t);
            if ((u16)w < r) {
                fux->state = 0x13;
            } else {
                fux->state = 0x17;
            }
        }
        switch (fux->state) {
        case 0x17:
            eng->sub->unk74 = 0x2015;
            fux->sub = 0xD;
            break;
        case 0x13:
            eng->sub->unk74 = 0x2014;
            fux->sub = 0xD;
            break;
        }
        break;
    case 13:
        if (D_8009D750(eng) != 0) {
            func_800270B0(nas);
            break;
        }
        done = 0;
        owner = fux->owner;
        if (owner != -1) {
            if (glow->unkC == 0) {
                s32 dx;
                s32 dy;
                s32 dz;

                fux->mask |= 8;
                hb = Heroes;
                h = &hb[owner];
                if (func_80015780(h->hero_eng, h->hero_sub, 0x80, &pos) != NULL) {
                    dx = eng->unk10.x - pos.vx;
                    dy = eng->unk10.y - pos.vy;
                    dz = eng->unk10.z - pos.vz;
                } else {
                    s32 ty;

                    dx = eng->unk10.x - hb[fux->owner].hero_eng->unk10.x;
                    ty = eng->unk10.y + 0x180;
                    dy = ty - hb[fux->owner].hero_eng->unk10.y;
                    dz = eng->unk10.z - hb[fux->owner].hero_eng->unk10.z;
                }
                dx >>= 2;
                dy >>= 2;
                dz >>= 2;
                eng->unk10.x -= dx;
                eng->unk10.y -= dy;
                eng->unk10.z -= dz;
                dx >>= 1;
                dz >>= 1;
                if (dx == 0 && dz == 0) {
                    done = 1;
                }
                func_800BB37C(eng, (Ovl64HitMask *)fux, 8, fux->owner);
                if (fux->hit & 8) {
                    done = 1;
                }
            }
        } else {
            KillNAS(nas);
            break;
        }
        if (GravitateNas(nas) != 0) {
            done = 1;
        }
        if (done) {
            owner = fux->owner;
            if (owner != -1 && owner < 5) {
                Heroes[owner].hero_pu = fux->state;
                switch (fux->state) {
                case 0x17:
                    he = Heroes[fux->owner].hero_powerup;
                    PlaySfx(0x5F2, &he->sfx, &he->unk10, 0x1000, 0x1E00, 0);
                    Heroes[fux->owner].hero_pu_timer = 0x12C;
                    Heroes[fux->owner].hero_powerup->sub->unk74 = 0x2015;
                    Heroes[fux->owner].hero_powerup->sub->unk6C = D_80052FCC;
                    break;
                case 0x13:
                    he = Heroes[fux->owner].hero_powerup;
                    PlaySfx(0x5F3, &he->sfx, &he->unk10, 0x1000, 0x1E00, 0);
                    Heroes[fux->owner].hero_pu_timer = 0xF0;
                    Heroes[fux->owner].hero_powerup->sub->unk74 = 0x2014;
                    Heroes[fux->owner].hero_powerup->sub->unk6C = D_80052FCC;
                    PlaySfx(0x41C, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    break;
                }
            }
            func_800270B0(nas);
        }
        eng->unk5E = 1;
        eng->unk1E += 0x71;
        break;
    default:
        if (fux->hit & 2) {
            if (fux->owner != -1) {
                fux->sub = 9;
                fux->hit &= 0xFFF4;
                fux->mask &= 0xFFF4;
            } else {
                fux->hit &= 0xFFFC;
            }
        }
        if ((fux->hit & 8) && kind == 6) {
            func_8008E27C((s32 *)&eng->unk10, fux, 0);
            func_800270B0(nas);
        }
        break;
    }
}

NAS *func_800B5D3C(s32 a, s32 b, u16 c, s16 d) {
    NAS *nas;
    EXTengine *eng;
    EXTengine *shadow;
    Ovl64Fux *fux;
    EXTmotion *mot;
    s32 x;
    s32 y;
    s32 z;
    s32 r;

    x = b * D_80068BD4[(a - 0x400) & 0xFFF].unk2 >> 12;
    z = b * D_80068BD4[a & 0xFFF].unk2 >> 12;
    r = Random(0x80);
    y = World_floor_level - 0x600;
    nas = CreateNAS(x, y - r, z, 0x1802, 0x11);
    if (nas != NULL) {
        eng = nas->Engine;
        if (eng != NULL) {
            eng->flags |= 0x20000000;
            fux = (Ovl64Fux *)AddObject(nas);
            if (fux != NULL) {
                fux->angle = a;
                fux->dist = b;
                mot = AddMotion(nas);
                if (mot != NULL) {
                    func_80026AD8((u8 *)nas);
                    func_8002694C(nas, 0x300);
                    nas->FUX = (EXT *)fux;
                    eng->sub->unk6C = D_80052FCC;
                    eng->unk5D = 1;
                    fux->kind = c;
                    D_8009E46C[fux->kind]++;
                    if (d >= 0) {
                        fux->sub = d;
                    } else {
                        fux->sub = 3;
                    }
                    switch (c) {
                    case 8:
                        eng->sub->unk74 = 0x2003;
                        fux->state = 1;
                        break;
                    case 9:
                        eng->sub->unk74 = 0x2001;
                        fux->state = 2;
                        break;
                    case 10:
                        eng->sub->unk74 = 0x2002;
                        fux->state = 3;
                        break;
                    case 11:
                        eng->sub->unk74 = 0x2004;
                        fux->state = 0;
                        break;
                    case 12:
                    default:
                        eng->sub->unk74 = 0x2005;
                        fux->state = Random(3) + 4;
                        break;
                    }
                    mot->Xvel = Random(8) + 4;
                    mot->Xa = a + 0x800;
                    mot->Xa = (mot->Xa - D_800BC01C * 0x200) & 0xFFF;
                }
            }
            shadow = AddExtEngine(nas, 2);
            if (shadow != NULL) {
                shadow->sub->unk6C = D_80052F5C;
                shadow->sub->unk74 = 0x2001;
                shadow->unk10 = eng->unk10;
                shadow->flags |= 0x41;
                shadow->sub->unk76 = 0x800;
                shadow->sub->flags |= 0x40000000;
            }
        }
        if (nas->Failure != 0) {
            KillNAS(nas);
            return NULL;
        }
    }
    return nas;
}

void PlotBalloon(NAS *nas) {
    EXTengine *eng;
    Ovl64Fux *fux;
    EXTmotion *mot;
    EXTengine *shadow;
    s32 kind;
    s32 owner;
    s32 s;
    s32 a;
    u16 t;
    u16 uo;
    s32 o;
    s32 g;
    s32 y;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    D_8009D750(eng);
    fux = (Ovl64Fux *)nas->FUX;
    if (fux != NULL) {
        kind = fux->kind;
        if ((mot = FindExtension(nas, 0x800)) != NULL) {
            switch (fux->sub) {
            case 0:
                fux->sub = 1;
                fux->mask &= 0xFFF5;
                if (kind < 0xD) if (kind >= 8) {
                    fux->mask |= 0xA;
                }
                eng->unk5E = 1;
                fux->owner = -1;
                FetchDefaultColBox(eng, 0x50);
                break;
            case 9:
                switch (kind) {
                case 8:
                case 9:
                case 10:
                case 11:
                    uo = fux->owner;
                    owner = (s16)uo;
                    if (owner != -1 && owner < 4) {
                        if (D_800BBD64 != 0) {
                            if (D_800BBD62 == owner) {
                                if (D_8005A646 & 0x40) {
                                    Heroes[uo & 2].hero_score--;
                                } else {
                                    Heroes[owner].hero_score++;
                                }
                            } else {
                                SetHeroAction((HeroInfo *)owner, (u16)D_8009E0D8);
                                if (OVL64_GAME->flags & 0x80) {
                                    owner = fux->owner;
                                    if (owner == 0 || (OVL64_GAME->teams != 0 && owner == 1)) {
                                        Heroes[owner].hero_health = 0;
                                        func_80084E04(fux->owner, NULL, -1, 1, 8);
                                    }
                                }
                            }
                            func_8007A524(D_800BB89C[D_800BBD62], &eng->unk10.x, (s32)D_80052FCC);
                            goto sfx;
                        }
                        if (fux->state == owner) {
                            if (D_8005A646 & 0x40) {
                                Heroes[uo & 2].hero_score--;
                            } else {
                                Heroes[owner].hero_score++;
                            }
                        } else {
                            SetHeroAction((HeroInfo *)owner, (u16)D_8009E0D8);
                            if (OVL64_GAME->flags & 0x80) {
                                owner = fux->owner;
                                if (owner == 0 || (OVL64_GAME->teams != 0 && owner == 1)) {
                                    Heroes[owner].hero_health = 0;
                                    func_80084E04(fux->owner, NULL, -1, 1, 8);
                                }
                            }
                        }
                    }
                    func_8007A524(D_800BB89C[fux->state], &eng->unk10.x, (s32)D_80052FCC);
                sfx:
                    PlaySfx(D_800BB8E8[kind - 8][Random(4)], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    o = fux->owner;
                    if (o != -1 && o < 5 && Heroes[o].hero_object == (EXTobject *)fux) {
                        Heroes[o].hero_object = NULL;
                        if (Heroes[fux->owner].hero_eng->sfx != NULL) {
                            StopSfx(Heroes[fux->owner].hero_eng->sfx);
                        }
                    }
                    goto kill;
                case 12:
                    func_800B697C(fux);
                    o = fux->owner;
                    if (o != -1 && o < 5 && Heroes[o].hero_object == (EXTobject *)fux) {
                        Heroes[o].hero_object = NULL;
                        if (Heroes[fux->owner].hero_eng->sfx != NULL) {
                            StopSfx(Heroes[fux->owner].hero_eng->sfx);
                        }
                    }
                    func_8007A524(4, &eng->unk10.x, (s32)D_80052FCC);
                    goto kill;
                }
            case 3:
                fux->sub = 0;
                break;
            case 0x63:
                if (GravitateNas(nas) != 0) {
                    goto kill;
                }
                s = eng->unk2C.x;
                s -= 0x100;
                if (s < 0x101) {
                kill:
                    func_800270B0(nas);
                    return;
                }
                eng->unk2C.x = s;
                eng->unk2C.y = s;
                eng->unk2C.z = s;
                eng->sub->unk76 = s;
                eng->unk5E = 1;
                break;
            default:
                OVL64_CHECK_POP(fux);
                break;
            }
            if (D_8005A644 & 0x801) {
                fux->owner = -1;
                fux->hit |= 8;
            }
            a = get_angle(-eng->unk10.x, -eng->unk10.z) + D_800BC018;
            mot->Zvel = 4;
            a &= 0xFFF;
            mot->Za = a;
            eng->unk10.x -= (mot->Xvel * D_80068BD4[(mot->Xa - 0x400) & 0xFFF].unk2) >> 12;
            eng->unk10.z -= (mot->Xvel * D_80068BD4[mot->Xa & 0xFFF].unk2) >> 12;
            OVL64_BOUNCE(eng, mot);
            func_800B6EC0(eng, mot, fux);
            eng->unk10.x -= (mot->Zvel * D_80068BD4[(mot->Za - 0x400) & 0xFFF].unk2) >> 12;
            eng->unk10.z -= (mot->Zvel * D_80068BD4[mot->Za & 0xFFF].unk2) >> 12;
        }
        if ((u16)fux->state < 4) {
            t = D_800BBD64;
            if (t != 0) {
                eng->sub->unk74 = D_800BB894[D_800BBD62];
                if (t < 0x1E) {
                    if (t & 2) {
                        eng->sub->unk74 = D_800BB894[fux->state];
                        if (D_800BBD68 == 0) {
                            PlaySfx(0x5EF, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        D_800BBD68 = 1;
                    } else {
                        D_800BBD68 = 0;
                    }
                }
            } else {
                eng->sub->unk74 = D_800BB894[fux->state];
            }
        }
        a = fux->angle + D_8004E0D8 * 16;
        eng->sub->unk08 = (D_80068BD4[(a * 4 - 0x400) & 0xFFF].unk2 + D_80068BD4[a & 0xFFF].unk2) >> 6;
        OVL64_FLOAT(eng, g, y, s);
    }
    OVL64_SHADOW(eng, shadow);
}

void func_800B697C(Ovl64Fux *arg0) {
    NAS *n;
    Ovl64Fux *f;
    s16 idx;
    u32 a;
    u32 b;
    u16 w0;
    u16 w1;
    u16 w2;
    u16 t;
    u32 r;
    s32 k;
    u16 *p;

    n = D_8005AA24;
    idx = arg0->owner;
    if (idx != -1) {
        a = D_8009D87C[idx][0];
        p = D_800BB848;
        b = 0x20 - a;
        a = p[a];
        b = p[b];
        if (D_8005A614[idx + 0x5C] != 0) {
            w0 = a;
            w1 = a >> 1;
            w2 = b;
        } else {
            w0 = b;
            w1 = b >> 1;
            w2 = a;
        }
        w2++;
        t = w2 + ((w0 = w0 + 1) + (w1 = w1 + 1));
        if (t == 0) {
            return;
        }
        r = (u16)Random(t);
        arg0->state = 4;
        if ((u16)w0 < r) {
            arg0->state = 5;
        }
        if ((u16)w0 + (u16)w1 < (s32)r) {
            arg0->state = 6;
        }
    }
    switch (arg0->state) {
    case 5:
        PlaySfx(0x5ED, NULL, NULL, 0x1000, 0x1E00, 0);
        for (; n != NULL; n = n->Object.Child) {
            f = (Ovl64Fux *)n->FUX;
            if (f != NULL && (k = f->kind) < 12 && k >= 8 && f->state != arg0->owner) {
                f->owner = -1;
                f->hit |= 8;
            }
        }
        PlaySfx(0x5F1, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    case 6:
        PlaySfx(0x5ED, NULL, NULL, 0x1000, 0x1E00, 0);
        if (D_800BBD6C.sfx != NULL) {
            StopSfx(D_800BBD6C.sfx);
        }
        PlaySfx(0x5EC, &D_800BBD6C.sfx, NULL, 0x1000, 0x1E00, 0);
        PlaySfx(0x5F0, NULL, NULL, 0x1000, 0x1E00, 0);
        D_800BC01C = ~D_800BC01C + 1;
        break;
    default:
        PlaySfx(0x5ED, NULL, NULL, 0x1000, 0x1E00, 0);
        PlaySfx(0x5EE, NULL, NULL, 0x1000, 0x1E00, 0);
        D_800BBD64 = 0x96;
        D_800BBD62 = arg0->owner;
        break;
    }
}

void func_800B6C50(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *pu;
    subengine *sub;
    VECTOR pos;
    s32 y;
    s32 top;

    base = Heroes;
    h = base + arg0;
    if (h->hero_pu_timer != 0) {
        h->hero_pu_timer--;
    }
    if (h->hero_pu_lock != 0) {
        h->hero_pu_lock--;
    }
    if (h->hero_pu != 0) {
        pu = h->hero_powerup;
        sub = pu->sub;
        if (func_80015780(h->hero_eng, h->hero_sub, 0x80, &pos) != NULL) {
            top = pos.vy - 0x40;
            y = top - h->hero_eng->unk10.y;
        } else {
            switch (h->hero_name) {
            case 1:
            case 2:
                y = -0x180;
                break;
            case 3:
            case 4:
                y = -0x200;
                break;
            case 5:
            case 6:
            case 7:
            default:
                y = -0x280;
                break;
            }
        }
        pu->unk10 = Heroes[arg0].hero_eng->unk10;
        sub->unk08 = y;
        if (Heroes[arg0].hero_pu != 0x13) {
            if (Heroes[arg0].hero_pu == 0x17 && D_800BBD58 > 30) {
                D_800BBD58 = 30;
            }
        }
        if (Heroes[arg0].hero_weapon_count == 0) {
            Heroes[arg0].hero_status &= 0xFFEF;
        }
        if (Heroes[arg0].hero_pu_timer != 0) {
            sub->flags |= 0x8000;
        } else {
            Heroes[arg0].hero_pu = 0;
            sub->flags &= ~0x8000;
            if (pu->sfx != NULL) {
                StopSfx(pu->sfx);
            }
        }
        pu->unk10 = Heroes[arg0].hero_eng->unk10;
        pu->unk5E = 1;
        pu->unk1E += 0xAA;
    }
}

s32 func_800B6EC0(EXTengine *eng, EXTmotion *mot, Ovl64Fux *fux) {
    FlagRecord *box;
    HeroInfo *h;
    NAS *it;
    EXTengine *other;
    Ovl64Fux *f;
    s32 hit;
    s32 i;
    s32 bot;
    s32 top;
    s32 dx;
    s32 dz;
    s32 d;
    s32 ang;
    s32 dy;
    s32 r;
    s32 t;
    s32 lo;
    s32 kind;

    hit = 0;
    box = func_80025A48(eng, 0x40);
    if (box != NULL) {
        i = 0;
        top = eng->unk10.y + box->unk2;
        bot = eng->unk10.y + box->height;
        do {
            h = &Heroes[i];
            if (!(h->hero_status & 0xF800) && h->hero_pu == 0x17) {
                if (D_800BBD64 != 0 && D_800BBD62 == i) {
                    if (D_800BBD64 < 0x1E) {
                        goto next;
                    }
                } else {
                    do {
                        do {
                            if (fux->state != i) {
                                goto next;
                            }
                        } while (0);
                    } while (0);
                }
                other = h->hero_eng;
                dx = other->unk10.x - eng->unk10.x;
                dz = other->unk10.z - eng->unk10.z;
                d = dx * dx + dz * dz;
                if (d <= 0x01E3FFFF) {
                    r = csqrt(d);
                    ang = get_angle(dx, dz) & 0xFFF;
                    d = r >> 9;
                    eng->unk10.x += (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                    eng->unk10.z += (d * D_80068BD4[ang].unk2) >> 12;
                    if (d < 0x280) {
                        t = eng->unk10.y - 0x200;
                        dy = t - h->hero_powerup->unk10.y;
                        if (abs(dy) < 0x10) {
                            eng->unk10.y += dy >> 3;
                        }
                    }
                }
            }
        next:
            i++;
        } while (i < 4);
        it = D_8005AA2C;
        while (it != NULL) {
            if (it->Class & 0x200) {
                other = it->Engine;
                if (other != NULL && other != eng) {
                    f = (Ovl64Fux *)it->FUX;
                    if (f != NULL) {
                        kind = f->kind;
                        if (kind < 0xD) if (kind >= 8) {
                            d = other->unk10.y;
                            lo = d - 0x100;
                            if (bot < d && lo < top) {
                                dx = other->unk10.x - eng->unk10.x;
                                dz = other->unk10.z - eng->unk10.z;
                                d = csqrt(dx * dx + dz * dz) >> 6;
                                if (d < 0x100) {
                                    ang = get_angle(dx, dz) & 0xFFF;
                                    d = 0xFF - d;
                                    eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                    hit = 1;
                                    eng->unk10.z -= (d * D_80068BD4[ang].unk2) >> 12;
                                } else if (d < 0x400) {
                                    ang = get_angle(dx, dz) & 0xFFF;
                                    mot->Zvel = (0x400 - d) >> 6;
                                    mot->Za = ang;
                                }
                            }
                        }
                    }
                }
            }
            it = it->Solid.Child;
        }
    }
    return hit;
}

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

void func_800B8294(s32 arg0) {
    EXTengine *eng;
    EXTheromove *mv;
    s32 rx;
    s32 rz;
    s32 lx;
    s32 lz;
    s32 dx;
    s32 dz;
    s32 ang;
    s32 c;
    s32 s;
    s32 gx;
    s32 gz;
    s32 id;
    s32 i;
    u8 v;

    eng = Heroes[arg0].hero_eng;
    rx = eng->unk10.x;
    rz = eng->unk10.z;
    mv = Heroes[arg0].hero_move;
    dx = D_80068BD4[mv->forward_angle & 0xFFF].unk2 * 15 >> 9;
    ang = mv->forward_angle;
    c = D_80068BD4[(ang + 0x400) & 0xFFF].unk2;
    s = D_80068BD4[(ang + 0x800) & 0xFFF].unk2;
    dz = c * 15 >> 9;
    lx = rx + dx;
    rx = rx - dx;
    lz = rz + dz;
    rz = rz - dz;
    dx = c * 11 >> 7;
    dz = s * 11 >> 7;
    lx += dx;
    lz += dz;
    rx += dx;
    rz += dz;
    dx = c * 256 >> 12;
    dz = s * 256 >> 12;
    id = (arg0 << 5) | 0x80;
    i = 0;
    while ((u32)(lx + 0x7FF) < 0xFFF && lz < 0x800 && lz >= -0x7FF && i < 8) {
        gx = (lx >> 8) + 10;
        gz = 9 - (lz >> 8);
        v = D_8009D90C[gz * 20 + gx].unk0;
        if ((v & 0xE0) == id) {
            v &= 0x1F;
        }
        lx += dx;
        lz += dz;
        Heroes[arg0].hero_ai->AIforward_left[i] = v;
        i++;
    }
    for (; i < 8; i++) {
        Heroes[arg0].hero_ai->AIforward_left[i] = 0xFE;
    }
    i = 0;
    while ((u32)(rx + 0x7FF) < 0xFFF && rz < 0x800 && rz >= -0x7FF && i < 8) {
        gx = (rx >> 8) + 10;
        gz = 9 - (rz >> 8);
        v = D_8009D90C[gz * 20 + gx].unk0;
        if ((v & 0xE0) == id) {
            v &= 0x1F;
        }
        rx += dx;
        rz += dz;
        Heroes[arg0].hero_ai->AIforward_right[i] = v;
        i++;
    }
    for (; i < 8; i++) {
        Heroes[arg0].hero_ai->AIforward_right[i] = 0xFE;
    }
}


s32 func_800B85A4(s32 player) {
    NAS *bestNas;
    s32 bestHeroScore;
    s32 r;
    s32 bestHero;
    s32 px;
    s32 pz;
    s32 mate;
    s16 heroW;
    EXTbotai *ai;
    EXTengine *e;
    EXTengine *he;
    NAS *n;
    Ovl64Fux *f;
    s32 lvl;
    s32 inv;
    s16 w0;
    s16 w1;
    s16 w2;
    s32 dx;
    s32 dz;
    s32 d;
    s32 d2;
    s32 cnt;
    s32 bestNasScore;
    s32 i;
    s32 k;

    n = D_8005AA24;
    bestNas = NULL;
    bestHero = -1;
    bestHeroScore = 0;
    lvl = D_8009D87C[player][0];
    inv = 0x21 - lvl;
    mate = Heroes[player].hero_teammate;
    he = Heroes[player].hero_eng;
    ai = Heroes[player].hero_ai;
    px = he->unk10.x;
    pz = he->unk10.z;
    Random(0x100);
    bestNasScore = 0;
    w1 = 0x1E;
    w2 = w1;
    r = Random(0x1000);
    w0 = (lvl + 0x14) * 4;
    heroW = lvl * 8;
    if (D_800BBD64 != 0 && D_800BBD62 != player) {
        w0 = 0;
        heroW = lvl << 5;
        w1 = lvl * 0x10;
    }
    if (Heroes[player].hero_pu != 0) {
        if (Heroes[player].hero_pu == 0x17) {
            if (D_800BBD64 != 0) {
                if (D_800BBD62 == player) {
                    w0 = lvl << 5;
                    w1 = inv * 2;
                    w2 = inv * 4;
                } else {
                    heroW = lvl * 8;
                    w0 = inv * 4;
                    w1 = inv * 2;
                    w2 = lvl << 5;
                }
            } else {
                w0 = lvl << 5;
                w1 = inv * 4;
                w2 = inv * 2;
            }
        } else if (D_800BBD64 != 0) {
            if (D_800BBD62 == player) {
                w0 = lvl << 5;
                w1 = inv * 4;
                w2 = lvl << 6;
            } else {
                heroW = lvl * 0x10;
                d2 = inv * 4;
                w0 = d2;
                w1 = d2;
                w2 = lvl << 5;
            }
        } else {
            w0 = lvl << 5;
            d2 = inv * 8;
            w1 = d2;
            w2 = d2;
        }
    }
    for (; n != NULL; n = n->Object.Child) {
        e = n->Engine;
        if (e == NULL) continue;
        f = (Ovl64Fux *)n->FUX;
        if (f == NULL) continue;
        switch (f->kind) {
        case 8: case 9: case 10: case 11:
            if (D_800BBD64 != 0 && D_800BBD62 != player) continue;
            if (f->state != player) continue;
            dx = px - e->unk10.x;
            dz = pz - e->unk10.z;
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * (s16)w0;
            if (bestNasScore < d) {
                bestNas = n;
                bestNasScore = d;
            }
            continue;
        case 2:
            dx = px - e->unk10.x;
            dz = pz - e->unk10.z;
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * (s16)w1;
            cnt = 0;
            for (i = 0; i < 4; i++) {
                if (ai->pTargetNas == n) cnt++;
            }
            if (cnt != 0) d >>= cnt;
            if (bestNasScore < d) {
                bestNas = n;
                bestNasScore = d;
            }
            continue;
        case 12:
            dx = px - e->unk10.x;
            dz = pz - e->unk10.z;
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * (s16)w2;
            cnt = 0;
            for (i = 0; i < 4; i++) {
                if (ai->pTargetNas == n) cnt++;
            }
            break;
        default:
            continue;
        }
        if (cnt != 0) d >>= cnt;
        if (bestNasScore < d) {
            bestNas = n;
            bestNasScore = d;
        }
    }
    for (i = 0; i < 4; i++) {
        if (i == player) continue;
        if (((Ovl64Game *)D_8005A614)->teams != 0 && i == mate) continue;
        if (Heroes[i].hero_status & 0xFA00) continue;
        dx = px - Heroes[i].hero_eng->unk10.x;
        dz = pz - Heroes[i].hero_eng->unk10.z;
        if (Heroes[i].hero_mode == 2 || Heroes[i].hero_pu != 0) {
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * heroW;
            cnt = 0;
            for (k = 0; k < 4; k++) {
                if (ai->TargetPlayer == k) cnt += 4;
            }
            if (cnt != 0) d >>= cnt;
            if (bestHeroScore < d) {
                bestHero = i;
                bestHeroScore = d;
            }
        }
    }
    if (bestHeroScore < bestNasScore) {
        ai->pTargetNas = bestNas;
        ai->TargetPlayer = -1;
        return 1;
    }
    if (bestHeroScore != 0) {
        ai->TargetPlayer = bestHero;
        ai->pTargetNas = NULL;
        return 1;
    }
    return 0;
}

s32 func_800B8B5C(s32 arg0) {
    s32 other;
    s32 sum;
    s32 i;
    s32 r;

    other = -1;
    if (D_8005A680 != 0) {
        other = arg0 ^ 1;
    }
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != other && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i][2];
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != other && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i][2];
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

s32 func_800B8C74(s32 arg0) {
    s32 flip = arg0 ^ 1;
    s32 best = 0x7FFFFF;
    u16 i = 0;
    u8 *g = D_8005A614;
    HeroInfo *base = Heroes;
    HeroInfo *me = base + arg0;
    HeroInfo *h;
    s32 dx;
    s32 dz;
    s32 d;
    EXTengine *a;
    EXTengine *b;

    for (; i < 4; i++) {
        if (i == arg0) {
            continue;
        }
        if (*(volatile s32 *)(g + 0x6C) != 0 && i == flip) {
            continue;
        }
        h = (HeroInfo *)(i * sizeof(HeroInfo) + (s32)base);
        if (h->hero_status & 0xFA00) {
            continue;
        }
        a = me->hero_eng;
        b = h->hero_eng;
        dx = a->unk10.x - b->unk10.x;
        dz = a->unk10.z - b->unk10.z;
        d = dx * dx + dz * dz;
        if (d < best) {
            best = d;
        }
    }
    return best != 0x7FFFFF ? csqrt(best) >> 6 : 0;
}

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

void func_800B8E64(u16 arg0, u16 arg1) {
    s32 i;
    Ovl64Pair *p;

    for (i = 0; i < arg0; i++) {
        p = func_800B4F7C(&D_800BBD5E);
        if (p != NULL) {
            func_800B51A0(p->a, p->b, arg1, -1);
        }
    }
}

void func_800B8EE0(void) {
    s32 key;
    s32 i;
    s32 r;

    key = 0x1801;
    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == key) {
            r = i;
            goto found;
        }
    }
    r = 0;
found:
    D_800BC048 = r;
    D_8009E488[2] = 0;
    D_8009E488[3] = 0;
}

s32 func_800B8F48(EXTengine *eng) {
    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;
    s32 limit;
    s32 ret = 0;
    s32 ang;

    if (eng->collision_box.flags & 0x40) {
        limit = 0x980 - eng->collision_box.radius;
    } else {
        limit = 0x980;
    }
    if (x * x + z * z >= limit * limit) {
        ang = get_angle(x, z);
        eng->unk10.x = limit * D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> 12;
        eng->unk10.z = limit * D_80068BD4[ang & 0xFFF].unk2 >> 12;
        ret = 1;
    }
    return ret;
}

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

void func_800B90D0(s32 arg0) {
    HeroInfo *h;
    EXTheromove *m;
    PadStick *pad;
    HeroInfo *hb;
    HeroInfo *hb2;
    HeroInfo *h2;
    PadStick *pb;
    u16 *row;
    s32 speed;
    u16 rw;

    hb = Heroes;
    h = hb + arg0;
    m = h->hero_move;
    m->forward_speed = 0;
    speed = (D_8009D6F4 + h->hero_name * 3)[1] + (D_8009D87C[arg0][0] >> 2);
    (D_8009D6F4 + h->hero_name * 3)[2] = speed;
    if (h->hero_frozen != 0) {
        ((PadStick *)D_80051338)[arg0].dirHeld2 = 0;
        ((PadStick *)D_80051338)[arg0].dirPressed = 0;
    }
    pb = (PadStick *)D_80051338;
    pad = pb + arg0;
    if ((pad->flags & 1) || h->hero_ai->AImode != 0) {
        if (pad->dirHeld2 & 0xF0) {
            m->forward_angle = *(u16 *)&pad->angle + 0x800;
            if (m->velocity_enable != 0) {
                rw = m->run_walk;
                if (h->hero_object != NULL) {
                    if (rw != 0) {
                        rw--;
                    }
                }
                row = D_8009D6F4;
                hb2 = Heroes;
                h2 = hb2 + arg0;
                speed = (row + h2->hero_name * 3)[rw];
                m->forward_speed = speed;
            }
        } else {
            if (pad->dirHeld2 & 0x100) {
                m->forward_angle -= 0x100;
            }
            if (pad->dirHeld2 & 0x200) {
                m->forward_angle += 0x100;
            }
            m->x_max = 0;
            m->z_max = 0;
            m->facing_angle = m->forward_angle = m->forward_angle & 0xFFF;
        }
    }
    speed = m->forward_angle & 0xFFF;
    m->forward_angle = speed;
    m->facing_angle = speed;
}

void func_800B92BC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    D_800BC05C[arg0] = 0;
    base = Heroes;
    h = base + arg0;
    h->hero_hit = 0x1E;
    h->hero_pu_timer = 0;
    h->hero_move->velocity_enable = 0;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq((HeroInfo *)arg0, 0x2B);
}

void func_800B937C(s32 arg0) {
    HeroInfo *h;
    HeroInfo *h2;
    u16 *flag;
    u16 *fbase;
    s32 r;
    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_move->reactive_speed != 0) {
        D_8009D73C();
    }
    r = func_8008502C(arg0);
    if (r != 8) {
        if (r == 9) {
            PlaySfx(D_800BC054[arg0], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        }
    } else {
        D_800BC05C[arg0] = 1;
        if (h->hero_health <= 0 && !(D_8005A618 & 2)) {
            h->hero_immunity = 0;
            h->hero_hit += 0x28;
            SetHeroSeq((HeroInfo *)arg0, 0x2D);
        }
        Heroes[arg0].hero_health = 0x14;
    }
    fbase = D_800BC05C;
    flag = fbase + arg0;
    if (*flag != 0) {
        HeroInfo *base2 = Heroes;

        h2 = base2 + arg0;
        if (abs(((s32 (*)(EXTengine *))func_80025F44)(h2->hero_eng)) < 0x20) {
            *flag = 0;
            switch (h2->hero_name) {
            case 1:
                PlaySfx(0x11, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x43, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x75, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 4:
                PlaySfx(0xA7, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 7:
                PlaySfx(0x10B, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 8:
                PlaySfx(0xD9, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 5:
                PlaySfx(0x13D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 6:
                PlaySfx(0x16F, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            }
        }
    }
    if (D_8009D740 != NULL) {
        D_8009D740(arg0);
    }
    func_800871E4(arg0);
}

void func_800B9608(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *p;
    s32 idx;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    SetHeroSeq((HeroInfo *)arg0, 0x2C);
    idx = 0;
    if (D_8005A614[arg0 + 0x5C] == 0) {
        idx = h->hero_name;
    }
    h->hero_move->jump_vel = D_8009D6E8.unk10[idx].vel;
    h->hero_move->jump_max = D_8009D6E8.unk10[idx].max;
    h->hero_move->jump_acc = D_8009D6E8.unk10[idx].acc;
    h->hero_move->jump_dec = D_8009D6E8.unk10[idx].dec;
    h->hero_move->jump_dir = -1;
}

void func_800B971C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *eng;

    if (func_80025BBC(h->hero_eng) != 0) {
        SetHeroAction((HeroInfo *)arg0, 7);
    } else {
        eng = h->hero_eng;
        if (World_floor_level < eng->unk10.y + h->hero_sub->unk08) {
            if (D_8005B5B8 == 0x7FFFFFFF) {
                SetHeroAction((HeroInfo *)arg0, 6);
            } else {
                eng->unk10.y = World_floor_level;
                h->hero_sub->unk08 = 0;
            }
        }
    }
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

void func_800B9804(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base = Heroes;

    h = base + arg0;
    h->hero_hit = 0;
    h->hero_immunity = 0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 2;
    h->hero_health = 0;
    h->hero_status |= 0x2000;
    h->hero_powerup->flags = 0;
    h->hero_fx->flags = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    func_800898EC(arg0);
}

void func_800B98B8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *base2;
    HeroInfo *h2;
    u16 st;
    s32 ret;

    base = Heroes;
    h = &base[arg0];
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    st = h->hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x36);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x37);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret != 0) {
        base2 = Heroes;
        h2 = &base2[arg0];
        if ((u32)(h2->hero_name - 1) < 8) {
            h2->hero_sub->unk6C = D_80052A54 + D_8009D6EC[*(volatile u16 *)&h2->hero_name] * 0x38;
        } else {
            h2->hero_sub->unk6C = 0;
        }
    } else {
        HeroGhost(arg0);
    }
}

void func_800B9A04(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    HeroInfo *b2;
    s32 dy;

    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 1;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->unk6C = D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    {
        HeroInfo *h2;
        b2 = Heroes;
        h2 = b2 + arg0;
        h2->hero_sub->unk08 = 0;
        h2->hero_pu = 0;
        h2->hero_pu_timer = 0;
    }
    func_80085718(arg0, 0);
    SetHeroSeq((HeroInfo *)arg0, 0xC);
    func_80089FA4(arg0);
    D_800AD12C = 1;
}

void func_800B9BF8(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

void func_800B9CE0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    HeroInfo *base2;
    HeroInfo *h2;
    s32 dy;

    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 8;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->unk6C = (s32)D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    base2 = Heroes;
    h2 = base2 + arg0;
    h2->hero_eng->unk5C = 1;
    h2->hero_eng->unk2C.x = 0x1000;
    h2->hero_eng->unk2C.y = 0x1000;
    h2->hero_eng->unk2C.z = 0x1000;
    h2->hero_sub->unk08 = 0;
    h2->hero_pu = 0;
    h2->hero_pu_timer = 0;
    SetHeroSeq(arg0, 1);
    func_80026980(h2->hero);
}

void func_800B9EEC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base = Heroes;
    s32 dy;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    func_8008502C(arg0);
    {
        HeroInfo *b2 = Heroes;
        h = b2 + arg0;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    if (D_8005A644 & 0x800) {
        EXTengine *e = h->hero_eng;
        s32 v = e->unk2C.x;
        v -= 0x60;
        if (v > 0x300) {
            e->unk2C.x = v;
            h->hero_eng->unk2C.y = v;
            h->hero_eng->unk2C.z = v;
        } else {
            func_8007A524(D_800BB89C[arg0], &e->unk10.x, (s32)&D_80052FCC);
            h->hero_status |= 0x8000;
        }
    }
}

/* The D_8009D6E8.unk4C hook: claim the first object in reach of player's
 * cylinder that can take mask (within cone of the hero's heading, 0: any).
 * Each arm claims in its own do-while (0); jump2 merges the copies. */
NAS *func_800BA098(player, mask, unused, cone)
    s32 player;
    s32 mask;
    s32 unused;
    u16 cone;
{
    VECTOR pos;
    s32 top;
    s32 bot;
    HeroInfo *heroes;
    HeroInfo *h;
    EXTengine *me;
    EXTengine *eng;
    FlagRecord *rec;
    FlagRecord *rec2;
    EXTobject *obj;
    NAS *nas;
    NAS *self;
    s32 rad;
    s32 lo;
    s32 hi;
    s32 dx;
    s32 dz;
    s32 r;

    heroes = Heroes;
    h = &heroes[player];
    me = h->hero_eng;
    if (me == NULL) {
        return NULL;
    }
    rec = func_80025A48(me, 0x400);
    if (rec == NULL) {
        return NULL;
    }
    if (me->unk5E != 0) {
        func_800142D4(&me->unk1C);
    }
    func_8001CD04(me->sub, me->unk3C, &me->unk10);
    bot = me->unk10.y + rec->unk2;
    top = me->unk10.y + rec->height;
    rad = rec->radius;
    func_80015758((SVECTOR *)rec, &pos);
    nas = D_8005AA2C;
    self = NULL;
    if (h->hero_object != NULL) {
        self = ((NAS **)h->hero_object)[-1];
    }
    for (; nas != NULL; nas = nas->Solid.Child) {
        if (!(nas->Class & 0x200) || nas == self) {
            continue;
        }
        eng = nas->Engine;
        if (eng == NULL || eng == me) {
            continue;
        }
        rec2 = func_80025A48(eng, 0x40);
        if (rec2 == NULL) {
            continue;
        }
        lo = eng->unk10.y + rec2->unk2;
        hi = eng->unk10.y + rec2->height;
        if (top < lo && hi < bot) {
            dx = eng->unk10.x - pos.vx;
            dz = eng->unk10.z - pos.vz;
            r = rec2->radius + rad;
            if (dx * dx + dz * dz < r * r) {
                obj = FindExtension(nas, 0x1000);
                if (obj != NULL && (obj->can_do & mask)) {
                    if (cone == 0) {
                        do {
                            obj->player = player;
                            obj->will_do |= mask;
                            return nas;
                        } while (0);
                    } else if (func_80015480(Heroes[player].hero_move->forward_angle,
                                             get_angle(eng->unk10.x - me->unk10.x, eng->unk10.z - me->unk10.z) ^ 0x800) < cone) {
                        do {
                            obj->player = player;
                            obj->will_do |= mask;
                            return nas;
                        } while (0);
                    }
                }
            }
        }
    }
    return NULL;
}

NAS *func_800BA344(s32 idx, s32 mask) {
    NAS *n;
    EXTengine *he;
    EXTengine *e;
    EXT *fux;
    HeroInfo *h;
    HeroInfo *hb;
    s32 hx, hz, dx, dz, m, lim;
    u16 ang;
    u16 um;

    um = (u16)mask;
    lim = 0x10000;
    hb = Heroes;
    n = D_8005AA24;
    h = &hb[idx];
    he = h->hero_eng;
    ang = Heroes[idx].hero_move->facing_angle;
    hx = he->unk10.x - (D_80068BD4[(ang - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    hz = he->unk10.z - (D_80068BD4[ang & 0xFFF].unk2 * 1024 >> 14);
    for (; n != NULL; n = n->Object.Child) {
        fux = n->FUX;
        if (fux == NULL) continue;
        if (!(*(u16 *)((u8 *)fux + 0x1C) & 1)) continue;
        e = n->Engine;
        if (e == NULL) continue;
        if (*(s32 *)((u8 *)e + 0x80) != 0) continue;
        dx = e->unk10.x - hx;
        dz = e->unk10.z - hz;
        if (dx * dx + dz * dz >= lim) continue;
        if (um != 0 && func_80015480(Heroes[idx].hero_move->forward_angle,
                get_angle(e->unk10.x - he->unk10.x, e->unk10.z - he->unk10.z) ^ 0x800) >= um) continue;
        do {
            *(s16 *)((u8 *)fux + 0x16) = idx;
            *(u16 *)((u8 *)fux + 0x1E) |= 1;
            return n;
        } while (0);
    }
    return 0;
}

s32 func_800BA514(s32 arg0) {
    TrigEntry *tbl;
    VECTOR pos;
    NAS *own;
    EXTengine *obj;
    s32 hit;
    s32 top;
    s32 push;
    s32 rad;
    s32 lo;
    s32 hi;
    s32 rad2;
    s32 hi2;
    s32 lo2;
    s32 rad3;
    s32 lo3;
    s32 hi3;
    u32 i;
    HeroInfo *self;
    HeroInfo *other;
    EXTengine *eng;
    EXTengine *e;
    FlagRecord *rec;
    FlagRecord *box;
    NAS *n;
    Ovl64HitFlags *t;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
    s32 rr;
    s32 ang;
    s32 a;
    s32 tmp;

    hit = 0;
    lo2 = 0;
    hi2 = 0;
    rad2 = 0;
    hi3 = 0;
    lo3 = 0;
    rad3 = 0;
    eng = Heroes[arg0].hero_eng;
    obj = NULL;
    if (eng != NULL) {
        box = func_80025A48(eng, 0x40);
        if (box != NULL) {
            rad = box->radius;
            hi = eng->unk10.y + box->unk2;
            lo = eng->unk10.y + box->height;
            box = func_80025A48(eng, 1);
            if (box != NULL) {
                if (eng->unk5E != 0) {
                    func_800142D4(&eng->unk1C);
                }
                func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
                lo2 = eng->unk10.y + box->unk2;
                hi2 = eng->unk10.y + box->height;
                if (lo2 < hi2) {
                    tmp = hi2;
                    hi2 = lo2;
                    lo2 = tmp;
                }
                rad2 = box->radius;
                func_80015758((SVECTOR *)box, &pos);
            }
            if (Heroes[arg0].hero_object != NULL) {
                n = ((NAS **)Heroes[arg0].hero_object)[-1];
                obj = n->Engine;
                if (obj != NULL) {
                    rad3 = 0x80;
                    hi3 = obj->unk10.y;
                    lo3 = hi3 - 0x100;
                }
            }
            other = Heroes;
            i = 0;
            self = &Heroes[arg0];
            do {
                tbl = D_80068BD4;
                if (arg0 != i && !(other[i].hero_status & 0xF800)) {
                    if (self->hero_move->forward_speed != 0) {
                        push = 1;
                    } else {
                        push = other[i].hero_move->forward_speed == 0;
                    }
                    if (D_8005A5DC < 4) {
                        if (D_8005A5DC >= 2) {
                            push = 1;
                        }
                    }
                    e = other[i].hero_eng;
                    if (e != NULL) {
                        rec = func_80025A48(e, 0x40);
                        if (rec != NULL) {
                            bot = e->unk10.y + rec->unk2;
                            top = e->unk10.y + rec->height;
                            if (push != 0) {
                                r = rec->radius;
                                r += rad;
                                rr = r * r;
                                if (lo < bot && top < hi) {
                                    dx = e->unk10.x - eng->unk10.x;
                                    dz = e->unk10.z - eng->unk10.z;
                                    d = dx * dx + dz * dz;
                                    if (d < rr) {
                                        d = csqrt(d) >> 6;
                                        if (d < r) {
                                            ang = get_angle(dx, dz) & 0xFFF;
                                            d = r - d + 1;
                                            eng->unk10.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->unk10.z -= (d * tbl[ang].unk2) >> 12;
                                            hit = 1;
                                            if (self->hero_pu != 0) {
                                                (*(void (**)(s32, s32))&D_8009D6E8.unk60)(arg0, i);
                                            }
                                            if (other[i].hero_pu != 0) {
                                                (*(void (**)(s32, s32))&D_8009D6E8.unk60)(i, arg0);
                                            }
                                        }
                                    }
                                }
                            }
                            if (rad2 != 0 && hi2 < bot && top < lo2) {
                                dx = e->unk10.x - pos.vx;
                                dz = e->unk10.z - pos.vz;
                                r = rec->radius;
                                r += rad2;
                                d = dx * dx + dz * dz;
                                rr = r * r;
                                if (d < rr) {
                                    d = csqrt(d) >> 6;
                                    if (d < r) {
                                        hit = 1;
                                        ang = get_angle(dx, dz) & 0xFFF;
                                        d = r - d + 1;
                                        if (push != 0) {
                                            eng->unk10.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->unk10.z -= (d * tbl[ang].unk2) >> 12;
                                        } else {
                                            e->unk10.x += (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            e->unk10.z += (d * tbl[ang].unk2) >> 12;
                                        }
                                    }
                                }
                            }
                            if (rad3 != 0 && lo3 < bot && top < hi3) {
                                dx = e->unk10.x - obj->unk10.x;
                                dz = e->unk10.z - obj->unk10.z;
                                r = rec->radius;
                                r += rad3;
                                d = dx * dx + dz * dz;
                                rr = r * r;
                                if (d < rr) {
                                    d = csqrt(d) >> 6;
                                    if (d < r) {
                                        hit = 1;
                                        ang = get_angle(dx, dz) & 0xFFF;
                                        d = r - d + 1;
                                        if (push != 0) {
                                            eng->unk10.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->unk10.z -= (d * tbl[ang].unk2) >> 12;
                                        } else {
                                            e->unk10.x += (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            e->unk10.z += (d * tbl[ang].unk2) >> 12;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                i++;
            } while (i < 4);
            own = NULL;
            n = D_8005AA2C;
            if (Heroes[arg0].hero_object != NULL) {
                own = ((NAS **)Heroes[arg0].hero_object)[-1];
            }
            rad = 0x78;
            push = Heroes[arg0].hero_move->forward_speed != 0;
            if (D_8005A5DC < 4) {
                if (D_8005A5DC >= 2) {
                    push = 1;
                }
            }
            for (; n != NULL; n = n->Solid.Child) {
                if (!(n->Class & 0x200)) continue;
                if (n == own) continue;
                e = n->Engine;
                if (e == NULL) continue;
                if (e == eng) continue;
                rec = func_80025A48(e, 0x40);
                if (rec == NULL) continue;
                bot = e->unk10.y + rec->unk2;
                top = e->unk10.y + rec->height;
                if (lo < bot && top < hi) {
                    dx = e->unk10.x - eng->unk10.x;
                    dz = e->unk10.z - eng->unk10.z;
                    r = rec->radius;
                    r += rad;
                    d = dx * dx + dz * dz;
                    rr = r * r;
                    if (d < rr) {
                        d = csqrt(d) >> 6;
                        if (d < r) {
                            hit = 1;
                            a = get_angle(dx, dz);
                            ang = a & 0xFFF;
                            if (!(a & 0xFF)) {
                                ang -= 8 - Random(0x10);
                            }
                            d = r - d + 1;
                            if (push != 0) {
                                eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                eng->unk10.z -= (d * D_80068BD4[ang & 0xFFF].unk2) >> 12;
                            }
                            t = FindExtension(n, 0x1000);
                            if (t != NULL && (t->unk1C & 8)) {
                                t->unk16 = arg0;
                                t->unk1E |= 8;
                            }
                        }
                    }
                }
            }
        }
    }
    return hit;
}

s32 func_800BAF1C(s32 idx) {
    VECTOR v;
    subengine *sub;
    s32 hit;
    s32 top;
    s32 bot;
    EXTengine *eng;
    EXTengine *other;
    FlagRecord *box;
    FlagRecord *rec;
    NAS *nas;
    NAS *self;
    Ovl64HitFlags *ext;
    s32 dx;
    s32 dz;
    s32 r;
    s32 d;
    s32 ang;
    s32 lo;
    s32 hi;

    eng = Heroes[idx].hero_eng;
    hit = 0;
    if (eng != NULL) {
        box = func_80025A48(eng, 0x40);
        if (box != NULL) {
            bot = eng->unk10.y + box->unk2;
            top = eng->unk10.y + box->height;
            box = func_80025A48(eng, 1);
            if (box != NULL) {
                if (eng->unk5E != 0) {
                    func_800142D4(&eng->unk1C);
                    sub = eng->sub;
                    eng->sub = sub;
                    func_8001CD04(sub, eng->unk3C, &eng->unk10);
                } else {
                    sub = eng->sub;
                    eng->sub = sub;
                    func_8001CD04(sub, eng->unk3C, &eng->unk10);
                }
                func_80015758((SVECTOR *)box, &v);
            }
            nas = D_8005AA2C;
            self = NULL;
            if (Heroes[idx].hero_object != NULL) {
                self = ((NAS **)Heroes[idx].hero_object)[-1];
            }
            if (nas != NULL) {
                do {
                    if ((nas->Class & 0x200) && nas != self) {
                        other = nas->Engine;
                        if (other != NULL && other != eng) {
                            ext = FindExtension(nas, 0x1000);
                            if (ext != NULL && (ext->unk1A == 1 || ext->unk1A == 0xB)) {
                                rec = func_80025A48(other, 0x40);
                                if (rec != NULL) {
                                    lo = other->unk10.y + rec->unk2;
                                    hi = other->unk10.y + rec->height;
                                    if (top < lo && hi < bot) {
                                        dx = other->unk10.x - eng->unk10.x;
                                        dz = other->unk10.z - eng->unk10.z;
                                        r = rec->radius;
                                        r += 0x78;
                                        d = dx * dx + dz * dz;
                                        if (d < r * r) {
                                            d = csqrt(d) >> 6;
                                            if (d < r) {
                                                ang = get_angle(dx, dz) & 0xFFF;
                                                d = r - d + 1;
                                                eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                eng->unk10.z -= (d * D_80068BD4[ang].unk2) >> 12;
                                                hit = 1;
                                                if (ext->unk1C & 8) {
                                                    ext->unk16 = idx;
                                                    ext->unk1E |= 8;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    nas = nas->Solid.Child;
                } while (nas != NULL);
            }
        }
    }
    return hit;
}

Ovl64Fux *func_800BB230(s32 hero, u16 mask, s32 r) {
    Ovl64Fux *best = NULL;
    NAS *n = D_8005AA24;
    EXTengine *eng = Heroes[hero].hero_eng;
    s32 cx = eng->unk10.x;
    s32 cz = eng->unk10.z;
    s32 px = cx - (D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    s32 pz = cz - (D_80068BD4[eng->unk1E & 0xFFF].unk2 * 1024 >> 14);
    s32 bestd;
    s32 lim, dx, dz, d;
    Ovl64Fux *f;

    r = r * r;
    bestd = r * 2;

    for (; n != NULL; n = n->Object.Child) {
        f = (Ovl64Fux *)n->FUX;
        if (f != NULL && (f->mask & mask) && n->Engine != NULL) {
            if (f->kind == 2) {
                lim = 0x10000;
                dx = n->Engine->unk10.x - px;
                dz = n->Engine->unk10.z - pz;
            } else {
                lim = r;
                dx = n->Engine->unk10.x - cx;
                dz = n->Engine->unk10.z - cz;
            }
            d = dx * dx + dz * dz;
            if (d < lim && d < bestd) {
                bestd = d;
                best = f;
            }
        }
    }
    return best;
}

void func_800BB37C(EXTengine *eng, Ovl64HitMask *m, u32 flag, s32 hero) {
    FlagRecord *r;
    FlagRecord *r2;
    HeroInfo *h;
    EXTengine *e2;
    s32 top, bot, dx, dz, rr, y, lo, hi;
    s32 rad;

    r = func_80025A48(eng, 0x40);
    if (r != NULL) {
        top = eng->unk10.y + r->unk2;
        rad = r->radius;
        h = &Heroes[hero];
        bot = eng->unk10.y + r->height;
        if (!(h->hero_status & 0xF800)) {
            e2 = h->hero_eng;
            if (e2 != NULL) {
                r2 = func_80025A48(e2, 0x40);
                if (r2 != NULL) {
                    lo = e2->unk10.y + r2->unk2;
                    hi = e2->unk10.y + r2->height;
                    if (bot < lo && hi < top) {
                        dx = e2->unk10.x - eng->unk10.x;
                        dz = e2->unk10.z - eng->unk10.z;
                        rr = r2->radius + rad;
                        rr *= rr;
                        if (dx * dx + dz * dz < rr && m != NULL && (m->mask & flag)) {
                            m->hit |= flag;
                        }
                    }
                }
            }
        }
    }
}

void func_800BB4FC(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
}

void func_800BB52C(s32 arg0) {
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

void func_800BB560(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *shadow = h->hero_shadow;
    EXTengine *eng;

    shadow->sub->unk6C = &D_80052FCC;
    shadow->sub->unk74 = D_800BC04C[arg0];
    eng = h->hero_eng;
    shadow->unk10 = eng->unk10;
    shadow->flags |= 0x8021;
    shadow->sub->unk76 = 0xE00;
}

s32 func_800BB5F0(s32 arg0) {
    if (!(D_80051338[arg0].unk54 & 0x4000)) {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, 0x34);
    return 1;
}

s32 func_800BB648(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x36;
    if (!(st & 0x200)) {
        act = 0x37;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_800BB6B4(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x36;
    if (!(st & 0x200)) {
        act = 0x37;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}
