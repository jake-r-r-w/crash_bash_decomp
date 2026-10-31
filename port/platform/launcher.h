/* The settings menu shown in the game's window before the game starts. */
#ifndef LAUNCHER_H
#define LAUNCHER_H

/* Shows the menu in this window until the player chooses. Returns 1 to play, 0 to quit. The settings are saved on play. */
int launcher_run(void *window);

/* The same with the keys already chosen, for testing: u, d, l and r to move, e to confirm. Nothing is saved. */
int launcher_run_keys(void *window, const char *keys);

/* Saves a picture of the menu as a PNG without waiting for anyone. Returns 0 if it couldn't. */
int launcher_screenshot(void *window, const char *path);

#endif
