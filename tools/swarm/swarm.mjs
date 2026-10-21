#!/usr/bin/env node
// Swarm orchestrator: N Claude workers match functions in parallel, one serial integrator
// commits each through the validation gate (decomp.py done). See tools/swarm/README.md.
//
//   node tools/swarm/swarm.mjs run [--workers 3] [--seg main/main] [--only f1,f2] ...
//   node tools/swarm/swarm.mjs stop [--now]     graceful: finish and integrate current work
//   node tools/swarm/swarm.mjs status
//   node tools/swarm/swarm.mjs stats [--run ID] outcomes and $ per match by function size; checkpoints
//   node tools/swarm/swarm.mjs clean            delete the worker and integration clones
//   node tools/swarm/swarm.mjs permute --only f1,f2 | --near [--minutes 30] [--jobs N]
//                                               permuter only (no Claude), from each one's checkpoint
//   node tools/swarm/swarm.mjs fold             fold patterns.md into docs/MATCHING.md (after a run)
//   node tools/swarm/swarm.mjs forget --only f1,f2 --why "..." [--checkpoint]
//                                               reset functions' attempts (a tool fix unblocked them)
//
// Runs on the host (Node, no dependencies): it launches `claude` and `tools/dk`, which can't
// run inside the toolchain container.
import { spawn, spawnSync } from 'node:child_process';
import crypto from 'node:crypto';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const REPO = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..', '..');
const DEFAULTS = JSON.parse(fs.readFileSync(path.join(REPO, 'tools/swarm/config.json'), 'utf8'));
const WIN = process.platform === 'win32';
const LABEL = 'crashbash-swarm';
// game data and tool downloads are git-ignored: each clone gets a copy
const DATA_DIRS = ['asm', 'linkers', 'ext'];   // splat's binary segments are in asm/bin; assets/ (the game data) isn't needed to match
const DISC_FILE_MAX = 16 << 20;           // disc/: the exe and overlay binaries, not the 170 MB image
const RESULT = '.swarm-result.json';
// gate output that means the environment is broken, not the function: the run stops at the first
// one and the function is not charged an attempt (its verified match stays resumable)
const INFRA_ERRORS = [
  /can't open (?:asm|linkers)\/\S+ for reading/,      // stale splat output in the clone
  /No rule to make target '[^']*'/,
  /Cannot connect to the Docker daemon|docker: Error response from daemon[^\n]*/,
  /No space left on device/,
  /error while loading shared libraries[^\n]*/,
  /ERROR: (?:docker not found|Docker daemon not reachable|could not hash tools\/docker\/Dockerfile)[^\n]*/,   // tools/dk
  /PermissionError: [^\n]*/,                          // Windows holding a file in the clone
  /TIMEOUT: tools\/dk killed after \d+ min/,           // a hung container (Swarm.dk's minutes)
];

// why the gate refused a match, for the log and the worker that fixes it: cc1's error lines (and
// their "previous declaration" partners) and the gate's verdict. They come before a screenful of
// warnings, so the output's last lines alone said only "Error 33", and gate-fix workers took that
// for a flaky build and changed nothing (2026-10-18: four refusals lost that way)
function gateReason(out) {
  const errs = out.split('\n').filter(l => /^\S+:\d+: (?!warning:)/.test(l) ||
                                           /\b(does not match|MISMATCH|BUILD FAILED)\b/.test(l));
  const uniq = [...new Set(errs)].slice(0, 14);
  return uniq.length ? uniq.join('\n') : out.trim().split('\n').slice(-6).join('\n');
}

// headers are union-merged (makeClone), so two matches that each added a declaration can leave
// both in the header: the usual reason a match that built in its worker fails at the gate
const DECL_CLASH = /conflicting types|redefinition|redeclar|has no member named|too (many|few) arguments|previous declaration/;
const DECL_HINT = `Other matches were committed while it waited, and headers are merged by keeping both
sides' lines, so the error above is most likely a declaration clash: a struct or typedef now defined
twice, or a prototype whose parameters differ from a caller or from another declaration. Keep one
declaration (merge a duplicate struct's fields into the existing one and use its field names), and
call a function whose callers pass other arguments than its definition takes through a cast
(MATCHING.md, Diff patterns). The whole file's build (make diff) shows every error.
`;

// an environment error in a tools/dk call's output, or its exit code: 125 docker run failed,
// 126 the command can't run, 127 not found (tools/dk's own checks exit 127 too)
function infraError(out, code) {
  const m = INFRA_ERRORS.map(re => out.match(re)).find(Boolean);
  if (m) return m[0];
  return [125, 126, 127].includes(code) ? `tools/dk exited ${code}: ${out.trim().split('\n').pop() || 'no output'}` : null;
}
const PATCH_MAX = 64 << 10;
const OLD_DIFF = '.swarm-old.diff';    // a checkpoint that no longer applies, for decomp.py port
// Diff patterns rows that teach what the gate refuses (pins, asm barriers, --hack) aren't passed on
const PATTERN_REFUSED = /\b(?:asm|__asm|__asm__)\s*(?:volatile|__volatile__)?\s*\(|--hack|\bregister\b[^|]*\b(?:asm|__asm|__asm__)\b/i;
const PATTERNS_MAX = 2500;
// the matching loop both prompts give inline (workers then needn't read SKILL.md, whose steps the
// swarm overrides, and the docs are for grepping)
const LOOP = (f, r) => `The loop (docs/MATCHING.md has the detail: grep it for your symptom, e.g.
\`grep -n -i 'WORD' docs/MATCHING.md\`, and never Read it whole; skip AGENT_LOG.md, CONTRIBUTING.md,
CONVENTIONS.md and SKILL.md):
1. \`tools/dk python3 tools/scripts/decomp.py next --func ${r}\` prints the asm, an m2c draft and
   relatives (identical or near copies, callers). Read the asm, not just the draft: m2c misses
   pass-through arguments, signedness and struct offsets.${f.twin ? `
   It also prints the C of a matched function with the same code (${f.twin === 'template'
     ? 'a look-alike' : `an ${f.twin} twin`}): adapt that rather than the m2c draft.` : ''}${/^main\//.test(f.seg || '') ? `
   If every global store is \`lui $at\` + \`sw\`, or jump delay slots are filled from a store, the code
   is gcc 2.7.2 + ASPSX: in a 2.8.1 file it can't match; say so in "note" and stop.` : ''}
2. Evidence before writing: \`decomp.py similar ${r}\` (matched functions with the same code, and
   where their C is), the relatives \`next\` printed, and \`grep\` in src/ for the globals it uses.
3. Write C89 in place of its INCLUDE_ASM line; types and declarations in the owned header of
   ${f.src} (the include/bash/*.h marked \`owner: ${f.src}\`); struct fields over pointer arithmetic.
4. \`tools/dk make diff FUNC=${r}\`: fix the STAGE its last line names, in order: structure (length,
   branches, loops), then instructions (order, temps, types), then registers. Its KINDS line says
   which "Classify the diff" list in MATCHING.md applies, and the Diff patterns table there has
   fixes by symptom (grep it). After MATCH, its LINK line says whether the whole binary still
   matches: fix a LINK difference (data or rodata the C adds or moves) before finishing.
5. For register swaps: \`decomp.py vars ${r}\` names the variable behind each, and
   \`decomp.py splitvars ${r} VAR\` when one variable wants different registers in different
   blocks. For a long diff, \`decomp.py regions ${r}\` aligns it block by block (\`--changed\` after
   an edit).
`;
const ASM_REFUSED = 'adds inline asm';   // checkPatch's finding, measured anyway
const TOOL_MINUTES = 10;                  // the orchestrator's try/regions; done and verify get 2x
const NEIGHBOUR = 8;            // lines between two INCLUDE_ASMs below which they aren't leased together
// environment of the session that started us: a nested claude must not inherit it
const STRIP_ENV = /^(CLAUDECODE|CLAUDE_CODE_(SESSION_ID|ENTRYPOINT|CHILD_SESSION|HOST_SESSION_ID|MESSAGING_\w+|SESSION_ATTENDED)|CLAUDE_PID)$/;

// ---------------------------------------------------------------- options

function parseArgs(argv) {
  const [cmd = 'help', ...rest] = argv;
  const o = { ...DEFAULTS };
  for (let i = 0; i < rest.length; i++) {
    const m = rest[i].match(/^--([\w-]+)(?:=(.*))?$/);
    if (!m) die(`unexpected argument ${rest[i]}`);
    const key = m[1].replace(/-(\w)/g, (_, c) => c.toUpperCase());
    let val = m[2];
    if (val === undefined) val = rest[i + 1] !== undefined && !rest[i + 1].startsWith('--') ? rest[++i] : true;
    if (typeof DEFAULTS[key] === 'number') val = Number(val);
    if (typeof DEFAULTS[key] === 'boolean' && typeof val === 'string') val = val !== 'false';
    o[key] = val;
  }
  o.dir = path.resolve(o.dir || `${REPO}-swarm`);
  o.only = o.only ? String(o.only).split(',').map(s => s.trim()).filter(Boolean) : null;
  return [cmd, o];
}

// ---------------------------------------------------------------- helpers

function die(msg) {
  console.error('swarm: ' + msg);
  process.exit(2);
}

const sleep = ms => new Promise(r => setTimeout(r, ms));
const stamp = () => new Date().toISOString().replace('T', ' ').slice(0, 19);

function sh(cmd, args, opts = {}) {
  const r = spawnSync(cmd, args, { encoding: 'utf8', maxBuffer: 256 << 20, ...opts });
  return { code: r.status ?? -1, out: r.stdout || '', err: (r.stderr || '') + (r.error ? String(r.error) : '') };
}

function git(cwd, ...args) {
  return sh('git', args, { cwd });
}

function gitOk(cwd, ...args) {
  const r = git(cwd, ...args);
  if (r.code) throw new Error(`git ${args.join(' ')} (in ${cwd}): ${(r.err || r.out).trim()}`);
  return r.out.trim();
}

function which(name) {
  if (path.isAbsolute(name)) return fs.existsSync(name) ? name : null;
  const exts = WIN ? ['.exe', '.cmd', '.bat', ''] : [''];
  for (const d of (process.env.PATH || '').split(path.delimiter))
    for (const e of exts) {
      const p = path.join(d, name + e);
      if (d && fs.existsSync(p) && fs.statSync(p).isFile()) return p;
    }
  return null;
}

// npm's claude.cmd wraps a native claude.exe: run that directly (no cmd.exe quoting)
function realClaude(p) {
  if (!p || !/\.cmd$/i.test(p)) return p;
  const exe = path.join(path.dirname(p), 'node_modules', '@anthropic-ai', 'claude-code', 'bin', 'claude.exe');
  return fs.existsSync(exe) ? exe : p;
}

// Git Bash, not WSL's bash.exe, which comes first on a Windows PATH
function findBash() {
  if (!WIN) return 'bash';
  const exec = sh('git', ['--exec-path']).out.trim();   // <git>/mingw64/libexec/git-core
  const p = path.resolve(exec, '..', '..', '..', 'bin', 'bash.exe');
  if (!fs.existsSync(p)) die(`Git Bash not found at ${p}`);
  return p;
}

function alive(pid) {
  if (!pid) return false;
  try {
    process.kill(pid, 0);
    return true;
  } catch (e) {
    return e.code === 'EPERM';
  }
}

function killTree(pid) {
  if (!pid) return;
  if (WIN) sh('taskkill', ['/PID', String(pid), '/T', '/F']);
  else {
    try { process.kill(-pid, 'SIGKILL'); } catch { try { process.kill(pid, 'SIGKILL'); } catch { /* gone */ } }
  }
}

// containers with this label, and (labels need our tools/dk) any that mount a path under `under`
function dockerKill(filter, under) {
  const ids = new Set(sh('docker', ['ps', '-q', '--filter', `label=${filter}`]).out.split(/\s+/).filter(Boolean));
  if (under) {
    const all = sh('docker', ['ps', '-q']).out.split(/\s+/).filter(Boolean);
    const norm = p => path.resolve(p).toLowerCase() + path.sep;
    const root = norm(under);
    if (all.length)
      for (const line of sh('docker', ['inspect', '--format', '{{.Id}} {{range .Mounts}}{{.Source}}|{{end}}', ...all]).out.split('\n')) {
        const sp = line.indexOf(' ');
        const id = line.slice(0, sp), mounts = line.slice(sp + 1).trim();
        if (sp > 0 && mounts.split('|').some(m => m && norm(m).startsWith(root))) ids.add(id.slice(0, 12));
      }
  }
  if (ids.size) {
    sh('docker', ['unpause', ...ids]);   // a paused container (permuter during a gate build) refuses kill
    sh('docker', ['kill', ...ids]);
  }
  return ids.size;
}

function readJson(p, dflt) {
  try { return JSON.parse(fs.readFileSync(p, 'utf8')); } catch { return dflt; }
}

function writeJson(p, obj) {
  fs.writeFileSync(p + '.tmp', JSON.stringify(obj, null, 1) + '\n');
  fs.renameSync(p + '.tmp', p);
}

// a file's last max bytes, from the first whole line
function readTail(p, max) {
  let fd;
  try {
    fd = fs.openSync(p, 'r');
    const size = fs.fstatSync(fd).size, len = Math.min(size, max), buf = Buffer.alloc(len);
    fs.readSync(fd, buf, 0, len, size - len);
    const s = buf.toString('utf8');
    return len < size ? s.slice(s.indexOf('\n') + 1) : s;
  } catch {
    return '';
  } finally {
    if (fd !== undefined) fs.closeSync(fd);
  }
}

// the runs before this one, newest first (dry and scripted runs left out): when and what from
// swarm.log's start and finish lines (its last 512 KB), sessions and spend from ledger.jsonl
function readRuns(dir, skip) {
  const runs = new Map(), utc = s => s.replace(' ', 'T') + 'Z';
  for (const line of readTail(path.join(dir, 'swarm.log'), 512 << 10).split('\n')) {
    const fin = line.match(/^(\S+ \S+) run (\d{14}) finished: (\d+) matched, (\d+) parked, (\d+) failed/);
    if (fin) {
      if (runs.has(fin[2])) Object.assign(runs.get(fin[2]), { ended: utc(fin[1]), matched: +fin[3], parked: +fin[4], failed: +fin[5] });
      continue;
    }
    const st = line.match(/^(\S+ \S+) run (\d{14}): (.*)$/);
    if (!st) continue;
    const only = st[3].match(/\(--only ([^)]*)\)/), seg = st[3].match(/ in ([^,]+)/);
    runs.set(st[2], { id: st[2], started: utc(st[1]), ended: null, target: only ? `--only ${only[1]}` : seg ? seg[1] : '?',
                      matched: 0, parked: 0, failed: 0, sessions: 0, usd: 0,
                      scripted: /, DRY RUN|, worker command:/.test(st[3]) });
  }
  const ledger = path.join(dir, 'ledger.jsonl');
  if (fs.existsSync(ledger))
    for (const l of fs.readFileSync(ledger, 'utf8').split('\n')) {
      if (!l) continue;
      let r;
      try { r = JSON.parse(l); } catch { continue; }
      const run = runs.get(r.run);
      if (run) {
        run.sessions++;
        run.usd += r.usd || 0;
      }
    }
  return [...runs.values()].filter(r => r.id !== skip && !r.scripted).reverse()
    .map(({ scripted, ...r }) => ({ ...r, usd: +r.usd.toFixed(2) }));
}

// ---------------------------------------------------------------- clones

// A clone (not a worktree: its .git must be a real directory for git inside the container),
// with the git-ignored data copied in after checkout so make sees it as newer than the sources.
function makeClone(dir, src, rev, log) {
  if (!fs.existsSync(path.join(dir, '.git'))) {
    log(`cloning ${path.basename(dir)}`);
    fs.mkdirSync(path.dirname(dir), { recursive: true });
    gitOk(path.dirname(dir), 'clone', '-q', '--no-checkout', '-c', 'core.autocrlf=false', src, dir);
    gitOk(dir, 'config', 'core.hooksPath', 'tools/hooks');   // git refuses this one at clone time
    gitOk(dir, 'remote', 'set-url', '--push', 'origin', 'PUSH-DISABLED');
  }
  // headers: keep both sides' added lines when patches meet. Every match appends its prototype to
  // the end of bash/main.h, so without this a worker patch that touches the end of main.h
  // conflicts with every commit made while it worked. A bad union (a duplicate declaration) fails
  // the gate's compile. Clone-local (.git/info), not the repo's .gitattributes.
  fs.mkdirSync(path.join(dir, '.git', 'info'), { recursive: true });
  fs.writeFileSync(path.join(dir, '.git', 'info', 'attributes'), 'include/**/*.h merge=union\n');
  // a branch or HEAD fetches; a bare commit id may not, but the clone usually has it already
  let target = 'FETCH_HEAD';
  if (git(dir, 'fetch', '-q', src, rev).code) {
    git(dir, 'fetch', '-q', src);
    target = rev;
  }
  try { fs.chmodSync(path.join(dir, 'tools', 'dk'), 0o755); } catch { /* none */ }   // useOurDk's
  gitOk(dir, 'checkout', '-q', '-f', '--detach', target);
  // asm/ and linkers/ are splat's output, so they go stale when the symbols or yamls change (a
  // symbol split adds an .s file the clone's copy lacks): copy again when their fingerprint differs.
  const stampFile = path.join(dir, '.git', 'swarm-data');
  const fp = splatFingerprint();
  if (fs.existsSync(stampFile) && fs.readFileSync(stampFile, 'utf8').includes(fp)) return;
  log(`copying game data into ${path.basename(dir)}`);
  for (const d of DATA_DIRS) {
    if (!fs.existsSync(path.join(REPO, d))) continue;
    if (d === 'asm' || d === 'linkers') fs.rmSync(path.join(dir, d), { recursive: true, force: true });
    fs.cpSync(path.join(REPO, d), path.join(dir, d), { recursive: true });
  }
  fs.mkdirSync(path.join(dir, 'disc'), { recursive: true });
  for (const f of fs.readdirSync(path.join(REPO, 'disc'))) {
    const p = path.join(REPO, 'disc', f), s = fs.statSync(p);
    if (s.isFile() && s.size <= DISC_FILE_MAX) fs.copyFileSync(p, path.join(dir, 'disc', f));
  }
  const drafts = path.join(REPO, 'build', 'auto_drafts');
  if (fs.existsSync(drafts)) fs.cpSync(drafts, path.join(dir, 'build', 'auto_drafts'), { recursive: true });
  // decomp.py's opcode sequences and look-alikes (~20 s and ~45 s to rebuild from asm/)
  fs.mkdirSync(path.join(dir, 'build'), { recursive: true });
  for (const c of ['similar.json', 'lookalike.json'])
    if (fs.existsSync(path.join(REPO, 'build', c))) fs.copyFileSync(path.join(REPO, 'build', c), path.join(dir, 'build', c));
  fs.writeFileSync(stampFile, `${stamp()} ${fp}\n`);
}

// what splat's output in REPO was made from: configs/USA's sym and rel files and yamls
function splatFingerprint() {
  const h = crypto.createHash('sha1');
  for (const sub of ['configs/USA', 'configs/USA/overlays']) {
    const d = path.join(REPO, sub);
    if (!fs.existsSync(d)) continue;
    for (const f of fs.readdirSync(d).sort())
      if (/\.(txt|yaml)$/.test(f)) h.update(f).update(fs.readFileSync(path.join(d, f)));
  }
  return 'splat:' + h.digest('hex').slice(0, 16);
}

// this checkout's tools/dk (labels, --cpus) in a clone; never committed (done commits src/ include/
// configs/ .decomp/ docs/), and every reset puts the clone's own back. Read-only: tools/dk runs on
// the host, so a worker must not be able to rewrite it (see workerMounts)
function useOurDk(dir) {
  const dk = path.join(dir, 'tools', 'dk');
  try { fs.chmodSync(dk, 0o755); } catch { /* not there yet */ }
  fs.copyFileSync(path.join(REPO, 'tools', 'dk'), dk);
  fs.chmodSync(dk, 0o555);
}

// a worker's containers see the clone's .git, tools/ and .claude/ read-only: from inside one it
// could otherwise rewrite what the host runs (tools/dk, hooks, git config such as core.fsmonitor,
// the session's settings). Workers run git through tools/dk for the same reason
function workerMounts(dir) {
  return ['.git', 'tools', '.claude'].filter(d => fs.existsSync(path.join(dir, d)))
    .map(d => `-v ${path.join(dir, d)}:/work/${d}:ro`).join(' ');
}

// Re-apply a worker's patch whose context no longer matches: the target's INCLUDE_ASM line is
// replaced by the worker's C, and lines the patch only adds go back after the same anchor line
// (the nearest non-blank context line above them, if it's unique in the file, or the end of the C
// that replaced the INCLUDE_ASM line). Added declarations the file already has (a prototype `done`
// added) are skipped whole, by declared name, never line by line: a struct's member line that
// another struct also has stays. Anything else (the patch edits or deletes another line, an anchor
// is gone or ambiguous) gives up. Writes nothing unless every file works out.
// Returns null when done, else why not. The gate checks the result like any other.
const INCLUDE_LINE = /^\s*INCLUDE_ASM\(\s*"[^"]+"\s*,\s*(\w+)\s*\)/;
// as decomp.py's DECL_NAME: the name a declaration ending in `;` declares
const DECL_NAME = /\b(\w+)\s*(?:\([^;{}]*\)|\[[^\]]*\])?\s*;\s*$/;

// lines -> whole declarations: a blank, preprocessor or comment line, or everything up to the `;`
// (or a body's closing `}`) that ends a declaration at brace depth 0
function declUnits(lines) {
  const units = [];
  let cur = [], depth = 0;
  for (const l of lines) {
    cur.push(l);
    const code = l.replace(/\/\*.*?\*\/|\/\/.*$/g, '').trim();
    depth += (code.match(/{/g) || []).length - (code.match(/}/g) || []).length;
    if (depth <= 0 && ((cur.length === 1 && (!code || code.startsWith('#'))) || /[;}]$/.test(code))) {
      units.push(cur);
      cur = [];
      depth = 0;
    }
  }
  if (cur.length) units.push(cur);
  return units;
}

// what a declaration declares: a typedef's, variable's or function's name, else a struct's tag
function declName(unit) {
  const code = unit.join('\n').replace(/\/\*[\s\S]*?\*\/|\/\/.*$/gm, '').trim();
  if (!code || code.startsWith('#')) return null;
  const m = code.match(DECL_NAME);
  if (m) return m[1];
  const tag = code.match(/^(?:typedef\s+)?(?:struct|union|enum)\s+(\w+)\s*\{/);
  return tag ? tag[1] : null;
}

function reapply(dir, patch, f) {
  const out = {};
  let replaced = false;
  const unique = (lines, s) => {
    const i = lines.indexOf(s);
    return i >= 0 && lines.indexOf(s, i + 1) < 0 ? i : -1;
  };
  for (const sec of patch.split(/^diff --git /m).slice(1)) {
    const file = (sec.match(/^a\/\S+ b\/(\S+)/) || [])[1];
    if (!file) return 'unreadable patch';
    if (/^(new file|deleted file|rename from)/m.test(sec)) return `${file} is new, deleted or renamed`;
    const p = path.join(dir, file);
    if (!out[file] && !fs.existsSync(p)) return `${file} is missing`;
    const lines = out[file] ||= fs.readFileSync(p, 'utf8').split('\n');
    for (const hunk of sec.split(/^@@[^\n]*\n/m).slice(1)) {
      // the anchor: a context line's text, or (after the INCLUDE_ASM line's replacement) an index
      let anchor = null, at = null, blanks = 0, del = null, run = [];
      const flush = () => {
        if (!run.length && !del) return null;
        if (del) {                              // the INCLUDE_ASM line -> the worker's C
          // by its text, else by the function's name (its asm path changes when a file is split)
          let i = unique(lines, del);
          if (i < 0) {
            const own = lines.map((l, k) => [k, l.match(INCLUDE_LINE)?.[1]]).filter(([, n]) => n === del.match(INCLUDE_LINE)[1]);
            if (own.length === 1) i = own[0][0];
          }
          if (i < 0) return `${f.name}'s INCLUDE_ASM line isn't there once`;
          lines.splice(i, 1, ...run);
          replaced = true;
          at = i + run.length - 1;              // lines added after it go after the C, not inside it
          anchor = null;
          blanks = 0;
        } else {                                // added lines (declarations) after their anchor
          const i = at !== null ? at : anchor === null ? -1 : unique(lines, anchor);
          if (i < 0) return `no unique anchor for an added line in ${file}`;
          const have = new Set(declUnits(lines).map(declName).filter(Boolean));
          const units = declUnits(run).filter(u => !have.has(declName(u)));
          const add = units.some(u => u.some(l => l.trim())) ? units.flat() : [];
          lines.splice(i + 1 + blanks, 0, ...add);
          if (at !== null) at += add.length;
        }
        del = null;
        run = [];
        return null;
      };
      for (const l of hunk.split('\n')) {
        if (!l || l.startsWith('\\')) continue;
        const t = l.slice(1);
        if (l[0] === '+') { run.push(t); continue; }
        if (l[0] === '-') {
          const m = t.match(INCLUDE_LINE);
          if (!m || ![f.name, `func_${f.key}`].includes(m[1])) return `it changes an existing line in ${file}`;
          const err = flush();
          if (err) return err;
          del = t;
          continue;
        }
        const err = flush();                    // a context line ends a run
        if (err) return err;
        if (t.trim()) { anchor = t; at = null; blanks = 0; } else if (anchor !== null || at !== null) blanks++;
      }
      const err = flush();
      if (err) return err;
    }
  }
  if (!replaced) return `no INCLUDE_ASM replacement for ${f.name} in the patch`;
  for (const [file, lines] of Object.entries(out)) fs.writeFileSync(path.join(dir, file), lines.join('\n'));
  return null;
}

// what to pass to decomp.py for a function: the queue's ref (its key when the slot overlays share
// its name, which find_func would refuse as ambiguous), else the name
const ref = f => f.ref || f.name;
// a function's directory under results/ and checkpoints/: by key (unique), ':' made path-safe
const keyDir = f => (f.key || f.name).replace(':', '_');

// back to `rev`, dropping every tracked change and untracked file (ignored data stays)
function resetClone(dir, rev) {
  try { fs.chmodSync(path.join(dir, 'tools', 'dk'), 0o755); } catch { /* none */ }   // useOurDk's
  gitOk(dir, 'reset', '-q', '--hard', rev);
  gitOk(dir, 'clean', '-fdq');
}

// ---------------------------------------------------------------- what a session cost

// where Claude Code keeps session transcripts (<project>/<session id>.jsonl)
const CLAUDE_PROJECTS = path.join(process.env.CLAUDE_CONFIG_DIR || path.join(os.homedir(), '.claude'), 'projects');

function transcriptOf(sessionId) {
  if (!sessionId || !fs.existsSync(CLAUDE_PROJECTS)) return null;
  for (const d of fs.readdirSync(CLAUDE_PROJECTS)) {
    const p = path.join(CLAUDE_PROJECTS, d, `${sessionId}.jsonl`);
    if (fs.existsSync(p)) return p;
  }
  return null;
}

// a transcript's model, token totals (one usage per API message: a message's content blocks are
// separate lines repeating it) and how often it went to docs/*.md (Read; Grep or a grep command)
function readTranscript(file) {
  const byId = new Map(), tools = new Set();
  let model = null, docReads = 0, docGreps = 0;
  for (const line of fs.readFileSync(file, 'utf8').split('\n')) {
    if (!line.startsWith('{')) continue;
    let j;
    try { j = JSON.parse(line); } catch { continue; }
    if (j.isSidechain || j.type !== 'assistant' || !j.message) continue;
    if (j.message.usage && j.message.id) {
      byId.set(j.message.id, j.message.usage);
      model = j.message.model || model;
    }
    for (const c of Array.isArray(j.message.content) ? j.message.content : []) {
      if (c.type !== 'tool_use' || tools.has(c.id)) continue;
      tools.add(c.id);
      const arg = String(c.input?.file_path || c.input?.path || c.input?.command || '');
      if (!/\bdocs[\\/]\w+\.md\b|\bdocs[\\/]?$/.test(arg)) continue;
      if (c.name === 'Read') docReads++;
      else if (c.name === 'Grep' || (c.name === 'Bash' && /\bgrep\b/.test(arg))) docGreps++;
    }
  }
  const tokens = { in: 0, out: 0, cacheRead: 0, cacheWrite: 0 };
  for (const u of byId.values()) {
    tokens.in += u.input_tokens || 0;
    tokens.out += u.output_tokens || 0;
    tokens.cacheRead += u.cache_read_input_tokens || 0;
    tokens.cacheWrite += u.cache_creation_input_tokens || 0;
  }
  return { model, tokens, turns: byId.size, docReads, docGreps };
}

// $ for tokens at config.json's prices ($ per million: input, output, cache read, cache write)
function priceOf(model, t, prices) {
  const p = prices?.[model];
  return p ? (t.in * p[0] + t.out * p[1] + t.cacheRead * p[2] + t.cacheWrite * p[3]) / 1e6 : 0;
}

// a session's usage: its result line's cost when it finished; killed (at its deadline, by a stop),
// the transcript's tokens priced, times `scale` (this run's reported / estimated cost so far)
function sessionUsage(result, transcript, prices, scale = 1) {
  const t = transcript ? readTranscript(transcript) : null;
  const done = result?.type === 'result' && typeof result.total_cost_usd === 'number';
  const ru = result?.usage;
  return {
    usd: done ? result.total_cost_usd : t ? priceOf(t.model, t.tokens, prices) * scale : 0,
    estimated: !done,
    known: done || !!(t && prices?.[t.model]),
    model: t?.model || Object.keys(result?.modelUsage || {})[0] || null,
    tokens: ru ? { in: ru.input_tokens || 0, out: ru.output_tokens || 0, cacheRead: ru.cache_read_input_tokens || 0,
                   cacheWrite: ru.cache_creation_input_tokens || 0 } : t?.tokens || null,
    turns: result?.num_turns ?? t?.turns ?? null,
    apiMinutes: done && result.duration_api_ms ? +(result.duration_api_ms / 60e3).toFixed(2) : null,
    docReads: t ? t.docReads : null, docGreps: t ? t.docGreps : null,
    // for calibrating estimates: the same session priced from its transcript
    priced: done && t ? priceOf(t.model, t.tokens, prices) : null,
  };
}

// ---------------------------------------------------------------- the run

class Swarm {
  constructor(o) {
    this.o = o;
    this.id = new Date().toISOString().replace(/\D/g, '').slice(0, 14);
    this.bash = findBash();
    this.integ = path.join(o.dir, 'integ');
    this.paths = {
      lock: path.join(o.dir, 'lock'), stop: path.join(o.dir, 'STOP'), status: path.join(o.dir, 'run.json'),
      log: path.join(o.dir, 'swarm.log'), history: path.join(o.dir, 'history.json'),
      report: path.join(o.dir, 'report.md'), patterns: path.join(o.dir, 'patterns.md'),
      results: path.join(o.dir, 'results'), checkpoints: path.join(o.dir, 'checkpoints'),
      ledger: path.join(o.dir, 'ledger.jsonl'), permuter: path.join(o.dir, 'permuter.jsonl'),
    };
    this.slots = Array.from({ length: o.workers }, (_, i) => ({ id: `w${i + 1}`, dir: path.join(o.dir, 'workers', `w${i + 1}`), job: null }));
    // big mode: long resumable sessions on big functions (>= --big-bytes, or parked in normal mode)
    this.bslots = Array.from({ length: o.bigWorkers }, (_, i) => ({ id: `b${i + 1}`, dir: path.join(o.dir, 'workers', `b${i + 1}`), job: null, big: true }));
    // permuter pool: plain compute, no Claude; a stuck worker hands its closest C here and frees its slot
    this.pslots = Array.from({ length: o.permuters }, (_, i) => ({ id: `p${i + 1}`, dir: path.join(o.dir, 'permuters', `p${i + 1}`), job: null }));
    this.permQueue = [];
    this.children = new Set();      // every process we started, for the hard stop
    this.queue = [];
    this.retry = [];                // jobs to run again (escalated model, or after a conflict)
    this.integQueue = [];
    this.integrating = null;
    this.started = Date.now();
    this.leases = 0;
    this.finished = new Set();      // keys done with this run (matched, parked or failed)
    this.stats = { matched: 0, parked: 0, failed: 0, conflicts: 0 };
    this.costUsd = 0;
    this.calib = { reported: 0, priced: 0 };   // finished sessions: reported $ vs their transcript priced
    this.consecutiveFailures = 0;
    this.stopping = null;
    this.history = readJson(this.paths.history, {});
    // for the dashboard's swarm card (writePage): the log's last lines, this run's outcomes
    this.logTail = [];
    this.results = [];
    this.sessions = 0;
    // half the cores for workers' builds, half for the permuters (all to workers without a pool)
    const ncpu = os.cpus().length, share = o.permuters ? ncpu / 2 : ncpu;
    this.cpus = Math.max(1, Math.floor(share / o.workers));
    this.permCpus = Math.max(1, Math.floor(share / Math.max(1, o.permuters)));
  }

  log(msg) {
    const line = `${stamp()} ${msg}`;
    console.log(line);
    fs.appendFileSync(this.paths.log, line + '\n');
    this.logTail.push(line.length > 400 ? line.slice(0, 400) + ' ...' : line);
    if (this.logTail.length > 12) this.logTail.shift();
  }

  // ---- safeguards before anything starts
  preflight() {
    const o = this.o;
    if (!(o.workers >= 1 && o.workers <= 16)) die('--workers must be 1..16');
    if (/\s/.test(o.dir)) die(`--dir ${o.dir} has a space: tools/dk splits DK_DOCKER_ARGS (the worker mounts) on spaces`);
    // commits go to <remote>/<branch> (GitHub main by default), never into your checkout directly
    const named = git(REPO, 'remote', 'get-url', o.remote);
    this.remoteUrl = named.code === 0 ? named.out.trim() : o.remote;
    this.remoteName = named.code === 0 ? o.remote : null;
    if (git(REPO, 'ls-remote', '-q', this.remoteUrl).code) die(`can't reach remote ${o.remote} (${this.remoteUrl})`);
    if (sh('docker', ['info']).code) die('Docker is not reachable');
    if (!o.workerCmd && !o.dryRun) {
      this.claude = realClaude(which(o.claudeBin));
      if (!this.claude) die(`${o.claudeBin} not found on PATH (install Claude Code or pass --claude-bin)`);
      // signed out, every worker would fail at once
      const auth = sh(this.claude, ['auth', 'status'], { timeout: 60e3 });
      let st = null;
      try { st = JSON.parse(auth.out); } catch { /* an older CLI: no JSON */ }
      if (auth.code || st?.loggedIn === false) die(`${o.claudeBin} is not signed in (\`claude auth status\`): ${(auth.err || auth.out).trim().slice(0, 200)}`);
    }
    fs.mkdirSync(o.dir, { recursive: true });
    fs.mkdirSync(this.paths.results, { recursive: true });
    if (fs.existsSync(this.paths.stop)) {
      if (!o.clearStop) die(`${this.paths.stop} exists: the swarm was told to stay off. Delete it or pass --clear-stop`);
      fs.unlinkSync(this.paths.stop);
    }
    const lock = readJson(this.paths.lock, null);
    if (lock && alive(lock.pid)) die(`another run (pid ${lock.pid}) owns ${o.dir}`);
    writeJson(this.paths.lock, { pid: process.pid, id: this.id, started: stamp() });
  }

  // integration clone at the remote branch's tip (the branch is created from --base if it's new)
  setupInteg() {
    const o = this.o;
    makeClone(this.integ, REPO, 'HEAD', m => this.log(m));
    if (!this.syncInteg()) {
      if (git(this.integ, 'fetch', '-q', REPO, o.base).code) die(`--base ${o.base} not found`);
      resetClone(this.integ, 'FETCH_HEAD');
      gitOk(this.integ, 'push', '-q', this.remoteUrl, `HEAD:refs/heads/${o.branch}`);
      this.log(`created ${o.branch} on ${o.remote} from ${o.base}`);
    }
    this.log(`integrating into ${o.remote}/${o.branch} at ${gitOk(this.integ, 'rev-parse', '--short', 'HEAD')}`);
  }

  // integration clone to the remote branch's tip: others (you, a hand session) may have pushed
  syncInteg() {
    if (git(this.integ, 'fetch', '-q', this.remoteUrl, `refs/heads/${this.o.branch}`).code) return false;
    resetClone(this.integ, 'FETCH_HEAD');
    this.tip = gitOk(this.integ, 'rev-parse', 'HEAD');
    return true;
  }

  // your checkout follows along when it's on the branch and has no uncommitted changes to tracked files
  updateLocal() {
    if (!this.remoteName) return;
    git(REPO, 'fetch', '-q', this.remoteName, this.o.branch);
    const head = git(REPO, 'symbolic-ref', '--short', 'HEAD').out.trim();
    const dirty = git(REPO, 'status', '--porcelain', '--untracked-files=no').out.trim();
    const ff = head === this.o.branch && !dirty &&
      git(REPO, 'merge', '-q', '--ff-only', `${this.remoteName}/${this.o.branch}`).code === 0;
    if (!ff && !this.warnedLocal) {
      this.warnedLocal = true;
      this.log(`note: your checkout isn't updated (${head !== this.o.branch ? `on ${head}` : dirty ? 'uncommitted changes' : 'diverged'}); ` +
               `\`git pull\` gets the swarm's commits`);
    }
  }

  // tools/dk in a clone, async; containers are labelled for the stop switch. `minutes` kills it.
  dk(cwd, args, slot = 'integ', { cpus = this.cpus, minutes = 0 } = {}) {
    useOurDk(cwd);
    return new Promise(resolve => {
      const env = {
        ...process.env, MSYS_NO_PATHCONV: '1', DK_NAME: `swarm-${slot}`,
        DK_DOCKER_ARGS: `--label ${LABEL}=${this.id} --label ${LABEL}-slot=${this.id}-${slot}` +
          // parallel make for done's full build, whatever the branch's decomp.py does
          (slot === 'integ' ? ` -e MAKEFLAGS=-j${os.cpus().length}` : ` --cpus ${cpus}`),
      };
      const p = spawn(this.bash, ['tools/dk', ...args], { cwd, env, stdio: ['ignore', 'pipe', 'pipe'], detached: !WIN });
      this.children.add(p);
      let out = '', timedOut = false;
      const timer = minutes && setTimeout(() => {
        timedOut = true;
        killTree(p.pid);
        dockerKill(`${LABEL}-slot=${this.id}-${slot}`, cwd);
      }, minutes * 60e3);
      p.stdout.on('data', d => { out += d; });
      p.stderr.on('data', d => { out += d; });
      p.on('close', code => {
        clearTimeout(timer);
        this.children.delete(p);
        if (timedOut) out += `\nTIMEOUT: tools/dk killed after ${minutes} min\n`;
        resolve({ code: code ?? -1, out, timedOut });
      });
      p.on('error', err => { clearTimeout(timer); this.children.delete(p); resolve({ code: -1, out: String(err), timedOut }); });
    });
  }

  async refreshQueue() {
    // this checkout's decomp.py (the integration branch may predate `queue`); it only reads
    fs.copyFileSync(path.join(REPO, 'tools/scripts/decomp.py'), path.join(this.integ, '.swarm-queue.py'));
    // (run from the clone's root, so its own imports, such as regions, need tools/scripts on the path)
    const args = ['env', 'PYTHONPATH=tools/scripts', 'python3', '.swarm-queue.py', 'queue', '--count', this.o.only ? '5000' : '300'];
    if (this.o.seg && !this.o.only) args.push('--seg', this.o.seg);
    const r = await this.dk(this.integ, args);
    const line = r.out.trim().split('\n').reverse().find(l => l.startsWith('['));
    if (r.code || !line) throw new Error('decomp.py queue failed:\n' + r.out.slice(-2000));
    let q = JSON.parse(line);
    if (this.o.only) q = q.filter(f => this.o.only.includes(f.name) || this.o.only.includes(f.key));
    // where each function's INCLUDE_ASM line is, to keep neighbours apart (nextJob)
    const lines = {};
    for (const src of new Set(q.map(f => f.src).filter(Boolean))) {
      const p = path.join(this.integ, src);
      if (!fs.existsSync(p)) continue;
      fs.readFileSync(p, 'utf8').split('\n').forEach((l, i) => {
        const m = l.match(/^\s*INCLUDE_ASM\(\s*"[^"]+"\s*,\s*(\w+)\s*\)/);
        if (m) lines[`${src}:${m[1]}`] = i;
      });
    }
    for (const f of q) f.line = lines[`${f.src}:${f.name}`];
    this.queue = q;
  }

  // within NEIGHBOUR lines of a function someone is already on, in the same file: patches that
  // close together overlap in their context lines and conflict
  nearBusy(f, busy) {
    return f.line !== undefined && busy.some(j => j.f.src === f.src && j.f.line !== undefined &&
                                                  Math.abs(j.f.line - f.line) <= NEIGHBOUR);
  }

  activeJobs() {
    return [...[...this.slots, ...this.bslots, ...this.pslots].map(s => s.job).filter(Boolean), ...this.permQueue,
            ...this.integQueue, ...(this.integrating || [])];
  }

  // a big-mode function: at least --big-bytes, or escalated after parking in normal mode
  // (one with a matched twin or template is an adapt job: normal mode, whatever its size)
  isBig(f) {
    return this.bslots.length > 0 && ((f.size >= this.o.bigBytes && !f.twin) || !!this.history[f.key]?.big);
  }

  // identical functions (one fingerprint) are one job: a match fills the rest (decomp.py done
  // propagates it). So a group waits while a member was tried and is still unmatched (one ladder,
  // not one per copy), and under --retry-failed one member per group is leased per run
  heldBack(f) {
    if (!f.fp) return false;
    if (this.o.retryFailed) return this.leasedFps?.has(f.fp) && !this.leasedKeys?.has(f.key);
    return this.queue.some(g => g.fp === f.fp && g.key !== f.key &&
                                (this.history[g.key]?.attempts || this.history[g.key]?.bigSessions));
  }

  // --seed-families (default on): the first member of each identical group that spans overlays
  // goes before the rest, so one match early fills copies across the slot overlays
  seedOrder(cands) {
    if (!this.o.seedFamilies) return cands;
    const ovl = f => (f.key.includes(':') ? f.key.split(':')[0] : 'main');
    const groups = {};
    for (const f of this.queue) if (f.fp) (groups[f.fp] ||= new Set()).add(ovl(f));
    const lead = new Set(), seen = new Set();
    for (const f of cands) if (f.fp && groups[f.fp].size > 1 && !seen.has(f.fp)) { seen.add(f.fp); lead.add(f.key); }
    return [...cands.filter(f => lead.has(f.key)), ...cands.filter(f => !lead.has(f.key))];
  }

  // still worth a big session: under --big-sessions, and not --big-stale sessions in a row without progress.
  // --retry-failed gives a used-up function a fresh allowance for this run (counted from its first lease
  // in it, see retryBase), not an unlimited one: the stale rule still parks it within the run
  bigLeft(f) {
    const h = this.history[f.key] || {};
    const base = this.bigBase?.[f.key];
    if (this.o.retryFailed && base === undefined) return true;
    return (h.bigSessions || 0) - (base || 0) < this.o.bigSessions && (h.bigStale || 0) < this.o.bigStale;
  }

  // the first lease of a function in a --retry-failed run: its sessions count from here, its streak restarts
  retryBase(f) {
    if (!this.o.retryFailed || (this.bigBase ||= {})[f.key] !== undefined) return;
    const h = this.history[f.key] ||= { name: f.name, attempts: 0, notes: [] };
    this.bigBase[f.key] = h.bigSessions || 0;
    h.bigStale = 0;
  }

  // `big`: for a big-mode slot (big functions only), else for a normal slot (small ones only)
  nextJob(peek = false, big = false) {
    const r = this.retry.findIndex(j => !!j.big === big);
    if (r >= 0) return peek ? this.retry[r] : this.retry.splice(r, 1)[0];
    const busy = this.activeJobs();
    const keys = new Set(busy.map(j => j.f.key)), fps = new Set(busy.map(j => j.f.fp).filter(Boolean));
    // big mode: resume the functions with a checkpoint first, then the smallest
    const cands = big ? this.queue.filter(f => this.isBig(f))
      .sort((a, b) => (this.checkpoint(b) ? 1 : 0) - (this.checkpoint(a) ? 1 : 0) || a.size - b.size)
      : this.seedOrder(this.queue.filter(f => !this.isBig(f)));
    for (const f of cands) {
      if (this.finished.has(f.key) || keys.has(f.key) || (f.fp && fps.has(f.fp)) || this.nearBusy(f, busy)) continue;
      if (this.heldBack(f)) continue;
      if (big ? !this.bigLeft(f) : (this.history[f.key]?.attempts || 0) >= this.o.maxAttempts && !this.o.retryFailed) continue;
      if (!peek && big) this.retryBase(f);
      if (!peek && f.fp) { (this.leasedFps ||= new Set()).add(f.fp); (this.leasedKeys ||= new Set()).add(f.key); }
      // a function an earlier run tried picks up where it left off: the escalated model, the next
      // attempt, and that run's last notes (forget's reason too)
      const tried = this.history[f.key]?.attempts || 0, notes = this.earlierNotes(f);
      return big ? this.bigJob(f, 1, notes)
        : { f, model: tried ? this.o.escalateModel || this.o.model : this.o.model, attempt: tried + 1, notes };
    }
    return null;
  }

  // when the build pipeline last changed (the asm filters, compiler settings, the Makefile's lines
  // about them): notes older than that may describe a bug since fixed (the old `$at` notes)
  pipelineChanged() {
    if (this.pipeline === undefined) {
      const t = [git(REPO, 'log', '-1', '--format=%cI', '--', 'tools/scripts/dmpsx.py', 'tools/scripts/aspsx_fill.py',
                     'configs/USA/project.mk'),
                 git(REPO, 'log', '-1', '--format=%cI', '-G', 'CC_VER|CC1|MASPSX|DMPSX|ASPSX|CFLAGS|ASFLAGS', '--', 'Makefile')]
        .map(r => Date.parse(r.out.trim())).filter(n => !isNaN(n));
      this.pipeline = t.length ? Math.max(...t) : 0;
    }
    return this.pipeline;
  }

  // a function's last 3 history notes from earlier runs, minus those older than the pipeline
  earlierNotes(f) {
    const since = this.pipelineChanged();
    return (this.history[f.key]?.notes || [])
      .filter(n => { const at = Date.parse(n.slice(0, 19).replace(' ', 'T') + 'Z'); return isNaN(at) || at >= since; })
      .slice(-3).map(n => `[earlier run] ${n}`);
  }

  bigJob(f, attempt, notes, extra = {}) {
    return { f, model: this.o.bigModel, attempt, notes, big: true, ...extra };
  }

  // the A/B arm of a big function: --regions f1,f2 (or "all") get the `decomp.py regions` prompt
  regionsArm(f) {
    const r = String(this.o.regions || '').split(',').map(s => s.trim()).filter(Boolean);
    return r.includes('all') || r.includes(f.name);
  }

  // ---- big-mode checkpoints: the best C so far for a function, kept across sessions and runs
  // keyed by function key; checkpoints saved before 2026-10-16 are keyed by name, and still resume
  // when the name is unique
  cpDir(f) {
    const byKey = path.join(this.paths.checkpoints, keyDir(f));
    if (fs.existsSync(byKey)) return byKey;
    const byName = path.join(this.paths.checkpoints, f.name);
    return ref(f) === f.name && fs.existsSync(byName) ? byName : byKey;
  }

  checkpoint(f) {
    const d = this.cpDir(f);
    const j = readJson(path.join(d, 'best.json'), null);
    return j && fs.existsSync(path.join(d, 'best.diff')) ? j : null;
  }

  // the `STAGE name stage n/total` line of decomp.py try (MATCH counts as the best stage)
  static score(tryOut) {
    if (/^MATCH /m.test(tryOut)) return { stage: 'match', diffs: 0, total: 0 };
    const m = tryOut.match(/^STAGE \S+ (structure|instructions|registers) (\d+)\/(\d+)/m);
    return m ? { stage: m[1], diffs: +m[2], total: +m[3] } : null;
  }

  // the `REGIONS name blocks m/n insns e/N (..%) paired p/N` line of decomp.py regions
  static regionScore(out) {
    const m = out.match(/^REGIONS \S+ blocks (\d+)\/(\d+) insns (\d+)\/(\d+) \S+ paired (\d+)\/(\d+)/m);
    return m ? { blocks: +m[1], nblocks: +m[2], exact: +m[3], len: +m[4], paired: +m[5] } : null;
  }

  static describe(s) {
    if (!s) return '-';
    const r = s.regions;
    return `${s.stage} ${s.diffs}/${s.total}${r ? ` (blocks ${r.blocks}/${r.nblocks}, paired ${r.paired}/${r.len}, exact ${r.exact}/${r.len})` : ''}`;
  }

  // a match beats everything; then, when both have a region score, more of the target paired by
  // opcode (structure), then more blocks identical, then more exact instructions; otherwise (or on
  // a tie) the STAGE line: a later stage, then fewer diffs
  static better(a, b) {
    if (!b) return true;
    if (a.stage === 'match' || b.stage === 'match') return a.stage === 'match' && b.stage !== 'match';
    const ra = a.regions, rb = b.regions;
    if (ra && rb) {
      const key = r => [r.paired / r.len, r.blocks, r.exact / r.len];
      const ka = key(ra), kb = key(rb);
      for (let i = 0; i < ka.length; i++) if (ka[i] !== kb[i]) return ka[i] > kb[i];
    }
    const rank = s => ['structure', 'instructions', 'registers', 'match'].indexOf(s.stage);
    return rank(a) > rank(b) || (rank(a) === rank(b) && a.diffs < b.diffs);
  }

  // build and score the function in a clone: the STAGE line, plus the region score unless it matches
  async measure(dir, f, slot, outDir = null) {
    const name = ref(f);
    const r = await this.dk(dir, ['python3', 'tools/scripts/decomp.py', 'try', name], slot, { minutes: TOOL_MINUTES });
    if (outDir) fs.writeFileSync(path.join(outDir, 'try.txt'), r.out);
    const score = Swarm.score(r.out);
    let infra = infraError(r.out, r.code);
    if (score && score.stage !== 'match') {
      const g = await this.dk(dir, ['python3', 'tools/scripts/decomp.py', 'regions', name, '--no-lines'], slot, { minutes: TOOL_MINUTES });
      if (outDir) fs.writeFileSync(path.join(outDir, 'regions.txt'), g.out);
      score.regions = Swarm.regionScore(g.out);
      infra ||= infraError(g.out, g.code);
    }
    return { score, out: r.out, infra };
  }

  dockerDown() {
    return sh('docker', ['info'], { timeout: 60e3 }).code !== 0;
  }

  // the environment broke under a job (Docker gone, a file locked): not the function's fault, so
  // nothing is charged, and the run stops rather than parking everything after it
  infraStop(slot, job, why) {
    if (slot) slot.job = null;
    this.finished.add(job.f.key);
    this.record(job, 'infra', why);
    this.log(`${job.f.name}: environment error, not a problem with the function: ${why}. Stopping the run ` +
             '(nothing charged; fix it and run again)');
    this.stopping ||= 'graceful';
  }

  saveCheckpoint(job, patchFile, score, note, next) {
    const d = this.cpDir(job.f);
    fs.mkdirSync(d, { recursive: true });
    const old = this.checkpoint(job.f);
    const log = [...(old?.log || []), `${stamp()} ${job.model}: ${Swarm.describe(score)}. ${note}${next ? ` Next: ${next}` : ''}`].slice(-12);
    fs.copyFileSync(patchFile, path.join(d, 'best.diff'));
    writeJson(path.join(d, 'best.json'), { ...score, name: job.f.name, key: job.f.key, seg: job.f.seg, base: job.base, updated: stamp(), log });
  }

  // a normal attempt's C that didn't match becomes the function's checkpoint if it is the closest yet
  keepCheckpoint(job, file, score, note) {
    // (after a checkpoint that no longer applied, any C that builds replaces it: the old one is dead)
    if (!score || score.stage === 'match' || (!job.oldCheckpoint && !Swarm.better(score, this.checkpoint(job.f)))) return false;
    this.saveCheckpoint(job, file, score, note);
    return true;
  }

  noteCheckpoint(job, line) {
    const old = this.checkpoint(job.f);
    if (!old) return;
    old.log = [...(old.log || []), `${stamp()} ${job.model}: ${line}`].slice(-12);
    writeJson(path.join(this.cpDir(job.f), 'best.json'), old);
  }

  // ---- session chaining (--chain N, off by default): after a normal session on a
  // small function, the next function of the same file runs in the same Claude session (--resume),
  // which already knows the file's types and globals. At most N in a row; a pilot (alternate
  // chained and single jobs, compare with `stats`), so it stays off until the numbers say so
  chainNext(slot, job) {
    const depth = (job.chainDepth || 0) + 1;
    if (!this.o.chain || depth > this.o.chain || slot.job || this.stopping || job.big || job.gateFix || job.permuterDiff
        || !job.resultSession || job.model !== this.o.model || this.leases >= this.o.maxFunctions) return;
    const busy = this.activeJobs();
    const keys = new Set(busy.map(j => j.f.key)), fps = new Set(busy.map(j => j.f.fp).filter(Boolean));
    const f = this.queue.find(g => g.src === job.f.src && g.key !== job.f.key && !this.isBig(g) && !this.finished.has(g.key)
      && !keys.has(g.key) && !(g.fp && fps.has(g.fp)) && !this.nearBusy(g, busy) && !this.heldBack(g)
      && !(this.history[g.key]?.attempts));
    if (!f) return;
    const next = { f, model: job.model, attempt: 1, notes: this.earlierNotes(f), conflicts: 0,
                   chained: true, chainDepth: depth, sessionId: job.resultSession };
    this.log(`${slot.id} ${f.name}: chained into ${job.f.name}'s session (${depth}/${this.o.chain})`);
    try {
      this.startJob(slot, next);
    } catch (e) {
      slot.job = null;
      this.fail(null, next, `could not start: ${e.message}`, false);
    }
  }

  // ---- Diff patterns workers propose (patterns.md, folded into MATCHING.md after a run)
  addPattern(f, row) {
    row = String(row).trim().replace(/\s*\n\s*/g, ' ');
    if (!row || PATTERN_REFUSED.test(row)) return;
    fs.appendFileSync(this.paths.patterns, `${row}  <!-- ${f.name} ${f.seg} -->\n`);
  }

  // the rows to show a worker on f: no asm tricks (rows from before addPattern filtered), no
  // duplicates, the same overlay's first, newest first, about PATTERNS_MAX characters
  patternsFor(f) {
    if (!fs.existsSync(this.paths.patterns)) return '';
    const unit = s => (s || '').split('/')[s?.startsWith('overlays/') ? 1 : 0];
    const rows = new Map();          // row -> found on the same overlay (by any worker proposing it)
    for (const line of fs.readFileSync(this.paths.patterns, 'utf8').trim().split('\n').reverse()) {
      const m = line.match(/^(.*?)\s*<!--\s*(\S+)(?:\s+(\S+))?\s*-->\s*$/);
      const row = (m ? m[1] : line).trim();
      if (!row || PATTERN_REFUSED.test(row)) continue;
      rows.set(row, rows.get(row) || (!!m?.[3] && unit(m[3]) === unit(f.seg)));
    }
    const all = [...rows];
    let out = '';
    for (const [row] of [...all.filter(r => r[1]), ...all.filter(r => !r[1])]) {
      if (out.length + row.length > PATTERNS_MAX) break;
      out += row + '\n';
    }
    return out.trim();
  }

  // ---- workers
  prompt(job) {
    if (job.big) return this.promptBig(job);
    const { f } = job, r = ref(f);
    const canPermute = this.pslots.length > 0 && !job.permuted;
    const patterns = this.patternsFor(f);
    return `${job.chained ? `Next function, in the same session (your previous one is done with: the orchestrator took its work
and reset the clone to a fresh base, so declarations you added then are gone unless they were
committed; add again what this one needs).

` : ''}You are one worker in a swarm matching Crash Bash functions. Match exactly one function:
${f.name} (${f.seg}, ${f.size} bytes, defined in ${f.src}).

${LOOP(f, r)}
These swarm rules override CLAUDE.md and the docs:
- Work only on ${f.name}. Change only its INCLUDE_ASM line in ${f.src}, and headers under include/
  when it needs a declaration or a type. Never touch another function's INCLUDE_ASM line.
- If ${f.name}'s own declarations have the wrong type (its prototype in a header, an extern for
  it elsewhere in src/), fix them in place: the gate rebuilds the whole exe and refuses a change
  that breaks another function's match. Don't hide a type change behind a macro or wrapper named
  like the function (refused).
- New structs, typedefs and global declarations go in the owned header of ${f.src} (the
  include/bash/*.h marked \`owner: ${f.src}\`), never in the .c file: the gate adds the function's
  prototype to that header, and it won't compile if a type in it is only declared in the .c.
- Do not run \`decomp.py done\`, \`decomp.py block\`, \`make clean\`, \`make verify\` or any git command
  that changes the repo, and do not edit docs/, .decomp/, configs/ or tools/. The orchestrator
  validates and commits your work.
- Pure C only: no \`asm\`/\`__asm__\` of any kind and no register pins (the swarm refuses the
  patch). Use the \`gte_*\` macros already in include/. If only asm would match, finish "parked" and
  say why.
- Don't run the permuter (it is refused). ${canPermute
    ? `If the evidence step, the Diff patterns, the checklist and (for register diffs) the cc1 dump
  don't get you to a match, leave your closest C in place of the INCLUDE_ASM line and finish with
  status "needs_permute": the orchestrator runs the permuter in the background and gives its result
  to a worker, so you free your slot instead of waiting on it.`
    : 'This function has had its permuter run already.'}
- Tools: change files only with the Edit and Write tools (shell edits such as \`sed -i\`, \`>\` or
  \`tee\` are refused). Run each \`tools/dk ... </dev/null\` as its own Bash command, not chained
  with \`&&\` or \`;\` (a chain is refused if any part isn't allowed). Read-only shell commands
  (grep, head, tail, cat, ls, wc) are fine; git runs as \`tools/dk git ...\` (log, show, diff,
  status). If a command is refused, do the same thing another way; a refusal is never a reason
  to stop.
- You have about ${this.o.workerMinutes} minutes.
- Stop when \`tools/dk make diff FUNC=${r}\` prints MATCH, or when you are out of ideas.
  If it does not match, leave your closest C in place of the INCLUDE_ASM line (not under
  \`#ifndef NON_MATCHING\`) if it compiles, and finish "parked": the orchestrator measures it and
  keeps the closest version, and the next attempt starts from it. Never put INCLUDE_ASM back.
- Last, write ${RESULT} in the repo root, JSON:
  {"status": "matched" | "parked"${canPermute ? ' | "needs_permute"' : ''},
   "note": "one line: what made it match, or what still differs",
   "pattern": "a new Diff patterns table row (| symptom | change | example |), or empty"}
${job.resumed && !job.gateFix && !job.permuterDiff ? `
This attempt starts from the closest C so far, already in ${f.src} in place of the INCLUDE_ASM line
(${Swarm.describe(job.resumed)}). Its log, oldest first:
${(job.resumed.log || []).map(l => '- ' + l).join('\n')}
` : ''}${job.oldCheckpoint ? `
An earlier attempt's C (${Swarm.describe(job.oldCheckpoint.cp)}) no longer applies to this branch
(${job.oldCheckpoint.why}); ${f.src} still has the INCLUDE_ASM line. Run \`tools/dk python3 tools/scripts/decomp.py port ${r} ${OLD_DIFF}\` first: it puts that C in place
mechanically (today's names for the placeholder ones, its helpers and declarations) and lists what it
couldn't port (edits to existing declarations); fix those and get it building.
(\`tools/dk git log -3 -- ${f.src}\` shows what changed.) Then go on. Its diff:
\`\`\`
${job.oldCheckpoint.diff}
\`\`\`
` : ''}${job.gateFix ? `
An earlier worker's C for ${f.name} is already in ${f.src} and matches the bytes, but the validation
gate (decomp.py done: scope, lint, full build) refused it with:
\`\`\`
${job.gateFix}
\`\`\`
${DECL_CLASH.test(job.gateFix) ? DECL_HINT : ''}Fix that (often a type declared in the .c instead of the owned header, or a lint finding), keep the
function matching (make diff), and report "matched". A build error in another file is real, not a
fluke: usually a caller there that no longer fits the prototype you added (\`grep -rn ${f.name} src\`);
build that object (\`tools/dk make build/src/...c.o\`) to see it, and fix the call.
` : ''}${job.permuterDiff ? `
An earlier worker's closest C for ${f.name} is already in ${f.src}. The permuter then got its score
from ${job.permuterScore.base} to ${job.permuterScore.best}${job.permuterScore.best === 0 ? ' (a MATCH)' : ''} (0 is a match) with these changes to that C. Port
the useful ones by hand (usually a temp, a statement order, a type or a cast; drop permuter noise
such as odd casts that don't change the code) and check with make diff:
\`\`\`
${job.permuterDiff}
\`\`\`
` : ''}${f.note ? `\nNote recorded for this function earlier: ${f.note}\n` : ''}${job.notes.length ? `\nEarlier swarm attempts at it:\n${job.notes.map(n => '- ' + n).join('\n')}\n` : ''}${patterns ? `\nDiff patterns other workers found in this run (not yet in MATCHING.md):\n${patterns}\n` : ''}`;
  }

  // big mode: one session of several on a big function, resuming from the best C so far
  promptBig(job) {
    const { f } = job, r = ref(f);
    const cp = job.resumed;
    const canPermute = this.pslots.length > 0;
    const patterns = this.patternsFor(f);
    return `You are a big-function worker in a swarm matching Crash Bash functions. Work on exactly one
function: ${f.name} (${f.seg}, ${f.size} bytes, defined in ${f.src}). A big function takes several
sessions; this is one of them. Make as much progress as you can and leave your best C in place: the
orchestrator measures it with \`decomp.py try\` and keeps the best version for the next session.
${cp ? `
This session resumes from the best C so far, already in ${f.src} in place of the INCLUDE_ASM line
(${Swarm.describe(cp)}). Log of earlier sessions, oldest first:
${(cp.log || []).map(l => '- ' + l).join('\n')}
` : job.oldCheckpoint ? `
Earlier sessions got it to ${Swarm.describe(job.oldCheckpoint.cp)}, but that C no longer applies to
this branch (${job.oldCheckpoint.why}): something it relied on was renamed, moved or redeclared
since. ${f.src} still has the INCLUDE_ASM line. Run \`tools/dk python3 tools/scripts/decomp.py port ${r} ${OLD_DIFF}\` first: it puts that C in place
mechanically (today's names for the placeholder ones, its helpers and declarations) and lists what it
couldn't port (edits to existing declarations); fix those and get it building.
(\`tools/dk git log -3 -- ${f.src}\` and the headers it touches show what changed.) Then go on as usual.
Its diff, for reference:
\`\`\`
${job.oldCheckpoint.diff}
\`\`\`
Log of the sessions that made it, oldest first:
${(job.oldCheckpoint.cp?.log || []).map(l => '- ' + l).join('\n')}
` : `
No session has worked on it yet: start from the m2c draft (\`tools/dk python3 tools/scripts/decomp.py
next --func ${r}\` shows it, with the function's evidence).
`}
${LOOP(f, r)}${this.regionsArm(f) ? `Work region by region with \`tools/dk python3 tools/scripts/decomp.py regions ${r} </dev/null\`.
It aligns your compiled code with the target (so an extra instruction doesn't shift every later
diff, as \`make diff\`'s STAGE count does), lists the target's basic blocks that differ with their status (ok /
operands / differ) and C lines, and shows the first differing block side by side (\`--block N\` for
another, \`--no-lines\` to skip the C-line build and save a few seconds; after an edit,
\`--changed\` lists only the blocks whose status changed since your last call). Its last line,
\`REGIONS ${f.name} blocks m/n insns e/N paired p/N ...\`, is your progress: paired (opcodes
aligned) is the structure, insns (exact) the rest. The loop:
1. Fix the first differing block: its structure (missing or extra instructions, "differ") before
   its operands. Read that block's C lines and the target asm for it; don't rewrite the rest.
2. Re-run regions. Keep a change if blocks or paired went up; undo it if it broke blocks that were
   ok. A register change can cascade into later blocks, so judge by paired and the block list
   together, not insns alone.
3. Repeat. When regions shows only "operands" blocks, run \`tools/dk python3 tools/scripts/decomp.py
   vars ${r} </dev/null\`: it names the C variable behind each block's register swap. Then the
   Diff patterns' register rows and the cc1 dump. If one variable sits in different registers in
   different blocks, run
   \`tools/dk python3 tools/scripts/decomp.py splitvars ${r} VAR </dev/null\`: it tries every
   grouping of VAR's assignments into separate variables and ranks them (\`--apply\` writes the
   best; give the new VAR_N real names).
The orchestrator keeps the best C by the same region score (paired first, then identical blocks,
then exact instructions), so the version you leave should be your best by it.` :
`Work in stages; \`tools/dk make diff FUNC=${r} </dev/null\` prints a \`STAGE\` line that says
which one you are in, and progress is measured by it (a later stage beats any count of diffs in an
earlier one, then fewer diffs):
1. structure: the same length and branch layout (control flow, switch/jump tables, loops, early
   returns). Get this first; don't polish registers before it.
2. instructions: the same opcodes in order (statement order, temps, types, casts).
3. registers: operands only. The cc1 dump and the Diff patterns' register rows.`}
${cp ? `Earlier sessions did the first-session checks (data typed, helpers and siblings looked for); go
on from the log above.
` : `Before matching code:
- Type the data: put structs for what it touches (fields at their offsets) in the owned header, using
  include/bash/*.h, the prototype debug types and the leaked headers (docs/ARCHITECTURE.md). Typed
  fields remove much of m2c's noise.
- Look for helpers and siblings: repeated instruction runs inside it, or code like an already matched
  function's (\`tools/dk python3 tools/scripts/decomp.py similar ${r}\` ranks matched
  functions by shared code, with where their C is; \`decomp.py dups\`; callers and callees in
  .decomp/functions.json). Original code often used \`static inline\` helpers and macros here.
`}Rules (they override CLAUDE.md and the docs):
- Work only on ${f.name}. Change only its definition (in place of its INCLUDE_ASM line) in ${f.src},
  and headers under include/. New types and declarations go in the owned header of ${f.src} (the
  include/bash/*.h marked \`owner: ${f.src}\`), never in the .c file.
- If ${f.name}'s own declarations have the wrong type (its prototype in a header, an extern for
  it elsewhere in src/), fix them in place: the gate rebuilds the whole exe and refuses a change
  that breaks another function's match. Don't hide a type change behind a macro or wrapper named
  like the function (refused).
- When something blocks one difference, note it and go on to the next differing block until the
  deadline: one blocker is not a reason to finish early.
- Do not run \`decomp.py done\`, \`decomp.py block\`, \`make clean\`, \`make verify\` or any git command
  that changes the repo, and do not edit docs/, .decomp/, configs/ or tools/.
- Pure C only: no \`asm\`/\`__asm__\` of any kind and no register pins (the swarm refuses the
  patch). Use the \`gte_*\` macros already in include/. If only asm would match, finish "parked" and
  say why.
- Don't run the permuter (it is refused).${canPermute ? ` When you are at the registers stage and
  stuck, finish with status "needs_permute": the orchestrator runs a long permuter on your C and the
  next session gets its result.` : ''}
- Tools: change files only with the Edit and Write tools. Run each \`tools/dk ... </dev/null\` as its
  own Bash command, not chained. Read-only shell commands (grep, head, tail, cat, ls, wc) are fine;
  git runs as \`tools/dk git ...\`. A refused command is never a reason to stop: do the same thing
  another way.
- This session ends at ${new Date(Date.now() + this.o.bigMinutes * 60e3).toTimeString().slice(0, 5)} local time
  (\`date +%H:%M\` shows the time now): use all of it. Being killed at the deadline is the normal end
  of a session, not a failure: the C left in the file is measured as usual. So never stop to wrap
  up, and never because little time is left: one block edit and a regions check take about a
  minute, so with any time left, work on the next differing block. Finish early only on a match,
  or as "parked" when you are sure it can't match from this file. Keep the file compiling and at
  your best version after each step (undo an experiment that made it worse before the next), and
  write ${RESULT} early and update it as you go, so a kill loses nothing. Never put the
  INCLUDE_ASM line back.
- Keep ${RESULT} in the repo root up to date (write it early, rewrite it after each improvement), JSON:
  {"status": "matched" | "progress" | ${canPermute ? '"needs_permute" | ' : ''}"parked",
   "note": "one line: what you changed this session and what still differs",
   "next": "one line: what the next session should try first",
   "pattern": "a new Diff patterns table row (| symptom | change | example |), or empty"}
  "parked" means you believe it can't be matched from this file (wrong unit, needs a split, or every
  idea is exhausted); say why in "note".
${job.permuterDiff ? `
The permuter ran on the best C (score ${job.permuterScore.base} -> ${job.permuterScore.best}, 0 is a match).
Port the useful changes by hand (a temp, a statement order, a type or a cast; drop noise):
\`\`\`
${job.permuterDiff}
\`\`\`
` : ''}${job.gateFix ? `
Your C matches the bytes, but the validation gate (decomp.py done) refused it with:
\`\`\`
${job.gateFix}
\`\`\`
${DECL_CLASH.test(job.gateFix) ? DECL_HINT : ''}Fix that, keep it matching, and report "matched".
` : ''}${f.note ? `\nNote recorded for this function earlier: ${f.note}\n` : ''}${job.notes.length ? `\nEarlier swarm attempts at it:\n${job.notes.map(n => '- ' + n).join('\n')}\n` : ''}${patterns ? `\nDiff patterns other workers found in this run (not yet in MATCHING.md):\n${patterns}\n` : ''}`;
  }

  // --start-gap-seconds between worker starts: sessions that start together all refresh the
  // OAuth token at once and fail (2026-10-19: 6 of 7 at the start of an ovl62 run). Not for
  // --worker-cmd, which starts no Claude.
  startGapWait() {
    return !this.o.workerCmd && Date.now() - (this.lastStart || 0) < (this.o.startGapSeconds || 0) * 1000;
  }

  startJob(slot, job) {
    const { f } = job;
    this.lastStart = Date.now();
    // the last pushed tip, not the integration clone's HEAD: while a batch is in the gate HEAD has
    // its unpushed commits, which a failed batch throws away (and a permuter could then not find)
    job.base = this.tip || gitOk(this.integ, 'rev-parse', 'HEAD');
    const kind = job.big ? `big${job.attempt}` : `attempt${job.attempt}`;
    job.dir = path.join(this.paths.results, keyDir(f), `${kind}${job.gateFix ? '-gatefix' : job.startPatch ? '-port' : ''}-${this.id}`);
    fs.mkdirSync(job.dir, { recursive: true });
    makeClone(slot.dir, this.integ, 'HEAD', m => this.log(m));   // HEAD may have moved on: base is in it
    resetClone(slot.dir, job.base);
    // every job (big or normal, any run) resumes from the function's closest C so far; a normal job
    // that already starts from other C (a permuter result, a conflict) keeps that
    if (!job.gateFix && (job.big || !job.startPatch)) {
      const cp = this.checkpoint(f);
      if (cp) {
        job.resumed ||= cp;
        job.startPatch ||= path.join(this.cpDir(f), 'best.diff');
      }
    }
    // a follow-up after the permuter (or a big session) starts from the earlier worker's C
    if (job.startPatch) {
      const ap = git(slot.dir, 'apply', '--3way', '--whitespace=nowarn', job.startPatch);
      if (ap.code) {
        // main moved on under it: put the function's C in place mechanically
        resetClone(slot.dir, job.base);
        const why = reapply(slot.dir, fs.readFileSync(job.startPatch, 'utf8'), f);
        if (why !== null) {
          // (a rename, a file split or a header change since): the session ports it by hand from
          // the old diff, and its first C that builds replaces the checkpoint (onBigExit)
          this.log(`${slot.id} ${f.name}: the checkpoint doesn't apply to ${this.o.branch} any more (${why}); ` +
                   'starting from INCLUDE_ASM with the old diff as reference');
          fs.appendFileSync(this.paths.report, `- ${stamp()} **${f.name}** (${f.seg}, ${f.size} B): checkpoint ` +
                            `(${Swarm.describe(job.resumed)}) no longer applies: ${why}. A session ports it by hand.\n`);
          (this.staleCheckpoints ||= []).push(f.name);
          let diff = fs.readFileSync(job.startPatch, 'utf8');
          if (diff.length > 12000) diff = diff.slice(0, 12000) + '\n... (cut)';
          job.oldCheckpoint = { why, diff, cp: job.resumed || { stage: '?', diffs: 0, total: 0 } };
          job.permuterDiff = job.gateFix = null;   // their C isn't in place: port the diff instead
          resetClone(slot.dir, job.base);
          // for `decomp.py port` (the prompt says to run it first); not under src/, so not in the patch
          fs.copyFileSync(job.startPatch, path.join(slot.dir, OLD_DIFF));
          job.startPatch = job.resumed = null;
        }
      }
      gitOk(slot.dir, 'reset', '-q');   // --3way stages it: unstaged, `git checkout -- src` undoes it
    }
    fs.rmSync(path.join(slot.dir, RESULT), { force: true });
    fs.rmSync(path.join(slot.dir, 'build', 'attempts'), { recursive: true, force: true });   // try's stall log: per job
    useOurDk(slot.dir);
    const prompt = this.prompt(job);
    fs.writeFileSync(path.join(job.dir, 'prompt.txt'), prompt);
    slot.job = job;
    if (!job.retry) this.leases++;
    job.started = Date.now();

    const env = Object.fromEntries(Object.entries(process.env).filter(([k]) => !STRIP_ENV.test(k)));
    Object.assign(env, {
      // SWARM_RESULT in the container too: decomp.py words its hints for a worker
      DK_DOCKER_ARGS: `--label ${LABEL}=${this.id} --label ${LABEL}-slot=${this.id}-${slot.id} --cpus ${this.cpus} -e SWARM_RESULT=${RESULT}${job.big ? ' -e SWARM_BIG=1' : ''} ${workerMounts(slot.dir)}`,
      DK_NAME: `swarm-${slot.id}`,
      SWARM_FUNC: f.name, SWARM_KEY: f.key, SWARM_REF: ref(f), SWARM_SRC: f.src || '', SWARM_MODEL: job.model, SWARM_RESULT: RESULT,
    }, this.o.workerEnv || {});   // config.json: no git status in the system prompt (it changes every
                                  // job and made each session rewrite ~11k tokens of cache), no memory
    let cmd, args, shell = false;
    if (this.o.workerCmd) {
      [cmd, args] = [this.bash, ['-c', this.o.workerCmd]];
    } else if (this.o.dryRun) {
      [cmd, args] = [this.bash, ['-c', `echo '{"status":"parked","note":"dry run"}' > ${RESULT}`]];
    } else {
      cmd = this.claude;
      // a known session id finds the transcript when the session is killed; a chained job resumes
      // the previous function's session instead
      job.sessionId ||= crypto.randomUUID();
      args = [...this.o.claudeArgs, '--model', job.model, ...(job.chained ? ['--resume', job.sessionId] : ['--session-id', job.sessionId]),
              '--max-budget-usd', String(job.big ? this.o.bigBudgetUsd : this.o.workerBudgetUsd),
              '--allowedTools', this.o.allowedTools.join(','), '--disallowedTools', this.o.disallowedTools.join(',')];
      if (WIN && /\.(cmd|bat)$/i.test(cmd)) {
        shell = true;
        cmd = `"${cmd}"`;
        args = args.map(a => `"${a}"`);
      }
    }
    const out = fs.openSync(path.join(job.dir, 'worker.out'), 'w');
    const p = spawn(cmd, args, { cwd: slot.dir, env, shell, stdio: ['pipe', out, out], detached: !WIN, windowsHide: true });
    fs.closeSync(out);
    p.stdin.on('error', () => {});
    p.stdin.end(prompt);
    this.children.add(p);
    job.pid = p.pid;
    const minutes = job.big ? this.o.bigMinutes : this.o.workerMinutes;
    job.timer = setTimeout(() => {
      this.log(`${slot.id} ${f.name}: over ${minutes} min, killing it${job.big ? ' (its C is still measured and kept if better)' : ''}`);
      job.timedOut = true;
      killTree(p.pid);
      dockerKill(`${LABEL}-slot=${this.id}-${slot.id}`, slot.dir);
    }, minutes * 60e3);
    p.on('close', code => {
      this.children.delete(p);
      clearTimeout(job.timer);
      this.onWorkerExit(slot, job, code).catch(e => this.fail(slot, job, `orchestrator error: ${e.message}`))
        .then(() => this.chainNext(slot, job));
    });
    p.on('error', err => {
      job.startError = err.message;   // 'close' follows: onWorkerExit records it uncharged
      this.log(`${slot.id} ${f.name}: could not start worker: ${err.message}`);
    });
    this.log(`${slot.id} start ${f.name} (${f.size} bytes, ${job.model}, ${job.big
      ? `big session ${(this.history[f.key]?.bigSessions || 0) + 1}${job.resumed ? `, from ${Swarm.describe(job.resumed)}` : ''}`
      : `attempt ${job.attempt}`})`);
  }

  // which INCLUDE_ASM lines a patch removes and adds, and the paths it touches
  checkPatch(patch, f) {
    const errs = [];
    if (patch.length > PATCH_MAX) errs.push(`patch is ${patch.length} bytes`);
    const files = [...patch.matchAll(/^diff --git a\/(\S+) b\/(\S+)/gm)].map(m => m[2]);
    for (const p of files) if (!/^(src|include)\//.test(p)) errs.push(`touches ${p}`);
    // [ \t]*, not \s*: \s spans lines, so an added blank line before a context INCLUDE_ASM read as an add
    const inc = /^([-+])[ \t]*INCLUDE_ASM\(\s*"[^"]+"\s*,\s*(\w+)\s*\)/gm;
    const removed = [], added = [];
    for (const m of patch.matchAll(inc)) (m[1] === '-' ? removed : added).push(m[2]);
    if (added.length) errs.push(`adds INCLUDE_ASM for ${added.join(', ')}`);
    if (removed.length !== 1 || ![f.name, `func_${f.key}`].includes(removed[0]))
      errs.push(`replaces INCLUDE_ASM for [${removed.join(', ')}], expected only ${f.name}`);
    // every spelling (decomp.py's ASM_CALL): asm, __asm, __asm__, optionally volatile; pins too
    if (/^\+.*\b(?:asm|__asm|__asm__)\s*(?:(?:volatile|__volatile|__volatile__)\s*)?\(/m.test(patch))
      errs.push(`${ASM_REFUSED} (needs a human --hack)`);
    // a function-like macro named after the function hides a change to its prototype from callers
    if (new RegExp(`^\\+[ \\t]*#[ \\t]*define[ \\t]+${f.name}[ \\t]*\\(`, 'm').test(patch))
      errs.push(`adds a macro named ${f.name} (fix its declarations instead)`);
    return errs;
  }

  async onWorkerExit(slot, job, code) {
    const { f } = job;
    const res = readJson(path.join(slot.dir, RESULT), {});
    fs.writeFileSync(path.join(job.dir, 'result.json'), JSON.stringify(res, null, 1));
    git(slot.dir, 'add', '-A', '--', 'src', 'include');
    const patch = git(slot.dir, 'diff', '--cached', '--binary', job.base, '--', 'src', 'include').out;
    const mins = ((Date.now() - job.started) / 60e3).toFixed(1);
    // claude -p --output-format json: the last JSON line has the cost and any session error
    const out = fs.readFileSync(path.join(job.dir, 'worker.out'), 'utf8').trim().split('\n');
    let session = {};
    try { session = JSON.parse(out.reverse().find(l => l.startsWith('{"')) || '{}'); } catch { /* not claude */ }
    // what it cost, killed sessions included (their transcript, priced), for --max-run-usd and the ledger
    const use = job.sessionId ? sessionUsage(session, job.chained ? null : transcriptOf(job.sessionId), this.o.prices, this.priceScale()) : null;
    job.resultSession = session.session_id || null;
    if (use?.priced) { this.calib.reported += use.usd; this.calib.priced += use.priced; }
    const cost = use?.usd || 0;
    this.costUsd += cost;
    // `try` runs this session made (decomp.py counts them in the clone's state.json)
    job.tries = readJson(path.join(slot.dir, '.decomp', 'state.json'), {})[f.key]?.tries ?? null;
    job.usage = use && { ...use, minutes: +mins };
    this.log(`${slot.id} ${f.name}: worker exited ${code}${job.timedOut ? ' (timeout)' : ''} after ${mins} min` +
             `${cost ? `, $${cost.toFixed(2)}${use.estimated ? ' (estimated from its transcript)' : ''} (run $${this.costUsd.toFixed(2)})` : ''}` +
             `${use && !use.known ? ', cost unknown (no result line, no transcript or price)' : ''}, says ${res.status || 'nothing'}`);
    if (res.pattern) this.addPattern(f, res.pattern);
    if (this.stopping === 'now') return this.ledger(job, 'stopped');
    if (job.startError) return this.fail(slot, job, `could not start the worker: ${job.startError}`, false);
    // a worker that couldn't build leaves no patch: if Docker went away, that's why
    if (!patch.trim() && this.dockerDown()) return this.infraStop(slot, job, 'Docker is not reachable');
    // a session that never got going (not logged in, API down) is a failure, not a park; not the
    // function's either, so uncharged (three in a row still stop the run)
    // a session stopped by --max-turns or its budget ends with an error result: that's a park
    // A big session's clone starts with the checkpoint's C, so it always leaves a patch: one that
    // cost nothing did no work either (2026-10-16: an OAuth refresh failure parked func_8002D4F4).
    const capped = /^error_max/.test(session.subtype || '');
    const noWork = !patch.trim() || (session.total_cost_usd === 0 && !(session.num_turns > 1));
    if (session.is_error && !capped && noWork && !res.status)
      return this.fail(slot, job, `claude: ${String(session.result || session.terminal_reason).slice(0, 200)}`, false);
    if (job.big && !job.gateFix) return this.onBigExit(slot, job, res, patch);
    if (capped && !patch.trim()) return this.notMatched(slot, job, `session hit its cap (${session.subtype}); ${res.note || ''}`.trim());
    if (!patch.trim()) return this.notMatched(slot, job, res.note || (job.timedOut ? 'timed out' : 'no change'));
    fs.writeFileSync(path.join(job.dir, 'patch.diff'), patch);
    const errs = this.checkPatch(patch, f);
    if (errs.length && errs.every(e => e.startsWith(ASM_REFUSED))) {
      // measured anyway: a match that needs the asm goes to a human (`done --hack`), not lost
      const r = await this.dk(slot.dir, ['python3', 'tools/scripts/decomp.py', 'try', ref(f)], slot.id, { minutes: TOOL_MINUTES });
      fs.writeFileSync(path.join(job.dir, 'try.txt'), r.out);
      const hack = /^MATCH /m.test(r.out);
      if (hack) this.needsHack(job);
      return this.notMatched(slot, job, `patch refused: inline asm${hack ? ' (it matches with it: needs a human --hack)' : ''}; ${res.note || ''}`);
    }
    // a park's leftover that the checks refuse is simply not kept; only a claimed match is a failure
    if (errs.length && res.status !== 'matched')
      return this.notMatched(slot, job, `${res.note || 'parked'} (its C not kept: ${errs.join('; ')})`);
    if (errs.length) return this.fail(slot, job, `patch refused: ${errs.join('; ')}`);
    if (res.status === 'needs_permute') {
      if (!this.pslots.length || job.permuted)
        return this.notMatched(slot, job, `asked for the permuter again; ${res.note || ''}`.trim());
      slot.job = null;              // the slot is free now: the permuter runs without Claude
      job.note = res.note || '';
      job.bestPatch = path.join(job.dir, 'patch.diff');
      this.permQueue.push(job);
      this.log(`${slot.id} ${f.name}: handed to the permuter pool (${job.note})`);
      return;
    }
    // don't trust the worker's word: build and compare the function ourselves (a park too: its C
    // may match after all, and otherwise it is kept if it is the closest yet)
    const { score, infra } = await this.measure(slot.dir, f, slot.id, job.dir);
    if (infra) return this.infraStop(slot, job, infra);
    if (score?.stage !== 'match') {
      const kept = this.keepCheckpoint(job, path.join(job.dir, 'patch.diff'), score, res.note || '');
      return this.notMatched(slot, job, (res.status === 'matched'
        ? `claimed a match that try rejects; ${res.note || ''}`.trim()
        : `${res.note || 'not matched'}`) + ` (${score ? Swarm.describe(score) : 'C did not build'}${kept ? ', kept as the closest' : ''})`);
    }
    job.note = res.note || '';
    job.patch = path.join(job.dir, 'patch.diff');
    slot.job = null;
    this.integQueue.push(job);
    this.log(`${slot.id} ${f.name}: try says MATCH, queued for integration`);
  }

  // a big session ended (any status, or killed at --big-minutes): measure its C ourselves, keep it
  // if it beats the checkpoint, then integrate, permute, run another session or park
  async onBigExit(slot, job, res, patch) {
    const { f } = job;
    let score = null, errs = [], infra = null, asmScore = null;
    const file = path.join(job.dir, 'patch.diff');
    if (patch.trim()) {
      fs.writeFileSync(file, patch);
      errs = this.checkPatch(patch, f);
      if (!errs.length) ({ score, infra } = await this.measure(slot.dir, f, slot.id, job.dir));
      // refused for inline asm only: measured anyway (never kept as the checkpoint), so a match
      // that needs the asm reaches a human
      else if (errs.every(e => e.startsWith(ASM_REFUSED))) ({ score: asmScore, infra } = await this.measure(slot.dir, f, slot.id, job.dir));
    }
    if (infra && this.stopping !== 'now') return this.infraStop(slot, job, infra);
    // a checkpoint from before region scores: measure it once (in this slot, before it is freed),
    // so it is compared by region score like the rest rather than by STAGE
    const old = this.checkpoint(f);
    if (old && !old.regions && score?.regions && !job.oldCheckpoint && this.stopping !== 'now') {
      resetClone(slot.dir, job.base);
      const cpFile = path.join(this.cpDir(f), 'best.diff');
      if (git(slot.dir, 'apply', '--3way', '--whitespace=nowarn', cpFile).code) {
        resetClone(slot.dir, job.base);
        reapply(slot.dir, fs.readFileSync(cpFile, 'utf8'), f);
      }
      git(slot.dir, 'reset', '-q');
      const m = (await this.measure(slot.dir, f, slot.id)).score;
      if (m?.regions && m.stage === old.stage && m.diffs === old.diffs) {   // the same C as when it was saved
        old.regions = m.regions;
        writeJson(path.join(this.cpDir(f), 'best.json'), old);
        this.log(`${slot.id} ${f.name}: checkpoint measured: ${Swarm.describe(old)}`);
      }
    }
    fs.writeFileSync(path.join(job.dir, 'ab.json'), JSON.stringify({
      arm: this.regionsArm(f) ? 'regions' : 'control', resumed: job.resumed ? Swarm.describe(job.resumed) : null,
      status: res.status || null, timedOut: !!job.timedOut, stage: score, ended: stamp() }, null, 1));
    if (this.stopping === 'now') return;
    slot.job = null;
    const note = res.note || (job.timedOut ? 'killed at --big-minutes' : 'no note');
    if (asmScore?.stage === 'match') {
      this.needsHack(job);
      return this.parkBig(job, `matches only with inline asm: needs a human --hack. ${note}`);
    }
    if (score?.stage === 'match') {
      (this.history[f.key] ||= { name: f.name, attempts: 0, notes: [] }).big = true;   // for `stats`
      job.note = note;
      job.patch = file;
      this.integQueue.push(job);
      this.log(`${slot.id} ${f.name}: try says MATCH, queued for integration`);
      return;
    }
    const h = this.history[f.key] ||= { name: f.name, attempts: 0, notes: [] };
    Object.assign(h, { big: true, size: f.size, bigSessions: (h.bigSessions || 0) + 1 });
    const cp = this.checkpoint(f);
    // a checkpoint that no longer applied: the session's first C that builds replaces it, whatever
    // its score (the old one can't be resumed), and a session that ports nothing isn't stale
    const improved = !!score && (!!job.oldCheckpoint || Swarm.better(score, cp));
    const port = !!job.oldCheckpoint && !improved;
    const desc = score ? Swarm.describe(score)
      : errs.length ? `patch refused: ${errs.join('; ')}${asmScore ? ` (with it: ${Swarm.describe(asmScore)})` : ''}` : patch.trim() ? 'C did not build' : 'no C left';
    if (improved) {
      this.saveCheckpoint(job, file, score, note, res.next);
      h.bigStale = 0;
    } else if (port) {
      this.noteCheckpoint(job, `could not port this checkpoint (${job.oldCheckpoint.why}): ${desc}. ${note}`);
    } else {
      h.bigStale = (h.bigStale || 0) + 1;
      this.noteCheckpoint(job, `no progress (${desc}). ${note}${res.next ? ` Next: ${res.next}` : ''}`);
    }
    this.record(job, 'progress', `${desc}${improved ? ' (new best)' : ''}. ${note}`);
    this.log(`${slot.id} ${f.name}: big session ${h.bigSessions}: ${desc}` +
             `${improved ? ', new best' : port ? ', checkpoint not ported (not counted as stale)' : `, no progress (${h.bigStale} in a row)`}` +
             `; says ${res.status || 'nothing'}`);
    if (res.status === 'parked') return this.parkBig(job, `worker parked it: ${note}`);
    const best = this.checkpoint(f);
    if (res.status === 'needs_permute' && this.pslots.length && best?.stage === 'registers' && !this.stopping) {
      job.note = note;
      job.bestPatch = path.join(this.cpDir(f), 'best.diff');
      job.base = best.base;
      this.permQueue.push(job);
      this.log(`${slot.id} ${f.name}: best C handed to the permuter pool (${this.o.bigPermuteMinutes} min)`);
      return;
    }
    this.continueBig(job, note);
  }

  // another big session, unless the function is out of sessions, stale, or the run is stopping
  continueBig(job, note, extra = {}) {
    const { f } = job;
    if (this.stopping) {
      this.finished.add(f.key);
      return;
    }
    if (!this.bigLeft(f)) {
      const h = this.history[f.key] || {};
      return this.parkBig(job, (h.bigStale || 0) >= this.o.bigStale
        ? `${h.bigStale} big sessions in a row without progress` : `${h.bigSessions} big sessions used`);
    }
    this.retry.push(this.bigJob(f, job.attempt + 1, [...job.notes, note].slice(-6), { retry: true, ...extra }));
  }

  // a C that matches only with inline asm: a human can commit it with `done --hack "reason"`
  needsHack(job) {
    const { f } = job;
    (this.hacks ||= []).push(f.name);
    fs.appendFileSync(this.paths.report, `- ${stamp()} **${f.name}** (${f.seg}, ${f.size} B): matches only with inline asm: ` +
                      `needs a human \`--hack\` (patch: ${path.join(job.dir, 'patch.diff')})\n`);
    this.log(`${f.name}: matches only with inline asm; listed in report.md for a human --hack`);
  }

  parkBig(job, why) {
    const cp = this.checkpoint(job.f);
    this.stats.parked++;
    this.finished.add(job.f.key);
    this.record(job, 'parked', `big mode: ${why}`);
    this.log(`${job.f.name}: parked (big mode): ${why}` +
             `${cp ? `; best ${Swarm.describe(cp)} kept in ${this.cpDir(job.f)}` : ''}`);
  }

  // ---- the permuter pool: no Claude, just CPU; an improvement goes back to a worker to port
  async runPermute(ps, job) {
    const { f } = job;
    ps.job = job;
    ps.killed = false;
    job.permuted = true;
    try {
      makeClone(ps.dir, this.integ, 'HEAD', m => this.log(m));
      resetClone(ps.dir, job.base);
      gitOk(ps.dir, 'apply', '--3way', '--whitespace=nowarn', job.bestPatch);
      fs.rmSync(path.join(ps.dir, 'build', 'permuter', f.name), { recursive: true, force: true });
      const mins = job.big ? this.o.bigPermuteMinutes : this.o.permuteMinutes;
      job.permStarted = Date.now();
      job.permMinutes = mins;
      this.log(`${ps.id} permuting ${f.name} for ${mins} min (${this.permCpus} cpus)`);
      // a big function: only the C of the blocks that differ (decomp.py permute --differing)
      const r = await this.dk(ps.dir, ['make', 'permute', `FUNC=${ref(f)}`, `MINUTES=${mins}`, ...(job.big ? ['ARGS=--differing'] : [])], ps.id,
                              { cpus: this.permCpus, minutes: mins + 20 });
      fs.writeFileSync(path.join(job.dir, 'permute.txt'), r.out);
      this.permStats(r.out, job.big ? 'pool-big' : 'pool');
      if (this.stopping) {          // a stop cuts permuter runs short: no worker would port the result
        this.finished.add(f.key);
        this.ledger(job, 'stopped');
        this.log(`${ps.id} ${f.name}: permuter stopped; worker's C kept in ${job.bestPatch}`);
        return;
      }
      const m = r.out.match(/permuter best score (\d+)(?: \(base (\d+)\))?: (\S+)\/source\.c/);
      const best = m ? +m[1] : null, base = m && m[2] !== undefined ? +m[2] : null;
      if (!m || (base !== null && best >= base)) {
        const why = r.timedOut ? 'permuter timed out' : 'permuter found no improvement';
        if (job.big) {
          ps.job = null;
          this.noteCheckpoint(job, why);
          return this.continueBig(job, why);
        }
        // the worker's C goes on as the closest so far (the permuter usually ends registers-stage
        // near misses here)
        const { score } = await this.measure(ps.dir, f, ps.id);
        const kept = this.keepCheckpoint(job, job.bestPatch, score, `${job.note} (${why})`);
        return this.notMatched(ps, job, `${why}; worker: ${job.note}${kept ? ` (${Swarm.describe(score)}, kept as the closest)` : ''}`);
      }
      const outDir = path.join(ps.dir, m[3]);
      let diff = fs.existsSync(path.join(outDir, 'diff.txt')) ? fs.readFileSync(path.join(outDir, 'diff.txt'), 'utf8') : '';
      if (diff.length > 6000) diff = diff.slice(0, 6000) + '\n... (cut)';
      fs.writeFileSync(path.join(job.dir, 'permuter-diff.txt'), diff);
      if (fs.existsSync(path.join(outDir, 'source.c'))) fs.copyFileSync(path.join(outDir, 'source.c'), path.join(job.dir, 'permuter-source.c'));
      const note = `permuter: score ${base ?? '?'} -> ${best}${best === 0 ? ' (MATCH)' : ''}`;
      // a match goes straight to the gate when try confirms it: no Claude session to port it
      if (best === 0 && !this.stopping && await this.applyPermuted(ps, job, outDir, note)) return;
      this.record(job, 'permuted', note);
      this.log(`${ps.id} ${f.name}: ${note}; back in the queue for a worker to port`);
      ps.job = null;
      if (!this.stopping)
        this.retry.unshift({ f, model: job.model, attempt: job.big ? job.attempt + 1 : job.attempt, retry: true, permuted: true,
                             big: job.big, notes: [...job.notes, `${job.note} (${note})`], startPatch: job.bestPatch,
                             permuterDiff: diff || '(no diff.txt)', permuterScore: { base: base ?? '?', best } });
      else this.finished.add(f.key);
    } catch (e) {
      this.fail(ps, job, `permuter: ${e.message}`, false);
    } finally {
      ps.job = null;
    }
  }

  // the permuter's matching version, applied in its clone and checked with try (decomp.py
  // permute-apply), queued for the gate like a worker's match. False (the port path) on any doubt
  async applyPermuted(ps, job, outDir, note) {
    const { f } = job;
    const rel = path.relative(ps.dir, outDir).split(path.sep).join('/');
    const r = await this.dk(ps.dir, ['python3', 'tools/scripts/decomp.py', 'permute-apply', ref(f), rel], ps.id,
                            { minutes: TOOL_MINUTES });
    fs.writeFileSync(path.join(job.dir, 'permute-apply.txt'), r.out);
    if (!/^PERMUTE-RESULT \S+ kept before \S+ after 0\s*$/m.test(r.out)) return false;
    const patch = git(ps.dir, 'diff', job.base, '--', 'src', 'include').out;   // the worker's C and the permuter's change
    if (!patch.trim() || this.checkPatch(patch, f).length) return false;
    const file = path.join(job.dir, 'patch.diff');
    fs.writeFileSync(file, patch);
    Object.assign(job, { patch: file, note: `matched by the permuter (${note}); worker: ${job.note}` });
    ps.job = null;
    this.integQueue.push(job);
    this.log(`${ps.id} ${f.name}: ${note}; try confirms it, queued for integration without a port session`);
    return true;
  }

  record(job, status, note) {
    const h = this.history[job.f.key] ||= { name: job.f.name, attempts: 0, notes: [] };
    h.size = job.f.size;
    // charged: a worker's go at it that ended (infra: the environment broke; error: the worker or
    // permuter never ran properly)
    if (!['conflict', 'permuted', 'gate-refused', 'progress', 'infra', 'error'].includes(status) && !job.big) h.attempts++;
    h.last = status;
    h.notes = [...h.notes, `${stamp()} ${job.model}: ${status}: ${note}`].slice(-6);
    writeJson(this.paths.history, this.history);
    fs.appendFileSync(this.paths.report, `- ${stamp()} **${job.f.name}** (${job.f.seg}, ${job.f.size} B, ${job.model}): ${status}. ${note}\n`);
    this.ledger(job, status);
    this.results.unshift({ at: new Date().toISOString(), name: job.f.name, seg: job.f.seg, size: job.f.size, outcome: status,
                           model: job.model, usd: job.usage?.usd != null ? +job.usage.usd.toFixed(2) : null });
    if (this.results.length > 100) this.results.length = 100;
  }

  // decomp.py permute's PERMUTE-STATS line (speed, base and best score) to permuter.jsonl, for
  // `stats` (and `decomp.py permstats <swarm dir>/permuter.jsonl`)
  permStats(out, kind) {
    const m = out.match(/^PERMUTE-STATS (\{.*\})\s*$/m);
    if (!m) return;
    try {
      fs.appendFileSync(this.paths.permuter, JSON.stringify({ run: this.id, kind, ...JSON.parse(m[1]) }) + '\n');
    } catch (e) {
      this.log(`permuter stats not recorded: ${e.message}`);
    }
  }

  // one line per worker session in ledger.jsonl, written when its outcome is first known (`stats`
  // reads it): the function, the kind of session, its usage and what came of it
  ledger(job, outcome) {
    if (!job.usage || job.ledgered) return;
    job.ledgered = true;
    this.sessions++;
    const { f } = job, u = job.usage;
    const kind = `${job.big ? 'big' : 'normal'}${job.gateFix ? '-gatefix' : job.permuterDiff ? '-permuted' : job.startPatch ? '-port' : job.chained ? '-chained' : ''}`;
    fs.appendFileSync(this.paths.ledger, JSON.stringify({
      run: this.id, at: stamp(), key: f.key, name: f.name, seg: f.seg, size: f.size, kind, model: job.model,
      attempt: job.attempt, twin: f.twin || null, outcome, timedOut: !!job.timedOut, usd: +u.usd.toFixed(4), estimated: u.estimated,
      known: u.known, turns: u.turns, tokens: u.tokens, minutes: u.minutes, apiMinutes: u.apiMinutes,
      tries: job.tries, docReads: u.docReads, docGreps: u.docGreps, session: job.sessionId,
    }) + '\n');
  }

  // estimates of killed sessions are scaled by how this run's finished sessions compare with their
  // own transcripts priced (1 until there are some)
  priceScale() {
    return this.calib.priced > 0 ? this.calib.reported / this.calib.priced : 1;
  }

  notMatched(slot, job, note) {
    slot.job = null;
    this.record(job, 'parked', note);
    if (this.o.escalateModel && job.model !== this.o.escalateModel && job.attempt < this.o.maxAttempts && !this.stopping) {
      this.log(`${job.f.name}: not matched (${note}); retrying with ${this.o.escalateModel}`);
      this.retry.push({ f: job.f, model: this.o.escalateModel, attempt: job.attempt + 1, notes: [...job.notes, note],
                        retry: true, permuted: job.permuted });
      return;
    }
    if (this.o.escalateBig && this.bslots.length && !job.big && !this.stopping) {
      this.history[job.f.key].big = true;
      writeJson(this.paths.history, this.history);
      this.log(`${job.f.name}: not matched (${note}); escalated to big mode`);
      this.retry.push(this.bigJob(job.f, 1, [...job.notes, note], { retry: true }));
      return;
    }
    this.stats.parked++;
    this.finished.add(job.f.key);
    this.log(`${job.f.name}: parked: ${note}`);
  }

  // `charge` false: the failure isn't the function's (the worker or permuter never ran properly);
  // it still counts towards the failures-in-a-row stop
  fail(slot, job, why, charge = true) {
    if (slot) slot.job = null;
    this.stats.failed++;
    this.consecutiveFailures++;
    this.finished.add(job.f.key);
    this.record(job, charge ? 'failed' : 'error', why);
    this.log(`${job.f.name}: FAILED: ${why} (${this.consecutiveFailures} in a row)`);
  }

  // ---- the single integrator. A batch: each match through decomp.py done (scope, lint, function)
  // with its own commit, then one full build for the whole batch (decomp.py verify), then one push.
  // If the batch's build fails, nothing is pushed and its functions go through the full gate singly.
  async integrateBatch(jobs) {
    this.integrating = jobs;
    const batch = jobs.length > 1;
    const paused = this.pausePermuters();         // the gate's builds get every core
    try {
      if (!this.syncInteg()) throw new Error(`can't fetch ${this.o.remote}/${this.o.branch}`);
      const base = gitOk(this.integ, 'rev-parse', 'HEAD');
      if (batch) this.log(`gate: batch of ${jobs.length}: ${jobs.map(j => j.f.name).join(', ')}`);
      const passed = [];
      for (const job of jobs) {
        if (this.stopping === 'now') return;
        if (await this.gateOne(job, batch)) passed.push(job);
      }
      if (!passed.length || this.stopping === 'now') return;
      if (batch) {
        const v = await this.dk(this.integ, ['python3', 'tools/scripts/decomp.py', 'verify'], 'integ', { minutes: 2 * TOOL_MINUTES });
        fs.writeFileSync(path.join(passed[0].dir, 'batch-verify.txt'), v.out);
        if (this.stopping === 'now') return;
        if (v.code !== 0 || !/^OK/m.test(v.out)) {
          resetClone(this.integ, base);
          this.log(`gate: the batch's full build failed (${v.out.trim().split('\n').pop()}); ` +
                   `redoing ${passed.length} one at a time with the full gate`);
          for (const job of passed.reverse()) this.integQueue.unshift(Object.assign(job, { single: true }));
          return;
        }
      }
      const push = git(this.integ, 'push', '-q', this.remoteUrl, `HEAD:refs/heads/${this.o.branch}`);
      if (push.code) {
        // someone pushed since our fetch: run the gate again on top of their commits
        if (/rejected|non-fast-forward|fetch first/i.test(push.err) && passed.every(j => (j.pushRetries = (j.pushRetries || 0) + 1) <= 3)) {
          this.log(`gate: push rejected, redoing ${passed.length} on the new tip of ${this.o.branch}: ` +
                   push.err.trim().split('\n').filter(l => /rejected|error|hint: Updates/i.test(l)).join(' | '));
          this.integQueue.unshift(...passed);
          return;
        }
        this.stopping ||= 'graceful';
        for (const job of passed) this.fail(null, job, `push to ${this.o.remote}/${this.o.branch} failed, stopping: ${push.err.trim()}`);
        return;
      }
      this.tip = gitOk(this.integ, 'rev-parse', 'HEAD');
      this.updateLocal();
      for (const job of passed) {
        this.stats.matched++;
        this.consecutiveFailures = 0;
        this.finished.add(job.f.key);
        this.record(job, 'matched', `${job.commit} ${job.note}`);
        this.log(`${job.f.name}: COMMITTED ${job.commit} ${job.subject}`);
      }
      await this.refreshQueue();   // done fills identical copies too
    } finally {
      this.unpause(paused);
      this.integrating = null;
    }
  }

  // one match through `done --commit` on top of the integration clone; true if it committed
  async gateOne(job, defer) {
    const { f } = job;
    let ap = git(this.integ, 'apply', '--3way', '--whitespace=nowarn', job.patch);
    if (ap.code) {
      resetClone(this.integ, 'HEAD');
      // the worker's C is fine, only its surroundings moved: put it in place ourselves
      const why = reapply(this.integ, fs.readFileSync(job.patch, 'utf8'), f);
      if (why === null) {
        this.log(`${f.name}: patch conflicted; re-applied its C to the new ${this.o.branch} mechanically`);
        ap = { code: 0 };
      } else this.log(`${f.name}: patch conflicted and can't be re-applied mechanically (${why})`);
    }
    if (ap.code) {
      resetClone(this.integ, 'HEAD');
      this.stats.conflicts++;
      this.record(job, 'conflict', (ap.err || ap.out).trim().split('\n').slice(-2).join(' '));
      if (job.conflicts++ < 1 && !this.stopping) {
        this.log(`${f.name}: patch conflicts with newer commits; it goes back in the queue`);
        this.retry.push({ f, model: job.model, attempt: job.attempt, notes: job.notes, retry: true, conflicts: job.conflicts,
                          startPatch: job.patch });
      } else this.fail(null, job, 'patch conflicts twice');
      return false;
    }
    if (job.reconcile) {
      // a declaration clash: keep the tree's declarations over the patch's (decomp.py reconcile)
      const rc = await this.dk(this.integ, ['python3', 'tools/scripts/decomp.py', 'reconcile'], 'integ',
                               { minutes: TOOL_MINUTES });
      this.log(`${f.name}: ${rc.out.trim().split('\n').pop()}`);
    }
    const trailers = [...this.o.trailers, `Swarm-Run: ${this.id} (${job.model})`];
    const args = ['python3', 'tools/scripts/decomp.py', 'done', ref(f), '--commit',
                  '--note', job.note || 'matched by a swarm worker', ...trailers.flatMap(t => ['--trailer', t]),
                  ...(defer ? ['--defer-build'] : [])];
    const before = gitOk(this.integ, 'rev-parse', 'HEAD');
    const r = await this.dk(this.integ, args, 'integ', { minutes: 2 * TOOL_MINUTES });
    fs.writeFileSync(path.join(job.dir, 'done.txt'), r.out);
    const after = gitOk(this.integ, 'rev-parse', 'HEAD');
    if (this.stopping === 'now') return false;
    if (r.code !== 0 || after === before) {
      resetClone(this.integ, before);
      const why = gateReason(r.out);
      const infra = infraError(r.out, r.code);
      if (infra) {
        this.log(`${f.name}: the gate hit an environment error, not a problem with the function: ` +
                 `${infra}. Stopping the run; fix it, then \`run --resume ${this.id}\` gates the ` +
                 'verified matches without matching them again');
        if (!this.stopping) this.stopping = 'graceful';
        return false;
      }
      // a declaration clash with matches committed meanwhile: the worker's C was right every time
      // (2026-10-19), so keep the tree's declarations and gate once more before paying a worker
      if (DECL_CLASH.test(why) && !job.reconcile && !this.stopping) {
        this.log(`${f.name}: done refused it on a declaration clash; reconciling the headers`);
        job.reconcile = true;
        const ok = await this.gateOne(job, defer);
        if (ok) this.stats.reconciled = (this.stats.reconciled || 0) + 1;
        return ok;
      }
      // the bytes matched (try said so): what's left is usually fixable (a header, lint), once
      if (!job.gateFix && !this.stopping) {
        this.record(job, 'gate-refused', why.replace(/\n/g, ' '));
        this.log(`${f.name}: done refused it; back to a worker to fix: ${why.split('\n').pop()}`);
        this.retry.unshift({ f, model: job.model, attempt: job.attempt, retry: true, permuted: job.permuted,
                             big: job.big, notes: job.notes, startPatch: job.patch, gateFix: why });
        return false;
      }
      this.fail(null, job, `done refused: ${why.replace(/\n/g, ' ')}`);
      return false;
    }
    const dirty = git(this.integ, 'status', '--porcelain').out.split('\n')
      .filter(l => l.trim() && !l.endsWith('tools/dk'));   // our copy of tools/dk
    if (dirty.length) this.log(`${f.name}: note: done left changes behind, discarded: ${dirty.join(', ')}`);
    resetClone(this.integ, 'HEAD');
    job.commit = gitOk(this.integ, 'rev-parse', '--short', 'HEAD');
    job.subject = gitOk(this.integ, 'log', '-1', '--format=%s');
    return true;
  }

  // --resume RUN[,RUN...]: matches an earlier run verified with `try` but stopped before gating go straight
  // to the gate (still unmatched ones only: the queue lists those)
  resumeFrom(runId) {
    // results/ is keyed by function key; runs before 2026-10-16 keyed it by name (unique names only)
    const byDir = new Map([...this.queue.filter(f => ref(f) === f.name).map(f => [f.name, f]),
                           ...this.queue.map(f => [keyDir(f), f])]);
    let n = 0;
    for (const name of fs.existsSync(this.paths.results) ? fs.readdirSync(this.paths.results) : [])
      for (const d of fs.readdirSync(path.join(this.paths.results, name)).filter(d => d.endsWith(`-${runId}`))) {
        const dir = path.join(this.paths.results, name, d), f = byDir.get(name);
        const tried = path.join(dir, 'try.txt');
        // not in the queue = matched already; done.txt alone proves nothing (a stop may cut a batch)
        // one per function: a run can verify two attempts of it, and the second would gate again
        if (!f || this.integQueue.some(j => j.f === f) || !fs.existsSync(tried) ||
            !/^MATCH /m.test(fs.readFileSync(tried, 'utf8'))) continue;
        const res = readJson(path.join(dir, 'result.json'), {});
        this.integQueue.push({ f, model: this.o.model, resumed: true, attempt: 1, notes: [], conflicts: 0, dir,
                               patch: path.join(dir, 'patch.diff'), note: res.note || '' });
        n++;
      }
    this.log(`resumed ${n} verified match(es) from run ${runId} into the gate queue`);
  }

  // freeze running permuter containers while the gate builds (docker pause), and thaw them after
  pausePermuters() {
    const ids = this.pslots.filter(p => p.job).flatMap(p =>
      sh('docker', ['ps', '-q', '--filter', `label=${LABEL}-slot=${this.id}-${p.id}`]).out.split(/\s+/).filter(Boolean));
    if (ids.length) sh('docker', ['pause', ...ids]);
    return ids;
  }

  unpause(ids) {
    if (ids.length) sh('docker', ['unpause', ...ids]);
  }

  // ---- stop switch
  checkStop() {
    if (fs.existsSync(this.paths.stop)) {
      const mode = fs.readFileSync(this.paths.stop, 'utf8').trim() === 'now' ? 'now' : 'graceful';
      if (this.stopping !== mode) {
        this.log(`STOP file found (${mode})`);
        this.stopping = mode;
      }
    }
    const mins = (Date.now() - this.started) / 60e3;
    const reason = mins > this.o.maxMinutes ? `--max-minutes ${this.o.maxMinutes} reached`
      : this.costUsd >= this.o.maxRunUsd ? `run cost ${this.costUsd.toFixed(2)} reached --max-run-usd ${this.o.maxRunUsd}`
      : this.consecutiveFailures >= this.o.maxConsecutiveFailures ? `${this.consecutiveFailures} failures in a row`
      : null;
    if (reason && !this.stopping) {
      this.log(`stopping: ${reason}`);
      this.stopping = 'graceful';
    }
  }

  hardStop() {
    this.log('hard stop: killing workers and containers');
    for (const s of [...this.slots, ...this.bslots]) if (s.job) clearTimeout(s.job.timer);
    for (const p of this.children) killTree(p.pid);
    const n = dockerKill(`${LABEL}=${this.id}`, this.o.dir);
    if (n) this.log(`killed ${n} container(s)`);
    try { resetClone(this.integ, 'HEAD'); } catch { /* best effort */ }
  }

  // `permute --only f1,f2`: permuter only, no Claude. Each function gets a clone of this checkout's
  // HEAD with its big-mode checkpoint applied, and `decomp.py permute --apply-best` for --minutes
  // (all functions at once, the cores split between them). A better result becomes the function's
  // checkpoint, so the next session starts from it; a match also goes to results/ so
  // `run --resume <id> --only NAME` sends it through the gate.
  async permuteOnly() {
    const o = this.o;
    if (o.near && !o.only?.length) {
      // the near misses: every checkpoint whose C is at the registers stage, fewest diffs first
      const d = path.join(o.dir, 'checkpoints');
      const cps = fs.existsSync(d) ? fs.readdirSync(d).map(n => [n, readJson(path.join(d, n, 'best.json'), null)]) : [];
      // still INCLUDE_ASM on main (a matched function's checkpoint is left over)
      // (by segment too when the checkpoint records it: slot overlays share names)
      const unmatched = (n, j) => git(REPO, 'grep', '-q', '-E',
        `INCLUDE_ASM\\(${j.seg ? `"asm/nonmatchings/${j.seg}"` : '[^)]*'}, ${j.name || n}\\)`, 'origin/main', '--', 'src').code === 0;
      o.only = cps.filter(([n, j]) => j?.stage === 'registers' && unmatched(n, j)).sort((x, y) => x[1].diffs - y[1].diffs)
        .map(([n, j]) => j.key || n).slice(0, 16);
      console.log(`near misses (registers stage): ${o.only.join(', ') || 'none'}`);
      if (!o.only.length) return;
    }
    if (!o.only?.length) die('permute needs --only f1,f2,... or --near (functions with a checkpoint or C to start from)');
    if (o.only.length > 16) die('permute: at most 16 functions at once');
    if (sh('docker', ['info']).code) die('Docker is not reachable');
    fs.mkdirSync(o.dir, { recursive: true });
    const lock = readJson(this.paths.lock, null);
    if (lock && alive(lock.pid)) die(`another run (pid ${lock.pid}) owns ${o.dir}`);
    if (fs.existsSync(this.paths.stop)) {
      if (!o.clearStop) die(`${this.paths.stop} exists: delete it or pass --clear-stop`);
      fs.unlinkSync(this.paths.stop);
    }
    writeJson(this.paths.lock, { pid: process.pid, id: this.id, started: stamp() });
    const minutes = o.minutes !== undefined ? Number(o.minutes) : 30;
    const jobs = o.jobs !== undefined ? Number(o.jobs) : Math.max(1, Math.floor((os.cpus().length - 1) / o.only.length));
    const base = gitOk(REPO, 'rev-parse', 'HEAD');
    this.log(`permute ${this.id}: ${o.only.join(', ')}; ${minutes} min, ${jobs} jobs each, from ${base.slice(0, 8)}`);
    const killAll = () => {
      for (const p of this.children) killTree(p.pid);
      dockerKill(`${LABEL}=${this.id}`, o.dir);
    };
    process.on('SIGINT', () => { this.log('Ctrl+C: killing the permuters (nothing is saved)'); killAll(); });
    const watch = setInterval(() => { if (fs.existsSync(this.paths.stop)) { this.log('STOP file: killing the permuters'); killAll(); } }, 2000);
    const one = async (name, i) => {
      // a name or a key (--near lists keys for checkpoints saved with one); the C name comes from
      // the checkpoint, and a name finds a checkpoint saved under its key
      const f = { name, key: name.includes(':') ? name : undefined, ref: name };
      if (!f.key && !this.checkpoint(f)) {
        const d = this.paths.checkpoints;
        const hits = (fs.existsSync(d) ? fs.readdirSync(d) : []).map(n => readJson(path.join(d, n, 'best.json'), null))
          .filter(j => j?.name === name && j.key);
        if (hits.length === 1) f.key = hits[0].key;
      }
      const cp0 = this.checkpoint(f);
      if (cp0?.name) f.name = cp0.name;
      const slot = `h${i + 1}`, dir = path.join(o.dir, 'permuters', slot);
      makeClone(dir, REPO, 'HEAD', m => this.log(m));
      resetClone(dir, base);
      const cp = this.checkpoint(f);
      if (cp) {
        const patch = path.join(this.cpDir(f), 'best.diff');
        if (git(dir, 'apply', '--3way', '--whitespace=nowarn', patch).code) {
          resetClone(dir, base);
          const why = reapply(dir, fs.readFileSync(patch, 'utf8'), f);
          if (why !== null) return this.log(`${name}: its checkpoint doesn't apply to HEAD any more (${why}); skipped`);
        }
        git(dir, 'reset', '-q');   // a --3way apply stages its result; keep the diff below simple
      }
      const { score: before, out: beforeOut } = await this.measure(dir, f, slot);
      if (!before) {
        // say why: a checkpoint that stopped building (a later match declared something it also
        // declares, differently) needs a big session to port it, not the permuter
        const why = /BUILD FAILED/.test(beforeOut)
          ? `its C no longer builds on this base (${(beforeOut.match(/^\s*line \d+ of preprocessed .*$/m) || ['see try'])[0].trim().slice(0, 140)}); a big session must port it`
          : /STILL-ASM/.test(beforeOut) ? 'no C (still INCLUDE_ASM, no checkpoint)' : beforeOut.trim().split('\n').pop().slice(0, 140);
        return this.log(`${name}: skipped: ${why}`);
      }
      this.log(`${name}: starting from ${Swarm.describe(before)}${cp ? ' (checkpoint)' : ''}`);
      const r = await this.dk(dir, ['python3', 'tools/scripts/decomp.py', 'permute', ref(f), '--minutes', String(minutes), '--differing',
                                    '--jobs', String(jobs), '--apply-best'], slot, { cpus: jobs, minutes: minutes + 10 });
      this.permStats(r.out, 'permute-only');
      const res = r.out.match(/^PERMUTE-RESULT \S+ (\w+) before (\S+)(?: after (\S+))?/m);
      if (!res) return this.log(`${name}: the permuter didn't finish (${r.timedOut ? 'killed' : r.out.trim().split('\n').pop()})`);
      if (res[1] !== 'kept') return this.log(`${name}: no improvement (${res[1]}${res[3] ? `, best ${res[3]}` : ''})`);
      const { score: after, out: triedOut } = await this.measure(dir, f, slot);
      const tried = { out: triedOut };
      if (!after || !Swarm.better(after, before)) return this.log(`${name}: permuter result not better on recheck; not saved`);
      const patchFile = path.join(dir, '.permute.diff');
      fs.writeFileSync(patchFile, git(dir, 'diff', '--', 'src', 'include').out);
      const job = { f, model: 'permuter', base };
      const old = this.checkpoint(f);
      if (!old || Swarm.better(after, old)) this.saveCheckpoint(job, patchFile, after, `permuter ${minutes} min, from ${before.diffs}/${before.total}.`);
      if (after.stage === 'match') {
        const rd = path.join(this.paths.results, keyDir(f), `permute-${this.id}`);
        fs.mkdirSync(rd, { recursive: true });
        fs.copyFileSync(patchFile, path.join(rd, 'patch.diff'));
        fs.writeFileSync(path.join(rd, 'try.txt'), tried.out);
        writeJson(path.join(rd, 'result.json'), { status: 'matched', note: `matched by the permuter (${minutes} min)` });
        return this.log(`${name}: MATCH. Gate it: node tools/swarm/swarm.mjs run --branch main --only ${ref(f)} --resume ${this.id}`);
      }
      this.log(`${name}: ${before.stage} ${before.diffs}/${before.total} -> ${after.stage} ${after.diffs}/${after.total}, saved as its checkpoint`);
    };
    try {
      await Promise.all(o.only.map((n, i) => one(n, i).catch(err => this.log(`${n}: error: ${err.message}`))));
    } finally {
      clearInterval(watch);
      fs.rmSync(this.paths.lock, { force: true });
    }
    this.log(`permute ${this.id} finished`);
  }

  writeStatus(state = 'running') {
    writeJson(this.paths.status, {
      id: this.id, pid: process.pid, state, stopping: this.stopping, started: new Date(this.started).toISOString(),
      branch: this.o.branch, stats: this.stats, costUsd: +this.costUsd.toFixed(2), leases: this.leases, queued: this.queue.length,
      workers: [...this.slots, ...this.bslots].map(s => ({ slot: s.id, func: s.job?.f.name || null, model: s.job?.model || null, pid: s.job?.pid || null,
                                      minutes: s.job ? +((Date.now() - s.job.started) / 60e3).toFixed(1) : null })),
      permuters: this.pslots.map(s => ({ slot: s.id, func: s.job?.f.name || null })),
      permuteWaiting: this.permQueue.map(j => j.f.name),
      integrating: this.integrating ? this.integrating.map(j => j.f.name).join(', ') : null, waiting: this.integQueue.map(j => j.f.name),
    });
    this.writePage(state);
  }

  // the dashboard's swarm card: what the run holds in memory, nothing that needs Docker or git
  pageStatus(state) {
    const o = this.o, now = Date.now(), mins = t => +((now - t) / 60e3).toFixed(1);
    const run = {
      id: this.id, pid: process.pid, state, stopping: this.stopping, started: new Date(this.started).toISOString(),
      ended: state === 'running' ? null : new Date(now).toISOString(), branch: o.branch,
      target: o.only ? `--only ${o.only.join(',')}` : o.seg || 'all game code', dryRun: !!o.dryRun,
      limits: { minutes: o.maxMinutes, usd: o.maxRunUsd, functions: o.maxFunctions },
      pool: { workers: o.workers, big: o.bigWorkers, permuters: o.permuters, bigBytes: o.bigBytes },
      costUsd: +this.costUsd.toFixed(2), stats: this.stats, leases: this.leases, queued: this.queue.length,
    };
    const fields = f => ({ func: f.name, seg: f.seg, size: f.size });
    return {
      v: 1, updated: new Date(now).toISOString(), run,
      slots: [...this.slots, ...this.bslots].map(s => !s.job ? { slot: s.id, big: !!s.big, func: null } : {
        slot: s.id, big: !!s.big, ...fields(s.job.f), model: s.job.model, attempt: s.job.attempt,
        session: s.big ? (this.history[s.job.f.key]?.bigSessions || 0) + 1 : null,
        minutes: mins(s.job.started), limit: s.big ? o.bigMinutes : o.workerMinutes,
        from: s.job.resumed ? Swarm.describe(s.job.resumed) : null }),
      permuters: this.pslots.map(s => !s.job ? { slot: s.id, func: null } : {
        slot: s.id, ...fields(s.job.f), minutes: s.job.permStarted ? mins(s.job.permStarted) : null, limit: s.job.permMinutes ?? null }),
      permuteWaiting: this.permQueue.map(j => j.f.name),
      gate: { checking: (this.integrating || []).map(j => j.f.name), waiting: this.integQueue.map(j => j.f.name) },
      results: this.results,
      log: this.logTail.slice(-10),
      runs: [{ id: this.id, started: run.started, ended: run.ended, target: run.target, matched: this.stats.matched,
               parked: this.stats.parked, failed: this.stats.failed, sessions: this.sessions, usd: run.costUsd },
             ...(this.pastRuns ||= readRuns(o.dir, this.id))].slice(0, 8),
      parks: this.parkList(),
    };
  }

  // every function a run has parked (history `last` = parked), for the dashboard's Parked total and
  // list: its best C so far and last note. The page drops the ones its code map shows matched since
  parkList() {
    return Object.entries(this.history).filter(([, h]) => h.last === 'parked').map(([key, h]) => {
      const cp = this.checkpoint({ key, name: h.name });
      const note = String(h.notes?.at(-1) || '').replace(/^S+ S+ /, '');
      return { key, name: h.name, best: cp ? Swarm.describe(cp) : null, note: note.slice(0, 400) };
    });
  }

  // docs/swarm.js in your checkout (git-ignored), which docs/index.html loads as a script: a file://
  // page can't fetch JSON. Written when something changed, else every 15 s, so the card can tell a
  // run that stopped answering; the clocks alone don't count as a change
  writePage(state) {
    if (!this.o.page) return;
    // never a reason for the run to stop: a failed write (a browser reading the file at that
    // moment, on Windows) is retried with the next change or heartbeat
    try {
      const st = this.pageStatus(state), now = Date.now();
      const sig = JSON.stringify([st.run, st.slots.map(s => s.func), st.permuters.map(p => p.func), st.permuteWaiting,
                                  st.gate, st.results.length, st.log.at(-1)]);
      if (state === 'running' && sig === this.pageSig && now - this.pageAt < 15e3) return;
      const file = path.resolve(REPO, this.o.page);
      fs.writeFileSync(file + '.tmp', `swarmStatus(${JSON.stringify(st)});\n`);
      fs.renameSync(file + '.tmp', file);
      this.pageSig = sig;
      this.pageAt = now;
    } catch (e) {
      if (!this.pageWarned) this.log(`note: the dashboard's swarm card (${this.o.page}) wasn't written: ${e.message}`);
      this.pageWarned = true;
    }
  }

  async run() {
    this.preflight();
    let interrupts = 0;
    process.on('SIGINT', () => {
      interrupts++;
      this.stopping = interrupts > 1 ? 'now' : 'graceful';
      this.log(interrupts > 1 ? 'Ctrl+C again: hard stop' : 'Ctrl+C: finishing current work (Ctrl+C again to kill)');
    });
    let exitCode = 0;
    try {
      this.setupInteg();
      await this.refreshQueue();
      if (this.o.resume) for (const id of String(this.o.resume).split(',')) this.resumeFrom(id.trim());
      const nbig = this.queue.filter(f => this.isBig(f)).length;
      this.log(`run ${this.id}: ${this.o.workers} workers, ${this.o.bigWorkers} big, ${this.o.permuters} permuters, ` +
               `${this.queue.length} candidates (${nbig} big: >= ${this.o.bigBytes} B or escalated)` +
               `${this.o.only ? ` (--only ${this.o.only.join(',')})` : ` in ${this.o.seg || 'all game code'}`}` +
               `${this.o.dryRun ? ', DRY RUN' : ''}${this.o.workerCmd ? `, worker command: ${this.o.workerCmd}` : ''}`);
      for (;;) {
        this.checkStop();
        if (this.stopping === 'now') {
          this.hardStop();
          exitCode = 130;
          break;
        }
        if (!this.integrating && this.integQueue.length) {
          // a batch of up to --batch-max; a function from a failed batch goes through alone
          let n = 1;
          // only if the branch's own gate has batch mode (done --defer-build)
          const batchable = fs.readFileSync(path.join(this.integ, 'tools/scripts/decomp.py'), 'utf8').includes('--defer-build');
          if (batchable && !this.integQueue[0].single)
            while (n < Math.min(this.o.batchMax, this.integQueue.length) && !this.integQueue[n].single) n++;
          this.integrateBatch(this.integQueue.splice(0, n)).catch(e => {
            this.log(`integration error: ${e.stack || e}`);
            this.stopping ||= 'graceful';
          });
        }
        if (this.stopping) {
          // no worker will port permuter results: drop waiting runs and cut running ones short
          for (const job of this.permQueue.splice(0)) {
            this.finished.add(job.f.key);
            this.ledger(job, 'stopped');
            this.log(`${job.f.name}: permuter run dropped by the stop; worker's C kept in ${job.bestPatch}`);
          }
          for (const ps of this.pslots)
            if (ps.job && !ps.killed) {
              ps.killed = true;
              dockerKill(`${LABEL}-slot=${this.id}-${ps.id}`, ps.dir);
            }
        } else
          for (const ps of this.pslots)
            if (!ps.job && this.permQueue.length)
              this.runPermute(ps, this.permQueue.shift()).catch(e => this.log(`permuter error: ${e.stack || e}`));
        // back-pressure: no new work while more matches wait for the gate than --max-queued
        if (!this.stopping && this.integQueue.length <= this.o.maxQueued)
          for (const slot of this.slots) {
            if (slot.job || (this.leases >= this.o.maxFunctions && !this.retry.length)) continue;
            if (this.startGapWait()) break;
            const job = this.nextJob();
            if (!job) break;
            job.conflicts ||= 0;
            try {
              this.startJob(slot, job);
            } catch (e) {
              slot.job = null;
              this.fail(null, job, `could not start: ${e.message}`, false);
            }
          }
        if (!this.stopping && this.integQueue.length <= this.o.maxQueued)
          for (const slot of this.bslots) {
            if (slot.job || (this.leases >= this.o.maxFunctions && !this.retry.some(j => j.big))) continue;
            if (this.startGapWait()) break;
            const job = this.nextJob(false, true);
            if (!job) break;
            job.conflicts ||= 0;
            try {
              this.startJob(slot, job);
            } catch (e) {
              slot.job = null;
              this.fail(null, job, `could not start: ${e.message}`, false);
            }
          }
        const idle = !this.activeJobs().length;
        const more = this.retry.length || (this.leases < this.o.maxFunctions && (this.nextJob(true) || this.nextJob(true, true)));
        if (idle && (this.stopping || !more)) break;
        this.writeStatus();
        await sleep(2000);
      }
    } catch (e) {
      this.log(`fatal: ${e.stack || e}`);
      this.hardStop();
      exitCode = 1;
    }
    this.writeStatus(exitCode === 130 ? 'killed' : 'finished');
    const s = this.stats;
    this.log(`run ${this.id} finished: ${s.matched} matched, ${s.parked} parked, ${s.failed} failed, ` +
             `${s.conflicts} conflicts. Commits are on ${this.o.branch}; notes in ${this.paths.report}`);
    if (this.staleCheckpoints?.length)
      this.log(`checkpoints that no longer apply (sessions port them by hand): ${[...new Set(this.staleCheckpoints)].join(', ')}`);
    if (this.hacks?.length) this.log(`match only with inline asm, for a human --hack (report.md): ${this.hacks.join(', ')}`);
    fs.rmSync(this.paths.lock, { force: true });
    fs.rmSync(this.paths.stop, { force: true });
    process.exit(exitCode);
  }
}

// ---------------------------------------------------------------- other commands

async function stop(o) {
  const p = n => path.join(o.dir, n);
  if (!fs.existsSync(o.dir)) die(`no swarm directory at ${o.dir}`);
  fs.writeFileSync(p('STOP'), o.now ? 'now\n' : 'graceful\n');
  const lock = readJson(p('lock'), null);
  if (!o.now) {
    console.log(lock && alive(lock.pid)
      ? `asked run pid ${lock.pid} to stop after its current work (\`stop --now\` to kill it)`
      : 'no run is active; the STOP file keeps the next `run` from starting until removed');
    return;
  }
  // the orchestrator kills its own workers within ~2 s; if it doesn't, do it from here
  for (let i = 0; i < 15 && lock && alive(lock.pid); i++) await sleep(1000);
  let forced = false;
  if (lock && alive(lock.pid)) {
    console.log(`run pid ${lock.pid} did not stop, killing it`);
    killTree(lock.pid);                 // on Windows /T takes the workers with it
    // elsewhere workers have their own process groups; their PIDs are fresh since the run was alive
    if (!WIN) for (const w of readJson(p('run.json'), {}).workers || []) killTree(w.pid);
    fs.rmSync(p('lock'), { force: true });
    forced = true;
  }
  const n = dockerKill(LABEL, o.dir);   // anything left over, from any run
  if (forced || !lock) {
    if (!fs.existsSync(p('STOP'))) fs.writeFileSync(p('STOP'), 'now\n');
    console.log(`stopped${n ? `; killed ${n} container(s)` : ''}. The STOP file stays: \`run --clear-stop\` to start again`);
  } else console.log(`run pid ${lock.pid} stopped itself${n ? `; killed ${n} leftover container(s)` : ''}`);
}

function status(o) {
  const st = readJson(path.join(o.dir, 'run.json'), null);
  if (!st) return console.log(`no run recorded in ${o.dir}`);
  const lock = readJson(path.join(o.dir, 'lock'), null);
  const live = lock && alive(lock.pid);
  console.log(`run ${st.id} ${live ? 'RUNNING' : st.state} (pid ${st.pid}), branch ${st.branch}` +
              `${st.stopping ? `, stopping: ${st.stopping}` : ''}${fs.existsSync(path.join(o.dir, 'STOP')) ? ', STOP file present' : ''}`);
  console.log(`  ${st.stats.matched} matched, ${st.stats.parked} parked, ${st.stats.failed} failed, ` +
              `${st.stats.conflicts} conflicts; ${st.leases} leased, ${st.queued} in the queue`);
  for (const w of st.workers) console.log(`  ${w.slot}: ${w.func ? `${w.func} (${w.model}, ${w.minutes} min)` : 'idle'}`);
  for (const p of st.permuters || []) console.log(`  ${p.slot}: ${p.func ? `permuting ${p.func}` : 'idle'}`);
  if (st.permuteWaiting?.length) console.log(`  waiting for a permuter: ${st.permuteWaiting.join(', ')}`);
  if (st.integrating || st.waiting.length)
    console.log(`  integrating ${st.integrating || '-'}; waiting ${st.waiting.join(', ') || '-'}`);
  const log = path.join(o.dir, 'swarm.log');
  if (fs.existsSync(log)) console.log(fs.readFileSync(log, 'utf8').trim().split('\n').slice(-8).map(l => '  | ' + l).join('\n'));
}

// outcomes by function size across all runs: where normal mode stops working is where --big-bytes goes
function stats(o) {
  const h = readJson(path.join(o.dir, 'history.json'), {});
  const size = {};
  const rep = path.join(o.dir, 'report.md');
  if (fs.existsSync(rep))
    for (const m of fs.readFileSync(rep, 'utf8').matchAll(/\*\*(\w+)\*\* \([^,]+, (\d+) B/g)) size[m[1]] = +m[2];
  const rows = Object.values(h).map(e => ({ ...e, size: e.size ?? size[e.name] })).filter(e => e.size !== undefined);
  const edges = [0, 150, 250, 400, 600, 900, 1400, Infinity];
  console.log(`function outcomes by size (${rows.length} functions; --big-bytes is ${o.bigBytes})`);
  console.log('  bytes        tried  matched  normal-mode  big-mode  parked  attempts  big sessions');
  for (let i = 0; i + 1 < edges.length; i++) {
    const b = rows.filter(e => e.size >= edges[i] && e.size < edges[i + 1]);
    if (!b.length) continue;
    const ok = b.filter(e => e.last === 'matched');
    const big = b.filter(e => e.big);
    const label = `${edges[i]}-${edges[i + 1] === Infinity ? '' : edges[i + 1] - 1}`.padEnd(11);
    console.log(`  ${label} ${String(b.length).padStart(5)}  ${`${ok.length} (${Math.round(100 * ok.length / b.length)}%)`.padStart(8)}` +
                `  ${String(ok.filter(e => !e.big).length).padStart(11)}  ${String(ok.filter(e => e.big).length).padStart(8)}` +
                `  ${String(b.filter(e => e.last === 'parked').length).padStart(6)}` +
                `  ${(b.reduce((s, e) => s + (e.attempts || 0), 0) / b.length).toFixed(1).padStart(8)}` +
                `  ${big.length ? (big.reduce((s, e) => s + (e.bigSessions || 0), 0) / big.length).toFixed(1) : '-'}`.padStart(14));
  }
  ledgerStats(path.join(o.dir, 'ledger.jsonl'), edges, o.run ? String(o.run) : null);
  permuterStats(path.join(o.dir, 'permuter.jsonl'), edges, o.run ? String(o.run) : null);
  const cps = fs.existsSync(path.join(o.dir, 'checkpoints')) ? fs.readdirSync(path.join(o.dir, 'checkpoints')) : [];
  if (cps.length) {
    console.log('big-mode checkpoints (best C so far):');
    for (const n of cps) {
      const j = readJson(path.join(o.dir, 'checkpoints', n, 'best.json'), null);
      if (j) console.log(`  ${j.key && j.name ? `${j.name} (${j.key})` : n}: ${Swarm.describe(j)} (${j.updated})`);
    }
  }
}

// spend from ledger.jsonl by function size and mode: $ per match (everything spent in the bucket,
// failures included, over the functions it matched) and how often a first session matches. Use it
// to set --big-bytes and to compare prompt or tool changes between runs (--run ID for one run)
function ledgerStats(file, edges, run = null) {
  if (!fs.existsSync(file)) return console.log('no ledger.jsonl yet: spend per session is recorded from the next run');
  const rows = fs.readFileSync(file, 'utf8').split('\n').filter(Boolean).map(l => JSON.parse(l))
    .filter(r => !run || r.run === run);
  const sum = (a, f) => a.reduce((s, r) => s + (f(r) || 0), 0);
  const med = a => { const s = a.filter(x => x != null).sort((x, y) => x - y); return s.length ? s[s.length >> 1] : '-'; };
  const est = rows.filter(r => r.estimated);
  console.log(`\nspend by size and mode (${rows.length} sessions${run ? ` in run ${run}` : ''}, $${sum(rows, r => r.usd).toFixed(2)}; ` +
              `${est.length} killed sessions estimated from transcripts, $${sum(est, r => r.usd).toFixed(2)}` +
              `${rows.some(r => r.known === false) ? `; ${rows.filter(r => r.known === false).length} of unknown cost` : ''})`);
  console.log('  bytes        mode     sessions    spend  matched  $/match  1st session  med turns  docs reads/greps');
  for (let i = 0; i + 1 < edges.length; i++) for (const mode of ['normal', 'big']) {
    const b = rows.filter(r => r.size >= edges[i] && r.size < edges[i + 1] && r.kind.split('-')[0] === mode);
    if (!b.length) continue;
    const spend = sum(b, r => r.usd), matched = new Set(b.filter(r => r.outcome === 'matched').map(r => r.key)).size;
    const first = b.filter(r => r.attempt === 1 && r.kind === mode);
    const docs = b.filter(r => r.docReads != null);
    console.log(`  ${`${edges[i]}-${edges[i + 1] === Infinity ? '' : edges[i + 1] - 1}`.padEnd(11)}  ${mode.padEnd(7)}` +
                `  ${String(b.length).padStart(8)}  ${`$${spend.toFixed(2)}`.padStart(7)}  ${String(matched).padStart(7)}` +
                `  ${(matched ? `$${(spend / matched).toFixed(2)}` : '-').padStart(7)}` +
                `  ${(first.length ? `${first.filter(r => r.outcome === 'matched').length}/${first.length}` : '-').padStart(11)}` +
                `  ${String(med(b.map(r => r.turns))).padStart(9)}` +
                `  ${docs.length ? `${(sum(docs, r => r.docReads) / docs.length).toFixed(1)}/${(sum(docs, r => r.docGreps) / docs.length).toFixed(1)}` : '-'}`);
  }
}

// permuter runs from permuter.jsonl by size: how often one improves the score or matches, and
// its speed per job (the CPU side of --permuters / --permute-minutes)
function permuterStats(file, edges, run = null) {
  if (!fs.existsSync(file)) return console.log('\nno permuter.jsonl yet: permuter runs are recorded from the next run');
  const rows = fs.readFileSync(file, 'utf8').split('\n').filter(Boolean).map(l => JSON.parse(l))
    .filter(r => !run || r.run === run);
  const med = a => { const s = a.filter(x => x != null).sort((x, y) => x - y); return s.length ? s[s.length >> 1] : null; };
  console.log(`\npermuter runs by size (${rows.length}${run ? ` in run ${run}` : ''}; decomp.py permstats for more)`);
  console.log('  bytes        runs  cpu-min  improved  matched  med it/s/job');
  for (let i = 0; i + 1 < edges.length; i++) {
    const b = rows.filter(r => (r.size ?? 0) >= edges[i] && (r.size ?? 0) < edges[i + 1]);
    if (!b.length) continue;
    const cpu = b.reduce((s, r) => s + (r.seconds || 0) * (r.jobs || 1), 0) / 60;
    const imp = b.filter(r => r.base != null && r.best != null && r.best < r.base).length;
    const speed = med(b.filter(r => r.per_s).map(r => r.per_s / (r.jobs || 1)));
    console.log(`  ${`${edges[i]}-${edges[i + 1] === Infinity ? '' : edges[i + 1] - 1}`.padEnd(11)}  ${String(b.length).padStart(4)}` +
                `  ${cpu.toFixed(0).padStart(7)}  ${String(imp).padStart(8)}  ${String(b.filter(r => r.matched).length).padStart(7)}` +
                `  ${(speed == null ? '-' : speed.toFixed(1)).padStart(12)}`);
  }
}

// a markdown table line's cells; a `|` inside backticks or escaped (\|) is not a separator
function tableCells(line) {
  const cells = [];
  let cur = '', tick = false;
  for (let i = 0; i < line.length; i++) {
    const c = line[i];
    if (c === '\\' && line[i + 1] === '|') { cur += '\\|'; i++; continue; }
    if (c === '`') tick = !tick;
    if (c === '|' && !tick) { cells.push(cur.trim()); cur = ''; continue; }
    cur += c;
  }
  cells.push(cur.trim());
  return cells;
}

// fold: the after-run step 3. The rows workers added to patterns.md go to the end of the Diff
// patterns table in docs/MATCHING.md (pipes inside code escaped, so the table keeps 3 columns),
// and patterns.md is archived as patterns.folded-<date>.md so nothing is folded twice.
function fold(o) {
  const src = path.join(o.dir, 'patterns.md');
  if (!fs.existsSync(src)) return console.log(`no ${src}: nothing to fold`);
  const rows = [], bad = [];
  for (const line of fs.readFileSync(src, 'utf8').split('\n')) {
    const cells = tableCells(line.replace(/<!--.*?-->/g, '')).filter(Boolean);
    if (!cells.length) continue;
    // a worker's row can run into the next one on the same line: take the cells three at a time
    if (cells.length % 3) { bad.push(line); continue; }
    for (let i = 0; i < cells.length; i += 3) {
      const [sym, fix, ex] = cells.slice(i, i + 3).map(c => c.replace(/`[^`]*`/g, m => m.replace(/(?<!\\)\|/g, '\\|')));
      rows.push(`| ${sym} | ${fix} | ${/^`.*`$/.test(ex) ? ex : `\`${ex}\``} |`);
    }
  }
  const doc = path.join(REPO, 'docs', 'MATCHING.md');
  const text = fs.readFileSync(doc, 'utf8');
  const start = text.indexOf('## Diff patterns'), next = text.indexOf('\n## ', start + 1);
  if (start < 0 || next < 0) die('no Diff patterns section in docs/MATCHING.md');
  const lastRow = text.lastIndexOf('\n|', next);
  const at = text.indexOf('\n', lastRow + 1) + 1;
  const fresh = rows.filter(r => !text.includes(r));
  fs.writeFileSync(doc, text.slice(0, at) + fresh.map(r => r + '\n').join('') + text.slice(at));
  let arch = path.join(o.dir, `patterns.folded-${stamp().slice(0, 10)}.md`);
  for (let n = 0; fs.existsSync(arch); n++) arch = arch.replace(/(-\d{4}-\d\d-\d\d)[a-z]?\.md$/, `$1${'bcdefghij'[n]}.md`);
  fs.renameSync(src, arch);
  console.log(`folded ${fresh.length} row(s) into docs/MATCHING.md${rows.length > fresh.length ? ` (${rows.length - fresh.length} already there)` : ''}; ` +
              `patterns.md -> ${path.basename(arch)}`);
  for (const l of bad) console.log(`  not a 3-column row, left for you (in ${path.basename(arch)}): ${l.slice(0, 100)}`);
  console.log('Review the new rows (git diff docs/MATCHING.md), then commit them.');
}

// forget: a fix (tool, header, split) made some functions' failures out of date. Their attempt and
// big-session counts go back to 0, so runs lease them again without --retry-failed (which reopens
// every failure), and their notes are replaced by the reason, so no worker is told the old blocker.
function forget(o) {
  if (!o.only?.length) die('forget needs --only f1,f2,... (function names)');
  if (typeof o.why !== 'string' || !o.why.trim()) die('forget needs --why "what changed"');
  const lock = readJson(path.join(o.dir, 'lock'), null);
  if (lock && alive(lock.pid)) die('a run is active; stop it first');
  const file = path.join(o.dir, 'history.json');
  const h = readJson(file, {});
  for (const name of o.only) {
    // a name or a key; a name the slot overlays share needs the key
    const hits = Object.entries(h).filter(([k, v]) => k === name || v.name === name);
    if (hits.length > 1) { console.log(`${name}: ambiguous, give the key: ${hits.map(([k]) => k).join(', ')}`); continue; }
    if (!hits.length) { console.log(`${name}: no swarm history, nothing to reset`); continue; }
    const [key, e] = hits[0];
    const was = `${e.attempts || 0} attempts${e.big ? `, ${e.bigSessions || 0} big sessions` : ''}`;
    Object.assign(e, { attempts: 0, bigSessions: 0, bigStale: 0, last: 'reset',
                       notes: [`${stamp()} reset (was ${was}, last ${e.last || '-'}): ${o.why.trim()}`] });
    let cp = '';
    if (o.checkpoint) {
      for (const n of [key.replace(':', '_'), e.name]) {
        const d = path.join(o.dir, 'checkpoints', n);
        const was = readJson(path.join(d, 'best.json'), {}).key;   // a name dir: no key, or this one
        if (fs.existsSync(d) && (!was || was === key)) {
          fs.rmSync(d, { recursive: true, force: true });
          cp = ', checkpoint deleted';
        }
      }
    }
    console.log(`${name}: reset (was ${was})${cp}`);
  }
  writeJson(file, h);
}

function clean(o) {
  const lock = readJson(path.join(o.dir, 'lock'), null);
  if (lock && alive(lock.pid)) die('a run is active; stop it first');
  for (const d of ['workers', 'integ']) fs.rmSync(path.join(o.dir, d), { recursive: true, force: true });
  console.log(`removed the clones under ${o.dir} (logs, history and results kept)`);
}

// for tools/swarm/swarm.test.mjs, which imports this file with SWARM_IMPORT=1 (an env guard: a
// path comparison with argv[1] can misfire on Windows path case and keep `run` from starting)
export { Swarm, reapply, parseArgs, INCLUDE_LINE, infraError, workerMounts, sessionUsage, readTranscript, ledgerStats, readRuns, gateReason };

if (process.env.SWARM_IMPORT !== '1') {
  const [cmd, o] = parseArgs(process.argv.slice(2));
  o.baseGiven = process.argv.includes('--base');
  if (cmd === 'run') await new Swarm(o).run();
  else if (cmd === 'permute') await new Swarm(o).permuteOnly();
  else if (cmd === 'stop') await stop(o);
  else if (cmd === 'status') status(o);
  else if (cmd === 'stats') stats(o);
  else if (cmd === 'clean') clean(o);
  else if (cmd === 'forget') forget(o);
  else if (cmd === 'fold') fold(o);
  else console.log(fs.readFileSync(fileURLToPath(import.meta.url), 'utf8').split('\n').slice(1, 11).map(l => l.replace(/^\/\/ ?/, '')).join('\n'));
}
