#ifndef BASH_OVL65_H
#define BASH_OVL65_H

/* Functions and data defined in the ovl65 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl65/ovl65.c */

#include "common.h"
#include "bash/nas.h"

s32 func_800B3760(s32 arg0);

s32 func_800B4C70(s32 *arg0, s32 arg1);

typedef struct Ovl65Ext {
    u8 pad[0xC];
    s32 unkC;
} Ovl65Ext;

extern NAS *D_800BDA08[];

void PlotDinoSmoke(NAS *m);

void func_800BC948(HeroInfo *hero);

void func_800BCA18(HeroInfo *hero);

s32 func_800BCDF0(u8 *arg0);

void func_800BCE6C(void);

void func_800BCE74(void);

void func_800BCF44(s32 arg0);

void func_800BD08C(void);

s32 func_800BD1B8(s32 *arg0, s32 arg1);

void func_800BD1FC(void);

s32 func_800BD2B8(s32 arg0);

s32 func_800BD408(s32 arg0);

s32 func_800BD488(u8 *arg0, u8 *arg1);

extern s32 D_800BD9FC;
extern s32 D_800BDA00;
extern s32 D_800BDA04;

void func_800BC5B0(void);

void func_800BCB4C(s32 arg0);

#endif /* BASH_OVL65_H */
