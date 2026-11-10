/* A fuzzer for online play's packets (platform/net.c; docs/PORT.md, Online play, Hardening). Whatever arrives at a host or a
 * joiner, in the lobby or in the game, from one of the game's machines or from anyone else, mustn't make it read or write outside
 * what's meant, or stop answering for long. Built with libFuzzer, ASan and UBSan where the compiler has libFuzzer (CMakeLists.txt,
 * netfuzz: the Windows toolchain's has), and run by playtest.mjs fuzz.
 *
 * An input is a state for the machine to start in and how many machines the game has, then a run of things that happen to it: a
 * packet from someone, or a frame and a step going by. A packet is a byte for who sent it and how it's framed, its size, and its
 * bytes. Framed, the fuzzer writes the packets' magic number first, and the game's session (or a joiner's nonce) after the type, so
 * that the packets that get past those are most of what's tried; unframed, they're as they come.
 *
 * Seeds: with NETFUZZ_SEEDS set to a folder, it writes an input there for each state, made of the packets a real game sends in it. */
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "../platform/net.h"
#include "../platform/platform.h"
#include "../platform/portmap.h"
#include "../platform/settings.h"

/* the packets' magic and types, as net.c has them */
#define MAGIC 0x314E4243u
#define PROTOCOL 2
enum { T_JOIN = 1, T_WELCOME, T_PING, T_PONG, T_SAVE, T_START, T_READY, T_INPUT, T_RECORD, T_BYE, T_FIND, T_HERE };

static jmp_buf Back;
static int Armed;

/* --- what net.c asks of the rest of the program: as little as will do --- */

static Settings Fake;

Settings *settings(void) {
    return &Fake;
}

void settings_borrow(int skip_intro, int skip_loading, int cheat_menu, int extra_heroes) {
    Fake.skip_intro = skip_intro;
    Fake.skip_loading = skip_loading;
    Fake.cheat_menu = cheat_menu;
    Fake.extra_heroes = extra_heroes;
}

void settings_return(void) {
}

int platform_local_pad(int port) {
    return port == 0 ? 0 : -1;
}

int platform_local_pad_sticks(int port, unsigned char sticks[4]) {
    if (port != 0) {
        return 0;
    }
    memset(sticks, 0x80, 4);
    return 1;
}

int platform_local_pad_rumbles(int port) {
    return port == 0;
}

int platform_idle(void) {
    return 1;
}

const char *platform_data_path(void) {
    return NULL;
}

/* a game that's over goes back to the start of the next input, as the real one goes back to the start of the game */
void platform_online_stop(const char *why) {
    (void)why;
    if (Armed) {
        longjmp(Back, 1);
    }
}

void platform_finish(int code) {
    (void)code;
    if (Armed) {
        longjmp(Back, 1);
    }
}

void platform_fatal(const char *why) {
    (void)why;
    if (Armed) {
        longjmp(Back, 1);
    }
}

void portmap_open(int port, int first, int pretend) {
    (void)port;
    (void)first;
    (void)pretend;
}

int portmap_state(void) {
    return PORTMAP_OFF;
}

const char *portmap_address(void) {
    return "";
}

int portmap_port(void) {
    return 0;
}

void portmap_close(void) {
}

/* the game's check of a save (game/main/card.c), which the fuzzer doesn't link: here a save's whatever its first byte says, so
   both ways are tried */
int card_check(const void *data, int size, char *why, size_t room) {
    snprintf(why, room, "the fuzzer's say");
    return size == 0 || (size == 0x194 && ((const uint8_t *)data)[0] != 0xEE);
}

/* --- the inputs --- */

static void run(const uint8_t *data, size_t size) {
    static uint8_t packet[2048];
    size_t at = 2;

    net_fuzz_begin(data[0] % NET_FUZZ_STATES, 2 + data[1] % 3);
    while (at < size) {
        uint8_t op = data[at++];
        size_t length, n = 0;

        if (op >= 0xE0) {
            net_fuzz_tick(op & 1);
            continue;
        }
        if (at >= size) {
            break;
        }
        length = data[at++];
        if (length == 0xFF && at + 1 < size) {
            length = data[at] | (size_t)data[at + 1] << 8;
            at += 2;
        }
        length = length > size - at ? size - at : length;
        length = length > 1400 ? 1400 : length;
        /* framed: the magic first, and the session or the nonce after the type */
        if (op & 0x18) {
            packet[n++] = (uint8_t)MAGIC;
            packet[n++] = (uint8_t)(MAGIC >> 8);
            packet[n++] = (uint8_t)(MAGIC >> 16);
            packet[n++] = (uint8_t)(MAGIC >> 24);
        }
        if ((op & 0x18) >= 0x10 && length > 0) {
            uint32_t word = (op & 0x18) == 0x10 ? NET_FUZZ_SESSION : NET_FUZZ_NONCE;

            packet[n++] = data[at++];
            length--;
            packet[n++] = (uint8_t)word;
            packet[n++] = (uint8_t)(word >> 8);
            packet[n++] = (uint8_t)(word >> 16);
            packet[n++] = (uint8_t)(word >> 24);
        }
        memcpy(packet + n, data + at, length);
        at += length;
        net_fuzz_take(packet, n + length, op & 7);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    char text[48], where[64];
    size_t n = size < sizeof(text) - 1 ? size : sizeof(text) - 1;

    /* what's typed as a join code, too */
    memcpy(text, data, n);
    text[n] = '\0';
    net_code_read(text, where, sizeof(where));
    if (size < 2) {
        return 0;
    }
    Armed = 1;
    if (setjmp(Back) == 0) {
        run(data, size);
    }
    Armed = 0;
    net_lobby_reset();
    return 0;
}

/* --- seeds: the packets a real game sends, a state's worth at a time --- */

typedef struct {
    uint8_t bytes[4096];
    size_t n;
} Seed;

static void add(Seed *s, const void *bytes, size_t n) {
    if (s->n + n <= sizeof(s->bytes)) {
        memcpy(s->bytes + s->n, bytes, n);
        s->n += n;
    }
}

static void byte(Seed *s, uint32_t v) {
    uint8_t b = (uint8_t)v;

    add(s, &b, 1);
}

static void word(Seed *s, uint32_t v, int bytes) {
    while (bytes-- > 0) {
        byte(s, v);
        v >>= 8;
    }
}

/* A packet, from who, framed (0x08: the magic; 0x10: and the session after the type; 0x18: and the nonce): its body is in body. */
static void packet(Seed *s, int who, int framing, const Seed *body) {
    byte(s, (uint32_t)(who | framing));
    if (body->n >= 0xFF) {
        byte(s, 0xFF);
        word(s, (uint32_t)body->n, 2);
    } else {
        byte(s, (uint32_t)body->n);
    }
    add(s, body->bytes, body->n);
}

static void tick(Seed *s) {
    byte(s, 0xE0);
}

static void write_seed(const char *folder, const char *name, const Seed *s) {
    char path[1024];
    FILE *f;

    snprintf(path, sizeof(path), "%s/seed-%s", folder, name);
    if ((f = fopen(path, "wb")) != NULL) {
        fwrite(s->bytes, 1, s->n, f);
        fclose(f);
    }
}

static void write_seeds(const char *folder) {
    Seed s, b;
    int i, m;

    /* looking for games: one answers */
    memset(&s, 0, sizeof(s));
    byte(&s, NET_FUZZ_FINDING);
    byte(&s, 0);
    memset(&b, 0, sizeof(b));
    byte(&b, T_HERE);
    word(&b, PROTOCOL, 2);
    word(&b, net_fuzz_build(), 4);
    byte(&b, 2);
    word(&b, NET_FUZZ_SESSION, 4);
    packet(&s, 0, 0x08, &b);
    tick(&s);
    write_seed(folder, "finding", &s);

    /* hosting: someone asks who's hosting, two join, their pings come back, one leaves */
    memset(&s, 0, sizeof(s));
    byte(&s, NET_FUZZ_HOST_OPEN);
    byte(&s, 0);
    memset(&b, 0, sizeof(b));
    byte(&b, T_FIND);
    word(&b, PROTOCOL, 2);
    packet(&s, 4, 0x08, &b);
    for (m = 2; m <= 3; m++) {
        memset(&b, 0, sizeof(b));
        byte(&b, T_JOIN);
        word(&b, PROTOCOL, 2);
        word(&b, net_fuzz_build(), 4);
        word(&b, 0, 4);
        byte(&b, 1);
        word(&b, 0x1000u + (uint32_t)m, 4);
        packet(&s, m, 0x08, &b);
        tick(&s);
        memset(&b, 0, sizeof(b));
        byte(&b, T_PONG);
        byte(&b, (uint32_t)m);
        word(&b, SDL_GetTicks(), 4);
        packet(&s, m, 0x10, &b);
    }
    memset(&b, 0, sizeof(b));
    byte(&b, T_BYE);
    byte(&b, 3);
    byte(&b, 1);
    word(&b, 0, 4);
    packet(&s, 3, 0x10, &b);
    tick(&s);
    write_seed(folder, "host-open", &s);

    /* starting: each says it has the saves and the start */
    memset(&s, 0, sizeof(s));
    byte(&s, NET_FUZZ_HOST_STARTING);
    byte(&s, 1);
    tick(&s);
    for (m = 1; m <= 2; m++) {
        memset(&b, 0, sizeof(b));
        byte(&b, T_READY);
        byte(&b, (uint32_t)m);
        byte(&b, 0x1F);
        packet(&s, m, 0x10, &b);
    }
    tick(&s);
    write_seed(folder, "host-starting", &s);

    /* playing, as the host: each joiner's pads come, and steps are played */
    memset(&s, 0, sizeof(s));
    byte(&s, NET_FUZZ_HOST_PLAYING);
    byte(&s, 1);
    for (i = 0; i < 4; i++) {
        for (m = 1; m <= 2; m++) {
            int k;

            memset(&b, 0, sizeof(b));
            byte(&b, T_INPUT);
            byte(&b, (uint32_t)m);
            word(&b, (uint32_t)(i * 4), 4);
            word(&b, 0, 4);
            word(&b, 0, 4);
            word(&b, (uint32_t)(i * 4), 4);
            byte(&b, 4);
            for (k = 0; k < 4; k++) {
                byte(&b, 3);
                word(&b, 0x4000, 2);
                word(&b, 0x80808080u, 4);
            }
            packet(&s, m, 0x10, &b);
        }
        for (m = 0; m < 4; m++) {
            tick(&s);
        }
    }
    write_seed(folder, "host-playing", &s);

    /* joining: welcomed */
    memset(&s, 0, sizeof(s));
    byte(&s, NET_FUZZ_JOINER_ASKING);
    byte(&s, 0);
    tick(&s);
    memset(&b, 0, sizeof(b));
    byte(&b, T_WELCOME);
    byte(&b, 0);
    byte(&b, 1);
    word(&b, NET_FUZZ_SESSION, 4);
    packet(&s, 0, 0x18, &b);
    tick(&s);
    write_seed(folder, "joiner-asking", &s);

    /* in the lobby: pinged with who's there, sent the saves and the start, and on into the game */
    memset(&s, 0, sizeof(s));
    byte(&s, NET_FUZZ_JOINER_IN);
    byte(&s, 1);
    memset(&b, 0, sizeof(b));
    byte(&b, T_PING);
    word(&b, 1234, 4);
    byte(&b, 1);
    byte(&b, 3);
    byte(&b, 1);
    byte(&b, 1);
    byte(&b, 1);
    byte(&b, 2);
    packet(&s, 0, 0x10, &b);
    tick(&s);
    for (i = 0; i < 4; i++) {
        int k;

        memset(&b, 0, sizeof(b));
        byte(&b, T_SAVE);
        byte(&b, (uint32_t)i);
        word(&b, i == 2 ? 0x194 : 0, 2);
        for (k = 0; i == 2 && k < 0x194; k++) {
            byte(&b, (uint32_t)k);
        }
        packet(&s, 0, 0x10, &b);
    }
    memset(&b, 0, sizeof(b));
    byte(&b, T_START);
    byte(&b, 2);
    byte(&b, 3);
    byte(&b, 1);
    byte(&b, 3);
    for (m = 0; m < 3; m++) {
        byte(&b, (uint32_t)m);
        byte(&b, 1);
    }
    packet(&s, 0, 0x10, &b);
    tick(&s);
    tick(&s);
    write_seed(folder, "joiner-in", &s);

    /* playing, as a joiner: the host's records come, saying who's slow and who's gone */
    memset(&s, 0, sizeof(s));
    byte(&s, NET_FUZZ_JOINER_PLAYING);
    byte(&s, 1);
    for (i = 0; i < 4; i++) {
        int k;

        memset(&b, 0, sizeof(b));
        byte(&b, T_RECORD);
        word(&b, (uint32_t)(i * 4), 4);
        word(&b, (uint32_t)(i * 4 + 4), 4);
        word(&b, (uint32_t)(i == 1 ? 0 : -1), 4);
        word(&b, 0, 4);
        byte(&b, 0);
        byte(&b, i == 2 ? 0x24 : 0);
        byte(&b, i == 3 ? 0x04 : 0);
        word(&b, (uint32_t)(i * 4), 4);
        byte(&b, 4);
        for (k = 0; k < 4 * 4; k++) {
            byte(&b, 3);
            word(&b, 0x0040, 2);
            word(&b, 0x80808080u, 4);
        }
        packet(&s, 0, 0x10, &b);
        for (k = 0; k < 4; k++) {
            tick(&s);
        }
    }
    memset(&b, 0, sizeof(b));
    byte(&b, T_BYE);
    byte(&b, 0);
    byte(&b, 1);
    word(&b, 16, 4);
    packet(&s, 0, 0x10, &b);
    tick(&s);
    write_seed(folder, "joiner-playing", &s);
}

int LLVMFuzzerInitialize(int *argc, char ***argv) {
    const char *seeds = getenv("NETFUZZ_SEEDS");

    (void)argc;
    (void)argv;
    if (seeds != NULL && seeds[0] != '\0') {
        write_seeds(seeds);
    }
    return 0;
}
