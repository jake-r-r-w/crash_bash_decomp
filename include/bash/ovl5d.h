#ifndef BASH_OVL5D_H
#define BASH_OVL5D_H

/* Functions and data defined in the ovl5d overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5d/ovl5d.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/8008e27c.h"

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
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 pad36[0x6C - 0x36];
    /* 0x6C */ s32 p2_active;
} Ovl5dGameView;
#define OVLG ((Ovl5dGameView *)D_8005A614)

/* Index of the ASTTable entry with this id, or 0. */
static inline s32 Ovl5dASTIndex(s32 id) {
    s32 i;

    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == id) {
            return i;
        }
    }
    return 0;
}

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

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
} Ovl5dPair;

extern u8 D_800C3B44[];
extern u16 D_800C3B84;
extern u8 D_800C3B86[];
extern u16 D_800C3B94[];
extern u16 D_8009E46C[];
extern s32 D_8009D758;
Ovl5dPair *func_800B8040(u8 *arg0);
void func_800BBA14(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

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

typedef struct Ovl5dTumbler {
    /* 0x00 */ u8 pad00[0x10];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 y;
    /* 0x18 */ s32 z;
    /* 0x1C */ u8 pad1C[2];
    /* 0x1E */ s16 angle;
} Ovl5dTumbler;

typedef struct Ovl5dTumbleMove {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ u8 dir;
    /* 0x0D */ u8 pad0D;
    /* 0x0E */ s16 vel;
    /* 0x10 */ s16 acc;
    /* 0x12 */ s16 dec;
    /* 0x14 */ s16 max;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s16 v2;
    /* 0x1A */ s16 acc2;
    /* 0x1C */ s16 dec2;
    /* 0x1E */ s16 max2;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ s16 sum;
    /* 0x32 */ u16 steps;
} Ovl5dTumbleMove;

void PlotBoing(u8 *arg0);

s32 func_800BB1B4(u8 *arg0);

/* Extension 0x20000002: one entry every 2 bytes, one per subengine. */
typedef struct SnowGlint {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ u16 on[10];
    /* 0x20 */ u16 timer[10];
} SnowGlint;

/* Extension 0x20000000 (BashShox): u16 per subengine. */
typedef struct BashShoxExt {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ u16 a[8];
    /* 0x28 */ u16 b[16];
    /* 0x48 */ u16 c[16];
} BashShoxExt;

void func_800BB360(void);

void func_800B3A38(void);
void func_800B3DD8(void);
void func_800B432C(void);
s32 func_800BAC5C(EXTengine *eng);

void func_800BB368(void);

extern u8 D_80052F5C[];
extern s16 D_800C3B08[][3];
extern u16 D_8009D754;
extern s32 D_8009E464;
extern s32 D_8009E468;
extern u8 D_8009E3B4[];
extern u8 D_8009E3DC[];
extern s16 D_800C3B40;
s16 func_800B6DA4(s32 arg0);
void func_800B7358(void);
void func_800BB7E8(void);
void func_8007A91C(void);
void func_80086D88(void);
void func_80086F78(void);
void func_800B8578(void);
void func_800B8B00(void);
void func_800BAD70(void);

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl5d_800bb64c.h"
#include "bash/ovl5d_800bff1c.h"

void func_800B4848(s32 arg0, s32 arg1, s32 arg2, Ovl5dAiState *ai, s32 prev);

void func_800B8314(void);

void func_800B8578(void);

void PlotSnowGlint(NAS *nas);

s32 func_800B95F0(s32 arg0);

void InitBash8way(NAS *nas);

void InitBashShox(NAS *nas);

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

void func_800BB3D0(void);

void func_800BB4AC(Ovl5dTumbler *arg0, Ovl5dTumbleMove *arg1, s32 arg2);

typedef struct Ovl5dGridCell {
    s32 x;
    s32 z;
} Ovl5dGridCell;

extern Ovl5dGridCell D_800C3EAC[16][16];

s32 func_800B6FE0(s32 arg0);

void func_800B7E74(void);

void func_800B94C4(s32 arg0, u16 arg1);

void InitBashHomer(NAS *nas);

#endif /* BASH_OVL5D_H */
