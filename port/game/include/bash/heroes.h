#ifndef BASH_HEROES_H
#define BASH_HEROES_H

/* Functions and data defined in src/overlays/engine/heroes.c, the file after PAUSE.CPP.
 * Anything the original headers declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/heroes.c */

#include "common.h"
#include "bash/engine.h"

void DefaultTouched(s32 arg0, s32 arg1);
s32 DefKeepInBounds(void);
s32 DefHeroKickCollision(void);
s32 DefHeroGrabObject(void);
s32 DefHero2ObjectCollision(u32 player, u16 a, u16 b, u16 c);
void MoveSecondHero(void);
void DefHeroBot(void);
void ReleaseHeroAnims(void);
void HeroQuiet(u32 player);
s32 HeroNoKick(void);
void DebugNasStatus(void);
void WeatherUnusedA(void);
void WeatherUnusedB(void);
s32 DistXZSq(SLONG_XYZ *a, SLONG_XYZ *b);
void PlotParticleEffect(void);
void InitParticleEffect(void);
void InitSmokePoof(NAS *nas);
void InitFloatyScore(NAS *nas);
void PlotBigPoof(NAS *nas);
void AddPowerupFloat(NAS *nas);
s32 HeroStubFalse(void);
s32 SetHeroSeq(s32 player, s32 seq);
void SetHeroAction(s32 player, s32 action);
extern u16 HeroLastHarm[4];
extern u16 RoundWinner;
extern s32 *CrateHitShapes[];
extern s32 *CrateHitShapes2[];
extern u16 HeroHitSfx[][4];
extern u16 PickupCounts[30];
extern s32 PlayerHitSfx[];
s32 ProcessHeroAnim(s32 player);            /* defined K&R style (heroes.c) */
void ArenaGridReset(void);
u8 *script_track_keys(u8 *arg0, threaddata *t);
s32 HeroTestActions(s32 arg0);
void ArenaGridClear(void);
void HeroJump(s32 arg0);
partdata *script_track_data(u8 *arg0, threaddata *t);
void ResetHeroGlobals(void);
void ArenaGridScan(void);
s32 HeroCopyPosStub(s32 *arg0);
s32 HeroIsActive(s32 arg0);
void SetLevelCameraMode(void);
s32 HeroNoJump(s32 arg0);
void DefChooseWinner(s32 idx, u16 arg1);
void DefPreMoveHero(s32 arg0);
void DefHeroFacing(s32 arg0);
void DefMoveHero(s32 arg0);
void SetHeroWhip(s32 arg0);
void SetHeroKick(s32 arg0);
void DefHeroTouched(s32 arg0);
void DefHeroSetIdle(s32 arg0);
void FinishLevel(void);
s32 HeroIsWinnerOrLoser(s32 arg0);
void MarkHeroesOnGrid(void);
void SetHeroTaunt(s32 arg0);
void SetHeroQuiet(s32 arg0);
s32 HeroCheckDying(s32 arg0);
s32 PointInZone(s32 *arg0, s32 arg1);
void InitWeatherNas(NAS *m);
void HeroLand(s32 arg0);
void SetHeroShoot(s32 arg0);
s32 HeroCheckResult(s32 arg0);
s32 HeroTestStop(s32 arg0);
void PlotWeatherNas(NAS *m);
FlagPoint *SubFindFlagPoint(subengine *sub, s32 idx, s32 mask);
void emit_script_particle(subengine *arg0, partdata *arg1, EmitPart *arg2, s32 *arg3, threaddata *arg4);
void FormatPercent(u8 *arg0, s32 arg1);
void InitBigPoof(NAS *nas);
void SetHeroSpin(s32 arg0);
void HeroDrop(s32 arg0);
s32 HeroTestMove(s32 arg0);
void UpdateHeroShadow(s32 idx);
void SetHeroIdle(s32 arg0);
void PlotSmokePoof(NAS *m);
void KillScriptFx(EXTengine *eng);
void PlotFloatyScore(NAS *nas);
s32 SetHeroAseq(s32 hero, u16 name, s32 seq);
void ClearHeroAnimSlots(void);
s32 CheckHeroAnim(s32 arg0);
void HeroGhost(u32 arg0);
s32 HeroMovedDist(s32 arg0);
s32 AdjustAiSkill(s32 arg0);
void SetHeroJump(s32 arg0);
void SetHeroFall(s32 arg0);
void HeroFall(s32 arg0);
void SetHeroDrop(s32 arg0);
void SetHeroLand(s32 arg0);
void HeroTakeHit(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);
void RequestHeroAnim(s32 arg0, u16 arg1);
void ModelCreateShadow(s32 idx);
void ModelUpdateShadow(s32 idx);
void CreateFloatyScore(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void UpdateBounceMotion(NAS *nas);
s32 HeroTestBox(s32 id, u32 flag);
s32 HeroTestFall(s32 arg0);
void HeroTaunt(s32 arg0);
void HeroIdle(s32 arg0);
void Init_HealthPU(NAS *nas);
void StartHeroVictory(s32 arg0);
s32 ArcUpdate(s32 *from, s32 *to, Arc *arc, s32 *pos);
void HeroApplyJump(s32 arg0);
void HeroWhip(s32 arg0);
void Plot_HealthPU(NAS *nas);
void SetTweakAi(void);
void HeroRun(s32 arg0);
void HeroSpin(s32 arg0);
void HeroKick(s32 arg0);
void HeroBecomeAngel(s32 arg0);
void ProcessHeroes(void);
void HeroShoot(s32 arg0);
s32 UpdateHeroVictory(s32 arg0);
s32 UpdateFxEmitter(EXTscriptdata *sd, subengine *arg1, threaddata *td, s32 *arg3);
void InitScriptFX(NAS *nas);
void PlotFallParticles(subengine *sub, MATRIX *m);
void InitialiseHeroes(void);
void SetHeroRun(s32 arg0);
void PlotSparks(SparkObj *obj);
void PlotScriptFX(NAS *nas);

/* Inline copy of SetHeroAction, the action handler taking the hero id
 * (HeroApplyJump uses it). */
static inline void SetHeroActionInline(s32 hero, s32 action) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + hero;
    u16 old = h->hero_mode;

    h->hero_mode = action;
    h->hero_last_mode = old;
    ((void (*)(s32))HeroActionTable[action].fn)(hero);
}

/* Inline copies of the hero action checks HeroCheckDying, HeroCheckResult and
 * HeroTestMove (HeroTaunt uses them). */
static inline s32 HeroCheckStatus2000(s32 arg0) {
    s32 ret;

    if (Heroes[arg0].hero_status & 0x2000) {
        SetHeroAction(arg0, (u16)HeroActDead);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 HeroCheckStatus600(s32 arg0) {
    u16 temp_v1;
    u16 act;
    s32 ret;

    temp_v1 = Heroes[arg0].hero_status;
    if (temp_v1 & 0x200) {
        act = HeroActWinner;
        goto call;
    }
    if (temp_v1 & 0x400) {
        act = HeroActLoser;
    call:
        SetHeroAction(arg0, act);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 HeroCheckMoving(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 mode;

    if (HeroHooks.TestRun(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        if (h->hero_move->forward_speed != 0) {
            mode = h->hero_mode;
            if (mode == 0xF || mode == 0x17) {
                SetHeroAction(arg0, (u16)HeroActCarry);
            } else {
                SetHeroAction(arg0, 3);
            }
            return 1;
        }
        return 0;
    }
    return 1;
}

static inline s32 HeroCheckPad1000(s32 arg0) {
    s32 ret;

    if (HeroHooks.TestKick != NULL) {
        return HeroHooks.TestKick(arg0);
    }
    if (Heroes[arg0].hero_status & 0xFA00) {
        return 0;
    }
    if (((PadState *)Pads)[arg0].dir & 0x1000) {
        SetHeroAction(arg0, 2);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

/* HeroRun: stop check (hero not moving). */
static inline s32 HeroCheckStopped(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 mode;

    base = Heroes;
    h = base + arg0;
    if (h->hero_move->forward_speed == 0) {
        mode = h->hero_mode;
        if (mode == 0x10 || mode == 0x18) {
            SetHeroAction(arg0, (u16)HeroActHold);
        } else {
            SetHeroAction(arg0, 0);
        }
        return 1;
    }
    return 0;
}

#endif /* BASH_HEROES_H */
