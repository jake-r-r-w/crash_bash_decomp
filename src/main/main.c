#include "common.h"
#include "bash/main.h"

s32 func_80010000(u8 *arg0) {
    s32 q;
    s32 n;
    s32 i;
    u8 *p;
    u8 b;
    s32 t;
    s32 d;

    n = 0;
    for (i = 0; i < 33; i++) {
        p = &arg0[i];
        if (p[0xE4] != 0) {
            n++;
        }
        if (p[0x126] != 0) {
            n++;
        }
        b = p[0x105];
        if (b != 0) {
            n++;
        }
        if (p[0x147] != 0) {
            n++;
        }
        if (b >= 2) {
            n += 2;
        }
        if (p[0x168] != 0) {
            n += 8;
        }
    }
    q = n / 10;
    t = 0;
    i = q;
    t = i - (i / 10) * 10;
    i /= 10;
    d = i - (i / 10) * 10;
    i /= 10;
    D_8004E0D0 = (n - q * 10) | (t << 4) | (d << 8) | ((i % 10) << 12);
    return n;
}

void func_80010158(void) {
    D_8004E0E0 = 2;
    func_80011BF8(&D_8004E0F0, D_80078C90, 0x801FF000 - (s32)D_80078C90);
    init_file();
    func_80013CA4();
    func_800138A4();
    func_800134FC(D_80050008, D_80078C90);
    D_8004E0DC = (VTable8 *)D_80078C90;
    func_800270F0(D_8004E0B8);
}

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004C56C);

/* Shuts the system down and runs the Spyro 3 demo from the disc. */
void func_800101E0(void) {
    char unused[24]; /* never used, but the frame has room for it */

    machine_quit_sound();
    DrawSync(0);
    StopRCnt(RCntCNT0);
    StopRCnt(RCntCNT1);
    StopRCnt(RCntCNT2);
    StopRCnt(RCntCNT3);
    VSync(4);
    ResetGraph(3);
    PadStopCom();
    StopCallback();
    VSync(10);
    _96_init();
    LoadExec("cdrom:\\SPYRO3\\SPYRO3.EXE;1", 0, 0);
}

void shell_render_dcache(void) {
    if (D_8004E0B4 == 0) {
        D_8004E0B4 = 1;
        D_8006E9F4 = func_800320CC();
    }
    func_800320DC(0x1F8003FC);
    D_8004E0DC->unk8();
    func_800272AC(D_8004E0E0);
    func_800320DC(D_8006E9F4);
}

void func_800102F8(void) {
    D_8004E0E0 = 2;
    func_80011BF8(&D_8004E0F0, D_80078C90, 0x801FF000 - (s32)D_80078C90);
    init_file();
    func_80013CA4();
    func_800138A4();
}

void func_80010354(void) {
    D_8004E0DC->unk8();
    func_800272AC(D_8004E0E0);
}

void func_80010394(void) {
    func_8001231C();
    func_800138A4();
    func_800155FC();
    func_80022838();
    func_8002BEA8();
    if (!(D_8005B690 & 1)) {
        func_8002D274();
    }
    D_8004E0DC->unk4();
}

void func_80010410(void) {
    u8 *p = D_8004E0DC->unk10;

    if (p != NULL) {
        func_80011BF8(&D_8004E0F0, p, 0x801FF000 - (s32)p);
        func_80012E54();
    }
    func_800222FC();
    SetNasDefaults();
    D_8004E0DC->unk0();
}

static inline void func_800107C8_merge(MemBlock *a, Heap *arg1, s32 m1) {
    s32 m2;
    MemBlock *b;
    s32 new_var2;
    MemBlock *prev;
    MemBlock *new_var;
    MemBlock *end;
    s32 *d;
    s32 *s;
    s32 n;
    u32 size;

    mal_unlink((u8 *)a, (u8 *)arg1);
    size = a->size;
    b = (MemBlock *)((u8 *)a + (size * 4 + 0x18));
    new_var = a;
    n = b->size;
    prev = a->prev;
    d = (s32 *)((u8 *)a + 0x18);
    *a = *b;
    s = (s32 *)((u8 *)b + 0x18);
    new_var->prev = prev;
    end = (MemBlock *)((u8 *)b + (n * 4 + 0x18));
    new_var2 = size;
    n--;
    *a->owner = (void *)d;
    if (n != m1) {
        do {
            m2 = -1;
            *d++ = *s++;
            n--;
        } while (n != m2);
    }
    b = (MemBlock *)((u8 *)a + (a->size * 4 + 0x18));
    end->prev = b;
    b->size = new_var2;
    b->owner = (void **)m1;
    b->unk0 = 0;
    b->prev = a;
    if (a == arg1->lowest) {
        arg1->lowest = func_80011BC0(b);
    }
    mal_free(b + 1, arg1);
}

#ifndef NON_MATCHING
void mal_update(Heap *heap) {
    u32 size;
    s32 size2;
    s32 n;
    s32 *s;
    s32 *d;
    MemBlock *end;
    MemBlock *p;
    MemBlock *a;
    MemBlock *b;
    MemBlock *prev;
    MemBlock *q;
    MemBlock *qprev;
    s32 budget;

    p = Heap_NextToMove(heap);
    budget = (0x1FE - VSync(1)) * 0x70;
    do {
        s32 t;

        if (p == NULL) break;
        budget -= p->size;
        if (budget <= 0) break;
        t = VSync(1);
        D_8004E0EC += p->size;
        a = heap->lowest;
        mal_unlink((u8 *)a, (u8 *)heap);
        size = a->size;
        b = (MemBlock *)((u8 *)a + (size * 4 + 0x18));
        n = b->size;
        prev = a->prev;
        d = (s32 *)((u8 *)a + 0x18);
        *a = *b;
        s = (s32 *)((u8 *)b + 0x18);
        a->prev = prev;
        end = (MemBlock *)((u8 *)b + (n * 4 + 0x18));
        size2 = size;
        n--;
        *a->owner = (void *)d;
        while (n != -1) {
            *d++ = *s++;
            n--;
        }
        b = (MemBlock *)((u8 *)a + (a->size * 4 + 0x18));
        end->prev = b;
        b->size = size2;
        b->owner = (void **)-1;
        b->unk0 = 0;
        b->prev = a;
        if (a == heap->lowest) {
            heap->lowest = func_80011BC0(b);
        }
        mal_free(b + 1, heap);
        t = VSync(1) - t;
        p = Heap_NextToMove(heap);
        D_8004E0E8 += t;
    } while (1);
    q = heap->deferred;
    qprev = NULL;
    while (q != NULL) {
        if (--*(s16 *)q->unk2 <= 0) {
            if (qprev != NULL) {
                qprev->nextFree = q->nextFree;
            } else {
                heap->deferred = q->nextFree;
            }
            mal_free(q + 1, &D_8004E0F0);
            if (qprev != NULL) {
                q = qprev->nextFree;
            } else {
                q = NULL;
            }
        } else {
            qprev = q;
            q = q->nextFree;
        }
    }
}
#else
static inline MemBlock *Heap_NextLowest(Heap *heap, s32 m1) {
    MemBlock *blk;
    MemBlock *next;

    blk = heap->lowest;
    if (blk == NULL) {
        return NULL;
    }
    next = (MemBlock *)((u8 *)blk + (blk->size * 4 + sizeof(MemBlock)));
    if (next->owner == (void **)m1 || next->size == 0) {
        return NULL;
    }
    return next;
}

void mal_update(Heap *heap) {
    s32 m1;
    MemBlock *lo;
    MemBlock *nx;
    MemBlock *ret;
    MemBlock *p;
    MemBlock *q;
    MemBlock *qprev;
    s32 budget;

    lo = heap->lowest;
    if (lo == NULL || (nx = (MemBlock *)((u8 *)lo + (lo->size * 4 + 0x18)),
                       nx->owner == (void **)-1) || nx->size == 0) {
        ret = NULL;
    } else {
        ret = nx;
    }
    p = ret;
    if (p != NULL) {
        if ((p->unk0 & 2) && (p->unk0 & 1) == (D_8005B690 & 1)) {
            p = NULL;
        }
    }
    budget = (0x1FE - VSync(1)) * 0x70;
    m1 = -1;
    do {
        s32 t;

        if (p == NULL) break;
        budget -= p->size;
        if (budget <= 0) break;
        t = VSync(1);
        D_8004E0EC += p->size;
        func_800107C8_merge(heap->lowest, heap, m1);
        t = VSync(1) - t;
        ret = Heap_NextLowest(heap, m1);
        p = ret;
        if (p != NULL) {
            if ((p->unk0 & 2) && (p->unk0 & 1) == (D_8005B690 & 1)) {
                p = NULL;
            }
        }
        D_8004E0E8 += t;
    } while (1);
    q = heap->deferred;
    qprev = NULL;
    while (q != NULL) {
        if (--*(s16 *)q->unk2 <= 0) {
            if (qprev != NULL) {
                qprev->nextFree = q->nextFree;
            } else {
                heap->deferred = q->nextFree;
            }
            mal_free(q + 1, &D_8004E0F0);
            if (qprev != NULL) {
                q = qprev->nextFree;
            } else {
                q = NULL;
            }
        } else {
            qprev = q;
            q = q->nextFree;
        }
    }
}
#endif

void func_800107C8(MemBlock *arg0, Heap *arg1) {
    s32 m1 = -1;
    MemBlock *nx;
    MemBlock *p;
    MemBlock *ret;

    for (;;) {
        if (arg0 == NULL) {
            return;
        }
        func_800107C8_merge(arg1->lowest, arg1, m1);
        nx = arg1->lowest;
        if (nx == NULL || (nx = (MemBlock *)((u8 *)nx + (nx->size * 4 + 0x18)),
                           nx->owner == (void **)m1) || nx->size == 0) {
            ret = NULL;
        } else {
            ret = nx;
        }
        p = ret;
        if (p != NULL) {
            if ((p->unk0 & 2) && (p->unk0 & 1) == (D_8005B690 & 1)) {
                p = NULL;
            }
        }
        arg0 = p;
    }
}

void func_8001097C(Heap *heap) {
    MemBlock *cur;
    MemBlock *prev;
    MemBlock *next;
    MemBlock *blk;
    MemBlock *new;
    u32 size;
    u32 *dst;
    u32 *src;
    s32 n;

    while (1) {
        cur = *(MemBlock **)&heap->unk0[4];
        for (;;) {
            cur = cur->prev;
            if (cur == NULL) break;
            if (cur->owner != NULL) continue;
            if (cur->prev == NULL) continue;
            if (cur->prev->owner != (void **)-1) break;
        }
        blk = cur;
        if (blk == NULL) {
            return;
        }
        size = blk->size;
        /* redundant duplicate: keeps cur out of blk's register and the loop unrotated */
        if (prev != NULL) {
            prev = blk->prev;
        } else {
            prev = blk->prev;
        }
        next = (MemBlock *)((u8 *)blk + (size * 4 + 0x18));
        if (prev == NULL) {
            continue;
        }
        mal_unlink((u8 *)blk, (u8 *)heap);
        dst = (u32 *)((u8 *)blk + (blk->size * 4 + 0x14));
        n = prev->size;
        src = (u32 *)((u8 *)prev + (n * 4 + 0x14));
        new = (MemBlock *)((u8 *)dst - n * 4 - 0x14);
        next->prev = new;
        for (n--; n != -1; n--) {
            *dst-- = *src--;
        }
        *new = *prev;
        *new->owner = new + 1;
        new->prev = prev;
        prev->owner = (void **)-1;
        prev->size = size;
        mal_free(prev + 1, heap);
    }
}

static inline MemBlock *func_80010AE8_next(Heap *heap) {
    MemBlock *blk;
    MemBlock *next;

    blk = heap->lowest;
    if (blk == NULL) {
        return NULL;
    }
    next = (MemBlock *)((u8 *)blk + (blk->size * 4 + sizeof(MemBlock)));
    if (next->owner == (void **)-1 || next->size == 0) {
        return NULL;
    }
    return next;
}

static inline void func_80010AE8_merge(MemBlock *arg0, Heap *arg1) {
    s32 m1;
    MemBlock *next;
    s32 new_var2;
    MemBlock *prev;
    MemBlock *new_var;
    MemBlock *end;
    s32 *d;
    s32 *s;
    s32 n;
    s32 size;

    mal_unlink((u8 *)arg0, (u8 *)arg1);
    size = arg0->size;
    next = (MemBlock *)((u8 *)arg0 + (size * 4 + 0x18));
    new_var = arg0;
    n = next->size;
    prev = arg0->prev;
    d = (s32 *)((u8 *)arg0 + 0x18);
    *arg0 = *next;
    s = (s32 *)((u8 *)next + 0x18);
    new_var->prev = prev;
    end = (MemBlock *)((u8 *)next + (n * 4 + 0x18));
    new_var2 = size;
    n--;
    *arg0->owner = (void *)d;
    if (n != -1) {
        do {
            m1 = -1;
            *d++ = *s++;
            n--;
        } while (n != m1);
    }
    next = (MemBlock *)((u8 *)arg0 + (arg0->size * 4 + 0x18));
    end->prev = next;
    next->size = new_var2;
    next->owner = (void **)-1;
    next->unk0 = 0;
    next->prev = arg0;
    if (arg0 == arg1->lowest) {
        arg1->lowest = func_80011BC0(next);
    }
    mal_free(next + 1, arg1);
}

void func_80010AE8(Heap *heap) {
    DrawSync(0);
    while (func_80010AE8_next(heap) != NULL) {
        func_80010AE8_merge(heap->lowest, heap);
    }
}

static inline MemBlock *Heap_NextFreeAfter(MemBlock *p) {
    while (p->size != 0) {
        p = (MemBlock *)((u8 *)p + (p->size * 4 + 0x18));
        if (p->owner == NULL) {
            return p;
        }
    }
    return NULL;
}

static inline MemBlock *Heap_FindFree(u32 words, MemBlock *blk) {
    for (; blk != NULL; blk = blk->nextFree) {
        if (blk->size >= words) {
            return blk;
        }
    }
    return blk;
}

MemBlock *func_80010C70(bytes, heap)
    s32 bytes;
    Heap *heap;
{
    u32 words;
    MemBlock *blk;
    MemBlock *ret;
    MemBlock *cur;
    MemBlock *p;
    MemBlock *nb;
    MemBlock *t;
    u32 sz;

    do {
        words = (u32)(bytes + 3) >> 2;
    } while (0);
    blk = Heap_FindFree(words, heap->free);
    if (blk == NULL) {
        return NULL;
    }
    if (blk->size <= words + 0x28) {
        p = blk->prevFree;
        if (p != NULL) {
            p->nextFree = blk->nextFree;
        } else {
            heap->free = blk->nextFree;
        }
        p = blk->nextFree;
        if (p != NULL) {
            p->prevFree = blk->prevFree;
        }
        ret = blk;
    } else {
        p = blk->prevFree;
        if (p != NULL) {
            p->nextFree = blk->nextFree;
        } else {
            heap->free = blk->nextFree;
        }
        p = blk->nextFree;
        if (p != NULL) {
            p->prevFree = blk->prevFree;
        }
        sz = blk->size - 6;
        sz -= words;
        blk->size = sz;
        cur = heap->free;
        if (cur == NULL) {
            heap->free = blk;
            blk->nextFree = NULL;
            blk->prevFree = NULL;
        } else {
            goto start;
        app:
            cur->nextFree = blk;
            blk->prevFree = cur;
            blk->nextFree = NULL;
            goto done;
        start:
            for (;;) {
                if (cur->size >= blk->size) {
                    t = cur->prevFree;
                    if (t != NULL) {
                        t->nextFree = blk;
                    } else {
                        heap->free = blk;
                    }
                    blk->prevFree = t;
                    blk->nextFree = cur;
                    cur->prevFree = blk;
                    goto done;
                }
                t = cur->nextFree;
                if (t == NULL) {
                    goto app;
                }
                cur = t;
            }
        }
    done:
        nb = (MemBlock *)((u8 *)blk + (blk->size * 4 + 0x18));
        nb->size = words;
        nb->prev = blk;
        MEMBLOCK_NEXT(nb)->prev = nb;
        ret = nb;
    }
    if (ret == heap->lowest) {
        heap->lowest = Heap_NextFreeAfter(ret);
    }
    return ret;
}

static inline MemBlock *Heap_NextFreeAfter2(MemBlock *p) {
    while (p->size != 0) {
        p = (MemBlock *)((u8 *)p + (p->size * 4 + 0x18));
        if (p->owner == NULL) {
            return p;
        }
    }
    return NULL;
}
static inline MemBlock *Heap_FindFree2(u32 words, MemBlock *blk) {
    for (; blk != NULL; blk = blk->nextFree) {
        if (blk->size >= words) {
            return blk;
        }
    }
    return blk;
}

void *func_80010E8C(bytes, heap)
    s32 bytes;
    Heap *heap;
{
    u32 words;
    MemBlock *blk;
    MemBlock *ret;
    MemBlock *cur;
    MemBlock *p;
    MemBlock *nb;
    MemBlock *t;
    u32 sz;

    do {
        words = (u32)(bytes + 3) >> 2;
    } while (0);
    blk = Heap_FindFree2(words, heap->free);
    if (blk == NULL) {
        return NULL;
    }
    if (blk->size <= words + 0x28) {
        p = blk->prevFree;
        if (p != NULL) {
            p->nextFree = blk->nextFree;
        } else {
            heap->free = blk->nextFree;
        }
        p = blk->nextFree;
        if (p != NULL) {
            p->prevFree = blk->prevFree;
        }
        ret = blk;
    } else {
        p = blk->prevFree;
        if (p != NULL) {
            p->nextFree = blk->nextFree;
        } else {
            heap->free = blk->nextFree;
        }
        p = blk->nextFree;
        if (p != NULL) {
            p->prevFree = blk->prevFree;
        }
        sz = blk->size - 6;
        sz -= words;
        nb = (MemBlock *)((u8 *)blk + (words * 4 + 0x18));
        nb->prev = blk;
        nb->owner = NULL;
        nb->unk0 = 0;
        nb->size = sz;
        blk->size = words;
        MEMBLOCK_NEXT(nb)->prev = nb;
        cur = heap->free;
        if (cur == NULL) {
            heap->free = nb;
            nb->nextFree = NULL;
            nb->prevFree = NULL;
        } else {
            goto start;
        app:
            cur->nextFree = nb;
            nb->prevFree = cur;
            nb->nextFree = NULL;
            goto done;
        start:
            for (;;) {
                if (cur->size >= nb->size) {
                    t = cur->prevFree;
                    if (t != NULL) {
                        t->nextFree = nb;
                    } else {
                        heap->free = nb;
                    }
                    nb->prevFree = t;
                    nb->nextFree = cur;
                    cur->prevFree = nb;
                    goto done;
                }
                t = cur->nextFree;
                if (t == NULL) {
                    goto app;
                }
                cur = t;
            }
        }
    done:
        ret = blk;
    }
    if (ret == heap->lowest) {
        heap->lowest = Heap_NextFreeAfter2(ret);
    }
    return ret;
}

void mal_free(void *ptr, Heap *heap) {
    MemBlock *blk;
    MemBlock *next;
    MemBlock *prev;
    MemBlock *p;
    MemBlock *q;

    blk = (MemBlock *)ptr - 1;
    if (blk->owner != (void **)-1) {
        *blk->owner = NULL;
    }
    prev = blk->prev;
    next = (MemBlock *)((u32 *)(blk + 1) + blk->size);
    if (blk < heap->lowest || heap->lowest == NULL) {
        heap->lowest = blk;
    }
    if (next->owner == NULL) {
        if (next->prevFree != NULL) {
            next->prevFree->nextFree = next->nextFree;
        } else {
            heap->free = next->nextFree;
        }
        if (next->nextFree != NULL) {
            next->nextFree->prevFree = next->prevFree;
        }
        blk->size += 6 + next->size;
        blk->unk0 = 0;
        MEMBLOCK_NEXT(blk)->prev = blk;
    }
    if (prev != NULL && prev->owner == NULL) {
        prev->size += 6 + blk->size;
        prev->unk0 = 0;
        MEMBLOCK_NEXT(blk)->prev = prev;
        if (prev->prevFree != NULL) {
            prev->prevFree->nextFree = prev->nextFree;
        } else {
            heap->free = prev->nextFree;
        }
        if (prev->nextFree != NULL) {
            prev->nextFree->prevFree = prev->prevFree;
        }
        blk = prev;
        if (blk < heap->lowest || heap->lowest == NULL) {
            heap->lowest = blk;
        }
    } else {
        blk->owner = NULL;
    }
    p = heap->free;
    if (p == NULL) {
        heap->free = blk;
        blk->nextFree = NULL;
        blk->prevFree = NULL;
        return;
    }
    while (p != NULL) {
        if (p->size >= blk->size) {
            q = p->prevFree;
            if (q != NULL) {
                q->nextFree = blk;
            } else {
                heap->free = blk;
            }
            blk->prevFree = q;
            blk->nextFree = p;
            p->prevFree = blk;
            return;
        }
        if (p->nextFree == NULL) {
            p->nextFree = blk;
            blk->prevFree = p;
            blk->nextFree = NULL;
            return;
        }
        p = p->nextFree;
    }
}

void *func_800112A4(void *base, s32 offset, Heap *arg2) {
    MemBlock *blk;
    MemBlock *nb;
    MemBlock *prev;
    MemBlock *next;
    void **owner;
    u32 words;
    u32 sz;

    words = (u32)offset >> 2;
    words -= 6;
    blk = (MemBlock *)base;
    blk--;
    next = (MemBlock *)((u8 *)blk + (blk->size * 4 + 0x18));
    prev = blk->prev;
    nb = (MemBlock *)((u8 *)blk + (words * 4 + 0x18));
    nb->owner = blk->owner;
    nb->unk0 = ((MemBlock *)base)[-1].unk0;
    *(u16 *)nb->unk2 = *(u16 *)blk->unk2;
    owner = nb->owner;
    if (owner != (void **)-1) {
        *owner = nb + 1;
    }
    sz = blk->size - 6;
    sz -= words;
    nb->size = sz;
    if (words < 0x10) {
        if (prev->owner == NULL) {
            next->prev = nb;
            if (prev->prevFree != NULL) {
                prev->prevFree->nextFree = prev->nextFree;
            } else {
                arg2->free = prev->nextFree;
            }
            if (prev->nextFree != NULL) {
                prev->nextFree->prevFree = prev->prevFree;
            }
            prev->owner = (void **)-1;
            prev->size += (u32)offset >> 2;
            nb->prev = prev;
            mal_free(prev + 1, arg2);
        } else {
            printf("first call here tag onto end\n");
            nb->prev = prev;
            prev->size += offset;
        }
    } else {
        blk->owner = (void **)-1;
        blk->size = words;
        next->prev = nb;
        nb->prev = blk;
        mal_free(base, arg2);
    }
    return nb + 1;
}

/* Free-list statistics for a heap; returns the free bytes. */
s32 func_80011410(Heap *heap) {
    MemBlock *blk;
    s32 bytes;

    bytes = 0;
    blk = heap->free;
    D_8004E108 = 0;
    D_8004E104 = 0;
    for (; blk != NULL; blk = blk->nextFree) {
        bytes = blk->size * 4;
        D_8004E108++;
        D_8004E104 += bytes;
    }
    D_8004E10C = bytes;
    return D_8004E104;
}

void mal_print_free(void) {
    func_80011410(&D_8004E0F0);
}

u8 *func_80011498(u8 *arg0, s32 arg1, Heap *arg2) {
    MemBlock *blk;
    MemBlock *new_blk;
    u32 words;
    u32 old;
    s32 rest;
    Heap *heap;

    void **neg1;

    words = (u32)arg1 + 3;
    words = words >> 2;
    heap = arg2;
    blk = (MemBlock *)(arg0 - 0x18);
    old = blk->size;
    neg1 = (void **)-1;
    rest = old - words - 6;
    if (rest >= 0x10) {
        blk->size = words;
        new_blk = (MemBlock *)((u8 *)blk + (0x18 + words * 4));
        new_blk->owner = neg1;
        new_blk->size = rest;
        new_blk->unk0 = 0;
        new_blk->prev = blk;
        ((MemBlock *)((u8 *)blk + old * 4 + 0x18))->prev = new_blk;
        mal_free((u8 *)new_blk + 0x18, heap);
    }
    return arg0;
}

s32 func_8001151C(u8 *arg0) {
    return ~(*(s32*)((u8*)(arg0) + -20)) == 0;
}

void func_80011530(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + -20)) = -1;
    (*(s16*)((u8*)(arg0) + -24)) = 4;
}

void func_80011544(u8 *arg0, u8 **arg1) {
    u8 **temp_v1;
    u8 *temp_a2;

    temp_a2 = arg0 - 0x18;
    temp_v1 = (*(s32*)((u8*)(temp_a2) + 4));
    if (temp_v1 != (u8 **)-1) {
        *temp_v1 = NULL;
    }
    (*(s16*)((u8*)(arg0) + -24)) = 0;
    (*(s32*)((u8*)(temp_a2) + 4)) = arg1;
    *arg1 = arg0;
}

s16 func_8001156C(u8 *arg0) {
    return (*(s16*)((u8*)(arg0) + -24));
}

void func_80011578(u8 *arg0, Heap *heap, s32 arg2) {
    MemBlock *blk;

    blk = (MemBlock *)(arg0 - 0x18);
    if (blk->owner != (void **)-1) {
        *blk->owner = NULL;
    }
    func_80011530(arg0);
    blk->nextFree = heap->deferred;
    *(s16 *)&blk->unk2 = arg2;
    heap->deferred = blk;
}

void *func_800115D8(s32 arg0, void **arg1, s32 arg2, s32 arg3) {
    u8 *blk;
    u8 *ret;

    blk = func_80010E8C(arg0, arg2);
    if (blk == NULL) {
        blk = func_80010E8C(arg0, arg2);
        if (blk == NULL) {
            return NULL;
        }
    }
    ret = blk + 0x18;
    *(void **)(blk + 4) = arg1;
    *arg1 = ret;
    *(s16 *)blk = arg3;
    return ret;
}

u8 *func_80011654(s32 arg0, Heap *arg1) {
    u16 *var_a0_2;
    u8 *temp_v1;
    u8 *var_a0;
    u8 *var_v1;

    func_80010AE8(arg1);
    var_v1 = func_80010C70(arg0, arg1);
    var_a0 = var_v1 + 0x18;
    if (var_v1 == NULL) {
        temp_v1 = (*(s32*)((u8*)(arg1) + 12));
        var_a0_2 = NULL;
        if ((temp_v1 != NULL) && ((var_a0_2 = temp_v1 + (((*(s32*)((u8*)(temp_v1) + 8)) * 4) + 0x18), ((*(s32*)((u8*)(var_a0_2) + 4)) == -1)) || ((*(s32*)((u8*)(var_a0_2) + 8)) == 0))) {
            var_a0_2 = NULL;
        }
        if ((var_a0_2 != NULL) && (*var_a0_2 & 2)) {
            var_a0_2 = NULL;
        }
        func_800107C8(var_a0_2, arg1);
        var_v1 = func_80010C70(arg0, arg1);
        var_a0 = var_v1 + 0x18;
        if (var_v1 == NULL) {
            mal_print_free();
            var_a0 = NULL;
        } else {
            goto block_10;
        }
    } else {
block_10:
        (*(s32*)((u8*)(var_v1) + 4)) = -1;
        (*(u16*)((u8*)(var_v1) + 0)) = 0;
    }
    return var_a0;
}

u8 *func_80011748(s32 arg0, u8 *arg1) {
    u16 *var_a0_2;
    u8 *temp_v1;
    u8 *var_a0;
    u8 *var_v0;

    var_a0 = func_80010C70();
    var_v0 = var_a0 + 0x18;
    if (var_a0 == NULL) {
        temp_v1 = (*(s32*)((u8*)(arg1) + 12));
        var_a0_2 = NULL;
        if ((temp_v1 != NULL) && ((var_a0_2 = temp_v1 + (((*(s32*)((u8*)(temp_v1) + 8)) * 4) + 0x18), ((*(s32*)((u8*)(var_a0_2) + 4)) == -1)) || ((*(s32*)((u8*)(var_a0_2) + 8)) == 0))) {
            var_a0_2 = NULL;
        }
        if ((var_a0_2 != NULL) && (*var_a0_2 & 2)) {
            var_a0_2 = NULL;
        }
        func_800107C8(var_a0_2, arg1);
        var_a0 = func_80010C70(arg0, arg1);
        var_v0 = var_a0 + 0x18;
        if (var_a0 == NULL) {
            mal_print_free();
            return NULL;
        }
        goto block_10;
    }
block_10:
    (*(s32*)((u8*)(var_a0) + 4)) = -1;
    (*(u16*)((u8*)(var_a0) + 0)) = 0;
    return var_v0;
}

void *func_80011828(void *ptr, s32 bytes) {
    MemBlock *blk;
    MemBlock *split;
    MemBlock *next;
    s32 words;

    blk = (MemBlock *)ptr - 1;
    words = bytes >> 2;
    split = (MemBlock *)((u32 *)blk + words);
    next = (MemBlock *)((u32 *)(blk + 1) + blk->size);
    split->unk0 = 4;
    split->owner = (void **)-1;
    split->size = blk->size - words;
    split->prev = blk;
    blk->size = words - 6;
    next->prev = split;
    return split + 1;
}



MemBlock *func_80011878(MemBlock *blk, s32 words) {
    MemBlock *split;
    MemBlock *next;

    split = (MemBlock *)((u32 *)blk + words);
    next = (MemBlock *)((u32 *)(blk + 1) + blk->size);
    split->unk0 = 4;
    split->owner = (void **)-1;
    split->size = blk->size - words;
    split->prev = blk;
    blk->size = words - 6;
    next->prev = split;
    return split;
}

MemBlock *func_800118BC(MemBlock *blk, s32 words, Heap *heap) {
    MemBlock *split;
    MemBlock *cur;
    MemBlock *t;
    u32 sz;

    if (blk->prevFree != NULL) {
        blk->prevFree->nextFree = blk->nextFree;
    } else {
        heap->free = blk->nextFree;
    }
    if (blk->nextFree != NULL) {
        blk->nextFree->prevFree = blk->prevFree;
    }
    sz = blk->size - 6;
    sz -= words;
    split = (MemBlock *)((u8 *)blk + (words * 4 + 0x18));
    split->prev = blk;
    split->owner = NULL;
    split->unk0 = 0;
    split->size = sz;
    blk->size = words;
    MEMBLOCK_NEXT(split)->prev = split;
    cur = heap->free;
    if (cur == NULL) {
        heap->free = split;
        split->nextFree = NULL;
        split->prevFree = NULL;
    } else {
        goto start;
    app:
        cur->nextFree = split;
        split->prevFree = cur;
        split->nextFree = NULL;
        goto done;
    start:
        for (;;) {
            if (cur->size >= split->size) {
                t = cur->prevFree;
                if (t != NULL) {
                    t->nextFree = split;
                } else {
                    heap->free = split;
                }
                split->prevFree = t;
                split->nextFree = cur;
                cur->prevFree = split;
                goto done;
            }
            t = cur->nextFree;
            if (t == NULL) {
                goto app;
            }
            cur = t;
        }
    }
done:
    return blk;
}

void func_800119D4(MemBlock *arg0, s32 arg1, Heap *arg2) {
    MemBlock *cur;
    MemBlock *t;
    MemBlock *nb;
    MemBlock *p;
    u32 sz;

    t = arg0->prevFree;
    if (t != NULL) {
        t->nextFree = arg0->nextFree;
    } else {
        arg2->free = arg0->nextFree;
    }
    t = arg0->nextFree;
    if (t != NULL) {
        t->prevFree = arg0->prevFree;
    }
    sz = arg0->size - 6;
    sz -= arg1;
    arg0->size = sz;
    cur = arg2->free;
    if (cur == NULL) {
        arg2->free = arg0;
        arg0->nextFree = NULL;
        arg0->prevFree = NULL;
    } else {
        goto start;
    app:
        cur->nextFree = arg0;
        arg0->prevFree = cur;
        arg0->nextFree = NULL;
        goto done;
    start:
        for (;;) {
            if (cur->size >= arg0->size) {
                p = cur->prevFree;
                if (p != NULL) {
                    p->nextFree = arg0;
                } else {
                    arg2->free = arg0;
                }
                arg0->prevFree = p;
                arg0->nextFree = cur;
                cur->prevFree = arg0;
                goto done;
            }
            p = cur->nextFree;
            if (p == NULL) {
                goto app;
            }
            cur = p;
        }
    }
done:
    nb = (MemBlock *)((u8 *)arg0 + (arg0->size * 4 + 0x18));
    nb->size = arg1;
    nb->prev = arg0;
    MEMBLOCK_NEXT(nb)->prev = nb;
}

void func_80011AE8(MemBlock *blk, Heap *heap) {
    MemBlock *p;
    MemBlock *prev;

    p = heap->free;
    if (p == NULL) {
        heap->free = blk;
        blk->nextFree = NULL;
        blk->prevFree = NULL;
        return;
    }
    goto loop;
append:
    p->nextFree = blk;
    blk->prevFree = p;
    blk->nextFree = NULL;
    return;
loop:
    if (p->size >= blk->size) {
        prev = p->prevFree;
        if (prev != NULL) {
            prev->nextFree = blk;
        } else {
            heap->free = blk;
        }
        blk->prevFree = prev;
        blk->nextFree = p;
        p->prevFree = blk;
        return;
    }
    prev = p->nextFree;
    if (prev == NULL) {
        goto append;
    }
    p = prev;
    goto loop;
}

void mal_unlink(u8 *arg0, u8 *arg1) {
    u8 *temp_v1;
    u8 *temp_v1_2;

    temp_v1 = (*(s32*)((u8*)(arg0) + 16));
    if (temp_v1 != NULL) {
        (*(s32*)((u8*)(temp_v1) + 20)) = (u8 *) (*(s32*)((u8*)(arg0) + 20));
    } else {
        (*(s32*)((u8*)(arg1) + 8)) = (u8 *) (*(s32*)((u8*)(arg0) + 20));
    }
    temp_v1_2 = (*(s32*)((u8*)(arg0) + 20));
    if (temp_v1_2 != NULL) {
        (*(s32*)((u8*)(temp_v1_2) + 16)) = (u8 *) (*(s32*)((u8*)(arg0) + 16));
    }
}

MemBlock *func_80011BC0(MemBlock *blk) {
    if (blk->size != 0) {
        blk = (MemBlock *)((u32 *)(blk + 1) + blk->size);
        if (blk->owner != NULL) {
            return func_80011BC0(blk);
        }
        return blk;
    }
    return NULL;
}



MemBlock *func_80011BF8(Heap *heap, u8 *start, s32 size) {
    MemBlock *blk;
    MemBlock *end;

    blk = (MemBlock *)start;
    blk->size = ((u32)(size + 3) >> 2) - 12;
    blk->owner = NULL;
    blk->unk0 = 0;
    blk->prev = NULL;
    blk->nextFree = NULL;
    blk->prevFree = NULL;
    end = (MemBlock *)((u32 *)(blk + 1) + blk->size);
    end->size = 0;
    end->owner = (void **)-1;
    end->unk0 = 0;
    end->prev = blk;
    *(MemBlock **)&heap->unk0[0] = blk;
    heap->free = blk;
    heap->lowest = blk;
    *(MemBlock **)&heap->unk0[4] = end;
    heap->deferred = NULL;
    return blk;
}

MemBlock *mal_first_free(u32 words, MemBlock *blk) {
    for (; blk != NULL; blk = blk->nextFree) {
        if (blk->size >= words) {
            return blk;
        }
    }
    return blk;
}

MemBlock *func_80011C98(MemBlock *arg0) {
    MemBlock *blk;
    MemBlock *p;

    blk = (MemBlock *)arg0->owner;
    blk = blk->prev;
    for (; blk != NULL; blk = blk->prev) {
        if (blk->owner == NULL && (p = blk->prev) != NULL && p->owner != (void **)-1) {
            return blk;
        }
    }
    return blk;
}

void func_80011D00(s32 arg0) {
    func_800107C8(arg0 - 0x18, &D_8004E0F0);
}

void func_80011D28(u8 *arg0) {
    u16 *var_a0;
    u8 *temp_v1;

    temp_v1 = (*(s32*)((u8*)(arg0) + 12));
    var_a0 = NULL;
    if ((temp_v1 != NULL) && ((var_a0 = temp_v1 + (((*(s32*)((u8*)(temp_v1) + 8)) * 4) + 0x18), ((*(s32*)((u8*)(var_a0) + 4)) == -1)) || ((*(s32*)((u8*)(var_a0) + 8)) == 0))) {
        var_a0 = NULL;
    }
    if ((var_a0 != NULL) && (*var_a0 & 2)) {
        var_a0 = NULL;
    }
    func_800107C8(var_a0, arg0);
}

u8 *func_80011DB4(Heap *arg0) {
    u16 temp_v0;
    u8 *var_a0;
    u8 *var_s0;
    u8 *var_s1;

    var_s0 = NULL;
    var_a0 = (*(s32*)((u8*)(arg0) + 16));
    var_s1 = NULL;
    if (var_a0 != NULL) {
        do {
            temp_v0 = (*(u16*)((u8*)(var_a0) + 2)) - 1;
            (*(u16*)((u8*)(var_a0) + 2)) = temp_v0;
            if ((s16) temp_v0 <= 0) {
                if (var_s0 != NULL) {
                    (*(s32*)((u8*)(var_s0) + 20)) = (u8 *) (*(s32*)((u8*)(var_a0) + 20));
                } else {
                    (*(s32*)((u8*)(arg0) + 16)) = (u8 *) (*(s32*)((u8*)(var_a0) + 20));
                }
                mal_free(var_a0 + 0x18, &D_8004E0F0);
                var_a0 = NULL;
                if (var_s0 != NULL) {
                    var_a0 = (*(s32*)((u8*)(var_s0) + 20));
                }
            } else {
                var_s1 += (s16) temp_v0;
                var_s0 = var_a0;
                var_a0 = (*(s32*)((u8*)(var_a0) + 20));
            }
        } while (var_a0 != NULL);
    }
    return var_s1;
}

MemBlock *func_80011E80(Heap *heap) {
    MemBlock *next;

    next = Heap_AfterLowest(heap);
    if (next != NULL) {
        if (next->unk0 & 2) {
            return NULL;
        }
    }
    return next;
}

MemBlock *func_80011EF0(Heap *heap) {
    MemBlock *blk;
    MemBlock *next;
    MemBlock *ret;

    blk = heap->lowest;
    ret = NULL;
    if (blk != NULL) {
        next = (MemBlock *)((u8 *)blk + (blk->size * 4 + sizeof(MemBlock)));
        if (next->owner != (void **)-1) {
            ret = next;
            if (next->size == 0) {
                ret = NULL;
            }
        }
    }
    if (ret != NULL) {
        if ((ret->unk0 & 2) && (ret->unk0 & 1) == (D_8005B690 & 1)) {
            return NULL;
        }
    }
    return ret;
}

MemBlock *func_80011F74(Heap *heap) {
    MemBlock *blk;
    MemBlock *next;

    blk = heap->lowest;
    if (blk == NULL) {
        return NULL;
    }
    next = (MemBlock *)((u8 *)blk + (blk->size * 4 + sizeof(MemBlock)));
    if (next->owner == (void **)-1 || next->size == 0) {
        return NULL;
    }
    return next;
}

void func_80011FC8(MemBlock *arg0, Heap *arg1) {
    MemBlock *next;
    s32 new_var2;
    MemBlock *prev;
    MemBlock *new_var;
    MemBlock *end;
    MemBlock *nb;
    s32 *d;
    s32 *s;
    s32 n;
    s32 size;

    mal_unlink((u8 *)arg0, (u8 *)arg1);
    size = arg0->size;
    next = (MemBlock *)((u8 *)arg0 + (size * 4 + 0x18));
    new_var = arg0;
    n = next->size;
    prev = arg0->prev;
    d = (s32 *)((u8 *)arg0 + 0x18);
    *arg0 = *next;
    s = (s32 *)((u8 *)next + 0x18);
    new_var->prev = prev;
    end = (MemBlock *)((u8 *)next + (n * 4 + 0x18));
    new_var2 = size;
    n--;
    *arg0->owner = (void *)d;
    if (n != -1) {
        do {
            *d++ = *s++;
            n--;
        } while (n != -1);
    }
    next = (MemBlock *)((u8 *)arg0 + (arg0->size * 4 + 0x18));
    end->prev = next;
    next->size = new_var2;
    next->owner = (void **)-1;
    next->unk0 = 0;
    next->prev = arg0;
    if (arg0 == arg1->lowest) {
        arg1->lowest = func_80011BC0(next);
    }
    mal_free(next + 1, arg1);
}

void func_800120E8(MemBlock *arg0, Heap *arg1) {
    s32 size;
    MemBlock *prev;
    MemBlock *next;
    MemBlock *dst;
    s32 *d;
    s32 *s;
    s32 n;

    size = arg0->size;
    prev = arg0->prev;
    next = (MemBlock *)((u8 *)arg0 + (size * 4 + 0x18));
    if (prev != NULL) {
        mal_unlink((u8 *)arg0, (u8 *)arg1);
        d = (s32 *)((u8 *)arg0 + (arg0->size * 4 + 0x14));
        n = prev->size;
        s = (s32 *)((u8 *)prev + (n * 4 + 0x14));
        dst = (MemBlock *)((u8 *)d - n * 4 - 0x14);
        next->prev = dst;
        n--;
        while (n != -1) {
            *d-- = *s--;
            n--;
        }
        *dst = *prev;
        *dst->owner = (void *)(dst + 1);
        dst->prev = prev;
        prev->owner = (void **)-1;
        prev->size = size;
        mal_free(prev + 1, arg1);
    }
}

u8 *func_800121F8(u8 *arg0) {
    Heap *h;
    s32 words;

    if (*(u8 **)(arg0 + 0x18) == NULL) {
        if (*(s32 *)(arg0 + 0xC) == 0) {
            return func_80011654(((*(s32 *)(arg0 + 8) + 0x7FF) / 0x800) << 11, &D_8004E0F0);
        }
        words = ((*(s32 *)(arg0 + 8) + 0x7FF) / 0x800) << 9;
        h = &D_8004E0F0;
        if (mal_first_free(words, h->free) == NULL) {
            func_80017640();
            if (mal_first_free(words, h->free) == NULL) {
                func_80011D28((u8 *)h);
                if (mal_first_free(words, h->free) == NULL) {
                    return NULL;
                }
            }
        }
        return func_80011748(((*(s32 *)(arg0 + 8) + 0x7FF) / 0x800) << 11, (u8 *)&D_8004E0F0);
    }
    return *(u8 **)(arg0 + 0x18);
}

s32 func_8001231C(void) {
    LoaderState *ls;
    LoadNode *node;
    u8 *buf;
    u8 *data;
    u8 *head;
    s32 r;

    ls = &D_80050628;
    if (ls->state != 0) {
        if (--ls->state == 0) {
            if (func_80012FFC() != 0) {
                return 0;
            }
            if (ls->unk8 != 0) {
                ls->state = 3;
                ls->unk8 = 0;
            }
        }
        return 0;
    }
    if (ls->unk4 != 0) {
        r = func_80027944();
        if (r > 0) {
            return 0;
        }
        if (r < 0) {
            buf = (u8 *)ls->unk4;
            goto free;
        } else {
            node = (LoadNode *)ls->unkC;
            ls->unkC = (s32)node->next;
            node->next = D_80050F3C;
            D_80050F3C = node;
            if (ls->unkC != 0) {
                ((LoadNode *)ls->unkC)->prev = NULL;
            } else {
                ls->unk10 = 0;
            }
            buf = (u8 *)ls->unk4;
            ls->unk4 = 0;
            ls->state = 3;
            if (node->file == NULL) {
            free:
                mal_free(buf, &D_8004E0F0);
            } else if (node->fn != NULL) {
                node->fn(buf, node->arg);
            } else {
                *(u8 **)node->arg = buf;
                if (node->mode == 1) {
                    func_80011544(buf, (u8 **)node->arg);
                } else {
                    *(u8 **)node->arg = buf;
                }
            }
        }
    }
    if (ls->state != 0) {
        return 0;
    }
    if (func_8002BB80() != 0) {
        return 0;
    }
    head = (u8 *)ls->unkC;
    if (head == NULL) {
        return 0;
    }
    node = (LoadNode *)head;
    data = func_800121F8(head);
    ls->unk4 = (s32)data;
    if (data == NULL) {
        return 0;
    }
    if (func_80027790(node->file, node->offset / 2048, data, (node->size + 0x7FF) / 2048) < 0) {
        mal_free((void *)ls->unk4, &D_8004E0F0);
        ls->unk4 = 0;
    }
    return 0;
}

SortNode *func_8001251C(SortList *arg0) {
    SortNode *cur;
    SortNode *nxt;
    SortNode *t;

    cur = arg0->head;
loop:
    if (cur != NULL) {
        nxt = cur->next;
        if (nxt == NULL) {
            goto end;
        }
        if (cur->key->unk4 < nxt->key->unk4) {
            t = cur->prev;
            nxt->prev = t;
            if (t != NULL) {
                t->next = nxt;
            } else {
                arg0->head = nxt;
            }
            t = nxt->next;
            cur->next = t;
            if (t != NULL) {
                t->prev = cur;
            } else {
                arg0->tail = cur;
            }
            nxt->next = cur;
            cur->prev = nxt;
            return cur;
        }
        cur = nxt;
        goto loop;
    }
end:
    return NULL;
}

PosNode *func_800125B4(PosList *arg0) {
    PosNode *cur;
    PosNode *nxt;
    PosNode *t;

    cur = arg0->head;
loop:
    if (cur != NULL) {
        nxt = cur->next;
        if (nxt == NULL) {
            goto end;
        }
        if (*nxt->base + nxt->off / 2048 < *cur->base + cur->off / 2048) {
            t = cur->prev;
            nxt->prev = t;
            if (t != NULL) {
                t->next = nxt;
            } else {
                arg0->head = nxt;
            }
            t = nxt->next;
            cur->next = t;
            if (t != NULL) {
                t->prev = cur;
            } else {
                arg0->tail = cur;
            }
            nxt->next = cur;
            cur->prev = nxt;
            return cur;
        }
        cur = nxt;
        goto loop;
    }
end:
    return NULL;
}

static inline s32 func_80012678_find(s32 *arg0, s32 *arg1) {
    s32 *base;
    s32 *p;
    s32 n;
    s32 end;
    s32 first;
    s32 *tbl;
    s32 end2;
    s32 i;

    tbl = D_8004E110;
    n = arg1[1];
    n--;
    base = tbl;
    base = base + arg1[0] * 2;
    end = -1;
    first = *base;
    if (n != end) {
        end2 = end;
        p = base;
        i = 0;
        do {
            n -= 1;
            if (&p[i * 2] == arg0) {
                return p[i * 2] - first;
            }
            i++;
        } while (n != end2);
    }
    return -1;
}

static inline void func_80012678_loop(u8 *arg0, s32 *arg1) {
    s32 first = arg1[0];
    s32 n = arg1[1];
    s32 *s2 = &D_8004E110[first * 2];
    s32 *s0;

    if (n != 0) {
        n--;
        s0 = &D_8004E110[(first + n) * 2];
        for (;;) {
            if (n == 0) {
                return;
            }
            func_80011498(func_80011828(arg0, (s0[0] - s2[0]) << 11), s0[1], &D_8004E0F0);
            n--;
            s0 -= 2;
        }
    }
}

void func_80012678(u8 *arg0, u8 *a1) {
    s32 **c;
    s32 *e;
    u8 *h;
    s32 i;

    c = *(s32 ***)(a1 + 4);
    if (c == NULL) {
        return;
    }
    func_80012678_loop(arg0, *(s32 **)a1);
    if (c == NULL) {
        return;
    }
    for (;;) {
        if (*c == NULL) {
            return;
        }
        *(s32 *)((u8 *)*c + 0x34) = 0;
        e = *(s32 **)a1;
        h = (u8 *)c[0];
        i = func_80012678_find(c[2], e);
        i <<= 11;
        func_80016E5C(arg0 + i, h);
        i = func_80012678_find(c[1], e);
        i <<= 11;
        func_80016234(arg0 + i, h);
        c += 3;
    }
}

static inline s32 func_80012840_find(s32 *arg0, s32 *arg1) {
    s32 *base;
    s32 *p;
    s32 n;
    s32 end;
    s32 first;
    s32 *tbl;
    s32 end2;

    tbl = D_8004E110;
    n = arg1[1];
    n--;
    base = tbl;
    base = base + arg1[0] * 2;
    end = -1;
    first = *base;
    if (n != end) {
        end2 = end;
        p = base;
loop:
        n -= 1;
        if (p == arg0) {
            return *p - first;
        }
        p += 2;
        if (n == end2) {
            return -1;
        }
        goto loop;
    }
    return end;
}

static inline void func_80012840_loop(u8 *arg0, s32 *arg1) {
    s32 first = arg1[0];
    s32 n = arg1[1];
    s32 *s2 = &D_8004E110[first * 2];
    s32 *s0;

    if (n != 0) {
        n--;
        s0 = &D_8004E110[(first + n) * 2];
        for (;;) {
            if (n == 0) {
                return;
            }
            func_80011498(func_80011828(arg0, (s0[0] - s2[0]) << 11), s0[1], &D_8004E0F0);
            n--;
            s0 -= 2;
        }
    }
}

void func_80012840(u8 *arg0, u8 *a1) {
    s32 *e;
    s32 **c;
    u8 *h;
    s32 i;

    func_80012840_loop(arg0, *(s32 **)a1);
    c = (s32 **)(a1 + 4);
    e = *(s32 **)a1;
    h = *(u8 **)(a1 + 4);
    i = func_80012840_find(c[2], e);
    i <<= 11;
    func_80016E5C(arg0 + i, h);
    i = func_80012840_find(c[1], e);
    i <<= 11;
    func_80016234(arg0 + i, h);
}

u8 *func_800129D0(u8 *arg0, s32 arg1) {
    LoadNode *p;
    LoadNode *next;
    LoadNode *tail;
    u8 *q;
    s32 a;
    s32 b;
    void (*fn)(u8 *, u8 *);
    LoaderState *ls;

    a = (s32)D_8004E110 + **(s32 **)arg0 * 8;
    *(s32 *)(*(u8 **)(arg0 + 4) + 0x34) = arg1;
    fn = func_80012840;
    ls = &D_80050628;
    b = *(s32 *)(*(u8 **)arg0 + 8);
    p = D_80050F3C;
    if (p != NULL) {
        next = p->next;
        p->next = NULL;
        p->prev = (LoadNode *)ls->unk10;
        D_80050F3C = next;
        tail = (LoadNode *)ls->unk10;
        if (tail != NULL) {
            tail->next = p;
        } else {
            ls->unkC = (s32)p;
        }
        ls->unk10 = (s32)p;
    }
    q = (u8 *)p;
    if (q == NULL) {
        printf(D_8004C83C);
    }
    *(s32 *)(q + 0) = a;
    *(s32 *)(q + 4) = 0;
    *(s32 *)(q + 0xC) = 1;
    *(s32 *)(q + 8) = b;
    *(void **)(q + 0x10) = fn;
    *(u8 **)(q + 0x14) = arg0;
    *(s32 *)(q + 0x18) = 0;
    return q;
}

u8 *func_80012AC4(u8 *arg0) {
    LoadNode *p;
    LoadNode *next;
    LoadNode *tail;
    u8 *q;
    s32 a;
    s32 b;
    void (*fn)(u8 *, u8 *);
    LoaderState *ls;

    if (*(u8 **)arg0 == NULL) {
        return NULL;
    }
    if (*(s32 *)(arg0 + 4) == 0) {
        return NULL;
    }
    a = (s32)D_8004E110 + **(s32 **)arg0 * 8;
    fn = func_80012678;
    ls = &D_80050628;
    b = *(s32 *)(*(u8 **)arg0 + 8);
    p = D_80050F3C;
    if (p != NULL) {
        next = p->next;
        p->next = NULL;
        p->prev = (LoadNode *)ls->unk10;
        D_80050F3C = next;
        tail = (LoadNode *)ls->unk10;
        if (tail != NULL) {
            tail->next = p;
        } else {
            ls->unkC = (s32)p;
        }
        ls->unk10 = (s32)p;
    }
    q = (u8 *)p;
    if (q == NULL) {
        printf(D_8004C83C);
    }
    *(s32 *)(q + 0) = a;
    *(s32 *)(q + 4) = 0;
    *(s32 *)(q + 0xC) = 1;
    *(s32 *)(q + 8) = b;
    *(void **)(q + 0x10) = fn;
    *(u8 **)(q + 0x14) = arg0;
    *(s32 *)(q + 0x18) = 0;
    return q;
}

static inline s32 func_80012BC8_find(s32 *arg0, s32 *arg1) {
    s32 *base;
    s32 *p;
    s32 n;
    s32 end;
    s32 first;
    s32 *tbl;
    s32 end2;

    tbl = D_8004E110;
    n = arg1[1];
    n--;
    base = tbl;
    base = base + arg1[0] * 2;
    end = -1;
    first = *base;
    if (n != end) {
        end2 = end;
        p = base;
loop:
        n -= 1;
        if (p == arg0) {
            return *p - first;
        }
        p += 2;
        if (n == end2) {
            return -1;
        }
        goto loop;
    }
    return end;
}

void func_80012BC8(s32 **arg0, s32 *arg1, u8 *arg2) {
    u8 *s2 = (u8 *)arg0[0];
    s32 i;

    i = func_80012BC8_find(arg0[2], arg1);
    i <<= 11;
    func_80016E5C(arg2 + i, s2);
    i = func_80012BC8_find(arg0[1], arg1);
    i <<= 11;
    func_80016234(arg2 + i, s2);
}

void func_80012CD4(void *arg0, s32 *arg1) {
    s32 first = arg1[0];
    s32 n = arg1[1];
    s32 *s2 = &D_8004E110[first * 2];
    s32 *s0;

    if (n != 0) {
        n--;
        s0 = &D_8004E110[(first + n) * 2];
        for (;;) {
            if (n == 0) {
                return;
            }
            func_80011498(func_80011828(arg0, (s0[0] - s2[0]) << 11), s0[1], &D_8004E0F0);
            n--;
            s0 -= 2;
        }
    }
}

s32 func_80012D80(s32 *arg0, s32 *arg1) {
    s32 *base;
    s32 *p;
    s32 n;
    s32 end;
    s32 first;
    s32 *tbl;
    s32 end2;

    tbl = D_8004E110;
    n = arg1[1];
    n--;
    base = tbl;
    base = base + arg1[0] * 2;
    end = -1;
    first = *base;
    if (n != end) {
        end2 = end;
        p = base;
loop:
        n -= 1;
        if (p == arg0) {
            return *p - first;
        }
        p += 2;
        if (n == end2) {
            return -1;
        }
        goto loop;
    }
    return end;
}

void func_80012DDC(s32 arg0) {
    do {

    } while (func_8001251C(arg0) != 0);
}

void func_80012E0C(void) {
}

void func_80012E14(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = D_80051240;
    D_80051240 = arg0;
    (*(s32*)((u8*)(arg0) + 8)) = temp_v1;
}

void *func_80012E28(void) {
    u8 *p;

    p = D_80051240;
    if (p != NULL) {
        *(s32 *)(p + 0) = 0;
        *(s32 *)(p + 4) = 0;
        D_80051240 = *(u8 **)(p + 8);
    }
    return p;
}


void func_80012E54(void) {
    u8 *p;
    s32 i;

    p = D_80050F40;
    D_80051240 = p;
    for (i = 62; i != -1; i--) {
        *(u8 **)(p + 8) = p + 0xC;
        p += 0xC;
    }
    *(u8 **)(p + 8) = NULL;
}

void func_80012E88(void) {
}

void init_file(void) {
    LoadNode *node;
    s32 i;

    func_800279A4();
    node = (LoadNode *)D_8005063C;
    i = 0x3E;
    D_80050628.state = 3;
    D_80050628.unk4 = 0;
    D_80050628.unk8 = 0;
    D_80050628.unkC = 0;
    D_80050628.unk10 = 0;
    D_80050F3C = D_8005063C;
    do {
        LoadNode *next = (LoadNode *)((u8 *)node + 0x24);
        node->next = next;
        i--;
        node = next;
    } while (i != -1);
    node->next = 0;
    func_80012E54();
}

void func_80012F04(LoadNode *arg0) {
    LoaderState *ls;
    LoadNode *prev;
    void *temp_v1;

    if ((s32)arg0 == D_80050628.unkC && D_80050628.unk4 != 0) {
        *(s32 *)arg0 = 0;
        return;
    }
    ls = &D_80050628;
    prev = arg0->prev;
    if (prev != NULL) {
        prev->next = arg0->next;
    } else {
        ls->unkC = (s32)arg0->next;
    }
    arg0->next->prev = prev->next;
    if ((s32)arg0 == ls->unk10) {
        ls->unk10 = (s32)prev;
    }
    temp_v1 = D_80050F3C;
    D_80050F3C = arg0;
    arg0->next = temp_v1;
}

void func_80012F90(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = D_80050F3C;
    D_80050F3C = arg0;
    (*(s32*)((u8*)(arg0) + 28)) = temp_v1;
}

void func_80012FA4(LoadNode *arg0) {
    LoaderState *ls = &D_80050628;
    LoadNode *prev = arg0->prev;

    if (prev != NULL) {
        prev->next = arg0->next;
    } else {
        ls->unkC = (s32)arg0->next;
    }
    arg0->next->prev = prev->next;
    if ((s32)arg0 == ls->unk10) {
        ls->unk10 = (s32)prev;
    }
}

s32 func_80012FFC(void) {
    LoaderState *p = &D_80050628;
    s32 a = p->unkC != 0;
    s32 b = p->unk4;
    s32 s = p->state;
    int new_var;
    if (b == 0) {
        return a + s;
    }
    new_var = s + 1;
    return a + new_var;
}

s32 func_80013034(s32 arg0, s32 arg1, s32 arg2, void (*done)(u8 *, u8 *), u8 *arg4, s32 arg5, s32 arg6) {
    LoadNode *p;
    LoadNode *next;
    LoadNode *tail;
    u8 *q;
    LoaderState *ls = &D_80050628;

    p = D_80050F3C;
    if (p != NULL) {
        next = p->next;
        p->next = NULL;
        p->prev = (LoadNode *)ls->unk10;
        D_80050F3C = next;
        tail = (LoadNode *)ls->unk10;
        if (tail != NULL) {
            tail->next = p;
        } else {
            ls->unkC = (s32)p;
        }
        ls->unk10 = (s32)p;
    }
    q = (u8 *)p;
    if (q == NULL) {
        printf(D_8004C83C);
    }
    *(s32 *)(q + 0) = arg0;
    *(s32 *)(q + 4) = arg1;
    *(s32 *)(q + 0xC) = arg5;
    *(s32 *)(q + 8) = arg2;
    *(void **)(q + 0x10) = done;
    *(u8 **)(q + 0x14) = arg4;
    *(s32 *)(q + 0x18) = arg6;
    return (s32)q;
}

u8 *func_80013110(u8 *arg0) {
    u8 *temp_a1;
    u8 *temp_v0;
    u8 *temp_v1;

    temp_a1 = D_80050F3C;
    if (temp_a1 != NULL) {
        temp_v1 = (*(s32*)((u8*)(temp_a1) + 28));
        (*(s32*)((u8*)(temp_a1) + 28)) = NULL;
        (*(s32*)((u8*)(temp_a1) + 32)) = (u8 *) (*(s32*)((u8*)(arg0) + 16));
        temp_v0 = (*(s32*)((u8*)(arg0) + 16));
        D_80050F3C = temp_v1;
        if (temp_v0 != NULL) {
            (*(s32*)((u8*)(temp_v0) + 28)) = temp_a1;
        } else {
            (*(s32*)((u8*)(arg0) + 12)) = temp_a1;
        }
        (*(s32*)((u8*)(arg0) + 16)) = temp_a1;
    }
    return temp_a1;
}

s32 func_80013160(u8 *arg0) {
    return (*(s32*)((u8*)(arg0) + 12));
}

u8 *func_8001316C(u8 *arg0) {
    u8 *node;
    u8 *next;
    u8 *head;

    head = D_80050F3C;
    node = *(u8 **)(arg0 + 12);
    *(u8 **)(arg0 + 12) = *(u8 **)(node + 28);
    *(u8 **)(node + 28) = head;
    next = *(u8 **)(arg0 + 12);
    D_80050F3C = node;
    if (next != NULL) {
        *(u8 **)(next + 32) = NULL;
    } else {
        *(u8 **)(arg0 + 16) = NULL;
    }
    return node;
}




s32 func_800131B0(s32 arg0) {
    Heap *h = &D_8004E0F0;
    s32 words;
    arg0 += 3;
    words = arg0 >> 2;
    if (mal_first_free(words, h->free) == NULL) {
        func_80017640();
        if (mal_first_free(words, h->free) == NULL) {
            func_80011D28((u8 *)h);
            if (mal_first_free(words, h->free) == NULL) {
                return 0;
            }
        }
    }
    return 1;
}

void func_80013238(void) {
    D_80050628.state = 3;
    D_80050628.unk4 = 0;
    D_80050628.unk8 = 0;
    D_80050628.unkC = 0;
    D_80050628.unk10 = 0;
}

void func_8001325C(void) {
    u8 *p;
    s32 i;

    p = D_8005063C;
    D_80050F3C = p;
    for (i = 62; i != -1; i--) {
        *(u8 **)(p + 0x1C) = p + 0x24;
        p += 0x24;
    }
    *(u8 **)(p + 0x1C) = NULL;
}

s32 func_80013290(s32 *arg0, u8 **arg1) {
    s32 size;

    size = arg0[1];
    D_8006E9FC = NULL;
    func_80013034((s32)arg0, 0, size, NULL, (u8 *)&D_8006E9FC, 1, 0);
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    func_80011544(D_8006E9FC, arg1);
    return size;
}

s32 func_80013334(s32 arg0, s32 arg1, u8 **arg2, s32 arg3) {
    D_8006E9FC = NULL;
    func_80013034(arg0, arg1, arg3, NULL, (u8 *)&D_8006E9FC, 1, 0);
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    func_80011544(D_8006E9FC, arg2);
    return 0;
}

s32 asload_chunk_relocatable(s32 arg0, s32 arg1, u32 **arg2, s32 arg3) {
    return func_80013034(arg0, arg1, arg3, NULL, arg2, 1, 0);
}

s32 load_file_permanent(s32 *arg0) {
    s32 a2 = arg0[1];

    if (a2 == -1) {
        return 0;
    }
    D_8006E9F8 = 0;
    func_80013034((s32)arg0, 0, a2, NULL, (u8 *)&D_8006E9F8, 0, 0);
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    return D_8006E9F8;
}

s32 func_80013488(s32 arg0, s32 arg1, s32 arg2) {
    D_8006E9F8 = 0;
    func_80013034(arg0, arg1, arg2, NULL, (u8 *)&D_8006E9F8, 0, 0);
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    return D_8006E9F8;
}

s32 func_800134FC(u8 *a, u8 *b) {
    s32 r;
    s32 n;
    printf("load file start\n");
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    D_800637B0 = 0;
    while (func_80027944() > 0) {
    }
    do {
        for (;;) {
            n = *(s32 *)(a + 4);
            if (func_80027790(a, 0, b, (n + 0x7FF) / 2048) >= 0) {
                break;
            }
        }
        do {
            r = func_80027944();
        } while (r > 0);
    } while (r < 0);
    D_800637B0 = 1;
    printf("done loading\n");
    return 0;
}

s32 func_800135E4(s32 *arg0, u8 *arg1) {
    return func_80013034((s32)arg0, 0, arg0[1], NULL, arg1, 1, 0);
}

s32 func_8001361C(s32 *arg0, u8 *arg1) {
    return func_80013034((s32)arg0, 0, arg0[1], NULL, arg1, 0, 0);
}

s32 func_80013650(s32 arg0, void (*done)(), void *ctx) {
    return func_80013034(arg0, 0, ((s32 *)arg0)[1], done, ctx, 1, 0);
}

s32 asload_permanent_callback(s32 arg0, void (*done)(), void *ctx) {
    return func_80013034(arg0, 0, ((s32 *)arg0)[1], done, ctx, 0, 0);
}

#define PS ((PadStick *)pad)

void func_800136BC(PadState *pad) {
    s32 btn;
    s32 dir;
    s32 x;
    s32 y;

    btn = PS->buttons;
    if (PS->forced != 0) {
        dir = PS->forcedButtons;
        PS->forced = 0;
        if (dir & 0xA0) {
            if (dir & 0x80) {
                PS->lx = 0;
            } else {
                PS->lx = 0xFF;
            }
        } else {
            PS->lx = 0x80;
        }
        if (dir & 0x50) {
            if (dir & 0x10) {
                PS->ly = 0;
            } else {
                PS->ly = 0xFF;
            }
        } else {
            PS->ly = 0x80;
        }
        PS->rx = 0x80;
        PS->ry = 0x80;
    } else {
        if ((PS->flags & 2) && !(btn & 0xF0)) {
            if (PS->lx < 0x25) {
                btn |= 0x80;
            }
            if (PS->lx >= 0xDB) {
                btn |= 0x20;
            }
            if (PS->ly < 0x25) {
                btn |= 0x10;
            }
            if (PS->ly >= 0xDB) {
                btn |= 0x40;
            }
        } else {
            if (btn & 0xA0) {
                if (btn & 0x80) {
                    PS->lx = 0;
                } else {
                    PS->lx = 0xFF;
                }
            } else {
                PS->lx = 0x80;
            }
            if (btn & 0x50) {
                if (btn & 0x10) {
                    PS->ly = 0;
                } else {
                    PS->ly = 0xFF;
                }
            } else {
                PS->ly = 0x80;
            }
            PS->rx = 0x80;
            PS->ry = 0x80;
        }
        dir = btn;
    }
    x = PS->lx - 0x80;
    y = 0x80 - PS->ly;
    PS->dist = csqrt(x * x + y * y);
    PS->angle = get_angle(x, y);
    {
        s32 oldBtn = PS->held;
        s32 oldDir = PS->dirHeld;
        s32 t;
        s32 u;
        PS->held = btn;
        PS->held2 = btn;
        PS->dirHeld2 = dir;
        PS->dirHeld = dir;
        u = btn ^ oldBtn;
        t = u & btn;
        PS->pressed = t;
        u = dir ^ oldDir;
        t = dir & u;
        PS->dirPressed = t;
    }
}

#undef PS

void func_800138A4(void) {
    PadState *pad;
    s32 any;
    s32 i;

    func_80027D90();
    pad = D_80051338;
    any = 0;
    D_80051600 = 0;
    D_800515FC = -1;
    for (i = 0; i < 8; pad++, i++) {
        func_800136BC(pad);
        if (pad->unk0 & 1) {
            if (D_800515FC < 0) {
                D_800515FC = i;
            }
            if (D_80051600 == i) {
                D_80051600 = i + 1;
            }
            any |= pad->unk4;
        } else {
            pad->unk4 = 0;
        }
    }
    if (any != 0) {
        D_80051604 = 0;
    } else {
        D_80051604++;
    }
}


void func_800139A8(void) {
    ComboSeq *c;
    s32 i;
    s32 m;

    c = (ComboSeq *)D_800512A8;
    for (i = 0;; i++) {
        if (i > 0) {
            goto done;
        }
        m = c->steps[c->step];
        if (m != 0 && (((PadStick *)D_80051338)->pressed & 0xFFFF)) {
            if ((((PadStick *)D_80051338)->pressed & m) == m) {
                if (c->step == 0) {
                    c->startTime = D_8005B690;
                }
                c->step++;
                if (c->steps[c->step] == 0) {
                    c->step = 0;
                    if (c->maxTime >= D_8005B690 - c->startTime) {
                        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                        c->fn();
                    }
                }
            } else {
                m = c->steps[0];
                if ((((PadStick *)D_80051338)->pressed & m) == m) {
                    c->step = 1;
                    c->startTime = D_8005B690;
                } else {
                    c->step = 0;
                }
            }
        }
        c = (ComboSeq *)((u8 *)c + 0x90);
    }
done:;
}

void func_80013B0C(ComboSeq *c) {
    s32 m;

    m = c->steps[c->step];
    if (m != 0 && (D_80051380 & 0xFFFF)) {
        if ((D_80051380 & m) == m) {
            if (c->step == 0) {
                c->startTime = D_8005B690;
            }
            c->step++;
            if (c->steps[c->step] == 0) {
                c->step = 0;
                if (c->maxTime >= D_8005B690 - c->startTime) {
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                    c->fn();
                }
            }
        } else {
            m = c->steps[0];
            if ((D_80051380 & m) == m) {
                c->step = 1;
                c->startTime = D_8005B690;
                return;
            }
            c->step = 0;
        }
    }
}

void func_80013C24(void) {
    if (D_800512A4 != 0) {
        D_800512A4 = 0;
        screen_aspect[0] = 4;
        screen_aspect[1] = 3;
        return;
    }
    D_800512A4 = 1;
    screen_aspect[0] = 0x10;
    screen_aspect[1] = 9;
}

void func_80013C78(void) {
    u32 *p = (u32 *)D_8005A614;

    p[1] |= 0x80010000;
}

void set_joypad(u8 *arg0, u32 arg1) {
    (*(s32*)((u8*)(arg0) + 48)) = arg1;
    (*(s32*)((u8*)(arg0) + 44)) = 1;
}

void func_80013CA4(void) {
    PadState *pad;
    s32 i;

    pad = D_80051338;
    i = 7;
    do {
        pad->unk0 = 0;
        pad->unk4 = 0;
        *(s32 *)&pad->unk2A[0x30 - 0x2A + 0x14] = 0;
        *(s32 *)&pad->unk2A[0x30 - 0x2A + 0x18] = 0;
        *(s32 *)&pad->unk2A[0x30 - 0x2A + 0x8] = 0;
        *(s32 *)&pad->unk2A[0x30 - 0x2A + 0xC] = 0;
        *(s32 *)&pad->unk2A[0x30 - 0x2A] = 0;
        pad++;
    } while (--i != -1);
    D_800515FC = -1;
    D_80051604 = 0;
}

void func_80013CFC(s32 pad, u8 *out) {
    if (pad & 0xA0) {
        if (pad & 0x80) {
            *(s32 *)(out + 0x10) = 0;
        } else {
            *(s32 *)(out + 0x10) = 0xFF;
        }
    } else {
        *(s32 *)(out + 0x10) = 0x80;
    }
    if (pad & 0x50) {
        if (pad & 0x10) {
            *(s32 *)(out + 0x14) = 0;
        } else {
            *(s32 *)(out + 0x14) = 0xFF;
        }
    } else {
        *(s32 *)(out + 0x14) = 0x80;
    }
    *(s32 *)(out + 8) = 0x80;
    *(s32 *)(out + 0xC) = 0x80;
}

/* 4.12 product rounded away from zero */
#define FIX12_MUL(a, b) (((a) * (b) >= 0 ? (a) * (b) + 0x800 : (a) * (b) - 0x800) >> 12)

void func_80013D54(SVECTOR *rot, MATRIX *m) {
    Quat *q = (Quat *)rot;
    s32 x2, y2, z2;
    s32 wx, wy, wz, xx, xy, xz, yy, yz, zz;
    s32 t;

    x2 = q->v[0] * 2;
    y2 = q->v[1] * 2;
    z2 = q->v[2] * 2;
    t = FIX12_MUL(q->v[3], x2);
    wx = t;
    t = FIX12_MUL(q->v[3], y2);
    wy = t;
    t = FIX12_MUL(q->v[3], z2);
    wz = t;
    t = FIX12_MUL(q->v[0], x2);
    xx = t;
    t = FIX12_MUL(q->v[0], y2);
    xy = t;
    t = FIX12_MUL(q->v[0], z2);
    xz = t;
    t = FIX12_MUL(q->v[1], y2);
    yy = t;
    t = FIX12_MUL(q->v[1], z2);
    yz = t;
    t = FIX12_MUL(q->v[2], z2);
    zz = t;
    m->m[0][0] = 0x1000 - (yy + zz);
    m->m[0][1] = xy - wz;
    m->m[0][2] = xz + wy;
    m->m[1][0] = xy + wz;
    m->m[1][1] = 0x1000 - (xx + zz);
    m->m[1][2] = yz - wx;
    m->m[2][0] = xz - wy;
    m->m[2][1] = yz + wx;
    m->m[2][2] = 0x1000 - (xx + yy);
    MatrixNormal_1(m, m);
}

void func_80013F44(CamView *cam, CamView *dst, CamEase *ease) {
    if (ease != NULL) {
        if (cam_ramp_overide_count != 0) {
            ease = (CamEase *)D_800517B4;
            cam_ramp_overide_count--;
        }
        cam->dist += (dst->dist - cam->dist + 4) >> 2;
        cam->at.x += (dst->at.x - cam->at.x + (ease->at.x >> 1)) / ease->at.x;
        cam->at.y += (dst->at.y - cam->at.y + (ease->at.y >> 1)) / ease->at.y;
        cam->at.z += (dst->at.z - cam->at.z + (ease->at.z >> 1)) / ease->at.z;
        cam->eye.x += (dst->eye.x - cam->eye.x) / ease->eye.x;
        cam->eye.y += (dst->eye.y - cam->eye.y) / ease->eye.y;
        cam->eye.z += (dst->eye.z - cam->eye.z) / ease->eye.z;
        cam->rot.vx += func_800154A0(cam->rot.vx, dst->rot.vx) / ease->rot.x;
        cam->rot.vy += func_800154A0(cam->rot.vy, dst->rot.vy) / ease->rot.y;
        cam->rot.vz += func_800154A0(cam->rot.vz, dst->rot.vz) / ease->rot.z;
    } else {
        cam_ramp_overide_count = 0;
        cam->dist = dst->dist;
        cam->at = dst->at;
        cam->rot = dst->rot;
        cam->eye = dst->eye;
    }
}

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004C83C);

void func_800142D4(void *arg0) {
    RotScale *p = arg0;

    p->dirty = 0;
    switch (p->order) {
    case 0:
        RotMatrix(&p->rot, &p->mtx);
        break;
    case 1:
        RotMatrixYXZ(&p->rot, &p->mtx);
        break;
    case 2:
        RotMatrixZYX(&p->rot, &p->mtx);
        break;
    case 3:
        func_80013D54(&p->rot, &p->mtx);
        break;
    case 4:
        break;
    }
    if (p->hasScale != 0) {
        ScaleMatrixL(&p->mtx, &p->scale);
    }
}

void cam_init(u8 *arg0) {
    cam_ramp_overide_count = 0;
    (*(s32*)((u8*)(arg0) + 100)) = 0x1000;
    (*(s32*)((u8*)(arg0) + 104)) = 0x1000;
    (*(s32*)((u8*)(arg0) + 108)) = 0x1000;
    (*(s32*)((u8*)(arg0) + 8)) = 0x300;
    (*(s32*)((u8*)(arg0) + 24)) = 0x140;
    (*(s16*)((u8*)(arg0) + 84)) = 0;
    (*(s16*)((u8*)(arg0) + 86)) = 0;
    (*(s16*)((u8*)(arg0) + 88)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
    (*(s32*)((u8*)(arg0) + 16)) = 0;
    (*(s32*)((u8*)(arg0) + 20)) = 0;
    (*(s8*)((u8*)(arg0) + 149)) = 0;
    (*(s8*)((u8*)(arg0) + 148)) = 1;
    (*(s32*)((u8*)(arg0) + 0)) = 0;
    (*(s32*)((u8*)(arg0) + 4)) = 0;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 60)) = 0;
    (*(s32*)((u8*)(arg0) + 56)) = 0;
    (*(s32*)((u8*)(arg0) + 64)) = 0;
}

void func_800143F0(s32 arg0, s32 arg1) {
    s32 t;
    s32 v;
    ((s32 *)arg0)[0x68 / 4] = 0x1000;
    ((s32 *)arg0)[0x64 / 4] = (screen_aspect[1] << 12) / screen_aspect[0];
    t = (((s32 *)arg0)[0x68 / 4] * *(s16 *)(arg1 + 6)) / 480;
    ((s32 *)arg0)[0x68 / 4] = t;
    if (D_800637A8 == 1) {
        v = t * 0xF8;
    } else {
        v = t * 0xF0;
    }
    ((s32 *)arg0)[0x68 / 4] = v / 240;
    ((s32 *)arg0)[0x64 / 4] = (((s32 *)arg0)[0x64 / 4] * *(s16 *)(arg1 + 4)) / 480;
    ((s32 *)arg0)[0x6C / 4] = 0x1000;
    if (((s32 *)arg0)[0x40 / 4] != 0) {
        ((s32 *)arg0)[0x68 / 4] = -((s32 *)arg0)[0x68 / 4];
    }
}

void func_8001452C(s32 arg0, s32 arg1) {
    cam_ramp_overide_count = arg1;
    D_800517B4 = arg0;
}

void func_80014540(u8 *arg0, RECT *rect) {
    s32 t;
    s32 y;
    u16 a;
    u16 b;
    u16 c;

    t = *(s32*)(arg0 + 0x38);
    if (t != 0) {
        *(s32*)(arg0 + 0x38) = t - 1;
    }
    func_800143F0((s32)arg0, (s32)rect);
    RotMatrix((SVECTOR*)(arg0 + 0x54), (MATRIX*)(arg0 + 0x74));
    if (arg0[0x94] != 0) {
        ScaleMatrixL((MATRIX*)(arg0 + 0x74), (VECTOR*)(arg0 + 0x64));
    }
    a = *(u16*)(arg0 + 0x64);
    b = *(u16*)(arg0 + 0x68);
    c = *(u16*)(arg0 + 0x6C);
    y = *(s32*)(arg0 + 8);
    *(s32*)(arg0 + 0x88) = *(s32*)(arg0 + 0);
    *(s16*)(arg0 + 0x9A) = 0;
    *(s16*)(arg0 + 0x9C) = 0;
    *(s16*)(arg0 + 0x9E) = 0;
    *(s16*)(arg0 + 0xA2) = 0;
    *(s32*)(arg0 + 0x8C) = *(s32*)(arg0 + 4);
    *(s16*)(arg0 + 0xA4) = 0;
    *(s16*)(arg0 + 0xA6) = 0;
    *(s32*)(arg0 + 0xAC) = 0;
    *(s32*)(arg0 + 0xB0) = 0;
    *(s32*)(arg0 + 0xB4) = 0;
    *(s32*)(arg0 + 0x88) = *(s32*)(arg0 + 0x88) * *(s32*)(arg0 + 0x64);
    *(s32*)(arg0 + 0x8C) = *(s32*)(arg0 + 0x8C) * *(s32*)(arg0 + 0x68);
    *(u16*)(arg0 + 0x98) = a;
    *(u16*)(arg0 + 0xA0) = b;
    *(u16*)(arg0 + 0xA8) = c;
    *(s32*)(arg0 + 0x90) = y;
    *(s32*)(arg0 + 0x88) = *(s32*)(arg0 + 0x88) >> 12;
    *(s32*)(arg0 + 0x8C) = *(s32*)(arg0 + 0x8C) >> 12;
}

/* Angle of (x, z) in 0x1000 units per turn, from the 0x800-entry arctangent table */
s32 get_angle(s32 x, s32 z) {
    if (x == 0 && z == 0) {
        return 0;
    }
    if (x == 0x80000000) {
        x = 0x80000001;
    }
    if (z == 0x80000000) {
        z = 0x80000001;
    }
    if (x >= 0) {
        if (z >= 0) {
            if (x < z) {
                return AtanTable[(x << 11) / z];
            }
            return 0x400 - AtanTable[(z << 11) / x];
        }
        z = -z;
        if (x >= z) {
            return AtanTable[(z << 11) / x] + 0x400;
        }
        return 0x800 - AtanTable[(x << 11) / z];
    }
    x = -x;
    if (z < 0) {
        z = -z;
        if (x < z) {
            return AtanTable[(x << 11) / z] + 0x800;
        }
        return 0xC00 - AtanTable[(z << 11) / x];
    }
    if (x >= z) {
        return AtanTable[(z << 11) / x] + 0xC00;
    }
    return 0x1000 - AtanTable[(x << 11) / z];
}

void func_8001492C(void) {
    void **t = (void **)D_800527C4;

    t[0x000 / 4] = func_80027408;
    t[0x004 / 4] = &num_sfx_banks;
    t[0x008 / 4] = PlaySfx;
    t[0x00C / 4] = StopSfx;
    t[0x010 / 4] = func_8002BD0C;
    t[0x014 / 4] = func_8002232C;
    t[0x018 / 4] = sfx_delete_all;
    t[0x01C / 4] = func_800228A4;
    t[0x020 / 4] = func_8002BCB0;
    t[0x024 / 4] = func_8002AC40;
    t[0x028 / 4] = func_8002AC18;
    t[0x02C / 4] = func_80022140;
    t[0x030 / 4] = func_8002C284;
    t[0x034 / 4] = func_800222FC;
    t[0x038 / 4] = func_800222DC;
    t[0x03C / 4] = func_800222BC;
    t[0x040 / 4] = music_fade_out;
    t[0x044 / 4] = path_find;
    t[0x048 / 4] = path_get;
    t[0x04C / 4] = func_8001E21C;
    t[0x050 / 4] = func_8001E41C;
    t[0x054 / 4] = func_8001DAF8;
    t[0x058 / 4] = draw_map_sub;
    t[0x05C / 4] = map_update;
    t[0x060 / 4] = func_8001DEF4;
    t[0x064 / 4] = func_8001E054;
    t[0x068 / 4] = func_8001DDD8;
    t[0x06C / 4] = func_8001E0A8;
    t[0x070 / 4] = func_80017164;
    t[0x074 / 4] = func_800180BC;
    t[0x078 / 4] = func_800184F0;
    t[0x07C / 4] = func_80018694;
    t[0x080 / 4] = func_8001888C;
    t[0x084 / 4] = func_80017D90;
    t[0x088 / 4] = func_80018728;
    t[0x08C / 4] = func_80018430;
    t[0x090 / 4] = func_800183DC;
    t[0x094 / 4] = release_sprim;
    t[0x098 / 4] = new_sprim;
    t[0x09C / 4] = func_8001A0D8;
    t[0x0A0 / 4] = func_8001A6D4;
    t[0x0A4 / 4] = sub_play_script;
    t[0x0A8 / 4] = func_8001AA48;
    t[0x0AC / 4] = func_8001C7FC;
    t[0x0B0 / 4] = func_8001C690;
    t[0x0B4 / 4] = func_8001A82C;
    t[0x0B8 / 4] = func_8001C3D0;
    t[0x0BC / 4] = kill_sprim_sub;
    t[0x0C0 / 4] = func_800158E4;
    t[0x0C4 / 4] = func_80015870;
    t[0x0C8 / 4] = SetDrawArea;
    t[0x0CC / 4] = tga_convert;
    t[0x0D0 / 4] = func_80018F3C;
    t[0x0D4 / 4] = func_8001C270;
    t[0x0D8 / 4] = func_8001C298;
    t[0x0DC / 4] = func_8001D640;
    t[0x0E0 / 4] = func_8001D390;
    t[0x0E4 / 4] = func_8001D3B0;
    t[0x0E8 / 4] = draw_init;
    t[0x0EC / 4] = func_80018B08;
    t[0x0F0 / 4] = entity_address;
    t[0x0F4 / 4] = init_dots;
    t[0x0F8 / 4] = func_80015C1C;
    t[0x0FC / 4] = func_80015D70;
    t[0x100 / 4] = func_80015D08;
    t[0x104 / 4] = calc_rgbcode;
    t[0x108 / 4] = cam_init;
    t[0x10C / 4] = func_80013F44;
    t[0x110 / 4] = func_800276A4;
    t[0x114 / 4] = func_80028774;
    t[0x118 / 4] = func_80017070;
    t[0x11C / 4] = func_8001612C;
    t[0x120 / 4] = func_800274FC;
    t[0x124 / 4] = func_80017138;
    t[0x128 / 4] = func_80016CA0;
    t[0x12C / 4] = func_800170D4;
    t[0x130 / 4] = D_80052A54;
    t[0x134 / 4] = MulMatrix;
    t[0x138 / 4] = func_80028018;
    t[0x13C / 4] = func_80027F7C;
    t[0x140 / 4] = func_80028C94;
    t[0x144 / 4] = func_800165F8;
    t[0x148 / 4] = dbase_mesh_anim_name_loaded;
    t[0x14C / 4] = func_80016478;
    t[0x150 / 4] = func_8001682C;
    t[0x154 / 4] = func_800166FC;
    t[0x158 / 4] = func_8001636C;
    t[0x15C / 4] = func_80016450;
    t[0x160 / 4] = func_800163E0;
    t[0x164 / 4] = set_joypad;
    t[0x168 / 4] = func_800139A8;
    t[0x16C / 4] = func_80015780;
    t[0x170 / 4] = func_80012DDC;
    t[0x174 / 4] = func_8001361C;
    t[0x178 / 4] = func_80011DB4;
    t[0x17C / 4] = func_8001C34C;
    t[0x180 / 4] = load_file_permanent;
    t[0x184 / 4] = func_800134FC;
    t[0x188 / 4] = func_8001D6B4;
    t[0x18C / 4] = func_8001DCD4;
    t[0x190 / 4] = func_80015984;
    t[0x194 / 4] = func_80015758;
    t[0x198 / 4] = func_8001E598;
    t[0x19C / 4] = set_fade;
    t[0x1A0 / 4] = process_init;
    t[0x1A4 / 4] = func_8001E610;
    t[0x1A8 / 4] = func_8001E898;
    t[0x1AC / 4] = process_quit;
    t[0x1B0 / 4] = func_8001E934;
    t[0x1B4 / 4] = func_8001E848;
    t[0x1B8 / 4] = process_interrupt;
    t[0x1BC / 4] = func_8001E824;
    t[0x1C0 / 4] = func_8002992C;
    t[0x1C4 / 4] = texture_release_loading_mem;
    t[0x1C8 / 4] = func_8002D274;
    t[0x1CC / 4] = func_800229FC;
    t[0x1D0 / 4] = func_80029F9C;
    t[0x1D4 / 4] = SysPrintAt3d;
    t[0x1D8 / 4] = func_800243A0;
    t[0x1DC / 4] = card_save;
    t[0x1E0 / 4] = func_8002D15C;
    t[0x1E4 / 4] = func_80013D54;
    t[0x1E8 / 4] = func_8001E984;
    t[0x1EC / 4] = scripts_update;
    t[0x1F0 / 4] = scripts_render;
    t[0x1F4 / 4] = func_80021FB0;
    t[0x1F8 / 4] = func_80022030;
    t[0x1FC / 4] = func_8001FE80;
    t[0x200 / 4] = get_script_header;
    t[0x204 / 4] = func_80021E44;
    t[0x208 / 4] = func_80021DD4;
    t[0x20C / 4] = func_8001ACC0;
    t[0x210 / 4] = func_8001F604;
    t[0x214 / 4] = func_8002147C;
    t[0x218 / 4] = interp;
    t[0x21C / 4] = func_800152A8;
    t[0x220 / 4] = func_80015284;
    t[0x224 / 4] = func_8001C584;
    t[0x228 / 4] = func_8001C448;
    t[0x22C / 4] = D_8005B648;
    t[0x230 / 4] = D_8004DDBC;
    t[0x234 / 4] = func_8002D294;
    t[0x238 / 4] = func_8002C1C0;
    t[0x23C / 4] = music_restart;
    t[0x240 / 4] = func_8002C29C;
    t[0x244 / 4] = music_duck_off;
    t[0x248 / 4] = func_8002BB80;
    t[0x24C / 4] = func_8002BA44;
    t[0x250 / 4] = func_80020344;
    t[0x254 / 4] = (void *)D_800654B0;
    t[0x258 / 4] = (void *)D_800654B4;
    t[0x25C / 4] = func_800150E8;
    t[0x260 / 4] = func_800101E0;
    t[0x264 / 4] = func_800160F8;
    t[0x268 / 4] = func_800160C4;
    t[0x26C / 4] = func_8002D108;
    t[0x270 / 4] = func_80028158;
    t[0x274 / 4] = func_8002D488;
    t[0x278 / 4] = func_8002D4F4;
    t[0x27C / 4] = func_80012AC4;
    t[0x280 / 4] = D_80050010;
    t[0x284 / 4] = func_8001D604;
    t[0x288 / 4] = func_80021054;
    t[0x28C / 4] = func_80020F5C;
}

void func_800150E8(char *dst, s32 val, s32 div) {
    s32 q;
    s32 t;
    if (div >= 10) {
        do {
            q = val / div;
            t = div * q;
            *dst = q + '0';
            div /= 10;
            dst++;
            val -= t;
        } while (div >= 10);
    }
    *dst = val + '0';
    dst[1] = 0;
}

char *func_8001517C(char *arg0, s32 arg1, s32 arg2) {
    s32 q;

    if (arg2 >= 10) {
        do {
            q = arg1 / arg2;
            *arg0 = q + 0x30;
            arg1 -= arg2 * q;
            arg2 /= 10;
            arg0++;
        } while (arg2 >= 10);
    }
    *arg0 = arg1 + 0x30;
    return arg0 + 1;
}

s32 func_80015210(s32 a, s32 b, s32 t) {
    s32 d;

    d = b - a;
    if (abs(d) < 3) {
        return b;
    }
    d = d * t;
    d = d / 4096;
    return a + d;
}

s32 func_80015284(s32 a, s32 b, s32 t) {
    b = (b - a) * t;
    b /= 0x1000;
    return a + b;
}

s32 func_800152A8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 d;

    d = (arg1 - arg0) & 0xFFF;
    if (d >= 0x801) {
        d -= 0x1000;
    }
    return (d * arg3) / arg2 + arg0;
}

s32 interp(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return ((arg1 - arg0) * arg3) / arg2 + arg0;
}

s32 func_8001534C(u8 *s) {
    s32 sum;
    s32 c;
    s32 i;

    sum = 0;
    i = 1;
    while (*s != 0) {
        c = *s++;
        c = c * i++;
        sum += c;
    }
    return sum;
}

s32 func_8001538C(u32 x) {
    s32 bits;

    bits = 0;
    while (x != 0) {
        x >>= 1;
        bits++;
    }
    return bits;
}

s32 *func_800153A8(void) {
    return D_800527C4;
}

s32 func_800153B4(s32 *a, s32 *b, s16 *out) {
    s32 dx, dy, dz;
    s32 xx, zz;
    s32 flat;

    dx = a[0] - b[0];
    xx = dx * dx;
    dz = b[2] - a[2];
    zz = dz * dz;
    dy = b[1] - a[1];
    flat = csqrt(xx + zz) >> 6;
    out[1] = get_angle(dx, dz);
    out[0] = get_angle(dy, flat) & 0xFFF;
    return csqrt(xx + dy * dy + zz) >> 6;
}

s32 func_80015480(s32 arg0, s32 arg1) {
    s32 var_a0;

    var_a0 = (arg1 - arg0) & 0xFFF;
    if (var_a0 >= 0x801) {
        var_a0 = 0x1000 - var_a0;
    }
    return var_a0;
}

s32 func_800154A0(s32 arg0, s32 arg1) {
    s32 var_a0;

    var_a0 = (arg1 - arg0) & 0xFFF;
    if (var_a0 >= 0x801) {
        var_a0 -= 0x1000;
    }
    return var_a0;
}

void func_800154C0(u16 *arg0, s32 arg1) {
    s32 i;

    for (i = arg1 - 1; i != -1; i--) {
        *arg0++ &= 0x7FFF;
    }
}

void palette_set_top_bits(u16 *arg0, u32 arg1) {
    s32 i;

    for (i = arg1 - 1; i != -1; i--) {
        *arg0++ |= 0x8000;
    }
}

void BSwapEndian(u8 *p, u32 n) {
    s32 i;
    u8 lo;
    s32 v;

    for (i = n - 1; i != -1; i--) {
        lo = *p >> 4;
        v = ((*p << 4) & 0xF0) | lo;
        *p = v;
        p++;
    }
}

void WSwapEndian(u16 *p, u32 n) {
    s32 i;
    u16 lo;
    s32 v;

    for (i = n - 1; i != -1; i--) {
        lo = *p >> 8;
        v = ((*p << 8) & 0xFF00) | lo;
        *p = v;
        p++;
    }
}

s32 Random(s32 n) {
    if (n < 2) {
        return 0;
    }
    RandomSeedA += RandomSeedB;
    RandomSeedB += RandomSeedA + ((u32)RandomSeedA >> 31);
    return (u32)RandomSeedA % n;
}

void SetRandomSeed(s32 arg0, s32 arg1) {
    RandomSeedA = arg0;
    RandomSeedB = arg1;
}

s32 func_800155FC(void) {
    RandomSeedA += RandomSeedB;
    RandomSeedB += RandomSeedA + ((u32)RandomSeedA >> 31);
    return RandomSeedA;
}

PointList *func_8001562C(subengine *sub) {
    s32 flag;
    u8 *base;
    u8 *rec;
    s32 off;

    flag = sub->unk74;
    if (flag != 0) {
        if (sub->unk6C == NULL) {
            return NULL;
        }
        base = *(u8 **)((u8 *)sub->unk6C + 0xC);
        if (base == NULL) {
            return NULL;
        }
        switch (flag & 0x7000) {
        case 0x3000:
            break;
        case 0x4000:
            rec = func_80015A78(flag, base + (*(s32 *)(base + 0x44) + 0x44) + ((flag & 0xF80) >> 7) * 0x18);
            if (rec != NULL) {
                off = *(s32 *)(rec + 0xC);
                if (off == 0) {
                    break;
                }
                return (PointList *)(rec + (off + 0xC));
            }
            return NULL;
        case 0x5000:
            rec = (u8 *)map_entity_address(flag & 0xFFF, base);
        tail:
            off = *(s32 *)(rec + 0x2C);
            if (off != 0) {
                goto found;
            }
            return NULL;
        case 0x2000:
            rec = base + ((flag & 0xFFF) * 0x34 + 0x24);
            goto tail;
        found:
            return (PointList *)(rec + (off + 0x2C));
        default:
            return NULL;
        }
    }
    return NULL;
}

void func_80015758(SVECTOR *v, VECTOR *out) {
    gte_ldv0(v);
    gte_rt();
    gte_stlvl(out);
}

FlagPoint *func_80015780(EXTengine *eng, subengine *sub, u16 flag, VECTOR *out) {
    PointList *list = func_8001562C(sub);
    FlagPoint *pts;
    FlagPoint *pt;
    FlagPoint *p;
    s32 i;
    s32 n;

    pt = list->pts;
    if (list == NULL) {
        return NULL;
    }
    n = list->count;
    pts = pt;
    for (i = 0;;) {
        if (--n == -1) {
            goto none;
        }
        p = &pts[i];
        if (p->flags & flag) {
            if (eng->unk5E != 0) {
                func_800142D4(&eng->unk1C);
            }
            func_8001CD04(sub, eng->unk3C, &eng->unk10);
            gte_ldv0(p);
            gte_rt();
            gte_stlvl(out);
            return pt;
        }
        i++;
        pt++;
    }
none:
    return NULL;
}

u8 *func_80015870(u8 *arg0, s32 arg1) {
    u8 *base;
    u8 *rec;

    base = *(u8 **)(arg0 + 0xC);
    switch (arg1 & 0x7000) {
    case 0x5000:
        rec = (u8 *)map_entity_address(arg1 & 0xFFF, base);
        break;
    case 0x2000:
        rec = base + ((arg1 & 0xFFF) * 0x34 + 0x24);
        break;
    default:
        rec = NULL;
        break;
    }
    if (rec != NULL) {
        return rec + 0xC;
    }
    return NULL;
}

s32 func_800158E4(u8 *arg0, s32 arg1) {
    u8 *base;
    u8 *p;
    s32 k;
    u16 v;

    base = *(u8 **)(arg0 + 0xC);
    k = arg1 & 0x7000;
    switch (k) {
    case 0x5000:
        p = (u8 *)map_entity_address(arg1 & 0xFFF, base);
        break;
    case 0x2000:
        p = base + ((arg1 & 0xFFF) * 0x34 + 0x24);
        break;
    default:
        p = NULL;
        break;
    }
    if (p != NULL) {
        v = *(u16 *)(*(s32 *)(p + 0x1C) + (s32)p + 0x1C);
        if (!(v & 0x8000)) {
            return (v & 0x1FF) | 0x3000;
        }
        return -1;
    }
    return -1;
}

s32 func_80015984(s32 i, EntryBlock *blk) {
    s32 idx = i;

    u8 *base;

    if (blk == NULL) {
        return -1;
    }
    base = (u8 *)blk + (blk->entries + 0x1C);
    idx = (idx - 1) & 0xFFF;
    return *(s32 *)(base + idx * 12 + 0xC);
}

s32 map_entity_address(s32 arg0, u8 *arg1) {
    u8 *base;
    s32 t;
    s32 r;

    t = (arg0 - 1) & 0xFFF;
    base = arg1 + (*(s32 *)(arg1 + 0x1C) + 0x1C);
    r = func_8001DD20((s32 *)(base + (t * 0xC + 4)));
    if (r == 0) {
        return 0;
    }
    return r;
}

u8 *entity_address(s32 arg0, u8 *arg1) {
    s32 k;

    k = arg0 & 0x7000;
    if (k != 0x2000) {
        if (k == 0x5000) {
            return (u8 *)map_entity_address(arg0 & 0xFFF, arg1);
        }
        return NULL;
    }
    return arg1 + ((arg0 & 0xFFF) * 0x34 + 0x24);
}

u8 *func_80015A78(s32 i, u8 *rec) {
    void **node;
    u8 *data;

    node = *(void ***)(rec + 0x14);
    data = NULL;
    if (node != NULL) {
        data = *node;
    }
    if (data == NULL) {
        return NULL;
    }
    return data + ((i & 0x7F) * 0x10 + 4);
}

void *func_80015AB0(s32 *arg0, u8 *rec) {
    void **node;

    node = *(void ***)(rec + 0x14);
    if (node == NULL) {
        return NULL;
    }
    return *node;
}

void init_dots(subengine *arg0) {
    s32 *p = (s32 *)((u8 *)arg0 + 0x6C);
    s32 n = p[3];
    TILE_1 *a = (TILE_1 *)p[1];
    TILE_1 *b = (TILE_1 *)p[2];
    s16 *vel = ((s16 **)arg0)[0x6C / 4];

    if (n != 0) {
        do {
            a->x0 = Random(0x200);
            b->x0 = Random(0x200);
            a->y0 = Random(0x200);
            b->y0 = Random(0x200);
            a->r0 = Random(0xFF);
            a->g0 = Random(0xFF);
            a->b0 = Random(0xFF);
            b->r0 = Random(0xFF);
            b->g0 = Random(0xFF);
            b->b0 = Random(0xFF);
            SetTile1(a);
            SetTile1(b);
            vel[0] = Random(0x2000) - 0x1000;
            vel[1] = Random(0x2000) - 0x1000;
            vel[2] = Random(0x2000) - 0x1000;
            a++;
            b++;
            vel += 12;
            n--;
        } while (n != 0);
    }
}

void func_80015C1C(PointSprites *ps, MATRIX *m) {
    PointSpriteSet *set = &ps->set;
    PointSpriteSrc *src = set->src;
    TILE_1 *prim = set->prims[D_8005B690 & 1];
    s32 n;

    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
    n = set->count;
    for (;;) {
        if (n == 0) {
            return;
        }
        gte_ldv0(&src->v);
        gte_rtps();
        gte_stsxy(&prim->x0);
        src++;
        n--;
        addPrim(D_80056998.ot, prim);
        prim++;
    }
}
void func_80015D08(u8 *arg0) {
    u8 *temp_s2;

    temp_s2 = arg0 + 0x6C;
    func_80011578((*(s32*)((u8*)(temp_s2) + 4)), &D_8004E0F0, 2);
    func_80011578((*(s32*)((u8*)(temp_s2) + 8)), &D_8004E0F0, 2);
    mal_free((*(s32*)((u8*)(arg0) + 108)), &D_8004E0F0);
}

s32 func_80015D70(u8 *arg0, u8 *arg1) {
    u8 *temp_s0;
    u8 *temp_s4;
    s32 var_v0;

    temp_s4 = arg1 + 0x70;
    temp_s0 = arg1 + 0x6C;
    *(s32 *)(temp_s0 + 4) = 0;
    *(s32 *)(temp_s0 + 8) = 0;
    *(s32 *)(arg1 + 0x6C) = 0;
    *(s32 *)(temp_s0 + 0xC) = 0;
    func_800115D8(*(s32 *)(arg0 + 4), (void **)temp_s4, (s32)&D_8004E0F0, 2);
    if (*(s32 *)(temp_s0 + 4) == 0) {
        var_v0 = 0;
    } else {
        func_800115D8(*(s32 *)(arg0 + 4), (void **)(arg1 + 0x74), (s32)&D_8004E0F0, 3);
        if (*(s32 *)(temp_s0 + 8) == 0) {
            mal_free(temp_s4, &D_8004E0F0);
            var_v0 = 0;
        } else {
            func_800115D8(*(s32 *)(arg0 + 8), (void **)temp_s0, (s32)&D_8004E0F0, 3);
            *(s32 *)(temp_s0 + 0xC) = *(s32 *)arg0;
            var_v0 = *(s32 *)(arg1 + 0x6C);
        }
    }
    if (var_v0 != 0) {
        s32 temp_v1 = *(s32 *)(arg0 + 0x10);
        *(void **)(arg1 + 0x58) = func_80015D08;
        *(s32 *)(arg1 + 0x54) = temp_v1;
        (*(void (**)(u8 *))(arg0 + 0xC))(arg1);
    } else {
        *(s32 *)(arg1 + 0x54) = 0;
        *(s32 *)(arg1 + 0x58) = 0;
    }
    return *(s32 *)(arg1 + 0x78);
}

s32 func_80015E94(u8 *arg0, u8 *arg1) {
    u8 *temp_s3;

    temp_s3 = arg1 + 4;
    (*(s32*)((u8*)(arg1) + 4)) = 0;
    (*(s32*)((u8*)(arg1) + 8)) = 0;
    (*(s32*)((u8*)(arg1) + 0)) = 0;
    (*(s32*)((u8*)(arg1) + 12)) = 0;
    func_800115D8((*(s32*)((u8*)(arg0) + 4)), temp_s3, &D_8004E0F0, 2);
    if ((*(s32*)((u8*)(arg1) + 4)) != 0) {
        func_800115D8((*(s32*)((u8*)(arg0) + 4)), arg1 + 8, &D_8004E0F0, 3);
        if ((*(s32*)((u8*)(arg1) + 8)) != 0) {
            func_800115D8((*(s32*)((u8*)(arg0) + 8)), arg1, &D_8004E0F0, 3);
            (*(s32*)((u8*)(arg1) + 12)) = (s32) (*(s32*)((u8*)(arg0) + 0));
            return (*(s32*)((u8*)(arg1) + 0));
        }
        mal_free(temp_s3, &D_8004E0F0);
        goto block_4;
    }
block_4:
    return 0;
}

void func_80015F60(u8 *arg0, s32 *arg1) {
    FileEntry *tbl;
    FileEntry *e;
    FileEntry *next;
    s32 *p;
    s32 found;
    BlockRef *blk;
    BlockRef *old;
    s32 i;

    i = *(s32 *)(arg0 + 0x40) - 1;
    tbl = (FileEntry *)(arg0 + (*(s32 *)(arg0 + 0x44) + 0x44));
    next = (FileEntry *)(i * sizeof(FileEntry) + (u32)tbl);
    for (;;) {
        if (i < 0) {
            goto second;
        }
        e = next;
        blk = func_80012E28();
        e->block = blk;
        blk->ptr = func_80011828(arg0, e->start);
        i--;
        func_80011544(blk->ptr, &blk->ptr);
        func_80011498(blk->ptr, e->end - e->start, &D_8004E0F0);
        next = e - 1;
    }
second:
    if (arg1 != NULL) {
        e = tbl;
        i = *(s32 *)(arg0 + 0x40);
        for (;;) {
            if (i == 0) {
                goto end;
            }
            p = arg1;
            if (p != NULL) {
                for (;; p++) {
                    if (*p == 0) {
                        goto none;
                    }
                    if (func_8001534C((u8 *)*p) == e->hash) {
                        found = *p;
                        goto chk;
                    }
                }
            }
none:
            found = 0;
chk:
            if (found == 0) {
                old = e->block;
                mal_free(old->ptr, &D_8004E0F0);
                e->block = NULL;
                func_80012E14((u8 *)old);
            }
            e++;
            i--;
        }
    }
end:
}

u8 *func_800160C4(u8 *arg0, s32 i) {
    if (arg0 != NULL) {
        arg0 += 0x10;
        if (*(s32 *)(arg0 + 8) != 0) {
            return *(u8 **)(arg0 + 0xC) + i * 0xC;
        }
    }
    return NULL;
}

s32 func_800160F8(u8 *arg0, s32 arg1) {
    s32 temp_a0;

    if (arg0 != NULL) {
        temp_a0 = (*(s32*)((u8*)(arg0) + 24));
        if (temp_a0 != 0) {
            return temp_a0 + ((arg1 & 0xFFF) * 0x38);
        }
    }
    return 0;
}

void func_8001612C(u8 *arg0, s32 arg1) {
    if (((*(s32*)((u8*)(arg0) + 8)) == 0) && ((*(s32*)((u8*)(arg0) + 12)) == 0)) {
        (*(s32*)((u8*)(arg0) + 52)) = arg1;
        (*(s32*)((u8*)(arg0) + 8)) = func_80013650((*(s32*)((u8*)(arg0) + 4)), dbase_asload_with_anims_texture_callback, arg0);
    }
}

void dbase_asload_with_anims_texture_callback(u8 *arg0, u8 *arg1) {
    Heap *heap;
    u8 *p;
    s32 n;

    n = func_8002A650(arg0);
    heap = &D_8004E0F0;
    if (mal_first_free((n + 3) >> 2, heap->free) == NULL) {
        mal_free(arg0, heap);
        *(s32 *)(arg1 + 8) = 0;
        func_80016D44(arg1);
    } else {
        p = arg1 + 0x10;
        func_8002A5A4(p, arg0);
        func_80029560(p);
        texture_release_loading_mem(p);
    }
    func_80013650(*(s32 *)arg1, func_80016234, arg1);
}

void func_80016234(u8 *arg0, u8 *arg1) {
    func_80011544(arg0, arg1 + 0xC);
    func_80015F60(arg0, (*(s32*)((u8*)(arg1) + 52)));
}

s32 func_80016274(s32 *list, u8 *arg1) {
    if (list != NULL) {
        for (;; list++) {
            if (*list == 0) {
                return 0;
            }
            if (func_8001534C((u8 *)*list) == *(s32 *)(arg1 + 0x10)) {
                return *list;
            }
        }
    }
    return 0;
}

void func_800162E4(u8 *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_s0;

    temp_v0 = func_80012E28();
    (*(s32*)((u8*)(arg0) + 20)) = temp_v0;
    temp_s0 = (*(s32*)((u8*)(arg0) + 4)) - (*(s32*)((u8*)(arg0) + 0));
    func_800131B0(temp_s0);
    func_800115D8(temp_s0, temp_v0, &D_8004E0F0, 0);
    memcpy(*temp_v0, arg1 + (*(s32*)((u8*)(arg0) + 0)), temp_s0);
}

void func_8001636C(u8 *arg0, s32 arg1) {
    u8 *temp_s0;
    u8 *temp_v0;

    temp_v0 = (*(s32*)((u8*)(arg0) + 12));
    temp_s0 = temp_v0 + ((*(s32*)((u8*)(temp_v0) + 60)) + 0x3C) + ((arg1 * 0x10) + 4);
    if ((*(s32*)((u8*)(temp_s0) + 8)) != 0) {
        mal_free((void *)(*(s32*)((u8*)(temp_s0) + 8)), &D_8004E0F0);
        (*(s32*)((u8*)(temp_s0) + 12)) = 0;
    } else if ((*(s32*)((u8*)(temp_s0) + 12)) != 0) {
        func_80012F04((void *)(*(s32*)((u8*)(temp_s0) + 12)));
        (*(s32*)((u8*)(temp_s0) + 12)) = 0;
    }
}


void func_800163E0(u8 *arg0, s32 arg1) {
    s32 temp_a1;
    u8 *temp_s0;
    u8 *temp_v1;

    temp_v1 = (*(s32*)((u8*)(arg0) + 12));
    temp_s0 = temp_v1 + ((*(s32*)((u8*)(temp_v1) + 60)) + 0x3C) + ((arg1 * 0x10) + 4);
    if (((*(s32*)((u8*)(temp_s0) + 8)) == 0) && ((*(s32*)((u8*)(temp_s0) + 12)) == 0)) {
        temp_a1 = (*(s32*)((u8*)(temp_s0) + 0));
        (*(s32*)((u8*)(temp_s0) + 12)) = asload_chunk_relocatable((*(s32*)((u8*)(arg0) + 0)), temp_a1, temp_s0 + 8, (*(s32*)((u8*)(temp_s0) + 4)) - temp_a1);
    }
}

s32 func_80016450(u8 *arg0, s32 arg1) {
    u8 *temp_s0;
    u8 *temp_v0;

    temp_v0 = (*(s32*)((u8*)(arg0) + 12));
    temp_s0 = temp_v0 + ((*(s32*)((u8*)(temp_v0) + 60)) + 0x3C) + ((arg1 * 0x10) + 4);
    return (*(s32*)((u8*)(temp_s0) + 8));
}


u8 *func_80016478(u8 *arg0, u8 *arg1) {
    u8 *tbl;
    u8 *p;
    s32 n;
    s32 key;
    u8 *res;
    u8 *blk;

    tbl = *(u8 **)(arg0 + 0xC);
    if (tbl == NULL) {
        res = NULL;
    } else {
        key = func_8001534C(arg1);
        p = tbl + (*(s32 *)(tbl + 0x44) + 0x44);
        n = *(s32 *)(tbl + 0x40);
        n--;
        for (; n != -1; n--, p += 0x18) {
            if (*(s32 *)(p + 0x10) == key) {
                res = p;
                goto found;
            }
        }
        res = NULL;
    }
found:
    if (res != NULL) {
        blk = *(u8 **)(res + 0x14);
        if (blk != NULL) {
            if (*(void **)blk != NULL) {
                mal_free(*(void **)blk, &D_8004E0F0);
            } else {
                func_80012F04(*(LoadNode **)(blk + 4));
            }
            func_80012E14(blk);
            *(u8 **)(res + 0x14) = NULL;
            return res;
        }
    }
    return NULL;
}

s32 dbase_mesh_anim_name_loaded(u8 *arg0, char *arg1) {
    u8 *tbl;
    u8 *p;
    s32 n;
    s32 key;
    u8 *res;

    tbl = *(u8 **)(arg0 + 0xC);
    if (tbl == NULL) {
        res = NULL;
    } else {
        key = func_8001534C(arg1);
        p = tbl + (*(s32 *)(tbl + 0x44) + 0x44);
        n = *(s32 *)(tbl + 0x40);
        n--;
        for (; n != -1; n--, p += 0x18) {
            if (*(s32 *)(p + 0x10) == key) {
                res = p;
                goto found;
            }
        }
        res = NULL;
    }
found:
    if (res != NULL) {
        return **(s32 **)(res + 0x14);
    }
    return 0;
}

u8 *func_800165F8(u8 *arg0, u8 *arg1) {
    u8 *tbl;
    u8 *p;
    s32 n;
    s32 key;
    u8 *res;
    u8 *node;
    u8 *cur;
    s32 start;
    s32 h;

    tbl = *(u8 **)(arg0 + 0xC);
    if (tbl == NULL) {
        res = NULL;
    } else {
        key = func_8001534C(arg1);
        p = tbl + (*(s32 *)(tbl + 0x44) + 0x44);
        n = *(s32 *)(tbl + 0x40);
        n--;
        for (; n != -1; n--, p += 0x18) {
            if (*(s32 *)(p + 0x10) == key) {
                res = p;
                goto found;
            }
        }
        res = NULL;
    }
found:
    if (res != NULL) {
        cur = *(u8 **)(res + 0x14);
        if (cur != NULL) {
            return cur;
        }
        {
            node = func_80012E28();
            if (node != NULL) {
                start = *(s32 *)res;
                *(u8 **)(res + 0x14) = node;
                h = asload_chunk_relocatable(*(s32 *)arg0, start, node, *(s32 *)(res + 4) - start);
                *(s32 *)(node + 4) = h;
                if (h == 0) {
                    func_80012E14(node);
                    return NULL;
                }
                return node;
            }
            return NULL;
        }
    }
    return NULL;
}

s32 func_800166FC(u8 *arg0, u8 *arg1) {
    u8 *tbl;
    u8 *p;
    s32 n;
    s32 key;
    s32 d;

    u8 *res;

    tbl = *(u8 **)(arg0 + 0xC);
    if (tbl == NULL) {
        res = NULL;
    } else {
        key = func_8001534C(arg1);
        p = tbl + (*(s32 *)(tbl + 0x44) + 0x44);
        n = *(s32 *)(tbl + 0x40);
        n--;
        for (; n != -1; n--, p += 0x18) {
            if (*(s32 *)(p + 0x10) == key) {
                res = p;
                goto found;
            }
        }
        res = NULL;
    }
found:
    d = *(s32 *)(res + 4) - *(s32 *)res;
    return ((d + 0x7FF) / 0x800) * 0x800;
}

u8 *func_800167A8(u8 *arg0, u8 *arg1) {
    u8 *p;
    s32 n;
    s32 key;

    if (arg0 == NULL) {
        return NULL;
    }
    key = func_8001534C(arg1);
    p = arg0 + (*(s32 *)(arg0 + 0x44) + 0x44);
    n = *(s32 *)(arg0 + 0x40);
    n--;
    for (; n != -1; n--, p += 0x18) {
        if (*(s32 *)(p + 0x10) == key) {
            return p;
        }
    }
    return NULL;
}

void func_8001682C(s32 *arg0) {
    u8 *tbl;
    u8 *t;
    u8 *rec;
    u8 *node;
    s32 n;
    s32 i;
    s32 start;
    s32 handle;

    tbl = *(u8 **)(arg0 + 3);
    if (tbl != NULL) {
        n = *(s32 *)(tbl + 0x40);
        for (i = 0;; i++) {
            if (i >= n) {
                return;
            }
            t = *(u8 **)(arg0 + 3);
            if (t != NULL && i < *(s32 *)(t + 0x40)) {
                rec = t + (*(s32 *)(t + 0x44) + 0x44);
                rec += i * 0x18;
                if (*(u8 **)(rec + 0x14) == NULL) {
                    node = func_80012E28();
                    if (node != NULL) {
                        start = *(s32 *)rec;
                        *(u8 **)(rec + 0x14) = node;
                        handle = asload_chunk_relocatable(*arg0, start, node, *(s32 *)(rec + 4) - start);
                        *(s32 *)(node + 4) = handle;
                        if (handle == 0) {
                            func_80012E14(node);
                        }
                    }
                }
            }
        }
    }
}

void dbase_release_all_mesh_anims(a)
u8 *a;
{
    u8 **slot;
    u8 *e;
    u8 *p;
    s32 off;
    s32 n;
    s32 i;
    u8 *arg0;

    arg0 = a;
    i = 0;
    if (*(u8 **)(arg0 + 0xC) != NULL) {
        n = *(s32 *)(*(u8 **)(arg0 + 0xC) + 0x40);
        off = 0;
    again:
        if (i < n) {
            p = *(u8 **)(arg0 + 0xC);
            if (p != NULL && i < *(s32 *)(p + 0x40)) {
                slot = (u8 **)(p + (*(s32 *)(p + 0x44) + 0x44));
                slot = (u8 **)((u8 *)slot + off);
                e = slot[5];
                if (e != NULL) {
                    if (*(void **)e != NULL) {
                        mal_free(*(void **)e, &D_8004E0F0);
                    } else {
                        func_80012F04(*(void **)(e + 4));
                    }
                    func_80012E14(e);
                    slot[5] = NULL;
                }
            }
            off += 0x18;
            i++;
            goto again;
        }
    }
}

u8 *func_80016A20(u8 *arg0, s32 arg1) {
    u8 *base;
    u8 *e;
    void **item;

    base = *(u8 **)(arg0 + 0xC);
    if (base != NULL) {
        if (arg1 < *(s32 *)(base + 0x40)) {
            e = base + (*(s32 *)(base + 0x44) + 0x44);
            e += arg1 * 0x18;
            item = *(void ***)(e + 0x14);
            if (item != NULL) {
                if (item[0] != NULL) {
                    mal_free(item[0], &D_8004E0F0);
                } else {
                    func_80012F04(item[1]);
                }
                func_80012E14((u8 *)item);
                *(s32 *)(e + 0x14) = 0;
                return e;
            }
        }
    }
    return NULL;
}

/* Unloads what func_80016C18 loaded: the record's node at +0x14 holds a heap
 * block or (if that is NULL) a handle at +4. */
u8 *func_80016AD4(s32 *arg0, u8 *rec) {
    u8 *node;

    node = *(u8 **)(rec + 0x14);
    if (node == NULL) {
        return NULL;
    }
    if (*(void **)node != NULL) {
        mal_free(*(void **)node, &D_8004E0F0);
    } else {
        func_80012F04(*(void **)(node + 4));
    }
    func_80012E14(node);
    *(u8 **)(rec + 0x14) = NULL;
    return rec;
}

/* Inlined copy of func_80016C18 (load a record's node); being an inline call
 * is what puts arg1 in v1 and the table in a1. */
static inline u8 *func_80016B48_load(s32 *arg0, u8 *arg1) {
    u8 *cur;
    u8 *node;
    s32 start;
    s32 h;

    cur = *(u8 **)(arg1 + 0x14);
    if (cur == NULL) {
        node = func_80012E28();
        if (node != NULL) {
            start = *(s32 *)arg1;
            *(u8 **)(arg1 + 0x14) = node;
            h = asload_chunk_relocatable(*arg0, start, node, *(s32 *)(arg1 + 4) - start);
            *(s32 *)(node + 4) = h;
            if (h != 0) {
                goto ok;
            }
            func_80012E14(node);
        }
        return NULL;
    ok:
        return node;
    }
    return cur;
}

u8 *func_80016B48(u8 *arg0, s32 arg1) {
    u8 *base;
    u8 *e;

    base = *(u8 **)(arg0 + 0xC);
    if (base == NULL || arg1 >= *(s32 *)(base + 0x40)) {
        return NULL;
    }
    e = base + (*(s32 *)(base + 0x44) + 0x44);
    e += arg1 * 0x18;
    return func_80016B48_load((s32 *)arg0, e);
}

u8 *func_80016C18(s32 *arg0, u8 *arg1) {
    s32 temp_a1;
    s32 temp_v0_2;
    u8 *temp_v0;
    u8 *var_v0;

    var_v0 = (*(s32*)((u8*)(arg1) + 20));
    if (var_v0 == NULL) {
        temp_v0 = func_80012E28();
        if (temp_v0 != NULL) {
            temp_a1 = (*(s32*)((u8*)(arg1) + 0));
            (*(s32*)((u8*)(arg1) + 20)) = temp_v0;
            temp_v0_2 = asload_chunk_relocatable(*arg0, temp_a1, temp_v0, (*(s32*)((u8*)(arg1) + 4)) - temp_a1);
            (*(s32*)((u8*)(temp_v0) + 4)) = temp_v0_2;
            if (temp_v0_2 != 0) {
                return temp_v0;
            }
            func_80012E14(temp_v0);
            goto block_5;
        }
block_5:
        var_v0 = NULL;
        /* Duplicate return node #6. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}

void func_80016CA0(u8 *rec) {
    if (*(s32 *)(rec + 8) == 0 && *(s32 *)(rec + 0xC) == 0) {
        *(s32 *)(rec + 8) = func_80013650(*(s32 *)(rec + 4), func_80016D98, rec);
    }
    for (;;) {
        if (func_80012FFC() == 0) {
            goto done;
        }
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
done:
    DrawSync(0);
    while (func_80011DB4(&D_8004E0F0) != NULL) {
    }
}

void func_80016D44(u8 *arg0) {
    if (((*(s32*)((u8*)(arg0) + 8)) == 0) && ((*(s32*)((u8*)(arg0) + 12)) == 0)) {
        (*(s32*)((u8*)(arg0) + 8)) = func_80013650((*(s32*)((u8*)(arg0) + 4)), func_80016D98, arg0);
    }
}

void func_80016D98(u8 *data, u8 *rec) {
    Heap *heap;
    s32 size;

    size = func_8002A650(data);
    heap = &D_8004E0F0;
    if (mal_first_free((size + 3) >> 2, heap->free) == NULL) {
        mal_free(data, heap);
        *(s32 *)(rec + 8) = 0;
        func_80016D44(rec);
    } else {
        func_8002A5A4(rec + 0x10, data);
        func_80029560(rec + 0x10);
        texture_release_loading_mem(rec + 0x10);
    }
    *(s32 *)(rec + 8) = func_80013034(*(s32 *)(rec + 0), 0, *(s32 *)(rec + 0x24), func_80016F34, rec, 1, 0);
}

void func_80016E5C(u8 *arg0, u8 *arg1) {
    Heap *heap;
    s32 n;

    n = func_8002A650(arg0);
    heap = &D_8004E0F0;
    if (mal_first_free((n + 3) >> 2, heap->free) == NULL) {
        mal_free(arg0, heap);
        *(s32 *)(arg1 + 8) = 0;
        func_80016D44(arg1);
    } else {
        arg1 += 0x10;
        func_8002A5A4(arg1, arg0);
        func_80029560(arg1);
        texture_release_loading_mem(arg1);
    }
}


void func_80016EF4(u8 *arg0, u8 *arg1) {
    u8 *temp_s0;

    temp_s0 = arg0 + 0x10;
    func_8002A5A4(temp_s0, arg1);
    func_80029560(temp_s0);
    (*(s32*)((u8*)(arg0) + 32)) = 0;
}

void func_80016F34(u8 *arg0, u8 *arg1) {
    func_80011544(arg0, arg1 + 0xC);
}

void func_80016F54(u8 *arg0) {
    s32 i;
    u8 *p;
    u8 *q;

    if (*(u8 **)(arg0 + 0xC) != NULL) {
        dbase_release_all_mesh_anims();
        p = *(u8 **)(arg0 + 0xC);
        q = p + 0x58;
        i = *(s32 *)(p + 0x54);
        i--;
        if (i != -1) {
            do {
                u8 *e = *(u8 **)q;
                if (e != NULL) {
                    *(s16 *)(e + 0x56) = 1;
                }
                i--;
                q += 0x34;
            } while (i != -1);
        }
        mal_free(*(void **)(arg0 + 0xC), &D_8004E0F0);
        *(s32 *)(arg0 + 0xC) = 0;
        func_80029790(arg0 + 0x10);
        *(s32 *)(arg0 + 8) = 0;
        return;
    }
    if (*(s32 *)(arg0 + 8) != 0) {
        if (*(s32 *)(arg0 + 0x18) != 0) {
            func_80029790(arg0 + 0x10);
        }
        func_80012F04(*(void **)(arg0 + 8));
        *(s32 *)(arg0 + 8) = 0;
    }
}

void func_8001702C(SlotList *arg0) {
    s32 i;
    SlotEntry *e;

    i = arg0->count;
    i--;
    e = arg0->entries;
    while (i != -1) {
        if (e->target != NULL) {
            e->target->flag = 1;
        }
        i--;
        e++;
    }
}

void func_80017070(void) {
    func_80016CA0(D_80055684);
}

void func_80017094(u8 *arg0, s32 arg1, s32 arg2) {
    s32 temp_s0;

    temp_s0 = arg0 + 0x10;
    (*(s32*)((u8*)(arg0) + 12)) = arg1;
    (*(s32*)((u8*)(arg0) + 32)) = arg2;
    func_8002A5A4(temp_s0, arg2);
    func_80029560(temp_s0);
}

void func_800170D4(void) {
    s32 i;
    s32 end;
    u8 *base;
    u8 *p;

    i = 0x11B;
    end = -1;
    base = D_80052A54;
    p = base + 0x10;
loop:
    i--;
    if (i != end) {
        *(s32 *)(p - 8) = 0;
        *(s32 *)(p - 4) = 0;
        *(s32 *)(p + 0x24) = 0;
        func_8002AA8C(p);
        p += 0x38;
        goto loop;
    }
}

void func_80017138(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
    (*(s32*)((u8*)(arg0) + 52)) = 0;
    func_8002AA8C(arg0 + 0x10);
}

static inline void func_80017164_clear(u8 *p) {
    *(s32 *)(p + 4) = 0;
    *(s32 *)(p + 8) = 0;
    *(s32 *)(p + 0xC) = 0;
    *(s32 *)(p + 0x10) = 0;
}
