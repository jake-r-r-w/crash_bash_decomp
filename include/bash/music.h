#ifndef BASH_MUSIC_H
#define BASH_MUSIC_H

/* Functions and data defined in src/main/music.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/music.c */

#include "common.h"
#include "bash/main.h"

void music_dload_midi(s32 arg0, music_midi *slot, s32 type);
void music_request(s32 arg0);
void music_duck_off(void);
void music_flag_restart(void);
void music_restart(void);
void music_set_data(s32 arg0, s32 *arg1);
void music_stop(void);
void music_update(void);
void music_free(void);
void music_load(s32 arg0, s32 type);
void music_replay(music_midi *arg0);
void music_pause_slot(music_midi *arg0);
void continue_music(void);
void music_pause(void);
void music_init(void);
void music_fade_out(s32 arg0);
void music_close(music_midi *slot);
void music_set_tempo(s32 arg0, s32 arg1);
void music_setup_slot(music_midi *slot);
void music_open_seq(music_midi *slot, s32 idx);

static inline void SeqSlot_SetVol(music_midi *slot, s32 vol) {
    SsSeqSetVol((s16)slot->seq, (s16)vol, (s16)vol);
}

#endif /* BASH_MUSIC_H */
