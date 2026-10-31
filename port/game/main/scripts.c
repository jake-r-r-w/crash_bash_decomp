#include "common.h"
#include "bash/main.h"
#include "bash/prims.h"
#include "bash/scripts.h"

void init_scripts(s32 arg0, s32 arg1) {
    script_handler *p;
    script_handler *next;
    script_thread_handler *q;
    script_thread_handler *nq;

    if (arg0 != 0) {
        ScriptPool = mal_alloc(arg0 * sizeof(script_handler), &MainHeap);
        p = (script_handler *)ScriptPool;
        for (; arg0 >= 2; arg0--) {
            next = p + 1;
            p->next = next;
            p = next;
        }
        p->next = NULL;
    } else {
        ScriptPool = 0;
    }
    active_scripts = NULL;
    free_scripts = (void *)ScriptPool;
    if (arg1 != 0) {
        ScriptThreadPool = mal_alloc(arg1 * sizeof(script_thread_handler), &MainHeap);
        q = (script_thread_handler *)ScriptThreadPool;
        for (; arg1 >= 2; arg1--) {
            nq = q + 1;
            q->next = nq;
            q = nq;
        }
        q->next = NULL;
    } else {
        ScriptThreadPool = 0;
    }
    ScriptTimeStep = 0x10000;
    ScriptList = (void *)ScriptThreadPool;
}


void update_model_thread(script_thread_handler *th, script_control_info *ci) {
    ModelFile *p;
    script_header *r;
    script_thread *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    script_thread *track;
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
        q = (s32 *)((u8 *)&p->scripts + p->scripts) + lo;
        r = (script_header *)SELF_REL(q);
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = (script_thread *)((u8 *)&r->threaddata[idx] + r->threaddata[idx]);
    }
    if (res != NULL) {
        if (v < res->start_frame || res->end_frame < v) {
            res = NULL;
        }
    }
    track = res;
    if (track != NULL) {
        k = ObjTrack_FindKey(&track->type.entity.key, ci->time.h.frame);
        key = k;
        d = &th->type.anim.sub;
        f = key->len;
        d->xform.order = 3;
        if (f != 0) {
            len = f << 8;
            t = (ci->time.w >> 8) - (key->start << 8);
            CamVec3_Lerp((CamVec3 *)&d->XYZ.x, &key->pos, &key[1].pos, len, t);
            quat_slerp_ex_frac(&key->rot, &key[1].rot, &d->xform.rot.vx, len, t, key->frac);
            d->xform.dirty = 1;
            CamVec3_Lerp((CamVec3 *)&d->xform.scale, &key->scale, &key[1].scale, len, t);
            w = interp(key->weight, key[1].weight, len, t);
        } else {
            *(CamVec3 *)&d->XYZ.x = key->pos;
            d->xform.dirty = 1;
            *(ObjQuat *)&d->xform.rot.vx = key->rot;
            d->xform.scale.vx = key->scale.v[0];
            d->xform.scale.vy = key->scale.v[1];
            d->xform.scale.vz = key->scale.v[2];
            w = key->weight;
        }
        w = 0x1000 - w;
        d->flags |= track->type.entity.flags | 0x8000;
        if (w != 0) {
            d->scale = w;
            d->flags |= 0x40000000;
        }
        d->xform.hasScale = 1;
        d->entity = key->entity;
    } else {
        th->type.anim.sub.flags &= ~0x8000;
    }
}

void update_camera_thread(script_thread_handler *th, script_control_info *ci) {
    ModelFile *p;
    script_header *r;
    script_thread *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    obj_dbase *e;
    CamKey *key;
    camera_info_tag *cam;
    s32 len;
    s32 t;
    CamVec3 eye;
    CamVec3 at;
    script_thread *track;

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
        q = (s32 *)((u8 *)&p->scripts + p->scripts) + lo;
        r = (script_header *)SELF_REL(q);
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = (script_thread *)((u8 *)&r->threaddata[idx] + r->threaddata[idx]);
    }
    if (res != NULL) {
        if (v < res->start_frame || res->end_frame < v) {
            res = NULL;
        }
    }
    track = res;
    if (track != NULL) {
        key = CamTrack_FindKey(track, ci->time.h.frame);
        cam = &MainCamera;
        cam->eye.z = 0x800;
        cam->eye.x = 0;
        cam->eye.y = 0;
        cam->at = *(CamXYZ *)&key->at;
        cam->dist = track->type.camera.dist;
        if (key->len != 0) {
            len = key->len << 8;
            t = (ci->time.w >> 8) - (key->start << 8);
            CamVec3_Lerp(&eye, &key->eye, &key[1].eye, len, t);
            CamVec3_Lerp(&at, &key->at, &key[1].at, len, t);
        } else {
            eye = key->eye;
            at = key->at;
        }
        get_dist_angles((s32 *)&eye, (s32 *)&at, &cam->xform.angle.vx);
        cam->at = *(CamXYZ *)&eye;
        cam->eye.z = 0;
        cam->eye.x = 0;
        cam->eye.y = 0;
    }
}

void update_entity_thread(script_thread_handler *th, script_control_info *ci) {
    script_key_anim *k;
    script_key_anim *key;
    ModelFile *p;
    script_header *r;
    script_thread *res;
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
        q = (s32 *)((u8 *)&p->scripts + p->scripts) + lo;
        r = (script_header *)SELF_REL(q);
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = (script_thread *)((u8 *)&r->threaddata[idx] + r->threaddata[idx]);
    }
    if (res != NULL) {
        if (v < res->start_frame || res->end_frame < v) {
            res = NULL;
        }
    }
    pose = (AnimPose *)&th->type.entity.sub;
    if (res != NULL) {
        range = (AnimRange *)&res->type.anim;
        k = AnimTrack_FindKey((script_key_anim *)res->type.anim.key, ci->time.h.frame);
        key = k;
        f = ci->time.w - (res->start_frame << 16);
        frame = AnimRange_Frame(range, f);
        pose->frame = frame;
        pose->unk74 = range->base + range->lo;
        len = key->frame.len;
        if (len != 0) {
            len <<= 8;
            t = (ci->time.w >> 8) - (key->frame.start << 8);
            quat_slerp_ex_frac(key->quat, key[1].quat, pose->rot, len, t, key->quat_interp);
            CamVec3_Lerp(&pose->pos, (CamVec3 *)&key->pos, (CamVec3 *)&key[1].pos, len, t);
            CamVec3_Lerp(&pose->scale, (CamVec3 *)&key->scale, (CamVec3 *)&key[1].scale, len, t);
            a = interp(key->fade, key[1].fade, len << 8, t);
        } else {
            pose->pos = *(CamVec3 *)&key->pos;
            *(AnimRot *)pose->rot = *(AnimRot *)key->quat;
            pose->scale.v[0] = key->scale.x;
            pose->scale.v[1] = key->scale.y;
            pose->scale.v[2] = key->scale.z;
            a = key->fade;
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
    ModelFile *p;
    script_header *r;
    script_thread *res;
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
        q = (s32 *)((u8 *)&p->scripts + p->scripts) + lo;
        r = (script_header *)SELF_REL(q);
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = (script_thread *)((u8 *)&r->threaddata[idx] + r->threaddata[idx]);
    }
    if (res != NULL) {
        if (v < res->start_frame || res->end_frame < v) {
            res = NULL;
        }
        if (res != NULL) {
            ScriptFade = interp(th->type.fade.start, th->type.fade.end,
                                (res->end_frame - res->start_frame) << 8,
                                (ci->time.w >> 8) - (res->start_frame << 8));
        }
    }
}

void emit_particle(subengine *obj, EmitPart *part, partdata *def) {
    s32 speed;
    s32 n;
    s32 yaw;
    s32 pitch;
    s32 radius;
    s32 r;

    yaw = def->yrotmin;
    pitch = def->xrotmin;
    radius = def->emitterradius;
    n = def->randomspeedflag - def->emitterspeed;
    if (n != 0) {
        speed = def->emitterspeed + Random(n);
    } else {
        speed = def->emitterspeed;
    }
    part->age = 0;
    if (def->zrot != 0) {
        obj->xform.rot.vz = Random(0x1000);
    }
    if (def->yrotmax < def->yrotmin) {
        r = abs((def->yrotmax - def->yrotmin) << 20 >> 20);
    } else {
        r = def->yrotmax - def->yrotmin;
    }
    if (r != 0) {
        yaw = (def->yrotmin + Random(r)) & 0xFFF;
    }
    if (def->xrotmax < def->xrotmin) {
        r = abs((def->xrotmax - def->xrotmin) << 20 >> 20);
    } else {
        r = def->xrotmax - def->xrotmin;
    }
    if (r != 0) {
        pitch = (def->xrotmin + Random(r)) & 0xFFF;
    }
    part->vel[0] = speed * rcossin_tbl[(yaw - 0x400) & 0xFFF].unk2 >> 4;
    part->vel[2] = speed * rcossin_tbl[yaw & 0xFFF].unk2 >> 4;
    part->vel[1] = -(speed * rcossin_tbl[pitch & 0xFFF].unk2 >> 4);
    obj->XYZ.x = (radius * rcossin_tbl[(yaw - 0x400) & 0xFFF].unk2 >> 12) + def->pos.x;
    obj->XYZ.z = (radius * rcossin_tbl[yaw & 0xFFF].unk2 >> 12) + def->pos.z;
    obj->XYZ.y = def->pos.y - (radius * rcossin_tbl[pitch & 0xFFF].unk2 >> 12);
    obj->flags |= 0x8000;
}

void update_emitter_thread(script_thread_handler *th, script_control_info *ci) {
    ModelFile *p;
    script_header *r;
    script_thread *res;
    s32 *q;
    s32 lo;
    s32 idx;
    u8 *e;
    subengine *obj;
    EmitPart *part;
    partdata *def;
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
        q = (s32 *)((u8 *)&p->scripts + p->scripts) + lo;
        r = (script_header *)SELF_REL(q);
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = (script_thread *)((u8 *)&r->threaddata[idx] + r->threaddata[idx]);  /* the header's self-relative offset to the thread */
    }
    if (res == NULL) {
        return;
    }
    def = &res->type.particle;
    if (ci->time.h.frame == res->start_frame) {
        th->type.particle.first = def->maxparticlecount;
    }
    if (res->start_frame >= ci->time.h.frame) {
        return;
    }
    if (th->type.particle.first != 0 || (spawn = 0, def->regenparticle != 0)) {
        spawn = def->particlesperframe;
    }
    if (def->effecttime < ci->time.h.frame) {
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
                obj->XYZ.x += part->vel[0] >> 8;
            }
            if (part->vel[1] != 0) {
                obj->XYZ.y += part->vel[1] >> 8;
            }
            if (part->vel[2] != 0) {
                obj->XYZ.z += part->vel[2] >> 8;
            }
            if (def->gxmin != 0) {
                part->vel[0] += def->gxmin;
            }
            if (def->gymin != 0) {
                part->vel[1] += def->gymin;
            }
            if (def->gzmin != 0) {
                part->vel[2] += def->gzmin;
            }
            if (def->gxmax != 0x100) {
                part->vel[0] = (part->vel[0] * def->gxmax) >> 8;
            }
            if (def->gymax != 0x100) {
                part->vel[1] = (part->vel[1] * def->gymax) >> 8;
            }
            if (def->gzmax != 0x100) {
                part->vel[2] = (part->vel[2] * def->gzmax) >> 8;
            }
            obj->xform.rot.vz += *(u16 *)&def->zrot;  /* the spin speed's low half */
            obj->xform.dirty = 1;
            if (def->particlelifetime < part->age++) {
                obj->flags &= ~0x8000;
            }
            if (obj->flags & 0x8000) {
                in = def->fadeinend;
                a = 0;
                if (in != 0 || def->fadeoutstart != 0) {
                    a = EmitPart_Fade(in, def->fadeoutstart, part->age, def->particlelifetime, 0x1000, 0);
                }
                if (a != 0) {
                    obj->flags |= 0x40000000;
                } else {
                    obj->flags &= ~0x40000000;
                }
                obj->scale = a;
                in = def->scaleinend;
                scale = 0x1000;
                if (in != 0 || def->scaleoutstart != 0) {
                    scale = EmitPart_Fade(in, def->scaleoutstart, part->age, def->particlelifetime, 0, 0x1000);
                }
                obj->xform.scale.vz = scale;
                obj->xform.scale.vy = scale;
                obj->xform.scale.vx = scale;
                obj->xform.hasScale = 1;
                obj->xform.dirty = 1;
            }
        }
        if (spawn != 0 && !(obj->flags & 0x8000)) {
            obj->flags |= 0x8000;
            emit_particle(obj, part, def);
            spawn--;
        }
    }
}

void update_sub_thread(script_thread_handler *th, script_control_info *ci) {
    ModelFile *p;
    script_header *r;
    script_thread *res;
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
        q = (s32 *)((u8 *)&p->scripts + p->scripts) + lo;
        r = (script_header *)SELF_REL(q);
    }
    if (r == NULL) {
        res = NULL;
    } else {
        res = (script_thread *)((u8 *)&r->threaddata[idx] + r->threaddata[idx]);
    }
    if (res != NULL) {
        if (v < res->start_frame || res->end_frame < v) {
            res = NULL;
        }
    }
    d = &th->type.subscript.sub;
    if (res != NULL && ci->time.w >= (res->start_frame << 16) &&
        !(th->type.subscript.sub.flags & 0x8000)) {
        d->XYZ.x = res->type.subscript.key.pos.x;
        d->XYZ.y = res->type.subscript.key.pos.y;
        d->XYZ.z = res->type.subscript.key.pos.z;
        d->xform.rot.vx = *(u16 *)&res->type.subscript.key.ang.x;
        d->xform.rot.vy = *(u16 *)&res->type.subscript.key.ang.y;
        d->xform.rot.vz = *(u16 *)&res->type.subscript.key.ang.z;
        d->xform.scale.vx = res->type.subscript.key.scale.x;
        d->xform.scale.vy = res->type.subscript.key.scale.y;
        d->xform.scale.vz = res->type.subscript.key.scale.z;
        d->xform.order = 0;
        t = *(u16 *)&res->type.subscript.sort_priority;
        d->xform.dirty = 1;
        d->otz = t;
        sub_play_script(ci->dbase, res->type.subscript.script_num, d, 0);
        th->type.subscript.sub.flags |= 0x8000;
    }
}

script_handler *play_script(obj_dbase *arg0, s32 arg1, Blk32 *arg2, CamVec3 *arg3, s32 arg4,
                            script_handler **arg5) {
    script_handler *p;
    ModelFile *e;
    script_header *hdr;
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
        *(CamVec3 *)&p->xyzrf.XYZ.x = *arg3;
        *(Blk32 *)&p->xyzrf.xform.mtx = *arg2;
        p->xyzrf.flags = 0x8000;
        p->xyzrf.xform.order = 4;
        p->xyzrf.xform.hasScale = 0;
        p->xyzrf.xform.dirty = 0;
        e = arg0->data;
        hdr = NULL;
        if (e != NULL) {
            s32 *r = (s32 *)((u8 *)&e->scripts + e->scripts) + arg1;
            hdr = (script_header *)SELF_REL(r);
        }
        p->info.flags = arg4;
        p->info.dbase = arg0;
        p->info.script = arg1;
        p->info.threads = NULL;
        p->info.time.w = 0;
        p->info.time.h.frame = *(s16 *)&hdr->start_frame;
        p->info.unk10 = hdr->end_frame;
        p->info.handle = (s32 *)arg5;
        if (arg5 != NULL) {
            *arg5 = p;
        }
        t = (s32 *)hdr->threaddata;
        for (i = 0; i < hdr->threadcount; i++) {
            q = (s32 *)SELF_REL(t);
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
    script_header *a;
    ModelFile *c;
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
        s32 *t = (s32 *)((u8 *)&c->scripts + c->scripts) + idx;
        a = (script_header *)SELF_REL(t);
    }
    if (ci->time.h.frame >= a->end_frame) {
        ci->flags |= 0x10;
        if (ci->flags & 8) {
            ci->time.w = (a->end_frame - 1) << 16;
        }
        if (ci->flags & 2) {
            ci->time.h.frame = *(u16 *)&a->start_frame;
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
        ((GfxScreen *)gs->screen)->x = arg1->ofx;
        ((GfxScreen *)gs->screen)->y = arg1->ofy;
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

void quat_slerp_ex_frac(void *a0, void *a1, void *a2, s32 a3, s32 a4, void *a5) {
    quat_slerp_ex(a0, a1, (a4 << 12) / a3, a2, a5);
}

void quat_slerp_frac(Quat *a0, Quat *a1, Quat *a2, s32 a3, s32 a4) {
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
    script_header *e;
    s32 *s;
    s32 *q;
    s32 i;
    void (*fn)(u8 *, script_control_info *, s32);
    ModelFile *base;

    p = (script_control_info *)&sub->u6C.dbase;
    base = db->data;
    if (base == NULL) {
        e = NULL;
    } else {
        s = (s32 *)((u8 *)&base->scripts + base->scripts) + arg1;
        e = (script_header *)SELF_REL(s);
    }
    p->flags = arg3;
    p->dbase = db;
    p->script = arg1;
    p->threads = NULL;
    p->time.w = 0;
    p->time.h.frame = *(u16 *)&e->start_frame;
    q = (s32 *)e->threaddata;
    p->unk10 = e->end_frame;
    p->handle = NULL;
    i = 0;
    if (e->threadcount > 0) {
        do {
            u8 *a = SELF_REL(q);
            fn = ScriptCommands[*(s32 *)a].init;
            if (fn != NULL) {
                fn(a, p, i);
            }
            q++;
        } while (++i < e->threadcount);
    }
    update_script(p);
    sub->hook = (void (*)(subengine *, s32, u32))plot_script_nas;
    sub->release = kill_nas_script;
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

void plot_script_nas(subengine *sub, MATRIX *arg1) {
    GteSetup s;
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
        s.rot = gfx_object_matrix(&sc->xyzrf);
        s.trans = (VECTOR *)((u8 *)s.rot + 0x14);
        s.ofx = *(u16 *)&gs->screen[0];
        s.ofy = *(u16 *)&gs->screen[4];
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
void *ScriptThreadPool = 0;
void *ScriptPool = 0;
script_thread_handler *ScriptList = 0;
script_handler *free_scripts = 0;
script_handler *active_scripts = 0;
u8 *ScriptTrackDbase = 0;
s32 ScriptTrackIndex = 0;
s32 ScriptTrackThread = 0;
