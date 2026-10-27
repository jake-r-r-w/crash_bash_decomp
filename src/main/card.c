#include "common.h"
#include "bash/main.h"
#include "bash/card.h"

s32 func_8002C4B0(u8 c) {
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

void func_8002C5BC(void) {
    RECT clut;
    RECT *rect;
    u8 *base;
    u8 *p;
    s32 k;
    u8 *img;
    u8 *pal;
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
    func_8001517C(D_800655BC + 0xC, func_80010000(CardBuffer + 0x208), 100);
    func_8001517C(D_800655BC + 0x11, func_80010000(CardBuffer + 0x208 + 0x194), 100);
    func_8001517C(D_800655BC + 0x16, func_80010000(CardBuffer + 0x208 + 0x194 * 2), 100);
    func_8001517C(D_800655BC + 0x1B, func_80010000(CardBuffer + 0x208 + 0x194 * 3), 100);
    McSetTitle(CardSaveTitleFormat, ((McHeader *)CardBuffer)->title);
    rect = (RECT *)(func_800160F8(FontModel, 0x306B) + 0x18);
    if (rect->w == 4) {
        h = rect->h;
        if (h == 0x10) {
            StoreImage(rect, (u_long *)D_80065648);
            img = (u8 *)func_800160F8(FontModel, 0x306C);
            StoreImage((RECT *)(img + 0x18), (u_long *)(D_80065648 + 0x80));
            pal = func_800160C4(FontModel, *(u16 *)(img + 2));
            clut.x = (*(u16 **)(pal + 4))[0];
            clut.y = (*(u16 **)(pal + 4))[1];
            w = *(u16 *)pal;
            if (w == h) {
                clut.w = w;
                clut.h = 1;
                StoreImage(&clut, (u_long *)(D_80065648 - 0x20));
            }
        }
    }
    base = CardBuffer;
    q = (s32 *)(base + 0x200);
    end = (s32 *)(base + 0x850);
    sum = 0;
    while (q < end) {
        sum += *q++;
    }
    D_80065E24 = sum;
}

void func_8002C894(void) {
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
            CardSlots[i].b[0] = 0;
            CardSlots[i].b[1] = 0;
            CardSlots[i].b[2] = 0;
            CardSlots[i].b[3] = 0;
            CardSlots[i].b[4] = 0;
            CardSlots[i].b[5] = 0;
            CardSlots[i].b[6] = 0;
            CardSlots[i].b[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = 0x850;
    *(s32 *)(CardBuffer + new_var) = 1;
    i = 7;
    p = CardBuffer + i;
    do {
        p[0x854] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)(CardBuffer + new_var);
    q = (s32 *)(CardBuffer + 0x200);
    sum = 0;
    while (q < end) {
        sum += *q++;
    }
    D_80065E24 = sum;
}

const char CardSaveTitleFormat[] = "CRASH BASH %000 %000 %000 %000";

const char CardFileName[] = "BASCUS-94570";

void func_8002C97C(void) {
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

    if (MemCardSync(1, &D_8006782C, &D_80067830) == 0) {
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
    switch (D_80067830) {
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
    s = D_80067830;
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
    if (MemCardGetDirentry(CardChannel, (char *)all,(struct DIRENTRY *)D_800675D4, &D_800675D0, 0, 0xF) != 0) {
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
            func_8002C5BC();
            MemCardWriteFile(CardChannel, CardFileName, (u32 *)CardBuffer, 0, 0x2000);
            CardCommand = 5;
            return;
        }
        break;
    }
    goto lend;
l4:
    s = D_80067830;
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
        if (D_80067830 == 0) {
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
    if (D_80067830 == 0) {
        p = CardBuffer;
        q = (s32 *)(p + 0x200);
        end = (s32 *)(p + 0x850);
        sum = 0;
        while (q < end) {
            sum += *q++;
        }
        if (*(s32 *)(p + 0x85C) == sum) {
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

s32 func_8002D094(void) {
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

void func_8002D134(s32 arg0) {
    card_command_delay = 1;
    CardRequest = arg0;
    CardStatus |= 0x80;
}

void func_8002D15C(void) {
    CardRequest = 0;
    CardCommand = 0;
    CardStatus = 0;
}

s32 func_8002D178(void) {
    SizeEntry *p;
    s32 i;
    s32 sum;

    i = D_800675D0;
    if (i >= 0xF) {
        return 0;
    }
    p = D_800675D4;
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

SizeEntry *func_8002D1D4(void) {
    SizeEntry *p;
    s32 i;

    i = D_800675D0;
    p = D_800675D4;
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

void func_8002D254(void) {
    MemCardStop();
}

void func_8002D274(void) {
    func_8002C97C();
}

void func_8002D294(void) {
    D_800657C8_Slot *s;
    u8 *p;
    s32 i;
    s32 *q;
    s32 *end;
    s32 sum;
    int new_var;

    i = 0;
    for (;;) {
        if (i < 4) {
            CardSlots[i].b[0] = 0;
            CardSlots[i].b[1] = 0;
            CardSlots[i].b[2] = 0;
            CardSlots[i].b[3] = 0;
            CardSlots[i].b[4] = 0;
            CardSlots[i].b[5] = 0;
            CardSlots[i].b[6] = 0;
            CardSlots[i].b[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = 0x850;
    *(s32 *)(CardBuffer + new_var) = 1;
    i = 7;
    p = CardBuffer + i;
    do {
        p[0x854] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)(CardBuffer + new_var);
    q = (s32 *)(CardBuffer + 0x200);
    sum = 0;
    while (q < end) {
        sum += *q++;
    }
    D_80065E24 = sum;
}

void func_8002D33C(u8 *arg0) {
    (*(s8*)((u8*)(arg0) + 0)) = 0;
    (*(s8*)((u8*)(arg0) + 1)) = 0;
    (*(s8*)((u8*)(arg0) + 2)) = 0;
    (*(s8*)((u8*)(arg0) + 3)) = 0;
    (*(s8*)((u8*)(arg0) + 4)) = 0;
    (*(s8*)((u8*)(arg0) + 5)) = 0;
    (*(s8*)((u8*)(arg0) + 6)) = 0;
    (*(s8*)((u8*)(arg0) + 7)) = 0;
}

s32 func_8002D360(u8 *arg0) {
    u32 *p;
    u32 *end;
    s32 sum;

    p = (u32 *)(arg0 + 0x200);
    end = (u32 *)(arg0 + 0x850);
    sum = 0;
    while (p < end) {
        sum += *p++;
    }
    return sum;
}


void func_8002D394(u8 *arg0, s16 *arg1) {
    s16 *end;
    s16 fill;
    s32 i;
    s32 done;
    u8 c;
    s32 r;
    s32 r2;

    r2 = func_8002C4B0(0x20);
    fill = ((r2 & 0xFF) << 8) | ((u32)(func_8002C4B0(0x20) & 0xFFFF) >> 8);
    i = 0;
    done = 0;
    end = arg1 + 0x20;
    do {
        if (done == 0) {
            c = arg0[i];
            i++;
            if (c >= 0x20U) {
                r = func_8002C4B0(c);
                *arg1 = ((r & 0xFF) << 8) | ((u32)(func_8002C4B0(c) & 0xFFFF) >> 8);
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
char *D_800655BC = (void *)&CardSaveTitleFormat;
s32 CardStatus = 0;
s32 CardRequest = 0;
u8 CardBuffer[128] = {0};
u8 D_80065648[384] = {0};
D_800657C8_Slot CardSlots[4] = {0};
s32 D_80065E18[3] = {0};
s32 D_80065E24 = 0;
s32 D_80065E28[1512] = {0};
s32 card_command_delay = 0;
s32 CardCommand = 0;
s32 D_800675D0 = 0;
SizeEntry D_800675D4[15] = {0};
s32 D_8006782C = 0;
s32 D_80067830 = 0;
s32 D_80067834[2] = { 0x1C7350, 0x3200000 };
