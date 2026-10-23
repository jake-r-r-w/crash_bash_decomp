#include "common.h"
#include "bash/main.h"
#include "bash/80017164.h"

static inline void func_80017164_clear(u8 *p) {
    *(s32 *)(p + 4) = 0;
    *(s32 *)(p + 8) = 0;
    *(s32 *)(p + 0xC) = 0;
    *(s32 *)(p + 0x10) = 0;
}

void func_80017164(s32 *cfg) {
    s32 cnt;
    u8 *p;
    u8 *q;
    u8 *next;
    s32 n;
    s32 end;
    s32 end2;
    s32 end1;

    cnt = cfg[0];
    D_8005683C = cfg;
    D_8005685C = func_80011654((u32)(cnt * 0x2C0) >> 3, &D_8004E0F0);
    D_80056840 = func_80011654((u32)(cfg[2] * 0x2A0) >> 3, &D_8004E0F0);
    q = func_80011654((u32)(cfg[1] << 6) >> 3, &D_8004E0F0);
    D_80056854 = q;
    p = D_8005685C;
    next = D_80056840;
    D_80056850 = (s32 *)next;
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
    D_80056858 = (s32 *)D_80056854;
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
    D_80056864 = NULL;
    *(u8 **)p = NULL;
    D_80056848 = 0;
    D_80056860 = (s32 *)D_8005685C;
}

static inline void PrimList_Flush(ListNode18938 *arg0, s32 arg1) {
    arg0->f0 = 0;
    if (arg1 == 1) {
        func_80018764(arg0->fC);
    } else {
        *arg0->f8 = D_80056850;
        D_80056850 = arg0->fC;
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
        D_80056864 = next;
    }
    set->next = (PrimSet *) D_80056860;
    D_80056860 = (s32 *) set;
}

void func_80017388(void) {
    PrimSet *set;
    PrimSet *prev;
    s32 off;
    PrimSet *p;
    PrimSet *q;

    off =(D_8005B690 & 1) * sizeof(PrimListPair);
    set = D_80056864;
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
                set = D_80056864;
            }
        } else {
            prev = set;
            set = set->next;
        }
    }
}

static inline void PrimList_Empty(ListNode18938 *arg0, u8 *node) {
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

void func_80017640(void) {
    s32 idx;
    u8 *node;

    idx = (D_8005B690 & 1) ^ 1;
    DrawSync(0);
    node = D_80056864;
    for (;;) {
        if (node == NULL) {
            return;
        }
        PrimList_Empty((ListNode18938 *) (node + idx * 0x28 + 0x18), node);
        PrimList_Empty((ListNode18938 *) (node + idx * 0x28 + 4), node);
        node = *(u8 **) node;
    }
}

void prim_colour_poly(RelTable *face, POLY_GT4 *poly, VECTOR *arg2, s32 mode) {
    u16 *idx;
    u32 *colors;
    u32 *c;
    s32 semi = mode & 1;
    s32 abr = mode & 0x60;

    gte_ldfcdir(D_80056998.farColor[0], D_80056998.farColor[1], D_80056998.farColor[2]);
    gte_lddp(D_80056998.dp);
    idx = (u16 *)((u8 *)&face->table + face->table);
    colors = (u32 *)((u8 *)&D_80056998.unk0->colors->table + D_80056998.unk0->colors->table);
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
    GfxModel *model = D_80056998.unk0;
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

void func_80017B08(FaceRuns *faces, FacePrim *prim, s32 arg2, s32 mode) {
    u16 *idx;
    u8 *run;
    u8 *count;
    u32 *colors;
    u32 *c;
    s32 n;
    s32 semi = mode & 1;
    s32 abr = mode & 0x60;

    idx = (u16 *)((u8 *)&faces->table + faces->table);
    gte_ldfcdir(D_80056998.farColor[0], D_80056998.farColor[1], D_80056998.farColor[2]);
    gte_lddp(D_80056998.dp);
    run = (u8 *)&faces->runs + faces->runs;
    colors = (u32 *)((u8 *)&D_80056998.unk0->colors->table + D_80056998.unk0->colors->table);
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

void func_80017D90(FaceRuns *faces, FacePrim *prim) {
    u16 *idx;
    u8 *run;
    u8 *count;
    u32 *colors;
    u32 *c;
    s32 n;

    idx = (u16 *)((u8 *)&faces->table + faces->table);
    run = (u8 *)&faces->runs + faces->runs;
    colors = (u32 *)((u8 *)&D_80056998.unk0->colors->table + D_80056998.unk0->colors->table);
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
    ts = &D_80056998.unk0->texs;
    uvIdx = (u16 *)((u8 *)&faces->uvIdx + faces->uvIdx);
    tex = (u16 *)((u8 *)&faces->texs + faces->texs);
    ut = (UvTable *)D_80056998.unk0->colors;
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

FacePrim *func_800180BC(s32 arg0, ListNode18938 *list, ListNode18938 *pool, FaceRuns *faces, s32 arg4) {
    s32 *node;
    FacePrim *prim;

    node = ListNode18938_Take(pool);
    if (node != NULL) {
        *node = list->f4;
        list->f4 = (s32)node;
        if (list->f8 == NULL) {
            list->f8 = (s32 **)node;
        }
        if (arg4 != 0) {
            func_80017D90(faces, (FacePrim *)node[1]);
        }
    } else {
        if (mal_first_free(*(u16 *)((u8 *)faces + 8) * 10, D_8004E0F0.free) == NULL || (node = D_80056858) == NULL) {
            return NULL;
        }
        D_80056858 = (s32 *)*node;
        if (list->f4 == 0) {
            list->f8 = (s32 **)node;
        }
        *node = list->f4;
        list->f4 = (s32)node;
        func_800115D8(*(u16 *)((u8 *)faces + 8) * 0x28, (void **)(node + 1), (s32)&D_8004E0F0, ((D_8005B690 & 1)?3:2));
        prim = (FacePrim *)node[1];
        prim_write_uvs(faces, prim);
        func_80017D90(faces, prim);
    }
    return (FacePrim *)node[1];
}

void func_8001828C(void) {
    NodePool *pool;
    PoolNode *node;
    PoolNode *prev;
    PoolNode *next;
    PoolNode *head;

    for (;;) {
        pool = &D_80056844;
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
                node->next = D_80056844.free;
                D_80056844.free = node;
            } else {
                prev = node;
            }
            node = next;
        } while (node != NULL);
    }
}

void func_8001830C(void) {
    PoolNode *next;
    PoolNode *prev;
    PoolNode *cur;

    cur = D_80056844.done;
    prev = NULL;
    if (cur != NULL) do {
        next = cur->next;
        cur->state--;
        if (cur->state == 0) {
            if (prev != NULL) {
                prev->next = cur->next;
            } else {
                D_80056844.done = cur->next;
            }
            cur->next = D_80056844.free;
            D_80056844.free = cur;
        } else {
            prev = cur;
        }
        cur = next;
    } while (cur != NULL);
}

void release_sprim(void *data) {
    NodePool *pool = &D_80056844;
    PoolNode *node;

    node = (PoolNode *)data - 1;
    node->next = pool->done;
    pool->done = node;
    node->state = 2;
}

void *new_sprim(void) {
    PoolNode *node;

    node = D_80056844.free;
    if (node != NULL) {
        D_80056844.free = node->next;
        return node + 1;
    }
    return NULL;
}

void func_800183DC(void) {
    NodePool *pool;

    DrawSync(0);
    func_8001828C();
    pool = &D_80056844;
    if (pool->mem != NULL) {
        mal_free(pool->mem, &D_8004E0F0);
    }
    pool->mem = NULL;
}

void func_80018430(s32 count) {
    PoolNode *p;
    PoolNode *n;
    s32 i;
    s32 last;

    if (D_80056844.mem == NULL) {
        D_80056844.mem = func_80011654(count * 0x64, &D_8004E0F0);
    }
    p = D_80056844.mem;
    last = count - 1;
    for (i = 0; i < last; i++) {
        n = (PoolNode *)((u8 *)p + 0x64);
        p->next = n;
        p = n;
    }
    p->next = NULL;
    D_80056844.done = NULL;
    D_80056844.free = D_80056844.mem;
}

void func_800184D4(u8 **arg0) {
    u8 *temp_a0;

    temp_a0 = *arg0;
    if (temp_a0 != NULL) {
        (*(s16*)((u8*)(temp_a0) + 86)) = 1;
    }
}

s32 *func_800184F0(s32 arg0, s32 *arg1, s32 arg2, s32 arg3) {
    s32 *node;
    s32 *data;

    node = D_80056850;
    if (node == NULL) {
        return NULL;
    }
    D_80056850 = (s32 *) *node;
    if (arg1[1] == 0) {
        arg1[2] = (s32) node;
    }
    data = node + 1;
    *node = arg1[1];
    arg1[1] = (s32) node;
    prim_write_uvs(arg3, data);
    func_80017D90(arg3, data);
    return data;
}

s32 *func_80018570(u8 *arg0) {
    s32 **temp_a1;
    s32 *temp_v1;

    if ((arg0 == NULL) || (temp_v1 = (*(s32*)((u8*)(arg0) + 12)), (temp_v1 == NULL))) {
        return NULL;
    }
    temp_a1 = (*(s32*)((u8*)(arg0) + 16));
    (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
    if (temp_a1 != NULL) {
        *temp_a1 = *temp_v1;
        (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
        if (*temp_v1 == NULL) {
            (*(s32*)((u8*)(arg0) + 8)) = temp_a1;
        }
    } else {
        (*(s32*)((u8*)(arg0) + 4)) = (s32 *) *temp_v1;
        (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
        *temp_v1 = NULL;
    }
    return temp_v1;
}

s32 *func_800185F8(u8 *arg0) {
    s32 **temp_a1;
    s32 *temp_v1;

    if ((arg0 == NULL) || (temp_v1 = (*(s32*)((u8*)(arg0) + 12)), (temp_v1 == NULL))) {
        return NULL;
    }
    temp_a1 = (*(s32*)((u8*)(arg0) + 16));
    (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
    if (temp_a1 != NULL) {
        *temp_a1 = *temp_v1;
        (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
        if (*temp_v1 == NULL) {
            (*(s32*)((u8*)(arg0) + 8)) = temp_a1;
        }
    } else {
        if ((*(s32*)((u8*)(arg0) + 8)) == temp_v1) {
            (*(s32*)((u8*)(arg0) + 8)) = NULL;
        }
        (*(s32*)((u8*)(arg0) + 4)) = (s32 *) *temp_v1;
        (*(s32*)((u8*)(arg0) + 12)) = (s32 *) *temp_v1;
        *temp_v1 = NULL;
    }
    return temp_v1;
}

s32 *func_80018694(s32 **arg0, s32 arg1) {
    s32 *p;
    s32 *next;
    void *prev;

    p = D_80056860;
    if (p == NULL) {
        return NULL;
    }
    *arg0 = p;
    next = (s32 *)p[0];
    prev = D_80056864;
    D_80056864 = p;
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
    D_80056860 = next;
    p[0] = (s32)prev;
    return p;
}

void func_80018728(s32 arg0, s32 arg1) {
    prim_write_uvs();
    func_80017D90(arg0, arg1);
}

void func_80018764(s32 *arg0) {
    s32 *temp_v1;
    s32 *var_a0;
    s32 *var_s0;

    var_s0 = arg0;
    var_a0 = NULL;
    if (arg0 != NULL) {
        do {
            mal_free((*(s32*)((u8*)(var_s0) + 4)), &D_8004E0F0);
            var_a0 = var_s0;
            var_s0 = (*(s32*)((u8*)(var_a0) + 0));
        } while (var_s0 != NULL);
    }
    temp_v1 = D_80056858;
    D_80056858 = arg0;
    *var_a0 = temp_v1;
}

void func_800187D4(ListNode18938 *arg0, s32 arg1) {
    arg0->f10 = NULL;
    arg0->fC = (s32 *) arg0->f4;
    if (arg0->fC != NULL) {
        arg0->f0 = 0;
        if (arg1 == 1) {
            func_80018764(arg0->fC);
        } else {
            *arg0->f8 = D_80056850;
            D_80056850 = arg0->fC;
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

void func_8001888C(void) {
    u8 *var_s0;

    DrawSync(0);
    printf("empty prims\n");
    if (D_8005B698 != 0) {
        func_8002775C(D_8005B790);
        func_8002775C(D_8005B790 + 0x400C);
    }
    var_s0 = D_80056864;
    if (var_s0 != NULL) {
        do {
            func_800187D4(var_s0 + 0x18, (*(u16*)((u8*)(var_s0) + 84)));
            func_800187D4(var_s0 + 4, (*(u16*)((u8*)(var_s0) + 84)));
            func_800187D4(var_s0 + 0x40, (*(u16*)((u8*)(var_s0) + 84)));
            func_800187D4(var_s0 + 0x2C, (*(u16*)((u8*)(var_s0) + 84)));
            var_s0 = (*(s32*)((u8*)(var_s0) + 0));
        } while (var_s0 != NULL);
    }
}

void func_80018938(ListNode18938 *arg0, s32 arg1) {
    arg0->f0 = 0;
    if (arg1 == 1) {
        func_80018764(arg0->fC);
    } else {
        *arg0->f8 = D_80056850;
        D_80056850 = arg0->fC;
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

void func_800189E0(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 4)) = 0;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
    (*(s32*)((u8*)(arg0) + 16)) = 0;
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
    res = func_80011654(w * h * 2, &D_8004E0F0);
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

void func_80018B08(GfxViewport *vp, GfxCamera *cam) {
    DR_ENV *dr;
    DRAWENV *env;
    RECT *disp;
    GfxState *gs = &D_80056998;

    gs->viewport = vp;
    GfxSetClip(gs, (RECT *)&vp->cx0, cam);
    disp = D_8005B698;
    if (cdb == D_8005B790) {
        disp += 1;
    }
    env = (*(DRAWENV **)(cdb + 0x4008))++;
    SetDefDrawEnv(env, vp->x0 * D_8005B698->w / 640 + disp->x, vp->y0 / 2 + disp->y,
                  (vp->x1 - vp->x0) * D_8005B698->w / 640, (vp->y1 - vp->y0) / 2);
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
    gte_SetGeomOffset(vp->ofx * D_8005B698->w / 640, vp->ofy / 2);
    gte_SetGeomScreen(*(s32 *)(cam->unk18));
    gte_SetRotMatrix(&cam->view);
    gte_SetTransMatrix(&cam->view);
    gs->ot = (u32 *)cdb + vp->otIdx;
    gs->otLen = vp->otLen - 1;
    ((GfxOffset *)gs->unk28)[1].x = vp->unk20;
    gs->otzAdd = 0;
    gs->modelDp = 0;
}

s32 func_80018F3C(EXTengine *eng, s32 n) {
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
            D_8005A830.next = free->next;
            D_8005A830.prev = free->prev;
            *sub = D_8005A830;
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
            D_8005A830.next = free->next;
            D_8005A830.prev = free->prev;
            *free = D_8005A830;
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

void *func_80019094(void *arg0, s32 arg1) {
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
    if ((arg1 & 0x40000001) | D_800569AC | (D_80056878 != 0 && (u32)prim < D_80056AC4)) {
        set = *(PrimSet **)arg0;
        if (set != NULL || (set = (PrimSet *)func_80018694((s32 **)arg0, *(u16 *)((u8 *)arg0 + 8) >= 3)) != NULL) {
            off = (D_8005B690 & 1) * sizeof(PrimListPair);
            p = (PrimSet *)(off + (s32)set);
            prim = PrimSet_NextPrim(set, off + 4, 1, &p->pairs[0].b, arg0, 0);
        }
        if (prim != NULL) {
            gs = &D_80056998;
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
                    gs->farColor = D_80056868;
                }
            }
            func_80017B08((FaceRuns *)arg0, prim, (s32)D_80056998.farColor, arg1);
            D_80056998.dp = dp;
        } else {
            return NULL;
        }
    } else {
        set = *(PrimSet **)arg0;
        if (set != NULL || (set = (PrimSet *)func_80018694((s32 **)arg0, *(u16 *)((u8 *)arg0 + 8) >= 3)) != NULL) {
            off = (D_8005B690 & 1) * sizeof(PrimListPair);
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

    gs = &D_80056998;
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

void func_8001965C(void *arg0, void *arg1, void *arg2, u8 *arg3, GteSetup *arg4) {
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
        D_8005688C.vz = pos->vz + obj->z;
        out->trans = &D_8005688C;
        gs = &D_80056998;
        out->ofx = ((GfxScreen *)gs->unk28)->x + obj->x;
        out->ofy = ((GfxScreen *)gs->unk28)->y + obj->y;
    } else {
        GfxState *gs = &D_80056998;
        out->ofx = ((GfxScreen *)gs->unk28)->x;
        out->ofy = ((GfxScreen *)gs->unk28)->y;
    }
    if (!(flags & 0x10010000)) {
        if (flags & 0x80000) {
            ApplyMatrixLV(&D_80056998.cam->view, (VECTOR *)&obj->x, (VECTOR *)D_80056998.unk88);
            ((VECTOR *)D_80056998.unk88)->vx += ((VECTOR *)D_80056998.cam)->vx;
            ((VECTOR *)D_80056998.unk88)->vy += ((VECTOR *)D_80056998.cam)->vy;
            ((VECTOR *)D_80056998.unk88)->vz += ((VECTOR *)D_80056998.cam)->vz;
        } else {
            ApplyMatrixLV(m, (VECTOR *)&obj->x, (VECTOR *)D_80056998.unk88);
            GfxAddTrans(pos);
        }
        out->trans = (VECTOR *)D_80056998.unk88;
    }
    if (obj->unk51 != 4) {
        if (obj->unk52 != 0) {
            func_800142D4(obj->unk10);
        }
        if (flags & 0x40000) {
            MulMatrix0(&D_80056998.cam->view, &obj->rot, (MATRIX *)D_80056998.unk84);
        } else if (flags & 0x20000000) {
            if (obj->unk50 != 0) {
                D_80056998.unk84 = (u8 *)&D_80056998.unk8C;
                D_80056998.unk8C.m[0][0] = (D_80056998.cam->rot.m[0][0] * obj->sx) >> 12;
                D_80056998.unk8C.m[1][1] = (D_80056998.cam->rot.m[1][1] * obj->sy) >> 12;
                D_80056998.unk8C.m[2][2] = (D_80056998.cam->rot.m[2][2] * obj->sz) >> 12;
            } else {
                D_80056998.unk84 = (u8 *)&D_80056998.cam->rot;
            }
        } else {
            MulMatrix0(m, &obj->rot, (MATRIX *)D_80056998.unk84);
        }
        if ((flags & 0x10010000) == 0x10000) {
            ApplyMatrixLV((MATRIX *)D_80056998.unk84, (VECTOR *)&obj->x, (VECTOR *)D_80056998.unk88);
            if (flags & 0x80000) {
                ((VECTOR *)D_80056998.unk88)->vx += ((VECTOR *)D_80056998.cam)->vx;
                ((VECTOR *)D_80056998.unk88)->vy += ((VECTOR *)D_80056998.cam)->vy;
                ((VECTOR *)D_80056998.unk88)->vz += ((VECTOR *)D_80056998.cam)->vz;
            } else {
                GfxAddTrans(pos);
            }
            out->trans = (VECTOR *)D_80056998.unk88;
        }
        out->rot = (MATRIX *)D_80056998.unk84;
    } else if (flags & 0x20000000) {
        out->rot = &D_80056998.cam->rot;
    } else if (flags & 0x40000) {
        out->rot = &D_80056998.cam->view;
    } else {
        out->rot = m;
    }
}

void func_80019A60(s32 a, u8 *set, s32 c, void *d) {
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
        D_80056AC4 = depth;
        if (on) {
            s32 dp = *(s16 *)((u8 *)d + 0x76);
            D_80056998.farColor = (s32 *)((u8 *)d + 0x78);
            D_80056998.dp = dp;
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
        anim = func_80015AB0((s32 *)a, e);
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
            func_8001C0F0(n, (MorphModel *)e, (u16 *)ia,
                          (u16 *)((u8 *)p + 0x14), (s16 (*)[3])verts);
        } else if (fr[2] != 0) {
            func_8001C0F0(fr[2], (MorphModel *)e, (u16 *)ia,
                          (u16 *)((u8 *)(fr + 1) + fr[1] + 0x14), (s16 (*)[3])verts);
        } else {
            func_8001C1E0(e, (u16 *)ia, verts, (u16 *)D_80056AC8);
        }
        ModelEntry_Draw(e, c, D_80056AC8);
        return;
    case 0x2000:
        idx = a & 0xFFF;
        if (*(s32 *)(set + 0x54) < idx) {
            return;
        }
        e = set + (idx * 0x34 + 0x24);
        goto block;
    case 0x5000:
        e = (u8 *)map_entity_address(a & 0xFFFF, set);
        if (e == NULL) {
            return;
        }
    block:
        set = e + (*(s32 *)(e + 0x10) + 0x10);
        if (!(c & 0x1000000) && !func_8001AF2C(set)) {
            return;
        }
        p = func_80019094(e, c);
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
        x = xy[0] * 640 / D_8005B698->w;
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
        if ((u32)otz < (u32)D_80056998.otLen) {
            texture_draw((SpriteFrame *)(*(u8 **)((u8 *)D_80056998.unk0 + 0x18) + (a & 0xFFF) * 0x38),
                          (s32)e, otz, g);
        }
        return;
    default:
        printf("UNKNOWN FRAME NUM %X\n", a);
        return;
    }
}

void func_80019F1C(u8 *arg0, u8 *arg1, s32 arg2) {
    GteSetup s;
    GfxModel *model;
    RelTable *colors;
    u16 n;
    GfxState *g;
    u8 *p1 = arg1;

    n = *(u16 *)(arg0 + 0x74);
    if (*(s32 *)arg0 & 0x02000000) {
        GfxState *g1 = &D_80056998;
        g1->otzAdd = ((GfxScreen *)g1->unk28)->otz + *(u16 *)(arg0 + 0x68);
    } else {
        g = &D_80056998;
        g->otzAdd = ((GfxScreen *)g->unk28)->otz;
    }
    if (n != 0 && (*(s32 *)arg0 & 0x8000)) {
        model = *(GfxModel **)(arg0 + 0x6C);
        if (model != NULL) {
            D_80056998.unk0 = model;
            colors = model->colors;
            if (colors != NULL) {
                D_80056998.unk84 = D_80056998.unkCC;
                D_80056998.unk88 = D_80056998.unk10C;
                func_8001965C(arg0, p1, p1 + 0x14, D_80056998.unk28, &s);
                gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
                if (s.trans != NULL) {
                    gte_SetTransVector(s.trans);
                }
                gte_SetRotMatrix(s.rot);
                func_80019A60(n, (u8 *)colors, *(s32 *)arg0 | arg2, arg0);
            }
        }
    }
}

s32 func_8001A0D8(void *arg0, u32 flags) {
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
        GfxState *gs = &D_80056998;
#define s ((GfxScreen *)gs->unk28)

        p->v[0].x = ((s->x + q->v[0].vx) * D_8005B698->w) / 640;
        p->v[0].y = (s->y + q->v[0].vy) / 2;
        p->v[1].x = ((s->x + q->v[1].vx) * D_8005B698->w) / 640;
        p->v[1].y = (s->y + q->v[1].vy) / 2;
        p->v[2].x = ((s->x + q->v[2].vx) * D_8005B698->w) / 640;
        p->v[2].y = (s->y + q->v[2].vy) / 2;
        p->v[3].x = ((s->x + q->v[3].vx) * D_8005B698->w) / 640;
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
    otz = (otz + D_80056998.otzAdd) >> 1;
    if (otz < D_80056998.otLen) {
        addPrim(D_80056998.ot + otz, p);
    }
    return (s32)p;
#undef q
}

void func_8001A43C(u16 *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4) {
    QuadVerts q;
    s32 x;
    s32 w;
    s32 l;
    s32 r;

    w = (s16) arg0[3];
    x = (s16) arg0[0];
    if ((x & 0xC000) == 0x8000) {
        x = 0x140 - (w / 2);
        x -= D_800569C0;
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
    func_8001A0D8(&q, arg2);
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
    func_8001A0D8(&q, arg2);
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
    func_8001A0D8(&q, arg2);
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
    func_8001A0D8(&q, arg2);
}

void func_8001A6D4(void *arg0, u32 hidden) {
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
                x -= D_800569C0;
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
            func_8001A0D8(&q, f);
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
            func_8001A0D8(&q, f);
        }
    }
    if (p[7] & 0x8000) {
        func_8001A43C((u16 *) arg0 + 2, p[5], p[7] | hidden, ((s16 *) arg0)[12], ((s16 *) arg0)[13]);
    }
#undef p
}

void func_8001A82C(TextNAS *t, s32 arg1, s32 arg2) {
    GteSetup s;
    s32 x;

    if (!(t->NASid & 0x8000)) {
        return;
    }
    if (t->str == NULL) {
        return;
    }
    D_8005B2B4 = t->font;
    if (t->NASid & 0x10000000) {
        x = t->x;
        if ((x & 0xC000) == 0x8000) {
            x &= ~0xC000;
            x <<= 14;
            x >>= 14;
            x += D_800569C0;
            x &= ~0xC000;
            x |= 0x8000;
        } else {
            x = D_800569C0 + t->x;
        }
        func_800243A0(x, (s16)(D_800569C4 + t->y), t->str, t->c0, t->c1, 0);
        return;
    }
    D_80056998.unk84 = D_80056998.unkCC;
    D_80056998.unk88 = D_80056998.unk10C;
    func_8001965C(t, (u8 *)arg1, (u8 *)arg1 + 0x14, D_80056998.unk28, &s);
    gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
    if (s.trans != NULL) {
        gte_SetTransVector(s.trans);
    }
    gte_SetRotMatrix(s.rot);
    ((GlyphPos *)&s)->v[0] = t->x;
    ((GlyphPos *)&s)->v[1] = t->y;
    ((GlyphPos *)&s)->v[2] = t->z;
    SysPrintAt3d((GlyphPos *)&s, t->str, t->c0, t->c1);
}

static inline u8 *func_8001AA48_setup(ModelObj *obj, void *arg1, s32 flags, RelTable *colors, u8 *t) {
    func_8001D894(obj, arg1, flags, (s32)colors);
    return t;
}

void func_8001AA48(ModelObj *obj, void *arg1, s32 flags) {
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
    prim = obj->slots[D_8005B690 & 1].prim;
    if (prim == NULL) {
        prim = new_sprim();
        obj->slots[D_8005B690 & 1].prim = prim;
        if (prim == NULL) {
            return;
        }
        D_80056998.unk0 = obj->model;
        prim_create_entity_single(entity_address(obj->index, (u8 *)colors), prim);
    }
    t = entity_address(obj->index, (u8 *)colors);
    D_80056998.dp = D_80056998.modelDp;
    D_80056998.unk0 = obj->model;
    prim_colour_poly((RelTable *)t, prim, (s32)D_80056998.farColor, flags);
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
    otz = (otz + D_80056998.otzAdd) >> 1;
    if (otz < D_80056998.otLen) {
        addPrim(D_80056998.ot + otz, prim);
    }
}

MATRIX *func_8001ACC0(GfxXform *obj) {
    GfxState *gs;
    u32 flags;
    MATRIX *view;
    MATRIX *rot;
    MATRIX *out;
    GfxCamScale *cs;
    VECTOR v;
    s32 *pos;

    gs = &D_80056998;
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
                func_800142D4(obj->unk10);
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
            rot = &D_80058AD4;
        }
        pos = &D_80056998.cam->x;
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
                func_800142D4(obj->unk10);
            }
            MulMatrix0(rot, &obj->rot, out);
        }
    }
    D_80056998.otzAdd = ((GfxScreen *)D_80056998.unk28)->otz;
    return out;
}

s32 func_8001AF2C(void *arg0) {
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
    in = &D_800569CC.in;
    in = &box->center;
    gte_ldv0(in);
    in = &D_800569CC.in;
    gte_rtps();
    gte_stsxy(&c);
    gte_stsz(&sz);
    if (sz >= 2) {
        r = box->center.pad;
        gs = (GfxState *)((u8 *)in - 0x34);
        if (r < 0x400) {
            cam = gs->cam;
            d = sz * 3 << 10;
            e = r * cam->unk54.scale.vx * *(s32 *)cam->unk18 / d;
            if (c.vx + e < 0 || gs->clipX1 < c.vx - e) {
            culled:
                return 0;
            }
            ey = r * cam->unk54.scale.vy * *(s32 *)cam->unk18 / d;
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
        if ((u16)sxy[0].vx < (u16)D_80056998.clipX1 && (u16)sxy[0].vy < (u16)D_80056998.clipY1) {
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
        if ((u16)sxy[1].vx < (u16)D_80056998.clipX1 && (u16)sxy[1].vy < (u16)D_80056998.clipY1) {
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
        if ((u16)sxy[2].vx < (u16)D_80056998.clipX1 && (u16)sxy[2].vy < (u16)D_80056998.clipY1) {
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
        if ((u16)sxy[3].vx < (u16)D_80056998.clipX1 && (u16)sxy[3].vy < (u16)D_80056998.clipY1) {
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
        if ((u16)sxy[4].vx < (u16)D_80056998.clipX1 && (u16)sxy[4].vy < (u16)D_80056998.clipY1) {
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
        if ((u16)sxy[5].vx < (u16)D_80056998.clipX1 && (u16)sxy[5].vy < (u16)D_80056998.clipY1) {
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
        if ((u16)sxy[6].vx < (u16)D_80056998.clipX1 && (u16)sxy[6].vy < (u16)D_80056998.clipY1) {
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
        if ((u16)sxy[7].vx < (u16)D_80056998.clipX1 && (u16)sxy[7].vy < (u16)D_80056998.clipY1) {
            return 1;
        }
    } else {
        flags |= 0x80;
    }
    if (flags == 0xFF) {
        goto culled;
    }
    if (sxy[0].vx > D_80056998.clipX1 && sxy[1].vx > D_80056998.clipX1 &&
        sxy[2].vx > D_80056998.clipX1 && sxy[3].vx > D_80056998.clipX1 &&
        sxy[4].vx > D_80056998.clipX1 && sxy[5].vx > D_80056998.clipX1 &&
        sxy[6].vx > D_80056998.clipX1 && sxy[7].vx > D_80056998.clipX1) {
        goto culled;
    }
    if (sxy[0].vx < 0 && sxy[1].vx < 0 && sxy[2].vx < 0 && sxy[3].vx < 0 &&
        sxy[4].vx < 0 && sxy[5].vx < 0 && sxy[6].vx < 0 && sxy[7].vx < 0) {
        goto culled;
    }
    if (sxy[0].vy > D_80056998.clipY1 && sxy[1].vy > D_80056998.clipY1 &&
        sxy[2].vy > D_80056998.clipY1 && sxy[3].vy > D_80056998.clipY1 &&
        sxy[4].vy > D_80056998.clipY1 && sxy[5].vy > D_80056998.clipY1 &&
        sxy[6].vy > D_80056998.clipY1 && sxy[7].vy > D_80056998.clipY1) {
        return 0;
    }
    return 1;
}

s32 func_8001B6D4(SVECTOR *v) {
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
    farZ = (D_80056998.otLen - D_80056998.otzAdd) * 8;
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
        if ((u16)sxy[0][0] < (u16)D_80056998.clipX1 && (u16)sxy[0][1] < (u16)D_80056998.clipY1) {
            if (behind) return 2;
            inside |= 1;
        }
        if (farZ < sz[0]) far |= 1;
    } else {
        behind |= 1;
        if (inside) return 2;
    }
    if (sz[1] != 0) {
        if ((u16)sxy[1][0] < (u16)D_80056998.clipX1 && (u16)sxy[1][1] < (u16)D_80056998.clipY1) {
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
        if ((u16)sxy[2][0] < (u16)D_80056998.clipX1 && (u16)sxy[2][1] < (u16)D_80056998.clipY1) {
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
        if ((u16)sxy[i][0] < (u16)D_80056998.clipX1 && (u16)sxy[i][1] < (u16)D_80056998.clipY1) {    \
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
    clip = D_80056998.clipX1;
    if (sxy[0][0] > clip && sxy[1][0] > clip && sxy[2][0] > clip && sxy[3][0] > clip &&
        sxy[4][0] > clip && sxy[5][0] > clip && sxy[6][0] > clip && sxy[7][0] > clip) {
        return 0;
    }
    clip = D_80056998.clipY1;
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

void func_8001C008(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc) {
    SVECTOR *dst = D_80056AC8;
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

void func_8001C0F0(s32 p, MorphModel *model, u16 *ia, u16 *ib, s16 (*verts)[3]) {
    SVECTOR *dst = D_80056AC8;
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

void func_8001C1E0(u8 *arg0, u16 *src, u8 *tab, u16 *dst) {
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

void func_8001C270(EXTengine *eng) {
    eng->kill(eng);
}

void func_8001C298(EXTengine *eng) {
    eng->render(eng);
}

void func_8001C2C0(u8 *arg0) {
    s32 (*temp_v0)(u8 *, s32, s32);
    s32 temp_s1;
    s32 temp_s2;
    u8 *var_s0;

    var_s0 = (*(s32*)((u8*)(arg0) + 108));
    D_8005699C = arg0;
    if (var_s0 != NULL) {
        temp_s1 = (*(s32*)((u8*)(arg0) + 12));
        if (temp_s1 & 0x8000) {
            temp_s2 = func_8001ACC0(arg0 + 0xC);
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

subengine *func_8001C34C(s32 n, subengine *sub) {
    while (sub != NULL && n != 0) {
        sub = sub->next;
        n--;
    }
    return sub;
}


void kill_sprim_sub(subengine *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;

    temp_a0 = (*(s32*)((u8*)(arg0) + 144));
    if (temp_a0 != 0) {
        release_sprim(temp_a0);
        (*(s32*)((u8*)(arg0) + 144)) = 0;
    }
    temp_a0_2 = (*(s32*)((u8*)(arg0) + 156));
    if (temp_a0_2 != 0) {
        release_sprim(temp_a0_2);
        (*(s32*)((u8*)(arg0) + 156)) = 0;
    }
}

void func_8001C3D0(u8 *arg0) {
    u8 *temp_v0;
    u16 temp_a0;

    temp_v0 = entity_address(*(u16*)(arg0 + 0x74), *(u8**)(*(u8**)(arg0 + 0x6C) + 0xC));
    temp_a0 = *(u16*)(temp_v0 + 8);
    if (temp_a0 == 2 && temp_v0[*(s32*)(temp_v0 + 0x14) + 0x15] == temp_a0) {
        *(void**)(arg0 + 0x54) = func_8001AA48;
        *(void**)(arg0 + 0x58) = kill_sprim_sub;
    }
}

void func_8001C448(PosDst *arg0, s32 arg1, s32 arg2) {
    PosEntry *e;
    s32 sp10;
    s32 sp14;

    e = &D_8005689C[arg0->idx];
    switch (e->mode) {
    case 1:
        if (D_8005B690 & 4) {
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
        func_80024214(&sp10, &sp14);
        arg0->x = e->b[0];
        arg0->y = sp10;
        arg0->z = sp14;
        break;
    }
    func_8001A82C((TextNAS *)arg0, arg1, arg2);
}

void func_8001C584(PosDst *arg0, s32 arg1) {
    PosEntry *e;
    s32 sp10;
    s32 sp14;

    e = &D_8005689C[arg1];
    switch (e->mode) {
    case 1:
        if (D_8005B690 & 4) {
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
        func_80024214(&sp10, &sp14);
        arg0->x = e->b[0];
        arg0->y = sp10;
        arg0->z = sp14;
        return;
    }
}

void func_8001C690(NAS *nas, u8 *arg1) {
    GteSetup s;
    u32 hidden;

    if (nas->NASid & 0x8000) {
        hidden = nas->NASid & 0x10000000;
        if (nas->NASid & 0x02000000) {
            D_800569DC = nas->Class;
        } else {
            D_800569DC = 0;
        }
        if (!(nas->NASid & 0x10000000)) {
            D_80056998.unk84 = D_80056998.unkCC;
            D_80056998.unk88 = D_80056998.unk10C;
            func_8001965C(nas, arg1, arg1 + 0x14, D_80056998.unk28, &s);
            gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
            if (s.trans != NULL) {
                gte_SetTransVector(s.trans);
            }
            gte_SetRotMatrix(s.rot);
        }
        func_8001A6D4(&nas->Type, hidden);
    }
}

void func_8001C7FC(NAS *nas, u8 *arg1, u32 flags) {
    GteSetup s;
    u32 f;

    if (nas->NASid & 0x8000) {
        f = flags | nas->NASid;
        if (nas->NASid & 0x02000000) {
            D_800569DC = nas->Class;
        } else {
            D_800569DC = 0;
        }
        if (!(nas->NASid & 0x10000000)) {
            D_80056998.unk84 = D_80056998.unkCC;
            D_80056998.unk88 = D_80056998.unk10C;
            func_8001965C(nas, arg1, arg1 + 0x14, D_80056998.unk28, &s);
            gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
            if (s.trans != NULL) {
                gte_SetTransVector(s.trans);
            }
            gte_SetRotMatrix(s.rot);
        }
        f &= ~0x10000000;
        func_8001A0D8(&nas->Type, f | nas->NASid);
    }
}

u32 func_8001C974(NAS *nas, u8 *arg1, u32 flags) {
    GteSetup s;

    flags |= nas->NASid;
    if (nas->NASid & 0x02000000) {
        D_800569DC = nas->Class;
    } else {
        D_800569DC = 0;
    }
    if (!(nas->NASid & 0x10000000)) {
        D_80056998.unk84 = D_80056998.unkCC;
        D_80056998.unk88 = D_80056998.unk10C;
        func_8001965C(nas, arg1, arg1 + 0x14, D_80056998.unk28, &s);
        gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
        if (s.trans != NULL) {
            gte_SetTransVector(s.trans);
        }
        gte_SetRotMatrix(s.rot);
    }
    flags &= ~0x10000000;
    return flags | nas->NASid;
}

void func_8001CAD4(u16 *arg0, s32 arg1, u32 arg2) {
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
            x -= D_800569C0;
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
    func_8001A0D8(&q, arg2);
}

void func_8001CBD4(u32 arg0) {
    calc_rgbcode((arg0 >> 0x10) & 0xFF, (arg0 >> 8) & 0xFF, arg0 & 0xFF);
}

void func_8001CC08(void) {
}

s32 func_8001CC10(void *arg0, s32 arg1) {
    Func8001CC10Slot *slot;
    u16 want;

    s32 off;

    off = (D_8005B690 & 1) * 12 + 0x90;
    slot = (Func8001CC10Slot *)(arg1 + off);
    want = *(u16 *)((u8 *)arg0 + 8);
    if (want != slot->size) {
        if (slot->size < want) {
            if (slot->ptr != 0) {
                mal_free(slot, &D_8004E0F0);
            }
            func_800115D8(*(u16 *)((u8 *)arg0 + 8) * 0x28, slot, &D_8004E0F0, ((D_8005B690 & 1)?3:2));
            if (slot->ptr == 0) {
                slot->size = 0;
                return 0;
            }
            slot->size = *(u16 *)((u8 *)arg0 + 8);
        }
        func_80018728((s32)arg0, (s32)slot->ptr);
    }
    return (s32)slot->ptr;
}

void func_8001CD04(void *arg0, void *arg1, void *arg2) {
    GteSetup s;

    D_80056998.unk84 = D_80056998.unkCC;
    D_80056998.unk88 = D_80056998.unk10C;
    func_8001965C(arg0, arg1, arg2, D_80056998.unk28, &s);
    gte_SetGeomOffset(s.ofx * D_8005B698->w / 640, s.ofy / 2);
    if (s.trans != NULL) {
        gte_SetTransVector(s.trans);
    }
    gte_SetRotMatrix(s.rot);
}

void func_8001CE00(s32 arg0, ModelSet *set, s32 arg2) {
    void *p;
    u8 *blk;
    ModelEntry *e;

    arg0 &= 0xFFF;
    if (*(s32 *)((u8 *)set + 0x54) >= arg0) {
        e = &set->entries[arg0];
        blk = (u8 *)&e->off + e->off;
        if (!(arg2 & 0x1000000) && !func_8001AF2C(blk)) {
            return;
        }
        do {
            p = func_80019094(e, arg2);
        } while (0);
        blk += 0x14;
        if (p != NULL) {
            draw_verts(p, (SVECTOR *)blk, (u8 *)&e->runs + e->runs);
        }
    }
}
void func_8001CEB4(u16 arg0, u8 *arg1, s32 arg2) {
    void *p;
    u8 *blk;
    ModelEntry *e;

    e = (ModelEntry *)map_entity_address(arg0, arg1);
    if (e == NULL) {
        return;
    }
    blk = (u8 *)&e->off + e->off;
    if (!(arg2 & 0x1000000) && !func_8001AF2C(blk)) {
        return;
    }
    p = func_80019094(e, arg2);
    blk += 0x14;
    if (p != NULL) {
        draw_verts(p, (SVECTOR *)blk, (u8 *)&e->runs + e->runs);
    }
}

void *func_8001CF50(ModelEntry *e, s32 arg1) {
    u8 *blk;
    void *p;
    u8 *q;

    blk = (u8 *)&e->off + e->off;
    if ((arg1 & 0x1000000) == 0) {
        if (func_8001AF2C(blk) == 0) {
            return NULL;
        }
    }
    q = blk + 0x14;
    p = func_80019094(e, arg1);
    if (p != NULL) {
        draw_verts(p, (SVECTOR *)q,(u8 *)&e->runs + e->runs);
    }
    return p;
}

void *func_8001CFE8(MorphModel *model, void *arg1, s32 arg2) {
    void *p;

    p = func_80019094(model, arg2);
    if (p != NULL) {
        draw_verts(p, arg1, (u8 *)&model->runs + model->runs);
    }
    return p;
}

void func_8001D048(SVECTOR *v, POLY_GT4 *poly) {
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
    otz = (otz + D_80056998.otzAdd) >> 1;
    if (otz < D_80056998.otLen) {
        addPrim(D_80056998.ot + otz, poly);
    }
}

void func_8001D174(EXTengine *eng) {
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

void func_8001D220(EXTengine *eng) {
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

void func_8001D2B4(subengine *first, subengine *last) {
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

s32 func_8001D390(EXTengine *eng, s32 n) {
    return func_80018F3C(eng, n);
}

void func_8001D3B0(EXTengine *eng, s32 n, subengine *sub) {
    if (eng != NULL) {
        eng->sub = sub;
        eng->num_subs = n;
    }
    *sub = D_8005A830;
    n--;
    sub->prev = NULL;
    while (n != 0) {
        sub->next = sub + 1;
        sub++;
        *sub = D_8005A830;
        n--;
        sub->prev = sub - 1;
    }
    sub->next = NULL;
}

s32 func_8001D48C(subengine **out_first, subengine **last, s32 n) {
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
            D_8005A830.next = sub->next;
            D_8005A830.prev = sub->prev;
            *sub = D_8005A830;
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
            D_8005A830.next = sub->next;
            D_8005A830.prev = sub->prev;
            *sub = D_8005A830;
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
    D_80056998.unk8C.m[0][0] = 0x1000;
    D_80056998.unk8C.m[0][1] = 0;
    D_80056998.unk8C.m[0][2] = 0;
    D_80056998.unk8C.m[1][0] = 0;
    D_80056998.unk8C.m[1][1] = 0x1000;
    D_80056998.unk8C.m[1][2] = 0;
    D_80056998.unk8C.m[2][0] = 0;
    D_80056998.unk8C.m[2][1] = 0;
    D_80056998.unk8C.m[2][2] = 0x1000;
}


void func_8001D604(void) {
    if (D_80058AC8 != 0) {
        mal_free(D_80058AC8, &D_8004E0F0);
        D_80058AC8 = 0;
    }
}

void func_8001D640(s32 n) {
    subengine *sub;
    s32 i;
    u8 *p;

    p = func_80011654(n * 0xA8, &D_8004E0F0);
    i = n - 1;
    sub = (subengine *)p;
    D_80058AC8 = (s32)sub;
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

void func_8001D6B4(MapEnvHead *env) {
    subengine *sub;
    Placement *p;
    s32 n;

    sub = MapEnv_GetSubs(env);
    n = env->num_placements;
    p = env->placements;
    env->subs = sub;
    while (--n != -1) {
        *sub = D_8005A830;
        sub->flags = p->flags;
        *(SLONG_XYZ *)&sub->unk04 = p->pos;
        *(PlacementBlk *)&sub->unk2C[4] = p->blk;
        sub->hook = (void *)draw_map_sub;
        sub->unk6C = env->dbase;
        sub->unk74 = p->unk88;
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

void func_8001D894(void *sub, void *arg1, s32 flags, s32 arg3) {
    MATRIX m;
    VECTOR v;
    MATRIX *rot;
    MATRIX *mp;

    m = D_80056998.cam->view;
    D_80056998.unk0 = ((subengine *)sub)->unk6C = *(GfxModel **)D_80058AD0;
    v.vx = ((subengine *)sub)->unk04 - D_80056998.cam->x;
    v.vy = ((subengine *)sub)->unk08 - D_80056998.cam->y;
    v.vz = ((subengine *)sub)->unk0C - D_80056998.cam->z;
    mp = &m;
    if (!(flags & 0x200000)) {
        rot = &D_80056998.cam->view;
    } else {
        rot = &D_80058AD4;
    }
    ApplyMatrixLV(rot, &v, (VECTOR *)m.t);
    m.t[0] += D_80056998.cam->view.t[0];
    m.t[1] += D_80056998.cam->view.t[1];
    m.t[2] += D_80056998.cam->view.t[2];
    if (((subengine *)sub)->flags & 0x20000000) {
        gte_SetRotMatrix(&D_80056998.cam->rot);
    } else {
        if (*(u16 *)&((subengine *)sub)->unk52 != 0) {
            func_800142D4(&((subengine *)sub)->unk10);
        }
        MulMatrix0(rot, (MATRIX *)((u8 *)sub + 0x30), mp);
        gte_SetRotMatrix(mp);
    }
    gte_SetTransMatrix(mp);
    D_80056998.otzAdd = *(u16 *)(D_80056998.unk28 + 8) + ((subengine *)sub)->unk68;
    gte_SetGeomOffset(D_80056998.ofx * D_8005B698->w / 640, D_80056998.ofy / 2);
}

void func_8001DAF8(GfxScene *scene) {
    MATRIX m;
    MATRIX view;
    MATRIX *pv;
    u8 *obj;
    void (*fn)(u8 *, MATRIX *, s32);

    D_80058AD0 = scene;
    RotMatrix(&D_80058AF4, &m);
    MulMatrix0(&D_80056998.cam->view, &m, &D_80058AD4);
    pv = &view;
    view = D_80056998.cam->view;
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
    GfxSetSceneBg(&D_80056998, scene);
}

subengine *func_8001DCD4(s32 i, s32 val) {
    subengine *sub;

    for (sub = *(subengine **)((u8 *)D_80058AD0 + 0x1C); sub != NULL; sub = sub->next) {
        if (((u8 *)sub + i)[0x64] == val) {
            return sub;
        }
    }
    return NULL;
}

s32 func_8001DD20(s32 *arg0) {
    s32 v;

    v = ((s32 *)(arg0[1] + (s32)arg0))[1];
    if (v == 0) {
        return 0;
    }
    return v + arg0[0];
}


extern void func_8001D894(void *a, void *b, s32 c, s32 d);
void draw_map_sub(void *arg0, void *arg1) {
    s32 flags;
    s32 x;

    flags = *(s32 *)arg0;
    x = *(s32 *)(*(u8 **)((u8 *)arg0 + 0x6C) + 0xC);
    if ((flags & 0x8000) && x != 0) {
        func_8001D894(arg0, arg1, flags, x);
        func_80019A60(*(u16 *)((u8 *)arg0 + 0x74), (u8 *)x, *(s32 *)arg0, arg0);
    }
}

void map_update(s32 *arg0) {
    func_80029100(*arg0 + 0x10);
}

void func_8001DDD8(u8 *arg0) {
    func_80016F54((*(s32*)((u8*)(arg0) + 0)));
    mal_free((*(s32*)((u8*)(arg0) + 28)), &D_8004E0F0);
    (*(s32*)((u8*)(arg0) + 28)) = 0;
}

void func_8001DE18(arg0)
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

void func_8001DEF4(s32 *arg0, s32 *arg1) {
    u8 *p;

    arg0[0] = (s32)arg1;
    arg0[2] = 0;
    p = (u8 *)arg0[0] + 0x10;
    D_80058AD0 = arg0;
    if (arg1 == NULL) {
        arg0[12] = 0;
        arg0[1] &= ~3;
        return;
    }
    arg0[1] = 3;
    func_8002AA8C(p);
    func_80013650(arg1[1], map_loaded_textureset, arg0);
}

void map_loaded_textureset(u8 *arg0, u8 **arg1) {
    u8 *temp_s0;
    u8 *temp_s1;

    temp_s1 = *arg1;
    func_80011544(arg0, temp_s1 + 0x20);
    temp_s0 = temp_s1 + 0x10;
    func_8002A5A4(temp_s0, (*(s32*)((u8*)(temp_s0) + 16)));
    func_80029560(temp_s0);
    texture_release_loading_mem(temp_s0);
    (*(s32*)((u8*)(temp_s1) + 8)) = func_80013034((*(s32*)((u8*)(temp_s1) + 0)), 0, (*(s32*)((u8*)(temp_s1) + 36)), map_loaded_geo, arg1, 0, 0);
}

void map_loaded_geo(s32 arg0, s32 *arg1) {
    s32 *temp_s1;
    s32 *t;

    t = *(s32 **)arg1;
    D_80058AD0 = arg1;
    t[3] = arg0;
    temp_s1 = *(s32 **)arg1;
    arg1[2] = 0;
    func_8001DE18(arg1);
    arg1[7] = 0;
    func_8001D6B4(arg1);
    func_8001682C(temp_s1);
}

void func_8001E054(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 0)) = arg1;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    D_80058AD0 = arg0;
    func_8001DE18();
    (*(s32*)((u8*)(arg0) + 28)) = 0;
    func_8001D6B4(arg0);
    func_8001682C(arg1);
}

void func_8001E0A8(u8 *arg0) {
    u8 *dst;
    u8 *src;
    s32 n;
    s32 end;

    dst = *(u8 **)(arg0 + 0x1C);
    src = *(u8 **)(arg0 + 0x14);
    n = *(s32 *)(arg0 + 0x18);
    n--;
    if (n != -1) do {
        *(subengine *)dst = D_8005A830;
        *(s32 *)(dst + 0) = *(s32 *)(src + 0);
        *(Words3 *)(dst + 4) = *(Words3 *)(src + 4);
        *(Words4 *)(dst + 0x30) = *(Words4 *)(src + 0x28);
        *(Words4 *)(dst + 0x40) = *(Words4 *)(src + 0x38);
        *(void **)(dst + 0x54) = draw_map_sub;
        *(s32 *)(dst + 0x6C) = *(s32 *)arg0;
        *(u16 *)(dst + 0x74) = *(u16 *)(src + 0x88);
        *(u16 *)(dst + 0x8A) = *(u16 *)(src + 0x9E);
        *(u16 *)(dst + 0x68) = *(u16 *)(src + 0x9C);
        *(u8 *)(dst + 0x64) = *(u8 *)(src + 0x74);
        *(u8 *)(dst + 0x65) = *(u8 *)(src + 0x75);
        *(u8 *)(dst + 0x66) = *(u8 *)(src + 0x76);
        *(u8 *)(dst + 0x67) = *(u8 *)(src + 0x77);
        if (n > 0) {
            *(u8 **)(dst + 0x5C) = dst + 0xA8;
        } else {
            *(u8 **)(dst + 0x5C) = NULL;
        }
        src += 0xA0;
        dst += 0xA8;
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

KeyEntry *func_8001E21C(KeyBlock *arg0, s32 arg1, SLONG_XYZ *out) {
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

KeyEntry *func_8001E41C(KeyBlock *arg0, s32 arg1) {
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

s32 *path_get(s32 arg0) {
    s32 *p;
    s32 n;
    s32 i;
    s32 *q;

    p = NULL;
    if (*(s32 *)D_80058AD0 != 0) {
        p = *(s32 **)((u8 *)D_80058AD0 + 0x28);
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
        q = (s32 *)(*(u8 **)((u8 *)D_80058AD0 + 0x2C) + (i << 2));
        return (s32 *)((u8 *)q + *q);
    }
    return NULL;
}

s32 path_find(u32 arg0) {
    s32 *p;
    s32 n;
    s32 i;

    p = NULL;
    if (*(s32 *)D_80058AD0 != 0) {
        p = *(s32 **)((u8 *)D_80058AD0 + 0x28);
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

void set_fade(u8 *arg0, s32 arg1, s32 arg2) {
    (*(s32*)((u8*)(arg0) + 4)) = arg1;
    (*(s32*)((u8*)(arg0) + 12)) = arg2;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
}

s32 func_8001E598(u8 *arg0) {
    s32 step;
    s32 level;

    step = *(s32 *)(arg0 + 4);
    if (step != 0) {
        level = *(s32 *)(arg0 + 0) + step;
        *(s32 *)(arg0 + 0) = level;
        if (*(s32 *)(arg0 + 4) > 0) {
            if (level >= 0x1000) {
                *(s32 *)(arg0 + 0) = 0x1000;
                *(s32 *)(arg0 + 8) += 1;
            }
        } else if (level <= 0) {
            *(s32 *)(arg0 + 0) = 0;
            *(s32 *)(arg0 + 8) += 1;
            if (*(s32 *)(arg0 + 8) >= 3) {
                *(s32 *)(arg0 + 4) = 0;
            }
        }
    }
    return *(s32 *)(arg0 + 0);
}

void func_8001E610(u8 *mgr, u8 *fadeState) {
    ModeMgr *arg0 = (ModeMgr *)mgr;
    s32 *fade = (s32 *)fadeState;

    if (arg0->next != NULL && fade != NULL && (fade[1] == 0 || fade[2] >= 3)) {
        if (arg0->cur != NULL) {
            if (!(arg0->flags & 1) && arg0->cur->exit != NULL) {
                arg0->cur->exit();
            }
            if (fade != NULL && fade[1] != 0 && fade[2] >= 3) {
                func_8001888C();
                while (func_80011DB4(&D_8004E0F0) != NULL) {
                }
                func_80010AE8(&D_8004E0F0);
            }
        }
        arg0->prev = arg0->cur;
        arg0->cur = arg0->next;
        arg0->next = NULL;
        if (arg0->cur->heapStart != NULL) {
            func_8002AC70();
            while (func_80012FFC() != 0) {
                func_8001231C();
                func_80010AE8(&D_8004E0F0);
                func_8002BAE8();
            }
            func_80011BF8(&D_8004E0F0, arg0->cur->heapStart, 0x801FF000 - (s32)arg0->cur->heapStart);
            func_80012E54();
        }
        ((D5A614View *)D_8005A614)->unk30 &= ~1;
        if (!(arg0->flags & 2)) {
            if (arg0->flags & 0x10) {
                arg0->flags = 0;
            }
            if (arg0->cur->init != NULL) {
                arg0->cur->init();
            }
        } else if (arg0->flags & 0x10) {
            arg0->flags = 0;
        }
    }
    if (arg0->cur != NULL && !(arg0->flags & 8) && arg0->cur->update != NULL) {
        arg0->cur->update();
    }
}

void func_8001E824(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 12)) = 2;
    (*(s32*)((u8*)(arg0) + 4)) = (s32) (*(s32*)((u8*)(arg0) + 8));
}

void process_interrupt(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 4)) = arg1;
    (*(s32*)((u8*)(arg0) + 12)) = 1;
}

void func_8001E848(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 4)) = arg1;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
}

/* arg0 points to a table of handlers; calls handler 3 if there is one. */
void process_quit(void *arg0) {
    void (**ops)(void);
    void (*fn)(void);

    if (arg0 != NULL) {
        ops = *(void (***)(void))arg0;
        if (ops != NULL) {
            fn = ops[3];
            if (fn != NULL) {
                fn();
            }
        }
    }
}

void func_8001E898(u8 *arg0) {
    s32 (*temp_v1)();
    u8 *temp_v0;

    if (arg0 != NULL) {
        temp_v0 = (*(s32*)((u8*)(arg0) + 0));
        if (temp_v0 != NULL) {
            temp_v1 = (*(s32*)((u8*)(temp_v0) + 8));
            if ((temp_v1 != NULL) && !((*(s32*)((u8*)(arg0) + 12)) & 4)) {
                temp_v1();
            }
        }
    }
}

/* arg0 points to a table of handlers; calls handler 0 if there is one. */
void process_init(void *arg0) {
    void (**ops)(void);
    void (*fn)(void);

    if (arg0 != NULL) {
        ops = *(void (***)(void))arg0;
        if (ops != NULL) {
            fn = ops[0];
            if (fn != NULL) {
                fn();
            }
        }
    }
}

void func_8001E934(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 0)) = arg1;
    (*(s32*)((u8*)(arg0) + 4)) = 0;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
}

void func_8001E948(void) {
    func_8001888C();
    do {

    } while (func_80011DB4(&D_8004E0F0) != 0);
    func_80010AE8(&D_8004E0F0);
}

void func_8001E984(s32 arg0, s32 arg1) {
    u8 *p;
    u8 *next;

    if (arg0 != 0) {
        D_80058B6C = (s32)func_80011654(arg0 * 0x7C, &D_8004E0F0);
        p = (u8 *)D_80058B6C;
        for (; arg0 >= 2; arg0--) {
            next = p + 0x7C;
            *(u8 **)(p + 0x74) = next;
            p = next;
        }
        *(s32 *)(p + 0x74) = 0;
    } else {
        D_80058B6C = 0;
    }
    D_80058B78 = NULL;
    free_scripts = (void *)D_80058B6C;
    if (arg1 != 0) {
        D_80058B68 = (s32)func_80011654(arg1 * 0xB8, &D_8004E0F0);
        p = (u8 *)D_80058B68;
        for (; arg1 >= 2; arg1--) {
            next = p + 0xB8;
            *(u8 **)(p + 0xB4) = next;
            p = next;
        }
        *(s32 *)(p + 0xB4) = 0;
    } else {
        D_80058B68 = 0;
    }
    D_80058B64 = 0x10000;
    D_80058B70 = (void *)D_80058B68;
}

void func_8001EAA4(u8 *arg0, u8 *arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    u8 *track;
    ObjKey *key;
    ObjKey *k;
    u8 *d;
    s32 len;
    s32 t;
    s32 w;
    s32 f;

    e = *(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    track = res;
    if (track != NULL) {
        k = ObjTrack_FindKey((ObjKey *)(track + 0x24), *(s16 *)(arg1 + 0xE));
        key = k;
        d = arg0 + 8;
        f = key->len;
        d[0x51] = 3;
        if (f != 0) {
            len = f << 8;
            t = (*(s32 *)(arg1 + 0xC) >> 8) - (key->start << 8);
            CamVec3_Lerp((CamVec3 *)(arg0 + 0xC), &key->pos, &key[1].pos, len, t);
            func_80020B44((s32)&key->rot, (s32)&key[1].rot, (s32)(arg0 + 0x18), len, t, (s32)key->unk34);
            *(s16 *)(d + 0x52) = 1;
            CamVec3_Lerp((CamVec3 *)(arg0 + 0x28), &key->scale, &key[1].scale, len, t);
            w = interp(key->unk4C, key[1].unk4C, len, t);
        } else {
            *(CamVec3 *)(arg0 + 0xC) = key->pos;
            *(s16 *)(d + 0x52) = 1;
            *(ObjQuat *)(arg0 + 0x18) = key->rot;
            *(s32 *)(d + 0x20) = key->scale.v[0];
            *(s32 *)(d + 0x24) = key->scale.v[1];
            *(s32 *)(d + 0x28) = key->scale.v[2];
            w = key->unk4C;
        }
        w = 0x1000 - w;
        *(s32 *)d |= *(s32 *)(track + 0x18) | 0x8000;
        if (w != 0) {
            *(s16 *)(d + 0x76) = w;
            *(s32 *)d |= 0x40000000;
        }
        d[0x50] = 1;
        *(s16 *)(d + 0x74) = key->unk8;
    } else {
        *(s32 *)(arg0 + 8) &= ~0x8000;
    }
}

void update_camera_thread(u8 *arg0, u8 *arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    CamKey *key;
    CamState *cam;
    s32 len;
    s32 t;
    CamVec3 eye;
    CamVec3 at;
    u8 *track;

    e =*(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    track = res;
    if (track != NULL) {
        key = CamTrack_FindKey(track, *(s16 *)(arg1 + 0xE));
        cam = (CamState *)D_80051640;
        cam->unk8 = 0x800;
        cam->unk0 = 0;
        cam->unk4 = 0;
        cam->pos = key->at;
        cam->unk18 = *(s32 *)(track + 0x18);
        if (key->len != 0) {
            len = key->len << 8;
            t = (*(s32 *)(arg1 + 0xC) >> 8) - (key->start << 8);
            CamVec3_Lerp(&eye, &key->eye, &key[1].eye, len, t);
            CamVec3_Lerp(&at, &key->at, &key[1].at, len, t);
        } else {
            eye = key->eye;
            at = key->at;
        }
        func_800153B4((s32 *)&eye, (s32 *)&at, cam->rot);
        cam->pos = eye;
        cam->unk8 = 0;
        cam->unk0 = 0;
        cam->unk4 = 0;
    }
}

void func_8001F0D4(u8 *arg0, u8 *arg1) {
    AnimKey *k;
    AnimKey *key;
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    AnimRange *range;
    AnimPose *pose;
    s32 f;
    s32 frame;
    s32 len;
    s32 t;
    s32 a;

    e =*(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    pose = (AnimPose *)(arg0 + 8);
    if (res != NULL) {
        range = (AnimRange *)(res + 0x14);
        k = AnimTrack_FindKey((AnimKey *)(res + 0x30), *(s16 *)(arg1 + 0xE));
        key = k;
        f = *(s32 *)(arg1 + 0xC) - (*(s32 *)(res + 4) << 16);
        frame = AnimRange_Frame(range, f);
        pose->frame = frame;
        pose->unk74 = range->base + range->lo;
        len = key->len;
        if (len != 0) {
            len <<= 8;
            t = (*(s32 *)(arg1 + 0xC) >> 8) - (key->start << 8);
            func_80020B44((s32)key->rot, (s32)key[1].rot, (s32)pose->rot, len, t, (s32)key->unk30);
            CamVec3_Lerp(&pose->pos, &key->pos, &key[1].pos, len, t);
            CamVec3_Lerp(&pose->scale, &key->scale, &key[1].scale, len, t);
            a = interp(key->alpha, key[1].alpha, len << 8, t);
        } else {
            pose->pos = key->pos;
            *(AnimRot *)pose->rot = *(AnimRot *)key->rot;
            pose->scale.v[0] = key->scale.v[0];
            pose->scale.v[1] = key->scale.v[1];
            pose->scale.v[2] = key->scale.v[2];
            a = key->alpha;
        }
        a = 0x1000 - a;
        if (a != 0) {
            pose->unk76 = a;
            pose->flags |= 0x40000000;
        }
        pose->unk50 = 1;
        pose->unk51 = 3;
        pose->unk52 = 1;
        pose->flags |= 0x80008000;
    } else {
        pose->flags = 0;
    }
}

void func_8001F4F8(u8 *arg0, u8 *arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;

    e = *(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
        if (res != NULL) {
            D_80058B60 = interp(*(s32 *)(arg0 + 8), *(s32 *)(arg0 + 0xC),
                                       (*(s32 *)(res + 8) - *(s32 *)(res + 4)) << 8,
                                       (*(s32 *)(arg1 + 0xC) >> 8) - (*(s32 *)(res + 4) << 8));
        }
    }
}

void func_8001F604(u8 *obj, EmitPart *part, u8 *def) {
    s32 speed;
    s32 n;
    s32 yaw;
    s32 pitch;
    s32 radius;
    s32 r;

    yaw = *(s32 *)(def + 0x30);
    pitch = *(s32 *)(def + 0x38);
    radius = *(s32 *)(def + 0x2C);
    n = *(s32 *)(def + 0x24) - *(s32 *)(def + 0x20);
    if (n != 0) {
        speed = *(s32 *)(def + 0x20) + Random(n);
    } else {
        speed = *(s32 *)(def + 0x20);
    }
    part->age = 0;
    if (*(s32 *)(def + 0x68) != 0) {
        *(s16 *)(obj + 0x14) = Random(0x1000);
    }
    if (*(s32 *)(def + 0x34) < *(s32 *)(def + 0x30)) {
        r = abs((*(s32 *)(def + 0x34) - *(s32 *)(def + 0x30)) << 20 >> 20);
    } else {
        r = *(s32 *)(def + 0x34) - *(s32 *)(def + 0x30);
    }
    if (r != 0) {
        yaw = (*(s32 *)(def + 0x30) + Random(r)) & 0xFFF;
    }
    if (*(s32 *)(def + 0x3C) < *(s32 *)(def + 0x38)) {
        r = abs((*(s32 *)(def + 0x3C) - *(s32 *)(def + 0x38)) << 20 >> 20);
    } else {
        r = *(s32 *)(def + 0x3C) - *(s32 *)(def + 0x38);
    }
    if (r != 0) {
        pitch = (*(s32 *)(def + 0x38) + Random(r)) & 0xFFF;
    }
    part->vel[0] = speed * D_80068BD4[(yaw - 0x400) & 0xFFF].unk2 >> 4;
    part->vel[2] = speed * D_80068BD4[yaw & 0xFFF].unk2 >> 4;
    part->vel[1] = -(speed * D_80068BD4[pitch & 0xFFF].unk2 >> 4);
    *(s32 *)(obj + 4) = (radius * D_80068BD4[(yaw - 0x400) & 0xFFF].unk2 >> 12) + *(s32 *)(def + 4);
    *(s32 *)(obj + 0xC) = (radius * D_80068BD4[yaw & 0xFFF].unk2 >> 12) + *(s32 *)(def + 0xC);
    *(s32 *)(obj + 8) = *(s32 *)(def + 8) - (radius * D_80068BD4[pitch & 0xFFF].unk2 >> 12);
    *(s32 *)(obj + 0) |= 0x8000;
}

void func_8001F828(u8 *arg0, u8 *arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    u8 *e;
    u8 *obj;
    EmitPart *part;
    u8 *def;
    s32 spawn;
    s32 a;
    s32 scale;
    s32 in;

    obj = *(u8 **)(arg0 + 0xC);
    part = *(EmitPart **)(arg0 + 0x14);
    e = *(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res == NULL) {
        return;
    }
    def = res + 0x14;
    if (*(s16 *)(arg1 + 0xE) == *(s32 *)(res + 4)) {
        *(s32 *)(arg0 + 0x18) = *(s32 *)(def + 0x10);
    }
    if (*(s32 *)(res + 4) >= *(s16 *)(arg1 + 0xE)) {
        return;
    }
    if (*(s32 *)(arg0 + 0x18) != 0 || (spawn = 0, *(s32 *)(def + 0x6C) != 0)) {
        spawn = *(s32 *)(def + 0x14);
    }
    if (*(s32 *)(def + 0x1C) < *(s16 *)(arg1 + 0xE)) {
        spawn = 0;
    }
    *(s32 *)(arg0 + 0x18) -= spawn;
    if (*(s32 *)(arg0 + 0x18) < 0) {
        *(s32 *)(arg0 + 0x18) = 0;
    }
    for (;; obj = *(u8 **)(obj + 0x5C), part++) {
        if (obj == NULL) {
            return;
        }
        if (*(s32 *)obj & 0x8000) {
            if (part->vel[0] != 0) {
                *(s32 *)(obj + 4) += part->vel[0] >> 8;
            }
            if (part->vel[1] != 0) {
                *(s32 *)(obj + 8) += part->vel[1] >> 8;
            }
            if (part->vel[2] != 0) {
                *(s32 *)(obj + 0xC) += part->vel[2] >> 8;
            }
            if (*(s32 *)(def + 0x40) != 0) {
                part->vel[0] += *(s32 *)(def + 0x40);
            }
            if (*(s32 *)(def + 0x48) != 0) {
                part->vel[1] += *(s32 *)(def + 0x48);
            }
            if (*(s32 *)(def + 0x50) != 0) {
                part->vel[2] += *(s32 *)(def + 0x50);
            }
            if (*(s32 *)(def + 0x44) != 0x100) {
                part->vel[0] = (part->vel[0] * *(s32 *)(def + 0x44)) >> 8;
            }
            if (*(s32 *)(def + 0x4C) != 0x100) {
                part->vel[1] = (part->vel[1] * *(s32 *)(def + 0x4C)) >> 8;
            }
            if (*(s32 *)(def + 0x54) != 0x100) {
                part->vel[2] = (part->vel[2] * *(s32 *)(def + 0x54)) >> 8;
            }
            *(u16 *)(obj + 0x14) += *(u16 *)(def + 0x68);
            *(s16 *)(obj + 0x52) = 1;
            if (*(s32 *)(def + 0x18) < part->age++) {
                *(s32 *)obj &= ~0x8000;
            }
            if (*(s32 *)obj & 0x8000) {
                in = *(s32 *)(def + 0x58);
                a = 0;
                if (in != 0 || *(s32 *)(def + 0x5C) != 0) {
                    a = EmitPart_Fade(in, *(s32 *)(def + 0x5C), part->age, *(s32 *)(def + 0x18), 0x1000, 0);
                }
                if (a != 0) {
                    *(s32 *)obj |= 0x40000000;
                } else {
                    *(s32 *)obj &= ~0x40000000;
                }
                *(s16 *)(obj + 0x76) = a;
                in = *(s32 *)(def + 0x60);
                scale = 0x1000;
                if (in != 0 || *(s32 *)(def + 0x64) != 0) {
                    scale = EmitPart_Fade(in, *(s32 *)(def + 0x64), part->age, *(s32 *)(def + 0x18), 0, 0x1000);
                }
                *(s32 *)(obj + 0x28) = scale;
                *(s32 *)(obj + 0x24) = scale;
                *(s32 *)(obj + 0x20) = scale;
                *(u8 *)(obj + 0x50) = 1;
                *(s16 *)(obj + 0x52) = 1;
            }
        }
        if (spawn != 0 && !(*(s32 *)obj & 0x8000)) {
            *(s32 *)obj |= 0x8000;
            func_8001F604(obj, part, def);
            spawn--;
        }
    }
}

void func_8001FCDC(u8 *arg0, u8 *arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    u8 *d;
    u32 t;

    e = *(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    d = arg0 + 8;
    if (res != NULL && *(s32 *)(arg1 + 0xC) >= (*(s32 *)(res + 4) << 16) &&
        !(*(s32 *)(arg0 + 8) & 0x8000)) {
        *(s32 *)(d + 4) = *(s32 *)(res + 0x2C);
        *(s32 *)(d + 8) = *(s32 *)(res + 0x30);
        *(s32 *)(d + 0xC) = *(s32 *)(res + 0x34);
        *(u16 *)(d + 0x10) = *(u16 *)(res + 0x38);
        *(u16 *)(d + 0x12) = *(u16 *)(res + 0x3C);
        *(u16 *)(d + 0x14) = *(u16 *)(res + 0x40);
        *(s32 *)(d + 0x20) = *(s32 *)(res + 0x60);
        *(s32 *)(d + 0x24) = *(s32 *)(res + 0x64);
        *(s32 *)(d + 0x28) = *(s32 *)(res + 0x68);
        *(u8 *)(d + 0x51) = 0;
        t = *(u16 *)(res + 0x18);
        *(u16 *)(d + 0x52) = 1;
        *(u16 *)(d + 0x68) = t;
        sub_play_script(*(u8 **)arg1, *(s32 *)(res + 0x14), d, 0);
        *(s32 *)(arg0 + 8) |= 0x8000;
    }
}

u8 *func_8001FE80(u8 *arg0, s32 arg1, Blk32 *arg2, CamVec3 *arg3, s32 arg4, u8 **arg5) {
    u8 *p;
    u8 *e;
    u8 *hdr;
    s32 *t;
    s32 *q;
    s32 i;
    u8 *prev;
    u8 *n;
    void (*fn)(s32 *, u8 *, s32);

    if (*(u8 **)(arg0 + 0xC) == NULL) {
        return NULL;
    }
    n = free_scripts;
    if (n != NULL) {
        free_scripts = *(u8 **)(n + 0x74);
        if (D_80058B78 != NULL) {
            *(u8 **)((u8 *)D_80058B78 + 0x70) = n;
        }
        prev = D_80058B78;
        *(u8 **)(n + 0x70) = NULL;
        D_80058B78 = n;
        *(u8 **)(n + 0x74) = prev;
    }
    p = n;
    if (p != NULL) {
        *(CamVec3 *)(p + 0x20) = *arg3;
        *(Blk32 *)(p + 0x4C) = *arg2;
        *(s32 *)(p + 0x1C) = 0x8000;
        p[0x6D] = 4;
        p[0x6C] = 0;
        *(s16 *)(p + 0x6E) = 0;
        e = *(u8 **)(arg0 + 0xC);
        hdr = NULL;
        if (e != NULL) {
            s32 *r = (s32 *)(e + (*(s32 *)(e + 0x4C) + 0x4C)) + arg1;
            hdr = (u8 *)r + *r;
        }
        *(s32 *)(p + 0x14) = arg4;
        *(u8 **)(p + 0x0) = arg0;
        *(s32 *)(p + 0x4) = arg1;
        *(s32 *)(p + 0x8) = 0;
        *(s32 *)(p + 0xC) = 0;
        *(s16 *)(p + 0xE) = *(s16 *)(hdr + 0xC);
        *(s32 *)(p + 0x10) = *(s32 *)(hdr + 0x8);
        *(u8 ***)(p + 0x18) = arg5;
        if (arg5 != NULL) {
            *arg5 = p;
        }
        t = (s32 *)(hdr + 0x1C);
        for (i = 0; i < *(s32 *)hdr; i++) {
            q = (s32 *)((u8 *)t + *t);
            fn = (void (*)(s32 *, u8 *, s32))D_80058B00[*q].unk0;
            if (fn != NULL) {
                fn(q, p, i);
            }
            t++;
        }
        func_80020064(p);
    }
    return p;
}

s32 func_80020064(u8 *arg0) {
    u8 *s;
    s32 *a;
    u8 *c;
    void (*fn)(u8 *, u8 *);
    s32 idx;

    s = *(u8 **)(arg0 + 8);
    D_80058B60 = 0;
    while (s != NULL) {
        fn = (void (*)(u8 *, u8 *))D_80058B00[*(s32 *)(s + 4)].unk4;
        if (fn != NULL) {
            fn(s, arg0);
        }
        s = *(u8 **)(s + 0xB4);
    }
    *(s32 *)(arg0 + 0xC) = *(s32 *)(arg0 + 0xC) + D_80058B64;
    c = *(u8 **)(*(u8 **)arg0 + 0xC);
    idx = *(s32 *)(arg0 + 4);
    if (c == NULL) {
        a = NULL;
    } else {
        s32 *t = (s32 *)(c + (*(s32 *)(c + 0x4C) + 0x4C) + idx * 4);
        a = (s32 *)((u8 *)t + *t);
    }
    if (*(s16 *)(arg0 + 0xE) >= a[2]) {
        *(s32 *)(arg0 + 0x14) |= 0x10;
        if (*(s32 *)(arg0 + 0x14) & 8) {
            *(s32 *)(arg0 + 0xC) = (a[2] - 1) << 16;
        }
        if (*(s32 *)(arg0 + 0x14) & 2) {
            *(s16 *)(arg0 + 0xE) = *(u16 *)(a + 3);
        }
        if (*(s32 *)(arg0 + 0x14) & 4) {
            func_80021054(arg0);
            return 0;
        }
    }
    return 1;
}

void func_800201BC(u8 *arg0, GteSetup *arg1) {
    MATRIX m;
    GteSetup out;
    s32 dp;
    u8 *p;
    GfxState *gs;
    void (*fn)(u8 *, GteSetup *);

    dp = D_80056998.modelDp;
    p = *(u8 **)(arg0 + 8);
    D_80056998.modelDp =0x1000 - (((0x1000 - D_80058B60) * (0x1000 - dp)) >> 12);
    m = *arg1->rot;
    m.t[0] = arg1->trans->vx;
    m.t[1] = arg1->trans->vy;
    m.t[2] = arg1->trans->vz;
    out.rot = &m;
    out.trans = arg1->trans;
    out.ofx = arg1->ofx;
    out.ofy = arg1->ofy;
    for (; p != NULL;) {
        gs = &D_80056998;
        ((GfxScreen *)gs->unk28)->x = arg1->ofx;
        ((GfxScreen *)gs->unk28)->y = arg1->ofy;
        fn = (void (*)(u8 *, GteSetup *))D_80058B00[*(s32 *)(p + 4)].unk8;
        if (fn != NULL) {
            fn(p, &out);
        }
        p = *(u8 **)(p + 0xB4);
    }
    D_800569AC = dp;
}

void func_80020344(Quat *a, Quat *b, s32 t, Quat *out) {
    Quat q;
    s32 d;
    s32 sq;
    s32 s;
    s32 ang;
    s32 inv;
    s32 ka;
    s32 kb;

    q.v[0] = b->v[0];
    q.v[1] = b->v[1];
    q.v[2] = b->v[2];
    q.v[3] = b->v[3];
    d = QuatDot(a, &q);
    if (d < 0) {
        q.v[0] = -q.v[0];
        q.v[1] = -q.v[1];
        q.v[2] = -q.v[2];
        q.v[3] = -q.v[3];
        d = QuatDot(a, &q);
    }
    sq = d * d;
    s = csqrt(abs(0x1000 - ((sq >= 0 ? sq + 0x800 : sq - 0x800) >> 12)));
    if (abs(s) < 0x80) {
        out->v[0] = a->v[0];
        out->v[1] = a->v[1];
        out->v[2] = a->v[2];
        out->v[3] = a->v[3];
        return;
    }
    ang = get_angle(s, d);
    inv = 0x1000000 / s;
    ka = (D_80068BD4[((((0x1000 - t) * ang >> 12) & 0xFFF) - 0x400) & 0xFFF].unk2 * inv) >> 6;
    kb = (D_80068BD4[(((t * ang >> 12) & 0xFFF) - 0x400) & 0xFFF].unk2 * inv) >> 6;
    out->v[0] = (ka * a->v[0] + kb * q.v[0]) >> 18;
    out->v[1] = (ka * a->v[1] + kb * q.v[1]) >> 18;
    out->v[2] = (ka * a->v[2] + kb * q.v[2]) >> 18;
    out->v[3] = (ka * a->v[3] + kb * q.v[3]) >> 18;
}

/* Cosine at angle x / 2, linearly interpolated for odd x. */
static inline s32 Trig_HalfCos(s32 x) {
    if (x & 1) {
        return (D_80068BD4[((x >> 1) - 0x400) & 0xFFF].unk2 + D_80068BD4[((x >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    }
    return D_80068BD4[((x >> 1) - 0x400) & 0xFFF].unk2;
}

void func_80020680(Quat *a, Quat *b, s32 t, Quat *out, s32 *pre) {
    Quat q;
    s32 d;
    s32 sq;
    s32 s;
    s32 ang;
    s32 inv;
    s32 ia;
    s32 ib;
    s32 ka;
    s32 kb;
    s32 pb;

    q.v[0] = b->v[0];
    q.v[1] = b->v[1];
    q.v[2] = b->v[2];
    q.v[3] = b->v[3];
    d = QuatDot(a, &q);
    if (d < 0) {
        q.v[0] = -q.v[0];
        q.v[1] = -q.v[1];
        q.v[2] = -q.v[2];
        q.v[3] = -q.v[3];
        d = QuatDot(a, &q);
    }
    sq = d * d;
    s = csqrt(abs(0x1000 - ((sq >= 0 ? sq + 0x800 : sq - 0x800) >> 12)));
    if (abs(s) < 0x80) {
        out->v[0] = a->v[0];
        out->v[1] = a->v[1];
        out->v[2] = a->v[2];
        out->v[3] = a->v[3];
        return;
    }
    get_angle(s, d);
    ang = pre[1];
    inv = pre[2];
    ib = t * ang;
    if (ib > 0) {
        ib += 0x400;
    } else {
        ib -= 0x400;
    }
    ib >>= 11;
    ia = (0x1000 - t) * ang;
    if (ia > 0) {
        ia += 0x400;
    } else {
        ia -= 0x400;
    }
    ia >>= 11;
    ka = Trig_HalfCos(ia) * inv;
    if (ib & 1) {
        ang = (D_80068BD4[((ib >> 1) - 0x400) & 0xFFF].unk2 + D_80068BD4[((ib >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    } else {
        ang = D_80068BD4[((ib >> 1) - 0x400) & 0xFFF].unk2;
    }
    ia = ang * inv;
    if (ka > 0) {
        ka += 0x10;
    } else {
        ka -= 0x10;
    }
    ka >>= 5;
    if (ia > 0) {
        ia += 0x10;
    } else {
        ia -= 0x10;
    }
    ia >>= 5;
    kb = ka * a->v[0];
    ib = ia * q.v[0];
    out->v[0] = (((kb >= 0 ? kb + 0x800 : kb - 0x800) >> 12) + ((ia * q.v[0] >= 0 ? ib + 0x800 : ib - 0x800) >> 12)) >> 7;
    kb = ka * a->v[1];
    ib = ia * q.v[1];
    out->v[1] = (((kb >= 0 ? kb + 0x800 : kb - 0x800) >> 12) + ((ia * q.v[1] >= 0 ? ib + 0x800 : ib - 0x800) >> 12)) >> 7;
    kb = ka * a->v[2];
    ib = ia * q.v[2];
    out->v[2] = (((kb >= 0 ? kb + 0x800 : kb - 0x800) >> 12) + ((ia * q.v[2] >= 0 ? ib + 0x800 : ib - 0x800) >> 12)) >> 7;
    pb = ka * a->v[3];
    kb = ia * q.v[3];
    out->v[3] = (((pb >= 0 ? pb + 0x800 : pb - 0x800) >> 12) + ((ia * q.v[3] >= 0 ? kb + 0x800 : kb - 0x800) >> 12)) >> 7;
}

void func_80020B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    func_80020680(a0, a1, (a4 << 12) / a3, a2, a5);
}

void func_80020BA8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    func_80020344(a0, a1, (a4 << 12) / a3, a2);
}

/* No callers. Retail multiplies (b - a) by the fraction but never reads lo
 * (no mflo) and returns a: the interpolating return sits behind a test that
 * is always false, which only combine sees (after flow), so the mult stays
 * and the dead block is dropped after reload. */
s32 func_80020C04(s32 arg0) {
    s32 a;
    s32 b;
    s32 t;

    a = D_80068BD4[((arg0 >> 16) - 0x400) & 0xFFF].unk2;
    b = D_80068BD4[((arg0 >> 16) - 0x3FF) & 0xFFF].unk2;
    t = (b - a) * (arg0 & 0xFFFF);
    if (!((arg0 & 0x7FFF) >> 15)) {
        return a;
    }
    return a + (t >> 16);
}

s16 func_80020C4C(s32 arg0) {
    if (arg0 & 1) {
        return (D_80068BD4[((arg0 >> 1) - 0x400) & 0xFFF].unk2 + D_80068BD4[((arg0 >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    }
    return D_80068BD4[((arg0 >> 1) - 0x400) & 0xFFF].unk2;
}

void sub_play_script(u8 *arg0, s32 arg1, u8 *arg2, s32 arg3) {
    u8 *p;
    u8 *e;
    s32 *s;
    s32 *q;
    s32 i;
    void (*fn)(u8 *, u8 *, s32);
    u8 *base;

    p = arg2 + 0x6C;
    base = *(u8 **)(arg0 + 0xC);
    if (base == NULL) {
        e = NULL;
    } else {
        s = (s32 *)(base + (*(s32 *)(base + 0x4C) + 0x4C)) + arg1;
        e = (u8 *)s + *s;
    }
    *(s32 *)(p + 0x14) = arg3;
    *(u8 **)p = arg0;
    *(s32 *)(p + 4) = arg1;
    *(s32 *)(p + 8) = 0;
    *(s32 *)(p + 0xC) = 0;
    *(u16 *)(p + 0xE) = *(u16 *)(e + 0xC);
    q = (s32 *)(e + 0x1C);
    *(s32 *)(p + 0x10) = *(s32 *)(e + 8);
    *(s32 *)(p + 0x18) = 0;
    i = 0;
    if (*(s32 *)e > 0) {
        do {
            u8 *a = (u8 *)q + *q;
            fn = (void (*)(u8 *, u8 *, s32))D_80058B00[*(s32 *)a].unk0;
            if (fn != NULL) {
                fn(a, p, i);
            }
            q++;
        } while (++i < *(s32 *)e);
    }
    func_80020064(p);
    *(void **)(arg2 + 0x54) = func_80020E9C;
    *(void **)(arg2 + 0x58) = func_80020DF8;
}

void func_80020DF8(u8 *arg0) {
    u8 *p;
    u8 *cur;
    u8 *next;
    s32 *flag;
    void *old;
    D_80058B00_Entry *tab;
    void (*fn)(u8 *, u8 *);

    p = arg0 + 0x6C;
    flag = *(s32 **)(p + 0x18);
    cur = *(u8 **)(p + 8);
    if (flag != NULL) {
        *flag = 0;
    }
    tab = D_80058B00;
    for (;;) {
        if (cur == NULL) {
            return;
        }
        fn = tab[*(s32 *)(cur + 4)].unkC;
        next = *(u8 **)(cur + 0xB4);
        if (fn != NULL) {
            fn(cur, p);
        }
        old = D_80058B70;
        D_80058B70 = cur;
        *(void **)(cur + 0xB4) = old;
        cur = next;
    }
}

void func_80020E9C(NAS *nas, u8 *arg1) {
    struct { s32 a; s32 b; } saved;
    GteSetup s;
    u8 *sub;

    if (nas->NASid & 0x8000) {
        sub = (u8 *)nas + 0x6C;
        if (func_80020064(sub) == 0) {
            nas->NASid &= ~0x8000;
            return;
        }
        saved = *(typeof(saved) *)D_80056998.unk28;
        func_8001965C(nas, arg1, arg1 + 0x14, D_80056998.unk28, &s);
        func_800201BC(sub, &s);
        *(typeof(saved) *)D_80056998.unk28 = saved;
    }
}

void func_80020F5C(u8 *arg0) {
    s32 *p;
    u8 *cur;
    u8 *next;
    void *head;
    u8 *prev;
    u8 *nxt;
    void *head2;
    void (*fn)(u8 *, u8 *);

    p = *(s32 **)(arg0 + 0x18);
    cur = *(u8 **)(arg0 + 8);
    if (p != NULL) {
        *p = 0;
    }
    for (;;) {
        if (cur == NULL) {
            goto unlink;
        }
        fn = D_80058B00[*(s32 *)(cur + 4)].unkC;
        next = *(u8 **)(cur + 0xB4);
        if (fn != NULL) {
            fn(cur, arg0);
        }
        head = D_80058B70;
        D_80058B70 = cur;
        *(void **)(cur + 0xB4) = head;
        cur = next;
    }
unlink:
    prev = *(u8 **)(arg0 + 0x70);
    if (prev != NULL) {
        *(s32 *)(prev + 0x74) = *(s32 *)(arg0 + 0x74);
    } else {
        D_80058B78 = *(void **)(arg0 + 0x74);
    }
    nxt = *(u8 **)(arg0 + 0x74);
    if (nxt != NULL) {
        *(s32 *)(nxt + 0x70) = *(s32 *)(arg0 + 0x70);
    }
    head2 = free_scripts;
    free_scripts = arg0;
    *(void **)(arg0 + 0x74) = head2;
}

void func_80021054(u8 *arg0) {
    s32 *p;
    u8 *cur;
    u8 *next;
    void *head;
    void (*fn)(u8 *, u8 *);

    p = *(s32 **)(arg0 + 0x18);
    cur = *(u8 **)(arg0 + 8);
    if (p != NULL) {
        *p = 0;
    }
    for (;;) {
        if (cur == NULL) {
            return;
        }
        fn = D_80058B00[*(s32 *)(cur + 4)].unkC;
        next = *(u8 **)(cur + 0xB4);
        if (fn != NULL) {
            fn(cur, arg0);
        }
        head = D_80058B70;
        D_80058B70 = cur;
        *(void **)(cur + 0xB4) = head;
        cur = next;
    }
}

void func_800210F8(u8 *arg0, s32 arg1, u8 *arg2, s32 arg3, u8 **arg4) {
    u8 *e;
    u8 *hdr;
    s32 *t;
    s32 *q;
    s32 i;
    void (*fn)(s32 *, u8 *, s32);

    e = *(u8 **)(arg0 + 0xC);
    if (e != NULL) {
        s32 *p = (s32 *)(e + (*(s32 *)(e + 0x4C) + 0x4C)) + arg1;
        hdr = (u8 *)p + *p;
    } else {
        hdr = NULL;
    }
    *(s32 *)(arg2 + 0x14) = arg3;
    *(u8 **)(arg2 + 0x0) = arg0;
    *(s32 *)(arg2 + 0x4) = arg1;
    *(s32 *)(arg2 + 0x8) = 0;
    *(s32 *)(arg2 + 0xC) = 0;
    *(s16 *)(arg2 + 0xE) = *(s16 *)(hdr + 0xC);
    *(s32 *)(arg2 + 0x10) = *(s32 *)(hdr + 0x8);
    *(u8 ***)(arg2 + 0x18) = arg4;
    if (arg4 != NULL) {
        *arg4 = arg2;
    }
    t = (s32 *)(hdr + 0x1C);
    for (i = 0; i < *(s32 *)hdr; i++) {
        q = (s32 *)((u8 *)t + *t);
        fn = (void (*)(s32 *, u8 *, s32))D_80058B00[*q].unk0;
        if (fn != NULL) {
            fn(q, arg2, i);
        }
        t++;
    }
    func_80020064(arg2);
}

void func_8002120C(u8 *arg0) {
    u8 *p;

    p = arg0 + 8;
    (*(void (**)(u8 *))(p + 0x58))(p);
}

void func_80021238(u8 *arg0, s32 *arg1) {
    s32 saved;
    u32 flags;
    subengine *sub;
    s32 val;

    sub = (subengine *)(arg0 + 8);
    flags = *(u32 *)(arg0 + 8);
    val = *arg1;
    saved = D_80058B60;
    if (flags & 0x8000) {
        sub->hook(sub, val, flags);
    }
    D_80058B60 = saved;
}

static inline u8 *Pop4(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

static inline u8 *Push4(u8 *arg1) {
    u8 *p;

    p = Pop4();
    if (p != NULL) {
        *(u8 **)(p + 0xB4) = *(u8 **)(arg1 + 8);
        *(u8 **)(arg1 + 8) = p;
    }
    return p;
}

void func_8002128C(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;
    subengine *sub;

    p = Push4(arg1);
    if (p != NULL) {
        sub = (subengine *)(p + 8);
        *(s32 *)(p + 4) = 5;
        *(s32 *)p = arg2;
        *sub = D_8005A830;
        *(s32 *)sub = 0;
    }
}

void func_80021330(u8 *arg0, s32 *arg1) {
    subengine *first;
    subengine *sub;
    s32 val;
    u32 flags;

    first = *(subengine **)(arg0 + 0xC);
    val = *arg1;
    if (first != NULL) {
        sub = first;
        flags = sub->flags;
        for (;;) {
            if (sub == NULL) {
                return;
            }
            if (sub->hook != NULL) {
                sub->hook(sub, val, flags);
            }
            sub = sub->next;
        }
    }
}

s32 func_800213A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if (arg5 < 0) {
        return arg0;
    }
    if (arg5 < arg2) {
        return interp(arg0, arg1, arg2, arg5);
    }
    if (arg3 < arg5) {
        if (arg4 < arg5) {
            return arg0;
        }
        return interp(arg1, arg0, arg4 - arg3, arg5 - arg3);
    }
    return arg1;
}

void func_8002141C(u8 *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;

    temp_a0 = (*(s32*)((u8*)(arg0) + 12));
    if (temp_a0 != 0) {
        func_8001D2B4(temp_a0, (*(s32*)((u8*)(arg0) + 16)));
        (*(s32*)((u8*)(arg0) + 12)) = 0;
    }
    temp_a0_2 = (*(s32*)((u8*)(arg0) + 20));
    if (temp_a0_2 != 0) {
        mal_free(temp_a0_2, &D_8004E0F0);
        (*(s32*)((u8*)(arg0) + 20)) = 0;
    }
}

void func_8002147C(VelObj *obj, VelParams *p, VelState *s) {
    if (s->vx != 0) {
        obj->x += s->vx >> 8;
    }
    if (s->vy != 0) {
        obj->y += s->vy >> 8;
    }
    if (s->vz != 0) {
        obj->z += s->vz >> 8;
    }
    if (p->addX != 0) {
        s->vx += p->addX;
    }
    if (p->addY != 0) {
        s->vy += p->addY;
    }
    if (p->addZ != 0) {
        s->vz += p->addZ;
    }
    if (p->mulX != 0x100) {
        s->vx = (s->vx * p->mulX) >> 8;
    }
    if (p->mulY != 0x100) {
        s->vy = (s->vy * p->mulY) >> 8;
    }
    if (p->mulZ != 0x100) {
        s->vz = (s->vz * p->mulZ) >> 8;
    }
    obj->angle += p->dAngle;
    obj->dirty = 1;
    if (p->limit < s->frame++) {
        obj->flags &= ~0x8000;
    }
}

void func_80021604(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;
    u8 *e;
    u8 *q;
    s32 v;
    u16 w;

    p = Push4(arg1);
    if (p != NULL) {
        e = arg0 + 0x14;
        *(s32 *)(p + 4) = 1;
        *(s32 *)p = arg2;
        *(s32 *)(p + 0xC) = 0;
        *(s32 *)(p + 0x10) = 0;
        *(s32 *)(p + 8) = func_8001D48C(p + 0xC, p + 0x10, *(s32 *)(e + 0x10));
        q = *(u8 **)(p + 0xC);
        if (q != NULL) {
            do {
                *(s32 *)q = 0;
                v = *(s32 *)arg1;
                *(s32 *)(q + 0x6C) = v;
                w = *(u16 *)(arg0 + 0x3C);
                *(u8 *)(q + 0x51) = 0;
                *(s16 *)(q + 0x52) = 1;
                *(u16 *)(q + 0x74) = w;
                q = *(u8 **)(q + 0x5C);
            } while (q != NULL);
        }
        *(s32 *)(p + 0x14) = 0;
        func_800115D8(*(s32 *)(e + 0x10) * 0x28, (void **)(p + 0x14), (s32)&D_8004E0F0, 0);
    }
}

static inline u8 *Pop3(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

static inline u8 *Push3(u8 *arg1) {
    u8 *p;

    p = Pop3();
    if (p != NULL) {
        *(u8 **)(p + 0xB4) = *(u8 **)(arg1 + 8);
        *(u8 **)(arg1 + 8) = p;
    }
    return p;
}

void func_80021708(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;

    p = Push3(arg1);
    if (p != NULL) {
        *(s32 *)(p + 4) = 4;
        *(s32 *)p = arg2;
        *(s32 *)(p + 8) = *(s32 *)(arg0 + 0x14);
        *(s32 *)(p + 0xC) = *(s32 *)(arg0 + 0x18);
    }
}

void render_entity_thread(u8 *arg0, s32 *arg1) {
    func_80019F1C(arg0 + 8, *arg1, (*(s32*)((u8*)(arg0) + 8)));
}

void func_80021798(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;
    u8 *sub;
    s32 v;
    u16 w;

    p = Push3(arg1);
    if (p != NULL) {
        sub = p + 8;
        *(s32 *)(p + 4) = 3;
        *(subengine *)sub = D_8005A830;
        v = *(s32 *)arg1;
        *(s16 *)(sub + 0x74) = 0;
        *(s32 *)sub = 0;
        *(s32 *)(sub + 0x6C) = v;
        w = *(u16 *)(arg0 + 0x28);
        *(u16 *)(sub + 0x68) = w;
        if ((w << 16) != 0) {
            *(s32 *)sub |= 0x2000000;
        }
        *(s32 *)p = arg2;
        *(s32 *)(p + 0xB0) = -1;
    }
}

s32 *func_80021878(u8 *arg0, s32 arg1) {
    s32 *p;

    p = (s32 *)(arg0 + 0x30);
    if (p[1] != 0) {
        do {
            if (arg1 >= p[0] && arg1 < p[0] + p[1]) {
                return p;
            }
            p += 19;
        } while (p[1] != 0);
    }
    return p;
}

s32 *func_800218DC(u8 *arg0, s32 arg1) {
    s32 *p;

    p = (s32 *)(arg0 + 0x1C);
    if (p[1] != 0) {
        do {
            if (arg1 >= p[0] && arg1 < p[0] + p[1]) {
                return p;
            }
            p += 10;
        } while (p[1] != 0);
    }
    return p;
}

static inline u8 *Pop2(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

static inline u8 *Push2(u8 *arg0) {
    u8 *p;

    p = Pop2();
    if (p != NULL) {
        *(u8 **)(p + 0xB4) = *(u8 **)(arg0 + 8);
        *(u8 **)(arg0 + 8) = p;
    }
    return p;
}

void func_80021940(s32 arg0, u8 *arg1, s32 arg2) {
    u8 *p;

    p = Push2(arg1);
    if (p != NULL) {
        *(s32 *)(p + 4) = 2;
        *(s32 *)p = arg2;
    }
}

void func_80021990(u8 *arg0, s32 *arg1) {
    func_80019F1C(arg0 + 8, *arg1, (*(s32*)((u8*)(arg0) + 8)));
}

s32 *func_800219B8(u8 *arg0, s32 arg1) {
    s32 *p;

    p = (s32 *)(arg0 + 0x24);
    while (p[1] != 0) {
        if (arg1 >= p[0] && arg1 < p[0] + p[1]) {
            return p;
        }
        p += 20;
    }
    return p;
}

void func_80021A1C(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;
    u8 *sub;
    s32 v;
    u16 w;

    p = Push4(arg1);
    if (p != NULL) {
        sub = p + 8;
        *(s32 *)(p + 4) = 0;
        *(subengine *)sub = D_8005A830;
        v = *(s32 *)arg1;
        *(s16 *)(sub + 0x74) = 0;
        *(s32 *)sub = 0;
        *(s32 *)(sub + 0x6C) = v;
        w = *(u16 *)(arg0 + 0x1C);
        *(u16 *)(sub + 0x68) = w;
        if ((w << 16) != 0) {
            *(s32 *)sub |= 0x2000000;
        }
        *(s32 *)p = arg2;
    }
}

void func_80021AF0(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4) {
    arg2[0] = func_800152A8(arg0[0], arg1[0], arg3, arg4);
    arg2[1] = func_800152A8(arg0[1], arg1[1], arg3, arg4);
    arg2[2] = func_800152A8(arg0[2], arg1[2], arg3, arg4);
}

void func_80021B88(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4) {
    arg2[0] = interp(arg0[0], arg1[0], arg3, arg4);
    arg2[1] = interp(arg0[1], arg1[1], arg3, arg4);
    arg2[2] = interp(arg0[2], arg1[2], arg3, arg4);
}

void func_80021C20(u8 *arg0, u8 *arg1, u8 *arg2, s32 arg3, s32 arg4) {
    (*(s32*)((u8*)(arg2) + 0)) = interp((*(s32*)((u8*)(arg0) + 0)), (*(s32*)((u8*)(arg1) + 0)), arg3, arg4);
    (*(s32*)((u8*)(arg2) + 4)) = interp((*(s32*)((u8*)(arg0) + 4)), (*(s32*)((u8*)(arg1) + 4)), arg3, arg4);
    (*(s32*)((u8*)(arg2) + 8)) = interp((*(s32*)((u8*)(arg0) + 8)), (*(s32*)((u8*)(arg1) + 8)), arg3, arg4);
}

s32 *func_80021CB8(s32 *arg0, s32 arg1, s32 arg2) {
    while (arg0[1] != 0) {
        if (arg0[0] <= arg1 && arg1 < arg0[0] + arg0[1]) {
            return arg0;
        }
        arg0 = (s32 *)((u8 *)arg0 + arg2);
    }
    return arg0;
}

u8 *func_80021D1C(u8 *arg0, s32 arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 v;

    u8 *e;

    e = *(u8 **)(arg0 + 0);
    lo = *(s32 *)(arg0 + 4);
    v = *(s16 *)(arg0 + 0xE);
    D_80058B84 = arg1;
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (arg1 * 4 + 0x1C) + *(s32 *)(r + (arg1 << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    return res;
}

u8 *func_80021DD4(u8 *arg0, s32 arg1, s32 arg2) {
    u8 *p;
    u8 *r;
    s32 *q;

    D_80058B7C = arg0;
    p = *(u8 **)(arg0 + 0xC);
    D_80058B80 = arg1;
    D_80058B84 = arg2;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + arg1 * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        return NULL;
    }
    return r + (arg2 * 4 + 0x1C) + *(s32 *)(r + (arg2 << 2) + 0x1C);
}

u8 *func_80021E44(OffsetTable *tbl, s32 i) {
    return (u8 *)&tbl->offs[i] + tbl->offs[i];
}

u8 *get_script_header(u8 *arg0, s32 arg1) {
    u8 *p;
    s32 *e;

    p = *(u8 **)(arg0 + 0xC);
    if (p != NULL) {
        e = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + arg1 * 4);
        return (u8 *)e + *e;
    }
    return NULL;
}

static inline u8 *PopD_80058B70(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

u8 *func_80021E98(u8 *arg0) {
    u8 *p;

    p = PopD_80058B70();
    if (p != NULL) {
        *(u8 **)(p + 0xB4) = *(u8 **)(arg0 + 8);
        *(u8 **)(arg0 + 8) = p;
    }
    return p;
}

void func_80021ED8(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = D_80058B70;
    D_80058B70 = arg0;
    (*(s32*)((u8*)(arg0) + 180)) = temp_v1;
}

void *func_80021EEC(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

void func_80021F14(u8 *arg0) {
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *temp_v1_3;

    temp_v1 = (*(s32*)((u8*)(arg0) + 112));
    if (temp_v1 != NULL) {
        (*(s32*)((u8*)(temp_v1) + 116)) = (u8 *) (*(s32*)((u8*)(arg0) + 116));
    } else {
        D_80058B78 = (*(s32*)((u8*)(arg0) + 116));
    }
    temp_v1_2 = (*(s32*)((u8*)(arg0) + 116));
    if (temp_v1_2 != NULL) {
        (*(s32*)((u8*)(temp_v1_2) + 112)) = (u8 *) (*(s32*)((u8*)(arg0) + 112));
    }
    temp_v1_3 = free_scripts;
    free_scripts = arg0;
    (*(s32*)((u8*)(arg0) + 116)) = temp_v1_3;
}

u8 *func_80021F6C(void) {
    u8 *temp_v0;
    u8 *temp_v1;

    temp_v1 = free_scripts;
    if (temp_v1 != NULL) {
        free_scripts = (*(s32*)((u8*)(temp_v1) + 116));
        if (D_80058B78 != NULL) {
            (*(s32*)((u8*)(D_80058B78) + 112)) = temp_v1;
        }
        temp_v0 = D_80058B78;
        (*(s32*)((u8*)(temp_v1) + 112)) = 0;
        D_80058B78 = temp_v1;
        (*(s32*)((u8*)(temp_v1) + 116)) = temp_v0;
    }
    return temp_v1;
}

void func_80021FB0(void) {
    u8 *temp_s0;
    u8 *var_a0;

    var_a0 = D_80058B78;
    if (var_a0 != NULL) {
        do {
            temp_s0 = (*(s32*)((u8*)(var_a0) + 116));
            func_80020F5C(var_a0);
            var_a0 = temp_s0;
        } while (var_a0 != NULL);
    }
    if (D_80058B6C != 0) {
        mal_free(D_80058B6C, &D_8004E0F0);
    }
    if (D_80058B68 != 0) {
        mal_free(D_80058B68, &D_8004E0F0);
    }
}

void func_80022030(void) {
    u8 *temp_s0;
    u8 *var_a0;

    var_a0 = D_80058B78;
    if (var_a0 != NULL) {
        do {
            temp_s0 = (*(s32*)((u8*)(var_a0) + 116));
            func_80020F5C(var_a0);
            var_a0 = temp_s0;
        } while (var_a0 != NULL);
    }
}

void scripts_render(void) {
    struct {
        s32 a;
        s32 b;
        u16 c;
        u16 d;
    } s;
    u8 *var_s0;
    GfxState *gs;

    var_s0 = D_80058B78;
    gs = &D_80056998;
    for (;;) {
        /* block-scope declaration, as the implicit one was: keeps the loop unrotated */
        extern void func_800201BC(u8 *, GteSetup *);
        if (var_s0 == NULL) {
            break;
        }
        s.a = func_8001ACC0(var_s0 + 0x1C);
        s.b = s.a + 0x14;
        s.c = *(u16 *)&gs->unk28[0];
        s.d = *(u16 *)&gs->unk28[4];
        func_800201BC(var_s0, &s);
        var_s0 = *(u8 **)(var_s0 + 0x74);
    }
}

void scripts_update(void) {
    u8 *temp_s1;
    u8 *var_s0;

    var_s0 = D_80058B78;
    if (var_s0 != NULL) {
        do {
            temp_s1 = (*(s32*)((u8*)(var_s0) + 116));
            if (func_80020064(var_s0) == 0) {
                func_80021F14(var_s0);
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}
