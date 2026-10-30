#ifndef BASH_SPIN_OVL_H
#define BASH_SPIN_OVL_H

/* Functions and data defined in the spin overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/spin/spin.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"


typedef struct SpinSlot {
    /* 0x00 */ subengine *obj;
    /* 0x04 */ s16 fixed[3]; /* used as is when nonzero */
    /* 0x0A */ s16 rate[3];  /* else SpinTurn * rate >> 4 */
} SpinSlot;

typedef struct SpinCell {
    s32 x;
    s32 y;
} SpinCell;

extern SpinCell SpinDropPoints[4][16];



#define SPIN_GAME ((GameState *)GameInfo)

/* Sets the round's result to a draw: every hero still standing loses. */
#define SPIN_DRAW(g, arg1, i)                          \
    do {                                                \
        (g)->winner[0] = 0;                             \
        (g)->exitFlags |= (arg1);                           \
        for ((i) = 0; (i) < 4; (i)++) {                 \
            if (!(Heroes[i].hero_status & 0xFA00)) {    \
                Heroes[i].hero_status |= 0x400;         \
            }                                           \
        }                                               \
    } while (0)

/* A hit (will_do 2|8) pops the object: status 9, will_do and can_do 2|8 cleared. */
#define SPIN_CHECK_POP(fux)                            \
    do {                                                \
        if ((fux)->will_do & 0xA) {                         \
            (fux)->status = 9;                             \
            (fux)->will_do &= 0xFFF5;                       \
            (fux)->can_do &= 0xFFF5;                      \
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
        (y) = (eng)->XYZ.y;                           \
        (y) += 0x300;                                   \
        (s) = (y) - (g);                                \
        if (abs(s) > 0x10) {                            \
            (eng)->XYZ.y -= (s) >> 6;                 \
        }                                               \
    } while (0)

/* Puts the engine's shadow (its child engine) on the ground below it. */
#define SPIN_SHADOW(eng, shadow)                                                   \
    do {                                                                            \
        (shadow) = (eng)->Engine;                                                   \
        if ((shadow) != NULL) {                                                     \
            (shadow)->XYZ = (eng)->XYZ;                                         \
            (shadow)->XYZ.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);        \
        }                                                                           \
    } while (0)

extern s32 SpinBestScore;
extern s32 SpinBestHero;
extern s32 SpinTurn;
extern s32 SpinTurnTarget;
extern s16 SpinAngle;
extern s16 SpinAngle2;
extern s16 SpinSpeed;
extern subengine *SpinArenaObjs[10];
extern SpinSlot SpinSlots[10];

#include "bash/main.h"

extern s32 SpinASTBase;

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
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u16 mask;
    /* 0x1E */ u16 hit;
} SpinHitMask;


/* balloon chance weights, indexed by score and 0x20 - score (SpinScoreBalloon) */
extern u16 SpinBalloonWeights[];


void SpinHitHeroes(EXTengine *eng, SpinHitMask *m, u32 flag, s32 hero);


void SpinHeroFlip(s32 arg0);

extern u16 SpinShadowModels[];

void SpinHeroWinner(s32 arg0);
extern u16 SpinBalloonAnims[];

/* PlotHazardType: let go of a hazard its owner is holding. */
static inline void SpinDropHazard(EXTobject *fux) {
    HeroInfo *hb;
    HeroInfo *h;
    s16 owner;
    _sfx_handler *sfx;

    owner = fux->player;
    if (owner != -1 && owner < 5) {
        hb = Heroes;
        h = &hb[owner];
        if (h->hero_object == (EXTobject *)fux) {
            h->hero_object = NULL;
            sfx = hb[fux->player].hero_eng->sfx;
            if (sfx != NULL) {
                StopSfx(sfx);
            }
        }
    }
}



s32 SpinAI_HazardAhead(s32 arg0);


void SpinInitScene(void);

s32 SpinAI_PickPlayer(s32 arg0);

void SetSpinHeroDead(s32 arg0);

extern u16 SpinBigDrop;
void SpinHero_ExtraEffects(s32 arg0);

void SpinHeroDead(s32 arg0);

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
extern u16 SpinHazardKinds[];
NAS *CreateHazard(s32 a, s32 b, u16 c, s16 d);

void SpinDropObject(u16 arg0, u16 arg1);

void SpinHeroLose(s32 arg0);
extern u16 SpinHitSfx[];
extern u16 SpinHitDone[];

void SpinShuffleDropPoints(void);

s32 SPIN_KeepInBounds(EXTengine *eng);

void SetSpinHeroHit(s32 arg0);

void SpinHeroHit(s32 arg0);

void SpinHeroFall(s32 arg0);

void SetSpinHeroWinner(s32 arg0);

void SetSpinHeroLose(s32 arg0);



s32 SpinAI_NearestEnemy(s32 arg0);
extern u16 SpinAiSkill[];
extern u16 SpinAiSkillAlt[];
extern u16 SpinAiTeamSkill[];
extern u16 SpinAiHumanRoundTweak[];
extern u16 SpinAiBotTweak[];
extern u16 SpinAiHumanTweak[];
extern HeroJumpEnt SpinJumpEnts[];
extern u16 SpinRunSpeeds[];
void SpinChooseWinner(s32 arg0, u16 arg1);
void Spin_HeroBot(s32 arg0);
void SpinInitASTIndices(void);
void SpinPreMoveHero(s32 arg0);
NAS *SpinHero2ObjectCollision(s32 player, s32 mask, s32 unused, u16 cone);
NAS *SpinHeroGrabObject(s32 idx, s32 mask);
s32 SpinHeroKickCollision(s32 arg0);
s32 SpinHeroCarryCollision(s32 idx);


s32 SpinRankHeroes(void);

void SpinCountObjects(void);

extern u16 SpinDropTimer;
extern u16 SpinDropDelay;
extern u16 SpinDropCount;
extern u16 SpinBalloonTimer;

void SpinDropMaster(void);

void SpinDropObjects(u16 count, s16 arg1);

void SetSpinHeroFlip(s32 arg0);

EXTobject *SpinHeroObjectNear(s32 hero, u16 mask, s32 r);

/* InitialiseSPIN */
extern u16 SpinUnused1;
extern u16 SpinBalloonOwner;
extern s32 SpinBalloonFlag;
extern EXTengine SpinSfxEngine;

void SpinTurnArena(void);

void PlotHazardType(NAS *nas);

void SpinScoreBalloon(EXTobject *arg0);

s32 SpinBalloonCollide(EXTengine *eng, EXTmotion *mot, EXTobject *fux);

s32 SpinAI_ChooseTarget(s32 player);

/* PlotBalloon */
extern u16 SpinBalloonModels[];
extern s32 *SpinBalloonSfx[];

void PlotBalloon(NAS *nas);

void SpinAI_ScanAhead(s32 arg0);

/* Spin_HeroBot */

#endif /* BASH_SPIN_OVL_H */
