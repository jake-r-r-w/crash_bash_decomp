#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/heroes.h"
#include "bash/hero.h"
#include "bash/shell.h"

void InitialiseHeroes(void) {
    s32 i;
    s32 j;
    HeroInfo *h;
    EXTengine *eng;
    EXTengine *e;
    EXTengine *fx;
    subengine *sub;
    s32 name;
    NAS *nas;
    NAS *hn;
    EXTheromove *mv;
    EXTbotai *ai;
    AiSkill *c;
    GameState *g;
    u8 *p;
    u8 *q;
    s32 m;
    s32 k;
    u16 ang;
    s32 mask;
    s32 fxmask;
    HeroInfo *hb;
    s8 tm;
    s32 idx;
    s32 fxv;

    i = 0;
    m = 0;
    k = 0;
    HeroAnimHeroA = -1;
    HeroAnimSeqA = 0;
    HeroAnimSeqB = 0;
    HeroAnimNameA = 0;
    HeroAnimNameB = 0;
    HeroAnimDbaseA = 0;
    HeroAnimDbaseB = 0;
    HeroesInPlay = 4;
    HeroesSidelined = 4;
    FinalHeroesTempo = 0;
    GAME->exitFlags &= 0xF7FF;
    hb = Heroes;
    while (1) {
        if (i >= 4) {
            break;
        }
        nas = (NAS *)((u8 *)NasPool + m * 16);  /* &NasPool[i] drops the m counter (+7 a hero) */
        h = &hb[i];
        h->hero = nas;
        *nas = NASTemplate;
        hn = h->hero;
        if (NASActiveTail != NULL) {
            NASActiveTail->Tag.Child = hn;
            hn->Tag.Parent = NASActiveTail;
            NASActiveTail = hn;
        } else {
            NASActiveTail = hn;
            NASActiveHead = hn;
        }
        h->hero->NASid = 1;
        h->hero->Class = 0x1400;
        h->hero->Type = 3;
        h->hero_eng = NULL;
        h->hero_sub = NULL;
        h->hero_shadow = NULL;
        h->hero_powerup = NULL;
        h->hero_fx = NULL;
        h->hero_move = NULL;
        h->hero_anim = NULL;
        h->hero_ai = NULL;
        h->hero_object = NULL;
        h->hero_health = 0x14;
        g = GAME;
        g->pause = 0;
        g = (GameState *)((u8 *)g + i);  /* indexing unk50/unk58 by i adds two insns */
        h->hero_status = 0;
        h->hero_strength = 0x14;
        h->hero_immunity = 0;
        h->hero_immunity_level = 0;
        h->hero_hit = 0;
        h->hero_last_mode = 0;
        h->hero_mode = 0;
        tm = g->teammates[0];
        h->hero_idle_count = 600;
        h->hero_harm_player = -1;
        h->hero_pick_delay = 0;
        h->hero_aseq = 0;
        h->hero_harm_Nas = NULL;
        h->hero_harm_value = 0;
        h->hero_harm_fraction = 0;
        h->hero_teammate = tm;
        h->hero_harm_flags = 0;
        h->hero_harm_sfx = 0;
        h->hero_ctrl = 0;
        h->hero_score = 0;
        h->hero_score_displayed = 0;
        h->hero_pu = 0;
        h->hero_pu_timer = 0;
        h->hero_pu_lock = 0;
        h->hero_attack_timer = 0;
        h->hero_attack_delay = 0;
        h->hero_pu_action = 0;
        h->hero_pu_action_timer = 0;
        h->hero_weapon_count = 0;
        h->hero_weapon_timer = 0;
        h->hero_attack_counter = 0;
        HeroLastHarm[i] = 0;
        h->hero_name = g->heroes[0];
        c = (AiSkill *)((s16 *)AiPlayerSkill + k);  /* &AiPlayerSkill[i] drops the k counter */
        c->level = 0x10;
        c->unk2 = 0x10;
        c->quality = 0x10;
        if (h->hero_name == 9) {
            h->hero_status = 0x1000;
        }
        eng = AddExtEngine(h->hero, 1);
        if (eng == NULL) {
            goto next;
        }
        sub = eng->sub;
        h->hero_eng = eng;
        h->hero_sub = sub;
        eng->unk5D = 0;
        eng->unk10 = MapPoints[i].pos;
        h->hero_eng->unk10.y = World_floor_level;
        eng->OldXYZ = eng->unk10;
        h->hero_anim = AllocExt400(h->hero);
        name = h->hero_name;
        if ((u32)(name - 1) < 8) {
            h->hero_sub->u6C.dbase = &HeroModelBank[((u16 *)HeroHooks.HeroModels)[name]];
        } else {
            h->hero_sub->u6C.dbase = NULL;
        }
        sub->entity = HeroEntities[name];
        FetchDefaultColBox(eng, 0x50);
        if (HeroHooks.InitHero != 0) {
            ((void (*)(s32))HeroHooks.InitHero)(i);
        } else {
            SetHeroAction(i, 0);
        }
        e = AddExtEngine(h->hero, 2);
        if (e != NULL) {
            h->hero_shadow = e;
            if (HeroHooks.CreateShadow != NULL) {
                ((void (*)(s32))HeroHooks.CreateShadow)(i);
            } else {
                e->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
                e->sub->entity = 0x2001;
                e->unk10 = eng->unk10;
                e->flags |= 0x8041;
                e->sub->unk76 = 0x800;
                e->sub->flags |= 0x40000000;
            }
        }
        fx = AddExtEngine(h->hero, 3);
        if (fx != NULL) {
            h->hero_powerup = fx;
            fx->unk10 = eng->unk10;
            sub = fx->sub;
            fx->unk5D = 0;
            if (sub != NULL) {
                sub->u6C.dbase = &HeroModelBank[DB_Powerups];
                sub->entity = 0;
                mask = ~0x8000;
                sub->flags &= mask;
            }
        }
        fx = AddExtEngine(h->hero, 3);
        if (fx != NULL) {
            h->hero_fx = fx;
            fx->unk10 = eng->unk10;
            fx->unk5D = 0;
            eng_add_subs_n(fx, 3);
            sub = fx->sub;
            fxv = 0x2002;
            while (sub != NULL) {
                idx = ((u16 *)HeroHooks.HeroModels)[name];
                fxmask = ~0x8000;
                sub->flags &= fxmask;
                sub->unk50 = 1;
                sub->unk51 = 0;
                sub->u6C.dbase = &HeroModelBank[idx];
                if (name == 3) {
                    sub->entity = fxv;
                } else if (name == 4) {
                    sub->entity = fxv;
                } else {
                    sub->entity = 0;
                }
                sub = sub->next;
            }
        }
        if (h->hero_move == NULL) {
            mv = AddExt(h->hero, 0x54, 0x400000F0);
            if (mv != NULL) {
                h->hero_move = mv;
                mv->forward_speed = 0;
                mv->reactive_speed = 0;
                mv->forward_angle = 0;
                mv->facing_angle = 0;
                mv->reactive_angle = 0;
                mv->run_walk = 1;
                mv->jump_ctrl = 3;
                mv->velocity_calc = 1;
                mv->velocity_enable = 1;
                mv->x_vel = 0;
                mv->x_max = 0;
                mv->x_acc = 0;
                mv->x_dec = 0;
                mv->x_dir = 0;
                mv->jump_vel = 0;
                mv->jump_max = 0;
                mv->jump_acc = 0;
                mv->jump_dec = 0;
                mv->jump_dir = 0;
                mv->z_vel = 0;
                mv->z_max = 0;
                mv->z_acc = 0;
                mv->z_dec = 0;
                mv->z_dir = 0;
                ang = MapPoints[i].rot.x;
                eng->unk1E = ang;
                mv->facing_angle = ang;
                mv->forward_angle = ang;
                mv->HitBy = -1;
            }
        }
        if (h->hero_ai == NULL) {
            ai = AddExt(h->hero, 0x44, 0x400000F4);
            if (ai != NULL) {
                h->hero_ai = ai;
                ai->AImode = 0;
                ai->AIaborted_mode = 0;
                ai->AItimer1 = 0;
                ai->AItimer2 = 0;
                ai->AItimer3 = 0;
                ai->AIang = 0;
                ai->AIpad = 0;
                ai->pTargetNas = NULL;
                ai->TargetPlayer = -1;
                ai->TargetXYZ.z = 0;
                ai->TargetXYZ.y = 0;
                ai->TargetXYZ.x = 0;
                ai->AIinitial_delay = Random(0x3C) + 0x1E;
                ai->AIsleep = 8;
                j = 7;  /* AIforward_left[j] loses the stepped pointer p */
                p = (u8 *)ai + j;
                do {
                    p[0x34] = 0;
                    j--;
                    p--;
                } while (j >= 0);
                j = 7;
                q = (u8 *)ai + j;
                do {
                    q[0x3C] = 0;
                    j--;
                    q--;
                } while (j >= 0);
            }
        }
        if (h->hero_status != 0x1000) {
            h->hero->Camera.width = 0x200;
            h->hero->Camera.height = 0x500;
            h->hero->Camera.look_ahead = 0x400;
            AddCamera(h->hero);
        }
    next:
        i++;
        m += 7;
        k += 3;
    }
    Setup_panel();
    ClearHeroAnimSlots();
    ArenaGridReset();
}

const char HeroNameNobody[] = "nobody";

const char HeroNameCrash[] = "Crash";

const char HeroNameCoco[] = "Coco";

const char HeroNameCortex[] = "Cortex";

const char HeroNameBrio[] = "Brio";

const char HeroNameTiny[] = "Tiny";

const char HeroNameKong[] = "Kong";

const char HeroNameDingodile[] = "Dingodile";

const char HeroNameKanga[] = "Kanga";

const char HeroAnimWin[] = "WIN";

void ProcessHeroes(void) {
    HeroInfo *h;
    HeroInfo *b;
    u16 *pc;
    HeroInfo *c;
    HeroInfo *d;
    HeroInfo *e;
    s32 r;
    s32 i;
    s32 id;
    s32 last;
    s32 ok;
    u16 status;
    u16 harm;
    u16 st;
    u16 lvl;
    subengine *sub;
    s32 bits;
    s32 pad;

    r = Random(0x10);
    last = -1;
    i = 0;
    HeroesInPlay = 0;
    HeroesSidelined = 0;
    for (;; i++) {
        HeroInfo *a;

        if (i >= 4) {
            break;
        }
        id = HeroOrders[r][i];
        a = &Heroes[id];
        status = a->hero_status;
        if (status & 0x1080) {
            a->hero_shadow->flags = 0;
            a->hero_eng->flags = 0;
            goto next;
        }
        if (status & 0x40) {
            goto next;
        }
        if (status & 0xFF00) {
            a->hero_immunity = 0;
            a->hero_eng->flags |= 0x8000;
            HPADS[id].unk50 = 0;
            HPADS[id].unk54 = 0;
            goto reset;
        }
        if (GAME->cheats < 0 && (GAME->cheats & 4) &&
            (pad = PadCount - 1, HPADS[pad].held & 0x400)) {
            a->hero_status = status | 0x2000;
            SetHeroAction(id, (u16)HeroActDead);
            goto reset;
        }
        b = &Heroes[id];
        if (b->hero_status & 0xFF00) {
            goto reset;
        }
        ok = 1;
        if (b->hero_immunity | b->hero_hit) {
            lvl = b->hero_harm_flags & 0x300;
            if (b->hero_immunity_level < lvl) {
                b->hero_immunity_level = lvl;
            } else {
                ok = 0;
            }
        } else {
            b->hero_immunity_level = 0;
        }
        h = &Heroes[id];
        harm = h->hero_harm_flags;
        if (harm & 0x1000) {
            h->hero_status |= 0x2000;
            SetHeroAction(id, (u16)HeroActDead);
            h->hero_immunity = 0;
            h->hero_health = 0;
            goto reset;
        }
        if (ok && harm) {
            h->hero_immunity_level = harm & 0x300;
            if (h->hero_harm_sfx != 0) {
                PlaySfx(h->hero_harm_sfx, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (h->hero_pu == 0x12) {
                h->hero_pu_timer = 0;
                SetHeroAction(id, (u16)HeroActHit);
            } else {
                HeroLastHarm[id] = h->hero_harm_value;
                h->hero_health -= h->hero_harm_value >> 8;
                h->hero_health -= h->hero_harm_fraction >> 8;
                h->hero_harm_fraction = (u8)h->hero_harm_fraction;
                h->hero_harm_fraction += (u8)h->hero_harm_value;
                SetHeroAction(id, (u16)HeroActHit);
                h->hero_immunity = 0;
                if (h->hero_harm_flags & 0x8000) {
                    ((void (*)(s32, s32, s32))pad_rumble_big)(id, 4, 0xFF);
                } else {
                    pad_rumble_small(id, 0xC);
                    ((void (*)(s32, s32, s32))pad_rumble_big)(id, 0xC, 0xFF);
                }
            }
        }
        h = &Heroes[id];
        if (h->hero_health < 0) {
            h->hero_health = 0;
        }
        if (h->hero_hit != 0) {
            if (--h->hero_hit == 0) {
                if (h->hero_health <= 0) {
                    if (!(GAME->cheats & 2)) {
                        h->hero_status |= 0x2000;
                        SetHeroAction(id, (u16)HeroActDead);
                        h->hero_immunity = 0;
                    }
                } else {
                    h->hero_immunity = 0x28;
                }
            }
            if (!(GAME->levelFlags & 0x1000)) {
                HPADS[id].unk50 = 0;
                HPADS[id].unk54 = 0;
            }
        }
    reset:
        c = &Heroes[id];
        st = c->hero_status;
        c->hero_harm_Nas = NULL;
        c->hero_harm_player = -1;
        c->hero_harm_value = 0;
        c->hero_harm_flags = 0;
        if (st & 0x8000) {
            c->hero_eng->flags = 0;
            RemoveCamera(c->hero);
            continue;
        }
        if (!(st & 0x2000)) {
            last = id;
            HeroesInPlay++;
        } else if (GAME->exitFlags & 0x800) {
            RemoveCamera(c->hero);
        } else {
            HeroesSidelined++;
        }
        d = &Heroes[id];
        if (d->hero_frozen != 0) {
            d->hero_frozen--;
        }
        if (d->hero_attack_timer != 0) {
            d->hero_attack_timer--;
        } else if (d->hero_attack_delay != 0) {
            d->hero_attack_delay--;
        }
        e = &Heroes[id];
        if (e->hero_immunity != 0 &&
            (e->hero_immunity--, e->hero_immunity % 3 == 0)) {
            e->hero_eng->flags &= ~0x8000;
        } else {
            e->hero_eng->flags |= 0x8000;
        }
        sub = Heroes[id].hero_fx->sub;
        if (sub != NULL) {
            sub->flags &= ~0x8000;
        }
        if (!(GAME->cheats & 0x100) && GAME->heroOut[id] != 0) {
            ((void (*)(s32))HeroHooks.HeroBot)(id);
        }
        h = &Heroes[id];
        if (h->hero_pu != 0x15 || (GameFrameCount & 1)) {
            if (GAME->pause & 0x4000000) {
                HPADS[id].unk50 = 0;
                HPADS[id].unk54 = 0;
            }
            ((void (*)(s32))HeroHooks.PreMoveHero)(id);
            HeroActionTable[h->hero_mode].update(id);
            ((void (*)(s32))HeroHooks.MoveHero)(id);
            if (HeroHooks.PostMoveHero != NULL) {
                ((void (*)(s32))HeroHooks.PostMoveHero)(id);
            }
        }
        ((void (*)(EXTengine *))HeroHooks.KeepInBounds)(Heroes[id].hero_eng);
        UpdateHeroShadow(id);
        if (HeroHooks.HeroExtraEffects != NULL) {
            ((void (*)(s32))HeroHooks.HeroExtraEffects)(id);
        }
    next:;
    }
    if (GAME->levelFlags & 0x100) {
        if (!(BossState.flags & 0x8000)) {
            if (!(BossState.flags & 0x2000)) {
                HeroesInPlay++;
            } else if (!(GAME->exitFlags & 0x800)) {
                HeroesSidelined++;
            }
            pc = (u16 *)&BossState;
            if (pc[4] != 0) {
                pc[4]--;
            }
        }
    }
    if (!(GAME->cheats & 0x20)) {
        bits = 1;
        if (GAME->cheats & 0x800) {
            bits = 0x201;
        }
        if (!(GAME->levelFlags & 0x200)) {
            ((void (*)(s32, s32))HeroHooks.ChooseWinner)(last, bits);
        }
        if (GAME->levelFlags & 0x400) {
            GAME->exitFlags |= bits;
        }
    }
    if (!(GAME->exitFlags & 0x801) && FinalHeroesTempo == 0) {
        switch (GAME->mode) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 30:
            if (HeroesInPlay + HeroesSidelined < 3) {
                FinalHeroesTempo = 1;
                music_set_tempo(0x6E, 0xF0);
            }
            break;
        }
    }
}

void DefChooseWinner(s32 idx, u16 arg1) {
    HeroInfo *h;
    HeroInfo *m;
    s32 i;
    s32 n;
    s32 best;

    if (GAME->exitFlags & 0x801) {
        return;
    }
    if (GAME->levelFlags & 0xE0) {
        if (idx == -1) {
            GAME->winner[0] = 0;
            GAME->exitFlags |= arg1;
            for (i = 0; i < 4; i++) {
                h = &Heroes[i];
                if (!(h->hero_status & 0xFA00)) {
                    h->hero_status |= 0x400;
                }
            }
        } else {
            n = 0;
            for (i = 0; i < 4; i++) {
                h = &Heroes[i];
                if (!(h->hero_status & 0xF800) && GAME->heroOut[i] == 0) {
                    n++;
                }
            }
            if (n == 0) {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
                for (i = 0; i < 4; i++) {
                    h = &Heroes[i];
                    if (!(h->hero_status & 0xFA00)) {
                        h->hero_status |= 0x400;
                    }
                }
                return;
            }
            if (GAME->heroOut[idx] != 0) {
                return;
            }
        }
    }
    best = 0;
    if (idx == -1) {
        if (!(GAME->levelFlags & 0xE0)) {
            switch (GAME->mode) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 5:
            case 6:
            case 7:
            case 8:
            case 18:
            case 19:
            case 20:
            case 21:
            case 30:
            case 32:
                if (GAME->teams != 0 && GAME->mode != 0x20) {
                    HeroInfo *hb = Heroes;
                    s32 t0 = GAME->teamLeader[0];
                    s32 t1;
                    s32 dead0, dead1;
                    u16 mate0, mate1;
                    s32 h0, h1;

                    m = &hb[t0];
                    dead0 = m->hero_status & 0xF800;
                    h0 = 0;
                    if (!dead0) {
                        h0 = m->hero_health;
                    }
                    mate0 = m->hero_teammate;
                    h = &Heroes[mate0];
                    if (!(h->hero_status & 0xF800)) {
                        h0 += h->hero_health;
                    }
                    t1 = GAME->teamLeader[1];
                    h = &Heroes[t1];
                    dead1 = h->hero_status & 0xF800;
                    h1 = 0;
                    if (!dead1) {
                        h1 = h->hero_health;
                    }
                    mate1 = h->hero_teammate;
                    h = &Heroes[mate1];
                    if (!(h->hero_status & 0xF800)) {
                        h1 += h->hero_health;
                    }
                    if (h0 == h1) {
                        idx = -1;
                    } else if (h0 > h1) {
                        idx = mate0;
                        if (!dead0) {
                            idx = t0;
                        }
                    } else {
                        idx = mate1;
                        if (!dead1) {
                            idx = t1;
                        }
                    }
                } else {
                    for (i = 0; i < 4; i++) {
                        h = &Heroes[i];
                        if (!(h->hero_status & 0xF800)) {
                            if (best < h->hero_health) {
                                best = h->hero_health;
                                idx = i;
                            } else if (h->hero_health == best) {
                                idx = -1;
                            }
                        }
                    }
                }
                break;
            case 4:
            case 9:
                BossState.flags |= 0x200;
                break;
            }
            if (idx != -1) {
                HeroInfo *b1 = Heroes;
                HeroInfo *w = &b1[idx];

                w->hero_status |= 0x200;
                if (GAME->teams != 0 && GAME->mode != 0x20) {
                    m = &b1[(u16)w->hero_teammate];
                    if (!(m->hero_status & 0xA000)) {
                        m->hero_status |= 0x200;
                    }
                }
                GAME->winner[0] = Heroes[idx].hero_name;
                GAME->exitFlags |= 0x800;
            } else {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
            }
        }
    } else {
        GameState *g;
        HeroInfo *base = Heroes;

        h = &base[idx];
        g = GAME;
        n = (u16)h->hero_teammate;
        if (g->teams != 0 && g->mode != 0x20) {
            idx = HeroesInPlay;
            if (idx < 3) {
                m = &base[(u16)n];
                if (m->hero_status & 0xA000) {
                    if (idx < 2) {
                        h->hero_status |= 0x200;
                        g->winner[0] = h->hero_name;
                        g->exitFlags |= 0x800;
                    }
                } else {
                    h->hero_status |= 0x200;
                    m->hero_status |= 0x200;
                    g->winner[0] = h->hero_name;
                    g->exitFlags |= 0x800;
                }
            }
        } else if (HeroesInPlay < 2) {
            Heroes[idx].hero_status |= 0x200;
            GAME->winner[0] = Heroes[idx].hero_name;
            GAME->exitFlags |= 0x800;
        }
    }
    if (GameExitFlags & 0x800) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00)) {
                h->hero_status |= 0x400;
            }
        }
    }
    if (HeroesAllOut == 0) {
        HeroesAllOut = 1;
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00)) {
                if (GAME->heroOut[i] == 0) {
                    HeroesAllOut = 0;
                }
                if (GAME->teams != 0 && GAME->heroOut[h->hero_teammate] == 0) {
                    HeroesAllOut = 0;
                }
            }
        }
    }
}

void HeroTakeHit(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx) {
    HeroInfo *h;
    u8 *base;
    u8 *g;
    s32 dist;
    s32 v;

    if (Heroes[player].hero_status & 0xFF00) {
        return;
    }
    dist = AiPlayerSkill[player].level;
    base = GameInfo;
    g = base + player;
    v = 0x20 - (dist + g[0xCC]);
    if (v < 0) {
        v = 0;
    }
    if (v >= 0x20) {
        v = 0x1F;
    }
    if (g[0x5C] == 0) {
        v = HitScale[v] - base[0x9B];
    } else {
        v = 0;
    }
    if (HeroesAllOut != 0) {
        v = 0;
    }
    if (v < 0) {
        v = 0;
    }
    if (v >= 4) {
        v = 3;
    }
    if (((GameState *)GameInfo)->levelFlags & 3) {
        v = 0;
    }
    if (*(s32 *)(GameInfo + 4) & 1) {
        return;
    }
    if (flags & 0x800) {
        v = flags & 0xFF;
    } else {
        v = HitDamage[flags & 0xF][v];
    }
    if (!(((GameState *)GameInfo)->levelFlags & 3)) {
        if (GameInfo[player + 0x5C] != 0 && (u32)(dist -= 0x18) < 9) {
            v = HitFalloff[v >> 8][dist];
        }
    }
    if (flags & 0x1000) {
        v = 0x7FFFF;
        Heroes[player].hero_immunity = 0;
    }
    if (Heroes[player].hero_harm_value < v) {
        h = &Heroes[player];
        h->hero_harm_value = v;
        h->hero_harm_flags = flags;
        h->hero_harm_Nas = nas;
        h->hero_harm_player = attacker;
        h->hero_harm_sfx = sfx;
    }
}

s32 ProcessHeroAnim(player)
s32 player;
{
    HeroInfo *h;
    EXTanimator *anim = Heroes[player].hero_anim;
    subengine *sub;
    s32 ret;
    u16 cmd;

    ret = 0;
    if (--anim->Aspeed < 0) {
        h = &Heroes[player];
        do {
            if (anim->Seq_count != 0 && anim->Currentframe != 0) {
                cmd = anim->Currentframe + 1;
                anim->Seq_count--;
                continue;
            }
            cmd = ((hero_seq *)anim->Atable)[++anim->Aindex];
            if (!(cmd & 0x8000)) {
                break;
            }
            switch (cmd) {
            case 0x8002:
                ret = ((s16 *)anim->Atable)[++anim->Aindex];
                cmd = h->hero_sub->entity;
                break;
            case 0x8003:
                ret = ((s16 *)anim->Atable)[++anim->Aindex];
                break;
            case 0x8004:
                SetHeroAction(player, ((s16 *)anim->Atable)[++anim->Aindex]);
                return ret;
            case 0x8005:
                return SetHeroSeq(player, ((s16 *)anim->Atable)[++anim->Aindex]);
            case 0x8001:
                h->hero_anim->Atable = (u8 *)h->hero_anim->Atable + (h->hero_anim->Aindex * 2 + 2);
                anim->Aindex = 0;
                break;
            case 0x8000:
                anim->Aindex = ((s16 *)anim->Atable)[++anim->Aindex];
                do {
                    do {
                        do {
                            if (ret == 0) {
                                ret = 2;
                            }
                        } while (0);
                    } while (0);
                } while (0);
                break;
            case 0x8006:
                h->hero_sub->u6C.dbase = &HeroModelBank[((s16 *)anim->Atable)[++anim->Aindex]];
                break;
            case 0x8007:
                anim->Seq_count = ((s16 *)anim->Atable)[++anim->Aindex] - 1;
                anim->Currentframe = 0;
                break;
            case 0x8008:
                PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 0x8009:
                PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 0x800A:
                PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], &h->hero_eng->sfx, NULL, 0x1000, 0x1E00, 0);
                break;
            case 0x800B:
                if (h->hero_eng->sfx != NULL) {
                    StopSfx(h->hero_eng->sfx);
                }
                break;
            case 0x8017:
                HeroTakeHit(player, NULL, -1, 0x1000, 0);
                break;
            case 0x8018:
                anim->Masterspeed = ((s16 *)anim->Atable)[++anim->Aindex];
                break;
            case 0x8019:
                h->hero_move->jump_ctrl |= ((u8 *)anim->Atable)[++anim->Aindex * 2];
                break;
            case 0x801A:
                h->hero_move->jump_ctrl &= ~((u8 *)anim->Atable)[++anim->Aindex * 2];
                break;
            }
        } while (cmd & 0x8000);
        sub = anim->sub;
        if (sub == NULL) {
            sub = Heroes[player].hero_sub;
        }
        sub->entity = cmd;
        anim->Currentframe = cmd;
        anim->Aspeed = anim->Masterspeed;
    }
    return ret;
}

s32 SetHeroSeq(s32 player, s32 seq) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + player;
    u16 name = h->hero_name;

    if ((u32)(name - 1) < 8) {
        hero_seq **t = HeroSeqTables[name];

        h->hero_aseq = seq;
        h->hero_anim->Atable = t[seq];
        h->hero_anim->Aspeed = -1;
        h->hero_anim->Aindex = 0;
        h->hero_anim->Masterspeed = *(u16 *)h->hero_anim->Atable;
        h->hero_anim->Seq_count = 0;
        return ProcessHeroAnim(player, seq);
    }
    return 0;
}

s32 SetHeroAseq(s32 hero, u16 name, s32 seq) {
    HeroInfo *h;

    if ((u16)(name - 1) < 8) {
        hero_seq **t = HeroSeqTables[name];

        HeroInfo *base = Heroes;

        h = base + hero;
        h->hero_aseq = seq;
        h->hero_anim->Atable = t[seq];
        h->hero_anim->Aspeed = -1;
        h->hero_anim->Aindex = 0;
        h->hero_anim->Masterspeed = *(u16 *)h->hero_anim->Atable;
        h->hero_anim->Seq_count = 0;
        return ProcessHeroAnim(hero);
    }
    return 0;
}

void ClearHeroAnimSlots(void) {
    s32 i;
    s32 j;
    s32 off;
    s32 k;
    s32 tab;
    s32 m;
    s32 base;
    s32 *p;
    s32 *q;
    s32 t;
    s32 r;

    i = 0;
    tab = (s32)HeroOrders;
    off = 0;
    do {
        j = 0;
        m = off;
        do {
            *(s32 *)(m + tab) = j;
            j++;
            m += 4;
        } while (j < 4);
        j = 0;
        base = off;
        k = off;
        do {
            q = (s32 *)((Random(4) * 4 + base) + tab);
            p = (s32 *)(k + tab);
            t = *p;
            *p = *q;
            *q = t;
            k += 4;
        } while (++j < 4);
        i++;
        off += 16;
    } while (i < 16);
}

static inline void HeroAnimSlotSet(volatile s16 *id, u16 *seq, volatile s32 *a, volatile s32 *p, volatile s32 *p2, s32 hero, u16 n) {
    subengine *s;
    volatile VictorySeq *t;
    s32 q;
    s32 v;

    s = Heroes[hero].hero_anim->sub;
    if (s == NULL) {
        s = Heroes[hero].hero_sub;
    }
    *p = (s32)s->u6C.dbase;
    do {
    } while (0);
    q = *p2;
    *id = hero;
    t = (volatile VictorySeq *)(n * sizeof(VictorySeq) + (u32)*(VictorySeq * volatile *)&HeroHooks.VictorySeqs);
    v = ((VictorySeq *)t)->name;
    *a = v;
    *seq = t->seq;
    dbase_mesh_anim_name_load((u8 *)q, (u8 *)((VictorySeq *)t)->name);
}

void RequestHeroAnim(s32 arg0, u16 arg1) {
    if (HeroAnimDbaseA == 0) {
        HeroAnimSlotSet(&HeroAnimHeroA, &HeroAnimSeqA, (volatile s32 *)&HeroAnimNameA, (volatile s32 *)&HeroAnimDbaseA, (volatile s32 *)((u8 *)&HeroAnimDbaseB - 12), arg0, arg1);
    } else if (ModeTypes[GameMode * 3] != 0x104) {
        HeroAnimSlotSet(&HeroAnimHeroB, &HeroAnimSeqB, (volatile s32 *)&HeroAnimNameB, (volatile s32 *)&HeroAnimDbaseB, (volatile s32 *)((u8 *)&HeroAnimNameA + 16), arg0, arg1);
    }
}

s32 CheckHeroAnim(s32 arg0) {
    s32 i;

    if (HeroAnimHeroA == arg0) {
        if (Heroes[arg0].hero_aseq == HeroAnimSeqA) {
            return HeroAnimSeqA;
        }
        if (HeroAnimDbaseA != 0) {
            if (dbase_mesh_anim_name_loaded(HeroAnimDbaseA, HeroAnimNameA) != 0) {
                SetHeroSeq(arg0, HeroAnimSeqA);
                i = 3;
                do {
                    i -= 1;
                } while (i >= 0);
            }
        }
    }
    if (HeroAnimHeroB == arg0 && Heroes[arg0].hero_aseq != HeroAnimSeqB && HeroAnimDbaseB != 0) {
        if (dbase_mesh_anim_name_loaded(HeroAnimDbaseB, HeroAnimNameB) != 0) {
            SetHeroSeq(arg0, HeroAnimSeqB);
            i = 3;
            do {
                i -= 1;
            } while (i >= 0);
            return i;
        }
    }
}

void DefPreMoveHero(s32 arg0) {
    HeroInfo *hb;
    HeroInfo *hb2;
    PadStick *pb;
    HeroInfo *h;
    EXTheromove *m;
    PadStick *pad;
    u16 *row;
    u16 speed;
    HeroInfo *h2;
    u16 rw;
    s32 vc;

    hb = Heroes;
    h = hb + arg0;
    m = h->hero_move;
    m->forward_speed = 0;
    if (h->hero_frozen != 0) {
        ((PadStick *)Pads)[arg0].dirHeld2 = 0;
        ((PadStick *)Pads)[arg0].dirPressed = 0;
    }
    pb = (PadStick *)Pads;
    pad = pb + arg0;
    if ((pad->flags & 1) || h->hero_ai->AImode != 0) {
        if (pad->dirHeld2 & 0xF0) {
            m->forward_angle = *(u16 *)&pad->angle + 0x800;
            if (m->velocity_enable != 0) {
                rw = m->run_walk;
                if (h->hero_object != NULL) {
                    if (rw != 0) {
                        rw--;
                    }
                }
                row = HeroHooks.RunSpeeds;
                hb2 = Heroes;
                h2 = hb2 + arg0;
                speed = (row + h2->hero_name * 3)[rw];
                if (h2->hero_pu == 0x13) {
                    speed += (row + h2->hero_name * 3)[2];
                }
                vc = m->velocity_calc;
                if (vc != 1) {
                    if (vc != 0) {
                        if (vc < 4) {
                            m->x_max = (speed * rcossin_tbl[(m->forward_angle + 0x400) & 0xFFF].unk2) >> 12;
                            m->z_max = (speed * rcossin_tbl[(m->forward_angle + 0x800) & 0xFFF].unk2) >> 12;
                        }
                    }
                }
                m->forward_speed = speed;
            }
        } else {
            if (GameInfo[arg0 + 0x5C] != 0) {
                if (pad->dirHeld2 & 0x100) {
                    m->forward_angle -= 0x100;
                }
                if (pad->dirHeld2 & 0x200) {
                    m->forward_angle += 0x100;
                }
            }
            m->x_max = 0;
            m->z_max = 0;
            m->facing_angle = m->forward_angle = m->forward_angle & 0xFFF;
        }
    }
    m->forward_angle &= 0xFFF;
    if (abs(angle_diff(m->forward_angle, m->facing_angle) & 0xFFFF) > 0x110) {
        m->facing_angle = (m->forward_angle + 0x100) & 0xE00;
    }
}

void DefHeroFacing(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    if (h->hero_frozen == 0) {
        if (!(h->hero_ctrl & 1)) {
            h->hero_eng->unk1E += angle_diff(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
        }
        h->hero_ctrl &= 0xFFFE;
        h->hero_eng->unk5E = 1;
    }
}

void DefMoveHero(s32 arg0) {
    HeroInfo *h;
    EXTengine *e;
    EXTheromove *m;
    s32 step;
    HeroInfo *base;

    base = Heroes;
    h = base + arg0;
    e = h->hero_eng;
    m = h->hero_move;
    ((void (*)(s32))HeroHooks.HeroFacing)(arg0);
    HeroApplyJump(arg0);
    switch (m->velocity_calc) {
    case 2:
        m->x_vel += m->x_acc - m->x_dec;
        if (m->x_vel < 0) {
            m->x_vel = 0;
        }
        if (m->x_vel > m->x_max) {
            m->x_vel = m->x_max;
        }
        if (m->x_dir != 0) {
            e->unk10.x -= m->x_vel;
        } else {
            e->unk10.x += m->x_vel;
        }
        m->z_vel += m->z_acc - m->z_dec;
        if (m->z_vel < 0) {
            m->z_vel = 0;
        }
        if (m->z_vel > m->z_max) {
            m->z_vel = m->z_max;
        }
        if (m->z_dir != 0) {
            e->unk10.z -= m->z_vel;
        } else {
            e->unk10.z += m->z_vel;
        }
        break;
    case 3:
        if (m->x_vel != m->x_max) {
            if (m->x_max != 0) {
                step = m->x_acc;
            } else {
                step = m->x_dec;
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
        e->unk10.z += m->z_vel;
        e->unk10.x += m->x_vel;
        break;
    case 9:
        break;
    case 1:
    default:
        e->unk10.x -= (m->forward_speed * rcossin_tbl[(m->forward_angle - 0x400) & 0xFFF].unk2) >> 12;
        e->unk10.z -= (m->forward_speed * rcossin_tbl[m->forward_angle & 0xFFF].unk2) >> 12;
        if (m->reactive_speed != 0) {
            e->unk10.x -= (m->reactive_speed * rcossin_tbl[(m->reactive_angle - 0x400) & 0xFFF].unk2) >> 12;
            e->unk10.z -= (m->reactive_speed * rcossin_tbl[m->reactive_angle & 0xFFF].unk2) >> 12;
            m->reactive_speed -= 4;
            if (m->reactive_speed < 0) {
                m->reactive_speed = 0;
            }
        }
        break;
    }
}

void HeroApplyJump(s32 arg0) {
    HeroInfo *b0;
    HeroInfo *base;
    HeroInfo *h;
    EXTheromove *m;
    HeroInfo *h2;
    s32 dec;
    s32 v;
    s32 y;

    b0 = Heroes;
    h = b0 + arg0;
    if ((m = h->hero_move)->jump_max == 0) {
        return;
    }
    if (m->jump_dir != 0) {
        dec = m->jump_dec;
        if ((m->jump_ctrl & 1) && GameInfo[arg0 + 0x5C] == 0 &&
            !(((PadButtons *)Pads)[arg0].unk50 & 0x4000) && m->jump_vel >= 0x41) {
            dec = 0x28;
        }
        base = Heroes;
        h2 = base + arg0;
        v = h2->hero_move->jump_vel - dec;
        h2->hero_move->jump_vel = v;
        if (v < 0) {
            h2->hero_move->jump_vel = 0;
            h2->hero_move->jump_dir = 0;
            switch (h2->hero_mode) {
            case 0x33:
            case 0x39:
            case 0x3B:
            case 0x3C:
            case 0x3F:
                return;
            case 0x34:
                SetHeroActionInline(arg0, 0x35);
                break;
            case 0x78:
                SetHeroActionInline(arg0, 0x7A);
                break;
            default:
                SetHeroActionInline(arg0, 5);
                break;
            }
        } else {
            h2->hero_eng->unk10.y -= h2->hero_move->jump_vel;
        }
    } else {
        m->jump_vel += m->jump_acc;
        if (h->hero_move->jump_max < m->jump_vel) {
            h->hero_move->jump_vel = h->hero_move->jump_max;
        }
        h->hero_eng->unk10.y += h->hero_move->jump_vel;
        if (h->hero_move->jump_ctrl & 0x10) {
            y = h->hero_eng->unk10.y;
            if (World_floor_level < y) {
                h->hero_sub->unk08 += y - World_floor_level;
                h->hero_eng->unk10.y = World_floor_level;
            }
        }
    }
}

s32 HeroMovedDist(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    s32 dist;
    s32 dy;

    int new_var;
    s32 dz;
    s32 far;

    dist = abs(h->hero_eng->unk10.x - h->hero_eng->OldXYZ.x);
    dz = abs(h->hero_eng->unk10.z - h->hero_eng->OldXYZ.z);
    new_var = 0x40;
    dist = (dist + new_var) + dz;
    dy = ((s32 (*)(EXTengine *))ApplyContour)(h->hero_eng);
    if (dy == 0x7FFFFFFF) {
        h->hero_eng->unk10 = h->hero_eng->OldXYZ;
        return 0;
    }
    if (dy < 0) {
        dist = ~dist + 1;
        far = dy < dist;
    } else {
        far = dist < dy;
    }
    if (far) {
        return 0;
    }
    h->hero_eng->unk10.y -= dy;
    return 1;
}

s32 HeroTestBox(s32 id, u32 flag) {
    VECTOR pos;
    HeroInfo *base;
    HeroInfo *me;
    HeroInfo *h;
    EXTengine *eng;
    FlagRecord *box;
    FlagRecord *rec;
    s32 i;
    s32 r;
    s32 top;
    s32 bot;
    s32 y;
    s32 dx;
    s32 dz;
    s32 lo;
    s32 hi;
    s32 d;

    base = Heroes;
    h = &base[id];
    eng = h->hero_eng;
    box = FindCollisionBox(eng, flag);
    if (box == NULL) {
        return -1;
    }
    if (eng->unk5E != 0) {
        rotscale_update(&eng->unk1C);
    }
    gfx_load_xform(eng->sub, eng->unk3C, &eng->unk10);
    i = 0;
    me = h;
    h = base;
    r = box->radius;
    top = eng->unk10.y + box->unk2;
    bot = eng->unk10.y + box->height;
    gte_rotate_point((SVECTOR *)box, &pos);
    for (; i < 4; i++) {
        if (i == id) {
            continue;
        }
        if (h[i].hero_status & 0xF800) {
            continue;
        }
        eng = h[i].hero_eng;
        if (eng == NULL) {
            continue;
        }
        rec = FindCollisionBox(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        lo = eng->unk10.y + rec->unk2;
        hi = eng->unk10.y + rec->height;
        if (bot < lo && hi < top) {
            dx = eng->unk10.x - pos.vx;
            dz = eng->unk10.z - pos.vz;
            d = dx * dx + dz * dz;
            y = rec->radius + r;
            if (d < y * y) {
                if (me->hero_pu != 0) {
                    ((void (*)(s32, s32))HeroHooks.HeroesTouched)(id, i);
                }
                if (h[i].hero_pu != 0) {
                    ((void (*)(s32, s32))HeroHooks.HeroesTouched)(i, id);
                }
                return i;
            }
        }
    }
    return -1;
}

s32 HeroCopyPosStub(s32 *arg0) {
    s32 buf[4];

    *(Vec3W *)(buf + 1) = *(Vec3W *)(arg0 + 4);
    return 0;
}

void DefaultTouched(s32 arg0, s32 arg1) {
}

s32 DefKeepInBounds(void) {
    return 0;
}

s32 DefHeroKickCollision(void) {
    return 0;
}

s32 DefHeroGrabObject(void) {
    return 0;
}

s32 DefHero2ObjectCollision(void) {
    return 0;
}

void UpdateHeroShadow(s32 idx) {
    HeroInfo *h;
    EXTengine *s;
    EXTengine *e;

    if (HeroHooks.UpdateShadow != NULL) {
        HeroHooks.UpdateShadow();
    } else {
        HeroInfo *base = Heroes;
        h = base + idx;
        s = h->hero_shadow;
        e = h->hero_eng;
        s->unk10 = e->unk10;
        if (h->hero_status & 0xA000) {
            s->sub->flags &= 0xFFFF7FFF;
        }
        s->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(s);
    }
}

void MoveSecondHero(void) {
    DefMoveHero(1);
}

void DefHeroBot(void) {
}

void ReleaseHeroAnims(void) {
    if (HeroAnimDbaseA != 0) {
        dbase_mesh_anim_name_release((u8 *)HeroAnimDbaseA, (u8 *)HeroAnimNameA);
    }
    if (HeroAnimDbaseB != 0) {
        dbase_mesh_anim_name_release((u8 *)HeroAnimDbaseB, (u8 *)HeroAnimNameB);
    }
}

void ResetHeroGlobals(void) {
    BossState.unk0 = 0;
    BossState.unk2 = 0;
    BossState.flags = 0;
    BossState.health = 20;
    BossState.unk8 = 0;
}

void SetLevelCameraMode(void) {
    CameraMode = LevelRecs[LevelRecIndex].unkC;
}

void SetHeroAction(s32 player, s32 action) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + player;
    u16 old = h->hero_mode;

    h->hero_mode = action;
    h->hero_last_mode = old;
    HeroActionTable[action].fn();
}

void SetTweakAi(void) {
    AiSkill *t;
    u8 *g;
    s32 ret;
    s32 i;
    u16 flags;
    s32 c;
    s32 n;

    ret = -1;
    g = GameInfo;
    t = AiPlayerSkill;
    for (i = 0; i < 4; i++) {
        flags = *(u16 *)(g + 0x32);
        if (flags & 0x100) {
            c = g[*(s16 *)(g + 0x34) + 0xAB];
            g[i + 0x9B] = c;
            t[i].level = ((u16 *)AiSkillTable)[c];
            t[i].quality = 0x10;
        } else if (flags & 1) {
            t[i].level = ((u16 *)AiTeamSkillTable)[*(s16 *)(g + 0x38)];
            t[i].quality = 0x10;
        } else if (flags & 2) {
            t[i].level = ((u16 *)AiTeamSkillTable)[*(s16 *)(g + 0x38) + 3];
            t[i].quality = 0x10;
        } else if (flags & 0x40) {
            t[i].level = ((u16 *)AiSkillTable)[4];
            if (g[i + 0x5C] != 0) {
                ret = t[i].quality = ((u16 *)AiBotTweakTable)[4];
            } else {
                t[i].quality = ((u16 *)AiHumanTweakTable)[4];
            }
        } else if (flags & 0x20) {
            if (flags & 8) {
                t[i].level = ((u16 *)AiSkillTable)[9];
                if (g[i + 0x5C] != 0) {
                    ret = t[i].quality = ((u16 *)AiBotTweakTable)[9];
                } else {
                    t[i].quality = ((u16 *)AiHumanTweakTable)[9];
                }
            } else {
                t[i].level = ((u16 *)AiSkillTable)[5];
                if (g[i + 0x5C] != 0) {
                    ret = t[i].quality = ((u16 *)AiBotTweakTable)[5];
                } else {
                    t[i].quality = ((u16 *)AiHumanTweakTable)[5];
                }
            }
        } else if (flags & 0x80) {
            t[i].level = ((u16 *)AiSkillTable)[6];
            if (g[i + 0x5C] != 0) {
                ret = t[i].quality = ((u16 *)AiBotTweakTable)[6];
            } else {
                t[i].quality = ((u16 *)AiHumanTweakTable)[6];
            }
        } else if (flags & 0x10) {
            if (*(s32 *)(g + 0x70) != 0) {
                c = g[*(s16 *)(g + 0x34) + 0xAB];
                g[i + 0x9B] = c;
                t[i].level = ((u16 *)AiSkillTable)[3];
                if (g[i + 0x5C] != 0) {
                    ret = t[i].quality = ((u16 *)AiBotTweakTable)[3];
                } else {
                    t[i].quality = ((u16 *)AiHumanRoundTweakTable)[9];
                }
            } else {
                c = g[*(s16 *)(g + 0x34) + 0xAB];
                g[i + 0x9B] = c;
                t[i].level = ((u16 *)AiSkillTable)[c];
                if (g[i + 0x5C] != 0) {
                    ret = t[i].quality = ((u16 *)AiBotTweakTable)[g[0x9B]];
                } else {
                    n = g[i + 0xCC];
                    if (n >= 3) {
                        n = 2;
                    }
                    t[i].quality = ((u16 (*)[3])AiHumanRoundTweakTable)[c][n];
                }
            }
        }
    }
    AdjustAiSkill(ret);
    n = 0;
    do {
        flags = n < 4;
        n++;
    } while (flags);
}

s32 AdjustAiSkill(s32 arg0) {
    s32 v;
    u8 *g;
    s32 idx;

    if (arg0 < 0) {
        return -1;
    }
    g = GameInfo;
    idx = *(s16 *)(g + 0x42);
    if (idx == 0 ||*(s32 *)(g + 0x6C) != 0 || !(*(u16 *)(g + 0x32) & 0x10)) {
        return -1;
    }
    {
        u8 t = g[*(s16 *)(g + 0x34) + 0xAB];

        if (t == 0) {
            arg0 = arg0 / 2;
        }
        if (t == 1) {
            v = arg0 * 3;
            arg0 = v / 4;
        }
        if (GameInfo[*(s16 *)(GameInfo + 0x34) + 0xAB] == 3) {
            arg0 *= 2;
        }
        idx = *(s16 *)(GameInfo + 0x42);
        if (idx != -1) {
            AiPlayerSkill[idx].quality = arg0;
        }
        if (arg0 <= 0) {
            arg0 = 1;
        }
        return arg0;
    }
    return -1;
}

void MarkHeroesOnGrid(void) {
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
                ArenaGrid[z * 20 + x].flags |= flag;
                ArenaGrid[zlo * 20 + x].flags |= flag;
                ArenaGrid[zhi * 20 + x].flags |= flag;
                ArenaGrid[z * 20 + xlo].flags |= flag;
                ArenaGrid[zlo * 20 + xlo].flags |= flag;
                ArenaGrid[zhi * 20 + xlo].flags |= flag;
                ArenaGrid[z * 20 + xhi].flags |= flag;
                ArenaGrid[zlo * 20 + xhi].flags |= flag;
                ArenaGrid[zhi * 20 + xhi].flags |= flag;
            }
        }
    }
}

void ArenaGridScan(void) {
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

void ArenaGridClear(void) {
    s32 i = 399;
    ArenaCell *p = ArenaGrid;

    for (p += 399; i >= 0; i--) {
        p->flags = 0;
        p--;
    }
}

void ArenaGridReset(void) {
    s32 i;
    ArenaCell *p;

    for (i = 0, p = ArenaGrid; i < 400; i++) {
        p->unk1 = 0;
        p->flags = 0;
        p++;
    }
}

s32 HeroTestActions(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    PadButtons *pads;
    PadButtons *pad;
    PadButtons *pads2;
    PadButtons *pad2;
    s32 action;

    if (HeroHooks.TestAttack(arg0) != 0) {
        return 1;
    }
    base = Heroes;
    h = base + arg0;
    if (!(h->hero_status & 0x10)) {
        pads = (PadButtons *)Pads;
        pad = pads + arg0;
        if (pad->unk54 & 0x8000) {
            if (h->hero_attack_timer != 0) {
                pad->unk4C &= ~0x8000;
            } else {
                switch (h->hero_name) {
                case 1:
                case 2:
                    SetHeroAction(arg0, 8);
                    return 1;
                case 7:
                case 8:
                    SetHeroAction(arg0, 9);
                    return 1;
                case 3:
                case 4:
                    SetHeroAction(arg0, 10);
                    return 1;
                default:
                    SetHeroAction(arg0, 11);
                    return 1;
                }
            }
        }
    }
    if (GameMode != 0x1E) {
        pads2 = (PadButtons *)Pads;
        pad2 = pads2 + arg0;
        if (pad2->unk54 & 0x2000) {
            if (HeroHooks.HeroGrabObject(arg0, 0x280) != 0 || Heroes[arg0].hero_pick_delay == 0) {
                SetHeroAction(arg0, (u16)HeroActPickup);
                return 1;
            }
            pad2->unk4C &= ~0x2000;
        } else {
            Heroes[arg0].hero_pick_delay = 5;
        }
    }
    return 0;
}

s32 HeroTestFall(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    HeroInfo *base13;
    HeroInfo *h13;
    HeroInfo *base2;
    HeroInfo *h2;
    EXTengine *e;
    EXTengine *e2;
    EXTengine *p;
    s32 moved;

    if (h->hero_move->jump_max != 0) {
        if (ApplyContourMoved(h->hero_eng) != 0) {
            h->hero_move->jump_max = 0;
            h->hero_sub->unk08 = 0;
        } else if (!(h->hero_ctrl & 8)) {
            e = h->hero_eng;
            if (World_floor_level < e->unk10.y + h->hero_sub->unk08) {
                if (ContourHeight == 0x7FFFFFFF) {
                    goto set6;
                }
                e->unk10.y = World_floor_level;
                h->hero_sub->unk08 = 0;
            }
        }
    } else if ((moved = HeroMovedDist(arg0) != 1)) {
        switch (h->hero_mode) {
        case 0:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            SetHeroAction(arg0, 5);
            return 1;
        case 14:
        case 15:
        case 16:
        case 17:
        case 22:
        case 23:
        case 24:
        case 25:
            SetHeroAction(arg0, 5);
            return 1;
        case 13:
            base13 = Heroes;
            h13 = base13 + arg0;
            if (!(h13->hero_ctrl & 8) && World_floor_level < h13->hero_eng->unk10.y + h13->hero_sub->unk08) {
                if (ContourHeight == 0x7FFFFFFF) {
                set6:
                    SetHeroAction(arg0, 6);
                    return 1;
                }
            }
        default:
            base2 = Heroes;
            h2 = base2 + arg0;
            e2 = h2->hero_eng;
            p = e2->platform;
            if (p != NULL) {
                p->platform = NULL;
            }
            e2->platform = NULL;
            h2->hero_move->jump_vel = 8;
            h2->hero_move->jump_max = 0x70;
            h2->hero_move->jump_acc = 10;
            h2->hero_move->jump_dec = 0;
            h2->hero_move->jump_dir = 0;
            return 0;
        }
    }
    return 0;
}

void SetHeroIdle(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;

    h = base + arg0;

    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 1;
    HeroHooks.HeroSetIdle();
    SetHeroSeq(arg0, 1);
    h->hero->Camera.look_ahead = 0x400;
    if (h->hero_ctrl & 4) {
        e = h->hero_eng;
        e->unk2C.z = 0x1000;
        e->unk2C.y = 0x1000;
        e->unk2C.x = 0x1000;
        h->hero_ctrl &= 0xFFFB;
    }
}

void HeroIdle(s32 arg0) {
    s32 ret2;
    HeroInfo *h;
    HeroInfo *h2;
    HeroInfo *base;
    HeroInfo *base2;
    EXTheromove *m;
    EXTengine *e;

    if (HeroCheckStatus2000(arg0) == 0 && HeroCheckStatus600(arg0) == 0 &&
        HeroTestActions(arg0) == 0) {
        if (HeroHooks.TestJump != NULL) {
            ret2 = HeroHooks.TestJump(arg0);
        } else if (((PadButtons *)Pads)[arg0].unk54 & 0x4000) {
            SetHeroAction(arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0 && HeroCheckMoving(arg0) == 0 &&
            HeroCheckPad1000(arg0) == 0) {
            base = Heroes;
            h = base + arg0;
            m = h->hero_move;
            if (m->velocity_calc == 3) {
                e = h->hero_eng;
                if (e->unk10.y < World_floor_level) {
                    if (h->hero_aseq == 0xE) {
                        SetHeroSeq(arg0, 1);
                    }
                } else if (m->x_vel != 0 || m->z_vel != 0) {
                    if (h->hero_aseq != 1) {
                        goto idle_done;
                    }
                    PlaySfx(HeroIdleSfx[Random(6)], &h->hero_eng->sfx, NULL,
                            0x1000, 0x1E00, 0);
                    SetHeroSeq(arg0, 0xE);
                } else if (h->hero_aseq == 0xE) {
                    if (e->sfx != NULL) {
                        StopSfx(e->sfx);
                    }
                    SetHeroSeq(arg0, 1);
                }
            }
            base2 = Heroes;
            h2 = base2 + arg0;
            if (h2->hero_aseq == 1) {
                if (h2->hero_idle_count != 0) {
                    h2->hero_idle_count--;
                } else {
                    HeroHooks.HeroTouched(arg0);
                }
            }
        idle_done:
            ProcessHeroAnim(arg0);
            if (VelocityCalc < 4) {
                if (VelocityCalc >= 2) {
                    HeroHooks.HeroKickCollision(arg0);
                } else if (HeroHooks.HeroCarryCollision != NULL) {
                    HeroHooks.HeroCarryCollision(arg0);
                }
            } else if (HeroHooks.HeroCarryCollision != NULL) {
                HeroHooks.HeroCarryCollision(arg0);
            }
            HeroTestFall(arg0);
        }
    }
}

void HeroTaunt(s32 arg0) {
    s32 ret2;

    if (HeroCheckStatus2000(arg0) == 0 && HeroCheckStatus600(arg0) == 0 &&
        HeroTestActions(arg0) == 0) {
        if (HeroHooks.TestJump != NULL) {
            ret2 = HeroHooks.TestJump(arg0);
        } else if (((PadButtons *)Pads)[arg0].unk54 & 0x4000) {
            SetHeroAction(arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0 && HeroCheckMoving(arg0) == 0) {
            ProcessHeroAnim(arg0);
            if (VelocityCalc < 4) {
                if (VelocityCalc >= 2) {
                    HeroHooks.HeroKickCollision(arg0);
                }
            }
            HeroTestFall(arg0);
        }
    }
}

void HeroRun(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    s32 ret2;
    s32 id;

    if (HeroCheckStatus2000(arg0) == 0 && HeroCheckStatus600(arg0) == 0 &&
        HeroTestActions(arg0) == 0) {
        if (HeroHooks.TestJump != NULL) {
            ret2 = HeroHooks.TestJump(arg0);
        } else if (((PadButtons *)Pads)[arg0].unk54 & 0x4000) {
            SetHeroAction(arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0 && HeroCheckPad1000(arg0) == 0 && HeroCheckStopped(arg0) == 0) {
            base = Heroes;
            h = base + arg0;
            h->hero_move->velocity_enable = 1;
            HeroHooks.HeroKickCollision(arg0);
            switch (ProcessHeroAnim(arg0)) {
            case 8:
                if (h->hero_pu != 0x15) {
                    switch (h->hero_name) {
                    case 1: id = 0xA; break;
                    case 2: id = 0x3C; break;
                    case 3: id = 0x6E; break;
                    case 4: id = 0xA0; break;
                    case 7: id = 0x104; break;
                    case 8: id = 0xD2; break;
                    case 5: id = 0x136; break;
                    case 6: id = 0x168; break;
                    default: goto end;
                    }
                    PlaySfx(id, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                } else {
                    PlaySfx(0x41E, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                break;
            case 9:
                if (h->hero_pu == 0x15) {
                    break;
                }
                switch (h->hero_name) {
                case 1: id = 0xB; break;
                case 2: id = 0x3D; break;
                case 3: id = 0x6F; break;
                case 4: id = 0xA1; break;
                case 7: id = 0x105; break;
                case 8: id = 0xD3; break;
                case 5: id = 0x137; break;
                case 6: id = 0x169; break;
                default: goto end;
                }
                PlaySfx(id, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            }
        end:
            HeroTestFall(arg0);
        }
    }
}

void SetHeroJump(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *p;
    s32 idx;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    SetHeroSeq(arg0, 6);
    idx = 0;
    if (GameInfo[arg0 + 0x5C] == 0) {
        idx = h->hero_name;
    }
    h->hero_move->jump_vel = ((HeroJumpEnt *)HeroHooks.JumpEnts)[idx].vel;
    h->hero_move->jump_max = ((HeroJumpEnt *)HeroHooks.JumpEnts)[idx].max;
    h->hero_move->jump_acc = ((HeroJumpEnt *)HeroHooks.JumpEnts)[idx].acc;
    h->hero_move->jump_dec = ((HeroJumpEnt *)HeroHooks.JumpEnts)[idx].dec;
    h->hero_move->jump_dir = -1;
    h->hero->Camera.look_ahead = 0x400;
}

void SetHeroFall(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *p;
    EXTobject *o;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    h->hero_move->jump_vel = 8;
    h->hero_move->jump_max = 0x70;
    h->hero_move->jump_acc = 10;
    h->hero_move->jump_dec = 0;
    h->hero_move->jump_dir = 0;
    o = h->hero_object;
    if (o != NULL) {
        o->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq(arg0, 7);
}

void HeroFall(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    u8 *eng;

    if (ApplyContourMoved(h->hero_eng) != 0) {
        SetHeroAction(arg0, 7);
    } else if (!(h->hero_ctrl & 8)) {
        eng = (u8 *)h->hero_eng;
        if (World_floor_level < *(s32 *)(eng + 0x14) + *(s32 *)((u8 *)h->hero_sub + 8)) {
            if (ContourHeight == 0x7FFFFFFF) {
                SetHeroAction(arg0, 6);
            } else {
                *(s32 *)(eng + 0x14) = World_floor_level;
                *(s32 *)((u8 *)h->hero_sub + 8) = 0;
            }
        }
    }
    HeroHooks.HeroKickCollision(arg0);
    ProcessHeroAnim(arg0);
}

void SetHeroDrop(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *p;
    EXTengine *e;

    h = base + arg0;
    h->hero_eng->unk10.x &= ~0x1FF;
    h->hero_eng->unk10.x += 0x100;
    h->hero_eng->unk10.z &= ~0x1FF;
    h->hero_eng->unk10.z += 0x100;
    h->hero_move->velocity_enable = 0;
    h->hero_move->forward_speed = 0;
    h->hero_move->jump_ctrl |= 0x10;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    h->hero_move->jump_vel = 8;
    h->hero_move->jump_max = 0x70;
    h->hero_move->jump_acc = 0xA;
    h->hero_move->jump_dec = 0;
    h->hero_move->jump_dir = 0;
    h->hero_status |= 0x800;
    if (h->hero_last_mode != 0xD) {
        SetHeroSeq(arg0, 7);
    }
    PlaySfx(0x410, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
}

void SetHeroLand(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *o;
    EXTengine *t;
    u8 *x;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    h->hero_move->jump_max = 0;
    h->hero_sub->unk08 = 0;
    SetHeroSeq(arg0, 8);
    if (ContourPlatform != NULL) {
        e = h->hero_eng;
        o = ContourPlatform->Engine;
        t = e->platform;
        if (t != NULL) {
            t->platform = NULL;
            e->platform = NULL;
        }
        t = o->platform;
        if (t != NULL) {
            t->platform = NULL;
            o->platform = NULL;
        }
        e->platform = o;
        o->platform = e;
        x = FindExtension(ContourPlatform, 0x1000);
        if (x != NULL && (*(u16 *)(x + 0x1C) & 8)) {
            *(u16 *)(x + 0x1E) |= 8;
        }
    }
}

void HeroLand(s32 arg0) {
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction(arg0, (u16)HeroActWinner);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction(arg0, (u16)HeroActLoser);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0 && HeroTestActions(arg0) == 0) {
        s32 ret2;
        if (HeroHooks.TestJump != NULL) {
            ret2 = HeroHooks.TestJump(arg0);
        } else if (((PadButtons *)Pads)[arg0].unk54 & 0x4000) {
            SetHeroAction(arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (HeroHooks.TestRun(arg0) != 0) {
                ret = 1;
            } else {
                HeroInfo *base = Heroes;
                HeroInfo *hero = &base[arg0];
                if (hero->hero_move->forward_speed != 0) {
                    if (hero->hero_mode == 0xF || hero->hero_mode == 0x17) {
                        SetHeroAction(arg0, (u16)HeroActCarry);
                    } else {
                        SetHeroAction(arg0, 3);
                    }
                    ret = 1;
                } else {
                    ret = 0;
                }
            }
            if (ret == 0) {
                HeroHooks.HeroKickCollision(arg0);
                ProcessHeroAnim(arg0);
                HeroTestFall(arg0);
            }
        }
    }
}

void SetHeroSpin(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];
    NAS *nas;

    hero->hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
    nas = hero->hero;
    nas->Camera.look_ahead = nas->Camera.width;
}

void HeroSpin(s32 arg0) {
    s32 ret2;
    s32 id;
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *o;
    EXTengine *me;
    EXTengine *oe;
    EXTheromove *m;
    s32 angle;
    s32 vc;
    s32 idx;
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction(arg0, (u16)HeroActWinner);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction(arg0, (u16)HeroActLoser);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        if (HeroHooks.TestJump != NULL) {
            ret2 = HeroHooks.TestJump(arg0);
        } else if (((PadButtons *)Pads)[arg0].unk54 & 0x4000) {
            SetHeroAction(arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (HeroCheckPad1000(arg0) == 0) {
                base = Heroes;
                h = base + arg0;
                h->hero_eng->unk1E -= 0x155;
                idx = (s16)((h->hero_eng->unk1E & 0xFFF) / 341);
                idx = (s16)(idx % 12);
                h->hero_eng->unk2C.x = HeroSquashCycle[idx];
                h->hero_eng->unk2C.y = HeroSpinScale[idx];
                h->hero_eng->unk2C.z = HeroSquashCycle[idx];
                h->hero_ctrl |= 5;
                id = HeroTestBox(arg0, 0x400);
                if (id != -1) {
                    o = base + id;
                    if ((o->hero_immunity | o->hero_hit) == 0) {
                        me = h->hero_eng;
                        oe = o->hero_eng;
                        angle = get_angle(me->unk10.x - oe->unk10.x, me->unk10.z - oe->unk10.z);
                        m = o->hero_move;
                        vc = m->velocity_calc;
                        if (vc != 1) if (vc != 0) if (vc < 4) {
                            m->x_vel -= (rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 3) >> 8;
                            o->hero_move->z_vel -= (rcossin_tbl[angle & 0xFFF].unk2 * 3) >> 8;
                        }
                        Heroes[id].hero_move->reactive_angle = angle;
                        Heroes[id].hero_move->reactive_speed = 0x30;
                        HeroTakeHit(id, NULL, arg0, 0x8001, 0x446);
                    }
                }
                ((void (*)(s32, s32, s32, s32))HeroHooks.Hero2ObjectCollision)(arg0, 2, 0x400, 0);
                ((void (*)(s32))HeroHooks.HeroKickCollision)(arg0);
                ProcessHeroAnim(arg0);
                HeroTestFall(arg0);
            }
        }
    }
}

void SetHeroWhip(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq(arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
}

void HeroWhip(s32 arg0) {
    s32 ret2;
    s32 id;
    HeroInfo *base;
    HeroInfo *o;
    EXTengine *me;
    EXTengine *oe;
    EXTheromove *m;
    s32 angle;
    s32 vc;
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction(arg0, (u16)HeroActWinner);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction(arg0, (u16)HeroActLoser);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        if (HeroHooks.TestJump != NULL) {
            ret2 = HeroHooks.TestJump(arg0);
        } else if (((PadButtons *)Pads)[arg0].unk54 & 0x4000) {
            SetHeroAction(arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (HeroCheckPad1000(arg0) == 0) {
                id = HeroTestBox(arg0, 0x400);
                if (id != -1) {
                    base = Heroes;
                    o = base + id;
                    if ((o->hero_immunity | o->hero_hit) == 0) {
                        me = base[arg0].hero_eng;
                        oe = o->hero_eng;
                        angle = get_angle(me->unk10.x - oe->unk10.x, me->unk10.z - oe->unk10.z);
                        m = o->hero_move;
                        vc = m->velocity_calc;
                        if (vc != 1) if (vc != 0) if (vc < 4) {
                            m->x_vel -= (rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 3) >> 8;
                            o->hero_move->z_vel -= (rcossin_tbl[angle & 0xFFF].unk2 * 3) >> 8;
                        }
                        Heroes[id].hero_move->reactive_angle = angle;
                        Heroes[id].hero_move->reactive_speed = 0x30;
                        HeroTakeHit(id, NULL, arg0, 0x8001, 0x446);
                    }
                }
                ((void (*)(s32, s32, s32, s32))HeroHooks.Hero2ObjectCollision)(arg0, 2, 0x400, 0);
                ((void (*)(s32))HeroHooks.HeroKickCollision)(arg0);
                ProcessHeroAnim(arg0);
                HeroTestFall(arg0);
            }
        }
    }
}

void SetHeroShoot(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq(arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
    if (hero->hero_name == 3) {
        hero->hero_weapon_timer = -2;
    } else {
        hero->hero_weapon_timer = 0;
    }
}

void HeroShoot(s32 arg0) {
    s32 ret2;
    s32 ret3;
    s32 other;
    HeroInfo *base;
    HeroInfo *h2;
    HeroInfo *h;
    HeroInfo *self;
    HeroInfo *base2;
    s16 (*tbl)[3];
    EXTheromove *m;
    subengine *s;
    s16 *p;
    u16 t;
    s32 i;
    VECTOR pos;
    s32 angle;
    s32 vc;
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction(arg0, (u16)HeroActWinner);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction(arg0, (u16)HeroActLoser);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        if (HeroHooks.TestJump != NULL) {
            ret2 = HeroHooks.TestJump(arg0);
        } else if (((PadButtons *)Pads)[arg0].unk54 & 0x4000) {
            SetHeroAction(arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (HeroHooks.TestKick != NULL) {
                ret3 = HeroHooks.TestKick(arg0);
            } else if (Heroes[arg0].hero_status & 0xFA00) {
                ret3 = 0;
            } else if (((PadButtons *)Pads)[arg0].unk50 & 0x1000) {
                SetHeroAction(arg0, 2);
                ret3 = 1;
            } else {
                ret3 = 0;
            }
            if (ret3 == 0) {
                other = HeroTestBox(arg0, 0x400);
                if (other != -1) {
                    base = Heroes;
                    h2 = &base[other];
                    if ((h2->hero_immunity | h2->hero_hit) == 0) {
                        h = &base[arg0];
                        if (angle_diff_abs(h->hero_move->forward_angle,
                                          get_angle(h->hero_eng->unk10.x - h2->hero_eng->unk10.x,
                                                    h->hero_eng->unk10.z - h2->hero_eng->unk10.z)) < 0x200) {
                            angle = h->hero_move->forward_angle;
                            h2->hero_move->forward_angle = angle ^ 0x800;
                            m = h2->hero_move;
                            vc = m->velocity_calc;
                            if (vc != 1) {
                                if (vc != 0) {
                                    if (vc < 4) {
                                        m->x_vel -= (rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 3) >> 8;
                                        h2->hero_move->z_vel -= (rcossin_tbl[angle & 0xFFF].unk2 * 3) >> 8;
                                    }
                                }
                            }
                            Heroes[other].hero_move->reactive_angle = angle;
                            Heroes[other].hero_move->reactive_speed = 0x30;
                            HeroTakeHit(other, NULL, arg0, 0x8001, 0x446);
                        }
                    }
                }
                ((void (*)(s32, s32, s32, s32))HeroHooks.Hero2ObjectCollision)(arg0, 2, 0x400, 0x280);
                ((void (*)(s32))HeroHooks.HeroKickCollision)(arg0);
                ProcessHeroAnim(arg0);
                base2 = Heroes;
                self = &base2[arg0];
                s = self->hero_fx->sub;
                if (s != NULL && sub_find_point(self->hero_eng, self->hero_sub, 0x40, &pos) != NULL) {
                    self->hero_fx->unk10 = self->hero_eng->unk10;
                    t = self->hero_weapon_timer;
                    i = (s16)t;
                    if (i < 0) {
                        self->hero_weapon_timer = t + 1;
                    } else {
                        tbl = ThrowArcs;
                        p = tbl[i];
                        if (*p != 0) {
                            s->unk20.x = p[0];
                            s->unk20.y = p[1];
                            s->unk20.x = p[2];
                            s->unk04 = pos.vx - self->hero_eng->unk10.x;
                            s->unk08 = pos.vy - self->hero_eng->unk10.y;
                            s->unk0C = pos.vz - self->hero_eng->unk10.z;
                            s->flags |= 0x20008000;
                            s->unk52 = 1;
                            self->hero_weapon_timer++;
                        } else {
                            s->flags &= ~0x8000;
                        }
                    }
                }
                HeroTestFall(arg0);
            }
        }
    }
}

void SetHeroKick(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq(arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
}

void HeroKick(s32 arg0) {
    s32 ret2;
    s32 ret3;
    s32 other;
    HeroInfo *base;
    HeroInfo *h2;
    HeroInfo *h;
    EXTheromove *m;
    s32 angle;
    s32 vc;
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction(arg0, (u16)HeroActWinner);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction(arg0, (u16)HeroActLoser);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        if (HeroHooks.TestJump != NULL) {
            ret2 = HeroHooks.TestJump(arg0);
        } else if (((PadButtons *)Pads)[arg0].unk54 & 0x4000) {
            SetHeroAction(arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (HeroHooks.TestKick != NULL) {
                ret3 = HeroHooks.TestKick(arg0);
            } else if (Heroes[arg0].hero_status & 0xFA00) {
                ret3 = 0;
            } else if (((PadButtons *)Pads)[arg0].unk50 & 0x1000) {
                SetHeroAction(arg0, 2);
                ret3 = 1;
            } else {
                ret3 = 0;
            }
            if (ret3 == 0) {
                other = HeroTestBox(arg0, 0x400);
                if (other != -1) {
                    base = Heroes;
                    h2 = &base[other];
                    if ((h2->hero_immunity | h2->hero_hit) == 0) {
                        h = &base[arg0];
                        if (angle_diff_abs(h->hero_move->forward_angle,
                                          get_angle(h->hero_eng->unk10.x - h2->hero_eng->unk10.x,
                                                    h->hero_eng->unk10.z - h2->hero_eng->unk10.z)) < 0x200) {
                            angle = h->hero_move->forward_angle;
                            h2->hero_move->forward_angle = angle ^ 0x800;
                            m = h2->hero_move;
                            vc = m->velocity_calc;
                            if (vc != 1) {
                                if (vc != 0) {
                                    if (vc < 4) {
                                        m->x_vel -= (rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 3) >> 8;
                                        h2->hero_move->z_vel -= (rcossin_tbl[angle & 0xFFF].unk2 * 3) >> 8;
                                    }
                                }
                            }
                            Heroes[other].hero_move->reactive_angle = angle;
                            Heroes[other].hero_move->reactive_speed = 0x30;
                            HeroTakeHit(other, NULL, arg0, 0x8001, 0x446);
                        }
                    }
                }
                ((void (*)(s32, s32, s32, s32))HeroHooks.Hero2ObjectCollision)(arg0, 2, 0x400, 0x280);
                ((void (*)(s32))HeroHooks.HeroKickCollision)(arg0);
                ProcessHeroAnim(arg0);
                HeroTestFall(arg0);
            }
        }
    }
}

void HeroBecomeAngel(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *e;
    subengine *s;
    s32 i;

    h->hero_sub->u6C.dbase = &HeroModelBank[DB_Angel];
    e = h->hero_eng;
    s = h->hero_sub;
    e->flags |= 0x8021;
    s->flags |= 0x8021;
    e->unk20 = 0;
    e->unk1E = 0;
    e->unk1C = 0;
    s->unk14 = 0;
    s->unk12 = 0;
    s->unk10 = 0;
    h->hero_anim->sub = NULL;
    e->unk2C.x = 0x1000;
    e->unk2C.y = 0x1000;
    e->unk2C.z = 0x1000;
    s->unk20.x = 0x1000;
    s->unk20.y = 0x1000;
    s->unk20.z = 0x1000;
    h->hero_ctrl |= 1;
    e->unk5E = 1;
    s->unk52 = 1;
    e = h->hero_fx;
    s = e->sub;
    e->unk20 = 0;
    e->unk1E = 0;
    e->unk1C = 0;
    s->unk14 = 0;
    s->unk12 = 0;
    s->unk10 = 0;
    e->flags |= 0x8021;
    s->flags |= 0x8021;
    h->hero_move->jump_max = 0;
    h->hero_move->x_vel = 0;
    h->hero_move->x_max = 0;
    h->hero_move->z_vel = 0;
    h->hero_move->z_max = 0;
    h->hero_move->jump_ctrl = 0;
    s->u6C.dbase = &HeroModelBank[DB_Angel];
    s->entity = 0x2002;
    e->unk2C.x = 0x1000;
    e->unk2C.y = 0x1000;
    e->unk2C.z = 0x1000;
    s->unk20.x = 0x1000;
    s->unk20.y = 0x1000;
    s->unk20.z = 0x1000;
    e->unk5E = 1;
    s->unk52 = 1;
    e->unk10 = h->hero_eng->unk10;
    *(SLONG_XYZ *)&s->unk04 = *(SLONG_XYZ *)&h->hero_sub->unk04;
    for (s = s->next; s != NULL; s = s->next) {
        s->unk04 = 0;
        s->unk08 = 0;
        s->unk0C = 0;
        s->u6C.dbase = &HeroModelBank[DB_Angel];
        s->entity = 0x2003;
        s->flags = (s->flags | 0x60000021) & ~0x8000;
    }
    for (i = 0; i < 3; i++) {
        AngelSparkles[i][arg0].unk0 = Random(10) + i * 3;
        AngelSparkles[i][arg0].unk2 = 0;
        AngelSparkles[i][arg0].unk4 = 0;
        AngelSparkles[i][arg0].unk6 = 0;
        AngelSparkles[i][arg0].unk8 = 0;
        AngelSparkles[i][arg0].unkA = 0;
        AngelSparkles[i][arg0].unkC = 0;
    }
    if (Random(2) != 0) {
        PlaySfx(0x1C4, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    } else {
        PlaySfx(0x1C3, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
    SetHeroAseq(arg0, 1, 0xB);
}

void HeroGhost(arg0)
s32 arg0;
{
    s32 y;
    EXTengine *e;
    subengine *sub;
    subengine *s;
    s32 i;

    sub = Heroes[arg0].hero_sub;
    Heroes[arg0].hero_frozen = 2;
    y = sub->unk08;
    y -= 0x18;
    sub->unk08 = y;
    Heroes[arg0].hero_eng->unk5D = 4;
    Heroes[arg0].hero_sub->unk51 = 4;
    Heroes[arg0].hero_fx->unk5D = 4;
    Heroes[arg0].hero_fx->sub->unk51 = 4;
    Heroes[arg0].hero_eng->unk5C = 0;
    Heroes[arg0].hero_sub->unk50 = 0;
    Heroes[arg0].hero_fx->unk5C = 0;
    Heroes[arg0].hero_fx->sub->unk50 = 0;
    e = Heroes[arg0].hero_fx;
    s = e->sub;
    if (y < -0xA00) {
        Heroes[arg0].hero_status |= 0x8000;
        for (s = s->next; s != NULL; s = s->next) {
            s->flags &= ~0x8000;
        }
        return;
    }
    e->unk10 = Heroes[arg0].hero_eng->unk10;
    *(SLONG_XYZ *)&s->unk04 = *(SLONG_XYZ *)&Heroes[arg0].hero_sub->unk04;
    s->flags |= 0x8000;
    ProcessHeroAnim(arg0, y);
    for (s = s->next, i = 0; s != NULL; s = s->next, i++) {
        if (AngelSparkles[i][arg0].unk2 != 0) {
            AngelSparkles[i][arg0].unk6 -= 3;
            if (AngelSparkles[i][arg0].unk6 < 0) {
                AngelSparkles[i][arg0].unk6 = 0;
            }
            s->unk04 += AngelSparkles[i][arg0].unk6 * rcossin_tbl[(AngelSparkles[i][arg0].unk4 - 0x400) & 0xFFF].unk2 >> 12;
            s->unk0C += AngelSparkles[i][arg0].unk6 * rcossin_tbl[AngelSparkles[i][arg0].unk4 & 0xFFF].unk2 >> 12;
            y = AngelSparkles[i][arg0].unk8;
            y += AngelSparkles[i][arg0].unkA;
            if (y > 0x30) {
                y = 0x30;
            }
            AngelSparkles[i][arg0].unk8 = y;
            s->unk08 += y;
            s->flags |= 0x8000;
            s->unk76 = AngelSparkles[i][arg0].unkC += 0x60;
            if (World_floor_level < s->unk08 || AngelSparkles[i][arg0].unkC >= 0x1000) {
                AngelSparkles[i][arg0].unk2 = 0;
                AngelSparkles[i][arg0].unk0 = Random(10) + 2;
                s->flags &= ~0x8000;
            }
        } else if (AngelSparkles[i][arg0].unk0 != 0) {
            AngelSparkles[i][arg0].unk0--;
        } else {
            AngelSparkles[i][arg0].unk2 = 1;
            AngelSparkles[i][arg0].unk4 = Random(0x800) + 0x400;
            AngelSparkles[i][arg0].unk6 = 0x30;
            AngelSparkles[i][arg0].unk8 = -0x40;
            AngelSparkles[i][arg0].unkA = 4;
            AngelSparkles[i][arg0].unkC = 0x100;
            *(SLONG_XYZ *)&s->unk04 = *(SLONG_XYZ *)&Heroes[arg0].hero_fx->sub->unk04;
        }
    }
}

void StartHeroVictory(s32 arg0) {
    u16 mate;
    GameState *g;

    mate = Heroes[arg0].hero_teammate;
    music_set_tempo(100, 5);
    music_flag_restart();
    g = (GameState *)GameInfo;
    if (g->mode == 0x20) {
        if (HeroSide[GameInfo[0x60]] == 1) {
            WinSeqTimer = 0xF;
            WinSeqStep = 0xF;
            WinSeqDelay = 0x1E;
            WinSeqHero = arg0;
        } else {
            WinSeqTimer = 0xF;
            WinSeqStep = 0x10;
            WinSeqDelay = 0x1E;
            WinSeqHero = arg0;
        }
    } else if (g->levelFlags & 0x1E0) {
        if ((s16)mate != -1) {
            if (Heroes[(s16)mate].hero_status & 0x200) {
                if (!(arg0 & 1)) {
                    WinSeqTimer = 0x23;
                    WinSeqStep = 0x14;
                    WinSeqDelay = 0x1E;
                    WinSeqHero = arg0;
                }
            } else {
                WinSeqTimer = 0x23;
                WinSeqDelay = 0x1E;
                WinSeqStep = 0x14;
                WinSeqHero = arg0;
            }
        } else {
            WinSeqTimer = 0x23;
            WinSeqStep = 0x14;
            WinSeqDelay = 0x1E;
            WinSeqHero = arg0;
        }
    } else if ((s16)mate != -1) {
        if (Heroes[(s16)mate].hero_status & 0x200) {
            if (!(arg0 & 1)) {
                WinSeqTimer = 0x23;
                WinSeqStep = 9;
                WinSeqDelay = 0x1E;
                WinSeqHero = arg0;
            }
        } else {
            WinSeqTimer = 0x23;
            WinSeqStep = 9;
            WinSeqDelay = 0x1E;
            WinSeqHero = arg0;
        }
    } else {
        WinSeqTimer = 0x1E;
        WinSeqStep = 0;
        WinSeqDelay = 0x1E;
        WinSeqHero = arg0;
    }
}

s32 UpdateHeroVictory(s32 arg0) {
    HeroInfo *h;
    HeroInfo *p;
    s32 ret;
    s32 i;
    u16 t;

    ret = 0;
    if (arg0 != -1) {
        p = Heroes;
        h = &p[arg0];
        if (!(h->hero_ctrl & 2)) {
            h->hero_eng->unk1E += angle_diff(h->hero_eng->unk1E, ret) >> 2;
        }
        h->hero_ctrl |= 1;
        h->hero_eng->unk5E = 1;
    } else if ((u16)WinSeqStep < 9) {
        return 1;
    }
    if (WinSeqHero != arg0) {
        return ret;
    }
    t = WinSeqTimer;
    if (t != 0) {
        WinSeqTimer = t - 1;
    } else {
    switch ((u16)WinSeqStep) {
    case 0:
        switch (Heroes[arg0].hero_name) {
        case 1:
            PlaySfx(1,NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x18;
            break;
        case 2:
            PlaySfx(0x33, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x17;
            break;
        case 3:
            PlaySfx(0x65, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x1A;
            break;
        case 4:
            PlaySfx(0x97, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x17;
            break;
        case 7:
            PlaySfx(0xFB, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x1A;
            break;
        case 8:
            PlaySfx(0xC9, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x1A;
            break;
        case 5:
            PlaySfx(0x12D, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x17;
            break;
        case 6:
            PlaySfx(0x15F, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x17;
            break;
        default:
            WinSeqTimer = 0x5A;
            WinSeqStep = 3;
            break;
        }
        break;
    case 1:
        PlaySfx(0x1B0, NULL, NULL, 0x1000, 0x1E00, 0);
        WinSeqTimer = 0x1E;
        break;
    case 2:
        ret = 1;
        switch (Heroes[arg0].hero_name) {
        case 1:
            PlaySfx(7, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x12;
            break;
        case 2:
            PlaySfx(0x39, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x12;
            break;
        case 3:
            PlaySfx(0x6B, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x12;
            break;
        case 4:
            PlaySfx(0x9D, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x12;
            break;
        case 7:
            PlaySfx(0x101, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x12;
            break;
        case 8:
            PlaySfx(0xCF, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x12;
            break;
        case 5:
            PlaySfx(0x133, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x12;
            break;
        case 6:
            PlaySfx(0x165, NULL, NULL, 0x1000, 0x1E00, 0);
            WinSeqTimer = 0x12;
            break;
        default:
            WinSeqTimer = 0x12;
            WinSeqStep = 3;
            break;
        }
        break;
    case 9:
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_name == GameInfo[0x60]) {
                if (i & 2) {
                    PlaySfx(0x1AE, NULL, NULL, 0x1000, 0x1E00, 0);
                    WinSeqTimer = 0xE;
                } else {
                    PlaySfx(0x1AE, NULL, NULL, 0x1000, 0x1E00, 0);
                    WinSeqTimer = 0xB;
                }
                break;
            }
        }
        break;
    case 10:
        WinSeqTimer = 0;
        WinSeqStep = 0;
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_name == GameInfo[0x60]) {
                if (i & 2) {
                    PlaySfx(0x1A5, NULL, NULL, 0x1000, 0x1E00, 0);
                    WinSeqTimer = 0x16;
                    WinSeqStep = 0;
                } else {
                    PlaySfx(0x1A4, NULL, NULL, 0x1000, 0x1E00, 0);
                    WinSeqTimer = 0x1B;
                    WinSeqStep = 0;
                }
                break;
            }
        }
        break;
    case 15:
        PlaySfx(0x1B1, NULL, NULL, 0x1000, 0x1E00, 0);
        WinSeqTimer = 0x10;
        WinSeqStep = 0;
        break;
    case 16:
        PlaySfx(0x1B2, NULL, NULL, 0x1000, 0x1E00, 0);
        WinSeqTimer = 0x14;
        WinSeqStep = 0;
        break;
    case 21:
        PlaySfx(0x1B9, NULL, NULL, 0x1000, 0x1E00, 0);
        WinSeqTimer = 0x1B;
        break;
    case 22:
        ret = 1;
        WinSeqStep = 1;
        WinSeqTimer = 0x1B;
        break;
    case 20:
    case 30:
        PlaySfx(0x1B8, NULL, NULL, 0x1000, 0x1E00, 0);
        WinSeqTimer = 0xC;
        break;
    case 31:
        PlaySfx(0x1BA, NULL, NULL, 0x1000, 0x1E00, 0);
        WinSeqTimer = 0x27;
        break;
    case 32:
        ret = 1;
        WinSeqStep = 3;
        WinSeqTimer = 2;
        break;
    case 40:
        PlaySfx(0x1BB, NULL, NULL, 0x1000, 0x1E00, 0);
        WinSeqTimer = 0x1E;
        break;
    case 41:
        ret = 1;
        WinSeqStep = 3;
        WinSeqTimer = 0x1B;
        break;
    default:
        WinSeqTimer = 0xB4;
        WinSeqStep = 3;
        break;
    }
    WinSeqStep++;
    }
    return ret;
}

void FinishLevel(void) {
    u8 *p = GameInfo;

    if (*(s32 *)(p + 4) & 0x800) {
        *(u16 *)(p + 0x30) |= 0x201;
        return;
    }
    *(u16 *)(p + 0x30) |= 1;
}

void HeroDrop(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    s32 sum;

    h->hero_frozen = 2;
    sum = *(s32 *)((u8 *)h->hero_eng + 0x14) + *(s32 *)((u8 *)h->hero_sub + 8);
    if (World_floor_level + 0x180 < sum) {
        SetHeroAction(arg0, (u16)HeroActDead);
    }
    HeroHooks.HeroKickCollision(arg0);
    ProcessHeroAnim(arg0);
}

void HeroJump(s32 arg0) {
    HeroHooks.HeroKickCollision(arg0);
    ProcessHeroAnim(arg0);
}

void SetHeroRun(s32 arg0) {
    u8 rw;

    Heroes[arg0].hero_move->velocity_enable = 1;
    do {
        rw = Heroes[arg0].hero_move->run_walk;
    } while (0);
    Heroes[arg0].hero_move->run_walk = rw;
    SetHeroSeq(arg0, 5);
    HeroHooks.HeroKickCollision(arg0);
}

void SetHeroTaunt(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 0;
    SetHeroSeq(arg0, 2);
}

void DefHeroTouched(s32 arg0) {
    Heroes[arg0].hero_idle_count = Random(0x258) + 0x12C;
}

void DefHeroSetIdle(s32 arg0) {
    Heroes[arg0].hero_idle_count = Random(0x96) + 0x5A;
}

void HeroQuiet(void) {
    ProcessHeroAnim();
}

void SetHeroQuiet(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 0;
    SetHeroSeq(arg0, 0);
}

s32 HeroNoJump(s32 arg0) {
    PadState *base = Pads;
    PadState *pad = &base[arg0];

    pad->unk50 &= ~0x4000;
    pad->unk54 &= ~0x4000;
    return 0;
}

s32 HeroNoKick(void) {
    return 0;
}

s32 HeroIsWinnerOrLoser(s32 arg0) {
    u16 temp_v1;
    u32 m;

    temp_v1 = Heroes[arg0].hero_status;

    if (temp_v1 & 0x200) {
        return 1;
    }
    m = temp_v1 & 0x400;
    return m != 0;
}

s32 HeroCheckDying(s32 arg0) {
    s32 ret;

    if (!(Heroes[arg0].hero_status & 0x2000)) {
        ret = 0;
    } else {
        SetHeroAction(arg0, (u16)HeroActDead);
        ret = 1;
    }
    return ret;
}

s32 HeroCheckResult(s32 arg0) {
    u16 temp_v1;
    u16 act;

    temp_v1 = Heroes[arg0].hero_status;
    if (temp_v1 & 0x200) {
        act = HeroActWinner;
    } else if (temp_v1 & 0x400) {
        act = HeroActLoser;
    } else {
        return 0;
    }
    SetHeroAction(arg0, act);
    return 1;
}

s32 HeroTestStop(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    u16 mode;
    s32 a1;

    h = base + arg0;
    if (h->hero_move->forward_speed != 0) {
        return 0;
    }
    mode = h->hero_mode;
    if (mode == 0x10 || mode == 0x18) {
        a1 = (u16)HeroActHold;
    } else {
        a1 = 0;
    }
    SetHeroAction(arg0, a1);
    return 1;
}

s32 HeroTestMove(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 mode;
    s32 a1;

    if (HeroHooks.TestRun(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        if (h->hero_move->forward_speed != 0) {
            mode = h->hero_mode;
            if (mode == 0xF || mode == 0x17) {
                SetHeroAction(arg0, (u16)HeroActCarry);
            } else {
                SetHeroAction(arg0, 3);
            }
            return 1;
        }
        return 0;
    }
    return 1;
}

void DebugNasStatus(void) {
}

s32 UpdateFxEmitter(u8 *arg0, u8 *arg1, u8 *arg2, s32 *arg3) {
    volatile s32 t0;
    volatile s32 t1;
    EmitPart *part;
    s32 *st;
    u8 *def;
    u8 *obj;
    u8 *p;
    s32 n;
    s32 i;
    s32 t;
    s32 r;

    obj = arg1;
    i = 0;
    part = *(EmitPart **)(arg2 + 0x30);
    st = *(s32 **)(arg2 + 0x34);
    t0 = VSync(1);
    def = NULL;
    p = get_script_track(((FxOwner *)arg0)->fx, ((FxEmitter *)arg2)->unk24, ((FxEmitter *)arg2)->unk4);
    if (p != NULL) {
        def = p + 0x14;
    }
    if (def != NULL) {
        n = 0;
        if (*(s32 *)(arg2 + 0x2C) <= *(s32 *)(def + 0x10)) {
            st[4] = 0;
            for (;;) {
                s32 life;

                if (n >= *(s32 *)(def + 0x14)) {
                    break;
                }
                if (!(*(s32 *)obj & 0x8000)) {
                emit_particle(obj, part, def);
                life = *(s32 *)(def + 0x18);
                if (life == 0) {
                    life = *(s32 *)(arg2 + 0x20);
                }
                if (*(s32 *)(def + 0x58) != 0) {
                    st[0] = 0x1000 / *(s32 *)(def + 0x58);
                    *(s16 *)(obj + 0x76) = 0x1000;
                }
                if (*(s32 *)(def + 0x5C) != 0) {
                    st[1] = 0x1000 / (life - *(s32 *)(def + 0x5C));
                }
                if (*(s32 *)(def + 0x60) != 0) {
                    st[2] = 0x1000 / *(s32 *)(def + 0x60);
                    *(s32 *)(obj + 0x28) = 0;
                    *(s32 *)(obj + 0x24) = 0;
                    *(s32 *)(obj + 0x20) = 0;
                }
                if (*(s32 *)(def + 0x64) != 0) {
                    if (life != *(s32 *)(def + 0x64)) {
                        st[3] = 0x1000 / (life - *(s32 *)(def + 0x64));
                    } else {
                        st[3] = 0;
                    }
                }
                if (*(s32 *)(def + 0x68) != 0) {
                    *(s16 *)(obj + 0x14) = Random(0x1000);
                }
                n++;
                *(s32 *)(arg2 + 0x2C) += 1;
                }
                obj = *(u8 **)(obj + 0x5C);
                part++;
            }
        }
        part = *(EmitPart **)(arg2 + 0x30);
        obj = arg1;
        for (; i < *(s32 *)(def + 0x10); i++, obj = *(u8 **)(obj + 0x5C), part++) {
            if (obj == NULL || !(*(s32 *)obj & 0x8000)) {
                continue;
            }
            t = *(s32 *)(def + 0x58);
            if (t != 0 && part->age < t) {
                if ((*(s16 *)(obj + 0x76) -= st[0]) < 0) {
                    *(s16 *)(obj + 0x76) = 0;
                }
            }
            t = *(s32 *)(def + 0x5C);
            if (t != 0 && t < part->age) {
                if ((*(s16 *)(obj + 0x76) += st[1]) > 0x1000) {
                    *(s16 *)(obj + 0x76) = 0x1000;
                }
            }
            t = *(s32 *)(def + 0x60);
            if (t != 0 && t >= part->age) {
                if ((*(s32 *)(obj + 0x20) += st[2]) <= 0) {
                    *(s32 *)(obj + 0x20) = 0;
                }
                *(s32 *)(obj + 0x24) = *(s32 *)(obj + 0x28) = *(s32 *)(obj + 0x20);
            }
            t = *(s32 *)(def + 0x64);
            if (t != 0 && part->age >= t) {
                if ((*(s32 *)(obj + 0x20) -= st[3]) <= 0) {
                    *(s32 *)(obj + 0x20) = 0;
                }
                *(s32 *)(obj + 0x24) = *(s32 *)(obj + 0x28) = *(s32 *)(obj + 0x20);
            }
            vel_update((VelObj *)obj, (VelParams *)def, (VelState *)part);
            if (!(*(s32 *)obj & 0x8000) && *(s32 *)(def + 0x6C) != 0) {
                if ((*(s32 *)(arg2 + 0x2C) -= 1) < 0) {
                    *(s32 *)(arg2 + 0x2C) = 0;
                }
                part->age = 0;
            }
        }
    }
    if (*arg3 == 1) {
        obj = arg1;
        while (obj != NULL) {
            s32 t;
    s32 r;

            *(s32 *)obj &= ~0x8000;
            *(s32 *)(obj + 0x20) = 0x1000;
            *(s32 *)(obj + 0x28) = 0x1000;
            *(s32 *)(obj + 0x24) = 0x1000;
            *(s16 *)(obj + 0x76) = 0;
            t = *(s32 *)(def + 0x14);
            obj = *(u8 **)(obj + 0x5C);
            *(s32 *)(arg2 + 0x2C) = t;
        }
        *arg3 = 0;
    }
    if (*(s32 *)(arg0 + 0x14) >= *(s32 *)(def + 0x1C) << 8) {
        *arg3 = 2;
    }
    t1 = VSync(1);
    t1;
    t0;
    return st[9];
}

void ModelCreateShadow(s32 idx) {
    EXTengine *s;
    EXTengine *e;

    if (GameInfo[idx + 0x50] != 9) {
        s = Heroes[idx].hero_shadow;
        if (((GameState *)GameInfo)->mode != 0x1C) {
            s->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            s->sub->entity = 0x202F;
        } else {
            s->sub->u6C.dbase = &HeroModelBank[DB_DragonObjects];
            s->sub->entity = 0x2014;
        }
        e = Heroes[idx].hero_eng;
        s->unk10 = e->unk10;
        s->flags |= 0x8041;
        s->sub->unk76 = 0x400;
        s->sub->flags |= 0x40000000;
        if (((GameState *)GameInfo)->mode == 0x17) {
            s->unk2C.x = s->unk2C.y = s->unk2C.z = 0x2000;
        }
        if (((GameState *)GameInfo)->mode == 0x19) {
            s->unk2C.x = s->unk2C.y = s->unk2C.z = 0x2000;
        }
        if (((GameState *)GameInfo)->mode == 0x1A) {
            s->unk2C.x = s->unk2C.y = s->unk2C.z = 0x2000;
        }
        if (((GameState *)GameInfo)->mode == 0x18) {
            s->unk2C.x = s->unk2C.y = s->unk2C.z = 0x2000;
        }
    }
}

void ModelUpdateShadow(s32 idx) {
    HeroInfo *h;
    EXTengine *s;
    EXTengine *e;

    if (GameInfo[idx + 0x50] != 9) {
        HeroInfo *base = Heroes;
        h = base + idx;
        s = h->hero_shadow;
        e = h->hero_eng;
        s->unk10 = e->unk10;
        if (((GameState *)GameInfo)->mode == 0x17) {
            s->unk10.y = 0;
        }
        if (((GameState *)GameInfo)->mode == 0x19) {
            s->unk10.y = 0;
        }
        if (((GameState *)GameInfo)->mode == 0x1A) {
            s->unk10.y = 0;
        }
        if (((GameState *)GameInfo)->mode == 0x18) {
            s->unk10.y = 0;
        }
        s->unk1E = h->hero_eng->unk1E;
        s->unk5E = 1;
        if (h->hero_status & 0xA400) {
            s->sub->flags &= 0xFFFF7FFF;
        }
    }
}

void InitScriptFX(NAS *nas)
{
  u32 id;
  EXTscriptdata *sd;
  u8 *dbase;
  u8 *scripts;
  u8 *hdr;
  NAS *new_var;
  script_thread *th;
  u8 *r;
  u8 *k;
  u8 *kf;
  s32 *ent;
  threaddata *t;
  subengine *sub;
  s32 i;
  s32 j;
  EXTengine *eng;
  EXT *fux;
  s32 type;
  s32 frames;
  s32 keys;

  new_var = nas;
  eng = new_var->Engine;
  if (eng == 0)
  {
    KillNAS(new_var);
    return;
  }
  eng->flags |= 0x8000;
  new_var->Engine->sub->flags = 0x42008000;
  new_var->Engine->unk5D = 0;
  new_var->Engine->unk5C = 1;
  eng = new_var->Engine;
  eng->unk2C.z = 0x1000;
  eng->unk2C.y = 0x1000;
  eng->unk2C.x = 0x1000;
  fux = new_var->FUX;
  if (fux == 0)
  {
    sd = AddExt(new_var, 0x3BC, 0x10000000);
    new_var->FUX = (EXT *) sd;
    if (sd == 0)
    {
      goto kill;
    }
    if (new_var->trigger == 0)
    {
      return;
    }
  }
  else
  {
    sd = (EXTscriptdata *) fux;
  }

  if (sd == 0)
  {
    return;
  }
  if (!(sd->flags & 4))
  {

    new_var->Engine->sub->u6C.dbase = MapScene->dbase;

  }

  scripts = new_var->Engine->sub->u6C.dbase;
  if (scripts == 0)
  {
    printf("script dbase NULL (probably not loaded)\n");
    goto kill;
  }

  dbase = scripts;

  sd->dbase = (obj_dbase *) dbase;
  scripts = *((u8 **) (dbase + 0xC));
  if (scripts == 0)
  {
    return;
  }
  sub = new_var->Engine->sub;
  sd->times = 0;
  id = 0;
  if (new_var->trigger != 0)
  {
    MapTriggerInfo *info = new_var->trigger->InfoStruct;
    sd->scriptnumber = info->val1;
    sd->speed = info->val2;
    if (sd->speed == 0)
    {
      sd->speed = 0x100;
    }
    id = info->val3;
    sd->maxloops = 0;
    sd->flags = 0;
  }

  sd->timer = 0;
  if ((*((s32 *) (scripts + 0x48))) == 0)
  {
    return;
  }
  if (sd->scriptnumber >= (*((s32 *) (scripts + 0x48))))
  {
    printf("requested script %d out of range\n", sd->scriptnumber);
    goto kill;
  }

  hdr = (scripts + ((*((s32 *) (scripts + 0x4C))) + 0x4C)) + (sd->scriptnumber * 4);
  hdr += *((s32 *) hdr);
  sd->threadcount = *((s32 *) hdr);
  sd->subcount = *((s32 *) (hdr + 4));
  ent = (s32 *) (hdr + 0x1C);
  if (eng_add_subs(new_var->Engine, sd->subcount) != sd->subcount)
  {
    for (i = 0; i < sd->threadcount; i++)
    {
      t = &sd->threads[i];
      t->threadinfo = 0;
      t->threadcontrol = 0;
    }

    goto kill;
  }

  i = 0;
  if (sd->threadcount != 0)
  {
    goto start;
  }
  goto kill;
  nokey:
  printf("must define more than one keyframe\n");

  kill:
  {

    KillNAS(new_var);
  }

  return;

  start:
  {

    new_var->Engine->kill = KillScriptFx;
  }

  while (1)
  {
    if (i >= sd->threadcount)
    {
      break;
    }
    t = &sd->threads[i];
    th = (script_thread *) (((u8 *) ent) + (*ent));  /* the header's self-relative offset to the thread */
    type = th->thread_type;
    frames = th->total_frames;
    keys = th->key_frame_count;
    sd->totalframes = frames;
    t->threadtype = type;
    t->scriptnumber = sd->scriptnumber;
    t->keyindex = 0;
    t->keycount = keys;
    t->framecount = frames;
    t->start = new_var->Engine->unk10;
    t->threadnumber = i;
    t->timer = 0;
    t->threadinfo = 0;
    t->threadcontrol = 0;
    switch (t->threadtype)
    {
      case 0:
        if (keys == 0)
      {
        goto nokey;
      }
        if (sub == 0)
      {
        break;
      }
        sub->u6C.dbase = dbase;
        sub->unk68 = id;
        sub->flags = 0x42000000;
        sub->flags = (((u32) th->type.entity[1]) | 0x42000000) & (~0x8000);
        if (sd->flags & 8)
      {
        sub->unk51 = 3;
      }
      else
      {
        sub->unk51 = 2;
      }
        sub->unk50 = 1;
        r = get_script_track(dbase, t->scriptnumber, t->threadnumber);
        kf = 0;
        if (r != 0)
      {
        kf = r + 0x24;
      }
        if (kf == 0)
      {
        break;
      }
        sub->unk10 = *((u16 *) (kf + 0x18));
        sub->unk12 = *((u16 *) (kf + 0x1C));
        sub->unk14 = *((u16 *) (kf + 0x20));
        sub->unk04 = *((s32 *) (kf + 0xC));
        sub->unk08 = *((s32 *) (kf + 0x10));
        sub->unk0C = *((s32 *) (kf + 0x14));
        sub->unk20.x = *((s32 *) (kf + 0x40));
        sub->unk20.y = *((s32 *) (kf + 0x44));
        sub->unk20.z = *((s32 *) (kf + 0x48));
        sub->entity = *((u16 *) (kf + 0x8));
        sub->unk52 = 1;
        sub = sub->next;
        break;

      case 1:
        r = get_script_track(dbase, t->scriptnumber, t->threadnumber);
        k = 0;
        if (r != 0)
      {
        k = r + 0x14;
      }
        if (k == 0)
      {
        break;
      }
        if (mal_first_free(((u32) ((*((s32 *) (k + 0x10))) * 0x28)) >> 2, MainHeap.free) == 0)
      {
        goto kill;
      }
        mal_alloc_owned_low((*((s32 *) (k + 0x10))) * 0x28, &t->threadinfo, (s32) (&MainHeap), 0);
        if (mal_first_free(0xA, MainHeap.free) == 0)
      {
        goto kill;
      }
        mal_alloc_owned_low(0x28, &t->threadcontrol, (s32) (&MainHeap), 0);
        t->particlesactive = *((s32 *) (k + 0x14));
        ((pcontrol *) t->threadcontrol)->freqcounter = 0;
        j = 1;
        if ((*((s32 *) (k + 0x10))) > 0)
      {
        do
        {
          if (sub != 0)
          {
            sub->flags = 0x42000000;
            if ((*((u32 *) k)) & 0x20000000)
            {
              sub->flags = 0x62000000;
            }
            sub->u6C.dbase = dbase;
            sub->unk68 = id;
            sub->flags &= ~0x8000;
            if (sd->flags & 8)
            {
              sub->unk51 = 3;
            }
            else
            {
              sub->unk51 = 2;
            }
            sub->unk50 = 1;
            sub->unk04 = 0;
            sub->unk08 = 0;
            sub->unk0C = 0;
            sub->entity = *((u16 *) (k + 0x28));
            sub->unk20.x = 0x1000;
            sub->unk20.z = 0x1000;
            sub->unk20.y = 0x1000;
            sub->unk76 = 0;
          }
          sub = sub->next;
        }
        while ((j++) < (*((s32 *) (k + 0x10))));
      }
        ((pcontrol *) t->threadcontrol)->lastsub = sub;
        break;

    }

    ent++;
    i++;
  }


}

void PlotScriptFX(NAS *nas) {
    volatile s32 t0 = 0;
    volatile s32 t1 = 0;
    EXTscriptdata *sd;
    subengine *sub;
    threaddata *t;
    threaddata *u;
    ScriptKey *kf;
    ScriptKey *cur;
    ScriptKey *next;
    ScriptKey *p;
    u8 *r;
    s32 i;
    s32 j;
    s32 k;
    s32 state;
    s32 f;
    s32 d;
    s32 start;
    s32 nf;
    s32 alpha;
    u32 fl;
    s32 first;

    t0 = VSync(1);
    if (((GameState *)GameInfo)->mode == 0x2C && ScriptFxOn != 0) {
        return;
    }
    sd = (EXTscriptdata *)nas->FUX;
    i = 0;
    if (sd != NULL) {
        sub = nas->Engine->sub;
        sd->timer += sd->speed;
        while (1) {
            if (i >= sd->threadcount) {
                break;
            }
            t = &sd->threads[i];
            switch (t->threadtype) {
            case 1:
                state = 0;
                if (sd->timer >= sd->totalframes << 8) {
                    state = 1;
                }
                sub = (subengine *)UpdateFxEmitter((u8 *)sd, (u8 *)sub, (u8 *)t, &state);
                if (nas->trigger == NULL && state == 2) {
                    j = 0;
                    while (1) {
                        if (j >= sd->threadcount) {
                            break;
                        }
                        u = &sd->threads[j];
                        if (u->threadinfo != NULL) {
                            mal_free(u->threadinfo, &MainHeap);
                        }
                        if (u->threadcontrol != NULL) {
                            mal_free(u->threadcontrol, &MainHeap);
                        }
                        j++;
                    }
                    KillTriggerNas(nas);
                    return;
                }
                break;
            case 0:
                r = get_script_track((u8 *)sd->dbase, t->scriptnumber, t->threadnumber);
                kf = NULL;
                if (r != NULL) {
                    kf = (ScriptKey *)(r + 0x24);
                }
                if (kf != NULL) {
                    t->fdata = kf;
                    p = &kf[t->keyindex];
                    start = p->frame;
                    cur = p;
                    p++;
                    next = p;
                    nf = next->frame;
                    d = nf - start;
                    first = kf->frame;
                    p = &kf[t->keycount - 1];
                    if (p->frame < sd->timer >> 8 || sd->timer >> 8 < first) {
                        sub->flags &= ~0x8000;
                    } else {
                        sub->flags |= 0x8000;
                    }
                    f = 0;
                    do {} while (0);
                    f = 0;
                    do {} while (0);
                    f = 0;
                    if (sd->timer - start > 0) {
                        f = sd->timer - (start << 8);
                    }
                    if (sub != NULL && (sub->flags & 0x8000)) {
                        sub->unk52 = 1;
                        if (sub->unk51 == 3) {
                            quat_slerp(&cur->quat, &next->quat, (f << 12) / d >> 8, (Quat *)&sub->unk10);
                        } else {
                            sub->unk12 = *(u16 *)&cur->ry - ((-next->ry + cur->ry) << 20 >> 20) * f / (d << 8);
                            sub->unk52 = 1;
                            sub->unk10 = *(u16 *)&cur->rx - ((-next->rx + cur->rx) << 20 >> 20) * f / (d << 8);
                            sub->unk52 = 1;
                            sub->unk14 = *(u16 *)&cur->rz + ((next->rz - cur->rz) << 20 >> 20) * f / (d << 8);
                            sub->unk52 = 1;
                        }
                        sub->unk04 = cur->x + (next->x - cur->x) * f / (d << 8);
                        sub->unk0C = cur->z + (next->z - cur->z) * f / (d << 8);
                        sub->unk08 = cur->y + (next->y - cur->y) * f / (d << 8);
                        sub->unk20.x = cur->sx + (next->sx - cur->sx) * f / (d << 8);
                        sub->unk52 = 1;
                        sub->unk20.y = cur->sy + (next->sy - cur->sy) * f / (d << 8);
                        sub->unk52 = 1;
                        sub->unk20.z = cur->sz + (next->sz - cur->sz) * f / (d << 8);
                        fl = sub->flags & ~0x40000000;
                        sub->flags = fl;
                        alpha = cur->fade + (next->fade - cur->fade) * f / (d << 8);
                        sub->unk76 = 0x1000 - alpha;
                        sub->flags = fl | 0x40000000;
                    }
                    p = NULL;
                    if (sd->timer >= nf << 8) {
                        t->keyindex++;
                    }
                    if (sd->timer >= t->framecount << 8) {
                        if (sd->flags & 1) {
                            sd->times++;
                            if (sd->maxloops < sd->times) {
                                k = 0;
                                while (1) {
                                    if (k >= sd->threadcount) {
                                        break;
                                    }
                                    u = &sd->threads[k];
                                    if (u->threadinfo != NULL) {
                                        mal_free(u->threadinfo, &MainHeap);
                                    }
                                    if (u->threadcontrol != NULL) {
                                        mal_free(u->threadcontrol, &MainHeap);
                                    }
                                    k++;
                                }
                                KillTriggerNas(nas);
                                return;
                            }
                        }
                        sub->unk20.x = 0x1000;
                        sub->unk20.z = 0x1000;
                        sub->unk20.y = 0x1000;
                        sub->unk76 = 0;
                        sub->unk52 = 1;
                        if (((GameState *)GameInfo)->mode == 0x2C) {
                            sub->flags &= ~0x8000;
                        }
                        t->keyindex = 0;
                    }
                }
                sub = sub->next;
                break;
            }
            i++;
        }
        if (sd->timer >= sd->totalframes << 8) {
            sd->timer = 0;
            if (((GameState *)GameInfo)->mode == 0x2C) {
                ScriptFxOn = 1;
            }
        }
    }
    t1 = VSync(1);
    t1;
    t0;
}

void PlotSmokePoof(NAS *m) {
    s32 w = m->UserInfo.user1;
    EXTengine **u;
    EXTengine *e;
    s32 x;
    s32 z;

    if (w != 0) {
        u = (EXTengine **)w;
        if (GameMode != 0x1C) {
            m->Engine->unk10.x += (u[1]->unk10.x - m->Engine->unk10.x) >> 4;
            z = (u[1]->unk10.z - (e = m->Engine)->unk10.z) >> 4;
            x = e->unk10.z;
        } else {
            m->Engine->unk10.x += (u[1]->unk10.x - m->Engine->unk10.x) / 6;
            z = (u[1]->unk10.z - (e = m->Engine)->unk10.z) / 6;
            x = e->unk10.z;
        }
        x += z;
        e->unk10.z = x;
        m->Engine->unk10.y = u[1]->unk10.y - ((u[1]->unk2C.x * 0x1A4) >> 12);
        m->Engine->unk2C.x = u[1]->unk2C.x;
        m->Engine->unk2C.y = u[1]->unk2C.y;
        m->Engine->unk2C.z = u[1]->unk2C.z;
    }
    RunNasAnim(m);
}

s32 PointInZone(s32 *arg0, s32 arg1) {
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

    base = ((u8 *)MapScene->map);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        n = *(s32 *)(p + 0x38);
        t = rcossin_tbl;
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

s32 ArcUpdate(s32 *from, s32 *to, Arc *arc, s32 *pos) {
    s32 dx;
    s32 dz;
    s32 dist;
    s32 n;
    s32 dy;
    s32 hit;
    s32 ang;

    hit = 0;
    if (arc->started == 0) {
        dx = to[0] - from[0];
        dz = to[2] - from[2];
        n = 0;
        dist = csqrt(dx * dx + dz * dz) >> 6;
        dy = to[1] - from[1];
        if (arc->speed != 0) {
            n = dist / arc->speed;
        }
        if (n == 0) {
            return 1;
        }
        arc->vy = arc->vy0 = dy / n - (n >> 1) * arc->gravity;
        ang = get_angle(dx, dz) & 0xFFF;
        arc->angle = ang;
        arc->vx = arc->speed * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
        arc->vz = arc->speed * rcossin_tbl[arc->angle & 0xFFF].unk2 >> 12;
        arc->started = 1;
    }
    pos[1] += arc->vy;
    arc->vy += arc->gravity;
    pos[0] += arc->vx;
    pos[2] += arc->vz;
    if (pos[1] >= arc->floor && arc->vy > 0) {
        pos[1] -= abs(pos[1] - arc->floor);
        hit = 1;
    }
    return hit;
}

void PlotSparks(SparkObj *obj) {
    SparkPoly *p0;
    Spark *s;
    SparkPoly *p1;
    SparkTex *tex;
    s32 n;
    s16 a;
    s32 t;
    s32 r;
    SparkSet *ss;

    ss = &obj->set;
    n = ss->count;
    p0 = ss->polys0;
    p1 = ss->polys1;
    s = obj->set.sparks;
    tex = (SparkTex *)((u8 *)HeroModelBank[DB_ArenaBossBearArena].textures.tex + 0xAF0);
    if (GameMode == 0x22) {
        tex = (SparkTex *)(*(u8 **)((u8 *)MapScene->dbase + 0x18) + 0x2BF8);
    }
    if (GameMode == 0x13) {
        tex = (SparkTex *)(*(u8 **)((u8 *)MapScene->dbase + 0x18) + 0x6C8);
    }
    if (GameMode == 0x1D) {
        tex = (SparkTex *)(*(u8 **)((u8 *)MapScene->dbase + 0x18) + 0xA10);
    }
    for (; n != 0; n--) {
        do {
        do {
        a = Random(0x2000);
        a -= Random(0x2000);
        s->x = a;
        s->y = -Random(0x1B58);
        a = Random(0x2000);
        r = Random(0x2000);
        } while (0);
        } while (0);
        a -= r;
        s->z = a;
        s->y2 = s->y + 0x40;
        s->z2 = s->z;
        t = Random(0xFFF);
        s->ang = t;
        s->base_x = s->x;
        s->x = s->base_x + ((rcossin_tbl[(t - 0x400) & 0xFFF].unk2 * 512 >> 12));
        s->x2 = s->x + 0x40;
        s++;
        *(u32 *)&p0->r0 = 0xA0A0A0;
        p0->tpage = (tex->tpage & 0xFF9F) | 0x20;
        p0->clut = tex->clut;
        p0->uv0 = tex->uv0;
        p0->uv1 = tex->uv1;
        p0->uv2 = tex->uv2;
        p0->uv3 = tex->uv3;
        ((u8 *)&p0->tag)[3] = 9;
        p0->code = 0x2E;
        *p1++ = *p0++;
    }
}

void PlotFallParticles(u8 *arg0, MATRIX *m) {
    FallParticles *fp = (FallParticles *)(arg0 + 0x6C);
    FallParticle *p;
    POLY_FT4 *prim;
    s32 n;
    s32 col;
    s32 tag2;
    s16 *sxy;
    s32 tag;
    s32 t;
    s32 r;
    SVECTOR *v;

    p = fp->parts;
    prim = fp->prims[FrameCount & 1];
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
    n = fp->count;
    col = calc_rgbcode(0x10, 0x10, 0x10);
    tag = col | 0x2E000000;
    while (n != 0) {
        p->top.vy += 0x18;
        p->top.vx = p->base + (rcossin_tbl[(p->angle - 0x400) & 0xFFF].unk2 * 512 >> 12);
        p->angle = (p->angle + 0x10) & 0xFFF;
        if (p->top.vy > 0x800) {
            p->angle = Random(0xFFF);
            p->base = Random(0x3000) - Random(0x3000);
            p->top.vx = p->base + (rcossin_tbl[(p->angle - 0x400) & 0xFFF].unk2 * 512 >> 12);
            t = Random(0x4000) - Random(0x1000);
            p->top.vz = t;
            p->top.vy = -0xDAC;
            p->bot.vx = p->top.vx + 0x40;
            p->bot.vz = p->top.vz;
        }
        p->bot.vy = p->top.vy + 0x40;
        p->bot.vx = p->top.vx + 0x40;
        v = &p->top;
        do { gte_ldv0(v); } while (0);
        gte_rtps();
        tag2 = tag;
        sxy = &prim->x0;
        do { *(s32 *)&prim->r0 = tag2; } while (0);
        gte_stsxy(sxy);
        v = &p->bot;
        gte_ldv0(v);
        gte_rtps();
        prim->y1 = prim->y0;
        prim->x2 = prim->x0;
        gte_stsxy(&prim->x3);
        prim->x1 = prim->x3;
        prim->y2 = prim->y3;
        addPrim(RenderState.ot + 0x40, prim);
        prim++;
        p++;
        n--;
    }
}

void PlotWeatherNas(u8 *arg0) {
    if (*(s32 *)(arg0 + 0x60) == 0) {
        s32 v = *(s32 *)(arg0 + 0x64);
        switch (v) {
        case 0:
            fx_alloc_buffers(&WeatherFxDef, *(subengine **)(*(u8 **)(arg0 + 0x54) + 0x6C));
            *(s32 *)(arg0 + 0x60) = 1;
            break;
        case 1:
            *(s32 *)(arg0 + 0x60) = v;
            break;
        }
    }
}

void InitWeatherNas(u8 *arg0) {
    *(s32 *)(arg0 + 0x60) = 0;
    *(s32 *)(arg0 + 0x64) = 0;
    **(s32 **)(*(u8 **)(arg0 + 0x54) + 0x6C) = 0x20008000;
    if (GameMode == 0x1F) {
        fx_alloc_buffers(&NoxSparkFxDef, *(subengine **)(*(u8 **)(arg0 + 0x54) + 0x6C));
    }
}

void WeatherUnusedA(void) {
}

void WeatherUnusedB(void) {
}

FlagPoint *SubFindFlagPoint(subengine *sub, s32 idx, s32 mask) {
    PointList *list = sub_point_list(sub);
    u8 *p;
    s32 i;
    s32 n;

    if (list != NULL) {
        n = list->nRecs;
        p = (u8 *)list->pts;
        i = 0;
        if (--n != -1) {
            do {
                if (p != NULL && i == idx && (((u32 *)p)[3] & 0xC0000000) == mask) {
                    return (FlagPoint *)p;
                }
                i++;
                p += 0x10;
            } while (--n != -1);
        }
    }
    return NULL;
}

s32 DistXZSq(SLONG_XYZ *a, SLONG_XYZ *b) {
    s32 dx;
    s32 dz;

    dx = a->x - b->x;
    dz = a->z - b->z;
    return (dx * dx) + (dz * dz);
}

void PlotParticleEffect(void) {
}

void InitParticleEffect(void) {
}

void InitSmokePoof(NAS *nas) {
    ScriptCursorFirst(nas, &SmokePoofInit);
    nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
    nas->Engine->sub->flags |= 0x40000041;
    nas->UserInfo.user1 = 0;
    if (((GameState *)GameInfo)->mode == 0x1C) {
        nas->Engine->sub->flags = 0x40008021;
    }
    if (((GameState *)GameInfo)->mode == 0x17) {
        nas->Engine->unk2C.x = 0x800;
        nas->Engine->unk2C.y = 0x800;
        nas->Engine->unk2C.z = 0x800;
    }
}

void KillScriptFx(EXTengine *eng) {
    NAS *nas;
    EXTscriptdata *sd;
    threaddata *t;
    s32 i;

    nas = ((NAS **)eng)[-1];
    if (nas != NULL) {
        sd = FindExtension(nas, 0x10000000);
        if (sd != NULL) {
            i = 0;
            while (1) {
                if (i >= sd->threadcount) {
                    break;
                }
                t = &sd->threads[i];
                if (t->threadinfo != NULL) {
                    mal_free(t->threadinfo, &MainHeap);
                }
                if (t->threadcontrol != NULL) {
                    mal_free(t->threadcontrol, &MainHeap);
                }
                i++;
            }
            eng_release_subs_sfx(eng);
        }
    }
}

void emit_script_particle(u8 *arg0, u8 *arg1, EmitPart *arg2, s32 *arg3, u8 *arg4) {
    s32 life;
    s32 t;
    s32 r;
    s32 u;

    emit_particle(arg0, arg2, arg1);
    life = *(s32 *)(arg1 + 0x18);
    if (life == 0) {
        life = *(s32 *)(arg4 + 0x20);
    }
    t = *(s32 *)(arg1 + 0x58);
    if (t != 0) {
        arg3[0] = 0x1000 / t;
        *(s16 *)(arg0 + 0x76) = 0x1000;
    }
    u = *(s32 *)(arg1 + 0x5C);
    if (u != 0) {
        arg3[1] = 0x1000 / (life - u);
    }
    t = *(s32 *)(arg1 + 0x60);
    if (t != 0) {
        arg3[2] = 0x1000 / t;
        *(s32 *)(arg0 + 0x28) = 0;
        *(s32 *)(arg0 + 0x24) = 0;
        *(s32 *)(arg0 + 0x20) = 0;
    }
    u = *(s32 *)(arg1 + 0x64);
    if (u != 0) {
        if (life != u) {
            arg3[3] = 0x1000 / (life - u);
        } else {
            arg3[3] = 0;
        }
    }
    if (*(s32 *)(arg1 + 0x68) != 0) {
        *(s16 *)(arg0 + 0x14) = Random(0x1000);
    }
    *(s32 *)(arg4 + 0x2C) += 1;
}

u8 *script_track_keys(u8 *arg0, s32 *arg1) {
    u8 *r;

    r = get_script_track(arg0, arg1[0x24 / 4], arg1[1]);
    if (r != NULL) {
        return r + 0x24;
    }
    return NULL;
}

u8 *script_track_data(u8 *arg0, u8 *arg1) {
    u8 *p = get_script_track(arg0, *(s32 *)(arg1 + 0x24), *(s32 *)(arg1 + 4));
    if (p != NULL) {
        return p + 0x14;
    }
    return NULL;
}

void PlotFloatyScore(NAS *nas) {
    EXTengine *eng = nas->Engine;
    s32 s;
    s32 one;
    s32 ang;
    s32 y;

    one = 1;
    if (eng != NULL) {
        switch (nas->UserInfo.status) {
        case 0:
            if (nas->UserInfo.user1 <= 0 || --nas->UserInfo.user1 <= 0) {
                nas->UserInfo.status = one;
                nas->UserInfo.user1 = 0;
            }
            break;
        case 1:
            eng->unk10.y -= 0x20;
            if (nas->UserInfo.user2 - (rcossin_tbl[0xC00].unk2 * 3 >> 7) >= eng->unk10.y) {
                nas->UserInfo.status = 2;
                nas->UserInfo.flags = 0x1E;
            }
            break;
        case 2:
            ang = ++nas->UserInfo.user1;
            y = nas->UserInfo.user2;
            ang = rcossin_tbl[((ang << 7) & 0xFFF) - 0x400 & 0xFFF].unk2;
            eng->unk5E = one;
            eng->unk10.y = y - (ang * 3 >> 7);
            s = eng->unk2C.x + 0x40;
            eng->unk2C.z = s;
            eng->unk2C.y = s;
            eng->unk2C.x = s;
            if (--nas->UserInfo.flags == 0) {
                nas->UserInfo.status = 3;
                nas->UserInfo.flags = 0x3C;
                nas->UserInfo.user1 = 6;
            }
            break;
        case 3:
            eng->unk10.y -= nas->UserInfo.user1;
            eng->unk5E = one;
            eng->unk2C.x = eng->unk2C.y = eng->unk2C.z -= (eng->unk2C.x - 0x800) >> 5;
            nas->UserInfo.user1 += 3;
            if (--nas->UserInfo.flags == 0) {
                KillNAS(nas);
            }
            break;
        }
    }
}

void FormatPercent(u8 *arg0, s32 arg1) {
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

void InitFloatyScore(NAS *nas) {
    EXTengine *e;

    e = nas->Engine;
    if (e != NULL) {
        nas->UserInfo.flags = 0x1E;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = 0;
        nas->UserInfo.user2 = e->unk10.y - 0x1A0;
        e->unk5D = 2;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void CreateFloatyScore(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    EXTengine *eng;
    NAS *nas;
    subengine *sub;
    s32 off;

    if (arg3 >= 0x64) {
        arg3 = 0x63;
    }
    nas = CreateNAS(arg0, arg1, arg2, 0x1409, 0x11);
    if (nas != NULL) {
        eng = nas->Engine;
        nas->UserInfo.user1 = arg4;
        if (eng != NULL) {
            off = 0;
            if (arg3 >= 10) {
                eng_add_subs(eng, 1);
            }
            sub = eng->sub;
            if (sub != NULL) {
                while (sub != NULL) {
                    sub->u6C.dbase = &HeroModelBank[DB_Interface];
                    sub->unk04 = off;
                    sub->entity = arg3 % 10 + 0x200E;
                    sub = sub->next;
                    arg3 /= 10;
                    if (sub != NULL) {
                        off -= 0xB4;
                    }
                }
            }
        } else {
            KillNAS(nas);
        }
    }
}

void UpdateBounceMotion(NAS *nas) {
    EXTengine *eng;
    EXTengine *next;
    EXTmotion *m;
    s32 dy;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    m = FindExtension(nas, 0x800);
    if (m == NULL) {
        return;
    }
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
    dy = ((s32 (*)(EXTengine *))ApplyContour)(eng);
    next = eng->Engine;
    if (next != NULL) {
        next->unk10 = eng->unk10;
        next->unk10.y -= dy;
    }
    if (m->Yvel == 0) {
        s16 acc = m->Ydec;
        m->Ydec = 0;
        m->Ydir = 0;
        m->Yacc = acc;
    }
    if (m->Ydir == 0 && ApplyContourMoved(eng) != 0) {
        m->Ydec = 8;
        m->Ydir = 0xFF;
        m->Yacc = 0;
        m->Yvel = 0x40;
    }
}

void Init_HealthPU(NAS *nas) {
    EXTengine *eng;
    EXTobject *obj;
    EXTmotion *mot;
    EXTengine *e2;
    s32 x;
    s32 z;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        nas->UserInfo.user2 = GameFrameCount;
        eng->sub->entity = Random(0xE) + 0x2018;
        obj = (EXTobject *)AddObject(nas);
        if (obj != NULL) {
            LinkObject(nas);
            AddSolid(nas, 0x200);
            nas->FUX = (EXT *)obj;
            obj->whatami = 0x11;
            obj->can_do |= 0x18;
            SetEngineColBox(eng, 0, 0, 0, 0x80, -0x100, 0x40);
            if (eng->unk10.y == World_floor_level) {
                obj->status = 1;
            } else {
                obj->status = 4;
            }
            mot = AddMotion(nas);
            if (mot != NULL) {
                mot->Ymax = 0xA0;
                mot->Ydec = 0xE;
                mot->Yacc = 0;
                mot->Ydir = 0xFF;
                mot->Yvel = 0;
            }
            e2 = AddExtEngine(nas, 1);
            if (e2 != NULL) {
                e2->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
                e2->sub->entity = 0x2001;
                e2->unk10 = nas->Engine->unk10;
                e2->flags |= 0x8041;
                e2->sub->unk76 = 0x800;
                e2->sub->flags |= 0x40000000;
            }
            PlaySfx(0x3FA, NULL, NULL, 0x1000, 0x1E00, 0);
            x = (eng->unk10.x >> 8) + 10;
            z = 9 - (eng->unk10.z >> 8);
            if (x >= 20) {
                x = 19;
            }
            if (x < 0) {
                x = 0;
            }
            if (z >= 20) {
                z = 19;
            }
            if (z < 0) {
                z = 0;
            }
            ArenaGrid[z * 20 + x].flags = 0x11;
        }
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Plot_HealthPU(NAS *nas) {
    EXTengine *eng;
    EXTobject *obj;
    EXTmotion *mot;
    s32 p;
    s32 n;
    s32 st;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    nas->UserInfo.user2++;
    eng->sub->entity = (nas->UserInfo.user2 >> 1) % 14 + 0x2018;
    obj = (EXTobject *)nas->FUX;
    if (obj == NULL) {
        return;
    }
    st = obj->status;
    if (st < 6) {
        if (st < 4) {
            goto plot;
        }
        mot = FindExtension(nas, 0x800);
        if (mot != NULL && GravitateNas(nas) != 0) {
            if (obj->status == 4) {
                PlaySfx(HealthPickupSfx[Random(8)], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
            obj->status = 1;
            mot->Ydec = 8;
            mot->Ydir = 0xFF;
            mot->Yacc = 0;
            mot->Yvel = 0x40;
        }
        obj->will_do &= ~8;
        return;
    }
plot:
    UpdateBounceMotion(nas);
    if (obj->will_do & 8) {
        p = obj->player;
        if (p != -1 && p < 5 && !(Heroes[p].hero_status & 0xFA00)) {
            if (GameInfo[p + 0x5C] != 0) {
                n = 3 - GameInfo[0x9B];
            } else {
                n = 0;
            }
            if (HeroesAllOut != 0) {
                n = 0;
            }
            Heroes[obj->player].hero_health += HealthPickupAmounts[n];
            if (Heroes[obj->player].hero_health > 20) {
                Heroes[obj->player].hero_health = 20;
            }
            PlaySfx(0x1C2, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        KillNAS(nas);
    }
}

void PlotBigPoof(NAS *nas) {
    RunNasAnim(nas);
}

void InitBigPoof(NAS *nas) {
    EXTengine *eng = nas->Engine;

    if (eng != NULL) {
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        eng->sub->entity = 0;
        ScriptCursorFirst(nas, &BigPoofInit);
        eng->flags |= 0x21;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void AddPowerupFloat(NAS *nas) {
    EXTmotion *mot;
    EXTengine *eng;
    EXTengine *src;

    mot = AddMotion(nas);
    if (mot != NULL) {
        mot->Ymax = 0xA0;
        mot->Ydec = 0xE;
        mot->Yacc = 0;
        mot->Ydir = 0xFF;
        mot->Yvel = 0;
    }
    eng = AddExtEngine(nas, 1);
    if (eng != NULL) {
        eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        eng->sub->entity = 0x2001;
        src = nas->Engine;
        eng->unk10 = src->unk10;
        eng->flags |= 0x8041;
        eng->sub->unk76 = 0x800;
        eng->sub->flags |= 0x40000000;
    }
}

s32 HeroStubFalse(void) {
    return 0;
}

s32 HeroIsActive(s32 arg0) {
    return (Heroes[arg0].hero_status & 0xFA00) == 0;
}

void HurtHeroesInRadius(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    EXTengine *eng;
    HeroInfo *hero;
    NAS *nas;
    FuxFlags *fux;
    EXTengine *e;
    s32 x, y, z;
    s32 dx, dz;
    s32 dist;
    s32 i;
    s32 ok;
    s32 ang;
    s32 vc;

    if (GameExitFlags & 0x801) {
        return;
    }
    eng = ((NAS *)arg0)->Engine;
    i = 0;
    if (eng == NULL) {
        return;
    }
    hero = Heroes;
    x = eng->unk10.x;
    y = eng->unk10.y;
    z = eng->unk10.z;
    do {
        if (arg2 != i && !(hero[i].hero_status & 0xFA00)) {
            if (arg4 != 0) {
                if (hero[i].hero_mode == 4 ? hero[i].hero_eng->unk10.y < y - arg4 : arg1 < y) {
                    continue;
                }
            }
            e = hero[i].hero_eng;
            dx = e->unk10.x - x;
            dz = e->unk10.z - z;
            dist = csqrt(dx * dx + dz * dz) >> 6;
            if (dist < arg1) {
                if ((GameCheats & 0x80000001) != 0x80000001) {
                    ok = 1;
                    if (hero[i].hero_hit != 0) {
                        ok = hero[i].hero_immunity_level < 0x200;
                    }
                    if (hero[i].hero_immunity != 0) {
                        ok = 0;
                    }
                    if (ok) {
                        if (dist < 0x80) {
                            ang = hero[i].hero_move->facing_angle;
                        } else {
                            ang = get_angle(dx, dz);
                        }
                        dist = (arg1 - dist) >> 3;
                        vc = hero[i].hero_move->velocity_calc;
                        if (vc != 1) if (vc != 0) if (vc < 4) {
                            hero[i].hero_move->x_vel = (dist * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                            hero[i].hero_move->z_vel = (dist * rcossin_tbl[ang & 0xFFF].unk2) >> 12;
                            hero[i].hero_move->x_max = 0;
                            hero[i].hero_move->z_max = 0;
                        }
                        hero[i].hero_move->reactive_angle = ang ^ 0x800;
                        hero[i].hero_move->reactive_speed = dist;
                        hero[i].hero_frozen = 0x1E;
                        hero[i].hero_move->forward_angle = hero[i].hero_move->reactive_angle + 0x800;
                    }
                }
                ((void (*)(s32, NAS *, s32, s32, s32))HeroTakeHit)(i, NULL, -1, arg3, 0);
            }
        }
    } while (++i < 4);
    if (HitHooks.unkC != NULL) {
        HitHooks.unkC(arg0, arg1, arg2, arg3);
    }
    if (abs(eng->unk10.y - World_floor_level) < 0x80) {
        arg1 = arg1 * arg1;
        for (nas = HeadOfObjects; nas != NULL; nas = nas->Object.Child) {
            fux = (FuxFlags *)nas->FUX;
            if (fux != NULL && (fux->unk1C & 0x100)) {
                e = nas->Engine;
                if (e != NULL) {
                    dx = e->unk10.x - x;
                    dz = e->unk10.z - z;
                    if (dx * dx + dz * dz < arg1) {
                        fux->unk16 = -1;
                        fux->unk1E |= 0x100;
                    }
                }
            }
        }
    }
}

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */
extern u8 ArkHeroDead[];
extern u8 ArkHeroDie[];
extern u8 ArkHeroGrab[];
extern u8 ArkHeroIdle[];
extern u8 ArkHeroKick[];
extern u8 ArkHeroLose[];
extern u8 ArkHeroMove[];
extern u8 ArkHeroRedKick[];
extern u8 ArkHeroTaunt[];
extern u8 ArkHeroWinner[];
extern u8 BRO_BA_seq_die[];
extern u8 BRO_BA_seq_hold[];
extern u8 BRO_BA_seq_hold_slow[];
extern u8 BRO_BA_seq_hold_throw[];
extern u8 BRO_BA_seq_pickup[];
extern u8 BRO_BA_seq_slide[];
extern u8 BRO_PA_seq_die[];
extern u8 BRO_PA_seq_hold[];
extern u8 BRO_PA_seq_hold_slow[];
extern u8 BRO_PA_seq_hold_throw[];
extern u8 BRO_PA_seq_pickup[];
extern u8 BRO_SP_seq_die[];
extern u8 BRO_seq_arkbreathe[];
extern u8 BRO_seq_arkidle[];
extern u8 BRO_seq_arkkick[];
extern u8 BRO_seq_arklose[];
extern u8 BRO_seq_arkmove[];
extern u8 BRO_seq_arktaunt[];
extern u8 BRO_seq_arkvictory[];
extern u8 BRO_seq_arkwin[];
extern u8 BRO_seq_bonk_backswing[];
extern u8 BRO_seq_bonk_extended_backswing[];
extern u8 BRO_seq_bonk_hit[];
extern u8 BRO_seq_bonk_idle[];
extern u8 BRO_seq_bonk_run[];
extern u8 BRO_seq_bonk_swing[];
extern u8 BRO_seq_bonk_winner[];
extern u8 BRO_seq_breathe_ft[];
extern u8 BRO_seq_dinodazed[];
extern u8 BRO_seq_dinoidle[];
extern u8 BRO_seq_dinorun[];
extern u8 BRO_seq_dinoshoot[];
extern u8 BRO_seq_dinoslide[];
extern u8 BRO_seq_dinowin[];
extern u8 BRO_seq_flip_jump[];
extern u8 BRO_seq_kegbreathe[];
extern u8 BRO_seq_kegfall[];
extern u8 BRO_seq_keghit[];
extern u8 BRO_seq_kegidle_a[];
extern u8 BRO_seq_kegidle_b[];
extern u8 BRO_seq_kegjump[];
extern u8 BRO_seq_kegland[];
extern u8 BRO_seq_kegmedium[];
extern u8 BRO_seq_kegnone[];
extern u8 BRO_seq_kegvictory[];
extern u8 BRO_seq_kegwin[];
extern u8 BRO_seq_lightbear[];
extern u8 BRO_seq_long_hit[];
extern u8 BRO_seq_n2obreathe[];
extern u8 BRO_seq_n2ofly[];
extern u8 BRO_seq_n2ohit[];
extern u8 BRO_seq_n2oidle[];
extern u8 BRO_seq_n2okick[];
extern u8 BRO_seq_n2olose[];
extern u8 BRO_seq_n2omove[];
extern u8 BRO_seq_n2otaunt[];
extern u8 BRO_seq_n2ovictory[];
extern u8 BRO_seq_n2owin[];
extern u8 BRO_seq_pogo_dazed[];
extern u8 BRO_seq_pogo_idle[];
extern u8 BRO_seq_pogo_quick_idle[];
extern u8 BRO_seq_pogo_tazing[];
extern u8 BRO_seq_pogo_winner[];
extern u8 BRO_seq_polarattack[];
extern u8 BRO_seq_polardazed[];
extern u8 BRO_seq_polardead[];
extern u8 BRO_seq_polarfall[];
extern u8 BRO_seq_polarhit[];
extern u8 BRO_seq_polaridle[];
extern u8 BRO_seq_polarrun[];
extern u8 BRO_seq_polartaunt[];
extern u8 BRO_seq_polarwin[];
extern u8 BRO_seq_racefall[];
extern u8 BRO_seq_racehit[];
extern u8 BRO_seq_raceidle[];
extern u8 BRO_seq_racerun[];
extern u8 BRO_seq_raceswimfall[];
extern u8 BRO_seq_raceswimidle[];
extern u8 BRO_seq_raceswimrun[];
extern u8 BRO_seq_raceswimwin[];
extern u8 BRO_seq_racewin[];
extern u8 BRO_seq_skate_attack[];
extern u8 BRO_seq_skate_ft[];
extern u8 BRO_seq_tankdie[];
extern u8 BRO_seq_tankfire1[];
extern u8 BRO_seq_tankfire2[];
extern u8 BRO_seq_tankfire3[];
extern u8 BRO_seq_tankidle[];
extern u8 BRO_seq_tankidle1[];
extern u8 BRO_seq_tankminelay[];
extern u8 BRO_seq_tankrecoil[];
extern u8 BRO_seq_tankvictory[];
extern u8 BRO_seq_win_ft[];
extern u8 BashHeroBigWeight[];
extern u8 BashHeroCarry[];
extern u8 BashHeroDead[];
extern u8 BashHeroHit[];
extern u8 BashHeroHold[];
extern u8 BashHeroLoser[];
extern u8 BashHeroPickup[];
extern u8 BashHeroThrow[];
extern u8 BashHeroWinner[];
extern u8 BonkHeroBackswing[];
extern u8 BonkHeroExtBackswing[];
extern u8 BonkHeroHit[];
extern u8 BonkHeroIdle[];
extern u8 BonkHeroLoser[];
extern u8 BonkHeroRun[];
extern u8 BonkHeroSwing[];
extern u8 BonkHeroSwingRecoil[];
extern u8 BonkHeroWinner[];
extern u8 CHASE_seq_kegdead[];
extern u8 COC_BA_seq_die[];
extern u8 COC_BA_seq_hold[];
extern u8 COC_BA_seq_hold_slow[];
extern u8 COC_BA_seq_hold_throw[];
extern u8 COC_BA_seq_pickup[];
extern u8 COC_BA_seq_slide[];
extern u8 COC_PA_seq_die[];
extern u8 COC_PA_seq_hold[];
extern u8 COC_PA_seq_hold_slow[];
extern u8 COC_PA_seq_hold_throw[];
extern u8 COC_PA_seq_pickup[];
extern u8 COC_SP_seq_die[];
extern u8 COC_seq_arkbreathe[];
extern u8 COC_seq_arkidle[];
extern u8 COC_seq_arkkick[];
extern u8 COC_seq_arklose[];
extern u8 COC_seq_arkmove[];
extern u8 COC_seq_arktaunt[];
extern u8 COC_seq_arkvictory[];
extern u8 COC_seq_arkwin[];
extern u8 COC_seq_bonk_backswing[];
extern u8 COC_seq_bonk_extended_backswing[];
extern u8 COC_seq_bonk_hit[];
extern u8 COC_seq_bonk_idle[];
extern u8 COC_seq_bonk_run[];
extern u8 COC_seq_bonk_swing[];
extern u8 COC_seq_bonk_winner[];
extern u8 COC_seq_breathe_ft[];
extern u8 COC_seq_dinodazed[];
extern u8 COC_seq_dinoidle[];
extern u8 COC_seq_dinorun[];
extern u8 COC_seq_dinoshoot[];
extern u8 COC_seq_dinoslide[];
extern u8 COC_seq_dinowin[];
extern u8 COC_seq_flip_jump[];
extern u8 COC_seq_kegbreathe[];
extern u8 COC_seq_kegfall[];
extern u8 COC_seq_keghit[];
extern u8 COC_seq_kegidle_a[];
extern u8 COC_seq_kegidle_b[];
extern u8 COC_seq_kegjump[];
extern u8 COC_seq_kegland[];
extern u8 COC_seq_kegmedium[];
extern u8 COC_seq_kegnone[];
extern u8 COC_seq_kegvictory[];
extern u8 COC_seq_kegwin[];
extern u8 COC_seq_lightbear[];
extern u8 COC_seq_long_hit[];
extern u8 COC_seq_n2obreathe[];
extern u8 COC_seq_n2ofly[];
extern u8 COC_seq_n2ohit[];
extern u8 COC_seq_n2oidle[];
extern u8 COC_seq_n2okick[];
extern u8 COC_seq_n2olose[];
extern u8 COC_seq_n2omove[];
extern u8 COC_seq_n2otaunt[];
extern u8 COC_seq_n2ovictory[];
extern u8 COC_seq_n2owin[];
extern u8 COC_seq_pogo_dazed[];
extern u8 COC_seq_pogo_idle[];
extern u8 COC_seq_pogo_quick_idle[];
extern u8 COC_seq_pogo_tazing[];
extern u8 COC_seq_pogo_winner[];
extern u8 COC_seq_polarattack[];
extern u8 COC_seq_polardazed[];
extern u8 COC_seq_polardead[];
extern u8 COC_seq_polarfall[];
extern u8 COC_seq_polarhit[];
extern u8 COC_seq_polaridle[];
extern u8 COC_seq_polarrun[];
extern u8 COC_seq_polartaunt[];
extern u8 COC_seq_polarwin[];
extern u8 COC_seq_racefall[];
extern u8 COC_seq_racehit[];
extern u8 COC_seq_raceidle[];
extern u8 COC_seq_racerun[];
extern u8 COC_seq_raceswimfall[];
extern u8 COC_seq_raceswimidle[];
extern u8 COC_seq_raceswimrun[];
extern u8 COC_seq_raceswimwin[];
extern u8 COC_seq_racewin[];
extern u8 COC_seq_skate_attack[];
extern u8 COC_seq_skate_ft[];
extern u8 COC_seq_tankdie[];
extern u8 COC_seq_tankfire1[];
extern u8 COC_seq_tankfire2[];
extern u8 COC_seq_tankfire3[];
extern u8 COC_seq_tankidle[];
extern u8 COC_seq_tankidle1[];
extern u8 COC_seq_tankminelay[];
extern u8 COC_seq_tankrecoil[];
extern u8 COC_seq_tankvictory[];
extern u8 COC_seq_win_ft[];
extern u8 CRA_BA_seq_die[];
extern u8 CRA_BA_seq_hold[];
extern u8 CRA_BA_seq_hold_slow[];
extern u8 CRA_BA_seq_hold_throw[];
extern u8 CRA_BA_seq_pickup[];
extern u8 CRA_BA_seq_slide[];
extern u8 CRA_PA_seq_die[];
extern u8 CRA_PA_seq_hold[];
extern u8 CRA_PA_seq_hold_slow[];
extern u8 CRA_PA_seq_hold_throw[];
extern u8 CRA_PA_seq_pickup[];
extern u8 CRA_SP_seq_die[];
extern u8 CRA_seq_arkbreathe[];
extern u8 CRA_seq_arkidle[];
extern u8 CRA_seq_arkkick[];
extern u8 CRA_seq_arklose[];
extern u8 CRA_seq_arkmove[];
extern u8 CRA_seq_arktaunt[];
extern u8 CRA_seq_arkvictory[];
extern u8 CRA_seq_arkwin[];
extern u8 CRA_seq_bonk_backswing[];
extern u8 CRA_seq_bonk_extended_backswing[];
extern u8 CRA_seq_bonk_hit[];
extern u8 CRA_seq_bonk_idle[];
extern u8 CRA_seq_bonk_run[];
extern u8 CRA_seq_bonk_swing[];
extern u8 CRA_seq_bonk_winner[];
extern u8 CRA_seq_breathe_ft[];
extern u8 CRA_seq_dinodazed[];
extern u8 CRA_seq_dinoidle[];
extern u8 CRA_seq_dinorun[];
extern u8 CRA_seq_dinoshoot[];
extern u8 CRA_seq_dinoslide[];
extern u8 CRA_seq_dinowin[];
extern u8 CRA_seq_flip_jump[];
extern u8 CRA_seq_kegbreathe[];
extern u8 CRA_seq_kegfall[];
extern u8 CRA_seq_keghit[];
extern u8 CRA_seq_kegidle_a[];
extern u8 CRA_seq_kegidle_b[];
extern u8 CRA_seq_kegjump[];
extern u8 CRA_seq_kegland[];
extern u8 CRA_seq_kegmedium[];
extern u8 CRA_seq_kegnone[];
extern u8 CRA_seq_kegvictory[];
extern u8 CRA_seq_kegwin[];
extern u8 CRA_seq_lightbear[];
extern u8 CRA_seq_long_hit[];
extern u8 CRA_seq_n2obreathe[];
extern u8 CRA_seq_n2ofly[];
extern u8 CRA_seq_n2ohit[];
extern u8 CRA_seq_n2oidle[];
extern u8 CRA_seq_n2okick[];
extern u8 CRA_seq_n2olose[];
extern u8 CRA_seq_n2omove[];
extern u8 CRA_seq_n2otaunt[];
extern u8 CRA_seq_n2ovictory[];
extern u8 CRA_seq_n2owin[];
extern u8 CRA_seq_pogo_dazed[];
extern u8 CRA_seq_pogo_idle[];
extern u8 CRA_seq_pogo_quick_idle[];
extern u8 CRA_seq_pogo_tazing[];
extern u8 CRA_seq_pogo_winner[];
extern u8 CRA_seq_polarattack[];
extern u8 CRA_seq_polardazed[];
extern u8 CRA_seq_polardead[];
extern u8 CRA_seq_polarfall[];
extern u8 CRA_seq_polarhit[];
extern u8 CRA_seq_polaridle[];
extern u8 CRA_seq_polarrun[];
extern u8 CRA_seq_polartaunt[];
extern u8 CRA_seq_polarwin[];
extern u8 CRA_seq_racefall[];
extern u8 CRA_seq_racehit[];
extern u8 CRA_seq_raceidle[];
extern u8 CRA_seq_racerun[];
extern u8 CRA_seq_raceswimfall[];
extern u8 CRA_seq_raceswimidle[];
extern u8 CRA_seq_raceswimrun[];
extern u8 CRA_seq_raceswimwin[];
extern u8 CRA_seq_racewin[];
extern u8 CRA_seq_skate_attack[];
extern u8 CRA_seq_skate_ft[];
extern u8 CRA_seq_tankdie[];
extern u8 CRA_seq_tankfire1[];
extern u8 CRA_seq_tankfire2[];
extern u8 CRA_seq_tankfire3[];
extern u8 CRA_seq_tankidle[];
extern u8 CRA_seq_tankidle1[];
extern u8 CRA_seq_tankminelay[];
extern u8 CRA_seq_tankrecoil[];
extern u8 CRA_seq_tankvictory[];
extern u8 CRA_seq_win_ft[];
extern u8 CTX_BA_seq_die[];
extern u8 CTX_BA_seq_hold[];
extern u8 CTX_BA_seq_hold_slow[];
extern u8 CTX_BA_seq_hold_throw[];
extern u8 CTX_BA_seq_pickup[];
extern u8 CTX_BA_seq_slide[];
extern u8 CTX_PA_seq_die[];
extern u8 CTX_PA_seq_hold[];
extern u8 CTX_PA_seq_hold_slow[];
extern u8 CTX_PA_seq_hold_throw[];
extern u8 CTX_PA_seq_pickup[];
extern u8 CTX_SP_seq_die[];
extern u8 CTX_seq_arkbreathe[];
extern u8 CTX_seq_arkidle[];
extern u8 CTX_seq_arkkick[];
extern u8 CTX_seq_arklose[];
extern u8 CTX_seq_arkmove[];
extern u8 CTX_seq_arktaunt[];
extern u8 CTX_seq_arkvictory[];
extern u8 CTX_seq_arkwin[];
extern u8 CTX_seq_bonk_backswing[];
extern u8 CTX_seq_bonk_extended_backswing[];
extern u8 CTX_seq_bonk_hit[];
extern u8 CTX_seq_bonk_idle[];
extern u8 CTX_seq_bonk_run[];
extern u8 CTX_seq_bonk_swing[];
extern u8 CTX_seq_bonk_winner[];
extern u8 CTX_seq_breathe_ft[];
extern u8 CTX_seq_dinodazed[];
extern u8 CTX_seq_dinoidle[];
extern u8 CTX_seq_dinorun[];
extern u8 CTX_seq_dinoshoot[];
extern u8 CTX_seq_dinoslide[];
extern u8 CTX_seq_dinowin[];
extern u8 CTX_seq_flip_jump[];
extern u8 CTX_seq_kegbreathe[];
extern u8 CTX_seq_kegfall[];
extern u8 CTX_seq_keghit[];
extern u8 CTX_seq_kegidle_a[];
extern u8 CTX_seq_kegidle_b[];
extern u8 CTX_seq_kegjump[];
extern u8 CTX_seq_kegland[];
extern u8 CTX_seq_kegmedium[];
extern u8 CTX_seq_kegnone[];
extern u8 CTX_seq_kegvictory[];
extern u8 CTX_seq_kegwin[];
extern u8 CTX_seq_lightbear[];
extern u8 CTX_seq_long_hit[];
extern u8 CTX_seq_n2obreathe[];
extern u8 CTX_seq_n2ofly[];
extern u8 CTX_seq_n2ohit[];
extern u8 CTX_seq_n2oidle[];
extern u8 CTX_seq_n2okick[];
extern u8 CTX_seq_n2olose[];
extern u8 CTX_seq_n2omove[];
extern u8 CTX_seq_n2otaunt[];
extern u8 CTX_seq_n2ovictory[];
extern u8 CTX_seq_n2owin[];
extern u8 CTX_seq_pogo_dazed[];
extern u8 CTX_seq_pogo_idle[];
extern u8 CTX_seq_pogo_quick_idle[];
extern u8 CTX_seq_pogo_tazing[];
extern u8 CTX_seq_pogo_winner[];
extern u8 CTX_seq_polarattack[];
extern u8 CTX_seq_polardazed[];
extern u8 CTX_seq_polardead[];
extern u8 CTX_seq_polarfall[];
extern u8 CTX_seq_polarhit[];
extern u8 CTX_seq_polaridle[];
extern u8 CTX_seq_polarrun[];
extern u8 CTX_seq_polartaunt[];
extern u8 CTX_seq_polarwin[];
extern u8 CTX_seq_racefall[];
extern u8 CTX_seq_racehit[];
extern u8 CTX_seq_raceidle[];
extern u8 CTX_seq_racerun[];
extern u8 CTX_seq_raceswimfall[];
extern u8 CTX_seq_raceswimidle[];
extern u8 CTX_seq_raceswimrun[];
extern u8 CTX_seq_raceswimwin[];
extern u8 CTX_seq_racewin[];
extern u8 CTX_seq_skate_attack[];
extern u8 CTX_seq_skate_ft[];
extern u8 CTX_seq_tankdie[];
extern u8 CTX_seq_tankfire1[];
extern u8 CTX_seq_tankfire2[];
extern u8 CTX_seq_tankfire3[];
extern u8 CTX_seq_tankidle[];
extern u8 CTX_seq_tankidle1[];
extern u8 CTX_seq_tankminelay[];
extern u8 CTX_seq_tankrecoil[];
extern u8 CTX_seq_tankvictory[];
extern u8 CTX_seq_win_ft[];
extern u8 ChaseHeroDead[];
extern u8 ChaseHeroDie[];
extern u8 ChaseHeroDrop[];
extern u8 ChaseHeroFall[];
extern u8 ChaseHeroHit[];
extern u8 ChaseHeroIdle[];
extern u8 ChaseHeroJump[];
extern u8 ChaseHeroLand[];
extern u8 ChaseHeroLose[];
extern u8 ChaseHeroRun[];
extern u8 ChaseHeroWinner[];
extern u8 DNG_BA_seq_die[];
extern u8 DNG_BA_seq_hold[];
extern u8 DNG_BA_seq_hold_slow[];
extern u8 DNG_BA_seq_hold_throw[];
extern u8 DNG_BA_seq_pickup[];
extern u8 DNG_BA_seq_slide[];
extern u8 DNG_PA_seq_die[];
extern u8 DNG_PA_seq_hold[];
extern u8 DNG_PA_seq_hold_slow[];
extern u8 DNG_PA_seq_hold_throw[];
extern u8 DNG_PA_seq_pickup[];
extern u8 DNG_SP_seq_die[];
extern u8 DNG_seq_arkbreathe[];
extern u8 DNG_seq_arkidle[];
extern u8 DNG_seq_arkkick[];
extern u8 DNG_seq_arklose[];
extern u8 DNG_seq_arkmove[];
extern u8 DNG_seq_arktaunt[];
extern u8 DNG_seq_arkvictory[];
extern u8 DNG_seq_arkwin[];
extern u8 DNG_seq_bonk_backswing[];
extern u8 DNG_seq_bonk_extended_backswing[];
extern u8 DNG_seq_bonk_hit[];
extern u8 DNG_seq_bonk_idle[];
extern u8 DNG_seq_bonk_run[];
extern u8 DNG_seq_bonk_swing[];
extern u8 DNG_seq_bonk_winner[];
extern u8 DNG_seq_breathe_ft[];
extern u8 DNG_seq_dinodazed[];
extern u8 DNG_seq_dinoidle[];
extern u8 DNG_seq_dinorun[];
extern u8 DNG_seq_dinoshoot[];
extern u8 DNG_seq_dinoslide[];
extern u8 DNG_seq_dinowin[];
extern u8 DNG_seq_flip_jump[];
extern u8 DNG_seq_kegbreathe[];
extern u8 DNG_seq_kegfall[];
extern u8 DNG_seq_keghit[];
extern u8 DNG_seq_kegidle_a[];
extern u8 DNG_seq_kegidle_b[];
extern u8 DNG_seq_kegjump[];
extern u8 DNG_seq_kegland[];
extern u8 DNG_seq_kegmedium[];
extern u8 DNG_seq_kegnone[];
extern u8 DNG_seq_kegvictory[];
extern u8 DNG_seq_kegwin[];
extern u8 DNG_seq_lightbear[];
extern u8 DNG_seq_long_hit[];
extern u8 DNG_seq_n2obreathe[];
extern u8 DNG_seq_n2ofly[];
extern u8 DNG_seq_n2ohit[];
extern u8 DNG_seq_n2oidle[];
extern u8 DNG_seq_n2okick[];
extern u8 DNG_seq_n2olose[];
extern u8 DNG_seq_n2omove[];
extern u8 DNG_seq_n2otaunt[];
extern u8 DNG_seq_n2ovictory[];
extern u8 DNG_seq_n2owin[];
extern u8 DNG_seq_pogo_dazed[];
extern u8 DNG_seq_pogo_idle[];
extern u8 DNG_seq_pogo_quick_idle[];
extern u8 DNG_seq_pogo_tazing[];
extern u8 DNG_seq_pogo_winner[];
extern u8 DNG_seq_polarattack[];
extern u8 DNG_seq_polardazed[];
extern u8 DNG_seq_polardead[];
extern u8 DNG_seq_polarfall[];
extern u8 DNG_seq_polarhit[];
extern u8 DNG_seq_polaridle[];
extern u8 DNG_seq_polarrun[];
extern u8 DNG_seq_polartaunt[];
extern u8 DNG_seq_polarwin[];
extern u8 DNG_seq_racefall[];
extern u8 DNG_seq_racehit[];
extern u8 DNG_seq_raceidle[];
extern u8 DNG_seq_racerun[];
extern u8 DNG_seq_raceswimfall[];
extern u8 DNG_seq_raceswimidle[];
extern u8 DNG_seq_raceswimrun[];
extern u8 DNG_seq_raceswimwin[];
extern u8 DNG_seq_racewin[];
extern u8 DNG_seq_skate_attack[];
extern u8 DNG_seq_skate_ft[];
extern u8 DNG_seq_tankdie[];
extern u8 DNG_seq_tankfire1[];
extern u8 DNG_seq_tankfire2[];
extern u8 DNG_seq_tankfire3[];
extern u8 DNG_seq_tankidle[];
extern u8 DNG_seq_tankidle1[];
extern u8 DNG_seq_tankminelay[];
extern u8 DNG_seq_tankrecoil[];
extern u8 DNG_seq_tankvictory[];
extern u8 DNG_seq_win_ft[];
extern u8 DinoHeroDazed[];
extern u8 DinoHeroIdle[];
extern u8 DinoHeroRun[];
extern u8 DinoHeroShoot[];
extern u8 DinoHeroSlide[];
extern u8 DinoHeroWinner[];
extern u8 HeroPogo[];
extern u8 HeroPogoDazed[];
extern u8 HeroPogoIdle[];
extern u8 HeroPogoLoser[];
extern u8 HeroPogoTazing[];
extern u8 HeroPogoTied[];
extern u8 HeroPogoWinner[];
extern u8 KAN_BA_seq_die[];
extern u8 KAN_BA_seq_hold[];
extern u8 KAN_BA_seq_hold_slow[];
extern u8 KAN_BA_seq_hold_throw[];
extern u8 KAN_BA_seq_pickup[];
extern u8 KAN_BA_seq_slide[];
extern u8 KAN_PA_seq_die[];
extern u8 KAN_PA_seq_hold[];
extern u8 KAN_PA_seq_hold_slow[];
extern u8 KAN_PA_seq_hold_throw[];
extern u8 KAN_PA_seq_pickup[];
extern u8 KAN_SP_seq_die[];
extern u8 KAN_seq_arkbreathe[];
extern u8 KAN_seq_arkidle[];
extern u8 KAN_seq_arkkick[];
extern u8 KAN_seq_arklose[];
extern u8 KAN_seq_arkmove[];
extern u8 KAN_seq_arktaunt[];
extern u8 KAN_seq_arkvictory[];
extern u8 KAN_seq_arkwin[];
extern u8 KAN_seq_bonk_backswing[];
extern u8 KAN_seq_bonk_extended_backswing[];
extern u8 KAN_seq_bonk_hit[];
extern u8 KAN_seq_bonk_idle[];
extern u8 KAN_seq_bonk_run[];
extern u8 KAN_seq_bonk_swing[];
extern u8 KAN_seq_bonk_winner[];
extern u8 KAN_seq_breathe_ft[];
extern u8 KAN_seq_dinodazed[];
extern u8 KAN_seq_dinoidle[];
extern u8 KAN_seq_dinorun[];
extern u8 KAN_seq_dinoshoot[];
extern u8 KAN_seq_dinoslide[];
extern u8 KAN_seq_dinowin[];
extern u8 KAN_seq_flip_jump[];
extern u8 KAN_seq_kegbreathe[];
extern u8 KAN_seq_kegfall[];
extern u8 KAN_seq_keghit[];
extern u8 KAN_seq_kegidle_a[];
extern u8 KAN_seq_kegidle_b[];
extern u8 KAN_seq_kegjump[];
extern u8 KAN_seq_kegland[];
extern u8 KAN_seq_kegmedium[];
extern u8 KAN_seq_kegnone[];
extern u8 KAN_seq_kegvictory[];
extern u8 KAN_seq_kegwin[];
extern u8 KAN_seq_lightbear[];
extern u8 KAN_seq_long_hit[];
extern u8 KAN_seq_n2obreathe[];
extern u8 KAN_seq_n2ofly[];
extern u8 KAN_seq_n2ohit[];
extern u8 KAN_seq_n2oidle[];
extern u8 KAN_seq_n2okick[];
extern u8 KAN_seq_n2olose[];
extern u8 KAN_seq_n2omove[];
extern u8 KAN_seq_n2otaunt[];
extern u8 KAN_seq_n2ovictory[];
extern u8 KAN_seq_n2owin[];
extern u8 KAN_seq_pogo_dazed[];
extern u8 KAN_seq_pogo_idle[];
extern u8 KAN_seq_pogo_quick_idle[];
extern u8 KAN_seq_pogo_tazing[];
extern u8 KAN_seq_pogo_winner[];
extern u8 KAN_seq_polarattack[];
extern u8 KAN_seq_polardazed[];
extern u8 KAN_seq_polardead[];
extern u8 KAN_seq_polarfall[];
extern u8 KAN_seq_polarhit[];
extern u8 KAN_seq_polaridle[];
extern u8 KAN_seq_polarrun[];
extern u8 KAN_seq_polartaunt[];
extern u8 KAN_seq_polarwin[];
extern u8 KAN_seq_racefall[];
extern u8 KAN_seq_racehit[];
extern u8 KAN_seq_raceidle[];
extern u8 KAN_seq_racerun[];
extern u8 KAN_seq_raceswimfall[];
extern u8 KAN_seq_raceswimidle[];
extern u8 KAN_seq_raceswimrun[];
extern u8 KAN_seq_raceswimwin[];
extern u8 KAN_seq_racewin[];
extern u8 KAN_seq_skate_attack[];
extern u8 KAN_seq_skate_ft[];
extern u8 KAN_seq_tankdie[];
extern u8 KAN_seq_tankfire1[];
extern u8 KAN_seq_tankfire2[];
extern u8 KAN_seq_tankfire3[];
extern u8 KAN_seq_tankidle[];
extern u8 KAN_seq_tankidle1[];
extern u8 KAN_seq_tankminelay[];
extern u8 KAN_seq_tankrecoil[];
extern u8 KAN_seq_tankvictory[];
extern u8 KAN_seq_win_ft[];
extern u8 KON_BA_seq_die[];
extern u8 KON_BA_seq_hold[];
extern u8 KON_BA_seq_hold_slow[];
extern u8 KON_BA_seq_hold_throw[];
extern u8 KON_BA_seq_pickup[];
extern u8 KON_BA_seq_slide[];
extern u8 KON_PA_seq_die[];
extern u8 KON_PA_seq_hold[];
extern u8 KON_PA_seq_hold_slow[];
extern u8 KON_PA_seq_hold_throw[];
extern u8 KON_PA_seq_pickup[];
extern u8 KON_SP_seq_die[];
extern u8 KON_seq_arkbreathe[];
extern u8 KON_seq_arkidle[];
extern u8 KON_seq_arkkick[];
extern u8 KON_seq_arklose[];
extern u8 KON_seq_arkmove[];
extern u8 KON_seq_arktaunt[];
extern u8 KON_seq_arkvictory[];
extern u8 KON_seq_arkwin[];
extern u8 KON_seq_bonk_backswing[];
extern u8 KON_seq_bonk_extended_backswing[];
extern u8 KON_seq_bonk_hit[];
extern u8 KON_seq_bonk_idle[];
extern u8 KON_seq_bonk_run[];
extern u8 KON_seq_bonk_swing[];
extern u8 KON_seq_bonk_winner[];
extern u8 KON_seq_breathe_ft[];
extern u8 KON_seq_dinodazed[];
extern u8 KON_seq_dinoidle[];
extern u8 KON_seq_dinorun[];
extern u8 KON_seq_dinoshoot[];
extern u8 KON_seq_dinoslide[];
extern u8 KON_seq_dinowin[];
extern u8 KON_seq_flip_jump[];
extern u8 KON_seq_kegbreathe[];
extern u8 KON_seq_kegfall[];
extern u8 KON_seq_keghit[];
extern u8 KON_seq_kegidle_a[];
extern u8 KON_seq_kegidle_b[];
extern u8 KON_seq_kegjump[];
extern u8 KON_seq_kegland[];
extern u8 KON_seq_kegmedium[];
extern u8 KON_seq_kegnone[];
extern u8 KON_seq_kegvictory[];
extern u8 KON_seq_kegwin[];
extern u8 KON_seq_lightbear[];
extern u8 KON_seq_long_hit[];
extern u8 KON_seq_n2obreathe[];
extern u8 KON_seq_n2ofly[];
extern u8 KON_seq_n2ohit[];
extern u8 KON_seq_n2oidle[];
extern u8 KON_seq_n2okick[];
extern u8 KON_seq_n2olose[];
extern u8 KON_seq_n2omove[];
extern u8 KON_seq_n2otaunt[];
extern u8 KON_seq_n2ovictory[];
extern u8 KON_seq_n2owin[];
extern u8 KON_seq_pogo_dazed[];
extern u8 KON_seq_pogo_idle[];
extern u8 KON_seq_pogo_quick_idle[];
extern u8 KON_seq_pogo_tazing[];
extern u8 KON_seq_pogo_winner[];
extern u8 KON_seq_polarattack[];
extern u8 KON_seq_polardazed[];
extern u8 KON_seq_polardead[];
extern u8 KON_seq_polarfall[];
extern u8 KON_seq_polarhit[];
extern u8 KON_seq_polaridle[];
extern u8 KON_seq_polarrun[];
extern u8 KON_seq_polartaunt[];
extern u8 KON_seq_polarwin[];
extern u8 KON_seq_racefall[];
extern u8 KON_seq_racehit[];
extern u8 KON_seq_raceidle[];
extern u8 KON_seq_racerun[];
extern u8 KON_seq_raceswimfall[];
extern u8 KON_seq_raceswimidle[];
extern u8 KON_seq_raceswimrun[];
extern u8 KON_seq_raceswimwin[];
extern u8 KON_seq_racewin[];
extern u8 KON_seq_skate_attack[];
extern u8 KON_seq_skate_ft[];
extern u8 KON_seq_tankdie[];
extern u8 KON_seq_tankfire1[];
extern u8 KON_seq_tankfire2[];
extern u8 KON_seq_tankfire3[];
extern u8 KON_seq_tankidle[];
extern u8 KON_seq_tankidle1[];
extern u8 KON_seq_tankminelay[];
extern u8 KON_seq_tankrecoil[];
extern u8 KON_seq_tankvictory[];
extern u8 KON_seq_win_ft[];
extern u8 N2OHeroDead[];
extern u8 N2OHeroDie[];
extern u8 N2OHeroEnter[];
extern u8 N2OHeroHit[];
extern u8 N2OHeroIdle[];
extern u8 N2OHeroKick[];
extern u8 N2OHeroMove[];
extern u8 N2OHeroTaunt[];
extern u8 N2OHeroWinner[];
extern u8 NoxHeroDead[];
extern u8 NoxHeroFly[];
extern u8 NoxHeroHit[];
extern u8 NoxHeroWinner[];
extern u8 PAHeroCarry[];
extern u8 PAHeroDead[];
extern u8 PAHeroHit[];
extern u8 PAHeroHold[];
extern u8 PAHeroPickup[];
extern u8 PAHeroThrow[];
extern u8 PAHeroWinner[];
extern u8 PolarHeroAttack[];
extern u8 PolarHeroChallenge[];
extern u8 PolarHeroDazed[];
extern u8 PolarHeroDead[];
extern u8 PolarHeroFootIdle[];
extern u8 PolarHeroFootRun[];
extern u8 PolarHeroHit[];
extern u8 PolarHeroIdle[];
extern u8 PolarHeroInBeam[];
extern u8 PolarHeroOffice[];
extern u8 PolarHeroPoll[];
extern u8 PolarHeroRun[];
extern u8 PolarHeroSnowball[];
extern u8 PolarHeroSquashed[];
extern u8 PolarHeroThrowBomb[];
extern u8 PolarHeroTransmute[];
extern u8 PolarHeroWeight[];
extern u8 PolarHeroWinner[];
extern u8 RaceHeroDazed[];
extern u8 RaceHeroIdle[];
extern u8 RaceHeroJump[];
extern u8 RaceHeroKick[];
extern u8 RaceHeroOffTrack[];
extern u8 RaceHeroOiled[];
extern u8 RaceHeroRun[];
extern u8 RaceHeroWinner[];
extern u8 SetArkHeroDead[];
extern u8 SetArkHeroDie[];
extern u8 SetArkHeroGrab[];
extern u8 SetArkHeroIdle[];
extern u8 SetArkHeroKick[];
extern u8 SetArkHeroLose[];
extern u8 SetArkHeroMove[];
extern u8 SetArkHeroRedKick[];
extern u8 SetArkHeroTaunt[];
extern u8 SetArkHeroWinner[];
extern u8 SetBashHeroBigWeight[];
extern u8 SetBashHeroCarry[];
extern u8 SetBashHeroDead[];
extern u8 SetBashHeroHit[];
extern u8 SetBashHeroHold[];
extern u8 SetBashHeroLoser[];
extern u8 SetBashHeroPickup[];
extern u8 SetBashHeroThrow[];
extern u8 SetBashHeroWinner[];
extern u8 SetBonkHeroBackswing[];
extern u8 SetBonkHeroExtBackswing[];
extern u8 SetBonkHeroHit[];
extern u8 SetBonkHeroIdle[];
extern u8 SetBonkHeroLoser[];
extern u8 SetBonkHeroRun[];
extern u8 SetBonkHeroSwing[];
extern u8 SetBonkHeroSwingRecoil[];
extern u8 SetBonkHeroWinner[];
extern u8 SetChaseHeroDead[];
extern u8 SetChaseHeroDie[];
extern u8 SetChaseHeroDrop[];
extern u8 SetChaseHeroFall[];
extern u8 SetChaseHeroHit[];
extern u8 SetChaseHeroIdle[];
extern u8 SetChaseHeroJump[];
extern u8 SetChaseHeroLand[];
extern u8 SetChaseHeroLose[];
extern u8 SetChaseHeroRun[];
extern u8 SetChaseHeroWinner[];
extern u8 SetDinoHeroDazed[];
extern u8 SetDinoHeroIdle[];
extern u8 SetDinoHeroRun[];
extern u8 SetDinoHeroShoot[];
extern u8 SetDinoHeroSlide[];
extern u8 SetDinoHeroWinner[];
extern u8 SetHeroPogo[];
extern u8 SetHeroPogoDazed[];
extern u8 SetHeroPogoIdle[];
extern u8 SetHeroPogoLoser[];
extern u8 SetHeroPogoTazing[];
extern u8 SetHeroPogoTied[];
extern u8 SetHeroPogoWinner[];
extern u8 SetN2OHeroDead[];
extern u8 SetN2OHeroDie[];
extern u8 SetN2OHeroEnter[];
extern u8 SetN2OHeroHit[];
extern u8 SetN2OHeroIdle[];
extern u8 SetN2OHeroKick[];
extern u8 SetN2OHeroMove[];
extern u8 SetN2OHeroTaunt[];
extern u8 SetN2OHeroWinner[];
extern u8 SetNoxHeroDead[];
extern u8 SetNoxHeroFly[];
extern u8 SetNoxHeroHit[];
extern u8 SetNoxHeroWinner[];
extern u8 SetPAHeroCarry[];
extern u8 SetPAHeroDead[];
extern u8 SetPAHeroHit[];
extern u8 SetPAHeroHold[];
extern u8 SetPAHeroPickup[];
extern u8 SetPAHeroThrow[];
extern u8 SetPAHeroWinner[];
extern u8 SetPolarHeroAttack[];
extern u8 SetPolarHeroChallenge[];
extern u8 SetPolarHeroDazed[];
extern u8 SetPolarHeroDead[];
extern u8 SetPolarHeroFootIdle[];
extern u8 SetPolarHeroFootRun[];
extern u8 SetPolarHeroHit[];
extern u8 SetPolarHeroIdle[];
extern u8 SetPolarHeroInBeam[];
extern u8 SetPolarHeroOffice[];
extern u8 SetPolarHeroPoll[];
extern u8 SetPolarHeroRun[];
extern u8 SetPolarHeroSnowball[];
extern u8 SetPolarHeroSquashed[];
extern u8 SetPolarHeroThrowBomb[];
extern u8 SetPolarHeroTransmute[];
extern u8 SetPolarHeroWeight[];
extern u8 SetPolarHeroWinner[];
extern u8 SetRaceHeroDazed[];
extern u8 SetRaceHeroIdle[];
extern u8 SetRaceHeroJump[];
extern u8 SetRaceHeroKick[];
extern u8 SetRaceHeroOffTrack[];
extern u8 SetRaceHeroOiled[];
extern u8 SetRaceHeroRun[];
extern u8 SetRaceHeroWinner[];
extern u8 SetSpinHeroDead[];
extern u8 SetSpinHeroFall[];
extern u8 SetSpinHeroFlip[];
extern u8 SetSpinHeroHit[];
extern u8 SetSpinHeroLose[];
extern u8 SetSpinHeroWinner[];
extern u8 SetTankHeroDead[];
extern u8 SetTankHeroDie[];
extern u8 SetTankHeroIdle[];
extern u8 SetTankHeroMinelay[];
extern u8 SetTankHeroMove[];
extern u8 SetTankHeroRecoil[];
extern u8 SetTankHeroTurn[];
extern u8 SetTankHeroVictory[];
extern u8 SpinHeroDead[];
extern u8 SpinHeroFall[];
extern u8 SpinHeroFlip[];
extern u8 SpinHeroHit[];
extern u8 SpinHeroLose[];
extern u8 SpinHeroWinner[];
extern u8 TIN_BA_seq_die[];
extern u8 TIN_BA_seq_hold[];
extern u8 TIN_BA_seq_hold_slow[];
extern u8 TIN_BA_seq_hold_throw[];
extern u8 TIN_BA_seq_pickup[];
extern u8 TIN_BA_seq_slide[];
extern u8 TIN_PA_seq_die[];
extern u8 TIN_PA_seq_hold[];
extern u8 TIN_PA_seq_hold_slow[];
extern u8 TIN_PA_seq_hold_throw[];
extern u8 TIN_PA_seq_pickup[];
extern u8 TIN_SP_seq_die[];
extern u8 TIN_seq_arkbreathe[];
extern u8 TIN_seq_arkidle[];
extern u8 TIN_seq_arkkick[];
extern u8 TIN_seq_arklose[];
extern u8 TIN_seq_arkmove[];
extern u8 TIN_seq_arktaunt[];
extern u8 TIN_seq_arkvictory[];
extern u8 TIN_seq_arkwin[];
extern u8 TIN_seq_bonk_backswing[];
extern u8 TIN_seq_bonk_extended_backswing[];
extern u8 TIN_seq_bonk_hit[];
extern u8 TIN_seq_bonk_idle[];
extern u8 TIN_seq_bonk_run[];
extern u8 TIN_seq_bonk_swing[];
extern u8 TIN_seq_bonk_winner[];
extern u8 TIN_seq_breathe_ft[];
extern u8 TIN_seq_dinodazed[];
extern u8 TIN_seq_dinoidle[];
extern u8 TIN_seq_dinorun[];
extern u8 TIN_seq_dinoshoot[];
extern u8 TIN_seq_dinoslide[];
extern u8 TIN_seq_dinowin[];
extern u8 TIN_seq_flip_jump[];
extern u8 TIN_seq_kegbreathe[];
extern u8 TIN_seq_kegfall[];
extern u8 TIN_seq_keghit[];
extern u8 TIN_seq_kegidle_a[];
extern u8 TIN_seq_kegidle_b[];
extern u8 TIN_seq_kegjump[];
extern u8 TIN_seq_kegland[];
extern u8 TIN_seq_kegmedium[];
extern u8 TIN_seq_kegnone[];
extern u8 TIN_seq_kegvictory[];
extern u8 TIN_seq_kegwin[];
extern u8 TIN_seq_lightbear[];
extern u8 TIN_seq_long_hit[];
extern u8 TIN_seq_n2obreathe[];
extern u8 TIN_seq_n2ofly[];
extern u8 TIN_seq_n2ohit[];
extern u8 TIN_seq_n2oidle[];
extern u8 TIN_seq_n2okick[];
extern u8 TIN_seq_n2olose[];
extern u8 TIN_seq_n2omove[];
extern u8 TIN_seq_n2otaunt[];
extern u8 TIN_seq_n2ovictory[];
extern u8 TIN_seq_n2owin[];
extern u8 TIN_seq_pogo_dazed[];
extern u8 TIN_seq_pogo_idle[];
extern u8 TIN_seq_pogo_quick_idle[];
extern u8 TIN_seq_pogo_tazing[];
extern u8 TIN_seq_pogo_winner[];
extern u8 TIN_seq_polarattack[];
extern u8 TIN_seq_polardazed[];
extern u8 TIN_seq_polardead[];
extern u8 TIN_seq_polarfall[];
extern u8 TIN_seq_polarhit[];
extern u8 TIN_seq_polaridle[];
extern u8 TIN_seq_polarrun[];
extern u8 TIN_seq_polartaunt[];
extern u8 TIN_seq_polarwin[];
extern u8 TIN_seq_racefall[];
extern u8 TIN_seq_racehit[];
extern u8 TIN_seq_raceidle[];
extern u8 TIN_seq_racerun[];
extern u8 TIN_seq_raceswimfall[];
extern u8 TIN_seq_raceswimidle[];
extern u8 TIN_seq_raceswimrun[];
extern u8 TIN_seq_raceswimwin[];
extern u8 TIN_seq_racewin[];
extern u8 TIN_seq_skate_attack[];
extern u8 TIN_seq_skate_ft[];
extern u8 TIN_seq_tankdie[];
extern u8 TIN_seq_tankfire1[];
extern u8 TIN_seq_tankfire2[];
extern u8 TIN_seq_tankfire3[];
extern u8 TIN_seq_tankidle[];
extern u8 TIN_seq_tankidle1[];
extern u8 TIN_seq_tankminelay[];
extern u8 TIN_seq_tankrecoil[];
extern u8 TIN_seq_tankvictory[];
extern u8 TIN_seq_win_ft[];
extern u8 TankHeroDead[];
extern u8 TankHeroDie[];
extern u8 TankHeroIdle[];
extern u8 TankHeroMinelay[];
extern u8 TankHeroMove[];
extern u8 TankHeroRecoil[];
extern u8 TankHeroTurn[];
extern u8 TankHeroVictory[];

s32 CRA_seq_none[2] = { 0x14, 0x8000 };
s32 CRA_seq_breathe[3] = { (s32)0x80070000, 0x40000016, 0x8000 };
s32 CRA_seq_taunt_a[4] = { (s32)0x800A0000, (s32)0x80070002, 0x45800038, 0x8004 };
s32 CRA_seq_idle_a[2] = { (s32)0x80050000, 1 };
s32 CRA_seq_idle_b[2] = { (s32)0x80050000, 3 };
s32 CRA_seq_medium[8] = {
    (s32)0x80070000, 0x40800009, 0x88003, 0xB8007, (s32)0x80034089, 0x40940009,
    (s32)0x80004095, 0,
};
s32 CRA_seq_jump[6] = { (s32)0x800A0000, (s32)0x8007000C, 0x41000007, 0x288018, (s32)0x80044107, 5 };
s32 CRA_seq_fall[7] = { 0x41080002, 0x48018, 0x410A4109, 0x288018, 0x410B410B, 0x410B410B, 0x78004 };
s32 CRA_seq_land[4] = { (s32)0x80090000, (s32)0x8007000D, 0x410C001B, 0x8004 };
s32 CRA_seq_attack[3] = { (s32)0x800A000F, 0x2002000E, 0x8004 };
s32 CRA_seq_hit[9] = {
    (s32)0x80070000, 0x4180000B, 0x88003, 0x48007, (s32)0x8007418B, 0x418F0004, 0xA8018,
    (s32)0x80044192, 0,
};
s32 CRA_seq_win[3] = { (s32)0x80070000, 0x45800038, 0x8000 };
s32 CRA_seq_victory[3] = { (s32)0x80070000, 0x44800022, 0x8000 };
s32 HERO_seq_dead[3] = { (s32)0x80070000, 0x40000014, 0x8000 };
s32 CRA_seqs[108] = {
    (s32)&CRA_seq_none, (s32)&CRA_seq_breathe, (s32)&CRA_seq_taunt_a, (s32)&CRA_seq_idle_a, (s32)&CRA_seq_idle_b,
    (s32)&CRA_seq_medium, (s32)&CRA_seq_jump, (s32)&CRA_seq_fall, (s32)&CRA_seq_land, (s32)&CRA_seq_hit,
    (s32)&CRA_seq_attack, (s32)&HERO_seq_dead, (s32)&CRA_seq_win, (s32)&CRA_seq_victory,
    (s32)&CRA_BA_seq_slide, (s32)&CRA_BA_seq_pickup, (s32)&CRA_BA_seq_hold,
    (s32)&CRA_BA_seq_hold_slow, (s32)&CRA_BA_seq_hold_throw, (s32)&CRA_BA_seq_die,
    (s32)&CRA_PA_seq_pickup, (s32)&CRA_PA_seq_hold, (s32)&CRA_PA_seq_hold_slow,
    (s32)&CRA_PA_seq_hold_throw, (s32)&CRA_PA_seq_die, (s32)&CRA_seq_arkbreathe,
    (s32)&CRA_seq_arkmove, (s32)&CRA_seq_arkidle, (s32)&CRA_seq_arktaunt,
    (s32)&CRA_seq_arkkick, (s32)&CRA_seq_arkwin, (s32)&CRA_seq_arkvictory,
    (s32)&CRA_seq_arklose, (s32)&CRA_seq_n2obreathe, (s32)&CRA_seq_n2omove,
    (s32)&CRA_seq_n2oidle, (s32)&CRA_seq_n2otaunt, (s32)&CRA_seq_n2okick, (s32)&CRA_seq_n2ohit,
    (s32)&CRA_seq_n2owin, (s32)&CRA_seq_n2ovictory, (s32)&CRA_seq_n2olose,
    (s32)&CRA_seq_n2ofly, (s32)&CRA_seq_long_hit, (s32)&CRA_seq_flip_jump,
    (s32)&CRA_SP_seq_die, (s32)&CRA_seq_pogo_idle, (s32)&CRA_seq_pogo_quick_idle,
    (s32)&CRA_seq_pogo_tazing, (s32)&CRA_seq_pogo_dazed, (s32)&CRA_seq_pogo_winner,
    (s32)&CRA_seq_bonk_idle, (s32)&CRA_seq_bonk_run, (s32)&CRA_seq_bonk_backswing,
    (s32)&CRA_seq_bonk_extended_backswing, (s32)&CRA_seq_bonk_swing, (s32)&CRA_seq_bonk_hit,
    (s32)&CRA_seq_bonk_winner, (s32)&CRA_seq_polaridle, (s32)&CRA_seq_polarrun,
    (s32)&CRA_seq_polardazed, (s32)&CRA_seq_polarfall, (s32)&CRA_seq_polarattack,
    (s32)&CRA_seq_polarhit, (s32)&CRA_seq_polardead, (s32)&CRA_seq_polartaunt,
    (s32)&CRA_seq_breathe_ft, (s32)&CRA_seq_skate_ft, (s32)&CRA_seq_lightbear,
    (s32)&CRA_seq_skate_attack, (s32)&CRA_seq_polarwin, (s32)&CRA_seq_win_ft,
    (s32)&CRA_seq_raceidle, (s32)&CRA_seq_racerun, (s32)&CRA_seq_racehit,
    (s32)&CRA_seq_racefall, (s32)&CRA_seq_racewin, (s32)&CRA_seq_dinoidle,
    (s32)&CRA_seq_dinorun, (s32)&CRA_seq_dinoshoot, (s32)&CRA_seq_dinoslide,
    (s32)&CRA_seq_dinodazed, (s32)&CRA_seq_raceswimidle, (s32)&CRA_seq_raceswimrun,
    (s32)&CRA_seq_raceswimfall, (s32)&CRA_seq_raceswimwin, (s32)&CRA_seq_dinowin,
    (s32)&CRA_seq_tankrecoil, (s32)&CRA_seq_tankdie, (s32)&CRA_seq_tankidle,
    (s32)&CRA_seq_tankidle1, (s32)&CRA_seq_tankminelay, (s32)&CRA_seq_tankvictory,
    (s32)&CRA_seq_tankfire1, (s32)&CRA_seq_tankfire2, (s32)&CRA_seq_tankfire3,
    (s32)&CRA_seq_kegnone, (s32)&CRA_seq_kegbreathe, (s32)&CRA_seq_kegidle_a,
    (s32)&CRA_seq_kegidle_b, (s32)&CRA_seq_kegmedium, (s32)&CRA_seq_kegjump,
    (s32)&CRA_seq_kegfall, (s32)&CRA_seq_kegland, (s32)&CRA_seq_keghit,
    (s32)&CHASE_seq_kegdead, (s32)&CRA_seq_kegwin, (s32)&CRA_seq_kegvictory,
};
s32 COC_seq_none[2] = { 2, 0x8000 };
s32 COC_seq_breathe[3] = { (s32)0x80070000, 0x40000014, 0x8000 };
s32 COC_seq_taunt_a[13] = {
    (s32)0x800A0000, (s32)0x80070034, 0x44800031, 0x448F4490, 0x448D448E, 0x448B448C,
    0x4489448A, 0x44874488, 0x44854486, 0x44834484, 0x44814482, (s32)0x80044480, 0,
};
s32 COC_seq_idle_a[2] = { (s32)0x80050000, 1 };
s32 COC_seq_idle_b[2] = { (s32)0x80050000, 1 };
s32 COC_seq_medium[8] = {
    (s32)0x80070000, 0x41000008, 0x88003, 0xB8007, (s32)0x80034108, 0x41130009, 0x41154114,
    0x8000,
};
s32 COC_seq_jump[4] = { (s32)0x800A0000, (s32)0x8007003E, 0x4182000A, 0x58004 };
s32 COC_seq_fall[6] = { 0x418C0002, 0x48018, 0x418E418D, 0x288018, (s32)0x8004418F, 7 };
s32 COC_seq_land[4] = { (s32)0x80090000, (s32)0x8007003F, 0x4190000B, 0x8004 };
s32 COC_seq_attack[3] = { (s32)0x800A000F, 0x20020040, 0x8004 };
s32 COC_seq_hit[7] = { (s32)0x80070000, 0x4080000E, 0x88003, (s32)0x8018408E, (s32)0x80070002, 0x408F0005, 0x8004 };
s32 COC_seq_win[12] = {
    (s32)0x80070000, 0x44800031, 0x448F4490, 0x448D448E, 0x448B448C, 0x4489448A, 0x44874488,
    0x44854486, 0x44834484, 0x44814482, (s32)0x80004480, 0,
};
s32 COC_seq_victory[3] = { (s32)0x80070000, 0x45000064, 0x8000 };
s32 COC_seqs[108] = {
    (s32)&COC_seq_none, (s32)&COC_seq_breathe, (s32)&COC_seq_taunt_a, (s32)&COC_seq_idle_a, (s32)&COC_seq_idle_b,
    (s32)&COC_seq_medium, (s32)&COC_seq_jump, (s32)&COC_seq_fall, (s32)&COC_seq_land, (s32)&COC_seq_hit,
    (s32)&COC_seq_attack, (s32)&HERO_seq_dead, (s32)&COC_seq_win, (s32)&COC_seq_victory,
    (s32)&COC_BA_seq_slide, (s32)&COC_BA_seq_pickup, (s32)&COC_BA_seq_hold,
    (s32)&COC_BA_seq_hold_slow, (s32)&COC_BA_seq_hold_throw, (s32)&COC_BA_seq_die,
    (s32)&COC_PA_seq_pickup, (s32)&COC_PA_seq_hold, (s32)&COC_PA_seq_hold_slow,
    (s32)&COC_PA_seq_hold_throw, (s32)&COC_PA_seq_die, (s32)&COC_seq_arkbreathe,
    (s32)&COC_seq_arkmove, (s32)&COC_seq_arkidle, (s32)&COC_seq_arktaunt,
    (s32)&COC_seq_arkkick, (s32)&COC_seq_arkwin, (s32)&COC_seq_arkvictory,
    (s32)&COC_seq_arklose, (s32)&COC_seq_n2obreathe, (s32)&COC_seq_n2omove,
    (s32)&COC_seq_n2oidle, (s32)&COC_seq_n2otaunt, (s32)&COC_seq_n2okick, (s32)&COC_seq_n2ohit,
    (s32)&COC_seq_n2owin, (s32)&COC_seq_n2ovictory, (s32)&COC_seq_n2olose,
    (s32)&COC_seq_n2ofly, (s32)&COC_seq_long_hit, (s32)&COC_seq_flip_jump,
    (s32)&COC_SP_seq_die, (s32)&COC_seq_pogo_idle, (s32)&COC_seq_pogo_quick_idle,
    (s32)&COC_seq_pogo_tazing, (s32)&COC_seq_pogo_dazed, (s32)&COC_seq_pogo_winner,
    (s32)&COC_seq_bonk_idle, (s32)&COC_seq_bonk_run, (s32)&COC_seq_bonk_backswing,
    (s32)&COC_seq_bonk_extended_backswing, (s32)&COC_seq_bonk_swing, (s32)&COC_seq_bonk_hit,
    (s32)&COC_seq_bonk_winner, (s32)&COC_seq_polaridle, (s32)&COC_seq_polarrun,
    (s32)&COC_seq_polardazed, (s32)&COC_seq_polarfall, (s32)&COC_seq_polarattack,
    (s32)&COC_seq_polarhit, (s32)&COC_seq_polardead, (s32)&COC_seq_polartaunt,
    (s32)&COC_seq_breathe_ft, (s32)&COC_seq_skate_ft, (s32)&COC_seq_lightbear,
    (s32)&COC_seq_skate_attack, (s32)&COC_seq_polarwin, (s32)&COC_seq_win_ft,
    (s32)&COC_seq_raceidle, (s32)&COC_seq_racerun, (s32)&COC_seq_racehit,
    (s32)&COC_seq_racefall, (s32)&COC_seq_racewin, (s32)&COC_seq_dinoidle,
    (s32)&COC_seq_dinorun, (s32)&COC_seq_dinoshoot, (s32)&COC_seq_dinoslide,
    (s32)&COC_seq_dinodazed, (s32)&COC_seq_raceswimidle, (s32)&COC_seq_raceswimrun,
    (s32)&COC_seq_raceswimfall, (s32)&COC_seq_raceswimwin, (s32)&COC_seq_dinowin,
    (s32)&COC_seq_tankrecoil, (s32)&COC_seq_tankdie, (s32)&COC_seq_tankidle,
    (s32)&COC_seq_tankidle1, (s32)&COC_seq_tankminelay, (s32)&COC_seq_tankvictory,
    (s32)&COC_seq_tankfire1, (s32)&COC_seq_tankfire2, (s32)&COC_seq_tankfire3,
    (s32)&COC_seq_kegnone, (s32)&COC_seq_kegbreathe, (s32)&COC_seq_kegidle_a,
    (s32)&COC_seq_kegidle_b, (s32)&COC_seq_kegmedium, (s32)&COC_seq_kegjump,
    (s32)&COC_seq_kegfall, (s32)&COC_seq_kegland, (s32)&COC_seq_keghit,
    (s32)&CHASE_seq_kegdead, (s32)&COC_seq_kegwin, (s32)&COC_seq_kegvictory,
};
s32 CTX_seq_none[2] = { 2, 0x8000 };
s32 CTX_seq_breathe[3] = { (s32)0x80070000, 0x40800020, 0x8000 };
s32 CTX_seq_taunt_a[4] = { (s32)0x800A0000, (s32)0x80070066, 0x45000028, 0x8004 };
s32 CTX_seq_idle_a[2] = { (s32)0x80050000, 1 };
s32 CTX_seq_idle_b[2] = { (s32)0x80050000, 1 };
s32 CTX_seq_medium[7] = { 0x41150000, 0x88003, 0xB8007, (s32)0x80034100, (s32)0x80070009, 0x410B000A, 0x8000 };
s32 CTX_seq_jump[6] = { (s32)0x800A0000, (s32)0x80070070, 0x41800007, 0x288018, (s32)0x80044187, 5 };
s32 CTX_seq_fall[9] = {
    0x41870000, 0x41894188, 0x28018, 0x418B418A, 0x288018, 0x418C418C, 0x418C418C,
    (s32)0x8004418C, 7,
};
s32 CTX_seq_land[4] = { (s32)0x80090000, (s32)0x80070071, 0x418D0006, 0x8004 };
s32 CTX_seq_attack[4] = { (s32)0x800A0000, (s32)0x80070072, 0x42000013, 0x8004 };
s32 CTX_seq_hit[11] = {
    (s32)0x80070000, 0x4000000D, 0x98003, 0x410E410D, 0x4110410F, 0x41124111, 0x88003, 0x78007,
    (s32)0x80184013, 0x40190003, 0x8004,
};
s32 CTX_seq_win[3] = { (s32)0x80070000, 0x45000028, 0x8000 };
s32 CTX_seq_victory[5] = { (s32)0x80070000, 0x4480000B, 0x168007, (s32)0x8000448A, 3 };
s32 CTX_seqs[108] = {
    (s32)&CTX_seq_none, (s32)&CTX_seq_breathe, (s32)&CTX_seq_taunt_a, (s32)&CTX_seq_idle_a, (s32)&CTX_seq_idle_b,
    (s32)&CTX_seq_medium, (s32)&CTX_seq_jump, (s32)&CTX_seq_fall, (s32)&CTX_seq_land, (s32)&CTX_seq_hit,
    (s32)&CTX_seq_attack, (s32)&HERO_seq_dead, (s32)&CTX_seq_win, (s32)&CTX_seq_victory,
    (s32)&CTX_BA_seq_slide, (s32)&CTX_BA_seq_pickup, (s32)&CTX_BA_seq_hold,
    (s32)&CTX_BA_seq_hold_slow, (s32)&CTX_BA_seq_hold_throw, (s32)&CTX_BA_seq_die,
    (s32)&CTX_PA_seq_pickup, (s32)&CTX_PA_seq_hold, (s32)&CTX_PA_seq_hold_slow,
    (s32)&CTX_PA_seq_hold_throw, (s32)&CTX_PA_seq_die, (s32)&CTX_seq_arkbreathe,
    (s32)&CTX_seq_arkmove, (s32)&CTX_seq_arkidle, (s32)&CTX_seq_arktaunt,
    (s32)&CTX_seq_arkkick, (s32)&CTX_seq_arkwin, (s32)&CTX_seq_arkvictory,
    (s32)&CTX_seq_arklose, (s32)&CTX_seq_n2obreathe, (s32)&CTX_seq_n2omove,
    (s32)&CTX_seq_n2oidle, (s32)&CTX_seq_n2otaunt, (s32)&CTX_seq_n2okick, (s32)&CTX_seq_n2ohit,
    (s32)&CTX_seq_n2owin, (s32)&CTX_seq_n2ovictory, (s32)&CTX_seq_n2olose,
    (s32)&CTX_seq_n2ofly, (s32)&CTX_seq_long_hit, (s32)&CTX_seq_flip_jump,
    (s32)&CTX_SP_seq_die, (s32)&CTX_seq_pogo_idle, (s32)&CTX_seq_pogo_quick_idle,
    (s32)&CTX_seq_pogo_tazing, (s32)&CTX_seq_pogo_dazed, (s32)&CTX_seq_pogo_winner,
    (s32)&CTX_seq_bonk_idle, (s32)&CTX_seq_bonk_run, (s32)&CTX_seq_bonk_backswing,
    (s32)&CTX_seq_bonk_extended_backswing, (s32)&CTX_seq_bonk_swing, (s32)&CTX_seq_bonk_hit,
    (s32)&CTX_seq_bonk_winner, (s32)&CTX_seq_polaridle, (s32)&CTX_seq_polarrun,
    (s32)&CTX_seq_polardazed, (s32)&CTX_seq_polarfall, (s32)&CTX_seq_polarattack,
    (s32)&CTX_seq_polarhit, (s32)&CTX_seq_polardead, (s32)&CTX_seq_polartaunt,
    (s32)&CTX_seq_breathe_ft, (s32)&CTX_seq_skate_ft, (s32)&CTX_seq_lightbear,
    (s32)&CTX_seq_skate_attack, (s32)&CTX_seq_polarwin, (s32)&CTX_seq_win_ft,
    (s32)&CTX_seq_raceidle, (s32)&CTX_seq_racerun, (s32)&CTX_seq_racehit,
    (s32)&CTX_seq_racefall, (s32)&CTX_seq_racewin, (s32)&CTX_seq_dinoidle,
    (s32)&CTX_seq_dinorun, (s32)&CTX_seq_dinoshoot, (s32)&CTX_seq_dinoslide,
    (s32)&CTX_seq_dinodazed, (s32)&CTX_seq_raceswimidle, (s32)&CTX_seq_raceswimrun,
    (s32)&CTX_seq_raceswimfall, (s32)&CTX_seq_raceswimwin, (s32)&CTX_seq_dinowin,
    (s32)&CTX_seq_tankrecoil, (s32)&CTX_seq_tankdie, (s32)&CTX_seq_tankidle,
    (s32)&CTX_seq_tankidle1, (s32)&CTX_seq_tankminelay, (s32)&CTX_seq_tankvictory,
    (s32)&CTX_seq_tankfire1, (s32)&CTX_seq_tankfire2, (s32)&CTX_seq_tankfire3,
    (s32)&CTX_seq_kegnone, (s32)&CTX_seq_kegbreathe, (s32)&CTX_seq_kegidle_a,
    (s32)&CTX_seq_kegidle_b, (s32)&CTX_seq_kegmedium, (s32)&CTX_seq_kegjump,
    (s32)&CTX_seq_kegfall, (s32)&CTX_seq_kegland, (s32)&CTX_seq_keghit,
    (s32)&CHASE_seq_kegdead, (s32)&CTX_seq_kegwin, (s32)&CTX_seq_kegvictory,
};
s32 BRO_seq_none[2] = { 2, 0x8000 };
s32 BRO_seq_breathe[3] = { (s32)0x80070000, 0x40000020, 0x8000 };
s32 BRO_seq_taunt_a[4] = { (s32)0x800A0000, (s32)0x80070098, 0x46000028, 0x8004 };
s32 BRO_seq_idle_a[2] = { (s32)0x80050000, 1 };
s32 BRO_seq_idle_b[2] = { (s32)0x80050000, 1 };
s32 BRO_seq_medium[7] = { 0x40950000, 0x88003, 0xB8007, (s32)0x80034080, (s32)0x80070009, 0x408B000A, 0x8000 };
s32 BRO_seq_jump[6] = { (s32)0x800A0000, (s32)0x800700A2, 0x41800007, 0x288018, (s32)0x80044183, 5 };
s32 BRO_seq_fall[9] = {
    0x41840000, 0x41864185, 0x28018, 0x41884187, 0x288018, 0x41894189, 0x41894189,
    (s32)0x80044189, 7,
};
s32 BRO_seq_land[4] = { (s32)0x80090000, (s32)0x800700A3, 0x418A0004, 0x8004 };
s32 BRO_seq_attack[4] = { (s32)0x800A0000, (s32)0x800700A4, 0x4480000D, 0x8004 };
s32 BRO_seq_hit[9] = {
    (s32)0x80070000, 0x4100000D, 0x98003, 0x410E410D, 0x88003, 0x58007, (s32)0x8018410F,
    0x41130009, 0x8004,
};
s32 BRO_seq_win[3] = { (s32)0x80070000, 0x46000028, 0x8000 };
s32 BRO_seq_victory[3] = { (s32)0x80070000, 0x45800018, 0x8000 };
s32 BRO_seqs[108] = {
    (s32)&BRO_seq_none, (s32)&BRO_seq_breathe, (s32)&BRO_seq_taunt_a, (s32)&BRO_seq_idle_a, (s32)&BRO_seq_idle_b,
    (s32)&BRO_seq_medium, (s32)&BRO_seq_jump, (s32)&BRO_seq_fall, (s32)&BRO_seq_land, (s32)&BRO_seq_hit,
    (s32)&BRO_seq_attack, (s32)&HERO_seq_dead, (s32)&BRO_seq_win, (s32)&BRO_seq_victory,
    (s32)&BRO_BA_seq_slide, (s32)&BRO_BA_seq_pickup, (s32)&BRO_BA_seq_hold,
    (s32)&BRO_BA_seq_hold_slow, (s32)&BRO_BA_seq_hold_throw, (s32)&BRO_BA_seq_die,
    (s32)&BRO_PA_seq_pickup, (s32)&BRO_PA_seq_hold, (s32)&BRO_PA_seq_hold_slow,
    (s32)&BRO_PA_seq_hold_throw, (s32)&BRO_PA_seq_die, (s32)&BRO_seq_arkbreathe,
    (s32)&BRO_seq_arkmove, (s32)&BRO_seq_arkidle, (s32)&BRO_seq_arktaunt,
    (s32)&BRO_seq_arkkick, (s32)&BRO_seq_arkwin, (s32)&BRO_seq_arkvictory,
    (s32)&BRO_seq_arklose, (s32)&BRO_seq_n2obreathe, (s32)&BRO_seq_n2omove,
    (s32)&BRO_seq_n2oidle, (s32)&BRO_seq_n2otaunt, (s32)&BRO_seq_n2okick, (s32)&BRO_seq_n2ohit,
    (s32)&BRO_seq_n2owin, (s32)&BRO_seq_n2ovictory, (s32)&BRO_seq_n2olose,
    (s32)&BRO_seq_n2ofly, (s32)&BRO_seq_long_hit, (s32)&BRO_seq_flip_jump,
    (s32)&BRO_SP_seq_die, (s32)&BRO_seq_pogo_idle, (s32)&BRO_seq_pogo_quick_idle,
    (s32)&BRO_seq_pogo_tazing, (s32)&BRO_seq_pogo_dazed, (s32)&BRO_seq_pogo_winner,
    (s32)&BRO_seq_bonk_idle, (s32)&BRO_seq_bonk_run, (s32)&BRO_seq_bonk_backswing,
    (s32)&BRO_seq_bonk_extended_backswing, (s32)&BRO_seq_bonk_swing, (s32)&BRO_seq_bonk_hit,
    (s32)&BRO_seq_bonk_winner, (s32)&BRO_seq_polaridle, (s32)&BRO_seq_polarrun,
    (s32)&BRO_seq_polardazed, (s32)&BRO_seq_polarfall, (s32)&BRO_seq_polarattack,
    (s32)&BRO_seq_polarhit, (s32)&BRO_seq_polardead, (s32)&BRO_seq_polartaunt,
    (s32)&BRO_seq_breathe_ft, (s32)&BRO_seq_skate_ft, (s32)&BRO_seq_lightbear,
    (s32)&BRO_seq_skate_attack, (s32)&BRO_seq_polarwin, (s32)&BRO_seq_win_ft,
    (s32)&BRO_seq_raceidle, (s32)&BRO_seq_racerun, (s32)&BRO_seq_racehit,
    (s32)&BRO_seq_racefall, (s32)&BRO_seq_racewin, (s32)&BRO_seq_dinoidle,
    (s32)&BRO_seq_dinorun, (s32)&BRO_seq_dinoshoot, (s32)&BRO_seq_dinoslide,
    (s32)&BRO_seq_dinodazed, (s32)&BRO_seq_raceswimidle, (s32)&BRO_seq_raceswimrun,
    (s32)&BRO_seq_raceswimfall, (s32)&BRO_seq_raceswimwin, (s32)&BRO_seq_dinowin,
    (s32)&BRO_seq_tankrecoil, (s32)&BRO_seq_tankdie, (s32)&BRO_seq_tankidle,
    (s32)&BRO_seq_tankidle1, (s32)&BRO_seq_tankminelay, (s32)&BRO_seq_tankvictory,
    (s32)&BRO_seq_tankfire1, (s32)&BRO_seq_tankfire2, (s32)&BRO_seq_tankfire3,
    (s32)&BRO_seq_kegnone, (s32)&BRO_seq_kegbreathe, (s32)&BRO_seq_kegidle_a,
    (s32)&BRO_seq_kegidle_b, (s32)&BRO_seq_kegmedium, (s32)&BRO_seq_kegjump,
    (s32)&BRO_seq_kegfall, (s32)&BRO_seq_kegland, (s32)&BRO_seq_keghit,
    (s32)&CHASE_seq_kegdead, (s32)&BRO_seq_kegwin, (s32)&BRO_seq_kegvictory,
};
s32 TIN_seq_none[2] = { 2, 0x8000 };
s32 TIN_seq_breathe[3] = { (s32)0x80070000, 0x40000020, 0x8000 };
s32 TIN_seq_taunt_a[7] = { (s32)0x800A0000, (s32)0x8007012E, 0x4580002A, 0x45834584, 0x45814582, (s32)0x80044580, 0 };
s32 TIN_seq_idle_a[2] = { (s32)0x80050000, 1 };
s32 TIN_seq_idle_b[2] = { (s32)0x80050000, 1 };
s32 TIN_seq_medium[7] = { 0x41130000, 0x88003, 0xA8007, (s32)0x80034100, (s32)0x80070009, 0x410A0009, 0x8000 };
s32 TIN_seq_jump[6] = { (s32)0x800A0000, (s32)0x80070138, 0x41800007, 0x288018, (s32)0x80044187, 5 };
s32 TIN_seq_fall[9] = {
    0x41880002, 0x418A4189, 0x418C418B, 0x48018, 0x418E418D, (s32)0x8018418F, 0x41900028,
    (s32)0x80044190, 7,
};
s32 TIN_seq_land[4] = { (s32)0x80090000, (s32)0x80070139, 0x4191000B, 0x8004 };
s32 TIN_seq_attack[4] = { (s32)0x800A0000, (s32)0x8007013A, 0x4200000C, 0x8004 };
s32 TIN_seq_hit[6] = { (s32)0x80070000, 0x40800013, 0x88003, 0xB8007, (s32)0x80044093, 0 };
s32 TIN_seq_win[6] = { (s32)0x80070000, 0x4580002A, 0x45834584, 0x45814582, (s32)0x80004580, 0 };
s32 TIN_seq_victory[5] = { (s32)0x80070000, 0x45000022, 0x2C8007, (s32)0x80004522, 3 };
s32 TIN_seqs[108] = {
    (s32)&TIN_seq_none, (s32)&TIN_seq_breathe, (s32)&TIN_seq_taunt_a, (s32)&TIN_seq_idle_a, (s32)&TIN_seq_idle_b,
    (s32)&TIN_seq_medium, (s32)&TIN_seq_jump, (s32)&TIN_seq_fall, (s32)&TIN_seq_land, (s32)&TIN_seq_hit,
    (s32)&TIN_seq_attack, (s32)&HERO_seq_dead, (s32)&TIN_seq_win, (s32)&TIN_seq_victory,
    (s32)&TIN_BA_seq_slide, (s32)&TIN_BA_seq_pickup, (s32)&TIN_BA_seq_hold,
    (s32)&TIN_BA_seq_hold_slow, (s32)&TIN_BA_seq_hold_throw, (s32)&TIN_BA_seq_die,
    (s32)&TIN_PA_seq_pickup, (s32)&TIN_PA_seq_hold, (s32)&TIN_PA_seq_hold_slow,
    (s32)&TIN_PA_seq_hold_throw, (s32)&TIN_PA_seq_die, (s32)&TIN_seq_arkbreathe,
    (s32)&TIN_seq_arkmove, (s32)&TIN_seq_arkidle, (s32)&TIN_seq_arktaunt,
    (s32)&TIN_seq_arkkick, (s32)&TIN_seq_arkwin, (s32)&TIN_seq_arkvictory,
    (s32)&TIN_seq_arklose, (s32)&TIN_seq_n2obreathe, (s32)&TIN_seq_n2omove,
    (s32)&TIN_seq_n2oidle, (s32)&TIN_seq_n2otaunt, (s32)&TIN_seq_n2okick, (s32)&TIN_seq_n2ohit,
    (s32)&TIN_seq_n2owin, (s32)&TIN_seq_n2ovictory, (s32)&TIN_seq_n2olose,
    (s32)&TIN_seq_n2ofly, (s32)&TIN_seq_long_hit, (s32)&TIN_seq_flip_jump,
    (s32)&TIN_SP_seq_die, (s32)&TIN_seq_pogo_idle, (s32)&TIN_seq_pogo_quick_idle,
    (s32)&TIN_seq_pogo_tazing, (s32)&TIN_seq_pogo_dazed, (s32)&TIN_seq_pogo_winner,
    (s32)&TIN_seq_bonk_idle, (s32)&TIN_seq_bonk_run, (s32)&TIN_seq_bonk_backswing,
    (s32)&TIN_seq_bonk_extended_backswing, (s32)&TIN_seq_bonk_swing, (s32)&TIN_seq_bonk_hit,
    (s32)&TIN_seq_bonk_winner, (s32)&TIN_seq_polaridle, (s32)&TIN_seq_polarrun,
    (s32)&TIN_seq_polardazed, (s32)&TIN_seq_polarfall, (s32)&TIN_seq_polarattack,
    (s32)&TIN_seq_polarhit, (s32)&TIN_seq_polardead, (s32)&TIN_seq_polartaunt,
    (s32)&TIN_seq_breathe_ft, (s32)&TIN_seq_skate_ft, (s32)&TIN_seq_lightbear,
    (s32)&TIN_seq_skate_attack, (s32)&TIN_seq_polarwin, (s32)&TIN_seq_win_ft,
    (s32)&TIN_seq_raceidle, (s32)&TIN_seq_racerun, (s32)&TIN_seq_racehit,
    (s32)&TIN_seq_racefall, (s32)&TIN_seq_racewin, (s32)&TIN_seq_dinoidle,
    (s32)&TIN_seq_dinorun, (s32)&TIN_seq_dinoshoot, (s32)&TIN_seq_dinoslide,
    (s32)&TIN_seq_dinodazed, (s32)&TIN_seq_raceswimidle, (s32)&TIN_seq_raceswimrun,
    (s32)&TIN_seq_raceswimfall, (s32)&TIN_seq_raceswimwin, (s32)&TIN_seq_dinowin,
    (s32)&TIN_seq_tankrecoil, (s32)&TIN_seq_tankdie, (s32)&TIN_seq_tankidle,
    (s32)&TIN_seq_tankidle1, (s32)&TIN_seq_tankminelay, (s32)&TIN_seq_tankvictory,
    (s32)&TIN_seq_tankfire1, (s32)&TIN_seq_tankfire2, (s32)&TIN_seq_tankfire3,
    (s32)&TIN_seq_kegnone, (s32)&TIN_seq_kegbreathe, (s32)&TIN_seq_kegidle_a,
    (s32)&TIN_seq_kegidle_b, (s32)&TIN_seq_kegmedium, (s32)&TIN_seq_kegjump,
    (s32)&TIN_seq_kegfall, (s32)&TIN_seq_kegland, (s32)&TIN_seq_keghit,
    (s32)&CHASE_seq_kegdead, (s32)&TIN_seq_kegwin, (s32)&TIN_seq_kegvictory,
};
s32 KON_seq_none[2] = { 2, 0x8000 };
s32 KON_seq_breathe[3] = { (s32)0x80070000, 0x40800020, 0x8000 };
s32 KON_seq_taunt_a[10] = {
    (s32)0x800A0000, (s32)0x80070160, 0x46000030, 0x460A460B, 0x46084609, 0x46064607,
    0x46044605, 0x46024603, 0x46004601, 0x8004,
};
s32 KON_seq_idle_a[3] = { (s32)0x80070000, 0x40800020, 0x18005 };
s32 KON_seq_idle_b[3] = { (s32)0x80070000, 0x40800020, 0x38005 };
s32 KON_seq_medium[7] = { 0x40130000, 0x88003, 0xA8007, (s32)0x80034000, (s32)0x80070009, 0x40090009, 0x8000 };
s32 KON_seq_jump[6] = { (s32)0x800A0000, (s32)0x8007016A, 0x42810007, 0x288018, (s32)0x80044288, 5 };
s32 KON_seq_fall[9] = {
    0x42890002, 0x428B428A, 0x428D428C, 0x48018, 0x428F428E, (s32)0x80184290, 0x42910028,
    (s32)0x80044291, 7,
};
s32 KON_seq_land[4] = { (s32)0x80090000, (s32)0x8007016B, 0x42920009, 0x8004 };
s32 KON_seq_attack[4] = { (s32)0x800A0000, (s32)0x8007016C, 0x41000010, 0x8004 };
s32 KON_seq_hit[6] = { (s32)0x80070000, 0x41800013, 0x88003, 0xA8007, (s32)0x80044193, 0 };
s32 KON_seq_win[9] = {
    (s32)0x80070000, 0x46000030, 0x460A460B, 0x46084609, 0x46064607, 0x46044605, 0x46024603,
    0x46004601, 0x8000,
};
s32 KON_seq_victory[5] = { (s32)0x80070000, 0x45800021, 0x2E8007, (s32)0x800045A1, 3 };
s32 KON_seqs[108] = {
    (s32)&KON_seq_none, (s32)&KON_seq_breathe, (s32)&KON_seq_taunt_a, (s32)&KON_seq_idle_a, (s32)&KON_seq_idle_b,
    (s32)&KON_seq_medium, (s32)&KON_seq_jump, (s32)&KON_seq_fall, (s32)&KON_seq_land, (s32)&KON_seq_hit,
    (s32)&KON_seq_attack, (s32)&HERO_seq_dead, (s32)&KON_seq_win, (s32)&KON_seq_victory,
    (s32)&KON_BA_seq_slide, (s32)&KON_BA_seq_pickup, (s32)&KON_BA_seq_hold,
    (s32)&KON_BA_seq_hold_slow, (s32)&KON_BA_seq_hold_throw, (s32)&KON_BA_seq_die,
    (s32)&KON_PA_seq_pickup, (s32)&KON_PA_seq_hold, (s32)&KON_PA_seq_hold_slow,
    (s32)&KON_PA_seq_hold_throw, (s32)&KON_PA_seq_die, (s32)&KON_seq_arkbreathe,
    (s32)&KON_seq_arkmove, (s32)&KON_seq_arkidle, (s32)&KON_seq_arktaunt,
    (s32)&KON_seq_arkkick, (s32)&KON_seq_arkwin, (s32)&KON_seq_arkvictory,
    (s32)&KON_seq_arklose, (s32)&KON_seq_n2obreathe, (s32)&KON_seq_n2omove,
    (s32)&KON_seq_n2oidle, (s32)&KON_seq_n2otaunt, (s32)&KON_seq_n2okick, (s32)&KON_seq_n2ohit,
    (s32)&KON_seq_n2owin, (s32)&KON_seq_n2ovictory, (s32)&KON_seq_n2olose,
    (s32)&KON_seq_n2ofly, (s32)&KON_seq_long_hit, (s32)&KON_seq_flip_jump,
    (s32)&KON_SP_seq_die, (s32)&KON_seq_pogo_idle, (s32)&KON_seq_pogo_quick_idle,
    (s32)&KON_seq_pogo_tazing, (s32)&KON_seq_pogo_dazed, (s32)&KON_seq_pogo_winner,
    (s32)&KON_seq_bonk_idle, (s32)&KON_seq_bonk_run, (s32)&KON_seq_bonk_backswing,
    (s32)&KON_seq_bonk_extended_backswing, (s32)&KON_seq_bonk_swing, (s32)&KON_seq_bonk_hit,
    (s32)&KON_seq_bonk_winner, (s32)&KON_seq_polaridle, (s32)&KON_seq_polarrun,
    (s32)&KON_seq_polardazed, (s32)&KON_seq_polarfall, (s32)&KON_seq_polarattack,
    (s32)&KON_seq_polarhit, (s32)&KON_seq_polardead, (s32)&KON_seq_polartaunt,
    (s32)&KON_seq_breathe_ft, (s32)&KON_seq_skate_ft, (s32)&KON_seq_lightbear,
    (s32)&KON_seq_skate_attack, (s32)&KON_seq_polarwin, (s32)&KON_seq_win_ft,
    (s32)&KON_seq_raceidle, (s32)&KON_seq_racerun, (s32)&KON_seq_racehit,
    (s32)&KON_seq_racefall, (s32)&KON_seq_racewin, (s32)&KON_seq_dinoidle,
    (s32)&KON_seq_dinorun, (s32)&KON_seq_dinoshoot, (s32)&KON_seq_dinoslide,
    (s32)&KON_seq_dinodazed, (s32)&KON_seq_raceswimidle, (s32)&KON_seq_raceswimrun,
    (s32)&KON_seq_raceswimfall, (s32)&KON_seq_raceswimwin, (s32)&KON_seq_dinowin,
    (s32)&KON_seq_tankrecoil, (s32)&KON_seq_tankdie, (s32)&KON_seq_tankidle,
    (s32)&KON_seq_tankidle1, (s32)&KON_seq_tankminelay, (s32)&KON_seq_tankvictory,
    (s32)&KON_seq_tankfire1, (s32)&KON_seq_tankfire2, (s32)&KON_seq_tankfire3,
    (s32)&KON_seq_kegnone, (s32)&KON_seq_kegbreathe, (s32)&KON_seq_kegidle_a,
    (s32)&KON_seq_kegidle_b, (s32)&KON_seq_kegmedium, (s32)&KON_seq_kegjump,
    (s32)&KON_seq_kegfall, (s32)&KON_seq_kegland, (s32)&KON_seq_keghit,
    (s32)&CHASE_seq_kegdead, (s32)&KON_seq_kegwin, (s32)&KON_seq_kegvictory,
};
s32 DNG_seq_none[2] = { 2, 0x8000 };
s32 DNG_seq_breathe[3] = { (s32)0x80070000, 0x40000016, 0x8000 };
s32 DNG_seq_taunt_a[4] = { (s32)0x800A0000, (s32)0x800700FC, 0x46000028, 0x8004 };
s32 DNG_seq_idle_a[3] = { (s32)0x80070000, 0x4000000E, 0x18005 };
s32 DNG_seq_idle_b[3] = { (s32)0x80070000, 0x4000000C, 0x38005 };
s32 DNG_seq_medium[8] = {
    (s32)0x80070000, 0x40800004, 0x88003, 0x98007, (s32)0x80034084, (s32)0x80070009,
    0x408D0005, 0x8000,
};
s32 DNG_seq_jump[6] = { (s32)0x800A0000, (s32)0x80070106, 0x41000008, 0x288018, (s32)0x80044107, 5 };
s32 DNG_seq_fall[6] = { 0x41080002, 0x48018, 0x410A4109, 0x288018, (s32)0x8004410B, 7 };
s32 DNG_seq_land[4] = { (s32)0x80090000, (s32)0x80070107, 0x410C000F, 0x8004 };
s32 DNG_seq_attack[4] = { (s32)0x800A0000, (s32)0x80070108, 0x4180000E, 0x78004 };
s32 DNG_seq_hit[6] = { (s32)0x80070000, 0x4200000B, 0x88003, 0x138007, (s32)0x8004420B, 0 };
s32 DNG_seq_win[3] = { (s32)0x80070000, 0x46000028, 0x8000 };
s32 DNG_seq_victory[3] = { (s32)0x80070000, 0x4580004C, 0x8000 };
s32 DNG_seqs[108] = {
    (s32)&DNG_seq_none, (s32)&DNG_seq_breathe, (s32)&DNG_seq_taunt_a, (s32)&DNG_seq_idle_a, (s32)&DNG_seq_idle_b,
    (s32)&DNG_seq_medium, (s32)&DNG_seq_jump, (s32)&DNG_seq_fall, (s32)&DNG_seq_land, (s32)&DNG_seq_hit,
    (s32)&DNG_seq_attack, (s32)&HERO_seq_dead, (s32)&DNG_seq_win, (s32)&DNG_seq_victory,
    (s32)&DNG_BA_seq_slide, (s32)&DNG_BA_seq_pickup, (s32)&DNG_BA_seq_hold,
    (s32)&DNG_BA_seq_hold_slow, (s32)&DNG_BA_seq_hold_throw, (s32)&DNG_BA_seq_die,
    (s32)&DNG_PA_seq_pickup, (s32)&DNG_PA_seq_hold, (s32)&DNG_PA_seq_hold_slow,
    (s32)&DNG_PA_seq_hold_throw, (s32)&DNG_PA_seq_die, (s32)&DNG_seq_arkbreathe,
    (s32)&DNG_seq_arkmove, (s32)&DNG_seq_arkidle, (s32)&DNG_seq_arktaunt,
    (s32)&DNG_seq_arkkick, (s32)&DNG_seq_arkwin, (s32)&DNG_seq_arkvictory,
    (s32)&DNG_seq_arklose, (s32)&DNG_seq_n2obreathe, (s32)&DNG_seq_n2omove,
    (s32)&DNG_seq_n2oidle, (s32)&DNG_seq_n2otaunt, (s32)&DNG_seq_n2okick, (s32)&DNG_seq_n2ohit,
    (s32)&DNG_seq_n2owin, (s32)&DNG_seq_n2ovictory, (s32)&DNG_seq_n2olose,
    (s32)&DNG_seq_n2ofly, (s32)&DNG_seq_long_hit, (s32)&DNG_seq_flip_jump,
    (s32)&DNG_SP_seq_die, (s32)&DNG_seq_pogo_idle, (s32)&DNG_seq_pogo_quick_idle,
    (s32)&DNG_seq_pogo_tazing, (s32)&DNG_seq_pogo_dazed, (s32)&DNG_seq_pogo_winner,
    (s32)&DNG_seq_bonk_idle, (s32)&DNG_seq_bonk_run, (s32)&DNG_seq_bonk_backswing,
    (s32)&DNG_seq_bonk_extended_backswing, (s32)&DNG_seq_bonk_swing, (s32)&DNG_seq_bonk_hit,
    (s32)&DNG_seq_bonk_winner, (s32)&DNG_seq_polaridle, (s32)&DNG_seq_polarrun,
    (s32)&DNG_seq_polardazed, (s32)&DNG_seq_polarfall, (s32)&DNG_seq_polarattack,
    (s32)&DNG_seq_polarhit, (s32)&DNG_seq_polardead, (s32)&DNG_seq_polartaunt,
    (s32)&DNG_seq_breathe_ft, (s32)&DNG_seq_skate_ft, (s32)&DNG_seq_lightbear,
    (s32)&DNG_seq_skate_attack, (s32)&DNG_seq_polarwin, (s32)&DNG_seq_win_ft,
    (s32)&DNG_seq_raceidle, (s32)&DNG_seq_racerun, (s32)&DNG_seq_racehit,
    (s32)&DNG_seq_racefall, (s32)&DNG_seq_racewin, (s32)&DNG_seq_dinoidle,
    (s32)&DNG_seq_dinorun, (s32)&DNG_seq_dinoshoot, (s32)&DNG_seq_dinoslide,
    (s32)&DNG_seq_dinodazed, (s32)&DNG_seq_raceswimidle, (s32)&DNG_seq_raceswimrun,
    (s32)&DNG_seq_raceswimfall, (s32)&DNG_seq_raceswimwin, (s32)&DNG_seq_dinowin,
    (s32)&DNG_seq_tankrecoil, (s32)&DNG_seq_tankdie, (s32)&DNG_seq_tankidle,
    (s32)&DNG_seq_tankidle1, (s32)&DNG_seq_tankminelay, (s32)&DNG_seq_tankvictory,
    (s32)&DNG_seq_tankfire1, (s32)&DNG_seq_tankfire2, (s32)&DNG_seq_tankfire3,
    (s32)&DNG_seq_kegnone, (s32)&DNG_seq_kegbreathe, (s32)&DNG_seq_kegidle_a,
    (s32)&DNG_seq_kegidle_b, (s32)&DNG_seq_kegmedium, (s32)&DNG_seq_kegjump,
    (s32)&DNG_seq_kegfall, (s32)&DNG_seq_kegland, (s32)&DNG_seq_keghit,
    (s32)&CHASE_seq_kegdead, (s32)&DNG_seq_kegwin, (s32)&DNG_seq_kegvictory,
};
s32 KAN_seq_none[2] = { 2, 0x8000 };
s32 KAN_seq_breathe[3] = { (s32)0x80070000, 0x40000014, 0x8000 };
s32 KAN_seq_taunt_a[7] = { (s32)0x800A0000, (s32)0x800700CA, 0x4580000D, 0xD8007, (s32)0x80074580, 0x4580000D, 0x8004 };
s32 KAN_seq_idle_a[3] = { (s32)0x80070000, 0x40000014, 0x18005 };
s32 KAN_seq_idle_b[3] = { (s32)0x80070000, 0x40000014, 0x38005 };
s32 KAN_seq_medium[9] = {
    0x40860000, 0x88003, 0xA8007, (s32)0x80034087, 0x40910009, 0x40934092, 0x68007,
    (s32)0x80004080, 0,
};
s32 KAN_seq_jump[6] = { (s32)0x800A0000, (s32)0x800700D4, 0x41030009, 0x288018, (s32)0x8004410C, 5 };
s32 KAN_seq_fall[5] = { 0x410D0001, 0x410F410E, 0x288018, (s32)0x80044110, 7 };
s32 KAN_seq_land[4] = { (s32)0x80090000, (s32)0x800700D5, 0x41110007, 0x8004 };
s32 KAN_seq_attack[4] = { (s32)0x80080000, (s32)0x800700D6, 0x4180000E, 0x78004 };
s32 KAN_seq_hit[9] = {
    (s32)0x80070000, 0x4200000A, 0x88003, 0x48007, (s32)0x80184209, 0x42140004, 0x42164215,
    (s32)0x80044217, 0,
};
s32 KAN_seq_win[5] = { (s32)0x80070000, 0x40000014, 0xD8007, (s32)0x80004580, 0 };
s32 KAN_seq_victory[3] = { (s32)0x80070000, 0x46000076, 0x8000 };
s32 KAN_seqs[108] = {
    (s32)&KAN_seq_none, (s32)&KAN_seq_breathe, (s32)&KAN_seq_taunt_a, (s32)&KAN_seq_idle_a, (s32)&KAN_seq_idle_b,
    (s32)&KAN_seq_medium, (s32)&KAN_seq_jump, (s32)&KAN_seq_fall, (s32)&KAN_seq_land, (s32)&KAN_seq_hit,
    (s32)&KAN_seq_attack, (s32)&HERO_seq_dead, (s32)&KAN_seq_win, (s32)&KAN_seq_victory,
    (s32)&KAN_BA_seq_slide, (s32)&KAN_BA_seq_pickup, (s32)&KAN_BA_seq_hold,
    (s32)&KAN_BA_seq_hold_slow, (s32)&KAN_BA_seq_hold_throw, (s32)&KAN_BA_seq_die,
    (s32)&KAN_PA_seq_pickup, (s32)&KAN_PA_seq_hold, (s32)&KAN_PA_seq_hold_slow,
    (s32)&KAN_PA_seq_hold_throw, (s32)&KAN_PA_seq_die, (s32)&KAN_seq_arkbreathe,
    (s32)&KAN_seq_arkmove, (s32)&KAN_seq_arkidle, (s32)&KAN_seq_arktaunt,
    (s32)&KAN_seq_arkkick, (s32)&KAN_seq_arkwin, (s32)&KAN_seq_arkvictory,
    (s32)&KAN_seq_arklose, (s32)&KAN_seq_n2obreathe, (s32)&KAN_seq_n2omove,
    (s32)&KAN_seq_n2oidle, (s32)&KAN_seq_n2otaunt, (s32)&KAN_seq_n2okick, (s32)&KAN_seq_n2ohit,
    (s32)&KAN_seq_n2owin, (s32)&KAN_seq_n2ovictory, (s32)&KAN_seq_n2olose,
    (s32)&KAN_seq_n2ofly, (s32)&KAN_seq_long_hit, (s32)&KAN_seq_flip_jump,
    (s32)&KAN_SP_seq_die, (s32)&KAN_seq_pogo_idle, (s32)&KAN_seq_pogo_quick_idle,
    (s32)&KAN_seq_pogo_tazing, (s32)&KAN_seq_pogo_dazed, (s32)&KAN_seq_pogo_winner,
    (s32)&KAN_seq_bonk_idle, (s32)&KAN_seq_bonk_run, (s32)&KAN_seq_bonk_backswing,
    (s32)&KAN_seq_bonk_extended_backswing, (s32)&KAN_seq_bonk_swing, (s32)&KAN_seq_bonk_hit,
    (s32)&KAN_seq_bonk_winner, (s32)&KAN_seq_polaridle, (s32)&KAN_seq_polarrun,
    (s32)&KAN_seq_polardazed, (s32)&KAN_seq_polarfall, (s32)&KAN_seq_polarattack,
    (s32)&KAN_seq_polarhit, (s32)&KAN_seq_polardead, (s32)&KAN_seq_polartaunt,
    (s32)&KAN_seq_breathe_ft, (s32)&KAN_seq_skate_ft, (s32)&KAN_seq_lightbear,
    (s32)&KAN_seq_skate_attack, (s32)&KAN_seq_polarwin, (s32)&KAN_seq_win_ft,
    (s32)&KAN_seq_raceidle, (s32)&KAN_seq_racerun, (s32)&KAN_seq_racehit,
    (s32)&KAN_seq_racefall, (s32)&KAN_seq_racewin, (s32)&KAN_seq_dinoidle,
    (s32)&KAN_seq_dinorun, (s32)&KAN_seq_dinoshoot, (s32)&KAN_seq_dinoslide,
    (s32)&KAN_seq_dinodazed, (s32)&KAN_seq_raceswimidle, (s32)&KAN_seq_raceswimrun,
    (s32)&KAN_seq_raceswimfall, (s32)&KAN_seq_raceswimwin, (s32)&KAN_seq_dinowin,
    (s32)&KAN_seq_tankrecoil, (s32)&KAN_seq_tankdie, (s32)&KAN_seq_tankidle,
    (s32)&KAN_seq_tankidle1, (s32)&KAN_seq_tankminelay, (s32)&KAN_seq_tankvictory,
    (s32)&KAN_seq_tankfire1, (s32)&KAN_seq_tankfire2, (s32)&KAN_seq_tankfire3,
    (s32)&KAN_seq_kegnone, (s32)&KAN_seq_kegbreathe, (s32)&KAN_seq_kegidle_a,
    (s32)&KAN_seq_kegidle_b, (s32)&KAN_seq_kegmedium, (s32)&KAN_seq_kegjump,
    (s32)&KAN_seq_kegfall, (s32)&KAN_seq_kegland, (s32)&KAN_seq_keghit,
    (s32)&CHASE_seq_kegdead, (s32)&KAN_seq_kegwin, (s32)&KAN_seq_kegvictory,
};
hero_seq **HeroSeqTables[18] = {
    0, (void *)&CRA_seqs, (void *)&COC_seqs, (void *)&CTX_seqs, (void *)&BRO_seqs,
    (void *)&TIN_seqs, (void *)&KON_seqs, (void *)&DNG_seqs, (void *)&KAN_seqs,
    (void *)&HeroNameNobody, (void *)&HeroNameCrash, (void *)&HeroNameCoco,
    (void *)&HeroNameCortex, (void *)&HeroNameBrio, (void *)&HeroNameTiny,
    (void *)&HeroNameKong, (void *)&HeroNameDingodile, (void *)&HeroNameKanga,
};
u8 DefaultHeroModels[20] = { 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 0, 0 };
s32 DefaultVictorySeqs[8] = {
    (s32)&HeroAnimWin, 0xD, (s32)&HeroAnimWin, 0xD, (s32)&HeroAnimWin, 0xD, (s32)&HeroAnimWin,
    0xD,
};
u8 DefaultRunSpeeds[60] = {
    0, 0, 0, 0, 0, 0, 0x1E, 0, 0x2D, 0, 0x14, 0, 0x1E, 0, 0x2D, 0, 0x14, 0, 0x1E, 0, 0x2D, 0,
    0x14, 0, 0x1E, 0, 0x2D, 0, 0x14, 0, 0x28, 0, 0x2D, 0, 0x14, 0, 0x28, 0, 0x2D, 0, 0x14, 0,
    0x28, 0, 0x2D, 0, 0x14, 0, 0x28, 0, 0x2D, 0, 0x14, 0, 0, 0, 0, 0, 0, 0,
};
u8 DefaultJumpEnts[80] = {
    0x6C, 0, 0x72, 0, 8, 0, 0xA, 0, 0x6C, 0, 0x72, 0, 8, 0, 0xA, 0, 0x6C, 0, 0x72, 0, 8, 0,
    0xA, 0, 0x6C, 0, 0x72, 0, 8, 0, 0xA, 0, 0x6C, 0, 0x72, 0, 8, 0, 0xA, 0, 0x48, 0, 0x72, 0,
    0xA, 0, 6, 0, 0x48, 0, 0x72, 0, 0xA, 0, 6, 0, 0x48, 0, 0x72, 0, 0xA, 0, 6, 0, 0x48, 0,
    0x72, 0, 0xA, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u16 HeroEntities[10] = { 0, 0x2001, 0x2001, 0x2001, 0x2001, 0x2001, 0x2001, 0x2001, 0x2001, 0 };
u16 HitDamage[9][4] = {
    { 0, 0, 0, 0 }, { 0x100, 0x100, 0x100, 0x100 }, { 0x200, 0x200, 0x200, 0x200 },
    { 0x300, 0x200, 0x200, 0x100 }, { 0x400, 0x400, 0x400, 0x400 },
    { 0x500, 0x400, 0x300, 0x200 }, { 0x600, 0x600, 0x600, 0x600 },
    { 0x700, 0x700, 0x700, 0x700 }, { 0x800, 0x700, 0x600, 0x500 },
};
u16 HitFalloff[9][9] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100, 0x100 },
    { 0x200, 0x1C0, 0x180, 0x140, 0x100, 0xC0, 0x80, 0x40, 0x40 },
    { 0x300, 0x2C0, 0x280, 0x240, 0x200, 0x1C0, 0x180, 0x140, 0x100 },
    { 0x400, 0x3C0, 0x380, 0x340, 0x300, 0x2C0, 0x280, 0x240, 0x200 },
    { 0x500, 0x4C0, 0x480, 0x440, 0x400, 0x3C0, 0x380, 0x340, 0x300 },
    { 0x600, 0x5C0, 0x580, 0x540, 0x500, 0x4C0, 0x480, 0x440, 0x400 },
    { 0x700, 0x6C0, 0x680, 0x640, 0x600, 0x5C0, 0x580, 0x540, 0x500 },
    { 0x800, 0x7C0, 0x780, 0x740, 0x700, 0x6C0, 0x680, 0x640, 0x600 },
};
u16 HitScale[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};
u16 HeroLastHarm[4] = {0};
HeroInfo Heroes[4] = {0};
BossRecord BossState = {0};
EngineHooks HeroHooks = {0};
s32 RoundWinner[1] = {0};
s32 HeroesInPlay = 0;
s32 HeroesSidelined = 0;
u16 FinalHeroesTempo = 0;
s32 HeroOrders[16][4] = {0};
s16 HeroAnimHeroA = 0;
u16 HeroAnimSeqA = 0;
char *HeroAnimNameA = 0;      /* the animation the slot loads (VictorySeqs' name) */
obj_dbase *HeroAnimDbaseA = 0; /* the hero's model it loads into */
s16 HeroAnimHeroB = 0;
u16 HeroAnimSeqB = 0;
char *HeroAnimNameB = 0;
obj_dbase *HeroAnimDbaseB = 0;
AiSkill AiPlayerSkill[4] = {0};
u8 DefaultAiBotTweaks[20] = { 8, 0, 8, 0, 8, 0, 8, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0 };
u8 DefaultAiHumanTweaks[20] = { 8, 0, 8, 0, 8, 0, 8, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0 };
u8 DefaultAiSkills[20] = { 0x18, 0, 0x14, 0, 0x10, 0, 0xC, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0 };
u8 DefaultAiTeamSkills[12] = { 8, 0, 0x10, 0, 0x18, 0, 8, 0, 0x10, 0, 0x18, 0 };
u8 DefaultAiHumanRoundTweaks[24] = {
    0x10, 0, 0x18, 0, 0x20, 0, 0x10, 0, 0x16, 0, 0x1C, 0, 0x10, 0, 0x14, 0, 0x18, 0, 0x10, 0,
    0x12, 0, 0x14, 0,
};
void *AiBotTweakTable = 0;
void *AiHumanTweakTable = 0;
void *AiSkillTable = 0;
void *AiTeamSkillTable = 0;
void *AiHumanRoundTweakTable = 0;
s32 HeroesAllOut = 0;
ArenaCell ArenaGrid[400] = {0};
u16 HeroIdleSfx[6] = { 0x456, 0x457, 0x458, 0x459, 0x45A, 0x45B };
u16 HeroSquashCycle[12] = { 0xE66, 0xEFF, 0xF99, 0x1032, 0x10CC, 0x1165, 0x1200, 0x1165, 0x10CC, 0x1032, 0xF99, 0xEFF };
u16 HeroSpinScale[12] = { 0x1000, 0xF66, 0xECC, 0xE33, 0xD99, 0xD00, 0xC66, 0xD00, 0xD99, 0xE33, 0xECC, 0xF66 };
s16 ThrowArcs[9][3] = {
    { 0x5000, 0x5000, 0x5000 }, { 0x4000, 0x2000, 0x4000 }, { 0x1800, 0x1000, 0x1800 },
    { 0x800, 0xA00, 0x800 }, { 0x200, 0x200, 0x200 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
    { 0, 0, 0 },
};
HeroActionEntry HeroActionTable[124] = {
    { SetHeroIdle, HeroIdle }, { SetHeroQuiet, HeroQuiet },
    { SetHeroTaunt, HeroTaunt }, { SetHeroRun, HeroRun },
    { SetHeroJump, HeroJump }, { SetHeroFall, HeroFall },
    { SetHeroDrop, HeroDrop }, { SetHeroLand, HeroLand },
    { SetHeroSpin, HeroSpin }, { SetHeroWhip, HeroWhip },
    { SetHeroShoot, HeroShoot }, { SetHeroKick, HeroKick },
    { &SetBashHeroLoser, BashHeroLoser }, { &SetBashHeroHit, BashHeroHit },
    { &SetBashHeroPickup, BashHeroPickup }, { &SetBashHeroHold, BashHeroHold },
    { &SetBashHeroCarry, BashHeroCarry }, { &SetBashHeroThrow, BashHeroThrow },
    { &SetBashHeroBigWeight, BashHeroBigWeight },
    { &SetBashHeroDead, BashHeroDead }, { &SetBashHeroWinner, BashHeroWinner },
    { &SetPAHeroHit, PAHeroHit }, { &SetPAHeroPickup, PAHeroPickup },
    { &SetPAHeroHold, PAHeroHold }, { &SetPAHeroCarry, PAHeroCarry },
    { &SetPAHeroThrow, PAHeroThrow }, { &SetPAHeroDead, PAHeroDead },
    { &SetPAHeroWinner, PAHeroWinner }, { &SetArkHeroIdle, ArkHeroIdle },
    { &SetArkHeroMove, ArkHeroMove }, { &SetArkHeroTaunt, ArkHeroTaunt },
    { &SetArkHeroKick, ArkHeroKick }, { &SetArkHeroRedKick, ArkHeroRedKick },
    { &SetArkHeroGrab, ArkHeroGrab }, { &SetArkHeroWinner, ArkHeroWinner },
    { &SetArkHeroLose, ArkHeroLose }, { &SetArkHeroDie, ArkHeroDie },
    { &SetArkHeroDead, ArkHeroDead }, { &SetN2OHeroEnter, N2OHeroEnter },
    { &SetN2OHeroIdle, N2OHeroIdle }, { &SetN2OHeroMove, N2OHeroMove },
    { &SetN2OHeroTaunt, N2OHeroTaunt }, { &SetN2OHeroHit, N2OHeroHit },
    { &SetN2OHeroKick, N2OHeroKick }, { &SetN2OHeroWinner, N2OHeroWinner },
    { &SetN2OHeroDie, N2OHeroDie }, { &SetN2OHeroDead, N2OHeroDead },
    { &SetNoxHeroFly, NoxHeroFly }, { &SetNoxHeroDead, NoxHeroDead },
    { &SetNoxHeroHit, NoxHeroHit }, { &SetNoxHeroWinner, NoxHeroWinner },
    { &SetSpinHeroHit, SpinHeroHit }, { &SetSpinHeroFlip, SpinHeroFlip },
    { &SetSpinHeroFall, SpinHeroFall }, { &SetSpinHeroWinner, SpinHeroWinner },
    { &SetSpinHeroLose, SpinHeroLose }, { &SetSpinHeroDead, SpinHeroDead },
    { &SetHeroPogoIdle, HeroPogoIdle }, { &SetHeroPogo, HeroPogo },
    { &SetHeroPogoDazed, HeroPogoDazed }, { &SetHeroPogoWinner, HeroPogoWinner },
    { &SetHeroPogoLoser, HeroPogoLoser }, { &SetHeroPogoTied, HeroPogoTied },
    { &SetHeroPogoTazing, HeroPogoTazing }, { &SetBonkHeroIdle, BonkHeroIdle },
    { &SetBonkHeroRun, BonkHeroRun }, { &SetBonkHeroHit, BonkHeroHit },
    { &SetBonkHeroBackswing, BonkHeroBackswing },
    { &SetBonkHeroExtBackswing, BonkHeroExtBackswing },
    { &SetBonkHeroSwing, BonkHeroSwing },
    { &SetBonkHeroSwingRecoil, BonkHeroSwingRecoil },
    { &SetBonkHeroWinner, BonkHeroWinner }, { &SetBonkHeroLoser, BonkHeroLoser },
    { &SetPolarHeroIdle, PolarHeroIdle }, { &SetPolarHeroRun, PolarHeroRun },
    { &SetPolarHeroDazed, PolarHeroDazed },
    { &SetPolarHeroTransmute, PolarHeroTransmute },
    { &SetPolarHeroWeight, PolarHeroWeight },
    { &SetPolarHeroOffice, PolarHeroOffice },
    { &SetPolarHeroInBeam, PolarHeroInBeam }, { &SetPolarHeroPoll, PolarHeroPoll },
    { &SetPolarHeroAttack, PolarHeroAttack },
    { &SetPolarHeroSnowball, PolarHeroSnowball },
    { &SetPolarHeroThrowBomb, PolarHeroThrowBomb },
    { &SetPolarHeroFootIdle, PolarHeroFootIdle },
    { &SetPolarHeroFootRun, PolarHeroFootRun }, { &SetPolarHeroHit, PolarHeroHit },
    { &SetPolarHeroDead, PolarHeroDead }, { &SetPolarHeroWinner, PolarHeroWinner },
    { &SetPolarHeroSquashed, PolarHeroSquashed },
    { &SetPolarHeroChallenge, PolarHeroChallenge },
    { &SetRaceHeroIdle, RaceHeroIdle }, { &SetRaceHeroRun, RaceHeroRun },
    { &SetRaceHeroDazed, RaceHeroDazed }, { &SetRaceHeroJump, RaceHeroJump },
    { &SetRaceHeroKick, RaceHeroKick }, { &SetRaceHeroOiled, RaceHeroOiled },
    { &SetRaceHeroOffTrack, RaceHeroOffTrack },
    { &SetRaceHeroWinner, RaceHeroWinner }, { &SetDinoHeroIdle, DinoHeroIdle },
    { &SetDinoHeroRun, DinoHeroRun }, { &SetDinoHeroShoot, DinoHeroShoot },
    { &SetDinoHeroSlide, DinoHeroSlide }, { &SetDinoHeroDazed, DinoHeroDazed },
    { &SetDinoHeroWinner, DinoHeroWinner }, { &SetTankHeroRecoil, TankHeroRecoil },
    { &SetTankHeroDie, TankHeroDie }, { &SetTankHeroDead, TankHeroDead },
    { &SetTankHeroIdle, TankHeroIdle }, { &SetTankHeroMinelay, TankHeroMinelay },
    { &SetTankHeroMove, TankHeroMove }, { &SetTankHeroTurn, TankHeroTurn },
    { &SetTankHeroVictory, TankHeroVictory }, { &SetChaseHeroIdle, ChaseHeroIdle },
    { &SetChaseHeroRun, ChaseHeroRun }, { &SetChaseHeroHit, ChaseHeroHit },
    { &SetChaseHeroLose, ChaseHeroLose }, { &SetChaseHeroWinner, ChaseHeroWinner },
    { &SetChaseHeroDie, ChaseHeroDie }, { &SetChaseHeroDead, ChaseHeroDead },
    { &SetChaseHeroJump, ChaseHeroJump }, { &SetChaseHeroDrop, ChaseHeroDrop },
    { &SetChaseHeroFall, ChaseHeroFall }, { &SetChaseHeroLand, ChaseHeroLand },
};
/* by hero name: the sfx a hit plays (four, one at random) */
u16 HeroHitSfx[10][4] = {
    { 0, 0, 0, 0 }, { 0x3, 0x4, 0x5, 0x6 }, { 0x35, 0x36, 0x37, 0x38 }, { 0x67, 0x68, 0x69, 0x6A }, { 0x99, 0x9A, 0x9B, 0x9C }, { 0x12F, 0x130, 0x131, 0x132 }, { 0x161, 0x162, 0x163, 0x164 }, { 0xFD, 0xFE, 0xFF, 0x100 }, { 0xCB, 0xCC, 0xCD, 0xCE }, { 0, 0, 0, 0 },
};
s16 WinSeqTimer = 0;
s16 WinSeqStep = 0;
s16 WinSeqHero = 0;
s16 WinSeqDelay = 0;
s16 HeroActHit = 0;
s16 HeroActPickup = 0;
s16 HeroActHold = 0;
s16 HeroActCarry = 0;
s16 HeroActThrow = 0;
s16 HeroActDead = 0;
s16 HeroActWinner = 0;
s16 HeroActLoser = 0;
AngelSparkle AngelSparkles[3][4] = {0};
s16 AngelUnused[2] = {0};
s32 ScriptFxOn = 1;
s32 SmokePoofScript[17] = {
    0x201B, 0x1201B, 0x201D, 0x201E, 0x201F, 0x2020, 0x2021, 0x2022, 0x2023, 0x2024, 0x2025,
    0x2026, 0x2027, 0x2028, 0x2029, 0x202A, 0x8003,
};
s32 SmokePoofScripts[3] = { (s32)&SmokePoofScript, 0, 0 };
ScriptInit SmokePoofInit = { 0, (void *)&SmokePoofScripts, 0x402 };
s32 PolarTiltX = 0;
s32 PolarTiltZ = 0;
s32 PolarTiltUnused = 0;
FxDef WeatherFxDef = { 0x100, 0x2800, 0x1800, PlotSparks, PlotFallParticles };
u16 HealthPickupSfx[8] = { 0x1C5, 0x1C6, 0x1C7, 0x1C8, 0x1C9, 0x1CA, 0x1CB, 0x1CC };
u16 HealthPickupAmounts[4] = { 1, 1, 2, 2 };
s32 BigPoofScript[9] = {
    0x20260000, 0x20282027, 0x202A2029, 0x202C202B, 0x202E202D, 0x2030202F, 0x20322031,
    0x20342033, (s32)0x80042035,
};
ScriptInit BigPoofInit = { (s32)&BigPoofScript, 0, 0x401 };
s32 PickupCounts[15] = {0};
s32 CratePickupAst = 0;
s32 HealthAst = 0;
s32 HitTextCodes[5] = { 0x20112010, 0x20122012, 0x20122012, 0x20122012, 0x2012 };
s32 PlayerHitSfx[4] = { 0x442, 0x443, 0x444, 0x445 };
s32 HitShape0[6] = { 0x800060, 0x20000, 0x500020, 0x60000, 0xFFFF, 0 };
s32 HitShape1[6] = { 0x800090, 0x50000, 0x500000, 6, 0, 0 };
s32 HitShape2[6] = { 0x900090, 0x50000, 0x500012, 0x80000, 0xFFFF, 0 };
s32 HitShape3[6] = { 0x800070, 0x50000, 0x500000, 7, 0, 0 };
s32 HitShape4[6] = { 0x800080, 0x50000, 0x500000, 7, 0, 0 };
s32 HitShape5[6] = { 0x800071, 0x60000, 0x500020, 0x90000, 0xFFFF, 0 };
s32 HitShape6[6] = { 0x800080, 0x50000, 0x500028, 0x90000, 0xFFFF, 0 };
s32 HitShape7[6] = { 0x800060, 0x30000, 0, 0, 0xFFFF, 0 };
s32 HitShape8[6] = { 0x800070, 0x40000, 0, 0, 0xFFFF, 0 };
s32 HitShape9[6] = { 0x800050, 0x30000, 0, 0, 0xFFFF, 0 };
s32 *CrateHitShapes[10] = { HitShape0, HitShape5, HitShape6, HitShape2, HitShape2, HitShape1, HitShape1, HitShape3, HitShape4, HitShape0 };
s32 *CrateHitShapes2[10] = { HitShape7, HitShape8, HitShape8, HitShape9, HitShape9, HitShape9, HitShape9, HitShape8, HitShape8, HitShape7 };
s32 HitPushDirs[8] = { 0x1FFFF, 0x4000000, -1, 0x4000800, -0xFFFF, 0xC000800, 0x10001, 0xC000000 };
