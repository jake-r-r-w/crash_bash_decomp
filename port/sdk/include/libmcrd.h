/* The memory card functions the game uses: PsyQ's names (docs/PORT.md). */
#ifndef PC_LIBMCRD_H
#define PC_LIBMCRD_H

#include <libapi.h>

void MemCardInit(long val);
void MemCardStart(void);
void MemCardStop(void);
long MemCardExist(long chan);
long MemCardAccept(long chan);
long MemCardFormat(long chan);
long MemCardSync(long mode, long *cmds, long *rslt);
long MemCardCreateFile(long chan, char *file, long blocks);
long MemCardGetDirentry(long chan, char *name, struct DIRENTRY *dir, long *files, long ofs, long max);
long MemCardReadFile(long chan, char *file, unsigned long *adrs, long ofs, long bytes);
long MemCardWriteFile(long chan, char *file, unsigned long *adrs, long ofs, long bytes);

#endif
