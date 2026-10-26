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
/* MainCamera as seen by HubEndCredits. */
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
extern s32 HubCreditsY;
extern s32 HubCreditsStart;
extern u16 HubSavedCamH;
extern u16 HubSavedFov;
extern s32 HubSavedCamX;
extern s32 HubSavedCamY;
extern s32 HubSavedCamZ;
extern s32 HubDoorChoice;
extern s32 HubSelectState;
extern s32 HubSelectedEntry;
extern subengine *HubSelectSub;

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/hub_hubcore.h"

void HubRender(void);

void HubEndCredits(void);

int HubCountLines(unsigned char *s);

void HUBSetUp(void);

/* One line of the scrolling text table at HubCreditsLines (HubCreditsScroll). */
typedef struct OvlTextLine {
    /* 0x00 */ u32 kind;   /* 0,1: big; 2: small; 3: gap; 4: end */
    /* 0x04 */ u8 *text;   /* '\r' separates lines */
    /* 0x08 */ s32 height; /* kind 3 */
} OvlTextLine;
extern OvlTextLine HubCreditsLines[];

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

/* MainCamera as HubCreditsScroll sets it up. */
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
