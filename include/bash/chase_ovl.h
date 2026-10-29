#ifndef BASH_CHASE_OVL_H
#define BASH_CHASE_OVL_H

/* Functions and data defined in the chase overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/chase/chase.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/aiskill.h"
#include "bash/core.h"   /* map_enviroment (MapScene) */



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

extern u16 ChaseHeroShadowModels[];
extern u16 ChaseHeroShadowSizes[];
extern s32 ChaseFlameTimer[];

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
void ChaseHeroJump(s32 arg0);
void ChaseUnusedStub0(void);
void ChaseUnusedStub1(void);
s32 ChaseNoKick(void);
s32 ChaseAlwaysOne(void);
void FirstTriggersCHASE(void);
void ChaseTriggers(void);
void ChaseStopHeroSfx(void);
void ChaseResetSlot(s32 arg0);
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
extern s32 ChaseGrid[];

/* Per-level spawn offsets read by ChaseSpawnCoal: radius, angle, height. */
typedef struct ChaseSpawnOfs {
    s32 r;
    s32 ang;
    s32 y;
} ChaseSpawnOfs;
extern ChaseSpawnOfs ChaseSpawnOffsets[];

extern NAS *ChasePickup[];
extern NAS *ChaseBarrels[8];
extern s32 ChaseBarrelCount;
NAS *ChaseFindPickupNear(s32 x, s32 z, s32 range);
extern s32 ChaseRoundOver;
extern s32 ChaseArenaRadius;
extern s32 ChaseArenaShrink;
extern s32 ChaseHitSfx[][4];
void ChaseHidePowerup(s32 arg0);

void ChaseChooseWinner(s32 arg0, s32 arg1);
void ChaseChooseWinnerScore(s32 arg0, u16 arg1);
void ChaseChooseWinnerScoreB(s32 arg0, u16 arg1);
void ChaseChooseWinnerTimed(s32 arg0, u16 arg1);
void ChaseChooseWinnerRank(s32 arg0, u16 arg1);

#define RANK68V ((GameState *)GameInfo)


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

void ChaseHeroWinner(s32 arg0);


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
extern s32 ChaseChainSfx[];
static inline HeroInfo *ChaseHeroes(void) {
    return Heroes;
}
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
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
} ChaseAiState;


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
            d = x - eng->XYZ.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (z - eng->XYZ.z) < 0xF00) {
                return nas;
            }
        }
    }
    return NULL;
}


void PlotEgg(NAS *nas);

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


#endif /* BASH_CHASE_OVL_H */
