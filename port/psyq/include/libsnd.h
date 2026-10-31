/* The sequencer and sound bank functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBSND_H
#define PC_LIBSND_H

#include <stdint.h>

typedef struct ProgAtr {
    unsigned char tones, mvol, prior, mode, mpan;
    char reserved0;
    short attr;
    uint32_t reserved1, reserved2;
} ProgAtr;

typedef struct VagAtr {
    unsigned char prior, mode, vol, pan, center, shift, min, max;
    unsigned char vibW, vibT, porW, porT, pbmin, pbmax, reserved1, reserved2;
    unsigned short adsr1, adsr2;
    short prog, vag;
    short reserved[4];
} VagAtr;

void SsInit(void);
void SsStart(void);
void SsEnd(void);
void SsQuit(void);
void SsSetTableSize(char *table, short s_max, short t_max);
void SsSetTickMode(int32_t mode);
void *SsSetTickCallback(void (*cb)());
void SsSeqCalledTbyT(void);
void SsSetMVol(short left, short right);
void SsSetMono(void);
void SsSetStereo(void);
void SsSetMute(char mode);
void SsSetVoiceMask(uint32_t mask);
short SsSeqOpen(uint32_t *addr, short vab);
void SsSeqClose(short seq);
void SsSeqPlay(short seq, char mode, short count);
void SsSeqPause(short seq);
void SsSeqReplay(short seq);
void SsSeqSetVol(short seq, short left, short right);
void SsSeqSetAccelerando(short seq, int32_t tempo, int32_t ticks);
void SsSeqSetDecrescendo(short seq, short vol, int32_t ticks);
void SsSepStop(short seq, short sep);
short SsVabOpenHead(unsigned char *addr, short vab);
short SsVabTransBody(unsigned char *addr, short vab);
void SsVabClose(short vab);
short SsUtKeyOn(short vab, short prog, short tone, short note, short fine, short left, short right);
short SsUtKeyOffV(short voice);
void SsUtAllKeyOff(short mode);
short SsUtChangePitch(short voice, short vab, short prog, short old_note, short old_fine,
                      short new_note, short new_fine);
short SsUtSetVVol(short voice, short left, short right);
short SsUtGetProgAtr(short vab, short prog, ProgAtr *atr);
short SsUtGetVagAtr(short vab, short prog, short tone, VagAtr *atr);
void SsUtFlush(void);
void SsUtReverbOn(void);
short SsUtSetReverbType(short type);
void SsUtSetReverbDepth(short left, short right);

#endif
