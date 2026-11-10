/* The router's port (portmap.c): a game hosted at home can only be joined from another home once the home's router passes a port on to
 * this machine, which the game asks it to do. */
#ifndef PORTMAP_H
#define PORTMAP_H

enum {
    PORTMAP_OFF,
    PORTMAP_WORKING,   /* asking: the answer can take a few seconds */
    PORTMAP_OPEN,      /* the port's open: portmap_address and portmap_port are what the world joins at */
    PORTMAP_NO_ROUTER, /* nothing answered: no router that takes such requests, or it's turned off there */
    PORTMAP_REFUSED,   /* a router answered, and wouldn't */
    PORTMAP_SHARED     /* the router's own address isn't one the world can reach: the provider shares one between homes, or
                          there's a second router between this one and the world */
};

/* Asks, on a thread of its own, for one of the eight ports from first to be passed on to this machine's port. pretend isn't 0 in a
   test: nothing is asked of any router, and the answer is that state after a moment. */
void portmap_open(int port, int first, int pretend);
int portmap_state(void);
const char *portmap_address(void);
int portmap_port(void);
/* The port's closed again, if it was opened. */
void portmap_close(void);

#endif
