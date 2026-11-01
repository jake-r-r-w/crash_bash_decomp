/* The player's settings: chosen in the settings menu, kept in a file in the user's own folder. */
#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct Settings {
    int skip_intro;    /* start at the main menu, without the logos */
    int skip_loading;  /* load in one go, without the loading screen */
    int fullscreen;
    int window_w, window_h;
    int render_w, render_h; /* 0 x 0: as sharp as the window can show. Else what the game draws at, whatever the window's size */
    int vsync;
    int cheat_menu;    /* the original's hidden cheat menu, in the pause menu */
    int fps;           /* 30 is the game's own rate; 60 and 120 draw between its frames */
    int antialias;     /* samples a pixel: 1 is off, or 2, 4 or 8 */
    int smooth;        /* textures filtered, rather than the original's square texels */
    int widescreen;    /* a wider window shows more of the world */
    int upscale;       /* textures made this many times bigger, on the player's machine: 1 is off, or 2 or 4 */
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
} Settings;

/* The settings in use. */
Settings *settings(void);

void settings_defaults(void);
void settings_load(void);
void settings_save(void);

/* The window and renderer take up the settings as soon as they can. */
void settings_apply(void);

#endif
