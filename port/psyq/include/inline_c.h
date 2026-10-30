/* The GTE operations as C (docs/PORT.md). On the PS1 these are macros around cop2 instructions, and
 * the game passes them pointers of many types, so each macro here casts them for a plain function
 * in port/sdk. The _b forms differ only in the branch-delay timing, which C doesn't have. */
#ifndef PC_INLINE_C_H
#define PC_INLINE_C_H

void pc_gte_SetGeomOffset(int x, int y);
void pc_gte_SetGeomScreen(int h);
void pc_gte_SetRotMatrix(const void *m);
void pc_gte_SetTransMatrix(const void *m);
void pc_gte_SetTransVector(const void *v);
void pc_gte_ldv0(const void *v);
void pc_gte_ldv0_words(int xy, int z);
void pc_gte_ldv3c(const void *v);
void pc_gte_ldsv(const void *v);
void pc_gte_ldrgb(const void *c);
void pc_gte_lddp(int p);
void pc_gte_ldfcdir(int r, int g, int b);
void pc_gte_rtps(void);
void pc_gte_rtpt(void);
void pc_gte_rt(void);
void pc_gte_nclip(void);
void pc_gte_avsz3(void);
void pc_gte_dpcs(void);
void pc_gte_intpl(void);
void pc_gte_stsxy(void *p);
void pc_gte_stsxy2(void *p);
void pc_gte_stsxy3c(void *p);
void pc_gte_stsxy3(void *p0, void *p1, void *p2);
void pc_gte_stsz(void *p);
void pc_gte_stsz3c(void *p);
void pc_gte_stotz(void *p);
void pc_gte_stopz(void *p);
void pc_gte_stlvl(void *p);
void pc_gte_strgb(void *p);
void pc_gte_stsv(void *p);

#define gte_SetGeomOffset(x, y) pc_gte_SetGeomOffset((x), (y))
#define gte_SetGeomScreen(h) pc_gte_SetGeomScreen((h))
#define gte_SetRotMatrix(m) pc_gte_SetRotMatrix((const void *)(m))
#define gte_SetTransMatrix(m) pc_gte_SetTransMatrix((const void *)(m))
#define gte_SetTransVector(v) pc_gte_SetTransVector((const void *)(v))
#define gte_ldv0(v) pc_gte_ldv0((const void *)(v))
#define gte_ldv3c(v) pc_gte_ldv3c((const void *)(v))
#define gte_ldsv(v) pc_gte_ldsv((const void *)(v))
#define gte_ldrgb(c) pc_gte_ldrgb((const void *)(c))
#define gte_lddp(p) pc_gte_lddp((p))
#define gte_ldfcdir(r, g, b) pc_gte_ldfcdir((r), (g), (b))
#define gte_rtps() pc_gte_rtps()
#define gte_rtps_b() pc_gte_rtps()
#define gte_rtpt() pc_gte_rtpt()
#define gte_rt() pc_gte_rt()
#define gte_nclip_b() pc_gte_nclip()
#define gte_avsz3() pc_gte_avsz3()
#define gte_avsz3_b() pc_gte_avsz3()
#define gte_dpcs() pc_gte_dpcs()
#define gte_intpl() pc_gte_intpl()
#define gte_intpl_b() pc_gte_intpl()
#define gte_stsxy(p) pc_gte_stsxy((void *)(p))
#define gte_stsxy2(p) pc_gte_stsxy2((void *)(p))
#define gte_stsxy3c(p) pc_gte_stsxy3c((void *)(p))
#define gte_stsxy3_gt3(p) \
    pc_gte_stsxy3(&((POLY_GT3 *)(p))->x0, &((POLY_GT3 *)(p))->x1, &((POLY_GT3 *)(p))->x2)
#define gte_stsxy3_gt4(p) \
    pc_gte_stsxy3(&((POLY_GT4 *)(p))->x0, &((POLY_GT4 *)(p))->x1, &((POLY_GT4 *)(p))->x2)
#define gte_stsz(p) pc_gte_stsz((void *)(p))
#define gte_stsz3c(p) pc_gte_stsz3c((void *)(p))
#define gte_stotz(p) pc_gte_stotz((void *)(p))
#define gte_stopz(p) pc_gte_stopz((void *)(p))
#define gte_stlvl(p) pc_gte_stlvl((void *)(p))
#define gte_strgb(p) pc_gte_strgb((void *)(p))
#define gte_stsv(p) pc_gte_stsv((void *)(p))

#endif
