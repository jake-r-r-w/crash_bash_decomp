#!/usr/bin/env node
// Finds where the port's game C reads one struct as another, and checks those views still line up now that pointers are
// 8 bytes: a field of the view at a PS1 offset inside a field of the struct has to be at that field's offset here.
// Runs on the host, with llvm-mingw's Clang on the PATH.
//
//   node tools/port/views.mjs            every view of a struct holding a pointer, BAD ones first
//   node tools/port/views.mjs A:B ...    just these
import fs from 'node:fs';
import path from 'node:path';
import { spawn } from 'node:child_process';
import readline from 'node:readline';
import { layouts, sources, FLAGS } from './reclib.mjs';

const CC = process.env.CC || 'x86_64-w64-mingw32-clang';
const PLAIN = new Set(['void', 's8', 'u8', 's16', 'u16', 's32', 'u32', 'char', 'short', 'int', 'long', 'iptr', 'unsigned', 'signed']);
const base = t => { const m = t.match(/^(?:const |volatile )*(?:struct |union )?(\w+) \*+$/); return m && m[1]; };

function scan(f, pairs) {
  return new Promise(res => {
    const p = spawn(CC, [...FLAGS, '-Xclang', '-ast-dump', f], { stdio: ['ignore', 'pipe', 'ignore'] });
    const rl = readline.createInterface({ input: p.stdout, crlfDelay: Infinity });
    let pend = null, at = 0;
    rl.on('line', line => {
      const depth = line.search(/[A-Z]/);
      const lm = line.match(/<(?:[^:<>]*?\.[ch]:(\d+):\d+|line:(\d+):\d+)/);
      if (lm) at = Number(lm[1] || lm[2]);
      if (pend) {
        const q = line.match(/'([^']+)'(?::'([^']+)')?/);
        if (q && depth > pend.depth) {
          const s = base(q[2] || q[1]);
          if (s && s !== pend.d && !PLAIN.has(s)) {
            const k = `${s}:${pend.d}`;
            const e = pairs.get(k) || { n: 0, at: `${path.basename(f)}:${pend.l}` };
            e.n++;
            pairs.set(k, e);
          }
        }
        pend = null;
      }
      const c = line.match(/CStyleCastExpr 0x\w+ <[^>]*> '([^']+)'(?::'([^']+)')? <BitCast>/);
      if (c) { const d = base(c[2] || c[1]); if (d && !PLAIN.has(d)) pend = { d, depth, l: at }; }
    });
    p.on('close', () => res());
  });
}

const leaves = r => r.fields.filter(f => f.leaf);
const [r32, r64] = await Promise.all([layouts(32), layouts(64)]);
let want = process.argv.slice(2).map(t => t.split(':'));
const info = new Map();
if (!want.length) {
  const files = sources();
  let i = 0;
  await Promise.all(Array.from({ length: 8 }, async () => { while (i < files.length) await scan(files[i++], info); }));
  want = [...info.keys()].map(k => k.split(':'));
}
const rows = [];
for (const [a, b] of want) {
  const where = info.get(`${a}:${b}`);
  const tail = where ? `  ${where.n}x, ${where.at}` : '';
  if (!r32[a] || !r32[b] || !r64[a] || !r64[b]) continue;
  if (r32[a].size === r64[a].size && r32[b].size === r64[b].size) continue;  // no pointers in either
  const a32 = leaves(r32[a]), a64 = leaves(r64[a]), b32 = leaves(r32[b]), b64 = leaves(r64[b]);
  const bad = [];
  for (let i = 0; i < b32.length; i++) {
    if (/\bpad/i.test(b32[i].name)) continue;
    const p = b32[i].off;
    // past the end of the smaller struct: a cast up to a type built on it, which only has to clear its header
    if (p >= r32[a].size) {
      if (b64[i].off < r64[a].size) bad.push(`${b32[i].name} 0x${p.toString(16)}: 0x${b64[i].off.toString(16)} is inside ${a}`);
      continue;
    }
    let k = -1;
    for (let j = 0; j < a32.length; j++) if (a32[j].off <= p) k = j;
    if (k < 0) continue;
    const should = a64[k].off + (p - a32[k].off);
    if (b64[i].off !== should) bad.push(`${b32[i].name} 0x${p.toString(16)}: 0x${b64[i].off.toString(16)} not 0x${should.toString(16)}`);
  }
  rows.push({ bad: bad.length, text: `${bad.length ? 'BAD' : 'ok '} ${a} (${r32[a].size}/${r64[a].size}) as ${b} (${r32[b].size}/${r64[b].size})${tail}${bad.length ? '\n      ' + bad.slice(0, 4).join(', ') + (bad.length > 4 ? `, +${bad.length - 4}` : '') : ''}` });
}
rows.sort((x, y) => y.bad - x.bad);
console.log(rows.map(r => r.text).join('\n'));
console.log(`${rows.filter(r => r.bad).length} of ${rows.length} views don't line up`);
