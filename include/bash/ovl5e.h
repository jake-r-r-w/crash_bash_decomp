#ifndef BASH_OVL5E_H
#define BASH_OVL5E_H

/* Functions and data defined in the ovl5e overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5e/ovl5e.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/engine.h"

typedef struct Ovl5eSpot {
    s32 x;
    s32 y;
    s32 z;
} Ovl5eSpot;

extern Ovl5eSpot D_800BD500[];
extern s16 D_8009D88E;

typedef struct Ovl5ePos {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 z;
} Ovl5ePos;

typedef struct PapuEntry {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
    /* 0xC */ s16 unkC;
    /* 0xE */ s16 unkE;
} PapuEntry;

extern s16 *D_800BD6F0[];

extern PapuEntry D_800BE648[20];

/* The object extension (AddObject) func_800B8508 fills. */
typedef struct Ovl5eObj {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u8 pad16[0x1A - 0x16];
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u8 pad1E[0x22 - 0x1E];
    /* 0x22 */ s16 unk22;
} Ovl5eObj;
extern s32 D_8009E464;
extern u16 D_8009E46C[];
typedef struct Ovl5eTile {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padC[0x74 - 0xC];
    u16 unk74;
} Ovl5eTile;

typedef struct Ovl5eCell {
    Ovl5eTile *tile;
    s32 pad4;
    s16 pad8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 unk10;
    s16 unk12;
} Ovl5eCell;

extern Ovl5eCell D_800BD948[8][8];

/* A scene object in the list at D_80058AD0 + 0x1C (func_800B6D14). */
typedef struct Ovl5eSceneObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 x;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 padA;
    /* 0x0C */ s32 z;
    /* 0x10 */ u8 pad10[0x24 - 0x10];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x50 - 0x28];
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad54[0x5C - 0x54];
    /* 0x5C */ struct Ovl5eSceneObj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ u8 *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} Ovl5eSceneObj;
extern Ovl5eSceneObj *D_800BE824;
extern u16 D_800BE838[40];
extern u16 D_800BE888;
extern s16 D_800BE88A;
extern s16 D_800BE88C;
typedef struct Ovl5eGridPos {
    s32 x;
    s32 y;
} Ovl5eGridPos;
extern Ovl5eGridPos D_800BDE48[16][16];
extern u8 D_8005A6C3;
extern s16 D_800BC95C[];
extern u16 D_800BD8CC;
extern u16 D_800BD8CE;
extern u16 D_800BD8D0;
extern u16 D_800BD8DC;
extern u16 D_800BD8E8;
typedef struct {
    s32 pad0[4];
    s32 x;
    s32 pad14;
    s32 y;
    s16 pad1C;
    s16 h;
} T_B8080;

typedef struct Slot40 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 pad4[7];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34[3];
} Slot40;

extern Slot40 D_800BD70C[6];

typedef struct B5E44Flag {
    /* 0x00 */ s32 pad0[6];
    /* 0x18 */ s16 pad18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u16 flags;
} B5E44Flag;

typedef struct B5E44Obj {
    /* 0x00 */ s32 pad0[4];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 pad14;
    /* 0x18 */ s32 z;
    /* 0x1C */ s32 pad1C[10];
    /* 0x44 */ s32 pad44[5];
    /* 0x58 */ B5E44Flag *flag;
} B5E44Obj;

typedef struct B5E44Pos {
    /* 0x00 */ s32 pad0[4];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 pad14;
    /* 0x18 */ s32 z;
} B5E44Pos;

typedef struct B5E44Dyn {
    /* 0x00 */ s32 pad0[5];
    /* 0x14 */ s16 pad14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s32 pad18;
    /* 0x1C */ s32 pad1C;
    /* 0x20 */ s16 pad20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 pad24[3];
    /* 0x2A */ s16 unk2A;
} B5E44Dyn;

typedef struct B5E44Owner {
    /* 0x00 */ s32 pad0[21];
    /* 0x54 */ B5E44Pos *pos;
} B5E44Owner;

typedef struct B5E44Slot {
    /* 0x00 */ B5E44Obj *obj;
    /* 0x04 */ B5E44Pos *pos;
    /* 0x08 */ s32 pad8[3];
    /* 0x14 */ B5E44Dyn *dyn;
    /* 0x18 */ s32 pad18[10];
} B5E44Slot;

extern u16 D_800BD700[];

typedef struct Ovl5eKey {
    /* 0x00 */ u16 a;
    /* 0x02 */ u16 b;
    /* 0x04 */ u16 pad4;
    /* 0x06 */ u16 c;
    /* 0x08 */ u16 pad8[2];
    /* 0x0C */ u16 d;
    /* 0x0E */ u16 e;
    /* 0x10 */ u16 f;
    /* 0x12 */ u16 g;
} Ovl5eKey;

extern Ovl5eKey *D_800BD008[];
extern Ovl5eKey *D_800BE82C;
extern s16 D_800BE894;

typedef struct PapuBossState {
    /* 0x00 */ NAS *nas;
    /* 0x04 */ EXTengine *eng;
    /* 0x08 */ subengine *sub;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ ScriptCursor *cursor;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} PapuBossState;

extern PapuBossState D_800BD88C;
extern u8 D_800533F4[];
extern ScriptInit D_800BD3AC;
extern s16 D_800BE88E;
extern s16 D_800BE890;
extern u16 D_800BD130[];
extern u16 D_800BD2F4[];
extern s16 D_800BC91C[];
extern s16 D_800BE82A;
extern u16 D_800BE834;
extern s32 D_800BD708;
extern u16 D_800BD8B4;
extern s32 D_800BD8D4;
extern s32 D_800BD8D8;

void func_800B7600(s32 arg0);

typedef struct Ovl5eMsg {
    s32 unk0;
    s32 unk4;
} Ovl5eMsg;

Ovl5eMsg *func_800B3E80(char *arg0);
NAS *func_800B82DC(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_800B7CBC(u16 arg0);
typedef struct Ovl5eStep {
    s32 a;
    s32 b;
    s32 c;
    s16 d;
    s16 pad;
} Ovl5eStep;

extern Ovl5eStep *D_800BD118[];
extern u8 D_8005342C[];
extern ScriptInit D_800BD4F4;
NAS *func_800B4EE8(s32 a, s32 b, s32 c, u16 d);

s32 func_800B7670(u8 *arg0);

void func_800B7778(void);

void func_800B790C(s32 arg0);

s32 func_800B794C(void);

void func_800B35AC(void);
void func_800B36D4(void);
void func_800B7D94(void);

void func_800B7D9C(void);

void func_800B7DA4(void);

extern s32 D_8009E468;
extern u8 D_8009E3B4[];
extern u8 D_8009E3DC[];

/* Index of the ASTTable entry with this id, or 0. */
static inline s32 Ovl5eASTIndex(s32 id) {
    s32 i;

    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == id) {
            return i;
        }
    }
    return 0;
}

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl5e_800b87a0.h"

void func_800B42C0(void);

s32 func_800B7558(EXTengine *eng);

void func_800B7894(u16 arg0);

void InitPapuColumn(NAS *nas);

void func_800B7B10(void);

void func_800B7B4C(void);

void func_800B7DE4(void);

s32 func_800B8080(T_B8080 *p);

extern s32 D_8005B2EC;
extern s16 D_8009D6DC[];

void func_800B4154(u16 arg0);

void func_800B4204(void);

s32 func_800B744C(s32 arg0);

s32 func_800B77D8(void);

void PlotPapuColumn(NAS *nas);

void func_800B7E38(void);
extern u16 D_8009E25C[13];
typedef struct {
    u8 pad0[0x14];
    u16 type;
} T_B80B0Fux;

typedef struct {
    u8 pad0[0xC];
    NAS *owner;
    u8 pad10[4];
    u16 type;
    u8 pad16[4];
    u16 status;
} T_B4B9CFux;
extern u16 D_8009E46C[];
extern u16 D_800BD8E0;
extern u16 D_800BD8E4;
extern char D_800BD8E2[];
extern u8 D_800BD908[];
extern u16 D_800BD8F0[4];

void func_800B7BE4(void);

void func_800B8508(NAS *arg0);

NAS *func_800BB3F0(s32 player, s32 mask, s32 unused, s32 cone);
NAS *func_800BB69C(s32 idx, s32 arg1);
s32 func_800BB86C(s32 arg0);
s32 func_800BC274(s32 idx);
extern u8 D_800BC89C[];
extern u8 D_800BC8DC[];

s32 func_800B4D70(Ovl5ePos *arg0);
extern u8 D_80051244[];

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 pad14;
    s32 z;
    s16 pad1C;
    s16 ang;
} T_B7F14_A;

typedef struct {
    u8 pad0[0xD];
    u8 dir;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 pad14;
    s16 unk16;
    u8 pad18[0x12];
    s16 unk2A;
} T_B7F14_B;

void func_800B3B08(void);

void func_800B3D18(void);

void InitPapuBoss(NAS *nas);

NAS *func_800B4B9C(s32 arg0);

void func_800B5E44(B5E44Owner *arg0, s32 arg1, s32 arg2);

void *func_800B6008(s32 arg0, s32 arg1);

s32 func_800B61A0(EXTengine *eng);

s32 func_800B6334(EXTengine *eng, u32 flag);

NAS *func_800B64AC(EXTengine *eng, u32 mask, s32 unused, s32 add);

NAS *func_800B668C(EXTengine *eng, s32 mask);

void func_800B6D14(void);

void func_800B6FF8(void);

void func_800B7F14(T_B7F14_A *a, T_B7F14_B *b);

void func_800B80B0(void);

extern s16 D_800BD8DE;
extern s32 D_800BD708;
extern s32 D_800BD8D4;
extern s32 D_800BD8D8;
extern EXTengine D_800BE788;
extern s16 D_800BE828;
extern s16 D_800BE82A;
extern u16 D_800BE830;
extern u16 D_800BE832;
void func_800B38FC(u16 a0, s32 a1, s32 a2, s32 a3, u16 a4, u16 a5, u16 a6,
                   u16 a7, u16 a8);

s32 func_800B6860(u32 idx);

extern _sfx_handler *D_800BE81C;
extern u16 D_800BD1B0[];

void PlotPapuBoss(NAS *nas);

void func_800B7134(void);
/* PlotPapuMinion's FUX extension. */
typedef struct MinionFux {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ NAS *target;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 pad18;
    /* 0x1A */ u16 state;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
} MinionFux;

/* PlotPapuMinion's 0x800 extension (movement). */
typedef struct MinionMove {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 speed;
    /* 0x10 */ u8 pad10[0x16 - 0x10];
    /* 0x16 */ s16 angle;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 pad20;
    /* 0x22 */ s16 push;
    /* 0x24 */ u8 pad24[0x2A - 0x24];
    /* 0x2A */ s16 push_angle;
} MinionMove;

extern s32 D_8005A680[];
extern u16 D_8009DC38[];
extern u16 D_8009DC50[];

void func_800B38FC(u16 arg0, s32 a1, s32 a2, s32 a3, u16 n4, u16 n5, u16 n6, u16 n7, u16 n8);

void PlotPapuMinion(NAS *nas);

#endif /* BASH_OVL5E_H */
