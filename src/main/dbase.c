#include "common.h"
#include "bash/main.h"
#include "bash/dbase.h"

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
        func_80011498(blk->ptr, e->end - e->start, &MainHeap);
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
                mal_free(old->ptr, &MainHeap);
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

void dbase_asload_with_anims_texture_callback(u8 *arg0, obj_dbase *db) {
    _mal_area *heap;
    obj_textureset *set;
    s32 n;

    n = func_8002A650(arg0);
    heap = &MainHeap;
    if (mal_first_free((n + 3) >> 2, heap->free) == NULL) {
        mal_free(arg0, heap);
        db->load = 0;
        func_80016D44((u8 *)db);
    } else {
        set = &db->textures;
        func_8002A5A4((u8 *)set, arg0);
        func_80029560((u8 *)set);
        texture_release_loading_mem(set);
    }
    func_80013650(db->file, func_80016234, db);
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
    func_800115D8(temp_s0, temp_v0, &MainHeap, 0);
    memcpy(*temp_v0, arg1 + (*(s32*)((u8*)(arg0) + 0)), temp_s0);
}

void func_8001636C(u8 *arg0, s32 arg1) {
    u8 *temp_s0;
    u8 *temp_v0;

    temp_v0 = (*(s32*)((u8*)(arg0) + 12));
    temp_s0 = temp_v0 + ((*(s32*)((u8*)(temp_v0) + 60)) + 0x3C) + ((arg1 * 0x10) + 4);
    if ((*(s32*)((u8*)(temp_s0) + 8)) != 0) {
        mal_free((void *)(*(s32*)((u8*)(temp_s0) + 8)), &MainHeap);
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
                mal_free(*(void **)blk, &MainHeap);
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

s32 dbase_mesh_anim_name_loaded(obj_dbase *db, char *arg1) {
    u8 *tbl;
    u8 *p;
    s32 n;
    s32 key;
    u8 *res;

    tbl = db->data;
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
                        mal_free(*(void **)e, &MainHeap);
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
                    mal_free(item[0], &MainHeap);
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
        mal_free(*(void **)node, &MainHeap);
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
        func_80010AE8(&MainHeap);
        func_8002BAE8();
    }
done:
    DrawSync(0);
    while (func_80011DB4(&MainHeap) != NULL) {
    }
}

void func_80016D44(u8 *arg0) {
    if (((*(s32*)((u8*)(arg0) + 8)) == 0) && ((*(s32*)((u8*)(arg0) + 12)) == 0)) {
        (*(s32*)((u8*)(arg0) + 8)) = func_80013650((*(s32*)((u8*)(arg0) + 4)), func_80016D98, arg0);
    }
}

void func_80016D98(u8 *data, u8 *rec) {
    _mal_area *heap;
    s32 size;

    size = func_8002A650(data);
    heap = &MainHeap;
    if (mal_first_free((size + 3) >> 2, heap->free) == NULL) {
        mal_free(data, heap);
        *(s32 *)(rec + 8) = 0;
        func_80016D44(rec);
    } else {
        func_8002A5A4(rec + 0x10, data);
        func_80029560(rec + 0x10);
        texture_release_loading_mem((obj_textureset *)(rec + 0x10));
    }
    *(s32 *)(rec + 8) = func_80013034(*(s32 *)(rec + 0), 0, *(s32 *)(rec + 0x24), func_80016F34, rec, 1, 0);
}

void func_80016E5C(u8 *arg0, u8 *arg1) {
    _mal_area *heap;
    s32 n;

    n = func_8002A650(arg0);
    heap = &MainHeap;
    if (mal_first_free((n + 3) >> 2, heap->free) == NULL) {
        mal_free(arg0, heap);
        *(s32 *)(arg1 + 8) = 0;
        func_80016D44(arg1);
    } else {
        arg1 += 0x10;
        func_8002A5A4(arg1, arg0);
        func_80029560(arg1);
        texture_release_loading_mem((obj_textureset *)arg1);
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
        mal_free(*(void **)(arg0 + 0xC), &MainHeap);
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
    base = HeroModelBank;
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
