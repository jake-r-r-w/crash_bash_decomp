/* The player's settings: chosen in the launcher, kept in a file in the user's own folder. */
#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct Settings {
    int skip_intro;    /* start at the main menu, without the logos */
    int skip_loading;  /* load in one go, without the loading screen */
    int fullscreen;
    int window_w, window_h;
    int render_w, render_h; /* what the game draws at, whatever the window's size */
    int vsync;
} Settings;

/* The settings in use. */
Settings *settings(void);

void settings_defaults(void);
void settings_load(void);
void settings_save(void);

#endif
