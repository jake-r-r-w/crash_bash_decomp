/* The pad functions the game uses: PsyQ's names (docs/PORT.md). */
#ifndef PC_LIBPAD_H
#define PC_LIBPAD_H

#define TRUE 1
#define FALSE 0

void PadInitMtap(unsigned char *buf1, unsigned char *buf2);
void PadStartCom(void);
void PadStopCom(void);
int PadGetState(int port);
int PadSetMainMode(int port, int offs, int lock);
void PadSetAct(int port, unsigned char *table, int len);
int PadSetActAlign(int port, unsigned char *table);

#endif
