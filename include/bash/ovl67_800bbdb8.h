#ifndef BASH_OVL67_800BBDB8_H
#define BASH_OVL67_800BBDB8_H

/* Functions and data defined in src/overlays/ovl67/800bbdb8.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl67/800bbdb8.c */

#include "common.h"
#include "bash/ovl67.h"

typedef struct MineSrc {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ s32 unk18;
} MineSrc;

typedef struct MineSFX {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ MineSrc *src;
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ s16 unk5C;
    /* 0x5E */ u8 pad5E[2];
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
} MineSFX;
extern s32 D_800C4480;
extern s32 D_800C4488;
extern s32 D_800C448C;
extern s32 D_800C44FC;
extern s32 D_800C4500;
extern s32 D_800C4504;
extern s32 D_800C4508;
extern ScriptInit D_800C44F0;

void func_800BC6B8(void);

void InitMineSFX(MineSFX *arg0);

void func_800C2904(s32 *arg0, s32 flag, s32 *out);

s32 func_800C2D10(s32 arg0);

void func_800C2D80(s32 arg0);

void func_800C2E58(void);

s32 func_800C2F70(void);

void InitDummyCamera(NAS *nas);

void func_800C3184(void);

void func_800C3234(void);

void func_800C3370(void);

void func_800C3440(void);

void func_800C36D4(void);

void func_800C36DC(void);

void PlotNitroSmokeBoost(NAS *nas);

void func_800C318C(s32 arg0);

void InitNitroSmokeBoost(NAS *nas);
extern s32 D_800C447C;

void func_800C2BB0(void);

void func_800C2EF8(s32 arg0);

#endif /* BASH_OVL67_800BBDB8_H */
