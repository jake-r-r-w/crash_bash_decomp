#ifndef BASH_ARKENOID_H
#define BASH_ARKENOID_H

/* Arkenoid (overlay ovl61): declarations recovered from the original headers
 * (GameEng/Arkenoid/arkenoid.h).
 * Names and types only; see docs/ARCHITECTURE.md (Leaked source). */

#include "bash/nas.h"

enum ARKENOID_AST_types {
    ARKENOID_AST_zero = 0x1500,
    AST_Ball,
    AST_AR_PU,
    AST_ArkRepulse,
    AST_ArkFlash,
    AST_ArkDeadWall,
    AST_NGDeadWall,
    AST_N_Gin,
    AST_SeaWeed,
    AST_LaserWall,
};

void AR_SetUp();
void AR_PreInitLevel();
void AR_InitLevel();
void AR_RestartLevel();
void AR_UpdateLevel();
void AR_Triggers(NASTrigger *, s32);
void AR_EarliestTriggers();
void AR_InitIcons();
void AR_InitHero(u32);
void AR_InitShadow(u32);
void AR_UpdateShadow(u32);
void AR_PreMoveHero(u32);
void AR_PI_PreMoveHero(u32);
void AR_MoveHero(u32 player);
void AR_PI_MoveHero(u32);
void HeroChooseARScoreWinner(s32, u16);
void Ark_HeroBot(u32);
NAS *DangerBalls(u32);
bool AR_KeepInBounds(EXTengine *);
bool AR_PI_KeepInBounds(EXTengine *);
s16 AR_BA_BounceOnBounds(EXTengine *, s32);
s16 AR_SE_BounceOnBounds(EXTengine *, s32);
s16 AR_PI_BounceOnBounds(EXTengine *, s32);
bool AR_BA_TestOutOfBounds(EXTengine *, EXTobject *, bool);
bool AR_SE_TestOutOfBounds(EXTengine *, EXTobject *, bool);
bool AR_NG_TestOutOfBounds(EXTengine *, EXTobject *, bool);
bool AR_PI_TestOutOfBounds(EXTengine *, EXTobject *, bool);
bool AR_BA_TestLaunchBounds(EXTengine *, EXTobject *);
bool AR_PI_TestLaunchBounds(EXTengine *, EXTobject *);
bool GetIntersect(SLONG_XYZ *, SLONG_XYZ *, SLONG_XYZ *);
bool TestArkenoidStop(u32 player);
bool TestForMove(u32 player);
void SetArkHeroIdle(u32 player);
void ArkHeroIdle(u32 player);
void SetArkHeroMove(u32 player);
void ArkHeroMove(u32 player);
void SetArkHeroKick(u32 player);
void ArkHeroKick(u32 player);
void SetArkHeroRedKick(u32 player);
void ArkHeroRedKick(u32 player);
void SetArkHeroGrab(u32);
void ArkHeroGrab(u32);
void SetArkHeroTaunt(u32);
void ArkHeroTaunt(u32);
void SetArkHeroWinner(u32);
void ArkHeroWinner(u32);
void SetArkHeroLose(u32);
void ArkHeroLose(u32);
void SetArkHeroDie(u32);
void ArkHeroDie(u32);
void SetArkHeroDead(u32);
void ArkHeroDead(u32);
s32 PickArkPlayer(u32);
void FinishArkLevel();
NAS *CreateBall(NAS *);
void InitBall(NAS *);
void PlotBall(NAS *);
void CalcArkInfluence(EXTmotion *);
void FreeGrabbedObject(u32, bool);
void AlignGrabBall(EXTengine *, EXTmotion *, EXTobject *);
void Init_N_Gin(NAS *);
void Plot_N_Gin(NAS *);
void InitARKfloor();
void UpdateArkFloor();
void ArkCrateMaster();
void Init_seaweed(NAS *);
void Plot_seaweed(NAS *);
void Init_ArkCameraFlash(NAS *);
void Plot_ArkCameraFlash(NAS *);
void Init_ArkDeadWall(NAS *);
void Init_NGArkDeadWall(NAS *);
void Plot_ArkDeadWall(NAS *);
void SetCornerFailure(u16);
void HandleCornerFailure();
s32 GetArkContour(EXTengine *);
s32 GetArkPIContour(EXTengine *);
void Create_ArkPickup();
void Plot_Ark_Pickup(NAS *);
void Init_ArkLaserWall(NAS *);
void Plot_ArkLaserWall(NAS *);
void Init_RepulseRing(NAS *);
void Plot_RepulseRing(NAS *);
void Init_MagnaRing(NAS *);
void Plot_MagnaRing(NAS *);
void LineToCircleTest(SLONG_XYZ *, s32, SLONG_XYZ *, s32);
NAS *ArkHero2ObjectCollision(u32);
bool ArkHeroRad2ObjectCollision(u32, s32, bool);
void ArkHeroRad2ObjectRepel(u32, s32);
void ArkHeroRad2ObjectAttract(u32, s32);
bool TestArkObjectObstacles(EXTengine *, EXTobject *);
s32 TestArkObject2Hero(EXTengine *, u16);
extern PreMap ARKENOID_PREMAP;
extern LevSpec ARKENOIDSpecifics;
extern shell_setup arkenoid_shell_info;
extern file_group_load_info file_group_BALLS;
extern file_group_load_info file_group_BEACHBALL;
extern file_group_load_info file_group_NGINBALL;
extern file_group_load_info file_group_SKYBALL;
#ifdef BASH_HAVE_HERO_SEQ
extern hero_seq CRA_seq_arkbreathe[];
extern hero_seq CRA_seq_arkidle[];
extern hero_seq CRA_seq_arktaunt[];
extern hero_seq CRA_seq_arkmove[];
extern hero_seq CRA_seq_arkkick[];
extern hero_seq CRA_seq_arkwin[];
extern hero_seq CRA_seq_arkvictory[];
extern hero_seq CRA_seq_arklose[];
extern hero_seq COC_seq_arkbreathe[];
extern hero_seq COC_seq_arkidle[];
extern hero_seq COC_seq_arktaunt[];
extern hero_seq COC_seq_arkmove[];
extern hero_seq COC_seq_arkkick[];
extern hero_seq COC_seq_arkwin[];
extern hero_seq COC_seq_arkvictory[];
extern hero_seq COC_seq_arklose[];
extern hero_seq TIN_seq_arkbreathe[];
extern hero_seq TIN_seq_arkidle[];
extern hero_seq TIN_seq_arktaunt[];
extern hero_seq TIN_seq_arkmove[];
extern hero_seq TIN_seq_arkkick[];
extern hero_seq TIN_seq_arkwin[];
extern hero_seq TIN_seq_arkvictory[];
extern hero_seq TIN_seq_arklose[];
extern hero_seq CTX_seq_arkbreathe[];
extern hero_seq CTX_seq_arkidle[];
extern hero_seq CTX_seq_arktaunt[];
extern hero_seq CTX_seq_arkmove[];
extern hero_seq CTX_seq_arkkick[];
extern hero_seq CTX_seq_arkwin[];
extern hero_seq CTX_seq_arkvictory[];
extern hero_seq CTX_seq_arklose[];
extern hero_seq BRO_seq_arkbreathe[];
extern hero_seq BRO_seq_arkidle[];
extern hero_seq BRO_seq_arktaunt[];
extern hero_seq BRO_seq_arkmove[];
extern hero_seq BRO_seq_arkkick[];
extern hero_seq BRO_seq_arkwin[];
extern hero_seq BRO_seq_arkvictory[];
extern hero_seq BRO_seq_arklose[];
extern hero_seq DNG_seq_arkbreathe[];
extern hero_seq DNG_seq_arkidle[];
extern hero_seq DNG_seq_arktaunt[];
extern hero_seq DNG_seq_arkmove[];
extern hero_seq DNG_seq_arkkick[];
extern hero_seq DNG_seq_arkwin[];
extern hero_seq DNG_seq_arkvictory[];
extern hero_seq DNG_seq_arklose[];
extern hero_seq KON_seq_arkbreathe[];
extern hero_seq KON_seq_arkidle[];
extern hero_seq KON_seq_arktaunt[];
extern hero_seq KON_seq_arkmove[];
extern hero_seq KON_seq_arkkick[];
extern hero_seq KON_seq_arkwin[];
extern hero_seq KON_seq_arkvictory[];
extern hero_seq KON_seq_arklose[];
extern hero_seq KAN_seq_arkbreathe[];
extern hero_seq KAN_seq_arkidle[];
extern hero_seq KAN_seq_arktaunt[];
extern hero_seq KAN_seq_arkmove[];
extern hero_seq KAN_seq_arkkick[];
extern hero_seq KAN_seq_arkwin[];
extern hero_seq KAN_seq_arkvictory[];
extern hero_seq KAN_seq_arklose[];
#endif /* BASH_HAVE_HERO_SEQ */

#endif /* BASH_ARKENOID_H */
