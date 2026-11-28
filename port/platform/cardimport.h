/* A PS1 memory card's Crash Bash saves into the port's save slots (cardimport.c; docs/PORT.md, Saves from a PS1 memory card). */
#ifndef CARDIMPORT_H
#define CARDIMPORT_H

#include <stddef.h>

/* The saves on a card image (.srm, .mcd, .mcr) or a save alone, the USA disc's or the European's, written over the slots they're in
   (what was there kept in before-import/). NULL when it's done, with what it did in report; else what's wrong, in the font's capitals,
   and in report which slot and why when a save's not one the game could make. */
const char *card_import(const char *path, char *report, size_t room);

/* The launcher's DATA page: the system's chooser for a card, then, once one's picked, the import (1 done, -1 not, 0 nothing yet), with
   what to show in message. */
void card_import_choose(void *window);
int card_import_poll(char *message, size_t room);
/* A test run: as if path had been chosen (--pick-card FILE) */
void card_import_pretend(const char *path);

/* And the other way: the port's saves as the USA game's save on a card image, onto the card at path if it's one (its other games' saves
   stay), else a new card. NULL when it's done, with what it did in report; else what's wrong. The chooser, a test run's pick and the
   poll, as the import's. */
const char *card_export(const char *path, char *report, size_t room);
void card_export_choose(void *window);
void card_export_pretend(const char *path);
int card_export_ready(void);
int card_export_poll(char *message, size_t room);

#endif
