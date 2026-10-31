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
int StoreImage(RECT *r, uint32_t *p) { return 0; }
void gpu_present(void) {}
