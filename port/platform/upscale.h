/* Scaling a texture up on the player's machine (upscale.c). */
#ifndef UPSCALE_H
#define UPSCALE_H

#include <stdint.h>

/* w x h texels into w * factor x h * factor: factor is 2 or 4. */
void upscale_texels(const uint32_t *in, int w, int h, uint32_t *out, int factor);

#endif
