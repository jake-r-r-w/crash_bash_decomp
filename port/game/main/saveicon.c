/* PC: the window's icon is the PS1 game's memory card icon (the user's word, 2026-11-09). That's the first of two 16x16 4-bit frames,
 * which the PS1 copied from the font's textures into a save (src/main/card.c: 0x306B and 0x306C, with the second's colours), so it's
 * made from the player's own data each time the game starts, once the font has loaded, and never kept or shipped. A download's own
 * icon (Android's, the .exe's) is a drawn one, at the user's word. */
#include "common.h"
#include "bash/main.h"
#include "bash/dbase.h"
#include "bash/modelbank.h"
#include "../../platform/platform.h"

#define ICON_TEXTURE 0x306B
#define ICON_PALETTE_TEXTURE 0x306C
#define ICON_SIZE 16

void save_icon_to_window(void) {
    obj_dbase *db = &HeroModelBank[DB_Font];
    const obj_textureset *ts = &db->textures;
    const obj_texture *t, *p;
    const u8 *pixels;
    const u16 *colours;
    uint32_t rgba[ICON_SIZE * ICON_SIZE];
    s32 x, y;

    if (ts->tex == NULL || ts->ent == NULL || ts->base == NULL || (ICON_PALETTE_TEXTURE & 0xFFF) >= ts->nTex) {
        return;
    }
    t = dbase_texture_ptr(db, ICON_TEXTURE);
    p = dbase_texture_ptr(db, ICON_PALETTE_TEXTURE);
    /* as the PS1's save did: only a 16x16 4-bit picture, with a 16-colour palette */
    if (t == NULL || p == NULL || t->w != ICON_SIZE || t->h != ICON_SIZE || t->flag != 0 || p->clut >= ts->nEnt || ts->ent[p->clut].w != 16) {
        return;
    }
    pixels = (const u8 *)ts->base + t->pix;
    colours = (const u16 *)((const u8 *)ts->base + ts->ent[p->clut].off);
    for (y = 0; y < ICON_SIZE; y++) {
        for (x = 0; x < ICON_SIZE; x++) {
            u8 two = pixels[(y * ICON_SIZE + x) / 2];
            u16 c = colours[x & 1 ? two >> 4 : two & 0xF];
            u32 r = c & 0x1F, g = (c >> 5) & 0x1F, b = (c >> 10) & 0x1F;

            /* the PS1's black with no semi-transparency bit is clear */
            rgba[y * ICON_SIZE + x] = c == 0 ? 0 : 0xFF000000u | ((b << 3 | b >> 2) << 16) | ((g << 3 | g >> 2) << 8) | (r << 3 | r >> 2);
        }
    }
    host_set_icon(rgba, ICON_SIZE, ICON_SIZE);
}
