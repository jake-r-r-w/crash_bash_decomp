/* Online play (net.c; docs/PORT.md, Online play): one machine hosts, the others join it, and every machine runs the whole game from
 * everyone's presses. */
#ifndef NET_H
#define NET_H

#include <stddef.h>
#include <stdint.h>

/* The port a game's hosted on from the launcher, and looked for on. */
#define NET_PORT 47000

typedef struct NetOptions {
    const char *host;   /* --host [ADDRESS:]PORT: host a game there, or NULL */
    const char *join;   /* --join ADDRESS:PORT: join the game hosted there, or NULL */
    const char *replay; /* --replay FILE: play a recorded game's presses again, with nobody else, or NULL */
    const char *record; /* --net-record FILE: keep the game's presses, step by step, or NULL */
    int joiners;        /* how many machines the host waits for before it starts */
    int delay;          /* how many steps late a press is used, or 0 for what the ping asks */
    int lag, loss;      /* pretend, for tests: milliseconds each packet takes, and how many in a hundred are lost */
    int patience;       /* how long the host waits for a machine that's gone quiet before playing on without it, ms (0: 5 seconds) */
    int differ_at;      /* a test: this machine's fingerprints are wrong from this step on, or -1 */
    int vanish_at;      /* a test: at this step the program ends without a word to anyone, as a machine switched off, or -1 */
    int stall_at, stall_ms; /* a test: at this step the machine does nothing for that long, as a window being dragged, or -1 */
    int local;          /* a test: the launcher's games are this PC's alone: hosted at 127.0.0.1, and looked for there (--online-local) */
    int port;           /* the launcher's games' port, if not NET_PORT (--online-port) */
    int pretend_router; /* a test: no router's asked for a port, and the answer's this (PORTMAP_, portmap.h; --online-pretend-router) */
    const char *pretend_network; /* a test: one more network adapter on this machine, as "WHAT IT'S CALLED=ADDRESS" (--online-pretend-network) */
} NetOptions;

/* What the command line asked of online play, for a game from there or from the launcher: before either. */
void net_configure(const NetOptions *options);
/* From the command line (--host, --join, --replay), before the game starts: hosts and waits for the others, or joins and waits for
   the host's start, or opens the recording. 0: it couldn't, and has said why; -1: the window was closed while it waited. */
int net_start(void);

/* The lobby, for the launcher's ONLINE screens: hosting or joining a frame at a time. net_lobby_poll, once a frame, says where it's
   got to. */
enum {
    LOBBY_OFF,
    LOBBY_FINDING,  /* looking for games on this network (net_lobby_found) */
    LOBBY_OPEN,     /* hosting: others can join (net_lobby_machines), till net_lobby_start */
    LOBBY_STARTING, /* hosting: the others are being sent the start */
    LOBBY_ASKING,   /* joining: no answer yet */
    LOBBY_IN,       /* joined, and waiting for the host to start */
    LOBBY_GO,       /* everyone's ready: the game's to start now, from its beginning (platform_online_start) */
    LOBBY_FAILED    /* it couldn't: net_lobby_problem says why */
};
int net_lobby_host(const char *where); /* NULL: the usual port. 0: it couldn't */
void net_lobby_start(void);
void net_lobby_find(void);
int net_lobby_found(int i, char *text, size_t room); /* the i'th game found: its address, and how many machines are in it (0: no i'th) */
int net_lobby_join(const char *where); /* an address, with a port or the usual one. 0: that's no address */
int net_lobby_poll(void);
/* Who's in the lobby, this machine too: as the host has them, or as it last told a machine that's joined (0 till it has). A machine's
   text is its address and ping for the host (nothing for its own), and for one that's joined the host's address and nothing for the
   others, whose addresses are the host's to know; what comes back is how many players it brings. */
int net_lobby_machines(void);
int net_lobby_machine(int m, char *text, size_t room);
int net_lobby_me(void); /* which of them this machine is: 0 when hosting */
const char *net_lobby_address(void); /* this machine's address on its main network, for the others to join */
/* Each address this machine's game can be joined at: the main network's first, with a name of "", then one for each virtual network
   it's on (Hamachi, Radmin VPN, ZeroTier, Tailscale), with that tool's name. 0: there's no i'th. */
int net_lobby_network(int i, char *address, size_t room, const char **name);
/* Every network adapter this machine has, and what each is taken for: said by --check-router. */
void net_say_networks(void);
/* Hosting: whether the router has let the world in (PORTMAP_, portmap.h), and the code the others join with once it has ("" till then). */
int net_lobby_router(void);
const char *net_lobby_code(void);
/* A join code: an address and one of the eight ports from the usual one, as eight letters and digits with a check, written
   XXXX-XXXX. net_code_read gives "ADDRESS:PORT" for one typed in, however it's spaced or cased, or 0 if it isn't one. And a check
   of both against each other, for the build's tests (--check-codes): 0 if they're wrong. */
int net_code_make(const char *address, int port, char *text, size_t room);
int net_code_read(const char *text, char *where, size_t room);
int net_code_check(void);
const char *net_lobby_problem(void);
void net_lobby_leave(void); /* backing out: the others are told */
void net_lobby_reset(void); /* a game that's over, put away: another can be hosted or joined */
void net_record(void);      /* keeps the game's presses, if that was asked (--net-record): as it starts */

/* Whether the pads are an online game's (or a recording's), not this machine's own. */
int net_on(void);
/* How many players the game has, on all its machines together: 0 when it isn't an online one. A recording says how many its game had. */
int net_players(void);
/* Before the game reads the pads, once a step: this machine's own go out, and it waits for everyone's for this step. */
void net_step(void);
/* The pads of the step being played, as platform_pad and its like give them. */
int net_pad(int pad);
int net_pad_sticks(int pad, unsigned char sticks[4]);
int net_pad_rumbles(int pad);
/* Which of this machine's own pads a player's is, or -1 if it's another machine's. */
int net_own_pad(int pad);
/* The machines of the game being played, for showing who's in it (game/main/online.c): how many (0 when it isn't an online game),
   which is this one, and each one's players (the first's number from 0, and how many) with how it's doing: playing; slow, which is
   that the game has had to wait for it in the last few seconds; not answering, so played on without; or gone for good. The host
   knows, and tells the others with every step; a joiner knows for itself when the host's packets have stopped coming. */
enum { NET_PLAYING, NET_SLOW, NET_QUIET, NET_LEFT };
int net_machines(void);
int net_me(void);
int net_machine(int m, int *first, int *pads);
/* After a step, with the game's fingerprint (game/main/fingerprint.c): the machines compare them once a second. */
void net_step_done(uint32_t fingerprint);
/* Once a vblank: what's arrived is taken in, so nobody waits on this machine longer than that. */
void net_vblank(void);
/* Whether this machine is behind the host, and should play on without waiting for its clock. */
int net_behind(void);
/* As the game or the program ends: the others are told, and a test run says how it went. */
void net_end(void);

#ifdef NET_FUZZ
/* For the fuzzer (tests/netfuzz.c): a machine in one of its states, with no socket; a packet taken as if from one of the game's
   machines (0 the host, then each that's joined, by number) or from a stranger (NET_FUZZ_WHO - 1); and what it does each frame and
   each step, with that step's fingerprint. A machine in one state is told by its packets into the next, as a real one is. */
enum { NET_FUZZ_FINDING, NET_FUZZ_HOST_OPEN, NET_FUZZ_HOST_STARTING, NET_FUZZ_HOST_PLAYING, NET_FUZZ_JOINER_ASKING, NET_FUZZ_JOINER_IN,
       NET_FUZZ_JOINER_PLAYING, NET_FUZZ_STATES };
#define NET_FUZZ_WHO 5
#define NET_FUZZ_SESSION 0x5E551011u
#define NET_FUZZ_NONCE 0x0D0CE123u
struct sockaddr_in;
void net_fuzz_begin(int state, int machines);
void net_fuzz_take(const uint8_t *bytes, size_t size, int who);
void net_fuzz_tick(uint32_t fingerprint);
const struct sockaddr_in *net_fuzz_from(int who);
uint32_t net_fuzz_build(void);
#endif

#endif
