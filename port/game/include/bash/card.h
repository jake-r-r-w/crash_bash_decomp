#ifndef BASH_CARD_H
#define BASH_CARD_H

/* The game's save slots (main/card.c). */

#include "common.h"
#include "bash/main.h"

void card_init(void);
void card_reset(void);
void card_clear_slot(CardSlot *slot);
void card_save(void);

#endif /* BASH_CARD_H */
