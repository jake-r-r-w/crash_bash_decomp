/* Writes port/platform/paltable.c, the European disc's (SCES-02834) files as the USA game has them: node port/pal_table.mjs
 *
 * The port plays the USA game's C, which reads CRASHBSH.DAT through the USA disc's table of its files. The European disc's archive has
 * the same files, most of them byte for byte, in its own order, with four more languages' text, speech and words drawn in pictures.
 * This lines the two up once, here, where both discs are (disc/ and protos/PAL_retail/), and writes for each USA file where the PAL
 * archive has it and what to do with it: numbers, none of either game's data. platform/paldata.c uses them to make a USA-ordered
 * archive from a player's European disc. docs/PORT.md, The European disc, says how the files were matched.
 *
 *   node port/pal_table.mjs [--usa DAT] [--pal DAT] [--pal-exe SCES_028.34]
 */
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const arg = (name, fallback) => { const i = process.argv.indexOf(name); return i > 0 ? process.argv[i + 1] : fallback; };
const usa = fs.readFileSync(arg('--usa', path.join(ROOT, 'disc/files/CRASHBSH/CRASHBSH.DAT')));
const palDat = fs.readFileSync(arg('--pal', path.join(ROOT, 'protos/PAL_retail/files/CRASHBSH/CRASHBSH.DAT')));
const palExe = fs.readFileSync(arg('--pal-exe', path.join(ROOT, 'protos/PAL_retail/files/SCES_028.34')));
const ENTRIES = 992;

/* the USA table, as the port's copy of the game has it */
const words = fs.readFileSync(path.join(ROOT, 'port/game/main/main.c'), 'utf8').match(/file DatIndex\[[^\]]*\] = \{([^}]*)\}/)[1]
    .replace(/\/\*.*?\*\//gs, '').match(/0x[0-9A-Fa-f]+|\d+/g).map(Number);
const U = e => usa.subarray(words[2 * e] * 2048, words[2 * e] * 2048 + words[2 * e + 1]);
const usaSize = e => words[2 * e + 1];

/* the European table, in its program: it starts as the USA's does (the same first two files at the same places) */
const start = Buffer.alloc(16);
start.writeUInt32LE(0, 0); start.writeUInt32LE(words[1], 4); start.writeUInt32LE(words[2], 8); start.writeUInt32LE(words[3], 12);
const at = palExe.indexOf(start);
if (at < 0) {
    throw new Error("the European program's table of files wasn't found");
}
const pal = [];
for (let i = at; i + 8 <= palExe.length; i += 8) {
    const sector = palExe.readUInt32LE(i), size = palExe.readUInt32LE(i + 4);
    if (sector * 2048 + size > palDat.length || (pal.length > 0 && sector < pal.at(-1)[0])) {
        break;
    }
    pal.push([sector, size]);
}
const P = p => palDat.subarray(pal[p][0] * 2048, pal[p][0] * 2048 + pal[p][1]);
console.log(`European table: ${pal.length} files, at 0x${at.toString(16)} in its program`);

/* 1: the files that are the same, byte for byte */
const key = b => `${b.length}:${b.subarray(0, 64).toString('hex')}`;
const byKey = new Map();
pal.forEach((_, p) => { const k = key(P(p)); if (!byKey.has(k)) byKey.set(k, []); byKey.get(k).push(p); });
const map = new Array(ENTRIES).fill(-1), kind = new Array(ENTRIES).fill('copy');
const palOf = new Array(pal.length).fill(-1);
for (let e = 0; e < ENTRIES; e++) {
    if (usaSize(e) === 0) {
        continue;
    }
    const p = (byKey.get(key(U(e))) ?? []).find(q => P(q).equals(U(e)));
    if (p !== undefined) {
        map[e] = p;
        if (palOf[p] < 0) palOf[p] = e;
    }
}
const same = map.filter(p => p >= 0).length;

/* 2: the rest lined up between them: the longest run of same files in the same order on both, and between two of those, as many
   USA files as European ones are each other's (a texture redrawn, a model with a few numbers changed) */
/* every USA file with every European copy of it (the same small model is in every overlay), a file's copies latest first, so the
   chain takes at most one of them */
const pairs = [];
for (let e = 0; e < ENTRIES; e++) {
    if (map[e] < 0) continue;
    const copies = (byKey.get(key(U(e))) ?? []).filter(q => P(q).equals(U(e)));
    for (const p of copies.sort((a, b) => b - a)) pairs.push([e, p]);
}
const tails = [], tailAt = [], prev = [];
pairs.forEach(([, p], i) => {
    let lo = 0, hi = tails.length;
    while (lo < hi) { const m = (lo + hi) >> 1; if (tails[m] < p) lo = m + 1; else hi = m; }
    tails[lo] = p; tailAt[lo] = i; prev[i] = lo > 0 ? tailAt[lo - 1] : -1;
});
const anchors = [];
for (let i = tailAt[tails.length - 1]; i >= 0; i = prev[i]) anchors.unshift(pairs[i]);
for (const [e, p] of anchors) map[e] = p;
anchors.unshift([-1, -1]);
anchors.push([ENTRIES, pal.length]);
for (let i = 1; i < anchors.length; i++) {
    const [u0, p0] = anchors[i - 1], [u1, p1] = anchors[i];
    if (u1 - u0 === p1 - p0) {
        for (let k = 1; u0 + k < u1; k++) if (map[u0 + k] < 0 && usaSize(u0 + k) > 0) map[u0 + k] = p0 + k;
    }
}

/* 3: where the European disc has more than one: its mode text is in five languages, Spanish, Italian, German, French and English, and
   the English is the last; and two pictures before them, as the USA's */
const text = 0x8D, texts = 12;
const first = map[0x8A] + 1; /* the European file after the USA's 08A, the same on both */
map[0x8B] = first;
map[0x8C] = first + 1;
for (let k = 0; k < texts; k++) map[text + k] = first + 2 + 4 * texts + k;
const english = P(map[text]).toString('latin1');
if (!english.includes('OBJECTIVE')) {
    throw new Error("the European disc's last set of mode text isn't the English");
}

/* 4: one overlay's files are in another order there: its interface picture and model are after its others, and it has no font of its
   own: any of the others' does, as every copy is the same */
const used = new Set(map.filter(p => p >= 0));
const loose = pal.map((_, p) => p).filter(p => !used.has(p) && pal[p][1] === pal[map[0x138]][1] && P(p).equals(P(map[0x138])));
if (loose.length !== 1) {
    throw new Error(`the overlay's interface picture: ${loose.length} European ones left over, not 1`);
}
map[0x2DB] = loose[0];
map[0x2DC] = loose[0] + 1;

/* the game's code (the overlays and the engine) is the port's own C: those files are never read, and have none here */
const CODE = [0x1E7, ...Array.from({ length: 13 }, (_, i) => 0x2CC + i), 0x3DF];
for (const e of CODE) map[e] = -1;

/* the font and the interface, whose pictures the code picks by number: in the USA's order (below) */
for (let e = 0; e < ENTRIES; e++) {
    if (usaSize(e) === usaSize(0x13A) && U(e).equals(U(0x13A))) kind[e] = 'font';
    if (usaSize(e) === usaSize(0x138) && U(e).equals(U(0x138))) kind[e] = 'interface';
}
const fontPal = map[0x13A];
if (map[0x2DD] < 0) map[0x2DD] = fontPal;

/* a texture set's pictures, each by its colours, so the same picture with its colours in other places is still the same */
function pictures(b) {
    const n = b.readUInt16LE(8), cluts = [];
    let p = 0x20;
    for (let i = 0; i < b.readUInt16LE(10); i++) {
        const k = b.readUInt32LE(p); const c = [];
        for (let j = 0; j < k; j++) c.push(b.readUInt16LE(p + 4 + 2 * j));
        cluts.push(c); p += 4 + 2 * k;
    }
    const out = [];
    for (let i = 0; i < n; i++) {
        const w = b.readUInt16LE(p), h = b.readUInt16LE(p + 2), f = b.readUInt16LE(p + 12), clut = cluts[f >> 1] ?? [], px = b.subarray(p + 20, p + 20 + w * h * 2);
        const cols = [];
        if (f & 1) for (const v of px) cols.push(clut[v]); else for (const v of px) cols.push(clut[v & 15], clut[v >> 4]);
        out.push({ w, h, key: `${w}x${h}:${cols.join(',')}` });
        p += 20 + w * h * 2;
    }
    return out;
}

/* the font: each USA glyph is in the European font, which has the accented letters too, so every USA glyph's number finds its own */
const fu = pictures(U(0x13A)), fp = pictures(P(fontPal));
const fontOrder = fu.map(g => fp.findIndex(q => q.key === g.key));
if (fontOrder.some(j => j < 0)) {
    throw new Error(`a USA glyph isn't in the European font: ${fontOrder.indexOf(-1)}`);
}

/* the interface: the same pictures to 33; the USA's four player tags (34 to 37) aren't in the European one, which draws them another
   way, and its frame and PAUSED and RULES pieces follow at 34, redrawn a little, as the USA's 38 to 45 */
const iu = pictures(U(0x138)), ip = pictures(P(map[0x138]));
const ifaceOrder = iu.map((g, i) => i < 34 ? i : i < 38 ? -1 : i - 4);
ifaceOrder.forEach((j, i) => {
    if (j >= 0 && (ip[j].w !== iu[i].w || ip[j].h !== iu[i].h)) throw new Error(`interface picture ${i} isn't the European ${j}'s size`);
    if (i < 34 && ip[i].key !== iu[i].key) throw new Error(`interface picture ${i} isn't the European's`);
});
const tagSize = [iu[34].w, iu[34].h];

const left = [];
for (let e = 0; e < ENTRIES; e++) if (map[e] < 0 && usaSize(e) > 0) left.push(e);
for (const e of left) kind[e] = 'none';
console.log(`${same} the same, ${map.filter(p => p >= 0).length - same} the European disc's own, ${left.length} with none: ${left.map(e => e.toString(16)).join(' ')}`);

const crc = zlib.crc32(palDat) >>> 0;
const kinds = { copy: 'PAL_COPY', font: 'PAL_FONT', interface: 'PAL_INTERFACE', none: 'PAL_NONE' };
let c = `/* The European disc's (SCES-02834) files as the USA game has them: made by port/pal_table.mjs from both discs, numbers only. Each USA
 * file's place and size in the European CRASHBSH.DAT, and what's done with it (platform/paldata.c; docs/PORT.md, The European disc). */
#include "paldata.h"

const unsigned PalDatSize = ${palDat.length}u;
const unsigned PalDatCrc = 0x${crc.toString(16).toUpperCase().padStart(8, '0')}u;

const PalFile PalFiles[PAL_FILES] = {
`;
for (let e = 0; e < ENTRIES; e += 2) {
    const one = k => map[k] >= 0 ? `{ ${pal[map[k]][0]}, ${pal[map[k]][1]}, ${kinds[kind[k]]} }` : `{ 0, 0, PAL_NONE }`;
    c += `    ${one(e)}, ${one(e + 1)}, /* ${e.toString(16).toUpperCase().padStart(3, '0')} */\n`;
}
c += `};

/* the USA font's glyphs, each the European font's picture of the same letter: the rest of the European ones follow */
const short PalFontOrder[PAL_FONT_GLYPHS] = {
${fontOrder.map(String).join(', ').replace(/(.{1,126})(, |$)/g, '    $1,\n').replace(/,,\n$/, ',\n')}};

/* the USA interface's pictures, each the European interface's: -1 the USA's player tags, which it hasn't (a clear picture this size) */
const short PalInterfaceOrder[PAL_INTERFACE_PICTURES] = {
${ifaceOrder.map(String).join(', ').replace(/(.{1,126})(, |$)/g, '    $1,\n').replace(/,,\n$/, ',\n')}};
const unsigned short PalTagSize[2] = { ${tagSize[0]}, ${tagSize[1]} };
`;
fs.writeFileSync(path.join(ROOT, 'port/platform/paltable.c'), c);
console.log(`port/platform/paltable.c: ${ENTRIES} files, the font's ${fontOrder.length} glyphs, the interface's ${ifaceOrder.length} pictures; European CRASHBSH.DAT ${palDat.length} bytes, CRC ${crc.toString(16)}`);
