/* PC: Recomp Achievements' events (docs/PORT.md, Achievements; the list they map to is port/release/achievements.json). After each
 * step of the game this looks at the Adventure's awards and fires an event for each goal reached: it only reads the game, so a game
 * plays the same with it as without, online on every machine alike. It fires an event each time a goal's been reached and not yet
 * told this run, so a loaded save's goals go too (as Casual, the launcher's rule after a load), and the launcher unlocks each once.
 * Nothing at all happens outside RecompLauncher. */
#include <stdio.h>
#include <string.h>
#include "../../platform/achievements.h"
#include "../../platform/settings.h"
#include "common.h"
#include "bash/main.h"

int platform_online_players(void);
int platform_replaying(void);

/* The Adventure's awards (AdventureState.won): per level, a trophy, a relic (1 gold, 2 platinum), a gem, a crystal, and a boss's win */
#define AWARD_TROPHY 0
#define AWARD_RELIC 1
#define AWARD_GEM 2
#define AWARD_CRYSTAL 3
#define AWARD_BOSS 4
#define LEVELS 0x21
/* The 28 levels with a door of their own have each award but the boss's; the four bosses have only theirs */
#define EACH 28
#define PAPU_PUMMEL 4
#define BIG_BAD_FOX 9
#define BEARMINATOR 0x16
#define OXIDE_RIDE 0x1F
#define CHEAT_MENU_ON 0x10000
#define CHEATS_ALWAYS 0x80000000

typedef struct {
    s32 trophies, gems, crystals, gold, platinum, percent;
} Counts;

/* As count_completion has it, but without setting CompletionBcd, which the game's panel reads: a trophy, a gem, a crystal and a gold
   relic are 1% each, a platinum 2% more, and a boss 8% */
static void count(Counts *c) {
    const AdventureState *a = &GAME->adventure;
    s32 i;

    memset(c, 0, sizeof(*c));
    for (i = 0; i < LEVELS; i++) {
        c->trophies += a->won[AWARD_TROPHY][i] != 0;
        c->gems += a->won[AWARD_GEM][i] != 0;
        c->crystals += a->won[AWARD_CRYSTAL][i] != 0;
        c->gold += a->won[AWARD_RELIC][i] != 0;
        c->platinum += a->won[AWARD_RELIC][i] >= 2;
        c->percent += (a->won[AWARD_TROPHY][i] != 0) + (a->won[AWARD_GEM][i] != 0) + (a->won[AWARD_CRYSTAL][i] != 0)
                      + (a->won[AWARD_RELIC][i] != 0) + 2 * (a->won[AWARD_RELIC][i] >= 2) + 8 * (a->won[AWARD_BOSS][i] != 0);
    }
}

/* Each goal: the event the list knows it by, and whether it's reached */
enum {
    G_TROPHY, G_TROPHIES_10, G_TROPHIES_ALL, G_GEM, G_GEMS_ALL, G_CRYSTAL, G_CRYSTALS_ALL, G_GOLD, G_GOLD_ALL, G_PLATINUM, G_PLATINUM_ALL,
    G_PAPU, G_BEARMINATOR, G_BIG_BAD_FOX, G_OXIDE, G_PERCENT_100, G_PERCENT_200, NUM_GOALS
};
static const char *const Events[NUM_GOALS] = {
    "first_trophy", "trophies_10", "all_trophies", "first_gem", "all_gems", "first_crystal", "all_crystals", "first_gold_relic",
    "all_gold_relics", "first_platinum_relic", "all_platinum_relics", "beat_papu_pummel", "beat_bearminator", "beat_big_bad_fox",
    "beat_oxide_ride", "complete_100", "complete_200",
};

static s32 reached(s32 goal, const Counts *c) {
    const AdventureState *a = &GAME->adventure;

    switch (goal) {
    case G_TROPHY: return c->trophies >= 1;
    case G_TROPHIES_10: return c->trophies >= 10;
    case G_TROPHIES_ALL: return c->trophies >= EACH;
    case G_GEM: return c->gems >= 1;
    case G_GEMS_ALL: return c->gems >= EACH;
    case G_CRYSTAL: return c->crystals >= 1;
    case G_CRYSTALS_ALL: return c->crystals >= EACH;
    case G_GOLD: return c->gold >= 1;
    case G_GOLD_ALL: return c->gold >= EACH;
    case G_PLATINUM: return c->platinum >= 1;
    case G_PLATINUM_ALL: return c->platinum >= EACH;
    case G_PAPU: return a->won[AWARD_BOSS][PAPU_PUMMEL] != 0;
    case G_BEARMINATOR: return a->won[AWARD_BOSS][BEARMINATOR] != 0;
    case G_BIG_BAD_FOX: return a->won[AWARD_BOSS][BIG_BAD_FOX] != 0;
    case G_OXIDE: return a->won[AWARD_BOSS][OXIDE_RIDE] != 0;
    case G_PERCENT_100: return c->percent >= 100;
    case G_PERCENT_200: return c->percent >= 200;
    default: return 0;
    }
}

/* "Pogo Painter" from "POGO PAINTER" */
static void title_case(const char *in, char *out, size_t size) {
    size_t n = 0;
    s32 start = 1;

    for (; *in != '\0' && n + 1 < size; in++) {
        char ch = *in;

        out[n++] = start ? ch : (ch >= 'A' && ch <= 'Z' ? ch - 'A' + 'a' : ch);
        start = ch == ' ' || ch == '-' || ch == '.';
    }
    out[n] = '\0';
}

/* The "now playing" line: the Adventure's level or warp room and how far it's got, or a Battle's level */
static void presence(const Counts *c) {
    extern void *LevelNames[45];
    char name[48] = "", text[128];
    s32 mode = GameMode;

    if (mode >= 0 && mode < 45 && LevelNames[mode] != NULL) {
        title_case(LevelNames[mode], name, sizeof(name));
    }
    if (mode == 40 || name[0] == '\0') {
        snprintf(text, sizeof(text), "In the menus");
    } else if (GAME->levelFlags & 4) {
        snprintf(text, sizeof(text), "Adventure: %s, %d%%", name, c->percent);
    } else {
        snprintf(text, sizeof(text), "%s: %s", platform_online_players() != 0 ? "Online" : "Battle", name);
    }
    achievements_presence(text);
}

static u8 Told[NUM_GOALS];

void achievements_step(void) {
    static s32 casual;
    Counts c;
    s32 g;

    achievements_frame();
    if (!achievements_on()) {
        return;
    }
    /* the cheat menu (which can set awards too), a test's cheats or a replayed online game: Casual from then on */
    if (!casual && (settings()->cheat_menu || (GAME->cheats & ~(CHEATS_ALWAYS | CHEAT_MENU_ON)) != 0 || platform_replaying())) {
        casual = 1;
        achievements_casual(platform_replaying() ? "replay" : "cheats");
    }
    count(&c);
    for (g = 0; g < NUM_GOALS; g++) {
        if (!Told[g] && reached(g, &c)) {
            Told[g] = 1;
            achievements_event(Events[g]);
        }
    }
    presence(&c);
}

/* A save's loaded (gameflow.c): its awards aren't the run's, so each goal it reaches is told again after it */
void achievements_loaded(void) {
    memset(Told, 0, sizeof(Told));
    achievements_save_loaded();
}
