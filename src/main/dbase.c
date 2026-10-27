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
s32 RingObjectsModel[14] = { (s32)&DatIndex[2 * 794], (s32)&DatIndex[2 * 793], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
u8 AngelModel[56] = {
    0xD0, 0xF9, 4, 0x80, 0xC8, 0xF9, 4, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};
s32 PenguinModel[14] = { (s32)&DatIndex[2 * 798], (s32)&DatIndex[2 * 797], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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
s32 PapuModel[14] = { (s32)&DatIndex[2 * 790], (s32)&DatIndex[2 * 789], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 MinionModel[140] = {
    (s32)&DatIndex[2 * 788], (s32)&DatIndex[2 * 787], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 695],
    (s32)&DatIndex[2 * 694], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 693], (s32)&DatIndex[2 * 692],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 691], (s32)&DatIndex[2 * 690], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 689], (s32)&DatIndex[2 * 688], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 687], (s32)&DatIndex[2 * 686], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 685],
    (s32)&DatIndex[2 * 684], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 683], (s32)&DatIndex[2 * 682],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 681], (s32)&DatIndex[2 * 680], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 679], (s32)&DatIndex[2 * 678], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
    (s32)&DatIndex[2 * 774], (s32)&DatIndex[2 * 773], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 776],
    (s32)&DatIndex[2 * 775], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 894], (s32)&DatIndex[2 * 893],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 892], (s32)&DatIndex[2 * 891], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 890], (s32)&DatIndex[2 * 889], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 888], (s32)&DatIndex[2 * 887], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 886],
    (s32)&DatIndex[2 * 885], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 884], (s32)&DatIndex[2 * 883],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 882], (s32)&DatIndex[2 * 881], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 880], (s32)&DatIndex[2 * 879], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 772], (s32)&DatIndex[2 * 771], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 675],
    (s32)&DatIndex[2 * 674], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 671], (s32)&DatIndex[2 * 670],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 667], (s32)&DatIndex[2 * 666], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 663], (s32)&DatIndex[2 * 662], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 673], (s32)&DatIndex[2 * 672], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 669],
    (s32)&DatIndex[2 * 668], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 665], (s32)&DatIndex[2 * 664],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 661], (s32)&DatIndex[2 * 660], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};
s32 PogoObjectsModel[14] = { (s32)&DatIndex[2 * 770], (s32)&DatIndex[2 * 769], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 RooModel[42] = {
    (s32)&DatIndex[2 * 768], (s32)&DatIndex[2 * 767], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 617],
    (s32)&DatIndex[2 * 616], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 615], (s32)&DatIndex[2 * 614],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 MalletObjectsModel[140] = {
    (s32)&DatIndex[2 * 343], (s32)&DatIndex[2 * 342], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 862],
    (s32)&DatIndex[2 * 861], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 860], (s32)&DatIndex[2 * 859],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 858], (s32)&DatIndex[2 * 857], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 856], (s32)&DatIndex[2 * 855], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 854], (s32)&DatIndex[2 * 853], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 852],
    (s32)&DatIndex[2 * 851], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 850], (s32)&DatIndex[2 * 849],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 848], (s32)&DatIndex[2 * 847], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 613], (s32)&DatIndex[2 * 612], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 OxideModel[14] = { (s32)&DatIndex[2 * 742], (s32)&DatIndex[2 * 741], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 PlayerVehicleModel[42] = {
    (s32)&DatIndex[2 * 740], (s32)&DatIndex[2 * 739], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 609],
    (s32)&DatIndex[2 * 608], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 607], (s32)&DatIndex[2 * 606],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 KegObjectsModel[350] = {
    (s32)&DatIndex[2 * 738], (s32)&DatIndex[2 * 737], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 878],
    (s32)&DatIndex[2 * 877], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 876], (s32)&DatIndex[2 * 875],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 874], (s32)&DatIndex[2 * 873], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 872], (s32)&DatIndex[2 * 871], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 870], (s32)&DatIndex[2 * 869], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 868],
    (s32)&DatIndex[2 * 867], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 866], (s32)&DatIndex[2 * 865],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 864], (s32)&DatIndex[2 * 863], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 659], (s32)&DatIndex[2 * 658], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 657], (s32)&DatIndex[2 * 656], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 655],
    (s32)&DatIndex[2 * 654], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 653], (s32)&DatIndex[2 * 652],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 651], (s32)&DatIndex[2 * 650], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 649], (s32)&DatIndex[2 * 648], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 647], (s32)&DatIndex[2 * 646], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 645],
    (s32)&DatIndex[2 * 644], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 910], (s32)&DatIndex[2 * 909],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 908], (s32)&DatIndex[2 * 907], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 906], (s32)&DatIndex[2 * 905], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 904], (s32)&DatIndex[2 * 903], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 902],
    (s32)&DatIndex[2 * 901], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 900], (s32)&DatIndex[2 * 899],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 898], (s32)&DatIndex[2 * 897], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 896], (s32)&DatIndex[2 * 895], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 BallsEffectsModel[14] = { (s32)&DatIndex[2 * 762], (s32)&DatIndex[2 * 761], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 BallsVehicleModel[14] = { (s32)&DatIndex[2 * 766], (s32)&DatIndex[2 * 765], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 NginModel[56] = {
    (s32)&DatIndex[2 * 764], (s32)&DatIndex[2 * 763], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 627],
    (s32)&DatIndex[2 * 626], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 625], (s32)&DatIndex[2 * 624],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 486], (s32)&DatIndex[2 * 485], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};
s32 PolarModel[14] = { (s32)&DatIndex[2 * 484], (s32)&DatIndex[2 * 483], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
s32 MasksModel[14] = { (s32)&DatIndex[2 * 482], (s32)&DatIndex[2 * 481], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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
    (s32)&DatIndex[2 * 462], (s32)&DatIndex[2 * 461], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 944],
    (s32)&DatIndex[2 * 943], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 476], (s32)&DatIndex[2 * 475],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 928], (s32)&DatIndex[2 * 927], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 930], (s32)&DatIndex[2 * 929], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 460], (s32)&DatIndex[2 * 459], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 458],
    (s32)&DatIndex[2 * 457], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 456], (s32)&DatIndex[2 * 455],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 926], (s32)&DatIndex[2 * 925], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 924], (s32)&DatIndex[2 * 923], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 922], (s32)&DatIndex[2 * 921], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 920],
    (s32)&DatIndex[2 * 919], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 918], (s32)&DatIndex[2 * 917],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 916], (s32)&DatIndex[2 * 915], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 914], (s32)&DatIndex[2 * 913], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 912], (s32)&DatIndex[2 * 911], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 643],
    (s32)&DatIndex[2 * 642], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 830], (s32)&DatIndex[2 * 829],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 828], (s32)&DatIndex[2 * 827], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 826], (s32)&DatIndex[2 * 825], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 824], (s32)&DatIndex[2 * 823], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 822],
    (s32)&DatIndex[2 * 821], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 639], (s32)&DatIndex[2 * 638],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 641], (s32)&DatIndex[2 * 640], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 637], (s32)&DatIndex[2 * 636], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 CrateEffectsModel[364] = {
    (s32)&DatIndex[2 * 760], (s32)&DatIndex[2 * 759], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 818],
    (s32)&DatIndex[2 * 817], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 816], (s32)&DatIndex[2 * 815],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 820], (s32)&DatIndex[2 * 819], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 814], (s32)&DatIndex[2 * 813], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 812], (s32)&DatIndex[2 * 811], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 810],
    (s32)&DatIndex[2 * 809], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 808], (s32)&DatIndex[2 * 807],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 806], (s32)&DatIndex[2 * 805], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 800], (s32)&DatIndex[2 * 799], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 804], (s32)&DatIndex[2 * 803], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 802],
    (s32)&DatIndex[2 * 801], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 635], (s32)&DatIndex[2 * 634],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 633], (s32)&DatIndex[2 * 632], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 631], (s32)&DatIndex[2 * 630], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 629], (s32)&DatIndex[2 * 628], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 623],
    (s32)&DatIndex[2 * 622], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 846], (s32)&DatIndex[2 * 845],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 844], (s32)&DatIndex[2 * 843], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 842], (s32)&DatIndex[2 * 841], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 840], (s32)&DatIndex[2 * 839], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 838],
    (s32)&DatIndex[2 * 837], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 834], (s32)&DatIndex[2 * 833],
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 836], (s32)&DatIndex[2 * 835], 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, (s32)&DatIndex[2 * 832], (s32)&DatIndex[2 * 831], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    (s32)&DatIndex[2 * 621], (s32)&DatIndex[2 * 620], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
