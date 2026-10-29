#include "common.h"
#include "bash/main.h"
#include "bash/card.h"

s32 ascii_to_sjis(u8 c) {
    static const void *const tbl[31] = {
        &&l21, &&l22, &&l23, &&l24, &&l25, &&ldef, &&ldef, &&l28,
        &&l29, &&l2A, &&l2B, &&l2C, &&l2D, &&l2E, &&l2F, &&ldef,
        &&ldef, &&ldef, &&ldef, &&ldef, &&ldef, &&ldef, &&ldef, &&ldef,
        &&ldef, &&l3A, &&l3B, &&ldef, &&ldef, &&ldef, &&l3F
    };
    u32 i;
    if ((u8)(c - 0x41) < 0x1A) {
        return (c + 0x1F) | 0x8200;
    }
    if ((u8)(c - 0x61) < 0x1A) {
        return (c + 0x20) | 0x8200;
    }
    if ((u8)(c - 0x30) < 0xA) {
        return (c + 0x1F) | 0x8200;
    }
    i = c - 0x21;
    if (i < 0x1F) {
        goto *tbl[i];
    }
    goto ldef;
l21: return 0x8149;
l22: return 0x8168;
l23: return 0x8194;
l24: return 0x8190;
l25: return 0x8193;
l28: return 0x8169;
l29: return 0x816A;
l2A: return 0x8196;
l2B: return 0x817B;
l2C: return 0x8143;
l2D: return 0x817C;
l2E: return 0x8144;
l2F: return 0x815E;
l3A: return 0x8146;
l3B: return 0x8147;
l3F: return 0x8148;
ldef: return 0x8140;
}

void card_make_header(void) {
    RECT clut;
    RECT *rect;
    u8 *base;
    u8 *p;
    s32 k;
    obj_texture *img;
    TexClut *pal;
    s16 h;
    u16 w;
    s32 *q;
    s32 *end;
    s32 sum;

    ((McHeader *)CardBuffer)->magic = 0x4353;
    ((McHeader *)CardBuffer)->frames = 2;
    ((McHeader *)CardBuffer)->type = 1;
    ((McHeader *)CardBuffer)->blocks = 1;
    k = 0x1B;
    p = CardBuffer + k;
    do {
        p[0x44] = 0;
        k--;
        p--;
    } while (k >= 0);
    /* the slots' saved states (CardSlots[n].state), by the card image's base */
    itoa_div_end(CardSaveTitle + 0xC, count_completion(CardBuffer + 0x208), 100);
    itoa_div_end(CardSaveTitle + 0x11, count_completion(CardBuffer + 0x208 + 0x194), 100);
    itoa_div_end(CardSaveTitle + 0x16, count_completion(CardBuffer + 0x208 + 0x194 * 2), 100);
    itoa_div_end(CardSaveTitle + 0x1B, count_completion(CardBuffer + 0x208 + 0x194 * 3), 100);
    McSetTitle(CardSaveTitleFormat, ((McHeader *)CardBuffer)->title);
    rect = (RECT *)&dbase_texture_ptr(&HeroModelBank[DB_Font], 0x306B)->info.vramX;
    if (rect->w == 4) {
        h = rect->h;
        if (h == 0x10) {
            StoreImage(rect, (u_long *)CardIconFrames);
            img = dbase_texture_ptr(&HeroModelBank[DB_Font], 0x306C);
            StoreImage((RECT *)&img->info.vramX, (u_long *)FILE_AT(CardIconFrames, 0x80)  /* the second 16x16 4-bit frame */);
            pal = dbase_texture_entry(&HeroModelBank[DB_Font], (u16)img->clut);
            clut.x = pal->pos->x;
            clut.y = pal->pos->y;
            w = pal->w;
            if (w == h) {
                clut.w = w;
                clut.h = 1;
                StoreImage(&clut, (u_long *)(CardIconFrames - 0x20));
            }
        }
    }
    base = CardBuffer;
    q = (s32 *)FILE_AT(base, CARD_SLOTS);
    end = (s32 *)FILE_AT(base, CARD_FLAGS);
    sum = 0;
    while (q < end) {
        sum += *q++;
    }
    CardSaveSum = sum;
}

void card_init(void) {
    u8 *p;
    s32 i;
    s32 *q;
    s32 *end;
    s32 sum;
    int new_var;

    MemCardInit(0);
    MemCardStart();
    i = 0;
    CardChannel = 0;
    CardStatus = 0;
    CardCommand = 0;
    CardRequest = 0;
    for (;;) {
        if (i < 4) {
            CardSlots[i].summary[0] = 0;
            CardSlots[i].summary[1] = 0;
            CardSlots[i].summary[2] = 0;
            CardSlots[i].summary[3] = 0;
            CardSlots[i].summary[4] = 0;
            CardSlots[i].summary[5] = 0;
            CardSlots[i].summary[6] = 0;
            CardSlots[i].summary[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = CARD_FLAGS;
    *(s32 *)FILE_AT(CardBuffer, new_var) = 1;
    i = 7;
    p = CardBuffer + i;
    do {
        p[CARD_FLAGS + 4] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)FILE_AT(CardBuffer, new_var);
    q = (s32 *)FILE_AT(CardBuffer, CARD_SLOTS);
    sum = 0;
    while (q < end) {
        sum += *q++;
    }
    CardSaveSum = sum;
}

const char CardSaveTitleFormat[] = "CRASH BASH %000 %000 %000 %000";

const char CardFileName[] = "BASCUS-94570";

void card_update(void) {
    static const char all[] = "*";
    static const void *const tbl[8] = {
        &&l0, &&l1, &&l2, &&l3, &&l4, &&l5, &&lend, &&l7
    };
    static const void *const tbl2[6] = {
        &&l2_03, &&l2_1, &&l2_2, &&l2_03, &&l2_4, 0
    };
    static const void *const tbl4[5] = {
        &&l4_024, &&l4_13, &&l4_024, &&l4_13, &&l4_024
    };
    u8 *p;
    s32 i;
    s32 *q;
    s32 *end;
    s32 sum;
    u32 s;

    if (MemCardSync(1, &CardSyncCmd, &CardSyncResult) == 0) {
        return;
    }
    s = CardCommand;
    if (s < 8) {
        goto *tbl[s];
    }
    goto lend;
l0:
    MemCardExist(CardChannel);
    i = 1;
    CardStatus = 0;
    CardCommand = i;
    return;
l1:
    switch (CardSyncResult) {
    case 1:
        CardCommand = 0;
        CardStatus = 0;
        return;
    case 0:
    case 2:
    case 3:
    case 4:
        MemCardAccept(CardChannel);
        CardStatus = 0x42;
        CardCommand = 2;
        break;
    }
    goto lend;
l2:
    McResetSave();
    s = CardSyncResult;
    if (s < 5) {
        goto *tbl2[s];
    }
    goto lend;
l2_1:
    CardCommand = 0;
    CardStatus = 0;
    return;
l2_4:
    CardStatus = 6;
    CardCommand = 3;
    goto lend;
l2_03:
    if (MemCardGetDirentry(CardChannel, (char *)all,(struct DIRENTRY *)CardDir, &CardDirCount, 0, 0xF) != 0) {
        CardCommand = 0;
    }
    if (McFindSave() != NULL) {
        McClearSave();
        MemCardReadFile(CardChannel, CardFileName, (u32 *)CardBuffer, 0, 0x2000);
        CardCommand = 7;
        goto lend;
    }
    McResetSave();
    CardStatus = 0;
    if (McFreeBytes() >= 0x2000) {
        CardStatus |= 0x20;
    }
    CardStatus |= 2;
    CardCommand = 3;
    return;
l3:
    if (CardRequest == 0 || card_command_delay != 0) {
        card_command_delay--;
        MemCardExist(CardChannel);
        CardCommand = 4;
        goto lend;
    }
    switch (CardRequest) {
    case 2:
        McResetSaveSum();
        if (MemCardFormat(CardChannel) == 0) {
            CardStatus = 0x22;
            CardCommand = 3;
            CardRequest = 0;
            return;
        }
        CardStatus |= 0x100;
        CardCommand = 6;
        return;
    case 1:
        if (!(CardStatus & 0x10) && MemCardCreateFile(CardChannel, CardFileName, 1) != 0) {
            CardCommand = 6;
            CardStatus |= 0x100;
        }
        if (CardCommand != 6) {
            card_make_header();
            MemCardWriteFile(CardChannel, CardFileName, (u32 *)CardBuffer, 0, 0x2000);
            CardCommand = 5;
            return;
        }
        break;
    }
    goto lend;
l4:
    s = CardSyncResult;
    if (s < 5) {
        goto *tbl4[s];
    }
    goto lend;
l4_13:
    CardStatus = 0;
    CardCommand = 0;
    return;
l5:
    if (CardRequest == 1) {
        if (CardSyncResult == 0) {
            CardStatus = 0x12;
            CardCommand = 3;
            CardRequest = 0;
            return;
        }
        CardStatus |= 0x100;
        CardCommand = 6;
        return;
    }
    goto lend;
l7:
    if (CardSyncResult == 0) {
        p = CardBuffer;
        q = (s32 *)FILE_AT(p, CARD_SLOTS);
        end = (s32 *)FILE_AT(p, CARD_FLAGS);
        sum = 0;
        while (q < end) {
            sum += *q++;
        }
        if (*(s32 *)FILE_AT(p, CARD_SUM) == sum) {
            CardCommand = 3;
            CardStatus = 0x12;
            return;
        }
    }
l2_2:
    CardStatus = 0xA;
l4_024:
    CardCommand = 3;
lend:;
}

s32 card_saving(void) {
    if (CardRequest == 1 && (CardStatus & 0x80)) {
        return 1;
    }
    return 0;
}

void card_save(void) {
    s32 flags;
    s32 on;

    flags = CardStatus;
    if ((flags & 2) && (flags & 0x30)) {
        on = TRUE;
        card_command_delay = on;
        CardRequest = on;
        CardStatus = flags | 0x80;
    }
}

void card_format(void) {
    s32 mode = 2;

    card_command_delay = 1;
    CardRequest = mode;
    CardStatus |= 0x80;
}

void card_request(s32 arg0) {
    card_command_delay = 1;
    CardRequest = arg0;
    CardStatus |= 0x80;
}

void card_reset(void) {
    CardRequest = 0;
    CardCommand = 0;
    CardStatus = 0;
}

s32 card_count_files(void) {
    SizeEntry *p;
    s32 i;
    s32 sum;

    i = CardDirCount;
    if (i >= 0xF) {
        return 0;
    }
    p = CardDir;
    i--;
    sum = 0;
    if (i != -1) {
        do {
            sum += p->size;
            p++;
            i--;
        } while (i != -1);
    }
    return 0x1E000 - sum;
}

SizeEntry *card_find_file(void) {
    SizeEntry *p;
    s32 i;

    i = CardDirCount;
    p = CardDir;
    i--;
    if (i != -1) {
        do {
            if (strcmp(CardFileName, (char *)p) == 0) {
                return p;
            }
            i--;
            p++;
        } while (i != -1);
    }
    return NULL;
}

void card_stop(void) {
    MemCardStop();
}

void card_poll(void) {
    card_update();
}

void card_clear_slots(void) {
    CardSlot *s;
    u8 *p;
    s32 i;
    s32 *q;
    s32 *end;
    s32 sum;
    int new_var;

    i = 0;
    for (;;) {
        if (i < 4) {
            CardSlots[i].summary[0] = 0;
            CardSlots[i].summary[1] = 0;
            CardSlots[i].summary[2] = 0;
            CardSlots[i].summary[3] = 0;
            CardSlots[i].summary[4] = 0;
            CardSlots[i].summary[5] = 0;
            CardSlots[i].summary[6] = 0;
            CardSlots[i].summary[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = CARD_FLAGS;
    *(s32 *)FILE_AT(CardBuffer, new_var) = 1;
    i = 7;
    p = CardBuffer + i;
    do {
        p[CARD_FLAGS + 4] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)FILE_AT(CardBuffer, new_var);
    q = (s32 *)FILE_AT(CardBuffer, CARD_SLOTS);
    sum = 0;
    while (q < end) {
        sum += *q++;
    }
    CardSaveSum = sum;
}

void card_clear_slot(CardSlot *slot) {
    slot->summary[0] = 0;
    slot->summary[1] = 0;
    slot->summary[2] = 0;
    slot->summary[3] = 0;
    slot->summary[4] = 0;
    slot->summary[5] = 0;
    slot->summary[6] = 0;
    slot->summary[7] = 0;
}

s32 card_checksum(u8 *arg0) {
    u32 *p;
    u32 *end;
    s32 sum;

    p = (u32 *)FILE_AT(arg0, CARD_SLOTS);
    end = (u32 *)FILE_AT(arg0, CARD_FLAGS);
    sum = 0;
    while (p < end) {
        sum += *p++;
    }
    return sum;
}


void card_sjis_title(u8 *arg0, s16 *arg1) {
    s16 *end;
    s16 fill;
    s32 i;
    s32 done;
    u8 c;
    s32 r;
    s32 r2;

    r2 = ascii_to_sjis(0x20);
    fill = ((r2 & 0xFF) << 8) | ((u32)(ascii_to_sjis(0x20) & 0xFFFF) >> 8);
    i = 0;
    done = 0;
    end = arg1 + 0x20;
    do {
        if (done == 0) {
            c = arg0[i];
            i++;
            if (c >= 0x20U) {
                r = ascii_to_sjis(c);
                *arg1 = ((r & 0xFF) << 8) | ((u32)(ascii_to_sjis(c) & 0xFFFF) >> 8);
            } else {
                done = 1;
            }
            if (done != 0) {
                goto fillit;
            }
        } else {
fillit:
            *arg1 = fill;
        }
        arg1++;
    } while ((s32)arg1 < (s32)end);
}

/* data, in address order (tools/scripts/data2c.py) */
s32 CardChannel = 0;
char *CardSaveTitle = (void *)&CardSaveTitleFormat;
s32 CardStatus = 0;
s32 CardRequest = 0;
u8 CardBuffer[128] = {0};
u8 CardIconFrames[384] = {0};
CardSlot CardSlots[4] = {0};
s32 CardSaveFlags[3] = {0};  /* CardBuffer + 0x850: the save's flags */
s32 CardSaveSum = 0;
s32 CardSaveRest[1512] = {0};  /* CardBuffer + 0x860 to the block's end */
s32 card_command_delay = 0;
s32 CardCommand = 0;
s32 CardDirCount = 0;
SizeEntry CardDir[15] = {0};
s32 CardSyncCmd = 0;
s32 CardSyncResult = 0;
s32 CardUnused[2] = { 0x1C7350, 0x3200000 };
