/* The PC port's entry point (docs/PORT.md).
 *
 *   crashbash [--data CRASHBSH.DAT] [--saves FOLDER] [--no-window] [--frames N]
 *
 * --frames stops after N vblanks. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "platform/platform.h"
#include "platform/gfx.h"
#include "platform/settings.h"
#include "platform/saves.h"

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

static const char *DataPath;
static const char *SavesFolder;
static int NoWindow;
static int Turbo;
static int StopAt = -1;

/* The first of these that exists, unless --data says */
const char *platform_data_path(void) {
    static const char *const places[] = {
        "CRASHBSH.DAT",
        "build/disc/files/CRASHBSH/CRASHBSH.DAT",
        "../disc/files/CRASHBSH/CRASHBSH.DAT",
        "../../disc/files/CRASHBSH/CRASHBSH.DAT",
    };
    size_t i;

    if (DataPath != NULL) {
        return DataPath;
    }
    for (i = 0; i < sizeof(places) / sizeof(places[0]); i++) {
        FILE *f = fopen(places[i], "rb");

        if (f != NULL) {
            fclose(f);
            return DataPath = places[i];
        }
    }
    return places[0];
}

void platform_finish(int code) {
    audio_finish();
    if (!NoWindow) {
        host_quit();
    }
    fflush(stdout);
    exit(code);
}

/* Once a vblank: the window and the clock */
#define MAX_SHOTS 64
static struct {
    int frame;
    const char *path;
} Shots[MAX_SHOTS];
static int NumShots;

/* --press: buttons held on pad 1 for a few vblanks */
#define MAX_PRESSES 256
#define PRESS_VBLANKS 6
static struct {
    int frame, buttons;
} Presses[MAX_PRESSES];
static int NumPresses;

void platform_vblank(void) {
    int i;

    Vblanks++;
    for (i = 0; i < NumShots; i++) {
        if (Shots[i].frame == Vblanks) {
            gfx_screenshot(Shots[i].path);
        }
    }
    if (!NoWindow) {
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

int platform_pad(int port) {
    int b = NoWindow ? (port == 0 ? 0 : -1) : host_pad(port);
    int i;

    if (port == 0) {
        for (i = 0; i < NumPresses; i++) {
            if (Vblanks >= Presses[i].frame && Vblanks < Presses[i].frame + PRESS_VBLANKS) {
                b = (b < 0 ? 0 : b) | Presses[i].buttons;
            }
        }
    }
    return b;
}

int main(int argc, char **argv) {
    int i;
    int window_w = 0, window_h = 0, render_w = -1, render_h = -1, cheat_menu = 0;
    int no_launcher = 0, launcher = 0, skip_intro = 0, skip_loading = 0, testing = 0, fps = 0, test_mode = -1, tile = 0, tiles = 0;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0) {
            printf("crashbash %s\n", BASH_VERSION);
            return 0;
        } else if (strcmp(argv[i], "--check-sdl") == 0) {
            printf("SDL %d\n", SDL_GetVersion());
            return 0;
        } else if (strcmp(argv[i], "--data") == 0 && i + 1 < argc) {
            DataPath = argv[++i];
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
        } else if (strcmp(argv[i], "--mode") == 0 && i + 1 < argc) {
            test_mode = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--wav") == 0 && i + 1 < argc) {
            audio_record(argv[++i]);
        } else if (strcmp(argv[i], "--tile") == 0 && i + 2 < argc) {
            tile = atoi(argv[++i]);
            tiles = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--turbo") == 0) {
            Turbo = 1;
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
    if (cheat_menu) {
        settings()->cheat_menu = 1;
    }
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
    if (!NoWindow && !host_init()) {
        fprintf(stderr, "No window; running without one.\n");
        NoWindow = 1;
    }
    overlays_init();
    machine_init();
    platform_finish(0);
    return 0;
}
