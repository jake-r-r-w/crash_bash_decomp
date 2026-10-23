#include "common.h"
#include "bash/main.h"
#include "bash/80017164.h"
#include "bash/scripts.h"

void func_8001E984(s32 arg0, s32 arg1) {
    u8 *p;
    u8 *next;

    if (arg0 != 0) {
        D_80058B6C = (s32)func_80011654(arg0 * 0x7C, &D_8004E0F0);
        p = (u8 *)D_80058B6C;
        for (; arg0 >= 2; arg0--) {
            next = p + 0x7C;
            *(u8 **)(p + 0x74) = next;
            p = next;
        }
        *(s32 *)(p + 0x74) = 0;
    } else {
        D_80058B6C = 0;
    }
    D_80058B78 = NULL;
    free_scripts = (void *)D_80058B6C;
    if (arg1 != 0) {
        D_80058B68 = (s32)func_80011654(arg1 * 0xB8, &D_8004E0F0);
        p = (u8 *)D_80058B68;
        for (; arg1 >= 2; arg1--) {
            next = p + 0xB8;
            *(u8 **)(p + 0xB4) = next;
            p = next;
        }
        *(s32 *)(p + 0xB4) = 0;
    } else {
        D_80058B68 = 0;
    }
    D_80058B64 = 0x10000;
    D_80058B70 = (void *)D_80058B68;
}

void func_8001EAA4(u8 *arg0, u8 *arg1) {
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
    u8 *d;
    s32 len;
    s32 t;
    s32 w;
    s32 f;

    e = *(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
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
        k = ObjTrack_FindKey((ObjKey *)(track + 0x24), *(s16 *)(arg1 + 0xE));
        key = k;
        d = arg0 + 8;
        f = key->len;
        d[0x51] = 3;
        if (f != 0) {
            len = f << 8;
            t = (*(s32 *)(arg1 + 0xC) >> 8) - (key->start << 8);
            CamVec3_Lerp((CamVec3 *)(arg0 + 0xC), &key->pos, &key[1].pos, len, t);
            func_80020B44((s32)&key->rot, (s32)&key[1].rot, (s32)(arg0 + 0x18), len, t, (s32)key->unk34);
            *(s16 *)(d + 0x52) = 1;
            CamVec3_Lerp((CamVec3 *)(arg0 + 0x28), &key->scale, &key[1].scale, len, t);
            w = interp(key->unk4C, key[1].unk4C, len, t);
        } else {
            *(CamVec3 *)(arg0 + 0xC) = key->pos;
            *(s16 *)(d + 0x52) = 1;
            *(ObjQuat *)(arg0 + 0x18) = key->rot;
            *(s32 *)(d + 0x20) = key->scale.v[0];
            *(s32 *)(d + 0x24) = key->scale.v[1];
            *(s32 *)(d + 0x28) = key->scale.v[2];
            w = key->unk4C;
        }
        w = 0x1000 - w;
        *(s32 *)d |= *(s32 *)(track + 0x18) | 0x8000;
        if (w != 0) {
            *(s16 *)(d + 0x76) = w;
            *(s32 *)d |= 0x40000000;
        }
        d[0x50] = 1;
        *(s16 *)(d + 0x74) = key->unk8;
    } else {
        *(s32 *)(arg0 + 8) &= ~0x8000;
    }
}

void update_camera_thread(u8 *arg0, u8 *arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    CamKey *key;
    CamState *cam;
    s32 len;
    s32 t;
    CamVec3 eye;
    CamVec3 at;
    u8 *track;

    e =*(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
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
        key = CamTrack_FindKey(track, *(s16 *)(arg1 + 0xE));
        cam = (CamState *)D_80051640;
        cam->unk8 = 0x800;
        cam->unk0 = 0;
        cam->unk4 = 0;
        cam->pos = key->at;
        cam->unk18 = *(s32 *)(track + 0x18);
        if (key->len != 0) {
            len = key->len << 8;
            t = (*(s32 *)(arg1 + 0xC) >> 8) - (key->start << 8);
            CamVec3_Lerp(&eye, &key->eye, &key[1].eye, len, t);
            CamVec3_Lerp(&at, &key->at, &key[1].at, len, t);
        } else {
            eye = key->eye;
            at = key->at;
        }
        func_800153B4((s32 *)&eye, (s32 *)&at, cam->rot);
        cam->pos = eye;
        cam->unk8 = 0;
        cam->unk0 = 0;
        cam->unk4 = 0;
    }
}

void func_8001F0D4(u8 *arg0, u8 *arg1) {
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

    e =*(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
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
    pose = (AnimPose *)(arg0 + 8);
    if (res != NULL) {
        range = (AnimRange *)(res + 0x14);
        k = AnimTrack_FindKey((AnimKey *)(res + 0x30), *(s16 *)(arg1 + 0xE));
        key = k;
        f = *(s32 *)(arg1 + 0xC) - (*(s32 *)(res + 4) << 16);
        frame = AnimRange_Frame(range, f);
        pose->frame = frame;
        pose->unk74 = range->base + range->lo;
        len = key->len;
        if (len != 0) {
            len <<= 8;
            t = (*(s32 *)(arg1 + 0xC) >> 8) - (key->start << 8);
            func_80020B44((s32)key->rot, (s32)key[1].rot, (s32)pose->rot, len, t, (s32)key->unk30);
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

void func_8001F4F8(u8 *arg0, u8 *arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;

    e = *(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
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
            D_80058B60 = interp(*(s32 *)(arg0 + 8), *(s32 *)(arg0 + 0xC),
                                       (*(s32 *)(res + 8) - *(s32 *)(res + 4)) << 8,
                                       (*(s32 *)(arg1 + 0xC) >> 8) - (*(s32 *)(res + 4) << 8));
        }
    }
}

void func_8001F604(u8 *obj, EmitPart *part, u8 *def) {
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
    part->vel[0] = speed * D_80068BD4[(yaw - 0x400) & 0xFFF].unk2 >> 4;
    part->vel[2] = speed * D_80068BD4[yaw & 0xFFF].unk2 >> 4;
    part->vel[1] = -(speed * D_80068BD4[pitch & 0xFFF].unk2 >> 4);
    *(s32 *)(obj + 4) = (radius * D_80068BD4[(yaw - 0x400) & 0xFFF].unk2 >> 12) + *(s32 *)(def + 4);
    *(s32 *)(obj + 0xC) = (radius * D_80068BD4[yaw & 0xFFF].unk2 >> 12) + *(s32 *)(def + 0xC);
    *(s32 *)(obj + 8) = *(s32 *)(def + 8) - (radius * D_80068BD4[pitch & 0xFFF].unk2 >> 12);
    *(s32 *)(obj + 0) |= 0x8000;
}

void func_8001F828(u8 *arg0, u8 *arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    u8 *e;
    u8 *obj;
    EmitPart *part;
    u8 *def;
    s32 spawn;
    s32 a;
    s32 scale;
    s32 in;

    obj = *(u8 **)(arg0 + 0xC);
    part = *(EmitPart **)(arg0 + 0x14);
    e = *(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
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
    if (*(s16 *)(arg1 + 0xE) == *(s32 *)(res + 4)) {
        *(s32 *)(arg0 + 0x18) = *(s32 *)(def + 0x10);
    }
    if (*(s32 *)(res + 4) >= *(s16 *)(arg1 + 0xE)) {
        return;
    }
    if (*(s32 *)(arg0 + 0x18) != 0 || (spawn = 0, *(s32 *)(def + 0x6C) != 0)) {
        spawn = *(s32 *)(def + 0x14);
    }
    if (*(s32 *)(def + 0x1C) < *(s16 *)(arg1 + 0xE)) {
        spawn = 0;
    }
    *(s32 *)(arg0 + 0x18) -= spawn;
    if (*(s32 *)(arg0 + 0x18) < 0) {
        *(s32 *)(arg0 + 0x18) = 0;
    }
    for (;; obj = *(u8 **)(obj + 0x5C), part++) {
        if (obj == NULL) {
            return;
        }
        if (*(s32 *)obj & 0x8000) {
            if (part->vel[0] != 0) {
                *(s32 *)(obj + 4) += part->vel[0] >> 8;
            }
            if (part->vel[1] != 0) {
                *(s32 *)(obj + 8) += part->vel[1] >> 8;
            }
            if (part->vel[2] != 0) {
                *(s32 *)(obj + 0xC) += part->vel[2] >> 8;
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
            *(u16 *)(obj + 0x14) += *(u16 *)(def + 0x68);
            *(s16 *)(obj + 0x52) = 1;
            if (*(s32 *)(def + 0x18) < part->age++) {
                *(s32 *)obj &= ~0x8000;
            }
            if (*(s32 *)obj & 0x8000) {
                in = *(s32 *)(def + 0x58);
                a = 0;
                if (in != 0 || *(s32 *)(def + 0x5C) != 0) {
                    a = EmitPart_Fade(in, *(s32 *)(def + 0x5C), part->age, *(s32 *)(def + 0x18), 0x1000, 0);
                }
                if (a != 0) {
                    *(s32 *)obj |= 0x40000000;
                } else {
                    *(s32 *)obj &= ~0x40000000;
                }
                *(s16 *)(obj + 0x76) = a;
                in = *(s32 *)(def + 0x60);
                scale = 0x1000;
                if (in != 0 || *(s32 *)(def + 0x64) != 0) {
                    scale = EmitPart_Fade(in, *(s32 *)(def + 0x64), part->age, *(s32 *)(def + 0x18), 0, 0x1000);
                }
                *(s32 *)(obj + 0x28) = scale;
                *(s32 *)(obj + 0x24) = scale;
                *(s32 *)(obj + 0x20) = scale;
                *(u8 *)(obj + 0x50) = 1;
                *(s16 *)(obj + 0x52) = 1;
            }
        }
        if (spawn != 0 && !(*(s32 *)obj & 0x8000)) {
            *(s32 *)obj |= 0x8000;
            func_8001F604(obj, part, def);
            spawn--;
        }
    }
}

void func_8001FCDC(u8 *arg0, u8 *arg1) {
    u8 *p;
    u8 *r;
    u8 *res;
    s32 *q;
    s32 lo;
    s32 idx;
    s32 v;
    u8 *e;
    u8 *d;
    u32 t;

    e = *(u8 **)(arg1 + 0);
    lo = *(s32 *)(arg1 + 4);
    idx = *(s32 *)(arg0 + 0);
    v = *(s16 *)(arg1 + 0xE);
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
    D_80058B84 = idx;
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
    d = arg0 + 8;
    if (res != NULL && *(s32 *)(arg1 + 0xC) >= (*(s32 *)(res + 4) << 16) &&
        !(*(s32 *)(arg0 + 8) & 0x8000)) {
        *(s32 *)(d + 4) = *(s32 *)(res + 0x2C);
        *(s32 *)(d + 8) = *(s32 *)(res + 0x30);
        *(s32 *)(d + 0xC) = *(s32 *)(res + 0x34);
        *(u16 *)(d + 0x10) = *(u16 *)(res + 0x38);
        *(u16 *)(d + 0x12) = *(u16 *)(res + 0x3C);
        *(u16 *)(d + 0x14) = *(u16 *)(res + 0x40);
        *(s32 *)(d + 0x20) = *(s32 *)(res + 0x60);
        *(s32 *)(d + 0x24) = *(s32 *)(res + 0x64);
        *(s32 *)(d + 0x28) = *(s32 *)(res + 0x68);
        *(u8 *)(d + 0x51) = 0;
        t = *(u16 *)(res + 0x18);
        *(u16 *)(d + 0x52) = 1;
        *(u16 *)(d + 0x68) = t;
        sub_play_script(*(struct obj_dbase **)arg1, *(s32 *)(res + 0x14), (subengine *)d, 0);
        *(s32 *)(arg0 + 8) |= 0x8000;
    }
}

u8 *func_8001FE80(u8 *arg0, s32 arg1, Blk32 *arg2, CamVec3 *arg3, s32 arg4, u8 **arg5) {
    u8 *p;
    u8 *e;
    u8 *hdr;
    s32 *t;
    s32 *q;
    s32 i;
    u8 *prev;
    u8 *n;
    void (*fn)(s32 *, u8 *, s32);

    if (*(u8 **)(arg0 + 0xC) == NULL) {
        return NULL;
    }
    n = free_scripts;
    if (n != NULL) {
        free_scripts = *(u8 **)(n + 0x74);
        if (D_80058B78 != NULL) {
            *(u8 **)((u8 *)D_80058B78 + 0x70) = n;
        }
        prev = D_80058B78;
        *(u8 **)(n + 0x70) = NULL;
        D_80058B78 = n;
        *(u8 **)(n + 0x74) = prev;
    }
    p = n;
    if (p != NULL) {
        *(CamVec3 *)(p + 0x20) = *arg3;
        *(Blk32 *)(p + 0x4C) = *arg2;
        *(s32 *)(p + 0x1C) = 0x8000;
        p[0x6D] = 4;
        p[0x6C] = 0;
        *(s16 *)(p + 0x6E) = 0;
        e = *(u8 **)(arg0 + 0xC);
        hdr = NULL;
        if (e != NULL) {
            s32 *r = (s32 *)(e + (*(s32 *)(e + 0x4C) + 0x4C)) + arg1;
            hdr = (u8 *)r + *r;
        }
        *(s32 *)(p + 0x14) = arg4;
        *(u8 **)(p + 0x0) = arg0;
        *(s32 *)(p + 0x4) = arg1;
        *(s32 *)(p + 0x8) = 0;
        *(s32 *)(p + 0xC) = 0;
        *(s16 *)(p + 0xE) = *(s16 *)(hdr + 0xC);
        *(s32 *)(p + 0x10) = *(s32 *)(hdr + 0x8);
        *(u8 ***)(p + 0x18) = arg5;
        if (arg5 != NULL) {
            *arg5 = p;
        }
        t = (s32 *)(hdr + 0x1C);
        for (i = 0; i < *(s32 *)hdr; i++) {
            q = (s32 *)((u8 *)t + *t);
            fn = (void (*)(s32 *, u8 *, s32))D_80058B00[*q].unk0;
            if (fn != NULL) {
                fn(q, p, i);
            }
            t++;
        }
        func_80020064(p);
    }
    return p;
}

s32 func_80020064(u8 *arg0) {
    u8 *s;
    s32 *a;
    u8 *c;
    void (*fn)(u8 *, u8 *);
    s32 idx;

    s = *(u8 **)(arg0 + 8);
    D_80058B60 = 0;
    while (s != NULL) {
        fn = (void (*)(u8 *, u8 *))D_80058B00[*(s32 *)(s + 4)].unk4;
        if (fn != NULL) {
            fn(s, arg0);
        }
        s = *(u8 **)(s + 0xB4);
    }
    *(s32 *)(arg0 + 0xC) = *(s32 *)(arg0 + 0xC) + D_80058B64;
    c = *(u8 **)(*(u8 **)arg0 + 0xC);
    idx = *(s32 *)(arg0 + 4);
    if (c == NULL) {
        a = NULL;
    } else {
        s32 *t = (s32 *)(c + (*(s32 *)(c + 0x4C) + 0x4C) + idx * 4);
        a = (s32 *)((u8 *)t + *t);
    }
    if (*(s16 *)(arg0 + 0xE) >= a[2]) {
        *(s32 *)(arg0 + 0x14) |= 0x10;
        if (*(s32 *)(arg0 + 0x14) & 8) {
            *(s32 *)(arg0 + 0xC) = (a[2] - 1) << 16;
        }
        if (*(s32 *)(arg0 + 0x14) & 2) {
            *(s16 *)(arg0 + 0xE) = *(u16 *)(a + 3);
        }
        if (*(s32 *)(arg0 + 0x14) & 4) {
            func_80021054(arg0);
            return 0;
        }
    }
    return 1;
}

void func_800201BC(u8 *arg0, GteSetup *arg1) {
    MATRIX m;
    GteSetup out;
    s32 dp;
    u8 *p;
    GfxState *gs;
    void (*fn)(u8 *, GteSetup *);

    dp = D_80056998.modelDp;
    p = *(u8 **)(arg0 + 8);
    D_80056998.modelDp =0x1000 - (((0x1000 - D_80058B60) * (0x1000 - dp)) >> 12);
    m = *arg1->rot;
    m.t[0] = arg1->trans->vx;
    m.t[1] = arg1->trans->vy;
    m.t[2] = arg1->trans->vz;
    out.rot = &m;
    out.trans = arg1->trans;
    out.ofx = arg1->ofx;
    out.ofy = arg1->ofy;
    for (; p != NULL;) {
        gs = &D_80056998;
        ((GfxScreen *)gs->unk28)->x = arg1->ofx;
        ((GfxScreen *)gs->unk28)->y = arg1->ofy;
        fn = (void (*)(u8 *, GteSetup *))D_80058B00[*(s32 *)(p + 4)].unk8;
        if (fn != NULL) {
            fn(p, &out);
        }
        p = *(u8 **)(p + 0xB4);
    }
    D_800569AC = dp;
}

void func_80020344(Quat *a, Quat *b, s32 t, Quat *out) {
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
    ka = (D_80068BD4[((((0x1000 - t) * ang >> 12) & 0xFFF) - 0x400) & 0xFFF].unk2 * inv) >> 6;
    kb = (D_80068BD4[(((t * ang >> 12) & 0xFFF) - 0x400) & 0xFFF].unk2 * inv) >> 6;
    out->v[0] = (ka * a->v[0] + kb * q.v[0]) >> 18;
    out->v[1] = (ka * a->v[1] + kb * q.v[1]) >> 18;
    out->v[2] = (ka * a->v[2] + kb * q.v[2]) >> 18;
    out->v[3] = (ka * a->v[3] + kb * q.v[3]) >> 18;
}

/* Cosine at angle x / 2, linearly interpolated for odd x. */
static inline s32 Trig_HalfCos(s32 x) {
    if (x & 1) {
        return (D_80068BD4[((x >> 1) - 0x400) & 0xFFF].unk2 + D_80068BD4[((x >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    }
    return D_80068BD4[((x >> 1) - 0x400) & 0xFFF].unk2;
}

void func_80020680(Quat *a, Quat *b, s32 t, Quat *out, s32 *pre) {
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
        ang = (D_80068BD4[((ib >> 1) - 0x400) & 0xFFF].unk2 + D_80068BD4[((ib >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    } else {
        ang = D_80068BD4[((ib >> 1) - 0x400) & 0xFFF].unk2;
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

void func_80020B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    func_80020680(a0, a1, (a4 << 12) / a3, a2, a5);
}

void func_80020BA8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    func_80020344(a0, a1, (a4 << 12) / a3, a2);
}

/* No callers. Retail multiplies (b - a) by the fraction but never reads lo
 * (no mflo) and returns a: the interpolating return sits behind a test that
 * is always false, which only combine sees (after flow), so the mult stays
 * and the dead block is dropped after reload. */
s32 func_80020C04(s32 arg0) {
    s32 a;
    s32 b;
    s32 t;

    a = D_80068BD4[((arg0 >> 16) - 0x400) & 0xFFF].unk2;
    b = D_80068BD4[((arg0 >> 16) - 0x3FF) & 0xFFF].unk2;
    t = (b - a) * (arg0 & 0xFFFF);
    if (!((arg0 & 0x7FFF) >> 15)) {
        return a;
    }
    return a + (t >> 16);
}

s16 func_80020C4C(s32 arg0) {
    if (arg0 & 1) {
        return (D_80068BD4[((arg0 >> 1) - 0x400) & 0xFFF].unk2 + D_80068BD4[((arg0 >> 1) - 0x3FF) & 0xFFF].unk2) >> 1;
    }
    return D_80068BD4[((arg0 >> 1) - 0x400) & 0xFFF].unk2;
}

void sub_play_script(struct obj_dbase *db, s32 arg1, subengine *sub, s32 arg3) {
    u8 *p;
    u8 *e;
    s32 *s;
    s32 *q;
    s32 i;
    void (*fn)(u8 *, u8 *, s32);
    u8 *base;

    p = (u8 *)sub + 0x6C;
    base = db->data;
    if (base == NULL) {
        e = NULL;
    } else {
        s = (s32 *)(base + (*(s32 *)(base + 0x4C) + 0x4C)) + arg1;
        e = (u8 *)s + *s;
    }
    *(s32 *)(p + 0x14) = arg3;
    *(struct obj_dbase **)p = db;
    *(s32 *)(p + 4) = arg1;
    *(s32 *)(p + 8) = 0;
    *(s32 *)(p + 0xC) = 0;
    *(u16 *)(p + 0xE) = *(u16 *)(e + 0xC);
    q = (s32 *)(e + 0x1C);
    *(s32 *)(p + 0x10) = *(s32 *)(e + 8);
    *(s32 *)(p + 0x18) = 0;
    i = 0;
    if (*(s32 *)e > 0) {
        do {
            u8 *a = (u8 *)q + *q;
            fn = (void (*)(u8 *, u8 *, s32))D_80058B00[*(s32 *)a].unk0;
            if (fn != NULL) {
                fn(a, p, i);
            }
            q++;
        } while (++i < *(s32 *)e);
    }
    func_80020064(p);
    *(void **)((u8 *)sub + 0x54) = func_80020E9C;
    *(void **)((u8 *)sub + 0x58) = func_80020DF8;
}

void func_80020DF8(u8 *arg0) {
    u8 *p;
    u8 *cur;
    u8 *next;
    s32 *flag;
    void *old;
    D_80058B00_Entry *tab;
    void (*fn)(u8 *, u8 *);

    p = arg0 + 0x6C;
    flag = *(s32 **)(p + 0x18);
    cur = *(u8 **)(p + 8);
    if (flag != NULL) {
        *flag = 0;
    }
    tab = D_80058B00;
    for (;;) {
        if (cur == NULL) {
            return;
        }
        fn = tab[*(s32 *)(cur + 4)].unkC;
        next = *(u8 **)(cur + 0xB4);
        if (fn != NULL) {
            fn(cur, p);
        }
        old = D_80058B70;
        D_80058B70 = cur;
        *(void **)(cur + 0xB4) = old;
        cur = next;
    }
}

void func_80020E9C(NAS *nas, u8 *arg1) {
    struct { s32 a; s32 b; } saved;
    GteSetup s;
    u8 *sub;

    if (nas->NASid & 0x8000) {
        sub = (u8 *)nas + 0x6C;
        if (func_80020064(sub) == 0) {
            nas->NASid &= ~0x8000;
            return;
        }
        saved = *(typeof(saved) *)D_80056998.unk28;
        func_8001965C(nas, arg1, arg1 + 0x14, D_80056998.unk28, &s);
        func_800201BC(sub, &s);
        *(typeof(saved) *)D_80056998.unk28 = saved;
    }
}

void func_80020F5C(u8 *arg0) {
    s32 *p;
    u8 *cur;
    u8 *next;
    void *head;
    u8 *prev;
    u8 *nxt;
    void *head2;
    void (*fn)(u8 *, u8 *);

    p = *(s32 **)(arg0 + 0x18);
    cur = *(u8 **)(arg0 + 8);
    if (p != NULL) {
        *p = 0;
    }
    for (;;) {
        if (cur == NULL) {
            goto unlink;
        }
        fn = D_80058B00[*(s32 *)(cur + 4)].unkC;
        next = *(u8 **)(cur + 0xB4);
        if (fn != NULL) {
            fn(cur, arg0);
        }
        head = D_80058B70;
        D_80058B70 = cur;
        *(void **)(cur + 0xB4) = head;
        cur = next;
    }
unlink:
    prev = *(u8 **)(arg0 + 0x70);
    if (prev != NULL) {
        *(s32 *)(prev + 0x74) = *(s32 *)(arg0 + 0x74);
    } else {
        D_80058B78 = *(void **)(arg0 + 0x74);
    }
    nxt = *(u8 **)(arg0 + 0x74);
    if (nxt != NULL) {
        *(s32 *)(nxt + 0x70) = *(s32 *)(arg0 + 0x70);
    }
    head2 = free_scripts;
    free_scripts = arg0;
    *(void **)(arg0 + 0x74) = head2;
}

void func_80021054(u8 *arg0) {
    s32 *p;
    u8 *cur;
    u8 *next;
    void *head;
    void (*fn)(u8 *, u8 *);

    p = *(s32 **)(arg0 + 0x18);
    cur = *(u8 **)(arg0 + 8);
    if (p != NULL) {
        *p = 0;
    }
    for (;;) {
        if (cur == NULL) {
            return;
        }
        fn = D_80058B00[*(s32 *)(cur + 4)].unkC;
        next = *(u8 **)(cur + 0xB4);
        if (fn != NULL) {
            fn(cur, arg0);
        }
        head = D_80058B70;
        D_80058B70 = cur;
        *(void **)(cur + 0xB4) = head;
        cur = next;
    }
}

void func_800210F8(u8 *arg0, s32 arg1, u8 *arg2, s32 arg3, u8 **arg4) {
    u8 *e;
    u8 *hdr;
    s32 *t;
    s32 *q;
    s32 i;
    void (*fn)(s32 *, u8 *, s32);

    e = *(u8 **)(arg0 + 0xC);
    if (e != NULL) {
        s32 *p = (s32 *)(e + (*(s32 *)(e + 0x4C) + 0x4C)) + arg1;
        hdr = (u8 *)p + *p;
    } else {
        hdr = NULL;
    }
    *(s32 *)(arg2 + 0x14) = arg3;
    *(u8 **)(arg2 + 0x0) = arg0;
    *(s32 *)(arg2 + 0x4) = arg1;
    *(s32 *)(arg2 + 0x8) = 0;
    *(s32 *)(arg2 + 0xC) = 0;
    *(s16 *)(arg2 + 0xE) = *(s16 *)(hdr + 0xC);
    *(s32 *)(arg2 + 0x10) = *(s32 *)(hdr + 0x8);
    *(u8 ***)(arg2 + 0x18) = arg4;
    if (arg4 != NULL) {
        *arg4 = arg2;
    }
    t = (s32 *)(hdr + 0x1C);
    for (i = 0; i < *(s32 *)hdr; i++) {
        q = (s32 *)((u8 *)t + *t);
        fn = (void (*)(s32 *, u8 *, s32))D_80058B00[*q].unk0;
        if (fn != NULL) {
            fn(q, arg2, i);
        }
        t++;
    }
    func_80020064(arg2);
}

void func_8002120C(u8 *arg0) {
    u8 *p;

    p = arg0 + 8;
    (*(void (**)(u8 *))(p + 0x58))(p);
}

void func_80021238(u8 *arg0, s32 *arg1) {
    s32 saved;
    u32 flags;
    subengine *sub;
    s32 val;

    sub = (subengine *)(arg0 + 8);
    flags = *(u32 *)(arg0 + 8);
    val = *arg1;
    saved = D_80058B60;
    if (flags & 0x8000) {
        sub->hook(sub, val, flags);
    }
    D_80058B60 = saved;
}

static inline u8 *Pop4(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

static inline u8 *Push4(u8 *arg1) {
    u8 *p;

    p = Pop4();
    if (p != NULL) {
        *(u8 **)(p + 0xB4) = *(u8 **)(arg1 + 8);
        *(u8 **)(arg1 + 8) = p;
    }
    return p;
}

void func_8002128C(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;
    subengine *sub;

    p = Push4(arg1);
    if (p != NULL) {
        sub = (subengine *)(p + 8);
        *(s32 *)(p + 4) = 5;
        *(s32 *)p = arg2;
        *sub = D_8005A830;
        *(s32 *)sub = 0;
    }
}

void func_80021330(u8 *arg0, s32 *arg1) {
    subengine *first;
    subengine *sub;
    s32 val;
    u32 flags;

    first = *(subengine **)(arg0 + 0xC);
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

s32 func_800213A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
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

void func_8002141C(u8 *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;

    temp_a0 = (*(s32*)((u8*)(arg0) + 12));
    if (temp_a0 != 0) {
        func_8001D2B4(temp_a0, (*(s32*)((u8*)(arg0) + 16)));
        (*(s32*)((u8*)(arg0) + 12)) = 0;
    }
    temp_a0_2 = (*(s32*)((u8*)(arg0) + 20));
    if (temp_a0_2 != 0) {
        mal_free(temp_a0_2, &D_8004E0F0);
        (*(s32*)((u8*)(arg0) + 20)) = 0;
    }
}

void func_8002147C(VelObj *obj, VelParams *p, VelState *s) {
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

void func_80021604(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;
    u8 *e;
    u8 *q;
    s32 v;
    u16 w;

    p = Push4(arg1);
    if (p != NULL) {
        e = arg0 + 0x14;
        *(s32 *)(p + 4) = 1;
        *(s32 *)p = arg2;
        *(s32 *)(p + 0xC) = 0;
        *(s32 *)(p + 0x10) = 0;
        *(s32 *)(p + 8) = func_8001D48C(p + 0xC, p + 0x10, *(s32 *)(e + 0x10));
        q = *(u8 **)(p + 0xC);
        if (q != NULL) {
            do {
                *(s32 *)q = 0;
                v = *(s32 *)arg1;
                *(s32 *)(q + 0x6C) = v;
                w = *(u16 *)(arg0 + 0x3C);
                *(u8 *)(q + 0x51) = 0;
                *(s16 *)(q + 0x52) = 1;
                *(u16 *)(q + 0x74) = w;
                q = *(u8 **)(q + 0x5C);
            } while (q != NULL);
        }
        *(s32 *)(p + 0x14) = 0;
        func_800115D8(*(s32 *)(e + 0x10) * 0x28, (void **)(p + 0x14), (s32)&D_8004E0F0, 0);
    }
}

static inline u8 *Pop3(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

static inline u8 *Push3(u8 *arg1) {
    u8 *p;

    p = Pop3();
    if (p != NULL) {
        *(u8 **)(p + 0xB4) = *(u8 **)(arg1 + 8);
        *(u8 **)(arg1 + 8) = p;
    }
    return p;
}

void func_80021708(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;

    p = Push3(arg1);
    if (p != NULL) {
        *(s32 *)(p + 4) = 4;
        *(s32 *)p = arg2;
        *(s32 *)(p + 8) = *(s32 *)(arg0 + 0x14);
        *(s32 *)(p + 0xC) = *(s32 *)(arg0 + 0x18);
    }
}

void render_entity_thread(u8 *arg0, s32 *arg1) {
    func_80019F1C(arg0 + 8, *arg1, (*(s32*)((u8*)(arg0) + 8)));
}

void func_80021798(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;
    u8 *sub;
    s32 v;
    u16 w;

    p = Push3(arg1);
    if (p != NULL) {
        sub = p + 8;
        *(s32 *)(p + 4) = 3;
        *(subengine *)sub = D_8005A830;
        v = *(s32 *)arg1;
        *(s16 *)(sub + 0x74) = 0;
        *(s32 *)sub = 0;
        *(s32 *)(sub + 0x6C) = v;
        w = *(u16 *)(arg0 + 0x28);
        *(u16 *)(sub + 0x68) = w;
        if ((w << 16) != 0) {
            *(s32 *)sub |= 0x2000000;
        }
        *(s32 *)p = arg2;
        *(s32 *)(p + 0xB0) = -1;
    }
}

s32 *func_80021878(u8 *arg0, s32 arg1) {
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

s32 *func_800218DC(u8 *arg0, s32 arg1) {
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

static inline u8 *Pop2(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

static inline u8 *Push2(u8 *arg0) {
    u8 *p;

    p = Pop2();
    if (p != NULL) {
        *(u8 **)(p + 0xB4) = *(u8 **)(arg0 + 8);
        *(u8 **)(arg0 + 8) = p;
    }
    return p;
}

void func_80021940(s32 arg0, u8 *arg1, s32 arg2) {
    u8 *p;

    p = Push2(arg1);
    if (p != NULL) {
        *(s32 *)(p + 4) = 2;
        *(s32 *)p = arg2;
    }
}

void func_80021990(u8 *arg0, s32 *arg1) {
    func_80019F1C(arg0 + 8, *arg1, (*(s32*)((u8*)(arg0) + 8)));
}

s32 *func_800219B8(u8 *arg0, s32 arg1) {
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

void func_80021A1C(u8 *arg0, u8 *arg1, s32 arg2) {
    u8 *p;
    u8 *sub;
    s32 v;
    u16 w;

    p = Push4(arg1);
    if (p != NULL) {
        sub = p + 8;
        *(s32 *)(p + 4) = 0;
        *(subengine *)sub = D_8005A830;
        v = *(s32 *)arg1;
        *(s16 *)(sub + 0x74) = 0;
        *(s32 *)sub = 0;
        *(s32 *)(sub + 0x6C) = v;
        w = *(u16 *)(arg0 + 0x1C);
        *(u16 *)(sub + 0x68) = w;
        if ((w << 16) != 0) {
            *(s32 *)sub |= 0x2000000;
        }
        *(s32 *)p = arg2;
    }
}

void func_80021AF0(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4) {
    arg2[0] = func_800152A8(arg0[0], arg1[0], arg3, arg4);
    arg2[1] = func_800152A8(arg0[1], arg1[1], arg3, arg4);
    arg2[2] = func_800152A8(arg0[2], arg1[2], arg3, arg4);
}

void func_80021B88(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4) {
    arg2[0] = interp(arg0[0], arg1[0], arg3, arg4);
    arg2[1] = interp(arg0[1], arg1[1], arg3, arg4);
    arg2[2] = interp(arg0[2], arg1[2], arg3, arg4);
}

void func_80021C20(u8 *arg0, u8 *arg1, u8 *arg2, s32 arg3, s32 arg4) {
    (*(s32*)((u8*)(arg2) + 0)) = interp((*(s32*)((u8*)(arg0) + 0)), (*(s32*)((u8*)(arg1) + 0)), arg3, arg4);
    (*(s32*)((u8*)(arg2) + 4)) = interp((*(s32*)((u8*)(arg0) + 4)), (*(s32*)((u8*)(arg1) + 4)), arg3, arg4);
    (*(s32*)((u8*)(arg2) + 8)) = interp((*(s32*)((u8*)(arg0) + 8)), (*(s32*)((u8*)(arg1) + 8)), arg3, arg4);
}

s32 *func_80021CB8(s32 *arg0, s32 arg1, s32 arg2) {
    while (arg0[1] != 0) {
        if (arg0[0] <= arg1 && arg1 < arg0[0] + arg0[1]) {
            return arg0;
        }
        arg0 = (s32 *)((u8 *)arg0 + arg2);
    }
    return arg0;
}

u8 *func_80021D1C(u8 *arg0, s32 arg1) {
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
    D_80058B84 = arg1;
    D_80058B7C = e;
    p = *(u8 **)(e + 0xC);
    D_80058B80 = lo;
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

u8 *func_80021DD4(u8 *arg0, s32 arg1, s32 arg2) {
    u8 *p;
    u8 *r;
    s32 *q;

    D_80058B7C = arg0;
    p = *(u8 **)(arg0 + 0xC);
    D_80058B80 = arg1;
    D_80058B84 = arg2;
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

u8 *func_80021E44(OffsetTable *tbl, s32 i) {
    return (u8 *)&tbl->offs[i] + tbl->offs[i];
}

u8 *get_script_header(struct obj_dbase *db, s32 arg1) {
    u8 *p;
    s32 *e;

    p = db->data;
    if (p != NULL) {
        e = (s32 *)(p + (*(s32 *)(p + 0x4C) + 0x4C) + arg1 * 4);
        return (u8 *)e + *e;
    }
    return NULL;
}

static inline u8 *PopD_80058B70(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

u8 *func_80021E98(u8 *arg0) {
    u8 *p;

    p = PopD_80058B70();
    if (p != NULL) {
        *(u8 **)(p + 0xB4) = *(u8 **)(arg0 + 8);
        *(u8 **)(arg0 + 8) = p;
    }
    return p;
}

void func_80021ED8(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = D_80058B70;
    D_80058B70 = arg0;
    (*(s32*)((u8*)(arg0) + 180)) = temp_v1;
}

void *func_80021EEC(void) {
    u8 *cur;

    cur = D_80058B70;
    if (cur != NULL) {
        D_80058B70 = *(void **)(cur + 0xB4);
    }
    return cur;
}

void func_80021F14(u8 *arg0) {
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *temp_v1_3;

    temp_v1 = (*(s32*)((u8*)(arg0) + 112));
    if (temp_v1 != NULL) {
        (*(s32*)((u8*)(temp_v1) + 116)) = (u8 *) (*(s32*)((u8*)(arg0) + 116));
    } else {
        D_80058B78 = (*(s32*)((u8*)(arg0) + 116));
    }
    temp_v1_2 = (*(s32*)((u8*)(arg0) + 116));
    if (temp_v1_2 != NULL) {
        (*(s32*)((u8*)(temp_v1_2) + 112)) = (u8 *) (*(s32*)((u8*)(arg0) + 112));
    }
    temp_v1_3 = free_scripts;
    free_scripts = arg0;
    (*(s32*)((u8*)(arg0) + 116)) = temp_v1_3;
}

u8 *func_80021F6C(void) {
    u8 *temp_v0;
    u8 *temp_v1;

    temp_v1 = free_scripts;
    if (temp_v1 != NULL) {
        free_scripts = (*(s32*)((u8*)(temp_v1) + 116));
        if (D_80058B78 != NULL) {
            (*(s32*)((u8*)(D_80058B78) + 112)) = temp_v1;
        }
        temp_v0 = D_80058B78;
        (*(s32*)((u8*)(temp_v1) + 112)) = 0;
        D_80058B78 = temp_v1;
        (*(s32*)((u8*)(temp_v1) + 116)) = temp_v0;
    }
    return temp_v1;
}

void func_80021FB0(void) {
    u8 *temp_s0;
    u8 *var_a0;

    var_a0 = D_80058B78;
    if (var_a0 != NULL) {
        do {
            temp_s0 = (*(s32*)((u8*)(var_a0) + 116));
            func_80020F5C(var_a0);
            var_a0 = temp_s0;
        } while (var_a0 != NULL);
    }
    if (D_80058B6C != 0) {
        mal_free(D_80058B6C, &D_8004E0F0);
    }
    if (D_80058B68 != 0) {
        mal_free(D_80058B68, &D_8004E0F0);
    }
}

void func_80022030(void) {
    u8 *temp_s0;
    u8 *var_a0;

    var_a0 = D_80058B78;
    if (var_a0 != NULL) {
        do {
            temp_s0 = (*(s32*)((u8*)(var_a0) + 116));
            func_80020F5C(var_a0);
            var_a0 = temp_s0;
        } while (var_a0 != NULL);
    }
}

void scripts_render(void) {
    struct {
        s32 a;
        s32 b;
        u16 c;
        u16 d;
    } s;
    u8 *var_s0;
    GfxState *gs;

    var_s0 = D_80058B78;
    gs = &D_80056998;
    for (;;) {
        /* block-scope declaration, as the implicit one was: keeps the loop unrotated */
        extern void func_800201BC(u8 *, GteSetup *);
        if (var_s0 == NULL) {
            break;
        }
        s.a = func_8001ACC0(var_s0 + 0x1C);
        s.b = s.a + 0x14;
        s.c = *(u16 *)&gs->unk28[0];
        s.d = *(u16 *)&gs->unk28[4];
        func_800201BC(var_s0, &s);
        var_s0 = *(u8 **)(var_s0 + 0x74);
    }
}

void scripts_update(void) {
    u8 *temp_s1;
    u8 *var_s0;

    var_s0 = D_80058B78;
    if (var_s0 != NULL) {
        do {
            temp_s1 = (*(s32*)((u8*)(var_s0) + 116));
            if (func_80020064(var_s0) == 0) {
                func_80021F14(var_s0);
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}
