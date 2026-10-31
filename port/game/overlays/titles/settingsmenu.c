/* The settings menu the PC version opens with, built like the game's own menus: PlaceRecs on the
 * titles' backdrop, in its font and panels. Everything here takes effect as it is changed. */
#include <stdio.h>
#include "../../../platform/settings.h"
#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/titles_ovl.h"
#include "bash/titles.h"

void platform_finish(int code);

typedef struct {
    s32 w, h;
} MenuSize;

static const MenuSize WindowSizes[] = { { 640, 480 }, { 960, 720 }, { 1280, 960 }, { 1600, 1200 }, { 1920, 1440 } };
static const MenuSize RenderSizes[] = { { 640, 480 }, { 1280, 960 }, { 1920, 1440 }, { 2560, 1920 }, { 3840, 2880 } };

enum {
    ROW_SKIP_INTRO,
    ROW_SKIP_LOADING,
    ROW_DISPLAY,
    ROW_WINDOW,
    ROW_RENDER,
    ROW_FPS,
    ROW_VSYNC,
    NUM_SETTINGS,
    ROW_PLAY = NUM_SETTINGS,
    ROW_QUIT,
    NUM_ROWS
};

/* The subengines, in PlaceRec order. */
#define SUB_LABEL 4
#define SUB_VALUE (SUB_LABEL + NUM_SETTINGS)
#define SUB_PLAY (SUB_VALUE + NUM_SETTINGS)
#define SUB_QUIT (SUB_PLAY + 1)

#define ROW_Y(n) (-0x88 + (n) * 0x1C)
#define LABEL_X -0x118
#define VALUE_X 0x38

static char Values[NUM_SETTINGS][16];
static s32 Cursor;

static PlaceRec SettingsPlace[] = {
    { 4, -0x12C, 0x12C, -0x96, 0x3E, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x8000, 0x190, 0x4C, 0x90, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x8000, 0x258, 0x9A, 0xC0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x15E, 0x258, -0xD0, -0xA6, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, LABEL_X, ROW_Y(0), 0, 0, 0, 0, 0, 0, "SKIP INTRO", 0, 0, 0, 0 },
    { 3, LABEL_X, ROW_Y(1), 0, 0, 0, 0, 0, 0, "SKIP LOADING", 0, 0, 0, 0 },
    { 3, LABEL_X, ROW_Y(2), 0, 0, 0, 0, 0, 0, "DISPLAY", 0, 0, 0, 0 },
    { 3, LABEL_X, ROW_Y(3), 0, 0, 0, 0, 0, 0, "WINDOW SIZE", 0, 0, 0, 0 },
    { 3, LABEL_X, ROW_Y(4), 0, 0, 0, 0, 0, 0, "RESOLUTION", 0, 0, 0, 0 },
    { 3, LABEL_X, ROW_Y(5), 0, 0, 0, 0, 0, 0, "FRAME RATE", 0, 0, 0, 0 },
    { 3, LABEL_X, ROW_Y(6), 0, 0, 0, 0, 0, 0, "VSYNC", 0, 0, 0, 0 },
    { 3, VALUE_X, ROW_Y(0), 0, 0, 0, 0, 0, 0, Values[0], 0, 0, 0, 0 },
    { 3, VALUE_X, ROW_Y(1), 0, 0, 0, 0, 0, 0, Values[1], 0, 0, 0, 0 },
    { 3, VALUE_X, ROW_Y(2), 0, 0, 0, 0, 0, 0, Values[2], 0, 0, 0, 0 },
    { 3, VALUE_X, ROW_Y(3), 0, 0, 0, 0, 0, 0, Values[3], 0, 0, 0, 0 },
    { 3, VALUE_X, ROW_Y(4), 0, 0, 0, 0, 0, 0, Values[4], 0, 0, 0, 0 },
    { 3, VALUE_X, ROW_Y(5), 0, 0, 0, 0, 0, 0, Values[5], 0, 0, 0, 0 },
    { 3, VALUE_X, ROW_Y(6), 0, 0, 0, 0, 0, 0, Values[6], 0, 0, 0, 0 },
    { 3, -0x8000, 0x54, 0, 0, 0, 0, 0, 0, "PLAY", 0, 0, 0, 0 },
    { 3, -0x8000, 0x70, 0, 0, 0, 0, 0, 0, "QUIT", 0, 0, 0, 0 },
    { 3, -0x8000, 0xA5, 0, 0, 0, 0, 0, 0, "PRESS UP - DOWN TO CHOOSE, LEFT - RIGHT TO CHANGE", 4, 0, 0, 0 },
    { 3, -0x8000, -0xCA, 0, 0, 0, 0, 0, 0, "SETTINGS", 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

/* Moves through a list of sizes from the one in use, wrapping at the ends. */
static void step_size(const MenuSize *sizes, s32 count, int *w, int *h, s32 dir) {
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

static void change_row(s32 row, s32 dir) {
    Settings *s = settings();

    switch (row) {
    case ROW_SKIP_INTRO:
        s->skip_intro = !s->skip_intro;
        break;
    case ROW_SKIP_LOADING:
        s->skip_loading = !s->skip_loading;
        break;
    case ROW_DISPLAY:
        s->fullscreen = !s->fullscreen;
        break;
    case ROW_WINDOW:
        step_size(WindowSizes, sizeof(WindowSizes) / sizeof(WindowSizes[0]), &s->window_w, &s->window_h, dir);
        break;
    case ROW_RENDER:
        step_size(RenderSizes, sizeof(RenderSizes) / sizeof(RenderSizes[0]), &s->render_w, &s->render_h, dir);
        break;
    case ROW_FPS:
        s->fps = dir > 0 ? (s->fps == 30 ? 60 : s->fps == 60 ? 120 : 30) : (s->fps == 30 ? 120 : s->fps == 120 ? 60 : 30);
        break;
    case ROW_VSYNC:
        s->vsync = !s->vsync;
        break;
    }
    settings_apply();
}

static void show_values(void) {
    const Settings *s = settings();

    snprintf(Values[ROW_SKIP_INTRO], sizeof(Values[0]), "%s", s->skip_intro ? "ON" : "OFF");
    snprintf(Values[ROW_SKIP_LOADING], sizeof(Values[0]), "%s", s->skip_loading ? "ON" : "OFF");
    snprintf(Values[ROW_DISPLAY], sizeof(Values[0]), "%s", s->fullscreen ? "FULLSCREEN" : "WINDOW");
    snprintf(Values[ROW_WINDOW], sizeof(Values[0]), "%d X %d", s->window_w, s->window_h);
    snprintf(Values[ROW_RENDER], sizeof(Values[0]), "%d X %d", s->render_w, s->render_h);
    if (s->fps == 30) {
        snprintf(Values[ROW_FPS], sizeof(Values[0]), "30 ORIGINAL");
    } else {
        snprintf(Values[ROW_FPS], sizeof(Values[0]), "%d", s->fps);
    }
    snprintf(Values[ROW_VSYNC], sizeof(Values[0]), "%s", s->vsync ? "ON" : "OFF");
}

/* The sub of a row, and its value's if it has one, lit or not. */
static void light_row(s32 row, s32 on) {
    subengine *sub = GetEngineSlot(0)->sub;
    s32 colour = on ? 2 : 0;

    if (row < NUM_SETTINGS) {
        sub[SUB_LABEL + row].unk7C = colour;
        sub[SUB_VALUE + row].unk7C = colour;
    } else {
        sub[SUB_PLAY + row - ROW_PLAY].unk7C = colour;
    }
}

static void SettingsMenuInit(void) {
    show_values();
    Cursor = ROW_PLAY;
    PlaceEngineSlot(0, SettingsPlace);
    light_row(Cursor, 1);
}

static void SettingsMenuUpdate(void) {
    s32 pressed = Pads[0].pressed;

    AnimateTitlesBackdrop();
    light_row(Cursor, 0);
    if ((pressed & 0x40) && Cursor < NUM_ROWS - 1) {
        Cursor++;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x10) && Cursor > 0) {
        Cursor--;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    light_row(Cursor, 1);
    if (Cursor < NUM_SETTINGS) {
        if (pressed & (0x20 | 0x4000)) {
            change_row(Cursor, 1);
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        } else if (pressed & 0x80) {
            change_row(Cursor, -1);
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        show_values();
    } else if (pressed & (0x4000 | 8)) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        settings_save();
        if (Cursor == ROW_QUIT) {
            platform_finish(0);
        }
        settings()->launcher = 0;
        if (settings()->skip_intro) {
            process_control_goto(&MenuProcessControl, &MainMenu);
        } else {
            FirstBootDone = 1; /* what has the level load play the intro first, as at boot */
            ProcessControl.next_process = &LevelLoadProcess;
        }
    }
}

process_info SettingsMenu = { SettingsMenuInit, SettingsMenuUpdate, RenderTitlesBackdropAndMenu, 0, 0 };
