/* Pulls the game's archive out of the player's own disc image, for the first time it runs: a .cue with its .bin, a raw .bin or an .iso. Only
 * the one file is read, by the disc's own ISO 9660 directories. SDL's file calls, so a path can hold any characters. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "platform.h"

#define SECTOR_BYTES 2048
#define RAW_SECTOR_BYTES 2352
#define VOLUME_SECTOR 16

typedef struct {
    SDL_IOStream *io;
    int stride;   /* bytes a sector takes up in the file */
    int skip;     /* bytes before its data */
} Disc;

typedef struct {
    uint32_t sector, size;
} DiscFile;

static int read_at(Disc *d, Sint64 at, void *buf, size_t size) {
    return SDL_SeekIO(d->io, at, SDL_IO_SEEK_SET) == at && SDL_ReadIO(d->io, buf, size) == size;
}

static int read_sector(Disc *d, uint32_t sector, void *buf) {
    return read_at(d, (Sint64)sector * d->stride + d->skip, buf, SECTOR_BYTES);
}

static uint32_t le32(const uint8_t *p) {
    return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24);
}

/* Whether a directory record's name is this one, going without the version after its semicolon. */
static int same_name(const uint8_t *name, int len, const char *want) {
    int i;

    for (i = 0; i < len && name[i] != ';'; i++) {
        if (want[i] == 0 || SDL_toupper(name[i]) != want[i]) {
            return 0;
        }
    }
    return want[i] == 0;
}

static int find_in(Disc *d, const DiscFile *dir, const char *name, DiscFile *out) {
    uint8_t sector[SECTOR_BYTES];
    uint32_t done;

    for (done = 0; done < dir->size; done += SECTOR_BYTES) {
        int at = 0;

        if (!read_sector(d, dir->sector + done / SECTOR_BYTES, sector)) {
            return 0;
        }
        /* a record doesn't cross a sector: a length of 0 is the rest of this one unused */
        while (at + 33 < SECTOR_BYTES && sector[at] != 0) {
            const uint8_t *rec = sector + at;

            if (same_name(rec + 33, rec[32], name)) {
                out->sector = le32(rec + 2);
                out->size = le32(rec + 10);
                return 1;
            }
            at += rec[0];
        }
    }
    return 0;
}

/* The shape of the file: raw sectors with their headers, or just the data. */
static int open_disc(Disc *d) {
    static const uint8_t Sync[12] = { 0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0 };
    uint8_t head[SECTOR_BYTES]; /* a whole sector: read_sector fills one */

    if (read_at(d, (Sint64)VOLUME_SECTOR * RAW_SECTOR_BYTES, head, sizeof(head)) && memcmp(head, Sync, sizeof(Sync)) == 0) {
        d->stride = RAW_SECTOR_BYTES;
        d->skip = head[15] == 2 ? 24 : 16;
    } else {
        d->stride = SECTOR_BYTES;
        d->skip = 0;
    }
    return read_sector(d, VOLUME_SECTOR, head) && memcmp(head + 1, "CD001", 5) == 0;
}

/* A .cue names the file that holds the disc, next to itself. Returns what to open. */
static char *image_file(const char *image) {
    size_t len = strlen(image), size = 0;
    char *text, *at, *end, *dup;
    const char *slash;

    if (len < 4 || SDL_strcasecmp(image + len - 4, ".cue") != 0) {
        return SDL_strdup(image);
    }
    text = SDL_LoadFile(image, &size);
    if (text == NULL) {
        return NULL;
    }
    at = SDL_strcasestr(text, "FILE");
    at = at != NULL ? strchr(at, '"') : NULL;
    end = at != NULL ? strchr(at + 1, '"') : NULL;
    if (end == NULL) {
        SDL_free(text);
        return NULL;
    }
    *end = 0;
    slash = SDL_strrchr(image, '/');
    if (SDL_strrchr(image, '\\') > slash) {
        slash = SDL_strrchr(image, '\\');
    }
    if (SDL_asprintf(&dup, "%.*s%s", slash != NULL ? (int)(slash + 1 - image) : 0, image, at + 1) < 0) {
        dup = NULL;
    }
    /* Windows finds it whatever the case of its name in the cue sheet, so cue sheets don't always have it right: elsewhere, look for it so */
    if (dup != NULL && !SDL_GetPathInfo(dup, NULL)) {
        char *folder = SDL_strndup(image, slash != NULL ? (size_t)(slash + 1 - image) : 0);
        int count = 0;
        char **names = folder != NULL ? SDL_GlobDirectory(folder[0] != 0 ? folder : "./", at + 1, SDL_GLOB_CASEINSENSITIVE, &count) : NULL;

        if (names != NULL && count > 0) {
            SDL_free(dup);
            if (SDL_asprintf(&dup, "%s%s", folder, names[0]) < 0) {
                dup = NULL;
            }
        }
        SDL_free(names);
        SDL_free(folder);
    }
    SDL_free(text);
    return dup;
}

const char *disc_extract(const char *image, const char *out) {
    uint8_t volume[SECTOR_BYTES], buf[SECTOR_BYTES];
    const char *problem = NULL;
    char *file = image_file(image);
    char *temp = NULL;
    SDL_IOStream *to = NULL;
    Disc d;
    DiscFile root, folder, dat, exe;
    uint32_t done;

    if (file == NULL) {
        return "That cue sheet can't be read, or doesn't say where its disc is.";
    }
    d.io = SDL_IOFromFile(file, "rb");
    SDL_free(file);
    if (d.io == NULL) {
        return "That file can't be opened. If it's a .cue, its .bin needs to be next to it.";
    }
    if (!open_disc(&d) || !read_sector(&d, VOLUME_SECTOR, volume)) {
        problem = "That doesn't look like a disc image this can read. It takes a .cue with its .bin, a raw .bin or an .iso.";
        goto done;
    }
    root.sector = le32(volume + 156 + 2);
    root.size = le32(volume + 156 + 10);
    if (!find_in(&d, &root, "CRASHBSH", &folder) || !find_in(&d, &folder, "CRASHBSH.DAT", &dat)) {
        problem = "That disc isn't Crash Bash: it has no CRASHBSH.DAT.";
        goto done;
    }
    if (!find_in(&d, &root, "SCUS_945.70", &exe)) {
        problem = "That's another version of Crash Bash. Only the USA version's data is supported.";
        goto done;
    }
    /* a stopped copy mustn't look like the data next time */
    if (SDL_asprintf(&temp, "%s.part", out) < 0) {
        temp = NULL;
    }
    to = temp != NULL ? SDL_IOFromFile(temp, "wb") : NULL;
    if (to == NULL) {
        problem = "The data can't be written. Is the disk full?";
        goto done;
    }
    for (done = 0; done < dat.size; done += SECTOR_BYTES) {
        size_t want = dat.size - done < SECTOR_BYTES ? dat.size - done : SECTOR_BYTES;

        if (!read_sector(&d, dat.sector + done / SECTOR_BYTES, buf)) {
            problem = "The disc image ends early: it may be damaged.";
            goto done;
        }
        if (SDL_WriteIO(to, buf, want) != want) {
            problem = "The data can't be written. Is the disk full?";
            goto done;
        }
    }
    if (!SDL_CloseIO(to)) {
        problem = "The data can't be written. Is the disk full?";
    }
    to = NULL;
    if (problem == NULL && !SDL_RenamePath(temp, out)) {
        problem = "The data can't be put in place.";
    }
done:
    if (to != NULL) {
        SDL_CloseIO(to);
    }
    if (temp != NULL) {
        if (problem != NULL) {
            SDL_RemovePath(temp);
        }
        SDL_free(temp);
    }
    SDL_CloseIO(d.io);
    return problem;
}

/* --- the player's other discs, for bonus content --- */

struct DiscImage {
    Disc d;
    DiscFile root;
    char volume[33];
};

DiscImage *disc_image_open(const char *image) {
    uint8_t volume[SECTOR_BYTES];
    char *file = image_file(image);
    DiscImage *di = file != NULL ? SDL_calloc(1, sizeof(DiscImage)) : NULL;
    int i;

    if (di == NULL) {
        SDL_free(file);
        return NULL;
    }
    di->d.io = SDL_IOFromFile(file, "rb");
    SDL_free(file);
    if (di->d.io == NULL || !open_disc(&di->d) || !read_sector(&di->d, VOLUME_SECTOR, volume)) {
        disc_image_close(di);
        return NULL;
    }
    di->root.sector = le32(volume + 156 + 2);
    di->root.size = le32(volume + 156 + 10);
    memcpy(di->volume, volume + 40, 32);
    for (i = 31; i >= 0 && di->volume[i] == ' '; i--) {
        di->volume[i] = 0;
    }
    return di;
}

const char *disc_image_volume(const DiscImage *di) {
    return di->volume;
}

void *disc_image_read(DiscImage *di, const char *path, size_t *size) {
    DiscFile at = di->root, next;
    char part[64];
    uint8_t *buf;
    uint32_t done;

    /* down its folders, a name at a time */
    while (*path != 0) {
        size_t n = strcspn(path, "/");

        if (n == 0 || n >= sizeof(part)) {
            return NULL;
        }
        memcpy(part, path, n);
        part[n] = 0;
        if (!find_in(&di->d, &at, part, &next)) {
            return NULL;
        }
        at = next;
        path += n + (path[n] == '/');
    }
    buf = SDL_malloc((size_t)at.size + SECTOR_BYTES);
    for (done = 0; buf != NULL && done < at.size; done += SECTOR_BYTES) {
        if (!read_sector(&di->d, at.sector + done / SECTOR_BYTES, buf + done)) {
            SDL_free(buf);
            return NULL;
        }
    }
    if (buf != NULL) {
        *size = at.size;
    }
    return buf;
}

void disc_image_close(DiscImage *di) {
    if (di != NULL) {
        if (di->d.io != NULL) {
            SDL_CloseIO(di->d.io);
        }
        SDL_free(di);
    }
}

/* In one folder and the folders in it, the first image that's that volume's. */
static DiscImage *find_volume_in(const char *folder, const char *volume, int depth) {
    static const char *const Patterns[] = { "*.cue", "*.iso", "*.bin", "*.img" };
    DiscImage *found = NULL;
    size_t p;
    int count = 0, i;
    char **names;

    for (p = 0; found == NULL && p < SDL_arraysize(Patterns); p++) {
        names = SDL_GlobDirectory(folder, Patterns[p], SDL_GLOB_CASEINSENSITIVE, &count);
        for (i = 0; found == NULL && names != NULL && i < count; i++) {
            char *path;

            if (SDL_asprintf(&path, "%s%s", folder, names[i]) >= 0) {
                DiscImage *di = disc_image_open(path);

                if (di != NULL && SDL_strcmp(di->volume, volume) == 0) {
                    found = di;
                } else {
                    disc_image_close(di);
                }
                SDL_free(path);
            }
        }
        SDL_free(names);
    }
    /* a disc in a folder of its own */
    names = found == NULL && depth > 0 ? SDL_GlobDirectory(folder, "*", 0, &count) : NULL;
    for (i = 0; found == NULL && names != NULL && i < count; i++) {
        char *path;
        SDL_PathInfo info;

        if (SDL_asprintf(&path, "%s%s", folder, names[i]) >= 0) {
            if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY) {
                char *inside;

                if (SDL_asprintf(&inside, "%s/", path) >= 0) {
                    found = find_volume_in(inside, volume, depth - 1);
                    SDL_free(inside);
                }
            }
            SDL_free(path);
        }
    }
    SDL_free(names);
    return found;
}

DiscImage *disc_image_find(const char *volume) {
    const char *base = platform_base_path();
    char *pref = SDL_GetPrefPath("CrashBash", "CrashBash");
    char places[9][1024];
    int depth[9];
    DiscImage *found = NULL;
    int n = 0, i;

    /* a discs folder beside the program, in the saves folder or where it's run from: an image in it, or in a folder of its own there */
    if (base != NULL) {
        depth[n] = 1;
        SDL_snprintf(places[n++], sizeof(places[0]), "%sdiscs/", base);
    }
    if (pref != NULL) {
        depth[n] = 1;
        SDL_snprintf(places[n++], sizeof(places[0]), "%sdiscs/", pref);
        SDL_free(pref);
    }
    depth[n] = 1;
    SDL_snprintf(places[n++], sizeof(places[0]), "discs/");
    /* or loose beside the program, as the game's own disc is the first time it runs (disc_find_beside), or where it's run from */
    if (base != NULL) {
        depth[n] = 0;
        SDL_snprintf(places[n++], sizeof(places[0]), "%s", base);
    }
    depth[n] = 0;
    SDL_snprintf(places[n++], sizeof(places[0]), "./");
    /* where a working copy of the repository keeps its discs: from where it's run, or above the program (build/port/windows-clang/),
       which is where it's run from when it's started from its own folder */
    depth[n] = 1;
    SDL_snprintf(places[n++], sizeof(places[0]), "disc/");
    if (base != NULL) {
        depth[n] = depth[n + 1] = depth[n + 2] = 1;
        SDL_snprintf(places[n++], sizeof(places[0]), "%s../disc/", base);
        SDL_snprintf(places[n++], sizeof(places[0]), "%s../../disc/", base);
        SDL_snprintf(places[n++], sizeof(places[0]), "%s../../../disc/", base);
    }
    for (i = 0; found == NULL && i < n; i++) {
        found = find_volume_in(places[i], volume, depth[i]);
    }
    return found;
}

/* The file dialog answers from its own thread. NoDialog: there wasn't one to show (on Linux it needs a desktop portal or zenity). */
static SDL_AtomicInt Answered;
static char *Chosen;
static int NoDialog;

static void SDLCALL file_chosen(void *unused, const char *const *files, int filter) {
    (void)unused;
    (void)filter;
    Chosen = files != NULL && files[0] != NULL ? SDL_strdup(files[0]) : NULL;
    if (files == NULL) {
        fprintf(stderr, "No file dialog: %s\n", SDL_GetError());
        NoDialog = 1;
    }
    SDL_SetAtomicInt(&Answered, 1);
}

static char *ask_for_image(void) {
    static const SDL_DialogFileFilter Filters[] = {
        { "Disc images (.cue, .iso, .bin, .img)", "cue;iso;bin;img" },
        { "All files", "*" },
    };

    SDL_SetAtomicInt(&Answered, 0);
    Chosen = NULL;
#ifdef SDL_PLATFORM_ANDROID
    /* Android's chooser filters by type, and has no type for a disc image's .bin, so it offers every file */
    (void)Filters;
    SDL_ShowOpenFileDialog(file_chosen, NULL, NULL, NULL, 0, NULL, false);
#else
    SDL_ShowOpenFileDialog(file_chosen, NULL, NULL, Filters, SDL_arraysize(Filters), NULL, false);
#endif
    while (!SDL_GetAtomicInt(&Answered)) {
        SDL_PumpEvents();
        SDL_Delay(10);
    }
    return Chosen;
}

/* The first image in this folder that's the right disc: a .cue before the .bin it names. Returns 1 with the data in place. Otherwise
   problem says what was wrong with the last one tried, if there was one. */
static int try_folder(const char *folder, const char *out, const char **problem) {
    static const char *const Patterns[] = { "*.cue", "*.iso", "*.bin", "*.img" };
    size_t p;

    for (p = 0; p < SDL_arraysize(Patterns); p++) {
        int count = 0, i;
        char **names = SDL_GlobDirectory(folder, Patterns[p], SDL_GLOB_CASEINSENSITIVE, &count);

        for (i = 0; names != NULL && i < count; i++) {
            char *path;

            if (SDL_asprintf(&path, "%s%s", folder, names[i]) >= 0) {
                const char *wrong = disc_extract(path, out);

                SDL_free(path);
                if (wrong == NULL) {
                    SDL_free(names);
                    return 1;
                }
                *problem = wrong;
            }
        }
        SDL_free(names);
    }
    return 0;
}

int disc_find_beside(const char *out, const char **problem) {
    const char *base = platform_base_path();

    *problem = NULL;
    return (base != NULL && try_folder(base, out, problem)) || try_folder("./", out, problem);
}

/* Returns 1 with the data in place, or 0 if the player's chosen to leave. first says what's wrong already, if it's not NULL. */
int disc_first_run(const char *out, const char *first) {
    static const SDL_MessageBoxButtonData Buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Quit" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Choose a disc image" },
    };
    SDL_MessageBoxData box;
#ifdef SDL_PLATFORM_ANDROID
    /* On Android a .cue's .bin can't be found from it (the chooser gives one file), and the folder is the app's in shared storage */
    static char android_message[1400], android_none[1400];
    const char *message = android_message;
    const char *base = platform_base_path() != NULL ? platform_base_path() : "";

    SDL_snprintf(android_message, sizeof(android_message),
                 "Crash Bash needs the game's data, from your own copy of Crash Bash (USA).\n\n"
                 "Choose your disc image (the .bin, or an .iso) and it's copied once, so you won't need the image again. "
                 "Or copy CRASHBSH.DAT from the disc to %s on this device.", base);
    SDL_snprintf(android_none, sizeof(android_none),
                 "There's no way to choose a file here. Put your disc image (the .bin, or an .iso) or CRASHBSH.DAT in %s on this "
                 "device, and start it again.", base);
#else
    const char *message = "Crash Bash needs the game's data, from your own copy of Crash Bash (USA).\n\n"
                          "Choose your disc image (a .cue, .iso or .bin) and it's copied once, so you won't need the image again. "
                          "Or copy CRASHBSH.DAT from the disc into the same folder as this program.";
#endif

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 0;
    }
    memset(&box, 0, sizeof(box));
    box.flags = SDL_MESSAGEBOX_INFORMATION;
    box.title = "Crash Bash";
    box.numbuttons = SDL_arraysize(Buttons);
    box.buttons = Buttons;
    for (;;) {
        int pressed = 0;
        char *image;
        const char *problem;

        box.message = first != NULL ? first : message;
        first = NULL;
        if (!SDL_ShowMessageBox(&box, &pressed) || pressed != 1) {
            return 0;
        }
        image = ask_for_image();
        if (image == NULL) {
            if (NoDialog) {
#ifdef SDL_PLATFORM_ANDROID
                first = android_none;
#else
                first = "There's no way to choose a file on this system. Put your disc image (a .cue with its .bin, an .iso or a .bin) "
                        "in the same folder as this program, or copy CRASHBSH.DAT there, and start it again.";
#endif
            }
            continue;
        }
        problem = disc_extract(image, out);
        SDL_free(image);
        if (problem == NULL) {
            return 1;
        }
        message = problem;
    }
}
