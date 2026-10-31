/* The PS1 library calls the native renderer and mixer haven't replaced yet. Generated once; delete each as it goes. */
#include <libapi.h>
#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgte.h>
#include <libsnd.h>
#include <libspu.h>

uint32_t *ClearOTag(uint32_t *ot, int n) { return 0; }
uint32_t *ClearOTagR(uint32_t *ot, int n) { return 0; }
int DrawSync(int mode) { return 0; }
uint32_t *FntFlush(int id) { return 0; }
void FntLoad(int tx, int ty) {}
int FntOpen(int x, int y, int w, int h, int isbg, int n) { return 0; }
int FntPrint(void) { return 0; }
uint16_t GetClut(int x, int y) { return 0; }
uint16_t GetTPage(int tp, int abr, int x, int y) { return 0; }
uint16_t LoadClut2(uint32_t *clut, int x, int y) { return 0; }
int MoveImage2(RECT *r, int x, int y) { return 0; }
DISPENV *PutDispEnv(DISPENV *env) { return 0; }
int ResetGraph(int mode) { return 0; }
DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h) { return 0; }
void SetDispMask(int mask) {}
void SetDrawArea(DR_AREA *p, RECT *r) {}
void SetDrawMove(DR_MOVE *p, RECT *r, int x, int y) {}
int SetGraphDebug(int level) { return 0; }
void SpuGetAllKeysStatus(char *status) {}
int32_t SpuIsTransferCompleted(int32_t flag) { return 0; }
int32_t SpuReserveReverbWorkArea(int32_t on_off) { return 0; }
void SpuSetCommonAttr(SpuCommonAttr *attr) {}
void SsEnd(void) {}
void SsInit(void) {}
void SsQuit(void) {}
void SsSepStop(short seq, short sep) {}
void SsSeqCalledTbyT(void) {}
void SsSeqClose(short seq) {}
short SsSeqOpen(uint32_t *addr, short vab) { return 0; }
void SsSeqPause(short seq) {}
void SsSeqPlay(short seq, char mode, short count) {}
void SsSeqReplay(short seq) {}
void SsSeqSetAccelerando(short seq, int32_t tempo, int32_t ticks) {}
void SsSeqSetDecrescendo(short seq, short vol, int32_t ticks) {}
void SsSeqSetVol(short seq, short left, short right) {}
void SsSetMVol(short left, short right) {}
void SsSetMono(void) {}
void SsSetMute(char mode) {}
void SsSetStereo(void) {}
void SsSetTableSize(char *table, short s_max, short t_max) {}
int SsSetTickCallback(void (*cb)()) { return 0; }
void SsSetTickMode(int32_t mode) {}
void SsSetVoiceMask(uint32_t mask) {}
void SsStart(void) {}
void SsUtAllKeyOff(short mode) {}
short SsUtChangePitch(short voice, short vab, short prog, short old_note, short old_fine, short new_note, short new_fine) { return 0; }
void SsUtFlush(void) {}
short SsUtGetProgAtr(short vab, short prog, ProgAtr *atr) { return 0; }
short SsUtGetVagAtr(short vab, short prog, short tone, VagAtr *atr) { return 0; }
short SsUtKeyOffV(short voice) { return 0; }
short SsUtKeyOn(short vab, short prog, short tone, short note, short fine, short left, short right) { return -1; }
void SsUtReverbOn(void) {}
void SsUtSetReverbDepth(short left, short right) {}
short SsUtSetReverbType(short type) { return 0; }
short SsUtSetVVol(short voice, short left, short right) { return 0; }
void SsVabClose(short vab) {}
short SsVabOpenHead(unsigned char *addr, short vab) { return 0; }
short SsVabTransBody(unsigned char *addr, short vab) { return 0; }
int StoreImage(RECT *r, uint32_t *p) { return 0; }
void gpu_present(void) {}
void snd_vblank(void) {}
