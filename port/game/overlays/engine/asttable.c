/* The engine's AST table (EngineASTTable, 0x80098720): every object type's id and its
 * Init/Plot, which CreateNAS and the triggers look up. Most entries point into the slot
 * overlays, which share one address range: the engine's link takes those functions'
 * addresses from the overlays' own links (tools/scripts/slot_syms.py), so an overlay
 * that moves takes its entries with it. The ids are the Oct 9 prototype's AST enums. */
#include "common.h"
#include "bash/nas.h"
#include "bash/nascalls.h"
#include "bash/arkenoid.h"
#include "bash/bash.h"
#include "bash/bonk.h"
#include "bash/chase.h"
#include "bash/dino.h"
#include "bash/hero.h"
#include "bash/hub.h"
#include "bash/n2o.h"
#include "bash/papu.h"
#include "bash/pogo.h"
#include "bash/polar.h"
#include "bash/race.h"
#include "bash/spin.h"
#include "bash/tank.h"
#include "bash/titles.h"
#include "bash/xtras.h"

/* Init/Plot functions no header included here declares (by the overlay that defines them) */
void InitAnvilPickup(NAS *nas);  /* bash */
void InitBash8way(NAS *nas);  /* bash */
void InitBashHomer(NAS *nas);  /* bash */
void InitBashPenguin(NAS *nas);  /* bash */
void InitBashShox(NAS *nas);  /* bash */
void InitBoing(NAS *nas);  /* bash */
void InitBootsPickup(NAS *nas);  /* bash */
void InitCratePickup(NAS *nas);  /* bash */
void InitMissileExplode(NAS *nas);  /* bash */
void InitShieldPickup(NAS *nas);  /* bash */
void InitSlowmoPickup(NAS *nas);  /* bash */
void InitSnowGlint(NAS *nas);  /* bash */
void InitTumbler(NAS *nas);  /* bash */
void PlotBash8way(NAS *nas);  /* bash */
void PlotBashHomer(NAS *nas);  /* bash */
void PlotBashPenguin(NAS *nas);  /* bash */
void PlotBashShox(NAS *nas);  /* bash */
void PlotBoing(NAS *nas);  /* bash */
void PlotMissileExplode(NAS *nas);  /* bash */
void PlotSnowGlint(NAS *nas);  /* bash */
void PlotTumbler(NAS *nas);  /* bash */
void InitCrate(NAS *nas);  /* bash */
void PlotCrate(NAS *nas);  /* bash */
void PlotPickup(NAS *nas);  /* bash */
void InitPapuBoss(NAS *nas);  /* papu */
void InitPapuColumn(NAS *nas);  /* papu */
void PlotPapuBoss(NAS *nas);  /* papu */
void PlotPapuColumn(NAS *nas);  /* papu */
void PlotPapuMinion(NAS *nas);  /* papu */
void PlotPACrate(NAS *nas);  /* papu */
void InitGravityObject(NAS *nas);  /* polar */
void PlotGravityObject(NAS *nas);  /* polar */
void InitBarrelRoll(NAS *nas);  /* race */
void InitDac(NAS *nas);  /* race */
void InitDinoEgg(NAS *nas);  /* race */
void InitGlob(NAS *nas);  /* race */
void InitRaceFire(NAS *nas);  /* race */
void InitRaceGreenSmokeBoost(NAS *nas);  /* race */
void InitRaceGreenSmokePoof(NAS *nas);  /* race */
void InitRaceMissile(NAS *nas);  /* race */
void InitRacePickUp(NAS *nas);  /* race */
void InitRaceShield(NAS *nas);  /* race */
void InitRaceSmokeBoost(NAS *nas);  /* race */
void InitRaceSmokeBoostWater(NAS *nas);  /* race */
void InitRaceSmokePoof(NAS *nas);  /* race */
void InitRaceTag(NAS *nas);  /* race */
void InitRaceVolcano(NAS *nas);  /* race */
void InitTNTCrate(NAS *nas);  /* race */
void InitTrackGlob(NAS *nas);  /* race */
void PlotBarrelRoll(NAS *nas);  /* race */
void PlotDac(NAS *nas);  /* race */
void PlotDinoEgg(NAS *nas);  /* race */
void PlotGlob(NAS *nas);  /* race */
void PlotRaceFire(NAS *nas);  /* race */
void PlotRaceGreenSmokeBoost(NAS *nas);  /* race */
void PlotRaceGreenSmokePoof(NAS *nas);  /* race */
void PlotRaceMissile(NAS *nas);  /* race */
void PlotRacePickUp(NAS *nas);  /* race */
void PlotRaceShield(NAS *nas);  /* race */
void PlotRaceSmokeBoost(NAS *nas);  /* race */
void PlotRaceSmokeBoostWater(NAS *nas);  /* race */
void PlotRaceSmokePoof(NAS *nas);  /* race */
void PlotRaceTag(NAS *nas);  /* race */
void PlotRaceVolcano(NAS *nas);  /* race */
void PlotTNTCrate(NAS *nas);  /* race */
void PlotTrackGlob(NAS *nas);  /* race */
void PlotBalloon(NAS *nas);  /* spin */
void PlotHazardType(NAS *nas);  /* spin */
void InitDinoGem(NAS *nas);  /* dino */
void InitDinoScratch(NAS *nas);  /* dino */
void InitDinoSmoke(NAS *nas);  /* dino */
void PlotDinoGem(NAS *nas);  /* dino */
void PlotDinoScratch(NAS *nas);  /* dino */
void PlotDinoSmoke(NAS *nas);  /* dino */
void InitBonkCrate(NAS *nas);  /* bonk */
void InitBonkFlag(NAS *nas);  /* bonk */
void InitBonkShockwave(NAS *nas);  /* bonk */
void InitBonkStool(NAS *nas);  /* bonk */
void PlotBonkCrate(NAS *nas);  /* bonk */
void PlotBonkFlag(NAS *nas);  /* bonk */
void PlotBonkShockwave(NAS *nas);  /* bonk */
void PlotBonkStool(NAS *nas);  /* bonk */
void InitAsteroidRoll(NAS *nas);  /* n2o */
void InitDummyCamera(NAS *nas);  /* n2o */
void InitLaserShoot(NAS *nas);  /* n2o */
void InitMineSFX(NAS *nas);  /* n2o */
void InitN2OBall(NAS *nas);  /* n2o */
void InitN2ODeadWall(NAS *nas);  /* n2o */
void InitN2Ofirstmissile(NAS *nas);  /* n2o */
void InitN2Omissile(NAS *nas);  /* n2o */
void InitN2OmissileExplode(NAS *nas);  /* n2o */
void InitNitroBoss(NAS *nas);  /* n2o */
void InitNitroMine(NAS *nas);  /* n2o */
void InitNitroMissile(NAS *nas);  /* n2o */
void InitNitroSmokeBoost(NAS *nas);  /* n2o */
void InitSpiralSat(NAS *nas);  /* n2o */
void PlotAsteroidRoll(NAS *nas);  /* n2o */
void PlotDummyCamera(NAS *nas);  /* n2o */
void PlotLaserShoot(NAS *nas);  /* n2o */
void PlotMineSFX(NAS *nas);  /* n2o */
void PlotN2OBall(NAS *nas);  /* n2o */
void PlotN2ODeadWall(NAS *nas);  /* n2o */
void PlotN2Ofirstmissile(NAS *nas);  /* n2o */
void PlotN2Omissile(NAS *nas);  /* n2o */
void PlotN2OmissileExplode(NAS *nas);  /* n2o */
void PlotNitroBoss(NAS *nas);  /* n2o */
void PlotNitroMine(NAS *nas);  /* n2o */
void PlotNitroMissile(NAS *nas);  /* n2o */
void PlotNitroSmokeBoost(NAS *nas);  /* n2o */
void PlotSpiralSat(NAS *nas);  /* n2o */
void InitBarrelCH(NAS *nas);  /* chase */
void InitBarrelGenerator(NAS *nas);  /* chase */
void InitCircleFire(NAS *nas);  /* chase */
void InitCoal(NAS *nas);  /* chase */
void InitCoalFlame(NAS *nas);  /* chase */
void InitCoalFlameSFX(NAS *nas);  /* chase */
void InitEgg(NAS *nas);  /* chase */
void InitPUGenerator_CH(NAS *nas);  /* chase */
void InitPickupCH(NAS *nas);  /* chase */
void PlotBarrelCH(NAS *nas);  /* chase */
void PlotBarrelGenerator(NAS *nas);  /* chase */
void PlotCircleFire(NAS *nas);  /* chase */
void PlotCoal(NAS *nas);  /* chase */
void PlotCoalFlame(NAS *nas);  /* chase */
void PlotCoalFlameSFX(NAS *nas);  /* chase */
void PlotEgg(NAS *nas);  /* chase */
void PlotPUGenerator_CH(NAS *nas);  /* chase */
void PlotPickupCH(NAS *nas);  /* chase */
void InitGatewayTrophy(NAS *nas);  /* hub */
void InitPodiumOdo(NAS *nas);  /* hub */
void InitPodiumTrophy(NAS *nas);  /* hub */
void PlotGatewayTrophy(NAS *nas);  /* hub */
void PlotPodiumOdo(NAS *nas);  /* hub */
void PlotPodiumTrophy(NAS *nas);  /* hub */
void InitBigPoof(NAS *nas);  /* the exe/engine */
void InitCrateExplode(NAS *nas);  /* the exe/engine */
void InitFloatyScore(NAS *nas);  /* the exe/engine */
void InitParticleEffect(NAS *nas);  /* the exe/engine */
void InitScriptFX(NAS *nas);  /* the exe/engine */
void Init_HealthPU(NAS *nas);  /* the exe/engine */
void PlotBigPoof(NAS *nas);  /* the exe/engine */
void PlotFloatyScore(NAS *nas);  /* the exe/engine */
void PlotParticleEffect(NAS *nas);  /* the exe/engine */
void PlotScriptFX(NAS *nas);  /* the exe/engine */
void Plot_HealthPU(NAS *nas);  /* the exe/engine */

ASTEntry EngineASTTable[174] = {
    { Null_AST, NullProc, NullProc },
    { HERO_AST_zero, NullProc, NullProc },
    { AST_Hero, NullProc, NullProc },
    { AST_hero2, NullProc, NullProc },
    { AST_SnowGlint, InitSnowGlint, PlotSnowGlint },
    { AST_BashPenguin, InitBashPenguin, PlotBashPenguin },
    { AST_Bash8way, InitBash8way, PlotBash8way },
    { AST_BashHomer, InitBashHomer, PlotBashHomer },
    { AST_BashShox, InitBashShox, PlotBashShox },
    { AST_MissileExplode, InitMissileExplode, PlotMissileExplode },
    { AST_Tumbler, InitTumbler, PlotTumbler },
    { AST_Boing, InitBoing, PlotBoing },
    { AST_PapuBoss, InitPapuBoss, PlotPapuBoss },
    { AST_PapuMinion, NullProc, PlotPapuMinion },
    { AST_PapuColumn, InitPapuColumn, PlotPapuColumn },
    { AST_Shell, InitShell, PlotShell },
    { AST_RocketTA, InitRocketTA, PlotRocketTA },
    { AST_Mine, InitMine, PlotMine },
    { AST_ShrapnelBit, InitShrapnelBit, PlotShrapnelBit },
    { AST_MineBall, InitMineBall, PlotMineBall },
    { AST_ShellExplosion, InitShellExplosion, PlotShellExplosion },
    { AST_HelicopterTA, InitHelicopterTA, PlotHelicopterTA },
    { AST_PickupTA, InitPickupTA, PlotPickupTA },
    { AST_PickupGenerator_TA, InitPickupGenerator_TA, PlotPickupGenerator_TA },
    { AST_Rock_TA, InitRock_TA, PlotRock_TA },
    { AST_Lobber_TA, InitLobber_TA, PlotLobber_TA },
    { AST_LaserBitTA, InitLaserBitTA, PlotLaserBitTA },
    { AST_FlamerBitTA, InitFlamerBitTA, PlotFlamerBitTA },
    { AST_FlubberTA, InitFlubberTA, PlotFlubberTA },
    { AST_SwirlyTA, InitSwirlyTA, PlotSwirlyTA },
    { AST_PillarControl, InitPillarControl, PlotPillarControl },
    { AST_Pillar, InitPillar, PlotPillar },
    { AST_PillarCover, InitPillarCover, PlotPillarCover },
    { AST_TankBoss, InitTankBoss, PlotTankBoss },
    { AST_BigApple, InitBigApple, PlotBigApple },
    { AST_TankBossCrashTurret, InitTankBossCrashTurret, PlotTankBossCrashTurret },
    { AST_TankBossCortexTurret, InitTankBossCortexTurret, PlotTankBossCortexTurret },
    { AST_TankBossRocketTurret, InitTankBossRocketTurret, PlotTankBossRocketTurret },
    { AST_Periscope, InitPeriscope, PlotPeriscope },
    { AST_PeriscopeGun, InitPeriscopeGun, PlotPeriscopeGun },
    { AST_TotemPole, InitTotemPole, PlotTotemPole },
    { AST_TotemTop, InitTotemTop, PlotTotemTop },
    { AST_TotemBot, InitTotemBot, PlotTotemBot },
    { AST_WaterMine, InitWaterMine, PlotWaterMine },
    { AST_Moe, InitMoe, PlotMoe },
    { AST_Joe, InitJoe, PlotJoe },
    { AST_BoatWake, InitBoatWake, PlotBoatWake },
    { AST_BoatField, InitBoatField, PlotBoatField },
    { AST_Torpedo, InitTorpedo, PlotTorpedo },
    { AST_TransmuterBeam, InitTransmuterBeam, PlotTransmuterBeam },
    { AST_WeightDeath, InitWeightDeath, PlotWeightDeath },
    { AST_ParticleEffect, InitParticleEffect, PlotParticleEffect },
    { AST_GravityObject, InitGravityObject, PlotGravityObject },
    { AST_ScriptFX, InitScriptFX, PlotScriptFX },
    { AST_ScratchMaster, InitScratchMaster, PlotScratchMaster },
    { AST_IceBerg, InitIceBerg, PlotIceBerg },
    { AST_SnowMaster, InitSnowMaster, PlotSnowMaster },
    { AST_BreathMaster, InitBreathMaster, PlotBreathMaster },
    { AST_UkaPolarMask, InitUkaPolarMask, PlotUkaPolarMask },
    { AST_CameraNull, NullProc, NullProc },
    { AST_PolarWalrus, InitPolarWalrus, PlotPolarWalrus },
    { AST_PolarBomb, InitPolarBomb, PlotPolarBomb },
    { AST_BombMaker, InitBombMaker, PlotBombMaker },
    { AST_BombGFX, InitBombGFX, PlotBombGFX },
    { AST_WalrusFish, InitWalrusFish, PlotWalrusFish },
    { AST_BearDeath, InitBearDeath, PlotBearDeath },
    { AST_SmokeTrail, InitSmokeTrail, PlotSmokeTrail },
    { AST_SmokePoof, InitSmokePoof, PlotSmokePoof },
    { AST_SimpleExplosion, InitSimpleExplosion, PlotSimpleExplosion },
    { AST_DingoTail, InitDingoTail, PlotDingoTail },
    { AST_BrioPack, InitBrioPack, PlotBrioPack },
    { AST_CortexPack, InitCortexPack, PlotCortexPack },
    { AST_PackEffect, InitPackEffect, PlotPackEffect },
    { AST_PolarBoss, InitPolarBoss, PlotPolarBoss },
    { AST_RockProjectile, InitRockProjectile, PlotRockProjectile },
    { AST_Bearminator, InitBearminator, PlotBearminator },
    { AST_PolarMissile, InitPolarMissile, PlotPolarMissile },
    { AST_Rippled, InitRippled, PlotRippled },
    { AST_InnerRipple, InitInnerRipple, PlotInnerRipple },
    { AST_BossTransmuter, InitBossTransmuter, PlotBossTransmuter },
    { AST_BearCam, InitBearCam, PlotBearCam },
    { AST_BigMissile, InitBigMissile, PlotBigMissile },
    { AST_BearBoss, InitBearBoss, PlotBearBoss },
    { AST_Sniper, InitSniper, PlotSniper },
    { AST_SniperMissile, InitSniperMissile, PlotSniperMissile },
    { AST_WeatherNas, InitWeatherNas, PlotWeatherNas },
    { AST_PathEffect, InitPathEffect, PlotPathEffect },
    { AST_CrystalFlash, InitCrystalFlash, PlotCrystalFlash },
    { AST_SparkGFX, InitSparkGFX, PlotSparkGFX },
    { AST_RaceMissile, InitRaceMissile, PlotRaceMissile },
    { AST_RacePickUp, InitRacePickUp, PlotRacePickUp },
    { AST_TNTCrate, InitTNTCrate, PlotTNTCrate },
    { AST_DinoEgg, InitDinoEgg, PlotDinoEgg },
    { AST_RaceFire, InitRaceFire, PlotRaceFire },
    { AST_RaceTag, InitRaceTag, PlotRaceTag },
    { AST_RaceVolcano, InitRaceVolcano, PlotRaceVolcano },
    { AST_RaceShield, InitRaceShield, PlotRaceShield },
    { AST_RaceSmokePoof, InitRaceSmokePoof, PlotRaceSmokePoof },
    { AST_RaceSmokeBoost, InitRaceSmokeBoost, PlotRaceSmokeBoost },
    { AST_Glob, InitGlob, PlotGlob },
    { AST_BarrelRoll, InitBarrelRoll, PlotBarrelRoll },
    { AST_TrackGlob, InitTrackGlob, PlotTrackGlob },
    { AST_Dac, InitDac, PlotDac },
    { AST_RaceGreenSmokePoof, InitRaceGreenSmokePoof, PlotRaceGreenSmokePoof },
    { AST_RaceGreenSmokeBoost, InitRaceGreenSmokeBoost, PlotRaceGreenSmokeBoost },
    { AST_RaceSmokeBoostWater, InitRaceSmokeBoostWater, PlotRaceSmokeBoostWater },
    { AST_Ball, InitBall, PlotBall },
    { AST_AR_PU, NullProc, Plot_Ark_Pickup },
    { AST_ArkRepulse, Init_RepulseRing, Plot_RepulseRing },
    { AST_ArkFlash, Init_ArkCameraFlash, Plot_ArkCameraFlash },
    { AST_ArkDeadWall, Init_ArkDeadWall, Plot_ArkDeadWall },
    { AST_NGDeadWall, Init_NGArkDeadWall, Plot_ArkDeadWall },
    { AST_N_Gin, Init_N_Gin, Plot_N_Gin },
    { AST_SeaWeed, Init_seaweed, Plot_seaweed },
    { AST_LaserWall, Init_ArkLaserWall, Plot_ArkLaserWall },
    { AST_HazardType, NullProc, PlotHazardType },
    { AST_Balloon, NullProc, PlotBalloon },
    { AST_TitlesOdo, NullProc, NullProc },
    { AST_MiniTitlesOdo, NullProc, NullProc },
    { AST_PodiumOdo, InitPodiumOdo, PlotPodiumOdo },
    { AST_PodiumTrophy, InitPodiumTrophy, PlotPodiumTrophy },
    { AST_GatewayTrophy, InitGatewayTrophy, PlotGatewayTrophy },
    { AST_Crate, InitCrate, PlotCrate },
    { AST_CrateType, NullProc, PlotCrate },
    { AST_PACrate, InitCrate, PlotPACrate },
    { AST_PACrateType, NullProc, PlotPACrate },
    { AST_CrateExplode, InitCrateExplode, NullProc },
    { AST_CratePickup, InitCratePickup, PlotPickup },
    { AST_BigPoof, InitBigPoof, PlotBigPoof },
    { AST_ShieldPickup, InitShieldPickup, PlotPickup },
    { AST_BootsPickup, InitBootsPickup, PlotPickup },
    { AST_AnvilPickup, InitAnvilPickup, PlotPickup },
    { AST_SlowmoPickup, InitSlowmoPickup, PlotPickup },
    { AST_Health, Init_HealthPU, Plot_HealthPU },
    { AST_SpecialFX, InitScriptFX, PlotScriptFX },
    { AST_PogoPickup, Init_PogoPickup, Plot_PogoPickup },
    { AST_PogoPowerup, Init_PogoPowerup, Plot_PogoPowerup },
    { AST_PogoBanker, Init_PogoBanker, Plot_PogoBanker },
    { AST_PogoProjectile, Init_PogoProjectile, Plot_PogoProjectile },
    { AST_PogoTazer, Init_PogoTazer, Plot_PogoTazer },
    { AST_PogoRipperRoo, Init_PogoRipperRoo, Plot_PogoRipperRoo },
    { AST_PogoMushroom, Init_PogoMushroom, Plot_PogoMushroom },
    { AST_PogoPadlock, Init_PogoPadlock, Plot_PogoPadlock },
    { AST_FloatyScore, InitFloatyScore, PlotFloatyScore },
    { AST_DinoGem, InitDinoGem, PlotDinoGem },
    { AST_DinoScratch, InitDinoScratch, PlotDinoScratch },
    { AST_DinoSmoke, InitDinoSmoke, PlotDinoSmoke },
    { AST_BonkStool, InitBonkStool, PlotBonkStool },
    { AST_BonkFlag, InitBonkFlag, PlotBonkFlag },
    { AST_BonkShockwave, InitBonkShockwave, PlotBonkShockwave },
    { AST_BonkCrate, InitBonkCrate, PlotBonkCrate },
    { AST_N2OBall, InitN2OBall, PlotN2OBall },
    { AST_N2Omissile, InitN2Omissile, PlotN2Omissile },
    { AST_N2Ofirstmissile, InitN2Ofirstmissile, PlotN2Ofirstmissile },
    { AST_N2OmissileExplode, InitN2OmissileExplode, PlotN2OmissileExplode },
    { AST_N2ODeadWall, InitN2ODeadWall, PlotN2ODeadWall },
    { AST_AsteroidRoll, InitAsteroidRoll, PlotAsteroidRoll },
    { AST_NitroBoss, InitNitroBoss, PlotNitroBoss },
    { AST_DummyCamera, InitDummyCamera, PlotDummyCamera },
    { AST_NitroMissile, InitNitroMissile, PlotNitroMissile },
    { AST_NitroMine, InitNitroMine, PlotNitroMine },
    { AST_SpiralSat, InitSpiralSat, PlotSpiralSat },
    { AST_LaserShoot, InitLaserShoot, PlotLaserShoot },
    { AST_MineSFX, InitMineSFX, PlotMineSFX },
    { AST_NitroSmokeBoost, InitNitroSmokeBoost, PlotNitroSmokeBoost },
    { AST_BarrelCH, InitBarrelCH, PlotBarrelCH },
    { AST_Coal, InitCoal, PlotCoal },
    { AST_PUGenerator_CH, InitPUGenerator_CH, PlotPUGenerator_CH },
    { AST_PickupCH, InitPickupCH, PlotPickupCH },
    { AST_CoalFlame, InitCoalFlame, PlotCoalFlame },
    { AST_BarrelGenerator, InitBarrelGenerator, PlotBarrelGenerator },
    { AST_CircleFire, InitCircleFire, PlotCircleFire },
    { AST_Egg, InitEgg, PlotEgg },
    { AST_CoalFlameSFX, InitCoalFlameSFX, PlotCoalFlameSFX },
};

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */
extern u8 BASHSetUp[];
extern u8 BASH_PREMAP[];
extern u8 BONKSetUp[];
extern u8 BONK_PREMAP[];
extern u8 CHASESetUp[];
extern u8 CHASE_PREMAP[];
extern u8 DINOSetUp[];
extern u8 DINO_PREMAP[];
extern u8 HUBSetUp[];
extern u8 HUB_PREMAP[];
extern u8 N2OSetUp[];
extern u8 N2O_PREMAP[];
extern u8 PAPUSetUp[];
extern u8 PAPU_PREMAP[];
extern u8 POLARSetUp[];
extern u8 POLAR_PREMAP[];
extern u8 RACESetUp[];
extern u8 RACE_PREMAP[];
extern u8 SPINSetUp[];
extern u8 SPIN_PREMAP[];
extern u8 TA_SetUp[];
extern u8 TITLES_PREMAP[];
extern u8 TitlesSetUp[];
extern u8 bash_shell_info[];
extern u8 bonk_shell_info[];
extern u8 chase_shell_info[];
extern u8 dino_shell_info[];
extern u8 hub_shell_info[];
extern u8 n2o_shell_info[];
extern u8 papu_shell_info[];
extern u8 polar_shell_info[];
extern u8 polar_shell_info_b[];
extern u8 race_shell_info[];
extern u8 spin_shell_info[];
extern u8 titles_shell_info[];

ArenaDef DefaultArenaDef = {
    0x104, 0x1A, 0, 0x12, 0x69, { 0, 0 }, (void *)&BASH_PREMAP, &bash_shell_info,
    &BASHSetUp,
};
/* the arenas after the default one (SetupLevelSpecs reads on to id 999) */
ArenaDef ArenaDefs[22] = {
    { 1, 0x1A, 0, 0x12, 0x69, { 0, 0 }, (void *)&BASH_PREMAP, &bash_shell_info, &BASHSetUp },
    { 2, 0x1A, 0, 0x12, 0x69, { 0, 0 }, (void *)&BASH_PREMAP, &bash_shell_info, &BASHSetUp },
    { 3, 0x1A, 0, 0x12, 0x69, { 0, 0 }, (void *)&BASH_PREMAP, &bash_shell_info, &BASHSetUp },
    { 0x105, 0x28, 0, 0x18, 0xA0, { 0, 0 }, (void *)&PAPU_PREMAP, &papu_shell_info, &PAPUSetUp },
    { 0x106, 0xA0, 0, 0x38, 0x12C, { 0, 0 }, (void *)&TANK_PREMAP, &tank_shell_info, &TA_SetUp },
    { 0x107, 0x32, 0, 0x26, 0xE6, { 0, 0 }, (void *)&POGO_PREMAP, &POGO_shell_info, &POGOSetUp },
    { 0x108, 0x28, 0, 0x20, 0xA0, { 0, 0 }, (void *)&ARKENOID_PREMAP, &arkenoid_shell_info, &AR_SetUp },
    { 0x109, 0x64, 0, 0x19, 0xC8, { 0, 0 }, (void *)&POLAR_PREMAP, &polar_shell_info, &POLARSetUp },
    { 0x14, 0x28, 0, 0xF, 0x82, { 0, 0 }, (void *)&POLAR_PREMAP, &polar_shell_info_b, &POLARSetUp },
    { 0x15, 0x3C, 0, 0xF, 0x8C, { 0, 0 }, (void *)&POLAR_PREMAP, &polar_shell_info_b, &POLARSetUp },
    { 0x12, 0x3C, 0, 0x14, 0x8C, { 0, 0 }, (void *)&POLAR_PREMAP, &polar_shell_info_b, &POLARSetUp },
    { 0x16, 0x3C, 0, 0x14, 0x78, { 0, 0 }, (void *)&POLAR_PREMAP, &polar_shell_info_b, &POLARSetUp },
    { 0x10A, 0x64, 0, 0x1E, 0x82, { 0, 0 }, (void *)&RACE_PREMAP, &race_shell_info, &RACESetUp },
    { 0x19, 0x64, 0, 0x1E, 0x64, { 0, 0 }, (void *)&RACE_PREMAP, &race_shell_info, &RACESetUp },
    { 0x101, 4, 0, 4, 0x40, { 0, 0 }, (void *)&TITLES_PREMAP, &titles_shell_info, &TitlesSetUp },
    { 0x102, 0x64, 0, 0x20, 0x12C, { 0, 0 }, (void *)&HUB_PREMAP, &hub_shell_info, &HUBSetUp },
    { 0x103, 0x1E, 0, 0x20, 0x78, { 0, 0 }, (void *)&SPIN_PREMAP, &spin_shell_info, &SPINSetUp },
    { 0x10B, 0x1E, 0, 0x1E, 0xC8, { 0, 0 }, (void *)&DINO_PREMAP, &dino_shell_info, &DINOSetUp },
    { 0x10E, 0x1E, 0, 0x12, 0x64, { 0, 0 }, (void *)&BONK_PREMAP, &bonk_shell_info, &BONKSetUp },
    { 0x10C, 0x64, 0, 0x1E, 0x96, { 0, 0 }, (void *)&N2O_PREMAP, &n2o_shell_info, &N2OSetUp },
    { 0x10D, 0x64, 0, 0x3C, 0xFA, { 0, 0 }, (void *)&CHASE_PREMAP, &chase_shell_info, &CHASESetUp },
    { 0x3E7, 0x14, 1, 0x10, 0x14, { 0, 0 }, 0, 0, 0 },
};
