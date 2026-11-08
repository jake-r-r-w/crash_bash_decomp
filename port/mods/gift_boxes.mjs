#!/usr/bin/env node
/* The demo mod, Gift Boxes: TNT crates wrapped up as presents, in new art drawn here, so none of it is the game's. It writes
 * port/mods/gift-boxes/: a mod.json, five PNGs named as the game names the textures they replace (docs/MODDING.md, Textures), and
 * patches to the crate arenas' files. Copy the folder into the game's mods folder, or zip it, to use it.
 *
 * The PNGs are TNT's sides, the sides of the 3, 2 and 1 it counts down with once it's hit, which are the same in every crate arena,
 * and its lid. The game's lid is the grey crate's image, which the model tints red for TNT and green for Nitro; a tint multiplies, so
 * nothing on it could be gold. So each arena's crates get patches (port/mods/bps.mjs): its texture set gains an image of the mod's own
 * for the lid, and its model points TNT's lid at it, untinted, as its sides are. Nitro keeps its own lid. A patch holds only what it
 * changes, and the PNGs are all that's drawn of the new image, so neither has any of the game's art. Making the patches needs your own
 * CRASHBSH.DAT, as they're made against it.
 *
 *   node port/mods/gift_boxes.mjs [--data CRASHBSH.DAT] */
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { fileURLToPath } from 'node:url';
import { bps_make, crc32 } from './bps.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const OUT = path.join(HERE, 'gift-boxes');
const SIZE = 128; /* four times the game's 32 texels */
const SAMPLES = 4; /* each pixel the mean of 4 x 4 */

/* the textures by the names gfx.c gives them (an FNV-1a hash of their pixels, then their palette), with the number each shows */
const SIDES = { '1a581a1597bc181d': null, '9da5575e3755bd98': 1, '32cb1b002b4ae4c2': 2, '8a933898c87a420d': 3 };

const RED = [205, 32, 48], RED_DARK = [112, 10, 22], RIM = [58, 5, 12], DOT = [252, 222, 222];
const GOLD = [238, 176, 38], GOLD_LIGHT = [255, 232, 140], GOLD_DARK = [150, 98, 12], GOLD_LINE = [104, 62, 6];
const TAG = [255, 249, 236], INK = [150, 12, 30];

const mix = (a, b, t) => a.map((x, i) => x + (b[i] - x) * t);
const smooth = (from, to, x) => {
    const t = Math.min(1, Math.max(0, (x - from) / (to - from)));
    return t * t * (3 - 2 * t);
};

/* --- the art --- */

/* how far a point is from the segment a b */
function segment(px, py, ax, ay, bx, by) {
    const dx = bx - ax, dy = by - ay, t = Math.max(0, Math.min(1, ((px - ax) * dx + (py - ay) * dy) / (dx * dx + dy * dy)));
    return Math.hypot(px - ax - t * dx, py - ay - t * dy);
}

/* and from the arc round c of radius r, from angle a0 clockwise to a1 (degrees, with y down) */
function arc(px, py, cx, cy, r, a0, a1) {
    const angle = Math.atan2(py - cy, px - cx) * 180 / Math.PI;
    const span = ((a1 - a0) % 360 + 360) % 360, at = ((angle - a0) % 360 + 360) % 360;
    const end = a => [cx + r * Math.cos(a * Math.PI / 180), cy + r * Math.sin(a * Math.PI / 180)];

    if (at <= span) {
        return Math.abs(Math.hypot(px - cx, py - cy) - r);
    }
    return Math.min(Math.hypot(px - end(a0)[0], py - end(a0)[1]), Math.hypot(px - end(a1)[0], py - end(a1)[1]));
}

/* how far out of an ellipse round c, with half-widths a and b, turned by angle degrees */
function ellipse(x, y, cx, cy, a, b, angle) {
    const t = angle * Math.PI / 180, u = (x - cx) * Math.cos(t) + (y - cy) * Math.sin(t), v = -(x - cx) * Math.sin(t) + (y - cy) * Math.cos(t);

    return (Math.hypot(u / a, v / b) - 1) * Math.min(a, b);
}

/* the numbers, as strokes round their middle lines */
const DIGITS = {
    1: (x, y) => Math.min(segment(x, y, 66, 46, 66, 82), segment(x, y, 66, 46, 56, 54), segment(x, y, 55, 82, 77, 82)),
    2: (x, y) => Math.min(arc(x, y, 64, 56, 11, 180, 30), segment(x, y, 73.5, 61.5, 54, 82), segment(x, y, 54, 82, 76, 82)),
    3: (x, y) => Math.min(arc(x, y, 63, 55, 10, 200, 100), arc(x, y, 63, 73, 11, 260, 150)),
};
const STROKE = 4.6;

/* how far a point is from the nearest polka dot's middle: two columns each side of the ribbon, the sides mirrored so the ribbon cuts
   none, the columns 32 apart down and half a step from each other */
function dot(x, y) {
    const mx = x < SIZE / 2 ? x : SIZE - x, inner = mx >= 23, t = (((y - (inner ? 0 : 16)) % 32) + 32) % 32;

    return Math.hypot(mx - (inner ? 35 : 11), Math.min(t, 32 - t));
}
const DOT_SIZE = 8.5;

/* the paper, darker towards the box's edges, with its dots, and the ribbon where band (how far from its middle line) says */
function paper(x, y, band) {
    const edge = Math.min(x, y, SIZE - x, SIZE - y);
    let c = mix(RED_DARK, RED, smooth(0, 16, edge));

    c = mix(c, DOT, 0.85 * (1 - smooth(DOT_SIZE - 0.4, DOT_SIZE + 0.4, dot(x, y))));
    /* the ribbon, lit down its middle, with its shadow on the paper */
    if (band < 17.5) {
        c = mix(c, RED_DARK, 0.45);
    }
    if (band < 15) {
        c = band < 13 ? mix(GOLD_LIGHT, GOLD, smooth(0, 11, band)) : GOLD_DARK;
    }
    /* the box's corner, a dark line round the edge */
    if (edge < 2) {
        c = mix(c, RIM, 0.85);
    }
    return c;
}

/* a side: the ribbon up the middle, and the number on a tag if it has one */
function side(x, y, digit) {
    let c = paper(x, y, Math.abs(x - SIZE / 2));

    if (digit != null) {
        const d = Math.hypot(x - SIZE / 2, y - SIZE / 2);

        if (d < 31) {
            c = d < 28 ? TAG : GOLD_DARK;
        }
        if (DIGITS[digit](x, y) < STROKE) {
            c = INK;
        }
    }
    return c;
}

/* a shape with a line along its edge: its fill inside, the line, and what was there outside (d is how far out of it) */
const LINE = 1.3;
const layer = (under, d, fill, line) => (d < -LINE ? fill : d < LINE ? line : under);

/* the lid: the ribbon across both ways, and a bow of it */
function lid(x, y) {
    let c = paper(x, y, Math.min(Math.abs(x - SIZE / 2), Math.abs(y - SIZE / 2)));

    /* its two ends, its two loops with their insides showing, and the knot */
    c = layer(c, segment(x, y, 60, 70, 45, 97) - 5, GOLD, GOLD_LINE);
    c = layer(c, segment(x, y, 68, 70, 83, 97) - 5, GOLD, GOLD_LINE);
    for (const side of [-1, 1]) {
        const cx = 64 + side * 20, cy = 54, d = ellipse(x, y, cx, cy, 20, 12, side * 28);

        c = layer(c, d, mix(GOLD, GOLD_LIGHT, smooth(8, 20, Math.hypot(x - cx, y - cy))), GOLD_LINE);
        if (d < 0) {
            c = layer(c, ellipse(x, y, cx - side * 4, cy + 1, 9, 4.5, side * 28), GOLD_DARK, GOLD_DARK);
        }
    }
    const kx = Math.max(Math.abs(x - 64) - 6, 0), ky = Math.max(Math.abs(y - 62) - 7, 0);
    return layer(c, Math.hypot(kx, ky) - 3, GOLD_LIGHT, GOLD_LINE);
}

function draw(paint) {
    const rgba = Buffer.alloc(SIZE * SIZE * 4);

    for (let y = 0; y < SIZE; y++) {
        for (let x = 0; x < SIZE; x++) {
            let sum = [0, 0, 0];

            for (let sy = 0; sy < SAMPLES; sy++) {
                for (let sx = 0; sx < SAMPLES; sx++) {
                    const c = paint(x + (sx + 0.5) / SAMPLES, y + (sy + 0.5) / SAMPLES);
                    sum = sum.map((v, i) => v + c[i]);
                }
            }
            sum.forEach((v, i) => { rgba[(y * SIZE + x) * 4 + i] = Math.round(v / (SAMPLES * SAMPLES)); });
            rgba[(y * SIZE + x) * 4 + 3] = 255;
        }
    }
    return rgba;
}

function png(w, h, rgba) {
    const chunk = (type, data) => {
        const body = Buffer.concat([Buffer.from(type, 'latin1'), data]), len = Buffer.alloc(4), crc = Buffer.alloc(4);
        len.writeUInt32BE(data.length);
        crc.writeUInt32BE(crc32(body));
        return Buffer.concat([len, body, crc]);
    };
    const raw = Buffer.alloc((w * 4 + 1) * h), head = Buffer.alloc(13);

    for (let y = 0; y < h; y++) {
        rgba.copy(raw, y * (w * 4 + 1) + 1, y * w * 4, (y + 1) * w * 4);
    }
    head.writeUInt32BE(w, 0);
    head.writeUInt32BE(h, 4);
    head[8] = 8; /* bits a channel */
    head[9] = 6; /* RGBA */
    return Buffer.concat([Buffer.from([0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A]), chunk('IHDR', head), chunk('IDAT', zlib.deflateSync(raw, { level: 9 })), chunk('IEND', Buffer.alloc(0))]);
}

/* --- the archive (docs/ASSETS.md) --- */

/* the game's data, and where each of its entries is, from the port's copy of the game's table */
function archive(data) {
    const places = [data, path.join(HERE, '../../build/disc/files/CRASHBSH/CRASHBSH.DAT'), path.join(process.env.APPDATA ?? '', 'CrashBash/CrashBash/CRASHBSH.DAT')];
    const file = places.find(p => p != null && fs.existsSync(p));

    if (file == null) {
        throw new Error('no CRASHBSH.DAT: give its path with --data');
    }
    const dat = fs.readFileSync(file);
    const table = fs.readFileSync(path.join(HERE, '../game/main/main.c'), 'utf8').match(/file DatIndex\[\d+\] = \{([^}]*)\}/)[1];
    const words = table.replace(/\/\*.*?\*\//gs, '').match(/0x[0-9A-Fa-f]+|\d+/g).map(Number);

    return id => Buffer.from(dat.subarray(words[id * 2] * 2048, words[id * 2] * 2048 + words[id * 2 + 1]));
}

const fnv = (bytes, h = 0xCBF29CE484222325n) => {
    for (const x of bytes) {
        h = ((h ^ BigInt(x)) * 0x100000001B3n) & 0xFFFFFFFFFFFFFFFFn;
    }
    return h;
};

/* a texture set's CLUTs and images: where each image's header is, its CLUT and its pixels */
function read_set(b) {
    if (b.length < 0x20 || b.readUInt32LE(0) !== 8 || b.readUInt32LE(4) !== b.length) {
        return null;
    }
    const cluts = [], images = [];
    let at = 0x20;

    for (let k = 0; k < b.readUInt16LE(10); k++) {
        cluts.push(at + 4);
        at += 4 + b.readUInt32LE(at) * 2;
    }
    at = 0x0C + b.readUInt32LE(0x0C);
    for (let k = 0; k < b.readUInt16LE(8); k++) {
        const bytes = b.readUInt16LE(at) * b.readUInt16LE(at + 2) * 2;

        images.push({ at, clut: b.readUInt16LE(at + 12) >> 1, pixels: b.subarray(at + 20, at + 20 + bytes) });
        at += 20 + bytes;
    }
    return { cluts, images, end: at };
}

/* a model file's meshes: its list, and its map directory's placed objects (docs/ASSETS.md, Model files) */
function mesh_headers(b) {
    const heads = [], dir = 0x1C + b.readInt32LE(0x1C), chunks = 0x3C + b.readInt32LE(0x3C);
    const fits = h => h > 0 && h + 0x34 <= b.length && [0x14, 0x1C, 0x20].every(o => { const p = h + o + b.readInt32LE(h + o); return p > 0 && p < b.length; });

    for (let m = 0; m < b.readUInt32LE(0x54); m++) {
        heads.push(0x58 + m * 0x34);
    }
    for (let k = 0; dir > 0x1C && dir + 4 <= b.length && k < b.readInt32LE(dir); k++) {
        const at = dir + 4 + k * 12, block = (at + b.readInt32LE(at + 4) - (chunks + 8)) / 16;

        if (b.readInt32LE(at) !== 0 && Number.isInteger(block) && block >= 0 && block <= b.readInt32LE(chunks)) {
            const h = (block === 0 ? 0 : b.readUInt32LE(chunks + 4 + block * 16)) + b.readInt32LE(at);

            if (!heads.includes(h)) {
                heads.push(h);
            }
        }
    }
    return heads.filter(fits);
}

/* each mesh's faces: the texture run each is in (where it is, and its image) and its first colour */
function faces_of(b, h) {
    const out = [], faces = b.readInt16LE(h + 8), ci = h + 0x20 + b.readInt32LE(h + 0x20);
    let strip = h + 0x14 + b.readInt32LE(h + 0x14), runs = h + 0x1C + b.readInt32LE(h + 0x1C), left = 0, run = -1, f = 0;

    for (; f < faces && b.readUInt8(strip + 1) !== 0xFF; strip += 2) {
        for (let n = b.readUInt8(strip + 1); n > 0 && f < faces; n--, f++) {
            if (--left < 0) {
                run = runs;
                runs += 2;
                left = (b.readUInt16LE(run) >> 9) & 0x3F;
            }
            const t = b.readUInt16LE(run);
            out.push({ run, image: b.readUInt8(strip) & 1 || t & 0x8000 ? -1 : t & 0x1FF, colour: b.readUInt16LE(ci + f * 2) & 0x1FFF });
        }
    }
    return out;
}

/* --- the lid's own image --- */

/* its pixels, 4 bits each in the grey crate's CLUT, two to a byte with the left one low: a grey gift's lid, for anything that has the
   patches without the PNG (the CLUT's first nine colours are black, so it's drawn in the rest) */
function lid_pixels() {
    const out = Buffer.alloc(32 * 16);

    for (let y = 0; y < 32; y++) {
        for (let x = 0; x < 32; x++) {
            const band = Math.min(Math.abs(x - 15.5), Math.abs(y - 15.5)), index = band < 3 ? 15 : band < 4 ? 9 : 11;

            out[y * 16 + (x >> 1)] |= x & 1 ? index << 4 : index;
        }
    }
    return out;
}

/* A crate arena's set and model with TNT's lid, and the lids of the 3, 2 and 1 it turns into, on an image of its own, untinted; or null
   if they've no TNT. And the new image's name. tnt is the pixels of those crates' sides, and lids those of the images their lids are
   on: the grey crate's for TNT, and the wooden crate's for the 3, 2 and 1. */
function give_lid(set_bytes, model_bytes, tnt, lids_on) {
    const set = read_set(set_bytes), add = Buffer.alloc(20 + 512), has = list => im => list.some(p => im.pixels.equals(p));
    const g = set?.images.findIndex(has([lids_on[0]])) ?? -1;
    const sides = new Set((set?.images ?? []).map((im, k) => (has(tnt)(im) ? k : -1)).filter(k => k >= 0));
    const lid_images = new Set((set?.images ?? []).map((im, k) => (has(lids_on)(im) ? k : -1)).filter(k => k >= 0));

    if (set == null || g < 0 || sides.size === 0) {
        return null;
    }
    /* the new image after the last, with the grey crate's header and the CLUT it uses: everything after it moves on */
    const n = set.images.length, pixels = lid_pixels(), at = set.end;

    set_bytes.copy(add, 0, set.images[g].at, set.images[g].at + 20);
    add.writeUInt16LE(0, 14);    /* flags: none */
    add.writeInt32LE(1, 16);     /* frames */
    pixels.copy(add, 20);
    const out = Buffer.concat([set_bytes.subarray(0, at), add, set_bytes.subarray(at)]);
    out.writeUInt32LE(out.length, 4);
    out.writeUInt16LE(n + 1, 8);
    if (out.readUInt32LE(0x18) !== 0) {
        out.writeUInt32LE(out.readUInt32LE(0x18) + add.length, 0x18);
    }
    const clut = set.cluts[set.images[g].clut];
    const name = fnv(set_bytes.subarray(clut, clut + 32), fnv(pixels)).toString(16).padStart(16, '0');

    /* in the model: a lid is the faces on a lid's image in a mesh with those sides. Their runs take the new image, and their colours,
       where nothing else has them, those of the sides */
    const model = Buffer.from(model_bytes), uses = new Map(), lids = new Set();
    let tnt_meshes = 0;
    for (const h of mesh_headers(model)) {
        const faces = faces_of(model, h), is_tnt = faces.some(f => sides.has(f.image));

        tnt_meshes += is_tnt;
        for (const f of faces) {
            const mine = is_tnt && lid_images.has(f.image);

            for (let k = f.colour; k < f.colour + 3; k++) {
                uses.set(k, (uses.get(k) ?? 0) | (mine ? 1 : 2));
            }
            if (mine) {
                lids.add(f.run);
            }
        }
    }
    if (tnt_meshes === 0 || lids.size === 0) {
        return null;
    }
    for (const run of lids) {
        model.writeUInt16LE((model.readUInt16LE(run) & ~0x1FF) | n, run);
    }
    const colours = 0x20 + model.readInt32LE(0x20);
    const side_colour = 0x7B7B7B;
    for (const [k, who] of uses) {
        if (who === 1) {
            const w = model.readUInt32LE(colours + k * 4);
            model.writeUInt32LE(((w & 0xFF000000) | side_colour) >>> 0, colours + k * 4);
        }
    }
    return { set: out, model, name, lids: lids.size, tnt_meshes };
}

/* --- the mod --- */

const args = process.argv.slice(2), read = archive(args[args.indexOf('--data') + 1] ?? null);
/* 252's crates: TNT's sides, one picture in every language but Japanese, whose TNT is a bomb and whose mesh shares the others' lid
   colour, and the sides of the 1, 2 and 3; and the grey crate and the wooden crate, TNT's lid and theirs */
const crates = read_set(read(0x252)).images.map(im => im.pixels);
const tnt = [9, 10, 16, 18, 19].map(k => crates[k]), lids_on = [7, 17].map(k => crates[k]);
const patches = [];
let lid_name = null;

fs.rmSync(OUT, { recursive: true, force: true });
fs.mkdirSync(path.join(OUT, 'files'), { recursive: true });
for (let id = 0; id < 992; id++) {
    const set = read(id), model = read(id + 1);

    if (read_set(set) == null || model.length < 0x58 || model.readUInt32LE(0) !== 0x0C160029) {
        continue;
    }
    const made = give_lid(set, model, tnt, lids_on);
    if (made == null) {
        continue;
    }
    if (lid_name != null && made.name !== lid_name) {
        throw new Error(`the lid's image is named ${made.name} in ${id.toString(16)}, not ${lid_name}`);
    }
    lid_name = made.name;
    for (const [entry, before, after] of [[id, set, made.set], [id + 1, model, made.model]]) {
        const { patch, own } = bps_make(before, after);
        const file = `${entry.toString(16).toUpperCase().padStart(3, '0')}.bps`;

        fs.writeFileSync(path.join(OUT, 'files', file), patch);
        patches.push(`${file} ${patch.length} bytes (${own} new)`);
    }
}
if (lid_name == null) {
    throw new Error('no crate arena with TNT in this CRASHBSH.DAT');
}
for (const [name, digit] of Object.entries(SIDES)) {
    fs.writeFileSync(path.join(OUT, `${name}.png`), png(SIZE, SIZE, draw((x, y) => side(x, y, digit))));
}
fs.writeFileSync(path.join(OUT, `${lid_name}.png`), png(SIZE, SIZE, draw(lid)));
fs.writeFileSync(path.join(OUT, 'mod.json'), `${JSON.stringify({ name: 'Gift Boxes', author: 'Crash Bash PC', version: '1.0', description: 'TNT crates wrapped up as presents, in new art' }, null, 2)}\n`);
console.log(`${path.relative(process.cwd(), OUT)}: mod.json, ${Object.keys(SIDES).length + 1} PNGs (the lid ${lid_name}) and ${patches.length} patches:\n  ${patches.join('\n  ')}`);
