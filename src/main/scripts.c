#include "common.h"
#include "bash/main.h"
#include "bash/prims.h"
#include "bash/scripts.h"

void init_scripts(s32 arg0, s32 arg1) {
    u8 *p;
    u8 *next;

    if (arg0 != 0) {
        ScriptPool = (s32)mal_alloc(arg0 * 0x7C, &MainHeap);
        p = (u8 *)ScriptPool;
        for (; arg0 >= 2; arg0--) {
            next = p + 0x7C;
            *(u8 **)(p + 0x74) = next;
            p = next;
        }
        *(s32 *)(p + 0x74) = 0;
    } else {
        ScriptPool = 0;
    }
    active_scripts = NULL;
    free_scripts = (void *)ScriptPool;
    if (arg1 != 0) {
        ScriptThreadPool = (s32)mal_alloc(arg1 * 0xB8, &MainHeap);
        p = (u8 *)ScriptThreadPool;
        for (; arg1 >= 2; arg1--) {
            next = p + 0xB8;
            *(u8 **)(p + 0xB4) = next;
            p = next;
        }
        *(s32 *)(p + 0xB4) = 0;
    } else {
        ScriptThreadPool = 0;
    }
    ScriptTimeStep = 0x10000;
    ScriptList = (void *)ScriptThreadPool;
}

void update_model_thread(script_thread_handler *th, script_control_info *ci) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    u8 *track;
    ObjKey *key;
    ObjKey *k;
    subengine *d;
    s32 len;
    s32 t;
    s32 w;
    s32 f;

    e = (u8 *)ci->dbase;
    lo = ci->script;
    idx = th->thread_num;
    v = ci->time.h.frame;
    ScriptTrackDbase = e;
    p = ((obj_dbase *)e)->data;
    ScriptTrackIndex = lo;
    ScriptTrackThread = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    track = res;
    if (track != NULL) {
        k = ObjTrack_FindKey((ObjKey *)(track + 0x24), ci->time.h.frame);
        key = k;
        d = &th->type.anim.sub;
        f = key->len;
        d->unk51 = 3;
        if (f != 0) {
            len = f << 8;
            t = (ci->time.w >> 8) - (key->start << 8);
            CamVec3_Lerp((CamVec3 *)&d->unk04, &key->pos, &key[1].pos, len, t);
            quat_slerp_ex_frac((s32)&key->rot, (s32)&key[1].rot, (s32)&d->unk10, len, t, (s32)key->unk34);
            d->unk52 = 1;
            CamVec3_Lerp((CamVec3 *)&d->unk20, &key->scale, &key[1].scale, len, t);
            w = interp(key->unk4C, key[1].unk4C, len, t);
        } else {
            *(CamVec3 *)&d->unk04 = key->pos;
            d->unk52 = 1;
            *(ObjQuat *)&d->unk10 = key->rot;
            d->unk20.x = key->scale.v[0];
            d->unk20.y = key->scale.v[1];
            d->unk20.z = key->scale.v[2];
            w = key->unk4C;
        }
        w = 0x1000 - w;
        d->flags |= *(s32 *)(track + 0x18) | 0x8000;
        if (w != 0) {
            d->unk76 = w;
            d->flags |= 0x40000000;
        }
        d->unk50 = 1;
        d->entity = key->unk8;
    } else {
        th->type.anim.sub.flags &= ~0x8000;
    }
}

void update_camera_thread(script_thread_handler *th, script_control_info *ci) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    obj_dbase *e;
    CamKey *key;
    CamState *cam;
    s32 len;
    s32 t;
    CamVec3 eye;
    CamVec3 at;
    u8 *track;

    e = ci->dbase;
    lo = ci->script;
    idx = th->thread_num;
    v = ci->time.h.frame;
    ScriptTrackDbase = (u8 *)e;
    p = e->data;
    ScriptTrackIndex = lo;
    ScriptTrackThread = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    track = res;
    if (track != NULL) {
        key = CamTrack_FindKey(track, ci->time.h.frame);
        cam = (CamState *)MainCamera;
        cam->unk8 = 0x800;
        cam->unk0 = 0;
        cam->unk4 = 0;
        cam->pos = key->at;
        cam->unk18 = *(s32 *)(track + 0x18);
        if (key->len != 0) {
            len = key->len << 8;
            t = (ci->time.w >> 8) - (key->start << 8);
            CamVec3_Lerp(&eye, &key->eye, &key[1].eye, len, t);
            CamVec3_Lerp(&at, &key->at, &key[1].at, len, t);
        } else {
            eye = key->eye;
            at = key->at;
        }
        get_dist_angles((s32 *)&eye, (s32 *)&at, cam->rot);
        cam->pos = eye;
        cam->unk8 = 0;
        cam->unk0 = 0;
        cam->unk4 = 0;
    }
}

void update_entity_thread(script_thread_handler *th, script_control_info *ci) {
    AnimKey *k;
    AnimKey *key;
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    AnimRange *range;
    AnimPose *pose;
    s32 f;
    s32 frame;
    s32 len;
    s32 t;
    s32 a;

    e = (u8 *)ci->dbase;
    lo = ci->script;
    idx = th->thread_num;
    v = ci->time.h.frame;
    ScriptTrackDbase = e;
    p = ((obj_dbase *)e)->data;
    ScriptTrackIndex = lo;
    ScriptTrackThread = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    pose = (AnimPose *)&th->type.entity.sub;
    if (res != NULL) {
        range = (AnimRange *)(res + 0x14);
        k = AnimTrack_FindKey((AnimKey *)(res + 0x30), ci->time.h.frame);
        key = k;
        f = ci->time.w - (*(s32 *)(res + 4) << 16);
        frame = AnimRange_Frame(range, f);
        pose->frame = frame;
        pose->unk74 = range->base + range->lo;
        len = key->len;
        if (len != 0) {
            len <<= 8;
            t = (ci->time.w >> 8) - (key->start << 8);
            quat_slerp_ex_frac((s32)key->rot, (s32)key[1].rot, (s32)pose->rot, len, t, (s32)key->unk30);
            CamVec3_Lerp(&pose->pos, &key->pos, &key[1].pos, len, t);
            CamVec3_Lerp(&pose->scale, &key->scale, &key[1].scale, len, t);
            a = interp(key->alpha, key[1].alpha, len << 8, t);
        } else {
            pose->pos = key->pos;
            *(AnimRot *)pose->rot = *(AnimRot *)key->rot;
            pose->scale.v[0] = key->scale.v[0];
            pose->scale.v[1] = key->scale.v[1];
            pose->scale.v[2] = key->scale.v[2];
            a = key->alpha;
        }
        a = 0x1000 - a;
        if (a != 0) {
            pose->unk76 = a;
            pose->flags |= 0x40000000;
        }
        pose->unk50 = 1;
        pose->unk51 = 3;
        pose->unk52 = 1;
        pose->flags |= 0x80008000;
    } else {
        pose->flags = 0;
    }
}

void update_fade_thread(script_thread_handler *th, script_control_info *ci) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;

    e = (u8 *)ci->dbase;
    lo = ci->script;
    idx = th->thread_num;
    v = ci->time.h.frame;
    ScriptTrackDbase = e;
    p = ((obj_dbase *)e)->data;
    ScriptTrackIndex = lo;
    ScriptTrackThread = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
        if (res != NULL) {
            ScriptFade = interp(th->type.fade.start, th->type.fade.end,
                                (*(s32 *)(res + 8) - *(s32 *)(res + 4)) << 8,
                                (ci->time.w >> 8) - (*(s32 *)(res + 4) << 8));
        }
    }
}

void emit_particle(u8 *obj, EmitPart *part, u8 *def) {
    s32 speed;
    s32 n;
    s32 yaw;
    s32 pitch;
    s32 radius;
    s32 r;

    yaw = *(s32 *)(def + 0x30);
    pitch = *(s32 *)(def + 0x38);
    radius = *(s32 *)(def + 0x2C);
    n = *(s32 *)(def + 0x24) - *(s32 *)(def + 0x20);
    if (n != 0) {
        speed = *(s32 *)(def + 0x20) + Random(n);
    } else {
        speed = *(s32 *)(def + 0x20);
    }
    part->age = 0;
    if (*(s32 *)(def + 0x68) != 0) {
        *(s16 *)(obj + 0x14) = Random(0x1000);
    }
    if (*(s32 *)(def + 0x34) < *(s32 *)(def + 0x30)) {
        r = abs((*(s32 *)(def + 0x34) - *(s32 *)(def + 0x30)) << 20 >> 20);
    } else {
        r = *(s32 *)(def + 0x34) - *(s32 *)(def + 0x30);
    }
    if (r != 0) {
        yaw = (*(s32 *)(def + 0x30) + Random(r)) & 0xFFF;
    }
    if (*(s32 *)(def + 0x3C) < *(s32 *)(def + 0x38)) {
        r = abs((*(s32 *)(def + 0x3C) - *(s32 *)(def + 0x38)) << 20 >> 20);
    } else {
        r = *(s32 *)(def + 0x3C) - *(s32 *)(def + 0x38);
    }
    if (r != 0) {
        pitch = (*(s32 *)(def + 0x38) + Random(r)) & 0xFFF;
    }
    part->vel[0] = speed * rcossin_tbl[(yaw - 0x400) & 0xFFF].unk2 >> 4;
    part->vel[2] = speed * rcossin_tbl[yaw & 0xFFF].unk2 >> 4;
    part->vel[1] = -(speed * rcossin_tbl[pitch & 0xFFF].unk2 >> 4);
    *(s32 *)(obj + 4) = (radius * rcossin_tbl[(yaw - 0x400) & 0xFFF].unk2 >> 12) + *(s32 *)(def + 4);
    *(s32 *)(obj + 0xC) = (radius * rcossin_tbl[yaw & 0xFFF].unk2 >> 12) + *(s32 *)(def + 0xC);
    *(s32 *)(obj + 8) = *(s32 *)(def + 8) - (radius * rcossin_tbl[pitch & 0xFFF].unk2 >> 12);
    *(s32 *)(obj + 0) |= 0x8000;
}

void update_emitter_thread(script_thread_handler *th, script_control_info *ci) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    u8 *e;
    subengine *obj;
    EmitPart *part;
    u8 *def;
    s32 spawn;
    s32 a;
    s32 scale;
    s32 in;

    obj = th->type.particle.subs;
    part = (EmitPart *)th->type.particle.mdata;
    e = (u8 *)ci->dbase;
    lo = ci->script;
    idx = th->thread_num;
    ScriptTrackDbase = e;
    p = ((obj_dbase *)e)->data;
    ScriptTrackIndex = lo;
    ScriptTrackThread = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res == NULL) {
        return;
    }
    def = res + 0x14;
    if (ci->time.h.frame == *(s32 *)(res + 4)) {
        th->type.particle.first = *(s32 *)(def + 0x10);
    }
    if (*(s32 *)(res + 4) >= ci->time.h.frame) {
        return;
    }
    if (th->type.particle.first != 0 || (spawn = 0, *(s32 *)(def + 0x6C) != 0)) {
        spawn = *(s32 *)(def + 0x14);
    }
    if (*(s32 *)(def + 0x1C) < ci->time.h.frame) {
        spawn = 0;
    }
    th->type.particle.first -= spawn;
    if (th->type.particle.first < 0) {
        th->type.particle.first = 0;
    }
    for (;; obj = obj->next, part++) {
        if (obj == NULL) {
            return;
        }
        if (obj->flags & 0x8000) {
            if (part->vel[0] != 0) {
                obj->unk04 += part->vel[0] >> 8;
            }
            if (part->vel[1] != 0) {
                obj->unk08 += part->vel[1] >> 8;
            }
            if (part->vel[2] != 0) {
                obj->unk0C += part->vel[2] >> 8;
            }
            if (*(s32 *)(def + 0x40) != 0) {
                part->vel[0] += *(s32 *)(def + 0x40);
            }
            if (*(s32 *)(def + 0x48) != 0) {
                part->vel[1] += *(s32 *)(def + 0x48);
            }
            if (*(s32 *)(def + 0x50) != 0) {
                part->vel[2] += *(s32 *)(def + 0x50);
            }
            if (*(s32 *)(def + 0x44) != 0x100) {
                part->vel[0] = (part->vel[0] * *(s32 *)(def + 0x44)) >> 8;
            }
            if (*(s32 *)(def + 0x4C) != 0x100) {
                part->vel[1] = (part->vel[1] * *(s32 *)(def + 0x4C)) >> 8;
            }
            if (*(s32 *)(def + 0x54) != 0x100) {
                part->vel[2] = (part->vel[2] * *(s32 *)(def + 0x54)) >> 8;
            }
            obj->unk14 += *(u16 *)(def + 0x68);
            obj->unk52 = 1;
            if (*(s32 *)(def + 0x18) < part->age++) {
                obj->flags &= ~0x8000;
            }
            if (obj->flags & 0x8000) {
                in = *(s32 *)(def + 0x58);
                a = 0;
                if (in != 0 || *(s32 *)(def + 0x5C) != 0) {
                    a = EmitPart_Fade(in, *(s32 *)(def + 0x5C), part->age, *(s32 *)(def + 0x18), 0x1000, 0);
                }
                if (a != 0) {
                    obj->flags |= 0x40000000;
                } else {
                    obj->flags &= ~0x40000000;
                }
                obj->unk76 = a;
                in = *(s32 *)(def + 0x60);
                scale = 0x1000;
                if (in != 0 || *(s32 *)(def + 0x64) != 0) {
                    scale = EmitPart_Fade(in, *(s32 *)(def + 0x64), part->age, *(s32 *)(def + 0x18), 0, 0x1000);
                }
                obj->unk20.z = scale;
                obj->unk20.y = scale;
                obj->unk20.x = scale;
                obj->unk50 = 1;
                obj->unk52 = 1;
            }
        }
        if (spawn != 0 && !(obj->flags & 0x8000)) {
            obj->flags |= 0x8000;
            emit_particle((u8 *)obj, part, def);
            spawn--;
        }
    }
}

void update_sub_thread(script_thread_handler *th, script_control_info *ci) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    subengine *d;
    u32 t;

    e = (u8 *)ci->dbase;
    lo = ci->script;
    idx = th->thread_num;
    v = ci->time.h.frame;
    ScriptTrackDbase = e;
    p = ((obj_dbase *)e)->data;
    ScriptTrackIndex = lo;
    ScriptTrackThread = idx;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (idx * 4 + 0x1C) + *(s32 *)(r + (idx << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    d = &th->type.subscript.sub;
    if (res != NULL && ci->time.w >= (*(s32 *)(res + 4) << 16) &&
        !(th->type.subscript.sub.flags & 0x8000)) {
        d->unk04 = *(s32 *)(res + 0x2C);
        d->unk08 = *(s32 *)(res + 0x30);
        d->unk0C = *(s32 *)(res + 0x34);
        d->unk10 = *(u16 *)(res + 0x38);
        d->unk12 = *(u16 *)(res + 0x3C);
        d->unk14 = *(u16 *)(res + 0x40);
        d->unk20.x = *(s32 *)(res + 0x60);
        d->unk20.y = *(s32 *)(res + 0x64);
        d->unk20.z = *(s32 *)(res + 0x68);
        d->unk51 = 0;
        t = *(u16 *)(res + 0x18);
        d->unk52 = 1;
        d->unk68 = t;
        sub_play_script(ci->dbase, *(s32 *)(res + 0x14), d, 0);
        th->type.subscript.sub.flags |= 0x8000;
    }
}

script_handler *play_script(obj_dbase *arg0, s32 arg1, Blk32 *arg2, CamVec3 *arg3, s32 arg4,
                            script_handler **arg5) {
    script_handler *p;
    u8 *e;
    u8 *hdr;
    s32 *t;
    s32 *q;
    s32 i;
    script_handler *prev;
    script_handler *n;
    void (*fn)(s32 *, script_control_info *, s32);

    if (arg0->data == NULL) {
        return NULL;
    }
    n = free_scripts;
    if (n != NULL) {
        free_scripts = n->next;
        if (active_scripts != NULL) {
            active_scripts->last = n;
        }
        prev = active_scripts;
        n->last = NULL;
        active_scripts = n;
        n->next = prev;
    }
    p = n;
    if (p != NULL) {
        *(CamVec3 *)&p->xyzrf.x = *arg3;
        *(Blk32 *)&p->xyzrf.rot = *arg2;
        p->xyzrf.flags = 0x8000;
        p->xyzrf.unk51 = 4;
        p->xyzrf.unk50 = 0;
        p->xyzrf.unk52 = 0;
        e = arg0->data;
        hdr = NULL;
        if (e != NULL) {
            s32 *r = (s32 *)(e + (*(s32 *)(e + 0x4C) + 0x4C)) + arg1;
            hdr = (u8 *)r + *r;
        }
        p->info.flags = arg4;
        p->info.dbase = arg0;
        p->info.script = arg1;
        p->info.threads = NULL;
        p->info.time.w = 0;
        p->info.time.h.frame = *(s16 *)(hdr + 0xC);
        p->info.unk10 = *(s32 *)(hdr + 0x8);
        p->info.handle = (s32 *)arg5;
        if (arg5 != NULL) {
            *arg5 = p;
        }
        t = (s32 *)(hdr + 0x1C);
        for (i = 0; i < *(s32 *)hdr; i++) {
            q = (s32 *)((u8 *)t + *t);
            fn = ScriptCommands[*q].init;
            if (fn != NULL) {
                fn(q, &p->info, i);
            }
            t++;
        }
        update_script(&p->info);
    }
    return p;
}

s32 update_script(script_control_info *ci) {
    script_thread_handler *s;
    s32 *a;
    u8 *c;
    void (*fn)(script_thread_handler *, script_control_info *);
    s32 idx;

    s = ci->threads;
    ScriptFade = 0;
    while (s != NULL) {
        fn = ScriptCommands[s->thread_type].update;
        if (fn != NULL) {
            fn(s, ci);
        }
        s = s->next;
    }
    ci->time.w = ci->time.w + ScriptTimeStep;
    c = ci->dbase->data;
    idx = ci->script;
    if (c == NULL) {
        a = NULL;
    } else {
        s32 *t = (s32 *)(c + (*(s32 *)(c + 0x4C) + 0x4C) + idx * 4);
        a = (s32 *)((u8 *)t + *t);
    }
    if (ci->time.h.frame >= a[2]) {
        ci->flags |= 0x10;
        if (ci->flags & 8) {
            ci->time.w = (a[2] - 1) << 16;
        }
        if (ci->flags & 2) {
            ci->time.h.frame = *(u16 *)(a + 3);
        }
        if (ci->flags & 4) {
            kill_script_threads(ci);
            return 0;
        }
    }
    return 1;
}

void render_script(script_control_info *ci, GteSetup *arg1) {
    MATRIX m;
    GteSetup out;
    s32 dp;
    script_thread_handler *p;
    GfxState *gs;
    void (*fn)(script_thread_handler *, GteSetup *);

    dp = RenderState.modelDp;
    p = ci->threads;
    RenderState.modelDp =0x1000 - (((0x1000 - ScriptFade) * (0x1000 - dp)) >> 12);
    m = *arg1->rot;
    m.t[0] = arg1->trans->vx;
    m.t[1] = arg1->trans->vy;
    m.t[2] = arg1->trans->vz;
    out.rot = &m;
    out.trans = arg1->trans;
    out.ofx = arg1->ofx;
    out.ofy = arg1->ofy;
    for (; p != NULL;) {
        gs = &RenderState;
        ((GfxScreen *)gs->unk28)->x = arg1->ofx;
        ((GfxScreen *)gs->unk28)->y = arg1->ofy;
        fn = ScriptCommands[p->thread_type].render;
        if (fn != NULL) {
            fn(p, &out);
        }
        p = p->next;
    }
    RenderState.modelDp = dp;
}

void quat_slerp(Quat *a, Quat *b, s32 t, Quat *out) {
    Quat q;
    s32 d;
    s32 sq;
    s32 s;
    s32 ang;
    s32 inv;
    s32 ka;
    s32 kb;

    q.v[0] = b->v[0];
    q.v[1] = b->v[1];
    q.v[2] = b->v[2];
    q.v[3] = b->v[3];
    d = QuatDot(a, &q);
    if (d < 0) {
        q.v[0] = -q.v[0];
        q.v[1] = -q.v[1];
        q.v[2] = -q.v[2];
        q.v[3] = -q.v[3];
        d = QuatDot(a, &q);
    }
    sq = d * d;
    s = csqrt(abs(0x1000 - ((sq >= 0 ? sq + 0x800 : sq - 0x800) >> 12)));
    if (abs(s) < 0x80) {
        out->v[0] = a->v[0];
        out->v[1] = a->v[1];
        out->v[2] = a->v[2];
        out->v[3] = a->v[3];
        return;
    }
    ang = get_angle(s, d);
    inv = 0x1000000 / s;
    ka = (rcossin_tbl[((((0x1000 - t) * ang >> 12) & 0xFFF) - 0x400) & 0xFFF].unk2 * inv) >> 6;
    kb = (rcossin_tbl[(((t * ang >> 12) & 0xFFF) - 0x400) & 0xFFF].unk2 * inv) >> 6;
    out->v[0] = (ka * a->v[0] + kb * q.v[0]) >> 18;
    out->v[1] = (ka * a->v[1] + kb * q.v[1]) >> 18;
    out->v[2] = (ka * a->v[2] + kb * q.v[2]) >> 18;
    out->v[3] = (ka * a->v[3] + kb * q.v[3]) >> 18;
}

/* Cosine at angle x / 2, linearly interpolated for odd x. */
static inline s32 Trig_HalfCos(s32 x) {
    if (x & 1) {
        return (rcossin_tbl[((x >> 1) - 0x400) & 0xFFF].unk2 + rcossin_tbl[((x >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    }
    return rcossin_tbl[((x >> 1) - 0x400) & 0xFFF].unk2;
}

void quat_slerp_ex(Quat *a, Quat *b, s32 t, Quat *out, s32 *pre) {
    Quat q;
    s32 d;
    s32 sq;
    s32 s;
    s32 ang;
    s32 inv;
    s32 ia;
    s32 ib;
    s32 ka;
    s32 kb;
    s32 pb;

    q.v[0] = b->v[0];
    q.v[1] = b->v[1];
    q.v[2] = b->v[2];
    q.v[3] = b->v[3];
    d = QuatDot(a, &q);
    if (d < 0) {
        q.v[0] = -q.v[0];
        q.v[1] = -q.v[1];
        q.v[2] = -q.v[2];
        q.v[3] = -q.v[3];
        d = QuatDot(a, &q);
    }
    sq = d * d;
    s = csqrt(abs(0x1000 - ((sq >= 0 ? sq + 0x800 : sq - 0x800) >> 12)));
    if (abs(s) < 0x80) {
        out->v[0] = a->v[0];
        out->v[1] = a->v[1];
        out->v[2] = a->v[2];
        out->v[3] = a->v[3];
        return;
    }
    get_angle(s, d);
    ang = pre[1];
    inv = pre[2];
    ib = t * ang;
    if (ib > 0) {
        ib += 0x400;
    } else {
        ib -= 0x400;
    }
    ib >>= 11;
    ia = (0x1000 - t) * ang;
    if (ia > 0) {
        ia += 0x400;
    } else {
        ia -= 0x400;
    }
    ia >>= 11;
    ka = Trig_HalfCos(ia) * inv;
    if (ib & 1) {
        ang = (rcossin_tbl[((ib >> 1) - 0x400) & 0xFFF].unk2 + rcossin_tbl[((ib >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    } else {
        ang = rcossin_tbl[((ib >> 1) - 0x400) & 0xFFF].unk2;
    }
    ia = ang * inv;
    if (ka > 0) {
        ka += 0x10;
    } else {
        ka -= 0x10;
    }
    ka >>= 5;
    if (ia > 0) {
        ia += 0x10;
    } else {
        ia -= 0x10;
    }
    ia >>= 5;
    kb = ka * a->v[0];
    ib = ia * q.v[0];
    out->v[0] = (((kb >= 0 ? kb + 0x800 : kb - 0x800) >> 12) + ((ia * q.v[0] >= 0 ? ib + 0x800 : ib - 0x800) >> 12)) >> 7;
    kb = ka * a->v[1];
    ib = ia * q.v[1];
    out->v[1] = (((kb >= 0 ? kb + 0x800 : kb - 0x800) >> 12) + ((ia * q.v[1] >= 0 ? ib + 0x800 : ib - 0x800) >> 12)) >> 7;
    kb = ka * a->v[2];
    ib = ia * q.v[2];
    out->v[2] = (((kb >= 0 ? kb + 0x800 : kb - 0x800) >> 12) + ((ia * q.v[2] >= 0 ? ib + 0x800 : ib - 0x800) >> 12)) >> 7;
    pb = ka * a->v[3];
    kb = ia * q.v[3];
    out->v[3] = (((pb >= 0 ? pb + 0x800 : pb - 0x800) >> 12) + ((ia * q.v[3] >= 0 ? kb + 0x800 : kb - 0x800) >> 12)) >> 7;
}

void quat_slerp_ex_frac(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    quat_slerp_ex(a0, a1, (a4 << 12) / a3, a2, a5);
}

void quat_slerp_frac(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    quat_slerp(a0, a1, (a4 << 12) / a3, a2);
}

/* No callers. Retail multiplies (b - a) by the fraction but never reads lo
 * (no mflo) and returns a: the interpolating return sits behind a test that
 * is always false, which only combine sees (after flow), so the mult stays
 * and the dead block is dropped after reload. */
s32 cos_interp(s32 arg0) {
    s32 a;
    s32 b;
    s32 t;

    a = rcossin_tbl[((arg0 >> 16) - 0x400) & 0xFFF].unk2;
    b = rcossin_tbl[((arg0 >> 16) - 0x3FF) & 0xFFF].unk2;
    t = (b - a) * (arg0 & 0xFFFF);
    if (!((arg0 & 0x7FFF) >> 15)) {
        return a;
    }
    return a + (t >> 16);
}

s16 cos_half(s32 arg0) {
    if (arg0 & 1) {
        return (rcossin_tbl[((arg0 >> 1) - 0x400) & 0xFFF].unk2 + rcossin_tbl[((arg0 >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    }
    return rcossin_tbl[((arg0 >> 1) - 0x400) & 0xFFF].unk2;
}

void sub_play_script(obj_dbase *db, s32 arg1, subengine *sub, s32 arg3) {
    script_control_info *p;
    u8 *e;
    s32 *s;
    s32 *q;
    s32 i;
    void (*fn)(u8 *, script_control_info *, s32);
    u8 *base;

    p = (script_control_info *)&sub->u6C.dbase;
    base = db->data;
    if (base == NULL) {
        e = NULL;
    } else {
        s = (s32 *)(base + (*(s32 *)(base + 0x4C) + 0x4C)) + arg1;
        e = (u8 *)s + *s;
    }
    p->flags = arg3;
    p->dbase = db;
    p->script = arg1;
    p->threads = NULL;
    p->time.w = 0;
    p->time.h.frame = *(u16 *)(e + 0xC);
    q = (s32 *)(e + 0x1C);
    p->unk10 = *(s32 *)(e + 8);
    p->handle = NULL;
    i = 0;
    if (*(s32 *)e > 0) {
        do {
            u8 *a = (u8 *)q + *q;
            fn = ScriptCommands[*(s32 *)a].init;
            if (fn != NULL) {
                fn(a, p, i);
            }
            q++;
        } while (++i < *(s32 *)e);
    }
    update_script(p);
    *(void **)((u8 *)sub + 0x54) = plot_script_nas;
    *(void **)((u8 *)sub + 0x58) = kill_nas_script;
}

void kill_nas_script(subengine *sub) {
    script_control_info *p;
    script_thread_handler *cur;
    script_thread_handler *next;
    s32 *flag;
    script_thread_handler *old;
    ScriptCommand *tab;
    void (*fn)(script_thread_handler *, script_control_info *);

    p = (script_control_info *)&sub->u6C;
    flag = p->handle;
    cur = p->threads;
    if (flag != NULL) {
        *flag = 0;
    }
    tab = ScriptCommands;
    for (;;) {
        if (cur == NULL) {
            return;
        }
        fn = (void (*)(script_thread_handler *, script_control_info *))tab[cur->thread_type].kill;
        next = cur->next;
        if (fn != NULL) {
            fn(cur, p);
        }
        old = ScriptList;
        ScriptList = cur;
        cur->next = old;
        cur = next;
    }
}

void plot_script_nas(subengine *sub, u8 *arg1) {
    struct { s32 a; s32 b; } saved;
    GteSetup s;
    script_control_info *ci;

    if (sub->flags & 0x8000) {
        ci = (script_control_info *)&sub->u6C;
        if (update_script(ci) == 0) {
            sub->flags &= ~0x8000;
            return;
        }
        saved = *(typeof(saved) *)RenderState.unk28;
        gfx_setup_xform(sub, arg1, arg1 + 0x14, RenderState.unk28, &s);
        render_script(ci, &s);
        *(typeof(saved) *)RenderState.unk28 = saved;
    }
}

void kill_script(script_handler *s) {
    s32 *p;
    script_thread_handler *cur;
    script_thread_handler *next;
    void *head;
    script_handler *prev;
    script_handler *nxt;
    script_handler *head2;
    void (*fn)(script_thread_handler *, script_control_info *);

    p = s->info.handle;
    cur = s->info.threads;
    if (p != NULL) {
        *p = 0;
    }
    for (;;) {
        if (cur == NULL) {
            goto unlink;
        }
        fn = ScriptCommands[cur->thread_type].kill;
        next = cur->next;
        if (fn != NULL) {
            fn(cur, &s->info);
        }
        head = ScriptList;
        ScriptList = cur;
        cur->next = head;
        cur = next;
    }
unlink:
    prev = s->last;
    if (prev != NULL) {
        prev->next = s->next;
    } else {
        active_scripts = s->next;
    }
    nxt = s->next;
    if (nxt != NULL) {
        nxt->last = s->last;
    }
    head2 = free_scripts;
    free_scripts = s;
    s->next = head2;
}

void kill_script_threads(script_control_info *ci) {
    s32 *p;
    script_thread_handler *cur;
    script_thread_handler *next;
    void *head;
    void (*fn)(script_thread_handler *, script_control_info *);

    p = ci->handle;
    cur = ci->threads;
    if (p != NULL) {
        *p = 0;
    }
    for (;;) {
        if (cur == NULL) {
            return;
        }
        fn = ScriptCommands[cur->thread_type].kill;
        next = cur->next;
        if (fn != NULL) {
            fn(cur, ci);
        }
        head = ScriptList;
        ScriptList = cur;
        cur->next = head;
        cur = next;
    }
}

void script_attach(obj_dbase *arg0, s32 arg1, script_control_info *arg2, s32 arg3,
                   script_control_info **arg4) {
    u8 *e;
    u8 *hdr;
    s32 *t;
    s32 *q;
    s32 i;
    void (*fn)(s32 *, script_control_info *, s32);

    e = arg0->data;
    if (e != NULL) {
        s32 *p = (s32 *)(e + (*(s32 *)(e + 0x4C) + 0x4C)) + arg1;
        hdr = (u8 *)p + *p;
    } else {
        hdr = NULL;
    }
    arg2->flags = arg3;
    arg2->dbase = arg0;
    arg2->script = arg1;
    arg2->threads = NULL;
    arg2->time.w = 0;
    arg2->time.h.frame = *(s16 *)(hdr + 0xC);
    arg2->unk10 = *(s32 *)(hdr + 0x8);
    arg2->handle = (s32 *)arg4;
    if (arg4 != NULL) {
        *arg4 = arg2;
    }
    t = (s32 *)(hdr + 0x1C);
    for (i = 0; i < *(s32 *)hdr; i++) {
        q = (s32 *)((u8 *)t + *t);
        fn = ScriptCommands[*q].init;
        if (fn != NULL) {
            fn(q, arg2, i);
        }
        t++;
    }
    update_script(arg2);
}

void kill_sub_thread(script_thread_handler *th) {
    subengine *sub;

    sub = &th->type.subscript.sub;
    sub->release(sub);
}

void render_sub_thread(script_thread_handler *th, s32 *arg1) {
    s32 saved;
    u32 flags;
    subengine *sub;
    s32 val;

    sub = &th->type.subscript.sub;
    flags = th->type.subscript.sub.flags;
    val = *arg1;
    saved = ScriptFade;
    if (flags & 0x8000) {
        sub->hook(sub, val, flags);
    }
    ScriptFade = saved;
}

/* A thread from the free list (ScriptList), or NULL. */
static inline script_thread_handler *ThreadPop(void) {
    script_thread_handler *cur;

    cur = ScriptList;
    if (cur != NULL) {
        ScriptList = cur->next;
    }
    return cur;
}

/* A thread from the free list, put first on a script's. */
static inline script_thread_handler *ThreadPush(script_control_info *ci) {
    script_thread_handler *p;

    p = ThreadPop();
    if (p != NULL) {
        p->next = ci->threads;
        ci->threads = p;
    }
    return p;
}

void init_sub_thread(u8 *def, script_control_info *ci, s32 num) {
    script_thread_handler *p;
    subengine *sub;

    p = ThreadPush(ci);
    if (p != NULL) {
        sub = &p->type.subscript.sub;
        p->thread_type = 5;
        p->thread_num = num;
        *sub = SubEngineTemplate;
        sub->flags = 0;
    }
}

void render_emitter_thread(script_thread_handler *th, s32 *arg1) {
    subengine *first;
    subengine *sub;
    s32 val;
    u32 flags;

    first = th->type.particle.subs;
    val = *arg1;
    if (first != NULL) {
        sub = first;
        flags = sub->flags;
        for (;;) {
            if (sub == NULL) {
                return;
            }
            if (sub->hook != NULL) {
                sub->hook(sub, val, flags);
            }
            sub = sub->next;
        }
    }
}

s32 interp_window(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if (arg5 < 0) {
        return arg0;
    }
    if (arg5 < arg2) {
        return interp(arg0, arg1, arg2, arg5);
    }
    if (arg3 < arg5) {
        if (arg4 < arg5) {
            return arg0;
        }
        return interp(arg1, arg0, arg4 - arg3, arg5 - arg3);
    }
    return arg1;
}

void kill_emitter_thread(script_thread_handler *th) {
    subengine *subs;
    partmotdata *mdata;

    subs = th->type.particle.subs;
    if (subs != NULL) {
        subs_release(subs, th->type.particle.last_sub);
        th->type.particle.subs = NULL;
    }
    mdata = th->type.particle.mdata;
    if (mdata != NULL) {
        mal_free(mdata, &MainHeap);
        th->type.particle.mdata = NULL;
    }
}

void vel_update(VelObj *obj, VelParams *p, VelState *s) {
    if (s->vx != 0) {
        obj->x += s->vx >> 8;
    }
    if (s->vy != 0) {
        obj->y += s->vy >> 8;
    }
    if (s->vz != 0) {
        obj->z += s->vz >> 8;
    }
    if (p->addX != 0) {
        s->vx += p->addX;
    }
    if (p->addY != 0) {
        s->vy += p->addY;
    }
    if (p->addZ != 0) {
        s->vz += p->addZ;
    }
    if (p->mulX != 0x100) {
        s->vx = (s->vx * p->mulX) >> 8;
    }
    if (p->mulY != 0x100) {
        s->vy = (s->vy * p->mulY) >> 8;
    }
    if (p->mulZ != 0x100) {
        s->vz = (s->vz * p->mulZ) >> 8;
    }
    obj->angle += p->dAngle;
    obj->dirty = 1;
    if (p->limit < s->frame++) {
        obj->flags &= ~0x8000;
    }
}

void init_emitter_thread(u8 *def, script_control_info *ci, s32 num) {
    script_thread_handler *p;
    u8 *e;
    subengine *q;
    obj_dbase *v;
    u16 w;

    p = ThreadPush(ci);
    if (p != NULL) {
        e = def + 0x14;
        p->thread_type = 1;
        p->thread_num = num;
        p->type.particle.subs = NULL;
        p->type.particle.last_sub = NULL;
        p->type.particle.num_subs = subs_alloc_list(&p->type.particle.subs, &p->type.particle.last_sub,
                                                    *(s32 *)(e + 0x10));
        q = p->type.particle.subs;
        if (q != NULL) {
            do {
                q->flags = 0;
                v = ci->dbase;
                q->u6C.dbase = v;
                w = *(u16 *)(def + 0x3C);
                q->unk51 = 0;
                q->unk52 = 1;
                q->entity = w;
                q = q->next;
            } while (q != NULL);
        }
        p->type.particle.mdata = NULL;
        mal_alloc_owned_low(*(s32 *)(e + 0x10) * 0x28, (void **)&p->type.particle.mdata, (s32)&MainHeap, 0);
    }
}

void init_fade_thread(u8 *def, script_control_info *ci, s32 num) {
    script_thread_handler *p;

    p = ThreadPush(ci);
    if (p != NULL) {
        p->thread_type = 4;
        p->thread_num = num;
        p->type.fade.start = *(s32 *)(def + 0x14);
        p->type.fade.end = *(s32 *)(def + 0x18);
    }
}

void render_entity_thread(script_thread_handler *th, geom_info *gi) {
    draw_object((u8 *)&th->type.entity.sub, gi->rot, th->type.entity.sub.flags);
}

void init_entity_thread(u8 *def, script_control_info *ci, s32 num) {
    script_thread_handler *p;
    subengine *sub;
    obj_dbase *v;
    u16 w;

    p = ThreadPush(ci);
    if (p != NULL) {
        sub = &p->type.anim.sub;
        p->thread_type = 3;
        *sub = SubEngineTemplate;
        v = ci->dbase;
        sub->entity = 0;
        sub->flags = 0;
        sub->u6C.dbase = v;
        w = *(u16 *)(def + 0x28);
        sub->unk68 = w;
        if ((w << 16) != 0) {
            sub->flags |= 0x2000000;
        }
        p->thread_num = num;
        p->type.anim.frame_count = -1;
    }
}

s32 *find_range_30(u8 *arg0, s32 arg1) {
    s32 *p;

    p = (s32 *)(arg0 + 0x30);
    if (p[1] != 0) {
        do {
            if (arg1 >= p[0] && arg1 < p[0] + p[1]) {
                return p;
            }
            p += 19;
        } while (p[1] != 0);
    }
    return p;
}

s32 *find_range_1c(u8 *arg0, s32 arg1) {
    s32 *p;

    p = (s32 *)(arg0 + 0x1C);
    if (p[1] != 0) {
        do {
            if (arg1 >= p[0] && arg1 < p[0] + p[1]) {
                return p;
            }
            p += 10;
        } while (p[1] != 0);
    }
    return p;
}

void init_camera_thread(s32 def, script_control_info *ci, s32 num) {
    script_thread_handler *p;

    p = ThreadPush(ci);
    if (p != NULL) {
        p->thread_type = 2;
        p->thread_num = num;
    }
}

void render_model_thread(script_thread_handler *th, s32 *arg1) {
    draw_object((u8 *)&th->type.anim.sub, (u8 *)*arg1, th->type.anim.sub.flags);
}

s32 *find_range_24(u8 *arg0, s32 arg1) {
    s32 *p;

    p = (s32 *)(arg0 + 0x24);
    while (p[1] != 0) {
        if (arg1 >= p[0] && arg1 < p[0] + p[1]) {
            return p;
        }
        p += 20;
    }
    return p;
}

void init_model_thread(u8 *def, script_control_info *ci, s32 num) {
    script_thread_handler *p;
    subengine *sub;
    obj_dbase *v;
    u16 w;

    p = ThreadPush(ci);
    if (p != NULL) {
        sub = &p->type.anim.sub;
        p->thread_type = 0;
        *sub = SubEngineTemplate;
        v = ci->dbase;
        sub->entity = 0;
        sub->flags = 0;
        sub->u6C.dbase = v;
        w = *(u16 *)(def + 0x1C);
        sub->unk68 = w;
        if ((w << 16) != 0) {
            sub->flags |= 0x2000000;
        }
        p->thread_num = num;
    }
}

void lerp_angles3(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4) {
    arg2[0] = lerp_angle(arg0[0], arg1[0], arg3, arg4);
    arg2[1] = lerp_angle(arg0[1], arg1[1], arg3, arg4);
    arg2[2] = lerp_angle(arg0[2], arg1[2], arg3, arg4);
}

void interp3_s16(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4) {
    arg2[0] = interp(arg0[0], arg1[0], arg3, arg4);
    arg2[1] = interp(arg0[1], arg1[1], arg3, arg4);
    arg2[2] = interp(arg0[2], arg1[2], arg3, arg4);
}

void interp3(s32 *arg0, s32 *arg1, s32 *arg2, s32 arg3, s32 arg4) {
    arg2[0] = interp(arg0[0], arg1[0], arg3, arg4);
    arg2[1] = interp(arg0[1], arg1[1], arg3, arg4);
    arg2[2] = interp(arg0[2], arg1[2], arg3, arg4);
}

s32 *find_range(s32 *arg0, s32 arg1, s32 arg2) {
    while (arg0[1] != 0) {
        if (arg0[0] <= arg1 && arg1 < arg0[0] + arg0[1]) {
            return arg0;
        }
        arg0 = (s32 *)((u8 *)arg0 + arg2);
    }
    return arg0;
}

u8 *script_track_at(u8 *arg0, s32 arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 v;

    u8 *e;

    e = *(u8 **)(arg0 + 0);
    lo = *(s32 *)(arg0 + 4);
    v = *(s16 *)(arg0 + 0xE);
    ScriptTrackThread = arg1;
    ScriptTrackDbase = e;
    p = *(u8 **)(e + 0xC);
    ScriptTrackIndex = lo;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + lo * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = r + (arg1 * 4 + 0x1C) + *(s32 *)(r + (arg1 << 2) + 0x1C);
    }
    if (res != NULL) {
        if (v < *(s32 *)(res + 4) || *(s32 *)(res + 8) < v) {
            res = NULL;
        }
    }
    return res;
}

u8 *get_script_track(u8 *arg0, s32 arg1, s32 arg2) {
    u8 *p;
    u8 *r;
    s32 *q;

    ScriptTrackDbase = arg0;
    p = *(u8 **)(arg0 + 0xC);
    ScriptTrackIndex = arg1;
    ScriptTrackThread = arg2;
    if (p == NULL) {
        r = NULL;
    } else {
        q = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + arg1 * 4);
        r = (u8 *)q + *q;
    }
    if (r == NULL) {
        return NULL;
    }
    return r + (arg2 * 4 + 0x1C) + *(s32 *)(r + (arg2 << 2) + 0x1C);
}

u8 *offset_table_entry(OffsetTable *tbl, s32 i) {
    return (u8 *)&tbl->offs[i] + tbl->offs[i];
}

u8 *get_script_header(obj_dbase *db, s32 arg1) {
    u8 *p;
    s32 *e;

    p = db->data;
    if (p != NULL) {
        e = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + arg1 * 4);
        return (u8 *)e + *e;
    }
    return NULL;
}

script_thread_handler *script_thread_alloc(script_control_info *ci) {
    script_thread_handler *p;

    p = ThreadPop();
    if (p != NULL) {
        p->next = ci->threads;
        ci->threads = p;
    }
    return p;
}

void script_list_push(script_thread_handler *th) {
    script_thread_handler *head;

    head = ScriptList;
    ScriptList = th;
    th->next = head;
}

script_thread_handler *script_list_pop(void) {
    script_thread_handler *cur;

    cur = ScriptList;
    if (cur != NULL) {
        ScriptList = cur->next;
    }
    return cur;
}

void script_unlink(script_handler *s) {
    script_handler *prev;
    script_handler *next;
    script_handler *head;

    prev = s->last;
    if (prev != NULL) {
        prev->next = s->next;
    } else {
        active_scripts = s->next;
    }
    next = s->next;
    if (next != NULL) {
        next->last = s->last;
    }
    head = free_scripts;
    free_scripts = s;
    s->next = head;
}

script_handler *script_alloc(void) {
    script_handler *s;
    script_handler *head;

    s = free_scripts;
    if (s != NULL) {
        free_scripts = s->next;
        if (active_scripts != NULL) {
            active_scripts->last = s;
        }
        head = active_scripts;
        s->last = NULL;
        active_scripts = s;
        s->next = head;
    }
    return s;
}

void scripts_release(void) {
    script_handler *next;
    script_handler *s;

    s = active_scripts;
    if (s != NULL) {
        do {
            next = s->next;
            kill_script(s);
            s = next;
        } while (s != NULL);
    }
    if (ScriptPool != 0) {
        mal_free(ScriptPool, &MainHeap);
    }
    if (ScriptThreadPool != 0) {
        mal_free(ScriptThreadPool, &MainHeap);
    }
}

void scripts_kill_all(void) {
    script_handler *next;
    script_handler *s;

    s = active_scripts;
    if (s != NULL) {
        do {
            next = s->next;
            kill_script(s);
            s = next;
        } while (s != NULL);
    }
}

void scripts_render(void) {
    struct {
        s32 a;
        s32 b;
        u16 c;
        u16 d;
    } s;
    script_handler *sc;
    GfxState *gs;

    sc = active_scripts;
    gs = &RenderState;
    for (;;) {
        /* block-scope declaration, as the implicit one was: keeps the loop unrotated */
        extern void render_script(script_control_info *, GteSetup *);
        if (sc == NULL) {
            break;
        }
        s.a = gfx_object_matrix(&sc->xyzrf);
        s.b = s.a + 0x14;
        s.c = *(u16 *)&gs->unk28[0];
        s.d = *(u16 *)&gs->unk28[4];
        render_script(&sc->info, &s);
        sc = sc->next;
    }
}

void scripts_update(void) {
    script_handler *next;
    script_handler *s;

    s = active_scripts;
    if (s != NULL) {
        do {
            next = s->next;
            if (update_script(&s->info) == 0) {
                script_unlink(s);
            }
            s = next;
        } while (s != NULL);
    }
}

/* data, in address order (tools/scripts/data2c.py) */
ScriptCommand ScriptCommands[6] = {
    /* the threads a script runs, by type */
    { init_model_thread, update_model_thread, render_model_thread, NULL },
    { init_emitter_thread, update_emitter_thread, render_emitter_thread, kill_emitter_thread },
    { init_camera_thread, update_camera_thread, NULL, NULL },
    { init_entity_thread, update_entity_thread, render_entity_thread, NULL },
    { init_fade_thread, update_fade_thread, NULL, NULL },
    { init_sub_thread, update_sub_thread, render_sub_thread, kill_sub_thread },
};
s32 ScriptFade = 0;
s32 ScriptTimeStep = 0;
s32 ScriptThreadPool = 0;
s32 ScriptPool = 0;
script_thread_handler *ScriptList = 0;
script_handler *free_scripts = 0;
script_handler *active_scripts = 0;
u8 *ScriptTrackDbase = 0;
s32 ScriptTrackIndex = 0;
s32 ScriptTrackThread = 0;
