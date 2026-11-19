/* Recomp Achievements, RecompLauncher's (docs/PORT.md, Achievements): the game's events, save loads and "now playing" line, sent to the
 * launcher over the local pipe it names in RL_SESSION, by its own library (rl_achievements, fetched by CMakeLists.txt). The game never
 * talks to accounts or servers, and outside RecompLauncher, or on Android, where there's no RecompLauncher and the library isn't built,
 * every call does nothing. Nothing here changes the game: the observer that fires the events only reads it. */
#include "achievements.h"

#ifdef BASH_ACHIEVEMENTS
#include <stddef.h>
#include <rl_achievements.h>

static int On;

void achievements_start(void) {
    rl_ach_set_info("crash-bash-decomp", BASH_VERSION);
    On = rl_ach_init("", NULL, NULL); /* events only: the port's code fires them, there's no console memory to read */
}

int achievements_on(void) {
    return On && rl_ach_connected();
}

void achievements_frame(void) {
    if (On) {
        rl_ach_frame();
    }
}

void achievements_event(const char *name) {
    if (On) {
        rl_ach_event(name);
    }
}

void achievements_save_loaded(void) {
    if (On) {
        rl_ach_save_loaded();
    }
}

void achievements_casual(const char *reason) {
    if (On) {
        rl_ach_casual(reason);
    }
}

void achievements_presence(const char *text) {
    if (On) {
        rl_ach_presence(text);
    }
}
#else
void achievements_start(void) {
}

int achievements_on(void) {
    return 0;
}

void achievements_frame(void) {
}

void achievements_event(const char *name) {
    (void)name;
}

void achievements_save_loaded(void) {
}

void achievements_casual(const char *reason) {
    (void)reason;
}

void achievements_presence(const char *text) {
    (void)text;
}
#endif
