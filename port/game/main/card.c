/* The game's save slots, in memory and as files (platform/saves.c). The save screens read CardStatus, which is always ready here,
 * as there's no card to wait for, take out or format. */
#include "common.h"
#include "bash/main.h"
#include "bash/card.h"
#include "../../platform/saves.h"

CardSlot CardSlots[4];
s32 CardSaveSum = 0;
s32 CardStatus = CARD_READY;

static s32 slots_sum(void) {
    u32 *p = (u32 *)CardSlots;
    u32 *end = (u32 *)(CardSlots + 4);
    u32 sum = 0;

    while (p < end) {
        sum += *p++;
    }
    return (s32)sum;
}

void card_init(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (!saves_read(i, &CardSlots[i], sizeof(CardSlot))) {
            card_clear_slot(&CardSlots[i]);
        }
    }
    CardSaveSum = slots_sum();
    CardStatus = CARD_READY;
}

void card_save(void) {
    s32 i;

    CardStatus = CARD_READY;
    for (i = 0; i < 4; i++) {
        if (*(s32 *)CardSlots[i].summary != 0 && !saves_write(i, &CardSlots[i], sizeof(CardSlot))) {
            CardStatus = CARD_READY | CARD_FAILED;
        }
    }
    CardSaveSum = slots_sum();
}

void card_reset(void) {
    CardStatus = CARD_READY;
}

void card_clear_slot(CardSlot *slot) {
    s32 i;

    for (i = 0; i < 8; i++) {
        slot->summary[i] = 0;
    }
}
