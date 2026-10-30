/* The sound hardware functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBSPU_H
#define PC_LIBSPU_H

#include <stdint.h>

#define SPU_TRANSFER_PEEK 0
#define SPU_TRANSFER_WAIT 1
#define SPU_COMMON_MVOLL (1 << 0)
#define SPU_COMMON_MVOLR (1 << 1)
#define SPU_COMMON_CDVOLL (1 << 6)
#define SPU_COMMON_CDVOLR (1 << 7)
#define SPU_COMMON_CDMIX (1 << 9)

typedef struct { short left, right; } SpuVolume;
typedef struct { SpuVolume volume; int32_t reverb, mix; } SpuExtAttr;
typedef struct {
    uint32_t mask;
    SpuVolume mvol, mvolmode, mvolx;
    SpuExtAttr cd, ext;
} SpuCommonAttr;

void SpuSetCommonAttr(SpuCommonAttr *attr);
void SpuGetAllKeysStatus(char *status);
int32_t SpuIsTransferCompleted(int32_t flag);
int32_t SpuReserveReverbWorkArea(int32_t on_off);

#endif
