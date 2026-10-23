#ifndef BASH_DBASE_H
#define BASH_DBASE_H

/* Functions and data defined in src/main/dbase.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/dbase.c */

#include "common.h"
#include "bash/main.h"

s32 func_800160F8(u8 *arg0, s32 arg1);
void func_8001612C(u8 *arg0, s32 arg1);
void func_80016234(u8 *arg0, u8 *arg1);
void func_800162E4(u8 *arg0, s32 arg1);
void func_800163E0(u8 *arg0, s32 arg1);
u8 *func_80016C18(s32 *arg0, u8 *arg1);
void func_80016D44(u8 *arg0);
void func_80016EF4(u8 *arg0, u8 *arg1);
void func_80017070(void);
void func_80017094(u8 *arg0, s32 arg1, s32 arg2);
void func_80017138(u8 *arg0);
void func_80016D98();
void dbase_asload_with_anims_texture_callback();
void func_80016F34(u8 *arg0, u8 *arg1);
u8 *func_80016AD4(s32 *arg0, u8 *rec);
u8 *func_800160C4(u8 *arg0, s32 i);
s32 func_80016274(s32 *list, u8 *arg1);
void func_8001682C(s32 *arg0);
void func_80016CA0(u8 *rec);
void func_8001636C(u8 *arg0, s32 arg1);
s32 func_80016450(u8 *arg0, s32 arg1);
void func_80016E5C(u8 *arg0, u8 *arg1);
void func_8001702C(SlotList *arg0);
void func_80016F54(u8 *arg0);
void func_800170D4(void);
u8 *func_800167A8(u8 *arg0, u8 *arg1);
u8 *func_80016478(u8 *arg0, u8 *arg1);
s32 dbase_mesh_anim_name_loaded(u8 *arg0, char *arg1);
u8 *func_800165F8(u8 *arg0, u8 *arg1);
s32 func_800166FC(u8 *arg0, u8 *arg1);
u8 *func_80016A20(u8 *arg0, s32 arg1);
u8 *func_80016B48(u8 *arg0, s32 arg1);
void func_80015F60(u8 *arg0, s32 *arg1);


#endif /* BASH_DBASE_H */
