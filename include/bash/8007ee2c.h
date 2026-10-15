#ifndef BASH_8007EE2C_H
#define BASH_8007EE2C_H

/* Functions and data defined in src/overlays/engine/8007ee2c.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/8007ee2c.c */

#include "common.h"
#include "bash/engine.h"

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

s32 func_80085584(s32 hero, u16 name, s32 seq);

void func_80085650(void);

void func_8008D5D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

#endif /* BASH_8007EE2C_H */
