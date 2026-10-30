/* u_char, u_short and u_long, which PsyQ's headers get from <sys/types.h>. Windows' has them elsewhere. */
#ifndef PC_TYPES_H
#define PC_TYPES_H

#ifdef _WIN32
#include <_bsd_types.h>
#else
#include <sys/types.h>
#endif

#endif
