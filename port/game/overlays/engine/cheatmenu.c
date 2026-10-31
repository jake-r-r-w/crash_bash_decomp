/* The cheat menu the game's development builds had behind the pause menu. Retail keeps its way in, its flags and its name, but not the menu, so
 * this is rebuilt from the Aug 2000 builds' copy (PAUSE.CPP on the demo disc), drawn with the game's own text where that drew on the
 * debug console. The launcher turns it on (shell_update keeps the cheat word in step). Left out: TWEAK SETTINGS, whose variables are gone,
 * and the flags nothing reads any more (force_victory, pick_characters, all_movies). */
#include <stdio.h>
#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/pause.h"
#include "bash/hero.h"
#include "bash/shell.h"
#include "bash/cheatmenu.h"

extern void **LangLevelNames[];

enum { PAGE_TOP, PAGE_LEVELS, PAGE_FLAGS, PAGE_AWARDS };

static const char *const TopRows[] = { "LEVEL SELECTOR", "CHEAT FLAGS", "TWEAK AWARDS", "EXIT" };
#define NUM_TOP 4

/* The prototype's flags in its order, then the others the game still reads. */
static const struct {
    const char *text;
    s32 mask;
} Flags[] = {
    { "NO AI BOTS", cheat_mode_no_aibots },
    { "NO HARM HERO", cheat_mode_no_harm_hero },
    { "NO DEATH", cheat_mode_no_death },
    { "FREEZE TIME", cheat_mode_freeze_time },
    { "NO RUMBLE", cheat_mode_no_rumble },
    { "LIMIT CAMERA", cheat_mode_limit_camera },
    { "HEALTH NUMS", cheat_mode_health_nums },
    { "LOOP LEVEL", cheat_mode_loop_level },
    { "FORCE DEATH", cheat_mode_force_death },
    { "NUMBERS", cheat_mode_numbers },
    { "NO WINNERS", cheat_mode_no_winners },
    { "NO HOLES", cheat_mode_no_holes },
    { "ALL HUB LEVELS", cheat_mode_all_hub_levels },
};
#define NUM_FLAGS 13

/* Each award's row of adventure.won, as the prototype had them: a relic is gold at 1 and platinum at 2. */
static const struct {
    const char *text;
    s32 row;
} Awards[] = { { "TROPHY", 0 }, { "GEM", 2 }, { "RELIC", 1 }, { "CRYSTAL", 3 }, { "BOSS", 4 } };
#define NUM_AWARDS 5

/* The modes to jump to, by LevelNames: the games, then the warp rooms. */
#define NUM_GAME_MODES 0x20
#define FIRST_WARP_MODE 0x21
#define NUM_WARP_MODES 5
#define NUM_LEVELS (NUM_GAME_MODES + NUM_WARP_MODES)
#define LEVEL_MODE(i) ((i) < NUM_GAME_MODES ? (i) : FIRST_WARP_MODE + (i) - NUM_GAME_MODES)

#define VISIBLE_ROWS 7
#define ROW_Y 0x3C
#define ROW_STEP 0x18
#define TOP_ROW_Y 0x4A
#define TOP_ROW_STEP 0x28 /* the large font */
#define LABEL_X 0x58
#define VALUE_X 0x170
#define TEXT_ON 0xD0FF
#define TEXT_OFF 0xFF

static s32 Page;
static s32 Cursor;
static s32 FirstRow;
static s32 Award;

static s32 row_count(void) {
    switch (Page) {
    case PAGE_LEVELS:
        return NUM_LEVELS;
    case PAGE_FLAGS:
        return NUM_FLAGS;
    case PAGE_AWARDS:
        return NUM_GAME_MODES;
    default:
        return NUM_TOP;
    }
}

static const char *level_name(s32 mode) {
    return (const char *)LangLevelNames[GameLanguage][mode];
}

static void go_to_page(s32 page, s32 cursor) {
    Page = page;
    Cursor = cursor;
    FirstRow = cursor < VISIBLE_ROWS ? 0 : cursor - VISIBLE_ROWS + 1;
}

static void leave_menu(void) {
    Page = PAGE_TOP;
    Cursor = 0;
    FirstRow = 0;
    PauseChoice = 0;
    GAME->pause &= ~0x10000;
}

static void play_level(s32 mode) {
    GAME->exitFlags = 0x8008;
    PausePad = 2;
    NextGameMode = mode;
    GAME->pause &= ~0x10001;
    Page = PAGE_TOP;
    Cursor = 0;
    FirstRow = 0;
    PauseChoice = 0;
}

static void toggle_award(s32 mode) {
    u8 *won = &GAME->adventure.won[Awards[Award].row][mode];

    *won = Awards[Award].row == 1 ? (*won + 1) % 3 : !*won;
}

/* The pause menu's update, while the menu is open. */
void CheatMenuUpdate(void) {
    s32 pressed = HPADS[PausePad].pressed;
    s32 count = row_count();

    if ((pressed & 0x10) && Cursor > 0) {
        Cursor--;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((pressed & 0x40) && Cursor < count - 1) {
        Cursor++;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (Cursor < FirstRow) {
        FirstRow = Cursor;
    }
    if (Cursor >= FirstRow + VISIBLE_ROWS) {
        FirstRow = Cursor - VISIBLE_ROWS + 1;
    }
    if (pressed & 0x1000) {
        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
        if (Page == PAGE_TOP) {
            leave_menu();
        } else {
            go_to_page(PAGE_TOP, Page - 1);
        }
        return;
    }
    switch (Page) {
    case PAGE_TOP:
        if (pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            if (Cursor == NUM_TOP - 1) {
                leave_menu();
            } else {
                go_to_page(Cursor + 1, 0);
            }
        }
        break;
    case PAGE_LEVELS:
        if (pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            play_level(LEVEL_MODE(Cursor));
        }
        break;
    case PAGE_FLAGS:
        if (pressed & (0x4000 | 0xA0)) {
            GAME->cheats ^= Flags[Cursor].mask;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case PAGE_AWARDS:
        if (pressed & 0x80) {
            Award = (Award + NUM_AWARDS - 1) % NUM_AWARDS;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pressed & 0x20) {
            Award = (Award + 1) % NUM_AWARDS;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pressed & 0x4000) {
            toggle_award(Cursor);
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    }
}

static const char *award_value(s32 mode) {
    s32 won = GAME->adventure.won[Awards[Award].row][mode];

    if (Awards[Award].row == 1 && won >= 2) {
        return "PLATINUM";
    }
    if (Awards[Award].row == 1 && won == 1) {
        return "GOLD";
    }
    return won != 0 ? "WON" : "-";
}

static void draw_row(s32 n, s32 y, s32 lit) {
    const char *label = "";
    const char *value = "";

    switch (Page) {
    case PAGE_TOP:
        label = TopRows[n];
        break;
    case PAGE_LEVELS:
        label = level_name(LEVEL_MODE(n));
        break;
    case PAGE_FLAGS:
        label = Flags[n].text;
        value = (GAME->cheats & Flags[n].mask) ? "ON" : "OFF";
        break;
    case PAGE_AWARDS:
        label = level_name(n);
        value = award_value(n);
        break;
    }
    if (Page == PAGE_TOP) {
        DrawSysText(-0x8000, (s16)y, (u8 *)label, TEXT_ON, TEXT_OFF, lit);
        return;
    }
    DrawSysText(LABEL_X, (s16)y, (u8 *)label, TEXT_ON, TEXT_OFF, lit);
    if (value[0] != '\0') {
        DrawSysText(VALUE_X, (s16)y, (u8 *)value, TEXT_ON, TEXT_OFF, lit);
    }
}

/* The pause menu's drawing, while the menu is open. */
void CheatMenuDraw(s32 top) {
    static char title[32];
    const char *help = "\xFC BUTTON TO SELECT, \xFE TO GO BACK";
    s32 count = row_count();
    s32 i;

    DrawHudEntries(PauseVideoMenu, top);
    if (PauseTextHold != 0) {
        return;
    }
    switch (Page) {
    case PAGE_LEVELS:
        snprintf(title, sizeof(title), "LEVEL SELECTOR");
        help = "\xFC BUTTON TO PLAY THE LEVEL, \xFE TO GO BACK";
        break;
    case PAGE_FLAGS:
        snprintf(title, sizeof(title), "CHEAT FLAGS");
        help = "LEFT - RIGHT TO CHANGE, \xFE TO GO BACK";
        break;
    case PAGE_AWARDS:
        snprintf(title, sizeof(title), "AWARD: %s", Awards[Award].text);
        help = "LEFT - RIGHT AWARD, \xFC BUTTON CHANGE, \xFE BACK";
        break;
    default:
        snprintf(title, sizeof(title), "CHEAT MENU");
        break;
    }
    DrawSysText(-0x8000, (s16)(0xA + top), (u8 *)title, TEXT_ON, TEXT_OFF, 0);
    FontPage = Page == PAGE_TOP ? 0 : 1;
    for (i = 0; i < VISIBLE_ROWS && FirstRow + i < count; i++) {
        draw_row(FirstRow + i, (Page == PAGE_TOP ? TOP_ROW_Y + i * TOP_ROW_STEP : ROW_Y + i * ROW_STEP) + top, FirstRow + i == Cursor);
    }
    FontPage = 1;
    DrawSysText(-0x8000, (s16)(0xE8 + top), (u8 *)help, TEXT_ON, TEXT_OFF, 0);
}
