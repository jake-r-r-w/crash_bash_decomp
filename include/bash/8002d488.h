#ifndef BASH_8002D488_H
#define BASH_8002D488_H

/* Functions and data defined in src/main/8002d488.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/8002d488.c */

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

typedef struct CdResult {
    u8 status;
    u8 unk1;
} CdResult;
extern s32 D_80067898;

s32 func_8002DE2C(s32 arg0);

#endif /* BASH_8002D488_H */
