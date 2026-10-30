/* A build without SDL: no window, no sound and no pads. It still runs the game, as fast as it can,
 * and main.c's --shot and --press options can see and drive it. */
#include <stdio.h>
#include "platform.h"

int host_init(void) {
    fprintf(stderr, "Built without SDL: no window (docs/PORT.md, Building).\n");
    return 0;
}
void host_present(const uint32_t *pixels, int w, int h) {
}
void host_wait_vblank(void) {
}
int host_poll(void) {
    return 1;
}
int host_pad(int port) {
    return port == 0 ? 0 : -1;
}
void host_audio(const int16_t *stereo, int frames) {
}
void host_quit(void) {
}
