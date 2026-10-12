#ifndef BASH_OVL5F_H
#define BASH_OVL5F_H

/* Functions and data defined in the ovl5f overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5f/ovl5f.c */

#include "common.h"
#include "bash/nas.h"

void func_800C0C68(NAS *nas, s32 id);
void func_800C0F24(NAS *nas, s32 id);

void func_800C68A8(void);
void func_800C69BC(void);
void func_800C7C08(u8 *arg0);
void func_800C7C68(void);
void func_800C7D04(void);
void func_800C81C0(void);
void func_800C831C(void);
void func_800C8324(void);
void func_800C832C(void);
void func_800C83D0(void);
u8 func_800C891C(s32 arg0, s32 arg1);
u8 func_800C8950(s32 arg0, s32 arg1);
void func_800C8984(s32 arg0, s32 arg1);
void func_800C89D8(s32 arg0, s32 arg1);
void func_800C8A40(s32 arg0);

extern u8 D_800CACFC[];
extern s32 D_800CAFF4;
extern u8 D_800CACB4[];
extern u8 D_800CAFFC[];
extern NAS *D_800CB264[8];    /* objects func_800C8598 adds and func_800C8560 removes; func_800C84D8 tests their distance */







void func_800C7D24(HeroInfo *hero);

void func_800C7EF8(HeroInfo *hero);

void func_800C8560(NAS *nas);

void func_800C8598(NAS *nas);

#endif /* BASH_OVL5F_H */
