/* The engine's AST table (EngineASTTable, 0x80098720): every object type's id and its
 * Init/Plot, which CreateNAS and the triggers look up. Most entries point into the slot
 * overlays, which share one address range: the engine's link takes those functions'
 * addresses from the overlays' own links (tools/scripts/slot_syms.py), so an overlay
 * that moves takes its entries with it. The ids are the Oct 9 prototype's AST enums. */
#include "common.h"
#include "bash/nas.h"
#include "bash/80022a3c.h"
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
void InitAnvilPickup(NAS *nas);  /* ovl5d */
void InitBash8way(NAS *nas);  /* ovl5d */
void InitBashHomer(NAS *nas);  /* ovl5d */
void InitBashPenguin(NAS *nas);  /* ovl5d */
void InitBashShox(NAS *nas);  /* ovl5d */
void InitBoing(NAS *nas);  /* ovl5d */
void InitBootsPickup(NAS *nas);  /* ovl5d */
void InitCratePickup(NAS *nas);  /* ovl5d */
void InitMissileExplode(NAS *nas);  /* ovl5d */
void InitShieldPickup(NAS *nas);  /* ovl5d */
void InitSlowmoPickup(NAS *nas);  /* ovl5d */
void InitSnowGlint(NAS *nas);  /* ovl5d */
void InitTumbler(NAS *nas);  /* ovl5d */
void PlotBash8way(NAS *nas);  /* ovl5d */
void PlotBashHomer(NAS *nas);  /* ovl5d */
void PlotBashPenguin(NAS *nas);  /* ovl5d */
void PlotBashShox(NAS *nas);  /* ovl5d */
void PlotBoing(NAS *nas);  /* ovl5d */
void PlotMissileExplode(NAS *nas);  /* ovl5d */
void PlotSnowGlint(NAS *nas);  /* ovl5d */
void PlotTumbler(NAS *nas);  /* ovl5d */
void func_800BBD48(NAS *nas);  /* ovl5d */
void func_800BC038(NAS *nas);  /* ovl5d */
void func_800BE9D0(NAS *nas);  /* ovl5d */
void InitPapuBoss(NAS *nas);  /* ovl5e */
void InitPapuColumn(NAS *nas);  /* ovl5e */
void PlotPapuBoss(NAS *nas);  /* ovl5e */
void PlotPapuColumn(NAS *nas);  /* ovl5e */
void PlotPapuMinion(NAS *nas);  /* ovl5e */
void func_800B87A0(NAS *nas);  /* ovl5e */
void InitGravityObject(NAS *nas);  /* ovl62 */
void PlotGravityObject(NAS *nas);  /* ovl62 */
void InitBarrelRoll(NAS *nas);  /* ovl63 */
void InitDac(NAS *nas);  /* ovl63 */
void InitDinoEgg(NAS *nas);  /* ovl63 */
void InitGlob(NAS *nas);  /* ovl63 */
void InitRaceFire(NAS *nas);  /* ovl63 */
void InitRaceGreenSmokeBoost(NAS *nas);  /* ovl63 */
void InitRaceGreenSmokePoof(NAS *nas);  /* ovl63 */
void InitRaceMissile(NAS *nas);  /* ovl63 */
void InitRacePickUp(NAS *nas);  /* ovl63 */
void InitRaceShield(NAS *nas);  /* ovl63 */
void InitRaceSmokeBoost(NAS *nas);  /* ovl63 */
void InitRaceSmokeBoostWater(NAS *nas);  /* ovl63 */
void InitRaceSmokePoof(NAS *nas);  /* ovl63 */
void InitRaceTag(NAS *nas);  /* ovl63 */
void InitRaceVolcano(NAS *nas);  /* ovl63 */
void InitTNTCrate(NAS *nas);  /* ovl63 */
void InitTrackGlob(NAS *nas);  /* ovl63 */
void PlotBarrelRoll(NAS *nas);  /* ovl63 */
void PlotDac(NAS *nas);  /* ovl63 */
void PlotDinoEgg(NAS *nas);  /* ovl63 */
void PlotGlob(NAS *nas);  /* ovl63 */
void PlotRaceFire(NAS *nas);  /* ovl63 */
void PlotRaceGreenSmokeBoost(NAS *nas);  /* ovl63 */
void PlotRaceGreenSmokePoof(NAS *nas);  /* ovl63 */
void PlotRaceMissile(NAS *nas);  /* ovl63 */
void PlotRacePickUp(NAS *nas);  /* ovl63 */
void PlotRaceShield(NAS *nas);  /* ovl63 */
void PlotRaceSmokeBoost(NAS *nas);  /* ovl63 */
void PlotRaceSmokeBoostWater(NAS *nas);  /* ovl63 */
void PlotRaceSmokePoof(NAS *nas);  /* ovl63 */
void PlotRaceTag(NAS *nas);  /* ovl63 */
void PlotRaceVolcano(NAS *nas);  /* ovl63 */
void PlotTNTCrate(NAS *nas);  /* ovl63 */
void PlotTrackGlob(NAS *nas);  /* ovl63 */
void PlotBalloon(NAS *nas);  /* ovl64 */
void PlotHazardType(NAS *nas);  /* ovl64 */
void InitDinoGem(NAS *nas);  /* ovl65 */
void InitDinoScratch(NAS *nas);  /* ovl65 */
void InitDinoSmoke(NAS *nas);  /* ovl65 */
void PlotDinoGem(NAS *nas);  /* ovl65 */
void PlotDinoScratch(NAS *nas);  /* ovl65 */
void PlotDinoSmoke(NAS *nas);  /* ovl65 */
void InitBonkCrate(NAS *nas);  /* ovl66 */
void InitBonkFlag(NAS *nas);  /* ovl66 */
void InitBonkShockwave(NAS *nas);  /* ovl66 */
void InitBonkStool(NAS *nas);  /* ovl66 */
void PlotBonkCrate(NAS *nas);  /* ovl66 */
void PlotBonkFlag(NAS *nas);  /* ovl66 */
void PlotBonkShockwave(NAS *nas);  /* ovl66 */
void PlotBonkStool(NAS *nas);  /* ovl66 */
void InitAsteroidRoll(NAS *nas);  /* ovl67 */
void InitDummyCamera(NAS *nas);  /* ovl67 */
void InitLaserShoot(NAS *nas);  /* ovl67 */
void InitMineSFX(NAS *nas);  /* ovl67 */
void InitN2OBall(NAS *nas);  /* ovl67 */
void InitN2ODeadWall(NAS *nas);  /* ovl67 */
void InitN2Ofirstmissile(NAS *nas);  /* ovl67 */
void InitN2Omissile(NAS *nas);  /* ovl67 */
void InitN2OmissileExplode(NAS *nas);  /* ovl67 */
void InitNitroBoss(NAS *nas);  /* ovl67 */
void InitNitroMine(NAS *nas);  /* ovl67 */
void InitNitroMissile(NAS *nas);  /* ovl67 */
void InitNitroSmokeBoost(NAS *nas);  /* ovl67 */
void InitSpiralSat(NAS *nas);  /* ovl67 */
void PlotAsteroidRoll(NAS *nas);  /* ovl67 */
void PlotDummyCamera(NAS *nas);  /* ovl67 */
void PlotLaserShoot(NAS *nas);  /* ovl67 */
void PlotMineSFX(NAS *nas);  /* ovl67 */
void PlotN2OBall(NAS *nas);  /* ovl67 */
void PlotN2ODeadWall(NAS *nas);  /* ovl67 */
void PlotN2Ofirstmissile(NAS *nas);  /* ovl67 */
void PlotN2Omissile(NAS *nas);  /* ovl67 */
void PlotN2OmissileExplode(NAS *nas);  /* ovl67 */
void PlotNitroBoss(NAS *nas);  /* ovl67 */
void PlotNitroMine(NAS *nas);  /* ovl67 */
void PlotNitroMissile(NAS *nas);  /* ovl67 */
void PlotNitroSmokeBoost(NAS *nas);  /* ovl67 */
void PlotSpiralSat(NAS *nas);  /* ovl67 */
void InitBarrelCH(NAS *nas);  /* ovl68 */
void InitBarrelGenerator(NAS *nas);  /* ovl68 */
void InitCircleFire(NAS *nas);  /* ovl68 */
void InitCoal(NAS *nas);  /* ovl68 */
void InitCoalFlame(NAS *nas);  /* ovl68 */
void InitCoalFlameSFX(NAS *nas);  /* ovl68 */
void InitEgg(NAS *nas);  /* ovl68 */
void InitPUGenerator_CH(NAS *nas);  /* ovl68 */
void InitPickupCH(NAS *nas);  /* ovl68 */
void PlotBarrelCH(NAS *nas);  /* ovl68 */
void PlotBarrelGenerator(NAS *nas);  /* ovl68 */
void PlotCircleFire(NAS *nas);  /* ovl68 */
void PlotCoal(NAS *nas);  /* ovl68 */
void PlotCoalFlame(NAS *nas);  /* ovl68 */
void PlotCoalFlameSFX(NAS *nas);  /* ovl68 */
void PlotEgg(NAS *nas);  /* ovl68 */
void PlotPUGenerator_CH(NAS *nas);  /* ovl68 */
void PlotPickupCH(NAS *nas);  /* ovl68 */
void InitGatewayTrophy(NAS *nas);  /* ovl6a */
void InitPodiumOdo(NAS *nas);  /* ovl6a */
void InitPodiumTrophy(NAS *nas);  /* ovl6a */
void PlotGatewayTrophy(NAS *nas);  /* ovl6a */
void PlotPodiumOdo(NAS *nas);  /* ovl6a */
void PlotPodiumTrophy(NAS *nas);  /* ovl6a */
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
    { AST_Crate, func_800BBD48, func_800BC038 },
    { AST_CrateType, NullProc, func_800BC038 },
    { AST_PACrate, func_800BBD48, func_800B87A0 },
    { AST_PACrateType, NullProc, func_800B87A0 },
    { AST_CrateExplode, InitCrateExplode, NullProc },
    { AST_CratePickup, InitCratePickup, func_800BE9D0 },
    { AST_BigPoof, InitBigPoof, PlotBigPoof },
    { AST_ShieldPickup, InitShieldPickup, func_800BE9D0 },
    { AST_BootsPickup, InitBootsPickup, func_800BE9D0 },
    { AST_AnvilPickup, InitAnvilPickup, func_800BE9D0 },
    { AST_SlowmoPickup, InitSlowmoPickup, func_800BE9D0 },
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
