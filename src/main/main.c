#include "common.h"
#include "bash/main.h"

s32 count_completion(u8 *arg0) {
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
    CompletionBcd = (n - q * 10) | (t << 4) | (d << 8) | ((i % 10) << 12);
    return n;
}

void shell_init(void) {
    VSyncRate = 2;
    mal_init(&MainHeap, HeapStart, 0x801FF000 - (s32)HeapStart);
    init_file();
    pads_init();
    pads_update();
    load_file_direct(&DatIndex[2 * 991], HeapStart);
    EngineModule = (ModuleEntry *)HeapStart;
    RunModules(&ShellModules);
}

const char LevNameJungleBash[] = "JUNGLE BASH";
const char LevNameSpaceBash[] = "SPACE BASH";
const char LevNameSnowBash[] = "SNOW BASH";
const char LevNameDrainBash[] = "DRAIN BASH";
const char LevNamePapuPummel[] = "PAPU PUMMEL";
const char LevNameSwampFox[] = "SWAMP FOX";
const char LevNameDesertFox[] = "DESERT FOX";
const char LevNameJungleFox[] = "JUNGLE FOX";
const char LevNameMetalFox[] = "METAL FOX";
const char LevNameBigBadFox[] = "BIG BAD FOX";
const char LevNamePogoPainter[] = "POGO PAINTER";
const char LevNamePogoAGogo[] = "POGO-A-GOGO";
const char LevNameElPogoLoco[] = "EL POGO LOCO";
const char LevNamePogoPadlock[] = "POGO PADLOCK";
const char LevNameCrashball[] = "CRASHBALL";
const char LevNameBeachBall[] = "BEACH BALL";
const char LevNameNBallism[] = "N.BALLISM";
const char LevNameSkyBalls[] = "SKY BALLS";
const char LevNameTiltPanic[] = "TILT PANIC";
const char LevNamePolarPanic[] = "POLAR PANIC";
const char LevNameMeltPanic[] = "MELT PANIC";
const char LevNameManicPanic[] = "MANIC PANIC";
const char LevNameBearminator[] = "BEARMINATOR";
const char LevNameDotDash[] = "DOT DASH";
const char LevNameToxicDash[] = "TOXIC DASH";
const char LevNameSplashDash[] = "SPLASH DASH";
const char LevNameDantesDash[] = "DANTE'S DASH";
const char LevNameRingDing[] = "RING DING";
const char LevNameDragonDrop[] = "DRAGON DROP";
const char LevNameMalletMash[] = "MALLET MASH";
const char LevNameKegKaboom[] = "KEG KABOOM";
const char LevNameOxideRide[] = "OXIDE RIDE";
const char LevNameTieBreaker[] = "TIE-BREAKER";
const char LevNameWarpRoom1[] = "WARP ROOM 1";
const char LevNameWarpRoom2[] = "WARP ROOM 2";
const char LevNameWarpRoom3[] = "WARP ROOM 3";
const char LevNameWarpRoom4[] = "WARP ROOM 4";
const char LevNameSecretWarpRoom[] = "SECRET WARP ROOM";
const char LevNameDemoHub[] = "DEMO HUB";
const char LevNameTitles[] = "TITLES";
const char LevNameEmpty[] = "";
const char LevNamePodium[] = "PODIUM";
const char LevNameScriptStuff[] = "SCRIPT STUFF";
const char LevNameMedievalMayhem[] = "MEDIEVAL MAYHEM";
const char LevNameCrateCrush[] = "CRATE CRUSH";
const char LevNameTankWars[] = "TANK WARS";
const char LevNamePogoPandemonium[] = "POGO PANDEMONIUM";
const char LevNameBallistix[] = "BALLISTIX";
const char LevNamePolarPush[] = "POLAR PUSH";
const char LevNameCrashDash[] = "CRASH DASH";

/* Shuts the system down and runs the Spyro 3 demo from the disc. */
void shell_run_spyro_demo(void) {
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
    if (DcacheReady == 0) {
        DcacheReady = 1;
        SavedSp = GetSp();
    }
    SetSp(0x1F8003FC);
    EngineModule->render();
    FlipScreen(VSyncRate);
    SetSp(SavedSp);
}

void shell_init_heap(void) {
    VSyncRate = 2;
    mal_init(&MainHeap, HeapStart, 0x801FF000 - (s32)HeapStart);
    init_file();
    pads_init();
    pads_update();
}

void shell_render(void) {
    EngineModule->render();
    FlipScreen(VSyncRate);
}

void shell_update(void) {
    loader_update();
    pads_update();
    rand_step();
    sfx_update_all();
    music_update();
    if (!(FrameCount & 1)) {
        card_poll();
    }
    EngineModule->update();
}

void shell_start_engine(void) {
    u8 *p = EngineModule->heap;

    if (p != NULL) {
        mal_init(&MainHeap, p, 0x801FF000 - (s32)p);
        smallnode_init();
    }
    sound_init();
    SetNasDefaults();
    EngineModule->init();
}

static inline void mal_merge_from(mal *a, _mal_area *arg1, s32 m1) {
    s32 m2;
    mal *b;
    s32 new_var2;
    mal *prev;
    mal *new_var;
    mal *end;
    s32 *d;
    s32 *s;
    s32 n;
    u32 size;

    mal_unlink(a, arg1);
    size = a->size;
    b = (mal *)((u8 *)a + (size * 4 + 0x18));
    new_var = a;
    n = b->size;
    prev = a->prev;
    d = (s32 *)((u8 *)a + 0x18);
    *a = *b;
    s = (s32 *)((u8 *)b + 0x18);
    new_var->prev = prev;
    end = (mal *)((u8 *)b + (n * 4 + 0x18));
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
    b = (mal *)((u8 *)a + (a->size * 4 + 0x18));
    end->prev = b;
    b->size = new_var2;
    b->owner = (void **)m1;
    b->unk0 = 0;
    b->prev = a;
    if (a == arg1->lowest) {
        arg1->lowest = mal_next_free_block(b);
    }
    mal_free(b + 1, arg1);
}

#ifndef NON_MATCHING
void mal_update(_mal_area *heap) {
    u32 size;
    s32 size2;
    s32 n;
    s32 *s;
    s32 *d;
    mal *end;
    mal *p;
    mal *a;
    mal *b;
    mal *prev;
    mal *q;
    mal *qprev;
    s32 budget;

    p = Heap_NextToMove(heap);
    budget = (0x1FE - VSync(1)) * 0x70;
    do {
        s32 t;

        if (p == NULL) break;
        budget -= p->size;
        if (budget <= 0) break;
        t = VSync(1);
        MalMoveWords += p->size;
        a = heap->lowest;
        mal_unlink(a, heap);
        size = a->size;
        b = (mal *)((u8 *)a + (size * 4 + 0x18));
        n = b->size;
        prev = a->prev;
        d = (s32 *)((u8 *)a + 0x18);
        *a = *b;
        s = (s32 *)((u8 *)b + 0x18);
        a->prev = prev;
        end = (mal *)((u8 *)b + (n * 4 + 0x18));
        size2 = size;
        n--;
        *a->owner = (void *)d;
        while (n != -1) {
            *d++ = *s++;
            n--;
        }
        b = (mal *)((u8 *)a + (a->size * 4 + 0x18));
        end->prev = b;
        b->size = size2;
        b->owner = (void **)-1;
        b->unk0 = 0;
        b->prev = a;
        if (a == heap->lowest) {
            heap->lowest = mal_next_free_block(b);
        }
        mal_free(b + 1, heap);
        t = VSync(1) - t;
        p = Heap_NextToMove(heap);
        MalMoveVSyncs += t;
    } while (1);
    q = heap->deferred;
    qprev = NULL;
    while (q != NULL) {
        if (--q->ticks <= 0) {
            if (qprev != NULL) {
                qprev->nextFree = q->nextFree;
            } else {
                heap->deferred = q->nextFree;
            }
            mal_free(q + 1, &MainHeap);
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
static inline mal *Heap_NextLowest(_mal_area *heap, s32 m1) {
    mal *blk;
    mal *next;

    blk = heap->lowest;
    if (blk == NULL) {
        return NULL;
    }
    next = (mal *)((u8 *)blk + (blk->size * 4 + sizeof(mal)));
    if (next->owner == (void **)m1 || next->size == 0) {
        return NULL;
    }
    return next;
}

void mal_update(_mal_area *heap) {
    s32 m1;
    mal *lo;
    mal *nx;
    mal *ret;
    mal *p;
    mal *q;
    mal *qprev;
    s32 budget;

    lo = heap->lowest;
    if (lo == NULL || (nx = (mal *)((u8 *)lo + (lo->size * 4 + 0x18)),
                       nx->owner == (void **)-1) || nx->size == 0) {
        ret = NULL;
    } else {
        ret = nx;
    }
    p = ret;
    if (p != NULL) {
        if ((p->unk0 & 2) && (p->unk0 & 1) == (FrameCount & 1)) {
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
        MalMoveWords += p->size;
        mal_merge_from(heap->lowest, heap, m1);
        t = VSync(1) - t;
        ret = Heap_NextLowest(heap, m1);
        p = ret;
        if (p != NULL) {
            if ((p->unk0 & 2) && (p->unk0 & 1) == (FrameCount & 1)) {
                p = NULL;
            }
        }
        MalMoveVSyncs += t;
    } while (1);
    q = heap->deferred;
    qprev = NULL;
    while (q != NULL) {
        if (--q->ticks <= 0) {
            if (qprev != NULL) {
                qprev->nextFree = q->nextFree;
            } else {
                heap->deferred = q->nextFree;
            }
            mal_free(q + 1, &MainHeap);
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

void mal_compact_from(mal *arg0, _mal_area *arg1) {
    s32 m1 = -1;
    mal *nx;
    mal *p;
    mal *ret;

    for (;;) {
        if (arg0 == NULL) {
            return;
        }
        mal_merge_from(arg1->lowest, arg1, m1);
        nx = arg1->lowest;
        if (nx == NULL || (nx = (mal *)((u8 *)nx + (nx->size * 4 + 0x18)),
                           nx->owner == (void **)m1) || nx->size == 0) {
            ret = NULL;
        } else {
            ret = nx;
        }
        p = ret;
        if (p != NULL) {
            if ((p->unk0 & 2) && (p->unk0 & 1) == (FrameCount & 1)) {
                p = NULL;
            }
        }
        arg0 = p;
    }
}

void mal_compact_high(_mal_area *heap) {
    mal *cur;
    mal *prev;
    mal *next;
    mal *blk;
    mal *new;
    u32 size;
    u32 *dst;
    u32 *src;
    s32 n;

    while (1) {
        cur = *(mal **)&heap->unk0[4];
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
        next = (mal *)((u8 *)blk + (size * 4 + 0x18));
        if (prev == NULL) {
            continue;
        }
        mal_unlink(blk, heap);
        dst = (u32 *)((u8 *)blk + (blk->size * 4 + 0x14));
        n = prev->size;
        src = (u32 *)((u8 *)prev + (n * 4 + 0x14));
        new = (mal *)((u8 *)dst - n * 4 - 0x14);
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

static inline mal *mal_compact_next(_mal_area *heap) {
    mal *blk;
    mal *next;

    blk = heap->lowest;
    if (blk == NULL) {
        return NULL;
    }
    next = (mal *)((u8 *)blk + (blk->size * 4 + sizeof(mal)));
    if (next->owner == (void **)-1 || next->size == 0) {
        return NULL;
    }
    return next;
}

static inline void mal_compact_merge(mal *arg0, _mal_area *arg1) {
    s32 m1;
    mal *next;
    s32 new_var2;
    mal *prev;
    mal *new_var;
    mal *end;
    s32 *d;
    s32 *s;
    s32 n;
    s32 size;

    mal_unlink(arg0, arg1);
    size = arg0->size;
    next = (mal *)((u8 *)arg0 + (size * 4 + 0x18));
    new_var = arg0;
    n = next->size;
    prev = arg0->prev;
    d = (s32 *)((u8 *)arg0 + 0x18);
    *arg0 = *next;
    s = (s32 *)((u8 *)next + 0x18);
    new_var->prev = prev;
    end = (mal *)((u8 *)next + (n * 4 + 0x18));
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
    next = (mal *)((u8 *)arg0 + (arg0->size * 4 + 0x18));
    end->prev = next;
    next->size = new_var2;
    next->owner = (void **)-1;
    next->unk0 = 0;
    next->prev = arg0;
    if (arg0 == arg1->lowest) {
        arg1->lowest = mal_next_free_block(next);
    }
    mal_free(next + 1, arg1);
}

void mal_compact(_mal_area *heap) {
    DrawSync(0);
    while (mal_compact_next(heap) != NULL) {
        mal_compact_merge(heap->lowest, heap);
    }
}

static inline mal *Heap_NextFreeAfter(mal *p) {
    while (p->size != 0) {
        p = (mal *)((u8 *)p + (p->size * 4 + 0x18));
        if (p->owner == NULL) {
            return p;
        }
    }
    return NULL;
}

static inline mal *Heap_FindFree(u32 words, mal *blk) {
    for (; blk != NULL; blk = blk->nextFree) {
        if (blk->size >= words) {
            return blk;
        }
    }
    return blk;
}

mal *mal_take_high(bytes, heap)
    s32 bytes;
    _mal_area *heap;
{
    u32 words;
    mal *blk;
    mal *ret;
    mal *cur;
    mal *p;
    mal *nb;
    mal *t;
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
        nb = (mal *)((u8 *)blk + (blk->size * 4 + 0x18));
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

static inline mal *Heap_NextFreeAfter2(mal *p) {
    while (p->size != 0) {
        p = (mal *)((u8 *)p + (p->size * 4 + 0x18));
        if (p->owner == NULL) {
            return p;
        }
    }
    return NULL;
}
static inline mal *Heap_FindFree2(u32 words, mal *blk) {
    for (; blk != NULL; blk = blk->nextFree) {
        if (blk->size >= words) {
            return blk;
        }
    }
    return blk;
}

void *mal_take_low(bytes, heap)
    s32 bytes;
    _mal_area *heap;
{
    u32 words;
    mal *blk;
    mal *ret;
    mal *cur;
    mal *p;
    mal *nb;
    mal *t;
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
        nb = (mal *)((u8 *)blk + (words * 4 + 0x18));
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

void mal_free(void *ptr, _mal_area *heap) {
    mal *blk;
    mal *next;
    mal *prev;
    mal *p;
    mal *q;

    blk = (mal *)ptr - 1;
    if (blk->owner != (void **)-1) {
        *blk->owner = NULL;
    }
    prev = blk->prev;
    next = (mal *)((u32 *)(blk + 1) + blk->size);
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

void *mal_trim_front(void *base, s32 offset, _mal_area *arg2) {
    mal *blk;
    mal *nb;
    mal *prev;
    mal *next;
    void **owner;
    u32 words;
    u32 sz;

    words = (u32)offset >> 2;
    words -= 6;
    blk = (mal *)base;
    blk--;
    next = (mal *)((u8 *)blk + (blk->size * 4 + 0x18));
    prev = blk->prev;
    nb = (mal *)((u8 *)blk + (words * 4 + 0x18));
    nb->owner = blk->owner;
    nb->unk0 = ((mal *)base)[-1].unk0;
    nb->ticks = blk->ticks;
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
s32 mal_count_free(_mal_area *heap) {
    mal *blk;
    s32 bytes;

    bytes = 0;
    blk = heap->free;
    MalFreeBlocks = 0;
    MalFreeBytes = 0;
    for (; blk != NULL; blk = blk->nextFree) {
        bytes = blk->size * 4;
        MalFreeBlocks++;
        MalFreeBytes += bytes;
    }
    MalLargestFree = bytes;
    return MalFreeBytes;
}

void mal_print_free(void) {
    mal_count_free(&MainHeap);
}

u8 *mal_trim(u8 *arg0, s32 arg1, _mal_area *arg2) {
    mal *blk;
    mal *new_blk;
    u32 words;
    u32 old;
    s32 rest;
    _mal_area *heap;

    void **neg1;

    words = (u32)arg1 + 3;
    words = words >> 2;
    heap = arg2;
    blk = (mal *)(arg0 - 0x18);
    old = blk->size;
    neg1 = (void **)-1;
    rest = old - words - 6;
    if (rest >= 0x10) {
        blk->size = words;
        new_blk = (mal *)((u8 *)blk + (0x18 + words * 4));
        new_blk->owner = neg1;
        new_blk->size = rest;
        new_blk->unk0 = 0;
        new_blk->prev = blk;
        ((mal *)((u8 *)blk + old * 4 + 0x18))->prev = new_blk;
        mal_free((u8 *)new_blk + 0x18, heap);
    }
    return arg0;
}

s32 mal_is_unowned(void *p) {
    return ~(s32)((mal *)p - 1)->owner == 0;
}

void mal_disown(void *p) {
    ((mal *)p - 1)->owner = (void **)-1;
    ((mal *)p - 1)->unk0 = 4;
}

void mal_set_owner(void *p, void **owner) {
    void **old;
    mal *b;

    b = (mal *)p - 1;
    old = b->owner;
    if (old != (void **)-1) {
        *old = NULL;
    }
    ((mal *)p - 1)->unk0 = 0;
    b->owner = owner;
    *owner = p;
}

s16 mal_get_flags(void *p) {
    return ((mal *)p - 1)->unk0;
}

void mal_free_later(u8 *arg0, _mal_area *heap, s32 arg2) {
    mal *blk;

    blk = (mal *)(arg0 - 0x18);
    if (blk->owner != (void **)-1) {
        *blk->owner = NULL;
    }
    mal_disown(arg0);
    blk->nextFree = heap->deferred;
    blk->ticks = arg2;
    heap->deferred = blk;
}

void *mal_alloc_owned_low(s32 arg0, void **arg1, s32 arg2, s32 arg3) {
    u8 *blk;
    u8 *ret;

    blk = mal_take_low(arg0, arg2);
    if (blk == NULL) {
        blk = mal_take_low(arg0, arg2);
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

u8 *mal_alloc(s32 arg0, _mal_area *arg1) {
    mal *next;
    mal *low;
    mal *b;
    u8 *data;

    mal_compact(arg1);
    b = mal_take_high(arg0, arg1);
    data = (u8 *)(b + 1);
    if (b == NULL) {
        low = arg1->lowest;
        next = NULL;
        if (low != NULL && ((next = (mal *)((u8 *)low + (low->size * 4 + 0x18))),
                            next->owner == (void **)-1 || next->size == 0)) {
            next = NULL;
        }
        if (next != NULL && (next->unk0 & 2)) {
            next = NULL;
        }
        mal_compact_from(next, arg1);
        b = mal_take_high(arg0, arg1);
        data = (u8 *)(b + 1);
        if (b == NULL) {
            mal_print_free();
            data = NULL;
        } else {
            goto set;
        }
    } else {
set:
        b->owner = (void **)-1;
        b->unk0 = 0;
    }
    return data;
}

u8 *mal_alloc_nocompact(s32 arg0, _mal_area *arg1) {
    mal *next;
    mal *low;
    mal *b;
    u8 *data;

    b = mal_take_high(); /* arg0 and arg1 pass through */
    data = (u8 *)(b + 1);
    if (b == NULL) {
        low = arg1->lowest;
        next = NULL;
        if (low != NULL && ((next = (mal *)((u8 *)low + (low->size * 4 + 0x18))),
                            next->owner == (void **)-1 || next->size == 0)) {
            next = NULL;
        }
        if (next != NULL && (next->unk0 & 2)) {
            next = NULL;
        }
        mal_compact_from(next, arg1);
        b = mal_take_high(arg0, arg1);
        data = (u8 *)(b + 1);
        if (b == NULL) {
            mal_print_free();
            return NULL;
        }
        goto set;
    }
set:
    b->owner = (void **)-1;
    b->unk0 = 0;
    return data;
}

void *mal_split_ptr(void *ptr, s32 bytes) {
    mal *blk;
    mal *split;
    mal *next;
    s32 words;

    blk = (mal *)ptr - 1;
    words = bytes >> 2;
    split = (mal *)((u32 *)blk + words);
    next = (mal *)((u32 *)(blk + 1) + blk->size);
    split->unk0 = 4;
    split->owner = (void **)-1;
    split->size = blk->size - words;
    split->prev = blk;
    blk->size = words - 6;
    next->prev = split;
    return split + 1;
}



mal *mal_split(mal *blk, s32 words) {
    mal *split;
    mal *next;

    split = (mal *)((u32 *)blk + words);
    next = (mal *)((u32 *)(blk + 1) + blk->size);
    split->unk0 = 4;
    split->owner = (void **)-1;
    split->size = blk->size - words;
    split->prev = blk;
    blk->size = words - 6;
    next->prev = split;
    return split;
}

mal *mal_take_low_from(mal *blk, s32 words, _mal_area *heap) {
    mal *split;
    mal *cur;
    mal *t;
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
    split = (mal *)((u8 *)blk + (words * 4 + 0x18));
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

void mal_take_high_from(mal *arg0, s32 arg1, _mal_area *arg2) {
    mal *cur;
    mal *t;
    mal *nb;
    mal *p;
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
    nb = (mal *)((u8 *)arg0 + (arg0->size * 4 + 0x18));
    nb->size = arg1;
    nb->prev = arg0;
    MEMBLOCK_NEXT(nb)->prev = nb;
}

void mal_insert_free(mal *blk, _mal_area *heap) {
    mal *p;
    mal *prev;

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

void mal_unlink(mal *blk, _mal_area *area) {
    mal *prev;
    mal *next;

    prev = blk->prevFree;
    if (prev != NULL) {
        prev->nextFree = blk->nextFree;
    } else {
        area->free = blk->nextFree;
    }
    next = blk->nextFree;
    if (next != NULL) {
        next->prevFree = blk->prevFree;
    }
}

mal *mal_next_free_block(mal *blk) {
    if (blk->size != 0) {
        blk = (mal *)((u32 *)(blk + 1) + blk->size);
        if (blk->owner != NULL) {
            return mal_next_free_block(blk);
        }
        return blk;
    }
    return NULL;
}



mal *mal_init(_mal_area *heap, u8 *start, s32 size) {
    mal *blk;
    mal *end;

    blk = (mal *)start;
    blk->size = ((u32)(size + 3) >> 2) - 12;
    blk->owner = NULL;
    blk->unk0 = 0;
    blk->prev = NULL;
    blk->nextFree = NULL;
    blk->prevFree = NULL;
    end = (mal *)((u32 *)(blk + 1) + blk->size);
    end->size = 0;
    end->owner = (void **)-1;
    end->unk0 = 0;
    end->prev = blk;
    *(mal **)&heap->unk0[0] = blk;
    heap->free = blk;
    heap->lowest = blk;
    *(mal **)&heap->unk0[4] = end;
    heap->deferred = NULL;
    return blk;
}

mal *mal_first_free(u32 words, mal *blk) {
    for (; blk != NULL; blk = blk->nextFree) {
        if (blk->size >= words) {
            return blk;
        }
    }
    return blk;
}

mal *mal_prev_movable(mal *arg0) {
    mal *blk;
    mal *p;

    blk = (mal *)arg0->owner;
    blk = blk->prev;
    for (; blk != NULL; blk = blk->prev) {
        if (blk->owner == NULL && (p = blk->prev) != NULL && p->owner != (void **)-1) {
            return blk;
        }
    }
    return blk;
}

void mal_compact_ptr(s32 arg0) {
    mal_compact_from(arg0 - 0x18, &MainHeap);
}

void mal_compact_step(_mal_area *heap) {
    mal *next;
    mal *low;

    low = heap->lowest;
    next = NULL;
    if (low != NULL && ((next = (mal *)((u8 *)low + (low->size * 4 + 0x18))),
                        next->owner == (void **)-1 || next->size == 0)) {
        next = NULL;
    }
    if (next != NULL && (next->unk0 & 2)) {
        next = NULL;
    }
    mal_compact_from(next, heap);
}

u8 *mal_update_deferred(_mal_area *arg0) {
    u16 n;
    mal *q;
    mal *prev;
    u8 *total;

    prev = NULL;
    q = arg0->deferred;
    total = NULL;
    if (q != NULL) {
        do {
            n = q->ticks - 1;
            q->ticks = n;
            if ((s16)n <= 0) {
                if (prev != NULL) {
                    prev->nextFree = q->nextFree;
                } else {
                    arg0->deferred = q->nextFree;
                }
                mal_free(q + 1, &MainHeap);
                q = NULL;
                if (prev != NULL) {
                    q = prev->nextFree;
                }
            } else {
                total += (s16)n;
                prev = q;
                q = q->nextFree;
            }
        } while (q != NULL);
    }
    return total;
}

mal *mal_next_movable(_mal_area *heap) {
    mal *next;

    next = Heap_AfterLowest(heap);
    if (next != NULL) {
        if (next->unk0 & 2) {
            return NULL;
        }
    }
    return next;
}

mal *mal_next_movable_now(_mal_area *heap) {
    mal *blk;
    mal *next;
    mal *ret;

    blk = heap->lowest;
    ret = NULL;
    if (blk != NULL) {
        next = (mal *)((u8 *)blk + (blk->size * 4 + sizeof(mal)));
        if (next->owner != (void **)-1) {
            ret = next;
            if (next->size == 0) {
                ret = NULL;
            }
        }
    }
    if (ret != NULL) {
        if ((ret->unk0 & 2) && (ret->unk0 & 1) == (FrameCount & 1)) {
            return NULL;
        }
    }
    return ret;
}

mal *mal_after_lowest(_mal_area *heap) {
    mal *blk;
    mal *next;

    blk = heap->lowest;
    if (blk == NULL) {
        return NULL;
    }
    next = (mal *)((u8 *)blk + (blk->size * 4 + sizeof(mal)));
    if (next->owner == (void **)-1 || next->size == 0) {
        return NULL;
    }
    return next;
}

void mal_move_down(mal *arg0, _mal_area *arg1) {
    mal *next;
    s32 new_var2;
    mal *prev;
    mal *new_var;
    mal *end;
    mal *nb;
    s32 *d;
    s32 *s;
    s32 n;
    s32 size;

    mal_unlink(arg0, arg1);
    size = arg0->size;
    next = (mal *)((u8 *)arg0 + (size * 4 + 0x18));
    new_var = arg0;
    n = next->size;
    prev = arg0->prev;
    d = (s32 *)((u8 *)arg0 + 0x18);
    *arg0 = *next;
    s = (s32 *)((u8 *)next + 0x18);
    new_var->prev = prev;
    end = (mal *)((u8 *)next + (n * 4 + 0x18));
    new_var2 = size;
    n--;
    *arg0->owner = (void *)d;
    if (n != -1) {
        do {
            *d++ = *s++;
            n--;
        } while (n != -1);
    }
    next = (mal *)((u8 *)arg0 + (arg0->size * 4 + 0x18));
    end->prev = next;
    next->size = new_var2;
    next->owner = (void **)-1;
    next->unk0 = 0;
    next->prev = arg0;
    if (arg0 == arg1->lowest) {
        arg1->lowest = mal_next_free_block(next);
    }
    mal_free(next + 1, arg1);
}

void mal_move_up(mal *arg0, _mal_area *arg1) {
    s32 size;
    mal *prev;
    mal *next;
    mal *dst;
    s32 *d;
    s32 *s;
    s32 n;

    size = arg0->size;
    prev = arg0->prev;
    next = (mal *)((u8 *)arg0 + (size * 4 + 0x18));
    if (prev != NULL) {
        mal_unlink(arg0, arg1);
        d = (s32 *)((u8 *)arg0 + (arg0->size * 4 + 0x14));
        n = prev->size;
        s = (s32 *)((u8 *)prev + (n * 4 + 0x14));
        dst = (mal *)((u8 *)d - n * 4 - 0x14);
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

u8 *loader_alloc(LoadNode *arg0) {
    _mal_area *h;
    s32 words;

    if (arg0->buf == NULL) {
        if (arg0->mode == 0) {
            return mal_alloc(((arg0->size + 0x7FF) / 0x800) << 11, &MainHeap);
        }
        words = ((arg0->size + 0x7FF) / 0x800) << 9;
        h = &MainHeap;
        if (mal_first_free(words, h->free) == NULL) {
            prims_empty_back();
            if (mal_first_free(words, h->free) == NULL) {
                mal_compact_step(h);
                if (mal_first_free(words, h->free) == NULL) {
                    return NULL;
                }
            }
        }
        return mal_alloc_nocompact(((arg0->size + 0x7FF) / 0x800) << 11, (u8 *)&MainHeap);
    }
    return arg0->buf;
}

s32 loader_update(void) {
    LoaderState *ls;
    LoadNode *node;
    u8 *buf;
    u8 *data;
    u8 *head;
    s32 r;

    ls = &Loader;
    if (ls->state != 0) {
        if (--ls->state == 0) {
            if (loader_pending() != 0) {
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
        r = cd_read_poll();
        if (r > 0) {
            return 0;
        }
        if (r < 0) {
            buf = (u8 *)ls->unk4;
            goto free;
        } else {
            node = ls->head;
            ls->head = node->next;
            node->next = LoadNodeFree;
            LoadNodeFree = node;
            if (ls->head != NULL) {
                (ls->head)->prev = NULL;
            } else {
                ls->tail = NULL;
            }
            buf = (u8 *)ls->unk4;
            ls->unk4 = 0;
            ls->state = 3;
            if (node->file == NULL) {
            free:
                mal_free(buf, &MainHeap);
            } else if (node->fn != NULL) {
                node->fn(buf, node->arg);
            } else {
                *(u8 **)node->arg = buf;
                if (node->mode == 1) {
                    mal_set_owner(buf, (u8 **)node->arg);
                } else {
                    *(u8 **)node->arg = buf;
                }
            }
        }
    }
    if (ls->state != 0) {
        return 0;
    }
    if (vab_loading() != 0) {
        return 0;
    }
    head = (u8 *)ls->head;
    if (head == NULL) {
        return 0;
    }
    node = (LoadNode *)head;
    data = loader_alloc(head);
    ls->unk4 = (s32)data;
    if (data == NULL) {
        return 0;
    }
    if (cd_read(node->file, node->offset / 2048, data, (node->size + 0x7FF) / 2048) < 0) {
        mal_free((void *)ls->unk4, &MainHeap);
        ls->unk4 = 0;
    }
    return 0;
}

SortNode *sortlist_bubble(SortList *arg0) {
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

LoadNode *loadlist_sort_by_sector(LoaderState *ls) {
    LoadNode *cur;
    LoadNode *nxt;
    LoadNode *t;

    cur = ls->head;
loop:
    if (cur != NULL) {
        nxt = cur->next;
        if (nxt == NULL) {
            goto end;
        }
        if (*nxt->file + nxt->offset / 2048 < *cur->file + cur->offset / 2048) {
            t = cur->prev;
            nxt->prev = t;
            if (t != NULL) {
                t->next = nxt;
            } else {
                ls->head = nxt;
            }
            t = nxt->next;
            cur->next = t;
            if (t != NULL) {
                t->prev = cur;
            } else {
                ls->tail = cur;
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

static inline s32 load_group_find(s32 *arg0, s32 *arg1) {
    s32 *base;
    s32 *p;
    s32 n;
    s32 end;
    s32 first;
    s32 *tbl;
    s32 end2;
    s32 i;

    tbl = DatIndex;
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

static inline void load_group_loop(u8 *arg0, s32 *arg1) {
    s32 first = arg1[0];
    s32 n = arg1[1];
    s32 *s2 = &DatIndex[first * 2];
    s32 *s0;

    if (n != 0) {
        n--;
        s0 = &DatIndex[(first + n) * 2];
        for (;;) {
            if (n == 0) {
                return;
            }
            mal_trim(mal_split_ptr(arg0, (s0[0] - s2[0]) << 11), s0[1], &MainHeap);
            n--;
            s0 -= 2;
        }
    }
}

void load_group_done(u8 *arg0, u8 *a1) {
    s32 **c;
    s32 *e;
    u8 *h;
    s32 i;

    c = *(s32 ***)(a1 + 4);
    if (c == NULL) {
        return;
    }
    load_group_loop(arg0, *(s32 **)a1);
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
        i = load_group_find(c[2], e);
        i <<= 11;
        dbase_group_textures_loaded(arg0 + i, h);
        i = load_group_find(c[1], e);
        i <<= 11;
        dbase_asload_with_anims_callback(arg0 + i, h);
        c += 3;
    }
}

static inline s32 load_dbase_find(s32 *arg0, s32 *arg1) {
    s32 *base;
    s32 *p;
    s32 n;
    s32 end;
    s32 first;
    s32 *tbl;
    s32 end2;

    tbl = DatIndex;
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

static inline void load_dbase_loop(u8 *arg0, s32 *arg1) {
    s32 first = arg1[0];
    s32 n = arg1[1];
    s32 *s2 = &DatIndex[first * 2];
    s32 *s0;

    if (n != 0) {
        n--;
        s0 = &DatIndex[(first + n) * 2];
        for (;;) {
            if (n == 0) {
                return;
            }
            mal_trim(mal_split_ptr(arg0, (s0[0] - s2[0]) << 11), s0[1], &MainHeap);
            n--;
            s0 -= 2;
        }
    }
}

void load_dbase_done(u8 *arg0, u8 *a1) {
    s32 *e;
    s32 **c;
    u8 *h;
    s32 i;

    load_dbase_loop(arg0, *(s32 **)a1);
    c = (s32 **)(a1 + 4);
    e = *(s32 **)a1;
    h = *(u8 **)(a1 + 4);
    i = load_dbase_find(c[2], e);
    i <<= 11;
    dbase_group_textures_loaded(arg0 + i, h);
    i = load_dbase_find(c[1], e);
    i <<= 11;
    dbase_asload_with_anims_callback(arg0 + i, h);
}

u8 *queue_dbase_load(u8 *arg0, s32 arg1) {
    LoadNode *p;
    LoadNode *next;
    LoadNode *tail;
    u8 *q;
    s32 a;
    s32 b;
    void (*fn)(u8 *, u8 *);
    LoaderState *ls;

    a = (s32)DatIndex + **(s32 **)arg0 * 8;
    *(s32 *)(*(u8 **)(arg0 + 4) + 0x34) = arg1;
    fn = load_dbase_done;
    ls = &Loader;
    b = *(s32 *)(*(u8 **)arg0 + 8);
    p = LoadNodeFree;
    if (p != NULL) {
        next = p->next;
        p->next = NULL;
        p->prev = ls->tail;
        LoadNodeFree = next;
        tail = ls->tail;
        if (tail != NULL) {
            tail->next = p;
        } else {
            ls->head = p;
        }
        ls->tail = p;
    }
    q = (u8 *)p;
    if (q == NULL) {
        printf(MsgTooManyLoadsInQueue);
    }
    p->file = (s32 *)a;
    p->offset = 0;
    p->mode = 1;
    p->size = b;
    p->fn = fn;
    p->arg = arg0;
    p->buf = NULL;
    return q;
}

u8 *queue_group_load(u8 *arg0) {
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
    a = (s32)DatIndex + **(s32 **)arg0 * 8;
    fn = load_group_done;
    ls = &Loader;
    b = *(s32 *)(*(u8 **)arg0 + 8);
    p = LoadNodeFree;
    if (p != NULL) {
        next = p->next;
        p->next = NULL;
        p->prev = ls->tail;
        LoadNodeFree = next;
        tail = ls->tail;
        if (tail != NULL) {
            tail->next = p;
        } else {
            ls->head = p;
        }
        ls->tail = p;
    }
    q = (u8 *)p;
    if (q == NULL) {
        printf(MsgTooManyLoadsInQueue);
    }
    p->file = (s32 *)a;
    p->offset = 0;
    p->mode = 1;
    p->size = b;
    p->fn = fn;
    p->arg = arg0;
    p->buf = NULL;
    return q;
}

static inline s32 dbase_files_find(s32 *arg0, s32 *arg1) {
    s32 *base;
    s32 *p;
    s32 n;
    s32 end;
    s32 first;
    s32 *tbl;
    s32 end2;

    tbl = DatIndex;
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

void dbase_files_loaded(s32 **arg0, s32 *arg1, u8 *arg2) {
    u8 *s2 = (u8 *)arg0[0];
    s32 i;

    i = dbase_files_find(arg0[2], arg1);
    i <<= 11;
    dbase_group_textures_loaded(arg2 + i, s2);
    i = dbase_files_find(arg0[1], arg1);
    i <<= 11;
    dbase_asload_with_anims_callback(arg2 + i, s2);
}

void group_split_buffer(void *arg0, s32 *arg1) {
    s32 first = arg1[0];
    s32 n = arg1[1];
    s32 *s2 = &DatIndex[first * 2];
    s32 *s0;

    if (n != 0) {
        n--;
        s0 = &DatIndex[(first + n) * 2];
        for (;;) {
            if (n == 0) {
                return;
            }
            mal_trim(mal_split_ptr(arg0, (s0[0] - s2[0]) << 11), s0[1], &MainHeap);
            n--;
            s0 -= 2;
        }
    }
}

s32 group_file_offset(s32 *arg0, s32 *arg1) {
    s32 *base;
    s32 *p;
    s32 n;
    s32 end;
    s32 first;
    s32 *tbl;
    s32 end2;

    tbl = DatIndex;
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

void sortlist_sort(s32 arg0) {
    do {

    } while (sortlist_bubble(arg0) != 0);
}

void loader_unused_a(void) {
}

void smallnode_free(BlockRef *node) {
    BlockRef *head;

    head = SmallNodeFree;
    SmallNodeFree = node;
    node->next = head;
}

void *smallnode_alloc(void) {
    BlockRef *p;

    p = SmallNodeFree;
    if (p != NULL) {
        p->ptr = NULL;
        p->load = 0;
        SmallNodeFree = p->next;
    }
    return p;
}


void smallnode_init(void) {
    BlockRef *p;
    s32 i;

    p = SmallNodePool;
    SmallNodeFree = p;
    for (i = 62; i != -1; i--) {
        p->next = p + 1;
        p++;
    }
    p->next = NULL;
}

void loader_unused_b(void) {
}

void init_file(void) {
    LoadNode *node;
    s32 i;

    cd_init();
    node = (LoadNode *)LoadNodePool;
    i = 0x3E;
    Loader.state = 3;
    Loader.unk4 = 0;
    Loader.unk8 = 0;
    Loader.head = NULL;
    Loader.tail = NULL;
    LoadNodeFree = LoadNodePool;
    do {
        LoadNode *next = node + 1;
        node->next = next;
        i--;
        node = next;
    } while (i != -1);
    node->next = 0;
    smallnode_init();
}

void loader_cancel(LoadNode *arg0) {
    LoaderState *ls;
    LoadNode *prev;
    void *temp_v1;

    if (arg0 == Loader.head && Loader.unk4 != 0) {
        *(s32 *)arg0 = 0;
        return;
    }
    ls = &Loader;
    prev = arg0->prev;
    if (prev != NULL) {
        prev->next = arg0->next;
    } else {
        ls->head = arg0->next;
    }
    arg0->next->prev = prev->next;
    if (arg0 == ls->tail) {
        ls->tail = prev;
    }
    temp_v1 = LoadNodeFree;
    LoadNodeFree = arg0;
    arg0->next = temp_v1;
}

void loadnode_free(LoadNode *node) {
    LoadNode *head;

    head = LoadNodeFree;
    LoadNodeFree = node;
    node->next = head;
}

void loadnode_unlink(LoadNode *arg0) {
    LoaderState *ls = &Loader;
    LoadNode *prev = arg0->prev;

    if (prev != NULL) {
        prev->next = arg0->next;
    } else {
        ls->head = arg0->next;
    }
    arg0->next->prev = prev->next;
    if (arg0 == ls->tail) {
        ls->tail = prev;
    }
}

s32 loader_pending(void) {
    LoaderState *p = &Loader;
    s32 a = p->head != NULL;
    s32 b = p->unk4;
    s32 s = p->state;
    int new_var;
    if (b == 0) {
        return a + s;
    }
    new_var = s + 1;
    return a + new_var;
}

s32 loader_queue(s32 arg0, s32 arg1, s32 arg2, void (*done)(u8 *, u8 *), u8 *arg4, s32 arg5, s32 arg6) {
    LoadNode *p;
    LoadNode *next;
    LoadNode *tail;
    u8 *q;
    LoaderState *ls = &Loader;

    p = LoadNodeFree;
    if (p != NULL) {
        next = p->next;
        p->next = NULL;
        p->prev = ls->tail;
        LoadNodeFree = next;
        tail = ls->tail;
        if (tail != NULL) {
            tail->next = p;
        } else {
            ls->head = p;
        }
        ls->tail = p;
    }
    q = (u8 *)p;
    if (q == NULL) {
        printf(MsgTooManyLoadsInQueue);
    }
    p->file = (s32 *)arg0;
    p->offset = arg1;
    p->mode = arg5;
    p->size = arg2;
    p->fn = done;
    p->arg = arg4;
    p->buf = (u8 *)arg6;
    return (s32)q;
}

LoadNode *loadlist_push(LoaderState *ls) {
    LoadNode *p;
    LoadNode *tail;
    LoadNode *next;

    p = LoadNodeFree;
    if (p != NULL) {
        next = p->next;
        p->next = NULL;
        p->prev = ls->tail;
        tail = ls->tail;
        LoadNodeFree = next;
        if (tail != NULL) {
            tail->next = p;
        } else {
            ls->head = p;
        }
        ls->tail = p;
    }
    return p;
}

LoadNode *loadlist_head(LoaderState *ls) {
    return ls->head;
}

LoadNode *loadlist_pop(LoaderState *ls) {
    LoadNode *node;
    LoadNode *next;
    LoadNode *head;

    head = LoadNodeFree;
    node = ls->head;
    ls->head = node->next;
    node->next = head;
    next = ls->head;
    LoadNodeFree = node;
    if (next != NULL) {
        next->prev = NULL;
    } else {
        ls->tail = NULL;
    }
    return node;
}




s32 mal_can_alloc(s32 arg0) {
    _mal_area *h = &MainHeap;
    s32 words;
    arg0 += 3;
    words = arg0 >> 2;
    if (mal_first_free(words, h->free) == NULL) {
        prims_empty_back();
        if (mal_first_free(words, h->free) == NULL) {
            mal_compact_step(h);
            if (mal_first_free(words, h->free) == NULL) {
                return 0;
            }
        }
    }
    return 1;
}

void loader_reset(void) {
    Loader.state = 3;
    Loader.unk4 = 0;
    Loader.unk8 = 0;
    Loader.head = NULL;
    Loader.tail = NULL;
}

void loadnode_init(void) {
    u8 *p;
    s32 i;

    p = LoadNodePool;
    LoadNodeFree = p;
    for (i = 62; i != -1; i--) {
        *(u8 **)(p + 0x1C) = p + 0x24;
        p += 0x24;
    }
    *(u8 **)(p + 0x1C) = NULL;
}

s32 load_file_wait(s32 *arg0, u8 **arg1) {
    s32 size;

    size = arg0[1];
    LoadWaitBuf = NULL;
    loader_queue((s32)arg0, 0, size, NULL, (u8 *)&LoadWaitBuf, 1, 0);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    mal_set_owner(LoadWaitBuf, arg1);
    return size;
}

s32 load_file_part_wait(s32 arg0, s32 arg1, u8 **arg2, s32 arg3) {
    LoadWaitBuf = NULL;
    loader_queue(arg0, arg1, arg3, NULL, (u8 *)&LoadWaitBuf, 1, 0);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    mal_set_owner(LoadWaitBuf, arg2);
    return 0;
}

s32 asload_chunk_relocatable(s32 arg0, s32 arg1, u32 **arg2, s32 arg3) {
    return loader_queue(arg0, arg1, arg3, NULL, arg2, 1, 0);
}

s32 load_file_permanent(s32 *arg0) {
    s32 a2 = arg0[1];

    if (a2 == -1) {
        return 0;
    }
    LoadPermanentBuf = 0;
    loader_queue((s32)arg0, 0, a2, NULL, (u8 *)&LoadPermanentBuf, 0, 0);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    return LoadPermanentBuf;
}

s32 load_file_to_wait(s32 arg0, s32 arg1, s32 arg2) {
    LoadPermanentBuf = 0;
    loader_queue(arg0, arg1, arg2, NULL, (u8 *)&LoadPermanentBuf, 0, 0);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    return LoadPermanentBuf;
}

s32 load_file_direct(s32 *a, u8 *b) {
    s32 r;
    s32 n;
    printf("load file start\n");
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    LoadDirectBusy = 0;
    while (cd_read_poll() > 0) {
    }
    do {
        for (;;) {
            n = a[1];
            if (cd_read(a, 0, b, (n + 0x7FF) / 2048) >= 0) {
                break;
            }
        }
        do {
            r = cd_read_poll();
        } while (r > 0);
    } while (r < 0);
    LoadDirectBusy = 1;
    printf("done loading\n");
    return 0;
}

s32 load_file(s32 *arg0, u8 *arg1) {
    return loader_queue((s32)arg0, 0, arg0[1], NULL, arg1, 1, 0);
}

s32 load_file_to(s32 *arg0, u8 *arg1) {
    return loader_queue((s32)arg0, 0, arg0[1], NULL, arg1, 0, 0);
}

s32 load_file_cb(s32 arg0, void (*done)(), void *ctx) {
    return loader_queue(arg0, 0, ((s32 *)arg0)[1], done, ctx, 1, 0);
}

s32 asload_permanent_callback(s32 arg0, void (*done)(), void *ctx) {
    return loader_queue(arg0, 0, ((s32 *)arg0)[1], done, ctx, 0, 0);
}

#define PS ((PadState *)pad)

void pad_read(PadState *pad) {
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
    PS->angle.w = get_angle(x, y);
    {
        s32 oldBtn = PS->lastHeld;
        s32 oldDir = PS->lastDir;
        s32 t;
        s32 u;
        PS->lastHeld = btn;
        PS->held = btn;
        PS->dir = dir;
        PS->lastDir = dir;
        u = btn ^ oldBtn;
        t = u & btn;
        PS->pressed = t;
        u = dir ^ oldDir;
        t = dir & u;
        PS->dirPressed = t;
    }
}

#undef PS

void pads_update(void) {
    PadState *pad;
    s32 any;
    s32 i;

    pads_read();
    pad = Pads;
    any = 0;
    PadRunEnd = 0;
    first_joypad = -1;
    for (i = 0; i < 8; pad++, i++) {
        pad_read(pad);
        if (pad->flags & 1) {
            if (first_joypad < 0) {
                first_joypad = i;
            }
            if (PadRunEnd == i) {
                PadRunEnd = i + 1;
            }
            any |= pad->buttons;
        } else {
            pad->buttons = 0;
        }
    }
    if (any != 0) {
        joypad_idle_count = 0;
    } else {
        joypad_idle_count++;
    }
}


void combo_check_all(void) {
    ComboSeq *c;
    s32 i;
    s32 m;

    c = (ComboSeq *)ComboSeqs;
    for (i = 0;; i++) {
        if (i > 0) {
            goto done;
        }
        m = c->steps[c->step];
        if (m != 0 && (((PadState *)Pads)->pressed & 0xFFFF)) {
            if ((((PadState *)Pads)->pressed & m) == m) {
                if (c->step == 0) {
                    c->startTime = FrameCount;
                }
                c->step++;
                if (c->steps[c->step] == 0) {
                    c->step = 0;
                    if (c->maxTime >= FrameCount - c->startTime) {
                        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                        c->fn();
                    }
                }
            } else {
                m = c->steps[0];
                if ((((PadState *)Pads)->pressed & m) == m) {
                    c->step = 1;
                    c->startTime = FrameCount;
                } else {
                    c->step = 0;
                }
            }
        }
        c++;
    }
done:;
}

void combo_check(ComboSeq *c) {
    s32 m;

    m = c->steps[c->step];
    if (m != 0 && (Pads[0].pressed & 0xFFFF)) {
        if ((Pads[0].pressed & m) == m) {
            if (c->step == 0) {
                c->startTime = FrameCount;
            }
            c->step++;
            if (c->steps[c->step] == 0) {
                c->step = 0;
                if (c->maxTime >= FrameCount - c->startTime) {
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                    c->fn();
                }
            }
        } else {
            m = c->steps[0];
            if ((Pads[0].pressed & m) == m) {
                c->step = 1;
                c->startTime = FrameCount;
                return;
            }
            c->step = 0;
        }
    }
}

void toggle_widescreen(void) {
    if (WideScreen != 0) {
        WideScreen = 0;
        screen_aspect[0] = 4;
        screen_aspect[1] = 3;
        return;
    }
    WideScreen = 1;
    screen_aspect[0] = 0x10;
    screen_aspect[1] = 9;
}

void cheat_menu_enable(void) {
    u32 *p = (u32 *)GameInfo;

    p[1] |= 0x80010000;
}

void set_joypad(PadState *pad, u32 buttons) {
    pad->forcedButtons = buttons;
    pad->forced = 1;
}

void pads_init(void) {
    PadState *pad;
    s32 i;

    pad = Pads;
    i = 7;
    do {
        pad->flags = 0;
        pad->buttons = 0;
        pad->held = 0;
        pad->pressed = 0;
        pad->angle.w = 0;
        pad->dist = 0;
        pad->forcedButtons = 0;
        pad++;
    } while (--i != -1);
    first_joypad = -1;
    joypad_idle_count = 0;
}

void pad_dpad_to_stick(s32 pad, u8 *out) {
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

void quat_to_matrix(SVECTOR *rot, MATRIX *m) {
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

void cam_ease_to(CamView *cam, CamView *dst, CamEase *ease) {
    if (ease != NULL) {
        if (cam_ramp_overide_count != 0) {
            ease = (CamEase *)CamEaseOverride;
            cam_ramp_overide_count--;
        }
        cam->dist += (dst->dist - cam->dist + 4) >> 2;
        cam->at.x += (dst->at.x - cam->at.x + (ease->at.x >> 1)) / ease->at.x;
        cam->at.y += (dst->at.y - cam->at.y + (ease->at.y >> 1)) / ease->at.y;
        cam->at.z += (dst->at.z - cam->at.z + (ease->at.z >> 1)) / ease->at.z;
        cam->eye.x += (dst->eye.x - cam->eye.x) / ease->eye.x;
        cam->eye.y += (dst->eye.y - cam->eye.y) / ease->eye.y;
        cam->eye.z += (dst->eye.z - cam->eye.z) / ease->eye.z;
        cam->rot.vx += angle_diff(cam->rot.vx, dst->rot.vx) / ease->rot.x;
        cam->rot.vy += angle_diff(cam->rot.vy, dst->rot.vy) / ease->rot.y;
        cam->rot.vz += angle_diff(cam->rot.vz, dst->rot.vz) / ease->rot.z;
    } else {
        cam_ramp_overide_count = 0;
        cam->dist = dst->dist;
        cam->at = dst->at;
        cam->rot = dst->rot;
        cam->eye = dst->eye;
    }
}

const char MsgTooManyLoadsInQueue[] = "too many loads in queue\n";
const char MsgSortScan[] = "SORT SCAN";
const char MsgSortScanSwappedEntry[] = "SORT SCAN SWAPPED ENTRY";
const char MsgSortScanSwappedNext[] = "SORT SCAN SWAPPED NEXT";

void rotscale_update(void *arg0) {
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
        quat_to_matrix(&p->rot, &p->mtx);
        break;
    case 4:
        break;
    }
    if (p->hasScale != 0) {
        ScaleMatrixL(&p->mtx, &p->scale);
    }
}

void cam_init(camera_info_tag *cam) {
    cam_ramp_overide_count = 0;
    cam->xform.scale.vx = 0x1000;
    cam->xform.scale.vy = 0x1000;
    cam->xform.scale.vz = 0x1000;
    cam->eye.z = 0x300;
    cam->dist = 0x140;
    cam->xform.angle.vx = 0;
    cam->xform.angle.vy = 0;
    cam->xform.angle.vz = 0;
    cam->at.x = 0;
    cam->at.y = 0;
    cam->at.z = 0;
    cam->unk95 = 0;
    cam->scaled = 1;
    cam->eye.x = 0;
    cam->eye.y = 0;
    cam->eye.z = 0;
    cam->shakeAmp = 0;
    cam->shakeTime = 0;
    cam->flipY = 0;
}

void cam_set_projection(camera_info_tag *cam, RECT *rect) {
    s32 t;
    s32 v;
    cam->xform.scale.vy = 0x1000;
    cam->xform.scale.vx = (screen_aspect[1] << 12) / screen_aspect[0];
    t = (cam->xform.scale.vy * rect->h) / 480;
    cam->xform.scale.vy = t;
    if (VideoMode == 1) {
        v = t * 0xF8;
    } else {
        v = t * 0xF0;
    }
    cam->xform.scale.vy = v / 240;
    cam->xform.scale.vx = (cam->xform.scale.vx * rect->w) / 480;
    cam->xform.scale.vz = 0x1000;
    if (cam->flipY != 0) {
        cam->xform.scale.vy = -cam->xform.scale.vy;
    }
}

void cam_ramp_override(s32 arg0, s32 arg1) {
    cam_ramp_overide_count = arg1;
    CamEaseOverride = arg0;
}

void cam_build_view(camera_info_tag *cam, RECT *rect) {
    s32 t;
    s32 y;
    u16 a;
    u16 b;
    u16 c;

    t = cam->shakeTime;
    if (t != 0) {
        cam->shakeTime = t - 1;
    }
    cam_set_projection(cam, rect);
    RotMatrix(&cam->xform.angle, &cam->view);
    if (cam->scaled != 0) {
        ScaleMatrixL(&cam->view, &cam->xform.scale);
    }
    a = *(u16 *)&cam->xform.scale.vx;
    b = *(u16 *)&cam->xform.scale.vy;
    c = *(u16 *)&cam->xform.scale.vz;
    y = cam->eye.z;
    cam->view.t[0] = cam->eye.x;
    cam->rot.m[0][1] = 0;
    cam->rot.m[0][2] = 0;
    cam->rot.m[1][0] = 0;
    cam->rot.m[1][2] = 0;
    cam->view.t[1] = cam->eye.y;
    cam->rot.m[2][0] = 0;
    cam->rot.m[2][1] = 0;
    cam->rot.t[0] = 0;
    cam->rot.t[1] = 0;
    cam->rot.t[2] = 0;
    cam->view.t[0] = cam->view.t[0] * cam->xform.scale.vx;
    cam->view.t[1] = cam->view.t[1] * cam->xform.scale.vy;
    cam->rot.m[0][0] = a;
    cam->rot.m[1][1] = b;
    cam->rot.m[2][2] = c;
    cam->view.t[2] = y;
    cam->view.t[0] = cam->view.t[0] >> 12;
    cam->view.t[1] = cam->view.t[1] >> 12;
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

void export_exe_functions(void) {
    void **t = (void **)ExportTable;

    t[0x000 / 4] = InitDisplay;
    t[0x004 / 4] = &num_sfx_banks;
    t[0x008 / 4] = PlaySfx;
    t[0x00C / 4] = StopSfx;
    t[0x010 / 4] = sfx_bank_load;
    t[0x014 / 4] = sound_reinit;
    t[0x018 / 4] = sfx_delete_all;
    t[0x01C / 4] = sfx_set_pitch;
    t[0x020 / 4] = sfx_bank_close;
    t[0x024 / 4] = sound_pause;
    t[0x028 / 4] = sound_resume;
    t[0x02C / 4] = music_play_level;
    t[0x030 / 4] = music_request;
    t[0x034 / 4] = sound_init;
    t[0x038 / 4] = sound_set_mono;
    t[0x03C / 4] = sound_set_stereo;
    t[0x040 / 4] = music_fade_out;
    t[0x044 / 4] = path_find;
    t[0x048 / 4] = path_get;
    t[0x04C / 4] = key_interp_pos;
    t[0x050 / 4] = key_find;
    t[0x054 / 4] = draw_map_scene;
    t[0x058 / 4] = draw_map_sub;
    t[0x05C / 4] = map_update;
    t[0x060 / 4] = mapscene_init;
    t[0x064 / 4] = mapscene_load;
    t[0x068 / 4] = mapscene_release;
    t[0x06C / 4] = mapscene_place_subs;
    t[0x070 / 4] = prims_init;
    t[0x074 / 4] = face_prim_alloc;
    t[0x078 / 4] = primnode_alloc;
    t[0x07C / 4] = primset_alloc;
    t[0x080 / 4] = prims_empty;
    t[0x084 / 4] = faces_write_colors;
    t[0x088 / 4] = faces_write_uvs_colors;
    t[0x08C / 4] = primpool_init;
    t[0x090 / 4] = primpool_free;
    t[0x094 / 4] = release_sprim;
    t[0x098 / 4] = new_sprim;
    t[0x09C / 4] = draw_gouraud_quad;
    t[0x0A0 / 4] = draw_panel_rect;
    t[0x0A4 / 4] = sub_play_script;
    t[0x0A8 / 4] = draw_model_quad;
    t[0x0AC / 4] = draw_nas_model_flags;
    t[0x0B0 / 4] = draw_nas_model;
    t[0x0B4 / 4] = draw_text_nas;
    t[0x0B8 / 4] = sub_use_quad_model;
    t[0x0BC / 4] = kill_sprim_sub;
    t[0x0C0 / 4] = model_entry_texture;
    t[0x0C4 / 4] = model_entry_data;
    t[0x0C8 / 4] = SetDrawArea;
    t[0x0CC / 4] = tga_convert;
    t[0x0D0 / 4] = eng_add_subs;
    t[0x0D4 / 4] = eng_kill;
    t[0x0D8 / 4] = eng_render;
    t[0x0DC / 4] = subpool_init;
    t[0x0E0 / 4] = eng_add_subs_n;
    t[0x0E4 / 4] = subs_init_array;
    t[0x0E8 / 4] = draw_init;
    t[0x0EC / 4] = gfx_set_viewport;
    t[0x0F0 / 4] = entity_address;
    t[0x0F4 / 4] = init_dots;
    t[0x0F8 / 4] = plot_point_sprites;
    t[0x0FC / 4] = fx_alloc_buffers;
    t[0x100 / 4] = fx_free_buffers;
    t[0x104 / 4] = calc_rgbcode;
    t[0x108 / 4] = cam_init;
    t[0x10C / 4] = cam_ease_to;
    t[0x110 / 4] = ResetScreen;
    t[0x114 / 4] = vram_init;
    t[0x118 / 4] = load_font;
    t[0x11C / 4] = dbase_asload_with_anims;
    t[0x120 / 4] = AllocOtBuffers;
    t[0x124 / 4] = dbase_init;
    t[0x128 / 4] = dbase_load_textures_wait;
    t[0x12C / 4] = dbase_init_all;
    t[0x130 / 4] = (u8 *)HeroModelBank;
    t[0x134 / 4] = MulMatrix;
    t[0x138 / 4] = pad_rumble_small;
    t[0x13C / 4] = pad_rumble_big;
    t[0x140 / 4] = debug_font_init2;
    t[0x144 / 4] = dbase_mesh_anim_name_load;
    t[0x148 / 4] = dbase_mesh_anim_name_loaded;
    t[0x14C / 4] = dbase_mesh_anim_name_release;
    t[0x150 / 4] = dbase_load_all_mesh_anims;
    t[0x154 / 4] = dbase_mesh_anim_name_size;
    t[0x158 / 4] = dbase_anim_release;
    t[0x15C / 4] = dbase_anim_ptr;
    t[0x160 / 4] = dbase_anim_load;
    t[0x164 / 4] = set_joypad;
    t[0x168 / 4] = combo_check_all;
    t[0x16C / 4] = sub_find_point;
    t[0x170 / 4] = sortlist_sort;
    t[0x174 / 4] = load_file_to;
    t[0x178 / 4] = mal_update_deferred;
    t[0x17C / 4] = sub_nth;
    t[0x180 / 4] = load_file_permanent;
    t[0x184 / 4] = load_file_direct;
    t[0x188 / 4] = mapenv_place_subs;
    t[0x18C / 4] = mapscene_find_sub;
    t[0x190 / 4] = entry_block_value;
    t[0x194 / 4] = gte_rotate_point;
    t[0x198 / 4] = fade_update;
    t[0x19C / 4] = set_fade;
    t[0x1A0 / 4] = process_init;
    t[0x1A4 / 4] = process_control_update;
    t[0x1A8 / 4] = process_control_render;
    t[0x1AC / 4] = process_quit;
    t[0x1B0 / 4] = process_control_init;
    t[0x1B4 / 4] = process_control_goto;
    t[0x1B8 / 4] = process_interrupt;
    t[0x1BC / 4] = process_control_back;
    t[0x1C0 / 4] = texture_draw_quad;
    t[0x1C4 / 4] = texture_release_loading_mem;
    t[0x1C8 / 4] = card_poll;
    t[0x1CC / 4] = music_load_track;
    t[0x1D0 / 4] = texture_draw_3d;
    t[0x1D4 / 4] = SysPrintAt3d;
    t[0x1D8 / 4] = DrawSysText;
    t[0x1DC / 4] = card_save;
    t[0x1E0 / 4] = card_reset;
    t[0x1E4 / 4] = quat_to_matrix;
    t[0x1E8 / 4] = init_scripts;
    t[0x1EC / 4] = scripts_update;
    t[0x1F0 / 4] = scripts_render;
    t[0x1F4 / 4] = scripts_release;
    t[0x1F8 / 4] = scripts_kill_all;
    t[0x1FC / 4] = play_script;
    t[0x200 / 4] = get_script_header;
    t[0x204 / 4] = offset_table_entry;
    t[0x208 / 4] = get_script_track;
    t[0x20C / 4] = gfx_object_matrix;
    t[0x210 / 4] = emit_particle;
    t[0x214 / 4] = vel_update;
    t[0x218 / 4] = interp;
    t[0x21C / 4] = lerp_angle;
    t[0x220 / 4] = lerp_fix12;
    t[0x224 / 4] = posdst_flash_at;
    t[0x228 / 4] = posdst_flash;
    t[0x22C / 4] = CurrentModule;
    t[0x230 / 4] = LangModeTypeTexts;
    t[0x234 / 4] = card_clear_slots;
    t[0x238 / 4] = music_set_tempo;
    t[0x23C / 4] = music_restart;
    t[0x240 / 4] = music_flag_restart;
    t[0x244 / 4] = music_duck_off;
    t[0x248 / 4] = vab_loading;
    t[0x24C / 4] = vab_transfer_wait;
    t[0x250 / 4] = quat_slerp;
    t[0x254 / 4] = (void *)VabCueTimer;
    t[0x258 / 4] = (void *)VabCuePending;
    t[0x25C / 4] = itoa_div;
    t[0x260 / 4] = shell_run_spyro_demo;
    t[0x264 / 4] = dbase_texture_ptr;
    t[0x268 / 4] = dbase_texture_entry;
    t[0x26C / 4] = card_format;
    t[0x270 / 4] = pads_reset_rumble;
    t[0x274 / 4] = modchip_check_region;
    t[0x278 / 4] = modchip_step;
    t[0x27C / 4] = queue_group_load;
    t[0x280 / 4] = FileGroups;
    t[0x284 / 4] = subpool_free;
    t[0x288 / 4] = kill_script_threads;
    t[0x28C / 4] = kill_script;
}

void itoa_div(char *dst, s32 val, s32 div) {
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

char *itoa_div_end(char *arg0, s32 arg1, s32 arg2) {
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

s32 lerp_snap(s32 a, s32 b, s32 t) {
    s32 d;

    d = b - a;
    if (abs(d) < 3) {
        return b;
    }
    d = d * t;
    d = d / 4096;
    return a + d;
}

s32 lerp_fix12(s32 a, s32 b, s32 t) {
    b = (b - a) * t;
    b /= 0x1000;
    return a + b;
}

s32 lerp_angle(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
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

s32 string_hash(u8 *s) {
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

s32 bit_count(u32 x) {
    s32 bits;

    bits = 0;
    while (x != 0) {
        x >>= 1;
        bits++;
    }
    return bits;
}

s32 *get_exe_exports(void) {
    return ExportTable;
}

s32 get_dist_angles(s32 *a, s32 *b, s16 *out) {
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

s32 angle_diff_abs(s32 arg0, s32 arg1) {
    s32 var_a0;

    var_a0 = (arg1 - arg0) & 0xFFF;
    if (var_a0 >= 0x801) {
        var_a0 = 0x1000 - var_a0;
    }
    return var_a0;
}

s32 angle_diff(s32 arg0, s32 arg1) {
    s32 var_a0;

    var_a0 = (arg1 - arg0) & 0xFFF;
    if (var_a0 >= 0x801) {
        var_a0 -= 0x1000;
    }
    return var_a0;
}

void clear_bit15(u16 *arg0, s32 arg1) {
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

s32 rand_step(void) {
    RandomSeedA += RandomSeedB;
    RandomSeedB += RandomSeedA + ((u32)RandomSeedA >> 31);
    return RandomSeedA;
}

PointList *sub_point_list(subengine *sub) {
    s32 flag;
    u8 *base;
    u8 *rec;
    s32 off;

    flag = sub->entity;
    if (flag != 0) {
        if (sub->u6C.dbase == NULL) {
            return NULL;
        }
        base = *(u8 **)((u8 *)sub->u6C.dbase + 0xC);
        if (base == NULL) {
            return NULL;
        }
        switch (flag & 0x7000) {
        case 0x3000:
            break;
        case 0x4000:
            rec = dbase_anim_frame(flag, base + (*(s32 *)(base + 0x44) + 0x44) + ((flag & 0xF80) >> 7) * 0x18);
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

void gte_rotate_point(SVECTOR *v, VECTOR *out) {
    gte_ldv0(v);
    gte_rt();
    gte_stlvl(out);
}

FlagPoint *sub_find_point(EXTengine *eng, subengine *sub, u16 flag, VECTOR *out) {
    PointList *list = sub_point_list(sub);
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
                rotscale_update(&eng->unk1C);
            }
            gfx_load_xform(sub, eng->unk3C, &eng->unk10);
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

u8 *model_entry_data(u8 *arg0, s32 arg1) {
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

s32 model_entry_texture(u8 *arg0, s32 arg1) {
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

s32 entry_block_value(s32 i, EntryBlock *blk) {
    s32 idx = i;

    u8 *base;

    if (blk == NULL) {
        return -1;
    }
    base = (u8 *)blk + (blk->entries + 0x1C);
    idx = (idx - 1) & 0xFFF;
    return *(s32 *)(base + idx * 12 + 0xC);
}

s32 map_entity_address(u16 arg0, u8 *arg1) {
    u8 *base;
    s32 t;
    s32 r;

    arg0--;
    t = arg0 & 0xFFF;
    base = arg1 + (*(s32 *)(arg1 + 0x1C) + 0x1C);
    r = rel_ptr((s32 *)(base + (t * 0xC + 4)));
    if (r == 0) {
        return 0;
    }
    return r;
}

u8 *entity_address(u32 arg0, u8 *arg1) {
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

u8 *dbase_anim_frame(s32 i, u8 *rec) {
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

void *dbase_anim_node(s32 *arg0, u8 *rec) {
    void **node;

    node = *(void ***)(rec + 0x14);
    if (node == NULL) {
        return NULL;
    }
    return *node;
}

void init_dots(subengine *arg0) {
    SubFx *fx = (SubFx *)&arg0->u6C;
    s32 n = fx->count;
    TILE_1 *a = fx->a;
    TILE_1 *b = fx->b;
    s16 *vel = arg0->u6C.ptr;

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

void plot_point_sprites(PointSprites *ps, MATRIX *m) {
    PointSpriteSet *set = &ps->set;
    PointSpriteSrc *src = set->src;
    TILE_1 *prim = set->prims[FrameCount & 1];
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
        addPrim(RenderState.ot, prim);
        prim++;
    }
}
void fx_free_buffers(subengine *sub) {
    SubFx *fx;

    fx = (SubFx *)&sub->u6C;
    mal_free_later(fx->a, &MainHeap, 2);
    mal_free_later(fx->b, &MainHeap, 2);
    mal_free(sub->u6C.ptr, &MainHeap);
}

s32 fx_alloc_buffers(FxDef *def, subengine *sub) {
    SubFx *fx;
    void **a;
    s32 var_v0;

    a = (void **)&((SubFx *)&sub->u6C)->a;
    fx = (SubFx *)&sub->u6C;
    fx->a = NULL;
    fx->b = NULL;
    sub->u6C.ptr = NULL;
    fx->count = 0;
    mal_alloc_owned_low(def->size, a, (s32)&MainHeap, 2);
    if (fx->a == NULL) {
        var_v0 = 0;
    } else {
        mal_alloc_owned_low(def->size, &((SubFx *)&sub->u6C)->b, (s32)&MainHeap, 3);
        if (fx->b == NULL) {
            mal_free(a, &MainHeap);
            var_v0 = 0;
        } else {
            mal_alloc_owned_low(def->bufSize, &fx->buf, (s32)&MainHeap, 3);
            fx->count = def->count;
            var_v0 = (s32)sub->u6C.ptr;
        }
    }
    if (var_v0 != 0) {
        void (*hook)() = def->hook;
        sub->release = fx_free_buffers;
        sub->hook = (void (*)(subengine *, s32, u32))hook;
        def->init(sub);
    } else {
        sub->hook = NULL;
        sub->release = NULL;
    }
    return ((SubFx *)&sub->u6C)->count;
}

s32 fx_alloc_buffers_b(FxDef *def, SubFx *fx) {
    void **a;

    a = &fx->a;
    fx->a = NULL;
    fx->b = NULL;
    fx->buf = NULL;
    fx->count = 0;
    mal_alloc_owned_low(def->size, a, (s32)&MainHeap, 2);
    if (fx->a != NULL) {
        mal_alloc_owned_low(def->size, &fx->b, (s32)&MainHeap, 3);
        if (fx->b != NULL) {
            mal_alloc_owned_low(def->bufSize, &fx->buf, (s32)&MainHeap, 3);
            fx->count = def->count;
            return (s32)fx->buf;
        }
        mal_free(a, &MainHeap);
        goto fail;
    }
fail:
    return 0;
}

/* data, in address order (tools/scripts/data2c.py) */
s32 LevelNames[45] = {
    (s32)&LevNameJungleBash, (s32)&LevNameSpaceBash, (s32)&LevNameSnowBash,
    (s32)&LevNameDrainBash, (s32)&LevNamePapuPummel, (s32)&LevNameSwampFox,
    (s32)&LevNameDesertFox, (s32)&LevNameJungleFox, (s32)&LevNameMetalFox,
    (s32)&LevNameBigBadFox, (s32)&LevNamePogoPainter, (s32)&LevNamePogoAGogo,
    (s32)&LevNameElPogoLoco, (s32)&LevNamePogoPadlock, (s32)&LevNameCrashball,
    (s32)&LevNameBeachBall, (s32)&LevNameNBallism, (s32)&LevNameSkyBalls,
    (s32)&LevNameTiltPanic, (s32)&LevNamePolarPanic, (s32)&LevNameMeltPanic,
    (s32)&LevNameManicPanic, (s32)&LevNameBearminator, (s32)&LevNameDotDash,
    (s32)&LevNameToxicDash, (s32)&LevNameSplashDash, (s32)&LevNameDantesDash,
    (s32)&LevNameRingDing, (s32)&LevNameDragonDrop, (s32)&LevNameMalletMash,
    (s32)&LevNameKegKaboom, (s32)&LevNameOxideRide, (s32)&LevNameTieBreaker,
    (s32)&LevNameWarpRoom1, (s32)&LevNameWarpRoom2, (s32)&LevNameWarpRoom3,
    (s32)&LevNameWarpRoom4, (s32)&LevNameSecretWarpRoom, (s32)&LevNameDemoHub,
    (s32)&LevNameDemoHub, (s32)&LevNameTitles, (s32)&LevNameEmpty, (s32)&LevNameEmpty,
    (s32)&LevNamePodium, (s32)&LevNameScriptStuff,
};
s32 *LangLevelNames[5] = { LevelNames, LevelNames, LevelNames, LevelNames, LevelNames };
s32 ModeTypeNames[14] = {
    0, 0, (s32)&LevNameMedievalMayhem, (s32)&LevNameCrateCrush, 0, (s32)&LevNameTankWars,
    (s32)&LevNamePogoPandemonium, (s32)&LevNameBallistix, (s32)&LevNamePolarPush,
    (s32)&LevNameCrashDash, (s32)&LevNameMedievalMayhem, 0, (s32)&LevNameMedievalMayhem,
    (s32)&LevNameMedievalMayhem,
};
s32 *LangModeTypeTexts[5] = { ModeTypeNames, ModeTypeNames, ModeTypeNames, ModeTypeNames, ModeTypeNames };
u16 ModeTypes[136] = {
    0x104, 0, 4, 0x104, 1, 5, 0x104, 2, 6, 0x104, 3, 7, 0x105, 0, 0, 0x106, 0, 5, 0x106, 1, 6,
    0x106, 2, 7, 0x106, 3, 8, 0x106, 4, 4, 0x107, 0, 4, 0x107, 1, 5, 0x107, 2, 6, 0x107, 3, 7,
    0x108, 0, 4, 0x108, 1, 5, 0x108, 2, 6, 0x108, 3, 7, 0x109, 0, 6, 0x109, 1, 9, 0x109, 2, 8,
    0x109, 3, 7, 0x109, 4, 4, 0x10A, 0, 6, 0x10A, 1, 4, 0x10A, 2, 7, 0x10A, 3, 5, 0x103, 0, 1,
    0x10B, 0, 1, 0x10E, 0, 1, 0x10D, 0, 1, 0x10C, 0, 1, 0x104, 8, 8, 0x102, 0, 0, 0x102, 1, 1,
    0x102, 2, 2, 0x102, 3, 3, 0x102, 4, 4, 0x102, 0xD, 0xD, 0x102, 0xE, 0xE, 0x101, 0, 0, 0x101, 0, 0,
    0x101, 0, 0, 0x102, 3, 3, 0x109, 5, 5, 0,
};
u16 LevelRequirements[234] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0, 0x17, 0x1F, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4,
    0, 0, 0, 0, 4, 0xF, 0, 0xA, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 6, 3, 0, 0, 0, 0, 0, 0x16,
    0, 0, 0, 0, 0x16, 0, 0x11, 0, 0, 0x16, 0x1B, 0, 0, 0, 0x16, 0, 0, 0, 0, 9,
    0, 0, 0, 0x13, 0x1F, 0, 0x15, 0, 0, 0x1F, 0, 0, 0x19, 0, 0x1F, 0x16, 0, 0xF, 0xC, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0x16,
    0, 0, 0, 0, 9, 0, 0, 0, 0, 0x1F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0x1FF, 0x201, 0x102, 0x102, 2, 0, 0, 0,
};
s32 DcacheReady = 0;
ModuleEntry ShellModules = { shell_start_engine, shell_update, shell_render_dcache, NULL, NULL };
s16 core_process = 0;
s32 CompletionBcd = 0;
s32 GraphicsMode = 0;
s32 GameFrameCount = 0;
ModuleEntry *EngineModule = 0;
s32 VSyncRate = 0;
s32 LastFlipVSync = 0;
s32 MalMoveVSyncs = 0;
s32 MalMoveWords = 0;
_mal_area MainHeap = {0};
s32 MalFreeBytes = 0;
s32 MalFreeBlocks = 0;
s32 MalLargestFree = 0;
/* The archive's index: {first sector, size in bytes} for each of CRASHBSH.DAT's 992 files,
 * file N at DatIndex[2 * N] (group_file_offset). */
s32 DatIndex[2 * 992] = {
    0x0, 0x3770, 0x7, 0x1FB2A, 0x47, 0x3770, 0x4E, 0x3E33C, /* 0 */
    0xCB, 0x3770, 0xD2, 0x2992C, 0x126, 0x3770, 0x12D, 0x293A2, /* 4 */
    0x180, 0x2AD8, 0x186, 0x19F78, 0x1BA, 0x2AD8, 0x1C0, 0x1DD82, /* 8 */
    0x1FC, 0x2AD8, 0x202, 0x1B760, 0x239, 0x2AD8, 0x23F, 0x18F66, /* 12 */
    0x271, 0x48C0, 0x27B, 0x18FFA, 0x2AE, 0x48C0, 0x2B8, 0x17FF6, /* 16 */
    0x2E9, 0x4240, 0x2F2, 0x1E8, 0x2F3, 0x4240, 0x2FC, 0x1E8, /* 20 */
    0x2FD, 0x4240, 0x306, 0x1E8, 0x307, 0x4240, 0x310, 0x1E8, /* 24 */
    0x311, 0x4240, 0x31A, 0x1E8, 0x31B, 0xA878, 0x331, 0x346FE, /* 28 */
    0x39A, 0xA878, 0x3B0, 0x42F44, 0x436, 0xA878, 0x44C, 0x2BD6E, /* 32 */
    0x4A4, 0xA878, 0x4BA, 0x20010, 0x4FB, 0xD9D8, 0x517, 0x1EFEA, /* 36 */
    0x556, 0xAB98, 0x56C, 0xD528, 0x587, 0xB5D8, 0x59E, 0x1DDB2, /* 40 */
    0x5DA, 0x5260, 0x5E5, 0xAF24, 0x5FB, 0x90B8, 0x60E, 0x14AFA, /* 44 */
    0x638, 0x8258, 0x649, 0x198A2, 0x67D, 0xB390, 0x694, 0x10BFA, /* 48 */
    0x6B6, 0xCA00, 0x6D0, 0x50230, 0x771, 0xB0E8, 0x788, 0x1AB0A, /* 52 */
    0x7BE, 0x8A90, 0x7D0, 0x14430, 0x7F9, 0x9B28, 0x80D, 0x22DAC, /* 56 */
    0x853, 0xBF20, 0x86B, 0x48116, 0x8FC, 0xB088, 0x913, 0x299D6, /* 60 */
    0x967, 0xB088, 0x97E, 0x219AC, 0x9C2, 0xB088, 0x9D9, 0x836FE, /* 64 */
    0xAE0, 0xC8B8, 0xAFA, 0x36882, 0xB68, 0x86C8, 0xB79, 0x195FC, /* 68 */
    0xBAC, 0xAE88, 0xBC2, 0x1C0B4, 0xBFB, 0xB3D8, 0xC12, 0x35080, /* 72 */
    0xC7D, 0xABD0, 0xC93, 0x284F6, 0xCE4, 0xADD0, 0xCFA, 0x25ACC, /* 76 */
    0xD46, 0xE728, 0xD63, 0x4A4C0, 0xDF8, 0xE130, 0xE15, 0x2DC3E, /* 80 */
    0xE71, 0xD728, 0xE8C, 0x3E432, 0xF09, 0xBBA0, 0xF21, 0x36D66, /* 84 */
    0xF8F, 0xDAE0, 0xFAB, 0x371C0, 0x101A, 0xBBA0, 0x1032, 0x3C12E, /* 88 */
    0x10AB, 0xE510, 0x10C8, 0x3C194, 0x1141, 0xDF10, 0x115D, 0x4146C, /* 92 */
    0x11E0, 0xCC28, 0x11FA, 0x2B3A8, 0x1251, 0xEA38, 0x126F, 0x32E74, /* 96 */
    0x12D5, 0xEAF0, 0x12F3, 0x354C8, 0x135E, 0x143E8, 0x1387, 0x6523C, /* 100 */
    0x1452, 0xAEC0, 0x1468, 0x255DC, 0x14B3, 0x8B28, 0x14C5, 0x176C4, /* 104 */
    0x14F4, 0xAFB0, 0x150A, 0xE356, 0x1527, 0xCC38, 0x1541, 0x261D2, /* 108 */
    0x158E, 0xB168, 0x15A5, 0x1EBBC, 0x15E3, 0xBCE8, 0x15FB, 0x25F18, /* 112 */
    0x1647, 0x92C8, 0x165A, 0x110A0, 0x167D, 0xAB98, 0x1693, 0x26D02, /* 116 */
    0x16E1, 0xF6B0, 0x1700, 0x18BFE, 0x1732, 0x1C78, 0x1736, 0xFFF4, /* 120 */
    0x1757, 0xA650, 0x176C, 0xAEDC, 0x1782, 0x35328, 0x17ED, 0x14FC4, /* 124 */
    0x1817, 0x27778, 0x1866, 0x1F3E4, 0x18A5, 0x1EA50, 0x18E3, 0x438E8, /* 128 */
    0x196B, 0xE0, 0x196C, 0x43E8, 0x1975, 0xF88, 0x1977, 0x764, /* 132 */
    0x1978, 0x4002C, 0x19F9, 0x3C02C, 0x1A72, 0x4002C, 0x1AF3, 0x4002C, /* 136 */
    0x1B74, 0x4002C, 0x1BF5, 0x2CD0, 0x1BFB, 0x1880, 0x1BFF, 0x2230, /* 140 */
    0x1C04, 0x2898, 0x1C0A, 0x3A70, 0x1C12, 0x7E4, 0x1C14, 0x574, /* 144 */
    0x1C15, 0x15C4, 0x1C18, 0x180C, 0x1C1C, 0x1858, 0x1C20, 0x3E54, /* 148 */
    0x1C28, 0x23A4, 0x1C2D, 0x16550, 0x1C5A, 0x2AD20, 0x1CB0, 0x22EB2, /* 152 */
    0x1CF6, 0x2B82C, 0x1D4E, 0x1B4BC, 0x1D85, 0x317E2, 0x1DE9, 0x37DF0, /* 156 */
    0x1E59, 0x35A42, 0x1EC5, 0x2EEEC, 0x1F23, 0x36264, 0x1F90, 0x233C6, /* 160 */
    0x1FD7, 0x26212, 0x2024, 0x2A25A, 0x2079, 0x29F2C, 0x20CD, 0x2A1BC, /* 164 */
    0x2122, 0x36B74, 0x2190, 0x36B2A, 0x21FE, 0x2AE4C, 0x2254, 0x28D5A, /* 168 */
    0x22A6, 0x35C10, 0x2312, 0x31A8A, 0x2376, 0x35722, 0x23E1, 0x1EDB4, /* 172 */
    0x241F, 0x23BF8, 0x2467, 0x29B4A, 0x24BB, 0x25AF4, 0x2507, 0x29494, /* 176 */
    0x255A, 0x28E42, 0x25AC, 0x33870, 0x2614, 0x54F6, 0x261F, 0x57B6, /* 180 */
    0x262A, 0x57A6, 0x2635, 0x57D6, 0x2640, 0x5826, 0x264C, 0x4CF6, /* 184 */
    0x2656, 0x5846, 0x2662, 0x4F06, 0x266C, 0x6966, 0x267A, 0x6648, /* 188 */
    0x2687, 0x6058, 0x2694, 0x57D6, 0x269F, 0x5FB8, 0x26AB, 0x4CF6, /* 192 */
    0x26B5, 0x64E8, 0x26C2, 0x4F06, 0x26CC, 0x9C80, 0x26E0, 0x9052, /* 196 */
    0x26F3, 0x9170, 0x2706, 0x9BD0, 0x271A, 0x9A42, 0x272E, 0x8D52, /* 200 */
    0x2740, 0x9302, 0x2753, 0x8E40, 0x2765, 0x9896, 0x2779, 0x9748, /* 204 */
    0x278C, 0x9596, 0x279F, 0x8AB6, 0x27B1, 0x9D78, 0x27C5, 0x83A6, /* 208 */
    0x27D6, 0x9BB8, 0x27EA, 0x8896, 0x27FC, 0x21750, 0x283F, 0x22ED0, /* 212 */
    0x2885, 0x279C0, 0x28D5, 0x2B700, 0x292C, 0x2DFD0, 0x2988, 0x29A90, /* 216 */
    0x29DC, 0x279C0, 0x2A2C, 0x2B700, 0x2A83, 0x3A180, 0x2AF8, 0x24760, /* 220 */
    0x2B41, 0x2D780, 0x2B9C, 0x2D780, 0x2BF7, 0x1F680, 0x2C36, 0x1F680, /* 224 */
    0x2C75, 0x1F680, 0x2CB4, 0x1F680, 0x2CF3, 0x1F680, 0x2D32, 0x1F680, /* 228 */
    0x2D71, 0x1F680, 0x2DB0, 0x1F680, 0x2DEF, 0x1F680, 0x2E2E, 0x1F680, /* 232 */
    0x2E6D, 0x1F680, 0x2EAC, 0x1F680, 0x2EEB, 0x1F680, 0x2F2A, 0x1F680, /* 236 */
    0x2F69, 0x1F680, 0x2FA8, 0x1F680, 0x2FE7, 0x1F680, 0x3026, 0x1F680, /* 240 */
    0x3065, 0x1F680, 0x30A4, 0x1F680, 0x30E3, 0x1F680, 0x3122, 0x1F680, /* 244 */
    0x3161, 0x1F680, 0x31A0, 0x1F680, 0x31DF, 0x1F680, 0x321E, 0x1F680, /* 248 */
    0x325D, 0x1F680, 0x329C, 0x1F680, 0x32DB, 0x1F680, 0x331A, 0x1F680, /* 252 */
    0x3359, 0x3E2B0, 0x33D6, 0x3E2B0, 0x3453, 0x3E2B0, 0x34D0, 0x3E2B0, /* 256 */
    0x354D, 0x3E2B0, 0x35CA, 0x3E2B0, 0x3647, 0x3E2B0, 0x36C4, 0x3E2B0, /* 260 */
    0x3741, 0x3E2B0, 0x37BE, 0x3E2B0, 0x383B, 0x3E2B0, 0x38B8, 0x3E2B0, /* 264 */
    0x3935, 0x3E2B0, 0x39B2, 0x3E2B0, 0x3A2F, 0x3E2B0, 0x3AAC, 0x3E2B0, /* 268 */
    0x3B29, 0x2197C, 0x3B6D, 0x28DB8, 0x3BBF, 0x44406, 0x3C48, 0x31572, /* 272 */
    0x3CAB, 0x20E6, 0x3CB0, 0x13BDE, 0x3CD8, 0x14434, 0x3D01, 0x26281, /* 276 */
    0x3D4E, 0x23234, 0x3D95, 0x1FF1D, 0x3DD5, 0x21EC3, 0x3E19, 0x22154, /* 280 */
    0x3E5E, 0x2197F, 0x3EA2, 0x228FA, 0x3EE8, 0x23ED3, 0x3F30, 0x24D14, /* 284 */
    0x3F7A, 0x3A9E9, 0x3FF0, 0x21BDC, 0x4034, 0x23CD7, 0x407C, 0x20E4B, /* 288 */
    0x40BE, 0x1E8AA, 0x40FC, 0x23AC6, 0x4144, 0x42E02, 0x41CA, 0x22B16, /* 292 */
    0x4210, 0x25CCC, 0x425C, 0x20C64, 0x429E, 0x23356, 0x42E5, 0x23DDA, /* 296 */
    0x432D, 0x1D541, 0x4368, 0xAF8, 0x436A, 0x1FE99, 0x43AA, 0x3F813, /* 300 */
    0x442A, 0x1B948, 0x4462, 0x7F44, 0x4472, 0x918, 0x4474, 0xB78, /* 304 */
    0x4476, 0x10F50, 0x4498, 0x3824, 0x44A0, 0xE0, 0x44A1, 0x5854, /* 308 */
    0x44AD, 0xB1E8, 0x44C4, 0x73C8, 0x44D3, 0x4BA8, 0x44DD, 0x78, /* 312 */
    0x44DE, 0x2A10, 0x44E4, 0x5710, 0x44EF, 0x3BF8, 0x44F7, 0x7EF2, /* 316 */
    0x4507, 0x86E8, 0x4518, 0x5928, 0x4524, 0x918, 0x4526, 0xB78, /* 320 */
    0x4528, 0x10F50, 0x454A, 0x3824, 0x4552, 0xE0, 0x4553, 0x5854, /* 324 */
    0x455F, 0xB1E8, 0x4576, 0x73C8, 0x4585, 0x4BA8, 0x458F, 0x78, /* 328 */
    0x4590, 0x5250, 0x459B, 0x3B88, 0x45A3, 0x10F50, 0x45C5, 0x3824, /* 332 */
    0x45CD, 0xE0, 0x45CE, 0x5854, 0x45DA, 0xB1E8, 0x45F1, 0x73C8, /* 336 */
    0x4600, 0x4BA8, 0x460A, 0x78, 0x460B, 0x5250, 0x4616, 0x3B88, /* 340 */
    0x461E, 0x8A80, 0x4630, 0x1378, 0x4633, 0xE0, 0x4634, 0x5854, /* 344 */
    0x4640, 0xB1E8, 0x4657, 0x73C8, 0x4666, 0x4BA8, 0x4670, 0x78, /* 348 */
    0x4671, 0x4590, 0x467A, 0x39F8, 0x4682, 0x69A0, 0x4690, 0x58AC, /* 352 */
    0x469C, 0x10F50, 0x46BE, 0x3824, 0x46C6, 0xE0, 0x46C7, 0x5854, /* 356 */
    0x46D3, 0xB1E8, 0x46EA, 0x73C8, 0x46F9, 0x4BA8, 0x4703, 0x78, /* 360 */
    0x4704, 0x10F50, 0x4726, 0x3824, 0x472E, 0xB1E8, 0x4745, 0x73C8, /* 364 */
    0x4754, 0x4BA8, 0x475E, 0x78, 0x475F, 0xE0, 0x4760, 0x5854, /* 368 */
    0x476C, 0x4BA8, 0x4776, 0x78, 0x4777, 0xB1E8, 0x478E, 0x73C8, /* 372 */
    0x479D, 0x4038, 0x47A6, 0x13890, 0x47CE, 0x7348, 0x47DD, 0x11780, /* 376 */
    0x4800, 0x23B18, 0x4848, 0x18284, 0x4879, 0x223F0, 0x48BE, 0x17284, /* 380 */
    0x48ED, 0x3BD58, 0x4965, 0x311BC, 0x49C8, 0x5D160, 0x4A83, 0x38EFC, /* 384 */
    0x4AF5, 0x44390, 0x4B7E, 0x39380, 0x4BF1, 0x48F50, 0x4C83, 0x2BB4C, /* 388 */
    0x4CDB, 0x3D630, 0x4D56, 0x2FFB8, 0x4DB6, 0xB1E8, 0x4DCD, 0x73C8, /* 392 */
    0x4DDC, 0x4BA8, 0x4DE6, 0x78, 0x4DE7, 0x35C0, 0x4DEE, 0x30C4, /* 396 */
    0x4DF5, 0x69A0, 0x4E03, 0x58AC, 0x4E0F, 0x12A50, 0x4E35, 0x60C0, /* 400 */
    0x4E42, 0x1E1C0, 0x4E7F, 0x38E4, 0x4E87, 0x10F50, 0x4EA9, 0x3824, /* 404 */
    0x4EB1, 0xE0, 0x4EB2, 0x5854, 0x4EBE, 0xB1E8, 0x4ED5, 0x73C8, /* 408 */
    0x4EE4, 0x4BA8, 0x4EEE, 0x78, 0x4EEF, 0x10F50, 0x4F11, 0x3824, /* 412 */
    0x4F19, 0x1F48, 0x4F1D, 0xAD00, 0x4F33, 0x198, 0x4F34, 0x64D2, /* 416 */
    0x4F41, 0x12A50, 0x4F67, 0x60C0, 0x4F74, 0xE0, 0x4F75, 0x5854, /* 420 */
    0x4F81, 0xB1E8, 0x4F98, 0x73C8, 0x4FA7, 0x4BA8, 0x4FB1, 0x78, /* 424 */
    0x4FB2, 0x198, 0x4FB3, 0x64D2, 0x4FC0, 0x12A50, 0x4FE6, 0x60C0, /* 428 */
    0x4FF3, 0xE0, 0x4FF4, 0x5854, 0x5000, 0xB1E8, 0x5017, 0x73C8, /* 432 */
    0x5026, 0x4BA8, 0x5030, 0x78, 0x5031, 0x12A50, 0x5057, 0x60C0, /* 436 */
    0x5064, 0xE0, 0x5065, 0x5854, 0x5071, 0xB1E8, 0x5088, 0x73C8, /* 440 */
    0x5097, 0x4BA8, 0x50A1, 0x78, 0x50A2, 0x2B840, 0x50FA, 0x295E0, /* 444 */
    0x514D, 0x2926A, 0x51A0, 0x2A63A, 0x51F5, 0x2A492, 0x524A, 0x2A46C, /* 448 */
    0x529F, 0x1F0DC, 0x52DE, 0x23B7A, 0x5326, 0x22232, 0x536B, 0x15468, /* 452 */
    0x5396, 0x6FCC, 0x53A4, 0x9618, 0x53B7, 0x17118, 0x53E6, 0x10468, /* 456 */
    0x5407, 0x8480, 0x5418, 0x1F48, 0x541C, 0xAD00, 0x5432, 0xEB0, /* 460 */
    0x5434, 0x19B64, 0x5468, 0x12E48, 0x548E, 0x6C80, 0x549C, 0x968, /* 464 */
    0x549E, 0x7358, 0x54AD, 0xD230, 0x54C8, 0x2AE4C, 0x551E, 0x1B3D0, /* 468 */
    0x5555, 0x3146E, 0x55B8, 0xD7E0, 0x55D4, 0x17918, 0x5604, 0xD190, /* 472 */
    0x561F, 0xD370, 0x563A, 0x19D50, 0x566E, 0x7680, 0x567D, 0x12A50, /* 476 */
    0x56A3, 0x60C0, 0x56B0, 0x35C0, 0x56B7, 0x30C4, 0x56BE, 0x198, /* 480 */
    0x56BF, 0x64D2, 0x56CC, 0x155E8, 0x56F7, 0x53BC, 0x5702, 0x23690, /* 484 */
    0x5749, 0x1618, 0x574C, 0x8B94, 0x575E, 0x86E8, 0x576F, 0x5928, /* 488 */
    0x577B, 0x858, 0x577D, 0x25CC, 0x5782, 0x918, 0x5784, 0xB78, /* 492 */
    0x5786, 0x10F50, 0x57A8, 0x3824, 0x57B0, 0xE0, 0x57B1, 0x5854, /* 496 */
    0x57BD, 0xB1E8, 0x57D4, 0x73C8, 0x57E3, 0x4BA8, 0x57ED, 0x78, /* 500 */
    0x57EE, 0x858, 0x57F0, 0x25CC, 0x57F5, 0x86E8, 0x5806, 0x5928, /* 504 */
    0x5812, 0x918, 0x5814, 0xB78, 0x5816, 0x10F50, 0x5838, 0x3824, /* 508 */
    0x5840, 0xE0, 0x5841, 0x5854, 0x584D, 0xB1E8, 0x5864, 0x73C8, /* 512 */
    0x5873, 0x4BA8, 0x587D, 0x78, 0x587E, 0x540, 0x587F, 0x13292, /* 516 */
    0x58A6, 0xA518, 0x58BB, 0x87AC, 0x58CC, 0xE0, 0x58CD, 0x5854, /* 520 */
    0x58D9, 0xB1E8, 0x58F0, 0x73C8, 0x58FF, 0x4BA8, 0x5909, 0x78, /* 524 */
    0x590A, 0xA518, 0x591F, 0x87AC, 0x5930, 0xE0, 0x5931, 0x5854, /* 528 */
    0x593D, 0xB1E8, 0x5954, 0x73C8, 0x5963, 0x4BA8, 0x596D, 0x78, /* 532 */
    0x596E, 0xFE18, 0x598E, 0xE0BA, 0x59AB, 0x11058, 0x59CE, 0xC2EC, /* 536 */
    0x59E7, 0x918, 0x59E9, 0xB78, 0x59EB, 0x10F50, 0x5A0D, 0x3824, /* 540 */
    0x5A15, 0xE0, 0x5A16, 0x5854, 0x5A22, 0xB1E8, 0x5A39, 0x73C8, /* 544 */
    0x5A48, 0x4BA8, 0x5A52, 0x78, 0x5A53, 0x61C0, 0x5A60, 0xD420, /* 548 */
    0x5A7B, 0x11058, 0x5A9E, 0xC2EC, 0x5AB7, 0x918, 0x5AB9, 0xB78, /* 552 */
    0x5ABB, 0x10F50, 0x5ADD, 0x3824, 0x5AE5, 0xE0, 0x5AE6, 0x5854, /* 556 */
    0x5AF2, 0xB1E8, 0x5B09, 0x73C8, 0x5B18, 0x4BA8, 0x5B22, 0x78, /* 560 */
    0x5B23, 0x1308, 0x5B26, 0x1B024, 0x5B5D, 0x1828, 0x5B61, 0x2C870, /* 564 */
    0x5BBB, 0x69A0, 0x5BC9, 0x58AC, 0x5BD5, 0x918, 0x5BD7, 0xB78, /* 568 */
    0x5BD9, 0x10F50, 0x5BFB, 0x3824, 0x5C03, 0xE0, 0x5C04, 0x5854, /* 572 */
    0x5C10, 0xB1E8, 0x5C27, 0x73C8, 0x5C36, 0x4BA8, 0x5C40, 0x78, /* 576 */
    0x5C41, 0x980, 0x5C43, 0x88BE, 0x5C55, 0x69A0, 0x5C63, 0x58AC, /* 580 */
    0x5C6F, 0x918, 0x5C71, 0xB78, 0x5C73, 0x10F50, 0x5C95, 0x3824, /* 584 */
    0x5C9D, 0xE0, 0x5C9E, 0x5854, 0x5CAA, 0xB1E8, 0x5CC1, 0x73C8, /* 588 */
    0x5CD0, 0x4BA8, 0x5CDA, 0x78, 0x5CDB, 0x69A0, 0x5CE9, 0x58AC, /* 592 */
    0x5CF5, 0x918, 0x5CF7, 0xB78, 0x5CF9, 0x10F50, 0x5D1B, 0x3824, /* 596 */
    0x5D23, 0xE0, 0x5D24, 0x5854, 0x5D30, 0xB1E8, 0x5D47, 0x73C8, /* 600 */
    0x5D56, 0x4BA8, 0x5D60, 0x78, 0x5D61, 0x1F560, 0x5DA0, 0xDCE4, /* 604 */
    0x5DBC, 0xBBA0, 0x5DD4, 0x6FA4, 0x5DE2, 0x10C18, 0x5E04, 0x26EEA, /* 608 */
    0x5E52, 0x29800, 0x5EA5, 0x38EF2, 0x5F17, 0x12B48, 0x5F3D, 0x8AD8, /* 612 */
    0x5F4F, 0x16778, 0x5F7C, 0x62D8, 0x5F89, 0x8A80, 0x5F9B, 0x1378, /* 616 */
    0x5F9E, 0xB980, 0x5FB6, 0x7EC0, 0x5FC6, 0x7230, 0x5FD5, 0x6E08, /* 620 */
    0x5FE3, 0x4380, 0x5FEC, 0x97BC, 0x5FFF, 0x3910, 0x6007, 0x8DAC, /* 624 */
    0x6019, 0x13DC8, 0x6041, 0xCE2C, 0x605B, 0x16B68, 0x6089, 0x18BD4, /* 628 */
    0x60BB, 0x13138, 0x60E2, 0x156E8, 0x610D, 0x21F78, 0x6151, 0x1E14A, /* 632 */
    0x618E, 0x17C70, 0x61BE, 0x14E4A, 0x61E8, 0x1F440, 0x6227, 0x1D94A, /* 636 */
    0x6263, 0x12E90, 0x6289, 0xBE2C, 0x62A1, 0x14038, 0x62CA, 0x15BD4, /* 640 */
    0x62F6, 0x1ABE8, 0x632C, 0xA6FC, 0x6341, 0x11F78, 0x6365, 0xA62C, /* 644 */
    0x637A, 0x9A90, 0x638E, 0x12EF4, 0x63B4, 0x1CF20, 0x63EE, 0x12F30, /* 648 */
    0x6414, 0x2F848, 0x6474, 0x10874, 0x6496, 0x27880, 0x64E6, 0xF874, /* 652 */
    0x6506, 0xA2C8, 0x651B, 0x12418, 0x6540, 0x9220, 0x6553, 0xEA80, /* 656 */
    0x6571, 0x1A420, 0x65A6, 0xC2E0, 0x65BF, 0x15560, 0x65EA, 0xA5E8, /* 660 */
    0x65FF, 0x1DAD0, 0x663B, 0xADDC, 0x6651, 0x15020, 0x667C, 0xBF00, /* 664 */
    0x6694, 0x13D20, 0x66BC, 0x1FFC0, 0x66FC, 0x2B990, 0x6754, 0x2146C, /* 668 */
    0x6797, 0x15918, 0x67C3, 0x11B58, 0x67E7, 0xDBE0, 0x6803, 0x90B4, /* 672 */
    0x6816, 0x1E150, 0x6853, 0xDFE0, 0x6870, 0x10F18, 0x6892, 0x918C, /* 676 */
    0x68A5, 0x13A30, 0x68CD, 0x5C3C, 0x68D9, 0x1FB40, 0x6919, 0x929E, /* 680 */
    0x692C, 0x17E78, 0x695C, 0x67F4, 0x696A, 0x16410, 0x6997, 0xAB08, /* 684 */
    0x69AD, 0xED88, 0x69CB, 0x898C, 0x69DD, 0xA448, 0x69F2, 0x64FC, /* 688 */
    0x69FF, 0x17A88, 0x6A2F, 0x5A8C, 0x6A3B, 0x13E40, 0x6A63, 0x6B1C, /* 692 */
    0x6A71, 0x8C58, 0x6A83, 0xB9CC, 0x6A9B, 0x3F48, 0x6AA3, 0x5C88, /* 696 */
    0x6AAF, 0xC7C8, 0x6AC8, 0xB06C, 0x6ADF, 0x13478, 0x6B06, 0x9A84, /* 700 */
    0x6B1A, 0x12658, 0x6B3F, 0x6948, 0x6B4D, 0xAA98, 0x6B63, 0x721C, /* 704 */
    0x6B72, 0xC2E0, 0x6B8B, 0xF450, 0x6BAA, 0xFE60, 0x6BCA, 0xED0C, /* 708 */
    0x6BE8, 0x1D758, 0x6C23, 0xD0BC, 0x6C3E, 0x1B190, 0x6C75, 0x8A1C, /* 712 */
    0x6C87, 0xAB34, 0x6C9D, 0x14438, 0x6CC6, 0x6CD8, 0x6CD4, 0xA9D4, /* 716 */
    0x6CEA, 0x8DB0, 0x6CFC, 0x14C5C, 0x6D26, 0x78B0, 0x6D36, 0x1741C, /* 720 */
    0x6D65, 0xF478, 0x6D84, 0x12944, 0x6DAA, 0x17FE0, 0x6DDB, 0xB682, /* 724 */
    0x6DF2, 0x11521, 0x6E15, 0xE0, 0x6E16, 0x5854, 0x6E22, 0xB1E8, /* 728 */
    0x6E39, 0x73C8, 0x6E48, 0x4BA8, 0x6E52, 0x78, 0x6E53, 0x10F50, /* 732 */
    0x6E75, 0x3824, 0x6E7D, 0x1B948, 0x6EB5, 0x7F44, 0x6EC5, 0x3BF8, /* 736 */
    0x6ECD, 0x7EF2, 0x6EDD, 0x2A10, 0x6EE3, 0x5710, 0x6EEE, 0x988, /* 740 */
    0x6EF0, 0xA452, 0x6F05, 0xAA0, 0x6F07, 0x12384, 0x6F2C, 0xD68, /* 744 */
    0x6F2E, 0xF718, 0x6F4D, 0x1068, 0x6F50, 0x1080E, 0x6F72, 0x768, /* 748 */
    0x6F73, 0xCCF2, 0x6F8D, 0x830, 0x6F8F, 0xD012, 0x6FAA, 0xA40, /* 752 */
    0x6FAC, 0x10554, 0x6FCD, 0xB10, 0x6FCF, 0xE5D4, 0x6FEC, 0x1E1C0, /* 756 */
    0x7029, 0x38E4, 0x7031, 0x86E8, 0x7042, 0x5928, 0x704E, 0x1618, /* 760 */
    0x7051, 0x8B94, 0x7063, 0x858, 0x7065, 0x25CC, 0x706A, 0x540, /* 764 */
    0x706B, 0x13292, 0x7092, 0xA518, 0x70A7, 0x87AC, 0x70B8, 0x5468, /* 768 */
    0x70C3, 0x95C, 0x70C5, 0xFE18, 0x70E5, 0xE0BA, 0x7102, 0x8D60, /* 772 */
    0x7114, 0xED1A, 0x7132, 0x25C8, 0x7137, 0x11D8C, 0x715B, 0x10B8, /* 776 */
    0x715E, 0x124C4, 0x7183, 0x1E648, 0x71C0, 0xFA00, 0x71E0, 0x61C0, /* 780 */
    0x71ED, 0xD420, 0x7208, 0x11058, 0x722B, 0xC2EC, 0x7244, 0x1308, /* 784 */
    0x7247, 0x1B024, 0x727E, 0x1828, 0x7282, 0x2C870, 0x72DC, 0x918, /* 788 */
    0x72DE, 0xB78, 0x72E0, 0x4590, 0x72E9, 0x39F8, 0x72F1, 0x69A0, /* 792 */
    0x72FF, 0x58AC, 0x730B, 0x980, 0x730D, 0x88BE, 0x731F, 0xCA0, /* 796 */
    0x7321, 0x21C22, 0x7365, 0x1268, 0x7368, 0x2407C, 0x73B1, 0xEB0, /* 800 */
    0x73B3, 0x2DC8C, 0x740F, 0x1410, 0x7412, 0x26030, 0x745F, 0x1380, /* 804 */
    0x7462, 0x1D0B0, 0x749D, 0x1710, 0x74A0, 0x2A0AC, 0x74F5, 0xE20, /* 808 */
    0x74F7, 0x25764, 0x7542, 0xF80, 0x7544, 0x2C344, 0x759D, 0xF78, /* 812 */
    0x759F, 0x110B6, 0x75C2, 0x1090, 0x75C5, 0x11B16, 0x75E9, 0x14C8, /* 816 */
    0x75EC, 0x10FDC, 0x760F, 0xE10, 0x7611, 0x11F0A, 0x7635, 0x1B98, /* 820 */
    0x7639, 0x132AA, 0x7660, 0x1F28, 0x7664, 0x12FEA, 0x768B, 0x1638, /* 824 */
    0x768E, 0x12FD0, 0x76B4, 0x1798, 0x76B7, 0x1594E, 0x76E3, 0x14C8, /* 828 */
    0x76E6, 0x1FBBE, 0x7726, 0x10E8, 0x7729, 0x20A2C, 0x776B, 0xD58, /* 832 */
    0x776D, 0x21B52, 0x77B1, 0x11A0, 0x77B4, 0x1FBF2, 0x77F4, 0x1200, /* 836 */
    0x77F7, 0x2118A, 0x783A, 0x17C8, 0x783D, 0x253AA, 0x7888, 0xED8, /* 840 */
    0x788A, 0x23266, 0x78D1, 0x1270, 0x78D4, 0x21B22, 0x7918, 0x2A80, /* 844 */
    0x791E, 0x2568E, 0x7969, 0x2D80, 0x796F, 0x2C82C, 0x79C9, 0x1278, /* 848 */
    0x79CC, 0x266F4, 0x7A19, 0x1390, 0x7A1C, 0x269EC, 0x7A6A, 0x2288, /* 852 */
    0x7A6F, 0x30D5C, 0x7AD1, 0x2408, 0x7AD6, 0x2D862, 0x7B32, 0x1EF0, /* 856 */
    0x7B36, 0x1C894, 0x7B70, 0x2130, 0x7B75, 0x19A54, 0x7BA9, 0x32A0, /* 860 */
    0x7BB0, 0x1A672, 0x7BE5, 0x3680, 0x7BEC, 0x2039A, 0x7C2D, 0x33B8, /* 864 */
    0x7C34, 0x1A454, 0x7C69, 0x3138, 0x7C70, 0x166BE, 0x7C9D, 0x3980, /* 868 */
    0x7CA5, 0x20824, 0x7CE7, 0x3090, 0x7CEE, 0x1B786, 0x7D25, 0x3928, /* 872 */
    0x7D2D, 0x1A438, 0x7D62, 0x3428, 0x7D69, 0x1BF0A, 0x7DA1, 0x760, /* 876 */
    0x7DA2, 0xFADE, 0x7DC2, 0xCB0, 0x7DC4, 0xEE7A, 0x7DE2, 0x878, /* 880 */
    0x7DE4, 0xE902, 0x7E02, 0x5F8, 0x7E03, 0x11C96, 0x7E27, 0xD78, /* 884 */
    0x7E29, 0x10220, 0x7E4A, 0x778, 0x7E4B, 0x10F2A, 0x7E6D, 0x878, /* 888 */
    0x7E6F, 0x119AA, 0x7E93, 0x820, 0x7E95, 0xF4B8, 0x7EB4, 0xD68, /* 892 */
    0x7EB6, 0xED6C, 0x7ED4, 0xE30, 0x7ED6, 0x108C0, 0x7EF8, 0x818, /* 896 */
    0x7EFA, 0x114BC, 0x7F1D, 0x930, 0x7F1F, 0xFE00, 0x7F3F, 0x6B0, /* 900 */
    0x7F40, 0x134C8, 0x7F67, 0x830, 0x7F69, 0x132D4, 0x7F90, 0x930, /* 904 */
    0x7F92, 0x1122E, 0x7FB5, 0x8D8, 0x7FB7, 0x1E980, 0x7FF5, 0x1B60, /* 908 */
    0x7FF9, 0x1DE20, 0x8035, 0x1300, 0x8038, 0x25068, 0x8083, 0x19D8, /* 912 */
    0x8087, 0x23966, 0x80CF, 0x1418, 0x80D2, 0x1EF38, 0x8110, 0x1AD0, /* 916 */
    0x8114, 0x2090A, 0x8156, 0x17C0, 0x8159, 0x2BCB6, 0x81B1, 0x12E8, /* 920 */
    0x81B4, 0x1B8A0, 0x81EC, 0x13F8, 0x81EF, 0x1BCE8, 0x8227, 0x988, /* 924 */
    0x8229, 0x170C0, 0x8258, 0xD68, 0x825A, 0x122F2, 0x827F, 0xC90, /* 928 */
    0x8281, 0x1053A, 0x82A2, 0xAA0, 0x82A4, 0x1A038, 0x82D9, 0x1068, /* 932 */
    0x82DC, 0x185F4, 0x830D, 0x1048, 0x8310, 0x18980, 0x8342, 0xBB0, /* 936 */
    0x8344, 0x15D54, 0x8370, 0xD48, 0x8372, 0x125E8, 0x8397, 0xBB0, /* 940 */
    0x8399, 0x25068, 0x83E4, 0xED8, 0x83E6, 0x1DE20, 0x8422, 0xE28, /* 944 */
    0x8424, 0x23966, 0x846C, 0xCC8, 0x846E, 0x1EF38, 0x84AC, 0x11D8, /* 948 */
    0x84AF, 0x2090A, 0x84F1, 0x11B8, 0x84F4, 0x2BCB6, 0x854C, 0xD20, /* 952 */
    0x854E, 0x1B8A0, 0x8586, 0xE00, 0x8588, 0x1BC64, 0x85C0, 0xDE8, /* 956 */
    0x85C2, 0x12AFC, 0x85E8, 0xF00, 0x85EA, 0x18C76, 0x861C, 0xFA0, /* 960 */
    0x861E, 0x1D102, 0x8659, 0x12A0, 0x865C, 0x1C188, 0x8695, 0xC90, /* 964 */
    0x8697, 0xFE8C, 0x86B7, 0x1048, 0x86BA, 0x1A83C, 0x86F0, 0xA40, /* 968 */
    0x86F2, 0x1A762, 0x8727, 0xD48, 0x8729, 0x1B0B2, 0x8760, 0xD68, /* 972 */
    0x8762, 0x339CA, 0x87CA, 0x988, 0x87CC, 0x3772A, 0x883B, 0xAA0, /* 976 */
    0x883D, 0x3C762, 0x88B6, 0x1068, 0x88B9, 0x3ACA2, 0x892F, 0x820, /* 980 */
    0x8931, 0x3252E, 0x8996, 0x8E8, 0x8998, 0x39626, 0x8A0B, 0x1010, /* 984 */
    0x8A0E, 0x34A7E, 0x8A78, 0x1250, 0x8A7B, 0x37C26, 0x8AEB, 0x5E4B8, /* 988 */
};
/* The archive's file groups: a group's first file, its file count and its bytes (the
 * games' file_group_load_info.group points at one). */
FileGroup FileGroups[130] = {
    { 0, 0x88, 0xCBA000 }, { 0x88, 5, 0x13E800 }, { 0x8D, 0xC, 0x1B800 },  /* 0 */
    { 0x99, 0x7E, 0x1069800 }, { 0x117, 0x19, 0x394800 }, { 0x130, 0xC, 0x5A000 },  /* 3 */
    { 0x13C, 0x10, 0x59000 }, { 0x14C, 0xA, 0x3D800 }, { 0x156, 2, 0x9800 },  /* 6 */
    { 0x158, 8, 0x29800 }, { 0x160, 0xC, 0x49800 }, { 0x16C, 6, 0x2D800 },  /* 9 */
    { 0x172, 8, 0x37800 }, { 0x17A, 0x10, 0x2F4000 }, { 0x18A, 4, 0x18800 },  /* 12 */
    { 0x18E, 0x10, 0x84000 }, { 0x19E, 0xE, 0x61800 }, { 0x1AC, 0xA, 0x3F800 },  /* 15 */
    { 0x1B6, 8, 0x38800 }, { 0x1BE, 0x2A, 0x353000 }, { 0x1E8, 0x10, 0x52800 },  /* 18 */
    { 0x1F8, 0xE, 0x48000 }, { 0x206, 0xA, 0x46000 }, { 0x210, 8, 0x32000 },  /* 21 */
    { 0x218, 0xE, 0x72800 }, { 0x226, 0xE, 0x68000 }, { 0x234, 0x10, 0x8F000 },  /* 24 */
    { 0x244, 0xE, 0x4D000 }, { 0x252, 0xC, 0x43000 }, { 0x25E, 0x6E, 0x792000 },  /* 27 */
    { 0x2CC, 0xD, 0xC6800 }, { 0x2D9, 8, 0x34000 }, { 0x2E1, 0x3E, 0x251000 },  /* 30 */
    { 0x31F, 2, 0x23000 }, { 0x321, 2, 0x26000 }, { 0x323, 2, 0x2F000 },  /* 33 */
    { 0x325, 2, 0x28000 }, { 0x327, 2, 0x1F000 }, { 0x329, 2, 0x2C000 },  /* 36 */
    { 0x32B, 2, 0x26800 }, { 0x32D, 2, 0x2D800 }, { 0x32F, 2, 0x12800 },  /* 39 */
    { 0x331, 2, 0x13800 }, { 0x333, 2, 0x12800 }, { 0x335, 2, 0x13000 },  /* 42 */
    { 0x337, 2, 0x15800 }, { 0x339, 2, 0x15000 }, { 0x33B, 2, 0x14800 },  /* 45 */
    { 0x33D, 2, 0x17800 }, { 0x33F, 2, 0x21800 }, { 0x341, 2, 0x22800 },  /* 48 */
    { 0x343, 2, 0x23000 }, { 0x345, 2, 0x21800 }, { 0x347, 2, 0x23000 },  /* 51 */
    { 0x349, 2, 0x27000 }, { 0x34B, 2, 0x24800 }, { 0x34D, 2, 0x23800 },  /* 54 */
    { 0x34F, 2, 0x28800 }, { 0x351, 2, 0x30000 }, { 0x353, 2, 0x28000 },  /* 57 */
    { 0x355, 2, 0x28800 }, { 0x357, 2, 0x33800 }, { 0x359, 2, 0x30800 },  /* 60 */
    { 0x35B, 2, 0x1F000 }, { 0x35D, 2, 0x1C800 }, { 0x35F, 2, 0x1E000 },  /* 63 */
    { 0x361, 2, 0x24000 }, { 0x363, 2, 0x1E000 }, { 0x365, 2, 0x1A000 },  /* 66 */
    { 0x367, 2, 0x25000 }, { 0x369, 2, 0x1F000 }, { 0x36B, 2, 0x1E800 },  /* 69 */
    { 0x36D, 2, 0x1F800 }, { 0x36F, 2, 0x10800 }, { 0x371, 2, 0x10000 },  /* 72 */
    { 0x373, 2, 0x10000 }, { 0x375, 2, 0x12800 }, { 0x377, 2, 0x11800 },  /* 75 */
    { 0x379, 2, 0x11800 }, { 0x37B, 2, 0x13000 }, { 0x37D, 2, 0x10800 },  /* 78 */
    { 0x37F, 2, 0x10000 }, { 0x381, 2, 0x12000 }, { 0x383, 2, 0x12800 },  /* 81 */
    { 0x385, 2, 0x11000 }, { 0x387, 2, 0x14000 }, { 0x389, 2, 0x14800 },  /* 84 */
    { 0x38B, 2, 0x12800 }, { 0x38D, 2, 0x20000 }, { 0x38F, 2, 0x20000 },  /* 87 */
    { 0x391, 2, 0x27000 }, { 0x393, 2, 0x26000 }, { 0x395, 2, 0x20800 },  /* 90 */
    { 0x397, 2, 0x23000 }, { 0x399, 2, 0x2D800 }, { 0x39B, 2, 0x1D800 },  /* 93 */
    { 0x39D, 2, 0x1D800 }, { 0x39F, 2, 0x18800 }, { 0x3A1, 2, 0x13800 },  /* 96 */
    { 0x3A3, 2, 0x11800 }, { 0x3A5, 2, 0x1B800 }, { 0x3A7, 2, 0x1A000 },  /* 99 */
    { 0x3A9, 2, 0x1A800 }, { 0x3AB, 2, 0x17000 }, { 0x3AD, 2, 0x13800 },  /* 102 */
    { 0x3AF, 2, 0x26800 }, { 0x3B1, 2, 0x1F000 }, { 0x3B3, 2, 0x25000 },  /* 105 */
    { 0x3B5, 2, 0x20000 }, { 0x3B7, 2, 0x22800 }, { 0x3B9, 2, 0x2D800 },  /* 108 */
    { 0x3BB, 2, 0x1D000 }, { 0x3BD, 2, 0x1D000 }, { 0x3BF, 2, 0x14000 },  /* 111 */
    { 0x3C1, 2, 0x1A000 }, { 0x3C3, 2, 0x1E800 }, { 0x3C5, 2, 0x1E000 },  /* 114 */
    { 0x3C7, 2, 0x11000 }, { 0x3C9, 2, 0x1C800 }, { 0x3CB, 2, 0x1B800 },  /* 117 */
    { 0x3CD, 2, 0x1C800 }, { 0x3CF, 2, 0x35000 }, { 0x3D1, 2, 0x38800 },  /* 120 */
    { 0x3D3, 2, 0x3D800 }, { 0x3D5, 2, 0x3C800 }, { 0x3D7, 2, 0x33800 },  /* 123 */
    { 0x3D9, 2, 0x3A800 }, { 0x3DB, 2, 0x36800 }, { 0x3DD, 2, 0x39800 },  /* 126 */
    { 0x3DF, 1, 0x5E800 },  /* 129 */
};
LoaderState Loader = {0};
u8 LoadNodePool[2304] = {0};
void *LoadNodeFree = 0;
BlockRef SmallNodePool[64] = {0};
BlockRef *SmallNodeFree = 0;
s32 DirPadBits[24] = {
    0x10, 0x30, 0x20, 0x60, 0x40, 0xC0, 0x80, 0x90, 0, 0, 0x400, 0x200, 0x800, 0, 0x600, 0,
    0xC00, 0xE00, 0, 0, 0xA00, 0, 0, 0,
};
s32 WideScreen = 0;
s32 ComboSeqs[36] = {
    (s32)&toggle_widescreen, 0x3C, 0, 0, 0x20, 0x80,
    0x80, 0x20, 0x80, 0x80, 0x20, 0x80,
    0x80, 0x20, 0x80, 0x80, 0x80, 0x20,
    0x80, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};
PadState Pads[8] = {0};
s32 PadCount = 0;
s32 first_joypad = 0;
s32 PadRunEnd = 0;
s32 joypad_idle_count = 0;
s32 ShellUnusedTable[12] = { 2, 2, 2, 2, 8, 2, 0x10, 1, 2, 1, 1, 4 };
s32 screen_aspect[2] = { 4, 3 };
camera_info_tag MainCamera = {0};
camera_info_tag MainCameraUnused = {0};  /* a second camera's room; read nowhere */
s32 cam_ramp_overide_count = 0;
s32 CamEaseOverride = 0;
s32 RandomSeedA = 0xC587C1;
s32 RandomSeedB = 0x573CEC9;
s16 AtanTable[2050] = {
    0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9,
    0xA, 0xA, 0xA, 0xB, 0xB, 0xB, 0xB, 0xC, 0xC, 0xC, 0xD, 0xD, 0xD, 0xE, 0xE, 0xE, 0xF, 0xF,
    0xF, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x14,
    0x14, 0x14, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x18,
    0x19, 0x19, 0x19, 0x1A, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1C, 0x1C, 0x1C, 0x1D, 0x1D, 0x1D,
    0x1E, 0x1E, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x20, 0x20, 0x20, 0x21, 0x21, 0x21, 0x22, 0x22,
    0x22, 0x23, 0x23, 0x23, 0x24, 0x24, 0x24, 0x25, 0x25, 0x25, 0x26, 0x26, 0x26, 0x26, 0x27,
    0x27, 0x27, 0x28, 0x28, 0x28, 0x29, 0x29, 0x29, 0x2A, 0x2A, 0x2A, 0x2B, 0x2B, 0x2B, 0x2C,
    0x2C, 0x2C, 0x2C, 0x2D, 0x2D, 0x2D, 0x2E, 0x2E, 0x2E, 0x2F, 0x2F, 0x2F, 0x30, 0x30, 0x30,
    0x31, 0x31, 0x31, 0x32, 0x32, 0x32, 0x33, 0x33, 0x33, 0x33, 0x34, 0x34, 0x34, 0x35, 0x35,
    0x35, 0x36, 0x36, 0x36, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x39, 0x3A,
    0x3A, 0x3A, 0x3B, 0x3B, 0x3B, 0x3C, 0x3C, 0x3C, 0x3D, 0x3D, 0x3D, 0x3E, 0x3E, 0x3E, 0x3F,
    0x3F, 0x3F, 0x3F, 0x40, 0x40, 0x40, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42, 0x43, 0x43, 0x43,
    0x44, 0x44, 0x44, 0x45, 0x45, 0x45, 0x45, 0x46, 0x46, 0x46, 0x47, 0x47, 0x47, 0x48, 0x48,
    0x48, 0x49, 0x49, 0x49, 0x4A, 0x4A, 0x4A, 0x4A, 0x4B, 0x4B, 0x4B, 0x4C, 0x4C, 0x4C, 0x4D,
    0x4D, 0x4D, 0x4E, 0x4E, 0x4E, 0x4F, 0x4F, 0x4F, 0x4F, 0x50, 0x50, 0x50, 0x51, 0x51, 0x51,
    0x52, 0x52, 0x52, 0x53, 0x53, 0x53, 0x54, 0x54, 0x54, 0x55, 0x55, 0x55, 0x55, 0x56, 0x56,
    0x56, 0x57, 0x57, 0x57, 0x58, 0x58, 0x58, 0x59, 0x59, 0x59, 0x5A, 0x5A, 0x5A, 0x5A, 0x5B,
    0x5B, 0x5B, 0x5C, 0x5C, 0x5C, 0x5D, 0x5D, 0x5D, 0x5E, 0x5E, 0x5E, 0x5F, 0x5F, 0x5F, 0x5F,
    0x60, 0x60, 0x60, 0x61, 0x61, 0x61, 0x62, 0x62, 0x62, 0x63, 0x63, 0x63, 0x63, 0x64, 0x64,
    0x64, 0x65, 0x65, 0x65, 0x66, 0x66, 0x66, 0x67, 0x67, 0x67, 0x68, 0x68, 0x68, 0x68, 0x69,
    0x69, 0x69, 0x6A, 0x6A, 0x6A, 0x6B, 0x6B, 0x6B, 0x6C, 0x6C, 0x6C, 0x6C, 0x6D, 0x6D, 0x6D,
    0x6E, 0x6E, 0x6E, 0x6F, 0x6F, 0x6F, 0x70, 0x70, 0x70, 0x71, 0x71, 0x71, 0x71, 0x72, 0x72,
    0x72, 0x73, 0x73, 0x73, 0x74, 0x74, 0x74, 0x75, 0x75, 0x75, 0x75, 0x76, 0x76, 0x76, 0x77,
    0x77, 0x77, 0x78, 0x78, 0x78, 0x79, 0x79, 0x79, 0x79, 0x7A, 0x7A, 0x7A, 0x7B, 0x7B, 0x7B,
    0x7C, 0x7C, 0x7C, 0x7D, 0x7D, 0x7D, 0x7D, 0x7E, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x80, 0x80,
    0x80, 0x80, 0x81, 0x81, 0x81, 0x82, 0x82, 0x82, 0x83, 0x83, 0x83, 0x84, 0x84, 0x84, 0x84,
    0x85, 0x85, 0x85, 0x86, 0x86, 0x86, 0x87, 0x87, 0x87, 0x88, 0x88, 0x88, 0x88, 0x89, 0x89,
    0x89, 0x8A, 0x8A, 0x8A, 0x8B, 0x8B, 0x8B, 0x8B, 0x8C, 0x8C, 0x8C, 0x8D, 0x8D, 0x8D, 0x8E,
    0x8E, 0x8E, 0x8F, 0x8F, 0x8F, 0x8F, 0x90, 0x90, 0x90, 0x91, 0x91, 0x91, 0x92, 0x92, 0x92,
    0x92, 0x93, 0x93, 0x93, 0x94, 0x94, 0x94, 0x95, 0x95, 0x95, 0x95, 0x96, 0x96, 0x96, 0x97,
    0x97, 0x97, 0x98, 0x98, 0x98, 0x98, 0x99, 0x99, 0x99, 0x9A, 0x9A, 0x9A, 0x9B, 0x9B, 0x9B,
    0x9C, 0x9C, 0x9C, 0x9C, 0x9D, 0x9D, 0x9D, 0x9E, 0x9E, 0x9E, 0x9F, 0x9F, 0x9F, 0x9F, 0xA0,
    0xA0, 0xA0, 0xA1, 0xA1, 0xA1, 0xA1, 0xA2, 0xA2, 0xA2, 0xA3, 0xA3, 0xA3, 0xA4, 0xA4, 0xA4,
    0xA4, 0xA5, 0xA5, 0xA5, 0xA6, 0xA6, 0xA6, 0xA7, 0xA7, 0xA7, 0xA7, 0xA8, 0xA8, 0xA8, 0xA9,
    0xA9, 0xA9, 0xAA, 0xAA, 0xAA, 0xAA, 0xAB, 0xAB, 0xAB, 0xAC, 0xAC, 0xAC, 0xAD, 0xAD, 0xAD,
    0xAD, 0xAE, 0xAE, 0xAE, 0xAF, 0xAF, 0xAF, 0xAF, 0xB0, 0xB0, 0xB0, 0xB1, 0xB1, 0xB1, 0xB2,
    0xB2, 0xB2, 0xB2, 0xB3, 0xB3, 0xB3, 0xB4, 0xB4, 0xB4, 0xB4, 0xB5, 0xB5, 0xB5, 0xB6, 0xB6,
    0xB6, 0xB7, 0xB7, 0xB7, 0xB7, 0xB8, 0xB8, 0xB8, 0xB9, 0xB9, 0xB9, 0xB9, 0xBA, 0xBA, 0xBA,
    0xBB, 0xBB, 0xBB, 0xBC, 0xBC, 0xBC, 0xBC, 0xBD, 0xBD, 0xBD, 0xBE, 0xBE, 0xBE, 0xBE, 0xBF,
    0xBF, 0xBF, 0xC0, 0xC0, 0xC0, 0xC1, 0xC1, 0xC1, 0xC1, 0xC2, 0xC2, 0xC2, 0xC3, 0xC3, 0xC3,
    0xC3, 0xC4, 0xC4, 0xC4, 0xC5, 0xC5, 0xC5, 0xC5, 0xC6, 0xC6, 0xC6, 0xC7, 0xC7, 0xC7, 0xC7,
    0xC8, 0xC8, 0xC8, 0xC9, 0xC9, 0xC9, 0xCA, 0xCA, 0xCA, 0xCA, 0xCB, 0xCB, 0xCB, 0xCC, 0xCC,
    0xCC, 0xCC, 0xCD, 0xCD, 0xCD, 0xCE, 0xCE, 0xCE, 0xCE, 0xCF, 0xCF, 0xCF, 0xD0, 0xD0, 0xD0,
    0xD0, 0xD1, 0xD1, 0xD1, 0xD2, 0xD2, 0xD2, 0xD2, 0xD3, 0xD3, 0xD3, 0xD4, 0xD4, 0xD4, 0xD4,
    0xD5, 0xD5, 0xD5, 0xD6, 0xD6, 0xD6, 0xD6, 0xD7, 0xD7, 0xD7, 0xD8, 0xD8, 0xD8, 0xD8, 0xD9,
    0xD9, 0xD9, 0xDA, 0xDA, 0xDA, 0xDA, 0xDB, 0xDB, 0xDB, 0xDC, 0xDC, 0xDC, 0xDC, 0xDD, 0xDD,
    0xDD, 0xDE, 0xDE, 0xDE, 0xDE, 0xDF, 0xDF, 0xDF, 0xDF, 0xE0, 0xE0, 0xE0, 0xE1, 0xE1, 0xE1,
    0xE1, 0xE2, 0xE2, 0xE2, 0xE3, 0xE3, 0xE3, 0xE3, 0xE4, 0xE4, 0xE4, 0xE5, 0xE5, 0xE5, 0xE5,
    0xE6, 0xE6, 0xE6, 0xE7, 0xE7, 0xE7, 0xE7, 0xE8, 0xE8, 0xE8, 0xE8, 0xE9, 0xE9, 0xE9, 0xEA,
    0xEA, 0xEA, 0xEA, 0xEB, 0xEB, 0xEB, 0xEC, 0xEC, 0xEC, 0xEC, 0xED, 0xED, 0xED, 0xEE, 0xEE,
    0xEE, 0xEE, 0xEF, 0xEF, 0xEF, 0xEF, 0xF0, 0xF0, 0xF0, 0xF1, 0xF1, 0xF1, 0xF1, 0xF2, 0xF2,
    0xF2, 0xF2, 0xF3, 0xF3, 0xF3, 0xF4, 0xF4, 0xF4, 0xF4, 0xF5, 0xF5, 0xF5, 0xF6, 0xF6, 0xF6,
    0xF6, 0xF7, 0xF7, 0xF7, 0xF7, 0xF8, 0xF8, 0xF8, 0xF9, 0xF9, 0xF9, 0xF9, 0xFA, 0xFA, 0xFA,
    0xFA, 0xFB, 0xFB, 0xFB, 0xFC, 0xFC, 0xFC, 0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE, 0xFE, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFF, 0x100, 0x100, 0x100, 0x100, 0x101, 0x101, 0x101, 0x102, 0x102,
    0x102, 0x102, 0x103, 0x103, 0x103, 0x103, 0x104, 0x104, 0x104, 0x105, 0x105, 0x105, 0x105,
    0x106, 0x106, 0x106, 0x106, 0x107, 0x107, 0x107, 0x107, 0x108, 0x108, 0x108, 0x109, 0x109,
    0x109, 0x109, 0x10A, 0x10A, 0x10A, 0x10A, 0x10B, 0x10B, 0x10B, 0x10C, 0x10C, 0x10C, 0x10C,
    0x10D, 0x10D, 0x10D, 0x10D, 0x10E, 0x10E, 0x10E, 0x10E, 0x10F, 0x10F, 0x10F, 0x110, 0x110,
    0x110, 0x110, 0x111, 0x111, 0x111, 0x111, 0x112, 0x112, 0x112, 0x112, 0x113, 0x113, 0x113,
    0x113, 0x114, 0x114, 0x114, 0x115, 0x115, 0x115, 0x115, 0x116, 0x116, 0x116, 0x116, 0x117,
    0x117, 0x117, 0x117, 0x118, 0x118, 0x118, 0x119, 0x119, 0x119, 0x119, 0x11A, 0x11A, 0x11A,
    0x11A, 0x11B, 0x11B, 0x11B, 0x11B, 0x11C, 0x11C, 0x11C, 0x11C, 0x11D, 0x11D, 0x11D, 0x11D,
    0x11E, 0x11E, 0x11E, 0x11F, 0x11F, 0x11F, 0x11F, 0x120, 0x120, 0x120, 0x120, 0x121, 0x121,
    0x121, 0x121, 0x122, 0x122, 0x122, 0x122, 0x123, 0x123, 0x123, 0x123, 0x124, 0x124, 0x124,
    0x125, 0x125, 0x125, 0x125, 0x126, 0x126, 0x126, 0x126, 0x127, 0x127, 0x127, 0x127, 0x128,
    0x128, 0x128, 0x128, 0x129, 0x129, 0x129, 0x129, 0x12A, 0x12A, 0x12A, 0x12A, 0x12B, 0x12B,
    0x12B, 0x12B, 0x12C, 0x12C, 0x12C, 0x12C, 0x12D, 0x12D, 0x12D, 0x12D, 0x12E, 0x12E, 0x12E,
    0x12F, 0x12F, 0x12F, 0x12F, 0x130, 0x130, 0x130, 0x130, 0x131, 0x131, 0x131, 0x131, 0x132,
    0x132, 0x132, 0x132, 0x133, 0x133, 0x133, 0x133, 0x134, 0x134, 0x134, 0x134, 0x135, 0x135,
    0x135, 0x135, 0x136, 0x136, 0x136, 0x136, 0x137, 0x137, 0x137, 0x137, 0x138, 0x138, 0x138,
    0x138, 0x139, 0x139, 0x139, 0x139, 0x13A, 0x13A, 0x13A, 0x13A, 0x13B, 0x13B, 0x13B, 0x13B,
    0x13C, 0x13C, 0x13C, 0x13C, 0x13D, 0x13D, 0x13D, 0x13D, 0x13E, 0x13E, 0x13E, 0x13E, 0x13F,
    0x13F, 0x13F, 0x13F, 0x140, 0x140, 0x140, 0x140, 0x141, 0x141, 0x141, 0x141, 0x142, 0x142,
    0x142, 0x142, 0x143, 0x143, 0x143, 0x143, 0x144, 0x144, 0x144, 0x144, 0x145, 0x145, 0x145,
    0x145, 0x146, 0x146, 0x146, 0x146, 0x146, 0x147, 0x147, 0x147, 0x147, 0x148, 0x148, 0x148,
    0x148, 0x149, 0x149, 0x149, 0x149, 0x14A, 0x14A, 0x14A, 0x14A, 0x14B, 0x14B, 0x14B, 0x14B,
    0x14C, 0x14C, 0x14C, 0x14C, 0x14D, 0x14D, 0x14D, 0x14D, 0x14E, 0x14E, 0x14E, 0x14E, 0x14F,
    0x14F, 0x14F, 0x14F, 0x14F, 0x150, 0x150, 0x150, 0x150, 0x151, 0x151, 0x151, 0x151, 0x152,
    0x152, 0x152, 0x152, 0x153, 0x153, 0x153, 0x153, 0x154, 0x154, 0x154, 0x154, 0x155, 0x155,
    0x155, 0x155, 0x155, 0x156, 0x156, 0x156, 0x156, 0x157, 0x157, 0x157, 0x157, 0x158, 0x158,
    0x158, 0x158, 0x159, 0x159, 0x159, 0x159, 0x15A, 0x15A, 0x15A, 0x15A, 0x15A, 0x15B, 0x15B,
    0x15B, 0x15B, 0x15C, 0x15C, 0x15C, 0x15C, 0x15D, 0x15D, 0x15D, 0x15D, 0x15E, 0x15E, 0x15E,
    0x15E, 0x15E, 0x15F, 0x15F, 0x15F, 0x15F, 0x160, 0x160, 0x160, 0x160, 0x161, 0x161, 0x161,
    0x161, 0x162, 0x162, 0x162, 0x162, 0x162, 0x163, 0x163, 0x163, 0x163, 0x164, 0x164, 0x164,
    0x164, 0x165, 0x165, 0x165, 0x165, 0x165, 0x166, 0x166, 0x166, 0x166, 0x167, 0x167, 0x167,
    0x167, 0x168, 0x168, 0x168, 0x168, 0x168, 0x169, 0x169, 0x169, 0x169, 0x16A, 0x16A, 0x16A,
    0x16A, 0x16B, 0x16B, 0x16B, 0x16B, 0x16B, 0x16C, 0x16C, 0x16C, 0x16C, 0x16D, 0x16D, 0x16D,
    0x16D, 0x16E, 0x16E, 0x16E, 0x16E, 0x16E, 0x16F, 0x16F, 0x16F, 0x16F, 0x170, 0x170, 0x170,
    0x170, 0x170, 0x171, 0x171, 0x171, 0x171, 0x172, 0x172, 0x172, 0x172, 0x173, 0x173, 0x173,
    0x173, 0x173, 0x174, 0x174, 0x174, 0x174, 0x175, 0x175, 0x175, 0x175, 0x175, 0x176, 0x176,
    0x176, 0x176, 0x177, 0x177, 0x177, 0x177, 0x177, 0x178, 0x178, 0x178, 0x178, 0x179, 0x179,
    0x179, 0x179, 0x179, 0x17A, 0x17A, 0x17A, 0x17A, 0x17B, 0x17B, 0x17B, 0x17B, 0x17B, 0x17C,
    0x17C, 0x17C, 0x17C, 0x17D, 0x17D, 0x17D, 0x17D, 0x17D, 0x17E, 0x17E, 0x17E, 0x17E, 0x17F,
    0x17F, 0x17F, 0x17F, 0x17F, 0x180, 0x180, 0x180, 0x180, 0x181, 0x181, 0x181, 0x181, 0x181,
    0x182, 0x182, 0x182, 0x182, 0x183, 0x183, 0x183, 0x183, 0x183, 0x184, 0x184, 0x184, 0x184,
    0x185, 0x185, 0x185, 0x185, 0x185, 0x186, 0x186, 0x186, 0x186, 0x186, 0x187, 0x187, 0x187,
    0x187, 0x188, 0x188, 0x188, 0x188, 0x188, 0x189, 0x189, 0x189, 0x189, 0x189, 0x18A, 0x18A,
    0x18A, 0x18A, 0x18B, 0x18B, 0x18B, 0x18B, 0x18B, 0x18C, 0x18C, 0x18C, 0x18C, 0x18D, 0x18D,
    0x18D, 0x18D, 0x18D, 0x18E, 0x18E, 0x18E, 0x18E, 0x18E, 0x18F, 0x18F, 0x18F, 0x18F, 0x18F,
    0x190, 0x190, 0x190, 0x190, 0x191, 0x191, 0x191, 0x191, 0x191, 0x192, 0x192, 0x192, 0x192,
    0x192, 0x193, 0x193, 0x193, 0x193, 0x194, 0x194, 0x194, 0x194, 0x194, 0x195, 0x195, 0x195,
    0x195, 0x195, 0x196, 0x196, 0x196, 0x196, 0x196, 0x197, 0x197, 0x197, 0x197, 0x198, 0x198,
    0x198, 0x198, 0x198, 0x199, 0x199, 0x199, 0x199, 0x199, 0x19A, 0x19A, 0x19A, 0x19A, 0x19A,
    0x19B, 0x19B, 0x19B, 0x19B, 0x19B, 0x19C, 0x19C, 0x19C, 0x19C, 0x19D, 0x19D, 0x19D, 0x19D,
    0x19D, 0x19E, 0x19E, 0x19E, 0x19E, 0x19E, 0x19F, 0x19F, 0x19F, 0x19F, 0x19F, 0x1A0, 0x1A0,
    0x1A0, 0x1A0, 0x1A0, 0x1A1, 0x1A1, 0x1A1, 0x1A1, 0x1A1, 0x1A2, 0x1A2, 0x1A2, 0x1A2, 0x1A2,
    0x1A3, 0x1A3, 0x1A3, 0x1A3, 0x1A3, 0x1A4, 0x1A4, 0x1A4, 0x1A4, 0x1A5, 0x1A5, 0x1A5, 0x1A5,
    0x1A5, 0x1A6, 0x1A6, 0x1A6, 0x1A6, 0x1A6, 0x1A7, 0x1A7, 0x1A7, 0x1A7, 0x1A7, 0x1A8, 0x1A8,
    0x1A8, 0x1A8, 0x1A8, 0x1A9, 0x1A9, 0x1A9, 0x1A9, 0x1A9, 0x1AA, 0x1AA, 0x1AA, 0x1AA, 0x1AA,
    0x1AB, 0x1AB, 0x1AB, 0x1AB, 0x1AB, 0x1AC, 0x1AC, 0x1AC, 0x1AC, 0x1AC, 0x1AD, 0x1AD, 0x1AD,
    0x1AD, 0x1AD, 0x1AE, 0x1AE, 0x1AE, 0x1AE, 0x1AE, 0x1AF, 0x1AF, 0x1AF, 0x1AF, 0x1AF, 0x1B0,
    0x1B0, 0x1B0, 0x1B0, 0x1B0, 0x1B1, 0x1B1, 0x1B1, 0x1B1, 0x1B1, 0x1B2, 0x1B2, 0x1B2, 0x1B2,
    0x1B2, 0x1B3, 0x1B3, 0x1B3, 0x1B3, 0x1B3, 0x1B3, 0x1B4, 0x1B4, 0x1B4, 0x1B4, 0x1B4, 0x1B5,
    0x1B5, 0x1B5, 0x1B5, 0x1B5, 0x1B6, 0x1B6, 0x1B6, 0x1B6, 0x1B6, 0x1B7, 0x1B7, 0x1B7, 0x1B7,
    0x1B7, 0x1B8, 0x1B8, 0x1B8, 0x1B8, 0x1B8, 0x1B9, 0x1B9, 0x1B9, 0x1B9, 0x1B9, 0x1BA, 0x1BA,
    0x1BA, 0x1BA, 0x1BA, 0x1BA, 0x1BB, 0x1BB, 0x1BB, 0x1BB, 0x1BB, 0x1BC, 0x1BC, 0x1BC, 0x1BC,
    0x1BC, 0x1BD, 0x1BD, 0x1BD, 0x1BD, 0x1BD, 0x1BE, 0x1BE, 0x1BE, 0x1BE, 0x1BE, 0x1BF, 0x1BF,
    0x1BF, 0x1BF, 0x1BF, 0x1BF, 0x1C0, 0x1C0, 0x1C0, 0x1C0, 0x1C0, 0x1C1, 0x1C1, 0x1C1, 0x1C1,
    0x1C1, 0x1C2, 0x1C2, 0x1C2, 0x1C2, 0x1C2, 0x1C3, 0x1C3, 0x1C3, 0x1C3, 0x1C3, 0x1C3, 0x1C4,
    0x1C4, 0x1C4, 0x1C4, 0x1C4, 0x1C5, 0x1C5, 0x1C5, 0x1C5, 0x1C5, 0x1C6, 0x1C6, 0x1C6, 0x1C6,
    0x1C6, 0x1C6, 0x1C7, 0x1C7, 0x1C7, 0x1C7, 0x1C7, 0x1C8, 0x1C8, 0x1C8, 0x1C8, 0x1C8, 0x1C9,
    0x1C9, 0x1C9, 0x1C9, 0x1C9, 0x1C9, 0x1CA, 0x1CA, 0x1CA, 0x1CA, 0x1CA, 0x1CB, 0x1CB, 0x1CB,
    0x1CB, 0x1CB, 0x1CB, 0x1CC, 0x1CC, 0x1CC, 0x1CC, 0x1CC, 0x1CD, 0x1CD, 0x1CD, 0x1CD, 0x1CD,
    0x1CE, 0x1CE, 0x1CE, 0x1CE, 0x1CE, 0x1CE, 0x1CF, 0x1CF, 0x1CF, 0x1CF, 0x1CF, 0x1D0, 0x1D0,
    0x1D0, 0x1D0, 0x1D0, 0x1D0, 0x1D1, 0x1D1, 0x1D1, 0x1D1, 0x1D1, 0x1D2, 0x1D2, 0x1D2, 0x1D2,
    0x1D2, 0x1D2, 0x1D3, 0x1D3, 0x1D3, 0x1D3, 0x1D3, 0x1D4, 0x1D4, 0x1D4, 0x1D4, 0x1D4, 0x1D4,
    0x1D5, 0x1D5, 0x1D5, 0x1D5, 0x1D5, 0x1D6, 0x1D6, 0x1D6, 0x1D6, 0x1D6, 0x1D6, 0x1D7, 0x1D7,
    0x1D7, 0x1D7, 0x1D7, 0x1D7, 0x1D8, 0x1D8, 0x1D8, 0x1D8, 0x1D8, 0x1D9, 0x1D9, 0x1D9, 0x1D9,
    0x1D9, 0x1D9, 0x1DA, 0x1DA, 0x1DA, 0x1DA, 0x1DA, 0x1DB, 0x1DB, 0x1DB, 0x1DB, 0x1DB, 0x1DB,
    0x1DC, 0x1DC, 0x1DC, 0x1DC, 0x1DC, 0x1DC, 0x1DD, 0x1DD, 0x1DD, 0x1DD, 0x1DD, 0x1DE, 0x1DE,
    0x1DE, 0x1DE, 0x1DE, 0x1DE, 0x1DF, 0x1DF, 0x1DF, 0x1DF, 0x1DF, 0x1DF, 0x1E0, 0x1E0, 0x1E0,
    0x1E0, 0x1E0, 0x1E0, 0x1E1, 0x1E1, 0x1E1, 0x1E1, 0x1E1, 0x1E2, 0x1E2, 0x1E2, 0x1E2, 0x1E2,
    0x1E2, 0x1E3, 0x1E3, 0x1E3, 0x1E3, 0x1E3, 0x1E3, 0x1E4, 0x1E4, 0x1E4, 0x1E4, 0x1E4, 0x1E4,
    0x1E5, 0x1E5, 0x1E5, 0x1E5, 0x1E5, 0x1E6, 0x1E6, 0x1E6, 0x1E6, 0x1E6, 0x1E6, 0x1E7, 0x1E7,
    0x1E7, 0x1E7, 0x1E7, 0x1E7, 0x1E8, 0x1E8, 0x1E8, 0x1E8, 0x1E8, 0x1E8, 0x1E9, 0x1E9, 0x1E9,
    0x1E9, 0x1E9, 0x1E9, 0x1EA, 0x1EA, 0x1EA, 0x1EA, 0x1EA, 0x1EA, 0x1EB, 0x1EB, 0x1EB, 0x1EB,
    0x1EB, 0x1EB, 0x1EC, 0x1EC, 0x1EC, 0x1EC, 0x1EC, 0x1EC, 0x1ED, 0x1ED, 0x1ED, 0x1ED, 0x1ED,
    0x1EE, 0x1EE, 0x1EE, 0x1EE, 0x1EE, 0x1EE, 0x1EF, 0x1EF, 0x1EF, 0x1EF, 0x1EF, 0x1EF, 0x1F0,
    0x1F0, 0x1F0, 0x1F0, 0x1F0, 0x1F0, 0x1F1, 0x1F1, 0x1F1, 0x1F1, 0x1F1, 0x1F1, 0x1F2, 0x1F2,
    0x1F2, 0x1F2, 0x1F2, 0x1F2, 0x1F3, 0x1F3, 0x1F3, 0x1F3, 0x1F3, 0x1F3, 0x1F4, 0x1F4, 0x1F4,
    0x1F4, 0x1F4, 0x1F4, 0x1F5, 0x1F5, 0x1F5, 0x1F5, 0x1F5, 0x1F5, 0x1F5, 0x1F6, 0x1F6, 0x1F6,
    0x1F6, 0x1F6, 0x1F6, 0x1F7, 0x1F7, 0x1F7, 0x1F7, 0x1F7, 0x1F7, 0x1F8, 0x1F8, 0x1F8, 0x1F8,
    0x1F8, 0x1F8, 0x1F9, 0x1F9, 0x1F9, 0x1F9, 0x1F9, 0x1F9, 0x1FA, 0x1FA, 0x1FA, 0x1FA, 0x1FA,
    0x1FA, 0x1FB, 0x1FB, 0x1FB, 0x1FB, 0x1FB, 0x1FB, 0x1FC, 0x1FC, 0x1FC, 0x1FC, 0x1FC, 0x1FC,
    0x1FC, 0x1FD, 0x1FD, 0x1FD, 0x1FD, 0x1FD, 0x1FD, 0x1FE, 0x1FE, 0x1FE, 0x1FE, 0x1FE, 0x1FE,
    0x1FF, 0x1FF, 0x1FF, 0x1FF, 0x1FF, 0x1FF, 0x200, 0x200, 0x200, 0x200, 0,
};
u8 ExportTable[656] = {0};

/* After every object's .data (main.yaml's .sdata entry): the startup code (2MBYTE.OBJ) points
 * $gp here and clears what follows up to HeapStart, the .bss. */
void (*ExportsInit)(void) __attribute__((section(".sdata"))) = export_exe_functions;

/* the .bss, in link order: written into the file as zeros, as the original link did */
s32 StartupRa __attribute__((section(".bss")));   /* where the startup code saves $ra */
s32 SavedSp __attribute__((section(".bss")));
s32 LoadPermanentBuf __attribute__((section(".bss")));
u8 *LoadWaitBuf __attribute__((section(".bss")));
