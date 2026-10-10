#ifndef BASH_POLAR_H
#define BASH_POLAR_H

/* Polar (overlay ovl62): declarations recovered from the original headers
 * (GameEng/Polar/polar.h: the leak ends part-way through it).
 * Names and types only; see docs/ARCHITECTURE.md (Leaked source). */

#include "bash/engine.h"

extern NAS *briosmokepack;
extern NAS *Cortexsmokepack;
#ifdef BASH_HAVE_EXTNODE
typedef struct {
    s32 EXTid;
    EXTnode Extend;
    s32 fadeuprate;
    s32 fadedownrate;
    s32 scalerate;
    s32 flag;
    s32 yvel;
    s32 startx;
    s32 amp;
    s32 ampdec;
    s32 angle;
} EXTsmoketrail;

typedef struct {
    s32 EXTid;
    EXTnode Extend;
    s32 NasStatus;
    s32 timer;
    s32 owner_id;
    s32 count;
} EXTbombmaker;
#endif /* BASH_HAVE_EXTNODE */

extern NAS *BombMaster;

typedef struct {
    s32 hitcount;
    s32 status;
    s32 secframe;
    s32 baseframe;
} psector;
#ifdef BASH_HAVE_EXTNODE
typedef struct {
    s32 EXTid;
    EXTnode Extend;
    s32 beamtype;
    s32 effect_player;
    s32 NasStatus;
    s32 hero_action[4];
    s32 hero_last_action[4];
    s32 hero_action_timer[4];
    s32 hero_immune_timer[4];
    s32 onoff_timer;
    s32 beamindex;
    s32 ontime;
    s32 offtime;
    s32 ittimer;
    u32 itplayer;
    s32 hero_in_beam_timer;
    EXTengine *it_eng;
    EXTengine *light_eng[4];
    SLONG_XYZ aimpoint;
    s32 pause;
    s32 rad;
    s32 yang;
    s32 addang;
    NAS *glow;
    NAS *shadow;
    NAS *beam;
    NAS *cam;
    s32 timer;
    psector sectordata[16];
    s32 angular_speed;
    s32 hit_player;
    s32 target_angle;
    s32 angular_acc;
    s32 angular_dec;
    s32 angular_max;
    SLONG_XYZ target_pos;
    s32 chasetimer;
} EXTtransmuter;
#endif /* BASH_HAVE_EXTNODE */

enum POLAR_AST_types {
    POLAR_AST_zero = 0x1600,
    AST_TransmuterBeam,
    AST_WeightDeath,
    AST_ParticleEffect,
    AST_GravityObject,
    AST_ScriptFX,
    AST_ScratchMaster,
    AST_IceBerg,
    AST_SnowMaster,
    AST_BreathMaster,
    AST_UkaPolarMask,
    AST_CameraNull,
    AST_PolarWalrus,
    AST_PolarBomb,
    AST_BombMaker,
    AST_BombGFX,
    AST_WalrusFish,
    AST_BearDeath,
    AST_SmokeTrail,
    AST_SmokePoof,
    AST_SimpleExplosion,
    AST_DingoTail,
    AST_BrioPack,
    AST_CortexPack,
    AST_PackEffect,
    AST_PolarBoss,
    AST_RockProjectile,
    AST_Bearminator,
    AST_PolarMissile,
    AST_Rippled,
    AST_InnerRipple,
    AST_BossTransmuter,
    AST_BearCam,
    AST_BigMissile,
    AST_BearBoss,
    AST_Sniper,
    AST_SniperMissile,
    AST_WeatherNas,
    AST_PathEffect,
    AST_CrystalFlash,
    AST_SparkGFX,
};

typedef struct {
    subengine *sub[6];
} bdata;

extern NAS *Walrus;
extern NAS *HeadPolarObject;
extern s32 NasCheckZoneBoundries(EXTengine *eng, SLONG_XYZ *pos, SLONG_XYZ *close, s32 radoff, s32 zoneid);
extern bdata barrierdata[];
extern subengine *bossmapsub;
s32 CalcCurrentSector(SLONG_XYZ *pt); /* inline in the original */
void InitialisePOLARFirst();
void InitialisePOLAR();
void UpdatePOLAR();
void POLARtriggers(NASTrigger *, s32);
void FirstTriggersPOLAR();
s32 CountBears();
bool DoNowt(EXTengine *eng);
void Polar_HeroBot(u32 player);
bool IsRayEndInZone(SLONG_XYZ *pos, s32 zoneid);
NAS *IsBombAvailable();
bool DoesPlayerHaveBomb(u32 player);
bool IsPlayerInRange(u32 player, u32 player2);
bool Player2PlayerRayTest(u32 player, u32 danger_player);
bool TestForWalrus(u32 player);
bool TestBeamChance(u32 player);
bool TestChaseChance(u32 player);
bool TestEvadeChance(u32 player);
bool TestEdgeChance(u32 player);
bool TestForDanger(u32 player);
bool TestMaskChance(u32 player);
bool TestSellDummyChance(u32 player);
bool TestBombChance(u32 player);
bool TestForTaunt(u32 player);
bool TestForTeamMate(u32 source, u32 target);
bool EnsureMateNotObstructingMate(u32 player);
s32 AIAttackOverride(u32 player);
void DoAcclnMovement(EXTheromove *mot, NAS *m);
void DoEvadePlayers(u32 player);
void DoChaseMask(u32 player);
void InitMoveToCentre(u32 player);
void DoMoveToCentre(u32 player);
void DoChooseNewAction(u32 player);
bool IsPlayerInWay(u32 player);
u32 FindClosetPlayer(u32 player, s32 *dist);
s32 PlayerActionCount(s32 action, s32 player);
void ResetMass(s32 player);
bool CheckMelted(SLONG_XYZ *pos, s32 zoneid);
bool IsPtInALLZone(SLONG_XYZ *pos, s32 zoneid);
void TE_InitTestCamera();
void InitTransmuterBeam(NAS *m);
void PlotTransmuterBeam(NAS *m);
void InitWeightDeath(NAS *m);
void PlotWeightDeath(NAS *m);
void InitBreathMaster(NAS *m);
void PlotBreathMaster(NAS *m);
void InitScratchMaster(NAS *m);
void PlotScratchMaster(NAS *m);
void InitIceBerg(NAS *m);
void PlotIceBerg(NAS *m);
void InitSnowMaster(NAS *m);
void PlotSnowMaster(NAS *m);
void InitUkaPolarMask(NAS *m);
void PlotUkaPolarMask(NAS *m);
void InitPolarWalrus(NAS *m);
void PlotPolarWalrus(NAS *m);
void InitPolarBomb(NAS *m);
void PlotPolarBomb(NAS *m);
void InitBombMaker(NAS *m);
void PlotBombMaker(NAS *m);
void InitBombGFX(NAS *m);
void PlotBombGFX(NAS *m);
void InitWalrusFish(NAS *m);
void PlotWalrusFish(NAS *m);
void InitBearDeath(NAS *m);
void PlotBearDeath(NAS *m);
void InitSmokeTrail(NAS *m);
void PlotSmokeTrail(NAS *m);
void InitSmokePoof(NAS *m);
void PlotSmokePoof(NAS *m);
void InitSimpleExplosion(NAS *m);
void PlotSimpleExplosion(NAS *m);
void InitDingoTail(NAS *m);
void PlotDingoTail(NAS *m);
void InitBrioPack(NAS *m);
void PlotBrioPack(NAS *m);
void InitCortexPack(NAS *m);
void PlotCortexPack(NAS *m);
void InitPackEffect(NAS *m);
void PlotPackEffect(NAS *m);
void InitPolarBoss(NAS *m);
void PlotPolarBoss(NAS *m);
void InitRockProjectile(NAS *m);
void PlotRockProjectile(NAS *m);
void InitPolarMissile(NAS *m);
void PlotPolarMissile(NAS *m);
void InitBearminator(NAS *m);
void PlotBearminator(NAS *m);
void InitInnerRipple(NAS *m);
void PlotInnerRipple(NAS *m);
void InitRippled(NAS *m);
void PlotRippled(NAS *m);
void InitBossTransmuter(NAS *m);
void PlotBossTransmuter(NAS *m);
void InitBearCam(NAS *m);
void PlotBearCam(NAS *m);
void InitBearBoss(NAS *m);
void PlotBearBoss(NAS *m);
void InitBigMissile(NAS *m);
void PlotBigMissile(NAS *m);
void InitSniper(NAS *m);
void PlotSniper(NAS *m);
void InitSniperMissile(NAS *m);
void PlotSniperMissile(NAS *m);
void InitWeatherNas(NAS *m);
void PlotWeatherNas(NAS *m);
void InitPathEffect(NAS *m);
void PlotPathEffect(NAS *m);
void InitCrystalFlash(NAS *m);
void PlotCrystalFlash(NAS *m);
void InitSparkGFX(NAS *m);
void PlotSparkGFX(NAS *m);
bool TestHeroObstaclesICE(u32 player);
extern s32 AIQuick;
extern file_group_load_info file_group_POLARDAY;
extern file_group_load_info file_group_POLARNGT;
extern file_group_load_info file_group_POLARCUT;
extern file_group_load_info file_group_POLARCHA;
extern file_group_load_info file_group_POLARCUC;
extern file_group_load_info file_group_POLARNGC;
extern file_group_load_info file_group_POLAREND;
extern file_group_load_info file_group_POLARENC;
extern file_group_load_info file_group_POLARBSS;
#ifdef BASH_HAVE_HERO_SEQ
extern hero_seq CRA_seq_polaridle[];
extern hero_seq CRA_seq_polarrun[];
extern hero_seq CRA_seq_polardazed[];
extern hero_seq CRA_seq_polarfall[];
extern hero_seq CRA_seq_polarattack[];
extern hero_seq CRA_seq_polarhit[];
extern hero_seq CRA_seq_polardead[];
extern hero_seq CRA_seq_polartaunt[];
extern hero_seq CRA_seq_breathe_ft[];
extern hero_seq CRA_seq_skate_ft[];
extern hero_seq CRA_seq_lightbear[];
extern hero_seq CRA_seq_skate_attack[];
extern hero_seq CRA_seq_win_ft[];
extern hero_seq CRA_seq_polarwin[];
extern hero_seq COC_seq_polaridle[];
extern hero_seq COC_seq_polarrun[];
extern hero_seq COC_seq_polardazed[];
extern hero_seq COC_seq_polarfall[];
extern hero_seq COC_seq_polarattack[];
extern hero_seq COC_seq_polarhit[];
extern hero_seq COC_seq_polardead[];
extern hero_seq COC_seq_polartaunt[];
extern hero_seq COC_seq_breathe_ft[];
extern hero_seq COC_seq_skate_ft[];
extern hero_seq COC_seq_lightbear[];
extern hero_seq COC_seq_skate_attack[];
extern hero_seq COC_seq_win_ft[];
extern hero_seq COC_seq_polarwin[];
extern hero_seq CTX_seq_polaridle[];
extern hero_seq CTX_seq_polarrun[];
extern hero_seq CTX_seq_polardazed[];
extern hero_seq CTX_seq_polarfall[];
extern hero_seq CTX_seq_polarattack[];
extern hero_seq CTX_seq_polarhit[];
extern hero_seq CTX_seq_polardead[];
extern hero_seq CTX_seq_polartaunt[];
extern hero_seq CTX_seq_breathe_ft[];
extern hero_seq CTX_seq_skate_ft[];
extern hero_seq CTX_seq_lightbear[];
extern hero_seq CTX_seq_skate_attack[];
extern hero_seq CTX_seq_win_ft[];
extern hero_seq CTX_seq_polarwin[];
extern hero_seq TIN_seq_polaridle[];
extern hero_seq TIN_seq_polarrun[];
extern hero_seq TIN_seq_polardazed[];
extern hero_seq TIN_seq_polarfall[];
extern hero_seq TIN_seq_polarattack[];
extern hero_seq TIN_seq_polarhit[];
extern hero_seq TIN_seq_polardead[];
extern hero_seq TIN_seq_polartaunt[];
extern hero_seq TIN_seq_breathe_ft[];
extern hero_seq TIN_seq_skate_ft[];
extern hero_seq TIN_seq_lightbear[];
extern hero_seq TIN_seq_skate_attack[];
extern hero_seq TIN_seq_win_ft[];
extern hero_seq TIN_seq_polarwin[];
extern hero_seq KON_seq_polaridle[];
extern hero_seq KON_seq_polarrun[];
extern hero_seq KON_seq_polardazed[];
extern hero_seq KON_seq_polarfall[];
extern hero_seq KON_seq_polarattack[];
extern hero_seq KON_seq_polarhit[];
extern hero_seq KON_seq_polardead[];
extern hero_seq KON_seq_polartaunt[];
extern hero_seq KON_seq_breathe_ft[];
extern hero_seq KON_seq_skate_ft[];
extern hero_seq KON_seq_lightbear[];
extern hero_seq KON_seq_skate_attack[];
extern hero_seq KON_seq_win_ft[];
extern hero_seq KON_seq_polarwin[];
extern hero_seq DNG_seq_polaridle[];
extern hero_seq DNG_seq_polarrun[];
extern hero_seq DNG_seq_polardazed[];
extern hero_seq DNG_seq_polarfall[];
extern hero_seq DNG_seq_polarattack[];
extern hero_seq DNG_seq_polarhit[];
extern hero_seq DNG_seq_polardead[];
extern hero_seq DNG_seq_polartaunt[];
extern hero_seq DNG_seq_breathe_ft[];
extern hero_seq DNG_seq_skate_ft[];
extern hero_seq DNG_seq_lightbear[];
extern hero_seq DNG_seq_skate_attack[];
extern hero_seq DNG_seq_win_ft[];
extern hero_seq DNG_seq_polarwin[];
extern hero_seq KAN_seq_polaridle[];
extern hero_seq KAN_seq_polarrun[];
extern hero_seq KAN_seq_polardazed[];
extern hero_seq KAN_seq_polarfall[];
extern hero_seq KAN_seq_polarattack[];
extern hero_seq KAN_seq_polarhit[];
extern hero_seq KAN_seq_polardead[];
extern hero_seq KAN_seq_polartaunt[];
extern hero_seq KAN_seq_breathe_ft[];
extern hero_seq KAN_seq_skate_ft[];
extern hero_seq KAN_seq_lightbear[];
extern hero_seq KAN_seq_skate_attack[];
extern hero_seq KAN_seq_win_ft[];
extern hero_seq KAN_seq_polarwin[];
extern hero_seq BRO_seq_polaridle[];
extern hero_seq BRO_seq_polarrun[];
extern hero_seq BRO_seq_polardazed[];
extern hero_seq BRO_seq_polarfall[];
extern hero_seq BRO_seq_polarattack[];
extern hero_seq BRO_seq_polarhit[];
extern hero_seq BRO_seq_polardead[];
extern hero_seq BRO_seq_polartaunt[];
extern hero_seq BRO_seq_breathe_ft[];
extern hero_seq BRO_seq_skate_ft[];
extern hero_seq BRO_seq_lightbear[];
extern hero_seq BRO_seq_skate_attack[];
extern hero_seq BRO_seq_win_ft[];
extern hero_seq BRO_seq_polarwin[];
#endif /* BASH_HAVE_HERO_SEQ */

extern s16 PBQualityFactor1[];

#endif /* BASH_POLAR_H */
