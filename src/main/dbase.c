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
    func_80016CA0(FontModel);
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

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */
extern u8 D_8004EBC0[];
extern u8 D_8004EBC8[];
extern u8 D_8004EF48[];
extern u8 D_8004EF50[];
extern u8 D_8004EF58[];
extern u8 D_8004EF60[];
extern u8 D_8004EF68[];
extern u8 D_8004EF70[];
extern u8 D_8004EF78[];
extern u8 D_8004EF80[];
extern u8 D_8004EFE8[];
extern u8 D_8004EFF0[];
extern u8 D_8004F018[];
extern u8 D_8004F020[];
extern u8 D_8004F028[];
extern u8 D_8004F030[];
extern u8 D_8004F038[];
extern u8 D_8004F040[];
extern u8 D_8004F400[];
extern u8 D_8004F408[];
extern u8 D_8004F410[];
extern u8 D_8004F418[];
extern u8 D_8004F430[];
extern u8 D_8004F438[];
extern u8 D_8004F440[];
extern u8 D_8004F448[];
extern u8 D_8004F450[];
extern u8 D_8004F458[];
extern u8 D_8004F470[];
extern u8 D_8004F478[];
extern u8 D_8004F480[];
extern u8 D_8004F488[];
extern u8 D_8004F490[];
extern u8 D_8004F498[];
extern u8 D_8004F4A0[];
extern u8 D_8004F4A8[];
extern u8 D_8004F4B0[];
extern u8 D_8004F4B8[];
extern u8 D_8004F4C0[];
extern u8 D_8004F4C8[];
extern u8 D_8004F4D0[];
extern u8 D_8004F4D8[];
extern u8 D_8004F4E0[];
extern u8 D_8004F4E8[];
extern u8 D_8004F4F0[];
extern u8 D_8004F4F8[];
extern u8 D_8004F500[];
extern u8 D_8004F508[];
extern u8 D_8004F510[];
extern u8 D_8004F518[];
extern u8 D_8004F520[];
extern u8 D_8004F528[];
extern u8 D_8004F530[];
extern u8 D_8004F538[];
extern u8 D_8004F540[];
extern u8 D_8004F548[];
extern u8 D_8004F550[];
extern u8 D_8004F558[];
extern u8 D_8004F560[];
extern u8 D_8004F568[];
extern u8 D_8004F570[];
extern u8 D_8004F578[];
extern u8 D_8004F580[];
extern u8 D_8004F588[];
extern u8 D_8004F590[];
extern u8 D_8004F598[];
extern u8 D_8004F5A0[];
extern u8 D_8004F5A8[];
extern u8 D_8004F5B0[];
extern u8 D_8004F5B8[];
extern u8 D_8004F5C0[];
extern u8 D_8004F5C8[];
extern u8 D_8004F5D0[];
extern u8 D_8004F5D8[];
extern u8 D_8004F5E0[];
extern u8 D_8004F5E8[];
extern u8 D_8004F5F0[];
extern u8 D_8004F5F8[];
extern u8 D_8004F600[];
extern u8 D_8004F608[];
extern u8 D_8004F610[];
extern u8 D_8004F618[];
extern u8 D_8004F620[];
extern u8 D_8004F628[];
extern u8 D_8004F640[];
extern u8 D_8004F648[];
extern u8 D_8004F650[];
extern u8 D_8004F658[];
extern u8 D_8004F660[];
extern u8 D_8004F668[];
extern u8 D_8004F670[];
extern u8 D_8004F678[];
extern u8 D_8004F680[];
extern u8 D_8004F688[];
extern u8 D_8004F690[];
extern u8 D_8004F698[];
extern u8 D_8004F6A0[];
extern u8 D_8004F6A8[];
extern u8 D_8004F6B0[];
extern u8 D_8004F6B8[];
extern u8 D_8004F6C0[];
extern u8 D_8004F6C8[];
extern u8 D_8004F818[];
extern u8 D_8004F820[];
extern u8 D_8004F828[];
extern u8 D_8004F830[];
extern u8 D_8004F838[];
extern u8 D_8004F840[];
extern u8 D_8004F8C8[];
extern u8 D_8004F8D0[];
extern u8 D_8004F8D8[];
extern u8 D_8004F8E0[];
extern u8 D_8004F8E8[];
extern u8 D_8004F8F0[];
extern u8 D_8004F8F8[];
extern u8 D_8004F900[];
extern u8 D_8004F908[];
extern u8 D_8004F910[];
extern u8 D_8004F918[];
extern u8 D_8004F920[];
extern u8 D_8004F928[];
extern u8 D_8004F930[];
extern u8 D_8004F938[];
extern u8 D_8004F940[];
extern u8 D_8004F948[];
extern u8 D_8004F950[];
extern u8 D_8004F9A8[];
extern u8 D_8004F9B0[];
extern u8 D_8004F9B8[];
extern u8 D_8004F9C0[];
extern u8 D_8004F9D8[];
extern u8 D_8004F9E0[];
extern u8 D_8004F9F8[];
extern u8 D_8004FA00[];
extern u8 D_8004FA08[];
extern u8 D_8004FA10[];
extern u8 D_8004FA18[];
extern u8 D_8004FA20[];
extern u8 D_8004FA28[];
extern u8 D_8004FA30[];
extern u8 D_8004FA38[];
extern u8 D_8004FA40[];
extern u8 D_8004FA48[];
extern u8 D_8004FA50[];
extern u8 D_8004FA58[];
extern u8 D_8004FA60[];
extern u8 D_8004FA68[];
extern u8 D_8004FA70[];
extern u8 D_8004FA78[];
extern u8 D_8004FA80[];
extern u8 D_8004FA88[];
extern u8 D_8004FA90[];
extern u8 D_8004FA98[];
extern u8 D_8004FAA0[];
extern u8 D_8004FAA8[];
extern u8 D_8004FAB0[];
extern u8 D_8004FAB8[];
extern u8 D_8004FAC0[];
extern u8 D_8004FAC8[];
extern u8 D_8004FAD0[];
extern u8 D_8004FAD8[];
extern u8 D_8004FAE0[];
extern u8 D_8004FAE8[];
extern u8 D_8004FAF0[];
extern u8 D_8004FAF8[];
extern u8 D_8004FB00[];
extern u8 D_8004FB08[];
extern u8 D_8004FB10[];
extern u8 D_8004FB18[];
extern u8 D_8004FB20[];
extern u8 D_8004FB28[];
extern u8 D_8004FB30[];
extern u8 D_8004FB38[];
extern u8 D_8004FB40[];
extern u8 D_8004FB48[];
extern u8 D_8004FB50[];
extern u8 D_8004FB58[];
extern u8 D_8004FB60[];
extern u8 D_8004FB68[];
extern u8 D_8004FB70[];
extern u8 D_8004FB78[];
extern u8 D_8004FB80[];
extern u8 D_8004FB88[];
extern u8 D_8004FB90[];
extern u8 D_8004FB98[];
extern u8 D_8004FBA0[];
extern u8 D_8004FBA8[];
extern u8 D_8004FBB0[];
extern u8 D_8004FBB8[];
extern u8 D_8004FBC0[];
extern u8 D_8004FBC8[];
extern u8 D_8004FBD0[];
extern u8 D_8004FBD8[];
extern u8 D_8004FBE0[];
extern u8 D_8004FBE8[];
extern u8 D_8004FBF0[];
extern u8 D_8004FBF8[];
extern u8 D_8004FC00[];
extern u8 D_8004FC08[];
extern u8 D_8004FC10[];
extern u8 D_8004FC18[];
extern u8 D_8004FC20[];
extern u8 D_8004FC28[];
extern u8 D_8004FC30[];
extern u8 D_8004FC38[];
extern u8 D_8004FC40[];
extern u8 D_8004FC48[];
extern u8 D_8004FC50[];
extern u8 D_8004FC58[];
extern u8 D_8004FC60[];
extern u8 D_8004FC68[];
extern u8 D_8004FC70[];
extern u8 D_8004FC78[];
extern u8 D_8004FC80[];
extern u8 D_8004FC88[];
extern u8 D_8004FC90[];
extern u8 D_8004FC98[];
extern u8 D_8004FCA0[];
extern u8 D_8004FCA8[];
extern u8 D_8004FCB0[];
extern u8 D_8004FCB8[];
extern u8 D_8004FCC0[];
extern u8 D_8004FCC8[];
extern u8 D_8004FCD0[];
extern u8 D_8004FCD8[];
extern u8 D_8004FCE0[];
extern u8 D_8004FCE8[];
extern u8 D_8004FCF0[];
extern u8 D_8004FCF8[];
extern u8 D_8004FD00[];
extern u8 D_8004FD08[];
extern u8 D_8004FD10[];
extern u8 D_8004FD18[];
extern u8 D_8004FD20[];
extern u8 D_8004FD28[];
extern u8 D_8004FD30[];
extern u8 D_8004FD38[];
extern u8 D_8004FD40[];
extern u8 D_8004FD48[];
extern u8 D_8004FD50[];
extern u8 D_8004FD58[];
extern u8 D_8004FD60[];
extern u8 D_8004FD68[];
extern u8 D_8004FD70[];
extern u8 D_8004FD78[];
extern u8 D_8004FD80[];
extern u8 D_8004FD88[];
extern u8 D_8004FD90[];
extern u8 D_8004FD98[];
extern u8 D_8004FDA0[];
extern u8 D_8004FDA8[];
extern u8 D_8004FDB0[];
extern u8 D_8004FDB8[];
extern u8 D_8004FDC0[];
extern u8 D_8004FDC8[];
extern u8 D_8004FDD0[];
extern u8 D_8004FDD8[];
extern u8 D_8004FDE0[];
extern u8 D_8004FDE8[];
extern u8 D_8004FDF0[];
extern u8 D_8004FDF8[];
extern u8 D_8004FE00[];
extern u8 D_8004FE08[];
extern u8 D_8004FE10[];
extern u8 D_8004FE18[];
extern u8 D_8004FE20[];
extern u8 D_8004FE88[];
extern u8 D_8004FE90[];

u8 HeroModelBank[1288] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
    0x80, 0xF8, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xF0,
    0xFF, 4, 0x80, 0xE8, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0xE0, 0xFF, 4, 0x80, 0xD8, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0xD0, 0xFF, 4, 0x80, 0xC8, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0xC0, 0xFF, 4, 0x80, 0xB8, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0x90, 0xFF, 4, 0x80, 0x88, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0xB0, 0xFF, 4, 0x80, 0xA8, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xA0, 0xFF, 4, 0x80, 0x98, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xFF, 4, 0x80, 0x78, 0xFF, 4, 0x80, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x70, 0xFF, 4, 0x80, 0x68, 0xFF, 4, 0x80, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x60, 0xFF, 4, 0x80, 0x58, 0xFF, 4, 0x80,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x50, 0xFF, 4, 0x80, 0x48, 0xFF, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x40, 0xFF, 4, 0x80, 0x38,
    0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x20, 0xFF, 4, 0x80,
    0x18, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30, 0xFF, 4,
    0x80, 0x28, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10,
    0xFF, 4, 0x80, 8, 0xFF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0xFF, 4, 0x80, 0xF8, 0xFE, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0xF0, 0xFE, 4, 0x80, 0xE8, 0xFE, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0xE0, 0xFE, 4, 0x80, 0xD8, 0xFE, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0xD0, 0xFE, 4, 0x80, 0xC8, 0xFE, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0xC0, 0xFE, 4, 0x80, 0xB8, 0xFE, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0xB0, 0xFE, 4, 0x80, 0xA8, 0xFE, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u8 PowerupsModel[56] = {
    0x10, 0xF8, 4, 0x80, 8, 0xF8, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};
u8 CrateObjectsModel[56] = {
    0xF0, 0xF9, 4, 0x80, 0xE8, 0xF9, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};
s32 RingObjectsModel[14] = { (s32)&D_8004F9E0, (s32)&D_8004F9D8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
u8 AngelModel[56] = {
    0xD0, 0xF9, 4, 0x80, 0xC8, 0xF9, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};
s32 PenguinModel[14] = { (s32)&D_8004FA00, (s32)&D_8004F9F8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
u8 InterfaceModel[896] = {
    0xF0, 0xF7, 4, 0x80, 0xE8, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0xF0, 0xF7, 4, 0x80, 0xE8, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0xF0, 0xF7, 4, 0x80, 0xE8, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0xF0, 0xF7, 4, 0x80, 0xE8, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0xF0, 0xF7, 4, 0x80, 0xE8, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xF0, 0xF7, 4, 0x80, 0xE8, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x48, 0xF7, 4, 0x80, 0x40, 0xF7, 4, 0x80, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x38, 0xF7, 4, 0x80, 0x30, 0xF7, 4, 0x80, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x68, 0xF7, 4, 0x80, 0x60, 0xF7, 4, 0x80,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x58, 0xF7, 4, 0x80, 0x50, 0xF7, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x28, 0xF7, 4, 0x80, 0x20,
    0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x18, 0xF7, 4, 0x80,
    0x10, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0xF7, 4,
    0x80, 0, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xF8, 0xF6,
    4, 0x80, 0xF0, 0xF6, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xE8,
    0xF6, 4, 0x80, 0xE0, 0xF6, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0xD8, 0xF6, 4, 0x80, 0xD0, 0xF6, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};
s32 PapuModel[14] = { (s32)&D_8004F9C0, (s32)&D_8004F9B8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 MinionModel[140] = {
    (s32)&D_8004F9B0, (s32)&D_8004F9A8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F6C8,
    (s32)&D_8004F6C0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F6B8, (s32)&D_8004F6B0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F6A8, (s32)&D_8004F6A0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004F698, (s32)&D_8004F690, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004F688, (s32)&D_8004F680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F678,
    (s32)&D_8004F670, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F668, (s32)&D_8004F660,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F658, (s32)&D_8004F650, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004F648, (s32)&D_8004F640, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u8 TankObjectsModel[56] = {
    0xA0, 0xF9, 4, 0x80, 0x98, 0xF9, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};
u8 TankObjects2Model[224] = {
    0x90, 0xF9, 4, 0x80, 0x88, 0xF9, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0x80, 0xF9, 4, 0x80, 0x78, 0xF9, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x70, 0xF9, 4, 0x80, 0x68, 0xF9, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0x60, 0xF9, 4, 0x80, 0x58, 0xF9, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
s32 TankVehicleModel[266] = {
    (s32)&D_8004F940, (s32)&D_8004F938, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F950,
    (s32)&D_8004F948, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FD00, (s32)&D_8004FCF8,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FCF0, (s32)&D_8004FCE8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FCE0, (s32)&D_8004FCD8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FCD0, (s32)&D_8004FCC8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FCC0,
    (s32)&D_8004FCB8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FCB0, (s32)&D_8004FCA8,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FCA0, (s32)&D_8004FC98, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FC90, (s32)&D_8004FC88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004F930, (s32)&D_8004F928, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F628,
    (s32)&D_8004F620, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F608, (s32)&D_8004F600,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F5E8, (s32)&D_8004F5E0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004F5C8, (s32)&D_8004F5C0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004F618, (s32)&D_8004F610, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F5F8,
    (s32)&D_8004F5F0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F5D8, (s32)&D_8004F5D0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F5B8, (s32)&D_8004F5B0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};
s32 PogoObjectsModel[14] = { (s32)&D_8004F920, (s32)&D_8004F918, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 RooModel[42] = {
    (s32)&D_8004F910, (s32)&D_8004F908, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F458,
    (s32)&D_8004F450, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F448, (s32)&D_8004F440,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 MalletObjectsModel[140] = {
    (s32)&D_8004EBC8, (s32)&D_8004EBC0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FC00,
    (s32)&D_8004FBF8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FBF0, (s32)&D_8004FBE8,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FBE0, (s32)&D_8004FBD8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FBD0, (s32)&D_8004FBC8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FBC0, (s32)&D_8004FBB8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FBB0,
    (s32)&D_8004FBA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FBA0, (s32)&D_8004FB98,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FB90, (s32)&D_8004FB88, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004F438, (s32)&D_8004F430, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 OxideModel[14] = { (s32)&D_8004F840, (s32)&D_8004F838, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 PlayerVehicleModel[42] = {
    (s32)&D_8004F830, (s32)&D_8004F828, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F418,
    (s32)&D_8004F410, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F408, (s32)&D_8004F400,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 KegObjectsModel[350] = {
    (s32)&D_8004F820, (s32)&D_8004F818, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FC80,
    (s32)&D_8004FC78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FC70, (s32)&D_8004FC68,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FC60, (s32)&D_8004FC58, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FC50, (s32)&D_8004FC48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FC40, (s32)&D_8004FC38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FC30,
    (s32)&D_8004FC28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FC20, (s32)&D_8004FC18,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FC10, (s32)&D_8004FC08, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004F5A8, (s32)&D_8004F5A0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004F598, (s32)&D_8004F590, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F588,
    (s32)&D_8004F580, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F578, (s32)&D_8004F570,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F568, (s32)&D_8004F560, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004F558, (s32)&D_8004F550, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004F548, (s32)&D_8004F540, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F538,
    (s32)&D_8004F530, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FD80, (s32)&D_8004FD78,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FD70, (s32)&D_8004FD68, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FD60, (s32)&D_8004FD58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FD50, (s32)&D_8004FD48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FD40,
    (s32)&D_8004FD38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FD30, (s32)&D_8004FD28,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FD20, (s32)&D_8004FD18, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FD10, (s32)&D_8004FD08, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 BallsEffectsModel[14] = { (s32)&D_8004F8E0, (s32)&D_8004F8D8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 BallsVehicleModel[14] = { (s32)&D_8004F900, (s32)&D_8004F8F8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 NginModel[56] = {
    (s32)&D_8004F8F0, (s32)&D_8004F8E8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F4A8,
    (s32)&D_8004F4A0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F498, (s32)&D_8004F490,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F040, (s32)&D_8004F038, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};
s32 PolarModel[14] = { (s32)&D_8004F030, (s32)&D_8004F028, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 MasksModel[14] = { (s32)&D_8004F020, (s32)&D_8004F018, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
u8 PolarObjectsModel[840] = {
    0x10, 0xF0, 4, 0x80, 8, 0xF0, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0xF0, 4, 0x80, 0xF8, 0xEF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0xE0, 0xEF, 4, 0x80, 0xD8, 0xEF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0xB0, 0xEF, 4, 0x80, 0xA8, 0xEF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0xA0, 0xEF, 4, 0x80, 0x98, 0xEF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xFE, 4, 0x80, 0x78, 0xFE, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x70, 0xFE, 4, 0x80, 0x68, 0xFE, 4, 0x80, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x60, 0xFE, 4, 0x80, 0x58, 0xFE, 4, 0x80, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x50, 0xFE, 4, 0x80, 0x48, 0xFE, 4, 0x80, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x40, 0xFE, 4, 0x80, 0x38, 0xFE, 4, 0x80,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30, 0xFE, 4, 0x80, 0x28, 0xFE, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xD0, 0xEF, 4, 0x80, 0xC8,
    0xEF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x90, 0xEF, 4, 0x80,
    0x88, 0xEF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xC0, 0xEF, 4,
    0x80, 0xB8, 0xEF, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xA0,
    0xFE, 4, 0x80, 0x98, 0xFE, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 RoboPolarModel[350] = {
    (s32)&D_8004EF80, (s32)&D_8004EF78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FE90,
    (s32)&D_8004FE88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004EFF0, (s32)&D_8004EFE8,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FE10, (s32)&D_8004FE08, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FE20, (s32)&D_8004FE18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004EF70, (s32)&D_8004EF68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004EF60,
    (s32)&D_8004EF58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004EF50, (s32)&D_8004EF48,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FE00, (s32)&D_8004FDF8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FDF0, (s32)&D_8004FDE8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FDE0, (s32)&D_8004FDD8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FDD0,
    (s32)&D_8004FDC8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FDC0, (s32)&D_8004FDB8,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FDB0, (s32)&D_8004FDA8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FDA0, (s32)&D_8004FD98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FD90, (s32)&D_8004FD88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F528,
    (s32)&D_8004F520, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FB00, (s32)&D_8004FAF8,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FAF0, (s32)&D_8004FAE8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FAE0, (s32)&D_8004FAD8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FAD0, (s32)&D_8004FAC8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FAC0,
    (s32)&D_8004FAB8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F508, (s32)&D_8004F500,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F518, (s32)&D_8004F510, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004F4F8, (s32)&D_8004F4F0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 CrateEffectsModel[364] = {
    (s32)&D_8004F8D0, (s32)&D_8004F8C8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FAA0,
    (s32)&D_8004FA98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FA90, (s32)&D_8004FA88,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FAB0, (s32)&D_8004FAA8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FA80, (s32)&D_8004FA78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FA70, (s32)&D_8004FA68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FA60,
    (s32)&D_8004FA58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FA50, (s32)&D_8004FA48,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FA40, (s32)&D_8004FA38, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FA10, (s32)&D_8004FA08, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FA30, (s32)&D_8004FA28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FA20,
    (s32)&D_8004FA18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F4E8, (s32)&D_8004F4E0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F4D8, (s32)&D_8004F4D0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004F4C8, (s32)&D_8004F4C0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004F4B8, (s32)&D_8004F4B0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004F488,
    (s32)&D_8004F480, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FB80, (s32)&D_8004FB78,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FB70, (s32)&D_8004FB68, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FB60, (s32)&D_8004FB58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004FB50, (s32)&D_8004FB48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FB40,
    (s32)&D_8004FB38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FB20, (s32)&D_8004FB18,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_8004FB30, (s32)&D_8004FB28, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&D_8004FB10, (s32)&D_8004FB08, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&D_8004F478, (s32)&D_8004F470, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u8 DragonObjectsModel[112] = {
    0x68, 0xF4, 4, 0x80, 0x60, 0xF4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0x48, 0xE5, 4, 0x80, 0x40, 0xE5, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};
u8 LoadingModel[224] = {
    0xE0, 0xF7, 4, 0x80, 0xD8, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0x28, 0xE5, 4, 0x80, 0x20, 0xE5, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x18, 0xE5, 4, 0x80, 0x10, 0xE5, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 8, 0xE5, 4, 0x80, 0, 0xE5, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};
u8 FontModel[616] = {
    0, 0xF8, 4, 0x80, 0xF8, 0xF7, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0xF8, 0xE4, 4, 0x80, 0xF0, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0xE8, 0xE4, 4, 0x80, 0xE0, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0xE8, 0xEC, 4, 0x80, 0xE0, 0xEC, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0x58, 0xED, 4, 0x80, 0x50, 0xED, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0x48, 0xED, 4, 0x80, 0x40, 0xED, 4, 0x80, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x38, 0xED, 4, 0x80, 0x30, 0xED, 4, 0x80, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x28, 0xED, 4, 0x80, 0x20, 0xED, 4, 0x80, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x18, 0xED, 4, 0x80, 0x10, 0xED, 4, 0x80, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0xED, 4, 0x80, 0, 0xED, 4, 0x80, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xF8, 0xEC, 4, 0x80, 0xF0, 0xEC, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u8 AdventureItemsModel[3920] = {
    0xD8, 0xEC, 4, 0x80, 0xD0, 0xEC, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0xC0, 0xF8, 4, 0x80, 0xB8, 0xF8, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0xB0, 0xF8, 4, 0x80, 0xA8, 0xF8, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0xA0, 0xF8, 4, 0x80, 0x98, 0xF8, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0x90, 0xF8, 4, 0x80, 0x88, 0xF8, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0xF8, 4, 0x80, 0x78, 0xF8, 4, 0x80, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x70, 0xF8, 4, 0x80, 0x68, 0xF8, 4, 0x80, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x60, 0xF8, 4, 0x80, 0x58, 0xF8, 4, 0x80, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x50, 0xF8, 4, 0x80, 0x48, 0xF8, 4, 0x80,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xD8, 0xE4, 4, 0x80, 0xD0, 0xE4, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xC8, 0xE4, 4, 0x80, 0xC0,
    0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xB8, 0xE4, 4, 0x80,
    0xB0, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xA8, 0xE4, 4,
    0x80, 0xA0, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x98,
    0xE4, 4, 0x80, 0x90, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x88, 0xE4, 4, 0x80, 0x80, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0x78, 0xE4, 4, 0x80, 0x70, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x68, 0xE4, 4, 0x80, 0x60, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0x58, 0xE4, 4, 0x80, 0x50, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0x48, 0xE4, 4, 0x80, 0x40, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x38, 0xE4, 4, 0x80, 0x30, 0xE4, 4, 0x80, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x28, 0xE4, 4, 0x80, 0x20, 0xE4, 4, 0x80, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x18, 0xE4, 4, 0x80, 0x10, 0xE4, 4, 0x80, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0xE4, 4, 0x80, 0, 0xE4, 4, 0x80, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xF8, 0xE3, 4, 0x80, 0xF0, 0xE3, 4, 0x80,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xE8, 0xE3, 4, 0x80, 0xE0, 0xE3, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xD8, 0xE3, 4, 0x80, 0xD0,
    0xE3, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xC8, 0xE3, 4, 0x80,
    0xC0, 0xE3, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xB8, 0xE3, 4,
    0x80, 0xB0, 0xE3, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xA8,
    0xE3, 4, 0x80, 0xA0, 0xE3, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x38, 0xE2, 4, 0x80, 0x30, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0x28, 0xE2, 4, 0x80, 0x20, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x18, 0xE2, 4, 0x80, 0x10, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 8, 0xE2, 4, 0x80, 0, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0xF8, 0xE2, 4, 0x80, 0xF0, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0xE8, 0xE2, 4, 0x80, 0xE0, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xD8, 0xE2, 4, 0x80, 0xD0, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xC8, 0xE2, 4, 0x80, 0xC0, 0xE2, 4, 0x80, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xB8, 0xE2, 4, 0x80, 0xB0, 0xE2, 4, 0x80, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xA8, 0xE2, 4, 0x80, 0xA0, 0xE2, 4, 0x80,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x98, 0xE2, 4, 0x80, 0x90, 0xE2, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x88, 0xE2, 4, 0x80, 0x80,
    0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x78, 0xE2, 4, 0x80,
    0x70, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x68, 0xE2, 4,
    0x80, 0x60, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x58,
    0xE2, 4, 0x80, 0x50, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x48, 0xE2, 4, 0x80, 0x40, 0xE2, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0x98, 0xE3, 4, 0x80, 0x90, 0xE3, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x88, 0xE3, 4, 0x80, 0x80, 0xE3, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0x78, 0xE3, 4, 0x80, 0x70, 0xE3, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0x68, 0xE3, 4, 0x80, 0x60, 0xE3, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x58, 0xE3, 4, 0x80, 0x50, 0xE3, 4, 0x80, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x48, 0xE3, 4, 0x80, 0x40, 0xE3, 4, 0x80, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x38, 0xE3, 4, 0x80, 0x30, 0xE3, 4, 0x80, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x28, 0xE3, 4, 0x80, 0x20, 0xE3, 4, 0x80,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x18, 0xE3, 4, 0x80, 0x10, 0xE3, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0xE3, 4, 0x80, 0, 0xE3, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xE8, 0xE1, 4, 0x80, 0xE0,
    0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xF8, 0xE1, 4, 0x80,
    0xF0, 0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xD8, 0xE1, 4,
    0x80, 0xD0, 0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xC8,
    0xE1, 4, 0x80, 0xC0, 0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0xB8, 0xE1, 4, 0x80, 0xB0, 0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0x48, 0xE1, 4, 0x80, 0x40, 0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x38, 0xE1, 4, 0x80, 0x30, 0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0x28, 0xE1, 4, 0x80, 0x20, 0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0x18, 0xE1, 4, 0x80, 0x10, 0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x88, 0xE1, 4, 0x80, 0x80, 0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x78, 0xE1, 4, 0x80, 0x70, 0xE1, 4, 0x80, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x68, 0xE1, 4, 0x80, 0x60, 0xE1, 4, 0x80, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x58, 0xE1, 4, 0x80, 0x50, 0xE1, 4, 0x80,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xA8, 0xE1, 4, 0x80, 0xA0, 0xE1, 4,
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x98, 0xE1, 4, 0x80, 0x90,
    0xE1, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
