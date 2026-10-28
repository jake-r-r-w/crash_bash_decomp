#include "common.h"
#include "bash/main.h"
#include "bash/nascalls.h"

static inline NAS *AllocNAS(void) {
    NAS *free;
    NAS *next;

    free = NASFreeList;
    if (free == NULL) {
        return NULL;
    }
    next = free->Tag.Child;
    NASFreeList = next;
    if (next != NULL) {
        next->Tag.Parent = NULL;
    }
    return free;
}

NAS *CreateNAS(s32 x, s32 y, s32 z, u16 id, u16 flags) {
    NAS *nas;
    EXTengine *eng;
    s32 i;
    s32 r;
    s32 want;
    u16 ast;

    nas = NULL;
    want = id;
    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == want) {
            r = i;
            goto found;
        }
    }
    r = 0;
found:
    ast = r;
    if (ast != 0 && ast < ASTCount) {
        nas = AllocNAS();
        if (nas != NULL) {
            *nas = NASTemplate;
            nas->NASid = 1;
            if (NASActiveTail != NULL) {
                NASActiveTail->Tag.Child = nas;
                nas->Tag.Parent = NASActiveTail;
                NASActiveTail = nas;
            } else {
                NASActiveTail = nas;
                NASActiveHead = nas;
            }
            nas->Type = ast;
            nas->InitDelete = ASTTable[ast].Init;
            nas->Handler = ASTTable[ast].Plot;
            if (flags & 1) {
                eng = AddExtEngine(nas, 1);
                if (eng != NULL) {
                    eng->unk10.x = x;
                    eng->unk10.y = y;
                    eng->unk10.z = z;
                } else {
                    if (flags & 0x10) {
                        KillNAS(nas);
                    }
                    return NULL;
                }
            }
            nas->InitDelete(nas);
            if (nas->NASid == 0) {
                nas = NULL;
            }
        }
    }
    return nas;
}

NAS *CreateASTNas(s32 x, s32 y, s32 z, u16 id, s32 slot) {
    NAS *nas;
    EXTengine *eng;
    s32 i;
    s32 r;
    s32 want;
    u16 ast;

    nas = NULL;
    want = id;
    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == want) {
            r = i;
            goto found;
        }
    }
    r = 0;
found:
    ast = r;
    if (ast != 0 && ast < ASTCount) {
        nas = &NasPool[slot];
        *nas = NASTemplate;
        nas->NASid = 1;
        if (NASActiveTail != NULL) {
            NASActiveTail->Tag.Child = nas;
            nas->Tag.Parent = NASActiveTail;
            NASActiveTail = nas;
        } else {
            NASActiveTail = nas;
            NASActiveHead = nas;
        }
        nas->Type = ast;
        nas->InitDelete = ASTTable[ast].Init;
        nas->Handler = ASTTable[ast].Plot;
        eng = AddExtEngine(nas, 1);
        if (eng == NULL) {
            return NULL;
        }
        eng->unk10.x = x;
        eng->unk10.y = y;
        eng->unk10.z = z;
        nas->InitDelete(nas);
    }
    return nas;
}

NAS *TriggerNas(NASTrigger *trig) {
    NAS *nas;
    EXTengine *eng;
    s32 i;
    s32 type;
    u8 dir;

    nas = NULL;
    type = trig->Type;
    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == type) {
            goto found;
        }
    }
    i = 0;
found:
    if (i > 0 && i < ASTCount) {
        nas = AllocNAS();
        if (nas != NULL) {
            *nas = NASTemplate;
            nas->NASid = 1;
            if (NASActiveTail != NULL) {
                NASActiveTail->Tag.Child = nas;
                nas->Tag.Parent = NASActiveTail;
                NASActiveTail = nas;
            } else {
                NASActiveTail = nas;
                NASActiveHead = nas;
            }
            nas->Type = i;
            nas->trigger = trig;
            trig->Status |= 1;
            nas->InitDelete = ASTTable[i].Init;
            nas->Handler = ASTTable[i].Plot;
            eng = AddExtEngine(nas, 1);
            if (eng != NULL) {
                eng->unk10.x = trig->TrigXYZ.x;
                eng->unk10.y = trig->TrigXYZ.y;
                eng->unk10.z = trig->TrigXYZ.z;
                dir = 0;
                if (trig->Status & 0x20) {
                    dir = 0xFF;
                }
                eng->direction = dir;
            } else {
                KillNAS(nas);
                return NULL;
            }
            nas->InitDelete(nas);
            if (nas->NASid == 0) {
                nas = NULL;
            }
        }
    }
    return nas;
}


s32 KillNAS(NAS *nas) {
    EXTengine *eng;

    if (!(nas->NASid & 1)) {
        return 1;
    }
    if (nas->trigger != NULL && (nas->trigger->Status & 0x10)) {
        return 0;
    }
    if (nas->Class & 4) {
        nas->InitDelete(nas);
    }
    if (nas->trigger != NULL) {
        nas->trigger->Status &= ~1;
    }
    eng = nas->Engine;
    for (;;) {
        if (eng == NULL) {
            goto freed;
        }
        if (eng->platform != NULL) {
            eng->platform->platform = NULL;
        }
        eng->platform = NULL;
        eng_kill(eng);
        eng = eng->Engine;
    }
freed:
    FreeAllExtensions(nas);
    RemoveFamily((u8 *)nas);
    RemoveFromGroup(nas);
    RemoveCamera(nas);
    UnlinkObject(nas);
    RemoveSolid(nas);
    if (nas->Tag.Parent != NULL) {
        nas->Tag.Parent->Tag.Child = nas->Tag.Child;
        if (nas->Tag.Child != NULL) {
            nas->Tag.Child->Tag.Parent = nas->Tag.Parent;
        } else {
            NASActiveTail = nas->Tag.Parent;
        }
    }
    nas->NASid = 0;
    if (NASFreeList != NULL) {
        NASFreeList->Tag.Parent = nas;
        nas->Tag.Child = NASFreeList;
        NASFreeList = nas;
    } else {
        NASFreeList = nas;
        nas->Tag.Child = NULL;
    }
    nas->Tag.Parent = NULL;
    return 1;
}

void InitNasHeap(s32 arg0) {
    s32 *base;
    s32 *hd;
    u32 size;
    u32 words;

    s32 *pb = &D_8006EA00;
    u8 *res;

    size = (arg0 + 3) & ~3;
    D_8006EA04 = size;
    res = mal_alloc(size, &MainHeap);
    *pb = (s32)res;
    if (res != NULL) {
        base = (s32 *)res;
        hd = D_8005A7A0;
        hd[0] = 0;
        hd[1] = (s32)base;
        hd[2] = 0;
        hd[3] = (s32)base;
        hd[4] = size | 2;
        hd[5] = 0;
        hd[6] = (s32)base;
        hd[7] = (s32)base + size;
        size -= 0x18;
        base[0] = (s32)hd;
        base[1] = 0;
        base[2] = (s32)hd;
        base[3] = 0;
        base[4] = size | 1;
        base[5] = 0;
        size = (size - 0x18) >> 2;
        base += 6;
        do {
            *base = 0;
            size--;
            base++;
        } while (size != 0);
    }
}

void ResetNasHeap(void) {
    s32 *base;
    s32 *hd;
    u32 size;
    u32 flag;
    u32 words;

    size = D_8006EA04;
    if (D_8006EA00 != 0) {
        base = (s32 *)D_8006EA00;
        flag = size | 2;
        words = size >> 2;
        size -= 0x18;
        hd = D_8005A7A0;
        hd[0] = 0;
        hd[1] = (s32)base;
        hd[2] = 0;
        hd[3] = (s32)base;
        hd[4] = flag;
        hd[5] = 0;
        hd[6] = (s32)base;
        hd[7] = (s32)base + words * 4;
        base[0] = (s32)hd;
        base[1] = 0;
        base[2] = (s32)hd;
        base[3] = 0;
        base[4] = size | 1;
        base[5] = 0;
        size = (size - 0x18) >> 2;
        base += 6;
        do {
            *base = 0;
            size--;
            base++;
        } while (size != 0);
    }
}

void *AllocExtension(void *owner, s32 size, s32 id, void *tmpl) {
    ExtBlk *b;
    ExtBlk *n;
    ExtBlk *p;
    ExtBlk *r;
    ExtBlk *q;
    EXT *e;
    EXT *o;
    s32 want;
    s32 total;
    u32 have;

    want = (size + 3) & ~3;
    total = (size + 0x1B) & ~3;
    n = D_8005A7AC;
    for (;;) {
        if (n == NULL) {
            goto fail;
        }
        have = n->size & ~3;
        b = n;
        if ((s32)have >= want) {
        r = n;
        if (total + 8 < (s32)have) {
            n = (ExtBlk *)((u8 *)n + total);
            p = b->next;
            if (p != NULL) {
                p->prev = n;
            }
            n->next = p;
            n->prev = b;
            b->next = n;
            b->size = want;
            have -= total;
            n->size = have | 1;
            p = b->fprev;
            n->fprev = p;
            if (p != NULL) {
                p->fnext = n;
            }
            p = b->fnext;
            n->fnext = p;
            if (p != NULL) {
                p->fprev = n;
            }
            b->fprev = NULL;
            b->fnext = NULL;
        } else {
            ExtBlk *s = n->fprev;
            q = n->fnext;
            n->size = have;
            s->fnext = q;
            if (q != NULL) {
                n->fnext->fprev = n->fprev;
            }
            n->fprev = NULL;
            n->fnext = NULL;
        }
        b->owner = owner;
        o = owner;
        while (o->Extend.Child != NULL) {
            o = o->Extend.Child;
        }
        e = &r->ext;
        if (tmpl != NULL) {
            ((EXT *)tmpl)->EXTid = id;
            ((EXT *)tmpl)->Extend.Parent = o;
        } else {
            e->EXTid = id;
            e->Extend.Parent = o;
            e->Extend.Child = NULL;
        }
        o->Extend.Child = e;
        return e;
        }
        n = n->fnext;
    }
fail:
    ((NAS *)owner)->Failure |= 1;
    return NULL;
}

void FreeExtension(void *ext) {
    ExtBlk *m;
    ExtBlk *n;
    ExtBlk *f;
    ExtBlk *b;
    EXT *e;
    EXT *p;
    ExtBlk *nx;
    u32 size;

    e = ext;
    if ((u32)D_8005A7A0[6] < (u32)ext && (u32)ext < (u32)D_8005A7A0[7]) {
        b = (ExtBlk *)((u8 *)ext - 0x18);
        f = (ExtBlk *)b->owner;
        size = b->size;
        if (f != NULL) {
            ((EXT *)f)->EXTid = !(e->EXTid & ~0xFF) ? ((EXT *)f)->EXTid & 1 : 0;
        }
        p = e->Extend.Parent;
        if (p != NULL) {
            if ((p->Extend.Child = e->Extend.Child) != NULL) {
                e->Extend.Child->Extend.Parent = p;
            }
        }
        f = b;
        n = f->next;
        m = (ExtBlk *)D_8005A7A0;
        if (n != NULL && (n->size & 1)) {
            size += 0x18 + (n->size & ~3);
            if ((n->fprev->fnext = n->fnext) != NULL) {
                n->fnext->fprev = n->fprev;
            }
            m = n->fprev;
            n = n->next;
        }
        b = f->prev;
        if (b != NULL && (b->size & 1)) {
            f = b;
            size += 0x18 + (f->size & ~3);
            if ((f->fprev->fnext = f->fnext) != NULL) {
                f->fnext->fprev = f->fprev;
            }
            m = f->fprev;
        }
        f->size = size | 1;
        for (b = m->fnext; b != NULL && size >= b->size; b = m->fnext) {
            m = b;
        }
        f->next = n;
        if (n != NULL) {
            n->prev = f;
        }
        nx = m->fnext;
        f->fnext = nx;
        if (nx != NULL) {
            nx->fprev = f;
        }
        m->fnext = f;
        f->fprev = m;
    }
}

const char MsgFatalMetricsError[] = "FATAL METRICS ERROR !!!!\nUNABLE TO CONTINUE..!!\nLEV %d WORLD %d";
const char PauseTextContinue[] = "CONTINUE";
const char PauseTextShowRules[] = "SHOW RULES";
const char PauseTextOptions[] = "OPTIONS";
const char PauseTextChangeArena[] = "CHANGE ARENA";
const char PauseTextChangeArena2[] = "CHANGE ARENA?";
const char PauseTextQuitGame[] = "QUIT GAME";
const char PauseTextExitArena[] = "EXIT ARENA";
const char PauseTextExitArena2[] = "EXIT ARENA?";
const char PauseTextReturnToWarpRoom[] = "RETURN TO WARP ROOM";
const char PauseTextReturnToWarpRoom2[] = "RETURN TO WARP ROOM?";
const char PauseTextNewGame[] = "NEW GAME";
const char PauseTextReplayThisArena[] = "REPLAY THIS ARENA";
const char PauseTextCheatMenu[] = "CHEAT MENU";
const char PauseTextLevelSelector[] = "LEVEL SELECTOR";
const char PauseTextQuitGame2[] = "QUIT GAME?";
const char PauseTextYesNo[] = "YES       NO";
const char PauseTextYes[] = "YES         ";
const char PauseTextNo[] = "          NO";
const char PauseTextVibration[] = "VIBRATION";
const char PauseTextOn[] = "ON";
const char PauseTextOff[] = "OFF";
const char PauseTextCheatFlags[] = "CHEAT FLAGS";
const char PauseTextTweakSettings[] = "TWEAK SETTINGS";
const char PauseTextTweakAwards[] = "TWEAK AWARDS";
const char PauseTextViewStats[] = "VIEW STATS";
const char PauseTextAdjustLevel[] = "ADJUST LEVEL";
const char PauseTextController1IsUnplugged[] = "CONTROLLER 1 IS UNPLUGGED!";
const char PauseTextController2IsUnplugged[] = "CONTROLLER 2 IS UNPLUGGED!";
const char PauseTextController1AIsUnplugged[] = "CONTROLLER 1-A IS UNPLUGGED!";
const char PauseTextController1BIsUnplugged[] = "CONTROLLER 1-B IS UNPLUGGED!";
const char PauseTextController1CIsUnplugged[] = "CONTROLLER 1-C IS UNPLUGGED!";
const char PauseTextController1DIsUnplugged[] = "CONTROLLER 1-D IS UNPLUGGED!";
const char PauseTextPleaseInsertAController[] = "PLEASE INSERT A CONTROLLER";
const char PauseTextEmpty[] = "";
const char PauseTextDemo[] = "DEMO";
const char PauseText01[] = "01";
const char PauseText02[] = "02";
const char PauseText03[] = "03";
const char PauseTextSound[] = "SOUND";
const char PauseTextSoundOptions[] = "SOUND OPTIONS";
const char PauseTextAudioMode[] = "AUDIO MODE";
const char PauseTextMusicVolume[] = "MUSIC VOLUME";
const char PauseTextFxVolume[] = "FX VOLUME";
const char PauseTextController[] = "CONTROLLER";
const char PauseTextStereo[] = "<STEREO>";
const char PauseTextMono[] = "< MONO >";
const char PauseText100Percent[] = "<100%>";
const char PauseText090Percent[] = "<090%>";
const char PauseTextExit[] = "EXIT";
const char PauseTextPressButtonToAccept[] = "PRESS \374 BUTTON TO ACCEPT";
const char PauseTextPressButtonToChange[] = "PRESS \377 BUTTON TO CHANGE";
const char PauseTextP1[] = "P1";
const char PauseTextP2[] = "P2";
const char PauseTextP3[] = "P3";
const char PauseTextP4[] = "P4";
const char PauseTextPressButtonToForceVictory[] = "PRESS \374 BUTTON TO FORCE VICTORY";
const char PauseTextEmpty2[] = "";

static inline ArenaDef *func_800235BC_find(ArenaDef *p, s32 id) {
    while (p != NULL) {
        if (p->id == id) {
            return p;
        }
        if (p->id == 999) {
            break;
        }
        p++;
    }
    return NULL;
}

void SetupLevelSpecs(ArenaDef *arg0) {
    ArenaDef *e;

    LevelSpecs.LevelPreInit = 0;
    LevelSpecs.LevelInit = 0;
    LevelSpecs.LevelRestartInit = 0;
    LevelSpecs.LevelRoutine = 0;
    LevelSpecs.LevelExit = 0;
    LevelSpecs.EarliestTriggers = 0;
    LevelSpecs.PostTriggers = 0;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
    LevelSpecs.GetContour = 0;
    LevelSpecs.FinishQuick = 0;
    e = func_800235BC_find(arg0, GameMode);
    if (e == NULL) {
        e = func_800235BC_find(arg0, ModeTypes[GameMode * 3]);
    }
    if (e != NULL) {
        FirstNas = e->unk4 + 4;
        D_8005AA12 = NasPoolUsed = e->unk2 + 4;
        D_8005AA50 = e->unk6 << 10;
        D_8005AA54 = e->unk8;
        D_8005A5E4 = e->unk10;
        ((D5A614View *)GameInfo)->unk0 = e->unk14;
        D_8005A5E0 = e->unkC;
        if (((D5A614View *)GameInfo)->flags & 0x80) {
            ((D5A614View *)GameInfo)->unk8 = ((LevelInfo *)ModeTypes)[((D5A614View *)GameInfo)->mode].unk4;
        } else {
            ((D5A614View *)GameInfo)->unk8 = ((LevelInfo *)ModeTypes)[((D5A614View *)GameInfo)->mode].unk2;
        }
    }
    if (e == NULL || D_8005A5E0 == NULL) {
        printf(MsgFatalMetricsError, GameMode, ((LevelInfo *)ModeTypes)[GameMode].world);
    }
}

void SetNasDefaults(void) {
    NASTemplate.NASid = 0;
    NASTemplate.Camera.width = 0x200;
    NASTemplate.Camera.look_ahead = 0x200;
    NASTemplate.Camera.height = 0x200;
    NASTemplate.Camera.flags = 0x8000;
    NASTemplate.InitDelete = (void (*)(NAS *)) NullProc;
    NASTemplate.Handler = (void (*)(NAS *)) NullProc;
    NASTemplate.Class = 2;
    NASTemplate.Extend.Parent = NULL;
    NASTemplate.Extend.Child = NULL;
    NASTemplate.Tag.Parent = NULL;
    NASTemplate.Tag.Child = NULL;
    NASTemplate.Family.Parent = NULL;
    NASTemplate.Family.Child = NULL;
    NASTemplate.Group.Parent = NULL;
    NASTemplate.Group.Child = NULL;
    NASTemplate.Group.Head = NULL;
    NASTemplate.Object.Parent = NULL;
    NASTemplate.Object.Child = NULL;
    NASTemplate.Solid.Parent = NULL;
    NASTemplate.Solid.Child = NULL;
    NASTemplate.Camera.Parent = NULL;
    NASTemplate.Camera.Child = NULL;
    NASTemplate.FUX = NULL;
    NASTemplate.trigger = NULL;
    NASTemplate.Engine = NULL;
    NASTemplate.Failure = 0;
    NASTemplate.Type = 0;
    NASTemplate.UserInfo.status = 0;
    NASTemplate.UserInfo.flags = 0;
    NASTemplate.UserInfo.user2 = 0;
    NASTemplate.UserInfo.user1 = 0;
    SubEngineTemplate.flags = 0x01008000;
    SubEngineTemplate.unk0C = 0;
    SubEngineTemplate.unk08 = 0;
    SubEngineTemplate.unk04 = 0;
    SubEngineTemplate.unk52 = 1;
    SubEngineTemplate.unk14 = 0;
    SubEngineTemplate.unk12 = 0;
    SubEngineTemplate.unk10 = 0;
    SubEngineTemplate.unk16 = 0;
    SubEngineTemplate.unk20.z = 0x1000;
    SubEngineTemplate.unk20.y = 0x1000;
    SubEngineTemplate.unk20.x = 0x1000;
    SubEngineTemplate.unk51 = 4;
    SubEngineTemplate.hook = (void (*)(subengine *, s32, u32)) draw_object;
    SubEngineTemplate.release = (void (*)(subengine *)) sub_release_none;
    SubEngineTemplate.unk50 = 0;
    SubEngineTemplate.unk74 = 0;
    SubEngineTemplate.unk70 = 0;
    SubEngineTemplate.unk6C = NULL;
    SubEngineTemplate.unk76 = 0;
    SubEngineTemplate.unk90 = 0;
    SubEngineTemplate.unk9C = 0;
    SubEngineTemplate.unk94 = 0;
    SubEngineTemplate.unkA0 = 0;
    SubEngineTemplate.unk80 = 0;
    SubEngineTemplate.unk7C = 0;
    SubEngineTemplate.unk78 = 0;
    SubEngineTemplate.unk8A = 0;
    EngineTemplate.EXTid = 0;
    EngineTemplate.flags = 0x8000;
    EngineTemplate.Extend.Child = NULL;
    EngineTemplate.Extend.Parent = NULL;
    EngineTemplate.unk10.z = 0;
    EngineTemplate.unk10.y = 0;
    EngineTemplate.unk10.x = 0;
    EngineTemplate.unk20 = 0;
    EngineTemplate.render = eng_render_subs;
    EngineTemplate.unk1E = 0;
    EngineTemplate.unk1C = 0;
    EngineTemplate.unk5E = 1;
    EngineTemplate.unk5D = 0;
    EngineTemplate.unk5C = 1;
    EngineTemplate.unk2C.z = 0x1000;
    EngineTemplate.unk2C.y = 0x1000;
    EngineTemplate.unk2C.x = 0x1000;
    EngineTemplate.num_subs = 0;
    EngineTemplate.kill = eng_release_subs_sfx;
    EngineTemplate.sub = NULL;
    EngineTemplate.last = NULL;
    EngineTemplate.OldXYZ.z = 0;
    EngineTemplate.OldXYZ.y = 0;
    EngineTemplate.OldXYZ.x = 0;
    EngineTemplate.Engine = NULL;
    EngineTemplate.platform = NULL;
    *(s16 *)&EngineTemplate.collision_box.unk4 = 0;
    EngineTemplate.collision_box.unk2 = 0;
    *(s16 *)&EngineTemplate.collision_box.unk0 = 0;
    EngineTemplate.collision_box.radius = 0;
    *(s32 *)&EngineTemplate.collision_box.height = 0;
    EngineTemplate.collision_box.flags = 0;
    EngineTemplate.boomshanka = 0;
    EngineTemplate.direction = 0;
    EngineTemplate.sfx = NULL;
    ((EXTobject *)&ExtTemplate1000)->EXTid = 0;
    ((EXTobject *)&ExtTemplate1000)->Extend.Child = NULL;
    ((EXTobject *)&ExtTemplate1000)->Extend.Parent = NULL;
    ((EXTobject *)&ExtTemplate1000)->whatami = 0;
    ((EXTobject *)&ExtTemplate1000)->player = -1;
    ((EXTobject *)&ExtTemplate1000)->owner = -1;
    ((EXTobject *)&ExtTemplate1000)->status = 0;
    ((EXTobject *)&ExtTemplate1000)->can_do = 0;
    ((EXTobject *)&ExtTemplate1000)->will_do = 0;
    ((EXTobject *)&ExtTemplate1000)->timer = 0;
    ((EXTobject *)&ExtTemplate1000)->countdown = 0;
    ((EXTobject *)&ExtTemplate1000)->target = NULL;
    ((EXTobject *)&ExtTemplate1000)->who = NULL;
    ((EXTobject *)&ExtTemplate1000)->contains = 0;
    ((EXTanimator *)&ExtTemplate400)->EXTid = 0;
    ((EXTanimator *)&ExtTemplate400)->Extend.Child = NULL;
    ((EXTanimator *)&ExtTemplate400)->Extend.Parent = NULL;
    ((EXTanimator *)&ExtTemplate400)->Engine = NULL;
    ((EXTanimator *)&ExtTemplate400)->sub = NULL;
    ((EXTanimator *)&ExtTemplate400)->Atable = NULL;
    ((EXTanimator *)&ExtTemplate400)->Alist = NULL;
    ((EXTanimator *)&ExtTemplate400)->Aspeed = -1;
    ((EXTanimator *)&ExtTemplate400)->Aindex = -1;
    ((EXTanimator *)&ExtTemplate400)->Acount = 0;
    ((EXTanimator *)&ExtTemplate400)->ALindex = -1;
    ((EXTanimator *)&ExtTemplate400)->Masterspeed = 0;
    ((EXTanimator *)&ExtTemplate400)->Currentframe = 0;
    ((EXTanimator *)&ExtTemplate400)->Seq_count = 0;
    ((EXTanimator *)&ExtTemplate400)->Seq_count_down = 0;
    ExtMotionTemplate.EXTid = 0;
    ExtMotionTemplate.Extend.Child = NULL;
    ExtMotionTemplate.Extend.Parent = NULL;
    ExtMotionTemplate.flags = 0;
    ExtMotionTemplate.Ydir = 0;
    ExtMotionTemplate.Xmax = 0;
    ExtMotionTemplate.Xdec = 0;
    ExtMotionTemplate.Xacc = 0;
    ExtMotionTemplate.Xvel = 0;
    ExtMotionTemplate.Ymax = 0;
    ExtMotionTemplate.Ydec = 0;
    ExtMotionTemplate.Yacc = 0;
    ExtMotionTemplate.Yvel = 0;
    ExtMotionTemplate.Zmax = 0;
    ExtMotionTemplate.Zdec = 0;
    ExtMotionTemplate.Zacc = 0;
    ExtMotionTemplate.Zvel = 0;
    ExtMotionTemplate.Za = 0;
    ExtMotionTemplate.Ya = 0;
    ExtMotionTemplate.Xa = 0;
    ExtMotionTemplate.Mass = 0;
    ExtMotionTemplate.floorheight = 0;
    ExtMotionTemplate.Ftime = 0;
    ExtMotionTemplate.Fdist = 0;
}

void InitSysFont(void) {
    s32 i;

    for (i = 0; i < 256; i++) {
        s16 *pg = &SysFontLookUp[i];
        s8 *pw;
        u8 *pf;

        pg[0] = -1;
        pw = &SysFontWidth[i];
        pw[0] = 16;
        pf = &SysFontFlags[i];
        pf[0] = 0;
        pg[256] = -1;
        pw[256] = 11;
        pf[256] = 0;
    }
    SysFontLookUp['@'] = 0x302F;
    SysFontLookUp['A'] = 0x303A;
    SysFontLookUp['B'] = 0x303B;
    SysFontLookUp['C'] = 0x303C;
    SysFontLookUp['D'] = 0x303D;
    SysFontLookUp['E'] = 0x303E;
    SysFontLookUp['F'] = 0x303F;
    SysFontLookUp['G'] = 0x3040;
    SysFontLookUp['H'] = 0x3041;
    SysFontLookUp['I'] = 0x3042;
    SysFontLookUp['J'] = 0x3043;
    SysFontLookUp['K'] = 0x3044;
    SysFontLookUp['L'] = 0x3045;
    SysFontLookUp['M'] = 0x3046;
    SysFontLookUp['N'] = 0x3047;
    SysFontLookUp['O'] = 0x3048;
    SysFontLookUp['P'] = 0x3049;
    SysFontLookUp['Q'] = 0x304A;
    SysFontLookUp['R'] = 0x304B;
    SysFontLookUp['S'] = 0x304C;
    SysFontLookUp['T'] = 0x304D;
    SysFontLookUp['U'] = 0x304E;
    SysFontLookUp['V'] = 0x304F;
    SysFontLookUp['W'] = 0x3050;
    SysFontLookUp['X'] = 0x3051;
    SysFontLookUp['Y'] = 0x3052;
    SysFontLookUp['Z'] = 0x3053;
    SysFontLookUp['a'] = 0x303A;
    SysFontLookUp['b'] = 0x303B;
    SysFontLookUp['c'] = 0x303C;
    SysFontLookUp['d'] = 0x303D;
    SysFontLookUp['e'] = 0x303E;
    SysFontLookUp['f'] = 0x303F;
    SysFontLookUp['g'] = 0x3040;
    SysFontLookUp['h'] = 0x3041;
    SysFontLookUp['i'] = 0x3042;
    SysFontLookUp['j'] = 0x3043;
    SysFontLookUp['k'] = 0x3044;
    SysFontLookUp['l'] = 0x3045;
    SysFontLookUp['m'] = 0x3046;
    SysFontLookUp['n'] = 0x3047;
    SysFontLookUp['o'] = 0x3048;
    SysFontLookUp['p'] = 0x3049;
    SysFontLookUp['q'] = 0x304A;
    SysFontLookUp['r'] = 0x304B;
    SysFontLookUp['s'] = 0x304C;
    SysFontLookUp['t'] = 0x304D;
    SysFontLookUp['u'] = 0x304E;
    SysFontLookUp['v'] = 0x304F;
    SysFontLookUp['w'] = 0x3050;
    SysFontLookUp['x'] = 0x3051;
    SysFontLookUp['y'] = 0x3052;
    SysFontLookUp['z'] = 0x3053;
    SysFontLookUp['0'] = 0x3030;
    SysFontLookUp['1'] = 0x3031;
    SysFontLookUp['2'] = 0x3032;
    SysFontLookUp['3'] = 0x3033;
    SysFontLookUp['4'] = 0x3034;
    SysFontLookUp['5'] = 0x3035;
    SysFontLookUp['6'] = 0x3036;
    SysFontLookUp['7'] = 0x3037;
    SysFontLookUp['8'] = 0x3038;
    SysFontLookUp['9'] = 0x3039;
    SysFontLookUp['.'] = 0x3054;
    SysFontLookUp[','] = 0x3055;
    SysFontLookUp['\''] = 0x3056;
    SysFontLookUp[':'] = 0x3057;
    SysFontLookUp['!'] = 0x3058;
    SysFontLookUp['='] = 0x3059;
    SysFontLookUp['+'] = 0x305A;
    SysFontLookUp['-'] = 0x305B;
    SysFontLookUp['?'] = 0x305C;
    SysFontLookUp['%'] = 0x305D;
    SysFontLookUp['<'] = 0x305E;
    SysFontLookUp['>'] = 0x305F;
    SysFontWidth['.'] = 9;
    SysFontWidth[','] = 9;
    SysFontWidth['\''] = 13;
    SysFontWidth[0] = 20;
    SysFontWidth[' '] = 12;
    SysFontWidth[9] = 48;
    SysFontLookUp[0xFC] = 0x3066;
    SysFontLookUp[0xFD] = 0x3062;
    SysFontLookUp[0xFE] = 0x3065;
    SysFontLookUp[0xFF] = 0x3064;
    SysFontFlags[0xFC] = 1;
    SysFontFlags[0xFD] = 1;
    SysFontFlags[0xFE] = 1;
    SysFontFlags[0xFF] = 1;
    SysFontWidth[0xFC] = 32;
    SysFontWidth[0xFD] = 32;
    SysFontWidth[0xFE] = 32;
    SysFontWidth[0xFF] = 32;
    SysFontLookUp[0x100 + '@'] = 0x3000;
    SysFontLookUp[0x100 + 'A'] = 0x3002;
    SysFontLookUp[0x100 + 'B'] = 0x3003;
    SysFontLookUp[0x100 + 'C'] = 0x3004;
    SysFontLookUp[0x100 + 'D'] = 0x3005;
    SysFontLookUp[0x100 + 'E'] = 0x3006;
    SysFontLookUp[0x100 + 'F'] = 0x3007;
    SysFontLookUp[0x100 + 'G'] = 0x3008;
    SysFontLookUp[0x100 + 'H'] = 0x3009;
    SysFontLookUp[0x100 + 'I'] = 0x300A;
    SysFontLookUp[0x100 + 'J'] = 0x300B;
    SysFontLookUp[0x100 + 'K'] = 0x300C;
    SysFontLookUp[0x100 + 'L'] = 0x300D;
    SysFontLookUp[0x100 + 'M'] = 0x300E;
    SysFontLookUp[0x100 + 'N'] = 0x300F;
    SysFontLookUp[0x100 + 'O'] = 0x3010;
    SysFontLookUp[0x100 + 'P'] = 0x3011;
    SysFontLookUp[0x100 + 'Q'] = 0x3012;
    SysFontLookUp[0x100 + 'R'] = 0x3013;
    SysFontLookUp[0x100 + 'S'] = 0x3014;
    SysFontLookUp[0x100 + 'T'] = 0x3015;
    SysFontLookUp[0x100 + 'U'] = 0x3016;
    SysFontLookUp[0x100 + 'V'] = 0x3017;
    SysFontLookUp[0x100 + 'W'] = 0x3018;
    SysFontLookUp[0x100 + 'X'] = 0x3019;
    SysFontLookUp[0x100 + 'Y'] = 0x301A;
    SysFontLookUp[0x100 + 'Z'] = 0x301B;
    SysFontLookUp[0x100 + 'a'] = 0x3002;
    SysFontLookUp[0x100 + 'b'] = 0x3003;
    SysFontLookUp[0x100 + 'c'] = 0x3004;
    SysFontLookUp[0x100 + 'd'] = 0x3005;
    SysFontLookUp[0x100 + 'e'] = 0x3006;
    SysFontLookUp[0x100 + 'f'] = 0x3007;
    SysFontLookUp[0x100 + 'g'] = 0x3008;
    SysFontLookUp[0x100 + 'h'] = 0x3009;
    SysFontLookUp[0x100 + 'i'] = 0x300A;
    SysFontLookUp[0x100 + 'j'] = 0x300B;
    SysFontLookUp[0x100 + 'k'] = 0x300C;
    SysFontLookUp[0x100 + 'l'] = 0x300D;
    SysFontLookUp[0x100 + 'm'] = 0x300E;
    SysFontLookUp[0x100 + 'n'] = 0x300F;
    SysFontLookUp[0x100 + 'o'] = 0x3010;
    SysFontLookUp[0x100 + 'p'] = 0x3011;
    SysFontLookUp[0x100 + 'q'] = 0x3012;
    SysFontLookUp[0x100 + 'r'] = 0x3013;
    SysFontLookUp[0x100 + 's'] = 0x3014;
    SysFontLookUp[0x100 + 't'] = 0x3015;
    SysFontLookUp[0x100 + 'u'] = 0x3016;
    SysFontLookUp[0x100 + 'v'] = 0x3017;
    SysFontLookUp[0x100 + 'w'] = 0x3018;
    SysFontLookUp[0x100 + 'x'] = 0x3019;
    SysFontLookUp[0x100 + 'y'] = 0x301A;
    SysFontLookUp[0x100 + 'z'] = 0x301B;
    SysFontLookUp[0x100 + '0'] = 0x301C;
    SysFontLookUp[0x100 + '1'] = 0x301D;
    SysFontLookUp[0x100 + '2'] = 0x301E;
    SysFontLookUp[0x100 + '3'] = 0x301F;
    SysFontLookUp[0x100 + '4'] = 0x3020;
    SysFontLookUp[0x100 + '5'] = 0x3021;
    SysFontLookUp[0x100 + '6'] = 0x3022;
    SysFontLookUp[0x100 + '7'] = 0x3023;
    SysFontLookUp[0x100 + '8'] = 0x3024;
    SysFontLookUp[0x100 + '9'] = 0x3025;
    SysFontLookUp[0x100 + '.'] = 0x3026;
    SysFontLookUp[0x100 + ','] = 0x3027;
    SysFontLookUp[0x100 + '!'] = 0x3028;
    SysFontLookUp[0x100 + '`'] = 0x3029;
    SysFontLookUp[0x100 + '\''] = 0x3029;
    SysFontLookUp[0x100 + '<'] = 0x302A;
    SysFontLookUp[0x100 + '>'] = 0x302B;
    SysFontLookUp[0x100 + '%'] = 0x302C;
    SysFontLookUp[0x100 + ':'] = 0x302D;
    SysFontLookUp[0x100 + '-'] = 0x302E;
    SysFontWidth[0x100 + 0] = 10;
    SysFontWidth[0x100 + ' '] = 6;
    SysFontWidth[0x100 + 9] = 24;
    SysFontWidth[0x100 + '.'] = 4;
    SysFontWidth[0x100 + '!'] = 6;
    SysFontWidth[0x100 + ','] = 6;
    SysFontWidth[0x100 + '`'] = 6;
    SysFontWidth[0x100 + '\''] = 6;
    SysFontLookUp[0x1FC] = 0x3066;
    SysFontLookUp[0x1FD] = 0x3062;
    SysFontLookUp[0x1FE] = 0x3065;
    SysFontLookUp[0x1FF] = 0x3064;
    SysFontFlags[0x1FC] = 1;
    SysFontFlags[0x1FD] = 1;
    SysFontFlags[0x1FE] = 1;
    SysFontFlags[0x1FF] = 1;
    SysFontFlags[0x1FC] = 5;
    SysFontFlags[0x1FD] = 5;
    SysFontFlags[0x1FE] = 5;
    SysFontFlags[0x1FF] = 5;
    SysFontWidth[0x1FC] = 32;
    SysFontWidth[0x1FD] = 32;
    SysFontWidth[0x1FE] = 32;
    SysFontWidth[0x1FF] = 32;
}

s16 TextAlignX(s16 x, u8 *s) {
    s32 y = x;
    s32 sum;
    s32 c;
    s32 d;
    s32 e;

    if ((y & 0xC000) == 0x8000) {
        if (y & 0x3FFF) {
            s32 n = 0;

            if (*s != 0 && *s != 10) {
                do {
                    s32 idx;
                    u8 *f;
                    s8 *w;
                    s32 v;
                    if (*s == 13) {
                        break;
                    }
                    idx = *s + (FontPage << 8);
                    w = &SysFontWidth[idx];
                    f = &SysFontFlags[idx];
                    v = *w;
                    if (!(*f & 2)) {
                        n += v;
                    }
                    s++;
                } while (*s != 0 && *s != 10);
            }
            x = (x & 0x3FFF) - (n * 5 * 128) / ScreenRect->w / 2;
        } else {
            c = *s;
            d = c & 0xFF;
            sum = 0;
            if (d != 0 && d != 10) {
                do {
                    s32 idx;
                    u8 *f;
                    s8 *w;
                    s32 v;
                    if (c == 13) {
                        break;
                    }
                    idx = *s + (FontPage << 8);
                    w = &SysFontWidth[idx];
                    f = &SysFontFlags[idx];
                    v = *w;
                    if (!(*f & 2)) {
                        sum += v;
                    }
                    s++;
                    c = *s;
                    e = c & 0xFF;
                } while (e != 0 && e != 10);
            }
            x = 320 - (sum * 5 * 128) / ScreenRect->w / 2;
        }
    }
    return x;
}

void TextFlashColour(u8 *a, u8 *b) {
    s32 t;
    s32 bl, g, r;

    r = a[0];
    t = (rcossin_tbl[(((FrameCount << 7) & 0x7FF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
    g = a[1];
    bl = a[2];
    r += ((0xFF - r) * t) / 20;
    a[0] = r;
    g += ((0xFF - g) * t) / 20;
    a[1] = g;
    bl += ((0xFF - bl) * t) / 20;
    a[2] = bl;
    r = b[0];
    g = b[1];
    bl = b[2];
    r += ((0xFF - r) * t) / 20;
    b[0] = r;
    g += ((0xFF - g) * t) / 20;
    b[1] = g;
    bl += (-bl * t) / 20;
    b[2] = bl;
}

void DrawSysText(s16 arg0, s32 arg1, u8 *s, u32 arg3, u32 arg4, s32 arg5) {
    s32 xy;
    s32 col;
    s16 x0;
    s32 c;
    s32 n;
    s32 i;
    s32 g;
    s16 w;
    u8 fl;
    s32 y;
    s32 ny;
    s32 oy;

    xy &= 0xFFFF;
    xy |= arg1 << 16;
    x0 = TextAlignX(arg0, s);
    col = 0;
    if (arg5 != 0) {
        TextFlashColour((u8 *)&arg3, (u8 *)&arg4);
    }
    c = *s++;
    while (c != 0) {
        if (c == '\n' || c == '\r') {
            col = 0;
            if (c == '\r') {
                c = *s++;
                n = c - '0';
                n *= 10;
                c = *s++;
                oy = n - '0';
                n = oy + c;
                if (n > 50) {
                    n -= 50;
                    n = ~n + 1;
                }
                y = xy >> 16;
                xy &= 0xFFFF;
                xy |= (y + n * 2) << 16;
            } else {
                ny = xy >> 16;
                xy &= 0xFFFF;
                xy |= (ny + SysFontWidth[FontPage << 8] * 2) << 16;
            }
            x0 = TextAlignX(arg0, s);
        } else {
            i = c + (FontPage << 8);
            w = SysFontWidth[i];
            fl = SysFontFlags[i];
            if (fl & 2) {
                col -= w;
            }
            g = ((s16 (*)[256])&SysFontLookUp[c])[FontPage][0];
            if (g != -1) {
                xy &= 0xFFFF0000;
                xy |= (u16)(x0 + col * 640 / ScreenRect->w);
                if (fl & 1) {
                    if (fl & 4) {
                        oy = xy >> 16;
                        xy &= 0xFFFF;
                        xy |= (oy - 7) << 16;
                        texture_draw((obj_texture *)(*(u8 **)(sysfont_dbase + 0x18) + (g & 0xFFF) * 0x38), xy, 0, 0x808080);
                        oy = xy >> 16;
                        xy &= 0xFFFF;
                        xy |= (oy + 7) << 16;
                    } else {
                        texture_draw((obj_texture *)(*(u8 **)(sysfont_dbase + 0x18) + (g & 0xFFF) * 0x38), xy, 0, 0x808080);
                    }
                } else {
                    texture_draw_quad((obj_texture *)(*(u8 **)(sysfont_dbase + 0x18) + (g & 0xFFF) * 0x38), xy, 0, arg3, arg3, arg4, arg4);
                }
            }
            col += w;
        }
        c = *s++;
    }
}

void SysPrintAt3d(SWORD_XYZ *arg0, char *s, u32 arg2, u32 arg3) {
    SWORD_XYZ pos;
    SWORD_XYZ tmp;
    s32 c;
    s32 n;
    u8 *p;

    pos = *arg0;
    if ((arg0->x & 0xC000) == 0x8000) {
        p = s;
        n = 0;
        if (*p != 0 && *p != 10) {
            do {
                s32 idx;
                u8 *f;
                s8 *w;
                s32 v;
                if (*p == 13) {
                    break;
                }
                idx = *p + (FontPage << 8);
                w = &SysFontWidth[idx];
                f = &SysFontFlags[idx];
                v = *w;
                if (!(*f & 2)) {
                    n += v;
                }
                p++;
            } while (*p != 0 && *p != 10);
        }
        pos.x = -((n * 16) >> 1);
    }
    c = *s++;
    if (c != 0) {
        u32 tab = (u32)SysFontLookUp;
        do {
            s32 g = *(s16 *)(c * 2 + (FontPage << 9) + tab);
            if (g != -1) {
                texture_draw_3d(*(u8 **)(sysfont_dbase + 0x18) + (g & 0xFFF) * 0x38, &pos, 0, arg2, arg3);
            }
            pos.x += (s32)((u8)SysFontWidth[c + (FontPage << 8)] << 24) >> 20;
            c = *s++;
        } while (c != 0);
    }
}

void DrawSpriteTriple(x, y, c0, c1, n)
    s16 x;
    s16 y;
    s32 c0;
    s32 c1;
    s32 n;
{
    u8 *base;
    u16 *ids;
    u16 (*tbl)[3];
    SprPos pos;

    if ((x & 0xC000) == 0x8000) {
        x = 0x140 - (0x1D880 / ScreenRect->w) / 2;
    }
    pos.f.y = y;
    pos.f.x = x;
    tbl = D_8005A5C0;
    ids = tbl[n];
    base = &HeroModelBank[DB_Interface];
    texture_draw_quad((obj_texture *)(*(u8 **)(base + 0x18) + (ids[0] & 0xFFF) * 0x38), pos.w, 0, c0, c0, c1, c1);
    pos.f.x = x + 0x9D80 / ScreenRect->w;
    texture_draw_quad((obj_texture *)(*(u8 **)(base + 0x18) + (ids[1] & 0xFFF) * 0x38), pos.w, 0, c0, c0, c1, c1);
    pos.f.x = x + 0x13B00 / ScreenRect->w;
    texture_draw_quad((obj_texture *)(*(u8 **)(base + 0x18) + (ids[2] & 0xFFF) * 0x38), pos.w, 0, c0, c0, c1, c1);
}

void InitTriggers(void) {
    EXTengine tmp;
    s32 pad[2];
    U16Table *tbl;
    u16 *types;
    s32 ntypes;
    s32 i;
    NASTrigger *t;
    NASTrigger *trig;
    MapTrigRec *src;
    MapTrigInfo *info;
    s32 idx;

    tmp = EngineTemplate;
    tbl = *(U16Table **)((u8 *)D_8005A5E0 + 8);
    types = tbl->data;
    ntypes = tbl->count;
    if ((TriggerCount = (*(s32 *)MapScene != 0) ? **(s32 **)((u8 *)MapScene + 0x30) : 0) == 0) {
        return;
    }
    if (HeadOfTriggers == NULL) {
        HeadOfTriggers = (NASTrigger *)mal_alloc(TriggerCount * sizeof(NASTrigger), &MainHeap);
    }
    t = HeadOfTriggers;
    if (t != NULL) {
    for (i = 0; i < 32; i++) {
        MapPoints[i].pos.x = MapPoints[i].pos.y = MapPoints[i].pos.z = 0;
        MapPoints[i].rot.x = MapPoints[i].rot.y = MapPoints[i].rot.z = 0;
    }
    trig = HeadOfTriggers;
    for (i = 0; i < TriggerCount; i++, trig++) {
        src = (MapTrigRec *)(*(u8 **)((u8 *)MapScene + 0x30) + 4) + i;
        info = (MapTrigInfo *)((u8 *)src + ((src->info & ~3) + 0xC));
        trig->InfoStruct = (MapTriggerInfo *)info;
        if (info->flags & 0x4000) {
            MapPoints[info->slot].pos = src->pos;
            MapPoints[info->slot].rot.x = info->rot.x;
            MapPoints[info->slot].rot.y = info->rot.y;
            MapPoints[info->slot].rot.z = info->rot.z;
            trig->TrigXYZ = *(SLONG_XYZ *)&src->pos;
            trig->Status = 0x8000;
            trig->cast = *(u16 *)&src->info & 3;
        } else {
            *(s32 *)&trig->TrigPath = 0x1FF;
            trig->cast = *(u16 *)&src->info & 3;
            if (!(src->info & 3)) {
                trig->TrigXYZ = *(SLONG_XYZ *)&src->pos;
            }
            idx = info->type;
            if (idx >= 0 && idx < ntypes) {
                trig->Type = types[idx];
            } else {
                trig->Type = 0;
            }
            trig->Status = (info->flags & 0xF7FF) << 4;
            trig->StartUp = *(u16 *)&info->slot;
        }
    }
    if (LevelSpecs.PostTriggers != NULL) {
        LevelSpecs.PostTriggers(HeadOfTriggers, TriggerCount);
    }
    } else {
        TriggerCount = 0;
    }
}

ScriptCursor *ScriptCursorFirst(NAS *nas, ScriptInit *init) {
    ScriptCursor *c;
    EXT *e;
    Ext400 *ext;
    u32 flags;
    s32 sub;
    s32 id;

    flags = init->flags;
    if (flags & 0x400) {
    id = 0x400 & ~0xFF;
    sub = 0x400 & 0xFF;
    for (e = nas->Extend.Child; e != NULL; e = e->Extend.Child) {
        if (e->EXTid & id) {
            if (sub == 0) {
                goto found;
            }
            if ((e->EXTid & 0xFF) == sub) {
                goto found;
            }
        }
    }
    c = NULL;
done:
    if (c == NULL) {
        ext = AllocExtension(nas, 0x2C, 0x400, &ExtTemplate400);
        if (ext != NULL) {
            nas->NASid |= 0x400;
            *ext = ExtTemplate400;
        }
        c = (ScriptCursor *)ext;
        if (c == NULL) {
            return NULL;
        }
        goto skip;
    found:
        c = (ScriptCursor *)e;
        goto done;
    }
skip:
    flags &= 0xFF;
    *(s32 *)c = (*(s32 *)c & ~0xFF) | flags;
    c->cur = init->cur;
    c->tbl = init->tbl;
    ScriptCursorNext(c);
    return c;
    }
    return NULL;
}

void ScriptCursorNext(ScriptCursor *c) {
    s32 e;

    switch (c->kind) {
    case 1:
        if (c->tbl != NULL) {
            c->idx++;
            do {
                e = c->tbl[c->idx];
                if (c->tbl[c->idx] == 0) {
                    c->idx++;
                    c->idx = *(u16 *)&c->tbl[c->idx];
                }
            } while (e == 0);
            c->cur = e;
        }
        c->unk1E = 0;
        c->unk1C = -1;
        c->unk28 = 0;
        break;
    case 2:
        if (c->tbl != NULL) {
            c->idx++;
            do {
                e = c->tbl[c->idx];
                if (c->tbl[c->idx] == 0) {
                    c->idx++;
                    c->idx = *(u16 *)&c->tbl[c->idx];
                }
            } while (e == 0);
            c->cur = e;
        }
        c->unk1E = -1;
        c->unk1C = -1;
        c->unk28 = 0;
        break;
    default:
        c->unk28 = 0;
        break;
    }
    c->unk2A = 0;
}

s16 RunNasAnim(NAS *nas) {
    EXTengine *eng;
    EXTanimator *anim;
    subengine *sub;
    s32 id;
    s32 kind;
    s32 want;

    if ((nas->NASid & 0x500) == 0x500 && (eng = nas->Engine) != NULL) {
        id = 0x400;
        want = 0;
        for (anim = (EXTanimator *)nas->Extend.Child; anim != NULL; anim = (EXTanimator *)anim->Extend.Child) {
            if (anim->EXTid & id) {
                if (want == 0) {
                    goto found;
                }
                if ((anim->EXTid & 0xFF) == want) {
                    goto found;
                }
            }
        }
        anim = NULL;
    found:
        if (anim != NULL) {
            kind = anim->EXTid & 0xFF;
            if (anim->Engine != NULL) {
                eng = anim->Engine;
            }
            sub = anim->sub;
            if (sub == NULL) {
                sub = eng->sub;
            }
            switch (kind) {
            case 1:
                return DoAnimCommand(nas, eng, sub, anim);
            case 2:
                return DoAnimCommand2(nas, eng, sub, anim);
            }
        }
    }
    return 1;
}

s16 DoAnimCommand(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim) {
    /* A label table, not a switch: cc1 2.8.1 aligns a switch's jump table
     * to 8 in .rodata, but this one sits at a 4 mod 8 offset (and is
     * followed by a zero word). */
    static const void *const tbl[20] = {
        &&l8000, &&l8001, &&l8002, &&l8003, &&l8004, &&l8005, &&l8006, &&ldef,
        &&ldef, &&ldef, &&ldef, &&l800B, &&l800C, &&l800D, &&l800E, &&l800F,
        &&l8010, &&l8011, &&l8012, 0
    };
    void (*fn)(NAS *, EXTanimator *, s16);
    u32 idx;
    s16 frame;
    s16 ret;
    s32 held;
    s32 vol;
    s32 i;

    ret = 0;
    held = 0;
    if (--anim->Aspeed < 0) {
        vol = 0x1E00;
        do {
            if (anim->Seq_count_down != 0 && anim->Currentframe != 0) {
                frame = anim->Currentframe - 1;
                anim->Seq_count_down--;
            } else if (anim->Seq_count != 0 && anim->Currentframe != 0) {
                frame = anim->Currentframe + 1;
                anim->Seq_count--;
            } else {
                frame = ((s16 *)anim->Atable)[++anim->Aindex];
                if (frame < 0) {
                    idx = (u16)frame - 0x8000;
                    if (idx < 0x13) {
                        goto *tbl[idx];
                    }
                    goto ldef;
                l8000:
                    anim->Aindex = 0;
                    if (ret == 0) {
                        ret = 2;
                    }
                    goto lnext;
                l8001:
                    ScriptCursorNext((ScriptCursor *)anim);
                    if (ret == 0) {
                        ret = 3;
                    }
                    goto lnext;
                l8002:
                    eng->direction = ~eng->direction;
                    goto lnext;
                l8004:
                    if (nas->trigger != NULL) {
                        nas->trigger->Status |= 0x8000;
                    }
                    KillNAS(nas);
                    frame = 0;
                    goto lnext;
                l8003:
                    KillNAS(nas);
                    frame = 0;
                    goto lnext;
                l8005:
                    ret = ((s16 *)anim->Atable)[++anim->Aindex];
                    goto lnext;
                l8006:
                    ret = ((s16 *)anim->Atable)[++anim->Aindex];
                    frame = sub->unk74;
                    goto lnext;
                l8011:
                    anim->Seq_count = ((s16 *)anim->Atable)[++anim->Aindex] - 1;
                    anim->Currentframe = 0;
                    goto lnext;
                l8012:
                    anim->Seq_count_down = ((s16 *)anim->Atable)[++anim->Aindex] - 1;
                    anim->Currentframe = 0;
                    goto lnext;
                l800D:
                    PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], &eng->sfx, NULL, 0x1000, vol, 0);
                    goto lnext;
                l800C:
                    PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], NULL, &eng->unk10, 0x1000, vol, 0);
                    goto lnext;
                l800B:
                    PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], NULL, NULL, 0x1000, vol, 0);
                    goto lnext;
                l800E:
                    PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], NULL, NULL, 0x1000, vol, 0);
                    goto lnext;
                l800F:
                    held = 1;
                    anim->Aspeed = ((s16 *)anim->Atable)[++anim->Aindex];
                    goto lnext;
                l8010:
                    sub->unk6C = &HeroModelBank[((s16 *)anim->Atable)[++anim->Aindex]];
                    goto lnext;
                ldef:
                    i = frame >> 8;
                    fn = (void (*)(NAS *, EXTanimator *, s16))ACMD[i & 0xF][0];
                    if (fn != NULL) {
                        fn(nas, anim, frame);
                    }
                lnext:;
                }
            }
        } while (frame < 0);
        anim->Currentframe = sub->unk74 = frame;
        if (held == 0) {
            anim->Aspeed = *(s16 *)anim->Atable;
        }
    }
    return ret;
}

s16 DoAnimCommand2(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim) {
    /* A label table, not a switch: cc1 2.8.1 aligns a switch's jump table
     * to 8 in .rodata, but this one sits at a 4 mod 8 offset. */
    static const void *const tbl[17] = {
        &&l8000, &&l8001, &&l8002, &&l8003, &&l8004, &&l8005, &&l8006, &&ldef,
        &&ldef, &&ldef, &&ldef, &&l800B, &&l800C, &&l800D, &&l800E, &&ldef,
        &&l8010
    };
    void (*fn)(NAS *, EXTanimator *, s16);
    u16 cmd;
    s32 ret;
    u32 i;
    const void *const *t;
    s32 vol;
    s32 (*hooks)[2];

    ret = 0;
    if (--anim->Aspeed < 0) {
        t = tbl;
        vol = 0x1E00;
        hooks = ACMD;
        do {
            anim->Aindex++;
            cmd = ((AnimStep *)anim->Atable)[anim->Aindex].cmd;
            if (((AnimStep *)anim->Atable)[anim->Aindex].cmd < 0) {
            i = cmd - 0x8000;
            if (i < 0x11) {
                goto *t[i];
            }
            goto ldef;
            l8000:
                anim->Aindex = -1;
                ret = 2;
                goto next;
            l8001:
                ScriptCursorNext((ScriptCursor *)anim);
                ret = 3;
                goto next;
            l8002:
                eng->direction = ~eng->direction;
                goto next;
            l8004:
                if (nas->trigger != NULL) {
                    nas->trigger->Status |= 0x8000;
                }
            l8003:
                KillNAS(nas);
                cmd = 0;
                goto next;
            l8006:
                cmd = sub->unk74;
            l8005:
                ret = ((AnimStep *)anim->Atable)[anim->Aindex].arg;
                goto next;
            l800B:
                PlaySfx(((AnimStep *)anim->Atable)[anim->Aindex].arg, &eng->sfx, NULL, 0x1000, vol, 0);
                goto next;
            l800D:
                PlaySfx(((AnimStep *)anim->Atable)[anim->Aindex].arg, &eng->sfx, NULL, 0x1000, vol, 0);
                goto next;
            l800C:
                PlaySfx(((AnimStep *)anim->Atable)[anim->Aindex].arg, NULL, &eng->unk10, 0x1000, vol, 0);
                goto next;
            l800E:
                PlaySfx(((AnimStep *)anim->Atable)[anim->Aindex].arg, NULL, NULL, 0x1000, vol, 0);
                goto next;
            l8010:
                sub->unk6C = &HeroModelBank[((AnimStep *)anim->Atable)[anim->Aindex].arg];
                goto next;
            ldef:
                fn = (void (*)(NAS *, EXTanimator *, s16))hooks[((s16)cmd >> 8) & 0xF][1];
                if (fn != NULL) {
                    fn(nas, anim, cmd);
                }
            next:;
            }
        } while ((s16)cmd < 0);
        sub->unk74 = cmd;
        anim->Aspeed = ((AnimStep *)anim->Atable)[anim->Aindex].arg;
    }
    return ret;
}

static inline FlagRecord *func_80025A48_inline(EXTengine *eng, u32 flag) {
    PointList *list;
    FlagRecord *rec;
    s32 n;

    if (eng->collision_box.flags & flag) {
        return &eng->collision_box;
    }
    if (eng->sub != NULL && (list = sub_point_list(eng->sub)) != NULL) {
        n = list->nRecs;
        rec = (FlagRecord *)&list->pts[list->count];
        for (; n != 0; n--, rec++) {
            if (rec->flags & flag) {
                return rec;
            }
        }
    }
    return NULL;
}

s32 CheckSolidCollision(NAS *nas) {
    EXTengine *eng;
    NAS *it;
    s32 hit;
    FlagRecord *box;
    EXTengine *other;
    s32 bot;
    s32 top;
    s32 dx;
    s32 dz;
    s32 d;
    s32 ang;
    s32 r;
    s32 lo;
    s32 hi;

    eng = nas->Engine;
    it = HeadOfSolids;
    hit = 0;
    if (eng != NULL && (box = func_80025A48_inline(eng, 0x50)) != NULL) {
        top = eng->unk10.y + box->unk2;
        bot = eng->unk10.y + box->height;
        do {
            FlagRecord *rec;

            if (it == NULL) {
                break;
            }
            if (it->Class & 0x200) {
                other = it->Engine;
                if (other != NULL && other != eng) {
                    rec = func_80025A48_inline(other, 0x50);
                    if (rec != NULL) {
                        lo = other->unk10.y + rec->unk2;
                        hi = other->unk10.y + rec->height;
                        if (bot < lo && hi < top) {
                            dx = other->unk10.x - eng->unk10.x;
                            dz = other->unk10.z - eng->unk10.z;
                            d = dx * dx + dz * dz;
                            r = rec->radius;
                            r *= r;
                            if (d < r) {
                                hit = 1;
                                ang = get_angle(dx, dz) & 0xFFF;
                                d = csqrt(d) >> 6;
                                d = rec->radius - d;
                                eng->unk10.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                eng->unk10.z -= (d * rcossin_tbl[ang].unk2) >> 12;
                            }
                        }
                    }
                }
            }
            it = it->Solid.Child;
        } while (1);
    }
    return hit;
}

FlagRecord *FindCollisionBox(EXTengine *eng, u32 flag) {
    PointList *list;
    FlagRecord *rec;
    s32 n;

    if (eng->collision_box.flags & flag) {
        return &eng->collision_box;
    }
    if (eng->sub != NULL && (list = sub_point_list(eng->sub)) != NULL) {
        n = list->nRecs;
        rec = (FlagRecord *)&list->pts[list->count];
        for (; n != 0; n--, rec++) {
            if (rec->flags & flag) {
                return rec;
            }
        }
    }
    return NULL;
}

void SetEngineMotion(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6) {
    (*(s16*)((u8*)(arg0) + 132)) = arg1;
    (*(s16*)((u8*)(arg0) + 134)) = arg2;
    (*(s16*)((u8*)(arg0) + 136)) = arg3;
    (*(s16*)((u8*)(arg0) + 142)) = 0;
    (*(s16*)((u8*)(arg0) + 138)) = (s16) arg4;
    (*(s16*)((u8*)(arg0) + 140)) = (s16) arg5;
    (*(s32*)((u8*)(arg0) + 144)) = arg6;
}

void FetchDefaultColBox(EXTengine *eng, u32 flag) {
    PointList *list;
    FlagRecord *rec;
    s32 n;

    eng->collision_box.flags = 0;
    if (eng->sub != NULL && (list = sub_point_list(eng->sub)) != NULL) {
        n = list->nRecs;
        rec = (FlagRecord *)&list->pts[list->count];
        for (; n != 0; n--, rec++) {
            if (rec->flags & flag) {
                eng->collision_box = *rec;
                return;
            }
        }
    }
}

s32 ApplyContourMoved(EXTengine *eng) {
    s32 dy;

    abs(eng->unk10.y - eng->OldXYZ.y);
    abs(eng->unk10.x - eng->OldXYZ.x);
    abs(eng->unk10.z - eng->OldXYZ.z);
    if (LevelSpecs.GetContour != NULL) {
        dy = LevelSpecs.GetContour(eng);
    } else {
        dy = AddPlatformContour(eng, eng->unk10.y - World_floor_level);
    }
    ContourHeight = dy;
    if (dy == 0x7FFFFFFF) {
        return 0;
    }
    if (dy < 0) {
        return 0;
    }
    eng->unk10.y -= dy;
    return 1;
}

s32 FollowUnderContour(EXTengine *eng) {
    s32 lim;
    s32 dy;

    lim = abs(eng->unk10.x - eng->OldXYZ.x);
    if (lim < 0x10) {
        lim = 4;
    }
    if (LevelSpecs.GetContour != NULL) {
        dy = LevelSpecs.GetContour(eng);
    } else {
        dy = AddPlatformContour(eng, eng->unk10.y - World_floor_level);
    }
    if (dy == 0x7FFFFFFF) {
        return 0;
    }
    if (dy >= 0) {
        return 0;
    }
    lim = ~lim + 1;
    if (dy < lim) {
        return 0;
    }
    eng->unk10.y += dy;
    return 1;
}

s32 FollowContour(EXTengine *eng) {
    s32 dy;
    int new_var;

    new_var = eng->unk10.x;
    new_var = -eng->OldXYZ.x + new_var;
    abs(new_var);
    abs(eng->unk10.z - eng->OldXYZ.z);
    if (LevelSpecs.GetContour != NULL) {
        dy = LevelSpecs.GetContour(eng);
    } else {
        dy = AddPlatformContour(eng, eng->unk10.y - World_floor_level);
    }
    if (dy == 0x7FFFFFFF) {
        return 1;
    }
    if (abs(dy) < 0x20) {
        eng->unk10.y -= dy;
        return 0;
    } else {
        return 1;
    }
}

s32 AddPlatformContour(EXTengine *eng, s32 y) {
    NAS *nas;
    EXTengine *other;
    FlagRecord *rec;
    s32 dx;
    s32 dz;
    s32 r;
    s32 top;
    s32 dy;

    nas = HeadOfSolids;
    ContourPlatform = NULL;
    if (nas != NULL) {
        do {
            if (nas->Class & 0x100) {
                other = nas->Engine;
                if (other != NULL && other != eng) {
                    rec = FindCollisionBox(other, 0x10);
                    if (rec != NULL) {
                        dx = other->unk10.x - eng->unk10.x;
                        dz = other->unk10.z - eng->unk10.z;
                        r = rec->radius + 0x40;
                        r *= r;
                        top = other->unk10.y + rec->height;
                        if (dx * dx + dz * dz < r) {
                            dy = eng->unk10.y - top;
                            if (abs(dy) < abs(y)) {
                                y = dy;
                                ContourPlatform = nas;
                            }
                        }
                    }
                }
            }
            nas = nas->Solid.Child;
        } while (nas != NULL);
    }
    return y;
}

void ApplyContour(EXTengine *eng) {
    if (LevelSpecs.GetContour == NULL) {
        AddPlatformContour(eng, eng->unk10.y - World_floor_level);
    } else {
        LevelSpecs.GetContour(eng);
    }
}

void nas_unused_a(void) {
}

void nas_unused_b(void) {
}

void nas_unused_c(void) {
}

void nas_unused_d(void) {
}

void GotoLinkAnim(EXTanimator *anim, s32 n) {
    switch (anim->EXTid & 0xFF) {
    case 1:
        if (anim->Alist != NULL) {
            anim->ALindex = n;
            anim->Atable = anim->Alist[anim->ALindex];
        }
        anim->Aindex = 0;
        anim->Aspeed = -1;
        anim->Seq_count = 0;
        break;
    case 2:
        if (anim->Alist != NULL) {
            anim->ALindex = n;
            anim->Atable = anim->Alist[anim->ALindex];
        }
        anim->Aindex = -1;
        anim->Aspeed = -1;
        anim->Seq_count = 0;
        break;
    default:
        anim->Seq_count = 0;
        break;
    }
    anim->Seq_count_down = 0;
}

ScriptCursor *CreateScriptCursor(NAS *nas, s32 arg1, s32 arg2, ScriptInit *init) {
    ScriptCursor *c;
    Ext400 *ext;
    u32 flags;

    flags = init->flags;
    if (flags & 0x400) {
        ext = AllocExtension(nas, 0x2C, 0x400, &ExtTemplate400);
        if (ext != NULL) {
            nas->NASid |= 0x400;
            *ext = ExtTemplate400;
        }
        c = (ScriptCursor *)ext;
        flags &= 0xFF;
        if (c != NULL) {
            *(s32 *)c = (*(s32 *)c & ~0xFF) | flags;
            c->cur = init->cur;
            c->tbl = init->tbl;
            *(s32 *)c->pad14 = arg1;
            *(s32 *)(c->pad14 + 4) = arg2;
            ScriptCursorNext(c);
            return c;
        }
    }
    return NULL;
}

void RunTriggers(void) {
    s32 i;
    NASTrigger *trig;

    trig = HeadOfTriggers;
    if (LevelSpecs.EarliestTriggers != NULL) {
        LevelSpecs.EarliestTriggers();
    }
    for (i = 0; i < TriggerCount; i++, trig++) {
        if (!(trig->Status & 0x8001)) {
            TriggerNas(trig);
        }
    }
}

s16 TextWidth(s32 x, u8 *s) {
    s32 sum;
    s32 c;
    s32 d;
    s32 e;

    c = *s;
    d = c & 0xFF;
    sum = 0;
    if (d != 0 && d != 10) {
        do {
            s32 idx;
            u8 *f;
            s8 *w;
            s32 v;
            if (c == 13) {
                break;
            }
            idx = *s + (FontPage << 8);
            w = &SysFontWidth[idx];
            f = &SysFontFlags[idx];
            v = *w;
            if (!(*f & 2)) {
                sum += v;
            }
            s++;
            c = *s;
            e = c & 0xFF;
        } while (e != 0 && e != 10);
    }
    return x - (sum * 5 * 128) / ScreenRect->w / 2;
}

void InitSysFontModel(void) {
    InitSysFont();
    FontPage = 0;
    sysfont_dbase = &HeroModelBank[DB_Font];
}

s32 SysGetLen(char *s) {
    s32 sum;
    s32 c;
    s32 d;
    s32 e;

    c = *s;
    d = c & 0xFF;
    sum = 0;
    if (d != 0 && d != 10) {
        do {
            s32 idx;
            u8 *f;
            s8 *w;
            s32 v;
            if (c == 13) {
                break;
            }
            idx = *s + (FontPage << 8);
            w = &SysFontWidth[idx];
            f = &SysFontFlags[idx];
            v = *w;
            if (!(*f & 2)) {
                sum += v;
            }
            s++;
            c = *s;
            e = c & 0xFF;
        } while (e != 0 && e != 10);
    }
    return sum;
}

IdEntry *FindIdEntry(IdEntry *p, s32 id) {
    if (p != NULL) {
        do {
            if (p->id == id) {
                return p;
            }
            if (p->id == 0x3E7) {
                return NULL;
            }
            p++;
        } while (p != NULL);
    }
    return NULL;
}

void nas_unused_e(void) {
}

void nas_unused_f(void) {
}

void nas_unused_g(void) {
}

void FreeAllExtensions(NAS *nas) {
    EXT *ext;

    ext = (EXT *)nas;
    while (ext->Extend.Child != NULL) {
        ext = ext->Extend.Child;
    }
    while (ext->Extend.Parent != NULL) {
        FreeExtension(ext);
        ext = ext->Extend.Parent;
    }
    nas->NASid = *(u8 *)nas;
}

void FreeNasHeap(void) {
    mal_free(D_8006EA00, &MainHeap);
    D_8006EA00 = 0;
}

void *FindExtension(NAS *nas, s32 id) {
    EXT *ext;
    s32 sub;

    sub = id & 0xFF;
    id &= ~0xFF;
    for (ext = nas->Extend.Child; ext != NULL; ext = ext->Extend.Child) {
        if (ext->EXTid & id) {
            if (sub == 0) {
            found:
                return ext;
            }
            if ((ext->EXTid & 0xFF) == sub) {
                goto found;
            }
        }
    }
    return NULL;
}

EXTmotion *AddMotion(NAS *nas) {
    EXTmotion *mot;

    mot = AllocExtension(nas, 0x34, 0x800, &ExtMotionTemplate);
    if (mot != NULL) {
        nas->NASid |= 0x800;
        *mot = ExtMotionTemplate;
    }
    return mot;
}

Ext400 *AllocExt400(NAS *nas) {
    Ext400 *ext;

    ext = AllocExtension(nas, 0x2C, 0x400, &ExtTemplate400);
    if (ext != NULL) {
        nas->NASid |= 0x400;
        *ext = ExtTemplate400;
    }
    return ext;
}

Ext1000 *AddObject(NAS *nas) {
    Ext1000 *ext;

    ext = AllocExtension(nas, 0x2C, 0x1000, &ExtTemplate1000);
    if (ext != NULL) {
        nas->NASid |= 0x1000;
        *ext = ExtTemplate1000;
    }
    return ext;
}

EXTengine *AddExtEngine(NAS *nas, s32 id) {
    EXTengine *eng;
    EXTengine *prev;
    EXTengine *ext;

    eng = AllocExtension(nas, 0x9C, id | 0x100, &EngineTemplate);
    if (eng != NULL) {
        nas->NASid |= 0x100;
        *eng = EngineTemplate;
        if (eng_add_subs_n(eng, 1) != 0) {
            prev = nas->Engine;
            if (prev == NULL) {
                nas->Engine = eng;
            } else {
                for (ext = (EXTengine *)prev->Extend.Child; ext != NULL; ext = (EXTengine *)ext->Extend.Child) {
                    if (ext->EXTid & 0x100) {
                        prev->Engine = ext;
                        prev = ext;
                    }
                }
            }
        } else {
            FreeExtension(eng);
            eng = NULL;
        }
    }
    return eng;
}

void AllocExtensionCopy(void *owner, s32 size, s32 id, Words3 *tmpl) {
    Words3 *ext;

    ext = AllocExtension(owner, size, id, tmpl);
    if (ext != NULL) {
        *ext = *tmpl;
    }
}

void *AddExt(NAS *owner, s32 size, s32 id) {
    return AllocExtension(owner, size, id, NULL);
}

void RemoveSolid(NAS *nas) {
    NAS **head = (NAS **)&HeadOfSolids;

    if (nas->Solid.Parent != NULL || nas->Solid.Child != NULL) {
        if (nas->Solid.Parent != NULL) {
            nas->Solid.Parent->Solid.Child = nas->Solid.Child;
            if (nas->Solid.Child != NULL) {
                nas->Solid.Child->Solid.Parent = nas->Solid.Parent;
            }
        } else {
            *head = nas->Solid.Child;
            nas->Solid.Child->Solid.Parent = NULL;
        }
        nas->Solid.Parent = NULL;
        nas->Solid.Child = NULL;
        nas->Class &= ~0x300;
    } else if (*head == nas) {
        *head = NULL;
    }
}

void AddSolid(NAS *nas, s32 bits) {
    if (HeadOfSolids != NULL) {
        nas->Solid.Child = HeadOfSolids;
        HeadOfSolids->Solid.Parent = nas;
    }
    HeadOfSolids = nas;
    nas->Class = (nas->Class & ~0x300) | bits;
}

void RemoveCamera(NAS *nas) {
    NAS **head = (NAS **)&HeadOfCameras;

    if (nas->Camera.Parent != NULL || nas->Camera.Child != NULL) {
        if (nas->Camera.Parent != NULL) {
            nas->Camera.Parent->Camera.Child = nas->Camera.Child;
            if (nas->Camera.Child != NULL) {
                nas->Camera.Child->Camera.Parent = nas->Camera.Parent;
            }
        } else {
            *head = nas->Camera.Child;
            nas->Camera.Child->Camera.Parent = NULL;
        }
        nas->Camera.Parent = NULL;
        nas->Camera.Child = NULL;
    } else if (*head == nas) {
        *head = NULL;
    }
}

void AddCamera(u8 *arg0) {
    if (HeadOfCameras != NULL) {
        (*(s32*)((u8*)(arg0) + 60)) = (u8 *) HeadOfCameras;
        (*(s32*)((u8*)(HeadOfCameras) + 56)) = arg0;
    }
    HeadOfCameras = arg0;
}

s32 CountObjects(void) {
    NAS *nas;
    s32 n;

    n = 0;
    for (nas = HeadOfObjects; nas != NULL; nas = nas->Object.Child) {
        n++;
    }
    return n;
}

void UnlinkObject(NAS *nas) {
    NAS **head = (NAS **)&HeadOfObjects;

    if (nas->Object.Parent != NULL || nas->Object.Child != NULL) {
        if (nas->Object.Parent != NULL) {
            nas->Object.Parent->Object.Child = nas->Object.Child;
            if (nas->Object.Child != NULL) {
                nas->Object.Child->Object.Parent = nas->Object.Parent;
            }
        } else {
            *head = nas->Object.Child;
            nas->Object.Child->Object.Parent = NULL;
        }
        nas->Object.Parent = NULL;
        nas->Object.Child = NULL;
    } else if (*head == nas) {
        *head = NULL;
    }
}

void LinkObject(u8 *arg0) {
    if (HeadOfObjects != NULL) {
        (*(s32*)((u8*)(arg0) + 44)) = (u8 *) HeadOfObjects;
        (*(s32*)((u8*)(HeadOfObjects) + 40)) = arg0;
    }
    HeadOfObjects = arg0;
}

void RemoveFromGroup(NAS *nas) {
    NAS **head = nas->Group.Head;

    if (head != NULL) {
        if (nas->Group.Parent != NULL) {
            nas->Group.Parent->Group.Child = nas->Group.Child;
            if (nas->Group.Child != NULL) {
                nas->Group.Child->Group.Parent = nas->Group.Parent;
            }
        } else {
            NAS *next = nas->Group.Child;
            *head = next;
            if (next != NULL) {
                nas->Group.Child->Group.Parent = NULL;
            }
        }
    }
    nas->Group.Parent = NULL;
    nas->Group.Child = NULL;
    nas->Group.Head = NULL;
}

void AddToGroup(u8 **arg0, u8 *arg1) {
    u8 *temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 != NULL) {
        (*(s32*)((u8*)(arg1) + 32)) = temp_v0;
        (*(s32*)((u8*)(temp_v0) + 28)) = arg1;
    }
    *arg0 = arg1;
    (*(s32*)((u8*)(arg1) + 36)) = arg0;
}

void ReAdoptChild(NAS *list, NAS *nas) {
    if (nas->Family.Parent != NULL) {
        nas->Family.Parent->Family.Child = nas->Family.Child;
    }
    if (nas->Family.Child != NULL) {
        nas->Family.Child->Family.Parent = nas->Family.Parent;
    }
    while (list->Family.Child != NULL) {
        list = list->Family.Child;
    }
    list->Family.Child = nas;
    nas->Family.Parent = list;
    nas->Family.Child = NULL;
}

void RemoveFamily(NAS *arg0) {
    u8 *temp_v1;
    u8 *temp_v1_2;

    temp_v1 = (*(s32*)((u8*)(arg0) + 20));
    if (temp_v1 != NULL) {
        (*(s32*)((u8*)(temp_v1) + 24)) = (u8 *) (*(s32*)((u8*)(arg0) + 24));
    }
    temp_v1_2 = (*(s32*)((u8*)(arg0) + 24));
    if (temp_v1_2 != NULL) {
        (*(s32*)((u8*)(temp_v1_2) + 20)) = (u8 *) (*(s32*)((u8*)(arg0) + 20));
    }
    (*(s32*)((u8*)(arg0) + 20)) = NULL;
    (*(s32*)((u8*)(arg0) + 24)) = NULL;
}

void AppendExtension(u8 *arg0, u8 *arg1) {
    u8 *var_a0;

    var_a0 = arg0;
    if ((*(s32*)((u8*)(var_a0) + 24)) != 0) {
        do {
            var_a0 = (*(s32*)((u8*)(var_a0) + 24));
        } while ((*(s32*)((u8*)(var_a0) + 24)) != 0);
    }
    (*(s32*)((u8*)(var_a0) + 24)) = arg1;
    (*(s32*)((u8*)(arg1) + 20)) = var_a0;
}

void InsertNasTag(NAS *arg0, NAS *arg1) {
    u8 *temp_v0;

    temp_v0 = (*(s32*)((u8*)(arg1) + 12));
    if (temp_v0 != NULL) {
        (*(s32*)((u8*)(temp_v0) + 16)) = arg0;
        (*(s32*)((u8*)(arg0) + 12)) = temp_v0;
        (*(s32*)((u8*)(arg0) + 16)) = arg1;
        (*(s32*)((u8*)(arg1) + 12)) = arg0;
    }
}

void MoveChildren(u8 *arg0, u8 *arg1) {
    u8 *temp_a2;
    u8 *temp_v1;
    u8 *temp_v1_2;

    temp_a2 = (*(s32*)((u8*)(arg1) + 12));
    if (temp_a2 != NULL) {
        temp_v1 = (*(s32*)((u8*)(arg0) + 12));
        if (temp_v1 != NULL) {
            (*(s32*)((u8*)(temp_v1) + 16)) = (u8 *) (*(s32*)((u8*)(arg0) + 16));
            temp_v1_2 = (*(s32*)((u8*)(arg0) + 16));
            if (temp_v1_2 != NULL) {
                (*(s32*)((u8*)(temp_v1_2) + 12)) = (u8 *) (*(s32*)((u8*)(arg0) + 12));
            } else {
                NASActiveTail = (*(s32*)((u8*)(arg0) + 12));
            }
        }
        (*(s32*)((u8*)(temp_a2) + 16)) = arg0;
        (*(s32*)((u8*)(arg0) + 12)) = temp_a2;
        (*(s32*)((u8*)(arg0) + 16)) = arg1;
        (*(s32*)((u8*)(arg1) + 12)) = arg0;
    }
}

void NullProc(NAS *arg0) {
}

void BuildNASFreeList(void) {
    NAS *nas;
    s32 i;

    NASFreeList = NULL;
    i = D_8005AA12 - 1;
    for (;;) {
        if (i >= FirstNas) {
            nas = &NasPool[i];
            if (nas->NASid == 0) {
                if (NASFreeList != NULL) {
                    NASFreeList->Tag.Parent = nas;
                    nas->Tag.Child = NASFreeList;
                    NASFreeList = nas;
                } else {
                    NASFreeList = nas;
                    nas->Tag.Child = NULL;
                }
                nas->Tag.Parent = NULL;
            }
            i--;
        } else {
            break;
        }
    }
}

void ResetNasLists(void) {
    s32 i;

    HeadOfCameras = NULL;
    HeadOfSolids = NULL;
    D_8005AA30 = NULL;
    HeadOfObjects = NULL;
    HeadOfMissiles = NULL;
    NASActiveHead = NULL;
    NASActiveTail = NULL;
    World_floor_level = 0;
    VelocityCalc = 1;
    BuildNASFreeList();
    for (i = 1; i < 16; i++) {
        ACMD[i][0] = 0;
        ACMD[i][1] = 0;
    }
}


void ClearNasPool(void) {
    s32 i;

    i = FirstNas;
    for (;;) {
        if (i < NasPoolUsed) {
            NasPool[i] = NASTemplate;
            i++;
        } else {
            break;
        }
    }
    BuildNASFreeList();
}

void ClearAllNasPool(void) {
    s32 i;

    i = 0;
    for (;;) {
        if (i >= NasPoolUsed) {
            return;
        }
        NasPool[i] = NASTemplate;
        i++;
    }
}

void KillAllNas(void) {
    s32 i;

    for (i = FirstNas; i < NasPoolUsed; i++) {
        KillNAS(&NasPool[i]);
    }
}

void KillEveryNas(void) {
    s32 i;

    for (i = 0; i < NasPoolUsed; i++) {
        KillNAS(&NasPool[i]);
    }
}

void DeleteNas_AllAST(s32 arg0) {
    u8 *temp_s0;
    u8 *var_a0;

    var_a0 = NASActiveHead;
    if (var_a0 != NULL) {
        do {
            temp_s0 = (*(s32*)((u8*)(var_a0) + 16));
            if (((*(s32*)((u8*)(var_a0) + 0)) & 1) && ((*(u16*)((u8*)(var_a0) + 108)) == (arg0 & 0xFFFF))) {
                KillNAS(var_a0);
            }
            var_a0 = temp_s0;
        } while (var_a0 != NULL);
    }
}

void KillTriggerNas(NAS *nas) {
    if (nas->trigger != NULL) {
        nas->trigger->Status |= 0x8000;
    }
    KillNAS(nas);
}

void RunModules(u8 *a) {
    ModuleEntry *p;

    p = (ModuleEntry *)a;
    while (p != NULL) {
        CurrentModule = p;
        p->init();
        while (CurrentModule == p) {
            p->update();
            p->render();
        }
        p = CurrentModule;
    }
}

s32 machine_init(void) {
    __main();
    ResetCallback();
    ResetGraph(0);
    modchip_check_region();
    SetGraphDebug(0);
    InitGeom();
    SsInit();
    VideoMode = 0;
    pads_init_mtap();
    SetVideoMode(VideoMode);
    DrawSync(0);
    ClearImage2(&D_8005B640, 0, 0, 0);
    DrawSync(0);
    machine_init_sound();
    debug_font_init2();
    shell_init();
    cd_read_wait();
    get_exe_exports();
    machine_quit_sound();
    StopCallback();
    PadStopCom();
    SetDispMask(0);
    ResetGraph(3);
    return 0;
}

void ClearVram(void) {
    DrawSync(0);
    ClearImage2(&D_8005B640, 0, 0, 0);
    DrawSync(0);
}

void FlipScreen(s32 arg0) {
    DISPENV sp10;
    RECT *r = ScreenRect;
    RECT *src;
    u8 *buf;

    FntFlush(D_800643E8);
    DrawSync(0);
    mal_update(&MainHeap);
    if (r != NULL) {
        if (DrawBuffers == cdb) {
            if (DrawBuffers) {
                src = r;
            } else {
                src = r;
            }
        } else {
            src = r + 1;
        }
        sp10.disp = *src;
        sp10.screen.x = 0;
        sp10.screen.w = 0x100;
        sp10.screen.y = (VideoMode == 0) ? 8 : 0x10;
        sp10.screen.h = (VideoMode == 0) ? 0xF0 : 0x100;
        sp10.isinter = ((u8 *)r)[0x14];
        sp10.isrgb24 = ((u8 *)r)[0x10];
        D_8004E0E4 = VSync(1);
        VSync(arg0);
        PutDispEnv(&sp10);
        DrawOTag((u32 *)(cdb + 0x3FFC));
        buf = DrawBuffers;
        if (cdb == buf) {
            buf += 0x400C;
        }
        cdb = buf;
        FrameCount += 1;
        ClearOTagR((u32 *)buf, 0x1000);
        *(s32 *)(buf + 0x4008) = *(s32 *)(buf + 0x4000);
    }
    prims_update();
}

void InitDisplay(void) {
    DISPENV disp;
    DRAWENV draw;

    DrawSync(0);
    SetDefDispEnv(&disp, 0, 0, 0x200, 0xF0);
    SetDispMask(1);
    disp.disp.x = 0;
    disp.disp.y = 0;
    disp.disp.w = 0x200;
    disp.disp.h = 0xF0;
    disp.screen.x = 0;
    disp.screen.y = 0;
    disp.screen.w = 0x200;
    disp.screen.h = 0xF0;
    disp.isinter = 0;
    disp.isrgb24 = 0;
    PutDispEnv(&disp);
    SetDefDrawEnv(&draw, 0, 0, 0x200, 0xF0);
    draw.dfe = 1;
    draw.isbg = 1;
    draw.dtd = 0;
    while (1) {
        ClearOTag((u_long *)cdb, 0x10);
        draw.r0 = Random(0xFF);
        draw.g0 = Random(0xFF);
        draw.b0 = Random(0xFF);
        PutDrawEnv(&draw);
        DrawOTag((u_long *)cdb);
        DrawSync(0);
        VSync(2);
    }
}

s32 AllocOtBuffers(s32 *arg0) {
    OtBuf *b;
    u8 *t;

    t = mal_alloc(*arg0 + 0x1800, &MainHeap);
    b = (OtBuf *)DrawBuffers;
    b[0].prim = t;
    b[0].end = b[0].prim + *arg0 + 0x1800;
    b[1].prim = mal_alloc(*arg0 + 0x1800, &MainHeap);
    b[1].end = b[1].prim + *arg0 + 0x1800;
    ScreenRect = NULL;
    return (s32)b[1].end;
}

void ClearScreens(void) {
    RECT *temp_s0;

    temp_s0 = ScreenRect;
    if (temp_s0 != 0) {
        DrawSync(0);
        ClearImage(&temp_s0[1], 0, 0, 0);
        ClearImage(&temp_s0[0], 0, 0, 0);
    }
}

void SetScreen(arg0)
RECT *arg0;
{
    DrawSync(0);
    D_8005B78C = 0;
    if (arg0 != NULL) {
        D_8005B694 = FrameCount + 1;
        if (ScreenRect == NULL) {
            u8 *g = DrawBuffers;
            u8 *h;

            ClearOTagR((u_long *)g, 0x1000);
            h = g + 0x400C;
            *(s32 *)(g + 0x4008) = *(s32 *)(g + 0x4000);
            ClearOTagR((u_long *)h, 0x1000);
            *(s32 *)(h + 0x4008) = *(s32 *)(h + 0x4000);
        }
    } else {
        SetDispMask(0);
    }
    ScreenRect = arg0;
}

void ResetScreen(void) {
    SetScreen();
}

void AllocPrimBuffers(s32 *arg0) {
    u8 *g;
    u8 *t;

    t = mal_alloc(*arg0 + 0x1800, &MainHeap);
    g = DrawBuffers;
    *(u8 **)(g + 0x4000) = t;
    *(s32 *)(g + 0x4004) = *(s32 *)(g + 0x4000) + *arg0 + 0x1800;
    *(u8 **)((g += 0x8000) + 0xC) = mal_alloc(*arg0 + 0x1800, &MainHeap);
    *(s32 *)(g + 0x10) = *(s32 *)(g + 0xC) + *arg0 + 0x1800;
}

void ClearOt(u8 *arg0) {
    ClearOTagR((u_long *)arg0, 0x1000);
    *(s32 *)(arg0 + 0x4008) = *(s32 *)(arg0 + 0x4000);
}

s32 cd_read(u8 *a, s32 b, u8 *c, s32 d) {
    s32 r;
    s32 s;
    r = CdReadStatus;
    if (r > 0) {
        CdReadStatus = CdReadSync(1, 0);
        if (CdReadStatus == 0) {
            CdControl(1, 0, D_8006EA08);
            func_80034AEC();
        }
        s = CdReadStatus;
    } else {
        s = r;
    }
    if (s > 0) {
        return -1;
    }
    CdIntToPos(*(s32 *)a + b + D_800637B8, &D_8006EA10);
    if (CdControl(2, (u8 *)&D_8006EA10, 0) == 0 || CdRead(d, (u_long *)c, 0x80) == 0) {
        return -1;
    }
    CdReadStatus = 1;
    CdReadStatus = CdReadSync(1, 0);
    if (CdReadStatus == 0) {
        CdControl(1, 0, D_8006EA08);
        func_80034AEC();
    }
    return CdReadStatus;
}

void cd_seek(s32 *arg0, s32 arg1) {
    CdIntToPos(*arg0 + arg1, &D_8006EA14);
    CdControlF(CdlSetloc, (u8 *)&D_8006EA14);
    CdControlF(CdlSeekL, (u8 *)&D_8006EA14);
}

void cd_read_wait(void) {
    if (CdReadStatus > 0) {
        CdReadStatus = CdReadSync(0, 0);
    }
}

s32 cd_read_poll(void) {
    s32 r;
    r = CdReadStatus;
    if (r > 0) {
        CdReadStatus = CdReadSync(1, 0);
        if (CdReadStatus == 0) {
            CdControl(1, 0, D_8006EA08);
            func_80034AEC();
        }
        r = CdReadStatus;
    }
    return r;
}

void cd_init(void) {
    CdlFILE file;

    do {
        CdInit();
    } while (CdDiskReady(1) != 2);
    CdSync(0, 0);
    CdReadStatus = 0;
    while (CdSearchFile(&file, "\\CRASHBSH\\CRASHBSH.DAT;1") == NULL) {
        printf("Cant find CRASHBSH.DAT");
    }
    D_800637B8 = CdPosToInt(&file.pos);
}

void xa_setvol(s32 vol) {
    SpuCommonAttr attr;

    attr.mask = SPU_COMMON_MVOLL | SPU_COMMON_MVOLR | SPU_COMMON_CDVOLL | SPU_COMMON_CDVOLR | SPU_COMMON_CDMIX;
    attr.mvol.left = 0x37FF;
    attr.mvol.right = 0x37FF;
    attr.cd.volume.left = vol;
    attr.cd.volume.right = vol;
    attr.cd.mix = 1;
    SpuSetCommonAttr(&attr);
}

void loader_pending_stub(void) {
    loader_pending();
}

void cd_unused(void) {
}

void xa_mute(void) {
    if (xa_count >= 0x1F) {
        xa_count = 0x1E;
    }
    xa_setvol(0);
}

void pad_read_packet(PadPacket *p, PadInput *in, s32 n) {
    if (p->status == 0) {
        in->flags |= 1;
        in->flags |= PadRumble_update(&in->rumble, n);
        if (FrameCount > D_80063A8C + 10 &&(in->flags & 8) && in->unk18 != 0
            && PadGetState(n) == 6 && PadSetMainMode(n, 1, 2) == 1) {
            in->unk18 = 0;
            D_80063A8C = FrameCount;
        }
        in->flags &= ~6;
        switch (*(u16 *)p >> 12) {
        case 7:
            in->flags |= 6;
            in->rx += p->rx;
            in->ry += p->ry;
            in->lx += p->lx;
            in->ly += p->ly;
            in->rx >>= 1;
            in->ry >>= 1;
            in->lx >>= 1;
            in->ly >>= 1;
            in->buttons = ~p->buttons & 0xFFFF;
            break;
        case 4:
            in->flags |= 4;
            in->buttons = ~p->buttons & 0xFFFF;
            break;
        }
    } else {
        in->buttons = 0;
        in->flags = 0;
        in->unk18 = 1;
        in->rumble.unk0 = 0;
        in->rumble.dirty = 0;
        in->rumble.reset = 0;
    }
}

static inline s32 func_80027D90_read(u8 *src, u8 *dst, s32 n) {
    s32 count;

    if ((*(u16 *)src & 0xF000) == 0x8000) {
        if (n == 0) {
            pad_read_packet(src + 2, dst, 0);
            pad_read_packet(src + 0xA, dst + 0x58, 1);
            pad_read_packet(src + 0x12, dst + 0xB0, 2);
            pad_read_packet(src + 0x1A, dst + 0x108, 3);
            count = 4;
        } else {
            count = 0;
        }
    } else {
        pad_read_packet(src, dst, n);
        count = 1;
    }
    return count;
}

void pads_read(void) {
    s32 count;
    PadState *pad;

    pad = Pads;
    PadCount = 0;
    PadCount += func_80027D90_read(D_80063A90[0], (u8 *)pad, 0);
    PadCount += func_80027D90_read(D_80063A90[1], (u8 *)&pad[PadCount], 0x10);
    count = 8 - PadCount;
    pad += PadCount;
    if (count > 0) {
        do {
            pad->unk4 = 0;
            pad->unk0 = 0;
            pad->unk18 = 1;
            pad++;
        } while (--count > 0);
    }
}

void pads_init_mtap(void) {
    PadState *pad;
    s32 i;
    s32 on;

    PadCount = 0;
    PadInitMtap(D_80063A90[0], D_80063A90[1]);
    i = 0;
    on = 1;
    pad = Pads;
loop:
    if (i < 8) {
        i++;
        pad->unk4 = 0;
        pad->unk0 = 0;
        pad->unk18 = on;
        pad->unk1C = 0;
        pad->unk22 = 0;
        pad->unk28 = 0;
        pad++;
        goto loop;
    }
    card_init();
    PadStartCom();
}

void pad_rumble_big(s32 pad, s32 arg1, s8 arg2) {
    u8 *p;

    if (GameInfo[pad + 0x5C] == 0 && !(*(s32 *)(GameInfo + 4) & 0x80) && GameOptions[pad + 0xC] != 0 &&
        (Pads[pad].unk0 & 8)) {
        p = (u8 *)&Pads[pad] + 0x1C;
        p[5] = arg2;
        *(s16 *)(p + 0xA) = arg1 + 1;
        *(u16 *)(p + 6) |= 1;
    }
}

void pad_rumble_small(s32 pad, s32 arg1) {
    u8 *p;

    if (GameInfo[pad + 0x5C] == 0 && !(*(s32 *)(GameInfo + 4) & 0x80) && pad < 4 && GameOptions[pad + 0xC] != 0 &&
        (Pads[pad].unk0 & 8)) {
        p = (u8 *)&Pads[pad] + 0x1C;
        p[4] = 1;
        *(s16 *)(p + 8) = arg1 + 1;
        *(u16 *)(p + 6) |= 1;
    }
}

s32 pad_read_mtap(u8 *src, u8 *dst, s32 n) {
    s32 count;

    if ((*(u16 *)src & 0xF000) == 0x8000) {
        if (n == 0) {
            pad_read_packet(src + 2, dst, 0);
            pad_read_packet(src + 0xA, dst + 0x58, 1);
            pad_read_packet(src + 0x12, dst + 0xB0, 2);
            pad_read_packet(src + 0x1A, dst + 0x108, 3);
            count = 4;
        } else {
            count = 0;
        }
    } else {
        pad_read_packet(src, dst, n);
        count = 1;
    }
    return count;
}

void pads_reset_rumble(void) {
    s32 v;
    s32 i;

    v = 1;
    for (i = 7; i >= 0; i--) {
        Pads[i].unk28 = v;
    }
}

void pad_set_rumble_big(u8 *arg0, s32 arg1, s8 arg2) {
    (*(s8*)((u8*)(arg0) + 5)) = arg2;
    (*(s16*)((u8*)(arg0) + 10)) = (s16) (arg1 + 1);
    (*(u16*)((u8*)(arg0) + 6)) = (u16) ((*(u16*)((u8*)(arg0) + 6)) | 1);
}

void pad_set_rumble_small(u8 *arg0, s32 arg1) {
    (*(s8*)((u8*)(arg0) + 4)) = 1;
    (*(s16*)((u8*)(arg0) + 8)) = (s16) (arg1 + 1);
    (*(u16*)((u8*)(arg0) + 6)) = (u16) ((*(u16*)((u8*)(arg0) + 6)) | 1);
}

s32 pad_rumble_send(PadRumble *r, s32 port) {
    if (r->reset != 0) {
        if (r->time0 != 0) {
            r->time0 = 1;
        }
        if (r->time1 != 0) {
            r->time1 = 1;
        }
    }
    r->reset = 0;
    if (r->time0 != 0) {
        r->time0 = r->time0 - 1;
        if ((r->time0 << 16) == 0) {
            r->act[0] = 0;
            r->dirty |= 1;
        }
    }
    if (r->time1 != 0) {
        r->time1 = r->time1 - 1;
        if ((r->time1 << 16) == 0) {
            r->act[1] = 0;
            r->dirty |= 2;
        }
    }
    if (PadGetState(port) != 6) {
        return 0;
    }
    if (r->dirty != 0) {
        PadSetAct(port, r->act, 2);
        if (PadSetActAlign(port, D_80063A84) == 1) {
            r->dirty = 0;
        }
    }
    return 8;
}

void pad_input_reset(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 4)) = 0;
    (*(s32*)((u8*)(arg0) + 0)) = 0;
    (*(s32*)((u8*)(arg0) + 24)) = 1;
    (*(s32*)((u8*)(arg0) + 28)) = 0;
    (*(s16*)((u8*)(arg0) + 34)) = 0;
    (*(s16*)((u8*)(arg0) + 40)) = 0;
}

FreeHdr *vram_take(u8 *arg0) {
#define pool ((FreePool *)arg0)
    FreeNode *a2;
    FreeNode *a3;
    FreeNode *a1;
    FreeNode *v1;
    FreeNode *t;
    FreeHdr *p;

    t = pool->head;
    if (t != NULL) {
        a3 = t;
        pool->head = a3->next;
        if (a3->next != NULL) {
            a3->next->f8 = 0;
        }
        if (a3->fC != NULL) {
            a3->f10 = ((FreeNode *)a3->fC)->f10;
        }
        return &a3->hdr;
    }
    if ((u8 *)pool < D_80063BA0) {
        p = vram_take((u8 *)(pool + 1));
        a2 = FreeBufNodes;
        FreeBufNodes = a2->next;
        a3 = FreeBufNodes;
        FreeBufNodes = a3->next;
        a1 = FreeBufNodes;
        a2->next = pool->head;
        pool->head = a2;
        v1 = a2->next;
        a2->f8 = 0;
        if (v1 != NULL) {
            v1->f8 = (s32)a2;
        }
        a2->f10 = NULL;
        a2->fC = a3;
        a2->f14 = (s32)pool;
        a2->hdr.f0 = p->f0 + pool->w;
        a2->hdr.f2 = p->f2 + pool->h;
        a3->f10 = p;
        a3->fC = a2;
        a3->hdr = *p;
        a3->f14 = (s32)pool;
        return &a3->hdr;
    }
    return NULL;
#undef pool
}

ClutPos *clut_alloc_small(void) {
    s32 i;
    FreeNode *a;
    FreeNode *head;
    FreeNode *prev;
    FreeNode *n;
    FreeNode *nx;
    FreeNode *t;
    FreeHdr *src;
    FreeHdr h;
    FreeNode *ret;
    s32 cnt;
    FreeNode *u;

    a =D_80063BAC;
    if (a == NULL) {
        head = FreeBufNodes;
        FreeBufNodes = head->next;
        src = vram_take(D_80063B58);
        prev = NULL;
        h = *src;
        i = 0x1F;
        do {
            n = FreeBufNodes;
            nx = n->next;
            i--;
            n->next = prev;
            n->fC = head;
            n->hdr = h;
            prev = n;
            FreeBufNodes = nx;
            h.f2++;
        } while (i != -1);
        a = head;
        a->f14 = 0x20;
        a->f10 = n;
        a->fC = src;
        a->f8 = 0;
        a->next = NULL;
        D_80063BAC = a;
    }
    ret = a->f10;
    cnt = a->f14;
    u = ret->next;
    cnt--;
    a->f14 = cnt;
    a->f10 = u;
    if (cnt == 0) {
        t = a->next;
        D_80063BAC = t;
        if (t != NULL) {
            t->f8 = 0;
        }
        a->next = D_80063BB0;
        if (D_80063BB0 != NULL) {
            D_80063BB0->f8 = (s32)a;
        }
        a->f8 = 0;
        D_80063BB0 = a;
    }
    return ret;
}

static inline void BufNode_Release(BufNode *arg0) {
    BufNode *p;
    BufNode *old;

    p = arg0->parent;
    if (p != NULL && p->child == NULL) {
        if (p->prev != NULL) {
            p->prev->next = p->next;
        } else {
            *p->head = p->next;
        }
        if (p->next != NULL) {
            p->next->prev = p->prev;
        }
        vram_free_node(arg0->child);
        old = (BufNode *)FreeBufNodes;
        FreeBufNodes = (FreeNode *)arg0;
        p->next = old;
        arg0->next = p;
        return;
    }
    arg0->child = NULL;
    arg0->next = *arg0->head;
    *arg0->head = arg0;
    arg0->prev = NULL;
    if (arg0->next != NULL) {
        arg0->next->prev = arg0;
    }
}

void vram_release(FreeNode *n) {
    FreeNode *a;
    FreeNode *p;

    a = n->fC;
    if (a == NULL) {
        n->next = D_80063BB4;
        D_80063BB4 = n;
        return;
    }
    n->next = a->f10;
    a->f10 = n;
    if (a->f14 == 0) {
        if ((FreeNode *)a->f8 != NULL) {
            ((FreeNode *)a->f8)->next = a->next;
        } else {
            D_80063BB0 = a->next;
        }
        if (a->next != NULL) {
            a->next->f8 = a->f8;
        }
        a->next = D_80063BAC;
        if (D_80063BAC != NULL) {
            D_80063BAC->f8 = (s32)a;
        }
        a->f8 = 0;
        D_80063BAC = a;
    }
    a->f14++;
    if (a->f14 == 0x20) {
        p = a->f10;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = FreeBufNodes;
        FreeBufNodes = a->f10;
        if ((FreeNode *)a->f8 != NULL) {
            ((FreeNode *)a->f8)->next = a->next;
        } else {
            D_80063BAC = a->next;
        }
        if (a->next != NULL) {
            a->next->f8 = a->f8;
        }
        a->next = FreeBufNodes;
        FreeBufNodes = a;
        BufNode_Release((BufNode *)a->fC);
    }
}

void vram_init(VramArea *areas, FreeNode *pool, s32 npool, s32 nbuf) {
    FreeNode *n;
    BufNode *b;
    BufNode *first;
    s32 i;

    D_800643F4 = mal_alloc(nbuf * 0x18, &MainHeap);
    D_80063BB4 = pool;
    n = pool;
    i = npool - 2;
    while (i != -1) {
        i--;
        n->next = n + 1;
        n->fC = NULL;
        n++;
    }
    n->next = NULL;
    D_80063BAC = NULL;
    D_80063BB0 = NULL;
    FreeBufNodes = (FreeNode *)(b = (BufNode *)D_800643F4);
    i = nbuf - 2;
    while (i != -1) {
        b->next = b + 1;
        i--;
        b++;
    }
    b->next = NULL;
    for (i = 11; i >= 0; i--) {
        *(s32 *)&D_80063B1C[i] = 0;
    }
    while ((s16)areas->hdr.f0 != -1) {
        b = (BufNode *)FreeBufNodes;
        b->parent = NULL;
        b->child = NULL;
        b->head = areas->head;
        first = *areas->head;
        FreeBufNodes = (FreeNode *)b->next;
        b->next = first;
        if (first != NULL) {
            first->prev = b;
        }
        b->prev = NULL;
        *areas->head = b;
        *(FreeHdr *)b = areas->hdr;
        areas++;
    }
    D_800335BC = 0x2626001F;
    FntLoad(0x380, 0x100);
    D_800643E8 = FntOpen(0x10, 0x10, ScreenRect->w - 8, 0xE0, 0, 0x800);
    D_800643EC = GetTPage(0, 1, 0x380, 0x100) & 0xFFFF;
    D_800643F0 = LoadClut2(D_80063AF8, 0x380, 0x11F) & 0xFFFF;
}

s32 vram_slot_class(s32 a, s32 b, s32 c) {
    s32 r;
    s32 h;

    r = 0;
    if (c == 0x10) {
        h = a >> 1;
        if (h >= b) {
            b = h;
            c = 0x100;
            goto dec;
        }
    }
    if (b < a) {
        b = a;
    }
dec:
    b--;
    b >>= 3;
    while (b > 0) {
        b >>= 1;
        r++;
    }
    r <<= 1;
    if (c == 0x100) {
        r++;
    }
    return r;
}

void *clut_alloc_large(void) {
    u8 *cur;

    cur = D_80063BB4;
    if (cur != NULL) {
        D_80063BB4 = *(void **)(cur + 4);
    }
    return cur;
}

FreeNode *vram_alloc_node(void) {
    FreeNode *head;
    FreeNode *prev;
    FreeNode *n;
    FreeNode *nx;
    FreeHdr *src;
    FreeHdr h;
    s32 i;

    head = FreeBufNodes;
    FreeBufNodes = head->next;
    src = vram_take(D_80063B58);
    prev = NULL;
    h = *src;
    i = 0x1F;
    do {
        n = FreeBufNodes;
        nx = n->next;
        i--;
        n->next = prev;
        n->fC = head;
        n->hdr = h;
        prev = n;
        FreeBufNodes = nx;
        h.f2++;
    } while (i != -1);
    head->f10 = n;
    head->f14 = 0x20;
    head->fC = src;
    return head;
}

void vram_free_node(BufNode *arg0) {
    BufNode *p;
    BufNode *old;

    p = arg0->parent;
    if (p != NULL && p->child == NULL) {
        if (p->prev != NULL) {
            p->prev->next = p->next;
        } else {
            *p->head = p->next;
        }
        if (p->next != NULL) {
            p->next->prev = p->prev;
        }
        vram_free_node(arg0->child);
        old = (BufNode *)FreeBufNodes;
        FreeBufNodes = (FreeNode *)arg0;
        p->next = old;
        arg0->next = p;
        return;
    }
    arg0->child = NULL;
    arg0->next = *arg0->head;
    *arg0->head = arg0;
    arg0->prev = NULL;
    if (arg0->next != NULL) {
        arg0->next->prev = arg0;
    }
}

s32 list_count(u8 **arg0) {
    u8 *p;
    s32 n;

    n = 0;
    for (p = *arg0; p != NULL; p = *(u8 **)(p + 4)) {
        n++;
    }
    return n;
}

void debug_font_init(void) {
    D_800335BC = 0x2626001F;
    FntLoad(0x380, 0x100);
    D_800643E8 = FntOpen(0x10, 0x10, ScreenRect->w - 8, 0xE0, 0, 0x800);
    D_800643EC = GetTPage(0, 1, 0x380, 0x100) & 0xFFFF;
    D_800643F0 = LoadClut2(D_80063AF8, 0x380, 0x11F) & 0xFFFF;
}

void debug_font_init2(void) {
    D_800335BC = 0x2626001F;
    FntLoad(0x380, 0x100);
    D_800643E8 = FntOpen(0x10, 0x10, ScreenRect->w - 8, 0xE0, 0, 0x800);
    D_800643EC = GetTPage(0, 1, 0x380, 0x100) & 0xFFFF;
    D_800643F0 = LoadClut2(D_80063AF8, 0x380, 0x11F) & 0xFFFF;
}

void texture_unused_a(void) {
}

void texture_upload_anim(AnimTex *t, s32 base, AnimTexSet *set) {
    TexUV uv;
    s16 y;
    u8 *p;
    AnimClut *cl;

    t->pos = vram_take(t->src);
    if (t->pos != NULL) {
        t->rect.x = t->pos->f0;
        t->rect.y = y = t->pos->f2;
        if (t->mode != 0) {
            t->tpage = getTPage(1, 1, t->rect.x >> 7 << 7, y);
            uv.f.u = t->rect.x << 1;
        } else {
            t->tpage = getTPage(0, 1, t->rect.x, y);
            uv.f.u = t->rect.x << 2;
        }
        if ((t->flags & 2) && GameLanguage < t->frames) {
            p = t[GameLanguage].pix + base;
        } else {
            p = t->pix + base;
        }
        LoadImage(&t->rect, (u_long *)p);
        uv.h = uv.b[0] | (*(u8 *)&t->rect.y << 8);
        t->uv0 = uv.h;
        ((u8 *)&uv)[0] += t->du;
        t->uv1 = uv.h;
        ((u8 *)&uv)[1] += t->dv;
        t->uv3 = uv.h;
        uv.b[0] -= t->du;
        t->uv2 = uv.h;
        if (t->clut != 0x7FFF) {
            cl = &set->cluts[t->clut];
            t->clutId = cl->id;
        }
    }
}

void texture_update_scroll(texture_scroll *s, obj_textureset *o) {
    AnimEntry *e;
    DR_MOVE *p;
    RECT r;
    s32 pos;
    s32 y;

    e = (AnimEntry *)&o->tex[s->idx];
    pos = s->pos + s->speed;
    s->pos = pos;
    if (s->speed > 0) {
        if ((pos >> 8) >= e->h) {
            s->pos = pos - (e->h << 8);
        }
    } else if (pos < 0) {
        s->pos = pos + (e->h << 8);
    }
    y = s->pos >> 8;
    r.x = s->dst[0];
    r.y = s->dst[1] + y;
    r.w = e->w;
    r.h = e->h - y;
    p = (*(DR_MOVE **)(cdb + 0x4008))++;
    SetDrawMove(p, &r, e->src[0], e->src[1]);
    addPrim(cdb + 0x3FFC, p);
    r.y = s->dst[1];
    r.h = y;
    p = (*(DR_MOVE **)(cdb + 0x4008))++;
    SetDrawMove(p, &r, e->src[0], e->src[1] + e->h - y);
    addPrim(cdb + 0x3FFC, p);
}

void texture_update_anims(obj_textureset *arg0) {
    AnimData *d;
    s32 *p;
    s32 n;
    s32 i;
    u8 *dst;
    s32 old;
    s32 pos;
    s32 *q;
    s32 *r;
    s32 *base;

    d = arg0->anims;
    if (d != NULL) {
        if (d->tracksOff != 0) {
            p = (s32 *)((u8 *)d + d->tracksOff);
            n = *p;
            base = p + 1;
            while (1) {
                if (--n == -1) {
                    break;
                }
                i = base[1];
                pos = base[4] + base[3];
                dst = (u8 *)&arg0->tex[i];
                old = base[4] >> 8;
                base[4] = pos;
                if (base[3] < 0) {
                    if (pos < 0) {
                        base[4] = pos + (base[2] << 8);
                    }
                } else if ((pos >> 8) >= base[2]) {
                    base[4] = pos - (base[2] << 8);
                }
                if ((base[4] >> 8) != old) {
                    q = (s32 *)((u8 *)base + *base) + (base[4] >> 8);
                    LoadImage((RECT *)(dst + 0x18), (u32 *)((u8 *)q + *q));
                }
                base += 5;
            }
        }
        if (d->subOff != 0) {
            p = (s32 *)((u8 *)d + (d->subOff + 4));
            r = p + 1;
            n = *p;
            if (--n != -1) {
                do {
                    texture_update_scroll(r, arg0);
                    r += 4;
                } while (--n != -1);
            }
        }
    }
}

void texture_init_mem_textureset(obj_textureset *arg0) {
    s32 n;
    u8 *src;
    TexEnt12 *e;
    TexSrc2A84C *s;
    TexDst2A84C *d;
    u8 *b;
    s32 *p;
    s32 *r;
    s32 cnt;
    u8 *a;
    s32 off;
    u32 t;
    s32 u;

    n = arg0->nEnt;
    e = arg0->ent;
    b = arg0->base;
    src = b + (*(s32 *)(b + 0x10) + 0x10);
    while (1) {
        u16 id;
        u8 *q;
        s32 len;

        if (--n == -1) {
            break;
        }
        q = src;
        len = (s32)arg0->base;
        id = *(u16 *)q;
        q -= len - 4;
        e->id = id;
        e->f4 = 0;
        e->off = (s32)q;
        e++;
        src += *(u16 *)src * 2 + 4;
    }
    n = arg0->nTex;
    d = arg0->tex;
    b = arg0->base;
    s = (TexSrc2A84C *)(b + (*(s32 *)(b + 0xC) + 0xC));
    while (1) {
        u8 *b;
        u32 t;
        s32 u;

        if (--n == -1) {
            break;
        }
        b = arg0->base;
        d->f20 = 0;
        d->f34 = -1;
        d->flag = *(u16 *)&s->wC & 1;
        d->f2 = (s->wC >> 1) & 0x7FFF;
        d->f12 = *(u16 *)((u8 *)s + 0xE);
        d->f14 = s->w10;
        d->fA = s->w2;
        if (d->flag == 0) {
            d->f8 = s->w0 << 2;
            d->slot = &D_80063B1C[vram_slot_class(s->w0 << 2, s->w2, 0x10)];
            d->f10 = (*(u8 *)s << 2) - 1;
        } else {
            d->f8 = s->w0 << 1;
            d->slot = &D_80063B1C[vram_slot_class(s->w0 << 1, s->w2, 0x100)];
            d->f10 = (*(u8 *)s << 1) - 1;
        }
        d->f11 = *(u8 *)((u8 *)s + 2) - 1;
        d->f1C = s->w0;
        d->f1E = s->w2;
        u = (s32)b - 0x14;
        t = s->w8;
        d->fC = t;
        d->f30 = (s32)s - u;
        d++;
        s = (TexSrc2A84C *)((u8 *)s + (s->w0 * s->w2 * 2 + 0x14));
    }
    b = arg0->base;
    off = *(s32 *)(b + 0x1C);
    if (off != 0) {
        p = (s32 *)(b + (off + 0x1C));
        r = p + 1;
        cnt = *p;
        a = mal_alloc(cnt * 0x5A8, &MainHeap);
        arg0->nAnim = cnt;
        arg0->anim = a;
        while (--cnt != -1) {
            *(s32 *)(a + 0x588) = *r++;
            a += 0x5A8;
        }
    } else {
        arg0->nAnim = 0;
        arg0->anim = NULL;
    }
    if (*(s32 *)(arg0->base + 0x18) != 0) {
        arg0->anims = (AnimData *)(arg0->base + *(s32 *)(arg0->base + 0x18));
    } else {
        arg0->anims = NULL;
    }
}

static inline void Tex_UploadCluts(u8 *set) {
    RECT rect;
    ClutUpload *e;
    u8 *base;
    s32 n;

    e = *(ClutUpload **)(set + 0xC);
    base = *(u8 **)(set + 0x10);
    n = *(s32 *)(set + 4);

    for (;;) {
        n--;
        if (n == -1) {
            return;
        }
        if (e->w < 0x11U) {
            e->pos = clut_alloc_small();
        } else {
            e->pos = clut_alloc_large();
        }
        rect.x = e->pos->x;
        rect.y = e->pos->y;
        rect.w = e->w;
        rect.h = 1;
        LoadImage(&rect, (u_long *)(base + e->off));
        e->clut = GetClut(rect.x, rect.y);
        e++;
    }
}

static inline void Tex_UploadList(TexUploadList *arg0, u8 *arg1) {
    s32 n;
    u8 *rec;
    s32 *ip;
    FreeHdr **pp;
    s32 base;
    RECT rect;

    ip = &arg0->e[0].idx;
    n = arg0->count;
    pp = &arg0->e[0].pos;
loop:
    n--;
    if (n != -1) {
        rec = *(u8 **)(arg1 + 8) + *ip * 0x38;
        ip += 4;
        *pp = vram_take(*(u8 **)(rec + 4));
        rect.w = *(u16 *)(rec + 0x1C);
        rect.h = *(u16 *)(rec + 0x1E);
        rect.x = (*pp)->f0;
        rect.y = (*pp)->f2;
        pp += 4;
        base = *(s32 *)(arg1 + 0x10);
        LoadImage(&rect, (u_long *)(*(s32 *)(rec + 0x30) + base));
        goto loop;
    }
}

void textureset_upload(u8 *arg0) {
    AnimTex *t;
    u8 *base;
    s32 n;
    u8 *p;

    t = *(AnimTex **)(arg0 + 8);
    base = *(u8 **)(arg0 + 0x10);
    Tex_UploadCluts(arg0);
    n = *(s32 *)arg0;
    while (n != 0) {
        texture_upload_anim(t, (s32)base, (AnimTexSet *)arg0);
        if (t->flags & 2) {
            n -= t->frames;
            t += t->frames;
        } else {
            t++;
            n--;
        }
    }
    p = *(u8 **)(arg0 + 0x18);
    if (p != NULL && *(s32 *)(p + 4) != 0) {
        Tex_UploadList((TexUploadList *)(p + 4 + *(s32 *)(p + 4)), arg0);
    }
}

static inline void func_8002AA50_i(u8 *arg0) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(arg0) + 32));
    if (temp_a0 != 0) {
        vram_free_node(temp_a0);
        (*(s32*)((u8*)(arg0) + 32)) = 0;
    }
}

static inline void func_8002A740_i(u8 *arg0) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(arg0) + 4));
    if (temp_a0 != 0) {
        vram_release(temp_a0);
        (*(s32*)((u8*)(arg0) + 4)) = 0;
    }
}

static inline void func_80029790_a(u8 *arg0) {
    s32 n;
    u8 *p;

    n = *(s32 *)(arg0 + 0);
    p = *(u8 **)(arg0 + 8);
    for (;; p += 0x38) {
        n--;
        if (n == -1) {
            return;
        }
        func_8002AA50_i(p);
    }
}

static inline void func_80029790_b(u8 *arg0, u8 *p) {
    s32 n;

    n = *(s32 *)(arg0 + 4);
    p -= 0xC;
    for (;;) {
        n--;
        p += 0xC;
        if (n == -1) {
            return;
        }
        func_8002A740_i(p);
    }
}

void textureset_release(u8 *arg0) {
    s32 n;
    u8 *d;
    s32 *q;
    s32 *r;
    u8 *b;

    b = *(u8 **)(arg0 + 0xC);
    func_80029790_a(arg0);
    func_80029790_b(arg0, b);
    d = *(u8 **)(arg0 + 0x18);
    if (d != NULL) {
        if (*(s32 *)(d + 4) != 0) {
            q = (s32 *)(d + (*(s32 *)(d + 4) + 4));
            r = q + 1;
            n = *q;
            if (--n != -1) {
                do {
                    vram_free_node(r[3]);
                    r += 4;
                } while (--n != -1);
            }
        }
        mal_free(*(void **)(arg0 + 0x18), &MainHeap);
        *(s32 *)(arg0 + 0x18) = 0;
    }
    if (*(void **)(arg0 + 8) != NULL) {
        mal_free(*(void **)(arg0 + 8), &MainHeap);
    }
    *(s32 *)(arg0 + 8) = 0;
    if (*(void **)(arg0 + 0xC) != NULL) {
        mal_free(*(void **)(arg0 + 0xC), &MainHeap);
    }
    *(s32 *)(arg0 + 0xC) = 0;
    if (*(void **)(arg0 + 0x20) != NULL) {
        mal_free(*(void **)(arg0 + 0x20), &MainHeap);
    }
    *(s32 *)(arg0 + 0x20) = 0;
    *(s32 *)(arg0 + 0x1C) = 0;
    if (*(void **)(arg0 + 0x10) != NULL) {
        mal_free(*(void **)(arg0 + 0x10), &MainHeap);
        *(s32 *)(arg0 + 0x10) = 0;
    }
}

s32 texture_draw_quad(obj_texture *f, s32 xy, s32 otz, u32 c0, u32 c1, u32 c2, u32 c3) {
    POLY_GT4 *p;
    GfxState *gs;
    ScreenXY v;

    if (ScreenRect == NULL) {
        return 0;
    }
    v.s.x = (s16)xy * ScreenRect->w / 640;
    p = *(POLY_GT4 **)(cdb + 0x4008);
    *(POLY_GT4 **)(cdb + 0x4008) = p + 1;
    v.s.y = (s16)(xy >> 16) / 2;
    gs = &RenderState;
    *(u32 *)&p->r0 = FadeColorGs(gs, (c0 >> 16) & 0xFF, (c0 >> 8) & 0xFF, c0 & 0xFF);
    *(u32 *)&p->r1 = FadeColorGs(gs, (c1 >> 16) & 0xFF, (c1 >> 8) & 0xFF, c1 & 0xFF);
    *(u32 *)&p->r2 = FadeColorGs(gs, (c2 >> 16) & 0xFF, (c2 >> 8) & 0xFF, c2 & 0xFF);
    *(u32 *)&p->r3 = FadeColorGs(gs, (c3 >> 16) & 0xFF, (c3 >> 8) & 0xFF, c3 & 0xFF);
    setPolyGT4(p);
    setSemiTrans(p, (s32)c0 < 0);
    *(u16 *)&p->u0 = f->uv0;
    *(u16 *)&p->u1 = f->uv1;
    *(u16 *)&p->u2 = f->uv2;
    *(u16 *)&p->u3 = f->uv3;
    p->tpage = (f->tpage & 0xFF9F) | ((c0 >> 19) & 0x60);
    p->clut = f->clut;
    *(s32 *)&p->x0 = v.w;
    v.s.x += f->w - 1;
    *(s32 *)&p->x1 = v.w;
    v.s.y += f->h - 1;
    *(s32 *)&p->x3 = v.w;
    v.s.x -= f->w - 1;
    *(s32 *)&p->x2 = v.w;
    addPrim(RenderState.ot + otz, p);
    return f->advance * 640 / ScreenRect->w;
}

s32 texture_draw(obj_texture *f, s32 xy, s32 otz, u32 color) {
    POLY_FT4 *p;
    ScreenXY v;

    if (ScreenRect == NULL) {
        return 0;
    }
    v.s.x = (s16)xy * ScreenRect->w / 640;
    p = *(POLY_FT4 **)(cdb + 0x4008);
    *(POLY_FT4 **)(cdb + 0x4008) = p + 1;
    v.s.y = (s16)(xy >> 16) / 2;
    *(u32 *)&p->r0 = FadeColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    setPolyFT4(p);
    setSemiTrans(p, (s32)color < 0);
    *(u16 *)&p->u0 = f->uv0;
    *(u16 *)&p->u1 = f->uv1;
    *(u16 *)&p->u2 = f->uv2;
    *(u16 *)&p->u3 = f->uv3;
    p->tpage = (f->tpage & 0xFF9F) | ((color >> 19) & 0x60);
    p->clut = f->clut;
    *(s32 *)&p->x0 = v.w;
    v.s.x += f->w - 1;
    *(s32 *)&p->x1 = v.w;
    v.s.y += f->h - 1;
    *(s32 *)&p->x3 = v.w;
    v.s.x -= f->w - 1;
    *(s32 *)&p->x2 = v.w;
    addPrim(RenderState.ot + otz, p);
    return f->advance * 640 / ScreenRect->w;
}

s32 texture_draw_3d(obj_texture *f, SWORD_XYZ *pos, s32 otz, u32 c0, u32 c1) {
    POLY_GT4 *p;
    GfxState *gs;
    volatile SVECTOR v;
    s32 t;

    if (ScreenRect == NULL) {
        return 0;
    }
    p = *(POLY_GT4 **)(cdb + 0x4008);
    *(POLY_GT4 **)(cdb + 0x4008) = p + 1;
    gs = &RenderState;
    t = FadeColorGs(gs, (c0 >> 16) & 0xFF, (c0 >> 8) & 0xFF, c0 & 0xFF);
    *(u32 *)&p->r0 = t;
    *(u32 *)&p->r1 = t;
    t = FadeColorGs(gs, (c1 >> 16) & 0xFF, (c1 >> 8) & 0xFF, c1 & 0xFF);
    setlen(p, 12);
    *(u32 *)&p->r2 = t;
    *(u32 *)&p->r3 = t;
    setcode(p, 0x3C);
    *(u16 *)&p->u0 = f->uv0;
    *(u16 *)&p->u1 = f->uv1;
    *(u16 *)&p->u2 = f->uv2;
    *(u16 *)&p->u3 = f->uv3;
    p->tpage = (f->tpage & 0xFF9F) | ((c0 >> 19) & 0x60);
    p->clut = f->clut;
    setSemiTrans(p, (s32)c0 < 0);
    v.vx = pos->x - 0x80;
    v.vy = pos->y - 0x80;
    v.vz = 0;
    gte_ldv0((SVECTOR *)&v);
    gte_rtps();
    v.vx += 0x100;
    gte_ldv0((SVECTOR *)&v);
    gte_rtps();
    v.vx -= 0x100;
    v.vy += 0x100;
    gte_ldv0((SVECTOR *)&v);
    gte_rtps();
    v.vx += 0x100;
    gte_stsxy3_gt3(p);
    gte_ldv0((SVECTOR *)&v);
    gte_rtps();
    gte_stsxy(&p->x3);
    setSemiTrans(p, (s32)c0 < 0);
    addPrim((u32 *)cdb + otz, p);
    return f->advance * 640 / ScreenRect->w;
}

void texture_unused_b(void) {
}

void texture_unused_c(void) {
}

s32 calc_rgbcode(s32 r, s32 g, s32 b) {
    s32 t;
    s32 d;

    d = RenderState.modelDp;
    if (d != 0) {
        t = 0x1000 - d;
        r *= t;
        g *= t;
        b *= t;
        r >>= 12;
        r &= 0xFF;
        g >>= 12;
        g &= 0xFF;
        b >>= 12;
        b &= 0xFF;
    }
    return (r << 16) | (g << 8) | b;
}

void texture_release_loading_mem(obj_textureset *set) {
    u8 *base;
    u8 *anims;

    if (set->anims != NULL) {
        base = set->base;
        anims = mal_trim_front(base, *(s32 *)(base + 0x18), &MainHeap);
        set->base = anims;
        mal_set_owner(anims, (u8 **)&set->anims);
        set->base = NULL;
        return;
    }
    mal_free_later(set->base, &MainHeap, 2);
    set->base = NULL;
}

void textureset_upload_cluts(u8 *arg0) {
    RECT rect;
    ClutUpload *e;
    u8 *base;
    s32 n;

    e = *(ClutUpload **)(arg0 + 0xC);
    base = *(u8 **)(arg0 + 0x10);
    n = *(s32 *)(arg0 + 4);
    for (;;) {
        n--;
        if (n == -1) {
            return;
        }
        if (e->w < 0x11U) {
            e->pos = clut_alloc_small();
        } else {
            e->pos = clut_alloc_large();
        }
        rect.x = e->pos->x;
        rect.y = e->pos->y;
        rect.w = e->w;
        rect.h = 1;
        LoadImage(&rect, (u_long *)(base + e->off));
        e->clut = GetClut(rect.x, rect.y);
        e++;
    }
}

void texture_unused_d(void) {
}

void texture_upload_list(TexUploadList *arg0, u8 *arg1) {
    s32 n;
    u8 *rec;
    s32 *ip;
    FreeHdr **pp;
    s32 base;
    RECT rect;

    ip = &arg0->e[0].idx;
    pp = &arg0->e[0].pos;
    n = arg0->count;
loop:
    n--;
    if (n != -1) {
        rec = *(u8 **)(arg1 + 8) + *ip * 0x38;
        ip += 4;
        *pp = vram_take(*(u8 **)(rec + 4));
        rect.w = *(u16 *)(rec + 0x1C);
        rect.h = *(u16 *)(rec + 0x1E);
        rect.x = (*pp)->f0;
        rect.y = (*pp)->f2;
        pp += 4;
        base = *(s32 *)(arg1 + 0x10);
        LoadImage(&rect, (u_long *)(*(s32 *)(rec + 0x30) + base));
        goto loop;
    }
}

void textureset_init(u8 *a0, u8 *arg1) {
    Func2A5A4Obj *arg0 = (Func2A5A4Obj *)a0;
    arg0->src = arg1;
    mal_alloc_owned_low(*(u16 *)(arg1 + 8) * 0x38, (void **)&arg0->f8, (s32)&MainHeap, 0);
    mal_alloc_owned_low(*(u16 *)(arg0->src + 0xA) * 0xC, (void **)&arg0->fC, (s32)&MainHeap, 0);
    arg0->f0 = *(u16 *)(arg0->src + 8);
    arg0->f4 = *(u16 *)(arg0->src + 0xA);
    arg0->f14 = *(s32 *)(arg0->src + 0x14);
    texture_init_mem_textureset((obj_textureset *)arg0);
}

/* Size in bytes: 0xB0 plus +8 records of 0x38 and +0xA records of 0xC. */
s32 textureset_mem_size(u8 *arg0) {
    s32 size;

    size = *(u16 *)(arg0 + 8) * 0x38 + 0xB0;
    size += *(u16 *)(arg0 + 0xA) * 0xC;
    return size;
}

void texture_unused_e(void) {
}

void texture_scroll_step(u8 *arg0, u8 *arg1) {
    s32 old;
    s32 pos;
    s32 step;
    s32 idx;
    s32 *p;
    u8 *rect;

    rect = *(u8 **)(arg1 + 8) + *(s32 *)(arg0 + 4) * 0x38;
    old = *(s32 *)(arg0 + 0x10) >> 8;
    pos = *(s32 *)(arg0 + 0x10) + *(s32 *)(arg0 + 0xC);
    *(s32 *)(arg0 + 0x10) = pos;
    if (*(s32 *)(arg0 + 0xC) < 0) {
        if (pos < 0) {
            *(s32 *)(arg0 + 0x10) = pos + (*(s32 *)(arg0 + 8) << 8);
        }
    } else {
        step = *(s32 *)(arg0 + 8);
        if ((pos >> 8) >= step) {
            *(s32 *)(arg0 + 0x10) = pos - (step << 8);
        }
    }
    idx = *(s32 *)(arg0 + 0x10) >> 8;
    if (idx != old) {
        p = (s32 *)(arg0 + *(s32 *)arg0 + idx * 4);
        LoadImage((RECT *)(rect + 0x18), (u_long *)((u8 *)p + *p));
    }
}

void clut_release(u8 *arg0) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(arg0) + 4));
    if (temp_a0 != 0) {
        vram_release(temp_a0);
        (*(s32*)((u8*)(arg0) + 4)) = 0;
    }
}

void clut_upload(ClutUpload *arg0, s32 arg1) {
    RECT rect;

    if (arg0->w < 0x11U) {
        arg0->pos = clut_alloc_small();
    } else {
        arg0->pos = clut_alloc_large();
    }
    rect.x = arg0->pos->x;
    rect.y = arg0->pos->y;
    rect.w = arg0->w;
    rect.h = 1;
    LoadImage(&rect, (u_long *)(arg1 + arg0->off));
    arg0->clut = GetClut(rect.x, rect.y);
}

void texsrc_init(u8 *src, u8 *out, s32 len) {
    u16 id;

    id = *(u16 *)src;
    src -= len - 4;
    *(s32 *)(out + 4) = 0;
    *(u8 **)(out + 8) = src;
    *(u16 *)out = id;
}

void texture_init_entry(TexSrc2A84C *s, TexDst2A84C *d, s32 n) {
    u32 t;
    s32 u;

    d->f20 = 0;
    d->f34 = -1;
    d->flag = *(u16 *)&s->wC & 1;
    d->f2 = (s->wC >> 1) & 0x7FFF;
    d->f12 = *(u16 *)((u8 *)s + 0xE);
    d->f14 = s->w10;
    d->fA = s->w2;
    if (d->flag == 0) {
        d->f8 = s->w0 << 2;
        d->slot = &D_80063B1C[vram_slot_class(s->w0 << 2, s->w2, 0x10)];
        d->f10 = (*(u8 *)s << 2) - 1;
    } else {
        d->f8 = s->w0 << 1;
        d->slot = &D_80063B1C[vram_slot_class(s->w0 << 1, s->w2, 0x100)];
        d->f10 = (*(u8 *)s << 1) - 1;
    }
    d->f11 = *(u8 *)((u8 *)s + 2) - 1;
    d->f1C = s->w0;
    d->f1E = s->w2;
    u = n - 0x14;
    t = s->w8;
    d->f30 = (s32)s - u;
    d->fC = t;
}

void texanims_release(s32 *arg0, s32 arg1) {
    s32 n;
    TexAnimEntry *p;

    p = (TexAnimEntry *)arg0[2];
    n = arg0[0];
    if (p != NULL) {
        for (;; p++) {
            n--;
            if (n == -1) {
                return;
            }
            if (p->handle != 0 && FrameCount - p->time >= arg1) {
                vram_free_node(p->handle);
                p->handle = 0;
            }
        }
    }
}

void texture_release_vram(u8 *arg0) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(arg0) + 32));
    if (temp_a0 != 0) {
        vram_free_node(temp_a0);
        (*(s32*)((u8*)(arg0) + 32)) = 0;
    }
}

void textureset_clear(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 0)) = 0;
    (*(s32*)((u8*)(arg0) + 8)) = 0;
    (*(s32*)((u8*)(arg0) + 12)) = 0;
    (*(s32*)((u8*)(arg0) + 16)) = 0;
}

/* data, in address order (tools/scripts/data2c.py) */
u16 D_8005A5C0[4][3] = {
    { 0x3027, 0x3028, 0x3029 }, { 0x302A, 0x302B, 0x302C }, { 0x302A, 0x302B, 0x302C },
    { 0x302A, 0x302B, 0x302C },
};
s32 World_floor_level = 0;
s32 VelocityCalc = 0;
void *D_8005A5E0 = 0;
s32 D_8005A5E4 = 0;
EngineRec32 *D_8005A5E8 = 0;
s32 D_8005A5EC[6] = {0};
s8 GameOptions[4] = {0};
s32 D_8005A608 = 0;
s32 D_8005A60C = 0;
s32 D_8005A610[1] = {0};
u8 GameInfo[4] = {0};
s32 GameCheats = 0;
s32 D_8005A61C = 0;
s32 D_8005A620 = 0;
s32 GamePause = 0;
s32 D_8005A628[4] = {0};
s16 D_8005A638 = 0;
u8 GamePlayers = 0;
u8 D_8005A63B = 0;
s8 D_8005A63C = 0;
u8 D_8005A63D = 0;
u8 D_8005A63E = 0;
u8 D_8005A63F = 0;
s32 GameLanguage = 0;
u16 GameExitFlags = 0;
u16 GameLevelFlags = 0;
s16 GameMode = 0;
s8 GameArena = 0;
s8 D_8005A64B = 0;
s16 D_8005A64C[2] = {0};
s16 D_8005A650 = 0;
s16 D_8005A652 = 0;
s16 D_8005A654[4] = {0};
s16 GameTeamLeader[2] = {0};
s32 D_8005A660 = 0;
u8 D_8005A664 = 0;
u8 D_8005A665 = 0;
u8 D_8005A666 = 0;
u8 D_8005A667 = 0;
s32 D_8005A668[2] = {0};
u8 GameHeroOut[4] = {0};
u8 GameWinner[2] = {0};
u8 GameWarpRoom = 0;
s8 GameGateway = 0;
s16 D_8005A678[4] = {0};
s32 GameTeams[1] = {0};
AdventureState GameAdventure = {0};
s32 D_8005A7A0[3] = {0};
ExtBlk *D_8005A7AC = 0;
s32 D_8005A7B0[4] = {0};
NAS NASTemplate = {0};
subengine SubEngineTemplate = {0};
EXTengine EngineTemplate = {0};
Ext1000 ExtTemplate1000 = {0};
Ext400 ExtTemplate400 = {0};
EXTmotion ExtMotionTemplate = {0};
s32 D_8005AA00[1] = {0};
NAS *NASActiveHead = 0;
NAS *NASActiveTail = 0;
NAS *NASFreeList = 0;
u16 FirstNas = 0;
u16 D_8005AA12 = 0;
u16 NasPoolUsed = 0;
s16 D_8005AA18[2] = {0};
NAS *NasPool = 0;
u8 *D_8005AA20 = 0;
void *HeadOfObjects = 0;
void *HeadOfMissiles = 0;
NAS *HeadOfSolids = 0;
void *D_8005AA30 = 0;
void *HeadOfCameras = 0;
void *HeadOfBalls = 0;
s32 D_8005AA3C[3] = {0};
NASTrigger *HeadOfTriggers = 0;
s32 TriggerCount = 0;
s32 D_8005AA50 = 0;
s32 D_8005AA54 = 0;
s32 GlobalNAStatus = 0;
s32 D_8005AA5C[1] = {0};
s32 ASTCount = 0;
s32 D_8005AA64[2] = {0};
ASTEntry *ASTTable = 0;
LevSpec LevelSpecs = {0};
NAS *ContourPlatform = 0;
s32 D_8005AAA0[5] = {0};
s16 SysFontLookUp[512] = {0};
s8 SysFontWidth[512] = {0};
u8 SysFontFlags[512] = {0};
s32 FontPage = 0;
MapPoint MapPoints[32] = {0};
s32 ContourHeight = 0;
s32 ACMD[16][2] = {0};
u8 *sysfont_dbase = 0;
RECT D_8005B640 = { 0, 0, 0x1FF, 0x1FF };
ModuleEntry *CurrentModule = 0;
s32 D_8005B64C[16] = {
    0, 0x200, 0x400, 0x580, 0x680, 0x720, 0x780, 0x7B8, 0, 0x80, 0x100, 0x160, 0x1A0, 0x1C8,
    0x1E0, 0x1EE,
};
u8 *cdb = (void *)&DrawBuffers;
s32 FrameCount = 0;
s32 D_8005B694 = 0;
RECT *ScreenRect = 0;
u8 D_8005B69C[144] = {
    0, 0, 0, 0, 0, 1, 0xF0, 0, 0, 0, 0, 1, 0, 1, 0xF0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x40, 1, 0xF0, 0,
    0, 0, 0, 1, 0x40, 1, 0xF0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x40, 1, 0, 1, 0, 0, 0,
    1, 0x40, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 1, 0xF0, 0, 0, 0, 0, 1, 0x80,
    1, 0xF0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 1, 0, 1, 0, 0, 0, 1, 0x80, 1, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
};
u8 vmode_512_ntsc[96] = {
    0, 0, 0, 0, 0, 2, 0xF0, 0, 0, 0, 0, 1, 0, 2, 0xF0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 1, 0, 0, 0, 1, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 2, 0xF0, 0,
    0, 0, 0, 1, 0x80, 2, 0xF0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 2, 0, 1, 0, 0, 0,
    1, 0x80, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_8005B78C = 0;
u8 DrawBuffers[32792] = {0};
s32 VideoMode = 0;
s32 D_800637AC[1] = {0};
s32 D_800637B0 = 0;
s32 CdReadStatus = 0;
s32 D_800637B8 = 0;
s32 D_800637BC[176] = {
    0x13F0199, 0x16D0128, 0x1CE0136, 0x18A0178, 0x10801A2, 0xFE00C5, 0xF100E4, 0x12B010C,
    0x10B00F1, 0xE000E2, 0x10300FB, 0x12E0126, 0x1610115, 0x12D0124, 0x1400138, 0x12B0112,
    0x144013D, 0x16D013D, 0x1440146, 0x16B0179, 0x1E7011E, 0x16A0189, 0x19E0162, 0x11E010B,
    0x1510150, 0x11501A0, 0x1870169, 0x10D0188, 0xE50106, 0xE200EA, 0xF400CF, 0x169010E,
    0x1390114, 0x1670132, 0x14400D8, 0x1AC00FF, 0x102015A, 0x14800D6, 0x1360136, 0xFC011C,
    0x16800BF, 0x1200151, 0xC7013E, 0x13500EF, 0x1140133, 0x1150117, 0x14D0193, 0x14D01A2,
    0x12A0101, 0x12C00E6, 0x11800F9, 0x1330184, 0x1450125, 0x123012B, 0x1180104, 0x19C0112,
    0x1370115, 0x1150102, 0x10600E9, 0x178013D, 0xF400FD, 0xFE0130, 0x1210121, 0x11D0147,
    0x191011B, 0xC9014A, 0x158010B, 0x18900CE, 0x168008A, 0xB800D4, 0xE500F8, 0xF400F4,
    0x11B00F4, 0xED00FD, 0x13C00FE, 0xDD013F, 0x14200F7, 0x19801D5, 0x1AD01CD, 0x1910103,
    0x1F401CE, 0x1D10192, 0x15F01A8, 0xF400E8, 0x130011C, 0x1500136, 0x11B00FC, 0x10C00F3,
    0x12600FD, 0x1AC012C, 0x1740193, 0x18C0170, 0x1560153, 0x1520158, 0x13E0114, 0x10F016A,
    0x17A0207, 0x1800178, 0xCB00C3, 0x17F00D3, 0xF300F1, 0xFD00F0, 0xFC00F5, 0xF800D9,
    0xD30103, 0x780078, 0x780078, 0x10600E9, 0xF80101, 0xF500F3, 0x13000EE, 0x12600FB,
    0xE50103, 0x10200EB, 0xD400C3, 0x1130105, 0xDA00E1, 0xD300FC, 0xE800E0, 0x10B011F,
    0x1230108, 0xDB00ED, 0xE9011D, 0x1030117, 0xC300C2, 0xEA0137, 0xFC00F2, 0x10600DA,
    0xC3010E, 0xC600CD, 0xD400D2, 0xFD00D7, 0x1120129, 0xFB0109, 0xEA0136, 0x11E010F,
    0x32E00D7, 0x225021D, 0x1FA0220, 0x1F6021A, 0x266025A, 0x22B0235, 0x2260273, 0x26A0280,
    0x22301BA, 0x2F802B3, 0x216033E, 0x15C00D2, 0x112010B, 0xF0011E, 0x1E10220, 0xFE01AD,
    0x1A60158, 0x145011B, 0x138012D, 0x18301BC, 0xFB0179, 0x11700CF, 0xC8011D, 0x11700F1,
    0xFC0100, 0xF10124, 0xDA00F3, 0x123010A, 0x137010E, 0x1390102, 0xF900EF, 0xEE010F,
    0x10E010B, 0x12700ED, 0x13000EC, 0x2830087, 0x2D00223, 0xD300D2, 0xE4, 0,
};
s32 xa_count = 0;
s32 D_80063A80[1] = {0};
u8 D_80063A84[8] = { 0, 1, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 };
s32 D_80063A8C = 0;
u8 D_80063A90[2][52] = {0};
u32 D_80063AF8[8] = {
    0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000,
    0xFFFF0000,
};
FreeNode *FreeBufNodes = 0;
Slot12 D_80063B1C[3] = {
    { { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0 } },
    { { 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0 } },
};
s32 D_80063B40[6] = { 0, 0, 0x10, 0, 8, 0 };
u8 D_80063B58[24] = { 0, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0, 0, 0, 0, 0, 0 };
s32 D_80063B70[6] = { 0, 0, 0x40, 0, 0x20, 0 };
s32 D_80063B88[6] = { 0, 0, 0x80, 0, 0x40, 0 };
u8 D_80063BA0[12] = { 0, 0, 0, 0, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0, 0 };
FreeNode *D_80063BAC = 0;
FreeNode *D_80063BB0 = 0;
void *D_80063BB4 = 0;
u8 VramNodePool[2096] = {
    0, 3, 0x60, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x61, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x62, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x63, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0x65, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0x66, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x67, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x68, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x69, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x6A, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0x6B, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0x6C, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x6D, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x6E, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x6F, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x70, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0x71, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0x72, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x73, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x74, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x75, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x76, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0x77, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0x78, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x79, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x7A, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x7B, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x7C, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0x7D, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0x7E, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0x7F, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xA0, 0x3B, 6, 0x80, 0x80, 1,
    0, 0, 0xA0, 0x3B, 6, 0x80, 0x80, 1, 0, 1, 0xA0, 0x3B, 6, 0x80, 0, 2, 0, 0, 0xA0, 0x3B, 6,
    0x80, 0, 2, 0, 1, 0xA0, 0x3B, 6, 0x80, 0x80, 2, 0, 0, 0xA0, 0x3B, 6, 0x80, 0x80, 2, 0, 1,
    0xA0, 0x3B, 6, 0x80, 0, 3, 0, 0, 0xA0, 0x3B, 6, 0x80, 0x80, 3, 0, 0, 0x88, 0x3B, 6, 0x80,
    0, 3, 0x80, 1, 0x88, 0x3B, 6, 0x80, 0x40, 3, 0x80, 1, 0x88, 0x3B, 6, 0x80, 0x80, 3, 0x80,
    1, 0x88, 0x3B, 6, 0x80, 0xC0, 3, 0x80, 1, 0x70, 0x3B, 6, 0x80, 0, 3, 0, 1, 0x70, 0x3B, 6,
    0x80, 0x20, 3, 0, 1, 0x70, 0x3B, 6, 0x80, 0x40, 3, 0, 1, 0x70, 0x3B, 6, 0x80, 0x60, 3, 0,
    1, 0x70, 0x3B, 6, 0x80, 0xA0, 3, 0, 1, 0x70, 0x3B, 6, 0x80, 0xC0, 3, 0, 1, 0x70, 0x3B, 6,
    0x80, 0xE0, 3, 0, 1, 0x58, 0x3B, 6, 0x80, 0x80, 3, 0x20, 1, 0x58, 0x3B, 6, 0x80, 0x90, 3,
    0x20, 1, 0x58, 0x3B, 6, 0x80, 0, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0x10, 3, 0x40, 1, 0x58,
    0x3B, 6, 0x80, 0x20, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0x30, 3, 0x40, 1, 0x58, 0x3B, 6,
    0x80, 0x40, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0x50, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0x60,
    3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0x70, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0x80, 3, 0x40, 1,
    0x58, 0x3B, 6, 0x80, 0x90, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0xA0, 3, 0x40, 1, 0x58, 0x3B,
    6, 0x80, 0xB0, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0xC0, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80,
    0xD0, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0xE0, 3, 0x40, 1, 0x58, 0x3B, 6, 0x80, 0xF0, 3,
    0x40, 1, 0x40, 0x3B, 6, 0x80, 0, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 8, 0, 0xF0, 0, 0x40,
    0x3B, 6, 0x80, 0x10, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x18, 0, 0xF0, 0, 0x40, 0x3B, 6,
    0x80, 0x20, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x28, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x30,
    0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x38, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x40, 0, 0xF0, 0,
    0x40, 0x3B, 6, 0x80, 0x48, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x50, 0, 0xF0, 0, 0x40, 0x3B,
    6, 0x80, 0x58, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x60, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80,
    0x68, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x70, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x78, 0,
    0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x80, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x88, 0, 0xF0, 0,
    0x40, 0x3B, 6, 0x80, 0x90, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x98, 0, 0xF0, 0, 0x40, 0x3B,
    6, 0x80, 0xA0, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xA8, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80,
    0xB0, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xB8, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xC0, 0,
    0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xC8, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xD0, 0, 0xF0, 0,
    0x40, 0x3B, 6, 0x80, 0xD8, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xE0, 0, 0xF0, 0, 0x40, 0x3B,
    6, 0x80, 0xE8, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xF0, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80,
    0xF8, 0, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 8, 1, 0xF0, 0,
    0x40, 0x3B, 6, 0x80, 0x10, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x18, 1, 0xF0, 0, 0x40, 0x3B,
    6, 0x80, 0x20, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x28, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80,
    0x30, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x38, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x40, 1,
    0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x48, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x50, 1, 0xF0, 0,
    0x40, 0x3B, 6, 0x80, 0x58, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x60, 1, 0xF0, 0, 0x40, 0x3B,
    6, 0x80, 0x68, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x70, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80,
    0x78, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x80, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x88, 1,
    0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x90, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0x98, 1, 0xF0, 0,
    0x40, 0x3B, 6, 0x80, 0xA0, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xA8, 1, 0xF0, 0, 0x40, 0x3B,
    6, 0x80, 0xB0, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xB8, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80,
    0xC0, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xC8, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xD0, 1,
    0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xD8, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xE0, 1, 0xF0, 0,
    0x40, 0x3B, 6, 0x80, 0xE8, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0xF0, 1, 0xF0, 0, 0x40, 0x3B,
    6, 0x80, 0xF8, 1, 0xF0, 0, 0x40, 0x3B, 6, 0x80, 0, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 8, 0,
    0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x10, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x18, 0, 0xF0, 1,
    0x40, 0x3B, 6, 0x80, 0x20, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x28, 0, 0xF0, 1, 0x40, 0x3B,
    6, 0x80, 0x30, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x38, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80,
    0x40, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x48, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x50, 0,
    0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x58, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x60, 0, 0xF0, 1,
    0x40, 0x3B, 6, 0x80, 0x68, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x70, 0, 0xF0, 1, 0x40, 0x3B,
    6, 0x80, 0x78, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x80, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80,
    0x88, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x90, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x98, 0,
    0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xA0, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xA8, 0, 0xF0, 1,
    0x40, 0x3B, 6, 0x80, 0xB0, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xB8, 0, 0xF0, 1, 0x40, 0x3B,
    6, 0x80, 0xC0, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xC8, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80,
    0xD0, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xD8, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xE0, 0,
    0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xE8, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xF0, 0, 0xF0, 1,
    0x40, 0x3B, 6, 0x80, 0xF8, 0, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0, 1, 0xF0, 1, 0x40, 0x3B, 6,
    0x80, 8, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x10, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x18, 1,
    0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x20, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x28, 1, 0xF0, 1,
    0x40, 0x3B, 6, 0x80, 0x30, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x38, 1, 0xF0, 1, 0x40, 0x3B,
    6, 0x80, 0x40, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x48, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80,
    0x50, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x58, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x60, 1,
    0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x68, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x70, 1, 0xF0, 1,
    0x40, 0x3B, 6, 0x80, 0x78, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x80, 1, 0xF0, 1, 0x40, 0x3B,
    6, 0x80, 0x88, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0x90, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80,
    0x98, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xA0, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xA8, 1,
    0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xB0, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xB8, 1, 0xF0, 1,
    0x40, 0x3B, 6, 0x80, 0xC0, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xC8, 1, 0xF0, 1, 0x40, 0x3B,
    6, 0x80, 0xD0, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xD8, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80,
    0xE0, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xE8, 1, 0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xF0, 1,
    0xF0, 1, 0x40, 0x3B, 6, 0x80, 0xF8, 1, 0xF0, 1, 0x88, 0x3B, 6, 0x80, 0xFF, 0xFF, 0xFF,
    0xFF,
};
s32 D_800643E8 = 0;
s32 D_800643EC = 0;
s32 D_800643F0 = 0;
u8 *D_800643F4 = 0;
void (*D_800643F8)() = 0x4D2;
s32 D_800643FC[6] = {0};

/* the .bss (main.c's comes first) */
s32 D_8006EA00 __attribute__((section(".bss")));
u32 D_8006EA04 __attribute__((section(".bss")));
u8 D_8006EA08[8] __attribute__((section(".bss")));   /* CdControl's result */
CdlLOC D_8006EA10 __attribute__((section(".bss")));
CdlLOC D_8006EA14 __attribute__((section(".bss")));
