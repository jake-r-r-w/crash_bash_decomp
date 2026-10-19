#ifndef BASH_OVL5D_H
#define BASH_OVL5D_H

/* Functions and data defined in the ovl5d overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5d/ovl5d.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"
#include "bash/engine.h"

struct Ovl5dObj;

/* A board cell of D_800C3BAC (0xC bytes, 8x8). */
typedef struct Ovl5dCell {
    /* 0x00 */ struct Ovl5dObj *obj;
    /* 0x04 */ struct Ovl5dObj *obj2;
    /* 0x08 */ u16 unk8; /* flags */
    /* 0x0A */ u16 unkA; /* object index */
} Ovl5dCell;

extern Ovl5dCell D_800C3BAC[8][8];
extern u8 D_8005A614[];
extern s32 D_8005A5D8;

extern HeroInfo Heroes[4];
extern u8 D_8005A614[];

/* D_8005A614 as seen by func_800BB2BC. */
typedef struct {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 flags;
    /* 0x34 */ u8 pad34[0x6C - 0x34];
    /* 0x6C */ s32 p2_active;
} Ovl5dGameView;
#define OVLG ((Ovl5dGameView *)D_8005A614)

extern HeroInfo Heroes[4];

/* The AI state func_800B4848 writes its chosen target into. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 target;
    /* 0x14 */ u8 pad14[0x20 - 0x14];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ u8 pad22[0x2C - 0x22];
    /* 0x2C */ s32 flags;
} Ovl5dAiState;

void func_800B62EC(void);

void func_800B6F0C(void);

extern u16 D_800C3B88;
extern ScriptInit D_800C3AFC;
extern ScriptInit D_800C3668;
extern EXTengine *D_800C46B0;
extern EXTobject *D_800C46B4;
extern u8 D_8005303C[];
void func_800B8B00(void);

void func_800B7D20(u16 arg0);

/* A scene object in the list at D_80058AD0 + 0x1C (func_800B8314). */
typedef struct Ovl5dObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ u8 pad10[0x5C - 0x10];
    /* 0x5C */ struct Ovl5dObj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ u8 *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} Ovl5dObj;

void PlotBoing(u8 *arg0);

s32 func_800BB1B4(u8 *arg0);

/* Extension 0x20000002: one entry every 2 bytes, one per subengine. */
typedef struct SnowGlint {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ u16 on[10];
    /* 0x20 */ u16 timer[10];
} SnowGlint;

void func_800BB360(void);

void func_800B3A38(void);
void func_800B3DD8(void);
void func_800B432C(void);
s32 func_800BAC5C(EXTengine *eng);

void func_800BB368(void);

extern u8 D_80052F5C[];
extern s16 D_800C3B08[][3];
extern u16 D_8009D754;

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl5d_800bb64c.h"
#include "bash/ovl5d_800bff1c.h"

void func_800B4848(s32 arg0, s32 arg1, s32 arg2, Ovl5dAiState *ai, s32 prev);

void func_800B8314(void);

void func_800B8578(void);

void PlotSnowGlint(NAS *nas);

void InitBash8way(NAS *nas);

void PlotMissileExplode(NAS *nas);

void InitMissileExplode(NAS *nas);

void func_800BA8A4(s32 x, s32 y, s32 z, s32 arg3, s32 arg4);

void InitBoing(NAS *nas);

void InitBashPenguin(NAS *nas);

void InitSnowGlint(NAS *nas);

void func_800BAC10(void);

u16 func_800BAD0C(s32 arg0, s32 arg1);

void func_800BAD70(void);

void func_800BAE08(s32 arg0);

void func_800BAE98(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void func_800BAF68(void);

void func_800BAFB4(void);

s32 func_800BB100(s32 arg0);

void func_800BB2BC(void);

void func_800BB370(void);

#endif /* BASH_OVL5D_H */
