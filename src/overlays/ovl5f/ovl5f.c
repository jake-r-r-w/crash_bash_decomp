#include "common.h"
#include "bash/main.h"
#include "bash/ovl5f.h"
#include "bash/engine.h"
#include "bash/tank.h"

s32 func_800B3B24(arg0)
s32 arg0;
{
    s32 sum;
    s32 i;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00) && D_800CAD50[i].Energy > 0) {
            sum += D_8009D87C[i].unk4;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00) && D_800CAD50[i].Energy > 0) {
            sum += D_8009D87C[i].unk4;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

s32 func_800B3C7C(arg0)
s32 arg0;
{
    s32 sum;
    s32 i;
    s32 j;
    s32 r;
    s32 start;
    s32 mate;
    HeroInfo *h;
    EXTengine *me;
    EXTengine *te;
    s32 ex;

    mate = Heroes[arg0].hero_teammate;
    start = Random(4);
    sum = 0;
    for (i = start; i < start + 4; i++) {
        j = D_800CA160[i];
        if (j != arg0 && j != mate && !((h = &Heroes[j])->hero_status & 0xFA00) && D_800CAD50[j].Energy > 0) {
            sum += D_8009D87C[j].unk4;
            if (D_8005A614[j + 0x5C] != 0 && (u32)Random(0x100) < 0xD3) {
                continue;
            }
            me = Heroes[arg0].hero_eng;
            te = h->hero_eng;
            if (Ovl5fNear(ex = me->unk10.x, te->unk10.x) || Ovl5fNear(ex = me->unk10.z, te->unk10.z)) {
                return j;
            }
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = start; i < start + 4; i++) {
        j = D_800CA160[i];
        if (j != arg0 && j != mate && !(Heroes[j].hero_status & 0xFA00) && D_800CAD50[j].Energy > 0) {
            sum += D_8009D87C[j].unk4;
            if (r < sum) {
                return j;
            }
        }
    }
    return -1;
}

s32 func_800B3F44(s32 player, EXTengine *eng, Ovl5fAITarget *ai) {
    EXTengine *te;
    EXTengine *me;
    HeroInfo *hb;
    s32 d;
    s32 dir;
    s32 ex;
    s32 ez;
    s32 tx;
    s32 tz;
    s32 dx;
    s32 dz;
    s32 a;
    s32 i;
    s32 t;

    if (D_8009D908 == 0) {
        i = Random(0x65);
        if (ai->timer < (u32)i) {
            return 0;
        }
    }
    do {
        do {
            i = ai->unk8;
        } while (0);
    } while (0);
    if (i >= 0) {
    if (((Ovl5fModeView *)D_8005A614)->mode == 5) {
        te = Heroes[ai->unk8].hero_eng;
        a = get_angle(eng->unk10.x - te->unk10.x, eng->unk10.z - te->unk10.z);
        if ((u32)(((eng->unk1E - a) & 0xFFF) - 100) < 0xF39) {
            goto ret0;
        }
    } else {
    d = eng->unk1E;
    dir = (d + ((D_800CAD50[player].TurretReqdDir - d) << 20 >> 22)) & 0xFFF;
    if (((s32 (*)(s32, s32))func_800C891C)(eng->unk10.x + (-(D_80068BD4[(dir - 0x400) & 0xFFF].unk2 << 9) >> 12),
                      eng->unk10.z + (-(D_80068BD4[dir].unk2 << 9) >> 12)) != 0) {
        return 0;
    }
    if (((Ovl5fGameView *)D_8005A614)->w6C != 0 || ((Ovl5fModeView *)D_8005A614)->mode == 9) {
        hb = Heroes;
        me = hb[hb[player].hero_teammate].hero_eng;
        ex = eng->unk10.x;
        tx = me->unk10.x;
        player = tx - ex;
        if (player < 0) {
            player = -player;
        }
        tz = me->unk10.z;
        ez = eng->unk10.z;
        dz = tz - ez;
        if (dz < 0) {
            dz = -dz;
        }
        player = (player + dz) >> 8;
        if (player == 0) {
            player = 1;
        }
        do {
            do {
                te = hb[ai->unk8].hero_eng;
            } while (0);
        } while (0);
        dx = te->unk10.x - ex;
        if (dx < 0) {
            dx = -dx;
        }
        dz = te->unk10.z - ez;
        if (dz < 0) {
            dz = -dz;
        }
        dx = (dx + dz) >> 8;
        if (dx >= player) {
        t = dir - 0x800;
        a = ((t - get_angle(tx - ex, tz - ez)) << 20) >> 20;
        if (a < 0) {
            a = -a;
        }
        if (a * player < 0x800) {
            return 0;
        }
        }
    }
    }
    ai->flags |= 0x8000;
    return 1;
    }
ret0:
    return 0;
}

void func_800B4240(s32 player, s32 x, s32 z, EXTengine *eng, Ovl5fAITarget *ai) {
    s32 ex;
    s32 ez;
    s32 a;
    s32 r;
    s32 speed;
    s32 px;
    s32 p;

    ex = eng->unk10.x;
    ez = eng->unk10.z;
    a = get_angle(ex - x, ez - z);
    ai->unk0 = 2;
    speed = D_800CAD50[player].Speed;
    ai->unk4 = (speed / 2 + 0x400) / speed;
    ai->flags = D_80051244[(a >> 9) & 7];
    if (D_8009D908 != 0) {
        return;
    }
    r = Random(4);
    switch (r) {
    case 0:
    case 1:
        if (ai->flags & 0xA0) {
            goto doz;
        }
    dox:
        if (ai->flags & 0xA0) {
            break;
        }
        px = eng->unk10.x;
        if (px - 0x400 < 0) {
            goto left;
        }
        if (D_800CB100 < px + 0x400) {
            goto right;
        }
        if (r & 1) {
            if (((s32 (*)(s32, s32))func_800C8950)(ex - 0x100, ez) != 0) {
                goto left;
            }
        right:
            ai->flags |= 0x80;
            break;
        }
        if (((s32 (*)(s32, s32))func_800C8950)(ex + 0x100, ez) != 0) {
            goto right;
        }
    left:
        ai->flags |= 0x20;
        break;
    case 2:
    case 3:
        if (ai->flags & 0x50) {
            goto dox;
        }
    doz:
        if (ai->flags & 0x50) {
            break;
        }
        p = eng->unk10.z;
        if (D_800CB104 < p + 0x400) {
            goto up;
        }
        if (p - 0x400 < 0) {
            goto down;
        }
        if (r & 1) {
            if (((s32 (*)(s32, s32))func_800C8950)(ex, ez + 0x100) != 0) {
                goto up;
            }
        down:
            ai->flags |= 0x10;
            break;
        }
        if (((s32 (*)(s32, s32))func_800C8950)(ex, ez - 0x100) != 0) {
            goto down;
        }
    up:
        ai->flags |= 0x40;
        break;
    }
}

s32 func_800B4490(s32 player, EXTengine *eng, Ovl5fAITarget *ai) {
    NAS *nas;
    EXTengine *te;
    s32 i;
    s32 r;
    u16 type;
    NAS *cur;
    s32 ex;

    if (D_8009D908 != 0) {
        return 0;
    }
    if ((u32)Random(D_8009D87C[player].unk0 + 1) < 8) {
        return 0;
    }
    if (ai->unk0 == 2) {
        return 0;
    }
    cur = ai->target;
    if (cur != NULL) {
        nas = cur;
        type = nas->Type;
        if ((type != Ovl5fASTIndex(0x1303)) | (nas->NASid == 0)) {
            ai->target = NULL;
            return 0;
        }
        te = nas->Engine;
        if (Ovl5fNear(ex = eng->unk10.x, te->unk10.x) || Ovl5fNear(ex = eng->unk10.z, te->unk10.z)) {
            goto hit;
        }
        return 0;
    }
    if (++ai->scan >= 8) {
        ai->scan = 0;
    }
    nas = D_800CB0C0[ai->scan];
    if (nas == NULL) {
        return 0;
    }
    te = nas->Engine;
    if (Ovl5fNear(ex = eng->unk10.x, te->unk10.x) || Ovl5fNear(ex = eng->unk10.z, te->unk10.z)) {
    hit:
        ai->target = nas;
        func_800B4240(player, te->unk10.x, te->unk10.z, eng, ai);
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5f/ovl5f", D_800B32B8);

void func_800B4688(s32 player, EXTengine *eng, Ovl5fAITarget *ai) {
    NAS **pp;
    NAS *nas;
    NAS *tgt;
    EXTengine *te;
    EXTengine *e;
    s32 j;
    s32 i;
    s32 ax;
    s32 az;
    s32 n;
    s32 sx;
    s32 sz;
    s32 ex;
    s32 ez;
    s32 d;
    s32 r;

    switch (D_8005A648) {
    case 6:
    case 8:
        r = func_800B3C7C(player);
        ai->unk8 = r;
        break;
    case 5:
    case 7:
    case 9:
    default:
        r = func_800B3B24(player);
        ai->unk8 = r;
        break;
    }
    if (r < 0) {
        goto avg;
    }
    if (D_800CAD50[player].TurretFiring < 0) {
        switch (Random(8)) {
        case 0:
        default:
            if (D_8009D908 == 0 && (u32)Random(D_8009D87C[player].unk0 + 1) < 13) {
                ai->unk4 = Random(30) + 15;
                ai->flags = 0;
                ai->unk0 = 0;
                return;
            }
            goto retry;
        case 1:
        case 2:
        case 3:
        case 4:
            goto retry;
        case 5:
        case 6:
            goto avg;
        case 7:
            break;
        }
    }
find:
    j = 0;
    pp = D_800CAD44;
    ex = eng->unk10.x;
    ez = eng->unk10.z;
    for (; j < 3; j++, pp++) {
        nas = *pp;
        if (nas != NULL) {
            te = nas->Engine;
            d = ex - te->unk10.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (ez - te->unk10.z) < 0x600) {
                tgt = nas;
                goto found;
            }
        }
    }
    tgt = NULL;
found:
    if (tgt != NULL) {
        e = tgt->Engine;
        ai->x = e->unk10.x;
        ai->z = e->unk10.z;
    } else {
        r = Random(9);
        ai->x = D_800CA17C[r][0];
        ai->z = D_800CA17C[r][1];
    }
    ai->unk0 = 4;
    ai->unk4 = 0x5A;
    return;

avg:
    n = 0;
    sz = 0;
    sx = 0;
    for (i = 0; i < 4; i++) {
        if (i != player && !(Heroes[player].hero_status & 0xB000)) {
            n++;
            sx += Heroes[i].hero_eng->unk10.x;
            sz += Heroes[i].hero_eng->unk10.z;
        }
    }
    if (n < 2) {
        if ((u32)Random(100) < 30) {
            goto find;
        }
        goto retry;
    }
    ax = sx / n;
    az = sz / n;
    ai->x = ax < 0x800 ? 0xE00 : 0x200;
    ai->z = az < 0x800 ? 0xE00 : 0x200;
    ai->unk0 = 7;
    ai->unk4 = 0xB4;
    return;

retry:
    if (ai->unk8 >= 0) {
        ai->unk0 = 6;
        ai->unk4 = 0x5A;
        return;
    }
    if ((u32)Random(100) < 30) {
        goto find;
    }
    goto avg;
}

void func_800B4A28(s32 player, EXTengine *eng, Ovl5fAITarget *ai) {
    EXTengine *te;
    EXTengine *e;
    NAS *nas;
    NAS **pp;
    NAS *tgt;
    TA_player *ta;
    TA_player *tab;
    TA_player *tab2;
    s32 j;
    s32 ex;
    s32 ez;
    s32 d;
    s32 x;
    s32 z;
    s32 delay;
    s32 speed;
    s32 aspeed;
    s32 sspeed;
    s32 a;
    s32 r;

    switch (D_8005A648) {
    case 6:
    case 8:
        r = func_800B3C7C(player);
        break;
    case 5:
    case 7:
    case 9:
    default:
        r = func_800B3B24(player);
        break;
    }
    ai->unk8 = r;
    ai->unk4 = 30;
    if (ai->target != NULL && func_800B4490(player, eng, ai) != 0) {
        return;
    }
    switch (Random(8)) {
    case 0:
        if (D_8009D908 == 0 && (u32)Random(D_8009D87C[player].unk0 + 1) < 13) {
            ai->unk4 = Random(30) + 15;
            ai->flags = 0;
            ai->unk0 = 0;
            return;
        }
        goto chase;
    case 1:
    find:
        j = 0;
        pp = D_800CAD44;
        ex = eng->unk10.x;
        ez = eng->unk10.z;
        for (; j < 3; j++, pp++) {
            nas = *pp;
            if (nas != NULL) {
                te = nas->Engine;
                d = ex - te->unk10.x;
                if (d < 0) {
                    d = -d;
                }
                if (d + (ez - te->unk10.z) < 0x600) {
                    tgt = nas;
                    goto found;
                }
            }
        }
        tgt = NULL;
    found:
        if (tgt != NULL) {
            e = tgt->Engine;
            d = e->unk10.x - ex;
            if (d > 0x70) {
                goto px;
            }
            if (d < -0x70) {
                goto nx;
            }
            d = e->unk10.z - ez;
            if (d > 0x70) {
                goto pz;
            }
            if (d < -0x70) {
                goto nz;
            }
        }
        speed = D_800CAD50[player].Speed;
        ai->unk0 = 5;
        ai->unk4 = (speed / 2 + 0x200) / speed;
        for (;;) {
            switch (Random(4)) {
            case 0:
            px:
                x = ex + 0x200;
                if (D_800CB100 < x || ((s32 (*)(s32, s32))func_800C8950)(x, ez) != 0) {
                    continue;
                }
                ai->flags = 0x20;
                return;
            case 1:
            nx:
                x = ex - 0x200;
                if (x < 0 || ((s32 (*)(s32, s32))func_800C8950)(x, ez) != 0) {
                    continue;
                }
                ai->flags = 0x80;
                return;
            case 2:
            pz:
                z = ez + 0x200;
                if (D_800CB104 < z || ((s32 (*)(s32, s32))func_800C8950)(ex, z) != 0) {
                    continue;
                }
                ai->flags = 0x10;
                return;
            case 3:
            nz:
                z = ez - 0x200;
                if (z < 0 || ((s32 (*)(s32, s32))func_800C8950)(ex, z) != 0) {
                    continue;
                }
                ai->flags = 0x40;
                return;
            }
            return;
        }
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    chase:
        if (ai->unk8 < 0) {
            goto find;
        }
        tab = D_800CAD50;
        ta = &tab[player];
        delay = ta->MainWeaponFireDelay;
        if (delay < 5) {
        aim:
            e = Heroes[ai->unk8].hero_eng;
            a = get_angle(e->unk10.x - eng->unk10.x, e->unk10.z - eng->unk10.z);
            ai->unk0 = 4;
            aspeed = D_800CAD50[player].Speed;
            ai->unk4 = (aspeed / 2 + 0x200) / aspeed;
            ai->flags = D_80051244[(a >> 9) & 7];
            return;
        }
        if (D_8009D908 != 0) {
            if (delay != 0) {
                ta->MainWeaponFireDelay = delay - 1;
            }
            if ((u32)Random(0x100) > 0x40) {
                goto aim;
            }
        }
        e = Heroes[ai->unk8].hero_eng;
        func_800B4240(player, e->unk10.x, e->unk10.z, eng, ai);
        return;
    case 2:
        tab2 = D_800CAD50;
        ta = &tab2[player];
        if (ta->SubWeaponRounds <= 0) {
            goto chase;
        }
        sspeed = ta->Speed;
        ai->flags |= 0x2000;
        ai->unk4 = (sspeed / 2 + 0x200) / sspeed;
        return;
    }
}

void func_800B4FAC(s32 player) {
    Ovl5fAITarget *ai;
    EXTengine *eng;
    EXTengine *te;
    s32 buttons;
    s32 d;
    s32 a;

    eng = Heroes[player].hero_eng;
    ai = (Ovl5fAITarget *)(D_800CAFFC + player * 0x2C);
    if (ai->timer == 0) {
        ai->timer = D_8009D87C[player].unk0 * 0x55 / 32 + 5;
    }
    if (--ai->unk4 <= 0) {
        func_800B4688(player, eng, ai);
    }
    buttons = 0;
    switch (ai->unk0) {
    case 4:
    case 7:
        d = eng->unk10.x - ai->x;
        if ((u32)(d + 99) < 199) {
            ai->unk4 = 0;
        }
        a = get_angle(d, eng->unk10.z - ai->z) - eng->unk1E;
        a = (a << 20) >> 20;
        if (a > 40) {
            buttons = 0x820;
        }
        if (a < -40) {
            buttons = 0x880;
        }
        break;
    case 6:
        te = Heroes[ai->unk8].hero_eng;
        a = get_angle(eng->unk10.x - te->unk10.x, eng->unk10.z - te->unk10.z) - eng->unk1E;
        a = (a << 20) >> 20;
        if (a > 70) {
            buttons = 0x20;
        }
        if (a < -70) {
            buttons = 0x80;
        }
        break;
    }
    ai->flags &= ~0x8000;
    func_800B3F44(player, eng, ai);
    if (ai->flags & 0x8000) {
        buttons |= 0x8000;
    }
    if (D_800CB284[player] != 0) {
        buttons |= 0x4000;
        D_800CB284[player] = 0;
    }
    func_80013C94((u8 *)&D_80051338[player], buttons);
}

void func_800B5228(s32 player) {
    Ovl5fAITarget *ai;
    EXTengine *eng;
    EXTengine *te;
    EXTengine *tgt;
    NAS *nas;
    s32 i;
    s32 d;
    s32 ex;
    s32 ez;
    u8 *g;

    g = D_8005A614;
    if (*(s32 *)(g + 0x10) & 0x04000000) {
        return;
    }
    if (((Ovl5fModeView *)g)->mode == 5) {
        func_800B4FAC(player);
        return;
    }
    eng = Heroes[player].hero->Engine;
    ai = (Ovl5fAITarget *)(D_800CAFFC + player * 0x2C);
    if (ai->timer == 0) {
        ai->timer = D_8009D87C[player].unk0 * 0x55 / 32 + 5;
    }
    if (D_8005A648 != 5 && ai->unk8 >= 0) {
        tgt = Heroes[ai->unk8].hero_eng;
        D_800CAD50[player].TurretReqdDir =
            (get_angle(tgt->unk10.x - eng->unk10.x, tgt->unk10.z - eng->unk10.z) + 0x800) & 0xFFF;
    }
    if (ai->unk0 == 5 && !(eng->unk10.x & 0x1FF) && !(eng->unk10.z & 0x1FF)) {
        func_800B4A28(player, eng, ai);
    }
    if (--ai->unk4 <= 0) {
        func_800B4A28(player, eng, ai);
    } else {
        func_800B4490(player, eng, ai);
        func_800B3F44(player, eng, ai);
    }
    if (D_8009D908 == 0 && (u32)Random(D_8009D87C[player].unk0 + 1) >= 9) {
        ex = eng->unk10.x;
        ez = eng->unk10.z;
        for (i = 0; i < 8; i++) {
            nas = D_800CB0C0[i];
            if (nas != NULL) {
                te = nas->Engine;
                d = ex - te->unk10.x;
                if (d > 250) {
                    if (d < 500) {
                        ai->flags &= ~0x80;
                    }
                } else if (d < -250 && d > -500) {
                    ai->flags &= ~0x20;
                }
                d = ez - te->unk10.z;
                if (d > 250) {
                    if (d < 500) {
                        ai->flags &= ~0x40;
                    }
                } else if (d < -250 && d > -500) {
                    ai->flags &= ~0x10;
                }
            }
        }
    }
    func_80013C94((u8 *)&D_80051338[player], ai->flags);
}

void func_800B5588(void) {
    void **p;
    void **p2;
    void **p3;
    void **p4;
    void **p5;
    void *t;
    Ovl5fGameView *g;

    D_8009D6E8.unk4 = D_800C8EA0;
    D_8009D6E8.unk38 = func_800B5228;
    D_8009D6E8.unk0 = (s32)func_800B6658;
    D_8009D6E8.unk68 = func_800C8294;
    D_8009D6E8.unk14 = D_800C8EB4;
    D_8009D6E8.unk18 = func_800C8334;
    D_8009D6E8.unk20 = func_800C81C0;
    D_8009D6E8.unk24 = func_800B6C24;
    switch (D_8005A648) {
    case 6:
    default:
        D_8009D8F4 = D_800C9E74;
        D_8009D8F8 = D_800C9E88;
        D_8009D904 = D_800C9E5C;
        p = &D_8009D8FC;
        if (D_8005A680[0] != 0) {
            t = D_800C9E3C;
        } else {
            t = D_800C9E28;
        }
        *p = t;
        D_8009D900 = D_800C9E50;
        return;
    case 8:
        D_8009D8F4 = D_800C9EE8;
        D_8009D8F8 = D_800C9EFC;
        D_8009D904 = D_800C9ED0;
        p2 = &D_8009D8FC;
        if (D_8005A680[0] != 0) {
            t = D_800C9EB0;
        } else {
            t = D_800C9E9C;
        }
        *p2 = t;
        D_8009D900 = D_800C9EC4;
        return;
    case 7:
        D_8009D8F4 = D_800C9F5C;
        D_8009D8F8 = D_800C9F70;
        D_8009D904 = D_800C9F44;
        p3 = &D_8009D8FC;
        if (D_8005A680[0] != 0) {
            t = D_800C9F24;
        } else {
            t = D_800C9F10;
        }
        *p3 = t;
        D_8009D900 = D_800C9F38;
        return;
    case 5:
        D_8009D8F4 = D_800C9FD0;
        D_8009D8F8 = D_800C9FE4;
        D_8009D904 = D_800C9FB8;
        p4 = &D_8009D8FC;
        if (D_8005A680[0] != 0) {
            t = D_800C9F98;
        } else {
            t = D_800C9F84;
        }
        *p4 = t;
        D_8009D900 = D_800C9FAC;
        return;
    case 9:
        D_8009D8F4 = D_800CA120;
        D_8009D8F8 = D_800CA134;
        D_8009D904 = D_800CA148;
        p5 = &D_8009D8FC;
        if (D_8005A680[0] != 0) {
            t = D_800CA08C;
        } else {
            t = D_800C9FF8;
        }
        *p5 = t;
        D_8009D900 = D_800C9FAC;
        Heroes[2].hero_name = 9;
        Heroes[3].hero_name = 9;
        g = (Ovl5fGameView *)D_8005A614;
        g->flags |= 0x100;
        if (g->w6C == 0) {
            Heroes[1].hero_name = 9;
            g->b51 = 9;
        }
        g->b52 = 9;
        g->b53 = 9;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B58B0);

void func_800B5FC4(void) {
    s32 i;
    TA_player *ta;
    HeroInfo *hero;
    EXTengine *eng;
    u8 *base;
    u8 *b2;
    s32 e;
    s32 j;

    if (D_8005A648 == 8) {
        D_800CB0F8--;
    }
    for (i = 0; i < 4; i++) {
        ta = &D_800CAD50[i];
        eng = Heroes[i].hero_eng;
        e = ta->Energy;
        ta->PosX = eng->unk10.x;
        ta->PosZ = eng->unk10.z;
        if (e <= 0) {
            func_800C8090(i, eng, 0);
            func_800C8008(i, (s32)eng, 0);
            func_800C7F80(i, (s32)eng, 0);
        }
    }
    if (D_800CB0E0 <= 0) {
        if (D_800CB0E0 != 0) {
            PlaySfx(0x4B9, NULL, &D_800CB0E8->unk10, 0x1000, 0x1E00, 0);
            D_800CB0E0 = 60;
        }
    } else {
        D_800CB0E0--;
    }
    if (D_800CB0EC > 0) {
        if (--D_800CB0EC == 0) {
            PlaySfx(0x501, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if (D_800CB0E4 != 0) {
        PlaySfx(D_800CB0E4, NULL, NULL, 0x1000, 0x1E00, 0);
        D_800CB0E4 = 0;
    }
    base = D_80052A54;
    if (TANK_MODEL_ON(base, 56)) {
        func_80029100((AnimOwner *)(TANK_MODEL(56) + 0x10));
    }
    if (D_8005A648 == 5 && TANK_MODEL_ON(base, 60)) {
        func_80029100((AnimOwner *)(TANK_MODEL(60) + 0x10));
    }
    if (D_8005A648 == 9 && TANK_MODEL_ON(b2 = D_80052A54, 57)) {
        func_80029100((AnimOwner *)(b2 + 57 * 0x38 + 0x10));
    }
    if (D_8009D908 == 0) {
        D_8009D908 = 1;
        for (j = 0; j < 4; j++) {
            if (!(Heroes[j].hero_status & 0xFA00)) {
                if (D_8005A614[j + 0x5C] == 0) {
                    D_8009D908 = 0;
                }
                if (((Ovl5fGameView *)D_8005A614)->w6C != 0 && D_8005A614[Heroes[j].hero_teammate + 0x5C] == 0) {
                    D_8009D908 = 0;
                }
            }
        }
        if (D_8009D908 != 0) {
            for (j = 0; j < 4; j++) {
                D_8009D87C[j].unk4 = 0x10;
                D_8009D87C[j].unk0 = 0x1E;
            }
        }
    }
}

void func_800B62A4(void) {
    s32 i;
    s32 best;
    s32 bestHealth;
    s32 h;

    bestHealth = 0;
    best = -1;
    i = bestHealth;
    for (; i < 4; i++) {
        if (!(Heroes[i].hero_status & 0xFA00)) {
            h = Heroes[i].hero_health;
            if (bestHealth < h) {
                bestHealth = h;
                best = i;
            }
        }
    }
    if (best != -1) {
        for (i = 0; i < 4; i++) {
            if (!(Heroes[i].hero_status & 0xFA00) && i != best) {
                D_800CAD50[i].RequestQuickFinish = 1;
            }
        }
    }
}

void func_800B6354(s32 player, s32 damage) {
    HeroInfo *h;
    HeroInfo *hb;
    EXTengine *eng;
    TA_player *ta;
    u8 *g;
    s32 t;
    s32 u;

    if (D_800CB0F0 != 0) {
        return;
    }
    if (Heroes[player].hero_immunity != 0) {
        return;
    }
    if (Heroes[player].hero_status & 0xFA00) {
        return;
    }
    g = D_8005A614;
    if (((Ovl5fModeView *)g)->mode == 9 && (D_8009D6E0 & 0x8000)) {
        return;
    }
    if (damage < 400) {
        ((void (*)(s32, s32, s32))func_80027F7C)(player, 0xA, 0xF0);
        if (D_8009D908 != 0) {
            damage *= 3;
        } else if (g[player + 0x5C] != 0) {
            t = (0x20 - D_8009D87C[player].unk0) * 150;
            u = t / 32; /* dead: keeps t live for the divide's copy */
            t = t / 32 + 50;
            damage = damage * t / 100;
        }
    }
    hb = Heroes;
    h = &hb[player];
    eng = h->hero_eng;
    if (h->hero_mode == 0x69) {
        return;
    }
    h->hero->UserInfo.status = 0x20;
    ta = &D_800CAD50[player];
    if (D_8005A648 == 9 && (u32)(player - 2) < 2) {
        func_800C6914(player, 0x69);
    } else {
        SetHeroAction((HeroInfo *)player, 0x69);
    }
    if (D_8005A614[player + 0x5C] != 0 || (((Ovl5fGameView *)D_8005A614)->w4 & 0x80000001) != 0x80000001) {
        ta->Energy -= damage;
    }
    if (ta->Energy <= 0) {
        func_800C8090(player, eng, 0);
        func_800C8008(player, (s32)eng, 0);
        func_800C7F80(player, (s32)eng, 0);
        ta->Energy = 0;
        if ((((Ovl5fGameView *)D_8005A614)->w4 & 0x80000002) == 0x80000002) {
            ta->Energy = 2;
        } else {
            Heroes[player].hero_status |= 0x2000;
        }
    }
    Heroes[player].hero_health = ta->Energy;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B6658);

void func_800B6C24(s32 player) {
    HeroInfo *hb;
    HeroInfo *hero;
    EXTengine *eng;
    TA_player *ta;
    NAS *nas;
    s32 x;
    s32 dz;

    hb = Heroes;
    hero = &hb[player];
    eng = hero->hero_eng;
    ta = &D_800CAD50[player];
    func_800B71F8(player);
    if (D_8005A648 != 5) {
        x = eng->unk10.x;
        dz = eng->unk10.z - eng->OldXYZ.z;
        if (x != eng->OldXYZ.x || dz != 0) {
            if (ta->HangaboutTime != 0) {
                ta->HangaboutTime--;
            }
            func_800C8090(player, eng, D_800C94C8[player]);
        } else {
            if (!(hero->hero_status & 0xFA00)) {
                ta->HangaboutTime++;
            }
            func_800C8090(player, eng, 0);
        }
        if (ta->TurretSFXDuration != 0) {
            if (--ta->TurretSFXDuration == 0) {
                func_800C8008(player, (s32)eng, 0);
            } else {
                func_800C8008(player, (s32)eng, D_800C94D8[player]);
            }
        }
    } else {
        if (((PadButtons *)D_80051338)[player].unk50 & 0x800) {
            func_800C8090(player, eng, D_800C9408[player]);
        } else {
            func_800C8090(player, eng, 0);
        }
    }
    if (((Ovl5fModeView *)D_8005A614)->mode == 8 && D_8005A614[player + 0x5C] == 0 &&
        (((Ovl5fGameView *)D_8005A614)->flags & 0x80) && ta->HangaboutTime >= 30 && D_800CB0F0 == 0 &&
        D_800CB0F8 <= 0 && (eng->unk10.z > 0x400 || (u32)(eng->unk10.x - 0x301) < 0xDFF)) {
        nas = SpawnNASById(eng->unk10.x, -0x8FC, eng->unk10.z, 0x1308, 0x11);
        if (nas != NULL) {
            nas->UserInfo.status = 6;
            D_800CB0F8 = 60;
            nas->Engine->sub->unk74 = 0x202B;
            ta->HangaboutTime = 0;
        }
    }
}

void func_800B6ED8(s32 player) {
    if (D_8005A614[player + 0x5C] != 0) {
        if (++D_800CAD50[player].IdleAnimCt >= 0x50 && D_800CB0F0 == 0) {
            D_800CAD50[player].IdleAnimCt = Random(0x1E);
            SetHeroSeq((HeroInfo *)player, 0x5A);
            D_800CAD50[player].IdleAnimCt = 0;
            PlaySfx(D_800C94A8[Heroes[player].hero_name - 1], NULL, &Heroes[player].hero_eng->unk10, 0x1000, 0x1E00,
                    0);
            return;
        }
    }
    SetHeroSeq((HeroInfo *)player, 0x59);
}

void func_800B6FF4(s32 player) {
    TA_player *ta;
    subengine *sub;

    ta = &D_800CAD50[player];
    ta->MainWeaponRounds = 999;
    ta->MainWeaponFireDelay = 30;
    sub = Heroes[player].hero_sub->next;
    switch (Heroes[player].hero_name) {
    case 1:
    case 2:
        ta->MainWeaponType = 9;
        if (D_8005A648 == 5) {
            ta->TurretAnimPtr = D_800C9A54;
            sub->next->unk74 = 0x4300;
        } else {
            sub->unk74 = 0x2010;
        }
        break;
    case 7:
    case 8:
        ta->MainWeaponType = 8;
        if (D_8005A648 == 5) {
            ta->TurretAnimPtr = D_800C9B44;
            sub->next->unk74 = 0x4100;
        } else {
            sub->unk74 = 0x2009;
        }
        break;
    default:
        ta->MainWeaponType = 11;
        if (D_8005A648 == 5) {
            ta->TurretAnimPtr = D_800C9AA4;
            sub->next->unk74 = 0x4200;
        } else {
            sub->unk74 = 0x2011;
        }
        break;
    case 5:
    case 6:
        ta->MainWeaponType = 10;
        if (D_8005A648 == 5) {
            ta->TurretAnimPtr = D_800C9AF4;
            sub->next->unk74 = 0x4000;
        } else {
            sub->unk74 = 0x2012;
        }
        break;
    }
    if (((Ovl5fModeView *)D_8005A614)->mode == 5) {
        sub->unk74 = 0x2005;
    }
    if (((Ovl5fModeView *)D_8005A614)->mode == 9) {
        ta->MainWeaponRounds = 99999;
        if (D_8005A614[player + 0x5C] != 0) {
            ta->MainWeaponType = 11;
            sub->unk74 = 0x2011;
            return;
        }
        if (D_800CB25C == 0x3A) {
            ta->MainWeaponType = 2;
            sub->unk74 = 0x200B;
            return;
        }
        ta->MainWeaponType = 3;
        sub->unk74 = 0x2008;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B71F8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B77E8);

void func_800B8EC4(s32 player) {
    s32 done;
    u8 unused[8];

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)player, 0x70);
        D_800CB0F0 = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        EXTengine *eng;

        Heroes[player].hero_move->jump_max = 0;
        Heroes[player].hero_move->velocity_enable = 1;
        Heroes[player].hero_idle_count = Random(0x96) + 0x5A;
        SetHeroSeq((HeroInfo *)player, 0x59);
        eng = Heroes[player].hero_eng;
        if (D_800CAD50[player].HullSFX != 0) {
            D_800CAD50[player].HullSFX = 0;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
        }
        if (D_800CAD50[player].TurretSFX != 0) {
            D_800CAD50[player].TurretSFX = 0;
            StopSfx(D_800CAD50[player].TurretSFXHandler);
        }
        if (D_800CAD50[player].FireBreatherSFX != 0) {
            D_800CAD50[player].FireBreatherSFX = 0;
            StopSfx(D_800CAD50[player].FireBreatherSFXHandler);
        }
        func_800B6ED8(player);
    }
}

void func_800B906C(s32 player) {
    s32 done;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)player, 0x70);
        D_800CB0F0 = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        SetHeroSeq((HeroInfo *)player, 0x59);
        PlaySfx(D_800C94E8[player], NULL, &Heroes[player].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void func_800B9170(s32 player) {
    s32 done;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)player, 0x70);
        D_800CB0F0 = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        EXTengine *eng;
        TA_player *ta;
        subengine *sub;
        HeroInfo *h;
        HeroInfo *hb;

        func_800B77E8(player);
        if (func_8008502C(player) != 0) {
            func_800B6ED8(player);
        }
        hb = Heroes;
        h = &hb[player];
        ta = &D_800CAD50[player];
        eng = h->hero_eng;
        sub = h->hero_sub->next->next;
        sub->unk12 += ta->ModeSpeed;
        sub->unk52 = 1;
        if (--ta->ModeCounter == 0) {
            SetHeroAction((HeroInfo *)player, 0x6E);
            switch (ta->MoveDir) {
            case 2:
                if (((PadButtons *)D_80051338)[player].unk50 & 0x10) {
                    eng->unk10.z += ta->Speed;
                }
                break;
            case 0:
                if (((PadButtons *)D_80051338)[player].unk50 & 0x40) {
                    eng->unk10.z -= ta->Speed;
                }
                break;
            case 3:
                if (((PadButtons *)D_80051338)[player].unk50 & 0x20) {
                    eng->unk10.x += ta->Speed;
                }
                break;
            case 1:
                if (((PadButtons *)D_80051338)[player].unk50 & 0x80) {
                    eng->unk10.x -= ta->Speed;
                }
                break;
            }
        }
    }
}

void func_800B9430(s32 player) {
    TA_player *ta;
    TA_player *o;
    EXTengine *eng;
    EXTengine *e;
    HeroInfo *h;
    s32 pad;
    s32 v;
    s32 px;
    s32 pz;
    s32 c;
    s32 x;
    s32 z;
    s32 d;
    s32 lim;
    s32 n;
    s32 bank;
    s32 sp;
    s32 i;
    s32 hit;
    u16 name;

    ta = &D_800CAD50[player];
    eng = Heroes[player].hero_eng;
    bank = ta->BankAngle;
    pad = ((PadButtons *)D_80051338)[player].unk50 & 0xF0;
    if (bank != 0) {
        eng->unk5E = 1;
        if (ta->BankAngle > 0) {
            ta->BankAngle -= 10;
            if (ta->BankAngle < 3) {
                ta->BankAngle = 0;
            }
        } else {
            ta->BankAngle += 10;
            if (ta->BankAngle >= -2) {
                ta->BankAngle = 0;
            }
        }
    }
    if (pad != 0) {
        if (pad & 0x20) {
            ta->BankAngle -= 20;
            if (ta->BankAngle < -200) {
                ta->BankAngle = -200;
            }
            eng->unk1E += ta->Speed * 3 / 2;
        }
        if (pad & 0x80) {
            ta->BankAngle += 20;
            if (ta->BankAngle > 200) {
                ta->BankAngle = 200;
            }
            eng->unk1E -= ta->Speed * 3 / 2;
        }
        eng->unk5E = 1;
        eng->unk1E &= 0xFFF;
    }
    eng->unk20 = ta->BankAngle & 0xFFF;
    c = ((PadButtons *)D_80051338)[player].unk50;
    if (c & 0x800) {
        goto thrust;
    }
    if (c & 0x200) {
    thrust:
        v = ta->Speed;
        px = v * D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2;
        pz = v * D_80068BD4[(u16)eng->unk1E & 0xFFF].unk2;
        ta->SpeedX += -(px >> 12) >> 2;
        ta->SpeedZ += -(pz >> 12) >> 2;
    }
    sp = ta->SpeedX;
    c = ta->Speed;
    if (c < sp || sp < (c = -c)) {
        ta->SpeedX = c;
    }
    sp = ta->SpeedZ;
    c = ta->Speed;
    if (c < sp || sp < (c = -c)) {
        ta->SpeedZ = c;
    }
    x = eng->unk10.x + ta->SpeedX;
    if (ta->SpeedX > 0) {
        ta->SpeedX--;
        if (D_800CB100 < x) {
            x = D_800CB100;
            ta->SpeedX = -ta->SpeedX;
        }
    } else {
        if (ta->SpeedX != 0) {
            ta->SpeedX++;
        }
        if (x < 0x100) {
            x = 0x100;
            ta->SpeedX = -ta->SpeedX;
        }
    }
    z = eng->unk10.z + ta->SpeedZ;
    if (ta->SpeedZ > 0) {
        ta->SpeedZ--;
        if (D_800CB104 < z) {
            z = D_800CB104;
            ta->SpeedZ = -ta->SpeedZ;
        }
    } else {
        if (ta->SpeedZ != 0) {
            ta->SpeedZ++;
        }
        if (z < 0x100) {
            z = 0x100;
            ta->SpeedZ = -ta->SpeedZ;
        }
    }
    lim = 0x1C0;
    for (i = 0; i < 4; i++) {
        if (i == player) {
            continue;
        }
        h = &Heroes[i];
        name = h->hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (h->hero_status & 0xC000) {
            continue;
        }
        e = h->hero_eng;
        d = x - e->unk10.x;
        if (d < -0x1C0) {
            continue;
        }
        if (lim < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -0x1C0;
        if (n < d && d < 0x1C0) {
            hit = i;
            goto found;
        }
    }
    hit = -1;
found:
    if (hit < 0) {
        eng->unk10.x = x;
        eng->unk10.z = z;
        return;
    }
    if (D_800CB0E0 == 0) {
        D_800CB0E8 = eng;
        D_800CB0E0 = -1;
    }
    o = &D_800CAD50[hit];
    o->SpeedX += ta->SpeedX / 2;
    ta->SpeedX /= 4;
    o->SpeedZ += ta->SpeedZ / 2;
    ta->SpeedZ /= 4;
}

void func_800B993C(s32 player) {
    EXTengine *eng;
    EXTengine *e;
    subengine *sub;
    HeroInfo *h;
    s32 pad;
    s32 speed;
    s32 dir;
    s32 angle;
    s32 dx;
    s32 dz;
    s32 m;
    s32 a;
    s32 sn;
    s32 cs;
    u16 old;
    s32 x;
    s32 z;
    s32 fx;
    s32 fz;
    s32 lim;
    s32 n;
    s32 d;
    s32 t;
    s32 df;
    u16 name;
    TA_player *ta;
    s32 hit;
    s32 i;

    pad = ((PadButtons *)D_80051338)[player].unk50;
    ta = &D_800CAD50[player];
    eng = Heroes[player].hero_eng;
    speed = ta->Speed;
    dir = pad & 0xF0;
    sub = eng->sub->next->next;
    if (dir == 0) {
        return;
    }
    if (((Ovl5fModeView *)D_8005A614)->mode == 7 && !(((Ovl5fModeView *)D_8005A614)->flags & 0x80)) {
        dx = D_800CB21C->Engine->unk10.x - eng->unk10.x;
        dz = D_800CB21C->Engine->unk10.z - eng->unk10.z;
        if (dx >= -0x15D) {
            if (dx < 0x15E) {
                if (dz >= -0x15D) {
                    if (dz < 0x15E) {
                        if (dx < 0) {
                            m = pad & 0x70;
                        } else {
                            m = pad & 0xD0;
                        }
                        if (dz > 0) {
                            m &= ~0x10;
                        } else {
                            m &= ~0x40;
                        }
                        dir = m;
                    }
                }
            }
        }
        dx = D_800CB220->Engine->unk10.x - eng->unk10.x;
        dz = D_800CB220->Engine->unk10.z - eng->unk10.z;
        if (dx >= -0x15D) if (dx < 0x15E) if (dz >= -0x15D) if (dz < 0x15E) {
            if (dx < 0) {
                m = dir & ~0x80;
            } else {
                m = dir & ~0x20;
            }
            if (dz > 0) {
                m &= ~0x10;
            } else {
                m &= ~0x40;
            }
            dir = m;
        }
    }
    if (dir == 0) {
        return;
    }
    switch (dir) {
    case 0x40:
        angle = 0x800;
        break;
    case 0x80:
        angle = 0xC00;
        break;
    case 0x20:
        angle = 0x400;
        break;
    case 0x90:
        angle = 0xE00;
        break;
    case 0x30:
        angle = 0x200;
        break;
    case 0xC0:
        angle = 0xA00;
        break;
    case 0x60:
        angle = 0x600;
        break;
    case 0x10:
    default:
        angle = 0;
        break;
    }
loop:
    a = (eng->unk1E + sub->unk12) & 0xFFF;
    df = ((angle - a) << 20) >> 20;
    a = (u16)sub->unk12;
    if ((u32)(df + 0x400) > 0x800) {
        sub->unk12 = (a + 0x800) & 0xFFF;
        goto loop;
    }
    df = df / 4;
    sub->unk12 = (a + df) & 0xFFF;
    sub->unk52 = 1;
    a = (eng->unk1E + sub->unk12) & 0xFFF;
    sn = D_80068BD4[(a - 0x400) & 0xFFF].unk2;
    cs = D_80068BD4[a].unk2;
    x = eng->unk10.x + ((speed * sn) >> 12);
    fx = x + ((sn * 256) >> 12);
    z = eng->unk10.z + ((speed * cs) >> 12);
    fz = z + ((cs * 256) >> 12);
    if (D_8005A648 == 7 && (u32)(x - 0x581) < 0x4FF && z >= 0x581 && z < 0xA80) {
        if (x < 0x800) {
            if (x < 0x5C0) {
                x = 0x580;
            } else if (x >= 0xA41) {
                x = 0xA80;
            }
        }
        if (z < 0x800) {
            if (z < 0x5C0) {
                z = 0x580;
            } else if (z >= 0xA41) {
                z = 0xA80;
            }
        }
    }
    lim = 0x1C0;
    for (i = 0; i < 4; i++) {
        if (i == player) {
            continue;
        }
        h = &Heroes[i];
        name = h->hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (h->hero_status & 0xC000) {
            continue;
        }
        e = h->hero_eng;
        d = x - e->unk10.x;
        if (d < -0x1C0) {
            continue;
        }
        if (lim < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -0x1C0;
        if (n < d && d < 0x1C0) {
            hit = i;
            goto found;
        }
    }
    hit = -1;
found:
    if (hit < 0) {
        t = ((fz >> 9) * D_800CAFF4) + (fx >> 9);
        if (D_800CACB4[t] == 0) {
            eng->unk10.x = x;
            eng->unk10.z = z;
        }
    } else if (D_800CB0E0 == 0) {
        D_800CB0E8 = eng;
        D_800CB0E0 = -1;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B9E20);

void func_800BA95C(s32 player) {
    s32 done;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)player, 0x70);
        D_800CB0F0 = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        D_800CAD50[player].SpeedY = -0x64;
        SetHeroSeq((HeroInfo *)player, 0x57);
        Heroes[player].hero_immunity = 0x3C;
        PlaySfx(D_800C9428[Heroes[player].hero_name - 1][Random(4)], NULL, &Heroes[player].hero_eng->unk10, 0x1000,
                0x1E00, 0);
    }
}

void func_800BAAB0(s32 arg0) {
    EXTengine *eng;
    HeroInfo *h;
    TA_player *p;

    eng = Heroes[arg0].hero_eng;
    h = &Heroes[arg0];
    D_800CAD50[arg0].SpeedY += 0xC;
    p = &D_800CAD50[arg0];
    eng->unk10.y += p->SpeedY;
    if (eng->unk10.y > 0) {
        p->SpeedY = -p->SpeedY / 2;
        eng->unk10.y = 0;
    }
    eng->sub->next->next->unk08 = -eng->unk10.y / 2;
    if (func_8008502C(arg0) != 0) {
        if (h->hero_status & 0x2000) {
            SetHeroAction((HeroInfo *)arg0, 0x6A);
        } else {
            SetHeroAction((HeroInfo *)arg0, h->hero_last_mode);
            SetHeroSeq((HeroInfo *)arg0, 0x59);
        }
    }
}

void func_800BABCC(s32 arg0) {
    u32 i;
    subengine *sub;

    if (D_8005A680[0] == 0) {
        D_800AD12C = 1;
        for (i = 0; i < 4; i++) {
            if (i != arg0 && !(Heroes[i].hero_status & 0x200)) {
                func_80026980(Heroes[i].hero);
            }
        }
    }
    Heroes[arg0].hero_ctrl |= 2;
    SetHeroSeq((HeroInfo *)arg0, 0x5A);
    func_80089FA4(arg0);
    func_80085718(arg0, 0);
    Heroes[arg0].hero_eng->unk10.y = 0;
    sub = Heroes[arg0].hero_eng->sub;
    sub = sub->next;
    sub = sub->next;
    sub->unk08 = 0;
}

void func_800BACD4(s32 arg0) {
    EXTengine *eng;
    subengine *sub;

    eng = Heroes[arg0].hero_eng;
    sub = Heroes[arg0].hero_sub->next->next;
    eng->unk1E += 0x15;
    eng->unk5E = 1;
    if (HLV->mode != 5) {
        sub->unk52 = 1;
        sub->unk12 -= 0x15;
    }
    func_8008502C(arg0);
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        if (HLV->state & 0x800) {
            HLV->unk30 |= 0x201;
        } else {
            HLV->unk30 |= 1;
        }
    }
}

void func_800BADB8(s32 arg0) {
    s32 ok;
    EXTengine *eng;
    s32 *pos;
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 0 && func_8008502C(arg0) != 0) {
        eng = Heroes[arg0].hero_eng;
        pos = (s32 *)&eng->unk10;
        p = D_8005365C;
        nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->unkC = 0xE;
                ext->unk18 = 0x100;
                if (p != NULL) {
                    ext->unk20 = 5;
                    nas->Engine->sub->unk6C = p;
                } else {
                    ext->unk20 = 1;
                }
                ext->unk28 = 0;
                nas->InitDelete(nas);
            }
        }
        PlaySfx(0x4F0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        SetHeroAction((HeroInfo *)arg0, 0x6B);
    }
}

void func_800BAF64(s32 arg0) {
    subengine *s;

    Heroes[arg0].hero_hit = 0;
    Heroes[arg0].hero_immunity = 0;
    Heroes[arg0].hero_health = 0;
    Heroes[arg0].hero_status |= 0x6000;
    func_800898EC(arg0);
    s = Heroes[arg0].hero_eng->sub;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800BAFFC);

void PlotLaserBitTA(NAS *nas) {
    EXTengine *eng;
    LaserBitFux *fux;
    subengine *sub;
    s32 hit;
    s32 sfx;
    s32 i;
    s16 rot;

    eng = nas->Engine;
    fux = (LaserBitFux *)nas->FUX;
    sub = eng->sub;
    sub = sub->next;
    sub->unk51 = 0;
    sub->unk14 += 0x32;
    rot = -eng->unk1E;
    sub->unk10 = 0;
    sub->unk52 = 1;
    sub->unk12 = rot;
    hit = func_800BAFFC(nas, 0x14, 0, 0xA);
    if (D_800CB0FC != 0) {
        switch ((u32)Random(3)) {
        case 0:
            sfx = 0x4D8;
            break;
        case 1:
            sfx = 0x4D9;
            break;
        case 2:
            sfx = 0x4DA;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:;
    }
    if (hit != 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        PlaySfx(0x4E7, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (i = 0; i < 3; i++) {
            if (D_800CAD50[fux->player].MainWeaponNas[i] == nas) {
                D_800CAD50[fux->player].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (fux->unk30 == 1) {
        fux->unk30 = 0;
    }
}

void PlotFlamerBitTA(NAS *nas) {
    EXTengine *eng;
    LaserBitFux *fux;
    s32 i;

    fux = (LaserBitFux *)nas->FUX;
    eng = nas->Engine;
    eng->unk5E = 1;
    eng->unk20 += 0x96;
    if (!(D_8004E0D8 & 7)) {
        Ovl5fSpawnBit1605(&eng->unk10, 2);
    }
    if (func_800BAFFC(nas, 0x14, 0x1306, 0xC) != 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        PlaySfx(0x4E4, NULL, NULL, 0x1000, 0x1E00, 0);
        for (i = 0; i < 3; i++) {
            if (D_800CAD50[fux->player].MainWeaponNas[i] == nas) {
                D_800CAD50[fux->player].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (D_800CB0FC != 0) {
        switch ((u32)Random(3)) {
        case 0:
            PlaySfx(0x4E1, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 1:
            PlaySfx(0x4E2, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 2:
            PlaySfx(0x4E3, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
            break;
        }
    }
    if (fux->unk30 == 1) {
        fux->unk30 = 0;
    }
}

void PlotFlubberTA(NAS *nas) {
    EXTengine *eng;
    FlubberFux *fux;
    EXTengine *e;
    s32 sfx;
    s32 i;

    eng = nas->Engine;
    fux = (FlubberFux *)nas->FUX;
    eng->unk5E = 1;
    eng->unk1C += 0x55;
    fux->vy += 0xC;
    eng->unk10.y += fux->vy;
    if (eng->unk10.y >= -0x64) {
        eng->unk10.y = -0x64;
        fux->vy = -0x3C;
        eng->unk10.y -= 0x3C;
        if (((Ovl5fModeView *)D_8005A614)->mode == 5 ||
            (((Ovl5fModeView *)D_8005A614)->mode == 6 && (((Ovl5fModeView *)D_8005A614)->flags & 0x80) &&
             D_800CACB4[((eng->unk10.z >> 9) * D_800CAFF4) + (eng->unk10.x >> 9)] != 0)) {
            switch ((u32)Random(4)) {
            case 0:
                sfx = 0x4F7;
                break;
            case 1:
                sfx = 0x4F8;
                break;
            case 2:
                sfx = 0x4F9;
                break;
            case 3:
                sfx = 0x4FA;
                break;
            default:
                goto skip1;
            }
            PlaySfx(sfx, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        skip1:
            Ovl5fSpawnBit1605(&eng->unk10, 0x12);
        } else {
            switch ((u32)Random(6)) {
            case 0:
                sfx = 0x4D2;
                break;
            case 1:
                sfx = 0x4D3;
                break;
            case 2:
                sfx = 0x4D4;
                break;
            case 3:
                sfx = 0x4D5;
                break;
            case 4:
                sfx = 0x4D6;
                break;
            case 5:
                sfx = 0x4D7;
                break;
            default:
                goto skip2;
            }
            PlaySfx(sfx, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        skip2:
            Ovl5fSpawnBit1605(&eng->unk10, 9);
        }
    }
    if (func_800BAFFC(nas, 0x1E, 0x1306, 0xB) != 0) {
        do {
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x4E5, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        } while (0);
        for (i = 0; i < 3; i++) {
            if (D_800CAD50[fux->player].MainWeaponNas[i] == nas) {
                D_800CAD50[fux->player].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (D_800CB0FC != 0) {
        switch ((u32)Random(3)) {
        case 0:
            sfx = 0x4DE;
            break;
        case 1:
            sfx = 0x4DF;
            break;
        case 2:
            sfx = 0x4E0;
            break;
        default:
            goto skip3;
        }
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    skip3:;
    }
    if (fux->unk30 == 1) {
        fux->unk30 = 0;
    }
    do {
        e = eng->Engine;
        if (e != NULL) {
            e->unk1E = eng->unk1E;
            e->unk10 = eng->unk10;
            e->unk10.y = 0;
        }
    } while (0);
}

void PlotSwirlyTA(NAS *nas) {
    EXTengine *eng;
    LaserBitFux *fux;
    subengine *sub;
    s32 sfx;
    s32 i;

    eng = nas->Engine;
    eng->unk5E = 1;
    eng->unk1C += 0x17C;
    eng->unk1E += 0x118;
    eng->unk20 += 0xB4;
    sub = eng->sub->next;
    sub->unk52 = 1;
    sub->unk10 += 0x46;
    sub->unk12 += 0x8C;
    sub->unk14 += 0xD2;
    fux = (LaserBitFux *)nas->FUX;
    if (func_800BAFFC(nas, 0x14, 0x1306, 0xD) != 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        PlaySfx(0x4E6, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (i = 0; i < 3; i++) {
            if (D_800CAD50[fux->player].MainWeaponNas[i] == nas) {
                D_800CAD50[fux->player].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (D_800CB0FC != 0) {
        switch ((u32)Random(3)) {
        case 0:
            sfx = 0x4DB;
            break;
        case 1:
            sfx = 0x4DC;
            break;
        case 2:
            sfx = 0x4DD;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:;
    }
    if (fux->unk30 == 1) {
        fux->unk30 = 0;
    }
}

void PlotShell(NAS *nas) {
    ShellFux *fux;
    EXTengine *eng;
    EXTengine *e2;
    SLONG_XYZ *pos;
    u8 *p;
    NAS *puff;
    Ovl5fExt1605 *ext;
    s32 sfx;

    fux = (ShellFux *)nas->FUX;
    eng = nas->Engine;
    fux->vy += 0xC;
    eng->unk10.y += fux->vy;
    if (eng->unk10.y > 0) {
        eng->unk10.y = 0;
        fux->vy = -0x54;
        eng->unk10.y -= 0x54;
        pos = &eng->unk10;
        p = D_8005365C;
        puff = SpawnNASById(pos->x, pos->y, pos->z, 0x1605, 0x11);
        if (puff != NULL) {
            ext = FindExtension(puff, 0x10000000);
            if (ext != NULL) {
                ext->unkC = 9;
                ext->unk18 = 0x100;
                if (p != NULL) {
                    ext->unk20 = 5;
                    puff->Engine->sub->unk6C = p;
                } else {
                    ext->unk20 = 1;
                }
                ext->unk28 = 0;
                puff->InitDelete(puff);
            }
        }
    }
    if (func_800BAFFC(nas, 0x3C, 0x1306, 4) != 0) {
        switch ((u32)Random(4)) {
        case 0:
        case 1:
            sfx = 0x490;
            break;
        case 2:
        case 3:
            sfx = 0x491;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:
        KillNAS(nas);
        return;
    }
    if (fux->unk30 == 1) {
        fux->unk30 = 0;
    }
    e2 = eng->Engine;
    if (e2 != NULL) {
        e2->unk1E = eng->unk1E;
        e2->unk10 = eng->unk10;
        e2->unk10.y = 0;
    }
}

void PlotLobber_TA(NAS *nas)
{
  EXTengine *eng;
  LobberFux *fux;
  EXTengine *e;
  EXTengine *e2;
  s32 x;
  s32 z;
  s32 d;
  s32 n;
  s32 lim;
  s32 i;
  s32 j;
  s32 lo;
  s32 fx;
  s32 fz;
  s32 flim;
  s32 hit;
  u16 name;

  eng = nas->Engine;
  fux = (LobberFux *) nas->FUX;
  eng->unk10.x += fux->vx;
  eng->unk10.z += fux->vz;
  fux->vy += 0xC;
  eng->unk10.y += fux->vy;
  fx = eng->unk10.x;
  fz = eng->unk10.z;
  flim = 0x180;
  for (j = 0; j < 4; j++)
  {
    if (j == fux->player)
    {
      continue;
    }
    name = (*(&Heroes[j])).hero_name;
    if ((name == 9) && (((((Ovl5fModeView *) D_8005A614)->mode != name) || (D_800CB210 != 0)) || (((u32) (j - 2)) >= 2)))
    {
      continue;
    }
    if (Heroes[j].hero_status & 0xC000)
    {
      continue;
    }
    e = Heroes[j].hero_eng;
    d = fx - e->unk10.x;
    if (d < (-0x180))
    {
      continue;
    }
    if (flim < d)
    {
      continue;
    }
    d = fz - e->unk10.z;
    lo = -0x180;
    if ((lo < d) && (d < 0x180))
    {
      hit = j;
      goto found;
    }
  }


  hit = -1;
  found:
  if (hit >= 0)
  {
    s32 hx;
    s32 hz;
    s32 r;
    s32 hd;
    r = 0x180;

    hx = eng->unk10.x;
    hz = eng->unk10.z;
    for (i = 0; i < 4; i++)
    {
      EXTengine *he;
      he = Heroes[i].hero_eng;
      hd = hx - he->unk10.x;
      if (hd < (-0x180))
      {
        continue;
      }
      if (r < hd)
      {
        continue;
      }
      hd = hz - he->unk10.z;
      if (hd < (-0x180))
      {
        continue;
      }
      if (r < hd)
      {
        continue;
      }
      func_800B6354(i, 0x3C);
    }


    goto boom;
  }


  if (((fux->vy > 0) && (eng->unk10.y >= (-0x103))) || ((D_8005A648 == 9) && (eng->unk10.z > 0x1000)))
  {
    eng->unk10.y = -0x104;
    if ((D_8005A648 == 9) && (fux->player != (-1)))
    {
      func_800C18F0(eng->unk10.x, eng->unk10.y, eng->unk10.z);
    }
    x = eng->unk10.x;
    z = eng->unk10.z;
    lim = 0x180;
    for (hit = 0; hit < 4; hit++)
    {
      name = Heroes[hit].hero_name;
      if ((name == 9) && (((((Ovl5fModeView *) D_8005A614)->mode != name) || (D_800CB210 != 0)) || (((u32) (hit - 2)) >= 2)))
      {
        continue;
      }
      if (Heroes[hit].hero_status & 0xC000)
      {
        continue;
      }
      e = Heroes[hit].hero_eng;
      d = x - e->unk10.x;
      if (d < (-0x180))
      {
        continue;
      }
      if (lim < d)
      {
        continue;
      }
      d = z - e->unk10.z;
      n = -0x180;
      if ((n < d) && (d < 0x180))
      {
        goto found2;
      }
    }

    hit = -1;
    found2:
    if (hit < 0)
    {
      goto puff;
    }

    func_800B6354(hit, 0x3C);
    boom:
    if (eng->sfx != 0)
    {
      StopSfx(eng->sfx);
    }

    switch ((u32) Random(4))
    {
      case 0:
        PlaySfx(0x4BA, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

      case 1:
        PlaySfx(0x4BB, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

      case 2:
        PlaySfx(0x4BC, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

      case 3:
        PlaySfx(0x4BD, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

    }

    puff:
    switch ((u32) Random(4))
    {
      case 0:

      case 1:
        PlaySfx(0x490, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

      case 2:

      case 3:
        PlaySfx(0x491, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

    }


    Ovl5fSpawnBit1605(&eng->unk10, 1);

    KillNAS(nas);
    return;
  }

  if (fux->unk30 == 1)
  {
    fux->unk30 = 0;
  }

  e2 = eng->Engine;
  if (e2 != 0)
  {
    e2->unk1E = eng->unk1E;
    e2->unk10 = eng->unk10;
    if (D_800CACFC[((eng->unk10.z >> 9) * D_800CAFF4) + (eng->unk10.x >> 9)] != 0)
    {
      e2->unk10.y = -0x100;
    }
    else
    {
      e2->unk10.y = 0;
    }
  }

}

void PlotRocketTA(NAS *nas)
{
    EXTengine *eng;
    LobberFux *fux;
    NAS *n;
    NAS *puff;
    Ovl5fExt1605 *ext;
    void *p;
    SLONG_XYZ *pos;
    s32 x;
    s32 z;
    s32 hit;
    s32 dmg;
    s32 ok;

    eng = nas->Engine;
    fux = (LobberFux *) nas->FUX;
    x = eng->unk10.x + fux->vx;
    z = eng->unk10.z + fux->vz;
    if (x < 0 || x >= D_800CB100 + 0x100) {
        goto boom;
    }
    if (Ovl5fRocketNear(x, z, 0xE0) != NULL) {
        goto boom;
    }
    if (D_8005A648 == 9) {
        ok = (u32) z < 0x1280;
    } else {
        if (z < 0) {
            goto boom;
        }
        ok = z < D_800CB104 + 0x100;
    }
    if (!ok) {
        goto boom;
    }
    if (Ovl5fRocketTile(x, z) != 0) {
        if (D_8005A648 == 7 || D_8005A648 == 9) {
            if (z < D_800CB104) {
                n = (NAS *) *(((z >> 9) * D_800CAFF4) + (x >> 9) + D_800CA34C);
                if (n != NULL) {
                    pos = &n->Engine->unk10;
                    p = *(void **) D_80058AD0;
                    puff = SpawnNASById(pos->x, pos->y, pos->z, 0x1605, 0x11);
                    if (puff != NULL) {
                        ext = FindExtension(puff, 0x10000000);
                        if (ext != NULL) {
                            ext->unkC = 0;
                            ext->unk18 = 0x100;
                            if (p != NULL) {
                                ext->unk20 = 5;
                                puff->Engine->sub->unk6C = p;
                            } else {
                                ext->unk20 = 1;
                            }
                            ext->unk28 = 0;
                            puff->InitDelete(puff);
                        }
                    }
                    Ovl5fRocketKillCell(x, z);
                }
            }
        }
        goto boom;
    }
    eng->unk10.x = x;
    eng->unk10.z = z;
    func_800BF548(eng);
    hit = Ovl5fRocketFindHero(x, z, 0x10E);
    if (hit < 0 || hit == fux->player) {
        goto puff;
    }
    if (((Ovl5fModeView *) D_8005A614)->mode != 9 || fux->player == -1) {
        dmg = 0x3C;
    } else {
        dmg = (((D_8005A614[0xB4] * 190) / 32 + 10) * 60) / 100;
    }
    func_800B6354(hit, dmg);
boom:
    SpawnNASById(x, eng->unk10.y, z, 0x1306, 0x11);
    KillNAS(nas);
    if (eng->sfx != NULL) {
        StopSfx(eng->sfx);
    }
    PlaySfx(0x43D, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    return;
puff:
    if (!(D_8004E0D8 & 1)) {
        Ovl5fSpawnBit1605(&eng->unk10, 0x11);
    }
}

void PlotMine(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    s32 i;
    s32 d;
    s32 x;
    s32 z;
    s32 mode;
    s32 t;
    s32 r;
    s32 n;
    u16 name;
    u8 *g;

    g = D_8005A614;
    eng = nas->Engine;
    if ((((Ovl5fModeView *)g)->mode == 8 &&
         D_800CACB4[((eng->unk10.z >> 9) * D_800CAFF4) + (eng->unk10.x >> 9)] != 0) ||
        --nas->UserInfo.status == 0) {
        PlaySfx(0x4EA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        func_800BD654(nas);
        for (i = 0; i < 8; i++) {
            if (D_800CB0C0[i] == nas) {
                D_800CB0C0[i] = NULL;
                break;
            }
        }
        r = 250;
        x = eng->unk10.x;
        z = eng->unk10.z;
        for (i = 0; i < 4; i++) {
            name = Heroes[i].hero_name;
            if (name == 9 &&
                (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
                 (u32)(i - 2) >= 2)) {
                continue;
            }
            if (Heroes[i].hero_status & 0xC000) {
                continue;
            }
            e = Heroes[i].hero_eng;
            d = x - e->unk10.x;
            if (d < -250) {
                continue;
            }
            if (r < d) {
                continue;
            }
            d = z - e->unk10.z;
            n = -250;
            if (n < d && d < 250) {
                goto found;
            }
        }
        i = -1;
    found:
        if (i >= 0) {
            if (((Ovl5fModeView *)D_8005A614)->mode == 8 && D_8005A614[i + 0x5C] == 0 &&
                (((Ovl5fGameView *)D_8005A614)->flags & 0x80)) {
                func_800B6354(i, 0x4B0);
            } else {
                func_800B6354(i, 0x3C);
            }
        }
        KillNAS(nas);
        return;
    }
    t = (u16)(nas->UserInfo.status / 30);
    if (t >= 4) {
        t = 3;
    }
    mode = ((Ovl5fModeView *)g)->mode;
    if ((mode == 8 && (((Ovl5fGameView *)g)->flags & 0x80)) || mode == 9) {
        nas->Engine->sub->unk74 = D_800C939C[t].v;
    } else {
        nas->Engine->sub->unk74 = D_800C938C[t].v;
    }
    if ((u16)(nas->UserInfo.status % 30) == 0) {
        PlaySfx(0x4EB, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void PlotWaterMine(NAS *nas) {
    s32 i;
    s32 j;
    TA_player *ta;
    EXTengine *eng;
    EXTengine *e;
    s32 t;
    s32 d;
    s32 hit;
    s32 x;
    s32 z;
    s32 r;
    s32 n;
    u16 name;

    t = nas->UserInfo.user2;
    eng = nas->Engine;
    eng->unk10.y += D_800CA44C[t & 0xF];
    nas->UserInfo.user2 = t + 1;
    nas->UserInfo.status = (nas->UserInfo.status + 4) & 0xFFF;
    eng->unk10.x += (D_80068BD4[(nas->UserInfo.status - 0x400) & 0xFFF].unk2 * 7) >> 12;
    eng->unk10.z += (D_80068BD4[nas->UserInfo.status & 0xFFF].unk2 * 7) >> 12;
    if (eng->unk10.x > D_800CB100) {
        eng->unk10.x = D_800CB100;
    }
    if (eng->unk10.x < 0x100) {
        eng->unk10.x = 0x100;
    }
    if (eng->unk10.z > D_800CB104) {
        eng->unk10.z = D_800CB104;
    }
    if (eng->unk10.z < 0x100) {
        eng->unk10.z = 0x100;
    }
    x = eng->unk10.x;
    z = eng->unk10.z;
    r = 400;
    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = x - e->unk10.x;
        if (d < -400) {
            continue;
        }
        if (r < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -400;
        if (n < d && d < 400) {
            hit = i;
            goto found;
        }
    }
    hit = -1;
found:
    if (nas->UserInfo.flags == 1) {
        if (hit < 0) {
            nas->UserInfo.flags = 0;
        }
    } else if (hit >= 0) {
        PlaySfx(0x4EA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (j = 0; j < 8; j++) {
            if (D_800CB0C0[j] == nas) {
                D_800CB0C0[j] = NULL;
                break;
            }
        }
        if (D_8005A614[hit + 0x5C] == 0) {
            if (((Ovl5fGameView *)D_8005A614)->flags & 0x80) {
                func_800B6354(hit, 0x4B0);
            } else {
                func_800B6354(hit, 0x3C);
            }
        } else {
            func_800B6354(hit, 0x3C);
        }
        ta = &D_800CAD50[hit];
        ta->SpeedX = 0;
        ta->SpeedZ = 0;
        func_800BD654(nas);
        KillNAS(nas);
    }
}

void PlotTorpedo(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    HeroInfo *hero;
    HeroInfo *h0;
    HeroInfo *hb;
    HeroInfo *h2;
    NAS *n;
    SLONG_XYZ *pos;
    u8 *p;
    Ovl5fExt1605 *ext;
    s32 player;
    s32 angle;
    s32 t;
    s32 dx;

    player = nas->UserInfo.user1;
    h0 = Heroes;
    hero = &h0[player];
    eng = nas->Engine;
    if (!(hero->hero_status & 0xA400) && hero->hero_health > 0) {
        t = nas->UserInfo.user2;
        eng->unk10.y += D_800CA44C[t & 0xF];
        nas->UserInfo.user2 = t + 1;
        he = hero->hero_eng;
        angle = get_angle(he->unk10.x - eng->unk10.x, he->unk10.z - eng->unk10.z);
        eng->unk1E = angle;
        eng->unk5E = 1;
        if (nas->UserInfo.status != 0) {
            nas->UserInfo.status--;
        } else {
            eng->unk10.x += (D_80068BD4[(angle - 0x400) & 0xFFF].unk2 * 9) >> 12;
            eng->unk10.z += (D_80068BD4[angle & 0xFFF].unk2 * 9) >> 12;
        }
        hb = Heroes;
        h2 = &hb[player];
        if (h2->hero_status & 0xFA00) {
            return;
        }
        if (h2->hero_immunity != 0) {
            return;
        }
        dx = eng->unk10.x;
        if ((u32)(dx - he->unk10.x + 0x15F) > 0x2BE) {
            return;
        }
        dx = eng->unk10.z;
        if ((u32)(dx - he->unk10.z + 0x15F) > 0x2BE) {
            return;
        }
        h2->hero_health = 1;
        D_800CAD50[player].Energy = 1;
        h2->hero_immunity = 0;
        func_800B6354(player, 0x4B0);
    }
    pos = &eng->unk10;
    PlaySfx(0x4EA, NULL, pos, 0x1000, 0x1E00, 0);
    p = D_8005365C;
    n = SpawnNASById(eng->unk10.x, pos->y, pos->z, 0x1605, 0x11);
    if (n != NULL) {
        ext = FindExtension(n, 0x10000000);
        if (ext != NULL) {
            ext->unkC = 0x10;
            ext->unk18 = 0x100;
            if (p != NULL) {
                ext->unk20 = 5;
                n->Engine->sub->unk6C = p;
            } else {
                ext->unk20 = 1;
            }
            ext->unk28 = 0;
            n->InitDelete(n);
        }
    }
    KillNAS(nas);
}

void func_800BD654(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    NAS *n;
    NAS *m;
    SLONG_XYZ *pos;
    u8 *p;
    Ovl5fExt1605 *ext;
    s32 i;
    s32 dx;
    s32 dz;
    s32 x;
    s32 z;

    eng = nas->Engine;
    switch ((s16)(D_8005A648 - 5)) {
    default:
    case 1:
    case 3:
        for (i = 0; i < 4; i++) {
            dx = D_800C935C[i].x;
            dz = D_800C935C[i].z;
            m = SpawnNASById(eng->unk10.x + dx, eng->unk10.y, eng->unk10.z + dz, 0x1304, 0x11);
            if (m != NULL) {
                m->UserInfo.user1 = dx;
                m->UserInfo.user2 = dz;
            }
        }
        break;
    case 0:
    case 2:
    case 4:
        pos = &eng->unk10;
        p = D_8005365C;
        n = SpawnNASById(pos->x, pos->y, pos->z, 0x1605, 0x11);
        if (n != NULL) {
            ext = FindExtension(n, 0x10000000);
            if (ext != NULL) {
                ext->unkC = 0x10;
                ext->unk18 = 0x100;
                if (p != NULL) {
                    ext->unk20 = 5;
                    n->Engine->sub->unk6C = p;
                } else {
                    ext->unk20 = 1;
                }
                ext->unk28 = 0;
                n->InitDelete(n);
            }
        }
        x = eng->unk10.x;
        z = eng->unk10.z;
        if (D_8005A648 == 9) {
            func_800C18F0(x, eng->unk10.y, z);
        }
        for (i = 0; i < 4; i++) {
            if (!(Heroes[i].hero_status & 0xFA00)) {
                he = Heroes[i].hero_eng;
                if ((u32)(x - he->unk10.x + 0x300) <= 0x600 && (u32)(z - he->unk10.z + 0x2FF) < 0x5FF) {
                    func_800B6354(i, 0x3C);
                }
            }
        }
        break;
    }
}

static inline s32 ShrapnelFindHero(s32 x, s32 z) {
    s32 i;
    s32 d;
    s32 r;
    s32 n;
    u16 name;
    EXTengine *e;

    r = 250;
    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = x - e->unk10.x;
        if (d < -250) {
            continue;
        }
        if (r < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -250;
        if (n < d && d < 250) {
            return i;
        }
    }
    return -1;
}

static inline u8 ShrapnelTile(s32 x, s32 z) {
    s32 i;

    i = ((z >> 9) * D_800CAFF4) + (x >> 9);
    return D_800CACB4[i];
}

void PlotShrapnelBit(NAS *nas) {
    EXTengine *eng;
    s32 x;
    s32 z;
    s32 dx;
    s32 dz;
    s32 hit;
    s32 sfx;

    eng = nas->Engine;
    SpawnNASById(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1305, 0x11);
    dz = nas->UserInfo.user2;
    z = eng->unk10.z + dz;
    dx = nas->UserInfo.user1;
    x = eng->unk10.x + dx;
    if (ShrapnelTile(x, z) != 0) {
        goto kill;
    }
    hit = ShrapnelFindHero(x, z);
    z += dz;
    if (hit < 0) {
        goto step;
    }
found:
    switch ((u32)Random(4)) {
    case 0:
        sfx = 0x4BA;
        goto play;
    case 1:
        sfx = 0x4BB;
        goto play;
    case 2:
        sfx = 0x4BC;
        goto play;
    case 3:
        sfx = 0x4BD;
    play:
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
    if (((Ovl5fModeView *)D_8005A614)->mode == 8 && D_8005A614[hit + 0x5C] == 0 &&
        (((Ovl5fGameView *)D_8005A614)->flags & 0x80)) {
        func_800B6354(hit, 0x4B0);
    } else {
        func_800B6354(hit, 0x3C);
    }
    goto kill;
step:
    x += dx;
    if (ShrapnelTile(x, z) != 0) {
        goto kill;
    }
    hit = ShrapnelFindHero(x, z);
    if (hit >= 0) {
        goto found;
    }
    eng->unk10.z = z;
    eng->unk10.x = x;
    nas->UserInfo.status++;
    if (D_800CB100 + 0x100 < eng->unk10.x || eng->unk10.x < 0x100 ||
        D_800CB104 + 0x100 < eng->unk10.z || eng->unk10.z < 0x100) {
        goto kill;
    }
    return;
kill:
    SpawnNASById(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1305, 0x11);
    KillNAS(nas);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotPickupGenerator_TA);

void InitHelicopterTA(NAS *nas) {
    HelicopterTAExt *ext;
    EXTengine *eng;
    EXTengine *e;
    subengine *sub;
    u8 *p;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        ext->unk30 = 3;
        eng = nas->Engine;
        if (++D_800CB0F4 & 1) {
            eng->unk1E = 0x400;
            eng->unk10.x = 0x1800;
            ext->unk10 = -0x40;
            eng->unk10.y = -0x400;
        } else {
            eng->unk1E = 0xC00;
            eng->unk10.x = -0x800;
            ext->unk10 = 0x40;
            eng->unk10.y = -0x400;
        }
        switch ((s16)(D_8005A648 - 5)) {
        case 0:
            eng->unk10.z = D_800CA600[Random(4)];
            break;
        case 1:
            eng->unk10.z = D_800CA610[Random(4)];
            break;
        case 2:
            eng->unk10.z = D_800CA620[Random(4)];
            break;
        case 3:
            eng->unk10.z = D_800CA630[Random(4)];
            break;
        case 4:
            eng->unk10.z = D_800CA640[Random(4)];
            break;
        }
        p = D_80053694;
        sub = eng->sub;
        sub->unk6C = p;
        sub->unk74 = 0x4080;
        eng->unk5D = 1;
        eng->unk5E = 1;
        nas->UserInfo.user2 = 0;
        PlaySfx(0x4F6, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        e = AddEngine(nas, 1);
        if (e != NULL) {
            e->unk10.x = eng->unk10.x;
            e->unk10.y = 0;
            e->unk10.z = eng->unk10.z;
            e->sub->unk51 = 2;
            e->sub->unk6C = p;
            e->sub->unk74 = 0x200F;
            e->sub->flags |= 0x40000041;
            e->sub->unk76 = 0x800;
            e->sub->unk50 = 1;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

void PlotHelicopterTA(NAS *nas) {
    NAS **slot;
    Ovl5fModeView *g;
    HeliFux *fux;
    EXTengine *eng;
    EXTengine *e2;
    subengine *sub;
    NAS *p;
    s32 i;
    s32 n;
    s32 x;
    u32 chance;
    u16 st;
    NAS **s;

    fux = (HeliFux *)nas->FUX;
    eng = nas->Engine;
    sub = eng->sub;
    if (D_800CB0F0 != 0) {
        eng->unk10.y -= 0x32;
    }
    eng->unk10.x += fux->vx;
    if (--nas->UserInfo.user2 < 0) {
        nas->UserInfo.user2 = 7;
    }
    sub->unk74 = D_800CA650[nas->UserInfo.user2].v;
    e2 = eng->Engine;
    if (e2 != NULL) {
        e2->unk1E = eng->unk1E;
        e2->unk10 = eng->unk10;
        e2->unk10.y = 0;
    }
    x = eng->unk10.x;
    if ((u32)(x + 0x800) > 0x2000) {
        KillNAS(nas);
        return;
    }
    if (fux->unk30 != 3 || D_800CB0F0 != 0 || (x & 0x1FF) != 0x100) {
        return;
    }
    for (i = 0, s = D_800CAD44; i < 3; i++, s++) {
        p = *s;
        if (p != NULL) {
            if (i) {
                if (p->Engine->unk10.x != eng->unk10.x || p->Engine->unk10.z != eng->unk10.z) {
                    break;
                }
            } else {
                if (p->Engine->unk10.x != eng->unk10.x || p->Engine->unk10.z != eng->unk10.z) {
                    break;
                }
            }
            return;
        }
    }
    if ((u32)(eng->unk10.x - 2) >= 0xFFD) {
        return;
    }
    if ((u32)Random(0x8000) >= 0x2800) {
        return;
    }
    chance = 0;
    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9 && D_800CAD50[i].Energy <= 0) {
            chance += 2000;
        }
    }
    n = 0;
    g = (Ovl5fModeView *)D_8005A614;
    slot = D_800CAD44;
loop:
    if (n < 3) {
        if (*slot == NULL) {
            p = SpawnNASById(eng->unk10.x, eng->unk10.y - 0xC8, eng->unk10.z + 0x3C, 0x1308, 0x11);
            if (p == NULL) {
                return;
            }
            fux->unk30 = 0;
            if (g->mode) {
                *slot = p;
            } else if (*slot) {
                *slot = p;
            } else {
                *slot = p;
            }
            if (g->mode != 9 && (u32)Random(7000) < chance) {
                st = 6;
            } else {
                switch (g->mode) {
                case 5:
                default:
                    if ((u32)Random(1000) >= 0x1F5) {
                        st = 5;
                    } else {
                        st = 4;
                    }
                    break;
                case 7:
                    switch ((u32)Random(3)) {
                    case 0:
                        st = 5;
                        break;
                    case 1:
                        st = 4;
                        break;
                    case 2:
                        st = 2;
                        break;
                    case 3:
                        st = 2;
                        break;
                    default:
                        st = 5;
                        break;
                    }
                    break;
                case 9:
                    switch ((u32)Random(2)) {
                    default:
                        st = 5;
                        break;
                    case 0:
                        st = 5;
                        break;
                    case 1:
                        st = 3;
                        break;
                    }
                    break;
                }
            }
            p->UserInfo.status = st;
            return;
        }
        slot++;
        n++;
        goto loop;
    }
}

void InitPickupTA(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    PickupTAExt *ext;
    subengine *sub;
    u8 *p;
    s32 y;

    eng = nas->Engine;
    switch ((u32)Random(4)) {
    default:
    case 0:
        PlaySfx(0x4B0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x4B1, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x4B2, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x4B3, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
    nas->UserInfo.user2 = 0;
    nas->UserInfo.user1 = 5;
    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        ext->unk14 = 0;
        nas->FUX = ext;
        p = D_8005365C;
        eng = nas->Engine;
        sub = eng->sub;
        sub->unk6C = p;
        sub->unk74 = 0x202B;
        eng->unk5D = 1;
        e = AddEngine(nas, 1);
        if (e != NULL) {
            e->unk10.x = eng->unk10.x;
            e->unk10.y = 0;
            e->unk10.z = eng->unk10.z;
            e->sub->unk51 = 2;
            e->sub->unk6C = p;
            e->sub->unk74 = 0x202A;
            e->sub->flags |= 0x40000041;
            e->sub->unk76 = 0x800;
            e->sub->unk50 = 1;
            y = eng->unk10.y + 0x1000;
            e->sub->unk20.x = y;
            e->sub->unk20.z = y;
            e->sub->unk52 = 1;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotPickupTA);

static inline void SpawnPuff1605(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;

    p = D_8005365C;
    nas = SpawnNASById(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = kind;
            ext->unk18 = 0x100;
            if (p != NULL) {
                ext->unk20 = 5;
                nas->Engine->sub->unk6C = p;
            } else {
                ext->unk20 = 1;
            }
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
    }
}

s32 func_800BF548(EXTengine *eng) {
    s32 i;
    s32 j;
    NAS *pickup;
    EXTengine *pe;
    NAS **pp;
    NAS **tbl;
    s32 x;
    s32 y;
    s32 z;

    x = eng->unk10.x;
    y = eng->unk10.y;
    z = eng->unk10.z;
    for (i = 0;; i++) {
        if (i >= 3) {
            return 0;
        }
        pickup = D_800CAD44[i];
        if (pickup == NULL) {
            continue;
        }
        pe = pickup->Engine;
        if ((u32)(pe->unk10.x - x + 0xFF) < 0x1FF
            && (u32)(pe->unk10.z - z + 0xFF) < 0x1FF
            && (u32)(pe->unk10.y - y + 0xFF) < 0x1FF) {
            SpawnPuff1605(&pe->unk10, 0x10);
            PlaySfx(0x4B8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            for (j = 0; j < 3; j++) {
                if (D_800CAD44[j] == pickup) {
                    D_800CAD44[j] = NULL;
                    break;
                }
            }
            KillNAS(pickup);
            return 1;
        }
    }
}

void PlotRock_TA(NAS *nas) {
    EXTengine *e;

    if (nas->UserInfo.status == 1) {
        e = nas->Engine;
        nas->UserInfo.status = 0;
        {
            s32 z = e->unk10.z >> 9;
            s32 x = e->unk10.x >> 9;
            u8 v = ((x << 5) | 0x10) + ((z * 2) + 1);
            D_800CACB4[(z * D_800CAFF4) + x] = v;
            D_800CACFC[(z * D_800CAFF4) + x] = v;
        }
        {
            s32 z = nas->Engine->unk10.z >> 9;
            s32 x = nas->Engine->unk10.x >> 9;
            u8 v = ((x << 5) | 0x10) + ((z * 2) + 1);
            s32 i = (z * D_800CAFF4) + x;
            D_800CACB4[i] = v;
            D_800CACFC[i] = v;
            D_800CA34C[i] = (s32)nas;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotPeriscope);

void PlotPeriscopeGun(NAS *nas) {
    EXTengine *e;
    EXTengine *se;
    NAS *shell;
    PeriscopeShellExt *ext;
    s32 dx;
    s32 dz;

    if (D_800CB0F0 != 0) {
        KillNAS(nas);
        return;
    }
    e = nas->Engine;
    e->sub->unk74++;
    switch (nas->Engine->sub->unk74) {
    case 0x4081:
        shell = SpawnNASById(e->unk10.x, e->unk10.y - 200, e->unk10.z, 0x130B, 0x11);
        if (shell == NULL) {
            return;
        }
        PlaySfx(0x4F3, &shell->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
        ext = AddExtension(shell, 0x38, 0x10000000);
        if (ext == NULL) {
            KillNAS(shell);
            return;
        }
        shell->FUX = (EXT *)ext;
        se = shell->Engine;
        dx = -(D_80068BD4[(e->unk1E - 0x400) & 0xFFF].unk2 * 620);
        se->unk10.x += dx >> 12;
        dz = -(D_80068BD4[(u16)e->unk1E & 0xFFF].unk2 * 620);
        se->unk10.z += dz >> 12;
        ext->vy = -150;
        ext->unk2C = -1;
        ext->vx = dx >> 16;
        ext->vz = dz >> 16;
        ext->unk30 = 1;
        break;
    case 0x4087:
        KillNAS(nas);
        break;
    }
}

void PlotBoatField(NAS *nas) {
    EXTengine *src;
    EXTengine *dst;
    s32 *pos;
    u8 *p;
    NAS *n;
    Ovl5fExt1605 *ext;
    u32 t;

    src = Heroes[nas->UserInfo.user1].hero_eng;
    dst = nas->Engine;
    dst->unk10.x = src->unk10.x;
    dst->unk10.z = src->unk10.z;
    t = nas->UserInfo.user2;
    nas->UserInfo.user2 = t + 1;
    switch (t) {
    case 0:
    case 4:
    case 9:
    case 14:
        pos = (s32 *)&dst->unk10;
        p = D_80053774;
        n = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
        if (n != NULL) {
            ext = FindExtension(n, 0x10000000);
            if (ext != NULL) {
                ext->unkC = 4;
                ext->unk18 = 0x100;
                if (p != NULL) {
                    ext->unk20 = 5;
                    n->Engine->sub->unk6C = p;
                } else {
                    ext->unk20 = 1;
                }
                ext->unk28 = 0;
                n->InitDelete(n);
            }
        }
        break;
    case 29:
        KillNAS(nas);
        break;
    }
}

void PlotPillarControl(NAS *nas) {
    s32 *p;
    s32 delay;
    s32 x;
    s32 z;
    s32 v;
    NAS *obj;

    if (D_800CB0F0 == 0 && --nas->UserInfo.flags == 0) {
        PlaySfx(0x501, NULL, NULL, 0x1000, 0x1E00, 0);
        p = D_800CA8C4[D_800CB204++];
        delay = *p++;
        while ((v = *p++) >= 0) {
            x = (v << 9) + 0x100;
            z = (*p++ << 9) + 0x100;
            obj = SpawnNASById(x, 0, z, 0x1311, 0x11);
            if (obj != NULL) {
                obj->Engine->sub->unk74 = *p++;
                obj->UserInfo.user2 = delay;
                obj = SpawnNASById(x, 0, z, 0x1312, 0x11);
                if (obj != NULL) {
                    obj->Engine->sub->unk74 = 0x4000;
                    obj->UserInfo.user1 = 6;
                    obj->UserInfo.status = 0;
                }
            }
        }
        if (D_800CB204 < 0x20) {
            nas->UserInfo.flags = delay + 0x46;
        } else {
            KillNAS(nas);
        }
    }
}

void PlotPillar(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    NAS *obj;
    s32 i;
    s32 d;
    s32 x;
    s32 z;
    s32 r;
    s32 n;
    s32 hit;
    u16 name;

    eng = nas->Engine;
    switch (nas->UserInfo.user1) {
    case 1:
        if (--nas->UserInfo.status != 0) {
            return;
        }
        r = 0x1FF;
        x = eng->unk10.x;
        z = eng->unk10.z;
        for (i = 0; i < 4; i++) {
            name = Heroes[i].hero_name;
            if (name == 9 &&
                (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
                 (u32)(i - 2) >= 2)) {
                continue;
            }
            if (Heroes[i].hero_status & 0xC000) {
                continue;
            }
            e = Heroes[i].hero_eng;
            d = x - e->unk10.x;
            if (d < -0x1FF) {
                continue;
            }
            if (r < d) {
                continue;
            }
            d = z - e->unk10.z;
            n = -0x1FF;
            if (n < d && d < 0x1FF) {
                hit = i;
                goto found;
            }
        }
        hit = -1;
    found:
        if (hit < 0) {
            if (D_800CACB4[((eng->unk10.z >> 9) * D_800CAFF4) + (eng->unk10.x >> 9)] != 0) {
                KillNAS(nas);
                return;
            }
            nas->UserInfo.user1 = 4;
            {
                s32 z = eng->unk10.z >> 9;
                s32 x = eng->unk10.x >> 9;
                u8 v = ((x << 5) | 0x10) + ((z * 2) + 1);
                D_800CACB4[(z * D_800CAFF4) + x] = v;
                D_800CACFC[(z * D_800CAFF4) + x] = v;
            }
            eng->sub->unk68 = 0;
            eng->sub->flags &= ~0x02000000;
            func_800BF548(eng);
            return;
        }
        if (nas->UserInfo.user2 >= 2) {
            nas->UserInfo.user2--;
        }
        nas->UserInfo.status++;
        return;
    case 4:
        eng->unk10.y -= 0x1E;
        if (eng->unk10.y <= 0) {
            eng->unk10.y = 0;
            nas->UserInfo.user1 = 0;
        }
        return;
    case 0:
    case 3:
    default:
        if (D_800CB0F0 == 0 && --nas->UserInfo.user2 <= 0) {
            nas->UserInfo.user1 = 5;
            eng->sub->unk68 = 0x20;
            D_800CB0EC = 2;
            eng->sub->flags |= 0x02000000;
        }
        return;
    case 5:
        eng->unk10.y += 0x1E;
        if (eng->unk10.y >= 0xB8) {
            obj = SpawnNASById(eng->unk10.x, 0, eng->unk10.z, 0x1312, 0x11);
            if (obj != NULL) {
                obj->UserInfo.user1 = 7;
                obj->UserInfo.status = 3;
                obj->Engine->sub->unk74 = 0x400F;
            }
            {
                s32 z = eng->unk10.z >> 9;
                s32 x = eng->unk10.x >> 9;
                D_800CACB4[(z * D_800CAFF4) + x] = 0;
                D_800CACFC[(z * D_800CAFF4) + x] = 0;
            }
            nas->UserInfo.user1 = 2;
            nas->UserInfo.status = 4;
        }
        return;
    case 2:
        if (--nas->UserInfo.status == 0) {
            KillNAS(nas);
        }
        return;
    }
}

static inline s32 TotemFindHero(s32 x, s32 z, s32 r) {
    s32 i;
    s32 d;
    u16 name;
    EXTengine *e;

    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = x - e->unk10.x;
        if (d < -r) {
            continue;
        }
        if (r < d) {
            continue;
        }
        d = z - e->unk10.z;
        if (-r < d && d < r) {
            return i;
        }
    }
    return -1;
}

void PlotTotemPole(NAS *nas) {
    TotemFux *fux;
    EXTengine *eng;
    EXTengine *e;
    s32 i;

    fux = (TotemFux *)nas->FUX;
    eng = nas->Engine;
    switch (fux->state) {
    case 1:
        if (--fux->timer > 0) {
            return;
        }
        if (((Ovl5fGameView *)D_8005A614)->flags & 0x80) {
            fux->state = 0x24;
            PlaySfx(0x502, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
            nas->UserInfo.user2 = 0x1E;
            return;
        }
        if (TotemFindHero(0x590, 0x400, 0x200) < 0) {
            fux->timer = 0x12C;
            fux->state = 0x11;
            return;
        }
        fux->timer++;
        return;
    case 0x11:
        if (--fux->timer > 0) {
            return;
        }
        if (TotemFindHero(0xA40, 0x440, 0x200) >= 0) {
            fux->timer++;
            return;
        }
        fux->state = 0;
        return;
    case 0:
    case 2:
    default:
        return;
    case 0x24:
        if (nas->UserInfo.user2 > 0) {
            nas->UserInfo.user2--;
            return;
        }
        eng->unk5E = 1;
        eng->unk1E += 0x22;
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_name == 9) {
                continue;
            }
            if (Heroes[i].hero_status & 0xFA00) {
                continue;
            }
            if (D_8005A614[i + 0x5C] != 0) {
                continue;
            }
            e = Heroes[i].hero_eng;
            if ((u32)(((eng->unk1E - get_angle(eng->unk10.x - e->unk10.x, eng->unk10.z - e->unk10.z)) & 0xFFF) - 0x15) < 0xFD7) {
                continue;
            }
            fux->state = 0xD;
            fux->timer = 0;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x503, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        return;
    }
}

void func_800C0C68(NAS *nas, s32 id) {
    TotemExt *ext;
    EXTengine *eng;
    subengine *sub;
    s32 i;
    NAS **p;

    nas->UserInfo.user1 = 1;
    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        goto kill;
    }
    nas->FUX = (EXT *)ext;
    ext->unk30 = 1;
    eng = nas->Engine;
    sub = eng->sub;
    eng->unk5D = 0;
    eng->unk1E = 0;
    sub->unk6C = D_800537AC;
    if (id == 0x131B) {
        sub->unk74 = 0x4180;
        ext->unk1C = 0;
    } else {
        sub->unk74 = 0x4180;
        ext->unk1C = 0xD;
    }
    ext->unk20 = D_800C9B94[ext->unk1C][0];
    ext->unk24 = *(s32 *)((u8 *)D_800C9B94 + ((ext->unk1C * 8) | 4));
    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    for (i = 0; i < 8; i++) {
        if (D_800CB264[i] == NULL) {
            D_800CB264[i] = nas;
            break;
        }
    }
    if (nas->Failure != 0) {
kill:
        KillNAS(nas);
    }
}

void func_800C0DA0(EXTengine *arg0) {
    NAS *nas;
    Ovl5fExt1605 *ext;
    SLONG_XYZ *pos;
    u8 *p;
    s32 saved;
    s32 x;
    s32 z;
    s32 d;
    s32 i;
    s32 lim;
    HeroInfo *h;

    saved = arg0->unk10.y;
    arg0->unk10.y = 0x12C;
    pos = &arg0->unk10;
    p = D_800537AC;
    nas = SpawnNASById(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = 0;
            ext->unk18 = 0x100;
            if (p != NULL) {
                ext->unk20 = 5;
                nas->Engine->sub->unk6C = p;
            } else {
                ext->unk20 = 1;
            }
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
    }
    lim = 0x300;
    i = 0;
    h = Heroes;
    x = arg0->unk10.x;
    z = arg0->unk10.z;
    for (; i < 4; i++, h++) {
        d = x - h->hero_eng->unk10.x;
        if (d >= -0x300 && d <= lim) {
            d = z - h->hero_eng->unk10.z;
            if (d >= -0x300 && d <= lim) {
                func_800B6354(i, 0x3C);
            }
        }
    }
    arg0->unk10.y = saved;
    PlaySfx(0x505, NULL, NULL, 0x1000, 0x1E00, 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C0F24);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C18F0);

void func_800C1F44(void) {
    D_800CB224[0] = SpawnNASById(0x369, 0x7D0, 0x10E0, 0x1315, 0x11);
    D_800CB224[1] = SpawnNASById(0x900, 0x7D0, 0x10E0, 0x1315, 0x11);
    D_800CB224[2] = SpawnNASById(0xE97, 0x7D0, 0x10E0, 0x1315, 0x11);
    D_800CB230[0] = SpawnNASById(0x300, 0x7D0, 0x10E0, 0x1316, 0x11);
    D_800CB230[1] = SpawnNASById(0x700, 0x7D0, 0x10E0, 0x1316, 0x11);
    D_800CB230[2] = SpawnNASById(0xB00, 0x7D0, 0x10E0, 0x1316, 0x11);
    D_800CB230[3] = SpawnNASById(0xF00, 0x7D0, 0x10E0, 0x1316, 0x11);
    D_800CB240[0] = SpawnNASById(0x300, 0x7D0, 0x1200, 0x1317, 0x11);
    D_800CB240[1] = SpawnNASById(0xF00, 0x7D0, 0x1200, 0x1317, 0x11);
}

void InitTankBoss(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    JoeExt *ext;
    u8 *base;
    u8 *model;

    func_80086868();
    D_800CB208 = nas;
    eng = nas->Engine;
    D_800CB20C = eng;
    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    model = TANK_MODEL(57);
    nas->FUX = (EXT *)ext;
    sub = eng->sub;
    sub->unk6C = model;
    sub->unk74 = 0x2002;
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    eng->unk5D = 0;
    eng->unk10.y = 0x7D0;
    ext->unk30 = D_800CB25C;
    D_800CB210 = D_800CB25C;
    switch (D_800CB25C) {
    case 1:
        base = D_80052A54;
        if (TANK_MODEL_ON(base, 59)) {
            func_80016F54(TANK_MODEL(59));
        }
        if (!TANK_MODEL_ON(base, 58)) {
            func_8001612C(TANK_MODEL(58), 0);
        }
        if (!TANK_MODEL_ON(base, 57)) {
            func_8001612C(TANK_MODEL(57), 0);
        }
        ext->unk28 = 0x5A;
        D_800CB25C = 0x2C;
        func_800C1F44();
        break;
    case 0x2C:
        base = D_80052A54;
        if (TANK_MODEL_ON(base, 59)) {
            func_80016F54(TANK_MODEL(59));
        }
        if (TANK_MODEL_ON(base, 58)) {
            func_80016F54(TANK_MODEL(58));
        }
        if (!TANK_MODEL_ON(base, 57)) {
            func_8001612C(TANK_MODEL(57), 0);
        }
        ext->unk28 = 0x3C;
        func_800C1F44();
        break;
    case 0x3A:
        base = D_80052A54;
        if (TANK_MODEL_ON(base, 58)) {
            func_80016F54(TANK_MODEL(58));
        }
        if (TANK_MODEL_ON(base, 57)) {
            func_80016F54(TANK_MODEL(57));
        }
        if (!TANK_MODEL_ON(base, 59)) {
            func_8001612C(TANK_MODEL(59), 0);
        }
        ext->unk28 = 0x3C;
        break;
    }
    if (nas->Failure) {
        KillNAS(nas);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotTankBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C2940);

void func_800C30C0(EXTengine *arg0) {
    NAS *shell;
    PeriscopeShellExt *ext;
    EXTengine *se;
    NAS *n;
    subengine *sub;
    s32 dz;
    s32 ang;

    shell = SpawnNASById(arg0->unk10.x, arg0->unk10.y - 0xF0, arg0->unk10.z, 0x130E, 0x11);
    if (shell == NULL) {
        return;
    }
    ext = (PeriscopeShellExt *)shell->FUX;
    se = shell->Engine;
    ext->unk28 *= 2;
    PlaySfx(0x4CE, &se->sfx, NULL, 0x1000, 0x1E00, 0);
    se->unk10.x += -(D_80068BD4[(arg0->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12;
    se->unk10.z += -(D_80068BD4[(u16)arg0->unk1E & 0xFFF].unk2 * 400) >> 12;
    se->unk1E = arg0->unk1E;
    ext->vx = -(D_80068BD4[(arg0->unk1E - 0x400) & 0xFFF].unk2 * 58) >> 12;
    ext->vy = -0x10;
    dz = D_80068BD4[(u16)arg0->unk1E & 0xFFF].unk2;
    ext->unk30 = 1;
    ext->unk2C = -1;
    ext->vz = -(dz * 58) >> 12;
loop:
    if (se->unk10.z > D_800CB104 + 0x100) {
        se->unk10.z -= 0x10;
        goto loop;
    }
    n = Ovl5fSpawnBit1605Nas(&shell->Engine->unk10, 6);
    if (n != NULL) {
        ang = arg0->unk1E;
        n->Engine->unk1E = ang;
        n->Engine->unk5E = 1;
        for (sub = n->Engine->sub; sub != NULL; sub = sub->next) {
            sub->flags |= 0x20000000;
        }
    }
}

void PlotTankBossCrashTurret(NAS *nas) {
    EXTengine *eng;
    s32 a;
    s32 t;
    s32 u;
    s32 r;
    s32 v;
    s32 k;
    u16 *tbl;

    eng = nas->Engine;
    eng->unk10.y = D_800CB20C->unk10.y - 0x46;
    switch (nas->UserInfo.status) {
    case 4:
        nas->UserInfo.user2--;
        if (D_800CB210 == 0x2E) {
            nas->UserInfo.status = 1;
        }
        break;
    case 1:
        eng->unk10.z -= 0x30;
        nas->UserInfo.flags -= 0x30;
        if (nas->UserInfo.flags > 1000) {
            nas->UserInfo.flags = 0;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 39;
            nas->UserInfo.user1 = 0;
        }
        break;
    case 39:
        a = eng->unk10.x;
        switch (a) {
        case 0x300:
        default:
            a = 200;
            break;
        case 0xE97:
            a = -200;
            break;
        case 0x900:
            a = 0x200;
            break;
        }
        a -= eng->unk1E;
        a = (a << 20) >> 20;
        if (a >= 0) {
            if (a < 20) {
                ((Ovl5fTurretFireFn)func_800C30C0)(eng, 10, -1);
                nas->UserInfo.status = 13;
                break;
            }
            eng->unk1E += 20;
        } else {
            if (a >= -19) {
                ((Ovl5fTurretFireFn)func_800C30C0)(eng, 10, -1);
                nas->UserInfo.status = 13;
                break;
            }
            eng->unk1E -= 20;
        }
        eng->unk5E = 1;
        break;
    case 13:
        k = nas->UserInfo.user2;
        u = k + 1;
        nas->UserInfo.user2 = u;
        if (nas->UserInfo.flags != 0) {
            tbl = D_800CA9BC;
        } else {
            tbl = D_800CA994;
        }
        u *= 2;
        u += (s32)tbl;
        v = *(u16 *)u;
        nas->UserInfo.user2 = k + 2;
        if (v == 0) {
            if (nas->UserInfo.flags != 0) {
                eng->sub->unk74 = 0x2006;
            } else {
                eng->sub->unk74 = 0x2003;
            }
            nas->UserInfo.user2 = 60;
            nas->UserInfo.status = 0;
            eng->unk1E &= 0xFFF;
        } else {
            eng->sub->unk74 = v;
        }
        break;
    case 0:
        if (nas->UserInfo.flags != 2) {
            if (eng->unk1E <= 16) {
                nas->UserInfo.user2 = 0;
                nas->UserInfo.status = 14;
            } else {
                if (eng->unk1E < 0x800) {
                    eng->unk1E -= 16;
                } else {
                    eng->unk1E += 16;
                }
                eng->unk5E = 1;
                eng->unk1E &= 0xFFF;
            }
        }
        break;
    case 14:
        if (nas->UserInfo.user1 & 1) {
            eng->unk1E += 16;
        } else {
            eng->unk1E -= 16;
        }
        eng->unk5E = 1;
        t = eng->unk1E &= 0xFFF;
        if ((u32)(t - 900) < 0x8F9U) {
            nas->UserInfo.user2 = 0;
            if (nas->UserInfo.user1 & 1) {
                nas->UserInfo.status = 15;
            } else {
                nas->UserInfo.status = 16;
            }
        }
        break;
    case 15:
        do {
            eng->unk1E = (eng->unk1E - 16) & 0xFFF;
        } while (0);
        u = eng->unk1E;
        eng->unk5E = 1;
        if ((u32)(u - 900) < 0x8F9U) {
            eng->unk1E = u + 16;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 16;
            break;
        }
        r = func_800C2940(eng);
        if (r >= 0) {
            nas->UserInfo.user1++;
            if (nas->UserInfo.user1 & 1) {
                ((Ovl5fTurretFireFn)func_800C30C0)(eng, 10, r);
            } else {
                ((Ovl5fTurretFireFn)func_800C30C0)(eng, 3, r);
            }
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 13;
        }
        break;
    case 16:
        do {
            eng->unk1E = (eng->unk1E + 16) & 0xFFF;
        } while (0);
        t = eng->unk1E;
        eng->unk5E = 1;
        if ((u32)(t - 900) < 0x8F9U) {
            eng->unk1E = t - 16;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 15;
            break;
        }
        r = func_800C2940(eng);
        if (r >= 0) {
            nas->UserInfo.user1++;
            if (nas->UserInfo.user1 & 1) {
                ((Ovl5fTurretFireFn)func_800C30C0)(eng, 10, r);
            } else {
                ((Ovl5fTurretFireFn)func_800C30C0)(eng, 3, r);
            }
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 13;
        }
        break;
    }
}

void func_800C3778(EXTengine *arg0) {
    NAS *shell;
    PeriscopeShellExt *ext;
    EXTengine *se;
    NAS *n;
    subengine *sub;
    s32 dz;
    s32 ang;

    shell = SpawnNASById(arg0->unk10.x, arg0->unk10.y - 0x113, arg0->unk10.z, 0x130C, 0x11);
    if (shell == NULL) {
        return;
    }
    ext = (PeriscopeShellExt *)shell->FUX;
    ext->unk28 = ext->unk28 * 5 / 3;
    se = shell->Engine;
    PlaySfx(0x4CC, &se->sfx, NULL, 0x1000, 0x1E00, 0);
    se->unk10.x += -(D_80068BD4[(arg0->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12;
    se->unk10.z += -(D_80068BD4[(u16)arg0->unk1E & 0xFFF].unk2 * 400) >> 12;
    se->unk1E = arg0->unk1E;
    ext->vx = -(D_80068BD4[(arg0->unk1E - 0x400) & 0xFFF].unk2 * 74) >> 12;
    dz = D_80068BD4[(u16)arg0->unk1E & 0xFFF].unk2;
    ext->unk30 = 1;
    ext->unk2C = -1;
    ext->vz = -(dz * 74) >> 12;
loop:
    if (se->unk10.z > D_800CB104 + 0x100) {
        se->unk10.z -= 0x10;
        goto loop;
    }
    n = Ovl5fSpawnBit1605Nas(&shell->Engine->unk10, 7);
    if (n != NULL) {
        ang = arg0->unk1E;
        n->Engine->unk1E = ang;
        n->Engine->unk5E = 1;
        for (sub = n->Engine->sub; sub != NULL; sub = sub->next) {
            sub->flags |= 0x20000000;
        }
    }
}

void PlotTankBossCortexTurret(NAS *nas) {
    EXTengine *eng;
    s32 a;
    s32 t;
    s32 u;

    eng = nas->Engine;
    eng->unk10.y = D_800CB20C->unk10.y - 0x2B6;
    switch (nas->UserInfo.status) {
    case 17:
        if (D_800CB210 == 0x32) {
            eng->unk5E = 1;
            eng->unk1C += 20;
        } else if (D_800CB210 == 0x33) {
            nas->UserInfo.flags = 0;
            nas->UserInfo.status = 39;
            nas->UserInfo.user1 = 0;
        }
        break;
    case 39:
        a = eng->unk10.x;
        switch (a) {
        case 0x300:
        default:
            a = 300;
            break;
        case 0xF00:
            a = -300;
            break;
        case 0x700:
            a = -200;
            break;
        case 0xB00:
            a = 200;
            break;
        }
        a -= eng->unk1E;
        a = (a << 20) >> 20;
        if (a >= 0) {
            if (a < 20) {
                func_800C3778(eng);
                nas->UserInfo.status = 13;
                nas->UserInfo.user2 = 0;
                break;
            }
            eng->unk1E += 16;
        } else {
            if (a >= -19) {
                func_800C3778(eng);
                nas->UserInfo.status = 13;
                break;
            }
            eng->unk1E -= 16;
        }
        eng->unk5E = 1;
        break;
    case 13:
        nas->UserInfo.user2 = 60;
        nas->UserInfo.status = 0;
        break;
    case 0:
        if (nas->UserInfo.flags != 2) {
            if (eng->unk1E <= 16) {
                nas->UserInfo.user2 = 0;
                nas->UserInfo.status = 14;
            } else {
                if (eng->unk1E < 0x800) {
                    eng->unk1E -= 16;
                } else {
                    eng->unk1E += 16;
                }
                eng->unk5E = 1;
                eng->unk1E &= 0xFFF;
            }
        }
        break;
    case 14:
        if (nas->UserInfo.user1 & 1) {
            eng->unk1E += 16;
        } else {
            eng->unk1E -= 16;
        }
        eng->unk5E = 1;
        t = eng->unk1E &= 0xFFF;
        if ((u32)(t - 900) < 0x8F9U) {
            nas->UserInfo.user2 = 0;
            if (nas->UserInfo.user1 & 1) {
                nas->UserInfo.status = 15;
            } else {
                nas->UserInfo.status = 16;
            }
        }
        break;
    case 15:
        do {
            eng->unk1E = (eng->unk1E - 16) & 0xFFF;
        } while (0);
        u = eng->unk1E;
        eng->unk5E = 1;
        if ((u32)(u - 900) < 0x8F9U) {
            eng->unk1E = u + 16;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 16;
            break;
        }
        goto fire;
    case 16:
        do {
            eng->unk1E = (eng->unk1E + 16) & 0xFFF;
        } while (0);
        t = eng->unk1E;
        eng->unk5E = 1;
        if ((u32)(t - 900) < 0x8F9U) {
            eng->unk1E = t - 16;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 15;
            break;
        }
    fire:
        if (func_800C2940(eng) >= 0) {
            nas->UserInfo.user1++;
            func_800C3778(eng);
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 13;
        }
        break;
    }
}

void func_800C3D48(NAS *nas, EXTengine *eng) {
    s32 i;
    s32 dir;
    s32 a;
    NAS *n;
    NAS *puff;
    PeriscopeShellExt *fux;
    EXTengine *e;

    dir = eng->unk1E;
    for (i = 0; ; i++) {
        {
            s32 k;
            if (i >= 2) {
                break;
            }
        }
        n = SpawnNASById(eng->unk10.x, eng->unk10.y - 0xFA, eng->unk10.z, 0x1302, 0x11);
        if (n != NULL) {
            PlaySfx(0x522, &n->Engine->sfx, (fux = (PeriscopeShellExt *)n->FUX, (SLONG_XYZ *)NULL), 0x1000, 0x1E00, 0);
            e = n->Engine;
            a = dir - Ovl5fShellAngleOffsets()[i];
            e->unk10.x -= (D_80068BD4[(a - 0x400) & 0xFFF].unk2 * 245) >> 11;
            e->unk10.z -= (D_80068BD4[a & 0xFFF].unk2 * 245) >> 11;
            e->unk1E = eng->unk1E;
            fux->vx = -(D_80068BD4[(dir - 0x400) & 0xFFF].unk2 * 200) >> 12;
            fux->vz = -(D_80068BD4[dir & 0xFFF].unk2 * 200) >> 12;
            fux->unk30 = 1;
            fux->unk2C = -1;
        }
        puff = Ovl5fSpawnBit1605Nas(&n->Engine->unk10, 7);
        if (puff != NULL) {
            Ovl5fPuffSetAngle(puff, eng->unk1E);
        }
    }
    nas->UserInfo.user2 = 0x78;
    nas->UserInfo.status = 0xD;
    if (nas->UserInfo.flags != 0) {
        eng->sub->unk74 = 0x2025;
    } else {
        eng->sub->unk74 = 0x2024;
    }
}

void PlotTankBossRocketTurret(NAS *nas) {
    EXTengine *eng;
    s32 t;

    eng = nas->Engine;
    eng->unk10.y = D_800CB20C->unk10.y - 0x4BF;
    if (D_800CB210 < 0x36) {
        return;
    }
    if (D_800CB210 >= 0x38) {
        return;
    }
    ((void (*)(char *, s32))func_8008AD9C)("RocketTurret", nas->UserInfo.status);
    if (nas->UserInfo.flags == 2) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 17:
        if (D_800CB210 == 0x36) {
            eng->flags = 0x8000;
            eng->unk5E = 1;
            eng->unk10.z -= 10;
            eng->unk10.y -= 3;
            nas->UserInfo.user2 = 30;
        } else if (D_800CB210 == 0x37) {
            if (--nas->UserInfo.user2 <= 0) {
                nas->UserInfo.flags = 0;
                nas->UserInfo.status = 19;
                nas->UserInfo.user2 = 0;
            }
        }
        break;
    case 19:
        t = ++nas->UserInfo.user2 & 0x3F;
        if (eng->unk10.x > 0x900) {
            if (t < 0x20) {
                eng->unk1E += 30;
            } else {
                eng->unk1E -= 30;
            }
        } else if (t >= 0x20) {
            eng->unk1E += 30;
        } else {
            eng->unk1E -= 30;
        }
        eng->unk5E = 1;
        if (nas->UserInfo.user2 > 30 && func_800C2940(eng) >= 0) {
            nas->UserInfo.status = 10;
            nas->UserInfo.user2 = D_800CB248;
        }
        break;
    case 10:
        if (--nas->UserInfo.user2 == D_800CB248 / 2) {
            if (nas->UserInfo.flags != 0) {
                eng->sub->unk74 = 0x2022;
            } else {
                eng->sub->unk74 = 0x2021;
            }
        }
        if (nas->UserInfo.user2 <= 0) {
            func_800C3D48(nas, eng);
            nas->UserInfo.status = 13;
            nas->UserInfo.user2 = 30;
        }
        break;
    case 13:
        if (--nas->UserInfo.user2 <= 0) {
            nas->UserInfo.status = 18;
        }
        break;
    case 18:
        eng->unk5E = 1;
        if (eng->unk10.x > 0x900) {
            if (eng->unk1E < 0xFCF) {
                eng->unk1E = (eng->unk1E - 20) & 0xFFF;
                break;
            }
        } else if (eng->unk1E < 0) {
            eng->unk1E = (eng->unk1E + 20) & 0xFFF;
            break;
        }
        eng->unk1E = 0;
        nas->UserInfo.status = 19;
        nas->UserInfo.user2 = 0;
        break;
    case 0:
        nas->UserInfo.status = 18;
        nas->UserInfo.user2--;
        break;
    }
}

void func_800C4320(NAS *nas, s32 id) {
    EXTengine *eng;
    subengine *sub;
    subengine *s;
    subengine *s2;
    JoeExt *ext;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    nas->UserInfo.flags = 0;
    sub = eng->sub;
    sub->unk6C = D_80053704;
    if (id == 0x131F) {
        sub->unk74 = 0x4180;
    } else {
        sub->unk74 = 0x4080;
    }
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    ext = AddExtension(nas, 0x38, 0x10000000);
    nas->FUX = (EXT *)ext;
    ext->unk28 = 3;
    func_80018F3C(eng, 2);
    sub = eng->sub;
    s = sub->next;
    sub->unk51 = 1;
    s2 = s->next;
    s->unk51 = 1;
    s->unk6C = D_80053694;
    s->unk74 = 0x2011;
    s->unk68 = 0x100;
    s->flags = (s->flags & 0xFFFF7FFF) | 0x02000000;
    s2->unk51 = 1;
    s2->unk6C = D_80053694;
    s2->unk74 = 0x4300;
    s2->unk68 = 0x100;
    s2->flags = (s2->flags & 0xFFFF7FFF) | 0x02000000;
    switch (D_800CB25C) {
    case 1:
        ext->unk30 = D_800CB25C;
        break;
    case 0x2C:
        eng->unk10.y = 0x1001;
        eng->unk10.z = 0x1001;
        sub->unk68 = 0x100;
        sub->flags |= 0x02000000;
        ext->unk30 = 5;
        break;
    case 0x3A:
        ext->unk30 = D_800CB25C;
        eng->unk10.y = 0x480;
        eng->unk10.z = 0x1001;
        sub->unk68 = 0x100;
        sub->flags |= 0x02000000;
        break;
    }
}

void func_800C44E8(s32 arg0) {
    if (arg0 == 3) {
        s32 val;
        D_800CB24C = D_800CAA0C;
        val = *D_800CB24C++;
        D_8009D678->unk74 = val;
    } else {
        s32 val;
        D_800CB250 = D_800CAB58;
        val = *D_800CB250++;
        D_8009D60C->unk74 = val;
    }
}

void func_800C4548(s32 player) {
    TA_player *ta;
    EXTengine *eng;
    EXTengine *e;
    EXTengine *se;
    subengine *sub;
    subengine *sub2;
    NAS *n;
    NAS *puff;
    PeriscopeShellExt *fux;
    s32 a;
    s32 dx;
    s32 dz;
    u16 anim;

    ta = &D_800CAD50[player];
    eng = Heroes[player].hero_eng;
    sub = Heroes[player].hero_sub->next;
    sub2 = sub->next;
    if (ta->TurretFiring >= 0) {
        a = *ta->TurretAnimPtr++;
        if (a != 0) {
            if (D_8005A648 == 5) {
                sub2->unk74 = a;
            } else {
                sub->unk74 = a;
            }
        } else {
            switch (ta->TurretFiring) {
            default:
                anim = 0x2010;
                break;
            case 11:
                anim = 0x2011;
                break;
            case 10:
                anim = 0x2012;
                break;
            case 8:
                anim = 0x2009;
                break;
            case 0:
            case 3:
                anim = 0x2008;
                break;
            case 2:
                anim = 0x200B;
                break;
            }
            sub->unk74 = anim;
            ta->TurretFiring = -1;
            if (ta->MainWeaponRounds <= 0) {
                func_800B6FF4(player);
            }
        }
        if (sub->unk74 == 0x4004) {
            e = Heroes[player].hero_eng;
            puff = Ovl5fSpawnBit1605Nas(&e->unk10, 0);
            if (puff != NULL) {
                Ovl5fPuffSetAngle(puff, eng->unk1E);
            }
            if (D_800CAD50[player].MainWeaponType == 3) {
                n = SpawnNASById(e->unk10.x, e->unk10.y - 0x12C, e->unk10.z, 0x130B, 0x11);
                if (n != NULL) {
                    PlaySfx(0x4F3, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    fux = AddExtension(n, 0x38, 0x10000000);
                    n->FUX = fux;
                    se = n->Engine;
                    dx = -(D_80068BD4[(e->unk1E - 0x400) & 0xFFF].unk2 * 250);
                    se->unk10.x += dx >> 12;
                    dz = -(D_80068BD4[(u16)e->unk1E & 0xFFF].unk2 * 250);
                    se->unk10.z += dz >> 12;
                    fux->vx = dx >> 13;
                    fux->vy = -0x5B;
                    fux->vz = dz >> 13;
                    fux->unk2C = player;
                    fux->unk30 = 1;
                    D_800CAD50[player].MainWeaponRounds--;
                }
            }
        }
    } else if (D_8005A648 == 5) {
    loop:
        a = *ta->TurretAnimPtr++;
        if (a == 0) {
            switch (ta->MainWeaponType) {
            case 9:
            default:
                ta->TurretAnimPtr = D_800C9A54;
                break;
            case 11:
                ta->TurretAnimPtr = D_800C9AA4;
                break;
            case 10:
                ta->TurretAnimPtr = D_800C9AF4;
                break;
            case 8:
                ta->TurretAnimPtr = D_800C9B44;
                break;
            }
            goto loop;
        }
        sub2->unk74 = a;
    }
    if (D_8005A648 != 5) {
        if (++sub2->unk74 == 0x4306) {
            sub2->unk74 = 0x4300;
        }
    }
}

void func_800C49F8(s32 arg0) {
    EXTengine *eng;
    s32 pad[2];

    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    Heroes[arg0].hero_idle_count = Random(0x96) + 0x5A;
    eng = Heroes[arg0].hero_eng;
    if (D_800CAD50[arg0].HullSFX != 0) {
        D_800CAD50[arg0].HullSFX = 0;
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
    }
    if (D_800CAD50[arg0].TurretSFX != 0) {
        D_800CAD50[arg0].TurretSFX = 0;
        StopSfx(D_800CAD50[arg0].TurretSFXHandler);
    }
    if (D_800CAD50[arg0].FireBreatherSFX != 0) {
        D_800CAD50[arg0].FireBreatherSFX = 0;
        StopSfx(D_800CAD50[arg0].FireBreatherSFXHandler);
    }
    if (arg0 == 3) {
        s32 v;
        D_800CB24C = D_800CAA0C;
        v = *D_800CB24C++;
        D_8009D678->unk74 = v;
    } else {
        s32 v;
        D_800CB250 = D_800CAB58;
        v = *D_800CB250++;
        D_8009D60C->unk74 = v;
    }
}

void func_800C4B60(s32 arg0) {
    if (arg0 == 3) {
        s32 val;
        D_800CB24C = D_800CAA0C;
        val = *D_800CB24C++;
        D_8009D678->unk74 = val;
    } else {
        s32 val;
        D_800CB250 = D_800CAB58;
        val = *D_800CB250++;
        D_8009D60C->unk74 = val;
    }
}

void func_800C4BC0(s32 arg0) {
    s32 val;
    s32 val2;
    s32 *p;
    s32 *p2;
    s32 z;

    if (D_800CB0F0 == 0) {
        if (arg0 == 3) {
        again:
            p = D_800CAADC;
            D_800CB24C = p;
            val = *D_800CB24C++;
            D_8009D678->unk74 = val;
            PlaySfx(0x261, 0, (SLONG_XYZ *)((u8 *)Heroes[arg0].hero_eng + 0x10), 0x1000, 0x1E00, 0);
        } else {
            p2 = D_800CAC38;
            D_800CB250 = p2;
            val2 = *D_800CB250++;
            ((Ovl5fUnk74 *)Heroes[2].hero_sub)->unk74 = val2;
            PlaySfx(0x260, 0, (SLONG_XYZ *)((u8 *)Heroes[arg0].hero_eng + 0x10), 0x1000, 0x1E00, 0);
        }
        D_800CAD50[arg0].SpeedY = -0x64;
        Heroes[arg0].hero_immunity = 0x3C;
        do {
            z = 0;
        } while (0);
        if (z) {
            goto again;
        }
    }
}

void func_800C4D04(s32 hero, s32 last_mode) {
    subengine *s;

    Heroes[hero].hero_hit = 0;
    Heroes[hero].hero_immunity = 0;
    Heroes[hero].hero_health = 0;
    Heroes[hero].hero_status |= 0x6000;
    func_800898EC(hero);
    s = Heroes[hero].hero_eng->sub;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
    if (hero == 3) {
        D_800CB254 = 0x26;
    } else {
        D_800CB258 = 0x26;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C4DCC);

void func_800C53B0(s32 arg0) {
    EXTengine *eng;
    TA_player *p;
    HeroInfo *h;
    HeroInfo *base;
    s32 val;
    s32 mode;
    s32 last;

    eng = Heroes[arg0].hero_eng;
    D_800CAD50[arg0].SpeedY += 0xC;
    p = &D_800CAD50[arg0];
    eng->unk10.y += p->SpeedY;
    if (eng->unk10.y > 0) {
        p->SpeedY = -p->SpeedY / 2;
        eng->unk10.y = 0;
    }
    eng->sub->next->next->unk08 = -eng->unk10.y / 2;
    if (arg0 == 3) {
        val = *D_800CB24C++;
        Heroes[3].hero_sub->unk74 = val;
    } else {
        val = *D_800CB250++;
        Heroes[2].hero_sub->unk74 = val;
    }
    if (val == 0) {
        base = Heroes;
        h = base + arg0;
        if (h->hero_status & 0x2000) {
            u16 old = h->hero_mode;

            h->hero_mode = 0x6A;
            h->hero_last_mode = old;
            if (arg0 == 3) {
                subengine *s;
                s32 v;
                v = D_800CAA60[0];
                D_800CB24C = D_800CAA60;
                s = Heroes[3].hero_sub;
                D_800CB24C++;
                s->unk74 = v;
            } else {
                subengine *s;
                s32 v;
                v = D_800CABAC[0];
                D_800CB250 = D_800CABAC;
                s = Heroes[2].hero_sub;
                D_800CB250++;
                s->unk74 = v;
            }
            return;
        }
        mode = h->hero_last_mode;
        last = h->hero_mode;
        h->hero_mode = mode;
        h->hero_last_mode = last;
        switch (mode) {
        case 0x6C:
            func_800C49F8(arg0);
            break;
        case 0x6E:
            func_800C4B60(arg0);
            break;
        case 0x69:
            func_800C4BC0(arg0);
            break;
        case 0x6B:
            ((void (*)(s32))func_800C4D04)(arg0);
            break;
        case 0x6A:
        case 0x6D:
            break;
        }
        if (arg0 == 3) {
            s32 v;
            D_800CB24C = D_800CAA0C;
            v = *D_800CB24C++;
            D_8009D678->unk74 = v;
        } else {
            s32 v;
            D_800CB250 = D_800CAB58;
            v = *D_800CB250++;
            D_8009D60C->unk74 = v;
        }
    }
}

void func_800C5620(s32 arg0) {
    s32 val;
    EXTengine *eng;
    s32 *pos;
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;
    s32 mode;

    if (arg0 == 3) {
        D_8009D678->unk74 = val = *D_800CB24C++;
    } else {
        D_8009D60C->unk74 = val = *D_800CB250++;
    }
    if (val == 0) {
        eng = Heroes[arg0].hero_eng;
        pos = (s32 *)&eng->unk10;
        p = D_8005365C;
        nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->unkC = 0xE;
                ext->unk18 = 0x100;
                if (p != NULL) {
                    ext->unk20 = 5;
                    nas->Engine->sub->unk6C = p;
                } else {
                    ext->unk20 = 1;
                }
                ext->unk28 = 0;
                nas->InitDelete(nas);
            }
        }
        PlaySfx(0x4F0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        mode = Heroes[arg0].hero_mode;
        Heroes[arg0].hero_mode = 0x6B;
        Heroes[arg0].hero_last_mode = mode;
        func_800C4D04(arg0, mode);
    }
}

void func_800C57B0(NAS *nas, s32 id) {
    EXTengine *eng;
    subengine *sub;
    HeroInfo *h;
    HeroInfo *hb;
    EXTbotai *ai;
    TA_player *ta;
    TA_player *tb;
    Ovl5fAITarget *t;
    s32 p;
    u16 mode;
    u8 *g;

    eng = nas->Engine;
    sub = eng->sub;
    if (id == 0x131E) {
        p = 3;
        D_8009D6A0 = 2;
        D_800CB254 = 0x3B;
    } else {
        p = 2;
        D_8009D634 = 3;
        D_800CB258 = 0x3B;
    }
    g = D_8005A614 + p;
    g[0x5C] = 1;
    hb = Heroes;
    h = &hb[p];
    h->hero_status = 0x80;
    h->hero_strength = 0x14;
    h->hero_idle_count = 0x258;
    h->hero_harm_player = -1;
    h->hero = nas;
    h->hero_eng = eng;
    h->hero_sub = sub;
    h->hero_immunity = 0;
    h->hero_immunity_level = 0;
    h->hero_hit = 0;
    h->hero_last_mode = 0;
    h->hero_mode = 0;
    h->hero_pick_delay = 0;
    h->hero_aseq = 0;
    h->hero_harm_Nas = NULL;
    h->hero_harm_value = 0;
    h->hero_harm_fraction = 0;
    h->hero_harm_flags = 0;
    h->hero_harm_sfx = 0;
    h->hero_ctrl = 0;
    h->hero_score = 0;
    h->hero_score_displayed = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    h->hero_pu_lock = 0;
    h->hero_attack_timer = 0;
    h->hero_attack_delay = 0;
    h->hero_pu_action = 0;
    h->hero_pu_action_timer = 0;
    h->hero_weapon_count = 0;
    h->hero_weapon_timer = 0;
    h->hero_attack_counter = 0;
    h->hero_name = 9;
    g[0x50] = 9;
    D_8009D87C[p].unk0 = 0x10;
    D_8009D87C[p].unk2 = 0x10;
    D_8009D87C[p].unk4 = 0x10;
    if (h->hero_ai == NULL) {
        ai = AddExtension(h->hero, 0x44, 0x400000F4);
        if (ai != NULL) {
            h->hero_ai = ai;
            ai->AImode = 0;
            ai->AIaborted_mode = 0;
            ai->AItimer1 = 0;
            ai->AItimer2 = 0;
            ai->AIang = 0;
            ai->AIpad = 0;
            ai->pTargetNas = NULL;
            ai->TargetPlayer = -1;
            ai->TargetXYZ.z = 0;
            ai->TargetXYZ.y = 0;
            ai->TargetXYZ.x = 0;
            ai->AIinitial_delay = Random(0x3C) + 0x1E;
            ai->AIsleep = 8;
        }
    }
    ((PadButtons *)D_80051338)[p].unk50 = 0;
    ((PadButtons *)D_80051338)[p].unk54 = 0;
    eng = Heroes[p].hero_eng;
    eng->unk10.z = 0xF00;
    eng->unk10.x = (eng->unk10.x + 0x80) & ~0xFF;
    mode = Heroes[p].hero_mode;
    Heroes[p].hero_mode = 0x6E;
    Heroes[p].hero_last_mode = mode;
    func_800C4B60(p);
    sub = eng->sub;
    sub->unk68 = 0;
    sub = sub->next;
    sub->unk51 = 1;
    sub->unk68 = 0;
    sub = sub->next;
    sub->unk51 = 1;
    sub->unk68 = 0;
    func_800C49F8(p);
    tb = D_800CAD50;
    ta = &tb[p];
    ta->Speed = 0x1A;
    ta->MoveDir = 0;
    ta->FaceDir = 0;
    ta->RunnerMoving = 0;
    ta->RunnerFacing = 0;
    ta->SpeedX = 0;
    ta->SpeedZ = 0;
    ta->Speed = ta->Speed * (D_8009D87C[p].unk0 + 0x54) / 100;
    ta->Energy = 0x63;
    Heroes[p].hero_health = ta->Energy;
    ta->PosX = eng->unk10.x;
    ta->PosZ = eng->unk10.z;
    ta->SubWeaponFireDelay = 0;
    ta->SubWeaponRounds = 0;
    ta->FireBreatherSFXDuration = 0;
    ta->TurretSFXDuration = 0;
    ta->TurretReqdDir = -1;
    ta->HangaboutTime = -0x5A;
    ta->IdleAnimCt = Random(0x1E);
    ta->TurretFiring = -1;
    ta->TauntDelay = 0;
    func_800B6FF4(p);
    ta->MainWeaponFireDelay = 0x1E;
    ta->MainWeaponNas[0] = (NAS *)0xFFEEEEFF;
    ta->MainWeaponNas[2] = (NAS *)0xFFEEEEFF;
    t = (Ovl5fAITarget *)(D_800CAFFC + p * 0x2C);
    t->unk0 = 0;
    t->unk4 = 0;
    t->unk8 = 0;
    t->target = NULL;
    t->unk10 = 0;
    t->flags = 0;
    t->timer = 0;
    if (D_800CB254 == 0x3B && D_800CB258 == D_800CB254) {
        func_8007E93C(0x3B, 0x300E, 0x300F);
        func_8007C830((s32)&Heroes[2].hero_health, (s32)&Heroes[3].hero_health);
    }
}

void func_800C5BCC(NAS *nas, s32 id) {
    EXTengine *eng;
    TA_player *ta;
    s32 p;
    s32 x;
    s32 dz;
    u16 mode;

    p = 2;
    if (id == 0x131E) {
        p = 3;
    }
    eng = nas->Engine;
    eng->flags = 0x8000;
    ta = &D_800CAD50[p];
    switch (Heroes[p].hero_mode) {
    case 0x6C:
        mode = Heroes[p].hero_mode;
        Heroes[p].hero_mode = 0x6E;
        Heroes[p].hero_last_mode = mode;
        func_800C4B60(p);
        break;
    case 0x6E:
        func_800C4DCC(p);
        break;
    case 0x69:
        func_800C53B0(p);
        break;
    case 0x6A:
        func_800C5620(p);
        break;
    case 0x6B:
        func_80089BE8(p);
        break;
    }
    if (eng->unk10.x > D_800CB100) {
        eng->unk10.x = D_800CB100;
    }
    if (eng->unk10.x < 0x100) {
        eng->unk10.x = 0x100;
    }
    if (eng->unk10.z > D_800CB104) {
        eng->unk10.z = D_800CB104;
    }
    if (eng->unk10.z < 0x100) {
        eng->unk10.z = 0x100;
    }
    func_800C4548(p);
    if (D_8005A648 != 5) {
        x = eng->unk10.x;
        dz = eng->unk10.z - eng->OldXYZ.z;
        if (x != eng->OldXYZ.x || dz != 0) {
            if (ta->HangaboutTime != 0) {
                ta->HangaboutTime--;
            }
            OVL5F_SET_HULL_SFX(p, eng, D_800C94C8[p]);
        } else {
            if (!(Heroes[p].hero_status & 0xFA00)) {
                ta->HangaboutTime++;
            }
            OVL5F_SET_HULL_SFX(p, eng, 0);
        }
        if (ta->TurretSFXDuration != 0) {
            if (--ta->TurretSFXDuration == 0) {
                OVL5F_SET_TURRET_SFX(p, 0);
            } else {
                OVL5F_SET_TURRET_SFX(p, D_800C94D8[p]);
            }
        }
    }
    if (((PadButtons *)D_80051338)[p].unk50 & 0x4800) {
        OVL5F_SET_HULL_SFX(p, eng, D_800C9408[p]);
    } else {
        OVL5F_SET_HULL_SFX(p, eng, 0);
    }
    if (Heroes[p].hero_immunity != 0) {
        Heroes[p].hero_immunity--;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C60AC);

void func_800C60AC(NAS *nas, s32 id);

void PlotJoe(NAS *nas) {
    func_800C60AC(nas, 0x131F);
}

void func_800C4320(NAS *nas, s32 id);

void InitJoe(NAS *nas) {
    func_800C4320(nas, 0x131F);
}

void PlotMoe(NAS *nas) {
    func_800C60AC(nas, 0x131E);
}

void InitMoe(NAS *nas) {
    func_800C4320(nas, 0x131E);
}

void func_800C68A8(void) {
    func_80089BE8();
}

void func_800C68C8(s32 arg0) {
    u16 prev = Heroes[arg0].hero_mode;
    Heroes[arg0].hero_mode = 0x6E;
    Heroes[arg0].hero_last_mode = prev;
    func_800C4B60(arg0);
}

void func_800C6914(s32 hero, s32 mode) {
    u16 last = Heroes[hero].hero_mode;

    Heroes[hero].hero_mode = mode;
    Heroes[hero].hero_last_mode = last;
    switch (mode) {
    case 0x6C:
        ((void (*)())func_800C49F8)();
        break;
    case 0x6E:
        ((void (*)())func_800C4B60)();
        break;
    case 0x69:
        ((void (*)())func_800C4BC0)();
        break;
    case 0x6B:
        func_800C4D04(hero, mode);
        break;
    case 0x6A:
    case 0x6D:
        break;
    }
}

void func_800C69BC(void) {
}

void func_800C69C4(s32 arg0, s32 *arg1) {
    s32 val;

    if (arg0 == 3) {
        D_800CB24C = arg1;
    } else {
        D_800CB250 = arg1;
    }
    if (arg0 == 3) {
        val = *D_800CB24C++;
        D_8009D678->unk74 = val;
    } else {
        val = *D_800CB250++;
        D_8009D60C->unk74 = val;
    }
}

s32 func_800C6A30(s32 arg0) {
    s32 val;

    if (arg0 == 3) {
        D_8009D678->unk74 = val = *D_800CB24C++;
    } else {
        D_8009D60C->unk74 = val = *D_800CB250++;
    }
    return val;
}

void InitTankBossRocketTurret(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    sub = eng->sub;
    eng->unk5D = 0;
    eng->unk10.z += 0x258;
    eng->unk10.y += 0xC8;
    sub->unk6C = D_800536CC;
    if (nas->UserInfo.flags == 0) {
        sub->unk74 = 0x2024;
    } else {
        sub->unk74 = 0x2025;
    }
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    nas->UserInfo.status = 0x11;
    eng->flags = 0;
}

void InitTankBossCortexTurret(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    eng->unk5D = 1;
    sub = eng->sub;
    sub->unk6C = D_800536CC;
    sub->unk74 = 0x2004;
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    nas->UserInfo.status = 0x11;
}

void InitTankBossCrashTurret(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 v;

    eng = nas->Engine;
    sub = eng->sub;
    eng->unk10.z += 0x100;
    v = 0x100;
    nas->UserInfo.flags = v;
    nas->UserInfo.user2 = 0x168;
    nas->UserInfo.status = 4;
    sub->unk6C = D_800536CC;
    sub->unk74 = 0x2003;
    sub->unk68 = v;
    sub->unk51 = 0;
    sub->flags |= 0x02000000;
}

s32 func_800C6B9C(s32 id, s32 x, s32 z, s32 angle) {
    s32 d;

    if (D_800CB0F0 == 0) {
        if (D_800CA9E4[id].z >= 0) {
            d = (angle - get_angle(x - D_800CA9E4[id].x, z - D_800CA9E4[id].z)) & 0xFFF;
            if (d < 0x15 || d > 0xFEB) {
                D_800CB260 = id;
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800C6C30(void) {
    if (D_8005A680[0] != 0) {
        if (Heroes[0].hero_health == 0) {
            return 1;
        }
        if (Heroes[1].hero_health == 0) {
            return 0;
        }
        return Random(2);
    }
    return 0;
}

s32 func_800C6C90(OvlClampPos *p, s32 x, s32 y, s32 z) {
    s32 d;

    d = p->x - x;
    if (d > 0x250) {
        return 0;
    }
    if (d < -0x250) {
        return 0;
    }
    d = p->pad14 - y;
    if (d > 0x200) {
        return 0;
    }
    if (d < -0x200) {
        return 0;
    }
    d = p->z - z;
    if (d > 0x500) {
        return 0;
    }
    return d >= -0x500;
}

void PlotTotemBot(NAS *nas) {
    func_800C0F24(nas, 0x131C);
}

void InitTotemBot(NAS *nas) {
    func_800C0C68(nas, 0x131C);
}

void PlotTotemTop(NAS *nas) {
    func_800C0F24(nas, 0x131B);
}

void InitTotemTop(NAS *nas) {
    func_800C0C68(nas, 0x131B);
}

void InitTotemPole(NAS *nas) {
    TotemPoleExt *ext;
    EXTengine *e;

    D_800CB218 = nas;
    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = &ext->ext;
        if (!(((u16 *)&D_8005A644)[1] & 0x80)) {
            ext->timer = 0x384;
        } else {
            ext->timer = 0x96;
        }
        ext->unk30 = 1;
        e = nas->Engine;
        e->flags = 0;
        D_800CB21C = SpawnNASById(e->unk10.x, e->unk10.y - 0x294, e->unk10.z, 0x131B, 0x11);
        D_800CB220 = SpawnNASById(e->unk10.x, e->unk10.y - 0x14A, e->unk10.z, 0x131C, 0x11);
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

void PlotPillarCover(NAS *nas) {
    switch (nas->UserInfo.user1) {
    case 6:
        nas->UserInfo.status++;
        if (nas->UserInfo.status >= 4) {
            KillNAS(nas);
            return;
        }
        break;
    case 7:
        if (nas->UserInfo.status == 0) {
            KillNAS(nas);
            return;
        }
        nas->UserInfo.status--;
        break;
    }
    nas->Engine->sub->unk74 = D_800CA944[nas->UserInfo.status].v;
}

void InitPillarCover(NAS *nas) {
    EXTengine *e = nas->Engine;
    e->unk10.y = 0;
    e->sub->unk6C = *(void **)D_80058AD0;
}

void InitPillar(NAS *nas) {
    EXTengine *e = nas->Engine;
    subengine *s;
    nas->UserInfo.user1 = 1;
    nas->UserInfo.status = 6;
    e->unk10.y = 0xB8;
    s = e->sub;
    s->unk6C = *(void **)D_80058AD0;
    s->unk68 = 0x20;
    s->flags |= 0x02000000;
}

void InitPillarControl(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x5A;
}

void InitBoatField(NAS *nas) {
    nas->UserInfo.user2 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBoatWake(NAS *nas) {
    EXTengine *src;
    EXTengine *dst;
    u16 t;

    src = Heroes[nas->UserInfo.user1].hero_eng;
    dst = nas->Engine;
    t = src->unk1E;
    dst->unk5E = 1;
    dst->unk1E = t;
    dst->unk10.x = src->unk10.x;
    dst->unk10.z = src->unk10.z;
    if (nas->UserInfo.status == 0) {
        nas->UserInfo.status = 0;
    }
    if (Heroes[nas->UserInfo.user1].hero_health <= 0) {
        KillNAS(nas);
    }
}

void InitBoatWake(NAS *nas) {
    subengine *sub;

    nas->UserInfo.status = 0;
    sub = nas->Engine->sub;
    sub->unk6C = D_80053774;
    sub->unk74 = 0x2007;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitPeriscopeGun(NAS *nas) {
    nas->Engine->sub->unk6C = *(void **)D_80058AD0;
    nas->Engine->sub->unk74 = 0x4080;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitPeriscope(NAS *nas) {
    EXTengine *eng;
    subengine *s;
    s32 one;
    void *p;
    PeriscopeShellExt *ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        ext->unk28 = 0x12C;
        one = 1;
        ext->unk30 = one;
        eng = nas->Engine;
        eng->unk5D = 0;
        p = *(void **)D_80058AD0;
        s = eng->sub;
        s->flags = 0;
        s->unk6C = p;
        if (func_80018F3C(eng, one) != 0) {
            s = s->next;
            s->flags = 0;
            s->unk6C = *(void **)D_80058AD0;
            s->unk74 = 0x2001;
        }
        if (nas->Failure != 0) {
            KillNAS(nas);
        }
    } else {
        KillNAS(nas);
    }
}

void InitRock_TA(NAS *nas) {
    MapTriggerInfo *info;
    EXT *ext;
    subengine *sub;
    u32 val;

    info = nas->trigger->InfoStruct;
    val = info->val1;
    nas->UserInfo.status = 1;
    nas->UserInfo.user2 = val;
    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = ext;
    sub = nas->Engine->sub;
    sub->unk6C = *(void **)D_80058AD0;
    sub->unk74 = D_800CA6D4[info->val1].v;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBigApple(NAS *nas) {
}

void InitBigApple(NAS *nas) {
}

void func_800C7298(NAS *nas) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_800CAD44[i] == nas) {
            D_800CAD44[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitPickupGenerator_TA(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0xF0;
}

void PlotShellExplosion(NAS *nas) {
    subengine *sub = nas->Engine->sub;

    sub->unk74++;
    if ((u16)sub->unk74 >= 0x2022) {
        KillNAS(nas);
    }
}

void InitShellExplosion(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    eng->flags |= 0x21;
    sub = eng->sub;
    sub->unk6C = D_80053694;
    sub->unk74 = 0x2019;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitShrapnelBit(NAS *nas) {
    subengine *sub = nas->Engine->sub;
    sub->unk6C = D_8005365C;
    sub->unk74 = D_800CA4AC[Random(3)].v;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotMineBall(NAS *nas) {
    if (++nas->UserInfo.status >= 8) {
        KillNAS(nas);
        return;
    }
    nas->Engine->sub->unk74 = D_800CA48C[nas->UserInfo.status];
}

void InitMineBall(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    eng->flags |= 0x21;
    sub = eng->sub;
    sub->unk6C = D_80053694;
    sub->unk74 = 0x2019;
    nas->UserInfo.status = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitTorpedo(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 i;

    eng = nas->Engine;
    nas->UserInfo.status = 0x5A;
    nas->UserInfo.flags = 0x11;
    nas->UserInfo.user2 = 0;
    eng->unk10.y = -0x14;
    sub = eng->sub;
    sub->unk6C = *(void **)D_80058AD0;
    sub->unk74 = 0x2001;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (D_800CB0C0[i] == NULL) {
                D_800CB0C0[i] = nas;
                break;
            }
        }
    }
}

void InitWaterMine(NAS *nas) {
    EXTengine *e;
    subengine *s;
    nas->UserInfo.flags = 1;
    nas->UserInfo.user1 = -1;
    nas->UserInfo.status = D_8004E0D8;
    e = nas->Engine;
    nas->UserInfo.user2 = 0;
    e->unk10.y = -0x14;
    s = e->sub;
    s->unk6C = *(void **)D_80058AD0;
    s->unk74 = 0x2001;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitMine(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 i;

    eng = nas->Engine;
    eng->unk10.y = -0x14;
    sub = eng->sub;
    sub->unk6C = D_80053694;
    sub->unk74 = 0x2005;
    nas->UserInfo.status = 0x78;
    nas->UserInfo.user1 = -1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (D_800CB0C0[i] == NULL) {
                D_800CB0C0[i] = nas;
                break;
            }
        }
    }
}

void InitRocketTA(NAS *nas) {
    FlamerBitExt *ext;
    subengine *sub;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = &ext->ext;
    sub = nas->Engine->sub;
    sub->unk6C = D_80053694;
    sub->unk74 = 0x2007;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitLobber_TA(NAS *nas) {
    EXTengine *eng;
    EXTengine *e2;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    eng->sub->unk6C = D_8005365C;
    eng->sub->unk74 = 0x200C;
    eng->unk5D = 1;
    e2 = AddEngine(nas, 1);
    if (e2 != NULL) {
        e2->unk10.x = eng->unk10.x;
        e2->unk10.y = 0;
        e2->unk10.z = eng->unk10.z;
        e2->sub->unk6C = D_8005365C - 0x700;
        e2->sub->unk74 = 0x2001;
        e2->sub->flags |= 0x40000041;
        e2->sub->unk76 = 0x800;
    }
}

void InitShell(NAS *nas) {
    EXTengine *eng;
    EXTengine *e2;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    eng->sub->unk6C = D_8005365C;
    eng->sub->unk74 = 0x200C;
    eng->unk5D = 1;
    e2 = AddEngine(nas, 1);
    if (e2 != NULL) {
        e2->unk10.x = eng->unk10.x;
        e2->unk10.y = 0;
        e2->unk10.z = eng->unk10.z;
        e2->sub->unk6C = D_8005365C - 0x700;
        e2->sub->unk74 = 0x2001;
        e2->sub->flags |= 0x40000041;
        e2->sub->unk76 = 0x800;
    }
}

void InitSwirlyTA(NAS *nas) {
    Ovl5fExt1605 *ext;
    EXTengine *eng;
    subengine *sub;
    void *p;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        p = D_8005365C;
        ext->unk28 = 0x87;
        eng = nas->Engine;
        sub = eng->sub;
        eng->unk5D = 0;
        sub->unk6C = p;
        sub->unk74 = 0x2038;
        sub->flags |= 0x20000000;
        eng->flags |= 0x21;
        if (func_80018F3C(eng, 2) == 2) {
            sub = sub->next;
            sub->unk6C = D_8005365C;
            sub->unk74 = 0x2030;
            sub = sub->next;
            sub->unk6C = D_8005365C;
            sub->unk74 = 0x2030;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

void InitFlubberTA(NAS *nas) {
    Ovl5fExt1605 *ext;
    EXTengine *eng;
    EXTengine *e2;
    subengine *sub;
    void *p;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        ext->unk28 = 0x4B;
        ext->unk14 = -0x1E;
        p = D_8005365C;
        eng = nas->Engine;
        sub = eng->sub;
        sub->unk6C = p;
        sub->unk74 = 0x2016;
        eng->unk5D = 1;
        e2 = AddEngine(nas, 1);
        if (e2 != NULL) {
            e2->unk10.x = eng->unk10.x;
            e2->unk10.y = 0;
            e2->unk10.z = eng->unk10.z;
            e2->sub->unk51 = 2;
            e2->sub->unk6C = D_8005365C - 0x700;
            e2->sub->unk74 = 0x2001;
            e2->sub->flags |= 0x40000041;
            e2->sub->unk76 = 0x800;
            e2->sub->unk50 = 1;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

void InitFlamerBitTA(NAS *nas) {
    FlamerBitExt *ext;
    EXTengine *eng;
    subengine *sub;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = &ext->ext;
    ext->timer = 0x3F;
    eng = nas->Engine;
    eng->unk5D = 1;
    sub = eng->sub;
    eng->flags |= 0x21;
    sub->unk6C = D_8005365C;
    sub->unk74 = 0x202F;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitLaserBitTA(NAS *nas) {
    FlamerBitExt *ext;
    EXTengine *eng;
    subengine *sub;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = &ext->ext;
    ext->timer = 0x5A;
    eng = nas->Engine;
    sub = eng->sub;
    eng->unk5D = 0;
    sub->unk6C = D_8005365C;
    sub->unk74 = 0x2021;
    eng->flags |= 0x21;
    if (func_80018F3C(eng, 1) != 0) {
        sub = sub->next;
        sub->unk6C = D_8005365C;
        sub->unk74 = 0x2023;
        eng->flags |= 0x21;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800C7C08(u8 *arg0) {
    u32 temp_v0;

    temp_v0 = (*(s32*)((u8*)(arg0) + 16)) * 3;
    (*(s32*)((u8*)(arg0) + 16)) = (s32) ((s32) (temp_v0 + (temp_v0 >> 0x1F)) >> 1);
    (*(s32*)((u8*)(arg0) + 24)) = (s32) ((s32) ((*(s32*)((u8*)(arg0) + 24)) * 3) / 2);
    (*(s32*)((u8*)(arg0) + 40)) = (s32) (((*(s32*)((u8*)(arg0) + 40)) * 2) / 3);
}

void func_800C7C68(void) {
}

void func_800C7C70(s32 arg0) {
    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
    }
}

void func_800C7D04(void) {
    func_80089BE8();
}

void func_800C7D24(HeroInfo *hero) {
    SetHeroSeq(hero, 0x58);
}

s32 func_800C7D44(s32 flags, Ovl5fHolder *a, Ovl5fPos *b) {
    s32 dx = a->pos->x - b->x;
    s32 dz = a->pos->z - b->z;

    if (dx < -0x15D) {
        return flags;
    }
    if (dx >= 0x15E || dz < -0x15D || dz >= 0x15E) {
        return flags;
    }
    flags &= (dx < 0) ? -0x81 : -0x21;
    flags &= (dz > 0) ? -0x11 : -0x41;
    return flags;
}

void func_800C7DB8(s32 arg0) {
    s32 ok;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 0) {
        SetHeroSeq((HeroInfo *)arg0, 0x59);
    }
}

void func_800C7E68(HeroInfo *hero, Ovl5fActObj *obj, s32 arg2) {
    SetHeroAction(hero, 0x6F);
    obj->unk54 = -0x80;
    obj->unk50 = 8;
    obj->unk8 = arg2;
}

void func_800C7EB0(HeroInfo *hero, Ovl5fActState *state, s32 arg2) {
    SetHeroAction(hero, 0x6F);
    state->unk54 = 0x80;
    state->unk50 = 8;
    state->unk8 = arg2;
}

void func_800C7EF8(HeroInfo *hero) {
    SetHeroAction(hero, 0x6E);
}

void func_800C7F18(NAS *nas, s32 player) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_800CAD50[player].MainWeaponNas[i] == nas) {
            D_800CAD50[player].MainWeaponNas[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void func_800C7F80(s32 player, s32 unused, s32 ReqdSFX) {
    if (D_800CAD50[player].FireBreatherSFX != ReqdSFX) {
        D_800CAD50[player].FireBreatherSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            StopSfx(D_800CAD50[player].FireBreatherSFXHandler);
        } else {
            PlaySfx(ReqdSFX, &D_800CAD50[player].FireBreatherSFXHandler, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void func_800C8008(s32 player, s32 unused, s32 ReqdSFX) {
    if (D_800CAD50[player].TurretSFX != ReqdSFX) {
        D_800CAD50[player].TurretSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            StopSfx(D_800CAD50[player].TurretSFXHandler);
        } else {
            PlaySfx(ReqdSFX, &D_800CAD50[player].TurretSFXHandler, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void func_800C8090(s32 player, EXTengine *pEngine, s32 ReqdSFX) {
    if (D_800CAD50[player].HullSFX != ReqdSFX) {
        D_800CAD50[player].HullSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            if (pEngine->sfx != NULL) {
                StopSfx(pEngine->sfx);
            }
        } else {
            PlaySfx(ReqdSFX, &pEngine->sfx, &pEngine->unk10, 0x1000, 0x1E00, 0);
        }
    }
}

s32 func_800C8120(s32 arg0) {
    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
        return 1;
    }
    return 0;
}

void func_800C81C0(void) {
}

void func_800C81C8(s32 x, s32 z, s32 range, s32 arg3) {
    HeroInfo *h;
    s32 d;
    s32 i;

    s32 neg;

    i = 0;
    neg = -range;
    for (h = Heroes; i < 4; i++, h++) {
        d = x - h->hero_eng->unk10.x;
        if (d >= neg && d <= range) {
            d = z - h->hero_eng->unk10.z;
            if (d >= neg && d <= range) {
                func_800B6354(i, arg3);
            }
        }
    }
}

s32 func_800C8294(OvlClampPos *p) {
    s32 r;
    s32 lim;

    r = 0;
    if (p->x > D_800CB100) {
        lim = D_800CB100;
        p->x = lim;
        r = 1;
    }
    if (p->x < 0x100) {
        p->x = 0x100;
        r = 1;
    }
    if (p->z > D_800CB104) {
        lim = D_800CB104;
        p->z = lim;
        r = 1;
    }
    if (p->z < 0x100) {
        p->z = 0x100;
        r = 1;
    }
    return r;
}

void func_800C831C(void) {
}

void func_800C8324(void) {
}

void func_800C832C(void) {
}

void func_800C8334(s32 arg0) {
    Heroes[arg0].hero_shadow->flags = 0;
}

void func_800C8360(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_800CAD50[i].TurretSFXHandler != NULL) {
            StopSfx(D_800CAD50[i].TurretSFXHandler);
        }
        if (D_800CAD50[i].FireBreatherSFXHandler != NULL) {
            StopSfx(D_800CAD50[i].FireBreatherSFXHandler);
        }
    }
}

void func_800C83D0(void) {
}

void func_800C83D8(void) {
    D_8005AA70[0] = (s32)func_800B5588;
    D_8005AA70[1] = (s32)func_800B58B0;
    D_8005AA70[2] = (s32)func_800B58B0;
    D_8005AA70[3] = (s32)func_800B5FC4;
    D_8005AA70[4] = (s32)func_800C8360;
    D_8005AA70[6] = (s32)func_800C8324;
    D_8005AA70[5] = (s32)func_800C832C;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[10] = (s32)func_800B62A4;
    D_800CB25C = 1;
}

void func_800C8450(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = D_800CB0C0[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d >= -dist && d <= dist) {
                d = z - eng->unk10.z;
                if (d >= -dist && d <= dist) {
                    nas->UserInfo.status = 1;
                }
            }
        }
    }
}

NAS *func_800C84D8(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = D_800CB264[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d >= -dist && d <= dist) {
                d = z - eng->unk10.z;
                if (d >= -dist && d <= dist) {
                    return nas;
                }
            }
        }
    }
    return NULL;
}


void func_800C8560(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (D_800CB264[i] == nas) {
            D_800CB264[i] = NULL;
            return;
        }
    }
}

void func_800C8598(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (D_800CB264[i] == NULL) {
            D_800CB264[i] = nas;
            return;
        }
    }
}

void func_800C85D0(void) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        D_800CB264[i] = NULL;
    }
}

void func_800C85F8(Ovl5fHolder *h, s16 v) {
    Ovl5fLink *l;

    ((Ovl5fObj *)h->pos)->unk1E = v;
    ((Ovl5fObj *)h->pos)->unk5E = 1;
    l = ((Ovl5fObj *)h->pos)->list;
    if (l != NULL) {
        do {
            l->flags |= 0x20000000;
            l = l->next;
        } while (l != NULL);
    }
}

s32 func_800C8650(s32 x, s32 z, s32 r, s32 skip) {
    s32 i;
    s32 dx;
    s32 dz;
    EXTengine *eng;
    u16 name;
    u8 *g;

    s32 nine;

    for (i = 0, nine = 9, g = D_8005A614; i < 4; i++) {
        if (i == skip) {
            continue;
        }
        name = Heroes[i].hero_name;
        if (name == nine &&
            (((Ovl5fModeView *)g)->mode != name ||D_800CB210 != 0 || (u32)(i - 2) >= 2U)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        eng = Heroes[i].hero_eng;
        dx = x - eng->unk10.x;
        if (dx < -r || r < dx) {
            continue;
        }
        dz = z - eng->unk10.z;
        if (dz <= -r || dz >= r) {
            continue;
        }
        return i;
    }
    return -1;
}

s32 func_800C872C(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 d;
    u16 name;
    EXTengine *e;

    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = arg0 - e->unk10.x;
        if (d < -arg2) {
            continue;
        }
        if (arg2 < d) {
            continue;
        }
        d = arg1 - e->unk10.z;
        if (-arg2 < d && d < arg2) {
            return i;
        }
    }
    return -1;
}

void func_800C8800(s32 arg0, s32 arg1) {
    s32 idx;
    NAS *nas;

    idx = ((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9);
    nas = (NAS *)D_800CA34C[idx];
    D_800CA34C[idx] = 0;
    D_800CACB4[idx] = 0;
    D_800CACFC[idx] = 0;
    KillNAS(nas);
}

s32 func_800C8870(s32 arg0, s32 arg1) {
    return D_800CA34C[((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9)];
}

void func_800C88A8(NAS *nas) {
    s32 temp_a0;
    s32 temp_a1;
    s32 idx;
    u8 temp_a2;

    temp_a1 = nas->Engine->unk10.z >> 9;
    temp_a0 = nas->Engine->unk10.x >> 9;
    temp_a2 = ((temp_a0 << 5) | 0x10) + ((temp_a1 * 2) + 1);
    idx = (temp_a1 * D_800CAFF4) + temp_a0;
    D_800CACB4[idx] = temp_a2;
    D_800CACFC[idx] = temp_a2;
    D_800CA34C[idx] = (s32)nas;
}

u8 func_800C891C(s32 arg0, s32 arg1) {
    return *(((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9) + D_800CACFC);
}

u8 func_800C8950(s32 arg0, s32 arg1) {
    return *(((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9) + D_800CACB4);
}

void func_800C8984(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_a1;

    temp_a1 = arg1 >> 9;
    temp_a0 = arg0 >> 9;
    D_800CACB4[(temp_a1 * D_800CAFF4) + temp_a0] = 0;
    D_800CACFC[(temp_a1 * D_800CAFF4) + temp_a0] = 0;
}

void func_800C89D8(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_a1;
    u8 temp_a2;

    temp_a1 = arg1 >> 9;
    temp_a0 = arg0 >> 9;
    temp_a2 = ((temp_a0 << 5) | 0x10) + ((temp_a1 * 2) + 1);
    D_800CACB4[(temp_a1 * D_800CAFF4) + temp_a0] = temp_a2;
    D_800CACFC[(temp_a1 * D_800CAFF4) + temp_a0] = temp_a2;
}

void func_800C8A40(s32 arg0) {
    u8 *temp_v0;

    temp_v0 = (arg0 * 0x2C) + D_800CAFFC;
    (*(s32*)((u8*)(temp_v0) + 0)) = 0;
    (*(s32*)((u8*)(temp_v0) + 4)) = 0;
    (*(s32*)((u8*)(temp_v0) + 8)) = 0;
    (*(s32*)((u8*)(temp_v0) + 12)) = 0;
    (*(s32*)((u8*)(temp_v0) + 16)) = 0;
    (*(s32*)((u8*)(temp_v0) + 28)) = 0;
    (*(s16*)((u8*)(temp_v0) + 40)) = 0;
}

NAS *func_800C8A80(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    NAS *nas;
    EXTengine *eng;

    for (i = 0; i < 3; i++) {
        nas = D_800CAD44[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (z - eng->unk10.z) < range) {
                return nas;
            }
        }
    }
    return NULL;
}

void func_800C8AF8(s32 player, EXTengine *eng, s32 *arg2) {
    s32 hero;
    EXTengine *target;

    if (D_8005A648 != 5) {
        hero = arg2[2];
        if (hero >= 0) {
            target = Heroes[hero].hero_eng;
            D_800CAD50[player].TurretReqdDir =
                (get_angle(target->unk10.x - eng->unk10.x, target->unk10.z - eng->unk10.z) + 0x800) & 0xFFF;
        }
    }
}

void func_800C8BA0(void) {
    switch ((s16)(D_8005A648 - 5)) {
    case 1:
    case 3:
        func_800B3C7C();
        break;
    case 0:
    case 2:
    case 4:
    default:
        func_800B3B24();
        break;
    }
}
