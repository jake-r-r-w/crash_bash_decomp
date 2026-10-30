/* The PC build's entry point. On the PS1 the BIOS starts the exe at __SN_ENTRY_POINT and the startup
 * code calls machine_init (src/main/nascalls.c); here the host's main does (docs/PORT.md).
 *
 *   crashbash [--data CRASHBSH.DAT] [--card FILE] [--no-host] [--no-pace] [--frames N]
 *             [--shot VBLANK:FILE.png]... [--press VBLANK:FRAMES:BUTTONS]... [--wav FILE.wav]
 *             [--playtest MODES [--playtest-shots DIR]]
 *
 * --frames stops after N vblanks. --shot saves what the screen shows at a vblank. --press holds
 * buttons (the PS1's bits, platform.h) from a vblank for that many vblanks. --playtest plays those
 * modes (0-32, say) to a win (playtest.c). They let a build be run and checked by a script, without
 * a window. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform/platform.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern unsigned char PcLowRam[];

/* A safety net for null pointers no one has marked with LOW_RAM yet (include/common.h). The PS1 has RAM
 * at address 0, so a null read or write there just carries on. This does the same: it points the
 * faulting instruction's base register at PcLowRam and runs it again, and says so once per place. */
static LONG WINAPI null_page(EXCEPTION_POINTERS *e) {
    CONTEXT *c = e->ContextRecord;
    const uint8_t *ip = (const uint8_t *)c->Eip;
    DWORD *regs[8];
    int rm, mod, base;

    /* only the game's own code, in the exe: DLLs and drivers may probe memory on purpose */
    if (e->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION ||
        e->ExceptionRecord->ExceptionInformation[1] >= 0x10000 || c->Eip >= 0x1000000) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    regs[0] = &c->Eax;
    regs[1] = &c->Ecx;
    regs[2] = &c->Edx;
    regs[3] = &c->Ebx;
    regs[4] = &c->Esp;
    regs[5] = &c->Ebp;
    regs[6] = &c->Esi;
    regs[7] = &c->Edi;
    while (*ip == 0x66 || *ip == 0x67 || *ip == 0xF2 || *ip == 0xF3 || *ip == 0x2E || *ip == 0x36 || *ip == 0x3E ||
           *ip == 0x26) {
        ip++;
    }
    if (*ip == 0xA4 || *ip == 0xA5) {
        base = c->Esi < 0x10000 ? 6 : 7;
    } else if (*ip == 0xAA || *ip == 0xAB) {
        base = 7;
    } else {
        ip += (*ip == 0x0F) ? 2 : 1;
        mod = *ip >> 6;
        rm = *ip & 7;
        if (mod == 3 || (rm == 5 && mod == 0)) {
            return EXCEPTION_CONTINUE_SEARCH;
        }
        base = rm == 4 ? (ip[1] & 7) : rm;
        if (rm == 4 && base == 5 && mod == 0) {
            return EXCEPTION_CONTINUE_SEARCH;
        }
    }
    if (*regs[base] >= 0x10000) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    {
        static DWORD seen[256];
        int i;

        for (i = 0; i < 256 && seen[i] != 0 && seen[i] != c->Eip; i++) {
        }
        if (i < 256 && seen[i] == 0) {
            seen[i] = c->Eip;
            fprintf(stderr, "null page: %08lX\n", (unsigned long)c->Eip);
        }
    }
    *regs[base] += (DWORD)(uintptr_t)PcLowRam;
    return EXCEPTION_CONTINUE_EXECUTION;
}

/* A crash prints where, and the return addresses up the stack, for llvm-symbolizer. */
static LONG WINAPI crash_report(EXCEPTION_POINTERS *e) {
    const uintptr_t *fp = (const uintptr_t *)e->ContextRecord->Ebp;
    int i;

    fprintf(stderr, "crash: code %08lX at %08lX, address %08lX\n", e->ExceptionRecord->ExceptionCode,
            (unsigned long)e->ContextRecord->Eip, (unsigned long)e->ExceptionRecord->ExceptionInformation[1]);
    fprintf(stderr, "stack: %08lX", (unsigned long)e->ContextRecord->Eip);
    for (i = 0; i < 24 && fp != NULL && !IsBadReadPtr(fp, 8); i++) {
        fprintf(stderr, " %08lX", (unsigned long)fp[1]);
        fp = (const uintptr_t *)fp[0];
    }
    fprintf(stderr, "\n");
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

extern unsigned char HeapStart[], ModeTextBuffer[];
int machine_init(void);
void overlays_init(void);
void playtest_start(const char *modes, const char *shot_dir);
void playtest_vblank(void);
int playtest_buttons(void);

#define MAX_LIST 256

static struct {
    int vblank;
    const char *path;
} Shots[MAX_LIST];
static int NumShots;

static struct {
    int start, count, buttons;
} Presses[MAX_LIST];
static int NumPresses;

static const char *DataPath;
static const char *CardPath = "memcard1.mcr";
static int NoHost, NoPace;
static int StopAt = -1;
static int Vblanks;
static FILE *Wav;
static uint32_t WavBytes;

static const uint32_t *LastPixels;
static int LastW, LastH;

/* The first of these that exists, unless --data says */
const char *platform_data_path(void) {
    static const char *const places[] = {
        "CRASHBSH.DAT",
        "build/disc/files/CRASHBSH/CRASHBSH.DAT",
        "disc/files/CRASHBSH/CRASHBSH.DAT",
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

/* A PNG with no compression: CRC-32, Adler-32 and stored deflate blocks */
static uint32_t crc_of(const uint8_t *p, size_t n, uint32_t crc) {
    static uint32_t tbl[256];
    size_t i;

    if (tbl[1] == 0) {
        uint32_t k;

        for (k = 0; k < 256; k++) {
            uint32_t c = k;
            int j;

            for (j = 0; j < 8; j++) {
                c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            }
            tbl[k] = c;
        }
    }
    for (i = 0; i < n; i++) {
        crc = tbl[(crc ^ p[i]) & 255] ^ (crc >> 8);
    }
    return crc;
}

static void put_be32(FILE *f, uint32_t v) {
    uint8_t b[4];

    b[0] = (uint8_t)(v >> 24);
    b[1] = (uint8_t)(v >> 16);
    b[2] = (uint8_t)(v >> 8);
    b[3] = (uint8_t)v;
    fwrite(b, 1, 4, f);
}

static void put_chunk(FILE *f, const char *type, const uint8_t *data, size_t n) {
    uint32_t crc;

    put_be32(f, (uint32_t)n);
    fwrite(type, 1, 4, f);
    if (n != 0) {
        fwrite(data, 1, n, f);
    }
    crc = crc_of((const uint8_t *)type, 4, 0xFFFFFFFFu);
    crc = crc_of(data, n, crc);
    put_be32(f, crc ^ 0xFFFFFFFFu);
}

static void save_png(const char *path, const uint32_t *pixels, int w, int h) {
    FILE *f = fopen(path, "wb");
    size_t row = (size_t)w * 3 + 1, raw_size = row * h;
    uint8_t *raw, *z, *q;
    uint8_t head[13];
    size_t left, pos;
    uint32_t a = 1, b = 0;
    int x, y;

    if (f == NULL) {
        fprintf(stderr, "Can't write %s\n", path);
        return;
    }
    raw = malloc(raw_size);
    for (y = 0; y < h; y++) {
        uint8_t *r = raw + row * y;

        *r++ = 0;
        for (x = 0; x < w; x++) {
            uint32_t c = pixels[y * w + x];

            *r++ = (uint8_t)(c >> 16);
            *r++ = (uint8_t)(c >> 8);
            *r++ = (uint8_t)c;
        }
    }
    z = malloc(raw_size + raw_size / 65535 * 5 + 16);
    q = z;
    *q++ = 0x78;
    *q++ = 1;
    for (pos = 0, left = raw_size; left > 0;) {
        size_t n = left > 65535 ? 65535 : left;

        *q++ = left == n;
        *q++ = (uint8_t)n;
        *q++ = (uint8_t)(n >> 8);
        *q++ = (uint8_t)~n;
        *q++ = (uint8_t)(~n >> 8);
        memcpy(q, raw + pos, n);
        q += n;
        pos += n;
        left -= n;
    }
    for (pos = 0; pos < raw_size; pos++) {
        a = (a + raw[pos]) % 65521;
        b = (b + a) % 65521;
    }
    *q++ = (uint8_t)(b >> 8);
    *q++ = (uint8_t)b;
    *q++ = (uint8_t)(a >> 8);
    *q++ = (uint8_t)a;
    fwrite("\x89PNG\r\n\x1A\n", 1, 8, f);
    memset(head, 0, sizeof(head));
    head[0] = (uint8_t)(w >> 24);
    head[1] = (uint8_t)(w >> 16);
    head[2] = (uint8_t)(w >> 8);
    head[3] = (uint8_t)w;
    head[4] = (uint8_t)(h >> 24);
    head[5] = (uint8_t)(h >> 16);
    head[6] = (uint8_t)(h >> 8);
    head[7] = (uint8_t)h;
    head[8] = 8;
    head[9] = 2;
    put_chunk(f, "IHDR", head, 13);
    put_chunk(f, "IDAT", z, (size_t)(q - z));
    put_chunk(f, "IEND", NULL, 0);
    fclose(f);
    free(raw);
    free(z);
}

void platform_present(const uint32_t *pixels, int w, int h) {
    LastPixels = pixels;
    LastW = w;
    LastH = h;
    if (!NoHost) {
        host_present(pixels, w, h);
    }
}

void platform_shot(const char *path) {
    if (LastPixels != NULL) {
        save_png(path, LastPixels, LastW, LastH);
    }
}

void platform_finish(int code) {
    if (Wav != NULL) {
        uint32_t v;

        fseek(Wav, 4, SEEK_SET);
        v = WavBytes + 36;
        fwrite(&v, 4, 1, Wav);
        fseek(Wav, 40, SEEK_SET);
        fwrite(&WavBytes, 4, 1, Wav);
        fclose(Wav);
    }
    if (!NoHost) {
        host_quit();
    }
    exit(code);
}

void platform_vblank(void) {
    int i;

    Vblanks++;
    for (i = 0; i < NumShots; i++) {
        if (Shots[i].vblank == Vblanks && LastPixels != NULL) {
            save_png(Shots[i].path, LastPixels, LastW, LastH);
        }
    }
    playtest_vblank();
    if (StopAt >= 0 && Vblanks >= StopAt) {
        platform_finish(0);
    }
    if (!NoHost) {
        if (!host_poll()) {
            platform_finish(0);
        }
        if (!NoPace) {
            host_wait_vblank();
        }
    }
}

int platform_pad(int port) {
    int b = NoHost ? (port == 0 ? 0 : -1) : host_pad(port);
    int i;

    if (port == 0) {
        if (b < 0) {
            b = 0;
        }
        b |= playtest_buttons();
        for (i = 0; i < NumPresses; i++) {
            if (Vblanks >= Presses[i].start && Vblanks < Presses[i].start + Presses[i].count) {
                b |= Presses[i].buttons;
            }
        }
    }
    return b;
}

void platform_audio(const int16_t *stereo, int frames) {
    if (!NoHost) {
        host_audio(stereo, frames);
    }
    if (Wav != NULL) {
        fwrite(stereo, 4, frames, Wav);
        WavBytes += (uint32_t)frames * 4;
    }
}

static void open_wav(const char *path) {
    static const uint8_t head[44] = {
        'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0, 1, 0, 2, 0,
        0x44, 0xAC, 0, 0, 0x10, 0xB1, 2, 0, 4, 0, 16, 0, 'd', 'a', 't', 'a', 0, 0, 0, 0,
    };

    Wav = fopen(path, "wb");
    if (Wav != NULL) {
        fwrite(head, 1, sizeof(head), Wav);
    }
}

int main(int argc, char **argv) {
    const char *playtest = NULL, *playtest_shots = NULL;
    int i;

#ifdef _WIN32
    AddVectoredExceptionHandler(1, null_page);
    SetUnhandledExceptionFilter(crash_report);
#endif
    for (i = 1; i < argc; i++) {
        const char *next = i + 1 < argc ? argv[i + 1] : NULL;

        if (strcmp(argv[i], "--no-host") == 0) {
            NoHost = 1;
        } else if (strcmp(argv[i], "--no-pace") == 0) {
            NoPace = 1;
        } else if (strcmp(argv[i], "--data") == 0 && next != NULL) {
            DataPath = argv[++i];
        } else if (strcmp(argv[i], "--card") == 0 && next != NULL) {
            CardPath = argv[++i];
        } else if (strcmp(argv[i], "--frames") == 0 && next != NULL) {
            StopAt = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--playtest") == 0 && next != NULL) {
            playtest = argv[++i];
        } else if (strcmp(argv[i], "--playtest-shots") == 0 && next != NULL) {
            playtest_shots = argv[++i];
        } else if (strcmp(argv[i], "--wav") == 0 && next != NULL) {
            open_wav(argv[++i]);
        } else if (strcmp(argv[i], "--shot") == 0 && next != NULL && NumShots < MAX_LIST) {
            const char *colon = strchr(next, ':');

            if (colon == NULL) {
                fprintf(stderr, "--shot VBLANK:FILE.png\n");
                return 1;
            }
            Shots[NumShots].vblank = atoi(next);
            Shots[NumShots].path = colon + 1;
            NumShots++;
            i++;
        } else if (strcmp(argv[i], "--press") == 0 && next != NULL && NumPresses < MAX_LIST) {
            int a, b, c;

            if (sscanf(next, "%i:%i:%i", &a, &b, &c) != 3) {
                fprintf(stderr, "--press VBLANK:FRAMES:BUTTONS\n");
                return 1;
            }
            Presses[NumPresses].start = a;
            Presses[NumPresses].count = b;
            Presses[NumPresses].buttons = c;
            NumPresses++;
            i++;
        } else {
            fprintf(stderr, "Unknown option %s (see port/main.c)\n", argv[i]);
            return 1;
        }
    }
    /* A primitive's link keeps 24 bits of its address, so the game's memory has to be below 16 MB. */
    if ((uintptr_t)HeapStart >= 0x1000000 || (uintptr_t)ModeTextBuffer >= 0x1000000) {
        fprintf(stderr, "The game's memory is above 16 MB (docs/PORT.md).\n");
        return 1;
    }
    if (!NoHost && !host_init()) {
        NoHost = 1;
    }
    overlays_init();
    if (playtest != NULL) {
        playtest_start(playtest, playtest_shots);
    }
    machine_init();
    platform_finish(0);
    return 0;
}
