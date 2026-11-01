#include "common.h"
#include "bash/main.h"
#include "bash/music.h"

void music_open_seq(music_midi *slot, s32 idx) {
    s16 vol;

    if (slot->handle != -1) {
        if (slot->seq != -1) {
            audio_song_close((s16)slot->seq);
        }
    }
    audio_song_open(slot->songs[idx], (s16)slot->seq);
    slot->handle = idx;
    audio_song_play((s16)slot->seq, 1, 0);
    vol = (GameOptions.music * 0x7F) / 100;
    audio_song_volume((s16)slot->seq, vol, vol);
    MusicVolCur = (GameOptions.music * 0x7F) / 100;
    audio_set_mute(0);
}

void music_update(void) {
    music_midi *slot = &CurrentMusic;
    s32 next = MusicNext;
    s32 cur = MusicCur;
    s32 vol;

    if (next != cur || MusicRestart != 0) {
        s32 t = MusicStopDelay;

        if (t != 0 || cur == -1) {
            MusicStopDelay = t - 1;
            if (MusicStopDelay == 0 || cur == -1) {
                MusicRestart = 0;
                MusicStopDelay = 0;
                if (next != -1) {
                    music_open_seq(slot, next);
                }
                MusicCur = MusicNext;
            }
        } else {
            MusicStopDelay = 10;
            music_fade_out(20);
        }
    } else {
        if (slot->seq >= 0) {
            vol = (GameOptions.music * 0x7F) / 100;
            if (MusicVolCur != vol || MusicDuck != MusicDuckCur) {
                if (MusicDuck != 0) {
                    SeqSlot_SetVol(slot, (GameOptions.music * 0x55) / 100);
                } else {
                    SeqSlot_SetVol(slot, vol);
                }
            }
        }
        MusicDuckCur = MusicDuck;
        MusicVolCur = (GameOptions.music * 0x7F) / 100;
    }
}

void music_init(void) {
    CurrentMusic.data = 0;
    CurrentMusic.seq = -1;
    MusicCur = -1;
    MusicNext = -1;
    MusicVolCur = 0;
}

void music_close(music_midi *slot) {
    if (slot->seq >= 0) {
        audio_song_close(slot->seq);
    }
    slot->handle = -1;
}

void continue_music(void) {
    if (CurrentMusic.seq >= 0) {
        audio_song_resume(CurrentMusic.seq);
    }
}

void music_pause(void) {
    if (CurrentMusic.seq >= 0) {
        audio_song_pause(CurrentMusic.seq);
    }
}

void music_replay(music_midi *arg0) {
    if (arg0->seq >= 0) {
        audio_song_resume(arg0->seq);
    }
}

void music_pause_slot(music_midi *arg0) {
    if (arg0->seq >= 0) {
        audio_song_pause(arg0->seq);
    }
}

void music_set_tempo(s32 arg0, s32 arg1) {
    if (CurrentMusic.seq >= 0) {
        audio_song_tempo((s16)CurrentMusic.seq, (MusicTable[GameMode].tempo * arg0) / 100, arg1);
    }
}

void music_fade_out(s32 arg0) {
    if (CurrentMusic.seq >= 0) {
        audio_song_fade(CurrentMusic.seq, 0x7F, arg0);
    }
}

void music_request(s32 arg0) {
    MusicNext = arg0;
}

void music_duck_off(void) {
    MusicDuck = 0;
}

void music_flag_restart(void) {
    MusicDuck = 1;
}

void music_restart(void) {
    MusicRestart += 1;
}

void music_setup_slot(music_midi *slot) {
    SeqHdr *h = (SeqHdr *)slot->data;
    u8 *head = FILE_AT(h, h->head);
    u8 *body = FILE_AT(h, h->body);

    slot->songs[0] = slot->data + h->o8;
    slot->songs[1] = slot->data + h->oC;
    slot->songs[2] = slot->data + h->o10;
    slot->songs[3] = slot->data + h->o14;
    slot->songs[4] = slot->data + h->o18;
    slot->handle = -1;
    slot->seq = audio_bank_open(head, -1);
    audio_bank_load(body, (s16)slot->seq);
    slot->data = mal_trim_front(slot->data, h->head, &MainHeap);
}

void music_dload_midi(file *arg0, music_midi *slot, s32 type) {
    asload_permanent_callback(arg0, music_set_data, slot);
    audio_reverb_type(type);
    audio_reverb_depth(0, 0);
}

void music_set_data(u8 *data, music_midi *slot) {
    slot->data = data;
    music_setup_slot(slot);
    MusicRestart = 0;
    MusicDuck = 0;
    MusicDuckCur = 0;
}

void music_stop(void) {
    music_close(&CurrentMusic);
}

void music_free(void) {
    if (CurrentMusic.data != 0) {
        mal_free(CurrentMusic.data, &MainHeap);
        CurrentMusic.data = 0;
    }
}

void music_load(file *arg0, s32 type) {
    music_dload_midi(arg0, &CurrentMusic, type);
}

/* data, in address order (tools/scripts/data2c.py) */
s32 MusicNext = 0;
s32 MusicStopDelay = 0;
s32 MusicCur = 0;
s32 MusicRestart = 0;
s32 MusicDuck = 0;
s32 MusicDuckCur = 0;
s32 MusicVolCur = 0;
