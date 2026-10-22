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

typedef struct Ovl69Sub {
    /* 0x000 */ u8 pad0[0x274];
    /* 0x274 */ s32 a;
    /* 0x278 */ u8 pad278[0x31C - 0x278];
    /* 0x31C */ s32 b;
} Ovl69Sub;

extern u8 D_8009F480[];
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
/* D_8005A604 as func_800B4910 edits it (options menu). */
typedef struct Ovl69Opt {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 unkC[4];
} Ovl69Opt;
extern s32 D_800B901C[];
extern s32 D_800B91CC[];
extern u8 D_800BA754[];
extern s32 D_800BAAC8;
extern s32 D_800BAACC;
extern void (*D_800B9DFC)(void);

void func_800B556C(void);

void func_800B6A64(void);
/* 16-byte slot record of an Ovl69Entry list (func_800B7458). */
typedef struct {
    s32 type;  /* 0x0: 1, or 2 = gated by D_8005A614[0xE4/0x168 + idx] */
    s32 idx;   /* 0x4 */
    void *model; /* 0x8 */
    s32 unkC;  /* 0xC */
} Ovl69ListSlot;

typedef struct {
    s32 unk0;
    s32 unk4;
} Ovl69Entry;

void func_800B6E0C(void);

extern Ovl69Entry D_800BA324[];
extern s32 D_800BA154[];
extern s32 D_800BA158[];
extern s32 D_800BA15C[];
extern s32 D_800BA160[];
extern s32 D_800BA660[];

/* Inline forms of the slot getters func_800B7D98 / func_800B7DC0 / func_800B7E04. */
static inline s32 Ovl69SlotModelI(s32 a, s32 b) {
    return *(s32 *)(D_800BA324[a].unk0 + (b << 4) + 8);
}

static inline s32 Ovl69SlotCI(s32 a, s32 b) {
    return *(s32 *)(D_800BA324[a].unk0 + (b << 4) + 0xC);
}

extern s32 *D_8004DD70[];

static inline s32 Ovl69SlotRowI(s32 a, s32 b) {
    s32 base = D_800BA324[a].unk0;
    s32 idx = *(s32 *)((u8 *)base + (b << 4) + 4);
    s32 *row = D_8004DD70[*(s32 *)(D_8005A614 + 0x2C)];
    return row[idx];
}

/* func_800B6E0C: is the selected slot of D_800BA324 available? */
static inline s32 Ovl69SlotOn(void) {
    u8 *g = D_8005A614;
    Ovl69ListSlot *slot;
    s32 type;
    s32 idx;

    slot = (Ovl69ListSlot *)((u8 *)D_800BA324[(s8)g[0x36]].unk0 + ((s8)g[0x37] << 4));
    idx = slot->idx;
    type = slot->type;
    if ((*(s32 *)(g + 4) & 0x2000) || type == 1
        || (type == 2 && (g[idx + 0xE4] != 0 || g[idx + 0x168] != 0))) {
        return 1;
    }
    return 0;
}

void func_800B39C8(void);
void func_800B3A9C(void);
void func_800B5360(s32 *arg0);

extern s32 D_800B8508[];
extern s32 D_800B8EA4[];
extern s32 D_800B8EC0[];
extern s32 D_800B8ED4[];
extern u8 D_800B9510[];

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
typedef struct Ovl69Obj {
    s32 unk0;
    s32 unk4;
    subengine *unk8;
    subengine *unkC;
    subengine *unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    Ext400 ext;
} Ovl69Obj;

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
/* D_800B9B44 as func_800B5CA8 reads it: 8 cards of 0x28 bytes. */
typedef struct {
    s32 flags;     /* 0x00: bit 0 taken */
    u8 unk4;
    u8 pad5[3];
    s32 x;         /* 0x08 */
    s32 y;         /* 0x0C */
    s32 target;    /* 0x10 */
    s32 unk14;     /* 0x14: index into D_80052A54 */
    s32 pad18;
    s32 unk1C;     /* 0x1C */
    s32 sfx;       /* 0x20 */
    s32 pad24;
} Ovl69Card;
#define O69CARD ((Ovl69Card *)D_800B9B44)
/* func_800B5CA8: speed toward tgt, +-lim (+-lim+900 when reversing), 0 when near. */
static inline s32 Ovl69Steer(s32 pos, s32 tgt, s32 vx, s32 lim) {
    s32 v;
    s32 a;
    s32 d;

    v = vx;
    if (pos != tgt) {
        if (pos < tgt) {
            if (vx < 0) {
                v = vx + (lim + 900);
            } else {
                v = vx + lim;
            }
        } else {
            v = vx - (lim + 900);
            if (vx <= 0) {
                v = vx - lim;
            }
        }
    }
    a = v;
    if (a < 0) {
        a = -a;
    }
    if (a <= lim) {
        d = tgt - pos;
        if (d < 0) {
            d = -d;
        }
        if (d <= lim) {
            v = 0;
        }
    }
    return v;
}
/* D_80068BD4 read bytewise: hi is the sine's high byte. */
typedef struct {
    s16 unk0;
    s8 lo;
    s8 hi;
} Ovl69Trig;
extern s32 D_800B9CA4[];
extern s32 D_800B9CB4[];
extern u8 D_80052A54[];
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

void func_800B42B0(void);

void func_800B4770(void);

void func_800B4848(void);

void func_800B5244(void);

void func_800B54B0(void);

void func_800B55B8(void);

void func_800B57BC(void);

void func_800B58E4(void);

s32 func_800B65D0(s32 a0);
void func_800B5CA8(Ovl69Slot *slot);

s8 func_800B6ACC(s32 a0);

s32 func_800B6C04(s32 arg0);

void func_800B6CF4(void);

void func_800B79A8(void);

void func_800B7B30(void);

void func_800B7BE0(s32 arg0);

s32 func_800B7DC0(s32 a, s32 b);

s32 func_800B7E54(s32 a, s32 b);

s32 func_800B7E7C(s32 a, s32 b);

s32 func_800B7EA4(s32 idx);

extern s8 D_8005A64A;

void func_800B7458(void);

s32 func_800B7CE8(s32 arg0);

/* 0x4C-byte preset record of D_800BAA08, picked by D_800BAACC (func_800B7EF4). */
typedef struct {
    s32 hero[4];   /* 0x00: indices into D_8005A614 (byte at +0x4C) */
    s32 flags[3];  /* 0x10: or-ed into sub[8/9/10].flags */
    s32 val[4];    /* 0x1C: targets for sub[4..7] */
    s32 *tbl;      /* 0x2C: indexed by D_8005A614+0x2C */
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
} Ovl69Preset;
extern Ovl69Preset D_800BAA08[];
/* func_800B83D8(i, val) inlined: ease sub[4 + i]'s +0x70 toward val. */
#define OVL69_SETBAR(i, val)                                                  \
    {                                                                         \
        s32 v_ = (val);                                                       \
        EXTengine *e_ = GetEngineSlot(0);                                     \
        u8 *q_ = (u8 *)e_->sub + ((i) * 0xA8 + 0x2A0);                        \
        s16 r_ = func_80015284(*(s16 *)(q_ + 0x70), v_, 0x800);               \
        *(s16 *)(q_ + 0x70) = r_;                                             \
        *(s16 *)(q_ + 0x76) = r_ + 0x78;                                      \
        *(s32 *)((u8 *)e_->sub + (i) * 0xA8 + 4) = *(s16 *)(q_ + 0x70) + 0x1A; \
    }
/* D_8005A614 as seen by func_800B7EF4. */
typedef struct {
    u8 pad0[0x2C];
    s32 unk2C;     /* 0x2C */
    u8 pad30[0x48 - 0x30];
    u16 unk48;     /* 0x48 */
    u16 unk4A;     /* 0x4A */
    u8 unk4C[8];   /* 0x4C: per player */
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 pad58[0x6C - 0x58];
    s32 unk6C;     /* 0x6C */
} Ovl69PresetView;
#define O69PV ((Ovl69PresetView *)D_8005A614)
extern u16 D_800BAAA0[];

void func_800B7EF4(void);

/* D_8005A614 as seen by func_800B4464 (option screen). */
typedef struct {
    u8 pad0[0x2C];
    s32 unk2C;     /* 0x2C */
    u8 pad30[0x32 - 0x30];
    u16 unk32;     /* 0x32 */
    u8 pad34[0x38 - 0x34];
    s16 unk38;     /* 0x38 */
    u16 unk3A;     /* 0x3A */
} Ovl69OptView;
#define O69OV ((Ovl69OptView *)D_8005A614)
extern s32 D_800B8E78[][3];
extern s32 D_800B8E84[][2];

void func_800B3CA8(void);

void func_800B4464(void);

/* D_8005A614 as seen by func_800B3F7C (player count menu). */
typedef struct {
    u8 pad0[0x26];
    u8 unk26;      /* 0x26: chosen count */
    u8 unk27;      /* 0x27: mode (D_8005A63B) */
    u8 pad28[0x32 - 0x28];
    u16 unk32;     /* 0x32 */
    u8 pad34[0x54 - 0x34];
    s8 unk54;
    s8 unk55;
    s8 unk56;
    s8 unk57;
    u8 pad58[0x5C - 0x58];
    u8 unk5C[4];   /* 0x5C */
    u8 pad60[0x6C - 0x60];
    s32 unk6C;     /* 0x6C */
} Ovl69CountView;
#define O69CV ((Ovl69CountView *)D_8005A614)
/* PadState (D_80051338) as seen by func_800B3F7C. */
typedef struct {
    u8 pad0[0x2A];
    s16 unk2A;     /* 0x2A */
    u8 pad2C[0x48 - 0x2C];
    s32 pressed;   /* 0x48 */
    u8 pad4C[0x58 - 0x4C];
} Ovl69Pad;
#define O69PAD ((Ovl69Pad *)D_80051338)
extern u8 D_8005A63B;
void func_800B3F7C(void);

void func_800B4910(void);

void func_800B53D4(void);
void func_800B5410(void);

void func_800B6734(void);

#endif /* BASH_OVL69_H */
