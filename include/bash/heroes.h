#ifndef BASH_HEROES_H
#define BASH_HEROES_H

/* Functions and data defined in src/overlays/engine/heroes.c, the file after PAUSE.CPP.
 * Anything the original headers declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/heroes.c */

#include "common.h"
#include "bash/engine.h"

void DefaultTouched(s32 arg0, s32 arg1);
s32 func_8008670C(void);
s32 func_80086714(void);
s32 func_8008671C(void);
s32 DefHero2ObjectCollision(void);
void func_800867EC(void);
void func_8008680C(void);
void func_80086814(void);
void HeroQuiet(void);
s32 func_8008AB48(void);
void func_8008AD9C(void);
void func_8008CE10(void);
void func_8008CE18(void);
s32 func_8008CEB0(u8 *arg0, u8 *arg1);
void PlotParticleEffect(void);
void InitParticleEffect(void);
void InitSmokePoof(NAS *nas);
void InitFloatyScore(u8 *arg0);
void PlotBigPoof(NAS *nas);
void func_8008DDC0(NAS *nas);
s32 func_8008DE8C(void);
s32 SetHeroSeq(s32 player, s32 seq);
void SetHeroAction(s32 player, s32 action);
void func_80086FA0(void);
u8 *func_8008D24C(u8 *arg0, s32 *arg1);
s32 func_80086FCC(s32 arg0);
void func_80086F78(void);
void func_8008A8F0(s32 arg0);
u8 *func_8008D284(u8 *arg0, u8 *arg1);
void func_80086868(void);
void func_80086F4C(void);
s32 func_800866DC(s32 *arg0);
s32 func_8008DE94(s32 arg0);
void func_8008688C(void);
s32 func_8008AB04(s32 arg0);
void func_80084788(s32 idx, u16 arg1);
void func_800859D4();
void func_80085C84();
void func_80085D2C();
void func_80088BAC(s32 arg0);
void func_800894E0(s32 arg0);
void func_8008A9F0();
void func_8008AA40();
void func_8008A810(void);
s32 func_8008AB50(s32 arg0);
void func_80086D88(void);
void func_8008A99C(s32 arg0);
void func_8008AAB0(s32 arg0);
s32 func_8008AB94(s32 arg0);
s32 func_8008C4C8(s32 *arg0, s32 arg1);
void InitWeatherNas(u8 *arg0);
void func_80088544(s32 arg0);
void func_80088F78(s32 arg0);
s32 func_8008ABF8(s32 arg0);
s32 func_8008AC74(s32 arg0);
void PlotWeatherNas(u8 *arg0);
FlagPoint *func_8008CE20(subengine *sub, s32 idx, s32 mask);
void func_8008D088(u8 *arg0, u8 *arg1, EmitPart *arg2, s32 *arg3, u8 *arg4);
void func_8008D488(u8 *arg0, s32 arg1);
void InitBigPoof(NAS *nas);
void func_80088708(s32 arg0);
void func_8008A854(s32 arg0);
s32 func_8008ACF0(s32 arg0);
void func_8008672C(s32 idx);
void func_80087428(s32 arg0);
void PlotSmokePoof(NAS *m);
void func_8008CFBC(EXTengine *eng);
void PlotFloatyScore(NAS *nas);
s32 func_80085584(s32 hero, u16 name, s32 seq);
void func_80085650(void);
s32 func_80085890();
s32 func_800863D0(s32 arg0);
s32 func_80086C94(s32 arg0);
void func_80087FC0(s32 arg0);
void func_800880E0(s32 arg0);
void func_800881E0(s32 arg0);
void func_800882DC(s32 arg0);
void func_80088450(s32 arg0);
void func_80084E04(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);
void func_80085718(s32 arg0, u16 arg1);
void func_8008B29C(s32 idx);
void func_8008B418(s32 idx);
void func_8008D5D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8008D6C8(NAS *nas);
s32 func_800864CC(s32 id, u32 flag);
s32 func_800871E4(s32 arg0);
void func_8008795C(s32 arg0);
void func_800874EC(s32 arg0);
void Init_HealthPU(NAS *nas);
void func_80089FA4(s32 arg0);
s32 func_8008C5C4(s32 *from, s32 *to, Arc *arc, s32 *pos);
void func_800860D4(s32 arg0);
void func_80088C28(s32 arg0);
void Plot_HealthPU(NAS *nas);
void SetTweakAi(void);
void func_80087B8C(s32 arg0);
void func_80088794(s32 arg0);
void func_8008955C(s32 arg0);
void func_800898EC(s32 arg0);
void func_80083E8C(void);
void func_8008900C(s32 arg0);
s32 func_8008A200(s32 arg0);
s32 func_8008ADA4(u8 *arg0, u8 *arg1, u8 *arg2, s32 *arg3);
void InitScriptFX(NAS *nas);
void func_8008CA90(u8 *arg0, MATRIX *m);
void func_80083708(void);
void func_8008A924(s32 arg0);
void func_8008C824(SparkObj *obj);
void PlotScriptFX(NAS *nas);

/* Inline copy of SetHeroAction, the action handler taking the hero id
 * (func_800860D4 uses it). */
static inline void SetHeroActionInline(s32 hero, s32 action) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + hero;
    u16 old = h->hero_mode;

    h->hero_mode = action;
    h->hero_last_mode = old;
    ((void (*)(s32))HeroActionTable[action].fn)(hero);
}

/* Inline copies of the hero action checks func_8008AB94, func_8008ABF8 and
 * func_8008ACF0 (func_8008795C uses them). */
static inline s32 HeroCheckStatus2000(s32 arg0) {
    s32 ret;

    if (Heroes[arg0].hero_status & 0x2000) {
        SetHeroAction(arg0, (u16)D_8009E0E2);
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
        act = D_8009E0E4;
        goto call;
    }
    if (temp_v1 & 0x400) {
        act = D_8009E0E6;
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

    if (D_8009D728(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        if (h->hero_move->forward_speed != 0) {
            mode = h->hero_mode;
            if (mode == 0xF || mode == 0x17) {
                SetHeroAction(arg0, (u16)D_8009E0DE);
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

    if (D_8009D730 != NULL) {
        return D_8009D730(arg0);
    }
    if (Heroes[arg0].hero_status & 0xFA00) {
        return 0;
    }
    if (((PadButtons *)D_80051338)[arg0].unk50 & 0x1000) {
        SetHeroAction(arg0, 2);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

/* func_80087B8C: stop check (hero not moving). */
static inline s32 HeroCheckStopped(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 mode;

    base = Heroes;
    h = base + arg0;
    if (h->hero_move->forward_speed == 0) {
        mode = h->hero_mode;
        if (mode == 0x10 || mode == 0x18) {
            SetHeroAction(arg0, (u16)D_8009E0DC);
        } else {
            SetHeroAction(arg0, 0);
        }
        return 1;
    }
    return 0;
}

#endif /* BASH_HEROES_H */
