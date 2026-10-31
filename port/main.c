/* The PC port's entry point (docs/PORT.md).
 *
 *   crashbash [--data CRASHBSH.DAT] [--card FILE] [--no-window] [--frames N]
 *
 * --frames stops after N vblanks. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "platform/platform.h"
#include "platform/gfx.h"
#include "platform/launcher.h"
#include "platform/settings.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* A crash prints where, and the return addresses up the stack, for llvm-symbolizer. */
static LONG WINAPI crash_report(EXCEPTION_POINTERS *e) {
    uintptr_t image = (uintptr_t)GetModuleHandleA(NULL);
    CONTEXT c = *e->ContextRecord;
    int i;

    fprintf(stderr, "crash: code %08lX, reading %llX, image %llX\n", e->ExceptionRecord->ExceptionCode,
            (unsigned long long)e->ExceptionRecord->ExceptionInformation[1], (unsigned long long)image);
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
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

int machine_init(void);
void overlays_init(void);

static const char *DataPath;
static const char *CardPath = "memcard1.mcr";
static int NoWindow;
static int Turbo;
static int StopAt = -1;
static int Vblanks;

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

const char *platform_card_path(void) {
    return CardPath;
}

void platform_finish(int code) {
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

void platform_audio(const int16_t *stereo, int frames) {
    if (!NoWindow) {
        host_audio(stereo, frames);
    }
}

int main(int argc, char **argv) {
    int i;
    int no_launcher = 0, skip_intro = 0, skip_loading = 0, testing = 0, fps = 0, test_mode = -1;
    const char *launcher_shot = NULL, *launcher_keys = NULL;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0) {
            printf("crashbash %s\n", BASH_VERSION);
            return 0;
        } else if (strcmp(argv[i], "--check-sdl") == 0) {
            printf("SDL %d\n", SDL_GetVersion());
            return 0;
        } else if (strcmp(argv[i], "--data") == 0 && i + 1 < argc) {
            DataPath = argv[++i];
        } else if (strcmp(argv[i], "--card") == 0 && i + 1 < argc) {
            CardPath = argv[++i];
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
        } else if (strcmp(argv[i], "--launcher-shot") == 0 && i + 1 < argc) {
            launcher_shot = argv[++i];
        } else if (strcmp(argv[i], "--launcher-keys") == 0 && i + 1 < argc) {
            launcher_keys = argv[++i];
        } else if (strcmp(argv[i], "--fps") == 0 && i + 1 < argc) {
            fps = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--mode") == 0 && i + 1 < argc) {
            test_mode = atoi(argv[++i]);
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
#endif
    /* a test run doesn't depend on what the player has chosen */
    if (testing) {
        settings_defaults();
    } else {
        settings_load();
    }
    if (launcher_shot != NULL) {
        return launcher_screenshot(host_window(), launcher_shot) ? 0 : 1;
    }
    if (launcher_keys != NULL) {
        if (!launcher_run_keys(host_window(), launcher_keys)) {
            return 0;
        }
    } else if (!testing && !no_launcher && !launcher_run(host_window())) {
        return 0;
    }
    settings()->test_mode = test_mode;
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
