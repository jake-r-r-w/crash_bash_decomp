/* What the PC build asks of its host. sdk/ is written on these and never talks to SDL itself. */
#ifndef PC_PLATFORM_H
#define PC_PLATFORM_H

#include <stddef.h>
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
/* The program's folder, ending in a slash (on Android the app's folder in shared storage), or NULL. */
const char *platform_base_path(void);
void archive_open(void);
void archive_read(int sector, int count, void *buf);
/* A mod's files in place of the archive's (archive.c): how many entries it has and how big one is as the game has it, a replacement
   (bytes it takes, or NULL for the game's own), and laying the archive out again for what's replaced, before the game reads on. */
int archive_entries(void);
int archive_entry_size(int entry);
void archive_replace(int entry, unsigned char *bytes, int size);
void archive_replace_none(void);
void archive_lay_out(void);
/* The game's own bytes of an entry, to free, for --dump-files. */
unsigned char *archive_own_file(int entry, int *size);
/* --log-files: each read says which files it's of, for finding what to put a mod's in place of. */
void archive_log_reads(void);
void platform_wait_vblanks(int count);
void platform_fatal(const char *message);
/* The game's archive from a disc image: what went wrong, or NULL with it at out (discimage.c). */
const char *disc_extract(const char *image, const char *out);
/* Looks for a disc image next to the program and in the current folder. 1: the data's in place. Otherwise problem says what was wrong, if
   anything was found. */
int disc_find_beside(const char *out, const char **problem);
/* Asks the player for their disc image, with first as what's wrong, if it's not NULL. 0: they'd rather not. */
int disc_first_run(const char *out, const char *first);
/* The player's other discs, for bonus content (game/main/bonusheroes.c): an image (.cue with its .bin, .bin, .iso or .img) opened, its
 * volume's name (the game's code, as SCUS-94900), and a file read whole ("S1/S0000017.NSF", to free with SDL_free; NULL if it has
 * none). disc_image_find looks in the discs folders (beside the program, in the user's folder, here) and the folders in them. */
typedef struct DiscImage DiscImage;
DiscImage *disc_image_open(const char *image);
const char *disc_image_volume(const DiscImage *di);
void *disc_image_read(DiscImage *di, const char *path, size_t *size);
void disc_image_close(DiscImage *di);
DiscImage *disc_image_find(const char *volume);
/* A pad, as the game's libpad reads it: its buttons, or -1 when there's none. */
int platform_pad(int port);
int platform_vblanks(void);
int platform_pad_sticks(int port, unsigned char sticks[4]);
int platform_pad_rumbles(int port);
void platform_rumble(int port, int small_on, int big);
void platform_present(const uint32_t *pixels, int w, int h);
void platform_vblank(void);
int platform_pad(int port); /* the buttons held, or -1 with no pad */
/* This machine's own pads, whatever the game's are: online the game's are everyone's (net.c). */
int platform_local_pad(int port);
int platform_local_pad_sticks(int port, unsigned char sticks[4]);
int platform_local_pad_rumbles(int port);
/* What the mouse presses on this machine's first pad this step, in the game's menus (game/main/menumouse.c): held with its own. */
void platform_mouse_buttons(int buttons);
/* How many players an online game has, on all its machines together, or 0 when the game isn't one: the same on every machine in it,
   and in a recording of it played again. */
int platform_online_players(void);
/* Before the game reads the pads, once a step, and after the step with the game's fingerprint (game/main/fingerprint.c). */
void platform_pad_step(void);
void platform_step_done(uint32_t fingerprint);
/* While the game waits on something outside it: the window still answers. 0: it's been closed. */
int platform_idle(void);
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
/* The window's icon, w x h colours (red in the low byte, alpha in the top): the memory card's, from the game's own font (saveicon.c). */
void host_set_icon(const uint32_t *rgba, int w, int h);
/* Held while anything changes what the sound device's thread is playing. */
void host_audio_lock(void);
void host_audio_unlock(void);
/* Sound by the game's clock (audio.c, snd_vblank): a vblank's worth of mixed sound, left then right, for the device to play next. */
void host_audio_queue(const int16_t *frames, int count);

/* The sound: audio.c. */
void audio_mix(int16_t *out, int frames);
void audio_record(const char *path); /* a test run's sound, to a .wav */
void audio_finish(void);
/* For the game starting again from its beginning in the same window (main.c, platform_restart): each part of the platform that the
   game reads from as it was when the program started. */
void audio_restart(void);
void clock_restart(void);
void pad_restart(void);
void overlays_restart(void);
/* The game again from its very beginning, in this window, as if the program had just started: an online game's start, the same on
   every machine whatever went before (online: with the game's own files only, no mod's). It doesn't return. */
void platform_restart(int online);
/* An online game from the launcher (net.c's lobby says everyone's ready): the game starts again from its beginning, as it does on
   every machine. And its end, back in the launcher, with why if it wasn't the players' doing (shown there), or NULL. A game from the
   command line ends the program instead, saying why. Neither returns. */
void platform_online_start(void);
void platform_online_stop(const char *why);
/* What the launcher's to say as it opens, once: why the last online game ended, or NULL. */
const char *platform_launcher_message(void);
/* Sound by the game's clock, or as the device asks (sdl.c): changed as an online game starts and ends. */
void host_audio_by_clock(int by_clock);
/* A character typed since this was last asked: a capital letter, a digit, a dot, a dash, 8 for backspace, 27 for Esc, or 0 for none. */
int host_typed(void);
void host_quit(void);

#endif
