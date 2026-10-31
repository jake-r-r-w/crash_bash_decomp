/* The launcher the PC version opens with, built like the game's own menus: PlaceRecs on the titles'
 * backdrop, in its font and panels, under the 3D logo from the intro. Settings take effect as they
 * change. */
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

/* What a row is: a setting, or something that happens when it's chosen. */
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
    ROW_GAME,
    ROW_VIDEO,
    ROW_QUIT,
    ROW_BACK
};

typedef struct {
    const char *label;
    s32 id;
} MenuRow;

typedef struct {
    const char *title;
    const char *hint;
    const MenuRow *rows;
    s32 count;
    s32 wide; /* has settings, with their values beside them */
} MenuPage;

static const MenuRow TopRows[] = { { "PLAY", ROW_PLAY }, { "GAME", ROW_GAME }, { "VIDEO", ROW_VIDEO }, { "QUIT", ROW_QUIT } };
static const MenuRow GameRows[] = { { "SKIP INTRO", ROW_SKIP_INTRO }, { "SKIP LOADING", ROW_SKIP_LOADING }, { "BACK", ROW_BACK } };
static const MenuRow VideoRows[] = {
    { "DISPLAY", ROW_DISPLAY }, { "WINDOW SIZE", ROW_WINDOW }, { "RESOLUTION", ROW_RENDER },
    { "FRAME RATE", ROW_FPS }, { "VSYNC", ROW_VSYNC }, { "BACK", ROW_BACK },
};

enum { PAGE_TOP, PAGE_GAME, PAGE_VIDEO };

static const MenuPage Pages[] = {
    { "", "PRESS UP - DOWN TO CHOOSE, \xFC BUTTON TO SELECT", TopRows, 4, 0 },
    { "GAME", "PRESS UP - DOWN TO CHOOSE, LEFT - RIGHT TO CHANGE", GameRows, 3, 1 },
    { "VIDEO", "PRESS UP - DOWN TO CHOOSE, LEFT - RIGHT TO CHANGE", VideoRows, 6, 1 },
};

/* The logo: two meshes of the intro's model, drawn by PlaceRecs like any model on a menu. */
#define LOGO_MODEL DB_CutsceneIntroLogo2
#define LOGO_TOP 0x2006
#define LOGO_BOTTOM 0x2007
#define LOGO_SCALE 0xD5
#define LOGO_Y -0xAE

/* The subengines, in PlaceRec order. */
#define SUB_LOGO 0
#define SUB_PANEL 2
#define SUB_TITLE 3
#define SUB_HINT 4
#define SUB_ROWS 5
#define SUB_LABEL(n) (SUB_ROWS + (n) * 2)
#define SUB_VALUE(n) (SUB_ROWS + (n) * 2 + 1)

#define MAX_ROWS 6
#define ROW_STEP 0x22
#define ROWS_Y -0x2C
#define LABEL_X -0x100
#define VALUE_X 0x38
#define TITLE_Y -0x5C

static PlaceRec Place[SUB_ROWS + MAX_ROWS * 2 + 1];
static char Values[MAX_ROWS][16];
static s32 Page;
static s32 Cursor;

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

static void change_setting(s32 id, s32 dir) {
    Settings *s = settings();

    switch (id) {
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

static void show_value(char *text, size_t size, s32 id) {
    const Settings *s = settings();

    text[0] = '\0';
    switch (id) {
    case ROW_SKIP_INTRO:
        snprintf(text, size, "%s", s->skip_intro ? "ON" : "OFF");
        break;
    case ROW_SKIP_LOADING:
        snprintf(text, size, "%s", s->skip_loading ? "ON" : "OFF");
        break;
    case ROW_DISPLAY:
        snprintf(text, size, "%s", s->fullscreen ? "FULLSCREEN" : "WINDOW");
        break;
    case ROW_WINDOW:
        snprintf(text, size, "%d X %d", s->window_w, s->window_h);
        break;
    case ROW_RENDER:
        snprintf(text, size, "%d X %d", s->render_w, s->render_h);
        break;
    case ROW_FPS:
        if (s->fps == 30) {
            snprintf(text, size, "30 ORIGINAL");
        } else {
            snprintf(text, size, "%d", s->fps);
        }
        break;
    case ROW_VSYNC:
        snprintf(text, size, "%s", s->vsync ? "ON" : "OFF");
        break;
    }
}

static void show_values(void) {
    const MenuPage *p = &Pages[Page];
    s32 i;

    for (i = 0; i < p->count; i++) {
        show_value(Values[i], sizeof(Values[i]), p->rows[i].id);
    }
}

static void place_text(PlaceRec *r, s32 x, s32 y, const char *text, s32 small) {
    r->type = 3;
    r->x = x;
    r->y = y;
    r->data = (void *)text;
    r->frame = small ? 4 : 0;
}

static void place_logo(PlaceRec *r, s32 entity) {
    r->type = 0xA;
    r->y = LOGO_Y;
    r->scale = LOGO_SCALE;
    r->model = LOGO_MODEL;
    r->player = entity;
}

/* A panel's records are its left, right, top and bottom. */
static void place_panel(PlaceRec *r, s32 half, s32 top, s32 bottom) {
    r->type = 4;
    r->x = -half;
    r->y = half;
    r->z = top;
    r->scale = bottom;
    r->rx = 1;
}

static void build_page(s32 page) {
    const MenuPage *p = &Pages[page];
    s32 i;

    Page = page;
    show_values();
    memset(Place, 0, sizeof(Place));
    place_logo(&Place[SUB_LOGO], LOGO_TOP);
    place_logo(&Place[SUB_LOGO + 1], LOGO_BOTTOM);
    place_panel(&Place[SUB_PANEL], p->wide ? 0x118 : 0xA0, ROWS_Y - 0x10, ROWS_Y + p->count * ROW_STEP + 0xA);
    place_text(&Place[SUB_TITLE], -0x8000, TITLE_Y, p->title, 0);
    place_text(&Place[SUB_HINT], -0x8000, 0xCC, p->hint, 1);
    for (i = 0; i < p->count; i++) {
        s32 y = ROWS_Y + i * ROW_STEP;

        if (p->rows[i].id < NUM_SETTINGS) {
            place_text(&Place[SUB_LABEL(i)], LABEL_X, y, p->rows[i].label, 0);
            place_text(&Place[SUB_VALUE(i)], VALUE_X, y, Values[i], 0);
        } else {
            place_text(&Place[SUB_LABEL(i)], -0x8000, y, p->rows[i].label, 0);
            place_text(&Place[SUB_VALUE(i)], VALUE_X, y, Values[i], 0);
        }
    }
    PlaceEngineSlot(0, Place);
}

static void light_row(s32 row, s32 on) {
    subengine *sub = GetEngineSlot(0)->sub;

    sub[SUB_LABEL(row)].unk7C = on ? 2 : 0;
    sub[SUB_VALUE(row)].unk7C = on ? 2 : 0;
}

static void go_to_page(s32 page, s32 row) {
    s32 i;

    build_page(page);
    for (i = 0; i < Pages[page].count; i++) {
        light_row(i, 0);
    }
    Cursor = row;
    light_row(Cursor, 1);
}

static void start_game(void) {
    settings_save();
    /* the intro plays once, when the game starts */
    if (!settings()->launcher || settings()->skip_intro) {
        settings()->launcher = 0;
        process_control_goto(&MenuProcessControl, &MainMenu);
    } else {
        settings()->launcher = 0;
        FirstBootDone = 1; /* what has the level load play the intro first, as at boot */
        ProcessControl.next_process = &LevelLoadProcess;
    }
}

static void choose(s32 id) {
    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
    switch (id) {
    case ROW_PLAY:
        start_game();
        break;
    case ROW_GAME:
        go_to_page(PAGE_GAME, 0);
        break;
    case ROW_VIDEO:
        go_to_page(PAGE_VIDEO, 0);
        break;
    case ROW_QUIT:
        settings_save();
        platform_finish(0);
        break;
    case ROW_BACK:
        go_to_page(PAGE_TOP, Page == PAGE_GAME ? 1 : 2);
        break;
    }
}

/* The logo sways a little. */
static void sway_logo(void) {
    subengine *sub = GetEngineSlot(0)->sub;
    s32 angle = (rcossin_tbl[(GameFrameCount * 24) & 0xFFF].unk2 * 0x60) >> 12;
    s32 i;

    for (i = SUB_LOGO; i < SUB_LOGO + 2; i++) {
        sub[i].xform.rot.vy = angle;
        sub[i].xform.dirty = 1;
    }
}

static void SettingsMenuInit(void) {
    obj_dbase *logo = &HeroModelBank[LOGO_MODEL];

    dbase_asload_with_anims(logo, NULL);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    go_to_page(PAGE_TOP, 0);
}

static void SettingsMenuUpdate(void) {
    const MenuPage *p = &Pages[Page];
    s32 pressed = Pads[0].pressed;
    s32 id;

    GameFrameCount++;
    AnimateTitlesBackdrop();
    sway_logo();
    light_row(Cursor, 0);
    if ((pressed & 0x40) && Cursor < p->count - 1) {
        Cursor++;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x10) && Cursor > 0) {
        Cursor--;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    light_row(Cursor, 1);
    id = p->rows[Cursor].id;
    if (id < NUM_SETTINGS) {
        if (pressed & (0x20 | 0x4000)) {
            change_setting(id, 1);
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        } else if (pressed & 0x80) {
            change_setting(id, -1);
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        show_values();
    } else if (pressed & (0x4000 | 8)) {
        choose(id);
    }
    if ((pressed & 0x1000) && Page != PAGE_TOP) {
        choose(ROW_BACK);
    }
}

static void SettingsMenuQuit(void) {
    dbase_release(&HeroModelBank[LOGO_MODEL]);
}

process_info SettingsMenu = { SettingsMenuInit, SettingsMenuUpdate, RenderTitlesBackdropAndMenu, SettingsMenuQuit, 0 };
