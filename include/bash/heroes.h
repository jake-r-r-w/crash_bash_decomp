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
s32 DefHero2ObjectCollision(void);
void func_800867EC(void);
void DefHeroBot(void);
void ReleaseHeroAnims(void);
void HeroQuiet(void);
s32 HeroNoKick(void);
void func_8008AD9C(void);
void func_8008CE10(void);
void func_8008CE18(void);
s32 DistXZSq(u8 *arg0, u8 *arg1);
void PlotParticleEffect(void);
void InitParticleEffect(void);
void InitSmokePoof(NAS *nas);
void InitFloatyScore(u8 *arg0);
void PlotBigPoof(NAS *nas);
void func_8008DDC0(NAS *nas);
s32 func_8008DE8C(void);
s32 SetHeroSeq(s32 player, s32 seq);
void SetHeroAction(s32 player, s32 action);
void ArenaGridReset(void);
u8 *func_8008D24C(u8 *arg0, s32 *arg1);
s32 HeroTestActions(s32 arg0);
void ArenaGridClear(void);
void HeroJump(s32 arg0);
u8 *func_8008D284(u8 *arg0, u8 *arg1);
void func_80086868(void);
void func_80086F4C(void);
s32 func_800866DC(s32 *arg0);
s32 func_8008DE94(s32 arg0);
void SetLevelCameraMode(void);
s32 HeroNoJump(s32 arg0);
void DefChooseWinner(s32 idx, u16 arg1);
void DefPreMoveHero();
void DefHeroFacing();
void DefMoveHero();
void SetHeroWhip(s32 arg0);
void SetHeroKick(s32 arg0);
void DefHeroTouched();
void DefHeroSetIdle();
void FinishLevel(void);
s32 HeroIsWinnerOrLoser(s32 arg0);
void MarkHeroesOnGrid(void);
void SetHeroTaunt(s32 arg0);
void SetHeroQuiet(s32 arg0);
s32 HeroCheckDying(s32 arg0);
s32 func_8008C4C8(s32 *arg0, s32 arg1);
void InitWeatherNas(u8 *arg0);
void HeroLand(s32 arg0);
void SetHeroShoot(s32 arg0);
s32 HeroCheckResult(s32 arg0);
s32 func_8008AC74(s32 arg0);
void PlotWeatherNas(u8 *arg0);
FlagPoint *SubFindFlagPoint(subengine *sub, s32 idx, s32 mask);
void func_8008D088(u8 *arg0, u8 *arg1, EmitPart *arg2, s32 *arg3, u8 *arg4);
void FormatPercent(u8 *arg0, s32 arg1);
void InitBigPoof(NAS *nas);
void SetHeroSpin(s32 arg0);
void HeroDrop(s32 arg0);
s32 func_8008ACF0(s32 arg0);
void UpdateHeroShadow(s32 idx);
void SetHeroIdle(s32 arg0);
void PlotSmokePoof(NAS *m);
void func_8008CFBC(EXTengine *eng);
void PlotFloatyScore(NAS *nas);
s32 SetHeroAseq(s32 hero, u16 name, s32 seq);
void ClearHeroAnimSlots(void);
s32 CheckHeroAnim();
s32 HeroMovedDist(s32 arg0);
s32 func_80086C94(s32 arg0);
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
void func_8008D6C8(NAS *nas);
s32 HeroTestBox(s32 id, u32 flag);
s32 func_800871E4(s32 arg0);
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
s32 func_8008A200(s32 arg0);
s32 func_8008ADA4(u8 *arg0, u8 *arg1, u8 *arg2, s32 *arg3);
void InitScriptFX(NAS *nas);
void PlotFallParticles(u8 *arg0, MATRIX *m);
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
 * func_8008ACF0 (HeroTaunt uses them). */
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
    if (((PadButtons *)Pads)[arg0].unk50 & 0x1000) {
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
