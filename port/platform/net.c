/* Online play (docs/PORT.md, Online play): one machine hosts, the others join it by its address, and every machine runs the whole game
 * from everyone's presses. Nothing of the game travels but the pads.
 *
 * A step is one read of the pads by the game (pads_update), 30 a second. At each, a machine takes its own pads as they are and sends
 * them for the step DELAY steps on, so they've time to arrive. The host puts every machine's together into that step's record, the
 * four pads as the game will read them, and sends it to everyone; a machine plays a step once it has its record, and waits for it if
 * it must. So the host decides what each step's presses were, and nobody can disagree.
 *
 * Packets are UDP, and can be lost: each carries everything the other side hasn't said it has, so a lost one is made up by the next.
 * A machine only talks to the host (the host's is the one address anyone needs), which passes the presses on.
 *
 * A machine that stops answering is waited for a few seconds, and then the host plays on with its pads as pulled out, which the game
 * has always allowed for; if it comes back and catches up, its pads are plugged in again. A joiner with no word from the host for
 * longer than that stops. Once a second the machines compare the game's fingerprint (game/main/fingerprint.c), and stop if they
 * differ, as from then on they'd be playing different games.
 *
 * Every machine starts the game from the very beginning, with the host's saves and the host's settings where those change the game
 * (settings_borrow), so they start the same.
 *
 * For tests: packets can be made late and lost (--net-lag, --net-loss), a game's records kept (--net-record) and played again with
 * nobody else (--replay). */
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define closesocket close
#endif
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "net.h"
#include "platform.h"
#include "saves.h"
#include "settings.h"

#define MAGIC 0x314E4243u /* "CBN1" */
#define PROTOCOL 1
#define MACHINES 4
#define PADS 4
#define PAD_BYTES 7 /* what's there, the buttons, the sticks */
#define RECORD_BYTES (PADS * PAD_BYTES)
#define HISTORY 2048   /* steps kept to send again: over a minute */
#define PER_PACKET 32  /* steps in one packet */
#define PACKET_MOST 1200
#define STEP_MS 33.3667 /* two vblanks */
#define PINGS 6
#define PRINT_EVERY 30 /* steps between fingerprints compared */
#define PRINTS 64
#define RESEND_MS 12
#define HOST_GONE_MS 10000

enum { PAD_THERE = 1, PAD_STICKS = 2, PAD_RUMBLES = 4 };
enum { T_JOIN = 1, T_WELCOME, T_PING, T_PONG, T_SAVE, T_START, T_READY, T_INPUT, T_RECORD, T_BYE };
enum { BYE_LEFT = 1, BYE_DIFFERS, BYE_FULL, BYE_VERSION, BYE_DATA, BYE_BEHIND };
enum { OFFLINE, HOST, JOINER, REPLAY };
enum { READY_SAVES = 0x0F, READY_START = 0x10, READY_ALL = 0x1F };

typedef struct {
    int32_t step;
    uint32_t value;
} Print;

typedef struct {
    struct sockaddr_in addr;
    int first, pads; /* its players: pads of the game's four */
    int quiet;       /* not waited for: it's stopped answering */
    int left;        /* it's said goodbye, or been told to go */
    Uint64 heard;    /* when it last said anything */
    int32_t have;    /* its pads are here for every step before this */
    int32_t acked;   /* it has every step's record before this */
    int ready;       /* what it has of the start */
    uint32_t nonce;
    int rtt[PINGS], rtts;
    uint8_t last[PADS]; /* what each of its pads was (PAD_ bits) when it last sent any */
    Print prints[PRINTS]; /* its fingerprints, as it's sent them */
    uint8_t in[HISTORY][RECORD_BYTES];
} Machine;

static int Role;
static SOCKET Sock = INVALID_SOCKET;
static NetOptions Options;
static uint32_t Session;
static int Delay, NumMachines, Me, Want, Patience;
static Machine Machines[MACHINES];
static uint8_t Records[HISTORY][RECORD_BYTES];
static int32_t Have;     /* every step's record before this is here */
static int32_t Next;     /* the step the game reads next */
static int32_t HostNext; /* a joiner: the host's, as last heard */
static int32_t HostAcked; /* a joiner: the host has its pads for every step before this */
static uint8_t Now[RECORD_BYTES]; /* the step being played */
static Print Mine[PRINTS];
static int Welcomed, Refused, Ready, Ended;
static int HostLeft; /* a joiner: the host has said goodbye, and what it sent before that is all there is */
static int LeftOut;  /* a joiner: the host is playing on without waiting for this machine, till its pads arrive in time again */
static FILE *Recording, *Replaying;
/* how it went, for a test run to say */
static int Sent, Received, Lost, Waits, Quiets;
static Uint64 Waited, Longest;

static void say(const char *format, ...) {
    va_list args;

    va_start(args, format);
    printf("online: ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
    fflush(stdout);
}

/* --- packets: numbers low byte first, and nothing read past a packet's end --- */

typedef struct {
    const uint8_t *p, *end;
    int bad;
} Reader;

static uint32_t get(Reader *r, int bytes) {
    uint32_t v = 0;
    int i;

    if (r->end - r->p < bytes) {
        r->bad = 1;
        r->p = r->end;
        return 0;
    }
    for (i = 0; i < bytes; i++) {
        v |= (uint32_t)*r->p++ << (8 * i);
    }
    return v;
}

static uint8_t *put(uint8_t *p, uint32_t v, int bytes) {
    while (bytes-- > 0) {
        *p++ = (uint8_t)v;
        v >>= 8;
    }
    return p;
}

static uint8_t *begin(uint8_t *p, int type) {
    return put(put(p, MAGIC, 4), (uint32_t)type, 1);
}

static uint32_t fold(uint32_t h, const void *bytes, size_t size) {
    const uint8_t *p = bytes;

    while (size-- != 0) {
        h = (h ^ *p++) * 0x01000193u;
    }
    return h;
}

/* --- sending, with a test's pretend lag and loss --- */

#define LATE 512
static struct {
    Uint64 due;
    struct sockaddr_in to;
    int size;
    uint8_t bytes[PACKET_MOST];
} Late[LATE];
static int LateFirst, LateCount;
static uint32_t Random = 0x2545F491u;

static uint32_t random_next(void) {
    Random ^= Random << 13;
    Random ^= Random >> 17;
    Random ^= Random << 5;
    return Random;
}

static void send_now(const struct sockaddr_in *to, const uint8_t *bytes, int size) {
    sendto(Sock, (const char *)bytes, size, 0, (const struct sockaddr *)to, sizeof(*to));
    Sent++;
}

static void send_packet(const struct sockaddr_in *to, const uint8_t *bytes, const uint8_t *end) {
    int size = (int)(end - bytes);

    if (Options.loss > 0 && (int)(random_next() % 100) < Options.loss) {
        Lost++;
        return;
    }
    if (Options.lag > 0) {
        if (LateCount < LATE) {
            int i = (LateFirst + LateCount++) % LATE;

            Late[i].due = SDL_GetTicks() + (Uint64)Options.lag;
            Late[i].to = *to;
            Late[i].size = size;
            memcpy(Late[i].bytes, bytes, (size_t)size);
        } else {
            Lost++;
        }
        return;
    }
    send_now(to, bytes, size);
}

static void send_late(void) {
    Uint64 now = SDL_GetTicks();

    while (LateCount > 0 && Late[LateFirst].due <= now) {
        send_now(&Late[LateFirst].to, Late[LateFirst].bytes, Late[LateFirst].size);
        LateFirst = (LateFirst + 1) % LATE;
        LateCount--;
    }
}

static int same_address(const struct sockaddr_in *a, const struct sockaddr_in *b) {
    return a->sin_addr.s_addr == b->sin_addr.s_addr && a->sin_port == b->sin_port;
}

static const char *address_text(const struct sockaddr_in *a) {
    static char text[64];
    char ip[32] = "?";

    inet_ntop(AF_INET, &a->sin_addr, ip, sizeof(ip));
    SDL_snprintf(text, sizeof(text), "%s:%d", ip, ntohs(a->sin_port));
    return text;
}

/* Up to that long for a packet to come, so a wait doesn't spin. */
static void wait_packet(int ms) {
    struct timeval tv;
    fd_set set;

    if (LateCount > 0) {
        ms = 1;
    }
    tv.tv_sec = 0;
    tv.tv_usec = ms * 1000;
    FD_ZERO(&set);
    FD_SET(Sock, &set);
    select((int)Sock + 1, &set, NULL, NULL, &tv);
}

/* --- the end of a game that can't go on --- */

static void send_bye(const struct sockaddr_in *to, int reason, int32_t step) {
    uint8_t buf[32], *p = begin(buf, T_BYE);
    int i;

    p = put(p, Session, 4);
    p = put(p, (uint32_t)Me, 1);
    p = put(p, (uint32_t)reason, 1);
    p = put(p, (uint32_t)step, 4);
    for (i = 0; i < 3; i++) {
        send_packet(to, buf, p);
    }
}

static void stop(const char *why) {
    say("%s", why);
    net_end();
    platform_fatal(why);
}

static void differs(int32_t step, int machine) {
    static int said;
    char why[200];
    int m;

    if (said || Ended) {
        return;
    }
    said = 1;
    say("the games differ at step %d: this machine's and %s", step, Role == HOST ? "a joiner's" : "the host's");
    if (Role == HOST) {
        for (m = 1; m < NumMachines; m++) {
            if (!Machines[m].left) {
                send_bye(&Machines[m].addr, BYE_DIFFERS, step);
            }
        }
    } else {
        send_bye(&Machines[0].addr, BYE_DIFFERS, step);
    }
    SDL_snprintf(why, sizeof(why), "Online play: the players' games are no longer the same (from step %d, machine %d), so this game has stopped.", step,
                 machine + 1);
    stop(why);
}

/* --- fingerprints, compared as each side's arrive --- */

static void prints_clear(Print *prints) {
    int i;

    for (i = 0; i < PRINTS; i++) {
        prints[i].step = -1;
    }
}

static void prints_check(const Print *theirs, int32_t step, int machine) {
    const Print *a = &Mine[(step / PRINT_EVERY) % PRINTS], *b = &theirs[(step / PRINT_EVERY) % PRINTS];

    if (a->step == step && b->step == step && a->value != b->value) {
        differs(step, machine);
    }
}

static void prints_take(Print *theirs, int32_t step, uint32_t value, int machine) {
    if (step >= 0 && step % PRINT_EVERY == 0) {
        theirs[(step / PRINT_EVERY) % PRINTS].step = step;
        theirs[(step / PRINT_EVERY) % PRINTS].value = value;
        prints_check(theirs, step, machine);
    }
}

static const Print *prints_latest(void) {
    static const Print none = { -1, 0 };
    int32_t step = (Next - 1) / PRINT_EVERY * PRINT_EVERY;
    const Print *p = &Mine[(step / PRINT_EVERY) % PRINTS];

    return Next > 0 && p->step == step ? p : &none;
}

/* --- this machine's own pads --- */

static void sample(uint8_t *to, int pads, int neutral) {
    int p;

    memset(to, 0, RECORD_BYTES);
    for (p = 0; p < pads; p++) {
        uint8_t *b = to + p * PAD_BYTES;
        unsigned char sticks[4];
        int buttons = platform_local_pad(p);

        if (buttons < 0) {
            continue;
        }
        if (neutral) {
            buttons = 0;
        }
        b[0] = PAD_THERE;
        b[1] = (uint8_t)buttons;
        b[2] = (uint8_t)(buttons >> 8);
        if (platform_local_pad_sticks(p, sticks)) {
            b[0] |= PAD_STICKS;
            memcpy(b + 3, sticks, 4);
            if (neutral) {
                memset(b + 3, 0x80, 4);
            }
        }
        if (platform_local_pad_rumbles(p)) {
            b[0] |= PAD_RUMBLES;
        }
    }
}

/* How many players this machine brings: the keyboard's, and each controller after it. */
static int own_pads(void) {
    int n = 1;

    while (n < PADS && platform_local_pad(n) >= 0) {
        n++;
    }
    return n;
}

/* --- the host: every machine's pads into each step's record --- */

static void finalise(void) {
    for (;;) {
        uint8_t *record = Records[Have % HISTORY];
        int m, without = 0;

        for (m = 0; m < NumMachines; m++) {
            if (Machines[m].have <= Have) {
                if (!Machines[m].quiet) {
                    return;
                }
                without = 1;
            }
        }
        /* a step is only made without a quiet machine once the game needs it, in case that machine's back by then: its pads for
           this step are ones it took DELAY steps ago, so it need only be that close behind */
        if (without && Have > Next) {
            return;
        }
        memset(record, 0, RECORD_BYTES);
        for (m = 0; m < NumMachines; m++) {
            Machine *mc = &Machines[m];

            if (mc->have > Have) {
                int p;

                memcpy(record + mc->first * PAD_BYTES, mc->in[Have % HISTORY], (size_t)(mc->pads * PAD_BYTES));
                for (p = 0; p < mc->pads; p++) {
                    mc->last[p] = mc->in[Have % HISTORY][p * PAD_BYTES];
                }
                if (mc->quiet && !mc->left) {
                    mc->quiet = 0;
                    say("machine %d is back, at step %d", m + 1, Have);
                }
            } else {
                /* played on without: its pads are as they last were, with nothing held. Pulled out, the game would pause for a
                   controller to come back, and everyone would wait on a machine that may never */
                int p;

                for (p = 0; p < mc->pads; p++) {
                    uint8_t *b = record + (mc->first + p) * PAD_BYTES;

                    b[0] = mc->last[p];
                    if (b[0] & PAD_STICKS) {
                        memset(b + 3, 0x80, 4);
                    }
                }
                mc->have = Have + 1;
            }
        }
        Have++;
    }
}

static void send_records(int m) {
    Machine *mc = &Machines[m];
    uint8_t buf[PACKET_MOST], *p = begin(buf, T_RECORD);
    const Print *print = prints_latest();
    int count = Have - mc->acked, i;

    if (count > PER_PACKET) {
        count = PER_PACKET;
    }
    p = put(p, Session, 4);
    p = put(p, (uint32_t)mc->have, 4);
    p = put(p, (uint32_t)Next, 4);
    p = put(p, (uint32_t)print->step, 4);
    p = put(p, print->value, 4);
    p = put(p, (uint32_t)mc->quiet, 1);
    p = put(p, (uint32_t)mc->acked, 4);
    p = put(p, (uint32_t)count, 1);
    for (i = 0; i < count; i++) {
        memcpy(p, Records[(mc->acked + i) % HISTORY], RECORD_BYTES);
        p += RECORD_BYTES;
    }
    send_packet(&mc->addr, buf, p);
}

static void send_inputs(void) {
    Machine *me = &Machines[Me];
    uint8_t buf[PACKET_MOST], *p = begin(buf, T_INPUT);
    const Print *print = prints_latest();
    int count = me->have - HostAcked, i, size = me->pads * PAD_BYTES;

    if (count > PER_PACKET) {
        count = PER_PACKET;
    }
    p = put(p, Session, 4);
    p = put(p, (uint32_t)Me, 1);
    p = put(p, (uint32_t)Have, 4);
    p = put(p, (uint32_t)print->step, 4);
    p = put(p, print->value, 4);
    p = put(p, (uint32_t)HostAcked, 4);
    p = put(p, (uint32_t)count, 1);
    for (i = 0; i < count; i++) {
        memcpy(p, me->in[(HostAcked + i) % HISTORY], (size_t)size);
        p += size;
    }
    send_packet(&Machines[0].addr, buf, p);
}

/* What the others haven't said they have. */
static void send_news(void) {
    int m;

    if (Role == JOINER) {
        send_inputs();
    } else if (Role == HOST) {
        for (m = 1; m < NumMachines; m++) {
            if (!Machines[m].left) {
                send_records(m);
            }
        }
    }
}

/* --- what arrives --- */

static uint32_t build_hash(void) {
    return fold(0x811C9DC5u, BASH_VERSION, strlen(BASH_VERSION));
}

/* The game's data, by its size and its first part: every machine has to have the same game. */
static uint32_t data_hash(void) {
    static uint8_t part[256 * 1024];
    const char *path = platform_data_path();
    SDL_IOStream *io = path != NULL ? SDL_IOFromFile(path, "rb") : NULL;
    uint32_t h = 0x811C9DC5u;
    Sint64 size;
    size_t n;

    if (io == NULL) {
        return 0;
    }
    size = SDL_GetIOSize(io);
    n = SDL_ReadIO(io, part, sizeof(part));
    SDL_CloseIO(io);
    return fold(fold(h, &size, sizeof(size)), part, n);
}

static void send_welcome(const struct sockaddr_in *to, uint32_t nonce, int refused, int machine) {
    uint8_t buf[32], *p = begin(buf, T_WELCOME);

    p = put(p, nonce, 4);
    p = put(p, (uint32_t)refused, 1);
    p = put(p, (uint32_t)machine, 1);
    p = put(p, Session, 4);
    send_packet(to, buf, p);
}

static void host_takes(int type, Reader *r, const struct sockaddr_in *from) {
    Machine *mc;
    int m;

    if (type == T_JOIN) {
        uint32_t protocol = get(r, 2), build = get(r, 4), data = get(r, 4), pads = get(r, 1), nonce = get(r, 4);

        if (r->bad) {
            return;
        }
        for (m = 1; m < NumMachines; m++) {
            if (same_address(&Machines[m].addr, from) && Machines[m].nonce == nonce) {
                send_welcome(from, nonce, 0, m);
                return;
            }
        }
        if (protocol != PROTOCOL || build != build_hash()) {
            send_welcome(from, nonce, BYE_VERSION, 0);
        } else if (data != data_hash()) {
            send_welcome(from, nonce, BYE_DATA, 0);
        } else if (Role == HOST || NumMachines > Want || NumMachines >= MACHINES) {
            send_welcome(from, nonce, BYE_FULL, 0);
        } else {
            mc = &Machines[NumMachines];
            memset(mc, 0, sizeof(*mc));
            prints_clear(mc->prints);
            mc->addr = *from;
            mc->nonce = nonce;
            mc->pads = pads < 1 ? 1 : pads > PADS ? PADS : (int)pads;
            mc->heard = SDL_GetTicks();
            say("%s has joined, with %d controller%s: %d of %d here", address_text(from), mc->pads, mc->pads == 1 ? "" : "s", NumMachines + 1, Want + 1);
            send_welcome(from, nonce, 0, NumMachines++);
        }
        return;
    }
    if (get(r, 4) != Session) {
        return;
    }
    m = (int)get(r, 1);
    if (r->bad || m < 1 || m >= NumMachines || !same_address(&Machines[m].addr, from) || Machines[m].left) {
        return;
    }
    mc = &Machines[m];
    mc->heard = SDL_GetTicks();
    if (type == T_PONG) {
        uint32_t sent = get(r, 4);

        if (!r->bad && mc->rtts < PINGS) {
            mc->rtt[mc->rtts++] = (int)((uint32_t)SDL_GetTicks() - sent);
        }
    } else if (type == T_READY) {
        mc->ready |= (int)get(r, 1);
    } else if (type == T_BYE) {
        int reason = (int)get(r, 1);
        int32_t step = (int32_t)get(r, 4);

        if (reason == BYE_DIFFERS && Role == HOST) {
            differs(step, m);
        }
        mc->left = mc->quiet = 1;
        say("machine %d has left, at step %d", m + 1, Have);
    } else if (type == T_INPUT && Role == HOST) {
        int32_t acked = (int32_t)get(r, 4), print_step = (int32_t)get(r, 4);
        uint32_t print = get(r, 4);
        int32_t first = (int32_t)get(r, 4);
        int count = (int)get(r, 1), size = mc->pads * PAD_BYTES, i;

        if (r->bad || count > PER_PACKET || first < 0 || r->end - r->p < count * size) {
            return;
        }
        if (acked > mc->acked && acked <= Have) {
            mc->acked = acked;
        }
        for (i = 0; i < count; i++, r->p += size) {
            /* the next it's owed, and not so far on that it would lap what's kept */
            if (first + i == mc->have && mc->have < Have + HISTORY / 2) {
                memset(mc->in[mc->have % HISTORY], 0, RECORD_BYTES);
                memcpy(mc->in[mc->have % HISTORY], r->p, (size_t)size);
                mc->have++;
            }
        }
        prints_take(mc->prints, print_step, print, m);
    }
}

static void joiner_takes(int type, Reader *r, const struct sockaddr_in *from) {
    Machine *host = &Machines[0];

    if (!same_address(&host->addr, from)) {
        return;
    }
    if (type == T_WELCOME) {
        uint32_t nonce = get(r, 4), refused = get(r, 1), machine = get(r, 1), session = get(r, 4);

        if (r->bad || nonce != Machines[1].nonce || Welcomed) {
            return;
        }
        if (refused != 0) {
            Refused = (int)refused;
        } else if (machine >= 1 && machine < MACHINES) {
            Welcomed = 1;
            Me = (int)machine;
            Session = session;
        }
        host->heard = SDL_GetTicks();
        return;
    }
    if (!Welcomed || get(r, 4) != Session || r->bad) {
        return;
    }
    host->heard = SDL_GetTicks();
    if (type == T_PING) {
        uint8_t buf[32], *p = begin(buf, T_PONG);
        uint32_t sent = get(r, 4);

        p = put(p, Session, 4);
        p = put(p, (uint32_t)Me, 1);
        p = put(p, sent, 4);
        send_packet(&host->addr, buf, p);
    } else if (type == T_SAVE || type == T_START) {
        uint8_t buf[32], *p = begin(buf, T_READY);

        if (type == T_SAVE) {
            uint32_t slot = get(r, 1), size = get(r, 2);

            if (r->bad || slot >= SAVES_SLOTS || size > SAVES_MOST || (uint32_t)(r->end - r->p) < size) {
                return;
            }
            if (!(Ready & (1 << slot))) {
                saves_put((int)slot, r->p, (int)size);
                Ready |= 1 << slot;
            }
        } else if (!(Ready & READY_START)) {
            uint32_t delay = get(r, 1), machines = get(r, 1), me = get(r, 1), flags = get(r, 1);
            uint32_t m;

            if (r->bad || machines < 2 || machines > MACHINES || (int)me != Me || delay < 1 || delay > HISTORY / 8) {
                return;
            }
            for (m = 0; m < machines; m++) {
                uint32_t first = get(r, 1), pads = get(r, 1);

                if (r->bad || first + pads > PADS) {
                    return;
                }
                Machines[m].first = (int)first;
                Machines[m].pads = (int)pads;
            }
            Delay = (int)delay;
            NumMachines = (int)machines;
            settings_borrow(flags & 1, (flags >> 1) & 1, (flags >> 2) & 1, 0);
            Ready |= READY_START;
        }
        p = put(p, Session, 4);
        p = put(p, (uint32_t)Me, 1);
        p = put(p, (uint32_t)Ready, 1);
        send_packet(&host->addr, buf, p);
    } else if (type == T_RECORD && Role == JOINER) {
        int32_t acked = (int32_t)get(r, 4), host_next = (int32_t)get(r, 4), print_step = (int32_t)get(r, 4);
        uint32_t print = get(r, 4), quiet = get(r, 1);
        int32_t first = (int32_t)get(r, 4);
        int count = (int)get(r, 1), i;

        if (r->bad || count > PER_PACKET || first < 0 || r->end - r->p < count * RECORD_BYTES) {
            return;
        }
        if (host_next >= HostNext) {
            LeftOut = (int)quiet;
        }
        if (acked > HostAcked && acked <= Machines[Me].have) {
            HostAcked = acked;
        }
        if (host_next > HostNext) {
            HostNext = host_next;
        }
        for (i = 0; i < count; i++, r->p += RECORD_BYTES) {
            if (first + i == Have && Have < Next + HISTORY / 2) {
                memcpy(Records[Have % HISTORY], r->p, RECORD_BYTES);
                Have++;
            }
        }
        prints_take(host->prints, print_step, print, 0);
    } else if (type == T_BYE) {
        int reason;
        int32_t step;

        get(r, 1);
        reason = (int)get(r, 1);
        step = (int32_t)get(r, 4);
        if (reason == BYE_DIFFERS) {
            differs(step, 0);
        }
        if (reason == BYE_BEHIND) {
            stop("Online play: this machine fell too far behind the others, and the game has gone on without it.");
        }
        /* the steps it sent before it left are still played */
        HostLeft = 1;
    }
}

static void pump(void) {
    send_late();
    for (;;) {
        uint8_t buf[PACKET_MOST + 64];
        struct sockaddr_in from;
        socklen_t from_size = sizeof(from);
        int size = (int)recvfrom(Sock, (char *)buf, sizeof(buf), 0, (struct sockaddr *)&from, &from_size);
        Reader r;
        int type;

        if (size <= 0) {
            return;
        }
        Received++;
        r.p = buf;
        r.end = buf + size;
        r.bad = 0;
        if (size > PACKET_MOST || get(&r, 4) != MAGIC) {
            continue;
        }
        type = (int)get(&r, 1);
        if (Me == 0) {
            host_takes(type, &r, &from);
        } else {
            joiner_takes(type, &r, &from);
        }
    }
}

/* --- starting --- */

static int open_socket(const char *address, int port, int bind_it, struct sockaddr_in *out) {
    struct addrinfo hints, *found = NULL;
    char service[16];
#ifdef _WIN32
    static int started;
    u_long on = 1;
    DWORD off = 0, got = 0;
    WSADATA data;

    if (!started && WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        return 0;
    }
    started = 1;
#endif
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = bind_it && address == NULL ? AI_PASSIVE : 0;
    SDL_snprintf(service, sizeof(service), "%d", port);
    if (getaddrinfo(address, service, &hints, &found) != 0 || found == NULL) {
        return 0;
    }
    memcpy(out, found->ai_addr, sizeof(*out));
    freeaddrinfo(found);
    Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (Sock == INVALID_SOCKET) {
        return 0;
    }
#ifdef _WIN32
    ioctlsocket(Sock, FIONBIO, &on);
    /* without this, a packet sent to a machine that's gone makes the next read fail */
    WSAIoctl(Sock, _WSAIOW(IOC_VENDOR, 12), &off, sizeof(off), NULL, 0, &got, NULL, NULL);
#else
    fcntl(Sock, F_SETFL, fcntl(Sock, F_GETFL, 0) | O_NONBLOCK);
#endif
    return !bind_it || bind(Sock, (const struct sockaddr *)out, sizeof(*out)) == 0;
}

/* "ADDRESS:PORT", or just "PORT": 0 if it's neither. */
static int split_address(const char *text, char *address, size_t room, int *port) {
    const char *colon = strrchr(text, ':');

    address[0] = '\0';
    if (colon != NULL) {
        if ((size_t)(colon - text) >= room) {
            return 0;
        }
        memcpy(address, text, (size_t)(colon - text));
        address[colon - text] = '\0';
        text = colon + 1;
    }
    *port = atoi(text);
    return *port > 0 && *port < 65536;
}

/* The saves this game starts with, as one number: every machine's says the same, if the host's arrived whole. */
static uint32_t saves_hash(void) {
    uint8_t save[SAVES_MOST];
    uint32_t h = 0x811C9DC5u;
    int slot, size;

    for (slot = 0; slot < SAVES_SLOTS; slot++) {
        size = saves_get(slot, save, sizeof(save));
        h = fold(fold(h, &size, sizeof(size)), save, (size_t)size);
    }
    return h;
}

static int median(int *v, int n) {
    int i, j;

    for (i = 1; i < n; i++) {
        for (j = i; j > 0 && v[j - 1] > v[j]; j--) {
            int t = v[j];

            v[j] = v[j - 1];
            v[j - 1] = t;
        }
    }
    return v[n / 2];
}

/* The first DELAY steps have nobody's presses: every pad there, with nothing held. */
static void first_steps(void) {
    Machine *me = &Machines[Me];

    for (me->have = 0; me->have < Delay; me->have++) {
        sample(me->in[me->have], me->pads, 1);
    }
}

static void record_header(void) {
    const Settings *s = settings();
    uint8_t save[SAVES_MOST];
    int slot, size, i;

    fprintf(Recording, "crashbash online %d\ndelay %d\nsettings %d %d %d\n", PROTOCOL, Delay, s->skip_intro, s->skip_loading, s->cheat_menu);
    for (slot = 0; slot < SAVES_SLOTS; slot++) {
        size = saves_get(slot, save, sizeof(save));
        if (size > 0) {
            fprintf(Recording, "save %d ", slot);
            for (i = 0; i < size; i++) {
                fprintf(Recording, "%02X", save[i]);
            }
            fprintf(Recording, "\n");
        }
    }
}

static int start_host(void) {
    const Settings *s = settings();
    char address[128];
    struct sockaddr_in here;
    Uint64 pinged = 0, sent = 0, began;
    int port, m, worst = 0, next_pad = 0;

    if (!split_address(Options.host, address, sizeof(address), &port) || !open_socket(address[0] != '\0' ? address : NULL, port, 1, &here)) {
        say("can't host on %s: the port may be in use", Options.host);
        return 0;
    }
    Me = 0;
    NumMachines = 1;
    Want = Options.joiners < 1 ? 1 : Options.joiners > MACHINES - 1 ? MACHINES - 1 : Options.joiners;
    Session = (uint32_t)SDL_GetPerformanceCounter() * 2654435761u + (uint32_t)SDL_GetTicksNS();
    Random ^= Session;
    say("hosting on port %d, waiting for %d to join", port, Want);
    /* everyone here, and how long a packet takes to each and back */
    for (;;) {
        Uint64 now = SDL_GetTicks();
        int measured = NumMachines == Want + 1;

        pump();
        for (m = 1; m < NumMachines; m++) {
            measured &= Machines[m].rtts >= PINGS;
        }
        if (measured) {
            break;
        }
        if (now - pinged >= 30) {
            pinged = now;
            for (m = 1; m < NumMachines; m++) {
                if (Machines[m].rtts < PINGS) {
                    uint8_t buf[32], *p = begin(buf, T_PING);

                    p = put(p, Session, 4);
                    p = put(p, (uint32_t)now, 4);
                    send_packet(&Machines[m].addr, buf, p);
                }
            }
        }
        if (!platform_idle()) {
            return -1;
        }
        wait_packet(2);
    }
    for (m = 1; m < NumMachines; m++) {
        int rtt = median(Machines[m].rtt, Machines[m].rtts);

        say("machine %d: %d ms there and back", m + 1, rtt);
        worst = rtt > worst ? rtt : worst;
    }
    /* long enough for a press to reach the host and its record to come back, with a little over, and the wait for a vblank's look at
       what's come */
    Delay = Options.delay > 0 ? Options.delay : (int)((worst * 1.2 + 25.0) / STEP_MS) + 1;
    Delay = Delay > 30 ? 30 : Delay;
    /* the host's players first, then each machine's as it joined */
    Machines[0].pads = own_pads();
    prints_clear(Machines[0].prints);
    for (m = 0; m < NumMachines; m++) {
        Machines[m].first = next_pad;
        Machines[m].pads = Machines[m].pads > PADS - next_pad ? PADS - next_pad : Machines[m].pads;
        next_pad += Machines[m].pads;
    }
    /* each gets the host's saves and the start, sent till it says it has them */
    began = SDL_GetTicks();
    for (;;) {
        Uint64 now = SDL_GetTicks();
        int ready = 1;

        pump();
        for (m = 1; m < NumMachines; m++) {
            ready &= Machines[m].ready == READY_ALL;
        }
        if (ready) {
            break;
        }
        if (now - began > 15000) {
            say("the others didn't take the start");
            return 0;
        }
        if (now - sent >= 60) {
            sent = now;
            for (m = 1; m < NumMachines; m++) {
                Machine *mc = &Machines[m];
                uint8_t buf[PACKET_MOST], *p;
                int slot, k;

                for (slot = 0; slot < SAVES_SLOTS; slot++) {
                    if (!(mc->ready & (1 << slot))) {
                        uint8_t save[SAVES_MOST];
                        int size = saves_get(slot, save, sizeof(save));

                        p = begin(buf, T_SAVE);
                        p = put(p, Session, 4);
                        p = put(p, (uint32_t)slot, 1);
                        p = put(p, (uint32_t)size, 2);
                        memcpy(p, save, (size_t)size);
                        send_packet(&mc->addr, buf, p + size);
                    }
                }
                if ((mc->ready & READY_SAVES) == READY_SAVES) {
                    p = begin(buf, T_START);
                    p = put(p, Session, 4);
                    p = put(p, (uint32_t)Delay, 1);
                    p = put(p, (uint32_t)NumMachines, 1);
                    p = put(p, (uint32_t)m, 1);
                    p = put(p, (uint32_t)(s->skip_intro | s->skip_loading << 1 | s->cheat_menu << 2), 1);
                    for (k = 0; k < NumMachines; k++) {
                        p = put(p, (uint32_t)Machines[k].first, 1);
                        p = put(p, (uint32_t)Machines[k].pads, 1);
                    }
                    send_packet(&mc->addr, buf, p);
                }
            }
        }
        if (!platform_idle()) {
            return -1;
        }
        wait_packet(2);
    }
    /* extra heroes come from each player's own disc, and there's no telling yet that everyone has them */
    settings_borrow(s->skip_intro, s->skip_loading, s->cheat_menu, 0);
    first_steps();
    Role = HOST;
    for (m = 0; m < NumMachines; m++) {
        say("machine %d has player%s %d to %d", m + 1, Machines[m].pads == 1 ? "" : "s", Machines[m].first + 1, Machines[m].first + Machines[m].pads);
    }
    say("starting: each press is used %d step%s late (%d ms); saves %08X", Delay, Delay == 1 ? "" : "s", (int)(Delay * STEP_MS), saves_hash());
    return 1;
}

static int start_join(void) {
    char address[128];
    struct sockaddr_in host;
    uint8_t buf[64], *p;
    Uint64 asked = 0, began = SDL_GetTicks();
    int port, pads;

    if (!split_address(Options.join, address, sizeof(address), &port) || address[0] == '\0' || !open_socket(address, port, 0, &host)) {
        say("can't find %s: it's the host's address and port, as 192.168.1.20:47000", Options.join);
        return 0;
    }
    Me = 1; /* till the host says which */
    memset(Machines, 0, sizeof(Machines));
    Machines[0].addr = host;
    Machines[0].heard = began;
    prints_clear(Machines[0].prints);
    Random ^= (uint32_t)SDL_GetPerformanceCounter() * 2654435761u;
    Machines[1].nonce = random_next();
    /* a moment for the controllers to be found */
    platform_idle();
    platform_idle();
    pads = own_pads();
    saves_in_memory();
    say("joining %s", address_text(&host));
    while (Ready != READY_ALL) {
        Uint64 now;

        pump();
        now = SDL_GetTicks();
        if (Refused != 0) {
            say(Refused == BYE_VERSION ? "the host has another version of the game: everyone needs the same one"
                : Refused == BYE_DATA ? "the host's game data isn't the same as this machine's" : "the host's game is full, or has started");
            return 0;
        }
        if (!Welcomed && now - asked >= 250) {
            asked = now;
            p = begin(buf, T_JOIN);
            p = put(p, PROTOCOL, 2);
            p = put(p, build_hash(), 4);
            p = put(p, data_hash(), 4);
            p = put(p, (uint32_t)pads, 1);
            p = put(p, Machines[1].nonce, 4);
            send_packet(&host, buf, p);
        }
        if (now - (Welcomed ? Machines[0].heard : began) > (Welcomed ? 15000u : 30000u)) {
            say(Welcomed ? "the host has gone" : "nothing answered at %s", address_text(&host));
            return 0;
        }
        if (!platform_idle()) {
            if (Welcomed) {
                send_bye(&host, BYE_LEFT, 0);
                send_late();
            }
            return -1;
        }
        wait_packet(2);
    }
    first_steps();
    Role = JOINER;
    say("joined as machine %d, with player%s %d to %d of %d machines: each press is used %d step%s late; saves %08X", Me + 1,
        Machines[Me].pads == 1 ? "" : "s", Machines[Me].first + 1, Machines[Me].first + Machines[Me].pads, NumMachines, Delay, Delay == 1 ? "" : "s", saves_hash());
    return 1;
}

static int hex_bytes(const char *text, uint8_t *out, int most) {
    int n = 0;
    unsigned v;

    while (n < most && sscanf(text + n * 2, "%2x", &v) == 1 && text[n * 2 + 1] > ' ') {
        out[n++] = (uint8_t)v;
    }
    return n;
}

static int start_replay(void) {
    static char line[SAVES_MOST * 2 + 64];
    int a, b, c;

    Replaying = fopen(Options.replay, "r");
    if (Replaying == NULL || fgets(line, sizeof(line), Replaying) == NULL || sscanf(line, "crashbash online %d", &a) != 1 || a != PROTOCOL) {
        say("%s isn't a game's record", Options.replay);
        return 0;
    }
    saves_in_memory();
    for (;;) {
        long at = ftell(Replaying);

        if (fgets(line, sizeof(line), Replaying) == NULL) {
            break;
        }
        if (sscanf(line, "delay %d", &a) == 1) {
            Delay = a;
        } else if (sscanf(line, "settings %d %d %d", &a, &b, &c) == 3) {
            settings_borrow(a, b, c, 0);
        } else if (sscanf(line, "save %d", &a) == 1 && strchr(line + 5, ' ') != NULL) {
            uint8_t save[SAVES_MOST];

            saves_put(a, save, hex_bytes(strchr(line + 5, ' ') + 1, save, sizeof(save)));
        } else {
            fseek(Replaying, at, SEEK_SET);
            break;
        }
    }
    Role = REPLAY;
    say("playing %s again", Options.replay);
    return 1;
}

int net_start(const NetOptions *options) {
    int ok;

    Options = *options;
    Patience = options->patience > 0 ? options->patience : 5000;
    prints_clear(Mine);
    ok = options->replay != NULL ? start_replay() : options->host != NULL ? start_host() : start_join();
    if (ok > 0 && options->record != NULL && (Recording = fopen(options->record, "w")) != NULL) {
        record_header();
    }
    return ok;
}

int net_on(void) {
    return Role != OFFLINE;
}

/* --- playing --- */

void net_step(void) {
    Machine *me = &Machines[Me];
    Uint64 began, sent;
    int32_t before;

    if (Role == OFFLINE || Ended) {
        return;
    }
    if (Role == REPLAY) {
        char line[128];
        int step;

        if (fgets(line, sizeof(line), Replaying) == NULL || sscanf(line, "%d", &step) != 1 || step != Next || strchr(line, ' ') == NULL
            || hex_bytes(strchr(line, ' ') + 1, Now, RECORD_BYTES) != RECORD_BYTES) {
            say("the record ends at step %d", Next);
            platform_finish(0);
        }
        Next++;
        return;
    }
    if (Next == Options.vanish_at) {
        say("vanishing, at step %d", Next);
        _Exit(0);
    }
    if (Next == Options.stall_at) {
        say("stalling for %d ms, at step %d", Options.stall_ms, Next);
        SDL_Delay((Uint32)Options.stall_ms);
    }
    /* this machine's pads as they are now, for the step DELAY on */
    sample(me->in[me->have % HISTORY], me->pads, 0);
    me->have++;
    began = sent = SDL_GetTicks();
    pump();
    if (Role == HOST) {
        finalise();
    }
    send_news();
    while (Have <= Next) {
        Uint64 now;
        int m;

        before = Have;
        pump();
        now = SDL_GetTicks();
        if (Role == HOST) {
            finalise();
            if (Have <= Next && now - began > (Uint64)Patience) {
                /* whoever this step waits for has had long enough: on without them */
                for (m = 1; m < NumMachines; m++) {
                    if (Machines[m].have <= Have && !Machines[m].quiet) {
                        Machines[m].quiet = 1;
                        Quiets++;
                        say("machine %d isn't answering: playing on without its player%s, from step %d", m + 1, Machines[m].pads == 1 ? "" : "s", Have);
                    }
                }
                finalise();
            }
        } else if (HostLeft) {
            stop("Online play: the host has left the game.");
        } else if (now - Machines[0].heard > HOST_GONE_MS) {
            stop("Online play: the host has stopped answering.");
        }
        if (Have != before || now - sent >= RESEND_MS) {
            send_news();
            sent = now;
        }
        if (Have > Next) {
            break;
        }
        if (!platform_idle()) {
            platform_finish(0);
        }
        wait_packet(1);
    }
    began = SDL_GetTicks() - began;
    if (began > 0) {
        Waits++;
        Waited += began;
        Longest = began > Longest ? began : Longest;
    }
    memcpy(Now, Records[Next % HISTORY], RECORD_BYTES);
    if (Recording != NULL) {
        int i;

        fprintf(Recording, "%d ", Next);
        for (i = 0; i < RECORD_BYTES; i++) {
            fprintf(Recording, "%02X", Now[i]);
        }
        fprintf(Recording, "\n");
    }
    Next++;
    if (Role == HOST) {
        /* a machine a long way behind can't be sent what it's missed any more */
        int m;

        for (m = 1; m < NumMachines; m++) {
            if (!Machines[m].left && Have - Machines[m].acked > HISTORY - 4 * PER_PACKET) {
                Machines[m].left = Machines[m].quiet = 1;
                send_bye(&Machines[m].addr, BYE_BEHIND, Have);
                say("machine %d is too far behind to catch up: it's out of the game", m + 1);
            }
        }
    }
}

int net_pad(int pad) {
    const uint8_t *b = Now + pad * PAD_BYTES;

    return pad < 0 || pad >= PADS || !(b[0] & PAD_THERE) ? -1 : b[1] | b[2] << 8;
}

int net_pad_sticks(int pad, unsigned char sticks[4]) {
    const uint8_t *b = Now + pad * PAD_BYTES;

    if (pad < 0 || pad >= PADS || !(b[0] & PAD_STICKS)) {
        return 0;
    }
    memcpy(sticks, b + 3, 4);
    return 1;
}

int net_pad_rumbles(int pad) {
    return pad >= 0 && pad < PADS && (Now[pad * PAD_BYTES] & PAD_RUMBLES) != 0;
}

int net_own_pad(int pad) {
    const Machine *me = &Machines[Me];

    if (Role == REPLAY) {
        return -1;
    }
    return pad >= me->first && pad < me->first + me->pads ? pad - me->first : -1;
}

void net_step_done(uint32_t fingerprint) {
    int32_t step = Next - 1;
    int m;

    if ((Role != HOST && Role != JOINER) || Ended || step < 0 || step % PRINT_EVERY != 0) {
        return;
    }
    if (Options.differ_at >= 0 && step >= Options.differ_at) {
        fingerprint ^= 1;
    }
    Mine[(step / PRINT_EVERY) % PRINTS].step = step;
    Mine[(step / PRINT_EVERY) % PRINTS].value = fingerprint;
    for (m = 0; m < NumMachines; m++) {
        if (m != Me && !Machines[m].left && (Role == HOST || m == 0)) {
            prints_check(Machines[m].prints, step, m);
        }
    }
}

void net_vblank(void) {
    int32_t before = Have;

    if ((Role != HOST && Role != JOINER) || Ended) {
        return;
    }
    pump();
    if (Role == HOST) {
        finalise();
        if (Have != before) {
            send_news();
        }
    }
}

/* A whole step behind the host, a joiner's pads arrive that much later, and the host waits on them. And one the host has stopped
   waiting for only gets back in by playing each step the moment it comes, so that its pads for DELAY steps on are there in time. */
int net_behind(void) {
    return Role == JOINER && (HostNext - Next >= 1 || (LeftOut && Have > Next));
}

void net_end(void) {
    Uint64 began = SDL_GetTicks();
    int m, owed;

    if (Ended || Role == OFFLINE) {
        return;
    }
    Ended = 1;
    if (Role == HOST) {
        /* the others get every step this machine played, so they can end where it did */
        do {
            pump();
            owed = 0;
            for (m = 1; m < NumMachines; m++) {
                if (!Machines[m].left && !Machines[m].quiet && Machines[m].acked < Next) {
                    owed = 1;
                    send_records(m);
                }
            }
            if (owed) {
                wait_packet(5);
            }
        } while (owed && SDL_GetTicks() - began < 1500);
        for (m = 1; m < NumMachines; m++) {
            if (!Machines[m].left) {
                send_bye(&Machines[m].addr, BYE_LEFT, Next);
            }
        }
    } else if (Role == JOINER) {
        send_bye(&Machines[0].addr, BYE_LEFT, Next);
    }
    /* what a test's pretend lag is still holding */
    while (LateCount > 0 && SDL_GetTicks() - began < 3000) {
        send_late();
        SDL_Delay(1);
    }
    if (Role != REPLAY) {
        say("%d steps; waited for the others %d times, %d ms in all, %d at the longest; %d packets sent, %d received, %d lost on purpose; %d left behind",
            Next, Waits, (int)Waited, (int)Longest, Sent, Received, Lost, Quiets);
    }
    if (Recording != NULL) {
        fclose(Recording);
        Recording = NULL;
    }
    if (Sock != INVALID_SOCKET) {
        closesocket(Sock);
        Sock = INVALID_SOCKET;
    }
}
