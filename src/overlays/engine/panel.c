#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/panel.h"
#include "bash/hero.h"
#include "bash/shell.h"
#include "bash/tank.h"   /* TA_players: the Tank HUD reads its weapon and rounds */

void Setup_panel(void) {
    EngineSlotView *v;
    s32 i;

    PanelCtrl = (PanelCtrl | 0x80000000) & 0xFFFE2FFF;
    v = (EngineSlotView *)GameInfo;
    if (v->unk6C != 0) {
        i = v->unk48;
        D_800991D8[0] = i;
        D_800991D8[1] = i;
        i = v->unk58[i];
        if (i != -1) {
            D_800991D8[1] = i;
        }
        i = v->unk4A;
        D_800991D8[2] = i;
        D_800991D8[3] = i;
        i = v->unk58[i];
        if (i != -1) {
            D_800991D8[3] = i;
        }
    } else {
        D_800991D8[0] = 0;
        D_800991D8[1] = 1;
        D_800991D8[2] = 2;
        D_800991D8[3] = 3;
    }
    InitCompletionCounter();
    if (ModeTypes[GameMode * 3] != 0x101 && ModeTypes[GameMode * 3] != 0x102) {
        switch (GameMode) {
        case 14:
        case 15:
        case 16:
        case 17:
            InitIcons4();
            InitScore1Icons();
            InitCountdown();
            StartPanelCountdown();
            break;
        case 10:
        case 11:
        case 12:
            InitIcons4();
            InitScore3Icons();
            InitCountdown();
            break;
        case 13:
            InitIcons4();
            InitScore3Icons();
            InitCountdown();
            break;
        case 29:
            InitIcons4();
            InitScore3Icons();
            InitCountdown();
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            InitIcons4();
            InitHealthIcons();
            InitCountdown();
            StartPanelCountdown();
            InitTankWeaponIcons();
            break;
        case 9:
            InitIcons4();
            InitHealthIcons();
            InitCountdown();
            InitTankWeaponIcons();
            break;
        case 30:
            InitIcons4();
            InitScore3Icons();
            InitCountdown();
            StartPanelCountdown();
            break;
        case 0:
        case 1:
        case 2:
        case 3:
            InitIcons4();
            InitHealthIcons();
            InitCountdown();
            StartPanelCountdown();
            break;
        case 4:
            InitIcons4();
            InitHealthIcons();
            InitCountdown();
            break;
        case 32:
            InitIcons2();
            InitHealthIcons();
            InitCountdown();
            break;
        case 23:
        case 24:
        case 25:
        case 26:
            InitIcons4();
            InitCountdown();
            InitScore2Icons();
            InitPowerIcons();
            break;
        case 28:
            InitIcons4();
            InitScore3Icons();
            InitCountdown();
            break;
        case 27:
            InitIcons4();
            InitScore3Icons();
            InitCountdown();
            break;
        case 22:
            InitIcons4();
            InitPowerIcons();
            InitCountdown();
            break;
        case 18:
        case 19:
        case 20:
        case 21:
            InitIcons4();
            InitPowerIcons();
            InitCountdown();
            StartPanelCountdown();
            break;
        case 31:
            InitIcons4();
            InitHealthIcons();
            InitCountdown();
            break;
        default:
            InitIcons4();
            InitCountdown();
            break;
        }
    }
    InitTrophyCups();
    PauseRecSet = 0;
    PauseRec = 0;
    PauseRecIndex = 0;
}

void UpdatePanel(void) {
    u16 *t;

    if (PanelCtrl & 8) {
        UpdateHealthIcons();
    }
    if (PanelCtrl & 0x10) {
        UpdatePowerIcons();
    }
    if (PanelCtrl & 0x700) {
        UpdateScoreIcons();
    }
    if (PanelCtrl & 0x1000) {
        UpdateBigScore();
    }
    if (PanelCtrl & 0x8000) {
        CheckHeroesAllOut();
    }
    if (PanelCtrl & 0x100000) {
        UpdateTankWeaponIcons();
    }
    Cups_Update();
    if (GameCheats & 0x400) {
        FntPrint("   HEALTH %2d   HEALTH %2d              HEALTH %2d    HEALTH %2d\n",
                 Heroes[0].hero_health, Heroes[1].hero_health, Heroes[2].hero_health,
                 Heroes[3].hero_health);
        t = D_8009D524;
        FntPrint("   LAST %d.%2d   LAST %d.%2d              LAST %d.%2d    LAST %d.%2d\n",
                 t[0] >> 8, (t[0] & 0xFF) * 100 / 256, t[1] >> 8, (t[1] & 0xFF) * 100 / 256,
                 t[2] >> 8, (t[2] & 0xFF) * 100 / 256, t[3] >> 8, (t[3] & 0xFF) * 100 / 256);
    }
}

void InitBigScore(void) {
    BigScore1 = EngineTemplate;
    BigScore1.flags = 0x10008000;
    BigScore1.unk10.x = 0x82;
    BigScore1.unk10.y = 0x8C;
    BigScore1.unk10.z = 0;
    subs_init_array(&BigScore1, 1, &BigScoreParts1);
    PlaceFlashSub((s32 *)&BigScoreParts1, -0x30, 0, left_score, 0);
    BigScore1.unk5D = 0;
    BigScore1.unk5C = 1;
    BigScoreParts1.flags &= ~0x10000000;
    BigScore2 = EngineTemplate;
    BigScore2.flags = 0x10008000;
    BigScore2.unk10.x = 0x1E0;
    BigScore2.unk10.y = 0x8C;
    BigScore2.unk10.z = 0;
    subs_init_array(&BigScore2, 1, &BigScoreParts2);
    PlaceFlashSub((s32 *)&BigScoreParts2, -0x30, 0, right_score, 0);
    BigScore2.unk5C = 1;
    BigScore2.unk5D = 0;
    BigScore_scale = 0x10;
    BigScore_teamA = 0;
    BigScore_teamB = 0;
    BigScore_Adelay = 0;
    BigScore_Bdelay = 0;
    BigScoreParts2.flags &= ~0x10000000;
    PanelCtrl |= 0x1000;
}

void UpdateBigScore(void) {
    s32 lim;
    s32 i;
    EXTengine *e;
    EXTengine *f;
    s32 v;
    s32 n = 0;
    s32 m;
    s32 q;
    s32 d;
    s32 c2;
    HeroInfo *a;
    HeroInfo *b;
    HeroInfo *hb;
    s32 h;
    s32 h2;
    s32 tm;
    s32 s1;
    s32 num2;
    s32 cur;
    s32 diff;

    if (CupMode != 0) {
        lim = 0xA00;
        if (BigScore1.unk10.y > 0x5A) {
            BigScore1.unk10.y -= 8;
        }
        if (BigScore2.unk10.y > 0x5A) {
            BigScore2.unk10.y -= 8;
        }
        for (i = 0; i < 4; i++) {
            if ((s16)Score[i].Yoffs > -0x100) {
                Score[i].Yoffs -= 8;
            }
        }
    } else {
        lim = 0x1000;
    }
    if ((u16)BigScore_scale < lim) {
        BigScore_scale += 0x100;
        if ((u16)BigScore_scale > lim) {
            BigScore_scale = lim;
        }
    }
    if ((u16)BigScore_scale > lim) {
        BigScore_scale -= 0x100;
        if ((u16)BigScore_scale < lim) {
            BigScore_scale = lim;
        }
    }
    e = &BigScore1;
    v = (u16)BigScore_scale;
    do {
        e->unk2C.x = v;
        e->unk2C.y = v;
        e->unk2C.z = v;
    } while (0);
    do {
        f = &BigScore2;
        f->unk2C.x = v;
        f->unk2C.y = v;
        f->unk2C.z = v;
    } while (0);

    hb = Heroes;
    h = GameTeamLeader[0];
    a = &hb[h];
    tm = a->hero_teammate;
    if (tm == -1) {
        tm = h;
    }
    b = &hb[tm];
    s1 = a->hero_score;
    num2 = b->hero_score;
    cur = (u16)BigScore_teamA;
    diff = cur - (s1 + num2);
    if (diff < 0) {
        BigScore_teamA = cur + 1;
        if (diff < -30) {
            BigScore_teamA = cur + 2;
        }
        if (diff < -60) {
            BigScore_teamA++;
        }
        if (diff < -90) {
            BigScore_teamA++;
        }
        if (diff < -120) {
            BigScore_teamA++;
        }
        diff = (u16)BigScore_teamA;
        if (s1 < diff) {
            a->hero_score_displayed = 0;
            b->hero_score_displayed = s1 + num2 - diff;
        } else {
            a->hero_score_displayed = s1 - diff;
            b->hero_score_displayed = num2;
        }
    } else {
        a->hero_score_displayed = 0;
        b->hero_score_displayed = 0;
    }

    hb = Heroes;
    h2 = GameTeamLeader[1];
    a = &hb[h2];
    tm = a->hero_teammate;
    if (tm == -1) {
        tm = h2;
    }
    b = &hb[tm];
    s1 = a->hero_score;
    num2 = b->hero_score;
    cur = (u16)BigScore_teamB;
    diff = cur - (s1 + num2);
    if (diff < 0) {
        BigScore_teamB = cur + 1;
        if (diff < -30) {
            BigScore_teamB = cur + 2;
        }
        if (diff < -60) {
            BigScore_teamB++;
        }
        if (diff < -90) {
            BigScore_teamB++;
        }
        if (diff < -120) {
            BigScore_teamB++;
        }
        diff = (u16)BigScore_teamB;
        if (s1 < diff) {
            a->hero_score_displayed = 0;
            b->hero_score_displayed = s1 + num2 - diff;
        } else {
            a->hero_score_displayed = s1 - diff;
            b->hero_score_displayed = num2;
        }
    } else {
        a->hero_score_displayed = 0;
        b->hero_score_displayed = 0;
    }

    n = (u16)BigScore_teamA;
    d = n / 10;
    q = d;
    d = n - q * 10;
    n = q;
    c2 = d + '0';
    d = n / 10;
    q = d;
    d = n - q * 10;
    n = q;
    ((char *)left_score)[2] = c2;
    ((char *)left_score)[1] = d + '0';
    d = n / 10;
    d = n - d * 10;
    ((char *)left_score)[0] = d + '0';
    n = (u16)BigScore_teamB;
    d = n / 10;
    q = d;
    d = n - q * 10;
    n = q;
    m = d + '0';
    d = n / 10;
    q = d;
    d = n - q * 10;
    n = q;
    ((char *)right_score)[2] = m;
    ((char *)right_score)[1] = d + '0';
    d = n / 10;
    d = n - d * 10;
    ((char *)right_score)[0] = d + '0';
    BigScore1.unk5E = 1;
    BigScore2.unk5E = 1;
}

void Set_Clock(u16 time, u16 flags) {
    s32 i;
    s32 d;

    if (time == 0xFFFF) {
        time = (u32)Clock_Time >> 8;
    }
    if (flags == 0xFFFF) {
        flags = Clock_flags;
    }
    Clock_Time = time << 8;
    flags |= 0x400;
    Clock_flags = flags;
    Clock[0].Ctrl = 0x10008000;
    Clock[0].frame = 0x3013;
    Clock[0].Xoffs = Clock_offsets[0].a;
    for (i = 1; i < 4; i++) {
        d = time & 0xF;
        if (d >= 0xA) {
            d = 0;
        }
        time >>= 4;
        Clock[i].frame = d + 0x3009;
        Clock[i].Ctrl = 0x10008000;
        Clock[i].Xoffs = Clock_offsets[i].a;
    }
}

void Draw_Clock(void) {
    EngineClockView *v;
    u32 time;
    u32 t;
    u32 e;
    s32 sec;
    s32 f;
    s32 min;
    s32 n = 0;
    s32 i;
    s32 d;
    s32 fl;
    ClockInfo *p;
    HeroInfo *h;

    if (!(PanelCtrl & 1)) {
        return;
    }
    if (GameExitFlags & 0x9800) {
        return;
    }
    time = (u32)Clock_Time >> 8;
    if (Clock_flags & 0x400) {
        if (Clock_Time == 0x2000) {
            music_set_tempo(0x6E, 0xF0);
        }
    }
    if (Clock_flags & 0x200) {
        if (StillTottingUpScore() != 0) {
            s32 k = 100;

            for (d = 3; d >= 0; d--) {
                Heroes[d].hero_frozen = k;
            }
        } else {
            n = 1;
            if (GameCheats & 0x800) {
                n = 0x201;
            }
            HeroHooks.ChooseWinner(-1, n);
            Clock_flags = 0x8000;
        }
    }
    if ((Clock_flags & 0x100) && time == 0) {
        PlaySfx(0x19C, NULL, NULL, 0x1000, 0x1E00, 0);
        if (StillTottingUpScore() != 0) {
            Clock_Time = 0;
            Clock_flags = 0x8200;
        } else {
            n = 1;
            if (GameCheats & 0x800) {
                n = 0x201;
            }
            HeroHooks.ChooseWinner(-1, n);
            Clock_Time = 0;
            Clock_flags = 0x8000;
        }
    }
    v = (EngineClockView *)GameInfo;
    if (!(v->unk4 & 0x40) && !(v->unk10 & 0x04000000)) {
        if (Clock_flags & 0x20) {
            u32 bf;
            u32 bs;
            u32 bm;

            sec = (u32)Clock_Time >> 8;
            min = (u32)Clock_Time >> 16;
            bf = *(u8 *)&Clock_Time;
            f = (bf >> 4) * 10 + (bf & 0xF);
            bs = (u8)sec;
            sec = (bs >> 4) * 10 + (bs & 0xF);
            bm = (u8)min;
            min = (bm >> 4) * 10 + (bm & 0xF);
            f += 2;
            if (v->unk21 < f) {
                sec++;
                f -= v->unk21;
            }
            if (sec >= 60) {
                min++;
                sec -= 60;
            }
            if (min >= 60) {
                min -= 60;
            }
            e = (f / 10) * 16 + f % 10;
            sec = (sec / 10) * 16 + sec % 10;
            min = (min / 10) * 16 + min % 10;
            Clock_Time = ((min & 0xFF) << 16) + ((sec & 0xFF) << 8) + (e & 0xFF);
        }
        if (Clock_flags & 0x40) {
            s32 min;
            u32 bf;
            u32 bs;
            u32 m;

            sec = (u32)Clock_Time >> 8;
            min = (u32)Clock_Time >> 16;
            bf = *(u8 *)&Clock_Time;
            f = (bf >> 4) * 10 + (bf & 0xF);
            bs = (u8)sec;
            sec = (bs >> 4) * 10 + (bs & 0xF);
            m = (u8)min;
            min = (m >> 4) * 10 + (m & 0xF);
            f -= 2;
            if (f < 0) {
                sec--;
                f += ((EngineClockView *)GameInfo)->unk21;
            }
            if (sec < 0) {
                min--;
                sec += 60;
            }
            if (min < 0) {
                min += 60;
            }
            e = (f / 10) * 16 + f % 10;
            sec = (sec / 10) * 16 + sec % 10;
            min = (min / 10) * 16 + min % 10;
            Clock_Time = ((min & 0xFF) << 16) + ((sec & 0xFF) << 8) + (e & 0xFF);
        }
    }
    if (Clock_flags & 0x80) {
        if (time < 6) {
            if ((u16)Clock_last_ping != time) {
                PlaySfx(0x19B, NULL, NULL, 0x1000, 0x1E00, 0);
                Clock_last_ping = time;
            }
        } else {
            Clock_last_ping = 99;
        }
    }
    fl = Clock_flags;
    do {
        if (!(fl & 0x10)) {
            goto join;
        }
        p = Clock;
        t = p->Ctrl & ~0x8000;
        p->Ctrl = t;
    } while (0);
    if (!(fl & 2) && (u32)(Clock_Time & 0x70) < 0x1D) {
        do {
            t |= 0x8000;
            Clock->Ctrl = t;
            break;
        } while (1);
    }
join:
    f = 4;
    if (Clock_flags & 4) {
        f = 3;
    }
    for (i = 1; i < f; i++) {
        d = time & 0xF;
        if (d >= 10) {
            d = 0;
        }
        Clock[i].frame = d + 0x3009;
        time >>= 4;
    }
}

void SetupCompletionDigits(void) {
    DigitEnt *e;
    s32 v;
    s32 n = 0;
    s32 i;

    v = D_8004E0D0;
    CompletionDigits[0].flags = 0x10008000;
    CompletionDigits[0].cmd = 0x3014;
    for (i = 1; i < 4; i++) {
        n = v & 0xF;
        if (n >= 10) {
            n = 0;
        }
        v >>= 4;
        CompletionDigits[i].cmd = n + 0x3009;
        CompletionDigits[i].flags = 0x10008000;
    }
    n = 3;
    i = 3;
    do {
        e = &CompletionDigits[i];
        if (e->cmd == 0x3009) {
            n--;
            e->flags &= 0xFFFF7FFF;
        } else {
            i = 0;
        }
        i--;
    } while (i >= 2);
    i = 1;
    for (; i < 4; i++) {
        CompletionDigits[i].arg = *(u16 *)((u8 *)D_80099198 + n * 16 + i * 4);
    }
    CompletionDigits[0].arg = *(u16 *)(n * 16 + (u8 *)D_80099198);
}

void InitIcons4(void) {
    s32 i;
    s32 j;
    subengine *sub;

    for (i = 0; i < 4; i++) {
        ((EXTengine **)&Icon)[i] = &PanelTab[i];
        sub = IconParts[i];
        subs_init_array(((EXTengine **)&Icon)[i], 7, sub);
        for (j = 0; j < 7; j++) {
            sub->unk6C = InterfaceModel;
            sub->flags = 0x10000000;
            sub++;
        }
        sub = IconParts[i];
        sub->unk6C = InterfaceModel;
        ((EXTengine **)&Icon)[i]->flags = 0x10008000;
        sub->flags = 0x10008000;
        ((EXTengine **)&Icon)[i]->unk10.x = D_800991E0[D_800991D8[i]][0];
        ((EXTengine **)&Icon)[i]->unk10.y = D_800991E0[D_800991D8[i]][1];
        ((EXTengine **)&Icon)[i]->unk10.z = 0;
        sub->unk74 = IconImage[Heroes[i].hero_name];
        sub = &IconParts[i][1];
        sub->unk6C = InterfaceModel;
        sub->flags = 0;
        sub->unk04 = D_80099240[i].x;
        sub->unk08 = D_80099240[i].y;
        sub->unk0C = 0;
        sub->unk74 = 0x3026;
    }
    PanelCtrl |= 2;
}

void InitIcons2(void) {
    s32 i;
    s32 j;
    subengine *sub;

    for (i = 0; i < 2; i++) {
        ((EXTengine **)&Icon)[i] = &PanelTab[i];
        sub = IconParts[i];
        subs_init_array(((EXTengine **)&Icon)[i], 7, sub);
        for (j = 0; j < 7; j++) {
            sub->unk6C = InterfaceModel;
            sub->flags = 0x10000000;
            sub++;
        }
        sub = IconParts[i];
        sub->unk6C = InterfaceModel;
        ((EXTengine **)&Icon)[i]->flags = 0x10008000;
        sub->flags = 0x10008000;
        ((EXTengine **)&Icon)[i]->unk10.x = D_800991F0[D_800991D8[i]][0];
        ((EXTengine **)&Icon)[i]->unk10.y = D_800991F0[D_800991D8[i]][1];
        ((EXTengine **)&Icon)[i]->unk10.z = 0;
        sub->unk74 = IconImage[Heroes[i].hero_name];
        sub = &IconParts[i][1];
        sub->unk6C = InterfaceModel;
        sub->flags = 0;
        sub->unk04 = D_80099240[i].x;
        sub->unk08 = D_80099240[i].y;
        sub->unk0C = 0;
        sub->unk74 = 0x3026;
    }
    PanelCtrl |= 2;
}

void InitHealthIcons(void) {
    s32 i;
    s16 x0;
    s16 x1;
    s32 c0;
    s32 c1;
    s32 off;
    PanelPrim *b;
    PanelPrim *p;
    PanelPrim *q;
    s32 col;
    s32 yb;
    u8 *tim;
    u8 *tex;
    s16 y0;
    s16 y1;
    s32 code;
    s16 n;

    i = 0;
    tim = PowerupsModel;
    y0 = 0x37;
    c0 = 0x3FFF7F;
    c1 = 0xF4F2F;
    tex = draw_nas_model_flags;
    y1 = 0x43;
    code = 0x12000000;
    n = 8;
    while (1) {
        if (i >= 4) {
            break;
        }
        col = 0;
        pHeroHealth[i] = col;
        if (Heroes[i].hero_name != 9) {
            pHeroHealth[i] = (s32)&Heroes[i].hero_health;
            p = ((PanelPrim (*)[7])&IconParts[0][2])[i];
            p->u6C.ptr = tim;
            p->unk0 = 0x10008000;
            p->unk4 = HealthIconPos[i].x + 0x10;
            yb = HealthIconPos[i].y;
            p->unk74 = 0x2018;
            off = i * sizeof(((PanelPrim (*)[7])&IconParts[0][2])[0]);
            b = (PanelPrim *)(off + (u8 *)&((PanelPrim (*)[7])&IconParts[0][2])[0][1]);
            p->unkC = 0;
            p->unk8 = yb + 0x10;
            x0 = HealthIconPos[i].x + 0x1C;
            x1 = HealthIconPos[i].x + 0x5C;
            b->unk54 = tex;
            b->u6C.s.unk6E = y0;
            b->unk70 = 0;
            b->unk76 = y0;
            b->unk78 = 0;
            b->unk7E = y1;
            b->unk80 = 0;
            b->unk86 = y1;
            b->unk88 = 0;
            b->unk8C = c0;
            b->unk90 = c0;
            b->unk94 = c1;
            b->unk98 = c1;
            b->unk0 = code;
            b->unk68 = n;
            b->u6C.s.unk6C = x0;
            b->unk74 = x1;
            b->unk7C = x0;
            b->unk84 = x1;
            x0 = HealthIconPos[i].x + 0x1C;
            x1 = HealthIconPos[i].x + 0x5C;
            q = &((PanelPrim (*)[7])&IconParts[0][2])[i][2];
            q->unk54 = tex;
            q->u6C.s.unk6E = y0;
            q->u6C.s.unk6C = x0;
            q->unk70 = 0;
            q->unk74 = x1;
            q->unk84 = x1;
            col = 0xBF;
            q->unk76 = y0;
            q->unk78 = 0;
            q->unk7C = x0;
            q->unk7E = y1;
            q->unk80 = 0;
            q->unk86 = y1;
            q->unk88 = 0;
            q->unk8C = col;
            q->unk90 = col;
            q->unk94 = col;
            q->unk98 = col;
            q->unk0 = code;
            q->unk68 = n;
        }
        i++;
    }
    PanelCtrl |= 8;
}

void ClearHealthIcons(void) {
    s32 i;
    s16 x0;
    s16 x1;
    s32 c0;
    s32 c1;
    s32 off;
    PanelPrim *b;
    PanelPrim *p;
    PanelPrim *q;
    s32 col;
    s32 yb;
    u8 *tim;
    u8 *tex;
    s16 y0;
    s16 y1;
    s32 code;
    s16 n;

    i = 0;
    tim = PowerupsModel;
    y0 = 0x37;
    c0 = 0x3FFF7F;
    c1 = 0xF4F2F;
    tex = draw_nas_model_flags;
    y1 = 0x43;
    code = 0x12000000;
    n = 8;
    while (1) {
        if (i >= 4) {
            break;
        }
        col = 0;
        pHeroHealth[i] = col;
        if (Heroes[i].hero_name != 9) {
            pHeroHealth[i] = (s32)&Heroes[i].hero_health;
            p = ((PanelPrim (*)[7])&IconParts[0][2])[i];
            p->u6C.ptr = tim;
            p->unk0 = 0x10008000;
            p->unk4 = HealthIconPos[i].x;
            yb = HealthIconPos[i].y;
            p->unk74 = 0x2018;
            off = i * sizeof(((PanelPrim (*)[7])&IconParts[0][2])[0]);
            b = (PanelPrim *)(off + (u8 *)&((PanelPrim (*)[7])&IconParts[0][2])[0][1]);
            p->unkC = 0;
            p->unk8 = yb + 0x10;
            x0 = HealthIconPos[i].x + 8;
            x1 = HealthIconPos[i].x + 0x48;
            b->unk54 = tex;
            b->u6C.s.unk6E = y0;
            b->unk70 = 0;
            b->unk76 = y0;
            b->unk78 = 0;
            b->unk7E = y1;
            b->unk80 = 0;
            b->unk86 = y1;
            b->unk88 = 0;
            b->unk8C = c0;
            b->unk90 = c0;
            b->unk94 = c1;
            b->unk98 = c1;
            b->unk0 = code;
            b->unk68 = n;
            b->u6C.s.unk6C = x0;
            b->unk74 = x1;
            b->unk7C = x0;
            b->unk84 = x1;
            x0 = HealthIconPos[i].x + 8;
            x1 = HealthIconPos[i].x + 0x48;
            q = &((PanelPrim (*)[7])&IconParts[0][2])[i][2];
            q->unk54 = tex;
            q->u6C.s.unk6E = y0;
            q->u6C.s.unk6C = x0;
            q->unk70 = 0;
            q->unk74 = x1;
            q->unk84 = x1;
            col = 0xBF;
            q->unk76 = y0;
            q->unk78 = 0;
            q->unk7C = x0;
            q->unk7E = y1;
            q->unk80 = 0;
            q->unk86 = y1;
            q->unk88 = 0;
            q->unk8C = col;
            q->unk90 = col;
            q->unk94 = col;
            q->unk98 = col;
            q->unk0 = code;
            q->unk68 = n;
        }
        i++;
    }
    PanelCtrl |= 8;
}

void InitBossHealthIcons(void) {
    PanelPrim *sub;
    PanelPrim *b;
    PanelPrim *q;
    s32 x0;
    s16 x1;
    s32 y0;
    s32 y1;
    SWORD_XY *s;
    s32 c0;
    s32 c1;
    s32 code;
    u8 *tex;
    u8 i;

    i = 3;
    pHeroHealth[3] = (s32)&D_8009D6DC[3];
    sub = ((PanelPrim (*)[7])&IconParts[0][2])[3];
    b = sub + 1;
    sub->u6C.ptr = PowerupsModel;
    sub->unk0 = 0x10008000;
    sub->unk74 = 0x2018;
    c0 = 0x3FFF7F;
    c1 = 0xF4F2F;
    tex = draw_nas_model_flags;
    y1 = 0x43;
    y0 = 0x37;
    code = 0x12000000;
    sub->unkC = 0;
    b->unk8C = c0;
    b->unk90 = c0;
    b->unk54 = tex;
    b->u6C.s.unk6E = y0;
    b->unk70 = 0;
    b->unk76 = y0;
    b->unk78 = 0;
    b->unk7E = y1;
    b->unk80 = 0;
    b->unk86 = y1;
    b->unk88 = 0;
    b->unk94 = c1;
    b->unk98 = c1;
    sub[1].unk0 = code;
    b->unk68 = 8;
    s = &HealthIconPos[i];
    q = sub + 2;
    q->unk54 = tex;
    q->u6C.s.unk6E = y0;
    q->unk70 = 0;
    sub->unk4 = s->x;
    sub->unk8 = s->y + 0x10;
    x0 = (u16)s->x + 8;
    x1 = (u16)s->x + 0x48;
    b->u6C.s.unk6C = x0;
    b->unk74 = x1;
    b->unk7C = x0;
    b->unk84 = x1;
    q->u6C.s.unk6C = x0;
    q->unk74 = x1;
    q->unk76 = y0;
    q->unk7C = x0;
    q->unk84 = x1;
    q->unk78 = 0;
    q->unk7E = y1;
    q->unk80 = 0;
    q->unk86 = y1;
    q->unk88 = 0;
    q->unk8C = 0xBF;
    q->unk90 = 0xBF;
    q->unk94 = 0xBF;
    q->unk98 = 0xBF;
    q->unk0 = code;
    q->unk68 = 8;
    PanelCtrl |= 8;
}

void InitPowerupIcons(s32 arg0, s32 arg1) {
    s16 x0;
    s16 x1;
    s32 yb;
    PanelPrim *b;
    PanelPrim *p;
    PanelPrim *q;
    s32 col;
    u8 *tim;
    s16 y0;
    s16 y1;
    s32 code;
    s16 n;

    tim = PowerupsModel;

    pHeroHealth[2] = arg0;
    p = ((PanelPrim (*)[7])&IconParts[0][2])[2];
    p->u6C.ptr = tim;
    p->unk0 = 0x10008000;
    y0 = 0x37;
    y1 = 0x43;
    code = 0x12000000;
    n = 8;
    p->unk4 = HealthIconPos[2].x;
    yb = HealthIconPos[2].y;
    p->unkC = 0;
    p->unk74 = 0x2018;
    p->unk8 = yb + 0x10;
    b = p + 1;
    x0 = (u16)HealthIconPos[2].x + 8;
    x1 = (u16)HealthIconPos[2].x + 0x48;
    b->unk54 = draw_nas_model_flags;
    b->u6C.s.unk6C = x0;
    b->u6C.s.unk6E = y0;
    b->unk70 = 0;
    b->unk74 = x1;
    b->unk76 = y0;
    b->unk78 = 0;
    b->unk7C = x0;
    b->unk7E = y1;
    b->unk80 = 0;
    b->unk84 = x1;
    b->unk86 = y1;
    b->unk88 = 0;
    b->unk8C = 0x3FFF7F;
    b->unk90 = 0x3FFF7F;
    b->unk94 = 0xF4F2F;
    b->unk98 = 0xF4F2F;
    p[1].unk0 = code;
    b->unk68 = n;
    q = b + 1;
    q->unk54 = draw_nas_model_flags;
    q->u6C.s.unk6C = x0;
    q->u6C.s.unk6E = y0;
    q->unk70 = 0;
    q->unk74 = x1;
    q->unk7C = x0;
    col = 0xBF;
    q->unk76 = y0;
    q->unk78 = 0;
    q->unk7E = y1;
    q->unk80 = 0;
    q->unk84 = x1;
    q->unk86 = y1;
    q->unk88 = 0;
    q->unk8C = col;
    q->unk90 = col;
    q->unk94 = col;
    q->unk98 = col;
    p[2].unk0 = code;
    q->unk68 = n;

    pHeroHealth[3] = arg1;
    p = p + 7;
    p->u6C.ptr = tim;
    p->unk0 = 0x10008000;
    p->unk4 = HealthIconPos[3].x;
    yb = HealthIconPos[3].y;
    p->unkC = 0;
    p->unk74 = 0x2018;
    p->unk8 = yb + 0x10;
    x0 = (u16)HealthIconPos[3].x + 8;
    x1 = (u16)HealthIconPos[3].x + 0x48;
    {
    PanelPrim *q;
    q = b + 7;
    q->unk54 = draw_nas_model_flags;
    q->u6C.s.unk6C = x0;
    q->u6C.s.unk6E = y0;
    q->unk70 = 0;
    q->unk74 = x1;
    q->unk76 = y0;
    q->unk78 = 0;
    q->unk7C = x0;
    q->unk7E = y1;
    q->unk80 = 0;
    q->unk84 = x1;
    q->unk86 = y1;
    q->unk88 = 0;
    q->unk8C = 0x3FFF7F;
    q->unk90 = 0x3FFF7F;
    q->unk94 = 0xF4F2F;
    q->unk98 = 0xF4F2F;
    b[7].unk0 = code;
    q->unk68 = n;
    }
    {
    PanelPrim *q;
    q = b + 8;
    q->unk54 = draw_nas_model_flags;
    q->u6C.s.unk6C = x0;
    q->u6C.s.unk6E = y0;
    q->unk70 = 0;
    q->unk74 = x1;
    q->unk76 = y0;
    q->unk78 = 0;
    q->unk7C = x0;
    q->unk7E = y1;
    q->unk80 = 0;
    q->unk84 = x1;
    q->unk86 = y1;
    q->unk88 = 0;
    q->unk8C = col;
    q->unk90 = col;
    q->unk94 = col;
    q->unk98 = col;
    b[8].unk0 = code;
    q->unk68 = n;
    }
    PanelCtrl |= 8;
}

void UpdateHealthIcons(void) {
    s32 i;
    s32 BarLength;
    s32 len;
    u8 *sub;
    s32 t;
    subengine (*e)[7];
    s32 c;

    e = IconParts;
    c = GameFrameCount;
    for (i = 0; i < 4; i++) {
        t = c >> 1;
        BarLength = 0;
        if (pHeroHealth[i] != 0) {
            switch (((EngineGameView *)GameInfo)->mode) {
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                e[i][2].unk74 = (t + i * 4) % 14 + 0x2018;
                BarLength = (u16)(*(u16 *)pHeroHealth[i] / 3);
                break;
            case 0x1F:
                e[i][2].unk74 = (t + i * 4) % 14 + 0x2018;
                BarLength = *(u16 *)pHeroHealth[i] >> 3;
                break;
            default:
                e[i][2].unk74 = (t + i * 4) % 14 + 0x2018;
                BarLength = *(u16 *)pHeroHealth[i] * 31 / 20;
                break;
            }
        }
        if (BarLength >= 0x20) {
            BarLength = 0x1F;
        }
        sub = ((u8 *)&IconParts[0][3]) + i * 0x498;
        len = BarLength * 2 + *(s16 *)(sub + 0x6C);
        *(s16 *)(sub + 0x74) = len;
        *(s16 *)(sub + 0x84) = len;
        if (BarLength != 0) {
            *(s32 *)sub |= 0x8000;
        } else {
            *(s32 *)sub &= 0xFFFF7FFF;
        }
        sub = ((u8 *)&IconParts[0][4]) + i * 0x498;
        *(s16 *)(sub + 0x6C) = len;
        *(s16 *)(sub + 0x7C) = len;
        if (BarLength < 0x1F) {
            *(s32 *)sub |= 0x8000;
        } else {
            *(s32 *)sub &= 0xFFFF7FFF;
        }
    }
}

void InitPowerIcons(void) {
    s32 i;
    s16 x0;
    s16 x1;
    s32 c0;
    s32 c1;
    s32 off;
    PanelPrim *b;
    PanelPrim *p;
    PanelPrim *q;
    s32 col;
    s32 yb;
    u8 *tim;
    u8 *tex;
    s16 y0;
    s16 y1;
    s32 code;
    s16 n;

    i = 0;
    tim = InterfaceModel;
    y0 = 0x37;
    c0 = 0x3FFF7F;
    c1 = 0xF4F2F;
    tex = draw_nas_model_flags;
    y1 = 0x43;
    code = 0x12000000;
    n = 8;
    while (1) {
        if (i >= 4) {
            break;
        }
        col = 0;
        pHeroHealth[i] = col;
        if (Heroes[i].hero_name != 9) {
            p = ((PanelPrim (*)[7])&IconParts[0][2])[i];
            p->u6C.ptr = tim;
            p->unk0 = 0x10008000;
            p->unk4 = HealthIconPos[i].x - 0xE;
            yb = HealthIconPos[i].y;
            p->unk74 = 0x3021;
            off = i * sizeof(((PanelPrim (*)[7])&IconParts[0][2])[0]);
            b = (PanelPrim *)(off + (u8 *)&((PanelPrim (*)[7])&IconParts[0][2])[0][1]);
            p->unkC = 0;
            p->unk8 = yb - 0x10;
            x0 = HealthIconPos[3].x + 0x10;
            x1 = HealthIconPos[3].x + 0x50;
            b->unk54 = tex;
            b->u6C.s.unk6E = y0;
            b->unk70 = 0;
            b->unk76 = y0;
            b->unk78 = 0;
            b->unk7E = y1;
            b->unk80 = 0;
            b->unk86 = y1;
            b->unk88 = 0;
            b->unk8C = c0;
            b->unk90 = c0;
            b->unk94 = c1;
            b->unk98 = c1;
            b->unk0 = code;
            b->unk68 = n;
            b->u6C.s.unk6C = x0;
            b->unk74 = x1;
            b->unk7C = x0;
            b->unk84 = x1;
            x0 = HealthIconPos[3].x + 0x10;
            x1 = HealthIconPos[3].x + 0x50;
            q = &((PanelPrim (*)[7])&IconParts[0][2])[i][2];
            q->unk54 = tex;
            q->u6C.s.unk6E = y0;
            q->unk70 = 0;
            q->u6C.s.unk6C = x0;
            q->unk74 = x1;
            q->unk84 = x1;
            col = 0xBF;
            q->unk76 = y0;
            q->unk78 = 0;
            q->unk7C = x0;
            q->unk7E = y1;
            q->unk80 = 0;
            q->unk86 = y1;
            q->unk88 = 0;
            q->unk8C = col;
            q->unk90 = col;
            q->unk94 = col;
            q->unk98 = col;
            q->unk0 = code;
            q->unk68 = n;
        }
        i++;
    }
    PanelCtrl |= 0x10;
}

void UpdatePowerIcons(void) {
    s32 i;
    s32 off;
    HeroInfo *h;
    s32 BarLength;
    s32 len;
    u8 *e;
    u8 *sub;

    i = 0;
    off = i;
    h = Heroes;
loop:
    if (i < 4) {
        if (h->hero_name != 9) {
            if (GameMode < 0x12) {
                BarLength = 0;
            } else {
                if (GameMode >= 0x17) {
                    if (GameMode >= 0x1B) {
                        BarLength = 0;
                    } else {
                        e = FindExtension(h->hero, 0x10000000);
                        if (e != NULL) {
                            BarLength = *(s32 *)(e + 0x58) >> 10;
                        } else {
                            BarLength = 0;
                        }
                    }
                } else {
                    e = FindExtension(h->hero, 0x20000000);
                    if (e != NULL) {
                        BarLength = *(s32 *)(e + 0x20) >> 2;
                    } else {
                        BarLength = 0;
                    }
                }
            }
            if (BarLength >= 0x40) {
                BarLength = 0x3F;
            }
            sub = ((u8 *)&IconParts[0][3]) + off;
            len = BarLength + *(s16 *)(sub + 0x6C);
            *(s16 *)(sub + 0x74) = len;
            *(s16 *)(sub + 0x84) = len;
            if (BarLength != 0) {
                *(s32 *)sub |= 0x8000;
            } else {
                *(s32 *)sub &= 0xFFFF7FFF;
            }
            sub = ((u8 *)&IconParts[0][4]) + off;
            *(s16 *)(sub + 0x6C) = len;
            *(s16 *)(sub + 0x7C) = len;
            if (BarLength < 0x3F) {
                *(s32 *)sub |= 0x8000;
            } else {
                *(s32 *)sub &= 0xFFFF7FFF;
            }
        }
        off += 0x498;
        h++;
        i++;
        goto loop;
    }
}

void InitScore3Icons(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            Score[i].score = 0;
            Score[i].Xoffs = ScoreIcon3Pos[i].x;
            Score[i].Yoffs = ScoreIcon3Pos[i].y;
            Score[i].Ctrl = 1;
        } else {
            Score[i].Ctrl = 0;
        }
    }
    PanelCtrl |= 0x400;
}

void InitScore2Icons(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            Score[i].score = 0;
            Score[i].Xoffs = ScoreIcon2Pos[i].x;
            Score[i].Yoffs = ScoreIcon2Pos[i].y;
            Score[i].Ctrl = 1;
        } else {
            Score[i].Ctrl = 0;
        }
    }
    PanelCtrl |= 0x200;
}

void InitScore1Icons(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            Score[i].score = 0;
            Score[i].Xoffs = ScoreIcon1Pos[i].x;
            Score[i].Yoffs = ScoreIcon1Pos[i].y;
            Score[i].Ctrl = 1;
        } else {
            Score[i].Ctrl = 0;
        }
    }
    PanelCtrl |= 0x200;
}

void InitSharedScoreIcons(void) {
    s32 i;
    u8 *g = GameInfo;

    if (*(s32 *)(g + 0x6C) != 0) {
        i = *(s16 *)(g + 0x48);
        Score[i].Ctrl = 1;
        Score[i].Xoffs += 0x46;
        i = Heroes[i].hero_teammate;
        if (i != -1) {
            Score[i].Ctrl = 0;
        }
        i = *(s16 *)(g + 0x4A);
        Score[i].Ctrl = 1;
        Score[i].Xoffs += 0x46;
        i = Heroes[i].hero_teammate;
        if (i != -1) {
            Score[i].Ctrl = 0;
        }
    }
}

void InitTankWeaponIcons(void) {
    s32 i;
    s32 off;
    PanelPrim *p;

    i = 0;
    while (1) {
        if (i >= 4) {
            break;
        }
        D_8009AE40[i].a = NULL;
        D_8009AE40[i].b = NULL;
        if (Heroes[i].hero_name != 9) {
            D_8009AE40[i].a = &TA_players[i].MainWeaponType;   /* Tank's, linked from tank (slot_syms.py) */
            off = i * sizeof(((PanelPrim (*)[7])&IconParts[0][5])[0]);
            p = (PanelPrim *)(off + (u8 *)&((PanelPrim (*)[7])&IconParts[0][5])[0][0]);
            p->unk4 = 0x2E;
            p->unk8 = 0x38;
            p->u6C.ptr = TankObjectsModel;
            p->unk0 = 0x10000000;
            p->unkC = 0;
            p->unk74 = 0x3029;
            p = (PanelPrim *)(off + (u8 *)&((PanelPrim (*)[7])&IconParts[0][5])[0][1]);
            D_8009AE40[i].b = &TA_players[i].MainWeaponRounds;
            p->unk4 = 0xA;
            p->unk8 = 0x39;
            p->u6C.ptr = TankObjectsModel;
            p->unk0 = 0x10000000;
            p->unkC = 0;
            p->unk74 = 0x302B;
        }
        i++;
    }
    PanelCtrl |= 0x100000;
}

void UpdateTankWeaponIcons(void) {
    s32 i;
    PanelPrim *p;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9) {
            p = &((PanelPrim (*)[7])&IconParts[0][5])[i][0];
            switch (*(s32 *)D_8009AE40[i].a) {
            case 3:
                p->unk0 |= 0x8000;
                switch (*(s32 *)D_8009AE40[i].b) {
                case 1:
                    p->unk74 = 0x3029;
                    break;
                case 2:
                    p->unk74 = 0x302D;
                    break;
                default:
                    p->unk74 = 0x302C;
                    break;
                }
                break;
            case 2:
                p->unk0 |= 0x8000;
                switch (*(s32 *)D_8009AE40[i].b) {
                case 0:
                    p->unk0 &= ~0x8000;
                    break;
                case 1:
                    p->unk74 = 0x3030;
                    break;
                case 2:
                    p->unk74 = 0x302F;
                    break;
                default:
                    p->unk74 = 0x302E;
                    break;
                }
                break;
            default:
                p->unk0 &= ~0x8000;
                break;
            }
            p = &((PanelPrim (*)[7])&IconParts[0][5])[i][1];
            switch (*D_8009AE40[i].c) {
            case 0:
                p->unk0 &= ~0x8000;
                break;
            case 1:
                p->unk74 = 0x302B;
                p->unk0 |= 0x8000;
                break;
            case 2:
                p->unk74 = 0x302A;
                p->unk0 |= 0x8000;
                break;
            default:
                p->unk74 = 0x3028;
                p->unk0 |= 0x8000;
                break;
            }
        }
    }
}

void UpdateScoreIcons(void) {
    ScoreInfo *q = Score;
    ScoreInfo *end = q;
    HeroInfo *base = Heroes;
    HeroInfo *h;
    int i = 0;

    do {
        h = base + i;
        if (h->hero_score >= 0x3E8) {
            h->hero_score = 0x3E7;
        }
        if (h->hero_score < 0) {
            h->hero_score = 0;
        }
        if (!(PanelCtrl & 0x1000)) {
            if (h->hero_score != h->hero_score_displayed &&
                (!(GameFrameCount & 3) || (Clock_flags & 0x200))) {
                if (h->hero_score_displayed < h->hero_score) {
                    h->hero_score_displayed++;
                } else {
                    h->hero_score_displayed--;
                }
            }
        }
        if ((PanelCtrl & 0x2000) && h->hero_teammate != -1) {
            q->score = h->hero_score_displayed + base[h->hero_teammate].hero_score_displayed;
        } else {
            q->score = h->hero_score_displayed;
        }
        q++;
        i++;
    } while ((s32)q < (s32)end + 0x20);
}

void CheckHeroesAllOut(void) {
    EngineGameView *g = (EngineGameView *)GameInfo;
    EXTengine *e;
    EXTengine *f;

    if (!(g->unk30 & 0x8801) && *(s32 *)(GameInfo + 0x68) == 0 && HeroesAllOut != 0) {
        if (D_8009AE78 != 0) {
            D_8009AE78 -= 1;
            return;
        }
        e = GetEngineSlot(5);
        PlaceEngineSlot(5, D_8005A554);
        PlaySfx(0x194, NULL, NULL, 0x1000, 0x1E00, 0);
        e->unk10.y = 0x1F4;
        e->flags |= 0x10008000;
        PanelCtrl = (PanelCtrl & 0xFFFF7FFF) | 0x4000;
    }
}

void UpdateOutMessage(void) {
    EXTengine *e;
    EXTengine *f;
    s32 i;
    EngineGameView *g;

    g =(EngineGameView *)GameInfo;
    if (*(s32 *)((u8 *)g + 0x10) & 1) {
        return;
    }
    e = GetEngineSlot(5);
    if (PanelCtrl & 0x10000) {
        if (e->unk10.y < 0x258) {
            e->unk10.y += 0x20;
        } else {
            PanelCtrl &= ~0x4000;
        }
        return;
    }
    if (g->unk30 & 0x8801) {
        PanelCtrl |= 0x10000;
        return;
    }
    if (e->unk10.y < 0x17D) {
        e->unk10.y = 0x17C;
    } else {
        e->unk10.y -= 0x10;
    }
    i = 0;
    for (;;) {
        if (i >= 4) {
            return;
        }
        if (GameInfo[i + 0x5C] == 0 && (((PadStick *)Pads)[i].pressed & 0x4000)) {
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
            if (LevelSpecs.FinishQuick != NULL) {
                LevelSpecs.FinishQuick();
            }
            PanelCtrl |= 0x10000;
        }
        i++;
    }
}

s16 FindTeamLeader(void) {
    u8 *g = GameInfo;
    s16 r = -1;
    s16 i;
    s16 t;
    HeroInfo *h;
    HeroInfo *h2;

    if (*(s32 *)(g + 0x6C) != 0 && *(s16 *)(g + 0x34) != 0x20) {
        HeroInfo *H = Heroes;
        s16 a = *(s16 *)(g + 0x48);
        s16 b;
        h = &H[a];
        t = h->hero_teammate;
        if (t == -1) {
            t = *(u16 *)(g + 0x48);
        }
        if (g[0x60] == h->hero_name || g[0x60] == Heroes[t].hero_name) {
            return a;
        }
        b = *(s16 *)(g + 0x4A);
        h2 = &Heroes[b];
        t = h2->hero_teammate;
        if (t == -1) {
            t = *(u16 *)(g + 0x4A);
        }
        if (g[0x60] == h2->hero_name || g[0x60] == Heroes[t].hero_name) {
            return b;
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (GameWinner[0] == Heroes[i].hero_name) {
                r = i;
            }
        }
    }
    return r;
}

const char PanelDigits123[] = "123";

const char PanelDigits456[] = "456";

void Cups_Update(void) {
    s32 key;
    u16 *p;
    s32 mode;
    s32 i;
    s32 num;
    s32 n = 0;
    s32 used;
    s32 slot;
    s16 r;
    s32 t;
    u8 v;

    if (CupTimer != 0) {
        CupTimer--;
    }
    if (!(GameLevelFlags & 0x100)) {
        s32 j;
        j = 0;
        key = GameLevelFlags & 0xFF;
        p = D_8009AF38;
        do {
            if (key == *p) {
                mode = j;
                goto found;
            }
            j++;
            p++;
        } while (j < 7);
        mode = 0;
    } else {
        mode = 7;
    }
found:
    switch ((u16)CupMode) {
    case 0:
        if ((MODEV->unk30 & 1) && !(MODEV->unk30 & 0x8000)) {
            MODEV->unk14 = 0;
            MODEV->unk10 |= 0x4000000;
            if (MODEV->unk60 == 0) {
                music_set_tempo(100, 5);
                music_flag_restart();
                RecordWin();
                CupWinner = -1;
                CupMode = 0xC;
                CupTimer = 0x78;
            } else if (MODEV->unk32 & 0x100) {
                CupWinner = FindTeamLeader();
                slot = MODEV->unk34;
                if (CupWinner != -1) {
                    MODEV->unk168[slot] = 1;
                    SetReturnGateway();
                }
                CupMode = 0x10;
                CupTimer = 0x78;
            } else {
                if (MODEV->unk34 == 0x20) {
                    CupWinner = FindTeamLeader();
                    if (CupWinner != -1) {
                        MODEV->unkCC[CupWinner]++;
                        ScreenLabelsStart();
                    }
                } else {
                    CupWinner = FindTeamLeader();
                    if (CupWinner != -1) {
                        MODEV->unkCC[CupWinner]++;
                        r = CupWinner;
                        t = Heroes[r].hero_teammate;
                        if (t != -1) {
                            MODEV->unkCC[t] = MODEV->unkCC[r];
                        }
                        ScreenLabelsStart();
                    }
                }
                CupMode = 1;
                CupTimer = 2;
            }
        }
        break;
    case 1:
        if (CupTimer == 0) {
            CupMode = 3;
            CupTimer = 0x5A;
        }
        break;
    case 3:
        if (CupTimer != 0) {
            break;
        }
        CupMode = 0xD;
        CupTimer = 0x1E;
        if (MODEV->unk42 == -1) {
            MODEV->unk42 = CupWinner;
        } else if (MODEV->unkCC[CupWinner] > MODEV->unkCC[MODEV->unk42]) {
            MODEV->unk42 = CupWinner;
        }
        printf("mode%x\n", mode);
        switch (mode) {
        case 2:
            if (MODEV->unkCC[CupWinner] < Trophy_Cups) {
                break;
            }
            if ((MODEV->unk4 & 0x800) || (MODEV->unk30 & 0x200)) {
                MODEV->unkCC[CupWinner] = Trophy_Cups;
                t = Heroes[CupWinner].hero_teammate;
                if (t != -1) {
                    MODEV->unkCC[t] = MODEV->unkCC[CupWinner];
                }
                break;
            }
            if (MODEV->unk6C != 0) {
                MODEV->unkA7[2] = (CupWinner & 2) ? 0 : 2;
                MODEV->unkA7[0] = CupWinner & 2;
                MODEV->unkA7[1] = CupWinner ^ 1;
                MODEV->unkA7[3] = (CupWinner & 2) ? 1 : 3;
            } else {
                n = 0;
                num = 3;
                used = 0;
                while (1) {
                    if (num < 0) {
                        break;
                    }
                    i = 0;
                    while (1) {
                        if (i >= 4) {
                            break;
                        }
                        if (MODEV->unkCC[i] == num && !((used >> i) & 1)) {
                            MODEV->unkA7[n] = i;
                            used |= 1 << i;
                            n++;
                        }
                        i++;
                    }
                    num--;
                }
            }
            CupMode = 5;
            slot = MODEV->unk34;
            if (MODEV->unk5C[CupWinner] == 0) {
                MODEV->unkE4[slot] = 1;
            }
            if (MODEV->unkE4[slot] != 0) {
                MODEV->unkAB[slot] = 0;
            } else {
                v = ++MODEV->unkAB[slot];
                if (v >= 4) {
                    MODEV->unkAB[slot] = 3;
                }
            }
            break;
        case 3:
            if (MODEV->unkCC[0] < Trophy_Cups) {
                break;
            }
            slot = MODEV->unk34;
            if (MODEV->unk5C[CupWinner] == 0) {
                MODEV->unk105[slot] = 1;
            }
            CupMode = 6;
            break;
        case 4:
            if (MODEV->unkCC[0] < Trophy_Cups) {
                break;
            }
            slot = MODEV->unk34;
            if (MODEV->unk5C[CupWinner] == 0) {
                MODEV->unk105[slot] = 2;
            }
            CupMode = 6;
            break;
        case 5:
            if (MODEV->unkCC[0] == 0) {
                break;
            }
            slot = MODEV->unk34;
            if (MODEV->unk5C[CupWinner] == 0) {
                MODEV->unk126[slot] = 1;
            }
            CupMode = 7;
            break;
        case 6:
            if (MODEV->unkCC[0] == 0) {
                break;
            }
            slot = MODEV->unk34;
            if (MODEV->unk5C[CupWinner] == 0) {
                MODEV->unk147[slot] = 1;
            }
            CupMode = 8;
            break;
        case 0:
        case 1:
            if (MODEV->unkCC[CupWinner] < Trophy_Cups) {
                break;
            }
            if ((MODEV->unk4 & 0x800) || (MODEV->unk30 & 0x200)) {
                MODEV->unkCC[CupWinner] = Trophy_Cups;
                t = Heroes[CupWinner].hero_teammate;
                if (t != -1) {
                    MODEV->unkCC[t] = MODEV->unkCC[CupWinner];
                }
                break;
            }
            if (MODEV->unk6C != 0) {
                MODEV->unkA7[2] = (CupWinner & 2) ? 0 : 2;
                MODEV->unkA7[0] = CupWinner & 2;
                MODEV->unkA7[1] = CupWinner ^ 1;
                MODEV->unkA7[3] = (CupWinner & 2) ? 1 : 3;
            } else {
                n = 0;
                num = 3;
                used = 0;
                while (1) {
                    if (num < 0) {
                        break;
                    }
                    i = 0;
                    while (1) {
                        if (i >= 4) {
                            break;
                        }
                        if (MODEV->unkCC[i] == num && !((used >> i) & 1)) {
                            MODEV->unkA7[n] = i;
                            used |= 1 << i;
                            n++;
                        }
                        i++;
                    }
                    num--;
                }
            }
            if (mode == 0) {
                CupMode = 4;
            } else {
                CupMode = 0xB;
                MODEV->unk9F[0] += MODEV->unkCC[0];
                MODEV->unk9F[1] += MODEV->unkCC[1];
                MODEV->unk9F[2] += MODEV->unkCC[2];
                MODEV->unk9F[3] += MODEV->unkCC[3];
            }
            break;
        }
        break;
    case 5:
        if (MODEV->unk5C[CupWinner] != 0) {
            D_800ADBCC = 1;
            PauseChoice = 0;
            CupTimer = 900;
            MODEV->unk10 |= 0x21;
            PlaySfx(0x19E, NULL, NULL, 0x1000, 0x1E00, 0);
            CupMode = 9;
            break;
        }
        if (MODEV->unk34 != 0x20) {
            D_800ADBCC = 1;
            CupTimer = 0x78;
        }
        CupMode = 0xB;
        break;
    case 6:
        D_800ADBCC = 1;
        CupTimer = 0x78;
        CupMode = 0xB;
        break;
    case 8:
        D_800ADBCC = 1;
        CupTimer = 0x78;
        CupMode = 0xB;
        break;
    case 7:
        D_800ADBCC = 1;
        CupTimer = 0x78;
        CupMode = 0xB;
        break;
    case 17:
        CupTimer = 0x1E;
        CupMode = 0xB;
        break;
    case 4:
        D_800ADBCC = 1;
        PauseChoice = 0;
        CupTimer = 900;
        MODEV->unk10 |= 0x201;
        PlaySfx(0x19E, NULL, NULL, 0x1000, 0x1E00, 0);
        CupMode = 0xA;
        break;
    case 9:
        for (i = 0; i < 4; i++) {
            if (HUDPLAYERS[i]->unk10.y >= -0xFF) {
                HUDPLAYERS[i]->unk10.y -= 8;
            }
        }
        if (Clock_XYZ[1] >= -0xFF) {
            Clock_XYZ[1] -= 8;
        }
        if (BigScore1.unk10.y >= -0xFF) {
            BigScore1.unk10.y -= 8;
        }
        if (BigScore2.unk10.y >= -0xFF) {
            BigScore2.unk10.y -= 8;
        }
        if (CupTimer == 0) {
            if (PauseChoice == 0) {
                MODEV->unk42 = -1;
                MODEV->unkCC[0] = 0;
                MODEV->unkCC[1] = 0;
                MODEV->unkCC[2] = 0;
                MODEV->unkCC[3] = 0;
                CupMode = 0xD;
            } else {
                ScreenLabelsOff();
                RequestQuit();
                MODEV->unk10 &= ~1;
            }
        }
        CupTimer++;
        break;
    case 10:
        for (i = 0; i < 4; i++) {
            if (HUDPLAYERS[i]->unk10.y >= -0xFF) {
                HUDPLAYERS[i]->unk10.y -= 8;
            }
        }
        if (Clock_XYZ[1] >= -0xFF) {
            Clock_XYZ[1] -= 8;
        }
        if (BigScore1.unk10.y >= -0xFF) {
            BigScore1.unk10.y -= 8;
        }
        if (BigScore2.unk10.y >= -0xFF) {
            BigScore2.unk10.y -= 8;
        }
        if (CupTimer == 0) {
            switch (PauseChoice) {
            case 0:
                MODEV->unk42 = -1;
                MODEV->unkCC[0] = 0;
                MODEV->unkCC[1] = 0;
                MODEV->unkCC[2] = 0;
                MODEV->unkCC[3] = 0;
                CupMode = 0xD;
                MODEV->unk10 &= ~1;
                break;
            case 1:
                MODEV->unk10 |= 0x40;
                break;
            case 3:
                NextGameMode = 0x28;
                MODEV->unk30 = 0x8008;
                MODEV->unk10 &= ~0x21;
                break;
            case 2:
            default:
                ScreenLabelsOff();
                RequestQuit();
                MODEV->unk10 &= ~1;
                break;
            }
        }
        CupTimer++;
        break;
    case 12:
        {
            s32 ok = UpdateHeroVictory(-1) ^ 1;
            if (ok) {
                UpdateWinCounter(0);
                break;
            }
        }
        switch (mode) {
        case 3:
        case 4:
        case 5:
        case 6:
            PauseChoice = 0;
            CupTimer = 900;
            MODEV->unk10 |= 0x401;
            PlaySfx(0x19E, NULL, NULL, 0x1000, 0x1E00, 0);
            CupMode = 0xF;
            break;
        case 7:
            PauseChoice = 0;
            CupTimer = 900;
            MODEV->unk10 |= 0x401;
            PlaySfx(0x19E, NULL, NULL, 0x1000, 0x1E00, 0);
            CupMode = 0xF;
            DrawPauseMenu();
            break;
        default:
            CupMode = 0xE;
            break;
        }
        break;
    case 15:
        if (CupTimer == 0) {
            if (PauseChoice == 0) {
                MODEV->unk42 = -1;
                MODEV->unkCC[0] = 0;
                MODEV->unkCC[1] = 0;
                MODEV->unkCC[2] = 0;
                MODEV->unkCC[3] = 0;
                CupMode = 0xD;
            } else {
                ScreenLabelsOff();
                RequestQuit();
                MODEV->unk10 &= ~1;
            }
        }
        CupTimer++;
    case 14:
        UpdateWinCounter(0);
    case 13:
        if (CupTimer != 0) {
            break;
        }
        MODEV->unk30 |= 0x8200;
        ScreenLabelsOff();
        music_duck_off();
    case 11:
    case 16:
        if (CupTimer == 0) {
            ScreenLabelsOff();
            RequestQuit();
        }
        break;
    }
}

void PanelCountCompletion(void) {
    count_completion(GameInfo);
}

s32 CalcElapsedTime(void) {
    s32 var_v0;

    var_v0 = D_8005A620 - D_8009BD04;
    if (var_v0 < 0) {
        var_v0 = 0;
    }
    return var_v0;
}

void InitTrophyCups(void) {
    u8 *p;
    u16 f;

    CupMode = 0;
    CupTimer = 0;
    Trophy_Cups = 3;
    p = GameInfo;
    f = *(u16 *)(p + 0x32);
    if (f & 3) {
        Trophy_Cups = *(u16 *)(p + 0x3A);
    }
    if (f & 0x20) {
        if (f & 8) {
            Trophy_Cups = 3;
        } else {
            Trophy_Cups = 2;
        }
    }
    if (GameMode == 0x20) {
        Trophy_Cups = 3;
    }
}

void StartPanelCountdown(void) {
    D_8009AE78 = 0xF0;
    PanelCtrl |= 0x8000;
}

s32 StillTottingUpScore(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_score != Heroes[i].hero_score_displayed) {
            return 1;
        }
    }
    return 0;
}

void MakeSubBar(EngineBox *p, s16 x, s16 y, u32 a, u32 b) {
    p->unk54 = draw_nas_model_flags;
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

void SetPanelHeroIconA(u16 arg0, s16 arg1, s16 arg2) {
    if (PanelCtrl & 2) {
        void *p = &HeroModelBank[arg0 * 0x38];
        subengine *s = IconParts[2];
        s->unk6C = p;
        s->unk74 = arg1;
        Icon.a[4] = D_80099208[4];
        Icon.a[5] = D_80099208[5];
        Icon.a[6] = 0;
        s = (subengine *)((u8 *)s + 0x498);
        s->unk6C = p;
        s->unk74 = arg2;
        Icon.b[4] = D_80099208[6];
        Icon.b[5] = D_80099208[7];
        Icon.b[6] = 0;
    }
}

void SetPanelHeroIconB(u16 arg0, s16 arg1) {
    if (PanelCtrl & 2) {
        subengine *s = IconParts[3];
        s->unk6C = &HeroModelBank[arg0 * 0x38];
        s->unk74 = arg1;
        Icon.b[4] = D_80099208[2];
        Icon.b[5] = D_80099208[3];
        Icon.b[6] = 0;
        if (GameTeams[0] == 0) {
            Icon.c[4] = D_80099208[0];
            Icon.c[5] = D_80099208[1];
            Icon.c[6] = 0;
        }
    }
}

void InitCompletionCounter(void) {
    D_8004E0D0 = 0;
    D_80099194 = 0;
    D_80099700[0] = 0x138;
    D_80099700[1] = 0x20;
    D_80099700[2] = 0;
    PanelCtrl |= 0x20000;
}

void Clock_Off(void) {
    PanelCtrl &= ~1;
}

void Init_Clock(s32 arg0, s32 arg1, s32 arg2) {
    Clock_Time = 0;
    Clock_flags = 0;
    Clock_XYZ[0] = arg0 - 8;
    Clock_XYZ[1] = arg1 - 8;
    Clock_XYZ[2] = arg2;
    Clock_last_ping = 0x63;
    PanelCtrl |= 1;
}

void InitCountdown(void) {
    subengine *sub;
    subengine *counter;

    counter = (subengine *)&PanelTab[4];
    countdown_eng = counter;
    counter->unk0C = 0x8000;
    *(s32 *)&counter->unk10 = 0;
    *(s32 *)&counter->unk14 = 0;
    counter->unk18 = 0;
    counter->unk6C = &counter_sub;
    sub = &counter_sub;
    if (sub != NULL) {
        *sub = SubEngineTemplate;
        sub->unk6C = InterfaceModel;
        sub->unk74 = 0x2004;
        sub->flags = 0x11000000;
        sub->unk04 = 0x140;
        sub->unk08 = 0xB4;
        sub->unk50 = 1;
        sub->unk0C = 0;
        sub->unk20.x = 0xA00;
        sub->unk20.y = 0xA00;
        sub->unk20.z = 0xA00;
        sub->unk51 = 0;
        sub->next = NULL;
        sub->prev = NULL;
    }
}

void Init_Panel(void) {
    s32 i;

    PanelCtrl = 0;
    do {
        for (i = 0; i < 7; i++) {
            do {
            } while (0);
            PanelTab[i] = EngineTemplate;
            PanelTab[i].flags = 0;
        }
        ((s32 *)&Icon)[i] = 0;
        i++;
    } while (i < 4);
    InitBossHealthBar();
}

void InitBossHealthBar(void) {
    EXTengine *e;
    EXTengine *f;

    e = &PanelTab[5];
    subs_init_array(e, 1, &D_8009AC04);
    e->flags |= 0x10008000;
    e->sub->flags |= 0x10008000;
    e->sub->unk6C = InterfaceModel;
    e->sub->unk74 = 0x2022;
    e->unk5D = 0;
    e->unk5C = 1;
    e->unk2C.x = 0xE00;
    e->unk2C.y = 0xE00;
    e->unk2C.z = 0xE00;
    e++;
    subs_init_array(e, 1, &D_8009ACAC);
    e->flags |= 0x10008000;
    e->sub->flags |= 0x10008000;
    e->sub->unk6C = InterfaceModel;
    e->sub->unk74 = 0x2022;
    e->unk5D = 0;
    e->unk5C = 1;
    e->unk2C.x = 0xE00;
    e->unk2C.y = 0xE00;
    e->unk2C.z = 0xE00;
}

/* data, in address order (tools/scripts/data2c.py) */
subengine *countdown_eng = 0;
s32 PanelCtrl = 0;
u32 Clock_Time = 0;
u16 Clock_flags = 0;
U16Pair Clock_offsets[5] = { { 0xFFE2, 0xFFFF }, { 0x19, 0 }, { 0xFFF7, 0xFFFF }, { 0xFFC9, 0xFFFF }, { 0xFFBF, 0xFFFF } };
s16 D_80099194 = 0;
DigitSrc D_80099198[16] = {
    { 5, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0xB, 0 }, { 0xFFEA, 0xFFFF }, { 0, 0 }, { 0, 0 },
    { 0x1E, 0 }, { 0xFFF5, 0xFFFF }, { 0xFFD4, 0xFFFF }, { 0, 0 }, { 0x28, 0 }, { 7, 0 },
    { 0xFFE6, 0xFFFF }, { 0xFFC5, 0xFFFF },
};
u16 D_800991D8[4] = { 1, 0, 3, 2 };
s16 D_800991E0[4][2] = { { 0x28, 0x18 }, { 0xA0, 0x18 }, { 0x190, 0x18 }, { 0x208, 0x18 } };
s16 D_800991F0[4][2] = { { 0x64, 0x18 }, { 0x1CC, 0x18 }, { -0x258, 0 }, { 0x258, 0 } };
char *left_score = (void *)&PanelDigits123;
char *right_score = (void *)&PanelDigits456;
s16 D_80099208[8] = { 0x64, 0x18, 0x1CC, 0x18, 0x190, 0x18, 0x208, 0x18 };
u16 IconImage[10] = { 0, 0x3003, 0x3002, 0x3004, 0x3006, 0x3007, 0x3008, 0x3005, 0x3016, 0 };
u16 D_8009922C[10] = { 0, 0x2008, 0x2007, 0x2009, 0x200B, 0x200C, 0x200D, 0x200A, 0x201D, 0 };
EngXY D_80099240[4] = { { -2, -4 }, { -2, -4 }, { -2, -4 }, { -2, -4 } };
SWORD_XY HealthIconPos[4] = { { -8, 0x45 }, { -8, 0x45 }, { -8, 0x45 }, { -8, 0x45 } };
SWORD_XY ScoreIcon3Pos[4] = { { 0x34, 0x32 }, { 0x34, 0x32 }, { 0x34, 0x32 }, { 0x34, 0x32 } };
SWORD_XY ScoreIcon2Pos[4] = { { 0x26, 0x46 }, { 0x26, 0x46 }, { 0x26, 0x46 }, { 0x26, 0x46 } };
SWORD_XY ScoreIcon1Pos[4] = { { 0x26, 0x32 }, { 0x26, 0x32 }, { 0x26, 0x32 }, { 0x26, 0x32 } };
EXTengine PanelTab[7] = {0};
s32 Clock_XYZ[3] = {0};
DigitEnt CompletionDigits[4] = {0};
s32 D_80099700[3] = {0};
IconSet Icon = {0};
subengine IconParts[4][7] = {0};
EXTengine BigScore1 = {0};
EXTengine BigScore2 = {0};
subengine BigScoreParts1 = {0};
subengine BigScoreParts2 = {0};
subengine D_8009AC04 = {0};
subengine D_8009ACAC = {0};
ClockInfo Clock[4] = {0};
ScoreInfo Score[4] = {0};
subengine counter_sub = {0};
s32 CupTimer = 0;
EngPtr3 D_8009AE40[4] = {0};
u16 CupMode = 0;
s16 CupWinner = 0;
u16 Trophy_Cups = 0;
u16 Clock_last_ping = 0;
s32 D_8009AE78 = 0;
u16 BigScore_scale = 0;
u16 BigScore_teamA = 0;
u16 BigScore_teamB = 0;
u16 BigScore_Adelay = 0;
u16 BigScore_Bdelay = 0;
u16 *pHeroHealth[4] = {0};
