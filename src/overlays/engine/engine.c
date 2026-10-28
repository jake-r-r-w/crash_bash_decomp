#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/hero.h"
#include "bash/shell.h"

void InitialiseNasSystem(void) {
    EngineRec32 *rec;
    EngineBootView *v;
    u8 *p;

    rec = ((EngineBootSrc *)D_8005A5E0)->unk4;
    D_8005A5E8 = rec;
    if (rec != NULL) {
        EngineBootView *w = (EngineBootView *)GameInfo;
        w->unk3C = *(u16 *)&rec[w->unk8];
    }
    v = (EngineBootView *)GameInfo;
    if (v->unk0 != NULL) {
        v->unk0(rec);
    }
    SetNasDefaults();
    Init_Panel();
    NasPool = (NAS *)mal_alloc(NasPoolUsed * 0x70, &MainHeap);
    D_8005AA20 = (u8 *)NasPool + (NasPoolUsed * 0x70 - 0x70);
    NasUpdateList = (EngObj **)mal_alloc(NasPoolUsed * 4, &MainHeap);
    *NasUpdateList = NULL;
    p = mal_alloc(NasPoolUsed * 4, &MainHeap);
    D_8005AA44 = p;
    *(s32 *)p = 0;
    InitSysFontModel();
    D_8005AAB0 = D_800986A4[v->unk2C];
}

void BuildCameraFrameList(void) {
    NAS *nas;
    EXTengine *eng;
    EngineGameView *g;
    s32 ang;
    s32 dx;
    s32 dz;
    s32 w;
    s32 l;
    s32 r;
    s32 t;

    nas = (NAS *)D_8005AA34;
    camera_frame_list_size = 0;
    if (nas != NULL) {
        g = (EngineGameView *)GameInfo;
        do {
            eng = nas->Engine;
            if (eng != NULL && (nas->Camera.flags & 0x8000)) {
                w = nas->Camera.width;
                r = w;
                if (!(g->unk30 & 0x801)) {
                    ang = eng->unk1E & 0xFFF;
                    dx = -rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * nas->Camera.look_ahead >> 12;
                    dz = -rcossin_tbl[ang].unk2 * nas->Camera.look_ahead >> 12;
                    if (dx < 0) {
                        dx = -dx;
                        l = w;
                        if (w < dx) {
                            l = dx;
                        }
                    } else {
                        l = w;
                        if (w < dx) {
                            r = dx;
                        }
                    }
                    if (dz < 0) {
                        dx = w;
                        dz = -dz;
                        if (dx < dz) {
                            dx = dz;
                        }
                        t = w;
                    } else {
                        t = w;
                        if (w < dz) {
                            t = dz;
                        }
                        dx = w;
                    }
                    camera_add_frame_list((s32)&eng->unk10, l, r, dx >> 1, t >> 1, nas->Camera.height);
                } else {
                    dx = nas->Camera.height;
                    dx -= (u32)dx >> 2;
                    camera_add_frame_list((s32)&eng->unk10, w >> 1, w >> 1, 0, 0, dx);
                }
            }
            nas = nas->Camera.Child;
        } while (nas != NULL);
    }
    {
        EngineGameView *g1 = (EngineGameView *)GameInfo;
        if ((g1->unk4 & 0x200) && !(g1->unk30 & 0x801)) {
            camera_add_frame_list((s32)D_8005B3A8, 0x80, 0x80, 0, 0, 0x80);
            camera_add_frame_list((s32)(D_8005B3A8 + 0x18), 0x80, 0x80, 0, 0, 0x80);
        }
    }
    {
        EngineGameView *g2 = (EngineGameView *)GameInfo;
        if (g2->flags & 0x800) {
            if (!(g2->unk30 & 0x800)) {
                camera_add_frame_list((s32)D_8005B3D8, 0x80, 0x80, 0, 0, 0x80);
                camera_add_frame_list((s32)(D_8005B3D8 + 0x18), 0x80, 0x80, 0, 0, 0x80);
            }
        }
    }
}

void UpdateAllNas(void) {
    s32 i;
    s32 n;
    NAS *nas;
    u16 cls;

    GlobalNAStatus = 1;
    *NasUpdateList = NULL;
    *(s32 *)D_8005AA44 = 0;
    i = 0;
    if (D_8005AA7C != NULL) {
        D_8005AA7C();
    }
    do {
        if (Heroes[i].hero->Class & 0x1000) {
            n = ++*(s32 *)D_8005AA44;
            ((NAS **)D_8005AA44)[n] = Heroes[i].hero;
        }
        i++;
    } while (i < 4);
    for (i = FirstNas; i < D_8005AA12; i++) {
        nas = &NasPool[i];
        if (nas->NASid & 1) {
            cls = nas->Class;
            if (cls & 1) {
                nas->Handler(nas);
            }
            if (cls & 2) {
                n = ++*(s32 *)NasUpdateList;
                NasUpdateList[n] = (EngObj *)nas;
            }
            if (cls & 0x1000) {
                n = ++*(s32 *)D_8005AA44;
                ((NAS **)D_8005AA44)[n] = nas;
            }
        }
    }
    GlobalNAStatus = 0;
}

void InitialiseLevel(void) {
    EngineGameView *g = (EngineGameView *)GameInfo;
    s32 r;
    s32 i;
    s32 want;
    s32 n;

    g->flags &= 0xF4FF;
    InitTriggers();
    mapenv_place_subs((MapEnvHead *)MapScene);
    ClearAllNasPool();
    ResetNasHeap();
    ResetNasLists();
    SetDefaultHeroHooks();
    SetLevelCameraMode();
    if (LevelSpecs.LevelPreInit != NULL) {
        LevelSpecs.LevelPreInit();
    }
    GlobalNAStatus = 0;
    ASTTable = EngineASTTable;
    ASTCount = ASTTableCount();
    InitialiseHeroes();
    SetTweakAi();
    RunLevelTriggers();
    {
        s32 w1 = 0x206;
        for (r = 0; r < ASTCount; r++) {
            if (ASTTable[r].id == w1) {
                goto found1;
            }
        }
        r = 0;
    }
found1:
    D_8009E298 = r;
    want = 0x20C;
    i = 0;
    if (ASTCount > 0) {
        n = ASTCount;
        r = (s32)ASTTable;
        for (;;) {
            if (((ASTEntry *)r)->id == want) {
                r = i;
                goto found2;
            }
            i++;
            if (i >= n) {
                break;
            }
            r += sizeof(ASTEntry);
        }
    }
    r = 0;
found2:
    D_8009E29C = r;
    if (D_8005AA74 != NULL) {
        D_8005AA74();
    }
}

void PickCpuHeroes(void) {
    s32 w;
    s32 i;
    s32 j;
    s32 n;
    s32 found;
    s32 off;
    u16 v;
    u8 *g;
    u8 *p;

    switch (ModeTypes[NextGameMode * 3]) {
    case 0x104:
        w = 0;
        break;
    case 0x109:
        w = 1;
        break;
    case 0x107:
        w = 2;
        break;
    case 0x108:
        w = 3;
        break;
    case 0x106:
        w = 4;
        break;
    case 0x10A:
        w = 5;
        break;
    case 0x103:
    case 0x10B:
    case 0x10D:
    case 0x10E:
        w = 6;
        break;
    default:
        return;
    }
    n = 0;
    i = 0;
    g = GameInfo;
    for (; i < 4; i++) {
        if (i != 0 && (*(s32 *)(g + 0x6C) == 0 || i != 1)) {
            found = 1;
            while (n < 6) {
                p = (u8 *)(i + (s32)g);
                if (found == 0) {
                    break;
                }
                v = *(u16 *)((u8 *)D_800986BC + (n * 2 + w * 8));
                n++;
                found = 0;
                for (j = 0; j < 4; j++) {
                    if ((j == 0 || (*(s32 *)(g + 0x6C) != 0 && j == 1)) && g[j + 0x50] == v) {
                        found = 1;
                    }
                }
                if (found == 0) {
                    p[0x50] = v;
                }
            }
        }
    }
}

void CheckControllers(void) {
    EngMenuGame *g = (EngMenuGame *)GameInfo;
    EngMenuItem *item;
    u16 *levels;
    s32 i;
    s16 y;

    if (g->unk74 != 0 || g->unk68 != 0) {
        return;
    }
    levels = ModeTypes;
    if (levels[g->mode * 3] == 0x101) {
        return;
    }
    item = D_8005A3EC;
    g->flags &= ~0x4000;
    item[1].unk0 = -1;
    item[2].unk0 = -1;
    item[3].unk0 = -1;
    item[4].unk0 = -1;
    y = 0x72;
    if (PadCount < g->count) {
        i = 0;
        if (g->count != 0) {
            u16 *lv = levels;
            EngMenuGame *g1 = (EngMenuGame *)GameInfo;

            do {
                if (lv[g1->mode * 3] == 0x102) {
                    g1->flags |= 0x4009;
                } else {
                    g1->flags |= 0x4001;
                }
                PausePad = 0;
                item->unk0 = 5;
                item->unk6 = i + 0x1C;
                item++;
                y += 0x12;
                i++;
            } while (i < g1->count);
        }
    } else {
        i = 0;
        if (g->count != 0) {
            u16 *lv2;
            EngMenuGame *g2;
            lv2 = levels;
            g2 = (EngMenuGame *)GameInfo;
            do {
                if (!(Pads[i].unk0 & 1)) {
                    if (lv2[g2->mode * 3] == 0x102) {
                        g2->flags |= 0x4009;
                    } else {
                        g2->flags |= 0x4001;
                    }
                    item->unk0 = 5;
                    PausePad = 0;
                    if (g2->count < 3) {
                        if (PadCount < 3) {
                            item->unk6 = i + 0x1A;
                        } else {
                            item->unk6 = i + 0x1C;
                        }
                    } else {
                        item->unk6 = i + 0x1C;
                    }
                    item++;
                    y += 0x12;
                }
                i++;
            } while (i < g2->count);
        }
    }
    item->unk6 = 0x20;
    item->unk0 = 5;
    D_8005A3A4[2] = y;
    D_8005A3A4[3] = y;
}

void UpdateLevel(void) {
    s32 i;
    EngObj *o;

    mal_count_free(&MainHeap);
    CheckControllers();
    if (GameCheats & 0x10) {
        FntPrint("\t\tFree Memory  - tot %d  max %d\n\0\x18\0", D_8004E104, D_8004E10C);
    }
    UpdateAllNas();
    ProcessHeroes();
    GlobalNAStatus = 2;
    i = 0;
    if (*(s32 *)NasUpdateList > 0) {
        do {
            i++;
            o = NasUpdateList[i];
            if (o->flags & 1) {
                o->update(o);
            }
        } while (i < *(s32 *)NasUpdateList);
    }
    GlobalNAStatus = 0;
    Draw_Clock();
    UpdatePanel();
    if (PanelCtrl & 0x4000) {
        UpdateOutMessage();
    }
    UpdatePauseSlide(1);
    UpdateCamera(0);
}

void DrawLevelHud(void) {
    s32 show;
    s32 i;
    s32 n;
    SprPos pos;
    u32 c0;
    u32 c1;
    u8 *tim;

    if (ModeTypes[HUDGAME->mode * 3] == 0x102) {
        show = 1;
        if (!(HUDGAME->flags & 1)) {
            show = 0;
        } else if (HUDGAME->flags & 0x0A007662) {
            show = 0;
        }
        if (GameLevelFlags & 3) {
            show = 0;
        }
        if (show && (PanelCtrl & 0x20000)) {
            tim = InterfaceModel;
            c0 = 0xFFFFFF;
            c1 = 0xFFFF;
            pos.f.y = D_80099700[1];
            for (i = 0; i < 4; i++) {
                if (CompletionDigits[i].flags & 0x8000) {
                    pos.f.x = D_80099700[0] + CompletionDigits[i].arg;
                    texture_draw_quad((obj_texture *)(*(u8 **)(tim + 0x18) + (CompletionDigits[i].cmd & 0xFFF) * 0x38),
                                  pos.w, 0, c0, c0, c1, c1);
                }
            }
        }
        return;
    }
    if (PanelCtrl >= 0) {
        return;
    }
    if ((PanelCtrl & 1) && (Clock_flags & 0x8000)) {
        show = (u32)FrameCount >> 5;
        show &= 1;
    } else {
        show = 1;
    }
    if (!(GamePause & 1)) {
        show = 0;
    } else if (GamePause & 0x0A007662) {
        show = 0;
    }
    if (HUDGAME->unk32 & 3) {
        show = 0;
    }
    if (show) {
        if (PanelCtrl & 0x20000) {
            tim = InterfaceModel;
            c0 = 0xFFFFFF;
            c1 = 0xFFFF;
            pos.f.y = D_80099700[1];
            for (i = 0; i < 4; i++) {
                if (CompletionDigits[i].flags & 0x8000) {
                    pos.f.x = D_80099700[0] + CompletionDigits[i].arg;
                    texture_draw_quad((obj_texture *)(*(u8 **)(tim + 0x18) + (CompletionDigits[i].cmd & 0xFFF) * 0x38),
                                  pos.w, 0, c0, c0, c1, c1);
                }
            }
        }
    } else if ((PanelCtrl & 1) && (Clock_flags & 0x8000)) {
        tim = InterfaceModel;
        c0 = 0xD0FF;
        c1 = 0xFF;
        pos.f.y = Clock_XYZ[1];
        if (!(Clock_flags & 2) && (HUDGAME->flags & 1)) {
            Clock[0].Ctrl |= 0x8000;
        }
        for (i = 0; i < 4; i++) {
            if (Clock[i].Ctrl & 0x8000) {
                pos.f.x = Clock_XYZ[0] + Clock[i].Xoffs;
                texture_draw_quad((obj_texture *)(*(u8 **)(tim + 0x18) + (Clock[i].frame & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
            }
        }
    }
    if (countdown_eng != NULL) {
        eng_render((EXTengine *)countdown_eng);
    }
    if (PanelCtrl & 0x1000) {
        eng_render(&BigScore1);
        eng_render(&BigScore2);
    }
    i = 0;
    do {
        EXTengine *e = HUDPLAYERS[i];
        if (e != NULL) {
            subengine *s = (subengine *)((u8 *)&IconParts[0][1] + i * sizeof(IconParts[0]));
            u32 v = s->flags & ~0x8000;
            s->flags = v;
            if (D_8009AEF4 == i && (FrameCount & 8)) {
                s->flags = v | 0x8000;
            }
            eng_render(HUDPLAYERS[i]);
        }
        if (PanelCtrl & 0x400) {
            if (Score[i].Ctrl != 0) {
                tim = InterfaceModel;
                c0 = 0xD0FF;
                c1 = 0xFF;
                pos.f.x = HUDPLAYERS[i]->unk10.x + Score[i].Xoffs;
                pos.f.y = HUDPLAYERS[i]->unk10.y + Score[i].Yoffs;
                n = Score[i].score;
                texture_draw_quad((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
                n /= 10;
                pos.f.x -= 0x18;
                texture_draw_quad((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
                n /= 10;
                pos.f.x -= 0x18;
                texture_draw_quad((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
            }
        }
        if (PanelCtrl & 0x200) {
            if (Score[i].Ctrl != 0) {
                tim = InterfaceModel;
                c0 = 0xD0FF;
                c1 = 0xFF;
                pos.f.x = HUDPLAYERS[i]->unk10.x + Score[i].Xoffs;
                pos.f.y = HUDPLAYERS[i]->unk10.y + Score[i].Yoffs;
                n = Score[i].score;
                texture_draw_quad((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
                n /= 10;
                pos.f.x -= 0x18;
                texture_draw_quad((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
            }
        }
        if ((PanelCtrl & 0x100) && Score[i].Ctrl != 0) {
            tim = InterfaceModel;
            c0 = 0xD0FF;
            c1 = 0xFF;
            n = Score[i].score;
            pos.f.x = HUDPLAYERS[i]->unk10.x + Score[i].Xoffs;
            pos.f.y = HUDPLAYERS[i]->unk10.y + Score[i].Yoffs;
            texture_draw_quad((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                          pos.w, 0, c0, c0, c1, c1);
        }
        i++;
    } while (i < 4);
    if (!(GamePause & 1) && (PanelCtrl & 0x4000)) {
        gfx_set_viewport(&HudViewport, &HudCamera);
        eng_render_subs((u8 *)GetEngineSlot(5));
    }
}

s32 LevelRequirementMet(arg0, arg1)
s32 arg0;
EngineReqObj *arg1;
{
    s32 sa, sb, sc, sd;
    s32 i;
    EngineReq *r;
    u8 *p;
    EngineReq *tab;

    if ((arg1->flags & 0x80002000) == 0x80002000) {
        return 1;
    }
    sd = 0;
    sb = 0;
    sc = 0;
    sa = 0;
    i = 0;
    do {
        p = (u8 *)arg1 + i;
        i++;
        sa += p[0xE4];
        sc += p[0x126];
        sb += p[0x105];
        sd += p[0x147];
    } while (i < 0x21);
    tab = D_8004DEE0;
    r = &tab[arg0 & 0xFFFF];
    if (sa < r->min0 || sc < r->min4 || sb < r->min2 || sd < r->min6) {
        return 0;
    }
    if (r->off != 0 && *((u8 *)arg1 + r->off + 0x168) == 0) {
        return 0;
    }
    return 1;
}

s32 GravitateNas(NAS *nas) {
    EXTengine *eng;
    EXTmotion *m;
    s16 dec;

    eng = nas->Engine;
    if (eng == NULL) {
        return 0;
    }
    m = FindExtension(nas, 0x800);
    if (m == NULL) {
        return 0;
    }
    m->Xvel += m->Xacc - m->Xdec;
    if (m->Xvel < 0) {
        m->Xvel = 0;
    }
    if (m->Xvel > m->Xmax) {
        m->Xvel = m->Xmax;
    }
    eng->unk10.x -= m->Xvel * rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 >> 12;
    eng->unk10.z -= m->Xvel * rcossin_tbl[eng->unk1E & 0xFFF].unk2 >> 12;
    m->Yvel += m->Yacc - m->Ydec;
    if (m->Yvel < 0) {
        m->Yvel = 0;
    }
    if (m->Yvel > m->Ymax) {
        m->Yvel = m->Ymax;
    }
    if (m->Ydir) {
        eng->unk10.y -= m->Yvel;
    } else {
        eng->unk10.y += m->Yvel;
    }
    if (m->Yvel == 0) {
        s16 acc = m->Ydec;
        m->Ydec = 0;
        m->Ydir = 0;
        m->Yacc = acc;
    }
    if (m->Ydir != 0) {
        return 0;
    }
    if (ApplyContourMoved(eng) == 0) {
        return 0;
    }
    dec = m->Yacc;
    m->Ydir = 0xFF;
    m->Yacc = 0;
    m->Yvel = m->Yvel >> 1;
    m->Xvel = m->Xvel >> 1;
    m->Ydec = dec;
    return 1;
}

NAS *SpawnModelFxOnce(s32 arg0, s32 *arg1, s32 arg2) {
    NAS *nas;
    s32 *ext;

    nas = CreateNAS(arg1[0], arg1[1], arg1[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = arg0;
            ext[6] = 0x100;
            if (arg2 != 0) {
                ext[8] = 4;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = arg2;
            } else {
                ext[8] = 0;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}

NAS *SpawnModelFx(s32 arg0, s32 *arg1, s32 arg2) {
    NAS *nas;
    s32 *ext;

    nas = CreateNAS(arg1[0], arg1[1], arg1[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = arg0;
            ext[6] = 0x100;
            if (arg2 != 0) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = arg2;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}

s32 CountTrophies(u8 *arg0) {
    s32 sum = 0;
    s32 i = 0;
    do {
        sum += (arg0 + i)[0xE4];
        i++;
    } while (i < 0x21);
    return sum;
}

void ArenaRequirementMet(s32 arg0) {
    u8 *p = GameInfo;
    LevelRequirementMet(D_800986FC[arg0 & 0xFFFF], p);
}

void ArenaRequirementMetAny(s32 arg0) {
    LevelRequirementMet(D_800986FC[arg0 & 0xFFFF]);
}

void RequirementMet(s32 arg0) {
    LevelRequirementMet(arg0 & 0xFFFF, GameInfo);
}

void RequirementMetAny(s32 arg0) {
    LevelRequirementMet(arg0 & 0xFFFF);
}

void ModeRequirementMet(s32 arg0) {
    LevelRequirementMet(arg0 & 0xFFFF, GameInfo);
}

void RunLevelTriggers(void) {
    RunTriggers();
}

void RenderAllNas(void) {
    NAS *nas;
    EXTengine *eng;

    for (nas = NASActiveHead; nas != NULL; nas = nas->Tag.Child) {
        eng = nas->Engine;
        if (eng != NULL && (nas->NASid & 1)) {
            do {
                if (!(nas->Class & 0x2000)) {
                    eng->OldXYZ = eng->unk10;
                }
                eng_render(eng);
                eng = eng->Engine;
            } while (eng != NULL);
        }
    }
}

void RenderLevel(void) {
    mal_count_free(&MainHeap);
    CheckControllers();
    GlobalNAStatus = 4;
    UpdatePanel();
    GlobalNAStatus = 0;
    UpdatePauseSlide(0);
}

void InitialiseEngine(void) {
    s32 i;
    s32 r;
    s32 want;

    GlobalNAStatus = 0;
    ASTTable = EngineASTTable;
    ASTCount = ASTTableCount();
    InitialiseHeroes();
    SetTweakAi();
    RunLevelTriggers();
    want = 0x20C;
    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == want) {
            r = i;
            goto found;
        }
    }
    r = 0;
found:
    D_8009E29C = r;
    if (D_8005AA74 != NULL) {
        D_8005AA74();
    }
}

void PreInitLevel(void) {
    EngineGameView *g = (EngineGameView *)GameInfo;

    g->flags &= 0xF4FF;
    SetDefaultHeroHooks();
    SetLevelCameraMode();
    if (LevelSpecs.LevelPreInit != NULL) {
        LevelSpecs.LevelPreInit();
    }
}

void DoAmbience(void) {
    if (D_8009871C != 0) {
        D_8009871C = D_8009871C - 1;
    } else {
        u8 *p = D_80098718;
        p += Random(8) * 2;
        PlaySfx(*(u16*)(p + 8), 0, 0, 0x1000, 0x1E00, 0);
        D_8009871C = (*(u16*)((u8*)D_80098718) + Random(*(u16*)((u8*)D_80098718 + 2) - *(u16*)((u8*)D_80098718))) * 30;
    }
}

void InitAmbience(Ambient_data *arg0) {
    D_80098718 = arg0;
    D_8009871C = 0;
    PlaySfx((*(u16*)((u8*)(arg0) + 4)), 0, 0, 0x1000, 0x1E00, 0);
}

void UpdateNasObjects(void) {
    s32 i;
    EngObj *o;

    GlobalNAStatus = 2;
    i = 0;
    if (*(s32 *)NasUpdateList > 0) {
        do {
            i++;
            o = NasUpdateList[i];
            if (o->flags & 1) {
                o->update(o);
            }
        } while (i < *(s32 *)NasUpdateList);
    }
    GlobalNAStatus = 0;
    Draw_Clock();
    UpdatePanel();
    if (PanelCtrl & 0x4000) {
        UpdateOutMessage();
    }
}

void RenderNas(void) {
    GlobalNAStatus = 4;
    UpdatePanel();
    GlobalNAStatus = 0;
}

void LevelUnused(void) {
}

void QuitLevel(void) {
    GlobalNAStatus = 0;
    if (D_8005AA80 != NULL) {
        D_8005AA80();
    }
    ReleaseHeroAnims();
    KillEveryNas();
}

void UpdateLevelTriggers(void) {
    RunLevelTriggers();
}

void ResetASTTable(void) {
    GlobalNAStatus = 0;
    ASTTable = EngineASTTable;
    ASTCount = ASTTableCount();
}

void ClearLevelNas(void) {
    GlobalNAStatus = 0;
    HeadOfTriggers = 0;
    InitTriggers();
    ClearAllNasPool();
    ResetNasLists();
}

int ASTTableCount(void) {
    unsigned int i;

    for (i = 0; i < 0xAE; i++) {
    }
    return i;
}

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */
extern u8 ModeTextBuffer[];
extern u8 SlotOverlayId[];

u8 *D_80098594 = (void *)&SlotOverlayId;
u8 *D_80098598 = (void *)&ModeTextBuffer;
s32 D_8009859C[9] = {0};
s32 D_800985C0[12] = {0};
s32 D_800985F0[45] = {
    (s32)(u8 *)&ModeTextBuffer + 0x3B44, (s32)(u8 *)&ModeTextBuffer + 0x3C1C,
    (s32)(u8 *)&ModeTextBuffer + 0x3CF4, (s32)(u8 *)&ModeTextBuffer + 0x3DCC,
    (s32)(u8 *)&ModeTextBuffer + 0x7B4, (s32)(u8 *)&ModeTextBuffer + 0x2C58,
    (s32)(u8 *)&ModeTextBuffer + 0x2A48, (s32)(u8 *)&ModeTextBuffer + 0x2BA8,
    (s32)(u8 *)&ModeTextBuffer + 0x2AF8, (s32)(u8 *)&ModeTextBuffer + 0x2CA0,
    (s32)(u8 *)&ModeTextBuffer + 0x3808, (s32)(u8 *)&ModeTextBuffer + 0x38B8,
    (s32)(u8 *)&ModeTextBuffer + 0x397C, (s32)(u8 *)&ModeTextBuffer + 0x3A40,
    (s32)(u8 *)&ModeTextBuffer + 0x2178, (s32)(u8 *)&ModeTextBuffer + 0x2228,
    (s32)(u8 *)&ModeTextBuffer + 0x22D8, (s32)(u8 *)&ModeTextBuffer + 0x2374,
    (s32)(u8 *)&ModeTextBuffer + 0x26B0, (s32)(u8 *)&ModeTextBuffer + 0x2600,
    (s32)(u8 *)&ModeTextBuffer + 0x2760, (s32)(u8 *)&ModeTextBuffer + 0x2820,
    (s32)(u8 *)&ModeTextBuffer + 0x2868, (s32)(u8 *)&ModeTextBuffer + 0x1FB4,
    (s32)(u8 *)&ModeTextBuffer + 0x2078, (s32)(u8 *)&ModeTextBuffer + 0x2200,
    (s32)(u8 *)&ModeTextBuffer + 0x213C, (s32)(u8 *)&ModeTextBuffer + 0x1850,
    (s32)(u8 *)&ModeTextBuffer + 0x1594, (s32)(u8 *)&ModeTextBuffer + 0x1828,
    (s32)(u8 *)&ModeTextBuffer + 0x17DC, (s32)(u8 *)&ModeTextBuffer + 0x544,
    (s32)(u8 *)&ModeTextBuffer + 0x3E24, (s32)&D_800985C0, (s32)&D_800985C0, (s32)&D_800985C0,
    (s32)&D_800985C0, (s32)&D_800985C0, (s32)&D_800985C0, (s32)&D_800985C0, (s32)&D_800985C0,
    (s32)&D_800985C0, (s32)&D_800985C0, (s32)&D_800985C0, (s32)&D_800985C0,
};
s32 D_800986A4[6] = {
    (s32)&D_800985F0, (s32)&D_800985F0, (s32)&D_800985F0, (s32)&D_800985F0, (s32)&D_800985F0,
    (s32)&D_800985F0,
};
u16 D_800986BC[32] = {
    4, 7, 6, 1, 3, 7, 2, 5, 2, 8, 1, 7, 6, 5, 3, 4, 5, 1, 8, 3, 8, 6, 7, 5, 1, 3, 4, 2, 4, 7,
    6, 1,
};
u16 D_800986FC[14] = { 0, 0x13, 0xA, 0xE, 6, 0x17, 0x17, 0x17, 0x17, 0, 0, 0, 0, 0 };
void *D_80098718 = 0;
u16 D_8009871C = 0;
/* the padding to the word asttable.c's table starts on: the engine links with SUBALIGN(2) */
s16 EngineDataPad = 0;
