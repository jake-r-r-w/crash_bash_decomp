#include "common.h"
#include "bash/main.h"
#include "bash/ovl66.h"
#include "bash/bonk.h"
#include "bash/pogo.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

void func_800B33A4() {
    D_8009D6E8.unk4 = D_800B9DF8;
    D_8009D6E8.unk2C = func_800B70C4;
    D_8009D6E8.unk68 = func_800B9054;
    D_8009D6E8.unk20 = func_800B717C;
    D_8009D6E8.unk28 = func_800B8EF0;
    D_8009D6E8.unk54 = func_800B7B48;
    D_8009D6E8.unk3C = func_800B8E64;
    D_8009D6E8.unk48 = D_8008AB48;
    D_8009D6E8.unk44 = D_8008AB04;
    D_8009D6E8.unk5C = func_800B6E1C;
    D_8009D6E8.unk64 = func_800B6238;
    D_8009D6E8.unk38 = func_800B5BA0;
    D_8009E0D8 = 0x42;
    D_8009E0E4 = 0x47;
    D_8009E0E6 = 0x48;
    func_800B67F4();
}

void func_800B347C(void) {
    s32 i;

    D_800B9EF0 = 0;
    D_800B9EFC = 0;
    D_800B9F04 = 0;
    D_800B9F00 = D_800B9E0C;
    for (i = 0; i < 10; i++) {
        D_800B9E50[i].unk0 = 0x100;
        D_800B9E50[i].unk4 = 0;
        D_800B9E50[i].unk8 = 0;
    }
    D_800B9EF4 = 0;
    D_800B9EF8 = D_800B9634 * 30;
}

void func_800B34F8(void) {
    s32 i;
    s32 *new_var;

    if (D_800B9EFC > 0) {
        D_800B9EFC -= 1;
    }
    if (D_800B9F00 > 0 && D_800B9F04 < 4) {
        D_800B9F00 -= 1;
    }
    if (D_800B9EF0 != 0) {
        for (i = 0; i < 10; i++) {
            NAS *n = (NAS *)D_800B9E50[i].unk4;
            if (n != 0 && n->UserInfo.status == 1) {
                KillNAS(n);
                D_800B9E50[i].unk4 = 0;
            }
        }
    }
    if (D_800B9EF4 < 4) {
        if (D_800B9EF8 != 0) {
            D_800B9EF8--;
        } else {
            D_800B9EF4++;
            D_800B9EF8 = (&D_800B9634)[D_800B9EF4] * 30;
        }
    }
    if (D_800B9EF0 < (new_var = &D_800B95A8)[D_800B9EF4] && D_800B9EFC == 0) {
        func_800B66C8();
        D_800B9EFC = 0x14;
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl66/ovl66", D_800B32B8);

void PlotBonkStool(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 x;
    s32 z;
    SLONG_XYZ pos;
    SLONG_XYZ *v;
    s32 *p;
    s32 *q;

    if (e == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        if (func_800B3D28(&x, &z) != 0) {
            D_800B9E50[nas->UserInfo.user2].unk8 = 1000;
            e->unk10.x = x;
            e->unk10.z = z;
            e->unk10.y = D_8005A5D8 - 0x2000;
            func_800B8F64(e);
            nas->UserInfo.status = 2;
            if (nas->UserInfo.user1 == 0) {
                D_800B9E50[nas->UserInfo.user2].unk8 = D_800B9300[D_800B9EEC];
                nas->UserInfo.user1 = Random(0x14) + 10;
            } else {
                D_800B9E50[nas->UserInfo.user2].unk8 = 0;
            }
            e->flags &= 0xFFFF7FFF;
            if (D_800B9F00 == 0) {
                e->sub->unk74 = 0x2015;
                p = &D_800B9E0C;
                D_800B9F00 = p[++D_800B9F04];
            } else {
                e->sub->unk74 = 0x2002;
            }
        }
        break;
    case 2:
        if (!(D_8005A624 & 0x4000000)) {
            nas->UserInfo.user1--;
        }
        if (nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 10;
            e->unk5E = 1;
            e->unk2C.y = 0;
            e->flags |= 0x8000;
            e->unk10.y = D_8005A5D8;
            if (e->sub->unk74 == 0x2015) {
                PlaySfx(0x72A, NULL, &e->unk10, 0x1000, 0x1E00, 0);
            } else {
                PlaySfx(0x528, NULL, &e->unk10, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 10:
        e->unk2C.y += 0x100;
        if (e->unk2C.y > 0xC00) {
            e->unk2C.y = 0xC00;
            nas->UserInfo.status = 12;
            nas->UserInfo.user1 = D_800B95C4[D_800B9EF4] + Random(D_800B95E0[D_800B9EF4]);
            if (e->sub->unk74 == 0x2015) {
                pos = e->unk10;
                pos.y -= 0x100;
                Ovl66_SpawnPuff((s32 *)&pos, 3, D_80053C7C);
            }
        }
        e->unk5E = 1;
        break;
    case 12:
        if (D_800B9E50[nas->UserInfo.user2].unk8 != 0) {
            D_800B9E50[nas->UserInfo.user2].unk8--;
        }
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 13;
            nas->UserInfo.user1 = 70;
        }
        break;
    case 13:
    case 14:
        if (D_800B9E50[nas->UserInfo.user2].unk8 != 0) {
            D_800B9E50[nas->UserInfo.user2].unk8--;
        }
        if (--nas->UserInfo.user1 <= 0) {
            if (nas->UserInfo.status == 13) {
                nas->UserInfo.status = 14;
                nas->UserInfo.user1 = 40;
            } else {
                nas->UserInfo.status = (D_8005A624 & 0x4000000) ? 4 : 5;
            }
            break;
        }
        if (nas->UserInfo.status == 13) {
            e->unk2C.y = Random(0x200) + 0xB00;
        } else {
            e->unk2C.y = Random(0x800) + 0x800;
        }
        e->unk5E = 1;
        break;
    case 5:
        Ovl66_SpawnPuff((s32 *)&e->unk10, 0, D_80053C7C);
        v = &e->unk10;
        func_800B4910((VECTOR *)v, 0x380, -1, 0);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        PlaySfx(D_800B9E20[Random(4)], NULL, v, 0x1000, 0x1E00, 0);
        break;
    case 4:
        Ovl66_SpawnPuff((s32 *)&e->unk10, 1, D_80053C7C);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        break;
    case 6:
        p = &D_800B9EF0;
        q = &D_800B95A8;
        if (*p > q[D_800B9EF4]) {
            nas->UserInfo.status = 1;
            (*p)--;
            e->unk10.y = D_8005A5D8 - 0x2000;
        } else {
            nas->UserInfo.status = 0;
            nas->UserInfo.user1 = 0;
        }
        break;
    }
}

s32 func_800B3D28(s32 *arg0, s32 *arg1) {
    s32 tries;
    s32 ok;
    s32 x;
    s32 z;
    s32 i;
    HeroInfo *h;
    Ovl66Entry *e;
    Ovl66Ent *f;
    EXTengine *eng;

    HeroInfo *heroes;

    tries = 0;
    heroes = Heroes;
    do {
        ok = 1;
        x = Random(0x1000) - 0x800;
        z = Random(0x1000) - 0x800;
        for (i = 0, h = heroes; i < 4; i++, h++) {
            eng = h->hero_eng;
            if ((csqrt(func_800B62C8(eng->unk10.x, eng->unk10.z, x, z)) >> 6) < 0x400) {
                ok = 0;
            }
        }
        if (ok) {
            if (D_800B9EF0 != 0) {
                for (i = 0, e = D_800B9E50; i < 10; i++, e++) {
                    if (e->unk4 != 0) {
                        eng = ((NAS *)e->unk4)->Engine;
                        if ((csqrt(func_800B62C8(eng->unk10.x, eng->unk10.z, x, z)) >> 6) < 0x400) {
                            ok = 0;
                        }
                    }
                }
            }
            if (ok && D_800B9F08 != 0) {
                for (i = 0, f = D_800B9EC8; i < 3; i++, f++) {
                    if (f->b != 0) {
                        eng = ((NAS *)f->b)->Engine;
                        if ((csqrt(func_800B62C8(eng->unk10.x, eng->unk10.z, x, z)) >> 6) < 0x300) {
                            ok = 0;
                        }
                    }
                }
            }
        }
        tries++;
        if (ok) {
            *arg0 = x;
            *arg1 = z;
            return 1;
        }
    } while (tries < 10);
    return 0;
}

void InitBonkCrate(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        subengine *s;

        e->unk5C = 1;
        e->unk2C.z = 0;
        e->unk2C.y = 0;
        e->unk2C.x = 0;
        e->unk5E = 1;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = D_800B9F08 * 0x5A + 0x1E;
        s = e->sub;
        e->flags &= 0xFFFF7FFF;
        if (s != NULL) {
            s->unk6C = D_80053C7C;
            s->unk74 = 0x200F;
        } else {
            nas->Failure = 1;
        }
        if (AddObject(nas) != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
        } else {
            nas->Failure = 1;
        }
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        nas->UserInfo.user2 = D_800B9F08;
        D_800B9EC8[D_800B9F08].b = (s32)nas;
        D_800B9F08 += 1;
    }
}

void PlotBonkCrate(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 x;
    s32 z;
    s32 s;
    SLONG_XYZ *v;

    if (e == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        e->unk10.y = D_8005A5D8 - 0x2000;
        if (nas->UserInfo.user1 == 0) {
            D_800B9EC8[nas->UserInfo.user2].c = D_800B93A0[D_800B9EEC];
            nas->UserInfo.user1 = Random(0x5A) + 0x5A;
        } else {
            D_800B9EC8[nas->UserInfo.user2].c = 0;
        }
        e->flags &= 0xFFFF7FFF;
        e->unk1E = Random(0x1000);
        nas->UserInfo.status = 2;
        break;
    case 2:
        if (!(D_8005A624 & 0x4000000)) {
            if (nas->UserInfo.user1 > 0) {
                nas->UserInfo.user1--;
            } else {
                goto spawn;
            }
        }
        if (nas->UserInfo.user1 > 0) {
            break;
        }
    spawn:
        if (func_800B3D28(&x, &z) != 0) {
            e->unk10.x = x;
            e->unk10.z = z;
            e->unk10.y = D_8005A5D8;
            func_800B8F64(e);
            nas->UserInfo.status = 10;
            e->unk2C.z = 0;
            e->unk2C.x = 0;
            e->unk2C.y = 0;
            e->unk5E = 1;
            e->flags |= 0x8021;
        }
        break;
    case 10:
        e->unk2C.x += 0x100;
        if (e->unk2C.y >= 0x1000) {
            e->unk2C.x = 0x1000;
            nas->UserInfo.status = 12;
            e->flags &= ~0x21;
        }
        s = e->unk2C.x;
        e->unk5E = 1;
        e->unk2C.y = s;
        e->unk2C.z = s;
        break;
    case 12:
        if (D_800B9EC8[nas->UserInfo.user2].c > 0) {
            D_800B9EC8[nas->UserInfo.user2].c--;
        }
        break;
    case 5:
        Ovl66_SpawnPuff((s32 *)&e->unk10, 0, D_80053C7C);
        v = &e->unk10;
        func_800B4910((VECTOR *)v, 0x380, -1, 0);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        PlaySfx(D_800B9E20[Random(4)], NULL, v, 0x1000, 0x1E00, 0);
        break;
    case 4:
        Ovl66_SpawnPuff((s32 *)&e->unk10, 2, D_80053C7C);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        break;
    case 6:
        nas->UserInfo.user1 = 0;
        e->unk10.y = D_8005A5D8 - 0x2000;
        nas->UserInfo.status = 0;
        break;
    }
}

s32 func_800B4454(s32 arg0, s32 *arg1, s32 arg2) {
    Ovl66Entry *found;
    s32 x;
    s32 z;
    s32 ex;
    s32 ez;
    s32 d;
    s32 best;
    s32 i;
    Ovl66Entry *e;
    Ovl66Ent *f;
    EXTengine *eng;

    found = 0;
    if (arg2 & 2) {
        best = D_800B9580[Heroes[arg0].hero_name];
    } else {
        best = 0xFFFFFF;
    }
    x = Heroes[arg0].hero_eng->unk10.x;
    z = Heroes[arg0].hero_eng->unk10.z;
    if ((arg2 & 0x100) && D_800B9EF0 != 0) {
        for (i = 0, e = D_800B9E50; i < 10; i++, e++) {
            if (e->unk4 == 0) {
                continue;
            }
            if (arg2 & 1) {
                if (((Ovl66Pad *)e->unk4)->mode < 9) {
                    continue;
                }
                if (e->unk8 != 0) {
                    continue;
                }
            }
            if (((Ovl66Pad *)e->unk4)->mode < 0xB) {
                continue;
            }
            ex = ((Ovl66Pad *)e->unk4)->eng->unk10.x;
            ez = ((Ovl66Pad *)e->unk4)->eng->unk10.z;
            d = csqrt(func_800B62C8(x, z, ex, ez));
            d >>= 6;
            if (d < best) {
                if (!(arg2 & 4) || func_80015480(get_angle(x - ex, z - ez), Heroes[arg0].hero_move->forward_angle) < 0x320) {
                    do { best = d; found = e; } while (0);
                }
            }
        }
    }
    if (arg2 & 0x200) {
        i = 0;
        if (D_800B9F08 != 0)
        for (f = D_800B9EC8; i < 3; i++, f++) {
            if (f->b == 0) {
                continue;
            }
            if (arg2 & 1) {
                if (((Ovl66Pad *)f->b)->mode < 9) {
                    continue;
                }
                if (f->c != 0) {
                    continue;
                }
            }
            if (((Ovl66Pad *)f->b)->mode < 0xB) {
                continue;
            }
            ex = ((Ovl66Pad *)f->b)->eng->unk10.x;
            ez = ((Ovl66Pad *)f->b)->eng->unk10.z;
            d = csqrt(func_800B62C8(x, z, ex, ez));
            d >>= 6;
            if (d < best) {
                if (!(arg2 & 4) || func_80015480(get_angle(x - ex, z - ez), Heroes[arg0].hero_move->forward_angle) < 0x320) {
                    best = d;
                    found = (Ovl66Entry *)f;
                }
            }
        }
    }
    if (found != 0) {
        *arg1 = (s32)found;
        return 1;
    }
    return 0;
}

void func_800B47C8(s32 arg0, s32 arg1, Ovl66Tgt2 *t) {
    Ovl66Pad *p;
    EXTengine *e;
    s32 big;

    p = t->obj;
    if (p != NULL) {
        e = p->eng;
        big = 0;
        if (e != NULL) {
            big = ((subengine *)e->sub)->unk74 == 0x2015;
        }
        if (arg0 >= 0) {
            switch (p->mode) {
            case 0xE:
                func_800B6B04(arg0, e, big ? 0x14 : 5);
                break;
            case 0xD:
                func_800B6B04(arg0, e, big ? 0xF : 3);
                break;
            default:
                func_800B6B04(arg0, e, big ? 0xA : 2);
                break;
            }
            PlaySfx(D_800B9E30[arg0], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            if (big) {
                PlaySfx(0x72B, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
        p->mode = 4;
    }
}

void func_800B4910(VECTOR *pos, s32 a, s32 b, s32 c) {
    s32 x;
    s32 z;
    s32 ex;
    s32 ez;
    s32 ang;
    s32 i;
    HeroInfo *h;
    Ovl66Entry *e;
    EXTengine *eng;
    EXTheromove *mv;
    s32 vc;

    x = pos->vx;
    z = pos->vz;
    if (D_8005A644 & 0x801) {
        return;
    }
    i = 0;
    do {
        h = &Heroes[i];
        if (i == b) {
            goto next;
        }
        ex = h->hero_eng->unk10.x;
        ez = h->hero_eng->unk10.z;
        if ((csqrt(func_800B62C8(x, z, ex, ez)) >> 6) > a) {
            goto next;
        }
        if ((h->hero_immunity | h->hero_hit) != 0) {
            goto next;
        }
        ang = get_angle(x - ex, z - ez);
        mv = h->hero_move;
        vc = mv->velocity_calc;
        if (vc != 1) {
            if (vc != 0) {
                if (vc < 4) {
                    mv->x_vel -= (D_80068BD4[(ang - 0x400) & 0xFFF].unk2 * 5) >> 8;
                    h->hero_move->z_vel -= (D_80068BD4[ang & 0xFFF].unk2 * 5) >> 8;
                }
            }
        }
        h->hero_move->reactive_angle = ang;
        h->hero_move->reactive_speed = 0x50;
        func_80084E04(i, NULL, -1, 0x8001, 0x446);
        h->hero_health = 0x14;
        func_80028018(i, 0xF);
        func_800B8EF8(i);
        PlaySfx(D_8009E080[h->hero_name][Random(4)], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
    next:
        i++;
    } while (i < 4);
    if ((c & 0x100) && D_800B9EF0 != 0) {
        i = 0;
        do {
            e = &D_800B9E50[i];
            if (e->unk4 != 0) {
                eng = ((NAS *)e->unk4)->Engine;
                if ((csqrt(func_800B62C8(x, z, eng->unk10.x, eng->unk10.z)) >> 6) <= a
                    && ((Ovl66Pad *)e->unk4)->mode >= 0xB) {
                    func_800B47C8(b, 1, (Ovl66Tgt2 *)e);
                }
            }
            i++;
        } while (i < 10);
    }
}

void func_800B4BC8(s32 arg0, u16 arg1) {
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
            HeroInfo *a;
            HeroInfo *b;
            HeroInfo *ta;
            HeroInfo *tb;
            HeroInfo *hb;
            s32 sa;
            s32 sb;

            func_8007B120();
            hb = Heroes;
            a = &hb[*(s16 *)(g + 0x48)];
            b = &hb[*(s16 *)(g + 0x4A)];
            ta = &hb[a->hero_teammate];
            tb = &hb[b->hero_teammate];
            sa = a->hero_score + ta->hero_score;
            sb = b->hero_score + tb->hero_score;
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
            } else if (sb < sa) {
                *(u16 *)(g + 0x30) |= 0x800;
                if (g[*(s16 *)(g + 0x48) + 0x5C] == 0) {
                    g[0x60] = a->hero_name;
                    g[0x61] = ta->hero_name;
                    a->hero_status |= 0x200;
                    ta->hero_status |= 0x200;
                    b->hero_status |= 0x400;
                    tb->hero_status |= 0x400;
                } else {
                    g[0x60] = b->hero_name;
                    g[0x61] = tb->hero_name;
                    b->hero_status |= 0x200;
                    tb->hero_status |= 0x200;
                    a->hero_status |= 0x400;
                    ta->hero_status |= 0x400;
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

void func_800B5050(s32 arg0, u16 arg1) {
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
            HeroInfo *a;
            HeroInfo *b;
            HeroInfo *ta;
            HeroInfo *tb;
            HeroInfo *hb;
            s32 sa;
            s32 sb;

            func_8007B120();
            hb = Heroes;
            a = &hb[*(s16 *)(g + 0x48)];
            b = &hb[*(s16 *)(g + 0x4A)];
            ta = &hb[a->hero_teammate];
            tb = &hb[b->hero_teammate];
            sa = a->hero_score + ta->hero_score;
            sb = b->hero_score + tb->hero_score;
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
            } else if (sb < sa) {
                *(u16 *)(g + 0x30) |= 0x800;
                if (g[*(s16 *)(g + 0x48) + 0x5C] == 0) {
                    g[0x60] = a->hero_name;
                    g[0x61] = ta->hero_name;
                    a->hero_status |= 0x200;
                    ta->hero_status |= 0x200;
                    b->hero_status |= 0x400;
                    tb->hero_status |= 0x400;
                } else {
                    g[0x60] = b->hero_name;
                    g[0x61] = tb->hero_name;
                    b->hero_status |= 0x200;
                    tb->hero_status |= 0x200;
                    a->hero_status |= 0x400;
                    ta->hero_status |= 0x400;
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

void func_800B54D8(s32 arg0, u16 arg1) {
    s32 found;
    s32 fail;
    u8 *g;
    s32 i;

    found = -1;
    fail = 0;
    g = D_8005A614;
    if (*(s32 *)(g + 0x6C) != 0) {
        HeroInfo *hb = Heroes;
        HeroInfo *a;
        HeroInfo *ta;
        HeroInfo *b;
        s32 sa;
        s32 sb;

        a = &hb[*(s16 *)(g + 0x48)];
        ta = &hb[a->hero_teammate];
        b = &hb[*(s16 *)(g + 0x4A)];
        sa = a->hero_score + ta->hero_score;
        sb = b->hero_score + hb[b->hero_teammate].hero_score;
        if (sa == 0) {
            if (g[*(s16 *)(g + 0x48) + 0x5C] == 0) {
                D_8005A614[0x60] = a->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                D_8009917C |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (D_8005A614[i + 0x5C] != 0) {
                        hb[i].hero_status |= 0x400;
                    } else {
                        hb[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        } else if (sb == 0) {
            if (g[*(s16 *)(g + 0x4A) + 0x5C] == 0) {
                D_8005A614[0x60] = b->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                D_8009917C |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (D_8005A614[i + 0x5C] != 0) {
                        hb[i].hero_status |= 0x400;
                    } else {
                        hb[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (!Heroes[i].hero_score) {
                found = i;
            }
        }
        if (found != -1) {
            if (D_8005A614[found + 0x5C] == 0) {
                D_8005A614[0x60] = Heroes[found].hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                D_8009917C |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (D_8005A614[i + 0x5C] != 0) {
                        Heroes[i].hero_status |= 0x400;
                    } else {
                        Heroes[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        }
    }
    if (fail || arg0 == -1) {
        D_8005A614[0x60] = 0;
        *(u16 *)(D_8005A614 + 0x30) |= arg1;
        D_8009917C |= 0x200;
        for (i = 0; i < 4; i++) {
            if (D_8005A614[i + 0x5C] != 0) {
                Heroes[i].hero_status |= 0x400;
            } else {
                Heroes[i].hero_status |= 0x100;
            }
        }
    }
}

void func_800B5820(s32 arg0, u16 arg1) {
    Ovl66RankView *g;
    Ovl66RankView *p;
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
        RANK66V->order[i] = best;
        used |= 1 << best;
    }
    g = RANK66V;
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
        RANK66V->winner[0] = b->hero_name;
        RANK66V->winner[1] = bm->hero_name;
        b->hero_status |= 0x200;
        bm->hero_status |= 0x200;
        ta->hero_status |= 0x400;
        am->hero_status |= 0x400;
    }
}

void func_800B5BA0(s32 arg0)
{
    u8 *g = D_8005A614;
    HeroInfo *h;
    HeroInfo *heroes;
    EXTbotai *ai;
    s32 sx;
    s32 sz;
    HeroInfo *me;
    u32 i;
    Ovl66Hit *hit;
    u16 mode;
    u16 ang;

    if (*(u32 *)(g + 0x10) & 0x04000000) {
        return;
    }
    heroes = Heroes;
    h = &heroes[arg0];
    ai = h->hero_ai;
    if (ai == NULL) {
        return;
    }
    if (ai->AItimer1 != 0) {
        ai->AItimer1--;
    }
    ai->AIpad = 0;
    if (func_800B61B0(arg0, (int *)&hit) != 0) {
        if (hit->nas != ai->pTargetNas && ai->AItimer1 == 0) {
            ai->pTargetNas = hit->nas;
            ai->TargetXYZ = hit->nas->Engine->unk10;
            ai->AItimer1 = 10;
            if (hit->kind == 0x100) {
                if (D_800B9F4C[arg0].f8 == 1 || (*(u16 *)(g + 0x32) & 0x80) ||
                    func_800B60FC(arg0) != 0) {
                    ai->AImode = 2;
                } else {
                    mode = 3;
                    if (D_8004E0D8 & 1) {
                        mode = 2;
                    }
                    ai->AImode = mode;
                }
            } else {
                if (D_800B9F4C[arg0].f8 == 1 || (*(u16 *)(g + 0x32) & 0x80) ||
                    func_800B60FC(arg0) != 0) {
                    ai->AImode = 4;
                } else {
                    mode = 5;
                    if (D_8004E0D8 & 1) {
                        mode = 4;
                    }
                    ai->AImode = mode;
                }
            }
        }
    } else {
        sz = 0;
        sx = 0;
        i = 0;
        me = h;
        while (1) {
            if (i < 4) {
                if (i != arg0) {
                    if ((csqrt(ovl66_sq(me->hero_eng->unk10.x, me->hero_eng->unk10.z,
                                        heroes[i].hero_eng->unk10.x, heroes[i].hero_eng->unk10.z)) >> 6) < 0x1000) {
                        sx += me->hero_eng->unk10.x - heroes[i].hero_eng->unk10.x;
                        sz += me->hero_eng->unk10.z - heroes[i].hero_eng->unk10.z;
                    }
                }
                i++;
                continue;
            }
            break;
        }
        if (sx != 0 || sz != 0) {
            ang = get_angle(sx, sz);
            ai->AIang = ang;
            ai->AIpad |= D_80051244[(ang >> 9) & 7];
            ai->AImode = 1;
        }
    }
    switch (ai->AImode) {
    case 1:
        break;
    case 2:
    case 3:
        if ((csqrt(ovl66_sq(ai->pTargetNas->Engine->unk10.x, ai->pTargetNas->Engine->unk10.z,
                            Heroes[arg0].hero_eng->unk10.x, Heroes[arg0].hero_eng->unk10.z)) >> 6) <
            D_800B9580[Heroes[arg0].hero_name]) {
            if ((ai->AImode == 3 && Heroes[arg0].hero_mode != 0x44) ||
                (ai->AImode == 2 && Heroes[arg0].hero_mode != 0x43)) {
                ai->AIpad |= 0x8000;
            }
        }
        goto face;
    case 4:
    case 5:
        if ((csqrt(ovl66_sq(ai->pTargetNas->Engine->unk10.x, ai->pTargetNas->Engine->unk10.z,
                            Heroes[arg0].hero_eng->unk10.x, Heroes[arg0].hero_eng->unk10.z)) >> 6) <
            D_800B9580[Heroes[arg0].hero_name]) {
            if ((ai->AImode == 5 && Heroes[arg0].hero_mode != 0x44) ||
                (ai->AImode == 4 && Heroes[arg0].hero_mode != 0x43)) {
                ai->AIpad |= 0x8000;
            }
        }
    face:
        ang = get_angle(ai->pTargetNas->Engine->unk10.x - Heroes[arg0].hero_eng->unk10.x,
                        ai->pTargetNas->Engine->unk10.z - Heroes[arg0].hero_eng->unk10.z);
        ai->AIang = ang;
        ai->AIpad |= D_80051244[(ang >> 9) & 7];
        break;
    }
    func_80013C94((u8 *)&D_80051338[arg0], ai->AIpad);
}

s32 func_800B60FC(s32 arg0)
{
  s32 new_var2;
  EXTengine *b;
  EXTengine *a;
  s32 dx;
  s32 dz;
  s32 new_var;
  s32 r;
  if ((*D_8005A680) != 0)
  {
    HeroInfo *t = &Heroes[Heroes[arg0].hero_teammate];
    a = Heroes[arg0].hero_eng;
    b = t->hero_eng;
    new_var = a->unk10.z;
    new_var2 = a->unk10.x;
    dx = b->unk10.x - new_var2;
    dz = b->unk10.z - new_var;
    r = (csqrt((dx * dx) + (dz * dz)) >> 6) < 0x501;
  }
  else
  {
    r = 0;
  }
  return r;
}

int func_800B61B0(int a0, int *out)
{
    int v;

    if (func_800B4454(a0, &v, 0x301) != 0) {
        *out = v;
        return 1;
    }
    return 0;
}

void func_800B61F8(s32 arg0) {
    EXTbotai *ai = Heroes[arg0].hero_ai;

    if (ai != NULL) {
        ai->AImode = 1;
        ai->AItimer1 = 0;
    }
}

void func_800B6238(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            func_800B4BC8(arg0, (u16)arg1);
        } else if (f & 0x20) {
            func_800B5050(arg0, (u16)arg1);
        } else if (f & 0x40) {
            func_800B54D8(arg0, (u16)arg1);
        } else {
            func_800B5820(arg0, (u16)arg1);
        }
    }
}

s32 func_800B62C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_a0;
    s32 temp_a1;

    temp_a0 = arg2 - arg0;
    temp_a1 = arg3 - arg1;
    return (temp_a0 * temp_a0) + (temp_a1 * temp_a1);
}

void PlotBonkFlag(NAS *nas) {
    func_800250B0(nas);
}

void InitBonkFlag(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        func_80024E30(nas, &D_800B9CE4);
        e->sub->unk6C = D_80053C0C;
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBonkShockwave(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 v;

    if (e != NULL) {
        s32 a = nas->UserInfo.user1;
        s32 b = nas->UserInfo.user2;

        if (a >= b) {
            KillNAS(nas);
        } else {
            s32 t = a - 0x800;

            nas->UserInfo.user1 = a + ((b - t) >> 1);
        }
        e->unk1E += 0x71;
        v = nas->UserInfo.user1;
        e->unk5E = 1;
        e->unk2C.z = v;
        e->unk2C.x = v;
    }
}

void InitBonkShockwave(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053C7C;
        e->sub->unk74 = 0x2014;
        e->unk5C = 1;
        e->unk2C.y = 0x1000;
        e->unk5D = 0;
        e->unk2C.z = 0;
        e->unk2C.x = 0;
        e->unk5E = 1;
        e->flags |= 0x8021;
        nas->UserInfo.user1 = 0;
        nas->UserInfo.user2 = 0x4000;
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800B64B4(s32 arg0, s32 arg1, Ovl66Obj *obj) {
    Ovl66Pad *p = obj->pad;

    if (p != NULL) {
        func_800B6CD8(arg0, Random(2));
        p->mode = 4;
        PlaySfx(D_800B9E40[Random(4)], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void func_800B6560(void) {
}

void func_800B6568(void) {
    s32 i;

    D_800B9F08 = 0;
    for (i = 0; i < 3; i++) {
        D_800B9EC8[i].a = 0x200;
        D_800B9EC8[i].b = 0;
        D_800B9EC8[i].c = 0;
    }
    for (i = 0; i < 2; i++) {
        SpawnNASById(0, 0, 0, 0x1A04, 0x11);
    }
}

void InitBonkStool(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        subengine *s;

        e->unk5C = 1;
        e->unk2C.z = 0xC00;
        e->unk2C.y = 0xC00;
        e->unk2C.x = 0xC00;
        e->unk5E = 1;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = 0;
        s = e->sub;
        e->flags &= 0xFFFF7FFF;
        if (s != NULL) {
            s->unk6C = D_80053C7C;
            s->unk74 = 0x2002;
        } else {
            nas->Failure = 1;
        }
        if (AddObject(nas) != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
        } else {
            nas->Failure = 1;
        }
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800B66C8(void) {
    NAS *n;

    if (D_800B9EF0 < 10) {
        n = SpawnNASById(0, D_8005A5D8 - 0x2000, 0, 0x1A01, 0x11);
        if (n != NULL) {
            n->UserInfo.user1 = Random(10) + D_800B9EF0 * 5;
            n->UserInfo.user2 = D_800B9EF0;
            D_800B9E50[D_800B9EF0].unk4 = (s32)n;
            D_800B9EF0 += 1;
        }
    }
}

void func_800B6774(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_8005A614[i + 0x5C] != 0) {
            D_800B9EEC = D_8009D87C[i][0];
        }
    }
}

void func_800B67C4(void) {
    func_800B34F8();
    func_800B6560();
    func_800B6774();
}

void func_800B67F4(void) {
    D_8009D904 = D_8005A680[0] != 0 ? D_800B92E8 : D_800B92D0;
    D_8009D8FC = D_8005A680[0] != 0 ? D_800B92B0 : D_800B929C;
    D_8009D900 = D_800B92C4;
}

void func_800B6868(s32 arg0) {
    func_8007B7FC(arg0 & 0xFFFF, 0x81D0);
}

void func_800B688C(void) {
    if (!(D_8005A646 & 0xE0)) {
        func_800B6868(0x130);
    }
}

void func_800B68BC(void) {
    if (D_8005A646 & 0x20) {
        func_800B6868(0x130);
    }
}

void func_800B68EC(void) {
    s32 i;
    s16 v;
    u8 *g = D_8005A614;

    u8 *h;

    if (*(u16 *)(g + 0x32) & 0x40) {
        i = 0;
        h = g;
        for (; i < 4; i++) {
            v = 0x3C;
            if (h[i + 0x5C] == 0) {
                v = 0x50;
                if (*(s32 *)(h + 0x6C) != 0) {
                    v = 0x5A;
                }
            }
            Heroes[i].hero_score_displayed = v;
            Heroes[i].hero_score = v;
        }
        func_800B6868(0x130);
        func_8007D144();
        if (D_8005A680[0] != 0) {
            D_80099174 |= 0x2000;
        }
    }
}

void func_800B69A0(void) {
    if (D_8005A646 & 0x80) {
        func_800B6868(0x130);
    }
}

void func_800B69D0(void) {
    Ovl66Cam *cam = (Ovl66Cam *)D_80051640;
    cam->unk0 = 0;
    cam->unk8 = 0xE98;
    cam->unk54 = 0x192;
    cam->unk18 = 0x200;
    cam->unkC = 0x60;
    cam->unk10 = 0x20;
    cam->unk4 = 0;
    cam->unk56 = 0;
    cam->unk58 = 0;
    cam->unk14 = -0x660;
}

void func_800B6A1C(void) {
    D_8005A5D8 = -0x10;
    func_8007EB1C(0x140, 0x28, 0);
    func_8007B7FC(0x130, 0x81D0);
    func_800B69D0();
    D_800B9EEC = 0x10;
    func_800B6774();
    func_800B69A0();
    func_800B68EC();
    func_800B68BC();
    func_800B688C();
    func_800B9264();
    func_800B347C();
    func_800B6568();
}

void func_800B6AAC(void) {
    D_8005AA70[0] = (s32)func_800B33A4;
    D_8005AA70[1] = (s32)func_800B6A1C;
    D_8005AA70[2] = (s32)func_800B6A1C;
    D_8005AA70[3] = (s32)func_800B67C4;
    D_8005AA70[5] = (s32)func_800B6AFC;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
}

void func_800B6AF4(void) {
}

void func_800B6AFC(void) {
}

void func_800B6B04(s32 arg0, EXTengine *p, s32 arg2) {
    HeroInfo *h;
    HeroInfo *t;
    HeroInfo *base, *b2, *b3, *h2;
    s32 v;
    s32 s;
    s32 x, y, z;
    u8 *g;

    if (p != NULL) {
        x = p->unk10.x;
    } else {
        x = Heroes[arg0].hero_eng->unk10.x;
    }
    if (p != NULL) {
        y = p->unk10.y;
    } else {
        y = Heroes[arg0].hero_eng->unk10.y;
    }
    if (p != NULL) {
        z = p->unk10.z;
    } else {
        z = Heroes[arg0].hero_eng->unk10.z;
    }
    func_8008D5D4(x, y, z, arg2, 0);
    g = D_8005A614;
    if (*(u16 *)(g + 0x32) & 0x40) {
        if (*(s32 *)(g + 0x6C) != 0) {
            base = Heroes;
            h = base + arg0;
            s = h->hero_score;
            if (s < arg2) {
                h->hero_score = 0;
                t = base + h->hero_teammate;
                t->hero_score = t->hero_score - (arg2 - s);
                return;
            }
        }
        b2 = Heroes;
        h2 = arg0 + b2;
        v = (u16)h2->hero_score - arg2;
    } else {
        b3 = Heroes;
        h2 = arg0 + b3;
        v = (u16)h2->hero_score + arg2;
    }
    h2->hero_score = v;
}

void func_800B6CD8(s32 arg0, s32 arg1) {
    EXTengine *e;
    subengine *s;
    s32 sfx;
    HeroInfo *h;
    HeroInfo *base;

    s = NULL;
    func_800B8EF8(arg0);
    base = Heroes;
    h = &base[arg0];
    e = h->hero_powerup;
    if (e != NULL) {
        s = e->sub;
    }
    switch (arg1) {
    case 0:
        D_800B9F4C[arg0].fC = 0xB4;
        if (s != NULL) {
            s->unk6C = D_80053C7C;
            s->unk74 = 0x2013;
            s->flags |= 0x8000;
            e->flags |= 0x8000;
        }
        sfx = 0x41C;
        break;
    case 1:
        if (s != NULL) {
            s->unk6C = D_80052F5C;
            s->unk74 = 0x200C;
            s->flags |= 0x8000;
            e->flags |= 0x8000;
        }
        sfx = 0x72E;
        break;
    default:
        return;
    }
    if (s != NULL) {
        PlaySfx(sfx, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        D_800B9F4C[arg0].f8 = arg1;
    } else {
        PlaySfx(sfx, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        D_800B9F4C[arg0].f8 = arg1;
    }
}

void func_800B6E1C(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + player;
    EXTengine *p = h->hero_powerup;

    if (p != NULL) {
        EXTengine *e = h->hero_eng;
        p->unk10 = e->unk10;
        p->unk10.y -= D_800B9594[h->hero_name];
        p->unk1E += 0x38;
        p->unk5E = 1;
    }
    if (D_800B9F4C[player].f8 == 0) {
        if (--D_800B9F4C[player].fC <= 0) {
            func_800B8EF8(player);
        }
    }
}
