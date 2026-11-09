/* On-screen buttons for a touch screen (touch.c): pad 1's buttons from fingers on the window, and their shapes, which the renderer
 * draws over the picture at the window's own size (gfx.c). They show from the start on Android, and anywhere once the screen is touched;
 * a controller or the keyboard hides them again, until the next touch. */
#ifndef TOUCH_H
#define TOUCH_H

#include <stdint.h>
#include <SDL3/SDL_events.h>

/* A finger down, moving or up, or any other event, as host_poll gets them. */
void touch_event(const SDL_Event *e);
/* Another way of playing was used (a key or a controller's button): the buttons go. */
void touch_other_input(void);
/* pad 1's buttons that fingers hold (PAD_ bits). */
int touch_buttons(void);

/* The shapes, as triangles in the window's pixels, w x h: first the buttons themselves, drawn at TOUCH_BODY_ALPHA, then their marks
 * at TOUCH_MARK_ALPHA. Returns how many corners there are in all, and *bodies how many of them are the buttons'; 0 when they're
 * hidden. rgb is 0xBBGGRR. */
#define TOUCH_BODY_ALPHA 0.4f
#define TOUCH_MARK_ALPHA 0.85f
typedef struct TouchVert {
    float x, y;
    uint32_t rgb;
} TouchVert;
int touch_shapes(int w, int h, TouchVert *out, int max, int *bodies);

#endif
