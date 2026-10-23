#ifndef BASH_MUSIC_H
#define BASH_MUSIC_H

/* Functions and data defined in src/main/music.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/music.c */

#include "common.h"
#include "bash/main.h"

void music_dload_midi(s32 arg0, SeqSlot *slot, s32 type);
void func_8002C284(s32 arg0);
void music_duck_off(void);
void func_8002C29C(void);
void music_restart(void);
void func_8002C3F0(s32 arg0, s32 *arg1);
void func_8002C428(void);
void func_8002BEA8(void);
void func_8002C44C(void);
void func_8002C488(s32 arg0, s32 type);
void func_8002C158(u8 *arg0);
void func_8002C18C(u8 *arg0);
void continue_music(void);
void func_8002C11C(void);
void func_8002C060(void);
void music_fade_out(s32 arg0);
void func_8002C090(SeqSlot *slot);
void func_8002C1C0(s32 arg0, s32 arg1);
void func_8002C2C4(SeqSlot *slot);
void func_8002BD98(SeqSlot *slot, s32 idx);

static inline void SeqSlot_SetVol(SeqSlot *slot, s32 vol) {
    SsSeqSetVol((s16)slot->seq, (s16)vol, (s16)vol);
}

#endif /* BASH_MUSIC_H */
