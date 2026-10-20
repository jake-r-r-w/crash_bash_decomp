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
    /* 0x00 */ u8 pad0[0x51];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 pad52[0x5C - 0x52];
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
    Ovl64Obj *obj;
    s32 pad[3];
} Ovl64Slot;

typedef struct Ovl64Cell {
    s32 x;
    s32 y;
} Ovl64Cell;

extern Ovl64Cell D_800BBE10[4][16];
extern s32 D_800BC010;
extern s32 D_800BC014;
extern s16 D_800BC018;
extern s16 D_800BC01A;
extern s16 D_800BC01C;
extern Ovl64Obj *D_800BC020[10];
extern Ovl64Slot D_800BBCB0[10];

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

void func_800BB37C(EXTengine *eng, Ovl64HitMask *m, u32 flag, s32 hero);

void func_800BB52C(s32 arg0);
extern void (*D_8009D73C)();

extern s32 D_8005AA70[11];
extern u8 D_80052FCC[];
extern u16 D_800BC04C[];

void func_800B9BF8(s32 arg0);
extern u16 D_800BB89C[];
extern s32 D_8005A5D8;
extern s32 D_8005B5B8;
extern HeroInfo Heroes[4];
extern s32 D_8005A680;
extern s16 D_8009D87C[][3];
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

void func_800BB4FC(s32 arg0);

void func_800BB560(s32 arg0);

s32 func_800BB5F0(s32 arg0);

s32 func_800BB648(s32 arg0);

s32 func_800BB6B4(s32 arg0);

typedef struct Ovl64Pair {
    s32 a;
    s32 b;
} Ovl64Pair;

extern u8 D_800BBD5E;
Ovl64Pair *func_800B4F7C(void *arg0);
void func_800B51A0(s32 a, s32 b, s32 c, s32 d);

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

#endif /* BASH_OVL64_H */
