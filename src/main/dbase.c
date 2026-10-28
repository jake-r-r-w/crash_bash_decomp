#include "common.h"
#include "bash/main.h"
#include "bash/dbase.h"

void dbase_setup_blocks(u8 *arg0, s32 *arg1) {
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
        blk = smallnode_alloc();
        e->block = blk;
        blk->ptr = mal_split_ptr(arg0, e->start);
        i--;
        mal_set_owner(blk->ptr, &blk->ptr);
        mal_trim(blk->ptr, e->end - e->start, &MainHeap);
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
                    if (string_hash((u8 *)*p) == e->hash) {
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
                smallnode_free((u8 *)old);
            }
            e++;
            i--;
        }
    }
end:
}

u8 *dbase_texture_entry(u8 *arg0, s32 i) {
    if (arg0 != NULL) {
        arg0 += 0x10;
        if (*(s32 *)(arg0 + 8) != 0) {
            return *(u8 **)(arg0 + 0xC) + i * 0xC;
        }
    }
    return NULL;
}

s32 dbase_texture_ptr(u8 *arg0, s32 arg1) {
    s32 temp_a0;

    if (arg0 != NULL) {
        temp_a0 = (*(s32*)((u8*)(arg0) + 24));
        if (temp_a0 != 0) {
            return temp_a0 + ((arg1 & 0xFFF) * 0x38);
        }
    }
    return 0;
}

void dbase_asload_with_anims(obj_dbase *db, s32 *keep) {
    if (db->load == 0 && db->data == NULL) {
        db->keep = keep;
        db->load = load_file_cb(db->textureFile, dbase_asload_with_anims_texture_callback, db);
    }
}

void dbase_asload_with_anims_texture_callback(u8 *arg0, obj_dbase *db) {
    _mal_area *heap;
    obj_textureset *set;
    s32 n;

    n = textureset_mem_size(arg0);
    heap = &MainHeap;
    if (mal_first_free((n + 3) >> 2, heap->free) == NULL) {
        mal_free(arg0, heap);
        db->load = 0;
        dbase_load_textures(db);
    } else {
        set = &db->textures;
        textureset_init((u8 *)set, arg0);
        textureset_upload((u8 *)set);
        texture_release_loading_mem(set);
    }
    load_file_cb(db->file, dbase_asload_with_anims_callback, db);
}

void dbase_asload_with_anims_callback(u8 *arg0, u8 *arg1) {
    mal_set_owner(arg0, arg1 + 0xC);
    dbase_setup_blocks(arg0, (*(s32*)((u8*)(arg1) + 52)));
}

s32 name_list_find(s32 *list, u8 *arg1) {
    if (list != NULL) {
        for (;; list++) {
            if (*list == 0) {
                return 0;
            }
            if (string_hash((u8 *)*list) == *(s32 *)(arg1 + 0x10)) {
                return *list;
            }
        }
    }
    return 0;
}

void dbase_copy_block(u8 *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_s0;

    temp_v0 = smallnode_alloc();
    (*(s32*)((u8*)(arg0) + 20)) = temp_v0;
    temp_s0 = (*(s32*)((u8*)(arg0) + 4)) - (*(s32*)((u8*)(arg0) + 0));
    mal_can_alloc(temp_s0);
    mal_alloc_owned_low(temp_s0, temp_v0, &MainHeap, 0);
    memcpy(*temp_v0, arg1 + (*(s32*)((u8*)(arg0) + 0)), temp_s0);
}

void dbase_anim_release(obj_dbase *db, s32 i) {
    DbaseAnim *a;
    u8 *d;

    d = db->data;
    a = (DbaseAnim *)(d + (*(s32 *)(d + 0x3C) + 0x3C) + (i * 0x10 + 4));
    if (a->data != NULL) {
        mal_free(a->data, &MainHeap);
        a->load = 0;
    } else if (a->load != 0) {
        loader_cancel((LoadNode *)a->load);
        a->load = 0;
    }
}


void dbase_anim_load(obj_dbase *db, s32 i) {
    s32 start;
    DbaseAnim *a;
    u8 *d;

    d = db->data;
    a = (DbaseAnim *)(d + (*(s32 *)(d + 0x3C) + 0x3C) + (i * 0x10 + 4));
    if (a->data == NULL && a->load == 0) {
        start = a->start;
        a->load = asload_chunk_relocatable(db->file, start, &a->data, a->end - start);
    }
}

s32 dbase_anim_ptr(u8 *arg0, s32 arg1) {
    u8 *temp_s0;
    u8 *temp_v0;

    temp_v0 = (*(s32*)((u8*)(arg0) + 12));
    temp_s0 = temp_v0 + ((*(s32*)((u8*)(temp_v0) + 60)) + 0x3C) + ((arg1 * 0x10) + 4);
    return (*(s32*)((u8*)(temp_s0) + 8));
}


u8 *dbase_mesh_anim_name_release(u8 *arg0, u8 *arg1) {
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
        key = string_hash(arg1);
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
                loader_cancel(*(LoadNode **)(blk + 4));
            }
            smallnode_free(blk);
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
        key = string_hash(arg1);
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

u8 *dbase_mesh_anim_name_load(u8 *arg0, u8 *arg1) {
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
        key = string_hash(arg1);
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
            node = smallnode_alloc();
            if (node != NULL) {
                start = *(s32 *)res;
                *(u8 **)(res + 0x14) = node;
                h = asload_chunk_relocatable(*(s32 *)arg0, start, node, *(s32 *)(res + 4) - start);
                *(s32 *)(node + 4) = h;
                if (h == 0) {
                    smallnode_free(node);
                    return NULL;
                }
                return node;
            }
            return NULL;
        }
    }
    return NULL;
}

s32 dbase_mesh_anim_name_size(u8 *arg0, u8 *arg1) {
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
        key = string_hash(arg1);
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

u8 *dbase_mesh_anim_find(u8 *arg0, u8 *arg1) {
    u8 *p;
    s32 n;
    s32 key;

    if (arg0 == NULL) {
        return NULL;
    }
    key = string_hash(arg1);
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

void dbase_load_all_mesh_anims(s32 *arg0) {
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
                    node = smallnode_alloc();
                    if (node != NULL) {
                        start = *(s32 *)rec;
                        *(u8 **)(rec + 0x14) = node;
                        handle = asload_chunk_relocatable(*arg0, start, node, *(s32 *)(rec + 4) - start);
                        *(s32 *)(node + 4) = handle;
                        if (handle == 0) {
                            smallnode_free(node);
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
                        loader_cancel(*(void **)(e + 4));
                    }
                    smallnode_free(e);
                    slot[5] = NULL;
                }
            }
            off += 0x18;
            i++;
            goto again;
        }
    }
}

u8 *dbase_mesh_anim_release(u8 *arg0, s32 arg1) {
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
                    loader_cancel(item[1]);
                }
                smallnode_free((u8 *)item);
                *(s32 *)(e + 0x14) = 0;
                return e;
            }
        }
    }
    return NULL;
}

/* Unloads what dbase_mesh_anim_rec_load loaded: the record's node at +0x14 holds a heap
 * block or (if that is NULL) a handle at +4. */
u8 *dbase_mesh_anim_rec_release(s32 *arg0, u8 *rec) {
    u8 *node;

    node = *(u8 **)(rec + 0x14);
    if (node == NULL) {
        return NULL;
    }
    if (*(void **)node != NULL) {
        mal_free(*(void **)node, &MainHeap);
    } else {
        loader_cancel(*(void **)(node + 4));
    }
    smallnode_free(node);
    *(u8 **)(rec + 0x14) = NULL;
    return rec;
}

/* Inlined copy of dbase_mesh_anim_rec_load (load a record's node); being an inline call
 * is what puts arg1 in v1 and the table in a1. */
static inline u8 *func_80016B48_load(s32 *arg0, u8 *arg1) {
    u8 *cur;
    u8 *node;
    s32 start;
    s32 h;

    cur = *(u8 **)(arg1 + 0x14);
    if (cur == NULL) {
        node = smallnode_alloc();
        if (node != NULL) {
            start = *(s32 *)arg1;
            *(u8 **)(arg1 + 0x14) = node;
            h = asload_chunk_relocatable(*arg0, start, node, *(s32 *)(arg1 + 4) - start);
            *(s32 *)(node + 4) = h;
            if (h != 0) {
                goto ok;
            }
            smallnode_free(node);
        }
        return NULL;
    ok:
        return node;
    }
    return cur;
}

u8 *dbase_mesh_anim_load(u8 *arg0, s32 arg1) {
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

u8 *dbase_mesh_anim_rec_load(s32 *arg0, u8 *arg1) {
    s32 temp_a1;
    s32 temp_v0_2;
    u8 *temp_v0;
    u8 *var_v0;

    var_v0 = (*(s32*)((u8*)(arg1) + 20));
    if (var_v0 == NULL) {
        temp_v0 = smallnode_alloc();
        if (temp_v0 != NULL) {
            temp_a1 = (*(s32*)((u8*)(arg1) + 0));
            (*(s32*)((u8*)(arg1) + 20)) = temp_v0;
            temp_v0_2 = asload_chunk_relocatable(*arg0, temp_a1, temp_v0, (*(s32*)((u8*)(arg1) + 4)) - temp_a1);
            (*(s32*)((u8*)(temp_v0) + 4)) = temp_v0_2;
            if (temp_v0_2 != 0) {
                return temp_v0;
            }
            smallnode_free(temp_v0);
            goto block_5;
        }
block_5:
        var_v0 = NULL;
        /* Duplicate return node #6. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}

void dbase_load_textures_wait(u8 *rec) {
    if (*(s32 *)(rec + 8) == 0 && *(s32 *)(rec + 0xC) == 0) {
        *(s32 *)(rec + 8) = load_file_cb(*(s32 *)(rec + 4), dbase_textures_loaded, rec);
    }
    for (;;) {
        if (loader_pending() == 0) {
            goto done;
        }
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
done:
    DrawSync(0);
    while (mal_update_deferred(&MainHeap) != NULL) {
    }
}

void dbase_load_textures(obj_dbase *db) {
    if (db->load == 0 && db->data == NULL) {
        db->load = load_file_cb(db->textureFile, dbase_textures_loaded, db);
    }
}

void dbase_textures_loaded(u8 *data, u8 *rec) {
    _mal_area *heap;
    s32 size;

    size = textureset_mem_size(data);
    heap = &MainHeap;
    if (mal_first_free((size + 3) >> 2, heap->free) == NULL) {
        mal_free(data, heap);
        *(s32 *)(rec + 8) = 0;
        dbase_load_textures(rec);
    } else {
        textureset_init(rec + 0x10, data);
        textureset_upload(rec + 0x10);
        texture_release_loading_mem((obj_textureset *)(rec + 0x10));
    }
    *(s32 *)(rec + 8) = loader_queue(*(s32 *)(rec + 0), 0, *(s32 *)(rec + 0x24), dbase_set_data, rec, 1, 0);
}

void dbase_group_textures_loaded(u8 *arg0, u8 *arg1) {
    _mal_area *heap;
    s32 n;

    n = textureset_mem_size(arg0);
    heap = &MainHeap;
    if (mal_first_free((n + 3) >> 2, heap->free) == NULL) {
        mal_free(arg0, heap);
        *(s32 *)(arg1 + 8) = 0;
        dbase_load_textures(arg1);
    } else {
        arg1 += 0x10;
        textureset_init(arg1, arg0);
        textureset_upload(arg1);
        texture_release_loading_mem((obj_textureset *)arg1);
    }
}


void dbase_set_textures(u8 *arg0, u8 *arg1) {
    u8 *temp_s0;

    temp_s0 = arg0 + 0x10;
    textureset_init(temp_s0, arg1);
    textureset_upload(temp_s0);
    (*(s32*)((u8*)(arg0) + 32)) = 0;
}

void dbase_set_data(u8 *arg0, u8 *arg1) {
    mal_set_owner(arg0, arg1 + 0xC);
}

void dbase_release(u8 *arg0) {
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
        textureset_release(arg0 + 0x10);
        *(s32 *)(arg0 + 8) = 0;
        return;
    }
    if (*(s32 *)(arg0 + 8) != 0) {
        if (*(s32 *)(arg0 + 0x18) != 0) {
            textureset_release(arg0 + 0x10);
        }
        loader_cancel(*(void **)(arg0 + 8));
        *(s32 *)(arg0 + 8) = 0;
    }
}

void slotlist_mark(SlotList *arg0) {
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

void load_font(void) {
    dbase_load_textures_wait(&HeroModelBank[DB_Font]);
}

void dbase_set(u8 *arg0, s32 arg1, s32 arg2) {
    s32 temp_s0;

    temp_s0 = arg0 + 0x10;
    (*(s32*)((u8*)(arg0) + 12)) = arg1;
    (*(s32*)((u8*)(arg0) + 32)) = arg2;
    textureset_init(temp_s0, arg2);
    textureset_upload(temp_s0);
}

void dbase_init_all(void) {
    s32 i;
    s32 end;
    u8 *base;
    u8 *p;

    i = 0x11B;
    end = -1;
    base = (u8 *)HeroModelBank;
    p = base + 0x10;
loop:
    i--;
    if (i != end) {
        *(s32 *)(p - 8) = 0;
        *(s32 *)(p - 4) = 0;
        *(s32 *)(p + 0x24) = 0;
        textureset_clear((obj_textureset *)p);
        p += 0x38;
        goto loop;
    }
}

void dbase_init(obj_dbase *db) {
    db->load = 0;
    db->data = NULL;
    db->keep = NULL;
    textureset_clear(&db->textures);
}

static inline void func_80017164_clear(u8 *p) {
    *(s32 *)(p + 4) = 0;
    *(s32 *)(p + 8) = 0;
    *(s32 *)(p + 0xC) = 0;
    *(s32 *)(p + 0x10) = 0;
}

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */

obj_dbase HeroModelBank[DB_COUNT] = {
    { 0 }, /* DB_Empty */
    { (s32)&DatIndex[2 * 990], (s32)&DatIndex[2 * 989] }, /* DB_CharsCrateCrash */
    { (s32)&DatIndex[2 * 988], (s32)&DatIndex[2 * 987] }, /* DB_CharsCrateCoco */
    { (s32)&DatIndex[2 * 986], (s32)&DatIndex[2 * 985] }, /* DB_CharsCrateCortex */
    { (s32)&DatIndex[2 * 984], (s32)&DatIndex[2 * 983] }, /* DB_CharsCrateBrio */
    { (s32)&DatIndex[2 * 982], (s32)&DatIndex[2 * 981] }, /* DB_CharsCrateTiny */
    { (s32)&DatIndex[2 * 976], (s32)&DatIndex[2 * 975] }, /* DB_CharsCrateKong */
    { (s32)&DatIndex[2 * 980], (s32)&DatIndex[2 * 979] }, /* DB_CharsCrateDingo */
    { (s32)&DatIndex[2 * 978], (s32)&DatIndex[2 * 977] }, /* DB_CharsCrateRilla */
    { (s32)&DatIndex[2 * 974], (s32)&DatIndex[2 * 973] }, /* DB_CharsPogoCrash */
    { (s32)&DatIndex[2 * 972], (s32)&DatIndex[2 * 971] }, /* DB_CharsPogoCoco */
    { (s32)&DatIndex[2 * 970], (s32)&DatIndex[2 * 969] }, /* DB_CharsPogoCortex */
    { (s32)&DatIndex[2 * 968], (s32)&DatIndex[2 * 967] }, /* DB_CharsPogoBrio */
    { (s32)&DatIndex[2 * 966], (s32)&DatIndex[2 * 965] }, /* DB_CharsPogoTiny */
    { (s32)&DatIndex[2 * 962], (s32)&DatIndex[2 * 961] }, /* DB_CharsPogoDingo */
    { (s32)&DatIndex[2 * 964], (s32)&DatIndex[2 * 963] }, /* DB_CharsPogoKong */
    { (s32)&DatIndex[2 * 960], (s32)&DatIndex[2 * 959] }, /* DB_CharsPogoRilla */
    { (s32)&DatIndex[2 * 958], (s32)&DatIndex[2 * 957] }, /* DB_CharsPolar2Crash */
    { (s32)&DatIndex[2 * 956], (s32)&DatIndex[2 * 955] }, /* DB_CharsPolar2Coco */
    { (s32)&DatIndex[2 * 954], (s32)&DatIndex[2 * 953] }, /* DB_CharsPolar2Cortex */
    { (s32)&DatIndex[2 * 952], (s32)&DatIndex[2 * 951] }, /* DB_CharsPolar2Tiny */
    { (s32)&DatIndex[2 * 950], (s32)&DatIndex[2 * 949] }, /* DB_CharsPolar2Dingo */
    { (s32)&DatIndex[2 * 948], (s32)&DatIndex[2 * 947] }, /* DB_CharsPolar2Brio */
    { (s32)&DatIndex[2 * 736], (s32)&DatIndex[2 * 735] }, /* DB_Powerups */
    { (s32)&DatIndex[2 * 796], (s32)&DatIndex[2 * 795] }, /* DB_CrateObjects */
    { (s32)&DatIndex[2 * 794], (s32)&DatIndex[2 * 793] }, /* DB_RingObjects */
    { (s32)&DatIndex[2 * 792], (s32)&DatIndex[2 * 791] }, /* DB_Angel */
    { (s32)&DatIndex[2 * 798], (s32)&DatIndex[2 * 797] }, /* DB_Penguin */
    { (s32)&DatIndex[2 * 732], (s32)&DatIndex[2 * 731] }, /* DB_Interface */
    { (s32)&DatIndex[2 * 732], (s32)&DatIndex[2 * 731] }, /* DB_InterfaceInterface22 */
    { (s32)&DatIndex[2 * 732], (s32)&DatIndex[2 * 731] }, /* DB_InterfaceInterface22_2 */
    { (s32)&DatIndex[2 * 732], (s32)&DatIndex[2 * 731] }, /* DB_InterfaceInterface22_3 */
    { (s32)&DatIndex[2 * 732], (s32)&DatIndex[2 * 731] }, /* DB_InterfaceInterface22_4 */
    { (s32)&DatIndex[2 * 732], (s32)&DatIndex[2 * 731] }, /* DB_InterfaceInterface22_5 */
    { (s32)&DatIndex[2 * 711], (s32)&DatIndex[2 * 710] }, /* DB_ArenaCrateSpaceArena */
    { (s32)&DatIndex[2 * 709], (s32)&DatIndex[2 * 708] }, /* DB_ArenaCrateSpaceCrystalarena */
    { (s32)&DatIndex[2 * 715], (s32)&DatIndex[2 * 714] }, /* DB_ArenaCrateJungleArena */
    { (s32)&DatIndex[2 * 713], (s32)&DatIndex[2 * 712] }, /* DB_ArenaCrateJungleCrystalarena */
    { (s32)&DatIndex[2 * 707], (s32)&DatIndex[2 * 706] }, /* DB_ArenaCrateSnowArena */
    { (s32)&DatIndex[2 * 705], (s32)&DatIndex[2 * 704] }, /* DB_ArenaCrateSnowCrystalarena */
    { (s32)&DatIndex[2 * 703], (s32)&DatIndex[2 * 702] }, /* DB_ArenaCrateDrainArena */
    { (s32)&DatIndex[2 * 701], (s32)&DatIndex[2 * 700] }, /* DB_ArenaCrateDrainCrystalarena */
    { (s32)&DatIndex[2 * 699], (s32)&DatIndex[2 * 698] }, /* DB_ArenaCrateTiebreakerArena */
    { (s32)&DatIndex[2 * 697], (s32)&DatIndex[2 * 696] }, /* DB_ArenaBossPapuArena */
    { (s32)&DatIndex[2 * 790], (s32)&DatIndex[2 * 789] }, /* DB_Papu */
    { (s32)&DatIndex[2 * 788], (s32)&DatIndex[2 * 787] }, /* DB_Minion */
    { (s32)&DatIndex[2 * 695], (s32)&DatIndex[2 * 694] }, /* DB_ArenaTankSwampArena */
    { (s32)&DatIndex[2 * 693], (s32)&DatIndex[2 * 692] }, /* DB_ArenaTankDesertArena */
    { (s32)&DatIndex[2 * 691], (s32)&DatIndex[2 * 690] }, /* DB_ArenaTankJungleArena */
    { (s32)&DatIndex[2 * 689], (s32)&DatIndex[2 * 688] }, /* DB_ArenaTankMetalArena */
    { (s32)&DatIndex[2 * 687], (s32)&DatIndex[2 * 686] }, /* DB_ArenaBossKomodoArena */
    { (s32)&DatIndex[2 * 685], (s32)&DatIndex[2 * 684] }, /* DB_ArenaTankSwampCrystalarena */
    { (s32)&DatIndex[2 * 683], (s32)&DatIndex[2 * 682] }, /* DB_ArenaTankDesertCrystalarena */
    { (s32)&DatIndex[2 * 681], (s32)&DatIndex[2 * 680] }, /* DB_ArenaTankJungleCrystalarena */
    { (s32)&DatIndex[2 * 679], (s32)&DatIndex[2 * 678] }, /* DB_ArenaTankMetalCrystalarena */
    { (s32)&DatIndex[2 * 786], (s32)&DatIndex[2 * 785] }, /* DB_TankObjects */
    { (s32)&DatIndex[2 * 784], (s32)&DatIndex[2 * 783] }, /* DB_TankObjects2 */
    { (s32)&DatIndex[2 * 782], (s32)&DatIndex[2 * 781] }, /* DB_ArenaBossKomodoArenaobjects */
    { (s32)&DatIndex[2 * 780], (s32)&DatIndex[2 * 779] }, /* DB_ArenaBossKomodoKomodo */
    { (s32)&DatIndex[2 * 778], (s32)&DatIndex[2 * 777] }, /* DB_ArenaBossKomodoKomodotank */
    { (s32)&DatIndex[2 * 774], (s32)&DatIndex[2 * 773] }, /* DB_TankVehicle */
    { (s32)&DatIndex[2 * 776], (s32)&DatIndex[2 * 775] }, /* DB_ArenaTankJungleObjects */
    { (s32)&DatIndex[2 * 894], (s32)&DatIndex[2 * 893] }, /* DB_CharsTankCrash */
    { (s32)&DatIndex[2 * 892], (s32)&DatIndex[2 * 891] }, /* DB_CharsTankCoco */
    { (s32)&DatIndex[2 * 890], (s32)&DatIndex[2 * 889] }, /* DB_CharsTankCortex */
    { (s32)&DatIndex[2 * 888], (s32)&DatIndex[2 * 887] }, /* DB_CharsTankTiny */
    { (s32)&DatIndex[2 * 886], (s32)&DatIndex[2 * 885] }, /* DB_CharsTankBrio */
    { (s32)&DatIndex[2 * 884], (s32)&DatIndex[2 * 883] }, /* DB_CharsTankDingo */
    { (s32)&DatIndex[2 * 882], (s32)&DatIndex[2 * 881] }, /* DB_CharsTankKong */
    { (s32)&DatIndex[2 * 880], (s32)&DatIndex[2 * 879] }, /* DB_CharsTankRilla */
    { (s32)&DatIndex[2 * 772], (s32)&DatIndex[2 * 771] }, /* DB_ArenaMedievalChickenChicken */
    { (s32)&DatIndex[2 * 675], (s32)&DatIndex[2 * 674] }, /* DB_ArenaPogoPainterArena */
    { (s32)&DatIndex[2 * 671], (s32)&DatIndex[2 * 670] }, /* DB_ArenaPogoGogoArena */
    { (s32)&DatIndex[2 * 667], (s32)&DatIndex[2 * 666] }, /* DB_ArenaPogoEplArena */
    { (s32)&DatIndex[2 * 663], (s32)&DatIndex[2 * 662] }, /* DB_ArenaPogoPadlockArena */
    { (s32)&DatIndex[2 * 673], (s32)&DatIndex[2 * 672] }, /* DB_ArenaPogoPainterCrystalarena */
    { (s32)&DatIndex[2 * 669], (s32)&DatIndex[2 * 668] }, /* DB_ArenaPogoGogoCrystalarena */
    { (s32)&DatIndex[2 * 665], (s32)&DatIndex[2 * 664] }, /* DB_ArenaPogoEplCrystalarena */
    { (s32)&DatIndex[2 * 661], (s32)&DatIndex[2 * 660] }, /* DB_ArenaPogoPadlockCrystalarena */
    { (s32)&DatIndex[2 * 770], (s32)&DatIndex[2 * 769] }, /* DB_PogoObjects */
    { (s32)&DatIndex[2 * 768], (s32)&DatIndex[2 * 767] }, /* DB_Roo */
    { (s32)&DatIndex[2 * 617], (s32)&DatIndex[2 * 616] }, /* DB_ArenaMedievalMalletArena */
    { (s32)&DatIndex[2 * 615], (s32)&DatIndex[2 * 614] }, /* DB_ArenaMedievalMalletCrystalarena */
    { (s32)&DatIndex[2 * 343], (s32)&DatIndex[2 * 342] }, /* DB_MalletObjects */
    { (s32)&DatIndex[2 * 862], (s32)&DatIndex[2 * 861] }, /* DB_CharsMalletCrash */
    { (s32)&DatIndex[2 * 860], (s32)&DatIndex[2 * 859] }, /* DB_CharsMalletCoco */
    { (s32)&DatIndex[2 * 858], (s32)&DatIndex[2 * 857] }, /* DB_CharsMalletCortex */
    { (s32)&DatIndex[2 * 856], (s32)&DatIndex[2 * 855] }, /* DB_CharsMalletBrio */
    { (s32)&DatIndex[2 * 854], (s32)&DatIndex[2 * 853] }, /* DB_CharsMalletDingo */
    { (s32)&DatIndex[2 * 852], (s32)&DatIndex[2 * 851] }, /* DB_CharsMalletRilla */
    { (s32)&DatIndex[2 * 850], (s32)&DatIndex[2 * 849] }, /* DB_CharsMalletTiny */
    { (s32)&DatIndex[2 * 848], (s32)&DatIndex[2 * 847] }, /* DB_CharsMalletKong */
    { (s32)&DatIndex[2 * 613], (s32)&DatIndex[2 * 612] }, /* DB_ArenaBossOxideArena */
    { (s32)&DatIndex[2 * 742], (s32)&DatIndex[2 * 741] }, /* DB_Oxide */
    { (s32)&DatIndex[2 * 740], (s32)&DatIndex[2 * 739] }, /* DB_PlayerVehicle */
    { (s32)&DatIndex[2 * 609], (s32)&DatIndex[2 * 608] }, /* DB_ArenaMedievalKegArena */
    { (s32)&DatIndex[2 * 607], (s32)&DatIndex[2 * 606] }, /* DB_ArenaMedievalKegCrystalarena */
    { (s32)&DatIndex[2 * 738], (s32)&DatIndex[2 * 737] }, /* DB_KegObjects */
    { (s32)&DatIndex[2 * 878], (s32)&DatIndex[2 * 877] }, /* DB_CharsKegsCrash */
    { (s32)&DatIndex[2 * 876], (s32)&DatIndex[2 * 875] }, /* DB_CharsKegsCoco */
    { (s32)&DatIndex[2 * 874], (s32)&DatIndex[2 * 873] }, /* DB_CharsKegsCortex */
    { (s32)&DatIndex[2 * 872], (s32)&DatIndex[2 * 871] }, /* DB_CharsKegsTiny */
    { (s32)&DatIndex[2 * 870], (s32)&DatIndex[2 * 869] }, /* DB_CharsKegsBrio */
    { (s32)&DatIndex[2 * 868], (s32)&DatIndex[2 * 867] }, /* DB_CharsKegsDingo */
    { (s32)&DatIndex[2 * 866], (s32)&DatIndex[2 * 865] }, /* DB_CharsKegsKong */
    { (s32)&DatIndex[2 * 864], (s32)&DatIndex[2 * 863] }, /* DB_CharsKegsRilla */
    { (s32)&DatIndex[2 * 659], (s32)&DatIndex[2 * 658] }, /* DB_ArenaBallsCrashArena */
    { (s32)&DatIndex[2 * 657], (s32)&DatIndex[2 * 656] }, /* DB_ArenaBallsCrashCrystalarena */
    { (s32)&DatIndex[2 * 655], (s32)&DatIndex[2 * 654] }, /* DB_ArenaBallsBeachArena */
    { (s32)&DatIndex[2 * 653], (s32)&DatIndex[2 * 652] }, /* DB_ArenaBallsBeachCrystalarena */
    { (s32)&DatIndex[2 * 651], (s32)&DatIndex[2 * 650] }, /* DB_ArenaBallsNginArena */
    { (s32)&DatIndex[2 * 649], (s32)&DatIndex[2 * 648] }, /* DB_ArenaBallsNginCrystalarena */
    { (s32)&DatIndex[2 * 647], (s32)&DatIndex[2 * 646] }, /* DB_ArenaBallsSkyArena */
    { (s32)&DatIndex[2 * 645], (s32)&DatIndex[2 * 644] }, /* DB_ArenaBallsSkyCrystalarena */
    { (s32)&DatIndex[2 * 910], (s32)&DatIndex[2 * 909] }, /* DB_CharsBallCrash */
    { (s32)&DatIndex[2 * 908], (s32)&DatIndex[2 * 907] }, /* DB_CharsBallCoco */
    { (s32)&DatIndex[2 * 906], (s32)&DatIndex[2 * 905] }, /* DB_CharsBallCortex */
    { (s32)&DatIndex[2 * 904], (s32)&DatIndex[2 * 903] }, /* DB_CharsBallBrio */
    { (s32)&DatIndex[2 * 902], (s32)&DatIndex[2 * 901] }, /* DB_CharsBallDingo */
    { (s32)&DatIndex[2 * 900], (s32)&DatIndex[2 * 899] }, /* DB_CharsBallRilla */
    { (s32)&DatIndex[2 * 898], (s32)&DatIndex[2 * 897] }, /* DB_CharsBallTiny */
    { (s32)&DatIndex[2 * 896], (s32)&DatIndex[2 * 895] }, /* DB_CharsBallKong */
    { (s32)&DatIndex[2 * 762], (s32)&DatIndex[2 * 761] }, /* DB_BallsEffects */
    { (s32)&DatIndex[2 * 766], (s32)&DatIndex[2 * 765] }, /* DB_BallsVehicle */
    { (s32)&DatIndex[2 * 764], (s32)&DatIndex[2 * 763] }, /* DB_Ngin */
    { (s32)&DatIndex[2 * 627], (s32)&DatIndex[2 * 626] }, /* DB_ArenaMedievalRingArena */
    { (s32)&DatIndex[2 * 625], (s32)&DatIndex[2 * 624] }, /* DB_ArenaMedievalRingCrystalarena */
    { (s32)&DatIndex[2 * 486], (s32)&DatIndex[2 * 485] }, /* DB_Unkscenery */
    { (s32)&DatIndex[2 * 484], (s32)&DatIndex[2 * 483] }, /* DB_Polar */
    { (s32)&DatIndex[2 * 482], (s32)&DatIndex[2 * 481] }, /* DB_Masks */
    { (s32)&DatIndex[2 * 480], (s32)&DatIndex[2 * 479] }, /* DB_PolarObjects */
    { (s32)&DatIndex[2 * 478], (s32)&DatIndex[2 * 477] }, /* DB_ArenaPolarPolarArena */
    { (s32)&DatIndex[2 * 474], (s32)&DatIndex[2 * 473] }, /* DB_ArenaPolarMeltArena */
    { (s32)&DatIndex[2 * 468], (s32)&DatIndex[2 * 467] }, /* DB_ArenaPolarTiltSeal */
    { (s32)&DatIndex[2 * 466], (s32)&DatIndex[2 * 465] }, /* DB_ArenaPolarManicArena */
    { (s32)&DatIndex[2 * 942], (s32)&DatIndex[2 * 941] }, /* DB_CharsCrate2Crash */
    { (s32)&DatIndex[2 * 940], (s32)&DatIndex[2 * 939] }, /* DB_CharsCrate2Coco */
    { (s32)&DatIndex[2 * 938], (s32)&DatIndex[2 * 937] }, /* DB_CharsCrate2Cortex */
    { (s32)&DatIndex[2 * 936], (s32)&DatIndex[2 * 935] }, /* DB_CharsCrate2Tiny */
    { (s32)&DatIndex[2 * 934], (s32)&DatIndex[2 * 933] }, /* DB_CharsCrate2Dingo */
    { (s32)&DatIndex[2 * 932], (s32)&DatIndex[2 * 931] }, /* DB_CharsCrate2Brio */
    { (s32)&DatIndex[2 * 472], (s32)&DatIndex[2 * 471] }, /* DB_ArenaBossBearArena */
    { (s32)&DatIndex[2 * 464], (s32)&DatIndex[2 * 463] }, /* DB_ArenaBossBearBigbear */
    { (s32)&DatIndex[2 * 470], (s32)&DatIndex[2 * 469] }, /* DB_Modelmix */
    { (s32)&DatIndex[2 * 946], (s32)&DatIndex[2 * 945] }, /* DB_CharsPolar2Kong */
    { (s32)&DatIndex[2 * 462], (s32)&DatIndex[2 * 461] }, /* DB_RoboPolar */
    { (s32)&DatIndex[2 * 944], (s32)&DatIndex[2 * 943] }, /* DB_CharsPolar2Rilla */
    { (s32)&DatIndex[2 * 476], (s32)&DatIndex[2 * 475] }, /* DB_ArenaPolarTiltCrystalarena */
    { (s32)&DatIndex[2 * 928], (s32)&DatIndex[2 * 927] }, /* DB_CharsCrate2Rilla */
    { (s32)&DatIndex[2 * 930], (s32)&DatIndex[2 * 929] }, /* DB_CharsCrate2Kong */
    { (s32)&DatIndex[2 * 460], (s32)&DatIndex[2 * 459] }, /* DB_ArenaPolarManicCrystalarena */
    { (s32)&DatIndex[2 * 458], (s32)&DatIndex[2 * 457] }, /* DB_ArenaPolarMeltCrystalarena */
    { (s32)&DatIndex[2 * 456], (s32)&DatIndex[2 * 455] }, /* DB_ArenaPolarPolarCrystalarena */
    { (s32)&DatIndex[2 * 926], (s32)&DatIndex[2 * 925] }, /* DB_CharsPolarCrash */
    { (s32)&DatIndex[2 * 924], (s32)&DatIndex[2 * 923] }, /* DB_CharsPolarCoco */
    { (s32)&DatIndex[2 * 922], (s32)&DatIndex[2 * 921] }, /* DB_CharsPolarCortex */
    { (s32)&DatIndex[2 * 920], (s32)&DatIndex[2 * 919] }, /* DB_CharsPolarTiny */
    { (s32)&DatIndex[2 * 918], (s32)&DatIndex[2 * 917] }, /* DB_CharsPolarDingo */
    { (s32)&DatIndex[2 * 916], (s32)&DatIndex[2 * 915] }, /* DB_CharsPolarBrio */
    { (s32)&DatIndex[2 * 914], (s32)&DatIndex[2 * 913] }, /* DB_CharsPolarRilla */
    { (s32)&DatIndex[2 * 912], (s32)&DatIndex[2 * 911] }, /* DB_CharsPolarKong */
    { (s32)&DatIndex[2 * 643], (s32)&DatIndex[2 * 642] }, /* DB_ArenaDashDotArena */
    { (s32)&DatIndex[2 * 830], (s32)&DatIndex[2 * 829] }, /* DB_CharsRaceCrash */
    { (s32)&DatIndex[2 * 828], (s32)&DatIndex[2 * 827] }, /* DB_CharsRaceCortex */
    { (s32)&DatIndex[2 * 826], (s32)&DatIndex[2 * 825] }, /* DB_CharsRaceTiny */
    { (s32)&DatIndex[2 * 824], (s32)&DatIndex[2 * 823] }, /* DB_CharsRaceDingo */
    { (s32)&DatIndex[2 * 822], (s32)&DatIndex[2 * 821] }, /* DB_CharsRaceBrio */
    { (s32)&DatIndex[2 * 639], (s32)&DatIndex[2 * 638] }, /* DB_ArenaDashToxicArena */
    { (s32)&DatIndex[2 * 641], (s32)&DatIndex[2 * 640] }, /* DB_ArenaDashSplashArena */
    { (s32)&DatIndex[2 * 637], (s32)&DatIndex[2 * 636] }, /* DB_ArenaDashDanteArena */
    { (s32)&DatIndex[2 * 760], (s32)&DatIndex[2 * 759] }, /* DB_CrateEffects */
    { (s32)&DatIndex[2 * 818], (s32)&DatIndex[2 * 817] }, /* DB_CharsRaceCoco */
    { (s32)&DatIndex[2 * 816], (s32)&DatIndex[2 * 815] }, /* DB_CharsRaceRilla */
    { (s32)&DatIndex[2 * 820], (s32)&DatIndex[2 * 819] }, /* DB_CharsRaceKong */
    { (s32)&DatIndex[2 * 814], (s32)&DatIndex[2 * 813] }, /* DB_CharsSplashCrash */
    { (s32)&DatIndex[2 * 812], (s32)&DatIndex[2 * 811] }, /* DB_CharsSplashCortex */
    { (s32)&DatIndex[2 * 810], (s32)&DatIndex[2 * 809] }, /* DB_CharsSplashTiny */
    { (s32)&DatIndex[2 * 808], (s32)&DatIndex[2 * 807] }, /* DB_CharsSplashDingo */
    { (s32)&DatIndex[2 * 806], (s32)&DatIndex[2 * 805] }, /* DB_CharsSplashKong */
    { (s32)&DatIndex[2 * 800], (s32)&DatIndex[2 * 799] }, /* DB_CharsSplashBrio */
    { (s32)&DatIndex[2 * 804], (s32)&DatIndex[2 * 803] }, /* DB_CharsSplashCoco */
    { (s32)&DatIndex[2 * 802], (s32)&DatIndex[2 * 801] }, /* DB_CharsSplashRilla */
    { (s32)&DatIndex[2 * 635], (s32)&DatIndex[2 * 634] }, /* DB_ArenaDashToxicCrystalarena */
    { (s32)&DatIndex[2 * 633], (s32)&DatIndex[2 * 632] }, /* DB_ArenaDashDanteCrystalarena */
    { (s32)&DatIndex[2 * 631], (s32)&DatIndex[2 * 630] }, /* DB_ArenaDashDotCrystalarena */
    { (s32)&DatIndex[2 * 629], (s32)&DatIndex[2 * 628] }, /* DB_ArenaDashSplashCrystalarena */
    { (s32)&DatIndex[2 * 623], (s32)&DatIndex[2 * 622] }, /* DB_ArenaMedievalDragonCrystalarena */
    { (s32)&DatIndex[2 * 846], (s32)&DatIndex[2 * 845] }, /* DB_CharsDinoCrash */
    { (s32)&DatIndex[2 * 844], (s32)&DatIndex[2 * 843] }, /* DB_CharsDinoCortex */
    { (s32)&DatIndex[2 * 842], (s32)&DatIndex[2 * 841] }, /* DB_CharsDinoTiny */
    { (s32)&DatIndex[2 * 840], (s32)&DatIndex[2 * 839] }, /* DB_CharsDinoDingo */
    { (s32)&DatIndex[2 * 838], (s32)&DatIndex[2 * 837] }, /* DB_CharsDinoCoco */
    { (s32)&DatIndex[2 * 834], (s32)&DatIndex[2 * 833] }, /* DB_CharsDinoRilla */
    { (s32)&DatIndex[2 * 836], (s32)&DatIndex[2 * 835] }, /* DB_CharsDinoBrio */
    { (s32)&DatIndex[2 * 832], (s32)&DatIndex[2 * 831] }, /* DB_CharsDinoKong */
    { (s32)&DatIndex[2 * 621], (s32)&DatIndex[2 * 620] }, /* DB_ArenaMedievalDragonArena */
    { (s32)&DatIndex[2 * 619], (s32)&DatIndex[2 * 618] }, /* DB_DragonObjects */
    { (s32)&DatIndex[2 * 135], (s32)&DatIndex[2 * 134] }, /* DB_ArenaTestObjects */
    { (s32)&DatIndex[2 * 730], (s32)&DatIndex[2 * 729] }, /* DB_Loading */
    { (s32)&DatIndex[2 * 131], (s32)&DatIndex[2 * 130] }, /* DB_MainmenuModels */
    { (s32)&DatIndex[2 * 129], (s32)&DatIndex[2 * 128] }, /* DB_MainmenuArenaModels2 */
    { (s32)&DatIndex[2 * 127], (s32)&DatIndex[2 * 126] }, /* DB_MainmenuArenaModels1 */
    { (s32)&DatIndex[2 * 734], (s32)&DatIndex[2 * 733] }, /* DB_Font */
    { (s32)&DatIndex[2 * 125], (s32)&DatIndex[2 * 124] }, /* DB_CutsceneIntroEurocom */
    { (s32)&DatIndex[2 * 123], (s32)&DatIndex[2 * 122] }, /* DB_CutsceneIntroLogo2 */
    { (s32)&DatIndex[2 * 379], (s32)&DatIndex[2 * 378] }, /* DB_MainmenuModels2 */
    { (s32)&DatIndex[2 * 393], (s32)&DatIndex[2 * 392] }, /* DB_WarpRoom1Level */
    { (s32)&DatIndex[2 * 391], (s32)&DatIndex[2 * 390] }, /* DB_WarpRoom2Level */
    { (s32)&DatIndex[2 * 389], (s32)&DatIndex[2 * 388] }, /* DB_WarpRoom3Level */
    { (s32)&DatIndex[2 * 387], (s32)&DatIndex[2 * 386] }, /* DB_WarpRoom4Level */
    { (s32)&DatIndex[2 * 385], (s32)&DatIndex[2 * 384] }, /* DB_WarpRoom5Level */
    { (s32)&DatIndex[2 * 383], (s32)&DatIndex[2 * 382] }, /* DB_DemoHub2Level */
    { (s32)&DatIndex[2 * 381], (s32)&DatIndex[2 * 380] }, /* DB_DemoHub1Level */
    { (s32)&DatIndex[2 * 377], (s32)&DatIndex[2 * 376] }, /* DB_AdventureItems */
    { (s32)&DatIndex[2 * 758], (s32)&DatIndex[2 * 757] }, /* DB_CharsWarpCrash */
    { (s32)&DatIndex[2 * 756], (s32)&DatIndex[2 * 755] }, /* DB_CharsWarpCoco */
    { (s32)&DatIndex[2 * 754], (s32)&DatIndex[2 * 753] }, /* DB_CharsWarpCortex */
    { (s32)&DatIndex[2 * 752], (s32)&DatIndex[2 * 751] }, /* DB_CharsWarpBrio */
    { (s32)&DatIndex[2 * 750], (s32)&DatIndex[2 * 749] }, /* DB_CharsWarpTiny */
    { (s32)&DatIndex[2 * 748], (s32)&DatIndex[2 * 747] }, /* DB_CharsWarpKong */
    { (s32)&DatIndex[2 * 746], (s32)&DatIndex[2 * 745] }, /* DB_CharsWarpDingo */
    { (s32)&DatIndex[2 * 744], (s32)&DatIndex[2 * 743] }, /* DB_CharsWarpRilla */
    { (s32)&DatIndex[2 * 121], (s32)&DatIndex[2 * 120] }, /* DB_CutsceneIntroLogo */
    { (s32)&DatIndex[2 * 119], (s32)&DatIndex[2 * 118] }, /* DB_CutsceneLevelShot19 */
    { (s32)&DatIndex[2 * 117], (s32)&DatIndex[2 * 116] }, /* DB_CutsceneLevelShot18 */
    { (s32)&DatIndex[2 * 115], (s32)&DatIndex[2 * 114] }, /* DB_CutsceneLevelShot17 */
    { (s32)&DatIndex[2 * 113], (s32)&DatIndex[2 * 112] }, /* DB_CutsceneLevelShot16 */
    { (s32)&DatIndex[2 * 111], (s32)&DatIndex[2 * 110] }, /* DB_CutsceneLevelShot15 */
    { (s32)&DatIndex[2 * 109], (s32)&DatIndex[2 * 108] }, /* DB_CutsceneLevelShot14 */
    { (s32)&DatIndex[2 * 107], (s32)&DatIndex[2 * 106] }, /* DB_CutsceneLevelIntroCrashplain */
    { (s32)&DatIndex[2 * 105], (s32)&DatIndex[2 * 104] }, /* DB_CutsceneLevelIntroCortexlab */
    { (s32)&DatIndex[2 * 103], (s32)&DatIndex[2 * 102] }, /* DB_CutsceneLevelIntroShotGroupTeamgood2 */
    { (s32)&DatIndex[2 * 101], (s32)&DatIndex[2 * 100] }, /* DB_CutsceneLevelIntroShotWelcome3 */
    { (s32)&DatIndex[2 * 99], (s32)&DatIndex[2 * 98] }, /* DB_CutsceneLevelIntroShotWelcome2 */
    { (s32)&DatIndex[2 * 97], (s32)&DatIndex[2 * 96] }, /* DB_CutsceneLevelIntroShotWelcome */
    { (s32)&DatIndex[2 * 95], (s32)&DatIndex[2 * 94] }, /* DB_CutsceneLevelIntroShotGroupTeamevil4 */
    { (s32)&DatIndex[2 * 93], (s32)&DatIndex[2 * 92] }, /* DB_CutsceneLevelIntroShotGroupTeamevil3 */
    { (s32)&DatIndex[2 * 91], (s32)&DatIndex[2 * 90] }, /* DB_CutsceneLevelShot13 */
    { (s32)&DatIndex[2 * 89], (s32)&DatIndex[2 * 88] }, /* DB_CutsceneLevelIntroShotGroupTeamevil2 */
    { (s32)&DatIndex[2 * 87], (s32)&DatIndex[2 * 86] }, /* DB_CutsceneLevelShot12 */
    { (s32)&DatIndex[2 * 85], (s32)&DatIndex[2 * 84] }, /* DB_CutsceneLevelIntroShotGroupTeamevil */
    { (s32)&DatIndex[2 * 83], (s32)&DatIndex[2 * 82] }, /* DB_CutsceneLevelIntroShotGroupTeamgood */
    { (s32)&DatIndex[2 * 37], (s32)&DatIndex[2 * 36] }, /* DB_CutsceneLevel4 */
    { (s32)&DatIndex[2 * 35], (s32)&DatIndex[2 * 34] }, /* DB_CutsceneLevel3 */
    { (s32)&DatIndex[2 * 33], (s32)&DatIndex[2 * 32] }, /* DB_CutsceneLevel2 */
    { (s32)&DatIndex[2 * 31], (s32)&DatIndex[2 * 30] }, /* DB_CutsceneLevel */
    { (s32)&DatIndex[2 * 61], (s32)&DatIndex[2 * 60] }, /* DB_CutsceneLevelEndingGoodShot3 */
    { (s32)&DatIndex[2 * 59], (s32)&DatIndex[2 * 58] }, /* DB_CutsceneLevelShot8 */
    { (s32)&DatIndex[2 * 57], (s32)&DatIndex[2 * 56] }, /* DB_CutsceneLevelShot7 */
    { (s32)&DatIndex[2 * 55], (s32)&DatIndex[2 * 54] }, /* DB_CutsceneLevelShot6 */
    { (s32)&DatIndex[2 * 53], (s32)&DatIndex[2 * 52] }, /* DB_CutsceneLevelEndingGoodShot2 */
    { (s32)&DatIndex[2 * 51], (s32)&DatIndex[2 * 50] }, /* DB_CutsceneLevelShot5 */
    { (s32)&DatIndex[2 * 49], (s32)&DatIndex[2 * 48] }, /* DB_CutsceneLevelShot4 */
    { (s32)&DatIndex[2 * 47], (s32)&DatIndex[2 * 46] }, /* DB_CutsceneLevelShot3 */
    { (s32)&DatIndex[2 * 45], (s32)&DatIndex[2 * 44] }, /* DB_CutsceneLevelShot2 */
    { (s32)&DatIndex[2 * 43], (s32)&DatIndex[2 * 42] }, /* DB_CutsceneLevelEndingEvilShot1 */
    { (s32)&DatIndex[2 * 41], (s32)&DatIndex[2 * 40] }, /* DB_CutsceneLevelEndingGoodShot1 */
    { (s32)&DatIndex[2 * 39], (s32)&DatIndex[2 * 38] }, /* DB_CutsceneLevelIntro */
    { (s32)&DatIndex[2 * 81], (s32)&DatIndex[2 * 80] }, /* DB_CutsceneLevelEndingEvilShot8 */
    { (s32)&DatIndex[2 * 79], (s32)&DatIndex[2 * 78] }, /* DB_CutsceneLevelShot11 */
    { (s32)&DatIndex[2 * 77], (s32)&DatIndex[2 * 76] }, /* DB_CutsceneLevelShot10 */
    { (s32)&DatIndex[2 * 75], (s32)&DatIndex[2 * 74] }, /* DB_CutsceneLevelEndingEvilShot7 */
    { (s32)&DatIndex[2 * 73], (s32)&DatIndex[2 * 72] }, /* DB_CutsceneLevelEndingEvilShot6 */
    { (s32)&DatIndex[2 * 71], (s32)&DatIndex[2 * 70] }, /* DB_CutsceneLevelShot9 */
    { (s32)&DatIndex[2 * 69], (s32)&DatIndex[2 * 68] }, /* DB_CutsceneLevelEndingEvilShot5 */
    { (s32)&DatIndex[2 * 67], (s32)&DatIndex[2 * 66] }, /* DB_CutsceneLevelEndingEvilShot4 */
    { (s32)&DatIndex[2 * 65], (s32)&DatIndex[2 * 64] }, /* DB_CutsceneLevelEndingEvilShot3 */
    { (s32)&DatIndex[2 * 63], (s32)&DatIndex[2 * 62] }, /* DB_CutsceneLevelEndingEvilShot2 */
    { (s32)&DatIndex[2 * 27], (s32)&DatIndex[2 * 26] }, /* DB_CutsceneTextGoodwins */
    { (s32)&DatIndex[2 * 29], (s32)&DatIndex[2 * 28] }, /* DB_CutsceneTextEvilwins */
    { (s32)&DatIndex[2 * 25], (s32)&DatIndex[2 * 24] }, /* DB_CutsceneTextMixteamwins */
    { (s32)&DatIndex[2 * 23], (s32)&DatIndex[2 * 22] }, /* DB_CutsceneTextCredits */
    { (s32)&DatIndex[2 * 21], (s32)&DatIndex[2 * 20] }, /* DB_CutsceneTextIntromovie */
    { (s32)&DatIndex[2 * 7], (s32)&DatIndex[2 * 6] }, /* DB_CutsceneUkaData4 */
    { (s32)&DatIndex[2 * 5], (s32)&DatIndex[2 * 4] }, /* DB_CutsceneUkaData3 */
    { (s32)&DatIndex[2 * 3], (s32)&DatIndex[2 * 2] }, /* DB_CutsceneUkaData2 */
    { (s32)&DatIndex[2 * 1], (s32)&DatIndex[2 * 0] }, /* DB_CutsceneUkaData */
    { (s32)&DatIndex[2 * 15], (s32)&DatIndex[2 * 14] }, /* DB_CutsceneAkuData4 */
    { (s32)&DatIndex[2 * 13], (s32)&DatIndex[2 * 12] }, /* DB_CutsceneAkuData3 */
    { (s32)&DatIndex[2 * 11], (s32)&DatIndex[2 * 10] }, /* DB_CutsceneAkuData2 */
    { (s32)&DatIndex[2 * 9], (s32)&DatIndex[2 * 8] }, /* DB_CutsceneAkuData */
    { (s32)&DatIndex[2 * 19], (s32)&DatIndex[2 * 18] }, /* DB_CutsceneAkuUkaData2 */
    { (s32)&DatIndex[2 * 17], (s32)&DatIndex[2 * 16] }, /* DB_CutsceneAkuUkaData */
};
