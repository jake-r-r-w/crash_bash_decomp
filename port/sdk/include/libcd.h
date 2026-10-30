/* The CD functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBCD_H
#define PC_LIBCD_H

#include <pc_types.h>

#define CdlSetloc 0x02
#define CdlSeekL 0x15

typedef struct { u_char minute, second, sector, track; } CdlLOC;
typedef struct { CdlLOC pos; u_long size; char name[16]; } CdlFILE;

int CdInit(void);
int CdStatus(void);
int CdDiskReady(int mode);
int CdSync(int mode, u_char *result);
int CdControl(u_char com, u_char *param, u_char *result);
int CdControlF(u_char com, u_char *param);
CdlLOC *CdIntToPos(int i, CdlLOC *p);
int CdPosToInt(CdlLOC *p);
CdlFILE *CdSearchFile(CdlFILE *fp, char *name);
int CdRead(int sectors, u_long *buf, int mode);
int CdReadSync(int mode, u_char *result);

#endif
