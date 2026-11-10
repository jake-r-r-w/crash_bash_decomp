/* The PC port's entry point (docs/PORT.md).
 *
 *   crashbash [--data CRASHBSH.DAT] [--saves FOLDER] [--no-window] [--frames N]
 *
 * --frames stops after N vblanks. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __ANDROID__
#include <unistd.h>
#endif

#include <SDL3/SDL.h>
#include "platform/platform.h"
#include "platform/controllers.h"
#include "platform/gfx.h"
#include "platform/mods.h"
#include "platform/texpack.h"
#include "platform/settings.h"
#include "platform/saves.h"

int extra_hero_named(const char *id); /* game/main/extraheroes.c: --extra-hero's names */

static int Vblanks;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* The return addresses up the stack, for llvm-symbolizer. */
static void print_stack(CONTEXT c) {
    int i;

                fprintf(stderr, "stack: %llX", (unsigned long long)c.Rip);
    for (i = 0; i < 32; i++) {
        DWORD64 base = 0, frame = 0;
        PVOID handler = NULL;
        PRUNTIME_FUNCTION fn = RtlLookupFunctionEntry(c.Rip, &base, NULL);

        if (fn != NULL) {
            RtlVirtualUnwind(UNW_FLAG_NHANDLER, base, c.Rip, fn, &c, &handler, &frame, NULL);
        } else if (c.Rsp != 0 && !IsBadReadPtr((void *)c.Rsp, 8)) {
            c.Rip = *(DWORD64 *)c.Rsp; /* a leaf function */
            c.Rsp += 8;
        } else {
            break;
        }
        if (c.Rip == 0) {
            break;
        }
        fprintf(stderr, " %llX", (unsigned long long)c.Rip);
    }
    fprintf(stderr, "\n");
    fflush(stderr);
}

/* A crash says what and where. */
static LONG WINAPI crash_report(EXCEPTION_POINTERS *e) {
    uintptr_t image = (uintptr_t)GetModuleHandleA(NULL);

    fflush(stdout); /* the log up to here: on Windows a line-buffered file is really fully buffered */
    fprintf(stderr, "crash: code %08lX, reading %llX, image %llX\n", e->ExceptionRecord->ExceptionCode,
            (unsigned long long)e->ExceptionRecord->ExceptionInformation[1], (unsigned long long)image);
    print_stack(*e->ContextRecord);
    return EXCEPTION_EXECUTE_HANDLER;
}

/* A test run that stops reaching vblanks is stuck, which says where. */
static DWORD WINAPI watchdog(LPVOID game_thread) {
    int seen = -1;

    for (;;) {
        CONTEXT c;

        Sleep(20000);
        if (*(volatile int *)&Vblanks != seen) {
            seen = Vblanks;
            continue;
        }
        c.ContextFlags = CONTEXT_FULL;
        SuspendThread(game_thread);
        GetThreadContext(game_thread, &c);
        fprintf(stderr, "crash: stuck at vblank %d, image %llX\n", seen, (unsigned long long)(uintptr_t)GetModuleHandleA(NULL));
        print_stack(c);
        ExitProcess(3);
    }
}
#endif

int machine_init(void);
void overlays_init(void);
int overlay_is_code(int entry);

static const char *DataPath;
static const char *SavesFolder;
static int NoWindow;
static int Turbo;
static int StopAt = -1;

/* The end of the program, from wherever in the game. On Android the process goes at once: an exit() there tears down shared code that
   Android's own threads still use (its UI thread crashed on a destroyed lock), and Android can keep a process to start the app in again,
   with all the game's state as it was left. */
static void platform_exit(int code) {
    fflush(stdout);
#ifdef __ANDROID__
    _exit(code);
#else
    exit(code);
#endif
}

/* Something's stopped the game from starting, and whoever double-clicked it should hear why. */
void platform_fatal(const char *message) {
    fprintf(stderr, "%s\n", message);
    if (StopAt < 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Crash Bash", message, NULL);
    }
    platform_exit(1);
}

/* The program's folder, where it looks for its data, a disc image, discs/ and mods/. On Android, where the program has no folder of its
   own to see, the app's folder in shared storage (Android/data/PACKAGE/files/), which a PC can reach over USB and adb can write to. */
const char *platform_base_path(void) {
#ifdef SDL_PLATFORM_ANDROID
    static char path[1024];

    if (path[0] == 0) {
        const char *shared = SDL_GetAndroidExternalStoragePath();

        if (shared == NULL) {
            return NULL;
        }
        SDL_snprintf(path, sizeof(path), "%s/", shared);
    }
    return path;
#else
    return SDL_GetBasePath();
#endif
}

/* Where a disc image's copy of the data goes: the user's own folder, as the program's may not be theirs to write to */
static const char *own_data_path(void) {
    static char path[1024];

    if (path[0] == 0) {
        char *dir = SDL_GetPrefPath("CrashBash", "CrashBash");

        SDL_snprintf(path, sizeof(path), "%sCRASHBSH.DAT", dir != NULL ? dir : "");
        SDL_free(dir);
    }
    return path;
}

/* The first of these that exists, unless --data says */
const char *platform_data_path(void) {
    static char beside_exe[1024];
    const char *places[] = {
        "CRASHBSH.DAT",
        beside_exe,
        own_data_path(),
        "build/disc/files/CRASHBSH/CRASHBSH.DAT",
        "../disc/files/CRASHBSH/CRASHBSH.DAT",
        "../../disc/files/CRASHBSH/CRASHBSH.DAT",
    };
    const char *base = platform_base_path();
    size_t i;

    if (DataPath != NULL) {
        return DataPath;
    }
    SDL_snprintf(beside_exe, sizeof(beside_exe), "%sCRASHBSH.DAT", base != NULL ? base : "");
    for (i = 0; i < sizeof(places) / sizeof(places[0]); i++) {
        if (SDL_GetPathInfo(places[i], NULL)) {
            return DataPath = places[i];
        }
    }
    return NULL;
}

void platform_finish(int code) {
    audio_finish();
    if (!NoWindow) {
        host_quit();
    }
    platform_exit(code);
}

/* Once a vblank: the window and the clock */
#define MAX_SHOTS 64
static struct {
    int frame;
    const char *path;
} Shots[MAX_SHOTS];
static int NumShots;

/* --press: buttons held on pad 1 for a few vblanks */
#define MAX_PRESSES 512
#define PRESS_VBLANKS 6
static struct {
    int frame, buttons;
} Presses[MAX_PRESSES];
static int NumPresses;

/* What the pretend controllers do (--pretend-pads): a press held for a few vblanks as --press's are, or pulled out or plugged in */
enum { PAD_PRESS, PAD_UNPLUG, PAD_PLUG, PAD_KEY };
static struct {
    int pad, frame, buttons, kind; /* a pretend key's (--key-press) buttons is its scancode */
} PadEvents[MAX_PRESSES];
static int NumPadEvents;

static void pretend_pads_vblank(void) {
    static int held[4] = { -1, -1, -1, -1 };
    int i, p;

    for (i = 0; i < NumPadEvents; i++) {
        if (PadEvents[i].kind == PAD_KEY) {
            if (Vblanks == PadEvents[i].frame || Vblanks == PadEvents[i].frame + PRESS_VBLANKS) {
                host_pretend_key(PadEvents[i].buttons, Vblanks == PadEvents[i].frame);
            }
        } else if (PadEvents[i].kind != PAD_PRESS && PadEvents[i].frame == Vblanks) {
            host_pretend_plug(PadEvents[i].pad, PadEvents[i].kind == PAD_PLUG);
            held[PadEvents[i].pad] = -1;
        }
    }
    for (p = 0; p < 4; p++) {
        int buttons = 0;

        for (i = 0; i < NumPadEvents; i++) {
            if (PadEvents[i].kind == PAD_PRESS && PadEvents[i].pad == p && Vblanks >= PadEvents[i].frame &&
                Vblanks < PadEvents[i].frame + PRESS_VBLANKS) {
                buttons |= PadEvents[i].buttons;
            }
        }
        if (buttons != held[p]) {
            host_pretend_press(p, buttons);
            held[p] = buttons;
        }
    }
}

void platform_vblank(void) {
    int i;

    Vblanks++;
    for (i = 0; i < NumShots; i++) {
        if (Shots[i].frame == Vblanks) {
            gfx_screenshot(Shots[i].path);
        }
    }
    if (!NoWindow) {
        if (NumPadEvents != 0) {
            pretend_pads_vblank();
        }
        if (!host_poll()) {
            platform_finish(0);
        }
        if (!Turbo) {
            host_wait_vblank();
        }
    }
    if (StopAt >= 0 && Vblanks >= StopAt) {
        printf("%d vblanks\n", Vblanks);
        platform_finish(0);
    }
}

/* --stick: a test's analog pad, with motors, on pad 1: its sticks at these for a few vblanks */
#define MAX_STICKS 256
static struct {
    int frame;
    unsigned char x, y;
} Sticks[MAX_STICKS];
static int NumSticks;

int platform_pad_sticks(int port, unsigned char sticks[4]) {
    int i;

    if (port == 0 && NumSticks > 0) {
        memset(sticks, 0x80, 4);
        for (i = 0; i < NumSticks; i++) {
            if (Vblanks >= Sticks[i].frame && Vblanks < Sticks[i].frame + PRESS_VBLANKS) {
                sticks[2] = Sticks[i].x;
                sticks[3] = Sticks[i].y;
            }
        }
        return 1;
    }
    return NoWindow ? 0 : host_pad_sticks(port, sticks);
}

int platform_pad_rumbles(int port) {
    if (port == 0 && NumSticks > 0) {
        return 1;
    }
    return NoWindow ? 0 : host_pad_rumbles(port);
}

/* A test run doesn't shake a controller left on the desk, and says what it would have done. */
void platform_rumble(int port, int small_on, int big) {
    if (StopAt >= 0 || NoWindow) {
        printf("rumble: pad %d small %d big %d at vblank %d\n", port, small_on, big, Vblanks);
    } else {
        host_rumble(port, small_on, big);
    }
}

/* --macro: a test's pattern of buttons on pad 1, over and over from a vblank on: "HEX:VBLANKS,HEX:VBLANKS,..." */
#define MAX_MACRO 64
static struct {
    int buttons, vblanks;
} Macro[MAX_MACRO];
static int MacroSteps, MacroLength, MacroFrom;

static int macro_buttons(void) {
    int at, i;

    if (MacroLength == 0 || Vblanks < MacroFrom) {
        return 0;
    }
    at = (Vblanks - MacroFrom) % MacroLength;
    for (i = 0; i < MacroSteps; i++) {
        if (at < Macro[i].vblanks) {
            return Macro[i].buttons;
        }
        at -= Macro[i].vblanks;
    }
    return 0;
}

static void read_macro(const char *text) {
    while (*text != '\0' && MacroSteps < MAX_MACRO) {
        char *end;

        Macro[MacroSteps].buttons = (int)strtol(text, &end, 16);
        text = *end == ':' ? end + 1 : end;
        Macro[MacroSteps].vblanks = (int)strtol(text, &end, 10);
        MacroLength += Macro[MacroSteps++].vblanks;
        text = *end == ',' ? end + 1 : end;
    }
}

static int BotButtons;

void platform_bot_buttons(int buttons) {
    BotButtons = buttons;
}

/* --stall: the fight's stuck, so finish on the next picture with a shot of it. */
static const char *StallShot;
static int Stalled;

void platform_stalled(void) {
    if (!Stalled) {
        Stalled = 1;
        if (StallShot != NULL) {
            gfx_screenshot(StallShot);
        }
        StopAt = Vblanks + 6;
    }
}

int platform_vblanks(void) {
    return Vblanks;
}

/* --dump-files: the archive's files but its code, named as a mod's files/ has them, for making one. */
static int dump_files(const char *folder) {
    int entry, n = 0;

    SDL_CreateDirectory(folder);
    for (entry = 0; entry < archive_entries(); entry++) {
        char path[1100];
        unsigned char *bytes;
        int size = 0;
        SDL_IOStream *io;

        if (overlay_is_code(entry) || archive_entry_size(entry) <= 0 || (bytes = archive_own_file(entry, &size)) == NULL) {
            continue;
        }
        SDL_snprintf(path, sizeof(path), "%s/%03X.bin", folder, entry);
        io = SDL_IOFromFile(path, "wb");
        if (io != NULL) {
            n += SDL_WriteIO(io, bytes, (size_t)size) == (size_t)size;
            SDL_CloseIO(io);
        }
        free(bytes);
    }
    printf("%d files in %s\n", n, folder);
    return n != 0 ? 0 : 1;
}

int platform_pad(int port) {
    int b = NoWindow ? (port == 0 ? 0 : -1) : host_pad(port);
    int i;

    if (port == 0) {
        if (macro_buttons() != 0) {
            b = (b < 0 ? 0 : b) | macro_buttons();
        }
        if (BotButtons != 0) {
            b = (b < 0 ? 0 : b) | BotButtons;
        }
        for (i = 0; i < NumPresses; i++) {
            if (Vblanks >= Presses[i].frame && Vblanks < Presses[i].frame + PRESS_VBLANKS) {
                b = (b < 0 ? 0 : b) | Presses[i].buttons;
            }
        }
    }
    return b;
}

int main(int argc, char **argv) {
    const char *Image = NULL;
    const char *dump_files_to = NULL;
    int antialias = 0, smooth = 0, widescreen = 0, upscale = 0, level_flags = 0, sharp_icons = 0;
    int i;
    int window_w = 0, window_h = 0, render_w = -1, render_h = -1, cheat_menu = 0, boss_beat = 0, bot = 0, cheats = 0, aim = 0, stall = 0;
    int extra[4] = { 0 }, extra_heroes = 0;
    int draw_every = 0, test_sound = 0, test_adventure = 0, test_challenge = -1, no_launcher = 0, launcher = 0, skip_intro = 0, skip_loading = 0, testing = 0, fps = 0, test_mode = -1, tile = 0, tiles = 0;

#ifdef SDL_PLATFORM_ANDROID
    /* Android sends a program's output nowhere: it goes in log.txt in its folder, for test runs and for anyone to send with a report */
    if (platform_base_path() != NULL) {
        char log[1100];

        SDL_snprintf(log, sizeof(log), "%slog.txt", platform_base_path());
        if (freopen(log, "w", stdout) != NULL) {
            setvbuf(stdout, NULL, _IOLBF, 0);
            dup2(fileno(stdout), fileno(stderr));
        }
        /* The game makes its discs and mods folders itself, so they're there to put things in, and are the app's: Android doesn't let
           an app look in a folder something else made in its own (one adb made was "No such directory" to it) */
        SDL_snprintf(log, sizeof(log), "%sdiscs", platform_base_path());
        SDL_CreateDirectory(log);
        SDL_snprintf(log, sizeof(log), "%smods", platform_base_path());
        SDL_CreateDirectory(log);
    }
#endif
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0) {
            printf("crashbash %s\n", BASH_VERSION);
            return 0;
        } else if (strcmp(argv[i], "--check-sdl") == 0) {
            printf("SDL %d\n", SDL_GetVersion());
            return 0;
        } else if (strcmp(argv[i], "--data") == 0 && i + 1 < argc) {
            DataPath = argv[++i];
        } else if (strcmp(argv[i], "--image") == 0 && i + 1 < argc) {
            Image = argv[++i];
        } else if (strcmp(argv[i], "--stick") == 0 && i + 3 < argc && NumSticks < MAX_STICKS) {
            Sticks[NumSticks].frame = atoi(argv[++i]);
            Sticks[NumSticks].x = (unsigned char)strtol(argv[++i], NULL, 16);
            Sticks[NumSticks++].y = (unsigned char)strtol(argv[++i], NULL, 16);
        } else if (strcmp(argv[i], "--window") == 0 && i + 2 < argc) {
            window_w = atoi(argv[++i]);
            window_h = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--render") == 0 && i + 2 < argc) {
            render_w = atoi(argv[++i]);
            render_h = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--macro") == 0 && i + 2 < argc) {
            MacroFrom = atoi(argv[++i]);
            read_macro(argv[++i]);
        } else if (strcmp(argv[i], "--stall") == 0 && i + 2 < argc) {
            stall = atoi(argv[++i]);
            StallShot = argv[++i];
        } else if (strcmp(argv[i], "--aim") == 0) {
            aim = 1;
        } else if (strcmp(argv[i], "--adventure") == 0) {
            test_adventure = 1;
        } else if (strcmp(argv[i], "--adventure-load") == 0) {
            test_adventure = 2;
        } else if (strcmp(argv[i], "--adventure-resume") == 0) {
            test_adventure = 3;
        } else if (strcmp(argv[i], "--challenge") == 0 && i + 2 < argc) {
            test_adventure = 1;
            test_challenge = atoi(argv[i + 1]) * 8 + atoi(argv[i + 2]);
            i += 2;
        } else if (strcmp(argv[i], "--bot") == 0) {
            bot = 1;
        } else if (strcmp(argv[i], "--extra-heroes") == 0) {
            extra_heroes = 1;
        } else if (strcmp(argv[i], "--extra-hero") == 0 && i + 2 < argc) {
            int player = atoi(argv[i + 1]);

            if (player >= 0 && player < 4 && (extra[player] = extra_hero_named(argv[i + 2])) == 0) {
                fprintf(stderr, "--extra-hero: no extra hero called %s\n", argv[i + 2]);
            }
            i += 2;
        } else if (strcmp(argv[i], "--cheats") == 0 && i + 1 < argc) {
            cheats = (int)strtol(argv[++i], NULL, 16);
        } else if (strcmp(argv[i], "--boss-beat") == 0 && i + 1 < argc) {
            boss_beat = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--cheat-menu") == 0) {
            cheat_menu = 1;
        } else if (strcmp(argv[i], "--saves") == 0 && i + 1 < argc) {
            SavesFolder = argv[++i];
        } else if (strcmp(argv[i], "--frames") == 0 && i + 1 < argc) {
            StopAt = atoi(argv[++i]);
            testing = 1;
        } else if (strcmp(argv[i], "--shot") == 0 && i + 2 < argc && NumShots < MAX_SHOTS) {
            Shots[NumShots].frame = atoi(argv[++i]);
            Shots[NumShots++].path = argv[++i];
        } else if (strcmp(argv[i], "--pretend-pads") == 0 && i + 1 < argc) {
            host_pretend_pads(atoi(argv[++i]));
        } else if (strcmp(argv[i], "--pad-press") == 0 && i + 3 < argc && NumPadEvents < MAX_PRESSES) {
            PadEvents[NumPadEvents].kind = PAD_PRESS;
            PadEvents[NumPadEvents].pad = atoi(argv[++i]) - 1;
            PadEvents[NumPadEvents].frame = atoi(argv[++i]);
            PadEvents[NumPadEvents++].buttons = (int)strtol(argv[++i], NULL, 16);
            if (PadEvents[NumPadEvents - 1].pad < 0 || PadEvents[NumPadEvents - 1].pad > 3) {
                NumPadEvents--;
            }
        } else if ((strcmp(argv[i], "--pad-unplug") == 0 || strcmp(argv[i], "--pad-plug") == 0) && i + 2 < argc && NumPadEvents < MAX_PRESSES) {
            PadEvents[NumPadEvents].kind = strcmp(argv[i], "--pad-plug") == 0 ? PAD_PLUG : PAD_UNPLUG;
            PadEvents[NumPadEvents].pad = atoi(argv[++i]) - 1;
            PadEvents[NumPadEvents++].frame = atoi(argv[++i]);
            if (PadEvents[NumPadEvents - 1].pad < 0 || PadEvents[NumPadEvents - 1].pad > 3) {
                NumPadEvents--;
            }
        } else if (strcmp(argv[i], "--key-press") == 0 && i + 2 < argc && NumPadEvents < MAX_PRESSES) {
            PadEvents[NumPadEvents].kind = PAD_KEY;
            PadEvents[NumPadEvents].frame = atoi(argv[++i]);
            PadEvents[NumPadEvents].buttons = controls_key_from_id(argv[++i]);
            if (PadEvents[NumPadEvents].buttons > 0) {
                NumPadEvents++;
            } else {
                fprintf(stderr, "--key-press: no key called %s\n", argv[i]);
            }
        } else if (strcmp(argv[i], "--press") == 0 && i + 2 < argc && NumPresses < MAX_PRESSES) {
            Presses[NumPresses].frame = atoi(argv[++i]);
            Presses[NumPresses++].buttons = (int)strtol(argv[++i], NULL, 16);
        } else if (strcmp(argv[i], "--no-window") == 0) {
            NoWindow = 1;
            testing = 1;
        } else if (strcmp(argv[i], "--no-launcher") == 0) {
            no_launcher = 1;
        } else if (strcmp(argv[i], "--launcher") == 0) {
            launcher = 1;
        } else if (strcmp(argv[i], "--fps") == 0 && i + 1 < argc) {
            fps = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--antialias") == 0 && i + 1 < argc) {
            antialias = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--smooth") == 0) {
            smooth = 1;
        } else if (strcmp(argv[i], "--widescreen") == 0) {
            widescreen = 1;
        } else if (strcmp(argv[i], "--sharp-icons") == 0) {
            sharp_icons = 1;
        } else if (strcmp(argv[i], "--upscale") == 0 && i + 1 < argc) {
            upscale = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--texture-pack") == 0 && i + 1 < argc) {
            mods_add_pack(argv[++i]);
        } else if (strcmp(argv[i], "--mods-dir") == 0 && i + 1 < argc) {
            mods_only_in(argv[++i]);
        } else if (strcmp(argv[i], "--mod") == 0 && i + 1 < argc) {
            mods_turn_on(argv[++i]);
        } else if (strcmp(argv[i], "--dump-textures") == 0 && i + 1 < argc) {
            texpack_dump_to(argv[++i]);
        } else if (strcmp(argv[i], "--dump-files") == 0 && i + 1 < argc) {
            dump_files_to = argv[++i];
        } else if (strcmp(argv[i], "--log-files") == 0) {
            archive_log_reads();
        } else if (strcmp(argv[i], "--mode") == 0 && i + 1 < argc) {
            test_mode = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--level-flags") == 0 && i + 1 < argc) {
            level_flags = (int)strtol(argv[++i], NULL, 16);
        } else if (strcmp(argv[i], "--wav") == 0 && i + 1 < argc) {
            audio_record(argv[++i]);
        } else if (strcmp(argv[i], "--tile") == 0 && i + 2 < argc) {
            tile = atoi(argv[++i]);
            tiles = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--sound") == 0) {
            test_sound = 1;
        } else if (strcmp(argv[i], "--turbo") == 0) {
            Turbo = 1;
        } else if (strcmp(argv[i], "--depth-clip") == 0) {
            gfx_without_depth_clamp();
        } else if (strcmp(argv[i], "--draw-every") == 0 && i + 1 < argc) {
            draw_every = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--skip-intro") == 0) {
            skip_intro = 1;
        } else if (strcmp(argv[i], "--skip-loading") == 0) {
            skip_loading = 1;
        } else {
            fprintf(stderr, "unknown option %s\n", argv[i]);
            return 2;
        }
    }
#ifdef _WIN32
    SetUnhandledExceptionFilter(crash_report);
    if (StopAt >= 0) {
        CreateThread(NULL, 0, watchdog, OpenThread(THREAD_ALL_ACCESS, FALSE, GetCurrentThreadId()), 0, NULL);
    }
#endif
    /* a test run doesn't depend on what the player has chosen */
    if (testing) {
        settings_defaults();
    } else {
        settings_load();
    }
    settings()->launcher = launcher || (!testing && !no_launcher);
    settings()->no_save = testing;
    settings()->test_sound = test_sound;
    if (cheat_menu) {
        settings()->cheat_menu = 1;
    }
    settings()->boss_beat_at = boss_beat;
    settings()->test_bot = bot;
    settings()->test_aim = aim;
    settings()->test_adventure = test_adventure;
    settings()->test_challenge = test_challenge;
    settings()->test_stall = stall;
    if (testing) {
        setvbuf(stdout, NULL, _IOLBF, 0); /* so a run's log can be read while it runs */
    }
    settings()->test_cheats = cheats;
    if (window_w > 0 && window_h > 0) {
        settings()->window_w = window_w;
        settings()->window_h = window_h;
    }
    if (render_w >= 0 && render_h >= 0) {
        settings()->render_w = render_w;
        settings()->render_h = render_h;
    }
    if (SavesFolder != NULL) {
        saves_set_folder(SavesFolder);
    } else if (!testing) {
        saves_use_default_folder();
    }
    settings()->test_mode = test_mode;
    settings()->test_level_flags = level_flags;
    settings()->testing = test_mode >= 0;
    settings()->tile = tile;
    settings()->tiles = tiles;
    if (skip_intro) {
        settings()->skip_intro = 1;
    }
    if (skip_loading) {
        settings()->skip_loading = 1;
    }
    if (fps == 30 || fps == 60 || fps == 120) {
        settings()->fps = fps;
    }
    if (antialias == 1 || antialias == 2 || antialias == 4 || antialias == 8) {
        settings()->antialias = antialias;
    }
    if (smooth) {
        settings()->smooth = 1;
    }
    if (widescreen) {
        settings()->widescreen = 1;
    }
    if (upscale == 1 || upscale == 2 || upscale == 4) {
        settings()->upscale = upscale;
    }
    if (sharp_icons) {
        settings()->icons = 1;
    }
    if (extra_heroes) {
        settings()->extra_heroes = 1;
    }
    memcpy(settings()->test_extra, extra, sizeof(extra));
    /* the first time: the launcher's made of the game's own art, so this comes before it */
    if (Image != NULL) {
        const char *problem = disc_extract(Image, DataPath != NULL ? DataPath : own_data_path());

        if (problem != NULL) {
            platform_fatal(problem);
        }
    } else if (platform_data_path() == NULL || !SDL_GetPathInfo(platform_data_path(), NULL)) {
        const char *out = DataPath != NULL ? DataPath : own_data_path();
        const char *problem = NULL;

        if (!disc_find_beside(out, &problem) && !testing && !disc_first_run(out, problem)) {
            return 0;
        }
    }
    if (dump_files_to != NULL) {
        return dump_files(dump_files_to);
    }
    /* before the renderer, which makes room for the textures they have */
    mods_scan();
    mods_apply_files();
    if (!NoWindow && !host_init()) {
        fprintf(stderr, "No window; running without one.\n");
        NoWindow = 1;
    }
    gfx_draw_every(draw_every);
    overlays_init();
    machine_init();
    platform_finish(0);
    return 0;
}
