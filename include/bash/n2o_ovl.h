#ifndef BASH_N2O_OVL_H
#define BASH_N2O_OVL_H

/* Functions and data defined in the n2o overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/n2o/n2o.c */

#include "common.h"
#include "bash/nas.h"

/* D_8009D87C (declared in bash/pause.h, which this unit does not include). */
typedef struct N2OOdds {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} N2OOdds;

extern N2OOdds D_8009D87C[];

s32 GetN2OContour(u8 *arg0);
u8 *N2OFreeBallSlot(void);
void UpdateN2O(void);
void InitialiseN2OFirst(void);
void InitialiseN2O(void);

extern u16 *D_8009D6EC;

extern u8 D_80053EAC[];
extern EXTengine *D_8009D530;
extern u16 D_800AD12C;
extern u16 D_800C39A0[];
extern s16 D_800C4150[][3];
extern u16 D_800C417A;
extern u16 D_800C417C[];
extern u16 D_800C39EC[][5];
extern s32 D_800C4050[][3];
extern EXTengine *D_8009D59C;
void NoxResetHeroes(void);
extern u16 D_800C438A;
extern s16 D_800C4184[];
extern u16 D_800C39EC[][5];
extern s32 (*D_8009D750)(EXTengine *);

/* PadState (D_80051338) as N2OPreMoveHero reads it: the buttons at 0x50. */
typedef struct N2OPad {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x50 - 0x04];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x58 - 0x54];
} N2OPad;

/* PadState as N2OHeroHit reads it: flags at 0x4C and 0x54. */
typedef struct N2OPadFlags {
    /* 0x00 */ u8 pad0[0x4C];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} N2OPadFlags;

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/n2o_nox.h"

void N2OUpdateShadow(s32 arg0);
extern HeroInfo Heroes[4];

void SetN2OHeroDead(s32 arg0);
extern char D_80053EE4[];
s32 csqrt(s32 x);

NAS *func_8007A524(s32 arg0, s32 *arg1, s32 arg2);
void func_800898EC(s32 arg0);

void N2OLineToCircleTest(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r);

s32 N2OAddPolar(s32 arg0, s16 arg1, s32 arg2, s16 arg3);

/* Each player's reach box for hitting the ball (N2OHeroBallCollision): the y range
 * relative to the hero, the near and far radii, and the near and far points. */
typedef struct N2OHeroBox {
    s32 a, b, c, d;
    s32 x0, y0, z0;
    s32 x1, y1, z1;
} N2OHeroBox;

extern N2OHeroBox D_800C4190[];

/* Extension 0x800 of a ball (N2OHeroBallCollision), as in arkenoid's ArkenoidPuck. */
typedef struct N2OPuck {
    u8 pad0[0xE];
    s16 speed;
    u8 pad10[4];
    s16 max;
    s16 dir;
    u8 pad18[0x32 - 0x18];
    u16 unk32;
} N2OPuck;

extern u16 D_800C39B0[];
extern u16 D_800C39DC[];
extern s32 D_8005A618;
extern s32 D_8005A680[];

s32 N2OHeroBallCollision(s32 arg0, EXTengine *unused);

typedef struct N2OVel {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
} N2OVel;

typedef struct N2OBlend {
    u8 pad0[0xE];
    s16 speed_a;
    u8 pad10[6];
    u16 dir;
    u8 pad18[0xA];
    s16 speed_b;
    u8 pad24[6];
    u16 dir_b;
} N2OBlend;

typedef struct N2OOut {
    u8 pad0[0x16];
    s16 dir;
} N2OOut;

extern u16 D_800C39B0[];
extern s32 D_8005A618;
extern s32 D_8005A680[];

s32 N2OHeroBallCollision(s32 arg0, EXTengine *unused);

void N2OPreMoveHero(s32 arg0);

s32 TestN2OObject2Hero(N2OVel *arg0, s32 arg1);

void PlotN2ODeadWall(NAS *arg0);

void N2OSpawnFirstMissile(void);

void SetN2OHeroMove(s32 arg0);

void SetN2OHeroTaunt(s32 arg0);
/* Round state cleared by InitialiseN2O. */
typedef struct N2ORound {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ EXTengine *unk4;
    /* 0x08 */ s32 unk8;
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

typedef struct N2OScene {
    u8 pad0[0x27];
    u8 unk27;
} N2OScene;

typedef struct N2OFlagNode {
    u32 flags;
    u8 pad4[0x58];
    struct N2OFlagNode *next;
    u8 pad60[0xC];
    u8 *unk6C;
    u8 pad70[4];
    u16 unk74;
} N2OFlagNode;

typedef struct N2OSceneRef {
    u8 pad0[0x10];
    N2OScene *unk10;
    u8 pad14[8];
    N2OFlagNode *unk1C;
} N2OSceneRef;

extern N2ORound D_800C4398;
extern s32 D_8005AA3C;
extern s32 D_800C4080;
extern NAS *D_800C4084;
typedef struct {
    s16 x;
    s16 z;
    s16 r;
} N2OCircle;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[0x6E];
    s16 radius;
    u8 pad8C[4];
    u32 flags;
} N2OObj;

extern N2OCircle *D_800C418C;
extern u8 D_800C37DC[];

/* The object a D_800C42D0 slot moves (N2OUpdateSlots). */
typedef struct N2OSlotObj {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ u8 pad10[2];
    /* 0x12 */ s16 ang;
    /* 0x14 */ u8 pad14[0x3D];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u16 unk52;
} N2OSlotObj;

/* D_8005A614 as seen by N2OUpdateSlots. */
typedef struct N2OAim {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 pad32[0x3A];
    /* 0x6C */ s32 unk6C;
} N2OAim;

typedef struct N2OSlot {
    N2OSlotObj *src;
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
extern N2OSlot D_800C42D0[];

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
extern N2ORec D_800C4230[];
extern char D_80054504[];
extern s16 D_800C4370;
extern s16 D_800C4372;
extern s16 D_800C4374;
extern s16 D_800C4376;
extern s16 D_800C4378;
extern s16 D_800C437A;
extern s16 D_800C437C;
extern s16 D_800C437E;
extern s16 D_800C4380;
extern s16 D_800C4382;
extern u16 D_800C4384;
extern s32 D_800C438C;
extern s32 D_800C4390;

/* N2OUpdateBoss's script: kind (99 = jump to step), slot kind, time/step. */
typedef struct N2OStep {
    u16 kind;
    u16 slot;
    u16 time;
} N2OStep;
extern N2OStep D_800C391C[];
typedef struct N2OHalf {
    s16 v;
} N2OHalf;
typedef struct N2OPanRef {
    struct N2OPan *p;
} N2OPanRef;
extern u16 D_800C39A6;
extern s16 D_8009D6D6;
extern u16 D_8009D6E0;
typedef struct N2OPan {
    u8 pad0[0xC];
    s32 unkC;
} N2OPan;
extern N2OPan *D_8009D688;
extern u16 D_800C4394;
extern s16 D_800C43DC;
void N2OInitBoss(void);
void N2OInitSlots(void);
void N2OShowPhaseObjects(u16 arg0);
void InitN2OmissileExplode(NAS *nas);

void NoxInit(void);

s32 N2OHeroTestIdle(s32 arg0);

void N2OSetUp(void);
/* A ball in the D_8005AA38 list (N2OBotCanHit), as in arkenoid's ArkenoidNode. */
typedef struct N2ONode {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[4];
    struct N2ONode *next;
    u8 pad24[0x30];
    struct N2ONode *target;
    struct {
        u8 pad0[0x14];
        u16 unk14;
        u8 pad16[4];
        u16 unk1A;
    } *unk58;
} N2ONode;

typedef struct N2OPos {
    s32 x;
    s32 y;
    s32 z;
} N2OPos;

s32 N2O_BA_TestLaunchBounds(N2OObj *arg0);

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
extern N2OEdge D_800C4090[];
extern u16 D_800C4110[];
extern u16 D_800C4386;

s32 N2OBotAimA(s32 arg0, N2OPos *from, N2OPos *to, s32 angle);

s32 N2OBotAimB(s32 arg0, N2OPos *from, N2OPos *to, s32 angle);

s32 N2OBotCanHit(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, s32 arg5);

void N2OSpawnSlotMissile(s32 a);

void N2OCameraFollow(void);

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

extern s8 D_8005A689;

s32 N2OHeroUpdateSpeed();
extern u32 *D_800C43D8;

s32 N2O_KeepInBounds(N2OObj *arg0);

void N2OHeroTaunt(s32 arg0);

void N2OHeroMove(s32 arg0);

void SetN2OHeroWinner(s32 arg0);

void N2OHeroWinner(s32 arg0);

void N2OHeroKick(s32 arg0);

void InitN2OBall(NAS *nas);

void N2OBlendDirection(N2OBlend *p);

s32 PickN2OPlayer(s32 arg0);

/* N2O_HeroBot */
extern s32 D_8005A624;
extern s32 D_8009D908;
extern u32 D_80051244[]; /* flags by angle octant */

/* D_8005A614 as seen by InitialiseN2OFirst. */
typedef struct N2OGame {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u8 pad34[0x1D];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 pad54[0x18];
    /* 0x6C */ s32 unk6C;
} N2OGame;
#define OVG ((N2OGame *)D_8005A614)

/* D_8005A614 as seen by PlotN2Omissile. */
typedef struct N2OMissileView {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x64];
    /* 0x6C */ s32 unk6C;
} N2OMissileView;
#define OVM ((N2OMissileView *)D_8005A614)
extern u16 D_800C39CC[];
extern u16 D_800C39D4[];

/* D_8005A614 as seen by N2OInitBoss. */
typedef struct N2OTeams {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 pad36[0x65];
    /* 0x9B */ u8 unk9B[0x10];
    /* 0xAB */ u8 unkAB[4];
} N2OTeams;
#define OVT ((N2OTeams *)D_8005A614)
extern u16 D_800C3EB0[];
extern u16 D_800C3EF0[];
extern s16 D_800C4182;
extern s32 D_800C43E0;
extern s16 D_8009D6E2;
void InitBossHealthIcons(void);
void func_80086868(void);

/* D_8009D6E8 (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct N2OHooks {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8 pad8[0xC];
    /* 0x14 */ void *unk14;
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ void *unk2C;
    /* 0x30 */ u8 pad30[0x38];
    /* 0x68 */ void *unk68;
} N2OHooks;
extern N2OHooks D_8009D6E8;
extern void *D_8009D8FC;
extern s16 D_8009E0D8;
extern s16 D_8009E0E2;
extern s32 D_800C4498;
extern s32 D_800C45F0;
extern u8 D_800C403C[];
extern s32 D_800C3774[]; /* {name, id} pairs: the victory sequences (the engine's hook 0x14) */
extern u8 D_800C3E30[];
extern u8 D_800C3E70[];
s32 N2O_BA_BounceOnBounds(N2OObj *arg0, s32 arg1);

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

extern N2OEdge D_800C40D0[];

/* Motion extension (0x800) viewed by TestN2OObjectObstacles. */
typedef struct N2OMot {
    u8 pad0[0xE];
    s16 a;
    s16 b;
    s16 c;
    s16 lim;
    s16 dir;
} N2OMot;

/* A FUX viewed for its flags word (0x80 tested by TestN2OObjectObstacles). */
typedef struct N2OFuxF {
    u8 pad0[0x1C];
    u16 flags;
} N2OFuxF;

void N2OUpdateBoss(void);

void N2O_HeroBot(u16 *arg0);

void PlotN2Omissile(NAS *nas);

void PlotN2Ofirstmissile(NAS *nas);

void N2OUpdateSlots(void);

s32 TestN2OObjectObstacles(EXTengine *eng, N2OFuxF *self);

/* D_8005A614 as seen by PlotN2OBall. */
typedef struct N2OBallView {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x1C];
    /* 0x30 */ u16 unk30;
} N2OBallView;
extern EXTengine *D_8009D674;
extern s16 D_8009D88E;
extern s32 D_8009E424[];
extern SLONG_XYZ D_800C3794[];
extern u16 D_800C39A8[];
extern u16 D_800C39B8[];

/* D_8005AA94 as PlotN2OBall reads it: the level's contour hook. */
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
    x = eng->unk10.x;
    if (x > lim) {
        obj->player = -1;
        return 1;
    }
    if (x < -lim) {
        obj->player = -1;
        return 1;
    }
    z = eng->unk10.z;
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
                eng->unk10.x = 0x600;
            }
            if (eng->unk10.x < -0x600) {
                eng->unk10.x = -0x600;
            }
        }
    }
    return 0;
}

void PlotN2OBall(NAS *nas);

#endif /* BASH_N2O_OVL_H */
