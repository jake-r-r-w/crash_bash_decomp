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
extern void (*D_8009D714)(void);

extern HeroInfo Heroes[4];
extern u8 D_8005A614[];

/* D_8005A614 as seen by func_800BB2BC. */
typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ u16 pause;
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
extern u16 D_800C3B8C[];
extern u16 D_8009E46C[];
extern s32 D_8009D758;
Ovl5dPair *func_800B8040(u8 *arg0);

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
    /* 0x48 */ s16 c[16];
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
s32 func_800B6DA4(s32 arg0);
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

u8 *func_800BBA14(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern u8 *D_800C478C;

void func_800BB370(void);

void func_800BB3D0(void);

void func_800BB4AC(Ovl5dTumbler *arg0, Ovl5dTumbleMove *arg1, s32 arg2);

typedef struct Ovl5dGridCell {
    s32 x;
    s32 z;
} Ovl5dGridCell;

extern Ovl5dGridCell D_800C3EAC[16][16];

void func_800B44B0(s32 arg0);

s32 func_800B46B8(s32 arg0);

s32 func_800B6138(s32 arg0);

s32 func_800B6FE0(s32 arg0);

void func_800B710C(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4, u16 arg5, u16 arg6, u16 arg7, u16 arg8);

void func_800B7E74(void);

void func_800B94C4(s32 arg0, u16 arg1);

void PlotBash8way(NAS *nas);

void InitBashHomer(NAS *nas);

u8 *func_800BAFDC(s32 x, s32 unused, s32 z, u16 flag);

/* The object extension InitTumbler adds (AddObject). */
typedef struct TumblerObj {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[2];
    /* 0x1A */ s16 unk1A;
} TumblerObj;

s32 func_800BF404(EXTengine *eng, s32 arg1, s32 arg2, s32 arg3);

/* Spawns the 0x1605 puff at v and returns it (InitTumbler). */
static inline NAS *Ovl5dSpawnPuff(SLONG_XYZ *v) {
    NAS *nas;
    s32 *ext;

    nas = SpawnNASById(v->x, v->y, v->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[6] = 0x100;
            ext[3] = 0;
            ext[8] = 9;
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}

void func_800B8C9C(EXTengine *self);

void InitTumbler(NAS *nas);

void PlotTumbler(NAS *nas);

void PlotBashHomer(NAS *nas);

void PlotBashShox(NAS *nas);

/* PlotBashPenguin's NAS.FUX. */
typedef struct PenguinFux {
    /* 0x00 */ u8 pad0[0x1A];
    /* 0x1A */ u16 state;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ u8 pad20[2];
    /* 0x22 */ s16 unk22;
} PenguinFux;

extern u16 D_800C3B38;
extern u16 D_800C3B3C;

void func_800B8C9C(EXTengine *eng);

void PlotBashPenguin(NAS *nas);

/* func_800B3A38: the overlay's init hook. */
extern EXTengine D_800C46EC;
extern void (*D_8009D70C)(s32);
void func_800B4AB0();
void func_800BEF00();
NAS *func_800C1848();
s32 func_800C1A30(s32 arg0);
void func_800C284C();
extern u8 D_800C3004[];
extern u8 D_800C3018[];
extern u8 D_800C302C[];
extern u8 D_800C3038[];
extern u8 D_800C3050[];
extern u8 D_800C3064[];
extern u8 D_800C3078[];
extern u8 D_800C308C[];
extern u8 D_800C30A0[];
extern u8 D_800C30AC[];
extern u8 D_800C30C4[];
extern u8 D_800C30D8[];
extern u8 D_800C30EC[];
extern u8 D_800C3100[];
extern u8 D_800C3114[];
extern u8 D_800C3120[];
extern u8 D_800C3138[];
extern u8 D_800C314C[];
extern u8 D_800C3160[];
extern u8 D_800C3174[];
extern u8 D_800C3188[];
extern u8 D_800C3194[];
extern u8 D_800C31AC[];
extern u8 D_800C31C0[];

/* func_800B3DD8 */
extern u16 D_800C3B32;
extern u16 D_800C3B34;
extern u16 D_800C478A;
extern u16 D_800C3B3A;
extern s16 D_800C3B3E;
extern u16 D_800C4788;
extern s32 D_800C46AC;
extern u8 *D_800C3B9C[4];
extern u16 D_800C46C0[];

/* The object func_800BBA14 returns, as func_800B7358 sees it. */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ u16 unk24;
} Ovl5dEdgeSub;
typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ Ovl5dEdgeSub *unk58;
} Ovl5dEdgeObj;

/* func_800B7358: from (x, z), walk the board along x (dx) or z (dz),
 * 1, -1, 2, -2, ... cells, to the first empty cell, and spawn there. */
static inline u8 *Ovl5dSpawnEdge(s32 x, s32 z, s32 dx, s32 dz) {
    s32 col;
    s32 row;
    s32 d;
    u8 *p;

    d = 1;
    col = (x >> 8) + 10;
    row = 9 - (z >> 8);
    do {
        if (D_8009D90C[row * 20 + col].unk0 != 0) {
            col = (x >> 8) + 10;
            row = 9 - (z >> 8);
            col += d * dx;
            row += d * dz;
            d = ~d + 1;
            if (d > 0) {
                d++;
            }
            if (d >= 9) {
                d = 0;
            }
        } else {
            d = 0;
        }
    } while (d != 0);
    col -= 10;
    col <<= 8;
    row = 19 - row;
    row -= 10;
    row <<= 8;
    if (col < -0x700 || col > 0x700) {
        col = x;
    }
    if (row < -0x700 || row > 0x700) {
        row = z;
    }
    p = func_800BBA14(col, 0, row, 2, -1);
    if (p != NULL) {
        func_80026B70(&D_800C478C, p);
    }
    return p;
}
extern u8 D_800C395C[];
extern u16 D_800C3984[];
extern u8 D_800C39C0[];
extern u8 D_800C39D8[];
extern u16 *D_8009D6F4;
void func_8007EB1C(s32 arg0, s32 arg1, s32 arg2);
void func_8007B7FC(u32 a, u32 b);
void func_80095258(u8 *arg0);

#endif /* BASH_OVL5D_H */
