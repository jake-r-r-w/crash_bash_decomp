#ifndef BASH_POLAR_OVL_H
#define BASH_POLAR_OVL_H

/* Functions and data defined in the polar overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/polar/polar.c */

#include "common.h"
#include "bash/nas.h"

/* defined later in polar.c than the data that points at it */
extern const char PolarAnimWin[];


/* Per-character tables SetPolarHeroHit hangs on hero_eng->sub->unk6C. */

/* Per-character tables SetPolarHeroIdle hangs on hero_eng->sub->unk6C. */

void PolarEmptyStub3(void);
s32 PolarAI_Never(void);
void PolarEmptyStub4(void);
void PolarEmptyStub5(void);
void PolarEmptyStub6(void);
void PolarEmptyStub7(void);
s32 PolarPadAngle(s32 arg0);
s32 PolarCompareXZ(u8 *arg0, u8 *arg1);
u32 PolarAngleOf(u8 *arg0);
void SetPolarHeroTransmute(s32 player);
s32 TestPolarHeroWon();
void SetPolarHeroWeight(void);
void PolarInitHero(void);
void PolarEmptyStub0(void);
void PolarEmptyStub1(void);
void PolarEmptyStub2(void);

extern NAS *PolarBomb;

/* InitBombGFX's extension (0x1C bytes). */
typedef struct BombGFXExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
} BombGFXExt;

/* A scene's model object as PolarInitDebug walks the list at MapScene->subs. */
typedef struct SceneObj {
    /* 0x00 */ u32 flags; /* 0x8000 visible */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x12 - 0x10];
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 pad14[0x51 - 0x14];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad54[0x5C - 0x54];
    /* 0x5C */ struct SceneObj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ obj_dbase *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} SceneObj;

/* Visible scene objects PolarInitDebug collects. */
extern SceneObj *PolarIceObjects[80];
extern const char PolarMsgFoundColSubs[];

/* Scene objects PolarInitScene picks out by index. */
extern s32 PolarTiltX;
extern s32 PolarTiltZ;
extern SceneObj *PolarSceneA;
extern SceneObj *PolarSceneB;
extern s32 PolarSceneCount;
extern s32 PolarSceneTimer;



/* A value BearminatorEaseCam eases toward its target by a fixed step. */
typedef struct PolarEase {
    /* 0x00 */ s32 cur;
    /* 0x04 */ s32 target;
    /* 0x08 */ s16 step;     /* used while target != 0 */
    /* 0x0A */ s16 stepZero; /* used while target == 0 */
    /* 0x0C */ u8 padC[0x20 - 0xC];
} PolarEase;

/* The camera offsets BearminatorEaseCam eases and subtracts from a position. */
typedef struct PolarEaseCam {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ PolarEase x;
    /* 0x40 */ PolarEase z;
} PolarEaseCam;

typedef struct Ext10000000 {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 unkC;
} Ext10000000;

/* The 0x10000000 extension of the object PolarBombHitHeroes spawns (0x1611). */
typedef struct PolarBombHit {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 angle;
    /* 0x14 */ SLONG_XYZ pos;
    /* 0x20 */ SLONG_XYZ start;
} PolarBombHit;

/* The 0x10000000 extension of the object SetPolarHeroThrowBomb spawns (0x160D). */
typedef struct PolarThrown {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 owner;
    /* 0x18 */ SLONG_XYZ pos;
    /* 0x24 */ SLONG_XYZ start;
    /* 0x30 */ u8 pad30[0x74 - 0x30];
    /* 0x74 */ s32 unk74;
} PolarThrown;

/* Extension 0x10000000 of the object PolarSpawnObjects spawns (NAS 0x1605). */
typedef struct ExtSpawn5910 {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x20 - 0x1C];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x28 - 0x24];
    /* 0x28 */ s32 unk28;
} ExtSpawn5910;

/* Extension 0x10000000 of the smoke trail PlotSmokeTrail moves. */
typedef struct ExtSmokeTrail {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 fadeIn;
    /* 0x10 */ s32 fadeOut;
    /* 0x14 */ s32 shrink;
    /* 0x18 */ s32 flags;
    /* 0x1C */ s32 rise;
    /* 0x20 */ s32 baseX;
    /* 0x24 */ s32 amp;
    /* 0x28 */ s32 ampDecay;
    /* 0x2C */ s32 angle;
} ExtSmokeTrail;

extern s32 PolarRoundState;

/* PolarBeamOwner's 0x40000000 extension as SetPolarHeroWinner uses it: engines at
 * 0x74 and four at 0x78. */
typedef struct PolarGameExt {
    /* 0x00 */ u8 pad[0x74];
    /* 0x74 */ EXTengine *unk74;
    /* 0x78 */ EXTengine *unk78[4];
} PolarGameExt;

/* PolarHeroAttackThrow: aim the hero's x/z velocity and max along forward_angle at
 * PolarHeroThrowPower's speed. Stored through a pointer so a later `max = 0` keeps
 * the max store (flow deletes it otherwise). */
#define POLAR_AIM(move, player)                                                    \
    {                                                                              \
        s32 spd_ = PolarHeroThrowPower(player);                                          \
        s32 x_;                                                                    \
        s32 z_;                                                                    \
        s32 *p_;                                                                   \
        x_ = (spd_ * rcossin_tbl[((move)->forward_angle + 0x400) & 0xFFF].unk2) >> 12; \
        p_ = &(move)->x_vel;                                                       \
        p_[0] = x_;                                                                \
        p_[1] = x_;                                                                \
        z_ = (spd_ * rcossin_tbl[((move)->forward_angle + 0x800) & 0xFFF].unk2) >> 12; \
        p_ = &(move)->z_vel;                                                       \
        p_[0] = z_;                                                                \
        p_[1] = z_;                                                                \
    }

typedef struct Ext20 {
    /* 0x00 */ u8 pad[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x4];
    /* 0x20 */ s32 val;
    /* 0x24 */ s32 state;
    /* 0x28 */ u8 pad28[0xA4 - 0x28];
    /* 0xA4 */ NAS *unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
} Ext20;

/* PadState (Pads) as PolarHeroFootIdle and PolarHeroInBeam use it: the
 * buttons at 0x50, a word at 0x54. */

void PolarHeroFootIdle(s32 player);

/* PadState (Pads) as PolarAI_SteerToTarget writes it: stick angle at 0x38,
 * buttons at 0x50. */


/* Inlined into PolarAI_SteerToTarget: the d-pad bits that steer from v toward p. */
static inline u32 PolarPadToward(SLONG_XYZ *p, SLONG_XYZ *v) {
    u32 flags;

    flags = 0;
    if (p->x > v->x) {
        flags = 0x80;
    }
    if (p->x < v->x) {
        flags |= 0x20;
    }
    if (p->z > v->z) {
        flags |= 0x40;
    }
    if (p->z < v->z) {
        flags |= 0x10;
    }
    return flags;
}

extern HeroInfo Heroes[4];

/* Extension 0x10000000 made by InitSimpleExplosion: a velocity per sub-engine. */
typedef struct ExtSimpleExplosion {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ SLONG_XYZ vel[20];
    /* 0x104 */ s32 unk104;
} ExtSimpleExplosion;


/* The object PlotBrioPack follows (its NAS.UserInfo.user1). */
typedef struct BrioAnim {
    /* 0x00 */ u8 pad0[0x1E];
    /* 0x1E */ s16 frame;
} BrioAnim;

typedef struct BrioOwner {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ EXTengine *engine;
    /* 0x08 */ u8 pad8[0x1C - 0x8];
    /* 0x1C */ BrioAnim *anim;
    /* 0x20 */ u8 pad20[0x2C - 0x20];
    /* 0x2C */ u16 type;
    /* 0x2E */ u16 subtype;
} BrioOwner;

/* Head of a list linked through +0x20 (counted by CountBears). */
extern u8 *PolarBears;
extern s32 GameTeams[];

/* TestPolarHeroMelted's ending: put the hero into action 0x4E unless status bit
 * 0x200 is set. */
static inline s32 PolarEndAction(s32 player) {
    if (Heroes[player].hero_mode == 0x4C) {
        if (!(Heroes[player].hero_status & 0x200)) {
            SetHeroAction(player, 0x4E);
        }
    } else if (!(Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x4E) {
        SetHeroAction(player, 0x4E);
    }
    return 0;
}

/* PolarBoxOverlap's box around an engine: min/max x,z corners and push radius. */
typedef struct PolarBox {
    SLONG_XYZ min;
    SLONG_XYZ max;
    s32 r;
} PolarBox;

s32 PolarBoxOverlap(EXTengine *a, s32 b, EXTengine *c, s32 d);

void GravityMotionUpdate();

/* A gravity object's motion block (GravityObject.target). */
typedef struct GravityMotion {
    /* 0x00 */ u8 pad0[0xD];
    /* 0x0D */ u8 flags;
    /* 0x0E */ s16 vx;
    /* 0x10 */ s16 ax;
    /* 0x12 */ u8 pad12[0x16 - 0x12];
    /* 0x16 */ s16 rx;
    /* 0x18 */ s16 vy;
    /* 0x1A */ s16 ay;
    /* 0x1C */ u8 pad1C[0x20 - 0x1C];
    /* 0x20 */ s16 ry;
    /* 0x22 */ s16 vz;
    /* 0x24 */ s16 az;
    /* 0x26 */ u8 pad26[0x2A - 0x26];
    /* 0x2A */ s16 rz;
    /* 0x2C */ u8 pad2C[0x2E - 0x2C];
    /* 0x2E */ s16 floor;
} GravityMotion;

typedef struct GravityObject {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 target;
} GravityObject;

void PlotGravityObject(GravityObject *obj);
/* A hero's FUX extension as TestWanderChance sets it up. */
typedef struct PolarWanderFux {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ SLONG_XYZ vel;
    /* 0x18 */ SLONG_XYZ pos;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 target;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 state;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[0x3C - 0x38];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 angle;
    /* 0x44 */ void *func;
    /* 0x48 */ u8 pad48[0x4C - 0x48];
    /* 0x4C */ s32 flags;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 timer;
} PolarWanderFux;

extern s32 PolarAIChances[];
extern s32 PolarAnimFlag;
void PolarAI_DoWander();
void PolarAI_AvoidMelt();
void DoGetBomb(u32 player);
void PolarAI_DoHunt();

/* Inlined into TestChaseChance. */
static inline void PolarStartChase(s32 player) {
    PolarWanderFux *f;
    s32 j;
    s32 n;

    f = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (f == NULL) {
        return;
    }
    for (n = j = 0; j < 4; j++) {
        if (Heroes[j].hero_status & 0x2000) {
            n++;
        }
        f->state = 4;
        f->func = PolarEmptyStub5;
        if (n >= 3) {
            return;
        }
    }
    f->unk3C = 1;
    f->unk24 = Random(0x3C) + 5;
}
s32 TestWanderChance(s32 player);

/* Inlined twice into EnsureMateNotObstructingMate. */
static inline void PolarGrab(s32 player) {
    PolarWanderFux *f;

    f = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (f == NULL) {
        return;
    }
    if (f->state != 6 || f->unk24 == 0) {
        f->unk24 = 0x3C;
    }
    f->state = 6;
    f->func = PolarAI_AvoidMelt;
    f->unk2C = 0;
    f->unk3C = 1;
    f->unk34 |= 1;
}

s32 PolarGetLevelPoint(s32 *arg0, s32 arg1);


void PolarHeroThrowBomb(s32 player);

void PolarHeroAddWeight(s32 arg0);

/* Bomb maker extension (0x1C bytes, id 0x10000000). */
typedef struct BombMakerExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 timer;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} BombMakerExt;

extern s32 PolarClock;
extern VECTOR PolarCentre;
/* DoesPlayerHaveBomb, inlined into PolarAI_DoBeam. */
static inline s32 PolarHasBomb(s32 player) {
    NAS *nas;
    BombMakerExt *ext;
    s32 one;
    s32 found;

    nas = PolarBomb;
    one = 1;
    found = 1;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        do {
            if (ext != NULL && ext->unkC == one && ext->unk14 == player) {
                goto done;
            }
        } while (0);
        nas = nas->Group.Child;
        goto loop;
    }
    found = 0;
done:
    return found;
}

/* FindClosetPlayer, inlined into PolarAI_DoChaseBomber. */
static inline s32 PolarClosest(s32 self, s32 *dist) {
    SLONG_XYZ pos;
    s32 best;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    s32 i;
    s32 d;
    s32 min;
    u16 st;

    best = -1;
    min = 0x07FFFFFF;
    i = 0;
    base = Heroes;
    me = &base[self];
    h = base;
loop:
    if (i < 4) {
        do {
            if (i != self && !((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
                pos = h->hero_eng->unk10;
                d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->unk10);
                if (d < min) {
                    min = d;
                    best = i;
                }
            }
            h++;
            i++;
        } while (0);
        goto loop;
    }
    *dist = min;
    return best;
}

/* DoesPlayerHaveBomb as inlined into DoGetBomb. */
static inline s32 PolarHasBomb2(s32 player) {
    NAS *nas;
    BombMakerExt *ext;
    s32 one;

    nas = PolarBomb;
    one = 1;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->unkC == one && ext->unk14 == player) {
            return 1;
        }
        nas = nas->Group.Child;
        goto loop;
    }
    return 0;
}

/* IsBombAvailable, inlined into DoGetBomb. */
static inline NAS *PolarFreeBomb(void) {
    NAS *nas;
    BombMakerExt *ext;

    nas = PolarBomb;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->unkC == 0) {
            return nas;
        }
        nas = nas->Group.Child;
        goto loop;
    }
    return NULL;
}

void PolarAI_StartAttack(s32 player);
void PolarAI_DoAttack(s32 player);

extern s32 PolarAttackOdds[];
extern s32 PolarRollOdds[];
extern s32 HeroesAllOut;

extern s32 PolarIceRadius;
s32 PolarRayHit(VECTOR *a, VECTOR *b, SVECTOR *c, VECTOR *out);


/* Roll the odds in PolarRollOdds (by AiPlayerSkill[player].level) and set ext->unk14. */
static inline void RollPolarOdds(s32 player, Ext20 *ext) {
    s32 r;
    s32 limit;

    r = Random(0x1000);
    if (AiPlayerSkill[player].level > 0x30) {
        AiPlayerSkill[player].level = 0x30;
    }
    limit = PolarRollOdds[AiPlayerSkill[player].level] << 7;
    if (GameMode == 0x12) {
        limit += 0x400;
    }
    if (r < limit) {
        ext->unk14 = 8;
    }
    if (HeroesAllOut != 0) {
        ext->unk14 = 0;
    }
}

s32 DistXZSq(SLONG_XYZ *a, SLONG_XYZ *b);
s32 PolarAI_ShotBlocked(s32 arg0);
s32 PolarHeroAttackPower(s32 arg0);

/* The NAS that owns the bomb maker. */

s32 PolarBossDazeHero(u8 *arg0);


void PolarHeroSquashed(s32 player);

void PolarHeroAttack(s32 player);

void PolarHeroPoll(s32 player);

extern s32 PolarAIFlags[4];
extern s32 PolarAIPace[4];

s32 PolarBombOwnerEngine(NAS *nas);

void PolarSpawnObjects(void);

/* One of the 16 angular sectors CheckMelted checks. */
typedef struct PolarSector {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 val;
    /* 0x08 */ s32 pad8[2];
} PolarSector;

/* Read by PolarBeamLinkState and CheckMelted. */
typedef struct PolarLink {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 unk28[(0x6C - 0x28) / 4];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[0xBC - 0x70];
    /* 0xBC */ PolarSector sectors[16];
} PolarLink;

typedef struct PolarOwner {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ EXTengine *unk54;
    /* 0x58 */ PolarLink *unk58;
} PolarOwner;

extern PolarOwner *PolarBeamOwner;
extern s32 PolarBeamMode;

/* Read by PolarAI_Proximity. */
typedef struct PolarMode {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 mode;
} PolarMode;

typedef struct PolarProx {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ EXTengine *eng;
    /* 0x58 */ PolarMode *unk58;
} PolarProx;

extern PolarProx *PolarProximity;

/* Action choosers DoChooseNewAction tries in turn from a random start. */
extern s32 (*PolarAITests[5])(s32 player);

/* DoChooseNewAction's "attack target t" step: chase t, and send a team mate
   after one of the other team. */
#define POLAR_ATTACK(fux, h, player, t, f, r)                    \
    {                                                            \
        if ((fux)->state != 1 || (fux)->unk24 == 0) {            \
            (fux)->unk24 = 0xD2;                                 \
        }                                                        \
        (fux)->state = 1;                                        \
        (fux)->unk3C = 1;                                        \
        (fux)->func = PolarAI_DoAttack;                             \
        (fux)->target = (t);                                     \
        (fux)->unk34 &= ~2;                                      \
        (f) = (PolarWanderFux *)(h)->hero->FUX;                        \
        if ((f) != NULL && TestForTeamMate((player), (t))) {     \
            (r) = PolarAI_TeamChoice(player);                         \
            if ((r) == -1) {                                     \
                (f)->state = 5;                                  \
                (f)->func = PolarEmptyStub6;                       \
            } else {                                             \
                (f)->target = (r);                               \
            }                                                    \
        }                                                        \
    }

/* Escape headings tried by PolarAI_DoIceWalk. */
extern s32 PolarMeltOffsets[8];

/* PolarUpdateScene's swaying-ground state. */
extern s32 PolarSceneWord0;
extern s32 PolarSceneWord1;
extern s32 PolarSceneWord2;
extern s32 PolarSceneWord3;
extern MATRIX PolarSceneMatrix;

void PolarAI_TryAttack(s32 player);

void PolarAI_DoChase(s32 player);


s32 PolarBossNear(EXTengine *eng, s32 player);

s32 PolarBossSpeedUp(void);

void PolarCamReset(void);


void PolarBeamLinkState(void);

void PolarAI_ClearState(s32 player);
void PolarAI_AvoidBomber();
void PolarAI_DoChaseBomber();

s32 PolarAI_AnyBusy(void);
s32 ProcessHeroAnim();

s32 PolarAI_NoneBusy(s32 arg0);

void PolarAI_Reset(s32 player);

void PolarAI_Timer2(s32 player);

void PolarAI_Timer3(s32 player);

void POLARSetUp(void);

s32 PolarAI_TargetShare(s32 arg0, s32 arg1);

void PolarAI_ResetPace(void);

s32 PolarClearHitBy(s32 arg0);

s32 PolarTestAttack(s32 player);

s32 PolarHeroThrowPower(s32 player);

s32 PolarTestSnowball(s32 player);

void PolarHeroHit(s32 player);

void PolarHeroInBeam(s32 player);

/* A rising slot as RiserSetReset steps it: 16 of them at +0xB8. */
typedef struct PolarRiser {
    /* 0x0 */ s32 active;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 height;
    /* 0xC */ s16 base;
    /* 0xE */ s16 padE;
} PolarRiser;

typedef struct PolarRiserSet {
    /* 0x00 */ u8 pad0[0xB8];
    /* 0xB8 */ PolarRiser slot[16];
} PolarRiserSet;

/* UkaMaskSetSegments's ring of 16 mask segments at +0xB8 of the Uka polar ext. */
typedef struct UkaMaskSeg {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 state;
    /* 0x8 */ s32 t;
    /* 0xC */ s32 angle;
} UkaMaskSeg;

typedef struct UkaMaskSet {
    /* 0x00 */ u8 pad0[0xB8];
    /* 0xB8 */ UkaMaskSeg seg[16];
} UkaMaskSet;

void UkaMaskSetSegments(s32 i, UkaMaskSet *m);

/* InitUkaPolarMask's extension (id 0x40000000, 0x1E0 bytes). */
typedef struct UkaPolarExt {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ s32 unk18[4];
    /* 0x028 */ s32 unk28[4];
    /* 0x038 */ s32 unk38[4];
    /* 0x048 */ s32 unk48[4];
    /* 0x058 */ s32 unk58;
    /* 0x05C */ s32 unk5C;
    /* 0x060 */ s32 unk60;
    /* 0x064 */ s32 unk64;
    /* 0x068 */ s32 unk68;
    /* 0x06C */ s32 unk6C;
    /* 0x070 */ s32 unk70;
    /* 0x074 */ EXTengine *eng;
    /* 0x078 */ u8 pad78[0x88 - 0x78];
    /* 0x088 */ SLONG_XYZ pos;
    /* 0x094 */ u8 pad94[0x9C - 0x94];
    /* 0x09C */ s32 unk9C;
    /* 0x0A0 */ u8 padA0[0xA8 - 0xA0];
    /* 0x0A8 */ NAS *bit;
    /* 0x0AC */ NAS *unkAC;
    /* 0x0B0 */ NAS *mask;
    /* 0x0B4 */ s32 unkB4;
    /* 0x0B8 */ UkaMaskSeg seg[16];
    /* 0x1B8 */ s32 unk1B8;
    /* 0x1BC */ s32 unk1BC;
    /* 0x1C0 */ s32 angle;
    /* 0x1C4 */ s32 unk1C4;
    /* 0x1C8 */ s32 unk1C8;
    /* 0x1CC */ s32 unk1CC;
    /* 0x1D0 */ s32 unk1D0;
    /* 0x1D4 */ s32 unk1D4;
    /* 0x1D8 */ s32 unk1D8;
    /* 0x1DC */ s32 unk1DC;
} UkaPolarExt;

/* The 0x1605 bit's extension (id 0x10000000) InitUkaPolarMask sets up. */
typedef struct UkaBitExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 unk28;
} UkaBitExt;

/* InitTransmuterBeam's extension (id 0x40000000, 0x1E0 bytes). */
typedef struct TransBeamExt {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ s32 unk18[4];
    /* 0x028 */ s32 unk28[4];
    /* 0x038 */ s32 unk38[4];
    /* 0x048 */ s32 unk48[4];
    /* 0x058 */ s32 unk58;
    /* 0x05C */ s32 unk5C;
    /* 0x060 */ s32 unk60;
    /* 0x064 */ s32 unk64;
    /* 0x068 */ s32 unk68;
    /* 0x06C */ s32 unk6C;
    /* 0x070 */ s32 unk70;
    /* 0x074 */ EXTengine *eng;
    /* 0x078 */ EXTengine *engs[4];
    /* 0x088 */ SLONG_XYZ pos;
    /* 0x094 */ s32 unk94;
    /* 0x098 */ s32 unk98;
    /* 0x09C */ s32 unk9C;
    /* 0x0A0 */ s32 unkA0;
    /* 0x0A4 */ NAS *bit1;
    /* 0x0A8 */ NAS *bit2;
    /* 0x0AC */ NAS *unkAC;
    /* 0x0B0 */ u8 padB0[0x1DC - 0xB0];
    /* 0x1DC */ s32 unk1DC;
} TransBeamExt;


#define TBV ((GameState *)GameInfo)
#define TB_MODE13() ((TBV->levelFlags & 0x80) && TBV->mode == 0x13)

/* The beam kinds PlotTransmuterBeam picks from (Random(4)). */
extern u32 BeamFrames[4];

extern s32 BeamState;
extern s32 BeamTimer;
extern s32 BeamTarget;
extern s32 UkaMaskFrames[4];
extern s32 UkaMaskTimer;
extern s32 BeamCount;
extern s32 BeamDelay;

typedef struct PolarRiserObj {
    /* 0x00 */ u8 pad0[0x74];
    /* 0x74 */ s16 y;
} PolarRiserObj;

extern PolarRiserObj *UkaMaskRisers[16];

/* Scene objects with index 0x4000, UkaMaskInitRisers collects. */
extern SceneObj *UkaMaskObjects[16];

void RiserSetReset(PolarRiserSet *arg0);

/* Walrus fish extension (id 0x10000000): a ballistic arc (ArcUpdate's
 * Arc, declared in bash/pause.h) at 0x10. */
typedef struct WalrusFishExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 state;
    /* 0x10 */ s32 arc[0xD];
    /* 0x44 */ s32 vx; /* arc vx */
    /* 0x48 */ s32 vz; /* arc vz */
    /* 0x4C */ u8 pad4C[0x54 - 0x4C];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ u8 pad58[0x64 - 0x58];
    /* 0x64 */ SLONG_XYZ to;
    /* 0x70 */ SLONG_XYZ from;
    /* 0x7C */ s32 timer;
    /* 0x80 */ s32 spin;
    /* 0x84 */ ScriptCursor *cursor;
} WalrusFishExt;

/* Polar walrus extension (0x94 bytes, id 0x10000000): arc at 0x14 like
 * WalrusFishExt's at 0x10. */
typedef struct PolarWalrusExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 state;
    /* 0x10 */ s32 timer;
    /* 0x14 */ s32 arc[0xD];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ u8 pad50[0x58 - 0x50];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[0x68 - 0x5C];
    /* 0x68 */ SLONG_XYZ to;
    /* 0x74 */ SLONG_XYZ from;
    /* 0x80 */ ScriptCursor *cursor;
    /* 0x84 */ NAS *unk84;
    /* 0x88 */ s32 speed;
    /* 0x8C */ s32 maxSpeed;
    /* 0x90 */ s32 unk90;
} PolarWalrusExt;

extern s32 PolarBeamTimer;
extern s32 WalrusFrames[];
extern s32 WalrusSteps0[];
extern s32 WalrusSteps1[];
extern s32 WalrusSteps2[];
extern s32 WalrusSteps3[];

/* Polar walrus second extension (0x54 bytes, id 0x20000000). */
typedef struct PolarWalrusExt2 {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x40 - 0x24];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[0x4C - 0x44];
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ u8 pad50[4];
} PolarWalrusExt2;

extern ScriptInit WalrusScriptInit;
extern ScriptInit WalrusFishScriptInit;
extern s32 WalrusState;

s32 ArcUpdate(s32 *from, s32 *to, void *arc, s32 *pos);

/* Big missile extension (id 0x10000000) as PlotBigMissile reads it: a ballistic
 * arc at 0xC (the same block as BigMissileExt, InitBigMissile's view). */
typedef struct BigMissileArcExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 arc[0x15];
    /* 0x60 */ SLONG_XYZ from;
    /* 0x6C */ SLONG_XYZ to;
    /* 0x78 */ s32 state;
} BigMissileArcExt;

/* Polar boss extension (0x94 bytes, id 0x10000000; InitPolarBoss, PlotBigMissile). */
typedef struct PolarBossExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 state;
    /* 0x10 */ ScriptCursor *cursor;
    /* 0x14 */ u8 pad14[0x34 - 0x14];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ u8 pad40[0x54 - 0x40];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s16 unk5C;
    /* 0x5E */ s16 unk5E;
    /* 0x60 */ u8 pad60[0x68 - 0x60];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 ang;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 phase;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ s32 unk80;
    /* 0x84 */ NAS *unk84;
    /* 0x88 */ NAS *unk88;
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ s32 unk90;
} PolarBossExt;

extern NAS *BearminatorNas;
extern ScriptInit PolarBossScriptInit;
void ResetHeroGlobals(void);

extern NAS *polarbossnas;


/* The 0x10000000 extension of the boss's partner (PlotPolarBoss). */
typedef struct PolarBossAnimExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ EXTanimator *anim;
} PolarBossAnimExt;

/* The 0x10000000 extension of the shards PlotPolarBoss spawns (0x161B). */
typedef struct PolarShardExt {
    /* 0x00 */ u8 pad0[0x94];
    /* 0x94 */ s32 unk94;
    /* 0x98 */ u8 pad98[0xB4 - 0x98];
    /* 0xB4 */ s32 unkB4;
} PolarShardExt;

/* PlotPolarBoss: r * sin(ang) in 12-bit fixed point. */
static inline s32 PolarSinMul(s32 ang, s32 r) {
    return (rcossin_tbl[ang & 0xFFF].unk2 * r) >> 12;
}

/* PlotPolarBoss: a random offset in (-n, n). */
static inline s32 PolarRandSpread(s32 n) {
    return Random(n) - Random(n);
}

/* PlotPolarBoss: the chance (out of 0x1000) a shard is special, by level. */
static inline s32 PolarShardChance(void) {
    s32 k;

    if (((GameState *)GameInfo)->adventure.unkAB[22] < 10) {
        return 0;
    }
    if (((GameState *)GameInfo)->adventure.unkAB[22] >= 30) {
        return 0xE66;
    }
    k = 0xB8;
    return (((GameState *)GameInfo)->adventure.unkAB[22] - 9) * k;
}

s32 PolarAI_PickVictim(s32 arg0);

/* Weight death extension (0x28 bytes, id 0x10000000). */
typedef struct WeightDeathExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
} WeightDeathExt;

/* Sniper missile extension (0x38 bytes, id 0x10000000). */
typedef struct SniperMissileExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ SLONG_XYZ from;
    /* 0x1C */ SLONG_XYZ to;
    /* 0x28 */ s32 pitch;
    /* 0x2C */ s32 yaw;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
} SniperMissileExt;

/* XZBoxToSphere's collision box (SubFindFlagPoint's FlagPoint data): two corners. */
typedef struct PolarBoxHalf {
    s16 v[3];
} PolarBoxHalf;

typedef struct PolarCollBox {
    PolarBoxHalf lo;
    PolarBoxHalf hi;
} PolarCollBox;

s32 XZBoxToSphere(EXTengine *a, EXTengine *b, s32 c);

/* Polar missile extension (0x70 bytes, id 0x10000000). */
typedef struct PolarMissileExt {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x2C - 0x1C];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0x38 - 0x30];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ u8 pad3C[0x44 - 0x3C];
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ SLONG_XYZ pos;
} PolarMissileExt;


/* Index of the first live hero within 0x7BC4 of pos (knocked back and a
 * sound played), or -1 (PlotPolarMissile). */
static inline s32 PolarMissileHitHero(SLONG_XYZ *pos) {
    HeroInfo *h;
    u32 i;
    u32 n;

    n = 1;
    if (GameTeams[0] != 0) {
        n = 2;
    }
    i = 0;
    while (1) {
        if (i >= n) {
            return -1;
        }
        h = &Heroes[i];
        if (!(h->hero_mode & 0x2000)
            && DistXZSq((SLONG_XYZ *)pos, (SLONG_XYZ *)&h->hero_eng->unk10) < 0x7BC4) {
            if (h->hero_mode != 0x4B) {
                SetHeroAction(i, 0x4B);
            }
            PlaySfx(0x43D, NULL, NULL, 0x1000, 0x1E00, 0);
            return i;
        }
        i++;
    }
}

/* Spawns the 0x1605 blast at m's position (PlotPolarMissile). */
static inline void PolarMissileBlast(NAS *m, s32 kind) {
    SLONG_XYZ *pos;
    u8 *anim;
    NAS *nas;
    ExtSpawn5910 *x;

    pos = &m->Engine->unk10;
    anim = &HeroModelBank[DB_RoboPolar];
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        x = FindExtension(nas, 0x10000000);
        if (x != NULL) {
            x->unkC = kind;
            x->unk18 = 0x100;
            if (anim != NULL) {
                x->unk20 = 5;
                nas->Engine->sub->u6C.dbase = anim;
            } else {
                x->unk20 = 1;
            }
            x->unk28 = 0;
            nas->InitDelete(nas);
        }
    }
}

/* Polar bomb extension (0x84 bytes, id 0x10000000). */
typedef struct PolarBombExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x18 - 0x14];
    /* 0x18 */ s32 to[3];   /* ArcUpdate target */
    /* 0x24 */ s32 from[3]; /* ArcUpdate start */
    /* 0x30 */ s32 unk30;   /* arc state from here */
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[0x48 - 0x38];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad4C[0x74 - 0x4C];
    /* 0x74 */ s32 unk74;
    /* 0x78 */ u8 pad78[0x84 - 0x78];
} PolarBombExt;

s32 PolarBombHitHeroes(NAS *nas, s32 dist);


/* Big missile extension (0x7C bytes, id 0x10000000). */
typedef struct BigMissileExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x24 - 0x14];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x50 - 0x28];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x60 - 0x54];
    /* 0x60 */ SLONG_XYZ pos;
    /* 0x6C */ u8 pad6C[0x78 - 0x6C];
    /* 0x78 */ s32 unk78;
} BigMissileExt;


/* Value easing towards a target by a step (SniperAimUpdate). */
typedef struct SniperEase {
    /* 0x00 */ s32 cur;
    /* 0x04 */ s32 target;
    /* 0x08 */ s16 step;
    /* 0x0A */ s16 stepZero;
    /* 0x0C */ u8 padC[0x20 - 0xC];
} SniperEase;

typedef struct SniperAim {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ SniperEase x;
    /* 0x40 */ SniperEase y;
} SniperAim;

/* Sniper extension (0x9C bytes, id 0x10000000; InitSniper). */
typedef struct SniperExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ EXTengine *unk10;
    /* 0x14 */ s32 unk14; /* saved camera eye.z */
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ EXTengine *unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x50 - 0x2C];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ s16 unk5A;
    /* 0x5C */ u8 pad5C[0x70 - 0x5C];
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s16 unk78;
    /* 0x7A */ s16 unk7A;
    /* 0x7C */ u8 pad7C[0x84 - 0x7C];
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ s32 unk90;
    /* 0x94 */ EXTengine *unk94;
    /* 0x98 */ s32 unk98;
} SniperExt;

extern u16 CameraMode;

#define SNIPER_CAM (&MainCamera)
#define SNIPER_PAD ((PadState *)Pads)

/* Bear death extension (0x88 bytes, id 0x10000000). */
typedef struct BearDeathExt {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SLONG_XYZ to;
    /* 0x20 */ SLONG_XYZ from;
    /* 0x2C */ s32 unk2C; /* start of the arc passed to ArcUpdate */
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad48[0x28];
    /* 0x70 */ s32 unk70;
    /* 0x74 */ u8 pad74[0xC];
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84;
} BearDeathExt;

extern ScriptInit BearDeathScriptInit;

void PolarMovesSwap(EXTheromove *a, EXTheromove *b);

void PolarAI_SetTarget(s32 player, s32 target);

void SetPolarHeroSquashed(s32 arg0);

/* Per-hero speed table (declared in bash/pause.h, not included here). */
extern s32 PolarHeroMass[];
extern s32 PolarShadowState[];

s32 PolarOfficeBox(s32 i);

s32 PolarAI_RandomPause(s32 player);

void PolarInitHeroMove(s32 arg0);

s32 PolarAI_TeamChoice(s32 hero);


void PolarAI_CountDead(s32 player);

void SetPolarHeroFootRun(s32 player);

extern ScriptInit BearBossScriptInit;

void PolarInitScene(void);



void PolarInitHeroAngle(s32 player);


void PolarInitHeroEngine(s32 arg0);

void PolarAI_PickTarget(s32 player);


void PolarAI_DoBeam(s32 player);


s32 TestAttackChance(s32 player);

s32 TestHuntChance(s32 player);


s32 PolarHeroAnimState(s32 player);

void PolarBeamRelease(s32 arg0, s32 arg1, s32 arg2);

void UkaMaskInitRisers(void);

s32 BearminatorFindPoint(s32 *arg0, s32 arg1);

s32 BearminatorTarget(NAS *m);

void BearminatorEaseCam(PolarEaseCam *c, s32 *pos);

void SniperAimUpdate(SniperAim *a, s32 *out);

void PolarBossFace(NAS *m);

void PolarInitDebug(void);

void InitGravityObject(NAS *m);

void PolarAI_Proximity(s32 player, s32 idx);

void PolarAI_Timer1(s32 player);

void PolarHeroBounceOff(s32 player, EXTheromove *other);

void PolarInitShadow(s32 player);

void PolarHeroIdle(s32 player);

void PolarHeroRun(s32 player);

void PolarHeroTransmute(s32 player);

void PolarHeroSlideFromCentre(s32 player);

void SetPolarHeroInBeam(s32 player);

void SetPolarHeroPoll(s32 player);

void SetPolarHeroAttack(s32 player);

void PolarHeroSnowball(s32 player);

void PolarHeroFootRun(s32 player);

void SetPolarHeroHit(s32 player);

void SetPolarHeroDead(s32 arg0);

void PolarHeroDead(s32 player);

void PolarHeroWinner(s32 player);

void PolarHeroWeight(s32 player);

void SetPolarHeroRun(s32 player);

s32 TestPolarHeroStop(s32 player);

s32 TestPolarHeroRun(s32 player);

/* A hero's 0x20000000 extension as SetPolarHeroOffice uses it. */
typedef struct PolarHitExt {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 speed;
    /* 0x1C */ u8 pad1C[0x24 - 0x1C];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0xA8 - 0x28];
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
} PolarHitExt;

void PolarHeroChallenge(s32 player);

void SetPolarHeroOffice(s32 player);

void PolarHeroOffice(s32 player);

void SetPolarHeroThrowBomb(s32 player);

void SetPolarHeroSnowball(s32 player);

void SetPolarHeroFootIdle(s32 player);

/* InitBearminator's second extension (0x54 bytes, id 0x20000000). */
typedef struct BearminatorExt2 {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x28 - 0x24];
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ u8 pad2C[0x40 - 0x2C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[0x48 - 0x44];
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ u8 pad50[4];
} BearminatorExt2;

/* InitBearminator's extension (0xB8 bytes, id 0x10000000). */
typedef struct BearminatorExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ ScriptCursor *cursor;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 vel[3];
    /* 0x2C */ s32 from[3];
    /* 0x38 */ s32 unk38; /* arc state for ArcUpdate */
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0x4C - 0x40];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x7C - 0x54];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ u8 pad80[0x8C - 0x80];
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ u8 pad90[0x94 - 0x90];
    /* 0x94 */ s32 unk94;
    /* 0x98 */ NAS *unk98;
    /* 0x9C */ EXTengine *unk9C;
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ BearminatorExt2 *unkAC;
    /* 0xB0 */ s32 unkB0;
    /* 0xB4 */ s32 unkB4;
} BearminatorExt;

extern ScriptInit BearminatorScriptInit;



/* HeroHooks (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct PolarHooks {
    /* 0x00 */ s32 InitHero;
    /* 0x04 */ void *HeroModels;
    /* 0x08 */ void *unk8;
    /* 0x0C */ u16 *RunSpeeds;
    /* 0x10 */ void *JumpEnts;
    /* 0x14 */ void *VictorySeqs;
    /* 0x18 */ void *CreateShadow;
    /* 0x1C */ void *UpdateShadow;
    /* 0x20 */ void *PreMoveHero;
    /* 0x24 */ void *MoveHero;
    /* 0x28 */ void *PostMoveHero;
    /* 0x2C */ void (*HeroFacing)(void);
    /* 0x30 */ void *HeroSetIdle;
    /* 0x34 */ void *HeroTouched;
    /* 0x38 */ void *HeroBot;
    /* 0x3C */ void *TestAttack;
    /* 0x40 */ void *TestRun;
    /* 0x44 */ void *TestJump;
    /* 0x48 */ void *TestKick;
    /* 0x4C */ void *Hero2ObjectCollision;
    /* 0x50 */ void *HeroGrabObject;
    /* 0x54 */ void *HeroKickCollision;
    /* 0x58 */ void *HeroCarryCollision;
    /* 0x5C */ void *HeroExtraEffects;
    /* 0x60 */ void *HeroesTouched;
    /* 0x64 */ void *ChooseWinner;
    /* 0x68 */ void *KeepInBounds;
} PolarHooks;
extern PolarHooks HeroHooks;
extern void *AiBotTweakTable;
extern void *AiHumanTweakTable;
extern void *AiSkillTable;
extern void *AiTeamSkillTable;
extern void *AiHumanRoundTweakTable;
extern s32 PolarFirstFlag;
extern s32 PolarArenaRadius2;
void ModelCreateShadow(s32 idx);
void ModelUpdateShadow(s32 idx);
extern s32 PolarVictorySeqs[]; /* {name, 0x5C} x4: the victory sequences (the engine's hook 0x14) */
s32 PolarIceHit(SLONG_XYZ *pos, s32 player);

void PolarHeroesCollide(s32 p1, s32 p2);

/* PolarUpdateShadow: shadow frame tables, indexed by the hero's Aindex - 1. */
extern u16 PolarShadowFrames0[][2], PolarShadowFrames1[][2], PolarShadowFrames2[][2], PolarShadowFrames3[][2];
extern u16 PolarShadowFrames4[][2], PolarShadowFrames5[][2], PolarShadowFrames6[][2], PolarShadowFrames7[][2];
extern u16 PolarShadowFrames8[][2], PolarShadowFrames9[][2], PolarShadowFrames10[][2];
/* the polar.h sequences the code names directly are declared there */
void PolarUpdateShadow(s32 player);
void PolarPreMoveHero(s32 player);

s32 TestPolarHeroMelted(s32 player);

void SetPolarHeroIdle(s32 player);

void PolarHeroDazed(s32 player);

void PolarHeroAttackThrow(s32 player);

void SetPolarHeroWinner(s32 player);

/* Extension of the 0x1605 objects UpdatePOLAR drops (id 0x10000000). */
typedef struct PolarDropExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 kind;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 pad1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 pad24;
    /* 0x28 */ s32 unk28;
} PolarDropExt;

extern s32 PolarUpdateWord;
extern s32 ScriptFxOn;
void PolarUpdateScene(void);

/* One drop of UpdatePOLAR's shower (like tank's TankSpawnBit1605). */
static inline void PolarSpawnDrop(VECTOR *pos, s32 kind) {
    NAS *nas;
    PolarDropExt *ext;

    nas = CreateNAS(pos->vx, pos->vy, pos->vz, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->kind = kind;
            ext->unk18 = 0x100;
            ext->unk20 = 1;
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
    }
}

void SetPolarHeroDazed(s32 player);

void SetPolarHeroChallenge(s32 player);

/* The hero's FUX extension (0x60 bytes) InitialisePOLAR sets up. */
typedef struct PolarFux {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x30 - 0x28];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ u8 pad3C[0x44 - 0x3C];
    /* 0x44 */ void *unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
} PolarFux;

/* The hero's 0x20000000 extension (0xB8 bytes). */
typedef struct PolarAi {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x98 - 0x2C];
    /* 0x98 */ s32 unk98;
    /* 0x9C */ u8 pad9C[0xA0 - 0x9C];
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ EXTengine *unkB0;
    /* 0xB4 */ s32 unkB4;
} PolarAi;

/* PolarBeamOwner's 0x40000000 extension as PolarHeroCollisions reads it. */
typedef struct PolarGame {
    /* 0x00 */ u8 pad0[0x48];
    /* 0x48 */ s32 unk48[8];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
} PolarGame;

extern u16 PolarHeroSfx[];
extern s32 PolarInitWord;
void SetPanelHeroIconB(u16 arg0, s16 arg1);
void Init_Clock(s32 arg0, s32 arg1, s32 arg2);
void Set_Clock(u16 time, u16 flags);
void InitBossHealthIcons(void);

void PolarAI_DoIceWalk(s32 player);

s32 PolarHeroCollisions(s32 player);

/* Polar's AI tweak tables and hero models (halfwords) */
extern u16 PolarDefaultAiSkill[];
extern u16 PolarDefaultAiSkillAlt[];
extern u16 PolarDefaultAiTeamSkill[];
extern u16 PolarDefaultAiHumanRoundTweak[];
extern u16 PolarDefaultAiBotTweak[];
extern u16 PolarDefaultAiHumanTweak[];
extern u16 PolarDayAiSkill[];
extern u16 PolarDayAiSkillAlt[];
extern u16 PolarDayAiTeamSkill[];
extern u16 PolarDayAiHumanRoundTweak[];
extern u16 PolarDayAiBotTweak[];
extern u16 PolarDayAiHumanTweak[];
extern u16 PolarCutAiSkill[];
extern u16 PolarCutAiSkillAlt[];
extern u16 PolarCutAiTeamSkill[];
extern u16 PolarCutAiHumanRoundTweak[];
extern u16 PolarCutAiBotTweak[];
extern u16 PolarCutAiHumanTweak[];
extern u16 PolarEndAiSkill[];
extern u16 PolarEndAiSkillAlt[];
extern u16 PolarEndAiTeamSkill[];
extern u16 PolarEndAiHumanRoundTweak[];
extern u16 PolarEndAiBotTweak[];
extern u16 PolarEndAiHumanTweak[];
extern u16 PolarBossAiSkill[];
extern u16 PolarBossAiBotTweak[];
extern u16 PolarBossAiHumanTweak[];
extern u16 PolarBossAiHumanRoundTweak[];
extern u16 PolarBossAiSkillAlt[];
extern u16 PolarHeroModels[];
extern u16 PolarHeroModelsB[];
extern u16 PolarHeroModelsCut[];


#endif /* BASH_POLAR_OVL_H */
