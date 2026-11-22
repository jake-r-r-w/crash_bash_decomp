/* What a kind's model is made of, for the animation tools (kit.h's KitPose): the game, started with --dump-hero, writes it beside the
 * model (port/game/main/packfollow.c, pose_dump_begin), little-endian, as it's read here. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kit.h"

static int rd(FILE *f, void *to, size_t n) {
    return n == 0 || fread(to, 1, n, f) == n;
}

void kit_pose_free(KitPose *p) {
    int a;

    free(p->names);
    free(p->parent);
    free(p->bone);
    free(p->part);
    free(p->rest);
    free(p->joint);
    free(p->weight);
    for (a = 0; p->anims != NULL && a < p->nanims; a++) {
        free(p->anims[a].pose);
    }
    free(p->anims);
    memset(p, 0, sizeof(*p));
}

int kit_pose_read(const char *path, KitPose *p) {
    FILE *f = fopen(path, "rb");
    char magic[4];
    uint32_t u[6];
    int b, v, a, ok = 0;

    memset(p, 0, sizeof(*p));
    if (f == NULL) {
        return 0;
    }
    if (!rd(f, magic, 4) || memcmp(magic, "HPP1", 4) != 0 || !rd(f, u, sizeof(u)) || u[0] == 0 || u[0] > 128 || u[1] == 0 || u[1] > 8192
        || u[5] > 64) {
        goto done;
    }
    p->nbones = (int)u[0], p->nvert = (int)u[1], p->nprop = (int)u[2], p->hand = (int)u[3], p->cut = (int)u[4], p->nanims = (int)u[5];
    p->names = calloc((size_t)p->nbones, 48);
    p->parent = calloc((size_t)p->nbones, sizeof(int));
    p->bone = calloc((size_t)p->nbones * 3, sizeof(float));
    p->part = calloc((size_t)p->nbones, 1);
    p->rest = calloc((size_t)p->nvert * 3, sizeof(float));
    p->joint = calloc((size_t)p->nvert * 4, 1);
    p->weight = calloc((size_t)p->nvert * 4, sizeof(float));
    p->anims = calloc((size_t)(p->nanims > 0 ? p->nanims : 1), sizeof(KitPoseAnim));
    if (p->names == NULL || p->parent == NULL || p->bone == NULL || p->part == NULL || p->rest == NULL || p->joint == NULL || p->weight == NULL
        || p->anims == NULL) {
        goto done;
    }
    for (b = 0; b < p->nbones; b++) {
        int32_t part;

        if (!rd(f, p->names[b], 48) || !rd(f, &p->parent[b], 4) || !rd(f, &p->bone[b * 3], 12) || !rd(f, &part, 4)) {
            goto done;
        }
        p->names[b][47] = 0;
        p->parent[b] = p->parent[b] >= -1 && p->parent[b] < p->nbones ? p->parent[b] : -1;
        p->part[b] = (signed char)part;
    }
    for (v = 0; v < p->nvert; v++) {
        if (!rd(f, &p->rest[v * 3], 12) || !rd(f, &p->joint[v * 4], 4) || !rd(f, &p->weight[v * 4], 16)) {
            goto done;
        }
        for (b = 0; b < 4; b++) {
            p->joint[v * 4 + b] = p->joint[v * 4 + b] < p->nbones ? p->joint[v * 4 + b] : 0;
        }
    }
    for (a = 0; a < p->nanims; a++) {
        uint32_t frames, own;

        if (!rd(f, p->anims[a].name, 32) || !rd(f, &frames, 4) || !rd(f, &own, 4) || frames > 4096) {
            goto done;
        }
        p->anims[a].name[31] = 0;
        p->anims[a].frames = (int)frames;
        p->anims[a].own = own != 0;
    }
    for (a = 0; a < p->nanims; a++) {
        size_t n = (size_t)p->anims[a].frames * (KIT_POSE_HEAD + (size_t)p->nbones * 12);

        p->anims[a].pose = malloc(n * sizeof(float) + 1);
        if (p->anims[a].pose == NULL || !rd(f, p->anims[a].pose, n * sizeof(float))) {
            goto done;
        }
    }
    ok = 1;
done:
    fclose(f);
    if (!ok) {
        kit_pose_free(p);
    }
    return ok;
}
