#ifndef BASH_80022A3C_H
#define BASH_80022A3C_H

/* Functions and data defined in src/main/80022a3c.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/80022a3c.c */

#include "common.h"
#include "bash/main.h"

void texture_update_scroll(AnimScroll *, AnimOwner *);
void func_80029560(u8 *arg0);
void func_8002A5A4(u8 *arg0, u8 *arg1);
void SetNasDefaults(void);
void func_80025AE4(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_80025F8C(void);
void func_80025F94(void);
void func_80025F9C(void);
void func_80025FA4(void);
void func_80026308(void);
void func_80026418(void);
void func_80026420(void);
void func_80026428(void);
void func_800264B4(void);
void func_80026A04(u8 *arg0);
void func_80026AD8(u8 *arg0);
void func_80026B70(u8 * *arg0, u8 *arg1);
void RemoveFamily(NAS *arg0);
void func_80026D08(void);
void func_80026430(NAS *nas);
void func_80026AFC(NAS *nas);
void func_80026980(NAS *nas);
void func_80026A54(NAS *nas);
void func_800268BC(NAS *nas);
void func_80026C38(u8 *arg0, u8 *arg1);
void InsertNasTag(NAS *arg0, NAS *arg1);
void func_80026C94(u8 *arg0, u8 *arg1);
void NullProc(NAS *arg0);
void DeleteNas_AllAST(s32 arg0);
void func_800270B0(NAS *nas);
s32 func_8002718C(void);
void func_8002726C(void);
void func_8002759C(void);
void func_800276A4(void);
void func_800278BC(s32 *arg0, s32 arg1);
void func_80027908(void);
void func_80027A74(void);
void func_80027A94(void);
void func_80027A9C(void);
void func_80028184(u8 *arg0, s32 arg1, s8 arg2);
void func_800281A0(u8 *arg0, s32 arg1);
void func_800282D8(u8 *arg0);
void func_80028D38(void);
void func_8002A2DC(void);
void func_8002A2E4(void);
void texture_release_loading_mem(u8 *arg0);
void func_8002A4B8(void);
void func_8002A67C(void);
void func_8002A740(u8 *arg0);
void func_8002AA50(u8 *arg0);
void func_8002AA8C(u8 *arg0);
void func_800270F0(u8 *a);
void func_800272AC(s32 arg0);
s32 func_80027790(u8 *a, s32 b, u8 *c, s32 d);
extern FreeHdr *func_800282F8(u8 *);
/* Draws f at xy (640-wide units) as a POLY_FT4 in ot[otz]; color bit 31 makes
 * it semi-transparent, bits 29-30 its blend mode. Returns f's advance. */
s32 texture_draw(SpriteFrame *f, s32 xy, s32 otz, u32 color);
/* texture_draw as a POLY_GT4 with one colour per vertex (c0 also sets the
 * semi-transparency and blend mode). */
s32 func_8002992C(SpriteFrame *f, s32 xy, s32 otz, u32 c0, u32 c1, u32 c2, u32 c3);
/* Draws f at the 3D point pos as a POLY_GT4 in the OT at cdb, c0 on
 * the top vertices and c1 on the bottom ones; returns f's advance. */
s32 func_80029F9C(SpriteFrame *f, GlyphPos *pos, s32 otz, u32 c0, u32 c1);
void *AllocExtension(void *owner, s32 size, s32 id, void *tmpl);
void FreeExtension(void *ext);
s32 AddPlatformContour(EXTengine *eng, s32 y);
s16 func_800251A8(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim);
s16 func_80025534(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim);
EXTmotion *AddMotion(NAS *nas);
EXTengine *AddExtEngine(NAS *nas, s32 id);
s32 func_80025BBC(EXTengine *eng);
void *AddExt(void *owner, s32 size, s32 id);
void func_8002775C(u8 *arg0);
s32 func_80028BC8(u8 * *arg0);
void *FindExtension(NAS *nas, s32 id);
NAS *CreateNAS(s32 x, s32 y, s32 z, u16 id, u16 flags);
s32 KillNAS(NAS *nas);
FlagRecord *func_80025A48(EXTengine *eng, u32 flag);
s16 func_800250B0(NAS *nas);
void GotoLinkAnim(EXTanimator *anim, s16 n);
s32 func_8002A650(u8 *arg0);
s32 func_800280C0(u8 *src, u8 *dst, s32 n);
s32 func_80026A28(void);
void ReAdoptChild(NAS *list, NAS *nas);
void func_8002A830(u8 *src, u8 *out, s32 len);
void *func_80028A00(void);
ClutPos *func_80028420(void);
void func_8002694C(NAS *nas, s32 bits);
void func_80027F00(void);
NAS *TriggerNas(NASTrigger *trig);
void func_80028158(void);
void func_80026FCC(void);
void func_80026D9C(void);
IdEntry *func_800263D8(IdEntry *p, s32 id);
void xa_setvol(s16 vol);
void func_80025F44(EXTengine *eng);
void func_80026858(void *owner, s32 size, s32 id, Words3 *tmpl);
void func_80028BF0(void);
void func_80028C94(void);
void func_80024214(u8 *a, u8 *b);
void func_80026ECC(void);
void func_80026F48(void);
s32 func_80027944(void);
s32 func_80028994(s32 a, s32 b, s32 c);
s32 calc_rgbcode(s32 r, s32 g, s32 b);
void texture_init_mem_textureset(struct TexSet2926C *arg0);
void func_80023158(s32 arg0);
void func_8002321C(void);
void FetchDefaultColBox(EXTengine *eng, u32 flag);
s32 FollowUnderContour(EXTengine *eng);
s32 FollowContour(EXTengine *eng);
void func_8002617C(void);
s16 func_80026210(s32 x, u8 *s);
s32 SysGetLen(char *s);
Ext400 *func_800265DC(NAS *nas);
Ext1000 *AddObject(NAS *nas);
void func_80026E2C(void);
s32 func_800274FC(s32 *arg0);  /* func_800276C4, then D_8005B698 = NULL; returns the second end */
void func_800276C4(s32 *arg0);
void func_800279A4(void);
void func_80027F7C(s32 pad, s32 arg1, s8 arg2);
void func_80028018(s32 pad, s32 arg1);
FreeNode *func_80028A28(void);
void func_80028AE4(BufNode *arg0);
void func_8002A3D0(u8 *arg0);
void func_8002A684(u8 *arg0, u8 *arg1);
void func_8002A77C(ClutUpload *arg0, s32 arg1);
void func_8002A9AC(s32 *arg0, s32 arg1);
void func_80027AD4(PadPacket *p, PadInput *in, s32 n);
s32 func_800281C0(PadRumble *r, s32 port);
void func_80029100(AnimOwner *arg0);
void func_8002A4C0(TexUploadList *arg0, u8 *arg1);
ScriptCursor *func_80024E30(NAS *nas, ScriptInit *init);
void func_80024FA4(ScriptCursor *c);
ScriptCursor *func_80026048(NAS *nas, s32 arg1, s32 arg2, ScriptInit *init);
NAS *func_80022C3C(s32 x, s32 y, s32 z, u16 id, s32 slot);
s16 func_80024008(s16 x, u8 *s);
void SysPrintAt3d(GlyphPos *arg0, char *s, u32 arg2, u32 arg3);
void func_80024AD8(void);
s32 func_8002578C(NAS *nas);
void func_80027408(void);
void func_80027D90(void);
void func_80029790(u8 *arg0);
void func_8002A84C(TexSrc2A84C *s, TexDst2A84C *d, s32 n);
void func_800243A0(s16 arg0, s32 arg1, u8 *s, u32 arg3, u32 arg4, s32 arg5);
void func_80028D40(AnimTex *t, s32 base, AnimTexSet *set);
void func_800235BC(ArenaDef *arg0);
void func_80023A90(void);
void func_8002854C(FreeNode *n);
void func_80028774(VramArea *areas, FreeNode *pool, s32 npool, s32 nbuf);

static inline s32 FadeColor(s32 r, s32 g, s32 b) {
    s32 t;
    s32 d;

    d = D_800569AC;
    if (d != 0) {
        t = 0x1000 - d;
        r *= t;
        g *= t;
        b *= t;
        r >>= 12;
        r &= 0xFF;
        g >>= 12;
        g &= 0xFF;
        b >>= 12;
        b &= 0xFF;
    }
    return (r << 16) | (g << 8) | b;
}
static inline s32 FadeColorGs(GfxState *gs, s32 r, s32 g, s32 b) {
    s32 t;

    if (gs->modelDp != 0) {
        t = 0x1000 - gs->modelDp;
        r *= t;
        g *= t;
        b *= t;
        r >>= 12;
        r &= 0xFF;
        g >>= 12;
        g &= 0xFF;
        b >>= 12;
        b &= 0xFF;
    }
    return (r << 16) | (g << 8) | b;
}
static inline s32 PadRumble_update(PadRumble *r, s32 port) {
    if (r->reset != 0) {
        if (r->time0 != 0) {
            r->time0 = 1;
        }
        if (r->time1 != 0) {
            r->time1 = 1;
        }
    }
    r->reset = 0;
    if (r->time0 != 0) {
        r->time0 = r->time0 - 1;
        if ((r->time0 << 16) == 0) {
            r->act[0] = 0;
            r->dirty |= 1;
        }
    }
    if (r->time1 != 0) {
        r->time1 = r->time1 - 1;
        if ((r->time1 << 16) == 0) {
            r->act[1] = 0;
            r->dirty |= 2;
        }
    }
    if (PadGetState(port) != 6) {
        return 0;
    }
    if (r->dirty != 0) {
        PadSetAct(port, r->act, 2);
        if (PadSetActAlign(port, D_80063A84) == 1) {
            r->dirty = 0;
        }
    }
    return 8;
}

#endif /* BASH_80022A3C_H */
