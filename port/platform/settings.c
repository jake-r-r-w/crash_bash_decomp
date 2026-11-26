#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "folders.h"
#include "prompts.h"
#include "settings.h"

static Settings Current;

static const char *const LanguageIds[] = { "en", "fr", "de", "it", "es" };

const char *settings_language_id(int language) {
    return language >= 0 && language < 5 ? LanguageIds[language] : "en";
}

int settings_language_from_id(const char *id) {
    int i;

    for (i = 0; i < 5; i++) {
        if (strncmp(id, LanguageIds[i], 2) == 0 && (id[2] == 0 || id[2] == '\n' || id[2] == '\r' || id[2] == ' ')) {
            return i;
        }
    }
    return -1;
}

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
    /* as the game starts with */
    Current.online_music = 70;
    Current.online_sfx = 100;
    Current.online_stereo = 1;
    Current.buttons_apart = 0;
    Current.stick_deadzone = 0;
    Current.language = 0;
    Current.button_prompts = PROMPTS_AUTO;
    controls_default_keys(Current.keys);
    controls_default_buttons(Current.buttons);
    Current.texture_pack[0] = 0;
    Current.num_mods = 0;
    Current.test_mode = -1;
}

/* Fills path with where the settings file goes (folders.h). Returns 0 if there's no such place. */
static int settings_path(char *path, size_t size) {
    const char *dir = folders_path(FOLDER_SETTINGS);

    if (dir[0] == '\0') {
        return 0;
    }
    SDL_snprintf(path, size, "%ssettings.ini", dir);
    return 1;
}

/* key_cross=Z or pad_cross=a: SDL's names, a control at a time, each keeping one of its own however the file was written */
static void load_control(const char *line) {
    char name[32], value[64];
    int c, code;

    if (sscanf(line + 4, "%31[^=]=%63[^\r\n]", name, value) != 2) {
        return;
    }
    for (c = 0; c < NUM_CONTROLS; c++) {
        if (strcmp(name, control_file_id(c)) == 0) {
            if (line[0] == 'k' && (code = controls_key_from_id(value)) >= 0) {
                controls_bind(Current.keys, c, code);
            } else if (line[0] == 'p' && (code = controls_button_from_id(value)) >= 0) {
                controls_bind(Current.buttons, c, code);
            }
        }
    }
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
        char word[16];
        char *eq = strchr(line, '=');

        /* "key = value", as another program editing it may write it, is "key=value"; a line starting ; or # is a comment, which
           matches nothing below */
        if (eq != NULL) {
            char *from = eq + 1, *to = eq;

            while (to > line && (to[-1] == ' ' || to[-1] == '\t')) {
                to--;
            }
            while (*from == ' ' || *from == '\t') {
                from++;
            }
            *to++ = '=';
            memmove(to, from, strlen(from) + 1);
        }

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
        } else if (sscanf(line, "buttons_apart=%d", &a) == 1) {
            Current.buttons_apart = a != 0;
        } else if (sscanf(line, "button_prompts=%15s", word) == 1 && prompts_from_id(word) >= 0) {
            Current.button_prompts = prompts_from_id(word);
        } else if (strncmp(line, "language=", 9) == 0 && settings_language_from_id(line + 9) >= 0) {
            Current.language = settings_language_from_id(line + 9);
        } else if (sscanf(line, "stick_deadzone=%d", &a) == 1 && a >= 0 && a <= 90) {
            Current.stick_deadzone = a;
        } else if (sscanf(line, "online_music=%d", &a) == 1 && a >= 0 && a <= 100) {
            Current.online_music = a;
        } else if (sscanf(line, "online_sfx=%d", &a) == 1 && a >= 0 && a <= 100) {
            Current.online_sfx = a;
        } else if (sscanf(line, "online_stereo=%d", &a) == 1) {
            Current.online_stereo = a != 0;
        } else if (sscanf(line, "texture_pack=%63[^\r\n]", Current.texture_pack) == 1) {
            /* that's it */
        } else if (strncmp(line, "mod=", 4) == 0 && Current.num_mods < SETTINGS_MODS) {
            ModChoice *m = &Current.mods[Current.num_mods];
            char state[4];

            if (sscanf(line + 4, "%3s %127[^\r\n]", state, m->id) == 2 && (strcmp(state, "on") == 0 || strcmp(state, "off") == 0)) {
                m->on = strcmp(state, "on") == 0;
                Current.num_mods++;
            }
        } else if (strncmp(line, "key_", 4) == 0 || strncmp(line, "pad_", 4) == 0) {
            load_control(line);
        }
    }
    fclose(f);
}

/* Online, the settings that change the game are the host's on every machine (net.c), for that game only: the player's own are kept
   here, and they're what's saved. */
static struct {
    int on, skip_intro, skip_loading, cheat_menu, extra_heroes;
} Own;

void settings_borrow(int skip_intro, int skip_loading, int cheat_menu, int extra_heroes) {
    if (!Own.on) {
        Own.on = 1;
        Own.skip_intro = Current.skip_intro;
        Own.skip_loading = Current.skip_loading;
        Own.cheat_menu = Current.cheat_menu;
        Own.extra_heroes = Current.extra_heroes;
    }
    Current.skip_intro = skip_intro;
    Current.skip_loading = skip_loading;
    Current.cheat_menu = cheat_menu;
    Current.extra_heroes = extra_heroes;
}

/* The game's over: the player's own again. */
void settings_return(void) {
    if (Own.on) {
        Own.on = 0;
        Current.skip_intro = Own.skip_intro;
        Current.skip_loading = Own.skip_loading;
        Current.cheat_menu = Own.cheat_menu;
        Current.extra_heroes = Own.extra_heroes;
    }
}

static void save_settings(void);

void settings_save(void) {
    Settings now;

    if (!Own.on) {
        save_settings();
        return;
    }
    now = Current;
    Current.skip_intro = Own.skip_intro;
    Current.skip_loading = Own.skip_loading;
    Current.cheat_menu = Own.cheat_menu;
    Current.extra_heroes = Own.extra_heroes;
    save_settings();
    Current = now;
}

static void save_settings(void) {
    char path[1024];
    FILE *f;
    int i;

    if (Current.no_save) {
        return;
    }
    if (!settings_path(path, sizeof(path)) || (f = fopen(path, "w")) == NULL) {
        return;
    }
    /* every option written out, each with what it does (RecompLauncher's Recomp Standards): the game writes the file again whenever a
       setting changes in its menus, so a line added by hand goes then, but a value changed here stays */
    fprintf(f, "; Crash Bash's settings, as its SETTINGS menu sets them. Each can be changed here too, while the game isn't running.\n"
               "; 1 is on and 0 off, unless a line says otherwise.\n\n");
    fprintf(f, "; Start at the main menu, without the logos\nskip_intro=%d\n", Current.skip_intro);
    fprintf(f, "; Load each level in one go, without the loading screen\nskip_loading=%d\n", Current.skip_loading);
    fprintf(f, "; The original's hidden cheat menu, in the pause menu\ncheat_menu=%d\n\n", Current.cheat_menu);
    fprintf(f, "; Full screen, or a window\nfullscreen=%d\n", Current.fullscreen);
    fprintf(f, "; The window's size, width x height, when it's neither full screen nor maximised\nwindow=%dx%d\n", Current.window_w,
            Current.window_h);
    fprintf(f, "; The window maximised\nmaximized=%d\n", Current.maximized);
    fprintf(f, "; What the game draws at, width x height: 0x0 is as sharp as the window can show\nrender=%dx%d\n", Current.render_w,
            Current.render_h);
    fprintf(f, "; Wait for the screen's refresh, so the picture never tears\nvsync=%d\n", Current.vsync);
    fprintf(f, "; Frames a second: 30 is the PlayStation's; 60 and 120 draw between its frames, and the game plays the same\nfps=%d\n",
            Current.fps);
    fprintf(f, "; Anti-aliasing: 1 is off, or 2, 4 or 8 samples a pixel\nantialias=%d\n", Current.antialias);
    fprintf(f, "; Textures filtered, rather than the original's square texels\nsmooth=%d\n", Current.smooth);
    fprintf(f, "; A wide screen shows more of each level at the sides\nwidescreen=%d\n", Current.widescreen);
    fprintf(f, "; Textures made bigger with clean edges: 1 is off, or 2 or 4 times\nupscale=%d\n", Current.upscale);
    fprintf(f, "; The heroes' and bosses' icons made crisp\nicons=%d\n\n", Current.icons);
    fprintf(f, "; Android: the touch buttons in a part of the screen of their own, not over the picture\nbuttons_apart=%d\n",
            Current.buttons_apart);
    fprintf(f, "; The buttons the game's text shows: auto (what player 1 last pressed with), original, xbox, playstation, nintendo, "
               "steamdeck or keyboard\nbutton_prompts=%s\n", prompts_id(Current.button_prompts));
    fprintf(f, "; How far a stick has to move to count, in percent, up to 90: 0 is the game's own\nstick_deadzone=%d\n\n",
            Current.stick_deadzone);
    fprintf(f, "; The European disc's language: en, fr, de, it or es (the USA disc's is English only)\nlanguage=%s\n\n",
            settings_language_id(Current.language));
    fprintf(f, "; Online games: music and effects volumes, 0 to 100, and stereo\nonline_music=%d\nonline_sfx=%d\nonline_stereo=%d\n\n",
            Current.online_music, Current.online_sfx, Current.online_stereo);
    fprintf(f, "; Each game button's key (SDL's name for it, or Mouse Left, Mouse Middle, Mouse Right, Mouse Back or Mouse Forward)\n");
    for (i = 0; i < NUM_CONTROLS; i++) {
        fprintf(f, "key_%s=%s\n", control_file_id(i), controls_key_id(Current.keys[i]));
    }
    fprintf(f, "\n; Each game button's controller button (SDL's name for it, or lefttrigger and righttrigger)\n");
    for (i = 0; i < NUM_CONTROLS; i++) {
        fprintf(f, "pad_%s=%s\n", control_file_id(i), controls_button_id(Current.buttons[i]));
    }
    if (Current.num_mods != 0) {
        fprintf(f, "\n; Mods, on or off, in the order they're applied (the Mods page)\n");
    }
    for (i = 0; i < Current.num_mods; i++) {
        fprintf(f, "mod=%s %s\n", Current.mods[i].on ? "on" : "off", Current.mods[i].id);
    }
    fclose(f);
}

/* Every setting a player can change, for a run's log (crashlog.c): what a report needs to try the same thing. As the game starts, and
   again when the launcher's PLAY or an online game starts it, as the launcher may have changed them. */
void settings_log(const char *when) {
    const Settings *s = &Current;
    char line[2048];
    size_t at;
    int i;

    printf("settings (%s): %s %dx%d%s, render %dx%d, %d FPS, vsync %d, widescreen %d, antialias %d, smooth %d, upscale %d, icons %d\n",
           when, s->fullscreen ? "fullscreen" : "window", s->window_w, s->window_h, s->maximized ? " maximised" : "", s->render_w,
           s->render_h, s->fps, s->vsync, s->widescreen, s->antialias, s->smooth, s->upscale, s->icons);
    printf("settings (%s): skip intro %d, skip loading %d, cheat menu %d, extra heroes %d, prompts %s, stick deadzone %d%%, "
           "touch buttons apart %d, language %s, online sound: music %d, fx %d, %s\n", when, s->skip_intro, s->skip_loading, s->cheat_menu,
           s->extra_heroes, prompts_id(s->button_prompts), s->stick_deadzone, s->buttons_apart, settings_language_id(s->language), s->online_music, s->online_sfx,
           s->online_stereo ? "stereo" : "mono");
    for (at = 0, i = 0; i < NUM_CONTROLS && at < sizeof(line); i++) {
        at += (size_t)snprintf(line + at, sizeof(line) - at, "%s%s %s", i ? ", " : "", control_file_id(i), controls_key_id(s->keys[i]));
    }
    printf("settings (%s): keys: %s\n", when, line);
    for (at = 0, i = 0; i < NUM_CONTROLS && at < sizeof(line); i++) {
        at += (size_t)snprintf(line + at, sizeof(line) - at, "%s%s %s", i ? ", " : "", control_file_id(i),
                               controls_button_id(s->buttons[i]));
    }
    printf("settings (%s): controller buttons: %s\n", when, line);
    line[0] = 0;
    for (at = 0, i = 0; i < s->num_mods && at < sizeof(line); i++) {
        at += (size_t)snprintf(line + at, sizeof(line) - at, "%s%s %s", i ? ", " : "", s->mods[i].id, s->mods[i].on ? "on" : "off");
    }
    printf("settings (%s): mods: %s\n", when, s->num_mods != 0 ? line : "none");
}
