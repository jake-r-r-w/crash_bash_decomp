#ifndef BASH_OVL68_H
#define BASH_OVL68_H

/* Functions and data defined in the ovl68 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl68/ovl68.c */

#include "common.h"
#include "bash/nas.h"

typedef struct CoalFlameInner {
    s32 pad0[3];
    s32 unkC;
} CoalFlameInner;

typedef struct CoalFlameObj {
    u8 pad0[0x54];
    CoalFlameInner *unk54;
} CoalFlameObj;

void func_800B3330(void);
void func_800BADA8(s32 arg0);
void func_800BAE20(void);
void func_800BAE28(void);
s32 func_800BAE84(void);
s32 func_800BAE8C(void);
void func_800BB36C(void);
void func_800BB374(void);
void func_800BB37C(void);
void func_800BB640(s32 arg0);

extern u8 D_800BC210[];
extern NAS *D_800BC420[];
NAS *func_800BB688(s32 x, s32 z, s32 range);
extern s32 D_800BC4E8[];

void func_800BA5A0(s32 arg0, s32 arg1);
void func_800B94FC(s32 arg0, u16 arg1);
void func_800B9A50(s32 arg0, u16 arg1);
void func_800B9ED8(s32 arg0, u16 arg1);
void func_800BA220(s32 arg0, u16 arg1);
extern u8 D_8005A614[];

void func_800B6E84(s32 arg0);
extern s32 D_8005A5D8;
extern HeroInfo Heroes[4];
void func_80025F44(EXTengine *eng);
void func_80084E04(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx);

s32 func_80085890();
s32 func_8008A200(s32 arg0);
void func_8008A810(void);

void func_800B44C8(void);

void func_800BA630(NAS *nas);

void InitCoalFlameSFX(CoalFlameObj *arg0);

void InitBarrelGenerator(NAS *nas);

void func_800BADD4(s32 arg0);

void func_800BB3F0(s32 x, s32 y);

#endif /* BASH_OVL68_H */
