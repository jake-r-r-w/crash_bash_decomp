#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"

/* Names the PS1 build got from its link, as addresses with no C object behind them. */
s32 IntroScriptHandle;
obj_dbase *IntroDbase;
void *PapuStraySfx;  /* a word past the end of every slot overlay, where Papu's hero code keeps a sound handle */
