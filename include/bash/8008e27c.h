#ifndef BASH_8008E27C_H
#define BASH_8008E27C_H

/* Functions and data defined in src/overlays/engine/8008e27c.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/8008e27c.c */

#include "common.h"
#include "bash/engine.h"

extern s32 D_8005687C[];
void func_800798A4(void);
s32 func_8007F314(void);
void func_8007976C(void);
void func_80097C28(void);
extern u8 D_8005164C[];
extern GfxViewport D_8009EA74;
extern GfxViewport D_8009EA9C;
extern GfxViewport D_8009EAC4;
extern GfxViewport D_8009EAEC;
extern GfxViewport D_8009EB14;
extern GfxCamera D_800B9538;
void func_800B566C(void);

typedef struct { s32 w[4]; } Blk16;
typedef struct { Blk16 a[0x18]; s32 b[3]; } Blk18C;
typedef struct { s32 w[3]; } Blk12;
typedef struct { Blk12 a; s32 b; } Blk12p4;

typedef struct { u8 pad[0x6C]; s32 f6C; } Sub3F0;
extern s32 D_8009EA64[];
extern s32 D_8009EA68[];
extern s32 D_8009EA6C[];
extern s32 D_8009EA70[];
s32 func_8008F248(s32 arg0);

/* Inlined copies used by func_8008F248. */
static inline s32 EngineTable_AllDone(EngineTable12 *t) {
    EngineEntry16Head *p;
    s32 n;
    s32 i;

    n = t->unk8;
    p = t->unk4;
    for (i = 0; i < n; i++, p++) {
        if (!func_800B7EE4(p->unk4, D_8005A614)) {
            return 0;
        }
    }
    return 1;
}

static inline s32 EngineTable_Reachable(void *arg0, u8 *tab) {
    s32 i;
    s32 j;
    s32 cnt;
    s32 obj;
    u8 *p;

    i = 0;
    for (;;) {
        if (i >= *(s32 *)((u8 *)arg0 + 8)) {
            goto done;
        }
        obj = *(s32 *)((u8 *)(i * 16 + *(s32 *)((u8 *)arg0 + 4)) + 4);
        if (func_800B7EE4(obj, D_8005A614) == 0) {
            cnt = 0;
            j = cnt;
            p = tab;
            do {
                if (func_800B7EE4(obj, p) != 0) {
                    cnt++;
                }
                j++;
                p += 0x194;
            } while (j < 4);
            if (cnt == 0) {
                return 0;
            }
        }
        i++;
    }
done:
    return 1;
}

static inline void EngineTable_MarkDone(void *arg0) {
    s32 i;

    for (i = 0; i < ((EngineTable12 *)arg0)->unk8; i++) {
        D_8005A614[((EngineTable12 *)arg0)->unk4[i].unk4 + 0xE4] = 1;
    }
}

void func_8008EB3C(void);

void func_8008EB8C(void);

void func_8008F084(void);

void func_8008FC58(void);

void func_80090048(void);

void func_80091F54(void);

s32 func_800923F0(s32 *arg0);

void func_800923FC(void);

void func_80092404(void);

void func_800929D8(void);

void func_80092B7C(void);

void func_80092BA0(void);

void func_80092C54(void);

void func_80092E40(void);

void func_80091E58(void);

void func_80090310(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 D_8009F298[];
extern void *D_8009F004[];
extern char D_8009F4A8[][5];

void func_80090564(u8 *arg0, s32 arg1);

void func_800920DC(void);

void func_80090A10(void);

void func_80090E20(void);

void func_8008ECE4(void);

s32 func_8008EF70(void);

void func_8008EF9C(void);

void func_8008F068(void);

void func_80092F9C(void);

void func_8008EC94(void);

s32 func_80092560(u8 *arg0);

void func_8009029C(void);

void func_8008EAC8(void);

s32 func_8009020C(s32 arg0);

void func_80092CAC(void);

s32 func_80092598(u8 *arg0);

s32 func_800925C4(u8 *arg0);

void func_80092C78(void);

void func_80093038(void);

void func_80093110(void);

s32 func_80090008(void);

s32 *func_800901B4(s32 arg0);

void func_800917B4(void);

s32 func_8009251C(u8 *arg0);

void func_80092B40(void);

void func_8008FD40(void *arg0);

s32 func_80090248(void);

void func_80092AA8(GameModeDef *arg0);

void func_80092AF4(s32 arg0, s32 arg1);

void func_80092E94(void);

void func_8008FD94(void);

void func_8008FE00(void);

s32 func_8008FF10(void);

s32 func_8008FF94(void);

void func_80092198(void);

void func_80092BDC(void);

void func_80092FB8(void);

void InitCrateExplode(NAS *arg0);

void func_8008EC00(void);

void func_8008EFD4(void);

s32 func_8008FE68(s32 arg0);

void func_8009246C(void);

void func_8009201C(void);

extern s32 D_8009F248[][5];

void func_8008F41C(void);

void func_8008FB48(void);

s32 func_8008FC60(void *arg0);

s32 func_800900CC(void);

void func_80092200(u8 *src);

void func_800922FC(u8 *dst);

void func_800925F0(s32 arg0);

void func_800928E8(void);

void func_80092EDC(void);

#endif /* BASH_8008E27C_H */
