#ifndef BASH_OVL60_H
#define BASH_OVL60_H

/* Functions and data defined in the ovl60 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl60/ovl60.c */

#include "common.h"
#include "bash/nas.h"

void func_800C0BF0(s32 arg0, s32 arg1);
s32 func_800BE158(s32 i, s32 x, s32 z, s32 angle, s32 *outX, s32 *outZ);
s32 func_800C16FC();
s32 func_800C1764(void);
s32 func_800C2660(u8 *arg0);
void func_800C2F50(void);
void func_800C2F84(void);
void func_800C318C(s32 arg0);
void func_800C31B0(void);
void func_800C3388(void);
void func_800C3390(void);
void func_800B3E34();
void func_800C3398();
extern u8 D_8008AB04[];
extern u8 D_800C52CC[];
extern u8 D_800C44A8[];
void func_800B574C();
void func_800B5B1C();
void func_800BD10C();
void func_800C32E8();
void func_800B3534();

/* The engine callback table (EngineHooks in bash/engine.h). */
typedef struct Ovl60Hooks {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ void *unkC;
    /* 0x10 */ void *unk10;
    /* 0x14 */ void *unk14;
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ void *unk2C;
    /* 0x30 */ void *unk30;
    /* 0x34 */ void *unk34;
    /* 0x38 */ void *unk38;
    /* 0x3C */ void *unk3C;
    /* 0x40 */ void *unk40;
    /* 0x44 */ void *unk44;
    /* 0x48 */ void *unk48;
    /* 0x4C */ void *unk4C;
    /* 0x50 */ void *unk50;
    /* 0x54 */ void *unk54;
    /* 0x58 */ void *unk58;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
} Ovl60Hooks;
extern Ovl60Hooks D_8009D6E8;
extern u8 D_800C52B8[];
extern u8 D_8009F8A4;
void func_800C34B0(void);
void func_800C34D0(void);
void func_800C34F0(s32 arg0);
void func_800C3510(u8 *arg0);

void func_800C353C();

void func_80089FA4(s32 arg0);
extern u16 D_800AD12C;

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
/* What a board cell's unk4 points to (Plot_PogoBanker reads its x and z). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 z;
} Ovl60CellPos;
typedef struct {
    HeroInfo *hero;
    Ovl60CellPos *unk4;
    u8 unk8[2];
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
/* The position a board cell's unk4 points to, as PlaySfx takes it. */
#define CELL_POS(cx, cz) ((SLONG_XYZ *)&((Ovl60Cell *)((cx) * sizeof(D_800C53DC[0]) + (cz) * sizeof(Ovl60Cell) + (u32)D_800C53DC))->unk4->x)

/* A scene object in the list at D_80058AD0 + 0x1C (func_800B3C44). */
typedef struct Ovl60Obj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x5C - 0x4];
    /* 0x5C */ struct Ovl60Obj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ u8 *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} Ovl60Obj;
extern Ovl60Obj *D_800C5BB4;
extern HeroInfo Heroes[4];
void func_800B83AC(s32, s32);

/* 0x1C-byte entries of D_800C535C */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u16 unk4; /* state */
    /* 0x06 */ s16 unk6; /* timer */
    /* 0x08 */ s32 unk8[2];
    /* 0x10 */ NAS *nas;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s32 unk18;
} Ovl60Slot;

extern Ovl60Slot D_800C535C[];
/* PadState (D_80051338) as func_800BC184 reads it: d-pad bits 0xF0 at 0x50. */
typedef struct Ovl60Pad {
    /* 0x00 */ u8 unk0[0x50];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl60Pad;
s32 func_800BA110(s32 arg0, s32 arg1, s32 arg2);
void func_800B8BE4(s32 arg0, s32 arg1);
void func_800B7B1C(s32 x, s32 z, s32 hero);
void func_800B8180(s32 x, s32 z);
s32 func_800C1EA8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 D_800C53CC[];
extern s16 D_800C44D8[];
extern s32 D_800C4484[];
void func_800C1004(s32 arg0);
void func_800C02AC(EXTengine *eng, s32 angle);
extern u8 D_80053B9C[];
extern u8 D_80053ABC[];
extern u8 D_80053BD4[];
extern ScriptInit D_800C4680;
extern s32 D_800C5308;
extern s32 D_8005A680[];
extern u8 D_8005A614[];
extern NAS *D_800C5BB0;
extern s32 D_800C52E0;
extern u16 D_800C530C[];
extern s32 D_800C52E4;
extern s32 D_800C52EC;
extern s16 D_800C52E8;
extern s32 D_800C38D8[];
extern s32 D_800C38E8[];
extern s32 D_800C38F8[];
extern s32 D_800C52F0;
extern s32 D_800C52F8;
extern s32 D_800C52FC;
extern s32 D_800C5304;
extern s32 D_800C5BAC;
extern s32 D_800C5BB8;
extern u16 D_800C3998[][2];
extern s32 D_800C5BBC;
extern u16 D_800C3A38[][2];
extern s32 D_800C5074[];
extern s32 D_800C5050[];
extern s32 D_800C502C[];
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
extern u8 D_800C3C14[][4][8];

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Ovl60Move; /* size 0x14 */

typedef struct {
    s32 x;
    s32 z;
} Ovl60Offset;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s16 unk20;
} Ovl60Goal;

extern Ovl60Move D_800C5ADC[];
extern Ovl60Offset D_800C457C[];
extern Ovl60Offset D_800C45BC[];
s32 func_800C0DE4(s32 arg0);
s32 func_800BE6C4(s32 arg0, Ovl60Goal *arg1);
void func_800C1068();

void func_800C10F8(void);

void func_800C11C4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void func_800C159C(s32 arg0);

void func_800C2810(s32 arg0);

void func_800C2B98(s32 arg0);
extern s16 D_8009D87C[][3];

void func_800B76CC(s32 arg0);

void func_800BC0EC(s32 arg0);

void func_800BD008(s32 arg0);

s32 func_800BD378(s32 hero);

s32 func_800C0AFC(s32 arg0, s32 arg1);

s32 func_800C0B70(s32 arg0, s32 arg1, s32 arg2);

void func_800C0C34(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 func_800C0DE4(s32 arg0);

s32 func_800C0E40(s32 arg0, u32 arg1, u32 arg2);

void func_800C0FAC(s32 i);

void func_800C1384(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_800C1400(s32 arg0);

void func_800C142C(s32 arg0);

s32 func_800C1460(s32 arg0);

s32 func_800C1608(s32 arg0, s32 arg1);

s32 func_800C165C(s32 *arg0, s32 *arg1);

void func_800C19A8(s32 *arg0, s32 arg1);

void func_800C2294(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

void func_800C22E8(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

void func_800C2338(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

void func_800B7990(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_800C244C(s32 arg0, s32 arg1);

void func_800C2488(s32 arg0, s32 arg1);

void func_800C24E4(s32 arg0, s32 arg1);

void func_800C2524(s32 arg0, s32 arg1, s32 arg2);

void func_800B7C3C(s32 arg0, s32 arg1);

void func_800C2620(s32 arg0, s32 arg1);

void func_800C286C(s32 arg0);

void func_800C28FC(s32 arg0);

void func_800C2A8C(s32 arg0);

s32 func_800C2C70(s32 arg0);

void func_800C2DCC(s32 arg0);

void func_800C2E50(s32 arg0, s32 arg1);

void func_800C2EE0(void);
void func_800C1CF0();
void func_800B4230();
void func_800B87DC();
void func_800B9C10();
void func_800B9D64();
void func_800BCB4C();
extern s32 D_800C52F4;
extern s32 D_80099178;
extern s32 D_800C5BD8;
extern s32 D_800C5BDC;
extern s32 D_800C5BE0;
extern s32 D_800C5BE4;
extern s32 D_800C5BE8;
extern s32 D_800C5BEC;

/* D_8005A614+0x34 as func_800B3E34 switches on it: an enum, so gcc's
 * switch tree skips its text cost table. */
typedef enum Ovl60Mode {
    OVL60_MODE_NONE = -1,
    OVL60_MODE_10 = 10,
    OVL60_MODE_11 = 11,
    OVL60_MODE_12 = 12
} Ovl60Mode;
s32 func_8008502C();
void func_800B3C44(void);

void func_800B6370(s32 arg0);

void func_800B6438(s32 arg0);

void func_800B6790(s32 arg0);

s32 func_800B6C14(s32 arg0);
s32 func_800B5D90(s32 arg0);
void func_800BDF60(s32 arg0, s32 *arg1, s32 *arg2);

s32 func_800C1DA4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 func_800C2CBC(s32 arg0);

void func_800B72B0(s32 arg0);
void func_800B741C(s32 arg0);
void func_800B7574(s32 arg0);
void func_800B8AF8(s32 arg0);

void func_800B4374(s32 arg0, u16 arg1);
void func_800B44F4(s32 arg0, u16 arg1);
void func_800B4A70(s32 arg0, u16 arg1);
void func_800B4E50(s32 arg0, u16 arg1);
void func_800B53CC(s32 arg0, u16 arg1);

void func_800B8DFC(u32 arg0);

void func_800B8EC8(u32 arg0);

void func_800B8F94(s32 arg0);

s32 func_800BBF00(s32 *arg0, s32 *arg1);

s32 func_800BEB6C(s32 arg0, s32 arg1, s32 arg2);

s32 func_800C0F08(s32 arg0, EXTbotai *ai);

s32 func_800C14BC(s32 arg0, s32 arg1);

s32 func_800C178C(void);

void func_800C1A18(s32 arg0);

void func_800C2018(s32 arg0, s32 arg1, s32 arg2);

void func_800C20C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_800C2204(void);

void func_800C2388(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

void func_800C2564(s32 arg0, s32 arg1, s16 arg2);

void func_800C2674(s32 arg0);

void func_800C2770(s32 arg0, u32 arg1, u32 arg2);

s32 func_800C295C(s32 arg0);

s32 func_800C29F4(s32 arg0);

s32 func_800C2B00(s32 arg0);

void func_800C2BE8(s32 arg0);

void func_800C2FB8(void);

void func_800C3048(void);

void func_800C3148(void);

s32 func_800BC04C(s32 arg0, s32 arg1);

void func_800C327C(s32 arg0);

void func_800C3434(void);
void func_800B8660(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_800B788C(s32 arg0);
void func_800BB840(s32 *arg0, s32 arg1);
s32 func_800BBB08(s32 *arg0, s32 arg1, s32 arg2);

s32 func_800BEA34(s32 x, s32 z, s32 hero);
s32 func_800BE864(s32 hero, Ovl60Goal *goal);

/* D_8005A614 as seen by func_800B3A38. */
typedef struct Ovl60ScoreView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0x5C - 0x36];
    /* 0x5C */ u8 out[4];
    /* 0x60 */ u8 unk60[0x6C - 0x60];
    /* 0x6C */ s32 teams;
} Ovl60ScoreView;
#define SCORE60V ((Ovl60ScoreView *)D_8005A614)

/* func_800B3FF0: per-mode entries picked by teams, with two values at 0xE/0x10. */
typedef struct Ovl60ModeEntry {
    /* 0x00 */ u8 unk0[0xE];
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u8 unk12[2];
} Ovl60ModeEntry;
extern Ovl60ModeEntry D_800C36B4;
extern Ovl60ModeEntry D_800C36C8;
extern Ovl60ModeEntry D_800C3760;
extern Ovl60ModeEntry D_800C3774;
extern Ovl60ModeEntry D_800C37F8;
extern Ovl60ModeEntry D_800C380C;
extern Ovl60ModeEntry D_800C3890;
extern Ovl60ModeEntry D_800C38A4;
extern u16 D_800C3678[];
extern u16 D_800C3724[];
extern u16 D_800C37BC[];
extern u16 D_800C3854[];
/* D_8005A614 as seen by func_800B3FF0. */
typedef struct Ovl60ModeView {
    /* 0x00 */ u8 unk0[0x32];
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[2];
    /* 0x38 */ s16 unk38;
    /* 0x3A */ u8 unk3A[0x5C - 0x3A];
    /* 0x5C */ u8 out[4];
    /* 0x60 */ u8 unk60[0x6C - 0x60];
    /* 0x6C */ s32 teams;
} Ovl60ModeView;
void func_8007B7FC(u32 a, u32 b);
void func_8007D144(void);
extern s32 D_80099174;

void func_800B3808(void);

void func_800B3A38(void);

void func_800B3FF0(void);

void func_800B662C(s32 arg0);

void func_800B6960(s32 arg0);

void func_800B6AE0(s32 arg0);

void func_800B6DF4(s32 arg0);

void func_800B7B1C(s32 arg0, s32 arg1, s32 arg2);

void func_800B9080(s32 arg0);

void func_800B91E8(s32 arg0);

s32 func_800BA018(s32 arg0, s32 arg1, s32 arg2);

void func_800BA25C(s32 arg0, s32 arg1, s32 arg2);

void func_800BA3E0(s32 arg0, s32 arg1, s32 arg2);

void func_800BB724(s32 arg0, s32 arg1);

s32 func_800BBC94(s32 x, s32 z);

void func_800BC184(s32 arg0, s32 arg1);

void func_800BC314(s32 arg0, s32 arg1);

void func_800BC490(s32 arg0);

s32 func_800BD520(s32 hero);
void func_800BD790(s32 hero);

s32 func_800BD630(s32 hero);

s32 func_800BDCB8(s32 arg0, s32 x, s32 z, s32 dx, s32 dz, s32 flags);

s32 func_800BDE24(s32 arg0);

void func_800BFFF4(EXTengine *eng);

void func_800C1840(u32 x, u32 z, s32 id, s32 *seen);
s32 func_800BDCB8(s32 hero, s32 x, s32 z, s32 arg3, s32 arg4, s32 arg5);

#endif /* BASH_OVL60_H */
