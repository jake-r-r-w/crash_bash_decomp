/* The launcher the PC version opens with, built like the game's own menus: PlaceRecs on the titles'
 * backdrop, in its font and panels, under the 3D logo from the intro. PLAY, SETTINGS and QUIT, and
 * the settings a screen down: categories on the left, a scrolling list of options on the right.
 * Settings take effect as they change. To add one: a row in its category's table, a case in
 * change_setting and in show_value. */
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
    SET_SKIP_INTRO,
    SET_SKIP_LOADING,
    SET_DISPLAY,
    SET_WINDOW,
    SET_RENDER,
    SET_FPS,
    SET_VSYNC
};

typedef struct {
    const char *label;
    const char *help;
    s32 id;
} MenuOption;

typedef struct {
    const char *label;
    const MenuOption *options;
    s32 count;
} MenuCategory;

static const MenuOption GameOptionsList[] = {
    { "SKIP INTRO", "START AT THE MAIN MENU, WITHOUT THE LOGOS", SET_SKIP_INTRO },
    { "SKIP LOADING", "LOAD IN ONE GO, WITHOUT THE LOADING SCREENS", SET_SKIP_LOADING },
};
static const MenuOption VideoOptionsList[] = {
    { "DISPLAY", "PLAY IN A WINDOW OR FULLSCREEN", SET_DISPLAY },
    { "WINDOW SIZE", "THE SIZE OF THE WINDOW ON YOUR SCREEN", SET_WINDOW },
    { "RESOLUTION", "THE SIZE THE GAME DRAWS AT", SET_RENDER },
    { "FRAME RATE", "30 IS THE ORIGINAL, 60 AND 120 ARE SMOOTHER", SET_FPS },
    { "VSYNC", "MATCH YOUR SCREEN, TO STOP TEARING", SET_VSYNC },
};

#define NUM_OF(a) ((s32)(sizeof(a) / sizeof((a)[0])))

static const MenuCategory Categories[] = {
    { "GAME", GameOptionsList, NUM_OF(GameOptionsList) },
    { "VIDEO", VideoOptionsList, NUM_OF(VideoOptionsList) },
};
#define NUM_CATEGORIES NUM_OF(Categories)
#define MAX_CATEGORIES 4 /* and BACK */

static const char *const TopRows[] = { "PLAY", "SETTINGS", "QUIT" };
enum { TOP_PLAY, TOP_SETTINGS, TOP_QUIT, NUM_TOP };

/* The logo: two meshes of the intro's model, drawn by PlaceRecs like any model on a menu. */
#define LOGO_MODEL DB_CutsceneIntroLogo2
#define LOGO_TOP 0x2006
#define LOGO_BOTTOM 0x2007
#define LOGO_SCALE 0xA0
#define LOGO_Y -0xC1

/* The subengines, in PlaceRec order: a slot has 32. */
#define SUB_LOGO 0
#define SUB_PANEL 2
#define SUB_HELP 4
#define SUB_NAV 5
#define SUB_CATEGORY 6
#define SUB_OPTION (SUB_CATEGORY + MAX_CATEGORIES + 1)
#define VISIBLE_OPTIONS 10
#define SUB_LABEL(n) (SUB_OPTION + (n) * 2)
#define SUB_VALUE(n) (SUB_OPTION + (n) * 2 + 1)

#define TOP_Y -0x38
#define TOP_STEP 0x22

#define PANEL_TOP -0x78
#define PANEL_BOTTOM 0x88
#define CATEGORY_LEFT -0x130
#define CATEGORY_RIGHT -0x5A
#define CATEGORY_STEP 0x22
#define OPTION_LEFT -0x50
#define OPTION_RIGHT 0x130
#define OPTION_TOP -0x6C
#define OPTION_STEP 0x18
#define LABEL_X -0x44
#define VALUE_X 0x70
#define HELP_Y 0x98
#define NAV_Y 0xC8

/* The game's text styles (PosFlashes), size and colour together. */
#define TEXT_LARGE 0
#define TEXT_LARGE_LIT 2
#define TEXT_LARGE_DIM 3
#define TEXT_SMALL 4
#define TEXT_SMALL_LIT 8

static PlaceRec Place[SUB_LABEL(VISIBLE_OPTIONS) + 1];
static char Values[VISIBLE_OPTIONS][24];

enum { SCREEN_TOP, SCREEN_CATEGORIES, SCREEN_OPTIONS };
static s32 Screen;
static s32 TopCursor;
static s32 Category;  /* NUM_CATEGORIES is BACK */
static s32 Option;
static s32 FirstShown;

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
    case SET_SKIP_INTRO:
        s->skip_intro = !s->skip_intro;
        break;
    case SET_SKIP_LOADING:
        s->skip_loading = !s->skip_loading;
        break;
    case SET_DISPLAY:
        s->fullscreen = !s->fullscreen;
        break;
    case SET_WINDOW:
        step_size(WindowSizes, NUM_OF(WindowSizes), &s->window_w, &s->window_h, dir);
        break;
    case SET_RENDER:
        step_size(RenderSizes, NUM_OF(RenderSizes), &s->render_w, &s->render_h, dir);
        break;
    case SET_FPS:
        s->fps = dir > 0 ? (s->fps == 30 ? 60 : s->fps == 60 ? 120 : 30) : (s->fps == 30 ? 120 : s->fps == 120 ? 60 : 30);
        break;
    case SET_VSYNC:
        s->vsync = !s->vsync;
        break;
    }
    settings_apply();
}

static void show_value(char *text, size_t size, s32 id) {
    const Settings *s = settings();

    switch (id) {
    case SET_SKIP_INTRO:
        snprintf(text, size, "%s", s->skip_intro ? "ON" : "OFF");
        break;
    case SET_SKIP_LOADING:
        snprintf(text, size, "%s", s->skip_loading ? "ON" : "OFF");
        break;
    case SET_DISPLAY:
        snprintf(text, size, "%s", s->fullscreen ? "FULLSCREEN" : "WINDOW");
        break;
    case SET_WINDOW:
        snprintf(text, size, "%d X %d", s->window_w, s->window_h);
        break;
    case SET_RENDER:
        snprintf(text, size, "%d X %d", s->render_w, s->render_h);
        break;
    case SET_FPS:
        snprintf(text, size, "%d", s->fps);
        break;
    case SET_VSYNC:
        snprintf(text, size, "%s", s->vsync ? "ON" : "OFF");
        break;
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
static void place_panel(PlaceRec *r, s32 left, s32 right, s32 top, s32 bottom) {
    r->type = 4;
    r->x = left;
    r->y = right;
    r->z = top;
    r->scale = bottom;
    r->rx = 1;
}

/* A screen's records: what it doesn't use is empty text, or a panel off the screen. */
static void place_screen(void) {
    s32 i;

    memset(Place, 0, sizeof(Place));
    place_logo(&Place[SUB_LOGO], LOGO_TOP);
    place_logo(&Place[SUB_LOGO + 1], LOGO_BOTTOM);
    if (Screen == SCREEN_TOP) {
        place_panel(&Place[SUB_PANEL], -0xA0, 0xA0, TOP_Y - 0x10, TOP_Y + NUM_TOP * TOP_STEP + 0xA);
        place_panel(&Place[SUB_PANEL + 1], 0, 0, -0x400, -0x400);
    } else {
        place_panel(&Place[SUB_PANEL], CATEGORY_LEFT, CATEGORY_RIGHT, PANEL_TOP, PANEL_BOTTOM);
        place_panel(&Place[SUB_PANEL + 1], OPTION_LEFT, OPTION_RIGHT, PANEL_TOP, PANEL_BOTTOM);
    }
    place_text(&Place[SUB_HELP], -0x8000, HELP_Y, "", 1);
    place_text(&Place[SUB_NAV], -0x8000, NAV_Y, "", 1);
    for (i = 0; i < MAX_CATEGORIES + 1; i++) {
        if (Screen == SCREEN_TOP) {
            place_text(&Place[SUB_CATEGORY + i], -0x8000, TOP_Y + i * TOP_STEP, "", 0);
        } else {
            place_text(&Place[SUB_CATEGORY + i], CATEGORY_LEFT + 0x14, OPTION_TOP + i * CATEGORY_STEP, "", 0);
        }
    }
    for (i = 0; i < VISIBLE_OPTIONS; i++) {
        s32 y = OPTION_TOP + i * OPTION_STEP;

        place_text(&Place[SUB_LABEL(i)], LABEL_X, y, "", 1);
        place_text(&Place[SUB_VALUE(i)], VALUE_X, y, Values[i], 1);
    }
    PlaceEngineSlot(0, Place);
}

static void set_row(subengine *sub, const char *text, s32 colour) {
    sub->u6C.text = (char *)text;
    sub->unk7C = colour;
}

/* What's on screen now, from where the cursor is. */
static void show_screen(void) {
    subengine *sub = GetEngineSlot(0)->sub;
    const MenuCategory *c = Category < NUM_CATEGORIES ? &Categories[Category] : NULL;
    s32 i;

    if (Screen == SCREEN_TOP) {
        for (i = 0; i < MAX_CATEGORIES + 1; i++) {
            set_row(&sub[SUB_CATEGORY + i], i < NUM_TOP ? TopRows[i] : "", i == TopCursor ? TEXT_LARGE_LIT : TEXT_LARGE);
        }
        set_row(&sub[SUB_HELP], "", TEXT_SMALL);
        set_row(&sub[SUB_NAV], "PRESS UP - DOWN TO CHOOSE, \xFC BUTTON TO SELECT", TEXT_SMALL);
        for (i = 0; i < VISIBLE_OPTIONS; i++) {
            set_row(&sub[SUB_LABEL(i)], "", TEXT_SMALL);
            Values[i][0] = '\0';
        }
        return;
    }
    for (i = 0; i < MAX_CATEGORIES + 1; i++) {
        s32 colour = i == Category ? TEXT_LARGE_LIT : Screen == SCREEN_OPTIONS ? TEXT_LARGE_DIM : TEXT_LARGE;

        set_row(&sub[SUB_CATEGORY + i], i < NUM_CATEGORIES ? Categories[i].label : i == NUM_CATEGORIES ? "BACK" : "", colour);
    }
    set_row(&sub[SUB_NAV], Screen == SCREEN_OPTIONS ? "UP - DOWN CHOOSE, LEFT - RIGHT CHANGE, \xFE BACK"
                                                    : "UP - DOWN TO CHOOSE, RIGHT OR \xFC BUTTON TO OPEN", TEXT_SMALL);
    set_row(&sub[SUB_HELP], Screen == SCREEN_OPTIONS ? c->options[Option].help : "", TEXT_SMALL);
    for (i = 0; i < VISIBLE_OPTIONS; i++) {
        s32 n = FirstShown + i;

        Values[i][0] = '\0';
        if (c != NULL && n < c->count) {
            s32 lit = Screen == SCREEN_OPTIONS && n == Option;
            char value[24];

            set_row(&sub[SUB_LABEL(i)], c->options[n].label, lit ? TEXT_SMALL_LIT : TEXT_SMALL);
            value[0] = '\0';
            show_value(value, sizeof(value), c->options[n].id);
            snprintf(Values[i], sizeof(Values[i]), lit ? "< %s >" : "%s", value);
            sub[SUB_VALUE(i)].unk7C = lit ? TEXT_SMALL_LIT : TEXT_SMALL;
        } else {
            set_row(&sub[SUB_LABEL(i)], "", TEXT_SMALL);
        }
    }
}

/* The top level and the settings lay out their rows differently. */
static void go_to_screen(s32 screen) {
    s32 replace = (screen == SCREEN_TOP) != (Screen == SCREEN_TOP);

    Screen = screen;
    if (replace) {
        place_screen();
    }
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

static void update_top(s32 pressed) {
    if ((pressed & 0x40) && TopCursor < NUM_TOP - 1) {
        TopCursor++;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x10) && TopCursor > 0) {
        TopCursor--;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (pressed & (0x4000 | 8)) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        switch (TopCursor) {
        case TOP_PLAY:
            start_game();
            break;
        case TOP_SETTINGS:
            Category = 0;
            FirstShown = 0;
            go_to_screen(SCREEN_CATEGORIES);
            break;
        case TOP_QUIT:
            settings_save();
            platform_finish(0);
            break;
        }
    }
}

static void update_categories(s32 pressed) {
    if ((pressed & 0x40) && Category < NUM_CATEGORIES) {
        Category++;
        FirstShown = 0;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x10) && Category > 0) {
        Category--;
        FirstShown = 0;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (Category == NUM_CATEGORIES) {
        if (pressed & (0x4000 | 8)) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            go_to_screen(SCREEN_TOP);
        }
    } else if (pressed & (0x20 | 0x4000)) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        Option = 0;
        FirstShown = 0;
        go_to_screen(SCREEN_OPTIONS);
    }
    if (pressed & 0x1000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        go_to_screen(SCREEN_TOP);
    }
}

static void update_options(s32 pressed) {
    const MenuCategory *c = &Categories[Category];

    if ((pressed & 0x40) && Option < c->count - 1) {
        Option++;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x10) && Option > 0) {
        Option--;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (Option < FirstShown) {
        FirstShown = Option;
    }
    if (Option >= FirstShown + VISIBLE_OPTIONS) {
        FirstShown = Option - VISIBLE_OPTIONS + 1;
    }
    if (pressed & (0x20 | 0x4000)) {
        change_setting(c->options[Option].id, 1);
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    } else if (pressed & 0x80) {
        change_setting(c->options[Option].id, -1);
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (pressed & 0x1000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        go_to_screen(SCREEN_CATEGORIES);
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
    dbase_asload_with_anims(&HeroModelBank[LOGO_MODEL], NULL);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    Screen = SCREEN_TOP;
    TopCursor = 0;
    place_screen();
    show_screen();
}

static void SettingsMenuUpdate(void) {
    s32 pressed = Pads[0].pressed;

    GameFrameCount++;
    AnimateTitlesBackdrop();
    sway_logo();
    switch (Screen) {
    case SCREEN_TOP:
        update_top(pressed);
        break;
    case SCREEN_CATEGORIES:
        update_categories(pressed);
        break;
    case SCREEN_OPTIONS:
        update_options(pressed);
        break;
    }
    show_screen();
}

static void SettingsMenuQuit(void) {
    dbase_release(&HeroModelBank[LOGO_MODEL]);
}

process_info SettingsMenu = { SettingsMenuInit, SettingsMenuUpdate, RenderTitlesBackdropAndMenu, SettingsMenuQuit, 0 };
