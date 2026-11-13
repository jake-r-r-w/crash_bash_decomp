/* The video options, for every menu that offers them: the launcher, and Options in the main menu and the pause menu. They take
 * effect as they change, and are kept. To add one: a row in the table, a case in video_option_change and in video_option_value. */
#include <stdio.h>
#include "../../platform/settings.h"
#include "common.h"
#include "bash/videomenu.h"

enum { VIDEO_DISPLAY, VIDEO_WINDOW, VIDEO_RENDER, VIDEO_FPS, VIDEO_VSYNC, VIDEO_ANTIALIAS, VIDEO_TEXTURES, VIDEO_WIDESCREEN, VIDEO_UPSCALE, VIDEO_ICONS, NUM_VIDEO };
_Static_assert(NUM_VIDEO == NUM_VIDEO_OPTIONS, "the menus are laid out for this many");

static const struct {
    const char *label;
    const char *help;
} Options[NUM_VIDEO] = {
    { "DISPLAY", "PLAY IN A WINDOW OR FULLSCREEN" },
#ifdef __ANDROID__
    /* A phone has no window to size, so on Android that row is MOBILE FOLD instead, the user's name for it: the touch buttons apart
       from the picture (platform/touch.c; the user's word, 2026-11-12: an option only Android shows). The menus are laid out for this
       many rows, and this way they still are */
    { "MOBILE FOLD", "THE GAME ON ONE SIDE, THE BUTTONS ON THE OTHER" },
#else
    { "WINDOW SIZE", "THE SIZE OF THE WINDOW ON YOUR SCREEN" },
#endif
    { "RESOLUTION", "THE SIZE THE GAME DRAWS AT" },
    { "FRAME RATE", "30 IS THE ORIGINAL, 60 AND 120 ARE SMOOTHER" },
    { "VSYNC", "MATCH YOUR SCREEN, TO STOP TEARING" },
    { "ANTIALIAS", "SMOOTHS THE EDGES OF THINGS, HIGHER IS SLOWER" },
    { "TEXTURES", "SHARP IS THE ORIGINAL, SMOOTH BLENDS PIXELS" },
    { "WIDESCREEN", "A WIDE WINDOW SHOWS MORE OF THE WORLD" },
    { "UPSCALE", "ROUNDS OFF TEXTURES AND ICONS, ON YOUR PC" },
    { "ICONS", "CRISP SHARPENS THE HERO AND BOSS ICONS" },
};

typedef struct {
    s32 w, h;
} VideoSize;

#ifndef __ANDROID__
static const VideoSize WindowSizes[] = { { 640, 480 }, { 960, 720 }, { 1280, 720 }, { 1280, 960 }, { 1600, 1200 }, { 1920, 1080 },
                                         { 1920, 1440 }, { 2560, 1440 }, { 3840, 2160 } };
#endif
static const VideoSize RenderSizes[] = { { 0, 0 }, { 640, 480 }, { 1280, 960 }, { 1920, 1440 }, { 2560, 1920 }, { 3840, 2880 } };

#define NUM_OF(a) ((s32)(sizeof(a) / sizeof((a)[0])))

s32 video_option_count(void) {
    return NUM_VIDEO;
}

const char *video_option_label(s32 i) {
    return Options[i].label;
}

const char *video_option_help(s32 i) {
    return Options[i].help;
}

#ifndef __ANDROID__
/* The window's sizes that fit on the screen, smallest first, then maximised, and round again. From a size that isn't one of them, the next one bigger or smaller. */
static void step_window(Settings *s, s32 dir) {
    s32 area = s->maximized ? (dir > 0 ? -1 : 0x7FFFFFFF) : s->window_w * s->window_h;
    s32 i, best = -1;

    for (i = 0; i < NUM_OF(WindowSizes); i++) {
        s32 a = WindowSizes[i].w * WindowSizes[i].h;

        if (!settings_window_fits(WindowSizes[i].w, WindowSizes[i].h) || (dir > 0 ? a <= area : a >= area)) {
            continue;
        }
        if (best < 0 || (dir > 0) == (a < WindowSizes[best].w * WindowSizes[best].h)) {
            best = i;
        }
    }
    if (best < 0 && !s->maximized) {
        s->maximized = 1;
        return;
    }
    if (best < 0) {
        /* nothing fits: the smallest anyway */
        best = 0;
    }
    s->window_w = WindowSizes[best].w;
    s->window_h = WindowSizes[best].h;
    s->maximized = 0;
}
#endif

/* Moves through a list of sizes from the one in use, wrapping at the ends. */
static void step_size(const VideoSize *sizes, s32 count, int *w, int *h, s32 dir) {
    s32 i, at = 0;

    for (i = 0; i < count; i++) {
        if (sizes[i].w == *w && sizes[i].h == *h) {
            at = i;
        }
    }
    at = (at + dir + count) % count;
    *w = sizes[at].w;
    *h = sizes[at].h;
}

void video_option_change(s32 i, s32 dir) {
    Settings *s = settings();

    switch (i) {
    case VIDEO_DISPLAY:
        s->fullscreen = !s->fullscreen;
        break;
    case VIDEO_WINDOW:
#ifdef __ANDROID__
        s->buttons_apart = !s->buttons_apart;
#else
        step_window(s, dir);
#endif
        break;
    case VIDEO_RENDER:
        step_size(RenderSizes, NUM_OF(RenderSizes), &s->render_w, &s->render_h, dir);
        break;
    case VIDEO_FPS:
        s->fps = dir > 0 ? (s->fps == 30 ? 60 : s->fps == 60 ? 120 : 30) : (s->fps == 30 ? 120 : s->fps == 120 ? 60 : 30);
        break;
    case VIDEO_VSYNC:
        s->vsync = !s->vsync;
        break;
    case VIDEO_ANTIALIAS:
        s->antialias = dir > 0 ? (s->antialias == 8 ? 1 : s->antialias * 2) : (s->antialias == 1 ? 8 : s->antialias / 2);
        break;
    case VIDEO_TEXTURES:
        s->smooth = !s->smooth;
        break;
    case VIDEO_WIDESCREEN:
        s->widescreen = !s->widescreen;
        break;
    case VIDEO_UPSCALE:
        s->upscale = dir > 0 ? (s->upscale == 4 ? 1 : s->upscale * 2) : (s->upscale == 1 ? 4 : s->upscale / 2);
        break;
    case VIDEO_ICONS:
        s->icons = !s->icons;
        break;
    }
    settings_apply();
    settings_save();
}

void video_option_value(s32 i, char *text, s32 size) {
    const Settings *s = settings();

    switch (i) {
    case VIDEO_DISPLAY:
        snprintf(text, size, "%s", s->fullscreen ? "FULLSCREEN" : "WINDOW");
        break;
    case VIDEO_WINDOW:
#ifdef __ANDROID__
        snprintf(text, size, "%s", s->buttons_apart ? "ON" : "OFF");
        break;
#endif
        if (s->maximized) {
            snprintf(text, size, "MAXIMISED");
        } else {
            snprintf(text, size, "%d X %d", s->window_w, s->window_h);
        }
        break;
    case VIDEO_RENDER:
        if (s->render_w == 0) {
            snprintf(text, size, "MATCH WINDOW");
        } else {
            snprintf(text, size, "%d X %d", s->render_w, s->render_h);
        }
        break;
    case VIDEO_FPS:
        snprintf(text, size, "%d", s->fps);
        break;
    case VIDEO_VSYNC:
        snprintf(text, size, "%s", s->vsync ? "ON" : "OFF");
        break;
    case VIDEO_ANTIALIAS:
        if (s->antialias == 1) {
            snprintf(text, size, "OFF");
        } else {
            snprintf(text, size, "%dX", s->antialias);
        }
        break;
    case VIDEO_TEXTURES:
        snprintf(text, size, "%s", s->smooth ? "SMOOTH" : "SHARP");
        break;
    case VIDEO_WIDESCREEN:
        snprintf(text, size, "%s", s->widescreen ? "ON" : "OFF");
        break;
    case VIDEO_UPSCALE:
        if (s->upscale == 1) {
            snprintf(text, size, "OFF");
        } else {
            snprintf(text, size, "%dX", s->upscale);
        }
        break;
    case VIDEO_ICONS:
        snprintf(text, size, "%s", s->icons ? "CRISP" : "ORIGINAL");
        break;
    }
}
