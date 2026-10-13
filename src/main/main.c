#include "common.h"
#include "bash/main.h"

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010000);

void func_80010158(void) {
    D_8004E0E0 = 2;
    func_80011BF8(&D_8004E0F0, D_80078C90, 0x801FF000 - (s32)D_80078C90);
    func_80012E90();
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

    func_8002AB44();
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

void func_80010278(void) {
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
    func_80012E90();
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
    func_800237BC();
    D_8004E0DC->unk0();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010488);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800107C8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001097C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010AE8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010C70);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80010E8C);

void func_800110A8(void *ptr, Heap *heap) {
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_800112A4);

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

void func_80011474(void) {
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
        func_800110A8((u8 *)new_blk + 0x18, heap);
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

void *func_800115D8(s32 arg0, void **arg1, s32 arg2, s16 arg3) {
    u8 *blk;
    u8 *ret;

    do {
        blk = func_80010E8C(arg0, arg2);
        if (blk == NULL) {
            blk = func_80010E8C(arg0, arg2);
            if (blk == NULL) {
                return NULL;
            }
        }
        ret = blk + 0x18;
        *(void **)(blk + 4) = arg1;
    } while (0);
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
            func_80011474();
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
            func_80011474();
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_800118BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800119D4);

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

void func_80011B74(u8 *arg0, u8 *arg1) {
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

MemBlock *func_80011C5C(u32 words, MemBlock *blk) {
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
                func_800110A8(var_a0 + 0x18, &D_8004E0F0);
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

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", func_80011E80);
#else
MemBlock *func_80011E80(Heap *heap) {
    MemBlock *blk;
    MemBlock *next;

    blk = heap->lowest;
    next = NULL;
    if (blk != NULL) {
        next = (MemBlock *)((u32 *)(blk + 1) + blk->size);
        if (next->owner == (void **)-1 || next->size == 0) {
            next = NULL;
        }
    }
    if (next != NULL && !(next->unk0 & 2)) {
        return next;
    }
    return NULL;
}
#endif

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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80011FC8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800120E8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800121F8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001231C);

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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012678);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012840);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800129D0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012AC4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012BC8);

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

void func_80012E90(void) {
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80012FFC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013034);

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
    if (func_80011C5C(words, h->free) == NULL) {
        func_80017640();
        if (func_80011C5C(words, h->free) == NULL) {
            func_80011D28((u8 *)h);
            if (func_80011C5C(words, h->free) == NULL) {
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

s32 func_800133C8(s32 arg0, s32 arg1, u8 *arg2, s32 arg3) {
    return func_80013034(arg0, arg1, arg3, NULL, arg2, 1, 0);
}

s32 func_800133FC(s32 *arg0) {
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

s32 func_80013688(s32 arg0, void (*done)(), void *ctx) {
    return func_80013034(arg0, 0, ((s32 *)arg0)[1], done, ctx, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800136BC);

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


INCLUDE_ASM("asm/nonmatchings/main/main", func_800139A8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013B0C);

void func_80013C24(void) {
    if (D_800512A4 != 0) {
        D_800512A4 = 0;
        D_80051638[0] = 4;
        D_80051638[1] = 3;
        return;
    }
    D_800512A4 = 1;
    D_80051638[0] = 0x10;
    D_80051638[1] = 9;
}

void func_80013C78(void) {
    u32 *p = (u32 *)D_8005A614;

    p[1] |= 0x80010000;
}

void func_80013C94(u8 *arg0, s32 arg1) {
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013D54);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80013F44);

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

void func_80014388(u8 *arg0) {
    D_800517B0 = 0;
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_800143F0);

void func_8001452C(s32 arg0, s32 arg1) {
    D_800517B0 = arg1;
    D_800517B4 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80014540);

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

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001492C);

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
    if (func_80032F3C(d) < 3) {
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

s32 func_80015304(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
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

void func_800154EC(u16 *arg0, s32 arg1) {
    s32 i;

    for (i = arg1 - 1; i != -1; i--) {
        *arg0++ |= 0x8000;
    }
}

void func_80015518(u8 *p, s32 n) {
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

void func_80015554(u16 *p, s32 n) {
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

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", func_800155FC);
#else
void func_800155FC(void) {
    RandomSeedA += RandomSeedB;
    RandomSeedB += RandomSeedA + ((u32)RandomSeedA >> 31);
}
#endif

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001562C);

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
        rec = (u8 *)func_800159C4(arg1 & 0xFFF, base);
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
        p = (u8 *)func_800159C4(arg1 & 0xFFF, base);
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

s32 func_800159C4(s32 arg0, u8 *arg1) {
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

u8 *func_80015A18(s32 arg0, u8 *arg1) {
    s32 k;

    k = arg0 & 0x7000;
    if (k != 0x2000) {
        if (k == 0x5000) {
            return (u8 *)func_800159C4(arg0 & 0xFFF, arg1);
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015AD4);

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
    func_800110A8((*(s32*)((u8*)(arg0) + 108)), &D_8004E0F0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015D70);

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
        func_800110A8(temp_s3, &D_8004E0F0);
        goto block_4;
    }
block_4:
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80015F60);

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
        (*(s32*)((u8*)(arg0) + 8)) = func_80013650((*(s32*)((u8*)(arg0) + 4)), func_80016188, arg0);
    }
}

void func_80016188(u8 *arg0, u8 *arg1) {
    Heap *heap;
    u8 *p;
    s32 n;

    n = func_8002A650(arg0);
    heap = &D_8004E0F0;
    if (func_80011C5C((n + 3) >> 2, heap->free) == NULL) {
        func_800110A8(arg0, heap);
        *(s32 *)(arg1 + 8) = 0;
        func_80016D44(arg1);
    } else {
        p = arg1 + 0x10;
        func_8002A5A4(p, arg0);
        func_80029560(p);
        func_8002A35C(p);
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
        func_800110A8((void *)(*(s32*)((u8*)(temp_s0) + 8)), &D_8004E0F0);
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
        (*(s32*)((u8*)(temp_s0) + 12)) = func_800133C8((*(s32*)((u8*)(arg0) + 0)), temp_a1, temp_s0 + 8, (*(s32*)((u8*)(temp_s0) + 4)) - temp_a1);
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
                func_800110A8(*(void **)blk, &D_8004E0F0);
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

s32 func_80016558(u8 *arg0, u8 *arg1) {
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_800165F8);

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
                        handle = func_800133C8(*arg0, start, node, *(s32 *)(rec + 4) - start);
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

void func_80016928(a)
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
                        func_800110A8(*(void **)e, &D_8004E0F0);
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
                    func_800110A8(item[0], &D_8004E0F0);
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
        func_800110A8(*(void **)node, &D_8004E0F0);
    } else {
        func_80012F04(*(void **)(node + 4));
    }
    func_80012E14(node);
    *(u8 **)(rec + 0x14) = NULL;
    return rec;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80016B48);

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
            temp_v0_2 = func_800133C8(*arg0, temp_a1, temp_v0, (*(s32*)((u8*)(arg1) + 4)) - temp_a1);
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
    if (func_80011C5C((size + 3) >> 2, heap->free) == NULL) {
        func_800110A8(data, heap);
        *(s32 *)(rec + 8) = 0;
        func_80016D44(rec);
    } else {
        func_8002A5A4(rec + 0x10, data);
        func_80029560(rec + 0x10);
        func_8002A35C(rec + 0x10);
    }
    *(s32 *)(rec + 8) = func_80013034(*(s32 *)(rec + 0), 0, *(s32 *)(rec + 0x24), func_80016F34, rec, 1, 0);
}

void func_80016E5C(u8 *arg0, u8 *arg1) {
    Heap *heap;
    s32 n;

    n = func_8002A650(arg0);
    heap = &D_8004E0F0;
    if (func_80011C5C((n + 3) >> 2, heap->free) == NULL) {
        func_800110A8(arg0, heap);
        *(s32 *)(arg1 + 8) = 0;
        func_80016D44(arg1);
    } else {
        arg1 += 0x10;
        func_8002A5A4(arg1, arg0);
        func_80029560(arg1);
        func_8002A35C(arg1);
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
        func_80016928();
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
        func_800110A8(*(void **)(arg0 + 0xC), &D_8004E0F0);
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017164);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017388);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017640);

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", func_800177F4);
#else
void func_800177F4(RelTable *face, POLY_GT4 *poly, s32 arg2, s32 mode) {
    u16 *idx;
    u32 *colors;
    u32 *c;
    s32 semi = mode & 1;
    s32 abr = mode & 0x60;
    u32 code;
    u16 tpage;

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
        code = *(u32 *)&poly->r0 | 0x02000000;
        tpage = (poly->tpage & ~0x60) | 0x20;
    } else {
        if (semi) {
            code = *(u32 *)&poly->r0 | 0x02000000;
            tpage = poly->tpage;
        } else {
            code = *(u32 *)&poly->r0 & ~0x02000000;
            tpage = poly->tpage;
        }
        tpage = (tpage & ~0x60) | abr;
    }
    *(u32 *)&poly->r0 = code; /* code byte: semi-transparency bit */
    poly->tpage = tpage;
    gte_ldrgb(&colors[idx[1] & 0x1FFF]);
    gte_dpcs();
    gte_strgb(&poly->r3);
}
#endif

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001795C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017B08);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017D90);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80017EE8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800180BC);

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

void func_80018388(void *data) {
    NodePool *pool = &D_80056844;
    PoolNode *node;

    node = (PoolNode *)data - 1;
    node->next = pool->done;
    pool->done = node;
    node->state = 2;
}

void *func_800183B0(void) {
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
        func_800110A8(pool->mem, &D_8004E0F0);
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
    func_80017EE8(arg3, data);
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
    func_80017EE8();
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
            func_800110A8((*(s32*)((u8*)(var_s0) + 4)), &D_8004E0F0);
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_800189F4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80018B08);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80018F3C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80019094);

/* Draws the triangle strips of a model: `runs` holds {mode, count} byte
 * pairs ended by count 0xFF, `v` the vertices (vz's high half carries
 * flags: 0x10000 flips the winding test, 0x20000 skips it). Mode bit 0
 * selects primitives whose SXY go at +0x10/+0x18/+0x20 and whose length is
 * preset; otherwise each is a POLY_GT3. The three register variables are
 * the original's: only they give s4/s3/s2 with s0/s1 unused. */
void func_800193A8(POLY_GT3 *poly, SVECTOR *v, u8 *runs) {
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001965C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80019A60);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80019F1C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001A0D8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001A43C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001A6D4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001A82C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001AA48);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001ACC0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001AF2C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001B6D4);

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


void func_8001C378(u8 *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;

    temp_a0 = (*(s32*)((u8*)(arg0) + 144));
    if (temp_a0 != 0) {
        func_80018388(temp_a0);
        (*(s32*)((u8*)(arg0) + 144)) = 0;
    }
    temp_a0_2 = (*(s32*)((u8*)(arg0) + 156));
    if (temp_a0_2 != 0) {
        func_80018388(temp_a0_2);
        (*(s32*)((u8*)(arg0) + 156)) = 0;
    }
}

void func_8001C3D0(u8 *arg0) {
    u8 *temp_v0;
    u16 temp_a0;

    temp_v0 = func_80015A18(*(u16*)(arg0 + 0x74), *(u8**)(*(u8**)(arg0 + 0x6C) + 0xC));
    temp_a0 = *(u16*)(temp_v0 + 8);
    if (temp_a0 == 2 && temp_v0[*(s32*)(temp_v0 + 0x14) + 0x15] == temp_a0) {
        *(void**)(arg0 + 0x54) = func_8001AA48;
        *(void**)(arg0 + 0x58) = func_8001C378;
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
    func_8001A82C(arg0, arg1, arg2);
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

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", func_8001C7FC);
#else
s32 func_8001C7FC(NAS *nas, u8 *arg1, u32 flags) {
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
        return func_8001A0D8(&nas->Type, f | nas->NASid);
    }
}
#endif

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

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001CAD4);

void func_8001CBD4(u32 arg0) {
    func_8002A2EC((arg0 >> 0x10) & 0xFF, (arg0 >> 8) & 0xFF, arg0 & 0xFF);
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
                func_800110A8(slot, &D_8004E0F0);
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
        if (e || blk) {
            e = &set->entries[arg0];
            blk = (u8 *)&e->off + e->off;
        } else {
            e = &set->entries[arg0];
            blk = (u8 *)&e->off + e->off;
        }
        if (!(arg2 & 0x1000000) && !func_8001AF2C(blk)) {
            return;
        }
        {
            p = func_80019094(e, arg2);
            blk += 0x14;
            if (p != NULL) {
                func_800193A8(p, (SVECTOR *)blk, (u8 *)&e->runs + e->runs);
            }
        }
    }
}

void func_8001CEB4(u16 arg0, u8 *arg1, s32 arg2) {
    void *p;
    u8 *blk;
    ModelEntry *e;

    e = (ModelEntry *)func_800159C4(arg0, arg1);
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
        func_800193A8(p, (SVECTOR *)blk, (u8 *)&e->runs + e->runs);
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
        func_800193A8(p, (SVECTOR *)q,(u8 *)&e->runs + e->runs);
    }
    return p;
}

void *func_8001CFE8(MorphModel *model, void *arg1, s32 arg2) {
    void *p;

    p = func_80019094(model, arg2);
    if (p != NULL) {
        func_800193A8(p, arg1, (u8 *)&model->runs + model->runs);
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
        freeList = D_80058ACC;
        D_80058ACC = first;
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
        freeList = D_80058ACC;
        D_80058ACC = first;
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
        freeList = D_80058ACC;
        D_80058ACC = first;
        last->next = freeList;
    }
}

void func_8001D334(subengine *first) {
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001D48C);

void func_8001D5D0(void) {
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
        func_800110A8(D_80058AC8, &D_8004E0F0);
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
    D_80058ACC = sub;
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001D6B4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001D894);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001DAF8);

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
extern void func_80019A60(u16 a, s32 b, s32 c, void *d);
void func_8001DD50(void *arg0, void *arg1) {
    s32 flags;
    s32 x;

    flags = *(s32 *)arg0;
    x = *(s32 *)(*(u8 **)((u8 *)arg0 + 0x6C) + 0xC);
    if ((flags & 0x8000) && x != 0) {
        func_8001D894(arg0, arg1, flags, x);
        func_80019A60(*(u16 *)((u8 *)arg0 + 0x74), x, *(s32 *)arg0, arg0);
    }
}

void func_8001DDB4(s32 *arg0) {
    func_80029100(*arg0 + 0x10);
}

void func_8001DDD8(u8 *arg0) {
    func_80016F54((*(s32*)((u8*)(arg0) + 0)));
    func_800110A8((*(s32*)((u8*)(arg0) + 28)), &D_8004E0F0);
    (*(s32*)((u8*)(arg0) + 28)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001DE18);

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
    func_80013650(arg1[1], func_8001DF74, arg0);
}

void func_8001DF74(u8 *arg0, u8 **arg1) {
    u8 *temp_s0;
    u8 *temp_s1;

    temp_s1 = *arg1;
    func_80011544(arg0, temp_s1 + 0x20);
    temp_s0 = temp_s1 + 0x10;
    func_8002A5A4(temp_s0, (*(s32*)((u8*)(temp_s0) + 16)));
    func_80029560(temp_s0);
    func_8002A35C(temp_s0);
    (*(s32*)((u8*)(temp_s1) + 8)) = func_80013034((*(s32*)((u8*)(temp_s1) + 0)), 0, (*(s32*)((u8*)(temp_s1) + 36)), func_8001DFF8, arg1, 0, 0);
}

void func_8001DFF8(s32 arg0, s32 *arg1) {
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E0A8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E21C);

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

s32 *func_8001E48C(s32 arg0) {
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

s32 func_8001E51C(s32 arg0) {
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

void func_8001E588(u8 *arg0, s32 arg1, s32 arg2) {
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001E610);

void func_8001E824(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 12)) = 2;
    (*(s32*)((u8*)(arg0) + 4)) = (s32) (*(s32*)((u8*)(arg0) + 8));
}

void func_8001E838(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 4)) = arg1;
    (*(s32*)((u8*)(arg0) + 12)) = 1;
}

void func_8001E848(u8 *arg0, s32 arg1) {
    (*(s32*)((u8*)(arg0) + 4)) = arg1;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
}

/* arg0 points to a table of handlers; calls handler 3 if there is one. */
void func_8001E854(void *arg0) {
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
void func_8001E8F0(void *arg0) {
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
    D_80058B74 = (void *)D_80058B6C;
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001EAA4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001EDFC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001F0D4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001F4F8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001F604);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001F828);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001FCDC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8001FE80);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020064);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800201BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020344);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020680);

void func_80020B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    func_80020680(a0, a1, (a4 << 12) / a3, a2, a5);
}

void func_80020BA8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    func_80020344(a0, a1, (a4 << 12) / a3, a2);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020C04);

s16 func_80020C4C(s32 arg0) {
    if (arg0 & 1) {
        return (D_80068BD4[((arg0 >> 1) - 0x400) & 0xFFF].unk2 + D_80068BD4[((arg0 >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    }
    return D_80068BD4[((arg0 >> 1) - 0x400) & 0xFFF].unk2;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020CC4);

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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80020F5C);

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

INCLUDE_ASM("asm/nonmatchings/main/main", func_800210F8);

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
        return func_80015304(arg0, arg1, arg2, arg5);
    }
    if (arg3 < arg5) {
        if (arg4 < arg5) {
            return arg0;
        }
        return func_80015304(arg1, arg0, arg4 - arg3, arg5 - arg3);
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
        func_800110A8(temp_a0_2, &D_8004E0F0);
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021604);

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

void func_80021770(u8 *arg0, s32 *arg1) {
    func_80019F1C(arg0 + 8, *arg1, (*(s32*)((u8*)(arg0) + 8)));
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021798);

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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80021A1C);

void func_80021AF0(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4) {
    arg2[0] = func_800152A8(arg0[0], arg1[0], arg3, arg4);
    arg2[1] = func_800152A8(arg0[1], arg1[1], arg3, arg4);
    arg2[2] = func_800152A8(arg0[2], arg1[2], arg3, arg4);
}

void func_80021B88(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4) {
    arg2[0] = func_80015304(arg0[0], arg1[0], arg3, arg4);
    arg2[1] = func_80015304(arg0[1], arg1[1], arg3, arg4);
    arg2[2] = func_80015304(arg0[2], arg1[2], arg3, arg4);
}

void func_80021C20(u8 *arg0, u8 *arg1, u8 *arg2, s32 arg3, s32 arg4) {
    (*(s32*)((u8*)(arg2) + 0)) = func_80015304((*(s32*)((u8*)(arg0) + 0)), (*(s32*)((u8*)(arg1) + 0)), arg3, arg4);
    (*(s32*)((u8*)(arg2) + 4)) = func_80015304((*(s32*)((u8*)(arg0) + 4)), (*(s32*)((u8*)(arg1) + 4)), arg3, arg4);
    (*(s32*)((u8*)(arg2) + 8)) = func_80015304((*(s32*)((u8*)(arg0) + 8)), (*(s32*)((u8*)(arg1) + 8)), arg3, arg4);
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

u8 *func_80021E60(u8 *arg0, s32 arg1) {
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
    temp_v1_3 = D_80058B74;
    D_80058B74 = arg0;
    (*(s32*)((u8*)(arg0) + 116)) = temp_v1_3;
}

u8 *func_80021F6C(void) {
    u8 *temp_v0;
    u8 *temp_v1;

    temp_v1 = D_80058B74;
    if (temp_v1 != NULL) {
        D_80058B74 = (*(s32*)((u8*)(temp_v1) + 116));
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
        func_800110A8(D_80058B6C, &D_8004E0F0);
    }
    if (D_80058B68 != 0) {
        func_800110A8(D_80058B68, &D_8004E0F0);
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

void func_80022070(void) {
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

void func_800220E8(void) {
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

INCLUDE_ASM("asm/nonmatchings/main/main", func_80022140);

void func_800222BC(void) {
    func_8003347C();
}

void func_800222DC(void) {
    func_8003346C();
}

void func_800222FC(void) {
    SsInit();
    func_8002C060();
    func_80022984();
}

void func_8002232C(void) {
    func_8002BBEC();
    SsInit();
    func_8002C44C();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002235C);

void func_80022504(void) {
    _sfx_handler *next;
    _sfx_handler *v;

    v = SfxVoicePool.active;
    for (;;) {
        if (v == NULL) {
            return;
        }
        next = v->next;
        if (v->owner != NULL) {
            *v->owner = 0;
        }
        func_8002B8F4(&v->unk2C);
        if (v->next != NULL) {
            v->next->prev = v->prev;
        }
        if (v->prev != NULL) {
            v->prev->next = v->next;
        } else {
            SfxVoicePool.active = v->next;
        }
        v->next = SfxVoicePool.free;
        SfxVoicePool.free = v;
        v = next;
    }
}

void StopSfx(_sfx_handler *voice) {
    if (voice != NULL) {
        if (voice->owner != NULL) {
            *voice->owner = 0;
        }
        func_8002B8F4(&voice->unk2C);
        if (voice->next != NULL) {
            voice->next->prev = voice->prev;
        }
        if (voice->prev != NULL) {
            voice->prev->next = voice->next;
        } else {
            SfxVoicePool.active = voice->next;
        }
        voice->next = SfxVoicePool.free;
        SfxVoicePool.free = voice;
    }
}

_sfx_handler *PlaySfx(s32 id, _sfx_handler **handle, SLONG_XYZ *pos, s32 arg3, s32 arg4, s32 flags) {
    _sfx_handler *voice;
    _sfx_handler *v;

    voice = NULL;
    if (id & 0x8000) {
        func_80027A74();
    } else {
        if (SfxTable[id][1] == -1) {
            return NULL;
        }
        v = SfxVoicePool.free;
        if (v != NULL) {
            SfxVoicePool.free = v->next;
            v->next = SfxVoicePool.active;
            v->prev = NULL;
            if (SfxVoicePool.active != NULL) {
                SfxVoicePool.active->prev = v;
            }
            voice = v;
            voice->owner = NULL;
            voice->unk28 = -1;
            voice->unk2C = -1;
        }
        if (voice != NULL) {
            voice->next = SfxVoicePool.active;
            voice->prev = NULL;
            SfxVoicePool.active = voice;
            voice->id = id;
            voice->unk20 = arg3;
            voice->unk24 = arg4;
            voice->flags = flags;
            voice->unk28 = -1;
            if (handle != NULL) {
                if (*handle != NULL) {
                    StopSfx(*handle);
                }
                *handle = voice;
                voice->owner = handle;
                voice->pos = pos;
            } else if (pos != NULL) {
                voice->posCopy = *pos;
                voice->pos = &voice->posCopy;
            } else {
                voice->pos = NULL;
            }
        }
    }
    return voice;
}

void func_800227C0(void) {
    _sfx_handler *voice;

    voice = SfxVoicePool.active;
    if (voice != NULL) {
        do {
            voice->flags |= 4;
            voice = voice->next;
        } while (voice != NULL);
    }
}

void func_800227FC(void) {
    _sfx_handler *voice;

    voice = SfxVoicePool.active;
    if (voice != NULL) {
        do {
            voice->flags |= 2;
            voice = voice->next;
        } while (voice != NULL);
    }
}

void func_80022838(void) {
    _sfx_handler *next;
    _sfx_handler *voice;

    func_8002AEE0();
    voice = SfxVoicePool.active;
    if (voice != NULL) {
        do {
            next = voice->next;
            if (func_8002235C(voice) != 1) {
                StopSfx(voice);
            }
            voice = next;
        } while (voice != NULL);
    }
}

void func_800228A4(_sfx_handler *voice, s32 arg1) {
    if (voice != NULL) {
        voice->unk24 = arg1;
        voice->flags |= 1;
    }
}

void func_800228C4(_sfx_handler *voice) {
    if (voice->next != NULL) {
        voice->next->prev = voice->prev;
    }
    if (voice->prev != NULL) {
        voice->prev->next = voice->next;
    } else {
        SfxVoicePool.active = voice->next;
    }
    voice->next = SfxVoicePool.free;
    SfxVoicePool.free = voice;
}

_sfx_handler *func_80022924(void) {
    _sfx_handler *v;
    _sfx_handler *r;

    v = SfxVoicePool.free;
    if (v == NULL) {
        return NULL;
    }
    SfxVoicePool.free = v->next;
    v->next = SfxVoicePool.active;
    v->prev = NULL;
    if (SfxVoicePool.active != NULL) {
        SfxVoicePool.active->prev = v;
    }
    r = v;
    r->owner = NULL;
    r->unk28 = -1;
    r->unk2C = -1;
    return r;
}

void func_80022984(void) {
    _sfx_handler *v;
    s32 i;

    v = SfxVoicePool.voices;
    for (i = 22; i >= 0; i--) {
        v->next = v + 1;
        v++;
    }
    v->next = NULL;
    SfxVoicePool.active = NULL;
    SfxVoicePool.free = SfxVoicePool.voices;
    func_8002B9A4();
}


void func_800229D4(void) {
    func_8002BBB4();
    func_80022984();
}

void func_800229FC(s32 i) {
    func_8002C488(D_80058B88[i].file, D_80058B88[i].reverb);
}

static inline NAS *AllocNAS(void) {
    NAS *free;
    NAS *next;

    free = NASFreeList;
    if (free == NULL) {
        return NULL;
    }
    next = free->Tag.Child;
    NASFreeList = next;
    if (next != NULL) {
        next->Tag.Parent = NULL;
    }
    return free;
}

NAS *SpawnNASById(s32 x, s32 y, s32 z, u16 id, u16 flags) {
    NAS *nas;
    EXTengine *eng;
    s32 i;
    s32 r;
    s32 want;
    u16 ast;

    nas = NULL;
    want = id;
    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == want) {
            r = i;
            goto found;
        }
    }
    r = 0;
found:
    ast = r;
    if (ast != 0 && ast < ASTCount) {
        nas = AllocNAS();
        if (nas != NULL) {
            *nas = NASTemplate;
            nas->NASid = 1;
            if (NASActiveTail != NULL) {
                NASActiveTail->Tag.Child = nas;
                nas->Tag.Parent = NASActiveTail;
                NASActiveTail = nas;
            } else {
                NASActiveTail = nas;
                NASActiveHead = nas;
            }
            nas->Type = ast;
            nas->InitDelete = ASTTable[ast].Init;
            nas->Handler = ASTTable[ast].Plot;
            if (flags & 1) {
                eng = AddEngine(nas, 1);
                if (eng != NULL) {
                    eng->unk10.x = x;
                    eng->unk10.y = y;
                    eng->unk10.z = z;
                } else {
                    if (flags & 0x10) {
                        KillNAS(nas);
                    }
                    return NULL;
                }
            }
            nas->InitDelete(nas);
            if (nas->NASid == 0) {
                nas = NULL;
            }
        }
    }
    return nas;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80022C3C);

NAS *SpawnNASFromRecord(NASTrigger *trig) {
    NAS *nas;
    EXTengine *eng;
    s32 i;
    s32 type;
    u8 dir;

    nas = NULL;
    type = trig->Type;
    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == type) {
            goto found;
        }
    }
    i = 0;
found:
    if (i > 0 && i < ASTCount) {
        nas = AllocNAS();
        if (nas != NULL) {
            *nas = NASTemplate;
            nas->NASid = 1;
            if (NASActiveTail != NULL) {
                NASActiveTail->Tag.Child = nas;
                nas->Tag.Parent = NASActiveTail;
                NASActiveTail = nas;
            } else {
                NASActiveTail = nas;
                NASActiveHead = nas;
            }
            nas->Type = i;
            nas->trigger = trig;
            trig->Status |= 1;
            nas->InitDelete = ASTTable[i].Init;
            nas->Handler = ASTTable[i].Plot;
            eng = AddEngine(nas, 1);
            if (eng != NULL) {
                eng->unk10.x = trig->TrigXYZ.x;
                eng->unk10.y = trig->TrigXYZ.y;
                eng->unk10.z = trig->TrigXYZ.z;
                dir = 0;
                if (trig->Status & 0x20) {
                    dir = 0xFF;
                }
                eng->direction = dir;
            } else {
                KillNAS(nas);
                return NULL;
            }
            nas->InitDelete(nas);
            if (nas->NASid == 0) {
                nas = NULL;
            }
        }
    }
    return nas;
}


s32 KillNAS(NAS *nas) {
    EXTengine *eng;

    if (!(nas->NASid & 1)) {
        return 1;
    }
    if (nas->trigger != NULL && (nas->trigger->Status & 0x10)) {
        return 0;
    }
    if (nas->Class & 4) {
        nas->InitDelete(nas);
    }
    if (nas->trigger != NULL) {
        nas->trigger->Status &= ~1;
    }
    eng = nas->Engine;
    for (;;) {
        if (eng == NULL) {
            goto freed;
        }
        if (eng->platform != NULL) {
            eng->platform->platform = NULL;
        }
        eng->platform = NULL;
        func_8001C270(eng);
        eng = eng->Engine;
    }
freed:
    func_80026430(nas);
    func_80026BF4((u8 *)nas);
    func_80026AFC(nas);
    func_80026980(nas);
    func_80026A54(nas);
    func_800268BC(nas);
    if (nas->Tag.Parent != NULL) {
        nas->Tag.Parent->Tag.Child = nas->Tag.Child;
        if (nas->Tag.Child != NULL) {
            nas->Tag.Child->Tag.Parent = nas->Tag.Parent;
        } else {
            NASActiveTail = nas->Tag.Parent;
        }
    }
    nas->NASid = 0;
    if (NASFreeList != NULL) {
        NASFreeList->Tag.Parent = nas;
        nas->Tag.Child = NASFreeList;
        NASFreeList = nas;
    } else {
        NASFreeList = nas;
        nas->Tag.Child = NULL;
    }
    nas->Tag.Parent = NULL;
    return 1;
}

void func_80023158(s32 arg0) {
    s32 *base;
    s32 *hd;
    u32 size;
    u32 words;

    s32 *pb = &D_8006EA00;
    u8 *res;

    size = (arg0 + 3) & ~3;
    D_8006EA04 = size;
    res = func_80011654(size, &D_8004E0F0);
    *pb = (s32)res;
    if (res != NULL) {
        base = (s32 *)res;
        hd = D_8005A7A0;
        hd[0] = 0;
        hd[1] = (s32)base;
        hd[2] = 0;
        hd[3] = (s32)base;
        hd[4] = size | 2;
        hd[5] = 0;
        hd[6] = (s32)base;
        hd[7] = (s32)base + size;
        size -= 0x18;
        base[0] = (s32)hd;
        base[1] = 0;
        base[2] = (s32)hd;
        base[3] = 0;
        base[4] = size | 1;
        base[5] = 0;
        size = (size - 0x18) >> 2;
        base += 6;
        do {
            *base = 0;
            size--;
            base++;
        } while (size != 0);
    }
}

void func_8002321C(void) {
    s32 *base;
    s32 *hd;
    u32 size;
    u32 flag;
    u32 words;

    size = D_8006EA04;
    if (D_8006EA00 != 0) {
        base = (s32 *)D_8006EA00;
        flag = size | 2;
        words = size >> 2;
        size -= 0x18;
        hd = D_8005A7A0;
        hd[0] = 0;
        hd[1] = (s32)base;
        hd[2] = 0;
        hd[3] = (s32)base;
        hd[4] = flag;
        hd[5] = 0;
        hd[6] = (s32)base;
        hd[7] = (s32)base + words * 4;
        base[0] = (s32)hd;
        base[1] = 0;
        base[2] = (s32)hd;
        base[3] = 0;
        base[4] = size | 1;
        base[5] = 0;
        size = (size - 0x18) >> 2;
        base += 6;
        do {
            *base = 0;
            size--;
            base++;
        } while (size != 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", AllocExtension);

INCLUDE_ASM("asm/nonmatchings/main/main", FreeExtension);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800235BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800237BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80023A90);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024008);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024214);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800243A0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800246D0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_800248A0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024AD8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024E30);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80024FA4);

s16 func_800250B0(NAS *nas) {
    EXTengine *eng;
    EXTanimator *anim;
    subengine *sub;
    s32 id;
    s32 kind;
    s32 want;

    if ((nas->NASid & 0x500) == 0x500 && (eng = nas->Engine) != NULL) {
        id = 0x400;
        want = 0;
        for (anim = (EXTanimator *)nas->Extend.Child; anim != NULL; anim = (EXTanimator *)anim->Extend.Child) {
            if (anim->EXTid & id) {
                if (want == 0) {
                    goto found;
                }
                if ((anim->EXTid & 0xFF) == want) {
                    goto found;
                }
            }
        }
        anim = NULL;
    found:
        if (anim != NULL) {
            kind = anim->EXTid & 0xFF;
            if (anim->Engine != NULL) {
                eng = anim->Engine;
            }
            sub = anim->sub;
            if (sub == NULL) {
                sub = eng->sub;
            }
            switch (kind) {
            case 1:
                return func_800251A8(nas, eng, sub, anim);
            case 2:
                return func_80025534(nas, eng, sub, anim);
            }
        }
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800251A8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80025534);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002578C);

FlagRecord *func_80025A48(EXTengine *eng, u32 flag) {
    PointList *list;
    FlagRecord *rec;
    s32 n;

    if (eng->collision_box.flags & flag) {
        return &eng->collision_box;
    }
    if (eng->sub != NULL && (list = func_8001562C(eng->sub)) != NULL) {
        n = list->nRecs;
        rec = (FlagRecord *)&list->pts[list->count];
        for (; n != 0; n--, rec++) {
            if (rec->flags & flag) {
                return rec;
            }
        }
    }
    return NULL;
}

void func_80025AE4(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6) {
    (*(s16*)((u8*)(arg0) + 132)) = arg1;
    (*(s16*)((u8*)(arg0) + 134)) = arg2;
    (*(s16*)((u8*)(arg0) + 136)) = arg3;
    (*(s16*)((u8*)(arg0) + 142)) = 0;
    (*(s16*)((u8*)(arg0) + 138)) = (s16) arg4;
    (*(s16*)((u8*)(arg0) + 140)) = (s16) arg5;
    (*(s32*)((u8*)(arg0) + 144)) = arg6;
}

void func_80025B10(EXTengine *eng, u32 flag) {
    PointList *list;
    FlagRecord *rec;
    s32 n;

    eng->collision_box.flags = 0;
    if (eng->sub != NULL && (list = func_8001562C(eng->sub)) != NULL) {
        n = list->nRecs;
        rec = (FlagRecord *)&list->pts[list->count];
        for (; n != 0; n--, rec++) {
            if (rec->flags & flag) {
                eng->collision_box = *rec;
                return;
            }
        }
    }
}

s32 func_80025BBC(EXTengine *eng) {
    s32 dy;

    func_80032F3C(eng->unk10.y - eng->OldXYZ.y);
    func_80032F3C(eng->unk10.x - eng->OldXYZ.x);
    func_80032F3C(eng->unk10.z - eng->OldXYZ.z);
    if (D_8005AA94 != NULL) {
        dy = D_8005AA94(eng);
    } else {
        dy = func_80025E00(eng, eng->unk10.y - D_8005A5D8);
    }
    D_8005B5B8 = dy;
    if (dy == 0x7FFFFFFF) {
        return 0;
    }
    if (dy < 0) {
        return 0;
    }
    eng->unk10.y -= dy;
    return 1;
}

s32 func_80025C80(EXTengine *eng) {
    s32 lim;
    s32 dy;

    lim = func_80032F3C(eng->unk10.x - eng->OldXYZ.x);
    if (lim < 0x10) {
        lim = 4;
    }
    if (D_8005AA94 != NULL) {
        dy = D_8005AA94(eng);
    } else {
        dy = func_80025E00(eng, eng->unk10.y - D_8005A5D8);
    }
    if (dy == 0x7FFFFFFF) {
        return 0;
    }
    if (dy >= 0) {
        return 0;
    }
    lim = ~lim + 1;
    if (dy < lim) {
        return 0;
    }
    eng->unk10.y += dy;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80025D40);

s32 func_80025E00(EXTengine *eng, s32 y) {
    NAS *nas;
    EXTengine *other;
    FlagRecord *rec;
    s32 dx;
    s32 dz;
    s32 r;
    s32 top;
    s32 dy;

    nas = D_8005AA2C;
    D_8005AA9C = NULL;
    if (nas != NULL) {
        do {
            if (nas->Class & 0x100) {
                other = nas->Engine;
                if (other != NULL && other != eng) {
                    rec = func_80025A48(other, 0x10);
                    if (rec != NULL) {
                        dx = other->unk10.x - eng->unk10.x;
                        dz = other->unk10.z - eng->unk10.z;
                        r = rec->radius + 0x40;
                        r *= r;
                        top = other->unk10.y + rec->height;
                        if (dx * dx + dz * dz < r) {
                            dy = eng->unk10.y - top;
                            if (func_80032F3C(dy) < func_80032F3C(y)) {
                                y = dy;
                                D_8005AA9C = nas;
                            }
                        }
                    }
                }
            }
            nas = nas->Solid.Child;
        } while (nas != NULL);
    }
    return y;
}

void func_80025F44(EXTengine *eng) {
    if (D_8005AA94 == NULL) {
        func_80025E00(eng, eng->unk10.y - D_8005A5D8);
    } else {
        D_8005AA94(eng);
    }
}

void func_80025F8C(void) {
}

void func_80025F94(void) {
}

void func_80025F9C(void) {
}

void func_80025FA4(void) {
}

void func_80025FAC(EXTanimator *anim, s16 n) {
    switch (anim->EXTid & 0xFF) {
    case 1:
        if (anim->Alist != NULL) {
            anim->ALindex = n;
            anim->Atable = anim->Alist[n];
        }
        anim->Aindex = 0;
        anim->Aspeed = -1;
        anim->Seq_count = 0;
        break;
    case 2:
        if (anim->Alist != NULL) {
            anim->ALindex = n;
            anim->Atable = anim->Alist[n];
        }
        anim->Aindex = -1;
        anim->Aspeed = -1;
        anim->Seq_count = 0;
        break;
    default:
        anim->Seq_count = 0;
        break;
    }
    anim->Seq_count_down = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026048);

void func_8002617C(void) {
    s32 i;
    NASTrigger *trig;

    trig = D_8005AA48;
    if (D_8005AA84 != NULL) {
        D_8005AA84();
    }
    for (i = 0; i < D_8005AA4C; i++, trig++) {
        if (!(trig->Status & 0x8001)) {
            SpawnNASFromRecord(trig);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80026210);

void func_80026308(void) {
    func_80023A90();
    D_8005B2B4 = 0;
    D_8005B63C = D_80055684;
}

s32 func_8002633C(u8 *s) {
    s32 sum;
    s32 c;
    s32 d;
    s32 e;

    c = *s;
    d = c & 0xFF;
    sum = 0;
    if (d != 0 && d != 10) {
        do {
            s32 idx;
            u8 *f;
            s8 *w;
            s32 v;
            if (c == 13) {
                break;
            }
            idx = *s + (D_8005B2B4 << 8);
            w = &D_8005AEB4[idx];
            f = &D_8005B0B4[idx];
            v = *w;
            if (!(*f & 2)) {
                sum += v;
            }
            s++;
            c = *s;
            e = c & 0xFF;
        } while (e != 0 && e != 10);
    }
    return sum;
}

IdEntry *func_800263D8(IdEntry *p, s32 id) {
    if (p != NULL) {
        do {
            if (p->id == id) {
                return p;
            }
            if (p->id == 0x3E7) {
                return NULL;
            }
            p++;
        } while (p != NULL);
    }
    return NULL;
}

void func_80026418(void) {
}

void func_80026420(void) {
}

void func_80026428(void) {
}

void func_80026430(NAS *nas) {
    EXT *ext;

    ext = (EXT *)nas;
    while (ext->Extend.Child != NULL) {
        ext = ext->Extend.Child;
    }
    while (ext->Extend.Parent != NULL) {
        FreeExtension(ext);
        ext = ext->Extend.Parent;
    }
    nas->NASid = *(u8 *)nas;
}

void func_800264B4(void) {
    func_800110A8(D_8006EA00, &D_8004E0F0);
    D_8006EA00 = 0;
}

void *FindExtension(NAS *nas, s32 id) {
    EXT *ext;
    s32 sub;

    sub = id & 0xFF;
    id &= ~0xFF;
    for (ext = nas->Extend.Child; ext != NULL; ext = ext->Extend.Child) {
        if (ext->EXTid & id) {
            if (sub == 0) {
            found:
                return ext;
            }
            if ((ext->EXTid & 0xFF) == sub) {
                goto found;
            }
        }
    }
    return NULL;
}

EXTmotion *AddMotion(NAS *nas) {
    EXTmotion *mot;

    mot = AllocExtension(nas, 0x34, 0x800, &D_8005A9CC);
    if (mot != NULL) {
        nas->NASid |= 0x800;
        *mot = D_8005A9CC;
    }
    return mot;
}

Ext400 *func_800265DC(NAS *nas) {
    Ext400 *ext;

    ext = AllocExtension(nas, 0x2C, 0x400, &D_8005A9A0);
    if (ext != NULL) {
        nas->NASid |= 0x400;
        *ext = D_8005A9A0;
    }
    return ext;
}

Ext1000 *AddObject(NAS *nas) {
    Ext1000 *ext;

    ext = AllocExtension(nas, 0x2C, 0x1000, &D_8005A974);
    if (ext != NULL) {
        nas->NASid |= 0x1000;
        *ext = D_8005A974;
    }
    return ext;
}

EXTengine *AddEngine(NAS *nas, s32 id) {
    EXTengine *eng;
    EXTengine *prev;
    EXTengine *ext;

    eng = AllocExtension(nas, 0x9C, id | 0x100, &EngineTemplate);
    if (eng != NULL) {
        nas->NASid |= 0x100;
        *eng = EngineTemplate;
        if (func_8001D390(eng, 1) != 0) {
            prev = nas->Engine;
            if (prev == NULL) {
                nas->Engine = eng;
            } else {
                for (ext = (EXTengine *)prev->Extend.Child; ext != NULL; ext = (EXTengine *)ext->Extend.Child) {
                    if (ext->EXTid & 0x100) {
                        prev->Engine = ext;
                        prev = ext;
                    }
                }
            }
        } else {
            FreeExtension(eng);
            eng = NULL;
        }
    }
    return eng;
}

void func_80026858(void *owner, s32 size, s32 id, Words3 *tmpl) {
    Words3 *ext;

    ext = AllocExtension(owner, size, id, tmpl);
    if (ext != NULL) {
        *ext = *tmpl;
    }
}

void *AddExtension(void *owner, s32 size, s32 id) {
    return AllocExtension(owner, size, id, NULL);
}

void func_800268BC(NAS *nas) {
    NAS **head = (NAS **)&D_8005AA2C;

    if (nas->Solid.Parent != NULL || nas->Solid.Child != NULL) {
        if (nas->Solid.Parent != NULL) {
            nas->Solid.Parent->Solid.Child = nas->Solid.Child;
            if (nas->Solid.Child != NULL) {
                nas->Solid.Child->Solid.Parent = nas->Solid.Parent;
            }
        } else {
            *head = nas->Solid.Child;
            nas->Solid.Child->Solid.Parent = NULL;
        }
        nas->Solid.Parent = NULL;
        nas->Solid.Child = NULL;
        nas->Class &= ~0x300;
    } else if (*head == nas) {
        *head = NULL;
    }
}

void func_8002694C(NAS *nas, s32 bits) {
    if (D_8005AA2C != NULL) {
        nas->Solid.Child = D_8005AA2C;
        D_8005AA2C->Solid.Parent = nas;
    }
    D_8005AA2C = nas;
    nas->Class = (nas->Class & ~0x300) | bits;
}

void func_80026980(NAS *nas) {
    NAS **head = (NAS **)&D_8005AA34;

    if (nas->Camera.Parent != NULL || nas->Camera.Child != NULL) {
        if (nas->Camera.Parent != NULL) {
            nas->Camera.Parent->Camera.Child = nas->Camera.Child;
            if (nas->Camera.Child != NULL) {
                nas->Camera.Child->Camera.Parent = nas->Camera.Parent;
            }
        } else {
            *head = nas->Camera.Child;
            nas->Camera.Child->Camera.Parent = NULL;
        }
        nas->Camera.Parent = NULL;
        nas->Camera.Child = NULL;
    } else if (*head == nas) {
        *head = NULL;
    }
}

void func_80026A04(u8 *arg0) {
    if (D_8005AA34 != NULL) {
        (*(s32*)((u8*)(arg0) + 60)) = (u8 *) D_8005AA34;
        (*(s32*)((u8*)(D_8005AA34) + 56)) = arg0;
    }
    D_8005AA34 = arg0;
}

s32 func_80026A28(void) {
    NAS *nas;
    s32 n;

    n = 0;
    for (nas = D_8005AA24; nas != NULL; nas = nas->Object.Child) {
        n++;
    }
    return n;
}

void func_80026A54(NAS *nas) {
    NAS **head = (NAS **)&D_8005AA24;

    if (nas->Object.Parent != NULL || nas->Object.Child != NULL) {
        if (nas->Object.Parent != NULL) {
            nas->Object.Parent->Object.Child = nas->Object.Child;
            if (nas->Object.Child != NULL) {
                nas->Object.Child->Object.Parent = nas->Object.Parent;
            }
        } else {
            *head = nas->Object.Child;
            nas->Object.Child->Object.Parent = NULL;
        }
        nas->Object.Parent = NULL;
        nas->Object.Child = NULL;
    } else if (*head == nas) {
        *head = NULL;
    }
}

void func_80026AD8(u8 *arg0) {
    if (D_8005AA24 != NULL) {
        (*(s32*)((u8*)(arg0) + 44)) = (u8 *) D_8005AA24;
        (*(s32*)((u8*)(D_8005AA24) + 40)) = arg0;
    }
    D_8005AA24 = arg0;
}

void func_80026AFC(NAS *nas) {
    NAS **head = nas->Group.Head;

    if (head != NULL) {
        if (nas->Group.Parent != NULL) {
            nas->Group.Parent->Group.Child = nas->Group.Child;
            if (nas->Group.Child != NULL) {
                nas->Group.Child->Group.Parent = nas->Group.Parent;
            }
        } else {
            NAS *next = nas->Group.Child;
            *head = next;
            if (next != NULL) {
                nas->Group.Child->Group.Parent = NULL;
            }
        }
    }
    nas->Group.Parent = NULL;
    nas->Group.Child = NULL;
    nas->Group.Head = NULL;
}

void func_80026B70(u8 **arg0, u8 *arg1) {
    u8 *temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 != NULL) {
        (*(s32*)((u8*)(arg1) + 32)) = temp_v0;
        (*(s32*)((u8*)(temp_v0) + 28)) = arg1;
    }
    *arg0 = arg1;
    (*(s32*)((u8*)(arg1) + 36)) = arg0;
}

void func_80026B94(NAS *list, NAS *nas) {
    if (nas->Family.Parent != NULL) {
        nas->Family.Parent->Family.Child = nas->Family.Child;
    }
    if (nas->Family.Child != NULL) {
        nas->Family.Child->Family.Parent = nas->Family.Parent;
    }
    while (list->Family.Child != NULL) {
        list = list->Family.Child;
    }
    list->Family.Child = nas;
    nas->Family.Parent = list;
    nas->Family.Child = NULL;
}

void func_80026BF4(u8 *arg0) {
    u8 *temp_v1;
    u8 *temp_v1_2;

    temp_v1 = (*(s32*)((u8*)(arg0) + 20));
    if (temp_v1 != NULL) {
        (*(s32*)((u8*)(temp_v1) + 24)) = (u8 *) (*(s32*)((u8*)(arg0) + 24));
    }
    temp_v1_2 = (*(s32*)((u8*)(arg0) + 24));
    if (temp_v1_2 != NULL) {
        (*(s32*)((u8*)(temp_v1_2) + 20)) = (u8 *) (*(s32*)((u8*)(arg0) + 20));
    }
    (*(s32*)((u8*)(arg0) + 20)) = NULL;
    (*(s32*)((u8*)(arg0) + 24)) = NULL;
}

void func_80026C38(u8 *arg0, u8 *arg1) {
    u8 *var_a0;

    var_a0 = arg0;
    if ((*(s32*)((u8*)(var_a0) + 24)) != 0) {
        do {
            var_a0 = (*(s32*)((u8*)(var_a0) + 24));
        } while ((*(s32*)((u8*)(var_a0) + 24)) != 0);
    }
    (*(s32*)((u8*)(var_a0) + 24)) = arg1;
    (*(s32*)((u8*)(arg1) + 20)) = var_a0;
}

void func_80026C6C(u8 *arg0, u8 *arg1) {
    u8 *temp_v0;

    temp_v0 = (*(s32*)((u8*)(arg1) + 12));
    if (temp_v0 != NULL) {
        (*(s32*)((u8*)(temp_v0) + 16)) = arg0;
        (*(s32*)((u8*)(arg0) + 12)) = temp_v0;
        (*(s32*)((u8*)(arg0) + 16)) = arg1;
        (*(s32*)((u8*)(arg1) + 12)) = arg0;
    }
}

void func_80026C94(u8 *arg0, u8 *arg1) {
    u8 *temp_a2;
    u8 *temp_v1;
    u8 *temp_v1_2;

    temp_a2 = (*(s32*)((u8*)(arg1) + 12));
    if (temp_a2 != NULL) {
        temp_v1 = (*(s32*)((u8*)(arg0) + 12));
        if (temp_v1 != NULL) {
            (*(s32*)((u8*)(temp_v1) + 16)) = (u8 *) (*(s32*)((u8*)(arg0) + 16));
            temp_v1_2 = (*(s32*)((u8*)(arg0) + 16));
            if (temp_v1_2 != NULL) {
                (*(s32*)((u8*)(temp_v1_2) + 12)) = (u8 *) (*(s32*)((u8*)(arg0) + 12));
            } else {
                NASActiveTail = (*(s32*)((u8*)(arg0) + 12));
            }
        }
        (*(s32*)((u8*)(temp_a2) + 16)) = arg0;
        (*(s32*)((u8*)(arg0) + 12)) = temp_a2;
        (*(s32*)((u8*)(arg0) + 16)) = arg1;
        (*(s32*)((u8*)(arg1) + 12)) = arg0;
    }
}

void func_80026D00(void) {
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/main", func_80026D08);
#else
static inline void FreeNAS(NAS *nas) {
    if (NASFreeList != NULL) {
        NASFreeList->Tag.Parent = nas;
        nas->Tag.Child = NASFreeList;
        NASFreeList = nas;
    } else {
        NASFreeList = nas;
        nas->Tag.Child = NULL;
    }
    nas->Tag.Parent = NULL;
}

void func_80026D08(void) {
    NAS *nas;
    s32 i;

    NASFreeList = NULL;
    i = D_8005AA12 - 1;
    nas = &D_8005AA1C[i];
loop:
    if (i >= D_8005AA10) {
        if (nas->NASid == 0) {
            FreeNAS(nas);
        }
        nas--;
        i--;
        goto loop;
    }
}
#endif

void func_80026D9C(void) {
    s32 i;

    D_8005AA34 = NULL;
    D_8005AA2C = NULL;
    D_8005AA30 = NULL;
    D_8005AA24 = NULL;
    D_8005AA28 = NULL;
    NASActiveHead = NULL;
    NASActiveTail = NULL;
    D_8005A5D8 = 0;
    D_8005A5DC = 1;
    func_80026D08();
    for (i = 1; i < 16; i++) {
        D_8005B5BC[i][0] = 0;
        D_8005B5BC[i][1] = 0;
    }
}


void func_80026E2C(void) {
    s32 i;

    i = D_8005AA10;
    for (;;) {
        if (i < D_8005AA14) {
            D_8005AA1C[i] = NASTemplate;
            i++;
        } else {
            break;
        }
    }
    func_80026D08();
}

void func_80026ECC(void) {
    s32 i;

    i = 0;
    for (;;) {
        if (i >= D_8005AA14) {
            return;
        }
        D_8005AA1C[i] = NASTemplate;
        i++;
    }
}

void func_80026F48(void) {
    s32 i;

    for (i = D_8005AA10; i < D_8005AA14; i++) {
        KillNAS(&D_8005AA1C[i]);
    }
}

void func_80026FCC(void) {
    s32 i;

    for (i = 0; i < D_8005AA14; i++) {
        KillNAS(&D_8005AA1C[i]);
    }
}

void func_80027040(s32 arg0) {
    u8 *temp_s0;
    u8 *var_a0;

    var_a0 = NASActiveHead;
    if (var_a0 != NULL) {
        do {
            temp_s0 = (*(s32*)((u8*)(var_a0) + 16));
            if (((*(s32*)((u8*)(var_a0) + 0)) & 1) && ((*(u16*)((u8*)(var_a0) + 108)) == (arg0 & 0xFFFF))) {
                KillNAS(var_a0);
            }
            var_a0 = temp_s0;
        } while (var_a0 != NULL);
    }
}

void func_800270B0(NAS *nas) {
    if (nas->trigger != NULL) {
        nas->trigger->Status |= 0x8000;
    }
    KillNAS(nas);
}

void func_800270F0(u8 *a) {
    VTable8 *p;

    p = (VTable8 *)a;
    while (p != NULL) {
        D_8005B648 = p;
        p->unk0();
        while (D_8005B648 == p) {
            p->unk4();
            p->unk8();
        }
        p = D_8005B648;
    }
}

s32 func_8002718C(void) {
    __main();
    ResetCallback();
    ResetGraph(0);
    func_8002D488();
    SetGraphDebug(0);
    InitGeom();
    SsInit();
    D_800637A8 = 0;
    func_80027F00();
    func_8003351C(D_800637A8);
    DrawSync(0);
    ClearImage2(&D_8005B640, 0, 0, 0);
    DrawSync(0);
    func_8002AABC();
    func_80028C94();
    func_80010158();
    func_80027908();
    func_800153A8();
    func_8002AB44();
    StopCallback();
    PadStopCom();
    SetDispMask(0);
    ResetGraph(3);
    return 0;
}

void func_8002726C(void) {
    DrawSync(0);
    ClearImage2(&D_8005B640, 0, 0, 0);
    DrawSync(0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800272AC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027408);

void func_8002759C(void) {
    RECT *temp_s0;

    temp_s0 = D_8005B698;
    if (temp_s0 != 0) {
        DrawSync(0);
        ClearImage(&temp_s0[1], 0, 0, 0);
        ClearImage(&temp_s0[0], 0, 0, 0);
    }
}

void func_800275F8(arg0)
RECT *arg0;
{
    DrawSync(0);
    D_8005B78C = 0;
    if (arg0 != NULL) {
        D_8005B694 = D_8005B690 + 1;
        if (D_8005B698 == NULL) {
            u8 *g = D_8005B790;
            u8 *h;

            ClearOTagR((u_long *)g, 0x1000);
            h = g + 0x400C;
            *(s32 *)(g + 0x4008) = *(s32 *)(g + 0x4000);
            ClearOTagR((u_long *)h, 0x1000);
            *(s32 *)(h + 0x4008) = *(s32 *)(h + 0x4000);
        }
    } else {
        SetDispMask(0);
    }
    D_8005B698 = arg0;
}

void func_800276A4(void) {
    func_800275F8();
}

void func_800276C4(s32 *arg0) {
    u8 *g;
    u8 *t;

    t = func_80011654(*arg0 + 0x1800, &D_8004E0F0);
    g = D_8005B790;
    *(u8 **)(g + 0x4000) = t;
    *(s32 *)(g + 0x4004) = *(s32 *)(g + 0x4000) + *arg0 + 0x1800;
    *(u8 **)((g += 0x8000) + 0xC) = func_80011654(*arg0 + 0x1800, &D_8004E0F0);
    *(s32 *)(g + 0x10) = *(s32 *)(g + 0xC) + *arg0 + 0x1800;
}

void func_8002775C(u8 *arg0) {
    ClearOTagR((u_long *)arg0, 0x1000);
    *(s32 *)(arg0 + 0x4008) = *(s32 *)(arg0 + 0x4000);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027790);

void func_800278BC(s32 *arg0, s32 arg1) {
    CdIntToPos(*arg0 + arg1, &D_8006EA14);
    CdControlF(CdlSetloc, (u8 *)&D_8006EA14);
    CdControlF(CdlSeekL, (u8 *)&D_8006EA14);
}

void func_80027908(void) {
    if (D_800637B4 > 0) {
        D_800637B4 = CdReadSync(0, 0);
    }
}

s32 func_80027944(void) {
    s32 r;
    r = D_800637B4;
    if (r > 0) {
        D_800637B4 = CdReadSync(1, 0);
        if (D_800637B4 == 0) {
            CdControl(1, 0, &D_8006EA08);
            func_80034AEC();
        }
        r = D_800637B4;
    }
    return r;
}

void func_800279A4(void) {
    CdlFILE file;

    do {
        CdInit();
    } while (CdDiskReady(1) != 2);
    func_80034C4C(0, 0);
    D_800637B4 = 0;
    while (CdSearchFile(&file, "\\CRASHBSH\\CRASHBSH.DAT;1") == NULL) {
        printf("Cant find CRASHBSH.DAT");
    }
    D_800637B8 = CdPosToInt(&file.pos);
}

void func_80027A30(s16 vol) {
    SpuCommonAttr attr;

    attr.mask = SPU_COMMON_MVOLL | SPU_COMMON_MVOLR | SPU_COMMON_CDVOLL | SPU_COMMON_CDVOLR | SPU_COMMON_CDMIX;
    attr.mvol.left = 0x37FF;
    attr.mvol.right = 0x37FF;
    attr.cd.volume.left = vol;
    attr.cd.volume.right = vol;
    attr.cd.mix = 1;
    SpuSetCommonAttr(&attr);
}

void func_80027A74(void) {
    func_80012FFC();
}

void func_80027A94(void) {
}

void func_80027A9C(void) {
    if (D_80063A7C >= 0x1F) {
        D_80063A7C = 0x1E;
    }
    func_80027A30(0);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027AD4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80027D90);

void func_80027F00(void) {
    PadState *pad;
    s32 i;
    s32 on;

    D_800515F8 = 0;
    PadInitMtap(D_80063A90[0], D_80063A90[1]);
    i = 0;
    on = 1;
    pad = D_80051338;
loop:
    if (i < 8) {
        i++;
        pad->unk4 = 0;
        pad->unk0 = 0;
        pad->unk18 = on;
        pad->unk1C = 0;
        pad->unk22 = 0;
        pad->unk28 = 0;
        pad++;
        goto loop;
    }
    func_8002C894();
    PadStartCom();
}

void func_80027F7C(s32 pad, s32 arg1, s8 arg2) {
    u8 *p;

    if (D_8005A614[pad + 0x5C] == 0 && !(*(s32 *)(D_8005A614 + 4) & 0x80) && D_8005A604[pad + 0xC] != 0 &&
        (D_80051338[pad].unk0 & 8)) {
        p = (u8 *)&D_80051338[pad] + 0x1C;
        p[5] = arg2;
        *(s16 *)(p + 0xA) = arg1 + 1;
        *(u16 *)(p + 6) |= 1;
    }
}

void func_80028018(s32 pad, s32 arg1) {
    u8 *p;

    if (D_8005A614[pad + 0x5C] == 0 && !(*(s32 *)(D_8005A614 + 4) & 0x80) && pad < 4 && D_8005A604[pad + 0xC] != 0 &&
        (D_80051338[pad].unk0 & 8)) {
        p = (u8 *)&D_80051338[pad] + 0x1C;
        p[4] = 1;
        *(s16 *)(p + 8) = arg1 + 1;
        *(u16 *)(p + 6) |= 1;
    }
}

s32 func_800280C0(u8 *src, u8 *dst, s32 n) {
    s32 count;

    if ((*(u16 *)src & 0xF000) == 0x8000) {
        if (n == 0) {
            func_80027AD4(src + 2, dst, 0);
            func_80027AD4(src + 0xA, dst + 0x58, 1);
            func_80027AD4(src + 0x12, dst + 0xB0, 2);
            func_80027AD4(src + 0x1A, dst + 0x108, 3);
            count = 4;
        } else {
            count = 0;
        }
    } else {
        func_80027AD4(src, dst, n);
        count = 1;
    }
    return count;
}

void func_80028158(void) {
    s32 v;
    s32 i;

    v = 1;
    for (i = 7; i >= 0; i--) {
        D_80051338[i].unk28 = v;
    }
}

void func_80028184(u8 *arg0, s32 arg1, s8 arg2) {
    (*(s8*)((u8*)(arg0) + 5)) = arg2;
    (*(s16*)((u8*)(arg0) + 10)) = (s16) (arg1 + 1);
    (*(u16*)((u8*)(arg0) + 6)) = (u16) ((*(u16*)((u8*)(arg0) + 6)) | 1);
}

void func_800281A0(u8 *arg0, s32 arg1) {
    (*(s8*)((u8*)(arg0) + 4)) = 1;
    (*(s16*)((u8*)(arg0) + 8)) = (s16) (arg1 + 1);
    (*(u16*)((u8*)(arg0) + 6)) = (u16) ((*(u16*)((u8*)(arg0) + 6)) | 1);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800281C0);

void func_800282D8(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 4)) = 0;
    (*(s32*)((u8*)(arg0) + 0)) = 0;
    (*(s32*)((u8*)(arg0) + 24)) = 1;
    (*(s32*)((u8*)(arg0) + 28)) = 0;
    (*(s16*)((u8*)(arg0) + 34)) = 0;
    (*(s16*)((u8*)(arg0) + 40)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_800282F8);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028420);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002854C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028774);

s32 func_80028994(s32 a, s32 b, s32 c) {
    s32 r;
    s32 h;

    r = 0;
    if (c == 0x10) {
        h = a >> 1;
        if (h >= b) {
            b = h;
            c = 0x100;
            goto dec;
        }
    }
    if (b < a) {
        b = a;
    }
dec:
    b--;
    b >>= 3;
    while (b > 0) {
        b >>= 1;
        r++;
    }
    r <<= 1;
    if (c == 0x100) {
        r++;
    }
    return r;
}

void *func_80028A00(void) {
    u8 *cur;

    cur = D_80063BB4;
    if (cur != NULL) {
        D_80063BB4 = *(void **)(cur + 4);
    }
    return cur;
}

FreeNode *func_80028A28(void) {
    FreeNode *head;
    FreeNode *prev;
    FreeNode *n;
    FreeNode *nx;
    FreeHdr *src;
    FreeHdr h;
    s32 i;

    head = D_80063B18;
    D_80063B18 = head->next;
    src = func_800282F8(D_80063B58);
    prev = NULL;
    h = *src;
    i = 0x1F;
    do {
        n = D_80063B18;
        nx = n->next;
        i--;
        n->next = prev;
        n->fC = head;
        n->hdr = h;
        prev = n;
        D_80063B18 = nx;
        h.f2++;
    } while (i != -1);
    head->f10 = n;
    head->f14 = 0x20;
    head->fC = src;
    return head;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028AE4);

s32 func_80028BC8(u8 **arg0) {
    u8 *p;
    s32 n;

    n = 0;
    for (p = *arg0; p != NULL; p = *(u8 **)(p + 4)) {
        n++;
    }
    return n;
}

void func_80028BF0(void) {
    D_800335BC = 0x2626001F;
    FntLoad(0x380, 0x100);
    D_800643E8 = FntOpen(0x10, 0x10, D_8005B698->w - 8, 0xE0, 0, 0x800);
    D_800643EC = GetTPage(0, 1, 0x380, 0x100) & 0xFFFF;
    D_800643F0 = LoadClut2(D_80063AF8, 0x380, 0x11F) & 0xFFFF;
}

void func_80028C94(void) {
    D_800335BC = 0x2626001F;
    FntLoad(0x380, 0x100);
    D_800643E8 = FntOpen(0x10, 0x10, D_8005B698->w - 8, 0xE0, 0, 0x800);
    D_800643EC = GetTPage(0, 1, 0x380, 0x100) & 0xFFFF;
    D_800643F0 = LoadClut2(D_80063AF8, 0x380, 0x11F) & 0xFFFF;
}

void func_80028D38(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028D40);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80028F28);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029100);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002926C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029560);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029790);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002992C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029D28);

INCLUDE_ASM("asm/nonmatchings/main/main", func_80029F9C);

void func_8002A2DC(void) {
}

void func_8002A2E4(void) {
}

s32 func_8002A2EC(s32 r, s32 g, s32 b) {
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

void func_8002A35C(u8 *arg0) {
    u8 *temp_a0;
    u8 *temp_v0;

    if ((*(s32*)((u8*)(arg0) + 24)) != 0) {
        temp_a0 = (*(s32*)((u8*)(arg0) + 16));
        temp_v0 = func_800112A4(temp_a0, (*(s32*)((u8*)(temp_a0) + 24)), &D_8004E0F0);
        (*(s32*)((u8*)(arg0) + 16)) = temp_v0;
        func_80011544(temp_v0, arg0 + 0x18);
        (*(s32*)((u8*)(arg0) + 16)) = NULL;
        return;
    }
    func_80011578((*(s32*)((u8*)(arg0) + 16)), &D_8004E0F0, 2);
    (*(s32*)((u8*)(arg0) + 16)) = NULL;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A3D0);

void func_8002A4B8(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A4C0);

void func_8002A5A4(u8 *a0, u8 *arg1) {
    Func2A5A4Obj *arg0 = (Func2A5A4Obj *)a0;
    arg0->src = arg1;
    func_800115D8(*(u16 *)(arg1 + 8) * 0x38, (void **)&arg0->f8, (s32)&D_8004E0F0, 0);
    func_800115D8(*(u16 *)(arg0->src + 0xA) * 0xC, (void **)&arg0->fC, (s32)&D_8004E0F0, 0);
    arg0->f0 = *(u16 *)(arg0->src + 8);
    arg0->f4 = *(u16 *)(arg0->src + 0xA);
    arg0->f14 = *(s32 *)(arg0->src + 0x14);
    func_8002926C(arg0);
}

/* Size in bytes: 0xB0 plus +8 records of 0x38 and +0xA records of 0xC. */
s32 func_8002A650(u8 *arg0) {
    s32 size;

    size = *(u16 *)(arg0 + 8) * 0x38 + 0xB0;
    size += *(u16 *)(arg0 + 0xA) * 0xC;
    return size;
}

void func_8002A67C(void) {
}

void func_8002A684(u8 *arg0, u8 *arg1) {
    s32 old;
    s32 pos;
    s32 step;
    s32 idx;
    s32 *p;
    u8 *rect;

    rect = *(u8 **)(arg1 + 8) + *(s32 *)(arg0 + 4) * 0x38;
    old = *(s32 *)(arg0 + 0x10) >> 8;
    pos = *(s32 *)(arg0 + 0x10) + *(s32 *)(arg0 + 0xC);
    *(s32 *)(arg0 + 0x10) = pos;
    if (*(s32 *)(arg0 + 0xC) < 0) {
        if (pos < 0) {
            *(s32 *)(arg0 + 0x10) = pos + (*(s32 *)(arg0 + 8) << 8);
        }
    } else {
        step = *(s32 *)(arg0 + 8);
        if ((pos >> 8) >= step) {
            *(s32 *)(arg0 + 0x10) = pos - (step << 8);
        }
    }
    idx = *(s32 *)(arg0 + 0x10) >> 8;
    if (idx != old) {
        p = (s32 *)(arg0 + *(s32 *)arg0 + idx * 4);
        LoadImage((RECT *)(rect + 0x18), (u_long *)((u8 *)p + *p));
    }
}

void func_8002A740(u8 *arg0) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(arg0) + 4));
    if (temp_a0 != 0) {
        func_8002854C(temp_a0);
        (*(s32*)((u8*)(arg0) + 4)) = 0;
    }
}

void func_8002A77C(ClutUpload *arg0, s32 arg1) {
    RECT rect;

    if (arg0->w < 0x11U) {
        arg0->pos = func_80028420();
    } else {
        arg0->pos = func_80028A00();
    }
    rect.x = arg0->pos->x;
    rect.y = arg0->pos->y;
    rect.w = arg0->w;
    rect.h = 1;
    LoadImage(&rect, (u_long *)(arg1 + arg0->off));
    arg0->clut = GetClut(rect.x, rect.y);
}

void func_8002A830(u8 *src, u8 *out, s32 len) {
    u16 id;

    id = *(u16 *)src;
    src -= len - 4;
    *(s32 *)(out + 4) = 0;
    *(u8 **)(out + 8) = src;
    *(u16 *)out = id;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A84C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002A9AC);

void func_8002AA50(u8 *arg0) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(arg0) + 32));
    if (temp_a0 != 0) {
        func_80028AE4(temp_a0);
        (*(s32*)((u8*)(arg0) + 32)) = 0;
    }
}

void func_8002AA8C(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 0)) = 0;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
    (*(s32*)((u8*)(arg0) + 16)) = 0;
}

void func_8002AAA0(void) {
    D_80064414 = 0;
}

void func_8002AAAC(void) {
    D_80064414 = 1;
}

void func_8002AABC(void) {
    D_8006557C.data = 0;
    SsInit();
    SsSetTableSize(D_800654CC, 1, 1);
    SpuReserveReverbWorkArea(1);
    SsUtSetReverbType(7);
    SsUtSetReverbDepth(100, 0);
    func_800369FC();
    func_8002AAA0();
    D_800643F8 = (void (*)())SsSetTickCallback(func_8002ABA8);
    SsSetTickMode(1);
    SsStart();
}

void func_8002AB44(void) {
    if (D_800643F8 != 0) {
        func_8002C428();
        SsUtFlush();
        SsEnd();
        SsSetTickCallback(D_800643F8);
        func_800366FC();
        VSync(3);
        D_800643F8 = 0;
    }
}

void func_8002ABA8(void) {
    s32 temp_v0;

    D_800654AC += 1;
    if (D_800654B0 > 0) {
        temp_v0 = D_800654B0 - 1;
        D_800654B0 = temp_v0;
        if (temp_v0 == 0) {
            D_800654B4 += 1;
        }
    }
    if (D_80064414 == 0) {
        func_8002B4B8();
    }
}

void func_8002AC18(void) {
    func_8002C0E0();
    func_800227C0();
}

void func_8002AC40(void) {
    func_8002C11C();
    func_800227FC();
    func_80027A9C();
}

void func_8002AC70(void) {
    func_8002C428();
    func_80022504();
    func_80027A9C();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002ACA0);

void func_8002ADD4(void *data, VabSlot *slot) {
    u8 *head;
    u8 *body;
    s16 id;

    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);
    if ((D_800654C0 != NULL || D_800654C4 != NULL) && SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
        D_800654C4->id = D_800654C8;
        func_800112A4(D_800654C0, D_800654C0->head, &D_8004E0F0);
        func_8002ACA0(D_800654C4->id, 0);
        D_800654C0 = NULL;
        D_800654C4 = NULL;
    }
    head = (u8 *)data + ((VabFile *)data)->head;
    slot->head = head;
    slot->handle = 0;
    body = (u8 *)data + ((VabFile *)data)->body;
    id = SsVabOpenHead(slot->head, -1);
    SsVabTransBody(body, id);
    D_800654C8 = id;
    D_800654C0 = data;
    D_800654C4 = slot;
}

void func_8002AEE0(void) {
    MATRIX *m;

    if (D_800654C0 != NULL || D_800654C4 != NULL) {
        if (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
            D_800654C4->id = D_800654C8;
            func_800112A4(D_800654C0, D_800654C0->head, &D_8004E0F0);
            func_8002ACA0(D_800654C4->id, 0);
            D_800654C0 = NULL;
            D_800654C4 = NULL;
        }
    }
    SpuGetAllKeysStatus(D_800644A8);
    func_80014540(D_80051640, D_8005B698);
    m = (MATRIX *)(D_80051640 + 0x74);
    gte_SetRotMatrix(m);
    gte_SetGeomScreen(*(s32 *)(D_80051640 + 0x18));
    gte_SetTransMatrix(m);
    gte_SetGeomOffset(D_8005B698->w / 2, 175);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B020);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B210);

void func_8002B4B8(void) {
    SsSeqCalledTbyT();
}

void func_8002B4D8(PitchVoice *arg0, s32 arg1) {
    PitchInfo *info = arg0->info;

    SsUtChangePitch(arg0->voice, info->note, ((s32)(info->fine << 24)) >> 28, 0x3C, 0,
                    (s32)(arg1 << 9) >> 16, arg1 & 0x7F);
}

void func_8002B52C(s16 *arg0, s32 arg1, s32 arg2) {
    s32 vol = D_8005A608 * 0x7F;

    SsUtSetVVol(*arg0, (arg1 * vol) / 409600, (arg2 * vol) / 409600);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B5B0);

s32 func_8002B6D0(VECTOR *pos, View *view) {
    VECTOR *out = &D_800569CC.out;
    s32 sxy;
    s32 *psxy = &sxy;
    SVECTOR *in = &D_800569CC.in;

    in->vx = pos->vx - view->pos.vx;
    in->vy = pos->vy - view->pos.vy;
    in->vz = pos->vz - view->pos.vz;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(psxy);
    return func_8002B020(out, D_80064478);
}

void func_8002B760(VECTOR *pos, View *view, VECTOR *out, s32 *sxy) {
    D_800569CC.in.vx = pos->vx - view->pos.vx;
    D_800569CC.in.vy = pos->vy - view->pos.vy;
    D_800569CC.in.vz = pos->vz - view->pos.vz;
    gte_ldv0(&D_800569CC.in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(sxy);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002B7C8);

void func_8002B8F4(s32 *voice) {
    if (*voice != -1) {
        SsUtKeyOffV(*voice);
        D_800654B8 &= ~D_80064418[*voice];
        func_800373FC(D_800654B8);
        *voice = -1;
    }
}


void func_8002B96C(void) {
    SpuIsTransferCompleted(1);
    D_800654B8 = 0;
    func_800373FC(0);
    SsUtAllKeyOff(0);
}

void func_8002B9A4(void) {
    s8 *p;
    s32 i;

    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);
    p = SfxTable[0];
    for (i = 0; i < 2001; i++) {
        p[0] = -1;
        p[1] = -1;
        p += 2;
    }
    D_800654BC = 0;
    D_800654C0 = NULL;
    D_800654C4 = NULL;
    D_800654B8 = 0;
    func_8002B96C();
}

void func_8002BA10(s32 arg0, VabSlot *slot) {
    slot->handle = func_80013688(arg0, func_8002ADD4, slot);
}

void func_8002BA44(void) {
    SpuIsTransferCompleted(1);
    if (D_800654C0 != NULL || D_800654C4 != NULL) {
        if (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
            D_800654C4->id = D_800654C8;
            func_800112A4(D_800654C0, D_800654C0->head, &D_8004E0F0);
            func_8002ACA0(D_800654C4->id, 0);
            D_800654C0 = NULL;
            D_800654C4 = NULL;
        }
    }
}

void func_8002BAE8(void) {
    if ((D_800654C0 != NULL || D_800654C4 != NULL) && SpuIsTransferCompleted(0) == 1) {
        D_800654C4->id = D_800654C8;
        func_800112A4(D_800654C0, D_800654C0->head, &D_8004E0F0);
        func_8002ACA0(D_800654C4->id, 0);
        D_800654C0 = NULL;
        D_800654C4 = NULL;
    }
}

s32 func_8002BB80(void) {
    if ((D_800654C0 != 0) || (D_800654C4 != 0)) {
        return 1;
    }
    return 0;
}

/* Empties the D_800644C0 slots and clears SfxTable. */
void func_8002BBB4(void) {
    s8 *p;
    s32 i;

    D_800654BC = 0;
    p = SfxTable[0];
    for (i = 0; i < 2001; i++) {
        p[0] = -1;
        p[1] = -1;
        p += 2;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BBEC);

void func_8002BCB0(s32 *arg0) {
    SpuIsTransferCompleted(1);
    func_8002ACA0(arg0[1], 1);
    SsVabClose((s16)arg0[1]);
    func_800110A8((void *)arg0[0], &D_8004E0F0);
    arg0[0] = 0;
    arg0[1] = -1;
}

void func_8002BD0C(s32 arg0) {
    if (D_800654BC < 6) {
        func_8002BA10(arg0, &D_800644C0[D_800654BC]);
        D_800654BC++;
    }
}

/* Clears SfxTable: no sound for any id. */
void func_8002BD68(void) {
    s8 *p;
    s32 i;

    p = SfxTable[0];
    for (i = 0; i < 2001; i++) {
        p[0] = -1;
        p[1] = -1;
        p += 2;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BD98);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002BEA8);

void func_8002C060(void) {
    D_8006557C.data = 0;
    D_8006557C.seq = -1;
    D_800655A4 = -1;
    D_8006559C = -1;
    D_800655B4 = 0;
}

void func_8002C090(SeqSlot *slot) {
    if (slot->seq >= 0) {
        SsSeqClose(slot->seq);
        SsSepStop(slot->seq, 0);
    }
    slot->unk1C = -1;
}

void func_8002C0E0(void) {
    if (D_8006557C.seq >= 0) {
        SsSeqReplay(D_8006557C.seq);
    }
}

void func_8002C11C(void) {
    if (D_8006557C.seq >= 0) {
        SsSeqPause(D_8006557C.seq);
    }
}

void func_8002C158(u8 *arg0) {
    if (*(s32 *)(arg0 + 4) >= 0) {
        SsSeqReplay(*(s32 *)(arg0 + 4));
    }
}

void func_8002C18C(u8 *arg0) {
    if (*(s32 *)(arg0 + 4) >= 0) {
        SsSeqPause(*(s32 *)(arg0 + 4));
    }
}

void func_8002C1C0(s32 arg0, s32 arg1) {
    if (D_8006557C.seq >= 0) {
        SsSeqSetAccelerando((s16)D_8006557C.seq, (D_80058B88[D_8005A648].tempo * arg0) / 100, arg1);
    }
}

void func_8002C248(s32 arg0) {
    if (D_8006557C.seq >= 0) {
        SsSeqSetDecrescendo(D_8006557C.seq, 0x7F, arg0);
    }
}

void func_8002C284(s32 arg0) {
    D_8006559C = arg0;
}

void func_8002C290(void) {
    D_800655AC = 0;
}

void func_8002C29C(void) {
    D_800655AC = 1;
}

void func_8002C2AC(void) {
    D_800655A8 += 1;
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C2C4);

void func_8002C394(s32 arg0, SeqSlot *slot, s32 type) {
    func_80013688(arg0, func_8002C3F0, slot);
    SpuReserveReverbWorkArea(1);
    SsUtSetReverbType(type);
    SsUtSetReverbDepth(0, 0);
    func_800369FC();
}

void func_8002C3F0(s32 arg0, s32 *arg1) {
    *arg1 = arg0;
    func_8002C2C4(arg1);
    D_800655A8 = 0;
    D_800655AC = 0;
    D_800655B0 = 0;
}

void func_8002C428(void) {
    func_8002C090(&D_8006557C);
}

void func_8002C44C(void) {
    if (D_8006557C.data != 0) {
        func_800110A8(D_8006557C.data, &D_8004E0F0);
        D_8006557C.data = 0;
    }
}

void func_8002C488(s32 arg0, s32 type) {
    func_8002C394(arg0, &D_8006557C, type);
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C4B0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C5BC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C894);

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004CD8C);

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004CDAC);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002C97C);

s32 func_8002D094(void) {
    if (D_800655C4 == 1 && (D_800655C0 & 0x80)) {
        return 1;
    }
    return 0;
}

void func_8002D0C8(void) {
    s32 flags;
    s32 on;

    flags = D_800655C0;
    if ((flags & 2) && (flags & 0x30)) {
        on = TRUE;
        D_800675C8 = on;
        D_800655C4 = on;
        D_800655C0 = flags | 0x80;
    }
}

void func_8002D108(void) {
    s32 mode = 2;

    D_800675C8 = 1;
    D_800655C4 = mode;
    D_800655C0 |= 0x80;
}

void func_8002D134(s32 arg0) {
    D_800675C8 = 1;
    D_800655C4 = arg0;
    D_800655C0 |= 0x80;
}

void func_8002D15C(void) {
    D_800655C4 = 0;
    D_800675CC = 0;
    D_800655C0 = 0;
}

s32 func_8002D178(void) {
    SizeEntry *p;
    s32 i;
    s32 sum;

    i = D_800675D0;
    if (i >= 0xF) {
        return 0;
    }
    p = D_800675D4;
    i--;
    sum = 0;
    if (i != -1) {
        do {
            sum += p->size;
            p++;
            i--;
        } while (i != -1);
    }
    return 0x1E000 - sum;
}

SizeEntry *func_8002D1D4(void) {
    SizeEntry *p;
    s32 i;

    i = D_800675D0;
    p = D_800675D4;
    i--;
    if (i != -1) {
        do {
            if (strcmp(D_8004CDAC, (char *)p) == 0) {
                return p;
            }
            i--;
            p++;
        } while (i != -1);
    }
    return NULL;
}

void func_8002D254(void) {
    MemCardStop();
}

void func_8002D274(void) {
    func_8002C97C();
}

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D294);

void func_8002D33C(u8 *arg0) {
    (*(s8*)((u8*)(arg0) + 0)) = 0;
    (*(s8*)((u8*)(arg0) + 1)) = 0;
    (*(s8*)((u8*)(arg0) + 2)) = 0;
    (*(s8*)((u8*)(arg0) + 3)) = 0;
    (*(s8*)((u8*)(arg0) + 4)) = 0;
    (*(s8*)((u8*)(arg0) + 5)) = 0;
    (*(s8*)((u8*)(arg0) + 6)) = 0;
    (*(s8*)((u8*)(arg0) + 7)) = 0;
}

s32 func_8002D360(u8 *arg0) {
    u32 *p;
    u32 *end;
    s32 sum;

    p = (u32 *)(arg0 + 0x200);
    end = (u32 *)(arg0 + 0x850);
    sum = 0;
    while (p < end) {
        sum += *p++;
    }
    return sum;
}


INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D394);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D488);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002D4F4);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002DE2C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E000);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E0F0);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E25C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E3D0);

s32 func_8002E584(u8 *arg0) {
    u16 max;
    u16 min;
    s32 i;
    s32 bit;
    s32 first;
    s32 b;

    max = 0;
    min = 7;
    i = 0xE;
    do {
        first = 1;
        bit = 7;
        b = *arg0;
        do {
            if ((b >> bit) & 1) {
                if (first != 0) {
                    if (max < bit) {
                        max = bit;
                    }
                    if (bit < min) {
                        min = bit;
                    }
                    first = 0;
                } else {
                    if (bit < min) {
                        min = bit;
                    }
                }
            }
            bit--;
        } while (bit >= 0);
        i--;
        arg0++;
    } while (i >= 0);
    return (min << 16) | max;
}

INCLUDE_RODATA("asm/nonmatchings/main/main", D_8004CE5C);

INCLUDE_ASM("asm/nonmatchings/main/main", func_8002E618);
