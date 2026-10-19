#ifndef BASH_OVL60_H
#define BASH_OVL60_H

/* Functions and data defined in the ovl60 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl60/ovl60.c */

#include "common.h"
#include "bash/nas.h"

void func_800C0BF0(s32 arg0, s32 arg1);
s32 func_800C1764(void);
s32 func_800C2660(u8 *arg0);
void func_800C2F50(void);
void func_800C2F84(void);
void func_800C318C(s32 arg0);
void func_800C31B0(void);
void func_800C3388(void);
void func_800C3390(void);
void func_800C34B0(void);
void func_800C34D0(void);
void func_800C34F0(s32 arg0);
void func_800C3510(u8 *arg0);

void func_800C353C();
typedef struct {
    u8 unk0[0x10];
    u16 unk10;
    u8 unk12[0xA];
} Ovl60Cell; /* size 0x1C */

extern Ovl60Cell D_800C53DC[8][8];
extern HeroInfo Heroes[4];

/* 0x1C-byte entries of D_800C535C */
typedef struct {
    /* 0x00 */ s32 unk0[4];
    /* 0x10 */ NAS *nas;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} Ovl60Slot;

extern Ovl60Slot D_800C535C[];
void func_800C02AC(EXTengine *eng, s32 angle);
extern u8 D_80053B9C[];
extern NAS *D_800C5BB0;
extern s32 D_800C52E0;
extern s32 D_800C52E4;
extern s32 D_800C52EC;
extern s32 D_800C52F0;
extern s32 D_800C52F8;
extern s32 D_800C52FC;
extern s32 D_800C5304;
extern s32 D_800C5BAC;
extern s32 D_800C5BB8;
extern s32 D_800C5BBC;
extern s32 D_800C5300;
extern s32 D_800C5BC0;
extern s32 D_800C5BC4;
extern s32 D_800C5BCC;
extern s32 D_800C5BD0;
extern s32 D_800C5BD4;

void func_800C1068();

void func_800C10F8(void);

void func_800C159C(s32 arg0);

void func_800C2810(s32 arg0);

void func_800C2B98(s32 arg0);
extern s16 D_8009D87C[][3];

void func_800C0C34(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_800C2EE0(void);

#endif /* BASH_OVL60_H */
