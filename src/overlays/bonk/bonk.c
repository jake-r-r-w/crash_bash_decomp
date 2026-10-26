#include "common.h"
#include "bash/main.h"
#include "bash/bonk_ovl.h"
#include "bash/bonk.h"
#include "bash/pogo.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern u8 D_80034214[];
extern u8 D_8004EB70[];
extern u8 D_8004EB78[];
extern u8 D_8004EB80[];
extern u8 D_8004EB88[];
extern u8 D_8004EB90[];
extern u8 D_8004EB98[];
extern u8 D_8004EBA0[];
extern u8 D_8004EBA8[];
extern u8 D_8004EBB0[];
extern u8 D_8004EBB8[];
extern u8 D_80050064[];
extern u8 D_80063BB8[];
extern u8 D_80063EC8[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char D_800B32B8[] = "BREATHE";
const char D_800B32C0[] = "MEDIUM";
const char D_800B32C8[] = "ATTACK";
const char D_800B32D0[] = "HIT";
const char D_800B32D4[] = "WIN";

/* data, in address order */
u8 D_800B929C[20] = { 0x19, 0, 0x16, 0, 0x13, 0, 0x10, 0, 0xC, 0, 0x1C, 0, 0x18, 0, 0x10, 0, 0x10, 0, 0x1E, 0 };
u8 D_800B92B0[20] = { 0x19, 0, 0x16, 0, 0x13, 0, 0x10, 0, 8, 0, 0x1C, 0, 0x18, 0, 0x10, 0, 0x10, 0, 0x1E, 0 };
u8 D_800B92C4[12] = { 0x16, 0, 0x19, 0, 0x1E, 0, 0x16, 0, 0x19, 0, 0x1E, 0 };
u8 D_800B92D0[24] = {
    0x14, 0, 0x1A, 0, 0x20, 0, 0x11, 0, 0x17, 0, 0x1C, 0, 0xE, 0, 0x13, 0, 0x18, 0, 0xC, 0,
    0x10, 0, 0x14, 0,
};
u8 D_800B92E8[24] = {
    0x14, 0, 0x1A, 0, 0x20, 0, 0x11, 0, 0x17, 0, 0x1C, 0, 0xE, 0, 0x13, 0, 0x18, 0, 0xC, 0,
    0x10, 0, 0x14, 0,
};
s32 D_800B9300[40] = {
    0x60, 0x5D, 0x5A, 0x57, 0x54, 0x51, 0x4E, 0x4B, 0x48, 0x45, 0x42, 0x3F, 0x3C, 0x39, 0x36,
    0x33, 0x30, 0x2D, 0x2A, 0x27, 0x24, 0x21, 0x1E, 0x1B, 0x18, 0x15, 0x12, 0xF, 0xC, 9, 6, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800B93A0[40] = {
    0x60, 0x5D, 0x5A, 0x57, 0x54, 0x51, 0x4E, 0x4B, 0x48, 0x45, 0x42, 0x3F, 0x3C, 0x39, 0x36,
    0x33, 0x30, 0x2D, 0x2A, 0x27, 0x24, 0x21, 0x1E, 0x1B, 0x18, 0x15, 0x12, 0xF, 0xC, 9, 6, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800B9440[40] = {
    0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
    0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x3C, 0x3D, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E,
};
s32 D_800B94E0[40] = {
    0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
    0x5F, 0x60, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
};
u16 D_800B9580[10] = { 0x1000, 0x200, 0x200, 0x220, 0x220, 0x320, 0x320, 0x300, 0x300, 0 };
u16 D_800B9594[10] = { 0x300, 0x240, 0x280, 0x200, 0x260, 0x280, 0x2C0, 0x340, 0x300, 0 };
s32 D_800B95A8 = 6;
s32 D_800B95AC[6] = { 5, 5, 4, 3, 2, 1 };
s32 D_800B95C4[7] = { 0x50, 0x50, 0x46, 0x46, 0x3C, 0x32, 0x28 };
s32 D_800B95E0[21] = {
    0x3C, 0x32, 0x28, 0x2D, 0x28, 0x23, 0x1E, 0x5A, 0x5A, 0x5A, 0x5A, 0x3C, 0x3C, 0x3C, 0x1E,
    0x1E, 0x19, 0x19, 0x14, 0xF, 0xF,
};
s32 D_800B9634 = 0xF;
s32 D_800B9638[421] = {
    0xF, 0xD, 0xA, 0xA, 0x5A, 0x5A, (s32)0x80070000, 0x41000016, 0x8000, (s32)0x80070000,
    0x4000001C, 0x8000, (s32)0x80070000, 0x40000020, 0x8000, (s32)0x80070000, 0x40000020,
    0x8000, (s32)0x80070000, 0x40000016, 0x8000, (s32)0x80070000, 0x40000014, 0x8000,
    (s32)0x80070000, 0x40000020, 0x8000, (s32)0x80070000, 0x40000020, 0x8000, 0x40000000,
    0x40024001, 0x40044003, 0x40064005, (s32)0x80034007, 0x40080008, 0x400A4009, 0x400C400B,
    0x400E400D, 0x4010400F, (s32)0x80034011, 0x40120009, (s32)0x80004013, 0, 0x41000000,
    0x41024101, (s32)0x80034103, 0x41040008, 0x41064105, 0x41084107, 0x410A4109, 0x410C410B,
    (s32)0x8003410D, 0x410E0009, 0x4110410F, 0x41124111, 0x41144113, (s32)0x80004115, 0,
    0x40800000, (s32)0x80034081, 0x40820008, 0x40844083, 0x40864085, 0x40884087, 0x408A4089,
    (s32)0x8003408B, 0x408C0009, 0x408E408D, 0x4090408F, 0x40924091, 0x40944093,
    (s32)0x80004095, 0, 0x41000000, (s32)0x80034101, 0x41020008, 0x41044103, 0x41064105,
    0x41084107, 0x410A4109, (s32)0x8003410B, 0x410C0009, 0x410E410D, 0x4110410F, 0x41124111,
    0x41144113, (s32)0x80004115, 0, 0x41000000, 0x41024101, (s32)0x80034103, 0x41040008,
    0x41064105, 0x41084107, 0x410A4109, 0x410C410B, 0x98003, 0x410E410D, 0x4110410F,
    (s32)0x80004111, 0, 0x41000000, 0x41024101, (s32)0x80034103, 0x41040008, 0x41064105,
    0x41084107, 0x410A4109, 0x410C410B, (s32)0x8003410D, 0x410E0009, 0x4110410F, 0x41124111,
    (s32)0x80004113, 0, 0x41000000, (s32)0x80034101, 0x41020008, 0x41044103, 0x41064105,
    0x41084107, 0x410A4109, 0x98003, 0x410C410B, 0x410E410D, 0x4110410F, 0x41124111,
    (s32)0x80004113, 0, 0x41000000, (s32)0x80034101, 0x41020008, 0x41044103, 0x41064105,
    0x41084107, 0x410A4109, 0x98003, 0x410C410B, 0x410E410D, 0x4110410F, 0x41124111,
    (s32)0x80004113, 0, (s32)0x80070000, 0x41800013, 0x408004, (s32)0x80070000, 0x40800015,
    0x408004, (s32)0x80070000, 0x4100001A, 0x408004, (s32)0x80070000, 0x40800014, 0x408004,
    (s32)0x80070000, 0x40800020, 0x408004, (s32)0x80070000, 0x4080001B, 0x408004,
    (s32)0x80070000, 0x4180001E, 0x408004, (s32)0x80070000, 0x4080001E, 0x408004,
    (s32)0x80070000, 0x4200001A, 0x8000, (s32)0x80070000, 0x4180005A, 0x8000, (s32)0x80070000,
    0x4280000E, 0x298007, (s32)0x8000428E, 3, (s32)0x80070000, 0x42800026, 0x298007,
    (s32)0x800042A6, 3, (s32)0x80070000, 0x42800025, 0xF8007, (s32)0x80004296, 3,
    (s32)0x80070000, 0x4200000D, 0x8000, (s32)0x80070000, 0x42800021, 0x2E8007,
    (s32)0x800042A1, 3, (s32)0x80070000, 0x42000021, 0x2F8007, (s32)0x80004221, 3,
    (s32)0x80070000, 0x40800009, 0x88003, 0x40884088, (s32)0x80004088, 0, (s32)0x80070000,
    0x42000009, 0x88003, 0x42084208, (s32)0x80004208, 0, (s32)0x80070000, 0x41800009, 0x88003,
    0x41884188, (s32)0x80004188, 0, (s32)0x80070000, 0x41800009, 0x88003, 0x41884188,
    (s32)0x80004188, 0, (s32)0x80070000, 0x4180000A, 0x88003, 0x41894189, (s32)0x80004189, 0,
    (s32)0x80070000, 0x41800009, 0x88003, 0x41884188, (s32)0x80004188, 0, (s32)0x80070000,
    0x42000009, 0x88003, 0x42084208, (s32)0x80004208, 0, (s32)0x80070000, 0x42800009, 0x88003,
    0x42884288, (s32)0x80004288, 0, (s32)0x80070000, 0x40890007, 0x88003, 0x40904090,
    (s32)0x80004090, 0, (s32)0x80070000, 0x42090007, 0x88003, 0x42104210, (s32)0x80004210, 0,
    (s32)0x80070000, 0x41890006, 0x88003, 0x418F418F, (s32)0x8000418F, 0, (s32)0x80070000,
    0x41890006, 0x88003, 0x418F418F, (s32)0x8000418F, 0, (s32)0x80070000, 0x418A0009, 0x88003,
    0x41934193, (s32)0x80004193, 0, (s32)0x80070000, 0x41890008, 0x88003, 0x41914191,
    (s32)0x80004191, 0, (s32)0x80070000, 0x42090008, 0x88003, 0x42114211, (s32)0x80004211, 0,
    (s32)0x80070000, 0x42890008, 0x88003, 0x42914291, (s32)0x80004291, 0, 0x40910000,
    (s32)0x80034092, 0x4093000C, 0x40954094, 0x40974096, 0x88003, 0x40994098, 0x409B409A,
    0x409D409C, 0x409F409E, 0x40A140A0, (s32)0x800340A2, 0x40A20009, (s32)0x800040A2, 0,
    0x42110000, (s32)0x80034212, 0x4213000C, (s32)&D_80034214, 0x42150008, 0x42174216,
    0x42194218, 0x421B421A, 0x421D421C, 0x421F421E, 0x42214220, 0x42234222, 0x42254224,
    0x98003, 0x42254225, 0x8000, 0x41900000, (s32)0x80034191, 0x4192000C, (s32)0x80034193,
    0x41940008, 0x41964195, 0x41984197, 0x419A4199, 0x419C419B, 0x419E419D, (s32)0x8003419F,
    0x419F0009, (s32)0x8000419F, 0, 0x41900000, (s32)0x80034191, 0x4192000C, (s32)0x80034193,
    0x41940008, 0x41964195, 0x41984197, 0x419A4199, 0x419C419B, 0x419E419D, (s32)0x8003419F,
    0x419F0009, (s32)0x8000419F, 0, 0x41940000, 0x41964195, (s32)0x80034197, 0x41980008,
    0x419A4199, 0x419C419B, 0x419E419D, 0x41A0419F, 0x41A241A1, 0x41A441A3, 0x98003,
    0x41A441A4, (s32)0x800041A4, 0, 0x41910000, 0x41934192, 0x41954194, (s32)0x80034196,
    0x41970008, 0x41994198, 0x419B419A, 0x419D419C, 0x419F419E, 0x41A141A0, 0x41A341A2,
    0x41A541A4, (s32)0x800341A6, 0x41A60009, (s32)0x800041A6, 0, 0x42120000, (s32)0x80034213,
    0x4214000C, 0x42164215, (s32)0x80034217, 0x42180008, 0x421A4219, 0x421C421B, 0x421E421D,
    0x4220421F, 0x42224221, 0x42244223, 0x42264225, (s32)0x80034227, 0x42270009,
    (s32)0x80004227, 0, 0x42920000, (s32)0x80034293, 0x4294000C, 0x42964295, 0x88003,
    0x42984297, 0x429A4299, 0x429C429B, 0x429E429D, 0x42A0429F, 0x42A242A1, 0x42A442A3,
    0x42A642A5, (s32)0x800342A7, 0x42A70009, (s32)0x800042A7, 0,
};
s32 D_800B9CCC[3] = { (s32)0x80110000, 0x40000010, 0x8001 };
s32 D_800B9CD8[3] = { (s32)&D_800B9CCC, 0, 0 };
ScriptInit D_800B9CE4 = { 0, (void *)&D_800B9CD8, 0x401 };
s32 D_800B9CF0[2] = { 0x17001C, 0xFFFF };
s32 D_800B9CF8[6] = { (s32)&D_800B32B8, (s32)&D_800B32C0, (s32)&D_800B32C8, (s32)&D_800B32D0, (s32)&D_800B32D4, 0 };
s32 D_800B9D10[1] = { -0xFFAD };
s32 D_800B9D14[16] = {
    0x51, (s32)&D_800B9CF8, (s32)&D_800B9D10, 0, 0, 0, 0, 0, 0x52, (s32)&D_800B9CF8,
    (s32)&D_800B9D10, 0, 0, 0, 0, 0,
};
s32 D_800B9D54[20] = {
    (s32)&D_80055684, (s32)&D_8004EBB8, (s32)&D_8004EBB0, (s32)&D_800555A4, (s32)&D_8004EB98,
    (s32)&D_8004EB90, (s32)&D_80053074, (s32)&D_8004EBA8, (s32)&D_8004EBA0, (s32)&D_80052F5C,
    (s32)&D_8004EB88, (s32)&D_8004EB80, (s32)&D_80053C7C, (s32)&D_8004EB78, (s32)&D_8004EB70,
    0, 0, 0, (s32)&D_80050064, (s32)&D_800B9D54,
};
s32 D_800B9DA4[2] = { 0x20D0003, 0x1A020201 };
s32 D_800B9DAC[19] = {
    (s32)&D_800B9DA4, 4, (s32)&D_800B9CF0, (s32)&D_800B9D14, (s32)&D_800B9DAC, 0, 0, 0, 0,
    0x2000, 0x10, 0x1000, 0x80, 0x200, 0x190, (s32)&D_80063EC8, (s32)&D_80063BB8, 0x20, 0x400,
};
u8 D_800B9DF8[20] = { 0, 0, 0x54, 0, 0x55, 0, 0x56, 0, 0x57, 0, 0x5A, 0, 0x5B, 0, 0x58, 0, 0x59, 0, 0, 0 };
s32 D_800B9E0C = 0x258;
s32 D_800B9E10[4] = { 0x258, 0x258, 0x1C2, 0x12C };
s32 D_800B9E20[4] = { 0x733, 0x734, 0x735, 0x736 };
s32 D_800B9E30[4] = { 0x71D, 0x71E, 0x71F, 0x720 };
s32 D_800B9E40[4] = { 0x3F5, 0x3F6, 0x3F7, 0x3F8 };
BonkEntry D_800B9E50[10] = {0};
BonkEnt D_800B9EC8[3] = {0};
s32 D_800B9EEC = 0;
s32 D_800B9EF0 = 0;
s32 D_800B9EF4 = 0;
s32 D_800B9EF8 = 0;
s32 D_800B9EFC = 0;
s32 D_800B9F00 = 0;
s32 D_800B9F04 = 0;
s32 D_800B9F08 = 0;

void InitialiseBONKFirst() {
    D_8009D6E8.unk4 = D_800B9DF8;
    D_8009D6E8.unk2C = BonkHeroFacing;
    D_8009D6E8.unk68 = BONK_KeepInBounds;
    D_8009D6E8.unk20 = BonkPreMoveHero;
    D_8009D6E8.unk28 = BonkHeroPostMove;
    D_8009D6E8.unk54 = BonkHeroKickCollision;
    D_8009D6E8.unk3C = BonkHeroTestSwing;
    D_8009D6E8.unk48 = D_8008AB48;
    D_8009D6E8.unk44 = D_8008AB04;
    D_8009D6E8.unk5C = BonkHero_ExtraEffects;
    D_8009D6E8.unk64 = BonkChooseWinner;
    D_8009D6E8.unk38 = Bonk_HeroBot;
    D_8009E0D8 = 0x42;
    D_8009E0E4 = 0x47;
    D_8009E0E6 = 0x48;
    BonkInitTweakAi();
}

void BonkInitEntries(void) {
    s32 i;

    D_800B9EF0 = 0;
    D_800B9EFC = 0;
    D_800B9F04 = 0;
    D_800B9F00 = D_800B9E0C;
    for (i = 0; i < 10; i++) {
        D_800B9E50[i].unk0 = 0x100;
        D_800B9E50[i].unk4 = 0;
        D_800B9E50[i].unk8 = 0;
    }
    D_800B9EF4 = 0;
    D_800B9EF8 = D_800B9634 * 30;
}

void BonkCrateMaster(void) {
    s32 i;
    s32 *new_var;

    if (D_800B9EFC > 0) {
        D_800B9EFC -= 1;
    }
    if (D_800B9F00 > 0 && D_800B9F04 < 4) {
        D_800B9F00 -= 1;
    }
    if (D_800B9EF0 != 0) {
        for (i = 0; i < 10; i++) {
            NAS *n = (NAS *)D_800B9E50[i].unk4;
            if (n != 0 && n->UserInfo.status == 1) {
                KillNAS(n);
                D_800B9E50[i].unk4 = 0;
            }
        }
    }
    if (D_800B9EF4 < 4) {
        if (D_800B9EF8 != 0) {
            D_800B9EF8--;
        } else {
            D_800B9EF4++;
            D_800B9EF8 = (&D_800B9634)[D_800B9EF4] * 30;
        }
    }
    if (D_800B9EF0 < (new_var = &D_800B95A8)[D_800B9EF4] && D_800B9EFC == 0) {
        BonkSpawnCrate();
        D_800B9EFC = 0x14;
    }
}


void PlotBonkStool(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 x;
    s32 z;
    SLONG_XYZ pos;
    SLONG_XYZ *v;
    s32 *p;
    s32 *q;

    if (e == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        if (BonkAI_PickEntry(&x, &z) != 0) {
            D_800B9E50[nas->UserInfo.user2].unk8 = 1000;
            e->unk10.x = x;
            e->unk10.z = z;
            e->unk10.y = World_floor_level - 0x2000;
            BonkKeepInBoundsB(e);
            nas->UserInfo.status = 2;
            if (nas->UserInfo.user1 == 0) {
                D_800B9E50[nas->UserInfo.user2].unk8 = D_800B9300[D_800B9EEC];
                nas->UserInfo.user1 = Random(0x14) + 10;
            } else {
                D_800B9E50[nas->UserInfo.user2].unk8 = 0;
            }
            e->flags &= 0xFFFF7FFF;
            if (D_800B9F00 == 0) {
                e->sub->unk74 = 0x2015;
                p = &D_800B9E0C;
                D_800B9F00 = p[++D_800B9F04];
            } else {
                e->sub->unk74 = 0x2002;
            }
        }
        break;
    case 2:
        if (!(D_8005A624 & 0x4000000)) {
            nas->UserInfo.user1--;
        }
        if (nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 10;
            e->unk5E = 1;
            e->unk2C.y = 0;
            e->flags |= 0x8000;
            e->unk10.y = World_floor_level;
            if (e->sub->unk74 == 0x2015) {
                PlaySfx(0x72A, NULL, &e->unk10, 0x1000, 0x1E00, 0);
            } else {
                PlaySfx(0x528, NULL, &e->unk10, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 10:
        e->unk2C.y += 0x100;
        if (e->unk2C.y > 0xC00) {
            e->unk2C.y = 0xC00;
            nas->UserInfo.status = 12;
            nas->UserInfo.user1 = D_800B95C4[D_800B9EF4] + Random(D_800B95E0[D_800B9EF4]);
            if (e->sub->unk74 == 0x2015) {
                pos = e->unk10;
                pos.y -= 0x100;
                Bonk_SpawnPuff((s32 *)&pos, 3, D_80053C7C);
            }
        }
        e->unk5E = 1;
        break;
    case 12:
        if (D_800B9E50[nas->UserInfo.user2].unk8 != 0) {
            D_800B9E50[nas->UserInfo.user2].unk8--;
        }
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 13;
            nas->UserInfo.user1 = 70;
        }
        break;
    case 13:
    case 14:
        if (D_800B9E50[nas->UserInfo.user2].unk8 != 0) {
            D_800B9E50[nas->UserInfo.user2].unk8--;
        }
        if (--nas->UserInfo.user1 <= 0) {
            if (nas->UserInfo.status == 13) {
                nas->UserInfo.status = 14;
                nas->UserInfo.user1 = 40;
            } else {
                nas->UserInfo.status = (D_8005A624 & 0x4000000) ? 4 : 5;
            }
            break;
        }
        if (nas->UserInfo.status == 13) {
            e->unk2C.y = Random(0x200) + 0xB00;
        } else {
            e->unk2C.y = Random(0x800) + 0x800;
        }
        e->unk5E = 1;
        break;
    case 5:
        Bonk_SpawnPuff((s32 *)&e->unk10, 0, D_80053C7C);
        v = &e->unk10;
        BonkMalletHit((VECTOR *)v, 0x380, -1, 0);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        PlaySfx(D_800B9E20[Random(4)], NULL, v, 0x1000, 0x1E00, 0);
        break;
    case 4:
        Bonk_SpawnPuff((s32 *)&e->unk10, 1, D_80053C7C);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        break;
    case 6:
        p = &D_800B9EF0;
        q = &D_800B95A8;
        if (*p > q[D_800B9EF4]) {
            nas->UserInfo.status = 1;
            (*p)--;
            e->unk10.y = World_floor_level - 0x2000;
        } else {
            nas->UserInfo.status = 0;
            nas->UserInfo.user1 = 0;
        }
        break;
    }
}

s32 BonkAI_PickEntry(s32 *arg0, s32 *arg1) {
    s32 tries;
    s32 ok;
    s32 x;
    s32 z;
    s32 i;
    HeroInfo *h;
    BonkEntry *e;
    BonkEnt *f;
    EXTengine *eng;

    HeroInfo *heroes;

    tries = 0;
    heroes = Heroes;
    do {
        ok = 1;
        x = Random(0x1000) - 0x800;
        z = Random(0x1000) - 0x800;
        for (i = 0, h = heroes; i < 4; i++, h++) {
            eng = h->hero_eng;
            if ((csqrt(BonkDist2XZ(eng->unk10.x, eng->unk10.z, x, z)) >> 6) < 0x400) {
                ok = 0;
            }
        }
        if (ok) {
            if (D_800B9EF0 != 0) {
                for (i = 0, e = D_800B9E50; i < 10; i++, e++) {
                    if (e->unk4 != 0) {
                        eng = ((NAS *)e->unk4)->Engine;
                        if ((csqrt(BonkDist2XZ(eng->unk10.x, eng->unk10.z, x, z)) >> 6) < 0x400) {
                            ok = 0;
                        }
                    }
                }
            }
            if (ok && D_800B9F08 != 0) {
                for (i = 0, f = D_800B9EC8; i < 3; i++, f++) {
                    if (f->b != 0) {
                        eng = ((NAS *)f->b)->Engine;
                        if ((csqrt(BonkDist2XZ(eng->unk10.x, eng->unk10.z, x, z)) >> 6) < 0x300) {
                            ok = 0;
                        }
                    }
                }
            }
        }
        tries++;
        if (ok) {
            *arg0 = x;
            *arg1 = z;
            return 1;
        }
    } while (tries < 10);
    return 0;
}

void InitBonkCrate(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        subengine *s;

        e->unk5C = 1;
        e->unk2C.z = 0;
        e->unk2C.y = 0;
        e->unk2C.x = 0;
        e->unk5E = 1;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = D_800B9F08 * 0x5A + 0x1E;
        s = e->sub;
        e->flags &= 0xFFFF7FFF;
        if (s != NULL) {
            s->unk6C = D_80053C7C;
            s->unk74 = 0x200F;
        } else {
            nas->Failure = 1;
        }
        if (AddObject(nas) != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
        } else {
            nas->Failure = 1;
        }
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        nas->UserInfo.user2 = D_800B9F08;
        D_800B9EC8[D_800B9F08].b = (s32)nas;
        D_800B9F08 += 1;
    }
}

void PlotBonkCrate(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 x;
    s32 z;
    s32 s;
    SLONG_XYZ *v;

    if (e == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        e->unk10.y = World_floor_level - 0x2000;
        if (nas->UserInfo.user1 == 0) {
            D_800B9EC8[nas->UserInfo.user2].c = D_800B93A0[D_800B9EEC];
            nas->UserInfo.user1 = Random(0x5A) + 0x5A;
        } else {
            D_800B9EC8[nas->UserInfo.user2].c = 0;
        }
        e->flags &= 0xFFFF7FFF;
        e->unk1E = Random(0x1000);
        nas->UserInfo.status = 2;
        break;
    case 2:
        if (!(D_8005A624 & 0x4000000)) {
            if (nas->UserInfo.user1 > 0) {
                nas->UserInfo.user1--;
            } else {
                goto spawn;
            }
        }
        if (nas->UserInfo.user1 > 0) {
            break;
        }
    spawn:
        if (BonkAI_PickEntry(&x, &z) != 0) {
            e->unk10.x = x;
            e->unk10.z = z;
            e->unk10.y = World_floor_level;
            BonkKeepInBoundsB(e);
            nas->UserInfo.status = 10;
            e->unk2C.z = 0;
            e->unk2C.x = 0;
            e->unk2C.y = 0;
            e->unk5E = 1;
            e->flags |= 0x8021;
        }
        break;
    case 10:
        e->unk2C.x += 0x100;
        if (e->unk2C.y >= 0x1000) {
            e->unk2C.x = 0x1000;
            nas->UserInfo.status = 12;
            e->flags &= ~0x21;
        }
        s = e->unk2C.x;
        e->unk5E = 1;
        e->unk2C.y = s;
        e->unk2C.z = s;
        break;
    case 12:
        if (D_800B9EC8[nas->UserInfo.user2].c > 0) {
            D_800B9EC8[nas->UserInfo.user2].c--;
        }
        break;
    case 5:
        Bonk_SpawnPuff((s32 *)&e->unk10, 0, D_80053C7C);
        v = &e->unk10;
        BonkMalletHit((VECTOR *)v, 0x380, -1, 0);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        PlaySfx(D_800B9E20[Random(4)], NULL, v, 0x1000, 0x1E00, 0);
        break;
    case 4:
        Bonk_SpawnPuff((s32 *)&e->unk10, 2, D_80053C7C);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        break;
    case 6:
        nas->UserInfo.user1 = 0;
        e->unk10.y = World_floor_level - 0x2000;
        nas->UserInfo.status = 0;
        break;
    }
}

s32 BonkAI_FindClosest(s32 arg0, s32 *arg1, s32 arg2) {
    BonkEntry *found;
    s32 x;
    s32 z;
    s32 ex;
    s32 ez;
    s32 d;
    s32 best;
    s32 i;
    BonkEntry *e;
    BonkEnt *f;

    found = 0;
    if (arg2 & 2) {
        best = D_800B9580[Heroes[arg0].hero_name];
    } else {
        best = 0xFFFFFF;
    }
    x = Heroes[arg0].hero_eng->unk10.x;
    z = Heroes[arg0].hero_eng->unk10.z;
    if ((arg2 & 0x100) && D_800B9EF0 != 0) {
        for (i = 0, e = D_800B9E50; i < 10; i++, e++) {
            if (e->unk4 == 0) {
                continue;
            }
            if (arg2 & 1) {
                if (((BonkPad *)e->unk4)->mode < 9) {
                    continue;
                }
                if (e->unk8 != 0) {
                    continue;
                }
            }
            if (((BonkPad *)e->unk4)->mode < 0xB) {
                continue;
            }
            ex = ((BonkPad *)e->unk4)->eng->unk10.x;
            ez = ((BonkPad *)e->unk4)->eng->unk10.z;
            d = csqrt(BonkDist2XZ(x, z, ex, ez));
            d >>= 6;
            if (d < best) {
                if (!(arg2 & 4) || func_80015480(get_angle(x - ex, z - ez), Heroes[arg0].hero_move->forward_angle) < 0x320) {
                    BONK_SET_CLOSEST(best, found, d, e);
                }
            }
        }
    }
    if (arg2 & 0x200) {
        i = 0;
        if (D_800B9F08 != 0)
        for (f = D_800B9EC8; i < 3; i++, f++) {
            if (f->b == 0) {
                continue;
            }
            if (arg2 & 1) {
                if (((BonkPad *)f->b)->mode < 9) {
                    continue;
                }
                if (f->c != 0) {
                    continue;
                }
            }
            if (((BonkPad *)f->b)->mode < 0xB) {
                continue;
            }
            ex = ((BonkPad *)f->b)->eng->unk10.x;
            ez = ((BonkPad *)f->b)->eng->unk10.z;
            d = csqrt(BonkDist2XZ(x, z, ex, ez));
            d >>= 6;
            if (d < best) {
                if (!(arg2 & 4) || func_80015480(get_angle(x - ex, z - ez), Heroes[arg0].hero_move->forward_angle) < 0x320) {
                    BONK_SET_CLOSEST(best, found, d, (BonkEntry *)f);
                }
            }
        }
    }
    if (found != 0) {
        *arg1 = (s32)found;
        return 1;
    }
    return 0;
}

void BonkHitTarget(s32 arg0, s32 arg1, BonkTgt2 *t) {
    BonkPad *p;
    EXTengine *e;
    s32 big;

    p = t->obj;
    if (p != NULL) {
        e = p->eng;
        big = 0;
        if (e != NULL) {
            big = ((subengine *)e->sub)->unk74 == 0x2015;
        }
        if (arg0 >= 0) {
            switch (p->mode) {
            case 0xE:
                BonkKnockBack(arg0, e, big ? 0x14 : 5);
                break;
            case 0xD:
                BonkKnockBack(arg0, e, big ? 0xF : 3);
                break;
            default:
                BonkKnockBack(arg0, e, big ? 0xA : 2);
                break;
            }
            PlaySfx(D_800B9E30[arg0], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            if (big) {
                PlaySfx(0x72B, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
        p->mode = 4;
    }
}

void BonkMalletHit(VECTOR *pos, s32 a, s32 b, s32 c) {
    s32 x;
    s32 z;
    s32 ex;
    s32 ez;
    s32 ang;
    s32 i;
    HeroInfo *h;
    BonkEntry *e;
    EXTengine *eng;
    EXTheromove *mv;
    s32 vc;

    x = pos->vx;
    z = pos->vz;
    if (D_8005A644 & 0x801) {
        return;
    }
    i = 0;
    do {
        h = &Heroes[i];
        if (i == b) {
            goto next;
        }
        ex = h->hero_eng->unk10.x;
        ez = h->hero_eng->unk10.z;
        if ((csqrt(BonkDist2XZ(x, z, ex, ez)) >> 6) > a) {
            goto next;
        }
        if ((h->hero_immunity | h->hero_hit) != 0) {
            goto next;
        }
        ang = get_angle(x - ex, z - ez);
        mv = h->hero_move;
        vc = mv->velocity_calc;
        if (vc != 1) {
            if (vc != 0) {
                if (vc < 4) {
                    mv->x_vel -= (D_80068BD4[(ang - 0x400) & 0xFFF].unk2 * 5) >> 8;
                    h->hero_move->z_vel -= (D_80068BD4[ang & 0xFFF].unk2 * 5) >> 8;
                }
            }
        }
        h->hero_move->reactive_angle = ang;
        h->hero_move->reactive_speed = 0x50;
        func_80084E04(i, NULL, -1, 0x8001, 0x446);
        h->hero_health = 0x14;
        func_80028018(i, 0xF);
        BonkHidePowerup(i);
        PlaySfx(D_8009E080[h->hero_name][Random(4)], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
    next:
        i++;
    } while (i < 4);
    if ((c & 0x100) && D_800B9EF0 != 0) {
        i = 0;
        do {
            e = &D_800B9E50[i];
            if (e->unk4 != 0) {
                eng = ((NAS *)e->unk4)->Engine;
                if ((csqrt(BonkDist2XZ(x, z, eng->unk10.x, eng->unk10.z)) >> 6) <= a
                    && ((BonkPad *)e->unk4)->mode >= 0xB) {
                    BonkHitTarget(b, 1, (BonkTgt2 *)e);
                }
            }
            i++;
        } while (i < 10);
    }
}

void BonkChooseWinnerScore(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    u8 *g;

    if (arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (i = 0; i < 4; i++) {
                if (!((taken >> i) & 1)) {
                    if (Heroes[i].hero_score > Heroes[best].hero_score || best == -1) {
                        best = i;
                    }
                }
            }
            D_8005A614[j + 0xA7] = best;
            taken |= 1 << best;
        }
        g = D_8005A614;
        if (*(s32 *)(g + 0x6C) == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g[g[0xA7] + 0x5C] != 0
                || (hb = Heroes, a = &hb[g[0xA7]], a->hero_score == hb[g[0xA8]].hero_score)) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else {
                D_8005A614[0x60] = a->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (D_8005A614[0xA7] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x400;
                    }
                }
            }
        } else {
            HeroInfo *a;
            HeroInfo *b;
            HeroInfo *ta;
            HeroInfo *tb;
            HeroInfo *hb;
            s32 sa;
            s32 sb;

            InitBigScore();
            hb = Heroes;
            a = &hb[*(s16 *)(g + 0x48)];
            b = &hb[*(s16 *)(g + 0x4A)];
            ta = &hb[a->hero_teammate];
            tb = &hb[b->hero_teammate];
            sa = a->hero_score + ta->hero_score;
            sb = b->hero_score + tb->hero_score;
            if (sa == sb) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if (sb < sa) {
                *(u16 *)(g + 0x30) |= 0x800;
                if (g[*(s16 *)(g + 0x48) + 0x5C] == 0) {
                    g[0x60] = a->hero_name;
                    g[0x61] = ta->hero_name;
                    a->hero_status |= 0x200;
                    ta->hero_status |= 0x200;
                    b->hero_status |= 0x400;
                    tb->hero_status |= 0x400;
                } else {
                    g[0x60] = b->hero_name;
                    g[0x61] = tb->hero_name;
                    b->hero_status |= 0x200;
                    tb->hero_status |= 0x200;
                    a->hero_status |= 0x400;
                    ta->hero_status |= 0x400;
                }
            } else {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            }
        }
    }
}

void BonkChooseWinnerScoreB(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    u8 *g;

    if (arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (i = 0; i < 4; i++) {
                if (!((taken >> i) & 1)) {
                    if (Heroes[i].hero_score > Heroes[best].hero_score || best == -1) {
                        best = i;
                    }
                }
            }
            D_8005A614[j + 0xA7] = best;
            taken |= 1 << best;
        }
        g = D_8005A614;
        if (*(s32 *)(g + 0x6C) == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g[g[0xA7] + 0x5C] != 0
                || (hb = Heroes, a = &hb[g[0xA7]], a->hero_score == hb[g[0xA8]].hero_score)) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else {
                D_8005A614[0x60] = a->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (D_8005A614[0xA7] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x400;
                    }
                }
            }
        } else {
            HeroInfo *a;
            HeroInfo *b;
            HeroInfo *ta;
            HeroInfo *tb;
            HeroInfo *hb;
            s32 sa;
            s32 sb;

            InitBigScore();
            hb = Heroes;
            a = &hb[*(s16 *)(g + 0x48)];
            b = &hb[*(s16 *)(g + 0x4A)];
            ta = &hb[a->hero_teammate];
            tb = &hb[b->hero_teammate];
            sa = a->hero_score + ta->hero_score;
            sb = b->hero_score + tb->hero_score;
            if (sa == sb) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if (sb < sa) {
                *(u16 *)(g + 0x30) |= 0x800;
                if (g[*(s16 *)(g + 0x48) + 0x5C] == 0) {
                    g[0x60] = a->hero_name;
                    g[0x61] = ta->hero_name;
                    a->hero_status |= 0x200;
                    ta->hero_status |= 0x200;
                    b->hero_status |= 0x400;
                    tb->hero_status |= 0x400;
                } else {
                    g[0x60] = b->hero_name;
                    g[0x61] = tb->hero_name;
                    b->hero_status |= 0x200;
                    tb->hero_status |= 0x200;
                    a->hero_status |= 0x400;
                    ta->hero_status |= 0x400;
                }
            } else {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            }
        }
    }
}

void BonkChooseWinnerTimed(s32 arg0, u16 arg1) {
    s32 found;
    s32 fail;
    u8 *g;
    s32 i;

    found = -1;
    fail = 0;
    g = D_8005A614;
    if (*(s32 *)(g + 0x6C) != 0) {
        HeroInfo *hb = Heroes;
        HeroInfo *a;
        HeroInfo *ta;
        HeroInfo *b;
        s32 sa;
        s32 sb;

        a = &hb[*(s16 *)(g + 0x48)];
        ta = &hb[a->hero_teammate];
        b = &hb[*(s16 *)(g + 0x4A)];
        sa = a->hero_score + ta->hero_score;
        sb = b->hero_score + hb[b->hero_teammate].hero_score;
        if (sa == 0) {
            if (g[*(s16 *)(g + 0x48) + 0x5C] == 0) {
                D_8005A614[0x60] = a->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (D_8005A614[i + 0x5C] != 0) {
                        hb[i].hero_status |= 0x400;
                    } else {
                        hb[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        } else if (sb == 0) {
            if (g[*(s16 *)(g + 0x4A) + 0x5C] == 0) {
                D_8005A614[0x60] = b->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (D_8005A614[i + 0x5C] != 0) {
                        hb[i].hero_status |= 0x400;
                    } else {
                        hb[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (!Heroes[i].hero_score) {
                found = i;
            }
        }
        if (found != -1) {
            if (D_8005A614[found + 0x5C] == 0) {
                D_8005A614[0x60] = Heroes[found].hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (D_8005A614[i + 0x5C] != 0) {
                        Heroes[i].hero_status |= 0x400;
                    } else {
                        Heroes[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        }
    }
    if (fail || arg0 == -1) {
        D_8005A614[0x60] = 0;
        *(u16 *)(D_8005A614 + 0x30) |= arg1;
        Clock_flags |= 0x200;
        for (i = 0; i < 4; i++) {
            if (D_8005A614[i + 0x5C] != 0) {
                Heroes[i].hero_status |= 0x400;
            } else {
                Heroes[i].hero_status |= 0x100;
            }
        }
    }
}

void BonkChooseWinnerRank(s32 arg0, u16 arg1) {
    BonkRankView *g;
    BonkRankView *p;
    HeroInfo *hb;
    HeroInfo *h;
    HeroInfo *top;
    HeroInfo *ta;
    HeroInfo *b;
    HeroInfo *am;
    HeroInfo *bm;
    s32 used;
    s32 i;
    s32 k;
    s32 n;
    s32 j;
    s32 best;
    s32 sa;
    s32 sb;

    if (arg0 != -1) {
        return;
    }
    used = 0;
    for (i = 0; i < 4; i++) {
        best = -1;
        for (j = 0; j < 4; j++) {
            if (!((used >> j) & 1) && (Heroes[j].hero_score > Heroes[best].hero_score || best == -1)) {
                best = j;
            }
        }
        RANK66V->order[i] = best;
        used |= 1 << best;
    }
    g = RANK66V;
    if (g->teams == 0) {
        h = Heroes;
        top = &h[g->order[0]];
        if (top->hero_score == h[g->order[1]].hero_score) {
            g->winner[0] = 0;
            g->unk30 |= arg1;
            for (k = 0; k < 4; k++) {
                Heroes[k].hero_status |= 0x100;
            }
            return;
        }
        p = g;
        p->winner[0] = top->hero_name;
        p->unk30 |= 0x800;
        for (n = 0; n < 4; n++) {
            if (p->order[0] == n) {
                h[n].hero_status |= 0x200;
            } else {
                h[n].hero_status |= 0x400;
            }
        }
        return;
    }
    InitBigScore();
    hb = Heroes;
    ta = &hb[g->team[0]];
    b = &hb[g->team[1]];
    am = &hb[ta->hero_teammate];
    bm = &hb[b->hero_teammate];
    sa = ta->hero_score + am->hero_score;
    sb = b->hero_score + bm->hero_score;
    if (sa == sb) {
        g->winner[0] = 0;
        g->unk30 |= arg1;
        for (k = 0; k < 4; k++) {
            Heroes[k].hero_status |= 0x100;
        }
        return;
    }
    g->unk30 |= 0x800;
    if (sb < sa) {
        g->winner[0] = ta->hero_name;
        g->winner[1] = am->hero_name;
        ta->hero_status |= 0x200;
        am->hero_status |= 0x200;
        b->hero_status |= 0x400;
        bm->hero_status |= 0x400;
    } else {
        RANK66V->winner[0] = b->hero_name;
        RANK66V->winner[1] = bm->hero_name;
        b->hero_status |= 0x200;
        bm->hero_status |= 0x200;
        ta->hero_status |= 0x400;
        am->hero_status |= 0x400;
    }
}

void Bonk_HeroBot(s32 arg0)
{
    u8 *g = D_8005A614;
    HeroInfo *h;
    HeroInfo *heroes;
    EXTbotai *ai;
    s32 sx;
    s32 sz;
    HeroInfo *me;
    u32 i;
    BonkHit *hit;
    u16 mode;
    u16 ang;

    if (*(u32 *)(g + 0x10) & 0x04000000) {
        return;
    }
    heroes = Heroes;
    h = &heroes[arg0];
    ai = h->hero_ai;
    if (ai == NULL) {
        return;
    }
    if (ai->AItimer1 != 0) {
        ai->AItimer1--;
    }
    ai->AIpad = 0;
    if (BonkAI_Closest(arg0, (int *)&hit) != 0) {
        if (hit->nas != ai->pTargetNas && ai->AItimer1 == 0) {
            ai->pTargetNas = hit->nas;
            ai->TargetXYZ = hit->nas->Engine->unk10;
            ai->AItimer1 = 10;
            if (hit->kind == 0x100) {
                if (D_800B9F4C[arg0].f8 == 1 || (*(u16 *)(g + 0x32) & 0x80) ||
                    BonkAI_NearestEnemy(arg0) != 0) {
                    ai->AImode = 2;
                } else {
                    mode = 3;
                    if (D_8004E0D8 & 1) {
                        mode = 2;
                    }
                    ai->AImode = mode;
                }
            } else {
                if (D_800B9F4C[arg0].f8 == 1 || (*(u16 *)(g + 0x32) & 0x80) ||
                    BonkAI_NearestEnemy(arg0) != 0) {
                    ai->AImode = 4;
                } else {
                    mode = 5;
                    if (D_8004E0D8 & 1) {
                        mode = 4;
                    }
                    ai->AImode = mode;
                }
            }
        }
    } else {
        sz = 0;
        sx = 0;
        i = 0;
        me = h;
        while (1) {
            if (i < 4) {
                if (i != arg0) {
                    if ((csqrt(bonk_sq(me->hero_eng->unk10.x, me->hero_eng->unk10.z,
                                        heroes[i].hero_eng->unk10.x, heroes[i].hero_eng->unk10.z)) >> 6) < 0x1000) {
                        sx += me->hero_eng->unk10.x - heroes[i].hero_eng->unk10.x;
                        sz += me->hero_eng->unk10.z - heroes[i].hero_eng->unk10.z;
                    }
                }
                i++;
                continue;
            }
            break;
        }
        if (sx != 0 || sz != 0) {
            ang = get_angle(sx, sz);
            ai->AIang = ang;
            ai->AIpad |= D_80051244[(ang >> 9) & 7];
            ai->AImode = 1;
        }
    }
    switch (ai->AImode) {
    case 1:
        break;
    case 2:
    case 3:
        if ((csqrt(bonk_sq(ai->pTargetNas->Engine->unk10.x, ai->pTargetNas->Engine->unk10.z,
                            Heroes[arg0].hero_eng->unk10.x, Heroes[arg0].hero_eng->unk10.z)) >> 6) <
            D_800B9580[Heroes[arg0].hero_name]) {
            if ((ai->AImode == 3 && Heroes[arg0].hero_mode != 0x44) ||
                (ai->AImode == 2 && Heroes[arg0].hero_mode != 0x43)) {
                ai->AIpad |= 0x8000;
            }
        }
        goto face;
    case 4:
    case 5:
        if ((csqrt(bonk_sq(ai->pTargetNas->Engine->unk10.x, ai->pTargetNas->Engine->unk10.z,
                            Heroes[arg0].hero_eng->unk10.x, Heroes[arg0].hero_eng->unk10.z)) >> 6) <
            D_800B9580[Heroes[arg0].hero_name]) {
            if ((ai->AImode == 5 && Heroes[arg0].hero_mode != 0x44) ||
                (ai->AImode == 4 && Heroes[arg0].hero_mode != 0x43)) {
                ai->AIpad |= 0x8000;
            }
        }
    face:
        ang = get_angle(ai->pTargetNas->Engine->unk10.x - Heroes[arg0].hero_eng->unk10.x,
                        ai->pTargetNas->Engine->unk10.z - Heroes[arg0].hero_eng->unk10.z);
        ai->AIang = ang;
        ai->AIpad |= D_80051244[(ang >> 9) & 7];
        break;
    }
    set_joypad((u8 *)&D_80051338[arg0], ai->AIpad);
}

s32 BonkAI_NearestEnemy(s32 arg0)
{
  s32 new_var2;
  EXTengine *b;
  EXTengine *a;
  s32 dx;
  s32 dz;
  s32 new_var;
  s32 r;
  if ((*D_8005A680) != 0)
  {
    HeroInfo *t = &Heroes[Heroes[arg0].hero_teammate];
    a = Heroes[arg0].hero_eng;
    b = t->hero_eng;
    new_var = a->unk10.z;
    new_var2 = a->unk10.x;
    dx = b->unk10.x - new_var2;
    dz = b->unk10.z - new_var;
    r = (csqrt((dx * dx) + (dz * dz)) >> 6) < 0x501;
  }
  else
  {
    r = 0;
  }
  return r;
}

int BonkAI_Closest(int a0, int *out)
{
    int v;

    if (BonkAI_FindClosest(a0, &v, 0x301) != 0) {
        *out = v;
        return 1;
    }
    return 0;
}

void BonkAI_Reset(s32 arg0) {
    EXTbotai *ai = Heroes[arg0].hero_ai;

    if (ai != NULL) {
        ai->AImode = 1;
        ai->AItimer1 = 0;
    }
}

void BonkChooseWinner(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            BonkChooseWinnerScore(arg0, (u16)arg1);
        } else if (f & 0x20) {
            BonkChooseWinnerScoreB(arg0, (u16)arg1);
        } else if (f & 0x40) {
            BonkChooseWinnerTimed(arg0, (u16)arg1);
        } else {
            BonkChooseWinnerRank(arg0, (u16)arg1);
        }
    }
}

s32 BonkDist2XZ(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_a0;
    s32 temp_a1;

    temp_a0 = arg2 - arg0;
    temp_a1 = arg3 - arg1;
    return (temp_a0 * temp_a0) + (temp_a1 * temp_a1);
}

void PlotBonkFlag(NAS *nas) {
    func_800250B0(nas);
}

void InitBonkFlag(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        func_80024E30(nas, &D_800B9CE4);
        e->sub->unk6C = D_80053C0C;
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBonkShockwave(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 v;

    if (e != NULL) {
        s32 a = nas->UserInfo.user1;
        s32 b = nas->UserInfo.user2;

        if (a >= b) {
            KillNAS(nas);
        } else {
            s32 t = a - 0x800;

            nas->UserInfo.user1 = a + ((b - t) >> 1);
        }
        e->unk1E += 0x71;
        v = nas->UserInfo.user1;
        e->unk5E = 1;
        e->unk2C.z = v;
        e->unk2C.x = v;
    }
}

void InitBonkShockwave(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053C7C;
        e->sub->unk74 = 0x2014;
        e->unk5C = 1;
        e->unk2C.y = 0x1000;
        e->unk5D = 0;
        e->unk2C.z = 0;
        e->unk2C.x = 0;
        e->unk5E = 1;
        e->flags |= 0x8021;
        nas->UserInfo.user1 = 0;
        nas->UserInfo.user2 = 0x4000;
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void BonkHitObject(s32 arg0, s32 arg1, BonkObj *obj) {
    BonkPad *p = obj->pad;

    if (p != NULL) {
        BonkHeroHitFlash(arg0, Random(2));
        p->mode = 4;
        PlaySfx(D_800B9E40[Random(4)], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void BonkUpdateStub(void) {
}

void BonkSpawnStool(void) {
    s32 i;

    D_800B9F08 = 0;
    for (i = 0; i < 3; i++) {
        D_800B9EC8[i].a = 0x200;
        D_800B9EC8[i].b = 0;
        D_800B9EC8[i].c = 0;
    }
    for (i = 0; i < 2; i++) {
        CreateNAS(0, 0, 0, 0x1A04, 0x11);
    }
}

void InitBonkStool(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        subengine *s;

        e->unk5C = 1;
        e->unk2C.z = 0xC00;
        e->unk2C.y = 0xC00;
        e->unk2C.x = 0xC00;
        e->unk5E = 1;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = 0;
        s = e->sub;
        e->flags &= 0xFFFF7FFF;
        if (s != NULL) {
            s->unk6C = D_80053C7C;
            s->unk74 = 0x2002;
        } else {
            nas->Failure = 1;
        }
        if (AddObject(nas) != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
        } else {
            nas->Failure = 1;
        }
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void BonkSpawnCrate(void) {
    NAS *n;

    if (D_800B9EF0 < 10) {
        n = CreateNAS(0, World_floor_level - 0x2000, 0, 0x1A01, 0x11);
        if (n != NULL) {
            n->UserInfo.user1 = Random(10) + D_800B9EF0 * 5;
            n->UserInfo.user2 = D_800B9EF0;
            D_800B9E50[D_800B9EF0].unk4 = (s32)n;
            D_800B9EF0 += 1;
        }
    }
}

void BonkUpdateSkill(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_8005A614[i + 0x5C] != 0) {
            D_800B9EEC = D_8009D87C[i][0];
        }
    }
}

void UpdateBONK(void) {
    BonkCrateMaster();
    BonkUpdateStub();
    BonkUpdateSkill();
}

void BonkInitTweakAi(void) {
    D_8009D904 = D_8005A680[0] != 0 ? D_800B92E8 : D_800B92D0;
    D_8009D8FC = D_8005A680[0] != 0 ? D_800B92B0 : D_800B929C;
    D_8009D900 = D_800B92C4;
}

void BonkSetClock(s32 arg0) {
    Set_Clock(arg0 & 0xFFFF, 0x81D0);
}

void BonkSetClockA(void) {
    if (!(D_8005A646 & 0xE0)) {
        BonkSetClock(0x130);
    }
}

void BonkSetClockB(void) {
    if (D_8005A646 & 0x20) {
        BonkSetClock(0x130);
    }
}

void BonkInitScores(void) {
    s32 i;
    s16 v;
    u8 *g = D_8005A614;

    u8 *h;

    if (*(u16 *)(g + 0x32) & 0x40) {
        i = 0;
        h = g;
        for (; i < 4; i++) {
            v = 0x3C;
            if (h[i + 0x5C] == 0) {
                v = 0x50;
                if (*(s32 *)(h + 0x6C) != 0) {
                    v = 0x5A;
                }
            }
            Heroes[i].hero_score_displayed = v;
            Heroes[i].hero_score = v;
        }
        BonkSetClock(0x130);
        InitSharedScoreIcons();
        if (D_8005A680[0] != 0) {
            PanelCtrl |= 0x2000;
        }
    }
}

void BonkSetClockC(void) {
    if (D_8005A646 & 0x80) {
        BonkSetClock(0x130);
    }
}

void BonkInitCamera(void) {
    BonkCam *cam = (BonkCam *)D_80051640;
    cam->unk0 = 0;
    cam->unk8 = 0xE98;
    cam->unk54 = 0x192;
    cam->unk18 = 0x200;
    cam->unkC = 0x60;
    cam->unk10 = 0x20;
    cam->unk4 = 0;
    cam->unk56 = 0;
    cam->unk58 = 0;
    cam->unk14 = -0x660;
}

void InitialiseBONK(void) {
    World_floor_level = -0x10;
    Init_Clock(0x140, 0x28, 0);
    Set_Clock(0x130, 0x81D0);
    BonkInitCamera();
    D_800B9EEC = 0x10;
    BonkUpdateSkill();
    BonkSetClockC();
    BonkInitScores();
    BonkSetClockB();
    BonkSetClockA();
    BonkBounceAll();
    BonkInitEntries();
    BonkSpawnStool();
}

void BONKSetUp(void) {
    D_8005AA70[0] = (s32)InitialiseBONKFirst;
    D_8005AA70[1] = (s32)InitialiseBONK;
    D_8005AA70[2] = (s32)InitialiseBONK;
    D_8005AA70[3] = (s32)UpdateBONK;
    D_8005AA70[5] = (s32)FirstTriggersBONK;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
}

void BonkRestartLevel(void) {
}

void FirstTriggersBONK(void) {
}

void BonkKnockBack(s32 arg0, EXTengine *p, s32 arg2) {
    HeroInfo *h;
    HeroInfo *t;
    HeroInfo *base, *b2, *b3, *h2;
    s32 v;
    s32 s;
    s32 x, y, z;
    u8 *g;

    if (p != NULL) {
        x = p->unk10.x;
    } else {
        x = Heroes[arg0].hero_eng->unk10.x;
    }
    if (p != NULL) {
        y = p->unk10.y;
    } else {
        y = Heroes[arg0].hero_eng->unk10.y;
    }
    if (p != NULL) {
        z = p->unk10.z;
    } else {
        z = Heroes[arg0].hero_eng->unk10.z;
    }
    func_8008D5D4(x, y, z, arg2, 0);
    g = D_8005A614;
    if (*(u16 *)(g + 0x32) & 0x40) {
        if (*(s32 *)(g + 0x6C) != 0) {
            base = Heroes;
            h = base + arg0;
            s = h->hero_score;
            if (s < arg2) {
                h->hero_score = 0;
                t = base + h->hero_teammate;
                t->hero_score = t->hero_score - (arg2 - s);
                return;
            }
        }
        b2 = Heroes;
        h2 = arg0 + b2;
        v = (u16)h2->hero_score - arg2;
    } else {
        b3 = Heroes;
        h2 = arg0 + b3;
        v = (u16)h2->hero_score + arg2;
    }
    h2->hero_score = v;
}

void BonkHeroHitFlash(s32 arg0, s32 arg1) {
    EXTengine *e;
    subengine *s;
    HeroInfo *h;
    HeroInfo *base;

    s = NULL;
    BonkHidePowerup(arg0);
    base = Heroes;
    h = &base[arg0];
    e = h->hero_powerup;
    if (e != NULL) {
        s = e->sub;
    }
    switch (arg1) {
    case 0:
        D_800B9F4C[arg0].fC = 0xB4;
        if (s != NULL) {
            s->unk6C = D_80053C7C;
            s->unk74 = 0x2013;
            s->flags |= 0x8000;
            e->flags |= 0x8000;
        }
        PlaySfx(0x41C, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        if (s != NULL) {
            s->unk6C = D_80052F5C;
            s->unk74 = 0x200C;
            s->flags |= 0x8000;
            e->flags |= 0x8000;
        }
        PlaySfx(0x72E, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    default:
        return;
    }
    D_800B9F4C[arg0].f8 = arg1;
}

void BonkHero_ExtraEffects(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + player;
    EXTengine *p = h->hero_powerup;

    if (p != NULL) {
        EXTengine *e = h->hero_eng;
        p->unk10 = e->unk10;
        p->unk10.y -= D_800B9594[h->hero_name];
        p->unk1E += 0x38;
        p->unk5E = 1;
    }
    if (D_800B9F4C[player].f8 == 0) {
        if (--D_800B9F4C[player].fC <= 0) {
            BonkHidePowerup(player);
        }
    }
}
