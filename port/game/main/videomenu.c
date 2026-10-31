/* The video options, for every menu that offers them: the launcher, and Options in the main menu and the pause menu. They take
 * effect as they change, and are kept. To add one: a row in the table, a case in video_option_change and in video_option_value. */
#include <stdio.h>
#include "../../platform/settings.h"
#include "common.h"
#include "bash/videomenu.h"

enum { VIDEO_DISPLAY, VIDEO_WINDOW, VIDEO_RENDER, VIDEO_FPS, VIDEO_VSYNC, NUM_VIDEO };

static const struct {
    const char *label;
    const char *help;
} Options[NUM_VIDEO] = {
    { "DISPLAY", "PLAY IN A WINDOW OR FULLSCREEN" },
    { "WINDOW SIZE", "THE SIZE OF THE WINDOW ON YOUR SCREEN" },
    { "RESOLUTION", "THE SIZE THE GAME DRAWS AT" },
    { "FRAME RATE", "30 IS THE ORIGINAL, 60 AND 120 ARE SMOOTHER" },
    { "VSYNC", "MATCH YOUR SCREEN, TO STOP TEARING" },
};

typedef struct {
    s32 w, h;
} VideoSize;

static const VideoSize WindowSizes[] = { { 640, 480 }, { 960, 720 }, { 1280, 720 }, { 1280, 960 }, { 1600, 1200 }, { 1920, 1080 },
                                         { 1920, 1440 }, { 2560, 1440 }, { 3840, 2160 } };
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
        step_size(WindowSizes, NUM_OF(WindowSizes), &s->window_w, &s->window_h, dir);
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
        snprintf(text, size, "%d X %d", s->window_w, s->window_h);
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
    }
}
