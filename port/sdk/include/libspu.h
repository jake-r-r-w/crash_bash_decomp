/* The sound hardware functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBSPU_H
#define PC_LIBSPU_H

#define SPU_TRANSFER_PEEK 0
#define SPU_TRANSFER_WAIT 1
#define SPU_COMMON_MVOLL (1 << 0)
#define SPU_COMMON_MVOLR (1 << 1)
#define SPU_COMMON_CDVOLL (1 << 6)
#define SPU_COMMON_CDVOLR (1 << 7)
#define SPU_COMMON_CDMIX (1 << 9)

typedef struct { short left, right; } SpuVolume;
typedef struct { SpuVolume volume; long reverb, mix; } SpuExtAttr;
typedef struct {
    unsigned long mask;
    SpuVolume mvol, mvolmode, mvolx;
    SpuExtAttr cd, ext;
} SpuCommonAttr;

void SpuSetCommonAttr(SpuCommonAttr *attr);
void SpuGetAllKeysStatus(char *status);
long SpuIsTransferCompleted(long flag);
long SpuReserveReverbWorkArea(long on_off);

#endif
