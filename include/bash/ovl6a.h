#ifndef BASH_OVL6A_H
#define BASH_OVL6A_H

/* Functions and data defined in the ovl6a overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl6a/ovl6a.c */

#include "common.h"
#include "bash/nas.h"

s32 *func_800B4D8C(void);
void func_800B4DE8(void);
void func_800B7874(void);
void func_800B7A78(void);
void func_800B7B7C(void);
void func_800B7C78(void);
void func_800B7C98(void);
void func_800B7D7C(void);
void func_800B7D8C(void);
void func_800B7DB4(u8 *arg0, s32 arg1);
void func_800B7E04(void);
void func_800B7E0C(void);
s32 func_800B7F74(u8 *arg0, u8 *arg1);
s32 func_800B821C(void);
s32 func_800B8224(void);
void func_800B8588(void);
void func_800B85A8(void);
void func_800BA804(void);
void func_800BA848(void);
void func_800BA978(u8 *arg0);
void func_800BAAFC(void);
void func_800BB364(void);
void func_800BB38C(void);
void func_800BBE1C(void);

extern u8 D_800C7294[];
extern s32 D_800BCED4;
extern s32 D_800BCED8;
extern s32 D_800BCEE0;
extern subengine *D_800BCEE8;

#endif /* BASH_OVL6A_H */
