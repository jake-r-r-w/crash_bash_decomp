#!/usr/bin/env node
/* The example hero pack, Bobby: a long-haired black-and-tan miniature dachshund stood up as a cartoon hero, made here from shapes, so
 * none of it is anyone's art but this script's. It writes port/mods/bobby/: a mod.json, and heroes/bobby/ with its hero.json and
 * hero.glb (docs/HERO_PACKS.md), as the hero tool writes a pack. Copy the folder into the game's mods folder, or zip it, to use it.
 *
 * The GLB is a skinned mesh on a skeleton of 25 bones, coloured by its corners (no textures), and its clips, each a pose worked out
 * for every 30th of a second: his own moves on foot (a panting BREATHE with his tail wagging, a scampering RUN, his zoomies as he
 * attacks and wins, and the rest). What he hasn't got, hero.json's stand-ins give him from his own.
 *
 *   node port/mods/bobby.mjs */
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const OUT = path.join(HERE, 'bobby');
const FPS = 30;

/* --- his colours (linear 0..1, as glTF's are), from photos of him --- */
const BLACK = [0.12, 0.11, 0.13];
const SHEEN = [0.3, 0.28, 0.36];      /* his coat where it catches the light */
const TAN = [0.72, 0.38, 0.13];
const TAN_LIGHT = [0.85, 0.52, 0.22];
const NOSE = [0.03, 0.03, 0.035];
const MOUTH = [0.32, 0.05, 0.07];
const TONGUE = [0.93, 0.42, 0.48];
const WHITE = [1, 1, 1];
const HARNESS = [0.16, 0.36, 0.92];
const HARNESS_RIM = [0.62, 0.68, 0.78];
const COLLAR = [0.1, 0.7, 0.78];
const TAG = [0.82, 0.84, 0.86];

const mix = (a, b, t) => a.map((x, i) => x + (b[i] - x) * t);
const clamp01 = (x) => Math.min(1, Math.max(0, x));
const smooth = (a, b, x) => {
    const t = clamp01((x - a) / (b - a));
    return t * t * (3 - 2 * t);
};

/* --- vectors and quaternions --- */
const add = (a, b) => [a[0] + b[0], a[1] + b[1], a[2] + b[2]];
const sub = (a, b) => [a[0] - b[0], a[1] - b[1], a[2] - b[2]];
const mul = (a, s) => [a[0] * s, a[1] * s, a[2] * s];
const len = (a) => Math.hypot(a[0], a[1], a[2]);
const norm = (a) => mul(a, 1 / (len(a) || 1));
const cross = (a, b) => [a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]];
const DEG = Math.PI / 180;
const quat = (axis, deg) => {
    const h = (deg * DEG) / 2, s = Math.sin(h), n = norm(axis);
    return [n[0] * s, n[1] * s, n[2] * s, Math.cos(h)];
};
const qmul = (a, b) => [
    a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1],
    a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0],
    a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3],
    a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2],
];
/* turns about x (pitch: + tips its end back), y (yaw: + turns to his left), z (roll: + tips to his right), in that order */
const euler = (x = 0, y = 0, z = 0) => qmul(qmul(quat([0, 1, 0], y), quat([1, 0, 0], x)), quat([0, 0, 1], z));

/* --- the skeleton: glTF's axes, y up, metres, facing +z; his left is +x. Every bone rests unturned, at its place from its parent --- */
const BONES = [];
const bone = (name, parent, at) => {
    BONES.push({ name, parent: parent == null ? -1 : BONES.findIndex((b) => b.name === parent), at });
    return BONES.length - 1;
};
bone('hips', null, [0, 0.24, 0]);
bone('spine', 'hips', [0, 0.35, 0]);
bone('chest', 'spine', [0, 0.49, 0]);
bone('neck', 'chest', [0, 0.61, 0.02]);
bone('head', 'neck', [0, 0.69, 0.03]);
bone('jaw', 'head', [0, 0.705, 0.11]);
for (const [s, x] of [['l', 1], ['r', -1]]) {
    bone(`ear_${s}1`, 'head', [0.1 * x, 0.83, 0.01]);
    bone(`ear_${s}2`, `ear_${s}1`, [0.135 * x, 0.69, 0.0]);
    bone(`upperarm_${s}`, 'chest', [0.13 * x, 0.555, 0.02]);
    bone(`forearm_${s}`, `upperarm_${s}`, [0.16 * x, 0.455, 0.04]);
    bone(`hand_${s}`, `forearm_${s}`, [0.17 * x, 0.37, 0.06]);
    bone(`thigh_${s}`, 'hips', [0.075 * x, 0.215, 0]);
    bone(`shin_${s}`, `thigh_${s}`, [0.08 * x, 0.125, 0.02]);
    bone(`foot_${s}`, `shin_${s}`, [0.08 * x, 0.045, 0.0]);
}
bone('tail1', 'hips', [0, 0.22, -0.1]);
bone('tail2', 'tail1', [0, 0.27, -0.19]);
bone('tail3', 'tail2', [0, 0.34, -0.245]);
const B = Object.fromEntries(BONES.map((b, i) => [b.name, i]));

/* --- the mesh: shapes, each corner coloured and given its bones --- */
const verts = []; /* { p, c, j: [4], w: [4] } */
const tris = [];

/* weights from a list of [bone, weight] */
const bind = (list) => {
    const l = list.filter(([, w]) => w > 1e-4).sort((a, b) => b[1] - a[1]).slice(0, 4);
    const sum = l.reduce((s, [, w]) => s + w, 0) || 1;
    const j = [0, 0, 0, 0], w = [0, 0, 0, 0];
    l.forEach(([b, x], i) => {
        j[i] = b;
        w[i] = x / sum;
    });
    return { j, w };
};
const vert = (p, c, weights) => {
    const { j, w } = bind(weights);
    verts.push({ p, c, j, w });
    return verts.length - 1;
};
/* a face, its corners round counter-clockwise as it's seen from outside */
const tri = (a, b, c) => tris.push([a, b, c]);

/* An ellipsoid: `rings` from top to bottom, `segs` round; turned by the frame (right, up, forward) */
function ellipsoid({ at, r, rings, segs, colour, weights, frame = null }) {
    const [rx, ry, rz] = r;
    const X = frame ? frame[0] : [1, 0, 0], Y = frame ? frame[1] : [0, 1, 0], Z = frame ? frame[2] : [0, 0, 1];
    const point = (u, v) => {
        const lat = Math.PI * (0.5 - v / rings), lon = (2 * Math.PI * u) / segs;
        const lx = Math.cos(lat) * Math.sin(lon) * rx, ly = Math.sin(lat) * ry, lz = Math.cos(lat) * Math.cos(lon) * rz;
        const local = [lx / rx, ly / ry, lz / rz];
        return { p: add(at, add(add(mul(X, lx), mul(Y, ly)), mul(Z, lz))), local };
    };
    const top = point(0, 0), bottom = point(0, rings);
    const ti = vert(top.p, colour(top.p, top.local), weights(top.p, top.local));
    const rows = [];
    for (let v = 1; v < rings; v++) {
        const row = [];
        for (let u = 0; u < segs; u++) {
            const q = point(u, v);
            row.push(vert(q.p, colour(q.p, q.local), weights(q.p, q.local)));
        }
        rows.push(row);
    }
    const bi = vert(bottom.p, colour(bottom.p, bottom.local), weights(bottom.p, bottom.local));
    for (let u = 0; u < segs; u++) {
        const u1 = (u + 1) % segs;
        tri(ti, rows[0][u], rows[0][u1]);
        for (let v = 0; v + 1 < rows.length; v++) {
            tri(rows[v][u], rows[v + 1][u], rows[v + 1][u1]);
            tri(rows[v][u], rows[v + 1][u1], rows[v][u1]);
        }
        tri(rows[rows.length - 1][u], bi, rows[rows.length - 1][u1]);
    }
}

/* A tube along a path of points, `segs` round, each point with its radii across (x-ish) and through; its ends closed. `side` is the
 * way its first radius points at each point (else across the path from +x). */
function tube({ path: pts, radii, segs, colour, weights, side = null }) {
    const rows = [];
    for (let i = 0; i < pts.length; i++) {
        const dir = norm(sub(pts[Math.min(i + 1, pts.length - 1)], pts[Math.max(i - 1, 0)]));
        let a = side ? side(i) : [1, 0, 0];
        a = norm(sub(a, mul(dir, a[0] * dir[0] + a[1] * dir[1] + a[2] * dir[2])));
        const b = cross(dir, a);
        const row = [];
        for (let u = 0; u < segs; u++) {
            const ang = (2 * Math.PI * u) / segs;
            const off = add(mul(a, Math.cos(ang) * radii[i][0]), mul(b, Math.sin(ang) * radii[i][1]));
            const p = add(pts[i], off);
            row.push(vert(p, colour(p, i / (pts.length - 1), [Math.cos(ang), Math.sin(ang)]), weights(p, i / (pts.length - 1))));
        }
        rows.push(row);
    }
    for (let i = 0; i + 1 < rows.length; i++) {
        for (let u = 0; u < segs; u++) {
            const u1 = (u + 1) % segs;
            tri(rows[i][u], rows[i][u1], rows[i + 1][u1]);
            tri(rows[i][u], rows[i + 1][u1], rows[i + 1][u]);
        }
    }
    /* the ends, fans about their middles */
    for (const [k, flip] of [[0, true], [rows.length - 1, false]]) {
        const p = pts[k];
        const c = vert(p, colour(p, k / (pts.length - 1), [0, 0]), weights(p, k / (pts.length - 1)));
        for (let u = 0; u < segs; u++) {
            const u1 = (u + 1) % segs;
            if (flip) tri(c, rows[k][u1], rows[k][u]);
            else tri(c, rows[k][u], rows[k][u1]);
        }
    }
}

/* the way two bones share a point between where they start (a) and the next starts (b): all the first's at a, half each at b */
const along = (b1, b2, t, from = 0.6) => {
    const s = smooth(from, 1, t);
    return [[b1, 1 - s * 0.5], [b2, s * 0.5]];
};

/* His body: his chest and belly, and his hips. Black, his chest's front tan where it shows above the harness, his coat's sheen on top */
const coat = (p, local) => mix(BLACK, SHEEN, smooth(0.2, 0.9, local[1]) * 0.6);
ellipsoid({
    at: [0, 0.455, 0], r: [0.13, 0.165, 0.12], rings: 8, segs: 10,
    colour: (p, l) => (l[2] > 0.55 && p[1] > 0.535 ? mix(coat(p, l), TAN, smooth(0.55, 0.8, l[2])) : coat(p, l)),
    weights: (p) => [[B.spine, 1 - smooth(0.4, 0.55, p[1])], [B.chest, smooth(0.4, 0.55, p[1])]],
});
ellipsoid({
    at: [0, 0.27, -0.01], r: [0.12, 0.1, 0.11], rings: 6, segs: 10,
    colour: coat, weights: (p) => [[B.hips, 1 - smooth(0.28, 0.36, p[1])], [B.spine, smooth(0.28, 0.36, p[1])]],
});
/* his harness: a band round his chest, just outside it, its edges a light trim */
{
    const ys = [0.395, 0.41, 0.53, 0.545];
    const rows = ys.map((y, k) => {
        const e = 1 - ((y - 0.455) / 0.165) ** 2, sx = 0.13 * Math.sqrt(Math.max(e, 0.05)) * 1.06, sz = 0.12 * Math.sqrt(Math.max(e, 0.05)) * 1.06;
        const row = [];
        for (let u = 0; u < 12; u++) {
            const a = (2 * Math.PI * u) / 12;
            const p = [Math.sin(a) * sx, y, Math.cos(a) * sz];
            row.push(vert(p, k === 0 || k === 3 ? HARNESS_RIM : HARNESS, [[B.spine, y < 0.47 ? 1 : 0.3], [B.chest, y < 0.47 ? 0 : 0.7]]));
        }
        return row;
    });
    for (let k = 0; k + 1 < rows.length; k++) {
        for (let u = 0; u < 12; u++) {
            const u1 = (u + 1) % 12;
            tri(rows[k][u], rows[k + 1][u1], rows[k][u1]);
            tri(rows[k][u], rows[k + 1][u], rows[k + 1][u1]);
        }
    }
}
/* his collar, and its tag */
{
    const segs = 10, rings = 3, R = 0.07, r = 0.013, c = [0, 0.6, 0.025];
    const rows = [];
    for (let v = 0; v < rings; v++) {
        const row = [];
        for (let u = 0; u < segs; u++) {
            const a = (2 * Math.PI * u) / segs, b = (2 * Math.PI * v) / rings;
            const p = [c[0] + Math.sin(a) * (R + Math.cos(b) * r), c[1] + Math.sin(b) * r, c[2] + Math.cos(a) * (R + Math.cos(b) * r)];
            row.push(vert(p, COLLAR, [[B.neck, 0.7], [B.chest, 0.3]]));
        }
        rows.push(row);
    }
    for (let v = 0; v < rings; v++) {
        for (let u = 0; u < segs; u++) {
            const v1 = (v + 1) % rings, u1 = (u + 1) % segs;
            tri(rows[v][u], rows[v1][u], rows[v1][u1]);
            tri(rows[v][u], rows[v1][u1], rows[v][u1]);
        }
    }
    /* the tag: a little bone shape, flat, hanging at the front */
    const t = [0, 0.565, 0.1], w = 0.022, h = 0.012;
    const tag = [[-w, 0], [-w * 0.6, h], [w * 0.6, h], [w, 0], [w * 0.6, -h], [-w * 0.6, -h]].map(([x, y]) =>
        vert([t[0] + x, t[1] + y, t[2]], TAG, [[B.neck, 1]]));
    const mid = vert(t, WHITE, [[B.neck, 1]]);
    for (let i = 0; i < tag.length; i++) tri(mid, tag[i], tag[(i + 1) % tag.length]);
}

/* His head: a big round skull, his long muzzle, black nose, open mouth and tongue; tan over his eyes and on his muzzle */
ellipsoid({
    at: [0, 0.78, 0.015], r: [0.12, 0.11, 0.115], rings: 8, segs: 10,
    colour: (p, l) => {
        const cheek = l[2] > 0.3 && l[1] < -0.1 ? smooth(0.3, 0.7, l[2]) * smooth(-0.1, -0.45, l[1]) : 0;
        return mix(coat(p, l), TAN, cheek);
    },
    weights: () => [[B.head, 1]],
});
tube({
    path: [[0, 0.765, 0.08], [0, 0.75, 0.17], [0, 0.738, 0.255]],
    radii: [[0.075, 0.06], [0.06, 0.048], [0.042, 0.036]],
    segs: 8,
    /* black along his bridge, tan below and at his sides */
    colour: (p, t, [ca, sa]) => mix(TAN, BLACK, smooth(0.35, 0.85, sa) * (1 - t * 0.4)),
    weights: () => [[B.head, 1]],
    side: () => [1, 0, 0],
});
ellipsoid({ at: [0, 0.745, 0.27], r: [0.032, 0.026, 0.024], rings: 4, segs: 6, colour: () => NOSE, weights: () => [[B.head, 1]] });
/* his lower jaw, the mouth's inside, and his tongue: they go with his jaw */
ellipsoid({ at: [0, 0.685, 0.175], r: [0.045, 0.02, 0.075], rings: 4, segs: 8, colour: () => TAN, weights: () => [[B.jaw, 1]] });
ellipsoid({ at: [0, 0.703, 0.17], r: [0.043, 0.016, 0.07], rings: 3, segs: 6, colour: () => MOUTH, weights: () => [[B.jaw, 0.5], [B.head, 0.5]] });
ellipsoid({ at: [0, 0.69, 0.225], r: [0.028, 0.009, 0.05], rings: 3, segs: 6, colour: () => TONGUE, weights: () => [[B.jaw, 1]] });
for (const x of [1, -1]) {
    /* his eyes, dark and shining, and the tan spots over them */
    ellipsoid({ at: [0.055 * x, 0.8, 0.105], r: [0.027, 0.03, 0.02], rings: 4, segs: 6, colour: () => NOSE, weights: () => [[B.head, 1]] });
    ellipsoid({ at: [0.05 * x, 0.812, 0.123], r: [0.008, 0.008, 0.004], rings: 3, segs: 4, colour: () => WHITE, weights: () => [[B.head, 1]] });
    ellipsoid({ at: [0.055 * x, 0.845, 0.095], r: [0.021, 0.012, 0.012], rings: 3, segs: 6, colour: () => TAN, weights: () => [[B.head, 1]] });
    const s = x > 0 ? 'l' : 'r';
    /* his long feathered ears, hanging and flaring at their ends */
    tube({
        path: [[0.095 * x, 0.85, 0.01], [0.13 * x, 0.77, 0.0], [0.145 * x, 0.69, -0.005], [0.155 * x, 0.61, -0.01], [0.15 * x, 0.565, -0.01]],
        radii: [[0.035, 0.018], [0.05, 0.022], [0.06, 0.022], [0.07, 0.02], [0.04, 0.012]],
        segs: 6,
        colour: (p, t) => mix(BLACK, SHEEN, (1 - t) * 0.4),
        weights: (p, t) => [[B[`ear_${s}1`], 1 - smooth(0.2, 0.6, t)], [B[`ear_${s}2`], smooth(0.2, 0.6, t)]],
        side: () => [0, 0, 1],
    });
    /* his arms, black above and tan below the elbow, and his paws */
    tube({
        path: [[0.12 * x, 0.565, 0.02], [0.16 * x, 0.455, 0.04], [0.17 * x, 0.375, 0.06]],
        radii: [[0.042, 0.042], [0.034, 0.034], [0.03, 0.03]],
        segs: 6,
        colour: (p, t) => mix(BLACK, TAN, smooth(0.45, 0.8, t)),
        weights: (p, t) => (t < 0.5 ? along(B[`upperarm_${s}`], B[`forearm_${s}`], t * 2) : along(B[`forearm_${s}`], B[`hand_${s}`], t * 2 - 1, 0.8))
            .concat(t >= 0.5 ? [[B[`upperarm_${s}`], 0]] : []),
    });
    ellipsoid({ at: [0.172 * x, 0.35, 0.07], r: [0.036, 0.03, 0.042], rings: 4, segs: 6, colour: () => TAN_LIGHT, weights: () => [[B[`hand_${s}`], 1]] });
    /* his short legs, and his feet */
    tube({
        path: [[0.075 * x, 0.25, 0], [0.08 * x, 0.125, 0.02], [0.08 * x, 0.05, 0.0]],
        radii: [[0.058, 0.058], [0.044, 0.044], [0.034, 0.034]],
        segs: 6,
        colour: (p, t) => mix(BLACK, TAN, smooth(0.4, 0.75, t)),
        weights: (p, t) => (t < 0.5 ? along(B[`thigh_${s}`], B[`shin_${s}`], t * 2) : along(B[`shin_${s}`], B[`foot_${s}`], t * 2 - 1, 0.8)),
    });
    ellipsoid({ at: [0.082 * x, 0.028, 0.035], r: [0.045, 0.028, 0.07], rings: 4, segs: 6, colour: () => TAN_LIGHT, weights: () => [[B[`foot_${s}`], 1]] });
}
/* his feathered tail */
tube({
    path: [[0, 0.23, -0.09], [0, 0.265, -0.18], [0, 0.33, -0.24], [0, 0.4, -0.265], [0, 0.445, -0.27]],
    radii: [[0.028, 0.028], [0.038, 0.034], [0.034, 0.03], [0.022, 0.02], [0.008, 0.008]],
    segs: 6,
    colour: (p, t, [ca, sa]) => mix(BLACK, TAN, ca < -0.5 && t < 0.6 ? 0.5 : 0),
    weights: (p, t) =>
        t < 0.33 ? along(B.tail1, B.tail2, t * 3) : t < 0.66 ? along(B.tail2, B.tail3, (t - 0.33) * 3) : [[B.tail3, 1]],
    side: () => [1, 0, 0],
});

/* the soles of his feet at 0 */
{
    const lo = Math.min(...verts.map((v) => v.p[1]));
    for (const v of verts) v.p[1] -= lo;
    for (const b of BONES) b.at = [b.at[0], b.at[1] - lo, b.at[2]];
}

/* --- his moves: each bone's turn (and his hips' move) at time t; a bone not named rests --- */

const wave = (t, period, phase = 0) => Math.sin(2 * Math.PI * (t / period + phase));
const ease = (x) => smooth(0, 1, x);

/* the swing of his ears and tail, lagging behind his body's moves */
const ears = (t, bob, period) => ({
    ear_l1: euler(0, 0, -6 - 10 * wave(t, period, -0.15) * bob),
    ear_r1: euler(0, 0, 6 + 10 * wave(t, period, -0.15) * bob),
    ear_l2: euler(8 * wave(t, period, -0.3) * bob, 0, -8 * wave(t, period, -0.3) * bob),
    ear_r2: euler(8 * wave(t, period, -0.3) * bob, 0, 8 * wave(t, period, -0.3) * bob),
});
const wag = (t, period, deg, lift = 20) => ({
    tail1: euler(-lift, deg * wave(t, period), 0),
    tail2: euler(-lift * 0.5, deg * 0.8 * wave(t, period, -0.12), 0),
    tail3: euler(0, deg * 0.6 * wave(t, period, -0.24), 0),
});
const pant = (t, period, open = 12) => ({ jaw: euler(open * 0.5 + open * 0.5 * (0.5 + 0.5 * wave(t, period)), 0, 0) });
/* his arms hanging a little out from his sides: x forward (+ back), z out */
const arms = (l, r, out = 10) => ({
    upperarm_l: euler(l[0], 0, -out + (l[1] || 0)),
    forearm_l: euler(l[2] || 0, 0, 0),
    upperarm_r: euler(r[0], 0, out - (r[1] || 0)),
    forearm_r: euler(r[2] || 0, 0, 0),
});
const legs = (l, r) => ({
    thigh_l: euler(l[0], 0, 0),
    shin_l: euler(l[1] || 0, 0, 0),
    foot_l: euler(l[2] || 0, 0, 0),
    thigh_r: euler(r[0], 0, 0),
    shin_r: euler(r[1] || 0, 0, 0),
    foot_r: euler(r[2] || 0, 0, 0),
});

const CLIPS = {
    /* standing, panting happily, his tail wagging */
    BREATHE: {
        length: 2,
        pose: (t) => ({
            hips: { t: [0, 0.003 * wave(t, 1), 0] },
            spine: { r: euler(2 * wave(t, 1), 0, 0) },
            chest: { r: euler(1.5 * wave(t, 1, 0.1), 0, 1.5 * wave(t, 2)) },
            head: { r: euler(-3 + 3 * wave(t, 2, 0.2), 6 * wave(t, 2), 4 * wave(t, 2, 0.25)) },
            ...wrap(pant(t, 0.4, 16)),
            ...wrap(ears(t, 0.5, 1)),
            ...wrap(wag(t, 0.4, 28)),
            ...wrap(arms([-6 + 3 * wave(t, 2), 0, -15], [-6 - 3 * wave(t, 2), 0, -15])),
        }),
    },
    /* scampering: his short legs a blur, his ears flapping */
    RUN: {
        length: 0.5,
        pose: (t) => {
            const s = wave(t, 0.5);
            return {
                hips: { t: [0, 0.018 * Math.abs(wave(t, 0.5, 0.25)), 0], r: euler(0, 6 * s, 0) },
                spine: { r: euler(12, -4 * s, 0) },
                chest: { r: euler(4, -4 * s, 0) },
                head: { r: euler(-14, 4 * s, 0) },
                ...wrap(pant(t, 0.25, 20)),
                ...wrap(ears(t, 1.6, 0.25)),
                ...wrap(wag(t, 0.25, 18, 35)),
                ...wrap(arms([-40 * s, 0, -50 - 20 * Math.max(0, s)], [40 * s, 0, -50 - 20 * Math.max(0, -s)], 18)),
                ...wrap(legs([-45 * s, 50 * Math.max(0, s), -10 * s], [45 * s, 50 * Math.max(0, -s), 10 * s])),
            };
        },
    },
    /* a crouch, a leap with his paws up, tucked in the air, and down */
    JUMP: {
        length: 1.2,
        pose: (t) => {
            const crouch = smooth(0, 0.12, t) * (1 - smooth(0.12, 0.22, t)) + smooth(0.85, 0.95, t) * (1 - smooth(0.95, 1.2, t));
            const stretch = smooth(0.12, 0.25, t) * (1 - smooth(0.3, 0.45, t));
            const tuck = smooth(0.3, 0.45, t) * (1 - smooth(0.7, 0.85, t));
            return {
                hips: { t: [0, -0.05 * crouch, 0], r: euler(-10 * tuck, 0, 0) },
                spine: { r: euler(20 * crouch - 10 * stretch, 0, 0) },
                chest: { r: euler(8 * crouch, 0, 0) },
                head: { r: euler(-15 * stretch + 10 * crouch, 0, 0) },
                ...wrap(pant(t, 0.3, 10 + 14 * stretch)),
                ...wrap(ears(t, 1.2, 0.6)),
                ...wrap(wag(t, 0.3, 20, 30 + 30 * stretch)),
                ...wrap(arms([-150 * stretch - 60 * tuck + 20 * crouch, 25 * stretch, -40 * tuck], [-150 * stretch - 60 * tuck + 20 * crouch, 25 * stretch, -40 * tuck], 14)),
                ...wrap(legs([-55 * crouch - 70 * tuck, 90 * crouch + 100 * tuck, -30 * crouch], [-55 * crouch - 70 * tuck, 90 * crouch + 100 * tuck, -30 * crouch])),
            };
        },
    },
    /* his zoomies: round and round twice, his arms flung out */
    ATTACK: {
        length: 0.53,
        pose: (t) => {
            const k = t / 0.53;
            return {
                hips: { t: [0, 0.02 * Math.sin(Math.PI * k), 0], r: euler(0, 720 * ease(k), 0) },
                spine: { r: euler(6, 0, 8 * Math.sin(Math.PI * k)) },
                head: { r: euler(-8, -20 * Math.sin(Math.PI * k), 0) },
                ...wrap(pant(t, 0.2, 22)),
                ...wrap(ears(t, 2, 0.26)),
                ...wrap(wag(t, 0.2, 10, 40)),
                ...wrap(arms([-20, 70 * Math.sin(Math.PI * k), -20], [-20, 70 * Math.sin(Math.PI * k), -20], 14)),
                ...wrap(legs([-20 * wave(t, 0.26), 30, 0], [20 * wave(t, 0.26), 30, 0])),
            };
        },
    },
    /* a win: hopping round in his zoomies with his paws up, tail going */
    WIN: {
        length: 2,
        pose: (t) => {
            const hop = Math.abs(Math.sin((2 * Math.PI * t) / 1));
            return {
                hips: { t: [0, 0.06 * hop, 0], r: euler(0, 360 * smooth(0.0, 1.0, t % 1) * (t < 1 ? 1 : 0), 0) },
                spine: { r: euler(-6 * hop, 0, 0) },
                head: { r: euler(-20, 10 * wave(t, 1), 8 * wave(t, 0.5)) },
                ...wrap(pant(t, 0.25, 24)),
                ...wrap(ears(t, 1.6, 0.5)),
                ...wrap(wag(t, 0.18, 35, 40)),
                ...wrap(arms([-160 + 15 * wave(t, 0.5), 20, -20], [-160 - 15 * wave(t, 0.5), 20, -20], 16)),
                ...wrap(legs([-30 * hop, 50 * hop, 0], [-30 * hop, 50 * hop, 0])),
            };
        },
    },
    /* hit: thrown back, his paws flung up, then back on his feet */
    HIT: {
        length: 0.8,
        pose: (t) => {
            const k = smooth(0, 0.15, t) * (1 - smooth(0.4, 0.8, t));
            return {
                hips: { t: [0, -0.03 * k, -0.04 * k] },
                spine: { r: euler(-28 * k, 0, 6 * k) },
                chest: { r: euler(-10 * k, 0, 0) },
                head: { r: euler(-25 * k, 0, -10 * k) },
                ...wrap(pant(t, 0.8, 28 * k)),
                ...wrap(ears(t, 2.5 * k, 0.3)),
                ...wrap(wag(t, 0.8, 5, 10 - 30 * k)),
                ...wrap(arms([-100 * k, 50 * k, -30 * k], [-100 * k, 50 * k, -30 * k])),
                ...wrap(legs([-40 * k, 50 * k, 0], [-20 * k, 30 * k, 0])),
            };
        },
    },
    /* a taunt: his back to them, wiggling his tail end, then a bark over his shoulder */
    TAUNT: {
        length: 1.5,
        pose: (t) => {
            const turn = smooth(0, 0.2, t) * (1 - smooth(1.3, 1.5, t));
            const bark = t > 0.8 && t < 1.3 ? Math.max(0, Math.sin((2 * Math.PI * (t - 0.8)) / 0.25)) : 0;
            return {
                hips: { r: euler(0, 160 * turn + 14 * wave(t, 0.25) * turn, 10 * wave(t, 0.25) * turn) },
                spine: { r: euler(18 * turn, 0, -6 * wave(t, 0.25) * turn) },
                chest: { r: euler(0, -40 * turn, 0) },
                head: { r: euler(-15 * bark, -70 * turn, 0) },
                jaw: { r: euler(8 + 26 * bark, 0, 0) },
                ...wrap(ears(t, 1.2, 0.25)),
                ...wrap(wag(t, 0.12, 40, 40)),
                ...wrap(arms([-20, 0, -40], [-20, 0, -40])),
                ...wrap(legs([-10 * turn, 20 * turn, 0], [-10 * turn, 20 * turn, 0])),
            };
        },
    },
    /* bending to pick up a crate, and up with it over his head */
    PICKUP: {
        length: 0.5,
        pose: (t) => {
            const down = smooth(0, 0.25, t) * (1 - smooth(0.25, 0.5, t)), up = smooth(0.25, 0.5, t);
            return {
                hips: { t: [0, -0.04 * down, 0] },
                spine: { r: euler(35 * down, 0, 0) },
                chest: { r: euler(20 * down, 0, 0) },
                head: { r: euler(-20 * down, 0, 0) },
                ...wrap(pant(t, 0.5, 10)),
                ...wrap(ears(t, 0.8, 0.5)),
                ...wrap(wag(t, 0.25, 20, 30)),
                ...wrap(arms([-70 * down - 165 * up, 10, -20 * down - 30 * up], [-70 * down - 165 * up, 10, -20 * down - 30 * up], 14)),
                ...wrap(legs([-50 * down, 80 * down, -20 * down], [-50 * down, 80 * down, -20 * down])),
            };
        },
    },
    /* walking with a crate held up over his head */
    HOLD_SLOW: {
        length: 0.7,
        pose: (t) => {
            const s = wave(t, 0.7);
            return {
                hips: { t: [0, 0.012 * Math.abs(wave(t, 0.7, 0.25)), 0] },
                spine: { r: euler(-4, -3 * s, 0) },
                head: { r: euler(-6, 0, 0) },
                ...wrap(pant(t, 0.35, 18)),
                ...wrap(ears(t, 1, 0.35)),
                ...wrap(wag(t, 0.35, 15, 30)),
                ...wrap(arms([-165, 10, -30], [-165, 10, -30], 14)),
                ...wrap(legs([-30 * s, 40 * Math.max(0, s), 0], [30 * s, 40 * Math.max(0, -s), 0])),
            };
        },
    },
    /* throwing it: arms back over his head, then flung forward */
    HOLD_THROW: {
        length: 0.5,
        pose: (t) => {
            const back = smooth(0, 0.2, t) * (1 - smooth(0.2, 0.3, t)), fling = smooth(0.2, 0.32, t) * (1 - smooth(0.35, 0.5, t));
            return {
                spine: { r: euler(-14 * back + 20 * fling, 0, 0) },
                head: { r: euler(-10 * back, 0, 0) },
                jaw: { r: euler(10 + 20 * fling, 0, 0) },
                ...wrap(ears(t, 1.4, 0.4)),
                ...wrap(wag(t, 0.25, 15, 30)),
                ...wrap(arms([-165 - 20 * back + 90 * fling, 10, -30 * back], [-165 - 20 * back + 90 * fling, 10, -30 * back], 14)),
                ...wrap(legs([-15 * fling, 20 * fling, 0], [10 * fling, 0, 0])),
            };
        },
    },
};
/* a pose's turns as { bone: { r } } */
function wrap(turns) {
    return Object.fromEntries(Object.entries(turns).map(([k, r]) => [k, { r }]));
}

/* --- the GLB --- */

class Bin {
    constructor() {
        this.parts = [];
        this.size = 0;
        this.views = [];
        this.accessors = [];
    }
    /* typed data as a buffer view and an accessor */
    add(array, type, componentType, count, extra = {}) {
        const bytes = Buffer.from(array.buffer, array.byteOffset, array.byteLength);
        const pad = (4 - (this.size % 4)) % 4;
        if (pad) {
            this.parts.push(Buffer.alloc(pad));
            this.size += pad;
        }
        this.views.push({ buffer: 0, byteOffset: this.size, byteLength: bytes.length });
        this.parts.push(bytes);
        this.size += bytes.length;
        this.accessors.push({ bufferView: this.views.length - 1, componentType, count, type, ...extra });
        return this.accessors.length - 1;
    }
}

function glb() {
    const bin = new Bin();
    const n = verts.length;
    const pos = new Float32Array(n * 3), col = new Float32Array(n * 4), joints = new Uint16Array(n * 4), weights = new Float32Array(n * 4);
    verts.forEach((v, i) => {
        pos.set(v.p, i * 3);
        col.set([...v.c, 1], i * 4);
        joints.set(v.j, i * 4);
        weights.set(v.w, i * 4);
    });
    const lo = [0, 1, 2].map((k) => Math.min(...verts.map((v) => v.p[k]))), hi = [0, 1, 2].map((k) => Math.max(...verts.map((v) => v.p[k])));
    const idx = new Uint16Array(tris.flat());
    const aPos = bin.add(pos, 'VEC3', 5126, n, { min: lo, max: hi });
    const aCol = bin.add(col, 'VEC4', 5126, n);
    const aJoint = bin.add(joints, 'VEC4', 5123, n);
    const aWeight = bin.add(weights, 'VEC4', 5126, n);
    const aIdx = bin.add(idx, 'SCALAR', 5123, idx.length);
    /* each bone rests unturned, so its inverse bind is a move back from where it is */
    const ibm = new Float32Array(BONES.length * 16);
    BONES.forEach((b, i) => ibm.set([1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -b.at[0], -b.at[1], -b.at[2], 1], i * 16));
    const aIbm = bin.add(ibm, 'MAT4', 5126, BONES.length);

    const nodes = BONES.map((b) => {
        const parent = b.parent >= 0 ? BONES[b.parent].at : [0, 0, 0];
        return { name: b.name, translation: sub(b.at, parent), children: [] };
    });
    BONES.forEach((b, i) => b.parent >= 0 && nodes[b.parent].children.push(i));
    for (const node of nodes) if (node.children.length === 0) delete node.children;
    const meshNode = nodes.length;
    nodes.push({ name: 'bobby', mesh: 0, skin: 0 });

    const animations = [];
    for (const [name, clip] of Object.entries(CLIPS)) {
        const frames = Math.round(clip.length * FPS) + 1;
        const times = new Float32Array(frames).map((_, f) => (f / (frames - 1)) * clip.length);
        const aTime = bin.add(times, 'SCALAR', 5126, frames, { min: [0], max: [clip.length] });
        const poses = Array.from(times, (t) => clip.pose(t));
        const channels = [], samplers = [];
        BONES.forEach((b, i) => {
            const rot = poses.some((p) => p[b.name] && p[b.name].r), move = poses.some((p) => p[b.name] && p[b.name].t);
            if (rot) {
                const v = new Float32Array(frames * 4);
                poses.forEach((p, f) => v.set(p[b.name] && p[b.name].r ? p[b.name].r : [0, 0, 0, 1], f * 4));
                samplers.push({ input: aTime, output: bin.add(v, 'VEC4', 5126, frames), interpolation: 'LINEAR' });
                channels.push({ sampler: samplers.length - 1, target: { node: i, path: 'rotation' } });
            }
            if (move) {
                const rest = nodes[i].translation;
                const v = new Float32Array(frames * 3);
                poses.forEach((p, f) => v.set(add(rest, p[b.name] && p[b.name].t ? p[b.name].t : [0, 0, 0]), f * 3));
                samplers.push({ input: aTime, output: bin.add(v, 'VEC3', 5126, frames), interpolation: 'LINEAR' });
                channels.push({ sampler: samplers.length - 1, target: { node: i, path: 'translation' } });
            }
        });
        animations.push({ name, channels, samplers });
    }

    const json = {
        asset: { version: '2.0', generator: 'port/mods/bobby.mjs' },
        scene: 0,
        scenes: [{ nodes: [0, meshNode] }],
        nodes,
        meshes: [{ name: 'bobby', primitives: [{ attributes: { POSITION: aPos, COLOR_0: aCol, JOINTS_0: aJoint, WEIGHTS_0: aWeight }, indices: aIdx, material: 0 }] }],
        materials: [{ name: 'coat', pbrMetallicRoughness: { baseColorFactor: [1, 1, 1, 1], metallicFactor: 0, roughnessFactor: 1 } }],
        skins: [{ joints: BONES.map((_, i) => i), inverseBindMatrices: aIbm, skeleton: 0 }],
        animations,
        buffers: [{ byteLength: bin.size }],
        bufferViews: bin.views,
        accessors: bin.accessors,
    };
    let text = Buffer.from(JSON.stringify(json));
    text = Buffer.concat([text, Buffer.alloc((4 - (text.length % 4)) % 4, 0x20)]);
    let data = Buffer.concat(bin.parts);
    data = Buffer.concat([data, Buffer.alloc((4 - (data.length % 4)) % 4)]);
    const head = Buffer.alloc(12), th = Buffer.alloc(8), dh = Buffer.alloc(8);
    head.write('glTF', 0);
    head.writeUInt32LE(2, 4);
    head.writeUInt32LE(12 + 8 + text.length + 8 + data.length, 8);
    th.writeUInt32LE(text.length, 0);
    th.write('JSON', 4);
    dh.writeUInt32LE(data.length, 0);
    dh.write('BIN\0', 4);
    return Buffer.concat([head, th, text, dh, data]);
}

/* --- how he plays: hero.json --- */
const hero = {
    format: 1,
    id: 'bobby',
    name: 'BOBBY',
    /* Crash's place if nobody has it: a small, quick hero */
    prefer: 'crash',
    height: 520,
    face: 'BREATHE',
    face_turn: 20,
    kinds: {
        foot: {
            subframes: 2,
            otherwise: 'BREATHE',
            stands: [
                ['MEDIUM', 'RUN'], ['SKATE', 'RUN'], ['SLIDE', 'RUN'], ['SWIM', 'RUN'], ['FLIP', 'JUMP'], ['FALL', 'JUMP'], ['PUSH', 'RUN'],
                ['TRANS', 'RUN'], ['BARGE', 'RUN'], ['BANK', 'BREATHE'], ['KICK', 'ATTACK'],
                ['WIN', 'WIN', 'loop'], ['WIN2', 'WIN', 'loop'], ['TAUNT_A', 'TAUNT'],
                ['FIRE1', 'HOLD_THROW'], ['FIRE2', 'HOLD_THROW'], ['FIRE3', 'HOLD_THROW'], ['MINE', 'PICKUP'], ['RECOIL', 'HIT'],
                ['TAZING', 'ATTACK'], ['LIGHT', 'TAUNT'],
                ['HIT', 'HIT', 'once'], ['DAZED', 'HIT', 'once'], ['DIE', 'HIT', 'once'],
            ],
        },
    },
};

fs.rmSync(OUT, { recursive: true, force: true });
fs.mkdirSync(path.join(OUT, 'heroes', 'bobby'), { recursive: true });
fs.writeFileSync(path.join(OUT, 'mod.json'), JSON.stringify({
    name: 'Bobby',
    author: 'Crash Bash PC',
    version: '1.0',
    description: 'Bobby the dachshund as a hero of his own: the example hero pack',
}, null, 2) + '\n');
fs.writeFileSync(path.join(OUT, 'heroes', 'bobby', 'hero.json'), JSON.stringify(hero, null, 2) + '\n');
const file = glb();
fs.writeFileSync(path.join(OUT, 'heroes', 'bobby', 'hero.glb'), file);
console.log(`bobby: ${verts.length} vertices, ${tris.length} triangles, ${BONES.length} bones, ${Object.keys(CLIPS).length} clips, ${file.length} bytes`);
