#ifndef BASH_OVL5F_H
#define BASH_OVL5F_H

/* Functions and data defined in the ovl5f overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5f/ovl5f.c */

#include "common.h"
#include "bash/nas.h"

extern u8 D_80053774[];
extern u8 D_800C9EE8[];
extern u8 D_800C9EFC[];
extern u8 D_800C9ED0[];
extern u8 D_800C9EB0[];
extern u8 D_800C9E9C[];
extern u8 D_800C9EC4[];

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

/* Extension 0x10000000 of the NAS 0x1605 object func_800C5620 spawns. */
typedef struct Ovl5fExt1605 {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x20 - 0x1C];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x28 - 0x24];
    /* 0x28 */ s32 unk28;
} Ovl5fExt1605;
void func_800C4D04(s32 hero, s32 last_mode);
s32 func_8008502C();
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

typedef struct {
    s32 x;
    s32 z;  /* negative: unused */
} Ovl5fPoint;

extern Ovl5fPoint D_800CA9E4[];
extern s32 D_800CB0F0;
/* PlotPillarControl's waves: delay, then {x, z, angle} until x < 0. */
extern s32 *D_800CA8C4[];
extern s32 D_800CB204;      /* next wave */
void func_800B6ED8(s32 player);
extern s32 D_800CB260;
extern NAS *D_800CB264[8];    /* objects func_800C8598 adds and func_800C8560 removes; func_800C84D8 tests their distance */
extern NAS *D_800CB0C0[8];







void func_800B56A0(void);

void func_800B5714(void);

void func_800C5620(s32 arg0);

void func_800C69C4(s32 arg0, s32 *arg1);

s32 func_800C6B9C(s32 id, s32 x, s32 z, s32 angle);

void func_800C7C70(s32 arg0);

void func_800C7D24(HeroInfo *hero);

void func_800C7DB8(s32 arg0);

void func_800C7E68(HeroInfo *hero, Ovl5fActObj *obj, s32 arg2);

void func_800C7EB0(HeroInfo *hero, Ovl5fActState *state, s32 arg2);

void func_800C7EF8(HeroInfo *hero);

void func_800C8090(s32 player, EXTengine *pEngine, s32 ReqdSFX);

void func_800C81C8(s32 x, s32 z, s32 range, s32 arg3);

void func_800C8450(s32 x, s32 z, s32 dist);

NAS *func_800C84D8(s32 x, s32 z, s32 dist);

void func_800C8560(NAS *nas);

void func_800C8598(NAS *nas);

void func_800C85D0(void);

s32 func_800C8870(s32 arg0, s32 arg1);

void func_800C88A8(NAS *nas);

void func_800C8BA0(void);

extern u8 D_800C9F5C[];
extern u8 D_800C9F70[];
extern u8 D_800C9F44[];
extern u8 D_800C9F24[];
extern u8 D_800C9F10[];
extern u8 D_800C9F38[];

void func_800B5788(void);
extern u8 D_800C9FD0[];
extern u8 D_800C9FE4[];
extern u8 D_800C9FB8[];
extern u8 D_800C9F98[];
extern u8 D_800C9F84[];
extern u8 D_800C9FAC[];

void func_800BADB8(s32 arg0);
typedef struct OvlClampPos {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 pad14;
    /* 0x18 */ s32 z;
} OvlClampPos;

extern s32 D_800CB100;
extern s32 D_800CB104;

void func_800B8EC4(s32 player);

void func_800C68C8(s32 arg0);

s32 func_800C8294(OvlClampPos *p);

#endif /* BASH_OVL5F_H */
