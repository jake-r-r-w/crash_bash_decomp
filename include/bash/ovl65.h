#ifndef BASH_OVL65_H
#define BASH_OVL65_H

/* Functions and data defined in the ovl65 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl65/ovl65.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"

s32 func_800B3760(s32 arg0);

s32 func_800B4C70(s32 *arg0, s32 arg1);

typedef struct Ovl65Ext {
    u8 pad[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Ovl65Ext;

/* Hero NAS extension 0x10000000 func_800B9C10 ticks: a 26-frame timer. */
typedef struct Ovl65HeroExt {
    u8 pad[0x1C];
    s32 timer; /* 0x1C */
} Ovl65HeroExt;

s32 func_800BA810(s32 hero);
void func_800BA960(s32 hero);
void func_800BC820(s32 hero);
typedef struct Ovl65ExtB {
    u8 pad[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    NAS *unk18;
} Ovl65ExtB;

typedef struct Ovl65Pad {
    u8 pad[0x50];
    s32 unk50;
    u8 pad2[4];
} Ovl65Pad;

extern NAS *D_800BDA08[];

void PlotDinoSmoke(NAS *m);

void func_800BC948(HeroInfo *hero);

void func_800BCA18(HeroInfo *hero);

s32 func_800BCDF0(u8 *arg0);

void func_800BCE6C(void);

void func_800B6FE4();
void func_800B7664();
void func_800B85DC();

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

s32 func_800B475C(s32 a, s32 b);

void func_800B9C10(s32 id);

void func_800BC5B0(void);

s32 func_800BC774(s32 arg0);

void func_800BC8D0(s32 arg0);

void func_800BCA38(s32 arg0);

void func_800BCB4C(s32 arg0);

void func_800BCE7C(s32 arg0, s32 arg1);

void func_800BCEFC(void);

void func_800BCF88(s32 arg0);

s32 func_800BD01C(s32 id);

s32 func_800BD258(s32 arg0);

s32 func_800BD320(s32 arg0, s32 arg1);
extern ScriptInit D_800BDC08;

void PlotDinoScratch(NAS *m);

void func_800BCAC0(s32 arg0);

s32 func_800BCCE8(s32 arg0);

void InitDinoSmoke(NAS *arg0);

s32 func_800BD0AC(s32 self, s32 *dist);

s32 func_800BD204(s32 arg0);

#endif /* BASH_OVL65_H */
