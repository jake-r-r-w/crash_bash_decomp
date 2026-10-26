#ifndef BASH_MODCHIP_H
#define BASH_MODCHIP_H

/* Functions and data defined in src/main/modchip.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/modchip.c */

#include "common.h"
#include "bash/nas.h"

void func_8002D488(void);
void func_8002E000(s32 arg0);

/* CD command table, 4 bytes per command: command byte, parameter count, pad */
extern u8 D_8006783C[];
extern volatile u8 *D_80067874; /* CD index/status register */
extern volatile u8 *D_80067878; /* CD command register */
extern volatile u8 *D_8006787C; /* CD parameter FIFO */
extern volatile u8 *D_80067880; /* CD interrupt flag register */
extern u8 D_80067884[];
extern u8 D_8006788C[]; /* CD response buffer */

extern s32 D_80067898;

s32 func_8002DE2C(s32 arg0);
void func_8002E0F0(void);
s32 func_8002D4F4(s32 arg0);

extern s32 D_8006EA18; /* VSync count at the last step */
extern s32 D_8006EA1C;
typedef struct {
    u8 min;
    u8 sec;
} BcdTime;
extern volatile u8 D_8006EA20; /* BCD minutes; BCD seconds at D_8006EA20 + 1 */

static inline u8 cd_to_bcd(u8 tens, u8 units) {
    return ((tens / 10) << 4) | (units % 10);
}
#endif /* BASH_MODCHIP_H */
