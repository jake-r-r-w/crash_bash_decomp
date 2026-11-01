/* The game's sound (audio.c): sound banks, the voices playing from them, and songs. */
#ifndef PLATFORM_AUDIO_H
#define PLATFORM_AUDIO_H

#include <stdint.h>

/* A bank's records, as its file holds them. */
typedef struct AudioProgram {
    unsigned char tones, mvol, prior, mode, mpan;
    char reserved0;
    short attr;
    uint32_t reserved1, reserved2;
} AudioProgram;

typedef struct AudioTone {
    unsigned char prior, mode, vol, pan, center, shift, min, max;
    unsigned char vibW, vibT, porW, porT, pbmin, pbmax, reserved1, reserved2;
    unsigned short adsr1, adsr2;
    short prog, vag;
    short reserved[4];
} AudioTone;

/* Every voice off and every bank closed. */
void audio_reset(void);
/* tick is called once a vblank till audio_stop. */
void audio_start(void (*tick)(void));
void audio_stop(void);
void audio_set_mono(int mono);
void audio_set_mute(int mute);
/* The voices the game's effects still hold, which nothing else may take. */
void audio_hold_voices(uint32_t mask);
void audio_reverb_type(short type);
void audio_reverb_depth(short left, short right);

short audio_bank_open(unsigned char *head, short bank);
short audio_bank_load(unsigned char *samples, short bank);
void audio_bank_close(short bank);
short audio_bank_program(short bank, short prog, AudioProgram *out);
short audio_bank_tone(short bank, short prog, short tone, AudioTone *out);

short audio_note_on(short bank, short prog, short tone, short note, short fine, short left, short right);
short audio_voice_off(short voice);
void audio_voices_all_off(void);
short audio_voice_pitch(short voice, short bank, short prog, short old_note, short old_fine, short new_note, short new_fine);
short audio_voice_volume(short voice, short left, short right);
/* 1 for each voice that's playing, as the game counts them. */
void audio_voice_states(char *states);

short audio_song_open(uint32_t *song, short bank);
void audio_song_close(short song);
void audio_song_play(short song, char mode, short count);
void audio_song_pause(short song);
void audio_song_resume(short song);
void audio_song_volume(short song, short left, short right);
void audio_song_tempo(short song, int32_t tempo, int32_t ticks);
void audio_song_fade(short song, short volume, int32_t ticks);

#endif
