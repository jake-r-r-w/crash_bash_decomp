/* The player's saves: a file for each slot, beside the settings. A header says what's in it, and a checksum whether it's
 * all there. Each is written to a new file and then moved over the old one, so stopping halfway can't lose a save. A test run has
 * nowhere to save unless --saves gives it a folder. */
#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "saves.h"

#define MAGIC "CBSV"
#define VERSION 1

typedef struct SaveHeader {
    char magic[4];
    Uint32 version;
    Uint32 size;
    Uint32 checksum;
} SaveHeader;

static char Folder[1024];

void saves_set_folder(const char *folder) {
    if (folder == NULL) {
        Folder[0] = '\0';
    } else {
        SDL_snprintf(Folder, sizeof(Folder), "%s/", folder);
        SDL_CreateDirectory(Folder);
    }
}

void saves_use_default_folder(void) {
    char *dir = SDL_GetPrefPath("CrashBash", "CrashBash");

    if (dir != NULL) {
        SDL_snprintf(Folder, sizeof(Folder), "%ssaves/", dir);
        SDL_free(dir);
        SDL_CreateDirectory(Folder);
    }
}

static void slot_path(char *path, size_t size, int slot) {
    SDL_snprintf(path, size, "%sslot%d.sav", Folder, slot + 1);
}

int saves_read(int slot, void *data, int size) {
    char path[1100];
    SaveHeader h;
    FILE *f;
    int ok;

    if (Folder[0] == '\0') {
        return 0;
    }
    slot_path(path, sizeof(path), slot);
    f = fopen(path, "rb");
    if (f == NULL) {
        return 0;
    }
    ok = fread(&h, sizeof(h), 1, f) == 1 && memcmp(h.magic, MAGIC, 4) == 0 && h.version == VERSION && h.size == (Uint32)size
        && fread(data, 1, (size_t)size, f) == (size_t)size && SDL_crc32(0, data, (size_t)size) == h.checksum;
    fclose(f);
    if (!ok) {
        fprintf(stderr, "Save %s can't be read, so that slot is empty.\n", path);
    }
    return ok;
}

int saves_write(int slot, const void *data, int size) {
    char path[1100], temp[1110];
    SaveHeader h;
    FILE *f;
    int ok;

    if (Folder[0] == '\0') {
        return 1;
    }
    slot_path(path, sizeof(path), slot);
    SDL_snprintf(temp, sizeof(temp), "%s.new", path);
    f = fopen(temp, "wb");
    if (f == NULL) {
        return 0;
    }
    memcpy(h.magic, MAGIC, 4);
    h.version = VERSION;
    h.size = (Uint32)size;
    h.checksum = SDL_crc32(0, data, (size_t)size);
    ok = fwrite(&h, sizeof(h), 1, f) == 1 && fwrite(data, 1, (size_t)size, f) == (size_t)size;
    ok = fclose(f) == 0 && ok;
    return ok && SDL_RenamePath(temp, path);
}
