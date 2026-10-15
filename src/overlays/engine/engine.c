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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A5E8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A614);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007A8C4);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007AB28);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D034);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007D0BC);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007E7F4);

void func_8007E874(void) {
    D_8009AE78 = 0xF0;
    D_80099174 |= 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007E894);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007E8D0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007E93C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007E9FC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007EAC0);

void func_8007EB04(void) {
    D_80099174 &= ~1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007EB1C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007EB68);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007EC48);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007ED0C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8007EE2C);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80082590);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800826F8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80082AC4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80082C34);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80082D90);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800831C4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800831F0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80083244);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800832BC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80083334);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80083488);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800834E4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008352C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80083550);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800866DC);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80086868);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008688C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", SetHeroAction);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80086918);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80086C94);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80086D88);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80086F4C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80086F78);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80088BAC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80088C28);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80088F78);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008900C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800894E0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008955C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800898EC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80089BE8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80089FA4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A200);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A810);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A854);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A8F0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A924);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A99C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008A9F0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008AA40);

void func_8008AA90(void) {
    func_8008502C();
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008AAB0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008AB04);

s32 func_8008AB48(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008AB50);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008AB94);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008C4C8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008C5C4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008C824);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008CA90);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", PlotWeatherNas);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", InitWeatherNas);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008D24C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008D284);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", PlotFloatyScore);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008D488);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008DE94);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008DEC8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008E27C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", InitCrateExplode);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008E5BC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008EAC8);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008EC94);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FD40);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FD94);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FE00);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FE68);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FF10);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8008FF94);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80090008);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800901B4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009020C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80090248);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009029C);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800917B4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009196C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80091E58);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80091F54);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009201C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800920DC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092198);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009251C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092560);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092598);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800925C4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800925F0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800926CC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800928E8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800929D8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092AA8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092AF4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092B40);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092C78);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092CAC);

void func_80092E40(void) {
    func_80022030();
    func_8007AB28();
    func_80022504();
    func_8001E854(&D_8009F8A4);
    func_8001888C();
    func_800183DC();
    func_8008ECE4();
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092E94);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092EDC);

void func_80092F9C(void) {
    D_8005A614[0x63] = 0xA;
    *(s32 *)(D_8005A614 + 0x64) = 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80092FB8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093038);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093110);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009317C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093270);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009342C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093588);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800936C4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093854);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800939FC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80093BE8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009400C);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80094364);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800943B8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009440C);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800945C0);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80094DD0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80094EB8);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80095020);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800950FC);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80095BEC);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80095F48);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80097334);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800973AC);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80097424);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_8009749C);

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

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80098284);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800982E0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80098344);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80098428);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80098464);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_800984D4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/engine", func_80098570);
