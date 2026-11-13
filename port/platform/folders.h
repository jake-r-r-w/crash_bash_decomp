/* Where the game keeps what it writes (folders.c): its saves, its settings, and the copy of the game's data it makes from the player's
 * disc. Each has a folder of its own beside the program (saves/, settings/, data/), which is where they go unless the player says
 * otherwise (the launcher's DATA page), or in the user's own folder, where they all went before (%APPDATA%\CrashBash\CrashBash on
 * Windows), or in a folder the player picks. */
#ifndef FOLDERS_H
#define FOLDERS_H

enum { FOLDER_SAVES, FOLDER_SETTINGS, FOLDER_DATA, NUM_FOLDERS };
/* Where one is: beside the program, in the user's own folder, or in a folder of the player's choosing */
enum { WHERE_GAME, WHERE_USER, WHERE_CUSTOM, NUM_WHERE };

/* Reads the player's choices, or makes them the first time: beside the program if that can be written to, with the saves and settings
   from the user's own folder copied over, so a player from before has theirs. Not called, as in a test run, everything's in the user's
   own folder, as before. */
void folders_load(void);
/* A kind's folder, ending in /, made if it wasn't there. */
const char *folders_path(int kind);
int folders_where(int kind);
/* Whether a kind can be there: beside the program only if that can be written to, and a folder of the player's only once picked. */
int folders_can(int kind, int where);
/* Moves a kind: what the new place hasn't got is copied to it from the old one, nothing there is written over, and nothing's taken
   from the old one. The saves and settings are read and written there from then on; the game's data from the next start. */
void folders_set(int kind, int where);
/* Asks the player for a folder for a kind, with the system's own folder chooser; folders_poll is 1 once, when one's been picked and
   the kind's moved there. */
void folders_choose(int kind);
int folders_poll(void);

#endif
