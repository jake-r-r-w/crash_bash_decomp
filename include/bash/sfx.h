#ifndef BASH_SFX_H
#define BASH_SFX_H

/* Functions and data defined in src/main/sfx.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/sfx.c */

#include "common.h"
#include "bash/main.h"

void sound_set_stereo(void);
void sound_set_mono(void);
void sound_init(void);
void sound_reinit(void);
void func_800227C0(void);
void func_800227FC(void);
void sfx_update_all(void);
void func_800228A4(_sfx_handler *voice, s32 arg1);
void sfx_reset(void);
void StopSfx(_sfx_handler *voice);
_sfx_handler *PlaySfx(s32 id, _sfx_handler **handle, SLONG_XYZ *pos, s32 arg3, s32 arg4, s32 flags);
void sfx_voice_free(_sfx_handler *voice);
void music_load_track(s32 i);
void sfx_voices_init(void);
_sfx_handler *sfx_voice_alloc(void);
void sfx_delete_all(void);
void music_play_level(s32 arg0, s32 arg1);
s32 sfx_update_voice(_sfx_handler *voice);


#endif /* BASH_SFX_H */
