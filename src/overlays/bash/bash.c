#include "common.h"
#include "bash/bash_ovl.h"
#include "bash/bash.h"
#include "bash/xtras.h"

void InitialiseBASHFirst(void) {
    u8 *g;
    void **p;
    void **p2;
    void **p3;
    void **p4;
    void *t;

    D_8009D6E8.unk68 = BASH_KeepInBounds;
    D_8009D6E8.unk38 = Bash_HeroBot;
    D_8009D6E8.unk5C = BashHero_ExtraEffects;
    D_8009D6E8.unk60 = BashHeroesTouched;
    D_8009D6E8.unk4C = BashHero2ObjectCollision;
    D_8009D6E8.unk50 = BashHeroGrabObject;
    D_8009D6E8.unk54 = BashHeroKickCollision;
    D_8009D6E8.unk58 = BashHeroCarryCollision;
    D_800C478C = NULL;
    BashInitCrateHooks();
    D_8009E298 = BashASTIndex(0x206);
    D_800C46EC = EngineTemplate;
    g = D_8005A614;
    switch (*(s16 *)(g + 0x34)) {
    case 1:
        D_8009D8F4 = D_800C30C4;
        D_8009D8F8 = D_800C30D8;
        D_8009D904 = D_800C30AC;
        p = &D_8009D8FC;
        if (*(s32 *)(g + 0x6C) != 0) {
            t = D_800C308C;
        } else {
            t = D_800C3078;
        }
        *p = t;
        D_8009D900 = D_800C30A0;
        if (D_8005A646 & 0x80) {
            D_8009D724 = BashSpacePadHook;
        }
        break;
    case 2:
        D_8009D8F4 = D_800C3138;
        D_8009D8F8 = D_800C314C;
        D_8009D904 = D_800C3120;
        p2 = &D_8009D8FC;
        if (*(s32 *)(g + 0x6C) != 0) {
            t = D_800C3100;
        } else {
            t = D_800C30EC;
        }
        *p2 = t;
        D_8009D900 = D_800C3114;
        D_8009D70C = BashSnowMoveHero;
        break;
    case 3:
        D_8009D8F4 = D_800C31AC;
        D_8009D8F8 = D_800C31C0;
        D_8009D904 = D_800C3194;
        p3 = &D_8009D8FC;
        if (*(s32 *)(g + 0x6C) != 0) {
            t = D_800C3174;
        } else {
            t = D_800C3160;
        }
        *p3 = t;
        D_8009D900 = D_800C3188;
        D_8009D724 = BashFirePowerup;
        break;
    case 0x20:
        D_8009D724 = BashFirePowerup;
        g[0x52] = 9;
        g[0x53] = 9;
        break;
    default:
        D_8009D8F4 = D_800C3050;
        D_8009D8F8 = D_800C3064;
        D_8009D904 = D_800C3038;
        p4 = &D_8009D8FC;
        if (D_8005A680[0] != 0) {
            t = D_800C3018;
        } else {
            t = D_800C3004;
        }
        *p4 = t;
        D_8009D900 = D_800C302C;
        break;
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/bash/bash", D_800B32B8);

void InitialiseBASH(void) {
    u8 *g;
    s32 i;
    s16 a;
    EXTheromove *m;

    Init_Clock(0x140, 0x28, 0);
    g = D_8005A614;
    if (*(u16 *)(g + 0x32) & 0x40) {
        if (*(s32 *)(g + 0x6C) != 0) {
            switch (*(s16 *)(g + 0x34)) {
            case 0:
                Set_Clock(0x30, 0x81D0);
                break;
            case 1:
                Set_Clock(0x30, 0x81D0);
                break;
            case 2:
                Set_Clock(0x30, 0x81D0);
                break;
            case 3:
                Set_Clock(0x25, 0x81D0);
                break;
            }
        } else {
            switch (*(s16 *)(g + 0x34)) {
            case 0:
                Set_Clock(0x45, 0x81D0);
                break;
            case 1:
                Set_Clock(0x40, 0x81D0);
                break;
            case 2:
                Set_Clock(0x40, 0x81D0);
                break;
            case 3:
                Set_Clock(0x30, 0x81D0);
                break;
            }
        }
    } else {
        Set_Clock(0x130, 0x81D0);
    }
    D_800C3B32 = 0;
    D_800C3B34 = 0;
    D_800C478A = 0x1E;
    *(s16 *)D_800C3B86 = 0;
    D_800C3B38 = 0;
    D_800C3B3A = 0;
    D_800C3B3E = 0;
    D_800C3B3C = (Random(0xA) + 0xA) * 0x1E;
    D_800C3B40 = -1;
    D_800C46B0 = NULL;
    D_800C46B4 = NULL;
    D_800C4788 = Random(0x12);
    D_800C46AC = 0x96;
    BashShuffleDropPoints();
    BashSpaceClearTiles();
    func_80086D88();
    for (i = 0; i < 4; i++) {
        a = get_angle(Heroes[i].hero_eng->unk10.x, Heroes[i].hero_eng->unk10.z);
        m = Heroes[i].hero_move;
        Heroes[i].hero_eng->unk1E = a;
        m->facing_angle = a;
        m->forward_angle = a;
    }
    D_800C3B9C[0] = D_800C3B9C[1] = D_800C3B9C[2] = D_800C3B9C[3] = 0;
    switch (D_8005A648) {
    case 0:
        World_floor_level = -0x10;
        BashSetCrateMix(8, 8, 0xA, 0xC, 0xA, 3, 3, 0, 0);
        BashDropFirstCrates();
        InitAmbience(&D_800C39C0);
        break;
    case 1:
        World_floor_level = -0x10;
        BashSpaceInitTiles();
        D_800C478A = (Random(0xA) + 0xA) * 0x1E;
        D_8009E488.unk8 = BashSpaceBreakTile;
        if (D_8005A646 & 0x80) {
            BashSetCrateMix(6, 7, 8, 8, 0, 0xE, 2, 0, 0);
        } else {
            BashSetCrateMix(0xA, 0xA, 0xC, 0xE, 0xA, 4, 2, 0, 0);
        }
        BashDropFirstCrates();
        break;
    case 2:
        World_floor_level = -0x10;
        D_8009E488.unk0 = D_800C395C;
        D_8009D6F4 = D_800C3984;
        D_800C478A = (Random(0xA) + 0xA) * 0x1E;
        BashSetCrateMix(8, 8, 0xA, 0xC, 0xA, 3, 3, 0, 0);
        D_8005A5DC = 3;
        for (i = 0; i < 4; i++) {
            Heroes[i].hero_move->velocity_calc = D_8005A5DC;
        }
        BashSnowInitHeroMove();
        CreateNAS(0, World_floor_level, 0, 0x1101, 0x11);
        CreateNAS(0, World_floor_level, 0, 0x1102, 0x11);
        BashDropFirstCrates();
        break;
    case 3:
        World_floor_level = 0;
        D_800C478A = Random(0x1E) + 0x1E;
        BashSetCrateMix(6, 6, 6, 6, 0xE, 2, 0, 0, 0);
        BashDropFirstCrates();
        InitAmbience(&D_800C39D8);
        break;
    case 0x20:
        Heroes[2].hero_shadow->flags = 0;
        World_floor_level = -0x10;
        Heroes[3].hero_shadow->flags = 0;
        BashSetCrateMix(8, 8, 0xA, 0xC, 0xA, 3, 3, 0, 0);
        BashDropFirstCrates();
        func_80095258(*(u8 **)D_80058AD0);
        D_8009F8B4 = BashDrawView;
        World_floor_level = 0;
        break;
    }
}

void UpdateBASH(void) {
    s32 i;
    s16 id;
    s16 *p;

    func_80086F78();
    BashCountObjects();
    func_80086D88();
    if (D_8009D908 != 0) {
        if (D_800C3B40 == -1) {
            for (i = 0; i < 4; i++) {
                D_8009D87C[i].unk4 = 0x10;
                D_8009D87C[i].unk0 = 0x1A;
            }
            p = &D_800C3B40;
            *p = BashAI_PickVictim(0x63);
            id = *p;
            if (id != -1 && id < 4) {
                D_8009D87C[id].unk4 = 0x30;
                D_8009D87C[id].unk0 = 8;
            } else {
                D_800C3B40 = -1;
            }
        } else if (Heroes[D_800C3B40].hero_status & 0xFA00) {
            D_800C3B40 = -1;
        }
    }
    switch (D_8005A648) {
    case 0:
    case 3:
        DoAmbience();
        BashCrateMaster();
        break;
    case 1:
        BashSpaceMarkHoles();
        BashSpaceTileEdges();
        BashCrateMaster();
        break;
    case 2:
        BashSnowMarkPenguin();
        BashCrateMaster();
        break;
    case 32:
        BashCrateMaster();
        break;
    }
}

void BashSnowMoveHero(s32 arg0) {
    EXTengine *e = Heroes[arg0].hero_eng;
    EXTheromove *m = Heroes[arg0].hero_move;
    s32 step;

    D_8009D714();
    func_800860D4(arg0);
    if (m->velocity_calc == 3) {
        if (m->x_vel != m->x_max) {
            if (m->x_max != 0) {
                step = m->x_acc;
            } else {
                step = m->x_dec;
            }
            if (Heroes[arg0].hero_eng->unk10.y < World_floor_level) {
                step = 0x80;
            }
            if (m->x_vel < m->x_max) {
                m->x_vel += step;
                if (m->x_vel > m->x_max) {
                    m->x_vel = m->x_max;
                }
            } else {
                m->x_vel -= step;
                if (m->x_vel < m->x_max) {
                    m->x_vel = m->x_max;
                }
            }
        }
        if (m->z_vel != m->z_max) {
            if (m->z_max != 0) {
                step = m->z_acc;
            } else {
                step = m->z_dec;
            }
            if (Heroes[arg0].hero_eng->unk10.y < World_floor_level) {
                step = 0x80;
            }
            if (m->z_vel < m->z_max) {
                m->z_vel += step;
                if (m->z_vel > m->z_max) {
                    m->z_vel = m->z_max;
                }
            } else {
                m->z_vel -= step;
                if (m->z_vel < m->z_max) {
                    m->z_vel = m->z_max;
                }
            }
        }
        e->unk10.z += m->z_vel >> 2;
        e->unk10.x += m->x_vel >> 2;
    }
}

s32 BashAI_EscapeAngle(s32 arg0) {
    s32 i;
    s32 dx;
    s32 dz;

    for (i = 0; i < 4; i++) {
        if (i != arg0 && !(Heroes[i].hero_status & 0xFA00) && Heroes[i].hero_pu == 0x14) {
            dx = Heroes[arg0].hero_eng->unk10.x - Heroes[i].hero_eng->unk10.x;
            dz = Heroes[arg0].hero_eng->unk10.z - Heroes[i].hero_eng->unk10.z;
            if (dx * dx + dz * dz < 0x100000) {
                return get_angle(dx, dz);
            }
        }
    }
    if (D_800C46B4 != NULL && D_800C46B4->status == 3) {
        dx = D_800C46B0->unk10.x - Heroes[arg0].hero_eng->unk10.x;
        dz = D_800C46B0->unk10.z - Heroes[arg0].hero_eng->unk10.z;
        if (dx * dx + dz * dz < 0x240000) {
            return get_angle(-dx, -dz);
        }
    }
    return 0;
}

void BashAI_CheckShot(s32 arg0, s32 arg1, s32 arg2, BashAiState *ai, s32 prev) {
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *o;
    s32 i;
    s32 best;
    s32 min;
    s32 dx;
    s32 dz;
    s32 d;

    base = Heroes;
    h = base + arg0;
    if (h->hero_weapon_count == 0 || h->hero_attack_timer != 0) {
        return;
    }
    best = -1;
    switch (h->hero_pu) {
    case 0x1C:
        min = 0x51000;
        for (i = 0; i < 4; i++) {
            o = &Heroes[i];
            if (arg0 != i && i != arg1 && !(o->hero_status & 0xFA00)) {
                dx = base[arg0].hero_eng->unk10.x - o->hero_eng->unk10.x;
                dz = dx * dx;
                dx = base[arg0].hero_eng->unk10.z - o->hero_eng->unk10.z;
                d = dz + dx * dx;
                if (d < min) {
                    best = i;
                    min = d;
                }
            }
        }
        break;
    case 0x1B:
        min = 0x800;
        goto loop2;
    case 0 ... 0x1A:
    default:
        min = 0x800;
    loop2:
        for (i = 0; i < 4; i++) {
            if (i != arg0 && i != arg1) {
                d = func_80015480(arg2, get_angle(Heroes[i].hero_eng->unk10.x - Heroes[arg0].hero_eng->unk10.x,
                                                  Heroes[i].hero_eng->unk10.z - Heroes[arg0].hero_eng->unk10.z));
                if (d < min) {
                    min = d;
                    best = i;
                }
            }
        }
        break;
    }
    if (best == prev && prev != -1) {
        ai->target = -1;
        ai->unk20 = 1;
        ai->flags |= 0x8000;
    } else if (best != -1 && (D_8005A680[0] == 0 || best != arg1)) {
        ai->target = -1;
        ai->unk20 = 1;
        ai->flags |= 0x8000;
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/bash/bash", D_800B3424);

void Bash_HeroBot(s32 arg0) {
    s32 unused[6];
    EXTbotai *ai;
    u32 *dirs;
    u32 *tbl;
    NAS *nas;
    EXTengine *neng;
    EXTengine *e;
    EXTobject *fux;
    EXTobject *nfux;
    EXTobject *obj;
    PadStick *pad;
    s32 mate;
    s32 dist;
    s32 target;
    s32 ang;
    s32 rnd;
    s32 ok;
    s32 dx;
    s32 dz;
    s32 r;
    s32 d;
    s32 tmp;
    s32 i;

    if (D_8005A624 & 0x04000000) {
        return;
    }
    ai = Heroes[arg0].hero_ai;
    if (ai->AIinitial_delay != 0) {
        ai->AIinitial_delay--;
    }
    if (ai->AIsleep != 0) {
        ai->AIsleep--;
    }
    if (ai->AItimer1 != 0) {
        ai->AItimer1--;
    }
    if (ai->AItimer2 != 0) {
        ai->AItimer2--;
    }
    ai->AIpad = 0;
    mate = Heroes[arg0].hero_teammate;
    if (Heroes[arg0].hero_mode == (u16)D_8009E0D8 || Heroes[arg0].hero_mode == (u16)D_8009E0E2) {
        ai->TargetPlayer = -1;
        ai->AIsleep = 5;
        ai->pTargetNas = NULL;
        ai->AImode = 6;
    }
    neng = NULL;
    dist = 0;
    nas = ai->pTargetNas;
    target = ai->TargetPlayer;
    ang = Heroes[arg0].hero_move->facing_angle;
    rnd = Random(0x100);
    ok = 0;
    if (nas != NULL) {
        if (nas->NASid != 0 && (neng = nas->Engine) != NULL && (fux = (EXTobject *)nas->FUX) != NULL &&
            (fux->status == 1 || fux->status == 4 || fux->status == 5)) {
            ok = 1;
        }
        if (ok) {
            e = Heroes[arg0].hero_eng;
            dx = neng->unk10.x - e->unk10.x;
            dz = neng->unk10.z - e->unk10.z;
            dist = dx * dx + dz * dz;
            ang = get_angle(dx, dz);
        } else {
            nas = NULL;
            ai->pTargetNas = NULL;
            ai->AImode = 1;
        }
    }
    ok = 0;
    if (target != -1) {
        if (target >= 4) {
            if (target == 6) {
                if (D_800C46B4->status != 1) {
                    ai->TargetPlayer = -1;
                    target = -1;
                    ai->AImode = 1;
                } else {
                    dx = D_800C46B0->unk10.x - Heroes[arg0].hero_eng->unk10.x;
                    dz = D_800C46B0->unk10.z - Heroes[arg0].hero_eng->unk10.z;
                    dist = dx * dx + dz * dz;
                    ang = get_angle(dx, dz);
                }
            } else {
                dx = D_800C3A04[target].x - Heroes[arg0].hero_eng->unk10.x;
                dz = D_800C3A04[target].z - Heroes[arg0].hero_eng->unk10.z;
                dist = dx * dx + dz * dz;
                ang = get_angle(dx, dz);
            }
        } else {
            if (!(Heroes[target].hero_status & 0xFA00)) {
                ok = 1;
            }
            if (ok) {
                dx = Heroes[target].hero_eng->unk10.x - Heroes[arg0].hero_eng->unk10.x;
                dz = Heroes[target].hero_eng->unk10.z - Heroes[arg0].hero_eng->unk10.z;
                dist = dx * dx + dz * dz;
                ang = get_angle(dx, dz);
            } else {
                ai->TargetPlayer = -1;
                target = -1;
                ai->AImode = 1;
            }
        }
    }
    BashAI_ScanAhead(arg0);
    switch (ai->AImode) {
    case 21:
        tmp = BashAI_EscapeAngle(arg0);
        if (tmp != 0) {
            u32 *t;

            ang = tmp;
            ai->AImode = 4;
            t = (u32 *)D_80051244;
            ai->AIang = ang;
            ai->AIpad |= t[(ang >> 9) & 7];
            break;
        }
        if (ai->AIsleep != 0) {
            break;
        }
        if (Heroes[arg0].hero_mode != 2) {
            ai->TargetPlayer = -1;
            ai->AImode = 1;
            ai->AIpad = 0;
            Heroes[arg0].hero_move->forward_speed = 0;
            break;
        }
        for (i = 0; i < 4; i++) {
            if (i != arg0 && Heroes[i].hero_ai->TargetPlayer == arg0) {
                dx = Heroes[i].hero_eng->unk10.x - Heroes[arg0].hero_eng->unk10.x;
                dz = Heroes[i].hero_eng->unk10.z - Heroes[arg0].hero_eng->unk10.z;
                if (dx * dx + dz * dz < 0x240000) {
                    ai->TargetPlayer = -1;
                    ai->AImode = 1;
                }
            }
        }
        goto steer;
    case 6:
        if (ai->AIsleep != 0) {
            break;
        }
        if (Heroes[arg0].hero_mode != 0) {
            break;
        }
        ai->AIsleep = 5;
        ai->AImode = 7;
        break;
    case 7:
        if (BashAI_HazardAhead(arg0) != 0) {
            ai->AIpad |= 0x100;
            break;
        }
        if (Heroes[arg0].hero_mode != 0) {
            break;
        }
        ai->AItimer2 = 0xB4;
        ai->AIsleep = 0x14;
        ai->TargetPlayer = -1;
        ai->pTargetNas = NULL;
        ai->AImode = 5;
        break;
    case 3:
        if (Heroes[arg0].hero_move->jump_max != 0) {
            goto steer;
        }
        if (nas != NULL) {
            ai->AImode = 2;
            break;
        }
        if (target == -1) {
            ai->AImode = 1;
            break;
        }
        if (target >= 5) {
            ai->AImode = 4;
            break;
        }
        tmp = BashAI_EscapeAngle(arg0);
        if (tmp != 0) {
            ang = tmp;
        }
        if (BashAI_AvoidAhead(arg0) != 0) {
            u32 *t;

            t = (u32 *)D_80051244;
            ai->AIang = ang;
            ai->AIpad |= t[(ang >> 9) & 7];
            goto done;
        }
        BashAI_CheckShot(arg0, mate, ang, (BashAiState *)ai, target);
        r = func_800864CC(arg0, 0x40);
        if (r == target) {
            ai->TargetPlayer = -1;
            ai->AImode = 1;
            ai->AIpad |= 0x8000;
        } else if (r != -1 && (D_8005A680[0] == 0 || r != mate) &&
                   (Heroes[r].hero_object != NULL || !(rnd & 0x10))) {
            ai->AIpad |= 0x8000;
        }
        fux = BashHeroObjectAhead(arg0, 3);
        if (fux == NULL) {
            u32 *t;

            t = (u32 *)D_80051244;
            ai->AIang = ang;
            ai->AIpad |= t[(ang >> 9) & 7];
            goto done;
        }
        ((PadStick *)D_80051338)[arg0].held &= ~0x2000;
        switch (fux->whatami) {
        case 3:
        case 4:
        case 5:
        case 7:
        case 14:
            if ((rnd & 1) == 0) {
                ai->AIpad |= 0x8000;
            } else {
                ai->AIpad |= 0x8000;
            }
            break;
        case 17:
        case 18:
        case 19:
            break;
        default:
            ai->AIpad |= 0x4000;
            break;
        }
        goto steer;
    case 2:
        if (Heroes[arg0].hero_move->jump_max != 0) {
            goto steer;
        }
        if (target != -1) {
            ai->AImode = 3;
            break;
        }
        if (nas == NULL) {
            ai->AImode = 1;
            break;
        }
        if (Heroes[arg0].hero_object != NULL) {
            ai->AImode = 10;
            break;
        }
        tmp = BashAI_EscapeAngle(arg0);
        if (tmp != 0) {
            ang = tmp;
        }
        if (Heroes[arg0].hero_eng->platform == nas->Engine) {
            ai->AImode = 1;
            break;
        }
        if (BashAI_AvoidAhead(arg0) != 0) {
            u32 *t;

            t = (u32 *)D_80051244;
            ai->AIang = ang;
            ai->AIpad |= t[(ang >> 9) & 7];
            goto done;
        }
        if (OVLG->mode == 3) {
            BashAI_CheckShot(arg0, mate, ang, (BashAiState *)ai, target);
        }
        r = func_800864CC(arg0, 0x40);
        if (r != target && (Heroes[r].hero_immunity | Heroes[r].hero_frozen) == 0 &&
            (OVLG->p2_active == 0 || r != mate) && Random(3) == 0) {
            ai->AIpad |= 0x8000;
        }
        fux = BashHeroObjectAhead(arg0, 3);
        if (fux == NULL) {
            u32 *t;

            t = (u32 *)D_80051244;
            ai->AIang = ang;
            ai->AIpad |= t[(ang >> 9) & 7];
            goto done;
        }
        ((PadStick *)D_80051338)[arg0].held &= ~0x2000;
        if (((NAS **)fux)[-1] == nas) {
            switch (fux->whatami) {
            case 2:
                ai->AIpad |= 0x8000;
                goto steer;
            case 3:
            case 4:
            case 5:
            case 17:
            case 18:
            case 19:
                ai->AImode = 1;
                ai->AIpad |= 0x2000;
                goto steer;
            case 7:
            case 14:
                ai->AImode = 1;
                ai->AIpad |= 0x8000;
                goto steer;
            }
        } else if ((nfux = (EXTobject *)nas->FUX) != NULL) {
            if (nfux->whatami == fux->whatami) {
                switch (nfux->whatami) {
                case 2:
                    ai->AIpad |= 0x8000;
                    goto steer;
                case 3:
                case 4:
                case 5:
                case 17:
                case 18:
                case 19:
                    ai->AImode = 1;
                    ai->AIpad |= 0x2000;
                    goto steer;
                case 7:
                case 14:
                    ai->AImode = 1;
                    ai->AIpad |= 0x8000;
                    goto steer;
                }
            } else {
                switch (fux->whatami) {
                case 2:
                case 3:
                case 4:
                case 5:
                case 7:
                case 14:
                    ai->AImode = 1;
                    ai->AIpad |= 0x8000;
                    goto steer;
                case 17:
                case 18:
                case 19:
                    goto steer;
                }
            }
        } else {
            switch (fux->whatami) {
            case 2:
            case 3:
            case 4:
            case 5:
            case 7:
            case 14:
                ai->AImode = 1;
                ai->AIpad |= 0x8000;
                goto steer;
            case 17:
            case 18:
            case 19:
                goto steer;
            }
        }
        ai->AIpad |= 0x4000;
        goto steer;
    case 4:
        if (target == -1) {
            ai->AImode = 1;
            break;
        }
        tmp = BashAI_EscapeAngle(arg0);
        if (tmp != 0) {
            ang = tmp;
        }
        if (BashAI_AvoidAhead(arg0) != 0) {
            u32 *t;

            t = (u32 *)D_80051244;
            ai->AIang = ang;
            ai->AIpad |= t[(ang >> 9) & 7];
            goto done;
        }
        BashAI_CheckShot(arg0, mate, ang, (BashAiState *)ai, -1);
        if (dist < 0x4000) {
            r = BashAI_PickVictim(arg0);
            if (r != -1) {
                ai->AImode = 0x15;
                ai->AIsleep = 4;
                ai->TargetPlayer = r;
                ai->AItimer2 = 0x12C;
                ai->AIpad |= 0x1000;
                break;
            }
            ai->AImode = 1;
            ai->TargetPlayer = r;
            ai->pTargetNas = NULL;
        }
        r = func_800864CC(arg0, 0x40);
        if (r != -1 && (D_8005A680[0] == 0 || r != mate)) {
            ai->TargetPlayer = -1;
            ai->AImode = 1;
            ai->AIpad |= 0x8000;
        }
        fux = BashHeroObjectAhead(arg0, 3);
        if (fux == NULL) {
            u32 *t;

            t = (u32 *)D_80051244;
            ai->AIang = ang;
            ai->AIpad |= t[(ang >> 9) & 7];
            goto done;
        }
        switch (fux->whatami) {
        case 3:
        case 4:
        case 5:
        case 7:
        case 14:
            ai->AIpad |= 0x8000;
            break;
        case 17:
        case 18:
        case 19:
            break;
        default:
            ai->AIpad |= 0x4000;
            break;
        }
        goto steer;
    case 5:
        if (ai->AIsleep != 0) {
            tmp = BashAI_EscapeAngle(arg0);
            if (tmp != 0) {
                ang = tmp;
            }
            if (BashAI_AvoidAhead(arg0) != 0) {
                break;
            }
            BashAI_CheckShot(arg0, mate, ang, (BashAiState *)ai, -1);
            r = func_800864CC(arg0, 0x40);
            if (r != -1 && (D_8005A680[0] == 0 || r != mate)) {
                ai->TargetPlayer = -1;
                ai->AImode = 1;
                ai->AIpad |= 0x8000;
            }
            fux = BashHeroObjectAhead(arg0, 3);
            if (fux != NULL) {
                switch (fux->whatami) {
                case 3:
                case 4:
                case 5:
                case 7:
                case 14:
                    ai->AIpad |= 0x8000;
                    break;
                case 17:
                case 18:
                case 19:
                    break;
                default:
                    ai->AIpad |= 0x4000;
                    break;
                }
            }
            tbl = (u32 *)D_80051244;
            ang = Heroes[arg0].hero_move->forward_angle + 0x800;
            ang &= 0xFFF;
            ai->AIang = ang;
            ai->AIpad |= tbl[(ang >> 9) & 7];
        } else {
            ai->TargetPlayer = -1;
            ai->pTargetNas = NULL;
            ai->AImode = 1;
        }
        break;
    case 12:
        if (target == -1 || nas != NULL) {
            ai->AImode = 10;
            break;
        }
        if (Heroes[arg0].hero_object == NULL) {
            ai->AImode = 1;
            ai->TargetPlayer = -1;
            ai->pTargetNas = NULL;
            break;
        }
        tmp = BashAI_EscapeAngle(arg0);
        if (tmp != 0) {
            ang = tmp;
        }
        if (BashAI_AvoidAhead(arg0) != 0) {
            u32 *t;

            t = (u32 *)D_80051244;
            ai->AIang = ang;
            ai->AIpad |= t[(ang >> 9) & 7];
            goto done;
        }
        r = func_800864CC(arg0, 0x40);
        if (r != -1 && (D_8005A680[0] == 0 || r != mate)) {
            ai->AImode = 13;
            ai->AIsleep = 5;
        }
        if (BashHeroObjectAhead(arg0, 3) != NULL) {
            ai->AImode = 13;
            ai->AIsleep = 5;
        }
        if (ai->AIsleep != 0) {
            goto steer;
        }
        d = csqrt(dist) >> 6;
        if (ai->AItimer1 != 0) {
            ang += 0x800;
            ang &= 0xFFF;
        } else {
            tmp = D_800C39F0[arg0] + ((0x10 - D_8009D87C[arg0].unk0) << 5);
            if (tmp < d) {
                if (d < tmp + 0x100) {
                    ai->AImode = 13;
                    ai->AIsleep = 5;
                }
            } else {
                if (Heroes[arg0].hero_eng->unk10.x < -0x780 || Heroes[arg0].hero_eng->unk10.x > 0x780 ||
                    Heroes[arg0].hero_eng->unk10.z > 0x780 || Heroes[arg0].hero_eng->unk10.z < -0x780) {
                    if (func_80015480(get_angle(Heroes[arg0].hero_eng->unk10.x, Heroes[arg0].hero_eng->unk10.z),
                                      Heroes[arg0].hero_move->forward_angle) > 0x400) {
                        ai->AImode = 13;
                        ai->AIsleep = 5;
                    }
                } else {
                    ang += 0x800;
                    ang &= 0xFFF;
                    ai->AItimer1 = 10;
                }
            }
        }
        obj = Heroes[arg0].hero_object;
        if (obj->whatami == 7 && obj->countdown < (s16)Random(4)) {
            ai->AImode = 13;
            ai->AIsleep = 5;
        }
        goto steer;
    case 13:
        if (Heroes[arg0].hero_weapon_count != 0) {
            if (Heroes[arg0].hero_object == NULL) {
                if (ai->AIsleep != 0) {
                    goto steer;
                }
                if (Heroes[arg0].hero_pu == 0x19 || Heroes[arg0].hero_pu == 0x1B) {
                    if (func_80015480(Heroes[arg0].hero_move->facing_angle + 0x800, ang) < 0x155) {
                        ai->AIpad |= 0x8000;
                    }
                } else {
                    ai->AIpad |= 0x8000;
                }
                goto steer;
            }
        } else if (Heroes[arg0].hero_object == NULL) {
            ai->AImode = 1;
            ai->TargetPlayer = -1;
            ai->pTargetNas = NULL;
            break;
        }
        if (ai->AIsleep == 0) {
            if ((Heroes[arg0].hero_mode == (u16)D_8009E0DC || Heroes[arg0].hero_mode == (u16)D_8009E0DE) &&
                func_80015480(Heroes[arg0].hero_move->facing_angle + 0x800, ang) < 0x155) {
                ai->AIpad |= 0x2000;
            }
        }
    steer:
        dirs = (u32 *)D_80051244;
        ai->AIang = ang;
        ai->AIpad |= dirs[(ang >> 9) & 7];
        break;
    case 10:
        if (Heroes[arg0].hero_object == NULL) {
            ai->AImode = 1;
        }
        r = BashAI_PickVictim(arg0);
        if (r != -1) {
            ai->TargetPlayer = r;
            ai->pTargetNas = NULL;
            ai->AImode = 12;
        }
        break;
    default:
        ai->AIpad = 0;
        if (Heroes[arg0].hero_object != NULL) {
            ai->AImode = 10;
        }
        if (Heroes[arg0].hero_mode != 0) {
            break;
        }
        if (ai->AIinitial_delay == 0 && ai->AItimer2 == 0 &&
            BashAI_NearestEnemy(arg0) > (D_8009D87C[arg0].unk0 << 5) + 0x400) {
            r = BashAI_PickVictim(arg0);
            if (r != -1) {
                ai->AImode = 0x15;
                ai->AIsleep = 4;
                ai->TargetPlayer = r;
                ai->AItimer2 = 0x12C;
                ai->AIpad |= 0x1000;
                break;
            }
        }
        if (D_8005A648 == 3) {
            BashAI_CheckShot(arg0, mate, ang, (BashAiState *)ai, -1);
        }
        if (ai->AIsleep == 0 && BashAI_ChooseTarget(arg0) != 0) {
            if (ai->pTargetNas != NULL) {
                ai->AImode = 2;
            } else {
                ai->AImode = 3;
            }
            ai->AIaborted_mode = 0;
        }
        break;
    }
done:
    pad = &((PadStick *)D_80051338)[arg0];
    set_joypad((u8 *)pad, ai->AIpad);
    pad->angle = ai->AIang;
}

void BashAI_ScanAhead(s32 arg0) {
    EXTengine *eng;
    EXTheromove *mv;
    s32 rx;
    s32 rz;
    s32 lx;
    s32 lz;
    s32 dx;
    s32 dz;
    s32 ang;
    s32 c;
    s32 s;
    s32 gx;
    s32 gz;
    s32 id;
    s32 i;
    u8 v;

    eng = Heroes[arg0].hero_eng;
    rx = eng->unk10.x;
    rz = eng->unk10.z;
    mv = Heroes[arg0].hero_move;
    dx = D_80068BD4[mv->forward_angle & 0xFFF].unk2 * 15 >> 9;
    ang = mv->forward_angle;
    c = D_80068BD4[(ang + 0x400) & 0xFFF].unk2;
    s = D_80068BD4[(ang + 0x800) & 0xFFF].unk2;
    dz = c * 15 >> 9;
    lx = rx + dx;
    rx = rx - dx;
    lz = rz + dz;
    rz = rz - dz;
    dx = c * 11 >> 7;
    dz = s * 11 >> 7;
    lx += dx;
    lz += dz;
    rx += dx;
    rz += dz;
    dx = c * 256 >> 12;
    dz = s * 256 >> 12;
    id = (arg0 << 5) | 0x80;
    i = 0;
    while ((u32)(lx + 0x7FF) < 0xFFF && lz < 0x800 && lz >= -0x7FF && i < 8) {
        gx = (lx >> 8) + 10;
        gz = 9 - (lz >> 8);
        v = D_8009D90C[gz * 20 + gx].unk0;
        if ((v & 0xF0) == id) {
            v &= 0x1F;
        }
        lx += dx;
        lz += dz;
        Heroes[arg0].hero_ai->AIforward_left[i] = v;
        i++;
    }
    for (; i < 8; i++) {
        Heroes[arg0].hero_ai->AIforward_left[i] = 0xFE;
    }
    i = 0;
    while ((u32)(rx + 0x7FF) < 0xFFF && rz < 0x800 && rz >= -0x7FF && i < 8) {
        gx = (rx >> 8) + 10;
        gz = 9 - (rz >> 8);
        v = D_8009D90C[gz * 20 + gx].unk0;
        if ((v & 0xF0) == id) {
            v &= 0x1F;
        }
        rx += dx;
        rz += dz;
        Heroes[arg0].hero_ai->AIforward_right[i] = v;
        i++;
    }
    for (; i < 8; i++) {
        Heroes[arg0].hero_ai->AIforward_right[i] = 0xFE;
    }
}

s32 BashAI_AvoidAhead(s32 arg0) {
    EXTbotai *ai;
    u16 w[3];
    s32 i;
    s32 l;
    s32 r;
    s32 a;
    s32 b;
    u16 *p;
    s32 ff;
    s32 fe;

    ai = Heroes[arg0].hero_ai;
    i = 0;
    ff = 0xFF;
    fe = 0xFE;
    p = w;
    for (; i < 3; i++) {
        *p = 0;
        a = ai->AIforward_left[i];
        b = ai->AIforward_right[i];
        l = a & 0x3F;
        r = b & 0x3F;
        if (a == ff || b == ff) {
            *p = ff;
        }
        if (a == fe || b == fe) {
            *p = fe;
        }
        if (l >= 6) {
            if (l < 8) {
                goto setl;
            }
            if (l < 0x16) {
                if (l >= 0x14) {
                setl:
                    *p = ff;
                }
            }
        }
        if (r >= 6) {
            if (r < 8) {
                goto setr;
            }
            if (r < 0x16) {
                if (r >= 0x14) {
                setr:
                    *p = ff;
                }
            }
        }
        p++;
    }
    if (w[0] != 0) {
        if (Heroes[arg0].hero_object != NULL) {
            Heroes[arg0].hero_move->velocity_enable = 0;
            ai->AImode = 0xD;
            return 1;
        }
        if (w[1] != 0 || w[2] != 0) {
            Heroes[arg0].hero_move->forward_angle += 0x31C;
            ai->AIaborted_mode = ai->AImode;
            ai->AImode = 5;
            ai->AIsleep = 0x1E;
            ai->TargetPlayer = -1;
            ai->pTargetNas = NULL;
            return 1;
        }
        Heroes[arg0].hero_move->velocity_enable = 1;
        ai->AIpad |= 0x4000;
    }
    return 0;
}

s32 BashAI_ChooseTarget(s32 player) {
    NAS *bestNas;
    s32 bestHeroScore;
    s32 r;
    s32 bestHero;
    s32 bestCell;
    s32 px;
    s32 mate;
    s32 pz;
    s32 wRnd;
    s32 bestNasScore;
    EXTbotai *ai;
    EXTengine *e;
    EXTengine *he;
    NAS *n;
    EXTobject *f;
    EXTobject *obj;
    u16 wNear;
    u16 wFar;
    u16 wHero;
    s16 wHealth;
    s16 wNas;
    s16 w;
    s32 lvl;
    s32 inv;
    s32 rnd;
    s32 d;
    s32 dh;
    s32 dx;
    s32 dz;
    s32 d2;
    s32 cnt;
    s32 i;
    s32 k;
    s32 wt;

    n = D_8005AA24;
    bestNas = NULL;
    bestHero = -1;
    bestCell = -1;
    bestHeroScore = 0;
    mate = Heroes[player].hero_teammate;
    he = Heroes[player].hero_eng;
    ai = Heroes[player].hero_ai;
    px = he->unk10.x;
    pz = he->unk10.z;
    Random(0x100);
    bestNasScore = 0;
    if (BashAI_NearestEnemy(player) < 0x300) {
        wNear = 0;
        wFar = 8;
    } else {
        wNear = 8;
        wFar = 0;
    }
    r = Random(0x1000);
    if (Heroes[player].hero_weapon_count != 0) {
        wHero = 0xA0;
        switch (Heroes[player].hero_pu) {
        case 0x19:
        case 0x1B:
            wNear = 0;
            wFar = 0xA;
            break;
        case 0x1C:
        default:
            wNear = 0xA;
            wFar = 0;
            break;
        }
    } else {
        wHero = 0;
    }
    wNas = 0x20;
    lvl = D_8009D87C[player].unk0;
    i = Heroes[player].hero_health;
    w = ((lvl - 0x10) * 2) | 1;
    if (i < 0) {
        i = 0;
    }
    if (i > 0x14) {
        i = 0x14;
    }
    inv = 0x10 - lvl;
    wHealth = D_800C3AC4[i] + inv * 4;
    if (Heroes[player].hero_pu == 0x14) {
        wHero = 0xA0;
        wNas = 0;
        w = wNear = 0x200;
        wFar = 0;
        wHealth = 0;
    }
    i = 0x180 - inv * 0x10;
    wRnd = i;
    switch (D_8005A648) {
    case 1:
        if (Random(3) == 0) {
            rnd = (s16)i * wHealth + 0x2000;
        } else {
            rnd = 0;
        }
        break;
    case 2:
        if (Random(4) == 0) {
            rnd = (s16)i * wHealth + 0x3000;
        } else {
            rnd = 0;
        }
        break;
    case 3:
        if (Random(8) == 0) {
            rnd = (s16)i * wHealth + 0x4000;
        } else {
            rnd = 0;
        }
        break;
    default:
        if (Random(6) == 0) {
            rnd = (s16)wRnd * wHealth + 0x4000;
        } else {
            rnd = 0;
        }
        break;
    }
    if (ai->AIinitial_delay != 0) {
        r = Random(0x400);
        w = 0;
        wNear = 0;
        wFar = 0;
    }
    if (n != NULL) {
    do {
        wRnd = wNas;
        wt = wRnd + 8;
        e = n->Engine;
        if (e == NULL) continue;
        f = (EXTobject *)n->FUX;
        if (f == NULL) continue;
        switch (f->whatami) {
        case 1: case 3: case 4: case 5:
            if (f->status != 1) continue;
            dx = px - e->unk10.x;
            dz = pz - e->unk10.z;
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = (0x1000 - abs(d - r)) * wt;
            cnt = 0;
            for (i = 0; i < 4; i++) {
                if (ai->pTargetNas == n) cnt += 2;
            }
            if (cnt != 0) d >>= cnt;
            if (bestNasScore < d) {
                bestNas = n;
                bestNasScore = d;
            }
            continue;
        case 14:
            if (f->status != 1) continue;
            dx = px - e->unk10.x;
            dz = pz - e->unk10.z;
            d2 = csqrt(dx * dx + dz * dz) >> 6;
            d = 0x800 - d2;
            if (d < 0) d = 0;
            d *= wNear + wRnd;
            d2 *= wFar | wRnd;
            cnt = 0;
            for (i = 0; i < 4; i++) {
                if (ai->pTargetNas == n) cnt += 2;
            }
            if (cnt != 0) {
                d >>= cnt;
                d2 >>= cnt;
            }
            if (bestNasScore < d) {
                bestNas = n;
                bestNasScore = d;
            }
            if (bestNasScore < d2) {
                bestNas = n;
                bestNasScore = d2;
            }
            continue;
        case 18: case 19: case 25: case 27: case 28:
            dx = px - e->unk10.x;
            dz = pz - e->unk10.z;
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * 0xFA;
            cnt = 0;
            for (i = 0; i < 4; i++) {
                if (ai->pTargetNas == n) cnt += 2;
            }
            if (cnt != 0) d >>= cnt;
            if (bestNasScore < d) {
                bestNas = n;
                bestNasScore = d;
            }
            continue;
        case 2:
            dx = px - e->unk10.x;
            dz = pz - e->unk10.z;
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * 0x190;
            if (Heroes[player].hero_pu != 0) {
                d >>= 8;
            }
            cnt = 0;
            for (i = 0; i < 4; i++) {
                if (ai->pTargetNas == n) cnt += 1;
            }
            if (cnt != 0) d >>= cnt;
            if (bestNasScore < d) {
                bestNas = n;
                bestNasScore = d;
            }
            continue;
        case 17:
            dx = px - e->unk10.x;
            dz = pz - e->unk10.z;
            d2 = csqrt(dx * dx + dz * dz) >> 6;
            d = 0xE00 - d2;
            if (d < 0) d = 0;
            cnt = 0;
            d *= wNear + wHealth;
            for (i = 0; i < 4; i++) {
                if (ai->pTargetNas == n) cnt += 3;
            }
            if (cnt != 0) d >>= cnt;
            if (bestNasScore < d) {
                bestNas = n;
                bestNasScore = d;
            }
            continue;
        default:
            continue;
        }
    } while ((n = n->Object.Child) != NULL);
    }
    for (i = 0; i < 4; i++) {
        s32 wn = (s16)wNear;
        wRnd = (s16)wHero;
        if (i == player) continue;
        if (OVLG->p2_active != 0 && i == mate) continue;
        if (Heroes[i].hero_status & 0xFA00) continue;
        do {
            dx = px - Heroes[i].hero_eng->unk10.x;
            dz = pz - Heroes[i].hero_eng->unk10.z;
        } while (0);
        if (Heroes[player].hero_pu != 0x14) {
            obj = Heroes[i].hero_object;
            if (obj != NULL) {
                if (OVLG->mode == 1) {
                    if (obj->whatami == 7) {
                        w += 1;
                    } else {
                        w += 5;
                    }
                } else if (obj->whatami == 7) {
                    w += 8;
                } else {
                    w += 0x10;
                }
            } else {
                w = 0;
            }
        }
        if (Heroes[i].hero_mode == 2) {
            w += 0x20;
        }
        d2 = csqrt(dx * dx + dz * dz) >> 6;
        dh = 0x800 - d2;
        if (dh < 0) dh = 0;
        dh *= wn + w + wRnd;
        d2 *= wFar - w - wRnd;
        cnt = 0;
        for (k = 0; k < 4; k++) {
            if (ai->TargetPlayer == k) cnt += 6;
        }
        if (cnt != 0) {
            dh >>= cnt;
            d2 >>= cnt;
        }
        if (bestHeroScore < dh) {
            bestHero = i;
            bestHeroScore = dh;
        }
        if (bestHeroScore < d2) {
            bestHero = i;
            bestHeroScore = d2;
        }
    }
    switch (ai->AIaborted_mode) {
    case 2:
        bestNasScore = 0;
        break;
    case 3:
        bestHeroScore = 0;
        break;
    case 4:
        rnd = 0;
        break;
    }
    if (bestNasScore < rnd && bestHeroScore < rnd) {
        d2 = 0;
        for (i = 7; i < 0x10; i++) {
            d = 0x7FFFFF;
            for (k = 0; k < 4; k++) {
                if (k == player) continue;
                dx = D_800C3A04[i].x - Heroes[k].hero_eng->unk10.x;
                dz = D_800C3A04[i].z - Heroes[k].hero_eng->unk10.z;
                if (dx * dx + dz * dz < d) {
                    d = d2;
                }
            }
            if (d2 < d) {
                d2 = d;
                bestCell = i;
            }
        }
        ai->TargetPlayer = bestCell;
        ai->pTargetNas = NULL;
        return 1;
    }
    if (bestHeroScore < bestNasScore) {
        ai->pTargetNas = bestNas;
        ai->TargetPlayer = -1;
        return 1;
    }
    if (bestHeroScore > 0x14) {
        ai->TargetPlayer = bestHero;
        ai->pTargetNas = NULL;
        return 1;
    }
    return 0;
}

s32 BashAI_PickVictim(s32 arg0) {
    s32 i;
    s32 sum;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
        }
    }
    if (D_800C46B4 != NULL && (u32)arg0 < 4 && D_800C46B4->status == 1) {
        if (Random(10) == 0) {
            return 6;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

void FinishBashLevel(void) {
    HeroInfo *h;
    s32 i;
    s32 best;
    s32 max;

    max = 0;
    best = -1;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (!(h->hero_status & 0xFA00) && max < h->hero_health) {
            max = h->hero_health;
            best = i;
        }
    }
    if (best != -1) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00) && i != best) {
                func_80084E04(i, 0, -1, 0x1000, 0);
            }
        }
    }
}

s32 BashAI_NearestEnemy(s32 arg0) {
    HeroInfo *base;
    HeroInfo *o;
    HeroInfo *h;
    EXTengine *a;
    EXTengine *b;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 min;

    u8 *g;
    s32 mate;
    s32 r;

    mate = Heroes[arg0].hero_teammate;
    min = 0x7FFFFF;
    i = 0;
    g = D_8005A614;
    base = Heroes;
    h = &Heroes[arg0];
    for (i = 0; (u16)i < 4; i++) {
        if ((u16)i != arg0 && (*(s32 *)(g + 0x6C) == 0 || (u16)i != mate)) {
            o = (HeroInfo *)((u16)i * sizeof(HeroInfo) + (u32)base);
            if (!(o->hero_status & 0xFA00)) {
                a = h->hero_eng;
                b = o->hero_eng;
                dx = a->unk10.x - b->unk10.x;
                dz = a->unk10.z - b->unk10.z;
                d = dx * dx + dz * dz;
                if (d < min) {
                    min = d;
                }
            }
        }
    }
    if (min != 0x7FFFFF) {
        r = csqrt(min) >> 6;
    } else {
        r = 0;
    }
    return r;
}

void BashSetCrateMix(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4, u16 arg5,
                   u16 arg6, u16 arg7, u16 arg8) {
    s32 i;
    s32 k;
    s32 b;
    u16 c;
    u8 t;

    D_800C3B94[3] = arg1 * arg6 / 16 + 1;
    D_800C3B94[2] = arg2 * arg6 / 16 + 1;
    D_800C3B94[1] = arg3 * arg6 / 16 + 1;
    D_800C3B94[0] = arg3 * arg6 / 16 + 1;
    D_800C3B88 = arg0;
    D_800C3B8C[3] = arg1;
    D_800C3B8C[2] = arg2;
    D_800C3B8C[0] = D_800C3B8C[1] = arg3;
    arg4 *= 4;
    arg5 *= 4;
    c = arg6 * 4;
    arg7 *= 4;
    arg8 *= 4;
    k = 0;
    for (i = 0; i < arg4; i++) {
        D_800C3B44[k++] = 4;
    }
    for (i = 0; i < arg5; i++) {
        D_800C3B44[k++] = 5;
    }
    for (i = 0; i < c; i++) {
        D_800C3B44[k++] = 6;
    }
    for (i = 0; i < arg7; i++) {
        D_800C3B44[k++] = 1;
    }
    for (i = 0; i < arg8; i++) {
        D_800C3B44[k++] = 2;
    }
    for (i = 0; i < 600; i++) {
        k = Random(0x40);
        b = Random(0x40);
        t = D_800C3B44[k];
        D_800C3B44[k] = D_800C3B44[b];
        D_800C3B44[b] = t;
    }
    D_800C3B84 = Random(0x20) + 0x14;
}

void BashCrateMaster(void) {
    u32 n;
    s32 left;
    BashGridCell *c;
    s32 pu;
    s32 i;
    s32 s;
    s32 r;
    s32 z;
    u8 **pp;
    BashEdgeSub *sub;

    if (OVLG->pause & 0x801) {
        return;
    }
    if (D_800C3B32 != 0) {
        D_800C3B32--;
    } else {
        if (D_8009E46C[0] < D_800C3B8C[D_8009D758 - 1]) {
            n = D_800C3B8C[D_8009D758 - 1];
            left = n - D_8009E46C[0];
            r = 4;
            if ((u16)n < 4) {
                r = n;
            }
            if (left < r) {
                pu = Random(left) + 1;
                D_800C3B32 = Random(20) + 8;
            } else {
                pu = r;
                D_800C3B32 = Random(10) + 2;
            }
            BashDropCrates(pu);
        } else {
            D_800C3B32 = Random(40) + 5;
        }
    }
    if (D_800C3B3C != 0) {
        D_800C3B3C--;
    }
    if (D_8009E25C[0] < 3) {
        if (D_800C3B34 != 0) {
            D_800C3B34--;
        } else {
            c = (BashGridCell *)BashFindDropPoint(D_800C3B86);
            if (c != NULL) {
                CreateNAS(c->x, World_floor_level, c->z, 0x20C, 0x11);
            }
            D_800C3B34 = Random(30) + 10;
        }
    }
    if (D_8005A648 != 3) {
        if (D_800C3B38 != 0) {
            if (D_800C3B3A != 0) {
                D_800C3B3A--;
            } else {
                c = D_800C3EAC[0];
                c += Random(256);
                s = (c->x >> 8) + 10;
                z = 9 - (c->z >> 8);
                if (D_8009D90C[z * 20 + s].unk0 < 16) {
                    CreateNAS(c->x, -0x800, c->z, 0x20C, 0x11);
                    D_800C3B38--;
                    D_800C3B3A = Random(4) + 2;
                }
            }
        } else if (D_800C3B3C == 0) {
            if (D_8009E25C[0] < 5) {
                D_800C3B38 = Random(4) + 6;
                D_800C3B3C = (Random(20) + 30) * 30;
            } else {
                D_800C3B3C = 60;
            }
        }
    }
    if (D_8005A648 == 1 || D_8005A648 == 2) {
        if (D_800C478A != 0) {
            D_800C478A--;
        } else if (D_8009E25C[18] + D_8009E25C[19] + D_8009E25C[20] + D_8009E25C[21] == 0) {
            pu = Random(4) + 18;
            for (i = 0; i < 4; i++) {
                if (Heroes[i].hero_pu == pu) {
                    pu = 0;
                }
            }
            if (pu != 0) {
                c = D_800C3EAC[0];
                c += Random(256);
                s = (c->x >> 8) + 10;
                z = 9 - (c->z >> 8);
                if (D_8009D90C[z * 20 + s].unk0 == 0) {
                    switch (pu) {
                    case 18:
                        CreateNAS(c->x, -0x800, c->z, 0x208, 0x11);
                        D_800C478A = (Random(10) + 10) * 30;
                        break;
                    case 19:
                        CreateNAS(c->x, -0x800, c->z, 0x209, 0x11);
                        D_800C478A = (Random(10) + 10) * 30;
                        break;
                    case 20:
                        CreateNAS(c->x, -0x800, c->z, 0x20A, 0x11);
                        D_800C478A = (Random(10) + 10) * 30;
                        break;
                    case 21:
                        CreateNAS(c->x, -0x800, c->z, 0x20B, 0x11);
                        D_800C478A = (Random(10) + 10) * 30;
                        break;
                    }
                }
            }
        } else {
            D_800C478A = (Random(10) + 5) * 30;
        }
    }
    if (D_8005A648 == 3) {
        if (D_800C478A != 0) {
            D_800C478A--;
        } else if (D_8009E46C[2] == 0) {
            s = (Random(12) << 8) - 0x580;
            D_800C3B9C[0] = BashSpawnEdge(-s, -0x700, 1, 0);
            D_800C3B9C[1] = BashSpawnEdge(s, 0x600, 1, 0);
            D_800C3B9C[2] = BashSpawnEdge(-0x700, s, 0, 1);
            D_800C3B9C[3] = BashSpawnEdge(0x600, -s, 0, 1);
            s = Random(4);
            pp = D_800C3B9C;
            for (i = 0; i < 4; i++, pp++) {
                if (*pp != NULL) {
                    sub = ((BashEdgeObj *)*pp)->unk58;
                    if (sub != NULL) {
                        if (i == s) {
                            sub->unk24 = D_800C46C0[D_800C4788];
                            D_800C4788++;
                            D_800C4788 %= 18;
                        } else {
                            sub->unk24 = 17;
                        }
                    }
                }
            }
        }
    }
    if (OVLG->mode == 0 && (OVLG->flags & 0x80)) {
        if (D_800C46AC != 0) {
            D_800C46AC--;
        } else {
            if (!(OVLG->pause & 0x801)) {
                CreateNAS(0, 0, 0, 0x1107, 0x11);
            }
            D_800C46AC = Random(90) + 30;
        }
    }
}

void BashDropCrates(u16 arg0) {
    s32 i;
    BashPair *p;
    s32 t;
    s32 c;
    s32 a;
    s32 b;

    for (i = 0;; i++) {
        if (i >= arg0) {
            return;
        }
        p = BashFindDropPoint(D_800C3B86);
        if (p != NULL) {
            a = p->unk0;
            b = p->unk4;
            c = D_800C3B44[D_800C3B84];
            D_800C3B84 = (D_800C3B84 + 1) & 0x3F;
            t = c;
            if (t == 6) {
                if (!(OVLG->flags & 0x80) || OVLG->mode != 1) {
                    if (D_8009E46C[6] >= D_800C3B94[D_8009D758 - 1]) {
                        t = 4;
                    }
                }
            }
            if (OVLG->mode == 3 && t == 4) {
                t = 3;
            }
            CreateCrate(a, 0, b, t, -1);
        }
    }
}

void BashShuffleDropPoints(void) {
    s32 i;
    s32 j;
    s32 x;
    s32 z;
    s32 n;
    s32 tx;
    s32 tz;

    if (D_8005A648 == 3) {
        for (i = 0; i < 16; i++) {
            z = i * 256 - 0x780;
            for (j = 0; j < 16; j++) {
                x = j * 256 - 0x780;
                if ((u32)(i - 2) >= 12) {
                    z = Random(11) * 256 - 0x580;
                }
                if ((u32)(j - 2) >= 12) {
                    x = Random(11) * 256 - 0x580;
                }
                D_800C3EAC[i][j].x = x;
                D_800C3EAC[i][j].z = z;
            }
        }
        n = 0;
    } else {
        for (i = 0; i < 16; i++) {
            z = i * 256 - 0x780;
            for (j = 0; j < 16; j++) {
                x = j * 256 - 0x780;
                D_800C3EAC[i][j].x = x;
                D_800C3EAC[i][j].z = z;
            }
        }
        n = 0;
    }
    do {
        i = Random(16);
        j = Random(16);
        tx = Random(16);
        tz = Random(16);
        x = D_800C3EAC[i][j].x;
        z = D_800C3EAC[i][j].z;
        D_800C3EAC[i][j].x = D_800C3EAC[tx][tz].x;
        D_800C3EAC[i][j].z = D_800C3EAC[tx][tz].z;
        D_800C3EAC[tx][tz].x = x;
        D_800C3EAC[tx][tz].z = z;
        n++;
    } while (n < 0x258);
}

BashPair *BashFindDropPoint(u8 *arg0) {
    BashGridCell *best;
    BashGridCell *cell;
    u16 bestn;
    u16 tries;
    u16 besti;
    s32 i;
    s32 x;
    s32 z;
    s32 xlo;
    s32 xhi;
    s32 zlo;
    s32 zhi;
    u16 n;

    cell = D_800C3EAC[0];
    bestn = 9;
    tries = 9;
    i = *(u16 *)arg0;
    best = cell = &cell[i];
    besti = i;
    i++;
    i &= 0xFF;
    x = (cell->x >> 8) + 10;
    z = 9 - (cell->z >> 8);
    do {
        while (D_8009D90C[z * 20 + x].unk0 != 0) {
            if (i == *(u16 *)arg0) {
                return NULL;
            }
            cell = D_800C3EAC[0];
            cell = &cell[i];
            i++;
            i &= 0xFF;
            x = (cell->x >> 8) + 10;
            z = 9 - (cell->z >> 8);
        }
        xhi = x;
        xlo = x;
        zhi = z;
        zlo = z;
        if (x > 0) {
            xlo = x - 1;
        }
        if (x < 19) {
            xhi = x + 1;
        }
        if (z > 0) {
            zlo = z - 1;
        }
        if (z < 19) {
            zhi = z + 1;
        }
        n = D_8009D90C[zlo * 20 + xlo].unk0 != 0;
        if (D_8009D90C[zlo * 20 + x].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[zlo * 20 + xhi].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[z * 20 + xlo].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[z * 20 + xhi].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[zhi * 20 + xlo].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[zhi * 20 + x].unk0 != 0) {
            n++;
        }
        if (D_8009D90C[zhi * 20 + xhi].unk0 != 0) {
            n++;
        }
        if (n == 0) {
            *(u16 *)arg0 = i;
            return (BashPair *)cell;
        }
        if (n < bestn) {
            bestn = n;
            best = cell;
            besti = i;
        }
        if (i == *(u16 *)arg0) {
            return NULL;
        }
        cell = D_800C3EAC[0];
        cell = &cell[i];
        i++;
        i &= 0xFF;
        x = (cell->x >> 8) + 10;
        z = 9 - (cell->z >> 8);
        if (--tries == 0xFFFF) {
            break;
        }
    } while (1);
    *(u16 *)arg0 = besti;
    return (BashPair *)best;
}

void BashSpaceInitTiles(void) {
    s32 j;
    s32 i;
    BashCell *p;
    BashObj *obj;
    u16 *id;
    s16 v;
    BashCell *base;
    BashCell *base2;
    s32 x;
    s32 z;
    s32 off;
    s32 colOff;

    z = 0;
    x = 0;
    obj = (BashObj *)((GfxScene *)D_80058AD0)->objs;
    func_80018430(200);
    if (obj != NULL) {
        base = D_800C3BAC[0];
        do {
            id = (u16 *)func_80015870(obj->model, obj->index);
            if ((u16)(*id - 10) < 78) {
                v = *id;
                i = 7 - (s16)(v % 10);
                j = (s16)(v / 10) - 1;
                p = (BashCell *)(i * sizeof(BashCell) + j * sizeof(D_800C3BAC[0]) + (u32)base);
                p->obj = obj;
                func_8001C3D0((u8 *)obj);
                obj->flags |= 0x1000000;
            }
            if (*(s16 *)id == 10) {
                x = obj->x;
                z = obj->z;
            }
            obj = obj->next;
        } while (obj != NULL);
    }
    if (x | z) {
        obj = (BashObj *)((GfxScene *)D_80058AD0)->objs;
        if (obj != NULL) {
            do {
                if (*(s16 *)func_80015870(obj->model, obj->index) == 5) {
                    j = obj->x - x;
                    i = obj->z - z;
                    j = (j + 0x80) >> 9;
                    i = ((i + 0x80) >> 9) + 7;
                    obj->flags = (obj->flags | 0x1000000) & ~0x8000;
                    D_800C3BAC[j][i].obj2 = obj;
                }
                obj = obj->next;
            } while (obj != NULL);
        }
    }
    i = 0;
    base2 = D_800C3BAC[0];
    for (colOff = 0; i < 8; i++, colOff += sizeof(BashCell)) {
        for (j = 0, off = colOff; j < 8; j++, off += sizeof(D_800C3BAC[0])) {
            p = (BashCell *)(off + (u32)base2);
            obj = p->obj;
            if (obj != NULL) {
                p->unkA = obj->index;
                p->unk8 = 0;
            } else {
                p->unkA = 0;
                p->unk8 |= 0x100;
            }
        }
    }
}

void BashSpaceTileEdges(void) {
    s32 i;
    s32 j;
    BashObj *obj;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            obj = D_800C3BAC[i][j].obj2;
            if (obj != NULL) {
                obj->flags &= ~0x8000;
                if (D_800C3BAC[i][j].unkA != 0) {
                    if (i > 0 && D_800C3BAC[i - 1][j].unkA == 0) {
                        obj->flags |= 0x01008000;
                    }
                    if (i < 7 && D_800C3BAC[i + 1][j].unkA == 0) {
                        obj->flags |= 0x01008000;
                    }
                    if (j > 0 && D_800C3BAC[i][j - 1].unkA == 0) {
                        obj->flags |= 0x01008000;
                    }
                }
            }
        }
    }
}

void PlotSnowGlint(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    SnowGlint *g;
    s32 i;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        g = FindExtension(nas, 0x20000002);
        if (g != NULL && sub != NULL) {
            i = 0;
            do {
                if (g->on[i] == 0) {
                    if (g->timer[i] != 0) {
                        g->timer[i]--;
                        sub->flags &= 0xFFFF7FFF;
                    } else {
                        sub->unk04 = Random(0xF00) - 0x780;
                        sub->unk0C = Random(0xF00) - 0x780;
                        sub->flags |= 0x8000;
                        g->timer[i] = Random(3);
                        g->on[i] = 1;
                    }
                } else {
                    if (g->timer[i] != 0) {
                        g->timer[i]--;
                    } else {
                        g->timer[i] = Random(0xF);
                        g->on[i] = 0;
                    }
                }
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
    }
}

void PlotBashPenguin(NAS *nas) {
    EXTengine *eng;
    EXTanimator *anim;
    PenguinFux *fux;
    s32 t;
    s32 r;
    s32 ang;
    s32 d;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    anim = FindExtension(nas, 0x400);
    if (anim != NULL) {
        fux = (PenguinFux *)nas->FUX;
        if (fux != NULL) {
            switch (fux->state) {
            case 0:
                if (D_8005A644 & 0x801) {
                    goto stop;
                }
                fux->state = 1;
                fux->unk1C |= 0xA;
                eng->unk1E = 0xE3;
                eng->unk10.x = 0;
                eng->unk10.z = 0;
                func_800250B0(nas);
                break;
            case 2:
                if (D_8005A644 & 0x801) {
                    GotoLinkAnim(anim, 0);
                    goto stop;
                }
                if (func_800250B0(nas) == 8) {
                    fux->state = 3;
                    nas->UserInfo.user1 = 0x800;
                    PlaySfx(0x427, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
                }
                break;
            case 3:
                if (D_8005A644 & 0x801) {
                    /* a value-returning call: keeps it from being cross-jumped into case 2's */
                    ((s32 (*)(EXTanimator *, s16))GotoLinkAnim)(anim, 0);
                    goto stop;
                }
                t = nas->UserInfo.user1;
                if (t > 0) {
                    r = (D_80068BD4[(t - 0x400) & 0xFFF].unk2 * 3) >> 3;
                    if (r > 0x500) {
                        r = 0x500;
                    }
                    nas->UserInfo.user1 = t - 4;
                    d = ((0x500 - abs(r)) >> 6) + 0x28;
                    ang = (nas->UserInfo.user2 + d) & 0xFFF;
                    nas->UserInfo.user2 = ang;
                    eng->unk10.x = (r * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                    eng->unk10.z = (r * D_80068BD4[ang].unk2) >> 12;
                    eng->sub->unk74 = 0x2002;
                } else {
                    GotoLinkAnim(anim, 2);
                    fux->state = 0;
                    fux->unk1E &= 0xFFF5;
                    if (eng->sfx != NULL) {
                        StopSfx(eng->sfx);
                    }
                    func_800250B0(nas);
                }
                BashPenguinPush(eng);
                eng->unk1E += 0x1C7;
                break;
            case 4:
                eng->unk1E = 0xE3;
                func_800250B0(nas);
                break;
            case 1:
            default:
                if (D_8005A644 & 0x801) {
                stop:
                    fux->state = 4;
                    if (eng->sfx != NULL) {
                        StopSfx(eng->sfx);
                    }
                    break;
                }
                if (fux->unk1E & 2) {
                    fux->state = 2;
                    fux->unk22 = 0x3C;
                    GotoLinkAnim(anim, 1);
                    PlaySfx(0x426, NULL, NULL, 0x1000, 0x1E00, 0);
                    D_800C3B38 = 0;
                    D_800C3B3C += 0x258;
                }
                func_800250B0(nas);
                break;
            }
        }
    }
    eng->unk5E = 1;
}

void BashSnowMarkPenguin(void) {
    s32 lo, hi;
    u32 x, z;
    u32 xhi, zhi;

    lo = 8;
    hi = 12;
    for (x = lo; (s32)x < hi; x++) {
        for (z = lo; (s32)z < hi; z++) {
            D_8009D90C[z * 20 + x].unk0 = 0xF0;
        }
    }
    x = ((D_800C46B0->unk10.x - 0x80) >> 8) + 10;
    z = 9 - ((D_800C46B0->unk10.z - 0x80) >> 8);
    xhi = ((D_800C46B0->unk10.x + 0x80) >> 8) + 10;
    zhi = 9 - ((D_800C46B0->unk10.z + 0x80) >> 8);
    if (x < 20) {
        if (z < 20) {
            D_8009D90C[z * 20 + x].unk0 |= 0x40;
        }
        if (zhi < 20) {
            D_8009D90C[zhi * 20 + x].unk0 |= 0x40;
        }
    }
    if (xhi < 20) {
        if (z < 20) {
            D_8009D90C[z * 20 + xhi].unk0 |= 0x40;
        }
        if (zhi < 20) {
            D_8009D90C[zhi * 20 + xhi].unk0 |= 0x40;
        }
    }
}

void BashPenguinPush(EXTengine *self) {
    FlagRecord *rec;
    FlagRecord *r;
    s32 etop;
    s32 ebot;
    EXTengine *eng;
    NAS *nas;
    EXTobject *obj;
    s32 i;
    s32 radius;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 rr;
    s32 dist;
    s32 ang;

    rec = func_80025A48(self, 0x400);
    i = 0;
    if (rec == NULL) {
        return;
    }
    radius = rec->radius;
    top = self->unk10.y + rec->unk2;
    bot = self->unk10.y + rec->height;
    for (; i < 4; i++) {
        if (!(Heroes[i].hero_status & 0xF800)) {
            eng = Heroes[i].hero_eng;
            if (eng != NULL) {
                r = func_80025A48(eng, 0x40);
                if (r == NULL) {
                    continue;
                }
                etop = eng->unk10.y + r->unk2;
                ebot = eng->unk10.y + r->height;
                if (bot < etop && ebot < top) {
                    dx = eng->unk10.x - self->unk10.x;
                    dz = eng->unk10.z - self->unk10.z;
                    rr = r->radius;
                    rr += radius;
                    dist = dx * dx + dz * dz;
                    rr *= rr;
                    if (dist < rr) {
                        Heroes[i].hero_move->reactive_angle = get_angle(self->unk10.x, self->unk10.z);
                        Heroes[i].hero_move->reactive_speed = 0x60;
                        func_80084E04(i, NULL, -1, 0x8005, 0x446);
                    }
                }
            }
        }
    }
    for (nas = D_8005AA2C; nas != NULL; nas = nas->Solid.Child) {
        if (!(nas->Class & 0x200)) {
            continue;
        }
        eng = nas->Engine;
        if (eng == NULL || eng == self) {
            continue;
        }
        r = func_80025A48(eng, 0x40);
        if (r == NULL) {
            continue;
        }
        etop = eng->unk10.y + r->unk2;
        ebot = eng->unk10.y + r->height;
        if (bot < etop && ebot < top) {
            dx = eng->unk10.x - self->unk10.x;
            dz = eng->unk10.z - self->unk10.z;
            rr = r->radius;
            rr += radius;
            dist = dx * dx + dz * dz;
            if (dist < rr * rr) {
                ang = get_angle(-dx, -dz);
                dist = csqrt(dist) >> 6;
                dist = rr - dist;
                eng->unk10.x -= (dist * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                eng->unk10.z -= (dist * D_80068BD4[ang & 0xFFF].unk2) >> 12;
                obj = FindExtension(nas, 0x1000);
                if (obj != NULL) {
                    if (obj->can_do & 8) {
                        obj->player = 6;
                        obj->will_do |= 8;
                    }
                    if (obj->can_do & 2) {
                        obj->player = 6;
                        obj->will_do |= 2;
                    }
                }
            }
        }
    }
}

void InitTumbler(NAS *nas) {
    EXTengine *eng;
    EXTmotion *mot;
    TumblerObj *obj;
    subengine *sub;
    NAS *puff;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        sub->unk6C = *(void **)D_80058AD0;
        eng->sub->unk74 = 0x200A;
        eng->flags &= 0xFFFF7FFF;
        eng->unk10 = D_8005B2B8[8].pos;
        eng->unk5D = 1;
        eng->unk5C = 1;
        func_8001D390(eng, 1);
        sub = sub->next;
        if (sub != NULL) {
            sub->unk6C = *(void **)D_80058AD0;
            sub->unk74 = 0x2001;
            sub->flags |= 0x40008041;
            sub->unk50 = 1;
            sub->unk76 = 0xB00;
        }
        puff = BashSpawnPuff(&D_8005B2B8[8].pos);
        if (puff != NULL) {
            func_80026C38((u8 *)nas, (u8 *)puff);
        }
        obj = (TumblerObj *)AddObject(nas);
        if (obj != NULL) {
            nas->FUX = (EXT *)obj;
            obj->unk1A = 1;
            obj->unk14 = 6;
        }
        mot = AddMotion(nas);
        if (mot != NULL) {
            mot->Xvel = 0x80;
            mot->Xmax = 0x80;
            mot->Yvel = 0xA0;
            mot->Ymax = 0xA0;
            mot->Ydec = 6;
            mot->Xacc = 0;
            mot->Xdec = 0;
            mot->Yacc = 0;
            mot->Ydir = 0xFF;
        }
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotTumbler(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    BashTumbleMove *move;
    TumblerObj *obj;
    NAS *child;
    EXTengine *ceng;
    s32 hero;
    s32 dy;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    sub = eng->sub;
    move = FindExtension(nas, 0x800);
    if (move == NULL) {
        return;
    }
    obj = (TumblerObj *)nas->FUX;
    if (obj == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 1:
        hero = 0;
        if (D_8005A680[0] != 0) {
            hero = Random(2);
            if (Heroes[hero].hero_status & 0xFA00) {
                hero = Heroes[hero].hero_teammate;
                if (hero == -1) {
                    hero = 0;
                }
            } else {
                goto spawn;
            }
        }
        if (Heroes[hero].hero_status & 0xFA00) {
            func_8008E27C(&eng->unk10.x, (HitInfo *)obj, 1);
            func_800270B0(nas);
            return;
        }
    spawn:
        BashTumblerAim((BashTumbler *)eng, move, hero);
        child = CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1108, 0x11);
        if (child != NULL) {
            ceng = child->Engine;
            if (ceng != NULL) {
                ceng->unk1E = eng->unk1E;
            }
        }
        eng->flags |= 0x8000;
        nas->UserInfo.status = 2;
        PlaySfx(0x409, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        return;
    case 2:
        if (GravitateNas(nas) != 0) {
            eng->unk10.y += 0x20;
            CrateCollide(eng, 0, -1, 0);
            eng->unk10.y -= 0x20;
            func_8008E27C(&eng->unk10.x, (HitInfo *)obj, 0);
            func_800270B0(nas);
            return;
        }
        if (CrateCollide(eng, 0, obj->unk16, 1) != 0) {
            eng->unk10.y += 0x80;
            func_8008E27C(&eng->unk10.x, (HitInfo *)obj, 1);
            func_800270B0(nas);
            return;
        }
        sub = sub->next;
        if (sub != NULL) {
            dy = ((s32 (*)(EXTengine *))func_80025F44)(eng);
            sub->unk08 = -dy;
            dy = 0x1000 - dy;
            if (dy < 0x10) {
                dy = 0x10;
            }
            sub->unk20.x = dy;
            sub->unk20.y = dy;
            sub->unk20.z = dy;
        }
        return;
    case 9:
        KillNAS(nas);
        return;
    default:
        if (nas->Family.Child == NULL) {
            nas->UserInfo.status = 1;
        }
        return;
    }
}

void BashGivePowerup(s32 arg0, u16 arg1) {
    HeroInfo *h2;
    HeroInfo *base2;
    (&Heroes[arg0])->hero_pu = arg1;
    (&Heroes[arg0])->hero_weapon_count = 1;
    (&Heroes[arg0])->hero_status |= 0x10;
    switch (arg1) {
    case 25:
        (&Heroes[arg0])->hero_powerup->sub->unk74 = 0x200B;
        (&Heroes[arg0])->hero_weapon_count = 2;
        break;
    case 27:
        (&Heroes[arg0])->hero_powerup->sub->unk74 = 0x200F;
        (&Heroes[arg0])->hero_weapon_count = 3;
        break;
    default:
        base2 = Heroes;
        h2 = base2 + arg0;
        h2->hero_powerup->sub->unk74 = 0x200C;
        h2->hero_weapon_count = 1;
        break;
    }
    switch (arg1) {
    case 25:
        PlaySfx(0x438, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    case 27:
        PlaySfx(0x437, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    default:
        PlaySfx(0x439, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    }
}

s32 BashFirePowerup(s32 arg0)
{
    HeroInfo *h;
    PadButtons *p;
    NAS *n;
    u16 pu;

    HeroInfo *ht;
    PadButtons *pt;

    ht = Heroes;
    h = &ht[arg0];
    if (h->hero_weapon_count != 0) {
        pt = (PadButtons *)D_80051338;
        p = &pt[arg0];
        if (p->unk50 & 0x8000) {
            if (h->hero_attack_timer != 0) {
                p->unk4C &= ~0x8000;
            } else {
            D_8009D754 = arg0;
            switch (h->hero_pu) {
            case 0x19:
                n = CreateNAS(h->hero_eng->unk10.x, h->hero_eng->unk10.y, h->hero_eng->unk10.z, 0x1103, 0x11);
                if (n != NULL) {
                    n->UserInfo.user1 = arg0;
                    h->hero_attack_timer = 0x14;
                    h->hero_weapon_timer = 0x1E;
                    h->hero_weapon_count -= 1;
                }
                break;
            case 0x1B:
                n = CreateNAS(h->hero_eng->unk10.x, h->hero_eng->unk10.y, h->hero_eng->unk10.z, 0x1104, 0x11);
                if (n != NULL) {
                    n->UserInfo.user1 = arg0;
                    h->hero_attack_timer = 0x14;
                    h->hero_weapon_timer = 0x1E;
                    h->hero_weapon_count -= 1;
                }
                break;
            case 0x1C:
                n = CreateNAS(h->hero_eng->unk10.x, h->hero_eng->unk10.y, h->hero_eng->unk10.z, 0x1105, 0x11);
                if (n != NULL) {
                    n->UserInfo.user1 = arg0;
                    h->hero_attack_timer = 0x28;
                    h->hero_weapon_timer = 0x3C;
                    h->hero_weapon_count -= 1;
                }
                break;
            default:
                return 0;
            }
            }
        } else {
            return 0;
        }
    }
    return 0;
}

s32 BashMissileHitTest(x, y, z, size, skip, mask)
    s32 x;
    s32 y;
    s32 z;
    u16 size;
    s32 skip;
    u16 mask;
{
    s32 ret;
    u32 i;
    EXTengine *eng;
    NAS *nas;
    EXTobject *obj;
    FlagRecord *rec;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 rr;
    s32 dist;

    ret = -1;
    for (i = 0; i < 4; i++) {
        if (skip == i) {
            continue;
        }
        if (Heroes[i].hero_status & 0xFA00) {
            continue;
        }
        eng = Heroes[i].hero_eng;
        if (eng == NULL) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        top = eng->unk10.y + rec->unk2 + size;
        bot = eng->unk10.y + rec->height - size;
        if (y < top && bot < y) {
            dx = eng->unk10.x - x;
            dist = dx * dx;
            dz = eng->unk10.z - z;
            dist += dz * dz;
            rr = rec->radius + size;
            rr *= rr;
            if (dist < rr) {
                ret = i;
            }
        }
    }
    for (nas = D_8005AA2C; nas != NULL; nas = nas->Solid.Child) {
        if (!(nas->Class & 0x200)) {
            continue;
        }
        eng = nas->Engine;
        if (eng == NULL) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        top = eng->unk10.y + rec->unk2 + size;
        bot = eng->unk10.y + rec->height - size;
        if (y < top && bot < y) {
            dx = eng->unk10.x - x;
            dist = dx * dx;
            dz = eng->unk10.z - z;
            dist += dz * dz;
            rr = rec->radius + size;
            rr *= rr;
            if (dist < rr) {
                obj = FindExtension(nas, 0x1000);
                if (obj != NULL && (obj->can_do & mask)) {
                    obj->player = skip;
                    obj->will_do |= mask;
                }
                if (ret != -1) {
                    ret = -1;
                }
            }
        }
    }
    return ret;
}

void InitBash8way(NAS *nas) {
    EXTengine *eng;
    EXTmotion *mot;
    subengine *sub;

    eng = nas->Engine;
    if (eng != NULL) {
        nas->UserInfo.user1 = D_8009D754;
        nas->UserInfo.user2 = Heroes[D_8009D754].hero_move->facing_angle;
        func_8001D390(eng, 7);
        sub = eng->sub;
        eng->unk5D = 0;
        for (; sub != NULL; sub = sub->next) {
            sub->unk51 = 0;
            sub->unk6C = D_80052F5C;
            sub->unk74 = 0x200E;
        }
        mot = AddMotion(nas);
        if (mot != NULL) {
            mot->Xvel = 0x20;
            mot->Xmax = 0xA0;
            mot->Xacc = 8;
            mot->Ymax = 0x80;
            mot->Xdec = 0;
            mot->Yvel = 0;
            mot->Yacc = 1;
            mot->Ydec = 0;
            mot->Ydir = 0;
        }
        eng->unk10.y -= 0xE0;
        PlaySfx(0x43C, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBash8way(NAS *nas) {
    EXTengine *eng;
    EXTmotion *mot;
    subengine *sub;
    s32 dead;
    s32 ang;
    s32 a;
    s32 speed;
    s32 t;
    s32 x, y, z;
    s32 kind;

    eng = nas->Engine;
    if (eng != NULL) {
        dead = 1;
        kind = 0x19;
        mot = FindExtension(nas, 0x800);
        if (mot != NULL) {
            mot->Xvel += mot->Xacc;
            if (mot->Xvel > mot->Xmax) {
                mot->Xvel = mot->Xmax;
            }
            mot->Yvel += mot->Yacc;
            if (mot->Yvel > mot->Ymax) {
                mot->Yvel = mot->Ymax;
            }
            ang = nas->UserInfo.user2;
            sub = eng->sub;
            speed = mot->Xvel;
            for (; sub != NULL; ang += 0x200, sub = sub->next, ang &= 0xFFF) {
                if (sub->flags != 0) {
                    a = (ang - 0x400) & 0xFFF;
                    sub->unk04 -= (speed * D_80068BD4[a].unk2) >> 12;
                    sub->unk0C -= (speed * D_80068BD4[ang & 0xFFF].unk2) >> 12;
                    t = sub->unk08 + mot->Yvel;
                    sub->unk08 = t;
                    x = eng->unk10.x + sub->unk04;
                    y = eng->unk10.y + t;
                    z = eng->unk10.z + sub->unk0C;
                    dead = 0;
                    sub->unk12 = a;
                    if (y >= World_floor_level) {
                        BashSpawnMissileExplode(x, World_floor_level, z, nas->UserInfo.user1, kind);
                        sub->flags = 0;
                    }
                    if (BashMissileHitTest(x, y, z, 0x20, nas->UserInfo.user1, 0xA) != -1) {
                        BashSpawnMissileExplode(x, World_floor_level, z, nas->UserInfo.user1, kind);
                        sub->flags = 0;
                    }
                    sub->unk52 = 1;
                }
            }
        }
        if (dead) {
            KillNAS(nas);
        }
    }
}

void InitBashHomer(NAS *nas) {
    EXTengine *eng;
    EXTmotion *mot;
    subengine *sub;
    HeroInfo *h;
    s32 i, best, bestd, d, u1;

    eng = nas->Engine;
    if (eng != NULL) {
        h = Heroes;
        nas->UserInfo.user1 = D_8009D754;
        nas->UserInfo.user2 = Heroes[D_8009D754].hero_move->facing_angle;
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0x200F;
        func_8001D390(eng, 1);
        eng->unk5D = 0;
        eng->unk5C = 1;
        eng->flags |= 0x21;
        sub = eng->sub;
        sub = sub->next;
        if (sub != NULL) {
            sub->unk20.x = 0x4000;
            sub->unk20.y = 0x4000;
            sub->unk20.z = 0x4000;
            sub->unk50 = 1;
            sub->unk51 = 0;
            sub->unk6C = D_80052F5C;
            sub->unk74 = 0x2010;
            sub->flags |= 0x21;
            sub = sub->next;
            sub->unk52 = 1;
        }
        mot = AddMotion(nas);
        if (mot != NULL) {
            mot->Xvel = 0x20;
            mot->Xmax = 0xA0;
            mot->Xacc = 8;
            mot->Ymax = 0x80;
            mot->Xdec = 0;
            mot->Yvel = 0;
            mot->Yacc = 1;
            mot->Ydec = 0;
            mot->Ydir = 0;
        }
        PlaySfx(0x43B, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        best = -1;
        bestd = 0x800;
        for (i = 0; i < 4; i++) {
            u1 = nas->UserInfo.user1;
            if (i != u1 && (OVLG->p2_active == 0 || Heroes[i].hero_teammate != u1)) {
                d = func_80015480(nas->UserInfo.user2,
                                  get_angle(eng->unk10.x - Heroes[i].hero_eng->unk10.x,
                                            eng->unk10.z - Heroes[i].hero_eng->unk10.z));
                if (d < bestd) {
                    bestd = d;
                    best = i;
                }
            }
        }
        nas->UserInfo.status = best;
        nas->UserInfo.flags = 0x64;
        eng->unk10.y -= 0x180;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBashHomer(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    EXTmotion *mot;
    s32 dead;
    s32 ang;
    s32 d;
    s32 dx, dz;
    s32 x, y, z;
    s16 target;

    eng = nas->Engine;
    dead = 1;
    if (eng != NULL) {
        mot = FindExtension(nas, 0x800);
        if (mot != NULL) {
            mot->Xvel += mot->Xacc;
            dead = 0;
            if (mot->Xvel > mot->Xmax) {
                mot->Xvel = mot->Xmax;
            }
            ang = nas->UserInfo.user2;
            target = nas->UserInfo.status;
            if (target != -1) {
                d = func_800154A0(get_angle(eng->unk10.x - Heroes[target].hero_eng->unk10.x,
                                            eng->unk10.z - Heroes[target].hero_eng->unk10.z), ang);
                if (abs(d) < 0x40) {
                    if (d < 0) {
                        ang += 0x40;
                    } else {
                        ang -= 0x40;
                    }
                } else {
                    ang -= d >> 3;
                }
                nas->UserInfo.user2 = ang;
                he = Heroes[target].hero_eng;
                dx = eng->unk10.x - he->unk10.x;
                dz = eng->unk10.z - he->unk10.z;
                if (dx * dx + dz * dz < 0x40000) {
                    eng->unk10.y -= (eng->unk10.y - (he->unk10.y - 0xA0)) >> 2;
                }
            }
            eng->unk10.x -= (mot->Xvel * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
            eng->unk10.z -= (mot->Xvel * D_80068BD4[ang & 0xFFF].unk2) >> 12;
            x = eng->unk10.x;
            y = eng->unk10.y;
            z = eng->unk10.z;
            if (y >= World_floor_level || nas->UserInfo.flags == 0) {
                BashSpawnMissileExplode(x, World_floor_level, z, nas->UserInfo.user1, 0x1B);
                dead = 1;
            }
            if (BashMissileHitTest(x, y, z, 0x80, nas->UserInfo.user1, 0xA) != -1) {
                BashSpawnMissileExplode(x, y, z, nas->UserInfo.user1, 0x1B);
                dead = 1;
            }
            nas->UserInfo.flags--;
            if ((u32)(eng->unk10.x + 0x1000) > 0x2000) {
                dead = 1;
            }
            if ((u32)(eng->unk10.z + 0x1000) > 0x2000) {
                dead = 1;
            }
        }
        if (dead) {
            KillNAS(nas);
        }
    }
}

extern u16 D_8009D754;

void InitBashShox(NAS *nas) {
    EXTengine *eng;
    BashShoxExt *x;
    subengine *sub;
    s32 i;

    eng = nas->Engine;
    if (eng != NULL) {
        nas->UserInfo.user1 = D_8009D754;
        x = AddExt(nas, 0x68, 0x20000000);
        if (x != NULL) {
            nas->FUX = (void *)x;
            func_8001D390(eng, 6);
            sub = eng->sub;
            eng->unk5D = 0;
            eng->unk5C = 1;
            eng->flags |= 0x21;
            if (sub != NULL) {
                i = 0;
                do {
                    sub->unk51 = 0;
                    sub->unk50 = 1;
                    sub->unk6C = D_80052F94;
                    sub->unk74 = 0x2015;
                    sub->flags = (sub->flags & 0xFFFF7FFF) | 0x21;
                    sub->unk08 = -0x100;
                    x->a[i] = i * 5;
                    x->b[i] = 0xC;
                    x->c[i] = 0x1000;
                    sub = sub->next;
                    i++;
                } while (sub != NULL);
            }
            PlaySfx(0x43A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBashShox(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    BashShoxExt *x;
    subengine *sub;
    HeroInfo *h;
    HeroInfo *base;
    EXTheromove *mv;
    s32 dead;
    s32 px, py, pz;
    s32 size;
    s32 r;
    s32 hit;
    s32 ang;
    s32 vc;
    s32 i;

    eng = nas->Engine;
    dead = 1;
    if (eng != NULL) {
        px = eng->unk10.x;
        x = nas->FUX;
        pz = eng->unk10.z;
        py = eng->unk10.y - 0x100;
        if (x != NULL) {
            eng->unk10 = Heroes[nas->UserInfo.user1].hero_eng->unk10;
            sub = eng->sub;
            size = 0;
            for (i = 0; sub != NULL; i++, sub = sub->next) {
                if (sub->flags != 0) {
                    dead = 0;
                    if (x->a[i] != 0) {
                        x->a[i]--;
                    } else {
                        sub->flags |= 0x40008000;
                        x->c[i] += 0x500;
                        sub->unk20.x = x->c[i];
                        sub->unk20.z = x->c[i];
                        sub->unk76 += 0x100;
                        r = x->c[i] * 128 >> 12;
                        if (size < r) {
                            size = r;
                        }
                        if (x->b[i] != 0) {
                            x->b[i]--;
                        } else {
                            sub->flags = 0;
                        }
                    }
                }
                sub->unk52 = 1;
            }
            hit = BashMissileHitTest(px, py, pz, (size + 0x40) & 0xFFFF, nas->UserInfo.user1, 0xA);
            if (hit != -1 && hit < 4) {
                base = Heroes;
                h = &base[hit];
                if ((h->hero_immunity | h->hero_hit) == 0) {
                    he = h->hero_eng;
                    ang = get_angle(px - he->unk10.x, pz - he->unk10.z);
                    mv = h->hero_move;
                    vc = mv->velocity_calc;
                    if (vc == 1) {
                    } else if (vc == 0) {
                    } else if (vc < 4) {
                        mv->x_vel -= (D_80068BD4[(ang - 0x400) & 0xFFF].unk2 * 3) >> 8;
                        h->hero_move->z_vel -= (D_80068BD4[ang & 0xFFF].unk2 * 3) >> 8;
                    }
                    Heroes[hit].hero_move->reactive_angle = ang;
                    Heroes[hit].hero_move->reactive_speed = 0x30;
                    func_80084E04(hit, NULL, nas->UserInfo.user1, 0x8008, 0x446);
                }
            }
        }
        if (dead) {
            KillNAS(nas);
        }
    }
}

void PlotMissileExplode(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s16 *row;
    s16 (*tbl)[3];
    s32 z;
    u16 idx;

    eng = nas->Engine;
    if (eng != NULL) {
        idx = nas->UserInfo.flags;
        tbl = D_800C3B08;
        row = tbl[idx];
        sub = eng->sub;
        if (row[0] != 0) {
            sub->unk20.x = row[0];
            sub->unk20.y = row[1];
            z = row[2];
            sub->unk52 = 1;
            sub->unk20.x = z;
            nas->UserInfo.flags++;
            if (idx == 1) {
                func_8008DEC8((s32)nas, 0x280, nas->UserInfo.user1, nas->UserInfo.user2, 0);
            }
        } else {
            KillNAS(nas);
        }
    }
}

void InitMissileExplode(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        sub->unk6C = &D_80052F5C;
        sub->unk74 = 0x2010;
        sub->unk50 = 1;
        sub->unk51 = 0;
        eng->flags |= 0x21;
        nas->UserInfo.flags = 0;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void BashSpawnMissileExplode(s32 x, s32 y, s32 z, s32 arg3, s32 arg4) {
    NAS *n;

    n = CreateNAS(x, y - 0x80, z, 0x1106, 0x11);
    if (n != NULL) {
        n->UserInfo.user1 = arg3;
        if (arg4 != 0x19) {
            if (arg4 != 0x1B) {
                return;
            }
        } else if (Random(4) != 0) {
            goto set;
        }
        PlaySfx(D_8009E424[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
set:
        n->UserInfo.user2 = 0x208;
    }
}

void PlotBoing(u8 *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        func_800250B0((NAS *)arg0);
    }
}

void InitBoing(NAS *nas) {
    EXTengine *eng;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->unk10.y -= 0x100;
        eng->sub->unk6C = *(void **)D_80058AD0;
        eng->unk5D = 1;
        eng->unk5C = 1;
        func_80024E30(nas, &D_800C3AFC);
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitBashPenguin(NAS *nas) {
    EXTengine *eng;
    EXTobject *obj;

    eng = nas->Engine;
    if (eng != NULL) {
        D_800C46B0 = eng;
        func_80024E30(nas, &D_800C3668);
        eng->sub->unk6C = (s32)D_8005303C;
        eng->sub->unk74 = 0x2001;
        obj = (EXTobject *)AddObject(nas);
        if (obj != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
            D_800C46B4 = obj;
            nas->FUX = (EXT *)obj;
            obj->whatami = 0xE;
            obj->can_do |= 2;
        }
        eng->unk5D = 1;
        FetchDefaultColBox(eng, 0x50);
        BashSnowMarkPenguin();
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitSnowGlint(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    SnowGlint *g;
    s32 i;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->sub->unk6C = *(s32 *)D_80058AD0;
        func_8001D390(eng, 9);
        sub = nas->Engine->sub;
        g = AddExt(nas, 0x34, 0x20000002);
        if (g != NULL && sub != NULL) {
            i = 0;
            do {
                s32 v = *(s32 *)D_80058AD0;
                sub->unk74 = 0x2001;
                sub->flags = (sub->flags | 0x21) & 0xFFFF7FFF;
                sub->unk6C = v;
                g->on[i] = 0;
                g->timer[i] = Random(0xF);
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void BashDrawView(void) {
    u8 *vp = D_8009F6F4;
    func_80093038();
    func_80018B08((GfxViewport *)vp, (GfxCamera *)D_80051640);
    D_800569AC = D_8009F644.amount;
    func_8009440C();
}

s32 GetBashContour(EXTengine *eng) {
    s32 x = (eng->unk10.x + 0x800) >> 9;
    s32 z = (eng->unk10.z + 0x800) >> 9;
    s32 dy = eng->unk10.y - World_floor_level;

    if ((u32)x >= 8 || (u32)z >= 8) {
        return dy;
    }
    if (D_800C3BAC[x][z].unk8 & 0x100) {
        return 0x7FFFFFFF;
    }
    return AddPlatformContour(eng, dy);
}

u16 BashSpaceTileAt(s32 arg0, s32 arg1) {
    s32 x = (arg0 + 0x800) >> 9;
    s32 y = (arg1 + 0x800) >> 9;

    if ((u32)x >= 8) {
        return 0;
    }
    if ((u32)y >= 8) {
        return 0;
    }
    return D_800C3BAC[x][y].unkA;
}

void BashSpaceMarkHoles(void) {
    s32 i, j;

    for (j = 2; j < 18; j++) {
        for (i = 2; i < 18; i++) {
            if (D_800C3BAC[(j - 2) >> 1][7 - ((i - 2) >> 1)].unkA == 0) {
                D_8009D90C[i * 20 + j].unk0 = 0xFF;
            }
        }
    }
}

void BashSpaceDropTile(s32 arg0) {
    EXTengine *eng = Heroes[arg0].hero_eng;
    BashCell *base = D_800C3BAC[0];
    s32 xi = (eng->unk10.x + 0x800) >> 9;
    s32 zi = (eng->unk10.z + 0x800) >> 9;
    BashCell *cell = (BashCell *)((char *)base + (zi * 12 + xi * 0x60));
    BashObj *obj = cell->obj;

    if (obj != NULL) {
        obj->flags &= 0xFFFF7FFF;
        cell->unkA = 0;
    }
}

void BashSpaceBreakTile(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    BashCell *c;
    s32 x;
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801) && !(*(s32 *)(g + 4) & 0x1000) &&
        arg1 == World_floor_level && arg5 < 8 && arg5 >= 5) {
        arg0 = (arg0 + 0x800) >> 9;
        arg2 = (arg2 + 0x800) >> 9;
        if ((u32)arg0 < 8 && (u32)arg2 < 8) {
            if (((BashCell *)((char *)D_800C3BAC + arg2 * 12 + arg0 * 0x60))->obj != NULL) {
                *(s32 *)((BashCell *)((char *)D_800C3BAC + arg2 * 12 + arg0 * 0x60))->obj &= 0xFFFF7FFF;
                ((BashCell *)((char *)D_800C3BAC + arg2 * 12 + arg0 * 0x60))->unkA = 0;
                ((BashCell *)((char *)D_800C3BAC + arg2 * 12 + arg0 * 0x60))->unk8 |= 0x100;
            }
        }
    }
}

void BashSpaceClearTiles(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            D_800C3BAC[j][i].unkA = 0;
            D_800C3BAC[j][i].unk8 = 0;
        }
    }
}

void BashDropFirstCrates(void) {
    BashDropCrates(D_800C3B88);
}

u8 *BashDropCrateNear(s32 x, s32 unused, s32 z, u16 flag) {
    s32 step;
    s32 neg;
    s32 nx;
    s32 nz;
    s32 zi;
    s32 xi;
    u8 *res;

    step = 1;
    xi = (x >> 8) + 10;
    zi = 9 - (z >> 8);

    do {
        if (D_8009D90C[zi * 20 + xi].unk0 != 0) {
            xi = (x >> 8) + 10;
            zi = 9 - (z >> 8);
            if (flag != 0) {
                zi += step;
            } else {
                xi += step;
            }
            neg = ~step;
            step = neg + 1;
            if (step > 0) {
                step = neg + 2;
            }
            if (step >= 9) {
                step = 0;
            }
        } else {
            step = 0;
        }
    } while (step != 0);
    xi = xi - 10;
    xi <<= 8;
    zi = 19 - zi;
    zi = zi - 10;
    nz = zi << 8;
    if ((u32)(xi + 0x700) > 0xE00U) {
        xi = x;
    }
    if ((u32)(nz + 0x700) > 0xE00U) {
        nz = z;
    }
    res = CreateCrate(xi, 0, nz, 2, -1);
    if (res != NULL) {
        func_80026B70(&D_800C478C, res);
    }
    return res;
}

s32 BashAI_HazardAhead(s32 arg0) {
    EXTbotai *ai = Heroes[arg0].hero_ai;
    s32 i;
    s32 l;
    s32 r;

    for (i = 0; i < 3; i++) {
        l = ai->AIforward_left[i] & 0x3F;
        r = ai->AIforward_right[i] & 0x3F;
        if (ai->AIforward_left[i] == 0xFF || ai->AIforward_right[i] == 0xFF) {
        ret1:
            return 1;
        }
        if (l >= 6) {
            if (l < 8) {
                goto ret1;
            }
            if (l < 0x16) {
                if (l >= 0x14) {
                    goto ret1;
                }
            }
        }
        if (r >= 6) {
            if (r < 8) {
                goto ret1;
            }
            if (r < 0x16) {
                if (r >= 0x14) {
                    goto ret1;
                }
            }
        }
    }
    return 0;
}

s32 BASH_KeepInBounds(u8 *arg0) {
    s32 temp_a0;
    s32 temp_v1;
    s32 var_a2;

    var_a2 = 0;
    if ((*(s32*)((u8*)(arg0) + 144)) & 0x40) {
        temp_a0 = 0x800 - (*(s16*)((u8*)(arg0) + 138));
        if (temp_a0 < (*(s32*)((u8*)(arg0) + 16))) {
            (*(s32*)((u8*)(arg0) + 16)) = temp_a0;
            var_a2 = 1;
        }
        temp_v1 = -temp_a0;
        if ((*(s32*)((u8*)(arg0) + 16)) < temp_v1) {
            (*(s32*)((u8*)(arg0) + 16)) = temp_v1;
            var_a2 = 1;
        }
        if (temp_a0 < (*(s32*)((u8*)(arg0) + 24))) {
            (*(s32*)((u8*)(arg0) + 24)) = temp_a0;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < temp_v1) {
            (*(s32*)((u8*)(arg0) + 24)) = temp_v1;
            goto block_17;
        }
    } else {
        if ((*(s32*)((u8*)(arg0) + 16)) >= 0x801) {
            (*(s32*)((u8*)(arg0) + 16)) = 0x800;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 16)) < -0x800) {
            (*(s32*)((u8*)(arg0) + 16)) = -0x800;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) >= 0x801) {
            (*(s32*)((u8*)(arg0) + 24)) = 0x800;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < -0x800) {
            (*(s32*)((u8*)(arg0) + 24)) = -0x800;
block_17:
            var_a2 = 1;
        }
    }
    return var_a2;
}

void BashSnowInitHeroMove(void) {
    s32 i;
    EXTheromove *m;
    for (i = 0; i < 4; i++) {
        m = Heroes[i].hero_move;
        if (m != NULL) {
            m->z_acc = 8;
            m->x_acc = 8;
            m->z_dec = 4;
            m->x_dec = 4;
            if ((OVLG->flags & 0x80) && (i == 0 || (OVLG->p2_active != 0 && i == 1))) {
                m->z_acc = 4;
                m->x_acc = 4;
                m->z_dec = 1;
                m->x_dec = 1;
            }
            m->velocity_calc = 3;
        }
    }
}

void FirstTriggersBASH(void) {
}

void BashRestartLevel(void) {
}

void BASHSetUp(void) {
    D_8005AA70[0] = (s32)InitialiseBASHFirst;
    D_8005AA70[1] = (s32)InitialiseBASH;
    D_8005AA70[2] = (s32)InitialiseBASH;
    D_8005AA70[3] = (s32)UpdateBASH;
    D_8005AA70[5] = (s32)FirstTriggersBASH;
    D_8005AA70[9] = (s32)GetBashContour;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[10] = (s32)FinishBashLevel;
}

void BashInitCrateHooks(void) {
    D_8009E464 = BashASTIndex(0x201);
    D_8009E468 = BashASTIndex(0x202);
    D_8009E488.unk4 = D_8009E3B4;
    D_8009E488.unk8 = NULL;
    D_8009E488.unkC = 0;
    D_8009E488.unk0 = D_8009E3DC;
}

void BashTumblerAim(BashTumbler *arg0, BashTumbleMove *arg1, s32 arg2) {
    s32 y;
    s32 vel;
    s32 v2;
    s32 sum;
    s32 d1;
    s32 d2;
    s32 max;
    s32 max2;
    s32 steps;
    s32 dir;
    s32 dx;
    s32 dz;
    s32 ang;
    s32 dist;

    dist = arg0->y;
    if (dist >= World_floor_level) {
        dist = World_floor_level - 1;
    }
    y = dist;
    sum = 0;
    vel = arg1->vel;
    v2 = arg1->v2;
    dir = arg1->dir;
    d1 = arg1->acc - arg1->dec;
    d2 = arg1->acc2 - arg1->dec2;
    max = arg1->max;
    max2 = arg1->max2;
    steps = 0;
    while (y < World_floor_level) {
        vel += d1;
        if (vel < 0) {
            vel = 0;
        }
        if (vel > max) {
            vel = max;
        }
        v2 += d2;
        sum += vel;
        if (v2 < 0) {
            v2 = 0;
        }
        if (v2 > max2) {
            v2 = max2;
        }
        if (dir != 0) {
            y -= v2;
        } else {
            y += v2;
        }
        if (v2 == 0) {
            d2 = arg1->dec2;
            dir = 0;
        }
        steps++;
    }
    arg1->sum = sum;
    arg1->steps = steps;
    dx = arg0->x - Heroes[arg2].hero_eng->unk10.x;
    dz = arg0->z - Heroes[arg2].hero_eng->unk10.z;
    ang = get_angle(dx, dz);
    dist = csqrt(dx * dx + dz * dz);
    arg0->angle = ang;
    arg1->vel = (u32)(dist >> 6) / arg1->steps;
}
