/* The PC build's entry point. On the PS1 the BIOS starts the exe at __SN_ENTRY_POINT and the startup
 * code calls machine_init (src/main/nascalls.c); here it will be the host's main (docs/PORT.md). */
#include <stdio.h>

int main(void) {
    puts("crash_bash_pc: the game links, but doesn't run yet (ROADMAP-B 6.2)");
    return 0;
}
