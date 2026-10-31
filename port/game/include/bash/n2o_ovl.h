#ifndef BASH_N2O_OVL_H
#define BASH_N2O_OVL_H

/* Functions and data defined in the n2o overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/n2o/n2o.c */

#include "common.h"
#include "bash/nas.h"


s32 GetN2OContour(EXTengine *arg0);
NAS *N2OFreeBallSlot(void);
void UpdateN2O(void);
void InitialiseN2OFirst(void);
void InitialiseN2O(void);


extern u16 N2OKickSfx[];
extern s16 N2OExplodeScales[][3];
extern u16 N2OBallPhase;
extern u16 N2OHeroPlaced[];
extern u16 N2OUnusedRows[][5];
extern s32 N2OHeroStarts[][3];
void NoxResetHeroes(void);
extern u16 N2OWallsHit;
extern s16 N2OHeroSpeed[];



/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/n2o_nox.h"

void N2OUpdateShadow(s32 arg0);

void SetN2OHeroDead(s32 arg0);


void N2OLineToCircleTest(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r);

s32 N2OAddPolar(s32 arg0, s16 arg1, s32 arg2, s16 arg3);

/* Each player's reach box for hitting the ball (N2OHeroBallCollision): the y range
 * relative to the hero, the near and far radii, and the near and far points. */
typedef struct N2OHeroBox {
    s32 a, b, c, d;
    s32 x0, y0, z0;
    s32 x1, y1, z1;
} N2OHeroBox;

extern N2OHeroBox N2OHeroBoxes[];



extern u16 N2OBallHitSfx[];
extern u16 N2OExplosionSfx[];

s32 N2OHeroBallCollision(s32 arg0, EXTengine *unused);

typedef struct N2OVel {
    u8 pad0[offsetof(EXTengine, XYZ.x)];
    s32 x;
    u8 pad14[4];
    s32 z;
} N2OVel;


typedef struct N2OOut {
    u8 pad0[0x16];
    s16 dir;
} N2OOut;



void N2OPreMoveHero(s32 arg0);

s32 TestN2OObject2Hero(N2OVel *arg0, s32 arg1);

void PlotN2ODeadWall(NAS *arg0);

void N2OSpawnFirstMissile(void);

void SetN2OHeroMove(s32 arg0);

void SetN2OHeroTaunt(s32 arg0);
/* Round state cleared by InitialiseN2O. */
typedef struct N2ORound {
    /* 0x00 */ void *unk0;
    /* 0x04 */ EXTengine *eng;
    /* 0x08 */ void *unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
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
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} N2ORound;

typedef struct N2OFlagNode {
    u32 flags;
    u8 pad4[offsetof(subengine, next) - (offsetof(subengine, flags) + 4)];
    struct N2OFlagNode *next;
    u8 pad60[offsetof(subengine, u6C.ptr) - (offsetof(subengine, next) + sizeof(void *))];
    u8 *dbase;
    u8 pad70[offsetof(subengine, entity) - (offsetof(subengine, u6C.ptr) + sizeof(void *))];
    u16 entity;
} N2OFlagNode;

extern N2ORound N2OBossRound;
extern s32 N2OHeroList;
extern NAS *N2OBossNas;
typedef struct {
    s16 x;
    s16 z;
    s16 r;
} N2OCircle;


extern N2OCircle *N2OPillarList;
extern N2OCircle N2OPillars[];



typedef struct N2OSlot {
    subengine *src;
    s32 unk4;
    NAS *nas;
    u16 f0C;
    u8 pad0E[4];
    u16 f12;
    u8 pad14[2];
    u16 f16;
    u16 f18;
    u16 kind;
    s32 f1C;
    s32 f20;
    s32 f24;
} N2OSlot;
extern N2OSlot N2OSlots[];

typedef struct N2OEnt {
    s32 flags;
    u8 pad4[4];
    s32 y;
    u8 pad0C[0x50];
    struct N2OEnt *next;
    u8 pad60[0xC];
    u8 *key;
    u8 pad70[4];
    u16 id;
} N2OEnt;

typedef struct N2ORec {
    N2OEnt *ent;
    s32 f4;
    s32 f8;
    u16 fC;
    u8 pad0E[4];
    u16 f12;
    u8 pad14[2];
    u16 f16;
    u16 f18;
    u16 f1A;
    s32 f1C;
    s32 f20;
    s32 f24;
} N2ORec;
extern N2ORec N2OSlotRecs[];
extern s16 N2OBallClock;
extern s16 N2OBallCount;
extern s16 N2OBallDelay;
extern s16 N2OBossStepKind;
extern s16 N2OBossPad;
extern s16 N2OBossStepFlag;
extern s16 N2OBossStepTimer;
extern s16 N2OBossStep;
extern s16 N2OMissileCount;
extern s16 N2OMissileDelay;
extern u16 N2OBossAimDelay;
extern s32 N2ODeadWallUp;
extern s32 N2OBossShooting;

/* N2OUpdateBoss's script: kind (99 = jump to step), slot kind, time/step. */
typedef struct N2OStep {
    u16 kind;
    u16 slot;
    u16 time;
} N2OStep;
extern N2OStep N2OBossSteps[];
typedef struct N2OHalf {
    s16 v;
} N2OHalf;
typedef struct N2OPanRef {
    struct N2OPan *p;
} N2OPanRef;
extern u16 N2OBossShotSfx;
typedef struct N2OPan {
    u8 pad0[offsetof(EXTheromove, forward_speed)];
    s32 flag;  /* tested for zero */
} N2OPan;
extern u16 N2OHeroPushTimer;
extern s16 N2OBallsLost;
void N2OInitBoss(void);
void N2OInitSlots(void);
void N2OShowPhaseObjects(u16 arg0);
void InitN2OmissileExplode(NAS *nas);

void NoxInit(void);

s32 N2OHeroTestIdle(s32 arg0);

void N2OSetUp(void);
/* A ball in the HeadOfBalls list (N2OBotCanHit), as in arkenoid's ArkenoidNode. */
typedef struct N2ONode {
    u8 pad0[offsetof(NAS, Group.Child)];
    struct N2ONode *next;
    u8 pad24[offsetof(NAS, Engine) - (offsetof(NAS, Group.Child) + sizeof(void *))];
    EXTengine *target;
    struct {
        u8 pad0[0x14];
        u16 whatami;
        u8 pad16[4];
        u16 status;
    } *unk58;
} N2ONode;

typedef struct N2OPos {
    s32 x;
    s32 y;
    s32 z;
} N2OPos;

s32 N2O_BA_TestLaunchBounds(EXTengine *arg0);

void SetN2OHeroHit(s32 arg0);

void N2OHeroHit(s32 arg0);

void SetN2OHeroDie(s32 arg0);

void SetN2OHeroKick(s32 arg0);

void N2OHeroDead(s32 arg0);

void N2OBallMaster(void);

typedef struct N2OEdge {
    s16 x0;
    s16 pad2;
    s16 z0;
    s16 pad6;
    s16 x1;
    s16 padA;
    s16 z1;
    s16 padE;
} N2OEdge;
extern N2OEdge N2OAimEdgesA[];
extern u16 N2OAimAngles[];
extern u16 N2OSkillBias;

s32 N2OBotAimA(s32 arg0, N2OPos *from, N2OPos *to, s32 angle);

s32 N2OBotAimB(s32 arg0, N2OPos *from, N2OPos *to, s32 angle);

s32 N2OBotCanHit(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, void *arg5);

void N2OSpawnSlotMissile(s32 a);


void PlotN2OmissileExplode(NAS *nas);

void N2OSpawnExplosion(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void InitN2Ofirstmissile(NAS *nas);

void InitN2Omissile(NAS *nas);

void N2OHeroDie(s32 arg0);

void SetN2OHeroIdle(s32 arg0);

void SetN2OHeroEnter(s32 arg0);

s32 N2OHeroTestMove(s32 arg0);

s32 N2OHeroTestKick(s32 arg0);

void N2OInitShadow(s32 arg0);

s32 N2OTestWalls(N2OVel *arg0, N2OOut *arg1, s32 arg2);


void N2OHeroSetSpeed(s32 arg0);
s32 N2OMissileHitTest(s32 x, s32 y, s32 z, u16 size, u16 mask);
s32 N2OHeroUpdateSpeed(s32 arg0);
extern u32 *N2OPhaseObject;

s32 N2O_KeepInBounds(EXTengine *arg0);

void N2OHeroTaunt(s32 arg0);

void N2OHeroMove(s32 arg0);

void SetN2OHeroWinner(s32 arg0);

void N2OHeroWinner(s32 arg0);

void N2OHeroKick(s32 arg0);

void InitN2OBall(NAS *nas);

void N2OBlendDirection(EXTmotion *p);

s32 PickN2OPlayer(s32 arg0);

/* N2O_HeroBot */

#define OVG ((GameState *)GameInfo)

#define OVM ((GameState *)GameInfo)
extern u16 N2OMissileSfx[];
extern u16 N2OHitSfx[];

#define OVT ((GameState *)GameInfo)
extern u16 N2OBossAiSkill[];
extern u16 N2OBossAiSkillAlt[];
extern s16 N2OBossAnimTimer;
extern s32 N2OBossClock;

extern s32 NoxBossDead;
extern s32 NoxStarted;
extern u16 N2OHeroModels[];
extern VictorySeq N2OVictorySeqs[]; /* {name, id} pairs: the victory sequences (the engine's hook 0x14) */
extern u16 N2OAiSkill[];
extern u16 N2OAiSkillAlt[];
s32 N2O_BA_BounceOnBounds(EXTengine *arg0, s32 arg1);

void N2OHeroEngineUpdate(s32 arg0);

void N2OInitHero(s32 i);
void N2OMoveHero(s32 arg0);
void N2OHeroEnter(s32 arg0);

void N2OHeroIdle(s32 arg0);

void InitN2ODeadWall(NAS *nas);

s32 N2OHeroRad2ObjectCollision(u32 arg0, s32 arg1, bool arg2);

NAS *N2OBossPushObjects(void);

s32 N2OHeroPushTeammate(s32 arg0);
void NoxPreMoveHero(s32 arg0);
void NoxMoveHero(s32 arg0);

extern N2OEdge N2OAimEdgesB[];


/* A FUX viewed for its flags word (0x80 tested by TestN2OObjectObstacles). */
typedef struct N2OFuxF {
    u8 pad0[offsetof(EXTobject, can_do)];
    u16 flags;
} N2OFuxF;

void N2OUpdateBoss(void);

void N2O_HeroBot(u16 *arg0);

void PlotN2Omissile(NAS *nas);

void PlotN2Ofirstmissile(NAS *nas);

void N2OUpdateSlots(void);

s32 TestN2OObjectObstacles(EXTengine *eng, N2OFuxF *self);

extern SLONG_XYZ N2OBallStarts[];
extern u16 N2OBallBounceSfx[];
extern u16 N2OBallSfx[];

/* LevelSpecs.GetContour as PlotN2OBall reads it: the level's contour hook. */
typedef struct N2OHook {
    s32 (*fn)(EXTengine *eng);
} N2OHook;

/* PlotN2OBall's bounds tests: past an edge sets the scoring side. */
static inline s32 N2OBallOut(EXTengine *eng, EXTobject *obj, s32 wide) {
    s32 lim;
    s32 x;
    s32 z;

    lim = 0xB00;
    if (wide != 0) {
        lim = 0x1200;
    }
    x = eng->XYZ.x;
    if (x > lim) {
        obj->player = -1;
        return 1;
    }
    if (x < -lim) {
        obj->player = -1;
        return 1;
    }
    z = eng->XYZ.z;
    if (z > lim) {
        obj->player = 3;
        return 1;
    }
    if (z < -lim) {
        obj->player = 0;
        return 1;
    }
    if (wide != 0) {
        if (z < -0xA00) {
            if (x > 0x600) {
                eng->XYZ.x = 0x600;
            }
            if (eng->XYZ.x < -0x600) {
                eng->XYZ.x = -0x600;
            }
        }
    }
    return 0;
}

void PlotN2OBall(NAS *nas);


#endif /* BASH_N2O_OVL_H */
