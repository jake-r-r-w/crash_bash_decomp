#include "common.h"
#include "bash/main.h"
#include "bash/prims.h"
#include "bash/engine.h"

static inline void prims_pool_clear(PrimList *l) {
    l->f4 = 0;
    l->f8 = NULL;
    l->fC = NULL;
    l->f10 = NULL;
}

void prims_init(s32 *cfg) {
    s32 cnt;
    u8 *p;
    u8 *q;
    u8 *next;
    s32 n;
    s32 end;
    s32 end2;
    s32 end1;

    cnt = cfg[0];
    PrimsConfig = cfg;
    PrimSetPool = mal_alloc(cnt * sizeof(PrimSet), &MainHeap);
    PrimNodePool = mal_alloc(cfg[2] * sizeof(PrimNode), &MainHeap);
    q = mal_alloc(cfg[1] * sizeof(PrimLink), &MainHeap);
    PrimLinkPool = q;
    p = PrimSetPool;
    next = PrimNodePool;
    PrimNodeFree = (PrimLink *)next;
    n = cfg[2] - 2;
    if (n != -1) {
        do {
            ((PrimNode *)next)->next = (PrimLink *)(next + sizeof(PrimNode));
            next += sizeof(PrimNode);
            n--;
            end1 = -1;
        } while (n != end1);
    }
    ((PrimNode *)next)->next = NULL;
    n = cfg[1] - 2;
    if (n != -1) {
        do {
            ((PrimLink *)q)->data = NULL;
            ((PrimLink *)q)->next = (PrimLink *)(q + sizeof(PrimLink));
            q += sizeof(PrimLink);
            n--;
            end2 = -1;
        } while (n != end2);
    }
    ((PrimLink *)q)->data = NULL;
    ((PrimLink *)q)->next = NULL;
    end = -1;
    n = cfg[0] + end;
    PrimLinkFree = (PrimLink *)PrimLinkPool;
    for (;;) {
        u8 *np;

        n--;
        if (n == end) {
            break;
        }
        np = (u8 *)((PrimSet *)p + 1);
        ((PrimSet *)p)->next = (PrimSet *)np;
        prims_pool_clear(&((PrimSet *)p)->pairs[0].b);
        prims_pool_clear(&((PrimSet *)p)->pairs[0].a);
        prims_pool_clear(&((PrimSet *)p)->pairs[1].b);
        prims_pool_clear(&((PrimSet *)p)->pairs[1].a);
        p = np;
    }
    prims_pool_clear(&((PrimSet *)p)->pairs[0].b);
    prims_pool_clear(&((PrimSet *)p)->pairs[0].a);
    prims_pool_clear(&((PrimSet *)p)->pairs[1].b);
    prims_pool_clear(&((PrimSet *)p)->pairs[1].a);
    PrimSetList = NULL;
    ((PrimSet *)p)->next = NULL;
    PrimPool.mem = NULL;
    PrimSetFree = (PrimSet *)PrimSetPool;
}

static inline void PrimList_Flush(PrimList *arg0, s32 arg1) {
    arg0->f0 = 0;
    if (arg1 == 1) {
        primnodes_free(arg0->fC);
    } else {
        arg0->f8->next = PrimNodeFree;
        PrimNodeFree = arg0->fC;
        if (arg0->f10 != NULL) {
            arg0->f10->next = NULL;
        } else {
            arg0->f4 = 0;
        }
        arg0->f8 = arg0->f10;
    }
    if (arg0->f10 != NULL) {
        arg0->f10->next = NULL;
    } else {
        arg0->f4 = 0;
    }
    arg0->f8 = arg0->f10;
}

static inline void PrimSet_Free(PrimSet *prev, PrimSet *set) {
    PrimSet *next;

    if (prev != NULL) {
        prev->next = set->next;
    } else {
        next = set->next;
        PrimSetList = next;
    }
    set->next = PrimSetFree;
    PrimSetFree = set;
}

void prims_update(void) {
    PrimSet *set;
    PrimSet *prev;
    s32 off;
    PrimSet *p;
    PrimSet *q;

    off =(FrameCount & 1) * sizeof(PrimListPair);
    set = PrimSetList;
    prev = NULL;
    while (1) {
        p = (PrimSet *)FILE_AT(set, off);
        if (set == NULL) {
            break;
        }
        if (p->pairs[0].a.fC != NULL) {
            if (++p->pairs[0].a.f0 >= 16) {
                PrimList_Flush(&p->pairs[0].a, set->mode);
            }
        } else {
            p->pairs[0].a.f0 = 0;
        }
        p = (PrimSet *)FILE_AT(set, off);
        if (p->pairs[0].b.fC != NULL) {
            if (++p->pairs[0].b.f0 >= 16) {
                PrimList_Flush(&p->pairs[0].b, set->mode);
            }
        } else {
            p->pairs[0].b.f0 = 0;
        }
        q = (PrimSet *)FILE_AT(set, off);
        q->pairs[0].a.f10 = NULL;
        q->pairs[0].b.f10 = NULL;
        q->pairs[0].a.fC = q->pairs[0].a.f4;
        q->pairs[0].b.fC = q->pairs[0].b.f4;
        if (set->dead == 1 && set->pairs[0].a.f4 == 0 && set->pairs[1].a.f4 == 0
            && set->pairs[0].b.f4 == 0 && set->pairs[1].b.f4 == 0) {
            PrimSet_Free(prev, set);
            if (prev != NULL) {
                set = prev->next;
            } else {
                set = PrimSetList;
            }
        } else {
            prev = set;
            set = set->next;
        }
    }
}

static inline void PrimList_Empty(PrimList *arg0, PrimSet *node) {
    s32 kind;

    PrimLink *list;

    list = arg0->f4;
    kind = node->mode;
    arg0->f10 = NULL;
    arg0->fC = list;
    if (arg0->fC != NULL) {
        PrimList_Flush(arg0, kind);
    }
}

void prims_empty_back(void) {
    s32 idx;
    PrimSet *node;

    idx = (FrameCount & 1) ^ 1;
    node = PrimSetList;
    for (;;) {
        if (node == NULL) {
            return;
        }
        PrimList_Empty(&node->pairs[idx].a, node);
        PrimList_Empty(&node->pairs[idx].b, node);
        node = node->next;
    }
}

void prim_colour_poly(MeshHeader *face, DrawFace *poly, VECTOR *arg2, s32 mode) {
    u16 *idx;
    u32 *colors;
    u32 *c;
    s32 semi = mode & 1;
    s32 blend = (mode >> 5) & 3;

    idx = (u16 *)((u8 *)&face->colors + face->colors);
    colors = (u32 *)((u8 *)&RenderState.dbase->data->colors + RenderState.dbase->data->colors);
    c = &colors[idx[0] & 0x1FFF];
    poly->colour[0] = colour_fade(c, RenderState.farColor, RenderState.dp);
    c++;
    poly->colour[1] = colour_fade(c, RenderState.farColor, RenderState.dp);
    c++;
    poly->colour[2] = colour_fade(c, RenderState.farColor, RenderState.dp);
    if (idx[0] & 0x8000) {
        poly->colour[0] |= DRAW_BLENDS;
        poly->blend = GFX_ADD;
    } else if (semi) {
        poly->colour[0] |= DRAW_BLENDS;
        poly->blend = blend;
    } else {
        poly->colour[0] &= ~DRAW_BLENDS;
        poly->blend = blend;
    }
    c = &colors[idx[1] & 0x1FFF];
    poly->colour[3] = colour_fade(c, RenderState.farColor, RenderState.dp);
}

void prim_create_entity_single(u8 *arg0, DrawFace *arg1) {
    obj_dbase *model = RenderState.dbase;
    MeshHeader *face = (MeshHeader *)arg0;
    DrawFace *poly = arg1;
    u16 *idx;
    u16 *uvIdx;
    u16 *tex;
    ModelFile *mc;
    u32 *colors;
    u32 *c;
    u16 *uvs;
    u16 *uv;
    GfxTexInfo *ti;
    u16 base;
    s32 t;
    obj_textureset *ts;

    uvIdx = (u16 *)((u8 *)&face->uvIdx + face->uvIdx);
    idx = (u16 *)((u8 *)&face->colors + face->colors);
    tex = (u16 *)((u8 *)&face->texs + face->texs);
    ts = &model->textures;
    mc = model->data;
    colors = (u32 *)((u8 *)&mc->colors + mc->colors);
    uvs = (u16 *)((u8 *)&mc->uvs + mc->uvs);
    c = &colors[*idx++ & 0x1FFF];
    poly->colour[0] = *c++;
    poly->colour[1] = c[0];
    poly->colour[2] = c[1];
    c = &colors[*idx & 0x1FFF];
    poly->colour[3] = *c;
    t = *tex;
    if (t & 0x8000) {
        ti = &ts->tex[ts->nTex - 1].info;
        poly->palette = ts->ent[t & 0x1FF].palette;
    } else {
        ti = &ts->tex[t & 0x1FF].info;
        poly->palette = ti->palette;
    }
    if (*idx & 0x8000) {
        poly->colour[0] |= DRAW_BLENDS;
    }
    poly->image = ti->image;
    poly->blend = GFX_ADD;
    base = ti->uv0;
    uv = &uvs[*uvIdx++];
    poly->uv[0] = base | uv[0];
    poly->uv[1] = base | uv[1];
    poly->uv[2] = base | uv[2];
    uv = &uvs[*uvIdx];
    poly->uv[3] = base | uv[2];
}

void faces_write_colors_dq(MeshHeader *faces, DrawFace *prim, s32 arg2, s32 mode) {
    u16 *idx;
    u8 *run;
    u8 *count;
    u32 *colors;
    u32 *c;
    s32 n;
    s32 semi = mode & 1;
    s32 blend = (mode >> 5) & 3;

    idx = (u16 *)((u8 *)&faces->colors + faces->colors);
    run = (u8 *)&faces->runs + faces->runs;
    colors = (u32 *)((u8 *)&RenderState.dbase->data->colors + RenderState.dbase->data->colors);
    count = run + 1;
    for (;; count += 2) {
        if (*count == 0xFF) {
            return;
        }
        for (n = *count; n > 0; n--) {
            c = &colors[*idx & 0x1FFF];
            prim->colour[0] = colour_fade(c, RenderState.farColor, RenderState.dp);
            if (semi) {
                prim->colour[0] |= DRAW_BLENDS;
                prim->blend = blend;
            } else if (*idx & 0x8000) {
                prim->colour[0] |= DRAW_BLENDS;
                prim->blend = (*idx >> 13) & 3;
            }
            c++;
            prim->colour[1] = colour_fade(c, RenderState.farColor, RenderState.dp);
            c++;
            prim->colour[2] = colour_fade(c, RenderState.farColor, RenderState.dp);
            prim++;
            idx++;
        }
    }
}

void faces_write_colors(MeshHeader *faces, DrawFace *prim) {
    u16 *idx;
    u8 *count;
    u32 *colors;
    u32 *c;
    s32 n;

    idx = (u16 *)((u8 *)&faces->colors + faces->colors);
    count = (u8 *)&faces->runs + faces->runs + 1;
    colors = (u32 *)((u8 *)&RenderState.dbase->data->colors + RenderState.dbase->data->colors);
    for (;; count += 2) {
        if (*count == 0xFF) {
            return;
        }
        for (n = *count; n > 0; n--) {
            c = &colors[*idx & 0x1FFF];
            prim->colour[0] = *c;
            if (*idx & 0x8000) {
                prim->colour[0] |= DRAW_BLENDS;
                prim->blend = (*idx >> 13) & 3;
            }
            c++;
            prim->colour[1] = c[0];
            prim->colour[2] = c[1];
            prim++;
            idx++;
        }
    }
}

void prim_write_uvs(MeshHeader *faces, DrawFace *prim) {
    s32 n;
    obj_textureset *ts;
    s32 left;
    u32 t;
    u16 *blend;
    u16 *tex;
    u16 *uvIdx;
    u8 *run;
    u16 *uvs;
    GfxTexInfo *ti;
    ModelFile *ut;
    u16 base;

    left = 0;
    ts = &RenderState.dbase->textures;
    uvIdx = (u16 *)((u8 *)&faces->uvIdx + faces->uvIdx);
    tex = (u16 *)((u8 *)&faces->texs + faces->texs);
    ut = RenderState.dbase->data;
    blend = (u16 *)((u8 *)&faces->colors + faces->colors);
    run = (u8 *)&faces->runs + faces->runs;
    uvs = (u16 *)((u8 *)&ut->uvs + ut->uvs);
    for (;; run += 2) {
        if (run[1] == 0xFF) {
            return;
        }
        for (n = run[1]; n > 0; n--) {
            u16 *uv;

            if (--left < 0) {
                t = *tex++;
                left = (t >> 9) & 0x3F;
            }
            prim->blend = (*blend >> 13) & 3;
            if (!(run[0] & 1)) {
                if (t & 0x8000) {
                    ti = &ts->tex[ts->nTex - 1].info;
                    prim->palette = ts->ent[t & 0x1FF].palette;
                } else {
                    ti = &ts->tex[t & 0x1FF].info;
                    prim->palette = ti->palette;
                }
                base = ti->uv0;
                prim->image = ti->image;
                uv = &uvs[*uvIdx];
                prim->uv[0] = base | uv[0];
                prim->uv[1] = base | uv[1];
                prim->uv[2] = base | uv[2];
            }
            prim++;
            uvIdx++;
            blend++;
        }
    }
}

DrawFace *face_prim_alloc(s32 arg0, PrimList *list, PrimList *pool, MeshHeader *faces, s32 arg4) {
    PrimLink *node;
    DrawFace *prim;

    node = PrimList_Take(pool);
    if (node != NULL) {
        node->next = list->f4;
        list->f4 = node;
        if (list->f8 == NULL) {
            list->f8 = node;
        }
        if (arg4 != 0) {
            faces_write_colors(faces, node->data);
        }
    } else {
        if (mal_first_free(faces->faces * (sizeof(DrawFace) / 4), MainHeap.free) == NULL || (node = PrimLinkFree) == NULL) {
            return NULL;
        }
        PrimLinkFree = node->next;
        if (list->f4 == NULL) {
            list->f8 = node;
        }
        node->next = list->f4;
        list->f4 = node;
        mal_alloc_owned_low(faces->faces * sizeof(DrawFace), &node->data, &MainHeap, ((FrameCount & 1)?3:2));
        prim = node->data;
        prim_write_uvs(faces, prim);
        faces_write_colors(faces, prim);
    }
    return node->data;
}

void primpool_release_all(void) {
    NodePool *pool;
    PoolNode *node;
    PoolNode *prev;
    PoolNode *next;
    PoolNode *head;

    for (;;) {
        pool = &PrimPool;
        head = pool->done;
        if (head == NULL) {
            return;
        }
        node = head;
        prev = NULL;
        do {
            next = node->next;
            if (--node->state == 0) {
                if (prev != NULL) {
                    prev->next = node->next;
                } else {
                    pool->done = node->next;
                }
                node->next = PrimPool.free;
                PrimPool.free = node;
            } else {
                prev = node;
            }
            node = next;
        } while (node != NULL);
    }
}

void primpool_age(void) {
    PoolNode *next;
    PoolNode *prev;
    PoolNode *cur;

    cur = PrimPool.done;
    prev = NULL;
    if (cur != NULL) do {
        next = cur->next;
        cur->state--;
        if (cur->state == 0) {
            if (prev != NULL) {
                prev->next = cur->next;
            } else {
                PrimPool.done = cur->next;
            }
            cur->next = PrimPool.free;
            PrimPool.free = cur;
        } else {
            prev = cur;
        }
        cur = next;
    } while (cur != NULL);
}

void release_sprim(void *data) {
    NodePool *pool = &PrimPool;
    PoolNode *node;

    node = (PoolNode *)data - 1;
    node->next = pool->done;
    pool->done = node;
    node->state = 2;
}

void *new_sprim(void) {
    PoolNode *node;

    node = PrimPool.free;
    if (node != NULL) {
        PrimPool.free = node->next;
        return node + 1;
    }
    return NULL;
}

void primpool_free(void) {
    NodePool *pool;

    primpool_release_all();
    pool = &PrimPool;
    if (pool->mem != NULL) {
        mal_free(pool->mem, &MainHeap);
    }
    pool->mem = NULL;
}

void primpool_init(s32 count) {
    PoolNode *p;
    PoolNode *n;
    s32 i;
    s32 last;

    if (PrimPool.mem == NULL) {
        PrimPool.mem = mal_alloc(count * (sizeof(PoolNode) + sizeof(DrawFace)), &MainHeap);
    }
    p = PrimPool.mem;
    last = count - 1;
    for (i = 0; i < last; i++) {
        n = (PoolNode *)FILE_AT(p, sizeof(PoolNode) + sizeof(DrawFace));
        p->next = n;
        p = n;
    }
    p->next = NULL;
    PrimPool.done = NULL;
    PrimPool.free = PrimPool.mem;
}

void primset_invalidate(ref32 *arg0) {
    PrimSet *set;

    set = ref_get(*arg0);
    if (set != NULL) {
        set->dead = 1;
    }
}

DrawFace *primnode_alloc(s32 arg0, PrimList *list, void *pool, MeshHeader *faces) {
    PrimLink *node;
    DrawFace *data;

    node = PrimNodeFree;
    if (node == NULL) {
        return NULL;
    }
    PrimNodeFree = node->next;
    if (list->f4 == NULL) {
        list->f8 = node;
    }
    data = (DrawFace *)&node->data;
    node->next = list->f4;
    list->f4 = node;
    prim_write_uvs(faces, data);
    faces_write_colors(faces, data);
    return data;
}

PrimLink *primlist_pop(PrimList *l) {
    PrimLink *tail;
    PrimLink *n;

    if (l == NULL || (n = l->fC) == NULL) {
        return NULL;
    }
    tail = l->f10;
    l->fC = n->next;
    if (tail != NULL) {
        tail->next = n->next;
        l->fC = n->next;
        if (n->next == NULL) {
            l->f8 = tail;
        }
    } else {
        l->f4 = n->next;
        l->fC = n->next;
        n->next = NULL;
    }
    return n;
}

PrimLink *primlist_pop_b(PrimList *l) {
    return PrimList_Take(l);
}

PrimSet *primset_alloc(ref32 *out, s32 mode) {
    PrimSet *p;
    PrimSet *next;
    PrimSet *prev;

    p = PrimSetFree;
    if (p == NULL) {
        return NULL;
    }
    *out = ref_of(p);
    next = p->next;
    prev = PrimSetList;
    PrimSetList = p;
    p->pairs[0].a.f0 = 0;
    p->pairs[0].a.f4 = 0;
    p->pairs[0].a.f8 = NULL;
    p->pairs[0].a.fC = NULL;
    p->pairs[0].a.f10 = NULL;
    p->pairs[1].a.f0 = 0;
    p->pairs[1].a.f4 = 0;
    p->pairs[1].a.f8 = NULL;
    p->pairs[1].a.fC = NULL;
    p->pairs[1].a.f10 = NULL;
    p->pairs[0].b.f0 = 0;
    p->pairs[0].b.f4 = 0;
    p->pairs[0].b.f8 = NULL;
    p->pairs[0].b.fC = NULL;
    p->pairs[0].b.f10 = NULL;
    p->pairs[1].b.f0 = 0;
    p->pairs[1].b.f4 = 0;
    p->pairs[1].b.f8 = NULL;
    p->pairs[1].b.fC = NULL;
    p->pairs[1].b.f10 = NULL;
    p->mode = mode;
    p->dead = 0;
    PrimSetFree = next;
    p->next = prev;
    return p;
}

void faces_write_uvs_colors(MeshHeader *arg0, DrawFace *arg1) {
    prim_write_uvs(arg0, arg1);
    faces_write_colors(arg0, arg1);
}

void primnodes_free(PrimLink *arg0) {
    PrimLink *temp_v1;
    PrimLink *var_a0;
    PrimLink *var_s0;

    var_s0 = arg0;
    var_a0 = NULL;
    if (arg0 != NULL) {
        do {
            mal_free(var_s0->data, &MainHeap);
            var_a0 = var_s0;
            var_s0 = var_a0->next;
        } while (var_s0 != NULL);
    }
    temp_v1 = PrimLinkFree;
    PrimLinkFree = arg0;
    var_a0->next = temp_v1;
}

void primlist_reset(PrimList *arg0, s32 arg1) {
    arg0->f10 = NULL;
    arg0->fC = arg0->f4;
    if (arg0->fC != NULL) {
        arg0->f0 = 0;
        if (arg1 == 1) {
            primnodes_free(arg0->fC);
        } else {
            arg0->f8->next = PrimNodeFree;
            PrimNodeFree = arg0->fC;
            if (arg0->f10 != NULL) {
                arg0->f10->next = NULL;
            } else {
                arg0->f4 = 0;
            }
            arg0->f8 = arg0->f10;
        }
        if (arg0->f10 != NULL) {
            arg0->f10->next = NULL;
        } else {
            arg0->f4 = 0;
        }
        arg0->f8 = arg0->f10;
    }
}

void prims_empty(void) {
    PrimSet *set;

    printf("empty prims\n");
    set = PrimSetList;
    if (set != NULL) {
        do {
            primlist_reset(&set->pairs[0].a, set->mode);
            primlist_reset(&set->pairs[0].b, set->mode);
            primlist_reset(&set->pairs[1].a, set->mode);
            primlist_reset(&set->pairs[1].b, set->mode);
            set = set->next;
        } while (set != NULL);
    }
}

void primlist_release(PrimList *arg0, s32 arg1) {
    arg0->f0 = 0;
    if (arg1 == 1) {
        primnodes_free(arg0->fC);
    } else {
        arg0->f8->next = PrimNodeFree;
        PrimNodeFree = arg0->fC;
        if (arg0->f10 != NULL) {
            arg0->f10->next = NULL;
        } else {
            arg0->f4 = 0;
        }
        arg0->f8 = arg0->f10;
    }
    if (arg0->f10 != NULL) {
        arg0->f10->next = NULL;
    } else {
        arg0->f4 = 0;
    }
    arg0->f8 = arg0->f10;
}

void primlist_clear(PrimList *l) {
    l->f4 = 0;
    l->f8 = NULL;
    l->fC = NULL;
    l->f10 = NULL;
}

u8 *tga_convert(tga_header *arg0, s32 w, s32 h) {
    s32 pw;
    s32 x;
    u16 *src;
    u16 *dst;
    u16 c;
    u8 *res;

    pw = arg0->width;
    if (pw < w) {
        w = pw;
    }
    if (arg0->height < h) {
        h = pw;
    }
    res = mal_alloc(w * h * 2, &MainHeap);
    src = (u16 *)FILE_AT(arg0, arg0->width * (arg0->height - 9) * 2 + 0x12);
    dst = (u16 *)res;
    for (h--; h != -1; h--) {
        for (x = w - 1; x != -1; x--) {
            c = *src++;
            *dst++ = ((c >> 10) & 0x1F) | ((c & 0x1F) << 10) | (c & 0x3E0);
        }
        src -= w + arg0->width;
    }
    return res;
}

void gfx_set_viewport(GfxViewport *vp, camera_info_tag *cam) {
    GfxState *gs = &RenderState;

    gs->viewport = vp;
    GfxSetClip(gs, (RECT *)&vp->cx0, cam);
    draw_viewport(vp->otIdx + vp->otLen - 1, vp->x0 * ScreenRect->w / 640, vp->y0 / 2, (vp->x1 - vp->x0) * ScreenRect->w / 640, (vp->y1 - vp->y0) / 2,
                  vp->isbg, vp->bg.r | (vp->bg.g << 8) | (vp->bg.b << 16));
    *(GfxOffset *)&gs->ofx = *(GfxOffset *)&vp->ofx;
    ((GfxOffset *)gs->screen)->x = gs->ofx;
    ((GfxOffset *)gs->screen)->y = gs->ofy;
    view_set_centre(vp->ofx * ScreenRect->w / 640, vp->ofy / 2);
    view_set_distance(cam->dist);
    view_set_rotation(&cam->view);
    view_set_translation(&cam->view);
    gs->layer = vp->otIdx;
    gs->otLen = vp->otLen - 1;
    ((GfxOffset *)gs->screen)[1].x = vp->ofs2;
    gs->otzAdd = 0;
    gs->modelDp = 0;
}

s32 eng_add_subs(EXTengine *eng, s32 n) {
    s32 count;
    subengine *free;
    subengine *prev;
    subengine *sub;
    subengine **lastp;

    count = 0;
    free = next_sub;
    lastp = &eng->last;
    if (free != NULL && n > 0) {
        prev = *lastp;
        sub = free;
        if (prev == NULL) {
            prev = free;
            eng->sub = free;
            free->prev = NULL;
            SubEngineTemplate.next = free->next;
            SubEngineTemplate.prev = free->prev;
            *sub = SubEngineTemplate;
            n--;
            count++;
            free = free->next;
        }
        while (n != 0) {
            if (free == NULL) {
                break;
            }
            prev->next = free;
            free->prev = prev;
            prev = free;
            SubEngineTemplate.next = free->next;
            SubEngineTemplate.prev = free->prev;
            *free = SubEngineTemplate;
            n--;
            free = free->next;
            count++;
        }
        prev->next = NULL;
        *lastp = prev;
        next_sub = free;
    }
    eng->num_subs += count;
    return count;
}

void *model_prims_begin(void *arg0, s32 arg1) {
    GfxState *gs;
    PrimSet *set;
    DrawFace *prim;
    s32 dp;
    s32 fog;
    s32 t;
    s32 off;
    PrimSet *p;
    static s32 c[3]; /* RenderState.farColor is left pointing at it */

    prim = NULL;
    if ((arg1 & 0x40000001) | RenderState.modelDp | (PrimDepthTest != 0 && 0 < PrimDepthLimit)) {
        set = ref_get(((MeshHeader *)arg0)->set);
        if (set != NULL || (set = primset_alloc(&((MeshHeader *)arg0)->set, ((MeshHeader *)arg0)->faces >= 3)) != NULL) {
            off = (FrameCount & 1) * sizeof(PrimListPair);
            p = (PrimSet *)((u8 *)set + off);
            prim = PrimSet_NextPrim(set, off + offsetof(PrimSet, pairs), 1, &p->pairs[0].b, arg0, 0);
        }
        if (prim != NULL) {
            gs = &RenderState;
            dp = gs->dp;
            fog = arg1 & 0x40000000;
            if (!fog) {
                gs->dp = 0;
            }
            if (gs->modelDp != 0) {
                gs->dp = gs->modelDp;
                if (fog) {
                    t = 0x1000 - gs->modelDp;
                    c[0] = t * gs->farColor[0];
                    c[1] = t * gs->farColor[1];
                    c[2] = t * gs->farColor[2];
                    c[0] >>= 12;
                    c[1] >>= 12;
                    gs->farColor = c;
                    c[2] >>= 12;
                } else {
                    gs->farColor = FarColor;
                }
            }
            faces_write_colors_dq((MeshHeader *)arg0, prim, 0, arg1);
            RenderState.dp = dp;
        } else {
            return NULL;
        }
    } else {
        set = ref_get(((MeshHeader *)arg0)->set);
        if (set != NULL || (set = primset_alloc(&((MeshHeader *)arg0)->set, ((MeshHeader *)arg0)->faces >= 3)) != NULL) {
            off = (FrameCount & 1) * sizeof(PrimListPair);
            p = (PrimSet *)((u8 *)set + off);
            prim = PrimSet_NextPrim(set, off + offsetof(PrimSet, pairs), 0, &p->pairs[0].a, arg0, set->mode);
        }
        if (prim == NULL) {
            return NULL;
        }
    }
    return prim;
}

/* Draws the triangle strips of a model: `runs` holds {mode, count} byte
 * pairs ended by count 0xFF, `v` the vertices (vz's high half carries
 * flags: 0x10000 flips the winding test, 0x20000 skips it). Mode bit 0
 * selects untextured faces. The three register variables are
 * the original's: only they give s4/s3/s2 with s0/s1 unused. */
void draw_verts(DrawFace *poly, SVECTOR *v, u8 *runs) {
    DrawFace *first = poly;
    u32 otz;
    s32 opz;
    GfxState *gs;
    s32 n;
    s32 mode;
    s32 flags;
    s32 wind;
    s32 hidden;
    ViewPoint p[3];

    gs = &RenderState;
    draw_owner_part(runs);
    for (;;) {
        if (runs[1] == 0xFF) {
            return;
        }
        project_point(v, &p[1]);
        v++;
        n = runs[1];
        project_point(v, &p[2]);
        v++;
        mode = *runs;
        if (mode & 1) {
            for (;;) {
                if (n-- <= 0) {
                    goto next;
                }
                flags = ((s32 *)v)[1];
                p[0] = p[1];
                p[1] = p[2];
                project_point(v, &p[2]);
                v++;
                otz = points_order(&p[0], &p[1], &p[2]);
                if (otz != 0) {
                    otz = (otz + gs->otzAdd) >> 1;
                    if (otz < gs->otLen) {
                        wind = 0;
                        hidden = 0;
                        if (!(flags & 0x20000)) {
                            opz = points_facing(&p[0], &p[1], &p[2]);
                            wind = (flags & 0x10000) ? -1 : 1;
                            hidden = (flags & 0x10000) ? opz > 0 : opz <= 0;
                        }
                        draw_mesh_tri_plain(gs->layer + otz, poly, p, (s32)(poly - first), wind, hidden);
                    }
                }
                poly++;
            }
        } else {
            while (n-- > 0) {
                flags = ((s32 *)v)[1];
                p[0] = p[1];
                p[1] = p[2];
                project_point(v, &p[2]);
                v++;
                otz = points_order(&p[0], &p[1], &p[2]);
                otz = (otz + gs->otzAdd) >> 1;
                if (otz < gs->otLen) {
                    wind = 0;
                    hidden = 0;
                    if (!(flags & 0x20000)) {
                        opz = points_facing(&p[0], &p[1], &p[2]);
                        wind = (flags & 0x10000) ? -1 : 1;
                        hidden = (flags & 0x10000) ? opz > 0 : opz <= 0;
                    }
                    draw_mesh_tri(gs->layer + otz, poly, p, (s32)(poly - first), wind, hidden);
                }
                poly++;
            }
        }
    next:
        runs += 2;
    }
}

void gfx_setup_xform(void *arg0, MATRIX *arg1, VECTOR *arg2, u8 *arg3, GteSetup *arg4) {
    GfxXform *obj = arg0;
    MATRIX *m = arg1;
    VECTOR *pos = arg2;
    u32 flags;
    GteSetup *out;

    out = arg4;
    out->trans = NULL;

    flags = obj->flags;
    if (flags & 0x10000000) {
        GfxState *gs;

        ScreenTrans.vz = pos->vz + obj->XYZ.z;
        out->trans = &ScreenTrans;
        gs = &RenderState;
        out->ofx = ((GfxScreen *)gs->screen)->x + obj->XYZ.x;
        out->ofy = ((GfxScreen *)gs->screen)->y + obj->XYZ.y;
    } else {
        GfxState *gs = &RenderState;
        out->ofx = ((GfxScreen *)gs->screen)->x;
        out->ofy = ((GfxScreen *)gs->screen)->y;
    }
    if (!(flags & 0x10010000)) {
        if (flags & 0x80000) {
            matrix_apply_long(&RenderState.cam->view, (VECTOR *)&obj->XYZ, (VECTOR *)RenderState.localPos);
            ((VECTOR *)RenderState.localPos)->vx += ((VECTOR *)RenderState.cam)->vx;
            ((VECTOR *)RenderState.localPos)->vy += ((VECTOR *)RenderState.cam)->vy;
            ((VECTOR *)RenderState.localPos)->vz += ((VECTOR *)RenderState.cam)->vz;
        } else {
            matrix_apply_long(m, (VECTOR *)&obj->XYZ, (VECTOR *)RenderState.localPos);
            GfxAddTrans(pos);
        }
        out->trans = (VECTOR *)RenderState.localPos;
    }
    if (obj->xform.order != 4) {
        if (obj->xform.dirty != 0) {
            rotscale_update(&obj->xform);
        }
        if (flags & 0x40000) {
            matrix_mul(&RenderState.cam->view, &obj->xform.mtx, (MATRIX *)RenderState.localMtx);
        } else if (flags & 0x20000000) {
            if (obj->xform.hasScale != 0) {
                RenderState.localMtx = (u8 *)&RenderState.alignMtx;
                RenderState.alignMtx.m[0][0] = (RenderState.cam->rot.m[0][0] * obj->xform.scale.vx) >> 12;
                RenderState.alignMtx.m[1][1] = (RenderState.cam->rot.m[1][1] * obj->xform.scale.vy) >> 12;
                RenderState.alignMtx.m[2][2] = (RenderState.cam->rot.m[2][2] * obj->xform.scale.vz) >> 12;
            } else {
                RenderState.localMtx = (u8 *)&RenderState.cam->rot;
            }
        } else {
            matrix_mul(m, &obj->xform.mtx, (MATRIX *)RenderState.localMtx);
        }
        if ((flags & 0x10010000) == 0x10000) {
            matrix_apply_long((MATRIX *)RenderState.localMtx, (VECTOR *)&obj->XYZ, (VECTOR *)RenderState.localPos);
            if (flags & 0x80000) {
                ((VECTOR *)RenderState.localPos)->vx += ((VECTOR *)RenderState.cam)->vx;
                ((VECTOR *)RenderState.localPos)->vy += ((VECTOR *)RenderState.cam)->vy;
                ((VECTOR *)RenderState.localPos)->vz += ((VECTOR *)RenderState.cam)->vz;
            } else {
                GfxAddTrans(pos);
            }
            out->trans = (VECTOR *)RenderState.localPos;
        }
        out->rot = (MATRIX *)RenderState.localMtx;
    } else if (flags & 0x20000000) {
        out->rot = &RenderState.cam->rot;
    } else if (flags & 0x40000) {
        out->rot = &RenderState.cam->view;
    } else {
        out->rot = m;
    }
}

void draw_object_model(s32 a, u8 *set, s32 c, subengine *d) {
    u8 *e;
    void *p;
    s32 *anim;
    s32 *fr;
    u8 *ia;
    u8 *verts;
    s32 idx;
    SVECTOR v;
    s32 entry;
    s32 frame;
    s16 xy[2];
    s32 depth;
    s16 *q;
    s32 x;
    s32 n;
    u32 flags;
    s32 col;
    s32 g;
    s32 otz;
    u32 fog = 0x40000000;

    depth = d->depth;
    {
        /* d->flags read as a plain word: as a field (in-struct) sched1 moves it past the
         * PrimDepthLimit store */
        s32 on = (*(u32 *)d | c) & fog;
        PrimDepthLimit = depth;
        if (on) {
            s32 dp = (s16)d->scale;
            RenderState.farColor = &d->color;
            RenderState.dp = dp;
        }
    }
    if (a == 0xFFFF) {
        return;
    }
    switch (a & 0x7000) {
    case 0x1000:
    case 0x4000:
        entry = (a & 0xF80) >> 7;
        if (entry >= ((ModelFile *)set)->nMeshAnims) {
            return;
        }
        e = (u8 *)((FileEntry *)((u8 *)&((ModelFile *)set)->meshAnims + ((ModelFile *)set)->meshAnims) + entry);
        anim = dbase_anim_node((s32 *)(iptr)a, (FileEntry *)e);
        if (c < 0) {
            frame = d->u70.h.hi;
        } else {
            frame = a & 0x7F;
        }
        if (frame >= ((FileEntry *)e)->frames) {
            return;
        }
        if (anim == NULL) {
            return;
        }
        e = (u8 *)&((FileEntry *)e)->mesh + ((FileEntry *)e)->mesh;
        fr = (s32 *)FILE_AT(anim, frame * 16 + 4);
        if (c & 0x400000) {
            e = (u8 *)&((ModelFile *)set)->meshes[(d->flags2 & 0xFFF) - 1];
        }
        if (fr[0] == 0) {
            return;
        }
        ia = SELF_REL(fr) + 0x14;
        if (anim[0] != 0) {
            verts = SELF_REL(anim);
        } else {
            verts = (u8 *)&((ModelFile *)set)->verts + ((ModelFile *)set)->verts;
        }
        if (c < 0 && (n = d->u70.h.lo) != 0) {
            s32 off;
            s32 *nx = (s32 *)FILE_AT(anim, frame * 16 + 0x14);
            s32 end = fr[6];
            s32 start = fr[2];
            s32 t;

            if (end == 0) {
                end = 0x1000;
            }
            t = n * (end - start);
            if (t < 0) {
                t += 0xFFFF;
            }
            n = t >> 16;
            n += start;
            if (nx[1] != 0) {
                off = nx[1] + 4;
            } else {
                off = nx[0];
            }
            p = FILE_AT(nx, off);
            morph_blend_verts(n, (MeshHeader *)e, (u16 *)ia,
                          (u16 *)FILE_AT(p, 0x14), (s16 (*)[3])verts);
        } else if (fr[2] != 0) {
            morph_blend_verts(fr[2], (MeshHeader *)e, (u16 *)ia,
                          (u16 *)(SELF_REL(fr + 1) + 0x14), (s16 (*)[3])verts);
        } else {
            morph_copy_verts((MeshHeader *)e, (u16 *)ia, verts, (u16 *)MorphVerts);
        }
        ModelEntry_Draw(e, c, MorphVerts);
        return;
    case 0x2000:
        idx = a & 0xFFF;
        if (((ModelFile *)set)->nMeshes < idx) {
            return;
        }
        e = (u8 *)&((ModelFile *)set)->meshes[idx - 1];
        goto block;
    case 0x5000:
        e = (u8 *)map_entity_address(a, (ModelFile *)set);
        if (e == NULL) {
            return;
        }
    block:
        set = (u8 *)&((MeshHeader *)e)->verts + ((MeshHeader *)e)->verts;
        if (!(c & 0x1000000) && !bbox_visible(set)) {
            return;
        }
        p = model_prims_begin(e, c);
        set += 0x14;
        if (p != NULL) {
            draw_verts(p, (SVECTOR *)set, (u8 *)&((MeshHeader *)e)->runs + ((MeshHeader *)e)->runs);
        }
        return;
    case 0x3000:
        memset(&v, 0, 8);
        q = xy;
        project_xy(&v, q);
        x = xy[0] * 640 / ScreenRect->w;
        flags = d->flags;
        g = ((flags & 0x21) != 0) << 31;
        g |= (((s32)flags >> 5) & 3) << 24;
        x = (x & 0xFFFF) | ((u16)q[1] << 17);
        if (flags & fog) {
            idx = d->scale >> 4;
            idx &= 0xFF;
            col = idx | (idx << 8) | (idx << 16);
        } else {
            col = 0x808080;
        }
        g |= col;
        otz = 1;
        if (c & 0x2000000) {
            otz = d->otz;
        }
        if ((u32)otz < (u32)RenderState.otLen) {
            texture_draw(&RenderState.dbase->textures.tex[a & 0xFFF],
                          x, otz, g);
        }
        return;
    default:
        printf("UNKNOWN FRAME NUM %X\n", a);
        return;
    }
}

void draw_object(subengine *arg0, MATRIX *arg1, s32 arg2) {
    GteSetup s;
    obj_dbase *model;
    ModelFile *colors;
    u16 n;
    GfxState *g;
    MATRIX *p1 = arg1;

    n = arg0->entity;
    if (arg0->flags & 0x02000000) {
        GfxState *g1 = &RenderState;
        g1->otzAdd = ((GfxScreen *)g1->screen)->otz + arg0->otz;
    } else {
        g = &RenderState;
        g->otzAdd = ((GfxScreen *)g->screen)->otz;
    }
    if (n != 0 && (arg0->flags & 0x8000)) {
        model = arg0->u6C.dbase;
        if (model != NULL) {
            RenderState.dbase = model;
            colors = model->data;
            if (colors != NULL) {
                RenderState.localMtx = RenderState.mtxBuf;
                RenderState.localPos = RenderState.posBuf;
                gfx_setup_xform(arg0, p1, (VECTOR *)p1->t, RenderState.screen, &s);
                view_set_centre(s.ofx * ScreenRect->w / 640, s.ofy / 2);
                if (s.trans != NULL) {
                    view_set_translation_vector(s.trans);
                }
                view_set_rotation(s.rot);
                /* an object with a layer of its own stays in the game's order; the rest are real solid things */
                draw_owner(arg0);
                draw_depth(!(arg0->flags & 0x02000000));
                draw_object_model(n, (u8 *)colors, arg0->flags | arg2, arg0);
                draw_depth(0);
                draw_owner(NULL);
            }
        }
    }
}

s32 draw_gouraud_quad(void *arg0, u32 flags) {
    ColorQuad *q = arg0;
    DVECTOR xy[4];
    u32 colour[4];
    u32 otz;
    s32 i;

    otz = 0;
    if (!(flags & 0x8000)) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        colour[i] = calc_rgbcode(q->c[i][2], q->c[i][1], q->c[i][0]);
    }
    if (flags & 0x10000000) {
        GfxScreen *s = (GfxScreen *)RenderState.screen;

        for (i = 0; i < 4; i++) {
            xy[i].vx = ((s->x + q->v[i].vx) * ScreenRect->w) / 640;
            xy[i].vy = (s->y + q->v[i].vy) / 2;
        }
    } else {
        ViewPoint c[3];

        project_point(&q->v[0], &c[0]);
        project_point(&q->v[1], &c[1]);
        project_point(&q->v[2], &c[2]);
        otz = points_order(&c[0], &c[1], &c[2]);
        point_store_xy(&c[0], &xy[0]);
        point_store_xy(&c[1], &xy[1]);
        point_store_xy(&c[2], &xy[2]);
        project_xy(&q->v[3], &xy[3]);
    }
    otz = (otz + RenderState.otzAdd) >> 1;
    if (otz < RenderState.otLen) {
        draw_plain_quad(RenderState.layer + otz, xy, colour, (flags & 0x21) ? (flags >> 5) & 3 : GFX_OPAQUE);
    }
    return 1;
}

void draw_panel_frame(u16 *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4) {
    QuadVerts q;
    s32 x;
    s32 w;
    s32 l;
    s32 r;

    w = (s16) arg0[3];
    x = (s16) arg0[0];
    if ((x & 0xC000) == 0x8000) {
        x = 0x140 - (w / 2);
        x -= ((GfxScreen *)RenderState.screen)->x;
        w += x;
    }
    l = x - arg3;
    q.v[0].x = l;
    q.v[0].y = arg0[1];
    q.v[0].z = arg0[2];
    q.v[1].x = x;
    q.v[1].y = arg0[1];
    q.v[1].z = arg0[2];
    q.v[2].x = l;
    q.v[2].y = arg0[4];
    q.v[2].z = arg0[2];
    q.v[3].x = x;
    q.v[3].y = arg0[4];
    q.v[3].z = arg0[2];
    q.c[0] = arg1;
    q.c[1] = arg1;
    q.c[2] = arg1;
    q.c[3] = arg1;
    draw_gouraud_quad(&q, arg2);
    r = w + arg3;
    q.v[0].x = w;
    q.v[0].y = arg0[1];
    q.v[0].z = arg0[2];
    q.v[1].x = r;
    q.v[1].y = arg0[1];
    q.v[1].z = arg0[2];
    q.v[2].x = w;
    q.v[2].y = arg0[4];
    q.v[2].z = arg0[2];
    q.v[3].x = r;
    q.v[3].y = arg0[4];
    q.v[3].z = arg0[2];
    q.c[0] = arg1;
    q.c[1] = arg1;
    q.c[2] = arg1;
    q.c[3] = arg1;
    draw_gouraud_quad(&q, arg2);
    q.v[0].x = l;
    q.v[0].y = arg0[1] - arg4;
    q.v[0].z = arg0[2];
    q.v[1].x = r;
    q.v[1].y = arg0[1] - arg4;
    q.v[1].z = arg0[2];
    q.v[2].x = l;
    q.v[2].y = arg0[1];
    q.v[2].z = arg0[2];
    q.v[3].x = r;
    q.v[3].y = arg0[1];
    q.v[3].z = arg0[2];
    q.c[0] = arg1;
    q.c[1] = arg1;
    q.c[2] = arg1;
    q.c[3] = arg1;
    draw_gouraud_quad(&q, arg2);
    q.v[0].x = l;
    q.v[0].y = arg0[4];
    q.v[0].z = arg0[2];
    q.v[1].x = r;
    q.v[1].y = arg0[4];
    q.v[1].z = arg0[2];
    q.v[2].x = l;
    q.v[2].y = arg0[4] + arg4;
    q.v[2].z = arg0[2];
    q.v[3].x = r;
    q.v[3].y = arg0[4] + arg4;
    q.v[3].z = arg0[2];
    q.c[0] = arg1;
    q.c[1] = arg1;
    q.c[2] = arg1;
    q.c[3] = arg1;
    draw_gouraud_quad(&q, arg2);
}

void draw_panel_rect(void *arg0, u32 hidden) {
    QuadVerts q;
    u32 f;
    u16 *r;
    s32 c;
    s32 w;
    s32 x;

#define p ((u32 *) arg0)
    f = p[0];
    r = (u16 *) arg0 + 2;
    if (f & 0x8000) {
        f |= hidden;
        w = (s16) r[3];
        x = (s16) r[0];
        c = p[4];
        if ((x & 0xC000) == 0x8000) {
            if (x & 0x3FFF) {
                x = (x & 0xFFFF3FFF) - (w / 2);
            } else {
                x = 0x140 - (w / 2);
                x -= ((GfxScreen *)RenderState.screen)->x;
            }
            w += x;
        }
        if (arg0) {
            q.v[0].x = x;
            q.v[0].y = r[1];
            q.v[0].z = r[2];
            q.v[1].x = w;
            q.v[1].y = r[1];
            q.v[1].z = r[2];
            q.v[2].x = x;
            q.v[2].y = r[4];
            q.v[2].z = r[2];
            q.v[3].x = w;
            q.v[3].y = r[4];
            q.v[3].z = r[2];
            q.c[0] = c;
            q.c[1] = c;
            q.c[2] = c;
            q.c[3] = c;
            draw_gouraud_quad(&q, f);
        } else {
            q.v[0].x = x;
            q.v[0].y = r[1];
            q.v[0].z = r[2];
            q.v[1].x = w;
            q.v[1].y = r[1];
            q.v[1].z = r[2];
            q.v[2].x = x;
            q.v[2].y = r[4];
            q.v[2].z = r[2];
            q.v[3].x = w;
            q.v[3].y = r[4];
            q.v[3].z = r[2];
            q.c[0] = c;
            q.c[1] = c;
            q.c[2] = c;
            q.c[3] = c;
            draw_gouraud_quad(&q, f);
        }
    }
    if (p[7] & 0x8000) {
        draw_panel_frame((u16 *) arg0 + 2, p[5], p[7] | hidden, ((s16 *) arg0)[12], ((s16 *) arg0)[13]);
    }
#undef p
}

void draw_text_nas(TextNAS *t, MATRIX *arg1, s32 arg2) {
    GteSetup s;
    s32 x;

    if (!(t->NASid & 0x8000)) {
        return;
    }
    if (t->str == NULL) {
        return;
    }
    FontPage = t->font;
    if (t->NASid & 0x10000000) {
        x = t->x;
        if ((x & 0xC000) == 0x8000) {
            x &= ~0xC000;
            x <<= 14;
            x >>= 14;
            x += ((GfxScreen *)RenderState.screen)->x;
            x &= ~0xC000;
            x |= 0x8000;
        } else {
            x = ((GfxScreen *)RenderState.screen)->x + t->x;
        }
        DrawSysText(x, (s16)(*(s16 *)&((GfxScreen *)RenderState.screen)->y + t->y), t->str, t->c0, t->c1, 0);
        return;
    }
    RenderState.localMtx = RenderState.mtxBuf;
    RenderState.localPos = RenderState.posBuf;
    gfx_setup_xform(t, arg1, (VECTOR *)arg1->t, RenderState.screen, &s);
    view_set_centre(s.ofx * ScreenRect->w / 640, s.ofy / 2);
    if (s.trans != NULL) {
        view_set_translation_vector(s.trans);
    }
    view_set_rotation(s.rot);
    ((SWORD_XYZ *)&s)->x = t->x;
    ((SWORD_XYZ *)&s)->y = t->y;
    ((SWORD_XYZ *)&s)->z = t->z;
    SysPrintAt3d((SWORD_XYZ *)&s, t->str, t->c0, t->c1);
}

static inline u8 *draw_model_quad_setup(ModelObj *obj, void *arg1, s32 flags, ModelFile *colors, u8 *t) {
    draw_map_object(obj, arg1, flags, colors);
    return t;
}

void draw_model_quad(ModelObj *obj, void *arg1, s32 flags) {
    ModelFile *colors;
    DrawFace *prim;
    u8 *t;
    u32 otz;
    ViewPoint c[4];

    if (!(obj->flags & 0x8000)) {
        return;
    }
    colors = obj->model->data;
    if (colors == NULL) {
        return;
    }
    prim = obj->slots[FrameCount & 1].prim;
    if (prim == NULL) {
        prim = new_sprim();
        obj->slots[FrameCount & 1].prim = prim;
        if (prim == NULL) {
            return;
        }
        RenderState.dbase = obj->model;
        prim_create_entity_single(entity_address(obj->index, colors), prim);
    }
    t = entity_address(obj->index, colors);
    RenderState.dp = RenderState.modelDp;
    RenderState.dbase = obj->model;
    prim_colour_poly((MeshHeader *)t, prim, (VECTOR *)RenderState.farColor, flags);
    colors = obj->model->data;
    t = draw_model_quad_setup(obj, arg1, flags, colors, entity_address(obj->index, colors));
    t = (u8 *)&((MeshHeader *)t)->verts + ((MeshHeader *)t)->verts;
    project_point(&((MeshVerts *)t)->v[0], &c[0]);
    project_point(&((MeshVerts *)t)->v[1], &c[1]);
    project_point(&((MeshVerts *)t)->v[2], &c[2]);
    project_point(&((MeshVerts *)t)->v[3], &c[3]);
    otz = points_order(&c[1], &c[2], &c[3]);
    otz = (otz + RenderState.otzAdd) >> 1;
    if (otz < RenderState.otLen) {
        draw_mesh_quad(RenderState.layer + otz, prim, c);
    }
}

MATRIX *gfx_object_matrix(GfxXform *obj) {
    GfxState *gs;
    u32 flags;
    MATRIX *view;
    MATRIX *rot;
    MATRIX *out;
    GfxCamScale *cs;
    VECTOR v;
    s32 *pos;

    gs = &RenderState;
    flags = obj->flags;
    gs->localMtx = gs->mtxBuf;
    view = &gs->cam->view;
    gs->localPos = gs->posBuf;
    out = &gs->objMtx;
    cs = &gs->cam->xform;
    if (flags & 0x10000000) {
        ((GfxScreen *)gs->screen)->x = obj->XYZ.x + gs->ofx;
        ((GfxScreen *)gs->screen)->y = obj->XYZ.y + gs->ofy;
        if (flags & 0x20000000) {
            out->m[0][0] = cs->scale.vx;
            out->m[0][1] = 0;
            out->m[0][2] = 0;
            out->m[1][0] = 0;
            out->m[1][1] = cs->scale.vy;
            out->m[1][2] = 0;
            out->m[2][0] = 0;
            out->m[2][1] = 0;
            out->m[2][2] = cs->scale.vz;
        } else {
            if (obj->xform.dirty != 0) {
                rotscale_update(&obj->xform);
            }
            matrix_mul(view, &obj->xform.mtx, out);
        }
        out->t[0] = view->t[0];
        out->t[1] = view->t[1];
        out->t[2] = view->t[2];
    } else {
        ((GfxScreen *)gs->screen)->x = gs->ofx;
        ((GfxScreen *)gs->screen)->y = gs->ofy;
        if (!(flags & 0x200000)) {
            rot = view;
        } else {
            rot = &MapSceneMatrix;
        }
        pos = &RenderState.cam->at.x;
        v.vx = obj->XYZ.x - pos[0];
        v.vy = obj->XYZ.y - pos[1];
        v.vz = obj->XYZ.z - pos[2];
        matrix_apply_long(rot, &v, (VECTOR *)out->t);
        out->t[0] += view->t[0];
        out->t[1] += view->t[1];
        out->t[2] += view->t[2];
        if (flags & 0x20000000) {
            out->m[0][0] = cs->scale.vx;
            out->m[0][1] = 0;
            out->m[0][2] = 0;
            out->m[1][0] = 0;
            out->m[1][1] = cs->scale.vy;
            out->m[1][2] = 0;
            out->m[2][0] = 0;
            out->m[2][1] = 0;
            out->m[2][2] = cs->scale.vz;
        } else {
            if (obj->xform.dirty != 0) {
                rotscale_update(&obj->xform);
            }
            matrix_mul(rot, &obj->xform.mtx, out);
        }
    }
    RenderState.otzAdd = ((GfxScreen *)RenderState.screen)->otz;
    return out;
}

s32 bbox_visible(void *arg0) {
    BoundBox *box = arg0;
    DVECTOR sxy[8];
    DVECTOR c;
    s32 sz;
    s32 flags;
    SVECTOR *in;
    GfxState *gs;
    camera_info_tag *cam;
    s32 r;
    s32 d;
    s32 e;
    s32 ey;

    flags = 0;
    in = &RenderState.viewIn;
    in = &box->center;
    project_xyz(in, &c, &sz);
    in = &RenderState.viewIn;
    if (sz >= 2) {
        r = box->center.pad;
        gs = &RenderState;
        if (r < 0x400) {
            cam = gs->cam;
            d = sz * 3 << 10;
            e = r * cam->xform.scale.vx * cam->dist / d;
            if (c.vx + e < gs->clipX0 || gs->clipX1 < c.vx - e) {
            culled:
                return 0;
            }
            ey = r * cam->xform.scale.vy * cam->dist / d;
            if (c.vy + ey < 0 || gs->clipY1 < c.vy - ey) {
                goto culled;
            }
        }
    }
    in->vx = box->min[0];
    in->vy = box->min[1];
    in->vz = box->min[2];
    project_xyz(in, &sxy[0], &sz);
    in->vx = box->max[0];
    if (sz > 0) {
        if (sxy[0].vx >= RenderState.clipX0 && sxy[0].vx < RenderState.clipX1 && (u16)sxy[0].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 1;
    }
    project_xyz(in, &sxy[1], &sz);
    in->vx = box->min[0];
    in->vy = box->max[1];
    if (sz > 0) {
        if (sxy[1].vx >= RenderState.clipX0 && sxy[1].vx < RenderState.clipX1 && (u16)sxy[1].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 2;
    }
    project_xyz(in, &sxy[2], &sz);
    in->vx = box->max[0];
    if (sz > 0) {
        if (sxy[2].vx >= RenderState.clipX0 && sxy[2].vx < RenderState.clipX1 && (u16)sxy[2].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 4;
    }
    project_xyz(in, &sxy[3], &sz);
    in->vx = box->min[0];
    in->vy = box->min[1];
    in->vz = box->max[2];
    if (sz > 0) {
        if (sxy[3].vx >= RenderState.clipX0 && sxy[3].vx < RenderState.clipX1 && (u16)sxy[3].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 8;
    }
    project_xyz(in, &sxy[4], &sz);
    in->vx = box->max[0];
    if (sz > 0) {
        if (sxy[4].vx >= RenderState.clipX0 && sxy[4].vx < RenderState.clipX1 && (u16)sxy[4].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 0x10;
    }
    project_xyz(in, &sxy[5], &sz);
    in->vx = box->min[0];
    in->vy = box->max[1];
    if (sz > 0) {
        if (sxy[5].vx >= RenderState.clipX0 && sxy[5].vx < RenderState.clipX1 && (u16)sxy[5].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 0x20;
    }
    project_xyz(in, &sxy[6], &sz);
    in->vx = box->max[0];
    if (sz > 0) {
        if (sxy[6].vx >= RenderState.clipX0 && sxy[6].vx < RenderState.clipX1 && (u16)sxy[6].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 0x40;
    }
    project_xyz(in, &sxy[7], &sz);
    if (sz > 0) {
        if (sxy[7].vx >= RenderState.clipX0 && sxy[7].vx < RenderState.clipX1 && (u16)sxy[7].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 0x80;
    }
    if (flags == 0xFF) {
        goto culled;
    }
    if (sxy[0].vx > RenderState.clipX1 && sxy[1].vx > RenderState.clipX1 &&
        sxy[2].vx > RenderState.clipX1 && sxy[3].vx > RenderState.clipX1 &&
        sxy[4].vx > RenderState.clipX1 && sxy[5].vx > RenderState.clipX1 &&
        sxy[6].vx > RenderState.clipX1 && sxy[7].vx > RenderState.clipX1) {
        goto culled;
    }
    if (sxy[0].vx < RenderState.clipX0 && sxy[1].vx < RenderState.clipX0 && sxy[2].vx < RenderState.clipX0 && sxy[3].vx < RenderState.clipX0 &&
        sxy[4].vx < RenderState.clipX0 && sxy[5].vx < RenderState.clipX0 && sxy[6].vx < RenderState.clipX0 && sxy[7].vx < RenderState.clipX0) {
        goto culled;
    }
    if (sxy[0].vy > RenderState.clipY1 && sxy[1].vy > RenderState.clipY1 &&
        sxy[2].vy > RenderState.clipY1 && sxy[3].vy > RenderState.clipY1 &&
        sxy[4].vy > RenderState.clipY1 && sxy[5].vy > RenderState.clipY1 &&
        sxy[6].vy > RenderState.clipY1 && sxy[7].vy > RenderState.clipY1) {
        return 0;
    }
    return 1;
}

s32 tri_visible(SVECTOR *v) {
    s16 sxy[9][2];
    u32 sz[9];
    s32 behind;
    s32 inside;
    s32 far;
    s32 minX, maxX, minY, maxY;
    u32 farZ;
    s32 dx, dy;
    s32 ret;
    s16 clip;

    behind = 0;
    inside = 0;
    far = 0;
    farZ = (RenderState.otLen - RenderState.otzAdd) * 8;
    project_three(v, sxy[0], (u32 *)&sz[0]);
    v += 3;
    project_three(v, sxy[3], (u32 *)&sz[3]);
    v += 3;
    minX = sxy[0][0];
    maxX = minX;
    minY = sxy[0][1];
    maxY = minY;
    if (sz[0] != 0) {
        if (sxy[0][0] >= RenderState.clipX0 && sxy[0][0] < RenderState.clipX1 && (u16)sxy[0][1] < (u16)RenderState.clipY1) {
            if (behind) return 2;
            inside |= 1;
        }
        if (farZ < sz[0]) far |= 1;
    } else {
        behind |= 1;
        if (inside) return 2;
    }
    if (sz[1] != 0) {
        if (sxy[1][0] >= RenderState.clipX0 && sxy[1][0] < RenderState.clipX1 && (u16)sxy[1][1] < (u16)RenderState.clipY1) {
            if (behind) return 2;
            inside |= 2;
        }
        if (farZ < sz[1]) far |= 2;
    } else {
        behind |= 2;
        if (inside) return 2;
    }
    if (sxy[1][0] < minX) minX = sxy[1][0];
    if (maxX < sxy[1][0]) maxX = sxy[1][0];
    if (sxy[1][1] < minY) minY = sxy[1][1];
    if (maxY < sxy[1][1]) maxY = sxy[1][1];
    if (sz[2] != 0) {
        if (sxy[2][0] >= RenderState.clipX0 && sxy[2][0] < RenderState.clipX1 && (u16)sxy[2][1] < (u16)RenderState.clipY1) {
            if (behind) return 2;
            inside |= 4;
        }
        if (farZ < sz[2]) far |= 4;
    } else {
        behind |= 4;
    }
    if (sxy[2][0] < minX) minX = sxy[2][0];
    if (maxX < sxy[2][0]) maxX = sxy[2][0];
    if (sxy[2][1] < minY) minY = sxy[2][1];
    if (maxY < sxy[2][1]) maxY = sxy[2][1];
    project_three(v, sxy[6], (u32 *)&sz[6]);
#define POINT(i)                                                                                      \
    if (sz[i] != 0) {                                                                                 \
        if (sxy[i][0] >= RenderState.clipX0 && sxy[i][0] < RenderState.clipX1 && (u16)sxy[i][1] < (u16)RenderState.clipY1) {    \
            if (behind) return 2;                                                                     \
            inside |= 1 << i;                                                                         \
            if (farZ < sz[i]) far |= 1 << i;                                                          \
        }                                                                                             \
    } else {                                                                                          \
        behind |= 1 << i;                                                                             \
    }                                                                                                 \
    if (sxy[i][0] < minX) minX = sxy[i][0];                                                           \
    if (maxX < sxy[i][0]) maxX = sxy[i][0];                                                           \
    if (sxy[i][1] < minY) minY = sxy[i][1];                                                           \
    if (maxY < sxy[i][1]) maxY = sxy[i][1];
    POINT(3)
    POINT(4)
    POINT(5)
    POINT(6)
    POINT(7)
#undef POINT
    if (far == 0xFF) return 0;
    if (behind != 0) {
        if (behind == 0xFF) return 0;
        if (inside != 0) return 2;
    } else if (inside == 0x7F) {
        return 1;
    }
    if (sxy[0][0] < RenderState.clipX0 && sxy[1][0] < RenderState.clipX0 && sxy[2][0] < RenderState.clipX0 && sxy[3][0] < RenderState.clipX0 && sxy[4][0] < RenderState.clipX0 &&
        sxy[5][0] < RenderState.clipX0 && sxy[6][0] < RenderState.clipX0 && sxy[7][0] < RenderState.clipX0) {
        return 0;
    }
    clip = RenderState.clipX1;
    if (sxy[0][0] > clip && sxy[1][0] > clip && sxy[2][0] > clip && sxy[3][0] > clip &&
        sxy[4][0] > clip && sxy[5][0] > clip && sxy[6][0] > clip && sxy[7][0] > clip) {
        return 0;
    }
    clip = RenderState.clipY1;
    if (clip < sxy[0][1] && clip < sxy[1][1] && clip < sxy[2][1] && clip < sxy[3][1] &&
        clip < sxy[4][1] && clip < sxy[5][1] && clip < sxy[6][1] && clip < sxy[7][1]) {
        return 0;
    }
    if (sxy[0][1] < 0 && sxy[1][1] < 0 && sxy[2][1] < 0 && sxy[3][1] < 0 && sxy[4][1] < 0 &&
        sxy[5][1] < 0 && sxy[6][1] < 0 && sxy[7][1] < 0) {
        return 0;
    }
    ret = 0;
    dx = maxX - minX;
    dy = maxY - minY;
    if (dx >= 0x400 || dy >= 0x200 || dx * dx + dy * dy > 0x7FA01) {
        ret = 4;
    }
    if (behind) return ret | 2;
    return ret | 1;
}

void morph_light_colors(s32 p, MeshHeader *model, SVECTOR *src, SVECTOR *fc) {
    SVECTOR *dst = MorphVerts;
    u8 *run = (u8 *)&model->runs + model->runs;
    s32 r, g, b;
    s32 i;

    r = fc->vx;
    g = fc->vy;
    b = fc->vz;
    for (; run[1] != 0xFF; run += 2) {
        i = run[1] + 2;
        while (i-- > 0) {
            vector_towards(src, r, g, b, p, dst);
            fc++;
            dst->pad = src->pad;
            src++;
            r = fc->vx;
            g = fc->vy;
            b = fc->vz;
            dst++;
        }
    }
}

void morph_blend_verts(s32 p, MeshHeader *model, u16 *ia, u16 *ib, s16 (*verts)[3]) {
    SVECTOR *dst = MorphVerts;
    u8 *run;
    u32 a;
    u32 b;
    s16 *va;
    s16 *vb;
    s32 flag;
    s32 i, n;
    s32 r, g, bl;

    a = *ia;
    b = *ib;
    run = (u8 *)model + model->runs + 0x15;
    for (;;) {
        n = *run;
        if (n == 0xFF) {
            return;
        }
        i = n + 2;
        while (i-- > 0) {
            va = verts[a >> 2];
            vb = verts[b >> 2];
            flag = b & 3;
            r = vb[0];
            g = vb[1];
            bl = vb[2];
            vector_towards(va, r, g, bl, p, dst);
            ia++;
            ib++;
            b = *ib;
            a = *ia;
            dst->pad = flag;
            dst++;
        }
        run += 2;
    }
}

void morph_copy_verts(MeshHeader *arg0, u16 *src, u8 *tab, u16 *dst) {
    u8 *p;
    s32 n;
    s32 c;
    s32 prev;
    u16 v;
    u16 *e;

    s32 off = arg0->runs;
    u8 *b = (u8 *)arg0 + off;
    p = b + 0x15;
    for (;;) {
        c = *p;
        if (c == 0xFF) {
            return;
        }
        n = c + 1;
        if (c + 2 > 0) {
            do {
                v = *src++;
                prev = n;
                e = (u16 *)(tab + (v >> 2) * 6);
                n--;
                *dst = e[0];
                dst[1] = e[1];
                dst[2] = e[2];
                dst[3] = v & 3;
                dst += 4;
            } while (prev > 0);
        }
        p += 2;
    }
}

void eng_kill(EXTengine *eng) {
    eng->kill(eng);
}

void eng_render(EXTengine *eng) {
    eng->render(eng);
}

void eng_render_subs(EXTengine *e) {
    void (*hook)(subengine *, void *, u32);
    s32 flags;
    MATRIX *m;
    subengine *sub;

    sub = e->sub;
    RenderState.unk4 = e;
    if (sub != NULL) {
        flags = e->flags;
        if (flags & 0x8000) {
            m = gfx_object_matrix((GfxXform *)&e->flags);
loop_3:
            if (sub != NULL) {
                hook = sub->hook;
                if (hook != NULL) {
                    draw_owner(sub);
                    hook(sub, m, flags);
                    draw_owner(NULL);
                }
                sub = sub->next;
                goto loop_3;
            }
        }
    }
}

subengine *sub_nth(s32 n, subengine *sub) {
    while (sub != NULL && n != 0) {
        sub = sub->next;
        n--;
    }
    return sub;
}


void kill_sprim_sub(subengine *arg0) {
    if (arg0->sprim != NULL) {
        release_sprim(arg0->sprim);
        arg0->sprim = NULL;
    }
    if (arg0->sprim2 != NULL) {
        release_sprim(arg0->sprim2);
        arg0->sprim2 = NULL;
    }
}

void sub_use_quad_model(subengine *arg0) {
    MeshHeader *temp_v0;
    u16 temp_a0;

    temp_v0 = (MeshHeader *)entity_address(arg0->entity, arg0->u6C.dbase->data);
    temp_a0 = temp_v0->faces;
    if (temp_a0 == 2 && ((u8 *)&temp_v0->runs + temp_v0->runs)[1] == temp_a0) {
        arg0->hook = draw_model_quad;
        arg0->release = kill_sprim_sub;
    }
}

void posdst_flash(PosDst *arg0, MATRIX *arg1, s32 arg2) {
    PosEntry *e;
    s32 sp10;
    s32 sp14;

    e = &PosFlashes[arg0->idx];
    switch (e->mode) {
    case 1:
        if (FrameCount & 4) {
    case 0:
            arg0->x = e->a[0];
            arg0->y = e->a[1];
            arg0->z = e->a[2];
        } else {
            arg0->x = e->b[0];
            arg0->y = e->b[1];
            arg0->z = e->b[2];
        }
        break;
    case 2:
        sp10 = e->a[1];
        sp14 = e->a[2];
        TextFlashColour(&sp10, &sp14);
        arg0->x = e->b[0];
        arg0->y = sp10;
        arg0->z = sp14;
        break;
    }
    draw_text_nas((TextNAS *)arg0, arg1, arg2);
}

void posdst_flash_at(PosDst *arg0, s32 arg1) {
    PosEntry *e;
    s32 sp10;
    s32 sp14;

    e = &PosFlashes[arg1];
    switch (e->mode) {
    case 1:
        if (FrameCount & 4) {
    case 0:
            arg0->x = e->a[0];
            arg0->y = e->a[1];
            arg0->z = e->a[2];
            return;
        }
        arg0->x = e->b[0];
        arg0->y = e->b[1];
        arg0->z = e->b[2];
        return;
    case 2:
        sp10 = e->a[1];
        sp14 = e->a[2];
        TextFlashColour(&sp10, &sp14);
        arg0->x = e->b[0];
        arg0->y = sp10;
        arg0->z = sp14;
        return;
    }
}

void draw_nas_model(subengine *nas, MATRIX *arg1) {
    GteSetup s;
    u32 hidden;
    u32 rec[9];

    if (nas->flags & 0x8000) {
        hidden = nas->flags & 0x10000000;
        if (nas->flags & 0x02000000) {
            RenderState.otzAdd = nas->otz;
        } else {
            RenderState.otzAdd = 0;
        }
        if (!(nas->flags & 0x10000000)) {
            RenderState.localMtx = RenderState.mtxBuf;
            RenderState.localPos = RenderState.posBuf;
            gfx_setup_xform(nas, arg1, (VECTOR *)arg1->t, RenderState.screen, &s);
            view_set_centre(s.ofx * ScreenRect->w / 640, s.ofy / 2);
            if (s.trans != NULL) {
                view_set_translation_vector(s.trans);
            }
            view_set_rotation(s.rot);
        }
        /* the record it's kept as in the node, without the pointer's extra bytes */
        rec[0] = (u32)(iptr)nas->u6C.ptr;
        memcpy(&rec[1], &nas->u70, sizeof(rec) - 4);
        draw_panel_rect(rec, hidden);
    }
}

void draw_nas_model_flags(subengine *nas, MATRIX *arg1, u32 flags) {
    GteSetup s;
    u32 f;
    PanelPrim *pp = (PanelPrim *)nas;
    u32 rec[12];

    if (nas->flags & 0x8000) {
        f = flags | nas->flags;
        if (nas->flags & 0x02000000) {
            RenderState.otzAdd = nas->otz;
        } else {
            RenderState.otzAdd = 0;
        }
        if (!(nas->flags & 0x10000000)) {
            RenderState.localMtx = RenderState.mtxBuf;
            RenderState.localPos = RenderState.posBuf;
            gfx_setup_xform(nas, arg1, (VECTOR *)arg1->t, RenderState.screen, &s);
            view_set_centre(s.ofx * ScreenRect->w / 640, s.ofy / 2);
            if (s.trans != NULL) {
                view_set_translation_vector(s.trans);
            }
            view_set_rotation(s.rot);
        }
        f &= ~0x10000000;
        /* the corners and colours the panels keep in the node, as a plain record */
        rec[0] = *(u32 *)&pp->u6C.s;
        memcpy(&rec[1], &pp->z0, 8 * 4);
        rec[9] = pp->c1;
        rec[10] = pp->c2;
        rec[11] = pp->c3;
        draw_gouraud_quad(rec, f | nas->flags);
    }
}

u32 draw_nas_model_ret(subengine *nas, MATRIX *arg1, u32 flags) {
    GteSetup s;

    flags |= nas->flags;
    if (nas->flags & 0x02000000) {
        RenderState.otzAdd = nas->otz;
    } else {
        RenderState.otzAdd = 0;
    }
    if (!(nas->flags & 0x10000000)) {
        RenderState.localMtx = RenderState.mtxBuf;
        RenderState.localPos = RenderState.posBuf;
        gfx_setup_xform(nas, arg1, (VECTOR *)arg1->t, RenderState.screen, &s);
        view_set_centre(s.ofx * ScreenRect->w / 640, s.ofy / 2);
        if (s.trans != NULL) {
            view_set_translation_vector(s.trans);
        }
        view_set_rotation(s.rot);
    }
    flags &= ~0x10000000;
    return flags | nas->flags;
}

void draw_flat_rect(u16 *arg0, s32 arg1, u32 arg2) {
    QuadVerts q;
    s32 x;
    s32 w;

    w = (s16) arg0[3];
    x = (s16) arg0[0];
    if ((x & 0xC000) == 0x8000) {
        if (x & 0x3FFF) {
            x = (x & ~0xC000) - (w / 2);
        } else {
            x = 0x140 - (w / 2);
            x -= ((GfxScreen *)RenderState.screen)->x;
        }
        w += x;
    }
    q.v[0].x = x;
    q.v[0].y = arg0[1];
    q.v[0].z = arg0[2];
    q.v[1].x = w;
    q.v[1].y = arg0[1];
    q.v[1].z = arg0[2];
    q.v[2].x = x;
    q.v[2].y = arg0[4];
    q.v[2].z = arg0[2];
    q.v[3].x = w;
    q.v[3].y = arg0[4];
    q.v[3].z = arg0[2];
    q.c[0] = arg1;
    q.c[1] = arg1;
    q.c[2] = arg1;
    q.c[3] = arg1;
    draw_gouraud_quad(&q, arg2);
}

void calc_rgbcode_packed(u32 arg0) {
    calc_rgbcode((arg0 >> 0x10) & 0xFF, (arg0 >> 8) & 0xFF, arg0 & 0xFF);
}

void sub_release_none(void) {
}


void gfx_load_xform(void *arg0, void *arg1, void *arg2) {
    GteSetup s;

    RenderState.localMtx = RenderState.mtxBuf;
    RenderState.localPos = RenderState.posBuf;
    gfx_setup_xform(arg0, arg1, arg2, RenderState.screen, &s);
    view_set_centre(s.ofx * ScreenRect->w / 640, s.ofy / 2);
    if (s.trans != NULL) {
        view_set_translation_vector(s.trans);
    }
    view_set_rotation(s.rot);
}

void draw_model_set_entry(s32 arg0, ModelFile *set, s32 arg2) {
    void *p;
    u8 *blk;
    MeshHeader *e;

    arg0 &= 0xFFF;
    if (set->nMeshes >= arg0) {
        e = &set->meshes[arg0 - 1];
        blk = (u8 *)&e->verts + e->verts;
        if (!(arg2 & 0x1000000) && !bbox_visible(blk)) {
            return;
        }
        do {
            p = model_prims_begin(e, arg2);
        } while (0);
        blk += 0x14;
        if (p != NULL) {
            draw_verts(p, (SVECTOR *)blk, (u8 *)&e->runs + e->runs);
        }
    }
}
void draw_map_entity(u16 arg0, u8 *arg1, s32 arg2) {
    void *p;
    u8 *blk;
    MeshHeader *e;

    e = (MeshHeader *)map_entity_address(arg0, arg1);
    if (e == NULL) {
        return;
    }
    blk = (u8 *)&e->verts + e->verts;
    if (!(arg2 & 0x1000000) && !bbox_visible(blk)) {
        return;
    }
    p = model_prims_begin(e, arg2);
    blk += 0x14;
    if (p != NULL) {
        draw_verts(p, (SVECTOR *)blk, (u8 *)&e->runs + e->runs);
    }
}

void *draw_model_entry(MeshHeader *e, s32 arg1) {
    u8 *blk;
    void *p;
    u8 *q;

    blk = (u8 *)&e->verts + e->verts;
    if ((arg1 & 0x1000000) == 0) {
        if (bbox_visible(blk) == 0) {
            return NULL;
        }
    }
    q = blk + 0x14;
    p = model_prims_begin(e, arg1);
    if (p != NULL) {
        draw_verts(p, (SVECTOR *)q,(u8 *)&e->runs + e->runs);
    }
    return p;
}

void *draw_morph_model(MeshHeader *model, void *arg1, s32 arg2) {
    void *p;

    p = model_prims_begin(model, arg2);
    if (p != NULL) {
        draw_verts(p, arg1, (u8 *)&model->runs + model->runs);
    }
    return p;
}

void eng_release_subs_sfx(EXTengine *eng) {
    void (*release)(subengine *);
    _sfx_handler *voice;
    subengine *next;
    subengine *first;
    subengine *last;
    subengine *freeList;
    subengine *sub;

    voice = eng->sfx;
    if (voice != 0) {
        StopSfx(voice);
    }
    first = eng->sub;
    last = eng->last;
    sub = first;
    if (first != NULL) {
        do {
            release = sub->release;
            next = sub->next;
            if (release != NULL) {
                release(sub);
            }
            sub->hook = NULL;
            sub->release = NULL;
            sub->flags = 0;
            sub = next;
        } while (sub != NULL);
        freeList = next_sub;
        next_sub = first;
        last->next = freeList;
    }
    eng->sub = NULL;
    eng->num_subs = 0;
}

void eng_release_subs(EXTengine *eng) {
    void (*release)(subengine *);
    subengine *next;
    subengine *first;
    subengine *last;
    subengine *freeList;
    subengine *sub;

    first = eng->sub;
    last = eng->last;
    sub = first;
    if (first != NULL) {
        do {
            release = sub->release;
            next = sub->next;
            if (release != NULL) {
                release(sub);
            }
            sub->hook = NULL;
            sub->release = NULL;
            sub->flags = 0;
            sub = next;
        } while (sub != NULL);
        freeList = next_sub;
        next_sub = first;
        last->next = freeList;
    }
    eng->sub = NULL;
    eng->num_subs = 0;
}

void subs_release(subengine *first, subengine *last) {
    void (*release)(subengine *);
    subengine *next;
    subengine *freeList;
    subengine *sub;

    if (first != NULL) {
        sub = first;
        do {
            release = sub->release;
            next = sub->next;
            if (release != NULL) {
                release(sub);
            }
            sub->hook = NULL;
            sub->release = NULL;
            sub->flags = 0;
            sub = next;
        } while (sub != NULL);
        freeList = next_sub;
        next_sub = first;
        last->next = freeList;
    }
}

void kill_subs(subengine *first) {
    void (*release)(subengine *);
    subengine *next;
    subengine *sub;

    sub = first;
    if (sub != NULL) {
        do {
            release = sub->release;
            next = sub->next;
            if (release != NULL) {
                release(sub);
            }
            sub->hook = NULL;
            sub->release = NULL;
            sub->flags = 0;
            sub = next;
        } while (sub != NULL);
    }
}

s32 eng_add_subs_n(EXTengine *eng, s32 n) {
    return eng_add_subs(eng, n);
}

void subs_init_array(EXTengine *eng, s32 n, subengine *sub) {
    if (eng != NULL) {
        eng->sub = sub;
        eng->num_subs = n;
    }
    *sub = SubEngineTemplate;
    n--;
    sub->prev = NULL;
    while (n != 0) {
        sub->next = sub + 1;
        sub++;
        *sub = SubEngineTemplate;
        n--;
        sub->prev = sub - 1;
    }
    sub->next = NULL;
}

s32 subs_alloc_list(subengine **out_first, subengine **last, s32 n) {
    subengine *sub;
    subengine *prev;
    s32 count;

    count = 0;
    sub = next_sub;
    if (sub != NULL && n > 0) {
        prev = *last;
        if (prev == NULL) {
            prev = sub;
            *out_first = sub;
            sub->prev = NULL;
            SubEngineTemplate.next = sub->next;
            SubEngineTemplate.prev = sub->prev;
            *sub = SubEngineTemplate;
            n--;
            count++;
            sub = sub->next;
        }
        while (n != 0) {
            if (sub == NULL) {
                break;
            }
            prev->next = sub;
            sub->prev = prev;
            prev = sub;
            SubEngineTemplate.next = sub->next;
            SubEngineTemplate.prev = sub->prev;
            *sub = SubEngineTemplate;
            n--;
            sub = sub->next;
            count++;
        }
        prev->next = NULL;
        *last = prev;
        next_sub = sub;
    }
    return count;
}

void draw_init(void) {
    RenderState.alignMtx.m[0][0] = 0x1000;
    RenderState.alignMtx.m[0][1] = 0;
    RenderState.alignMtx.m[0][2] = 0;
    RenderState.alignMtx.m[1][0] = 0;
    RenderState.alignMtx.m[1][1] = 0x1000;
    RenderState.alignMtx.m[1][2] = 0;
    RenderState.alignMtx.m[2][0] = 0;
    RenderState.alignMtx.m[2][1] = 0;
    RenderState.alignMtx.m[2][2] = 0x1000;
}


void subpool_free(void) {
    if (SubPool != 0) {
        mal_free(SubPool, &MainHeap);
        SubPool = 0;
    }
}

void subpool_init(s32 n) {
    subengine *sub;
    s32 i;
    u8 *p;

    p = mal_alloc(n * sizeof(subengine), &MainHeap);
    i = n - 1;
    sub = (subengine *)p;
    SubPool = sub;
    next_sub = sub;
    if (i > 0) {
        do {
            subengine *nx = sub + 1;
            sub->next = nx;
            i--;
            sub = nx;
        } while (i > 0);
    }
    sub->next = NULL;
}

void mapenv_place_subs(map_enviroment *env) {
    subengine *sub;
    Placement *p;
    s32 n;

    sub = MapEnv_GetSubs(env);
    n = env->num_placements;
    p = env->placements;
    env->subs = sub;
    while (--n != -1) {
        *sub = SubEngineTemplate;
        sub->flags = p->flags;
        *(SLONG_XYZ *)&sub->XYZ.x = p->pos;
        *(PlacementBlk *)&sub->xform.mtx = p->blk;
        sub->hook = (void *)draw_map_sub;
        sub->u6C.dbase = env->dbase;
        sub->entity = p->entity;
        sub->depth = p->depth;
        sub->otz = p->otz;
        sub->ids[0] = p->ids[0];
        sub->ids[1] = p->ids[1];
        sub->ids[2] = p->ids[2];
        sub->ids[3] = p->ids[3];
        if (n > 0) {
            sub->next = sub + 1;
        } else {
            sub->next = NULL;
        }
        p++;
        sub++;
    }
}

void draw_map_object(void *sub, void *arg1, s32 flags, void *arg3) {
    MATRIX m;
    VECTOR v;
    MATRIX *rot;
    MATRIX *mp;

    m = RenderState.cam->view;
    RenderState.dbase = ((subengine *)sub)->u6C.dbase = ((obj_dbase *)MapScene->dbase);
    v.vx = ((subengine *)sub)->XYZ.x - RenderState.cam->at.x;
    v.vy = ((subengine *)sub)->XYZ.y - RenderState.cam->at.y;
    v.vz = ((subengine *)sub)->XYZ.z - RenderState.cam->at.z;
    mp = &m;
    if (!(flags & 0x200000)) {
        rot = &RenderState.cam->view;
    } else {
        rot = &MapSceneMatrix;
    }
    matrix_apply_long(rot, &v, (VECTOR *)m.t);
    m.t[0] += RenderState.cam->view.t[0];
    m.t[1] += RenderState.cam->view.t[1];
    m.t[2] += RenderState.cam->view.t[2];
    if (((subengine *)sub)->flags & 0x20000000) {
        view_set_rotation(&RenderState.cam->rot);
    } else {
        if (*(u16 *)&((subengine *)sub)->xform.dirty != 0) {
            rotscale_update(&((subengine *)sub)->xform.rot.vx);
        }
        matrix_mul(rot, &((subengine *)sub)->xform.mtx, mp);
        view_set_rotation(mp);
    }
    view_set_translation(mp);
    RenderState.otzAdd = *(u16 *)(RenderState.screen + 8) + ((subengine *)sub)->otz;
    view_set_centre(RenderState.ofx * ScreenRect->w / 640, RenderState.ofy / 2);
}

void draw_map_scene(map_enviroment *scene) {
    MATRIX m;
    MATRIX view;
    MATRIX *pv;
    subengine *sub;
    void (*fn)(subengine *, void *, u32);

    MapScene = scene;
    matrix_rotation_xyz(&MapSceneRot, &m);
    matrix_mul(&RenderState.cam->view, &m, &MapSceneMatrix);
    pv = &view;
    view = RenderState.cam->view;
    sub = scene->subs;
    while (1) {
        if (sub == NULL) {
            break;
        }
        fn = sub->hook;
        if (fn != NULL && !wide_scenery_hides(scene, sub)) {
            draw_owner(sub);
            fn(sub, pv, 0);
            draw_owner(NULL);
        }
        sub = sub->next;
    }
    wide_scenery_draw(scene, pv);
    GfxSetSceneBg(&RenderState, scene);
}

subengine *mapscene_find_sub(s32 i, s32 val) {
    subengine *sub;

    for (sub = MapScene->subs; sub != NULL; sub = sub->next) {
        if (sub->ids[i] == val) {
            return sub;
        }
    }
    return NULL;
}

/* An entity's mesh: arg0[0] its offset in a block, and arg0[1] leads to the slot with the block in. */
void *rel_ptr(s32 *arg0) {
    ref32 v;

    v = ((ref32 *)((u8 *)arg0 + arg0[1]))[1];
    if (v.off == 0) {
        return NULL;
    }
    return (u8 *)ref_get(v) + arg0[0];
}


extern void draw_map_object(void *sub, void *arg1, s32 flags, void *arg3);
void draw_map_sub(subengine *sub, MATRIX *arg1, s32 arg2) {
    s32 flags;
    u8 *x;

    flags = sub->flags;
    x = (u8 *)sub->u6C.dbase->data;
    if ((flags & 0x8000) && x != NULL) {
        draw_map_object(sub, arg1, flags, x);
        draw_object_model(sub->entity, x, sub->flags, sub);
    }
}

void map_update(map_enviroment *env, SLONG_XYZ *at) {
    texture_update_anims(&env->dbase->textures);
}

void mapscene_release(map_enviroment *env) {
    dbase_release((u8 *)env->dbase);
    mal_free(env->subs, &MainHeap);
    env->subs = NULL;
}

void mapscene_setup_tables(map_enviroment *env) {
    obj_dbase *outer;
    ModelFile *p;
    map_list *maps;
    map_map *q;
    u8 *r;
    ModelFile *s;
    ModelFile *t;
    ModelFile *u;
    s32 idx;

    outer = env->dbase;
    idx = env->map_num;
    p = outer->data;
    maps = (map_list *)((u8 *)&p->maps + p->maps);
    env->maps = maps;
    q = (map_map *)((u8 *)&maps->map_offset[idx] + maps->map_offset[idx]);
    env->map = q;
    r = (u8 *)&q->path_list + q->path_list;
    env->path_list = r;
    env->path = (u32 *)(r + 4);
    env->triggers = (trigger_list *)((u8 *)&q->trigger_list + q->trigger_list);
    env->placements = (struct Placement *)((u8 *)&q->placements + q->placements);
    env->num_placements = q->num_placements;
    s = outer->data;
    env->map_directory = (u8 *)&s->entities + s->entities;
    t = outer->data;
    env->map_animations = (u8 *)&t->mapAnims + t->mapAnims + 8;
    u = outer->data;
    ((DbaseAnim *)((u8 *)&u->anims + u->anims + 4))->data = ref_of(u);
}

void mapscene_init(map_enviroment *env, obj_dbase *db) {
    env->dbase = db;
    env->map_num = 0;
    MapScene = env;
    if (db == NULL) {
        env->triggers = NULL;
        env->flags &= ~3;
        return;
    }
    env->flags = 3;
    textureset_clear(&db->textures);
    load_file_cb(db->textureFile, map_loaded_textureset, env);
}

void map_loaded_textureset(u8 *arg0, u8 **arg1) {
    obj_textureset *ts;
    obj_dbase *db;

    db = ((map_enviroment *)arg1)->dbase;
    mal_set_owner(arg0, (void **)&db->textures.base);
    ts = &db->textures;
    textureset_init(ts, (u8 *)ts->base);
    textureset_upload(ts);
    texture_release_loading_mem(ts);
    db->load = loader_queue(db->file, 0, db->textures.f14, map_loaded_geo, (u8 *)arg1, 0, 0);
}

void map_loaded_geo(u8 *arg0, map_enviroment *env) {
    obj_dbase *db;
    obj_dbase *t;

    t = env->dbase;
    MapScene = env;
    t->data = (ModelFile *)arg0;
    db = env->dbase;
    env->map_num = 0;
    mapscene_setup_tables(env);
    env->subs = NULL;
    mapenv_place_subs(env);
    dbase_load_all_mesh_anims((s32 *)db);
}

void mapscene_load(map_enviroment *env, obj_dbase *db) {
    env->dbase = db;
    env->map_num = 0;
    MapScene = env;
    mapscene_setup_tables(env);
    env->subs = NULL;
    mapenv_place_subs(env);
    dbase_load_all_mesh_anims(db);
}

void mapscene_place_subs(map_enviroment *env) {
    subengine *dst;
    Placement *src;
    s32 n;
    s32 end;

    dst = env->subs;
    src = env->placements;
    n = env->num_placements;
    n--;
    if (n != -1) do {
        *dst = SubEngineTemplate;
        dst->flags = src->flags;
        *(Words3 *)&dst->XYZ.x = *(Words3 *)&src->pos;
        *(Words4 *)&dst->xform.mtx = *(Words4 *)&src->blk.w[0];
        *(Words4 *)&dst->xform.mtx.m[2][2] = *(Words4 *)&src->blk.w[4];
        dst->hook = (void (*)(subengine *, void *, u32))draw_map_sub;
        dst->u6C.dbase = env->dbase;
        dst->entity = src->entity;
        dst->depth = src->depth;
        dst->otz = src->otz;
        dst->ids[0] = src->ids[0];
        dst->ids[1] = src->ids[1];
        dst->ids[2] = src->ids[2];
        dst->ids[3] = src->ids[3];
        if (n > 0) {
            dst->next = dst + 1;
        } else {
            dst->next = NULL;
        }
        src++;
        dst++;
        n--;
        end = -1;
    } while (n != end);
}

static inline KeyEntry *key_interp_find(KeyBlock *arg0, s32 arg1) {
    KeyEntry *p;
    KeyEntry *q;

    if (arg1 < 0 || (p = arg0->entries, arg0->count < arg1)) {
        return NULL;
    }
    q = p;
    if (arg1 >= p->key) {
        do {
            if (q->flags & 0x2000) {
                return p;
            }
            q++;
            p++;
        } while (arg1 >= q->key);
    }
    return p - 1;
}

KeyEntry *key_interp_pos(KeyBlock *arg0, s32 arg1, SLONG_XYZ *out) {
    KeyEntry *p;
    SLONG_XYZ d;
    s32 span;
    s32 dt;

    p = key_interp_find(arg0, arg1);
    if (p != NULL) {
        if (p->flags & 0x2000) {
            *out = p->pos;
        } else {
            dt = arg1 - p->key;
            span = p[1].key - p->key;
            d.x = p[1].pos.x - p->pos.x;
            d.y = p[1].pos.y - p->pos.y;
            d.z = p[1].pos.z - p->pos.z;
            d.x *= dt;
            d.y *= dt;
            d.z *= dt;
            d.x /= span;
            d.y /= span;
            d.z /= span;
            out->x = p->pos.x + d.x;
            out->y = p->pos.y + d.y;
            out->z = p->pos.z + d.z;
        }
    }
    return p;
}

KeyEntry *key_find(KeyBlock *arg0, s32 arg1) {
    KeyEntry *p;
    KeyEntry *q;

    if (arg1 < 0 || (p = arg0->entries, arg0->count < arg1)) {
        return NULL;
    }
    q = p;
    if (arg1 >= p->key) {
        do {
            if (q->flags & 0x2000) {
                return p;
            }
            q++;
            p++;
        } while (arg1 >= q->key);
    }
    return p - 1;
}

s32 *path_get(u32 arg0) {
    s32 *p;
    s32 n;
    s32 i;
    s32 *q;

    p = NULL;
    if (MapScene->dbase != NULL) {
        p = ((s32 *)MapScene->path_list);
    }
    i = 0;
    if (p == NULL) {
        return NULL;
    }
    n = *p;
loop:
    p++;
    if (i < n) {
        if (*(s32 *)(SELF_REL(p) + 0xC) == arg0) {
            goto found;
        }
        i++;
        goto loop;
    }
    i = -1;
found:
    if (i >= 0) {
        q = (s32 *)(((u8 *)MapScene->path) + (i << 2));
        return (s32 *)SELF_REL(q);
    }
    return NULL;
}

s32 path_find(u32 arg0) {
    s32 *p;
    s32 n;
    s32 i;

    p = NULL;
    if (MapScene->dbase != NULL) {
        p = ((s32 *)MapScene->path_list);
    }
    i = 0;
    if (p == NULL) {
        return -1;
    }
    n = *p;
loop:
    p++;
    if (i < n) {
        if (*(s32 *)(SELF_REL(p) + 0xC) == arg0) {
            return i;
        }
        i++;
        goto loop;
    }
    return -1;
}

void set_fade(fade_info *fade, s32 speed, s32 delay) {
    fade->speed = speed;
    fade->delay = delay;
    fade->maxed = 0;
}

s32 fade_update(fade_info *fade) {
    s32 step;
    s32 level;

    step = fade->speed;
    if (step != 0) {
        level = fade->amount + step;
        fade->amount = level;
        if (fade->speed > 0) {
            if (level >= 0x1000) {
                fade->amount = 0x1000;
                fade->maxed += 1;
            }
        } else if (level <= 0) {
            fade->amount = 0;
            fade->maxed += 1;
            if (fade->maxed >= 3) {
                fade->speed = 0;
            }
        }
    }
    return fade->amount;
}

void process_control_update(process_control_info *pc, fade_info *fade) {
    if (pc->next_process != NULL && fade != NULL && (fade->speed == 0 || fade->maxed >= 3)) {
        if (pc->process != NULL) {
            if (!(pc->flags & 1) && pc->process->quit != NULL) {
                pc->process->quit();
            }
            if (fade != NULL && fade->speed != 0 && fade->maxed >= 3) {
                prims_empty();
                while (mal_update_deferred(&MainHeap) != NULL) {
                }
                mal_compact(&MainHeap);
            }
        }
        pc->last_process = pc->process;
        pc->process = pc->next_process;
        pc->next_process = NULL;
        if (pc->process->heap != NULL) {
            sound_stop();
            while (loader_pending() != 0) {
                loader_update();
                mal_compact(&MainHeap);
                vab_transfer_check();
            }
            mal_init(&MainHeap, (u8 *)pc->process->heap, HEAP_END - (u8 *)pc->process->heap);
            smallnode_init();
        }
        ((GameState *)GameInfo)->exitFlags &= ~1;
        if (!(pc->flags & 2)) {
            if (pc->flags & 0x10) {
                pc->flags = 0;
            }
            if (pc->process->init != NULL) {
                pc->process->init();
            }
        } else if (pc->flags & 0x10) {
            pc->flags = 0;
        }
    }
    if (pc->process != NULL && !(pc->flags & 8) && pc->process->update != NULL) {
        pc->process->update();
    }
}

void process_control_back(process_control_info *pc) {
    pc->flags = 2;
    pc->next_process = pc->last_process;
}

void process_interrupt(process_control_info *pc, process_info *process) {
    pc->next_process = process;
    pc->flags = 1;
}

void process_control_goto(process_control_info *pc, process_info *process) {
    pc->next_process = process;
    pc->flags = 0;
}

void process_quit(process_control_info *pc) {
    process_info *p;
    void (*fn)(void);

    if (pc != NULL) {
        p = pc->process;
        if (p != NULL) {
            fn = p->quit;
            if (fn != NULL) {
                fn();
            }
        }
    }
}

void process_control_render(process_control_info *pc) {
    process_info *p;
    void (*fn)(void);

    if (pc != NULL) {
        p = pc->process;
        if (p != NULL) {
            fn = p->render;
            if (fn != NULL && !(pc->flags & 4)) {
                fn();
            }
        }
    }
}

void process_init(process_control_info *pc) {
    process_info *p;
    void (*fn)(void);

    if (pc != NULL) {
        p = pc->process;
        if (p != NULL) {
            fn = p->init;
            if (fn != NULL) {
                fn();
            }
        }
    }
}

void process_control_init(process_control_info *pc, process_info *process) {
    pc->process = process;
    pc->next_process = NULL;
    pc->last_process = NULL;
    pc->flags = 0;
}

void prims_empty_and_compact(void) {
    prims_empty();
    do {

    } while (mal_update_deferred(&MainHeap) != 0);
    mal_compact(&MainHeap);
}

/* data, in address order (tools/scripts/data2c.py) */
s32 *PrimsConfig = 0;
u8 *PrimNodePool = 0;
NodePool PrimPool = {0};
PrimLink *PrimNodeFree = 0;
u8 *PrimLinkPool = 0;
PrimLink *PrimLinkFree = 0;
u8 *PrimSetPool = 0;
PrimSet *PrimSetFree = 0;
PrimSet *PrimSetList = 0;
s32 FarColor[3] = {0};
s32 PrimUnused[1] = {0};
s32 PrimDepthTest = 0;
s32 GameSceneCounts[4] = {0};
VECTOR ScreenTrans = { 0, 0, 0x680, 0 };
PosEntry PosFlashes[9] = {
    { { 0, 0xD0FF, 0xFF }, 0, { 0, 0, 0 } }, { { 0, 0x808080, 0x808080 }, 0, { 0, 0, 0 } },
    { { 0, 0xD0FF, 0xFF }, 2, { 0, 0, 0 } }, { { 0, 0x606060, 0x606060 }, 0, { 0, 0, 0 } },
    { { 1, 0xD0FF, 0xFF }, 0, { 0, 0, 0 } }, { { 0, 0xFF, 0xFF }, 0, { 0, 0, 0 } },
    { { 1, 0xFF, 0xFF }, 0, { 0, 0, 0 } }, { { 0, 0xFFFFFF, 0xFFFF }, 0, { 0, 0, 0 } },
    { { 1, 0xFFFFFF, 0xFFFF }, 0, { 0, 0, 0 } },
};
GfxState RenderState = {0};
s32 PrimDepthLimit = 0;
SVECTOR MorphVerts[1024] = {0};
void *SubPool = 0;
subengine *next_sub = 0;
map_enviroment *MapScene = 0;
MATRIX MapSceneMatrix = {0};
SVECTOR MapSceneRot = {0};
s32 map_frame_counter[1] = {0};  /* the demo's name; read nowhere */
