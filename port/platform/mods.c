/* Mods: zips in a mods folder beside the program (or in the user's own folder), each of things in place of the game's own, in an order
 * the player chooses in the launcher's Mods page: where two have the same thing, the one higher in the list wins. A folder is a mod
 * too, for someone making one: it's the same as its zip. The program ships none (docs/MODDING.md).
 *
 * What a mod can have, anywhere in it unless it says:
 *   mod.json         at its top: its name, author, version and description, all optional
 *   <hash>.png       a texture in place of the game's: the 16 hex digits of the hash of what it replaces (texpack.c), as
 *                    --dump-textures names them
 *   files/<NNN>.bin  an archive file in place of entry NNN, in hex, as tools/scripts/dat.py and --dump-files name them (archive.c)
 *   files/<NNN>.bps  or a patch to the game's own entry NNN (bps.c), which has only what it changes, so a mod needn't have the game's
 *                    files to change them
 * A zip with its things in one folder, as zipping a folder makes it, is read from inside that folder. Texture packs from before mods,
 * folders of PNGs in packs/, are mods too. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "bps.h"
#include "mods.h"
#include "platform.h"
#include "settings.h"
#include "zipread.h"

#define MAX_MODS SETTINGS_MODS
#define MAX_THINGS 65536
#define MAX_DEPTH 8
#define MAX_ROOTS 8
#define MAX_TURNED_ON 16
#define MAX_PNG_BYTES (64 * 1024 * 1024)
#define MAX_FILE_BYTES (32 * 1024 * 1024)
#define MAX_MANIFEST_BYTES (64 * 1024)

int overlay_is_code(int entry);
int mode_text_room(int entry);
/* game/main/bonusheroes.c: the bonus heroes' files, from the player's other discs */
void bonus_heroes_files(void);

typedef struct {
    char *name;  /* from where its things start */
    int entry;   /* in its zip, or -1 in a folder */
} Thing;

typedef struct {
    uint64_t hash;
    int thing;
} ModTexture;

typedef struct {
    int entry, thing;
    int patch;  /* a patch to the game's own, rather than the file itself */
} ModFile;

typedef struct {
    ModInfo info;
    int wanted;         /* on, as the player has it: it's only on if it can be used too */
    char path[1024];    /* its zip, or the folder its things start in, with a / at the end */
    ZipFile *zip;       /* NULL for a folder */
    Thing *things;
    int count, cap;
    ModTexture *textures;
    int num_tex, tex_cap;
    ModFile *files;
    int num_files, file_cap;
} Mod;

/* A texture in use, and which mod has it */
typedef struct {
    uint64_t hash;
    int mod, thing;
} TextureRef;

typedef struct {
    char path[1024];
    int packs;          /* a folder of texture packs from before mods, rather than of mods */
} Root;

static Mod Mods[MAX_MODS];
static int NumMods;
static Root Roots[MAX_ROOTS];
static int NumRoots, OnlyGiven, RootsFound;
static char PackFolders[MAX_ROOTS][1024];
static int NumPackFolders;
static char TurnedOn[MAX_TURNED_ON][MOD_ID];
static int NumTurnedOn;
static uint64_t Signature;

static TextureRef *Textures;
static int NumTextures;
static unsigned Generation;

/* Room for one more in an array of n things of each bytes, with room for *cap: doubled when it's full. NULL if there's no more. */
static void *room_for_one(void *array, int n, int *cap, size_t each) {
    void *p;
    int want;

    if (n < *cap) {
        return array;
    }
    want = *cap != 0 ? *cap * 2 : 16;
    p = realloc(array, each * (size_t)want);
    if (p != NULL) {
        *cap = want;
    }
    return p;
}

/* --- where mods are --- */

static void add_root(const char *path, int packs) {
    int i;

    for (i = 0; i < NumRoots; i++) {
        if (SDL_strcasecmp(Roots[i].path, path) == 0) {
            return;
        }
    }
    if (NumRoots < MAX_ROOTS) {
        SDL_strlcpy(Roots[NumRoots].path, path, sizeof(Roots[NumRoots].path));
        Roots[NumRoots++].packs = packs;
    }
}

/* The places a folder of them is kept: beside the program, in the user's own folder, and here, each with / at the end. */
static int standard_places(char places[3][1024], const char *folder) {
    const char *base = SDL_GetBasePath();
    char *pref = SDL_GetPrefPath("CrashBash", "CrashBash");
    int n = 0;

    if (base != NULL) {
        SDL_snprintf(places[n++], 1024, "%s%s/", base, folder);
    }
    if (pref != NULL) {
        SDL_snprintf(places[n++], 1024, "%s%s/", pref, folder);
        SDL_free(pref);
    }
    SDL_snprintf(places[n++], 1024, "%s/", folder);
    return n;
}

/* Where mods are looked for, unless a test run says: the mods folders, and the packs folders texture packs were kept in. */
static void default_roots(void) {
    char places[3][1024];
    int n, i;

    if (OnlyGiven || settings()->testing || settings()->no_save) {
        return;
    }
    n = standard_places(places, "mods");
    for (i = 0; i < n; i++) {
        add_root(places[i], 0);
    }
    n = standard_places(places, "packs");
    for (i = 0; i < n; i++) {
        add_root(places[i], 1);
    }
}

void mods_only_in(const char *folder) {
    char path[1024];

    OnlyGiven = 1;
    SDL_snprintf(path, sizeof(path), "%s/", folder);
    add_root(path, 0);
}

void mods_turn_on(const char *id) {
    if (NumTurnedOn < MAX_TURNED_ON) {
        SDL_strlcpy(TurnedOn[NumTurnedOn++], id, sizeof(TurnedOn[0]));
    }
}

void mods_add_pack(const char *folder) {
    char places[3][1024], path[1024];
    size_t n;
    int count, i;

    SDL_strlcpy(path, folder, sizeof(path));
    n = strlen(path);
    while (n > 1 && (path[n - 1] == '/' || path[n - 1] == '\\')) {
        path[--n] = 0;
    }
    if (strchr(path, '/') == NULL && strchr(path, '\\') == NULL) {
        count = standard_places(places, "packs");
        for (i = 0; i < count; i++) {
            char at[1024];
            SDL_PathInfo info;

            SDL_snprintf(at, sizeof(at), "%s%s", places[i], path);
            if (SDL_GetPathInfo(at, &info) && info.type == SDL_PATHTYPE_DIRECTORY) {
                SDL_strlcpy(path, at, sizeof(path));
                break;
            }
        }
    }
    if (NumPackFolders < MAX_ROOTS) {
        SDL_strlcpy(PackFolders[NumPackFolders++], path, sizeof(PackFolders[0]));
    }
    mods_turn_on(path);
}

/* --- reading what a mod has --- */

static int ends_with(const char *s, const char *end) {
    size_t a = strlen(s), b = strlen(end);

    return a >= b && SDL_strcasecmp(s + a - b, end) == 0;
}

static const char *base_name(const char *path) {
    const char *at = path, *c;

    for (c = path; *c != 0; c++) {
        if (*c == '/' || *c == '\\') {
            at = c + 1;
        }
    }
    return at;
}

static int hex_digit(char c) {
    return c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
}

/* 16 hex digits, then .png */
static int texture_hash(const char *name, uint64_t *out) {
    uint64_t v = 0;
    int i;

    if (strlen(name) != 20 || SDL_strcasecmp(name + 16, ".png") != 0) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        int d = hex_digit(name[i]);

        if (d < 0) {
            return 0;
        }
        v = v << 4 | (uint64_t)d;
    }
    *out = v;
    return 1;
}

/* An archive entry's number from its file's name: up to 4 hex digits, then a dot or the end, as dat.py names them (0A3.bin). -1 if it
   isn't one. */
static int entry_number(const char *name) {
    int v = 0, digits = 0;

    for (; *name != 0 && *name != '.'; name++) {
        int d = hex_digit(*name);

        if (d < 0 || ++digits > 4) {
            return -1;
        }
        v = v * 16 + d;
    }
    return digits != 0 && v < archive_entries() ? v : -1;
}

static int add_thing(Mod *m, const char *name, int entry) {
    size_t n = strlen(name);
    Thing *t;

    /* what a Mac puts in a zip beside what was zipped */
    if (strncmp(name, "__MACOSX/", 9) == 0) {
        return 1;
    }
    if (m->count >= MAX_THINGS || (t = room_for_one(m->things, m->count, &m->cap, sizeof(Thing))) == NULL) {
        return 0;
    }
    m->things = t;
    t[m->count].name = malloc(n + 1);
    if (t[m->count].name == NULL) {
        return 0;
    }
    memcpy(t[m->count].name, name, n + 1);
    t[m->count++].entry = entry;
    return 1;
}

typedef struct {
    Mod *mod;
    char at[1024]; /* the folder being read, from the mod's own */
    int depth;
} Walk;

static SDL_EnumerationResult SDLCALL walk_one(void *data, const char *dirname, const char *fname) {
    Walk *w = data, inner;
    SDL_PathInfo info;
    char full[2048], name[1024];

    if (fname[0] == '.') {
        return SDL_ENUM_CONTINUE;
    }
    SDL_snprintf(full, sizeof(full), "%s%s", dirname, fname);
    SDL_snprintf(name, sizeof(name), "%s%s", w->at, fname);
    if (!SDL_GetPathInfo(full, &info)) {
        return SDL_ENUM_CONTINUE;
    }
    if (info.type == SDL_PATHTYPE_DIRECTORY) {
        if (w->depth < MAX_DEPTH) {
            inner.mod = w->mod;
            inner.depth = w->depth + 1;
            SDL_snprintf(inner.at, sizeof(inner.at), "%s/", name);
            SDL_EnumerateDirectory(full, walk_one, &inner);
        }
    } else if (info.type == SDL_PATHTYPE_FILE && !add_thing(w->mod, name, -1)) {
        return SDL_ENUM_FAILURE;
    }
    return SDL_ENUM_CONTINUE;
}

/* Where a mod's things start: its top, or the one folder at its top that everything's in, as zipping a folder makes it. Not files/,
   which is a mod's own. */
static size_t things_start(const Mod *m) {
    const char *first, *slash;
    size_t length;
    int i;

    if (m->count == 0 || (slash = strchr(first = m->things[0].name, '/')) == NULL) {
        return 0;
    }
    length = (size_t)(slash - first) + 1;
    if (length == 6 && SDL_strncasecmp(first, "files/", 6) == 0) {
        return 0;
    }
    for (i = 1; i < m->count; i++) {
        if (strncmp(m->things[i].name, first, length) != 0) {
            return 0;
        }
    }
    return length;
}

/* Everything it has, from where its things start, a folder or two in as it may be: a folder mod's are read from there. */
static void strip_start(Mod *m) {
    size_t start;
    int i, depth;

    for (depth = 0; depth < MAX_DEPTH && (start = things_start(m)) != 0; depth++) {
        if (m->zip == NULL) {
            size_t at = strlen(m->path);

            if (at + start >= sizeof(m->path)) {
                return;
            }
            memcpy(m->path + at, m->things[0].name, start);
            m->path[at + start] = 0;
        }
        for (i = 0; i < m->count; i++) {
            memmove(m->things[i].name, m->things[i].name + start, strlen(m->things[i].name + start) + 1);
        }
    }
}

static uint8_t *read_file(const char *path, size_t limit, size_t *size, const char **problem) {
    SDL_IOStream *io = SDL_IOFromFile(path, "rb");
    Sint64 n;
    uint8_t *buf;

    *problem = "CAN'T BE READ";
    if (io == NULL) {
        return NULL;
    }
    n = SDL_GetIOSize(io);
    if (n < 0 || (uint64_t)n > limit) {
        *problem = "TOO BIG";
        SDL_CloseIO(io);
        return NULL;
    }
    buf = malloc((size_t)n + 1);
    if (buf != NULL && SDL_ReadIO(io, buf, (size_t)n) != (size_t)n) {
        free(buf);
        buf = NULL;
    }
    SDL_CloseIO(io);
    if (buf != NULL) {
        *size = (size_t)n;
        *problem = NULL;
    }
    return buf;
}

/* The bytes of one of a mod's things, to free. */
static uint8_t *read_thing(const Mod *m, int thing, size_t limit, size_t *size, const char **problem) {
    char path[2048];

    if (m->zip != NULL) {
        return zip_read(m->zip, m->things[thing].entry, limit, size, problem);
    }
    SDL_snprintf(path, sizeof(path), "%s%s", m->path, m->things[thing].name);
    return read_file(path, limit, size, problem);
}

/* --- mod.json: an object with strings in it, the rest passed over --- */

typedef struct {
    const char *at, *end;
} Json;

static void json_space(Json *j) {
    while (j->at < j->end && (*j->at == ' ' || *j->at == '\t' || *j->at == '\r' || *j->at == '\n')) {
        j->at++;
    }
}

/* A string into out, if out isn't NULL, with what it can't keep as a space. Whether there was a string. */
static int json_string(Json *j, char *out, size_t size) {
    size_t n = 0;

    if (j->at >= j->end || *j->at != '"') {
        return 0;
    }
    for (j->at++; j->at < j->end && *j->at != '"'; j->at++) {
        char c = *j->at;

        if (c == '\\' && j->at + 1 < j->end) {
            j->at++;
            c = *j->at == '"' || *j->at == '\\' || *j->at == '/' ? *j->at : ' ';
            if (*j->at == 'u') {
                j->at = j->at + 4 < j->end ? j->at + 4 : j->end - 1;
            }
        }
        if ((unsigned char)c < 0x20) {
            c = ' ';
        }
        if (out != NULL && n + 1 < size) {
            out[n++] = c;
        }
    }
    if (out != NULL && size != 0) {
        out[n] = 0;
    }
    if (j->at >= j->end) {
        return 0;
    }
    j->at++;
    return 1;
}

/* Any value but a string: a number, true, false, null, or an object or array, whatever's in it. */
static int json_skip(Json *j) {
    int depth = 0;

    do {
        json_space(j);
        if (j->at >= j->end) {
            return 0;
        }
        if (*j->at == '"') {
            if (!json_string(j, NULL, 0)) {
                return 0;
            }
            continue;
        }
        if (*j->at == '{' || *j->at == '[') {
            depth++;
        } else if (*j->at == '}' || *j->at == ']') {
            if (depth == 0) {
                return 1;
            }
            depth--;
        } else if (*j->at == ',' && depth == 0) {
            return 1;
        }
        j->at++;
    } while (depth > 0 || (j->at < j->end && *j->at != ',' && *j->at != '}' && *j->at != ']'));
    return 1;
}

static void read_manifest(const char *text, size_t n, ModInfo *info) {
    Json j;

    j.at = text;
    j.end = text + n;
    /* a byte order mark, as some editors write */
    if (n >= 3 && (unsigned char)text[0] == 0xEF && (unsigned char)text[1] == 0xBB && (unsigned char)text[2] == 0xBF) {
        j.at += 3;
    }
    json_space(&j);
    if (j.at >= j.end || *j.at++ != '{') {
        return;
    }
    for (;;) {
        char key[32], value[256];
        char *into = NULL;
        size_t size = 0;

        json_space(&j);
        if (!json_string(&j, key, sizeof(key))) {
            return;
        }
        json_space(&j);
        if (j.at >= j.end || *j.at++ != ':') {
            return;
        }
        json_space(&j);
        if (SDL_strcasecmp(key, "name") == 0) {
            into = info->name;
            size = sizeof(info->name);
        } else if (SDL_strcasecmp(key, "author") == 0) {
            into = info->author;
            size = sizeof(info->author);
        } else if (SDL_strcasecmp(key, "version") == 0) {
            into = info->version;
            size = sizeof(info->version);
        } else if (SDL_strcasecmp(key, "description") == 0) {
            into = info->description;
            size = sizeof(info->description);
        }
        if (j.at < j.end && *j.at == '"') {
            if (!json_string(&j, value, sizeof(value))) {
                return;
            }
            if (into != NULL && value[0] != 0) {
                SDL_strlcpy(into, value, size);
            }
        } else if (!json_skip(&j)) {
            return;
        }
        json_space(&j);
        if (j.at >= j.end || *j.at != ',') {
            return;
        }
        j.at++;
    }
}

static void set_problem(Mod *m, const char *problem) {
    if (m->info.problem[0] == 0) {
        SDL_strlcpy(m->info.problem, problem, sizeof(m->info.problem));
    }
}

static void add_texture(Mod *m, uint64_t hash, int thing) {
    ModTexture *t = room_for_one(m->textures, m->num_tex, &m->tex_cap, sizeof(ModTexture));

    if (t != NULL) {
        m->textures = t;
        t[m->num_tex].hash = hash;
        t[m->num_tex++].thing = thing;
    }
}

static void add_file(Mod *m, int entry, int thing, int patch) {
    ModFile *f = room_for_one(m->files, m->num_files, &m->file_cap, sizeof(ModFile));

    if (f != NULL) {
        m->files = f;
        f[m->num_files].entry = entry;
        f[m->num_files].patch = patch;
        f[m->num_files++].thing = thing;
    }
}

/* What it has: its mod.json, its textures and its files, the first of each entry's. */
static void sort_things(Mod *m) {
    unsigned char *seen = calloc((size_t)archive_entries() + 1, 1);
    int i;

    for (i = 0; i < m->count; i++) {
        const char *name = m->things[i].name, *base = base_name(name);
        uint64_t hash;
        int entry;

        if (SDL_strcasecmp(name, "mod.json") == 0) {
            const char *problem;
            size_t size;
            uint8_t *text = read_thing(m, i, MAX_MANIFEST_BYTES, &size, &problem);

            if (text != NULL) {
                read_manifest((const char *)text, size, &m->info);
                free(text);
            } else {
                set_problem(m, "ITS MOD.JSON CAN'T BE READ");
            }
        } else if (SDL_strncasecmp(name, "files/", 6) == 0) {
            entry = entry_number(base);
            if (entry < 0 || seen == NULL || seen[entry]) {
                continue;
            }
            seen[entry] = 1;
            if (overlay_is_code(entry)) {
                set_problem(m, "ITS CODE IS LEFT OUT: THE PC GAME HAS ITS OWN");
                continue;
            }
            add_file(m, entry, i, ends_with(base, ".bps"));
        } else if (texture_hash(base, &hash)) {
            add_texture(m, hash, i);
        }
    }
    free(seen);
    m->info.textures = m->num_tex;
    m->info.files = m->num_files;
}

static int usable(const Mod *m) {
    return m->num_tex != 0 || m->num_files != 0;
}

static void free_mod(Mod *m) {
    int i;

    for (i = 0; i < m->count; i++) {
        free(m->things[i].name);
    }
    free(m->things);
    free(m->textures);
    free(m->files);
    zip_close(m->zip);
    memset(m, 0, sizeof(*m));
}

/* A mod from a zip or a folder at path, known as id. */
static void load_mod(Mod *m, const char *path, const char *id, int folder, int pack) {
    memset(m, 0, sizeof(*m));
    SDL_strlcpy(m->info.id, id, sizeof(m->info.id));
    SDL_strlcpy(m->info.name, base_name(id), sizeof(m->info.name));
    if (!folder && ends_with(m->info.name, ".zip")) {
        m->info.name[strlen(m->info.name) - 4] = 0;
    }
    m->info.pack = pack;
    if (folder) {
        Walk w;

        SDL_snprintf(m->path, sizeof(m->path), "%s/", path);
        w.mod = m;
        w.at[0] = 0;
        w.depth = 0;
        SDL_EnumerateDirectory(m->path, walk_one, &w);
    } else {
        const char *problem;
        int i;

        SDL_strlcpy(m->path, path, sizeof(m->path));
        m->zip = zip_open(path, &problem);
        if (m->zip == NULL) {
            set_problem(m, problem);
            return;
        }
        for (i = 0; i < m->zip->count; i++) {
            add_thing(m, m->zip->entries[i].name, i);
        }
    }
    strip_start(m);
    sort_things(m);
    if (!usable(m)) {
        set_problem(m, "NO TEXTURES OR FILES IN IT");
    }
}

static int have_mod(const char *id) {
    int i;

    for (i = 0; i < NumMods; i++) {
        if (SDL_strcasecmp(Mods[i].info.id, id) == 0) {
            return 1;
        }
    }
    return 0;
}

/* One of a root's: a zip, or a folder. The first root a name is found in has it. */
static SDL_EnumerationResult SDLCALL look_one(void *data, const char *dirname, const char *fname) {
    const Root *root = data;
    SDL_PathInfo info;
    char path[2048];
    int folder;

    if (fname[0] == '.' || NumMods >= MAX_MODS || have_mod(fname)) {
        return SDL_ENUM_CONTINUE;
    }
    SDL_snprintf(path, sizeof(path), "%s%s", dirname, fname);
    if (!SDL_GetPathInfo(path, &info)) {
        return SDL_ENUM_CONTINUE;
    }
    folder = info.type == SDL_PATHTYPE_DIRECTORY;
    if (folder || (!root->packs && info.type == SDL_PATHTYPE_FILE && ends_with(fname, ".zip"))) {
        load_mod(&Mods[NumMods++], path, fname, folder, root->packs);
    }
    return SDL_ENUM_CONTINUE;
}

/* What's at the top of each root, by name, size, time and kind: summed, so the order they come in doesn't matter. */
static SDL_EnumerationResult SDLCALL sign_one(void *data, const char *dirname, const char *fname) {
    uint64_t *sum = data, h = 0xCBF29CE484222325ULL;
    SDL_PathInfo info;
    char path[2048];
    const char *c;

    SDL_snprintf(path, sizeof(path), "%s%s", dirname, fname);
    if (!SDL_GetPathInfo(path, &info)) {
        return SDL_ENUM_CONTINUE;
    }
    for (c = fname; *c != 0; c++) {
        h = (h ^ (unsigned char)*c) * 0x100000001B3ULL;
    }
    h = (h ^ info.size) * 0x100000001B3ULL;
    h = (h ^ (uint64_t)info.modify_time) * 0x100000001B3ULL;
    h = (h ^ (uint64_t)info.type) * 0x100000001B3ULL;
    *sum += h;
    return SDL_ENUM_CONTINUE;
}

static uint64_t folders_signature(void) {
    uint64_t sum = 0;
    int i;

    for (i = 0; i < NumRoots; i++) {
        SDL_EnumerateDirectory(Roots[i].path, sign_one, &sum);
    }
    return sum;
}

static int by_id(const void *a, const void *b) {
    return SDL_strcasecmp(((const Mod *)a)->info.id, ((const Mod *)b)->info.id);
}

/* --- the list --- */

static void remember_order(void) {
    Settings *s = settings();
    int i;

    s->num_mods = 0;
    for (i = 0; i < NumMods && i < SETTINGS_MODS; i++) {
        SDL_strlcpy(s->mods[i].id, Mods[i].info.id, sizeof(s->mods[i].id));
        s->mods[i].on = Mods[i].wanted;
        s->num_mods++;
    }
}

static void set_wanted(Mod *m, int on) {
    m->wanted = on;
    m->info.on = on && usable(m);
}

void mods_scan(void) {
    static Mod found[MAX_MODS];
    int placed[MAX_MODS];
    Settings *s = settings();
    int fresh = !(s->testing || s->no_save), num_found, i, j, n = 0;

    for (i = 0; i < NumMods; i++) {
        free_mod(&Mods[i]);
    }
    NumMods = 0;
    if (!RootsFound) {
        RootsFound = 1;
        default_roots();
    }
    for (i = 0; i < NumRoots; i++) {
        SDL_EnumerateDirectory(Roots[i].path, look_one, &Roots[i]);
    }
    for (i = 0; i < NumPackFolders && NumMods < MAX_MODS; i++) {
        if (!have_mod(PackFolders[i])) {
            load_mod(&Mods[NumMods++], PackFolders[i], PackFolders[i], 1, 1);
        }
    }
    Signature = folders_signature();
    /* before mods, a texture pack was chosen by name: it's a mod that's on */
    if (s->texture_pack[0] != 0 && s->num_mods == 0) {
        SDL_strlcpy(s->mods[0].id, s->texture_pack, sizeof(s->mods[0].id));
        s->mods[0].on = 1;
        s->num_mods = 1;
    }
    s->texture_pack[0] = 0;
    /* the order: new ones first, by name, and on in the player's own runs, unless they're from packs/, where one was chosen before;
       then the rest in the settings' order */
    num_found = NumMods;
    memcpy(found, Mods, sizeof(Mod) * (size_t)num_found);
    qsort(found, (size_t)num_found, sizeof(Mod), by_id);
    memset(placed, 0, sizeof(placed));
    for (i = 0; i < num_found; i++) {
        int known = 0;

        for (j = 0; j < s->num_mods; j++) {
            known |= SDL_strcasecmp(s->mods[j].id, found[i].info.id) == 0;
        }
        if (!known) {
            Mods[n] = found[i];
            set_wanted(&Mods[n++], fresh && !found[i].info.pack);
            placed[i] = 1;
        }
    }
    for (j = 0; j < s->num_mods; j++) {
        for (i = 0; i < num_found; i++) {
            if (!placed[i] && SDL_strcasecmp(s->mods[j].id, found[i].info.id) == 0) {
                Mods[n] = found[i];
                set_wanted(&Mods[n++], s->mods[j].on);
                placed[i] = 1;
            }
        }
    }
    NumMods = n;
    for (i = 0; i < NumMods; i++) {
        for (j = 0; j < NumTurnedOn; j++) {
            if (SDL_strcasecmp(Mods[i].info.id, TurnedOn[j]) == 0) {
                set_wanted(&Mods[i], 1);
            }
        }
        printf("mod %s (%s%s%s): %s, %d textures, %d files%s%s\n", Mods[i].info.id, Mods[i].info.name, Mods[i].info.author[0] != 0 ? " by " : "",
               Mods[i].info.author, Mods[i].info.on ? "on" : "off", Mods[i].info.textures, Mods[i].info.files,
               Mods[i].info.problem[0] != 0 ? ", " : "", Mods[i].info.problem);
    }
    remember_order();
    mods_apply_textures();
}

int mods_rescan_if_changed(void) {
    if (!RootsFound || folders_signature() == Signature) {
        return 0;
    }
    mods_scan();
    return 1;
}

int mods_count(void) {
    return NumMods;
}

const ModInfo *mods_info(int i) {
    return i >= 0 && i < NumMods ? &Mods[i].info : NULL;
}

void mods_set_on(int i, int on) {
    if (i >= 0 && i < NumMods) {
        set_wanted(&Mods[i], on);
        remember_order();
    }
}

void mods_move(int i, int dir) {
    int j = i + dir;
    Mod t;

    if (i < 0 || i >= NumMods || j < 0 || j >= NumMods) {
        return;
    }
    t = Mods[i];
    Mods[i] = Mods[j];
    Mods[j] = t;
    remember_order();
}

/* The one beside the program, or the one in the user's own folder if that can't be made there. */
void mods_open_folder(void) {
    char places[3][1024], url[3200];
    const char *path = NULL;
    int n = standard_places(places, "mods"), i;

    for (i = 0; i < n - 1 && path == NULL; i++) {
        SDL_PathInfo info;

        if ((SDL_GetPathInfo(places[i], &info) && info.type == SDL_PATHTYPE_DIRECTORY) || SDL_CreateDirectory(places[i])) {
            path = places[i];
        }
    }
    if (path == NULL) {
        return;
    }
#ifdef _WIN32
    /* Windows opens a folder from its path, whatever's in its name */
    SDL_strlcpy(url, path, sizeof(url));
    for (i = 0; url[i] != 0; i++) {
        url[i] = url[i] == '/' ? '\\' : url[i];
    }
#else
    {
        char *out = url + SDL_snprintf(url, sizeof(url), "file://");
        const char *c;

        for (c = path; *c != 0 && out < url + sizeof(url) - 4; c++) {
            unsigned char ch = (unsigned char)*c;

            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || strchr("/-_.~", ch) != NULL) {
                *out++ = (char)ch;
            } else {
                out += SDL_snprintf(out, 4, "%%%02X", ch);
            }
        }
        *out = 0;
    }
#endif
    if (!SDL_OpenURL(url)) {
        fprintf(stderr, "mods folder %s: %s\n", path, SDL_GetError());
    }
}

/* --- textures --- */

static int by_hash_then_order(const void *a, const void *b) {
    const TextureRef *x = a, *y = b;

    if (x->hash != y->hash) {
        return x->hash < y->hash ? -1 : 1;
    }
    return x->mod != y->mod ? x->mod - y->mod : x->thing - y->thing;
}

static int by_hash(const void *key, const void *ref) {
    uint64_t h = *(const uint64_t *)key, r = ((const TextureRef *)ref)->hash;

    return h < r ? -1 : h > r;
}

void mods_apply_textures(void) {
    int total = 0, i, k, n = 0;

    free(Textures);
    Textures = NULL;
    NumTextures = 0;
    for (i = 0; i < NumMods; i++) {
        total += Mods[i].info.on ? Mods[i].num_tex : 0;
    }
    Textures = total != 0 ? malloc(sizeof(TextureRef) * (size_t)total) : NULL;
    for (i = 0; Textures != NULL && i < NumMods; i++) {
        for (k = 0; Mods[i].info.on && k < Mods[i].num_tex; k++) {
            Textures[n].hash = Mods[i].textures[k].hash;
            Textures[n].mod = i;
            Textures[n++].thing = Mods[i].textures[k].thing;
        }
    }
    if (n != 0) {
        qsort(Textures, (size_t)n, sizeof(TextureRef), by_hash_then_order);
    }
    /* where two have the same texture, the higher in the list */
    for (i = 0; i < n; i++) {
        if (NumTextures == 0 || Textures[NumTextures - 1].hash != Textures[i].hash) {
            Textures[NumTextures++] = Textures[i];
        }
    }
    Generation++;
}

int mods_textures_on(void) {
    return NumTextures != 0;
}

static TextureRef *find_texture(uint64_t hash) {
    return NumTextures != 0 ? bsearch(&hash, Textures, (size_t)NumTextures, sizeof(TextureRef), by_hash) : NULL;
}

int mods_texture_has(uint64_t hash) {
    return find_texture(hash) != NULL;
}

uint8_t *mods_texture_read(uint64_t hash, size_t *size) {
    const TextureRef *r = find_texture(hash);
    const char *problem;
    uint8_t *bytes;

    if (r == NULL) {
        return NULL;
    }
    bytes = read_thing(&Mods[r->mod], r->thing, MAX_PNG_BYTES, size, &problem);
    if (bytes == NULL) {
        fprintf(stderr, "mod %s: %s: %s\n", Mods[r->mod].info.id, Mods[r->mod].things[r->thing].name, problem);
    }
    return bytes;
}

void mods_texture_drop(uint64_t hash) {
    TextureRef *at = find_texture(hash);

    if (at != NULL) {
        memmove(at, at + 1, sizeof(TextureRef) * (size_t)(Textures + NumTextures - at - 1));
        NumTextures--;
    }
}

unsigned mods_textures_generation(void) {
    return Generation;
}

/* --- archive files --- */

void mods_apply_files(void) {
    int entries = archive_entries(), e, i, k, replaced = 0;
    int *from = malloc(sizeof(int) * (size_t)entries * 2);

    archive_replace_none();
    /* the bonus heroes' first, so a mod's can go over them */
    bonus_heroes_files();
    for (e = 0; from != NULL && e < entries * 2; e++) {
        from[e] = -1;
    }
    /* the lowest in the list first, so the higher ones' files go over theirs: each entry's mod, and which of its files */
    for (i = NumMods - 1; from != NULL && i >= 0; i--) {
        for (k = 0; Mods[i].info.on && k < Mods[i].num_files; k++) {
            from[Mods[i].files[k].entry * 2] = i;
            from[Mods[i].files[k].entry * 2 + 1] = k;
        }
    }
    for (e = 0; from != NULL && e < entries; e++) {
        Mod *m = from[e * 2] >= 0 ? &Mods[from[e * 2]] : NULL;
        const ModFile *f = m != NULL ? &m->files[from[e * 2 + 1]] : NULL;
        int room = mode_text_room(e);
        size_t limit = room != 0 ? (size_t)room : MAX_FILE_BYTES, size;
        const char *problem;
        uint8_t *bytes;

        if (m == NULL) {
            continue;
        }
        bytes = read_thing(m, f->thing, f->patch ? MAX_FILE_BYTES : limit, &size, &problem);
        if (bytes != NULL && f->patch) {
            int own_size = 0;
            uint8_t *own = archive_own_file(e, &own_size), *made = NULL;

            problem = "THE GAME'S OWN CAN'T BE READ";
            if (own != NULL) {
                made = bps_apply(bytes, size, own, (size_t)own_size, limit, &size, &problem);
                free(own);
            }
            free(bytes);
            bytes = made;
        }
        if (bytes == NULL) {
            fprintf(stderr, "mod %s: %s: %s\n", m->info.id, m->things[f->thing].name, problem);
            set_problem(m, f->patch && problem != NULL && strcmp(problem, "FOR ANOTHER FILE") == 0 ? "A PATCH IN IT IS FOR OTHER FILES THAN YOURS"
                           : room != 0 && problem != NULL && strcmp(problem, "TOO BIG") == 0 ? "A TEXT FILE IN IT IS TOO BIG FOR THE GAME"
                           : f->patch ? "A PATCH IN IT CAN'T BE USED"
                                      : "A FILE IN IT CAN'T BE READ");
            continue;
        }
        archive_replace(e, bytes, (int)size);
        replaced++;
    }
    free(from);
    archive_lay_out();
    if (replaced != 0) {
        printf("mods: %d of the archive's files replaced\n", replaced);
    }
}
