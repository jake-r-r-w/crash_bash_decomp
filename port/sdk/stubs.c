/* Every PsyQ function the game calls, doing nothing yet: enough for the game to link (ROADMAP-B 6.1).
 * 6.2 replaces them with real ones. Each says so once on stderr when it's first called. */
#include <stdio.h>
#include <common.h>

#define STUB() \
    do { \
        static int said; \
        if (!said) { \
            said = 1; \
            fprintf(stderr, "stub: %s\n", __func__); \
        } \
    } while (0)

/* libgte's sine and cosine table (cstbl). */
short rcossin_tbl[4096 * 2];

/* The mod-chip check is PS1-only (src/ps1/modchip.c). */
void modchip_check_region(void) {
}
s32 modchip_step(s32 arg0) {
    return 0;
}

/* libapi.h */
unsigned long GetSp(void) {
    STUB();
    return 0;
}
unsigned long SetSp(unsigned long sp) {
    STUB();
    return 0;
}
long LoadExec(char *file, unsigned long sp, unsigned long size) {
    STUB();
    return 0;
}
long StopRCnt(unsigned long spec) {
    STUB();
    return 0;
}
void _96_init(void) {
    STUB();
}

/* libetc.h */
int ResetCallback(void) {
    STUB();
    return 0;
}
int StopCallback(void) {
    STUB();
    return 0;
}
long SetVideoMode(long mode) {
    STUB();
    return 0;
}
int VSync(int mode) {
    STUB();
    return 0;
}

/* libgte.h */
void InitGeom(void) {
    STUB();
}
int csqrt(int a) {
    STUB();
    return 0;
}
VECTOR *ApplyMatrixLV(MATRIX *m, VECTOR *in, VECTOR *out) {
    STUB();
    return 0;
}
void MatrixNormal_1(MATRIX *m, MATRIX *n) {
    STUB();
}
MATRIX *MulMatrix(MATRIX *m0, MATRIX *m1) {
    STUB();
    return 0;
}
MATRIX *MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *out) {
    STUB();
    return 0;
}
MATRIX *RotMatrix(SVECTOR *r, MATRIX *m) {
    STUB();
    return 0;
}
MATRIX *RotMatrixYXZ(SVECTOR *r, MATRIX *m) {
    STUB();
    return 0;
}
MATRIX *RotMatrixZYX(SVECTOR *r, MATRIX *m) {
    STUB();
    return 0;
}
MATRIX *ScaleMatrixL(MATRIX *m, VECTOR *v) {
    STUB();
    return 0;
}

/* libgpu.h */
int ResetGraph(int mode) {
    STUB();
    return 0;
}
int SetGraphDebug(int level) {
    STUB();
    return 0;
}
void SetDispMask(int mask) {
    STUB();
}
int DrawSync(int mode) {
    STUB();
    return 0;
}
DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h) {
    STUB();
    return 0;
}
DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h) {
    STUB();
    return 0;
}
DISPENV *PutDispEnv(DISPENV *env) {
    STUB();
    return 0;
}
DRAWENV *PutDrawEnv(DRAWENV *env) {
    STUB();
    return 0;
}
u_long *ClearOTag(u_long *ot, int n) {
    STUB();
    return 0;
}
u_long *ClearOTagR(u_long *ot, int n) {
    STUB();
    return 0;
}
void DrawOTag(u_long *ot) {
    STUB();
}
int ClearImage(RECT *r, u_char red, u_char green, u_char blue) {
    STUB();
    return 0;
}
int ClearImage2(RECT *r, u_char red, u_char green, u_char blue) {
    STUB();
    return 0;
}
int LoadImage(RECT *r, u_long *p) {
    STUB();
    return 0;
}
int StoreImage(RECT *r, u_long *p) {
    STUB();
    return 0;
}
int MoveImage2(RECT *r, int x, int y) {
    STUB();
    return 0;
}
u_short GetClut(int x, int y) {
    STUB();
    return 0;
}
u_short GetTPage(int tp, int abr, int x, int y) {
    STUB();
    return 0;
}
u_short LoadClut2(u_long *clut, int x, int y) {
    STUB();
    return 0;
}
void SetDrawArea(DR_AREA *p, RECT *r) {
    STUB();
}
void SetDrawEnv(DR_ENV *dr, DRAWENV *env) {
    STUB();
}
void SetDrawMove(DR_MOVE *p, RECT *r, int x, int y) {
    STUB();
}
void SetTile1(TILE_1 *p) {
    STUB();
}
void FntLoad(int tx, int ty) {
    STUB();
}
int FntOpen(int x, int y, int w, int h, int isbg, int n) {
    STUB();
    return 0;
}
int FntPrint() {
    STUB();
    return 0;
}
u_long *FntFlush(int id) {
    STUB();
    return 0;
}

/* libcd.h */
int CdInit(void) {
    STUB();
    return 0;
}
int CdStatus(void) {
    STUB();
    return 0;
}
int CdDiskReady(int mode) {
    STUB();
    return 0;
}
int CdSync(int mode, u_char *result) {
    STUB();
    return 0;
}
int CdControl(u_char com, u_char *param, u_char *result) {
    STUB();
    return 0;
}
int CdControlF(u_char com, u_char *param) {
    STUB();
    return 0;
}
CdlLOC *CdIntToPos(int i, CdlLOC *p) {
    STUB();
    return 0;
}
int CdPosToInt(CdlLOC *p) {
    STUB();
    return 0;
}
CdlFILE *CdSearchFile(CdlFILE *fp, char *name) {
    STUB();
    return 0;
}
int CdRead(int sectors, u_long *buf, int mode) {
    STUB();
    return 0;
}
int CdReadSync(int mode, u_char *result) {
    STUB();
    return 0;
}

/* libspu.h */
void SpuSetCommonAttr(SpuCommonAttr *attr) {
    STUB();
}
void SpuGetAllKeysStatus(char *status) {
    STUB();
}
long SpuIsTransferCompleted(long flag) {
    STUB();
    return 0;
}
long SpuReserveReverbWorkArea(long on_off) {
    STUB();
    return 0;
}

/* libsnd.h */
void SsInit(void) {
    STUB();
}
void SsStart(void) {
    STUB();
}
void SsEnd(void) {
    STUB();
}
void SsQuit(void) {
    STUB();
}
void SsSetTableSize(char *table, short s_max, short t_max) {
    STUB();
}
void SsSetTickMode(long mode) {
    STUB();
}
int SsSetTickCallback(void (*cb)()) {
    STUB();
    return 0;
}
void SsSeqCalledTbyT(void) {
    STUB();
}
void SsSetMVol(short left, short right) {
    STUB();
}
void SsSetMono(void) {
    STUB();
}
void SsSetStereo(void) {
    STUB();
}
void SsSetMute(char mode) {
    STUB();
}
void SsSetVoiceMask(unsigned long mask) {
    STUB();
}
short SsSeqOpen(unsigned long *addr, short vab) {
    STUB();
    return 0;
}
void SsSeqClose(short seq) {
    STUB();
}
void SsSeqPlay(short seq, char mode, short count) {
    STUB();
}
void SsSeqPause(short seq) {
    STUB();
}
void SsSeqReplay(short seq) {
    STUB();
}
void SsSeqSetVol(short seq, short left, short right) {
    STUB();
}
void SsSeqSetAccelerando(short seq, long tempo, long ticks) {
    STUB();
}
void SsSeqSetDecrescendo(short seq, short vol, long ticks) {
    STUB();
}
void SsSepStop(short seq, short sep) {
    STUB();
}
short SsVabOpenHead(unsigned char *addr, short vab) {
    STUB();
    return 0;
}
short SsVabTransBody(unsigned char *addr, short vab) {
    STUB();
    return 0;
}
void SsVabClose(short vab) {
    STUB();
}
short SsUtKeyOn(short vab, short prog, short tone, short note, short fine, short left, short right) {
    STUB();
    return 0;
}
short SsUtKeyOffV(short voice) {
    STUB();
    return 0;
}
void SsUtAllKeyOff(short mode) {
    STUB();
}
short SsUtChangePitch(short voice, short vab, short prog, short old_note, short old_fine, short new_note, short new_fine) {
    STUB();
    return 0;
}
short SsUtSetVVol(short voice, short left, short right) {
    STUB();
    return 0;
}
short SsUtGetProgAtr(short vab, short prog, ProgAtr *atr) {
    STUB();
    return 0;
}
short SsUtGetVagAtr(short vab, short prog, short tone, VagAtr *atr) {
    STUB();
    return 0;
}
void SsUtFlush(void) {
    STUB();
}
void SsUtReverbOn(void) {
    STUB();
}
short SsUtSetReverbType(short type) {
    STUB();
    return 0;
}
void SsUtSetReverbDepth(short left, short right) {
    STUB();
}

/* libpad.h */
void PadInitMtap(unsigned char *buf1, unsigned char *buf2) {
    STUB();
}
void PadStartCom(void) {
    STUB();
}
void PadStopCom(void) {
    STUB();
}
int PadGetState(int port) {
    STUB();
    return 0;
}
int PadSetMainMode(int port, int offs, int lock) {
    STUB();
    return 0;
}
void PadSetAct(int port, unsigned char *table, int len) {
    STUB();
}
int PadSetActAlign(int port, unsigned char *table) {
    STUB();
    return 0;
}

/* libmcrd.h */
void MemCardInit(long val) {
    STUB();
}
void MemCardStart(void) {
    STUB();
}
void MemCardStop(void) {
    STUB();
}
long MemCardExist(long chan) {
    STUB();
    return 0;
}
long MemCardAccept(long chan) {
    STUB();
    return 0;
}
long MemCardFormat(long chan) {
    STUB();
    return 0;
}
long MemCardSync(long mode, long *cmds, long *rslt) {
    STUB();
    return 0;
}
long MemCardCreateFile(long chan, char *file, long blocks) {
    STUB();
    return 0;
}
long MemCardGetDirentry(long chan, char *name, struct DIRENTRY *dir, long *files, long ofs, long max) {
    STUB();
    return 0;
}
long MemCardReadFile(long chan, char *file, unsigned long *adrs, long ofs, long bytes) {
    STUB();
    return 0;
}
long MemCardWriteFile(long chan, char *file, unsigned long *adrs, long ofs, long bytes) {
    STUB();
    return 0;
}

/* inline_c.h */
void pc_gte_SetGeomOffset(int x, int y) {
    STUB();
}
void pc_gte_SetGeomScreen(int h) {
    STUB();
}
void pc_gte_SetRotMatrix(const void *m) {
    STUB();
}
void pc_gte_SetTransMatrix(const void *m) {
    STUB();
}
void pc_gte_SetTransVector(const void *v) {
    STUB();
}
void pc_gte_ldv0(const void *v) {
    STUB();
}
void pc_gte_ldv0_words(int xy, int z) {
    STUB();
}
void pc_gte_ldv3c(const void *v) {
    STUB();
}
void pc_gte_ldsv(const void *v) {
    STUB();
}
void pc_gte_ldrgb(const void *c) {
    STUB();
}
void pc_gte_lddp(int p) {
    STUB();
}
void pc_gte_ldfcdir(int r, int g, int b) {
    STUB();
}
void pc_gte_rtps(void) {
    STUB();
}
void pc_gte_rtpt(void) {
    STUB();
}
void pc_gte_rt(void) {
    STUB();
}
void pc_gte_nclip(void) {
    STUB();
}
void pc_gte_avsz3(void) {
    STUB();
}
void pc_gte_dpcs(void) {
    STUB();
}
void pc_gte_intpl(void) {
    STUB();
}
void pc_gte_stsxy(void *p) {
    STUB();
}
void pc_gte_stsxy2(void *p) {
    STUB();
}
void pc_gte_stsxy3c(void *p) {
    STUB();
}
void pc_gte_stsxy3(void *p0, void *p1, void *p2) {
    STUB();
}
void pc_gte_stsz(void *p) {
    STUB();
}
void pc_gte_stsz3c(void *p) {
    STUB();
}
void pc_gte_stotz(void *p) {
    STUB();
}
void pc_gte_stopz(void *p) {
    STUB();
}
void pc_gte_stlvl(void *p) {
    STUB();
}
void pc_gte_strgb(void *p) {
    STUB();
}
void pc_gte_stsv(void *p) {
    STUB();
}
