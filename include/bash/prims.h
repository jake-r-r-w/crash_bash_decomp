#ifndef BASH_PRIMS_H
#define BASH_PRIMS_H

/* Functions and data defined in src/main/prims.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/prims.c */

#include "common.h"
#include "bash/main.h"

void primpool_release_all(void);
void primset_invalidate(u8 * *arg0);
s32 *primlist_pop(PrimList *l);
s32 *primlist_pop_b(PrimList *l);
void faces_write_uvs_colors(s32 arg0, s32 arg1);
void primnodes_free(s32 *arg0);
void prims_empty(void);
void primlist_clear(PrimList *l);
void morph_light_colors(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc);
void morph_blend_verts(s32 p, MorphModel *model, u16 *ia, u16 *ib, s16 (*verts)[3]);
void gfx_setup_xform(void *arg0, void *arg1, void *arg2, u8 *arg3, GteSetup *out);
void gfx_load_xform(void *arg0, void *arg1, void *arg2);
void draw_object_model(s32 a, u8 *set, s32 c, void *d);
void eng_render_subs(u8 *arg0);
void kill_sprim_sub(subengine *arg0);
void draw_model_quad(ModelObj *obj, void *arg1, s32 flags);
void prim_colour_poly(RelTable *face, POLY_GT4 *poly, VECTOR *arg2, s32 mode);
void prim_create_entity_single(u8 *face, POLY_GT4 *poly);
void calc_rgbcode_packed(u32 arg0);
void sub_release_none(void);
void eng_release_subs_sfx(EXTengine *eng);
void eng_release_subs(EXTengine *eng);
void subs_release(subengine *first, subengine *last);
void kill_subs(subengine *first);
s32 eng_add_subs(EXTengine *eng, s32 n);
s32 eng_add_subs_n(EXTengine *eng, s32 n);
void subpool_free(void);
void map_update(s32 *arg0);
void mapscene_release(u8 *arg0);
void map_loaded_textureset(u8 *arg0, u8 * *arg1);
void mapscene_load(u8 *arg0, s32 arg1);
void set_fade(fade_info *fade, s32 speed, s32 delay);
void process_control_back(process_control_info *pc);
void process_interrupt(process_control_info *pc, process_info *process);
void process_control_goto(process_control_info *pc, process_info *process);
void process_control_render(process_control_info *pc);
void process_control_init(process_control_info *pc, process_info *process);
void prims_empty_and_compact(void);
void eng_kill(EXTengine *eng);
void prims_update(void);
void draw_panel_rect(void *arg0, u32 hidden);
s32 draw_gouraud_quad(void *arg0, u32 flags);
void *model_prims_begin(void *arg0, s32 arg1);
s32 bbox_visible(void *arg0);
void draw_verts(POLY_GT3 *poly, SVECTOR *v, u8 *runs);
void prims_init(s32 *cfg);
void map_loaded_geo();
void subs_init_array(EXTengine *eng, s32 n, subengine *sub);
void morph_light_colors(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc);
void project_quad(SVECTOR *v, POLY_GT4 *poly);
u32 draw_nas_model_ret(NAS *nas, u8 *arg1, u32 flags);
void draw_nas_model(NAS *nas, u8 *arg1);
void *draw_morph_model(MorphModel *model, void *arg1, s32 arg2);
void process_quit(process_control_info *pc);
void process_init(process_control_info *pc);
s32 fade_update(fade_info *fade);
subengine *mapscene_find_sub(s32 i, s32 val);
void *new_sprim(void);
void release_sprim(void *data);
void primpool_free(void);
void eng_render(EXTengine *eng);
subengine *sub_nth(s32 n, subengine *sub);
void draw_init(void);
s32 rel_ptr(s32 *arg0);
void draw_model_set_entry(s32 arg0, ModelSet *set, s32 arg2);
void draw_map_entity(u16 arg0, u8 *arg1, s32 arg2);
void draw_text_nas(TextNAS *t, s32 arg1, s32 arg2);
void primpool_age(void);
s32 *primnode_alloc(s32 arg0, s32 *arg1, s32 arg2, s32 arg3);
void sub_use_quad_model(u8 *arg0);
void posdst_flash(PosDst *arg0, s32 arg1, s32 arg2);
void posdst_flash_at(PosDst *arg0, s32 arg1);
s32 sub_alloc_prims(void *arg0, s32 arg1);
void subpool_init(s32 n);
void draw_map_sub(void *arg0, void *arg1);
void mapscene_init(s32 *arg0, s32 *arg1);
KeyEntry *key_find(KeyBlock *arg0, s32 arg1);
s32 path_find(u32 arg0);
void primpool_init(s32 count);
s32 *primset_alloc(s32 * *arg0, s32 arg1);
void primlist_reset(PrimList *arg0, s32 arg1);
void primlist_release(PrimList *arg0, s32 arg1);
void morph_copy_verts(u8 *arg0, u16 *src, u8 *tab, u16 *dst);
void *draw_model_entry(ModelEntry *e, s32 arg1);
s32 *path_get(u32 arg0);
void draw_flat_rect(u16 *arg0, s32 arg1, u32 arg2);
void draw_map_object(void *sub, void *arg1, s32 flags, s32 arg3);
MATRIX *gfx_object_matrix(GfxXform *obj);
s32 subs_alloc_list(subengine * *out_first, subengine * *last, s32 n);
u8 *tga_convert(u8 *arg0, s32 w, s32 h);
void prims_empty_back(void);
void faces_write_colors_dq(FaceRuns *faces, FacePrim *prim, s32 arg2, s32 mode);
void faces_write_colors(FaceRuns *faces, FacePrim *prim);
void draw_object(u8 *arg0, u8 *arg1, s32 arg2);
void draw_panel_frame(u16 *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4);
void mapscene_place_subs(u8 *arg0);
FacePrim *face_prim_alloc(s32 arg0, PrimList *list, PrimList *pool, FaceRuns *faces, s32 arg4);
void gfx_set_viewport(GfxViewport *vp, GfxCamera *cam);
void draw_map_scene(GfxScene *scene);
void mapenv_place_subs(MapEnvHead *env);
s32 tri_visible(SVECTOR *v);
void draw_nas_model_flags(NAS *nas, u8 *arg1, u32 flags);
KeyEntry *key_interp_pos(KeyBlock *arg0, s32 arg1, SLONG_XYZ *out);
void process_control_update(process_control_info *pc, fade_info *fade);







/* Adds v to the translation GfxState.unk88 points at (gfx_setup_xform). */
static inline void GfxAddTrans(VECTOR *v) {
    ((VECTOR *)RenderState.unk88)->vx += v->vx;
    ((VECTOR *)RenderState.unk88)->vy += v->vy;
    ((VECTOR *)RenderState.unk88)->vz += v->vz;
}

/* Copies a scene's background into the current viewport, faded by modelDp
 * (draw_map_scene). */
static inline void GfxSetSceneBg(GfxState *gs, GfxScene *scene) {
    GfxViewport *vp;

    vp = gs->viewport;
    vp->isbg = scene->viewport->isbg;
    if (gs->modelDp != 0) {
        vp->isbg = 1;
    }
    if (vp->isbg != 0) {
        if (gs->modelDp != 0) {
            vp->r = scene->viewport->r * (0x1000 - gs->modelDp) / 4096;
            vp->g = scene->viewport->g * (0x1000 - gs->modelDp) / 4096;
            vp->b = scene->viewport->b * (0x1000 - gs->modelDp) / 4096;
        } else {
            *(GfxRgb8 *)&vp->r = *(GfxRgb8 *)&scene->viewport->r;
        }
    }
}

/* Sets the clip area c (640-wide units) for cam. */
static inline void GfxSetClip(GfxState *gs, RECT *c, GfxCamera *cam) {
    RECT clip;

    clip.x = c->x * ScreenRect->w / 640;
    clip.y = c->y / 2;
    clip.w = (c->w - c->x) * ScreenRect->w / 640;
    clip.h = (c->h - c->y) / 2;
    gs->clipX1 = c->w * ScreenRect->w / 640;
    gs->clipY1 = c->h / 2;
    gs->cam = cam;
    cam_build_view((u8 *)cam, &clip);
}

/* draw_object_model: draw model entry e with vertices v. */
static inline void ModelEntry_Draw(u8 *e, s32 c, SVECTOR *v) {
    void *p = model_prims_begin(e, c);

    if (p != NULL) {
        draw_verts(p, v, e + (*(s32 *)(e + 0x14) + 0x14));
    }
}

/* Takes the node at the cursor fC of a list (primlist_pop_b's code). */
static inline s32 *PrimList_Take(PrimList *l) {
    s32 *n;
    s32 **tail;

    if (l == NULL || (n = l->fC) == NULL) {
        return NULL;
    }
    tail = l->f10;
    l->fC = (s32 *)*n;
    if (tail != NULL) {
        *tail = (s32 *)*n;
        l->fC = (s32 *)*n;
        if (*n == 0) {
            l->f8 = tail;
        }
    } else {
        if (l->f8 == (s32 **)n) {
            l->f8 = NULL;
        }
        l->f4 = *n;
        l->fC = (s32 *)*n;
        *n = 0;
    }
    return n;
}


/* model_prims_begin: the next free primitive slot of list (refilled from pool). */
static inline FacePrim *PrimSet_NextPrim(PrimSet *set, s32 pairOff, s32 which, PrimList *pool,
                                         void *faces, s32 flag) {
    s32 *n;
    s32 *next;
    FacePrim *ret;
    PrimList *list =(PrimList *)((u8 *)set + pairOff) + which;

    if (set->mode == 1) {
        n = list->fC;
        if (n != NULL) {
            next = (s32 *)*n;
            list->f10 = (s32 **)n;
            list->fC = next;
            set = (PrimSet *)n[1];
        } else {
            set = (PrimSet *)face_prim_alloc((s32)set, list, pool, (FaceRuns *)faces, flag);
        }
    } else {
        n = list->fC;
        if (n != NULL) {
            list->fC = (s32 *)*n;
            list->f10 = (s32 **)n;
            set = (PrimSet *)(n + 1);
        } else {
            set = (PrimSet *)primnode_alloc((s32)set, (s32 *)list, (s32)pool, (s32)faces);
        }
    }
    return (FacePrim *)set;
}

/* mapenv_place_subs: reuse the map's subengine array or allocate it. */
static inline subengine *MapEnv_GetSubs(MapEnvHead *env) {
    subengine *sub;
    subengine *t;

    t = env->subs;
    if (t != NULL) {
        sub = t;
        kill_subs(sub);
    } else {
        sub = (subengine *)mal_alloc(env->num_placements * 0xA8, &MainHeap);
    }
    return sub;
}

#include "bash/scripts.h"

#endif /* BASH_PRIMS_H */
