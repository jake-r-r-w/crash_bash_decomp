#!/usr/bin/env node
/* The demo mod, Gift Boxes: TNT crates wrapped up as presents, in new art drawn here, so none of it is the game's. It writes
 * port/mods/gift-boxes/: a mod.json and five PNGs, named as the game names the textures they replace (docs/MODDING.md, Textures):
 * TNT's sides, the sides of the 3, 2 and 1 it counts down with once it's hit, and its lid. Those names are the same in every crate
 * arena. Copy the folder into the game's mods folder, or zip it, to use it.
 *
 * The lid is the grey crate's image, which the model tints red for TNT and green for Nitro, and a tint multiplies: TNT's lid shows only
 * the image's red, and Nitro's only its green. So the lid PNG has the gift's lid and bow in its red, for TNT, and a crate lid of its own
 * in its green, for Nitro.
 *
 *   node port/mods/gift_boxes.mjs */
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { fileURLToPath } from 'node:url';

const OUT = path.join(path.dirname(fileURLToPath(import.meta.url)), 'gift-boxes');
const SIZE = 128; /* four times the game's 32 texels */
const SAMPLES = 4; /* each pixel the mean of 4 x 4 */

/* the textures by the names gfx.c gives them (an FNV-1a hash of their pixels, then their palette): the sides with the number each
   shows, and the lid */
const SIDES = { '1a581a1597bc181d': null, '9da5575e3755bd98': 1, '32cb1b002b4ae4c2': 2, '8a933898c87a420d': 3 };
const LID = '07e0cd6021f94c95';

const RED = [205, 32, 48], RED_DARK = [112, 10, 22], RIM = [58, 5, 12], DOT = [252, 222, 222];
const GOLD = [238, 176, 38], GOLD_LIGHT = [255, 232, 140], GOLD_DARK = [150, 98, 12];
const TAG = [255, 249, 236], INK = [150, 12, 30];

const mix = (a, b, t) => a.map((x, i) => x + (b[i] - x) * t);
const smooth = (from, to, x) => {
    const t = Math.min(1, Math.max(0, (x - from) / (to - from)));
    return t * t * (3 - 2 * t);
};

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

function colour(x, y, digit) {
    const edge = Math.min(x, y, SIZE - x, SIZE - y), rx = Math.abs(x - SIZE / 2);
    /* the paper, a little darker towards the box's edges, with its dots */
    let c = mix(RED_DARK, RED, smooth(0, 16, edge));

    c = mix(c, DOT, 0.85 * (1 - smooth(DOT_SIZE - 0.4, DOT_SIZE + 0.4, dot(x, y))));
    /* the ribbon up the middle, lit down its centre, with its shadow on the paper */
    if (rx < 17.5) {
        c = mix(c, RED_DARK, 0.45);
    }
    if (rx < 15) {
        c = rx < 13 ? mix(GOLD_LIGHT, GOLD, smooth(0, 11, rx)) : GOLD_DARK;
    }
    /* the box's corner, a dark line round the edge */
    if (edge < 2) {
        c = mix(c, RIM, 0.85);
    }
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

/* --- the lid: one shade a channel, as a tint keeps only one of them --- */

/* a shape's edge as a dark line: its fill inside, the line along its edge, and what was there outside (d is how far out of it) */
const LINE = 1.3;
const layer = (under, d, fill, line) => (d < -LINE ? fill : d < LINE ? line : under);

/* how far out of an ellipse round c, with half-widths a and b, turned by angle degrees */
function ellipse(x, y, cx, cy, a, b, angle) {
    const t = angle * Math.PI / 180, u = (x - cx) * Math.cos(t) + (y - cy) * Math.sin(t), v = -(x - cx) * Math.sin(t) + (y - cy) * Math.cos(t);

    return (Math.hypot(u / a, v / b) - 1) * Math.min(a, b);
}

/* the gift's lid, for TNT's red: the paper, the ribbon across both ways, and the bow, from 0 to 127 as the tint doubles it */
function gift_lid(x, y) {
    const edge = Math.min(x, y, SIZE - x, SIZE - y), rx = Math.abs(x - SIZE / 2), ry = Math.abs(y - SIZE / 2), band = Math.min(rx, ry);
    const DARK = 52;
    let v = 62 + 26 * smooth(0, 16, edge);

    if (dot(x, y) < DOT_SIZE) {
        v = 112;
    }
    if (band < 17.5) {
        v = 66;
    }
    if (band < 15) {
        v = band < 13 ? 127 - 19 * smooth(0, 11, band) : DARK;
    }
    /* the bow: its two ends, its two loops with their insides showing, and the knot */
    v = layer(v, segment(x, y, 60, 70, 45, 97) - 5, 116, DARK);
    v = layer(v, segment(x, y, 68, 70, 83, 97) - 5, 116, DARK);
    for (const side of [-1, 1]) {
        const cx = 64 + side * 20, cy = 54, d = ellipse(x, y, cx, cy, 20, 12, side * 28);

        v = layer(v, d, 104 + 23 * smooth(8, 20, Math.hypot(x - cx, y - cy)), DARK);
        if (d < 0) {
            v = layer(v, ellipse(x, y, cx - side * 4, cy + 1, 9, 4.5, side * 28), 66, 66);
        }
    }
    const kx = Math.max(Math.abs(x - 64) - 6, 0), ky = Math.max(Math.abs(y - 62) - 7, 0);
    return layer(v, Math.hypot(kx, ky) - 3, 127, DARK);
}

/* a crate's lid, for Nitro's green: planks in a frame, braced corner to corner, from 0 to 141 as the tint takes it to 255 */
function crate_lid(x, y) {
    const edge = Math.min(x, y, SIZE - x, SIZE - y), DARK = 42, WOOD = 96;
    let v = 70 + 6 * Math.sin(y * 0.9 + Math.sin(x * 0.13) * 2);

    if (Math.abs(((y - 4) % 21) - 10.5) > 9.6) {
        v = DARK;
    }
    const brace = Math.min(Math.abs(x - y), Math.abs(x + y - SIZE)) / Math.SQRT2;
    v = layer(v, brace - 7, WOOD, DARK);
    v = layer(v, edge - 12, WOOD, DARK);
    if (edge < 1.5) {
        v = DARK;
    }
    for (const [nx, ny] of [[7, 7], [121, 7], [7, 121], [121, 121], [64, 7], [64, 121], [7, 64], [121, 64]]) {
        if (Math.hypot(x - nx, y - ny) < 2.4) {
            v = 132;
        }
    }
    return v;
}

/* the lid's colour: red for TNT, green for Nitro, and blue between them for anything left untinted */
function lid(x, y) {
    const r = gift_lid(x, y), g = crate_lid(x, y);

    return [r, g, (r + g) / 2];
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
        crc.writeUInt32BE(zlib.crc32(body));
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

fs.mkdirSync(OUT, { recursive: true });
for (const [name, digit] of Object.entries(SIDES)) {
    fs.writeFileSync(path.join(OUT, `${name}.png`), png(SIZE, SIZE, draw((x, y) => colour(x, y, digit))));
}
fs.writeFileSync(path.join(OUT, `${LID}.png`), png(SIZE, SIZE, draw(lid)));
fs.writeFileSync(path.join(OUT, 'mod.json'), `${JSON.stringify({ name: 'Gift Boxes', author: 'Crash Bash PC', version: '1.0', description: 'TNT crates wrapped up as presents, in new art' }, null, 2)}\n`);
console.log(`${path.relative(process.cwd(), OUT)}: mod.json and ${Object.keys(SIDES).length + 1} PNGs`);
