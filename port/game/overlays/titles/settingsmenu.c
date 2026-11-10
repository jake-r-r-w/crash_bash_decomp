/* The launcher the PC version opens with, built like the game's own menus: PlaceRecs on the titles'
 * backdrop, in its font and panels, under the 3D logo from the intro. PLAY, SETTINGS and QUIT, and
 * the settings a screen down: categories on the left, a scrolling list of options on the right.
 * Settings take effect as they change. To add one: a row in its category's table, a case in
 * change_setting and in show_value; a video one goes in main/videomenu.c, as other menus show those too.
 * The Mods page lists the mods (mods.c) to turn on and off and put in order. With two or more controllers, PLAY goes through the
 * players screen first: each controller's player, put in order by pressing up or down on it. */
#include <stdio.h>
#include <string.h>
#include "../../../platform/controllers.h"
#include "../../../platform/controls.h"
#include "../../../platform/mods.h"
#include "../../../platform/settings.h"
#include "common.h"
#include "bash/engine.h"
#include "bash/fingerprint.h"
#include "bash/main.h"
#include "bash/titles_ovl.h"
#include "bash/titles.h"
#include "bash/videomenu.h"

void platform_finish(int code);
int platform_vblanks(void);

enum {
    SET_SKIP_INTRO,
    SET_SKIP_LOADING,
    SET_CHEAT_MENU,
    SET_EXTRA_HEROES
};

typedef struct {
    const char *label;
    const char *help;
    s32 id;
} MenuOption;

/* What a category's list is: options from its table, the video options, the mods, or the keys or controller buttons for the game's
   buttons (controls.h) */
enum { KIND_OPTIONS, KIND_VIDEO, KIND_MODS, KIND_CONTROLS };

/* The CONTROLS page's rows: one for each of the game's buttons, and RESET TO DEFAULTS */
#define ROW_RESET NUM_CONTROLS

typedef struct {
    const char *label;
    const MenuOption *options;
    s32 count;
    s32 kind;
} MenuCategory;

static const MenuOption GameOptionsList[] = {
    { "SKIP INTRO", "START AT THE MAIN MENU, WITHOUT THE LOGOS", SET_SKIP_INTRO },
    { "SKIP LOADING", "LOAD IN ONE GO, WITHOUT THE LOADING SCREENS", SET_SKIP_LOADING },
    { "CHEAT MENU", "THE ORIGINAL'S HIDDEN CHEATS, IN THE PAUSE MENU", SET_CHEAT_MENU },
    { "EXTRA HEROES", "RIPPER ROO AND PINSTRIPE, FROM YOUR OWN CRASH BANDICOOT DISC", SET_EXTRA_HEROES },
};
#define NUM_OF(a) ((s32)(sizeof(a) / sizeof((a)[0])))

static const MenuCategory Categories[] = {
    { "GAME", GameOptionsList, NUM_OF(GameOptionsList), KIND_OPTIONS },
    { "VIDEO", NULL, NUM_VIDEO_OPTIONS, KIND_VIDEO },
    { "CONTROLS", NULL, ROW_RESET + 1, KIND_CONTROLS },
    { "MODS", NULL, 0, KIND_MODS },
};
#define NUM_CATEGORIES NUM_OF(Categories)
#define MAX_CATEGORIES 4 /* and BACK */

/* The control waiting for its new key or button, and for how many more frames: the page gives up after a few seconds */
#define WAIT_FRAMES 300
static s32 Waiting = -1;
static s32 WaitFrames;
static s32 Settling; /* a key or button was just chosen: till everything's let go, as the one still held is a press of its new control */
#define CONTROL_VALUE_X 0x18 /* past SELECT, with room for PRESS A KEY OR BUTTON */

static const char *const TopRows[] = { "PLAY", "SETTINGS", "QUIT" };
enum { TOP_PLAY, TOP_SETTINGS, TOP_QUIT, NUM_TOP };

/* The logo: two meshes of the intro's model at their full size, in a view of their own: small because
 * its camera is far away, as the intro has it. A rotation matrix scaled this far down loses too much. */
#define LOGO_MODEL DB_CutsceneIntroLogo2
#define LOGO_SLOT 2
#define LOGO_EYE 0x1400
#define LOGO_DIST 0x3A
#define LOGO_CENTRE_Y 0x30

static PlaceRec LogoPlace[3] = {
    { 0xA, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0, 0, LOGO_MODEL, 0x2006, 0 },
    { 0xA, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0, 0, LOGO_MODEL, 0x2007, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
static GfxViewport LogoView = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, LOGO_CENTRE_Y, 0x200, 0x5FF, 0, 0, 0, 0, 0 };
static camera_info_tag LogoCamera;

/* The subengines, in PlaceRec order: a slot has 32. */
#define SUB_PANEL 0
#define SUB_HELP 2
#define SUB_NAV 3
#define SUB_CATEGORY 4
#define SUB_OPTION (SUB_CATEGORY + MAX_CATEGORIES + 1)
#define VISIBLE_OPTIONS 10
#define SUB_LABEL(n) (SUB_OPTION + (n) * 2)
#define SUB_VALUE(n) (SUB_OPTION + (n) * 2 + 1)
#define SUB_HELP_MORE SUB_LABEL(VISIBLE_OPTIONS)

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

/* The pad's shoulder buttons, which move a mod up and down the list */
#define PAD_L1 0x400
#define PAD_R1 0x800

/* The longest a mod's name is in the list, and a line of help under it, in the game's small letters */
#define LABEL_CHARS 18
#define HELP_CHARS 48

/* The players screen: a controller's name in its player's row, in the game's small letters */
#define PLAYERS 4
#define PLAYERS_TOP -0x50
#define PLAYERS_STEP 0x22
#define PLAYERS_LEFT -0x130
#define PLAYERS_RIGHT 0x130
#define PLAYER_X -0x118
#define CONTROLLER_X -0x68 /* room for 31 letters, as long as SDL's names get: DUALSENSE WIRELESS CONTROLLER is 29 */
#define LIT_FRAMES 40

static PlaceRec Place[SUB_HELP_MORE + 2];
static char Values[VISIBLE_OPTIONS][32];
static char Labels[VISIBLE_OPTIONS][LABEL_CHARS + 1];
static char Help[2][HELP_CHARS + 1];
static s32 PlayerLit[PLAYERS]; /* frames its row stays lit, after its controller was pressed or moved */

enum { SCREEN_TOP, SCREEN_CATEGORIES, SCREEN_OPTIONS, SCREEN_PLAYERS };
static s32 Screen;
static s32 TopCursor;
static s32 Category;  /* NUM_CATEGORIES is BACK */
static s32 Option;
static s32 FirstShown;

/* Text for the game's font, which has capitals, digits, a few marks and the space: the rest goes, and what's too long is cut at a word,
   with dots. */
static void font_text(char *out, size_t size, const char *in) {
    char clean[512];
    size_t n = 0, cut;

    for (; *in != 0 && n + 1 < sizeof(clean); in++) {
        char c = *in;

        if (c >= 'a' && c <= 'z') {
            c = (char)(c - 'a' + 'A');
        } else if (c == '_' || c == '\t') {
            c = ' ';
        } else if (c == '&') {
            c = '+';
        } else if (c == '"' || c == '`') {
            c = '\'';
        } else if (c == ';') {
            c = ',';
        } else if (c == '/' || c == '\\') {
            c = '-';
        } else if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || strchr(".,':!=+-?%<> ", c) != NULL)) {
            continue;
        }
        if (c == ' ' && (n == 0 || clean[n - 1] == ' ')) {
            continue;
        }
        clean[n++] = c;
    }
    while (n > 0 && clean[n - 1] == ' ') {
        n--;
    }
    clean[n] = 0;
    if (n >= size) {
        /* at a space in its second half, if there's one */
        for (cut = size - 4; cut > size / 2 && clean[cut] != ' '; cut--) {
        }
        if (clean[cut] != ' ') {
            cut = size - 4;
        }
        while (cut > 0 && strchr(" ,:", clean[cut - 1]) != NULL) {
            cut--;
        }
        memcpy(clean + cut, "...", 4);
    }
    memcpy(out, clean, strlen(clean) + 1);
}

/* A game button as the CONTROLS page shows it: the face buttons as the font's own button pictures, as the game's
   menus draw them ("PRESS \xFC BUTTON TO START"), and the rest by name */
static const char *control_label(s32 n) {
    switch (n) {
    case CONTROL_CROSS:
        return "\xFC";
    case CONTROL_CIRCLE:
        return "\xFD";
    case CONTROL_SQUARE:
        return "\xFF";
    case CONTROL_TRIANGLE:
        return "\xFE";
    default:
        return control_name(n);
    }
}

/* A row of the CONTROLS page: what plays that button on what the player's using, the keyboard or a controller, whichever had the last
   press (host_using_gamepad), as modern games' button prompts follow it */
static void control_value(char *text, size_t size, s32 n) {
    static const char *const Faces[][2] = { { "CROSS", "\xFC" }, { "CIRCLE", "\xFD" }, { "SQUARE", "\xFF" }, { "TRIANGLE", "\xFE" } };
    const char *label;
    s32 f;

    if (n == ROW_RESET) {
        text[0] = '\0';
        return;
    }
    if (Waiting == n) {
        snprintf(text, size, "%s", "PRESS A KEY OR BUTTON");
        return;
    }
    if (!host_using_gamepad()) {
        font_text(text, size, controls_key_label(settings()->keys[n]));
        return;
    }
    /* a PlayStation controller's face buttons as the font's pictures of them, as the rows have them */
    label = controls_button_label(settings()->buttons[n]);
    font_text(text, size, label);
    for (f = 0; f < NUM_OF(Faces); f++) {
        if (strcmp(label, Faces[f][0]) == 0) {
            snprintf(text, size, "%s", Faces[f][1]);
        }
    }
}

/* --- the Mods page: OPEN MODS FOLDER, then the mods in their order --- */

static s32 option_count(const MenuCategory *c) {
    return c->kind == KIND_MODS ? mods_count() + 1 : c->count;
}

static s32 mod_usable(const ModInfo *m) {
    return m != NULL && m->textures + m->files != 0;
}

/* Two lines: what it does, or what's wrong with it; and who made it. */
static void mod_help(s32 n) {
    const ModInfo *m = mods_info(n - 1);
    char text[512];

    if (m == NULL) {
        font_text(Help[0], sizeof(Help[0]), "PUT MOD ZIPS IN IT, AND THEY SHOW UP HERE");
        font_text(Help[1], sizeof(Help[1]), "WHERE TWO CHANGE ONE THING, THE HIGHER ONE WINS");
        return;
    }
    if (m->problem[0] != 0) {
        snprintf(text, sizeof(text), "%s", m->problem);
    } else if (m->description[0] != 0) {
        snprintf(text, sizeof(text), "%s", m->description);
    } else if (m->textures != 0 && m->files != 0) {
        snprintf(text, sizeof(text), "%d TEXTURES AND %d FILES", m->textures, m->files);
    } else {
        snprintf(text, sizeof(text), "%d %s", m->textures + m->files, m->textures != 0 ? (m->textures == 1 ? "TEXTURE" : "TEXTURES") : (m->files == 1 ? "FILE" : "FILES"));
    }
    font_text(Help[0], sizeof(Help[0]), text);
    if (m->pack) {
        snprintf(text, sizeof(text), "A TEXTURE PACK FROM THE PACKS FOLDER");
    } else {
        snprintf(text, sizeof(text), "%s%s%s%s", m->version[0] != 0 ? "VERSION " : "", m->version,
                 m->author[0] != 0 ? (m->version[0] != 0 ? " BY " : "BY ") : "", m->author);
    }
    font_text(Help[1], sizeof(Help[1]), text);
}

static void change_mod(s32 n, s32 dir) {
    const ModInfo *m = mods_info(n - 1);

    if (n == 0) {
        if (dir > 0) {
            mods_open_folder();
        }
    } else if (mod_usable(m)) {
        mods_set_on(n - 1, !m->on);
        mods_apply_textures();
    }
}

/* A mod up or down the list, and the cursor with it. */
static void move_mod(s32 dir) {
    if (Option + dir >= 1 && Option + dir <= mods_count()) {
        mods_move(Option - 1, dir);
        Option += dir;
        mods_apply_textures();
        settings_apply();
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

/* --- rows --- */

static void change_setting(const MenuCategory *c, s32 n, s32 dir) {
    Settings *s = settings();

    if (c->kind == KIND_VIDEO) {
        video_option_change(n, dir);
        return;
    }
    if (c->kind == KIND_MODS) {
        change_mod(n, dir);
    } else {
        switch (c->options[n].id) {
        case SET_SKIP_INTRO:
            s->skip_intro = !s->skip_intro;
            break;
        case SET_SKIP_LOADING:
            s->skip_loading = !s->skip_loading;
            break;
        case SET_CHEAT_MENU:
            s->cheat_menu = !s->cheat_menu;
            break;
        case SET_EXTRA_HEROES:
            s->extra_heroes = !s->extra_heroes;
            break;
        }
    }
    settings_apply();
}

static void show_value(char *text, size_t size, const MenuCategory *c, s32 n) {
    const Settings *s = settings();

    if (c->kind == KIND_VIDEO) {
        video_option_value(n, text, (s32)size);
        return;
    }
    if (c->kind == KIND_MODS) {
        const ModInfo *m = mods_info(n - 1);

        snprintf(text, size, "%s", m == NULL ? "" : !mod_usable(m) ? "BROKEN" : m->on ? "ON" : "OFF");
        return;
    }
    if (c->kind == KIND_CONTROLS) {
        control_value(text, size, n);
        return;
    }
    switch (c->options[n].id) {
    case SET_SKIP_INTRO:
        snprintf(text, size, "%s", s->skip_intro ? "ON" : "OFF");
        break;
    case SET_SKIP_LOADING:
        snprintf(text, size, "%s", s->skip_loading ? "ON" : "OFF");
        break;
    case SET_CHEAT_MENU:
        snprintf(text, size, "%s", s->cheat_menu ? "ON" : "OFF");
        break;
    case SET_EXTRA_HEROES:
        snprintf(text, size, "%s", s->extra_heroes ? "ON" : "OFF");
        break;
    }
}

/* Row n's label, shown in row i. */
static const char *option_label(const MenuCategory *c, s32 n, s32 i) {
    if (c->kind == KIND_VIDEO) {
        return video_option_label(n);
    }
    if (c->kind == KIND_MODS) {
        if (n == 0) {
            return "OPEN MODS FOLDER";
        }
        font_text(Labels[i], sizeof(Labels[i]), mods_info(n - 1)->name);
        return Labels[i];
    }
    if (c->kind == KIND_CONTROLS) {
        return n == ROW_RESET ? "RESET TO DEFAULTS" : control_label(n);
    }
    return c->options[n].label;
}

static const char *option_help(const MenuCategory *c, s32 n) {
    if (c->kind == KIND_VIDEO) {
        return video_option_help(n);
    }
    if (c->kind == KIND_MODS) {
        mod_help(n);
        return Help[0];
    }
    if (c->kind == KIND_CONTROLS) {
        if (Waiting >= 0) {
            snprintf(Help[0], sizeof(Help[0]), "PRESS A KEY OR BUTTON FOR %s, OR ESC", control_label(Waiting));
            return Help[0];
        }
        return n == ROW_RESET ? "EVERY KEY AND BUTTON BACK AS IT WAS" : "\xFC THEN A KEY OR A BUTTON FOR IT";
    }
    return c->options[n].help;
}

static void place_text(PlaceRec *r, s32 x, s32 y, const char *text, s32 small) {
    r->type = 3;
    r->x = x;
    r->y = y;
    r->data = (void *)text;
    r->frame = small ? 4 : 0;
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
    if (Screen == SCREEN_TOP) {
        place_panel(&Place[SUB_PANEL], -0xA0, 0xA0, TOP_Y - 0x10, TOP_Y + NUM_TOP * TOP_STEP + 0xA);
        place_panel(&Place[SUB_PANEL + 1], 0, 0, -0x400, -0x400);
    } else if (Screen == SCREEN_PLAYERS) {
        place_panel(&Place[SUB_PANEL], PLAYERS_LEFT, PLAYERS_RIGHT, PLAYERS_TOP - 0x12, PLAYERS_TOP + PLAYERS * PLAYERS_STEP + 0x10);
        place_panel(&Place[SUB_PANEL + 1], 0, 0, -0x400, -0x400);
    } else {
        place_panel(&Place[SUB_PANEL], CATEGORY_LEFT, CATEGORY_RIGHT, PANEL_TOP, PANEL_BOTTOM);
        place_panel(&Place[SUB_PANEL + 1], OPTION_LEFT, OPTION_RIGHT, PANEL_TOP, PANEL_BOTTOM);
    }
    place_text(&Place[SUB_HELP], -0x8000, HELP_Y, "", 1);
    place_text(&Place[SUB_HELP_MORE], -0x8000, HELP_Y + OPTION_STEP, "", 1);
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

        if (Screen == SCREEN_PLAYERS && i < PLAYERS) {
            place_text(&Place[SUB_LABEL(i)], PLAYER_X, PLAYERS_TOP + i * PLAYERS_STEP, "", 0);
            place_text(&Place[SUB_VALUE(i)], CONTROLLER_X, PLAYERS_TOP + i * PLAYERS_STEP + 4, Values[i], 1);
        } else {
            place_text(&Place[SUB_LABEL(i)], LABEL_X, y, "", 1);
            place_text(&Place[SUB_VALUE(i)], VALUE_X, y, Values[i], 1);
        }
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
    s32 mods = Screen == SCREEN_OPTIONS && c->kind == KIND_MODS;
    s32 controls = c != NULL && c->kind == KIND_CONTROLS;
    s32 i;

    set_row(&sub[SUB_HELP_MORE], "", TEXT_SMALL);
    if (Screen == SCREEN_PLAYERS) {
        for (i = 0; i < MAX_CATEGORIES + 1; i++) {
            set_row(&sub[SUB_CATEGORY + i], "", TEXT_LARGE);
        }
        for (i = 0; i < VISIBLE_OPTIONS; i++) {
            Values[i][0] = '\0';
            if (i < PLAYERS) {
                const char *name = host_controller_name(i);

                snprintf(Labels[i], sizeof(Labels[i]), "PLAYER %d", i + 1);
                set_row(&sub[SUB_LABEL(i)], Labels[i], PlayerLit[i] != 0 ? TEXT_LARGE_LIT : name != NULL ? TEXT_LARGE : TEXT_LARGE_DIM);
                font_text(Values[i], sizeof(Values[i]), name != NULL ? name : "-");
                sub[SUB_VALUE(i)].unk7C = PlayerLit[i] != 0 ? TEXT_SMALL_LIT : TEXT_SMALL;
            } else {
                set_row(&sub[SUB_LABEL(i)], "", TEXT_SMALL);
            }
        }
        set_row(&sub[SUB_HELP], "PRESS UP OR DOWN ON A CONTROLLER TO MOVE IT", TEXT_SMALL);
        set_row(&sub[SUB_NAV], "START TO PLAY, \xFE BACK", TEXT_SMALL);
        return;
    }
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
    set_row(&sub[SUB_NAV], mods && Option > 0 ? "\xFC ON OR OFF, L1 - R1 MOVE IT UP OR DOWN, \xFE BACK"
                           : mods             ? "UP - DOWN CHOOSE, \xFC BUTTON TO OPEN, \xFE BACK"
                           : controls && Waiting >= 0 ? ""
                           : controls         ? "UP - DOWN CHOOSE, \xFC BUTTON TO CHANGE, \xFE BACK"
                           : Screen == SCREEN_OPTIONS ? "UP - DOWN CHOOSE, LEFT - RIGHT CHANGE, \xFE BACK"
                                                      : "UP - DOWN TO CHOOSE, RIGHT OR \xFC BUTTON TO OPEN", TEXT_SMALL);
    set_row(&sub[SUB_HELP], Screen == SCREEN_OPTIONS ? option_help(c, Option) : "", TEXT_SMALL);
    if (mods) {
        set_row(&sub[SUB_HELP_MORE], Help[1], TEXT_SMALL);
    }
    for (i = 0; i < VISIBLE_OPTIONS; i++) {
        s32 n = FirstShown + i;

        Values[i][0] = '\0';
        if (c != NULL && n < option_count(c)) {
            s32 lit = Screen == SCREEN_OPTIONS && n == Option;
            char value[32];

            set_row(&sub[SUB_LABEL(i)], option_label(c, n, i), lit ? TEXT_SMALL_LIT : TEXT_SMALL);
            value[0] = '\0';
            show_value(value, sizeof(value), c, n);
            /* left and right change an option's value; a key or button is changed by pressing the new one */
            snprintf(Values[i], sizeof(Values[i]), lit && value[0] != '\0' && !controls ? "< %s >" : "%s", value);
            sub[SUB_VALUE(i)].unk7C = lit || (controls && Waiting == n) ? TEXT_SMALL_LIT : TEXT_SMALL;
            sub[SUB_VALUE(i)].XYZ.x = controls ? CONTROL_VALUE_X : VALUE_X;
        } else {
            set_row(&sub[SUB_LABEL(i)], "", TEXT_SMALL);
        }
    }
}

/* The top level, the settings and the players screen lay out their rows differently. */
static s32 layout(s32 screen) {
    return screen == SCREEN_OPTIONS ? SCREEN_CATEGORIES : screen;
}

static void go_to_screen(s32 screen) {
    s32 replace = layout(screen) != layout(Screen);

    Screen = screen;
    if (replace) {
        place_screen();
    }
}

static void start_game(void) {
    if (settings()->no_save) {
        printf("launcher: play at vblank %d\n", platform_vblanks());
    }
    settings_save();
    settings()->launcher = 0;
    /* what the game reads from here on: textures took effect as they were chosen */
    mods_apply_files();
    fingerprint_start();
    if (settings()->skip_intro) {
        process_control_goto(&MenuProcessControl, &MainMenu);
    } else {
        FirstBootDone = 1; /* what has the level load play the intro first, as at boot */
        ProcessControl.next_process = &LevelLoadProcess;
    }
}

/* --- the players screen --- */

static void light_player(s32 player) {
    PlayerLit[player] = LIT_FRAMES;
    host_controller_buzz(player);
}

static void open_players(void) {
    s32 i;

    host_controllers_close_gaps();
    for (i = 0; i < PLAYERS; i++) {
        host_controller_pressed(i); /* PLAY's own press, and anything before it */
        PlayerLit[i] = 0;
    }
    if (settings()->no_save) {
        printf("pads: the players screen\n");
    }
    go_to_screen(SCREEN_PLAYERS);
}

/* Up or down on a controller moves it up or down the players; START on any plays, Triangle on any goes back. The keyboard is player
   1's with the first controller, as ever, so its keys only play or go back. */
static void update_players(s32 pressed) {
    s32 play = (pressed & 8) != 0, back = (pressed & 0x1000) != 0;
    s32 i;

    host_controllers_close_gaps();
    for (i = 0; i < PLAYERS; i++) {
        if (PlayerLit[i] > 0) {
            PlayerLit[i]--;
        }
    }
    for (i = 0; i < PLAYERS; i++) {
        s32 p = host_controller_pressed(i);

        if (p == 0) {
            continue;
        }
        if ((p & 0x10) && i > 0 && host_controller_name(i - 1) != NULL) {
            host_controllers_swap(i, i - 1);
            light_player(i - 1);
            PlayerLit[i] = 0;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        } else if ((p & 0x40) && i < PLAYERS - 1 && host_controller_name(i + 1) != NULL) {
            host_controllers_swap(i, i + 1);
            light_player(i + 1);
            PlayerLit[i] = 0;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        } else if (!(p & (8 | 0x1000))) {
            light_player(i);
        }
        play |= (p & 8) != 0;
        back |= (p & 0x1000) != 0;
    }
    if (back) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        go_to_screen(SCREEN_TOP);
    } else if (play) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        start_game();
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
            if (host_controllers() >= 2) {
                open_players();
            } else {
                start_game();
            }
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
        /* what's in the mods folders now, a mod being made and all */
        if (Categories[Category].kind == KIND_MODS) {
            mods_scan();
            settings_apply();
        }
        go_to_screen(SCREEN_OPTIONS);
    }
    if (pressed & 0x1000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        go_to_screen(SCREEN_TOP);
    }
}

/* A control waiting for its new key or button: whichever's pressed first, a key (Esc keeps them as they were) or a button on any
   controller. Given one another control has, that control gets this one's old one (controls_bind). Nothing else on the page answers
   till it's done. */
static void update_waiting(void) {
    Settings *s = settings();
    s32 key = host_key_pressed_any();
    s32 button = host_button_pressed_any();
    s32 keys = key >= 0;
    s32 code = keys ? key : button;

    if (key == KEY_ESCAPE || --WaitFrames <= 0) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        Waiting = -1;
        Settling = 1;
        return;
    }
    if (code < 0) {
        return;
    }
    controls_bind(keys ? s->keys : s->buttons, Waiting, code);
    if (s->no_save) {
        printf("controls: %s is %s %s at vblank %d\n", control_name(Waiting), keys ? "key" : "button",
               keys ? controls_key_id(code) : controls_button_id(code), platform_vblanks());
    }
    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
    Waiting = -1;
    Settling = 1;
}

static void update_controls(s32 pressed) {
    if (!(pressed & (0x20 | 0x4000))) {
        return;
    }
    if (Option == ROW_RESET) {
        controls_default_keys(settings()->keys);
        controls_default_buttons(settings()->buttons);
        if (settings()->no_save) {
            printf("controls: keys and buttons as they were\n");
        }
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        Settling = 1;
        return;
    }
    /* what was pressed to get here isn't the new one */
    host_key_pressed_any();
    host_button_pressed_any();
    Waiting = Option;
    WaitFrames = WAIT_FRAMES;
    PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
}

static void update_options(s32 pressed) {
    static s32 updates;
    const MenuCategory *c = &Categories[Category];
    s32 controls = c->kind == KIND_CONTROLS;

    if (controls && Waiting >= 0) {
        update_waiting();
        return;
    }
    if (Settling) {
        if (Pads[0].buttons != 0) {
            return;
        }
        Settling = 0;
    }
    /* a mod put in the folder shows up in a second or so, with the cursor still on the mod it was on */
    if (c->kind == KIND_MODS && ++updates % 30 == 0) {
        const ModInfo *at = mods_info(Option - 1);
        char id[MOD_ID];
        s32 i;

        snprintf(id, sizeof(id), "%s", at != NULL ? at->id : "");
        if (mods_rescan_if_changed()) {
            settings_apply();
            for (i = 0; i < mods_count(); i++) {
                if (id[0] != '\0' && strcmp(mods_info(i)->id, id) == 0) {
                    Option = i + 1;
                }
            }
            if (Option >= option_count(c)) {
                Option = option_count(c) - 1;
            }
        }
    }
    if ((pressed & 0x40) && Option < option_count(c) - 1) {
        Option++;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x10) && Option > 0) {
        Option--;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (c->kind == KIND_MODS && Option > 0 && (pressed & (PAD_L1 | PAD_R1))) {
        move_mod(pressed & PAD_L1 ? -1 : 1);
    }
    if (Option < FirstShown) {
        FirstShown = Option;
    }
    if (Option >= FirstShown + VISIBLE_OPTIONS) {
        FirstShown = Option - VISIBLE_OPTIONS + 1;
    }
    if (controls) {
        update_controls(pressed);
    } else if (pressed & (0x20 | 0x4000)) {
        change_setting(c, Option, 1);
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    } else if (pressed & 0x80) {
        change_setting(c, Option, -1);
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (pressed & 0x1000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        go_to_screen(SCREEN_CATEGORIES);
    }
}

/* The logo sways a little. */
static void sway_logo(void) {
    subengine *sub = GetEngineSlot(LOGO_SLOT)->sub;
    s32 angle = (sin_cos_table[(GameFrameCount * 24) & 0xFFF].unk2 * 0x40) >> 12;
    s32 i;

    for (i = 0; i < 2; i++) {
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
    cam_init(&LogoCamera);
    LogoCamera.eye.z = LOGO_EYE;
    LogoCamera.dist = LOGO_DIST;
    PlaceEngineSlot(LOGO_SLOT, LogoPlace);
    /* its layers are almost in one plane: in the game's order, as the intro draws them, not by depth */
    GetEngineSlot(LOGO_SLOT)->sub[0].flags |= 0x02000000;
    GetEngineSlot(LOGO_SLOT)->sub[1].flags |= 0x02000000;
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
    case SCREEN_PLAYERS:
        update_players(pressed);
        break;
    }
    show_screen();
}

static void SettingsMenuRender(void) {
    RenderTitlesBackdropAndMenu();
    gfx_set_viewport(&LogoView, &LogoCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(LOGO_SLOT));
}

static void SettingsMenuQuit(void) {
    dbase_release(&HeroModelBank[LOGO_MODEL]);
}

process_info SettingsMenu = { SettingsMenuInit, SettingsMenuUpdate, SettingsMenuRender, SettingsMenuQuit, 0 };
