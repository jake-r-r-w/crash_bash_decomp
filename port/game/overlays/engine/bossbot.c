/* A test run's player for the boss fights that need aiming (--aim): it looks where things are, as a player does, and only ever presses
 * the pad's buttons, so a boss goes down by the fight's own rules. */
#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/hero.h"
#include "bash/papu_ovl.h"
#include "bash/tank_ovl.h"

void platform_bot_buttons(int buttons);
extern s32 PapuBossFlag;

#define BTN_UP 0x10
#define BTN_RIGHT 0x20
#define BTN_DOWN 0x40
#define BTN_LEFT 0x80
#define BTN_L1 0x400
#define BTN_R1 0x800
#define BTN_SQUARE 0x8000
#define BTN_CIRCLE 0x2000
#define BTN_CROSS 0x4000

static s32 BotTicks;
static s32 BearFlip;

static s32 step_towards(s32 dx, s32 dz, s32 near_enough) {
    s32 b = 0;

    if (dx > near_enough) {
        b |= BTN_RIGHT;
    } else if (dx < -near_enough) {
        b |= BTN_LEFT;
    }
    if (dz > near_enough) {
        b |= BTN_UP;
    } else if (dz < -near_enough) {
        b |= BTN_DOWN;
    }
    return b;
}

static NAS *PapuCrate;
static s32 PapuCrateUntil;

/* The nearest minion still up, or NULL. */
static EXTengine *papu_minion(const EXTengine *he) {
    EXTengine *best = NULL;
    s32 bestD = 0x7FFFFFFF;
    s32 i;

    for (i = 0; i < 6; i++) {
        NAS *n = (NAS *)PapuMinionSlots[i].flag;

        if (n != NULL && n->Engine != NULL) {
            s32 ex = (n->Engine->XYZ.x - he->XYZ.x) >> 4;
            s32 ez = (n->Engine->XYZ.z - he->XYZ.z) >> 4;
            s32 d = ex * ex + ez * ez;

            if (d < bestD) {
                bestD = d;
                best = n->Engine;
            }
        }
    }
    return best;
}

/* Carrying a crate: square up to the target on one axis, turn to face it, and throw. */
static s32 throw_at(const EXTengine *he, const EXTengine *target, s32 ready) {
    s32 dx = target->XYZ.x - he->XYZ.x;
    s32 dz = target->XYZ.z - he->XYZ.z;

    if (abs(dz) >= abs(dx)) {
        if (abs(dx) > 0x180) {
            return dx > 0 ? BTN_RIGHT : BTN_LEFT;
        }
        if (abs(dz) > 0x1800) {
            return dz > 0 ? BTN_UP : BTN_DOWN;
        }
        if ((BotTicks & 15) < 3) {
            return dz > 0 ? BTN_UP : BTN_DOWN;
        }
    } else {
        if (abs(dz) > 0x180) {
            return dz > 0 ? BTN_UP : BTN_DOWN;
        }
        if (abs(dx) > 0x1800) {
            return dx > 0 ? BTN_RIGHT : BTN_LEFT;
        }
        if ((BotTicks & 15) < 3) {
            return dx > 0 ? BTN_RIGHT : BTN_LEFT;
        }
    }
    return ready && (BotTicks & 15) == 8 ? BTN_CIRCLE : 0;
}

/* Papu Papu: each wave of minions goes first, by thrown crates, and he only hurts while he's open (PapuBossFlag). */
static s32 play_papu(void) {
    HeroInfo *h = &Heroes[0];
    EXTengine *he = h->hero_eng;
    EXTengine *minion;
    NAS *n, *best = NULL;
    s32 bestD = 0x7FFFFFFF;
    s32 dx, dz;

    if (he == NULL || PapuBoss.nas == NULL || PapuBoss.nas->Engine == NULL) {
        return 0;
    }
    if (h->hero_object != NULL) {
        minion = papu_minion(he);
        if (minion != NULL) {
            return throw_at(he, minion, 1);
        }
        return throw_at(he, PapuBoss.nas->Engine, PapuBossFlag != 0);
    }
    for (n = HeadOfObjects; n != NULL; n = n->Object.Child) {
        EXTobject *fux = (EXTobject *)n->FUX;

        if (fux != NULL && (fux->can_do & 1) && n->Engine != NULL && n != PapuBoss.nas) {
            s32 ex = (n->Engine->XYZ.x - he->XYZ.x) >> 4;
            s32 ez = (n->Engine->XYZ.z - he->XYZ.z) >> 4;
            s32 d = ex * ex + ez * ez;

            if (n == PapuCrate && BotTicks < PapuCrateUntil) {
                best = n;
                break;
            }
            if (d < bestD) {
                bestD = d;
                best = n;
            }
        }
    }
    if (best != PapuCrate || BotTicks >= PapuCrateUntil) {
        PapuCrate = best;
        PapuCrateUntil = BotTicks + 90;
    }
    if (best == NULL) {
        return 0;
    }
    dx = best->Engine->XYZ.x - he->XYZ.x;
    dz = best->Engine->XYZ.z - he->XYZ.z;
    if (abs(dz) >= abs(dx)) {
        if (abs(dx) > 0x80) {
            return dx > 0 ? BTN_RIGHT : BTN_LEFT;
        }
        if (abs(dz) > 0x1C0 || (BotTicks & 15) < 4) {
            return dz > 0 ? BTN_UP : BTN_DOWN;
        }
    } else {
        if (abs(dz) > 0x80) {
            return dz > 0 ? BTN_UP : BTN_DOWN;
        }
        if (abs(dx) > 0x1C0 || (BotTicks & 15) < 4) {
            return dx > 0 ? BTN_RIGHT : BTN_LEFT;
        }
    }
    return (BotTicks & 15) == 8 ? BTN_CIRCLE : 0;
}

/* The Bearminator: the sniper missile drops into the middle of the ice, so wait there; touching it brings up a crosshair, to
   put on him and fire. */
typedef struct BearSniper {
    u8 pad[sizeof(EXT)];
    s32 state;
    EXTengine *aimEng;
    s32 savedEyeZ;
    s32 timer;
    s32 timer2;
    EXTengine *aimEng2;
    s32 aimX;
    s32 aimY;
} BearSniper;

extern NAS *PolarSniperNas;
extern s32 PolarSniperSeen;
extern NAS *polarbossnas;
NAS *PolarFirstBear(void);
s32 PolarHeroMeter(s32 player);
s32 CountBears(void);

#define BEAR_REACH 0x400
#define BEAR_SAFE_R 0x900

static s32 play_bear(void) {
    EXTengine *he = Heroes[0].hero_eng;
    BearSniper *sn = NULL;

    if (he == NULL || polarbossnas == NULL || polarbossnas->Engine == NULL) {
        return 0;
    }
    if (PolarSniperNas != NULL && (u32)(FrameCount - PolarSniperSeen) < 4) {
        sn = FindExtension(PolarSniperNas, 0x10000000);
    }
    if (sn != NULL && sn->state == 1) {
        s32 off = polarbossnas->Engine->XYZ.x - sn->aimEng2->sub->XYZ.x;
        s32 up = polarbossnas->Engine->XYZ.y - sn->aimEng2->sub->XYZ.y;
        s32 b = 0;

        if (off > 0x80) {
            b |= BearFlip ? BTN_LEFT : BTN_RIGHT;
        } else if (off < -0x80) {
            b |= BearFlip ? BTN_RIGHT : BTN_LEFT;
        }
        if (up > 0x180) {
            b |= BTN_DOWN;
        } else if (up < -0x180) {
            b |= BTN_UP;
        }
        if (b == 0 && (BotTicks & 7) == 0) {
            b = BTN_SQUARE;
        }
        return b;
    }
    /* a mechanical bear about: charge it off the ice, as the missile only comes when they're gone */
    {
        NAS *bear = PolarFirstBear();

        if (bear != NULL && bear->Engine != NULL) {
            s32 bx = bear->Engine->XYZ.x, bz = bear->Engine->XYZ.z;
            s32 hr = csqrt(he->XYZ.x * he->XYZ.x + he->XYZ.z * he->XYZ.z) >> 6;
            s32 br = csqrt(bx * bx + bz * bz) >> 6;
            s32 dx = bx - he->XYZ.x, dz = bz - he->XYZ.z;
            s32 d = csqrt(dx * dx + dz * dz) >> 6;

            /* hold the middle while the bar fills. Full, go for a bear that's further out than the hero, so the hit drives it at the
               edge, and charge in reach; never so far out that the hero goes over himself. */
            if (PolarHeroMeter(0) >= 0x100 && hr < BEAR_SAFE_R && br > hr) {
                if (d < BEAR_REACH) {
                    return step_towards(dx, dz, 0x10) | (Heroes[0].hero_mode != 0x51 && (BotTicks & 7) == 0 ? BTN_SQUARE : 0);
                }
                return step_towards(dx, dz, 0x10);
            }
            return hr > 0x180 ? step_towards(-he->XYZ.x, -he->XYZ.z, 0x80) : 0;
        }
    }
    return step_towards(-he->XYZ.x, -he->XYZ.z, 0x100);
}

#define FOX_RANGE 0x700 /* close up to the cannons, past the ice */
#define FOX_SWEEP 40
static s32 FoxSweep, FoxSweepDir = 1, FoxLastX, FoxLastZ;

/* Big Bad Fox's next target, in the order the fight brings them down: a list's turrets are done at flags 2. */
static EXTengine *fox_list(NAS **list, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (list[i] != NULL && list[i]->UserInfo.flags != 2 && list[i]->Engine != NULL) {
            return list[i]->Engine;
        }
    }
    return NULL;
}

static EXTengine *fox_target(void) {
    EXTengine *t;
    s32 i;

    if (TankBossStatus >= 0x2E && TankBossStatus <= 0x31) {
        return fox_list(TankBossCrashTurrets, 3);
    }
    if (TankBossStatus >= 0x32 && TankBossStatus <= 0x36) {
        return fox_list(TankBossCortexTurrets, 4);
    }
    if (TankBossStatus >= 0x37 && TankBossStatus <= 0x39) {
        return fox_list(TankBossRocketTurrets, 2);
    }
    for (i = 3; i >= 2; i--) {
        if (Heroes[i].hero_eng != NULL && !(Heroes[i].hero_status & 0x6000)) {
            return Heroes[i].hero_eng;
        }
    }
    t = NULL;
    return t;
}

/* Drive up in front of the target, turn the gun onto it, and fire when it's on. */
static s32 play_fox(void) {
    EXTengine *he = Heroes[0].hero_eng;
    EXTengine *t = fox_target();
    s32 buttons = 0;
    s32 dx, dz, want, off;

    if (he == NULL || t == NULL) {
        return 0;
    }
    dx = t->XYZ.x - he->XYZ.x;
    dz = t->XYZ.z - he->XYZ.z;
    if (TankBossStatus == 0) {
        s32 d = abs(dx) > abs(dz) ? abs(dx) : abs(dz);

        if (d > 0x900) {
            buttons |= step_towards(dx, dz, 0x100);
        } else if (d < 0x400) {
            buttons |= step_towards(-dx, -dz, 0x100);
        } else if ((BotTicks & 127) < 24) {
            /* and move about, as a target that's sat in a corner won't be hit */
            buttons |= (BotTicks & 256) ? BTN_LEFT : BTN_RIGHT;
        }
    } else if (dz > FOX_RANGE) {
        /* up, and when that stops getting anywhere along the wall for a way through, as a player would */
        if (FoxSweep > 0) {
            FoxSweep--;
            buttons |= FoxSweepDir > 0 ? BTN_RIGHT : BTN_LEFT;
            if (abs(he->XYZ.x - FoxLastX) < 4 && FoxSweep < FOX_SWEEP - 20) {
                FoxSweepDir = -FoxSweepDir;
                FoxSweep = FOX_SWEEP;
            }
        } else {
            buttons |= BTN_UP;
            if ((BotTicks & 31) == 0) {
                if (he->XYZ.z - FoxLastZ < 16) {
                    FoxSweep = FOX_SWEEP;
                }
                FoxLastZ = he->XYZ.z;
            }
        }
        if ((BotTicks & 15) == 0) {
            FoxLastX = he->XYZ.x;
        }
    } else if (abs(dx) > 0x300) {
        buttons |= dx > 0 ? BTN_RIGHT : BTN_LEFT;
    }
    want = (get_angle(dx, dz) + 0x800) & 0xFFF;
    off = ((want - he->xform.rot.vy + 0x800) & 0xFFF) - 0x800;
    if (off > 0x20) {
        buttons |= BTN_R1;
    } else if (off < -0x20) {
        buttons |= BTN_L1;
    }
    if (abs(off) < 0x80 && (BotTicks & 7) == 0) {
        buttons |= BTN_SQUARE;
    }
    return buttons;
}

void BossBotUpdate(void) {
    s32 buttons = 0;

    BotTicks++;
    /* the end of a fight waits for a button */
    if (GAME->exitFlags != 0 || (BossState.flags & 0x8000)) {
        platform_bot_buttons((BotTicks & 31) < 3 ? BTN_CROSS : 0);
        return;
    }
    switch (GameMode) {
    case 4:
        buttons = play_papu();
        break;
    case 9:
        buttons = play_fox();
        break;
    case 22:
        buttons = play_bear();
        break;
    }
    platform_bot_buttons(buttons);
}

/* How far a boss fight has got, as a number that changes when anything does (--stall). */
s32 CheatFightProgress(void) {
    s32 n = BossState.health * 0x100 + BossState.flags;
    s32 i;

    if (GameMode == 22) {
        n += CountBears() * 0x10000 + (PolarSniperNas != NULL && (u32)(FrameCount - PolarSniperSeen) < 4 ? 0x100000 : 0);
    }
    if (GameMode == 4) {
        for (i = 0; i < 6; i++) {
            n += PapuMinionSlots[i].flag != NULL ? (i + 1) * 0x10000 : 0;
        }
    } else if (GameMode == 9) {
        n += TankBossStatus * 0x10000 + Heroes[3].hero_health * 0x100000 + Heroes[2].hero_health * 0x1000000;
        for (i = 0; i < 3; i++) {
            n += TankBossCrashTurrets[i] != NULL ? TankBossCrashTurrets[i]->UserInfo.flags : 0;
        }
        for (i = 0; i < 4; i++) {
            n += TankBossCortexTurrets[i] != NULL ? TankBossCortexTurrets[i]->UserInfo.flags * 3 : 0;
        }
        for (i = 0; i < 2; i++) {
            n += TankBossRocketTurrets[i] != NULL ? TankBossRocketTurrets[i]->UserInfo.flags * 7 : 0;
        }
    }
    return n;
}

/* What a test run's log says of the fight, beyond the boss's health. */
void CheatFightLog(void) {
    if (GameMode == 22) {
        extern int printf(const char *, ...);
        BearSniper *sn = PolarSniperNas != NULL && (u32)(FrameCount - PolarSniperSeen) < 4 ? FindExtension(PolarSniperNas, 0x10000000) : NULL;

        printf("  dir %X held %X frozen %d ctrl %X status %X speed %d; ", Pads[0].dir, Pads[0].held, Heroes[0].hero_frozen, Heroes[0].hero_ctrl, Heroes[0].hero_status, Heroes[0].hero_move->forward_speed);
        printf("  bears %d at %d,%d,%d; meter %d mode %X; ", CountBears(), PolarFirstBear() != NULL ? PolarFirstBear()->Engine->XYZ.x : 0, PolarFirstBear() != NULL ? PolarFirstBear()->Engine->XYZ.y : 0, PolarFirstBear() != NULL ? PolarFirstBear()->Engine->XYZ.z : 0, PolarHeroMeter(0), Heroes[0].hero_mode);
        printf("bear hero at %d,%d y %d; boss at %d,%d,%d", Heroes[0].hero_eng->XYZ.x, Heroes[0].hero_eng->XYZ.z, Heroes[0].hero_eng->XYZ.y, polarbossnas != NULL ? polarbossnas->Engine->XYZ.x : 0,
               polarbossnas != NULL ? polarbossnas->Engine->XYZ.y : 0, polarbossnas != NULL ? polarbossnas->Engine->XYZ.z : 0);
        if (sn != NULL) {
            printf("; sniper state %d aim %d,%d at %d,%d", sn->state, sn->aimX, sn->aimY, sn->aimEng2->sub->XYZ.x, sn->aimEng2->sub->XYZ.y);
        }
        printf("\n");
    }
    if (GameMode == 9) {
        extern int printf(const char *, ...);
        printf("  fox status %X, moe %d joe %d; turret flags %d %d %d\n", TankBossStatus, Heroes[3].hero_health, Heroes[2].hero_health,
               TankBossCrashTurrets[0] != NULL ? TankBossCrashTurrets[0]->UserInfo.flags : -1,
               TankBossCortexTurrets[0] != NULL ? TankBossCortexTurrets[0]->UserInfo.flags : -1,
               TankBossRocketTurrets[0] != NULL ? TankBossRocketTurrets[0]->UserInfo.flags : -1);
        {
            static s32 shown;
            s32 x, z;

            if (!shown) {
                shown = 1;
                for (z = 0x1000; z >= -0x1000; z -= 0x200) {
                    printf("  map z %6d: ", z);
                    for (x = -0x1000; x <= 0x1000; x += 0x200) {
                        printf("%c", TankMapCell(x, z) ? '#' : '.');
                    }
                    printf("\n");
                }
            }
        }
        {
            s32 i;

            for (i = 1; i < 4; i++) {
                printf("  other hero %d name %d status %X at %d,%d flags %X mode %X\n", i, Heroes[i].hero_name, Heroes[i].hero_status, Heroes[i].hero_eng->XYZ.x, Heroes[i].hero_eng->XYZ.z, Heroes[i].hero_eng->flags, Heroes[i].hero_mode);
            }
        }
        printf("  fox hero at %d,%d rot %X mode %X frozen %d status %X; target ", Heroes[0].hero_eng->XYZ.x, Heroes[0].hero_eng->XYZ.z, Heroes[0].hero_eng->xform.rot.vy, Heroes[0].hero_mode, Heroes[0].hero_frozen, Heroes[0].hero_status);
        if (fox_target() != NULL) {
            printf("%d,%d\n", fox_target()->XYZ.x, fox_target()->XYZ.z);
        } else {
            printf("none\n");
        }
    } else if (GameMode == 4) {
        extern int printf(const char *, ...);
        {
            NAS *n;
            s32 shown = 0;

            for (n = HeadOfObjects; n != NULL && shown < 3; n = n->Object.Child) {
                EXTobject *fux = (EXTobject *)n->FUX;

                if (fux != NULL && (fux->can_do & 1) && n->Engine != NULL) {
                    printf("  crate at %d,%d can_do %X platform %p\n", n->Engine->XYZ.x, n->Engine->XYZ.z, fux->can_do, (void *)n->Engine->platform);
                    shown++;
                }
            }
            printf("  hero mode %X facing %X frozen %d\n", Heroes[0].hero_mode, Heroes[0].hero_move->facing_angle, Heroes[0].hero_frozen);
        }
        if (PapuCrate != NULL && PapuCrate->Engine != NULL) {
            printf("  after crate at %d,%d\n", PapuCrate->Engine->XYZ.x, PapuCrate->Engine->XYZ.z);
        }
        printf("  papu hero at %d,%d carrying %d; papu at %d,%d open %d; minions %d\n", Heroes[0].hero_eng->XYZ.x, Heroes[0].hero_eng->XYZ.z, Heroes[0].hero_object != NULL,
               PapuBoss.nas != NULL ? PapuBoss.nas->Engine->XYZ.x : 0, PapuBoss.nas != NULL ? PapuBoss.nas->Engine->XYZ.z : 0, PapuBossFlag,
               (PapuMinionSlots[0].flag != NULL) + (PapuMinionSlots[1].flag != NULL) + (PapuMinionSlots[2].flag != NULL) + (PapuMinionSlots[3].flag != NULL));
    }
}
