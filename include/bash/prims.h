#ifndef BASH_PRIMS_H
#define BASH_PRIMS_H

/* Functions and data defined in src/main/prims.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/prims.c */

#include "common.h"
#include "bash/main.h"

void func_8001828C(void);
void func_800184D4(u8 * *arg0);
s32 *func_80018570(u8 *arg0);
s32 *func_800185F8(u8 *arg0);
void func_80018728(s32 arg0, s32 arg1);
void func_80018764(s32 *arg0);
void func_8001888C(void);
void func_800189E0(u8 *arg0);
void func_8001C008(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc);
void func_8001C0F0(s32 p, MorphModel *model, u16 *ia, u16 *ib, s16 (*verts)[3]);
void func_8001965C(void *arg0, void *arg1, void *arg2, u8 *arg3, GteSetup *out);
void func_8001CD04(void *arg0, void *arg1, void *arg2);
void func_80019A60(s32 a, u8 *set, s32 c, void *d);
void func_8001C2C0(u8 *arg0);
void kill_sprim_sub(subengine *arg0);
void func_8001AA48(ModelObj *obj, void *arg1, s32 flags);
void prim_colour_poly(RelTable *face, POLY_GT4 *poly, VECTOR *arg2, s32 mode);
void prim_create_entity_single(u8 *face, POLY_GT4 *poly);
void func_8001CBD4(u32 arg0);
void func_8001CC08(void);
void func_8001D174(EXTengine *eng);
void func_8001D220(EXTengine *eng);
void func_8001D2B4(subengine *first, subengine *last);
void kill_subs(subengine *first);
s32 func_80018F3C(EXTengine *eng, s32 n);
s32 func_8001D390(EXTengine *eng, s32 n);
void func_8001D604(void);
void map_update(s32 *arg0);
void func_8001DDD8(u8 *arg0);
void map_loaded_textureset(u8 *arg0, u8 * *arg1);
void func_8001E054(u8 *arg0, s32 arg1);
void set_fade(fade_info *fade, s32 speed, s32 delay);
void func_8001E824(process_control_info *pc);
void process_interrupt(process_control_info *pc, process_info *process);
void func_8001E848(process_control_info *pc, process_info *process);
void func_8001E898(process_control_info *pc);
void func_8001E934(process_control_info *pc, process_info *process);
void func_8001E948(void);
void func_8001C270(EXTengine *eng);
void func_80017388(void);
void func_8001A6D4(void *arg0, u32 hidden);
s32 func_8001A0D8(void *arg0, u32 flags);
void *func_80019094(void *arg0, s32 arg1);
s32 func_8001AF2C(void *arg0);
void draw_verts(POLY_GT3 *poly, SVECTOR *v, u8 *runs);
void func_80017164(s32 *cfg);
void map_loaded_geo();
void func_8001D3B0(EXTengine *eng, s32 n, subengine *sub);
void func_8001C008(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc);
void func_8001D048(SVECTOR *v, POLY_GT4 *poly);
u32 func_8001C974(NAS *nas, u8 *arg1, u32 flags);
void func_8001C690(NAS *nas, u8 *arg1);
void *func_8001CFE8(MorphModel *model, void *arg1, s32 arg2);
void process_quit(process_control_info *pc);
void process_init(process_control_info *pc);
s32 func_8001E598(fade_info *fade);
subengine *func_8001DCD4(s32 i, s32 val);
void *new_sprim(void);
void release_sprim(void *data);
void func_800183DC(void);
void func_8001C298(EXTengine *eng);
subengine *func_8001C34C(s32 n, subengine *sub);
void draw_init(void);
s32 func_8001DD20(s32 *arg0);
void func_8001CE00(s32 arg0, ModelSet *set, s32 arg2);
void func_8001CEB4(u16 arg0, u8 *arg1, s32 arg2);
void func_8001A82C(TextNAS *t, s32 arg1, s32 arg2);
void func_8001830C(void);
s32 *func_800184F0(s32 arg0, s32 *arg1, s32 arg2, s32 arg3);
void func_8001C3D0(u8 *arg0);
void func_8001C448(PosDst *arg0, s32 arg1, s32 arg2);
void func_8001C584(PosDst *arg0, s32 arg1);
s32 func_8001CC10(void *arg0, s32 arg1);
void func_8001D640(s32 n);
void draw_map_sub(void *arg0, void *arg1);
void func_8001DEF4(s32 *arg0, s32 *arg1);
KeyEntry *func_8001E41C(KeyBlock *arg0, s32 arg1);
s32 path_find(u32 arg0);
void func_80018430(s32 count);
s32 *func_80018694(s32 * *arg0, s32 arg1);
void func_800187D4(ListNode18938 *arg0, s32 arg1);
void func_80018938(ListNode18938 *arg0, s32 arg1);
void func_8001C1E0(u8 *arg0, u16 *src, u8 *tab, u16 *dst);
void *func_8001CF50(ModelEntry *e, s32 arg1);
s32 *path_get(u32 arg0);
void func_8001CAD4(u16 *arg0, s32 arg1, u32 arg2);
void func_8001D894(void *sub, void *arg1, s32 flags, s32 arg3);
MATRIX *func_8001ACC0(GfxXform *obj);
s32 func_8001D48C(subengine * *out_first, subengine * *last, s32 n);
u8 *tga_convert(u8 *arg0, s32 w, s32 h);
void func_80017640(void);
void func_80017B08(FaceRuns *faces, FacePrim *prim, s32 arg2, s32 mode);
void func_80017D90(FaceRuns *faces, FacePrim *prim);
void func_80019F1C(u8 *arg0, u8 *arg1, s32 arg2);
void func_8001A43C(u16 *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4);
void func_8001E0A8(u8 *arg0);
FacePrim *func_800180BC(s32 arg0, ListNode18938 *list, ListNode18938 *pool, FaceRuns *faces, s32 arg4);
void func_80018B08(GfxViewport *vp, GfxCamera *cam);
void func_8001DAF8(GfxScene *scene);
void func_8001D6B4(MapEnvHead *env);
s32 func_8001B6D4(SVECTOR *v);
void func_8001C7FC(NAS *nas, u8 *arg1, u32 flags);
KeyEntry *func_8001E21C(KeyBlock *arg0, s32 arg1, SLONG_XYZ *out);
void func_8001E610(process_control_info *pc, fade_info *fade);







/* Adds v to the translation GfxState.unk88 points at (func_8001965C). */
static inline void GfxAddTrans(VECTOR *v) {
    ((VECTOR *)D_80056998.unk88)->vx += v->vx;
    ((VECTOR *)D_80056998.unk88)->vy += v->vy;
    ((VECTOR *)D_80056998.unk88)->vz += v->vz;
}

/* Copies a scene's background into the current viewport, faded by modelDp
 * (func_8001DAF8). */
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

    clip.x = c->x * D_8005B698->w / 640;
    clip.y = c->y / 2;
    clip.w = (c->w - c->x) * D_8005B698->w / 640;
    clip.h = (c->h - c->y) / 2;
    gs->clipX1 = c->w * D_8005B698->w / 640;
    gs->clipY1 = c->h / 2;
    gs->cam = cam;
    func_80014540((u8 *)cam, &clip);
}

/* func_80019A60: draw model entry e with vertices v. */
static inline void ModelEntry_Draw(u8 *e, s32 c, SVECTOR *v) {
    void *p = func_80019094(e, c);

    if (p != NULL) {
        draw_verts(p, v, e + (*(s32 *)(e + 0x14) + 0x14));
    }
}

/* Takes the node at the cursor fC of a list (func_800185F8's code). */
static inline s32 *ListNode18938_Take(ListNode18938 *l) {
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


/* func_80019094: the next free primitive slot of list (refilled from pool). */
static inline FacePrim *PrimSet_NextPrim(PrimSet *set, s32 pairOff, s32 which, ListNode18938 *pool,
                                         void *faces, s32 flag) {
    s32 *n;
    s32 *next;
    FacePrim *ret;
    ListNode18938 *list =(ListNode18938 *)((u8 *)set + pairOff) + which;

    if (set->mode == 1) {
        n = list->fC;
        if (n != NULL) {
            next = (s32 *)*n;
            list->f10 = (s32 **)n;
            list->fC = next;
            set = (PrimSet *)n[1];
        } else {
            set = (PrimSet *)func_800180BC((s32)set, list, pool, (FaceRuns *)faces, flag);
        }
    } else {
        n = list->fC;
        if (n != NULL) {
            list->fC = (s32 *)*n;
            list->f10 = (s32 **)n;
            set = (PrimSet *)(n + 1);
        } else {
            set = (PrimSet *)func_800184F0((s32)set, (s32 *)list, (s32)pool, (s32)faces);
        }
    }
    return (FacePrim *)set;
}

/* func_8001D6B4: reuse the map's subengine array or allocate it. */
static inline subengine *MapEnv_GetSubs(MapEnvHead *env) {
    subengine *sub;
    subengine *t;

    t = env->subs;
    if (t != NULL) {
        sub = t;
        kill_subs(sub);
    } else {
        sub = (subengine *)func_80011654(env->num_placements * 0xA8, &D_8004E0F0);
    }
    return sub;
}

#include "bash/scripts.h"

#endif /* BASH_PRIMS_H */
