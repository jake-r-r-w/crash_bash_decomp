#ifndef BASH_ARKENOID_OVL_H
#define BASH_ARKENOID_OVL_H

/* Functions and data defined in the arkenoid overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/arkenoid/arkenoid.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/engine.h"
#include "bash/pause.h"
#include "bash/heroes.h"


typedef struct ArkenoidHeroBox {
    s32 a, b, c, d;
    s32 x0, y0, z0;
    s32 x1, y1, z1;
} ArkenoidHeroBox;

extern ArkenoidHeroBox ArkHeroBoxes[];

typedef struct ArkenoidSpawn {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[2];
    s16 dir;
} ArkenoidSpawn;

extern s32 *ArkWallTargets[][3];
extern s32 ArkFloorTiltX;
extern s32 ArkFloorTiltZ;
extern u16 ArkGrabTimes[];
extern u16 ArkKickFrames[];
extern u16 ArkRedKickTimer;
extern s32 ArkSidePlayer1;
extern s32 ArkSidePlayer2;
extern s32 ArkSidePlayer3;
extern s32 ArkDeadWalls;
extern s32 ArkSidePlayer0;
extern u16 ArkFloorState;
extern u16 ArkWallFlags;
extern s32 CrateMasterState;
extern u16 ArkHeroSpeeds[][5];
extern u16 ArkHeroSpeed[];
extern s16 ArkHeroSpeedDelta[];
extern s32 ArkHeroStarts[4][3];


extern s32 ArkBallTimer;

typedef struct ArkenoidFuxType {
    u8 pad0[0x14];
    u16 type;
} ArkenoidFuxType;



typedef struct {
    s16 x;
    s16 z;
    s16 r;
} ArkenoidCircle;



extern ScriptInit NGinScriptInit;
extern EXTengine *ArkBallSpawner;
extern EXTobject *ArkNGin;
typedef struct ArkenoidNode {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[4];
    struct ArkenoidNode *next;
    u8 pad24[0x30];
    struct ArkenoidNode *target;
    struct {
        u8 pad0[0x14];
        u16 unk14;
    } *unk58;
} ArkenoidNode;

extern ArkenoidCircle *ArkPosts;
extern ArkenoidCircle CameraFlashCircles[];
extern ScriptInit SeaweedScriptInit;

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
    s16 state;
} ArkenoidSlot;

extern ArkenoidSlot ArkGoals[];


extern u16 ArkPickupCount;
extern s16 ArkPickupSpots[];



extern u16 ArkPickupBounce;


void ArkHeroPowerupTimers(s32 arg0);







NAS *ArkFreeBallSlot(void);






extern u16 ArkHeroModels[];
extern s32 ArkVictorySeqs[];
extern void *ArkBounceOnBounds;
extern void *ArkTestOutOfBounds;
extern void *ArkTestLaunchBounds;
extern u16 ArkBallsAiSkill[];
extern u16 ArkBallsAiSkillAlt[];
extern u16 ArkBallsAiTeamSkill[];
extern u16 ArkBallsAiHumanRoundTweak[];
extern u16 ArkBallsAiBotTweak[];
extern u16 ArkBallsAiHumanTweak[];
extern u16 ArkBeachAiSkill[];
extern u16 ArkBeachAiSkillAlt[];
extern u16 ArkBeachAiTeamSkill[];
extern u16 ArkBeachAiHumanRoundTweak[];
extern u16 ArkBeachAiBotTweak[];
extern u16 ArkBeachAiHumanTweak[];
extern u16 ArkNginAiSkill[];
extern u16 ArkNginAiSkillAlt[];
extern u16 ArkNginAiTeamSkill[];
extern u16 ArkNginAiHumanRoundTweak[];
extern u16 ArkNginAiBotTweak[];
extern u16 ArkNginAiHumanTweak[];
extern u16 ArkSkyAiSkill[];
extern u16 ArkSkyAiSkillAlt[];
extern u16 ArkSkyAiTeamSkill[];
extern u16 ArkSkyAiHumanRoundTweak[];
extern u16 ArkSkyAiBotTweak[];
extern u16 ArkSkyAiHumanTweak[];




s32 ArkWallIndex(s32 arg0, s32 arg1);








typedef struct {
    s32 pad0;
    s32 f4;
    s32 f8;
    s32 fC;
    s32 f10;
    s32 f14;
    u16 f18;
    u16 f1A;
} ArkenoidWall;

extern ArkenoidWall ArkWalls[];

typedef struct {
    u8 pad0[0x74];
    u16 unk74;
} ArkenoidWallSub;

typedef struct {
    ArkenoidWallSub *sub;
    u8 pad4[0xA - 0x4];
    u16 unkA;
    u8 padC[0xE - 0xC];
    u16 unkE;
    u8 pad10[0x14 - 0x10];
} ArkenoidWallObj;

extern ArkenoidWallObj ArkWallObjects[];

typedef struct {
    u32 *flags;
    ArkenoidWallSub *sub;
    u8 pad8[0xE - 0x8];
    u16 unkE;
    u8 pad10[0x14 - 0x10];
} ArkenoidDoor;

extern ArkenoidDoor ArkDoorsA[];
extern ArkenoidDoor ArkDoorsB[];


/* AR_InitLevel (stage setup) */
extern s16 ArkInitWord0;
extern s16 ArkInitWord1;
extern s16 ArkInitWord2;
extern s16 ArkInitWord3;
extern u16 CrateMasterWordC;
extern s16 ArkBallCount;
extern s32 CrateMasterTimer;
extern s32 CrateMasterCount;
extern s32 CrateMasterNext;
extern u16 CrateMasterWordA;
extern u16 CrateMasterWordB;
extern u16 NGinState;
extern EXTengine ArkCornerEngine;
extern EXTengine ArkDieEngine;
extern ArkenoidCircle *ArkCircles;
extern ArkenoidCircle *ArkBumpers;
extern ArkenoidCircle ArkBallsPosts[];
extern ArkenoidCircle ArkBallsCircles[];
extern ArkenoidCircle ArkBallsBumpers[];
extern ArkenoidCircle ArkBeachPosts[];
extern ArkenoidCircle ArkBeachCircles[];
extern ArkenoidCircle ArkBeachBumpers[];
extern ArkenoidCircle ArkNginPosts[];
extern ArkenoidCircle ArkNginCircles[];
extern ArkenoidCircle ArkNginBumpers[];
extern ArkenoidCircle ArkSkyPosts[];
extern ArkenoidCircle ArkSkyCircles[];
extern ArkenoidCircle ArkSkyBumpers[];
extern u16 CornerFailFrames[];

/* Blinking light slots (ArkGoals, ArkDoorsA) as ArkenoidLink (declared
 * below), as seen by UpdateArkFloor. */
#define BLINK_E28 ((ArkenoidLink *)ArkGoals)
#define BLINK_D38 ((ArkenoidLink *)ArkDoorsA)


typedef struct {
    u8 pad0[4];
    s32 unk4;
} ArkenoidRise;

typedef struct {
    s16 step;
    s16 reset;
    s16 max;
} ArkenoidRiseCfg;

extern ArkenoidRiseCfg FloorRiseCfg[];

typedef struct ArkenoidEng {
    u8 pad0[0xC];
    u32 flags;
    s32 x;
    s32 pad14;
    s32 z;
    u8 pad1C[0x7C - 0x1C];
    struct ArkenoidEng *sub;
} ArkenoidEng;

typedef struct ArkenoidFux {
    u8 pad0[0x1A];
    s16 flag;
} ArkenoidFux;


/* A FUX viewed for its flags word (0x80 tested by TestArkObjectObstacles). */
typedef struct ArkenoidFuxF {
    u8 pad0[0x1C];
    u16 flags;
} ArkenoidFuxF;

void ArkHeroEngineUpdate(s32 arg0);











s32 ArkHeroModeCheck(s32 arg0);













s32 ArkBallComingAtHero(s32 arg0);


void ArkSpawnBall(void);


#define O61GV ((GameState *)GameInfo)

extern u16 CrateMasterTimes[];
extern u16 CrateMasterSpotsX[];
extern u16 CrateMasterSpotsZ[];


s32 ArkAddPolar(s32 arg0, s16 arg1, s32 arg2, s16 arg3);

/* ArkBotAimA: a player's edge segment (x0,z0)-(x1,z1) and aim spread. */
typedef struct ArkenoidPos {
    s32 x;
    s32 y;
    s32 z;
} ArkenoidPos;
typedef struct ArkenoidEdge {
    s16 x0;
    s16 pad2;
    s16 z0;
    s16 pad6;
    s16 x1;
    s16 padA;
    s16 z1;
    s16 padE;
} ArkenoidEdge;
extern ArkenoidEdge ArkBotEdgesA[4];
extern u16 ArkBotAimAngles[];
extern s16 ArkBotSkill;

s32 ArkBotAimA(s32 arg0, ArkenoidPos *from, ArkenoidPos *to, s32 angle);

s32 ArkBotAimB(s32 arg0, ArkenoidPos *from, ArkenoidPos *to, s32 angle);
extern ArkenoidEdge ArkBotEdgesB[4];
/* A scene object as InitARKfloor sorts it (list at MapScene->subs). */
typedef struct ArkenoidScnObj {
    u32 flags;
    u8 pad4[0x5C - 0x4];
    struct ArkenoidScnObj *next; /* 0x5C */
    u8 pad60[0x68 - 0x60];
    s16 unk68;
    u8 pad6A[2];
    u8 *unk6C;
    u8 pad70[4];
    u16 unk74;
} ArkenoidScnObj;

/* The 0x14-byte link slots InitARKfloor fills (FloorLinks, ArkDoorsA...). */
typedef struct {
    ArkenoidScnObj *a;
    ArkenoidScnObj *b;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 timer;  /* counted down */
    u16 timer2;  /* counted down */
} ArkenoidLink;

extern ArkenoidScnObj *FloorObjectsA[10];
extern ArkenoidScnObj *FloorObjectsB[10];
extern ArkenoidLink FloorLinks[4];
extern u16 FloorFramesA[];
extern u16 FloorFramesB[];
extern u16 FloorFramesC[];
extern u16 FloorLayout[][3];

s32 ArkBotCanHit(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, s32 arg5);



/* Extension 0x10000000: the ball's trail positions. */
typedef struct {
    s32 EXTid;
    EXTnode Extend;
    SLONG_XYZ pos[8];
    u8 on[8];
} ArkenoidTrail;

extern u16 BallSpeedFrames[][2];
extern u16 BallSpeedTable[10][10];
extern u16 BallFramesA[];
extern u16 BallFramesB[];
extern u16 BallFramesC[];


/* The spot a grabbed ball is held at (ArkHero2ObjectCollision). */
typedef struct ArkenoidSpot {
    s16 x;
    s16 pad2;
    s16 z;
} ArkenoidSpot;

extern u16 ArkGrabFrames[];


/* Ark_HeroBot: the object lists (HeadOfBalls by next, HeadOfObjects by child). */
typedef struct ArkenoidAiFux {
    u8 pad0[0x1A];
    u16 pad1A;
    u8 pad1C[0x24 - 0x1C];
    s16 unk24;
} ArkenoidAiFux;

typedef struct ArkenoidAiExt {
    u8 pad0[0x16];
    u16 unk16;
    u16 pad18;
    u16 unk1A;
} ArkenoidAiExt;


extern s16 ArkBotWaits[];

#endif /* BASH_ARKENOID_OVL_H */
