#ifndef BASH_SFX_H
#define BASH_SFX_H

/* Functions and data defined in src/main/sfx.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/sfx.c */

#include "common.h"
#include "bash/main.h"

void func_800222BC(void);
void func_800222DC(void);
void func_800222FC(void);
void func_8002232C(void);
void func_800227C0(void);
void func_800227FC(void);
void func_80022838(void);
void func_800228A4(_sfx_handler *voice, s32 arg1);
void func_800229D4(void);
void StopSfx(_sfx_handler *voice);
_sfx_handler *PlaySfx(s32 id, _sfx_handler **handle, SLONG_XYZ *pos, s32 arg3, s32 arg4, s32 flags);
void func_800228C4(_sfx_handler *voice);
void func_800229FC(s32 i);
void func_80022984(void);
_sfx_handler *func_80022924(void);
void sfx_delete_all(void);
void func_80022140(s32 arg0, s32 arg1);
s32 func_8002235C(_sfx_handler *voice);


#endif /* BASH_SFX_H */
