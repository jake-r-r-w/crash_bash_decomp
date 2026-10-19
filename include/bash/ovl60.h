#ifndef BASH_OVL60_H
#define BASH_OVL60_H

/* Functions and data defined in the ovl60 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl60/ovl60.c */

#include "common.h"
#include "bash/nas.h"

void func_800C0BF0(s32 arg0, s32 arg1);
s32 func_800C16FC();
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

/* D_80051640 as the camera func_800C3148 sets up. */
typedef struct Ovl60Cam {
    /* 0x00 */ s32 eyeX, eyeY, eyeZ;
    /* 0x0C */ s32 atX, atY, atZ;
    /* 0x18 */ s32 dist;
    /* 0x1C */ u8 pad1C[0x54 - 0x1C];
    /* 0x54 */ s16 rotX;
    /* 0x56 */ s16 rotY;
    /* 0x58 */ s16 rotZ;
} Ovl60Cam;
typedef struct {
    HeroInfo *hero;
    u8 unk4[6];
    s16 unkA;
    u8 unkC[2];
    u16 unkE;
    u16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8 unk18[4];
} Ovl60Cell; /* size 0x1C */

extern Ovl60Cell D_800C53DC[8][8];
extern HeroInfo Heroes[4];

/* 0x1C-byte entries of D_800C535C */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s32 unk8[2];
    /* 0x10 */ NAS *nas;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} Ovl60Slot;

extern Ovl60Slot D_800C535C[];
extern s32 D_800C53CC[];
void func_800C1004(s32 arg0);
void func_800C02AC(EXTengine *eng, s32 angle);
extern u8 D_80053B9C[];
extern s32 D_8005A680[];
extern u8 D_8005A614[];
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
extern u16 D_800C3998[][2];
extern s32 D_800C5BBC;
extern u16 D_800C3A38[][2];
extern s32 D_800C5300;
extern s32 D_800C5BC0;
extern s32 D_800C5BC4;
extern s32 D_800C5BC8;
extern s32 D_800C5BCC;
extern s32 D_800C5BD0;
extern s32 D_800C5BD4;
extern s32 D_800C5B2C[8];
extern s32 D_800C5B4C[8];
extern s32 D_800C5B6C[8];
extern s32 D_800C5B8C[8];

void func_800C1068();

void func_800C10F8(void);

void func_800C159C(s32 arg0);

void func_800C2810(s32 arg0);

void func_800C2B98(s32 arg0);
extern s16 D_8009D87C[][3];

void func_800BD008(s32 arg0);

s32 func_800BD378(s32 hero);

s32 func_800C0AFC(s32 arg0, s32 arg1);

void func_800C0C34(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 func_800C0DE4(s32 arg0);

s32 func_800C0E40(s32 arg0, u32 arg1, u32 arg2);

void func_800C0FAC(s32 i);

void func_800C1400(s32 arg0);

void func_800C142C(s32 arg0);

s32 func_800C1460(s32 arg0);

s32 func_800C1608(s32 arg0, s32 arg1);

s32 func_800C165C(s32 *arg0, s32 *arg1);

void func_800C2294(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

void func_800C22E8(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

void func_800C2338(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

void func_800C244C(s32 arg0, s32 arg1);

void func_800C2488(s32 arg0, s32 arg1);

void func_800C24E4(s32 arg0, s32 arg1);

void func_800C2524(s32 arg0, s32 arg1, s32 arg2);

void func_800C2620(s32 arg0, s32 arg1);

void func_800C286C(s32 arg0);

void func_800C28FC(s32 arg0);

s32 func_800C2C70(s32 arg0);

void func_800C2E50(s32 arg0, s32 arg1);

void func_800C2EE0(void);
void func_800C1CF0();
s32 func_8008502C();

void func_800B4374(s32 arg0, u16 arg1);
void func_800B4A70(s32 arg0, u16 arg1);
void func_800B4E50(s32 arg0, u16 arg1);
void func_800B53CC(s32 arg0, u16 arg1);

void func_800C3048(void);

void func_800C3148(void);

s32 func_800BC04C(s32 arg0, s32 arg1);

void func_800C327C(s32 arg0);

#endif /* BASH_OVL60_H */
