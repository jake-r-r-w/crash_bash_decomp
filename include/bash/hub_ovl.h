#ifndef BASH_HUB_OVL_H
#define BASH_HUB_OVL_H

/* Functions and data defined in the hub overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/hub/hub.c */

#include "common.h"
#include "bash/nas.h"

s32 *HubCreditsText(void);
void UpdateHUB(void);
void InitialiseHUB(void);
void InitialiseHUBFirst(void);
void InitialisePodiumFirst(void);
void InitialisePodium(void);

extern s32 D_80098714;
/* D_80051640 as seen by HubEndCredits. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ u8 pad18[0x56 - 0x18];
    /* 0x56 */ u16 h;
} HubCam;
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
#include "bash/hub_hubcore.h"

void HubRender(void);

void HubEndCredits(void);

int HubCountLines(unsigned char *s);

void HUBSetUp(void);

/* One line of the scrolling text table at D_800C7294 (HubCreditsScroll). */
typedef struct OvlTextLine {
    /* 0x00 */ u32 kind;   /* 0,1: big; 2: small; 3: gap; 4: end */
    /* 0x04 */ u8 *text;   /* '\r' separates lines */
    /* 0x08 */ s32 height; /* kind 3 */
} OvlTextLine;
extern OvlTextLine D_800C7294[];

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

/* D_80051640 as HubCreditsScroll sets it up. */
typedef struct OvlTextView {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[0x54 - 0x18];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} OvlTextView;

s32 HubCreditsScroll(void);

#endif /* BASH_HUB_OVL_H */
