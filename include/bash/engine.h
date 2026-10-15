#ifndef BASH_ENGINE_H
#define BASH_ENGINE_H

/* Functions and data defined in the engine overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/engine.c */

#include "common.h"
#include "bash/nas.h"

void func_8007A688(s32 arg0);
void func_8007A6B0(s32 arg0);
void func_8007A6D0(s32 arg0);
void func_8007A6F8(void);
void func_8007A7C0(void);
void func_8007A9D0(u8 *arg0);
void func_8007AAF0(void);
void func_8007AB20(void);
void func_8007AB6C(void);
void func_8007AB8C(void);
void func_8007ABC4(void);
void func_8007E7A4(void);
s32 func_8007E7C8(void);
void func_8007E874(void);
void func_8007EB04(void);
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
void InitFloatyScore(u8 *arg0);
void PlotBigPoof(NAS *nas);
s32 func_8008DE8C(void);
void func_8008EB3C(void);
void func_8008EB8C(void);
void func_8008FC58(void);
void func_80090048(void);
s32 func_800923F0(s32 *arg0);
void func_800923FC(void);
void func_80092404(void);
void func_80092B7C(void);
void func_80092BA0(void);
void func_80092C54(void);
void func_80092E40(void);
void func_80094058(void);
void func_8009407C(void);
void func_80094120(void);
void func_8009414C(void);
void func_800941BC(void);
void func_800941EC(void);
void func_80094D88(s32 arg0, s32 arg1);
s32 func_80094F30(void);
void func_80094F60(void);
void func_80094FA8(void);
void func_80094FE0(void);
void func_80095A04(s32 arg0);
EXTengine *GetEngineSlot(s32 arg0);
s32 func_800976A8(u8 *arg0, s32 arg1);
void func_80098278(void);

extern s32 D_800ADBC0;
extern s32 D_80099174;
extern s32 D_8009AE78;
extern u8 D_8009F658[];
extern u8 D_800A015C[];
extern EXTengine EnginePool[];
extern HeroInfo Heroes[4];
extern hero_seq **HeroSeqTables[]; /* by HeroInfo.hero_name (1-8): each character's sequences, by id */
extern s32 D_8009BD04;
extern s32 D_800A02C0;
extern s32 D_800A02C4;
extern u8 D_800A0224[];
extern void *D_800A0D2C;
extern u8 D_8009F8A4;
extern s32 D_800A0158;
extern ASTEntry EngineASTTable[];
extern u8 D_8009F644[];
extern s32 D_800A0C08;
extern s32 D_800A0D3C;
extern u8 D_8009F67C[];
extern u8 D_8009F734[];
extern u8 D_8009F46C[];
extern s32 *D_8009F65C;
extern void *D_80098718;
extern s16 D_8009871C;
extern u8 D_800A0BF4[];
extern s32 D_800A0C0C;
extern s32 D_800A0D30;
extern u8 D_8009E498[];
extern s32 D_800AEC5C;
extern s32 D_8009D868;
extern s32 D_8009D86C;
extern s32 D_8009D874;
extern s32 D_8009D878;
extern u8 D_8009F9AC[];
extern s32 D_8009E5A4;
extern s32 D_8009E5A8;
extern s32 D_8009E5B0;
extern s32 D_8009E5B4;
extern s32 D_800AEC58;
extern u8 D_8009E7F4[];
extern u8 D_8009E8B4[];
extern u8 D_8009EB3C[];
extern s32 D_8009EB98;
extern s32 D_8009E5DC;

int func_8007AC00(void);

void SetHeroSeq(HeroInfo *hero, s32 seq);

void SetHeroAction(HeroInfo *hero, s32 action);

void func_8008ECE4(void);

s32 func_8008EF70(void);

void func_8008EF9C(void);

void func_8008F068(void);

s32 func_80095D88(s32 *rec);

#endif /* BASH_ENGINE_H */
