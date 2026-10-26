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
    D_8005AA1C = (NAS *)func_80011654(D_8005AA14 * 0x70, &MainHeap);
    D_8005AA20 = (u8 *)D_8005AA1C + (D_8005AA14 * 0x70 - 0x70);
    D_8005AA40 = (EngObj **)func_80011654(D_8005AA14 * 4, &MainHeap);
    *D_8005AA40 = NULL;
    p = func_80011654(D_8005AA14 * 4, &MainHeap);
    D_8005AA44 = p;
    *(s32 *)p = 0;
    func_80026308();
    D_8005AAB0 = D_800986A4[v->unk2C];
}

void func_80078DD4(void) {
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
                    dx = -TrigTable[(ang - 0x400) & 0xFFF].unk2 * nas->Camera.look_ahead >> 12;
                    dz = -TrigTable[ang].unk2 * nas->Camera.look_ahead >> 12;
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

void func_8007906C(void) {
    s32 i;
    s32 n;
    NAS *nas;
    u16 cls;

    GlobalNAStatus = 1;
    *D_8005AA40 = NULL;
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
        nas = &D_8005AA1C[i];
        if (nas->NASid & 1) {
            cls = nas->Class;
            if (cls & 1) {
                nas->Handler(nas);
            }
            if (cls & 2) {
                n = ++*(s32 *)D_8005AA40;
                D_8005AA40[n] = (EngObj *)nas;
            }
            if (cls & 0x1000) {
                n = ++*(s32 *)D_8005AA44;
                ((NAS **)D_8005AA44)[n] = nas;
            }
        }
    }
    GlobalNAStatus = 0;
}

void func_80079260(void) {
    EngineGameView *g = (EngineGameView *)GameInfo;
    s32 r;
    s32 i;
    s32 want;
    s32 n;

    g->flags &= 0xF4FF;
    func_80024AD8();
    func_8001D6B4((MapEnvHead *)MapScene);
    func_80026ECC();
    func_8002321C();
    func_80026D9C();
    func_80083550();
    func_8008688C();
    if (LevelSpecs[0] != 0) {
        ((void (*)(void))LevelSpecs[0])();
    }
    GlobalNAStatus = 0;
    ASTTable = EngineASTTable;
    ASTCount = func_8007AC00();
    func_80083708();
    SetTweakAi();
    func_8007A6F8();
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

void func_800793E8(void) {
    s32 w;
    s32 i;
    s32 j;
    s32 n;
    s32 found;
    s32 off;
    u16 v;
    u8 *g;
    u8 *p;

    switch (ModeTypes[D_8009E5DC * 3]) {
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

void func_80079554(void) {
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
    if (D_800515F8 < g->count) {
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
                        if (D_800515F8 < 3) {
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

void func_8007976C(void) {
    s32 i;
    EngObj *o;

    func_80011410(&MainHeap);
    func_80079554();
    if (D_8005A618 & 0x10) {
        FntPrint("\t\tFree Memory  - tot %d  max %d\n\0\x18\0", D_8004E104, D_8004E10C);
    }
    func_8007906C();
    func_80083E8C();
    GlobalNAStatus = 2;
    i = 0;
    if (*(s32 *)D_8005AA40 > 0) {
        do {
            i++;
            o = D_8005AA40[i];
            if (o->flags & 1) {
                o->update(o);
            }
        } while (i < *(s32 *)D_8005AA40);
    }
    GlobalNAStatus = 0;
    Draw_Clock();
    func_8007AF3C();
    if (PanelCtrl & 0x4000) {
        func_8007D6D0();
    }
    func_80081F64(1);
    func_8009608C(0);
}

void func_800798A4(void) {
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
            tim = D_80053074;
            c0 = 0xFFFFFF;
            c1 = 0xFFFF;
            pos.f.y = D_80099700[1];
            for (i = 0; i < 4; i++) {
                if (D_800996E0[i].flags & 0x8000) {
                    pos.f.x = D_80099700[0] + D_800996E0[i].arg;
                    func_8002992C((obj_texture *)(*(u8 **)(tim + 0x18) + (D_800996E0[i].cmd & 0xFFF) * 0x38),
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
            tim = D_80053074;
            c0 = 0xFFFFFF;
            c1 = 0xFFFF;
            pos.f.y = D_80099700[1];
            for (i = 0; i < 4; i++) {
                if (D_800996E0[i].flags & 0x8000) {
                    pos.f.x = D_80099700[0] + D_800996E0[i].arg;
                    func_8002992C((obj_texture *)(*(u8 **)(tim + 0x18) + (D_800996E0[i].cmd & 0xFFF) * 0x38),
                                  pos.w, 0, c0, c0, c1, c1);
                }
            }
        }
    } else if ((PanelCtrl & 1) && (Clock_flags & 0x8000)) {
        tim = D_80053074;
        c0 = 0xD0FF;
        c1 = 0xFF;
        pos.f.y = Clock_XYZ[1];
        if (!(Clock_flags & 2) && (HUDGAME->flags & 1)) {
            Clock[0].Ctrl |= 0x8000;
        }
        for (i = 0; i < 4; i++) {
            if (Clock[i].Ctrl & 0x8000) {
                pos.f.x = Clock_XYZ[0] + Clock[i].Xoffs;
                func_8002992C((obj_texture *)(*(u8 **)(tim + 0x18) + (Clock[i].frame & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
            }
        }
    }
    if (countdown_eng != NULL) {
        func_8001C298((EXTengine *)countdown_eng);
    }
    if (PanelCtrl & 0x1000) {
        func_8001C298(&BigScore1);
        func_8001C298(&BigScore2);
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
            func_8001C298(HUDPLAYERS[i]);
        }
        if (PanelCtrl & 0x400) {
            if (Score[i].Ctrl != 0) {
                tim = D_80053074;
                c0 = 0xD0FF;
                c1 = 0xFF;
                pos.f.x = HUDPLAYERS[i]->unk10.x + Score[i].Xoffs;
                pos.f.y = HUDPLAYERS[i]->unk10.y + Score[i].Yoffs;
                n = Score[i].score;
                func_8002992C((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
                n /= 10;
                pos.f.x -= 0x18;
                func_8002992C((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
                n /= 10;
                pos.f.x -= 0x18;
                func_8002992C((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
            }
        }
        if (PanelCtrl & 0x200) {
            if (Score[i].Ctrl != 0) {
                tim = D_80053074;
                c0 = 0xD0FF;
                c1 = 0xFF;
                pos.f.x = HUDPLAYERS[i]->unk10.x + Score[i].Xoffs;
                pos.f.y = HUDPLAYERS[i]->unk10.y + Score[i].Yoffs;
                n = Score[i].score;
                func_8002992C((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
                n /= 10;
                pos.f.x -= 0x18;
                func_8002992C((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                              pos.w, 0, c0, c0, c1, c1);
            }
        }
        if ((PanelCtrl & 0x100) && Score[i].Ctrl != 0) {
            tim = D_80053074;
            c0 = 0xD0FF;
            c1 = 0xFF;
            n = Score[i].score;
            pos.f.x = HUDPLAYERS[i]->unk10.x + Score[i].Xoffs;
            pos.f.y = HUDPLAYERS[i]->unk10.y + Score[i].Yoffs;
            func_8002992C((obj_texture *)(*(u8 **)(tim + 0x18) + ((n % 10 + 0x3017) & 0xFFF) * 0x38),
                          pos.w, 0, c0, c0, c1, c1);
        }
        i++;
    } while (i < 4);
    if (!(GamePause & 1) && (PanelCtrl & 0x4000)) {
        func_80018B08(&D_8009F6A4, &D_8009F7EC);
        func_8001C2C0((u8 *)GetEngineSlot(5));
    }
}

s32 func_8007A164(arg0, arg1)
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
    eng->unk10.x -= m->Xvel * TrigTable[(eng->unk1E - 0x400) & 0xFFF].unk2 >> 12;
    eng->unk10.z -= m->Xvel * TrigTable[eng->unk1E & 0xFFF].unk2 >> 12;
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
    if (func_80025BBC(eng) == 0) {
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

NAS *func_8007A464(s32 arg0, s32 *arg1, s32 arg2) {
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

NAS *func_8007A524(s32 arg0, s32 *arg1, s32 arg2) {
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

s32 func_8007A5E8(u8 *arg0) {
    s32 sum = 0;
    s32 i = 0;
    do {
        sum += (arg0 + i)[0xE4];
        i++;
    } while (i < 0x21);
    return sum;
}

void func_8007A614(s32 arg0) {
    u8 *p = GameInfo;
    func_8007A164(D_800986FC[arg0 & 0xFFFF], p);
}

void func_8007A650(s32 arg0) {
    func_8007A164(D_800986FC[arg0 & 0xFFFF]);
}

void func_8007A688(s32 arg0) {
    func_8007A164(arg0 & 0xFFFF, GameInfo);
}

void func_8007A6B0(s32 arg0) {
    func_8007A164(arg0 & 0xFFFF);
}

void func_8007A6D0(s32 arg0) {
    func_8007A164(arg0 & 0xFFFF, GameInfo);
}

void func_8007A6F8(void) {
    func_8002617C();
}

void func_8007A718(void) {
    NAS *nas;
    EXTengine *eng;

    for (nas = NASActiveHead; nas != NULL; nas = nas->Tag.Child) {
        eng = nas->Engine;
        if (eng != NULL && (nas->NASid & 1)) {
            do {
                if (!(nas->Class & 0x2000)) {
                    eng->OldXYZ = eng->unk10;
                }
                func_8001C298(eng);
                eng = eng->Engine;
            } while (eng != NULL);
        }
    }
}

void func_8007A7C0(void) {
    func_80011410(&MainHeap);
    func_80079554();
    GlobalNAStatus = 4;
    func_8007AF3C();
    GlobalNAStatus = 0;
    func_80081F64(0);
}

void func_8007A808(void) {
    s32 i;
    s32 r;
    s32 want;

    GlobalNAStatus = 0;
    ASTTable = EngineASTTable;
    ASTCount = func_8007AC00();
    func_80083708();
    SetTweakAi();
    func_8007A6F8();
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

void func_8007A8C4(void) {
    EngineGameView *g = (EngineGameView *)GameInfo;

    g->flags &= 0xF4FF;
    func_80083550();
    func_8008688C();
    if (LevelSpecs[0] != 0) {
        ((void (*)(void))LevelSpecs[0])();
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

void func_8007AA18(void) {
    s32 i;
    EngObj *o;

    GlobalNAStatus = 2;
    i = 0;
    if (*(s32 *)D_8005AA40 > 0) {
        do {
            i++;
            o = D_8005AA40[i];
            if (o->flags & 1) {
                o->update(o);
            }
        } while (i < *(s32 *)D_8005AA40);
    }
    GlobalNAStatus = 0;
    Draw_Clock();
    func_8007AF3C();
    if (PanelCtrl & 0x4000) {
        func_8007D6D0();
    }
}

void func_8007AAF0(void) {
    GlobalNAStatus = 4;
    func_8007AF3C();
    GlobalNAStatus = 0;
}

void func_8007AB20(void) {
}

void func_8007AB28(void) {
    GlobalNAStatus = 0;
    if (D_8005AA80 != NULL) {
        D_8005AA80();
    }
    func_80086814();
    func_80026FCC();
}

void func_8007AB6C(void) {
    func_8007A6F8();
}

void func_8007AB8C(void) {
    GlobalNAStatus = 0;
    ASTTable = EngineASTTable;
    ASTCount = func_8007AC00();
}

void func_8007ABC4(void) {
    GlobalNAStatus = 0;
    D_8005AA48 = 0;
    func_80024AD8();
    func_80026ECC();
    func_80026D9C();
}

int func_8007AC00(void) {
    unsigned int i;

    for (i = 0; i < 0xAE; i++) {
    }
    return i;
}
