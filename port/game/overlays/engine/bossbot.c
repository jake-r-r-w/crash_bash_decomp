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
extern NAS *PolarMissiles[4];
NAS *PolarFirstBear(void);
s32 PolarBearState(NAS *bear);
s32 PolarBearFireIn(NAS *bear);
s32 BearminatorOffIce(s32 *pos);
extern subengine *bossmapsub;
s32 PolarHeroAttackPower(s32 player);
void PolarBossBodyAim(s32 *x, s32 *y);
s32 PolarHeroMeter(s32 player);
s32 CountBears(void);

#define BEAR_REACH 0x300
#define BEAR_CLOSE 0x280
#define BEAR_STANDOFF 0x2C0
#define BEAR_EDGE 0x280
#define BEAR_STEP_OUT 0x500
#define BEAR_FIRE_SOON 12
#define BEAR_FAR 0xE00
#define BEAR_DODGE 0x200
#define BEAR_AIMING 5
#define BEAR_FALLING 0x4E

#define BEAR_AIM_X 0x60
#define BEAR_AIM_Y 0x80
#define BEAR_SHOT_TICKS 24

static s32 BearSide;
static s32 BearLastX;
static s32 BearMapSeen = -1;
static s32 BearMidX, BearMidZ;
static const s16 BearDirs[8][2] = {
    { 0x1000, 0 }, { 0xB50, 0xB50 }, { 0, 0x1000 }, { -0xB50, 0xB50 }, { -0x1000, 0 }, { -0xB50, -0xB50 }, { 0, -0x1000 }, { 0xB50, -0xB50 }
};

/* The nearest of the pad's eight ways to a direction: a lunge goes where the pad points. */
static s32 aim_at(s32 dx, s32 dz) {
    s32 big = abs(dx) > abs(dz) ? abs(dx) : abs(dz);

    return step_towards(dx, dz, big * 5 / 12);
}

static s32 off_ice(s32 x, s32 z) {
    s32 pos[3];

    pos[0] = x;
    pos[1] = 0;
    pos[2] = z;
    return BearminatorOffIce(pos);
}

/* The middle of what ice is left, worked out again when a chunk goes: the arena's own middle may be gone. */
static void ice_middle(void) {
    s32 x, z;
    s32 n = 0, sx = 0, sz = 0;

    if (BearMapSeen == bossmapsub->entity) {
        return;
    }
    BearMapSeen = bossmapsub->entity;
    for (z = -BEAR_FAR; z <= BEAR_FAR; z += 0x100) {
        for (x = -BEAR_FAR; x <= BEAR_FAR; x += 0x100) {
            if (!off_ice(x, z)) {
                sx += x >> 4;
                sz += z >> 4;
                n++;
            }
        }
    }
    BearMidX = n != 0 ? sx / n * 16 : 0;
    BearMidZ = n != 0 ? sz / n * 16 : 0;
}

static s32 dir_x(s32 i) {
    return BearDirs[i][0];
}

static s32 dir_z(s32 i) {
    return BearDirs[i][1];
}

/* Whether the hero's near an edge. */
static s32 near_edge(s32 x, s32 z) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (off_ice(x + ((dir_x(i) * BEAR_EDGE) >> 12), z + ((dir_z(i) * BEAR_EDGE) >> 12))) {
            return 1;
        }
    }
    return 0;
}

/* Sidesteps a shot along the facing of e, a missile or a robot about to fire one: 0 if it isn't coming this way. The hero
   holds to one side, one with ice on it, as a robot turns to follow him and swapping sides lets it catch up. */
static s32 bear_dodge(EXTengine *he, EXTengine *e) {
    s32 vy = e->xform.rot.vy;
    s32 fx = -rcossin_tbl[(vy - 0x400) & 0xFFF].unk2;
    s32 fz = -rcossin_tbl[vy & 0xFFF].unk2;
    s32 rx = he->XYZ.x - e->XYZ.x;
    s32 rz = he->XYZ.z - e->XYZ.z;
    s32 along = (rx * fx + rz * fz) >> 12;
    s32 across = (rx * fz - rz * fx) >> 12;

    if (along < -0x100 || abs(across) > BEAR_DODGE) {
        return 0;
    }
    if (BearSide == 0 || off_ice(he->XYZ.x + ((fz * BearSide * BEAR_EDGE) >> 12), he->XYZ.z - ((fx * BearSide * BEAR_EDGE) >> 12))) {
        BearSide = ((he->XYZ.x - BearMidX) >> 4) * fz - ((he->XYZ.z - BearMidZ) >> 4) * fx > 0 ? -1 : 1;
        if (off_ice(he->XYZ.x + ((fz * BearSide * BEAR_EDGE) >> 12), he->XYZ.z - ((fx * BearSide * BEAR_EDGE) >> 12))) {
            BearSide = -BearSide;
        }
    }
    return step_towards(fz * BearSide, -fx * BearSide, 0x600);
}

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
        s32 tx, ty, off, up;
        s32 b = 0;

        /* the crosshair stands in the sub's plane and the shot flies through it, so it's the world's x and y; the sub moves on
           meanwhile, for as long as the shot's flying */
        PolarBossBodyAim(&tx, &ty);
        tx += (polarbossnas->Engine->XYZ.x - BearLastX) * BEAR_SHOT_TICKS;
        BearLastX = polarbossnas->Engine->XYZ.x;
        off = tx - sn->aimEng2->sub->XYZ.x;
        up = ty - sn->aimEng2->sub->XYZ.y;
        if (off > BEAR_AIM_X) {
            b |= BTN_RIGHT;
        } else if (off < -BEAR_AIM_X) {
            b |= BTN_LEFT;
        }
        if (up > BEAR_AIM_Y) {
            b |= BTN_DOWN;
        } else if (up < -BEAR_AIM_Y) {
            b |= BTN_UP;
        }
        if (b == 0 && (BotTicks & 1) == 0) {
            b = BTN_SQUARE;
        }
        return b;
    }
    BearLastX = polarbossnas->Engine->XYZ.x;
    /* the missile the last robot left: go and get it before it goes off */
    if (sn != NULL && sn->state == 0 && PolarSniperNas->Engine != NULL) {
        return aim_at(PolarSniperNas->Engine->XYZ.x - he->XYZ.x, PolarSniperNas->Engine->XYZ.z - he->XYZ.z);
    }
    ice_middle();
    {
        NAS *bear, *best = NULL;
        s32 bestd = 0x7FFFFFFF;
        s32 i, b;
        s32 mx = BearMidX - he->XYZ.x, mz = BearMidZ - he->XYZ.z;

        /* going over: push back into the arena and press repeatedly, as the rules say */
        if (Heroes[0].hero_mode == BEAR_FALLING) {
            return aim_at(mx, mz) | ((BotTicks & 1) != 0 ? BTN_SQUARE : 0);
        }
        /* a missile stuns, and a stunned hero gets shoved off: out of its way before anything else */
        for (i = 0; i < 4; i++) {
            if (PolarMissiles[i] != NULL && PolarMissiles[i]->Engine != NULL
                && (b = bear_dodge(he, PolarMissiles[i]->Engine)) != 0) {
                return b;
            }
        }
        for (bear = PolarFirstBear(); bear != NULL; bear = bear->Group.Child) {
            if (bear->Engine != NULL) {
                s32 dx = bear->Engine->XYZ.x - he->XYZ.x, dz = bear->Engine->XYZ.z - he->XYZ.z;
                s32 d = csqrt(dx * dx + dz * dz) >> 6;
                s32 state = PolarBearState(bear);

                /* each robot fires once, a while after it lands, along the way it faces: it follows the hero until then, so
                   being on the move as it fires is what makes it miss */
                if (state == BEAR_AIMING && PolarBearFireIn(bear) < BEAR_FIRE_SOON && (b = bear_dodge(he, bear->Engine)) != 0) {
                    return b;
                }
                /* in the air, aiming, or going over already: nothing to hit */
                if (state != 3 && state != 4 && state != BEAR_AIMING && state != 7 && state != 9 && d < bestd) {
                    bestd = d;
                    best = bear;
                }
            }
        }
        BearSide = 0;
        if (Heroes[0].hero_mode == 0x51) {
            return best != NULL ? aim_at(best->Engine->XYZ.x - he->XYZ.x, best->Engine->XYZ.z - he->XYZ.z) : 0;
        }
        /* never mind the robots when the edge is close */
        if (near_edge(he->XYZ.x, he->XYZ.z)) {
            return aim_at(mx, mz);
        }
        /* Near the middle a robot's shove hardly moves the hero, and it's the edge that kills. A charge drives one about 1450 along
           the lunge, and the bounce brings the hero back: so stand a step out towards the nearest, a lunge from it, and hit it in
           reach. From there one further out goes over, and one from the middle gets hit again as it walks back. */
        if (best != NULL) {
            s32 bx = best->Engine->XYZ.x, bz = best->Engine->XYZ.z;
            s32 dx = bx - he->XYZ.x, dz = bz - he->XYZ.z;
            s32 ox = bx - BearMidX, oz = bz - BearMidZ;
            s32 br = csqrt(ox * ox + oz * oz) >> 6;
            s32 tr = br - BEAR_STANDOFF;
            s32 full = PolarHeroMeter(0) >= PolarHeroAttackPower(0);

            if (full && bestd < BEAR_REACH) {
                return aim_at(dx, dz) | ((BotTicks & 3) == 0 ? BTN_SQUARE : 0);
            }
            if (!full && bestd < BEAR_CLOSE) {
                return aim_at(mx - dx, mz - dz);
            }
            if (tr > BEAR_STEP_OUT) {
                tr = BEAR_STEP_OUT;
            }
            if (tr > 0 && br > 0) {
                while (tr > 0 && near_edge(BearMidX + ox * tr / br, BearMidZ + oz * tr / br)) {
                    tr -= 0x80;
                }
            }
            if (tr > 0 && br > 0) {
                s32 tx = BearMidX + ox * tr / br - he->XYZ.x, tz = BearMidZ + oz * tr / br - he->XYZ.z;

                return abs(tx) + abs(tz) > 0x80 ? aim_at(tx, tz) : 0;
            }
        }
        return abs(mx) + abs(mz) > 0x100 ? aim_at(mx, mz) : 0;
    }
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
