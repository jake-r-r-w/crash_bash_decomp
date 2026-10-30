/* Callbacks, the video mode and vsync: PsyQ's names (docs/PORT.md). */
#ifndef PC_LIBETC_H
#define PC_LIBETC_H

int ResetCallback(void);
int StopCallback(void);
long SetVideoMode(long mode);
int VSync(int mode);

#endif
