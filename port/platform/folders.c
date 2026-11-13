/* Where the game keeps what it writes (folders.h). The player's choices are in folders.ini beside the program, or in the user's own
 * folder when the program's can't be written to (one in Program Files, a Linux package): a line for each kind, game, user or custom,
 * and the folder picked for it. With no such file everything is in the user's own folder, where it always was, so a player from
 * before keeps their saves with nothing to do (the user's word, 2026-11-13). Nothing is ever deleted or written over by a move: the
 * old place keeps what it had. */
#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "folders.h"
#include "platform.h"
#include "saves.h"
#include "settings.h"

#define PATH_ROOM 1024

static const char *const Names[NUM_FOLDERS] = { "saves", "settings", "data" };
static const char *const WhereNames[NUM_WHERE] = { "game", "user", "custom" };
static int Where[NUM_FOLDERS] = { WHERE_USER, WHERE_USER, WHERE_USER };
static char Custom[NUM_FOLDERS][PATH_ROOM];
static int Loaded;

/* The folder picked in the chooser, which may answer on another thread: taken up by folders_poll */
static char Picked[PATH_ROOM];
static int PickedKind;
static SDL_AtomicInt PickedReady;

static const char *user_root(void) {
    static char root[PATH_ROOM];

    if (root[0] == '\0') {
        char *dir = SDL_GetPrefPath("CrashBash", "CrashBash");

        SDL_strlcpy(root, dir != NULL ? dir : "", sizeof(root));
        SDL_free(dir);
    }
    return root;
}

static int exists(const char *path) {
    return SDL_GetPathInfo(path, NULL);
}

/* Whether the program's own folder is somewhere to keep things: it isn't on Android, where the program has none, nor where it can't
   be written to. */
static int game_writable(void) {
#ifdef SDL_PLATFORM_ANDROID
    return 0;
#else
    static int known = -1;

    if (known < 0) {
        const char *base = platform_base_path();
        char probe[PATH_ROOM];
        FILE *f;

        known = 0;
        if (base != NULL) {
            SDL_snprintf(probe, sizeof(probe), "%sfolders.tmp", base);
            if ((f = fopen(probe, "w")) != NULL) {
                fclose(f);
                SDL_RemovePath(probe);
                known = 1;
            }
        }
    }
    return known;
#endif
}

/* A kind's folder in a place. In the user's own folder it's where each always was: the settings and the data loose in it, and the saves
   in saves/. */
static void path_in(int kind, int where, char *out, size_t size) {
    if (where == WHERE_GAME && platform_base_path() != NULL) {
        SDL_snprintf(out, size, "%s%s/", platform_base_path(), Names[kind]);
    } else if (where == WHERE_CUSTOM && Custom[kind][0] != '\0') {
        SDL_snprintf(out, size, "%s/", Custom[kind]);
    } else {
        SDL_snprintf(out, size, kind == FOLDER_SAVES ? "%ssaves/" : "%s", user_root());
    }
}

const char *folders_path(int kind) {
    static char paths[NUM_FOLDERS][PATH_ROOM];

    path_in(kind, Where[kind], paths[kind], sizeof(paths[kind]));
    SDL_CreateDirectory(paths[kind]);
    return paths[kind];
}

int folders_where(int kind) {
    return Where[kind];
}

int folders_can(int kind, int where) {
    return where == WHERE_USER || (where == WHERE_GAME && game_writable()) || (where == WHERE_CUSTOM && Custom[kind][0] != '\0');
}

/* A kind's files that the new place hasn't got, copied from the old one. */
static void copy_missing(int kind, const char *from, const char *to) {
    static const char *const Files[NUM_FOLDERS][SAVES_SLOTS + 1] = {
        { "slot1.sav", "slot2.sav", "slot3.sav", "slot4.sav", NULL },
        { "settings.ini", NULL },
        { "CRASHBSH.DAT", NULL },
    };
    int i;

    if (strcmp(from, to) == 0) {
        return;
    }
    SDL_CreateDirectory(to);
    for (i = 0; Files[kind][i] != NULL; i++) {
        char a[PATH_ROOM + 32], b[PATH_ROOM + 32];

        SDL_snprintf(a, sizeof(a), "%s%s", from, Files[kind][i]);
        SDL_snprintf(b, sizeof(b), "%s%s", to, Files[kind][i]);
        if (exists(a) && !exists(b) && !SDL_CopyFile(a, b)) {
            SDL_Log("Couldn't copy %s to %s: %s", a, b, SDL_GetError());
        }
    }
}

static void ini_path(char *out, size_t size, int beside) {
    SDL_snprintf(out, size, "%sfolders.ini", beside && platform_base_path() != NULL ? platform_base_path() : user_root());
}

static void write_ini(void) {
    char path[PATH_ROOM + 16];
    FILE *f;
    int i;

    ini_path(path, sizeof(path), game_writable());
    if ((f = fopen(path, "w")) == NULL) {
        return;
    }
    for (i = 0; i < NUM_FOLDERS; i++) {
        fprintf(f, "%s=%s\n", Names[i], WhereNames[Where[i]]);
        if (Custom[i][0] != '\0') {
            fprintf(f, "%s_folder=%s\n", Names[i], Custom[i]);
        }
    }
    fclose(f);
}

/* The one beside the program first. */
static void read_ini(void) {
    char path[PATH_ROOM + 16], line[PATH_ROOM + 64];
    FILE *f;
    int i, w;

    ini_path(path, sizeof(path), 1);
    if (!game_writable() || (f = fopen(path, "r")) == NULL) {
        ini_path(path, sizeof(path), 0);
        if ((f = fopen(path, "r")) == NULL) {
            return;
        }
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        char *value = strchr(line, '=');

        line[strcspn(line, "\r\n")] = '\0';
        if (value == NULL) {
            continue;
        }
        *value++ = '\0';
        for (i = 0; i < NUM_FOLDERS; i++) {
            size_t n = strlen(Names[i]);

            if (strcmp(line, Names[i]) == 0) {
                for (w = 0; w < NUM_WHERE; w++) {
                    if (strcmp(value, WhereNames[w]) == 0) {
                        Where[i] = w;
                    }
                }
            } else if (strncmp(line, Names[i], n) == 0 && strcmp(line + n, "_folder") == 0) {
                SDL_strlcpy(Custom[i], value, sizeof(Custom[i]));
            }
        }
    }
    fclose(f);
}

void folders_load(void) {
    int i;

    read_ini();
    Loaded = 1;
    for (i = 0; i < NUM_FOLDERS; i++) {
        if (!folders_can(i, Where[i])) {
            Where[i] = WHERE_USER;
        }
    }
}

/* A kind's in a new place, from the old one: what the new one hasn't got is copied, and it's used from now. */
static void moved(int kind, const char *old) {
    copy_missing(kind, old, folders_path(kind));
    if (kind == FOLDER_SAVES) {
        char folder[PATH_ROOM];

        SDL_strlcpy(folder, folders_path(kind), sizeof(folder));
        folder[strlen(folder) - 1] = '\0';
        saves_set_folder(folder);
    } else if (kind == FOLDER_SETTINGS) {
        settings_save();
    }
    write_ini();
}

void folders_set(int kind, int where) {
    char old[PATH_ROOM];

    if (!folders_can(kind, where)) {
        return;
    }
    /* a test run's choice is only for show: its saves and settings are where the run says, and nothing's kept */
    if (!Loaded) {
        Where[kind] = where;
        return;
    }
    SDL_strlcpy(old, folders_path(kind), sizeof(old));
    Where[kind] = where;
    moved(kind, old);
}

static void SDLCALL picked(void *userdata, const char *const *list, int filter) {
    (void)filter;
    if (list == NULL || list[0] == NULL || SDL_GetAtomicInt(&PickedReady)) {
        return;
    }
    PickedKind = (int)(intptr_t)userdata;
    SDL_strlcpy(Picked, list[0], sizeof(Picked));
    SDL_SetAtomicInt(&PickedReady, 1);
}

void folders_choose(int kind) {
    if (Loaded) {
        SDL_ShowOpenFolderDialog(picked, (void *)(intptr_t)kind, host_window(), folders_path(kind), false);
    }
}

int folders_poll(void) {
    char old[PATH_ROOM];
    size_t n, i;

    if (!SDL_GetAtomicInt(&PickedReady)) {
        return 0;
    }
    SDL_strlcpy(old, folders_path(PickedKind), sizeof(old));
    for (i = 0; Picked[i] != '\0'; i++) {
        if (Picked[i] == '\\') {
            Picked[i] = '/';
        }
    }
    for (n = strlen(Picked); n > 1 && Picked[n - 1] == '/'; n--) {
        Picked[n - 1] = '\0';
    }
    SDL_strlcpy(Custom[PickedKind], Picked, sizeof(Custom[PickedKind]));
    SDL_SetAtomicInt(&PickedReady, 0);
    Where[PickedKind] = WHERE_CUSTOM;
    moved(PickedKind, old);
    return 1;
}
