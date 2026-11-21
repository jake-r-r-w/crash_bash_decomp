/* Mods (mods.c): zips, or folders while one's being made, of things in place of the game's own: its textures and its archive's files. */
#ifndef MODS_H
#define MODS_H

#include <stddef.h>
#include <stdint.h>

#define MOD_ID 128
#define MOD_TEXT 128
#define MOD_HERO_ID 32

typedef struct ModInfo {
    char id[MOD_ID];       /* its zip's or folder's name, which the settings know it by */
    char name[MOD_TEXT];   /* from its mod.json, or its id without .zip */
    char author[MOD_TEXT];
    char version[32];
    char description[256];
    int on;
    int textures;          /* how many textures it has in place of the game's */
    int files;             /* and archive files, */
    int heroes;            /* and heroes of its own (heroes/<id>/): with none of the three, it can't be used */
    int pack;              /* a texture pack from before mods: a folder in packs/ */
    char problem[96];      /* why it can't be used, or what of it couldn't be; empty if all's well. In capitals, for the game's font */
} ModInfo;

/* Finds the mods, in the order the settings have them and on or off as they say (new ones go first, on), and their textures take
   effect. */
void mods_scan(void);
/* The same, if what's in the mods folders has changed since: whether it had. */
int mods_rescan_if_changed(void);
int mods_count(void);
const ModInfo *mods_info(int i);
void mods_set_on(int i, int on);
/* Moves one up the list (dir -1), which puts it before the rest where two have the same thing, or down (1). */
void mods_move(int i, int dir);
/* Opens the mods folder for the player to put mods in. */
void mods_open_folder(void);
/* Whether the hero tool is beside the program (herotool/, docs/HERO_PACKS.md), and starting it, its mods folder the game's own */
int mods_hero_tool_there(void);
void mods_open_hero_tool(void);
/* The mods that are on, in their order, take effect: their textures at once, and their files for what the game reads from now on. */
void mods_apply_textures(void);
void mods_apply_files(void);
/* And online: no mod's files, which other machines mightn't have, but the heroes of their own every machine has (mods_heroes_only) */
void mods_apply_online(void);

/* Test runs, and someone making a mod: only this folder is looked in (--mods-dir); a folder of PNGs, by its path or its name in
   packs/, is a texture pack that's on (--texture-pack); a mod's on whatever the settings say (--mod). Before mods_scan. */
void mods_only_in(const char *folder);
void mods_add_pack(const char *folder);
void mods_turn_on(const char *id);

/* The textures in use, for texpack.c: whether there are any, whether one's had, its PNG's bytes to free, and leaving out one that
   wouldn't load. The generation changes whenever they do. */
int mods_textures_on(void);
int mods_texture_has(uint64_t hash);
uint8_t *mods_texture_read(uint64_t hash, size_t *size);
void mods_texture_drop(uint64_t hash);
unsigned mods_textures_generation(void);

/* The heroes of the mods that are on (heroes/<id>/, docs/HERO_PACKS.md), in the mods' order, an id only once (the higher mod's): how
   many, each one's id and its mod's, and one of its files (hero.json, hero.glb, ...) to free, up to limit bytes; NULL if it has none. */
int mods_heroes_count(void);
const char *mods_hero_id(int i);
const char *mods_hero_mod(int i);
uint8_t *mods_hero_file(int i, const char *file, size_t limit, size_t *size);

/* Online (net.c, docs/HERO_PACKS.md, Online): the heroes every machine has, played in the host's order, as places in the list above
   when nothing's restricted (mods_heroes_all), so that list is just those; or every one again (n < 0). Each of the whole list's hashed
   by its id and every file it has, so machines can tell they have the same pack (0: none there). */
#define MODS_HEROES_ONLY 4
int mods_heroes_all(void);
uint64_t mods_hero_hash(int i);
void mods_heroes_only(const int *which, int n);

#endif
