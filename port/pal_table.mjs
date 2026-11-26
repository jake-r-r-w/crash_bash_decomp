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
        out.push({ w, h, flags: b.readUInt16LE(p + 14), frames: b.readInt32LE(p + 16), key: `${w}x${h}:${cols.join(',')}` });
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

/* Any other texture set the European disc has its pictures of in another order: the main menu's (082), with the hero select's portraits,
   has more pictures before them. The game picks them by number, so it's read in the USA's order (archive.c: put right as the archive
   opens, so data made before this is too). A USA picture the European one hasn't is the European's of its size in the same place (a
   piece of a word, redrawn). One file only, which is what the table has room for. */
const isSet = b => b.length >= 0x20 && b.readUInt32LE(0) === 8 && b.readUInt32LE(4) === b.length && b.readUInt32LE(0x18) === 0;
let picsOrder = null, picsFrames = null, picsLanguage = null;
for (let e = 0; e < ENTRIES; e++) {
    if (kind[e] !== 'copy' || map[e] < 0 || usaSize(e) === 0 || !isSet(U(e)) || !isSet(P(map[e])) || U(e).equals(P(map[e]))) continue;
    const pu = pictures(U(e)), pp = pictures(P(map[e]));
    const order = pu.map(g => pp.findIndex(q => q.key === g.key));
    if (order.every((j, i) => j === i)) continue;
    order.forEach((j, i) => {
        if (j < 0 && i < pp.length && pp[i].w === pu[i].w && pp[i].h === pu[i].h && !order.includes(i)) order[i] = i;
        if (order[i] < 0) throw new Error(`file ${e.toString(16)}: its picture ${i} isn't in the European one`);
    });
    /* a picture with frames (flags 2: it and the next ones, frames of them; the hero select's player tags, P1 and 1P) has one for
       each language after them on the European disc: its frames are the USA's, and those follow the rest */
    order.forEach((j, i) => {
        if (pp[j].flags !== pu[i].flags) throw new Error(`file ${e.toString(16)}: its picture ${i}'s flags aren't the European's`);
    });
    picsFrames = order.map((j, i) => pp[j].frames !== pu[i].frames ? pu[i].frames : 0);
    /* and a picture whose European frames are the USA's and then one for each language: where the English one is, for the machine's
       language's in place of its first (the tags are P1 in English, J1 in French...) */
    picsLanguage = order.map((j, i) => pu[i].frames > 0 && pp[j].frames === pu[i].frames + 5 ? j + pu[i].frames : -1);
    if (picsOrder !== null && (picsOrder.length !== order.length || picsOrder.some((j, i) => j !== order[i]))) {
        throw new Error(`file ${e.toString(16)}: a second texture set in another order`);
    }
    picsOrder = order;
    kind[e] = 'pictures';
    console.log(`file ${e.toString(16)}: its ${pu.length} pictures in the USA's order, of the European's ${pp.length}`);
}
if (picsOrder === null) throw new Error("the main menu's pictures are in the same order: this expects them not to be");

const left = [];
for (let e = 0; e < ENTRIES; e++) if (map[e] < 0 && usaSize(e) > 0) left.push(e);
for (const e of left) kind[e] = 'none';
console.log(`${same} the same, ${map.filter(p => p >= 0).length - same} the European disc's own, ${left.length} with none: ${left.map(e => e.toString(16)).join(' ')}`);

/* The other four languages (docs/PORT.md, The European disc, Languages): each of these USA files has a version for each language on the
   European disc, in sets in the order Spanish, Italian, German, French, English (data), where the game's code has them English, French,
   German, Italian, Spanish (the languages here are the code's: 1 French to 4 Spanish). The English is the one mapped above, last in its
   set, so the others are a set's length before it, one set each: the mode text (12), the cutscene speech (27), and the heroes' voices
   with the sounds after them (51). */
const LANG_NAMES = ['English', 'French', 'German', 'Italian', 'Spanish'];
const langGroups = [
    { name: 'mode text', entries: Array.from({ length: 12 }, (_, k) => 0x8D + k), stride: 12 },
    { name: 'cutscene speech', entries: Array.from({ length: 27 }, (_, k) => 0xF5 + k), stride: 27 },
    { name: 'voices and sounds', entries: [...Array.from({ length: 44 }, (_, k) => 0xB6 + k), ...Array.from({ length: 7 }, (_, k) => 0x110 + k)], stride: 51 },
];
const langFiles = [];
for (const g of langGroups) {
    let kept = 0;
    for (const e of g.entries) {
        const en = map[e];
        if (en < 0) throw new Error(`${g.name}: USA ${e.toString(16)} has no English`);
        const v = [1, 2, 3, 4].map(L => en - L * g.stride); /* French a set before the English, German two, Italian three, Spanish four */
        for (const [i, p] of v.entries()) {
            /* the same kind of file: a mode text's first word is its number, 12 more a set before; anything else's starts the same */
            const head = g.name === 'mode text' ? P(p).readUInt32LE(0) === P(en).readUInt32LE(0) + 12 * (i + 1)
                                                : P(p).subarray(0, 4).compare(P(en).subarray(0, 4)) === 0;
            if (p < 0 || !head || pal[p][1] > pal[en][1] * 2 || pal[p][1] * 2 < pal[en][1]) {
                throw new Error(`${g.name}: USA ${e.toString(16)}'s language version ${p} isn't the same kind of file as its English ${en}`);
            }
        }
        if (v.every(p => P(p).equals(P(en)))) continue; /* the same in every language */
        langFiles.push({ e, v, sound: g.name !== 'mode text' });
        kept++;
    }
    console.log(`${g.name}: ${kept} of ${g.entries.length} differ by language`);
}
/* the mode text's languages, by their first words: the French says OBJECTIF */
if (!P(langFiles.find(f => f.e === text).v[0]).toString('latin1').includes('OBJECTIF')) {
    throw new Error("the mode text's language a set before the English isn't the French");
}

/* The interface's words drawn as pictures (the pause frame's PAUSED and RULES): the European disc has the interface once more for each
   other language, the same pictures but those, in the order Spanish, Italian, German, French, before the first of the English's copies.
   They're the interface's (0x138's) language versions: every interface file the game reads is drawn with the language's (archive.c). */
{
    const en = pictures(P(map[0x138])), frame = en[34].key;
    const others = pal.map((_, p) => p).filter(p => {
        if (p === map[0x138] || pal[p][1] !== pal[map[0x138]][1] || P(p).equals(P(map[0x138]))) return false;
        const o = pictures(P(p));
        return o.length === en.length && o[34].key === frame && o.every((q, i) => q.w === en[i].w && q.h === en[i].h) && o.some((q, i) => q.key !== en[i].key);
    });
    const distinct = others.filter((p, i) => !others.slice(0, i).some(q => P(q).equals(P(p))));
    if (distinct.length !== 4) throw new Error(`the interface's languages: ${distinct.length} found, not 4`);
    /* the disc's order is Spanish, Italian, German, French: the code's French first */
    langFiles.push({ e: 0x138, v: [distinct[3], distinct[2], distinct[1], distinct[0]], words: true });
    console.log(`the interface's words: its languages the European files ${distinct.join(' ')}`);
}

/* The European font's letters past the USA's: the accented ones, each code a glyph, as its program's InitSysFont sets them (a run of li
   reg, 0x30NN; sh reg, code * 2 (base)); in the converted font's order, the USA's glyphs first and the European's others after. Its last
   four stores on each page are the button icons, which the USA has at 0xFC to 0xFF too, and keep. And each one's flags (sb reg, code
   (flags' base)): 4 for a letter with its accent above, which the European DrawSysText draws higher (nascalls.c). */
function fontStores(exe, aPage0, aPage1, flagsOut) {
    const regs = new Map(), rec = [], bytes = [];
    for (let i = 0x800; i + 4 <= exe.length; i += 4) {
        const w = exe.readUInt32LE(i), op = w >>> 26, rs = (w >> 21) & 31, rt = (w >> 16) & 31, rd = (w >> 11) & 31, imm = w & 0xFFFF;
        if ((op === 9 || op === 13) && rs === 0) { regs.set(rt, imm); continue; }
        /* a move (addu rd, rs, zero) */
        if (op === 0 && (w & 63) === 33 && rt === 0) { if (regs.has(rs)) regs.set(rd, regs.get(rs)); else regs.delete(rd); continue; }
        if (op === 41 && rt !== 0 && regs.has(rt)) { rec.push({ at: i, base: rs, off: imm << 16 >> 16, v: regs.get(rt) }); continue; }
        if (op === 40 && rt !== 0 && regs.has(rt)) { bytes.push({ at: i, base: rs, off: imm << 16 >> 16, v: regs.get(rt) }); continue; }
        if (op === 2 || op === 3 || (op === 0 && (w & 63) === 8)) regs.clear();
    }
    /* the flags' stores: the run with the button icons' 1 at 0xFC to 0xFF and 0x1FC to 0x1FF, the code itself the offset */
    const icon = bytes.find(r => r.off === 0xFC && r.v === 1 && bytes.some(q => q.base === r.base && q.off === 0x1FC && Math.abs(q.at - r.at) < 0x1000));
    if (icon === undefined) throw new Error("the European program's font flags weren't found");
    for (const r of bytes) if (r.base === icon.base && Math.abs(r.at - icon.at) < 0x1000 && r.off >= 0x80 && r.off < 0x200) flagsOut.set(r.off, r.v);
    const a0 = rec.find(r => r.v === aPage0 && rec.some(q => q.base === r.base && q.v === aPage1 && q.off - r.off === 0x200));
    if (a0 === undefined) throw new Error("the European program's InitSysFont wasn't found");
    return rec.filter(r => r.base === a0.base && Math.abs(r.at - a0.at) < 0x4000).map(r => {
        const rel = (r.off - a0.off) / 2 + 0x41;
        return { page: rel >= 256 ? 1 : 0, c: rel % 256, g: r.v & 0xFFF, v: r.v };
    }).filter(r => (r.v & 0xF000) === 0x3000);
}
const leftovers = fp.map((_, i) => i).filter(i => !fontOrder.includes(i));
const ourGlyph = p => fontOrder.includes(p) ? fontOrder.indexOf(p) : fontOrder.length + leftovers.indexOf(p);
/* A on each page: the USA's glyph 0x3A and 0x02, which are the European font's fontOrder[...] */
const glyphFlags = new Map();
const stores = fontStores(palExe, 0x3000 | fontOrder[0x3A], 0x3000 | fontOrder[0x02], glyphFlags);
const accents = new Map();
for (const page of [0, 1]) {
    const high = stores.filter(r => r.page === page && r.c >= 0x80);
    for (const r of high.slice(0, -4)) accents.set(`${page}:${r.c}`, { page, c: r.c, g: ourGlyph(r.g), flags: glyphFlags.get(page * 256 + r.c) ?? 0 });
}
console.log(`the font: ${accents.size} letters past the USA's, on its two pages`);

/* The game's words in the five languages, where the European code has them: its program (only on a disc image: -1), its engine and its
   overlays (archive files, where each loads). Two kinds: a message's five strings one after another, English first, with a table of
   five pointers to them; and a menu's array of strings for each language, with a table of five pointers to the arrays, each one's
   strings in the same order. Each is where its English and translations are, by blob and offset: the converter copies them from the
   player's disc, and the game draws a translation in place of the English it matches (paldata.c, pal_text). */
const ENGINE_ENTRY = 1348, OVERLAY_ENTRIES = [848, ...Array.from({ length: 13 }, (_, i) => 1077 + i)];
if (pal[ENGINE_ENTRY][1] !== 374020) throw new Error("the European engine isn't the archive's file " + ENGINE_ENTRY);
const blobs = [{ id: -1, bytes: palExe, base: 0x80010000 - 0x800 }, { id: ENGINE_ENTRY, bytes: P(ENGINE_ENTRY), base: 0x8007A498 },
    ...OVERLAY_ENTRIES.map(e => ({ id: e, bytes: P(e), base: 0x800B29B8 }))];
/* The mode text's roots (modetext.c): the European mode text files load 0x50 later than the USA's (their pointers say where), and its
   engine has a table for each language of where each game mode's pause and rules menus start in its file (45 pointers, 0 for none),
   where the USA has one. The game reads the English (the same on every machine) and draws the language's strings, found by walking
   both files from the same root (modetext.c); so the five tables, in the code's language order. */
const MODES = 45;
const modeTypes = (() => { const m = fs.readFileSync(path.join(ROOT, 'port/game/main/main.c'), 'utf8').match(/u16 ModeTypes\[\d+\] = \{([^}]*)\}/); return m[1].split(',').map(s => s.trim()).filter(Boolean).map(Number); })();
const modeFiles = (() => { const m = fs.readFileSync(path.join(ROOT, 'port/game/overlays/engine/levelload.c'), 'utf8').match(/file \*ModeTextFilesLang\[14\] = \{([^}]*)\}/); return m[1].replace(/\/\*[^]*?\*\//g, '').split(',').map(s => s.trim()).filter(Boolean).map(s => s === '0' ? -1 : Number(s.match(/DatIndex\[(\d+)\]/)[1])); })();
const textOf = (e, L) => L === 0 ? P(map[e]) : P(langFiles.find(f => f.e === e).v[L - 1]);
/* the load address: the one whose first string (at 4) the file points at, of the USA's and the 0x50 later one seen */
const textBase = (() => {
    const b = P(map[text]), hits = base => { let n = 0; for (let p = 4; p + 4 <= b.length; p += 4) n += b.readUInt32LE(p) === base + 4; return n; };
    const base = [0x800AECB0, 0x800AEC60].sort((a, c) => hits(c) - hits(a))[0];
    if (hits(base) === 0) throw new Error("the European mode text's load address wasn't found");
    return base;
})();
const rootTables = [];
{
    const isRoot = x => x === 0 || (x >= textBase && x < textBase + 0x4660 && !(x & 3));
    for (let f = 0; f < pal.length && rootTables.length === 0; f++) {
        const eng = P(f);
        for (let i = 0; i + MODES * 4 * 5 <= eng.length && rootTables.length === 0; i += 4) {
            let ok = true;
            for (let t = 0; t < 5 && ok; t++) for (let k = 0; k < MODES && ok; k++) { const x = eng.readUInt32LE(i + (t * MODES + k) * 4); ok = k >= 33 || (isRoot(x) && x !== 0); }
            if (ok) {
                for (let t = 0; t < 5; t++) rootTables.push(Array.from({ length: MODES }, (_, k) => { const x = eng.readUInt32LE(i + (t * MODES + k) * 4); return k >= 33 || !isRoot(x) || x === 0 ? 0 : x - textBase; }));
                console.log(`the mode text roots: the European file ${f}, at ${i.toString(16)}`);
            }
        }
    }
    if (rootTables.length !== 5) throw new Error("the European mode text roots weren't found");
}
/* which table is which language's: its roots in that language's files are each a pointer into the file */
const modeRoots = [0, 1, 2, 3, 4].map(L => {
    const score = t => { let n = 0; for (let m = 0; m < 33; m++) { const e = modeFiles[modeTypes[m * 3] - 0x101]; if (e < 0) continue; const b = textOf(e, L), r = rootTables[t][m]; if (r > 4 && r + 4 <= b.length) { const x = b.readUInt32LE(r); if (x >= textBase && x < textBase + b.length) n++; } } return n; };
    const best = [0, 1, 2, 3, 4].map(t => [t, score(t)]).sort((a, c) => c[1] - a[1]);
    if (best[0][1] < 30 || best[0][1] === best[1][1]) throw new Error(`the ${LANG_NAMES[L]} mode text roots aren't clear: ${best.map(b => b.join(':')).join(' ')}`);
    return rootTables[best[0][0]];
});
console.log(`the mode text: base ${textBase.toString(16)}, each language's roots found`);

/* Its records are 7 words where the USA's are 9 (modetext.c reads both), and name the European engine's model banks, some of which are
   numbered otherwise: each European bank a record names, the USA's, from the panel lists both discs' English have (the same strings
   in the same lists), every field the same but these and where things are. */
const modelMap = new Map();
{
    const strAt = (d, o) => { let e = o; while (e < d.length && d[e]) e++; return d.toString('latin1', o, e); };
    const s16 = v => (v << 16) >> 16;
    const listStart = (d, o, len) => { while (o - len >= 0) { const t = d.readUInt32LE(o - len) & 0xFF; if (t < 2 || t > 0x20) break; o -= len; } return o; };
    const textRecs = (d, base, len, at) => { const m = new Map(); for (let o = 0; o + len <= d.length; o += 4) { const t = d.readUInt32LE(o) & 0xFF, x = d.readUInt32LE(o + at); if (t === 3 && x >= base + 4 && x < base + d.length) { const s = strAt(d, x - base); if (s.length > 5 && !m.has(s)) m.set(s, o); } } return m; };
    let lists = 0;
    for (let e = text; e < text + texts; e++) {
        const u = U(e), p = P(map[e]);
        const um = textRecs(u, 0x800AEC60, 0x24, 0x14), pm = textRecs(p, textBase, 0x1C, 0x10), done = new Set();
        for (const [s, uo] of um) {
            if (!pm.has(s)) continue;
            let a = listStart(u, uo, 0x24), b = listStart(p, pm.get(s), 0x1C);
            if (done.has(a)) continue;
            done.add(a);
            const pairs = [];
            for (; a + 0x24 <= u.length && b + 0x1C <= p.length; a += 0x24, b += 0x1C) {
                const ut = u.readUInt32LE(a), pt = p.readUInt32LE(b) & 0xFF;
                if (ut < 2 || ut > 0x20 || pt < 2 || pt > 0x20) break;
                pairs.push([ut, s16(u.readUInt32LE(a + 0x1C)), pt, s16(p.readUInt32LE(b + 0x18))]);
            }
            if (!pairs.every(([ut, , pt]) => ut === pt)) continue;
            lists++;
            for (const [, um2, , pm2] of pairs) {
                if (pm2 === um2) continue;
                if (modelMap.has(pm2) && modelMap.get(pm2) !== um2) throw new Error(`the European model bank ${pm2} is the USA's ${modelMap.get(pm2)} and ${um2}`);
                modelMap.set(pm2, um2);
            }
        }
    }
    if (lists < 50) throw new Error(`the mode text's panel lists: only ${lists} the same on both discs`);
    console.log(`the mode text: ${lists} panel lists the same on both discs, ${modelMap.size} model banks numbered otherwise`);
}

const cstr = (b, at) => { let e = at; while (e < b.length && b[e] !== 0) e++; return { s: b.subarray(at, e).toString('latin1'), end: e }; };
const good = s => s.length > 0 && s.length < 300 && !/[\x00-\x09\x0b\x0c\x0e-\x1f]/.test(s);
/* where an address is, seen from blob from: the overlays all load at one place, so an overlay's pointer is into itself, the program or
   the engine, never another overlay */
const seen = from => [from, ...blobs.filter(b => b !== from && b.id !== -2 && (b.id === -1 || b.id === ENGINE_ENTRY))];
const stringAt = (addr, from) => { for (const b of seen(from)) { const o = addr - b.base; if (o >= 0 && o < b.bytes.length && o % 4 === 0) { const { s, end } = cstr(b.bytes, o); if (good(s)) return { b, o, s, end }; } } return null; };
const msgs = new Map();
const pendingList = new Set(); /* messages that may be the middle of a list: checked against the game's English, below */
const messageAt = new Set(); /* where each message set's table is */ /* English -> { blob, offs[5] } */
const keep = (en, tr) => { if (!msgs.has(en.s) && tr.every(x => x.b === en.b)) msgs.set(en.s, { blob: en.b.id, offs: [en.o, ...tr.map(x => x.o)] }); };
/* menus first: a table of five pointers to five arrays of strings is surer than five strings in a row, which a menu's own English
   array looks like too */
for (const pass of ['menus', 'messages']) {
    for (const c of blobs) {
        for (let q = 0; q + 20 <= c.bytes.length; q += 4) {
            const ptr = [0, 1, 2, 3, 4].map(k => c.bytes.readUInt32LE(q + 4 * k));
            if (pass === 'messages') {
                /* a message: five strings one after another */
                const str = ptr.map(p => stringAt(p, c));
                if (str.every(x => x !== null) && str.every((x, k) => k === 4 || (str[k + 1].b === x.b && str[k + 1].o === ((x.end + 4) & ~3)))) {
                    /* not the middle of a list: the pointer before it to the string right before it, an English one (EASY, MEDIUM, HARD
                       and the other languages' after them looked like a message from HARD on) */
                    const before = q >= 4 ? stringAt(c.bytes.readUInt32LE(q - 4), c) : null;
                    const mine = !msgs.has(str[0].s);
                    keep(str[0], str.slice(1));
                    messageAt.add(c.id + ':' + q);
                    /* a set right after another set is a run of messages (EXIT after the sound settings); one inside a list isn't */
                    if (mine && before !== null && before.b === str[0].b && ((before.end + 4) & ~3) === str[0].o && /^[ -~]*$/.test(before.s)) {
                        pendingList.add({ en: str[0].s, at: c.id + ':' + (q - 20) });
                    }
                }
                continue;
            }
            /* a menu: five arrays of string pointers, each string at the same place in each */
            const arr = ptr.map(p => { for (const b of seen(c)) { const o = p - b.base; if (o >= 0 && o + 12 <= b.bytes.length && o % 4 === 0) return { b, o }; } return null; });
            if (arr.some(x => x === null) || new Set(arr.map(x => x.b.id + ':' + x.o)).size < 5) continue;
            const rows = arr.map(x => { const r = []; for (let k = 0; k < 64 && x.o + 4 * k + 4 <= x.b.bytes.length; k++) { const s = stringAt(x.b.bytes.readUInt32LE(x.o + 4 * k), x.b); if (s === null) break; r.push(s); } return r; });
            const n = Math.min(...rows.map(r => r.length));
            if (n < 3) continue;
            /* five arrays apart: not five places in one list (the level names) */
            const spans = arr.map(x => [x.b.id, x.o, x.o + 4 * n]).sort((p, q) => p[0] - q[0] || p[1] - q[1]);
            if (spans.some((s, k) => k > 0 && s[0] === spans[k - 1][0] && s[1] < spans[k - 1][2])) continue;
            for (let k = 0; k < n; k++) keep(rows[0][k], rows.slice(1).map(r => r[k]));
        }
    }
}
/* Only the words the USA game's code has a language table for (its Lang... tables, each a pointer for each language that the USA's all
   point at its English): their strings, followed through the tables and the arrays and structures of strings they point at. A set whose
   English is one of those, by its letters (the European English is spaced and stopped a little otherwise), is kept. */
const cSource = (() => { const all = []; const walk = d => { for (const e of fs.readdirSync(d, { withFileTypes: true })) { const p = path.join(d, e.name); if (e.isDirectory()) walk(p); else if (p.endsWith('.c')) all.push(fs.readFileSync(p, 'utf8')); } }; walk(path.join(ROOT, 'port/game')); return all.join('\n'); })();
const unescape = s => s.replace(/\\n/g, '\n').replace(/\\r/g, '\r').replace(/\\"/g, '"').replace(/\\x([0-9A-Fa-f]{2})/g, (_, h) => String.fromCharCode(parseInt(h, 16))).replace(/\\\\/g, '\\');
/* a string's definition, with its size or none, to its first zero (some keep the bytes the disc has after it) */
const stringDefs = new Map([...cSource.matchAll(/\b(\w+)\[\w*\]\s*=\s*"((?:[^"\\]|\\.)*)"/g)].map(m => [m[1], unescape(m[2]).split('\0')[0]]));
const bodyOf = name => { const m = cSource.match(new RegExp(`\\b${name}\\s*(?:\\[[^\\]]*\\])*\\s*=\\s*\\{`)); if (!m) return null; let i = m.index + m[0].length, depth = 1, s = i; while (depth > 0 && i < cSource.length) { if (cSource[i] === '{') depth++; else if (cSource[i] === '}') depth--; i++; } return cSource.slice(s, i - 1); };
const langKeys = new Set();
const follow = (name, depth) => {
    if (stringDefs.has(name)) { langKeys.add(stringDefs.get(name)); return; }
    if (depth > 2) return;
    const body = bodyOf(name);
    if (body === null) return;
    for (const m of body.matchAll(/"((?:[^"\\]|\\.)*)"/g)) langKeys.add(unescape(m[1]));
    for (const m of body.matchAll(/&?\s*\(?\s*(?:void\s*\*\s*\)\s*&?)?\s*\b([A-Z]\w+)\b/g)) follow(m[1], depth + 1);
};
for (const m of cSource.matchAll(/\b(Lang\w+|PauseMenuSetsByLang)\s*\[[^\]]*\]\s*=/g)) follow(m[1], 0);
/* and the menus' own words: the strings the title menus, the game's flow and the pause menus define */
for (const file of ['overlays/titles/titles.c', 'overlays/engine/gameflow.c', 'overlays/engine/pause.c']) {
    for (const m of fs.readFileSync(path.join(ROOT, 'port/game', file), 'utf8').matchAll(/\b(\w+)\[\w*\]\s*=\s*"((?:[^"\\]|\\.)*)"/g)) langKeys.add(unescape(m[2]).split('\0')[0]);
}
for (const [name, s] of stringDefs) if (/^PauseText/.test(name)) langKeys.add(s);
const letters = s => s.toUpperCase().replace(/[^A-Z0-9]/g, '');
const langLetters = new Set([...langKeys].map(letters).filter(l => l.length > 0));
console.log(`the words the USA code has language tables for: ${langKeys.size}`);
for (const [en] of [...msgs]) if (!langKeys.has(en) && !langLetters.has(letters(en))) msgs.delete(en);
/* not in the middle of a list of the game's English, nor with German where French or Spanish should be */
for (const [en, v] of [...msgs]) {
    const b = blobs.find(x => x.id === v.blob), tr = v.offs.slice(1).map(o => cstr(b.bytes, o).s);
    const german = s => /[ÄÖÜäöüß]/.test(s.replace(/ü (BUTTON|TASTE)|ü-TASTE|TOUCHE ü|TECLA ü|PULSA ü|TASTO ü|ü/g, ''));
    /* and four languages that start alike where the English doesn't are four of one language's messages, not a translation */
    const first = s => s.trim().split(/[\s|]+/).filter(Boolean)[0] ?? '';
    const alike = tr.every(s => first(s) === first(tr[0])) && first(tr[0]) !== first(en) && first(tr[0]).length > 1;
    if (german(tr[0]) || german(tr[3]) || alike) msgs.delete(en);
}
for (const { en, at } of pendingList) {
    const v = msgs.get(en);
    if (v === undefined || messageAt.has(at)) continue;
    const b = blobs.find(x => x.id === v.blob), prev = (() => { let o = v.offs[0] - 2; while (o > 0 && b.bytes[o - 1] !== 0) o--; return cstr(b.bytes, o).s; })();
    if (langKeys.has(prev)) msgs.delete(en);
}
/* and not a list of the game's English names one after another (the level names, the modes'), whose "translations" are the next ones */
for (const [en, v] of [...msgs]) {
    const b = blobs.find(x => x.id === v.blob);
    if (v.offs.slice(1).some(o => { const s = cstr(b.bytes, o).s; return s !== en && langKeys.has(s); })) msgs.delete(en);
}
const palStrings = [...msgs.entries()];
console.log(`the words: ${palStrings.length} English strings with their four languages (${palStrings.filter(([, v]) => v.blob === -1).length} in the program, which only a disc image has)`);
if (!['SELECT A GAME SAVE', 'CONTINUE', 'QUIT GAME', 'EXIT'].every(s => msgs.has(s))) throw new Error("the European code's words weren't found: " + ['SELECT A GAME SAVE', 'CONTINUE', 'QUIT GAME', 'EXIT'].filter(s => !msgs.has(s)).join(', '));
console.log('  CONTINUE ->', msgs.get('CONTINUE').offs.slice(1).map(o => cstr(blobs.find(b => b.id === msgs.get('CONTINUE').blob).bytes, o).s).join(' | '));

const crc = zlib.crc32(palDat) >>> 0;
const kinds = { copy: 'PAL_COPY', font: 'PAL_FONT', interface: 'PAL_INTERFACE', none: 'PAL_NONE', pictures: 'PAL_PICTURES' };
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

/* the main menu's pictures (082) in the USA's order, each the European's: the rest of the European ones follow (archive.c) */
const short PalPicturesOrder[PAL_PICTURES_COUNT] = {
${picsOrder.map(String).join(', ').replace(/(.{1,126})(, |$)/g, '    $1,\n').replace(/,,\n$/, ',\n')}};
/* and each one's frames where they aren't the European's (0: as they are) */
const signed char PalPicturesFrames[PAL_PICTURES_COUNT] = {
${picsFrames.map(String).join(', ').replace(/(.{1,126})(, |$)/g, '    $1,\n').replace(/,,\n$/, ',\n')}};
/* and for one with a frame for each language, the European English one's picture (-1: none), the others following it */
const short PalPicturesLanguage[PAL_PICTURES_COUNT] = {
${picsLanguage.map(String).join(', ').replace(/(.{1,126})(, |$)/g, '    $1,\n').replace(/,,\n$/, ',\n')}};

/* the European mode text's load address, and each language's roots in its files: each game mode's pause and rules menus (modetext.c) */
const unsigned PalModeTextBase = 0x${textBase.toString(16).toUpperCase()}u;
/* the model banks the European mode text's records name that the USA's numbers otherwise: European, USA */
const short PalModeTextModels[][2] = {
${[...modelMap.entries()].sort((a, c) => a[0] - c[0]).map(([p, u]) => `    { ${p}, ${u} },`).join('\n')}
};
const int PalModeTextModelCount = ${modelMap.size};
const unsigned short PalModeTextRoots[PAL_LANGUAGES][PAL_MODES] = {
${modeRoots.map(r => '    { ' + r.map(x => '0x' + x.toString(16).toUpperCase()).join(', ') + ' },').join('\n')}
};

/* the USA files with a version for each language: the French's, German's, Italian's and Spanish's place and size (the English is PalFiles') */
const PalLanguageFile PalLanguageFiles[] = {
${langFiles.map(f => `    { ${f.e}, ${f.words ? 'PAL_WORDS' : f.sound ? 'PAL_SOUND' : 'PAL_TEXT'}, { ${f.v.map(p => `{ ${pal[p][0]}, ${pal[p][1]} }`).join(', ')} } }, /* ${f.e.toString(16).toUpperCase().padStart(3, '0')} */`).join('\n')}
};
const int PalLanguageFileCount = ${langFiles.length};

/* the font's letters past the USA's (its accented ones): page, code (Latin-1, as the European text has them), glyph in the made font */
const PalGlyph PalGlyphs[] = {
${[...accents.values()].map(a => `    { ${a.page}, 0x${a.c.toString(16).toUpperCase()}, ${a.flags}, ${a.g} },`).join('\n')}
};
const int PalGlyphCount = ${accents.size};

/* the game's words: where each English string and its French, German, Italian and Spanish are in the European code: the sector and size
   in the European CRASHBSH.DAT of its engine or overlay, or -1 and its size for its program file, then the five offsets in it
   (paldata.c copies them from the player's disc) */
const PalString PalStrings[] = {
${palStrings.map(([en, v]) => `    { ${v.blob === -1 ? -1 : pal[v.blob][0]}, ${v.blob === -1 ? palExe.length : pal[v.blob][1]}, { ${v.offs.join(', ')} } }, /* ${JSON.stringify(en).slice(1, -1).replace(/\*\//g, '* /').slice(0, 60)} */`).join('\n')}
};
const int PalStringCount = ${palStrings.length};
`;
fs.writeFileSync(path.join(ROOT, 'port/platform/paltable.c'), c);
console.log(`port/platform/paltable.c: ${ENTRIES} files, the font's ${fontOrder.length} glyphs, the interface's ${ifaceOrder.length} pictures; European CRASHBSH.DAT ${palDat.length} bytes, CRC ${crc.toString(16)}`);
