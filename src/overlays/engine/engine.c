#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/hero.h"
#include "bash/shell.h"

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80078CA4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80078DD4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007906C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80079260);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800793E8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80079554);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007976C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800798A4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A164);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A26C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A464);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A524);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A718);

void func_8007A7C0(void) {
    func_80011410(&D_8004E0F0);
    func_80079554();
    D_8005AA58 = 4;
    func_8007AF3C();
    D_8005AA58 = 0;
    func_80081F64(0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A808);

void func_8007A8C4(void) {
    EngineGameView *g = (EngineGameView *)D_8005A614;

    g->flags &= 0xF4FF;
    func_80083550();
    func_8008688C();
    if (D_8005AA70[0] != 0) {
        ((void (*)(void))D_8005AA70[0])();
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A91C);

void func_8007A9D0(u8 *arg0) {
    D_80098718 = arg0;
    D_8009871C = 0;
    PlaySfx((*(u16*)((u8*)(arg0) + 4)), 0, 0, 0x1000, 0x1E00, 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007AA18);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007AC20);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007AF3C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007B120);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007B330);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007B7FC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007B8D4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007BE84);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007BFA8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007C170);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007C338);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007C514);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007C6EC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007C830);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007CA70);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007CC80);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007CE50);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007CFAC);

void func_8007D034(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            D_8009AD74[i].a = 0;
            D_8009AD74[i].b = D_80099270[i].a;
            D_8009AD74[i].c = D_80099270[i].b;
            D_8009AD74[i].d = 1;
        } else {
            D_8009AD74[i].d = 0;
        }
    }
    D_80099174 |= 0x200;
}

void func_8007D0BC(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            D_8009AD74[i].a = 0;
            D_8009AD74[i].b = D_80099280[i].a;
            D_8009AD74[i].c = D_80099280[i].b;
            D_8009AD74[i].d = 1;
        } else {
            D_8009AD74[i].d = 0;
        }
    }
    D_80099174 |= 0x200;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D144);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D208);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D2F4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D4D0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D5EC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D6D0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D850);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADD3C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADD40);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D9DC);

void func_8007E7A4(void) {
    func_80010000(D_8005A614);
}

s32 func_8007E7C8(void) {
    s32 var_v0;

    var_v0 = D_8005A620 - D_8009BD04;
    if (var_v0 < 0) {
        var_v0 = 0;
    }
    return var_v0;
}

void func_8007E7F4(void) {
    u8 *p;
    u16 f;

    D_8009AE70 = 0;
    D_8009AE3C = 0;
    D_8009AE74 = 3;
    p = D_8005A614;
    f = *(u16 *)(p + 0x32);
    if (f & 3) {
        D_8009AE74 = *(u16 *)(p + 0x3A);
    }
    if (f & 0x20) {
        if (f & 8) {
            D_8009AE74 = 3;
        } else {
            D_8009AE74 = 2;
        }
    }
    if (D_8005A648 == 0x20) {
        D_8009AE74 = 3;
    }
}

void func_8007E874(void) {
    D_8009AE78 = 0xF0;
    D_80099174 |= 0x8000;
}

s32 func_8007E894(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_score != Heroes[i].hero_score_displayed) {
            return 1;
        }
    }
    return 0;
}

void func_8007E8D0(EngineBox *p, s16 x, s16 y, s32 a, s32 b) {
    p->unk54 = D_8001C7FC;
    p->unk6E = y;
    p->unk76 = y;
    p->unk74 = x + 0x40;
    p->unk84 = x + 0x40;
    p->unk6C = x;
    p->unk70 = 0;
    p->unk78 = 0;
    p->unk7C = x;
    p->unk7E = y + 0xC;
    p->unk80 = 0;
    p->unk86 = y + 0xC;
    p->unk88 = 0;
    p->unk8C = a;
    p->unk90 = a;
    p->unk0 = 0x12000000;
    p->unk68 = 8;
    p->unk94 = b;
    p->unk98 = b;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007E93C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007E9FC);

void func_8007EAC0(void) {
    D_8004E0D0 = 0;
    D_80099194 = 0;
    D_80099700[0] = 0x138;
    D_80099700[1] = 0x20;
    D_80099700[2] = 0;
    D_80099174 |= 0x20000;
}

void func_8007EB04(void) {
    D_80099174 &= ~1;
}

void func_8007EB1C(s32 arg0, s32 arg1, s32 arg2) {
    D_80099178 = 0;
    D_8009917C = 0;
    D_800996D4[0] = arg0 - 8;
    D_800996D4[1] = arg1 - 8;
    D_800996D4[2] = arg2;
    D_8009AE76 = 0x63;
    D_80099174 |= 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007EB68);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007EC48);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007ED0C);

s32 func_8007EE2C(s32 pad) {
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x80) && D_8009AEEC != 0) {
        D_8009AEEC = 0;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x20) && D_8009AEEC == 0) {
        D_8009AEEC = 1;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadButtons *)D_80051338)[pad].held & 9) != 9 &&
        (((PadButtons *)D_80051338)[pad].pressed & 8)) {
        s32 *p = (s32 *)D_8005A614;
        D_8009AEF0 = 0;
        D_8009AEEC = 0;
        p[4] &= 0x04000010;
        func_8002AC18();
        return 0;
    }
    return ((PadButtons *)D_80051338)[pad].pressed & 0x4000;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007EFC0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007F1A8);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADDB0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADDBC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADDCC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADDE0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADDF8);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADE10);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADE20);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADE38);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007F314);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800809A0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80080FBC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80081470);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80081F64);

void func_80082590(void) {
    GfxState *gs;
    u8 *e;

    if (D_8005A624 & 0x7000) {
        gs = &D_80056998;
        *(s32 *)&gs->unk28[0] = 0;
        *(s32 *)&gs->unk28[4] = 0;
        if (D_8005A624 & 0x4000) {
            e = D_8009BCE0;
            if (*(s16 *)(e + 0xC) != -1) {
                func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)D_8009F644;
                func_8001A6D4(e, 0x10000000);
            }
        }
    } else if (D_8005A624 & 1) {
        gs = &D_80056998;
        *(s32 *)&gs->unk28[0] = 0;
        *(s32 *)&gs->unk28[4] = 0;
        if (!(D_8005A624 & 0x40)) {
            e = D_8009BCE0;
            if (*(s16 *)(e + 0xC) != -1) {
                func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)D_8009F644;
                func_8001A6D4(e, 0x10000000);
            }
        }
        func_80018B08(&D_8009F6A4, &D_8009F7EC);
        func_8001C2C0((u8 *)GetEngineSlot(0));
        func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
        func_8001C2C0((u8 *)GetEngineSlot(4));
        func_8001C298(&D_8009959C);
        func_8001C298(&D_8009959C + 1);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800826F8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80082AC4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80082C34);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80082D90);

void func_800831C4(s32 arg0) {
    GetEngineSlot(0)->unk10.y = arg0;
}

s16 func_800831F0(void) {
    EngineRec24 *p = D_8009BD0C;

    if (p != NULL) {
        do {
            if (p->kind != 0) {
                if (p->kind == 1) {
                    return p->val;
                }
            } else {
                return 0;
            }
            p++;
        } while (p != NULL);
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80083244);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800832BC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80083334);

s32 func_80083488(void) {
    PadButtons *base = (PadButtons *)D_80051338;
    PadButtons *pad = &base[D_8009AEF0];
    if ((pad->held & 9) != 9) {
        if (pad->pressed & 0x4008) {
            return 1;
        }
    }
    return 0;
}

void func_800834E4(void) {
    s32 *p = (s32 *)D_8005A614;
    D_8009AEF0 = 0;
    D_8009AEEC = 0;
    p[4] &= 0x04000010;
    func_8002AC18();
}

void func_8008352C(void) {
    u8 *p = D_8005A614;
    s32 a = *(s32 *)(p + 0x10);
    u16 b = *(u16 *)(p + 0x30);
    *(s32 *)(p + 0x10) = a | 0x10;
    *(u16 *)(p + 0x30) = b | 0x8002;
}

void func_80083550(void) {
    D_8009D6E8.unk0 = 0;
    D_8009D6E8.unk4 = D_8009D2E4;
    D_8009D6E8.unkC = D_8009D318;
    D_8009D6E8.unk10 = D_8009D354;
    D_8009D6E8.unk14 = D_8009D2F8;
    D_8009D6E8.unk20 = func_800859D4;
    D_8009D6E8.unk24 = func_80085D2C;
    D_8009D6E8.unk2C = func_80085C84;
    D_8009D6E8.unk30 = func_8008AA40;
    D_8009D6E8.unk34 = func_8008A9F0;
    D_8009D6E8.unk38 = func_8008680C;
    D_8009D6E8.unk3C = func_8008AB48;
    D_8009D6E8.unk40 = func_8008AB48;
    D_8009D6E8.unk4C = func_80086724;
    D_8009D6E8.unk50 = func_8008671C;
    D_8009D6E8.unk54 = func_80086714;
    D_8009D6E8.unk64 = func_80084788;
    D_8009D6E8.unk60 = func_80086704;
    D_8009D6E8.unk8 = 0;
    D_8009D6E8.unk18 = 0;
    D_8009D6E8.unk1C = 0;
    D_8009D6E8.unk28 = 0;
    D_8009D6E8.unk44 = 0;
    D_8009D6E8.unk48 = 0;
    D_8009D6E8.unk5C = 0;
    D_8009D6E8.unk58 = 0;
    D_8009D6E8.unk68 = func_8008670C;
    D_8009D8F4 = D_8009D894;
    D_8009D8F8 = D_8009D8A8;
    D_8009D8FC = D_8009D8BC;
    D_8009D904 = D_8009D8DC;
    D_8009D900 = D_8009D8D0;
    D_8009D908 = 0;
    D_8009E0D8 = 0xD;
    D_8009E0DA = 0xE;
    D_8009E0DC = 0xF;
    D_8009E0DE = 0x10;
    D_8009E0E0 = 0x11;
    D_8009E0E2 = 0x13;
    D_8009E0E4 = 0x14;
    D_8009E0E6 = 0xC;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80083708);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADF60);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADF68);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADF70);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADF78);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADF80);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADF88);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADF90);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADF98);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADFA4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800ADFAC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80083E8C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80084788);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80084E04);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008502C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", SetHeroSeq);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80085584);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80085650);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80085718);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80085890);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800859D4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80085C84);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80085D2C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800860D4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800863D0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800864CC);

s32 func_800866DC(s32 *arg0) {
    s32 buf[4];

    *(Vec3W *)(buf + 1) = *(Vec3W *)(arg0 + 4);
    return 0;
}

void func_80086704(void) {
}

s32 func_8008670C(void) {
    return 0;
}

s32 func_80086714(void) {
    return 0;
}

s32 func_8008671C(void) {
    return 0;
}

s32 func_80086724(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008672C);

void func_800867EC(void) {
    func_80085D2C(1);
}

void func_8008680C(void) {
}

void func_80086814(void) {
    if (D_8009D86C != 0) {
        func_80016478(D_8009D86C, D_8009D868);
    }
    if (D_8009D878 != 0) {
        func_80016478(D_8009D878, D_8009D874);
    }
}

void func_80086868(void) {
    D_8009D6DC[0] = 0;
    D_8009D6DC[1] = 0;
    D_8009D6DC[2] = 0;
    D_8009D6DC[3] = 20;
    D_8009D6DC[4] = 0;
}

void func_8008688C(void) {
    D_800AD12C = D_8005A5E8[D_8005A61C].unkC;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", SetHeroAction);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80086918);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80086C94);

void func_80086D88(void) {
    s32 i;
    HeroInfo *hero;
    EXTengine *eng;
    s32 flag;
    s32 px;
    s32 x;
    s32 z;
    s32 xlo;
    s32 xhi;
    s32 zlo;
    s32 zhi;

    for (i = 0; i < 4; i++) {
        hero = &Heroes[i];
        if ((u32)(hero->hero_name - 1) < 8) {
            eng = hero->hero_eng;
            flag = (i << 5) | 0x80;
            px = eng->unk10.x >> 8;
            x = px + 10;
            z = 9 - (eng->unk10.z >> 8);
            if (x < 0) {
                continue;
            }
            if (x >= 20) {
                continue;
            }
            if (z < 0) {
                continue;
            }
            if (z < 20) {
                xhi = x;
                xlo = xhi;
                zhi = z;
                zlo = zhi;
                if (xhi > 0) {
                    xlo = px + 9;
                }
                if (xhi < 19) {
                    xhi = px + 11;
                }
                if (zhi > 0) {
                    zlo = zhi - 1;
                }
                if (zhi < 19) {
                    zhi++;
                }
                D_8009D90C[z * 20 + x].unk0 |= flag;
                D_8009D90C[zlo * 20 + x].unk0 |= flag;
                D_8009D90C[zhi * 20 + x].unk0 |= flag;
                D_8009D90C[z * 20 + xlo].unk0 |= flag;
                D_8009D90C[zlo * 20 + xlo].unk0 |= flag;
                D_8009D90C[zhi * 20 + xlo].unk0 |= flag;
                D_8009D90C[z * 20 + xhi].unk0 |= flag;
                D_8009D90C[zlo * 20 + xhi].unk0 |= flag;
                D_8009D90C[zhi * 20 + xhi].unk0 |= flag;
            }
        }
    }
}

void func_80086F4C(void) {
    s32 i;
    s32 j;

    i = 0;
loop:
    j = 0;
    if (i < 20) {
inner:
        if (j < 20) {
            j++;
            goto inner;
        }
        i++;
        goto loop;
    }
}

void func_80086F78(void) {
    s32 i = 399;
    D_8009D90C_t *p = D_8009D90C;

    for (p += 399; i >= 0; i--) {
        p->unk0 = 0;
        p--;
    }
}

void func_80086FA0(void) {
    s32 i;
    D_8009D90C_t *p;

    for (i = 0, p = D_8009D90C; i < 400; i++) {
        p->unk1 = 0;
        p->unk0 = 0;
        p++;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80086FCC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800871E4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80087428);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800874EC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008795C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80087B8C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80087FC0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800880E0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800881E0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800882DC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80088450);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80088544);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80088708);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80088794);

void func_80088BAC(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80088C28);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80088F78);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008900C);

void func_800894E0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008955C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800898EC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80089BE8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80089FA4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A200);

void func_8008A810(void) {
    u8 *p = D_8005A614;

    if (*(s32 *)(p + 4) & 0x800) {
        *(u16 *)(p + 0x30) |= 0x201;
        return;
    }
    *(u16 *)(p + 0x30) |= 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A854);

void func_8008A8F0(s32 arg0) {
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A924);

void func_8008A99C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 2);
}

void func_8008A9F0(s32 arg0) {
    Heroes[arg0].hero_idle_count = Random(0x258) + 0x12C;
}

void func_8008AA40(s32 arg0) {
    Heroes[arg0].hero_idle_count = Random(0x96) + 0x5A;
}

void func_8008AA90(void) {
    func_8008502C();
}

void func_8008AAB0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0);
}

s32 func_8008AB04(s32 arg0) {
    PadState *base = D_80051338;
    PadState *pad = &base[arg0];

    *(s32 *)((u8 *)pad + 0x50) &= ~0x4000;
    *(s32 *)((u8 *)pad + 0x54) &= ~0x4000;
    return 0;
}

s32 func_8008AB48(void) {
    return 0;
}

s32 func_8008AB50(s32 arg0) {
    u16 temp_v1;
    u32 m;

    temp_v1 = Heroes[arg0].hero_status;

    if (temp_v1 & 0x200) {
        return 1;
    }
    m = temp_v1 & 0x400;
    return m != 0;
}

s32 func_8008AB94(s32 arg0) {
    s32 ret;

    if (!(Heroes[arg0].hero_status & 0x2000)) {
        ret = 0;
    } else {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E2);
        ret = 1;
    }
    return ret;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008ABF8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008AC74);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008ACF0);

void func_8008AD9C(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008ADA4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008B29C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008B418);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", InitScriptFX);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", PlotScriptFX);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", PlotSmokePoof);

s32 func_8008C4C8(s32 *arg0, s32 arg1) {
    s32 v[3];
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 ang;

    base = *(u8 **)((u8 *)D_80058AD0 + 0x10);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        n = *(s32 *)(p + 0x38);
        t = D_80068BD4;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = *(s32 *)(a3 + 0x64);
            v[0] = arg0[0] - *(s32 *)(a2 - 8);
            v[2] = arg0[2] - *(s32 *)a2;
            ang = -ang;
            a3 += 0x38;
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                return 0;
            }
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008C5C4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008C824);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008CA90);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", PlotWeatherNas);

void InitWeatherNas(u8 *arg0) {
    *(s32 *)(arg0 + 0x60) = 0;
    *(s32 *)(arg0 + 0x64) = 0;
    **(s32 **)(*(u8 **)(arg0 + 0x54) + 0x6C) = 0x20008000;
    if (D_8005A648 == 0x1F) {
        func_80015D70(D_800C45DC, *(u8 **)(*(u8 **)(arg0 + 0x54) + 0x6C));
    }
}

void func_8008CE10(void) {
}

void func_8008CE18(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008CE20);

s32 func_8008CEB0(u8 *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = (*(s32*)((u8*)(arg0) + 0)) - (*(s32*)((u8*)(arg1) + 0));
    temp_v0_2 = (*(s32*)((u8*)(arg0) + 8)) - (*(s32*)((u8*)(arg1) + 8));
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
}

void PlotParticleEffect(void) {
}

void InitParticleEffect(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", InitSmokePoof);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008CFBC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008D088);

u8 *func_8008D24C(u8 *arg0, s32 *arg1) {
    u8 *r;

    r = func_80021DD4(arg0, arg1[0x24 / 4], arg1[1]);
    if (r != NULL) {
        return r + 0x24;
    }
    return NULL;
}

u8 *func_8008D284(u8 *arg0, u8 *arg1) {
    u8 *p = func_80021DD4(arg0, *(s32 *)(arg1 + 0x24), *(s32 *)(arg1 + 4));
    if (p != NULL) {
        return p + 0x14;
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", PlotFloatyScore);

void func_8008D488(u8 *arg0, s32 arg1) {
    s32 a;
    s32 b;

    if (arg1 != 0) {
        arg0[4] = 0x25;
        arg0[3] = (arg1 % 10) + 0x30;
        a = ((arg1 / 10) % 10) + 0x30;
        arg0[2] = a;
        b = (((arg1 / 10) / 10) % 10) + 0x30;
        arg0[1] = b;
        b &= 0xFF;
        if (b == 0x30) {
            s32 c = a & 0xFF;
            do { } while (0);
            arg0[1] = 0x20;
            if (b == c) {
                arg0[2] = 0x20;
            }
        }
    } else {
        arg0[1] = 0x20;
        arg0[2] = 0x4F;
        arg0[3] = 0x46;
        arg0[4] = 0x46;
    }
}

void InitFloatyScore(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = (*(s32*)((u8*)(arg0) + 84));
    if (temp_v1 != NULL) {
        (*(s16*)((u8*)(arg0) + 94)) = 0x1E;
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = (s32) ((*(s32*)((u8*)(temp_v1) + 20)) - 0x1A0);
        (*(s8*)((u8*)(temp_v1) + 93)) = 2;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS((NAS *)arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008D5D4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008D6C8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", InitHealth);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", PlotHealth);

void PlotBigPoof(NAS *nas) {
    func_800250B0(nas);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", InitBigPoof);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008DDC0);

s32 func_8008DE8C(void) {
    return 0;
}

s32 func_8008DE94(s32 arg0) {
    return (Heroes[arg0].hero_status & 0xFA00) == 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008DEC8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008E27C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", InitCrateExplode);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008E5BC);

void func_8008EAC8(void) {
    s32 f;
    RECT *a;
    RECT *r;

    if (D_800AEC5C != 0) {
        func_80016F54((u8 *)D_800AEC5C);
    }
    func_80021FB0();
    func_8001D604();
    f = D_8005B690;
    a = &D_8005B698[f & 1];
    r = &D_8005B698[(f + 1) & 1];
    MoveImage2(a, r->x, r->y);
}

void func_8008EB3C(void) {
    func_80018B08(D_8009E498, D_80051640);
    if (D_800AEC5C != 0) {
        func_8009440C();
    }
    func_80022070();
}

void func_8008EB8C(void) {
    D_8009E5A4 = 0;
    D_8009E5A8 = 0;
    D_800AEC58 = 0;
    D_800AEC5C = 0;
    func_8001E984(0x20, 0x80);
    D_8009E5B0 = -1;
    D_8009E5B4 = 0;
    func_8001D640(0x200);
    func_80014388(D_80051640);
    D_800590CC = 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008EC00);

void func_8008EC94(void) {
    s32 f = D_8005B690;
    RECT *a = &D_8005B698[f & 1];
    RECT *r = &D_8005B698[(f + 1) & 1];

    MoveImage2(a, r->x, r->y);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008ECE4);

s32 func_8008EF70(void) {
    return D_8009EDD4[D_8005A676][0];
}

void func_8008EF9C(void) {
    D_8009E5DC = func_8008EF70();
    D_8005A644 = 0x8008;
    func_8008ECE4();
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008EFD4);

void func_8008F068(void) {
    D_8009E5DC = 0x28;
    D_8005A644 = 0x8008;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008F084);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008F248);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008F41C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008F554);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FB48);

void func_8008FC58(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FC60);

void func_8008FD40(void *arg0) {
    s32 i;
    s32 n = *(s32 *)((u8 *)arg0 + 8);

    for (i = 0; i < n; i++) {
        u8 *e = *(u8 **)((u8 *)arg0 + 4);
        D_8005A614[*(s32 *)(e + i * 16 + 4) + 0xE4] = 1;
        n = *(s32 *)((u8 *)arg0 + 8);
    }
}

void func_8008FD94(void) {
    u8 *g = D_8005A614;
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
    u8 *g = D_8005A614;
    s32 n;

    D_8009EB98 = 4;
    n = g[0x28];
    if (n > 0) {
        n--;
        PlaySfx(0x190, 0, 0, 0x1000, 0x1E00, 0);
        g[0x28] = n;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FE68);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FF10);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FF94);

s32 func_80090008(void) {
    return D_8009E7A0[D_8005A614[0x28]].unk4[D_8005A614[0x29]].unk4;
}

void func_80090048(void) {
    D_8009EB98 = 0;
    func_80095BEC(0, D_8009E8B4);
    func_80095BEC(2, D_8009E7F4);
    func_80095BEC(3, D_8009EB3C);
    func_80095BEC(4, D_8009EB3C);
    func_80095BEC(5, D_8009EB3C);
    func_80095BEC(6, D_8009EB3C);
    D_8005A63C = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800900CC);

static inline s32 func_800901B4_find(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = D_8009EDD4[0]; (u32)i < 10; i++, p += 5) {
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
    return D_8009EDD4[i];
}

s32 func_8009020C(s32 arg0) {
    s32 i;
    s32 *p;

    for (i = 0, p = D_8009EDD4[0]; (u32)i < 10; i++, p += 5) {
        if (*p == arg0) {
            return i;
        }
    }
    return -1;
}

s32 func_80090248(void) {
    EngSub90248 *p;

    if (D_8005A677 < 0 || (p = D_800BCCF4[D_8005A677].unk14, p->unk2 != 2)) {
        return -1;
    }
    return p->unk4;
}

void func_8009029C(void) {
    s32 i;
    EngEntry *e;
    EngTable *t = (EngTable *)D_8009EDD4;

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80090310);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80090564);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80090A10);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE47C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE490);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE498);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE4B4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE4FC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE500);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE504);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE554);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE594);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE5CC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE618);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE634);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE64C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE664);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE68C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE6A0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE6B4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE6EC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE760);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE7D4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE7E0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE850);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE8D4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE8E0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AE988);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEA04);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEA34);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEA64);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEA8C);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEAC4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEAEC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEAF4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEAF8);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEAFC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEB00);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEB04);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEB08);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEB14);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/engine", D_800AEB1C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80090E20);

void func_800917B4(void) {
    EXTengine *e;
    subengine *s;
    s32 v;

    func_80018B08(&D_8009F3E4, &D_8009F578);
    e = GetEngineSlot(8);
    s = e->sub;
    s->unk14 = D_8005B690 * 20;
    s->unk12 = 0x293;
    s->unk50 = 1;
    s->unk20.z = 0x2800;
    s->unk20.x = 0x2800;
    s->unk52 = 1;
    s->unk51 = 0;
    D_800569AC = *(s32 *)D_8009F644;
    func_8001C2C0((u8 *)e);
    func_80018B08(&D_8009F3BC, &D_8009F4C0);
    switch (D_8009F630) {
    case 0:
        if (D_80051380 & 0xF0) {
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            v = 1;
            if (D_8009F634 == 1) {
                v = 2;
            }
            D_8009F634 = v;
        }
        if (D_80051380 & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            if (D_8009F634 == 1) {
                D_8009F640 = 0;
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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009196C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80091E58);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80091F54);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009201C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800920DC);

void func_80092198(void) {
    EXTengine *eng;

    func_80091E58();
    func_80095BEC(2, &D_8009EF64);
    GetEngineSlot(2)->unk10.x = 0x28;
    GetEngineSlot(2)->unk10.y = 0x1E;
    eng = GetEngineSlot(2);
    eng->flags |= 0x10000000;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092200);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800922FC);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009246C);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800925F0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800926CC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800928E8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800929D8);

void func_80092AA8(GameModeDef *arg0) {
    if (((ModeMgr *)D_8009F658)->next != arg0) {
        ((void (*)(u8 *))func_8001E588)(D_8009F644);
        ((ModeMgr *)D_8009F658)->next = arg0;
    }
}

void func_80092AF4(s32 arg0, s32 arg1) {
    u8 *p = D_8009F658;

    if (*(s32 *)(p + 4) != arg0) {
        func_8001E588(D_8009F644, arg1, 0);
        *(s32 *)(p + 4) = arg0;
    }
}

void func_80092B40(void) {
    if (((ModeMgr *)D_8009F658)->cur != NULL) {
        if (((ModeMgr *)D_8009F658)->cur->exit != NULL) {
            ((ModeMgr *)D_8009F658)->cur->exit();
        }
    }
}

void func_80092B7C(void) {
    func_8001E898(D_8009F658);
}

void func_80092BA0(void) {
    func_8001E598(D_8009F644);
    func_8001E610(D_8009F658, D_8009F644);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092BDC);

void func_80092C54(void) {
    func_800926CC(D_8005A614);
}

void func_80092C78(void) {
    s8 *p;
    *(s32 *)D_8005A604 = 0x46;
    p = D_8005A604;
    *(s32 *)(p + 4) = 0x64;
    *(s32 *)(p + 8) = 1;
    p[0xC] = 1;
    p[0xD] = 1;
    p[0xE] = 1;
    p[0xF] = 1;
}

void func_80092CAC(void) {
    EngineGameView *g;

    g = (EngineGameView *)D_8005A614;
    g->unk30 &= 0x7FF8;
    func_8001E588(D_8009F644, -0x300, 0);
    D_8009F8B8 = 0;
    if (g->mode == 0x24 && g->unkE0 <= 0) {
        g->unkE0 = 1;
    }
    g = (EngineGameView *)D_8005A614;
    if (func_8007A5E8((u8 *)g) >= 0x19 && g->unkE0 < 2) {
        g->unkE0 = 2;
    }
    func_80017640();
    func_80010AE8(&D_8004E0F0);
    D_800590CC = 0;
    if (D_8005A644 & 0x200) {
        func_80079260();
    } else {
        func_8002C284(1);
    }
    EngineBeginMode(1);
}

void func_80092E40(void) {
    func_80022030();
    func_8007AB28();
    func_80022504();
    func_8001E854(&D_8009F8A4);
    func_8001888C();
    func_800183DC();
    func_8008ECE4();
}

void func_80092E94(void) {
    D_8009F8B4();
    D_800569AC = *(s32 *)D_8009F644;
    func_80082590();
    func_8001E898(&D_8009F8A4);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092EDC);

void func_80092F9C(void) {
    D_8005A614[0x63] = 0xA;
    *(s32 *)(D_8005A614 + 0x64) = 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092FB8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093038);

void func_80093110(void) {
    func_80018B08((GfxViewport *)D_8009F6F4, (GfxCamera *)D_80051640);
    D_80056998.modelDp = 0x1000 - ((0x1000 - D_8009F8B8) * (0x1000 - *(s32 *)D_8009F644) / 4096);
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009317C);

void func_80093270(void) {
    TrigEntry *t;
    WobbleObj *o;
    s32 f;
    s32 i;

    if (D_800A0158 >= 2 && (o = D_800A0290) != NULL) {
        t = D_80068BD4;
        o->unk51 = 0;
        o->unk12 += 0x20;
        f = D_8005B690;
        o->unk10 = *(s32 *)&t[(((f * 70) & 0xFFF) - 0x400) & 0xFFF] >> 20;
        o->unk12 = *(s32 *)&t[(((f * 140) & 0xFFF) - 0x400) & 0xFFF] >> 21;
        o->unk14 = (t[(((f * 70) & 0xFFF) - 0x400) & 0xFFF].unk2 +
                    t[(((f * 100) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 5;
        o->unk4 = *(s32 *)&t[(((f * 35) & 0xFFF) - 0x400) & 0xFFF] >> 23;
        o->unk8 = (t[(((f * 45) & 0xFFF) - 0x400) & 0xFFF].unk2 +
                   t[(((f * 145) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 5;
        o->unk52 = 1;
        o = o->unk5C;
        if (o != NULL) {
            o->unk14 += 0x17;
            i = (-f * 10) & 0xFFF;
            o->unk10 = *(s32 *)&t[(i - 0x400) & 0xFFF] >> 20;
            o->unk12 = *(s32 *)&t[i] >> 20;
            o->unk52 = 1;
        }
    }
}

void func_8009342C(void) {
    u8 *p;
    u8 *b;

    if (D_800A0158 >= 2) {
        if (D_800A02C0 == 0 && *(s32 *)((b = D_80052A54) + 0x2CAC) != 0) {
            if (D_80064C93 != -1) {
                PlaySfx(0x3C5, NULL, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x3C6, NULL, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x3C7, NULL, NULL, 0x1000, 0x1E00, 0);
                p = b + 0x2CA0;
                D_800A02C4 = 6;
                func_80095258(p);
                D_800A02C0 += 1;
                func_8001FE80(p, 0, &D_8009F9D4, &D_8009F9F4, 8, NULL);
            }
        }
        if (D_800A02C4 != 0) {
            D_800A02C4 -= 1;
            if (D_800A02C4 == 0) {
                PlaySfx(0x3C8, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        func_800220E8();
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093588);

static inline void func_800936C4_heap(Heap *heap, s32 p) {
    s32 size;

    size = 0x801FF000;
    size -= p;
    func_80011BF8(heap, (u8 *)p, size);
    func_80012E54();
}

void func_800936C4(void) {
    s32 start;
    EngineCfg *cfg;

    func_8002AC70();
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    func_800936C4_heap(&D_8004E0F0, (s32)D_800D7144);
    start = func_8009317C() + 7;
    start &= ~7;
    func_800936C4_heap(&D_8004E0F0, start);
    func_800235BC(&D_80098F48);
    cfg = (EngineCfg *)D_8005A5E4;
    if (cfg == NULL) {
        cfg = &D_8009F96C;
    }
    func_800274FC((s32 *)cfg);
    ((void (*)(u8 *))func_800276A4)(D_800637A8 == 0 ? D_8005B72C : D_8005B744);
    D_8005A614[0x60] = 0;
    D_8005A614[0x61] = 0;
    func_80028774(cfg->areas, cfg->pool, cfg->npool, cfg->nbuf);
    func_80017164(cfg->pools);
    func_800170D4();
    func_800222FC();
    func_8001D5D0();
    D_800A0158 = 0;
    if (((D5A614View *)D_8005A614)->mode == 0x28 && D_800590CC != 0) {
        ((ModeMgr *)D_8009F658)->flags = 0x1D;
        ((ModeMgr *)D_8009F658)->next = (GameModeDef *)D_800B9524;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093854);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800939FC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093BE8);

void func_8009400C(void) {
    struct E { s32 a, b; u8 pad[0x30]; };
    struct G { struct E e[0x1C]; s32 x, y; } *g;
    struct E *e;
    u16 i = D_800A00D0[D_8005A640];
    g = (void *)D_80052A54;
    e = &g->e[i];
    g->x = e->a;
    g->y = e->b;
}

void func_80094058(void) {
    func_8001E854(D_800A015C);
}

void func_8009407C(void) {
    if (D_800A0158 >= 2) {
        func_8001E898(D_800A015C);
    }
}

void func_800940B0(s32 arg0, s32 arg1) {
    s32 i;
    u8 *p = D_8009FA14;

    for (i = 0; i < 0x60; i++) {
        if (arg0 == *(s32 *)(p + 4)) {
            func_800129D0(p, arg1);
        }
        p += 0x10;
    }
}

void func_80094120(void) {
    func_80022030();
    func_80016F54(D_800556F4);
}

void func_8009414C(void) {
    if (D_800A0158 >= 2) {
        D_8004E0D8 += 1;
        func_80018B08(D_8009F9AC, D_80051640);
        if (D_800A02C0 != 0) {
            func_8009440C();
        }
        func_80022070();
    }
}

void func_800941BC(void) {
    func_80014388(D_80051640);
    D_800A02C0 = 0;
    D_800A02C4 = 0;
}

void func_800941EC(void) {
    func_8001D174(D_800A0224);
    func_80016F54(D_800555A4);
}

void func_8009421C(void) {
    if (D_800A0158 >= 2) {
        func_80018B08((GfxViewport *)D_8009F8BC, (GfxCamera *)D_800A016C);
        D_800569AC = *(s32 *)D_8009F644;
        func_8001C298((EXTengine *)D_800A0224);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80094274);

void func_80094364(void) {
    s32 *a = D_800A00F0;
    s32 *p;

    if (D_8005A650 >= 0) {
        p = (s32 *)(D_80052A54 + D_8005A650 * 0x38);
    } else {
        p = NULL;
    }
    func_8001DEF4(a, p);
}

void func_800943B8(void) {
    if (D_8005A648 == 0x28 && D_800590CC == 0) {
        func_80016CA0(D_800556F4);
    } else {
        func_80016CA0(D_800555A4);
    }
}

void func_8009440C(void) {
    s32 h = *(s32 *)(D_80056998.cam->unk18);
    subengine *e = &D_800A0C74;
    EngineMarker *m;
    EngineMarkerPos *p;
    s32 i;
    s32 s;

    for (i = 0; i < D_800A0C70; i++) {
        m = &D_800A0C10[i];
        p = &D_800A02D8[m->idx];
        *e = D_8005A830;
        e->unk74 = m->unk0;
        e->unk6C = m->unk8;
        e->unk51 = 0;
        s = D_8004E0D8;
        e->unk10 = (s * p->x) >> 12;
        e->unk12 = (s * p->y) >> 12;
        e->unk14 = (s * p->z) >> 12;
        e->flags = 0x03008000;
        e->unk68 = p->unkC;
        gte_SetGeomScreen(h);
        func_80019F1C((u8 *)e, (u8 *)&D_80056998.cam->view, 0x03008000);
        gte_SetGeomScreen(*(s32 *)(D_80056998.cam->unk18));
    }
}

void func_800945C0(EngineStartArgs *arg0) {
    s32 rate;

    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    rate = 3;
    D_8004E0E0 = rate;
    D_80058B64 = 0x1E0000 / (60 / rate);
    D_80058B64 = arg0->unk8 * rate / 60;
    func_80014388(D_80051640);
    D_800A0D30 = 0;
    D_800A0D38 = 1;
    D_800A0C70 = 0;
    ((EngineStartArgs *)&D_800A0D2C)->unk0 = arg0->unk0;
    D_800A0D1C = arg0->unk4;
    D_800A0D20[0] = 0;
    D_800A0D24[0] = 0;
    func_8002232C();
    D_800644C0[0].id = -1;
    D_800644C0[0].handle = 0;
    D_800644C0[0].head = NULL;
    D_800644C0[1].id = -1;
    D_800644C0[1].handle = 0;
    D_800644C0[1].head = NULL;
    D_800A0D28 = 0;
    D_800654B0 = 0;
    D_800654B4 = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80094754);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80094860);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80094A28);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80094B38);

void func_80094D88(s32 arg0, s32 arg1) {
    D_800A0C08 = arg1;
    D_800A0C0C = arg0;
    func_8001E588(D_8009F644, 0x200, 0);
    D_8009F65C = D_800A0BF4;
}

void func_80094DD0(void) {
    func_8002BA44();
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    func_80094F60();
    func_80022030();
    func_8002BA44();
    func_8002B96C();
    if (D_800644C0[0].id != -1) {
        func_8002BCB0((s32 *)&D_800644C0[0]);
    }
    if (D_800644C0[1].id != -1) {
        func_8002BCB0((s32 *)&D_800644C0[1]);
    }
    func_8002232C();
    func_8002726C();
    D_800A0C70 = 0;
    D_800A0D3C += 1;
    D_8004E0E0 = 2;
}

void func_80094EB8(void) {
    func_80018B08(&D_800A0BCC, (GfxCamera *)D_80051640);
    D_80056998.modelDp = *(s32 *)D_8009F644;
    func_80022070();
    D_80056998.modelDp = 0x1000 - (((0x1000 - D_80058B60) * (0x1000 - D_80056998.modelDp)) >> 12);
    func_8009440C();
}

s32 func_80094F30(void) {
    s32 var_v0;
    u8 *temp_v1;

    temp_v1 = (*(s32*)((u8*)(D_800A0D2C) + 4));
    var_v0 = 0;
    if (temp_v1 != NULL) {
        var_v0 = (*(s32*)((u8*)(temp_v1) + 12)) != 0;
    }
    return var_v0;
}

void func_80094F60(void) {
    if (((*(s32*)((u8*)(D_800A0D2C) + 4)) != D_800A0D30) && (D_800A0D30 != 0)) {
        func_80016F54(D_800A0D30);
    }
}

void func_80094FA8(void) {
    s32 temp_a0;

    temp_a0 = (*(s32*)((u8*)(D_800A0D2C) + 4));
    if (temp_a0 != 0) {
        func_8001612C(temp_a0, 0);
    }
}

void func_80094FE0(void) {
    if (D_800A0C08 != 0) {
        func_80092AF4(D_800A0C08, 0x200);
        D_800A0D3C += 1;
    }
}

void func_80095020(void) {
    func_8002BA44();
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    func_80094F60();
    func_80022030();
    func_8002BA44();
    func_8002B96C();
    if (D_800644C0[0].id != -1) {
        func_8002BCB0((s32 *)&D_800644C0[0]);
    }
    if (D_800644C0[1].id != -1) {
        func_8002BCB0((s32 *)&D_800644C0[1]);
    }
    func_8002232C();
    func_8002726C();
    D_800A0C70 = 0;
    D_800A0D3C += 1;
}

s32 func_800950FC(void) {
    if (D_800A0D1C == 0) {
        return 1;
    }
    if (*(s32 *)D_800A0D1C == 0) {
        return 1;
    }
    return ~D_800644C0[D_800A0D20[0]].id != 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80095160);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800951D0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80095258);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800952F8);

void func_80095A04(s32 arg0) {
    EXTengine *eng;

    eng = &EnginePool[arg0];
    func_8001D334(eng->sub);
    eng->flags &= 0xFFFF7FFF;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80095A60);

void func_80095BEC(s32 arg0, s32 *recs) {
    EXTengine *eng;
    subengine *subs;
    s32 *rec;
    s32 i;
    s32 n;

    if (recs != NULL) {
        eng = &EnginePool[arg0];
        *eng = EngineTemplate;
        rec = recs;
        i = 0;
        subs = D_800A13F4[arg0];
        while (*rec >= 2) {
            rec += 9;
            i++;
        }
        n = i;
        func_8001D334(eng->sub);
        *eng = EngineTemplate;
        eng->unk5C = 1;
        eng->unk5D = 0;
        eng->flags |= 0x10000000;
        if (n > 0) {
            func_8001D3B0(eng, n, subs);
            func_800952F8(eng->sub, recs);
        }
    }
}

/* Counts the 0x24-byte records before the first whose leading word is
 * below 2. */
s32 func_80095D88(s32 *rec) {
    s32 n;

    n = 0;
    while (*rec >= 2) {
        rec += 9;
        n++;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80095DBC);

EXTengine *GetEngineSlot(s32 arg0) {
    return &EnginePool[arg0];
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80095E5C);

s32 func_80095F48(s32 *p, s32 a1, s32 a2, s32 a3, s32 a4) {
    if (p == NULL) {
        return 0;
    }
    p[0] = 0x10008000;
    p[0x54 / 4] = (s32)D_8001C448;
    p[1] = a1;
    p[2] = a2;
    p[0x6C / 4] = a3;
    p[0x7C / 4] = a4;
    return p[0x5C / 4];
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80095F88);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009608C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80096280);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80096530);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800967E0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80096A08);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80096F98);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80097158);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80097244);

void func_800972BC(s32 arg0, s32 arg1) {
    s32 temp = arg0 - ((arg1 + 0x3000) * 0xF0) / D_80051658;

    if (D_800AD320 < temp) {
        D_800AD320 = temp;
        D_800AD2F8[1] = arg0;
        D_800AD2F8[2] = arg1;
    }
}

void func_80097334(s32 arg0, s32 arg1) {
    s32 v = arg0 + ((arg1 + 0x3000) * 0xF0) / D_80051658;

    if (v < D_800AD31C) {
        D_800AD31C = v;
        D_800AD2EC.unk4 = arg0;
        D_800AD2EC.unk8 = arg1;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800973AC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80097424);

void func_8009749C(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5) {
    D_800AD1EC_entry *e;

    if (D_800AD1E8 < 0x10) {
        e = &D_800AD1EC[D_800AD1E8];
        e->unk0 = arg0;
        e->unk4 = arg1;
        e->unk6 = arg2;
        e->unk8 = arg3;
        e->unkA = arg4;
        e->unkC = arg5;
        D_800AD1E8 += 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800974EC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80097608);

s32 func_800976A8(u8 *arg0, s32 arg1) {
    s32 temp_v1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t0;
    u8 *var_a0;

    var_a0 = arg0;
    var_t0 = 0x80000000;
    var_a3 = 0x7FFFFFFF;
    var_a2 = 0;
    if (arg1 > 0) {
        do {
            temp_v1 = (*(s32*)((u8*)(var_a0) + 8));
            if (var_t0 < temp_v1) {
                var_t0 = temp_v1;
            }
            if (temp_v1 < var_a3) {
                var_a3 = temp_v1;
            }
            var_a2 += 1;
            var_a0 += 0xC;
        } while (var_a2 < arg1);
    }
    return var_t0 - var_a3;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800976F8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80097AA4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80097C28);

void func_80098278(void) {
    D_800ADBC0 = 0;
}

void func_800976F8(s32 *);
void func_80098284(void) {
    s32 *p;
    if (D_800ADBC0 == 0) {
        do {
            D_800ADBC0 = 1;
            p = &D_800ADBC0;
            p[1] = 0;
            p[2] = 0;
            func_800976F8(p);
        } while (0);
        PlaySfx(0x19D, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

extern s32 D_800ADBD4;
void func_800982E0(void) {
    s32 i;
    subengine *s = GetEngineSlot(3)->sub + 2;
    for (i = 0; i < D_800ADBD4; i++) {
        s->flags &= ~0x8000;
        s = s->next;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80098344);

void func_80098428(void) {
    subengine *s = GetEngineSlot(3)->sub;
    s->flags |= 0x8000;
    func_80097AA4(s);
}

void func_80098464(void) {
    subengine *s = GetEngineSlot(3)->sub;
    u32 t = D_8005A646 & 0xF0;
    if (t == 0x40 || t == 0x80) {
        s->flags |= 0x8000;
        func_80097AA4(s);
    } else {
        s->flags &= ~0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800984D4);

void func_80098570(void) {
    s32 *p;
    D_800ADBC0 = 0;
    p = &D_800ADBC0;
    p[1] = -1;
    p[3] = 0;
    p[4] = 1;
}
