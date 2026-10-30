/* The kernel functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBAPI_H
#define PC_LIBAPI_H

#include <stdint.h>

#define RCntCNT0 0xF2000000
#define RCntCNT1 0xF2000001
#define RCntCNT2 0xF2000002
#define RCntCNT3 0xF2000003

struct DIRENTRY {
    char name[20];
    int32_t attr, size;
    struct DIRENTRY *next;
    int32_t head;
    char system[4];
};

uint32_t GetSp(void);
uint32_t SetSp(uint32_t sp);
int32_t LoadExec(char *file, uint32_t sp, uint32_t size);
int32_t StopRCnt(uint32_t spec);
void _96_init(void);

#endif
