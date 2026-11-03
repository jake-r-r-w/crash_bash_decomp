#!/usr/bin/env node
/* Makes a BPS patch, for a mod's files/NNN.bps (docs/MODDING.md): what a changed file has that the original doesn't, so a mod can be
 * shared without the game's files. The game applies it to its own copy (port/platform/bps.c), and the checksums in it say whether
 * that's the file it was made from. Each patch made here is applied again before it's kept, to be sure.
 *
 *   node port/mods/bps.mjs ORIGINAL CHANGED PATCH.bps
 *
 * As a module: bps_create(source, target) gives the patch, with how many of its bytes are the changed file's own; bps_apply(patch,
 * source) gives what it makes. */
import fs from 'node:fs';
import zlib from 'node:zlib';
import { fileURLToPath } from 'node:url';

/* a run of matching bytes worth a copy rather than the bytes themselves */
const MATCH = 8;

/* a number as BPS has it: 7 bits a byte, the last with its top bit set, each byte but the first counting one more than its bits say */
function number(out, n) {
    for (;;) {
        const x = n % 128;

        n = Math.floor(n / 128);
        if (n === 0) {
            out.push(0x80 | x);
            return;
        }
        out.push(x);
        n--;
    }
}

const crc = bytes => zlib.crc32(bytes) >>> 0;
const u32 = n => [n & 255, (n >>> 8) & 255, (n >>> 16) & 255, (n >>> 24) & 255];

export function bps_create(source, target) {
    const out = [...Buffer.from('BPS1')], keys = new Map();
    let at = 0, from = 0, literal = [], own = 0;

    number(out, source.length);
    number(out, target.length);
    number(out, 0);
    /* where each run of the source's bytes starts, the first few of each */
    for (let i = 0; i + MATCH <= source.length; i++) {
        const key = source.toString('latin1', i, i + MATCH), list = keys.get(key);

        if (list == null) {
            keys.set(key, [i]);
        } else if (list.length < 16) {
            list.push(i);
        }
    }
    const flush = () => {
        if (literal.length !== 0) {
            number(out, (literal.length - 1) * 4 + 1);
            for (const x of literal) {
                out.push(x);
            }
            own += literal.length;
            literal = [];
        }
    };
    while (at < target.length) {
        let same = 0, best = 0, where = -1;

        /* the source's own bytes where the target's are, or the longest run of them anywhere else */
        while (at + same < target.length && at + same < source.length && target[at + same] === source[at + same]) {
            same++;
        }
        if (same >= MATCH) {
            flush();
            number(out, (same - 1) * 4);
            at += same;
            continue;
        }
        for (const i of at + MATCH <= target.length ? keys.get(target.toString('latin1', at, at + MATCH)) ?? [] : []) {
            let n = 0;

            while (at + n < target.length && i + n < source.length && target[at + n] === source[i + n]) {
                n++;
            }
            if (n > best) {
                best = n;
                where = i;
            }
        }
        if (best >= MATCH) {
            flush();
            number(out, (best - 1) * 4 + 2);
            number(out, Math.abs(where - from) * 2 + (where < from ? 1 : 0));
            from = where + best;
            at += best;
            continue;
        }
        literal.push(target[at++]);
    }
    flush();
    out.push(...u32(crc(source)), ...u32(crc(target)));
    out.push(...u32(crc(Buffer.from(out))));
    return { patch: Buffer.from(out), own };
}

export function bps_apply(patch, source) {
    let at = 4, made = 0, from_source = 0, from_target = 0;
    const read = () => {
        let data = 0, shift = 1;

        for (;;) {
            const x = patch[at++];

            data += (x & 0x7F) * shift;
            if (x & 0x80) {
                return data;
            }
            shift *= 128;
            data += shift;
        }
    };
    const end = patch.length - 12, source_size = read(), target = Buffer.alloc(read()), meta = read();

    at += meta;
    if (patch.toString('latin1', 0, 4) !== 'BPS1' || source_size !== source.length || patch.readUInt32LE(end) !== crc(source)) {
        throw new Error('not a patch for this file');
    }
    while (at < end) {
        const data = read(), length = Math.floor(data / 4) + 1, kind = data % 4;

        if (kind === 0) {
            source.copy(target, made, made, made + length);
        } else if (kind === 1) {
            patch.copy(target, made, at, at + length);
            at += length;
        } else {
            const move = read(), step = move % 2 ? -Math.floor(move / 2) : Math.floor(move / 2);

            if (kind === 2) {
                from_source += step;
                source.copy(target, made, from_source, from_source + length);
                from_source += length;
            } else {
                from_target += step;
                for (let i = 0; i < length; i++) {
                    target[made + i] = target[from_target++];
                }
            }
        }
        made += length;
    }
    if (made !== target.length || patch.readUInt32LE(end + 4) !== crc(target) || patch.readUInt32LE(end + 8) !== crc(patch.subarray(0, end + 8))) {
        throw new Error('a damaged patch');
    }
    return target;
}

/* a patch, applied again to be sure of it */
export function bps_make(source, target) {
    const made = bps_create(source, target);

    if (!bps_apply(made.patch, source).equals(target)) {
        throw new Error('the patch made something else');
    }
    return made;
}

if (process.argv[1] === fileURLToPath(import.meta.url)) {
    const [original, changed, out] = process.argv.slice(2);

    if (out == null) {
        console.log('node port/mods/bps.mjs ORIGINAL CHANGED PATCH.bps');
        process.exit(2);
    }
    const { patch, own } = bps_make(fs.readFileSync(original), fs.readFileSync(changed));
    fs.writeFileSync(out, patch);
    console.log(`${out}: ${patch.length} bytes, ${own} of them the changed file's own`);
}
