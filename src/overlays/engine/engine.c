#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/hero.h"
#include "bash/shell.h"

void func_80078CA4(void) {
    EngineRec32 *rec;
    EngineBootView *v;
    u8 *p;

    rec = ((EngineBootSrc *)D_8005A5E0)->unk4;
    D_8005A5E8 = rec;
    if (rec != NULL) {
        EngineBootView *w = (EngineBootView *)D_8005A614;
        w->unk3C = *(u16 *)&rec[w->unk8];
    }
    v = (EngineBootView *)D_8005A614;
    if (v->unk0 != NULL) {
        v->unk0(rec);
    }
    func_800237BC();
    func_8007EC48();
    D_8005AA1C = (NAS *)func_80011654(D_8005AA14 * 0x70, &D_8004E0F0);
    D_8005AA20 = (u8 *)D_8005AA1C + (D_8005AA14 * 0x70 - 0x70);
    D_8005AA40 = (EngObj **)func_80011654(D_8005AA14 * 4, &D_8004E0F0);
    *D_8005AA40 = NULL;
    p = func_80011654(D_8005AA14 * 4, &D_8004E0F0);
    D_8005AA44 = p;
    *(s32 *)p = 0;
    func_80026308();
    D_8005AAB0 = D_800986A4[v->unk2C];
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80078DD4);

void func_8007906C(void) {
    s32 i;
    s32 n;
    NAS *nas;
    u16 cls;

    D_8005AA58 = 1;
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
    for (i = D_8005AA10; i < D_8005AA12; i++) {
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
    D_8005AA58 = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80079260);

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

    switch (D_8004DDD0[D_8009E5DC * 3]) {
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
    g = D_8005A614;
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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80079554);

void func_8007976C(void) {
    s32 i;
    EngObj *o;

    func_80011410(&D_8004E0F0);
    func_80079554();
    if (D_8005A618 & 0x10) {
        FntPrint("\t\tFree Memory  - tot %d  max %d\n\0\x18\0", D_8004E104, D_8004E10C);
    }
    func_8007906C();
    func_80083E8C();
    D_8005AA58 = 2;
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
    D_8005AA58 = 0;
    func_8007B8D4();
    func_8007AF3C();
    if (D_80099174 & 0x4000) {
        func_8007D6D0();
    }
    func_80081F64(1);
    func_8009608C(0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800798A4);

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

s32 func_8007A26C(NAS *nas) {
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
    eng->unk10.x -= m->Xvel * D_80068BD4[(eng->unk1E - 0x400) & 0xFFF].unk2 >> 12;
    eng->unk10.z -= m->Xvel * D_80068BD4[eng->unk1E & 0xFFF].unk2 >> 12;
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

    nas = SpawnNASById(arg1[0], arg1[1], arg1[2], 0x1605, 0x11);
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

    nas = SpawnNASById(arg1[0], arg1[1], arg1[2], 0x1605, 0x11);
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
    u8 *p = D_8005A614;
    func_8007A164(D_800986FC[arg0 & 0xFFFF], p);
}

void func_8007A650(s32 arg0) {
    func_8007A164(D_800986FC[arg0 & 0xFFFF]);
}

void func_8007A688(s32 arg0) {
    func_8007A164(arg0 & 0xFFFF, D_8005A614);
}

void func_8007A6B0(s32 arg0) {
    func_8007A164(arg0 & 0xFFFF);
}

void func_8007A6D0(s32 arg0) {
    func_8007A164(arg0 & 0xFFFF, D_8005A614);
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
    func_80011410(&D_8004E0F0);
    func_80079554();
    D_8005AA58 = 4;
    func_8007AF3C();
    D_8005AA58 = 0;
    func_80081F64(0);
}

void func_8007A808(void) {
    s32 i;
    s32 r;
    s32 want;

    D_8005AA58 = 0;
    ASTTable = EngineASTTable;
    ASTCount = func_8007AC00();
    func_80083708();
    func_80086918();
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
    EngineGameView *g = (EngineGameView *)D_8005A614;

    g->flags &= 0xF4FF;
    func_80083550();
    func_8008688C();
    if (D_8005AA70[0] != 0) {
        ((void (*)(void))D_8005AA70[0])();
    }
}

void func_8007A91C(void) {
    if (D_8009871C != 0) {
        D_8009871C = D_8009871C - 1;
    } else {
        u8 *p = D_80098718;
        p += Random(8) * 2;
        PlaySfx(*(u16*)(p + 8), 0, 0, 0x1000, 0x1E00, 0);
        D_8009871C = (*(u16*)((u8*)D_80098718) + Random(*(u16*)((u8*)D_80098718 + 2) - *(u16*)((u8*)D_80098718))) * 30;
    }
}

void func_8007A9D0(u8 *arg0) {
    D_80098718 = arg0;
    D_8009871C = 0;
    PlaySfx((*(u16*)((u8*)(arg0) + 4)), 0, 0, 0x1000, 0x1E00, 0);
}

void func_8007AA18(void) {
    s32 i;
    EngObj *o;

    D_8005AA58 = 2;
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
    D_8005AA58 = 0;
    func_8007B8D4();
    func_8007AF3C();
    if (D_80099174 & 0x4000) {
        func_8007D6D0();
    }
}

void func_8007AAF0(void) {
    D_8005AA58 = 4;
    func_8007AF3C();
    D_8005AA58 = 0;
}

void func_8007AB20(void) {
}

void func_8007AB28(void) {
    D_8005AA58 = 0;
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
    D_8005AA58 = 0;
    ASTTable = EngineASTTable;
    ASTCount = func_8007AC00();
}

void func_8007ABC4(void) {
    D_8005AA58 = 0;
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
