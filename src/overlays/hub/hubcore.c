#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/hub_ovl.h"
#include "bash/hub_hubcore.h"
#include "bash/hub.h"
#include "bash/n2o.h"
#include "bash/titles.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern u8 D_8004ECA0[];
extern u8 D_8004ECA8[];
extern u8 D_8004ECB0[];
extern u8 D_8004ECB8[];
extern u8 D_8004ECC0[];
extern u8 D_8004ECC8[];
extern u8 D_8004ECD0[];
extern u8 D_8004ECD8[];
extern u8 D_800500A0[];
extern u8 D_80063BB8[];
extern u8 D_80063EC8[];

void InitialiseHUB(void) {
    HeroInfo *hero;
    EXTengine *eng;
    u16 size;

    D_800BCEF0 = 0;
    D_8009F71C = 0;
    D_800BCEF4 = 0;
    hero = Heroes;
    World_floor_level = 0;
    eng = hero->hero_eng;
    G6A->unk32 |= 0x200;
    eng->flags |= 0x8000;
    D_8009D6E8.unk44 = HubHeroTestA;
    D_8009D6E8.unk48 = HubHeroTestB;
    D_8009D6E8.unk3C = HubHeroTestButton;
    D_8009D6E8.unk30 = HubHeroSetIdle;
    HubInitHero(0);
    if (G6A->unk34 == 0x22) {
        HubSpawnEffects();
    }
    SetHeroAction(0, 0);
    size = D_800BCBDC[hero->hero_name];
    eng->unk2C.z = size;
    eng->unk2C.y = size;
    eng->unk2C.x = size;
    G6A->unk62 = func_8009020C(G6A->unk34);
    HubInitGateways();
    func_80095258(*(u8 **)D_80058AD0);
    HubInitLevelSelect();
    D_800BF260 = 0;
    if (G6A->unk34 == 0x25 && G6A->unk75 == 0) {
        CAM6A->unk4 = 0xEBC;
        G6A->unk63 = -1;
        CAM6A->unk8 = (CAM6A->unkC + CAM6A->unk10) >> 1;
    } else if (G6A->unk64 != 0) {
        G6A->unk64 = 0;
        CAM6A->unk4 = CAMTBL6A[G6A->unk63].unk10;
        CAM6A->unk8 = (CAMTBL6A[G6A->unk63].unkC + CAMTBL6A[G6A->unk63].unk8) >> 1;
    } else {
        switch (((s32(*)(void))HubGatewayKind)()) {
        case 5:
            G6A->unk63 = -1;
            CAM6A->unk8 = (CAM6A->unkC + CAM6A->unk10) >> 1;
            CAM6A->unk4 = CAMTBL6A[0].unk10;
            break;
        case 2:
            CAM6A->unk8 = (CAMTBL6A[G6A->unk63].unkC + CAMTBL6A[G6A->unk63].unk8) >> 1;
            break;
        default:
            G6A->unk63 = -1;
            CAM6A->unk8 = (CAM6A->unkC + CAM6A->unk10) >> 1;
            break;
        }
    }
    HubCamTarget();
    D_800BCEEC = 0;
    func_8001E934(&D_8009F8A4, &D_800BCC04);
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char D_800B330C[] = "COMPLETE ALL CHALLENGES\nTO UNLOCK THIS LEVEL";
const char D_800B333C[] = "YOU NEED 4 TROPHIES\nTO FACE PAPU PAPU";
const char D_800B3364[] = "YOU NEED 9 TROPHIES, 6 GEMS\nAND 3 CRYSTALS TO FACE THE BEARMINATOR";
const char D_800B33A8[] = "YOU NEED 15 TROPHIES, 10 GEMS\nAND 7 CRYSTALS TO FACE THE TANK BOSS";
const char D_800B33EC[] = "YOU NEED 22 TROPHIES, 15 GEMS\nAND 12 CRYSTALS TO FACE N. OXIDE";
const char D_800B342C[] = "YOU NEED 19 CRYSTALS\nTO OPEN DRAGON DROP";
const char D_800B3458[] = "YOU NEED 21 GOLD RELICS\nTO OPEN MALLET MASH";
const char D_800B3484[] = "YOU NEED 25 GEMS\nTO OPEN KEG KABOOM";
const char D_800B34A8[] = "YOU NEED 27 TROPHIES\nTO OPEN DANTE'S DASH";
const char D_800B34D4[] = "YOU NEED 17 GOLD RELICS\nTO OPEN SPLASH DASH";
const char D_800B3500[] = "YOU NEED 23 CRYSTALS\nTO OPEN SWAMP FOX";
const char D_800B3528[] = "PRESS \374 BUTTON TO SAVE";
const char D_800B3540[] = "PRESS \374 BUTTON TO START";
const char D_800B3558[] = "PRESS LEFT - RIGHT TO SELECT CHALLENGE";
const char D_800B3580[] = "PRESS LEFT - RIGHT TO SELECT WARP ROOM";
const char D_800B35A8[] = "PRESS \374 BUTTON TO ENTER";
const char D_800B35C0[] = "YOU MUST DEFEAT PAPU PAPU BEFORE\nYOU CAN GO TO THE NEXT WARP ROOM";

void HubInitGateways(void) {
    s32 i;
    EngRec90248 *p;
    u8 *o;
    u8 *obj;
    EngSub90248 *e;
    Vec3W ref;
    Vec3W d;

    for (i = 0; i < 12; i++) {
        D_800BCCF4[i].unk0 = NULL;
        D_800BCCF4[i].unk4 = NULL;
        D_800BCCF4[i].unk18 = NULL;
        D_800BCCF4[i].unk8 = 0;
        D_800BCCF4[i].unkC = 0;
        D_800BCCF4[i].unk10 = 0;
        D_800BCCF4[i].unk14 = NULL;
        D_800BCCF4[i].unk1C = 0;
        D_800BCCF4[i].unk20 = 0;
        D_800BCCF4[i].unk24 = 0;
    }
    o = *(u8 **)((u8 *)D_80058AD0 + 0x1C);
    for (;;) {
        s32 k;
        void *g;
        EngRec90248 *q;

        if (o == NULL) {
            break;
        }
        i = o[0x65] - 1;
        if ((u32)i < 12) {
            switch (o[0x64]) {
            case 1:
                D_800BCCF4[i].unk0 = (s32 *)o;
                break;
            case 2:
                D_800BCCF4[i].unk4 = (s32 *)o;
                break;
            case 3:
                break;
            case 5:
                break;
            case 4:
                q = &D_800BCCF4[i];
                q->unk18 = o;
                k = *(s32 *)o;
                g = D_80058AD0;
                *(void **)(o + 0x54) = HubDrawObj;
                *(s32 *)o = k & ~0x8000;
                q->unk24 = func_80015984(*(u16 *)(o + 0x74), *(EntryBlock **)(*(u8 **)g + 0xC));
                break;
            }
        }
        o = *(u8 **)(o + 0x5C);
    }
    for (e = (EngSub90248 *)D_8009EDD4[D_8005A676][1]; e->unk0 != 0; e++) {
        i = e->unk0 - 1;
        p = &D_800BCCF4[i];
        p->unk14 = e;
        if (e->unk6 & 1) {
            if (p->unk0 != NULL) {
                *p->unk0 |= 0x8000;
            }
            if (p->unk4 != NULL) {
                *p->unk4 |= 0x8000;
            }
        } else {
            if (p->unk0 != NULL) {
                *p->unk0 &= ~0x8000;
            }
            if (p->unk4 != NULL) {
                *p->unk4 &= ~0x8000;
            }
        }
    }
    ref = *(Vec3W *)((u8 *)path_get(2) + 0x30);
    i = 0;
    for (;;) {
        s32 k;

        if (i >= 12) {
            break;
        }
        p = &D_800BCCF4[i];
        if (p->unk14 != NULL) {
            obj = (u8 *)path_get(i + 0x65);
            if (obj != NULL) {
                d = *(Vec3W *)(obj + 0x30);
                d.a -= ref.a;
                d.c -= ref.c;
                p->unk8 = csqrt(d.a * d.a + d.c * d.c) >> 6;
                p->unk10 = get_angle(d.a, d.c);
                d = *(Vec3W *)(obj + 0x4C);
                d.a -= ref.a;
                d.c -= ref.c;
                p->unkC = csqrt(d.a * d.a + d.c * d.c) >> 6;
            }
        }
        i++;
    }
}

void HubUpdateGateways(void) {
    s32 i;
    s32 *o;
    s32 *q;
    char one;
    EngRec90248 *p;

    for (i = 0, one = 1, p = D_800BCCF4; i < 12; i++, p++) {
        q = &p->unk24;
        if (q[-4] != 0) {
            o = (s32 *)q[-3];
            if (o != NULL) {
                s32 v = D_80068BD4[(((D_8004E0D8 * 0x50) & 0xFFF) - 0x400) & 0xFFF].unk2;
                o[2] = v * 0x20 >> 12;
            }
            if (q[-2] == one) {
                if (q[0] == 0 || (func_800163E0(*(u8 **)D_80058AD0, q[0]), q[0] == 0) ||
                    func_80016450(*(u8 **)D_80058AD0, q[0]) != 0) {
                    if (q[-1] == one) {
                        if (o != NULL) {
                            o[0] |= 0x8000;
                        }
                        D_800BF260 += 0x100;
                        if (D_800BF260 >= 0x1000) {
                            D_800BF260 = 0x1000;
                        }
                    } else {
                        D_800BF260 -= 0x100;
                        if (D_800BF260 < 0) {
                            D_800BF260 = 0;
                            if (o != NULL) {
                                o[0] &= ~0x8000;
                            }
                        }
                    }
                }
            } else {
                if (o != NULL) {
                    o[0] &= ~0x8000;
                }
                if (q[0] != 0) {
                    func_8001636C(*(u8 **)D_80058AD0, q[0]);
                }
            }
        }
    }
}

void HubPreMoveHero(s32 arg0) {
    HubGameView *g;
    HubGameView *gv;
    EXTengine *eng;
    EXTheromove *mv;
    EngRec90248 *p;
    s32 ang;
    s32 step;
    s32 stop;
    s32 i;
    s32 best;
    s32 bestd;
    s32 d;
    s32 lim;
    s32 t;
    s32 old;
    s32 n;
    s32 j;
    u32 rate;
    s32 speed;
    s32 sel;
    s32 mode;
    u16 v;

    eng = Heroes[arg0].hero_eng;
    mv = Heroes[arg0].hero_move;
    speed = D_8009D6F4[Heroes[arg0].hero_name * 3 + 1];
    rate = (u32)(D_8009EEA4 * 0x648) >> 8;
    if (rate != 0) {
        step = (s32)(speed << 12) / (s32)rate;
    } else {
        step = 0;
    }
    stop = 0;
    ang = eng->unk1E;
    if (((HubGameView *)D_8005A614)->unk74 != 0) {
        eng->flags &= ~0x8000;
        if (D_8009D538[0] != NULL) {
            D_8009D538[0]->flags &= ~0x8000;
        }
        return;
    }
    mv->forward_speed = 0;
    D_800BCEF4 = 0;
    if (((HubGameView *)D_8005A614)->unk30 & 0x8008) {
        return;
    }
    if (Heroes[arg0].hero_frozen != 0) {
        return;
    }
    if (((HubGameView *)D_8005A614)->unk63 == -1) {
        if (((HubPad *)D_80051338)[arg0].unk50 & 0x10) {
            mv->forward_speed = speed;
            ang = D_8009EE9C.unk4 + 0x800;
            D_8009EE9C.unk8 += speed;
            if ((u32)D_8009EE9C.unk10 < (u32)D_8009EE9C.unk8) {
                best = -1;
                bestd = 0x1000;
                for (i = 0; i < 12; i++) {
                    if (D_800BCCF4[i].unk14 != NULL && (D_800BCCF4[i].unk14->unk6 & 2)) {
                        d = func_80015480(D_800BCCF4[i].unk10, D_8009EE9C.unk4);
                        if (d < bestd) {
                            bestd = d;
                            if ((u32)d < 0x81) {
                                best = i;
                            }
                        }
                    }
                }
                if (best != -1) {
                    D_8009EE9C.unk4 = D_800BCCF4[best].unk10;
                    ((HubGameView *)D_8005A614)->unk63 = best;
                    D_800BCCF4[best].unk1C = 1;
                } else {
                    D_8009EE9C.unk8 = D_8009EE9C.unk10;
                    stop++;
                    mv->forward_speed = 0;
                }
            }
        }
        if (((HubPad *)D_80051338)[arg0].unk50 & 0x40) {
            ang = D_8009EE9C.unk4;
            mv->forward_speed = speed;
            D_8009EE9C.unk8 -= speed;
            if ((u32)D_8009EE9C.unk8 < (u32)D_8009EE9C.unkC) {
                stop++;
                D_8009EE9C.unk8 = D_8009EE9C.unkC;
                mv->forward_speed = 0;
            }
        }
        if (stop == 0) {
            if (((HubPad *)D_80051338)[arg0].unk50 & 0x80) {
                mv->forward_speed = speed;
                ang = 0x400;
                ang = (D_8009EE9C.unk4 -= step % 0x1000) + ang;
            }
            if (((HubPad *)D_80051338)[arg0].unk50 & 0x20) {
                mv->forward_speed = speed;
                ang = (D_8009EE9C.unk4 += step) - 0x400;
            }
            D_8009EE9C.unk4 &= 0xFFF;
        }
    } else {
        if (((HubPad *)D_80051338)[arg0].unk50 & 0x10) {
            mv->forward_speed = speed;
            t = D_8009EE9C.unk8 + speed;
            D_8009EE9C.unk8 = t;
            lim = D_800BCCF4[((HubGameView *)D_8005A614)->unk63].unkC;
            ang = D_8009EE9C.unk4 + 0x800;
            if ((u32)lim < (u32)t) {
                D_8009EE9C.unk8 = lim;
                mv->forward_speed = 0;
            }
        }
        if (((HubPad *)D_80051338)[arg0].unk50 & 0x40) {
            ang = D_8009EE9C.unk4;
            mv->forward_speed = speed;
            D_8009EE9C.unk8 -= speed;
            if ((u32)D_8009EE9C.unk8 < (u32)D_8009EE9C.unk10) {
                D_8005A677 = -1;
            }
        }
        if ((u8)D_8005A677 >= 12) {
            goto done;
        }
        old = D_800BCCF4[D_8005A677].unk20;
        if ((u32)(D_800BCCF4[D_8005A677].unkC - D_8009EEA4) < 0x100) {
            D_800BCCF4[D_8005A677].unk20 = 1;
        } else {
            D_800BCCF4[D_8005A677].unk20 = 0;
        }
        if (D_800BCCF4[D_8005A677].unk20 != old) {
            if (D_800BCCF4[D_8005A677].unk20 != 0) {
                switch (((s32(*)(void))HubGatewayKind)()) {
                case 2:
                    PlaySfx(0x399, NULL, NULL, 0x1000, 0x1E00, 0);
                    break;
                case 5:
                    PlaySfx(((s32(*)(void))func_800900CC)() >= 0 ? 0x39E : 0x39D, &D_800BCEEC, NULL, 0x1000, 0x1E00, 0);
                case 4:
                    PlaySfx(0x194, NULL, NULL, 0x1000, 0x1E00, 0);
                    break;
                }
            } else {
                switch (((s32(*)(void))HubGatewayKind)()) {
                case 2:
                    PlaySfx(0x39A, NULL, NULL, 0x1000, 0x1E00, 0);
                    break;
                case 5:
                    if (D_800BCEEC != NULL) {
                        StopSfx(D_800BCEEC);
                    }
                case 4:
                    PlaySfx(0x194, NULL, NULL, 0x1000, 0x1E00, 0);
                    break;
                }
            }
        }
        if (!(((HubPad *)D_80051338)[arg0].unk54 & 0x4000)) {
            goto done;
        }
        p = D_800BCCF4;
        g = (HubGameView *)D_8005A614;
        if ((u32)(p[g->unk63].unkC - D_8009EEA4) >= 0x100) {
            goto done;
        }
        switch (((s32(*)(void))HubGatewayKind)()) {
        case 5:
            n = HubSelectedLevel();
            if (n >= 0 && ((s32(*)(s32))func_8007A6D0)(n & 0xFFFF) == 1) {
                HubGoToMode(n);
                SetHeroAction(0, 1);
                if (Heroes[0].hero_shadow != NULL) {
                    Heroes[0].hero_shadow->flags &= ~0x8000;
                }
                HubSpawnFlash(&Heroes[0].hero_eng->unk10.x, 9);
            } else {
                PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            break;
        case 2:
            sel = p[g->unk63].unk14->unk4;
            if ((g->unk4 & 0x2000) || ((g->unkE4[sel] != 0 || D_800BCED4 <= 0) && ((s32(*)(s32))func_8007A6D0)(sel) == 1)) {
            mode = D_800BCED4;
            if (mode != 4 || ((HubGameView *)D_8005A614)->unk105[sel] != 0) {
            gv = (HubGameView *)D_8005A614;
            v = gv->unk32 & 0xFF07;
            gv->unk32 = v;
            switch (mode) {
            case 3:
            case 4:
                gv->unk32 = v | 0x20;
                if (mode == 4) {
                    gv->unk32 = v | 0x28;
                }
                break;
            case 2:
                gv->unk32 = v | 0x80;
                break;
            case 1:
                gv->unk32 = v | 0x40;
                break;
            default:
                ((HubGameView *)D_8005A614)->unk32 |= 0x10;
                break;
            }
            HubSpawnFlash(&Heroes[0].hero_eng->unk10.x, 9);
            HubGoToMode(D_800BCCF4[D_8005A677].unk14->unk4);
            SetHeroAction(0, 1);
            if (Heroes[0].hero_shadow != NULL) {
                Heroes[0].hero_shadow->flags &= ~0x8000;
            }
            } else {
                PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            } else {
                PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            break;
        case 4:
            HubSpawnFlash(&Heroes[0].hero_eng->unk10.x, 9);
            SetHeroAction(0, 1);
            if (Heroes[0].hero_shadow != NULL) {
                Heroes[0].hero_shadow->flags &= ~0x8000;
            }
            g->unk30 |= 0x200;
            func_80092AF4(&D_8009F46C, 0x200);
            break;
        default:
            HubSpawnFlash(&D_8009D530->unk10.x, 9);
            break;
        }
    }
done:
    for (j = 11; j >= 0; j--) {
        D_800BCCF4[j].unk1C = 0;
    }
    if (D_8005A677 != -1) {
        D_800BCCF4[D_8005A677].unk1C = 1;
    }
    mv->forward_angle = ang;
    if (Heroes[arg0].hero_aseq != 10 && D_800BCEF0 != 0) {
        D_800BCEF0 = 0;
        Heroes[arg0].hero_eng->unk1E = ang;
        mv->facing_angle = ang;
    }
    HubCamTarget();
}

void HubOpenDoorMenu(s32 arg0) {
    EXTengine *e;
    subengine *sub;
    subengine *p;
    s32 w;
    s32 n;

    D_800BCA20[5].unk1E = 4;
    D_800BCA20[6].unk1E = 4;
    if (((HubGameView *)D_8005A614)->unk105[arg0] != 0) {
        D_800BCA20[5].unk1E = 3;
        if (((HubGameView *)D_8005A614)->unk105[arg0] >= 2) {
            D_800BCA20[6].unk1E = 2;
        }
    }
    n = ((HubGameView *)D_8005A614)->unkE0;
    if (n >= 2 && ((HubGameView *)D_8005A614)->unk105[arg0] == 0) {
        D_800BCEFC = 4;
    } else {
        D_800BCEFC = n + 3;
    }
    if (((HubGameView *)D_8005A614)->unk4 & 0x2000) {
        D_800BCEFC = ((HubGameView *)D_8005A614)->unkE0 + 3;
    }
    D_800BCA20[4].unk1E = ((HubGameView *)D_8005A614)->unk147[arg0] ? 7 : 8;
    D_800BCA20[3].unk1E = ((HubGameView *)D_8005A614)->unk126[arg0] ? 5 : 6;
    e = GetEngineSlot(1);
    func_80095BEC(1, (s32 *)D_800BCA20);
    e->unk10.y = 200;
    e->unk10.x = 0;
    D_800BCED4 = 0;
    e->flags = (e->flags & ~0x8000) | 0x10000000;
    if (((HubGameView *)D_8005A614)->unkE4[arg0] != 0) {
        D_800BCED4 = 1;
    }
    if (((HubGameView *)D_8005A614)->unk126[arg0] != 0) {
        D_800BCED4 = 2;
    }
    if (((HubGameView *)D_8005A614)->unk147[arg0] != 0 && ((HubGameView *)D_8005A614)->unkE0 > 0) {
        D_800BCED4 = 3;
    }
    if (((HubGameView *)D_8005A614)->unk105[arg0] != 0 && ((HubGameView *)D_8005A614)->unkE0 >= 2) {
        D_800BCED4 = 4;
        n = 1;
    } else {
        n = 1;
    }
    p = e->sub;
    w = (((D_800BCEFC << n) + D_800BCEFC) * 15) << n;
    p[1].unk76 = w;
    e->unk10.x = 320 - (w >> n);
    sub = GetEngineSlot(n)->sub;
    p = &sub[5];
    switch (D_800BCEFC - 3) {
    case 0:
        sub[5].flags &= ~0x8000;
        p[1].flags &= ~0x8000;
        break;
    case 1:
        p[1].flags &= ~0x8000;
        break;
    case 2:
        break;
    }
}

void HubDoorMenuUpdate(void) {
    HubGameView *g = (HubGameView *)D_8005A614;
    HubGameView *g2;
    HubGameView *g3;
    EngSub90248 *rec;
    subengine *sub;
    s32 *recs;
    s32 i;
    KeyedPair *p;
    s32 key;
    s16 v;
    u16 n;

    if (g->unk63 < 0 || D_800BF260 <= 0
        || (rec = D_800BCCF4[g->unk63].unk14, rec->unk2 != 2)
        || (!(g->unk4 & 0x2000) && g->unkE4[rec->unk4] == 0)) {
        if (D_800BCED4 >= 0) {
            D_800BCED4 = -1;
            func_80095A04(1);
            func_80095A04(2);
        }
    } else {
        if (D_800BCED4 == -1) {
            HubOpenDoorMenu(D_800BCCF4[D_8005A677].unk14->unk4);
        }
        sub = GetEngineSlot(1)->sub;
        if ((D_80051380 & 0x80) && D_8005A644 != 0x8008 && D_800BCED4 > 0) {
            PlaySfx(0x385, NULL, NULL, 0x1000, 0x1E00, 0);
            D_800BCED4--;
        }
        if ((D_80051380 & 0x20) && D_8005A644 != 0x8008 && D_800BCED4 < D_800BCEFC - 1) {
            PlaySfx(0x385, NULL, NULL, 0x1000, 0x1E00, 0);
            D_800BCED4++;
        }
        v = func_80015284(*(s16 *)&sub->unk70, D_800BCB40[D_800BCED4], 2000);
        *(s16 *)&sub->unk70 = v;
        sub->unk76 = v + 90;
    }
    if (D_800BF260 != 0) {
        switch (((s32(*)(void))HubGatewayKind)()) {
        case 2:
            if (!(GetEngineSlot(2)->flags & 0x8000)) {
                n = D_800BCCF4[((HubGameView *)D_8005A614)->unk63].unk14->unk4;
                g2 = (HubGameView *)D_8005A614;
                if (((s32(*)(s32))func_8007A6D0)(n) == 1) {
                    if (g2->unkE4[n] != 0) {
                        func_80095BEC(2, D_800BC918[g2->unk2C]);
                    } else {
                        func_80095BEC(2, D_800BC884[g2->unk2C]);
                    }
                } else {
                    key = func_80090248();
                    p = D_800BCB54;
                    for (i = 10; i != -1; i--) {
                        if (p->key == key) {
                            recs = (s32 *)p->val[D_8005A640];
                            goto found;
                        }
                        p++;
                    }
                    recs = NULL;
                found:
                    func_80095BEC(2, recs);
                }
            }
            GetEngineSlot(2)->unk10.y = (D_800BF260 >> 4) - 200;
            break;
        case 5:
            if (!(GetEngineSlot(2)->flags & 0x8000)) {
                g3 = (HubGameView *)D_8005A614;
                if (g3->unk34 == 0x21 && ((s32(*)(s32))func_8007A6D0)(0x22) == 0) {
                    recs = D_800BCA1C[g3->unk2C];
                } else {
                    recs = D_800BC9AC[g3->unk2C];
                }
                func_80095BEC(2, recs);
            }
            GetEngineSlot(2)->unk10.y = 0x280 - (D_800BF260 >> 4);
            break;
        case 4:
            if (!(GetEngineSlot(2)->flags & 0x8000)) {
                func_80095BEC(2, D_800BC810[D_8005A640]);
            }
            GetEngineSlot(2)->unk10.y = (D_800BF260 >> 4) - 200;
            break;
        }
    } else {
        func_80095A04(2);
    }
}

s32 HubSelectedLevel(void) {
    s32 r = 0;
    s32 v;
    s32 *t;
    s32 k = 5;

    if (D_8005A677 >= 0) {
        v = D_800BCCF4[D_8005A677].unk14->unk2;
    } else {
        v = 0;
    }
    if (v == k && D_800BCCF4[D_8005A677].unk20 != 0) {
        r = 1;
    }
    if (r != 0) {
        s32 (*base)[5];
        r = D_800BCEE0;
        if (r < 0) {
            return -1;
        }
        base = D_8009EDD4;
        t = base[D_8005A676];
        if (r < t[2]) {
            return ((s32 *)(t[3] + r * 8))[1];
        }
        return -1;
    }
    return -1;
}

void HubLevelSelect(void) {
    s32 (*base)[5] = D_8009EDD4;
    HubSelView *g = (HubSelView *)D_8005A614;
    HubSelEntry *ents = (HubSelEntry *)base[g->unk62][3];
    s32 r;
    s32 v;
    s32 k;
    s32 n;
    fade_info *p;

    switch (D_800BCED8) {
    case 0:
        r = 0;
        k = 5;
        if (g->unk63 >= 0) {
            v = D_800BCCF4[g->unk63].unk14->unk2;
        } else {
            v = 0;
        }
        if (v == k && D_800BCCF4[D_8005A677].unk20 != 0) {
            r = 1;
        }
        if (r != 0 && ((HubSelView *)D_8005A614)->unk30 != 0x8008) {
            if ((D_80051380 & 0x20)
                && D_800BCEE0 < D_8009EDD4[((HubSelView *)D_8005A614)->unk62][2] - 1
                && ((s32(*)(s32))func_8007A6D0)(ents[D_800BCEE0 + 1].unk4)) {
                PlaySfx(0x39F, NULL, NULL, 0x1000, 0x1E00, 0);
                D_800BCEE0++;
            }
            if ((D_80051380 & 0x80) && D_800BCEE0 > 0) {
                PlaySfx(0x39F, NULL, NULL, 0x1000, 0x1E00, 0);
                D_800BCEE0--;
            }
            if (D_800BCEE0 < 0) {
                D_800BCEE0 = func_800900CC();
            }
            if (D_800BCEE0 >= D_8009EDD4[D_8005A676][2]) {
                n = D_8009EDD4[D_8005A676][2];
                D_800BCEE0 = n - 1;
            }
            if (D_800BCEE8 != NULL) {
                if (D_800BCEE0 >= 0 && ((s32(*)(s32))func_8007A6D0)(ents[D_800BCEE0].unk4)) {
                    D_800BCEE8->unk74 = ents[D_800BCEE0].unk0;
                    return;
                }
                D_800BCEE8->unk74 = D_8009EDD4[D_8005A676][4];
            }
        } else if (D_800BCEE8 != NULL) {
            D_800BCEE8->unk74 = D_8009EDD4[D_8005A676][4];
        }
        break;
    case 1:
        if (func_80012FFC() == 0) {
            set_fade(&D_8009F644, 0x200, 0);
            D_800BCED8 = 2;
        }
        break;
    case 2:
        p = &D_8009F644;
        if (p->maxed >= 3) {
            func_8001DDD8(D_80058AD0);
            func_8001E054(D_80058AD0, D_800BCEE4);
            g->unk62 = D_800BCEDC;
            g->unk34 = base[g->unk62][0];
            InitialiseHUB();
            D_800BCED8 = 0;
            set_fade(p, -0x200, 0);
        }
        break;
    }
}

void HubSpawnEffects(void) {
    s32 *tbl;
    s32 n;
    s32 i;
    s32 *p;
    s32 *e;
    NAS *nas;
    HubSpawnExt *ext;

    tbl = NULL;
    if (*(s32 *)D_80058AD0 != 0) {
        tbl = *(s32 **)((u8 *)D_80058AD0 + 0x28);
    }
    i = 0;
    n = tbl[0];
    p = tbl + 1;
    while (1) {
        if (!(i < n)) {
            break;
        }
        e = (s32 *)((u8 *)p + *p);
        if ((u32)(e[3] - 0xC8) < 5) {
            nas = CreateNAS(0, 0, 0, 0x1626, 0x11);
            if (nas != NULL) {
                ext = FindExtension(nas, 0x10000000);
                if (ext != NULL) {
                    s32 *q = (s32 *)(*(u8 **)((u8 *)D_80058AD0 + 0x2C) + i * 4);
                    ext->unkC = (s32 *)((u8 *)q + *q);
                    if (e[3] == 0xC8) {
                        ext->unk34 = 0;
                    }
                    if (e[3] == 0xC9) {
                        ext->unk34 = 1;
                    }
                    if (e[3] == 0xCA) {
                        ext->unk34 = 2;
                    }
                    if (e[3] == 0xCB) {
                        ext->unk34 = 3;
                        nas->Engine->sub->flags &= 0xFFFF7FFF;
                    }
                    if (e[3] == 0xCC) {
                        ext->unk34 = 4;
                        nas->Engine->sub->flags &= 0xFFFF7FFF;
                    }
                }
            }
        }
        i++;
        p++;
    }
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char D_800B3618[] = "YOU ALREADY HAVE A TROPHY\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char D_800B3670[] = "YOU ALREADY HAVE A GEM\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char D_800B36C4[] = "YOU ALREADY HAVE A CRYSTAL\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char D_800B371C[] = "YOU ALREADY HAVE ARELIC\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char D_800B3770[] = "YOU ALREADY HAVE A PLATINUM RELIC\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char D_800B37D0[] = "";

static inline void PathFace(KeyEntry *k, s32 dir, PathRot *ang) {
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    KeyEntry *n;

    if (k->flags & 0x2000) {
        k--;
    }
    n = k + 1;
    dx = k->pos.x - n->pos.x;
    dz = k->pos.z - n->pos.z;
    dy = k->pos.y - n->pos.y;
    dist = csqrt(dx * dx + dz * dz) >> 6;
    ang->y = get_angle(dx, dz) + (dir << 11);
    if (dir != 0) {
        ang->x = get_angle(dy, dist);
    } else {
        ang->x = -get_angle(dy, dist);
    }
}

void PlotPathEffect(NAS *nas) {
    PathExt *ext;
    KeyEntry *k;
    KeyEntry *old;
    u32 flags;
    s32 type;
    s32 angle;
    PathRot rot;

    ext = FindExtension(nas, 0x10000000);
    if (ext == NULL) {
        return;
    }
    angle = ((-(get_angle(nas->Engine->unk10.x, nas->Engine->unk10.z) & 0xFFF) - D_80051696) << 20) >> 20;
    if (ext->kind < 4) {
        if (abs(angle) > 0x400) {
            nas->Engine->sub->flags &= 0xFFFF7FFF;
        } else {
            nas->Engine->sub->flags |= 0x8000;
        }
    }
    nas->Engine->unk5E = 1;
    switch (ext->state) {
    case 0:
        func_8001E21C(ext->path, ext->pos, &nas->Engine->unk10);
        old = func_8001E41C(ext->path, 0);
        ext->key = old;
        if (ext->kind < 3) {
            ext->keyFlags = old->flags;
            ext->dir = 0;
            PathFace(old, 0, &rot);
            nas->Engine->unk1E = rot.y;
            nas->Engine->unk1C = rot.x;
            ext->state = 2;
            ((void (*)(EXTanimator *, s32))GotoLinkAnim)(ext->anim, ext->kind * 2);
            func_800250B0(nas);
        }
        if ((u32)(ext->kind - 3) < 2) {
            ext->fx = CreateNAS(nas->Engine->unk10.x, nas->Engine->unk10.y, nas->Engine->unk10.z, 0x1627, 0x11);
            ext->state = 5;
            nas->Engine->flags &= 0xFFFF7FFF;
        }
        break;
    case 2:
        if (ext->pos >= ext->path->count) {
            ext->dir = 1;
        }
        if (ext->pos <= 0) {
            ext->dir = 0;
        }
        flags = ext->key->flags;
        if (ext->dir == 0) {
            ext->pos = ext->pos + ext->speed;
        } else {
            ext->pos = ext->pos - ext->speed;
        }
        func_8001E21C(ext->path, ext->pos, &nas->Engine->unk10);
        k = func_8001E41C(ext->path, ext->pos);
        if (ext->dir == 0) {
            type = ((u16 *)&k->flags)[1];
            ext->key = k;
        } else {
            type = ((u16 *)&ext->key->flags)[1];
            ext->key = k;
        }
        if (flags != k->flags) {
            switch (type) {
            case 1:
                ext->timer = 0x5A;
                ext->state = 3;
                ((void (*)(EXTanimator *, s32))GotoLinkAnim)(ext->anim, (ext->kind * 2) | 1);
                return;
            case 2:
                ext->timer = -1;
                ext->state = 4;
                return;
            }
        }
        PathFace(k, ext->dir, &rot);
        nas->Engine->unk1E = rot.y;
        nas->Engine->unk1C = rot.x;
        func_800250B0(nas);
        return;
    case 3:
        if (ext->anim->Aindex != 0) {
            func_800250B0(nas);
        }
        if (ext->timer-- < 0) {
            ((void (*)(EXTanimator *, s32))GotoLinkAnim)(ext->anim, ext->kind * 2);
            ext->state = 2;
        }
        break;
    case 4:
        func_800250B0(nas);
        if (ext->timer-- < 0) {
            if (Random(2) == 0) {
                ext->dir = 0;
            } else {
                ext->dir = 1;
            }
            ext->state = 2;
        }
        break;
    case 5:
        if (ext->fx != NULL) {
            if (ext->kind == 3) {
                if ((u32)(angle - 0x101) < 0xFF) {
                    if (ext->fx->UserInfo.user1 == 1) {
                        s32 s = 0x1000 - Random(0x800);
                        ext->fx->Engine->sub->unk20.x = s;
                        ext->fx->Engine->sub->unk20.y = s;
                        ext->fx->Engine->sub->unk20.z = s;
                        ext->fx->UserInfo.user1 = 0;
                        ext->fx->Engine->sub->unk52 = 1;
                    }
                    ext->fx->Engine->sub->unk76 = abs(0x180 - abs(angle)) << 5;
                    ext->fx->Engine->sub->flags |= 0x8000;
                } else {
                    ext->fx->Engine->sub->flags &= 0xFFFF7FFF;
                    ext->fx->UserInfo.user1 = 1;
                }
            }
            if (ext->kind == 4) {
                if (angle < -0x1FF) {
                    goto off;
                }
                if (angle < -0x100) {
                    if (ext->fx->UserInfo.user1 == 1) {
                        s32 s = 0x1000 - Random(0x800);
                        ext->fx->Engine->sub->unk20.x = s;
                        ext->fx->Engine->sub->unk20.y = s;
                        ext->fx->Engine->sub->unk20.z = s;
                        ext->fx->UserInfo.user1 = 0;
                        ext->fx->Engine->sub->unk52 = 1;
                    }
                    ext->fx->Engine->sub->unk76 = abs(0x180 - abs(angle)) << 5;
                    ext->fx->Engine->sub->flags |= 0x8000;
                } else {
                off:
                    ext->fx->Engine->sub->flags &= 0xFFFF7FFF;
                    ext->fx->UserInfo.user1 = 1;
                }
            }
        }
        ext->absAngle = abs(angle);
        break;
    }
}

void InitPathEffect(NAS *nas) {
    ScriptExt40 *ext = AddExt(nas, 0x40, 0x10000000);

    if (ext != NULL) {
        ext->cursor = func_80024E30(nas, &D_800BCCE8);
        nas->Engine->sub->unk6C = *(void **)D_80058AD0;
        nas->Engine->sub->unk74 = 0x2018;
        nas->Engine->sub->flags &= 0xFEFFFFFF;
        nas->Engine->unk5D = 2;
        ext->unk18 = 0;
        ext->unk24 = 0xC;
        ext->unk14 = 0;
        ext->unk38 = 0;
        ext->unk3C = 0;
    }
}

void PlotCrystalFlash(void) {
}

void InitCrystalFlash(FxObj *arg0) {
    arg0->mid->inner->unk6C = **(s32 **)&D_80058AD0;
    arg0->mid->inner->unk74 = 0x200E;
    arg0->mid->inner->unk51 = 0;
    arg0->mid->inner->unk50 = 1;
    arg0->mid->inner->unk20 = 0x1000;
    arg0->mid->inner->unk24 = 0x1000;
    arg0->mid->inner->unk28 = 0x1000;
    arg0->mid->inner->unk10 = 0;
    arg0->mid->inner->unk12 = 0;
    arg0->mid->inner->unk14 = 0;
    arg0->mid->inner->unk52 = 1;
    arg0->mid->inner->flags |= 0x60000000;
    arg0->unk60 = 0;
}

void HubPathAngles(s32 *arg0, s32 flag, s32 *out) {
    s32 *p = arg0;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 *q;

    if (p[5] & 0x2000) {
        p -= 7;
    }
    dx = p[0] - p[7];
    q = p + 7;
    dz = p[2] - q[2];
    dy = p[1] - q[1];
    dist = csqrt(dx * dx + dz * dz) >> 6;
    out[1] = get_angle(dx, dz) + (flag << 11);
    if (flag != 0) {
        out[0] = get_angle(dy, dist);
        return;
    }
    out[0] = -get_angle(dy, dist);
}

void HubInitLevelSelect(void) {
    D_800BCEE8 = func_8001DCD4(1, 0x14);
    D_800BCED8 = 0;
    D_800BCEE0 = -1;
}

s32 HubGatewayIsKind(s32 arg0) {
    s32 r = 0;
    s32 v;

    if (D_8005A677 >= 0) {
        v = D_800BCCF4[D_8005A677].unk14->unk2;
    } else {
        v = 0;
    }
    if (v == arg0 && D_800BCCF4[D_8005A677].unk20 != 0) {
        r = 1;
    }
    return r;
}

u16 HubGatewayKind(void) {
    if (D_8005A677 >= 0) {
        return D_800BCCF4[D_8005A677].unk14->unk2;
    }
    return 0;
}

void HubDoorMenu2Quit(void) {
    func_80095A04(1);
    func_80095A04(2);
}

void HubDoorMenu2Render(void) {
    EXTengine *e;

    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = D_8009F644.amount;
    if (D_800BCED4 >= 0) {
        if (!(D_8005A624 & 1)) {
            e = GetEngineSlot(1);
            e->flags |= 0x8000;
            e->unk10.y = 0x1D6 - (D_800BF260 >> 5);
            func_8001C2C0((u8 *)GetEngineSlot(1));
        }
    }
    if (D_800BF260 != 0 && !(D_8005A624 & 1)) {
        func_8001C2C0((u8 *)GetEngineSlot(2));
    }
}

void HubDoorMenu2Update(void) {
    HubDoorMenuUpdate();
}

void HubDoorMenu2Init(void) {
    D_800BCED4 = -1;
}

void HubDoorMenuRender(void) {
    EXTengine *e;

    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = D_8009F644.amount;
    if (D_800BCED4 >= 0) {
        if (!(D_8005A624 & 1)) {
            e = GetEngineSlot(1);
            e->flags |= 0x8000;
            e->unk10.y = 0x1D6 - (D_800BF260 >> 5);
            func_8001C2C0((u8 *)GetEngineSlot(1));
        }
    }
    if (D_800BF260 != 0 && !(D_8005A624 & 1)) {
        func_8001C2C0((u8 *)GetEngineSlot(2));
    }
}

void HubDoorMenuInit(void) {
    D_800BCED4 = -1;
}

void HubDoorMenuQuit(void) {
    func_80095A04(1);
    func_80095A04(2);
}

void HubShowSub(u8 *arg0, s32 arg1) {
    u8 *temp_a2;

    temp_a2 = (*(s32*)((u8*)(arg0) + 84));
    if (temp_a2 != NULL) {
        if (arg1 != 0) {
            (*(s32*)((u8*)(temp_a2) + 12)) = (s32) ((*(s32*)((u8*)(temp_a2) + 12)) | 0x8000);
            (*(s16*)((u8*)(arg0) + 92)) = 1;
            return;
        }
        (*(s32*)((u8*)(temp_a2) + 12)) = (s32) ((*(s32*)((u8*)(temp_a2) + 12)) & 0xFFFF7FFF);
        (*(s16*)((u8*)(arg0) + 92)) = 0;
    }
}

void PlotGatewayTrophy(void) {
}

void InitGatewayTrophy(void) {
}

void HubSpawnFlash(s32 *arg0, s32 arg1) {
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    u8 *p;

    p = D_800558EC;
    nas = CreateNAS(arg0[0], arg0[1], arg0[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = arg1;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    if (r != NULL) {
        eng = r->Engine;
        if (eng != NULL) {
            eng->flags |= 0x20000000;
        }
    }
    PlaySfx(0x398, NULL, NULL, 0x1000, 0x1E00, 0);
}

s32 HubDist(DistPos *a, DistPos *b) {
    s32 dx = b->x - a->x;
    s32 dz = b->z - a->z;
    return csqrt(dx * dx + dz * dz) >> 6;
}

s32 HubDistXZ(u8 *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = (*(s32*)((u8*)(arg1) + 16)) - (*(s32*)((u8*)(arg0) + 16));
    temp_v0_2 = (*(s32*)((u8*)(arg1) + 24)) - (*(s32*)((u8*)(arg0) + 24));
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
}

void HubGoToMode(s32 arg0) {
    u8 *p;
    D_8009E5DC = arg0;
    p = D_8005A614;
    *(u16 *)(p + 0x30) = 0x8008;
    *(u16 *)(p + 0x24) = 0xA;
}

void HubCamFromPath(u8 *cam, s32 arg1) {
    u8 *o = (u8 *)path_get(arg1);
    s32 *p;
    s32 t;

    if (o != NULL) {
        *(s16 *)(cam + 0x54) = (get_angle(*(s32 *)(o + 0x58) - *(s32 *)(o + 0x3C),
                                          *(s32 *)(o + 0x50) - *(s32 *)(o + 0x34)) - 0x400) & 0xFFF;
        *(s16 *)(cam + 0x56) = 0;
        *(s16 *)(cam + 0x58) = 0;
        *(s32 *)(cam + 0x18) = (((s32)(*(u16 *)(o + 0x40) << 16)) >> 18) + 0x200;
        *(s32 *)(cam + 0xC) = *(s32 *)(o + 0x30);
        p = (s32 *)(o + 0x30);
        t = p[1];
        *(s32 *)(cam + 0x10) = t;
        t = p[2];
        *(s32 *)(cam + 0x14) = t;
        *(s32 *)(cam + 4) = -p[1];
        *(s32 *)cam = 0;
        *(s32 *)(cam + 8) = *(s32 *)(o + 0x58);
    }
}

void HubCameraFollow(u8 *arg0, s32 arg1) {
    CamView *cam = (CamView *)arg0;
    struct {
        CamView v;
        u8 pad[0xB8 - sizeof(CamView)];
    } buf;
    s32 n;
#define v buf.v

    n = 1;
    cam->dist = 0x200;
    if (D_800BCEF4 != 0) {
        n = 3;
    }
    HubCamFromPath((u8 *)&v, n);
    v.at = *(CamXYZ *)&D_8009D530->unk10;
    v.rot.vy = -D_8009EEA0;
    if (arg1 != 0) {
        func_80013F44(cam, &v, NULL);
    } else {
        func_80013F44(cam, &v, &D_800BCBAC);
    }
}
#undef v

void HubHeroSetIdle(s32 arg0) {
    Heroes[arg0].hero_idle_count = 0xF;
}

s32 HubHeroTestButton(s32 arg0) {
    if (D_8005A688 != 0) {
        return 1;
    }
    if (((PadButtons *)D_80051338)[arg0].unk54 & 0x8000) {
        if ((u32)(Heroes[arg0].hero_name - 1) >= 2U) {
            if (Heroes[arg0].hero_frozen == 0) {
                Heroes[arg0].hero_frozen = 0xF;
            }
        } else {
            D_800BCEF0 = 1;
        }
    }
    return 0;
}

s32 HubHeroTestA(void) {
    return 0;
}

s32 HubHeroTestB(void) {
    return 0;
}

void HubHeroTouched(s32 arg0) {
    if (Heroes[arg0].hero->UserInfo.user1 != 0) {
        D_800BCEF4 += 1;
        return;
    }
    D_800BCEF4 = 0;
}

void HubCamTarget(void) {
    volatile s32 v[2];

    s32 a, b;

    a = D_80068BD4[(D_8009EE9C.unk4 - 0x400) & 0xFFF].unk2 * D_8009EE9C.unk8;
    v[0] = D_80068BD4[(D_8009EE9C.unk4 - 0x400) & 0xFFF].unk2;
    b = D_80068BD4[D_8009EE9C.unk4 & 0xFFF].unk2 * D_8009EE9C.unk8;
    v[1] = D_80068BD4[D_8009EE9C.unk4 & 0xFFF].unk2;
    v[0] = a;
    a >>= 12;
    v[0] = a;
    v[1] = b;
    b >>= 12;
    v[1] = b;
    Heroes->hero_eng->unk10.x = a;
    Heroes->hero_eng->unk10.z = v[1];
}

void HubShowShadow(s32 arg0) {
    HeroInfo *hero;
    EXTengine *shadow;

    HeroInfo *base = Heroes;

    hero = &base[arg0];
    shadow = hero->hero_shadow;

    if (shadow != NULL) {
        EXTengine *eng;

        shadow->flags |= 0x8000;
        eng = hero->hero_eng;
        shadow->unk10 = eng->unk10;
    }
}

void HubHideShadow(s32 arg0) {
    EXTengine *s = Heroes[arg0].hero_shadow;

    if (s != NULL) {
        s->flags &= 0xFFFF7FFF;
    }
}

void HubCreateShadow(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    if (s != NULL) {
        s->sub->unk6C = D_800558EC;
        s->sub->unk74 = 0x2020;
        s->unk10 = h->hero_eng->unk10;
        s->flags |= 0x8041;
        s->sub->unk76 = 0x800;
        s->sub->flags |= 0x40000000;
    }
}

void HubInitHero(s32 arg0) {
    u8 *o;

    HubCreateShadow(arg0);
    o = (u8 *)path_get(2);
    if (o != NULL) {
        D_8009EE9C.unkC = *(s32 *)(o + 0x58);
        D_8009EE9C.unk10 = *(s32 *)(o + 0x74);
    }
    HubPreMoveHero(arg0);
}

s32 HubNearestGateway(void) {
    EngRec90248 *p;
    s32 i;
    s32 best;
    s32 bestDist;
    s32 d;

    best = -1;
    bestDist = 0x1000;
    for (i = 0; i < 12; i++) {
        p = &D_800BCCF4[i];
        if (p->unk14 != NULL && (p->unk14->unk6 & 2)) {
            d = func_80015480(p->unk10, D_8009EE9C.unk4);
            if (d < bestDist) {
                bestDist = d;
                if ((u32)d < 0x81U) {
                    best = i;
                }
            }
        }
    }
    return best;
}

void HubRenderScene(void) {
    func_8009440C();
}

void HubUpdateRoom(void) {
    if (D_8005A688 == 0) {
        HubUpdateGateways();
        HubLevelSelect();
        func_800139A8();
    }
}

void HubCullObjects(void) {
    u8 *o;
    u8 *cam;
    s32 a;

    o = *(u8 **)((u8 *)D_80058AD0 + 0x1C);
    cam = D_80051640;
    for (;;) {
        if (o == 0) {
            return;
        }
        if (o[0x64] == 0) {
            a = (-(get_angle(*(s32 *)(o + 4), *(s32 *)(o + 0xC)) & 0xFFF) - *(s16 *)(cam + 0x56)) << 20 >> 20;
            *(s32 *)o &= 0xFFFF7FFF;
            if (a >= 0x301) {
                *(s32 *)o |= 0x8000;
            }
            if (a < -0x300) {
                *(s32 *)o |= 0x8000;
            }
        }
        o = *(u8 **)(o + 0x5C);
    }
}

void InitialiseHUBFirst(void) {
    ((HubGameView *)D_8005A614)->unk51 = 9;
    ((HubGameView *)D_8005A614)->unk52 = 9;
    ((HubGameView *)D_8005A614)->unk53 = 9;
    D_8009D6E8.unk0 = (s32)HubInitHero;
    D_8009D6E8.unk20 = (void *)HubPreMoveHero;
    D_8009D6E8.unk34 = (void *)HubHeroTouched;
    ((HubGameView *)D_8005A614)->unk50 = ((HubGameView *)D_8005A614)->unk4C;
    ((HubGameView *)D_8005A614)->unk32 |= 0x200;
}

s32 HubLookup(s32 arg0) {
    KeyedPair *p = D_800BCB54;
    s32 i = 10;
    u8 *g = D_8005A614;

    for (; i != -1; i--) {
        if (p->key == arg0) {
            return p->val[*(s32 *)(g + 0x2C)];
        }
        p++;
    }
    return 0;
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char D_800B37EC[] = "WIN_BREATHE";
const char D_800B37F8[] = "WIN";
const char D_800B37FC[] = "LOSE_BREATHE";
const char D_800B380C[] = "LOSE";

void InitialisePodiumFirst(void) {
    s32 i;

    D_800BD594 = NULL;
    for (i = 0; i < 4; i++) {
        D_800BD584[i] = 0;
        switch (D_8005A614[i + 0x4C]) {
        case 1: func_80016D44(D_80055924); break;
        case 2: func_80016D44(D_8005595C); break;
        case 3: func_80016D44(D_80055994); break;
        case 4: func_80016D44(D_800559CC); break;
        case 5: func_80016D44(D_80055A04); break;
        case 6: func_80016D44(D_80055A3C); break;
        case 7: func_80016D44(D_80055A74); break;
        case 8: func_80016D44(D_80055AAC); break;
        }
    }
    ((HubGameView *)D_8005A614)->unk50 = 9;
    ((HubGameView *)D_8005A614)->unk51 = 9;
    ((HubGameView *)D_8005A614)->unk52 = 9;
    ((HubGameView *)D_8005A614)->unk53 = 9;
    D_800BD5B0 = 0;
    ((HubGameView *)D_8005A614)->unk32 |= 0x200;
    PodiumRankPlayers();
    PodiumFindTies();
    *(s32 *)&D_8005A5E8[((HubGameView *)D_8005A614)->unk8] = 0xCD;
    ((HubGameView *)D_8005A614)->unk3C = 0xCD;
}

void InitialisePodium(void) {
    s32 i;
    s32 k;
    s32 j;
    s32 name;
    s32 h;

    World_floor_level = 0;
    D_800AD12C = 4;
    D_800BD5B4 = 0;
    D_800BD390 = 0x5A;
    for (i = 0; i < 4; i++) {
        h = D_8005A614[i + 0xA7];
        name = D_8005A614[h + 0x4C];
        Heroes[h].hero_name = name;
        switch (name) {
        case 2: k = 0xD7; break;
        case 3: k = 0xD8; break;
        case 4: k = 0xD9; break;
        case 5: k = 0xDA; break;
        case 6: k = 0xDB; break;
        case 7: k = 0xDC; break;
        case 8: k = 0xDD; break;
        case 1:
        default: k = 0xD6; break;
        }
        for (j = 0; (&D_800BD398)[j] != NULL;) {
            func_800165F8(D_80052A54 + k * 0x38, (&D_800BD398)[j++]);
        }
        PodiumSpawnHero(i, &((HubSpawnEnt (*)[4])D_800BD210)[D_800BD5A8][i]);
        PodiumInitScript(i, name);
    }
    PodiumShowScores();
    func_80095258(*(u8 **)D_80058AD0);
}

void PodiumRankPlayers(void) {
    s32 mask = 0;
    s32 i;
    s32 j;
    s32 best;
    u8 *p;
    u8 *q;

    for (i = 0; i < 4; i++) {
        best = -1;
        for (j = 0; j < 4; j++) {
            if (!((mask >> j) & 1)) {
                if (D_8005A614[0x9F + j] > D_8005A614[0x9F + best] || best == -1) {
                    best = j;
                }
            }
        }
        D_8005A614[0xA7 + i] = best;
        mask |= 1 << best;
    }
    for (i = 0; i < 4; i++) {
        s32 k;
        k = 4;
        q = D_8005A614 + i;
        if (i == 0) {
            D_800BD598[0] = k;
        } else {
            if (D_8005A614[q[0xA7] + 0x9F] == D_8005A614[(p = D_8005A614 + (j = i - 1))[0xA7] + 0x9F]) {
                D_800BD598[i] = D_800BD598[i - 1];
            } else {
                D_800BD598[i] = D_800BD598[j] - 1;
            }
        }
    }
}

void PodiumFindTies(void) {
    u8 *g = D_8005A614;
    s32 a = g[g[0xA7] + 0x9F];
    s32 b = g[g[0xA8] + 0x9F];
    s32 c = g[g[0xA9] + 0x9F];
    s32 d = g[g[0xAA] + 0x9F];

    D_800BD5A8 = 0;
    if (a == d) {
        D_800BD5A8 = 7;
        return;
    }
    if (b == d) {
        D_800BD5A8 = 5;
        return;
    }
    if (a == c) {
        D_800BD5A8 = 6;
        return;
    }
    if (c == d && a != b) {
        D_800BD5A8 = 1;
        return;
    }
    if (b == c) {
        D_800BD5A8 = 2;
        return;
    }
    if (a == b) {
        if (c != d) {
            D_800BD5A8 = 3;
            return;
        }
        D_800BD5A8 = 4;
    }
}

void HubCreditsEffects(void) {
    s32 r;
    s32 x;

    if (D_8005A624 == 0 && D_800BD390-- < 0) {
        D_800BD394 = Random(4);
        r = Random(6);
        if (r == 0) {
            PlaySfx(0x355, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x356, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x357, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 3) {
            PlaySfx(0x358, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 4) {
            PlaySfx(0x359, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 5) {
            PlaySfx(0x35A, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        r = Random(6);
        if (r == 0) {
            PlaySfx(0x35B, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x35C, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x35D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 3) {
            PlaySfx(0x35E, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 4) {
            PlaySfx(0x35F, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 5) {
            PlaySfx(0x360, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        x = Random(10000) - 5000;
        CreateNAS(x, Random(10000) - 5000, 0xE10, 0x1628, 0x11);
        D_800BD390 = Random(10) + 5;
    }
}

void PodiumCheer(void) {
    s32 r;
    s32 x;

    switch (D_8005A614[D_8005A614[0xA7] + 0x4C]) {
    case 3:
    case 4:
    case 6:
    case 8:
        if (D_8005A624 == 0 && D_800BD390-- < 0) {
            D_800BD394 = Random(2) + 2;
            goto spawn;
        }
        break;
    default:
        if (D_8005A624 == 0 && D_800BD390-- < 0) {
            D_800BD394 = Random(2);
        spawn:
            r = Random(6);
            if (r == 0) {
                PlaySfx(0x355, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x356, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x357, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 3) {
                PlaySfx(0x358, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 4) {
                PlaySfx(0x359, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 5) {
                PlaySfx(0x35A, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            r = Random(6);
            if (r == 0) {
                PlaySfx(0x35B, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x35C, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x35D, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 3) {
                PlaySfx(0x35E, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 4) {
                PlaySfx(0x35F, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 5) {
                PlaySfx(0x360, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            x = -Random(0x2000) + Random(0x2000);
            CreateNAS(x, Random(0x400) - 0xC00, 0x1000, 0x1628, 0x11);
            D_800BD390 = Random(0x14) + 0xA;
        }
        break;
    }
}

void PlotPodiumOdo(NAS *arg0) {
    EXTengine *eng;
    EXTanimator *anim;
    s16 idx;

    eng = arg0->Engine;
    if (eng != NULL) {
        anim = FindExtension(arg0, 0x400);
        if (anim != NULL && ((idx = anim->ALindex) == 0 || idx == 4)) {
            if (arg0->UserInfo.status == 0) {
                if (arg0->UserInfo.flags != 0) {
                    GotoLinkAnim(anim, 5);
                    arg0->UserInfo.status = Random(0x28);
                } else {
                    GotoLinkAnim(anim, 1);
                    switch (arg0->UserInfo.user2) {
                    case 3:
                        arg0->UserInfo.status = Random(0x1E) + 0x14;
                        break;
                    case 4:
                        arg0->UserInfo.status = Random(0x3C) + 0x3C;
                        break;
                    default:
                        arg0->UserInfo.status = Random(0x28) + 0xA;
                        break;
                    }
                }
            } else {
                arg0->UserInfo.status = arg0->UserInfo.status - 1;
            }
        }
        if (eng->flags & 0x8000) {
            func_800250B0(arg0);
        }
    }
}

void PodiumInitScript(s32 arg0, s32 arg1) {
    PodiumScriptObj *obj;
    PodiumMid *mid;

    obj = (PodiumScriptObj *)D_800BD584[arg0];
    if (obj != NULL) {
        mid = obj->mid;
        if (mid != NULL) {
            switch (arg1) {
            case 1:
                mid->inner->script = D_80055924;
                func_80024E30((NAS *)obj, &D_800BD008);
                break;
            case 2:
                mid->inner->script = D_8005595C;
                func_80024E30((NAS *)obj, &D_800BCFB8);
                break;
            case 3:
                mid->inner->script = D_80055994;
                func_80024E30((NAS *)obj, &D_800BD070);
                break;
            case 4:
                mid->inner->script = D_800559CC;
                func_80024E30((NAS *)obj, &D_800BCF68);
                break;
            case 5:
                mid->inner->script = D_80055A04;
                func_80024E30((NAS *)obj, &D_800BD18C);
                break;
            case 6:
                mid->inner->script = D_80055A3C;
                func_80024E30((NAS *)obj, &D_800BD1E4);
                break;
            case 7:
                mid->inner->script = D_80055A74;
                func_80024E30((NAS *)obj, &D_800BD0D8);
                break;
            case 8:
                mid->inner->script = D_80055AAC;
                func_80024E30((NAS *)obj, &D_800BD134);
                break;
            }
            obj->unk64 = arg1;
            func_800250B0((NAS *)obj);
            mid->flags |= 0x8000;
        }
    }
}

void PlotPodiumTrophy(PodiumTrophy *arg0) {
    TrophyInner *in;
    PodiumFx *fx;
    TrigEntry *tb;
    s32 t;
    u16 st;
    s32 v;

    in = arg0->inner;
    if (in != NULL) {
        st = arg0->state;
        if (st == 1) {
            in->unk2C += (0x1000 - in->unk2C) >> 5;
            if (in->unk2C >= 0xF00) {
                in->unk2C = 0x1000;
                arg0->state = 2;
            }
            v = in->unk2C;
            in->unk5E = st;
            in->unk34 = v;
            in->unk30 = v;
        }
        fx = in->fx;
        if (fx != NULL) {
            tb = D_80068BD4;
            t = D_8005B690;
#define TSIN(k) tb[((((t * (k)) & 0xFFF) - 0x400) & 0xFFF)].unk2
            fx->unk10 = *(s32 *)&tb[(((t * 0x46) & 0xFFF) - 0x400) & 0xFFF] >> 20;
            fx->unk12 = (TSIN(0x8C) + 0x800) >> 5;
            fx->unk14 = (TSIN(0x46) + TSIN(0x64)) >> 5;
            fx->unk4 = tb[((t * 0x28) - 0x400) & 0xFFF].unk2 * 0x118 / 4096;
            fx->unk8 = (TSIN(0x2D) + TSIN(0x50)) >> 6;
#undef TSIN
            fx->unk52 = 1;
        }
    }
}

void PodiumCamera(CamView *out, s32 arg1) {
    SLONG_XYZ key;
    CamXYZ d;
    u8 *o;
    PodiumChild *c;
    s32 id;
    s32 end;
    s32 t;

    if (arg1 != 0) {
        D_800BD5B0 = 0;
        switch (D_8005A614[D_8005A614[0xA7] + 0x4C]) {
        case 1:
        case 2:
            PlaySfx(0x352, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 5:
        case 7:
            PlaySfx(0x353, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 3:
        case 4:
        case 6:
        case 8:
            PlaySfx(0x354, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        }
    }
    id = 2;
    if (D_800BD5B0 < 2) {
        id = 1;
    }
    o = (u8 *)path_get(id);
    if (arg1 != 0) {
        t = *(s32 *)(o + 0x58);
        D_800C7F04 = *(CamXYZ *)(o + 0x30);
        D_800C7EF4 = t;
    }
    func_8001E21C((KeyBlock *)o, D_800C7EF4, &key);
    D_800C7F04.x += (*(s32 *)(o + 0x30) - D_800C7F04.x) >> 4;
    D_800C7F04.y += (*(s32 *)(o + 0x34) - D_800C7F04.y) >> 4;
    D_800C7F04.z += (*(s32 *)(o + 0x38) - D_800C7F04.z) >> 4;
    out->at = D_800C7F04;
    d.x = D_800C7F04.x - key.x;
    d.y = D_800C7F04.y - key.y;
    d.z = D_800C7F04.z - key.z;
    out->eye.z = csqrt(d.x * d.x + d.y * d.y + d.z * d.z) >> 6;
    out->eye.x = 0;
    out->eye.y = 0;
    out->rot.vy = get_angle(-d.x, d.z);
    out->rot.vx = get_angle(d.y, csqrt(d.x * d.x + d.z * d.z) >> 6);
    out->rot.vz = 0;
    switch (D_800BD5B0) {
    case 0:
        end = *(s32 *)(o + 4);
        D_800C7EF4 += 0x20;
        if (D_800C7EF4 >= end) {
            D_800C7EF4 = end;
            D_800BD5AC = 0x3C;
            D_800BD5B0 = 1;
        }
        break;
    case 1:
        if (--D_800BD5AC <= 0) {
            D_800BD5B0 = 2;
            D_800C7EF4 = *(s32 *)((u8 *)path_get(2) + 0x58);
            if (D_800BD594 != NULL) {
                c = D_800BD594->child;
                if (c != NULL && D_800BD594->f5C == 0) {
                    D_800BD594->f5C = 1;
                    c->f5C = 1;
                    c->f34 = 0;
                    c->f30 = 0;
                    c->f2C = 0;
                    c->f5E = 1;
                    c->flags |= 0x8000;
                }
            }
        }
        break;
    case 2:
        end = *(s32 *)(o + 4);
        D_800C7EF4 += 0x20;
        if (D_800C7EF4 >= end) {
            D_800BD5AC = 0xF0;
            D_800C7EF4 = end;
            D_800BD5B0 = 3;
            D_800BD5B4 = 1;
        }
        break;
    case 3:
        PodiumScoresDown();
        if (--D_800BD5AC <= 0) {
            D_800BD5B0 = 4;
        }
        break;
    case 4:
        D_800BD5B0 = 5;
        D_800BD5AC = 0x3C;
        break;
    case 5:
        PodiumScoresUp();
        if (--D_800BD5AC <= 0) {
            D_800BD5B0 = 0x64;
        }
        break;
    case 0x64:
        break;
    }
    out->dist = 0x200;
    D_8009F708 = 0x15E;
}

void PodiumShowScores(void) {
    EXTengine *e;
    s32 n;

    e = GetEngineSlot(6);
    if (e != NULL) {
        n = D_8005A614[0x9F];
        D_800BD3AC[1] = n % 10 + 0x30;
        D_800BD3AC[0] = n / 10 % 10 + 0x30;
        n = D_8005A614[0xA0];
        D_800BD3B0[1] = n % 10 + 0x30;
        D_800BD3B0[0] = n / 10 % 10 + 0x30;
        n = D_8005A614[0xA1];
        D_800BD3B4[1] = n % 10 + 0x30;
        D_800BD3B4[0] = n / 10 % 10 + 0x30;
        n = D_8005A614[0xA2];
        D_800BD3B8[1] = n % 10 + 0x30;
        D_800BD3B8[0] = n / 10 % 10 + 0x30;
        func_80095BEC(6, &D_800BD3C4);
        e->unk10.x = 0x28;
        e->unk10.y = 0x21C;
        e->flags |= 0x10008000;
    }
}

void InitSparkGFX(NAS *arg0) {
    u8 *nv;
    subengine *s;

    arg0->Engine->sub->flags = 0x8000;
    arg0->UserInfo.user1 = 0;
    arg0->Engine->unk5D = 0;
    arg0->Engine->unk5C = 1;
    if (func_80018F3C(arg0->Engine, 12) != 12 || func_80015D70((u8 *)&D_800BD570, (u8 *)arg0->Engine->sub) == 0) {
        KillNAS(arg0);
        return;
    }
    s = arg0->Engine->sub;
    s = s->next;
    if (s != NULL) {
        u32 fl;
        do {
            s->unk51 = 0;
            nv = D_800558EC;
            s->unk50 = 1;
            s->unk6C = nv;
            if (D_800BD394 == 0) {
                s->unk74 = 0x2021;
            }
            if (D_800BD394 == 1) {
                s->unk74 = 0x2025;
            }
            if (D_800BD394 == 2) {
                s->unk74 = 0x2022;
            }
            if (D_800BD394 == 3) {
                s->unk74 = 0x2024;
            }
            if (D_800BD394 == 4) {
                s->unk74 = 0x2027;
            }
            if (D_800BD394 == 5) {
                s->unk74 = 0x2026;
            }
            s->unk20.x = 0x1000;
            fl = 0x20000000;
            s->unk20.y = 0x1000;
            s->unk20.z = 0x1000;
            s->flags = fl;
            s->unk76 = 0;
            s = s->next;
        } while (s != NULL);
    }
}

void HubInitSparks(HubSparks *arg0) {
    s32 n;
    HubFt4 *src;
    HubFt4 *dst;
    HubSparkRow *row;
    HubTex *tex;
    TrigEntry *a;
    TrigEntry *b;
    s32 ang;
    s32 k;
    HubSparkSet *set;
    u8 *g;
    s32 sel;

    set = &arg0->set;
    tex = NULL;
    n = set->count;
    src = set->src;
    dst = set->dst;
    row = arg0->set.rows;
    g = D_800558EC;
    sel = D_800BD394;
    if (sel == 0) {
        tex = (HubTex *)(*(u8 **)(g + 0x18) + 0x188);
    }
    if (sel == 1) {
        tex = (HubTex *)(*(u8 **)(g + 0x18) + 0x268);
    }
    if (sel == 2) {
        tex = (HubTex *)(*(u8 **)(g + 0x18) + 0x1C0);
    }
    if (sel == 3) {
        tex = (HubTex *)(*(u8 **)(g + 0x18) + 0x230);
    }
    if (sel == 4) {
        tex = (HubTex *)(*(u8 **)(g + 0x18) + 0x1F8);
    }
    if (sel == 5) {
        tex = (HubTex *)(*(u8 **)(g + 0x18) + 0x2A0);
    }
    k = 4;
    for (;;) {
        s32 i;

        i = 0;
        if (n == 0) {
            break;
        }
        row->unk134 = 0;
        ang = n << 9;
        a = &D_80068BD4[ang & 0xFFF];
        for (;;) {
            s32 r;

            if (i >= 8) {
                break;
            }
            row->rot[i].vx = D_80068BD4[(ang - 0x400) & 0xFFF].unk2 * 64 >> 12;
            row->rot[i].vy = (a->unk2 * 64 >> 12) - 0x400;
            row->rot[i].vz = 0x2000;
            row->unk120 = (a->unk2 * 256 >> 12) + Random(0x40) - Random(0x40);
            row->unk12C = (D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> k) + Random(0x40) - Random(0x40);
            row->unkE0[i].vz = 0;
            row->unkA0[i].vz = 0;
            row->unk40[i] = 0;
            row->unk128 = 6;
            row->unk130 = Random(k) - Random(k);
            row->unk124 = 0;
            row->unk138 = 0;
            row->unk13C = 0;
            *(u32 *)&src->r0 = 0xA0A0A0;
            src->tpage = (tex->tpage & ~0x60) | 0x20;
            src->clut = tex->clut;
            src->uv0 = tex->uv0;
            src->uv1 = tex->uv1;
            src->uv2 = tex->uv2;
            src->uv3 = tex->uv3;
            ((u8 *)src)[3] = 9;
            src->code = 0x2E;
            *dst = *src;
            src++;
            dst++;
            i++;
        }
        row++;
        n--;
    }
}

void HubTrailPoint(HubTrailDst *d, HubTrail *t, s32 idx) {
    HubPt *p;
    s32 px, py;

    if (idx == 0) {
        if (t->unk138 != 0) {
            d->flags |= 0x40000000;
            d->unk76 += 0x200;
        }
        t->prev[0].x = t->pos[0].x;
        t->prev[0].y = t->pos[0].y;
        d->flags |= 0x8000;
        t->unk120 = ((t->unk120 + t->unk128) * 0xE6) >> 8;
        t->unk12C = (t->unk12C * 0xE6) >> 8;
        t->pos[0].y += *(u16 *)&t->unk120;
        t->pos[0].x += *(u16 *)&t->unk12C;
        d->x = (s16)t->pos[0].x;
        d->y = (s16)t->pos[0].y;
        d->z = t->pos[0].z;
        px = (s16)t->prev[0].x;
        py = (s16)t->prev[0].y;
        t->ang[0].z = get_angle(py - (s16)t->pos[0].y,
                                px - (s16)t->pos[0].x) & 0xFFF;
        return;
    }
    p = &t->pos[idx];
    t->prev[idx].x = p->x;
    t->prev[idx].y = p->y;
    p->x = t->prev[idx - 1].x;
    p->y = t->prev[idx - 1].y;
    t->ang[idx].z = get_angle((s16)t->prev[idx].x - (s16)p->x,
                              (s16)t->prev[idx].y - (s16)p->y) & 0xFFF;
    if (idx != 1) {
        t->flag[idx] = 1;
    }
}

void HubDrawSparks(HubSparks *arg0, MATRIX *m) {
    HubSparkRow *row;
    HubFt4 *prim;
    HubSparkSet *set;
    SVECTOR *p;
    s32 i;
    s32 c;
    s32 r;

    set = &arg0->set;
    row = set->rows;
    set = (HubSparkSet *)((u8 *)set + (D_8005B690 & 1) * 4);
    prim = set->src;
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
    arg0 = arg0->next;
    while (1) {
        if (arg0 == NULL) {
            break;
        }
        if (D_8005A624 == 0) {
            row->unk134++;
        }
        i = 0;
        if (row->unk134 == 0x14) {
            row->unk138 = 1;
        }
        while (1) {
            SVECTOR v;

            if (i >= 8) {
                break;
            }
            HubTrailPoint((HubTrailDst *)arg0, (HubTrail *)row, i);
            if (row->unk138 != 0) {
                row->unk13C = row->unk13C * 220 >> 8;
            } else {
                row->unk13C = 255 - i * 255 / 8;
            }
            c = calc_rgbcode(row->unk13C, row->unk13C, row->unk13C) | 0x2E000000;
            if (i == 0) {
                v.vx = row->rot[0].vx - 0x80;
                v.vy = row->rot[0].vy - 0x80;
                v.vz = row->rot[0].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                *(s32 *)&prim->r0 = c;
                gte_stsxy(&prim->x0);
                prim->y1 = prim->y0;
                prim->x2 = prim->x0;
            } else {
                *(s32 *)&prim->r0 = c;
                prim->x0 = prim[-1].x1;
                prim->y0 = prim[-1].y1;
                prim->x2 = prim[-1].x3;
                prim->y2 = prim[-1].y3;
            }
            if (i == 0) {
                v.vx = row->rot[0].vx + 0x80;
                v.vy = row->rot[0].vy + 0x80;
                v.vz = row->rot[0].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                gte_stsxy(&prim->x3);
                prim->x1 = prim->x3;
                prim->y2 = prim->y3;
            } else {
                r = 0x80 - i * 16;
                v.vx = row->rot[i].vx + (r * D_80068BD4[row->unkE0[i].vz & 0xFFF].unk2 >> 12);
                v.vy = row->rot[i].vy + (r * D_80068BD4[(row->unkE0[i].vz + 0x400) & 0xFFF].unk2 >> 12);
                v.vz = row->rot[i].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                gte_stsxy(&prim->x3);
                v.vx = row->rot[i].vx + (r * D_80068BD4[(((row->unkE0[i].vz - 0x400) & 0xFFF) - 0x400) & 0xFFF].unk2 >> 12);
                v.vy = row->rot[i].vy + (r * D_80068BD4[(row->unkE0[i].vz - 0x400) & 0xFFF].unk2 >> 12);
                v.vz = row->rot[i].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                gte_stsxy(&prim->x1);
            }
            if (row->unk40[i] != 0) {
                prim->tag = (prim->tag & 0xFF000000) | (D_80056998.ot[700] & 0xFFFFFF);
                p = (SVECTOR *)0xFF000000;
                D_80056998.ot[700] = (D_80056998.ot[700] & (u32)p) | ((u32)prim & 0xFFFFFF);
            }
            prim++;
            i++;
        }
        arg0 = arg0->next;
        row++;
    }
}

void PlotSparkGFX(NAS *arg0) {
    if (D_8005A624 == 0) {
        if (arg0->UserInfo.user1++ >= 0x1D) {
            KillNAS(arg0);
        }
    }
}

void PodiumScoresUp(void) {
    s32 temp_a0;
    EXTengine *eng;

    eng = GetEngineSlot(6);
    if (eng != NULL) {
        temp_a0 = eng->unk10.y;
        if (temp_a0 < 0x21C) {
            eng->unk10.y = temp_a0 + 5;
        }
    }
}

void PodiumScoresDown(void) {
    s32 temp_a0;
    EXTengine *eng;

    eng = GetEngineSlot(6);
    if (eng != NULL) {
        temp_a0 = eng->unk10.y;
        if (temp_a0 >= 0x155) {
            eng->unk10.y = temp_a0 - 5;
        }
    }
}

void PodiumReleaseTrophy(void) {
    PodiumChild *c;

    if (D_800BD594 != NULL) {
        c = D_800BD594->child;
        if (c != NULL && D_800BD594->f5C == 0) {
            D_800BD594->f5C = 1;
            c->f5C = 1;
            c->f34 = 0;
            c->f30 = 0;
            c->f2C = 0;
            c->f5E = 1;
            c->flags |= 0x8000;
        }
    }
}

void InitPodiumTrophy(NAS *arg0) {
    EXTengine *e;
    subengine *s;

    e = arg0->Engine;
    arg0->UserInfo.status = 0;
    if (e != NULL && (e->flags &= ~0x8000, s = e->sub, s != NULL)) {
        s->unk6C = &D_8005572C;
        s->unk74 = 0x200A;
        s->unk51 = 0;
    } else {
        arg0->Failure = 1;
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
        return;
    }
    D_800BD594 = (PodiumObj *)arg0;
}

void InitPodiumOdo(u8 *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = 0;
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s16*)((u8*)(arg0) + 94)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS((NAS *)arg0);
    }
}

void PodiumSpawnHero(s32 arg0, HubSpawnEnt *arg1) {
    NAS *nas;
    EXTengine *e;
    subengine *s;
    s32 k;
    s32 t;
    u8 *p;

    nas = CreateNAS(arg1->x, World_floor_level, arg1->z, 0x1F01, 0x11);
    if (nas != NULL) {
        D_800BD584[arg0] = nas;
        nas->UserInfo.user1 = arg0 + 1;
        nas->UserInfo.flags = arg1->flags;
        k = arg1->kind;
        if (k <= 0) {
            k = 1;
        }
        if (k >= 5) {
            k = 4;
        }
        if (k >= 2) {
            e = nas->Engine;
            if (e != NULL) {
                func_80018F3C(e, 1);
                s = e->sub;
                if (s != NULL && (s = s->next, s != NULL)) {
                    s->unk6C = &D_8005572C;
                    p = (u8 *)D_800BD1F0;
                    t = (k - 1) * 4;
                    s->unk74 = *(u16 *)(p + t);
                    s->flags |= 0x8000;
                    s = e->sub;
                    s->unk08 = -*(s32 *)((u8 *)D_800BD200 + t);
                }
            }
        }
    }
}

void PodiumRenderScene(void) {
    func_8009440C();
    func_80018B08(D_8009F67C, D_8009F734);
    func_8001C2C0(GetEngineSlot(6));
}

void UpdatePodium(void) {
    s32 i;
    s32 any = 0;

    for (i = 0; i < 4; i++) {
        if (((PadButtons *)D_80051338)[i].unk54 & 0x4000) {
            any = 1;
        }
    }
    if (any != 0 && D_800BD5B4 != 0 && D_800BD5B0 < 4U) {
        D_800BD5B0 = 4;
    }
    if (D_800BD5B0 == 0x64) {
        ((HubGameView *)D_8005A614)->unk24 = 0;
        ((HubGameView *)D_8005A614)->unk30 = 0x8002;
    }
    PodiumCheer();
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char D_800B3894[] = "DONE";
const char D_800B389C[] = "CANCEL";
const char D_800B38A4[] = "A";
const char D_800B38A8[] = "B";
const char D_800B38AC[] = "C";
const char D_800B38B0[] = "D";
const char D_800B38B4[] = "E";
const char D_800B38B8[] = "F";
const char D_800B38BC[] = "G";
const char D_800B38C0[] = "H";
const char D_800B38C4[] = "I";
const char D_800B38C8[] = "J";
const char D_800B38CC[] = "K";
const char D_800B38D0[] = "L";
const char D_800B38D4[] = "M";
const char D_800B38D8[] = "N";
const char D_800B38DC[] = "O";
const char D_800B38E0[] = "P";
const char D_800B38E4[] = "Q";
const char D_800B38E8[] = "R";
const char D_800B38EC[] = "S";
const char D_800B38F0[] = "T";
const char D_800B38F4[] = "U";
const char D_800B38F8[] = "V";
const char D_800B38FC[] = "W";
const char D_800B3900[] = "X";
const char D_800B3904[] = "Y";
const char D_800B3908[] = "Z";
const char D_800B390C[] = "@";
const char D_800B3910[] = "<";

void HubSaveMenuInput(void) {
    HubKey *k;
    PadButtons *pad;
    HubEntry12 *e;
    HubEntry12 *e4;
    subengine *s;
    subengine *cur;
    s32 i;
    s32 j;
    u8 c;

    if ((u32)(D_800BDB50 - 1) >= 2U) {
        s = D_800BDB58;
        i = D_800BF19C;
        cur = s + i;
        j = D_800BF1A0 * sizeof(HubEntry12);
        cur->unk7C = 0;
        k = (HubKey *)((HubEntry12 *)((u8 *)D_800BD7E0 + j))->unk0 + i;
        pad = (PadButtons *)D_80051338;
        if (pad->pressed & 0x80) {
            D_800BF19C += k->unkA;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pad->pressed & 0x20) {
            D_800BF19C += k->unkB;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pad->pressed & 0x10) {
            D_800BF19C += k->unkC;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pad->pressed & 0x40) {
            D_800BF19C += k->unkD;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pad->pressed & 0x4000) {
            switch (k->unk8) {
            case 4:
                e4 = &D_800BD7E0[D_800BF1A0];
                if (D_800BF1A4 < 8) {
                    D_8005A614[D_800BF1A4 + 0x18] = 0x20;
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                    if (D_800BF1A4 < 7) {
                        D_800BF1A4 += 1;
                    }
                    if (D_800BF1A4 == 8) {
                        D_800BF19C = e4->unk8;
                    }
                } else {
                    PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                break;
            case 0:
                e = &D_800BD7E0[D_800BF1A0];
                c = *k->unk4;
                if (D_800BF1A4 < 8) {
                    D_8005A614[D_800BF1A4 + 0x18] = c;
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                    if (D_800BF1A4 < 7) {
                        D_800BF1A4 += 1;
                    }
                    if (D_800BF1A4 == 8) {
                        D_800BF19C = e->unk8;
                    }
                } else {
                    PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                break;
            case 3:
                if (D_8005A614[D_800BF1A4 + 0x18] == 0x20) {
                    if (D_800BF1A4 > 0) {
                        D_800BF1A4 -= 1;
                        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                }
                D_8005A614[D_800BF1A4 + 0x18] = 0x20;
                break;
            case 1:
                PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                D_800BDB50 = 1;
                break;
            case 2:
                PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                D_800BDB50 = 2;
                break;
            }
        }
        D_800BDB58[D_800BF19C].unk7C = 2;
    }
}

void HubSaveMenuInit(void) {
    subengine *s;
    s32 i;
    SubInit *p;
    SubInitList *list;
    HubEntry12 *new_var;

    D_800BF100 = EngineTemplate;
    D_800BF19C = 0;
    D_800BF1A0 = 0;
    D_800BF19C = 0;
    D_800BF1A0 = 0;
    i = 0;
    kill_subs(D_800BF100.sub);
    func_8001D3B0(&D_800BF100, 0x21, D_800BDB58);
    s = D_800BDB58;
    list = (SubInitList *)D_800BD7E0;
    p = list->items;
    D_800BF100.unk10.x = 0xC4;
    D_800BF100.unk10.y = 0xBE;
    D_800BF100.flags |= 0x10000000;
    if (i < list->count) {
        do {
            func_80095F48((s32 *)s, p->a, p->b, p->c, 0);
            p++;
            s++;
            i++;
        } while (i < list->count);
    }
    D_800BDB50 = 0;
    for (D_800BF1A4 = 7; D_800BF1A4 > 0; D_800BF1A4--) {
        if (D_8005A614[D_800BF1A4 + 0x18] != 0x20) {
            break;
        }
    }
    if (D_800BF1A4 > 0) {
        new_var = &D_800BD7E0[D_800BF1A0];
        D_800BF19C = (*new_var).unk8;
    }
    set_fade(&D_8009F644, -0x300, 0);
    cam_init(D_800BF1A8);
    func_80095BEC(0, D_800BD8B0);
    GetEngineSlot(0)->unk10.x = 0xC8;
    GetEngineSlot(0)->unk10.y = 0x6E;
    func_80095BEC(1, D_800BD7FC);
    GetEngineSlot(1)->unk10.x = 0;
    GetEngineSlot(1)->unk10.y = 0;
}

void HubSaveMenuRender(void) {
    s32 i;
    u8 *g;
    s32 *p;
    EXTengine *e;

    func_80018B08(&D_800BDB14, (GfxCamera *)D_800BF1A8);
    D_800569AC = D_8009F644.amount;
    e = GetEngineSlot(0);
    i = 0;
    g = D_8005A614;
    p = (s32 *)((u8 *)e->sub + 0x540);
    do {
        D_800BD5B8[i * 2] = g[i + 0x18];
        if (D_800BD5B8[i * 2] == 0x20) {
            *p |= 0x8000;
        } else {
            *p &= ~0x8000;
        }
        p += 0x2A;
        i++;
    } while (i < 8);
    func_8001C2C0((u8 *)GetEngineSlot(0));
    func_8001C2C0((u8 *)GetEngineSlot(1));
    func_8001C2C0((u8 *)&D_800BF100);
}

void HubSaveMenuQuit(void) {
    func_80095A04(0);
    func_80095A04(1);
}

void HubSaveMenuUpdate(void) {
    HubSaveMenuInput();
    if (D_80051380 & 9) {
        D_8009F658.next_process = &D_8009F46C;
    }
}

void HubNameEntryKey(s8 arg0) {
    HubEntry12 *e;

    e = &D_800BD7E0[D_800BF1A0];
    if (D_800BF1A4 < 8) {
        D_8005A614[D_800BF1A4 + 0x18] = arg0;
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        if (D_800BF1A4 < 7) {
            D_800BF1A4 += 1;
        }
        if (D_800BF1A4 == 8) {
            D_800BF19C = e->unk8;
        }
    } else {
        PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void HubSaveMenuSubs(SubInitList *list) {
    s32 i;
    subengine *s;
    SubInit *p;

    kill_subs(D_800BF100.sub);
    do {
        i = 0;
        func_8001D3B0(&D_800BF100, 0x21, D_800BDB58);
    } while (0);
    s = D_800BDB58;
    D_800BF100.unk10.x = 0xC4;
    D_800BF100.unk10.y = 0xBE;
    D_800BF100.flags |= 0x10000000;
    p = list->items;
    if (list->count > 0) {
        do {
            func_80095F48((s32 *)s, p->a, p->b, p->c, 0);
            p++;
            s++;
            i++;
        } while (i < list->count);
    }
}

void HubSaveMenuReset(void) {
    D_800BF100 = EngineTemplate;
    D_800BF19C = 0;
    D_800BF1A0 = 0;
}

void HubProjectObj(HubRenderObj *obj, s16 *out) {
    SVECTOR v[8];
    s16 xy[2];
    u8 *m;
    s16 *bb;
    s32 off;
    s16 *p;

    m =(u8 *)map_entity_address(obj->unk74, (u8 *)obj->sub->unkC);
    off = *(s32 *)(m + 0x10);
    out[0] = 0x7FFF;
    out[1] = 0x7FFF;
    out[2] = -0x8000;
    out[3] = -0x8000;
    bb = (s16 *)(m + (off + 0x10));
    p = xy;
    v[0].vx = bb[0];
    v[0].vy = bb[1];
    v[0].vz = bb[2];
    v[1].vx = bb[3];
    v[1].vy = bb[1];
    v[1].vz = bb[2];
    v[2].vx = bb[0];
    v[2].vy = bb[4];
    v[2].vz = bb[2];
    v[3].vx = bb[3];
    v[3].vy = bb[4];
    v[3].vz = bb[2];
    v[4].vx = bb[0];
    v[4].vy = bb[1];
    v[4].vz = bb[5];
    v[5].vx = bb[3];
    v[5].vy = bb[1];
    v[5].vz = bb[5];
    v[6].vx = bb[0];
    v[6].vy = bb[4];
    v[6].vz = bb[5];
    v[7].vx = bb[3];
    v[7].vy = bb[4];
    v[7].vz = bb[5];
    gte_ldv0(&v[0]);
    gte_rtps();
    gte_stsxy(p);
    if (xy[0] < out[0]) {
        out[0] = xy[0];
    }
    if (xy[0] > out[2]) {
        out[2] = xy[0];
    }
    if (xy[1] < out[1]) {
        out[1] = xy[1];
    }
    if (xy[1] > out[3]) {
        out[3] = xy[1];
    }
    HUB_BOUND_XY(&v[1], out, p, xy);
    HUB_BOUND_XY(&v[2], out, p, xy);
    HUB_BOUND_XY(&v[3], out, p, xy);
    HUB_BOUND_XY(&v[4], out, p, xy);
    HUB_BOUND_XY(&v[5], out, p, xy);
    HUB_BOUND_XY(&v[6], out, p, xy);
    HUB_BOUND_XY(&v[7], out, p, xy);
}

void HubSetDrawArea(s16 *a, void *b, s32 c, s32 d) {
    RECT *r;
    RECT *rc;
    u32 *p;
    u32 *q;
    s32 off2;
    s32 off;

    r = D_8005B698;
    if (cdb == D_8005B790) {
        r += 1;
    }
    SetDrawArea((DRAWENV *)((u8 *)b + (D_8005B690 & 1) * 12), r);
    rc = (RECT *)a;
    rc->x += r->x;
    rc->y += r->y;
    SetDrawArea((DRAWENV *)((u8 *)b + ((D_8005B690 & 1) * 12 + 0x18)), rc);
    off = (D_8005B690 & 1) * 12;
    off2 = off + 0x18;
    p = (u32 *)(off + (s32)b);
    ((HubPTag *)&p[6])->addr = ((HubPTag *)&D_80056998.ot[0x136])->addr;
    ((HubPTag *)&D_80056998.ot[0x136])->addr = (u32)((u8 *)b + off2);
    ((HubPTag *)&p[0])->addr = ((HubPTag *)&D_80056998.ot[2])->addr;
    ((HubPTag *)&D_80056998.ot[2])->addr = (u32)p;
    q = ((u32 *)cdb) - (-c);
    ((HubPTag *)&D_80056998.ot[0])->addr = ((HubPTag *)q)->addr;
    ((HubPTag *)q)->addr = (u32)(D_80056998.ot + 0x137);
}

void HubStub(void) {
}

void HubDrawObj(HubRenderObj *obj, void *arg1) {
    s16 sp10[4];
    s16 sp18[4];
    s32 c;
    s32 h;
    s32 y;
    s32 d;
    u32 *saved;

    c = obj->sub->unkC;
    if ((obj->flags & 0x8000) && c != 0) {
        saved = D_80056998.ot;
        func_8001D894(obj, arg1, obj->flags, c);
        D_80056998.ot = (u32 *)(D_800BF264 + ((D_8005B690 & 1) << 14));
        ClearOTagR(D_80056998.ot, 0x138);
        obj->flags |= 0x2000000;
        obj->unk68 = -0x170;
        func_80019A60(obj->unk74, (u8 *)c, obj->flags, obj);
        HubProjectObj(obj, sp10);
        d = sp10[3];
        y = sp10[1];
        h = (D_800BF260 * (d - y)) / 4096;
        sp18[2] = 0x200;
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[3] = h + y;
        HubSetDrawArea(sp18, D_800C7264, 0x400, y);
        D_80056998.ot = saved;
    }
}

void HubProject(SVECTOR *v, s16 *b) {
    s16 xy[2];

    gte_ldv0(v);
    gte_rtps();
    gte_stsxy(xy);
    if (xy[0] < b[0]) {
        b[0] = xy[0];
    }
    if (xy[0] > b[2]) {
        b[2] = xy[0];
    }
    if (xy[1] < b[1]) {
        b[1] = xy[1];
    }
    if (b[3] < xy[1]) {
        b[3] = xy[1];
    }
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char D_800B3928[] = "ENTER NAME";
const char D_800B3934[4] = ".\000\"\226";
const char D_800B3938[] = "DEVELOPED BY";
const char D_800B3948[] = "EUROCOM ENTERTAINMENT";
const char D_800B3960[] = "SOFTWARE";
const char D_800B396C[] = "IN ASSOCATION WITH";
const char D_800B3980[] = "CERNY GAMES";
const char D_800B398C[] = "PRODUCERS";
const char D_800B3998[] = "JON WILLIAMS";
const char D_800B39A8[] = "MARK CERNY";
const char D_800B39B4[] = "PROGRAMMING";
const char D_800B39C0[] = "DAVE PRIDMORE";
const char D_800B39D0[] = "STUART JOHNSON";
const char D_800B39E0[] = "JULIAN WALSHAW-VAUGHAN";
const char D_800B39F8[] = "STEVE BAK";
const char D_800B3A04[] = "KRIS ADCOCK";
const char D_800B3A10[] = "GRAPHICS";
const char D_800B3A1C[] = "MATT DIXON";
const char D_800B3A28[] = "STEVE BAMFORD";
const char D_800B3A38[] = "CLIVE STEVENSON";
const char D_800B3A48[] = "JON PARR";
const char D_800B3A54[] = "ANIMATION";
const char D_800B3A60[] = "MARK POVEY";
const char D_800B3A6C[] = "BRYAN ROGERS";
const char D_800B3A7C[] = "BRIAN MALONE-WEST";
const char D_800B3A90[] = "MUSIC AND SOUND EFFECTS";
const char D_800B3AA8[] = "STEVE DUCKWORTH";
const char D_800B3AB8[] = "ASSISTANT PRODUCER";
const char D_800B3ACC[] = "PHIL BENNETT";
const char D_800B3ADC[] = "TOOLS SUPPORT";
const char D_800B3AEC[] = "ANDY MITCHELL";
const char D_800B3AFC[] = "CHRIS JACKSON";
const char D_800B3B0C[] = "KEVIN MARKS";
const char D_800B3B18[] = "QUALITY ASSURANCE MANAGER";
const char D_800B3B34[] = "MICHAEL ROBINSON";
const char D_800B3B48[] = "QUALITY ASSURANCE";
const char D_800B3B5C[] = "ANDY DILKS";
const char D_800B3B68[] = "RICH HIGGINS";
const char D_800B3B78[] = "ANDY LALLY";
const char D_800B3B84[] = "JOHN LEE BARKER";
const char D_800B3B94[] = "CHRIS J. JACKSON";
const char D_800B3BA8[] = "RICHARD NICHOLLS";
const char D_800B3BBC[] = "MARK SHAW";
const char D_800B3BC8[] = "RICHARD CHARLES";
const char D_800B3BD8[] = "SPECIAL THANKS";
const char D_800B3BE8[] = "MICHAEL JOHN";
const char D_800B3BF8[] = "HUGH BINNS";
const char D_800B3C04[] = "MAT SNEAP";
const char D_800B3C10[] = "JIM WILSON";
const char D_800B3C1C[] = "RACHEL HALL";
const char D_800B3C28[] = "MIKE BOTHAM";
const char D_800B3C34[] = "CARL CROPLEY";
const char D_800B3C44[] = "CORNELL JOHN";
const char D_800B3C54[] = "PRODUCED BY";
const char D_800B3C60[] = "UNIVERSAL INTERACTIVE";
const char D_800B3C78[] = "STUDIOS";
const char D_800B3C80[] = "SENIOR PRODUCER";
const char D_800B3C90[] = "VIJAY LAKSHMAN";
const char D_800B3CA0[] = "PRODUCTION COORDINATORS";
const char D_800B3CB8[] = "MELISSA MILLER";
const char D_800B3CC8[] = "BRIAN STEPHENS";
const char D_800B3CD8[] = "ADDITIONAL MINIGAME DESIGN";
const char D_800B3CF4[] = "SEAN KRANKEL";
const char D_800B3D04[] = "RICCI RUKAVINA";
const char D_800B3D14[] = "ANTHONY AHN";
const char D_800B3D20[] = "SEAN MOUNTAIN";
const char D_800B3D30[] = "NOEL GALLAN";
const char D_800B3D3C[] = "JOSH GOTTSEGEN";
const char D_800B3D4C[] = "DIRECTOR OF MARKETING";
const char D_800B3D64[] = "PRITY PATEL";
const char D_800B3D70[] = "MARKETING COORDINATOR";
const char D_800B3D88[] = "MARCUS SAVINO";
const char D_800B3D98[] = "PUBLISHED AND PRODUCED BY";
const char D_800B3DB4[] = "SONY COMPUTER";
const char D_800B3DC4[] = "ENTERTAINMENT";
const char D_800B3DD4[] = "AMERICA";
const char D_800B3DDC[] = "DONOVAN SOTO";
const char D_800B3DEC[] = "PRODUCER";
const char D_800B3DF8[] = "GRADY HUNT";
const char D_800B3E04[] = "SAM THOMPSON";
const char D_800B3E14[] = "DIRECTOR OF\01516PRODUCT DEVELOPMENT";
const char D_800B3E38[] = "CONNIE BOOTH";
const char D_800B3E48[] = "VICE PRESIDENT OF\01516PRODUCT DEVELOPMENT";
const char D_800B3E70[] = "SHUHEI YOSHIDA";
const char D_800B3E80[] = "AMI MATSUMURA-BLAIRE";
const char D_800B3E98[] = "ASSOCIATE PRODUCT MANAGER";
const char D_800B3EB4[] = "RON EAGLE";
const char D_800B3EC0[] = "LEGAL AND INTELLECTUAL\01516PROPERTY";
const char D_800B3EE4[] = "KIRSTEN COSTELLO";
const char D_800B3EF8[] = "KERRY HOPKINS";
const char D_800B3F08[] = "LISA LUNGER";
const char D_800B3F14[] = "RILEY RUSSELL";
const char D_800B3F24[] = "MARKET RESEARCH";
const char D_800B3F34[] = "JOHN CROMPTON";
const char D_800B3F44[] = "PROMOTIONS";
const char D_800B3F50[] = "SHARON SHAPIRO";
const char D_800B3F60[] = "JANEEN ANDERSON";
const char D_800B3F70[] = "DONNA ARMENTOR";
const char D_800B3F80[] = "AIMEE DUELL";
const char D_800B3F8C[] = "PUBLIC RELATIONS";
const char D_800B3FA0[] = "MOLLY SMITH";
const char D_800B3FAC[] = "RYAN BOWLING";
const char D_800B3FBC[] = "STEPHANIE IWAMASA";
const char D_800B3FD0[] = "CHARLOTTE PANTHER";
const char D_800B3FE4[] = "EILEEN RODRIQUEZ";
const char D_800B3FF8[] = "CREATIVE SERVICES";
const char D_800B400C[] = "MARILYN WEYANT";
const char D_800B401C[] = "JOSH BINGHAM";
const char D_800B402C[] = "JOHN DIAMONON";
const char D_800B403C[] = "PEGGY GALLAGHER";
const char D_800B404C[] = "TED JALBERT";
const char D_800B4058[] = "QUINN LEE";
const char D_800B4064[] = "MARIE MACASPAC";
const char D_800B4074[] = "RUTH SATO";
const char D_800B4080[] = "JACK SILVER";
const char D_800B408C[] = "QA MANAGER";
const char D_800B4098[] = "CHRIS CAPRIO";
const char D_800B40A8[] = "SUPERVISORS";
const char D_800B40B4[] = "BRUCE COCHRANE";
const char D_800B40C4[] = "TIM DUZMAL";
const char D_800B40D0[] = "CONNER MORLANG";
const char D_800B40E0[] = "TECHNICAL COORDINATORS";
const char D_800B40F8[] = "BEN BRIONES";
const char D_800B4104[] = "DATABASE COORDINATOR";
const char D_800B411C[] = "CHRISTIAN DAVIES";
const char D_800B4130[] = "LEAD TESTERS";
const char D_800B4140[] = "ANDREW BYRNE";
const char D_800B4150[] = "DEREK RAYLA";
const char D_800B415C[] = "ASSISTANT LEAD TESTERS";
const char D_800B4174[] = "MICHAEL CHANG";
const char D_800B4184[] = "STEVEN GONZALEZ";
const char D_800B4194[] = "TERESA KOSSOB";
const char D_800B41A4[] = "TESTERS";
const char D_800B41AC[] = "ANGELICA BERNARBE";
const char D_800B41C0[] = "ERIC BODINE";
const char D_800B41CC[] = "COBALT DENA";
const char D_800B41D8[] = "JENNYFER HAMILTON";
const char D_800B41EC[] = "BURR HILSABECK";
const char D_800B41FC[] = "RICH KRINOK";
const char D_800B4208[] = "JOEL MANZANO";
const char D_800B4218[] = "ROSS MONTGOMERY";
const char D_800B4228[] = "ERIC ROSSI";
const char D_800B4234[] = "AARON WHITMORE";
const char D_800B4244[] = "ANDREW WOODWORTH";
const char D_800B4258[] = "CHRIS ARENDS";
const char D_800B4268[] = "ERIC FRANKLIN";
const char D_800B4278[] = "GREG PHILLIPS";
const char D_800B4288[] = "GRETEL PEREZ";
const char D_800B4298[] = "HENRY MACAIRAN";
const char D_800B42A8[] = "JACOB SLIPAK";
const char D_800B42B8[] = "JASON GREEN";
const char D_800B42C4[] = "JO ANNE LARSEN";
const char D_800B42D4[] = "JOE RAPOSO";
const char D_800B42E0[] = "LEONARD KO";
const char D_800B42EC[] = "MIKAL DACE";
const char D_800B42F8[] = "NATHAN FRANTZ";
const char D_800B4308[] = "RAMON CONCEPCION";
const char D_800B431C[] = "RUSSELL YOUNG";
const char D_800B432C[] = "SEAN PHELPS";
const char D_800B4338[] = "SEANN PARKER";
const char D_800B4348[] = "SHANNON LYNAS";
const char D_800B4358[] = "SHAWN REFOUA";
const char D_800B4368[] = "STEPHANIE BEIN";
const char D_800B4378[] = "STERLING HAYWOOD";
const char D_800B438C[] = "TONY FELICE";
const char D_800B4398[] = "WILL SHEPARD";
const char D_800B43A8[] = "SETH EVANS";
const char D_800B43B4[] = "BRUCE ADAMS";
const char D_800B43C0[] = "GARY BARTH";
const char D_800B43CC[] = "CLAUDETTE CASTILLO";
const char D_800B43E0[] = "LORI CHASE";
const char D_800B43EC[] = "SARA CHENOWETH";
const char D_800B43FC[] = "EDDY CHU";
const char D_800B4408[] = "MIKE CZAKO";
const char D_800B4414[] = "BRIAN DIMICK";
const char D_800B4424[] = "ALBERTO ESCOBEDO";
const char D_800B4438[] = "BOB FREMGEN";
const char D_800B4444[] = "GERRY GENTILE";
const char D_800B4454[] = "BRIAN HALE";
const char D_800B4460[] = "KARA HARRIS";
const char D_800B446C[] = "PHIL HARRISON";
const char D_800B447C[] = "KAZ HIRAI";
const char D_800B4488[] = "BETSEY HOROWITZ";
const char D_800B4498[] = "ANDREW HOUSE";
const char D_800B44A8[] = "JEFF HUTCHINSON";
const char D_800B44B8[] = "GRACE KAO";
const char D_800B44C4[] = "LORAINE KRAUS";
const char D_800B44D4[] = "CHUCK LACSON";
const char D_800B44E4[] = "COLIN MACLEAN";
const char D_800B44F4[] = "KRIS MATHUR";
const char D_800B4500[] = "FRANK O'MALLEY";
const char D_800B4510[] = "JOEL PAMBID";
const char D_800B451C[] = "GARY PASCOE";
const char D_800B4528[] = "JOHN PAYNE";
const char D_800B4534[] = "RICH LAROCCA";
const char D_800B4544[] = "MAGGIE ROJAS";
const char D_800B4554[] = "RICK ROONEY";
const char D_800B4560[] = "STEVE ROSS";
const char D_800B456C[] = "PETER STEINZEIG";
const char D_800B457C[] = "JONI TONEY";
const char D_800B4588[] = "JACK TRETTON";
const char D_800B4598[] = "MARK VALLEDOR";
const char D_800B45A8[] = "TONEY WONG";
const char D_800B45B4[] = "FLEISHMAN-HILLARD";
const char D_800B45C8[] = "TBWA - CHIAT DAY";
const char D_800B45DC[] = "AXIOM DESIGNS";
const char D_800B45EC[] = "SONY COMPUTER ENTERTAINMENT JAPAN";
const char D_800B4610[] = "RYOICHI HASEGAWA";
const char D_800B4624[] = "ATSUSHI MATSUSHITA";
const char D_800B4638[] = "SONY COMPUTER ENTERTAINMENT EUROPE";
const char D_800B465C[] = "ELIZABETH ASHFORD";
const char D_800B4670[] = "KENNY MATHERS";
const char D_800B4680[20] = "ISABELLE TOMATIS\000\000\004\225";

/* data, in address order */
s32 D_800BC2D0[1] = { (s32)&D_800B330C };
s32 D_800BC2D4[1] = { (s32)&D_800B333C };
s32 D_800BC2D8[1] = { (s32)&D_800B3364 };
s32 D_800BC2DC[1] = { (s32)&D_800B33A8 };
s32 D_800BC2E0[1] = { (s32)&D_800B33EC };
s32 D_800BC2E4[1] = { (s32)&D_800B342C };
s32 D_800BC2E8[1] = { (s32)&D_800B3458 };
s32 D_800BC2EC[1] = { (s32)&D_800B3484 };
s32 D_800BC2F0[1] = { (s32)&D_800B34A8 };
s32 D_800BC2F4[1] = { (s32)&D_800B34D4 };
s32 D_800BC2F8[1] = { (s32)&D_800B3500 };
s32 D_800BC2FC[27] = {
    4, 0x12C8000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&D_800BC2D0, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC368[27] = {
    4, 0x12C8000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&D_800BC2D4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC3D4[27] = {
    4, 0x1908000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&D_800BC2D8, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC440[27] = {
    4, 0x1868000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&D_800BC2DC, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC4AC[27] = {
    4, 0x1728000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&D_800BC2E0, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC518[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&D_800BC2E4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC584[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&D_800BC2E8, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC5F0[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&D_800BC2EC, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC65C[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&D_800BC2F0, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC6C8[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&D_800BC2F4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC734[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&D_800BC2F8, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BC7A0[1] = { (s32)&D_800B3528 };
s32 D_800BC7A4[27] = {
    4, 0x12C8000, 0x2C0000, 0, 0, 0, 0, 0, 0, 0xE, 0xE00AA, 0, 0, 0, (s32)&D_800BC7A0, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *D_800BC810[1] = { (void *)&D_800BC7A4 };
s32 D_800BC814[1] = { (s32)&D_800B3540 };
s32 D_800BC818[27] = {
    4, 0x12C8000, 0x2C0000, 0, 0, 0, 0, 0, 0, 0xE, 0xE00AA, 0, 0, 0, (s32)&D_800BC814, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *D_800BC884[1] = { (void *)&D_800BC818 };
s32 D_800BC888[36] = {
    4, 0x1A48000, 0x370000, 0, 0, 0, 0, 0, 0, 3, 0x48000, 0, 0, 0, (s32)&D_800B3558, 4, 0, 0,
    3, 0x1E8000, 0, 0, 0, (s32)&D_800B3540, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *D_800BC918[1] = { (void *)&D_800BC888 };
s32 D_800BC91C[36] = {
    4, 0x18B8000, 0x3C0000, 0, 0, 0, 0, 0, 0, 3, 0x68000, 0, 0, 0, (s32)&D_800B3580, 4, 0, 0,
    3, 0x268000, 0, 0, 0, (s32)&D_800B35A8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *D_800BC9AC[1] = { (void *)&D_800BC91C };
s32 D_800BC9B0[27] = {
    4, 0x1908000, 0x3A0000, 0, 0, 0, 0, 0, 0, 3, 0xA8000, 0, 0, 0, (s32)&D_800B35C0, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *D_800BCA1C[1] = { (void *)&D_800BC9B0 };
EngRec24 D_800BCA20[8] = {
    {
        {
            4, 0, 0, 0, 0x5A, 0, 0xBE, 0, 0, 0, 0x56, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
        },
        0, { 0, 0, 0, 0 },
    },
    {
        {
            4, 0, 0, 0, 0, 0, 0xC2, 1, 0, 0, 0x56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0,
        },
        0, { 0, 0, 0, 0 },
    },
    {
        {
            5, 0, 0, 0, 0x2D, 0, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0xD5, 0,
        },
        0, { 2, 0, 0, 0 },
    },
    {
        {
            5, 0, 0, 0, 0x87, 0, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0xD5, 0,
        },
        6, { 2, 0, 0, 0 },
    },
    {
        {
            5, 0, 0, 0, 0xE1, 0, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0xD5, 0,
        },
        8, { 2, 0, 0, 0 },
    },
    {
        {
            5, 0, 0, 0, 0x3B, 1, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0xD5, 0,
        },
        6, { 2, 0, 0, 0 },
    },
    {
        {
            5, 0, 0, 0, 0x95, 1, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0xD5, 0,
        },
        6, { 2, 0, 0, 0 },
    },
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0,
        },
        0, { 0, 0, 0, 0 },
    },
};
s32 D_800BCB40[5] = { 0, 0x5A, 0xB4, 0x10E, 0x168 };
KeyedPair D_800BCB54[11] = {
    { 0xB, { (s32)&D_800BC2FC } }, { 4, { (s32)&D_800BC368 } }, { 0x16, { (s32)&D_800BC3D4 } },
    { 9, { (s32)&D_800BC440 } }, { 0x1F, { (s32)&D_800BC4AC } },
    { 0x1C, { (s32)&D_800BC518 } }, { 0x1D, { (s32)&D_800BC584 } },
    { 0x1E, { (s32)&D_800BC5F0 } }, { 0x1A, { (s32)&D_800BC65C } },
    { 0x19, { (s32)&D_800BC6C8 } }, { 5, { (s32)&D_800BC734 } },
};
CamEase D_800BCBAC = { { 4, 4, 4 }, { 0x10, 4, 1 }, { 1, 0x10, 0x10 } };
s32 D_800BCBD0[3] = { 1, 1, 1 };
u16 D_800BCBDC[10] = { 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0xC00, 0xC00, 0xC00, 0xC00, 0x1000 };
char *D_800BCBF0[5] = {
    (void *)&D_800B3618, (void *)&D_800B3670, (void *)&D_800B36C4, (void *)&D_800B371C,
    (void *)&D_800B3770,
};
process_info D_800BCC04 = { HubDoorMenuInit, HubDoorMenuUpdate, HubDoorMenuRender, HubDoorMenuQuit, 0 };
process_info D_800BCC18 = { HubDoorMenu2Init, HubDoorMenu2Update, HubDoorMenu2Render, HubDoorMenu2Quit, 0 };
s32 D_800BCC2C[11] = { 0x2019, 0x201A, 0x201B, 0x201C, 0x201D, 0x201E, 0x201D, 0x201C, 0x201B, 0x201A, 0x8000 };
s32 D_800BCC58[2] = { 0x22019, 0x8000 };
s32 D_800BCC60[11] = { 0x201F, 0x2020, 0x2021, 0x2022, 0x2023, 0x2024, 0x2023, 0x2022, 0x2021, 0x2020, 0x8000 };
s32 D_800BCC8C[2] = { 0x22019, 0x8000 };
s32 D_800BCC94[11] = { 0x2025, 0x2026, 0x2027, 0x2028, 0x2029, 0x202A, 0x2029, 0x2028, 0x2027, 0x2026, 0x8000 };
s32 D_800BCCC0[2] = { 0x22025, 0x8000 };
s32 D_800BCCC8[8] = {
    (s32)&D_800BCC2C, (s32)&D_800BCC58, (s32)&D_800BCC60, (s32)&D_800BCC8C, (s32)&D_800BCC94,
    (s32)&D_800BCCC0, 0, 0,
};
ScriptInit D_800BCCE8 = { 0, (void *)&D_800BCCC8, 0x402 };
EngRec90248 D_800BCCF4[12] = {0};
s32 D_800BCED4 = 0;
s32 D_800BCED8 = 0;
u8 D_800BCEDC = 0;
s32 D_800BCEE0 = 0;
s32 D_800BCEE4 = 0;
subengine *D_800BCEE8 = 0;
_sfx_handler *D_800BCEEC = 0;
s32 D_800BCEF0 = 0;
s32 D_800BCEF4 = 0;
s32 D_800BCEF8[1] = {0};
s32 D_800BCEFC = 0;
s32 D_800BCF00[3] = { (s32)0x80110000, 0x41000019, 0x8000 };
s32 D_800BCF0C[3] = { (s32)0x80110000, 0x41190017, 0x8001 };
s32 D_800BCF18[3] = { (s32)0x80110000, 0x40000020, 0x8000 };
s32 D_800BCF24[9] = {
    (s32)0x80120000, 0x40A5000B, 0xB8011, (s32)0x8011408E, 0x408E000B, 0xB8011,
    (s32)0x8011408E, 0x408E0017, 0x8001,
};
s32 D_800BCF48[8] = { (s32)&D_800BCF00, (s32)&D_800BCF0C, 0, 0, (s32)&D_800BCF18, (s32)&D_800BCF24, 0, 4 };
ScriptInit D_800BCF68 = { 0, (void *)&D_800BCF48, 0x401 };
s32 D_800BCF74[3] = { (s32)0x80110000, 0x4000001C, 0x8000 };
s32 D_800BCF80[3] = { (s32)0x80110000, 0x41000064, 0x8001 };
s32 D_800BCF8C[3] = { (s32)0x80110000, 0x40800038, 0x8001 };
s32 D_800BCF98[8] = { (s32)&D_800BCF74, (s32)&D_800BCF80, 0, 0, (s32)&D_800BCF74, (s32)&D_800BCF8C, 0, 4 };
ScriptInit D_800BCFB8 = { 0, (void *)&D_800BCF98, 0x401 };
s32 D_800BCFC4[3] = { (s32)0x80110000, 0x40000016, 0x8000 };
s32 D_800BCFD0[3] = { (s32)0x80110000, 0x40800022, 0x8001 };
s32 D_800BCFDC[3] = { (s32)0x80110000, 0x4100005A, 0x8001 };
s32 D_800BCFE8[8] = { (s32)&D_800BCFC4, (s32)&D_800BCFD0, 0, 0, (s32)&D_800BCFC4, (s32)&D_800BCFDC, 0, 4 };
ScriptInit D_800BD008 = { 0, (void *)&D_800BCFE8, 0x401 };
s32 D_800BD014[3] = { (s32)0x80110000, 0x41000020, 0x8000 };
s32 D_800BD020[6] = { (s32)0x80110000, 0x40000020, 0x168011, (s32)0x80124009, 0x4009000A, 0x8001 };
s32 D_800BD038[6] = { (s32)0x80110000, 0x4080002F, 0x1F8011, (s32)0x8011408F, 0x40AF0007, 0x8001 };
s32 D_800BD050[8] = { (s32)&D_800BD014, (s32)&D_800BD020, 0, 0, (s32)&D_800BD014, (s32)&D_800BD038, 0, 4 };
ScriptInit D_800BD070 = { 0, (void *)&D_800BD050, 0x401 };
s32 D_800BD07C[3] = { (s32)0x80110000, 0x40000016, 0x8000 };
s32 D_800BD088[3] = { (s32)0x80110000, 0x40800060, 0x8001 };
s32 D_800BD094[3] = { (s32)0x80110000, 0x4180001E, 0x8000 };
s32 D_800BD0A0[6] = { (s32)0x80110000, 0x41000020, 0x148011, (s32)0x8011410B, 0x410B001C, 0x8001 };
s32 D_800BD0B8[8] = { (s32)&D_800BD07C, (s32)&D_800BD088, 0, 0, (s32)&D_800BD094, (s32)&D_800BD0A0, 0, 4 };
ScriptInit D_800BD0D8 = { 0, (void *)&D_800BD0B8, 0x401 };
s32 D_800BD0E4[3] = { (s32)0x80110000, 0x41000014, 0x8000 };
s32 D_800BD0F0[3] = { (s32)0x80110000, 0x4180000D, 0x8001 };
s32 D_800BD0FC[3] = { (s32)0x80110000, 0x40000014, 0x8000 };
s32 D_800BD108[3] = { (s32)0x80110000, 0x40800043, 0x8000 };
s32 D_800BD114[8] = { (s32)&D_800BD0E4, (s32)&D_800BD0F0, 0, 0, (s32)&D_800BD0FC, (s32)&D_800BD108, 0, 4 };
ScriptInit D_800BD134 = { 0, (void *)&D_800BD114, 0x401 };
s32 D_800BD140[3] = { (s32)0x80110000, 0x40000020, 0x8000 };
s32 D_800BD14C[5] = { (s32)0x80110000, 0x4080004F, 0x218012, (s32)0x800140A1, 0 };
s32 D_800BD160[3] = { (s32)0x80110000, 0x4100003C, 0x8001 };
s32 D_800BD16C[8] = { (s32)&D_800BD140, (s32)&D_800BD14C, 0, 0, (s32)&D_800BD140, (s32)&D_800BD160, 0, 4 };
ScriptInit D_800BD18C = { 0, (void *)&D_800BD16C, 0x401 };
s32 D_800BD198[3] = { (s32)0x80110000, 0x41000020, 0x8000 };
s32 D_800BD1A4[3] = { (s32)0x80110000, 0x40800066, 0x8001 };
s32 D_800BD1B0[5] = { (s32)0x80110000, 0x40000040, 0x1E8012, (s32)0x8001401E, 0 };
s32 D_800BD1C4[8] = { (s32)&D_800BD198, (s32)&D_800BD1A4, 0, 0, (s32)&D_800BD198, (s32)&D_800BD1B0, 0, 4 };
ScriptInit D_800BD1E4 = { 0, (void *)&D_800BD1C4, 0x401 };
u16 D_800BD1F0[8] = { 0x200C, 0, 0x200C, 0, 0x200B, 0, 0x2009, 0 };
s32 D_800BD200[4] = { 0, 0x140, 0x180, 0x200 };
u8 D_800BD210[384] = {
    0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0xFD, 0xFF, 0xFF, 0, 0, 0, 0, 3, 1, 0, 0, 0, 3, 0,
    0, 0, 0, 0, 0, 2, 1, 0, 0, 0x80, 2, 0, 0, 0, 0xFE, 0xFF, 0xFF, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 4, 0, 0, 0, 0, 0xFD, 0xFF, 0xFF, 0, 0, 0, 0, 3, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0xC0, 1, 0, 0, 0, 0xFE, 0xFF, 0xFF, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0,
    0, 0, 0, 0xFD, 0xFF, 0xFF, 0, 0, 0, 0, 3, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0,
    0x80, 2, 0, 0, 0, 0xFE, 0xFF, 0xFF, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0,
    0xFD, 0xFF, 0xFF, 0, 0, 0, 0, 4, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0x80, 2, 0,
    0, 0, 0xFE, 0xFF, 0xFF, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0xFD, 0xFF,
    0xFF, 0, 0, 0, 0, 4, 0, 0, 0, 0x80, 0xFE, 0xFF, 0xFF, 0, 0xFE, 0xFF, 0xFF, 1, 1, 0, 0,
    0x80, 2, 0, 0, 0, 0xFE, 0xFF, 0xFF, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0x80,
    0xFD, 0xFF, 0xFF, 0, 0xFE, 0xFF, 0xFF, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0xFE, 0xFF, 0xFF, 1, 1,
    0, 0, 0x80, 2, 0, 0, 0, 0xFE, 0xFF, 0xFF, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0,
    0, 0xFD, 0xFF, 0xFF, 0, 0, 0, 0, 4, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0x80, 2,
    0, 0, 0, 0xFE, 0xFF, 0xFF, 1, 1, 0, 0, 0, 0xFD, 0xFF, 0xFF, 0, 0, 0, 0, 1, 0, 0, 0, 0x80,
    0xFE, 0xFF, 0xFF, 0, 0xFE, 0xFF, 0xFF, 1, 0, 0, 0, 0x80, 1, 0, 0, 0, 0xFE, 0xFF, 0xFF, 1,
    0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
};
s32 D_800BD390 = 0;
s32 D_800BD394 = 0;
u8 *D_800BD398 = (void *)&D_800B37EC;
s32 D_800BD39C[4] = { (s32)&D_800B37F8, (s32)&D_800B37FC, (s32)&D_800B380C, 0 };
u8 D_800BD3AC[4] = { 0x31, 0x30, 0, 0 };
u8 D_800BD3B0[4] = { 0x32, 0x30, 0, 0 };
u8 D_800BD3B4[4] = { 0x33, 0x30, 0, 0 };
u8 D_800BD3B8[12] = { 0x34, 0x30, 0, 0, 0x35, 0x30, 0, 0, 0x36, 0x30, 0, 0 };
s32 D_800BD3C4 = 4;
s32 D_800BD3C8[106] = {
    0x2300000, 0x560000, 0, 0, 0, 0, 0, 0, 0xA, 0x400118, 0x18000000, (s32)0xFF800100, 0, 0, 0,
    0x2001001C, 2, 8, 0x100000, 0x7000000, 0, 0, 0, 0, 0, 0, 3, 0x40003D, 0, 0, 0,
    (s32)&D_800BD3AC, 4, 0, 0, 8, 0x100070, 0x7000000, 0, 0, 0, 0, 0x10000, 0, 3, 0x4000AD, 0,
    0, 0, (s32)&D_800BD3B0, 4, 0, 0, 8, 0x100150, 0x7000000, 0, 0, 0, 0, 0x20000, 0, 3,
    0x40018D, 0, 0, 0, (s32)&D_800BD3B4, 4, 0, 0, 8, 0x1001C0, 0x7000000, 0, 0, 0, 0, 0x30000,
    0, 3, 0x4001FD, 0, 0, 0, (s32)&D_800BD3B8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1000, 0,
    0x1000, 0, 0x1000, 0, 0, 0,
};
HubSparkFx D_800BD570 = { 0xC, 0xF00, 0xF00, HubInitSparks, HubDrawSparks };
NAS *D_800BD584[4] = {0};
PodiumObj *D_800BD594 = 0;
s32 D_800BD598[4] = {0};
s32 D_800BD5A8 = 0;
s32 D_800BD5AC = 0;
u32 D_800BD5B0 = 0;
s32 D_800BD5B4 = 0;
u8 D_800BD5B8[4] = { 0x20, 0, 0x20, 0 };
s32 D_800BD5BC[1] = { 0x200020 };
s32 D_800BD5C0[1] = { 0x200020 };
s32 D_800BD5C4[3] = { 0x200020, (s32)&D_800B3894, (s32)&D_800B389C };
s32 D_800BD5D0[120] = {
    0, (s32)&D_800B38A4, 0x1060000, 0x715, 0x28, (s32)&D_800B38A8, 0x1FF0000, 0x715, 0x50,
    (s32)&D_800B38AC, 0x1FF0000, 0x715, 0x78, (s32)&D_800B38B0, 0x1FF0000, 0x715, 0xA0,
    (s32)&D_800B38B4, 0x1FF0000, 0x715, 0xC8, (s32)&D_800B38B8, 0x1FF0000, 0x715, 0xF0,
    (s32)&D_800B38BC, (s32)0xFAFF0000, 0x715, 0x280000, (s32)&D_800B38C0, 0x1060000, 0x7F9,
    0x280028, (s32)&D_800B38C4, 0x1FF0000, 0x7F9, 0x280050, (s32)&D_800B38C8, 0x1FF0000, 0x7F9,
    0x280078, (s32)&D_800B38CC, 0x1FF0000, 0x7F9, 0x2800A0, (s32)&D_800B38D0, 0x1FF0000, 0x7F9,
    0x2800C8, (s32)&D_800B38D4, 0x1FF0000, 0x7F9, 0x2800F0, (s32)&D_800B38D8, (s32)0xFAFF0000,
    0x7F9, 0x500000, (s32)&D_800B38DC, 0x1060000, 0x7F9, 0x500028, (s32)&D_800B38E0, 0x1FF0000,
    0x7F9, 0x500050, (s32)&D_800B38E4, 0x1FF0000, 0x7F9, 0x500078, (s32)&D_800B38E8, 0x1FF0000,
    0x7F9, 0x5000A0, (s32)&D_800B38EC, 0x1FF0000, 0x7F9, 0x5000C8, (s32)&D_800B38F0, 0x1FF0000,
    0x7F9, 0x5000F0, (s32)&D_800B38F4, (s32)0xFAFF0000, 0x7F9, 0x780000, (s32)&D_800B38F8,
    0x1060000, 0x7F9, 0x780028, (s32)&D_800B38FC, 0x1FF0000, 0x6F9, 0x780050, (s32)&D_800B3900,
    0x1FF0000, 0x5F9, 0x780078, (s32)&D_800B3904, 0x1FF0000, 0x4F9, 0x7800A0, (s32)&D_800B3908,
    0x1FF0000, 0x3F9, 0x7800C8, (s32)&D_800B390C, 0x1FF0004, 0x2F9, 0x7800F0, (s32)&D_800B3910,
    (s32)0xFAFF0003, 0x1F9, 0xA0808C, (s32)&D_800B3894, 1, 0x1FB, 0xC8808C, (s32)&D_800B389C,
    2, 0xFF,
};
s32 D_800BD7B0[12] = {
    0, (s32)&D_800B38A4, 0x7F90000, 0x1FF, 0xC88000, (s32)&D_800B3894, 1, 0, 0xE68000,
    (s32)&D_800B389C, 2, 0,
};
HubEntry12 D_800BD7E0[2] = { { (s32)&D_800BD5D0, 0x1E, 0x1C }, { (s32)&D_800BD7B0, 3, 1 } };
s32 D_800BD7F8[1] = { (s32)&D_800B3928 };
s32 D_800BD7FC[45] = {
    0xE, 0x2C8000, 0, 0, 0, (s32)&D_800BD7F8, 0, 0, 0, 4, 0x2808000, 0x500022, 0, 0, 0, 0, 0,
    0, 4, 0x12C8000, 0x960064, 0, 0, 0, 0, 0, 0, 4, 0x12C8000, 0x1AE00AA, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
s32 D_800BD8B0[153] = {
    3, 0, 0, 0, 0, (s32)&D_800BD5B8, 0, 0, 0, 3, 0x22, 0, 0, 0, (s32)(u8 *)&D_800BD5B8 + 0x2,
    0, 0, 0, 3, 0x44, 0, 0, 0, (s32)&D_800BD5BC, 0, 0, 0, 3, 0x66, 0, 0, 0,
    (s32)(u8 *)&D_800BD5BC + 0x2, 0, 0, 0, 3, 0x88, 0, 0, 0, (s32)&D_800BD5C0, 0, 0, 0, 3,
    0xAA, 0, 0, 0, (s32)(u8 *)&D_800BD5C0 + 0x2, 0, 0, 0, 3, 0xCC, 0, 0, 0, (s32)&D_800BD5C4,
    0, 0, 0, 3, 0xEE, 0, 0, 0, (s32)(u8 *)&D_800BD5C4 + 0x2, 0, 0, 0, 3, 0, 0, 0, 0,
    (s32)&D_800B3934, 0, 0, 0, 3, 0x22, 0, 0, 0, (s32)&D_800B3934, 0, 0, 0, 3, 0x44, 0, 0, 0,
    (s32)&D_800B3934, 0, 0, 0, 3, 0x66, 0, 0, 0, (s32)&D_800B3934, 0, 0, 0, 3, 0x88, 0, 0, 0,
    (s32)&D_800B3934, 0, 0, 0, 3, 0xAA, 0, 0, 0, (s32)&D_800B3934, 0, 0, 0, 3, 0xCC, 0, 0, 0,
    (s32)&D_800B3934, 0, 0, 0, 3, 0xEE, 0, 0, 0, (s32)&D_800B3934, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0,
};
GfxViewport D_800BDB14 = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0, 0, 0, 0xFF0, 0, 0, 0, 0, 1 };
SaveMenuHooks D_800BDB3C = { HubSaveMenuInit, HubSaveMenuUpdate, HubSaveMenuRender, HubSaveMenuQuit };
s32 D_800BDB4C[1] = {0};
s32 D_800BDB50 = 0;
s32 D_800BDB54[1] = {0};
subengine D_800BDB58[33] = {0};
EXTengine D_800BF100 = {0};
s32 D_800BF19C = 0;
s32 D_800BF1A0 = 0;
s32 D_800BF1A4 = 0;
u8 D_800BF1A8[184] = {0};
s32 D_800BF260 = 0;
u8 D_800BF264[32768] = {0};
u8 D_800C7264[48] = {0};
OvlTextLine D_800C7294[264] = {
    { 3, 0, 0x1F4 }, { 0, (void *)&D_800B3938, 0 }, { 1, (void *)&D_800B3948, 0 },
    { 1, (void *)&D_800B3960, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B396C, 0 },
    { 1, (void *)&D_800B3980, 0 }, { 3, 0, 0x96 }, { 0, (void *)&D_800B398C, 0 },
    { 2, (void *)&D_800B3998, 0 }, { 2, (void *)&D_800B39A8, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B39B4, 0 }, { 2, (void *)&D_800B39C0, 0 },
    { 2, (void *)&D_800B39D0, 0 }, { 2, (void *)&D_800B39E0, 0 },
    { 2, (void *)&D_800B39F8, 0 }, { 2, (void *)&D_800B3A04, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3A10, 0 }, { 2, (void *)&D_800B3A1C, 0 },
    { 2, (void *)&D_800B3A28, 0 }, { 2, (void *)&D_800B3A38, 0 },
    { 2, (void *)&D_800B3A48, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3A54, 0 },
    { 2, (void *)&D_800B3A60, 0 }, { 2, (void *)&D_800B3A6C, 0 },
    { 2, (void *)&D_800B3A7C, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3A90, 0 },
    { 2, (void *)&D_800B3AA8, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3AB8, 0 },
    { 2, (void *)&D_800B3ACC, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3ADC, 0 },
    { 2, (void *)&D_800B3AEC, 0 }, { 2, (void *)&D_800B3AFC, 0 },
    { 2, (void *)&D_800B3B0C, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3B18, 0 },
    { 2, (void *)&D_800B3B34, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3B48, 0 },
    { 2, (void *)&D_800B3B5C, 0 }, { 2, (void *)&D_800B3B68, 0 },
    { 2, (void *)&D_800B3B78, 0 }, { 2, (void *)&D_800B3B84, 0 },
    { 2, (void *)&D_800B3B94, 0 }, { 2, (void *)&D_800B3BA8, 0 },
    { 2, (void *)&D_800B3BBC, 0 }, { 2, (void *)&D_800B3BC8, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3BD8, 0 }, { 2, (void *)&D_800B3BE8, 0 },
    { 2, (void *)&D_800B3BF8, 0 }, { 2, (void *)&D_800B3C04, 0 },
    { 2, (void *)&D_800B3C10, 0 }, { 2, (void *)&D_800B3C1C, 0 },
    { 2, (void *)&D_800B3C28, 0 }, { 2, (void *)&D_800B3C34, 0 },
    { 2, (void *)&D_800B3C44, 0 }, { 3, 0, 0x32 }, { 3, 0, 0x96 },
    { 0, (void *)&D_800B3C54, 0 }, { 1, (void *)&D_800B3C60, 0 },
    { 1, (void *)&D_800B3C78, 0 }, { 3, 0, 0x96 }, { 0, (void *)&D_800B3C80, 0 },
    { 2, (void *)&D_800B3C90, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3CA0, 0 },
    { 2, (void *)&D_800B3CB8, 0 }, { 2, (void *)&D_800B3CC8, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3CD8, 0 }, { 2, (void *)&D_800B3CF4, 0 },
    { 2, (void *)&D_800B3D04, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3B48, 0 },
    { 2, (void *)&D_800B3D14, 0 }, { 2, (void *)&D_800B3D20, 0 },
    { 2, (void *)&D_800B3D30, 0 }, { 2, (void *)&D_800B3D3C, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3D4C, 0 }, { 2, (void *)&D_800B3D64, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3D70, 0 }, { 2, (void *)&D_800B3D88, 0 }, { 3, 0, 0x32 },
    { 3, 0, 0x96 }, { 0, (void *)&D_800B3D98, 0 }, { 1, (void *)&D_800B3DB4, 0 },
    { 1, (void *)&D_800B3DC4, 0 }, { 1, (void *)&D_800B3DD4, 0 }, { 3, 0, 0x96 },
    { 0, (void *)&D_800B3AB8, 0 }, { 2, (void *)&D_800B3DDC, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3DEC, 0 }, { 2, (void *)&D_800B3DF8, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3AB8, 0 }, { 2, (void *)&D_800B3E04, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3E14, 0 }, { 2, (void *)&D_800B3E38, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3E48, 0 }, { 2, (void *)&D_800B3E70, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3D4C, 0 }, { 2, (void *)&D_800B3E80, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3E98, 0 }, { 2, (void *)&D_800B3EB4, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3EC0, 0 }, { 2, (void *)&D_800B3EE4, 0 },
    { 2, (void *)&D_800B3EF8, 0 }, { 2, (void *)&D_800B3F08, 0 },
    { 2, (void *)&D_800B3F14, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3F24, 0 },
    { 2, (void *)&D_800B3F34, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B3F44, 0 },
    { 2, (void *)&D_800B3F50, 0 }, { 2, (void *)&D_800B3F60, 0 },
    { 2, (void *)&D_800B3F70, 0 }, { 2, (void *)&D_800B3F80, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3F8C, 0 }, { 2, (void *)&D_800B3FA0, 0 },
    { 2, (void *)&D_800B3FAC, 0 }, { 2, (void *)&D_800B3FBC, 0 },
    { 2, (void *)&D_800B3FD0, 0 }, { 2, (void *)&D_800B3FE4, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3FF8, 0 }, { 2, (void *)&D_800B400C, 0 },
    { 2, (void *)&D_800B401C, 0 }, { 2, (void *)&D_800B402C, 0 },
    { 2, (void *)&D_800B403C, 0 }, { 2, (void *)&D_800B404C, 0 },
    { 2, (void *)&D_800B4058, 0 }, { 2, (void *)&D_800B4064, 0 },
    { 2, (void *)&D_800B4074, 0 }, { 2, (void *)&D_800B4080, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3B48, 0 }, { 0, (void *)&D_800B408C, 0 },
    { 2, (void *)&D_800B4098, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B40A8, 0 },
    { 2, (void *)&D_800B40B4, 0 }, { 2, (void *)&D_800B40C4, 0 },
    { 2, (void *)&D_800B40D0, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B40E0, 0 },
    { 2, (void *)&D_800B40F8, 0 }, { 2, (void *)&D_800B3E04, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B4104, 0 }, { 2, (void *)&D_800B411C, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B4130, 0 }, { 2, (void *)&D_800B4140, 0 },
    { 2, (void *)&D_800B4150, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B415C, 0 },
    { 2, (void *)&D_800B4174, 0 }, { 2, (void *)&D_800B4184, 0 },
    { 2, (void *)&D_800B4194, 0 }, { 3, 0, 0x32 }, { 0, (void *)&D_800B41A4, 0 },
    { 2, (void *)&D_800B41AC, 0 }, { 2, (void *)&D_800B41C0, 0 },
    { 2, (void *)&D_800B41CC, 0 }, { 2, (void *)&D_800B41D8, 0 },
    { 2, (void *)&D_800B41EC, 0 }, { 2, (void *)&D_800B41FC, 0 },
    { 2, (void *)&D_800B4208, 0 }, { 2, (void *)&D_800B4218, 0 },
    { 2, (void *)&D_800B4228, 0 }, { 2, (void *)&D_800B4234, 0 },
    { 2, (void *)&D_800B4244, 0 }, { 2, (void *)&D_800B4258, 0 },
    { 2, (void *)&D_800B4268, 0 }, { 2, (void *)&D_800B4278, 0 },
    { 2, (void *)&D_800B4288, 0 }, { 2, (void *)&D_800B4298, 0 },
    { 2, (void *)&D_800B42A8, 0 }, { 2, (void *)&D_800B42B8, 0 },
    { 2, (void *)&D_800B42C4, 0 }, { 2, (void *)&D_800B42D4, 0 },
    { 2, (void *)&D_800B42E0, 0 }, { 2, (void *)&D_800B42EC, 0 },
    { 2, (void *)&D_800B42F8, 0 }, { 2, (void *)&D_800B4308, 0 },
    { 2, (void *)&D_800B431C, 0 }, { 2, (void *)&D_800B432C, 0 },
    { 2, (void *)&D_800B4338, 0 }, { 2, (void *)&D_800B4348, 0 },
    { 2, (void *)&D_800B4358, 0 }, { 2, (void *)&D_800B4368, 0 },
    { 2, (void *)&D_800B4378, 0 }, { 2, (void *)&D_800B438C, 0 },
    { 2, (void *)&D_800B4398, 0 }, { 2, (void *)&D_800B43A8, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&D_800B3BD8, 0 }, { 2, (void *)&D_800B43B4, 0 },
    { 2, (void *)&D_800B43C0, 0 }, { 2, (void *)&D_800B43CC, 0 },
    { 2, (void *)&D_800B43E0, 0 }, { 2, (void *)&D_800B43EC, 0 },
    { 2, (void *)&D_800B43FC, 0 }, { 2, (void *)&D_800B4408, 0 },
    { 2, (void *)&D_800B4414, 0 }, { 2, (void *)&D_800B4424, 0 },
    { 2, (void *)&D_800B4438, 0 }, { 2, (void *)&D_800B4444, 0 },
    { 2, (void *)&D_800B4454, 0 }, { 2, (void *)&D_800B4460, 0 },
    { 2, (void *)&D_800B446C, 0 }, { 2, (void *)&D_800B447C, 0 },
    { 2, (void *)&D_800B4488, 0 }, { 2, (void *)&D_800B4498, 0 },
    { 2, (void *)&D_800B44A8, 0 }, { 2, (void *)&D_800B44B8, 0 },
    { 2, (void *)&D_800B44C4, 0 }, { 2, (void *)&D_800B44D4, 0 },
    { 2, (void *)&D_800B44E4, 0 }, { 2, (void *)&D_800B44F4, 0 },
    { 2, (void *)&D_800B4500, 0 }, { 2, (void *)&D_800B4510, 0 },
    { 2, (void *)&D_800B451C, 0 }, { 2, (void *)&D_800B4528, 0 },
    { 2, (void *)&D_800B4534, 0 }, { 2, (void *)&D_800B4544, 0 },
    { 2, (void *)&D_800B4554, 0 }, { 2, (void *)&D_800B4560, 0 },
    { 2, (void *)&D_800B456C, 0 }, { 2, (void *)&D_800B457C, 0 },
    { 2, (void *)&D_800B4588, 0 }, { 2, (void *)&D_800B4598, 0 },
    { 2, (void *)&D_800B45A8, 0 }, { 2, (void *)&D_800B45B4, 0 },
    { 2, (void *)&D_800B45C8, 0 }, { 2, (void *)&D_800B45DC, 0 }, { 3, 0, 0x14 },
    { 2, (void *)&D_800B45EC, 0 }, { 2, (void *)&D_800B4610, 0 },
    { 2, (void *)&D_800B4624, 0 }, { 3, 0, 0x14 }, { 2, (void *)&D_800B4638, 0 },
    { 2, (void *)&D_800B465C, 0 }, { 2, (void *)&D_800B4670, 0 },
    { 2, (void *)&D_800B4680, 0 }, { 3, 0, 0x32 }, { 4, 0, 0 },
};
s32 D_800C7EF4 = 0;
s32 D_800C7EF8[3] = {0};
CamXYZ D_800C7F04 = {0};
