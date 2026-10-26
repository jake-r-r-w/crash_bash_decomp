#ifndef BASH_POGO_H
#define BASH_POGO_H

/* Pogo (overlay pogo): declarations recovered from the original headers
 * (GameEng/Pogo/pogo.h and psx_pogo.h).
 * Names and types only; see docs/ARCHITECTURE.md (Leaked source). */

#include "bash/nas.h"

enum POGO_AST_types {
    POGO_AST_zero = 0x1400,
    AST_PogoPickup,
    AST_PogoPowerup,
    AST_PogoBanker,
    AST_PogoProjectile,
    AST_PogoTazer,
    AST_PogoRipperRoo,
    AST_PogoMushroom,
    AST_PogoPadlock,
    AST_FloatyScore,
};

enum {
    PPP_SHOOT = 0x0001,
    PPP_SHOOTFOURWAYS,
    PPP_TAZER,
    PPP_SUPAROCKET,
    PPP_SUPERQUICK,
    PPP_TILELOCKER,
    PPP_ARROW,
    PPP_DOUBLEARROW,
    PPT_FOURWAYS,
    PPP_BANK,
    PPP_MUSHROOM,
};

enum {
    TILEMODE_FIZZNORMAL,
    TILEMODE_NORMAL,
    TILEMODE_PREFIZZOUT,
    TILEMODE_FIZZOUT,
    TILEMODE_ONFIRE,
    TILEMODE_TNT,
    TILEMODE_NITRO,
    TILEMODE_FOURWAY,
    TILEMODE_PRESHOCK,
    TILEMODE_POSTSHOCK,
    TILEMODE_LOCKED,
};

enum {
    PBM_WAITINGTOAPPEAR,
    PBM_INIT,
    PBM_WARPING,
    PBM_IDLING,
    PBM_COLLECTED,
    PBM_PUFFOUT,
    PBM_WARNINGPUFFDELAY,
};

enum {
    PogoAI_Bored = 1,
    PogoAI_FindClosestPickup,
    PogoAI_JustWandering,
    PogoAI_GotoLocation,
    PogoAI_EncircleArea,
    PogoAI_EncloseArea,
    PogoAI_GotoMyOwn,
};

enum {
    ROO_INIT,
    ROO_PAUSEATSTART,
    ROO_ONEDGE,
    ROO_ONMAP,
    ROO_TEMPSTOPONMAP,
    ROO_SPINAROUND,
    ROO_DROPTNT,
    ROO_DROPTNTANDFOURWAYS,
    ROO_FIREROCKETS,
    ROO_CRYSTAL_SHOOTHUMANS,
    ROO_CRYSTAL_DROPNITRO,
    ROO_CRYSTAL_DROPNITROANDBULLY,
    ROO_RESETLIST,
};

enum {
    DIR_DOWN,
    DIR_LEFT,
    DIR_UP,
    DIR_RIGHT,
    DIR_ALL,
};

enum {
    PROJECTILE_FIREBALL,
    PROJECTILE_ROCKET1,
    PROJECTILE_ROCKET2,
    PROJECTILE_SUPAROCKET,
};

enum {
    PADLOCK_INLIMBO,
    PADLOCK_WARPIN,
    PADLOCK_ACTIVE,
    PADLOCK_STATIONARY,
    PADLOCK_PICKRNDDIRECTION,
    PADLOCK_MOVE,
    PADLOCK_PUFFOFF,
};

typedef struct {
    NAS *pickup;
    subengine *sub;
    u16 frame;
    s16 player;
    u16 flags;
    u16 occupied;
    u16 reserved;
    u16 mode;
    s16 timer;
    s16 old_player;
    u8 sfx;
} PogoSquare;
BASH_SIZE_CHECK(PogoSquare, 0x1C); /* the 8x8 grid at 0x800C53DC has rows of 0xE0 */

typedef struct {
    s32 player;
} PogoKerb;

typedef struct {
    u32 flags;
    u16 status;
    s16 timer;
    s16 offender;
    u32 spin;
    NAS *tazer;
    u16 forward_speed;
    u16 forward_frame;
    bool forward_quick;
} PogoPlayer;

typedef struct {
    bool going_clockwise;
    bool touched_floor;
    u32 offset, count, steps;
} PogoAIPlayer;
#ifdef BASH_HAVE_EXTNODE
typedef struct {
    s32 EXTid;
    EXTnode Extend;
    u16 Xvel;
    u16 Xmax;
    u16 Xacc;
    u16 Yvel;
    u16 Ymax;
    u16 Yacc;
    u16 delay[8];
    u16 timer[8];
    s16 angle[8];
    s16 dist[8];
    s16 ypos[8];
} EXTPogoShockwaveInfo;
#endif /* BASH_HAVE_EXTNODE */

void pogo_init(void);
void pogo_kill(void);
void POGO_InitHero(u32);
void POGOSetUp();
void InitialisePOGOFirst();
void InitialisePOGO();
void PogoInit_AIQuality(void);
void PogoInit_SetClock(u32);
void PogoInit_Camera(void);
void PogoInit_AllHeros(void);
void PogoInit_AllChallenges(void);
void PogoInit_CrystalChallenge(void);
void PogoInit_RelicChallenge(void);
void PogoInit_GemChallenge(void);
void PogoInit_TrophyChallenge(void);
void UpdatePOGO();
void PogoQuality_Init(void);
void PogoQuality_Update(void);
void POGOriggers(NASTrigger *, s32);
void FirstTriggersPOGO();
void PreMovePOGO(u32);
void PostMovePOGO(u32);
void PogoHeroChooseWinner(s32, u16);
void PogoWinner_Crystal(s32, u16);
void PogoWinner_CrystalOther(s32, u16);
void PogoWinner_Relic(s32, u16);
void PogoWinner_Gem(s32, u16);
void PogoWinner_Normal(s32, u16);
void PogoHero_ExtraEffects(s32);
void PogoCreateShadow(u32);
void SetHeroPogoIdle(u32);
void HeroPogoIdle(u32);
bool TestHeroPogo(u32 player);
void SetHeroPogo(u32);
void HeroPogo(u32);
bool TestHeroPogoDazed(u32);
void SetHeroPogoDazed(u32);
void HeroPogoDazed(u32);
void SetHeroPogoTazing(u32);
void HeroPogoTazing(u32);
void HeroPogoTazingCancel(u32);
bool TestHeroPogoWinner(u32);
void SetHeroPogoWinner(u32);
void HeroPogoWinner(u32);
bool TestHeroPogoLoser(u32);
void SetHeroPogoLoser(u32);
void HeroPogoLoser(u32);
bool TestHeroPogoTied(u32);
void SetHeroPogoTied(u32);
void HeroPogoTied(u32);
bool TestHeroPogoShoot(u32);
bool TestHeroPogoShootCancel(u32);
void Player_RemoveReserve(u32);
void Player_SetReserve(u32, s32, s32);
void Player_SnapToTile(u32);
void Player_RampToTile(u32);
s32 Player_CalcForwardSpeed(u32, s32);
s32 Player_CountTiles(u32);
void Player_SetDazed(u32);
void Player_SetPoisoned(u32);
bool Pickup_IsClaimable(s32, s32, s32);
bool Pickup_IsClaimableByAI(s32, s32, s32);
void AddDingoSmoke(u32);
void AddSomeDust(u32);
void AddBrioSmoke(u32);
void AddTazerRing(u32);
void Fire_Rocket(s32, s32);
void Fire_RocketFourWays(s32, s32);
void Fire_RocketInDir(s32, s32, s32, s32);
s32 Fire_Tazer(s32, s32);
void Init_PogoTazer(NAS *);
void Plot_PogoTazer(NAS *);
void Init_PogoPickup(NAS *);
void Plot_PogoPickup(NAS *);
void Init_PogoPowerup(NAS *);
void Plot_PogoPowerup(NAS *);
void Init_PogoBanker(NAS *);
void Plot_PogoBanker(NAS *);
void Init_PogoMushroom(NAS *);
void Plot_PogoMushroom(NAS *);
void InitPogo_Tiles(void);
void Update_Occupancy();
void Update_Tiles();
void Set_Tile_Under_Player(u32, bool);
bool SetTile_PogoThree(s32, s32, s32, bool);
bool SetTile_PogoFour(s32, s32, s32, bool);
void PogoFour_PlayerHasStoodOnOwnTile(s32);
void PogoFour_FreezeMyTiles(s32);
void PogoFour_UnfreezeMyTiles(s32);
void PlayBoingNoise(u32);
void PlayThrowNoise(u32);
void PlaySquareChangeNoise(u32);
void PlaySquareLockedNoise(u32);
s32 GetPogoContour(EXTengine *);
void Tile_Clear(s32, s32);
void Tile_Set(s32, s32, s32);
void Tile_SetLocked(s32, s32, s32);
void Tile_DelayedSet(s32, s32, s32, s32);
void Tile_Claim(s32, s32, s32);
void Tile_FizzleOut(s32, s32, s32, s32);
void Tile_DelayedFizzleOut(s32, s32, s32, s32);
void Tile_OnFire(s32, s32, s32, s32);
void Tile_Update(s32, s32);
void Tile_SetTnt(s32, s32);
void Tile_FireTnt(s32, s32);
void Tile_SetFourWay(s32, s32);
void Tile_SetNitro(s32, s32);
void Tile_FireNitro(s32, s32);
void Tile_FireAllNitros(void);
void Pickup_Claim(s32, s32, s32);
void Pickup_Lose(s32);
void PogoPickups_Poll(void);
void PogoPowerups_Poll(void);
void CreatePowerupOnTile(s32, s32, s32);
void CreatePickupOnTile(s32, s32, s32);
s32 Bank_Tiles(s32);
void Handle_Score(u32, s32, s32, s32);
void ZapArrow(s32, s32, s32, s32);
void ZapFourWays(s32, s32, s32);
void ZapInDir(s32, s32, s32, s32, s32);
void Tiles_PlayerIsStealing(s32, s32);
void Tiles_CheckForIslands(s32);
void Tiles_MakeUpIslandMap(s32 *, s32);
void Tiles_FloodFillIslandMap(s32 *, s32);
s32 Tiles_CountUnflagged(s32 *, s32, s32);
void Island_FloodFill(s32, s32, u32, s32 *);
s32 LevelSpecific_UnclaimedTile(s32, s32);
s32 LevelSpecific_ClaimedTile(s32, s32);
void Init_PogoRipperRoo(NAS *);
void Plot_PogoRipperRoo(NAS *);
bool Roo_ChangeDirOnEdge(void);
bool Roo_FindNewSquare(EXTengine *);
bool Roo_Bounce(NAS *);
void Roo_GoBoing(EXTengine *);
void Roo_UpdatePosition(EXTengine *);
void Roo_UpdateVelocities(EXTengine *);
void Roo_FireRocket(EXTengine *);
void Roo_ShootAtHumans(NAS *);
/* the leak also declares void Roo_FireRocket(EXTengine *, s32): func_800C02AC (pogo) */
bool SquareIsFreeForRoo(s32, s32);
void Create_PogoProjectile(s32, s32, s32, s32, s32, s32);
void Init_PogoProjectile(NAS *);
void Plot_PogoProjectile(NAS *);
void PogoKerb_Init(void);
void PogoKerb_CopyData(void);
void PogoKerb_Poll(void);
void PogoKerb_SetMode(s32);
void PogoAI_InitHero(u32);
void PogoAI_Poll(u32);
void PogoAI_HeroClaimedPickup(u32);
void PogoAI_MoveTowardsTarget(u32);
bool PogoAI_IsSquareOKForPlayer(u32, s32, s32, s32, s32, u32);
bool PogoAI_PrimeDirective(u32);
void PogoAI_ScanForClosestPickup(u32, s32 *, s32 *);
bool PogoAI_CanPlayerShootAnyone(u32);
bool PogoAI_CanPlayerShootAnyoneFourWays(u32);
bool PogoAI_CanPlayerShockAnyone(u32);
bool PogoAI_GoForMyOwn(u32, EXTbotai *);
bool PogoAI_StartEncircling(u32, EXTbotai *);
bool PogoAI_StartEnclosing(u32, EXTbotai *);
bool PogoAI_ClaimingThisTileWillEncircle(s32, s32, s32);
s32 PogoAI_CountNeighbours(s32, s32, s32);
s32 PogoAI_CountMyTiles(u32);
bool PogoAI_RadialSearch(u32, s32, s32, s32, s32 *, s32 *);
bool PogoAI_RadialSearch_CheckKerbs(s32, s32, s32);
u32 PogoDistanceToDestination(s32, s32, s32, s32);
u32 PogoSquaredDistanceToDestination(s32, s32, s32, s32);
bool FindFreeSquare(s32 *, s32 *);
bool FindFreeSquareForMushroom(s32 *, s32 *);
bool SquareContainsMushroom(s32, s32);
bool SquareIsFreeForMushroom(s32, s32);
void CreateFloatyPogoScore(u32, s32, s32, s32);
void Create_PogoPadlock(s32, s32);
void Init_PogoPadlock(NAS *);
void Plot_PogoPadlock(NAS *);
bool PadlockAI_IsTileOKForPadlock(NAS *, s32, s32);
bool PogoPadlock_IsHere(s32, s32);
extern PreMap POGO_PREMAP;
extern LevSpec POGOSpecifics;
extern shell_setup POGO_shell_info;
extern file_group_load_info file_group_POGOPAINTER;
extern file_group_load_info file_group_POGOAGOGO;
extern file_group_load_info file_group_POGOLOCO;
extern file_group_load_info file_group_POGOBOUNCER;
extern s32 number_of_pickups;
extern s32 number_of_powerups;
extern s16 pickup_spawn_time;
extern s16 powerup_spawn_time;
extern s32 number_of_banks;
extern PogoPlayer PogoPlay[];
#ifdef BASH_HAVE_HERO_SEQ
extern hero_seq CRA_seq_pogo_idle[];
extern hero_seq CRA_seq_pogo_quick_idle[];
extern hero_seq CRA_seq_pogo_tazing[];
extern hero_seq CRA_seq_pogo_dazed[];
extern hero_seq CRA_seq_pogo_winner[];
extern hero_seq COC_seq_pogo_idle[];
extern hero_seq COC_seq_pogo_quick_idle[];
extern hero_seq COC_seq_pogo_tazing[];
extern hero_seq COC_seq_pogo_dazed[];
extern hero_seq COC_seq_pogo_winner[];
extern hero_seq BRO_seq_pogo_idle[];
extern hero_seq BRO_seq_pogo_quick_idle[];
extern hero_seq BRO_seq_pogo_tazing[];
extern hero_seq BRO_seq_pogo_dazed[];
extern hero_seq BRO_seq_pogo_winner[];
extern hero_seq CTX_seq_pogo_idle[];
extern hero_seq CTX_seq_pogo_quick_idle[];
extern hero_seq CTX_seq_pogo_tazing[];
extern hero_seq CTX_seq_pogo_dazed[];
extern hero_seq CTX_seq_pogo_winner[];
extern hero_seq DNG_seq_pogo_idle[];
extern hero_seq DNG_seq_pogo_quick_idle[];
extern hero_seq DNG_seq_pogo_tazing[];
extern hero_seq DNG_seq_pogo_dazed[];
extern hero_seq DNG_seq_pogo_winner[];
extern hero_seq KAN_seq_pogo_idle[];
extern hero_seq KAN_seq_pogo_quick_idle[];
extern hero_seq KAN_seq_pogo_tazing[];
extern hero_seq KAN_seq_pogo_dazed[];
extern hero_seq KAN_seq_pogo_winner[];
extern hero_seq TIN_seq_pogo_idle[];
extern hero_seq TIN_seq_pogo_quick_idle[];
extern hero_seq TIN_seq_pogo_tazing[];
extern hero_seq TIN_seq_pogo_dazed[];
extern hero_seq TIN_seq_pogo_winner[];
extern hero_seq KON_seq_pogo_idle[];
extern hero_seq KON_seq_pogo_quick_idle[];
extern hero_seq KON_seq_pogo_tazing[];
extern hero_seq KON_seq_pogo_dazed[];
extern hero_seq KON_seq_pogo_winner[];
#endif /* BASH_HAVE_HERO_SEQ */

void psxpogo_init_tile(subengine *sub);
void psxpogo_init_tile_heap(s32 num_tiles);

#endif /* BASH_POGO_H */
