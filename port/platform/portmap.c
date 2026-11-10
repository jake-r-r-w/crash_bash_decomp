/* The router's port (docs/PORT.md, Online play). A home's router lets nothing in from outside that wasn't asked for from inside, so a
 * game hosted at home can't be joined from another home until the router is told to pass one port on to this machine. Most routers
 * take that request from a program on the network: by UPnP (nearly all home routers, where it's turned on), or by NAT-PMP (Apple's,
 * and some others). The game asks both ways, UPnP first, through miniupnpc and libnatpmp.
 *
 * It asks on a thread of its own, as finding the router takes a couple of seconds and the launcher is still drawing. Each asking has
 * its own record, so one that's given up on (the player backed out) finishes by itself, closes whatever it opened, and is freed.
 *
 * The port is asked for 2 hours, so a game that ends without closing it (a crash, the power) doesn't leave it open for good; a router
 * that only does ports for good gets that, and it's closed when the game's over. The router's own address is the one the others
 * join at. If that isn't an address the world can reach (it's a private one), the provider is sharing one address between homes, or
 * there's another router beyond this one, and no port opened here would help: that's said, not hidden.
 *
 * The port outside is one of eight in a row, the first that's free, so that a join code can say which in three bits (net.c). */
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"
#include "natpmp.h"
#include "getgateway.h"
#include "portmap.h"

#define PORTS 8
#define LEASE_SECONDS 7200
#define LEASE_TEXT "7200"
#define GIVEN_UP (-1) /* an asking's state once nobody's waiting for its answer */
#define UPNP_ONLY_FOR_GOOD 725 /* OnlyPermanentLeasesSupported */
#define UPNP_TAKEN 718         /* ConflictInMappingEntry: another machine has that port */

enum { BY_NOTHING, BY_UPNP, BY_NATPMP };

typedef struct Asking {
    SDL_AtomicInt state; /* PORTMAP_WORKING till its thread's done, or GIVEN_UP: whichever changes it from WORKING leaves the tidying to the other */
    int port, first, pretend;
    char address[64];
    int outside;
    int by;
    struct UPNPUrls urls;
    struct IGDdatas data;
} Asking;

static Asking *Now;

/* An address the world can't reach: private ranges, a provider's shared range, this machine. */
static int is_private(const char *address) {
    unsigned a = 0, b = 0, c = 0, d = 0;

    if (sscanf(address, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) {
        return 1;
    }
    return a == 10 || a == 127 || a == 0 || (a == 172 && b >= 16 && b < 32) || (a == 192 && b == 168) || (a == 100 && b >= 64 && b < 128)
        || (a == 169 && b == 254);
}

static int try_upnp(Asking *k) {
    struct UPNPDev *found;
    char lan[64] = "", wan[64] = "", outside[16], inside[16];
    int error = 0, status, state = PORTMAP_NO_ROUTER, i;

    found = upnpDiscover(2000, NULL, NULL, UPNP_LOCAL_PORT_ANY, 0, 2, &error);
    if (found == NULL) {
        return PORTMAP_NO_ROUTER;
    }
    status = UPNP_GetValidIGD(found, &k->urls, &k->data, lan, sizeof(lan), wan, sizeof(wan));
    if (status == UPNP_CONNECTED_IGD || status == UPNP_PRIVATEIP_IGD) {
        state = PORTMAP_REFUSED;
        if (status == UPNP_PRIVATEIP_IGD || is_private(wan)) {
            state = PORTMAP_SHARED;
        } else {
            SDL_snprintf(inside, sizeof(inside), "%d", k->port);
            for (i = 0; i < PORTS; i++) {
                int r;

                SDL_snprintf(outside, sizeof(outside), "%d", k->first + i);
                r = UPNP_AddPortMapping(k->urls.controlURL, k->data.first.servicetype, outside, inside, lan, "Crash Bash", "UDP", NULL, LEASE_TEXT);
                if (r == UPNP_ONLY_FOR_GOOD) {
                    r = UPNP_AddPortMapping(k->urls.controlURL, k->data.first.servicetype, outside, inside, lan, "Crash Bash", "UDP", NULL, "0");
                }
                if (r == UPNPCOMMAND_SUCCESS) {
                    SDL_strlcpy(k->address, wan, sizeof(k->address));
                    k->outside = k->first + i;
                    k->by = BY_UPNP;
                    state = PORTMAP_OPEN;
                    break;
                }
                if (r != UPNP_TAKEN) {
                    break;
                }
            }
        }
    }
    if (status != UPNP_NO_IGD && state != PORTMAP_OPEN) {
        FreeUPNPUrls(&k->urls);
    }
    freeUPNPDevlist(found);
    return state;
}

/* An answer to what was just sent, within a second and a half: the library would go on trying for a minute. */
static int natpmp_answer(natpmp_t *n, natpmpresp_t *answer) {
    Uint64 began = SDL_GetTicks();
    int r;

    do {
        struct timeval wait;
        fd_set set;

        FD_ZERO(&set);
        FD_SET(n->s, &set);
        if (getnatpmprequesttimeout(n, &wait) != 0 || wait.tv_sec > 0 || wait.tv_usec > 250000) {
            wait.tv_sec = 0;
            wait.tv_usec = 250000;
        }
        select((int)n->s + 1, &set, NULL, NULL, &wait);
        r = readnatpmpresponseorretry(n, answer);
    } while (r == NATPMP_TRYAGAIN && SDL_GetTicks() - began < 1500);
    return r == 0;
}

static int try_natpmp(Asking *k) {
    natpmpresp_t answer;
    natpmp_t n;
    char address[64] = "";
    int state = PORTMAP_NO_ROUTER, i;

    if (initnatpmp(&n, 0, 0) < 0) {
        return PORTMAP_NO_ROUTER;
    }
    if (sendpublicaddressrequest(&n) >= 0 && natpmp_answer(&n, &answer)) {
        inet_ntop(AF_INET, &answer.pnu.publicaddress.addr, address, sizeof(address));
        state = is_private(address) ? PORTMAP_SHARED : PORTMAP_REFUSED;
        for (i = 0; i < PORTS && state == PORTMAP_REFUSED; i++) {
            if (sendnewportmappingrequest(&n, NATPMP_PROTOCOL_UDP, (uint16_t)k->port, (uint16_t)(k->first + i), LEASE_SECONDS) < 0 || !natpmp_answer(&n, &answer)) {
                break;
            }
            if (answer.pnu.newportmapping.mappedpublicport == k->first + i) {
                SDL_strlcpy(k->address, address, sizeof(k->address));
                k->outside = k->first + i;
                k->by = BY_NATPMP;
                state = PORTMAP_OPEN;
            } else {
                /* it gave another port, which a join code couldn't say: that one's given back */
                if (sendnewportmappingrequest(&n, NATPMP_PROTOCOL_UDP, (uint16_t)k->port, answer.pnu.newportmapping.mappedpublicport, 0) >= 0) {
                    natpmp_answer(&n, &answer);
                }
            }
        }
    }
    closenatpmp(&n);
    return state;
}

static void shut(Asking *k) {
    char outside[16];

    if (k->by == BY_UPNP) {
        SDL_snprintf(outside, sizeof(outside), "%d", k->outside);
        UPNP_DeletePortMapping(k->urls.controlURL, k->data.first.servicetype, outside, "UDP", NULL);
        FreeUPNPUrls(&k->urls);
    } else if (k->by == BY_NATPMP) {
        natpmpresp_t answer;
        natpmp_t n;

        if (initnatpmp(&n, 0, 0) >= 0) {
            if (sendnewportmappingrequest(&n, NATPMP_PROTOCOL_UDP, (uint16_t)k->port, (uint16_t)k->outside, 0) >= 0) {
                natpmp_answer(&n, &answer);
            }
            closenatpmp(&n);
        }
    }
    k->by = BY_NOTHING;
}

static int SDLCALL ask(void *data) {
    Asking *k = data;
    int state;

    if (k->pretend != 0) {
        /* a test: no router's asked anything */
        SDL_Delay(300);
        state = k->pretend;
        SDL_strlcpy(k->address, state == PORTMAP_SHARED ? "100.64.1.2" : "127.0.0.1", sizeof(k->address));
        k->outside = k->port;
    } else {
        state = try_upnp(k);
        if (state != PORTMAP_OPEN && state != PORTMAP_SHARED) {
            int other = try_natpmp(k);

            state = other != PORTMAP_NO_ROUTER ? other : state;
        }
    }
    if (!SDL_CompareAndSwapAtomicInt(&k->state, PORTMAP_WORKING, state)) {
        /* given up on while it was asking: what it opened is closed again, and it's done with. (Otherwise the record is the
           launcher's from here, and isn't touched again.) */
        if (k->pretend == 0) {
            shut(k);
        }
        SDL_free(k);
    }
    return 0;
}

/* --check-router: the same asking as a hosted game's, a step at a time, saying what each found, for when the launcher says the router
   won't. It opens the port if it can, and closes it again at once. */
int portmap_check(int port, int first) {
    static const char *const status_names[] = { "none of them is a router", "a router that's connected", "a router whose own address is a private one",
                                                 "a router that isn't connected", "something that isn't a router" };
    struct UPNPDev *found, *d;
    struct UPNPUrls urls;
    struct IGDdatas data;
    char lan[64] = "", wan[64] = "", outside[16], inside[16];
    struct in_addr gateway;
    in_addr_t gw = 0;
    natpmpresp_t answer;
    natpmp_t n;
    int error = 0, status, r, opened = 0, count = 0;
#ifdef _WIN32
    WSADATA wsa;

    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    if (getdefaultgateway(&gw) == 0) {
        gateway.s_addr = gw;
        inet_ntop(AF_INET, &gateway, lan, sizeof(lan));
        printf("router: this machine's way out is through %s\n", lan);
    } else {
        printf("router: this machine has no way out that could be found\n");
    }
    lan[0] = '\0';
    printf("router: asking by UPnP who's a router (2 seconds)...\n");
    fflush(stdout);
    found = upnpDiscover(2000, NULL, NULL, UPNP_LOCAL_PORT_ANY, 0, 2, &error);
    for (d = found; d != NULL; d = d->pNext) {
        printf("router:   an answer from %s (%s)\n", d->descURL, d->st);
        count++;
    }
    if (found == NULL) {
        printf("router:   nothing answered (error %d): no router here takes UPnP requests, or it's turned off on it\n", error);
        /* anything at all that speaks UPnP on this network? If not even that, the network keeps its machines apart */
        found = upnpDiscoverAll(2000, NULL, NULL, UPNP_LOCAL_PORT_ANY, 0, 2, &error);
        for (d = found, count = 0; d != NULL; d = d->pNext) {
            count++;
        }
        printf("router:   %d other UPnP device%s on this network answered\n", count, count == 1 ? "" : "s");
        freeUPNPDevlist(found);
    } else {
        status = UPNP_GetValidIGD(found, &urls, &data, lan, sizeof(lan), wan, sizeof(wan));
        printf("router:   %s; this machine is %s to it, and its own address is %s\n", status >= 0 && status <= 4 ? status_names[status] : "?", lan[0] != '\0' ? lan : "?",
               wan[0] != '\0' ? wan : "unknown");
        if (status == UPNP_CONNECTED_IGD || status == UPNP_PRIVATEIP_IGD) {
            if (status == UPNP_PRIVATEIP_IGD || is_private(wan)) {
                printf("router:   that address isn't one the world can reach: the provider shares one between homes, or there's another router beyond this one\n");
            }
            SDL_snprintf(inside, sizeof(inside), "%d", port);
            SDL_snprintf(outside, sizeof(outside), "%d", first);
            r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype, outside, inside, lan, "Crash Bash", "UDP", NULL, LEASE_TEXT);
            printf("router:   asked for UDP port %s for two hours: %s (%d)\n", outside, r == UPNPCOMMAND_SUCCESS ? "opened" : strupnperror(r), r);
            if (r == UPNP_ONLY_FOR_GOOD) {
                r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype, outside, inside, lan, "Crash Bash", "UDP", NULL, "0");
                printf("router:   asked for it with no time limit: %s (%d)\n", r == UPNPCOMMAND_SUCCESS ? "opened" : strupnperror(r), r);
            }
            if (r == UPNPCOMMAND_SUCCESS) {
                opened = 1;
                r = UPNP_DeletePortMapping(urls.controlURL, data.first.servicetype, outside, "UDP", NULL);
                printf("router:   and closed it again: %s (%d)\n", r == UPNPCOMMAND_SUCCESS ? "closed" : strupnperror(r), r);
            }
        }
        if (status != UPNP_NO_IGD) {
            FreeUPNPUrls(&urls);
        }
        freeUPNPDevlist(found);
    }
    printf("router: asking by NAT-PMP (up to 2 seconds)...\n");
    fflush(stdout);
    if (initnatpmp(&n, 0, 0) < 0) {
        printf("router:   no way out to ask\n");
    } else {
        if (sendpublicaddressrequest(&n) >= 0 && natpmp_answer(&n, &answer)) {
            inet_ntop(AF_INET, &answer.pnu.publicaddress.addr, wan, sizeof(wan));
            printf("router:   it answered: its own address is %s%s\n", wan, is_private(wan) ? ", which isn't one the world can reach" : "");
        } else {
            printf("router:   nothing answered: it doesn't take NAT-PMP requests\n");
        }
        closenatpmp(&n);
    }
    printf(opened ? "router: it opens ports when asked, so hosting should work\n" : "router: no port could be opened\n");
    return opened;
}

void portmap_open(int port, int first, int pretend) {
    SDL_Thread *thread;

    portmap_close();
    Now = SDL_calloc(1, sizeof(*Now));
    if (Now == NULL) {
        return;
    }
    Now->port = port;
    Now->first = first;
    Now->pretend = pretend;
    SDL_SetAtomicInt(&Now->state, PORTMAP_WORKING);
    thread = SDL_CreateThread(ask, "router", Now);
    if (thread == NULL) {
        SDL_free(Now);
        Now = NULL;
        return;
    }
    SDL_DetachThread(thread);
}

int portmap_state(void) {
    return Now != NULL ? SDL_GetAtomicInt(&Now->state) : PORTMAP_OFF;
}

const char *portmap_address(void) {
    return Now != NULL && portmap_state() != PORTMAP_WORKING ? Now->address : "";
}

int portmap_port(void) {
    return Now != NULL && portmap_state() == PORTMAP_OPEN ? Now->outside : 0;
}

void portmap_close(void) {
    Asking *k = Now;

    if (k == NULL) {
        return;
    }
    Now = NULL;
    /* still asking: its thread finishes by itself, and closes what it opens. Otherwise the answer's in, and it's closed here */
    if (!SDL_CompareAndSwapAtomicInt(&k->state, PORTMAP_WORKING, GIVEN_UP)) {
        if (k->pretend == 0) {
            shut(k);
        }
        SDL_free(k);
    }
}
