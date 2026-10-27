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

    HubHeroPressed = 0;
    D_8009F71C = 0;
    HubHeroTouchCount = 0;
    hero = Heroes;
    World_floor_level = 0;
    eng = hero->hero_eng;
    G6A->unk32 |= 0x200;
    eng->flags |= 0x8000;
    HeroHooks.unk44 = HubHeroTestJump;
    HeroHooks.unk48 = HubHeroTestKick;
    HeroHooks.unk3C = HubHeroTestButton;
    HeroHooks.unk30 = HubHeroSetIdle;
    HubInitHero(0);
    if (G6A->unk34 == 0x22) {
        HubSpawnEffects();
    }
    SetHeroAction(0, 0);
    size = HubHeroSizes[hero->hero_name];
    eng->unk2C.z = size;
    eng->unk2C.y = size;
    eng->unk2C.x = size;
    G6A->unk62 = func_8009020C(G6A->unk34);
    HubInitGateways();
    func_80095258(*(u8 **)MapScene);
    HubInitLevelSelect();
    HubDoorFade = 0;
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
    HubSelectSfx = 0;
    func_8001E934(&MenuProcessControl, &HubDoorMenu);
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char HubLockMsg00[] = "COMPLETE ALL CHALLENGES\nTO UNLOCK THIS LEVEL";
const char HubLockMsg01[] = "YOU NEED 4 TROPHIES\nTO FACE PAPU PAPU";
const char HubLockMsg02[] = "YOU NEED 9 TROPHIES, 6 GEMS\nAND 3 CRYSTALS TO FACE THE BEARMINATOR";
const char HubLockMsg03[] = "YOU NEED 15 TROPHIES, 10 GEMS\nAND 7 CRYSTALS TO FACE THE TANK BOSS";
const char HubLockMsg04[] = "YOU NEED 22 TROPHIES, 15 GEMS\nAND 12 CRYSTALS TO FACE N. OXIDE";
const char HubLockMsg05[] = "YOU NEED 19 CRYSTALS\nTO OPEN DRAGON DROP";
const char HubLockMsg06[] = "YOU NEED 21 GOLD RELICS\nTO OPEN MALLET MASH";
const char HubLockMsg07[] = "YOU NEED 25 GEMS\nTO OPEN KEG KABOOM";
const char HubLockMsg08[] = "YOU NEED 27 TROPHIES\nTO OPEN DANTE'S DASH";
const char HubLockMsg09[] = "YOU NEED 17 GOLD RELICS\nTO OPEN SPLASH DASH";
const char HubLockMsg10[] = "YOU NEED 23 CRYSTALS\nTO OPEN SWAMP FOX";
const char HubTextPressSave[] = "PRESS \374 BUTTON TO SAVE";
const char HubTextPressStart[] = "PRESS \374 BUTTON TO START";
const char HubTextSelectChallenge[] = "PRESS LEFT - RIGHT TO SELECT CHALLENGE";
const char HubTextSelectWarpRoom[] = "PRESS LEFT - RIGHT TO SELECT WARP ROOM";
const char HubTextPressEnter[] = "PRESS \374 BUTTON TO ENTER";
const char HubTextDefeatPapu[] = "YOU MUST DEFEAT PAPU PAPU BEFORE\nYOU CAN GO TO THE NEXT WARP ROOM";

void HubInitGateways(void) {
    s32 i;
    EngRec90248 *p;
    u8 *o;
    u8 *obj;
    EngSub90248 *e;
    Vec3W ref;
    Vec3W d;

    for (i = 0; i < 12; i++) {
        HubGateways[i].unk0 = NULL;
        HubGateways[i].unk4 = NULL;
        HubGateways[i].unk18 = NULL;
        HubGateways[i].unk8 = 0;
        HubGateways[i].unkC = 0;
        HubGateways[i].unk10 = 0;
        HubGateways[i].unk14 = NULL;
        HubGateways[i].unk1C = 0;
        HubGateways[i].unk20 = 0;
        HubGateways[i].unk24 = 0;
    }
    o = *(u8 **)((u8 *)MapScene + 0x1C);
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
                HubGateways[i].unk0 = (s32 *)o;
                break;
            case 2:
                HubGateways[i].unk4 = (s32 *)o;
                break;
            case 3:
                break;
            case 5:
                break;
            case 4:
                q = &HubGateways[i];
                q->unk18 = o;
                k = *(s32 *)o;
                g = MapScene;
                *(void **)(o + 0x54) = HubDrawObj;
                *(s32 *)o = k & ~0x8000;
                q->unk24 = func_80015984(*(u16 *)(o + 0x74), *(EntryBlock **)(*(u8 **)g + 0xC));
                break;
            }
        }
        o = *(u8 **)(o + 0x5C);
    }
    for (e = (EngSub90248 *)WarpRooms[GameWarpRoom][1]; e->unk0 != 0; e++) {
        i = e->unk0 - 1;
        p = &HubGateways[i];
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
        p = &HubGateways[i];
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

    for (i = 0, one = 1, p = HubGateways; i < 12; i++, p++) {
        q = &p->unk24;
        if (q[-4] != 0) {
            o = (s32 *)q[-3];
            if (o != NULL) {
                s32 v = TrigTable[(((GameFrameCount * 0x50) & 0xFFF) - 0x400) & 0xFFF].unk2;
                o[2] = v * 0x20 >> 12;
            }
            if (q[-2] == one) {
                if (q[0] == 0 || (func_800163E0(*(u8 **)MapScene, q[0]), q[0] == 0) ||
                    func_80016450(*(u8 **)MapScene, q[0]) != 0) {
                    if (q[-1] == one) {
                        if (o != NULL) {
                            o[0] |= 0x8000;
                        }
                        HubDoorFade += 0x100;
                        if (HubDoorFade >= 0x1000) {
                            HubDoorFade = 0x1000;
                        }
                    } else {
                        HubDoorFade -= 0x100;
                        if (HubDoorFade < 0) {
                            HubDoorFade = 0;
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
                    func_8001636C(*(u8 **)MapScene, q[0]);
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
    speed = HeroHooks.unkC[Heroes[arg0].hero_name * 3 + 1];
    rate = (u32)(D_8009EEA4 * 0x648) >> 8;
    if (rate != 0) {
        step = (s32)(speed << 12) / (s32)rate;
    } else {
        step = 0;
    }
    stop = 0;
    ang = eng->unk1E;
    if (((HubGameView *)GameInfo)->unk74 != 0) {
        eng->flags &= ~0x8000;
        if (Heroes[0].hero_shadow != NULL) {
            Heroes[0].hero_shadow->flags &= ~0x8000;
        }
        return;
    }
    mv->forward_speed = 0;
    HubHeroTouchCount = 0;
    if (((HubGameView *)GameInfo)->unk30 & 0x8008) {
        return;
    }
    if (Heroes[arg0].hero_frozen != 0) {
        return;
    }
    if (((HubGameView *)GameInfo)->unk63 == -1) {
        if (((HubPad *)Pads)[arg0].unk50 & 0x10) {
            mv->forward_speed = speed;
            ang = D_8009EE9C.unk4 + 0x800;
            D_8009EE9C.unk8 += speed;
            if ((u32)D_8009EE9C.unk10 < (u32)D_8009EE9C.unk8) {
                best = -1;
                bestd = 0x1000;
                for (i = 0; i < 12; i++) {
                    if (HubGateways[i].unk14 != NULL && (HubGateways[i].unk14->unk6 & 2)) {
                        d = func_80015480(HubGateways[i].unk10, D_8009EE9C.unk4);
                        if (d < bestd) {
                            bestd = d;
                            if ((u32)d < 0x81) {
                                best = i;
                            }
                        }
                    }
                }
                if (best != -1) {
                    D_8009EE9C.unk4 = HubGateways[best].unk10;
                    ((HubGameView *)GameInfo)->unk63 = best;
                    HubGateways[best].unk1C = 1;
                } else {
                    D_8009EE9C.unk8 = D_8009EE9C.unk10;
                    stop++;
                    mv->forward_speed = 0;
                }
            }
        }
        if (((HubPad *)Pads)[arg0].unk50 & 0x40) {
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
            if (((HubPad *)Pads)[arg0].unk50 & 0x80) {
                mv->forward_speed = speed;
                ang = 0x400;
                ang = (D_8009EE9C.unk4 -= step % 0x1000) + ang;
            }
            if (((HubPad *)Pads)[arg0].unk50 & 0x20) {
                mv->forward_speed = speed;
                ang = (D_8009EE9C.unk4 += step) - 0x400;
            }
            D_8009EE9C.unk4 &= 0xFFF;
        }
    } else {
        if (((HubPad *)Pads)[arg0].unk50 & 0x10) {
            mv->forward_speed = speed;
            t = D_8009EE9C.unk8 + speed;
            D_8009EE9C.unk8 = t;
            lim = HubGateways[((HubGameView *)GameInfo)->unk63].unkC;
            ang = D_8009EE9C.unk4 + 0x800;
            if ((u32)lim < (u32)t) {
                D_8009EE9C.unk8 = lim;
                mv->forward_speed = 0;
            }
        }
        if (((HubPad *)Pads)[arg0].unk50 & 0x40) {
            ang = D_8009EE9C.unk4;
            mv->forward_speed = speed;
            D_8009EE9C.unk8 -= speed;
            if ((u32)D_8009EE9C.unk8 < (u32)D_8009EE9C.unk10) {
                GameGateway = -1;
            }
        }
        if ((u8)GameGateway >= 12) {
            goto done;
        }
        old = HubGateways[GameGateway].unk20;
        if ((u32)(HubGateways[GameGateway].unkC - D_8009EEA4) < 0x100) {
            HubGateways[GameGateway].unk20 = 1;
        } else {
            HubGateways[GameGateway].unk20 = 0;
        }
        if (HubGateways[GameGateway].unk20 != old) {
            if (HubGateways[GameGateway].unk20 != 0) {
                switch (((s32(*)(void))HubGatewayKind)()) {
                case 2:
                    PlaySfx(0x399, NULL, NULL, 0x1000, 0x1E00, 0);
                    break;
                case 5:
                    PlaySfx(((s32(*)(void))func_800900CC)() >= 0 ? 0x39E : 0x39D, &HubSelectSfx, NULL, 0x1000, 0x1E00, 0);
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
                    if (HubSelectSfx != NULL) {
                        StopSfx(HubSelectSfx);
                    }
                case 4:
                    PlaySfx(0x194, NULL, NULL, 0x1000, 0x1E00, 0);
                    break;
                }
            }
        }
        if (!(((HubPad *)Pads)[arg0].unk54 & 0x4000)) {
            goto done;
        }
        p = HubGateways;
        g = (HubGameView *)GameInfo;
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
            if ((g->unk4 & 0x2000) || ((g->unkE4[sel] != 0 || HubDoorChoice <= 0) && ((s32(*)(s32))func_8007A6D0)(sel) == 1)) {
            mode = HubDoorChoice;
            if (mode != 4 || ((HubGameView *)GameInfo)->unk105[sel] != 0) {
            gv = (HubGameView *)GameInfo;
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
                ((HubGameView *)GameInfo)->unk32 |= 0x10;
                break;
            }
            HubSpawnFlash(&Heroes[0].hero_eng->unk10.x, 9);
            HubGoToMode(HubGateways[GameGateway].unk14->unk4);
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
            func_80092AF4(&SaveScreenProcess, 0x200);
            break;
        default:
            HubSpawnFlash(&Heroes[0].hero_eng->unk10.x, 9);
            break;
        }
    }
done:
    for (j = 11; j >= 0; j--) {
        HubGateways[j].unk1C = 0;
    }
    if (GameGateway != -1) {
        HubGateways[GameGateway].unk1C = 1;
    }
    mv->forward_angle = ang;
    if (Heroes[arg0].hero_aseq != 10 && HubHeroPressed != 0) {
        HubHeroPressed = 0;
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

    HubDoorMenuRecs[5].unk1E = 4;
    HubDoorMenuRecs[6].unk1E = 4;
    if (((HubGameView *)GameInfo)->unk105[arg0] != 0) {
        HubDoorMenuRecs[5].unk1E = 3;
        if (((HubGameView *)GameInfo)->unk105[arg0] >= 2) {
            HubDoorMenuRecs[6].unk1E = 2;
        }
    }
    n = ((HubGameView *)GameInfo)->unkE0;
    if (n >= 2 && ((HubGameView *)GameInfo)->unk105[arg0] == 0) {
        HubDoorChoices = 4;
    } else {
        HubDoorChoices = n + 3;
    }
    if (((HubGameView *)GameInfo)->unk4 & 0x2000) {
        HubDoorChoices = ((HubGameView *)GameInfo)->unkE0 + 3;
    }
    HubDoorMenuRecs[4].unk1E = ((HubGameView *)GameInfo)->unk147[arg0] ? 7 : 8;
    HubDoorMenuRecs[3].unk1E = ((HubGameView *)GameInfo)->unk126[arg0] ? 5 : 6;
    e = GetEngineSlot(1);
    func_80095BEC(1, (s32 *)HubDoorMenuRecs);
    e->unk10.y = 200;
    e->unk10.x = 0;
    HubDoorChoice = 0;
    e->flags = (e->flags & ~0x8000) | 0x10000000;
    if (((HubGameView *)GameInfo)->unkE4[arg0] != 0) {
        HubDoorChoice = 1;
    }
    if (((HubGameView *)GameInfo)->unk126[arg0] != 0) {
        HubDoorChoice = 2;
    }
    if (((HubGameView *)GameInfo)->unk147[arg0] != 0 && ((HubGameView *)GameInfo)->unkE0 > 0) {
        HubDoorChoice = 3;
    }
    if (((HubGameView *)GameInfo)->unk105[arg0] != 0 && ((HubGameView *)GameInfo)->unkE0 >= 2) {
        HubDoorChoice = 4;
        n = 1;
    } else {
        n = 1;
    }
    p = e->sub;
    w = (((HubDoorChoices << n) + HubDoorChoices) * 15) << n;
    p[1].unk76 = w;
    e->unk10.x = 320 - (w >> n);
    sub = GetEngineSlot(n)->sub;
    p = &sub[5];
    switch (HubDoorChoices - 3) {
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
    HubGameView *g = (HubGameView *)GameInfo;
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

    if (g->unk63 < 0 || HubDoorFade <= 0
        || (rec = HubGateways[g->unk63].unk14, rec->unk2 != 2)
        || (!(g->unk4 & 0x2000) && g->unkE4[rec->unk4] == 0)) {
        if (HubDoorChoice >= 0) {
            HubDoorChoice = -1;
            func_80095A04(1);
            func_80095A04(2);
        }
    } else {
        if (HubDoorChoice == -1) {
            HubOpenDoorMenu(HubGateways[GameGateway].unk14->unk4);
        }
        sub = GetEngineSlot(1)->sub;
        if ((Pads[0].pressed & 0x80) && GameExitFlags != 0x8008 && HubDoorChoice > 0) {
            PlaySfx(0x385, NULL, NULL, 0x1000, 0x1E00, 0);
            HubDoorChoice--;
        }
        if ((Pads[0].pressed & 0x20) && GameExitFlags != 0x8008 && HubDoorChoice < HubDoorChoices - 1) {
            PlaySfx(0x385, NULL, NULL, 0x1000, 0x1E00, 0);
            HubDoorChoice++;
        }
        v = func_80015284(*(s16 *)&sub->unk70, HubDoorAngles[HubDoorChoice], 2000);
        *(s16 *)&sub->unk70 = v;
        sub->unk76 = v + 90;
    }
    if (HubDoorFade != 0) {
        switch (((s32(*)(void))HubGatewayKind)()) {
        case 2:
            if (!(GetEngineSlot(2)->flags & 0x8000)) {
                n = HubGateways[((HubGameView *)GameInfo)->unk63].unk14->unk4;
                g2 = (HubGameView *)GameInfo;
                if (((s32(*)(s32))func_8007A6D0)(n) == 1) {
                    if (g2->unkE4[n] != 0) {
                        func_80095BEC(2, HubChallengePanels[g2->unk2C]);
                    } else {
                        func_80095BEC(2, HubStartPanels[g2->unk2C]);
                    }
                } else {
                    key = func_80090248();
                    p = HubLockPanels;
                    for (i = 10; i != -1; i--) {
                        if (p->key == key) {
                            recs = (s32 *)p->val[GameLanguage];
                            goto found;
                        }
                        p++;
                    }
                    recs = NULL;
                found:
                    func_80095BEC(2, recs);
                }
            }
            GetEngineSlot(2)->unk10.y = (HubDoorFade >> 4) - 200;
            break;
        case 5:
            if (!(GetEngineSlot(2)->flags & 0x8000)) {
                g3 = (HubGameView *)GameInfo;
                if (g3->unk34 == 0x21 && ((s32(*)(s32))func_8007A6D0)(0x22) == 0) {
                    recs = HubPapuPanels[g3->unk2C];
                } else {
                    recs = HubWarpPanels[g3->unk2C];
                }
                func_80095BEC(2, recs);
            }
            GetEngineSlot(2)->unk10.y = 0x280 - (HubDoorFade >> 4);
            break;
        case 4:
            if (!(GetEngineSlot(2)->flags & 0x8000)) {
                func_80095BEC(2, HubSavePanels[GameLanguage]);
            }
            GetEngineSlot(2)->unk10.y = (HubDoorFade >> 4) - 200;
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

    if (GameGateway >= 0) {
        v = HubGateways[GameGateway].unk14->unk2;
    } else {
        v = 0;
    }
    if (v == k && HubGateways[GameGateway].unk20 != 0) {
        r = 1;
    }
    if (r != 0) {
        s32 (*base)[5];
        r = HubSelectedEntry;
        if (r < 0) {
            return -1;
        }
        base = WarpRooms;
        t = base[GameWarpRoom];
        if (r < t[2]) {
            return ((s32 *)(t[3] + r * 8))[1];
        }
        return -1;
    }
    return -1;
}

void HubLevelSelect(void) {
    s32 (*base)[5] = WarpRooms;
    HubSelView *g = (HubSelView *)GameInfo;
    HubSelEntry *ents = (HubSelEntry *)base[g->unk62][3];
    s32 r;
    s32 v;
    s32 k;
    s32 n;
    fade_info *p;

    switch (HubSelectState) {
    case 0:
        r = 0;
        k = 5;
        if (g->unk63 >= 0) {
            v = HubGateways[g->unk63].unk14->unk2;
        } else {
            v = 0;
        }
        if (v == k && HubGateways[GameGateway].unk20 != 0) {
            r = 1;
        }
        if (r != 0 && ((HubSelView *)GameInfo)->unk30 != 0x8008) {
            if ((Pads[0].pressed & 0x20)
                && HubSelectedEntry < WarpRooms[((HubSelView *)GameInfo)->unk62][2] - 1
                && ((s32(*)(s32))func_8007A6D0)(ents[HubSelectedEntry + 1].unk4)) {
                PlaySfx(0x39F, NULL, NULL, 0x1000, 0x1E00, 0);
                HubSelectedEntry++;
            }
            if ((Pads[0].pressed & 0x80) && HubSelectedEntry > 0) {
                PlaySfx(0x39F, NULL, NULL, 0x1000, 0x1E00, 0);
                HubSelectedEntry--;
            }
            if (HubSelectedEntry < 0) {
                HubSelectedEntry = func_800900CC();
            }
            if (HubSelectedEntry >= WarpRooms[GameWarpRoom][2]) {
                n = WarpRooms[GameWarpRoom][2];
                HubSelectedEntry = n - 1;
            }
            if (HubSelectSub != NULL) {
                if (HubSelectedEntry >= 0 && ((s32(*)(s32))func_8007A6D0)(ents[HubSelectedEntry].unk4)) {
                    HubSelectSub->unk74 = ents[HubSelectedEntry].unk0;
                    return;
                }
                HubSelectSub->unk74 = WarpRooms[GameWarpRoom][4];
            }
        } else if (HubSelectSub != NULL) {
            HubSelectSub->unk74 = WarpRooms[GameWarpRoom][4];
        }
        break;
    case 1:
        if (func_80012FFC() == 0) {
            set_fade(&ScreenFade, 0x200, 0);
            HubSelectState = 2;
        }
        break;
    case 2:
        p = &ScreenFade;
        if (p->maxed >= 3) {
            func_8001DDD8(MapScene);
            func_8001E054(MapScene, HubSelectTimer);
            g->unk62 = HubSelectFlag;
            g->unk34 = base[g->unk62][0];
            InitialiseHUB();
            HubSelectState = 0;
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
    if (*(s32 *)MapScene != 0) {
        tbl = *(s32 **)((u8 *)MapScene + 0x28);
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
                    s32 *q = (s32 *)(*(u8 **)((u8 *)MapScene + 0x2C) + i * 4);
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
const char HubTextHaveTrophy[] = "YOU ALREADY HAVE A TROPHY\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char HubTextHaveGem[] = "YOU ALREADY HAVE A GEM\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char HubTextHaveCrystal[] = "YOU ALREADY HAVE A CRYSTAL\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char HubTextHaveRelic[] = "YOU ALREADY HAVE ARELIC\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char HubTextHavePlatinum[] = "YOU ALREADY HAVE A PLATINUM RELIC\nARE YOU SURE YOU WANT TO PLAY AGAIN\nPRESS X BUTTON TO START";
const char HubTextEmpty[] = "";

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
        ext->cursor = func_80024E30(nas, &HubPathScriptInit);
        nas->Engine->sub->unk6C = *(void **)MapScene;
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
    arg0->mid->inner->unk6C = **(s32 **)&MapScene;
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
    HubSelectSub = func_8001DCD4(1, 0x14);
    HubSelectState = 0;
    HubSelectedEntry = -1;
}

s32 HubGatewayIsKind(s32 arg0) {
    s32 r = 0;
    s32 v;

    if (GameGateway >= 0) {
        v = HubGateways[GameGateway].unk14->unk2;
    } else {
        v = 0;
    }
    if (v == arg0 && HubGateways[GameGateway].unk20 != 0) {
        r = 1;
    }
    return r;
}

u16 HubGatewayKind(void) {
    if (GameGateway >= 0) {
        return HubGateways[GameGateway].unk14->unk2;
    }
    return 0;
}

void HubDoorMenu2Quit(void) {
    func_80095A04(1);
    func_80095A04(2);
}

void HubDoorMenu2Render(void) {
    EXTengine *e;

    func_80018B08(&MainViewport, &MainGfxCamera);
    RenderState.modelDp = ScreenFade.amount;
    if (HubDoorChoice >= 0) {
        if (!(GamePause & 1)) {
            e = GetEngineSlot(1);
            e->flags |= 0x8000;
            e->unk10.y = 0x1D6 - (HubDoorFade >> 5);
            func_8001C2C0((u8 *)GetEngineSlot(1));
        }
    }
    if (HubDoorFade != 0 && !(GamePause & 1)) {
        func_8001C2C0((u8 *)GetEngineSlot(2));
    }
}

void HubDoorMenu2Update(void) {
    HubDoorMenuUpdate();
}

void HubDoorMenu2Init(void) {
    HubDoorChoice = -1;
}

void HubDoorMenuRender(void) {
    EXTengine *e;

    func_80018B08(&MainViewport, &MainGfxCamera);
    RenderState.modelDp = ScreenFade.amount;
    if (HubDoorChoice >= 0) {
        if (!(GamePause & 1)) {
            e = GetEngineSlot(1);
            e->flags |= 0x8000;
            e->unk10.y = 0x1D6 - (HubDoorFade >> 5);
            func_8001C2C0((u8 *)GetEngineSlot(1));
        }
    }
    if (HubDoorFade != 0 && !(GamePause & 1)) {
        func_8001C2C0((u8 *)GetEngineSlot(2));
    }
}

void HubDoorMenuInit(void) {
    HubDoorChoice = -1;
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

    p = AdventureItemsModel;
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
    NextGameMode = arg0;
    p = GameInfo;
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
    if (HubHeroTouchCount != 0) {
        n = 3;
    }
    HubCamFromPath((u8 *)&v, n);
    v.at = *(CamXYZ *)&Heroes[0].hero_eng->unk10;
    v.rot.vy = -D_8009EEA0;
    if (arg1 != 0) {
        func_80013F44(cam, &v, NULL);
    } else {
        func_80013F44(cam, &v, &HubCamEase);
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
    if (((PadButtons *)Pads)[arg0].unk54 & 0x8000) {
        if ((u32)(Heroes[arg0].hero_name - 1) >= 2U) {
            if (Heroes[arg0].hero_frozen == 0) {
                Heroes[arg0].hero_frozen = 0xF;
            }
        } else {
            HubHeroPressed = 1;
        }
    }
    return 0;
}

s32 HubHeroTestJump(void) {
    return 0;
}

s32 HubHeroTestKick(void) {
    return 0;
}

void HubHeroTouched(s32 arg0) {
    if (Heroes[arg0].hero->UserInfo.user1 != 0) {
        HubHeroTouchCount += 1;
        return;
    }
    HubHeroTouchCount = 0;
}

void HubCamTarget(void) {
    volatile s32 v[2];

    s32 a, b;

    a = TrigTable[(D_8009EE9C.unk4 - 0x400) & 0xFFF].unk2 * D_8009EE9C.unk8;
    v[0] = TrigTable[(D_8009EE9C.unk4 - 0x400) & 0xFFF].unk2;
    b = TrigTable[D_8009EE9C.unk4 & 0xFFF].unk2 * D_8009EE9C.unk8;
    v[1] = TrigTable[D_8009EE9C.unk4 & 0xFFF].unk2;
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
        s->sub->unk6C = AdventureItemsModel;
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
        p = &HubGateways[i];
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

    o = *(u8 **)((u8 *)MapScene + 0x1C);
    cam = MainCamera;
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
    ((HubGameView *)GameInfo)->unk51 = 9;
    ((HubGameView *)GameInfo)->unk52 = 9;
    ((HubGameView *)GameInfo)->unk53 = 9;
    HeroHooks.unk0 = (s32)HubInitHero;
    HeroHooks.unk20 = (void *)HubPreMoveHero;
    HeroHooks.unk34 = (void *)HubHeroTouched;
    ((HubGameView *)GameInfo)->unk50 = ((HubGameView *)GameInfo)->unk4C;
    ((HubGameView *)GameInfo)->unk32 |= 0x200;
}

s32 HubLookup(s32 arg0) {
    KeyedPair *p = HubLockPanels;
    s32 i = 10;
    u8 *g = GameInfo;

    for (; i != -1; i--) {
        if (p->key == arg0) {
            return p->val[*(s32 *)(g + 0x2C)];
        }
        p++;
    }
    return 0;
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char PodiumAnimWinBreathe[] = "WIN_BREATHE";
const char PodiumAnimWin[] = "WIN";
const char PodiumAnimLoseBreathe[] = "LOSE_BREATHE";
const char PodiumAnimLose[] = "LOSE";

void InitialisePodiumFirst(void) {
    s32 i;

    PodiumTrophyObj = NULL;
    for (i = 0; i < 4; i++) {
        PodiumHeroNas[i] = 0;
        switch (GameInfo[i + 0x4C]) {
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
    ((HubGameView *)GameInfo)->unk50 = 9;
    ((HubGameView *)GameInfo)->unk51 = 9;
    ((HubGameView *)GameInfo)->unk52 = 9;
    ((HubGameView *)GameInfo)->unk53 = 9;
    PodiumState = 0;
    ((HubGameView *)GameInfo)->unk32 |= 0x200;
    PodiumRankPlayers();
    PodiumFindTies();
    *(s32 *)&D_8005A5E8[((HubGameView *)GameInfo)->unk8] = 0xCD;
    ((HubGameView *)GameInfo)->unk3C = 0xCD;
}

void InitialisePodium(void) {
    s32 i;
    s32 k;
    s32 j;
    s32 name;
    s32 h;

    World_floor_level = 0;
    CameraMode = 4;
    PodiumReady = 0;
    PodiumCheerTimer = 0x5A;
    for (i = 0; i < 4; i++) {
        h = GameInfo[i + 0xA7];
        name = GameInfo[h + 0x4C];
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
        for (j = 0; (&PodiumAnimName)[j] != NULL;) {
            func_800165F8(HeroModelBank + k * 0x38, (&PodiumAnimName)[j++]);
        }
        PodiumSpawnHero(i, &((HubSpawnEnt (*)[4])PodiumSpawns)[PodiumTies][i]);
        PodiumInitScript(i, name);
    }
    PodiumShowScores();
    func_80095258(*(u8 **)MapScene);
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
                if (GameInfo[0x9F + j] > GameInfo[0x9F + best] || best == -1) {
                    best = j;
                }
            }
        }
        GameInfo[0xA7 + i] = best;
        mask |= 1 << best;
    }
    for (i = 0; i < 4; i++) {
        s32 k;
        k = 4;
        q = GameInfo + i;
        if (i == 0) {
            PodiumRanks[0] = k;
        } else {
            if (GameInfo[q[0xA7] + 0x9F] == GameInfo[(p = GameInfo + (j = i - 1))[0xA7] + 0x9F]) {
                PodiumRanks[i] = PodiumRanks[i - 1];
            } else {
                PodiumRanks[i] = PodiumRanks[j] - 1;
            }
        }
    }
}

void PodiumFindTies(void) {
    u8 *g = GameInfo;
    s32 a = g[g[0xA7] + 0x9F];
    s32 b = g[g[0xA8] + 0x9F];
    s32 c = g[g[0xA9] + 0x9F];
    s32 d = g[g[0xAA] + 0x9F];

    PodiumTies = 0;
    if (a == d) {
        PodiumTies = 7;
        return;
    }
    if (b == d) {
        PodiumTies = 5;
        return;
    }
    if (a == c) {
        PodiumTies = 6;
        return;
    }
    if (c == d && a != b) {
        PodiumTies = 1;
        return;
    }
    if (b == c) {
        PodiumTies = 2;
        return;
    }
    if (a == b) {
        if (c != d) {
            PodiumTies = 3;
            return;
        }
        PodiumTies = 4;
    }
}

void HubCreditsEffects(void) {
    s32 r;
    s32 x;

    if (GamePause == 0 && PodiumCheerTimer-- < 0) {
        PodiumCheerKind = Random(4);
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
        PodiumCheerTimer = Random(10) + 5;
    }
}

void PodiumCheer(void) {
    s32 r;
    s32 x;

    switch (GameInfo[GameInfo[0xA7] + 0x4C]) {
    case 3:
    case 4:
    case 6:
    case 8:
        if (GamePause == 0 && PodiumCheerTimer-- < 0) {
            PodiumCheerKind = Random(2) + 2;
            goto spawn;
        }
        break;
    default:
        if (GamePause == 0 && PodiumCheerTimer-- < 0) {
            PodiumCheerKind = Random(2);
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
            PodiumCheerTimer = Random(0x14) + 0xA;
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

    obj = (PodiumScriptObj *)PodiumHeroNas[arg0];
    if (obj != NULL) {
        mid = obj->mid;
        if (mid != NULL) {
            switch (arg1) {
            case 1:
                mid->inner->script = D_80055924;
                func_80024E30((NAS *)obj, &PodiumScript2);
                break;
            case 2:
                mid->inner->script = D_8005595C;
                func_80024E30((NAS *)obj, &PodiumScript1);
                break;
            case 3:
                mid->inner->script = D_80055994;
                func_80024E30((NAS *)obj, &PodiumScript3);
                break;
            case 4:
                mid->inner->script = D_800559CC;
                func_80024E30((NAS *)obj, &PodiumScript0);
                break;
            case 5:
                mid->inner->script = D_80055A04;
                func_80024E30((NAS *)obj, &PodiumScript6);
                break;
            case 6:
                mid->inner->script = D_80055A3C;
                func_80024E30((NAS *)obj, &PodiumScript7);
                break;
            case 7:
                mid->inner->script = D_80055A74;
                func_80024E30((NAS *)obj, &PodiumScript4);
                break;
            case 8:
                mid->inner->script = D_80055AAC;
                func_80024E30((NAS *)obj, &PodiumScript5);
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
            tb = TrigTable;
            t = FrameCount;
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
        PodiumState = 0;
        switch (GameInfo[GameInfo[0xA7] + 0x4C]) {
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
    if (PodiumState < 2) {
        id = 1;
    }
    o = (u8 *)path_get(id);
    if (arg1 != 0) {
        t = *(s32 *)(o + 0x58);
        PodiumCamAt = *(CamXYZ *)(o + 0x30);
        PodiumCamTimer = t;
    }
    func_8001E21C((KeyBlock *)o, PodiumCamTimer, &key);
    PodiumCamAt.x += (*(s32 *)(o + 0x30) - PodiumCamAt.x) >> 4;
    PodiumCamAt.y += (*(s32 *)(o + 0x34) - PodiumCamAt.y) >> 4;
    PodiumCamAt.z += (*(s32 *)(o + 0x38) - PodiumCamAt.z) >> 4;
    out->at = PodiumCamAt;
    d.x = PodiumCamAt.x - key.x;
    d.y = PodiumCamAt.y - key.y;
    d.z = PodiumCamAt.z - key.z;
    out->eye.z = csqrt(d.x * d.x + d.y * d.y + d.z * d.z) >> 6;
    out->eye.x = 0;
    out->eye.y = 0;
    out->rot.vy = get_angle(-d.x, d.z);
    out->rot.vx = get_angle(d.y, csqrt(d.x * d.x + d.z * d.z) >> 6);
    out->rot.vz = 0;
    switch (PodiumState) {
    case 0:
        end = *(s32 *)(o + 4);
        PodiumCamTimer += 0x20;
        if (PodiumCamTimer >= end) {
            PodiumCamTimer = end;
            PodiumCamState = 0x3C;
            PodiumState = 1;
        }
        break;
    case 1:
        if (--PodiumCamState <= 0) {
            PodiumState = 2;
            PodiumCamTimer = *(s32 *)((u8 *)path_get(2) + 0x58);
            if (PodiumTrophyObj != NULL) {
                c = PodiumTrophyObj->child;
                if (c != NULL && PodiumTrophyObj->f5C == 0) {
                    PodiumTrophyObj->f5C = 1;
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
        PodiumCamTimer += 0x20;
        if (PodiumCamTimer >= end) {
            PodiumCamState = 0xF0;
            PodiumCamTimer = end;
            PodiumState = 3;
            PodiumReady = 1;
        }
        break;
    case 3:
        PodiumScoresDown();
        if (--PodiumCamState <= 0) {
            PodiumState = 4;
        }
        break;
    case 4:
        PodiumState = 5;
        PodiumCamState = 0x3C;
        break;
    case 5:
        PodiumScoresUp();
        if (--PodiumCamState <= 0) {
            PodiumState = 0x64;
        }
        break;
    case 0x64:
        break;
    }
    out->dist = 0x200;
    GameViewport.ofy = 0x15E;
}

void PodiumShowScores(void) {
    EXTengine *e;
    s32 n;

    e = GetEngineSlot(6);
    if (e != NULL) {
        n = GameInfo[0x9F];
        PodiumScore1[1] = n % 10 + 0x30;
        PodiumScore1[0] = n / 10 % 10 + 0x30;
        n = GameInfo[0xA0];
        PodiumScore2[1] = n % 10 + 0x30;
        PodiumScore2[0] = n / 10 % 10 + 0x30;
        n = GameInfo[0xA1];
        PodiumScore3[1] = n % 10 + 0x30;
        PodiumScore3[0] = n / 10 % 10 + 0x30;
        n = GameInfo[0xA2];
        PodiumScore4[1] = n % 10 + 0x30;
        PodiumScore4[0] = n / 10 % 10 + 0x30;
        func_80095BEC(6, &PodiumScoreCount);
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
    if (func_80018F3C(arg0->Engine, 12) != 12 || func_80015D70((u8 *)&HubSparkFxDef, (u8 *)arg0->Engine->sub) == 0) {
        KillNAS(arg0);
        return;
    }
    s = arg0->Engine->sub;
    s = s->next;
    if (s != NULL) {
        u32 fl;
        do {
            s->unk51 = 0;
            nv = AdventureItemsModel;
            s->unk50 = 1;
            s->unk6C = nv;
            if (PodiumCheerKind == 0) {
                s->unk74 = 0x2021;
            }
            if (PodiumCheerKind == 1) {
                s->unk74 = 0x2025;
            }
            if (PodiumCheerKind == 2) {
                s->unk74 = 0x2022;
            }
            if (PodiumCheerKind == 3) {
                s->unk74 = 0x2024;
            }
            if (PodiumCheerKind == 4) {
                s->unk74 = 0x2027;
            }
            if (PodiumCheerKind == 5) {
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
    g = AdventureItemsModel;
    sel = PodiumCheerKind;
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
        a = &TrigTable[ang & 0xFFF];
        for (;;) {
            s32 r;

            if (i >= 8) {
                break;
            }
            row->rot[i].vx = TrigTable[(ang - 0x400) & 0xFFF].unk2 * 64 >> 12;
            row->rot[i].vy = (a->unk2 * 64 >> 12) - 0x400;
            row->rot[i].vz = 0x2000;
            row->unk120 = (a->unk2 * 256 >> 12) + Random(0x40) - Random(0x40);
            row->unk12C = (TrigTable[(ang - 0x400) & 0xFFF].unk2 >> k) + Random(0x40) - Random(0x40);
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
    set = (HubSparkSet *)((u8 *)set + (FrameCount & 1) * 4);
    prim = set->src;
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
    arg0 = arg0->next;
    while (1) {
        if (arg0 == NULL) {
            break;
        }
        if (GamePause == 0) {
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
                v.vx = row->rot[i].vx + (r * TrigTable[row->unkE0[i].vz & 0xFFF].unk2 >> 12);
                v.vy = row->rot[i].vy + (r * TrigTable[(row->unkE0[i].vz + 0x400) & 0xFFF].unk2 >> 12);
                v.vz = row->rot[i].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                gte_stsxy(&prim->x3);
                v.vx = row->rot[i].vx + (r * TrigTable[(((row->unkE0[i].vz - 0x400) & 0xFFF) - 0x400) & 0xFFF].unk2 >> 12);
                v.vy = row->rot[i].vy + (r * TrigTable[(row->unkE0[i].vz - 0x400) & 0xFFF].unk2 >> 12);
                v.vz = row->rot[i].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                gte_stsxy(&prim->x1);
            }
            if (row->unk40[i] != 0) {
                prim->tag = (prim->tag & 0xFF000000) | (RenderState.ot[700] & 0xFFFFFF);
                p = (SVECTOR *)0xFF000000;
                RenderState.ot[700] = (RenderState.ot[700] & (u32)p) | ((u32)prim & 0xFFFFFF);
            }
            prim++;
            i++;
        }
        arg0 = arg0->next;
        row++;
    }
}

void PlotSparkGFX(NAS *arg0) {
    if (GamePause == 0) {
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

    if (PodiumTrophyObj != NULL) {
        c = PodiumTrophyObj->child;
        if (c != NULL && PodiumTrophyObj->f5C == 0) {
            PodiumTrophyObj->f5C = 1;
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
    PodiumTrophyObj = (PodiumObj *)arg0;
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
        PodiumHeroNas[arg0] = nas;
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
                    p = (u8 *)PodiumHeroAnims;
                    t = (k - 1) * 4;
                    s->unk74 = *(u16 *)(p + t);
                    s->flags |= 0x8000;
                    s = e->sub;
                    s->unk08 = -*(s32 *)((u8 *)PodiumHeroX + t);
                }
            }
        }
    }
}

void PodiumRenderScene(void) {
    func_8009440C();
    func_80018B08(&MainViewport, &MainGfxCamera);
    func_8001C2C0(GetEngineSlot(6));
}

void UpdatePodium(void) {
    s32 i;
    s32 any = 0;

    for (i = 0; i < 4; i++) {
        if (((PadButtons *)Pads)[i].unk54 & 0x4000) {
            any = 1;
        }
    }
    if (any != 0 && PodiumReady != 0 && PodiumState < 4U) {
        PodiumState = 4;
    }
    if (PodiumState == 0x64) {
        ((HubGameView *)GameInfo)->unk24 = 0;
        ((HubGameView *)GameInfo)->unk30 = 0x8002;
    }
    PodiumCheer();
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char HubTextDone[] = "DONE";
const char HubTextCancel[] = "CANCEL";
const char HubKeyA[] = "A";
const char HubKeyB[] = "B";
const char HubKeyC[] = "C";
const char HubKeyD[] = "D";
const char HubKeyE[] = "E";
const char HubKeyF[] = "F";
const char HubKeyG[] = "G";
const char HubKeyH[] = "H";
const char HubKeyI[] = "I";
const char HubKeyJ[] = "J";
const char HubKeyK[] = "K";
const char HubKeyL[] = "L";
const char HubKeyM[] = "M";
const char HubKeyN[] = "N";
const char HubKeyO[] = "O";
const char HubKeyP[] = "P";
const char HubKeyQ[] = "Q";
const char HubKeyR[] = "R";
const char HubKeyS[] = "S";
const char HubKeyT[] = "T";
const char HubKeyU[] = "U";
const char HubKeyV[] = "V";
const char HubKeyW[] = "W";
const char HubKeyX[] = "X";
const char HubKeyY[] = "Y";
const char HubKeyZ[] = "Z";
const char HubKeyAt[] = "@";
const char HubKeyBack[] = "<";

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

    if ((u32)(HubSaveChoice - 1) >= 2U) {
        s = HubNameSubs;
        i = HubNameNext;
        cur = s + i;
        j = HubNamePage * sizeof(HubEntry12);
        cur->unk7C = 0;
        k = (HubKey *)((HubEntry12 *)((u8 *)HubNamePages + j))->unk0 + i;
        pad = (PadButtons *)Pads;
        if (pad->pressed & 0x80) {
            HubNameNext += k->unkA;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pad->pressed & 0x20) {
            HubNameNext += k->unkB;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pad->pressed & 0x10) {
            HubNameNext += k->unkC;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pad->pressed & 0x40) {
            HubNameNext += k->unkD;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (pad->pressed & 0x4000) {
            switch (k->unk8) {
            case 4:
                e4 = &HubNamePages[HubNamePage];
                if (HubNameLen < 8) {
                    GameInfo[HubNameLen + 0x18] = 0x20;
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                    if (HubNameLen < 7) {
                        HubNameLen += 1;
                    }
                    if (HubNameLen == 8) {
                        HubNameNext = e4->unk8;
                    }
                } else {
                    PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                break;
            case 0:
                e = &HubNamePages[HubNamePage];
                c = *k->unk4;
                if (HubNameLen < 8) {
                    GameInfo[HubNameLen + 0x18] = c;
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                    if (HubNameLen < 7) {
                        HubNameLen += 1;
                    }
                    if (HubNameLen == 8) {
                        HubNameNext = e->unk8;
                    }
                } else {
                    PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                break;
            case 3:
                if (GameInfo[HubNameLen + 0x18] == 0x20) {
                    if (HubNameLen > 0) {
                        HubNameLen -= 1;
                        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                }
                GameInfo[HubNameLen + 0x18] = 0x20;
                break;
            case 1:
                PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                HubSaveChoice = 1;
                break;
            case 2:
                PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                HubSaveChoice = 2;
                break;
            }
        }
        HubNameSubs[HubNameNext].unk7C = 2;
    }
}

void HubSaveMenuInit(void) {
    subengine *s;
    s32 i;
    SubInit *p;
    SubInitList *list;
    HubEntry12 *new_var;

    HubNameEngine = EngineTemplate;
    HubNameNext = 0;
    HubNamePage = 0;
    HubNameNext = 0;
    HubNamePage = 0;
    i = 0;
    kill_subs(HubNameEngine.sub);
    func_8001D3B0(&HubNameEngine, 0x21, HubNameSubs);
    s = HubNameSubs;
    list = (SubInitList *)HubNamePages;
    p = list->items;
    HubNameEngine.unk10.x = 0xC4;
    HubNameEngine.unk10.y = 0xBE;
    HubNameEngine.flags |= 0x10000000;
    if (i < list->count) {
        do {
            func_80095F48((s32 *)s, p->a, p->b, p->c, 0);
            p++;
            s++;
            i++;
        } while (i < list->count);
    }
    HubSaveChoice = 0;
    for (HubNameLen = 7; HubNameLen > 0; HubNameLen--) {
        if (GameInfo[HubNameLen + 0x18] != 0x20) {
            break;
        }
    }
    if (HubNameLen > 0) {
        new_var = &HubNamePages[HubNamePage];
        HubNameNext = (*new_var).unk8;
    }
    set_fade(&ScreenFade, -0x300, 0);
    cam_init(HubSaveCamera);
    func_80095BEC(0, HubNameLetters);
    GetEngineSlot(0)->unk10.x = 0xC8;
    GetEngineSlot(0)->unk10.y = 0x6E;
    func_80095BEC(1, HubNamePanel);
    GetEngineSlot(1)->unk10.x = 0;
    GetEngineSlot(1)->unk10.y = 0;
}

void HubSaveMenuRender(void) {
    s32 i;
    u8 *g;
    s32 *p;
    EXTengine *e;

    func_80018B08(&HubSaveViewport, (GfxCamera *)HubSaveCamera);
    RenderState.modelDp = ScreenFade.amount;
    e = GetEngineSlot(0);
    i = 0;
    g = GameInfo;
    p = (s32 *)((u8 *)e->sub + 0x540);
    do {
        HubNameCursor[i * 2] = g[i + 0x18];
        if (HubNameCursor[i * 2] == 0x20) {
            *p |= 0x8000;
        } else {
            *p &= ~0x8000;
        }
        p += 0x2A;
        i++;
    } while (i < 8);
    func_8001C2C0((u8 *)GetEngineSlot(0));
    func_8001C2C0((u8 *)GetEngineSlot(1));
    func_8001C2C0((u8 *)&HubNameEngine);
}

void HubSaveMenuQuit(void) {
    func_80095A04(0);
    func_80095A04(1);
}

void HubSaveMenuUpdate(void) {
    HubSaveMenuInput();
    if (Pads[0].pressed & 9) {
        ProcessControl.next_process = &SaveScreenProcess;
    }
}

void HubNameEntryKey(s8 arg0) {
    HubEntry12 *e;

    e = &HubNamePages[HubNamePage];
    if (HubNameLen < 8) {
        GameInfo[HubNameLen + 0x18] = arg0;
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        if (HubNameLen < 7) {
            HubNameLen += 1;
        }
        if (HubNameLen == 8) {
            HubNameNext = e->unk8;
        }
    } else {
        PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void HubSaveMenuSubs(SubInitList *list) {
    s32 i;
    subengine *s;
    SubInit *p;

    kill_subs(HubNameEngine.sub);
    do {
        i = 0;
        func_8001D3B0(&HubNameEngine, 0x21, HubNameSubs);
    } while (0);
    s = HubNameSubs;
    HubNameEngine.unk10.x = 0xC4;
    HubNameEngine.unk10.y = 0xBE;
    HubNameEngine.flags |= 0x10000000;
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
    HubNameEngine = EngineTemplate;
    HubNameNext = 0;
    HubNamePage = 0;
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

    r = ScreenRect;
    if (cdb == D_8005B790) {
        r += 1;
    }
    SetDrawArea((DRAWENV *)((u8 *)b + (FrameCount & 1) * 12), r);
    rc = (RECT *)a;
    rc->x += r->x;
    rc->y += r->y;
    SetDrawArea((DRAWENV *)((u8 *)b + ((FrameCount & 1) * 12 + 0x18)), rc);
    off = (FrameCount & 1) * 12;
    off2 = off + 0x18;
    p = (u32 *)(off + (s32)b);
    ((HubPTag *)&p[6])->addr = ((HubPTag *)&RenderState.ot[0x136])->addr;
    ((HubPTag *)&RenderState.ot[0x136])->addr = (u32)((u8 *)b + off2);
    ((HubPTag *)&p[0])->addr = ((HubPTag *)&RenderState.ot[2])->addr;
    ((HubPTag *)&RenderState.ot[2])->addr = (u32)p;
    q = ((u32 *)cdb) - (-c);
    ((HubPTag *)&RenderState.ot[0])->addr = ((HubPTag *)q)->addr;
    ((HubPTag *)q)->addr = (u32)(RenderState.ot + 0x137);
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
        saved = RenderState.ot;
        func_8001D894(obj, arg1, obj->flags, c);
        RenderState.ot = (u32 *)(HubOtBuffer + ((FrameCount & 1) << 14));
        ClearOTagR(RenderState.ot, 0x138);
        obj->flags |= 0x2000000;
        obj->unk68 = -0x170;
        func_80019A60(obj->unk74, (u8 *)c, obj->flags, obj);
        HubProjectObj(obj, sp10);
        d = sp10[3];
        y = sp10[1];
        h = (HubDoorFade * (d - y)) / 4096;
        sp18[2] = 0x200;
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[3] = h + y;
        HubSetDrawArea(sp18, HubDrawEnv, 0x400, y);
        RenderState.ot = saved;
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
const char HubTextEnterName[] = "ENTER NAME";
const char HubTextDot[4] = ".\000\"\226";
const char HubCredit000[] = "DEVELOPED BY";
const char HubCredit001[] = "EUROCOM ENTERTAINMENT";
const char HubCredit002[] = "SOFTWARE";
const char HubCredit003[] = "IN ASSOCATION WITH";
const char HubCredit004[] = "CERNY GAMES";
const char HubCredit005[] = "PRODUCERS";
const char HubCredit006[] = "JON WILLIAMS";
const char HubCredit007[] = "MARK CERNY";
const char HubCredit008[] = "PROGRAMMING";
const char HubCredit009[] = "DAVE PRIDMORE";
const char HubCredit010[] = "STUART JOHNSON";
const char HubCredit011[] = "JULIAN WALSHAW-VAUGHAN";
const char HubCredit012[] = "STEVE BAK";
const char HubCredit013[] = "KRIS ADCOCK";
const char HubCredit014[] = "GRAPHICS";
const char HubCredit015[] = "MATT DIXON";
const char HubCredit016[] = "STEVE BAMFORD";
const char HubCredit017[] = "CLIVE STEVENSON";
const char HubCredit018[] = "JON PARR";
const char HubCredit019[] = "ANIMATION";
const char HubCredit020[] = "MARK POVEY";
const char HubCredit021[] = "BRYAN ROGERS";
const char HubCredit022[] = "BRIAN MALONE-WEST";
const char HubCredit023[] = "MUSIC AND SOUND EFFECTS";
const char HubCredit024[] = "STEVE DUCKWORTH";
const char HubCredit025[] = "ASSISTANT PRODUCER";
const char HubCredit026[] = "PHIL BENNETT";
const char HubCredit027[] = "TOOLS SUPPORT";
const char HubCredit028[] = "ANDY MITCHELL";
const char HubCredit029[] = "CHRIS JACKSON";
const char HubCredit030[] = "KEVIN MARKS";
const char HubCredit031[] = "QUALITY ASSURANCE MANAGER";
const char HubCredit032[] = "MICHAEL ROBINSON";
const char HubCredit033[] = "QUALITY ASSURANCE";
const char HubCredit034[] = "ANDY DILKS";
const char HubCredit035[] = "RICH HIGGINS";
const char HubCredit036[] = "ANDY LALLY";
const char HubCredit037[] = "JOHN LEE BARKER";
const char HubCredit038[] = "CHRIS J. JACKSON";
const char HubCredit039[] = "RICHARD NICHOLLS";
const char HubCredit040[] = "MARK SHAW";
const char HubCredit041[] = "RICHARD CHARLES";
const char HubCredit042[] = "SPECIAL THANKS";
const char HubCredit043[] = "MICHAEL JOHN";
const char HubCredit044[] = "HUGH BINNS";
const char HubCredit045[] = "MAT SNEAP";
const char HubCredit046[] = "JIM WILSON";
const char HubCredit047[] = "RACHEL HALL";
const char HubCredit048[] = "MIKE BOTHAM";
const char HubCredit049[] = "CARL CROPLEY";
const char HubCredit050[] = "CORNELL JOHN";
const char HubCredit051[] = "PRODUCED BY";
const char HubCredit052[] = "UNIVERSAL INTERACTIVE";
const char HubCredit053[] = "STUDIOS";
const char HubCredit054[] = "SENIOR PRODUCER";
const char HubCredit055[] = "VIJAY LAKSHMAN";
const char HubCredit056[] = "PRODUCTION COORDINATORS";
const char HubCredit057[] = "MELISSA MILLER";
const char HubCredit058[] = "BRIAN STEPHENS";
const char HubCredit059[] = "ADDITIONAL MINIGAME DESIGN";
const char HubCredit060[] = "SEAN KRANKEL";
const char HubCredit061[] = "RICCI RUKAVINA";
const char HubCredit062[] = "ANTHONY AHN";
const char HubCredit063[] = "SEAN MOUNTAIN";
const char HubCredit064[] = "NOEL GALLAN";
const char HubCredit065[] = "JOSH GOTTSEGEN";
const char HubCredit066[] = "DIRECTOR OF MARKETING";
const char HubCredit067[] = "PRITY PATEL";
const char HubCredit068[] = "MARKETING COORDINATOR";
const char HubCredit069[] = "MARCUS SAVINO";
const char HubCredit070[] = "PUBLISHED AND PRODUCED BY";
const char HubCredit071[] = "SONY COMPUTER";
const char HubCredit072[] = "ENTERTAINMENT";
const char HubCredit073[] = "AMERICA";
const char HubCredit074[] = "DONOVAN SOTO";
const char HubCredit075[] = "PRODUCER";
const char HubCredit076[] = "GRADY HUNT";
const char HubCredit077[] = "SAM THOMPSON";
const char HubCredit078[] = "DIRECTOR OF\01516PRODUCT DEVELOPMENT";
const char HubCredit079[] = "CONNIE BOOTH";
const char HubCredit080[] = "VICE PRESIDENT OF\01516PRODUCT DEVELOPMENT";
const char HubCredit081[] = "SHUHEI YOSHIDA";
const char HubCredit082[] = "AMI MATSUMURA-BLAIRE";
const char HubCredit083[] = "ASSOCIATE PRODUCT MANAGER";
const char HubCredit084[] = "RON EAGLE";
const char HubCredit085[] = "LEGAL AND INTELLECTUAL\01516PROPERTY";
const char HubCredit086[] = "KIRSTEN COSTELLO";
const char HubCredit087[] = "KERRY HOPKINS";
const char HubCredit088[] = "LISA LUNGER";
const char HubCredit089[] = "RILEY RUSSELL";
const char HubCredit090[] = "MARKET RESEARCH";
const char HubCredit091[] = "JOHN CROMPTON";
const char HubCredit092[] = "PROMOTIONS";
const char HubCredit093[] = "SHARON SHAPIRO";
const char HubCredit094[] = "JANEEN ANDERSON";
const char HubCredit095[] = "DONNA ARMENTOR";
const char HubCredit096[] = "AIMEE DUELL";
const char HubCredit097[] = "PUBLIC RELATIONS";
const char HubCredit098[] = "MOLLY SMITH";
const char HubCredit099[] = "RYAN BOWLING";
const char HubCredit100[] = "STEPHANIE IWAMASA";
const char HubCredit101[] = "CHARLOTTE PANTHER";
const char HubCredit102[] = "EILEEN RODRIQUEZ";
const char HubCredit103[] = "CREATIVE SERVICES";
const char HubCredit104[] = "MARILYN WEYANT";
const char HubCredit105[] = "JOSH BINGHAM";
const char HubCredit106[] = "JOHN DIAMONON";
const char HubCredit107[] = "PEGGY GALLAGHER";
const char HubCredit108[] = "TED JALBERT";
const char HubCredit109[] = "QUINN LEE";
const char HubCredit110[] = "MARIE MACASPAC";
const char HubCredit111[] = "RUTH SATO";
const char HubCredit112[] = "JACK SILVER";
const char HubCredit113[] = "QA MANAGER";
const char HubCredit114[] = "CHRIS CAPRIO";
const char HubCredit115[] = "SUPERVISORS";
const char HubCredit116[] = "BRUCE COCHRANE";
const char HubCredit117[] = "TIM DUZMAL";
const char HubCredit118[] = "CONNER MORLANG";
const char HubCredit119[] = "TECHNICAL COORDINATORS";
const char HubCredit120[] = "BEN BRIONES";
const char HubCredit121[] = "DATABASE COORDINATOR";
const char HubCredit122[] = "CHRISTIAN DAVIES";
const char HubCredit123[] = "LEAD TESTERS";
const char HubCredit124[] = "ANDREW BYRNE";
const char HubCredit125[] = "DEREK RAYLA";
const char HubCredit126[] = "ASSISTANT LEAD TESTERS";
const char HubCredit127[] = "MICHAEL CHANG";
const char HubCredit128[] = "STEVEN GONZALEZ";
const char HubCredit129[] = "TERESA KOSSOB";
const char HubCredit130[] = "TESTERS";
const char HubCredit131[] = "ANGELICA BERNARBE";
const char HubCredit132[] = "ERIC BODINE";
const char HubCredit133[] = "COBALT DENA";
const char HubCredit134[] = "JENNYFER HAMILTON";
const char HubCredit135[] = "BURR HILSABECK";
const char HubCredit136[] = "RICH KRINOK";
const char HubCredit137[] = "JOEL MANZANO";
const char HubCredit138[] = "ROSS MONTGOMERY";
const char HubCredit139[] = "ERIC ROSSI";
const char HubCredit140[] = "AARON WHITMORE";
const char HubCredit141[] = "ANDREW WOODWORTH";
const char HubCredit142[] = "CHRIS ARENDS";
const char HubCredit143[] = "ERIC FRANKLIN";
const char HubCredit144[] = "GREG PHILLIPS";
const char HubCredit145[] = "GRETEL PEREZ";
const char HubCredit146[] = "HENRY MACAIRAN";
const char HubCredit147[] = "JACOB SLIPAK";
const char HubCredit148[] = "JASON GREEN";
const char HubCredit149[] = "JO ANNE LARSEN";
const char HubCredit150[] = "JOE RAPOSO";
const char HubCredit151[] = "LEONARD KO";
const char HubCredit152[] = "MIKAL DACE";
const char HubCredit153[] = "NATHAN FRANTZ";
const char HubCredit154[] = "RAMON CONCEPCION";
const char HubCredit155[] = "RUSSELL YOUNG";
const char HubCredit156[] = "SEAN PHELPS";
const char HubCredit157[] = "SEANN PARKER";
const char HubCredit158[] = "SHANNON LYNAS";
const char HubCredit159[] = "SHAWN REFOUA";
const char HubCredit160[] = "STEPHANIE BEIN";
const char HubCredit161[] = "STERLING HAYWOOD";
const char HubCredit162[] = "TONY FELICE";
const char HubCredit163[] = "WILL SHEPARD";
const char HubCredit164[] = "SETH EVANS";
const char HubCredit165[] = "BRUCE ADAMS";
const char HubCredit166[] = "GARY BARTH";
const char HubCredit167[] = "CLAUDETTE CASTILLO";
const char HubCredit168[] = "LORI CHASE";
const char HubCredit169[] = "SARA CHENOWETH";
const char HubCredit170[] = "EDDY CHU";
const char HubCredit171[] = "MIKE CZAKO";
const char HubCredit172[] = "BRIAN DIMICK";
const char HubCredit173[] = "ALBERTO ESCOBEDO";
const char HubCredit174[] = "BOB FREMGEN";
const char HubCredit175[] = "GERRY GENTILE";
const char HubCredit176[] = "BRIAN HALE";
const char HubCredit177[] = "KARA HARRIS";
const char HubCredit178[] = "PHIL HARRISON";
const char HubCredit179[] = "KAZ HIRAI";
const char HubCredit180[] = "BETSEY HOROWITZ";
const char HubCredit181[] = "ANDREW HOUSE";
const char HubCredit182[] = "JEFF HUTCHINSON";
const char HubCredit183[] = "GRACE KAO";
const char HubCredit184[] = "LORAINE KRAUS";
const char HubCredit185[] = "CHUCK LACSON";
const char HubCredit186[] = "COLIN MACLEAN";
const char HubCredit187[] = "KRIS MATHUR";
const char HubCredit188[] = "FRANK O'MALLEY";
const char HubCredit189[] = "JOEL PAMBID";
const char HubCredit190[] = "GARY PASCOE";
const char HubCredit191[] = "JOHN PAYNE";
const char HubCredit192[] = "RICH LAROCCA";
const char HubCredit193[] = "MAGGIE ROJAS";
const char HubCredit194[] = "RICK ROONEY";
const char HubCredit195[] = "STEVE ROSS";
const char HubCredit196[] = "PETER STEINZEIG";
const char HubCredit197[] = "JONI TONEY";
const char HubCredit198[] = "JACK TRETTON";
const char HubCredit199[] = "MARK VALLEDOR";
const char HubCredit200[] = "TONEY WONG";
const char HubCredit201[] = "FLEISHMAN-HILLARD";
const char HubCredit202[] = "TBWA - CHIAT DAY";
const char HubCredit203[] = "AXIOM DESIGNS";
const char HubCredit204[] = "SONY COMPUTER ENTERTAINMENT JAPAN";
const char HubCredit205[] = "RYOICHI HASEGAWA";
const char HubCredit206[] = "ATSUSHI MATSUSHITA";
const char HubCredit207[] = "SONY COMPUTER ENTERTAINMENT EUROPE";
const char HubCredit208[] = "ELIZABETH ASHFORD";
const char HubCredit209[] = "KENNY MATHERS";
const char HubCredit210[20] = "ISABELLE TOMATIS\000\000\004\225";

/* data, in address order */
s32 HubLockText00[1] = { (s32)&HubLockMsg00 };
s32 HubLockText01[1] = { (s32)&HubLockMsg01 };
s32 HubLockText02[1] = { (s32)&HubLockMsg02 };
s32 HubLockText03[1] = { (s32)&HubLockMsg03 };
s32 HubLockText04[1] = { (s32)&HubLockMsg04 };
s32 HubLockText05[1] = { (s32)&HubLockMsg05 };
s32 HubLockText06[1] = { (s32)&HubLockMsg06 };
s32 HubLockText07[1] = { (s32)&HubLockMsg07 };
s32 HubLockText08[1] = { (s32)&HubLockMsg08 };
s32 HubLockText09[1] = { (s32)&HubLockMsg09 };
s32 HubLockText10[1] = { (s32)&HubLockMsg10 };
s32 HubLockPanel00[27] = {
    4, 0x12C8000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&HubLockText00, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel01[27] = {
    4, 0x12C8000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&HubLockText01, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel02[27] = {
    4, 0x1908000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&HubLockText02, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel03[27] = {
    4, 0x1868000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&HubLockText03, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel04[27] = {
    4, 0x1728000, 0x360000, 0, 0, 0, 0, 0, 0, 0xE, 0x88000, 0, 0, 0, (s32)&HubLockText04, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel05[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&HubLockText05, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel06[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&HubLockText06, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel07[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&HubLockText07, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel08[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&HubLockText08, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel09[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&HubLockText09, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubLockPanel10[27] = {
    4, 0xFA8000, 0x2D0000, 0, 0, 0, 0, 0, 0, 0xE, 0x48000, 0, 0, 0, (s32)&HubLockText10, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubSaveText[1] = { (s32)&HubTextPressSave };
s32 HubSavePanel[27] = {
    4, 0x12C8000, 0x2C0000, 0, 0, 0, 0, 0, 0, 0xE, 0xE00AA, 0, 0, 0, (s32)&HubSaveText, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *HubSavePanels[1] = { (void *)&HubSavePanel };
s32 HubStartText[1] = { (s32)&HubTextPressStart };
s32 HubStartPanel[27] = {
    4, 0x12C8000, 0x2C0000, 0, 0, 0, 0, 0, 0, 0xE, 0xE00AA, 0, 0, 0, (s32)&HubStartText, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *HubStartPanels[1] = { (void *)&HubStartPanel };
s32 HubChallengePanel[36] = {
    4, 0x1A48000, 0x370000, 0, 0, 0, 0, 0, 0, 3, 0x48000, 0, 0, 0, (s32)&HubTextSelectChallenge, 4, 0, 0,
    3, 0x1E8000, 0, 0, 0, (s32)&HubTextPressStart, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *HubChallengePanels[1] = { (void *)&HubChallengePanel };
s32 HubWarpPanel[36] = {
    4, 0x18B8000, 0x3C0000, 0, 0, 0, 0, 0, 0, 3, 0x68000, 0, 0, 0, (s32)&HubTextSelectWarpRoom, 4, 0, 0,
    3, 0x268000, 0, 0, 0, (s32)&HubTextPressEnter, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *HubWarpPanels[1] = { (void *)&HubWarpPanel };
s32 HubPapuPanel[27] = {
    4, 0x1908000, 0x3A0000, 0, 0, 0, 0, 0, 0, 3, 0xA8000, 0, 0, 0, (s32)&HubTextDefeatPapu, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
s32 *HubPapuPanels[1] = { (void *)&HubPapuPanel };
EngRec24 HubDoorMenuRecs[8] = {
    { { 4, 0, 0, 0, 0x5A, 0, 0xBE, 0, 0, 0, 0x56, 0, 2, 0, 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 } },
    { { 4, 0, 0, 0, 0, 0, 0xC2, 1, 0, 0, 0x56, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 } },
    { { 5, 0, 0, 0, 0x2D, 0, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0, 0xD5, 0 }, 0, { 2, 0, 0, 0 } },
    { { 5, 0, 0, 0, 0x87, 0, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0, 0xD5, 0 }, 6, { 2, 0, 0, 0 } },
    { { 5, 0, 0, 0, 0xE1, 0, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0, 0xD5, 0 }, 8, { 2, 0, 0, 0 } },
    { { 5, 0, 0, 0, 0x3B, 1, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0, 0xD5, 0 }, 6, { 2, 0, 0, 0 } },
    { { 5, 0, 0, 0, 0x95, 1, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0, 0xD5, 0 }, 6, { 2, 0, 0, 0 } },
    { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0, 0, 0 }, 0, { 0, 0, 0, 0 } },
};
s32 HubDoorAngles[5] = { 0, 0x5A, 0xB4, 0x10E, 0x168 };
KeyedPair HubLockPanels[11] = {
    { 0xB, { (s32)&HubLockPanel00 } }, { 4, { (s32)&HubLockPanel01 } }, { 0x16, { (s32)&HubLockPanel02 } },
    { 9, { (s32)&HubLockPanel03 } }, { 0x1F, { (s32)&HubLockPanel04 } },
    { 0x1C, { (s32)&HubLockPanel05 } }, { 0x1D, { (s32)&HubLockPanel06 } },
    { 0x1E, { (s32)&HubLockPanel07 } }, { 0x1A, { (s32)&HubLockPanel08 } },
    { 0x19, { (s32)&HubLockPanel09 } }, { 5, { (s32)&HubLockPanel10 } },
};
CamEase HubCamEase = { { 4, 4, 4 }, { 0x10, 4, 1 }, { 1, 0x10, 0x10 } };
s32 HubUnused0[3] = { 1, 1, 1 };
u16 HubHeroSizes[10] = { 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0xC00, 0xC00, 0xC00, 0xC00, 0x1000 };
char *HubHaveTexts[5] = {
    (void *)&HubTextHaveTrophy, (void *)&HubTextHaveGem, (void *)&HubTextHaveCrystal, (void *)&HubTextHaveRelic,
    (void *)&HubTextHavePlatinum,
};
process_info HubDoorMenu = { HubDoorMenuInit, HubDoorMenuUpdate, HubDoorMenuRender, HubDoorMenuQuit, 0 };
process_info HubDoorMenu2 = { HubDoorMenu2Init, HubDoorMenu2Update, HubDoorMenu2Render, HubDoorMenu2Quit, 0 };
s32 HubPathSteps0[11] = { 0x2019, 0x201A, 0x201B, 0x201C, 0x201D, 0x201E, 0x201D, 0x201C, 0x201B, 0x201A, 0x8000 };
s32 HubPathSteps1[2] = { 0x22019, 0x8000 };
s32 HubPathSteps2[11] = { 0x201F, 0x2020, 0x2021, 0x2022, 0x2023, 0x2024, 0x2023, 0x2022, 0x2021, 0x2020, 0x8000 };
s32 HubPathSteps3[2] = { 0x22019, 0x8000 };
s32 HubPathSteps4[11] = { 0x2025, 0x2026, 0x2027, 0x2028, 0x2029, 0x202A, 0x2029, 0x2028, 0x2027, 0x2026, 0x8000 };
s32 HubPathSteps5[2] = { 0x22025, 0x8000 };
s32 HubPathAnimList[8] = {
    (s32)&HubPathSteps0, (s32)&HubPathSteps1, (s32)&HubPathSteps2, (s32)&HubPathSteps3, (s32)&HubPathSteps4,
    (s32)&HubPathSteps5, 0, 0,
};
ScriptInit HubPathScriptInit = { 0, (void *)&HubPathAnimList, 0x402 };
EngRec90248 HubGateways[12] = {0};
s32 HubDoorChoice = 0;
s32 HubSelectState = 0;
u8 HubSelectFlag = 0;
s32 HubSelectedEntry = 0;
s32 HubSelectTimer = 0;
subengine *HubSelectSub = 0;
_sfx_handler *HubSelectSfx = 0;
s32 HubHeroPressed = 0;
s32 HubHeroTouchCount = 0;
s32 HubUnused1[1] = {0};
s32 HubDoorChoices = 0;
s32 PodiumSteps0A[3] = { (s32)0x80110000, 0x41000019, 0x8000 };
s32 PodiumSteps0B[3] = { (s32)0x80110000, 0x41190017, 0x8001 };
s32 PodiumSteps0C[3] = { (s32)0x80110000, 0x40000020, 0x8000 };
s32 PodiumSteps0D[9] = {
    (s32)0x80120000, 0x40A5000B, 0xB8011, (s32)0x8011408E, 0x408E000B, 0xB8011,
    (s32)0x8011408E, 0x408E0017, 0x8001,
};
s32 PodiumAnimList0[8] = { (s32)&PodiumSteps0A, (s32)&PodiumSteps0B, 0, 0, (s32)&PodiumSteps0C, (s32)&PodiumSteps0D, 0, 4 };
ScriptInit PodiumScript0 = { 0, (void *)&PodiumAnimList0, 0x401 };
s32 PodiumSteps1A[3] = { (s32)0x80110000, 0x4000001C, 0x8000 };
s32 PodiumSteps1B[3] = { (s32)0x80110000, 0x41000064, 0x8001 };
s32 PodiumSteps1C[3] = { (s32)0x80110000, 0x40800038, 0x8001 };
s32 PodiumAnimList1[8] = { (s32)&PodiumSteps1A, (s32)&PodiumSteps1B, 0, 0, (s32)&PodiumSteps1A, (s32)&PodiumSteps1C, 0, 4 };
ScriptInit PodiumScript1 = { 0, (void *)&PodiumAnimList1, 0x401 };
s32 PodiumSteps2A[3] = { (s32)0x80110000, 0x40000016, 0x8000 };
s32 PodiumSteps2B[3] = { (s32)0x80110000, 0x40800022, 0x8001 };
s32 PodiumSteps2C[3] = { (s32)0x80110000, 0x4100005A, 0x8001 };
s32 PodiumAnimList2[8] = { (s32)&PodiumSteps2A, (s32)&PodiumSteps2B, 0, 0, (s32)&PodiumSteps2A, (s32)&PodiumSteps2C, 0, 4 };
ScriptInit PodiumScript2 = { 0, (void *)&PodiumAnimList2, 0x401 };
s32 PodiumSteps3A[3] = { (s32)0x80110000, 0x41000020, 0x8000 };
s32 PodiumSteps3B[6] = { (s32)0x80110000, 0x40000020, 0x168011, (s32)0x80124009, 0x4009000A, 0x8001 };
s32 PodiumSteps3C[6] = { (s32)0x80110000, 0x4080002F, 0x1F8011, (s32)0x8011408F, 0x40AF0007, 0x8001 };
s32 PodiumAnimList3[8] = { (s32)&PodiumSteps3A, (s32)&PodiumSteps3B, 0, 0, (s32)&PodiumSteps3A, (s32)&PodiumSteps3C, 0, 4 };
ScriptInit PodiumScript3 = { 0, (void *)&PodiumAnimList3, 0x401 };
s32 PodiumSteps4A[3] = { (s32)0x80110000, 0x40000016, 0x8000 };
s32 PodiumSteps4B[3] = { (s32)0x80110000, 0x40800060, 0x8001 };
s32 PodiumSteps4C[3] = { (s32)0x80110000, 0x4180001E, 0x8000 };
s32 PodiumSteps4D[6] = { (s32)0x80110000, 0x41000020, 0x148011, (s32)0x8011410B, 0x410B001C, 0x8001 };
s32 PodiumAnimList4[8] = { (s32)&PodiumSteps4A, (s32)&PodiumSteps4B, 0, 0, (s32)&PodiumSteps4C, (s32)&PodiumSteps4D, 0, 4 };
ScriptInit PodiumScript4 = { 0, (void *)&PodiumAnimList4, 0x401 };
s32 PodiumSteps5A[3] = { (s32)0x80110000, 0x41000014, 0x8000 };
s32 PodiumSteps5B[3] = { (s32)0x80110000, 0x4180000D, 0x8001 };
s32 PodiumSteps5C[3] = { (s32)0x80110000, 0x40000014, 0x8000 };
s32 PodiumSteps5D[3] = { (s32)0x80110000, 0x40800043, 0x8000 };
s32 PodiumAnimList5[8] = { (s32)&PodiumSteps5A, (s32)&PodiumSteps5B, 0, 0, (s32)&PodiumSteps5C, (s32)&PodiumSteps5D, 0, 4 };
ScriptInit PodiumScript5 = { 0, (void *)&PodiumAnimList5, 0x401 };
s32 PodiumSteps6A[3] = { (s32)0x80110000, 0x40000020, 0x8000 };
s32 PodiumSteps6B[5] = { (s32)0x80110000, 0x4080004F, 0x218012, (s32)0x800140A1, 0 };
s32 PodiumSteps6C[3] = { (s32)0x80110000, 0x4100003C, 0x8001 };
s32 PodiumAnimList6[8] = { (s32)&PodiumSteps6A, (s32)&PodiumSteps6B, 0, 0, (s32)&PodiumSteps6A, (s32)&PodiumSteps6C, 0, 4 };
ScriptInit PodiumScript6 = { 0, (void *)&PodiumAnimList6, 0x401 };
s32 PodiumSteps7A[3] = { (s32)0x80110000, 0x41000020, 0x8000 };
s32 PodiumSteps7B[3] = { (s32)0x80110000, 0x40800066, 0x8001 };
s32 PodiumSteps7C[5] = { (s32)0x80110000, 0x40000040, 0x1E8012, (s32)0x8001401E, 0 };
s32 PodiumAnimList7[8] = { (s32)&PodiumSteps7A, (s32)&PodiumSteps7B, 0, 0, (s32)&PodiumSteps7A, (s32)&PodiumSteps7C, 0, 4 };
ScriptInit PodiumScript7 = { 0, (void *)&PodiumAnimList7, 0x401 };
u16 PodiumHeroAnims[8] = { 0x200C, 0, 0x200C, 0, 0x200B, 0, 0x2009, 0 };
s32 PodiumHeroX[4] = { 0, 0x140, 0x180, 0x200 };
u8 PodiumSpawns[384] = {
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
s32 PodiumCheerTimer = 0;
s32 PodiumCheerKind = 0;
u8 *PodiumAnimName = (void *)&PodiumAnimWinBreathe;
s32 PodiumAnims[4] = { (s32)&PodiumAnimWin, (s32)&PodiumAnimLoseBreathe, (s32)&PodiumAnimLose, 0 };
u8 PodiumScore1[4] = { 0x31, 0x30, 0, 0 };
u8 PodiumScore2[4] = { 0x32, 0x30, 0, 0 };
u8 PodiumScore3[4] = { 0x33, 0x30, 0, 0 };
u8 PodiumScore4[12] = { 0x34, 0x30, 0, 0, 0x35, 0x30, 0, 0, 0x36, 0x30, 0, 0 };
s32 PodiumScoreCount = 4;
s32 PodiumUnused[106] = {
    0x2300000, 0x560000, 0, 0, 0, 0, 0, 0, 0xA, 0x400118, 0x18000000, (s32)0xFF800100, 0, 0, 0,
    0x2001001C, 2, 8, 0x100000, 0x7000000, 0, 0, 0, 0, 0, 0, 3, 0x40003D, 0, 0, 0,
    (s32)&PodiumScore1, 4, 0, 0, 8, 0x100070, 0x7000000, 0, 0, 0, 0, 0x10000, 0, 3, 0x4000AD, 0,
    0, 0, (s32)&PodiumScore2, 4, 0, 0, 8, 0x100150, 0x7000000, 0, 0, 0, 0, 0x20000, 0, 3,
    0x40018D, 0, 0, 0, (s32)&PodiumScore3, 4, 0, 0, 8, 0x1001C0, 0x7000000, 0, 0, 0, 0, 0x30000,
    0, 3, 0x4001FD, 0, 0, 0, (s32)&PodiumScore4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1000, 0,
    0x1000, 0, 0x1000, 0, 0, 0,
};
HubSparkFx HubSparkFxDef = { 0xC, 0xF00, 0xF00, HubInitSparks, HubDrawSparks };
NAS *PodiumHeroNas[4] = {0};
PodiumObj *PodiumTrophyObj = 0;
s32 PodiumRanks[4] = {0};
s32 PodiumTies = 0;
s32 PodiumCamState = 0;
u32 PodiumState = 0;
s32 PodiumReady = 0;
u8 HubNameCursor[4] = { 0x20, 0, 0x20, 0 };
s32 HubNameCursor2[1] = { 0x200020 };
s32 HubNameCursor3[1] = { 0x200020 };
s32 HubNameButtons[3] = { 0x200020, (s32)&HubTextDone, (s32)&HubTextCancel };
s32 HubNameKeys[120] = {
    0, (s32)&HubKeyA, 0x1060000, 0x715, 0x28, (s32)&HubKeyB, 0x1FF0000, 0x715, 0x50,
    (s32)&HubKeyC, 0x1FF0000, 0x715, 0x78, (s32)&HubKeyD, 0x1FF0000, 0x715, 0xA0,
    (s32)&HubKeyE, 0x1FF0000, 0x715, 0xC8, (s32)&HubKeyF, 0x1FF0000, 0x715, 0xF0,
    (s32)&HubKeyG, (s32)0xFAFF0000, 0x715, 0x280000, (s32)&HubKeyH, 0x1060000, 0x7F9,
    0x280028, (s32)&HubKeyI, 0x1FF0000, 0x7F9, 0x280050, (s32)&HubKeyJ, 0x1FF0000, 0x7F9,
    0x280078, (s32)&HubKeyK, 0x1FF0000, 0x7F9, 0x2800A0, (s32)&HubKeyL, 0x1FF0000, 0x7F9,
    0x2800C8, (s32)&HubKeyM, 0x1FF0000, 0x7F9, 0x2800F0, (s32)&HubKeyN, (s32)0xFAFF0000,
    0x7F9, 0x500000, (s32)&HubKeyO, 0x1060000, 0x7F9, 0x500028, (s32)&HubKeyP, 0x1FF0000,
    0x7F9, 0x500050, (s32)&HubKeyQ, 0x1FF0000, 0x7F9, 0x500078, (s32)&HubKeyR, 0x1FF0000,
    0x7F9, 0x5000A0, (s32)&HubKeyS, 0x1FF0000, 0x7F9, 0x5000C8, (s32)&HubKeyT, 0x1FF0000,
    0x7F9, 0x5000F0, (s32)&HubKeyU, (s32)0xFAFF0000, 0x7F9, 0x780000, (s32)&HubKeyV,
    0x1060000, 0x7F9, 0x780028, (s32)&HubKeyW, 0x1FF0000, 0x6F9, 0x780050, (s32)&HubKeyX,
    0x1FF0000, 0x5F9, 0x780078, (s32)&HubKeyY, 0x1FF0000, 0x4F9, 0x7800A0, (s32)&HubKeyZ,
    0x1FF0000, 0x3F9, 0x7800C8, (s32)&HubKeyAt, 0x1FF0004, 0x2F9, 0x7800F0, (s32)&HubKeyBack,
    (s32)0xFAFF0003, 0x1F9, 0xA0808C, (s32)&HubTextDone, 1, 0x1FB, 0xC8808C, (s32)&HubTextCancel,
    2, 0xFF,
};
s32 HubNameKeys2[12] = {
    0, (s32)&HubKeyA, 0x7F90000, 0x1FF, 0xC88000, (s32)&HubTextDone, 1, 0, 0xE68000,
    (s32)&HubTextCancel, 2, 0,
};
HubEntry12 HubNamePages[2] = { { (s32)&HubNameKeys, 0x1E, 0x1C }, { (s32)&HubNameKeys2, 3, 1 } };
s32 HubEnterNameText[1] = { (s32)&HubTextEnterName };
s32 HubNamePanel[45] = {
    0xE, 0x2C8000, 0, 0, 0, (s32)&HubEnterNameText, 0, 0, 0, 4, 0x2808000, 0x500022, 0, 0, 0, 0, 0,
    0, 4, 0x12C8000, 0x960064, 0, 0, 0, 0, 0, 0, 4, 0x12C8000, 0x1AE00AA, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
s32 HubNameLetters[153] = {
    3, 0, 0, 0, 0, (s32)&HubNameCursor, 0, 0, 0, 3, 0x22, 0, 0, 0, (s32)(u8 *)&HubNameCursor + 0x2,
    0, 0, 0, 3, 0x44, 0, 0, 0, (s32)&HubNameCursor2, 0, 0, 0, 3, 0x66, 0, 0, 0,
    (s32)(u8 *)&HubNameCursor2 + 0x2, 0, 0, 0, 3, 0x88, 0, 0, 0, (s32)&HubNameCursor3, 0, 0, 0, 3,
    0xAA, 0, 0, 0, (s32)(u8 *)&HubNameCursor3 + 0x2, 0, 0, 0, 3, 0xCC, 0, 0, 0, (s32)&HubNameButtons,
    0, 0, 0, 3, 0xEE, 0, 0, 0, (s32)(u8 *)&HubNameButtons + 0x2, 0, 0, 0, 3, 0, 0, 0, 0,
    (s32)&HubTextDot, 0, 0, 0, 3, 0x22, 0, 0, 0, (s32)&HubTextDot, 0, 0, 0, 3, 0x44, 0, 0, 0,
    (s32)&HubTextDot, 0, 0, 0, 3, 0x66, 0, 0, 0, (s32)&HubTextDot, 0, 0, 0, 3, 0x88, 0, 0, 0,
    (s32)&HubTextDot, 0, 0, 0, 3, 0xAA, 0, 0, 0, (s32)&HubTextDot, 0, 0, 0, 3, 0xCC, 0, 0, 0,
    (s32)&HubTextDot, 0, 0, 0, 3, 0xEE, 0, 0, 0, (s32)&HubTextDot, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0,
};
GfxViewport HubSaveViewport = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0, 0, 0, 0xFF0, 0, 0, 0, 0, 1 };
SaveMenuHooks HubSaveMenuHooks = { HubSaveMenuInit, HubSaveMenuUpdate, HubSaveMenuRender, HubSaveMenuQuit };
s32 HubUnused2[1] = {0};
s32 HubSaveChoice = 0;
s32 HubUnused3[1] = {0};
subengine HubNameSubs[33] = {0};
EXTengine HubNameEngine = {0};
s32 HubNameNext = 0;
s32 HubNamePage = 0;
s32 HubNameLen = 0;
u8 HubSaveCamera[184] = {0};
s32 HubDoorFade = 0;
u8 HubOtBuffer[32768] = {0};
u8 HubDrawEnv[48] = {0};
OvlTextLine HubCreditsLines[264] = {
    { 3, 0, 0x1F4 }, { 0, (void *)&HubCredit000, 0 }, { 1, (void *)&HubCredit001, 0 },
    { 1, (void *)&HubCredit002, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit003, 0 },
    { 1, (void *)&HubCredit004, 0 }, { 3, 0, 0x96 }, { 0, (void *)&HubCredit005, 0 },
    { 2, (void *)&HubCredit006, 0 }, { 2, (void *)&HubCredit007, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit008, 0 }, { 2, (void *)&HubCredit009, 0 },
    { 2, (void *)&HubCredit010, 0 }, { 2, (void *)&HubCredit011, 0 },
    { 2, (void *)&HubCredit012, 0 }, { 2, (void *)&HubCredit013, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit014, 0 }, { 2, (void *)&HubCredit015, 0 },
    { 2, (void *)&HubCredit016, 0 }, { 2, (void *)&HubCredit017, 0 },
    { 2, (void *)&HubCredit018, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit019, 0 },
    { 2, (void *)&HubCredit020, 0 }, { 2, (void *)&HubCredit021, 0 },
    { 2, (void *)&HubCredit022, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit023, 0 },
    { 2, (void *)&HubCredit024, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit025, 0 },
    { 2, (void *)&HubCredit026, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit027, 0 },
    { 2, (void *)&HubCredit028, 0 }, { 2, (void *)&HubCredit029, 0 },
    { 2, (void *)&HubCredit030, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit031, 0 },
    { 2, (void *)&HubCredit032, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit033, 0 },
    { 2, (void *)&HubCredit034, 0 }, { 2, (void *)&HubCredit035, 0 },
    { 2, (void *)&HubCredit036, 0 }, { 2, (void *)&HubCredit037, 0 },
    { 2, (void *)&HubCredit038, 0 }, { 2, (void *)&HubCredit039, 0 },
    { 2, (void *)&HubCredit040, 0 }, { 2, (void *)&HubCredit041, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit042, 0 }, { 2, (void *)&HubCredit043, 0 },
    { 2, (void *)&HubCredit044, 0 }, { 2, (void *)&HubCredit045, 0 },
    { 2, (void *)&HubCredit046, 0 }, { 2, (void *)&HubCredit047, 0 },
    { 2, (void *)&HubCredit048, 0 }, { 2, (void *)&HubCredit049, 0 },
    { 2, (void *)&HubCredit050, 0 }, { 3, 0, 0x32 }, { 3, 0, 0x96 },
    { 0, (void *)&HubCredit051, 0 }, { 1, (void *)&HubCredit052, 0 },
    { 1, (void *)&HubCredit053, 0 }, { 3, 0, 0x96 }, { 0, (void *)&HubCredit054, 0 },
    { 2, (void *)&HubCredit055, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit056, 0 },
    { 2, (void *)&HubCredit057, 0 }, { 2, (void *)&HubCredit058, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit059, 0 }, { 2, (void *)&HubCredit060, 0 },
    { 2, (void *)&HubCredit061, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit033, 0 },
    { 2, (void *)&HubCredit062, 0 }, { 2, (void *)&HubCredit063, 0 },
    { 2, (void *)&HubCredit064, 0 }, { 2, (void *)&HubCredit065, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit066, 0 }, { 2, (void *)&HubCredit067, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit068, 0 }, { 2, (void *)&HubCredit069, 0 }, { 3, 0, 0x32 },
    { 3, 0, 0x96 }, { 0, (void *)&HubCredit070, 0 }, { 1, (void *)&HubCredit071, 0 },
    { 1, (void *)&HubCredit072, 0 }, { 1, (void *)&HubCredit073, 0 }, { 3, 0, 0x96 },
    { 0, (void *)&HubCredit025, 0 }, { 2, (void *)&HubCredit074, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit075, 0 }, { 2, (void *)&HubCredit076, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit025, 0 }, { 2, (void *)&HubCredit077, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit078, 0 }, { 2, (void *)&HubCredit079, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit080, 0 }, { 2, (void *)&HubCredit081, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit066, 0 }, { 2, (void *)&HubCredit082, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit083, 0 }, { 2, (void *)&HubCredit084, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit085, 0 }, { 2, (void *)&HubCredit086, 0 },
    { 2, (void *)&HubCredit087, 0 }, { 2, (void *)&HubCredit088, 0 },
    { 2, (void *)&HubCredit089, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit090, 0 },
    { 2, (void *)&HubCredit091, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit092, 0 },
    { 2, (void *)&HubCredit093, 0 }, { 2, (void *)&HubCredit094, 0 },
    { 2, (void *)&HubCredit095, 0 }, { 2, (void *)&HubCredit096, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit097, 0 }, { 2, (void *)&HubCredit098, 0 },
    { 2, (void *)&HubCredit099, 0 }, { 2, (void *)&HubCredit100, 0 },
    { 2, (void *)&HubCredit101, 0 }, { 2, (void *)&HubCredit102, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit103, 0 }, { 2, (void *)&HubCredit104, 0 },
    { 2, (void *)&HubCredit105, 0 }, { 2, (void *)&HubCredit106, 0 },
    { 2, (void *)&HubCredit107, 0 }, { 2, (void *)&HubCredit108, 0 },
    { 2, (void *)&HubCredit109, 0 }, { 2, (void *)&HubCredit110, 0 },
    { 2, (void *)&HubCredit111, 0 }, { 2, (void *)&HubCredit112, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit033, 0 }, { 0, (void *)&HubCredit113, 0 },
    { 2, (void *)&HubCredit114, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit115, 0 },
    { 2, (void *)&HubCredit116, 0 }, { 2, (void *)&HubCredit117, 0 },
    { 2, (void *)&HubCredit118, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit119, 0 },
    { 2, (void *)&HubCredit120, 0 }, { 2, (void *)&HubCredit077, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit121, 0 }, { 2, (void *)&HubCredit122, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit123, 0 }, { 2, (void *)&HubCredit124, 0 },
    { 2, (void *)&HubCredit125, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit126, 0 },
    { 2, (void *)&HubCredit127, 0 }, { 2, (void *)&HubCredit128, 0 },
    { 2, (void *)&HubCredit129, 0 }, { 3, 0, 0x32 }, { 0, (void *)&HubCredit130, 0 },
    { 2, (void *)&HubCredit131, 0 }, { 2, (void *)&HubCredit132, 0 },
    { 2, (void *)&HubCredit133, 0 }, { 2, (void *)&HubCredit134, 0 },
    { 2, (void *)&HubCredit135, 0 }, { 2, (void *)&HubCredit136, 0 },
    { 2, (void *)&HubCredit137, 0 }, { 2, (void *)&HubCredit138, 0 },
    { 2, (void *)&HubCredit139, 0 }, { 2, (void *)&HubCredit140, 0 },
    { 2, (void *)&HubCredit141, 0 }, { 2, (void *)&HubCredit142, 0 },
    { 2, (void *)&HubCredit143, 0 }, { 2, (void *)&HubCredit144, 0 },
    { 2, (void *)&HubCredit145, 0 }, { 2, (void *)&HubCredit146, 0 },
    { 2, (void *)&HubCredit147, 0 }, { 2, (void *)&HubCredit148, 0 },
    { 2, (void *)&HubCredit149, 0 }, { 2, (void *)&HubCredit150, 0 },
    { 2, (void *)&HubCredit151, 0 }, { 2, (void *)&HubCredit152, 0 },
    { 2, (void *)&HubCredit153, 0 }, { 2, (void *)&HubCredit154, 0 },
    { 2, (void *)&HubCredit155, 0 }, { 2, (void *)&HubCredit156, 0 },
    { 2, (void *)&HubCredit157, 0 }, { 2, (void *)&HubCredit158, 0 },
    { 2, (void *)&HubCredit159, 0 }, { 2, (void *)&HubCredit160, 0 },
    { 2, (void *)&HubCredit161, 0 }, { 2, (void *)&HubCredit162, 0 },
    { 2, (void *)&HubCredit163, 0 }, { 2, (void *)&HubCredit164, 0 }, { 3, 0, 0x32 },
    { 0, (void *)&HubCredit042, 0 }, { 2, (void *)&HubCredit165, 0 },
    { 2, (void *)&HubCredit166, 0 }, { 2, (void *)&HubCredit167, 0 },
    { 2, (void *)&HubCredit168, 0 }, { 2, (void *)&HubCredit169, 0 },
    { 2, (void *)&HubCredit170, 0 }, { 2, (void *)&HubCredit171, 0 },
    { 2, (void *)&HubCredit172, 0 }, { 2, (void *)&HubCredit173, 0 },
    { 2, (void *)&HubCredit174, 0 }, { 2, (void *)&HubCredit175, 0 },
    { 2, (void *)&HubCredit176, 0 }, { 2, (void *)&HubCredit177, 0 },
    { 2, (void *)&HubCredit178, 0 }, { 2, (void *)&HubCredit179, 0 },
    { 2, (void *)&HubCredit180, 0 }, { 2, (void *)&HubCredit181, 0 },
    { 2, (void *)&HubCredit182, 0 }, { 2, (void *)&HubCredit183, 0 },
    { 2, (void *)&HubCredit184, 0 }, { 2, (void *)&HubCredit185, 0 },
    { 2, (void *)&HubCredit186, 0 }, { 2, (void *)&HubCredit187, 0 },
    { 2, (void *)&HubCredit188, 0 }, { 2, (void *)&HubCredit189, 0 },
    { 2, (void *)&HubCredit190, 0 }, { 2, (void *)&HubCredit191, 0 },
    { 2, (void *)&HubCredit192, 0 }, { 2, (void *)&HubCredit193, 0 },
    { 2, (void *)&HubCredit194, 0 }, { 2, (void *)&HubCredit195, 0 },
    { 2, (void *)&HubCredit196, 0 }, { 2, (void *)&HubCredit197, 0 },
    { 2, (void *)&HubCredit198, 0 }, { 2, (void *)&HubCredit199, 0 },
    { 2, (void *)&HubCredit200, 0 }, { 2, (void *)&HubCredit201, 0 },
    { 2, (void *)&HubCredit202, 0 }, { 2, (void *)&HubCredit203, 0 }, { 3, 0, 0x14 },
    { 2, (void *)&HubCredit204, 0 }, { 2, (void *)&HubCredit205, 0 },
    { 2, (void *)&HubCredit206, 0 }, { 3, 0, 0x14 }, { 2, (void *)&HubCredit207, 0 },
    { 2, (void *)&HubCredit208, 0 }, { 2, (void *)&HubCredit209, 0 },
    { 2, (void *)&HubCredit210, 0 }, { 3, 0, 0x32 }, { 4, 0, 0 },
};
s32 PodiumCamTimer = 0;
s32 PodiumUnused2[3] = {0};
CamXYZ PodiumCamAt = {0};
