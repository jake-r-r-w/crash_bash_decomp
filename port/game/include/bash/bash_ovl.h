#ifndef BASH_BASH_OVL_H
#define BASH_BASH_OVL_H

/* Functions and data defined in the bash overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/bash/bash.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/gameflow.h"
#include "bash/xtras.h"

struct subengine;

/* A board cell of BashSpaceTiles (0xC bytes, 8x8). */
typedef struct BashCell {
    /* 0x00 */ struct subengine *obj;
    /* 0x04 */ struct subengine *obj2;
    /* 0x08 */ u16 flags; /* flags */
    /* 0x0A */ u16 flag;  /* tested for zero */
} BashCell;

extern BashCell BashSpaceTiles[8][8];


#define OVLG ((GameState *)GameInfo)

/* Index of the ASTTable entry with this id, or 0. */
static inline s32 BashASTIndex(s32 id) {
    s32 i;

    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == id) {
            return i;
        }
    }
    return 0;
}


/* The AI state BashAI_CheckShot writes its chosen target into. */
typedef struct {
    /* 0x00 */ u8 pad0[offsetof(EXTbotai, TargetPlayer)];
    /* 0x10 */ s32 target;
    /* 0x14 */ u8 pad14[offsetof(EXTbotai, AImode) - (offsetof(EXTbotai, TargetPlayer) + 4)];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ u8 pad22[offsetof(EXTbotai, AIpad) - (offsetof(EXTbotai, AImode) + 2)];
    /* 0x2C */ s32 flags;
} BashAiState;

s32 BashAI_ChooseTarget(s32 player);
extern u16 BashAIHealthWeights[];      /* BashAI_ChooseTarget: weight by hero_health (0..20) */
extern SLONG_XYZ BashAITargetPoints[]; /* BashAI_ChooseTarget: grid points, 7..15 scanned */

void FinishBashLevel(void);

/* Bash_HeroBot: the bot AI tick. */
void BashAI_ScanAhead(s32 arg0);
extern u16 BashAIRange[];

extern u16 BashFirstCrates;
extern ScriptInit BoingScriptInit;
extern ScriptInit PenguinScriptInit;
extern EXTengine *BashPenguinEng;
extern EXTobject *BashPenguinObj;
void BashSnowMarkPenguin(void);

void BashDropCrates(u16 arg0);

typedef struct {
    /* 0x00 */ s32 a;
    /* 0x04 */ s32 b;
} BashPair;

extern u8 BashCrateMix[];
extern u16 BashCrateMixPos;
extern u16 BashDropPos;
extern u16 BashMaxNitro[];
extern u16 BashMaxCrates[];
BashPair *BashFindDropPoint(u16 *arg0);


typedef struct BashTumbler {
    /* 0x00 */ u8 pad00[offsetof(EXTengine, XYZ.x)];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 y;
    /* 0x18 */ s32 z;
    /* 0x1C */ u8 pad1C[offsetof(EXTengine, xform.rot.vy) - (offsetof(EXTengine, XYZ.z) + 4)];
    /* 0x1E */ s16 angle;
} BashTumbler;


void PlotBoing(NAS *arg0);

s32 BASH_KeepInBounds(EXTengine *arg0);

/* Extension 0x20000002: one entry every 2 bytes, one per subengine. */
typedef struct SnowGlint {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ u16 on[10];
    /* 0x20 */ u16 timer[10];
} SnowGlint;

/* Extension 0x20000000 (BashShox): u16 per subengine. */
typedef struct BashShoxExt {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ u16 a[8];
    /* 0x28 */ u16 b[16];
    /* 0x48 */ s16 c[16];
} BashShoxExt;

void FirstTriggersBASH(void);

void InitialiseBASHFirst(void);
void InitialiseBASH(void);
void UpdateBASH(void);
s32 GetBashContour(EXTengine *eng);

void BashRestartLevel(void);

extern s16 BashExplodeScales[][3];
extern s16 BashAIVictim;
s32 BashAI_PickVictim(s32 arg0);
void BashCrateMaster(void);
void BashCountObjects(void);
void BashSpaceTileEdges(void);
void BashSpaceMarkHoles(void);

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/bash_xtras.h"
#include "bash/bash_bahero.h"

void BashAI_CheckShot(s32 arg0, s32 arg1, s32 arg2, BashAiState *ai, s32 prev);

void BashSpaceInitTiles(void);


void PlotSnowGlint(NAS *nas);

s32 BashFirePowerup(s32 arg0);

void InitBash8way(NAS *nas);

void InitBashShox(NAS *nas);

void PlotMissileExplode(NAS *nas);

void InitMissileExplode(NAS *nas);

void BashSpawnMissileExplode(s32 x, s32 y, s32 z, s32 arg3, s32 arg4);

void InitBoing(NAS *nas);

void InitBashPenguin(NAS *nas);

void InitSnowGlint(NAS *nas);

void BashDrawView(void);

u16 BashSpaceTileAt(s32 arg0, s32 arg1);


void BashSpaceDropTile(s32 arg0);

void BashSpaceBreakTile(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void BashSpaceClearTiles(void);

void BashDropFirstCrates(void);

s32 BashAI_HazardAhead(s32 arg0);

void BashSnowInitHeroMove(void);

u8 *CreateCrate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void BASHSetUp(void);

void BashInitCrateHooks(void);

void BashTumblerAim(BashTumbler *arg0, EXTmotion *arg1, s32 arg2);

typedef struct BashGridCell {
    s32 x;
    s32 z;
} BashGridCell;

extern BashGridCell BashDropPoints[16][16];

void BashSnowMoveHero(s32 arg0);

s32 BashAI_EscapeAngle(s32 arg0);

s32 BashAI_AvoidAhead(s32 arg0);

s32 BashAI_NearestEnemy(s32 arg0);

void BashSetCrateMix(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, u16 arg7, u16 arg8);

void BashShuffleDropPoints(void);

void BashGivePowerup(s32 arg0, u16 arg1);

void PlotBash8way(NAS *nas);

void InitBashHomer(NAS *nas);

u8 *BashDropCrateNear(s32 x, s32 unused, s32 z, u16 flag);


void *CrateCollide(EXTengine *eng, void *arg1, s32 arg2, s32 arg3);

/* Spawns the 0x1605 puff at v and returns it (InitTumbler). */
static inline NAS *BashSpawnPuff(SLONG_XYZ *v) {
    NAS *nas;
    EXTscriptdata *ext;

    nas = CreateNAS(v->x, v->y, v->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->speed = 0x100;
            ext->scriptnumber = 0;
            ext->flags = 9;
            ext->maxloops = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}

void BashPenguinPush(EXTengine *self);

void InitTumbler(NAS *nas);

void PlotTumbler(NAS *nas);

void PlotBashHomer(NAS *nas);

void PlotBashShox(NAS *nas);


extern u16 BashRainCount;
extern u16 BashRainTimer;


void PlotBashPenguin(NAS *nas);

/* InitialiseBASHFirst: the overlay's init hook. */
void Bash_HeroBot(s32 arg0);
void BashHero_ExtraEffects(s32 arg0);
NAS *BashHeroGrabObject(s32 idx, u16 cone);
s32 BashHeroKickCollision(s32 arg0);
s32 BashHeroCarryCollision(s32 idx);
extern u16 BashJungleAiSkill[];
extern u16 BashJungleAiSkillAlt[];
extern u16 BashJungleAiTeamSkill[];
extern u16 BashJungleAiHumanRoundTweak[];
extern u16 BashJungleAiBotTweak[];
extern u16 BashJungleAiHumanTweak[];
extern u16 BashSpaceAiSkill[];
extern u16 BashSpaceAiSkillAlt[];
extern u16 BashSpaceAiTeamSkill[];
extern u16 BashSpaceAiHumanRoundTweak[];
extern u16 BashSpaceAiBotTweak[];
extern u16 BashSpaceAiHumanTweak[];
extern u16 BashSnowAiSkill[];
extern u16 BashSnowAiSkillAlt[];
extern u16 BashSnowAiTeamSkill[];
extern u16 BashSnowAiHumanRoundTweak[];
extern u16 BashSnowAiBotTweak[];
extern u16 BashSnowAiHumanTweak[];
extern u16 BashDrainsAiSkill[];
extern u16 BashDrainsAiSkillAlt[];
extern u16 BashDrainsAiTeamSkill[];
extern u16 BashDrainsAiHumanRoundTweak[];
extern u16 BashDrainsAiBotTweak[];
extern u16 BashDrainsAiHumanTweak[];

/* InitialiseBASH */
extern u16 BashCrateTimer;
extern u16 BashPickupTimer;
extern u16 BashRainDelay;
extern s16 BashUnused1;
extern s32 BashTumblerTimer;
extern u8 *BashDrainsEdges[4];

/* The object CreateCrate returns, as BashCrateMaster sees it. */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ u16 unk24;
} BashEdgeSub;
typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ BashEdgeSub *edge;
} BashEdgeObj;

/* BashCrateMaster: from (x, z), walk the board along x (dx) or z (dz),
 * 1, -1, 2, -2, ... cells, to the first empty cell, and spawn there. */
static inline u8 *BashSpawnEdge(s32 x, s32 z, s32 dx, s32 dz) {
    s32 col;
    s32 row;
    s32 d;
    u8 *p;

    d = 1;
    col = (x >> 8) + 10;
    row = 9 - (z >> 8);
    do {
        if (ArenaGrid[row * 20 + col].flags != 0) {
            col = (x >> 8) + 10;
            row = 9 - (z >> 8);
            col += d * dx;
            row += d * dz;
            d = ~d + 1;
            if (d > 0) {
                d++;
            }
            if (d >= 9) {
                d = 0;
            }
        } else {
            d = 0;
        }
    } while (d != 0);
    col -= 10;
    col <<= 8;
    row = 19 - row;
    row -= 10;
    row <<= 8;
    if (col < -0x700 || col > 0x700) {
        col = x;
    }
    if (row < -0x700 || row > 0x700) {
        row = z;
    }
    p = CreateCrate(col, 0, row, 2, -1);
    if (p != NULL) {
        AddToGroup(&BashCrateList, p);
    }
    return p;
}
/* Snow's crate velocities by hero_name (HitHooks.unk0, the hooks' KickVels) */
extern CrateVelocity BashSnowKickVelMid, BashSnowKickVelFast, BashSnowKickVelSlow;
extern CrateVelocity *BashSnowKickVels[];
extern u16 BashSnowRunSpeeds[];
extern Ambient_data BashJungleAmbience;
extern Ambient_data BashDrainsAmbience;

#endif /* BASH_BASH_OVL_H */
