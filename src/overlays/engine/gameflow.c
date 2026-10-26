#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/gameflow.h"
#include "bash/hero.h"
#include "bash/shell.h"

void func_8008E27C(s32 *arg0, HitInfo *arg1, s32 arg2) {
    s32 id;
    s32 obj;

    obj = ((s32 *)arg1)[-1];
    id = arg2 != 0 ? arg1->unk16 : -1;
    switch (arg1->unk14) {
    case 5:
    case 7:
        PlaySfx(HitSfx[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        func_8007A524(1, arg0, (s32)CrateObjectsModel);
        func_8008DEC8(obj, 0x280, id, 0x208, 0xA0);
        break;
    case 6:
        PlaySfx(((HitSfxTable *)HitSfx)->hit6[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        func_8007A524(0, arg0, (s32)CrateObjectsModel);
        func_8008DEC8(obj, 0x280, id, 0x208, 0);
        break;
    case 2:
        PlaySfx(((HitSfxTable *)HitSfx)->crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        func_8007A524(2, arg0, (s32)CrateObjectsModel);
        break;
    case 3:
        PlaySfx(((HitSfxTable *)HitSfx)->crate[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        func_8007A524(4, arg0, (s32)CrateObjectsModel);
        func_8008DEC8(obj, 0x280, id, 0x203, 0xA0);
        break;
    default:
        PlaySfx(((HitSfxTable *)HitSfx)->other[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        func_8007A524(3, arg0, (s32)CrateObjectsModel);
        func_8008DEC8(obj, 0x280, id, 0x205, 0xA0);
        break;
    }
    if (HitHooks.unk8 != NULL) {
        HitHooks.unk8(arg0[0], arg0[1], arg0[2], 0, id, arg1->unk14);
    }
}

void InitCrateExplode(NAS *arg0) {
    EXTengine *e = arg0->Engine;
    u8 *t;

    if (e != NULL) {
        t = (u8 *)HitSfx;
        PlaySfx(*(s32 *)(t + Random(4) * 4 + 0x30), NULL, NULL, 0x1000, 0x1E00, 0);
        func_8007A524(2, (u8 *)e + 0x10, CrateObjectsModel);
    }
    KillNAS(arg0);
}

void func_8008E5BC(void) {
    IntroStep *e;
    IntroStep *tbl;
    s32 type;
    u8 *a;
    u8 *b;

    GameFrameCount++;
    if ((Pads[0].held & 0x8C08) == 0x8C08) {
        func_800101E0();
    }
    scripts_update();
    if (D_8009E5B4 != 0) {
        if (--D_8009E5B4 == 0) {
            PlaySfx(D_8009E5B0, &D_8009E5C4, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if (D_800AEC58 != 0) {
        return;
    }
    if (D_800AEC5C != 0) {
        func_80016F54((u8 *)D_800AEC5C);
        D_800AEC5C = 0;
    }
    if (IntroStepIndex == 1 && D_8009E5A8 == IntroSteps[0].delay) {
        func_8002D4F4(0);
    }
    if (D_8009E5A8 < 0) {
        return;
    }
    if (--D_8009E5A8 > 0) {
        return;
    }
    if (func_80012FFC() != 0) {
        D_8009E5A8 = 1;
        return;
    }
    if ((u32)IntroStepIndex >= 4) {
        ProcessControl.flags = 0x12;
        ProcessControl.next_process = &D_800A00DC;
        D_8009E5A8 = -1;
        return;
    }
    tbl = IntroSteps;
    e = &tbl[IntroStepIndex];
    type = e->type;
    switch (type) {
    case 1:
        D_8009E5A8 = e->delay;
        a = (u8 *)load_file_permanent(e->pic);
        b = tga_convert(a, ScreenRect->w, ScreenRect->h);
        mal_free(a, &MainHeap);
        LoadImage(ScreenRect, (u32 *)b);
        LoadImage(ScreenRect + 1, (u32 *)b);
        DrawSync(0);
        mal_free(b, &MainHeap);
        if (IntroSteps[IntroStepIndex].sfx[0] != -1) {
            PlaySfx(IntroSteps[IntroStepIndex].sfx[0], &IntroSfx[0], NULL, 0x1000, 0x1E00, 0);
            PlaySfx(IntroSteps[IntroStepIndex].sfx[1], &IntroSfx[1], NULL, 0x1000, 0x1E00, 0);
            PlaySfx(IntroSteps[IntroStepIndex].sfx[2], &IntroSfx[2], NULL, 0x1000, 0x1E00, 0);
            D_8009E5B4 = 6;
            D_8009E5B0 = IntroSteps[IntroStepIndex].sfxNext;
        }
        break;
    case 2:
        D_8009E5A8 = e->delay;
        func_80095258(e->model);
        D_80058B64 = 0x10000;
        D_8004E0E0 = type;
        D_8009E5AC = func_8001FE80(IntroSteps[IntroStepIndex].model, IntroSteps[IntroStepIndex].unkC,
                                   &D_8009E560, &D_8009E580, 4, (u8 **)&D_800AEC58);
        if (IntroSteps[IntroStepIndex].sfx[0] != -1) {
            PlaySfx(IntroSteps[IntroStepIndex].sfx[0], &IntroSfx[0], NULL, 0x1000, 0x1E00, 0);
            PlaySfx(IntroSteps[IntroStepIndex].sfx[1], &IntroSfx[1], NULL, 0x1000, 0x1E00, 0);
            PlaySfx(IntroSteps[IntroStepIndex].sfx[2], &IntroSfx[2], NULL, 0x1000, 0x1E00, 0);
            D_8009E5B4 = 0x16;
            D_8009E5B0 = IntroSteps[IntroStepIndex].sfxNext;
        }
        if (IntroSteps[IntroStepIndex].unk10 != -1) {
            func_8002C284(IntroSteps[IntroStepIndex].unk10);
        }
        if (D_800AEC5C != 0) {
            func_80016F54((u8 *)D_800AEC5C);
        }
        D_800AEC5C = (s32)IntroSteps[IntroStepIndex].model;
        break;
    }
    IntroStepIndex++;
    if (IntroSteps[IntroStepIndex].type == 2) {
        func_8001612C(IntroSteps[IntroStepIndex].model, 0);
    }
}

void func_8008EAC8(void) {
    s32 f;
    RECT *a;
    RECT *r;

    if (D_800AEC5C != 0) {
        func_80016F54((u8 *)D_800AEC5C);
    }
    func_80021FB0();
    func_8001D604();
    f = FrameCount;
    a = &ScreenRect[f & 1];
    r = &ScreenRect[(f + 1) & 1];
    MoveImage2(a, r->x, r->y);
}

void func_8008EB3C(void) {
    func_80018B08(D_8009E498, MainCamera);
    if (D_800AEC5C != 0) {
        func_8009440C();
    }
    scripts_render();
}

void func_8008EB8C(void) {
    IntroStepIndex = 0;
    D_8009E5A8 = 0;
    D_800AEC58 = 0;
    D_800AEC5C = 0;
    func_8001E984(0x20, 0x80);
    D_8009E5B0 = -1;
    D_8009E5B4 = 0;
    func_8001D640(0x200);
    cam_init(MainCamera);
    FirstBootDone = 1;
}

void func_8008EC00(void) {
    u8 *a;
    u8 *b;
    s32 load_file_permanent();

    a = (u8 *)load_file_permanent();
    b = tga_convert(a, ScreenRect->w, ScreenRect->h);
    mal_free(a, &MainHeap);
    LoadImage(ScreenRect, (u32 *)b);
    LoadImage(ScreenRect + 1, (u32 *)b);
    DrawSync(0);
    mal_free(b, &MainHeap);
}

void func_8008EC94(void) {
    s32 f = FrameCount;
    RECT *a = &ScreenRect[f & 1];
    RECT *r = &ScreenRect[(f + 1) & 1];

    MoveImage2(a, r->x, r->y);
}

void func_8008ECE4(void) {
    EngineModeView *g = (EngineModeView *)GameInfo;
    s32 msg;
    s32 res;
    u16 flags;
    u16 w;

    if (!(g->unk30 & 0x8000)) {
        return;
    }
    g->unk50[0] = g->unk4C[0];
    g->unk50[1] = g->unk4C[1];
    g->unk50[2] = g->unk4C[2];
    g->unk50[3] = g->unk4C[3];
    g->unk58[0] = g->unk54[0];
    g->unk58[1] = g->unk54[1];
    g->unk58[2] = g->unk54[2];
    g->unk58[3] = g->unk54[3];
    if (g->unk32 & 0x20) {
        func_800793E8();
    }
    flags = g->unk30;
    if ((flags & 0x8008) == 0x8008) {
        g->mode = NextGameMode;
        res = func_800951D0((s16)NextGameMode);
        if (res == 0) {
            ProcessControl.next_process = &D_800A00DC;
            return;
        }
        func_80094D88(res, &D_800A00DC);
        return;
    }
    w = ModeTypes[g->mode * 3];
    if (w == 0x101) {
        return;
    }
    if (w == 0x102) {
        if (g->mode == 0x2B) {
            g->mode = 0x28;
            ProcessControl.next_process = &D_800A00DC;
            return;
        }
        if (flags == 0x8002) {
            g->mode = func_8008EF70();
        } else {
            g->mode = NextGameMode;
        }
        ProcessControl.next_process = &D_800A00DC;
        return;
    }
    if (flags & 0x200) {
        ProcessControl.next_process = &RetryProcess;
        return;
    }
    switch (g->mode) {
    case 0x1F:
        if (g->unk187 != 0 && (flags & 1)) {
            if (g->unk44 == 3) {
                g->mode = 0x20;
                func_80094D88((s32)D_800A0888, &D_800A00DC);
            } else if (g->unk44 == 1) {
                g->mode = 0x25;
                func_80094D88((s32)D_800A0858, &D_800A00DC);
            } else {
                g->mode = 0x25;
                func_80094D88((s32)D_800A0870, &D_800A00DC);
            }
            return;
        }
        GameMode = func_8008EF70();
        return;
    case 0x20:
        g->mode = 0x25;
        if (D_8004E0A4[g->unk60] == 1) {
            msg = (s32)D_800A0858;
        } else {
            msg = (s32)D_800A0870;
        }
        func_80094D88(msg, &D_800A00DC);
        return;
    }
    g = (EngineModeView *)GameInfo;
    switch (*(u8 *)&g->unk32) {
    case 2:
        if (g->unk30 & 1) {
            g->mode = func_8008FF94();
        } else {
            g->mode = 0x2A;
        }
        break;
    case 1:
        g->mode = 0x29;
        break;
    default:
        g->mode = func_8008EF70();
        break;
    }
    ProcessControl.next_process = &D_800A00DC;
}

s32 func_8008EF70(void) {
    return WarpRooms[GameWarpRoom][0];
}

void func_8008EF9C(void) {
    NextGameMode = func_8008EF70();
    GameExitFlags = 0x8008;
    func_8008ECE4();
}

void func_8008EFD4(void) {
    u8 *g = GameInfo;

    if (*(u16 *)(g + 0x30) & 0x8000) {
        if (ProcessControl.next_process == NULL) {
            if (*(u16 *)(g + 0x24) != 0) {
                *(u16 *)(g + 0x24) -= 1;
                if (*(u16 *)(g + 0x24) != 0) {
                    return;
                }
            }
            func_80092AF4(&D_800A00DC, 0x200);
            if (!(*(u16 *)(g + 0x30) & 0x200)) {
                func_8002C284(-1);
            }
        }
    }
}

void func_8008F068(void) {
    NextGameMode = 0x28;
    GameExitFlags = 0x8008;
}

void func_8008F084(void) {
    EngineEntry16Head *p;
    EngineTable12 *base;
    EngineTable12 *t;
    subengine *sub;
    TrigEntry *trig;
    s32 n;
    s32 i;
    u32 mask;

    i = 0;
    trig = TrigTable;
    base = CupTable;
    t = base + (u8)D_8005A63C;
    n = t->unk8;
    p = t->unk4;
loop:
    if (i < n) {
        sub = GetEngineSlot(i + 3)->sub;
        sub->unk6C = (void *)p->unk8;
        sub->unk74 = p->unkC;
        if (p->unk0 == 1 || (p->unk0 == 2 && LevelUnlockedIn(p->unk4, GameInfo) != 0)) {
            sub->flags |= 0x8000;
        } else {
            sub->flags &= ~0x8000;
        }
        p++;
        sub->unk10 = 0x7D;
        sub->unk12 = 0;
        sub->unk14 = 0;
        sub->unk04 = trig[(((GameFrameCount * 20 + (i << 10)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 128 >> 12;
        sub->unk08 = (trig[(((GameFrameCount * 70 + (i << 9)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
        i++;
        goto loop;
    }
    i = n;
    mask = ~0x8000;
loop2:
    if (i < 4) {
        GetEngineSlot(i + 3)->sub->flags &= mask;
        i++;
        goto loop2;
    }
}

s32 func_8008F248(s32 arg0) {
    s32 cnt;
    s32 i;
    u8 *tab;

    tab = D_800657D0;
    cnt = 0;
    i = 0;
    for (;;) {
    loop:
        if (i >= 7) {
            break;
        }
        if (CupTable[i].unk0 == 3 && !EngineTable_AllDone(&CupTable[i]) &&
            EngineTable_Reachable(&CupTable[i], tab)) {
            cnt++;
            if (arg0 != 0) {
                EngineTable_MarkDone(&CupTable[i]);
            }
        }
        i++;
    }
    return cnt;
}

void func_8008F41C(void) {
    u8 *s0;
    Sub3F0 *s1;
    s32 *tab;

    s0 = (u8 *)GetEngineSlot(0)->sub;
    s1 = (Sub3F0 *)(s0 + 0x3F0);
    if (CardStatus & 2) {
        if (CardStatus & 0x40) {
            s1->f6C = D_8009EA6C[GameLanguage];
            *(s32 *)(s0 + 0x3F0) |= 0x8000;
            return;
        } else if (CardStatus & 0x10) {
            if (func_8008F248(0) != 0) {
                s1->f6C = D_8009EA64[GameLanguage];
                *(s32 *)(s0 + 0x3F0) |= 0x8000;
                if (Pads[0].pressed & 0x2000) {
                    PlaySfx(0x396, NULL, NULL, 0x1000, 0x1E00, 0);
                    func_8008F248(1);
                }
                return;
            }
            s1->f6C = D_8009EA70[GameLanguage];
            *(s32 *)(s0 + 0x3F0) |= 0x8000;
            return;
        } else {
            goto def;
        }
    } else {
def:
        tab = D_8009EA68;
    }
    s1->f6C = tab[GameLanguage];
    *(s32 *)(s0 + 0x3F0) |= 0x8000;
}

void func_8008F554(void) {
    u8 *g;
    u8 *p;
    subengine *sub;
    s32 i;

    AnimateTitlesBackdrop();
    func_8008F084();
    if (D_8009EB98 != 0) {
        D_8009EB98--;
        func_80017640();
        func_80011D28((u8 *)&MainHeap);
    }
    if (Pads[0].pressed & 0x80) {
        EngineWorldPrev();
    }
    if (Pads[0].pressed & 0x20) {
        EngineWorldNext();
    }
    if (Pads[0].pressed & 0x1000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
    if (EngineTable_Unlocked(GameInfo[0x28]) && (Pads[0].pressed & 0x4000)) {
        g = GameInfo;
        ((EngineModeView *)g)->unk32 = (((EngineModeView *)g)->unk32 & 0xFF00) | 2;
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        g[0x29] = 0;
        NextGameMode = CupTable[g[0x28]].unk4->unk4;
        MenuChainNext();
        p = GameInfo;
        for (i = 0; i < 4; i++) {
            p[i + 0x9F] = 0;
            p[i + 0xA3] = 0;
        }
    }
    GetEngineSlot(0)->sub->unk6C = (void *)EngineCurWorldName();
    if (GameInfo[0x28] != 0) {
        GetEngineSlot(0)->sub[8].flags |= 0x8000;
    } else {
        GetEngineSlot(0)->sub[8].flags &= ~0x8000;
    }
    if ((u8)D_8005A63C < 6) {
        GetEngineSlot(0)->sub[9].flags |= 0x8000;
    } else {
        GetEngineSlot(0)->sub[9].flags &= ~0x8000;
    }
    if (EngineTable_Unlocked(GameInfo[0x28])) {
        GetEngineSlot(0)->sub[7].flags |= 0x8000;
        GetEngineSlot(0)->sub[4].flags &= ~0x8000;
        GetEngineSlot(0)->sub[3].flags &= ~0x8000;
        GetEngineSlot(0)->sub[5].flags &= ~0x8000;
        GetEngineSlot(0)->sub[6].flags &= ~0x8000;
        return;
    }
    GetEngineSlot(0)->sub[7].flags &= ~0x8000;
    GetEngineSlot(0)->sub[3].flags |= 0x8000;
    sub = &GetEngineSlot(0)->sub[4];
    sub->flags |= 0x02008000;
    sub->unk68 = -0x80;
    sub->unk6C = D_800555DC;
    sub->unk74 = 0x2006;
    sub->unk12 = FrameCount * 50;
    sub->unk10 = 0;
    sub->unk14 = TrigTable[(((GameFrameCount * 100) & 0xFFF) - 0x400) & 0xFFF].unk2 * 25 >> 9;
    sub->unk52 = 1;
    GetEngineSlot(0)->sub[5].flags |= 0x8000;
    func_8008F41C();
}

void func_8008FB48(void) {
    func_80018B08(&D_8009EA9C, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    func_8001C2C0((u8 *)GetEngineSlot(3));
    func_80018B08(&D_8009EAC4, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    func_8001C2C0((u8 *)GetEngineSlot(4));
    func_80018B08(&D_8009EAEC, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    func_8001C2C0((u8 *)GetEngineSlot(5));
    func_80018B08(&D_8009EB14, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    func_8001C2C0((u8 *)GetEngineSlot(6));
    RenderTitlesBackdropAndMenu();
    func_80018B08(&D_8009EA74, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    func_8001C2C0((u8 *)GetEngineSlot(2));
}

void func_8008FC58(void) {
}

s32 func_8008FC60(void *arg0) {
    s32 i;
    s32 j;
    s32 cnt;
    s32 obj;
    u8 *p;
    u8 *tab;

    i = 0;
    tab = D_800657D0;
    for (;;) {
        if (i >= *(s32 *)((u8 *)arg0 + 8)) {
            goto done;
        }
        obj = *(s32 *)((u8 *)(i * 16 + *(s32 *)((u8 *)arg0 + 4)) + 4);
        if (LevelUnlockedIn(obj, GameInfo) == 0) {
            cnt = 0;
            j = cnt;
            p = tab;
            do {
                if (LevelUnlockedIn(obj, p) != 0) {
                    cnt++;
                }
                j++;
                p += 0x194;
            } while (j < 4);
            if (cnt == 0) {
                return 0;
            }
        }
        i++;
    }
done:
    return 1;
}

void func_8008FD40(void *arg0) {
    s32 i;
    s32 n = *(s32 *)((u8 *)arg0 + 8);

    for (i = 0; i < n; i++) {
        u8 *e = *(u8 **)((u8 *)arg0 + 4);
        GameInfo[*(s32 *)(e + i * 16 + 4) + 0xE4] = 1;
        n = *(s32 *)((u8 *)arg0 + 8);
    }
}

void func_8008FD94(void) {
    u8 *g = GameInfo;
    s32 n;

    D_8009EB98 = 4;
    n = g[0x28];
    if (n < 6) {
        n++;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g[0x28] = n;
    }
}

void func_8008FE00(void) {
    u8 *g = GameInfo;
    s32 n;

    D_8009EB98 = 4;
    n = g[0x28];
    if (n > 0) {
        n--;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g[0x28] = n;
    }
}

s32 func_8008FE68(s32 arg0) {
    EngineTable12 *base = CupTable;
    EngineTable12 *t;
    EngineEntry16Head *p;
    s32 n;
    s32 i;

    t = base + arg0;
    if (t->unk0 == 1) {
        return 1;
    }
    if (t->unk0 == 2) {
        return 0;
    }
    n = t->unk8;
    p = t->unk4;
    for (i = 0; i < n; i++, p++) {
        if (!LevelUnlockedIn(p->unk4, GameInfo)) {
            return 0;
        }
    }
    return 1;
}

s32 func_8008FF10(void) {
    u16 *wt = ModeTypes;
    EngineTable12 *tab = CupTable;
    u8 *g = GameInfo;
    EngineEntry16Head *e = tab[g[0x28]].unk4;
    s32 idx = e[g[0x29]].unk4;
    s32 **tbl;
    s32 w = wt[idx * 3] - 0x101;
    s32 *t;
    tbl = (s32 **)D_8004DDBC;
    t = tbl[*(s32 *)(g + 0x2C)];
    return *(s32 *)((u8 *)t + w * 4);
}

s32 func_8008FF94(void) {
    if (GameInfo[0x29] < CupTable[GameInfo[0x28]].unk8 - 1) {
        GameInfo[0x29] = GameInfo[0x29] + 1;
        return CupTable[GameInfo[0x28]].unk4[GameInfo[0x29]].unk4;
    }
    return 0x2B;
}

s32 func_80090008(void) {
    return CupTable[GameInfo[0x28]].unk4[GameInfo[0x29]].unk4;
}

void func_80090048(void) {
    D_8009EB98 = 0;
    func_80095BEC(0, (s32 *)D_8009E8B4);
    func_80095BEC(2, D_8009E7F4);
    func_80095BEC(3, (s32 *)D_8009EB3C);
    func_80095BEC(4, (s32 *)D_8009EB3C);
    func_80095BEC(5, (s32 *)D_8009EB3C);
    func_80095BEC(6, (s32 *)D_8009EB3C);
    D_8005A63C = 0;
}

static inline s32 func_800900CC_find(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = WarpRooms[0]; (u32)i < 10; i++, p += 5) {
        if (*p == arg0) {
            return i;
        }
    }
    return -1;
}

s32 func_800900CC(void) {
    s32 i;
    s32 j;
    s32 best;
    s32 *t;

    best = -1;
    i = func_800900CC_find(GameMode);
    if (i < 0) {
        t = 0;
    } else {
        t = WarpRooms[i];
    }
    if (t == 0) {
        return best;
    }
    for (j = 0; j < t[2]; j++) {
        if (((s32(*)(s32))func_8007A6D0)(((u16 *)(t[3] + j * 8))[2]) != 0 && best < j) {
            best = j;
        }
    }
    return best;
}

static inline s32 func_800901B4_find(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = WarpRooms[0]; (u32)i < 10; i++, p += 5) {
        if (*p == arg0) {
            return i;
        }
    }
    return -1;
}

s32 *func_800901B4(s32 arg0) {
    s32 i = func_800901B4_find(arg0);

    if (i < 0) {
        return 0;
    }
    return WarpRooms[i];
}

s32 func_8009020C(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = WarpRooms[0]; (u32)i < 10; i++, p += 5) {
        if (*p == arg0) {
            return i;
        }
    }
    return -1;
}

s32 func_80090248(void) {
    EngSub90248 *p;

    if (GameGateway < 0 || (p = HubGateways[GameGateway].unk14, p->unk2 != 2)) {
        return -1;
    }
    return p->unk4;
}

void func_8009029C(void) {
    s32 i;
    EngEntry *e;
    EngTable *t = (EngTable *)WarpRooms;

    for (i = 9; i != -1; i--) {
        e = t->entries;
        while (e->a != 0) {
            e->flags &= ~8;
            e++;
        }
        t++;
    }
    D_8009EE9C.unk0 = 0;
    D_8009EE9C.unk4 = 0;
    D_8009EE9C.unk8 = 0x400;
}

void func_80090310(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    EXTengine *e;
    s32 n;
    s32 d;
    s32 r;

    func_80095BEC(arg1 + 4, D_8009F298);
    e = GetEngineSlot(arg1 + 4);
    e->unk10.x = arg3;
    e->unk10.y = arg4;
    e->flags |= 0x10000000;
    if (arg2 != 0) {
        e->sub[3].unk74 = 0;
        e->sub[4].unk74 = 0;
        e->sub[5].unk74 = 0;
        e->sub[6].unk74 = 0;
        e->sub[2].unk6C = NULL;
        e->sub[1].unk6C = D_8009F004[GameLanguage];
        e->sub[1].unk04 = 0x8096;
        e->sub[1].unk08 = 0x1E;
        return;
    }
    e->sub[3].unk74 = IconImage[arg0[0x4C]];
    e->sub[4].unk74 = IconImage[arg0[0x4D]];
    e->sub[5].unk74 = IconImage[arg0[0x4E]];
    e->sub[6].unk74 = IconImage[arg0[0x4F]];
    e->sub[1].unk6C = arg0 + 0x18;
    e->sub[1].unk04 = 0xC;
    e->sub[1].unk08 = 2;
    n = func_80010000(arg0);
    D_8009F4A8[arg1][0] = n / 100 + '0';
    n %= 100;
    d = n / 10;
    r = n - d * 10;
    d += '0';
    r += '0';
    D_8009F4A8[arg1][1] = d;
    D_8009F4A8[arg1][2] = r;
    D_8009F4A8[arg1][3] = '%';
    D_8009F4A8[arg1][4] = 0;
    e->sub[2].unk6C = D_8009F4A8[arg1];
}

void func_80090564(u8 *arg0, s32 arg1) {
    EXTengine *e;

    e = GetEngineSlot(3);
    e->unk10.x = 0x28;
    e->unk10.y = 0xDA;
    e->flags |= 0x10000000;
    if (arg1 != 0) {
        EngineSetSlot3Digits(3, 3);
        return;
    }
    func_800150E8(D_8009F498, func_8007A5E8(arg0), 10);
    e->sub[2].unk6C = D_8009F498;
    func_800150E8(D_8009F498 + 3, EngineSum147(arg0), 10);
    e->sub[6].unk6C = D_8009F498 + 3;
    func_800150E8(D_8009F49E, EngineSum126(arg0), 10);
    e->sub[4].unk6C = D_8009F49E;
    func_800150E8(D_8009F4A1, EngineCount105(arg0), 10);
    e->sub[8].unk6C = D_8009F4A1;
    func_8001517C(D_8009F4A4, EngineExtra105(arg0), 10);
    e->sub[10].unk6C = D_8009F4A4;
    switch (*(s32 *)(arg0 + 0xE0)) {
    case 1:
        EngineSetSlot3Digits(3, 1);
        break;
    case 2:
        EngineSetSlot3Digits(3, 2);
        break;
    default:
        EngineSetSlot3Digits(3, 0);
        break;
    }
}

void func_80090A10(void) {
    PadState *pad;
    s32 idx;
    EXTengine *e;
    u8 *g;
    u8 *src;
    u8 *p;

    FadeAmount = ScreenFade.amount;
    switch (D_8009F638) {
    case 0:
        pad = Pads;
        if (*(s32 *)((u8 *)pad + 0x48) & 0x1000) {
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009F63C = 1;
            func_80092AF4(&RetryProcess, 0x200);
            {
                u8 *q = GameInfo;
                *(u16 *)(q + 0x30) |= 0x200;
            }
        }
        if (CardStatus & 2) {
            if (CardStatus & 0x40) {
                e = GetEngineSlot(1);
                e->sub[1].unk6C = D_8009EFCC[*(s32 *)((g = GameInfo) + 0x2C)];
                GetEngineSlot(1)->sub->unk6C = D_8009EFD8[*(s32 *)(g + 0x2C)];
                func_8001C2C0((u8 *)GetEngineSlot(1));
                return;
            } else if (CardStatus & 0x10) {
                func_8009196C();
                idx = CardSlotIndex * 0x194;
                p = (u8 *)CardSlots;
                if (*(s32 *)(idx + p) != 0
                    && (*(s32 *)((u8 *)pad + 0x48) & 0x4000)) {
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                    func_8002C284(-1);
                    src = CardSlotIndex * 0x194 + p;
                    {
                        u8 *q = GameInfo;
                        q[0x4C] = src[0];
                        q[0x4D] = src[1];
                        q[0x4E] = src[2];
                        q[0x4F] = src[3];
                        q[0x5C] = src[4];
                        q[0x5D] = src[5];
                        q[0x5E] = src[6];
                        q[0x5F] = src[7];
                    }
                    *(Blk18C *)GameInfo = *(Blk18C *)(src + 8);
                    {
                        Blk12p4 *d = (Blk12p4 *)GameOptions;
                        u8 *s = GameInfo;
                        *d = *(Blk12p4 *)(s + 0xD0);
                    }
                    func_80091E58();
                    func_80092AF4(&D_800A00DC, 0x200);
                    D_8009F638 = 3;
                }
                return;
            } else {
                if (CardStatus & 4) {
                    GetEngineSlot(1)->sub[1].unk6C = D_8009EFD0[GameLanguage];
                } else {
                    GetEngineSlot(1)->sub[1].unk6C = D_8009EFD4[GameLanguage];
                }
                GetEngineSlot(1)->sub->unk6C = D_8009EFD8[GameLanguage];
                func_8001C2C0((u8 *)GetEngineSlot(1));
                return;
            }
        }
        e = GetEngineSlot(1);
        {
            u8 *q;
            e->sub[1].unk6C = D_8009EFDC[*(s32 *)((q = GameInfo) + 0x2C)];
            GetEngineSlot(1)->sub->unk6C = D_8009EFD8[*(s32 *)(q + 0x2C)];
        }
        func_8001C2C0((u8 *)GetEngineSlot(1));
        break;
    case 1:
        if (func_80012FFC() == 0) {
            u8 *q = GameInfo;
            D_8009F638 = 2;
            q[0x50] = q[0x4C];
        }
        break;
    case 2:
        if (func_80012FFC() == 0) {
            u8 *q;
            func_80092AF4(&RetryProcess, 0x200);
            q = GameInfo;
            D_8009F638 = 3;
            *(u16 *)(q + 0x30) |= 0x200;
        }
        break;
    }
}

const char MenuTournamentSelect[] = "TOURNAMENT SELECT";

const char MenuLocked[] = "LOCKED";

const char MenuPressButtonToSelect[] = "PRESS \374 BUTTON TO SELECT";

const char MenuInsertCardForUnlocked[] = "INSERT A MEMORY CARD\nWITH CRASH BASH DATA\nTO CHECK FOR \nUNLOCKED LEVELS";

const char MenuArrowLeft[] = "<";

const char MenuArrowRight[] = ">";

const char MenuUnlockWithCard[] = "PRESS \375BUTTON TO\nUNLOCK LOCKED LEVELS\nUSING MEMORY CARD IN\nMEMORY CARD SLOT 1";

const char MenuCheckingUnlocked[] = "CHECKING MEMORY CARD\nIN MEMORY CARD SLOT 1\nFOR UNLOCKED\nLEVELS";

const char MenuThereAreNoMoreUnlockedLevels[] = "THERE ARE NO MORE\nUNLOCKED LEVELS\nON THIS MEMORY CARD\n";

const char MenuConfirmOverwrite[] = "\01508CONFIRM OVERWRITE\n\nPRESS \377 TO SAVE\01525PRESS \376 TO EXIT\01517WITHOUT SAVING";

const char MenuPressButtonToOverwrite[] = "PRESS \374 BUTTON TO OVERWRITE";

const char MenuPressButtonToLoad[] = "PRESS \374 BUTTON TO LOAD";

const char MenuPressButtonToSave[] = "PRESS \374 BUTTON TO SAVE";

const char MenuPressButtonToExitWithoutSaving[] = "PRESS \376 BUTTON TO EXIT WITHOUT SAVING";

const char MenuSelectAGameSave[] = "SELECT A GAME SAVE";

const char MenuSelectGameToLoad[] = "SELECT GAME TO LOAD";

const char MenuCheckingCard[] = "\nCHECKING MEMORY CARD\nIN MEMORY CARD SLOT 1\nPLEASE WAIT";

const char MenuCardUnformatted[] = "WARNING\01530MEMORY CARD IS UNFORMATTED\nINSERT FORMATTED MEMORY CARD\nINTO MEMORY CARD SLOT 1\n\01535PRESS \376 BUTTON TO EXIT";

const char MenuNoCrashBashData[] = "WARNING\01525THERE IS NO CRASH BASH\nDATA ON THIS MEMORY CARD\nPLEASE INSERT ANOTHER\nMEMORY CARD\n\nPRESS \376 BUTTON TO EXIT";

const char MenuLoadGame[] = "LOAD GAME";

const char MenuNoCard[] = "WARNING\01530THERE IS NO MEMORY CARD\nIN MEMORY CARD SLOT 1\nPLEASE INSERT A MEMORY CARD\n\01535PRESS \376 BUTTON TO EXIT";

const char MenuFormatCard[] = "WARNING\01525MEMORY CARD IS UNFORMATTED\nDO YOU WANT TO FORMAT ?\01525PRESS \377 BUTTON TO FORMAT\nPRESS \376 BUTTON TO EXIT\nWITHOUT FORMATTING";

const char MenuSaveGame[] = "SAVE GAME";

const char MenuCardFull[] = "WARNING\01520THE MEMORY CARD IN MEMORY\01516CARD SLOT 1 HAS\01516INSUFFICIENT SPACE\01520PLEASE INSERT A MEMORY CARD\01516CONTAINING AT LEAST\015161 FREE BLOCK\01520PRESS \376 BUTTON TO EXIT";

const char MenuNoCard2[] = "WARNING\01530THERE IS NO MEMORY CARD\nIN MEMORY CARD SLOT 1\nPLEASE INSERT A MEMORY CARD\01530PRESS \376 BUTTON TO\nEXIT WITHOUT SAVING";

const char MenuSaving[] = "\01508SAVING\n\nPLEASE DO NOT REMOVE\nTHE MEMORY CARD";

const char MenuSaveCompleted[] = "\01508SAVE COMPLETED\n\nPRESS \374 BUTTON\nTO CONTINUE";

const char MenuSaveFailedPressButtonToExit[] = "\nSAVE FAILED\n\nPRESS \376 BUTTON TO EXIT";

const char MenuFormatting[] = "\01508FORMATTING\n\nPLEASE DO NOT REMOVE\nTHE MEMORY CARD\n";

const char MenuPressButtonToExitWithoutLoading[] = "PRESS \376 BUTTON TO EXIT WITHOUT LOADING";

const char MenuEmpty[] = "EMPTY";

const char Menu01[] = "01";

const char Menu02[] = "02";

const char Menu03[] = "03";

const char Menu04[] = "04";

const char Menu05[] = "05";

const char MenuNameWidth[] = "WWWWWWWW";

const char Menu200Percent[] = "200%";

const char MenuCardCorrupt[] = "WARNING\nTHE MEMORY CARD IN\nMEMORY CARD SLOT 1\nIS CORRUPT. PLEASE INSERT\nANOTHER MEMORY CARD\nPRESS \376 BUTTON TO EXIT\nWITHOUT SAVING";
const char MenuEmpty2[] = "";

void func_80090E20(void) {
    s32 f;

    FadeAmount = ScreenFade.amount;
    switch (SaveMenuState) {
    case 0:
        f = CardStatus;
        if (f & 2) {
            if (f & 0xC) {
                if (f & 4) {
                    SAVEMENU_SET_ANIM(D_8009EFE0);
                    if (Pads[0].pressed & 0x8000) {
                        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                        func_8002D108();
                        SaveMenuState = 6;
                    }
                } else {
                    SaveMenuState = 8;
                }
            } else if (f & 0x40) {
                SAVEMENU_SET_ANIM(D_8009EFCC);
            } else if (f & 0x30) {
                if (!(f & 0x10)) {
                    func_8002D294();
                }
                func_8009196C();
                if (Pads[0].pressed & 0x4000) {
                    s32 idx = CardSlotIndex * 0x194;
                    u8 *p = (u8 *)CardSlots;
                    if (*(s32 *)(idx + p) == 0) {
                        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                        if (HubSaveMenuHooks.unk0 != NULL) {
                            HubSaveMenuHooks.unk0();
                        }
                        SaveMenuState = 3;
                    } else {
                        SaveMenuState = 7;
                    }
                }
            } else {
                SAVEMENU_SET_ANIM(D_8009EFE8);
            }
        } else {
            SAVEMENU_SET_ANIM(D_8009EFEC);
        }
        if (Pads[0].pressed & 0x1000) {
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
            func_80092AF4(&RetryProcess, 0x200);
            {
                u8 *q = GameInfo;
                SaveMenuState = 4;
                *(u16 *)(q + 0x30) |= 0x200;
            }
        }
        break;
    case 7:
        if (CardStatus & 2) {
            SAVEMENU_SET_ANIM(D_8009EFAC);
            f = Pads[0].pressed;
            if (f & 0x8000) {
                PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                if (HubSaveMenuHooks.unk0 != NULL) {
                    HubSaveMenuHooks.unk0();
                }
                SaveMenuState = 3;
            } else if (f & 0x1000) {
                SaveMenuState = 0;
            }
            break;
        }
        SaveMenuState = 0;
        break;
    case 1:
        f = CardStatus;
        if (f & 0x100) {
            SaveMenuState = 2;
        } else if (!(f & 0x80)) {
            SaveMenuState = 5;
        }
        SAVEMENU_SET_ANIM(D_8009EFF0);
        break;
    case 5:
        SAVEMENU_SET_ANIM(D_8009EFF4);
        if (Pads[0].pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            func_80092AF4(&RetryProcess, 0x200);
            SaveMenuState = 4;
            *(u16 *)(GameInfo + 0x30) |= 0x200;
        }
        break;
    case 2:
        SAVEMENU_SET_ANIM(D_8009EFF8);
        if (Pads[0].pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            func_8002D15C();
            func_80092198();
            SaveMenuState = 0;
        }
        break;
    case 3:
        f = CardStatus;
        if (!(f & 2) || !(f & 0x30)) {
            SaveMenuState = 0;
            if (HubSaveMenuHooks.unkC != NULL) {
                HubSaveMenuHooks.unkC();
            }
            func_80095BEC(1, &D_8009EEB0);
        } else {
            if (HubSaveMenuHooks.unk4 != NULL) {
                HubSaveMenuHooks.unk4();
            }
            if (HubSaveMenuHooks.unk8 != NULL) {
                HubSaveMenuHooks.unk8();
            }
            switch (HubSaveChoice) {
            case 1: {
                D_800657C8_Slot *dst = &CardSlots[CardSlotIndex];
                u8 *q = GameInfo;
                dst->b[0] = q[0x4C];
                dst->b[1] = q[0x4D];
                dst->b[2] = q[0x4E];
                dst->b[3] = q[0x4F];
                dst->b[4] = q[0x5C];
                dst->b[5] = q[0x5D];
                dst->b[6] = q[0x5E];
                dst->b[7] = q[0x5F];
                *(Blk12p4 *)(q + 0xD0) = *(Blk12p4 *)GameOptions;
                *(Blk18C *)dst->pad = *(Blk18C *)q;
                func_80091E58();
                card_save();
                SaveMenuState = 1;
                if (HubSaveMenuHooks.unkC != NULL) {
                    HubSaveMenuHooks.unkC();
                }
                func_80095BEC(1, &D_8009EEB0);
                break;
            }
            case 2:
                func_80092AF4(&RetryProcess, 0x200);
                SaveMenuState = 4;
                {
                    u8 *q = GameInfo;
                    *(u16 *)(q + 0x30) |= 0x200;
                }
                if (HubSaveMenuHooks.unkC != NULL) {
                    HubSaveMenuHooks.unkC();
                }
                break;
            }
            break;
        }
    case 6:
        f = CardStatus;
        if (f & 0x100) {
            SaveMenuState = 2;
        } else if (f & 0x80) {
            SAVEMENU_SET_ANIM(D_8009EFFC);
        } else {
            SaveMenuState = 0;
        }
        break;
    case 8:
        if (!(CardStatus & 0xE)) {
            SaveMenuState = 0;
        }
        SAVEMENU_SET_ANIM(D_8009F454);
        break;
    }
    if ((SaveMenuState == 2 || SaveMenuState == 8) && (Pads[0].pressed & 0x1000)) {
        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
        if (SaveMenuState == 2) {
            func_8002D15C();
        }
        func_80092AF4(&RetryProcess, 0x200);
        SaveMenuState = 4;
        {
            u8 *q = GameInfo;
            *(u16 *)(q + 0x30) |= 0x200;
        }
    }
}

void func_800917B4(void) {
    EXTengine *e;
    subengine *s;
    s32 v;

    func_80018B08(&SaveBackViewport, &SaveBackCamera);
    e = GetEngineSlot(8);
    s = e->sub;
    s->unk14 = FrameCount * 20;
    s->unk12 = 0x293;
    s->unk50 = 1;
    s->unk20.z = 0x2800;
    s->unk20.x = 0x2800;
    s->unk52 = 1;
    s->unk51 = 0;
    FadeAmount = ScreenFade.amount;
    func_8001C2C0((u8 *)e);
    func_80018B08(&SaveViewport, &SaveCamera);
    switch (D_8009F630) {
    case 0:
        if (Pads[0].pressed & 0xF0) {
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            v = 1;
            if (D_8009F634 == 1) {
                v = 2;
            }
            D_8009F634 = v;
        }
        if (Pads[0].pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            if (D_8009F634 == 1) {
                SaveMenuState = 0;
                D_8009F630 = D_8009F634;
            } else {
                D_8009F638 = 0;
                D_8009F630 = 2;
            }
        }
        break;
    case 1:
        func_80090E20();
        break;
    case 2:
        func_80090A10();
        break;
    }
}

void func_8009196C(void) {
    PadState *pad;
    EXTengine *e;
    EXTengine *e2;

    if (CardStatus & 2) {
        u8 *c = CardBuffer;
        if (*(s32 *)(c + 0x85C) != D_8009F4BC) {
            func_80092198();
            D_8009F4BC = *(s32 *)(c + 0x85C);
        }
    }
    pad = Pads;
    if (*(s32 *)((u8 *)pad + 0x48) & 0x10) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex - 2);
    }
    if (*(s32 *)((u8 *)pad + 0x48) & 0x40) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex + 2);
    }
    if (*(s32 *)((u8 *)pad + 0x48) & 0x80) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex - 1);
    }
    if (*(s32 *)((u8 *)pad + 0x48) & 0x20) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        SELECT_SLOT(CardSlotIndex + 1);
    }
    CardSlotIndex &= 3;
    e = GetEngineSlot(CardSlotIndex | 4);
    e2 = GetEngineSlot(2);
    e2->unk10.y = e->unk10.y;
    e2->unk10.x = e->unk10.x;
    RenderState.modelDp = ScreenFade.amount;
    func_8001C2C0((u8 *)GetEngineSlot(4));
    func_8001C2C0((u8 *)GetEngineSlot(5));
    func_8001C2C0((u8 *)GetEngineSlot(6));
    func_8001C2C0((u8 *)GetEngineSlot(7));
    func_8001C2C0((u8 *)GetEngineSlot(3));
    func_8001C2C0((u8 *)GetEngineSlot(2));
    switch (D_8009F630) {
    case 2:
        {
            s32 idx;
            u8 *p;
            u8 *g;
            idx = CardSlotIndex * 0x194;
            p = (u8 *)CardSlots;
            if (*(s32 *)(idx + p) == 0) {
                GetEngineSlot(3)->sub[13].unk6C = 0;
            } else {
                GetEngineSlot(3)->sub[13].unk6C = D_8009EFB4[GameLanguage];
            }
            GetEngineSlot(3)->sub[12].unk6C = D_8009EFC8[*(s32 *)((g = GameInfo) + 0x2C)];
            GetEngineSlot(3)->sub[14].unk6C = D_8009F000[*(s32 *)(g + 0x2C)];
        }
        break;
    case 1:
        {
            s32 idx;
            u8 *p;
            u8 *g;
            idx = CardSlotIndex * 0x194;
            p = (u8 *)CardSlots;
            if (*(s32 *)(idx + p) == 0) {
                GetEngineSlot(3)->sub[13].unk6C = D_8009EFB8[GameLanguage];
            } else {
                GetEngineSlot(3)->sub[13].unk6C = D_8009EFB0[GameLanguage];
            }
            GetEngineSlot(3)->sub[12].unk6C = D_8009EFC0[*(s32 *)((g = GameInfo) + 0x2C)];
            GetEngineSlot(3)->sub[14].unk6C = D_8009EFC4[*(s32 *)(g + 0x2C)];
        }
        break;
    }
}

void func_80091E58(void) {
    u8 *p = (u8 *)CardSlots;
    u8 *pad = p + 8;

    func_80090310(pad, 0,*(s32 *)CardSlots == 0, 0x28, 0x1E);
    func_80090310(p + 0x19C, 1, *(s32 *)(p + 0x194) == 0, 0x140, 0x1E);
    func_80090310(p + 0x330, 2, *(s32 *)(p + 0x328) == 0, 0x28, 0x7C);
    func_80090310(p + 0x4C4, 3, *(s32 *)(p + 0x4BC) == 0, 0x140, 0x7C);
    func_80095BEC(3, &D_8009F008);
    func_80090564(pad + CardSlotIndex * 0x194,*(s32 *)(p + CardSlotIndex * 0x194) == 0);
}

void func_80091F54(void) {
    subengine *sub;

    CardSlotIndex = 0;
    D_8009F4BC = 0x4D2;
    func_800920DC();
    FontPage = 0;
    D_8009F630 = 0;
    D_8009F634 = 1;
    func_80095BEC(8, &D_8009F40C);
    sub = GetEngineSlot(8)->sub;
    if (GameMode == 0x28) {
        sub->unk74 = 0x2007;
        sub->unk6C = D_800555DC;
    } else {
        sub->unk74 = 0x201F;
        sub->unk6C = AdventureItemsModel;
    }
    func_80095BEC(1, &D_8009EEB0);
    D_8009F494 = 0;
    SaveMenuState = 0;
    D_8009F630 = 1;
}

void func_8009201C(void) {
    subengine *sub;

    CardSlotIndex = 0;
    D_8009F4BC = 0x4D2;
    func_800920DC();
    FontPage = 0;
    D_8009F630 = 0;
    D_8009F634 = 1;
    func_80095BEC(8, &D_8009F40C);
    sub = GetEngineSlot(8)->sub;
    if (GameMode == 0x28) {
        sub->unk74 = 0x2007;
        sub->unk6C = D_800555DC;
    } else {
        sub->unk74 = 0x201F;
        sub->unk6C = AdventureItemsModel;
    }
    func_80095BEC(1, &D_8009EEB0);
    D_8009F638 = 0;
    D_8009F630 = 2;
}

void func_800920DC(void) {
    EXTengine *eng;

    func_80091E58();
    func_80095BEC(2, &D_8009EF64);
    GetEngineSlot(2)->unk10.x = 0x28;
    GetEngineSlot(2)->unk10.y = 0x1E;
    eng = GetEngineSlot(2);
    eng->flags |= 0x10000000;
    cam_init((u8 *)&SaveCamera);
    *(s32 *)((u8 *)&SaveCamera + 8) = 0x5DC;
    cam_init((u8 *)&SaveBackCamera);
    *(s32 *)((u8 *)&SaveBackCamera + 8) = 0x400;
    *(s32 *)((u8 *)&SaveBackCamera + 0x18) = 0x190;
    set_fade(&ScreenFade, -0x200, 0);
}

void func_80092198(void) {
    EXTengine *eng;

    func_80091E58();
    func_80095BEC(2, &D_8009EF64);
    GetEngineSlot(2)->unk10.x = 0x28;
    GetEngineSlot(2)->unk10.y = 0x1E;
    eng = GetEngineSlot(2);
    eng->flags |= 0x10000000;
}

void func_80092200(u8 *src) {
    u8 *g = GameInfo;

    g[0x4C] = src[0];
    g[0x4D] = src[1];
    g[0x4E] = src[2];
    g[0x4F] = src[3];
    g[0x5C] = src[4];
    g[0x5D] = src[5];
    g[0x5E] = src[6];
    g[0x5F] = src[7];
    *(Blk18C *)GameInfo = *(Blk18C *)(src + 8);
    {
        Blk12p4 *d = (Blk12p4 *)GameOptions;
        u8 *s = GameInfo;
        *d = *(Blk12p4 *)(s + 0xD0);
    }
    func_80091E58();
}

void func_800922FC(u8 *dst) {
    u8 *g = GameInfo;

    dst[0] = g[0x4C];
    dst[1] = g[0x4D];
    dst[2] = g[0x4E];
    dst[3] = g[0x4F];
    dst[4] = g[0x5C];
    dst[5] = g[0x5D];
    dst[6] = g[0x5E];
    dst[7] = g[0x5F];
    *(Blk16 *)(g + 0xD0) = *(Blk16 *)GameOptions;
    *(Blk18C *)(dst + 8) = *(Blk18C *)g;
    func_80091E58();
}

s32 func_800923F0(s32 *arg0) {
    return *arg0 == 0;
}

void func_800923FC(void) {
}

void func_80092404(void) {
    func_80095A04(2);
    func_80095A04(3);
    func_80095A04(4);
    func_80095A04(5);
    func_80095A04(6);
    func_80095A04(7);
    func_80095A04(8);
    func_80017640();
    func_8001888C();
    func_800183DC();
}

void func_8009246C(void) {
    subengine *sub;

    CardSlotIndex = 0;
    D_8009F4BC = 0x4D2;
    func_800920DC();
    FontPage = 0;
    D_8009F630 = 0;
    D_8009F634 = 1;
    func_80095BEC(8, &D_8009F40C);
    sub = GetEngineSlot(8)->sub;
    if (GameMode == 0x28) {
        sub->unk74 = 0x2007;
        sub->unk6C = D_800555DC;
    } else {
        sub->unk74 = 0x201F;
        sub->unk6C = AdventureItemsModel;
    }
    func_80095BEC(1, &D_8009EEB0);
}

s32 func_8009251C(u8 *arg0) {
    u8 *p;
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 0x21; i++) {
        p = arg0 + i;
        if (p[0x105] >= 2) {
            n += p[0x105] - 1;
        }
    }
    return n;
}

s32 func_80092560(u8 *arg0) {
    s32 n;
    s32 i;

    n = 0;
    i = 0;
    do {
        if ((arg0 + i)[0x105] != 0) {
            n++;
        }
        i++;
    } while (i < 0x21);
    return n;
}

s32 func_80092598(u8 *arg0) {
    s32 n;
    s32 i;

    n = 0;
    i = 0;
    do {
        n += (arg0 + i)[0x126];
        i++;
    } while (i < 0x21);
    return n;
}

s32 func_800925C4(u8 *arg0) {
    s32 n;
    s32 i;

    n = 0;
    i = 0;
    do {
        n += (arg0 + i)[0x147];
        i++;
    } while (i < 0x21);
    return n;
}

void func_800925F0(s32 arg0) {
    s32 *src;
    s32 *p;
    s32 i;
    s32 *base;

    src = (s32 *)(arg0 * 4);
    src = (s32 *)((s32)src + arg0);
    src = (s32 *)((s32)src * 4);
    src = (s32 *)((s32)src + (s32)D_8009F248);
    base = (s32 *)GetEngineSlot(3)->sub;
    p = base + 0xA8 / 4;
    i = 0;
    do {
        if (*src == -1) {
            p[0] &= ~0x8000;
            p[0x2A] &= ~0x8000;
        } else {
            p[0] |= 0x8000;
            p[0x2A] |= 0x8000;
            p[1] = *src;
            p[0x2B] = *src;
        }
        p += 0x54;
        i++;
        src++;
    } while (i < 5);
}

void func_800926CC(GameState *arg0) {
    s32 i;
    u8 t;
    u8 *p;
    s16 j;
    s16 k;

    arg0->unk34 = 0x28;
    arg0->unk36 = 0;
    arg0->unk37 = 0;
    arg0->unk63 = -1;
    arg0->unk64 = 0;
    arg0->unk36 = 0;
    arg0->unk37 = 0;
    k = ' ';
    for (i = 7, p = (u8 *)arg0 + i; i >= 0; i--, p--) {
        p[0x18] = k;
    }
    t = 0x3C;
    do {
        j = 1;
    } while (0);
    arg0->unk32 = 0x14;
    arg0->unk3E = 0x200;
    arg0->unk20 = 0;
    arg0->unkE0 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unk38 = j;
    arg0->unk24 = 0;
    arg0->unk30 = 0;
    arg0->unk10 = 0;
    if (D_800637A8 == 1) {
        t = 0x32;
    }
    arg0->unk21 = t;
    arg0->unk4C = j;
    arg0->unk3A = 3;
    arg0->unk4D[0] = 3;
    arg0->unk4D[1] = 5;
    arg0->unk4D[2] = 7;
    arg0->unk44 = 1;
    arg0->unk54[0] = -1;
    arg0->unk54[1] = -1;
    arg0->unk54[2] = -1;
    arg0->unk54[3] = -1;
    arg0->unk14 = 0;
    arg0->unk68 = 0;
    arg0->unk60 = 0;
    arg0->unk61 = 0;
    arg0->unk5C[0] = 0;
    arg0->unk5C[1] = 0;
    arg0->unk5C[2] = 0;
    arg0->unk5C[3] = 0;
    arg0->unk46 = 2;
    arg0->unk48 = 0;
    arg0->unk4A = 2;
    arg0->unk50 = arg0->unk4C;
    arg0->unk51[0] = arg0->unk4D[0];
    arg0->unk51[1] = arg0->unk4D[1];
    arg0->unk51[2] = arg0->unk4D[2];
    arg0->unk58[0] = arg0->unk54[0];
    arg0->unk58[1] = arg0->unk54[1];
    arg0->unk74 = 0;
    arg0->unk75 = 0;
    arg0->unk58[2] = arg0->unk54[2];
    arg0->unk58[3] = arg0->unk54[3];
    for (i = 0; i < 0x21; i++) {
        arg0->unkAB[i] = 0;
        arg0->unk76[i] = 0;
        arg0->unkE4[i] = 0;
        arg0->unk126[i] = 0;
        arg0->unk105[i] = 0;
        arg0->unk147[i] = 0;
        arg0->unk168[i] = 0;
    }
    t = 1;
    arg0->unkA9 = 2;
    arg0->unkAA = 3;
    arg0->unk9B[0] = 0;
    arg0->unk9B[1] = 0;
    arg0->unk9B[2] = 0;
    arg0->unk9B[3] = 0;
    arg0->unkCC[0] = 0;
    arg0->unkCC[1] = 0;
    arg0->unkCC[2] = 0;
    arg0->unkCC[3] = 0;
    arg0->unk9B[4] = 0;
    arg0->unk9B[5] = 0;
    arg0->unk9B[6] = 0;
    arg0->unk9B[7] = 0;
    arg0->unkA7 = 0;
    arg0->unkA8 = t;
    arg0->unk26 = t;
    arg0->unk27 = 0;
    arg0->unk42 = -1;
    arg0->unk6C = 0;
    arg0->unk70 = 0;
    arg0->unk2C = D_8004E0D4;
    arg0->unkD0 = *(Blk12p4 *)GameOptions;
}

void func_800928E8(void) {
    EngineCfg *cfg = &D_8009F96C;
    u8 *g;

    func_800274FC((s32 *)cfg);
    ((void (*)(u8 *))func_800276A4)(D_800637A8 == 0 ? vmode_512_ntsc : D_8005B744);
    g = GameInfo;
    *(s32 *)(g + 4) = 0x80000000;
    func_80028774(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    func_80017164(cfg->pools);
    func_800170D4();
    func_80095DBC();
    D_8004E0D4 = 0;
    D_8009F654 = 0;
    core_process = 0;
    *(s32 *)GameOptions = 0x46;
    *(s32 *)(GameOptions + 4) = 0x64;
    *(s32 *)(GameOptions + 8) = 1;
    GameOptions[0xC] = 1;
    GameOptions[0xD] = 1;
    GameOptions[0xE] = 1;
    GameOptions[0xF] = 1;
    func_800926CC((GameState *)g);
    func_800222FC();
}

void func_800929D8(void) {
    ProcessControl.process = NULL;
    func_8001E934(&ProcessControl, NULL);
    func_800928E8();
    ScreenFade.amount = 0x1000;
    ScreenFade.maxed = 3;
    ScreenFade.speed = 0x100;
    SetDispMask(1);
    FirstBootDone = 1;
    func_80092AF4(&D_800A00DC, 0x200);
    func_8001E598(&ScreenFade);
    func_8001E610(&ProcessControl, &ScreenFade);
    func_8001E898(&ProcessControl);
    if (ProcessControl.process != NULL) {
        if (ProcessControl.process->quit != NULL) {
            ProcessControl.process->quit();
        }
    }
}

void func_80092AA8(process_info *arg0) {
    if (ProcessControl.next_process != arg0) {
        ((void (*)(fade_info *))set_fade)(&ScreenFade);
        ProcessControl.next_process = arg0;
    }
}

void func_80092AF4(process_info *process, s32 speed) {
    process_control_info *p = &ProcessControl;

    if (p->next_process != process) {
        set_fade(&ScreenFade, speed, 0);
        p->next_process = process;
    }
}

void func_80092B40(void) {
    if (ProcessControl.process != NULL) {
        if (ProcessControl.process->quit != NULL) {
            ProcessControl.process->quit();
        }
    }
}

void func_80092B7C(void) {
    func_8001E898(&ProcessControl);
}

void func_80092BA0(void) {
    func_8001E598(&ScreenFade);
    func_8001E610(&ProcessControl, &ScreenFade);
}

void func_80092BDC(void) {
    ProcessControl.process = NULL;
    func_8001E934(&ProcessControl, NULL);
    func_800928E8();
    ScreenFade.amount = 0x1000;
    ScreenFade.maxed = 3;
    ScreenFade.speed = 0x100;
    SetDispMask(1);
    FirstBootDone = 1;
    func_80092AF4(&D_800A00DC, 0x200);
}

void func_80092C54(void) {
    func_800926CC(GameInfo);
}

void func_80092C78(void) {
    s8 *p;
    *(s32 *)GameOptions = 0x46;
    p = GameOptions;
    *(s32 *)(p + 4) = 0x64;
    *(s32 *)(p + 8) = 1;
    p[0xC] = 1;
    p[0xD] = 1;
    p[0xE] = 1;
    p[0xF] = 1;
}

void func_80092CAC(void) {
    EngineGameView *g;

    g = (EngineGameView *)GameInfo;
    g->unk30 &= 0x7FF8;
    set_fade(&ScreenFade, -0x300, 0);
    D_8009F8B8 = 0;
    if (g->mode == 0x24 && g->unkE0 <= 0) {
        g->unkE0 = 1;
    }
    g = (EngineGameView *)GameInfo;
    if (func_8007A5E8((u8 *)g) >= 0x19 && g->unkE0 < 2) {
        g->unkE0 = 2;
    }
    func_80017640();
    func_80010AE8(&MainHeap);
    FirstBootDone = 0;
    if (GameExitFlags & 0x200) {
        func_80079260();
    } else {
        func_8002C284(1);
    }
    EngineBeginMode(1);
}

void func_80092E40(void) {
    func_80022030();
    func_8007AB28();
    sfx_delete_all();
    process_quit(&MenuProcessControl);
    func_8001888C();
    func_800183DC();
    func_8008ECE4();
}

void func_80092E94(void) {
    RenderHook();
    FadeAmount = ScreenFade.amount;
    func_80082590();
    func_8001E898(&MenuProcessControl);
}

void func_80092EDC(void) {
    if (func_8007F314() != 0) {
        u8 *g;
        GameFrameCount++;
        g = GameInfo;
        *(s32 *)(g + 0xC) += 1;
        func_8007976C();
        ((void (*)(void *, u8 *))map_update)(MapScene, D_8005164C);
    } else {
        func_8007A7C0();
    }
    MainGfxCamera.z = 0;
    *(s32 *)((u8 *)&MainGfxCamera + 8) = 0x800;
    HudCamera.z = 0;
    *(s32 *)((u8 *)&HudCamera + 8) = 0x40C;
    func_80097C28();
    scripts_update();
    func_8001E610(&MenuProcessControl, &ScreenFade);
    func_8008EFD4();
}

void func_80092F9C(void) {
    GameInfo[0x63] = 0xA;
    *(s32 *)(GameInfo + 0x64) = 1;
}

void func_80092FB8(void) {
    EngineGameView *a = (EngineGameView *)GameInfo;
    EngineGameView *g;

    if (a->mode == 0x24 && a->unkE0 <= 0) {
        a->unkE0 = 1;
    }
    g = (EngineGameView *)GameInfo;
    if (func_8007A5E8((u8 *)g) >= 0x19 && g->unkE0 < 2) {
        g->unkE0 = 2;
    }
}

void func_80093038(void) {
    func_80018B08(&GameViewport, (GfxCamera *)MainCamera);
    RenderState.modelDp = 0x1000 - ((0x1000 - D_8009F8B8) * (0x1000 - ScreenFade.amount) / 4096);
    D_80056878 = 0;
    D_8005687C[0] = 0;
    D_8005687C[1] = 0;
    D_8005687C[2] = 0;
    scripts_render();
    func_8007A718();
    func_8001DAF8((GfxScene *)MapScene);
    func_80018B08(&MainViewport, &MainGfxCamera);
    RenderState.modelDp = ScreenFade.amount;
    func_800798A4();
    func_800984D4();
}

void func_80093110(void) {
    func_80018B08(&GameViewport, (GfxCamera *)MainCamera);
    RenderState.modelDp = 0x1000 - ((0x1000 - D_8009F8B8) * (0x1000 - ScreenFade.amount) / 4096);
}
