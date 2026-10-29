#ifndef BASH_TANK_OVL_H
#define BASH_TANK_OVL_H

/* Functions and data defined in the tank overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/tank/tank.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/shell.h"   /* Ambient_data */
#include "bash/tank.h"    /* TA_Bot, TA_player */
#include "bash/core.h"    /* EXTscriptdata */


extern s32 TankBossStatus;
extern NAS *TankBossCrashTurrets[3];

extern NAS *TankBossCortexTurrets[4];
extern NAS *TankBossRocketTurrets[2];
extern u32 MineBallFrames[];
extern s32 TankWaterBob[];
typedef struct { u16 v, pad; } RockTblEnt;
extern RockTblEnt RockFrames[];
extern RockTblEnt MineFrames[]; /* PlotMine: frame by countdown */
extern RockTblEnt MineFramesBig[]; /* PlotMine: frame by countdown, modes 8/9 */

extern s32 TankHeroSpeeds[]; /* TA_InitHero: tank speed by hero_name - 1 */
extern u16 TankMetalAiBotTweak[];
extern u16 TankMetalAiHumanTweak[];
extern u16 TankMetalAiHumanRoundTweak[];
extern u16 TankMetalAiSkillAlt[];
extern u16 TankMetalAiSkill[];
extern u16 TankMetalAiTeamSkill[];

void InitTotemBit(NAS *nas, s32 id);
void PlotTotemBit(NAS *nas, s32 id);

void MoeJoeDead(void);
void MoeJoeMinelay(void);
void TankEmptyStub(void);
void TA_RestartLevel(void);
extern s32 TankBossStartStatus;
extern s32 TankRocketAngles[];
extern s32 TankSwampSwirlyTurret[];
extern s32 TankSwampLaserTurret[];
extern s32 TankSwampFlubberTurret[];
extern s32 TankSwampFlamerTurret[];
extern s32 MoeState;
extern s32 JoeState;
extern s16 TankJoeTeammate;
extern s16 TankMoeTeammate;
extern u16 TankHeroModels[];
extern s32 TankVictorySeqs[];   /* {name, 0x5C} pairs: four "WIN" entries */
extern u16 TankDesertAiBotTweak[];
extern u16 TankDesertAiHumanTweak[];
extern u16 TankDesertAiHumanRoundTweak[];
extern u16 TankDesertAiSkillAlt[];
extern u16 TankDesertAiSkill[];
extern u16 TankDesertAiTeamSkill[];
void TA_PreInitLevel(void);
void TA_InitLevel(void);
void TA_UpdateLevel(void);
void FinishTankLevel(void);
void ClearTankMapCell(s32 arg0, s32 arg1);
void SetTankMapCell(s32 arg0, s32 arg1);
void TankBotReset(s32 arg0);
NAS *FindTankPickupNear(s32 x, s32 z, s32 range);


extern NAS *TotemPole;
extern NAS *TotemTop;
extern NAS *TotemBot;

extern s32 TotemTargets[][2];

extern s32 TotemAnim0D[];
extern s32 TotemAnim29[];
extern s32 TotemAnim25[];
extern s32 TotemAnim27[];
extern s32 TotemAnim26[];


extern s32 TankBotTargetOrder[];
extern s32 TankMapNas[];
extern RockTblEnt ShrapnelFrames[];
extern s32 TankMapWidth;
extern TA_Bot TankBotAI[4];

extern subengine *TankJoeSub;  /* Heroes[2].hero_sub, by a label of its own: Heroes[2].hero_sub as a field shares a lui with Heroes[3] and does not match */
extern subengine *TankMoeSub;  /* Heroes[3].hero_sub */

typedef struct MineOffset {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 z;
    /* 0x08 */ s32 unk8;
} MineOffset;
extern MineOffset MineShrapnelOffsets[4];
void SetMoeJoeDead(s32 hero, s32 last_mode);
extern s32 *MoeAnimPtr;
extern s32 *JoeAnimPtr;
extern s32 MoeMoveAnim[];
extern s32 JoeMoveAnim[];
extern s32 MoeDieAnim[];
extern s32 JoeDieAnim[];
extern s32 MoeRecoilAnim[];
extern s32 JoeRecoilAnim[];
extern s32 TankShotBounce;







s32 MoveTankShot(NAS *nas, s32 arg1, s32 arg2, s32 arg3);





typedef struct {
    s32 x;
    s32 z;  /* negative: unused */
} TankPoint;

extern TankPoint TankBossTargets[];
extern s32 TankLevelOver;
extern s32 TankBossSightedHero;

/* TankBossTurretFindTarget: can the turret at (x, z) facing ang see target i? Records state. */
static inline s32 TankTurretSees(s32 i, s32 state, s32 x, s32 z, s32 ang) {
    if (TankLevelOver != 0) {
        return 0;
    }
    if (TankBossTargets[i].z < 0) {
        return 0;
    }
    if ((u32)(((ang - get_angle(x - TankBossTargets[i].x, z - TankBossTargets[i].z)) & 0xFFF) - 0x15) < 0xFD7U) {
        return 0;
    }
    TankBossSightedHero = state;
    return 1;
}

typedef struct {
    u16 v, pad;
} TankU16Pad;
extern TankU16Pad PillarCoverFrames[];
extern u16 CrashTurretFrames[]; /* crash turret anim frames (flags == 0) */
extern u16 CrashTurretFramesAlt[]; /* crash turret anim frames (flags != 0) */
typedef void (*TankTurretFireFn)(EXTengine *eng, s32 a, s32 b);
extern s32 TankTurnSfx[];
extern s32 TankRecoilSfx[][4];
/* TankBotPlanSteerMove's fallback roam points: {x, z}. */
extern s32 TankBotWanderPoints[][2];
extern TankU16Pad HelicopterFrames[];    /* PlotHelicopterTA's rotor frames, by UserInfo.user2 */
/* PlotPillarControl's waves: delay, then {x, z, angle} until x < 0. */
extern s32 *PillarWaves[];
extern s32 PillarWave;      /* next wave */
extern NAS *TankObjects[8];    /* objects AddTankObject adds and RemoveTankObject removes; FindTankObjectNear tests their distance */
extern NAS *TankMines[8];

extern NAS *TankBossNas;       /* InitTankBoss's NAS */
extern EXTengine *TankBossEng; /* and its engine */
extern s32 TankBossRocketDelay;
s32 TankBossTurretFindTarget(EXTengine *eng);
void TankBossCortexTurretFire(EXTengine *eng);
void TankBossRocketTurretFire(NAS *nas, EXTengine *eng);
/* Field 0xC of the 0x38-byte model record N at HeroModelBank (record 57 is DB_ArenaBossKomodoArenaobjects). */
#define TANK_MODEL(n) ((u8 *)&HeroModelBank[n])
#define TANK_MODEL_ON(b, n) (*(s32 *)((b) + (n) * 0x38 + 0xC))
extern s32 TankBossBob[16]; /* PlotTankBoss: per-frame height wobble */









void MoeJoeDie(s32 arg0);

void StartMoeJoeAnim(s32 arg0, s32 *arg1);

s32 TankBossTurretCheckSight(s32 id, s32 x, s32 z, s32 angle);



s32 TankBlockPadNear(s32 flags, NAS *nas, EXTengine *eng);

void TankHeroTurnClockwise(s32 player, TA_player *ta, s32 dir);

void TankHeroTurnAnticlockwise(s32 player, TA_player *ta, s32 dir);



void DamageTankHeroesNear(s32 x, s32 z, s32 range, s32 arg3);

void TriggerTankMinesNear(s32 x, s32 z, s32 dist);

NAS *FindTankObjectNear(s32 x, s32 z, s32 dist);

void RemoveTankObject(NAS *nas);

void AddTankObject(NAS *nas);

void ClearTankObjects(void);

s32 GetTankMapCell(s32 arg0, s32 arg1);

void SetTankMapCellNas(NAS *nas);

void TankBotChooseTarget(void);

extern u16 TankJungleAiBotTweak[];
extern u16 TankJungleAiHumanTweak[];
extern u16 TankJungleAiHumanRoundTweak[];
extern u16 TankJungleAiSkillAlt[];
extern u16 TankJungleAiSkill[];
extern u16 TankJungleAiTeamSkill[];

extern u16 TankSwampAiBotTweak[];
extern u16 TankSwampAiHumanTweak[];
extern u16 TankSwampAiHumanRoundTweak[];
extern u16 TankSwampAiSkillAlt[];
extern u16 TankSwampAiSkill[];
extern u16 TankSwampAiTeamSkill[];
extern u16 TankBossAiBotTweak[];
extern u16 TankBossAiHumanTweak[];
extern u16 TankBossAiHumanRoundTweak[];
extern u16 TankBossAiSkillAlt[];
extern u16 TankBossAiSkill[];

extern s32 TankTauntSfx[];
extern s32 TankHullSfx[]; /* TA_MoveHero: hull sfx while moving, by player */
extern s32 TankTurretSfx[]; /* turret sfx, by player */
extern s32 TankHoverSfx[]; /* hull sfx in mode 5, by player */
extern s32 TankDropDelay;    /* delay before the next hangabout spawn */
extern s32 TankCountdown;    /* TA_UpdateLevel: countdown, plays sfx 0x4B9 when negative */
extern s32 TankPendingSfx;    /* TA_UpdateLevel: sfx id to play once */
extern EXTengine *TankCountdownEng;
extern s32 TankPillarSfxDelay;    /* TA_UpdateLevel: countdown to sfx 0x501 */
void AnimateTankTurret(s32 player);
/* UpdateTankHeroWeapons: turret anim scripts by weapon (swamp / other levels). */
extern s32 TankSwirlyFireAnim[];
extern s32 TankLaserFireAnim[];
extern s32 TankFlubberFireAnim[];
extern s32 TankFlamerFireAnim[];
extern s32 TankShellFireAnim[];
extern s32 TankRocketFireAnim[];
extern s32 TankSwampSwirlyFireAnim[];
extern s32 TankSwampLaserFireAnim[];
extern s32 TankSwampFlubberFireAnim[];
extern s32 TankSwampFlamerFireAnim[];
extern s32 TankForceFieldSfx[]; /* force field sfx, by player */
extern s32 TankMineSfx[]; /* sub weapon sfx, by player */
/* TriggerTankMinesNear, inlined into UpdateTankHeroWeapons: sets status 1 on the TankMines
 * mines within dist of (x, z). */
static inline void TankTriggerNear(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = TankMines[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->XYZ.x;
            if (d >= -dist && d <= dist) {
                d = z - eng->XYZ.z;
                if (d >= -dist && d <= dist) {
                    nas->UserInfo.status = 1;
                }
            }
        }
    }
}


extern s32 TankArenaMaxX;
extern s32 TankArenaMaxZ;











void TotemBitExplode(EXTengine *arg0);

void SpawnTankBossTurrets(void);

void InitMoeJoe(NAS *nas, s32 id);

void LoopMoeJoeMoveAnim(s32 arg0);

void SetMoeJoeIdle(s32 arg0);

void SetMoeJoeMove(s32 arg0);

void MoeJoeIdle(s32 arg0);


s32 StepMoeJoeAnim(s32 arg0);

s32 TankBossPickHero(void);

s32 TankWithinBox(EXTengine *eng, s32 x, s32 y, s32 z);

void KillPickupTA(NAS *nas);

void KillTankMainWeaponNas(NAS *nas, s32 player);



s32 CheckTankHeroWon(s32 arg0);



void TA_StopHeroSfx(void);

void TA_SetUp(void);

void SetPuffAngle(NAS *puff, s16 angle);

s32 TankFindHeroNear(s32 x, s32 z, s32 r, s32 skip);

s32 RocketFindHero(s32 arg0, s32 arg1, s32 arg2);

void KillTankMapCell(s32 arg0, s32 arg1);

void TankBotAimTurret(s32 player, EXTengine *eng, s32 *arg2);

s32 TankBotPickTarget();

/* AI target state TankBotAvoidMines fills (target NAS, scan cursor). */
extern s32 TankBotForceField[];
s32 TankBotTryFire(s32 player, EXTengine *eng, TA_Bot *ai);
void TankBotPlanSteerMove(s32 player, EXTengine *eng, TA_Bot *ai);
void TankBotPlanGridMove(s32 player, EXTengine *eng, TA_Bot *ai);
static inline s32 TankASTIndex(s32 id) {
    s32 i;

    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == id) {
            return i;
        }
    }
    return 0;
}
/* PlotRocketTA: FindTankObjectNear inlined. */
static inline NAS *TankRocketNear(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = TankObjects[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->XYZ.x;
            if (d >= -dist && d <= dist) {
                d = z - eng->XYZ.z;
                if (d >= -dist && d <= dist) {
                    return nas;
                }
            }
        }
    }
    return NULL;
}
/* PlotRocketTA: RocketFindHero inlined. */
static inline s32 TankRocketFindHero(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 d;
    u16 name;
    EXTengine *e;

    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((GameState *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = arg0 - e->XYZ.x;
        if (d < -arg2) {
            continue;
        }
        if (arg2 < d) {
            continue;
        }
        d = arg1 - e->XYZ.z;
        if (-arg2 < d && d < arg2) {
            return i;
        }
    }
    return -1;
}
/* PlotRocketTA: KillTankMapCell inlined. */
static inline void TankRocketKillCell(s32 arg0, s32 arg1) {
    s32 idx;
    NAS *nas;

    idx = ((arg1 >> 9) * TankMapWidth) + (arg0 >> 9);
    nas = (NAS *)TankMapNas[idx];
    TankMapNas[idx] = 0;
    TankLevelsGridMapMiss[idx] = 0;
    TankLevelsGridMapTank[idx] = 0;
    KillNAS(nas);
}
/* PlotRocketTA: the TankLevelsGridMapTank tile under (x, z), 0 off the map in mode 9. */
static inline s32 TankRocketTile(s32 x, s32 z) {
    if (GameMode == 9) {
        if (z >= TankArenaMaxZ || z < 0 || x >= TankArenaMaxX || x < 0) {
            return 0;
        }
    }
    return *(((z >> 9) * TankMapWidth) + (x >> 9) + TankLevelsGridMapTank);
}
/* MoeJoeMove: the TankLevelsGridMapMiss cell under (x, z). */
static inline s32 TankMapCell(s32 x, s32 z) {
    return *(((z >> 9) * TankMapWidth) + (x >> 9) + TankLevelsGridMapMiss);
}
/* TankHeroMove: the other hero within lim of (x, z), or -1. */
static inline s32 TankMoveFindHero(s32 player, s32 x, s32 z, s32 lim) {
    s32 i;
    s32 d;
    s32 n;
    u16 name;
    HeroInfo *h;
    EXTengine *e;

    for (i = 0; i < 4; i++) {
        if (i == player) {
            continue;
        }
        h = &Heroes[i];
        name = h->hero_name;
        if (name == 9 &&
            (((GameState *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (h->hero_status & 0xC000) {
            continue;
        }
        e = h->hero_eng;
        d = x - e->XYZ.x;
        if (d < -lim) {
            continue;
        }
        if (lim < d) {
            continue;
        }
        d = z - e->XYZ.z;
        n = -lim;
        if (n < d && d < lim) {
            return i;
        }
    }
    return -1;
}
static inline s32 TankNear(s32 a, s32 b) {
    s32 d;

    d = a - b;
    return d >= -999 && d <= 999;
}
static inline void TankSpawnBit1605(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    EXTscriptdata *ext;

    p = &HeroModelBank[DB_TankObjects];
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->scriptnumber = kind;
            ext->speed = 0x100;
            if (p != NULL) {
                ext->flags = 5;
                nas->Engine->sub->u6C.dbase = p;
            } else {
                ext->flags = 1;
            }
            ext->maxloops = 0;
            nas->InitDelete(nas);
        }
    }
}
static inline NAS *TankSpawnBit1605Nas(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    EXTscriptdata *ext;

    p = &HeroModelBank[DB_TankObjects];
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->scriptnumber = kind;
            ext->speed = 0x100;
            if (p != NULL) {
                ext->flags = 5;
                nas->Engine->sub->u6C.dbase = p;
            } else {
                ext->flags = 1;
            }
            ext->maxloops = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}
/* CheckMineToBoss's puff: TankSpawnBit1605 with model record DB_ArenaBossKomodoArenaobjects. */
static inline void TankSpawnBit1605B(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    EXTscriptdata *ext;

    p = &HeroModelBank[DB_ArenaBossKomodoArenaobjects];
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->scriptnumber = kind;
            ext->speed = 0x100;
            if (p != NULL) {
                ext->flags = 5;
                nas->Engine->sub->u6C.dbase = p;
            } else {
                ext->flags = 1;
            }
            ext->maxloops = 0;
            nas->InitDelete(nas);
        }
    }
}

/* CheckMineToBoss: is e within rx/ry/rz of (x, y, z)? One early return per
   bound: with one || test per axis, the callers' loops are a few insns smaller
   at loop.c time and their table index gets strength-reduced. */
static inline s32 TankWithin(EXTengine *e, s32 x, s32 y, s32 z, s32 rx, s32 ry, s32 rz) {
    s32 dx;
    s32 dy;
    s32 dz;

    dx = e->XYZ.x - x;
    if (dx >= rx + 1) {
        return 0;
    }
    if (dx < -rx) {
        return 0;
    }
    dy = e->XYZ.y - y;
    if (dy >= ry + 1) {
        return 0;
    }
    if (dy < -ry) {
        return 0;
    }
    dz = e->XYZ.z - z;
    if (dz >= rz + 1) {
        return 0;
    }
    return dz >= -rz;
}
/* PlotPeriscope: a 0x1605 puff with the scene's first model. */
static inline NAS *TankPeriscopePuff(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    EXTscriptdata *ext;

    p = ((u8 *)MapScene->dbase);
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->scriptnumber = kind;
            ext->speed = 0x100;
            if (p != NULL) {
                ext->flags = 5;
                nas->Engine->sub->u6C.dbase = p;
            } else {
                ext->flags = 1;
            }
            ext->maxloops = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}
extern NAS *PeriscopePuff;  /* PlotPeriscope's last puff */
extern s32 PeriscopeFrames[]; /* PlotPeriscope: frame by timer in state 10, 0 ends */
/* TankBossRocketTurretFire: per-shell angle offsets. */
static inline s32 *TankShellAngleOffsets(void) {
    return TankRocketAngles;
}
/* TankBossRocketTurretFire: face a spawned puff along `angle` and flag its subengines. */
static inline void TankPuffSetAngle(NAS *puff, s32 angle) {
    subengine *s;

    puff->Engine->xform.rot.vy = angle;
    puff->Engine->xform.dirty = 1;
    for (s = puff->Engine->sub; s != NULL; s = s->next) {
        s->flags |= 0x20000000;
    }
}
void TankBotDodgeFrom(s32 player, s32 x, s32 z, EXTengine *eng, TA_Bot *ai);
s32 TankBotAvoidMines(s32 player, EXTengine *eng, TA_Bot *ai);

void TankBotSteer(s32 player);

void SetMoeJoeRecoil(s32 arg0);

void MoeJoeRecoil(s32 arg0);

extern s32 HelicopterCount;
extern s32 HelicopterZSwamp[4];
extern s32 HelicopterZDesert[4];
extern s32 HelicopterZJungle[4];
extern s32 HelicopterZMetal[4];
extern s32 HelicopterZBoss[4];
extern s32 PickupSpots[]; /* (x, z) pairs */
/* PlotPickupGenerator_TA: hero within 0x200 of (x, z). */
static inline s32 PickupFindHero(s32 x, s32 z) {
    s32 i;
    s32 d;
    s32 r;
    s32 n;
    u16 name;
    EXTengine *e;

    r = 0x200;
    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((GameState *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = x - e->XYZ.x;
        if (d < -0x200) {
            continue;
        }
        if (r < d) {
            continue;
        }
        d = z - e->XYZ.z;
        n = -0x200;
        if (n < d && d < 0x200) {
            return i;
        }
    }
    return -1;
}

void DamageTankHero(s32 player, s32 damage);


void TankBossCrashTurretFire(EXTengine *arg0);

void AnimateMoeJoeTurret(s32 player);

void MoeJoeBecomeHero(NAS *nas, s32 id);

/* SetHullSFX inlined by PlotMoeJoeHero. */
#define TANK_SET_HULL_SFX(player, pEngine, ReqdSFX)                        \
    {                                                                       \
        s32 sfx_ = (ReqdSFX);                                               \
        TA_player *tb_ = TA_players;                                        \
        TA_player *ta_ = &tb_[player];                                      \
        if (ta_->HullSFX != sfx_) {                                         \
            ta_->HullSFX = sfx_;                                            \
            if (sfx_ != 0) {                                                \
                PlaySfx(sfx_, &(pEngine)->sfx, &(pEngine)->XYZ, 0x1000,   \
                        0x1E00, 0);                                         \
            } else if ((pEngine)->sfx != NULL) {                            \
                StopSfx((pEngine)->sfx);                                    \
            }                                                               \
        }                                                                   \
    }

/* SetTurretSFX inlined by PlotMoeJoeHero. */
#define TANK_SET_TURRET_SFX(player, ReqdSFX)                               \
    {                                                                       \
        s32 sfx_ = (ReqdSFX);                                               \
        TA_player *tb_ = TA_players;                                        \
        TA_player *ta_ = &tb_[player];                                      \
        if (ta_->TurretSFX != sfx_) {                                       \
            ta_->TurretSFX = sfx_;                                          \
            if (sfx_ == 0) {                                                \
                StopSfx(ta_->TurretSFXHandler);                             \
            } else {                                                        \
                PlaySfx(sfx_, &TA_players[player].TurretSFXHandler, NULL,   \
                        0x1000, 0x1E00, 0);                                 \
            }                                                               \
        }                                                                   \
    }

void TankHeroSteerMove(s32 player);

void TankHeroGridMove(s32 player);

void MoeJoeMove(s32 player);

void PlotMoeJoeHero(NAS *nas, s32 id);

#define CAM5F (&MainCamera)
typedef struct TankScreen {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 w10;
    /* 0x14 */ s32 w14;
} TankScreen;
extern s32 TankUnused0;
extern s32 TankUnused1;
extern s32 TankUnused2;
extern s32 TankUnused3;
extern s32 TankUnused4;
extern s32 TankMapHeight;
extern s32 TankUnusedGrid[9][7];
extern Ambient_data TankJungleAmbience;
extern u8 TankDesertMap[];
extern u8 TankDesertMapAlt[];
extern u8 TankJungleMap[];
extern u8 TankMetalMap[];
extern u8 TankBossMap[];
extern u8 TankSwampMap[];


/* A 0x38-byte model record of HeroModelBank (see TANK_MODEL). */
typedef struct ModelRec {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 on;
    /* 0x10 */ u8 unk10[0x38 - 0x10];
} ModelRec;

/* PlotMoeJoe: anim scripts, Moe (0x131E) and Joe */
extern s32 MoeScript0[];
extern s32 MoeScript1[];
extern s32 MoeScript2[];
extern s32 MoeScript3[];
extern s32 MoeScript4[];
extern s32 MoeScript5[];
extern s32 MoeScript6[];
extern s32 MoeScript7[];
extern s32 JoeScript0[];
extern s32 JoeScript1[];
extern s32 JoeScript2[];
extern s32 JoeScript3[];
extern s32 JoeScript4[];
extern s32 JoeScript5[];
extern s32 JoeScript6[];
extern s32 JoeScript7[];

void UpdateTankHeroWeapons(s32 player);

void PlotMoeJoe(NAS *nas, s32 id);


#endif /* BASH_TANK_OVL_H */
