// Policy tests for swarm.mjs (no Docker, no Claude, no git remote).
// Run: node --test tools/swarm/swarm.test.mjs
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import test from 'node:test';
import { fileURLToPath } from 'node:url';

process.env.SWARM_IMPORT = '1';
const { Swarm, reapply, parseArgs, INCLUDE_LINE } = await import('./swarm.mjs');

const REPO = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..', '..');
const DECOMP = fs.readFileSync(path.join(REPO, 'tools/scripts/decomp.py'), 'utf8');

function tmpDir() {
  return fs.mkdtempSync(path.join(os.tmpdir(), 'swarm-test-'));
}

// a Swarm on a temp --dir; extra options as CLI arguments
function swarm(...args) {
  const dir = tmpDir();
  const [, o] = parseArgs(['run', '--dir', dir, ...args]);
  fs.mkdirSync(dir, { recursive: true });
  return new Swarm(o);
}

const fn = (name, extra = {}) => ({ key: name.replace(/^func_/, ''), name, seg: 'main/main', size: 100,
                                    src: 'src/main/main.c', ...extra });

// ---------------------------------------------------------------- ranking

test('better: a match beats everything, and only a match beats a match', () => {
  const m = { stage: 'match', diffs: 0, total: 0 };
  const r = { stage: 'registers', diffs: 1, total: 100 };
  assert.equal(Swarm.better(m, r), true);
  assert.equal(Swarm.better(r, m), false);
  assert.equal(Swarm.better(m, m), false);
  assert.equal(Swarm.better(r, null), true);
});

test('better: paired, then blocks, then exact, then STAGE', () => {
  const s = (stage, diffs, paired, blocks, exact) => ({ stage, diffs, total: 100,
    regions: { paired, blocks, exact, len: 100, nblocks: 20 } });
  assert.equal(Swarm.better(s('structure', 50, 90, 1, 1), s('registers', 1, 80, 19, 99)), true);
  assert.equal(Swarm.better(s('structure', 50, 90, 10, 1), s('registers', 1, 90, 9, 99)), true);
  assert.equal(Swarm.better(s('structure', 50, 90, 10, 60), s('registers', 1, 90, 10, 59)), true);
  assert.equal(Swarm.better(s('registers', 9, 90, 10, 60), s('instructions', 1, 90, 10, 60)), true);
  assert.equal(Swarm.better(s('registers', 2, 90, 10, 60), s('registers', 3, 90, 10, 60)), true);
  assert.equal(Swarm.better(s('registers', 3, 90, 10, 60), s('registers', 3, 90, 10, 60)), false);
});

test('better: without region scores, a later stage beats any count, then fewer diffs', () => {
  assert.equal(Swarm.better({ stage: 'registers', diffs: 40 }, { stage: 'instructions', diffs: 1 }), true);
  assert.equal(Swarm.better({ stage: 'instructions', diffs: 4 }, { stage: 'instructions', diffs: 5 }), true);
  assert.equal(Swarm.better({ stage: 'instructions', diffs: 5 }, { stage: 'instructions', diffs: 5 }), false);
});

// ---------------------------------------------------------------- output contracts with decomp.py

test('score reads the STAGE and MATCH lines', () => {
  assert.deepEqual(Swarm.score('...\nSTAGE func_80012345 registers 3/120 (length 120 vs 120)\n'),
                   { stage: 'registers', diffs: 3, total: 120 });
  assert.equal(Swarm.score('MATCH func_80012345\n').stage, 'match');
  assert.equal(Swarm.score('BUILD FAILED\n'), null);
});

test('regionScore reads the REGIONS line', () => {
  const line = 'REGIONS func_80012345 blocks 12/20 insns 300/400 (75.0%) paired 380/400 differ 8 first 3';
  assert.deepEqual(Swarm.regionScore(line), { blocks: 12, nblocks: 20, exact: 300, len: 400, paired: 380 });
});

test('the decomp.py lines the swarm parses are still printed', () => {
  // if one of these moves or changes shape, every big session scores null and parks
  assert.match(DECOMP, /print\(f"STAGE \{name\} \{stage\} \{len\(diffs\)\}\/\{max\(nt, nc\)\}/);
  assert.match(DECOMP, /print\(f"REGIONS \{name\} blocks \{res\['ok'\]\}\/\{res\['total'\]\} insns \{res\['exact'\]\}\/\{res\['len'\]\} "\n\s*f"\(\{100 \* res\['exact'\] \/ max\(res\['len'\], 1\):\.1f\}%\) paired \{res\['paired'\]\}\/\{res\['len'\]\} "/);
  assert.match(DECOMP, /print\(f"permuter best score \{best\}\{f' \(base \{base\}\)' if base is not None else ''\}: \{path\}\/source\.c"/);
  assert.match(DECOMP, /print\(f"PERMUTE-RESULT \{name\} kept before \{n0\} after \{n1\}"\)/);
  assert.match(DECOMP, /print\(f"MATCH /);
  // the queue's fields that swarm.mjs reads
  const q = DECOMP.slice(DECOMP.indexOf('def cmd_queue'), DECOMP.indexOf('def cmd_m2c'));
  for (const k of ['key', 'name', 'seg', 'size', 'src', 'fp', 'note']) assert.match(q, new RegExp(`"${k}":`));
});

// ---------------------------------------------------------------- checkPatch

const SP = ' ';   // a blank context line (editors strip trailing spaces)
const PATCH = (removed, added = '') => `diff --git a/src/main/main.c b/src/main/main.c
--- a/src/main/main.c
+++ b/src/main/main.c
@@ -10,3 +10,5 @@ void f(void);
${SP}
-INCLUDE_ASM("asm/nonmatchings/main/main", ${removed});
+void ${removed}(void) {
+${added}
+}
${SP}
`;

test('checkPatch: a plain replacement of the function passes', () => {
  assert.deepEqual(swarm().checkPatch(PATCH('func_80012345'), fn('func_80012345')), []);
});

test('checkPatch: another function, an added INCLUDE_ASM, a path outside src/include, asm', () => {
  const s = swarm(), f = fn('func_80012345');
  assert.match(s.checkPatch(PATCH('func_80099999'), f).join(), /expected only func_80012345/);
  assert.match(s.checkPatch(PATCH('func_80012345', 'INCLUDE_ASM("x", func_8001);'), f).join(), /adds INCLUDE_ASM/);
  assert.match(s.checkPatch(PATCH('func_80012345').replace(/src\/main\/main\.c/g, 'tools/dk'), f).join(), /touches tools\/dk/);
  assert.match(s.checkPatch(PATCH('func_80012345', '    asm("nop");'), f).join(), /inline asm/);
  assert.match(s.checkPatch(PATCH('func_80012345', '#define func_80012345(x) x'), f).join(), /macro named/);
});

// ---------------------------------------------------------------- reapply

function clone(files) {
  const dir = tmpDir();
  for (const [p, text] of Object.entries(files)) {
    fs.mkdirSync(path.dirname(path.join(dir, p)), { recursive: true });
    fs.writeFileSync(path.join(dir, p), text);
  }
  return dir;
}

test('reapply puts the C in place of the INCLUDE_ASM line', () => {
  const dir = clone({ 'src/main/main.c': 'void f(void);\n\nint other;\nINCLUDE_ASM("asm/nonmatchings/main/main", func_80012345);\n\nvoid g(void) {}\n' });
  assert.equal(reapply(dir, PATCH('func_80012345', '    other = 1;'), fn('func_80012345')), null);
  assert.equal(fs.readFileSync(path.join(dir, 'src/main/main.c'), 'utf8'),
               'void f(void);\n\nint other;\nvoid func_80012345(void) {\n    other = 1;\n}\n\nvoid g(void) {}\n');
});

test('reapply gives up when the patch changes another line', () => {
  const dir = clone({ 'src/main/main.c': 'int a;\nINCLUDE_ASM("asm/nonmatchings/main/main", func_80012345);\n' });
  const patch = PATCH('func_80012345').replace(' \n-INCLUDE', '-int a;\n+int b;\n-INCLUDE');
  assert.match(reapply(dir, patch, fn('func_80012345')), /changes an existing line/);
});

test('INCLUDE_LINE names the function', () => {
  assert.equal('INCLUDE_ASM("asm/nonmatchings/main/main", func_80012345);'.match(INCLUDE_LINE)[1], 'func_80012345');
});

// ---------------------------------------------------------------- leasing and escalation

test('nextJob: normal slots take small functions, big slots big ones; leased and finished are skipped', () => {
  const s = swarm('--big-workers', '1', '--big-bytes', '600');
  const small = fn('func_80010000'), small2 = fn('func_80020000'), big = fn('func_80030000', { size: 900 });
  s.queue = [small, small2, big];
  const j = s.nextJob();
  assert.equal(j.f, small);
  assert.equal(j.model, s.o.model);
  s.slots[0].job = j;
  assert.equal(s.nextJob().f, small2);
  s.finished.add(small2.key);
  assert.equal(s.nextJob(), null);
  const b = s.nextJob(false, true);
  assert.equal(b.f, big);
  assert.equal(b.big, true);
  assert.equal(b.model, s.o.bigModel);
});

test('nextJob: neighbours in the same file and identical copies are not leased together', () => {
  const s = swarm();
  const a = fn('func_80010000', { line: 100, fp: 'x' }), near = fn('func_80010100', { line: 104 });
  const twin = fn('func_80010200', { line: 500, fp: 'x' }), far = fn('func_80010300', { line: 300 });
  s.queue = [a, near, twin, far];
  s.slots[0].job = s.nextJob();
  assert.equal(s.nextJob().f, far);
});

test('nextJob: functions out of attempts are skipped unless --retry-failed', () => {
  const s = swarm();
  const f = fn('func_80010000');
  s.queue = [f];
  s.history[f.key] = { name: f.name, attempts: s.o.maxAttempts, notes: [] };
  assert.equal(s.nextJob(), null);
  s.o.retryFailed = true;
  assert.equal(s.nextJob().f, f);
});

test('big mode: functions with a checkpoint first, then the smallest; stale ones park', () => {
  const s = swarm('--big-workers', '1', '--big-bytes', '600');
  const a = fn('func_80010000', { size: 800 }), b = fn('func_80020000', { size: 1200 }), c = fn('func_80030000', { size: 700 });
  s.queue = [a, b, c];
  const cp = path.join(s.paths.checkpoints, b.name);
  fs.mkdirSync(cp, { recursive: true });
  fs.writeFileSync(path.join(cp, 'best.json'), JSON.stringify({ stage: 'registers', diffs: 3, total: 300 }));
  fs.writeFileSync(path.join(cp, 'best.diff'), PATCH(b.name));
  s.bslots[0].job = s.nextJob(false, true);
  assert.equal(s.bslots[0].job.f, b);
  assert.equal(s.nextJob(false, true).f, c);
  s.bslots[0].job = null;
  s.history[a.key] = { name: a.name, attempts: 0, notes: [], bigStale: s.o.bigStale };
  s.history[b.key] = { name: b.name, attempts: 0, notes: [], bigSessions: s.o.bigSessions };
  s.history[c.key] = { name: c.name, attempts: 0, notes: [], bigStale: s.o.bigStale };
  assert.equal(s.nextJob(false, true), null);
});

test('escalation: sonnet -> opus -> big mode -> parked', () => {
  const s = swarm('--big-workers', '1', '--max-attempts', '2');
  const f = fn('func_80010000');
  s.notMatched({ job: 1 }, { f, model: s.o.model, attempt: 1, notes: [] }, 'first');
  const opus = s.retry.shift();
  assert.equal(opus.model, s.o.escalateModel);
  assert.equal(opus.attempt, 2);
  s.notMatched({ job: 1 }, opus, 'second');
  const big = s.retry.shift();
  assert.equal(big.big, true);
  assert.equal(s.history[f.key].big, true);
  assert.equal(s.history[f.key].attempts, 2);
  s.o.escalateBig = false;
  s.notMatched({ job: 1 }, { f, model: s.o.escalateModel, attempt: 2, notes: [] }, 'third');
  assert.equal(s.retry.length, 0);
  assert.equal(s.stats.parked, 1);
  assert.ok(s.finished.has(f.key));
});
