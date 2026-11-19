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

/* Each goal: the event the list knows it by, and whether it's reached. The counted ones first, then a trophy for each level, then
   each warp room cleared. */
enum {
    G_TROPHY, G_TROPHIES_10, G_TROPHIES_20, G_TROPHIES_ALL, G_GEM, G_GEMS_10, G_GEMS_20, G_GEMS_ALL, G_CRYSTAL, G_CRYSTALS_10,
    G_CRYSTALS_20, G_CRYSTALS_ALL, G_GOLD, G_GOLD_10, G_GOLD_20, G_GOLD_ALL, G_PLATINUM, G_PLATINUM_10, G_PLATINUM_20, G_PLATINUM_ALL,
    G_PAPU, G_BEARMINATOR, G_BIG_BAD_FOX, G_OXIDE, G_PERCENT_100, G_PERCENT_200, NUM_COUNTED
};
static const char *const Events[NUM_COUNTED] = {
    "first_trophy", "trophies_10", "trophies_20", "all_trophies", "first_gem", "gems_10", "gems_20", "all_gems", "first_crystal",
    "crystals_10", "crystals_20", "all_crystals", "first_gold_relic", "gold_relics_10", "gold_relics_20", "all_gold_relics",
    "first_platinum_relic", "platinum_relics_10", "platinum_relics_20", "all_platinum_relics", "beat_papu_pummel", "beat_bearminator",
    "beat_big_bad_fox", "beat_oxide_ride", "complete_100", "complete_200",
};

/* Each level's trophy, by its mode (LevelNames) */
static const struct {
    s32 level;
    const char *event;
} LevelTrophies[EACH] = {
    { 0x00, "trophy_jungle_bash" }, { 0x01, "trophy_space_bash" }, { 0x02, "trophy_snow_bash" }, { 0x03, "trophy_drain_bash" },
    { 0x05, "trophy_swamp_fox" }, { 0x06, "trophy_desert_fox" }, { 0x07, "trophy_jungle_fox" }, { 0x08, "trophy_metal_fox" },
    { 0x0A, "trophy_pogo_painter" }, { 0x0B, "trophy_pogo_a_gogo" }, { 0x0C, "trophy_el_pogo_loco" }, { 0x0D, "trophy_pogo_padlock" },
    { 0x0E, "trophy_crashball" }, { 0x0F, "trophy_beach_ball" }, { 0x10, "trophy_n_ballism" }, { 0x11, "trophy_sky_balls" },
    { 0x12, "trophy_tilt_panic" }, { 0x13, "trophy_polar_panic" }, { 0x14, "trophy_melt_panic" }, { 0x15, "trophy_manic_panic" },
    { 0x17, "trophy_dot_dash" }, { 0x18, "trophy_toxic_dash" }, { 0x19, "trophy_splash_dash" }, { 0x1A, "trophy_dantes_dash" },
    { 0x1B, "trophy_ring_ding" }, { 0x1C, "trophy_dragon_drop" }, { 0x1D, "trophy_mallet_mash" }, { 0x1E, "trophy_keg_kaboom" },
};

/* The warp rooms (gameflow.c's WarpRoom1Items..WarpRoom5Items): cleared with every trophy in it won and its boss beaten. The secret
   room has no boss. */
#define ROOMS 5
#define NO_BOSS -1
static const struct {
    const char *event;
    s32 boss;
    s32 levels[8]; /* up to -1 */
} Rooms[ROOMS] = {
    { "clear_warp_room_1", PAPU_PUMMEL, { 0x00, 0x0E, 0x0A, 0x13, -1 } },
    { "clear_warp_room_2", BEARMINATOR, { 0x01, 0x0F, 0x0B, 0x12, 0x06, -1 } },
    { "clear_warp_room_3", BIG_BAD_FOX, { 0x02, 0x10, 0x0C, 0x14, 0x08, 0x17, -1 } },
    { "clear_warp_room_4", OXIDE_RIDE, { 0x03, 0x11, 0x0D, 0x15, 0x07, 0x18, 0x1B, -1 } },
    { "clear_secret_warp_room", NO_BOSS, { 0x05, 0x19, 0x1A, 0x1C, 0x1D, 0x1E, -1 } },
};

#define G_LEVEL_TROPHY NUM_COUNTED
#define G_ROOM (G_LEVEL_TROPHY + EACH)
#define NUM_GOALS (G_ROOM + ROOMS)

static const char *goal_event(s32 goal) {
    return goal < NUM_COUNTED ? Events[goal] : goal < G_ROOM ? LevelTrophies[goal - G_LEVEL_TROPHY].event : Rooms[goal - G_ROOM].event;
}

static s32 room_cleared(s32 room) {
    const AdventureState *a = &GAME->adventure;
    s32 i;

    if (Rooms[room].boss != NO_BOSS && a->won[AWARD_BOSS][Rooms[room].boss] == 0) {
        return 0;
    }
    for (i = 0; Rooms[room].levels[i] >= 0; i++) {
        if (a->won[AWARD_TROPHY][Rooms[room].levels[i]] == 0) {
            return 0;
        }
    }
    return 1;
}

/* As count_completion has it, but without setting CompletionBcd, which the game's panel reads: a trophy, a gem, a crystal and a gold
   relic are 1% each, a platinum 2% more, and a boss 8% */
static void count(Counts *c) {
    const AdventureState *a = &GAME->adventure;
    s32 i;

    memset(c, 0, sizeof(*c));
    /* the awards of the 28 levels with doors only, so "all 28" means each of them */
    for (i = 0; i < EACH; i++) {
        s32 l = LevelTrophies[i].level;

        c->trophies += a->won[AWARD_TROPHY][l] != 0;
        c->gems += a->won[AWARD_GEM][l] != 0;
        c->crystals += a->won[AWARD_CRYSTAL][l] != 0;
        c->gold += a->won[AWARD_RELIC][l] != 0;
        c->platinum += a->won[AWARD_RELIC][l] >= 2;
    }
    for (i = 0; i < LEVELS; i++) {
        c->percent +=(a->won[AWARD_TROPHY][i] != 0) + (a->won[AWARD_GEM][i] != 0) + (a->won[AWARD_CRYSTAL][i] != 0)
                      + (a->won[AWARD_RELIC][i] != 0) + 2 * (a->won[AWARD_RELIC][i] >= 2) + 8 * (a->won[AWARD_BOSS][i] != 0);
    }
}

static s32 reached(s32 goal, const Counts *c) {
    const AdventureState *a = &GAME->adventure;

    if (goal >= G_ROOM) {
        return room_cleared(goal - G_ROOM);
    }
    if (goal >= G_LEVEL_TROPHY) {
        return a->won[AWARD_TROPHY][LevelTrophies[goal - G_LEVEL_TROPHY].level] != 0;
    }
    switch (goal) {
    case G_TROPHY: return c->trophies >= 1;
    case G_TROPHIES_10: return c->trophies >= 10;
    case G_TROPHIES_20: return c->trophies >= 20;
    case G_TROPHIES_ALL: return c->trophies >= EACH;
    case G_GEM: return c->gems >= 1;
    case G_GEMS_10: return c->gems >= 10;
    case G_GEMS_20: return c->gems >= 20;
    case G_GEMS_ALL: return c->gems >= EACH;
    case G_CRYSTAL: return c->crystals >= 1;
    case G_CRYSTALS_10: return c->crystals >= 10;
    case G_CRYSTALS_20: return c->crystals >= 20;
    case G_CRYSTALS_ALL: return c->crystals >= EACH;
    case G_GOLD: return c->gold >= 1;
    case G_GOLD_10: return c->gold >= 10;
    case G_GOLD_20: return c->gold >= 20;
    case G_GOLD_ALL: return c->gold >= EACH;
    case G_PLATINUM: return c->platinum >= 1;
    case G_PLATINUM_10: return c->platinum >= 10;
    case G_PLATINUM_20: return c->platinum >= 20;
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
            achievements_event(goal_event(g));
        }
    }
    presence(&c);
}

/* A save's loaded (gameflow.c): its awards aren't the run's, so each goal it reaches is told again after it */
void achievements_loaded(void) {
    memset(Told, 0, sizeof(Told));
    achievements_save_loaded();
}
