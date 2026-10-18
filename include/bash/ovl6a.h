#ifndef BASH_OVL6A_H
#define BASH_OVL6A_H

/* Functions and data defined in the ovl6a overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl6a/ovl6a.c */

#include "common.h"
#include "bash/nas.h"

s32 *func_800B4D8C(void);
void func_800B4DE8(void);
void func_800B4EAC(void);
void func_800B86AC(void);
void func_800B875C(void);
void func_800B88B8(void);

extern u8 D_800C7294[];
extern s32 D_80098714;
/* D_80051640 as seen by func_800B4C94. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ u8 pad18[0x56 - 0x18];
    /* 0x56 */ u16 h;
} Ovl6aCam;
extern u16 D_8009870E;
extern u16 D_80098710;
extern s32 D_800BC2B4;
extern s32 D_800BC2B8;
extern u16 D_800BC2BC;
extern u16 D_800BC2C0;
extern s32 D_800BC2C4;
extern s32 D_800BC2C8;
extern s32 D_800BC2CC;
extern s32 D_800BCED4;
extern s32 D_800BCED8;
extern s32 D_800BCEE0;
extern subengine *D_800BCEE8;

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl6a_800b4eac.h"

void func_800B4694(void);

void func_800B4C94(void);

int func_800B4D98(unsigned char *s);

void func_800B4E1C(void);

/* One line of the scrolling text table at D_800C7294 (func_800B488C). */
typedef struct OvlTextLine {
    /* 0x00 */ u32 kind;   /* 0,1: big; 2: small; 3: gap; 4: end */
    /* 0x04 */ u8 *text;   /* '\r' separates lines */
    /* 0x08 */ s32 height; /* kind 3 */
} OvlTextLine;

/* n = the number of '\r'-separated lines in text, 0 for none. */
#define OVL_TEXT_LINE_COUNT(n, text)          \
    {                                         \
        s32 _cnt = 1;                         \
        u8 *_p = (text);                      \
        if (_p == NULL) {                     \
            n = 0;                            \
        } else {                              \
            s32 _k;                           \
            for (_k = 0; _p[_k] != 0; _k++) { \
                if (_p[_k] == '\r') {         \
                    _cnt++;                   \
                }                             \
            }                                 \
            n = _cnt;                         \
        }                                     \
    }

/* D_80051640 as func_800B488C sets it up. */
typedef struct OvlTextView {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[0x54 - 0x18];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} OvlTextView;

s32 func_800B488C(void);

#endif /* BASH_OVL6A_H */
