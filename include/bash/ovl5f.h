#ifndef BASH_OVL5F_H
#define BASH_OVL5F_H

/* Functions and data defined in the ovl5f overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5f/ovl5f.c */

#include "common.h"
#include "bash/nas.h"

void func_800C0C68(NAS *nas, s32 id);
void func_800C0F24(NAS *nas, s32 id);

void func_800C68A8(void);
void func_800C69BC(void);
void func_800C7C08(u8 *arg0);
void func_800C7C68(void);
void func_800C7D04(void);
void func_800C81C0(void);
void func_800C831C(void);
void func_800C8324(void);
void func_800C832C(void);
void func_800C83D0(void);
u8 func_800C891C(s32 arg0, s32 arg1);
u8 func_800C8950(s32 arg0, s32 arg1);
void func_800C8984(s32 arg0, s32 arg1);
void func_800C89D8(s32 arg0, s32 arg1);
void func_800C8A40(s32 arg0);

/* Object func_800C7E68/func_800C7EB0 set up alongside a hero action. */
typedef struct Ovl5fActObj {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x50 - 0xC];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl5fActObj;
/* InitFlamerBitTA's 0x38-byte extension (id 0x10000000). */
typedef struct FlamerBitExt {
    /* 0x00 */ EXT ext;
    /* 0x0C */ u8 pad0C[0x1C];
    /* 0x28 */ s32 timer;
    /* 0x2C */ u8 pad2C[0xC];
} FlamerBitExt;

extern u8 D_8005365C[];
/* State block func_800C7E68/func_800C7EB0 set up alongside a hero action. */
typedef struct Ovl5fActState {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC[17];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl5fActState;

extern s32 D_800CA34C[];
extern u8 D_800CACFC[];
extern s32 D_800CAFF4;
extern u8 D_800CACB4[];
extern u8 D_800CAFFC[];
typedef struct {
    u8 pad0[0x74];
    s16 unk74;
} Ovl5fUnk74;

extern Ovl5fUnk74 *D_8009D60C;
extern Ovl5fUnk74 *D_8009D678;
extern s32 *D_800CB24C;
extern s32 *D_800CB250;
extern s32 D_800CB0FC;

/* PlotLaserBitTA's NAS.FUX. */
typedef struct LaserBitFux {
    /* 0x00 */ u8 unk0[0x2C];
    /* 0x2C */ s32 player;
    /* 0x30 */ s32 unk30;
} LaserBitFux;

s32 func_800BAFFC(NAS *nas, s32 arg1, s32 arg2, s32 arg3);

extern NAS *D_800CB264[8];    /* objects func_800C8598 adds and func_800C8560 removes; func_800C84D8 tests their distance */
extern NAS *D_800CB0C0[8];
extern NAS *D_800CB264[8];   /* objects func_800C8598 adds and func_800C8560 removes; func_800C84D8 tests their distance */







void func_800C69C4(s32 arg0, s32 *arg1);

void func_800C7D24(HeroInfo *hero);

void func_800C7E68(HeroInfo *hero, Ovl5fActObj *obj, s32 arg2);

void func_800C7EB0(HeroInfo *hero, Ovl5fActState *state, s32 arg2);

void func_800C7EF8(HeroInfo *hero);

void func_800C81C8(s32 x, s32 z, s32 range, s32 arg3);

void func_800C8450(s32 x, s32 z, s32 dist);

NAS *func_800C84D8(s32 x, s32 z, s32 dist);

void func_800C8560(NAS *nas);

void func_800C8598(NAS *nas);

void func_800C85D0(void);

s32 func_800C8870(s32 arg0, s32 arg1);

void func_800C88A8(NAS *nas);

void func_800C8BA0(void);

#endif /* BASH_OVL5F_H */
