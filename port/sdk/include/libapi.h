/* The kernel functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBAPI_H
#define PC_LIBAPI_H

#define RCntCNT0 0xF2000000
#define RCntCNT1 0xF2000001
#define RCntCNT2 0xF2000002
#define RCntCNT3 0xF2000003

struct DIRENTRY {
    char name[20];
    long attr, size;
    struct DIRENTRY *next;
    long head;
    char system[4];
};

unsigned long GetSp(void);
unsigned long SetSp(unsigned long sp);
long LoadExec(char *file, unsigned long sp, unsigned long size);
long StopRCnt(unsigned long spec);
void _96_init(void);

#endif
