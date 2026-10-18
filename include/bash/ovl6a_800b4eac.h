#ifndef BASH_OVL6A_800B4EAC_H
#define BASH_OVL6A_800B4EAC_H

/* Functions and data defined in src/overlays/ovl6a/800b4eac.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl6a/800b4eac.c */

#include "common.h"
#include "bash/ovl6a.h"

extern s32 D_800BF260;
extern u32 D_800BD5B0;
extern s32 D_800BD5B4;

typedef struct Ovl6aGameView {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8 pad26[0x30 - 0x26];
    /* 0x30 */ u16 unk30;
} Ovl6aGameView;
extern s32 D_800BD390;
extern s32 D_800BD394;
extern s32 D_800BCEF4;

void func_800B7874(void);

void func_800B7988(s32 *arg0, s32 flag, s32 *out);

void func_800B7A78(void);

u16 func_800B7B38(void);

void func_800B7B7C(void);

void func_800B7BA4(void);

void func_800B7C78(void);

void func_800B7C98(void);

void func_800B7CA8(void);

void func_800B7D7C(void);

void func_800B7D8C(void);

void func_800B7DB4(u8 *arg0, s32 arg1);

void PlotGatewayTrophy(void);

void InitGatewayTrophy(void);

s32 func_800B7F74(u8 *arg0, u8 *arg1);

void func_800B7FAC(s32 arg0);

void func_800B7FD0(u8 *cam, s32 arg1);

void func_800B8148(s32 arg0);

s32 func_800B821C(void);

s32 func_800B8224(void);

void func_800B822C(s32 arg0);

void func_800B8474(s32 arg0);

s32 func_800B84CC(void);

void func_800B8588(void);

void func_800B85A8(void);

void func_800B8CA0(void);

void func_800BA7BC(NAS *arg0);

void func_800BA804(void);

void func_800BA848(void);

void InitPodiumOdo(u8 *arg0);

void func_800BAAFC(void);

void func_800BAB40(void);

void func_800BB364(void);

void func_800BB38C(void);

void func_800BBE1C(void);

#endif /* BASH_OVL6A_800B4EAC_H */
