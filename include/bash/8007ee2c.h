#ifndef BASH_8007EE2C_H
#define BASH_8007EE2C_H

/* Functions and data defined in src/overlays/engine/8007ee2c.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/8007ee2c.c */

#include "common.h"
#include "bash/engine.h"

extern s32 (*D_8009D724)(s32);
extern s32 (*D_8009D738)(s32, s32);
extern s32 (*D_8009D730)(s32);
typedef struct {
    s32 a;
    u16 b;
    u16 pad;
} D_8009D6FC_t;
extern D_8009D6FC_t *D_8009D6FC;
/* Per-hero run speed rows: 3 u16 per hero_name (func_800859D4). */
extern u16 *D_8009D6F4;
extern s16 D_8009D864;
extern u16 D_8009D866;
extern s16 D_8009D870;
extern u16 D_8009D872;

void func_80086704(void);

s32 func_8008670C(void);

s32 func_80086714(void);

s32 func_8008671C(void);

s32 func_80086724(void);

void func_800867EC(void);

void func_8008680C(void);

void func_80086814(void);

void func_8008AA90(void);

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

void func_80082590(void);

void func_800826F8(s32 arg0);

s32 SetHeroSeq(HeroInfo *hero, s32 seq);

extern s16 D_8009D864;
extern u16 D_8009D866;
extern s16 D_8009D870;
extern u16 D_8009D872;

void SetHeroAction(HeroInfo *hero, s32 action);

void func_80086FA0(void);

u8 *func_8008D24C(u8 *arg0, s32 *arg1);

s32 func_8007EE2C(s32 pad);

s32 func_80086FCC(s32 arg0);

void func_80086F78(void);

void func_8008A8F0(s32 arg0);

u8 *func_8008D284(u8 *arg0, u8 *arg1);

void func_800831C4(s32 arg0);

void func_8008352C(void);

void func_80086868(void);

void func_80086F4C(void);

s32 func_800866DC(s32 *arg0);

s32 func_8008DE94(s32 arg0);

void func_800834E4(void);

s16 func_800831F0(void);

void func_8008688C(void);

s32 func_8008AB04(s32 arg0);

void func_80084788();

void func_800859D4();

void func_80085C84();

void func_80085D2C();

void func_80088BAC(s32 arg0);

void func_800894E0(s32 arg0);

void func_8008A9F0();

void func_8008AA40();

void func_80083550(void);

void func_80082590(void);

void func_8008A810(void);

s32 func_80083488(void);

s32 func_8008AB50(s32 arg0);

void func_80086D88(void);

void func_8008A99C(s32 arg0);

void func_8008AAB0(s32 arg0);

s32 func_8008AB94(s32 arg0);

s32 func_8008C4C8(s32 *arg0, s32 arg1);

void InitWeatherNas(u8 *arg0);

void func_80083244(void);

s32 func_800832BC(void);

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

extern s32 D_8009D764[16][4];

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} D_8009D87C_t;

extern D_8009D87C_t D_8009D87C[];

/* func_80084E04: harm tables. */
extern u16 D_8009D3B8[][4];
extern u16 D_8009D400[][9];
extern u16 D_8009D4A4[];

extern s16 D_8009E0D0;
extern s16 D_8009E0D2;
extern s16 D_8009E0D4;
extern s16 D_8009E0D6;
extern s8 D_8004E0A4[];
extern s32 D_8009BD08;
extern s32 D_8009BD40;
extern s32 D_8009BD44;
extern s16 D_8009ADE6;

s32 func_8007EFC0(s32 pad, s32 max);

s32 func_8007F1A8(s32 pad, s32 max);

void func_80082C34(void);

void func_80083334(s32 arg0, s32 kind, s32 flag);

s32 func_80085584(s32 hero, u16 name, s32 seq);

void func_80085650(void);

s32 func_80085890(s32 arg0);

s32 func_800863D0(s32 arg0);

s32 func_80086C94(s32 arg0);

void func_80087FC0(s32 arg0);

void func_800880E0(s32 arg0);

void func_800881E0(s32 arg0);

void func_800882DC(s32 arg0);

void func_80088450(s32 arg0);

extern u8 D_80055534[];

void func_80082AC4(void);

void func_80084E04(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);

void func_80085718(s32 arg0, u16 arg1);

void func_8008B29C(s32 idx);

void func_8008B418(s32 idx);

void func_8008D5D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void func_8008D6C8(NAS *nas);

s32 func_800864CC(s32 id, u32 flag);

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
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E2);
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
        SetHeroAction((HeroInfo *)arg0, act);
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
                SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0DE);
            } else {
                SetHeroAction((HeroInfo *)arg0, 3);
            }
            return 1;
        }
        return 0;
    }
    return 1;
}

s32 func_800871E4(s32 arg0);

extern s32 (*D_8009D730)(s32);

static inline s32 HeroCheckPad1000(s32 arg0) {
    s32 ret;

    if (D_8009D730 != NULL) {
        return D_8009D730(arg0);
    }
    if (Heroes[arg0].hero_status & 0xFA00) {
        return 0;
    }
    if (((PadButtons *)D_80051338)[arg0].unk50 & 0x1000) {
        SetHeroAction((HeroInfo *)arg0, 2);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void func_8008795C(s32 arg0);

void InitHealth(NAS *nas);

/* ballistic arc state used by func_8008C5C4 */
typedef struct {
    /* 0x00 */ s32 gravity;
    /* 0x04 */ s32 started;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 vy0;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 vy;
    /* 0x18 */ s32 floor;
    /* 0x1C */ s32 unk1C[2];
    /* 0x24 */ s32 angle;
    /* 0x28 */ s32 unk28[3];
    /* 0x34 */ s32 vx;
    /* 0x38 */ s32 vz;
    /* 0x3C */ s32 unk3C[2];
    /* 0x44 */ s32 speed;
} Arc;

s32 csqrt(s32 x);
s32 abs(s32);

void func_80089FA4(s32 arg0);

s32 func_8008C5C4(s32 *from, s32 *to, Arc *arc, s32 *pos);

extern u16 D_8009E214[];
extern u16 D_8009E224[];

void func_800860D4(s32 arg0);

void func_80088C28(s32 arg0);

void PlotHealth(NAS *nas);

/* func_800898EC: a 14-byte record per hero, 3 rows of 4 heroes. */
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
} D_8009E0E8_t;
extern D_8009E0E8_t D_8009E0E8[3][4];
extern u8 D_80053004[];

void func_8008955C(s32 arg0);

void func_800898EC(s32 arg0);

#endif /* BASH_8007EE2C_H */
