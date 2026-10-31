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

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* A crash prints where, and the return addresses up the stack, for llvm-symbolizer. */
static LONG WINAPI crash_report(EXCEPTION_POINTERS *e) {
    const uintptr_t *fp = (const uintptr_t *)e->ContextRecord->Rbp;
    uintptr_t image = (uintptr_t)GetModuleHandleA(NULL);
    int i;

    fprintf(stderr, "crash: code %08lX, reading %llX, image %llX\n", e->ExceptionRecord->ExceptionCode,
            (unsigned long long)e->ExceptionRecord->ExceptionInformation[1], (unsigned long long)image);
    fprintf(stderr, "stack: %llX", (unsigned long long)e->ContextRecord->Rip);
    for (i = 0; i < 32 && fp != NULL && !IsBadReadPtr(fp, 16); i++) {
        fprintf(stderr, " %llX", (unsigned long long)fp[1]);
        fp = (const uintptr_t *)fp[0];
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
void platform_vblank(void) {
    Vblanks++;
    if (!NoWindow) {
        if (!host_poll()) {
            platform_finish(0);
        }
        host_wait_vblank();
    }
    if (StopAt >= 0 && Vblanks >= StopAt) {
        printf("%d vblanks\n", Vblanks);
        platform_finish(0);
    }
}

int platform_pad(int port) {
    return NoWindow ? (port == 0 ? 0 : -1) : host_pad(port);
}

void platform_audio(const int16_t *stereo, int frames) {
    if (!NoWindow) {
        host_audio(stereo, frames);
    }
}

int main(int argc, char **argv) {
    int i;

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
        } else if (strcmp(argv[i], "--no-window") == 0) {
            NoWindow = 1;
        } else {
            fprintf(stderr, "unknown option %s\n", argv[i]);
            return 2;
        }
    }
#ifdef _WIN32
    SetUnhandledExceptionFilter(crash_report);
#endif
    if (!NoWindow && !host_init()) {
        fprintf(stderr, "No window; running without one.\n");
        NoWindow = 1;
    }
    overlays_init();
    machine_init();
    platform_finish(0);
    return 0;
}
