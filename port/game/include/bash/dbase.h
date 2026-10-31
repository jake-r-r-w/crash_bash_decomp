#ifndef BASH_DBASE_H
#define BASH_DBASE_H

/* Functions and data defined in src/main/dbase.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/dbase.c */

#include "common.h"
#include "bash/main.h"

struct obj_texture *dbase_texture_ptr(obj_dbase *db, s32 i);
void dbase_asload_with_anims(obj_dbase *db, void **keep);
void dbase_asload_with_anims_callback(u8 *data, obj_dbase *db);
void dbase_copy_block(FileEntry *e, u8 *base);
/* One of a .mdl's animations that load on their own (dbase_anim_load): its byte range in the
 * file, the loaded copy and the load in flight. The table (a count, then these) is at the
 * data's +0x3C offset. */
typedef struct DbaseAnim {
    /* 0x00 */ s32 start;
    /* 0x04 */ s32 end;
    /* 0x08 */ ref32 data;  /* a u32 *, in the file */
    /* 0x0C */ ref32 load;  /* a LoadNode *, in the file */
} DbaseAnim;

void dbase_anim_load(obj_dbase *db, s32 i);
BlockRef *dbase_mesh_anim_rec_load(file **f, FileEntry *e);
void dbase_load_textures(obj_dbase *db);
void dbase_set_textures(obj_dbase *db, u8 *base);
void load_font(void);
void dbase_set(obj_dbase *db, u8 *data, u8 *base);
void dbase_init(obj_dbase *db);
void dbase_textures_loaded(u8 *data, obj_dbase *rec);
void dbase_asload_with_anims_texture_callback(u8 *arg0, obj_dbase *db);
void dbase_set_data(u8 *arg0, u8 *arg1);
struct FileEntry *dbase_mesh_anim_rec_release(s32 *arg0, struct FileEntry *rec);
struct TexClut *dbase_texture_entry(obj_dbase *db, s32 i);
void *name_list_find(void **list, u8 *arg1);
void dbase_load_all_mesh_anims(obj_dbase *arg0);
void dbase_load_textures_wait(obj_dbase *rec);
void dbase_anim_release(obj_dbase *db, s32 i);
void *dbase_anim_ptr(obj_dbase *arg0, s32 arg1);
void dbase_group_textures_loaded(u8 *arg0, obj_dbase *arg1);
void slotlist_mark(SlotList *arg0);
void dbase_release(obj_dbase *arg0);
void dbase_init_all(void);
struct FileEntry *dbase_mesh_anim_find(struct ModelFile *arg0, u8 *arg1);
u8 *dbase_mesh_anim_name_release(obj_dbase *arg0, u8 *arg1);
void *dbase_mesh_anim_name_loaded(obj_dbase *db, char *arg1);
u8 *dbase_mesh_anim_name_load(obj_dbase *arg0, u8 *arg1);
s32 dbase_mesh_anim_name_size(obj_dbase *arg0, u8 *arg1);
u8 *dbase_mesh_anim_release(obj_dbase *arg0, s32 arg1);
struct BlockRef *dbase_mesh_anim_load(obj_dbase *arg0, s32 arg1);
void dbase_setup_blocks(ModelFile *arg0, void **arg1);


#endif /* BASH_DBASE_H */
