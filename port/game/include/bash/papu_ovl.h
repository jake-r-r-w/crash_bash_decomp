#ifndef BASH_PAPU_OVL_H
#define BASH_PAPU_OVL_H

/* Functions and data defined in the papu overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/papu/papu.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/papu.h"   /* PapuSquare, PapuFloor_info, PapuMinion_info */
#include "bash/engine.h"

typedef struct PapuSpot {
    s32 x;
    s32 y;
    s32 z;
} PapuSpot;

extern PapuSpot PapuSpawnPoints[];

typedef struct PapuPos {
    u8 pad0[offsetof(EXTengine, XYZ.x)];
    s32 x;
    u8 pad14[offsetof(EXTengine, XYZ.z) - (offsetof(EXTengine, XYZ.x) + 4)];
    s32 z;
} PapuPos;

typedef struct PapuEntry {
    /* 0x0 */ void *flag;  /* tested for zero */
    /* 0x4 */ s16 pad4;
    /* 0x6 */ s16 flag2;  /* tested for zero */
    /* 0x8 */ s16 y;
    /* 0xA */ s16 vel;
    /* 0xC */ s16 limit;  /* vel turns at it */
    /* 0xE */ s16 accel;
} PapuEntry;

extern s16 *PapuColumnHeights[];

extern PapuEntry PapuColumns[20];

typedef struct PapuTile {
    s32 flags;
    u8 pad4[offsetof(subengine, XYZ.y) - (offsetof(subengine, flags) + 4)];
    s32 y;  /* raised and lowered with the floor's wave */
    u8 padC[offsetof(subengine, entity) - (offsetof(subengine, XYZ.y) + 4)];
    u16 unk74;
} PapuTile;


extern PapuSquare PapuTiles[8][8];

/* A scene object in the list at MapScene->subs (PapuInitTiles). */
typedef struct PapuSceneObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 x;
    /* 0x08 */ s16 yoffset;  /* the tile's yoffset */
    /* 0x0A */ u8 padA[offsetof(subengine, XYZ.z) - (offsetof(subengine, XYZ.y) + 2)];
    /* 0x0C */ s32 z;
    /* 0x10 */ u8 pad10[offsetof(subengine, xform.scale.vy) - (offsetof(subengine, XYZ.z) + 4)];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[offsetof(subengine, xform.hasScale) - (offsetof(subengine, xform.scale.vy) + 4)];
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad54[offsetof(subengine, next) - (offsetof(subengine, xform.dirty) + 2)];
    /* 0x5C */ struct PapuSceneObj *next;
    /* 0x60 */ u8 pad60[offsetof(subengine, u6C.ptr) - (offsetof(subengine, next) + sizeof(void *))];
    /* 0x6C */ u8 *model;
    /* 0x70 */ u8 pad70[offsetof(subengine, entity) - (offsetof(subengine, u6C.ptr) + sizeof(void *))];
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

typedef struct Slot40 {
    /* 0x00 */ void *flag;  /* tested for zero */
    /* 0x04 */ void *pad4[7];  /* [0] engine, [1] sub, [4] motion, [5] cursor, [6] target */
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 whatami;  /* its object's whatami */
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 flag2;  /* tested for zero */
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 pos[3];
} Slot40;

extern Slot40 PapuMinionSlots[6];

typedef struct B5E44Flag {
    /* 0x00 */ u8 pad0[offsetof(EXTobject, owner)];
    /* 0x18 */ s16 pad18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 flags;
} B5E44Flag;

typedef struct B5E44Obj {
    /* 0x00 */ u8 pad0[offsetof(NAS, FUX)];
    B5E44Flag *flag;
} B5E44Obj;

typedef struct B5E44Pos {
    /* 0x00 */ u8 pad0[offsetof(EXTengine, XYZ)];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 pad14;
    /* 0x18 */ s32 z;
} B5E44Pos;


typedef struct B5E44Owner {
    /* 0x00 */ u8 pad0[offsetof(NAS, Engine)];
    /* 0x54 */ B5E44Pos *pos;
} B5E44Owner;


extern u16 PapuMinionSfx[];


extern PapuFloor_info *PapuFloorWaves[];
extern PapuFloor_info *PapuFloorWaveCur;
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
    /* 0x28 */ u16 flag;  /* tested for zero */
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
    s32 a;
    s32 b;
} PapuMsg;

PapuMsg *PapuFindDropPoint(u16 *arg0);
NAS *CreatePACrate(s32 a, s32 b, s32 c, s32 d, s32 e);
void PapuDropCrates(u16 arg0);

extern PapuMinion_info *PapuMinionWaves[];
extern ScriptInit PapuMinionScriptInit;
NAS *CreatePapuMinion(s32 a, s32 b, s32 c, u16 d);

s32 PAPU_KeepInBounds(EXTengine *arg0);

void PapuStopCrates(void);

void PapuHeroCheckPit(s32 arg0);

s32 PapuFreeMinionSlot(void);

void InitialisePAPUFirst(void);
void InitialisePAPU(void);
void FirstTriggersPAPU(void);

void PapuRestartLevel(void);

void UpdatePAPU(void);


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

s32 PapuGridKey(EXTengine *p);


void PapuSetColumns(u16 arg0);

void PapuMoveColumns(void);

s32 PapuHeroOverPit(s32 arg0);

s32 PapuPickTarget(void);

void PlotPapuColumn(NAS *nas);

void PapuInitCrateHooks(void);
typedef struct {
    u8 pad0[offsetof(EXTobject, whatami)];
    u16 type;
} T_B80B0Fux;

typedef struct {
    u8 pad0[offsetof(EXTobject, target)];
    NAS *owner;
    u8 pad10[offsetof(EXTobject, whatami) - (offsetof(EXTobject, target) + sizeof(void *))];
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

NAS *PapuHero2ObjectCollision(s32 player, s32 mask, s32 unused, u16 cone);
NAS *PapuHeroGrabObject(s32 idx, u16 arg1);
s32 PapuHeroKickCollision(s32 arg0);
s32 PapuHeroCarryCollision(s32 idx);
extern u16 PapuAiSkill[];
extern u16 PapuAiSkillAlt[];

s32 PapuPickSpawnPoint(PapuPos *arg0);



void PapuCrateMaster(void);

void PapuShuffleDropPoints(void);

void InitPapuBoss(NAS *nas);

NAS *PapuMinionFindCrate(s32 arg0);

void PapuExploRange(B5E44Owner *arg0, s32 arg1, s32 arg2, s32 arg3);

void *PapuMinionObjectAhead(s32 arg0, s32 arg1);

s32 PapuHitHeroes(EXTengine *eng);

s32 PapuHitHeroesFlag(EXTengine *eng, u32 flag);

NAS *PapuHitSolids(EXTengine *eng, u32 mask, s32 unused, s32 add);

NAS *PapuHitObjects(EXTengine *eng, s32 mask);

void PapuInitTiles(void);

void PapuFloorWave(void);

void PACrateAimAtGrid(EXTengine *a, EXTmotion *b);

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



void PapuSetCrateMix(u16 arg0, s32 a1, s32 a2, s32 a3, u16 n4, u16 n5, u16 n6, u16 n7, u16 n8);

void PlotPapuMinion(NAS *nas);


#endif /* BASH_PAPU_OVL_H */
