#ifndef BASH_OVL64_H
#define BASH_OVL64_H

/* Functions and data defined in the ovl64 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl64/ovl64.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"

/* A scene object in the list at D_80058AD0 + 0x1C (func_800B3938). */
typedef struct Ovl64Obj {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 rx;
    /* 0x12 */ u16 ry;
    /* 0x14 */ u16 rz;
    /* 0x16 */ u8 pad16[0x51 - 0x16];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u16 dirty;
    /* 0x54 */ u8 pad54[0x5C - 0x54];
    /* 0x5C */ struct Ovl64Obj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ u8 *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} Ovl64Obj;

typedef struct Ovl64Scene {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Ovl64Obj *objs;
} Ovl64Scene;

typedef struct Ovl64Slot {
    /* 0x00 */ Ovl64Obj *obj;
    /* 0x04 */ s16 fixed[3]; /* used as is when nonzero */
    /* 0x0A */ s16 rate[3];  /* else D_800BC010 * rate >> 4 */
} Ovl64Slot;

typedef struct Ovl64Cell {
    s32 x;
    s32 y;
} Ovl64Cell;

extern Ovl64Cell D_800BBE10[4][16];

/* D_8005A614 as seen by the ranking code (func_800B3EDC). */
typedef struct Ovl64Game {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 flags;
    /* 0x34 */ u8 pad34[0x48 - 0x34];
    /* 0x48 */ s16 team[2];
    /* 0x4C */ u8 pad4C[0x5C - 0x4C];
    /* 0x5C */ u8 cpu[4];
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ s32 teams;
    /* 0x70 */ u8 pad70[0xA7 - 0x70];
    /* 0xA7 */ u8 rank[4];
} Ovl64Game;

extern s32 D_800BBE08;
extern s32 D_800BBE0C;
extern s32 D_800BC010;
extern s32 D_800BC014;
extern s16 D_800BC018;
extern s16 D_800BC01A;
extern s16 D_800BC01C;
extern Ovl64Obj *D_800BC020[10];
extern Ovl64Slot D_800BBCB0[10];

#include "bash/main.h"

extern s32 D_800BC048;
extern s32 D_8009E488[];

void func_800B3428();

void func_800B359C();

s32 func_800B8E4C(void);

s32 func_800B8E54(void);

void func_800B8E5C(void);

void func_800B9038(void);

void func_800B9040(void);

void func_800B9048(void);

void func_800B9088(void);

typedef struct {
    /* 0x00 */ u8 unk0[0x1C];
    /* 0x1C */ u16 mask;
    /* 0x1E */ u16 hit;
} Ovl64HitMask;

/* NAS.FUX of an object in the D_8005AA24 list (func_800BB230). */
typedef struct Ovl64Fux {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u16 kind;
    /* 0x16 */ s16 owner;
    /* 0x18 */ u8 pad18[0x1A - 0x18];
    /* 0x1A */ u16 sub;
    /* 0x1C */ u16 mask;
    /* 0x1E */ u16 hit;
    /* 0x20 */ u8 pad20[0x22 - 0x20];
    /* 0x22 */ s16 timer;
    /* 0x24 */ s16 state;
    /* 0x26 */ s16 angle;
    /* 0x28 */ s16 dist;
} Ovl64Fux;

/* balloon chance weights, indexed by score and 0x20 - score (func_800B697C) */
extern u16 D_800BB848[];

typedef struct {
    u8 unk0;
    u8 unk1;
} Ovl64MapCell;
extern Ovl64MapCell D_8009D90C[400];
extern u16 D_8009E25C[13];

void func_800BB37C(EXTengine *eng, Ovl64HitMask *m, u32 flag, s32 hero);

void func_800BB52C(s32 arg0);
extern void (*D_8009D73C)();

extern s32 D_8005AA70[11];
extern u8 D_80052FCC[];
extern u8 D_80052F94[];
extern u8 D_80052F5C[];
extern u16 D_800BC04C[];

void func_800B9BF8(s32 arg0);
extern u16 D_800BB89C[];
extern s32 D_8005A5D8;
extern s32 D_8005B5B8;
extern HeroInfo Heroes[4];
extern s32 D_8005A680;
extern s16 D_8009D87C[][3];
extern u16 *D_8009D6F4;
extern u16 *D_8009D6EC;
extern u16 D_800AD12C;
FlagRecord *func_80025A48(EXTengine *eng, u32 flag);

void func_80025F44(EXTengine *eng);
void *FindExtension(NAS *nas, s32 id);

s32 func_80085890();
s32 func_8008A200(s32 arg0);
void func_8008A810(void);

s32 func_800B8D98(s32 arg0);

extern NAS *D_8005AA2C;

void func_800B3938(void);

s32 func_800B8B5C(s32 arg0);

void func_800B9804(s32 arg0);

extern u16 D_800BBD58;
void func_800B6C50(s32 arg0);

void func_800B98B8(s32 arg0);
extern void *D_8005AA24;
s32 get_angle(s32 x, s32 z);
s32 func_80015480(s32 arg0, s32 arg1);

void func_800BB4FC(s32 arg0);

void func_800BB560(s32 arg0);

s32 func_800BB5F0(s32 arg0);

s32 func_800BB648(s32 arg0);

s32 func_800BB6B4(s32 arg0);

typedef struct Ovl64Pair {
    s32 a;
    s32 b;
} Ovl64Pair;

extern u16 D_800BBD5E;
Ovl64Pair *func_800B4F7C(u16 *head);
NAS *func_800B5D3C(s32 a, s32 b, u16 c, s16 d);
extern u16 D_800BBD5A;
static inline u16 Ovl64NextSlot(void) {
    D_800BBD5A = (D_800BBD5A + 1) & 3;
    return D_800BBD5A;
}
extern s16 D_800BB88C[];
extern u16 D_8009E46C[];
extern s32 D_80099178;
extern u16 D_800BB888[];
NAS *func_800B51A0(s32 a, s32 b, u16 c, s16 d);

void func_800B8E64(u16 arg0, u16 arg1);

void func_800B9EEC(s32 arg0);
extern u16 D_800BC054[];
extern u16 D_800BC05C[];
extern s32 D_8005A618;
extern void (*D_8009D740)(s32);
s32 func_8008502C();
s32 func_800871E4(s32 arg0);
s32 SetHeroSeq(HeroInfo *hero, s32 seq);

void func_800B382C(void);

s32 func_800B8F48(EXTengine *eng);

void func_800B92BC(s32 arg0);

void func_800B937C(s32 arg0);

void func_800B971C(s32 arg0);

void func_800B9A04(s32 arg0);

void func_800B9CE0(s32 arg0);
extern u8 D_8005A614[];

/* Hero jump parameters, 8 bytes per hero (D_8009D6E8.unk10). */
typedef struct Ovl64JumpEnt {
    u16 vel, max, acc, dec;
} Ovl64JumpEnt;

typedef struct Ovl64Hooks {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ void *unkC;
    /* 0x10 */ Ovl64JumpEnt *unk10;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ void *unk18;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ void *unk20;
    /* 0x24 */ u8 pad24[0x14];
    /* 0x38 */ void *unk38;
    /* 0x3C */ void *unk3C;
    /* 0x40 */ u8 pad40[4];
    /* 0x44 */ void *unk44;
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ void *unk4C;
    /* 0x50 */ void *unk50;
    /* 0x54 */ void *unk54;
    /* 0x58 */ void *unk58;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
} Ovl64Hooks;
extern Ovl64Hooks D_8009D6E8;

s32 func_800B8C74(s32 arg0);
extern void *D_8009D8F4;
extern void *D_8009D8F8;
extern void *D_8009D8FC;
extern void *D_8009D900;
extern void *D_8009D904;
extern s16 D_8009E0D8;
extern s16 D_8009E0E2;
extern s16 D_8009E0E4;
extern s16 D_8009E0E6;
extern u8 D_800BB720[];
extern u8 D_800BB734[];
extern u8 D_800BB748[];
extern u8 D_800BB754[];
extern u8 D_800BB76C[];
extern u8 D_800BB780[];
extern u8 D_800BB7BC[];
extern u8 D_800BB80C[];
void func_800B4144();
void func_800B7508();
void func_800B8EE0();
void func_800B90D0();
NAS *func_800BA098(s32 player, s32 mask, s32 unused, s32 cone);
NAS *func_800BA344(s32 idx, s32 mask);
s32 func_800BA514(s32 arg0);
void func_800BAF1C();

/* Extension 0x1000 flags set by func_800BA514. */
typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
} Ovl64HitFlags;

s32 func_800B3EDC(void);

void func_800B4BA4(void);

extern u16 D_800BBD54;
extern u16 D_800BBD56;
extern u16 D_800BBD5C;
extern u16 D_800BBD64;

void func_800B4970(void);

void func_800B4DC4(u16 count, s16 arg1);

void func_800B9608(s32 arg0);

Ovl64Fux *func_800BB230(s32 hero, u16 mask, s32 r);

/* func_800B359C */
extern u16 D_800BBD52;
extern u16 D_800BBD62;
extern s32 D_800BBD68;
extern EXTengine D_800BBD6C;
extern _sfx_handler *D_800BBE00;
extern s32 D_80053004;

void func_800B3A3C(void);

void func_800B697C(Ovl64Fux *arg0);

s32 func_800B6EC0(EXTengine *eng, EXTmotion *mot, Ovl64Fux *fux);

s32 func_800B85A4(s32 player);

#endif /* BASH_OVL64_H */
