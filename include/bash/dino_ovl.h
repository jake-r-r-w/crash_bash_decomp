#ifndef BASH_DINO_OVL_H
#define BASH_DINO_OVL_H

/* Functions and data defined in the dino overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/dino/dino.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"

s32 DinoAI_NearestGem(s32 arg0);
s32 DinoAI_PickGem(s32 arg0);
s32 DinoAI_PickVictim(s32 arg0);
s32 DinoTestTeamMate(s32 arg0, s32 arg1);

s32 DinoGetLevelPoint(s32 *arg0, s32 arg1);

typedef struct DinoPhys {
    u8 pad0[0x20];
    s32 unk20;
    u8 pad24[0x1C];
    s32 unk40;
    u8 pad44[0xA];
    s16 unk4E;
} DinoPhys;
/* Extension 0x10000000 (FindExtension): a gem's or hero's physics record sits at +0x94. */
typedef struct DinoExt {
    u8 pad[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x94 - 0x18];
    /* 0x94 */ DinoPhys phys;
} DinoExt;

typedef struct DinoChase {
    u8 pad[0xC];
    void (*fn)(); /* 0xC */
    SLONG_XYZ pos;    /* 0x10 */
    SLONG_XYZ tgt;    /* 0x1C */
    s32 unk28;
    u8 pad2[4];
    s32 unk30;
    u8 pad2b[4];
    s32 state; /* 0x38 */
    u8 pad3[4];
    s32 timer; /* 0x40 */
} DinoChase;

typedef struct DinoGem {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x10];
    s32 unk28;
    s32 unk2C;
    u8 pad30[0x18];
    s32 unk48;
    s32 unk4C;
    u8 pad50[0x18];
    SLONG_XYZ unk68;
    u8 pad74[0xC];
    s32 unk80;
    EXTengine *unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    u8 pad94[0x20];
    s32 unkB4;
    u8 padB8[0x1C];
    s32 unkD4;
    u8 padD8[0xA];
    s16 unkE2;
} DinoGem;

typedef struct DinoPos {
    u8 pad[4];
    SLONG_XYZ pos; /* 0x4 */
} DinoPos;

typedef struct DinoPosH {
    u8 pad[4];
    u16 x;
    u8 pad2[6];
    u16 z;
} DinoPosH;

typedef struct DinoEngH {
    u8 pad[0x10];
    u16 x;
    u8 pad2[6];
    u16 z;
} DinoEngH;

s32 DinoAI_ShotBlocked(s32 id);
void DinoAI_SteerToTarget(s32 id);

/* Hero NAS extension 0x10000000 DinoHeroRun ticks: a 26-frame timer. */
typedef struct DinoHeroExt {
    u8 pad[0xC];
    s32 unkC;  /* 0x0C */
    u8 pad10[0xC];
    s32 timer; /* 0x1C */
} DinoHeroExt;

s32 DinoHeroTestShoot(s32 hero);
s32 DinoHeroCollide(s32 hero);
s32 DinoHeroTestIdle(s32 hero);
typedef struct DinoExtB {
    u8 pad[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    NAS *unk18;
} DinoExtB;


extern NAS *DinoGems[];

/* DinoPreMoveHero: move speeds (normal, carrying, by AiPlayerSkill[id].level / 2). */
extern s32 DinoSteerMin;
extern s32 DinoSteerMax;
extern s32 DinoSteerTable[];

void PlotDinoSmoke(NAS *m);

void SetDinoHeroShoot(s32 player);

void SetDinoHeroRun(s32 player);

s32 DinoKeepInBounds(EXTengine *arg0);

void FirstTriggersDINO(void);

/* A scene object in the list at MapScene->subs (DinoFindTargets). */
typedef struct DinoObj {
    u32 flags;             /* 0x00 */
    u8 pad4[0x58];
    struct DinoObj *next; /* 0x5C */
    u8 pad60[0x14];
    u16 index;             /* 0x74 */
    s16 scale;             /* 0x76 */
    s32 r;                 /* 0x78 */
    s32 g;                 /* 0x7C */
    s32 b;                 /* 0x80 */
} DinoObj;



/* DinoChooseWinnerTeam: the 0x10000000 extension of the spawned 0x1605 effect. */
typedef struct DinoPuff {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10[0x18 - 0x10];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x20 - 0x1C];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 unk24[0x28 - 0x24];
    /* 0x28 */ s32 unk28;
} DinoPuff;

/* DinoChooseWinnerTeam: a puff of smoke at hero h. */
#define DINO_PUFF(h)                                                       \
    {                                                                       \
        NAS *nas_;                                                          \
        DinoPuff *p_;                                                      \
        SLONG_XYZ *v_;                                                      \
                                                                            \
        v_ = &(h)->hero_eng->unk10;                                         \
        nas_ = CreateNAS(v_->x, v_->y, v_->z, 0x1605, 0x11);             \
        if (nas_ != NULL) {                                                 \
            p_ = FindExtension(nas_, 0x10000000);                           \
            if (p_ != NULL) {                                               \
                p_->unk18 = 0x100;                                          \
                p_->unkC = 0;                                               \
                p_->unk20 = 1;                                              \
                p_->unk28 = 0;                                              \
                nas_->InitDelete(nas_);                                     \
            }                                                               \
        }                                                                   \
    }

extern DinoObj *DinoGoal;
extern DinoObj *DinoTargetA;
extern DinoObj *DinoTargetB;
extern DinoObj *DinoTargetC;
extern DinoObj *DinoTargetObjs[3];
extern DinoObj *DinoTargetObjsB[3];

void DinoAI_Think();
void InitialiseDINOFirst();
void InitialiseDINO();
void UpdateDINO();

void DinoRestartLevel(void);

void DinoHeroWinner(s32 arg0);

void Dino_HeroBot(void);

s32 DinoAI_SetNearestGem(s32 *arg0, s32 arg1);

void DinoUnusedStub(void);

s32 DinoGemLoose(s32 arg0);

s32 DinoPadAngle(s32 arg0);

s32 DinoCompareXZ(SLONG_XYZ *arg0, SLONG_XYZ *arg1);

extern s32 DinoGem0;
extern NAS *DinoGem1;
extern NAS *DinoGem2;
extern s32 DinoGemTimer;
extern s32 DinoScoreFlag;
extern s32 DinoScoreDelay;

void DinoUpdateTargets(void);
void DinoScoreGems(void);

s32 DinoAI_RayToPlayer(s32 a, s32 b);

void DinoHeroRun(s32 id);

void DinoInitGems(void);

s32 DinoHeroTestStop(s32 arg0);

void DinoHeroShoot(s32 arg0);

void DinoHeroIdle(s32 arg0);

void DinoInitHero(s32 arg0);

void DinoChooseWinner(s32 arg0, s32 arg1);

void DINOSetUp(void);

void SetDinoHeroWinner(s32 arg0);

s32 TestDinoHeroWon(s32 id);

s32 DinoHeroHasGem(s32 arg0);

s32 DinoAI_TargetShare(s32 arg0, s32 arg1);
extern ScriptInit DinoSmokeScriptInit;


void DinoAI_GuardGem(s32 id);

void DinoAI_ShootGem(s32 id);

s32 DinoRayHitB(s32 *a, s32 *b, s16 *c, s32 *d);

void DinoAI_ReturnGem(s32 id);
void DinoAI_ChaseGem(s32 id);
void DinoAI_CarryGem(s32 id);
void DinoAI_Wander(s32 id);

void DinoFindTargets(void);

s32 DinoKeepInArena(s32 arg0);

void DinoMovesSwap(s32 arg0, s32 arg1);

void DinoHeroFacing(s32 arg0);

void DinoHeroDazed(s32 id);

void SetDinoHeroSlide(s32 id);

void DinoHeroSlide(s32 id);

s32 DinoGemCarrier(NAS *m);

void InitDinoGem(NAS *m);

void PlotDinoScratch(NAS *m);

void InitDinoScratch(NAS *m);

s32 DinoClampToArena(s32 *p, s32 arg1);

s32 DinoHeroTestRun(s32 arg0);

void SetDinoHeroIdle(s32 arg0);

void DinoMovesSwapB(EXTheromove *a, EXTheromove *b);

s32 DinoFloorKind(s32 arg0);

void InitDinoSmoke(NAS *arg0);

s32 DinoFindClosestPlayer(s32 self, s32 *dist);

s32 DinoGemFree(s32 arg0);

extern s32 DinoWanderAngles[];
extern u16 DinoWanderTime;

typedef struct DinoBoxHalf {
    s16 v[3];
} DinoBoxHalf;

typedef struct DinoBox2 {
    DinoBoxHalf lo;
    DinoBoxHalf hi;
} DinoBox2;

s32 csqrt(s32 x);

/* XZBoxToSphere's push: sets up the object's rotation and passes v through. */
static inline s32 DinoBoxPushRot(SVECTOR *ang, u16 *a, s32 v) {
    ang->vx = a[8];
    ang->vy = a[9];
    ang->vz = a[10];
    return v;
}

/* Hero NAS extension 0x20000000: the AI chase state DinoAI_Wander runs. */
typedef struct DinoAiExt {
    u8 pad[0xC];
    void (*func)(); /* 0x0C */
    SLONG_XYZ pos;    /* 0x10 */
    SLONG_XYZ target; /* 0x1C */
    s32 unk28;        /* 0x28 */
    s32 victim;       /* 0x2C: nearest other hero */
    s32 unk30;
    s32 unk34;
    s32 unk38;        /* 0x38 */
    s32 angle;        /* 0x3C */
    s32 timer;        /* 0x40 */
} DinoAiExt;

void DinoChooseWinnerOther(s32 arg0, u16 arg1);

void DinoGemPhysics(NAS *arg0);

/* plays one of four sfx at random (r: the caller's s32) */
#define SFX4(a, b, c, d)                                    \
    r = Random(4);                                          \
    if (r == 0) {                                           \
        PlaySfx(a, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }                                                       \
    if (r == 1) {                                           \
        PlaySfx(b, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }                                                       \
    if (r == 2) {                                           \
        PlaySfx(c, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }                                                       \
    if (r == 3) {                                           \
        PlaySfx(d, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }

void DinoChooseWinnerPuffs(s32 arg0, u16 arg1);

void DinoChooseWinnerScore(s32 arg0, u16 arg1);

s32 XZBoxToSphere(SLONG_XYZ *a, SLONG_XYZ *b, s32 c, SLONG_XYZ *e);

void SetDinoHeroDazed(s32 id);

void PlotDinoGem(NAS *m);

#endif /* BASH_DINO_OVL_H */

void ModelCreateShadow(s32 idx);
void ModelUpdateShadow(s32 idx);
extern s32 DinoVictorySeqs[]; /* {name, id} pairs: the victory sequences (the engine's hook 0x14) */
extern DinoObj *DinoTargetD, *DinoTargetE;
extern s32 DinoTargetTimer, DinoTargetDelay, DinoTargetStep, DinoScoreTimer, DinoTargetFlag;
void DinoChooseWinnerTeam(s32 arg0, u16 arg1);
void DinoPreMoveHero();
void DinoMoveHero();

typedef struct DinoVec {
    s32 vx, vy, vz;
} DinoVec;

/* A wall segment of a scene collision object (DinoTrackCollide). */
typedef struct DinoSeg {
    DinoVec p0; /* 0x0 */
    DinoVec p1; /* 0xC */
    s32 len;      /* 0x18 */
    s32 ang;      /* 0x1C */
    u8 pad20[0x18];
} DinoSeg;

typedef struct DinoSegObj {
    u8 pad0[0x38];
    s32 count; /* 0x38 */
    u8 pad3C[0xC];
    DinoSeg segs[1]; /* 0x48 */
} DinoSegObj;

typedef struct DinoSceneData {
    u8 pad0[0x14];
    s32 off14;
} DinoSceneData;

s32 DinoTrackCollide(DinoVec *pos, DinoVec *out, s32 radius, s32 idx, s32 flags);

/* DinoScoreGems: DinoGoal (the swinging target) as it moves. */
typedef struct DinoTarget {
    u8 pad0[4];
    s32 x;      /* 0x04 */
    u8 pad8[4];
    s32 z;      /* 0x0C */
    u8 pad10[2];
    s16 ang;    /* 0x12 */
    s16 ang2;   /* 0x14 */
    u8 pad16[0x51 - 0x16];
    u8 unk51;   /* 0x51 */
    s16 unk52;  /* 0x52 */
} DinoTarget;

extern s32 DinoScoreA;
extern s32 DinoScoreB;

static inline s32 DinoDot(VECTOR *a, VECTOR *b) {
    return (a->vx * b->vx >> 12) + (a->vz * b->vz >> 12);
}
/* Dino's AI tweak tables and hero models (halfwords) */
extern u16 DinoAiSkill[];
extern u16 DinoAiSkillAlt[];
extern u16 DinoAiTeamSkill[];
extern u16 DinoAiHumanRoundTweak[];
extern u16 DinoAiBotTweak[];
extern u16 DinoAiHumanTweak[];
extern u16 DinoHeroModels[];
