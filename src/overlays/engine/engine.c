#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/hero.h"
#include "bash/shell.h"

void InitialiseNasSystem(void) {
    EngineRec32 *rec;
    GameState *v;
    u8 *p;

    rec = LevelTriggerData->recs;
    LevelRecs = rec;
    if (rec != NULL) {
        GameState *w = (GameState *)GameInfo;
        w->levelLoadModel = *(u16 *)&rec[w->levelRecIndex];
    }
    v = (GameState *)GameInfo;
    if (v->boot != NULL) {
        v->boot(rec);
    }
    SetNasDefaults();
    Init_Panel();
    NasPool = (NAS *)mal_alloc(NasPoolUsed * 0x70, &MainHeap);
    NasPoolLast = &NasPool[NasPoolUsed - 1];
    NasUpdateList = (EngObj **)mal_alloc(NasPoolUsed * 4, &MainHeap);
    *NasUpdateList = NULL;
    p = mal_alloc(NasPoolUsed * 4, &MainHeap);
    HeroNasList = p;
    *(s32 *)p = 0;
    InitSysFontModel();
    PauseMenuSets = PauseMenuSetsByLang[v->language];
}

void BuildCameraFrameList(void) {
    NAS *nas;
    EXTengine *eng;
    GameState *g;
    s32 ang;
    s32 dx;
    s32 dz;
    s32 w;
    s32 l;
    s32 r;
    s32 t;

    nas = (NAS *)HeadOfCameras;
    camera_frame_list_size = 0;
    if (nas != NULL) {
        g = (GameState *)GameInfo;
        do {
            eng = nas->Engine;
            if (eng != NULL && (nas->Camera.flags & 0x8000)) {
                w = nas->Camera.width;
                r = w;
                if (!(g->exitFlags & 0x801)) {
                    ang = eng->xform.rot.vy & 0xFFF;
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
                    camera_add_frame_list((s32)&eng->XYZ, l, r, dx >> 1, t >> 1, nas->Camera.height);
                } else {
                    dx = nas->Camera.height;
                    dx -= (u32)dx >> 2;
                    camera_add_frame_list((s32)&eng->XYZ, w >> 1, w >> 1, 0, 0, dx);
                }
            }
            nas = nas->Camera.Child;
        } while (nas != NULL);
    }
    {
        GameState *g1 = (GameState *)GameInfo;
        if ((g1->cheats & 0x200) && !(g1->exitFlags & 0x801)) {
            camera_add_frame_list((s32)&MapPoints[10], 0x80, 0x80, 0, 0, 0x80);
            camera_add_frame_list((s32)&MapPoints[11], 0x80, 0x80, 0, 0, 0x80);
        }
    }
    {
        GameState *g2 = (GameState *)GameInfo;
        if (g2->levelFlags & 0x800) {
            if (!(g2->exitFlags & 0x800)) {
                camera_add_frame_list((s32)&MapPoints[12], 0x80, 0x80, 0, 0, 0x80);
                camera_add_frame_list((s32)&MapPoints[13], 0x80, 0x80, 0, 0, 0x80);
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
    *(s32 *)HeroNasList = 0;
    i = 0;
    if (LevelSpecs.LevelRoutine != NULL) {
        LevelSpecs.LevelRoutine();
    }
    do {
        if (Heroes[i].hero->Class & 0x1000) {
            n = ++*(s32 *)HeroNasList;
            ((NAS **)HeroNasList)[n] = Heroes[i].hero;
        }
        i++;
    } while (i < 4);
    for (i = FirstNas; i < NasPoolSlots; i++) {
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
                n = ++*(s32 *)HeroNasList;
                ((NAS **)HeroNasList)[n] = nas;
            }
        }
    }
    GlobalNAStatus = 0;
}

void InitialiseLevel(void) {
    GameState *g = (GameState *)GameInfo;
    s32 r;
    s32 i;
    s32 want;
    s32 n;

    g->levelFlags &= 0xF4FF;
    InitTriggers();
    mapenv_place_subs(MapScene);
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
    CratePickupAst = r;
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
    HealthAst = r;
    if (LevelSpecs.LevelInit != NULL) {
        LevelSpecs.LevelInit();
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
    GameState *g;
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
    g = GAME;
    for (; i < 4; i++) {
        if (i != 0 && (g->teams == 0 || i != 1)) {
            found = 1;
            while (n < 6) {
                p = (u8 *)(i + (s32)g);  /* matching: as an int sum: g + i swaps two registers */
                if (found == 0) {
                    break;
                }
                v = (&CpuHeroChoices[w * 4])[n];
                n++;
                found = 0;
                for (j = 0; j < 4; j++) {
                    if ((j == 0 || (g->teams != 0 && j == 1)) && ((u8 *)g)[j + 0x50] == v) {  /* heroes[j], added the other way round */
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
    GameState *g = (GameState *)GameInfo;
    HudEntry *item;
    u16 *levels;
    s32 i;
    s16 y;

    if (g->adventure.active != 0 || g->demo != 0) {
        return;
    }
    levels = ModeTypes;
    if (levels[g->mode * 3] == 0x101) {
        return;
    }
    item = PauseUnpluggedMenu;
    g->pause &= ~0x4000;
    item[1].type = -1;
    item[2].type = -1;
    item[3].type = -1;
    item[4].type = -1;
    y = 0x72;
    if (PadCount < g->players) {
        i = 0;
        if (g->players != 0) {
            u16 *lv = levels;
            GameState *g1 = (GameState *)GameInfo;

            do {
                if (lv[g1->mode * 3] == 0x102) {
                    g1->pause |= 0x4009;
                } else {
                    g1->pause |= 0x4001;
                }
                PausePad = 0;
                item->type = 5;
                item->idx = i + 0x1C;
                item++;
                y += 0x12;
                i++;
            } while (i < g1->players);
        }
    } else {
        i = 0;
        if (g->players != 0) {
            u16 *lv2;
            GameState *g2;
            lv2 = levels;
            g2 = (GameState *)GameInfo;
            do {
                if (!(Pads[i].flags & 1)) {
                    if (lv2[g2->mode * 3] == 0x102) {
                        g2->pause |= 0x4009;
                    } else {
                        g2->pause |= 0x4001;
                    }
                    item->type = 5;
                    PausePad = 0;
                    if (g2->players < 3) {
                        if (PadCount < 3) {
                            item->idx = i + 0x1A;
                        } else {
                            item->idx = i + 0x1C;
                        }
                    } else {
                        item->idx = i + 0x1C;
                    }
                    item++;
                    y += 0x12;
                }
                i++;
            } while (i < g2->players);
        }
    }
    item->idx = 0x20;
    item->type = 5;
    PausePadMessage[0].y = y;
    PausePadMessage[0].idx = y;
}

void UpdateLevel(void) {
    s32 i;
    EngObj *o;

    mal_count_free(&MainHeap);
    CheckControllers();
    if (GameCheats & 0x10) {
        FntPrint("\t\tFree Memory  - tot %d  max %d\n\0\x18\0", MalFreeBytes, MalLargestFree);
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
    obj_dbase *tim;

    if (ModeTypes[GAME->mode * 3] == 0x102) {
        show = 1;
        if (!(GAME->pause & 1)) {
            show = 0;
        } else if (GAME->pause & 0x0A007662) {
            show = 0;
        }
        if (GameLevelFlags & 3) {
            show = 0;
        }
        if (show && (PanelCtrl & 0x20000)) {
            tim = &HeroModelBank[DB_Interface];
            c0 = 0xFFFFFF;
            c1 = 0xFFFF;
            pos.f.y = CompletionPos[1];
            for (i = 0; i < 4; i++) {
                if (CompletionDigits[i].flags & 0x8000) {
                    pos.f.x = CompletionPos[0] + CompletionDigits[i].arg;
                    texture_draw_quad((obj_texture *)&tim->textures.tex[CompletionDigits[i].cmd & 0xFFF],
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
    if (GAME->levelFlags & 3) {
        show = 0;
    }
    if (show) {
        if (PanelCtrl & 0x20000) {
            tim = &HeroModelBank[DB_Interface];
            c0 = 0xFFFFFF;
            c1 = 0xFFFF;
            pos.f.y = CompletionPos[1];
            for (i = 0; i < 4; i++) {
                if (CompletionDigits[i].flags & 0x8000) {
                    pos.f.x = CompletionPos[0] + CompletionDigits[i].arg;
                    texture_draw_quad((obj_texture *)&tim->textures.tex[CompletionDigits[i].cmd & 0xFFF],
                                  pos.w, 0, c0, c0, c1, c1);
                }
            }
        }
    } else if ((PanelCtrl & 1) && (Clock_flags & 0x8000)) {
        tim = &HeroModelBank[DB_Interface];
        c0 = 0xD0FF;
        c1 = 0xFF;
        pos.f.y = Clock_XYZ[1];
        if (!(Clock_flags & 2) && (GAME->pause & 1)) {
            Clock[0].Ctrl |= 0x8000;
        }
        for (i = 0; i < 4; i++) {
            if (Clock[i].Ctrl & 0x8000) {
                pos.f.x = Clock_XYZ[0] + Clock[i].Xoffs;
                texture_draw_quad((obj_texture *)&tim->textures.tex[Clock[i].frame & 0xFFF],
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
            if (PauseFlashItem == i && (FrameCount & 8)) {
                s->flags = v | 0x8000;
            }
            eng_render(HUDPLAYERS[i]);
        }
        if (PanelCtrl & 0x400) {
            if (Score[i].Ctrl != 0) {
                tim = &HeroModelBank[DB_Interface];
                c0 = 0xD0FF;
                c1 = 0xFF;
                pos.f.x = HUDPLAYERS[i]->XYZ.x + Score[i].Xoffs;
                pos.f.y = HUDPLAYERS[i]->XYZ.y + Score[i].Yoffs;
                n = Score[i].score;
                texture_draw_quad((obj_texture *)&tim->textures.tex[(n % 10 + 0x3017) & 0xFFF],
                              pos.w, 0, c0, c0, c1, c1);
                n /= 10;
                pos.f.x -= 0x18;
                texture_draw_quad((obj_texture *)&tim->textures.tex[(n % 10 + 0x3017) & 0xFFF],
                              pos.w, 0, c0, c0, c1, c1);
                n /= 10;
                pos.f.x -= 0x18;
                texture_draw_quad((obj_texture *)&tim->textures.tex[(n % 10 + 0x3017) & 0xFFF],
                              pos.w, 0, c0, c0, c1, c1);
            }
        }
        if (PanelCtrl & 0x200) {
            if (Score[i].Ctrl != 0) {
                tim = &HeroModelBank[DB_Interface];
                c0 = 0xD0FF;
                c1 = 0xFF;
                pos.f.x = HUDPLAYERS[i]->XYZ.x + Score[i].Xoffs;
                pos.f.y = HUDPLAYERS[i]->XYZ.y + Score[i].Yoffs;
                n = Score[i].score;
                texture_draw_quad((obj_texture *)&tim->textures.tex[(n % 10 + 0x3017) & 0xFFF],
                              pos.w, 0, c0, c0, c1, c1);
                n /= 10;
                pos.f.x -= 0x18;
                texture_draw_quad((obj_texture *)&tim->textures.tex[(n % 10 + 0x3017) & 0xFFF],
                              pos.w, 0, c0, c0, c1, c1);
            }
        }
        if ((PanelCtrl & 0x100) && Score[i].Ctrl != 0) {
            tim = &HeroModelBank[DB_Interface];
            c0 = 0xD0FF;
            c1 = 0xFF;
            n = Score[i].score;
            pos.f.x = HUDPLAYERS[i]->XYZ.x + Score[i].Xoffs;
            pos.f.y = HUDPLAYERS[i]->XYZ.y + Score[i].Yoffs;
            texture_draw_quad((obj_texture *)&tim->textures.tex[(n % 10 + 0x3017) & 0xFFF],
                          pos.w, 0, c0, c0, c1, c1);
        }
        i++;
    } while (i < 4);
    if (!(GamePause & 1) && (PanelCtrl & 0x4000)) {
        gfx_set_viewport(&HudViewport, &HudCamera);
        eng_render_subs(GetEngineSlot(5));
    }
}

s32 LevelRequirementMet(arg0, arg1)
s32 arg0;
GameState *arg1;
{
    s32 sa, sb, sc, sd;
    s32 i;
    EngineReq *r;
    EngineReq *tab;

    if ((arg1->cheats & 0x80002000) == 0x80002000) {
        return 1;
    }
    sd = 0;
    sb = 0;
    sc = 0;
    sa = 0;
    i = 0;
    do {
        sa += arg1->adventure.won[0][i];
        sc += arg1->adventure.won[2][i];
        sb += arg1->adventure.won[1][i];
        sd += arg1->adventure.won[3][i];
        i++;
    } while (i < 0x21);
    tab = LevelRequirements;
    r = &tab[arg0 & 0xFFFF];
    if (sa < r->min0 || sc < r->min4 || sb < r->min2 || sd < r->min6) {
        return 0;
    }
    if (r->off != 0 && arg1->adventure.won[4][r->off] == 0) {
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
    eng->XYZ.x -= m->Xvel * rcossin_tbl[(eng->xform.rot.vy - 0x400) & 0xFFF].unk2 >> 12;
    eng->XYZ.z -= m->Xvel * rcossin_tbl[eng->xform.rot.vy & 0xFFF].unk2 >> 12;
    m->Yvel += m->Yacc - m->Ydec;
    if (m->Yvel < 0) {
        m->Yvel = 0;
    }
    if (m->Yvel > m->Ymax) {
        m->Yvel = m->Ymax;
    }
    if (m->Ydir) {
        eng->XYZ.y -= m->Yvel;
    } else {
        eng->XYZ.y += m->Yvel;
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

s32 CountTrophies(GameState *arg0) {
    s32 sum = 0;
    s32 i = 0;
    do {
        sum += arg0->adventure.won[0][i];
        i++;
    } while (i < 0x21);
    return sum;
}

void ArenaRequirementMet(s32 arg0) {
    u8 *p = GameInfo;
    LevelRequirementMet(ArenaRequirements[arg0 & 0xFFFF], p);
}

void ArenaRequirementMetAny(s32 arg0) {
    LevelRequirementMet(ArenaRequirements[arg0 & 0xFFFF]);
}

void RequirementMet(s32 arg0) {
    LevelRequirementMet(arg0 & 0xFFFF, GameInfo);
}

void RequirementMetAny(s32 arg0) {
    LevelRequirementMet(arg0 & 0xFFFF);
}

s32 ModeRequirementMet(u16 mode) {
    return LevelRequirementMet(mode, GameInfo);
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
                    eng->OldXYZ = eng->XYZ;
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
    HealthAst = r;
    if (LevelSpecs.LevelInit != NULL) {
        LevelSpecs.LevelInit();
    }
}

void PreInitLevel(void) {
    GameState *g = (GameState *)GameInfo;

    g->levelFlags &= 0xF4FF;
    SetDefaultHeroHooks();
    SetLevelCameraMode();
    if (LevelSpecs.LevelPreInit != NULL) {
        LevelSpecs.LevelPreInit();
    }
}

void DoAmbience(void) {
    if (AmbienceTimer != 0) {
        AmbienceTimer = AmbienceTimer - 1;
    } else {
        PlaySfx(AmbienceDef->Random[Random(8)], 0, 0, 0x1000, 0x1E00, 0);
        AmbienceTimer = (AmbienceDef->min_delay + Random(AmbienceDef->max_delay - AmbienceDef->min_delay)) * 30;
    }
}

void InitAmbience(Ambient_data *arg0) {
    AmbienceDef = arg0;
    AmbienceTimer = 0;
    PlaySfx(arg0->Background1, 0, 0, 0x1000, 0x1E00, 0);
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
    if (LevelSpecs.LevelExit != NULL) {
        LevelSpecs.LevelExit();
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

u8 *SlotBase = (void *)&SlotOverlayId;
u8 *ModeTextLoadAddr = (void *)&ModeTextBuffer;
s32 ModeTextUnused[9] = {0};
s32 ModeTextEmpty[12] = {0};
PlaceRec *** PauseMenuTexts[45] = {  /* the pause menu's texts in the mode text buffer */
    (PlaceRec ***)(ModeTextBuffer + 0x3B44), (PlaceRec ***)(ModeTextBuffer + 0x3C1C),
    (PlaceRec ***)(ModeTextBuffer + 0x3CF4), (PlaceRec ***)(ModeTextBuffer + 0x3DCC),
    (PlaceRec ***)(ModeTextBuffer + 0x7B4), (PlaceRec ***)(ModeTextBuffer + 0x2C58),
    (PlaceRec ***)(ModeTextBuffer + 0x2A48), (PlaceRec ***)(ModeTextBuffer + 0x2BA8),
    (PlaceRec ***)(ModeTextBuffer + 0x2AF8), (PlaceRec ***)(ModeTextBuffer + 0x2CA0),
    (PlaceRec ***)(ModeTextBuffer + 0x3808), (PlaceRec ***)(ModeTextBuffer + 0x38B8),
    (PlaceRec ***)(ModeTextBuffer + 0x397C), (PlaceRec ***)(ModeTextBuffer + 0x3A40),
    (PlaceRec ***)(ModeTextBuffer + 0x2178), (PlaceRec ***)(ModeTextBuffer + 0x2228),
    (PlaceRec ***)(ModeTextBuffer + 0x22D8), (PlaceRec ***)(ModeTextBuffer + 0x2374),
    (PlaceRec ***)(ModeTextBuffer + 0x26B0), (PlaceRec ***)(ModeTextBuffer + 0x2600),
    (PlaceRec ***)(ModeTextBuffer + 0x2760), (PlaceRec ***)(ModeTextBuffer + 0x2820),
    (PlaceRec ***)(ModeTextBuffer + 0x2868), (PlaceRec ***)(ModeTextBuffer + 0x1FB4),
    (PlaceRec ***)(ModeTextBuffer + 0x2078), (PlaceRec ***)(ModeTextBuffer + 0x2200),
    (PlaceRec ***)(ModeTextBuffer + 0x213C), (PlaceRec ***)(ModeTextBuffer + 0x1850),
    (PlaceRec ***)(ModeTextBuffer + 0x1594), (PlaceRec ***)(ModeTextBuffer + 0x1828),
    (PlaceRec ***)(ModeTextBuffer + 0x17DC), (PlaceRec ***)(ModeTextBuffer + 0x544),
    (PlaceRec ***)(ModeTextBuffer + 0x3E24), (s32)&ModeTextEmpty, (s32)&ModeTextEmpty, (s32)&ModeTextEmpty,
    (s32)&ModeTextEmpty, (s32)&ModeTextEmpty, (s32)&ModeTextEmpty, (s32)&ModeTextEmpty, (s32)&ModeTextEmpty,
    (s32)&ModeTextEmpty, (s32)&ModeTextEmpty, (s32)&ModeTextEmpty, (s32)&ModeTextEmpty,
};
PlaceRec **** PauseMenuSetsByLang[6] = {
    PauseMenuTexts, PauseMenuTexts, PauseMenuTexts, PauseMenuTexts, PauseMenuTexts,
    PauseMenuTexts,
};
u16 CpuHeroChoices[32] = {
    4, 7, 6, 1, 3, 7, 2, 5, 2, 8, 1, 7, 6, 5, 3, 4, 5, 1, 8, 3, 8, 6, 7, 5, 1, 3, 4, 2, 4, 7,
    6, 1,
};
u16 ArenaRequirements[9] = { 0, 0x13, 0xA, 0xE, 6, 0x17, 0x17, 0x17, 0x17 };
u16 CreditsStep = 0;
u16 CreditsTimer = 0;
u16 CreditsUnused = 0;
s32 CreditsActive = 0;
struct Ambient_data *AmbienceDef = 0;
u16 AmbienceTimer = 0;
/* the padding to the word asttable.c's table starts on: the engine links with SUBALIGN(2) */
s16 EngineDataPad = 0;
