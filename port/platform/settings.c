#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "settings.h"

static Settings Current;

Settings *settings(void) {
    return &Current;
}

void settings_defaults(void) {
    Current.skip_intro = 0;
    Current.skip_loading = 0;
    Current.fullscreen = 0;
    Current.window_w = 1280;
    Current.window_h = 960;
    Current.maximized = 0;
    Current.render_w = 0;
    Current.render_h = 0;
    Current.vsync = 1;
    Current.fps = 30;
    Current.antialias = 1;
    Current.smooth = 0;
#ifdef SDL_PLATFORM_ANDROID
    /* a phone held sideways is filled, the touch buttons over the picture, as phones' games are (the user's word, 2026-11-09) */
    Current.widescreen = 1;
#else
    Current.widescreen = 0;
#endif
    Current.upscale = 1;
    Current.icons = 0;
    Current.extra_heroes = 0;
    Current.texture_pack[0] = 0;
    Current.num_mods = 0;
    Current.test_mode = -1;
}

/* Fills path with where the settings file goes. Returns 0 if there's no such place. */
static int settings_path(char *path, size_t size) {
    char *dir = SDL_GetPrefPath("CrashBash", "CrashBash");

    if (dir == NULL) {
        return 0;
    }
    SDL_snprintf(path, size, "%ssettings.ini", dir);
    SDL_free(dir);
    return 1;
}

void settings_load(void) {
    char path[1024], line[256];
    FILE *f;

    settings_defaults();
    if (!settings_path(path, sizeof(path)) || (f = fopen(path, "r")) == NULL) {
        return;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        int a, b;

        if (sscanf(line, "skip_intro=%d", &a) == 1) {
            Current.skip_intro = a != 0;
        } else if (sscanf(line, "skip_loading=%d", &a) == 1) {
            Current.skip_loading = a != 0;
        } else if (sscanf(line, "fullscreen=%d", &a) == 1) {
            Current.fullscreen = a != 0;
        } else if (sscanf(line, "window=%dx%d", &a, &b) == 2 && a > 0 && b > 0) {
            Current.window_w = a;
            Current.window_h = b;
        } else if (sscanf(line, "maximized=%d", &a) == 1) {
            Current.maximized = a != 0;
        } else if (sscanf(line, "render=%dx%d", &a, &b) == 2 && a >= 0 && b >= 0) {
            Current.render_w = a;
            Current.render_h = b;
        } else if (sscanf(line, "cheat_menu=%d", &a) == 1) {
            Current.cheat_menu = a != 0;
        } else if (sscanf(line, "vsync=%d", &a) == 1) {
            Current.vsync = a != 0;
        } else if (sscanf(line, "fps=%d", &a) == 1 && (a == 30 || a == 60 || a == 120)) {
            Current.fps = a;
        } else if (sscanf(line, "antialias=%d", &a) == 1 && (a == 1 || a == 2 || a == 4 || a == 8)) {
            Current.antialias = a;
        } else if (sscanf(line, "smooth=%d", &a) == 1) {
            Current.smooth = a != 0;
        } else if (sscanf(line, "widescreen=%d", &a) == 1) {
            Current.widescreen = a != 0;
        } else if (sscanf(line, "upscale=%d", &a) == 1 && (a == 1 || a == 2 || a == 4)) {
            Current.upscale = a;
        } else if (sscanf(line, "icons=%d", &a) == 1) {
            Current.icons = a != 0;
        } else if (sscanf(line, "extra_heroes=%d", &a) == 1) {
            Current.extra_heroes = a != 0;
        } else if (sscanf(line, "texture_pack=%63[^\r\n]", Current.texture_pack) == 1) {
            /* that's it */
        } else if (strncmp(line, "mod=", 4) == 0 && Current.num_mods < SETTINGS_MODS) {
            ModChoice *m = &Current.mods[Current.num_mods];
            char state[4];

            if (sscanf(line + 4, "%3s %127[^\r\n]", state, m->id) == 2 && (strcmp(state, "on") == 0 || strcmp(state, "off") == 0)) {
                m->on = strcmp(state, "on") == 0;
                Current.num_mods++;
            }
        }
    }
    fclose(f);
}

void settings_save(void) {
    char path[1024];
    FILE *f;
    int i;

    if (Current.no_save) {
        return;
    }
    if (!settings_path(path, sizeof(path)) || (f = fopen(path, "w")) == NULL) {
        return;
    }
    fprintf(f, "skip_intro=%d\n", Current.skip_intro);
    fprintf(f, "skip_loading=%d\n", Current.skip_loading);
    fprintf(f, "fullscreen=%d\n", Current.fullscreen);
    fprintf(f, "window=%dx%d\n", Current.window_w, Current.window_h);
    fprintf(f, "maximized=%d\n", Current.maximized);
    fprintf(f, "render=%dx%d\n", Current.render_w, Current.render_h);
    fprintf(f, "cheat_menu=%d\n", Current.cheat_menu);
    fprintf(f, "vsync=%d\n", Current.vsync);
    fprintf(f, "fps=%d\n", Current.fps);
    fprintf(f, "antialias=%d\n", Current.antialias);
    fprintf(f, "smooth=%d\n", Current.smooth);
    fprintf(f, "widescreen=%d\n", Current.widescreen);
    fprintf(f, "upscale=%d\n", Current.upscale);
    fprintf(f, "icons=%d\n", Current.icons);
    fprintf(f, "extra_heroes=%d\n", Current.extra_heroes);
    for (i = 0; i < Current.num_mods; i++) {
        fprintf(f, "mod=%s %s\n", Current.mods[i].on ? "on" : "off", Current.mods[i].id);
    }
    fclose(f);
}
