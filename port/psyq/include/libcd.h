/* The CD functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBCD_H
#define PC_LIBCD_H

#include <stdint.h>

#define CdlSetloc 0x02
#define CdlSeekL 0x15

typedef struct { uint8_t minute, second, sector, track; } CdlLOC;
typedef struct { CdlLOC pos; uint32_t size; char name[16]; } CdlFILE;

int CdInit(void);
int CdStatus(void);
int CdDiskReady(int mode);
int CdSync(int mode, uint8_t *result);
int CdControl(uint8_t com, uint8_t *param, uint8_t *result);
int CdControlF(uint8_t com, uint8_t *param);
CdlLOC *CdIntToPos(int i, CdlLOC *p);
int CdPosToInt(CdlLOC *p);
CdlFILE *CdSearchFile(CdlFILE *fp, char *name);
int CdRead(int sectors, uint32_t *buf, int mode);
int CdReadSync(int mode, uint8_t *result);

#endif
