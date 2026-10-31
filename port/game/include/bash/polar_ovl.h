#ifndef BASH_POLAR_OVL_H
#define BASH_POLAR_OVL_H

/* Functions and data defined in the polar overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/polar/polar.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/polar.h"   /* psector, EXTsmoketrail */

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
s32 PolarCompareXZ(SLONG_XYZ *arg0, SLONG_XYZ *arg1);
u32 PolarAngleOf(SLONG_XYZ *arg0);
void SetPolarHeroTransmute(s32 player);
s32 TestPolarHeroWon(s32 arg0);
void SetPolarHeroWeight(s32 player);
void PolarInitHero(void);
void PolarEmptyStub0(void);
void PolarEmptyStub1(void);
void PolarEmptyStub2(void);

extern NAS *PolarBomb;

/* InitBombGFX's extension (0x1C bytes). */
typedef struct BombGFXExt {
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
} BombGFXExt;


/* Visible scene objects PolarInitDebug collects. */
extern subengine *PolarIceObjects[80];
extern const char PolarMsgFoundColSubs[];

/* Scene objects PolarInitScene picks out by index. */
extern s32 PolarTiltX;
extern s32 PolarTiltZ;
extern subengine *PolarSceneA;
extern subengine *PolarSceneB;
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
    /* 0x00 */ u8 pad0[offsetof(EXTheromove, x_vel)];
    /* 0x20 */ PolarEase x;
    /* 0x40 */ PolarEase z;
} PolarEaseCam;

typedef struct Ext10000000 {
    /* 0x00 */ EXT base;
    /* 0x0C */ void *flag;  /* tested for zero */
} Ext10000000;

/* The 0x10000000 extension of the object PolarBombHitHeroes spawns (0x1611). */
typedef struct PolarBombHit {
    /* 0x00 */ EXT base; u8 padC[4];
    /* 0x10 */ s32 angle;
    /* 0x14 */ SLONG_XYZ pos;
    /* 0x20 */ SLONG_XYZ start;
} PolarBombHit;

/* The 0x10000000 extension of the object SetPolarHeroThrowBomb spawns (0x160D). */
typedef struct PolarThrown {
    /* 0x00 */ EXT base; u8 padC[8];
    /* 0x14 */ s32 owner;
    /* 0x18 */ SLONG_XYZ pos;
    /* 0x24 */ SLONG_XYZ start;
    /* 0x30 */ u8 pad30[0x74 - 0x30];
    /* 0x74 */ s32 unk74;
} PolarThrown;

/* Extension 0x10000000 of the object PolarSpawnObjects spawns (NAS 0x1605). */
typedef struct ExtSpawn5910 {
    /* 0x00 */ u8 pad[sizeof(EXT)];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 unk28;
} ExtSpawn5910;


extern s32 PolarRoundState;

/* PolarBeamOwner's 0x40000000 extension as SetPolarHeroWinner uses it: engines at
 * 0x74 and four at 0x78. */
typedef struct PolarGameExt {
    /* 0x00 */ u8 pad[offsetof(EXTtransmuter, it_eng)];
    /* 0x74 */ EXTengine *eng;
    /* 0x78 */ EXTengine *eng2[4];
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
    /* 0x00 */ EXT base; u8 padC[8];
    /* 0x14 */ s32 count;  /* counted up */
    /* 0x18 */ s32 flag;  /* tested for zero */
    /* 0x1C */ u8 pad1C[0x4];
    /* 0x20 */ s32 val;
    /* 0x24 */ s32 state;
    /* 0x28 */ u8 pad28[0xA4 - 0x28];
    /* 0xA4 */ NAS *nas;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 flag2;  /* tested for zero */
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


/* Extension 0x10000000 made by InitSimpleExplosion: a velocity per sub-engine. */
typedef struct ExtSimpleExplosion {
    /* 0x000 */ EXT base; u8 padC[8];
    /* 0x014 */ SLONG_XYZ vel[20];
    /* 0x104 */ s32 count;  /* counted up */
} ExtSimpleExplosion;


/* The object PlotBrioPack follows (its NAS.UserInfo.user1). */
typedef struct BrioAnim {
    /* 0x00 */ u8 pad0[offsetof(EXTanimator, Aindex)];
    /* 0x1E */ s16 frame;
} BrioAnim;

typedef struct BrioOwner {
    /* 0x00 */ u8 pad0[offsetof(HeroInfo, hero_eng)];
    /* 0x04 */ EXTengine *engine;
    /* 0x08 */ u8 pad8[offsetof(HeroInfo, hero_anim) - (offsetof(HeroInfo, hero_eng) + sizeof(void *))];
    /* 0x1C */ BrioAnim *anim;
    /* 0x20 */ u8 pad20[offsetof(HeroInfo, hero_mode) - (offsetof(HeroInfo, hero_anim) + sizeof(void *))];
    /* 0x2C */ u16 type;
    /* 0x2E */ u16 subtype;
} BrioOwner;

/* Head of a list linked through +0x20 (counted by CountBears). */
extern u8 *PolarBears;

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

void GravityMotionUpdate(NAS *nas, EXTmotion *m);


typedef struct GravityObject {
    /* 0x00 */ u8 pad0[offsetof(NAS, FUX)];
    /* 0x58 */ EXTmotion *target;
} GravityObject;

void PlotGravityObject(GravityObject *obj);
/* A hero's FUX extension as TestWanderChance sets it up. */
typedef struct PolarWanderFux {
    /* 0x00 */ u8 pad0[sizeof(EXT)];
    /* 0x0C */ SLONG_XYZ vel;
    /* 0x18 */ SLONG_XYZ pos;
    /* 0x24 */ s32 timer2;  /* counted down */
    /* 0x28 */ s32 target;
    /* 0x2C */ s32 player;  /* a hero's index */
    /* 0x30 */ s32 state;
    /* 0x34 */ s32 flags2;
    /* 0x38 */ u8 pad38[4];
    /* 0x3C */ s32 aiDelay;  /* copied to the AI's AItimer1 */
    /* 0x40 */ s32 angle;
    /* 0x44 */ void *func;
    /* 0x48 */ u8 pad48[sizeof(EXT) + 0x40 - (sizeof(EXT) + 0x38 + sizeof(void *))];
    /* 0x4C */ s32 flags;
    /* 0x50 */ s32 flag;  /* tested for zero */
    /* 0x54 */ s32 count;  /* counted up */
    /* 0x58 */ s32 flag2;  /* tested for zero */
    /* 0x5C */ s32 timer;
} PolarWanderFux;

extern s32 PolarAIChances[];
extern s32 PolarAnimFlag;
void PolarAI_DoWander(s32 player);
void PolarAI_AvoidMelt(s32 player);
void DoGetBomb(u32 player);
void PolarAI_DoHunt(s32 player);

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
    f->aiDelay = 1;
    f->timer2 = Random(0x3C) + 5;
}
s32 TestWanderChance(s32 player);

/* Inlined twice into EnsureMateNotObstructingMate. */
static inline void PolarGrab(s32 player) {
    PolarWanderFux *f;

    f = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (f == NULL) {
        return;
    }
    if (f->state != 6 || f->timer2 == 0) {
        f->timer2 = 0x3C;
    }
    f->state = 6;
    f->func = PolarAI_AvoidMelt;
    f->player = 0;
    f->aiDelay = 1;
    f->flags2 |= 1;
}

s32 PolarGetLevelPoint(s32 *arg0, s32 arg1);


void PolarHeroThrowBomb(s32 player);

void PolarHeroAddWeight(s32 arg0);


extern s32 PolarClock;
extern VECTOR PolarCentre;
/* DoesPlayerHaveBomb, inlined into PolarAI_DoBeam. */
static inline s32 PolarHasBomb(s32 player) {
    NAS *nas;
    EXTbombmaker *ext;
    s32 one;
    s32 found;

    nas = PolarBomb;
    one = 1;
    found = 1;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        do {
            if (ext != NULL && ext->NasStatus == one && ext->owner_id == player) {
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
                pos = h->hero_eng->XYZ;
                d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->XYZ);
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
    EXTbombmaker *ext;
    s32 one;

    nas = PolarBomb;
    one = 1;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->NasStatus == one && ext->owner_id == player) {
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
    EXTbombmaker *ext;

    nas = PolarBomb;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->NasStatus == 0) {
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
        ext->count = 8;
    }
    if (HeroesAllOut != 0) {
        ext->count = 0;
    }
}

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


/* Read by PolarBeamLinkState and CheckMelted. */
typedef struct PolarLink {
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 state;
    /* 0x10 */ s32 player;  /* its index in PolarAIFlags, -1 for none */
    /* 0x14 */ s32 beamMode;  /* copied to PolarBeamMode */
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 modes[(0x6C - 0x28) / 4];  /* per player */
    /* 0x6C */ s32 aiFlag;  /* its index in PolarAIFlags */
    /* 0x70 */ u8 pad70[offsetof(EXTtransmuter, sectordata) - offsetof(EXTtransmuter, itplayer)];
    /* 0xBC */ psector sectors[16];
} PolarLink;

typedef struct PolarOwner {
    /* 0x00 */ u8 pad0[offsetof(NAS, Engine)];
    /* 0x54 */ EXTengine *eng;
    /* 0x58 */ PolarLink *link;
} PolarOwner;

extern PolarOwner *PolarBeamOwner;
extern s32 PolarBeamMode;

/* Read by PolarAI_Proximity. */
typedef struct PolarMode {
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 mode;
} PolarMode;

typedef struct PolarProx {
    /* 0x00 */ u8 pad0[offsetof(NAS, Engine)];
    /* 0x54 */ EXTengine *eng;
    /* 0x58 */ PolarMode *flag;  /* tested for zero */
} PolarProx;

extern PolarProx *PolarProximity;

/* Action choosers DoChooseNewAction tries in turn from a random start. */
extern s32 (*PolarAITests[5])(s32 player);

/* DoChooseNewAction's "attack target t" step: chase t, and send a team mate
   after one of the other team. */
#define POLAR_ATTACK(fux, h, player, t, f, r)                    \
    {                                                            \
        if ((fux)->state != 1 || (fux)->timer2 == 0) {            \
            (fux)->timer2 = 0xD2;                                 \
        }                                                        \
        (fux)->state = 1;                                        \
        (fux)->aiDelay = 1;                                        \
        (fux)->func = PolarAI_DoAttack;                             \
        (fux)->target = (t);                                     \
        (fux)->flags2 &= ~2;                                      \
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
void PolarAI_AvoidBomber(s32 player);
void PolarAI_DoChaseBomber(s32 player);

s32 PolarAI_AnyBusy(void);

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
    /* 0x4 */ s32 pad4;
    /* 0x8 */ s32 height;
    /* 0xC */ s16 base;
    /* 0xE */ s16 padE;
} PolarRiser;

typedef struct PolarRiserSet {
    /* 0x00 */ u8 pad0[offsetof(EXTtransmuter, sectordata)];
    /* 0xB8 */ PolarRiser slot[16];
} PolarRiserSet;


typedef struct UkaMaskSet {
    /* 0x00 */ u8 pad0[offsetof(EXTtransmuter, sectordata)];
    /* 0xB8 */ psector seg[16];
} UkaMaskSet;

void UkaMaskSetSegments(s32 i, UkaMaskSet *m);


/* The 0x1605 bit's extension (id 0x10000000) InitUkaPolarMask sets up. */
typedef struct UkaBitExt {
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 flags;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 unk28;
} UkaBitExt;

/* InitTransmuterBeam's extension (id 0x40000000, 0x1E0 bytes) is the leak's EXTtransmuter. */


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
    /* 0x00 */ u8 pad0[offsetof(subengine, entity)];
    /* 0x74 */ s16 y;
} PolarRiserObj;

extern PolarRiserObj *UkaMaskRisers[16];

/* Scene objects with index 0x4000, UkaMaskInitRisers collects. */
extern subengine *UkaMaskObjects[16];

void RiserSetReset(PolarRiserSet *arg0);

/* Walrus fish extension (id 0x10000000): a ballistic arc (ArcUpdate's
 * Arc, declared in bash/pause.h) at 0x10. */
typedef struct WalrusFishExt {
    /* 0x00 */ EXT base;
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
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 state;
    /* 0x10 */ s32 timer;
    /* 0x14 */ s32 arc[0xD];
    /* 0x48 */ s32 xvel;  /* the hero move's x_vel */
    /* 0x4C */ s32 zvel;  /* the hero move's z_vel */
    /* 0x50 */ u8 pad50[0x58 - 0x50];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[0x68 - 0x5C];
    /* 0x68 */ SLONG_XYZ to;
    /* 0x74 */ SLONG_XYZ from;
    /* 0x80 */ ScriptCursor *cursor;
    /* 0x84 */ NAS *nas;
    /* 0x88 */ s32 speed;
    /* 0x8C */ s32 maxSpeed;
    /* 0x90 */ s32 angle;
} PolarWalrusExt;

extern s32 PolarBeamTimer;
extern s32 WalrusFrames[];
extern s32 WalrusSteps0[];
extern s32 WalrusSteps1[];
extern s32 WalrusSteps2[];
extern s32 WalrusSteps3[];


extern ScriptInit WalrusScriptInit;
extern ScriptInit WalrusFishScriptInit;
extern s32 WalrusState;


/* Big missile extension (id 0x10000000) as PlotBigMissile reads it: a ballistic
 * arc at 0xC (the same block as BigMissileExt, InitBigMissile's view). */
typedef struct BigMissileArcExt {
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 arc[0x15];
    /* 0x60 */ SLONG_XYZ from;
    /* 0x6C */ SLONG_XYZ to;
    /* 0x78 */ s32 state;
} BigMissileArcExt;

/* Polar boss extension (0x94 bytes, id 0x10000000; InitPolarBoss, PlotBigMissile). */
typedef struct PolarBossExt {
    /* 0x00 */ EXT base;
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
    /* 0x68 */ s32 timer;
    /* 0x6C */ s32 ang;
    /* 0x70 */ s32 flag;  /* tested for zero */
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 phase;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ s32 flag2;  /* tested for zero */
    /* 0x84 */ NAS *animNas;  /* the boss's animated body (its ext 0x10000000) */
    /* 0x88 */ NAS *nas;
    /* 0x8C */ s32 spin;  /* added to ang */
    /* 0x90 */ s32 count;  /* counted up */
} PolarBossExt;

extern NAS *BearminatorNas;
extern ScriptInit PolarBossScriptInit;

extern NAS *polarbossnas;


/* The 0x10000000 extension of the boss's partner (PlotPolarBoss). */
typedef struct PolarBossAnimExt {
    /* 0x00 */ EXT base;
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

    if (((GameState *)GameInfo)->adventure.modeSkill[22] < 10) {
        return 0;
    }
    if (((GameState *)GameInfo)->adventure.modeSkill[22] >= 30) {
        return 0xE66;
    }
    k = 0xB8;
    return (((GameState *)GameInfo)->adventure.modeSkill[22] - 9) * k;
}

s32 PolarAI_PickVictim(s32 arg0);


/* Sniper missile extension (0x38 bytes, id 0x10000000). */
typedef struct SniperMissileExt {
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 state;  /* switched on */
    /* 0x10 */ SLONG_XYZ from;
    /* 0x1C */ SLONG_XYZ to;
    /* 0x28 */ s32 pitch;
    /* 0x2C */ s32 yaw;
    /* 0x30 */ s32 timer;  /* counted down */
    /* 0x34 */ s32 count;  /* counted up */
} SniperMissileExt;

/* PolarXZBoxToSphere's collision box (SubFindFlagPoint's FlagPoint data): two corners. */
typedef struct PolarBoxHalf {
    s16 v[3];
} PolarBoxHalf;

typedef struct PolarCollBox {
    PolarBoxHalf lo;
    PolarBoxHalf hi;
} PolarCollBox;

s32 PolarXZBoxToSphere(EXTengine *a, EXTengine *b, s32 c);

/* Polar missile extension (0x70 bytes, id 0x10000000). */
typedef struct PolarMissileExt {
    /* 0x00 */ EXT base; u8 padC[4];
    /* 0x10 */ s32 state;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x2C - 0x1C];
    /* 0x2C */ s32 speedMax;  /* copied to unk50 */
    /* 0x30 */ u8 pad30[0x38 - 0x30];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ u8 pad3C[0x44 - 0x3C];
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 flag;  /* tested for zero */
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 bounces;
    /* 0x5C */ s32 speed;  /* how far pos moves each frame, along the engine's yaw */
    /* 0x60 */ s32 timer;  /* counted down */
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
            && DistXZSq((SLONG_XYZ *)pos, (SLONG_XYZ *)&h->hero_eng->XYZ) < 0x7BC4) {
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

    pos = &m->Engine->XYZ;
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


s32 PolarBombHitHeroes(NAS *nas, s32 dist);


/* Big missile extension (0x7C bytes, id 0x10000000). */
typedef struct BigMissileExt {
    /* 0x00 */ EXT base;
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
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 state;
    /* 0x10 */ EXTengine *aimEng;
    /* 0x14 */ s32 savedEyeZ;  /* the camera's eye.z, put back at the end */
    /* 0x18 */ s32 timer;
    /* 0x1C */ s32 timer2;
    /* 0x20 */ EXTengine *aimEng2;
    /* 0x24 */ s32 aimX;  /* with aimY, SniperAimUpdate's aim */
    /* 0x28 */ s32 aimY;
    /* 0x2C */ u8 pad2C[0x50 - 0x2C];
    /* 0x50 */ s32 xmove;  /* its sideways aim speed */
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ s16 unk5A;
    /* 0x5C */ u8 pad5C[0x70 - 0x5C];
    /* 0x70 */ s32 ymove;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s16 unk78;
    /* 0x7A */ s16 unk7A;
    /* 0x7C */ u8 pad7C[0x84 - 0x7C];
    /* 0x84 */ s32 fired;  /* the shot is away */
    /* 0x88 */ s32 player;  /* the hero aiming */
    /* 0x8C */ s32 gravity;  /* added to yvel */
    /* 0x90 */ s32 yvel;  /* the dropped body's fall; halved and turned on a bounce */
    /* 0x94 */ EXTengine *shadow;  /* mirrors the body under the floor */
    /* 0x98 */ s32 bounces;
} SniperExt;


#define SNIPER_CAM (&MainCamera)
#define SNIPER_PAD ((PadState *)Pads)

/* Bear death extension (0x88 bytes, id 0x10000000). */
typedef struct BearDeathExt {
    /* 0x00 */ EXT base; u8 padC[8];
    /* 0x14 */ SLONG_XYZ to;
    /* 0x20 */ SLONG_XYZ from;
    /* 0x2C */ s32 arc;  /* ArcUpdate's state */
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ s32 yvel;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad48[0x28];
    /* 0x70 */ s32 unk70;
    /* 0x74 */ u8 pad74[0xC];
    /* 0x80 */ s32 count;  /* counted up */
    /* 0x84 */ s32 timer;  /* counted down */
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
    /* 0x00 */ EXT base; u8 padC[12];
    /* 0x18 */ s32 speed;
    /* 0x1C */ u8 pad1C[0x24 - 0x1C];
    /* 0x24 */ s32 flag;  /* tested for zero */
    /* 0x28 */ u8 pad28[0xA8 - 0x28];
    /* 0xA8 */ s32 flag2;  /* tested for zero */
    /* 0xAC */ s32 unkAC;
} PolarHitExt;

void PolarHeroChallenge(s32 player);

void SetPolarHeroOffice(s32 player);

void PolarHeroOffice(s32 player);

void SetPolarHeroThrowBomb(s32 player);

void SetPolarHeroSnowball(s32 player);

void SetPolarHeroFootIdle(s32 player);


/* InitBearminator's extension (0xB8 bytes, id 0x10000000). */
typedef struct BearminatorExt {
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 state;
    /* 0x10 */ ScriptCursor *cursor;
    /* 0x14 */ s32 timer;
    /* 0x18 */ s32 targetAngle;  /* turned towards, from the target hero */
    /* 0x1C */ s32 timer2;
    /* 0x20 */ s32 vel[3];
    /* 0x2C */ s32 from[3];
    /* 0x38 */ s32 arc;  /* ArcUpdate's state */
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0x4C - 0x40];
    /* 0x4C */ s32 yvel;  /* its bounce, damped by 0xAA/0x100 */
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x7C - 0x54];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ u8 pad80[0x8C - 0x80];
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ u8 pad90[0x94 - 0x90];
    /* 0x94 */ s32 angle;  /* the direction it moves in */
    /* 0x98 */ NAS *follower;  /* a NAS kept at its position */
    /* 0x9C */ EXTengine *fxEng;  /* an engine kept at its position */
    /* 0xA0 */ s32 target;  /* the hero it goes for */
    /* 0xA4 */ s32 timer3;  /* counted down */
    /* 0xA8 */ s32 count;  /* counted up */
    /* 0xAC */ EXTheromove *easeCam;
    /* 0xB0 */ s32 timer22;  /* counted down */
    /* 0xB4 */ s32 flag;  /* tested for zero */
} BearminatorExt;

extern ScriptInit BearminatorScriptInit;



extern s32 PolarFirstFlag;
extern s32 PolarArenaRadius2;
extern VictorySeq PolarVictorySeqs[]; /* {name, 0x5C} x4: the victory sequences (the engine's hook 0x14) */
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
    /* 0x00 */ EXT base;
    /* 0x0C */ s32 kind;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 pad1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 pad24;
    /* 0x28 */ s32 unk28;
} PolarDropExt;

extern s32 PolarUpdateWord;
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


/* The hero's 0x20000000 extension (0xB8 bytes). */
typedef struct PolarAi {
    /* 0x00 */ u8 pad0[sizeof(EXT)];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 flag;  /* tested for zero */
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x6C];
    /* 0x98 */ s32 unk98;
    /* 0x9C */ s32 deadTimer;     /* counts up in the dead state */
    /* 0xA0 */ NAS *unkA0;
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ EXTengine *eng;
    /* 0xB4 */ s32 flag2;  /* tested for zero */
} PolarAi;

/* PolarBeamOwner's 0x40000000 extension as PolarHeroCollisions reads it. */
typedef struct PolarGame {
    /* 0x00 */ u8 pad0[sizeof(EXT)];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[0x30];
    /* 0x48 */ s32 out[8];  /* per player, zero while in play */
    /* 0x68 */ s32 itTime;  /* the it player's time left */
    /* 0x6C */ s32 itPlayer;
} PolarGame;

extern u16 PolarHeroSfx[];
extern s32 PolarInitWord;

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
