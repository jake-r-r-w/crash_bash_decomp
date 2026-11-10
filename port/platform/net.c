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
 * nobody else (--replay).
 *
 * Whatever arrives may be anything at all, from anyone: nothing is read past a packet's end (Reader), every count and index in one is
 * checked before it's used, and a packet from an address that isn't one of the game's is only ever a question to answer. A machine in
 * the game can still spoil it, as it chooses its own presses; it can't make another machine read or write outside what's meant, or
 * stop it answering for long. tests/netfuzz.c throws packets at every state a machine can be in, to keep that so (NET_FUZZ). */
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
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
#include "portmap.h"
#include "saves.h"
#include "settings.h"

#define MAGIC 0x314E4243u /* "CBN1" */
#define PROTOCOL 2
#define MACHINES 4
#define PADS 4
#define PAD_BYTES 7 /* what's there, the buttons, the sticks */
#define RECORD_BYTES (PADS * PAD_BYTES)
#define HISTORY 2048   /* steps kept to send again: over a minute */
#define PER_PACKET 32  /* steps in one packet */
#define PACKET_MOST 1200
#define STEP_MS 33.3667 /* two vblanks */
#define PINGS 6
#define TIMED 24 /* pings timed as the game starts */
#define PRINT_EVERY 30 /* steps between fingerprints compared */
#define PRINTS 64
#define RESEND_MS 12
#define HOST_GONE_MS 10000
#define ANSWER_MOST_MS 400 /* the longest a joiner does nothing but answer the host's timed pings (time_pings) */
#define RTT_MOST_MS 10000  /* a ping back that says it took longer than this, or less than nothing, is wrong */
#ifdef NET_FUZZ
#define END_WAIT_MS 0      /* the fuzzer's machine has nobody to see the last steps off to */
#else
#define END_WAIT_MS 1500   /* how long the host's goodbye waits for the others to have every step it played */
#endif
#define SLOW_MS 100        /* a wait this long for a machine's pads, or the host's packets, and its connection is slow */
#define SLOW_SHOWN_MS 3000 /* and it's said to be for this long after */

enum { PAD_THERE = 1, PAD_STICKS = 2, PAD_RUMBLES = 4 };
enum { T_JOIN = 1, T_WELCOME, T_PING, T_PONG, T_SAVE, T_START, T_READY, T_INPUT, T_RECORD, T_BYE, T_FIND, T_HERE };
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
    Uint64 slow_at;  /* when the game last had to wait for it, or 0 */
    int32_t have;    /* its pads are here for every step before this */
    int32_t acked;   /* it has every step's record before this */
    int ready;       /* what it has of the start */
    uint32_t nonce;
    int rtt, rtts;   /* the quickest a packet's been there and back, of how many tries */
    int timed[TIMED], timeds; /* and how long each took as the game was about to start, with both sides doing nothing else */
    Uint64 pinged;   /* when it was last pinged, in the lobby */
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
static int Welcomed, Refused, Ready, Ended, Differed;
static int Lobby; /* where hosting or joining has got to (LOBBY_, net.h) */
static int Timing; /* the host: pings are being timed (time_pings) */
static void lobby_fail(const char *why);
/* Games found on this network, by asking everyone on it (T_FIND) */
#define FOUND 3
static struct {
    struct sockaddr_in addr;
    uint32_t session;
    int machines;
    Uint64 seen;
} Found[FOUND];
static int NumFound;
static Uint64 AnswerTill; /* a joiner: till then it does nothing but answer pings, so they're timed truly */
static int HostLeft; /* a joiner: the host has said goodbye, and what it sent before that is all there is */
static int LeftOut;  /* a joiner: the host is playing on without waiting for this machine, till its pads arrive in time again */
static int Gone;     /* a joiner: the machines the host is playing on without (bit m), and those gone for good (bit 4 + m) */
static int Slow;     /* a joiner: the machines the host has lately had to wait for (bit m) */
/* A joiner in the lobby: who's there, as the host last said: how many machines, which is this one, and the players each brings */
static int RosterMachines, RosterMe, RosterPads[MACHINES];
static FILE *Recording, *Replaying;
static int ReplayPlayers; /* how many players a recording's game had */
/* how it went, for a test run to say */
static int Sent, Received, Lost, Waits, Quiets;
static Uint64 Waited, Longest;

static void say(const char *format, ...) {
    va_list args;

#ifdef NET_FUZZ
    /* the fuzzer's millions of runs, quietly */
    if (getenv("NETFUZZ_SAY") == NULL) {
        return;
    }
#endif
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

    if (Sock == INVALID_SOCKET) {
        return; /* (FD_SET of none is no set at all) */
    }
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
    /* back to the launcher, if that's where the game was started; else the program ends, saying why */
    platform_online_stop(why);
}

static void differs(int32_t step, int machine) {
    char why[200];
    int m;

    if (Differed || Ended) {
        return;
    }
    Differed = 1;
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

/* The machines the game's going on without, for the others to show: bit m for one that isn't answering, bit 4 + m for one that's
   gone for good. */
static int gone_mask(void) {
    int m, mask = 0;

    for (m = 1; m < NumMachines; m++) {
        mask |= (Machines[m].quiet ? 1 << m : 0) | (Machines[m].left ? 16 << m : 0);
    }
    return mask;
}

/* Whether the game has had to wait for a machine in the last few seconds. */
static int slow_now(const Machine *mc) {
    return mc->slow_at != 0 && SDL_GetTicks() - mc->slow_at < SLOW_SHOWN_MS;
}

static int slow_mask(void) {
    int m, mask = 0;

    for (m = 1; m < NumMachines; m++) {
        mask |= slow_now(&Machines[m]) ? 1 << m : 0;
    }
    return mask;
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
    p = put(p, (uint32_t)gone_mask(), 1);
    p = put(p, (uint32_t)slow_mask(), 1);
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
        } else if (Lobby != LOBBY_OPEN || (Want > 0 && NumMachines > Want) || NumMachines >= MACHINES) {
            send_welcome(from, nonce, BYE_FULL, 0);
        } else {
            mc = &Machines[NumMachines];
            memset(mc, 0, sizeof(*mc));
            prints_clear(mc->prints);
            mc->addr = *from;
            mc->nonce = nonce;
            mc->pads = pads < 1 ? 1 : pads > PADS ? PADS : (int)pads;
            mc->heard = SDL_GetTicks();
            say("%s has joined, with %d controller%s: %d here", address_text(from), mc->pads, mc->pads == 1 ? "" : "s", NumMachines + 1);
            send_welcome(from, nonce, 0, NumMachines++);
        }
        return;
    }
    if (type == T_FIND) {
        /* someone looking for games on this network: there's one here while the lobby's open */
        if (get(r, 2) == PROTOCOL && !r->bad && Lobby == LOBBY_OPEN) {
            uint8_t buf[32], *p = begin(buf, T_HERE);

            p = put(p, PROTOCOL, 2);
            p = put(p, build_hash(), 4);
            p = put(p, (uint32_t)NumMachines, 1);
            p = put(p, Session, 4);
            send_packet(from, buf, p);
        }
        return;
    }
    if (get(r, 4) != Session) {
        return;
    }
    get(r, 1);
    /* by its address: a machine's number changes if one before it leaves the lobby */
    for (m = 1; m < NumMachines && !same_address(&Machines[m].addr, from); m++) {
    }
    if (r->bad || m >= NumMachines || Machines[m].left) {
        return;
    }
    mc = &Machines[m];
    mc->heard = SDL_GetTicks();
    if (type == T_PONG) {
        uint32_t sent = get(r, 4);

        if (!r->bad && (uint32_t)SDL_GetTicks() - sent <= RTT_MOST_MS) {
            int took = (int)((uint32_t)SDL_GetTicks() - sent);

            /* the quickest: from the launcher each side only looks at what's come once a vblank, so most tries wait on that, not on
               the network */
            mc->rtt = mc->rtts == 0 || took < mc->rtt ? took : mc->rtt;
            mc->rtts = mc->rtts < PINGS ? mc->rtts + 1 : PINGS;
            if (Timing && mc->timeds < TIMED) {
                mc->timed[mc->timeds++] = took;
            }
        }
    } else if (type == T_READY) {
        mc->ready |= (int)get(r, 1) & READY_ALL;
    } else if (type == T_BYE) {
        int reason = (int)get(r, 1);
        int32_t step = (int32_t)get(r, 4);

        if (reason == BYE_DIFFERS && Role == HOST) {
            differs(step, m);
        }
        if (Role != HOST) {
            /* still in the lobby: there's one fewer */
            say("%s has left", address_text(from));
            if (Lobby == LOBBY_STARTING) {
                lobby_fail("The game couldn't start: a player left");
                return;
            }
            memmove(&Machines[m], &Machines[m + 1], sizeof(Machine) * (size_t)(NumMachines - m - 1));
            NumMachines--;
            return;
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

        if (r->bad || nonce != Machines[Me].nonce || Welcomed) {
            return;
        }
        if (refused != 0) {
            Refused = (int)refused;
        } else if (machine >= 1 && machine < MACHINES) {
            Welcomed = 1;
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
        uint32_t sent = get(r, 4), timed = get(r, 1), machines = get(r, 1), me = get(r, 1), m;
        int pads[MACHINES];

        p = put(p, Session, 4);
        p = put(p, (uint32_t)Me, 1);
        p = put(p, sent, 4);
        send_packet(&host->addr, buf, p);
        if (!r->bad && timed != 0 && Lobby == LOBBY_IN && Role == OFFLINE) {
            AnswerTill = SDL_GetTicks() + 60;
        }
        /* and who's in the lobby now, for this machine to show */
        for (m = 0; m < machines && m < MACHINES; m++) {
            pads[m] = (int)get(r, 1);
        }
        if (!r->bad && machines >= 2 && machines <= MACHINES && me >= 1 && me < machines) {
            RosterMachines = (int)machines;
            RosterMe = (int)me;
            memcpy(RosterPads, pads, sizeof(pads[0]) * machines);
        }
    } else if (type == T_SAVE || type == T_START) {
        uint8_t buf[32], *p = begin(buf, T_READY);

        if (type == T_SAVE) {
            uint32_t slot = get(r, 1), size = get(r, 2);

            if (r->bad || slot >= SAVES_SLOTS || size > SAVES_MOST || (uint32_t)(r->end - r->p) < size) {
                return;
            }
            if (!(Ready & (1 << slot))) {
                char why[64];

                /* the game would take it as it is: one it couldn't have made could have it read or write anywhere */
                if (!card_check(r->p, (int)size, why, sizeof(why))) {
                    say("the host's save slot %d isn't one the game could have made (%s)", (int)slot + 1, why);
                    send_bye(&host->addr, BYE_DATA, 0);
                    lobby_fail("The host's saves aren't ones this game can play with");
                    return;
                }
                saves_put((int)slot, r->p, (int)size);
                Ready |= 1 << slot;
            }
        } else if (!(Ready & READY_START)) {
            uint32_t delay = get(r, 1), machines = get(r, 1), me = get(r, 1), flags = get(r, 1);
            uint32_t m;

            if (r->bad || machines < 2 || machines > MACHINES || me < 1 || me >= machines || delay < 1 || delay > HISTORY / 8) {
                return;
            }
            /* which machine this is now: one that joined before it may have left since */
            if ((int)me != Me) {
                Machines[me].nonce = Machines[Me].nonce;
                Me = (int)me;
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
        uint32_t print = get(r, 4), quiet = get(r, 1), gone = get(r, 1), slow = get(r, 1);
        int32_t first = (int32_t)get(r, 4);
        int count = (int)get(r, 1), i;

        if (r->bad || count > PER_PACKET || first < 0 || r->end - r->p < count * RECORD_BYTES) {
            return;
        }
        if (host_next >= HostNext) {
            LeftOut = (int)quiet;
            Gone = (int)gone;
            Slow = (int)slow;
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

/* Who's hosting on this network, as they answer */
static void finder_takes(int type, Reader *r, const struct sockaddr_in *from) {
    uint32_t protocol = get(r, 2), build = get(r, 4), machines = get(r, 1), session = get(r, 4);
    int i;

    if (type != T_HERE || r->bad || protocol != PROTOCOL || build != build_hash()) {
        return;
    }
    /* one game, whichever of its addresses answered first */
    for (i = 0; i < NumFound && Found[i].session != session; i++) {
    }
    if (i == NumFound) {
        if (NumFound == FOUND) {
            return;
        }
        NumFound++;
        Found[i].addr = *from;
        Found[i].session = session;
    }
    Found[i].machines = (int)machines;
    Found[i].seen = SDL_GetTicks();
}

static void pump_once(void);

/* What's come is taken in. A joiner the host is timing does nothing else till that's over, so its answers go back at once. */
static void pump(void) {
    static int answering;

    pump_once();
    if (!answering) {
        Uint64 began = SDL_GetTicks();

        answering = 1;
        /* a host that kept timing would have this machine answer it and nothing else: the window has to answer too */
        while (Sock != INVALID_SOCKET && SDL_GetTicks() < AnswerTill && SDL_GetTicks() - began < ANSWER_MOST_MS) {
            wait_packet(1);
            pump_once();
        }
        answering = 0;
    }
}

/* A packet that's come, whatever's in it and whoever it's from. */
static void take_packet(const uint8_t *bytes, int size, const struct sockaddr_in *from) {
    Reader r;
    int type;

    r.p = bytes;
    r.end = bytes + size;
    r.bad = 0;
    if (size > PACKET_MOST || get(&r, 4) != MAGIC) {
        return;
    }
    type = (int)get(&r, 1);
    if (Lobby == LOBBY_FINDING) {
        finder_takes(type, &r, from);
    } else if (Me == 0) {
        host_takes(type, &r, from);
    } else {
        joiner_takes(type, &r, from);
    }
}

static void pump_once(void) {
    if (Sock == INVALID_SOCKET) {
        return;
    }
    send_late();
    for (;;) {
        uint8_t buf[PACKET_MOST + 64];
        struct sockaddr_in from;
        socklen_t from_size = sizeof(from);
        int size = (int)recvfrom(Sock, (char *)buf, sizeof(buf), 0, (struct sockaddr *)&from, &from_size);

        if (size <= 0) {
            return;
        }
        Received++;
        if (from_size < (socklen_t)sizeof(from) || from.sin_family != AF_INET) {
            continue;
        }
        take_packet(buf, size, &from);
        if (Sock == INVALID_SOCKET) {
            return; /* what came ended the lobby */
        }
    }
}

/* --- starting: the lobby, a frame at a time, so the launcher can show it (net_lobby_poll) --- */

static char Problem[128]; /* why the last try failed, for the launcher to say */
static Uint64 LobbyBegan, LobbySent;
static int OwnPads, LobbyPort, AskFor = 8000;

static int default_port(void) {
    return Options.port > 0 ? Options.port : NET_PORT;
}

/* Windows wants asking before the first socket. */
static int sockets_start(void) {
#ifdef _WIN32
    static int started;
    WSADATA data;

    if (!started && WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        return 0;
    }
    started = 1;
#endif
    return 1;
}

static int open_socket(const char *address, int port, int bind_it, struct sockaddr_in *out) {
    struct addrinfo hints, *found = NULL;
    char service[16];
    int yes = 1;
#ifdef _WIN32
    u_long on = 1;
    DWORD off = 0, got = 0;
#endif

    if (!sockets_start()) {
        return 0;
    }
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
    /* for asking a whole network who's hosting */
    setsockopt(Sock, SOL_SOCKET, SO_BROADCAST, (const char *)&yes, sizeof(yes));
    return !bind_it || bind(Sock, (const struct sockaddr *)out, sizeof(*out)) == 0;
}

static void close_socket(void) {
    if (Sock != INVALID_SOCKET) {
        closesocket(Sock);
        Sock = INVALID_SOCKET;
    }
}

/* "ADDRESS:PORT", or just "PORT", or just "ADDRESS" where there's a port to fall back on: 0 if it's none of them. */
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
    } else if (strchr(text, '.') != NULL && *port > 0) {
        SDL_strlcpy(address, text, room);
        return 1;
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

    fprintf(Recording, "crashbash online %d\ndelay %d\nplayers %d\nsettings %d %d %d\n", PROTOCOL, Delay, net_players(), s->skip_intro, s->skip_loading, s->cheat_menu);
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

/* --- join codes ---
 *
 * What a player at another home needs to join is the host's address as the world sees it and the port its router has opened: six
 * bytes, more than anyone wants to read out. The port is one of eight in a row (portmap.c), so the address and which port are 35
 * bits, and with 5 more to catch a slip they're eight characters of 32, written XXXX-XXXX. The 32 are the digits and the letters
 * without I, L, O and U, so none is read as another (an O typed is taken as 0, an I or L as 1). The 35 bits are stirred first, so
 * that the codes of addresses that start alike don't. */
static const char CodeLetters[] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
#define CODE_BITS 35
#define CODE_MASK ((1ULL << CODE_BITS) - 1)
#define CODE_STIR 0x5DEECE66DULL /* odd, so multiplying by it loses nothing */
#define CODE_ADD 0x2B992DDFAULL

/* Five bits that change if any one character does. */
static uint32_t code_sum(uint64_t v) {
    uint32_t sum = 0x1F;
    int i;

    for (i = CODE_BITS - 1; i >= 0; i--) {
        uint32_t top = ((sum >> 4) ^ (uint32_t)(v >> i)) & 1;

        sum = ((sum << 1) & 0x1F) ^ (top ? 0x05 : 0);
    }
    return sum;
}

int net_code_make(const char *address, int port, char *text, size_t room) {
    unsigned a, b, c, d;
    uint64_t v;
    int i, n = 0;

    if (room < 10 || sscanf(address, "%u.%u.%u.%u", &a, &b, &c, &d) != 4 || a > 255 || b > 255 || c > 255 || d > 255 || port < default_port()
        || port >= default_port() + 8) {
        return 0;
    }
    v = ((uint64_t)(a << 24 | b << 16 | c << 8 | d) << 3) | (uint64_t)(port - default_port());
    v = (v * CODE_STIR + CODE_ADD) & CODE_MASK;
    v = v << 5 | code_sum(v);
    for (i = 7; i >= 0; i--) {
        if (i == 3) {
            text[n++] = '-';
        }
        text[n++] = CodeLetters[(v >> (i * 5)) & 31];
    }
    text[n] = '\0';
    return 1;
}

int net_code_read(const char *text, char *where, size_t room) {
    uint64_t v = 0, unstir = CODE_STIR;
    int n = 0, i;

    for (; *text != '\0'; text++) {
        char c = *text >= 'a' && *text <= 'z' ? (char)(*text - 'a' + 'A') : *text;
        const char *at;

        if (c == '-' || c == ' ') {
            continue;
        }
        c = c == 'O' ? '0' : c == 'I' || c == 'L' ? '1' : c;
        at = strchr(CodeLetters, c);
        if (at == NULL || n == 8) {
            return 0;
        }
        v = v << 5 | (uint64_t)(at - CodeLetters);
        n++;
    }
    if (n != 8 || code_sum(v >> 5) != (v & 31)) {
        return 0;
    }
    /* the stir undone: what CODE_STIR times gives 1, found by doubling the bits it's right for */
    for (i = 0; i < 6; i++) {
        unstir *= 2 - CODE_STIR * unstir;
    }
    v = (((v >> 5) - CODE_ADD) * unstir) & CODE_MASK;
    SDL_snprintf(where, room, "%u.%u.%u.%u:%d", (unsigned)(v >> 27) & 255, (unsigned)(v >> 19) & 255, (unsigned)(v >> 11) & 255, (unsigned)(v >> 3) & 255,
                 default_port() + (int)(v & 7));
    return 1;
}

/* Codes made and read back, and every one-character slip in some caught. */
int net_code_check(void) {
    static const char *const addresses[] = { "0.0.0.0", "255.255.255.255", "192.168.1.20", "10.0.0.1", "203.0.113.7", "86.12.200.3", "1.2.3.4" };
    char code[16], where[64], want[64];
    size_t a;
    int port, i, k;

    for (a = 0; a < sizeof(addresses) / sizeof(addresses[0]); a++) {
        for (port = default_port(); port < default_port() + 8; port++) {
            SDL_snprintf(want, sizeof(want), "%s:%d", addresses[a], port);
            if (!net_code_make(addresses[a], port, code, sizeof(code)) || !net_code_read(code, where, sizeof(where)) || strcmp(where, want) != 0) {
                printf("codes: %s came back from %s as %s\n", want, code, where);
                return 0;
            }
            for (i = 0; code[i] != '\0'; i++) {
                char was = code[i];

                for (k = 0; k < 32 && was != '-'; k++) {
                    code[i] = CodeLetters[k];
                    if (code[i] != was && net_code_read(code, where, sizeof(where))) {
                        printf("codes: %s with a wrong character at %d was taken\n", code, i);
                        return 0;
                    }
                }
                code[i] = was;
            }
        }
    }
    return !net_code_make("192.168.1.20", default_port() + 8, code, sizeof(code)) && !net_code_read("KQ7M-2XP", where, sizeof(where))
        && net_code_make("192.168.1.20", default_port(), code, sizeof(code)) && code[4] == '-' && strlen(code) == 9;
}

/* Everything of a game that's over, or never began, put away, so that another can start. */
static void reset_session(void) {
    portmap_close();
    close_socket();
    if (Recording != NULL) {
        fclose(Recording);
        Recording = NULL;
    }
    if (Replaying != NULL) {
        fclose(Replaying);
        Replaying = NULL;
    }
    settings_return();
    saves_in_memory(0);
    Role = OFFLINE;
    ReplayPlayers = 0;
    Session = 0;
    Delay = NumMachines = Me = Want = 0;
    memset(Machines, 0, sizeof(Machines));
    memset(Now, 0, sizeof(Now));
    Have = Next = HostNext = HostAcked = 0;
    prints_clear(Mine);
    Welcomed = Refused = Ready = Ended = HostLeft = LeftOut = Differed = 0;
    Gone = Slow = RosterMachines = RosterMe = 0;
    AnswerTill = 0;
    Timing = 0;
    LateFirst = LateCount = 0;
    Sent = Received = Lost = Waits = Quiets = 0;
    Waited = Longest = 0;
    NumFound = 0;
}

static void lobby_fail(const char *why) {
    SDL_strlcpy(Problem, why, sizeof(Problem));
    say("%s", why);
    settings_return();
    reset_session();
    Lobby = LOBBY_FAILED;
}

/* Whether this machine's saves are all ones the game could have made (card_check): the others will be playing with them. */
static int own_saves_fine(void) {
    uint8_t save[SAVES_MOST];
    char why[64], text[128];
    int slot, size;

    for (slot = 0; slot < SAVES_SLOTS; slot++) {
        size = saves_get(slot, save, sizeof(save));
        if (!card_check(save, size, why, sizeof(why))) {
            say("save slot %d isn't one the game could have made (%s), so it can't be played online", slot + 1, why);
            SDL_snprintf(text, sizeof(text), "Save slot %d can't be played online: the game couldn't have made it", slot + 1);
            lobby_fail(text);
            return 0;
        }
    }
    return 1;
}

int net_lobby_host(const char *where) {
    char address[128];
    struct sockaddr_in here;
    int port = default_port();

    reset_session();
    Problem[0] = '\0';
    if (!own_saves_fine()) {
        return 0;
    }
    /* from the launcher: the usual port, on every address this PC has (a test: only its own) */
    if (where == NULL) {
        where = Options.local ? "127.0.0.1" : "0.0.0.0";
    }
    if (!split_address(where, address, sizeof(address), &port) || !open_socket(address[0] != '\0' ? address : NULL, port, 1, &here)) {
        lobby_fail("Can't host a game: is this PC hosting one already?");
        return 0;
    }
    LobbyPort = port;
    Me = 0;
    NumMachines = 1;
    Session = (uint32_t)SDL_GetPerformanceCounter() * 2654435761u + (uint32_t)SDL_GetTicksNS();
    Random ^= Session;
    prints_clear(Machines[0].prints);
    Lobby = LOBBY_OPEN;
    say("hosting on port %d", port);
    /* and the router's asked to let the world in at it (portmap.c): not for a test's game on this PC alone, unless it pretends */
    if (Options.pretend_router != 0 || (!Options.local && Options.host == NULL)) {
        portmap_open(port, default_port(), Options.pretend_router);
    }
    return 1;
}

/* A ping, which also says who's in the lobby, for the machine to show: how many machines, which of them it is, and the players each
   brings. */
static void send_ping(int m, Uint64 now, int timed) {
    uint8_t buf[32], *p = begin(buf, T_PING);
    int k;

    p = put(p, Session, 4);
    p = put(p, (uint32_t)now, 4);
    p = put(p, (uint32_t)timed, 1);
    p = put(p, (uint32_t)NumMachines, 1);
    p = put(p, (uint32_t)m, 1);
    for (k = 0; k < NumMachines; k++) {
        p = put(p, (uint32_t)(k == 0 ? own_pads() : Machines[k].pads), 1);
    }
    send_packet(&Machines[m].addr, buf, p);
}

/* How long a packet takes to each machine and back, timed with nothing in the way: for a quarter of a second the host and the
   joiners do nothing but this. In the lobby each side only looks at what's come once a vblank, which adds up to two of them to
   every ping, and the delay would be a step longer than the network needs. The middle one of each machine's, so one late packet
   doesn't count. */
static void time_pings(void) {
    Uint64 began = SDL_GetTicks(), sent = 0, now;
    int m, i, j;

    Timing = 1;
    for (m = 1; m < NumMachines; m++) {
        Machines[m].timeds = 0;
    }
    while ((now = SDL_GetTicks()) - began < 260 && Lobby == LOBBY_OPEN) {
        if (now - sent >= 10 && now - began < 220) {
            sent = now;
            for (m = 1; m < NumMachines; m++) {
                send_ping(m, now, 1);
            }
        }
        wait_packet(1);
        pump();
    }
    Timing = 0;
    for (m = 1; m < NumMachines && Lobby == LOBBY_OPEN; m++) {
        Machine *mc = &Machines[m];
        /* the first few come before the other side's noticed it's being timed */
        int *v = mc->timed + (mc->timeds > 8 ? 4 : 0), n = mc->timeds > 8 ? mc->timeds - 4 : mc->timeds;

        for (i = 1; i < n; i++) {
            for (j = i; j > 0 && v[j - 1] > v[j]; j--) {
                int t = v[j];

                v[j] = v[j - 1];
                v[j - 1] = t;
            }
        }
        if (n > 0) {
            mc->rtt = v[n / 2];
        }
    }
}

/* The host: everyone's here. They're sent the host's saves and the start, till each says it has them. */
void net_lobby_start(void) {
    int m, worst = 0, next_pad = 0;

    if (Lobby != LOBBY_OPEN || NumMachines < 2) {
        return;
    }
    time_pings();
    if (Lobby != LOBBY_OPEN || NumMachines < 2) {
        return;
    }
    for (m = 1; m < NumMachines; m++) {
        int rtt = Machines[m].rtt;

        say("machine %d: %d ms there and back", m + 1, rtt);
        worst = rtt > worst ? rtt : worst;
    }
    /* long enough for a press to reach the host and its record to come back, with a little over, and the wait for a vblank's look at
       what's come */
    Delay = Options.delay > 0 ? Options.delay : (int)((worst * 1.2 + 25.0) / STEP_MS) + 1;
    Delay = Delay > 30 ? 30 : Delay < 1 ? 1 : Delay;
    /* the host's players first, then each machine's as it joined */
    Machines[0].pads = own_pads();
    for (m = 0; m < NumMachines; m++) {
        Machines[m].first = next_pad;
        Machines[m].pads = Machines[m].pads > PADS - next_pad ? PADS - next_pad : Machines[m].pads;
        Machines[m].ready = 0;
        next_pad += Machines[m].pads;
    }
    LobbyBegan = SDL_GetTicks();
    LobbySent = 0;
    Lobby = LOBBY_STARTING;
}

int net_lobby_join(const char *where) {
    char address[128];
    struct sockaddr_in host;
    int port = default_port(), slot;

    char from_code[64];

    reset_session();
    Problem[0] = '\0';
    /* a join code, or an address */
    if (strchr(where, '.') == NULL && strchr(where, ':') == NULL) {
        if (!net_code_read(where, from_code, sizeof(from_code))) {
            lobby_fail("That code isn't right: it's eight letters and digits, as KQ7M-2XPA");
            return 0;
        }
        where = from_code;
    }
    if (!split_address(where, address, sizeof(address), &port) || address[0] == '\0' || !open_socket(address, port, 0, &host)) {
        lobby_fail("That isn't an address: it's the host's, as 192.168.1.20");
        return 0;
    }
    Me = 1; /* till the host says which */
    Machines[0].addr = host;
    Machines[0].heard = SDL_GetTicks();
    prints_clear(Machines[0].prints);
    Random ^= (uint32_t)SDL_GetPerformanceCounter() * 2654435761u;
    Machines[1].nonce = random_next();
    OwnPads = own_pads();
    for (slot = 0; slot < SAVES_SLOTS; slot++) {
        saves_put(slot, "", 0);
    }
    LobbyBegan = SDL_GetTicks();
    LobbySent = 0;
    Lobby = LOBBY_ASKING;
    say("joining %s", address_text(&host));
    return 1;
}

/* Looks for games on this network, from now till something's hosted or joined: whoever's hosting answers (net_lobby_found). */
void net_lobby_find(void) {
    struct sockaddr_in everyone;
    int port = default_port();

    if (Lobby == LOBBY_FINDING) {
        return;
    }
    reset_session();
    Lobby = LOBBY_OFF;
    if (!open_socket(Options.local ? "127.0.0.1" : "255.255.255.255", port, 0, &everyone)) {
        return;
    }
    Machines[0].addr = everyone;
    LobbyPort = port;
    Me = 1;
    LobbySent = 0;
    Lobby = LOBBY_FINDING;
}

int net_lobby_found(int i, char *text, size_t room) {
    char ip[32] = "?";

    if (Lobby != LOBBY_FINDING || i < 0 || i >= NumFound) {
        return 0;
    }
    inet_ntop(AF_INET, &Found[i].addr.sin_addr, ip, sizeof(ip));
    if (ntohs(Found[i].addr.sin_port) == LobbyPort) {
        SDL_snprintf(text, room, "%s", ip);
    } else {
        SDL_snprintf(text, room, "%s:%d", ip, ntohs(Found[i].addr.sin_port));
    }
    return Found[i].machines;
}

int net_lobby_machines(void) {
    return Lobby == LOBBY_OPEN || Lobby == LOBBY_STARTING ? NumMachines : Lobby == LOBBY_IN ? RosterMachines : 0;
}

int net_lobby_me(void) {
    return Lobby == LOBBY_IN ? RosterMe : 0;
}

/* A machine in the lobby, and how many players it brings. To the host: its address and ping as text (nothing for the host's own). To
   a machine that's joined: the host's address, and nothing for the others. */
int net_lobby_machine(int m, char *text, size_t room) {
    char ip[32] = "?";

    text[0] = '\0';
    if (m < 0 || m >= net_lobby_machines()) {
        return 0;
    }
    if (Lobby == LOBBY_IN) {
        if (m == 0) {
            inet_ntop(AF_INET, &Machines[0].addr.sin_addr, ip, sizeof(ip));
            SDL_snprintf(text, room, "%s", ip);
        }
        return RosterPads[m];
    }
    if (m == 0) {
        return own_pads();
    }
    inet_ntop(AF_INET, &Machines[m].addr.sin_addr, ip, sizeof(ip));
    SDL_snprintf(text, room, "%s, %d ms", ip, Machines[m].rtt);
    return Machines[m].pads;
}

/* --- the networks this machine is on ---
 *
 * A PC is often on more than one: its Wi-Fi or cable, and a virtual network (Hamachi, Radmin VPN, ZeroTier, Tailscale), which is what
 * players use when no router will let them in (docs/PORT.md, Online play). Each is one more adapter with an address of its own. A
 * game is looked for on every one of them, and the host's screen says the address to join at on each that players may be on. */
typedef struct {
    struct in_addr address;
    struct in_addr everyone; /* the address that reaches everyone on it, or 0.0.0.0 if it has none */
    char called[160];        /* its name, and on Windows what it is too: "Hamachi (LogMeIn Hamachi Virtual Ethernet Adapter)", or "ham0" */
} Adapter;

static int adapters(Adapter *out, int most) {
    int n = 0;
    /* (a test's games are this PC's alone, and so none of its real networks count: Options.local) */
#ifdef _WIN32
    ULONG size = 32 * 1024;
    IP_ADAPTER_ADDRESSES *list = Options.local ? NULL : malloc(size), *a;

    if (list == NULL || GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER, NULL, list, &size) != NO_ERROR) {
        free(list);
        list = NULL;
    }
    for (a = list; a != NULL; a = a->Next) {
        IP_ADAPTER_UNICAST_ADDRESS *u;

        if (a->OperStatus != IfOperStatusUp || a->IfType == IF_TYPE_SOFTWARE_LOOPBACK) {
            continue;
        }
        for (u = a->FirstUnicastAddress; u != NULL && n < most; u = u->Next) {
            const struct sockaddr_in *at = (const struct sockaddr_in *)u->Address.lpSockaddr;
            char *name, *what;

            if (at->sin_family != AF_INET) {
                continue;
            }
            out[n].address = at->sin_addr;
            out[n].everyone.s_addr = 0;
            if (u->OnLinkPrefixLength > 0 && u->OnLinkPrefixLength < 31) {
                out[n].everyone.s_addr = at->sin_addr.s_addr | ~htonl(0xFFFFFFFFu << (32 - u->OnLinkPrefixLength));
            }
            name = SDL_iconv_string("UTF-8", "UTF-16LE", (const char *)a->FriendlyName, (SDL_wcslen(a->FriendlyName) + 1) * sizeof(WCHAR));
            what = SDL_iconv_string("UTF-8", "UTF-16LE", (const char *)a->Description, (SDL_wcslen(a->Description) + 1) * sizeof(WCHAR));
            SDL_snprintf(out[n].called, sizeof(out[n].called), "%s (%s)", name != NULL ? name : "?", what != NULL ? what : "?");
            SDL_free(name);
            SDL_free(what);
            n++;
        }
    }
    free(list);
#elif !defined(__ANDROID__) || __ANDROID_API__ >= 24
    struct ifaddrs *list, *a;

    if (Options.local || getifaddrs(&list) != 0) {
        list = NULL;
    }
    for (a = list; a != NULL && n < most; a = a->ifa_next) {
        if (a->ifa_addr == NULL || a->ifa_addr->sa_family != AF_INET || !(a->ifa_flags & IFF_UP) || (a->ifa_flags & IFF_LOOPBACK)) {
            continue;
        }
        out[n].address = ((const struct sockaddr_in *)a->ifa_addr)->sin_addr;
        out[n].everyone.s_addr = 0;
        if ((a->ifa_flags & IFF_BROADCAST) && a->ifa_broadaddr != NULL) {
            out[n].everyone = ((const struct sockaddr_in *)a->ifa_broadaddr)->sin_addr;
        }
        SDL_strlcpy(out[n].called, a->ifa_name, sizeof(out[n].called));
        n++;
    }
    if (list != NULL) {
        freeifaddrs(list);
    }
#endif
    /* a test's: one more, as the command line has it */
    if (Options.pretend_network != NULL && n < most) {
        const char *equals = strrchr(Options.pretend_network, '=');

        if (equals != NULL && inet_pton(AF_INET, equals + 1, &out[n].address) == 1) {
            size_t len = (size_t)(equals - Options.pretend_network);

            len = len < sizeof(out[n].called) - 1 ? len : sizeof(out[n].called) - 1;
            memcpy(out[n].called, Options.pretend_network, len);
            out[n].called[len] = '\0';
            out[n].everyone.s_addr = 0;
            n++;
        }
    }
    return n;
}

/* Each network by the address that reaches everyone on it, for asking who's hosting: the plain "everyone" address only goes out of the
   main adapter, and a virtual network's games should show by themselves like any other's. */
static int everyone_on_each(struct in_addr *out, int most) {
    Adapter all[16];
    int count = adapters(all, 16), n = 0, i;

    for (i = 0; i < count && n < most; i++) {
        if (all[i].everyone.s_addr != 0) {
            out[n++] = all[i].everyone;
        }
    }
    return n;
}

/* The virtual network an adapter is, by its tool's name, or NULL if it isn't one this knows: by what the adapter's called, or by its
   address where the tool has addresses nobody else uses (Hamachi's all start 25, Radmin VPN's 26). Other adapters a PC has (a virtual
   machine's, a work VPN's) are no use to players, and telling those apart by name is guesswork, so only these are offered. */
static const char *virtual_network(const char *called, struct in_addr address) {
    char low[160];
    unsigned first = ntohl(address.s_addr) >> 24;
    size_t i;

    for (i = 0; called[i] != '\0' && i < sizeof(low) - 1; i++) {
        low[i] = called[i] >= 'A' && called[i] <= 'Z' ? (char)(called[i] - 'A' + 'a') : called[i];
    }
    low[i] = '\0';
    if (strstr(low, "hamachi") != NULL || strncmp(low, "ham", 3) == 0 || first == 25) {
        return "Hamachi";
    }
    if (strstr(low, "radmin") != NULL || first == 26) {
        return "Radmin VPN";
    }
    if (strstr(low, "zerotier") != NULL || (low[0] == 'z' && low[1] == 't' && strchr(low, ' ') == NULL)) {
        return "ZeroTier";
    }
    if (strstr(low, "tailscale") != NULL) {
        return "Tailscale";
    }
    return NULL;
}

/* The main network's address: the one this machine's packets to the wider world would leave by. */
static void main_address(char *text, size_t room) {
    struct sockaddr_in there, mine; /* (far and near are words Windows has taken) */
    socklen_t size = sizeof(mine);
    SOCKET s;

    SDL_strlcpy(text, "127.0.0.1", room);
    if (Options.local || !sockets_start() || (s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        return;
    }
    memset(&there, 0, sizeof(there));
    there.sin_family = AF_INET;
    there.sin_port = htons(9);
    inet_pton(AF_INET, "192.0.2.1", &there.sin_addr);
    /* nothing's sent: a UDP socket's "connection" only picks the way out */
    if (connect(s, (const struct sockaddr *)&there, sizeof(there)) == 0 && getsockname(s, (struct sockaddr *)&mine, &size) == 0) {
        inet_ntop(AF_INET, &mine.sin_addr, text, (socklen_t)room);
    }
    closesocket(s);
}

/* The addresses a game here can be joined at: looked up again once a second, as the launcher asks every frame and a virtual network
   can be switched on while its lobby's open. */
#define NETWORKS 4
static struct {
    char address[32];
    const char *name;
} Networks[NETWORKS];
static int NumNetworks;
static Uint64 NetworksAt;

static void find_networks(void) {
    Adapter all[16];
    Uint64 now = SDL_GetTicks();
    int count, i, k;

    if (NumNetworks > 0 && now - NetworksAt < 1000) {
        return;
    }
    NetworksAt = now;
    main_address(Networks[0].address, sizeof(Networks[0].address));
    Networks[0].name = "";
    NumNetworks = 1;
    count = adapters(all, 16);
    for (i = 0; i < count && NumNetworks < NETWORKS; i++) {
        const char *name = virtual_network(all[i].called, all[i].address);
        char text[32] = "";

        inet_ntop(AF_INET, &all[i].address, text, sizeof(text));
        for (k = 0; name != NULL && k < NumNetworks; k++) {
            /* the main one may be it; and one address for each tool */
            if (strcmp(Networks[k].address, text) == 0 || strcmp(Networks[k].name, name) == 0) {
                name = NULL;
            }
        }
        if (name != NULL) {
            SDL_strlcpy(Networks[NumNetworks].address, text, sizeof(Networks[NumNetworks].address));
            Networks[NumNetworks++].name = name;
        }
    }
}

const char *net_lobby_address(void) {
    find_networks();
    return Networks[0].address;
}

int net_lobby_network(int i, char *address, size_t room, const char **name) {
    find_networks();
    if (i < 0 || i >= NumNetworks) {
        return 0;
    }
    SDL_strlcpy(address, Networks[i].address, room);
    *name = Networks[i].name;
    return 1;
}

/* For --check-router: every adapter, and what it's taken for, so a virtual network that isn't offered can be seen not to be. */
void net_say_networks(void) {
    Adapter all[16];
    char main_one[32], text[32];
    int count = adapters(all, 16), i;

    main_address(main_one, sizeof(main_one));
    printf("The networks this PC is on:\n");
    for (i = 0; i < count; i++) {
        const char *name = virtual_network(all[i].called, all[i].address);

        text[0] = '\0';
        inet_ntop(AF_INET, &all[i].address, text, sizeof(text));
        if (strcmp(text, main_one) == 0) {
            printf("  %s, %s: the main one, which players on it join at\n", all[i].called, text);
        } else if (name != NULL) {
            printf("  %s, %s: a virtual network (%s), which players on it join at\n", all[i].called, text, name);
        } else {
            printf("  %s, %s: not one for players\n", all[i].called, text);
        }
    }
    if (count == 0) {
        printf("  none\n");
    }
    fflush(stdout);
}

const char *net_lobby_problem(void) {
    return Problem;
}

int net_lobby_router(void) {
    return Lobby == LOBBY_OPEN || Lobby == LOBBY_STARTING ? portmap_state() : PORTMAP_OFF;
}

const char *net_lobby_code(void) {
    static char code[16];
    static int said;

    code[0] = '\0';
    if (net_lobby_router() != PORTMAP_OPEN || !net_code_make(portmap_address(), portmap_port(), code, sizeof(code))) {
        said = 0;
        return "";
    }
    if (!said) {
        said = 1;
        say("the router's port %d is open: the join code is %s", portmap_port(), code);
    }
    return code;
}

/* Backing out: the others are told. */
void net_lobby_leave(void) {
    int m;

    if (Lobby == LOBBY_OPEN || Lobby == LOBBY_STARTING) {
        for (m = 1; m < NumMachines; m++) {
            send_bye(&Machines[m].addr, BYE_LEFT, 0);
        }
    } else if ((Lobby == LOBBY_ASKING || Lobby == LOBBY_IN) && Welcomed) {
        send_bye(&Machines[0].addr, BYE_LEFT, 0);
    }
    while (LateCount > 0) {
        send_late();
        SDL_Delay(1);
    }
    settings_return();
    reset_session();
    Lobby = LOBBY_OFF;
}

/* Once a frame while there's a lobby: where it's got to. */
int net_lobby_poll(void) {
    const Settings *s = settings();
    Uint64 now;
    int m, i;

    if (Lobby == LOBBY_OFF || Lobby == LOBBY_GO || Lobby == LOBBY_FAILED) {
        return Lobby;
    }
    pump();
    now = SDL_GetTicks();
    if (Lobby == LOBBY_FINDING) {
        if (now - LobbySent >= 700 || LobbySent == 0) {
            uint8_t buf[16], *p = put(begin(buf, T_FIND), PROTOCOL, 2);

            LobbySent = now;
            send_packet(&Machines[0].addr, buf, p);
            if (!Options.local) {
                /* and each network this machine is on by name, a virtual one too; and this PC itself, which a network with nobody
                   else on it mightn't pass the question back to */
                struct sockaddr_in to = Machines[0].addr;
                struct in_addr each[8];
                int count = everyone_on_each(each, 8), k;

                for (k = 0; k < count; k++) {
                    to.sin_addr = each[k];
                    send_packet(&to, buf, p);
                }
                inet_pton(AF_INET, "127.0.0.1", &to.sin_addr);
                send_packet(&to, buf, p);
            }
        }
        for (i = 0; i < NumFound; i++) {
            if (now - Found[i].seen > 2500) {
                Found[i] = Found[--NumFound];
                i--;
            }
        }
    } else if (Lobby == LOBBY_OPEN) {
        /* how long a packet takes to each and back: quickly at first, then now and then, which also says each is still there */
        int pinged = 0;

        for (m = 1; m < NumMachines; m++) {
            if (now - Machines[m].pinged >= (Uint64)(Machines[m].rtts < PINGS ? 30 : 400)) {
                Machines[m].pinged = now;
                send_ping(m, now, 0);
                pinged = 1;
            }
        }
        /* a moment for an answer that's quick, which otherwise isn't seen till the next vblank's look */
        if (pinged && Options.lag == 0) {
            wait_packet(3);
            pump();
            if (Lobby != LOBBY_OPEN) {
                return Lobby;
            }
        }
        for (m = 1; m < NumMachines; m++) {
            if (now > Machines[m].heard && now - Machines[m].heard > 6000) {
                say("%s has gone", address_text(&Machines[m].addr));
                memmove(&Machines[m], &Machines[m + 1], sizeof(Machine) * (size_t)(NumMachines - m - 1));
                NumMachines--;
                m--;
            }
        }
    } else if (Lobby == LOBBY_STARTING) {
        int ready = 1;

        for (m = 1; m < NumMachines; m++) {
            ready &= Machines[m].ready == READY_ALL;
        }
        if (ready) {
            /* extra heroes come from each player's own disc, and there's no telling yet that everyone has them */
            settings_borrow(s->skip_intro, s->skip_loading, s->cheat_menu, 0);
            first_steps();
            Role = HOST;
            for (m = 0; m < NumMachines; m++) {
                say("machine %d has player%s %d to %d", m + 1, Machines[m].pads == 1 ? "" : "s", Machines[m].first + 1, Machines[m].first + Machines[m].pads);
            }
            say("starting: each press is used %d step%s late (%d ms); saves %08X", Delay, Delay == 1 ? "" : "s", (int)(Delay * STEP_MS), saves_hash());
            Lobby = LOBBY_GO;
        } else if (now - LobbyBegan > 15000) {
            lobby_fail("The game couldn't start: a player didn't answer");
        } else if (now - LobbySent >= 60) {
            LobbySent = now;
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
                if ((mc->ready & READY_SAVES) == READY_SAVES && !(mc->ready & READY_START)) {
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
    } else if (Refused != 0) {
        lobby_fail(Refused == BYE_VERSION ? "The host has another version of the game: everyone needs the same one"
                   : Refused == BYE_DATA ? "The host's game data isn't the same as this machine's" : "That game is full, or has started");
    } else if (Lobby == LOBBY_ASKING) {
        if (Welcomed) {
            Lobby = LOBBY_IN;
            say("in the lobby of %s", address_text(&Machines[0].addr));
        } else if (now - LobbyBegan > (Uint64)AskFor) {
            lobby_fail("Nobody's hosting a game at that address");
        } else if (now - LobbySent >= 250 || LobbySent == 0) {
            uint8_t buf[64], *p = begin(buf, T_JOIN);

            LobbySent = now;
            p = put(p, PROTOCOL, 2);
            p = put(p, build_hash(), 4);
            p = put(p, data_hash(), 4);
            p = put(p, (uint32_t)OwnPads, 1);
            p = put(p, Machines[1].nonce, 4);
            send_packet(&Machines[0].addr, buf, p);
        }
    } else if (Lobby == LOBBY_IN) {
        if (HostLeft) {
            lobby_fail("The host has stopped hosting");
        } else if (Ready == READY_ALL) {
            saves_in_memory(1);
            first_steps();
            Role = JOINER;
            say("joined as machine %d, with player%s %d to %d of %d machines: each press is used %d step%s late; saves %08X", Me + 1,
                Machines[Me].pads == 1 ? "" : "s", Machines[Me].first + 1, Machines[Me].first + Machines[Me].pads, NumMachines, Delay, Delay == 1 ? "" : "s", saves_hash());
            Lobby = LOBBY_GO;
        } else if (now > Machines[0].heard && now - Machines[0].heard > 6000) {
            lobby_fail("The host has gone");
        }
    }
    return Lobby;
}

/* From the command line (--host, --join): the same lobby, waited through here, with the window still answering. */
static int start_host(void) {
    int m;

    if (!net_lobby_host(Options.host)) {
        return 0;
    }
    Want = Options.joiners < 1 ? 1 : Options.joiners > MACHINES - 1 ? MACHINES - 1 : Options.joiners;
    say("waiting for %d to join", Want);
    for (;;) {
        int state = net_lobby_poll(), measured = NumMachines == Want + 1;

        if (state == LOBBY_GO || state == LOBBY_FAILED) {
            return state == LOBBY_GO;
        }
        for (m = 1; m < NumMachines; m++) {
            measured &= Machines[m].rtts >= PINGS;
        }
        if (state == LOBBY_OPEN && measured) {
            net_lobby_start();
        }
        if (!platform_idle()) {
            net_lobby_leave();
            return -1;
        }
        wait_packet(2);
    }
}

static int start_join(void) {
    /* a moment for the controllers to be found */
    platform_idle();
    platform_idle();
    AskFor = 30000;
    if (!net_lobby_join(Options.join)) {
        return 0;
    }
    for (;;) {
        int state = net_lobby_poll();

        if (state == LOBBY_GO || state == LOBBY_FAILED) {
            return state == LOBBY_GO;
        }
        if (!platform_idle()) {
            net_lobby_leave();
            return -1;
        }
        wait_packet(2);
    }
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

    reset_session();
    Replaying = fopen(Options.replay, "r");
    if (Replaying == NULL || fgets(line, sizeof(line), Replaying) == NULL || sscanf(line, "crashbash online %d", &a) != 1 || a != PROTOCOL) {
        say("%s isn't a game's record", Options.replay);
        return 0;
    }
    for (a = 0; a < SAVES_SLOTS; a++) {
        saves_put(a, "", 0);
    }
    saves_in_memory(1);
    for (;;) {
        long at = ftell(Replaying);

        if (fgets(line, sizeof(line), Replaying) == NULL) {
            break;
        }
        if (sscanf(line, "delay %d", &a) == 1) {
            Delay = a;
        } else if (sscanf(line, "players %d", &a) == 1) {
            ReplayPlayers = a;
        } else if (sscanf(line, "settings %d %d %d", &a, &b, &c) == 3) {
            settings_borrow(a, b, c, 0);
        } else if (sscanf(line, "save %d", &a) == 1 && strchr(line + 5, ' ') != NULL) {
            uint8_t save[SAVES_MOST];
            char why[64];
            int size = hex_bytes(strchr(line + 5, ' ') + 1, save, sizeof(save));

            if (!card_check(save, size, why, sizeof(why))) {
                say("%s's save slot %d isn't one the game could have made (%s)", Options.replay, a + 1, why);
                return 0;
            }
            saves_put(a, save, size);
        } else {
            fseek(Replaying, at, SEEK_SET);
            break;
        }
    }
    Role = REPLAY;
    say("playing %s again", Options.replay);
    return 1;
}

/* What the command line asked of online play, for a game from there or from the launcher. */
void net_configure(const NetOptions *options) {
    Options = *options;
    Patience = options->patience > 0 ? options->patience : 5000;
    prints_clear(Mine);
}

int net_start(void) {
    int ok = Options.replay != NULL ? start_replay() : Options.host != NULL ? start_host() : start_join();

    if (ok > 0) {
        net_record();
    }
    return ok;
}

/* Keeps the game's presses, as it starts: where --net-record asked, or else beside the saves, so that a game that stopped because the
   machines differed can be played again to see why (--replay). The last game's only: each is kept over the one before, about 7 MB
   an hour. That's the player's own saves folder, or a test run's --saves; a test run with none keeps nothing it wasn't asked to. */
void net_record(void) {
    char path[1100];
    const char *to = Options.record;

    if (Recording != NULL || (Role != HOST && Role != JOINER && Options.record == NULL)) {
        return;
    }
    if (to == NULL && saves_folder()[0] != '\0') {
        SDL_snprintf(path, sizeof(path), "%slast-online-game.txt", saves_folder());
        to = path;
    }
    if (to != NULL && (Recording = fopen(to, "w")) != NULL) {
        record_header();
        if (Options.record == NULL) {
            say("this game's presses are kept in %s, to play it again (--replay)", to);
        }
    }
}

/* A game that's over, put away: another can be hosted or joined. */
void net_lobby_reset(void) {
    reset_session();
    Lobby = LOBBY_OFF;
}

int net_on(void) {
    return Role != OFFLINE;
}

int net_machines(void) {
    return Role == HOST || Role == JOINER ? NumMachines : 0;
}

int net_me(void) {
    return Me;
}

int net_machine(int m, int *first, int *pads) {
    if (m < 0 || m >= net_machines()) {
        *first = *pads = 0;
        return NET_LEFT;
    }
    *first = Machines[m].first;
    *pads = Machines[m].pads;
    if (Role == HOST) {
        return Machines[m].left ? NET_LEFT : Machines[m].quiet ? NET_QUIET : slow_now(&Machines[m]) ? NET_SLOW : NET_PLAYING;
    }
    return (Gone >> 4) & (1 << m) ? NET_LEFT : Gone & (1 << m) ? NET_QUIET : (Slow & (1 << m)) || (m == 0 && slow_now(&Machines[0])) ? NET_SLOW : NET_PLAYING;
}

int net_players(void) {
    int m, players = 0;

    if (Role == REPLAY) {
        return ReplayPlayers;
    }
    for (m = 0; (Role == HOST || Role == JOINER) && m < NumMachines; m++) {
        players += Machines[m].pads;
    }
    return players;
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
            if (Have <= Next && now - began > SLOW_MS) {
                /* whoever this step is waiting for is holding everyone up */
                for (m = 1; m < NumMachines; m++) {
                    if (Machines[m].have <= Have && !Machines[m].quiet) {
                        if (!slow_now(&Machines[m])) {
                            say("machine %d is slow: the game waited for it, at step %d", m + 1, Have);
                        }
                        Machines[m].slow_at = now;
                    }
                }
            }
            if (Have <= Next && now - began >= (Uint64)Patience) {
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
        } else if (now > Machines[0].heard && now - Machines[0].heard > SLOW_MS) {
            /* nothing from the host for a while: while it waits for another machine, it still sends */
            Machines[0].slow_at = now;
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
    const uint8_t *b;

    if (pad < 0 || pad >= PADS) {
        return -1;
    }
    b = Now + pad * PAD_BYTES;
    return !(b[0] & PAD_THERE) ? -1 : b[1] | b[2] << 8;
}

int net_pad_sticks(int pad, unsigned char sticks[4]) {
    const uint8_t *b;

    if (pad < 0 || pad >= PADS || !(Now[pad * PAD_BYTES] & PAD_STICKS)) {
        return 0;
    }
    b = Now + pad * PAD_BYTES;
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

    if (Ended) {
        return;
    }
    if (Role != HOST && Role != JOINER) {
        /* in a lobby: pings are answered a vblank sooner than the launcher's own look, once a frame */
        if (Lobby != LOBBY_OFF && Lobby != LOBBY_GO && Lobby != LOBBY_FAILED) {
            pump();
        }
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
        } while (owed && SDL_GetTicks() - began < END_WAIT_MS);
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
    close_socket();
}

#ifdef NET_FUZZ
/* --- for the fuzzer (tests/netfuzz.c): a machine put straight into one of its states, with no socket, packets taken as if they'd
   come, and what it does each frame and each step done --- */

static const struct sockaddr_in *fuzz_address(int who) {
    static struct sockaddr_in at[NET_FUZZ_WHO];
    int i;

    for (i = 0; i < NET_FUZZ_WHO; i++) {
        at[i].sin_family = AF_INET;
        at[i].sin_port = htons((uint16_t)(NET_PORT + i));
        /* the host, then each machine that's joined it, then a stranger */
        at[i].sin_addr.s_addr = htonl(i < MACHINES ? 0x0A000001u + (uint32_t)i : 0xC0000263u);
    }
    return &at[who < 0 || who >= NET_FUZZ_WHO ? NET_FUZZ_WHO - 1 : who];
}

const struct sockaddr_in *net_fuzz_from(int who) {
    return fuzz_address(who);
}

uint32_t net_fuzz_build(void) {
    return build_hash();
}

void net_fuzz_begin(int state, int machines) {
    NetOptions o;
    int m, pad = 0;

    memset(&o, 0, sizeof(o));
    o.joiners = 1;
    o.differ_at = o.vanish_at = o.stall_at = -1;
    o.local = 1;
    net_configure(&o);
    Patience = 0;
    reset_session();
    Problem[0] = '\0';
    machines = machines < 2 ? 2 : machines > MACHINES ? MACHINES : machines;
    Session = NET_FUZZ_SESSION;
    LobbyPort = NET_PORT;
    LobbyBegan = LobbySent = SDL_GetTicks();
    for (m = 0; m < MACHINES; m++) {
        Machines[m].addr = *fuzz_address(m);
        Machines[m].heard = SDL_GetTicks();
        prints_clear(Machines[m].prints);
    }
    switch (state) {
    case NET_FUZZ_FINDING:
        Me = 1;
        Lobby = LOBBY_FINDING;
        break;
    case NET_FUZZ_HOST_OPEN:
    case NET_FUZZ_HOST_STARTING:
    case NET_FUZZ_HOST_PLAYING:
        Me = 0;
        NumMachines = machines;
        for (m = 0; m < machines; m++) {
            Machines[m].pads = 1;
            Machines[m].first = pad++;
        }
        Lobby = LOBBY_OPEN;
        if (state != NET_FUZZ_HOST_OPEN) {
            Delay = 2;
            Lobby = LOBBY_STARTING;
        }
        if (state == NET_FUZZ_HOST_PLAYING) {
            first_steps();
            Role = HOST;
            Lobby = LOBBY_GO;
        }
        break;
    default:
        Me = 1;
        Machines[1].nonce = NET_FUZZ_NONCE;
        Lobby = LOBBY_ASKING;
        if (state != NET_FUZZ_JOINER_ASKING) {
            Welcomed = 1;
            Lobby = LOBBY_IN;
        }
        if (state == NET_FUZZ_JOINER_PLAYING) {
            NumMachines = machines;
            for (m = 0; m < machines; m++) {
                Machines[m].pads = 1;
                Machines[m].first = pad++;
            }
            Delay = 2;
            Ready = READY_ALL;
            saves_in_memory(1);
            first_steps();
            Role = JOINER;
            Lobby = LOBBY_GO;
        }
        break;
    }
}

void net_fuzz_take(const uint8_t *bytes, size_t size, int who) {
    take_packet(bytes, size > PACKET_MOST + 64 ? PACKET_MOST + 64 : (int)size, fuzz_address(who));
}

void net_fuzz_tick(uint32_t fingerprint) {
    char text[96];
    unsigned char sticks[4];
    const char *name;
    int m, first, pads;

    if (Lobby != LOBBY_GO && Lobby != LOBBY_OFF && Lobby != LOBBY_FAILED) {
        net_lobby_poll();
    }
    for (m = -1; m <= MACHINES; m++) {
        net_lobby_machine(m, text, sizeof(text));
        net_machine(m, &first, &pads);
    }
    net_lobby_machines();
    net_lobby_me();
    net_lobby_found(0, text, sizeof(text));
    net_lobby_network(0, text, sizeof(text), &name);
    net_lobby_code();
    for (m = -1; m <= PADS; m++) {
        net_pad(m);
        net_pad_sticks(m, sticks);
        net_pad_rumbles(m);
        net_own_pad(m);
    }
    net_players();
    net_machines();
    net_behind();
    net_vblank();
    /* a step, once there's one to play: a joiner waits for the host's record of it, the host for no one */
    if (Role == HOST || (Role == JOINER && Have > Next)) {
        net_step();
        net_step_done(fingerprint);
    }
}
#endif
