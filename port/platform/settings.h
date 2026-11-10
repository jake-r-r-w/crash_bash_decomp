/* The player's settings: chosen in the settings menu, kept in a file in the user's own folder. */
#ifndef SETTINGS_H
#define SETTINGS_H

#include "controls.h"

#define SETTINGS_MODS 64
#define SETTINGS_MOD_ID 128

/* A mod as the settings have it: its zip's or folder's name, and whether it's on (mods.c) */
typedef struct ModChoice {
    char id[SETTINGS_MOD_ID];
    int on;
} ModChoice;

typedef struct Settings {
    int skip_intro;    /* start at the main menu, without the logos */
    int skip_loading;  /* load in one go, without the loading screen */
    int fullscreen;
    int window_w, window_h; /* the window's size when it's neither maximised nor fullscreen */
    int maximized;
    int render_w, render_h; /* 0 x 0: as sharp as the window can show. Else what the game draws at, whatever the window's size */
    int vsync;
    int cheat_menu;    /* the original's hidden cheat menu, in the pause menu */
    int fps;           /* 30 is the game's own rate; 60 and 120 draw between its frames */
    int antialias;     /* samples a pixel: 1 is off, or 2, 4 or 8 */
    int smooth;        /* textures filtered, rather than the original's square texels */
    int widescreen;    /* a wider window shows more of the world */
    int upscale;       /* textures made this many times bigger, on the player's machine: 1 is off, or 2 or 4 */
    int icons;         /* the heroes' and bosses' icons made sharp, on the player's machine */
    int extra_heroes;  /* Ripper Roo and the bosses as heroes too, with cards of their own (game/main/extraheroes.c) */
    int keys[NUM_CONTROLS];    /* each game button's key, an SDL scancode (controls.h) */
    int buttons[NUM_CONTROLS]; /* and its controller button, SDL's or a trigger */
    ModChoice mods[SETTINGS_MODS]; /* the mods in the player's order, first first */
    int num_mods;
    char texture_pack[64]; /* from before mods: the texture pack that was on, which becomes a mod that's on (mods.c) */
    int test_mode;     /* a test run's game mode to go straight to, or -1 */
    int test_level_flags; /* and its challenge's flags, as a warp room's door sets them: 0x80 is a crystal's, on the level's other map */
    int testing;       /* went there with --mode: the computer plays, with no time limit */
    int launcher;      /* the settings menu comes first */
    int test_stall;    /* a test run: stop a fight with no progress in this many vblanks */
    int test_aim;      /* a test run: bossbot.c plays pad 1 in a boss fight */
    int test_bot;      /* a test run: the game's own AI plays pad 1 */
    int test_adventure; /* a test run: adventureplayer.c plays the whole Adventure */
    int test_challenge; /* a test run, if not -1: --adventure goes straight to this one, level * 8 + the door's choice */
    int test_cheats;   /* a test run: cheat flags kept on */
    int boss_beat_at;  /* a test run's vblank to mark the boss beaten at, or 0 */
    int no_save;       /* a test run leaves the player's file alone */
    int test_sound;    /* a test run that's for watching: its sound plays (--sound) */
    int tile, tiles;   /* a test window's slot in a grid of them on the screen, if tiles isn't 0 */
    int test_extra[4]; /* a test run: a player's extra hero (EXTRA_, bash/extraheroes.h), or 0 */
    int fingerprints;  /* a test run: print the game's fingerprint every this many steps (game/main/fingerprint.c), or 0 */
    int game_clock_sound; /* sound mixed a vblank at a time by the game's clock, not as the sound device asks, so that voices end at the
                             same step on every machine (audio.c, snd_vblank): online play's */
} Settings;

/* The settings in use. */
Settings *settings(void);

void settings_defaults(void);
void settings_load(void);
void settings_save(void);

/* The window and renderer take up the settings as soon as they can. */
void settings_apply(void);
/* Whether a window of that size fits on the screen it's on, title bar and all. */
int settings_window_fits(int w, int h);

#endif
