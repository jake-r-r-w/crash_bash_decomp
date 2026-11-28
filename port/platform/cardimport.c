/* A PS1 memory card's Crash Bash saves, brought into the port's save slots (docs/PORT.md, Saves from a PS1 memory card).
 *
 * The card is an emulator's image of it: 128 KB, "MC", its directory in 15 frames of 0x80 bytes and 15 blocks of 0x2000 (RetroArch's
 * .srm, and .mcd and .mcr), or the game's save alone, one block starting "SC". The game's save is one block, named BASCUS-94570 on the
 * USA disc and BESCES-02834 on the European one: the PsyQ header, then from 0x200 the four slots the game keeps, each 8 bytes of
 * summary (the heroes picked) and the game's state. That's what the port keeps, one slot to a file (saves.c), so a USA slot comes across
 * as it is. The European game's state is 8 bytes longer: its options are 0x18 bytes to the USA's 0x10 (two more words after the
 * vibration's), so its slot is 0x19C and everything after the options sits 8 later (the awards at 0xEC, not 0xE4: both programs'
 * code). Those two words are dropped, and its language and frame rate (50) are made the USA game's (0, 60).
 *
 * Every slot is checked as the game would check a save from another machine (card_check), and only then are any written; what was
 * in a slot it writes over is copied first to before-import/ beside the saves. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include "cardimport.h"
#include "saves.h"

#define CARD_BYTES (16 * 0x2000)
#define BLOCK 0x2000
#define FRAME 0x80
#define SLOTS_AT 0x200
#define USA_SLOT 0x194 /* the port's slot: 8 bytes of summary, then the game's 0x18C bytes of state */
#define EUROPE_SLOT 0x19C
#define SUMMARY 8
#define STATE_FPS 0x21      /* GameState.fps */
#define STATE_LANGUAGE 0x2C /* GameState.language */
#define EUROPE_EXTRA_AT 0xE0 /* the state's offset of the European options' two more words */
#define EUROPE_EXTRA 8

typedef struct {
    const char *name;
    int europe;
} SaveName;

static const SaveName Names[] = { { "BASCUS-94570", 0 }, { "BESCES-02834", 1 } };

/* The block with the game's save on it, and whose it is: NULL if there's none */
static const unsigned char *find_save(const unsigned char *card, size_t size, int *europe) {
    int i, k;

    if (size == BLOCK && card[0] == 'S' && card[1] == 'C') {
        *europe = -1; /* a save alone: which game it's from, its slots' sizes say */
        return card;
    }
    if (size != CARD_BYTES || card[0] != 'M' || card[1] != 'C') {
        return NULL;
    }
    for (i = 1; i < 16; i++) {
        const unsigned char *f = card + i * FRAME;
        unsigned state = f[0] | f[1] << 8 | f[2] << 16 | (unsigned)f[3] << 24;

        for (k = 0; state == 0x51 && k < (int)(sizeof(Names) / sizeof(Names[0])); k++) {
            if (memcmp(f + 0x0A, Names[k].name, strlen(Names[k].name)) == 0) {
                *europe = Names[k].europe;
                return card + i * BLOCK;
            }
        }
    }
    return NULL;
}

/* A slot of the card's, as the port keeps it */
static void port_slot(const unsigned char *from, int europe, unsigned char *to) {
    if (!europe) {
        memcpy(to, from, USA_SLOT);
        return;
    }
    memcpy(to, from, SUMMARY + EUROPE_EXTRA_AT);
    memcpy(to + SUMMARY + EUROPE_EXTRA_AT, from + SUMMARY + EUROPE_EXTRA_AT + EUROPE_EXTRA, USA_SLOT - SUMMARY - EUROPE_EXTRA_AT);
    memset(to + SUMMARY + STATE_LANGUAGE, 0, 4);
    to[SUMMARY + STATE_FPS] = 60;
}

static int slot_used(const unsigned char *slot) {
    return slot[0] != 0 || slot[1] != 0 || slot[2] != 0 || slot[3] != 0;
}

/* What's in slot n now, copied to before-import/ beside the saves */
static void keep_old(int n) {
    char from[1200], to[1200], folder[1100];
    size_t size = 0;
    void *bytes;

    SDL_snprintf(from, sizeof(from), "%sslot%d.sav", saves_folder(), n + 1);
    if ((bytes = SDL_LoadFile(from, &size)) == NULL) {
        return;
    }
    SDL_snprintf(folder, sizeof(folder), "%sbefore-import", saves_folder());
    SDL_CreateDirectory(folder);
    SDL_snprintf(to, sizeof(to), "%s/slot%d.sav", folder, n + 1);
    SDL_SaveFile(to, bytes, size);
    SDL_free(bytes);
}

const char *card_import(const char *path, char *report, size_t room) {
    unsigned char slots[4][USA_SLOT];
    const unsigned char *save;
    unsigned char *card;
    size_t size = 0;
    int europe = 0, slot_size, n, found = 0;
    char why[96];

    report[0] = '\0';
    if (saves_folder()[0] == '\0') {
        return "THERE'S NOWHERE TO KEEP SAVES";
    }
    if ((card = SDL_LoadFile(path, &size)) == NULL) {
        return "THE FILE CAN'T BE READ";
    }
    if ((save = find_save(card, size, &europe)) == NULL) {
        SDL_free(card);
        return size == CARD_BYTES ? "NO CRASH BASH SAVE ON THIS CARD" : "THAT ISN'T A PS1 MEMORY CARD";
    }
    /* a save alone: the European one's slots are 0x19C, so its fourth ends at 0x870 and the USA's at 0x850; the USA's checksum is at
       0x85C, where the European's fourth slot still is. Its sum says which. */
    if (europe < 0) {
        unsigned sum = 0, stored = save[0x85C] | save[0x85D] << 8 | save[0x85E] << 16 | (unsigned)save[0x85F] << 24;

        for (n = SLOTS_AT; n < 0x850; n += 4) {
            sum += save[n] | save[n + 1] << 8 | save[n + 2] << 16 | (unsigned)save[n + 3] << 24;
        }
        europe = sum != stored;
    }
    slot_size = europe ? EUROPE_SLOT : USA_SLOT;
    for (n = 0; n < 4; n++) {
        const unsigned char *from = save + SLOTS_AT + n * slot_size;

        if (!slot_used(from)) {
            memset(slots[n], 0, USA_SLOT);
            continue;
        }
        port_slot(from, europe, slots[n]);
        if (!card_check(slots[n], USA_SLOT, why, sizeof(why))) {
            SDL_free(card);
            SDL_snprintf(report, room, "slot %d: %s", n + 1, why);
            printf("card import: %s's slot %d isn't a save the game could make: %s\n", path, n + 1, why);
            return "A SAVE ON IT ISN'T ONE THE GAME COULD MAKE";
        }
        found++;
    }
    SDL_free(card);
    if (found == 0) {
        return "ITS CRASH BASH SAVE HAS NO GAMES IN IT";
    }
    for (n = 0; n < 4; n++) {
        if (slot_used(slots[n])) {
            keep_old(n);
            if (!saves_write(n, slots[n], USA_SLOT)) {
                return "THE SAVES CAN'T BE WRITTEN";
            }
        }
    }
    SDL_snprintf(report, room, "IMPORTED %d %s FROM THE %s GAME", found, found == 1 ? "SAVE" : "SAVES", europe ? "EUROPEAN" : "USA");
    printf("card import: %s: %s\n", path, report);
    return NULL;
}

/* --- the launcher's chooser --- */

static char Picked[1100];
static SDL_AtomicInt PickedReady;

static void SDLCALL picked(void *userdata, const char *const *list, int filter) {
    (void)userdata;
    (void)filter;
    if (list == NULL || list[0] == NULL || SDL_GetAtomicInt(&PickedReady)) {
        return;
    }
    SDL_strlcpy(Picked, list[0], sizeof(Picked));
    SDL_SetAtomicInt(&PickedReady, 1);
}

/* A test run's pick, as if chosen in the dialog (--pick-card) */
void card_import_pretend(const char *path) {
    SDL_strlcpy(Picked, path, sizeof(Picked));
    SDL_SetAtomicInt(&PickedReady, 1);
}

void card_import_choose(void *window) {
    static const SDL_DialogFileFilter filters[] = {
        { "PS1 memory card", "srm;mcd;mcr;mc;bin;sav" },
        { "Any file", "*" },
    };

    SDL_ShowOpenFileDialog(picked, NULL, window, filters, 2, NULL, false);
}

int card_import_poll(char *message, size_t room) {
    char report[96];
    const char *problem;

    if (!SDL_GetAtomicInt(&PickedReady)) {
        return 0;
    }
    problem = card_import(Picked, report, sizeof(report));
    SDL_SetAtomicInt(&PickedReady, 0);
    SDL_snprintf(message, room, "%s", problem != NULL ? problem : report);
    return problem != NULL ? -1 : 1;
}
