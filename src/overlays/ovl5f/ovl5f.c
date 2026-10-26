#include "common.h"
#include "bash/main.h"
#include "bash/ovl5f.h"
#include "bash/engine.h"
#include "bash/tank.h"

s32 TankBotPickTarget(arg0)
s32 arg0;
{
    s32 sum;
    s32 i;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00) && TA_players[i].Energy > 0) {
            sum += D_8009D87C[i].unk4;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00) && TA_players[i].Energy > 0) {
            sum += D_8009D87C[i].unk4;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

s32 TankBotPickLinedUpTarget(arg0)
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
        if (j != arg0 && j != mate && !((h = &Heroes[j])->hero_status & 0xFA00) && TA_players[j].Energy > 0) {
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
        if (j != arg0 && j != mate && !(Heroes[j].hero_status & 0xFA00) && TA_players[j].Energy > 0) {
            sum += D_8009D87C[j].unk4;
            if (r < sum) {
                return j;
            }
        }
    }
    return -1;
}

s32 TankBotTryFire(s32 player, EXTengine *eng, TA_Bot *ai) {
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
        if (ai->FirePercentChance < (u32)i) {
            return 0;
        }
    }
    do {
        do {
            i = ai->Victim;
        } while (0);
    } while (0);
    if (i >= 0) {
    if (((Ovl5fModeView *)D_8005A614)->mode == 5) {
        te = Heroes[ai->Victim].hero_eng;
        a = get_angle(eng->unk10.x - te->unk10.x, eng->unk10.z - te->unk10.z);
        if ((u32)(((eng->unk1E - a) & 0xFFF) - 100) < 0xF39) {
            goto ret0;
        }
    } else {
    d = eng->unk1E;
    dir = (d + ((TA_players[player].TurretReqdDir - d) << 20 >> 22)) & 0xFFF;
    if (GetSandbagValueTank(eng->unk10.x + (-(D_80068BD4[(dir - 0x400) & 0xFFF].unk2 << 9) >> 12),
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
                te = hb[ai->Victim].hero_eng;
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
    ai->AIpad |= 0x8000;
    return 1;
    }
ret0:
    return 0;
}

void TankBotDodgeFrom(s32 player, s32 x, s32 z, EXTengine *eng, TA_Bot *ai) {
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
    ai->Doing = 2;
    speed = TA_players[player].Speed;
    ai->DoingTimer = (speed / 2 + 0x400) / speed;
    ai->AIpad = D_80051244[(a >> 9) & 7];
    if (D_8009D908 != 0) {
        return;
    }
    r = Random(4);
    switch (r) {
    case 0:
    case 1:
        if (ai->AIpad & 0xA0) {
            goto doz;
        }
    dox:
        if (ai->AIpad & 0xA0) {
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
            if (GetSandbagValueMiss(ex - 0x100, ez) != 0) {
                goto left;
            }
        right:
            ai->AIpad |= 0x80;
            break;
        }
        if (GetSandbagValueMiss(ex + 0x100, ez) != 0) {
            goto right;
        }
    left:
        ai->AIpad |= 0x20;
        break;
    case 2:
    case 3:
        if (ai->AIpad & 0x50) {
            goto dox;
        }
    doz:
        if (ai->AIpad & 0x50) {
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
            if (GetSandbagValueMiss(ex, ez + 0x100) != 0) {
                goto up;
            }
        down:
            ai->AIpad |= 0x10;
            break;
        }
        if (GetSandbagValueMiss(ex, ez - 0x100) != 0) {
            goto down;
        }
    up:
        ai->AIpad |= 0x40;
        break;
    }
}

s32 TankBotAvoidMines(s32 player, EXTengine *eng, TA_Bot *ai) {
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
    if (ai->Doing == 2) {
        return 0;
    }
    cur = ai->pMine;
    if (cur != NULL) {
        nas = cur;
        type = nas->Type;
        if ((type != Ovl5fASTIndex(0x1303)) | (nas->NASid == 0)) {
            ai->pMine = NULL;
            return 0;
        }
        te = nas->Engine;
        if (Ovl5fNear(ex = eng->unk10.x, te->unk10.x) || Ovl5fNear(ex = eng->unk10.z, te->unk10.z)) {
            goto hit;
        }
        return 0;
    }
    if (++ai->nMine >= 8) {
        ai->nMine = 0;
    }
    nas = TankMines[ai->nMine];
    if (nas == NULL) {
        return 0;
    }
    te = nas->Engine;
    if (Ovl5fNear(ex = eng->unk10.x, te->unk10.x) || Ovl5fNear(ex = eng->unk10.z, te->unk10.z)) {
    hit:
        ai->pMine = nas;
        TankBotDodgeFrom(player, te->unk10.x, te->unk10.z, eng, ai);
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5f/ovl5f", D_800B32B8);

void TankBotPlanSteerMove(s32 player, EXTengine *eng, TA_Bot *ai) {
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
        r = TankBotPickLinedUpTarget(player);
        ai->Victim = r;
        break;
    case 5:
    case 7:
    case 9:
    default:
        r = TankBotPickTarget(player);
        ai->Victim = r;
        break;
    }
    if (r < 0) {
        goto avg;
    }
    if (TA_players[player].TurretFiring < 0) {
        switch (Random(8)) {
        case 0:
        default:
            if (D_8009D908 == 0 && (u32)Random(D_8009D87C[player].unk0 + 1) < 13) {
                ai->DoingTimer = Random(30) + 15;
                ai->AIpad = 0;
                ai->Doing = 0;
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
    pp = PU_TA_list;
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
        ai->TargetX = e->unk10.x;
        ai->TargetZ = e->unk10.z;
    } else {
        r = Random(9);
        ai->TargetX = D_800CA17C[r][0];
        ai->TargetZ = D_800CA17C[r][1];
    }
    ai->Doing = 4;
    ai->DoingTimer = 0x5A;
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
    ai->TargetX = ax < 0x800 ? 0xE00 : 0x200;
    ai->TargetZ = az < 0x800 ? 0xE00 : 0x200;
    ai->Doing = 7;
    ai->DoingTimer = 0xB4;
    return;

retry:
    if (ai->Victim >= 0) {
        ai->Doing = 6;
        ai->DoingTimer = 0x5A;
        return;
    }
    if ((u32)Random(100) < 30) {
        goto find;
    }
    goto avg;
}

void TankBotPlanGridMove(s32 player, EXTengine *eng, TA_Bot *ai) {
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
        r = TankBotPickLinedUpTarget(player);
        break;
    case 5:
    case 7:
    case 9:
    default:
        r = TankBotPickTarget(player);
        break;
    }
    ai->Victim = r;
    ai->DoingTimer = 30;
    if (ai->pMine != NULL && TankBotAvoidMines(player, eng, ai) != 0) {
        return;
    }
    switch (Random(8)) {
    case 0:
        if (D_8009D908 == 0 && (u32)Random(D_8009D87C[player].unk0 + 1) < 13) {
            ai->DoingTimer = Random(30) + 15;
            ai->AIpad = 0;
            ai->Doing = 0;
            return;
        }
        goto chase;
    case 1:
    find:
        j = 0;
        pp = PU_TA_list;
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
        speed = TA_players[player].Speed;
        ai->Doing = 5;
        ai->DoingTimer = (speed / 2 + 0x200) / speed;
        for (;;) {
            switch (Random(4)) {
            case 0:
            px:
                x = ex + 0x200;
                if (D_800CB100 < x || GetSandbagValueMiss(x, ez) != 0) {
                    continue;
                }
                ai->AIpad = 0x20;
                return;
            case 1:
            nx:
                x = ex - 0x200;
                if (x < 0 || GetSandbagValueMiss(x, ez) != 0) {
                    continue;
                }
                ai->AIpad = 0x80;
                return;
            case 2:
            pz:
                z = ez + 0x200;
                if (D_800CB104 < z || GetSandbagValueMiss(ex, z) != 0) {
                    continue;
                }
                ai->AIpad = 0x10;
                return;
            case 3:
            nz:
                z = ez - 0x200;
                if (z < 0 || GetSandbagValueMiss(ex, z) != 0) {
                    continue;
                }
                ai->AIpad = 0x40;
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
        if (ai->Victim < 0) {
            goto find;
        }
        tab = TA_players;
        ta = &tab[player];
        delay = ta->MainWeaponFireDelay;
        if (delay < 5) {
        aim:
            e = Heroes[ai->Victim].hero_eng;
            a = get_angle(e->unk10.x - eng->unk10.x, e->unk10.z - eng->unk10.z);
            ai->Doing = 4;
            aspeed = TA_players[player].Speed;
            ai->DoingTimer = (aspeed / 2 + 0x200) / aspeed;
            ai->AIpad = D_80051244[(a >> 9) & 7];
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
        e = Heroes[ai->Victim].hero_eng;
        TankBotDodgeFrom(player, e->unk10.x, e->unk10.z, eng, ai);
        return;
    case 2:
        tab2 = TA_players;
        ta = &tab2[player];
        if (ta->SubWeaponRounds <= 0) {
            goto chase;
        }
        sspeed = ta->Speed;
        ai->AIpad |= 0x2000;
        ai->DoingTimer = (sspeed / 2 + 0x200) / sspeed;
        return;
    }
}

void TankBotSteer(s32 player) {
    TA_Bot *ai;
    EXTengine *eng;
    EXTengine *te;
    s32 buttons;
    s32 d;
    s32 a;

    eng = Heroes[player].hero_eng;
    ai = &TankBotAI[player];
    if (ai->FirePercentChance == 0) {
        ai->FirePercentChance = D_8009D87C[player].unk0 * 0x55 / 32 + 5;
    }
    if (--ai->DoingTimer <= 0) {
        TankBotPlanSteerMove(player, eng, ai);
    }
    buttons = 0;
    switch (ai->Doing) {
    case 4:
    case 7:
        d = eng->unk10.x - ai->TargetX;
        if ((u32)(d + 99) < 199) {
            ai->DoingTimer = 0;
        }
        a = get_angle(d, eng->unk10.z - ai->TargetZ) - eng->unk1E;
        a = (a << 20) >> 20;
        if (a > 40) {
            buttons = 0x820;
        }
        if (a < -40) {
            buttons = 0x880;
        }
        break;
    case 6:
        te = Heroes[ai->Victim].hero_eng;
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
    ai->AIpad &= ~0x8000;
    TankBotTryFire(player, eng, ai);
    if (ai->AIpad & 0x8000) {
        buttons |= 0x8000;
    }
    if (D_800CB284[player] != 0) {
        buttons |= 0x4000;
        D_800CB284[player] = 0;
    }
    set_joypad((u8 *)&D_80051338[player], buttons);
}

void TA_HeroBot(u32 player) {
    TA_Bot *ai;
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
        TankBotSteer(player);
        return;
    }
    eng = Heroes[player].hero->Engine;
    ai = &TankBotAI[player];
    if (ai->FirePercentChance == 0) {
        ai->FirePercentChance = D_8009D87C[player].unk0 * 0x55 / 32 + 5;
    }
    if (D_8005A648 != 5 && ai->Victim >= 0) {
        tgt = Heroes[ai->Victim].hero_eng;
        TA_players[player].TurretReqdDir =
            (get_angle(tgt->unk10.x - eng->unk10.x, tgt->unk10.z - eng->unk10.z) + 0x800) & 0xFFF;
    }
    if (ai->Doing == 5 && !(eng->unk10.x & 0x1FF) && !(eng->unk10.z & 0x1FF)) {
        TankBotPlanGridMove(player, eng, ai);
    }
    if (--ai->DoingTimer <= 0) {
        TankBotPlanGridMove(player, eng, ai);
    } else {
        TankBotAvoidMines(player, eng, ai);
        TankBotTryFire(player, eng, ai);
    }
    if (D_8009D908 == 0 && (u32)Random(D_8009D87C[player].unk0 + 1) >= 9) {
        ex = eng->unk10.x;
        ez = eng->unk10.z;
        for (i = 0; i < 8; i++) {
            nas = TankMines[i];
            if (nas != NULL) {
                te = nas->Engine;
                d = ex - te->unk10.x;
                if (d > 250) {
                    if (d < 500) {
                        ai->AIpad &= ~0x80;
                    }
                } else if (d < -250 && d > -500) {
                    ai->AIpad &= ~0x20;
                }
                d = ez - te->unk10.z;
                if (d > 250) {
                    if (d < 500) {
                        ai->AIpad &= ~0x40;
                    }
                } else if (d < -250 && d > -500) {
                    ai->AIpad &= ~0x10;
                }
            }
        }
    }
    set_joypad((u8 *)&D_80051338[player], ai->AIpad);
}

void TA_PreInitLevel(void) {
    void **p;
    void **p2;
    void **p3;
    void **p4;
    void **p5;
    void *t;
    Ovl5fGameView *g;

    D_8009D6E8.unk4 = D_800C8EA0;
    D_8009D6E8.unk38 = TA_HeroBot;
    D_8009D6E8.unk0 = (s32)TA_InitHero;
    D_8009D6E8.unk68 = TA_KeepInBounds;
    D_8009D6E8.unk14 = D_800C8EB4;
    D_8009D6E8.unk18 = TA_CreateShadow;
    D_8009D6E8.unk20 = TA_PreMoveHero;
    D_8009D6E8.unk24 = TA_MoveHero;
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

void TA_InitLevel(void) {
    Ovl5fSetupView *g;
    u8 *src1;
    u8 *src2;
    u8 *p;
    u8 *q;
    s32 i;
    s32 j;
    s32 k;

    D_800CAFF0 = 0;
    D_800CAFEC = 0;
    D_800CAFE8 = 0;
    D_800CAFE4 = 0;
    D_800CAFE0 = 0;
    for (j = 7; j >= 0; j--) {
        D_800CB264[j] = NULL;
    }
    D_800CB0F0 = 0;
    D_800CB260 = -1;
    D_800CB0E0 = 0;
    D_800CB0EC = 0;
    World_floor_level = 0;
    Init_Clock(0x140, 0x1E, 0);
    g = (Ovl5fSetupView *)D_8005A614;
    if (g->flags & 0x40) {
        if (g->w6C != 0) {
            switch (g->mode) {
            case 6:
                Set_Clock(0x30, 0x81C0);
                break;
            case 8:
                Set_Clock(0x30, 0x81C0);
                break;
            case 7:
                Set_Clock(0x25, 0x81C0);
                break;
            case 5:
                Set_Clock(0x25, 0x81C0);
                break;
            case 9:
                Clock_Off();
                break;
            }
        } else {
            switch (g->mode) {
            case 6:
                Set_Clock(0x40, 0x81C0);
                break;
            case 8:
                Set_Clock(0x40, 0x81C0);
                break;
            case 7:
                Set_Clock(0x30, 0x81C0);
                break;
            case 5:
                Set_Clock(0x40, 0x81C0);
                break;
            case 9:
                Clock_Off();
                break;
            }
        }
    } else if (g->mode == 9) {
        Clock_Off();
    } else {
        Set_Clock(0x130, 0x81C0);
    }
    if (D_8005A648 == 5) {
        func_8001682C((s32 *)D_80053774);
    } else {
        func_8001682C((s32 *)D_80053694);
    }
    if (D_8005A648 == 7) {
        func_8001682C((s32 *)D_800537AC);
    }
    func_8001682C((s32 *)D_8005365C);
    func_8001682C((s32 *)(D_8005365C - 0x658));
    func_8001682C(*(s32 **)D_80058AD0);
    ((Ovl5fScreen *)D_8009F6F4)->w10 = 0x140;
    ((Ovl5fScreen *)D_8009F6F4)->w14 = 0x15E;
    CAM5F->w0 = 0;
    CAM5F->w8 = 0xE98;
    CAM5F->h54 = 0x200;
    CAM5F->w18 = 0x200;
    CAM5F->w4 = 0;
    CAM5F->h56 = 0;
    CAM5F->h58 = 0;
    CAM5F->w10 = 0x90;
    for (i = 0; i < 4; i++) {
        SetHeroAction(i, 0x6C);
    }
    for (i = 2; i >= 0; i--) {
        PU_TA_list[i] = NULL;
    }
    for (i = 7; i >= 0; i--) {
        TankMines[i] = NULL;
    }
    switch (D_8005A648) {
    default:
    case 5:
        src1 = src2 = D_800CA30C;
        D_800CAFF4 = 9;
        D_800CAFF8 = 8;
        CAM5F->wC = 0x930;
        CAM5F->w14 = 0x1C0;
        D_800CB100 = 0x1100;
        D_800CB104 = 0xF00;
        PlaySfx(0x50F, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    case 6:
        src2 = D_800CA1C4;
        D_800CAFF4 = 8;
        D_800CAFF8 = 8;
        CAM5F->wC = 0x800;
        CAM5F->w14 = 0x2A0;
        D_800CB100 = 0xF00;
        D_800CB104 = 0xF00;
        if (D_8005A646 & 0x80) {
            CreateNAS(0x800, -0x3C, 0x800, 0x1318, 0x11);
            src1 = D_800CA204;
        } else {
            src1 = src2;
        }
        break;
    case 7:
        InitAmbience(&D_800C8ED4);
        CreateNAS(0x800, 0, 0x800, 0x131A, 0x11);
        src1 = src2 = D_800CA244;
        D_800CAFF4 = 8;
        D_800CAFF8 = 8;
        CAM5F->wC = 0x800;
        CAM5F->w14 = 0x220;
        CAM5F->h54 = 0x1EA;
        CAM5F->w10 = 0;
        D_800CB100 = 0xF00;
        D_800CB104 = 0xF00;
        break;
    case 8:
        D_800CAFF4 = 9;
        D_800CAFF8 = 7;
        CAM5F->wC = 0x930;
        CAM5F->w14 = 0x1C0;
        D_800CB204 = 0;
        CreateNAS(0, 0, 0, 0x1310, 0x11);
        CreateNAS(0, 0, 0, 0x1309, 0x11);
        src1 = src2 = D_800CA284;
        D_800CB100 = 0x1100;
        D_800CB104 = 0xD00;
        break;
    case 9:
        g = (Ovl5fSetupView *)D_8005A614;
        if (g->w6C == 0) {
            g->b51 = 9;
        }
        g->b52 = 9;
        g->b53 = 9;
        Heroes[2].hero_name = 9;
        Heroes[3].hero_name = 9;
        if (g->w6C == 0) {
            Heroes[1].hero_name = 9;
        }
        CAM5F->h54 = 0x192;
        D_800CAFF4 = 9;
        D_800CAFF8 = 8;
        CAM5F->wC = 0x930;
        CAM5F->w10 = -0x70;
        CAM5F->w14 = 0x1C0;
        D_800CB100 = 0x1100;
        D_800CB104 = 0xF00;
        CreateNAS(0x600, 0, 0x900, 0x131F, 0x11);
        CreateNAS(0xC00, 0, 0x900, 0x131E, 0x11);
        CreateNAS(0x900, 0x7D0, 0x10E0, 0x1313, 0x11);
        src1 = src2 = D_800CA2C4;
        D_800CB254 = 0;
        D_800CB258 = 0;
        break;
    }
    p = D_800CACB4;
    q = D_800CACFC;
    for (i = 0; i < 0x48; i++) {
        *p++ = *src2++;
        *q++ = *src1++;
    }
    for (k = 0; k < 7; k++) {
        for (j = 0; j < 9; j++) {
            D_800CB108[j][k] = 0;
        }
    }
    D_800CB0E4 = 0;
    D_800CB0F8 = 0x50;
    D_800CB248 = 0x1E0 / (D_8009D88E + 1);
}

void TA_UpdateLevel(void) {
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
        ta = &TA_players[i];
        eng = Heroes[i].hero_eng;
        e = ta->Energy;
        ta->PosX = eng->unk10.x;
        ta->PosZ = eng->unk10.z;
        if (e <= 0) {
            SetHullSFX(i, eng, 0);
            SetTurretSFX(i, (s32)eng, 0);
            SetFireBreatherSFX(i, (s32)eng, 0);
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
        func_80029100((obj_textureset *)(TANK_MODEL(56) + 0x10));
    }
    if (D_8005A648 == 5 && TANK_MODEL_ON(base, 60)) {
        func_80029100((obj_textureset *)(TANK_MODEL(60) + 0x10));
    }
    if (D_8005A648 == 9 && TANK_MODEL_ON(b2 = D_80052A54, 57)) {
        func_80029100((obj_textureset *)(b2 + 57 * 0x38 + 0x10));
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

void FinishTankLevel(void) {
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
                TA_players[i].RequestQuickFinish = 1;
            }
        }
    }
}

void DamageTankHero(s32 player, s32 damage) {
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
    ta = &TA_players[player];
    if (D_8005A648 == 9 && (u32)(player - 2) < 2) {
        SetMoeJoeMode(player, 0x69);
    } else {
        SetHeroAction(player, 0x69);
    }
    if (D_8005A614[player + 0x5C] != 0 || (((Ovl5fGameView *)D_8005A614)->w4 & 0x80000001) != 0x80000001) {
        ta->Energy -= damage;
    }
    if (ta->Energy <= 0) {
        SetHullSFX(player, eng, 0);
        SetTurretSFX(player, (s32)eng, 0);
        SetFireBreatherSFX(player, (s32)eng, 0);
        ta->Energy = 0;
        if ((((Ovl5fGameView *)D_8005A614)->w4 & 0x80000002) == 0x80000002) {
            ta->Energy = 2;
        } else {
            Heroes[player].hero_status |= 0x2000;
        }
    }
    Heroes[player].hero_health = ta->Energy;
}

void TA_InitHero(u32 player) {
    HeroInfo *h;
    HeroInfo *hh;
    HeroInfo *hb;
    EXTengine *eng;
    subengine *sub;
    TA_player *ta;
    TA_Bot *bot;
    EngPtr3 *pp;
    EngPtr3 *pb;
    PanelPrim *pnl;
    NAS *nas;
    u8 *g;
    u8 *g1;
    u8 *g2;
    s32 n;

    ((PadButtons *)D_80051338)[player].unk50 = 0;
    ((PadButtons *)D_80051338)[player].unk54 = 0;
    hb = Heroes;
    h = &hb[player];
    if (h->hero_name == 9) {
        return;
    }
    g = D_8005A614;
    if (g[player + 0x50] == 9) {
        return;
    }
    eng = h->hero_eng;
    eng->unk10.x = (eng->unk10.x + 0x80) & ~0xFF;
    eng->unk10.z = (eng->unk10.z + 0x80) & ~0xFF;
    SetHeroAction(player, 0x6C);
    func_80018F3C(eng, 2);
    sub = eng->sub;
    eng->unk5D = 1;
    eng->unk1E = 0x400;
    sub->unk51 = 1;
    if (((Ovl5fModeView *)g)->mode == 5) {
        sub->unk08 = 0x48;
    }
    sub = sub->next;
    sub->unk51 = 1;
    if (((Ovl5fModeView *)g)->mode == 5) {
        sub->unk6C = D_80053774;
        sub->unk74 = 0x2005;
        sub->unk08 = -0x100;
    } else {
        sub->unk6C = D_80053694;
        sub->unk74 = 0x2009;
    }
    sub = sub->next;
    sub->unk51 = 1;
    if (D_8005A648 == 5) {
        sub->unk6C = D_80053774;
        sub->unk74 = 0x2001;
    } else {
        sub->unk6C = D_80053694;
        sub->unk74 = 0x4300;
    }
    SetTankHeroIdle(player);
    switch ((u32)player) {
    case 0:
        TA_players[player].MoveDir = 2;
        TA_players[player].FaceDir = 2;
        TA_players[player].RunnerMoving = 2;
        TA_players[player].RunnerFacing = 2;
        break;
    case 1:
        TA_players[player].MoveDir = 3;
        TA_players[player].FaceDir = 3;
        TA_players[player].RunnerMoving = 3;
        TA_players[player].RunnerFacing = 3;
        break;
    case 2:
        TA_players[player].MoveDir = 0;
        TA_players[player].FaceDir = 0;
        TA_players[player].RunnerMoving = 0;
        TA_players[player].RunnerFacing = 0;
        break;
    case 3:
        TA_players[player].MoveDir = 1;
        TA_players[player].FaceDir = 1;
        TA_players[player].RunnerMoving = 1;
        TA_players[player].RunnerFacing = 1;
        break;
    }
    ta = &TA_players[player];
    ta->Speed = D_800C8F14[Heroes[player].hero_name - 1];
    hh = &Heroes[player];
    ta->SpeedX = 0;
    ta->SpeedZ = 0;
    g1 = D_8005A614;
    if (((Ovl5fModeView *)g1)->mode == 5) {
        ta->Speed = ta->Speed * 3 / 2;
    }
    if (g1[player + 0x5C] != 0) {
        ta->Speed = ta->Speed * (D_8009D87C[player].unk0 + 0x54) / 100;
    }
    ta->Energy = 0x63;
    hh->hero_health = ta->Energy;
    ta->PosX = eng->unk10.x;
    ta->PosZ = eng->unk10.z;
    ta->RequestQuickFinish = 0;
    ta->SubWeaponFireDelay = 0;
    if (((Ovl5fModeView *)g1)->mode == 5 && (((Ovl5fModeView *)g1)->flags & 0x80)) {
        ta->SubWeaponRounds = 0;
    } else {
        ta->SubWeaponRounds = 3;
    }
    ta->FireBreatherSFXDuration = 0;
    ta->TurretSFXDuration = 0;
    ta->TurretReqdDir = -1;
    ta->HangaboutTime = -0x5A;
    ta->IdleAnimCt = Random(0x1E);
    ta->TurretFiring = -1;
    ta->TauntDelay = 0;
    ta->ForceFieldDelay = 0;
    ResetToBasicWeapon(player);
    if (D_8005A614[player + 0x5C] != 0) {
        ta->MainWeaponFireDelay = 0x96;
    } else {
        ta->MainWeaponFireDelay = 0;
    }
    ta->MainWeaponNas[0] = NULL;
    n = Heroes[player].hero_name;
    if (n == 0 || n >= 7) {
        ta->MainWeaponNas[2] = (NAS *)0xFFEEEEFF;
    } else {
        ta->MainWeaponNas[1] = (NAS *)0xFFEEEEFF;
        ta->MainWeaponNas[2] = (NAS *)0xFFEEEEFF;
    }
    if (D_8005A648 == 5) {
        nas = CreateNAS(eng->unk10.x, 0, eng->unk10.z, 0x1320, 0x11);
        if (nas != NULL) {
            nas->UserInfo.user1 = player;
        }
    }
    g2 = D_8005A614;
    if (g2[player + 0x5C] != 0) {
        bot = &TankBotAI[player];
        bot->Doing = 0;
        bot->DoingTimer = 0;
        bot->Victim = 0;
        bot->pMine = NULL;
        bot->pMissile = NULL;
        bot->AIpad = 0;
        bot->FirePercentChance = 0;
    }
    pnl = D_80099A64[player];
    pnl->unk0 |= 0x8000;
    pb = D_8009AE40;
    pp = &pb[player];
    pp->a = &ta->MainWeaponType;
    pp->b = &ta->MainWeaponRounds;
    pp->c = &ta->SubWeaponRounds;
    if (((Ovl5fModeView *)g2)->mode == 5 && g2[player + 0x5C] == 0 && (((Ovl5fModeView *)g2)->flags & 0x80)) {
        if (player == 0) {
            nas = CreateNAS(0x880, 0, 0x880, 0x1322, 0x11);
        } else {
            nas = CreateNAS(0x980, 0, 0x880, 0x1322, 0x11);
        }
        if (nas != NULL) {
            nas->UserInfo.user1 = player;
        }
    }
}

void TA_MoveHero(u32 player) {
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
    ta = &TA_players[player];
    AnimateTankTurret(player);
    if (D_8005A648 != 5) {
        x = eng->unk10.x;
        dz = eng->unk10.z - eng->OldXYZ.z;
        if (x != eng->OldXYZ.x || dz != 0) {
            if (ta->HangaboutTime != 0) {
                ta->HangaboutTime--;
            }
            SetHullSFX(player, eng, D_800C94C8[player]);
        } else {
            if (!(hero->hero_status & 0xFA00)) {
                ta->HangaboutTime++;
            }
            SetHullSFX(player, eng, 0);
        }
        if (ta->TurretSFXDuration != 0) {
            if (--ta->TurretSFXDuration == 0) {
                SetTurretSFX(player, (s32)eng, 0);
            } else {
                SetTurretSFX(player, (s32)eng, D_800C94D8[player]);
            }
        }
    } else {
        if (((PadButtons *)D_80051338)[player].unk50 & 0x800) {
            SetHullSFX(player, eng, D_800C9408[player]);
        } else {
            SetHullSFX(player, eng, 0);
        }
    }
    if (((Ovl5fModeView *)D_8005A614)->mode == 8 && D_8005A614[player + 0x5C] == 0 &&
        (((Ovl5fGameView *)D_8005A614)->flags & 0x80) && ta->HangaboutTime >= 30 && D_800CB0F0 == 0 &&
        D_800CB0F8 <= 0 && (eng->unk10.z > 0x400 || (u32)(eng->unk10.x - 0x301) < 0xDFF)) {
        nas = CreateNAS(eng->unk10.x, -0x8FC, eng->unk10.z, 0x1308, 0x11);
        if (nas != NULL) {
            nas->UserInfo.status = 6;
            D_800CB0F8 = 60;
            nas->Engine->sub->unk74 = 0x202B;
            ta->HangaboutTime = 0;
        }
    }
}

void AnimateTank(u32 player) {
    if (D_8005A614[player + 0x5C] != 0) {
        if (++TA_players[player].IdleAnimCt >= 0x50 && D_800CB0F0 == 0) {
            TA_players[player].IdleAnimCt = Random(0x1E);
            SetHeroSeq(player, 0x5A);
            TA_players[player].IdleAnimCt = 0;
            PlaySfx(D_800C94A8[Heroes[player].hero_name - 1], NULL, &Heroes[player].hero_eng->unk10, 0x1000, 0x1E00,
                    0);
            return;
        }
    }
    SetHeroSeq(player, 0x59);
}

void ResetToBasicWeapon(s32 player) {
    TA_player *ta;
    subengine *sub;

    ta = &TA_players[player];
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

void AnimateTankTurret(s32 player) {
    TA_player *ta;
    TA_player *ta2;
    TA_player *tab;
    EXTengine *eng;
    EXTengine *e;
    EXTengine *se;
    EXTengine *se2;
    subengine *sub;
    subengine *sub2;
    NAS *n;
    NAS *puff;
    PeriscopeShellExt *fux;
    s32 a;
    s32 dx;
    s32 dz;
    s32 pad;
    u16 anim;

    ta = &TA_players[player];
    eng = Heroes[player].hero_eng;
    sub = Heroes[player].hero_sub->next;
    sub2 = sub->next;
    if (D_8005A648 == 5) {
        pad = ((PadButtons *)D_80051338)[player].unk50;
        if (pad & 0x800) {
            goto right;
        } else if (pad & 0x200) {
        right:
            sub->unk74 = 0x2006;
            sub->unk14 -= 0x2A7;
        } else {
            sub->unk74 = 0x2005;
            sub->unk14 -= 0xB3;
        }
        sub->unk52 = 1;
    }
    if (ta->TurretFiring >= 0) {
        a = *ta->TurretAnimPtr++;
        if (a != 0) {
            if (D_8005A648 == 5) {
                sub2->unk74 = a;
            } else {
                sub->unk74 = a;
            }
        } else {
            if (D_8005A648 == 5) {
                switch (ta->MainWeaponType) {
                case 9:
                default:
                    sub2->unk74 = 0x4300;
                    ta->TurretAnimPtr = D_800C9A54;
                    break;
                case 11:
                    sub2->unk74 = 0x4200;
                    ta->TurretAnimPtr = D_800C9AA4;
                    break;
                case 10:
                    sub2->unk74 = 0x4000;
                    ta->TurretAnimPtr = D_800C9AF4;
                    break;
                case 8:
                    sub2->unk74 = 0x4100;
                    ta->TurretAnimPtr = D_800C9B44;
                    break;
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
            }
            ta->TurretFiring = -1;
            if (ta->MainWeaponRounds <= 0) {
                ResetToBasicWeapon(player);
            }
        }
        if (sub->unk74 == 0x4004) {
            e = Heroes[player].hero_eng;
            puff = Ovl5fSpawnBit1605Nas(&e->unk10, 0);
            if (puff != NULL) {
                Ovl5fPuffSetAngle(puff, eng->unk1E);
            }
            tab = TA_players;
            ta2 = &tab[player];
            if (ta2->MainWeaponType == 3) {
                n = CreateNAS(e->unk10.x, e->unk10.y - 0x12C, e->unk10.z, 0x130B, 0x11);
                if (n != NULL) {
                    fux = AddExt(n, 0x38, 0x10000000);
                    if (fux == NULL) {
                        KillNAS(n);
                        return;
                    }
                    n->FUX = (EXT *)fux;
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
                    ta2->MainWeaponRounds--;
                    se2 = n->Engine;
                    PlaySfx(0x4F3, &se2->sfx, &se2->unk10, 0x1000, 0x1E00, 0);
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

void UpdateTankHeroWeapons(s32 player) {
    EXTengine *eng;
    TA_player *ta;
    subengine *sub;
    subengine *sub2;
    HeroInfo *h;
    HeroInfo *hb;
    NAS *n;
    NAS *mine;
    NAS *obj;
    EXTobject_TA *fux;
    EXTengine *e;
    s32 d;
    s32 i;
    s32 a;
    s32 dir;
    u32 pad;

    if (Heroes[player].hero_status & 0xFA00) {
        return;
    }
    if (D_800CB0F0 != 0) {
        return;
    }
    eng = Heroes[player].hero_eng;
    ta = &TA_players[player];
    sub = eng->sub->next;
    sub2 = sub->next;
    if (ta->TurretReqdDir != -1) {
        d = ta->TurretReqdDir - eng->unk1E;
        d = (d << 20) >> 22;
        eng->unk5E = 1;
        eng->unk1E += d;
        sub2->unk52 = 1;
        sub2->unk12 -= d;
        if ((u32)(d + 3) < 7) {
            ta->TurretReqdDir = -1;
        }
    } else if (D_8005A648 != 5) {
        pad = ((PadButtons *)D_80051338)[player].unk50;
        if (pad & 0xA00) {
            eng->unk5E = 1;
            eng->unk1E = (eng->unk1E + 0x40) & 0xFFF;
            sub2->unk52 = 1;
            sub2->unk12 = (sub2->unk12 - 0x40) & 0xFFF;
            ta->TurretSFXDuration = 2;
        } else if (pad & 0x500) {
            eng->unk5E = 1;
            eng->unk1E = (eng->unk1E - 0x40) & 0xFFF;
            sub2->unk52 = 1;
            sub2->unk12 = (sub2->unk12 + 0x40) & 0xFFF;
            ta->TurretSFXDuration = 2;
        }
    }
    if (ta->MainWeaponFireDelay == 0) {
        if (ta->TurretFiring < 0) {
            for (i = 0; i < 3; i++) {
                if (ta->MainWeaponNas[i] == NULL) {
                    break;
                }
            }
            if (i < 3 && (((PadButtons *)D_80051338)[player].unk54 & 0x8000)) {
                switch (Random(4)) {
                default:
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                    SetHeroSeq(player, 0x5D);
                    break;
                case 5:
                case 6:
                    SetHeroSeq(player, 0x5E);
                    break;
                case 7:
                    SetHeroSeq(player, 0x5F);
                    break;
                }
                if (ta->MainWeaponType == 3) {
                    ta->MainWeaponFireDelay = 0x3C;
                } else {
                    ta->MainWeaponFireDelay = 0xA;
                }
                switch (ta->MainWeaponType) {
                default:
                case 11:
                    n = CreateNAS(eng->unk10.x, eng->unk10.y - 0x113, eng->unk10.z, 0x130C, 0x11);
                    if (n == NULL) {
                        break;
                    }
                    ta->MainWeaponNas[i] = n;
                    ta->TurretFiring = 0xB;
                    if (D_8005A648 == 5) {
                        sub->unk74 = 0x427F;
                        ta->TurretAnimPtr = D_800C9AC0;
                    } else {
                        sub->unk74 = 0x427F;
                        ta->TurretAnimPtr = D_800C98B0;
                    }
                    if (Heroes[player].hero_name == 3) {
                        PlaySfx(0x4CC, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x4CD, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    }
                    n->Engine->unk10.x += (-(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12) + ta->SpeedX;
                    n->Engine->unk10.z += (-(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 400) >> 12) + ta->SpeedZ;
                    n->Engine->unk1E = eng->unk1E;
                    fux = (EXTobject_TA *)n->FUX;
                    fux->Owner = player;
                    fux->SpeedX = -(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 0x4A) >> 12;
                    fux->SpeedZ = -(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 0x4A) >> 12;
                    if (D_8005A648 == 5) {
                        BumpSpeedsForSwamp(fux);
                    }
                    fux->Status = 1;
                    obj = Ovl5fSpawnBit1605Nas(&n->Engine->unk10, 7);
                    if (obj != NULL) {
                        Ovl5fPuffSetAngle(obj, eng->unk1E);
                    }
                    break;
                case 0:
                    sub->unk74 = 0x3FFF;
                    ta->TurretFiring = 0;
                    ta->TurretAnimPtr = D_800C9980;
                    break;
                case 3:
                    sub->unk74 = 0x3FFF;
                    ta->TurretFiring = 3;
                    ta->TurretAnimPtr = D_800C9980;
                    break;
                case 8:
                    n = CreateNAS(eng->unk10.x, eng->unk10.y - 0xFA, eng->unk10.z, 0x130D, 0x11);
                    if (n == NULL) {
                        break;
                    }
                    ta->MainWeaponNas[i] = n;
                    ta->TurretFiring = 8;
                    if (D_8005A648 == 5) {
                        sub->unk74 = 0x417F;
                        ta->TurretAnimPtr = D_800C9B60;
                    } else {
                        sub->unk74 = 0x41FF;
                        ta->TurretAnimPtr = D_800C9918;
                    }
                    if (Heroes[player].hero_name == 2) {
                        PlaySfx(0x4CB, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x4D0, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    }
                    fux = (EXTobject_TA *)n->FUX;
                    n->Engine->unk10.x += (-(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12) + ta->SpeedX;
                    n->Engine->unk10.z += (-(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 400) >> 12) + ta->SpeedZ;
                    n->Engine->unk1E = eng->unk1E;
                    fux->SpeedX = -(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 0x36) >> 12;
                    fux->SpeedZ = -(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 0x36) >> 12;
                    if (D_8005A648 == 5) {
                        BumpSpeedsForSwamp(fux);
                    }
                    fux->Status = 1;
                    fux->Owner = player;
                    ta->FireBreatherSFXDuration = 4;
                    obj = Ovl5fSpawnBit1605Nas(&n->Engine->unk10, 8);
                    if (obj != NULL) {
                        Ovl5fPuffSetAngle(obj, eng->unk1E);
                    }
                    break;
                case 9:
                    n = CreateNAS(eng->unk10.x, eng->unk10.y - 0x104, eng->unk10.z, 0x130F, 0x11);
                    if (n == NULL) {
                        break;
                    }
                    ta->MainWeaponNas[i] = n;
                    ta->TurretFiring = 9;
                    if (D_8005A648 == 5) {
                        sub->unk74 = 0x437F;
                        ta->TurretAnimPtr = D_800C9A70;
                    } else {
                        ta->TurretAnimPtr = D_800C987C;
                        sub->unk74 = 0x417F;
                    }
                    if (Heroes[player].hero_name == 1) {
                        PlaySfx(0x4CA, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x4CB, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    }
                    fux = (EXTobject_TA *)n->FUX;
                    n->Engine->unk10.x += (-(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12) + ta->SpeedX;
                    n->Engine->unk10.z += (-(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 400) >> 12) + ta->SpeedZ;
                    n->Engine->unk1E = eng->unk1E;
                    fux->SpeedX = -(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 0x2C) >> 12;
                    fux->SpeedZ = -(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 0x2C) >> 12;
                    if (D_8005A648 == 5) {
                        BumpSpeedsForSwamp(fux);
                    }
                    fux->Status = 1;
                    fux->Owner = player;
                    obj = Ovl5fSpawnBit1605Nas(&n->Engine->unk10, 0xF);
                    if (obj != NULL) {
                        Ovl5fPuffSetAngle(obj, eng->unk1E);
                    }
                    break;
                case 10:
                    n = CreateNAS(eng->unk10.x, eng->unk10.y - 0x14A, eng->unk10.z, 0x130E, 0x11);
                    if (n == NULL) {
                        break;
                    }
                    ta->MainWeaponNas[i] = n;
                    ta->TurretFiring = 0xA;
                    if (D_8005A648 == 5) {
                        sub->unk74 = 0x407F;
                        ta->TurretAnimPtr = D_800C9B10;
                    } else {
                        sub->unk74 = 0x40FF;
                        ta->TurretAnimPtr = D_800C98E4;
                    }
                    if (Heroes[player].hero_name == 5) {
                        PlaySfx(0x4CE, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x4CF, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    }
                    fux = (EXTobject_TA *)n->FUX;
                    n->Engine->unk10.x += (-(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12) + ta->SpeedX;
                    n->Engine->unk10.z += (-(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 400) >> 12) + ta->SpeedZ;
                    n->Engine->unk1E = eng->unk1E;
                    fux->SpeedX = -(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 0x2C) >> 12;
                    fux->SpeedZ = -(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 0x2C) >> 12;
                    if (D_8005A648 == 5) {
                        BumpSpeedsForSwamp(fux);
                    }
                    fux->Status = 1;
                    fux->Owner = player;
                    obj = Ovl5fSpawnBit1605Nas(&n->Engine->unk10, 6);
                    if (obj != NULL) {
                        Ovl5fPuffSetAngle(obj, eng->unk1E);
                    }
                    break;
                case 2:
                    dir = eng->unk1E;
                    for (i = 0; i < 2; i++) {
                        n = CreateNAS(eng->unk10.x, eng->unk10.y - 0xE6, eng->unk10.z, 0x1302, 0x11);
                        if (n != NULL) {
                            PlaySfx(0x522, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                            e = n->Engine;
                            a = dir - D_800C93AC[i];
                            e->unk10.x -= (D_80068BD4[(a - 0x400) & 0xFFF].unk2 * 0x19) >> 8;
                            e->unk10.z -= (D_80068BD4[a & 0xFFF].unk2 * 0x19) >> 8;
                            e->unk1E = eng->unk1E;
                            fux = (EXTobject_TA *)n->FUX;
                            fux->SpeedX = -(D_80068BD4[(dir - 0x400) & 0xFFF].unk2 * 200) >> 12;
                            fux->SpeedZ = -(D_80068BD4[dir & 0xFFF].unk2 * 200) >> 12;
                            fux->Status = 1;
                            fux->Owner = player;
                        }
                    }
                    sub->unk74 = 0x200C;
                    ta->TurretFiring = 2;
                    ta->TurretAnimPtr = D_800C99B4;
                    ta->MainWeaponRounds--;
                    break;
                }
            } else if (D_8005A614[player + 0x5C] == 0 &&
                       (((PadButtons *)D_80051338)[player].unk54 & 0x8000)) {
                PlaySfx(0x4F1, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
    } else {
        ta->MainWeaponFireDelay--;
        if (D_8009D908 != 0 && ta->MainWeaponFireDelay != 0) {
            ta->MainWeaponFireDelay--;
        }
    }
    if (ta->SubWeaponFireDelay != 0) {
        ta->SubWeaponFireDelay--;
    }
    if ((((PadButtons *)D_80051338)[player].unk54 & 0x2000) &&
        ((Ovl5fModeView *)D_8005A614)->mode < 10 && ((Ovl5fModeView *)D_8005A614)->mode >= 5) {
        if (ta->SubWeaponRounds != 0) {
            if (ta->SubWeaponFireDelay == 0) {
                if (((Ovl5fModeView *)D_8005A614)->mode == 5) {
                    for (i = 0; i < 8; i++) {
                        if (TankMines[i] != NULL && TankMines[i]->UserInfo.user1 == player) {
                            goto subdone;
                        }
                    }
                    for (i = 0; i < 8; i++) {
                        if (TankMines[i] == NULL) {
                            break;
                        }
                    }
                    mine = CreateNAS(eng->unk10.x, eng->unk10.y + 0x14, eng->unk10.z, 0x131D, 0x11);
                    if (mine != NULL) {
                        TankMines[i] = mine;
                        mine->UserInfo.user1 = player;
                    }
                    ta->SubWeaponFireDelay = 0x1E;
                    PlaySfx(0x50A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    PlaySfx(D_800C9418[player], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    ta->SubWeaponRounds--;
                    SetHeroSeq(player, 0x5B);
                } else {
                    Ovl5fTriggerNear(eng->unk10.x, eng->unk10.z, 0x100);
                    CreateNAS(eng->unk10.x, eng->unk10.y + 0x14, eng->unk10.z, 0x1303, 0x11);
                    ta->SubWeaponFireDelay = 0x78;
                    PlaySfx(0x4E9, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    PlaySfx(D_800C9418[player], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    ta->SubWeaponRounds--;
                    SetHeroSeq(player, 0x5B);
                }
            } else if (D_8005A614[player + 0x5C] == 0) {
                PlaySfx(0x4F1, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
        } else if (D_8005A614[player + 0x5C] == 0) {
            PlaySfx(0x4F2, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
    }
subdone:
    if (((Ovl5fModeView *)D_8005A614)->mode == 5) {
        if (ta->ForceFieldDelay != 0) {
            ta->ForceFieldDelay--;
        } else if (((PadButtons *)D_80051338)[player].unk54 & 0x4000) {
            obj = CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1321, 0x11);
            PlaySfx(D_800C93F8[player], NULL, NULL, 0x1000, 0x1E00, 0);
            if (obj != NULL) {
                obj->UserInfo.user1 = player;
            }
            Heroes[player].hero_immunity = 0xA;
            if (D_8005A614[player + 0x5C] == 0) {
                ta->ForceFieldDelay = 0x3C;
            } else {
                ta->ForceFieldDelay = (0x20 - D_8009D87C[player].unk0) * 0x3C / 32 + 0xF;
            }
        }
    }
    if (ta->TauntDelay <= 0) {
        if (((PadButtons *)D_80051338)[player].unk50 & 0x1000) {
            hb = Heroes;
            h = &hb[player];
            if (!(h->hero_status & 0xFA00)) {
                switch (h->hero_mode) {
                case 0x6C:
                case 0x6E:
                case 0x6F:
                    ta->TauntDelay = 0x5A;
                    SetHeroSeq(player, 0x5A);
                    PlaySfx(D_800C94A8[h->hero_name - 1], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    break;
                }
            }
        }
    } else {
        ta->TauntDelay--;
    }
}

void SetTankHeroIdle(u32 player) {
    s32 done;
    u8 unused[8];

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
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
        SetHeroSeq(player, 0x59);
        eng = Heroes[player].hero_eng;
        if (TA_players[player].HullSFX != 0) {
            TA_players[player].HullSFX = 0;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
        }
        if (TA_players[player].TurretSFX != 0) {
            TA_players[player].TurretSFX = 0;
            StopSfx(TA_players[player].TurretSFXHandler);
        }
        if (TA_players[player].FireBreatherSFX != 0) {
            TA_players[player].FireBreatherSFX = 0;
            StopSfx(TA_players[player].FireBreatherSFXHandler);
        }
        AnimateTank(player);
    }
}

void SetTankHeroTurn(u32 player) {
    s32 done;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
        D_800CB0F0 = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        SetHeroSeq(player, 0x59);
        PlaySfx(D_800C94E8[player], NULL, &Heroes[player].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void TankHeroTurn(u32 player) {
    s32 done;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
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

        UpdateTankHeroWeapons(player);
        if (func_8008502C(player) != 0) {
            AnimateTank(player);
        }
        hb = Heroes;
        h = &hb[player];
        ta = &TA_players[player];
        eng = h->hero_eng;
        sub = h->hero_sub->next->next;
        sub->unk12 += ta->ModeSpeed;
        sub->unk52 = 1;
        if (--ta->ModeCounter == 0) {
            SetHeroAction(player, 0x6E);
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

void TankHeroSteerMove(s32 player) {
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

    ta = &TA_players[player];
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
    o = &TA_players[hit];
    o->SpeedX += ta->SpeedX / 2;
    ta->SpeedX /= 4;
    o->SpeedZ += ta->SpeedZ / 2;
    ta->SpeedZ /= 4;
}

void TankHeroGridMove(s32 player) {
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
    ta = &TA_players[player];
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

void TankHeroMove(u32 player) {
    s32 done;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
        D_800CB0F0 = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        TA_player *ta;
        EXTengine *eng;
        s32 speed;
        s32 pad;
        s32 x;
        s32 z;
        s32 dir;
        s32 half;
        s32 off;
        s32 pos;
        s32 frac;
        s32 hit;

        UpdateTankHeroWeapons(player);
        if (func_8008502C(player) != 0) {
            AnimateTank(player);
        }
        if (D_800CB0F0 != 0) {
            return;
        }
        ta = &TA_players[player];
        eng = Heroes[player].hero_eng;
        if (ta->RequestQuickFinish != 0) {
            DamageTankHero(player, 9999);
            return;
        }
        if (D_8005A648 == 5) {
            TankHeroSteerMove(player);
            return;
        }
        if (D_8005A648 == 7 || D_8005A648 == 9 || D_8005A648 == 5) {
            TankHeroGridMove(player);
            return;
        }
        half = (speed = ta->Speed) / 2;
        pad = ((PadButtons *)D_80051338)[player].unk50;
        x = eng->unk10.x;
        z = eng->unk10.z;
        dir = pad & 0xF0;
        if (dir == 0) {
            return;
        }
        if (!(x & 0xFF) && !(z & 0xFF) && (x & 0x100) && (z & 0x100)) {
            if ((pad & 0x20) && x != 0xF00 && Ovl5fMapCell(x + 0x100, z) == 0) {
                switch (ta->MoveDir) {
                case 0:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = -0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 3;
                    return;
                case 2:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = 0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 3;
                    return;
                case 1:
                    ta->MoveDir = 3;
                    return;
                }
            }
            if ((dir & 0x80) && eng->unk10.x != 0x100 &&
                (off = ta->Speed + 0x100, Ovl5fMapCell(x - off, z) == 0)) {
                switch (ta->MoveDir) {
                case 1:
                    break;
                case 3:
                    ta->MoveDir = 1;
                    return;
                case 0:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = 0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 1;
                    return;
                case 2:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = -0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 1;
                    return;
                }
            }
            if ((dir & 0x10) && eng->unk10.z != 0xF00 && Ovl5fMapCell(x, z + 0x100) == 0) {
                switch (ta->MoveDir) {
                case 2:
                    break;
                case 1:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = 0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 2;
                    return;
                case 0:
                    ta->MoveDir = 2;
                    return;
                case 3:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = -0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 2;
                    return;
                }
            }
            if ((dir & 0x40) && eng->unk10.z != 0x100 &&
                (off = ta->Speed + 0x100, Ovl5fMapCell(x, z - off) == 0)) {
                switch (ta->MoveDir) {
                case 1:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = -0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 0;
                    return;
                case 3:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = 0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 0;
                    return;
                case 2:
                    ta->MoveDir = 0;
                    return;
                case 0:
                    break;
                }
            }
        } else {
            switch (ta->MoveDir) {
            case 0:
                if (dir & 0x10) {
                    ta->MoveDir = 2;
                }
                break;
            case 2:
                if (dir & 0x40) {
                    ta->MoveDir = 0;
                }
                break;
            case 1:
                if (dir & 0x20) {
                    ta->MoveDir = 3;
                }
                break;
            case 3:
                if (dir & 0x80) {
                    ta->MoveDir = 1;
                }
                break;
            }
        }
        switch (ta->MoveDir) {
        case 2:
            hit = TankMoveFindHero(player, eng->unk10.x, eng->unk10.z + speed, 0x1C0 - speed);
            if (hit < 0) {
                if (Ovl5fMapCell(x, z + 0x100) == 0) {
                    pos = eng->unk10.z + ta->Speed;
                    frac = pos & 0xFF;
                    eng->unk10.z = pos;
                    if (frac > 0x80) {
                        frac -= 0x100;
                    }
                    if (frac >= -half && frac <= half) {
                        eng->unk10.z = pos - frac;
                    }
                }
            } else if (D_800CB0E0 == 0) {
                D_800CB0E8 = eng;
                D_800CB0E0 = -1;
            }
            break;
        case 0:
            hit = TankMoveFindHero(player, eng->unk10.x, eng->unk10.z - speed, 0x1C0);
            if (hit < 0) {
                off = half + 0x100;
                if (Ovl5fMapCell(x, z - off) == 0) {
                    pos = eng->unk10.z - ta->Speed;
                    frac = pos & 0xFF;
                    eng->unk10.z = pos;
                    if (frac > 0x80) {
                        frac -= 0x100;
                    }
                    if (frac >= -half && frac <= half) {
                        eng->unk10.z = pos - frac;
                    }
                }
            } else if (D_800CB0E0 == 0) {
                D_800CB0E8 = eng;
                D_800CB0E0 = -1;
            }
            break;
        case 3:
            hit = TankMoveFindHero(player, eng->unk10.x + speed, eng->unk10.z, 0x1C0 - speed);
            if (hit < 0) {
                if (Ovl5fMapCell(x + 0x100, z) == 0) {
                    pos = eng->unk10.x + ta->Speed;
                    frac = pos & 0xFF;
                    eng->unk10.x = pos;
                    if (frac > 0x80) {
                        frac -= 0x100;
                    }
                    if (frac >= -half && frac <= half) {
                        eng->unk10.x = pos - frac;
                    }
                }
            } else if (D_800CB0E0 == 0) {
                D_800CB0E8 = eng;
                D_800CB0E0 = -1;
            }
            break;
        case 1:
            hit = TankMoveFindHero(player, eng->unk10.x - speed, eng->unk10.z, 0x1C0);
            if (hit < 0) {
                off = half + 0x100;
                if (Ovl5fMapCell(x - off, z) == 0) {
                    pos = eng->unk10.x - ta->Speed;
                    frac = pos & 0xFF;
                    eng->unk10.x = pos;
                    if (frac > 0x80) {
                        frac -= 0x100;
                    }
                    if (frac >= -half && frac <= half) {
                        eng->unk10.x = pos - frac;
                    }
                }
            } else if (D_800CB0E0 == 0) {
                D_800CB0E8 = eng;
                D_800CB0E0 = -1;
            }
            break;
        }
    }
}

void SetTankHeroRecoil(u32 player) {
    s32 done;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
        D_800CB0F0 = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        TA_players[player].SpeedY = -0x64;
        SetHeroSeq(player, 0x57);
        Heroes[player].hero_immunity = 0x3C;
        PlaySfx(D_800C9428[Heroes[player].hero_name - 1][Random(4)], NULL, &Heroes[player].hero_eng->unk10, 0x1000,
                0x1E00, 0);
    }
}

void TankHeroRecoil(u32 arg0) {
    EXTengine *eng;
    HeroInfo *h;
    TA_player *p;

    eng = Heroes[arg0].hero_eng;
    h = &Heroes[arg0];
    TA_players[arg0].SpeedY += 0xC;
    p = &TA_players[arg0];
    eng->unk10.y += p->SpeedY;
    if (eng->unk10.y > 0) {
        p->SpeedY = -p->SpeedY / 2;
        eng->unk10.y = 0;
    }
    eng->sub->next->next->unk08 = -eng->unk10.y / 2;
    if (func_8008502C(arg0) != 0) {
        if (h->hero_status & 0x2000) {
            SetHeroAction(arg0, 0x6A);
        } else {
            SetHeroAction(arg0, h->hero_last_mode);
            SetHeroSeq(arg0, 0x59);
        }
    }
}

void SetTankHeroVictory(u32 arg0) {
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
    SetHeroSeq(arg0, 0x5A);
    func_80089FA4(arg0);
    func_80085718(arg0, 0);
    Heroes[arg0].hero_eng->unk10.y = 0;
    sub = Heroes[arg0].hero_eng->sub;
    sub = sub->next;
    sub = sub->next;
    sub->unk08 = 0;
}

void TankHeroVictory(u32 arg0) {
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

void TankHeroDie(u32 arg0) {
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
        SetHeroAction(arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 0 && func_8008502C(arg0) != 0) {
        eng = Heroes[arg0].hero_eng;
        pos = (s32 *)&eng->unk10;
        p = D_8005365C;
        nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
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
        SetHeroAction(arg0, 0x6B);
    }
}

void SetTankHeroDead(u32 arg0) {
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

s32 func_800BAFFC(NAS *nas, s32 arg1, s32 arg2, s32 arg3) {
    EXTengine *eng;
    Ovl5fShotFux *fux;
    s32 owner;
    s32 ret;
    s32 x;
    s32 z;
    s32 tile;
    s32 row;
    s32 col;
    s32 dx;
    s32 dz;
    s32 v;
    s32 a;
    s32 hit;
    s32 sfx;

retry:
    D_800CB0FC = 0;
    eng = nas->Engine;
    fux = (Ovl5fShotFux *)nas->FUX;
    owner = fux->player;
    ret = 0;
    if (D_8005A648 == 8 &&
        *(((eng->unk10.z >> 9) * D_800CAFF4) + (eng->unk10.x >> 9) + D_800CACFC) != 0 &&
        fux->unk30 != 1) {
        x = eng->unk10.x;
        z = eng->unk10.z;
        goto dead;
    }
    x = eng->unk10.x + fux->vx;
    z = eng->unk10.z + fux->vz;
    if (x < 0 || D_800CB100 + 0x100 < x) {
        if (fux->unk30 == 1) {
        back:
            eng->unk10.x -= fux->vx;
            eng->unk10.z -= fux->vz;
            goto retry;
        }
        x += fux->vx = -fux->vx;
        eng->unk5E = 1;
        D_800CB0FC = x;
        eng->unk1E = 0x1000 - eng->unk1E;
    } else
    if (z < 0 || D_800CB104 + 0x100 < z) {
        if (fux->unk30 == 1) {
            goto back;
        }
        z += fux->vz = -fux->vz;
        eng->unk5E = 1;
        D_800CB0FC = z;
        eng->unk1E = 0x800 - eng->unk1E;
    } else if (eng->unk10.y >= -400) {
        v = ((z >> 9) * D_800CAFF4) + (x >> 9);
        tile = *(v + D_800CACFC);
        if (tile != 0) {
            D_800CB0FC = tile;
            if (fux->unk30 == 1) {
                goto back;
            }
            dx = eng->unk10.x - ((tile & 0xF0) << 4);
            if (dx < 0) {
                dx = -dx;
            }
            dz = eng->unk10.z - ((tile & 0xF) << 8);
            if (dz < 0) {
                dz = -dz;
            }
            if (dz < dx) {
                a = fux->vx;
                x += fux->vx = -a;
                if (D_800CACFC[((z >> 9) * D_800CAFF4) + (x >> 9)] != 0) {
                    x += a;
                    fux->vx = a;
                    z += fux->vz = -fux->vz;
                    eng->unk1E = 0x800 - eng->unk1E;
                } else {
                    eng->unk1E = 0x1000 - eng->unk1E;
                }
            } else {
                a = (sfx = fux->vz);
                z += fux->vz = -a;
                if (D_800CACFC[((z >> 9) * D_800CAFF4) + (x >> 9)] != 0) {
                    z += a;
                    fux->vz = a;
                    x += fux->vx = -fux->vx;
                    eng->unk1E = 0x1000 - eng->unk1E;
                } else {
                    eng->unk1E = 0x800 - eng->unk1E;
                }
            }
            eng->unk5E = 1;
        }
    }
    eng->unk10.x = x;
    eng->unk10.z = z;
    MissileCollideWithPickup(eng);
    hit = Ovl5fRocketFindHero(x, z, 0x10E);
    if (hit >= 0) {
        switch ((u32)Random(4)) {
        case 0:
            sfx = 0x4BA;
            break;
        case 1:
            sfx = 0x4BB;
            break;
        case 2:
            sfx = 0x4BC;
            break;
        case 3:
            sfx = 0x4BD;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:
        if (((Ovl5fModeView *)D_8005A614)->mode == 5 && D_8005A614[hit + 0x5C] != 0 &&
            TA_players[hit].ForceFieldDelay == 0 && D_8009D908 == 0 &&
            (u32)Random(0x20) < D_8009D87C[hit].unk0) {
            D_800CB284[hit] = 1;
            goto hit2;
        }
        if (owner == hit) {
            DamageTankHero(hit, arg1 / 2);
        } else {
            DamageTankHero(hit, arg1);
        }
        goto hit2;
    }
    if (Ovl5fRocketNear(eng->unk10.x, eng->unk10.z, 0xE0) == NULL) {
        if (--fux->life != 0) {
            return ret;
        }
    dead:
        ret = 1;
        goto spawn;
    }
hit2:
    ret = 2;
spawn:
    if (arg3 != 0) {
        Ovl5fSpawnBit1605(&eng->unk10, arg3);
    }
    if (arg2 != 0) {
        CreateNAS(x, eng->unk10.y, z, arg2, 0x11);
    }
    return ret;
}

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
            if (TA_players[fux->player].MainWeaponNas[i] == nas) {
                TA_players[fux->player].MainWeaponNas[i] = NULL;
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
            if (TA_players[fux->player].MainWeaponNas[i] == nas) {
                TA_players[fux->player].MainWeaponNas[i] = NULL;
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
            if (TA_players[fux->player].MainWeaponNas[i] == nas) {
                TA_players[fux->player].MainWeaponNas[i] = NULL;
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
            if (TA_players[fux->player].MainWeaponNas[i] == nas) {
                TA_players[fux->player].MainWeaponNas[i] = NULL;
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
        puff = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
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
      DamageTankHero(i, 0x3C);
    }


    goto boom;
  }


  if (((fux->vy > 0) && (eng->unk10.y >= (-0x103))) || ((D_8005A648 == 9) && (eng->unk10.z > 0x1000)))
  {
    eng->unk10.y = -0x104;
    if ((D_8005A648 == 9) && (fux->player != (-1)))
    {
      CheckMineToBoss(eng->unk10.x, eng->unk10.y, eng->unk10.z);
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

    DamageTankHero(hit, 0x3C);
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
                    puff = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
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
    MissileCollideWithPickup(eng);
    hit = Ovl5fRocketFindHero(x, z, 0x10E);
    if (hit < 0 || hit == fux->player) {
        goto puff;
    }
    if (((Ovl5fModeView *) D_8005A614)->mode != 9 || fux->player == -1) {
        dmg = 0x3C;
    } else {
        dmg = (((D_8005A614[0xB4] * 190) / 32 + 10) * 60) / 100;
    }
    DamageTankHero(hit, dmg);
boom:
    CreateNAS(x, eng->unk10.y, z, 0x1306, 0x11);
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
        CreateMineShrapnelBits(nas);
        for (i = 0; i < 8; i++) {
            if (TankMines[i] == nas) {
                TankMines[i] = NULL;
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
                DamageTankHero(i, 0x4B0);
            } else {
                DamageTankHero(i, 0x3C);
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
            if (TankMines[j] == nas) {
                TankMines[j] = NULL;
                break;
            }
        }
        if (D_8005A614[hit + 0x5C] == 0) {
            if (((Ovl5fGameView *)D_8005A614)->flags & 0x80) {
                DamageTankHero(hit, 0x4B0);
            } else {
                DamageTankHero(hit, 0x3C);
            }
        } else {
            DamageTankHero(hit, 0x3C);
        }
        ta = &TA_players[hit];
        ta->SpeedX = 0;
        ta->SpeedZ = 0;
        CreateMineShrapnelBits(nas);
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
        TA_players[player].Energy = 1;
        h2->hero_immunity = 0;
        DamageTankHero(player, 0x4B0);
    }
    pos = &eng->unk10;
    PlaySfx(0x4EA, NULL, pos, 0x1000, 0x1E00, 0);
    p = D_8005365C;
    n = CreateNAS(eng->unk10.x, pos->y, pos->z, 0x1605, 0x11);
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

void CreateMineShrapnelBits(NAS *nas) {
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
            m = CreateNAS(eng->unk10.x + dx, eng->unk10.y, eng->unk10.z + dz, 0x1304, 0x11);
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
        n = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
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
            CheckMineToBoss(x, eng->unk10.y, z);
        }
        for (i = 0; i < 4; i++) {
            if (!(Heroes[i].hero_status & 0xFA00)) {
                he = Heroes[i].hero_eng;
                if ((u32)(x - he->unk10.x + 0x300) <= 0x600 && (u32)(z - he->unk10.z + 0x2FF) < 0x5FF) {
                    DamageTankHero(i, 0x3C);
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
    CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1305, 0x11);
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
        DamageTankHero(hit, 0x4B0);
    } else {
        DamageTankHero(hit, 0x3C);
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
    CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1305, 0x11);
    KillNAS(nas);
}

void PlotPickupGenerator_TA(NAS *nas)
{
  s32 i;
  s32 j;
  s32 x;
  s32 z;
  s32 r;
  s32 n;
  s32 new_var;
  NAS *p;
  EXTengine *eng;
  if (D_800CB0F0 != 0)
  {
    KillNAS(nas);
    return;
  }
  if (nas->UserInfo.flags == 0)
  {
    nas->UserInfo.flags = Random(0x78) + 0xB4;
    switch (D_8005A648)
    {
      case 7:

      default:
        CreateNAS(0x600, -0x500, 0x600, 0x1307, 0x11);
        return;

      case 8:
        for (i = 0;; i++)
      {
        s32 r;
        if (i >= 3)
        {
          break;
        }
        if (PU_TA_list[i] != 0)
        {
          continue;
        }

        n = Random(0x29);
        x = D_800CA4B8[n * 2];
        new_var = x;
        z = D_800CA4B8[(n * 2) | 1];
        if (PickupFindHero(new_var, z) >= 0)
        {
          return;
        }
        for (j = 0; j < 3; j++)
        {
          p = PU_TA_list[j];
          if (p != 0)
          {
            if ((p->Engine->unk10.x != new_var) || (p->Engine->unk10.z != z))
            {
              break;
            }
            return;
          }
        }


        p = CreateNAS(new_var, 0, z, 0x1308, 0x11);
        if (p == 0)
        {
          continue;
        }
        eng = p->Engine;
        PU_TA_list[i] = p;
        r = Random(3);
        switch (r)
        {
          case 0:
            p->Engine->sub->unk6C = D_80053694;
            p->Engine->sub->unk74 = 0x2013;
            r = 5;
            break;

          case 1:
            eng->sub->unk6C = D_80052F5C;
            eng->sub->unk74 = 0x2018;
            r = 4;
            break;

          case 2:
            p->Engine->sub->unk74 = 0x200C;
            r = 3;
            break;

        }

        p->UserInfo.status = r;
      }

        break;

      case 9:
        for (i = 0;; i++)
      {
        s32 k;
        if (i >= 3)
        {
          break;
        }
        if (PU_TA_list[i] != 0)
        {
          continue;
        }
        x = nas->Engine->unk10.x;
        z = nas->Engine->unk10.z;
        if (PickupFindHero(x, z) >= 0)
        {
          return;
        }
        for (k = 0; k < 3; k++)
        {
          p = PU_TA_list[k];
          if (((p != 0) && (p->Engine->unk10.x == x)) && (p->Engine->unk10.z == z))
          {
            return;
          }
        }

        p = CreateNAS(x, 0, z, 0x1308, 0x11);
        if (p == 0)
        {
          return;
        }
        PU_TA_list[i] = p;
        nas->UserInfo.flags = ((D_8009D87C[3].unk0 * 2) - D_8005A614[0xB4]) * 30;
        if (((u16) (nas->UserInfo.flags & 0xFFFE)) < 60)
        {
          nas->UserInfo.flags = 60;
        }
        if (nas->UserInfo.flags > 1200)
        {
          nas->UserInfo.flags = 1200;
        }
        if (D_8005A614[0xB4] > 10)
        {
          nas->UserInfo.flags >>= 1;
        }
        if (((u32) Random(0x20)) > 8)
        {
          eng = p->Engine;
          p->UserInfo.status = 4;
          eng->sub->unk6C = D_80052F5C;
          eng->sub->unk74 = 0x2018;
          eng->sub->flags |= 0x20000000;
          eng = eng->Engine;
          eng->sub->unk6C = D_80052F5C;
          eng->sub->unk74 = 0x2001;
          return;
        }
        p->UserInfo.status = 5;
        p->Engine->sub->unk6C = D_80053694;
        p->Engine->sub->unk74 = 0x2013;
        return;
      }

        break;

    }

  }
  else
  {
    nas->UserInfo.flags--;
  }
}

void InitHelicopterTA(NAS *nas) {
    HelicopterTAExt *ext;
    EXTengine *eng;
    EXTengine *e;
    subengine *sub;
    u8 *p;

    ext = AddExt(nas, 0x38, 0x10000000);
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
        e = AddExtEngine(nas, 1);
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
    for (i = 0, s = PU_TA_list; i < 3; i++, s++) {
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
        if (Heroes[i].hero_name != 9 && TA_players[i].Energy <= 0) {
            chance += 2000;
        }
    }
    n = 0;
    g = (Ovl5fModeView *)D_8005A614;
    slot = PU_TA_list;
loop:
    if (n < 3) {
        if (*slot == NULL) {
            p = CreateNAS(eng->unk10.x, eng->unk10.y - 0xC8, eng->unk10.z + 0x3C, 0x1308, 0x11);
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
    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        ext->unk14 = 0;
        nas->FUX = ext;
        p = D_8005365C;
        eng = nas->Engine;
        sub = eng->sub;
        sub->unk6C = p;
        sub->unk74 = 0x202B;
        eng->unk5D = 1;
        e = AddExtEngine(nas, 1);
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

void PlotPickupTA(NAS *nas) {
    EXTengine *eng;
    FlubberFux *fux;
    NAS *n;
    EXTengine *e;
    EXTengine *fe;
    subengine *sub;
    s32 dx;
    s32 fx;
    s32 fz;
    s32 d;
    s32 j;
    s32 i;
    s32 hit;
    s32 k;
    s32 lim;
    s32 v;
    s32 x;
    s32 sc;
    s32 anim;
    s32 sn;
    u16 st;

    eng = nas->Engine;
    fux = (FlubberFux *)nas->FUX;
    dx = 0;
    if (Ovl5fMapCell(d = eng->unk10.x, eng->unk10.z) != 0) {
        goto puff;
    }
    switch (nas->UserInfo.user1) {
    case 0:
    default:
        if (nas->UserInfo.status == 4) {
            eng->sub->unk74++;
            if (eng->sub->unk74 >= 0x2026) {
                eng->sub->unk74 = 0x2018;
            }
        }
        if (nas->UserInfo.status == 2) {
            eng->unk5E = 1;
            eng->unk1E += 0x28;
        }
        fux->vy += 8;
        eng->unk10.y += fux->vy;
        if (eng->unk10.y >= 0) {
            eng->unk10.y = -1;
            fux->vy = -0x28;
            eng->unk10.y -= 0x28;
        }
        break;
    case 5:
        eng->unk10.y += 0x14;
        nas->UserInfo.user2 = (nas->UserInfo.user2 + 0x78) & 0xFFF;
        sn = D_80068BD4[(nas->UserInfo.user2 - 0x400) & 0xFFF].unk2;
        eng->unk5E = 1;
        eng->unk20 = (sn * 25) >> 9;
        dx = -(D_80068BD4[(nas->UserInfo.user2 - 0x400) & 0xFFF].unk2 * 120) >> 12;
        if (eng->unk10.y < -0x12B) {
            break;
        }
        fx = eng->unk10.x;
        fz = eng->unk10.z;
        lim = 0x100;
        for (j = 0; j < 8; j++) {
            n = TankMines[j];
            if (n == NULL) {
                continue;
            }
            e = n->Engine;
            d = fx - e->unk10.x;
            if (d < -0x100) {
                continue;
            }
            if (lim < d) {
                continue;
            }
            d = fz - e->unk10.z;
            if (d < -0x100) {
                continue;
            }
            if (lim < d) {
                continue;
            }
            n->UserInfo.status = 1;
        }
        if ((u32)((u16)D_8005A648 - 8) >= 2) {
            Ovl5fSpawnBit1605(&eng->unk10, 3);
            switch ((u32)Random(4)) {
            case 0:
                PlaySfx(0x4B4, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 1:
                PlaySfx(0x4B5, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x4B6, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x4B7, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            }
        }
        eng->unk10.y = 0;
        fux->vy = -0x28;
        nas->UserInfo.user1 = 0;
        eng->unk20 = 0;
        switch (nas->UserInfo.status) {
        case 2:
            eng->sub->unk6C = D_80053694;
            eng->sub->unk74 = 0x2007;
            eng->Engine->sub->unk6C = D_80053694;
            eng->Engine->sub->unk74 = 0x200F;
            eng->Engine->unk1E = eng->unk1E + 0x400;
            eng->Engine->sub->unk20.x = 0x20;
            eng->Engine->sub->unk20.z = 0x60;
            eng->Engine->unk5E = 1;
            break;
        case 6:
            CreateNAS(eng->unk10.x, -0x14, eng->unk10.z, 0x1303, 0x11);
            for (i = 0; i < 3; i++) {
                if (PU_TA_list[i] == nas) {
                    PU_TA_list[i] = NULL;
                    break;
                }
            }
            KillNAS(nas);
            return;
        case 5:
        default:
            eng->sub->unk6C = D_80053694;
            eng->sub->unk74 = 0x2013;
            eng->Engine->sub->unk6C = D_80053694 - 0x738;
            eng->Engine->sub->unk74 = 0x2001;
            break;
        case 3:
            eng->sub->unk74 = 0x200C;
            eng->Engine->sub->unk6C = D_80052F5C;
            eng->Engine->sub->unk74 = 0x2001;
            break;
        case 4:
            eng->sub->unk6C = D_80052F5C;
            eng->sub->unk74 = 0x2018;
            eng->sub->flags |= 0x20000000;
            eng->Engine->sub->unk6C = D_80052F5C;
            eng->Engine->sub->unk74 = 0x2001;
            break;
        }
        break;
    }
    eng->unk5E = 1;
    if (eng->unk10.y < -0x2ED) {
        goto follow;
    }
    hit = Ovl5fRocketFindHero(eng->unk10.x, eng->unk10.z, 0x12C);
    if (hit < 0) {
        goto follow;
    }
    if (D_800CB0F0 != 0) {
    puff:
        Ovl5fSpawnBit1605(&eng->unk10, 3);
        PlaySfx(0x4EA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (k = 0; k < 3; k++) {
            if (PU_TA_list[k] == nas) {
                PU_TA_list[k] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    switch (nas->UserInfo.status) {
    case 6:
        if (((Ovl5fModeView *)D_8005A614)->mode != 8 || D_8005A614[hit + 0x5C] != 0 ||
            !(((Ovl5fModeView *)D_8005A614)->flags & 0x80)) {
            v = 0x1E;
        } else {
            v = 0x4B0;
        }
        DamageTankHero(hit, v);
        CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1306, 0x11);
        goto puff;
    case 5:
        TA_players[hit].SubWeaponRounds++;
        break;
    case 4:
        TA_players[hit].Energy += 0x14;
        if (TA_players[hit].Energy >= 0x64) {
            TA_players[hit].Energy = 0x63;
        }
        Heroes[hit].hero_health = TA_players[hit].Energy;
        break;
    case 0:
    case 1:
    case 2:
    case 3:
        sub = Heroes[hit].hero_sub->next;
        TA_players[hit].TurretFiring = -1;
        st = nas->UserInfo.status;
        if (st != 2) {
            if (st != TA_players[hit].MainWeaponType) {
                TA_players[hit].MainWeaponRounds = 2;
            } else {
                TA_players[hit].MainWeaponRounds += 2;
            }
            anim = 0x2008;
        } else {
            if (TA_players[hit].MainWeaponType != st) {
                TA_players[hit].MainWeaponRounds = st;
            } else {
                TA_players[hit].MainWeaponRounds += 2;
            }
            anim = 0x200B;
        }
        sub->unk74 = anim;
        TA_players[hit].MainWeaponType = nas->UserInfo.status;
        break;
    case 7:
        sub = Heroes[hit].hero_sub->next;
        sub->unk74 = 0x2008;
        if (hit < 2) {
            TA_players[hit].SubWeaponRounds++;
        }
        if (TA_players[hit].MainWeaponType != 3) {
            TA_players[hit].MainWeaponType = 3;
            TA_players[hit].MainWeaponRounds = 1;
        } else {
            TA_players[hit].MainWeaponRounds++;
        }
        break;
    }
    for (i = 0; i < 3; i++) {
        if (PU_TA_list[i] == nas) {
            PU_TA_list[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
    switch (nas->UserInfo.status) {
    case 4:
    default:
        PlaySfx(0x1C2, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        return;
    case 3:
        PlaySfx(0x48E, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        return;
    case 2:
        PlaySfx(0x51F, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        return;
    case 5:
        PlaySfx(0x4E8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        return;
    }
follow:
    fe = eng->Engine;
    if (fe != NULL) {
        fe->unk1E = eng->unk1E;
        fe->unk10 = eng->unk10;
        x = eng->unk10.x;
        fe->unk10.y = 0;
        fe->unk10.x = x + dx;
        sc = ((eng->unk10.y + 0x1000) * 0xAF0) / 0x1000;
        fe->sub->unk20.x = sc;
        fe->sub->unk20.z = sc;
        fe->sub->unk52 = 1;
    }
    if (nas->UserInfo.status == 2) {
        fe->unk1E = eng->unk1E + 0x400;
        fe->sub->unk20.x = 0x3E8;
        fe->sub->unk20.z = 0xAF0;
        eng->Engine->unk5E = 1;
    }
}

static inline void SpawnPuff1605(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;

    p = D_8005365C;
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
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

bool MissileCollideWithPickup(EXTengine *eng) {
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
        pickup = PU_TA_list[i];
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
                if (PU_TA_list[j] == pickup) {
                    PU_TA_list[j] = NULL;
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

void PlotPeriscope(NAS *nas) {
    TotemFux *fux;
    EXTengine *eng;
    EXTengine *e;
    NAS *n;
    subengine *sub;
    s32 i;
    s32 j;
    s32 frame;

    if (D_800CB0F0 != 0) {
        KillNAS(nas);
        return;
    }
    fux = (TotemFux *)nas->FUX;
    eng = nas->Engine;
    switch (fux->state) {
    case 1:
        if (--fux->timer == 30) {
            D_800CB214 = Ovl5fPeriscopePuff(&nas->Engine->unk10, 1);
            PlaySfx(0x4FB, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (fux->timer <= 0) {
            fux->state = 4;
            fux->timer = 4;
        }
        break;
    case 4:
        Ovl5fPeriscopePuff(&nas->Engine->unk10, 0);
        fux->timer--;
        switch (fux->timer) {
        case 3:
            eng->sub->next->flags = 0x8000;
            eng->sub->next->unk74 = 0x2001;
            break;
        case 2:
            eng->sub->next->unk74 = 0x2002;
            break;
        case 1:
            eng->sub->next->unk74 = 0x2003;
            break;
        case 0:
            for (j = 0; j < 8; j++) {
                if (D_800CB264[j] == NULL) {
                    D_800CB264[j] = nas;
                    break;
                }
            }
            eng->sub->next->unk74 = 0x2004;
            fux->state = 0;
            fux->timer = 210;
            break;
        }
        break;
    case 0:
        if (!(D_8004E0D8 & 0x3F)) {
            Ovl5fPeriscopePuff(&nas->Engine->unk10, 0);
        }
        sub = eng->sub;
        eng->unk5E = 1;
        eng->unk1E += 30;
        sub = sub->next;
        sub->unk51 = 0;
        sub->unk52 = 1;
        if (--fux->timer <= 0) {
            fux->state = 5;
            fux->timer = 4;
            break;
        }
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_name != 9 && !(Heroes[i].hero_status & 0xFA00) && D_8005A614[i + 0x5C] == 0) {
                e = Heroes[i].hero_eng;
                if ((u32)(((eng->unk1E - get_angle(eng->unk10.x - e->unk10.x, eng->unk10.z - e->unk10.z)) & 0xFFF) - 0x15) >= 0xFD7U) {
                    fux->state = 10;
                    fux->timer = 0;
                    PlaySfx(0x4FC, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                }
            }
        }
        break;
    case 10:
        if (!(D_8004E0D8 & 0x1F)) {
            Ovl5fPeriscopePuff(&nas->Engine->unk10, 0);
        }
        frame = D_800CA6EC[fux->timer];
        if (++fux->timer == 9) {
            n = CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1319, 0x11);
            if (n != NULL) {
                n->Engine->unk1E = eng->unk1E;
            }
        }
        if (frame == 0) {
            fux->state = 11;
            fux->timer = 4;
            break;
        }
        eng->sub->next->unk74 = frame;
        break;
    case 11:
        Ovl5fPeriscopePuff(&nas->Engine->unk10, 0);
        fux->timer--;
        switch (fux->timer) {
        case 3:
            eng->sub->next->flags = 0x8000;
            eng->sub->next->unk74 = 0x2003;
            break;
        case 2:
            for (j = 0; j < 8; j++) {
                if (D_800CB264[j] == nas) {
                    D_800CB264[j] = NULL;
                    break;
                }
            }
            eng->sub->next->unk74 = 0x2002;
            break;
        case 1:
            eng->sub->next->unk74 = 0x2001;
            break;
        case 0:
            fux->state = 12;
            fux->timer = 1;
            eng->sub->next->flags = 0;
            break;
        }
        break;
    case 12:
        if (--fux->timer <= 0) {
            fux->state = 1;
            fux->timer = 120;
        }
        break;
    case 5:
        Ovl5fPeriscopePuff(&nas->Engine->unk10, 0);
        fux->timer--;
        switch (fux->timer) {
        case 3:
            eng->sub->next->flags = 0x8000;
            eng->sub->next->unk74 = 0x2003;
            break;
        case 2:
            eng->sub->next->unk74 = 0x2002;
            break;
        case 1:
            eng->sub->next->unk74 = 0x2001;
            break;
        case 0:
            fux->state = 1;
            fux->timer = 150;
            eng->sub->next->flags = 0;
            break;
        }
        break;
    }
}

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
        shell = CreateNAS(e->unk10.x, e->unk10.y - 200, e->unk10.z, 0x130B, 0x11);
        if (shell == NULL) {
            return;
        }
        PlaySfx(0x4F3, &shell->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
        ext = AddExt(shell, 0x38, 0x10000000);
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
        n = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
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
            obj = CreateNAS(x, 0, z, 0x1311, 0x11);
            if (obj != NULL) {
                obj->Engine->sub->unk74 = *p++;
                obj->UserInfo.user2 = delay;
                obj = CreateNAS(x, 0, z, 0x1312, 0x11);
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
            MissileCollideWithPickup(eng);
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
            obj = CreateNAS(eng->unk10.x, 0, eng->unk10.z, 0x1312, 0x11);
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

void InitTotemBit(NAS *nas, s32 id) {
    TotemExt *ext;
    EXTengine *eng;
    subengine *sub;
    s32 i;
    NAS **p;

    nas->UserInfo.user1 = 1;
    ext = AddExt(nas, 0x38, 0x10000000);
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

void TotemBitExplode(EXTengine *arg0) {
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
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
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
                DamageTankHero(i, 0x3C);
            }
        }
    }
    arg0->unk10.y = saved;
    PlaySfx(0x505, NULL, NULL, 0x1000, 0x1E00, 0);
}

void PlotTotemBit(NAS *nas, s32 id) {
    EXTengine *eng;
    s32 v;
    NAS *pole;
    TotemPoleExt *pf;
    TotemExt *ext;
    NAS *n;
    EXTengine *ne;
    TotemExt *nx;
    s32 x;
    s32 z;
    s32 k;
    s32 t;
    s32 a;

    eng = nas->Engine;
    ext = (TotemExt *)nas->FUX;
    x = eng->unk10.x;
    z = eng->unk10.z;
    switch ((u32)ext->unk30) {
    case 1:
        pole = D_800CB218;
        pf = (TotemPoleExt *)pole->FUX;
        switch (pf->unk30) {
        case 0xD:
            if (id == 0x131B) {
                ext->unk30 = 0xD;
                ext->anim = D_800C9C14;
            }
            break;
        case 0x24:
            if (eng->unk1E != pole->Engine->unk1E) {
                eng->unk1E = pole->Engine->unk1E;
                eng->unk5E = 1;
            }
            break;
        case 0x11:
            if (id == 0x131B) {
                ext->unk30 = 5;
                ext->unk14 = -0x5A;
                ext->unk10 = -0x1E;
                ext->unk18 = -0x2D;
                PlaySfx(0x504, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
            break;
        case 0:
            if (id == 0x131C) {
                ext->unk30 = 5;
                ext->unk14 = -0x5A;
                ext->unk10 = 0x1E;
                ext->unk18 = -0x2D;
                PlaySfx(0x504, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
            break;
        }
        break;
    case 13:
        v = *ext->anim++;
        if (v != 0) {
            if (v == -1) {
            n = CreateNAS(eng->unk10.x, eng->unk10.y - 100, eng->unk10.z, 0x130E, 0x11);
            if (n != NULL) {
                ne = n->Engine;
                nx = (TotemExt *)n->FUX;
                ne->sub->unk6C = D_800537AC;
                ne->sub->unk74 = 0x2006;
                nx->unk2C = v;
                ne->unk10.x += -(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 300) >> 12;
                ne->unk10.z += -(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 300) >> 12;
                ne->unk1E = eng->unk1E;
                nx->unk10 = -(D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 * 60) >> 12;
                nx->unk18 = -(D_80068BD4[eng->unk1E & 0xFFF].unk2 * 60) >> 12;
            }
            } else {
                eng->sub->unk74 = v;
            }
        } else {
            pole = D_800CB218;
            ext->unk30 = 1;
            eng->sub->unk74 = 0x2005;
            pf = (TotemPoleExt *)pole->FUX;
            pf->unk30 = 0x24;
            PlaySfx(0x502, &pole->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            pole->UserInfo.user2 = 0x1E;
        }
        break;
    case 5:
        ext->unk14 += 12;
        eng->unk10.y += ext->unk14;
        x = eng->unk10.x + ext->unk10;
        z = eng->unk10.z + ext->unk18;
        if (eng->unk10.y < 0) {
            break;
        }
        eng->unk10.y = 0;
        if (ext->unk14 >= 0x25) {
            TotemBitExplode(eng);
        } else {
            PlaySfx(0x506, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (ext->unk14 < 13) {
            ext->unk30 = 0;
            ext->timer = 0x5A;
        } else {
            ext->unk14 = -(ext->unk14 * 2) / 3;
            ext->unk10 = (ext->unk10 * 2) / 3;
            ext->unk18 = (ext->unk18 * 2) / 3;
        }
        break;
    case 0:
        if (--ext->timer > 0) {
            break;
        }
        ext->unk30 = 0x25;
        ext->anim = D_800C9CE4;
        nas->UserInfo.status = 0;
        ext->unk1C = (ext->unk1C + 1) & 0xF;
        ext->unk20 = D_800C9B94[ext->unk1C][0];
        ext->unk24 = *(s32 *)((u8 *)D_800C9B94 + ((ext->unk1C * 8) | 4));
        ext->unk10 = (ext->unk20 - eng->unk10.x) / 31;
        ext->unk18 = (ext->unk24 - eng->unk10.z) / 31;
        a = get_angle(ext->unk10, ext->unk18) + 0x800;
        a -= eng->unk1E;
        a = (a << 20) >> 20;
        a /= 12;
        ext->unkC = a;
        PlaySfx(0x507, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 0x25:
        v = *ext->anim++;
        if (v != 0) {
            eng->sub->unk74 = v;
            k = nas->UserInfo.status++;
            k -= 16;
            if (k > 0) {
                eng->unk1E += ext->unkC;
                eng->unk5E = 1;
                x = eng->unk10.x + ext->unk10 * k / 12;
                z = eng->unk10.z + ext->unk18 * k / 12;
            }
        } else {
            ext->anim = D_800C9C94;
            ext->unk30 = 0x29;
        }
        break;
    case 0x29:
        v = *ext->anim++;
        if (v == 0) {
            ext->unk30 = 0x26;
            ext->anim = D_800C9DD0;
            nas->UserInfo.status = 0;
            a = -eng->unk1E;
            a = (a << 20) >> 20;
            a /= 12;
            ext->unkC = a;
            break;
        }
        eng->sub->unk74 = v;
        x = eng->unk10.x + ext->unk10;
        z = eng->unk10.z + ext->unk18;
        break;
    case 0x26:
        v = *ext->anim++;
        if (v != 0) {
            eng->sub->unk74 = v;
            t = nas->UserInfo.status++;
            if (t < 12) {
                t = 12 - t;
                x = eng->unk10.x + ext->unk10 * t / 12;
                z = eng->unk10.z + ext->unk18 * t / 12;
                eng->unk1E += ext->unkC;
                eng->unk5E = 1;
            }
        } else if (D_800CB0F0 != 0) {
            ext->unk30 = 0;
            eng->sub->unk74 = 0x2004;
            ext->timer = 0xB9A;
        } else {
            ext->anim = D_800C9D58;
            ext->unk30 = 0x27;
            PlaySfx(0x504, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x27:
    next:
        v = *ext->anim++;
        if (v != 0) {
            eng->sub->unk74 = v;
            if (v == -1) {
                TotemBitExplode(eng);
                goto next;
            }
        } else {
            ext->unk30 = 0;
            eng->sub->unk74 = 0x2004;
            ext->timer = 0x96;
        }
        break;
    }
    if (Ovl5fRocketFindHero(x, z, 0x180) < 0) {
        eng->unk10.x = x;
        eng->unk10.z = z;
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
    MissileCollideWithPickup(eng);
}

void CheckMineToBoss(s32 x, s32 y, s32 z) {
    s32 i;

    switch (D_800CB210) {
    case 0x2F:
        for (i = 0;; i++) {
            NAS *nas;
            EXTengine *eng;

            if (i >= 3) {
                break;
            }
            nas = TankBossCrashTurrets[i];
            if (nas == NULL || nas->UserInfo.flags == 2) {
                continue;
            }
            eng = nas->Engine;
            if (Ovl5fWithin(eng, x, y, z, 0x250, 0x200, 0x500)) {
                if (++nas->UserInfo.flags == 1) {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->unk74 = 0x2006;
                    Ovl5fSpawnBit1605B(&eng->unk10, 0);
                } else {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->unk74 = 0x2008;
                    Ovl5fSpawnBit1605B(&eng->unk10, 0);
                }
            }
        }
        break;
    case 0x33:
        for (i = 0;; i++) {
            NAS *nas;
            EXTengine *eng;

            if (i >= 4) {
                break;
            }
            nas = TankBossCortexTurrets[i];
            if (nas == NULL || nas->UserInfo.flags == 2) {
                continue;
            }
            eng = nas->Engine;
            if (Ovl5fWithin(eng, x, y, z, 0x250, 0x200, 0x500)) {
                if (++nas->UserInfo.flags == 1) {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->unk74 = 0x2007;
                    Ovl5fSpawnBit1605B(&eng->unk10, 1);
                } else {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->unk74 = 0x2005;
                    Ovl5fSpawnBit1605B(&eng->unk10, 1);
                }
            }
        }
        break;
    case 0x37:
        for (i = 0;; i++) {
            NAS *nas;
            EXTengine *eng;

            if (i >= 2) {
                break;
            }
            nas = TankBossRocketTurrets[i];
            if (nas == NULL || nas->UserInfo.flags == 2) {
                continue;
            }
            eng = nas->Engine;
            if (Ovl5fWithin(eng, x, y, z, 0x250, 0x200, 0x500)) {
                if (++nas->UserInfo.flags == 1) {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->unk74 = 0x2025;
                    Ovl5fSpawnBit1605B(&eng->unk10, 2);
                } else {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->unk74 = 0x2026;
                    Ovl5fSpawnBit1605B(&eng->unk10, 2);
                }
            }
        }
        break;
    }
}

void SpawnTankBossTurrets(void) {
    TankBossCrashTurrets[0] = CreateNAS(0x369, 0x7D0, 0x10E0, 0x1315, 0x11);
    TankBossCrashTurrets[1] = CreateNAS(0x900, 0x7D0, 0x10E0, 0x1315, 0x11);
    TankBossCrashTurrets[2] = CreateNAS(0xE97, 0x7D0, 0x10E0, 0x1315, 0x11);
    TankBossCortexTurrets[0] = CreateNAS(0x300, 0x7D0, 0x10E0, 0x1316, 0x11);
    TankBossCortexTurrets[1] = CreateNAS(0x700, 0x7D0, 0x10E0, 0x1316, 0x11);
    TankBossCortexTurrets[2] = CreateNAS(0xB00, 0x7D0, 0x10E0, 0x1316, 0x11);
    TankBossCortexTurrets[3] = CreateNAS(0xF00, 0x7D0, 0x10E0, 0x1316, 0x11);
    TankBossRocketTurrets[0] = CreateNAS(0x300, 0x7D0, 0x1200, 0x1317, 0x11);
    TankBossRocketTurrets[1] = CreateNAS(0xF00, 0x7D0, 0x1200, 0x1317, 0x11);
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
    ext = AddExt(nas, 0x38, 0x10000000);
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
        SpawnTankBossTurrets();
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
        SpawnTankBossTurrets();
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

void PlotTankBoss(NAS *nas) {
    JoeExt *fux;
    EXTengine *eng;

    if (D_800CB0F0 != 0) {
        return;
    }
    fux = (JoeExt *)nas->FUX;
    eng = nas->Engine;
    switch (fux->unk30) {
    case 1:
        if (--fux->unk28 <= 0) {
            fux->unk30 = 0x2C;
            fux->unk28 = 0x96;
        }
        break;
    case 0x2C:
        if (--fux->unk28 <= 0) {
            fux->unk30 = 0x2D;
            PlaySfx(0x258, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x2D:
        if ((eng->unk10.y -= 0x1E) <= 0) {
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x259, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            eng->unk10.y = 0;
            fux->unk30 = 0x2E;
            fux->unk28 = 0x1E;
            PlaySfx(0x25B, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x25D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x2E:
        if (--fux->unk28 <= 0) {
            fux->unk30 = 0x2F;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
        }
        break;
    case 0x2F:
        if (TankBossCrashTurrets[0]->UserInfo.flags == 2 &&
            TankBossCrashTurrets[1]->UserInfo.flags == TankBossCrashTurrets[0]->UserInfo.flags &&
            TankBossCrashTurrets[2]->UserInfo.flags == TankBossCrashTurrets[1]->UserInfo.flags) {
            fux->unk30 = 0x30;
            fux->unk28 = 0x2D;
            PlaySfx(0x25A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x30:
        eng->unk10.y += D_800CA954[fux->unk28 & 0xF];
        if (--fux->unk28 <= 0) {
            fux->unk30 = 0x31;
        }
        break;
    case 0x31:
        if ((eng->unk10.y += 10) >= 0x2B6) {
            KillNAS(TankBossCrashTurrets[0]);
            KillNAS(TankBossCrashTurrets[1]);
            KillNAS(TankBossCrashTurrets[2]);
            eng->unk10.y = 0x2B6;
            fux->unk30 = 0x32;
            fux->unk28 = 0x33;
            PlaySfx(0x25C, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x25D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x32:
        if (--fux->unk28 <= 0) {
            fux->unk30 = 0x33;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            D_800CB0E4 = 0x25D;
        }
        break;
    case 0x33:
        if (TankBossCortexTurrets[0]->UserInfo.flags == 2 &&
            TankBossCortexTurrets[1]->UserInfo.flags == TankBossCortexTurrets[0]->UserInfo.flags &&
            TankBossCortexTurrets[2]->UserInfo.flags == TankBossCortexTurrets[1]->UserInfo.flags &&
            TankBossCortexTurrets[3]->UserInfo.flags == TankBossCortexTurrets[2]->UserInfo.flags) {
            fux->unk30 = 0x34;
            fux->unk28 = 0x2D;
            PlaySfx(0x25A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x34:
        eng->unk10.y += D_800CA954[fux->unk28 & 0xF];
        if (--fux->unk28 <= 0) {
            fux->unk30 = 0x35;
        }
        break;
    case 0x35:
        if ((eng->unk10.y += 10) >= 0x4BF) {
            KillNAS(TankBossCortexTurrets[0]);
            KillNAS(TankBossCortexTurrets[1]);
            KillNAS(TankBossCortexTurrets[2]);
            KillNAS(TankBossCortexTurrets[3]);
            eng->unk10.y = 0x4BF;
            fux->unk30 = 0x36;
            fux->unk28 = 0x3C;
            PlaySfx(0x25C, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x25D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x36:
        if (--fux->unk28 <= 0) {
            fux->unk30 = 0x37;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            D_800CB0E4 = 0x25D;
        }
        break;
    case 0x37:
        if (TankBossRocketTurrets[0]->UserInfo.flags == 2 &&
            TankBossRocketTurrets[1]->UserInfo.flags == TankBossRocketTurrets[0]->UserInfo.flags) {
            fux->unk30 = 0x38;
            fux->unk28 = 0x5A;
            PlaySfx(0x25A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x38:
        eng->unk10.y += D_800CA954[fux->unk28 & 0xF];
        if (--fux->unk28 <= 0) {
            fux->unk30 = 0x39;
            fux->unk28 = 0x5A;
        }
        break;
    case 0x39:
        if ((eng->unk10.y += 10) >= 0x7D0) {
            KillNAS(TankBossRocketTurrets[0]);
            KillNAS(TankBossRocketTurrets[1]);
            D_800CB25C = 0x3A;
            D_8005A6C8 = 0;
            fux->unk28 = 0x5A;
            fux->unk30 = 0x3A;
            PlaySfx(0x25D, NULL, NULL, 0x1000, 0x1E00, 0);
            func_80016F54(D_800536CC);
            func_8001612C(D_800536CC + 0x70, 0);
        }
        break;
    case 0x3A:
        if (--fux->unk28 == 0) {
            eng->flags &= ~0x8000;
            fux->unk30 = 0;
            ResetToBasicWeapon(0);
            if (D_8005A680[0] != 0) {
                ResetToBasicWeapon(1);
            }
        }
        break;
    case 0:
        if (D_800CB254 == 0x26 && D_800CB258 == D_800CB254) {
            D_8009D6DC[2] |= 0x8000;
        }
        break;
    }
    D_800CB210 = fux->unk30;
}

s32 TankBossTurretFindTarget(EXTengine *eng) {
    s32 x;
    s32 z;
    s32 ang;

    if (D_800CB0F0 != 0) {
        return -1;
    }
    x = eng->unk10.x;
    z = eng->unk10.z;
    ang = eng->unk1E;
    if ((u32)Random(0x20) < (u32)D_8009D88E) {
        return -1;
    }
    if (Heroes[0].hero_health <= 0 || (Heroes[0].hero_status & 0xFA00)) {
        D_800CA9E4[0].z = -1;
    } else {
        D_800CA9E4[0].x = Heroes[0].hero_eng->unk10.x;
        D_800CA9E4[0].z = Heroes[0].hero_eng->unk10.z;
    }
    if (D_8005A680[0] == 0 || Heroes[1].hero_health <= 0 || (Heroes[1].hero_status & 0xFA00)) {
        D_800CA9E4[1].z = -1;
    } else {
        D_800CA9E4[1].x = Heroes[1].hero_eng->unk10.x;
        D_800CA9E4[1].z = Heroes[1].hero_eng->unk10.z;
    }
    if (D_800CB210 == 0x37) {
        if (Ovl5fTurretSees(0, 0, x, z, ang)) {
            return 0;
        }
        if (Ovl5fTurretSees(1, 1, x, z, ang)) {
            return 1;
        }
        return -1;
    }
    if (D_800CA9E4[0].z > 0x600 || D_800CA9E4[1].z > 0x600) {
        if (D_800CA9E4[0].z > 0x600 && Ovl5fTurretSees(0, 0, x, z, ang)) {
            return 0;
        }
        if (D_800CA9E4[1].z > 0x600 && Ovl5fTurretSees(1, 1, x, z, ang)) {
            return 1;
        }
        return -1;
    }
    switch ((u32)Random(3)) {
    case 0:
        if (D_800CB260 != 2 && Ovl5fTurretSees(3, 3, x, z, ang)) {
            return 3;
        }
        if (D_800CB260 != 3 && Ovl5fTurretSees(2, 2, x, z, ang)) {
            return 2;
        }
        if (D_800CB260 != 4 && Ovl5fTurretSees(4, 4, x, z, ang)) {
            return 4;
        }
        break;
    case 1:
        if (D_800CB260 != 4 && Ovl5fTurretSees(3, 3, x, z, ang)) {
            return 3;
        }
        if (D_800CB260 != 3 && Ovl5fTurretSees(2, 2, x, z, ang)) {
            return 2;
        }
        if (D_800CB260 != 2 && Ovl5fTurretSees(4, 4, x, z, ang)) {
            return 4;
        }
        break;
    case 2:
        if (D_800CB260 != 3 && Ovl5fTurretSees(3, 3, x, z, ang)) {
            return 3;
        }
        if (D_800CB260 != 2 && Ovl5fTurretSees(2, 2, x, z, ang)) {
            return 2;
        }
        if (D_800CB260 != 4 && Ovl5fTurretSees(4, 4, x, z, ang)) {
            return 4;
        }
        break;
    }
    return -1;
}

void TankBossCrashTurretFire(EXTengine *arg0) {
    NAS *shell;
    PeriscopeShellExt *ext;
    EXTengine *se;
    NAS *n;
    subengine *sub;
    s32 dz;
    s32 ang;

    shell = CreateNAS(arg0->unk10.x, arg0->unk10.y - 0xF0, arg0->unk10.z, 0x130E, 0x11);
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
                ((Ovl5fTurretFireFn)TankBossCrashTurretFire)(eng, 10, -1);
                nas->UserInfo.status = 13;
                break;
            }
            eng->unk1E += 20;
        } else {
            if (a >= -19) {
                ((Ovl5fTurretFireFn)TankBossCrashTurretFire)(eng, 10, -1);
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
        r = TankBossTurretFindTarget(eng);
        if (r >= 0) {
            nas->UserInfo.user1++;
            if (nas->UserInfo.user1 & 1) {
                ((Ovl5fTurretFireFn)TankBossCrashTurretFire)(eng, 10, r);
            } else {
                ((Ovl5fTurretFireFn)TankBossCrashTurretFire)(eng, 3, r);
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
        r = TankBossTurretFindTarget(eng);
        if (r >= 0) {
            nas->UserInfo.user1++;
            if (nas->UserInfo.user1 & 1) {
                ((Ovl5fTurretFireFn)TankBossCrashTurretFire)(eng, 10, r);
            } else {
                ((Ovl5fTurretFireFn)TankBossCrashTurretFire)(eng, 3, r);
            }
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 13;
        }
        break;
    }
}

void TankBossCortexTurretFire(EXTengine *arg0) {
    NAS *shell;
    PeriscopeShellExt *ext;
    EXTengine *se;
    NAS *n;
    subengine *sub;
    s32 dz;
    s32 ang;

    shell = CreateNAS(arg0->unk10.x, arg0->unk10.y - 0x113, arg0->unk10.z, 0x130C, 0x11);
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
                TankBossCortexTurretFire(eng);
                nas->UserInfo.status = 13;
                nas->UserInfo.user2 = 0;
                break;
            }
            eng->unk1E += 16;
        } else {
            if (a >= -19) {
                TankBossCortexTurretFire(eng);
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
        if (TankBossTurretFindTarget(eng) >= 0) {
            nas->UserInfo.user1++;
            TankBossCortexTurretFire(eng);
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 13;
        }
        break;
    }
}

void TankBossRocketTurretFire(NAS *nas, EXTengine *eng) {
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
        n = CreateNAS(eng->unk10.x, eng->unk10.y - 0xFA, eng->unk10.z, 0x1302, 0x11);
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
        if (nas->UserInfo.user2 > 30 && TankBossTurretFindTarget(eng) >= 0) {
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
            TankBossRocketTurretFire(nas, eng);
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

void InitMoeJoe(NAS *nas, s32 id) {
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
    ext = AddExt(nas, 0x38, 0x10000000);
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

void LoopMoeJoeMoveAnim(s32 arg0) {
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

void AnimateMoeJoeTurret(s32 player) {
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

    ta = &TA_players[player];
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
                ResetToBasicWeapon(player);
            }
        }
        if (sub->unk74 == 0x4004) {
            e = Heroes[player].hero_eng;
            puff = Ovl5fSpawnBit1605Nas(&e->unk10, 0);
            if (puff != NULL) {
                Ovl5fPuffSetAngle(puff, eng->unk1E);
            }
            if (TA_players[player].MainWeaponType == 3) {
                n = CreateNAS(e->unk10.x, e->unk10.y - 0x12C, e->unk10.z, 0x130B, 0x11);
                if (n != NULL) {
                    PlaySfx(0x4F3, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    fux = AddExt(n, 0x38, 0x10000000);
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
                    TA_players[player].MainWeaponRounds--;
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

void SetMoeJoeIdle(s32 arg0) {
    EXTengine *eng;
    s32 pad[2];

    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    Heroes[arg0].hero_idle_count = Random(0x96) + 0x5A;
    eng = Heroes[arg0].hero_eng;
    if (TA_players[arg0].HullSFX != 0) {
        TA_players[arg0].HullSFX = 0;
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
    }
    if (TA_players[arg0].TurretSFX != 0) {
        TA_players[arg0].TurretSFX = 0;
        StopSfx(TA_players[arg0].TurretSFXHandler);
    }
    if (TA_players[arg0].FireBreatherSFX != 0) {
        TA_players[arg0].FireBreatherSFX = 0;
        StopSfx(TA_players[arg0].FireBreatherSFXHandler);
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

void SetMoeJoeMove(s32 arg0) {
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

void SetMoeJoeRecoil(s32 arg0) {
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
        TA_players[arg0].SpeedY = -0x64;
        Heroes[arg0].hero_immunity = 0x3C;
        do {
            z = 0;
        } while (0);
        if (z) {
            goto again;
        }
    }
}

void SetMoeJoeDead(s32 hero, s32 last_mode) {
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

void MoeJoeMove(s32 player) {
    EXTengine *eng;
    EXTengine *e;
    subengine *sub;
    HeroInfo *h;
    TA_Bot *ai;
    Ovl5fUnk74 *u;
    TA_player *ta;
    s32 ex;
    s32 ez;
    s32 v;
    s32 pad;
    s32 speed;
    s32 dir;
    s32 angle;
    s32 a;
    s32 sn;
    s32 cs;
    s32 x;
    s32 z;
    s32 fx;
    s32 fz;
    s32 lim;
    s32 n;
    s32 d;
    s32 t;
    s32 df;
    s32 saved;
    u16 name;
    s32 hit;
    s32 i;

    UpdateTankHeroWeapons(player);
    if (player == 3) {
        v = *D_800CB24C++;
        D_8009D678->unk74 = v;
    } else {
        v = *D_800CB250++;
        D_8009D60C->unk74 = v;
    }
    if (v == 0) {
        LoopMoeJoeMoveAnim(player);
    }
    if (D_800CB0F0 != 0) {
        return;
    }
    ta = &TA_players[player];
    eng = Heroes[player].hero_eng;
    speed = ta->Speed;
    sub = eng->sub->next->next;
    saved = ((Ovl5fGameView *)D_8005A614)->w6C;
    ((Ovl5fGameView *)D_8005A614)->w6C = 1;
    ai = &TankBotAI[player];
    TA_HeroBot(player);
    ((Ovl5fGameView *)D_8005A614)->w6C = saved;
    pad = ai->AIpad;
    dir = pad & 0xF0;
    if (dir == 0) {
        return;
    }
    if ((pad & 0x20) && dir != 0x20) {
        ex = eng->unk10.x;
        if (D_800CB100 - speed < ex ||
            Ovl5fMapCell(ex + speed + 0x100, eng->unk10.z) != 0) {
            dir = pad & 0xD0;
        }
    }
    if ((dir & 0x80) && dir != 0x80) {
        ex = eng->unk10.x;
        if (ex < speed + 0x100 ||
            Ovl5fMapCell(ex - speed - 0x100, eng->unk10.z) != 0) {
            dir &= ~0x80;
        }
    }
    if ((dir & 0x10) && dir != 0x10) {
        ez = eng->unk10.z;
        if (D_800CB104 - speed < ez ||
            Ovl5fMapCell(eng->unk10.x, ez + speed + 0x100) != 0) {
            dir &= ~0x10;
        }
    }
    if ((dir & 0x40) && dir != 0x40) {
        ez = eng->unk10.z;
        if (ez < speed + 0x100 ||
            Ovl5fMapCell(eng->unk10.x, ez - speed - 0x100) != 0) {
            dir &= ~0x40;
        }
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

void MoeJoeRecoil(s32 arg0) {
    EXTengine *eng;
    TA_player *p;
    HeroInfo *h;
    HeroInfo *base;
    s32 val;
    s32 mode;
    s32 last;

    eng = Heroes[arg0].hero_eng;
    TA_players[arg0].SpeedY += 0xC;
    p = &TA_players[arg0];
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
            SetMoeJoeIdle(arg0);
            break;
        case 0x6E:
            SetMoeJoeMove(arg0);
            break;
        case 0x69:
            SetMoeJoeRecoil(arg0);
            break;
        case 0x6B:
            ((void (*)(s32))SetMoeJoeDead)(arg0);
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

void MoeJoeDie(s32 arg0) {
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
        nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
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
        SetMoeJoeDead(arg0, mode);
    }
}

void MoeJoeBecomeHero(NAS *nas, s32 id) {
    EXTengine *eng;
    subengine *sub;
    HeroInfo *h;
    HeroInfo *hb;
    EXTbotai *ai;
    TA_player *ta;
    TA_player *tb;
    TA_Bot *t;
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
        ai = AddExt(h->hero, 0x44, 0x400000F4);
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
    SetMoeJoeMove(p);
    sub = eng->sub;
    sub->unk68 = 0;
    sub = sub->next;
    sub->unk51 = 1;
    sub->unk68 = 0;
    sub = sub->next;
    sub->unk51 = 1;
    sub->unk68 = 0;
    SetMoeJoeIdle(p);
    tb = TA_players;
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
    ResetToBasicWeapon(p);
    ta->MainWeaponFireDelay = 0x1E;
    ta->MainWeaponNas[0] = (NAS *)0xFFEEEEFF;
    ta->MainWeaponNas[2] = (NAS *)0xFFEEEEFF;
    t = &TankBotAI[p];
    t->Doing = 0;
    t->DoingTimer = 0;
    t->Victim = 0;
    t->pMine = NULL;
    t->pMissile = 0;
    t->AIpad = 0;
    t->FirePercentChance = 0;
    if (D_800CB254 == 0x3B && D_800CB258 == D_800CB254) {
        func_8007E93C(0x3B, 0x300E, 0x300F);
        func_8007C830((s32)&Heroes[2].hero_health, (s32)&Heroes[3].hero_health);
    }
}

void PlotMoeJoeHero(NAS *nas, s32 id) {
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
    ta = &TA_players[p];
    switch (Heroes[p].hero_mode) {
    case 0x6C:
        mode = Heroes[p].hero_mode;
        Heroes[p].hero_mode = 0x6E;
        Heroes[p].hero_last_mode = mode;
        SetMoeJoeMove(p);
        break;
    case 0x6E:
        MoeJoeMove(p);
        break;
    case 0x69:
        MoeJoeRecoil(p);
        break;
    case 0x6A:
        MoeJoeDie(p);
        break;
    case 0x6B:
        HeroGhost(p);
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
    AnimateMoeJoeTurret(p);
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

void PlotMoeJoe(NAS *nas, s32 id) {
    JoeMoeFux *fux;
    EXTengine *eng;
    subengine *sub;
    subengine *s;
    s32 *p;
    ModelRec *m;
    s32 anim;
    s32 next;
    s32 z;

    fux = (JoeMoeFux *)nas->FUX;
    eng = nas->Engine;
    sub = eng->sub;
    if (fux->unk30 == 0x3B) {
        PlotMoeJoeHero(nas, id);
        return;
    }
    if (D_800CB0F0 != 0) {
        return;
    }
    if (D_800CB210 == 0) {
        sub->unk6C = D_8005373C;
        eng->unk2C.x = 0x1266;
        eng->unk2C.y = 0x1266;
        eng->unk2C.z = 0x1266;
        sub->unk74++;
        if (id == 0x131E) {
            if ((u16)(sub->unk74 - 0x4180) >= 20) {
                sub->unk74 = 0x4180;
            }
        } else if ((u16)(sub->unk74 - 0x4000) >= 20) {
            sub->unk74 = 0x4000;
        }
        s = sub->next;
        s->flags |= 0x8000;
        s->next->flags |= 0x8000;
        fux = (JoeMoeFux *)nas->FUX;
        switch (fux->unk30) {
        case 4:
            eng->unk10.y -= 0x14;
            if (eng->unk10.y < 0) {
                fux->unk30 = 0x29;
                fux->unk28 = 1;
            }
            break;
        case 0x29:
            eng->unk10.z -= 0x3C;
            if ((u32)(Heroes[0].hero_eng->unk10.x - eng->unk10.x + 0x1FF) < 0x3FF) {
                z = eng->unk10.z - 0x200;
                if (z < Heroes[0].hero_eng->unk10.z) {
                    Heroes[0].hero_eng->unk10.z = z;
                }
            }
            if (D_8005A680[0] != 0 &&
                (u32)(Heroes[1].hero_eng->unk10.x - eng->unk10.x + 0x1FF) < 0x3FF) {
                z = eng->unk10.z - 0x200;
                if (z < Heroes[1].hero_eng->unk10.z) {
                    Heroes[1].hero_eng->unk10.z = z;
                }
            }
            if (eng->unk10.z <= 0xF00) {
                MoeJoeBecomeHero(nas, id);
                fux->unk30 = 0x3B;
            }
            break;
        }
        return;
    }
    if (D_800CB210 == 0x3A) {
        fux->unk30 = 4;
        sub->unk6C = D_8005373C;
        eng->unk10.y = 0x480;
        if (id == 0x131E) {
            eng->unk10.x = 0xD00;
            sub->unk74 = 0x4180;
            TankBossCrashTurrets[0] = nas;
        } else {
            eng->unk10.x = 0x400;
            sub->unk74 = 0x400A;
            D_800CB228 = nas;
        }
        return;
    }
    anim = sub->unk74 + 1;
    next = 0;
    switch (anim) {
    case 0x419E:
        anim = 0x4180;
        next = anim;
        break;
    case 0x411E:
        anim = 0x4100;
        next = anim;
        break;
    case 0x4014:
        anim = 0x4000;
        next = anim;
        break;
    case 0x4214:
        anim = 0x4200;
        next = anim;
        break;
    case 0x40A3:
        anim = 0x4080;
        next = anim;
        break;
    }
    switch (fux->unk30) {
    case 1:
        if (next != 0) {
            anim = 0x4180;
            if (id != 0x131F) {
                anim = 0x4080;
                if (fux->unk28 & 1) {
                    anim = 0x4100;
                }
            }
            if (--fux->unk28 <= 0) {
                fux->unk30 = 0x24;
                fux->unk28 = 0xD;
                D_800CB0E4 = 0x25E;
                anim = 0x4000;
                if (id == 0x131F) {
                    anim = 0x4200;
                }
            }
        }
        break;
    case 0x24:
        if (id == 0x131E) {
            eng->unk1E += 0x96;
        } else {
            eng->unk1E -= 0x96;
        }
        eng->unk5E = 1;
        if (--fux->unk28 <= 0) {
            fux->unk30 = 9;
            if (id == 0x131F) {
                sub->unk74 = 0x4200;
            } else {
                sub->unk74 = 0x4000;
            }
        }
        break;
    case 9:
        eng->unk10.z += 0x46;
        if (id == 0x131E) {
            eng->unk10.x -= 0xD;
        } else {
            eng->unk10.x += 0xD;
        }
        if (eng->unk10.z > 0x1100) {
            fux->unk30 = 5;
            fux->unk14 = -0x46;
            sub->unk68 = 0x100;
            sub->flags |= 0x02000000;
        }
        break;
    case 5:
        fux->unk14 += 0xC;
        eng->unk10.y += fux->unk14;
        if (eng->unk10.y > 0x1000) {
            eng->unk1E = 0;
            eng->unk5E = 1;
            eng->unk10.z = D_800CB20C->unk10.z + 0x100;
            fux->unk30 = 0;
            fux->unk28 = Random(0x5A) + 0x1E;
            eng->unk2C.x = 0x14CC;
            eng->unk2C.y = 0x14CC;
            eng->unk2C.z = 0x14CC;
            eng->unk5C = 1;
            if (id == 0x131E) {
                anim = 0x2028;
                fux->unk34 = D_800C8F54;
                eng->unk10.x = 0xA00;
            } else {
                anim = 0x2027;
                fux->unk34 = D_800C9140;
                eng->unk10.x = 0x810;
            }
            m = (ModelRec *)D_800536CC;
            sub->unk6C = m;
            sub->unk74 = anim;
            if (m[1].on == 0) {
                return;
            }
            func_80016F54((u8 *)&m[1]);
        }
        break;
    case 0:
        eng->unk10.y = D_800CB20C->unk10.y - 0x528;
        if (--fux->unk28 <= 0) {
            fux->unk30 = 0x27;
            switch (Random(8)) {
            default:
            case 0:
                if (id == 0x131E) {
                    fux->unk34 = D_800C8F54;
                    anim = 0x2028;
                } else {
                    fux->unk34 = D_800C9140;
                    anim = 0x2027;
                }
                break;
            case 1:
                if (id == 0x131E) {
                    fux->unk34 = D_800C8F68;
                    anim = 0x2028;
                } else {
                    fux->unk34 = D_800C9164;
                    anim = 0x2027;
                }
                break;
            case 2:
                if (id == 0x131E) {
                    fux->unk34 = D_800C8FA8;
                    anim = 0x2028;
                } else {
                    fux->unk34 = D_800C91A4;
                    anim = 0x2027;
                }
                break;
            case 3:
                if (id == 0x131E) {
                    fux->unk34 = D_800C8FE8;
                    anim = 0x2028;
                } else {
                    fux->unk34 = D_800C91E4;
                    anim = 0x2027;
                }
                break;
            case 4:
                if (id == 0x131E) {
                    fux->unk34 = D_800C9048;
                    anim = 0x2028;
                } else {
                    fux->unk34 = D_800C9244;
                    anim = 0x2027;
                }
                break;
            case 5:
                if (id == 0x131E) {
                    fux->unk34 = D_800C9080;
                    anim = 0x2028;
                } else {
                    fux->unk34 = D_800C927C;
                    anim = 0x2027;
                }
                break;
            case 6:
                if (id == 0x131E) {
                    fux->unk34 = D_800C90B8;
                    anim = 0x2028;
                } else {
                    fux->unk34 = D_800C92B4;
                    anim = 0x2027;
                }
                break;
            case 7:
                if (id == 0x131E) {
                    fux->unk34 = D_800C911C;
                    anim = 0x2028;
                } else {
                    fux->unk34 = D_800C9318;
                    anim = 0x2027;
                }
                break;
            }
        } else {
            anim = sub->unk74;
        }
        break;
    case 0x27:
        eng->unk10.y = D_800CB20C->unk10.y - 0x528;
        p = fux->unk34;
        anim = *p;
        if (D_8004E0D8 & 1) {
            fux->unk34 = p + 1;
        }
        if (anim == 0) {
            anim = 0x2027;
            if (id == 0x131E) {
                anim = 0x2028;
            }
            fux->unk30 = 0;
            fux->unk28 = Random(0x1E) + 0xF;
        }
        break;
    case 0x3A:
        break;
    }
    sub->unk74 = anim;
}

void PlotMoeJoe(NAS *nas, s32 id);

void PlotJoe(NAS *nas) {
    PlotMoeJoe(nas, 0x131F);
}

void InitMoeJoe(NAS *nas, s32 id);

void InitJoe(NAS *nas) {
    InitMoeJoe(nas, 0x131F);
}

void PlotMoe(NAS *nas) {
    PlotMoeJoe(nas, 0x131E);
}

void InitMoe(NAS *nas) {
    InitMoeJoe(nas, 0x131E);
}

void MoeJoeDead(void) {
    HeroGhost();
}

void SwitchMoeJoeToMove(s32 arg0) {
    u16 prev = Heroes[arg0].hero_mode;
    Heroes[arg0].hero_mode = 0x6E;
    Heroes[arg0].hero_last_mode = prev;
    SetMoeJoeMove(arg0);
}

void SetMoeJoeMode(s32 hero, s32 mode) {
    u16 last = Heroes[hero].hero_mode;

    Heroes[hero].hero_mode = mode;
    Heroes[hero].hero_last_mode = last;
    switch (mode) {
    case 0x6C:
        ((void (*)())SetMoeJoeIdle)();
        break;
    case 0x6E:
        ((void (*)())SetMoeJoeMove)();
        break;
    case 0x69:
        ((void (*)())SetMoeJoeRecoil)();
        break;
    case 0x6B:
        SetMoeJoeDead(hero, mode);
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
    PlotTotemBit(nas, 0x131C);
}

void InitTotemBot(NAS *nas) {
    InitTotemBit(nas, 0x131C);
}

void PlotTotemTop(NAS *nas) {
    PlotTotemBit(nas, 0x131B);
}

void InitTotemTop(NAS *nas) {
    InitTotemBit(nas, 0x131B);
}

void InitTotemPole(NAS *nas) {
    TotemPoleExt *ext;
    EXTengine *e;

    D_800CB218 = nas;
    ext = AddExt(nas, 0x38, 0x10000000);
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
        D_800CB21C = CreateNAS(e->unk10.x, e->unk10.y - 0x294, e->unk10.z, 0x131B, 0x11);
        D_800CB220 = CreateNAS(e->unk10.x, e->unk10.y - 0x14A, e->unk10.z, 0x131C, 0x11);
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
    PeriscopeShellExt *ext = AddExt(nas, 0x38, 0x10000000);
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
    ext = AddExt(nas, 0x38, 0x10000000);
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
        if (PU_TA_list[i] == nas) {
            PU_TA_list[i] = NULL;
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
            if (TankMines[i] == NULL) {
                TankMines[i] = nas;
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
            if (TankMines[i] == NULL) {
                TankMines[i] = nas;
                break;
            }
        }
    }
}

void InitRocketTA(NAS *nas) {
    FlamerBitExt *ext;
    subengine *sub;

    ext = AddExt(nas, 0x38, 0x10000000);
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
    e2 = AddExtEngine(nas, 1);
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
    e2 = AddExtEngine(nas, 1);
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

    ext = AddExt(nas, 0x38, 0x10000000);
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

    ext = AddExt(nas, 0x38, 0x10000000);
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
        e2 = AddExtEngine(nas, 1);
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

    ext = AddExt(nas, 0x38, 0x10000000);
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

    ext = AddExt(nas, 0x38, 0x10000000);
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

void BumpSpeedsForSwamp(EXTobject_TA *pObject) {
    pObject->SpeedX = pObject->SpeedX * 3 / 2;
    pObject->SpeedZ = pObject->SpeedZ * 3 / 2;
    pObject->Counter = pObject->Counter * 2 / 3;
}

void TankHeroMinelay(u32 player) {
}

void SetTankHeroMinelay(u32 arg0) {
    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction(arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
    }
}

void TankHeroDead(u32 player) {
    HeroGhost(player);
}

void SetTankHeroDie(u32 player) {
    SetHeroSeq(player, 0x58);
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

void SetTankHeroMove(u32 arg0) {
    s32 ok;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction(arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 0) {
        SetHeroSeq(arg0, 0x59);
    }
}

void func_800C7E68(s32 player, Ovl5fActObj *obj, s32 arg2) {
    SetHeroAction(player, 0x6F);
    obj->unk54 = -0x80;
    obj->unk50 = 8;
    obj->unk8 = arg2;
}

void func_800C7EB0(s32 player, Ovl5fActState *state, s32 arg2) {
    SetHeroAction(player, 0x6F);
    state->unk54 = 0x80;
    state->unk50 = 8;
    state->unk8 = arg2;
}

void TankHeroIdle(u32 player) {
    SetHeroAction(player, 0x6E);
}

void func_800C7F18(NAS *nas, s32 player) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (TA_players[player].MainWeaponNas[i] == nas) {
            TA_players[player].MainWeaponNas[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void SetFireBreatherSFX(s32 player, EXTengine *unused, s32 ReqdSFX) {
    if (TA_players[player].FireBreatherSFX != ReqdSFX) {
        TA_players[player].FireBreatherSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            StopSfx(TA_players[player].FireBreatherSFXHandler);
        } else {
            PlaySfx(ReqdSFX, &TA_players[player].FireBreatherSFXHandler, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void SetTurretSFX(s32 player, EXTengine *unused, s32 ReqdSFX) {
    if (TA_players[player].TurretSFX != ReqdSFX) {
        TA_players[player].TurretSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            StopSfx(TA_players[player].TurretSFXHandler);
        } else {
            PlaySfx(ReqdSFX, &TA_players[player].TurretSFXHandler, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void SetHullSFX(s32 player, EXTengine *pEngine, s32 ReqdSFX) {
    if (TA_players[player].HullSFX != ReqdSFX) {
        TA_players[player].HullSFX = ReqdSFX;
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
        SetHeroAction(arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
        return 1;
    }
    return 0;
}

void TA_PreMoveHero(u32 player) {
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
                DamageTankHero(i, arg3);
            }
        }
    }
}

bool TA_KeepInBounds(EXTengine *p) {
    s32 r;
    s32 lim;

    r = 0;
    if (p->unk10.x > D_800CB100) {
        lim = D_800CB100;
        p->unk10.x = lim;
        r = 1;
    }
    if (p->unk10.x < 0x100) {
        p->unk10.x = 0x100;
        r = 1;
    }
    if (p->unk10.z > D_800CB104) {
        lim = D_800CB104;
        p->unk10.z = lim;
        r = 1;
    }
    if (p->unk10.z < 0x100) {
        p->unk10.z = 0x100;
        r = 1;
    }
    return r;
}

void func_800C831C(void) {
}

void TA_Triggers(NASTrigger *arg0, s32 arg1) {
}

void TA_EarliestTriggers(void) {
}

void TA_CreateShadow(u32 arg0) {
    Heroes[arg0].hero_shadow->flags = 0;
}

void TA_StopHeroSfx(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (TA_players[i].TurretSFXHandler != NULL) {
            StopSfx(TA_players[i].TurretSFXHandler);
        }
        if (TA_players[i].FireBreatherSFXHandler != NULL) {
            StopSfx(TA_players[i].FireBreatherSFXHandler);
        }
    }
}

void func_800C83D0(void) {
}

void TA_SetUp(void) {
    D_8005AA70[0] = (s32)TA_PreInitLevel;
    D_8005AA70[1] = (s32)TA_InitLevel;
    D_8005AA70[2] = (s32)TA_InitLevel;
    D_8005AA70[3] = (s32)TA_UpdateLevel;
    D_8005AA70[4] = (s32)TA_StopHeroSfx;
    D_8005AA70[6] = (s32)TA_Triggers;
    D_8005AA70[5] = (s32)TA_EarliestTriggers;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[10] = (s32)FinishTankLevel;
    D_800CB25C = 1;
}

void func_800C8450(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = TankMines[i];
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

u32 GetSandbagValueTank(s32 arg0, s32 arg1) {
    return *(((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9) + D_800CACFC);
}

u32 GetSandbagValueMiss(s32 arg0, s32 arg1) {
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

void TankBotReset(s32 player) {
    TA_Bot *bot;

    bot = &TankBotAI[player];
    bot->Doing = 0;
    bot->DoingTimer = 0;
    bot->Victim = 0;
    bot->pMine = NULL;
    bot->pMissile = NULL;
    bot->AIpad = 0;
    bot->FirePercentChance = 0;
}

NAS *func_800C8A80(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    NAS *nas;
    EXTengine *eng;

    for (i = 0; i < 3; i++) {
        nas = PU_TA_list[i];
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
            TA_players[player].TurretReqdDir =
                (get_angle(target->unk10.x - eng->unk10.x, target->unk10.z - eng->unk10.z) + 0x800) & 0xFFF;
        }
    }
}

void func_800C8BA0(void) {
    switch ((s16)(D_8005A648 - 5)) {
    case 1:
    case 3:
        TankBotPickLinedUpTarget();
        break;
    case 0:
    case 2:
    case 4:
    default:
        TankBotPickTarget();
        break;
    }
}
