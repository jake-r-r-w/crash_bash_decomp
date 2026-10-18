#ifndef BASH_OVL69_H
#define BASH_OVL69_H

/* Functions and data defined in the ovl69 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl69/ovl69.c */

#include "common.h"
#include "bash/nas.h"

void func_800B5324(void);
void func_800B532C(void);
void func_800B5360(s32 *arg0);
void func_800B5390(s32 arg0);
void func_800B544C(void);
void func_800B5454(void);
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
extern u8 D_800A0840[];
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

void func_800B556C(void);

void func_800B6A64(void);
typedef struct {
    s32 unk0;
    s32 unk4;
} Ovl69Entry;

extern Ovl69Entry D_800BA324[];

void func_800B39C8(void);

void func_800B5880(void);

void func_800B5A94(void);

int func_800B6B88(int a0, int a1, int a2, int a3, int a4);

s32 func_800B7DE8(s32 arg0);

void func_800B7E04(void);

void func_800B83D8(s32 arg0, s32 arg1);
/* func_800B4F1C */
#include "bash/main.h"
void func_8008D488(u8 *arg0, s32 arg1);
extern GfxCamera D_800B9538;
extern GfxViewport D_800B8DD8;
extern GfxViewport D_800B8E00;

void func_800B4F1C(void);

#endif /* BASH_OVL69_H */
