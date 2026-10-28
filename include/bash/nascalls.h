#ifndef BASH_NASCALLS_H
#define BASH_NASCALLS_H

/* Functions and data defined in src/main/nascalls.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/nascalls.c */

#include "common.h"
#include "bash/main.h"

void texture_update_scroll(texture_scroll *, obj_textureset *);
void textureset_upload(u8 *arg0);
void textureset_init(u8 *arg0, u8 *arg1);
void SetNasDefaults(void);
void SetEngineColBox(EXTengine *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6);
void nas_unused_a(void);
void nas_unused_b(void);
void nas_unused_c(void);
void nas_unused_d(void);
void InitSysFontModel(void);
void nas_unused_e(void);
void nas_unused_f(void);
void nas_unused_g(void);
void FreeNasHeap(void);
void AddCamera(NAS *nas);
void LinkObject(NAS *nas);
void AddToGroup(NAS **head, NAS *nas);
void RemoveFamily(NAS *arg0);
void BuildNASFreeList(void);
void FreeAllExtensions(NAS *nas);
void RemoveFromGroup(NAS *nas);
void RemoveCamera(NAS *nas);
void UnlinkObject(NAS *nas);
void RemoveSolid(NAS *nas);
void AppendExtension(NAS *arg0, NAS *arg1);
void InsertNasTag(NAS *arg0, NAS *arg1);
void MoveChildren(NAS *arg0, NAS *arg1);
void NullProc(NAS *arg0);
void DeleteNas_AllAST(s32 arg0);
void KillTriggerNas(NAS *nas);
s32 machine_init(void);
void ClearVram(void);
void ClearScreens(void);
void ResetScreen(void);
void cd_seek(s32 *arg0, s32 arg1);
void cd_read_wait(void);
void loader_pending_stub(void);
void cd_unused(void);
void xa_mute(void);
void pad_set_rumble_big(PadRumble *r, s32 time, s8 level);
void pad_set_rumble_small(PadRumble *r, s32 time);
void pad_input_reset(PadState *arg0);
void texture_unused_a(void);
void texture_unused_b(void);
void texture_unused_c(void);
void texture_release_loading_mem(obj_textureset *set);
void texture_unused_d(void);
void texture_unused_e(void);
void clut_release(u8 *arg0);
void texture_release_vram(u8 *arg0);
void textureset_clear(obj_textureset *ts);
void RunModules(ModuleEntry *p);
void FlipScreen(s32 arg0);
s32 cd_read(s32 *file, s32 b, u8 *c, s32 d);
extern FreeHdr *vram_take(u8 *);
/* Draws f at xy (640-wide units) as a POLY_FT4 in ot[otz]; color bit 31 makes
 * it semi-transparent, bits 29-30 its blend mode. Returns f's advance. */
s32 texture_draw(obj_texture *f, s32 xy, s32 otz, u32 color);
/* texture_draw as a POLY_GT4 with one colour per vertex (c0 also sets the
 * semi-transparency and blend mode). */
s32 texture_draw_quad(obj_texture *f, s32 xy, s32 otz, u32 c0, u32 c1, u32 c2, u32 c3);
/* Draws f at the 3D point pos as a POLY_GT4 in the OT at cdb, c0 on
 * the top vertices and c1 on the bottom ones; returns f's advance. */
s32 texture_draw_3d(obj_texture *f, SWORD_XYZ *pos, s32 otz, u32 c0, u32 c1);
void *AllocExtension(void *owner, s32 size, s32 id, void *tmpl);
void FreeExtension(void *ext);
s32 AddPlatformContour(EXTengine *eng, s32 y);
s16 DoAnimCommand(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim);
s16 DoAnimCommand2(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim);
EXTmotion *AddMotion(NAS *nas);
EXTengine *AddExtEngine(NAS *nas, s32 id);
s32 ApplyContourMoved(EXTengine *eng);
void *AddExt(NAS *owner, s32 size, s32 id);
void ClearOt(u8 *arg0);
s32 list_count(u8 * *arg0);
void *FindExtension(NAS *nas, s32 id);
NAS *CreateNAS(s32 x, s32 y, s32 z, u16 id, u16 flags);
s32 KillNAS(NAS *nas);
FlagRecord *FindCollisionBox(EXTengine *eng, u32 flag);
s16 RunNasAnim(NAS *nas);
void GotoLinkAnim(EXTanimator *anim, s32 n);
s32 textureset_mem_size(u8 *arg0);
s32 pad_read_mtap(u8 *src, u8 *dst, s32 n);
s32 CountObjects(void);
void ReAdoptChild(NAS *list, NAS *nas);
void texsrc_init(u8 *src, u8 *out, s32 len);
void *clut_alloc_large(void);
ClutPos *clut_alloc_small(void);
void AddSolid(NAS *nas, s32 bits);
void pads_init_mtap(void);
NAS *TriggerNas(NASTrigger *trig);
void pads_reset_rumble(void);
void KillEveryNas(void);
void ResetNasLists(void);
IdEntry *FindIdEntry(IdEntry *p, s32 id);
void xa_setvol(s32 vol);
void ApplyContour(EXTengine *eng);
void AllocExtensionCopy(void *owner, s32 size, s32 id, Words3 *tmpl);
void debug_font_init(void);
void debug_font_init2(void);
void TextFlashColour(u8 *a, u8 *b);
void ClearAllNasPool(void);
void KillAllNas(void);
s32 cd_read_poll(void);
s32 vram_slot_class(s32 a, s32 b, s32 c);
s32 calc_rgbcode(s32 r, s32 g, s32 b);
void texture_init_mem_textureset(obj_textureset *arg0);
void InitNasHeap(s32 arg0);
void ResetNasHeap(void);
void FetchDefaultColBox(EXTengine *eng, u32 flag);
s32 FollowUnderContour(EXTengine *eng);
s32 FollowContour(EXTengine *eng);
void RunTriggers(void);
s16 TextWidth(s32 x, u8 *s);
s32 SysGetLen(char *s);
Ext400 *AllocExt400(NAS *nas);
Ext1000 *AddObject(NAS *nas);
void ClearNasPool(void);
s32 AllocOtBuffers(s32 *arg0);  /* AllocPrimBuffers, then ScreenRect = NULL; returns the second end */
void AllocPrimBuffers(s32 *arg0);
void cd_init(void);
void pad_rumble_big(s32 pad, s32 arg1, s8 arg2);
void pad_rumble_small(s32 pad, s32 arg1);
FreeNode *vram_alloc_node(void);
void vram_free_node(BufNode *arg0);
void textureset_upload_cluts(u8 *arg0);
void texture_scroll_step(u8 *arg0, u8 *arg1);
void clut_upload(ClutUpload *arg0, s32 arg1);
void texanims_release(s32 *arg0, s32 arg1);
void pad_read_packet(PadPacket *p, PadState *in, s32 n);
s32 pad_rumble_send(PadRumble *r, s32 port);
void texture_update_anims(obj_textureset *arg0);
void texture_upload_list(TexUploadList *arg0, obj_textureset *arg1);
ScriptCursor *ScriptCursorFirst(NAS *nas, ScriptInit *init);
void ScriptCursorNext(ScriptCursor *c);
ScriptCursor *CreateScriptCursor(NAS *nas, s32 arg1, s32 arg2, ScriptInit *init);
NAS *CreateASTNas(s32 x, s32 y, s32 z, u16 id, s32 slot);
s16 TextAlignX(s16 x, u8 *s);
void SysPrintAt3d(SWORD_XYZ *arg0, char *s, u32 arg2, u32 arg3);
void InitTriggers(void);
s32 CheckSolidCollision(NAS *nas);
void InitDisplay(void);
void pads_read(void);
void textureset_release(obj_textureset *arg0);
void texture_init_entry(TexSrc2A84C *s, obj_texture *d, s32 n);
void DrawSysText(s16 arg0, s32 arg1, u8 *s, u32 arg3, u32 arg4, s32 arg5);
void texture_upload_anim(AnimTex *t, s32 base, AnimTexSet *set);
void SetupLevelSpecs(ArenaDef *arg0);
void InitSysFont(void);
void vram_release(FreeNode *n);
void vram_init(VramArea *areas, FreeNode *pool, s32 npool, s32 nbuf);

static inline s32 FadeColor(s32 r, s32 g, s32 b) {
    s32 t;
    s32 d;

    d = RenderState.modelDp;
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
        if (PadSetActAlign(port, PadActAlign) == 1) {
            r->dirty = 0;
        }
    }
    return 8;
}

#endif /* BASH_NASCALLS_H */
