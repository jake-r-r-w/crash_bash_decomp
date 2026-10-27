#ifndef BASH_CHASE_OVL_H
#define BASH_CHASE_OVL_H

/* Functions and data defined in the chase overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/chase/chase.c */

#include "common.h"
#include "bash/nas.h"

typedef struct { s16 unk0; s16 unk2; } ChaseTrig;
extern ChaseTrig rcossin_tbl[];

extern NAS *D_8005AA9C;
extern s32 VelocityCalc;

typedef struct CoalFlameInner {
    s32 pad0[3];
    s32 unkC;
} CoalFlameInner;

typedef struct CoalFlameObj {
    u8 pad0[0x54];
    CoalFlameInner *unk54;
} CoalFlameObj;

typedef struct CoalFlameExt {
    EXT ext;
    u8 pad0C[0x10];
    s32 unk1C;
    u8 pad20[0x24];
} CoalFlameExt;

typedef struct CircleFireExt {
    EXT ext;
    u8 pad0C[0x24];
    s32 unk30;
    u8 pad34[0x10];
} CircleFireExt;

extern u8 HeroModelBank[];
extern u8 KegObjectsModel[];
extern s32 *MapScene;

typedef struct EggExt {
    EXT ext;
    u8 pad0C[4];
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    u8 pad20[0x10];
    s32 unk30;
    u8 pad34[0x10];
} EggExt;

extern u32 Clock_Time;
extern u16 GameExitFlags;

/* Extension 0x10000000 of the NAS 0x1605 puff PlotEgg spawns. */
typedef struct EggPuffExt {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x20 - 0x1C];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x28 - 0x24];
    /* 0x28 */ s32 unk28;
} EggPuffExt;

/* PadState (Pads) as ChasePadHook reads it: flags at 0x54. */
typedef struct ChasePad {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x34];
    /* 0x38 */ u16 unk38;
    /* 0x3A */ u8 pad3A[0x16];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} ChasePad;
extern u16 ChaseHeroShadowModels[];
extern u16 ChaseHeroShadowSizes[];
extern s32 ChaseFlameTimer[];
extern ChasePad Pads[];
extern s32 D_8005A670;

typedef struct CoalPosInner {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 z;
} CoalPosInner;

typedef struct CoalGridObj {
    u8 pad0[0x54];
    CoalPosInner *pos;
} CoalGridObj;

extern u32 DirPadBits[];
extern s32 HeroesAllOut;

typedef struct ChaseOut {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} ChaseOut;
typedef struct CoalSfxInner {
    u8 pad0[0x10];
    SLONG_XYZ pos;
    u8 pad1C[0x94 - 0x10 - sizeof(SLONG_XYZ)];
    _sfx_handler *voice;
} CoalSfxInner;

typedef struct CoalSfxObj {
    u8 pad0[0x54];
    CoalSfxInner *unk54;
    u8 pad58[0xC];
    s32 idx;
} CoalSfxObj;

extern s32 ChaseFlameState[];
extern s32 ChaseFlameSfxHandle[];
extern s32 ChaseFlameObj[];
extern s32 ChaseHeroSpeed[];
extern s32 ChaseFlameSfx[];
void PlotCoalFlameSFX(CoalSfxObj *obj);

void ChaseAimAt(s32 arg0, s32 arg1, s32 arg2, EXTengine *arg3, ChaseOut *arg4);

typedef struct CoalSub {
    u8 pad0[0x74];
    u16 unk74;
} CoalSub;

typedef struct CoalEng {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    u16 unk1C;
    u8 pad1E[0x5E - 0x1E];
    u16 unk5E;
    u8 pad60[0x6C - 0x60];
    CoalSub *sub;
} CoalEng;

typedef struct CoalNAS {
    u8 pad0[0x54];
    CoalEng *eng;
    u8 pad58[4];
    u16 unk5C;
    u16 unk5E;
    s32 unk60;
} CoalNAS;

void PlotCoal(CoalNAS *nas);
void ChaseKillGridObj(CoalGridObj *obj);
typedef struct ChasePos {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
} ChasePos;

typedef struct ChaseObj {
    u8 pad0[0x54];
    ChasePos *pos;
} ChaseObj;

void ChaseUpdateStub(s32 *arg0);
extern s32 ChaseUpdateState[];
void func_80029100(void *arg0);
void ChaseHeroJump(s32 arg0);
void ChaseUnusedStub0(void);
void ChaseUnusedStub1(void);
s32 ChaseNoKick(void);
s32 ChaseAlwaysOne(void);
void FirstTriggersCHASE(void);
void ChaseTriggers(void);
void ChaseStopHeroSfx(void);
void ChaseResetSlot(s32 arg0);
extern LevSpec LevelSpecs; /* the level's hooks */
void InitialiseCHASEFirst(void);
void InitialiseCHASE(void);
void UpdateCHASE(void);

/* InitPickupCH's extension 0x10000006 (0x44 bytes), its NAS.FUX. */
typedef struct PickupCHExt {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 unk18[0x44 - 0x18];
} PickupCHExt;

extern u8 ChaseHeroSlots[];
extern s32 ChaseHeroState[][21];
extern u8 PowerupsModel[];
extern s32 ChaseGrid[];

/* Per-level spawn offsets read by ChaseSpawnCoal: radius, angle, height. */
typedef struct ChaseSpawnOfs {
    s32 r;
    s32 ang;
    s32 y;
} ChaseSpawnOfs;
extern ChaseSpawnOfs ChaseSpawnOffsets[];

/* AiPlayerSkill (declared in bash/pause.h, which this unit does not include). */
typedef struct ChaseOdds {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} ChaseOdds;
extern ChaseOdds AiPlayerSkill[];
extern NAS *ChasePickup[];
extern NAS *ChaseBarrels[8];
extern NAS *ChaseBarrels[];
extern s32 ChaseBarrelCount;
NAS *ChaseFindPickupNear(s32 x, s32 z, s32 range);
extern s32 ChaseGrid[];
extern s32 ChaseRoundOver;
extern s32 ChaseArenaRadius;
extern s32 ChaseArenaShrink;
extern s32 ChaseHitSfx[][4];
void ChaseHidePowerup(s32 arg0);
extern s16 HeroActHit;
extern u8 HeroModelBank[];
extern u16 CameraMode;

void ChaseChooseWinner(s32 arg0, s32 arg1);
void ChaseChooseWinnerScore(s32 arg0, u16 arg1);
void ChaseChooseWinnerScoreB(s32 arg0, u16 arg1);
void ChaseChooseWinnerTimed(s32 arg0, u16 arg1);
void ChaseChooseWinnerRank(s32 arg0, u16 arg1);
extern u8 GameInfo[];

/* GameInfo as seen by ChaseChooseWinnerRank (the final ranking). */
typedef struct ChaseRankView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32[0x48 - 0x32];
    /* 0x48 */ s16 team[2];
    /* 0x4C */ u8 unk4C[0x60 - 0x4C];
    /* 0x60 */ u8 winner[2];
    /* 0x62 */ u8 unk62[0x6C - 0x62];
    /* 0x6C */ s32 teams;
    /* 0x70 */ u8 unk70[0xA7 - 0x70];
    /* 0xA7 */ u8 order[4];
} ChaseRankView;
#define RANK68V ((ChaseRankView *)GameInfo)

/* Hero jump parameters, 8 bytes per hero (HeroHooks.unk10). */
typedef struct ChaseJumpEnt {
    u16 vel, max, acc, dec;
} ChaseJumpEnt;

/* HeroHooks (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct ChaseHooks {
    /* 0x00 */ void *unk0;
    /* 0x04 */ u16 *unk4;
    /* 0x08 */ u8 pad8[0x4];
    /* 0x0C */ u16 *unkC;
    /* 0x10 */ ChaseJumpEnt *unk10;
    /* 0x14 */ void *unk14;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ void *unk2C;
    /* 0x30 */ u8 pad30[8];
    /* 0x38 */ void *unk38;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ void *unk40;
    /* 0x44 */ void *unk44;
    /* 0x48 */ void *unk48;
    /* 0x4C */ u8 pad4C[8];
    /* 0x54 */ void *unk54;
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ void *unk5C;
    /* 0x60 */ u8 pad60[4];
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
} ChaseHooks;
extern ChaseHooks HeroHooks;
extern void *AiBotTweakTable;
extern void *AiHumanTweakTable;
extern void *AiSkillTable;
extern void *AiTeamSkillTable;
extern void *AiHumanRoundTweakTable;
extern s16 HeroActDead;
extern s16 HeroActWinner;
extern s16 HeroActLoser;
extern s32 GameTeams;
extern u16 ChaseHeroModels[];
extern s32 ChaseVictorySeqs[]; /* {name, id} pairs: the victory sequences (the engine's hook 0x14) */
extern u16 ChaseAiSkill[];
extern u16 ChaseAiSkillAlt[];
extern u16 ChaseAiTeamSkill[];
extern u16 ChaseAiHumanRoundTweak[];
extern u16 ChaseAiBotTweak[];
extern u16 ChaseAiHumanTweak[];
void Chase_HeroBot(s32 arg0);
void ChaseInitHero(s32 arg0);
void ChasePreMoveHero(s32 arg0);
void ChaseMoveHero(s32 arg0);
s32 ChaseHeroKickCollision();

/* Extension 0x1000 flagged by ChaseHeroKickCollision. */
typedef struct ChaseHitFlags {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} ChaseHitFlags;
extern NAS *HeadOfSolids;
void func_8001CD04(void *arg0, void *arg1, void *arg2);
void func_800142D4(void *arg0);
void func_80015758(SVECTOR *v, VECTOR *out);
s32 Random(s32 n);
void *FindExtension(NAS *nas, s32 id);
s32 get_angle(s32 x, s32 z);
FlagRecord *func_80025A48(EXTengine *eng, u32 flag);
s32 csqrt(s32 x);

void ChaseHeroWinner(s32 arg0);
extern s32 World_floor_level;
extern s32 D_8005B5B8;
extern HeroInfo Heroes[4];
void func_80025F44(EXTengine *eng);
void func_80084E04(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);

s32 func_80085890();
s32 func_8008A200(s32 arg0);
void func_8008A810(void);

void ChaseAI_AngleTo(s32 arg0, EXTengine *arg1);

void FinishChaseLevel(void);

s32 CHASE_KeepInBounds(s32 *p);

s32 ChaseHeroTestDrop(s32 arg0);

void SetChaseHeroJump(s32 arg0);

void SetChaseHeroFall(s32 arg0);

void ChaseHeroFall(s32 arg0);

void SetChaseHeroLand(s32 arg0);

void ChaseHeroLand(s32 arg0);

void SetChaseHeroHit(s32 arg0);

void ChaseHeroHit(s32 arg0);

void SetChaseHeroDie(s32 arg0);

void SetChaseHeroDead(s32 arg0);

void ChaseHeroDead(s32 arg0);

void ChaseHeroLose(s32 arg0);

void InitBarrelCH(NAS *nas);

/* InitBarrelCH's extension 0x10000001 (0x44 bytes), its NAS.FUX. */
typedef struct BarrelCHExt {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 scale;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ s32 growing;
} BarrelCHExt;
extern s32 ChaseBarrelSfx[];
void PlotBarrelCH(NAS *nas);

void InitPickupCH(NAS *nas);

void KillPickupCH(NAS *nas);

void InitPUGenerator_CH(NAS *nas);

void ChaseHero_ExtraEffects(s32 arg0);

void ChaseHeroHitFlash(s32 arg0, s32 arg1);

void InitCoalFlameSFX(CoalFlameObj *arg0);

void InitEgg(NAS *nas);

void InitCircleFire(NAS *nas);

void InitCoalFlame(NAS *nas);

void InitCoal(NAS *nas);

void KillPickupCHB(NAS *nas);

void InitBarrelGenerator(NAS *nas);

void ChaseHeroDie(s32 arg0);

void ChaseHeroDrop(s32 arg0);

void SetChaseHeroRun(s32 arg0);

void SetChaseHeroIdle(s32 arg0);

s32 ChaseTestEnd(s32 arg0);

s32 ChaseHeroTestJump(s32 arg0);

s32 ChaseHeroToIdle(s32 arg0);

s32 ChaseHeroTestRun(s32 arg0);

void ChaseHitHero(s32 arg0);

s32 ChaseFindHero(s32 x, s32 z, s32 range);

s32 ChasePadHook(s32 arg0);

void ChaseClampToArena(s32 *p, s32 arg1);

void CHASESetUp(void);

void ChaseGridClear(s32 x, s32 y);

s32 ChaseGridAt(s32 x, s32 z);

s32 ChaseFindHeroNear(s32 x, s32 z, s32 range, s32 skip);

s32 ChaseFindHeroAt(s32 x, s32 z, s32 range);
/* PlotCircleFire: offsets of the 8 fire points around the circle, and the
 * 40x40 grid of fire NASes (128 units a cell, centred on 0). */
typedef struct { s32 x; s32 z; } CircleFireOfs;
extern CircleFireOfs CircleFireOffsets[8];
NAS *CreateNAS(s32 x, s32 y, s32 z, u16 id, u16 flags);
s32 KillNAS(NAS *nas);
typedef struct { u8 pad0[0x2C]; s32 unk2C; } CircleFireFux;
void PlotCircleFire(NAS *nas);

static inline s32 CircleFireIndex(s32 x, s32 z) {
    s32 col = (x + 0xA00) >> 7;
    s32 row = (z + 0xA00) >> 7;

    return row * 40 + col;
}
#define CIRCLEFIRE_CELL(x, z) CircleFireIndex(x, z)

static inline NAS *CircleFireCell(s32 x, s32 z) {
    if (x >= -0x9FF && x < 0xA00 && z >= -0x9FF) {
        return (NAS *)ChaseGrid[CIRCLEFIRE_CELL(x, z)];
    }
    return NULL;
}

static inline NAS *CircleFireGridCell(s32 *grid, s32 x, s32 z) {
    if (x >= -0x9FF && x < 0xA00 && z >= -0x9FF) {
        return (NAS *)grid[CIRCLEFIRE_CELL(x, z)];
    }
    return NULL;
}

void SetChaseHeroDrop(s32 arg0);

/* CoalFlameSpread: coal flame spreading over the circle-fire grid. */
typedef struct { u8 pad0[0x10]; s32 x; s32 pad14; s32 z; } CoalFlamePos;
typedef struct {
    u8 pad0[0x1C]; s32 unk1C; s32 unk20; s32 unk24; s32 unk28; s32 unk2C;
} CoalFlameFux;
typedef struct {
    u8 pad0[0x54]; CoalFlamePos *pos; CoalFlameFux *fux; u16 unk5C;
} CoalFlameCell;
typedef struct {
    u8 pad0[0x10]; s32 x; s32 pad14; s32 z; u8 pad1C[0x10];
    s32 sx; s32 sy; s32 sz; u8 pad38[0x26]; s16 unk5E;
    u8 pad60[0x34]; _sfx_handler *sfx;
} CoalFlameEng;
s32 CoalChainHit(s32 x, s32 z, s32 id, s32 cur, s32 arg4, s32 arg5);
/* CoalChainHit: the coal NASes in ChaseBarrels it ignites. */
typedef struct { u8 pad0[0x30]; s32 unk30; } CoalChainFux;
typedef struct {
    u8 pad0[0x54]; CoalEng *eng; CoalChainFux *fux; u16 unk5C;
} CoalChainNAS;
typedef struct { u8 pad0[0x32]; u16 flags; u8 pad34[0x6C - 0x34]; s32 unk6C; } CoalChainGame;
extern s32 ChaseChainSfx[];
static inline HeroInfo *ChaseHeroes(void) {
    return Heroes;
}
void func_8008D5D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
/* ChaseHeroSlots: one 0x30-byte record per player (id). */
typedef struct { u8 pad0[0x20]; s32 unk20; u8 pad24[0xC]; } CoalPlayerRec;
void ChaseHeroIdle(s32 arg0);

void ChaseHeroRun(s32 arg0);

void SetChaseHeroWinner(s32 arg0);

void SetChaseHeroLose(s32 arg0);

void PlotBarrelGenerator(NAS *nas);

NAS *ChaseSpawnCoal(s32 arg0);

void CoalFlameSpread(CoalFlameEng *eng, CoalFlameFux *fux, s32 id);

void PlotCoalFlame(NAS *nas);

void PlotPUGenerator_CH(NAS *nas);

void PlotPickupCH(NAS *nas);

void ChaseHitHeroesNear(s32 x, s32 z, s32 skip, s32 range);

s32 ChaseDist(ChaseObj *a, ChasePos *b);

/* ChaseAI_ChooseTarget: a CPU player's target choice (its third argument). */
typedef struct ChaseAiState {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 timer;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 z;
    /* 0x14 */ s32 angle;
    /* 0x18 */ NAS *target;
    /* 0x1C */ NAS *pickup;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
} ChaseAiState;

extern s32 ASTCount;
extern ASTEntry *ASTTable;
extern s32 GamePause;

/* Index of the ASTTable entry with this id, or 0. */
static inline s32 ChaseASTIndex(s32 id) {
    s32 i;

    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == id) {
            return i;
        }
    }
    return 0;
}
typedef struct { u8 pad0[0x40]; s32 owner; } ChaseCoalFux;
void ChaseAI_ChooseTarget(s32 arg0, EXTengine *arg1, ChaseAiState *arg2);

/* ChaseAI_ChooseTarget: the first pickup in ChasePickup near (x, z), or NULL. */
static inline NAS *ChaseNearPickup(s32 x, s32 z) {
    s32 i;
    s32 d;
    NAS *nas;
    EXTengine *eng;

    for (i = 0; i < 1; i++) {
        nas = ChasePickup[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (z - eng->unk10.z) < 0xF00) {
                return nas;
            }
        }
    }
    return NULL;
}

extern u16 Clock_flags;

void PlotEgg(NAS *nas);

/* InitialiseCHASE: the camera (MainCamera) and screen (GameViewport) it sets up. */
typedef struct ChaseCam {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x54 - 0x1C];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
} ChaseCam;
typedef struct ChaseScreen {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} ChaseScreen;
/* Per-character pair at ChaseRunSpeeds, indexed by hero_name. */
typedef struct ChasePct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
} ChasePct;
extern u16 ChaseRunSpeeds[];
extern NAS *ChaseGenerator;
extern NAS *ChaseGeneratorB;
extern s16 D_8009D88E;
extern s32 PanelCtrl;
extern u8 GameViewport[];
extern u8 MainCamera[];
void Init_Clock(s32 arg0, s32 arg1, s32 arg2);
void Set_Clock(u16 time, u16 flags);
void InitSharedScoreIcons(void);

#endif /* BASH_CHASE_OVL_H */
