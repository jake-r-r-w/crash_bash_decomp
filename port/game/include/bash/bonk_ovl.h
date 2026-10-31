#ifndef BASH_BONK_OVL_H
#define BASH_BONK_OVL_H

/* Functions and data defined in the bonk overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/bonk/bonk.c */

#include "common.h"
#include "bash/nas.h"

typedef struct BonkPad {
    /* 0x00 */ u8 pad0[offsetof(NAS, Engine)];
    /* 0x54 */ EXTengine *eng;
    /* 0x58 */ u8 pad58[offsetof(NAS, UserInfo) - (offsetof(NAS, Engine) + sizeof(void *))];
    /* 0x5C */ u16 mode;
} BonkPad;

typedef struct BonkTgt2 {
    /* 0x0 */ s32 kind;
    /* 0x4 */ BonkPad *obj;
} BonkTgt2;

extern s32 BonkTargetSfx[];

typedef struct BonkObj {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ BonkPad *pad;
} BonkObj;

extern s32 BonkObjectSfx[];
void BonkHeroHitFlash(s32 arg0, s32 arg1);

s32 BonkDist2XZ(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void PlotBonkFlag(NAS *nas);
void BonkUpdateStub(void);
void UpdateBONK(void);
void BonkSetClock(s32 arg0);
void BonkSetClockA(void);
void BonkSetClockB(void);
void BonkSetClockC(void);
void InitialiseBONK(void);
void BonkRestartLevel(void);
void FirstTriggersBONK(void);

extern s32 BonkSkill;
typedef struct { s32 a; void *b; s32 c; } BonkEnt;
extern BonkEnt BonkEnts[];
extern s32 BonkCrateFrames[];
extern s32 BonkStoolSfx[];
/* PlotBonkCrate's puff of smoke at pos (type 0x1605, flags 0x11). */
static inline void Bonk_SpawnPuff(s32 *pos, s32 mode, u8 *p) {
    NAS *obj;
    EXTscriptdata *ext;

    obj = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (obj != NULL) {
        ext = FindExtension(obj, 0x10000000);
        if (ext != NULL) {
            ext->scriptnumber = mode;
            ext->speed = 0x100;
            if (p != NULL) {
                ext->flags = 5;
                obj->Engine->sub->u6C.ptr = p;
            } else {
                ext->flags = 1;
            }
            ext->maxloops = 0;
            obj->InitDelete(obj);
        }
    }
}
extern s32 BonkEntCur;
extern u16 BonkAIRange[];
extern ScriptInit BonkFlagScriptInit;
extern u16 BonkAiSkill[];
extern u16 BonkAiSkillAlt[];
extern u16 BonkAiTeamSkill[];
extern u16 BonkAiHumanRoundTweak[];
extern u16 BonkAiHumanRoundTweakAlt[];

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/bonk_bohero.h"

void InitialiseBONKFirst();

typedef struct BonkEntry {
    s32 unk0;
    void *flag;  /* tested for zero */
    s32 timer;  /* counted down */
} BonkEntry;
extern s32 BonkEntryCount;
extern s32 BonkStoolCount;
extern s32 BonkRoundTime;
extern BonkEntry BonkEntries[10];
extern s32 BonkEntryCur;
extern s32 BonkCrateTimer;
extern s32 BonkCrateDelay;
extern s32 BonkCrateCount;
extern s32 BonkRoundTimer;
extern s32 BonkRoundState;
extern s32 BonkStoolFrames[];
extern s32 BonkStoolHeights[];
extern s32 BonkStoolTable[];

int BonkAI_Closest(int a0, void **out);
extern u16 BonkHeroModels[];
void BonkHeroFacing(s32 arg0);
s32 BONK_KeepInBounds(EXTengine *eng);
void BonkPreMoveHero(s32 arg0);
s32 BonkHeroKickCollision(s32 arg0);
s32 BonkHeroTestSwing(s32 arg0);
void BonkHero_ExtraEffects(s32 player);
extern u16 BonkPowerupTimes[];
void Bonk_HeroBot(s32 arg0);
void BonkInitTweakAi(void);

void BonkChooseWinner(s32 arg0, s32 arg1);
void BonkChooseWinnerScore(s32 arg0, u16 arg1);
void BonkChooseWinnerScoreB(s32 arg0, u16 arg1);
void BonkChooseWinnerTimed(s32 arg0, u16 arg1);
void BonkChooseWinnerRank(s32 arg0, u16 arg1);

#define RANK66V ((GameState *)GameInfo)

/* BonkAI_FindClosest: keep the closest target found so far (both search loops). */
#define BONK_SET_CLOSEST(best, found, d, p) \
    do {                                     \
        (best) = (d);                        \
        (found) = (p);                       \
    } while (0)

void PlotBonkShockwave(NAS *nas);

void BONKSetUp(void);


void BonkInitEntries(void);

void BonkCrateMaster(void);

s32 BonkAI_PickEntry(s32 *arg0, s32 *arg1);

void InitBonkCrate(NAS *nas);

s32 BonkAI_NearestEnemy(s32 arg0);

void InitBonkFlag(NAS *nas);

void InitBonkShockwave(NAS *nas);

void BonkHitObject(s32 arg0, s32 arg1, BonkObj *obj);

void BonkSpawnStool(void);

void InitBonkStool(NAS *nas);

void BonkSpawnCrate(void);

void BonkUpdateSkill(void);

void BonkInitScores(void);

void BonkInitCamera(void);

void BonkKnockBack(s32 arg0, EXTengine *p, s32 arg2);



void PlotBonkCrate(NAS *nas);

s32 BonkAI_FindClosest(s32 arg0, void **arg1, s32 arg2);

/* Target found by BonkAI_Closest (Bonk_HeroBot reads it). */
typedef struct BonkHit {
    /* 0x00 */ s32 kind;
    /* 0x04 */ NAS *nas;
} BonkHit;

/* Squared ground distance from (x0,z0) to (x1,z1), as Bonk_HeroBot uses it. */
static inline s32 bonk_sq(s32 x0, s32 z0, s32 x1, s32 z1)
{
    s32 dx = x1 - x0;
    s32 dz = z1 - z0;

    return dx * dx + dz * dz;
}

void PlotBonkStool(NAS *nas);

#endif /* BASH_BONK_OVL_H */
