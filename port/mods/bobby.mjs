#!/usr/bin/env node
/* The example hero pack, Bobby: the user's long-haired black-and-tan miniature dachshund, stood up as a cartoon hero. His model is
 * the user's own, made from photos of him (port/mods/bobby-model.glb, beside this script, as a folder there would be a mod of its
 * own: 1367 triangles, one 2048 texture, no skeleton). This script makes it a pack, as the hero tool will: it writes port/mods/bobby/, a mod.json and heroes/bobby/ with its
 * hero.json and hero.glb (docs/HERO_PACKS.md). Copy the folder into the game's mods folder, or zip it, to use it.
 *
 * What it does to the model:
 * - its two smallest triangles go, for the game's 1365;
 * - its texture is laid out again at 256 x 256 (the game's most), each of its pieces with a border of its own colours, as the game
 *   moves each corner to a whole texel (texture());
 * - a skeleton of 25 bones is fitted to him by hand (BONES: measured off front and side views of him on a grid), and each corner
 *   given its bones: by the part of him it's in (PARTS), then smoothed over his surface so the joins bend softly;
 * - his ten moves (CLIPS), each a pose for every 30th of a second, as before he had a model of his own.
 *
 *   node port/mods/bobby.mjs */
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { fileURLToPath } from 'node:url';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const OUT = path.join(HERE, 'bobby');
const SOURCE = path.join(HERE, 'bobby-model.glb');
const FPS = 30;
const MAX_TRIS = 1365;
const TEX = 256;

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

/* --- PNG, enough for the model's texture: 8 bits, RGB or RGBA, not interlaced --- */

function png_read(buf) {
    let at = 8, w = 0, h = 0, type = 0;
    const idat = [];
    while (at + 8 <= buf.length) {
        const n = buf.readUInt32BE(at), kind = buf.toString('latin1', at + 4, at + 8), data = buf.subarray(at + 8, at + 8 + n);
        if (kind === 'IHDR') {
            w = data.readUInt32BE(0);
            h = data.readUInt32BE(4);
            type = data[9];
            if (data[8] !== 8 || (type !== 2 && type !== 6) || data[12] !== 0) throw new Error('the texture is a PNG this script can\'t read');
        } else if (kind === 'IDAT') idat.push(data);
        else if (kind === 'IEND') break;
        at += 12 + n;
    }
    const bpp = type === 6 ? 4 : 3, raw = zlib.inflateSync(Buffer.concat(idat)), out = Buffer.alloc(w * h * 4), row = w * bpp;
    let prev = Buffer.alloc(row);
    for (let y = 0; y < h; y++) {
        const f = raw[y * (row + 1)], line = Buffer.from(raw.subarray(y * (row + 1) + 1, (y + 1) * (row + 1)));
        for (let i = 0; i < row; i++) {
            const a = i >= bpp ? line[i - bpp] : 0, b = prev[i], c = i >= bpp ? prev[i - bpp] : 0;
            const p = a + b - c, pa = Math.abs(p - a), pb = Math.abs(p - b), pc = Math.abs(p - c);
            line[i] = (line[i] + [0, a, b, (a + b) >> 1, pa <= pb && pa <= pc ? a : pb <= pc ? b : c][f]) & 255;
        }
        for (let x = 0; x < w; x++) {
            for (let k = 0; k < 3; k++) out[(y * w + x) * 4 + k] = line[x * bpp + k];
            out[(y * w + x) * 4 + 3] = bpp === 4 ? line[x * bpp + 3] : 255;
        }
        prev = line;
    }
    return { w, h, rgba: out };
}

function crc32(buf) {
    let c = ~0;
    for (const b of buf) {
        c ^= b;
        for (let k = 0; k < 8; k++) c = (c >>> 1) ^ (0xEDB88320 & -(c & 1));
    }
    return ~c >>> 0;
}

function png_write(w, h, rgb) {
    const raw = Buffer.alloc(h * (w * 3 + 1));
    for (let y = 0; y < h; y++) rgb.copy(raw, y * (w * 3 + 1) + 1, y * w * 3, (y + 1) * w * 3);
    const chunk = (kind, data) => {
        const head = Buffer.alloc(8), tail = Buffer.alloc(4);
        head.writeUInt32BE(data.length, 0);
        head.write(kind, 4, 'latin1');
        tail.writeUInt32BE(crc32(Buffer.concat([Buffer.from(kind, 'latin1'), data])), 0);
        return Buffer.concat([head, data, tail]);
    };
    const ihdr = Buffer.alloc(13);
    ihdr.writeUInt32BE(w, 0);
    ihdr.writeUInt32BE(h, 4);
    ihdr.set([8, 2, 0, 0, 0], 8);
    return Buffer.concat([Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]), chunk('IHDR', ihdr), chunk('IDAT', zlib.deflateSync(raw, { level: 9 })), chunk('IEND', Buffer.alloc(0))]);
}

/* --- his model, as the user gave it --- */

function glb_read(file) {
    const b = fs.readFileSync(file), jl = b.readUInt32LE(12), json = JSON.parse(b.toString('utf8', 20, 20 + jl)), bin = b.subarray(20 + jl + 8);
    const view = (i) => {
        const v = json.bufferViews[i];
        return bin.subarray(v.byteOffset || 0, (v.byteOffset || 0) + v.byteLength);
    };
    const read = (i) => {
        const a = json.accessors[i], v = view(a.bufferView).subarray(a.byteOffset || 0), size = { SCALAR: 1, VEC2: 2, VEC3: 3, VEC4: 4 }[a.type];
        const out = [];
        for (let k = 0; k < a.count * size; k++) {
            out.push(a.componentType === 5126 ? v.readFloatLE(k * 4) : a.componentType === 5125 ? v.readUInt32LE(k * 4) : a.componentType === 5123 ? v.readUInt16LE(k * 2) : v[k]);
        }
        return out;
    };
    if (json.meshes.length !== 1 || json.meshes[0].primitives.length !== 1 || (json.nodes || []).some((n) => n.matrix || n.rotation || n.scale || n.translation)) {
        throw new Error('the model has more than one mesh, or a moved node: this script reads only the one it was made for');
    }
    const p = json.meshes[0].primitives[0];
    const pos = read(p.attributes.POSITION), uv = read(p.attributes.TEXCOORD_0), idx = read(p.indices);
    const img = json.images[json.textures[json.materials[p.material].pbrMetallicRoughness.baseColorTexture.index].source];
    return { pos, uv, idx, png: view(img.bufferView) };
}

const SRC = glb_read(SOURCE);

/* the triangles, but the two smallest (slivers), for the game's most */
let tris = [];
for (let i = 0; i < SRC.idx.length; i += 3) tris.push([SRC.idx[i], SRC.idx[i + 1], SRC.idx[i + 2]]);
{
    const P = (v) => [SRC.pos[v * 3], SRC.pos[v * 3 + 1], SRC.pos[v * 3 + 2]];
    const area = ([a, b, c]) => len(cross(sub(P(b), P(a)), sub(P(c), P(a)))) / 2;
    tris = tris.map((t) => [t, area(t)]).sort((x, y) => y[1] - x[1]).slice(0, MAX_TRIS).map(([t]) => t);
}
const NV = SRC.pos.length / 3;
const POS = Array.from({ length: NV }, (_, v) => [SRC.pos[v * 3], SRC.pos[v * 3 + 1], SRC.pos[v * 3 + 2]]);
const UV = Array.from({ length: NV }, (_, v) => [SRC.uv[v * 2], SRC.uv[v * 2 + 1]]);

/* --- his texture: laid out again, 256 x 256, each piece with a border of its own colours round it ---
 * The game keeps a corner's place in its image as whole texels, so each corner moves up to half a texel; the model's pieces lie up
 * against each other, black between, and at 256 a corner moved so showed its neighbour's colour (orange flecks all over his coat). So
 * each piece (triangles joined by their corners) gets a place of its own, as big as they can all be at once, PAD texels clear all
 * round and filled with its edge's colours. */
const PAD = 2;

function texture() {
    const src = png_read(SRC.png), { w, h } = src;
    /* the pieces */
    const parent = Int32Array.from({ length: NV }, (_, i) => i);
    const root = (a) => {
        while (parent[a] !== a) a = parent[a] = parent[parent[a]];
        return a;
    };
    for (const t of tris) {
        parent[root(t[1])] = root(t[0]);
        parent[root(t[2])] = root(t[0]);
    }
    const byRoot = new Map();
    for (const t of tris) {
        const r = root(t[0]);
        if (!byRoot.has(r)) byRoot.set(r, { tris: [], verts: new Set() });
        byRoot.get(r).tris.push(t);
        t.forEach((v) => byRoot.get(r).verts.add(v));
    }
    const pieces = [...byRoot.values()];
    for (const p of pieces) {
        const us = [...p.verts].map((v) => UV[v][0] * w), vs = [...p.verts].map((v) => UV[v][1] * h);
        p.x0 = Math.min(...us);
        p.x1 = Math.max(...us);
        p.y0 = Math.min(...vs);
        p.y1 = Math.max(...vs);
    }
    /* shelves, tallest first, at k texels a source pixel: whether they all fit */
    const place = (k) => {
        const order = [...pieces].sort((a, b) => b.y1 - b.y0 - (a.y1 - a.y0));
        let x = 0, y = 0, row = 0;
        for (const p of order) {
            const pw = Math.ceil((p.x1 - p.x0) * k) + 1 + 2 * PAD, ph = Math.ceil((p.y1 - p.y0) * k) + 1 + 2 * PAD;
            if (pw > TEX) return false;
            if (x + pw > TEX) {
                x = 0;
                y += row;
                row = 0;
            }
            p.ox = x;
            p.oy = y;
            p.pw = pw;
            p.ph = ph;
            x += pw;
            row = Math.max(row, ph);
        }
        return y + row <= TEX;
    };
    let lo = 0.001, hi = 1;
    for (let i = 0; i < 30; i++) {
        const mid = (lo + hi) / 2;
        if (place(mid)) lo = mid;
        else hi = mid;
    }
    const k = lo;
    place(k);
    /* each piece's texels: the mean of the source pixels under each that its own triangles cover */
    const rgb = new Float32Array(TEX * TEX * 3), have = new Uint8Array(TEX * TEX);
    for (const p of pieces) {
        const bx = Math.max(0, Math.floor(p.x0) - 1), by = Math.max(0, Math.floor(p.y0) - 1);
        const bw = Math.min(w, Math.ceil(p.x1) + 2) - bx, bh = Math.min(h, Math.ceil(p.y1) + 2) - by;
        const mine = new Uint8Array(bw * bh);
        for (const t of p.tris) {
            const q = t.map((v) => [UV[v][0] * w - bx, UV[v][1] * h - by]);
            const d = (q[1][0] - q[0][0]) * (q[2][1] - q[0][1]) - (q[2][0] - q[0][0]) * (q[1][1] - q[0][1]);
            if (Math.abs(d) < 1e-9) continue;
            const e = 1 / Math.max(1, Math.abs(d) ** 0.5);
            for (let y = 0; y < bh; y++) {
                for (let x = 0; x < bw; x++) {
                    const px = x + 0.5, py = y + 0.5;
                    const a = ((q[1][0] - px) * (q[2][1] - py) - (q[2][0] - px) * (q[1][1] - py)) / d;
                    const b = ((q[2][0] - px) * (q[0][1] - py) - (q[0][0] - px) * (q[2][1] - py)) / d;
                    if (a >= -e && b >= -e && 1 - a - b >= -e) mine[y * bw + x] = 1;
                }
            }
        }
        for (let ty = p.oy; ty < p.oy + p.ph; ty++) {
            for (let tx = p.ox; tx < p.ox + p.pw; tx++) {
                /* the source pixels under this texel */
                const sx0 = p.x0 + (tx - p.ox - PAD) / k, sy0 = p.y0 + (ty - p.oy - PAD) / k;
                let n = 0;
                const s = [0, 0, 0];
                for (let sy = Math.floor(sy0); sy < Math.ceil(sy0 + 1 / k); sy++) {
                    for (let sx = Math.floor(sx0); sx < Math.ceil(sx0 + 1 / k); sx++) {
                        const mx = sx - bx, my = sy - by;
                        if (mx < 0 || my < 0 || mx >= bw || my >= bh || !mine[my * bw + mx]) continue;
                        for (let c = 0; c < 3; c++) s[c] += src.rgba[(sy * w + sx) * 4 + c];
                        n++;
                    }
                }
                if (n > 0) {
                    for (let c = 0; c < 3; c++) rgb[(ty * TEX + tx) * 3 + c] = s[c] / n;
                    have[ty * TEX + tx] = 1;
                }
            }
        }
        /* its corners where it now is (the game rounds them to a texel: within its border) */
        for (const v of p.verts) {
            UV[v] = [(p.ox + PAD + (UV[v][0] * w - p.x0) * k) / TEX, (p.oy + PAD + (UV[v][1] * h - p.y0) * k) / TEX];
        }
    }
    /* the borders and what's between filled from their neighbours */
    for (let pass = 0; pass < 64; pass++) {
        const next = have.slice();
        let any = false;
        for (let y = 0; y < TEX; y++) {
            for (let x = 0; x < TEX; x++) {
                if (have[y * TEX + x]) continue;
                let n = 0;
                const s = [0, 0, 0];
                for (const [dx, dy] of [[1, 0], [-1, 0], [0, 1], [0, -1]]) {
                    const nx = x + dx, ny = y + dy;
                    if (nx < 0 || ny < 0 || nx >= TEX || ny >= TEX || !have[ny * TEX + nx]) continue;
                    for (let c = 0; c < 3; c++) s[c] += rgb[(ny * TEX + nx) * 3 + c];
                    n++;
                }
                if (n > 0) {
                    for (let c = 0; c < 3; c++) rgb[(y * TEX + x) * 3 + c] = s[c] / n;
                    next[y * TEX + x] = 1;
                    any = true;
                }
            }
        }
        have.set(next);
        if (!any) break;
    }
    console.log(`bobby: his texture's ${pieces.length} pieces laid out again at ${(k * w / TEX).toFixed(2)} of the size they'd be shrunk to whole`);
    return png_write(TEX, TEX, Buffer.from(Array.from(rgb, (v) => Math.round(Math.min(255, Math.max(0, v))))));
}

/* --- his skeleton: glTF's axes, y up, facing +z, his left +x; every bone rests unturned, at its place in his model --- */
const BONES = [];
const bone = (name, parent, at, tip = null) => {
    BONES.push({ name, parent: parent == null ? -1 : BONES.findIndex((b) => b.name === parent), at, tip });
    return BONES.length - 1;
};
bone('hips', null, [0, -0.21, -0.03]);
bone('spine', 'hips', [0, -0.07, 0.0]);
bone('chest', 'spine', [0, 0.07, 0.04]);
bone('neck', 'chest', [0, 0.17, 0.05]);
bone('head', 'neck', [0, 0.24, 0.06], [0, 0.42, 0.06]);
bone('jaw', 'head', [0, 0.27, 0.17], [0, 0.24, 0.34]);
for (const [s, x] of [['l', 1], ['r', -1]]) {
    bone(`ear_${s}1`, 'head', [0.13 * x, 0.38, 0.0]);
    bone(`ear_${s}2`, `ear_${s}1`, [0.17 * x, 0.26, 0.0], [0.19 * x, 0.14, 0.0]);
    bone(`upperarm_${s}`, 'chest', [0.14 * x, 0.12, 0.07]);
    bone(`forearm_${s}`, `upperarm_${s}`, [0.17 * x, 0.02, 0.13]);
    bone(`hand_${s}`, `forearm_${s}`, [0.12 * x, 0.05, 0.23], [0.11 * x, 0.01, 0.33]);
    bone(`thigh_${s}`, 'hips', [0.1 * x, -0.24, -0.03]);
    bone(`shin_${s}`, `thigh_${s}`, [0.12 * x, -0.34, -0.05]);
    bone(`foot_${s}`, `shin_${s}`, [0.12 * x, -0.43, -0.04], [0.12 * x, -0.46, 0.07]);
}
bone('tail1', 'hips', [0, -0.17, -0.14]);
bone('tail2', 'tail1', [0, -0.12, -0.24]);
bone('tail3', 'tail2', [0, -0.06, -0.3], [0, 0.0, -0.335]);
const B = Object.fromEntries(BONES.map((b, i) => [b.name, i]));
/* each bone's line: from it to its first child, or its tip */
for (const b of BONES) {
    const child = BONES.find((c) => c.parent === BONES.indexOf(b));
    b.end = b.tip || (child ? child.at : add(b.at, [0, 0.05, 0]));
}

/* --- his weights: each corner's bones, by the part of him it's in, then smoothed over his surface --- */

/* the bones a corner at p can go with */
function part(p) {
    const [x, y, z] = p, s = x >= 0 ? 'l' : 'r', ax = Math.abs(x);
    if ((ax > 0.105 && y > 0.2 && y < 0.44 && z < 0.2) || (ax > 0.12 && y > 0.13 && y <= 0.2 && z < 0.16)) return [`ear_${s}1`, `ear_${s}2`];
    if (y > 0.19) return z > 0.2 && y < 0.27 ? ['jaw', 'head'] : ['head', 'neck'];
    if (y > -0.1 && y < 0.17 && (z > 0.12 || ax > 0.16)) return [`upperarm_${s}`, `forearm_${s}`, `hand_${s}`];
    if (y < -0.26 && ax > 0.03) return [`thigh_${s}`, `shin_${s}`, `foot_${s}`];
    if (z < -0.12 && y > -0.28 && ax < 0.09) return ['hips', 'tail1', 'tail2', 'tail3'];
    return ['hips', 'spine', 'chest', 'neck'];
}

function seg_dist(p, a, b) {
    const ab = sub(b, a), t = Math.min(1, Math.max(0, (sub(p, a)[0] * ab[0] + sub(p, a)[1] * ab[1] + sub(p, a)[2] * ab[2]) / (ab[0] ** 2 + ab[1] ** 2 + ab[2] ** 2 || 1)));
    return len(sub(p, add(a, mul(ab, t))));
}

function weigh() {
    /* first, by the nearest of its part's bones */
    let W = POS.map((p) => {
        const near = part(p).map((n) => [B[n], 1 / (seg_dist(p, BONES[B[n]].at, BONES[B[n]].end) + 0.01) ** 4]).sort((a, b) => b[1] - a[1]).slice(0, 2);
        const sum = near.reduce((s, [, w]) => s + w, 0);
        return new Map(near.map(([b, w]) => [b, w / sum]));
    });
    /* his surface: corners joined by an edge, or at the same place (across the texture's seams) */
    const key = (p) => p.map((v) => Math.round(v * 1e4)).join(',');
    const same = new Map();
    POS.forEach((p, v) => same.set(key(p), [...(same.get(key(p)) || []), v]));
    const near = POS.map(() => new Set());
    for (const t of tris) for (const a of t) for (const b of t) if (a !== b) near[a].add(b);
    for (const group of same.values()) for (const a of group) for (const b of group) if (a !== b) near[a].add(b);
    for (let pass = 0; pass < 3; pass++) {
        W = W.map((own, v) => {
            const m = new Map();
            for (const [b, w] of own) m.set(b, w * 0.5);
            const n = near[v].size || 1;
            for (const u of near[v]) for (const [b, w] of W[u]) m.set(b, (m.get(b) || 0) + (w * 0.5) / n);
            return m;
        });
    }
    /* the copies of a corner at one place (across the texture's seams) all the same, or they'd come apart as he moves: cracks */
    for (const group of same.values()) {
        const m = new Map();
        for (const v of group) for (const [b, w] of W[v]) m.set(b, (m.get(b) || 0) + w / group.length);
        for (const v of group) W[v] = m;
    }
    return W.map((m) => {
        const l = [...m].sort((a, b) => b[1] - a[1]).slice(0, 4), sum = l.reduce((s, [, w]) => s + w, 0);
        const j = [0, 0, 0, 0], w = [0, 0, 0, 0];
        l.forEach(([b, x], i) => {
            j[i] = b;
            w[i] = x / sum;
        });
        return { j, w };
    });
}
const WEIGHTS = weigh();

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
/* His arms are short against his big head, and rest held forward, begging: one raised over his head (a crate, a cheer) goes up no
 * further than up at his chin, begging as he rests, a little out (up any further his paws went inside his muzzle or ears); a crate he carries balances on his head */
const raised = (up) => smooth(-60, -150, up);
const arm = (a, side) => ({
    up: euler(Math.max(a[0], -40), 0, side * (-(a.out) + (a[1] || 0) - 20 * raised(a[0]))),
    fore: euler((a[2] || 0) * (1 - raised(a[0])) - 25 * raised(a[0]) + 12 * Math.sin(a[0] * 0.2) * raised(a[0]), 0, 0),
});
const arms = (l, r, out = 10) => {
    const L = arm(Object.assign([...l], { out }), 1), R = arm(Object.assign([...r], { out }), -1);
    return { upperarm_l: L.up, forearm_l: L.fore, upperarm_r: R.up, forearm_r: R.fore };
};
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
    /* bytes as a buffer view */
    view(bytes) {
        const pad = (4 - (this.size % 4)) % 4;
        if (pad) {
            this.parts.push(Buffer.alloc(pad));
            this.size += pad;
        }
        this.views.push({ buffer: 0, byteOffset: this.size, byteLength: bytes.length });
        this.parts.push(bytes);
        this.size += bytes.length;
        return this.views.length - 1;
    }
    /* typed data as a buffer view and an accessor */
    add(array, type, componentType, count, extra = {}) {
        const v = this.view(Buffer.from(array.buffer, array.byteOffset, array.byteLength));
        this.accessors.push({ bufferView: v, componentType, count, type, ...extra });
        return this.accessors.length - 1;
    }
}

function glb(png) {
    const bin = new Bin();
    const used = [...new Set(tris.flat())].sort((a, b) => a - b), at = new Map(used.map((v, i) => [v, i]));
    const n = used.length;
    const pos = new Float32Array(n * 3), uv = new Float32Array(n * 2), joints = new Uint16Array(n * 4), weights = new Float32Array(n * 4);
    used.forEach((v, i) => {
        pos.set(POS[v], i * 3);
        uv.set(UV[v], i * 2);
        joints.set(WEIGHTS[v].j, i * 4);
        weights.set(WEIGHTS[v].w, i * 4);
    });
    const lo = [0, 1, 2].map((k) => Math.min(...used.map((v) => POS[v][k]))), hi = [0, 1, 2].map((k) => Math.max(...used.map((v) => POS[v][k])));
    const idx = new Uint16Array(tris.flat().map((v) => at.get(v)));
    const aPos = bin.add(pos, 'VEC3', 5126, n, { min: lo, max: hi });
    const aUv = bin.add(uv, 'VEC2', 5126, n);
    const aJoint = bin.add(joints, 'VEC4', 5123, n);
    const aWeight = bin.add(weights, 'VEC4', 5126, n);
    const aIdx = bin.add(idx, 'SCALAR', 5123, idx.length);
    const image = bin.view(png);
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
        meshes: [{ name: 'bobby', primitives: [{ attributes: { POSITION: aPos, TEXCOORD_0: aUv, JOINTS_0: aJoint, WEIGHTS_0: aWeight }, indices: aIdx, material: 0 }] }],
        materials: [{ name: 'bobby', pbrMetallicRoughness: { baseColorTexture: { index: 0 }, metallicFactor: 0, roughnessFactor: 1 } }],
        textures: [{ source: 0, sampler: 0 }],
        samplers: [{ magFilter: 9728, minFilter: 9728 }],
        images: [{ bufferView: image, mimeType: 'image/png' }],
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
    return { file: Buffer.concat([head, th, text, dh, data]), verts: n };
}

/* --- how he plays: hero.json --- */
const hero = {
    format: 1,
    id: 'bobby',
    name: 'BOBBY',
    /* Crash's place if nobody has it: a small, quick hero */
    prefer: 'crash',
    height: 540,
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
    version: '1.1',
    description: 'Bobby the dachshund as a hero of his own: the example hero pack',
}, null, 2) + '\n');
fs.writeFileSync(path.join(OUT, 'heroes', 'bobby', 'hero.json'), JSON.stringify(hero, null, 2) + '\n');
const made = glb(texture());
fs.writeFileSync(path.join(OUT, 'heroes', 'bobby', 'hero.glb'), made.file);
console.log(`bobby: ${made.verts} vertices, ${tris.length} triangles, ${BONES.length} bones, ${Object.keys(CLIPS).length} clips, ${made.file.length} bytes`);
