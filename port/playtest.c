/* --playtest: plays minigames through to a win with four computer players, as
 * tools/playtest/playtest.mjs does with an emulator on the PS1 build. It writes what the menus would
 * leave (GameState, include/bash/nas.h), taps Cross while the rules pages wait, and for a boss sets the
 * flag beating it would. Run with --no-host --no-pace it takes seconds per mode. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform/platform.h"

extern unsigned char GameInfo[], Heroes[], BossState[];
extern char RetryProcess[];
extern int NextGameMode;
extern struct {
    void *process, *next;
} ProcessControl;

#define FRAMES (*(uint32_t *)(GameInfo + 0x0C))
#define EXIT_FLAGS (*(uint16_t *)(GameInfo + 0x30))
#define MODE (*(int16_t *)(GameInfo + 0x34))
#define HERO_CHOICE (GameInfo + 0x4C)
#define HERO_CPU (GameInfo + 0x5C)
#define WINNER (GameInfo + 0x60)
#define HERO_SIZE 0x6C
#define HERO_HEALTH 0x36
#define BOSS_FLAGS (*(uint16_t *)(BossState + 4))
#define BOSS_HEALTH (*(int16_t *)(BossState + 6))
#define BOSS_FIGHT_FRAMES 150

/* in vblanks */
#define LOAD_LIMIT 12000
#define PLAY_LIMIT 400000
#define STILL_TAP 30
#define STILL_HANG 20000

static const char *const Names[] = {
    "Jungle Bash", "Space Bash", "Snow Bash", "Drain Bash", "Papu Pummel", "Swamp Fox", "Desert Fox", "Jungle Fox",
    "Metal Fox", "Big Bad Fox", "Pogo Painter", "Pogo-a-Gogo", "El Pogo Loco", "Pogo Padlock", "Crashball",
    "Beach Ball", "N. Ballism", "Sky Balls", "Tilt Panic", "Polar Panic", "Melt Panic", "Manic Panic", "Bearminator",
    "Dot Dash", "Toxic Dash", "Splash Dash", "Dante's Dash", "Ring Ding", "Dragon Drop", "Mallet Mash", "Keg Kaboom",
    "Oxide Ride", "Tie-Breaker",
};

static enum { IDLE, TITLES, LOADING, PLAYING } State;
static int Modes[64], NumModes, Next, Mode, Bad;
static int Clock, StillSince, Taps, TapHold;
static uint32_t LastFrames;
static int HasLast, Fight0, HasFight;
static int16_t HeroHealth[4];
static const char *ShotDir;

static int is_boss(int m) {
    return m == 4 || m == 9 || m == 22 || m == 31;
}

void playtest_start(const char *modes, const char *shot_dir) {
    const char *p = modes;

    while (*p != '\0' && NumModes < 64) {
        char *end;
        int a = (int)strtol(p, &end, 10), b = a, i;

        p = end;
        if (*p == '-') {
            b = (int)strtol(p + 1, &end, 10);
            p = end;
        }
        if (*p == ',') {
            p++;
        }
        for (i = a; i <= b && NumModes < 64; i++) {
            Modes[NumModes++] = i;
        }
    }
    ShotDir = shot_dir;
    State = TITLES;
}

int playtest_buttons(void) {
    return TapHold > 4 ? PAD_CROSS : 0;
}

static void result(const char *what) {
    const char *name = Mode < 33 ? Names[Mode] : "?";

    printf("mode %2d %-14s %s frames %u exit 0x%04X winner %d taps %d vblanks %d\n", Mode, name, what, (unsigned)FRAMES,
           EXIT_FLAGS, WINNER[0], Taps, Clock);
    fflush(stdout);
    if (strcmp(what, "WIN") != 0 && strcmp(what, "DRAW") != 0) {
        Bad++;
    }
    if (ShotDir != NULL) {
        char path[512];

        snprintf(path, sizeof(path), "%s/mode%02d.png", ShotDir, Mode);
        platform_shot(path);
    }
}

static void start_next(void) {
    if (Next >= NumModes) {
        printf("%d of %d modes won\n", NumModes - Bad, NumModes);
        fflush(stdout);
        platform_finish(Bad != 0);
    }
    Mode = Modes[Next++];
    HERO_CHOICE[0] = 1;
    HERO_CHOICE[1] = 2;
    HERO_CHOICE[2] = 3;
    HERO_CHOICE[3] = 4;
    memset(HERO_CPU, 1, 4);
    NextGameMode = Mode;
    EXIT_FLAGS = 0x8008;
    Clock = 0;
    Taps = 0;
    TapHold = 0;
    HasLast = 0;
    HasFight = 0;
    State = LOADING;
}

void playtest_vblank(void) {
    int i;

    if (State == IDLE) {
        return;
    }
    Clock++;
    if (TapHold > 0) {
        TapHold--;
    }
    switch (State) {
    case TITLES:
        if (MODE == 0x28 && ProcessControl.process == RetryProcess && FRAMES > 60) {
            start_next();
        }
        break;
    case LOADING:
        if (MODE == Mode && ProcessControl.process == RetryProcess && ProcessControl.next == NULL) {
            if (EXIT_FLAGS & ~0x1000) {
                EXIT_FLAGS &= 0x1000;
            }
            if (is_boss(Mode)) {
                for (i = 0; i < 4; i++) {
                    int16_t h = *(int16_t *)(Heroes + i * HERO_SIZE + HERO_HEALTH);

                    HeroHealth[i] = h < 1 ? 1 : h;
                }
            }
            StillSince = Clock;
            State = PLAYING;
        } else if (Clock > LOAD_LIMIT) {
            result("NO LOAD");
            start_next();
        }
        break;
    case PLAYING:
        memset(HERO_CPU, 1, 4);
        if (is_boss(Mode)) {
            for (i = 0; i < 4; i++) {
                *(int16_t *)(Heroes + i * HERO_SIZE + HERO_HEALTH) = HeroHealth[i];
            }
            if (HasFight && LastFrames - (uint32_t)Fight0 >= BOSS_FIGHT_FRAMES && !(BOSS_FLAGS & 0x8000)) {
                BOSS_FLAGS |= 0x8000;
                BOSS_HEALTH = 0;
            }
        }
        if (EXIT_FLAGS & 0x801) {
            int won = WINNER[0] != 0;

            if (is_boss(Mode)) {
                won = won && (BOSS_FLAGS & 0x8000);
            }
            result(won ? "WIN" : is_boss(Mode) ? "LOST" : "DRAW");
            start_next();
            break;
        }
        if (!HasLast || FRAMES != LastFrames) {
            HasLast = 1;
            LastFrames = FRAMES;
            StillSince = Clock;
        }
        if (is_boss(Mode) && !HasFight) {
            HasFight = 1;
            Fight0 = (int)LastFrames;
        }
        if (Clock - StillSince > STILL_TAP && TapHold == 0) {
            TapHold = 8;
            Taps++;
        }
        if (Clock - StillSince > STILL_HANG) {
            result("HANG");
            start_next();
        } else if (Clock > PLAY_LIMIT) {
            result("TIMEOUT");
            start_next();
        }
        break;
    default:
        break;
    }
}
