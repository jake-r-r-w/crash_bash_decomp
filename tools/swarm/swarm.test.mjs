// Policy tests for swarm.mjs (no Docker, no Claude, no git remote).
// Run: node --test tools/swarm/swarm.test.mjs
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import test from 'node:test';
import { fileURLToPath } from 'node:url';

process.env.SWARM_IMPORT = '1';
const { Swarm, reapply, parseArgs, INCLUDE_LINE, sessionUsage: sessionUsageFn, readTranscript: readTranscriptFn,
        ledgerStats: ledgerStatsFn } = await import('./swarm.mjs');

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

const HEADER_PATCH = (decl) => `diff --git a/include/bash/main.h b/include/bash/main.h
--- a/include/bash/main.h
+++ b/include/bash/main.h
@@ -1,3 +1,6 @@
 typedef struct Old { s16 x; s16 y; } Old;
${decl.map(l => '+' + l).join('\n')}
${SP}
 void func_80000000(void);
`;

test('reapply keeps a new struct whole even when another struct has the same member line', () => {
  const dir = clone({
    'src/main/main.c': 'int other;\nINCLUDE_ASM("asm/nonmatchings/main/main", func_80012345);\n',
    'include/bash/main.h': 'typedef struct Old {\n    s16 x;\n    s16 y;\n} Old;\n\nvoid func_80000000(void);\nvoid func_80012345(void);\n',
  });
  const decl = ['typedef struct New {', '    s16 x;', '    s16 y;', '    s32 z;', '} New;', 'void func_80012345(void);'];
  const patch = HEADER_PATCH(decl).replace(' typedef struct Old { s16 x; s16 y; } Old;', ' } Old;') + PATCH('func_80012345');
  assert.equal(reapply(dir, patch, fn('func_80012345')), null);
  const h = fs.readFileSync(path.join(dir, 'include/bash/main.h'), 'utf8');
  assert.match(h, /typedef struct New \{\n {4}s16 x;\n {4}s16 y;\n {4}s32 z;\n\} New;\n/);
  assert.equal(h.match(/void func_80012345\(void\);/g).length, 1);   // the prototype `done` added stays single
});

test('reapply puts lines added after the replaced INCLUDE_ASM line after the new C', () => {
  const dir = clone({ 'src/main/main.c': 'int other;\nINCLUDE_ASM("asm/nonmatchings/main/main", func_80012345);\n\nvoid g(void) {}\n' });
  const patch = `diff --git a/src/main/main.c b/src/main/main.c
--- a/src/main/main.c
+++ b/src/main/main.c
@@ -1,4 +1,7 @@
 int other;
-INCLUDE_ASM("asm/nonmatchings/main/main", func_80012345);
+void func_80012345(void) {
+}
${SP}
+static int added;
+
 void g(void) {}
`;
  assert.equal(reapply(dir, patch, fn('func_80012345')), null);
  assert.equal(fs.readFileSync(path.join(dir, 'src/main/main.c'), 'utf8'),
               'int other;\nvoid func_80012345(void) {\n}\n\nstatic int added;\n\nvoid g(void) {}\n');
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

// ---------------------------------------------------------------- names shared across slot overlays

const slot = ovl => fn('func_800B3A38', { key: `${ovl}:800B3A38`, ref: `${ovl}:800B3A38`,
                                          seg: `overlays/${ovl}/${ovl}`, src: `src/overlays/${ovl}/${ovl}.c` });

test('shared names: tools get the key, checkpoints and results are kept apart', () => {
  const s = swarm('--big-workers', '1');
  const a = slot('ovl5d'), b = slot('ovl60');
  assert.notEqual(s.cpDir(a), s.cpDir(b));
  assert.equal(path.basename(s.cpDir(a)), 'ovl5d_800B3A38');
  const p = s.prompt({ f: a, model: 'sonnet', attempt: 1, notes: [] });
  assert.match(p, /next --func ovl5d:800B3A38/);
  assert.match(p, /make diff FUNC=ovl5d:800B3A38/);
  assert.match(p, /Work only on func_800B3A38\./);
  const pb = s.prompt(s.bigJob(a, 1, []));
  assert.match(pb, /regions ovl5d:800B3A38/);
  assert.doesNotMatch(pb, /(FUNC=|regions |vars |similar )func_800B3A38/);
});

test('checkpoints saved by name still resume, for unique names only', () => {
  const s = swarm();
  const d = path.join(s.paths.checkpoints, 'func_800B3A38');
  fs.mkdirSync(d, { recursive: true });
  fs.writeFileSync(path.join(d, 'best.json'), JSON.stringify({ stage: 'registers', diffs: 3, total: 30 }));
  fs.writeFileSync(path.join(d, 'best.diff'), PATCH('func_800B3A38'));
  assert.equal(s.checkpoint(slot('ovl5d')), null);
  const unique = fn('func_800B3A38', { key: 'ovl5d:800B3A38' });
  assert.equal(s.checkpoint(unique).diffs, 3);
  s.saveCheckpoint({ f: slot('ovl60'), model: 'opus', base: 'x' }, path.join(d, 'best.diff'),
                   { stage: 'instructions', diffs: 9, total: 30 }, 'n');
  assert.equal(s.checkpoint(slot('ovl60')).diffs, 9);
  assert.equal(s.checkpoint(slot('ovl60')).key, 'ovl60:800B3A38');
  assert.equal(s.checkpoint(unique).diffs, 3);
});

// ---------------------------------------------------------------- environment errors

test('infraError: tools/dk messages, exit codes 125-127, Windows file locks; not a mismatch', async () => {
  const { infraError } = await import('./swarm.mjs');
  assert.match(infraError('ERROR: Docker daemon not reachable. Start Docker Desktop and retry.\n', 127), /daemon not reachable/);
  assert.match(infraError('', 125), /exited 125/);
  assert.match(infraError('PermissionError: [WinError 32] in use\n', 1), /PermissionError/);
  assert.equal(infraError('STAGE func_80012345 registers 3/120 (length 120 vs 120)\n', 1), null);
  assert.equal(infraError('MATCH func_80012345\n', 0), null);
});

test('environment errors and failed starts charge no attempt', () => {
  const s = swarm();
  const f = fn('func_80010000');
  s.infraStop(null, { f, model: 'sonnet', notes: [] }, 'Docker is not reachable');
  s.fail(null, { f, model: 'sonnet', notes: [] }, 'could not start', false);
  assert.equal(s.history[f.key].attempts, 0);
  assert.equal(s.stopping, 'graceful');
  assert.equal(s.consecutiveFailures, 1);
  s.fail(null, { f, model: 'sonnet', notes: [] }, 'bad patch');
  assert.equal(s.history[f.key].attempts, 1);
});

// ---------------------------------------------------------------- checkpoints that no longer apply

test('reapply finds the INCLUDE_ASM line by name when its asm path changed (a file split)', () => {
  const dir = clone({ 'src/main/main.c': 'int other;\nINCLUDE_ASM("asm/nonmatchings/main/split2", func_80012345);\n' });
  assert.equal(reapply(dir, PATCH('func_80012345'), fn('func_80012345')), null);
  assert.match(fs.readFileSync(path.join(dir, 'src/main/main.c'), 'utf8'), /void func_80012345\(void\) \{/);
});

test('a big session after a checkpoint that no longer applied: told to port it', () => {
  const s = swarm('--big-workers', '1');
  const f = fn('func_80030000', { size: 900 });
  const cp = { stage: 'registers', diffs: 4, total: 225, log: ['2026-10-15 opus: registers 4/225.'] };
  const p = s.prompt(s.bigJob(f, 1, [], { oldCheckpoint: { why: 'no unique anchor', diff: PATCH(f.name), cp } }));
  assert.match(p, /no longer applies to\nthis branch \(no unique anchor\)/);
  assert.match(p, /-INCLUDE_ASM/);
  assert.match(p, /2026-10-15 opus: registers 4\/225\./);
  assert.doesNotMatch(p, /No session has worked on it yet/);
});

test('the first C that builds replaces a checkpoint that no longer applied; failing to port is not stale', async () => {
  const s = swarm('--big-workers', '1');
  const f = fn('func_80030000', { size: 900 });
  const d = s.cpDir(f);
  fs.mkdirSync(d, { recursive: true });
  fs.writeFileSync(path.join(d, 'best.json'), JSON.stringify({ stage: 'registers', diffs: 4, total: 225 }));
  fs.writeFileSync(path.join(d, 'best.diff'), PATCH(f.name));
  const session = async score => {
    s.measure = async () => ({ score, infra: null });
    const job = s.bigJob(f, 1, [], { oldCheckpoint: { why: 'gone', diff: '', cp: s.checkpoint(f) } });
    job.dir = tmpDir();
    await s.onBigExit({ id: 'b1', dir: job.dir, job }, job, { status: 'progress', note: 'n' }, PATCH(f.name));
  };
  await session(null);                                      // did not build
  assert.equal(s.history[f.key].bigStale || 0, 0);
  assert.equal(s.checkpoint(f).diffs, 4);
  await session({ stage: 'structure', diffs: 60, total: 225 });
  assert.equal(s.checkpoint(f).stage, 'structure');
  assert.equal(s.history[f.key].bigStale, 0);
});

// ---------------------------------------------------------------- across runs

test('a function an earlier run parked once comes back escalated, with its notes', () => {
  const s = swarm('--max-attempts', '2');
  const f = fn('func_80010000');
  s.queue = [f];
  s.pipeline = Date.parse('2026-10-14T12:00:00Z');
  s.history[f.key] = { name: f.name, attempts: 1, notes: [
    '2026-10-01 10:00:00 sonnet: parked: $at was wrong (before the filter fix)',
    '2026-10-15 10:00:00 sonnet: parked: a register swap in the loop',
  ] };
  const j = s.nextJob();
  assert.equal(j.model, s.o.escalateModel);
  assert.equal(j.attempt, 2);
  assert.deepEqual(j.notes, ['[earlier run] 2026-10-15 10:00:00 sonnet: parked: a register swap in the loop']);
  assert.match(s.prompt(j), /\[earlier run\] 2026-10-15/);
  // and that attempt's failure goes to big mode, not round the ladder again
  s.notMatched({ job: j }, j, 'still a swap');
  assert.equal(s.retry[0].big, true);
});

test('a forgotten function starts from the bottom, told why', () => {
  const s = swarm();
  const f = fn('func_80010000');
  s.queue = [f];
  s.pipeline = 0;
  s.history[f.key] = { name: f.name, attempts: 0, notes: ['2026-10-16 09:00:00 reset (was 2 attempts): a header fix'] };
  const j = s.nextJob();
  assert.equal(j.model, s.o.model);
  assert.equal(j.attempt, 1);
  assert.match(j.notes[0], /a header fix/);
});

// ---------------------------------------------------------------- Diff patterns passed between workers

test('patterns: asm tricks dropped, duplicates once, same overlay first, capped', () => {
  const s = swarm();
  const eng = fn('func_80078CA4', { seg: 'overlays/engine/engine' }), main = fn('func_80012345');
  s.addPattern(main, '| main row | change | ex |');
  s.addPattern(eng, '| pin | `register s32 x asm("v1")` | ex |');
  s.addPattern(eng, '| barrier | `__asm__("" : "=r"(p))` | ex |');
  s.addPattern(eng, '| target has a gte_* asm barrier | reorder | ex |');   // mentions asm, teaches no asm
  s.addPattern(eng, '| engine row | change | ex |');
  s.addPattern(main, '| engine row | change | ex |');
  fs.appendFileSync(s.paths.patterns, '| old pin | `--hack` | ex |  <!-- func_1 -->\n');   // from before the filter
  const rows = s.patternsFor(fn('func_8007AC20', { seg: 'overlays/engine/8007ac20' })).split('\n');
  assert.deepEqual(rows, ['| engine row | change | ex |', '| target has a gte_* asm barrier | reorder | ex |', '| main row | change | ex |']);
  for (let i = 0; i < 100; i++) s.addPattern(main, `| row ${i} ${'x'.repeat(60)} | change | ex |`);
  assert.ok(s.patternsFor(main).length <= 2500);
});

test('both prompts forbid inline asm', () => {
  const s = swarm('--big-workers', '1');
  const f = fn('func_80010000');
  assert.match(s.prompt({ f, model: 'sonnet', attempt: 1, notes: [] }), /Pure C only: no `asm`/);
  assert.match(s.prompt(s.bigJob(f, 1, [])), /Pure C only: no `asm`/);
});

test('checkPatch refuses every inline asm spelling', () => {
  const s = swarm(), f = fn('func_80012345');
  for (const a of ['asm("nop");', '__asm("nop");', '__asm__ __volatile__("nop");', 'asm __volatile__("");',
                   'asm("" ::: "memory");', 'register int x __asm("$20");'])
    assert.match(s.checkPatch(PATCH(f.name, '    ' + a), f).join(), /inline asm/, a);
  assert.deepEqual(s.checkPatch(PATCH(f.name, '    gte_ldv0(p);'), f), []);
});

// ---------------------------------------------------------------- what a worker can touch

const allowRules = () => JSON.parse(fs.readFileSync(path.join(REPO, 'tools/swarm/config.json'), 'utf8')).allowedTools;

test('workers: Edit/Write only src/ include/ and the result file; no host commands that write or run', () => {
  assert.ok(allowRules().includes('Edit(.swarm-result.json)'));
  const allow = allowRules();
  for (const t of allow) {
    // path rules work only as Edit(...), which covers Write too: a Write(path) rule is ignored
    // (checked 2026-10-16: Write(.swarm-result.json) alone left workers unable to write their result)
    assert.doesNotMatch(t, /^Write\(/, t);
    if (/^Edit\b/.test(t)) assert.match(t, /^Edit\((src\/\*\*|include\/\*\*|\.swarm-result\.json)\)$/, t);
    assert.ok(!['Edit', 'Write'].includes(t), t);
    // find -exec, sort -o, sed e/w, uniq OUT, git --output / grep -O all write or run on the host
    assert.doesNotMatch(t, /^Bash\((find|sort|uniq|sed|git)\b/, t);
  }
});

test('workers: their containers mount .git, tools/ and .claude/ read-only', async () => {
  const { workerMounts } = await import('./swarm.mjs');
  const dir = clone({ '.git/HEAD': 'x', 'tools/dk': 'x', 'src/a.c': '' });
  const m = workerMounts(dir);
  assert.ok(m.includes(`${path.join(dir, '.git')}:/work/.git:ro`));
  assert.ok(m.includes(`${path.join(dir, 'tools')}:/work/tools:ro`));
  assert.doesNotMatch(m, /\.claude|src/);
});

// ---------------------------------------------------------------- what sessions cost

const PRICES = { 'claude-opus-5-5': [4, 20, 0.2, 8] };
const TRANSCRIPT = [
  // one API message split over two lines (thinking, then a tool call): its usage counts once
  { type: 'assistant', message: { id: 'm1', model: 'claude-opus-5-5', usage: { input_tokens: 1000, output_tokens: 100, cache_read_input_tokens: 0, cache_creation_input_tokens: 0 },
    content: [{ type: 'thinking' }] } },
  { type: 'assistant', message: { id: 'm1', model: 'claude-opus-5-5', usage: { input_tokens: 1000, output_tokens: 100, cache_read_input_tokens: 0, cache_creation_input_tokens: 0 },
    content: [{ type: 'tool_use', id: 't1', name: 'Read', input: { file_path: 'C:/x/docs/MATCHING.md' } }] } },
  { type: 'assistant', message: { id: 'm2', model: 'claude-opus-5-5', usage: { input_tokens: 0, output_tokens: 1000, cache_read_input_tokens: 1e6, cache_creation_input_tokens: 0 },
    content: [{ type: 'tool_use', id: 't2', name: 'Bash', input: { command: 'grep -n -i swap docs/MATCHING.md' } },
              { type: 'tool_use', id: 't3', name: 'Read', input: { file_path: 'src/main/main.c' } }] } },
  { type: 'assistant', isSidechain: true, message: { id: 'm3', model: 'claude-opus-5-5', usage: { input_tokens: 9e9, output_tokens: 0 } } },
].map(j => JSON.stringify(j)).join('\n') + '\n';

test('readTranscript: tokens once per API message, docs reads and greps', () => {
  const dir = clone({ 's.jsonl': TRANSCRIPT });
  const t = readTranscriptFn(path.join(dir, 's.jsonl'));
  assert.deepEqual(t.tokens, { in: 1000, out: 1100, cacheRead: 1e6, cacheWrite: 0 });
  assert.equal(t.turns, 2);
  assert.equal(t.docReads, 1);
  assert.equal(t.docGreps, 1);
});

test('sessionUsage: the result line when there is one, else the transcript priced and scaled', () => {
  const dir = clone({ 's.jsonl': TRANSCRIPT });
  const file = path.join(dir, 's.jsonl');
  const done = sessionUsageFn({ type: 'result', total_cost_usd: 0.5, num_turns: 7, duration_api_ms: 60000,
                                usage: { input_tokens: 1, output_tokens: 2 } }, file, PRICES);
  assert.equal(done.usd, 0.5);
  assert.equal(done.estimated, false);
  assert.equal(done.turns, 7);
  assert.ok(Math.abs(done.priced - 0.226) < 1e-9);          // (1000*4 + 1100*20 + 1e6*0.2) / 1e6
  const killed = sessionUsageFn({}, file, PRICES, 2);
  assert.ok(Math.abs(killed.usd - 0.452) < 1e-9);
  assert.equal(killed.estimated, true);
  assert.equal(killed.known, true);
  assert.equal(sessionUsageFn({}, null, PRICES).known, false);
});

test('ledger: one line per session, at its first outcome; stats reads it', () => {
  const s = swarm();
  const f = fn('func_80010000', { size: 120 });
  const job = { f, model: 'sonnet', attempt: 1, notes: [], sessionId: 'x',
                usage: { usd: 0.3, estimated: false, known: true, turns: 9, tokens: null, minutes: 1, apiMinutes: 0.5, docReads: 0, docGreps: 2 } };
  s.notMatched({ job }, job, 'no idea');          // records "parked"
  s.record(job, 'conflict', 'again');              // a later record of the same session: not a second line
  const lines = fs.readFileSync(s.paths.ledger, 'utf8').trim().split('\n').map(l => JSON.parse(l));
  assert.equal(lines.length, 1);
  assert.equal(lines[0].outcome, 'parked');
  assert.equal(lines[0].kind, 'normal');
  assert.equal(lines[0].key, f.key);
  const big = { f: fn('func_80020000', { size: 700 }), model: 'opus', attempt: 1, notes: [], big: true,
                usage: { usd: 3, estimated: true, known: true, turns: 50, minutes: 27 } };
  s.ledger(big, 'matched');
  const out = [];
  const log = console.log;
  console.log = (...a) => out.push(a.join(' '));
  try { ledgerStatsFn(s.paths.ledger, [0, 150, 600, Infinity]); } finally { console.log = log; }
  const text = out.join('\n');
  assert.match(text, /2 sessions.*1 killed sessions estimated from transcripts, \$3\.00/);
  assert.match(text, /0-149\s+normal\s+1\s+\$0\.30\s+0\s+-\s+0\/1/);
  assert.match(text, /600-\s+big\s+1\s+\$3\.00\s+1\s+\$3\.00\s+1\/1/);
});

// ---------------------------------------------------------------- the fixed context of a session

test('prompts: the loop inline, no SKILL.md; the 2.7.2 check only for main/; resumed big sessions skip the first checks', () => {
  const s = swarm('--big-workers', '1');
  const main = fn('func_80010000'), eng = fn('func_80078CA4', { seg: 'overlays/engine/engine', src: 'src/overlays/engine/engine.c' });
  const n = s.prompt({ f: main, model: 'sonnet', attempt: 1, notes: [] });
  assert.match(n, /The loop \(docs\/MATCHING\.md has the detail: grep it/);
  assert.doesNotMatch(n, /Follow \.claude\/skills/);
  assert.match(n, /gcc 2\.7\.2 \+ ASPSX/);
  assert.doesNotMatch(s.prompt({ f: eng, model: 'sonnet', attempt: 1, notes: [] }), /gcc 2\.7\.2/);
  const fresh = s.prompt(s.bigJob(eng, 1, []));
  assert.match(fresh, /Before matching code:\n- Type the data/);
  const resumed = s.prompt(s.bigJob(eng, 2, [], { resumed: { stage: 'registers', diffs: 3, total: 75, log: [] } }));
  assert.doesNotMatch(resumed, /Before matching code/);
  assert.match(resumed, /Earlier sessions did the first-session checks/);
});

test('workers: no git status or memory in the system prompt, no skills or todo tools', () => {
  const c = JSON.parse(fs.readFileSync(path.join(REPO, 'tools/swarm/config.json'), 'utf8'));
  assert.equal(c.workerEnv.CLAUDE_CODE_DISABLE_GIT_INSTRUCTIONS, '1');
  assert.ok(c.claudeArgs.includes('--disable-slash-commands'));
  assert.ok(c.disallowedTools.includes('TodoWrite') && !c.allowedTools.includes('TodoWrite'));
});
