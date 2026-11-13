/* The launcher the PC version opens with, built like the game's own menus: PlaceRecs on the titles'
 * backdrop, in its font and panels, under the 3D logo from the intro. PLAY, SETTINGS and QUIT, and
 * the settings a screen down: categories on the left, a scrolling list of options on the right.
 * Settings take effect as they change. To add one: a row in its category's table, a case in
 * change_setting and in show_value; a video one goes in main/videomenu.c, as other menus show those too.
 * The Mods page lists the mods (mods.c) to turn on and off and put in order. The Data page (not on Android) is where the saves, the
 * settings and the game's data are kept (platform/folders.h): beside the program, in the user's own folder, or in a folder picked. With two or more controllers, PLAY goes through the
 * players screen first: each controller's player, put in order by pressing up or down on it.
 *
 * ONLINE (docs/PORT.md, Online play): HOST GAME opens a game for others to join and lists them as they do, and START begins it. It
 * asks the router to let players elsewhere in, and shows the join code they need once it has, or says plainly that it couldn't and
 * what can be done. JOIN GAME lists the games being hosted on this network, and takes a host's code typed in (or its address) for
 * one that isn't. The game then starts again from its very beginning on every machine (platform_online_start), and comes back here
 * when it's over. */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "../../../platform/controllers.h"
#include "../../../platform/controls.h"
#include "../../../platform/folders.h"
#include "../../../platform/mods.h"
#include "../../../platform/net.h"
#include "../../../platform/portmap.h"
#include "../../../platform/settings.h"
#include "common.h"
#include "bash/engine.h"
#include "bash/fingerprint.h"
#include "bash/main.h"
#include "bash/menumouse.h"
#include "bash/titles_ovl.h"
#include "bash/titles.h"
#include "bash/videomenu.h"
#include "bash/draw.h"

void platform_finish(int code);
void platform_restart(int online);
void platform_online_start(void);
void platform_online_stop(const char *why);
const char *platform_launcher_message(void);
int host_typed(void);
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
enum { KIND_OPTIONS, KIND_VIDEO, KIND_MODS, KIND_CONTROLS, KIND_DATA };

/* The DATA page's rows: where each kind is kept (FOLDER_, folders.h), then a row each to pick a folder for it */
#define ROW_CHOOSE NUM_FOLDERS
#define DATA_ROWS (NUM_FOLDERS * 2)

/* The CONTROLS page's rows: STICK DEADZONE first, a line under it that the cursor passes over, one for each of the game's buttons, and
   RESET TO DEFAULTS (the user's word on where the deadzone goes, 2026-11-10) */
#define ROW_DEADZONE 0
#define ROW_LINE 1
#define ROW_CONTROLS 2 /* the first button's row: a button's row is ROW_CONTROLS + its CONTROL_ number */
#define ROW_RESET (ROW_CONTROLS + NUM_CONTROLS)
#define ROW_CONTROL(row) ((row) - ROW_CONTROLS)

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
#ifndef __ANDROID__
    /* a phone's app has no folder of its own to see, and Android says where its files go */
    { "DATA", NULL, DATA_ROWS, KIND_DATA },
#endif
};
#define NUM_CATEGORIES NUM_OF(Categories)
#define MAX_CATEGORIES 5 /* and BACK */

/* The control waiting for its new key or button, and for how many more frames: the page gives up after a few seconds */
#define WAIT_FRAMES 300
static s32 Waiting = -1;
static s32 WaitFrames;
static s32 Settling; /* a key or button was just chosen: till everything's let go, as the one still held is a press of its new control */
#define CONTROL_VALUE_X 0x18 /* past SELECT, with room for PRESS A KEY OR BUTTON */
#define DEADZONE_VALUE_X 0x90 /* past STICK DEADZONE, with room for its arrows */

static const char *const TopRows[] = { "PLAY", "ONLINE", "SETTINGS", "QUIT" };
enum { TOP_PLAY, TOP_ONLINE, TOP_SETTINGS, TOP_QUIT, NUM_TOP };
static const char *const OnlineRows[] = { "HOST GAME", "JOIN GAME", "BACK" };
enum { ONLINE_HOST, ONLINE_JOIN, ONLINE_BACK, NUM_ONLINE };

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
#define SUB_STICK_BOX (SUB_HELP_MORE + 1) /* the deadzone's box, on the CONTROLS page */

/* The deadzone's box: in the options panel's right-hand side, under STICK DEADZONE's row and its line, past the buttons' longest names */
#define STICK_BOX_LEFT 0xC0
#define STICK_BOX_RIGHT 0x124
#define STICK_BOX_TOP -0x46
#define STICK_BOX_BOTTOM 0x1E

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

static PlaceRec Place[SUB_STICK_BOX + 2];
static char Values[VISIBLE_OPTIONS][32];
static char Labels[VISIBLE_OPTIONS][LABEL_CHARS + 1];
static char Help[2][HELP_CHARS + 1];
static s32 PlayerLit[PLAYERS]; /* frames its row stays lit, after its controller was pressed or moved */

enum { SCREEN_TOP, SCREEN_CATEGORIES, SCREEN_OPTIONS, SCREEN_PLAYERS, SCREEN_ONLINE, SCREEN_HOST, SCREEN_JOIN, SCREEN_JOINED };
static s32 Screen;
/* ONLINE's screens: where its cursors are, the address being typed, the host being joined, and what there is to say (why a game
   ended, or couldn't be joined), over two lines */
#define ADDRESS_CHARS 15
static s32 OnlineCursor, JoinCursor, Typing;
static s32 JoinMoved, JoinFound; /* whether the cursor's been moved, and how many games there were last frame */
static char Address[ADDRESS_CHARS + 1];
static char Joining[32];
static char Message[2][HELP_CHARS + 1];
static s32 layout(s32 screen);
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

    if (n == ROW_RESET || n == ROW_LINE) {
        text[0] = '\0';
        return;
    }
    if (n == ROW_DEADZONE) {
        if (settings()->stick_deadzone == 0) {
            snprintf(text, size, "ORIGINAL");
        } else {
            snprintf(text, size, "%d%%", settings()->stick_deadzone);
        }
        return;
    }
    n = ROW_CONTROL(n);
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

/* --- the Data page --- */

static const char *const DataNames[NUM_FOLDERS] = { "SAVES", "SETTINGS", "GAME DATA" };
static const char *const ChooseNames[NUM_FOLDERS] = { "CHOOSE SAVES FOLDER", "CHOOSE SETTINGS FOLDER", "CHOOSE GAME DATA FOLDER" };

static void change_data(s32 n, s32 dir) {
    s32 kind = n % NUM_FOLDERS, where = folders_where(kind), i;

    if (n >= ROW_CHOOSE) {
        if (dir > 0) {
            folders_choose(kind);
        }
        return;
    }
    /* the next place it can be, either way round */
    for (i = 0; i < NUM_WHERE; i++) {
        where = (where + (dir > 0 ? 1 : NUM_WHERE - 1)) % NUM_WHERE;
        if (folders_can(kind, where)) {
            break;
        }
    }
    if (where != folders_where(kind)) {
        folders_set(kind, where);
        if (settings()->no_save) {
            printf("data: %s in %d\n", DataNames[kind], where);
        }
    }
}

static const char *data_label(s32 n, s32 i) {
    if (n < ROW_CHOOSE) {
        return DataNames[n];
    }
    (void)i;
    return ChooseNames[n % NUM_FOLDERS];
}

static void data_value(char *text, size_t size, s32 n) {
    static const char *const WhereNames[NUM_WHERE] = { "GAME FOLDER", "USER FOLDER", "YOUR FOLDER" };

    if (n < ROW_CHOOSE) {
        snprintf(text, size, "%s", WhereNames[folders_where(n)]);
    } else {
        text[0] = 0;
    }
}

/* Two lines, kept short at the user's word (2026-11-13): what's kept, and the folder itself, its end if it's too long to show whole. */
static void data_help(s32 n) {
    static const char *const What[NUM_FOLDERS] = { "SAVES ARE KEPT IN", "SETTINGS ARE KEPT IN", "GAME DATA IS KEPT IN" };
    const char *path = folders_path(n % NUM_FOLDERS);
    char shown[HELP_CHARS + 8];
    size_t length = strlen(path);

    snprintf(Help[0], sizeof(Help[0]), "%s", What[n % NUM_FOLDERS]);
    snprintf(shown, sizeof(shown), "%s%s", length > HELP_CHARS - 8 ? "..." : "", length > HELP_CHARS - 8 ? path + length - (HELP_CHARS - 8) : path);
    font_text(Help[1], sizeof(Help[1]), shown);
}

/* --- rows --- */

static void change_setting(const MenuCategory *c, s32 n, s32 dir) {
    Settings *s = settings();

    if (c->kind == KIND_VIDEO) {
        video_option_change(n, dir);
        return;
    }
    if (c->kind == KIND_DATA) {
        change_data(n, dir);
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
    if (c->kind == KIND_DATA) {
        data_value(text, size, n);
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
/* The line between the deadzone and the buttons, the panel's width in the font's dashes */
#define CONTROLS_LINE "- - - - - - - - - - - - - - - - -"

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
        return n == ROW_RESET ? "RESET TO DEFAULTS" : n == ROW_DEADZONE ? "STICK DEADZONE" : n == ROW_LINE ? CONTROLS_LINE : control_label(ROW_CONTROL(n));
    }
    if (c->kind == KIND_DATA) {
        return data_label(n, i);
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
        return n == ROW_RESET ? "EVERY KEY AND BUTTON BACK AS IT WAS" : n == ROW_DEADZONE ? "HOW FAR OUT A STICK GOES: GREEN MOVES YOU"
               : "\xFC THEN A KEY OR A BUTTON FOR IT";
    }
    if (c->kind == KIND_DATA) {
        data_help(n);
        return Help[0];
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
    s32 top = layout(Screen) == SCREEN_TOP, players = layout(Screen) == SCREEN_PLAYERS;
    s32 i;

    memset(Place, 0, sizeof(Place));
    if (top) {
        place_panel(&Place[SUB_PANEL], -0xA0, 0xA0, TOP_Y - 0x10, TOP_Y + NUM_TOP * TOP_STEP + 0xA);
        place_panel(&Place[SUB_PANEL + 1], 0, 0, -0x400, -0x400);
        place_panel(&Place[SUB_STICK_BOX], 0, 0, -0x400, -0x400);
    } else if (players) {
        place_panel(&Place[SUB_PANEL], PLAYERS_LEFT, PLAYERS_RIGHT, PLAYERS_TOP - 0x12, PLAYERS_TOP + PLAYERS * PLAYERS_STEP + 0x10);
        place_panel(&Place[SUB_PANEL + 1], 0, 0, -0x400, -0x400);
        place_panel(&Place[SUB_STICK_BOX], 0, 0, -0x400, -0x400);
    } else {
        place_panel(&Place[SUB_PANEL], CATEGORY_LEFT, CATEGORY_RIGHT, PANEL_TOP, PANEL_BOTTOM);
        place_panel(&Place[SUB_PANEL + 1], OPTION_LEFT, OPTION_RIGHT, PANEL_TOP, PANEL_BOTTOM);
        place_panel(&Place[SUB_STICK_BOX], STICK_BOX_LEFT, STICK_BOX_RIGHT, STICK_BOX_TOP, STICK_BOX_BOTTOM);
    }
    place_text(&Place[SUB_HELP], -0x8000, HELP_Y, "", 1);
    place_text(&Place[SUB_HELP_MORE], -0x8000, HELP_Y + OPTION_STEP, "", 1);
    place_text(&Place[SUB_NAV], -0x8000, NAV_Y, "", 1);
    for (i = 0; i < MAX_CATEGORIES + 1; i++) {
        if (top) {
            place_text(&Place[SUB_CATEGORY + i], -0x8000, TOP_Y + i * TOP_STEP, "", 0);
        } else {
            place_text(&Place[SUB_CATEGORY + i], CATEGORY_LEFT + 0x14, OPTION_TOP + i * CATEGORY_STEP, "", 0);
        }
    }
    for (i = 0; i < VISIBLE_OPTIONS; i++) {
        s32 y = OPTION_TOP + i * OPTION_STEP;

        if (players && i < PLAYERS) {
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

/* What there is to say, over the help's two lines: why an online game ended, or couldn't be joined. */
static void say_message(const char *text) {
    char all[HELP_CHARS * 2 + 2];
    size_t cut;

    Message[0][0] = Message[1][0] = '\0';
    if (text == NULL) {
        return;
    }
    if (strncmp(text, "Online play: ", 13) == 0) {
        text += 13;
    }
    font_text(all, sizeof(all), text);
    if (strlen(all) <= HELP_CHARS) {
        memcpy(Message[0], all, strlen(all) + 1);
        return;
    }
    for (cut = HELP_CHARS; cut > 0 && all[cut] != ' '; cut--) {
    }
    cut = cut == 0 ? HELP_CHARS : cut;
    memcpy(Message[0], all, cut);
    Message[0][cut] = '\0';
    font_text(Message[1], sizeof(Message[1]), all + cut);
}

/* Under the host's lobby: how the others join, two lines at a time, a page of them for five seconds where there's more. Players
   elsewhere need the router to let them in, which the game has asked of it (portmap.c): the join code if it has, and if it hasn't,
   that it hasn't and what can be done. Players on this network need neither: the game shows in their JOIN GAME by itself. And if
   this PC is on a virtual network (Hamachi and its like: net.c), players on that join at its address there, whatever the router
   said: that's what such a network is for, so then it's said in place of what else could be done. */
static void router_lines(void) {
    static s32 said = -1, said_networks = -1;
    s32 router = net_lobby_router(), networks = 0, n = 0, page;
    char line[8][96], address[32], own[32];
    const char *name = "";

    net_lobby_network(0, own, sizeof(own), &name);
    while (networks < 2 && net_lobby_network(networks + 1, address, sizeof(address), &name)) {
        networks++;
    }
    if (router == PORTMAP_OPEN) {
        snprintf(line[n++], sizeof(line[0]), "Players elsewhere join with the code %s", net_lobby_code());
        snprintf(line[n++], sizeof(line[0]), "On your network, the game shows by itself");
    } else if (router == PORTMAP_WORKING) {
        snprintf(line[n++], sizeof(line[0]), "Asking your router to let players elsewhere in");
        snprintf(line[n++], sizeof(line[0]), "On your network, the game shows by itself");
    } else if (networks > 0) {
        /* below */
    } else if (router == PORTMAP_SHARED) {
        snprintf(line[n++], sizeof(line[0]), "Your provider shares your address with others,");
        snprintf(line[n++], sizeof(line[0]), "so only players on your network can join");
        snprintf(line[n++], sizeof(line[0]), "Someone else could host, or you could all use");
        snprintf(line[n++], sizeof(line[0]), "a free virtual network such as Tailscale");
    } else if (router == PORTMAP_NO_ROUTER) {
        /* nothing answered at all: a phone's hotspot, a shared network, or a router with this turned off. It's not "your router":
           there may be none */
        snprintf(line[n++], sizeof(line[0]), "This network can't let players elsewhere in.");
        snprintf(line[n++], sizeof(line[0]), "On your network, they join at %s", own);
        snprintf(line[n++], sizeof(line[0]), "You can still join a game someone else hosts,");
        snprintf(line[n++], sizeof(line[0]), "or all use a virtual network such as Tailscale");
    } else if (router == PORTMAP_REFUSED) {
        snprintf(line[n++], sizeof(line[0]), "Your router won't let players elsewhere in.");
        snprintf(line[n++], sizeof(line[0]), "On your network, they join at %s", own);
        snprintf(line[n++], sizeof(line[0]), "Someone else could host, or open UDP port %d", NET_PORT);
        snprintf(line[n++], sizeof(line[0]), "on it by hand, or all use a VPN like Tailscale");
    } else {
        snprintf(line[n++], sizeof(line[0]), "The others join at %s", own);
    }
    if (networks > 0) {
        s32 i;

        for (i = 1; i <= networks; i++) {
            net_lobby_network(i, address, sizeof(address), &name);
            snprintf(line[n++], sizeof(line[0]), "On %s, they join at %s", name, address);
        }
        if (router != PORTMAP_OPEN && router != PORTMAP_WORKING) {
            snprintf(line[n++], sizeof(line[0]), "On your network, they join at %s", own);
        }
    }
    if (n & 1) {
        line[n++][0] = '\0';
    }
    page = (GameFrameCount / 300) % (n / 2);
    font_text(Help[0], sizeof(Help[0]), line[page * 2]);
    font_text(Help[1], sizeof(Help[1]), line[page * 2 + 1]);
    if (settings()->no_save && router != said && router != PORTMAP_WORKING) {
        said = router;
        printf("launcher: router %s\n", router == PORTMAP_OPEN ? "open" : router == PORTMAP_SHARED ? "shared" : router == PORTMAP_REFUSED ? "refused"
                                         : router == PORTMAP_NO_ROUTER ? "none" : "not asked");
    }
    if (settings()->no_save && networks != said_networks) {
        s32 i;

        said_networks = networks;
        for (i = 1; i <= networks; i++) {
            net_lobby_network(i, address, sizeof(address), &name);
            printf("launcher: on %s, they join at %s\n", name, address);
        }
    }
}

/* The host's lobby, the games to join, and the wait once joined: rows in the players screen's places. */
static void show_online(subengine *sub) {
    s32 state = net_lobby_poll(), found = 0, machines = net_lobby_machines(), first = 0, i;
    char text[64];

    for (i = 0; i < MAX_CATEGORIES + 1; i++) {
        set_row(&sub[SUB_CATEGORY + i], "", TEXT_LARGE);
    }
    for (i = 0; i < VISIBLE_OPTIONS; i++) {
        Values[i][0] = '\0';
        Labels[i][0] = '\0';
        set_row(&sub[SUB_LABEL(i)], Labels[i], TEXT_LARGE);
        if (i < PLAYERS) {
            sub[SUB_VALUE(i)].unk7C = TEXT_SMALL;
        }
    }
    set_row(&sub[SUB_HELP], Message[0], TEXT_SMALL);
    set_row(&sub[SUB_HELP_MORE], Message[1], TEXT_SMALL);
    if (Screen == SCREEN_HOST || (Screen == SCREEN_JOINED && machines > 0)) {
        /* who's here, the same list on every machine: each one's first player, with this PC said as that. A machine that's joined
           knows the host's address, and of the others only that they're there */
        s32 me = net_lobby_me();

        for (i = 0; i < PLAYERS; i++) {
            s32 pads = i < machines ? net_lobby_machine(i, text, sizeof(text)) : 0;

            if (i < machines && first < PLAYERS) {
                char value[96], who[64];

                snprintf(Labels[i], sizeof(Labels[i]), "PLAYER %d", first + 1);
                if (i == me) {
                    snprintf(who, sizeof(who), "This PC");
                } else if (Screen == SCREEN_HOST) {
                    snprintf(who, sizeof(who), "%s", text);
                } else {
                    snprintf(who, sizeof(who), i == 0 ? "The host, %s" : "Another player%s", text);
                }
                snprintf(value, sizeof(value), pads > 1 ? "%s, %d players" : "%s", who, pads);
                font_text(Values[i], sizeof(Values[i]), value);
                first += pads;
            } else if (i == machines && Screen == SCREEN_HOST) {
                font_text(Values[i], sizeof(Values[i]), "Waiting for someone to join");
            }
        }
    }
    if (Screen == SCREEN_HOST) {
        router_lines();
        if (Message[0][0] == '\0') {
            set_row(&sub[SUB_HELP], Help[0], TEXT_SMALL);
            set_row(&sub[SUB_HELP_MORE], Help[1], TEXT_SMALL);
        }
        set_row(&sub[SUB_NAV], state == LOBBY_STARTING ? "STARTING" : machines >= 2 ? "START TO PLAY, \xFE BACK" : "\xFE BACK", TEXT_SMALL);
    } else if (Screen == SCREEN_JOIN) {
        while (found < PLAYERS - 1 && net_lobby_found(found, text, sizeof(text)) != 0) {
            char value[96];

            snprintf(Labels[found], sizeof(Labels[found]), "GAME %d", found + 1);
            snprintf(value, sizeof(value), "%s, %d here", text, net_lobby_found(found, text, sizeof(text)));
            font_text(Values[found], sizeof(Values[found]), value);
            found++;
        }
        JoinCursor = JoinCursor > found ? found : JoinCursor;
        snprintf(Labels[found], sizeof(Labels[found]), "CODE");
        snprintf(Values[found], sizeof(Values[found]), Typing && (GameFrameCount & 0x10) ? "%s<" : "%s", Address);
        for (i = 0; i <= found; i++) {
            set_row(&sub[SUB_LABEL(i)], Labels[i], i == JoinCursor ? TEXT_LARGE_LIT : TEXT_LARGE);
            sub[SUB_VALUE(i)].unk7C = i == JoinCursor ? TEXT_SMALL_LIT : TEXT_SMALL;
        }
        if (Message[0][0] == '\0') {
            set_row(&sub[SUB_HELP], Typing ? "THE HOST'S JOIN CODE, OR ITS ADDRESS" : found == 0 ? "A GAME ON YOUR NETWORK SHOWS HERE BY ITSELF" : "", TEXT_SMALL);
        }
        set_row(&sub[SUB_NAV], Typing ? "TYPE IT, THEN START TO JOIN" : "UP - DOWN CHOOSE, \xFC BUTTON TO JOIN, \xFE BACK", TEXT_SMALL);
    } else {
        static s32 said_machines, said_me;

        if (machines == 0) {
            /* till the host has said who's there */
            snprintf(Labels[0], sizeof(Labels[0]), "JOINED");
            font_text(Values[0], sizeof(Values[0]), Joining);
        }
        if (settings()->no_save && (machines != said_machines || net_lobby_me() != said_me)) {
            said_machines = machines;
            said_me = net_lobby_me();
            printf("launcher: the lobby has %d, this one %d\n", said_machines, said_me + 1);
        }
        set_row(&sub[SUB_HELP], state == LOBBY_ASKING ? "ASKING TO JOIN" : "WAITING FOR THE HOST TO START", TEXT_SMALL);
        set_row(&sub[SUB_NAV], "\xFE LEAVE", TEXT_SMALL);
    }
}

/* Whether the deadzone's box and picture are up: on the CONTROLS page, with STICK DEADZONE chosen. */
static s32 deadzone_shown(void) {
    return Screen == SCREEN_OPTIONS && Category < NUM_CATEGORIES && Categories[Category].kind == KIND_CONTROLS && Option == ROW_DEADZONE && Waiting < 0;
}

/* What's on screen now, from where the cursor is. */
static void show_screen(void) {
    subengine *sub = GetEngineSlot(0)->sub;
    const MenuCategory *c = Category < NUM_CATEGORIES ? &Categories[Category] : NULL;
    s32 mods = Screen == SCREEN_OPTIONS && c->kind == KIND_MODS;
    s32 controls = c != NULL && c->kind == KIND_CONTROLS;
    s32 data = Screen == SCREEN_OPTIONS && c->kind == KIND_DATA;
    s32 i;

    /* the deadzone's box, while it's chosen (draw_deadzone) */
    if (deadzone_shown()) {
        sub[SUB_STICK_BOX].flags |= 0x8000;
    } else {
        sub[SUB_STICK_BOX].flags &= ~0x8000;
    }

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
    if (Screen == SCREEN_HOST || Screen == SCREEN_JOIN || Screen == SCREEN_JOINED) {
        show_online(sub);
        return;
    }
    if (Screen == SCREEN_ONLINE) {
        for (i = 0; i < MAX_CATEGORIES + 1; i++) {
            set_row(&sub[SUB_CATEGORY + i], i < NUM_ONLINE ? OnlineRows[i] : "", i == OnlineCursor ? TEXT_LARGE_LIT : TEXT_LARGE);
        }
        set_row(&sub[SUB_HELP], Message[0], TEXT_SMALL);
        set_row(&sub[SUB_HELP_MORE], Message[1], TEXT_SMALL);
        set_row(&sub[SUB_NAV], "PRESS UP - DOWN TO CHOOSE, \xFC BUTTON TO SELECT", TEXT_SMALL);
        for (i = 0; i < VISIBLE_OPTIONS; i++) {
            set_row(&sub[SUB_LABEL(i)], "", TEXT_SMALL);
            Values[i][0] = '\0';
        }
        return;
    }
    if (Screen == SCREEN_TOP) {
        for (i = 0; i < MAX_CATEGORIES + 1; i++) {
            set_row(&sub[SUB_CATEGORY + i], i < NUM_TOP ? TopRows[i] : "", i == TopCursor ? TEXT_LARGE_LIT : TEXT_LARGE);
        }
        set_row(&sub[SUB_HELP], Message[0], TEXT_SMALL);
        set_row(&sub[SUB_HELP_MORE], Message[1], TEXT_SMALL);
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
                           : mods || (data && Option >= ROW_CHOOSE) ? "UP - DOWN CHOOSE, \xFC BUTTON TO OPEN, \xFE BACK"
                           : controls && Waiting >= 0 ? ""
                           /* a CONTROLS row's own: the help over it says how (the user's word on what was there: "super cluttered") */
                           : controls && Screen == SCREEN_OPTIONS && Option == ROW_DEADZONE ? "UP - DOWN CHOOSE, LEFT - RIGHT CHANGE, \xFE BACK"
                           : controls && Screen == SCREEN_OPTIONS && Option == ROW_RESET ? "UP - DOWN CHOOSE, \xFC BUTTON TO RESET, \xFE BACK"
                           : controls         ? "UP - DOWN CHOOSE, \xFE BACK"
                           : Screen == SCREEN_OPTIONS ? "UP - DOWN CHOOSE, LEFT - RIGHT CHANGE, \xFE BACK"
                                                      : "UP - DOWN TO CHOOSE, RIGHT OR \xFC BUTTON TO OPEN", TEXT_SMALL);
    set_row(&sub[SUB_HELP], Screen == SCREEN_OPTIONS ? option_help(c, Option) : "", TEXT_SMALL);
    if (mods || data) {
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
            snprintf(Values[i], sizeof(Values[i]), lit && value[0] != '\0' && (!controls || n == ROW_DEADZONE) && !(data && n >= ROW_CHOOSE) ? "< %s >" : "%s", value);
            sub[SUB_VALUE(i)].unk7C = lit || (controls && Waiting >= 0 && Waiting == ROW_CONTROL(n)) ? TEXT_SMALL_LIT : TEXT_SMALL;
            /* (the deadzone's name is longer than a button's, so its value is further out) */
            sub[SUB_VALUE(i)].XYZ.x = controls ? (n == ROW_DEADZONE ? DEADZONE_VALUE_X : CONTROL_VALUE_X) : VALUE_X;
        } else {
            set_row(&sub[SUB_LABEL(i)], "", TEXT_SMALL);
        }
    }
}

/* The top level, the settings and the players screen lay out their rows differently. */
static s32 layout(s32 screen) {
    return screen == SCREEN_OPTIONS ? SCREEN_CATEGORIES : screen == SCREEN_ONLINE ? SCREEN_TOP
           : screen == SCREEN_HOST || screen == SCREEN_JOIN || screen == SCREEN_JOINED ? SCREEN_PLAYERS : screen;
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
    if (settings()->test_restart) {
        platform_restart(0);
    }
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

/* --- online --- */

static void open_join(void) {
    JoinCursor = 0;
    JoinMoved = JoinFound = 0;
    Typing = 0;
    net_lobby_find();
    go_to_screen(SCREEN_JOIN);
}

static void update_online(s32 pressed) {
    if ((pressed & 0x40) && OnlineCursor < NUM_ONLINE - 1) {
        OnlineCursor++;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x10) && OnlineCursor > 0) {
        OnlineCursor--;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x1000) || ((pressed & (0x4000 | 8)) && OnlineCursor == ONLINE_BACK)) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        say_message(NULL);
        go_to_screen(SCREEN_TOP);
    } else if (pressed & (0x4000 | 8)) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        say_message(NULL);
        if (OnlineCursor == ONLINE_JOIN) {
            open_join();
        } else if (net_lobby_host(NULL)) {
            if (settings()->no_save) {
                printf("launcher: hosting\n");
            }
            go_to_screen(SCREEN_HOST);
        } else {
            say_message(net_lobby_problem());
        }
    }
}

/* The host's lobby: START once someone's here, and the game begins when they've all taken the start. */
static void update_host(s32 pressed) {
    s32 state = net_lobby_poll();

    if (state == LOBBY_GO) {
        platform_online_start();
    } else if (state == LOBBY_FAILED) {
        say_message(net_lobby_problem());
        go_to_screen(SCREEN_ONLINE);
    } else if (state == LOBBY_OPEN && (pressed & 0x1000)) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        net_lobby_leave();
        go_to_screen(SCREEN_ONLINE);
    } else if (state == LOBBY_OPEN && (pressed & (0x4000 | 8)) && net_lobby_machines() >= 2) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        net_lobby_start();
    }
}

static void join(const char *where) {
    say_message(NULL);
    snprintf(Joining, sizeof(Joining), "%s", where);
    if (net_lobby_join(where)) {
        if (settings()->no_save) {
            printf("launcher: joining %s\n", where);
        }
        go_to_screen(SCREEN_JOINED);
    } else {
        say_message(net_lobby_problem());
        net_lobby_find();
    }
}

/* The games found, and under them the host's join code to type, or its address: letters, digits and dots from the keyboard, or with
   a controller up and down for the last character, right for another and left to take one off. While it's typed a key is a
   character and not the button it's set to: START joins (Enter, by default) and Esc goes back, or a controller's own Cross and
   Triangle. */
static void update_join(s32 pressed) {
    static const char pick[] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ.";
    s32 pad;
    size_t n = strlen(Address);
    char text[64];
    s32 found = 0, typed;

    net_lobby_poll();
    while (found < PLAYERS - 1 && net_lobby_found(found, text, sizeof(text)) != 0) {
        found++;
    }
    if (settings()->no_save && found > JoinFound) {
        net_lobby_found(found - 1, text, sizeof(text));
        printf("launcher: found a game at %s\n", text);
    }
    /* till it's moved, the cursor's on the first game there is; once on the address, it stays there as games come and go */
    if (!JoinMoved) {
        JoinCursor = 0;
    } else if (JoinCursor == JoinFound || JoinCursor > found) {
        JoinCursor = found;
    }
    JoinFound = found;
    if (!Typing) {
        if ((pressed & 0x40) && JoinCursor < found) {
            JoinCursor++;
            JoinMoved = 1;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if ((pressed & 0x10) && JoinCursor > 0) {
            JoinCursor--;
            JoinMoved = 1;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pressed & 0x1000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            net_lobby_leave();
            say_message(NULL);
            go_to_screen(SCREEN_ONLINE);
        } else if (pressed & (0x4000 | 8)) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            if (JoinCursor < found) {
                net_lobby_found(JoinCursor, text, sizeof(text));
                join(text);
            } else {
                Typing = 1;
                JoinMoved = 1;
                host_typed(); /* not a key from before, nor a controller's press */
                host_controller_pressed(0);
            }
        }
        return;
    }
    typed = host_typed();
    pad = host_controller_pressed(0);
    if (typed == 27 || (pad & 0x1000)) {
        Typing = 0;
    } else if (typed == 8 || (typed == 0 && (pressed & 0x80))) {
        if (n > 0) {
            Address[n - 1] = '\0';
        }
    } else if (typed != 0) {
        if (n < ADDRESS_CHARS) {
            Address[n] = (char)typed;
            Address[n + 1] = '\0';
        }
    } else if ((pressed & 0x20) && n < ADDRESS_CHARS) {
        Address[n] = n > 0 ? Address[n - 1] : '0';
        Address[n + 1] = '\0';
    } else if ((pressed & (0x10 | 0x40)) && n > 0) {
        const char *at = strchr(pick, Address[n - 1]);
        s32 i = at != NULL ? (s32)(at - pick) : 0;

        Address[n - 1] = pick[(i + ((pressed & 0x10) ? 1 : (s32)sizeof(pick) - 2)) % ((s32)sizeof(pick) - 1)];
    } else if (((pressed & 8) || (pad & 0x4000)) && n > 0) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        Typing = 0;
        join(Address);
    }
}

/* Joined, and waiting for the host's START: the game begins when it comes. */
static void update_joined(s32 pressed) {
    s32 state = net_lobby_poll();

    if (state == LOBBY_GO) {
        platform_online_start();
    } else if (state == LOBBY_FAILED) {
        say_message(net_lobby_problem());
        open_join();
    } else if (pressed & 0x1000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        net_lobby_leave();
        open_join();
    }
}

static void update_top(s32 pressed) {
    if (pressed != 0) {
        say_message(NULL);
    }
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
        case TOP_ONLINE:
            OnlineCursor = 0;
            go_to_screen(SCREEN_ONLINE);
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
    if (Option == ROW_DEADZONE) {
        /* ORIGINAL, the game's own, then 10% to 60% in tens, round again either way */
        Settings *s = settings();
        s32 steps = STICK_DEADZONE_MOST / 10 + 1, at = s->stick_deadzone / 10;

        if (pressed & (0x20 | 0x80 | 0x4000)) {
            at = (at + ((pressed & 0x80) ? steps - 1 : 1)) % steps;
            s->stick_deadzone = at * 10;
            settings_save();
            if (s->no_save) {
                printf("controls: stick deadzone %d\n", s->stick_deadzone);
            }
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        return;
    }
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
    Waiting = ROW_CONTROL(Option);
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
    /* on STICK DEADZONE the stick is for trying out in its box: there only the d-pad and the keys change it or move off it. A stick's
       direction is what the game adds to the buttons, held but not among them (pad_read) */
    if (deadzone_shown()) {
        pressed &= ~(Pads[0].held & ~Pads[0].buttons & 0xF0);
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
    /* a folder picked in the system's chooser, which the page shows as it's taken up */
    if (c->kind == KIND_DATA && folders_poll()) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x40) && Option < option_count(c) - 1) {
        Option++;
        /* the CONTROLS page's line is passed over */
        Option += controls && Option == ROW_LINE;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x10) && Option > 0) {
        Option--;
        Option -= controls && Option == ROW_LINE;
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
    /* the main menu's EXIT, in an online game: the game's over for this machine, and it's back here on its own (it doesn't return:
       the game starts again from its beginning, offline) */
    if (net_on()) {
        platform_online_stop(NULL);
    }
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
    /* why the last online game ended, if it wasn't the players' doing */
    say_message(platform_launcher_message());
    place_screen();
    show_screen();
}

/* --- the mouse (the user's word, 2026-11-10: "we should be able to click around the menu") ---
 *
 * Pointing at a row chooses it, as up and down would, but only when the mouse has moved, so one lying still never fights the keys or
 * a controller; a click is Cross on it, a right click Triangle (back), and the wheel up and down. A row on the other panel of the
 * settings is moved to first: a category clicked opens it, and a setting on a page that isn't open yet is chosen as it opens, for the
 * next click to change. Where a screen's only press is START (HOST GAME's, the players screen's), it's the line at the foot that says
 * so that's clicked, so a stray click doesn't start a game. What's clicked is in the same units the rows are laid out in
 * (gfx_window_point). */
static s32 MouseOption = -1; /* a setting clicked on a page that wasn't open: chosen once it is */

/* The row of a list at top, step apart, that y is on: each row's text from its own top to the next's, a little higher. */
static s32 row_at(float y, s32 top, s32 step, s32 rows) {
    s32 i = (s32)floorf((y - top + 4) / step);

    return y >= top - 4 && i >= 0 && i < rows ? i : -1;
}

static s32 mouse_presses(void) {
    HostMouse m;
    s32 out = 0, row = -1;
    s32 point;

    menu_mouse_take(&m);
    if (Screen == SCREEN_OPTIONS && Waiting >= 0) {
        return 0; /* waiting for a key or a button: not the mouse's */
    }
    if (m.right) {
        out |= 0x1000;
    }
    out |= m.wheel > 0 ? 0x10 : m.wheel < 0 ? 0x40 : 0;
    if (!m.on) {
        return out;
    }
    point = m.moved || m.left;
    switch (Screen) {
    case SCREEN_TOP:
    case SCREEN_ONLINE:
        if (m.x >= -0xA0 && m.x <= 0xA0) {
            row = row_at(m.y, TOP_Y, TOP_STEP, Screen == SCREEN_TOP ? NUM_TOP : NUM_ONLINE);
        }
        if (row >= 0 && point) {
            *(Screen == SCREEN_TOP ? &TopCursor : &OnlineCursor) = row;
        }
        if (row >= 0 && m.left) {
            out |= 0x4000;
        }
        break;
    case SCREEN_CATEGORIES:
    case SCREEN_OPTIONS:
        if (m.x >= CATEGORY_LEFT && m.x <= CATEGORY_RIGHT) {
            row = row_at(m.y, OPTION_TOP, CATEGORY_STEP, NUM_CATEGORIES + 1);
            if (row >= 0 && Screen == SCREEN_CATEGORIES && point && row != Category) {
                Category = row;
                FirstShown = 0;
            }
            if (row >= 0 && m.left) {
                /* opens it, or BACK goes back, from this panel or the other */
                if (Screen == SCREEN_OPTIONS) {
                    go_to_screen(SCREEN_CATEGORIES);
                }
                Category = row;
                FirstShown = 0;
                out |= 0x4000;
            }
        } else if (m.x >= OPTION_LEFT && m.x <= OPTION_RIGHT && Category < NUM_CATEGORIES) {
            const MenuCategory *c = &Categories[Category];
            s32 n;

            row = row_at(m.y, OPTION_TOP, OPTION_STEP, VISIBLE_OPTIONS);
            n = row >= 0 ? FirstShown + row : -1;
            if (n >= option_count(c) || (c->kind == KIND_CONTROLS && n == ROW_LINE)) {
                n = -1;
            }
            if (n >= 0 && Screen == SCREEN_OPTIONS && point) {
                Option = n;
            }
            if (n >= 0 && m.left) {
                if (Screen == SCREEN_OPTIONS) {
                    out |= 0x4000;
                } else {
                    /* the page opens with this one chosen */
                    MouseOption = n;
                    out |= 0x4000;
                }
            }
        }
        break;
    case SCREEN_JOIN: {
        char text[64];
        s32 found = 0;

        while (found < PLAYERS - 1 && net_lobby_found(found, text, sizeof(text)) != 0) {
            found++;
        }
        if (m.x >= PLAYERS_LEFT && m.x <= PLAYERS_RIGHT) {
            row = row_at(m.y, PLAYERS_TOP, PLAYERS_STEP, found + 1);
        }
        if (row >= 0 && point && !Typing) {
            JoinCursor = row;
            JoinMoved = 1;
        }
        if (row >= 0 && m.left) {
            /* a game is joined, CODE typed in, and the code typed joined */
            out |= Typing ? (row == found ? 8 : 0) : 0x4000;
        }
        break;
    }
    case SCREEN_HOST:
    case SCREEN_PLAYERS:
        if (m.left && m.y >= NAV_Y - 4 && m.y < NAV_Y + 0x14) {
            out |= 8;
        }
        break;
    default:
        break;
    }
    return out;
}

static void SettingsMenuUpdate(void) {
    s32 pressed = Pads[0].pressed | mouse_presses();

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
    case SCREEN_ONLINE:
        update_online(pressed);
        break;
    case SCREEN_HOST:
        update_host(pressed);
        break;
    case SCREEN_JOIN:
        update_join(pressed);
        break;
    case SCREEN_JOINED:
        update_joined(pressed);
        break;
    }
    /* a setting clicked on a page that's open now */
    if (MouseOption >= 0) {
        if (Screen == SCREEN_OPTIONS) {
            Option = MouseOption;
            FirstShown = Option >= VISIBLE_OPTIONS ? Option - VISIBLE_OPTIONS + 1 : 0;
        }
        MouseOption = -1;
    }
    show_screen();
}

/* --- the deadzone, drawn ---
 *
 * While STICK DEADZONE is chosen, a picture of the stick in a box of its own, in the panel's empty right-hand side, so nothing else
 * moves (the user's words, 2026-11-10): a ring for as far as it goes,
 * the deadzone darker inside it, and a dot for where player 1's stick is now, lit once it's far enough out to move a hero. The game's
 * own deadzone is a square, as it's each axis on its own (controls.c), and pokes out past the ring at the corners, where a stick never
 * gets. It's drawn in the panel's units (640 across and 480 down, from the screen's middle), and made the game's screen's. */
#define STICK_X ((STICK_BOX_LEFT + STICK_BOX_RIGHT) / 2)
#define STICK_Y ((STICK_BOX_TOP + STICK_BOX_BOTTOM) / 2)
#define STICK_R 0x26
enum { STICK_RING = 0x20B0FF, STICK_ZONE = 0x1870D0, STICK_IDLE = 0xB0B0B0, STICK_MOVES = 0x40FF40 }; /* blue, green, red */

#define STICK_PI 3.14159265f
#define STICK_CLAMP(v) ((v) < -1.0f ? -1.0f : (v) > 1.0f ? 1.0f : (v))

static void stick_quad(const float x[4], const float y[4], u32 colour, s32 blend) {
    DVECTOR xy[4];
    u32 c[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        xy[i].vx = (s16)((x[i] + 320.0f) * ScreenRect->w / 640.0f);
        xy[i].vy = (s16)((y[i] + 240.0f) / 2.0f);
        c[i] = colour;
    }
    draw_plain_quad(RenderState.layer, xy, c, blend);
}

/* A wedge of a disc, or of a ring from radius r0 out to r1, round a point, from angle a to b */
static void stick_arc(float cx, float cy, float r0, float r1, float a, float b, u32 colour, s32 blend) {
    float x[4] = { cx + r0 * cosf(a), cx + r0 * cosf(b), cx + r1 * cosf(a), cx + r1 * cosf(b) };
    float y[4] = { cy + r0 * sinf(a), cy + r0 * sinf(b), cy + r1 * sinf(a), cy + r1 * sinf(b) };

    stick_quad(x, y, colour, blend);
}

static void draw_deadzone(void) {
    const s32 parts = 48;
    s32 dz = settings()->stick_deadzone, sx, sy, i;
    float step = 2.0f * STICK_PI / parts;

    if (!deadzone_shown() || ScreenRect == NULL) {
        return;
    }
    /* drawn front to back, after the panels: what's drawn later in a layer goes under what's drawn before */
    if (host_controller_stick(0, &sx, &sy)) {
        unsigned char at[2];
        float px = STICK_X + STICK_CLAMP(sx / 32767.0f) * STICK_R, py = STICK_Y + STICK_CLAMP(sy / 32767.0f) * STICK_R;
        s32 moves;

        controls_stick(sx, sy, dz, at);
        moves = at[0] < 0x25 || at[0] >= 0xDB || at[1] < 0x25 || at[1] >= 0xDB;
        for (i = 0; i < parts / 4; i++) {
            stick_arc(px, py, 0.0f, 5.0f, i * step * 4, (i + 1) * step * 4, moves ? STICK_MOVES : STICK_IDLE, GFX_OPAQUE);
        }
    }
    for (i = 0; i < parts; i++) {
        stick_arc(STICK_X, STICK_Y, STICK_R - 1.5f, STICK_R + 1.5f, i * step, (i + 1) * step, STICK_RING, GFX_OPAQUE);
    }
    if (dz == 0) {
        float h = STICK_R * 92.0f / 127.5f;
        float x[4] = { STICK_X - h, STICK_X + h, STICK_X - h, STICK_X + h }, y[4] = { STICK_Y - h, STICK_Y - h, STICK_Y + h, STICK_Y + h };

        stick_quad(x, y, STICK_ZONE, GFX_HALF);
    } else {
        for (i = 0; i < parts; i++) {
            stick_arc(STICK_X, STICK_Y, 0.0f, STICK_R * dz / 100.0f, i * step, (i + 1) * step, STICK_ZONE, GFX_HALF);
        }
    }
}

static void SettingsMenuRender(void) {
    RenderTitlesBackdropAndMenu();
    draw_deadzone();
    gfx_set_viewport(&LogoView, &LogoCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(LOGO_SLOT));
}

static void SettingsMenuQuit(void) {
    dbase_release(&HeroModelBank[LOGO_MODEL]);
}

process_info SettingsMenu = { SettingsMenuInit, SettingsMenuUpdate, SettingsMenuRender, SettingsMenuQuit, 0 };
