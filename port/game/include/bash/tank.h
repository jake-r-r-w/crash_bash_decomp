#ifndef BASH_TANK_H
#define BASH_TANK_H

/* Tank (overlay tank): declarations recovered from the original headers
 * (GameEng/Tank: the leak starts part-way through it). The enums and structs
 * before the prototypes come from the Oct 9 2000 prototype's debug types,
 * which the retail AST table confirms for TANK_AST_types.
 * Names and types only; see docs/ARCHITECTURE.md (Leaked source, Prototype
 * debug types). */

#include "bash/nas.h"

enum TANK_AST_types {
    TANK_AST_zero = 0x1300,
    AST_Shell,
    AST_RocketTA,
    AST_Mine,
    AST_ShrapnelBit,
    AST_MineBall,
    AST_ShellExplosion,
    AST_HelicopterTA,
    AST_PickupTA,
    AST_PickupGenerator_TA,
    AST_Rock_TA,
    AST_Lobber_TA,
    AST_LaserBitTA,
    AST_FlamerBitTA,
    AST_FlubberTA,
    AST_SwirlyTA,
    AST_PillarControl,
    AST_Pillar,
    AST_PillarCover,
    AST_TankBoss,
    AST_BigApple,
    AST_TankBossCrashTurret,
    AST_TankBossCortexTurret,
    AST_TankBossRocketTurret,
    AST_Periscope,
    AST_PeriscopeGun,
    AST_TotemPole,
    AST_TotemTop,
    AST_TotemBot,
    AST_WaterMine,
    AST_Moe,
    AST_Joe,
    AST_BoatWake,
    AST_BoatField,
    AST_Torpedo
};

/* Tank's pickup types. */
enum PU_TA_Names {
    TA_BOMB_CHUCKER,
    TA_CANNON,
    TA_ROCKET_LAUNCHER,
    TA_BOMB_LOBBER,
    TA_TANK_FUEL,
    TA_TANK_MINE,
    TA_TANK_LIVE_MINE,
    TA_TANK_MINE_AND_LOBBER,
    TA_NUM_PUS
};

enum TA_BASIC_WEAPONS {
    TA_FLAMER = 8,
    TA_SWIRLY,
    TA_FLUBBER,
    TA_LASER
};

/* A Tank player's state (TA_players[4]). */
typedef struct TA_player {
    /* 0x00 */ s32 Energy;
    /* 0x04 */ s32 FaceDir;
    /* 0x08 */ s32 MoveDir;
    /* 0x0C */ s32 MainWeaponType;
    /* 0x10 */ s32 MainWeaponRounds;
    /* 0x14 */ s32 MainWeaponFireDelay;
    /* 0x18 */ NAS *MainWeaponNas[3];
    /* 0x24 */ s32 SubWeaponType;
    /* 0x28 */ s32 SubWeaponRounds;
    /* 0x2C */ s32 SubWeaponFireDelay;
    /* 0x30 */ s32 PosX;
    /* 0x34 */ s32 PosY;
    /* 0x38 */ s32 PosZ;
    /* 0x3C */ s32 HullSFX;
    /* 0x40 */ s32 TurretSFX;
    /* 0x44 */ s32 TurretSFXDuration;
    /* 0x48 */ s32 FireBreatherSFX;
    /* 0x4C */ s32 FireBreatherSFXDuration;
    /* 0x50 */ s32 ModeCounter;
    /* 0x54 */ s32 ModeSpeed;
    /* 0x58 */ _sfx_handler *HulFSXHandler;
    /* 0x5C */ _sfx_handler *TurretSFXHandler;
    /* 0x60 */ _sfx_handler *FireBreatherSFXHandler;
    /* 0x64 */ s32 TurretReqdDir;
    /* 0x68 */ s32 Speed;
    /* 0x6C */ s32 SpeedX;
    /* 0x70 */ s32 SpeedY;
    /* 0x74 */ s32 SpeedZ;
    /* 0x78 */ s32 HangaboutTime;
    /* 0x7C */ s32 IdleAnimCt;
    /* 0x80 */ s32 TurretFiring;
    /* 0x84 */ s32 *TurretAnimPtr;
    /* 0x88 */ s32 TauntDelay;
    /* 0x8C */ s32 RunnerSpeed;
    /* 0x90 */ s32 RunnerFacing;
    /* 0x94 */ s32 RunnerMoving;
    /* 0x98 */ s32 BankAngle;
    /* 0x9C */ s32 ForceFieldDelay;
    /* 0xA0 */ s32 RequestQuickFinish;
} TA_player;
BASH_SIZE_CHECK(TA_player, 0xA4);

/* A computer-controlled Tank player's state. */
typedef struct TA_Bot {
    /* 0x00 */ s32 Doing;
    /* 0x04 */ s32 DoingTimer;
    /* 0x08 */ s32 Victim;
    /* 0x0C */ NAS *pMine;
    /* 0x10 */ NAS *pMissile;
    /* 0x14 */ s32 nMine;
    /* 0x18 */ s32 nMissile;
    /* 0x1C */ u32 AIpad;
    /* 0x20 */ s32 TargetX;
    /* 0x24 */ s32 TargetZ;
    /* 0x28 */ u16 FirePercentChance;
    /* 0x2A */ u16 pad;
} TA_Bot;
BASH_SIZE_CHECK(TA_Bot, 0x2C);

/* The extension of a Tank object (shells, mines, pickups, ...). */
typedef struct EXTobject_TA {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ s32 Speed;
    /* 0x10 */ s32 SpeedX;
    /* 0x14 */ s32 SpeedY;
    /* 0x18 */ s32 SpeedZ;
    /* 0x1C */ s32 TargetIndex;
    /* 0x20 */ s32 TargetX;
    /* 0x24 */ s32 TargetZ;
    /* 0x28 */ s32 Counter;
    /* 0x2C */ s32 Owner;
    /* 0x30 */ s32 Status;
    /* 0x34 */ s32 *pAnimList;
} EXTobject_TA;
BASH_SIZE_CHECK(EXTobject_TA, 0x38);

void TA_Triggers(NASTrigger *, s32);
void TA_EarliestTriggers();
void TA_InitHero(u32 player);
void TA_MoveHero(u32 player);
void TA_PreMoveHero(u32 player);
void TA_CreateShadow(u32 player);
void AnimateTank(u32 player);
u32 GetSandbagValueTank(s32 PosX, s32 PosZ);
u32 GetSandbagValueMiss(s32 PosX, s32 PosZ);
void SetTankHeroIdle(u32 player);
void TankHeroIdle(u32 player);
void SetTankHeroMove(u32 player);
void TankHeroMove(u32 player);
void SetTankHeroTurn(u32 player);
void TankHeroTurn(u32 player);
void SetTankHeroRecoil(u32 player);
void TankHeroRecoil(u32 player);
void SetTankHeroVictory(u32 player);
void TankHeroVictory(u32 player);
void SetTankHeroDie(u32 player);
void TankHeroDie(u32 player);
void SetTankHeroDead(u32 player);
void TankHeroDead(u32 player);
void SetTankHeroMinelay(u32 player);
void TankHeroMinelay(u32 player);
void SetHullSFX(s32 player, EXTengine *pEngine, s32 ReqdSFX);
void SetTurretSFX(s32 player, EXTengine *pEngine, s32 ReqdSFX);
void SetFireBreatherSFX(s32 player, EXTengine *pEngine, s32 ReqdSFX);
bool TA_KeepInBounds(EXTengine *eng);
void TA_HeroBot(u32 player);
void InitShell(NAS *);
void PlotShell(NAS *);
void InitRocketTA(NAS *);
void PlotRocketTA(NAS *);
void InitMine(NAS *);
void PlotMine(NAS *);
void InitWaterMine(NAS *);
void PlotWaterMine(NAS *);
void InitShrapnelBit(NAS *);
void PlotShrapnelBit(NAS *);
void InitMineBall(NAS *);
void PlotMineBall(NAS *);
void InitShellExplosion(NAS *);
void PlotShellExplosion(NAS *);
void InitPickupTA(NAS *);
void PlotPickupTA(NAS *);
void InitHelicopterTA(NAS *);
void PlotHelicopterTA(NAS *);
void InitPickupGenerator_TA(NAS *);
void PlotPickupGenerator_TA(NAS *);
void InitRock_TA(NAS *);
void PlotRock_TA(NAS *);
void InitLobber_TA(NAS *);
void PlotLobber_TA(NAS *);
void InitLaserBitTA(NAS *);
void PlotLaserBitTA(NAS *);
void InitFlamerBitTA(NAS *);
void PlotFlamerBitTA(NAS *);
void InitFlubberTA(NAS *);
void PlotFlubberTA(NAS *);
void InitSwirlyTA(NAS *);
void PlotSwirlyTA(NAS *);
void InitPillar(NAS *);
void PlotPillar(NAS *);
void InitPillarCover(NAS *);
void PlotPillarCover(NAS *);
void InitBigApple(NAS *);
void PlotBigApple(NAS *);
void InitPillarControl(NAS *);
void PlotPillarControl(NAS *);
void InitPeriscope(NAS *);
void PlotPeriscope(NAS *);
void InitPeriscopeGun(NAS *);
void PlotPeriscopeGun(NAS *);
void InitTotemPole(NAS *);
void PlotTotemPole(NAS *);
void InitTotemBot(NAS *);
void PlotTotemBot(NAS *);
void InitTotemTop(NAS *);
void PlotTotemTop(NAS *);
void CreateMineShrapnelBits(NAS *);
void ResetToBasicWeapon(s32 player);
void BumpSpeedsForSwamp(EXTobject_TA *pObject);
bool MissileCollideWithPickup(EXTengine *pEngine);
void InitTankBoss(NAS *);
void PlotTankBoss(NAS *);
void InitMoe(NAS *);
void PlotMoe(NAS *);
void InitJoe(NAS *);
void PlotJoe(NAS *);
void InitBoatWake(NAS *);
void PlotBoatWake(NAS *);
void InitTorpedo(NAS *);
void PlotTorpedo(NAS *);
void InitBoatField(NAS *);
void PlotBoatField(NAS *);
void InitTankBossCrashTurret(NAS *);
void PlotTankBossCrashTurret(NAS *);
void InitTankBossCortexTurret(NAS *);
void PlotTankBossCortexTurret(NAS *);
void InitTankBossRocketTurret(NAS *);
void PlotTankBossRocketTurret(NAS *);
void CheckMineToBoss(s32 PosX, s32 PosY, s32);
void SetMoeJoeMode(s32 player, s32 mode);
extern u8 TankLevelsGridMapTank[9 *8];
extern u8 TankLevelsGridMapMiss[9 *8];
extern PreMap TANK_PREMAP;
extern LevSpec TANKSpecifics;
extern shell_setup tank_shell_info;
extern NAS *PU_TA_list[3];
extern TA_player TA_players[4];

extern file_group_load_info file_group_TANKLAST;
extern file_group_load_info file_group_TANKDESERT;
extern file_group_load_info file_group_TANKJUNGLE;
extern file_group_load_info file_group_TANKMETAL;
extern file_group_load_info file_group_TANKBOSS;
#ifdef BASH_HAVE_HERO_SEQ
extern hero_seq CRA_seq_tankrecoil[];
extern hero_seq CRA_seq_tankdie[];
extern hero_seq CRA_seq_tankidle[];
extern hero_seq CRA_seq_tankidle1[];
extern hero_seq CRA_seq_tankminelay[];
extern hero_seq CRA_seq_tankvictory[];
extern hero_seq CRA_seq_tankfire1[];
extern hero_seq CRA_seq_tankfire2[];
extern hero_seq CRA_seq_tankfire3[];
extern hero_seq COC_seq_tankrecoil[];
extern hero_seq COC_seq_tankdie[];
extern hero_seq COC_seq_tankidle[];
extern hero_seq COC_seq_tankidle1[];
extern hero_seq COC_seq_tankminelay[];
extern hero_seq COC_seq_tankvictory[];
extern hero_seq COC_seq_tankfire1[];
extern hero_seq COC_seq_tankfire2[];
extern hero_seq COC_seq_tankfire3[];
extern hero_seq TIN_seq_tankrecoil[];
extern hero_seq TIN_seq_tankdie[];
extern hero_seq TIN_seq_tankidle[];
extern hero_seq TIN_seq_tankidle1[];
extern hero_seq TIN_seq_tankminelay[];
extern hero_seq TIN_seq_tankvictory[];
extern hero_seq TIN_seq_tankfire1[];
extern hero_seq TIN_seq_tankfire2[];
extern hero_seq TIN_seq_tankfire3[];
extern hero_seq CTX_seq_tankrecoil[];
extern hero_seq CTX_seq_tankdie[];
extern hero_seq CTX_seq_tankidle[];
extern hero_seq CTX_seq_tankidle1[];
extern hero_seq CTX_seq_tankminelay[];
extern hero_seq CTX_seq_tankvictory[];
extern hero_seq CTX_seq_tankfire1[];
extern hero_seq CTX_seq_tankfire2[];
extern hero_seq CTX_seq_tankfire3[];
extern hero_seq BRO_seq_tankrecoil[];
extern hero_seq BRO_seq_tankdie[];
extern hero_seq BRO_seq_tankidle[];
extern hero_seq BRO_seq_tankidle1[];
extern hero_seq BRO_seq_tankminelay[];
extern hero_seq BRO_seq_tankvictory[];
extern hero_seq BRO_seq_tankfire1[];
extern hero_seq BRO_seq_tankfire2[];
extern hero_seq BRO_seq_tankfire3[];
extern hero_seq DNG_seq_tankrecoil[];
extern hero_seq DNG_seq_tankdie[];
extern hero_seq DNG_seq_tankidle[];
extern hero_seq DNG_seq_tankidle1[];
extern hero_seq DNG_seq_tankminelay[];
extern hero_seq DNG_seq_tankvictory[];
extern hero_seq DNG_seq_tankfire1[];
extern hero_seq DNG_seq_tankfire2[];
extern hero_seq DNG_seq_tankfire3[];
extern hero_seq KON_seq_tankrecoil[];
extern hero_seq KON_seq_tankdie[];
extern hero_seq KON_seq_tankidle[];
extern hero_seq KON_seq_tankidle1[];
extern hero_seq KON_seq_tankminelay[];
extern hero_seq KON_seq_tankvictory[];
extern hero_seq KON_seq_tankfire1[];
extern hero_seq KON_seq_tankfire2[];
extern hero_seq KON_seq_tankfire3[];
extern hero_seq KAN_seq_tankrecoil[];
extern hero_seq KAN_seq_tankdie[];
extern hero_seq KAN_seq_tankidle[];
extern hero_seq KAN_seq_tankidle1[];
extern hero_seq KAN_seq_tankminelay[];
extern hero_seq KAN_seq_tankvictory[];
extern hero_seq KAN_seq_tankfire1[];
extern hero_seq KAN_seq_tankfire2[];
extern hero_seq KAN_seq_tankfire3[];
#endif /* BASH_HAVE_HERO_SEQ */

#endif /* BASH_TANK_H */
