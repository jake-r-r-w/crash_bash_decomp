/* The Adventure test's player for the Bash games: the computer's hands, which go after crates and heroes well enough, but walk into
 * what's about to go off. So whenever something is about to hurt him he gets out of its way first. In Drain Bash's crystal challenge a question
 * mark crate kills whoever hits it, so there he plays himself: picks up the nearest plain crate and throws it at the nearest hero. In Space Bash the computer's hands don't
 * go for heroes that stand still, so he does that there as well. */
#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/hero.h"
#include "bash/bash_ovl.h"

#define TNT_LIT 7
#define NITRO 6
#define QUESTION_MARK 2
#define QUESTION_REACH 0x280
#define FLAG_CRYSTAL 0x80
#define BTN_CIRCLE 0x2000
#define BTN_SQUARE 0x8000
#define SPACE_BASH 1
#define DRAIN_BASH 3
#define GRAB_REACH 0x180
#define THROW_REACH 0x900
#define PLAIN_CRATE_FIRST 3
#define PLAIN_CRATE_LAST 4
#define CRATE_FLYING 8
#define BLAST_REACH 0x340
#define NITRO_REACH 0x200
#define FLIGHT_REACH 0x280
#define SPIN_REACH 0x260
#define ATTACK_FIRST 8
#define ATTACK_LAST 11
#define FLIGHT_LOOK 0x1400
#define FLIGHT_WIDE 0x240

static s32 distance(s32 dx, s32 dz) {
    return csqrt(dx * dx + dz * dz) >> 6;
}

/* Where to get away from: fills it in and says if there's anything to get away from, the nearest of what's dangerous. */
static s32 bash_danger(const EXTengine *he, s32 *from_x, s32 *from_z) {
    s32 best = 0x7FFFFFFF, i;
    NAS *n;

    for (n = HeadOfObjects; n != NULL; n = n->Object.Child) {
        EXTobject *f = (EXTobject *)n->FUX;
        EXTengine *e = n->Engine;
        s32 x, z, d;

        if (f == NULL || e == NULL || Heroes[0].hero_object == f) {
            continue;
        }
        x = e->XYZ.x;
        z = e->XYZ.z;
        if (f->status == CRATE_FLYING) {
            /* he's in its way if he's on the line it's flying along: off to the side he's on already */
            s32 vy = e->xform.rot.vy;
            s32 fx = -rcossin_tbl[(vy - 0x400) & 0xFFF].unk2, fz = -rcossin_tbl[vy & 0xFFF].unk2;
            s32 rx = he->XYZ.x - x, rz = he->XYZ.z - z;
            s32 along = (rx * fx + rz * fz) >> 12;
            s32 side = (rx * fz - rz * fx) >> 12;

            if (along < -FLIGHT_REACH || along > FLIGHT_LOOK || abs(side) > FLIGHT_WIDE) {
                continue;
            }
            d = along > 0 ? along / 4 : 0;
            x = he->XYZ.x - (side >= 0 ? fz : -fz);
            z = he->XYZ.z + (side >= 0 ? fx : -fx);
        } else if (f->whatami == TNT_LIT) {
            d = distance(he->XYZ.x - x, he->XYZ.z - z);
            if (d >= BLAST_REACH) {
                continue;
            }
        } else if (f->whatami == QUESTION_MARK && GameMode == DRAIN_BASH && (GAME->levelFlags & FLAG_CRYSTAL)) {
            d = distance(he->XYZ.x - x, he->XYZ.z - z);
            if (d >= QUESTION_REACH) {
                continue;
            }
        } else if (f->whatami == NITRO) {
            d = distance(he->XYZ.x - x, he->XYZ.z - z);
            if (d >= NITRO_REACH) {
                continue;
            }
        } else {
            continue;
        }
        if (d < best) {
            best = d;
            *from_x = x;
            *from_z = z;
        }
    }
    for (i = 1; i < 4; i++) {
        EXTengine *e = Heroes[i].hero_eng;
        s32 d;

        if (e == NULL || (Heroes[i].hero_status & 0xFA00) || Heroes[i].hero_mode < ATTACK_FIRST || Heroes[i].hero_mode > ATTACK_LAST) {
            continue;
        }
        d = distance(he->XYZ.x - e->XYZ.x, he->XYZ.z - e->XYZ.z);
        if (d < SPIN_REACH && d < best) {
            best = d;
            *from_x = e->XYZ.x;
            *from_z = e->XYZ.z;
        }
    }
    return best != 0x7FFFFFFF;
}

void BashPlayerBot(s32 id) {
    EXTengine *he;
    s32 x, z;

    Bash_HeroBot(id);
    if (id != 0 || (GAME->pause & 0x04000000)) {
        return;
    }
    he = Heroes[0].hero_eng;
    if (bash_danger(he, &x, &z)) {
        s32 ang = get_angle(he->XYZ.x - x, he->XYZ.z - z);

        /* not into the wall behind him */
        if (abs(he->XYZ.x) > 0x900 || abs(he->XYZ.z) > 0x900) {
            s32 toward_middle = get_angle(-he->XYZ.x, -he->XYZ.z);

            if (abs((((ang - toward_middle) + 0x800) & 0xFFF) - 0x800) > 0x400) {
                ang = toward_middle;
            }
        }
        set_joypad(&Pads[0], DirPadBits[(ang >> 9) & 7]);
        Pads[0].angle.w = ang;
    }
    else if ((GameMode == DRAIN_BASH && (GAME->levelFlags & FLAG_CRYSTAL)) || (GameMode == SPACE_BASH && Heroes[1].hero_frozen != 0)) {
        s32 i, best = 0x7FFFFFFF, ang = 0, pad = 0;
        NAS *n;

        if (Heroes[0].hero_object != NULL) {
            for (i = 1; i < 4; i++) {
                EXTengine *e = Heroes[i].hero_eng;

                if (e != NULL && !(Heroes[i].hero_status & 0xFA00)) {
                    s32 d = distance(e->XYZ.x - he->XYZ.x, e->XYZ.z - he->XYZ.z);

                    if (d < best) {
                        best = d;
                        ang = get_angle(e->XYZ.x - he->XYZ.x, e->XYZ.z - he->XYZ.z);
                    }
                }
            }
            pad = best < THROW_REACH && (GameFrameCount & 15) == 0 ? BTN_CIRCLE : 0;
        } else {
            for (n = HeadOfObjects; n != NULL; n = n->Object.Child) {
                EXTobject *f = (EXTobject *)n->FUX;

                if (f != NULL && n->Engine != NULL && f->status == 1 && f->whatami >= PLAIN_CRATE_FIRST && f->whatami <= PLAIN_CRATE_LAST) {
                    s32 d = distance(n->Engine->XYZ.x - he->XYZ.x, n->Engine->XYZ.z - he->XYZ.z);

                    if (d < best) {
                        best = d;
                        ang = get_angle(n->Engine->XYZ.x - he->XYZ.x, n->Engine->XYZ.z - he->XYZ.z);
                    }
                }
            }
            pad = best < GRAB_REACH && (GameFrameCount & 7) == 0 ? BTN_CIRCLE : 0;
        }
        if (best != 0x7FFFFFFF) {
            set_joypad(&Pads[0], DirPadBits[(ang >> 9) & 7] | pad);
            Pads[0].angle.w = ang;
        }
    }
}
