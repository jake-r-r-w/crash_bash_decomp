#ifndef BASH_BONK_OVL_H
#define BASH_BONK_OVL_H

/* Functions and data defined in the bonk overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/bonk/bonk.c */

#include "common.h"
#include "bash/nas.h"

typedef struct BonkPad {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ EXTengine *eng;
    /* 0x58 */ u8 pad58[4];
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
typedef struct { s32 a, b, c; } BonkEnt;
extern BonkEnt BonkEnts[];
extern s32 BonkCrateFrames[];
extern s32 BonkStoolSfx[];
extern s32 GamePause;
/* PlotBonkCrate's puff of smoke at pos (type 0x1605, flags 0x11). */
static inline void Bonk_SpawnPuff(s32 *pos, s32 mode, u8 *p) {
    NAS *obj;
    s32 *ext;

    obj = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (obj != NULL) {
        ext = FindExtension(obj, 0x10000000);
        if (ext != NULL) {
            ext[3] = mode;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)obj->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            obj->InitDelete(obj);
        }
    }
}
extern s32 BonkEntCur;
extern u16 BonkAIRange[];
extern ScriptInit BonkFlagScriptInit;
extern s32 GameTeams[];
extern s32 PanelCtrl;
extern void *AiSkillTable;
extern void *AiTeamSkillTable;
extern void *AiHumanRoundTweakTable;
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
    s32 unk4;
    s32 unk8;
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

int BonkAI_Closest(int a0, int *out);
/* HeroHooks (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct BonkHooks {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ void *HeroModels;
    /* 0x08 */ u8 pad8[0x18];
    /* 0x20 */ void *PreMoveHero;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ void *PostMoveHero;
    /* 0x2C */ void *HeroFacing;
    /* 0x30 */ u8 pad30[8];
    /* 0x38 */ void *HeroBot;
    /* 0x3C */ void *TestAttack;
    /* 0x40 */ s32 (*TestRun)(s32);
    /* 0x44 */ void *TestJump;
    /* 0x48 */ void *TestKick;
    /* 0x4C */ u8 pad4C[8];
    /* 0x54 */ void (*HeroKickCollision)();
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ void *HeroExtraEffects;
    /* 0x60 */ u8 pad60[4];
    /* 0x64 */ void *ChooseWinner;
    /* 0x68 */ void *KeepInBounds;
} BonkHooks;
extern BonkHooks HeroHooks;
extern s16 HeroActHit;
extern s16 HeroActWinner;
extern s16 HeroActLoser;
s32 HeroNoKick(void);
extern u16 BonkHeroModels[];
s32 HeroNoJump(s32 arg0);
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

/* GameInfo as seen by BonkChooseWinnerRank (the final ranking). */
typedef struct BonkRankView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32[0x48 - 0x32];
    /* 0x48 */ s16 team[2];
    /* 0x4C */ u8 unk4C[0x60 - 0x4C];
    /* 0x60 */ u8 winner[2];
    /* 0x62 */ u8 unk62[0x6C - 0x62];
    /* 0x6C */ s32 teams;
    /* 0x70 */ u8 unk70[0xA7 - 0x70];
    /* 0xA7 */ u8 order[4];
} BonkRankView;
#define RANK66V ((BonkRankView *)GameInfo)

/* BonkAI_FindClosest: keep the closest target found so far (both search loops). */
#define BONK_SET_CLOSEST(best, found, d, p) \
    do {                                     \
        (best) = (d);                        \
        (found) = (p);                       \
    } while (0)

void PlotBonkShockwave(NAS *nas);

void BONKSetUp(void);

/* MainCamera as BonkInitCamera sets it up. */
typedef struct BonkCam {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x54 - 0x1C];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
} BonkCam;

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

extern u16 Clock_flags;

extern u16 D_8009E080[][4];
void HeroTakeHit(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);

void PlotBonkCrate(NAS *nas);

s32 BonkAI_FindClosest(s32 arg0, s32 *arg1, s32 arg2);

/* Target found by BonkAI_Closest (Bonk_HeroBot reads it). */
typedef struct BonkHit {
    /* 0x00 */ s32 kind;
    /* 0x04 */ NAS *nas;
} BonkHit;
extern u32 DirPadBits[]; /* pad flags by angle octant */

/* Squared ground distance from (x0,z0) to (x1,z1), as Bonk_HeroBot uses it. */
static inline s32 bonk_sq(s32 x0, s32 z0, s32 x1, s32 z1)
{
    s32 dx = x1 - x0;
    s32 dz = z1 - z0;

    return dx * dx + dz * dz;
}

void PlotBonkStool(NAS *nas);

#endif /* BASH_BONK_OVL_H */
