#ifndef BASH_DBASE_H
#define BASH_DBASE_H

/* Functions and data defined in src/main/dbase.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/dbase.c */

#include "common.h"
#include "bash/main.h"

s32 dbase_texture_ptr(u8 *arg0, s32 arg1);
void dbase_asload_with_anims(u8 *arg0, s32 arg1);
void dbase_asload_with_anims_callback(u8 *arg0, u8 *arg1);
void dbase_copy_block(u8 *arg0, s32 arg1);
/* One of a .mdl's animations that load on their own (dbase_anim_load): its byte range in the
 * file, the loaded copy and the load in flight. The table (a count, then these) is at the
 * data's +0x3C offset. */
typedef struct DbaseAnim {
    /* 0x00 */ s32 start;
    /* 0x04 */ s32 end;
    /* 0x08 */ u32 *data;
    /* 0x0C */ s32 load;
} DbaseAnim;

void dbase_anim_load(obj_dbase *db, s32 i);
u8 *dbase_mesh_anim_rec_load(s32 *arg0, u8 *arg1);
void dbase_load_textures(obj_dbase *db);
void dbase_set_textures(u8 *arg0, u8 *arg1);
void load_font(void);
void dbase_set(u8 *arg0, s32 arg1, s32 arg2);
void dbase_init(obj_dbase *db);
void dbase_textures_loaded();
void dbase_asload_with_anims_texture_callback(u8 *arg0, obj_dbase *db);
void dbase_set_data(u8 *arg0, u8 *arg1);
u8 *dbase_mesh_anim_rec_release(s32 *arg0, u8 *rec);
u8 *dbase_texture_entry(u8 *arg0, s32 i);
s32 name_list_find(s32 *list, u8 *arg1);
void dbase_load_all_mesh_anims(s32 *arg0);
void dbase_load_textures_wait(u8 *rec);
void dbase_anim_release(obj_dbase *db, s32 i);
s32 dbase_anim_ptr(u8 *arg0, s32 arg1);
void dbase_group_textures_loaded(u8 *arg0, u8 *arg1);
void slotlist_mark(SlotList *arg0);
void dbase_release(u8 *arg0);
void dbase_init_all(void);
u8 *dbase_mesh_anim_find(u8 *arg0, u8 *arg1);
u8 *dbase_mesh_anim_name_release(u8 *arg0, u8 *arg1);
s32 dbase_mesh_anim_name_loaded(obj_dbase *db, char *arg1);
u8 *dbase_mesh_anim_name_load(u8 *arg0, u8 *arg1);
s32 dbase_mesh_anim_name_size(u8 *arg0, u8 *arg1);
u8 *dbase_mesh_anim_release(u8 *arg0, s32 arg1);
u8 *dbase_mesh_anim_load(u8 *arg0, s32 arg1);
void dbase_setup_blocks(u8 *arg0, s32 *arg1);


#endif /* BASH_DBASE_H */
