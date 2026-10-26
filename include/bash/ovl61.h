#ifndef BASH_OVL61_H
#define BASH_OVL61_H

/* Functions and data defined in the ovl61 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl61/ovl61.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/engine.h"
#include "bash/pause.h"
#include "bash/80083708.h"

extern HeroInfo Heroes[4];
extern char D_80054504[];

typedef struct Ovl61HeroBox {
    s32 a, b, c, d;
    s32 x0, y0, z0;
    s32 x1, y1, z1;
} Ovl61HeroBox;

extern Ovl61HeroBox ArkHeroBoxes[];

typedef struct Ovl61Spawn {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[2];
    s16 dir;
} Ovl61Spawn;

extern s32 *ArkWallTargets[][3];
extern u16 *D_8009D6EC;
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
extern s32 ArkDeadWalls;
extern u16 ArkFloorState;
extern u16 ArkWallFlags;
extern HeroInfo Heroes[4];
extern s32 CrateMasterState;
extern u16 ArkHeroSpeeds[][5];
extern u16 ArkHeroSpeed[];
extern s16 ArkHeroSpeedDelta[];
extern s32 ArkHeroStarts[4][3];
extern char D_8005453C[];
extern u16 ArkWallFlags;

extern u16 D_8009D754;
typedef struct Ovl61Cnt {
    u16 n;
} Ovl61Cnt;

extern Ovl61Cnt D_8009E25C;
extern s32 ArkBallTimer;
extern u16 D_8009E46C[];

typedef struct Ovl61FuxType {
    u8 pad0[0x14];
    u16 type;
} Ovl61FuxType;

void ArkCrateMaster(void);
void UpdateArkFloor(void);


typedef struct {
    s16 x;
    s16 z;
    s16 r;
} Ovl61Circle;

typedef struct Ovl61Gin {
    u8 pad0[0x14];
    s16 unk14;
    u8 pad16[4];
    s16 unk1A;
    s16 unk1C;
    u8 pad1E[2];
    s16 unk20;
    s16 unk22;
} Ovl61Gin;

/* D_8005A614 as seen by Plot_N_Gin. */
typedef struct {
    u8 pad0[0x32];
    u16 unk32;
    u8 pad34[0x6C - 0x34];
    s32 unk6C;
} Ovl61GinView;

extern char D_80054574[];
extern ScriptInit NGinScriptInit;
extern EXTengine *ArkBallSpawner;
extern s32 (*D_8009D750)(EXTengine *);
extern Ovl61Gin *ArkNGin;
typedef struct Ovl61Node {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[4];
    struct Ovl61Node *next;
    u8 pad24[0x30];
    struct Ovl61Node *target;
    struct {
        u8 pad0[0x14];
        u16 unk14;
    } *unk58;
} Ovl61Node;

extern Ovl61Circle *ArkPosts;
extern Ovl61Circle CameraFlashCircles[];
extern ScriptInit SeaweedScriptInit;

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
    s16 state;
} Ovl61Slot;

extern Ovl61Slot ArkGoals[];

typedef struct {
    u8 pad0[0x14];
    s16 unk14;
    u8 pad16[4];
    s16 unk1A;
    u8 pad1C[8];
    s16 unk24;
} Ovl61Spark;

extern u16 ArkPickupCount;
extern s16 ArkPickupSpots[];



extern u16 ArkPickupBounce;


void ArkHeroPowerupTimers(s32 arg0);







u8 *ArkFreeBallSlot(void);






void Ark_HeroBot(u32 arg0);
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
void AR_InitLevel(void);




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
} Ovl61Wall;

extern Ovl61Wall ArkWalls[];

typedef struct {
    u8 pad0[0x74];
    u16 unk74;
} Ovl61WallSub;

typedef struct {
    Ovl61WallSub *sub;
    u8 pad4[0xA - 0x4];
    u16 unkA;
    u8 padC[0xE - 0xC];
    u16 unkE;
    u8 pad10[0x14 - 0x10];
} Ovl61WallObj;

extern Ovl61WallObj ArkWallObjects[];

typedef struct {
    u32 *flags;
    Ovl61WallSub *sub;
    u8 pad8[0xE - 0x8];
    u16 unkE;
    u8 pad10[0x14 - 0x10];
} Ovl61Door;

extern Ovl61Door ArkDoorsA[];
extern Ovl61Door ArkDoorsB[];

typedef struct {
    u8 pad0[0x30];
    u16 unk30;
    u16 unk32;
    s16 unk34;
    u8 pad36[0x6C - 0x36];
    s32 unk6C;
} Ovl61ModeView;

/* AR_InitLevel (stage setup) */
extern s32 D_8005AA3C;
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
extern Ovl61Circle *ArkCircles;
extern Ovl61Circle *ArkBumpers;
extern Ovl61Circle ArkBallsPosts[];
extern Ovl61Circle ArkBallsCircles[];
extern Ovl61Circle ArkBallsBumpers[];
extern Ovl61Circle ArkBeachPosts[];
extern Ovl61Circle ArkBeachCircles[];
extern Ovl61Circle ArkBeachBumpers[];
extern Ovl61Circle ArkNginPosts[];
extern Ovl61Circle ArkNginCircles[];
extern Ovl61Circle ArkNginBumpers[];
extern Ovl61Circle ArkSkyPosts[];
extern Ovl61Circle ArkSkyCircles[];
extern Ovl61Circle ArkSkyBumpers[];
void InitARKfloor(void);
extern u16 CornerFailFrames[];

/* Blinking light slots (ArkGoals, ArkDoorsA) as Ovl61Link (declared
 * below), as seen by UpdateArkFloor. */
#define BLINK_E28 ((Ovl61Link *)ArkGoals)
#define BLINK_D38 ((Ovl61Link *)ArkDoorsA)

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[4];
    u16 unk10;
    u8 pad12[2];
    u16 unk14;
    u8 pad16[0x51 - 0x16];
    u8 unk51;
    s16 unk52;
} Ovl61Spr;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} Ovl61Rise;

typedef struct {
    s16 step;
    s16 reset;
    s16 max;
} Ovl61RiseCfg;

extern Ovl61RiseCfg FloorRiseCfg[];

typedef struct Ovl61Eng {
    u8 pad0[0xC];
    u32 flags;
    s32 x;
    s32 pad14;
    s32 z;
    u8 pad1C[0x7C - 0x1C];
    struct Ovl61Eng *sub;
} Ovl61Eng;

typedef struct Ovl61Fux {
    u8 pad0[0x1A];
    s16 flag;
} Ovl61Fux;

typedef struct Ovl61Mot {
    u8 pad0[0xE];
    s16 a;
    s16 b;
    s16 c;
    s16 lim;
    s16 dir;
} Ovl61Mot;

/* A FUX viewed for its flags word (0x80 tested by TestArkObjectObstacles). */
typedef struct Ovl61FuxF {
    u8 pad0[0x1C];
    u16 flags;
} Ovl61FuxF;

void ArkHeroEngineUpdate(s32 arg0);











s32 ArkHeroModeCheck(s32 arg0);













s32 ArkBallComingAtHero(s32 arg0);


void ArkSpawnBall(void);


/* D_8005A614 as seen by ArkCrateMaster. */
typedef struct Ovl61GameView {
    u8 pad0[0x30];
    u16 flags;
    u16 pad_buttons;
    s16 mode;
    u8 pad36[0x6C - 0x36];
    s32 unk6C;
} Ovl61GameView;
#define O61GV ((Ovl61GameView *)D_8005A614)

extern u16 CrateMasterTimes[];
extern u16 CrateMasterSpotsX[];
extern u16 CrateMasterSpotsZ[];


s32 ArkAddPolar(s32 arg0, s16 arg1, s32 arg2, s16 arg3);

/* ArkBotAimA: a player's edge segment (x0,z0)-(x1,z1) and aim spread. */
typedef struct Ovl61Pos {
    s32 x;
    s32 y;
    s32 z;
} Ovl61Pos;
typedef struct Ovl61Edge {
    s16 x0;
    s16 pad2;
    s16 z0;
    s16 pad6;
    s16 x1;
    s16 padA;
    s16 z1;
    s16 padE;
} Ovl61Edge;
extern Ovl61Edge ArkBotEdgesA[4];
extern u16 ArkBotAimAngles[];
extern s16 ArkBotSkill;

s32 ArkBotAimA(s32 arg0, Ovl61Pos *from, Ovl61Pos *to, s32 angle);

s32 ArkBotAimB(s32 arg0, Ovl61Pos *from, Ovl61Pos *to, s32 angle);
extern Ovl61Edge ArkBotEdgesB[4];
/* A scene object as InitARKfloor sorts it (list at GfxScene.objs). */
typedef struct Ovl61ScnObj {
    u32 flags;
    u8 pad4[0x5C - 0x4];
    struct Ovl61ScnObj *next; /* 0x5C */
    u8 pad60[0x68 - 0x60];
    s16 unk68;
    u8 pad6A[2];
    u8 *unk6C;
    u8 pad70[4];
    u16 unk74;
} Ovl61ScnObj;

/* The 0x14-byte link slots InitARKfloor fills (FloorLinks, ArkDoorsA...). */
typedef struct {
    Ovl61ScnObj *a;
    Ovl61ScnObj *b;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    u16 unk12;
} Ovl61Link;

extern Ovl61ScnObj *FloorObjectsA[10];
extern Ovl61ScnObj *FloorObjectsB[10];
extern Ovl61Link FloorLinks[4];
extern u16 FloorFramesA[];
extern u16 FloorFramesB[];
extern u16 FloorFramesC[];
extern u16 FloorLayout[][3];

s32 ArkBotCanHit(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, s32 arg5);

void InitARKfloor(void);

bool TestArkObjectObstacles(EXTengine *eng, EXTobject *self);
/* D_8005A614 as seen by PlotBall (unk34 is D_8005A648, the mode). */
typedef struct {
    u8 pad0[4];
    u32 unk4;
    u8 pad8[8];
    u32 unk10;
    u8 pad14[0x30 - 0x14];
    u16 unk30;
    u16 unk32;
    s16 unk34;
    u8 pad36[0x48 - 0x36];
    s16 unk48;
    s16 unk4A;
    u8 pad4C[0x5C - 0x4C];
    u8 unk5C[4];
    u8 pad60[0x6C - 0x60];
    s32 unk6C;
} Ovl61BallView;

/* Extension 0x10000000: the ball's trail positions. */
typedef struct {
    s32 EXTid;
    EXTnode Extend;
    SLONG_XYZ pos[8];
    u8 on[8];
} Ovl61Trail;

extern u16 BallSpeedFrames[][2];
extern u16 BallSpeedTable[10][10];
extern u16 BallFramesA[];
extern u16 BallFramesB[];
extern u16 BallFramesC[];

/* Extension 0x800 of a ball (ArkHero2ObjectCollision). */
typedef struct Ovl61Puck {
    u8 pad0[0xE];
    s16 speed;
    u8 pad10[4];
    s16 max;
    s16 dir;
    u8 pad18[0x32 - 0x18];
    u16 unk32;
} Ovl61Puck;

/* The spot a grabbed ball is held at (ArkHero2ObjectCollision). */
typedef struct Ovl61Spot {
    s16 x;
    s16 pad2;
    s16 z;
} Ovl61Spot;

extern u16 ArkGrabFrames[];

NAS *ArkHero2ObjectCollision(u32 arg0);

/* Ark_HeroBot: the object lists (D_8005AA38 by next, D_8005AA24 by child). */
typedef struct Ovl61AiFux {
    u8 pad0[0x1A];
    u16 unk1A;
    u8 pad1C[0x24 - 0x1C];
    s16 unk24;
} Ovl61AiFux;

typedef struct Ovl61AiExt {
    u8 pad0[0x16];
    u16 unk16;
    u16 unk18;
    u16 unk1A;
} Ovl61AiExt;

typedef struct Ovl61AiNode {
    u8 pad0[0x20];
    struct Ovl61AiNode *next;
    u8 pad24[0x2C - 0x24];
    struct Ovl61AiNode *child;
    u8 pad30[0x54 - 0x30];
    EXTengine *eng;
    Ovl61AiFux *fux;
} Ovl61AiNode;

extern u32 D_80051244[];
extern s16 ArkBotWaits[];

#endif /* BASH_OVL61_H */
