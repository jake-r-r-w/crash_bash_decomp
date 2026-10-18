#ifndef BASH_OVL69_H
#define BASH_OVL69_H

/* Functions and data defined in the ovl69 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl69/ovl69.c */

#include "common.h"
#include "bash/nas.h"

extern u8 D_8004E568[];
extern void (*D_8009E58C)(void);
extern s32 D_800B865C[];
extern s32 D_800B877C[];
void func_800B5324(void);
void func_800B532C(void);
void func_800B5360(s32 *arg0);
void func_800B5390(s32 arg0);
void func_800B544C(void);
void func_800B5454(void);
void func_800B37BC(void);
void func_800B5A6C(void);
void func_800B5A74(void);
void func_800B7D20(void);
void func_800B81F0(void);
void func_800B83A4(void);
void func_800B8470(void);
void func_800B8490(void);
void func_800B8498(void);
void func_800B84B8(void);
void func_800B84E0(void);

extern GfxViewport D_800B8DD8;
extern s32 D_800B95F0;
extern s32 D_800B9628;
extern s32 D_800B889C[];
extern s32 D_800B9624;
extern s8 D_800B9CC4[];
extern s8 D_800B9CF4[];
extern s32 D_800B8518[];
extern u8 D_800A0840[];
extern u8 D_8009E5C8[];
extern u16 D_800B8DC8[];
void func_80094D88(s32 arg0, s32 arg1);
extern u8 D_800BAAD0[];
extern GfxViewport D_800B8DD8;
extern GfxCamera D_800B9F1C;
extern s32 *D_800B9620;
extern u8 D_800B8974[];
extern s32 D_800B962C;
extern u8 D_800B8F20[];
extern s32 D_800B9508;
extern s32 D_800B950C;
extern u8 D_800BA754[];
extern s32 D_800BAAC8;
extern s32 D_800BAACC;
extern void (*D_800B9DFC)(void);

void func_800B556C(void);

void func_800B6A64(void);
typedef struct {
    s32 unk0;
    s32 unk4;
} Ovl69Entry;

extern Ovl69Entry D_800BA324[];

void func_800B39C8(void);

extern s32 D_800B8C84;
extern u8 D_800B8E28[];
extern s32 *D_800B8EB8;
extern s32 *D_800B8EE4;
extern Ext400 D_800B95F4;
extern Ext400 *D_800B9600;
extern Ext400 D_800B8C2C;

typedef struct {
    s32 unk0;
    u8 unk4[0x20];
    u8 *unk24;
} Ovl69Rec;

extern Ovl69Rec D_800B9B44[];
extern s32 D_800B9634;
extern s32 D_800B9A00;
extern s32 D_800BA35C;
extern s32 D_800BA3A4;
extern s32 D_800BA3EC;
extern s32 D_800BA434;
extern s32 D_800BA47C;
extern s32 D_800BA4D4;
extern s32 D_800BA740;
extern s32 D_800BA744;
void func_800B5B10(s32 arg0);
void func_800B64B8(void);
typedef struct {
    s32 pad0[2];
    s32 x;
    s32 y;
    s32 pad10[6];
} Ovl69Pt;
typedef struct {
    s32 pad0[1];
    s32 unk4;
    s32 unk8;
} Ovl69Dst;
typedef struct {
    s32 pad0[3];
    Ovl69Dst *dst;
    s32 pad10[8];
    s32 idx;
    s32 pad34[11];
} Ovl69Slot;
typedef struct {
    s32 x;
    s32 y;
} Ovl69Off;
extern Ovl69Slot D_800B9FD4[];
extern Ovl69Off D_800B9C84[];
/* D_8005A614 as seen by func_800B64B8 (unk26 is D_8005A63A). */
typedef struct {
    u8 pad0[0x26];
    u8 unk26;
} Ovl69ModeView;

void func_800B5880(void);

void func_800B5A94(void);

int func_800B6B88(int a0, int a1, int a2, int a3, int a4);

s32 func_800B7DE8(s32 arg0);

extern s32 *D_8004DD70[];

s32 func_800B7E04(s32 a, s32 b);

void func_800B83D8(s32 arg0, s32 arg1);
/* func_800B4F1C */
#include "bash/main.h"
void func_8008D488(u8 *arg0, s32 arg1);
extern GfxCamera D_800B9538;
extern GfxViewport D_800B8DD8;
extern GfxViewport D_800B8E00;
extern GfxViewport D_800BA664;
extern GfxViewport D_800BA68C;
extern GfxViewport D_800BA6B4;
extern GfxViewport D_800BA6DC;
extern GfxViewport D_800BA704;

void func_800B4F1C(void);

void func_800B51C0(void);

void func_800B51EC(void);

void func_800B5218(void);

void func_800B5484(void);

void func_800B54F0(void);

void func_800B5614(void);

void func_800B5734(void);

void func_800B5978(u8 *cam, s32 arg1);

void func_800B5AC0(void);

void func_800B63F4(void);

s32 func_800B7C68(s32 arg0);

s32 func_800B7D28(s32 arg0, s32 arg1);

s32 func_800B7D98(s32 arg0, s32 arg1);

extern void (*D_8009E598)(void);

extern void (*D_8009E594)(void);

extern void (*D_8009E590)(void);

extern s8 D_8005A64B;

void func_800B4770(void);

void func_800B4848(void);

void func_800B5244(void);

void func_800B54B0(void);

void func_800B55B8(void);

void func_800B57BC(void);

void func_800B58E4(void);

s32 func_800B65D0(s32 a0);

s8 func_800B6ACC(s32 a0);

void func_800B6CF4(void);

void func_800B79A8(void);

void func_800B7B30(void);

void func_800B7BE0(s32 arg0);

s32 func_800B7DC0(s32 a, s32 b);

s32 func_800B7E54(s32 a, s32 b);

s32 func_800B7E7C(s32 a, s32 b);

s32 func_800B7EA4(s32 idx);

#endif /* BASH_OVL69_H */
