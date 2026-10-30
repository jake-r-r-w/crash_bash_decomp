/* Callbacks, the video mode and vsync: PsyQ's names (docs/PORT.md). */
#ifndef PC_LIBETC_H
#define PC_LIBETC_H

#include <stdint.h>

int ResetCallback(void);
int StopCallback(void);
int32_t SetVideoMode(int32_t mode);
int VSync(int mode);

#endif
