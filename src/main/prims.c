#include "common.h"
#include "bash/main.h"
#include "bash/prims.h"

static inline void func_80017164_clear(u8 *p) {
    *(s32 *)(p + 4) = 0;
    *(s32 *)(p + 8) = 0;
    *(s32 *)(p + 0xC) = 0;
    *(s32 *)(p + 0x10) = 0;
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
    PrimSetPool = mal_alloc((u32)(cnt * 0x2C0) >> 3, &MainHeap);
    PrimNodePool = mal_alloc((u32)(cfg[2] * 0x2A0) >> 3, &MainHeap);
    q = mal_alloc((u32)(cfg[1] << 6) >> 3, &MainHeap);
    PrimLinkPool = q;
    p = PrimSetPool;
    next = PrimNodePool;
    PrimNodeFree = (s32 *)next;
    n = cfg[2] - 2;
    if (n != -1) {
        do {
            *(u8 **)next = next + 0x54;
            next += 0x54;
            n--;
            end1 = -1;
        } while (n != end1);
    }
    *(u8 **)next = NULL;
    n = cfg[1] - 2;
    if (n != -1) {
        do {
            *(s32 *)(q + 4) = 0;
            *(u8 **)q = q + 8;
            q += 8;
            n--;
            end2 = -1;
        } while (n != end2);
    }
    *(s32 *)(q + 4) = 0;
    *(u8 **)q = NULL;
    end = -1;
    n = cfg[0] + end;
    PrimLinkFree = (s32 *)PrimLinkPool;
    for (;;) {
        u8 *np;

        n--;
        if (n == end) {
            break;
        }
        np = p + 0x58;
        *(u8 **)p = np;
        func_80017164_clear(p + 4);
        func_80017164_clear(p + 0x18);
        func_80017164_clear(p + 0x2C);
        func_80017164_clear(p + 0x40);
        p = np;
    }
    func_80017164_clear(p + 4);
    func_80017164_clear(p + 0x18);
    func_80017164_clear(p + 0x2C);
    func_80017164_clear(p + 0x40);
    PrimSetList = NULL;
    *(u8 **)p = NULL;
    PrimPool.mem = NULL;
    PrimSetFree = (s32 *)PrimSetPool;
}

static inline void PrimList_Flush(PrimList *arg0, s32 arg1) {
    arg0->f0 = 0;
    if (arg1 == 1) {
        primnodes_free(arg0->fC);
    } else {
        *arg0->f8 = PrimNodeFree;
        PrimNodeFree = arg0->fC;
        if (arg0->f10 != NULL) {
            *arg0->f10 = NULL;
        } else {
            arg0->f4 = 0;
        }
        arg0->f8 = arg0->f10;
    }
    if (arg0->f10 != NULL) {
        *arg0->f10 = NULL;
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
    set->next = (PrimSet *) PrimSetFree;
    PrimSetFree = (s32 *) set;
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
        p = (PrimSet *) ((u8 *) set + off);
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
        p = (PrimSet *) ((u8 *) set + off);
        if (p->pairs[0].b.fC != NULL) {
            if (++p->pairs[0].b.f0 >= 16) {
                PrimList_Flush(&p->pairs[0].b, set->mode);
            }
        } else {
            p->pairs[0].b.f0 = 0;
        }
        q = (PrimSet *) ((u8 *) set + off);
        q->pairs[0].a.f10 = NULL;
        q->pairs[0].b.f10 = NULL;
        q->pairs[0].a.fC = (s32 *) q->pairs[0].a.f4;
        q->pairs[0].b.fC = (s32 *) q->pairs[0].b.f4;
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

static inline void PrimList_Empty(PrimList *arg0, u8 *node) {
    s32 kind;

    s32 *list;

    list = (s32 *) arg0->f4;
    kind = *(u16 *) (node + 0x54);
    arg0->f10 = NULL;
    arg0->fC = list;
    if (arg0->fC != NULL) {
        PrimList_Flush(arg0, kind);
    }
}

void prims_empty_back(void) {
    s32 idx;
    u8 *node;

    idx = (FrameCount & 1) ^ 1;
    DrawSync(0);
    node = PrimSetList;
    for (;;) {
        if (node == NULL) {
            return;
        }
        PrimList_Empty((PrimList *) (node + idx * 0x28 + 0x18), node);
        PrimList_Empty((PrimList *) (node + idx * 0x28 + 4), node);
        node = *(u8 **) node;
    }
}

void prim_colour_poly(RelTable *face, POLY_GT4 *poly, VECTOR *arg2, s32 mode) {
    u16 *idx;
    u32 *colors;
    u32 *c;
    s32 semi = mode & 1;
    s32 abr = mode & 0x60;

    gte_ldfcdir(RenderState.farColor[0], RenderState.farColor[1], RenderState.farColor[2]);
    gte_lddp(RenderState.dp);
    idx = (u16 *)((u8 *)&face->table + face->table);
    colors = (u32 *)((u8 *)&RenderState.unk0->colors->table + RenderState.unk0->colors->table);
    c = &colors[idx[0] & 0x1FFF];
    gte_ldrgb(c);
    gte_dpcs();
    gte_strgb(&poly->r0);
    c++;
    gte_ldrgb(c);
    gte_dpcs();
    gte_strgb(&poly->r1);
    c++;
    gte_ldrgb(c);
    gte_dpcs();
    gte_strgb(&poly->r2);
    setPolyGT4(poly);
    if (idx[0] & 0x8000) {
        *(u32 *)&poly->r0 = *(u32 *)&poly->r0 | 0x02000000;
        poly->tpage = (poly->tpage & 0xFF9F) | 0x20;
    } else if (semi) {
        *(u32 *)&poly->r0 = *(u32 *)&poly->r0 | 0x02000000;
        poly->tpage = (poly->tpage & 0xFF9F) | abr;
    } else {
        *(u32 *)&poly->r0 = *(u32 *)&poly->r0 & ~0x02000000;
        poly->tpage = (poly->tpage & 0xFF9F) | abr;
    }
    c = &colors[idx[1] & 0x1FFF];
    gte_ldrgb(c);
    gte_dpcs();
    gte_strgb(&poly->r3);
}

void prim_create_entity_single(u8 *arg0, POLY_GT4 *arg1) {
    GfxModel *model = RenderState.unk0;
    TexFaceRuns *face = (TexFaceRuns *)arg0;
    POLY_GT4 *poly = arg1;
    u16 *idx;
    u16 *uvIdx;
    u16 *tex;
    RelTable *mc;
    u32 *colors;
    u32 *c;
    u16 *uvs;
    u16 *uv;
    GfxTexInfo *ti;
    u16 base;
    s32 t;
    GfxTexSet *ts;

    uvIdx = (u16 *)((u8 *)&face->uvIdx + face->uvIdx);
    idx = (u16 *)((u8 *)&face->table + face->table);
    tex = (u16 *)((u8 *)&face->texs + face->texs);
    ts = &model->texs;
    mc = model->colors;
    colors = (u32 *)((u8 *)&mc->table + mc->table);
    uvs = (u16 *)((u8 *)&((UvTable *)mc)->uvs + ((UvTable *)mc)->uvs);
    c = &colors[*idx++ & 0x1FFF];
    *(u32 *)&poly->r0 = *c++;
    *(u32 *)&poly->r1 = c[0];
    *(u32 *)&poly->r2 = c[1];
    c = &colors[*idx & 0x1FFF];
    *(u32 *)&poly->r3 = *c;
    setPolyGT4(poly);
    t = *tex;
    if (t & 0x8000) {
        ti = &ts->tex[ts->count - 1].info;
        poly->clut = ts->cluts[t & 0x1FF].clut;
    } else {
        ti = &ts->tex[t & 0x1FF].info;
        poly->clut = ti->clut;
    }
    if (*idx & 0x8000) {
        *(u32 *)&poly->r0 |= 0x02000000;
    }
    poly->tpage = ti->tpage | 0x20;
    base = ti->uv;
    uv = &uvs[*uvIdx++];
    *(u16 *)&poly->u0 = base | uv[0];
    *(u16 *)&poly->u1 = base | uv[1];
    *(u16 *)&poly->u2 = base | uv[2];
    uv = &uvs[*uvIdx];
    *(u16 *)&poly->u3 = base | uv[2];
}

void faces_write_colors_dq(FaceRuns *faces, FacePrim *prim, s32 arg2, s32 mode) {
    u16 *idx;
    u8 *run;
    u8 *count;
    u32 *colors;
    u32 *c;
    s32 n;
    s32 semi = mode & 1;
    s32 abr = mode & 0x60;

    idx = (u16 *)((u8 *)&faces->table + faces->table);
    gte_ldfcdir(RenderState.farColor[0], RenderState.farColor[1], RenderState.farColor[2]);
    gte_lddp(RenderState.dp);
    run = (u8 *)&faces->runs + faces->runs;
    colors = (u32 *)((u8 *)&RenderState.unk0->colors->table + RenderState.unk0->colors->table);
    count = run + 1;
    for (;; count += 2, run += 2) {
        if (*count == 0xFF) {
            goto done;
        }
        n = *count;
        for (;;) {
            if (n-- == 0) {
                goto next;
            }
            c = &colors[*idx & 0x1FFF];
            if (run[0] & 1) {
                FacePrim *p = prim;

                gte_ldrgb(c);
                gte_dpcs();
                gte_strgb(&prim->g.g.r0);
                if (semi) {
                    *(u32 *)&prim->g.g.r0 |= 0x32000000;
                    *(u32 *)&prim->g.tp.code[0] = abr | 0xE1000000;
                } else if (*idx & 0x8000) {
                    *(u32 *)&prim->g.g.r0 |= 0x32000000;
                    *(u32 *)&prim->g.tp.code[0] = ((*idx >> 8) & 0x60) | 0xE1000000;
                } else {
                    *(u32 *)&prim->g.g.r0 |= 0x30000000;
                }
                c++;
                gte_ldrgb(c);
                gte_dpcs();
                gte_strgb(&p->g.g.r1);
                c++;
                gte_ldrgb(c);
                gte_dpcs();
                gte_strgb(&p->g.g.r2);
            } else {
                gte_ldrgb(c);
                gte_dpcs();
                gte_strgb(&prim->gt.r0);
                if (semi) {
                    *(u32 *)&prim->gt.r0 |= 0x36000000;
                    prim->gt.tpage = (prim->gt.tpage & ~0x60) | abr;
                } else if (*idx & 0x8000) {
                    *(u32 *)&prim->gt.r0 |= 0x36000000;
                    prim->gt.tpage = (prim->gt.tpage & ~0x60) | ((*idx >> 8) & 0x60);
                } else {
                    *(u32 *)&prim->gt.r0 |= 0x34000000;
                }
                c++;
                gte_ldrgb(c);
                gte_dpcs();
                gte_strgb(&prim->gt.r1);
                c++;
                gte_ldrgb(c);
                gte_dpcs();
                gte_strgb(&prim->gt.r2);
            }
            prim++;
            idx++;
        }
    next:;
    }
done:;
}

void faces_write_colors(FaceRuns *faces, FacePrim *prim) {
    u16 *idx;
    u8 *run;
    u8 *count;
    u32 *colors;
    u32 *c;
    s32 n;

    idx = (u16 *)((u8 *)&faces->table + faces->table);
    run = (u8 *)&faces->runs + faces->runs;
    colors = (u32 *)((u8 *)&RenderState.unk0->colors->table + RenderState.unk0->colors->table);
    count = run + 1;
    for (;; count += 2, run += 2) {
        if (*count == 0xFF) {
            goto done;
        }
        n = *count;
        for (;;) {
            if (n-- == 0) {
                goto next;
            }
            c = &colors[*idx & 0x1FFF];
            if (run[0] & 1) {
                FacePrim *p;

                *(u32 *)&prim->g.tp.code[0] = ((*idx >> 8) & 0x60) | 0xE1000000;
                p = prim;
                *(u32 *)&p->g.g.r0 = *c | ((*idx & 0x8000) ? 0x32000000 : 0x30000000);
                c++;
                *(u32 *)&p->g.g.r1 = c[0];
                *(u32 *)&p->g.g.r2 = c[1];
            } else {
                u32 code = *c | 0x34000000;

                *(u32 *)&prim->gt.r0 = code;
                if (*idx & 0x8000) {
                    *(u32 *)&prim->gt.r0 = code | 0x02000000;
                    prim->gt.tpage &= ~0x60;
                    prim->gt.tpage |= (*idx >> 8) & 0x60;
                }
                c++;
                *(u32 *)&prim->gt.r1 = c[0];
                *(u32 *)&prim->gt.r2 = c[1];
            }
            prim++;
            idx++;
        }
    next:;
    }
done:;
}

void prim_write_uvs(faces, prim)
    TexFaceRuns *faces;
    FacePrim *prim;
{
    u8 *p;
    s32 n;
    GfxTexSet *ts;
    s32 left;
    u32 t;
    u16 *blend;
    u16 *tex;
    u16 *uvIdx;
    u8 *run;
    u16 *uvs;
    u32 v;
    u32 idx;
    GfxTexInfo *ti;
    UvTable *ut;
    u16 base;

    left = 0;
    ts = &RenderState.unk0->texs;
    uvIdx = (u16 *)((u8 *)&faces->uvIdx + faces->uvIdx);
    tex = (u16 *)((u8 *)&faces->texs + faces->texs);
    ut = (UvTable *)RenderState.unk0->colors;
    blend = (u16 *)((u8 *)&faces->table + faces->table);
    run = (u8 *)&faces->runs + faces->runs;
    uvs = (u16 *)((u8 *)&ut->uvs + ut->uvs);
    for (;;) {
        if (run[1] == 0xFF) {
            return;
        }
        n = run[1];
        for (;;) {
            u16 *uv;
            u16 w;

            if (--n == -1) {
                break;
            }
            p = (u8 *)prim;
            if (--left < 0) {
                t = *tex++;
                left = (t >> 9) & 0x3F;
            }
            if (run[0] & 1) {
                p[3] = 8;
                *(u32 *)(p + 4) = 0xE1000200;
                p[15] = 0x30;
                *(u32 *)(p + 8) = 0;
            } else {
                v = t;
                if (v & 0x8000) {
                    ti = &ts->tex[ts->count - 1].info;
                    idx = v & 0x1FF;
                    *(u16 *)(p + 14) = ts->cluts[idx].clut;
                } else {
                    v = t & 0x1FF;
                    ti = &ts->tex[v].info;
                    *(u16 *)(p + 14) = ti->clut;
                }
                base = ti->uv;
                *(u16 *)(p + 0x1A) = (ti->tpage & 0xFF9F) | ((*blend >> 8) & 0x60);
                {
                    uv = &uvs[*uvIdx];
                    *(u16 *)(p + 12) = base | uv[0];
                    *(u16 *)(p + 0x18) = base | uv[1];
                    w = uv[2];
                    p[3] = 9;
                    *(u16 *)(p + 0x24) = base | w;
                    p[7] = 0x34;
                }
            }
            prim++;
            uvIdx++;
            blend++;
        }
        run += 2;
    }
}

FacePrim *face_prim_alloc(s32 arg0, PrimList *list, PrimList *pool, FaceRuns *faces, s32 arg4) {
    s32 *node;
    FacePrim *prim;

    node = PrimList_Take(pool);
    if (node != NULL) {
        *node = list->f4;
        list->f4 = (s32)node;
        if (list->f8 == NULL) {
            list->f8 = (s32 **)node;
        }
        if (arg4 != 0) {
            faces_write_colors(faces, (FacePrim *)node[1]);
        }
    } else {
        if (mal_first_free(*(u16 *)((u8 *)faces + 8) * 10, MainHeap.free) == NULL || (node = PrimLinkFree) == NULL) {
            return NULL;
        }
        PrimLinkFree = (s32 *)*node;
        if (list->f4 == 0) {
            list->f8 = (s32 **)node;
        }
        *node = list->f4;
        list->f4 = (s32)node;
        mal_alloc_owned_low(*(u16 *)((u8 *)faces + 8) * 0x28, (void **)(node + 1), (s32)&MainHeap, ((FrameCount & 1)?3:2));
        prim = (FacePrim *)node[1];
        prim_write_uvs(faces, prim);
        faces_write_colors(faces, prim);
    }
    return (FacePrim *)node[1];
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

    DrawSync(0);
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
        PrimPool.mem = mal_alloc(count * 0x64, &MainHeap);
    }
    p = PrimPool.mem;
    last = count - 1;
    for (i = 0; i < last; i++) {
        n = (PoolNode *)((u8 *)p + 0x64);
        p->next = n;
        p = n;
    }
    p->next = NULL;
    PrimPool.done = NULL;
    PrimPool.free = PrimPool.mem;
}

void primset_invalidate(u8 **arg0) {
    u8 *temp_a0;

    temp_a0 = *arg0;
    if (temp_a0 != NULL) {
        (*(s16*)((u8*)(temp_a0) + 86)) = 1;
    }
}

s32 *primnode_alloc(s32 arg0, s32 *arg1, s32 arg2, s32 arg3) {
    s32 *node;
    s32 *data;

    node = PrimNodeFree;
    if (node == NULL) {
        return NULL;
    }
    PrimNodeFree = (s32 *) *node;
    if (arg1[1] == 0) {
        arg1[2] = (s32) node;
    }
    data = node + 1;
    *node = arg1[1];
    arg1[1] = (s32) node;
    prim_write_uvs(arg3, data);
    faces_write_colors(arg3, data);
    return data;
}

s32 *primlist_pop(PrimList *l) {
    s32 **tail;
    s32 *n;

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
        l->f4 = *n;
        l->fC = (s32 *)*n;
        *n = 0;
    }
    return n;
}

s32 *primlist_pop_b(PrimList *l) {
    s32 **tail;
    s32 *n;

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

s32 *primset_alloc(s32 **arg0, s32 arg1) {
    s32 *p;
    s32 *next;
    void *prev;

    p = PrimSetFree;
    if (p == NULL) {
        return NULL;
    }
    *arg0 = p;
    next = (s32 *)p[0];
    prev = PrimSetList;
    PrimSetList = p;
    p[6] = 0;
    p[7] = 0;
    p[8] = 0;
    p[9] = 0;
    p[10] = 0;
    p[16] = 0;
    p[17] = 0;
    p[18] = 0;
    p[19] = 0;
    p[20] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[11] = 0;
    p[12] = 0;
    p[13] = 0;
    p[14] = 0;
    p[15] = 0;
    *(s16 *)(p + 21) = arg1;
    *((s16 *)(p + 21) + 1) = 0;
    PrimSetFree = next;
    p[0] = (s32)prev;
    return p;
}

void faces_write_uvs_colors(s32 arg0, s32 arg1) {
    prim_write_uvs();
    faces_write_colors(arg0, arg1);
}

void primnodes_free(s32 *arg0) {
    s32 *temp_v1;
    s32 *var_a0;
    s32 *var_s0;

    var_s0 = arg0;
    var_a0 = NULL;
    if (arg0 != NULL) {
        do {
            mal_free((*(s32*)((u8*)(var_s0) + 4)), &MainHeap);
            var_a0 = var_s0;
            var_s0 = (*(s32*)((u8*)(var_a0) + 0));
        } while (var_s0 != NULL);
    }
    temp_v1 = PrimLinkFree;
    PrimLinkFree = arg0;
    *var_a0 = temp_v1;
}

void primlist_reset(PrimList *arg0, s32 arg1) {
    arg0->f10 = NULL;
    arg0->fC = (s32 *) arg0->f4;
    if (arg0->fC != NULL) {
        arg0->f0 = 0;
        if (arg1 == 1) {
            primnodes_free(arg0->fC);
        } else {
            *arg0->f8 = PrimNodeFree;
            PrimNodeFree = arg0->fC;
            if (arg0->f10 != NULL) {
                *arg0->f10 = NULL;
            } else {
                arg0->f4 = 0;
            }
            arg0->f8 = arg0->f10;
        }
        if (arg0->f10 != NULL) {
            *arg0->f10 = NULL;
        } else {
            arg0->f4 = 0;
        }
        arg0->f8 = arg0->f10;
    }
}

void prims_empty(void) {
    PrimSet *set;

    DrawSync(0);
    printf("empty prims\n");
    if (ScreenRect != 0) {
        ClearOt(DrawBuffers);
        ClearOt(DrawBuffers + 0x400C);
    }
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
        *arg0->f8 = PrimNodeFree;
        PrimNodeFree = arg0->fC;
        if (arg0->f10 != NULL) {
            *arg0->f10 = NULL;
        } else {
            arg0->f4 = 0;
        }
        arg0->f8 = arg0->f10;
    }
    if (arg0->f10 != NULL) {
        *arg0->f10 = NULL;
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

u8 *tga_convert(u8 *arg0, s32 w, s32 h) {
    s32 pw;
    s32 x;
    u16 *src;
    u16 *dst;
    u16 c;
    u8 *res;

    pw = *(s16 *)(arg0 + 0xC);
    if (pw < w) {
        w = pw;
    }
    if (*(s16 *)(arg0 + 0xE) < h) {
        h = pw;
    }
    res = mal_alloc(w * h * 2, &MainHeap);
    src = (u16 *)(arg0 + (*(s16 *)(arg0 + 0xC) * (*(s16 *)(arg0 + 0xE) - 9) * 2 + 0x12));
    dst = (u16 *)res;
    for (h--; h != -1; h--) {
        for (x = w - 1; x != -1; x--) {
            c = *src++;
            *dst++ = ((c >> 10) & 0x1F) | ((c & 0x1F) << 10) | (c & 0x3E0);
        }
        src -= w + *(s16 *)(arg0 + 0xC);
    }
    return res;
}

void gfx_set_viewport(GfxViewport *vp, GfxCamera *cam) {
    DR_ENV *dr;
    DRAWENV *env;
    RECT *disp;
    GfxState *gs = &RenderState;

    gs->viewport = vp;
    GfxSetClip(gs, (RECT *)&vp->cx0, cam);
    disp = ScreenRect;
    if (cdb == DrawBuffers) {
        disp += 1;
    }
    env = (*(DRAWENV **)(cdb + 0x4008))++;
    SetDefDrawEnv(env, vp->x0 * ScreenRect->w / 640 + disp->x, vp->y0 / 2 + disp->y,
                  (vp->x1 - vp->x0) * ScreenRect->w / 640, (vp->y1 - vp->y0) / 2);
    env->dfe = 1;
    env->dtd = 0;
    env->isbg = vp->isbg;
    env->r0 = vp->r;
    env->g0 = vp->g;
    env->b0 = vp->b;
    env->ofs[0] = disp->x;
    env->ofs[1] = disp->y;
    dr = (*(DR_ENV **)(cdb + 0x4008))++;
    SetDrawEnv(dr, env);
    addPrim((u32 *)cdb + vp->otIdx + vp->otLen - 1, dr);
    *(GfxOffset *)&gs->ofx = *(GfxOffset *)&vp->ofx;
    ((GfxOffset *)gs->unk28)->x = gs->ofx;
    ((GfxOffset *)gs->unk28)->y = gs->ofy;
    gte_SetGeomOffset(vp->ofx * ScreenRect->w / 640, vp->ofy / 2);
    gte_SetGeomScreen(cam->unk18);
    gte_SetRotMatrix(&cam->view);
    gte_SetTransMatrix(&cam->view);
    gs->ot = (u32 *)cdb + vp->otIdx;
    gs->otLen = vp->otLen - 1;
    ((GfxOffset *)gs->unk28)[1].x = vp->unk20;
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
    FacePrim *prim;
    s32 dp;
    s32 fog;
    s32 t;
    s32 off;
    PrimSet *p;
    s32 c[3];

    prim = NULL;
    if ((arg1 & 0x40000001) | RenderState.modelDp | (PrimDepthTest != 0 && (u32)prim < PrimDepthLimit)) {
        set = *(PrimSet **)arg0;
        if (set != NULL || (set = (PrimSet *)primset_alloc((s32 **)arg0, *(u16 *)((u8 *)arg0 + 8) >= 3)) != NULL) {
            off = (FrameCount & 1) * sizeof(PrimListPair);
            p = (PrimSet *)(off + (s32)set);
            prim = PrimSet_NextPrim(set, off + 4, 1, &p->pairs[0].b, arg0, 0);
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
            faces_write_colors_dq((FaceRuns *)arg0, prim, (s32)RenderState.farColor, arg1);
            RenderState.dp = dp;
        } else {
            return NULL;
        }
    } else {
        set = *(PrimSet **)arg0;
        if (set != NULL || (set = (PrimSet *)primset_alloc((s32 **)arg0, *(u16 *)((u8 *)arg0 + 8) >= 3)) != NULL) {
            off = (FrameCount & 1) * sizeof(PrimListPair);
            p = (PrimSet *)(off + (s32)set);
            prim = PrimSet_NextPrim(set, off + 4, 0, &p->pairs[0].a, arg0, set->mode);
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
 * selects primitives whose SXY go at +0x10/+0x18/+0x20 and whose length is
 * preset; otherwise each is a POLY_GT3. The three register variables are
 * the original's: only they give s4/s3/s2 with s0/s1 unused. */
void draw_verts(POLY_GT3 *poly, SVECTOR *v, u8 *runs) {
    u32 otz;
    s32 opz;
    GfxState *gs;
    s32 n;
    s32 mode;
    register s32 xy asm("$20");
    register s32 z asm("$19");
    register s32 flags asm("$18");

    gs = &RenderState;
    xy = ((s32 *)v)[0];
    z = ((s32 *)v)[1];
    for (;;) {
        if (runs[1] == 0xFF) {
            return;
        }
        gte_ldv0_w(xy, z);
        v++;
        n = runs[1];
        gte_rtps_b();
        xy = ((s32 *)v)[0];
        z = ((s32 *)v)[1];
        gte_ldv0_w(xy, z);
        v++;
        mode = *runs;
        gte_rtps_b();
        xy = ((s32 *)v)[0];
        z = ((s32 *)v)[1];
        if (mode & 1) {
            for (;;) {
                if (n-- <= 0) {
                    goto next;
                }
                gte_ldv0_w(xy, z);
                v++;
                flags = z;
                gte_rtps_b();
                xy = ((s32 *)v)[0];
                z = ((s32 *)v)[1];
                gte_avsz3_b();
                gte_stotz(&otz);
                if (otz != 0) {
                    otz = (otz + gs->otzAdd) >> 1;
                    if (otz < gs->otLen) {
                        if (!(flags & 0x20000)) {
                            gte_nclip_b();
                            gte_stopz(&opz);
                            if (flags & 0x10000) {
                                if (opz > 0) {
                                    goto skip;
                                }
                            } else if (opz <= 0) {
                                goto skip;
                            }
                        }
                        addPrim(gs->ot + otz, poly);
                        gte_stsxy3_10(poly);
                    }
                }
            skip:
                poly++;
            }
        } else {
            while (n-- > 0) {
                gte_ldv0_w(xy, z);
                v++;
                flags = z;
                gte_rtps_b();
                xy = ((s32 *)v)[0];
                z = ((s32 *)v)[1];
                gte_avsz3_b();
                gte_stotz(&otz);
                otz = (otz + gs->otzAdd) >> 1;
                if (otz < gs->otLen) {
                    if (!(flags & 0x20000)) {
                        gte_nclip_b();
                        gte_stopz(&opz);
                        if (flags & 0x10000) {
                            if (opz > 0) {
                                poly++;
                                continue;
                            }
                        } else if (opz <= 0) {
                            poly++;
                            continue;
                        }
                    }
                    poly->tag = 0x09000000 | (gs->ot[otz] & 0xFFFFFF);
                    setaddr(gs->ot + otz, poly);
                    gte_stsxy3_gt3(poly);
                }
                poly++;
            }
        }
    next:
        runs += 2;
    }
}

void gfx_setup_xform(void *arg0, void *arg1, void *arg2, u8 *arg3, GteSetup *arg4) {
    GfxXform *obj = arg0;
    MATRIX *m = arg1;
    VECTOR *pos = arg2;
    u32 flags;
    GteSetup *out;

    out = arg4;

    flags = obj->flags;
    if (flags & 0x10000000) {
        GfxState *gs;

        out->trans = NULL;
        ScreenTrans.vz = pos->vz + obj->z;
        out->trans = &ScreenTrans;
        gs = &RenderState;
        out->ofx = ((GfxScreen *)gs->unk28)->x + obj->x;
        out->ofy = ((GfxScreen *)gs->unk28)->y + obj->y;
    } else {
        GfxState *gs = &RenderState;
        out->ofx = ((GfxScreen *)gs->unk28)->x;
        out->ofy = ((GfxScreen *)gs->unk28)->y;
    }
    if (!(flags & 0x10010000)) {
        if (flags & 0x80000) {
            ApplyMatrixLV(&RenderState.cam->view, (VECTOR *)&obj->x, (VECTOR *)RenderState.unk88);
            ((VECTOR *)RenderState.unk88)->vx += ((VECTOR *)RenderState.cam)->vx;
            ((VECTOR *)RenderState.unk88)->vy += ((VECTOR *)RenderState.cam)->vy;
            ((VECTOR *)RenderState.unk88)->vz += ((VECTOR *)RenderState.cam)->vz;
        } else {
            ApplyMatrixLV(m, (VECTOR *)&obj->x, (VECTOR *)RenderState.unk88);
            GfxAddTrans(pos);
        }
        out->trans = (VECTOR *)RenderState.unk88;
    }
    if (obj->unk51 != 4) {
        if (obj->unk52 != 0) {
            rotscale_update(obj->unk10);
        }
        if (flags & 0x40000) {
            MulMatrix0(&RenderState.cam->view, &obj->rot, (MATRIX *)RenderState.unk84);
        } else if (flags & 0x20000000) {
            if (obj->unk50 != 0) {
                RenderState.unk84 = (u8 *)&RenderState.unk8C;
                RenderState.unk8C.m[0][0] = (RenderState.cam->rot.m[0][0] * obj->sx) >> 12;
                RenderState.unk8C.m[1][1] = (RenderState.cam->rot.m[1][1] * obj->sy) >> 12;
                RenderState.unk8C.m[2][2] = (RenderState.cam->rot.m[2][2] * obj->sz) >> 12;
            } else {
                RenderState.unk84 = (u8 *)&RenderState.cam->rot;
            }
        } else {
            MulMatrix0(m, &obj->rot, (MATRIX *)RenderState.unk84);
        }
        if ((flags & 0x10010000) == 0x10000) {
            ApplyMatrixLV((MATRIX *)RenderState.unk84, (VECTOR *)&obj->x, (VECTOR *)RenderState.unk88);
            if (flags & 0x80000) {
                ((VECTOR *)RenderState.unk88)->vx += ((VECTOR *)RenderState.cam)->vx;
                ((VECTOR *)RenderState.unk88)->vy += ((VECTOR *)RenderState.cam)->vy;
                ((VECTOR *)RenderState.unk88)->vz += ((VECTOR *)RenderState.cam)->vz;
            } else {
                GfxAddTrans(pos);
            }
            out->trans = (VECTOR *)RenderState.unk88;
        }
        out->rot = (MATRIX *)RenderState.unk84;
    } else if (flags & 0x20000000) {
        out->rot = &RenderState.cam->rot;
    } else if (flags & 0x40000) {
        out->rot = &RenderState.cam->view;
    } else {
        out->rot = m;
    }
}

void draw_object_model(s32 a, u8 *set, s32 c, void *d) {
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

    depth = *(s16 *)((u8 *)d + 0x8A);
    {
        s32 on = (*(u32 *)d | c) & fog;
        PrimDepthLimit = depth;
        if (on) {
            s32 dp = *(s16 *)((u8 *)d + 0x76);
            RenderState.farColor = (s32 *)((u8 *)d + 0x78);
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
        if (entry >= *(s32 *)(set + 0x40)) {
            return;
        }
        e = set + (*(s32 *)(set + 0x44) + 0x44) + entry * 0x18;
        anim = dbase_anim_node((s32 *)a, e);
        if (c < 0) {
            frame = *(s16 *)((u8 *)d + 0x72);
        } else {
            frame = a & 0x7F;
        }
        if (frame >= *(s32 *)(e + 8)) {
            return;
        }
        if (anim == NULL) {
            return;
        }
        e += *(s32 *)(e + 0xC) + 0xC;
        fr = (s32 *)((u8 *)anim + (frame * 16 + 4));
        if (c & 0x400000) {
            e = set + ((*(u16 *)((u8 *)d + 0x8E) & 0xFFF) * 0x34 + 0x24);
        }
        if (fr[0] == 0) {
            return;
        }
        ia = (u8 *)fr + fr[0] + 0x14;
        if (anim[0] != 0) {
            verts = (u8 *)anim + anim[0];
        } else {
            verts = set + (*(s32 *)(set + 0x28) + 0x28);
        }
        if (c < 0 && (n = *(u16 *)((u8 *)d + 0x70)) != 0) {
            s32 off;
            s32 *nx = (s32 *)((u8 *)anim + (frame * 16 + 0x14));
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
            p = (u8 *)nx + off;
            morph_blend_verts(n, (MorphModel *)e, (u16 *)ia,
                          (u16 *)((u8 *)p + 0x14), (s16 (*)[3])verts);
        } else if (fr[2] != 0) {
            morph_blend_verts(fr[2], (MorphModel *)e, (u16 *)ia,
                          (u16 *)((u8 *)(fr + 1) + fr[1] + 0x14), (s16 (*)[3])verts);
        } else {
            morph_copy_verts(e, (u16 *)ia, verts, (u16 *)MorphVerts);
        }
        ModelEntry_Draw(e, c, MorphVerts);
        return;
    case 0x2000:
        idx = a & 0xFFF;
        if (*(s32 *)(set + 0x54) < idx) {
            return;
        }
        e = set + (idx * 0x34 + 0x24);
        goto block;
    case 0x5000:
        e = (u8 *)map_entity_address(a, set);
        if (e == NULL) {
            return;
        }
    block:
        set = e + (*(s32 *)(e + 0x10) + 0x10);
        if (!(c & 0x1000000) && !bbox_visible(set)) {
            return;
        }
        p = model_prims_begin(e, c);
        set += 0x14;
    draw:
        if (p != NULL) {
            draw_verts(p, (SVECTOR *)set, e + (*(s32 *)(e + 0x14) + 0x14));
        }
        return;
    case 0x3000:
        memset(&v, 0, 8);
        gte_ldv0(&v);
        gte_rtps();
        q = xy;
        gte_stsxy(q);
        x = xy[0] * 640 / ScreenRect->w;
        flags = *(u32 *)d;
        g = ((flags & 0x21) != 0) << 31;
        g |= (((s32)flags >> 5) & 3) << 24;
        e = (u8 *)(x & 0xFFFF);
        e = (u8 *)((s32)e | ((u16)q[1] << 17));
        if (flags & fog) {
            idx = *(u16 *)((u8 *)d + 0x76) >> 4;
            idx &= 0xFF;
            col = idx | (idx << 8) | (idx << 16);
        } else {
            col = 0x808080;
        }
        g |= col;
        otz = 1;
        if (c & 0x2000000) {
            otz = *(s16 *)((u8 *)d + 0x68);
        }
        if ((u32)otz < (u32)RenderState.otLen) {
            texture_draw((obj_texture *)(*(u8 **)((u8 *)RenderState.unk0 + 0x18) + (a & 0xFFF) * 0x38),
                          (s32)e, otz, g);
        }
        return;
    default:
        printf("UNKNOWN FRAME NUM %X\n", a);
        return;
    }
}

void draw_object(u8 *arg0, u8 *arg1, s32 arg2) {
    GteSetup s;
    GfxModel *model;
    RelTable *colors;
    u16 n;
    GfxState *g;
    u8 *p1 = arg1;

    n = *(u16 *)(arg0 + 0x74);
    if (*(s32 *)arg0 & 0x02000000) {
        GfxState *g1 = &RenderState;
        g1->otzAdd = ((GfxScreen *)g1->unk28)->otz + *(u16 *)(arg0 + 0x68);
    } else {
        g = &RenderState;
        g->otzAdd = ((GfxScreen *)g->unk28)->otz;
    }
    if (n != 0 && (*(s32 *)arg0 & 0x8000)) {
        model = *(GfxModel **)(arg0 + 0x6C);
        if (model != NULL) {
            RenderState.unk0 = model;
            colors = model->colors;
            if (colors != NULL) {
                RenderState.unk84 = RenderState.unkCC;
                RenderState.unk88 = RenderState.unk10C;
                gfx_setup_xform(arg0, p1, p1 + 0x14, RenderState.unk28, &s);
                gte_SetGeomOffset(s.ofx * ScreenRect->w / 640, s.ofy / 2);
                if (s.trans != NULL) {
                    gte_SetTransVector(s.trans);
                }
                gte_SetRotMatrix(s.rot);
                draw_object_model(n, (u8 *)colors, *(s32 *)arg0 | arg2, arg0);
            }
        }
    }
}

s32 draw_gouraud_quad(void *arg0, u32 flags) {
    ModePolyG4 *p;
    u32 otz;
#define q ((ColorQuad *)arg0)

    p =*(ModePolyG4 **)(cdb + 0x4008);
    otz = 0;
    *(ModePolyG4 **)(cdb + 0x4008) = p + 1;
    if (!(flags & 0x8000)) {
        return 0;
    }
    p->v[0].c = calc_rgbcode(q->c[0][2], q->c[0][1], q->c[0][0]);
    p->v[1].c = calc_rgbcode(q->c[1][2], q->c[1][1], q->c[1][0]);
    p->v[2].c = calc_rgbcode(q->c[2][2], q->c[2][1], q->c[2][0]);
    p->v[3].c = calc_rgbcode(q->c[3][2], q->c[3][1], q->c[3][0]);
    ((u8 *)p)[3] = 0xA;
    ((u8 *)p)[0xF] = 0x38;
    p->mode = 0xE1000200;
    p->unk8 = 0;
    if (flags & 0x21) {
        p->mode = (flags & 0x60) | 0xE1000200;
        p->v[0].c |= 0x32000000;
    }
    if (flags & 0x10000000) {
        GfxState *gs = &RenderState;
#define s ((GfxScreen *)gs->unk28)

        p->v[0].x = ((s->x + q->v[0].vx) * ScreenRect->w) / 640;
        p->v[0].y = (s->y + q->v[0].vy) / 2;
        p->v[1].x = ((s->x + q->v[1].vx) * ScreenRect->w) / 640;
        p->v[1].y = (s->y + q->v[1].vy) / 2;
        p->v[2].x = ((s->x + q->v[2].vx) * ScreenRect->w) / 640;
        p->v[2].y = (s->y + q->v[2].vy) / 2;
        p->v[3].x = ((s->x + q->v[3].vx) * ScreenRect->w) / 640;
        p->v[3].y = (s->y + q->v[3].vy) / 2;
#undef s
    } else {
        gte_ldv3c(q->v);
        gte_rtpt();
        gte_avsz3();
        gte_stotz(&otz);
        gte_stsxy3_10(p);
        gte_ldv0(&q->v[3]);
        gte_rtps();
        gte_stsxy(&p->v[3].x);
    }
    otz = (otz + RenderState.otzAdd) >> 1;
    if (otz < RenderState.otLen) {
        addPrim(RenderState.ot + otz, p);
    }
    return (s32)p;
#undef q
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
        x -= ((GfxScreen *)RenderState.unk28)->x;
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
                x -= ((GfxScreen *)RenderState.unk28)->x;
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

void draw_text_nas(TextNAS *t, s32 arg1, s32 arg2) {
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
            x += ((GfxScreen *)RenderState.unk28)->x;
            x &= ~0xC000;
            x |= 0x8000;
        } else {
            x = ((GfxScreen *)RenderState.unk28)->x + t->x;
        }
        DrawSysText(x, (s16)(*(s16 *)&((GfxScreen *)RenderState.unk28)->y + t->y), t->str, t->c0, t->c1, 0);
        return;
    }
    RenderState.unk84 = RenderState.unkCC;
    RenderState.unk88 = RenderState.unk10C;
    gfx_setup_xform(t, (u8 *)arg1, (u8 *)arg1 + 0x14, RenderState.unk28, &s);
    gte_SetGeomOffset(s.ofx * ScreenRect->w / 640, s.ofy / 2);
    if (s.trans != NULL) {
        gte_SetTransVector(s.trans);
    }
    gte_SetRotMatrix(s.rot);
    ((SWORD_XYZ *)&s)->x = t->x;
    ((SWORD_XYZ *)&s)->y = t->y;
    ((SWORD_XYZ *)&s)->z = t->z;
    SysPrintAt3d((SWORD_XYZ *)&s, t->str, t->c0, t->c1);
}

static inline u8 *func_8001AA48_setup(ModelObj *obj, void *arg1, s32 flags, RelTable *colors, u8 *t) {
    draw_map_object(obj, arg1, flags, (s32)colors);
    return t;
}

void draw_model_quad(ModelObj *obj, void *arg1, s32 flags) {
    RelTable *colors;
    POLY_GT4 *prim;
    u8 *t;
    s32 xy, z;
    u32 otz;

    if (!(obj->flags & 0x8000)) {
        return;
    }
    colors = obj->model->colors;
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
        RenderState.unk0 = obj->model;
        prim_create_entity_single(entity_address(obj->index, (u8 *)colors), prim);
    }
    t = entity_address(obj->index, (u8 *)colors);
    RenderState.dp = RenderState.modelDp;
    RenderState.unk0 = obj->model;
    prim_colour_poly((RelTable *)t, prim, (s32)RenderState.farColor, flags);
    colors = obj->model->colors;
    t = func_8001AA48_setup(obj, arg1, flags, colors, entity_address(obj->index, (u8 *)colors));
    t += *(s32 *)(t + 0x10) + 0x10;
    gte_ldv0_t(t + 0x14);
    gte_rtps();
    gte_ldv0_t(t + 0x1C);
    gte_rtps();
    gte_ldv0_t(t + 0x24);
    gte_rtps();
    xy = *(s32 *)(t + 0x2C);
    z = *(s32 *)(t + 0x30);
    gte_stsxy3_gt3(prim);
    gte_ldv0_w(xy, z);
    gte_rtps();
    gte_stsxy2(&prim->x3);
    gte_avsz3();
    gte_stotz(&otz);
    otz = (otz + RenderState.otzAdd) >> 1;
    if (otz < RenderState.otLen) {
        addPrim(RenderState.ot + otz, prim);
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
    gs->unk84 = gs->unkCC;
    view = &gs->cam->view;
    gs->unk88 = gs->unk10C;
    out = &gs->unkAC;
    cs = &gs->cam->unk54;
    if (flags & 0x10000000) {
        ((GfxScreen *)gs->unk28)->x = obj->x + gs->ofx;
        ((GfxScreen *)gs->unk28)->y = obj->y + gs->ofy;
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
            if (obj->unk52 != 0) {
                rotscale_update(obj->unk10);
            }
            MulMatrix0(view, &obj->rot, out);
        }
        out->t[0] = view->t[0];
        out->t[1] = view->t[1];
        out->t[2] = view->t[2];
    } else {
        ((GfxScreen *)gs->unk28)->x = gs->ofx;
        ((GfxScreen *)gs->unk28)->y = gs->ofy;
        if (!(flags & 0x200000)) {
            rot = view;
        } else {
            rot = &MapSceneMatrix;
        }
        pos = &RenderState.cam->x;
        v.vx = obj->x - pos[0];
        v.vy = obj->y - pos[1];
        v.vz = obj->z - pos[2];
        ApplyMatrixLV(rot, &v, (VECTOR *)out->t);
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
            if (obj->unk52 != 0) {
                rotscale_update(obj->unk10);
            }
            MulMatrix0(rot, &obj->rot, out);
        }
    }
    RenderState.otzAdd = ((GfxScreen *)RenderState.unk28)->otz;
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
    GfxCamera *cam;
    s32 r;
    s32 d;
    s32 e;
    s32 ey;

    flags = 0;
    in = &RenderState.gteIn;
    in = &box->center;
    gte_ldv0(in);
    in = &RenderState.gteIn;
    gte_rtps();
    gte_stsxy(&c);
    gte_stsz(&sz);
    if (sz >= 2) {
        r = box->center.pad;
        gs = (GfxState *)((u8 *)in - 0x34);
        if (r < 0x400) {
            cam = gs->cam;
            d = sz * 3 << 10;
            e = r * cam->unk54.scale.vx * cam->unk18 / d;
            if (c.vx + e < 0 || gs->clipX1 < c.vx - e) {
            culled:
                return 0;
            }
            ey = r * cam->unk54.scale.vy * cam->unk18 / d;
            if (c.vy + ey < 0 || gs->clipY1 < c.vy - ey) {
                goto culled;
            }
        }
    }
    in->vx = box->min[0];
    in->vy = box->min[1];
    in->vz = box->min[2];
    gte_ldv0(in);
    gte_rtps();
    in->vx = box->max[0];
    gte_stsxy(&sxy[0]);
    gte_stsz(&sz);
    if (sz > 0) {
        if ((u16)sxy[0].vx < (u16)RenderState.clipX1 && (u16)sxy[0].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 1;
    }
    gte_ldv0(in);
    gte_rtps();
    in->vx = box->min[0];
    in->vy = box->max[1];
    gte_stsxy(&sxy[1]);
    gte_stsz(&sz);
    if (sz > 0) {
        if ((u16)sxy[1].vx < (u16)RenderState.clipX1 && (u16)sxy[1].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 2;
    }
    gte_ldv0(in);
    gte_rtps();
    in->vx = box->max[0];
    gte_stsxy(&sxy[2]);
    gte_stsz(&sz);
    if (sz > 0) {
        if ((u16)sxy[2].vx < (u16)RenderState.clipX1 && (u16)sxy[2].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 4;
    }
    gte_ldv0(in);
    gte_rtps();
    in->vx = box->min[0];
    in->vy = box->min[1];
    in->vz = box->max[2];
    gte_stsxy(&sxy[3]);
    gte_stsz(&sz);
    if (sz > 0) {
        if ((u16)sxy[3].vx < (u16)RenderState.clipX1 && (u16)sxy[3].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 8;
    }
    gte_ldv0(in);
    gte_rtps();
    in->vx = box->max[0];
    gte_stsxy(&sxy[4]);
    gte_stsz(&sz);
    if (sz > 0) {
        if ((u16)sxy[4].vx < (u16)RenderState.clipX1 && (u16)sxy[4].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 0x10;
    }
    gte_ldv0(in);
    gte_rtps();
    in->vx = box->min[0];
    in->vy = box->max[1];
    gte_stsxy(&sxy[5]);
    gte_stsz(&sz);
    if (sz > 0) {
        if ((u16)sxy[5].vx < (u16)RenderState.clipX1 && (u16)sxy[5].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 0x20;
    }
    gte_ldv0(in);
    gte_rtps();
    in->vx = box->max[0];
    gte_stsxy(&sxy[6]);
    gte_stsz(&sz);
    if (sz > 0) {
        if ((u16)sxy[6].vx < (u16)RenderState.clipX1 && (u16)sxy[6].vy < (u16)RenderState.clipY1) {
            return 1;
        }
    } else {
        flags |= 0x40;
    }
    gte_ldv0(in);
    gte_rtps();
    gte_stsxy(&sxy[7]);
    gte_stsz(&sz);
    if (sz > 0) {
        if ((u16)sxy[7].vx < (u16)RenderState.clipX1 && (u16)sxy[7].vy < (u16)RenderState.clipY1) {
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
    if (sxy[0].vx < 0 && sxy[1].vx < 0 && sxy[2].vx < 0 && sxy[3].vx < 0 &&
        sxy[4].vx < 0 && sxy[5].vx < 0 && sxy[6].vx < 0 && sxy[7].vx < 0) {
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
    gte_ldv3c(v);
    v += 3;
    gte_rtpt();
    gte_stsxy3c(sxy[0]);
    gte_stsz3c(&sz[0]);
    gte_ldv3c(v);
    v += 3;
    gte_rtpt();
    minX = sxy[0][0];
    maxX = minX;
    minY = sxy[0][1];
    maxY = minY;
    if (sz[0] != 0) {
        if ((u16)sxy[0][0] < (u16)RenderState.clipX1 && (u16)sxy[0][1] < (u16)RenderState.clipY1) {
            if (behind) return 2;
            inside |= 1;
        }
        if (farZ < sz[0]) far |= 1;
    } else {
        behind |= 1;
        if (inside) return 2;
    }
    if (sz[1] != 0) {
        if ((u16)sxy[1][0] < (u16)RenderState.clipX1 && (u16)sxy[1][1] < (u16)RenderState.clipY1) {
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
        if ((u16)sxy[2][0] < (u16)RenderState.clipX1 && (u16)sxy[2][1] < (u16)RenderState.clipY1) {
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
    gte_stsxy3c(sxy[3]);
    gte_stsz3c(&sz[3]);
    gte_ldv3c(v);
    gte_rtpt();
#define POINT(i)                                                                                      \
    if (sz[i] != 0) {                                                                                 \
        if ((u16)sxy[i][0] < (u16)RenderState.clipX1 && (u16)sxy[i][1] < (u16)RenderState.clipY1) {    \
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
    gte_stsxy3c(sxy[6]);
    gte_stsz3c(&sz[6]);
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
    if (sxy[0][0] < 0 && sxy[1][0] < 0 && sxy[2][0] < 0 && sxy[3][0] < 0 && sxy[4][0] < 0 &&
        sxy[5][0] < 0 && sxy[6][0] < 0 && sxy[7][0] < 0) {
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

void morph_light_colors(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc) {
    SVECTOR *dst = MorphVerts;
    u8 *run = (u8 *)&model->runs + model->runs;
    s32 r, g, b;
    s32 i;

    gte_lddp(p);
    r = fc->vx;
    g = fc->vy;
    b = fc->vz;
    for (; run[1] != 0xFF; run += 2) {
        i = run[1] + 2;
        while (i-- > 0) {
            gte_ldsv(src);
            gte_ldfcdir(r, g, b);
            fc++;
            dst->pad = src->pad;
            src++;
            gte_intpl();
            r = fc->vx;
            g = fc->vy;
            b = fc->vz;
            gte_stsv(dst);
            dst++;
        }
    }
}

void morph_blend_verts(s32 p, MorphModel *model, u16 *ia, u16 *ib, s16 (*verts)[3]) {
    SVECTOR *dst = MorphVerts;
    u8 *run;
    u32 a;
    u32 b;
    s16 *va;
    s16 *vb;
    s32 flag;
    s32 i, n;
    s32 r, g, bl;

    gte_lddp(p);
    a = *ia;
    b = *ib;
    run = (u8 *)(model->runs + (s32)model) + 0x15;
    for (;;) {
        n = *run;
        if (n == 0xFF) {
            return;
        }
        i = n + 2;
        while (i-- > 0) {
            va = verts[a >> 2];
            vb = verts[b >> 2];
            gte_ldsv(va);
            flag = b & 3;
            r = vb[0];
            g = vb[1];
            bl = vb[2];
            gte_ldfcdir(r, g, bl);
            ia++;
            ib++;
            gte_intpl_b();
            b = *ib;
            a = *ia;
            gte_stsv(dst);
            dst->pad = flag;
            dst++;
        }
        run += 2;
    }
}

void morph_copy_verts(u8 *arg0, u16 *src, u8 *tab, u16 *dst) {
    u8 *p;
    s32 n;
    s32 c;
    s32 prev;
    u16 v;
    u16 *e;

    s32 off = *(s32 *)(arg0 + 0x14);
    u8 *b = (u8 *)off + (s32)arg0;
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

void eng_render_subs(u8 *arg0) {
    s32 (*temp_v0)(u8 *, s32, s32);
    s32 temp_s1;
    s32 temp_s2;
    u8 *var_s0;

    var_s0 = (*(s32*)((u8*)(arg0) + 108));
    RenderState.unk4 = arg0;
    if (var_s0 != NULL) {
        temp_s1 = (*(s32*)((u8*)(arg0) + 12));
        if (temp_s1 & 0x8000) {
            temp_s2 = gfx_object_matrix(arg0 + 0xC);
loop_3:
            if (var_s0 != NULL) {
                temp_v0 = (*(s32*)((u8*)(var_s0) + 84));
                if (temp_v0 != NULL) {
                    temp_v0(var_s0, temp_s2, temp_s1);
                }
                var_s0 = (*(s32*)((u8*)(var_s0) + 92));
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
    s32 p;
    s32 q;

    p = arg0->unk90;
    if (p != 0) {
        release_sprim((void *)p);
        arg0->unk90 = 0;
    }
    q = arg0->unk9C;
    if (q != 0) {
        release_sprim((void *)q);
        arg0->unk9C = 0;
    }
}

void sub_use_quad_model(u8 *arg0) {
    u8 *temp_v0;
    u16 temp_a0;

    temp_v0 = entity_address(*(u16*)(arg0 + 0x74), *(u8**)(*(u8**)(arg0 + 0x6C) + 0xC));
    temp_a0 = *(u16*)(temp_v0 + 8);
    if (temp_a0 == 2 && temp_v0[*(s32*)(temp_v0 + 0x14) + 0x15] == temp_a0) {
        *(void**)(arg0 + 0x54) = draw_model_quad;
        *(void**)(arg0 + 0x58) = kill_sprim_sub;
    }
}

void posdst_flash(PosDst *arg0, s32 arg1, s32 arg2) {
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

void draw_nas_model(NAS *nas, u8 *arg1) {
    GteSetup s;
    u32 hidden;

    if (nas->NASid & 0x8000) {
        hidden = nas->NASid & 0x10000000;
        if (nas->NASid & 0x02000000) {
            RenderState.otzAdd = nas->Class;
        } else {
            RenderState.otzAdd = 0;
        }
        if (!(nas->NASid & 0x10000000)) {
            RenderState.unk84 = RenderState.unkCC;
            RenderState.unk88 = RenderState.unk10C;
            gfx_setup_xform(nas, arg1, arg1 + 0x14, RenderState.unk28, &s);
            gte_SetGeomOffset(s.ofx * ScreenRect->w / 640, s.ofy / 2);
            if (s.trans != NULL) {
                gte_SetTransVector(s.trans);
            }
            gte_SetRotMatrix(s.rot);
        }
        draw_panel_rect(&nas->Type, hidden);
    }
}

void draw_nas_model_flags(NAS *nas, u8 *arg1, u32 flags) {
    GteSetup s;
    u32 f;

    if (nas->NASid & 0x8000) {
        f = flags | nas->NASid;
        if (nas->NASid & 0x02000000) {
            RenderState.otzAdd = nas->Class;
        } else {
            RenderState.otzAdd = 0;
        }
        if (!(nas->NASid & 0x10000000)) {
            RenderState.unk84 = RenderState.unkCC;
            RenderState.unk88 = RenderState.unk10C;
            gfx_setup_xform(nas, arg1, arg1 + 0x14, RenderState.unk28, &s);
            gte_SetGeomOffset(s.ofx * ScreenRect->w / 640, s.ofy / 2);
            if (s.trans != NULL) {
                gte_SetTransVector(s.trans);
            }
            gte_SetRotMatrix(s.rot);
        }
        f &= ~0x10000000;
        draw_gouraud_quad(&nas->Type, f | nas->NASid);
    }
}

u32 draw_nas_model_ret(NAS *nas, u8 *arg1, u32 flags) {
    GteSetup s;

    flags |= nas->NASid;
    if (nas->NASid & 0x02000000) {
        RenderState.otzAdd = nas->Class;
    } else {
        RenderState.otzAdd = 0;
    }
    if (!(nas->NASid & 0x10000000)) {
        RenderState.unk84 = RenderState.unkCC;
        RenderState.unk88 = RenderState.unk10C;
        gfx_setup_xform(nas, arg1, arg1 + 0x14, RenderState.unk28, &s);
        gte_SetGeomOffset(s.ofx * ScreenRect->w / 640, s.ofy / 2);
        if (s.trans != NULL) {
            gte_SetTransVector(s.trans);
        }
        gte_SetRotMatrix(s.rot);
    }
    flags &= ~0x10000000;
    return flags | nas->NASid;
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
            x -= ((GfxScreen *)RenderState.unk28)->x;
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

s32 sub_alloc_prims(void *arg0, s32 arg1) {
    Func8001CC10Slot *slot;
    u16 want;

    s32 off;

    off = (FrameCount & 1) * 12 + 0x90;
    slot = (Func8001CC10Slot *)(arg1 + off);
    want = *(u16 *)((u8 *)arg0 + 8);
    if (want != slot->size) {
        if (slot->size < want) {
            if (slot->ptr != 0) {
                mal_free(slot, &MainHeap);
            }
            mal_alloc_owned_low(*(u16 *)((u8 *)arg0 + 8) * 0x28, slot, &MainHeap, ((FrameCount & 1)?3:2));
            if (slot->ptr == 0) {
                slot->size = 0;
                return 0;
            }
            slot->size = *(u16 *)((u8 *)arg0 + 8);
        }
        faces_write_uvs_colors((s32)arg0, (s32)slot->ptr);
    }
    return (s32)slot->ptr;
}

void gfx_load_xform(void *arg0, void *arg1, void *arg2) {
    GteSetup s;

    RenderState.unk84 = RenderState.unkCC;
    RenderState.unk88 = RenderState.unk10C;
    gfx_setup_xform(arg0, arg1, arg2, RenderState.unk28, &s);
    gte_SetGeomOffset(s.ofx * ScreenRect->w / 640, s.ofy / 2);
    if (s.trans != NULL) {
        gte_SetTransVector(s.trans);
    }
    gte_SetRotMatrix(s.rot);
}

void draw_model_set_entry(s32 arg0, ModelSet *set, s32 arg2) {
    void *p;
    u8 *blk;
    ModelEntry *e;

    arg0 &= 0xFFF;
    if (*(s32 *)((u8 *)set + 0x54) >= arg0) {
        e = &set->entries[arg0];
        blk = (u8 *)&e->off + e->off;
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
    ModelEntry *e;

    e = (ModelEntry *)map_entity_address(arg0, arg1);
    if (e == NULL) {
        return;
    }
    blk = (u8 *)&e->off + e->off;
    if (!(arg2 & 0x1000000) && !bbox_visible(blk)) {
        return;
    }
    p = model_prims_begin(e, arg2);
    blk += 0x14;
    if (p != NULL) {
        draw_verts(p, (SVECTOR *)blk, (u8 *)&e->runs + e->runs);
    }
}

void *draw_model_entry(ModelEntry *e, s32 arg1) {
    u8 *blk;
    void *p;
    u8 *q;

    blk = (u8 *)&e->off + e->off;
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

void *draw_morph_model(MorphModel *model, void *arg1, s32 arg2) {
    void *p;

    p = model_prims_begin(model, arg2);
    if (p != NULL) {
        draw_verts(p, arg1, (u8 *)&model->runs + model->runs);
    }
    return p;
}

void project_quad(SVECTOR *v, POLY_GT4 *poly) {
    u32 otz;
    s32 xy;
    s32 z;

    gte_ldv0_w(((s32 *)v)[0], ((s32 *)v)[1]);
    v++;
    gte_rtps();
    gte_ldv0_w(((s32 *)v)[0], ((s32 *)v)[1]);
    v++;
    gte_rtps();
    gte_ldv0_w(((s32 *)v)[0], ((s32 *)v)[1]);
    v++;
    gte_rtps();
    xy = ((s32 *)v)[0];
    z = ((s32 *)v)[1];
    gte_stsxy3_gt4(poly);
    gte_ldv0_w(xy, z);
    gte_rtps();
    gte_stsxy(&poly->x3);
    gte_avsz3();
    gte_stotz(&otz);
    otz = (otz + RenderState.otzAdd) >> 1;
    if (otz < RenderState.otLen) {
        addPrim(RenderState.ot + otz, poly);
    }
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
    RenderState.unk8C.m[0][0] = 0x1000;
    RenderState.unk8C.m[0][1] = 0;
    RenderState.unk8C.m[0][2] = 0;
    RenderState.unk8C.m[1][0] = 0;
    RenderState.unk8C.m[1][1] = 0x1000;
    RenderState.unk8C.m[1][2] = 0;
    RenderState.unk8C.m[2][0] = 0;
    RenderState.unk8C.m[2][1] = 0;
    RenderState.unk8C.m[2][2] = 0x1000;
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

    p = mal_alloc(n * 0xA8, &MainHeap);
    i = n - 1;
    sub = (subengine *)p;
    SubPool = (s32)sub;
    next_sub = sub;
    if (i > 0) {
        do {
            subengine *nx = (subengine *)((u8 *)sub + 0xA8);
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
        *(SLONG_XYZ *)&sub->unk04 = p->pos;
        *(PlacementBlk *)&sub->unk2C[4] = p->blk;
        sub->hook = (void *)draw_map_sub;
        sub->u6C.dbase = env->dbase;
        sub->entity = p->unk88;
        sub->unk8A = p->unk9E;
        sub->unk68 = p->unk9C;
        sub->unk64[0] = p->unk74[0];
        sub->unk64[1] = p->unk74[1];
        sub->unk64[2] = p->unk74[2];
        sub->unk64[3] = p->unk74[3];
        if (n > 0) {
            sub->next = sub + 1;
        } else {
            sub->next = NULL;
        }
        p++;
        sub++;
    }
}

void draw_map_object(void *sub, void *arg1, s32 flags, s32 arg3) {
    MATRIX m;
    VECTOR v;
    MATRIX *rot;
    MATRIX *mp;

    m = RenderState.cam->view;
    RenderState.unk0 = ((subengine *)sub)->u6C.dbase = ((GfxModel *)MapScene->dbase);
    v.vx = ((subengine *)sub)->unk04 - RenderState.cam->x;
    v.vy = ((subengine *)sub)->unk08 - RenderState.cam->y;
    v.vz = ((subengine *)sub)->unk0C - RenderState.cam->z;
    mp = &m;
    if (!(flags & 0x200000)) {
        rot = &RenderState.cam->view;
    } else {
        rot = &MapSceneMatrix;
    }
    ApplyMatrixLV(rot, &v, (VECTOR *)m.t);
    m.t[0] += RenderState.cam->view.t[0];
    m.t[1] += RenderState.cam->view.t[1];
    m.t[2] += RenderState.cam->view.t[2];
    if (((subengine *)sub)->flags & 0x20000000) {
        gte_SetRotMatrix(&RenderState.cam->rot);
    } else {
        if (*(u16 *)&((subengine *)sub)->unk52 != 0) {
            rotscale_update(&((subengine *)sub)->unk10);
        }
        MulMatrix0(rot, (MATRIX *)((u8 *)sub + 0x30), mp);
        gte_SetRotMatrix(mp);
    }
    gte_SetTransMatrix(mp);
    RenderState.otzAdd = *(u16 *)(RenderState.unk28 + 8) + ((subengine *)sub)->unk68;
    gte_SetGeomOffset(RenderState.ofx * ScreenRect->w / 640, RenderState.ofy / 2);
}

void draw_map_scene(GfxScene *scene) {
    MATRIX m;
    MATRIX view;
    MATRIX *pv;
    u8 *obj;
    void (*fn)(u8 *, MATRIX *, s32);

    MapScene = scene;
    RotMatrix(&MapSceneRot, &m);
    MulMatrix0(&RenderState.cam->view, &m, &MapSceneMatrix);
    pv = &view;
    view = RenderState.cam->view;
    obj = scene->objs;
    while (1) {
        if (obj == NULL) {
            break;
        }
        fn = *(void (**)(u8 *, MATRIX *, s32))(obj + 0x54);
        if (fn != NULL) {
            fn(obj, pv, 0);
        }
        obj = *(u8 **)(obj + 0x5C);
    }
    GfxSetSceneBg(&RenderState, scene);
}

subengine *mapscene_find_sub(s32 i, s32 val) {
    subengine *sub;

    for (sub = MapScene->subs; sub != NULL; sub = sub->next) {
        if (((u8 *)sub + i)[0x64] == val) {
            return sub;
        }
    }
    return NULL;
}

s32 rel_ptr(s32 *arg0) {
    s32 v;

    v = ((s32 *)(arg0[1] + (s32)arg0))[1];
    if (v == 0) {
        return 0;
    }
    return v + arg0[0];
}


extern void draw_map_object(void *a, void *b, s32 c, s32 d);
void draw_map_sub(void *arg0, void *arg1) {
    s32 flags;
    s32 x;

    flags = *(s32 *)arg0;
    x = *(s32 *)(*(u8 **)((u8 *)arg0 + 0x6C) + 0xC);
    if ((flags & 0x8000) && x != 0) {
        draw_map_object(arg0, arg1, flags, x);
        draw_object_model(*(u16 *)((u8 *)arg0 + 0x74), (u8 *)x, *(s32 *)arg0, arg0);
    }
}

void map_update(s32 *arg0) {
    texture_update_anims(*arg0 + 0x10);
}

void mapscene_release(map_enviroment *env) {
    dbase_release((u8 *)env->dbase);
    mal_free(env->subs, &MainHeap);
    env->subs = NULL;
}

void mapscene_setup_tables(arg0)
    s32 *arg0;
{
    s32 *outer;
    u8 *p;
    u8 *q;
    u8 *r;
    u8 *s;
    u8 *t;
    u8 *u;
    s32 idx;

    outer = (s32 *)arg0[0];
    idx = arg0[2] * 4;
    p = (u8 *)outer[3];
    p = p + (*(s32 *)(p + 0x18) + 0x18);
    arg0[3] = (s32)p;
    q = p + (idx + 4) + *(s32 *)(p + idx + 4);
    arg0[4] = (s32)q;
    r = q + (*(s32 *)(q + 0xC) + 0xC);
    arg0[10] = (s32)r;
    arg0[11] = (s32)(r + 4);
    arg0[12] = (s32)(q + (*(s32 *)(q + 0x10) + 0x10));
    arg0[5] = (s32)(q + (*(s32 *)(q + 0x20) + 0x20));
    arg0[6] = *(s32 *)(q + 0x1C);
    s = (u8 *)outer[3];
    s = s + (*(s32 *)(s + 0x1C) + 0x1C);
    arg0[8] = (s32)s;
    t = (u8 *)outer[3];
    arg0[9] = *(s32 *)(t + 0x10) + (s32)t + 0x18;
    u = (u8 *)outer[3];
    *(s32 *)(*(s32 *)(u + 0x3C) + (s32)u + 0x48) = (s32)u;
}

void mapscene_init(s32 *arg0, s32 *arg1) {
    u8 *p;

    arg0[0] = (s32)arg1;
    arg0[2] = 0;
    p = (u8 *)arg0[0] + 0x10;
    MapScene = arg0;
    if (arg1 == NULL) {
        arg0[12] = 0;
        arg0[1] &= ~3;
        return;
    }
    arg0[1] = 3;
    textureset_clear((obj_textureset *)p);
    load_file_cb(arg1[1], map_loaded_textureset, arg0);
}

void map_loaded_textureset(u8 *arg0, u8 **arg1) {
    u8 *temp_s0;
    u8 *temp_s1;

    temp_s1 = *arg1;
    mal_set_owner(arg0, temp_s1 + 0x20);
    temp_s0 = temp_s1 + 0x10;
    textureset_init(temp_s0, (*(s32*)((u8*)(temp_s0) + 16)));
    textureset_upload(temp_s0);
    texture_release_loading_mem((obj_textureset *)temp_s0);
    (*(s32*)((u8*)(temp_s1) + 8)) = loader_queue((*(s32*)((u8*)(temp_s1) + 0)), 0, (*(s32*)((u8*)(temp_s1) + 36)), map_loaded_geo, arg1, 0, 0);
}

void map_loaded_geo(s32 arg0, map_enviroment *env) {
    obj_dbase *db;
    obj_dbase *t;

    t = env->dbase;
    MapScene = env;
    t->data = (u8 *)arg0;
    db = env->dbase;
    env->map_num = 0;
    mapscene_setup_tables(env);
    env->subs = NULL;
    mapenv_place_subs(env);
    dbase_load_all_mesh_anims((s32 *)db);
}

void mapscene_load(map_enviroment *env, s32 db) {
    env->dbase = (obj_dbase *)db;
    env->map_num = 0;
    MapScene = env;
    mapscene_setup_tables();
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
        *(Words3 *)&dst->unk04 = *(Words3 *)&src->pos;
        *(Words4 *)&dst->unk2C[4] = *(Words4 *)&src->blk.w[0];
        *(Words4 *)&dst->unk2C[0x14] = *(Words4 *)&src->blk.w[4];
        dst->hook = (void (*)(subengine *, s32, u32))draw_map_sub;
        dst->u6C.dbase = env->dbase;
        dst->entity = src->unk88;
        dst->unk8A = src->unk9E;
        dst->unk68 = src->unk9C;
        dst->unk64[0] = src->unk74[0];
        dst->unk64[1] = src->unk74[1];
        dst->unk64[2] = src->unk74[2];
        dst->unk64[3] = src->unk74[3];
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

static inline KeyEntry *func_8001E21C_find(KeyBlock *arg0, s32 arg1) {
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

    p = func_8001E21C_find(arg0, arg1);
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
    if (((s32)MapScene->dbase) != 0) {
        p = ((s32 *)MapScene->path_list);
    }
    i = 0;
    n = *p;
loop:
    p++;
    if (i < n) {
        if (*(s32 *)((u8 *)p + *p + 0xC) == arg0) {
            goto found;
        }
        i++;
        goto loop;
    }
    i = -1;
found:
    if (i >= 0) {
        q = (s32 *)(((u8 *)MapScene->path) + (i << 2));
        return (s32 *)((u8 *)q + *q);
    }
    return NULL;
}

s32 path_find(u32 arg0) {
    s32 *p;
    s32 n;
    s32 i;

    p = NULL;
    if (((s32)MapScene->dbase) != 0) {
        p = ((s32 *)MapScene->path_list);
    }
    i = 0;
    n = *p;
loop:
    p++;
    if (i < n) {
        if (*(s32 *)((u8 *)p + *p + 0xC) == arg0) {
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
            mal_init(&MainHeap, (u8 *)pc->process->heap, 0x801FF000 - (s32)pc->process->heap);
            smallnode_init();
        }
        ((D5A614View *)GameInfo)->unk30 &= ~1;
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
s32 *PrimNodeFree = 0;
u8 *PrimLinkPool = 0;
s32 *PrimLinkFree = 0;
u8 *PrimSetPool = 0;
s32 *PrimSetFree = 0;
void *PrimSetList = 0;
s32 FarColor[3] = {0};
s32 D_80056874[1] = {0};
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
s32 SubPool = 0;
subengine *next_sub = 0;
map_enviroment *MapScene = 0;
MATRIX MapSceneMatrix = {0};
SVECTOR MapSceneRot = {0};
s32 D_80058AFC[1] = {0};
