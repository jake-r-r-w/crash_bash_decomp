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

    ((McHeader *)D_800655C8)->magic = 0x4353;
    ((McHeader *)D_800655C8)->frames = 2;
    ((McHeader *)D_800655C8)->type = 1;
    ((McHeader *)D_800655C8)->blocks = 1;
    k = 0x1B;
    p = D_800655C8 + k;
    do {
        p[0x44] = 0;
        k--;
        p--;
    } while (k >= 0);
    func_8001517C(D_800655BC + 0xC, func_80010000(D_800655C8 + 0x208), 100);
    func_8001517C(D_800655BC + 0x11, func_80010000(D_800655C8 + 0x208 + 0x194), 100);
    func_8001517C(D_800655BC + 0x16, func_80010000(D_800655C8 + 0x208 + 0x194 * 2), 100);
    func_8001517C(D_800655BC + 0x1B, func_80010000(D_800655C8 + 0x208 + 0x194 * 3), 100);
    McSetTitle(D_8004CD8C, ((McHeader *)D_800655C8)->title);
    rect = (RECT *)(func_800160F8(D_80055684, 0x306B) + 0x18);
    if (rect->w == 4) {
        h = rect->h;
        if (h == 0x10) {
            StoreImage(rect, (u_long *)D_80065648);
            img = (u8 *)func_800160F8(D_80055684, 0x306C);
            StoreImage((RECT *)(img + 0x18), (u_long *)(D_80065648 + 0x80));
            pal = func_800160C4(D_80055684, *(u16 *)(img + 2));
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
    base = D_800655C8;
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
    D_800655B8 = 0;
    D_800655C0 = 0;
    D_800675CC = 0;
    D_800655C4 = 0;
    for (;;) {
        if (i < 4) {
            D_800657C8[i].b[0] = 0;
            D_800657C8[i].b[1] = 0;
            D_800657C8[i].b[2] = 0;
            D_800657C8[i].b[3] = 0;
            D_800657C8[i].b[4] = 0;
            D_800657C8[i].b[5] = 0;
            D_800657C8[i].b[6] = 0;
            D_800657C8[i].b[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = 0x850;
    *(s32 *)(D_800655C8 + new_var) = 1;
    i = 7;
    p = D_800655C8 + i;
    do {
        p[0x854] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)(D_800655C8 + new_var);
    q = (s32 *)(D_800655C8 + 0x200);
    sum = 0;
    while (q < end) {
        sum += *q++;
    }
    D_80065E24 = sum;
}

INCLUDE_RODATA("asm/nonmatchings/main/card", D_8004CD8C);

INCLUDE_RODATA("asm/nonmatchings/main/card", D_8004CDAC);

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
    s = D_800675CC;
    if (s < 8) {
        goto *tbl[s];
    }
    goto lend;
l0:
    MemCardExist(D_800655B8);
    i = 1;
    D_800655C0 = 0;
    D_800675CC = i;
    return;
l1:
    switch (D_80067830) {
    case 1:
        D_800675CC = 0;
        D_800655C0 = 0;
        return;
    case 0:
    case 2:
    case 3:
    case 4:
        MemCardAccept(D_800655B8);
        D_800655C0 = 0x42;
        D_800675CC = 2;
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
    D_800675CC = 0;
    D_800655C0 = 0;
    return;
l2_4:
    D_800655C0 = 6;
    D_800675CC = 3;
    goto lend;
l2_03:
    if (MemCardGetDirentry(D_800655B8, (char *)all,(struct DIRENTRY *)D_800675D4, &D_800675D0, 0, 0xF) != 0) {
        D_800675CC = 0;
    }
    if (McFindSave() != NULL) {
        McClearSave();
        MemCardReadFile(D_800655B8, D_8004CDAC, (u32 *)D_800655C8, 0, 0x2000);
        D_800675CC = 7;
        goto lend;
    }
    McResetSave();
    D_800655C0 = 0;
    if (McFreeBytes() >= 0x2000) {
        D_800655C0 |= 0x20;
    }
    D_800655C0 |= 2;
    D_800675CC = 3;
    return;
l3:
    if (D_800655C4 == 0 || card_command_delay != 0) {
        card_command_delay--;
        MemCardExist(D_800655B8);
        D_800675CC = 4;
        goto lend;
    }
    switch (D_800655C4) {
    case 2:
        McResetSaveSum();
        if (MemCardFormat(D_800655B8) == 0) {
            D_800655C0 = 0x22;
            D_800675CC = 3;
            D_800655C4 = 0;
            return;
        }
        D_800655C0 |= 0x100;
        D_800675CC = 6;
        return;
    case 1:
        if (!(D_800655C0 & 0x10) && MemCardCreateFile(D_800655B8, D_8004CDAC, 1) != 0) {
            D_800675CC = 6;
            D_800655C0 |= 0x100;
        }
        if (D_800675CC != 6) {
            func_8002C5BC();
            MemCardWriteFile(D_800655B8, D_8004CDAC, (u32 *)D_800655C8, 0, 0x2000);
            D_800675CC = 5;
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
    D_800655C0 = 0;
    D_800675CC = 0;
    return;
l5:
    if (D_800655C4 == 1) {
        if (D_80067830 == 0) {
            D_800655C0 = 0x12;
            D_800675CC = 3;
            D_800655C4 = 0;
            return;
        }
        D_800655C0 |= 0x100;
        D_800675CC = 6;
        return;
    }
    goto lend;
l7:
    if (D_80067830 == 0) {
        p = D_800655C8;
        q = (s32 *)(p + 0x200);
        end = (s32 *)(p + 0x850);
        sum = 0;
        while (q < end) {
            sum += *q++;
        }
        if (*(s32 *)(p + 0x85C) == sum) {
            D_800675CC = 3;
            D_800655C0 = 0x12;
            return;
        }
    }
l2_2:
    D_800655C0 = 0xA;
l4_024:
    D_800675CC = 3;
lend:;
}

s32 func_8002D094(void) {
    if (D_800655C4 == 1 && (D_800655C0 & 0x80)) {
        return 1;
    }
    return 0;
}

void card_save(void) {
    s32 flags;
    s32 on;

    flags = D_800655C0;
    if ((flags & 2) && (flags & 0x30)) {
        on = TRUE;
        card_command_delay = on;
        D_800655C4 = on;
        D_800655C0 = flags | 0x80;
    }
}

void func_8002D108(void) {
    s32 mode = 2;

    card_command_delay = 1;
    D_800655C4 = mode;
    D_800655C0 |= 0x80;
}

void func_8002D134(s32 arg0) {
    card_command_delay = 1;
    D_800655C4 = arg0;
    D_800655C0 |= 0x80;
}

void func_8002D15C(void) {
    D_800655C4 = 0;
    D_800675CC = 0;
    D_800655C0 = 0;
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
            if (strcmp(D_8004CDAC, (char *)p) == 0) {
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
            D_800657C8[i].b[0] = 0;
            D_800657C8[i].b[1] = 0;
            D_800657C8[i].b[2] = 0;
            D_800657C8[i].b[3] = 0;
            D_800657C8[i].b[4] = 0;
            D_800657C8[i].b[5] = 0;
            D_800657C8[i].b[6] = 0;
            D_800657C8[i].b[7] = 0;
            i++;
        } else {
            break;
        }
    }
    new_var = 0x850;
    *(s32 *)(D_800655C8 + new_var) = 1;
    i = 7;
    p = D_800655C8 + i;
    do {
        p[0x854] = 0;
        i--;
        p--;
    } while (i >= 0);
    end = (s32 *)(D_800655C8 + new_var);
    q = (s32 *)(D_800655C8 + 0x200);
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
