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
    int fps;           /* 30 is the game's own rate; 60 and 120 draw between its frames */
    int test_mode;     /* a test run's game mode to go straight to, or -1 */
    int testing;       /* went there with --mode: the computer plays, with no time limit */
    int launcher;      /* the settings menu comes first */
    int no_save;       /* a test run leaves the player's file alone */
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
