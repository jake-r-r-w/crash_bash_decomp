#include "common.h"
#include "bash/main.h"
#include "bash/dbase.h"

void dbase_setup_blocks(ModelFile *arg0, s32 *arg1) {
    FileEntry *tbl;
    FileEntry *e;
    FileEntry *next;
    s32 *p;
    s32 found;
    BlockRef *blk;
    BlockRef *old;
    s32 i;

    i = arg0->nMeshAnims - 1;
    tbl = (FileEntry *)((u8 *)&arg0->meshAnims + arg0->meshAnims);
    next = (FileEntry *)(i * sizeof(FileEntry) + (u32)tbl);
    for (;;) {
        if (i < 0) {
            goto second;
        }
        e = next;
        blk = smallnode_alloc();
        e->block = blk;
        blk->ptr = mal_split_ptr((u8 *)arg0, e->start);
        i--;
        mal_set_owner(blk->ptr, &blk->ptr);
        mal_trim(blk->ptr, e->end - e->start, &MainHeap);
        next = e - 1;
    }
second:
    if (arg1 != NULL) {
        e = tbl;
        i = arg0->nMeshAnims;
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

TexClut *dbase_texture_entry(obj_dbase *db, s32 i) {
    obj_textureset *set;

    if (db != NULL) {
        set = &db->textures;
        if (set->tex != NULL) {
            return &set->ent[i];
        }
    }
    return NULL;
}

obj_texture *dbase_texture_ptr(obj_dbase *db, s32 i) {
    obj_texture *tex;

    if (db != NULL) {
        tex = db->textures.tex;
        if (tex != NULL) {
            return &tex[i & 0xFFF];
        }
    }
    return NULL;
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
        textureset_init(set, arg0);
        textureset_upload((u8 *)set);
        texture_release_loading_mem(set);
    }
    load_file_cb(db->file, dbase_asload_with_anims_callback, db);
}

void dbase_asload_with_anims_callback(u8 *data, obj_dbase *db) {
    mal_set_owner(data, (void **)&db->data);
    dbase_setup_blocks((ModelFile *)data, db->keep);
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

void dbase_copy_block(FileEntry *e, s32 base) {
    BlockRef *node;
    s32 size;

    node = smallnode_alloc();
    e->block = node;
    size = e->end - e->start;
    mal_can_alloc(size);
    mal_alloc_owned_low(size, (void **)node, &MainHeap, 0);
    memcpy(node->ptr, FILE_AT(base, e->start), size);
}

void dbase_anim_release(obj_dbase *db, s32 i) {
    DbaseAnim *a;
    ModelFile *d;

    d = db->data;
    a = (DbaseAnim *)((u8 *)&d->anims + d->anims + 4) + i;
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
    ModelFile *d;

    d = db->data;
    a = (DbaseAnim *)((u8 *)&d->anims + d->anims + 4) + i;
    if (a->data == NULL && a->load == 0) {
        start = a->start;
        a->load = asload_chunk_relocatable(db->file, start, &a->data, a->end - start);
    }
}

s32 dbase_anim_ptr(obj_dbase *arg0, s32 arg1) {
    DbaseAnim *a;
    ModelFile *d;

    d = arg0->data;
    a = (DbaseAnim *)((u8 *)&d->anims + d->anims + 4) + arg1;
    return (s32)a->data;
}


u8 *dbase_mesh_anim_name_release(obj_dbase *arg0, u8 *arg1) {
    ModelFile *tbl;
    FileEntry *p;
    s32 n;
    s32 key;
    FileEntry *res;
    BlockRef *blk;

    tbl = arg0->data;
    if (tbl == NULL) {
        res = NULL;
    } else {
        key = string_hash(arg1);
        p = (FileEntry *)((u8 *)&tbl->meshAnims + tbl->meshAnims);
        n = tbl->nMeshAnims;
        n--;
        for (; n != -1; n--, p++) {
            if (p->hash == key) {
                res = p;
                goto found;
            }
        }
        res = NULL;
    }
found:
    if (res != NULL) {
        blk = res->block;
        if (blk != NULL) {
            if (blk->ptr != NULL) {
                mal_free(blk->ptr, &MainHeap);
            } else {
                loader_cancel((LoadNode *)blk->load);
            }
            smallnode_free((u8 *)blk);
            res->block = NULL;
            return (u8 *)res;
        }
    }
    return NULL;
}

s32 dbase_mesh_anim_name_loaded(obj_dbase *db, char *arg1) {
    ModelFile *tbl;
    FileEntry *p;
    s32 n;
    s32 key;
    FileEntry *res;

    tbl = db->data;
    if (tbl == NULL) {
        res = NULL;
    } else {
        key = string_hash(arg1);
        p = (FileEntry *)((u8 *)&tbl->meshAnims + tbl->meshAnims);
        n = tbl->nMeshAnims;
        n--;
        for (; n != -1; n--, p++) {
            if (p->hash == key) {
                res = p;
                goto found;
            }
        }
        res = NULL;
    }
found:
    if (res != NULL) {
        return (s32)res->block->ptr;
    }
    return 0;
}

u8 *dbase_mesh_anim_name_load(obj_dbase *arg0, u8 *arg1) {
    ModelFile *tbl;
    FileEntry *p;
    s32 n;
    s32 key;
    FileEntry *res;
    BlockRef *node;
    BlockRef *cur;
    s32 start;
    s32 h;

    tbl = arg0->data;
    if (tbl == NULL) {
        res = NULL;
    } else {
        key = string_hash(arg1);
        p = (FileEntry *)((u8 *)&tbl->meshAnims + tbl->meshAnims);
        n = tbl->nMeshAnims;
        n--;
        for (; n != -1; n--, p++) {
            if (p->hash == key) {
                res = p;
                goto found;
            }
        }
        res = NULL;
    }
found:
    if (res != NULL) {
        cur = res->block;
        if (cur != NULL) {
            return (u8 *)cur;
        }
        {
            node = smallnode_alloc();
            if (node != NULL) {
                start = res->start;
                res->block = node;
                h = asload_chunk_relocatable(arg0->file, start, (u32 **)node, res->end - start);
                node->load = h;
                if (h == 0) {
                    smallnode_free((u8 *)node);
                    return NULL;
                }
                return (u8 *)node;
            }
            return NULL;
        }
    }
    return NULL;
}

s32 dbase_mesh_anim_name_size(obj_dbase *arg0, u8 *arg1) {
    ModelFile *tbl;
    FileEntry *p;
    s32 n;
    s32 key;
    s32 d;

    FileEntry *res;

    tbl = arg0->data;
    if (tbl == NULL) {
        res = NULL;
    } else {
        key = string_hash(arg1);
        p = (FileEntry *)((u8 *)&tbl->meshAnims + tbl->meshAnims);
        n = tbl->nMeshAnims;
        n--;
        for (; n != -1; n--, p++) {
            if (p->hash == key) {
                res = p;
                goto found;
            }
        }
        res = NULL;
    }
found:
    d = res->end - res->start;
    return ((d + 0x7FF) / 0x800) * 0x800;
}

FileEntry *dbase_mesh_anim_find(ModelFile *arg0, u8 *arg1) {
    FileEntry *p;
    s32 n;
    s32 key;

    if (arg0 == NULL) {
        return NULL;
    }
    key = string_hash(arg1);
    p = (FileEntry *)((u8 *)&arg0->meshAnims + arg0->meshAnims);
    n = arg0->nMeshAnims;
    n--;
    for (; n != -1; n--, p++) {
        if (p->hash == key) {
            return p;
        }
    }
    return NULL;
}

void dbase_load_all_mesh_anims(obj_dbase *arg0) {
    ModelFile *tbl;
    ModelFile *t;
    FileEntry *rec;
    BlockRef *node;
    s32 n;
    s32 i;
    s32 start;
    s32 handle;

    tbl = arg0->data;
    if (tbl != NULL) {
        n = tbl->nMeshAnims;
        for (i = 0;; i++) {
            if (i >= n) {
                return;
            }
            t = arg0->data;
            if (t != NULL && i < t->nMeshAnims) {
                rec = (FileEntry *)((u8 *)&t->meshAnims + t->meshAnims);
                rec += i;
                if (rec->block == NULL) {
                    node = smallnode_alloc();
                    if (node != NULL) {
                        start = rec->start;
                        rec->block = node;
                        handle = asload_chunk_relocatable(arg0->file, start, (u32 **)node, rec->end - start);
                        node->load = handle;
                        if (handle == 0) {
                            smallnode_free((u8 *)node);
                        }
                    }
                }
            }
        }
    }
}

void dbase_release_all_mesh_anims(a)
obj_dbase *a;
{
    FileEntry *slot;
    BlockRef *e;
    ModelFile *p;
    s32 off;
    s32 n;
    s32 i;
    obj_dbase *arg0;

    arg0 = a;
    i = 0;
    if (arg0->data != NULL) {
        n = arg0->data->nMeshAnims;
        off = 0;
    again:
        if (i < n) {
            p = arg0->data;
            if (p != NULL && i < p->nMeshAnims) {
                slot = (FileEntry *)((u8 *)&p->meshAnims + p->meshAnims);
                slot = (FileEntry *)FILE_AT(slot, off);
                e = slot->block;
                if (e != NULL) {
                    if (e->ptr != NULL) {
                        mal_free(e->ptr, &MainHeap);
                    } else {
                        loader_cancel((void *)e->load);
                    }
                    smallnode_free((u8 *)e);
                    slot->block = NULL;
                }
            }
            off += 0x18;
            i++;
            goto again;
        }
    }
}

u8 *dbase_mesh_anim_release(obj_dbase *arg0, s32 arg1) {
    ModelFile *base;
    FileEntry *e;
    BlockRef *item;

    base = arg0->data;
    if (base != NULL) {
        if (arg1 < base->nMeshAnims) {
            e = (FileEntry *)((u8 *)&base->meshAnims + base->meshAnims);
            e += arg1;
            item = e->block;
            if (item != NULL) {
                if (item->ptr != NULL) {
                    mal_free(item->ptr, &MainHeap);
                } else {
                    loader_cancel((void *)item->load);
                }
                smallnode_free((u8 *)item);
                e->block = NULL;
                return (u8 *)e;
            }
        }
    }
    return NULL;
}

/* Unloads what dbase_mesh_anim_rec_load loaded: the record's node at +0x14 holds a heap
 * block or (if that is NULL) a handle at +4. */
FileEntry *dbase_mesh_anim_rec_release(s32 *arg0, FileEntry *rec) {
    BlockRef *node;

    node = rec->block;
    if (node == NULL) {
        return NULL;
    }
    if (node->ptr != NULL) {
        mal_free(node->ptr, &MainHeap);
    } else {
        loader_cancel((void *)node->load);
    }
    smallnode_free((u8 *)node);
    rec->block = NULL;
    return rec;
}

/* Inlined copy of dbase_mesh_anim_rec_load (load a record's node); being an inline call
 * is what puts arg1 in v1 and the table in a1. */
static inline BlockRef *dbase_mesh_anim_load_one(obj_dbase *arg0, FileEntry *arg1) {
    BlockRef *cur;
    BlockRef *node;
    s32 start;
    s32 h;

    cur = arg1->block;
    if (cur == NULL) {
        node = smallnode_alloc();
        if (node != NULL) {
            start = arg1->start;
            arg1->block = node;
            h = asload_chunk_relocatable(arg0->file, start, (u32 **)node, arg1->end - start);
            node->load = h;
            if (h != 0) {
                goto ok;
            }
            smallnode_free((u8 *)node);
        }
        return NULL;
    ok:
        return node;
    }
    return cur;
}

BlockRef *dbase_mesh_anim_load(obj_dbase *arg0, s32 arg1) {
    ModelFile *base;
    FileEntry *e;

    base = arg0->data;
    if (base == NULL || arg1 >= base->nMeshAnims) {
        return NULL;
    }
    e = (FileEntry *)((u8 *)&base->meshAnims + base->meshAnims);
    e += arg1;
    return dbase_mesh_anim_load_one(arg0, e);
}

BlockRef *dbase_mesh_anim_rec_load(file **f, FileEntry *e) {
    s32 start;
    s32 load;
    BlockRef *node;
    BlockRef *ret;

    ret = e->block;
    if (ret == NULL) {
        node = smallnode_alloc();
        if (node != NULL) {
            start = e->start;
            e->block = node;
            load = asload_chunk_relocatable(*f, start, (u32 **)node, e->end - start);
            node->load = load;
            if (load != 0) {
                return node;
            }
            smallnode_free(node);
            goto fail;
        }
fail:
        ret = NULL;
        return ret;
    }
    return ret;
}

void dbase_load_textures_wait(obj_dbase *rec) {
    if (rec->load == 0 && rec->data == 0) {
        rec->load = load_file_cb(rec->textureFile, dbase_textures_loaded, rec);
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

void dbase_textures_loaded(u8 *data, obj_dbase *rec) {
    _mal_area *heap;
    s32 size;

    size = textureset_mem_size(data);
    heap = &MainHeap;
    if (mal_first_free((size + 3) >> 2, heap->free) == NULL) {
        mal_free(data, heap);
        rec->load = 0;
        dbase_load_textures(rec);
    } else {
        textureset_init(&rec->textures, data);
        textureset_upload(&rec->textures);
        texture_release_loading_mem(&rec->textures);
    }
    rec->load = loader_queue(rec->file, 0, rec->textures.f14, dbase_set_data, rec, 1, 0);
}

void dbase_group_textures_loaded(u8 *arg0, obj_dbase *arg1) {
    _mal_area *heap;
    s32 n;

    n = textureset_mem_size(arg0);
    heap = &MainHeap;
    if (mal_first_free((n + 3) >> 2, heap->free) == NULL) {
        mal_free(arg0, heap);
        arg1->load = 0;
        dbase_load_textures(arg1);
    } else {
        textureset_init(&arg1->textures, arg0);
        textureset_upload(&arg1->textures);
        texture_release_loading_mem(&arg1->textures);
    }
}


void dbase_set_textures(obj_dbase *db, u8 *base) {
    u8 *ts;

    ts = (u8 *)&db->textures;
    textureset_init((obj_textureset *)ts, base);
    textureset_upload(ts);
    db->textures.base = NULL;
}

void dbase_set_data(u8 *arg0, u8 *arg1) {
    mal_set_owner(arg0, arg1 + 0xC);
}

void dbase_release(obj_dbase *arg0) {
    s32 i;
    ModelFile *p;
    MeshHeader *q;

    if (arg0->data != NULL) {
        dbase_release_all_mesh_anims();
        p = arg0->data;
        q = p->meshes;
        i = p->nMeshes;
        i--;
        if (i != -1) {
            do {
                PrimSet *e = q->set;
                if (e != NULL) {
                    e->dead = 1;
                }
                i--;
                q++;
            } while (i != -1);
        }
        mal_free(arg0->data, &MainHeap);
        arg0->data = 0;
        textureset_release(&arg0->textures);
        arg0->load = 0;
        return;
    }
    if (arg0->load != 0) {
        if (arg0->textures.tex != 0) {
            textureset_release(&arg0->textures);
        }
        loader_cancel(arg0->load);
        arg0->load = 0;
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

void dbase_set(obj_dbase *db, u8 *data, u8 *base) {
    u8 *ts;

    ts = (u8 *)&db->textures;
    db->data = data;
    db->textures.base = (TexFile *)base;
    textureset_init((obj_textureset *)ts, base);
    textureset_upload(ts);
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

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */

obj_dbase HeroModelBank[DB_COUNT] = {
    { 0 }, /* DB_Empty */
    { &DatIndex[990], &DatIndex[989] }, /* DB_CharsCrateCrash */
    { &DatIndex[988], &DatIndex[987] }, /* DB_CharsCrateCoco */
    { &DatIndex[986], &DatIndex[985] }, /* DB_CharsCrateCortex */
    { &DatIndex[984], &DatIndex[983] }, /* DB_CharsCrateBrio */
    { &DatIndex[982], &DatIndex[981] }, /* DB_CharsCrateTiny */
    { &DatIndex[976], &DatIndex[975] }, /* DB_CharsCrateKong */
    { &DatIndex[980], &DatIndex[979] }, /* DB_CharsCrateDingo */
    { &DatIndex[978], &DatIndex[977] }, /* DB_CharsCrateRilla */
    { &DatIndex[974], &DatIndex[973] }, /* DB_CharsPogoCrash */
    { &DatIndex[972], &DatIndex[971] }, /* DB_CharsPogoCoco */
    { &DatIndex[970], &DatIndex[969] }, /* DB_CharsPogoCortex */
    { &DatIndex[968], &DatIndex[967] }, /* DB_CharsPogoBrio */
    { &DatIndex[966], &DatIndex[965] }, /* DB_CharsPogoTiny */
    { &DatIndex[962], &DatIndex[961] }, /* DB_CharsPogoDingo */
    { &DatIndex[964], &DatIndex[963] }, /* DB_CharsPogoKong */
    { &DatIndex[960], &DatIndex[959] }, /* DB_CharsPogoRilla */
    { &DatIndex[958], &DatIndex[957] }, /* DB_CharsPolar2Crash */
    { &DatIndex[956], &DatIndex[955] }, /* DB_CharsPolar2Coco */
    { &DatIndex[954], &DatIndex[953] }, /* DB_CharsPolar2Cortex */
    { &DatIndex[952], &DatIndex[951] }, /* DB_CharsPolar2Tiny */
    { &DatIndex[950], &DatIndex[949] }, /* DB_CharsPolar2Dingo */
    { &DatIndex[948], &DatIndex[947] }, /* DB_CharsPolar2Brio */
    { &DatIndex[736], &DatIndex[735] }, /* DB_Powerups */
    { &DatIndex[796], &DatIndex[795] }, /* DB_CrateObjects */
    { &DatIndex[794], &DatIndex[793] }, /* DB_RingObjects */
    { &DatIndex[792], &DatIndex[791] }, /* DB_Angel */
    { &DatIndex[798], &DatIndex[797] }, /* DB_Penguin */
    { &DatIndex[732], &DatIndex[731] }, /* DB_Interface */
    { &DatIndex[732], &DatIndex[731] }, /* DB_InterfaceInterface22 */
    { &DatIndex[732], &DatIndex[731] }, /* DB_InterfaceInterface22_2 */
    { &DatIndex[732], &DatIndex[731] }, /* DB_InterfaceInterface22_3 */
    { &DatIndex[732], &DatIndex[731] }, /* DB_InterfaceInterface22_4 */
    { &DatIndex[732], &DatIndex[731] }, /* DB_InterfaceInterface22_5 */
    { &DatIndex[711], &DatIndex[710] }, /* DB_ArenaCrateSpaceArena */
    { &DatIndex[709], &DatIndex[708] }, /* DB_ArenaCrateSpaceCrystalarena */
    { &DatIndex[715], &DatIndex[714] }, /* DB_ArenaCrateJungleArena */
    { &DatIndex[713], &DatIndex[712] }, /* DB_ArenaCrateJungleCrystalarena */
    { &DatIndex[707], &DatIndex[706] }, /* DB_ArenaCrateSnowArena */
    { &DatIndex[705], &DatIndex[704] }, /* DB_ArenaCrateSnowCrystalarena */
    { &DatIndex[703], &DatIndex[702] }, /* DB_ArenaCrateDrainArena */
    { &DatIndex[701], &DatIndex[700] }, /* DB_ArenaCrateDrainCrystalarena */
    { &DatIndex[699], &DatIndex[698] }, /* DB_ArenaCrateTiebreakerArena */
    { &DatIndex[697], &DatIndex[696] }, /* DB_ArenaBossPapuArena */
    { &DatIndex[790], &DatIndex[789] }, /* DB_Papu */
    { &DatIndex[788], &DatIndex[787] }, /* DB_Minion */
    { &DatIndex[695], &DatIndex[694] }, /* DB_ArenaTankSwampArena */
    { &DatIndex[693], &DatIndex[692] }, /* DB_ArenaTankDesertArena */
    { &DatIndex[691], &DatIndex[690] }, /* DB_ArenaTankJungleArena */
    { &DatIndex[689], &DatIndex[688] }, /* DB_ArenaTankMetalArena */
    { &DatIndex[687], &DatIndex[686] }, /* DB_ArenaBossKomodoArena */
    { &DatIndex[685], &DatIndex[684] }, /* DB_ArenaTankSwampCrystalarena */
    { &DatIndex[683], &DatIndex[682] }, /* DB_ArenaTankDesertCrystalarena */
    { &DatIndex[681], &DatIndex[680] }, /* DB_ArenaTankJungleCrystalarena */
    { &DatIndex[679], &DatIndex[678] }, /* DB_ArenaTankMetalCrystalarena */
    { &DatIndex[786], &DatIndex[785] }, /* DB_TankObjects */
    { &DatIndex[784], &DatIndex[783] }, /* DB_TankObjects2 */
    { &DatIndex[782], &DatIndex[781] }, /* DB_ArenaBossKomodoArenaobjects */
    { &DatIndex[780], &DatIndex[779] }, /* DB_ArenaBossKomodoKomodo */
    { &DatIndex[778], &DatIndex[777] }, /* DB_ArenaBossKomodoKomodotank */
    { &DatIndex[774], &DatIndex[773] }, /* DB_TankVehicle */
    { &DatIndex[776], &DatIndex[775] }, /* DB_ArenaTankJungleObjects */
    { &DatIndex[894], &DatIndex[893] }, /* DB_CharsTankCrash */
    { &DatIndex[892], &DatIndex[891] }, /* DB_CharsTankCoco */
    { &DatIndex[890], &DatIndex[889] }, /* DB_CharsTankCortex */
    { &DatIndex[888], &DatIndex[887] }, /* DB_CharsTankTiny */
    { &DatIndex[886], &DatIndex[885] }, /* DB_CharsTankBrio */
    { &DatIndex[884], &DatIndex[883] }, /* DB_CharsTankDingo */
    { &DatIndex[882], &DatIndex[881] }, /* DB_CharsTankKong */
    { &DatIndex[880], &DatIndex[879] }, /* DB_CharsTankRilla */
    { &DatIndex[772], &DatIndex[771] }, /* DB_ArenaMedievalChickenChicken */
    { &DatIndex[675], &DatIndex[674] }, /* DB_ArenaPogoPainterArena */
    { &DatIndex[671], &DatIndex[670] }, /* DB_ArenaPogoGogoArena */
    { &DatIndex[667], &DatIndex[666] }, /* DB_ArenaPogoEplArena */
    { &DatIndex[663], &DatIndex[662] }, /* DB_ArenaPogoPadlockArena */
    { &DatIndex[673], &DatIndex[672] }, /* DB_ArenaPogoPainterCrystalarena */
    { &DatIndex[669], &DatIndex[668] }, /* DB_ArenaPogoGogoCrystalarena */
    { &DatIndex[665], &DatIndex[664] }, /* DB_ArenaPogoEplCrystalarena */
    { &DatIndex[661], &DatIndex[660] }, /* DB_ArenaPogoPadlockCrystalarena */
    { &DatIndex[770], &DatIndex[769] }, /* DB_PogoObjects */
    { &DatIndex[768], &DatIndex[767] }, /* DB_Roo */
    { &DatIndex[617], &DatIndex[616] }, /* DB_ArenaMedievalMalletArena */
    { &DatIndex[615], &DatIndex[614] }, /* DB_ArenaMedievalMalletCrystalarena */
    { &DatIndex[343], &DatIndex[342] }, /* DB_MalletObjects */
    { &DatIndex[862], &DatIndex[861] }, /* DB_CharsMalletCrash */
    { &DatIndex[860], &DatIndex[859] }, /* DB_CharsMalletCoco */
    { &DatIndex[858], &DatIndex[857] }, /* DB_CharsMalletCortex */
    { &DatIndex[856], &DatIndex[855] }, /* DB_CharsMalletBrio */
    { &DatIndex[854], &DatIndex[853] }, /* DB_CharsMalletDingo */
    { &DatIndex[852], &DatIndex[851] }, /* DB_CharsMalletRilla */
    { &DatIndex[850], &DatIndex[849] }, /* DB_CharsMalletTiny */
    { &DatIndex[848], &DatIndex[847] }, /* DB_CharsMalletKong */
    { &DatIndex[613], &DatIndex[612] }, /* DB_ArenaBossOxideArena */
    { &DatIndex[742], &DatIndex[741] }, /* DB_Oxide */
    { &DatIndex[740], &DatIndex[739] }, /* DB_PlayerVehicle */
    { &DatIndex[609], &DatIndex[608] }, /* DB_ArenaMedievalKegArena */
    { &DatIndex[607], &DatIndex[606] }, /* DB_ArenaMedievalKegCrystalarena */
    { &DatIndex[738], &DatIndex[737] }, /* DB_KegObjects */
    { &DatIndex[878], &DatIndex[877] }, /* DB_CharsKegsCrash */
    { &DatIndex[876], &DatIndex[875] }, /* DB_CharsKegsCoco */
    { &DatIndex[874], &DatIndex[873] }, /* DB_CharsKegsCortex */
    { &DatIndex[872], &DatIndex[871] }, /* DB_CharsKegsTiny */
    { &DatIndex[870], &DatIndex[869] }, /* DB_CharsKegsBrio */
    { &DatIndex[868], &DatIndex[867] }, /* DB_CharsKegsDingo */
    { &DatIndex[866], &DatIndex[865] }, /* DB_CharsKegsKong */
    { &DatIndex[864], &DatIndex[863] }, /* DB_CharsKegsRilla */
    { &DatIndex[659], &DatIndex[658] }, /* DB_ArenaBallsCrashArena */
    { &DatIndex[657], &DatIndex[656] }, /* DB_ArenaBallsCrashCrystalarena */
    { &DatIndex[655], &DatIndex[654] }, /* DB_ArenaBallsBeachArena */
    { &DatIndex[653], &DatIndex[652] }, /* DB_ArenaBallsBeachCrystalarena */
    { &DatIndex[651], &DatIndex[650] }, /* DB_ArenaBallsNginArena */
    { &DatIndex[649], &DatIndex[648] }, /* DB_ArenaBallsNginCrystalarena */
    { &DatIndex[647], &DatIndex[646] }, /* DB_ArenaBallsSkyArena */
    { &DatIndex[645], &DatIndex[644] }, /* DB_ArenaBallsSkyCrystalarena */
    { &DatIndex[910], &DatIndex[909] }, /* DB_CharsBallCrash */
    { &DatIndex[908], &DatIndex[907] }, /* DB_CharsBallCoco */
    { &DatIndex[906], &DatIndex[905] }, /* DB_CharsBallCortex */
    { &DatIndex[904], &DatIndex[903] }, /* DB_CharsBallBrio */
    { &DatIndex[902], &DatIndex[901] }, /* DB_CharsBallDingo */
    { &DatIndex[900], &DatIndex[899] }, /* DB_CharsBallRilla */
    { &DatIndex[898], &DatIndex[897] }, /* DB_CharsBallTiny */
    { &DatIndex[896], &DatIndex[895] }, /* DB_CharsBallKong */
    { &DatIndex[762], &DatIndex[761] }, /* DB_BallsEffects */
    { &DatIndex[766], &DatIndex[765] }, /* DB_BallsVehicle */
    { &DatIndex[764], &DatIndex[763] }, /* DB_Ngin */
    { &DatIndex[627], &DatIndex[626] }, /* DB_ArenaMedievalRingArena */
    { &DatIndex[625], &DatIndex[624] }, /* DB_ArenaMedievalRingCrystalarena */
    { &DatIndex[486], &DatIndex[485] }, /* DB_Unkscenery */
    { &DatIndex[484], &DatIndex[483] }, /* DB_Polar */
    { &DatIndex[482], &DatIndex[481] }, /* DB_Masks */
    { &DatIndex[480], &DatIndex[479] }, /* DB_PolarObjects */
    { &DatIndex[478], &DatIndex[477] }, /* DB_ArenaPolarPolarArena */
    { &DatIndex[474], &DatIndex[473] }, /* DB_ArenaPolarMeltArena */
    { &DatIndex[468], &DatIndex[467] }, /* DB_ArenaPolarTiltSeal */
    { &DatIndex[466], &DatIndex[465] }, /* DB_ArenaPolarManicArena */
    { &DatIndex[942], &DatIndex[941] }, /* DB_CharsCrate2Crash */
    { &DatIndex[940], &DatIndex[939] }, /* DB_CharsCrate2Coco */
    { &DatIndex[938], &DatIndex[937] }, /* DB_CharsCrate2Cortex */
    { &DatIndex[936], &DatIndex[935] }, /* DB_CharsCrate2Tiny */
    { &DatIndex[934], &DatIndex[933] }, /* DB_CharsCrate2Dingo */
    { &DatIndex[932], &DatIndex[931] }, /* DB_CharsCrate2Brio */
    { &DatIndex[472], &DatIndex[471] }, /* DB_ArenaBossBearArena */
    { &DatIndex[464], &DatIndex[463] }, /* DB_ArenaBossBearBigbear */
    { &DatIndex[470], &DatIndex[469] }, /* DB_Modelmix */
    { &DatIndex[946], &DatIndex[945] }, /* DB_CharsPolar2Kong */
    { &DatIndex[462], &DatIndex[461] }, /* DB_RoboPolar */
    { &DatIndex[944], &DatIndex[943] }, /* DB_CharsPolar2Rilla */
    { &DatIndex[476], &DatIndex[475] }, /* DB_ArenaPolarTiltCrystalarena */
    { &DatIndex[928], &DatIndex[927] }, /* DB_CharsCrate2Rilla */
    { &DatIndex[930], &DatIndex[929] }, /* DB_CharsCrate2Kong */
    { &DatIndex[460], &DatIndex[459] }, /* DB_ArenaPolarManicCrystalarena */
    { &DatIndex[458], &DatIndex[457] }, /* DB_ArenaPolarMeltCrystalarena */
    { &DatIndex[456], &DatIndex[455] }, /* DB_ArenaPolarPolarCrystalarena */
    { &DatIndex[926], &DatIndex[925] }, /* DB_CharsPolarCrash */
    { &DatIndex[924], &DatIndex[923] }, /* DB_CharsPolarCoco */
    { &DatIndex[922], &DatIndex[921] }, /* DB_CharsPolarCortex */
    { &DatIndex[920], &DatIndex[919] }, /* DB_CharsPolarTiny */
    { &DatIndex[918], &DatIndex[917] }, /* DB_CharsPolarDingo */
    { &DatIndex[916], &DatIndex[915] }, /* DB_CharsPolarBrio */
    { &DatIndex[914], &DatIndex[913] }, /* DB_CharsPolarRilla */
    { &DatIndex[912], &DatIndex[911] }, /* DB_CharsPolarKong */
    { &DatIndex[643], &DatIndex[642] }, /* DB_ArenaDashDotArena */
    { &DatIndex[830], &DatIndex[829] }, /* DB_CharsRaceCrash */
    { &DatIndex[828], &DatIndex[827] }, /* DB_CharsRaceCortex */
    { &DatIndex[826], &DatIndex[825] }, /* DB_CharsRaceTiny */
    { &DatIndex[824], &DatIndex[823] }, /* DB_CharsRaceDingo */
    { &DatIndex[822], &DatIndex[821] }, /* DB_CharsRaceBrio */
    { &DatIndex[639], &DatIndex[638] }, /* DB_ArenaDashToxicArena */
    { &DatIndex[641], &DatIndex[640] }, /* DB_ArenaDashSplashArena */
    { &DatIndex[637], &DatIndex[636] }, /* DB_ArenaDashDanteArena */
    { &DatIndex[760], &DatIndex[759] }, /* DB_CrateEffects */
    { &DatIndex[818], &DatIndex[817] }, /* DB_CharsRaceCoco */
    { &DatIndex[816], &DatIndex[815] }, /* DB_CharsRaceRilla */
    { &DatIndex[820], &DatIndex[819] }, /* DB_CharsRaceKong */
    { &DatIndex[814], &DatIndex[813] }, /* DB_CharsSplashCrash */
    { &DatIndex[812], &DatIndex[811] }, /* DB_CharsSplashCortex */
    { &DatIndex[810], &DatIndex[809] }, /* DB_CharsSplashTiny */
    { &DatIndex[808], &DatIndex[807] }, /* DB_CharsSplashDingo */
    { &DatIndex[806], &DatIndex[805] }, /* DB_CharsSplashKong */
    { &DatIndex[800], &DatIndex[799] }, /* DB_CharsSplashBrio */
    { &DatIndex[804], &DatIndex[803] }, /* DB_CharsSplashCoco */
    { &DatIndex[802], &DatIndex[801] }, /* DB_CharsSplashRilla */
    { &DatIndex[635], &DatIndex[634] }, /* DB_ArenaDashToxicCrystalarena */
    { &DatIndex[633], &DatIndex[632] }, /* DB_ArenaDashDanteCrystalarena */
    { &DatIndex[631], &DatIndex[630] }, /* DB_ArenaDashDotCrystalarena */
    { &DatIndex[629], &DatIndex[628] }, /* DB_ArenaDashSplashCrystalarena */
    { &DatIndex[623], &DatIndex[622] }, /* DB_ArenaMedievalDragonCrystalarena */
    { &DatIndex[846], &DatIndex[845] }, /* DB_CharsDinoCrash */
    { &DatIndex[844], &DatIndex[843] }, /* DB_CharsDinoCortex */
    { &DatIndex[842], &DatIndex[841] }, /* DB_CharsDinoTiny */
    { &DatIndex[840], &DatIndex[839] }, /* DB_CharsDinoDingo */
    { &DatIndex[838], &DatIndex[837] }, /* DB_CharsDinoCoco */
    { &DatIndex[834], &DatIndex[833] }, /* DB_CharsDinoRilla */
    { &DatIndex[836], &DatIndex[835] }, /* DB_CharsDinoBrio */
    { &DatIndex[832], &DatIndex[831] }, /* DB_CharsDinoKong */
    { &DatIndex[621], &DatIndex[620] }, /* DB_ArenaMedievalDragonArena */
    { &DatIndex[619], &DatIndex[618] }, /* DB_DragonObjects */
    { &DatIndex[135], &DatIndex[134] }, /* DB_ArenaTestObjects */
    { &DatIndex[730], &DatIndex[729] }, /* DB_Loading */
    { &DatIndex[131], &DatIndex[130] }, /* DB_MainmenuModels */
    { &DatIndex[129], &DatIndex[128] }, /* DB_MainmenuArenaModels2 */
    { &DatIndex[127], &DatIndex[126] }, /* DB_MainmenuArenaModels1 */
    { &DatIndex[734], &DatIndex[733] }, /* DB_Font */
    { &DatIndex[125], &DatIndex[124] }, /* DB_CutsceneIntroEurocom */
    { &DatIndex[123], &DatIndex[122] }, /* DB_CutsceneIntroLogo2 */
    { &DatIndex[379], &DatIndex[378] }, /* DB_MainmenuModels2 */
    { &DatIndex[393], &DatIndex[392] }, /* DB_WarpRoom1Level */
    { &DatIndex[391], &DatIndex[390] }, /* DB_WarpRoom2Level */
    { &DatIndex[389], &DatIndex[388] }, /* DB_WarpRoom3Level */
    { &DatIndex[387], &DatIndex[386] }, /* DB_WarpRoom4Level */
    { &DatIndex[385], &DatIndex[384] }, /* DB_WarpRoom5Level */
    { &DatIndex[383], &DatIndex[382] }, /* DB_DemoHub2Level */
    { &DatIndex[381], &DatIndex[380] }, /* DB_DemoHub1Level */
    { &DatIndex[377], &DatIndex[376] }, /* DB_AdventureItems */
    { &DatIndex[758], &DatIndex[757] }, /* DB_CharsWarpCrash */
    { &DatIndex[756], &DatIndex[755] }, /* DB_CharsWarpCoco */
    { &DatIndex[754], &DatIndex[753] }, /* DB_CharsWarpCortex */
    { &DatIndex[752], &DatIndex[751] }, /* DB_CharsWarpBrio */
    { &DatIndex[750], &DatIndex[749] }, /* DB_CharsWarpTiny */
    { &DatIndex[748], &DatIndex[747] }, /* DB_CharsWarpKong */
    { &DatIndex[746], &DatIndex[745] }, /* DB_CharsWarpDingo */
    { &DatIndex[744], &DatIndex[743] }, /* DB_CharsWarpRilla */
    { &DatIndex[121], &DatIndex[120] }, /* DB_CutsceneIntroLogo */
    { &DatIndex[119], &DatIndex[118] }, /* DB_CutsceneLevelShot19 */
    { &DatIndex[117], &DatIndex[116] }, /* DB_CutsceneLevelShot18 */
    { &DatIndex[115], &DatIndex[114] }, /* DB_CutsceneLevelShot17 */
    { &DatIndex[113], &DatIndex[112] }, /* DB_CutsceneLevelShot16 */
    { &DatIndex[111], &DatIndex[110] }, /* DB_CutsceneLevelShot15 */
    { &DatIndex[109], &DatIndex[108] }, /* DB_CutsceneLevelShot14 */
    { &DatIndex[107], &DatIndex[106] }, /* DB_CutsceneLevelIntroCrashplain */
    { &DatIndex[105], &DatIndex[104] }, /* DB_CutsceneLevelIntroCortexlab */
    { &DatIndex[103], &DatIndex[102] }, /* DB_CutsceneLevelIntroShotGroupTeamgood2 */
    { &DatIndex[101], &DatIndex[100] }, /* DB_CutsceneLevelIntroShotWelcome3 */
    { &DatIndex[99], &DatIndex[98] }, /* DB_CutsceneLevelIntroShotWelcome2 */
    { &DatIndex[97], &DatIndex[96] }, /* DB_CutsceneLevelIntroShotWelcome */
    { &DatIndex[95], &DatIndex[94] }, /* DB_CutsceneLevelIntroShotGroupTeamevil4 */
    { &DatIndex[93], &DatIndex[92] }, /* DB_CutsceneLevelIntroShotGroupTeamevil3 */
    { &DatIndex[91], &DatIndex[90] }, /* DB_CutsceneLevelShot13 */
    { &DatIndex[89], &DatIndex[88] }, /* DB_CutsceneLevelIntroShotGroupTeamevil2 */
    { &DatIndex[87], &DatIndex[86] }, /* DB_CutsceneLevelShot12 */
    { &DatIndex[85], &DatIndex[84] }, /* DB_CutsceneLevelIntroShotGroupTeamevil */
    { &DatIndex[83], &DatIndex[82] }, /* DB_CutsceneLevelIntroShotGroupTeamgood */
    { &DatIndex[37], &DatIndex[36] }, /* DB_CutsceneLevel4 */
    { &DatIndex[35], &DatIndex[34] }, /* DB_CutsceneLevel3 */
    { &DatIndex[33], &DatIndex[32] }, /* DB_CutsceneLevel2 */
    { &DatIndex[31], &DatIndex[30] }, /* DB_CutsceneLevel */
    { &DatIndex[61], &DatIndex[60] }, /* DB_CutsceneLevelEndingGoodShot3 */
    { &DatIndex[59], &DatIndex[58] }, /* DB_CutsceneLevelShot8 */
    { &DatIndex[57], &DatIndex[56] }, /* DB_CutsceneLevelShot7 */
    { &DatIndex[55], &DatIndex[54] }, /* DB_CutsceneLevelShot6 */
    { &DatIndex[53], &DatIndex[52] }, /* DB_CutsceneLevelEndingGoodShot2 */
    { &DatIndex[51], &DatIndex[50] }, /* DB_CutsceneLevelShot5 */
    { &DatIndex[49], &DatIndex[48] }, /* DB_CutsceneLevelShot4 */
    { &DatIndex[47], &DatIndex[46] }, /* DB_CutsceneLevelShot3 */
    { &DatIndex[45], &DatIndex[44] }, /* DB_CutsceneLevelShot2 */
    { &DatIndex[43], &DatIndex[42] }, /* DB_CutsceneLevelEndingEvilShot1 */
    { &DatIndex[41], &DatIndex[40] }, /* DB_CutsceneLevelEndingGoodShot1 */
    { &DatIndex[39], &DatIndex[38] }, /* DB_CutsceneLevelIntro */
    { &DatIndex[81], &DatIndex[80] }, /* DB_CutsceneLevelEndingEvilShot8 */
    { &DatIndex[79], &DatIndex[78] }, /* DB_CutsceneLevelShot11 */
    { &DatIndex[77], &DatIndex[76] }, /* DB_CutsceneLevelShot10 */
    { &DatIndex[75], &DatIndex[74] }, /* DB_CutsceneLevelEndingEvilShot7 */
    { &DatIndex[73], &DatIndex[72] }, /* DB_CutsceneLevelEndingEvilShot6 */
    { &DatIndex[71], &DatIndex[70] }, /* DB_CutsceneLevelShot9 */
    { &DatIndex[69], &DatIndex[68] }, /* DB_CutsceneLevelEndingEvilShot5 */
    { &DatIndex[67], &DatIndex[66] }, /* DB_CutsceneLevelEndingEvilShot4 */
    { &DatIndex[65], &DatIndex[64] }, /* DB_CutsceneLevelEndingEvilShot3 */
    { &DatIndex[63], &DatIndex[62] }, /* DB_CutsceneLevelEndingEvilShot2 */
    { &DatIndex[27], &DatIndex[26] }, /* DB_CutsceneTextGoodwins */
    { &DatIndex[29], &DatIndex[28] }, /* DB_CutsceneTextEvilwins */
    { &DatIndex[25], &DatIndex[24] }, /* DB_CutsceneTextMixteamwins */
    { &DatIndex[23], &DatIndex[22] }, /* DB_CutsceneTextCredits */
    { &DatIndex[21], &DatIndex[20] }, /* DB_CutsceneTextIntromovie */
    { &DatIndex[7], &DatIndex[6] }, /* DB_CutsceneUkaData4 */
    { &DatIndex[5], &DatIndex[4] }, /* DB_CutsceneUkaData3 */
    { &DatIndex[3], &DatIndex[2] }, /* DB_CutsceneUkaData2 */
    { &DatIndex[1], &DatIndex[0] }, /* DB_CutsceneUkaData */
    { &DatIndex[15], &DatIndex[14] }, /* DB_CutsceneAkuData4 */
    { &DatIndex[13], &DatIndex[12] }, /* DB_CutsceneAkuData3 */
    { &DatIndex[11], &DatIndex[10] }, /* DB_CutsceneAkuData2 */
    { &DatIndex[9], &DatIndex[8] }, /* DB_CutsceneAkuData */
    { &DatIndex[19], &DatIndex[18] }, /* DB_CutsceneAkuUkaData2 */
    { &DatIndex[17], &DatIndex[16] }, /* DB_CutsceneAkuUkaData */
};
