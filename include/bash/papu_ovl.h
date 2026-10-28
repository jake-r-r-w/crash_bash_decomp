#ifndef BASH_PAPU_OVL_H
#define BASH_PAPU_OVL_H

/* Functions and data defined in the papu overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/papu/papu.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/engine.h"

typedef struct PapuSpot {
    s32 x;
    s32 y;
    s32 z;
} PapuSpot;

extern PapuSpot PapuSpawnPoints[];

typedef struct PapuPos {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 z;
} PapuPos;

typedef struct PapuEntry {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
    /* 0xC */ s16 unkC;
    /* 0xE */ s16 unkE;
} PapuEntry;

extern s16 *PapuColumnHeights[];

extern PapuEntry PapuColumns[20];

/* The object extension (AddObject) InitPACrate fills. */
typedef struct PapuObj {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u8 pad16[0x1A - 0x16];
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u8 pad1E[0x22 - 0x1E];
    /* 0x22 */ s16 unk22;
} PapuObj;
extern s32 CrateAstType;
extern u16 CrateCounts[];
typedef struct PapuTile {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padC[0x74 - 0xC];
    u16 unk74;
} PapuTile;

typedef struct PapuCell {
    PapuTile *tile;
    s32 pad4;
    s16 pad8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    s16 unk12;
} PapuCell;

extern PapuCell PapuTiles[8][8];

/* A scene object in the list at MapScene->subs (PapuInitTiles). */
typedef struct PapuSceneObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 x;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 padA;
    /* 0x0C */ s32 z;
    /* 0x10 */ u8 pad10[0x24 - 0x10];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x50 - 0x28];
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad54[0x5C - 0x54];
    /* 0x5C */ struct PapuSceneObj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ u8 *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} PapuSceneObj;
extern PapuSceneObj *PapuFloorObj;
extern u16 PapuFloorState[40];
extern u16 PapuFloorDepth;
extern s16 PapuUnused4;
extern s16 PapuUnused5;
typedef struct PapuGridPos {
    s32 x;
    s32 y;
} PapuGridPos;
extern PapuGridPos PapuDropPoints[16][16];
extern s16 PapuPickupRates[];
extern u16 PapuPickupActive;
extern u16 PapuPickupDelay;
extern u16 PapuPickupTimer;
extern u16 PapuCrateTimer;
extern u16 PapuMaxCrates;
typedef struct {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 y;
    s16 pad1C;
    s16 h;
} T_B8080;

typedef struct Slot40 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 pad4[7];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34[3];
} Slot40;

extern Slot40 PapuMinionSlots[6];

typedef struct B5E44Flag {
    /* 0x00 */ s32 pad0[6];
    /* 0x18 */ s16 pad18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 flags;
} B5E44Flag;

typedef struct B5E44Obj {
    /* 0x00 */ s32 pad0[4];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 pad14;
    /* 0x18 */ s32 z;
    /* 0x1C */ s32 pad1C[10];
    /* 0x44 */ s32 pad44[5];
    /* 0x58 */ B5E44Flag *flag;
} B5E44Obj;

typedef struct B5E44Pos {
    /* 0x00 */ s32 pad0[4];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 pad14;
    /* 0x18 */ s32 z;
} B5E44Pos;

typedef struct B5E44Dyn {
    /* 0x00 */ s32 pad0[5];
    /* 0x14 */ s16 pad14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s32 pad18;
    /* 0x1C */ s32 pad1C;
    /* 0x20 */ s16 pad20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 pad24[3];
    /* 0x2A */ s16 unk2A;
} B5E44Dyn;

typedef struct B5E44Owner {
    /* 0x00 */ s32 pad0[21];
    /* 0x54 */ B5E44Pos *pos;
} B5E44Owner;

typedef struct B5E44Slot {
    /* 0x00 */ B5E44Obj *obj;
    /* 0x04 */ B5E44Pos *pos;
    /* 0x08 */ s32 pad8[3];
    /* 0x14 */ B5E44Dyn *dyn;
    /* 0x18 */ s32 pad18[10];
} B5E44Slot;

extern u16 PapuMinionSfx[];

typedef struct PapuKey {
    /* 0x00 */ u16 a;
    /* 0x02 */ u16 b;
    /* 0x04 */ u16 pad4;
    /* 0x06 */ u16 c;
    /* 0x08 */ u16 pad8[2];
    /* 0x0C */ u16 d;
    /* 0x0E */ u16 e;
    /* 0x10 */ u16 f;
    /* 0x12 */ u16 g;
} PapuKey;

extern PapuKey *PapuFloorWaves[];
extern PapuKey *PapuFloorWaveCur;
extern s16 PapuWaveFlag;

typedef struct PapuBossState {
    /* 0x00 */ NAS *nas;
    /* 0x04 */ EXTengine *eng;
    /* 0x08 */ subengine *sub;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ ScriptCursor *cursor;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ u16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} PapuBossState;

extern PapuBossState PapuBoss;
extern ScriptInit PapuBossScriptInit;
extern s16 PapuBossCount;
extern s16 PapuBossLast;
extern u16 PapuAttackPlan[];
extern u16 PapuBossRandomAnims[];
extern s16 PapuBossRates[];
extern s16 PapuBossRate;
extern u16 PapuAttackWave;
extern s32 PapuBossFlag;
extern s32 PapuFloorTimer;
extern s32 PapuFloorStarted;

void PapuStartFloorWave(s32 arg0);

typedef struct PapuMsg {
    s32 unk0;
    s32 unk4;
} PapuMsg;

PapuMsg *PapuFindDropPoint(u16 *arg0);
NAS *CreatePACrate(s32 a, s32 b, s32 c, s32 d, s32 e);
void PapuDropCrates(u16 arg0);
typedef struct PapuStep {
    s32 a;
    s32 b;
    s32 c;
    s16 d;
    s16 pad;
} PapuStep;

extern PapuStep *PapuMinionWaves[];
extern ScriptInit PapuMinionScriptInit;
NAS *CreatePapuMinion(s32 a, s32 b, s32 c, u16 d);

s32 PAPU_KeepInBounds(u8 *arg0);

void PapuStopCrates(void);

void PapuHeroCheckPit(s32 arg0);

s32 PapuFreeMinionSlot(void);

void InitialisePAPUFirst(void);
void InitialisePAPU(void);
void FirstTriggersPAPU(void);

void PapuRestartLevel(void);

void UpdatePAPU(void);

extern s32 CrateAstType2;
extern s32 *CrateHitShapes[];
extern s32 *CrateHitShapes2[];

/* Index of the ASTTable entry with this id, or 0. */
static inline s32 PapuASTIndex(s32 id) {
    s32 i;

    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == id) {
            return i;
        }
    }
    return 0;
}

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/papu_pahero.h"

void PapuInitMinionSlots(void);

s32 GetPapuContour(EXTengine *eng);

void PapuSpawnMinions(u16 arg0);

void InitPapuColumn(NAS *nas);

void PapuDropColumns(void);

void PapuMarkHoles(void);

void PAPUSetUp(void);

s32 PapuGridKey(T_B8080 *p);


void PapuSetColumns(u16 arg0);

void PapuMoveColumns(void);

s32 PapuHeroOverPit(s32 arg0);

s32 PapuPickTarget(void);

void PlotPapuColumn(NAS *nas);

void PapuInitCrateHooks(void);
extern u16 PickupCounts[13];
typedef struct {
    u8 pad0[0x14];
    u16 type;
} T_B80B0Fux;

typedef struct {
    u8 pad0[0xC];
    NAS *owner;
    u8 pad10[4];
    u16 type;
    u8 pad16[4];
    u16 status;
} T_B4B9CFux;
extern u16 PapuCrateMixPos;
extern u16 PapuFirstCrates;
extern u16 PapuDropPos;
extern u8 PapuCrateMix[];
extern u16 PapuCrateCounts[4];

void PapuDropFirstCrates(void);

void InitPACrate(NAS *arg0);

NAS *PapuHero2ObjectCollision(s32 player, s32 mask, s32 unused, s32 cone);
NAS *PapuHeroGrabObject(s32 idx, s32 arg1);
s32 PapuHeroKickCollision(s32 arg0);
s32 PapuHeroCarryCollision(s32 idx);
extern u16 PapuAiSkill[];
extern u16 PapuAiSkillAlt[];

s32 PapuPickSpawnPoint(PapuPos *arg0);
extern u8 DirPadBits[];

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 pad14;
    s32 z;
    s16 pad1C;
    s16 ang;
} T_B7F14_A;

typedef struct {
    u8 pad0[0xD];
    u8 dir;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 pad14;
    s16 unk16;
    u8 pad18[0x12];
    s16 unk2A;
} T_B7F14_B;

void PapuCrateMaster(void);

void PapuShuffleDropPoints(void);

void InitPapuBoss(NAS *nas);

NAS *PapuMinionFindCrate(s32 arg0);

void PapuExploRange(B5E44Owner *arg0, s32 arg1, s32 arg2);

void *PapuMinionObjectAhead(s32 arg0, s32 arg1);

s32 PapuHitHeroes(EXTengine *eng);

s32 PapuHitHeroesFlag(EXTengine *eng, u32 flag);

NAS *PapuHitSolids(EXTengine *eng, u32 mask, s32 unused, s32 add);

NAS *PapuHitObjects(EXTengine *eng, s32 mask);

void PapuInitTiles(void);

void PapuFloorWave(void);

void PACrateAimAtGrid(T_B7F14_A *a, T_B7F14_B *b);

void PapuCountObjects(void);

extern s16 PapuUnused0;
extern EXTengine PapuSfxEngine;
extern s16 PapuFloorSink;
extern u16 PapuAttackPos;
extern u16 PapuAttackDelay;

s32 PapuMinionKick(u32 idx);

extern u16 PapuTileModels[];

void PlotPapuBoss(NAS *nas);

void PapuUpdateFloor(void);
/* PlotPapuMinion's FUX extension. */
typedef struct MinionFux {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ NAS *target;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 pad18;
    /* 0x1A */ u16 state;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
} MinionFux;

/* PlotPapuMinion's 0x800 extension (movement). */
typedef struct MinionMove {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 speed;
    /* 0x10 */ u8 pad10[0x16 - 0x10];
    /* 0x16 */ s16 angle;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 pad20;
    /* 0x22 */ s16 push;
    /* 0x24 */ u8 pad24[0x2A - 0x24];
    /* 0x2A */ s16 push_angle;
} MinionMove;

extern s32 GameTeams[];
extern u16 HeroSquashCycle[];
extern u16 HeroSpinScale[];

void PapuSetCrateMix(u16 arg0, s32 a1, s32 a2, s32 a3, u16 n4, u16 n5, u16 n6, u16 n7, u16 n8);

void PlotPapuMinion(NAS *nas);


#endif /* BASH_PAPU_OVL_H */
