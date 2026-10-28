#ifndef BASH_SPIN_OVL_H
#define BASH_SPIN_OVL_H

/* Functions and data defined in the spin overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/spin/spin.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"

/* A scene object in the list at MapScene + 0x1C (SpinInitScene). */
typedef struct SpinObj {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 rx;
    /* 0x12 */ u16 ry;
    /* 0x14 */ u16 rz;
    /* 0x16 */ u8 pad16[0x51 - 0x16];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u16 dirty;
    /* 0x54 */ u8 pad54[0x5C - 0x54];
    /* 0x5C */ struct SpinObj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ u8 *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} SpinObj;

typedef struct SpinScene {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ SpinObj *objs;
} SpinScene;

typedef struct SpinSlot {
    /* 0x00 */ SpinObj *obj;
    /* 0x04 */ s16 fixed[3]; /* used as is when nonzero */
    /* 0x0A */ s16 rate[3];  /* else SpinTurn * rate >> 4 */
} SpinSlot;

typedef struct SpinCell {
    s32 x;
    s32 y;
} SpinCell;

extern SpinCell SpinDropPoints[4][16];

/* GameInfo as seen by the ranking code (SpinRankHeroes). */
typedef struct SpinGame {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u32 flags10;
    /* 0x14 */ u8 pad14[0x30 - 0x14];
    /* 0x30 */ u16 state;
    /* 0x32 */ u16 flags;
    /* 0x34 */ u8 pad34[0x48 - 0x34];
    /* 0x48 */ s16 team[2];
    /* 0x4C */ u8 pad4C[0x5C - 0x4C];
    /* 0x5C */ u8 cpu[4];
    /* 0x60 */ u8 winner[2];
    /* 0x62 */ u8 pad62[0x6C - 0x62];
    /* 0x6C */ s32 teams;
    /* 0x70 */ u8 pad70[0xA7 - 0x70];
    /* 0xA7 */ u8 rank[4];
} SpinGame;

void InitBigScore(void);

#define SPIN_GAME ((SpinGame *)GameInfo)

/* Sets the round's result to a draw: every hero still standing loses. */
#define SPIN_DRAW(g, arg1, i)                          \
    do {                                                \
        (g)->winner[0] = 0;                             \
        (g)->state |= (arg1);                           \
        for ((i) = 0; (i) < 4; (i)++) {                 \
            if (!(Heroes[i].hero_status & 0xFA00)) {    \
                Heroes[i].hero_status |= 0x400;         \
            }                                           \
        }                                               \
    } while (0)

/* A hit (flags 2|8) pops the object: state 9, hit and mask flags cleared. */
#define SPIN_CHECK_POP(fux)                            \
    do {                                                \
        if ((fux)->hit & 0xA) {                         \
            (fux)->sub = 9;                             \
            (fux)->hit &= 0xFFF5;                       \
            (fux)->mask &= 0xFFF5;                      \
        }                                               \
    } while (0)

/* Turns the heading half a quadrant when the engine collides. */
#define SPIN_BOUNCE(eng, mot)                                          \
    do {                                                                \
        if (HeroHooks.KeepInBounds(eng) != 0) {                                     \
            (mot)->Xa = ((mot)->Xa - SpinSpeed * 0x200) & 0xFFF;       \
        }                                                               \
    } while (0)

/* Eases the engine towards 0x300 below the water level World_floor_level. */
#define SPIN_FLOAT(eng, g, y, s)                       \
    do {                                                \
        (g) = World_floor_level;                               \
        (y) = (eng)->unk10.y;                           \
        (y) += 0x300;                                   \
        (s) = (y) - (g);                                \
        if (abs(s) > 0x10) {                            \
            (eng)->unk10.y -= (s) >> 6;                 \
        }                                               \
    } while (0)

/* Puts the engine's shadow (its child engine) on the ground below it. */
#define SPIN_SHADOW(eng, shadow)                                                   \
    do {                                                                            \
        (shadow) = (eng)->Engine;                                                   \
        if ((shadow) != NULL) {                                                     \
            (shadow)->unk10 = (eng)->unk10;                                         \
            (shadow)->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);        \
        }                                                                           \
    } while (0)

extern s32 SpinBestScore;
extern s32 SpinBestHero;
extern s32 SpinTurn;
extern s32 SpinTurnTarget;
extern s16 SpinAngle;
extern s16 SpinAngle2;
extern s16 SpinSpeed;
extern SpinObj *SpinArenaObjs[10];
extern SpinSlot SpinSlots[10];

#include "bash/main.h"

extern s32 SpinASTBase;
extern s32 HitHooks[];

void InitialiseSPINFirst();

void InitialiseSPIN();

s32 SpinAlwaysZero(s32 player);

s32 SpinNoAttack(void);

void SpinTouched(void);

void FirstTriggersSPIN(void);

void SpinRestartLevel(void);

void UpdateSPIN(void);

void SPINSetUp(void);

typedef struct {
    /* 0x00 */ u8 unk0[0x1C];
    /* 0x1C */ u16 mask;
    /* 0x1E */ u16 hit;
} SpinHitMask;

/* NAS.FUX of an object in the HeadOfObjects list (SpinHeroObjectNear). */
typedef struct SpinFux {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 kind;
    /* 0x16 */ s16 owner;
    /* 0x18 */ u8 pad18[0x1A - 0x18];
    /* 0x1A */ u16 sub;
    /* 0x1C */ u16 mask;
    /* 0x1E */ u16 hit;
    /* 0x20 */ u8 pad20[0x22 - 0x20];
    /* 0x22 */ s16 timer;
    /* 0x24 */ s16 state;
    /* 0x26 */ s16 angle;
    /* 0x28 */ s16 dist;
} SpinFux;

/* balloon chance weights, indexed by score and 0x20 - score (SpinScoreBalloon) */
extern u16 SpinBalloonWeights[];

typedef struct {
    u8 unk0;
    u8 unk1;
} SpinMapCell;
extern SpinMapCell ArenaGrid[400];
extern u16 D_8009E25C[13];

void SpinHitHeroes(EXTengine *eng, SpinHitMask *m, u32 flag, s32 hero);

/* FindExtension(nas, 0x800) of a hazard (PlotHazardType). */
typedef struct SpinGlow {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x18 - 0xD];
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
} SpinGlow;
s32 GravitateNas(NAS *nas);
void func_8008E27C(s32 *arg0, void *arg1, s32 arg2);

void SpinHeroFlip(s32 arg0);

extern LevSpec LevelSpecs; /* the level's hooks */
extern u8 RingObjectsModel[];
extern u8 CrateObjectsModel[];
extern u8 PowerupsModel[];
extern u16 SpinShadowModels[];

void SpinHeroWinner(s32 arg0);
extern u16 SpinBalloonAnims[];
extern s32 World_floor_level;
extern s32 D_8005B5B8;
extern HeroInfo Heroes[4];

/* PlotHazardType: let go of a hazard its owner is holding. */
static inline void SpinDropHazard(SpinFux *fux) {
    HeroInfo *hb;
    HeroInfo *h;
    s16 owner;
    _sfx_handler *sfx;

    owner = fux->owner;
    if (owner != -1 && owner < 5) {
        hb = Heroes;
        h = &hb[owner];
        if (h->hero_object == (EXTobject *)fux) {
            h->hero_object = NULL;
            sfx = hb[fux->owner].hero_eng->sfx;
            if (sfx != NULL) {
                StopSfx(sfx);
            }
        }
    }
}
extern s32 GameTeams;
extern s16 AiPlayerSkill[][3];
extern u16 CameraMode;
FlagRecord *FindCollisionBox(EXTengine *eng, u32 flag);

void ApplyContour(EXTengine *eng);
void *FindExtension(NAS *nas, s32 id);

s32 CheckHeroAnim();
s32 func_8008A200(s32 arg0);
void FinishLevel(void);

s32 SpinAI_HazardAhead(s32 arg0);

extern NAS *HeadOfSolids;

void SpinInitScene(void);

s32 SpinAI_PickPlayer(s32 arg0);

void SetSpinHeroDead(s32 arg0);

extern u16 SpinBigDrop;
void SpinHero_ExtraEffects(s32 arg0);

void SpinHeroDead(s32 arg0);
extern void *HeadOfObjects;
s32 get_angle(s32 x, s32 z);
s32 angle_diff_abs(s32 arg0, s32 arg1);

void SetSpinHeroFall(s32 arg0);

void SpinCreateShadow(s32 arg0);

s32 SpinHeroTestFlip(s32 arg0);

s32 SpinTestEnd(s32 arg0);

s32 SpinTestEndB(s32 arg0);

typedef struct SpinPair {
    s32 a;
    s32 b;
} SpinPair;

extern u16 SpinDropPos;
SpinPair *SpinFindDropPoint(u16 *head);
NAS *CreateBalloon(s32 a, s32 b, u16 c, s16 d);
extern u16 SpinSlotPos;
static inline u16 SpinNextSlot(void) {
    SpinSlotPos = (SpinSlotPos + 1) & 3;
    return SpinSlotPos;
}
extern s16 SpinDropKinds[];
extern u16 D_8009E46C[];
extern u32 Clock_Time;
extern u16 SpinHazardKinds[];
NAS *CreateHazard(s32 a, s32 b, u16 c, s16 d);

void SpinDropObject(u16 arg0, u16 arg1);

void SpinHeroLose(s32 arg0);
extern u16 SpinHitSfx[];
extern u16 SpinHitDone[];
extern s32 GameCheats;
s32 ProcessHeroAnim();
s32 func_800871E4(s32 arg0);
s32 SetHeroSeq(s32 player, s32 seq);

void SpinShuffleDropPoints(void);

s32 SPIN_KeepInBounds(EXTengine *eng);

void SetSpinHeroHit(s32 arg0);

void SpinHeroHit(s32 arg0);

void SpinHeroFall(s32 arg0);

void SetSpinHeroWinner(s32 arg0);

void SetSpinHeroLose(s32 arg0);
extern u8 GameInfo[];

/* Hero jump parameters, 8 bytes per hero (HeroHooks.JumpEnts). */
typedef struct SpinJumpEnt {
    u16 vel, max, acc, dec;
} SpinJumpEnt;

typedef struct SpinHooks {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ u16 *HeroModels;
    /* 0x08 */ u8 pad8[0x4];
    /* 0x0C */ u16 *RunSpeeds;
    /* 0x10 */ SpinJumpEnt *JumpEnts;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ void *CreateShadow;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ void *PreMoveHero;
    /* 0x24 */ u8 pad24[0x14];
    /* 0x38 */ void *HeroBot;
    /* 0x3C */ void *TestAttack;
    /* 0x40 */ u8 pad40[4];
    /* 0x44 */ void *TestJump;
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ void *Hero2ObjectCollision;
    /* 0x50 */ void *HeroGrabObject;
    /* 0x54 */ void (*HeroKickCollision)();
    /* 0x58 */ void (*HeroCarryCollision)(s32);
    /* 0x5C */ void *HeroExtraEffects;
    /* 0x60 */ void *HeroesTouched;
    /* 0x64 */ void *ChooseWinner;
    /* 0x68 */ s32 (*KeepInBounds)(EXTengine *);
} SpinHooks;
extern SpinHooks HeroHooks;

s32 SpinAI_NearestEnemy(s32 arg0);
extern void *AiBotTweakTable;
extern void *AiHumanTweakTable;
extern void *AiSkillTable;
extern void *AiTeamSkillTable;
extern void *AiHumanRoundTweakTable;
extern s16 HeroActHit;
extern s16 HeroActDead;
extern s16 HeroActWinner;
extern s16 HeroActLoser;
extern u16 SpinAiSkill[];
extern u16 SpinAiSkillAlt[];
extern u16 SpinAiTeamSkill[];
extern u16 SpinAiHumanRoundTweak[];
extern u16 SpinAiBotTweak[];
extern u16 SpinAiHumanTweak[];
extern SpinJumpEnt SpinJumpEnts[];
extern u16 SpinRunSpeeds[];
void SpinChooseWinner(s32 arg0, u16 arg1);
void Spin_HeroBot();
void SpinInitASTIndices();
void SpinPreMoveHero();
NAS *SpinHero2ObjectCollision(s32 player, s32 mask, s32 unused, s32 cone);
NAS *SpinHeroGrabObject(s32 idx, s32 mask);
s32 SpinHeroKickCollision(s32 arg0);
s32 SpinHeroCarryCollision(s32 idx);

/* Extension 0x1000 flags set by SpinHeroKickCollision and SpinHeroCarryCollision. */
typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[2];
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} SpinHitFlags;

s32 SpinRankHeroes(void);

void SpinCountObjects(void);

extern u16 SpinDropTimer;
extern u16 SpinDropDelay;
extern u16 SpinDropCount;
extern u16 SpinBalloonTimer;

void SpinDropMaster(void);

void SpinDropObjects(u16 count, s16 arg1);

void SetSpinHeroFlip(s32 arg0);

SpinFux *SpinHeroObjectNear(s32 hero, u16 mask, s32 r);

/* InitialiseSPIN */
extern u16 SpinUnused1;
extern u16 SpinBalloonOwner;
extern s32 SpinBalloonFlag;
extern EXTengine SpinSfxEngine;
extern s32 AngelModel;

void SpinTurnArena(void);

void PlotHazardType(NAS *nas);

void SpinScoreBalloon(SpinFux *arg0);

s32 SpinBalloonCollide(EXTengine *eng, EXTmotion *mot, SpinFux *fux);

s32 SpinAI_ChooseTarget(s32 player);

/* PlotBalloon */
extern u16 SpinBalloonModels[];
extern s32 *SpinBalloonSfx[];

void PlotBalloon(NAS *nas);

void SpinAI_ScanAhead(s32 arg0);

/* Spin_HeroBot */
extern u32 DirPadBits[]; /* pad flags by angle octant */
s32 csqrt(s32 x);
s32 HeroTestBox(s32 id, u32 flag);

#endif /* BASH_SPIN_OVL_H */
