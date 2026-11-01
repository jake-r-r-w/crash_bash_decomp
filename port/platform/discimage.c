/* Pulls the game's archive out of the player's own disc image, for the first time it runs: a .cue with its .bin, a raw .bin or an .iso. Only
 * the one file is read, by the disc's own ISO 9660 directories. SDL's file calls, so a path can hold any characters. */
#include <stdint.h>
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
    uint8_t head[32];

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

/* The file dialog answers from its own thread. */
static SDL_AtomicInt Answered;
static char *Chosen;

static void SDLCALL file_chosen(void *unused, const char *const *files, int filter) {
    (void)unused;
    (void)filter;
    Chosen = files != NULL && files[0] != NULL ? SDL_strdup(files[0]) : NULL;
    SDL_SetAtomicInt(&Answered, 1);
}

static char *ask_for_image(void) {
    static const SDL_DialogFileFilter Filters[] = {
        { "Disc images (.cue, .iso, .bin, .img)", "cue;iso;bin;img" },
        { "All files", "*" },
    };

    SDL_SetAtomicInt(&Answered, 0);
    Chosen = NULL;
    SDL_ShowOpenFileDialog(file_chosen, NULL, NULL, Filters, SDL_arraysize(Filters), NULL, false);
    while (!SDL_GetAtomicInt(&Answered)) {
        SDL_PumpEvents();
        SDL_Delay(10);
    }
    return Chosen;
}

/* Returns 1 with the data in place, or 0 if the player's chosen to leave. */
int disc_first_run(const char *out) {
    static const SDL_MessageBoxButtonData Buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Quit" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Choose a disc image" },
    };
    SDL_MessageBoxData box;
    const char *message = "Crash Bash needs the game's data, from your own copy of Crash Bash (USA).\n\n"
                          "Choose your disc image (a .cue, .iso or .bin) and it's copied once, so you won't need the image again. "
                          "Or copy CRASHBSH.DAT from the disc into the same folder as this program.";

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

        box.message = message;
        if (!SDL_ShowMessageBox(&box, &pressed) || pressed != 1) {
            return 0;
        }
        image = ask_for_image();
        if (image == NULL) {
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
