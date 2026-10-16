#ifndef BASH_8009317C_H
#define BASH_8009317C_H

/* Functions and data defined in src/overlays/engine/8009317c.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/8009317c.c */

#include "common.h"
#include "bash/engine.h"

extern u16 D_8009F994[];

typedef struct Tmpl1C {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fC;
    u16 f10;
    u16 pad12;
    u16 f14;
    u16 pad16;
    u16 f18;
    u16 pad1A;
} Tmpl1C;

extern Tmpl1C D_800A0D44[];
extern u8 D_8001C690[];
extern u8 *D_800A001C[];

void func_80094058(void);

void func_8009407C(void);

void func_80094120(void);

void func_8009414C(void);

void func_800941BC(void);

void func_800941EC(void);

void func_80094274(void);

void func_80094D88(s32 arg0, s32 arg1);

s32 func_80094F30(void);

void func_80094F60(void);

void func_80094FA8(void);

void func_80094FE0(void);

void func_80095A04(s32 arg0);

void func_800952F8(subengine *sub, s32 *recs);

void func_80095BEC(s32 arg0, s32 *recs);

EXTengine *GetEngineSlot(s32 arg0);

void func_80095258(u8 *arg0);

s32 func_8009317C(void);
extern s32 *D_8009F954[];
extern s32 D_8009F8E4[];
extern u8 *D_80098598;

void func_80093588(void);

void func_800940B0(s32 arg0, s32 arg1);

void func_8009421C(void);

s32 func_80095D88(s32 *rec);

void func_800936C4(void);

void func_800945C0(EngineStartArgs *arg0);

void func_8009342C(void);

void func_80095020(void);

void func_800943B8(void);

void func_8009440C(void);

void func_80095E5C(u8 *arg0, u8 *arg1, s32 arg2);

s32 func_80095F48(s32 *p, s32 a1, s32 a2, s32 a3, s32 a4);

void func_80094DD0(void);

void func_80093270(void);

void func_8009400C(void);

void func_80094364(void);

void func_80094EB8(void);

s32 func_800950FC(void);

void func_80095160(EngineStartArgs *arg0);

void func_80095DBC(void);

void func_80094754(void);

void func_80094860(void);

s32 func_800951D0(s32 arg0);

s32 func_80095F88(u8 *p, s16 a1, s32 a2, s16 a3, s32 a4, s32 idx);

typedef struct MarkSrcEnt {
    /* 0x00 */ s16 val;
    /* 0x02 */ u8 pad2[0x32];
} MarkSrcEnt;
typedef struct MarkSrc {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ s32 count;
    /* 0x58 */ u8 pad58[0xE];
    /* 0x66 */ MarkSrcEnt ent[1];
} MarkSrc;
typedef struct MarkObj {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ MarkSrc *src;
} MarkObj;
typedef struct MarkCtx {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ MarkObj *obj;
    /* 0x8 */ s32 unk8;
    /* 0xC */ u8 unkC;
} MarkCtx;
extern Blk32 D_800A0BA0;
extern CamVec3 D_800A0BC0;
extern u8 D_800A0BF3;
extern u8 *D_800A0D34;

void func_80094A28(void);

void func_80094B38(void);

void func_80095A60(s32 arg0, s32 arg1);

#endif /* BASH_8009317C_H */
