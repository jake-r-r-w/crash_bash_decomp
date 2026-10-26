#ifndef BASH_BONK_OVL_H
#define BASH_BONK_OVL_H

/* Functions and data defined in the bonk overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/bonk/bonk.c */

#include "common.h"
#include "bash/nas.h"

typedef struct BonkPad {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ EXTengine *eng;
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ u16 mode;
} BonkPad;

typedef struct BonkTgt2 {
    /* 0x0 */ s32 kind;
    /* 0x4 */ BonkPad *obj;
} BonkTgt2;

extern s32 D_800B9E30[];

typedef struct BonkObj {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ BonkPad *pad;
} BonkObj;

extern s32 D_800B9E40[];
void func_800B6CD8(s32 arg0, s32 arg1);

s32 func_800B62C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void PlotBonkFlag(NAS *nas);
void func_800B6560(void);
void func_800B67C4(void);
void func_800B6868(s32 arg0);
void func_800B688C(void);
void func_800B68BC(void);
void func_800B69A0(void);
void func_800B6A1C(void);
void func_800B6AF4(void);
void func_800B6AFC(void);

extern s32 D_800B9EEC;
typedef struct { s32 a, b, c; } BonkEnt;
extern BonkEnt D_800B9EC8[];
extern s32 D_800B93A0[];
extern s32 D_800B9E20[];
extern s32 D_8005A624;
/* PlotBonkCrate's puff of smoke at pos (type 0x1605, flags 0x11). */
static inline void Bonk_SpawnPuff(s32 *pos, s32 mode, u8 *p) {
    NAS *obj;
    s32 *ext;

    obj = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (obj != NULL) {
        ext = FindExtension(obj, 0x10000000);
        if (ext != NULL) {
            ext[3] = mode;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)obj->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            obj->InitDelete(obj);
        }
    }
}
extern s32 D_800B9F08;
extern u16 D_800B9580[];
extern ScriptInit D_800B9CE4;
extern u8 D_80053C0C[];
extern u8 D_80053C7C[];
extern u8 D_80052F5C[];
extern s32 D_8005A680[];
extern s32 PanelCtrl;
extern void *D_8009D8FC;
extern void *D_8009D900;
extern void *D_8009D904;
extern u8 D_800B929C[];
extern u8 D_800B92B0[];
extern u8 D_800B92C4[];
extern u8 D_800B92D0[];
extern u8 D_800B92E8[];
extern s16 D_8009D87C[][3];

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/bonk_800b6efc.h"

void func_800B33A4();

typedef struct BonkEntry {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} BonkEntry;
extern s32 D_800B9634;
extern s32 D_800B95A8;
extern s32 D_800B9E0C;
extern BonkEntry D_800B9E50[10];
extern s32 D_800B9EF0;
extern s32 D_800B9EF4;
extern s32 D_800B9EF8;
extern s32 D_800B9EFC;
extern s32 D_800B9F00;
extern s32 D_800B9F04;
extern s32 D_800B9300[];
extern s32 D_800B95C4[];
extern s32 D_800B95E0[];

int func_800B61B0(int a0, int *out);
/* D_8009D6E8 (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct BonkHooks {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8 pad8[0x18];
    /* 0x20 */ void *unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ void *unk28;
    /* 0x2C */ void *unk2C;
    /* 0x30 */ u8 pad30[8];
    /* 0x38 */ void *unk38;
    /* 0x3C */ void *unk3C;
    /* 0x40 */ u8 pad40[4];
    /* 0x44 */ void *unk44;
    /* 0x48 */ void *unk48;
    /* 0x4C */ u8 pad4C[8];
    /* 0x54 */ void *unk54;
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ void *unk5C;
    /* 0x60 */ u8 pad60[4];
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
} BonkHooks;
extern BonkHooks D_8009D6E8;
extern s16 D_8009E0D8;
extern s16 D_8009E0E4;
extern s16 D_8009E0E6;
extern u8 D_8008AB48[];
extern u8 D_800B9DF8[];
extern u8 D_8008AB04[];
void func_800B70C4(s32 arg0);
s32 func_800B9054(EXTengine *eng);
void func_800B717C(s32 arg0);
s32 func_800B7B48(s32 arg0);
s32 func_800B8E64(s32 arg0);
void func_800B6E1C(s32 player);
extern u16 D_800B9594[];
void func_800B5BA0(s32 arg0);
void func_800B67F4(void);

void func_800B6238(s32 arg0, s32 arg1);
void func_800B4BC8(s32 arg0, u16 arg1);
void func_800B5050(s32 arg0, u16 arg1);
void func_800B54D8(s32 arg0, u16 arg1);
void func_800B5820(s32 arg0, u16 arg1);

/* D_8005A614 as seen by func_800B5820 (the final ranking). */
typedef struct BonkRankView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32[0x48 - 0x32];
    /* 0x48 */ s16 team[2];
    /* 0x4C */ u8 unk4C[0x60 - 0x4C];
    /* 0x60 */ u8 winner[2];
    /* 0x62 */ u8 unk62[0x6C - 0x62];
    /* 0x6C */ s32 teams;
    /* 0x70 */ u8 unk70[0xA7 - 0x70];
    /* 0xA7 */ u8 order[4];
} BonkRankView;
#define RANK66V ((BonkRankView *)D_8005A614)

/* func_800B4454: keep the closest target found so far (both search loops). */
#define BONK_SET_CLOSEST(best, found, d, p) \
    do {                                     \
        (best) = (d);                        \
        (found) = (p);                       \
    } while (0)

void PlotBonkShockwave(NAS *nas);

void func_800B6AAC(void);

/* D_80051640 as func_800B69D0 sets it up. */
typedef struct BonkCam {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x54 - 0x1C];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
} BonkCam;

void func_800B347C(void);

void func_800B34F8(void);

s32 func_800B3D28(s32 *arg0, s32 *arg1);

void InitBonkCrate(NAS *nas);

s32 func_800B60FC(s32 arg0);

void InitBonkFlag(NAS *nas);

void InitBonkShockwave(NAS *nas);

void func_800B64B4(s32 arg0, s32 arg1, BonkObj *obj);

void func_800B6568(void);

void InitBonkStool(NAS *nas);

void func_800B66C8(void);

void func_800B6774(void);

void func_800B68EC(void);

void func_800B69D0(void);

void func_800B6B04(s32 arg0, EXTengine *p, s32 arg2);

extern u16 Clock_flags;

extern u16 D_8009E080[][4];
void func_80084E04(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);

void PlotBonkCrate(NAS *nas);

s32 func_800B4454(s32 arg0, s32 *arg1, s32 arg2);

/* Target found by func_800B61B0 (func_800B5BA0 reads it). */
typedef struct BonkHit {
    /* 0x00 */ s32 kind;
    /* 0x04 */ NAS *nas;
} BonkHit;
extern u32 D_80051244[]; /* pad flags by angle octant */

/* Squared ground distance from (x0,z0) to (x1,z1), as func_800B5BA0 uses it. */
static inline s32 bonk_sq(s32 x0, s32 z0, s32 x1, s32 z1)
{
    s32 dx = x1 - x0;
    s32 dz = z1 - z0;

    return dx * dx + dz * dz;
}

void PlotBonkStool(NAS *nas);

#endif /* BASH_BONK_OVL_H */
