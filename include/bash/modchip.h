#ifndef BASH_MODCHIP_H
#define BASH_MODCHIP_H

/* Functions and data defined in src/main/modchip.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/modchip.c */

#include "common.h"
#include "bash/nas.h"

void modchip_check_region(void);
void modchip_cd_reset(s32 arg0);

/* CD command table, 4 bytes per command: command byte, parameter count, pad */
extern u8 ModchipCdCmds[];
extern volatile u8 *CdRegIndex; /* CD index/status register */
extern volatile u8 *CdCmdReg; /* CD command register */
extern volatile u8 *CdParamFifo; /* CD parameter FIFO */
extern volatile u8 *CdIntFlagReg; /* CD interrupt flag register */
extern u8 CdParams[];
extern u8 CdResult[]; /* CD response buffer */

extern s32 ModchipWaitCount;

s32 modchip_cd_wait(s32 arg0);
void modchip_show_warning(void);
s32 modchip_step(s32 arg0);

extern s32 ModchipVSync; /* VSync count at the last step */
extern s32 ModchipCdStatus;
typedef struct {
    u8 min;
    u8 sec;
} BcdTime;
extern volatile u8 CdTimeBcd; /* BCD minutes; BCD seconds at CdTimeBcd + 1 */

static inline u8 cd_to_bcd(u8 tens, u8 units) {
    return ((tens / 10) << 4) | (units % 10);
}
#endif /* BASH_MODCHIP_H */
