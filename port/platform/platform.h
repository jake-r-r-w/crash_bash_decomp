/* What the PC build asks of its host. sdk/ is written on these and never talks to SDL itself. */
#ifndef PC_PLATFORM_H
#define PC_PLATFORM_H

#include <stdint.h>

/* The pad buttons, as bits of platform_pad's result: the PS1's own order. */
#define PAD_SELECT 0x0001
#define PAD_START 0x0008
#define PAD_UP 0x0010
#define PAD_RIGHT 0x0020
#define PAD_DOWN 0x0040
#define PAD_LEFT 0x0080
#define PAD_L2 0x0100
#define PAD_R2 0x0200
#define PAD_L1 0x0400
#define PAD_R1 0x0800
#define PAD_TRIANGLE 0x1000
#define PAD_CIRCLE 0x2000
#define PAD_CROSS 0x4000
#define PAD_SQUARE 0x8000

/* main.c */
const char *platform_data_path(void);
const char *platform_card_path(void);
/* A pad, as the game's libpad reads it: its buttons, or -1 when there's none. */
int platform_pad(int port);
int platform_pad_sticks(int port, unsigned char sticks[4]);
int platform_pad_rumbles(int port);
void platform_rumble(int port, int small_on, int big);
void platform_present(const uint32_t *pixels, int w, int h);
void platform_vblank(void);
int platform_pad(int port); /* the buttons held, or -1 with no pad */
void platform_shot(const char *path);
void platform_finish(int code);

/* The window, sound and input: sdl.c, or nothing in a build without SDL. */
/* The window, opened once and used by the game. Returns NULL if there's none. */
void *host_window(void);
int host_init(void);
void host_present(const uint32_t *pixels, int w, int h);
void host_wait_vblank(void);
int host_poll(void); /* 0: the window was closed */
int host_pad(int port);
int host_pad_sticks(int port, unsigned char sticks[4]); /* 0: no sticks. Else right x, y, left x, y, 0 to 0xFF, centred at 0x80. */
int host_pad_rumbles(int port);
void host_rumble(int port, int small_on, int big);
/* Held while anything changes what the sound device's thread is playing. */
void host_audio_lock(void);
void host_audio_unlock(void);

/* The sound: audio.c. */
void audio_mix(int16_t *out, int frames);
void audio_record(const char *path); /* a test run's sound, to a .wav */
void audio_finish(void);
void host_quit(void);

#endif
