#ifndef BASH_80017164_H
#define BASH_80017164_H

/* Functions and data defined in src/main/80017164.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/80017164.c */

#include "common.h"
#include "bash/main.h"

void func_80020B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
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
void set_fade(u8 *arg0, s32 arg1, s32 arg2);
void func_8001E824(u8 *arg0);
void process_interrupt(u8 *arg0, s32 arg1);
void func_8001E848(u8 *arg0, s32 arg1);
void func_8001E898(u8 *arg0);
void func_8001E934(u8 *arg0, s32 arg1);
void func_8001E948(void);
void func_8002141C(u8 *arg0);
void render_entity_thread(u8 *arg0, s32 *arg1);
void func_80021990(u8 *arg0, s32 *arg1);
void func_80021C20(u8 *arg0, u8 *arg1, u8 *arg2, s32 arg3, s32 arg4);
void func_80021ED8(u8 *arg0);
void func_80021F14(u8 *arg0);
u8 *func_80021F6C(void);
void func_80021FB0(void);
void func_80022030(void);
void scripts_update(void);
void func_8001C270(EXTengine *eng);
void func_80017388(void);
void func_8001A6D4(void *arg0, u32 hidden);
s32 func_8001A0D8(void *arg0, u32 flags);
void *func_80019094(void *arg0, s32 arg1);
s32 func_8001AF2C(void *arg0);
void draw_verts(POLY_GT3 *poly, SVECTOR *v, u8 *runs);
void func_80017164(s32 *cfg);
void map_loaded_geo();
void func_8001F604(u8 *obj, EmitPart *part, u8 *def);
void func_8001D3B0(EXTengine *eng, s32 n, subengine *sub);
void func_8001C008(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc);
void func_8001D048(SVECTOR *v, POLY_GT4 *poly);
u32 func_8001C974(NAS *nas, u8 *arg1, u32 flags);
void func_8001C690(NAS *nas, u8 *arg1);
void *func_8001CFE8(MorphModel *model, void *arg1, s32 arg2);
void func_80021330(u8 *arg0, s32 *arg1);
void process_quit(void *arg0);
void process_init(void *arg0);
s32 func_8001E598(u8 *arg0);
subengine *func_8001DCD4(s32 i, s32 val);
void *new_sprim(void);
void release_sprim(void *data);
void func_800183DC(void);
u8 *func_80021E44(OffsetTable *tbl, s32 i);
void *func_80021EEC(void);
void func_8001C298(EXTengine *eng);
subengine *func_8001C34C(s32 n, subengine *sub);
void func_8002120C(u8 *arg0);
void draw_init(void);
u8 *get_script_header(u8 *arg0, s32 arg1);
s32 func_8001DD20(s32 *arg0);
u8 *func_80021E98(u8 *arg0);
void func_80021940(s32 arg0, u8 *arg1, s32 arg2);
void func_8001CE00(s32 arg0, ModelSet *set, s32 arg2);
void func_8001CEB4(u16 arg0, u8 *arg1, s32 arg2);
void func_80021238(u8 *arg0, s32 *arg1);
void func_80021B88(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4);
void func_80021AF0(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4);
void func_80021708(u8 *arg0, u8 *arg1, s32 arg2);
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
void func_80020B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void func_80020BA8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
s16 func_80020C4C(s32 arg0);
s32 func_800213A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 *func_80021878(u8 *arg0, s32 arg1);
s32 *func_800218DC(u8 *arg0, s32 arg1);
s32 *func_800219B8(u8 *arg0, s32 arg1);
u8 *func_80021DD4(u8 *arg0, s32 arg1, s32 arg2);
void func_800201BC(u8 *arg0, GteSetup *arg1);
void scripts_render(void);
void func_80018430(s32 count);
s32 *func_80018694(s32 * *arg0, s32 arg1);
void func_800187D4(ListNode18938 *arg0, s32 arg1);
void func_80018938(ListNode18938 *arg0, s32 arg1);
void func_8001C1E0(u8 *arg0, u16 *src, u8 *tab, u16 *dst);
void *func_8001CF50(ModelEntry *e, s32 arg1);
s32 *path_get(s32 arg0);
void func_8001E984(s32 arg0, s32 arg1);
void func_80020DF8(u8 *arg0);
void func_80020E9C(NAS *nas, u8 *arg1);
void func_80020F5C(u8 *arg0);
void func_80021054(u8 *arg0);
void func_8002128C(u8 *arg0, u8 *arg1, s32 arg2);
void func_8002147C(VelObj *obj, VelParams *p, VelState *s);
void func_80021604(u8 *arg0, u8 *arg1, s32 arg2);
void func_80021798(u8 *arg0, u8 *arg1, s32 arg2);
void func_80021A1C(u8 *arg0, u8 *arg1, s32 arg2);
s32 *func_80021CB8(s32 *arg0, s32 arg1, s32 arg2);
u8 *func_80021D1C(u8 *arg0, s32 arg1);
void func_8001CAD4(u16 *arg0, s32 arg1, u32 arg2);
void func_8001D894(void *sub, void *arg1, s32 flags, s32 arg3);
void func_8001F4F8(u8 *arg0, u8 *arg1);
MATRIX *func_8001ACC0(GfxXform *obj);
s32 func_8001D48C(subengine * *out_first, subengine * *last, s32 n);
void sub_play_script(u8 *arg0, s32 arg1, u8 *arg2, s32 arg3);
void func_800210F8(u8 *arg0, s32 arg1, u8 *arg2, s32 arg3, u8 * *arg4);
u8 *tga_convert(u8 *arg0, s32 w, s32 h);
s32 func_80020064(u8 *arg0);
void func_80017640(void);
void func_80017B08(FaceRuns *faces, FacePrim *prim, s32 arg2, s32 mode);
void func_80017D90(FaceRuns *faces, FacePrim *prim);
void func_80019F1C(u8 *arg0, u8 *arg1, s32 arg2);
void func_8001A43C(u16 *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4);
void func_8001E0A8(u8 *arg0);
void update_camera_thread(u8 *arg0, u8 *arg1);
void func_8001FCDC(u8 *arg0, u8 *arg1);
void func_80020344(Quat *a, Quat *b, s32 t, Quat *out);
FacePrim *func_800180BC(s32 arg0, ListNode18938 *list, ListNode18938 *pool, FaceRuns *faces, s32 arg4);
void func_80018B08(GfxViewport *vp, GfxCamera *cam);
void func_8001DAF8(GfxScene *scene);
void func_8001F0D4(u8 *arg0, u8 *arg1);
void func_8001F828(u8 *arg0, u8 *arg1);
void func_8001D6B4(MapEnvHead *env);
u8 *func_8001FE80(u8 *arg0, s32 arg1, Blk32 *arg2, CamVec3 *arg3, s32 arg4, u8 * *arg5);
s32 func_8001B6D4(SVECTOR *v);
void func_8001C7FC(NAS *nas, u8 *arg1, u32 flags);
KeyEntry *func_8001E21C(KeyBlock *arg0, s32 arg1, SLONG_XYZ *out);
void func_8001E610(u8 *mgr, u8 *fadeState);
void func_8001EAA4(u8 *arg0, u8 *arg1);
void func_80020680(Quat *a, Quat *b, s32 t, Quat *out, s32 *pre);
s32 func_80020C04(s32 arg0);

/* out = a + (b - a) * t / len, per component. */
static inline void CamVec3_Lerp(CamVec3 *out, CamVec3 *a, CamVec3 *b, s32 len, s32 t) {
    out->v[0] = interp(a->v[0], b->v[0], len, t);
    out->v[1] = interp(a->v[1], b->v[1], len, t);
    out->v[2] = interp(a->v[2], b->v[2], len, t);
}

/* The key of the camera track that covers frame v (or the terminating one). */
static inline CamKey *CamTrack_FindKey(u8 *track, s32 v) {
    CamKey *key = (CamKey *)(track + 0x1C);

    while (key->len != 0) {
        if (v >= key->start && v < key->start + key->len) {
            return key;
        }
        key++;
    }
    return key;
}

static inline AnimKey *AnimTrack_FindKey(AnimKey *key, s32 v) {
    while (key->len != 0) {
        if (v >= key->start && v < key->start + key->len) {
            return key;
        }
        key++;
    }
    return key;
}

/* The key of the object track that covers frame v (or the terminating one). */
static inline ObjKey *ObjTrack_FindKey(ObjKey *key, s32 v) {
    while (key->len != 0) {
        if (v >= key->start && v < key->start + key->len) {
            return key;
        }
        key++;
    }
    return key;
}

/* Frame t (16.16) of the range, clamped or looped, relative to its first frame. */
static inline s32 AnimRange_Frame(AnimRange *range, s32 t) {
    s32 frame;

    t -= range->off << 16;
    if (t < 0) {
        frame = range->lo << 16;
    } else if (range->clamp != 0) {
        if (t < (range->hi - range->lo) << 16) {
            frame = t + (range->lo << 16);
        } else {
            frame = (range->hi - 1) << 16;
        }
    } else {
        frame = t % ((range->hi - range->lo + 1) << 16);
        frame += range->lo << 16;
        if ((range->hi - 1) << 16 < frame) {
            frame = (range->hi - 1) << 16;
        }
    }
    frame -= range->lo << 16;
    return frame;
}

static inline s32 QuatDot(Quat *a, Quat *b) {
    return (a->v[0] * b->v[0] + a->v[1] * b->v[1] + a->v[2] * b->v[2] + a->v[3] * b->v[3] + 0x800) >> 12;
}

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

/* fade from `from` to `to` over [0,in), hold `to`, back to `from` over (out,end] */
static inline s32 EmitPart_Fade(s32 in, s32 out, s32 n, s32 end, s32 from, s32 to) {
    s32 ret;

    if (n < 0) {
        ret = from;
    } else if (n < in) {
        ret = interp(from, to, in, n);
    } else if (out < n) {
        if (end < n) {
            ret = from;
        } else {
            ret = interp(to, from, end - out, n - out);
        }
    } else {
        ret = to;
    }
    return ret;
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

#endif /* BASH_80017164_H */
