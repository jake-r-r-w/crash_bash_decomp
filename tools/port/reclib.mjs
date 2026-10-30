// Record layouts of the port's game C from Clang, at 32 bits (the PS1's sizes) and 64 bits. Runs on the host.
import fs from 'node:fs';
import path from 'node:path';
import { execFile } from 'node:child_process';

const ROOT = 'port/game';
const CC = process.env.CC || 'x86_64-w64-mingw32-clang';
export const FLAGS = ['-fsyntax-only', '-w', '-std=gnu17', '-fsigned-char', '-fno-builtin', '-Iport/psyq/include', '-Iport/game/include'];

export function sources() {
  const out = [];
  const walk = d => { for (const e of fs.readdirSync(d, { withFileTypes: true })) { const p = path.join(d, e.name); if (e.isDirectory()) walk(p); else if (e.name.endsWith('.c')) out.push(p); } };
  walk(ROOT + '/main'); walk(ROOT + '/overlays');
  return out;
}

// the typedef name of an unnamed record, from where it starts
const lines = new Map();
function typedefAt(file, line) {
  if (!lines.has(file)) { try { lines.set(file, fs.readFileSync(file, 'utf8').split(/\r?\n/)); } catch { lines.set(file, []); } }
  const src = lines.get(file);
  let d = 0;
  for (let i = line - 1; i < src.length; i++) {
    for (const c of src[i].replace(/\/\*.*?\*\//g, '')) { if (c === '{') d++; if (c === '}') d--; }
    if (d <= 0 && i >= line - 1 && src[i].includes('}')) { const m = src[i].match(/\}\s*(\w+)\s*;/); return m ? m[1] : null; }
  }
  return null;
}

function run(args) {
  return new Promise(res => execFile(CC, args, { maxBuffer: 1 << 30 }, (err, stdout) => res(stdout || '')));
}

async function dump(bits) {
  const target = bits === 32 ? ['--target=i686-w64-mingw32'] : [];
  const recs = {};
  const files = sources();
  let i = 0;
  await Promise.all(Array.from({ length: 8 }, async () => {
    while (i < files.length) {
      const f = files[i++];
      const out = await run([...target, ...FLAGS, '-Xclang', '-fdump-record-layouts-complete', f]);
      for (const block of out.split('*** Dumping AST Record Layout').slice(1)) {
        const rows = block.split(/\r?\n/).filter(l => l.includes('|'));
        const head = rows[0]?.match(/\|\s*(?:struct|union)\s+(.*)$/);
        if (!head) continue;
        let name = head[1].trim();
        const anon = name.match(/^\((?:unnamed|anonymous)(?: struct| union)? at (.*?):(\d+):\d+\)$/);
        if (anon) name = typedefAt(anon[1], Number(anon[2]));
        if (!name || !/^\w+$/.test(name)) continue;
        const fields = [];
        for (const l of rows.slice(1)) {
          const m = l.match(/^\s*(\d+)(?::\d+-\d+)?\s*\|( *)(.*)$/);
          if (!m) continue;
          const body = m[3].trim();
          fields.push({ off: Number(m[1]), depth: (m[2].length - 1) / 2, name: body.split(/\s+/).pop(), leaf: !/^(struct|union)\b/.test(body) || /[*[]/.test(body.split(/\s+\S+$/)[0]), text: body });
        }
        const size = Number([...block.matchAll(/sizeof=(\d+)/g)].pop()?.[1]);
        // the same name in two overlays: the larger is the real record, not a forward view
        if (!recs[name] || recs[name].size < size) recs[name] = { size, fields };
      }
    }
  }));
  // typedef X Y
  const inc = ROOT + '/include/bash';
  for (const e of fs.readdirSync(inc)) {
    const s = fs.readFileSync(path.join(inc, e), 'utf8');
    for (const m of s.matchAll(/typedef\s+(?:struct|union)\s+(\w+)\s+(\w+)\s*;/g)) if (recs[m[1]] && !recs[m[2]]) recs[m[2]] = recs[m[1]];
    for (const m of s.matchAll(/typedef\s+(?:struct|union)\s+(\w+)\s*\{[\s\S]*?\}\s*(\w+)\s*;/g)) if (m[1] !== m[2] && recs[m[1]] && !recs[m[2]]) recs[m[2]] = recs[m[1]];
  }
  return recs;
}

export const layouts = bits => dump(bits);
