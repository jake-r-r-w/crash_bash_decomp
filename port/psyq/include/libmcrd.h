/* The memory card functions the game uses: PsyQ's names (docs/PORT.md). */
#ifndef PC_LIBMCRD_H
#define PC_LIBMCRD_H

#include <stdint.h>

#include <libapi.h>

void MemCardInit(int32_t val);
void MemCardStart(void);
void MemCardStop(void);
int32_t MemCardExist(int32_t chan);
int32_t MemCardAccept(int32_t chan);
int32_t MemCardFormat(int32_t chan);
int32_t MemCardSync(int32_t mode, int32_t *cmds, int32_t *rslt);
int32_t MemCardCreateFile(int32_t chan, char *file, int32_t blocks);
int32_t MemCardGetDirentry(int32_t chan, char *name, struct DIRENTRY *dir, int32_t *files, int32_t ofs, int32_t max);
int32_t MemCardReadFile(int32_t chan, char *file, uint32_t *adrs, int32_t ofs, int32_t bytes);
int32_t MemCardWriteFile(int32_t chan, char *file, uint32_t *adrs, int32_t ofs, int32_t bytes);

#endif
