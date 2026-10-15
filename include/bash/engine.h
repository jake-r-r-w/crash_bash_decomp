#ifndef BASH_ENGINE_H
#define BASH_ENGINE_H

/* Functions and data defined in the engine overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/engine.c */

#include "common.h"
#include "bash/nas.h"

extern s32 D_80051658;
extern s32 D_800AD2F8[];
extern s32 D_800AD320;

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
extern s16 D_8009AE70;
extern s32 D_8009AE3C;
extern u16 D_8009AE74;
extern u8 D_8009F658[];
extern u8 D_8009FA14[];
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
extern u8 D_8009F8BC[];
extern u8 D_800A016C[];
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
extern s32 D_800A0C70;
extern s32 D_800A0D1C;
extern s32 D_800A0D20[]; /* arrays: their stores must alias struct loads (gcc 2.7 sched) */
extern s32 D_800A0D24[];
extern s32 D_800A0D28;
extern s32 D_800A0D38;
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
typedef struct { u16 a, b, c, flags; } EngEntry;
typedef struct { s32 unk0; EngEntry *entries; s32 unk8, unkC, unk10; } EngTable;
typedef struct { u16 unk0; s32 unk4; s32 unk8; } EngState;
extern EngState D_8009EE9C;
extern s32 D_8009E5DC;
extern u8 D_8005A676;
extern s32 D_8009EDD4[][5];
extern u16 D_800986FC[];

void func_8007A650(s32 arg0);

typedef struct {
    u8 unk0;
    u8 unk1;
} D_8009D90C_t;
extern D_8009D90C_t D_8009D90C[400];

int func_8007AC00(void);

void SetHeroSeq(HeroInfo *hero, s32 seq);

void SetHeroAction(HeroInfo *hero, s32 action);

void func_80086FA0(void);

void func_8008ECE4(void);

s32 func_8008EF70(void);

void func_8008EF9C(void);

void func_8008F068(void);

void func_80092F9C(void);

void func_800940B0(s32 arg0, s32 arg1);

void func_8009421C(void);

s32 func_80095D88(s32 *rec);

void func_800972BC(s32 arg0, s32 arg1);
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} D_800AD2EC_t;

extern s32 D_80051658;
extern D_800AD2EC_t D_800AD2EC;
extern s32 D_800AD31C;

void func_8007E7F4(void);

u8 *func_8008D24C(u8 *arg0, s32 *arg1);

void func_8008EC94(void);

s32 func_80092560(u8 *arg0);

void func_80097334(s32 arg0, s32 arg1);

/* PadState (D_80051338) seen by func_8007EE2C: button flags at 0x44/0x48. */
typedef struct PadButtons {
    /* 0x00 */ u8 unk0[0x44];
    /* 0x44 */ s32 held;
    /* 0x48 */ s32 pressed;
    /* 0x4C */ u8 unk4C[0x58 - 0x4C];
} PadButtons;

extern s32 D_8009AEEC;
extern s32 D_8009AEF0;
void func_8007A614(s32 arg0);

s32 func_8007EE2C(s32 pad);

extern void (*D_8009D73C)();

void func_80086F78(void);

void func_8008A8F0(s32 arg0);

u8 *func_8008D284(u8 *arg0, u8 *arg1);

void func_8009029C(void);
/* func_800945C0's argument. */
typedef struct {
    void *unk0; /* -> D_800A0D2C */
    s32 unk4;   /* -> D_800A0D1C */
    s32 unk8;   /* frame rate scale */
} EngineStartArgs;
void func_800945C0(EngineStartArgs *arg0);

#endif /* BASH_ENGINE_H */
