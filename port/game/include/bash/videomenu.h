#ifndef BASH_VIDEOMENU_H
#define BASH_VIDEOMENU_H

/* The video options, which every menu offering them shares (main/videomenu.c). */

#include "common.h"

#define NUM_VIDEO_OPTIONS 10

s32 video_option_count(void);
const char *video_option_label(s32 i);
const char *video_option_help(s32 i);
void video_option_value(s32 i, char *text, s32 size);
/* dir: 1 or -1, along its values. It takes effect, and is kept. */
void video_option_change(s32 i, s32 dir);

#endif /* BASH_VIDEOMENU_H */
