#!/usr/bin/env node
// Swarm orchestrator: N Claude workers match functions in parallel, one serial integrator
// commits each through the validation gate (decomp.py done). See tools/swarm/README.md.
//
//   node tools/swarm/swarm.mjs run [--workers 3] [--seg main/main] [--only f1,f2] ...
//   node tools/swarm/swarm.mjs stop [--now]     graceful: finish and integrate current work
//   node tools/swarm/swarm.mjs status
//   node tools/swarm/swarm.mjs clean            delete the worker and integration clones
//
// Runs on the host (Node, no dependencies): it launches `claude` and `tools/dk`, which can't
// run inside the toolchain container.
import { spawn, spawnSync } from 'node:child_process';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const REPO = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..', '..');
const DEFAULTS = JSON.parse(fs.readFileSync(path.join(REPO, 'tools/swarm/config.json'), 'utf8'));
const WIN = process.platform === 'win32';
const LABEL = 'crashbash-swarm';
// game data and tool downloads are git-ignored: each clone gets a copy
const DATA_DIRS = ['asm', 'assets', 'linkers', 'ext'];
const DISC_FILE_MAX = 16 << 20;           // disc/: the exe and overlay binaries, not the 170 MB image
const RESULT = '.swarm-result.json';
const PATCH_MAX = 64 << 10;
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
  // a branch or HEAD fetches; a bare commit id may not, but the clone usually has it already
  let target = 'FETCH_HEAD';
  if (git(dir, 'fetch', '-q', src, rev).code) {
    git(dir, 'fetch', '-q', src);
    target = rev;
  }
  gitOk(dir, 'checkout', '-q', '-f', '--detach', target);
  if (fs.existsSync(path.join(dir, '.git', 'swarm-data'))) return;
  log(`copying game data into ${path.basename(dir)}`);
  for (const d of DATA_DIRS)
    if (fs.existsSync(path.join(REPO, d))) fs.cpSync(path.join(REPO, d), path.join(dir, d), { recursive: true });
  fs.mkdirSync(path.join(dir, 'disc'), { recursive: true });
  for (const f of fs.readdirSync(path.join(REPO, 'disc'))) {
    const p = path.join(REPO, 'disc', f), s = fs.statSync(p);
    if (s.isFile() && s.size <= DISC_FILE_MAX) fs.copyFileSync(p, path.join(dir, 'disc', f));
  }
  const drafts = path.join(REPO, 'build', 'auto_drafts');
  if (fs.existsSync(drafts)) fs.cpSync(drafts, path.join(dir, 'build', 'auto_drafts'), { recursive: true });
  fs.writeFileSync(path.join(dir, '.git', 'swarm-data'), stamp() + '\n');
}

// this checkout's tools/dk (labels, --cpus) in a clone; never committed (done commits src/ include/
// configs/ .decomp/ docs/), and every reset puts the clone's own back
function useOurDk(dir) {
  fs.copyFileSync(path.join(REPO, 'tools', 'dk'), path.join(dir, 'tools', 'dk'));
}

// back to `rev`, dropping every tracked change and untracked file (ignored data stays)
function resetClone(dir, rev) {
  gitOk(dir, 'reset', '-q', '--hard', rev);
  gitOk(dir, 'clean', '-fdq');
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
      results: path.join(o.dir, 'results'),
    };
    this.slots = Array.from({ length: o.workers }, (_, i) => ({ id: `w${i + 1}`, dir: path.join(o.dir, 'workers', `w${i + 1}`), job: null }));
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
    this.consecutiveFailures = 0;
    this.stopping = null;
    this.history = readJson(this.paths.history, {});
    // half the cores for workers' builds, half for the permuters (all to workers without a pool)
    const ncpu = os.cpus().length, share = o.permuters ? ncpu / 2 : ncpu;
    this.cpus = Math.max(1, Math.floor(share / o.workers));
    this.permCpus = Math.max(1, Math.floor(share / Math.max(1, o.permuters)));
  }

  log(msg) {
    const line = `${stamp()} ${msg}`;
    console.log(line);
    fs.appendFileSync(this.paths.log, line + '\n');
  }

  // ---- safeguards before anything starts
  preflight() {
    const o = this.o;
    if (!(o.workers >= 1 && o.workers <= 16)) die('--workers must be 1..16');
    // commits go to <remote>/<branch> (GitHub main by default), never into your checkout directly
    const named = git(REPO, 'remote', 'get-url', o.remote);
    this.remoteUrl = named.code === 0 ? named.out.trim() : o.remote;
    this.remoteName = named.code === 0 ? o.remote : null;
    if (git(REPO, 'ls-remote', '-q', this.remoteUrl).code) die(`can't reach remote ${o.remote} (${this.remoteUrl})`);
    if (sh('docker', ['info']).code) die('Docker is not reachable');
    if (!o.workerCmd && !o.dryRun) {
      this.claude = realClaude(which(o.claudeBin));
      if (!this.claude) die(`${o.claudeBin} not found on PATH (install Claude Code or pass --claude-bin)`);
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
        ...process.env, MSYS_NO_PATHCONV: '1',
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
      p.on('close', code => { clearTimeout(timer); this.children.delete(p); resolve({ code: code ?? -1, out, timedOut }); });
      p.on('error', err => { clearTimeout(timer); this.children.delete(p); resolve({ code: -1, out: String(err), timedOut }); });
    });
  }

  async refreshQueue() {
    // this checkout's decomp.py (the integration branch may predate `queue`); it only reads
    fs.copyFileSync(path.join(REPO, 'tools/scripts/decomp.py'), path.join(this.integ, '.swarm-queue.py'));
    const args = ['python3', '.swarm-queue.py', 'queue', '--count', this.o.only ? '5000' : '300'];
    if (this.o.seg && !this.o.only) args.push('--seg', this.o.seg);
    const r = await this.dk(this.integ, args);
    const line = r.out.trim().split('\n').reverse().find(l => l.startsWith('['));
    if (r.code || !line) throw new Error('decomp.py queue failed:\n' + r.out.slice(-2000));
    let q = JSON.parse(line);
    if (this.o.only) q = q.filter(f => this.o.only.includes(f.name) || this.o.only.includes(f.key));
    this.queue = q;
  }

  activeJobs() {
    return [...[...this.slots, ...this.pslots].map(s => s.job).filter(Boolean), ...this.permQueue,
            ...this.integQueue, ...(this.integrating || [])];
  }

  nextJob(peek = false) {
    if (this.retry.length) return peek ? this.retry[0] : this.retry.shift();
    const busy = this.activeJobs();
    const keys = new Set(busy.map(j => j.f.key)), fps = new Set(busy.map(j => j.f.fp).filter(Boolean));
    for (const f of this.queue) {
      if (this.finished.has(f.key) || keys.has(f.key) || (f.fp && fps.has(f.fp))) continue;
      if ((this.history[f.key]?.attempts || 0) >= this.o.maxAttempts && !this.o.retryFailed) continue;
      return { f, model: this.o.model, attempt: 1, notes: [] };
    }
    return null;
  }

  // ---- workers
  prompt(job) {
    const { f } = job;
    const canPermute = this.pslots.length > 0 && !job.permuted;
    const patterns = fs.existsSync(this.paths.patterns)
      ? fs.readFileSync(this.paths.patterns, 'utf8').trim().split('\n').slice(-30).join('\n') : '';
    return `You are one worker in a swarm matching Crash Bash functions. Match exactly one function:
${f.name} (${f.seg}, ${f.size} bytes, defined in ${f.src}).

Follow .claude/skills/match-function/SKILL.md and docs/MATCHING.md, steps 1 to 4 only (pick with
\`tools/dk python3 tools/scripts/decomp.py next --func ${f.name}\`). These swarm rules override the
skill, CLAUDE.md and the docs:
- Work only on ${f.name}. Change only its INCLUDE_ASM line in ${f.src}, and headers under include/
  when it needs a declaration or a type. Never touch another function's INCLUDE_ASM line.
- New structs, typedefs and global declarations go in the owned header of ${f.src} (the
  include/bash/*.h marked \`owner: ${f.src}\`), never in the .c file: the gate adds the function's
  prototype to that header, and it won't compile if a type in it is only declared in the .c.
- Do not run \`decomp.py done\`, \`decomp.py block\`, \`make clean\`, \`make verify\` or any git command
  that changes the repo, and do not edit docs/, .decomp/, configs/ or tools/. The orchestrator
  validates and commits your work.
- Don't run the permuter (it is refused). ${canPermute
    ? `If the evidence step, the Diff patterns, the checklist and (for register diffs) the cc1 dump
  don't get you to a match, leave your closest C in place of the INCLUDE_ASM line and finish with
  status "needs_permute": the orchestrator runs the permuter in the background and gives its result
  to a worker, so you free your slot instead of waiting on it.`
    : 'This function has had its permuter run already.'}
- Tools: change files only with the Edit and Write tools (shell edits such as \`sed -i\`, \`>\` or
  \`tee\` are refused). Run each \`tools/dk ... </dev/null\` as its own Bash command, not chained
  with \`&&\` or \`;\` (a chain is refused if any part isn't allowed). Read-only shell commands
  (grep, sed -n, head, ...) are fine. If a command is refused, do the same thing another way;
  a refusal is never a reason to stop.
- You have about ${this.o.workerMinutes} minutes.
- Stop when \`tools/dk make diff FUNC=${f.name}\` prints MATCH, or when you are out of ideas.
  If it does not match${canPermute ? ' and you are not asking for the permuter' : ''}, put the
  INCLUDE_ASM line back exactly as it was and leave no changes in src/ or include/.
- Last, write ${RESULT} in the repo root, JSON:
  {"status": "matched" | "parked"${canPermute ? ' | "needs_permute"' : ''},
   "note": "one line: what made it match, or what still differs",
   "pattern": "a new Diff patterns table row (| symptom | change | example |), or empty",
   "best_c": "your closest C when parked, else empty"}
${job.gateFix ? `
An earlier worker's C for ${f.name} is already in ${f.src} and matches the bytes, but the validation
gate (decomp.py done: scope, lint, full build) refused it with:
\`\`\`
${job.gateFix}
\`\`\`
Fix that (often a type declared in the .c instead of the owned header, or a lint finding), keep the
function matching (make diff), and report "matched".
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

  startJob(slot, job) {
    const { f } = job;
    job.base = gitOk(this.integ, 'rev-parse', 'HEAD');
    job.dir = path.join(this.paths.results, f.name, `attempt${job.attempt}${job.gateFix ? '-gatefix' : job.startPatch ? '-port' : ''}-${this.id}`);
    fs.mkdirSync(job.dir, { recursive: true });
    makeClone(slot.dir, this.integ, 'HEAD', m => this.log(m));   // HEAD may have moved on: base is in it
    resetClone(slot.dir, job.base);
    // a follow-up after the permuter starts from the earlier worker's C
    if (job.startPatch) {
      gitOk(slot.dir, 'apply', '--3way', '--whitespace=nowarn', job.startPatch);
      gitOk(slot.dir, 'reset', '-q');   // --3way stages it: unstaged, `git checkout -- src` undoes it
    }
    fs.rmSync(path.join(slot.dir, RESULT), { force: true });
    useOurDk(slot.dir);
    const prompt = this.prompt(job);
    fs.writeFileSync(path.join(job.dir, 'prompt.txt'), prompt);
    slot.job = job;
    if (!job.retry) this.leases++;
    job.started = Date.now();

    const env = Object.fromEntries(Object.entries(process.env).filter(([k]) => !STRIP_ENV.test(k)));
    Object.assign(env, {
      DK_DOCKER_ARGS: `--label ${LABEL}=${this.id} --label ${LABEL}-slot=${this.id}-${slot.id} --cpus ${this.cpus}`,
      SWARM_FUNC: f.name, SWARM_KEY: f.key, SWARM_SRC: f.src || '', SWARM_MODEL: job.model, SWARM_RESULT: RESULT,
    });
    let cmd, args, shell = false;
    if (this.o.workerCmd) {
      [cmd, args] = [this.bash, ['-c', this.o.workerCmd]];
    } else if (this.o.dryRun) {
      [cmd, args] = [this.bash, ['-c', `echo '{"status":"parked","note":"dry run"}' > ${RESULT}`]];
    } else {
      cmd = this.claude;
      args = [...this.o.claudeArgs, '--model', job.model,
              '--max-budget-usd', String(this.o.workerBudgetUsd),
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
    job.timer = setTimeout(() => {
      this.log(`${slot.id} ${f.name}: over ${this.o.workerMinutes} min, killing it`);
      job.timedOut = true;
      killTree(p.pid);
      dockerKill(`${LABEL}-slot=${this.id}-${slot.id}`, slot.dir);
    }, this.o.workerMinutes * 60e3);
    p.on('close', code => {
      this.children.delete(p);
      clearTimeout(job.timer);
      this.onWorkerExit(slot, job, code).catch(e => this.fail(slot, job, `orchestrator error: ${e.message}`));
    });
    p.on('error', err => this.log(`${slot.id} ${f.name}: could not start worker: ${err.message}`));
    this.log(`${slot.id} start ${f.name} (${f.size} bytes, ${job.model}, attempt ${job.attempt})`);
  }

  // which INCLUDE_ASM lines a patch removes and adds, and the paths it touches
  checkPatch(patch, f) {
    const errs = [];
    if (patch.length > PATCH_MAX) errs.push(`patch is ${patch.length} bytes`);
    const files = [...patch.matchAll(/^diff --git a\/(\S+) b\/(\S+)/gm)].map(m => m[2]);
    for (const p of files) if (!/^(src|include)\//.test(p)) errs.push(`touches ${p}`);
    const inc = /^([-+])\s*INCLUDE_ASM\(\s*"[^"]+"\s*,\s*(\w+)\s*\)/gm;
    const removed = [], added = [];
    for (const m of patch.matchAll(inc)) (m[1] === '-' ? removed : added).push(m[2]);
    if (added.length) errs.push(`adds INCLUDE_ASM for ${added.join(', ')}`);
    if (removed.length !== 1 || ![f.name, `func_${f.key}`].includes(removed[0]))
      errs.push(`replaces INCLUDE_ASM for [${removed.join(', ')}], expected only ${f.name}`);
    if (/^\+.*\b(?:asm|__asm__)\s*(?:volatile\s*)?\(/m.test(patch)) errs.push('adds inline asm (needs a human --hack)');
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
    const cost = Number(session.total_cost_usd) || 0;
    this.costUsd += cost;
    this.log(`${slot.id} ${f.name}: worker exited ${code}${job.timedOut ? ' (timeout)' : ''} after ${mins} min` +
             `${cost ? `, $${cost.toFixed(2)} (run $${this.costUsd.toFixed(2)})` : ''}, says ${res.status || 'nothing'}`);
    if (res.pattern) fs.appendFileSync(this.paths.patterns, `${res.pattern.trim()}  <!-- ${f.name} -->\n`);
    if (this.stopping === 'now') return;
    // a session that never got going (not logged in, API down) is a failure, not a park
    if (session.is_error && !patch.trim() && !res.status)
      return this.fail(slot, job, `claude: ${String(session.result || session.terminal_reason).slice(0, 200)}`);
    if (!patch.trim()) return this.notMatched(slot, job, res.note || (job.timedOut ? 'timed out' : 'no change'));
    // a park that left edits behind (a header type it added): a park, not a bad patch
    if (res.status === 'parked') return this.notMatched(slot, job, `${res.note || 'parked'} (its leftover edits dropped)`);
    fs.writeFileSync(path.join(job.dir, 'patch.diff'), patch);
    const errs = this.checkPatch(patch, f);
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
    // don't trust the worker's word: build and compare the function ourselves
    const r = await this.dk(slot.dir, ['python3', 'tools/scripts/decomp.py', 'try', f.name], slot.id);
    fs.writeFileSync(path.join(job.dir, 'try.txt'), r.out);
    if (r.code !== 0 || !/^MATCH /m.test(r.out))
      return this.notMatched(slot, job, res.status === 'matched'
        ? `claimed a match that try rejects; ${res.note || ''}`.trim()
        : `${res.note || 'not matched'} (left its C in place)`);
    job.note = res.note || '';
    job.patch = path.join(job.dir, 'patch.diff');
    slot.job = null;
    this.integQueue.push(job);
    this.log(`${slot.id} ${f.name}: try says MATCH, queued for integration`);
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
      this.log(`${ps.id} permuting ${f.name} for ${this.o.permuteMinutes} min (${this.permCpus} cpus)`);
      const r = await this.dk(ps.dir, ['make', 'permute', `FUNC=${f.name}`, `MINUTES=${this.o.permuteMinutes}`], ps.id,
                              { cpus: this.permCpus, minutes: this.o.permuteMinutes + 20 });
      fs.writeFileSync(path.join(job.dir, 'permute.txt'), r.out);
      if (this.stopping) {          // a stop cuts permuter runs short: no worker would port the result
        this.finished.add(f.key);
        this.log(`${ps.id} ${f.name}: permuter stopped; worker's C kept in ${job.bestPatch}`);
        return;
      }
      const m = r.out.match(/permuter best score (\d+)(?: \(base (\d+)\))?: (\S+)\/source\.c/);
      const best = m ? +m[1] : null, base = m && m[2] !== undefined ? +m[2] : null;
      if (!m || (base !== null && best >= base)) {
        const why = r.timedOut ? 'permuter timed out' : 'permuter found no improvement';
        return this.notMatched(ps, job, `${why}; worker: ${job.note}`);
      }
      const outDir = path.join(ps.dir, m[3]);
      let diff = fs.existsSync(path.join(outDir, 'diff.txt')) ? fs.readFileSync(path.join(outDir, 'diff.txt'), 'utf8') : '';
      if (diff.length > 6000) diff = diff.slice(0, 6000) + '\n... (cut)';
      fs.writeFileSync(path.join(job.dir, 'permuter-diff.txt'), diff);
      if (fs.existsSync(path.join(outDir, 'source.c'))) fs.copyFileSync(path.join(outDir, 'source.c'), path.join(job.dir, 'permuter-source.c'));
      const note = `permuter: score ${base ?? '?'} -> ${best}${best === 0 ? ' (MATCH)' : ''}`;
      this.record(job, 'permuted', note);
      this.log(`${ps.id} ${f.name}: ${note}; back in the queue for a worker to port`);
      ps.job = null;
      if (!this.stopping)
        this.retry.unshift({ f, model: job.model, attempt: job.attempt, retry: true, permuted: true,
                             notes: [...job.notes, `${job.note} (${note})`], startPatch: job.bestPatch,
                             permuterDiff: diff || '(no diff.txt)', permuterScore: { base: base ?? '?', best } });
      else this.finished.add(f.key);
    } catch (e) {
      this.fail(ps, job, `permuter: ${e.message}`);
    } finally {
      ps.job = null;
    }
  }

  record(job, status, note) {
    const h = this.history[job.f.key] ||= { name: job.f.name, attempts: 0, notes: [] };
    if (!['conflict', 'permuted', 'gate-refused'].includes(status)) h.attempts++;
    h.last = status;
    h.notes = [...h.notes, `${stamp()} ${job.model}: ${status}: ${note}`].slice(-6);
    writeJson(this.paths.history, this.history);
    fs.appendFileSync(this.paths.report, `- ${stamp()} **${job.f.name}** (${job.f.seg}, ${job.f.size} B, ${job.model}): ${status}. ${note}\n`);
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
    this.stats.parked++;
    this.finished.add(job.f.key);
    this.log(`${job.f.name}: parked: ${note}`);
  }

  fail(slot, job, why) {
    if (slot) slot.job = null;
    this.stats.failed++;
    this.consecutiveFailures++;
    this.finished.add(job.f.key);
    this.record(job, 'failed', why);
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
        const v = await this.dk(this.integ, ['python3', 'tools/scripts/decomp.py', 'verify']);
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
          this.log(`gate: ${this.o.branch} moved on while the gate ran; redoing ${passed.length} on the new tip`);
          this.integQueue.unshift(...passed);
          return;
        }
        this.stopping ||= 'graceful';
        for (const job of passed) this.fail(null, job, `push to ${this.o.remote}/${this.o.branch} failed, stopping: ${push.err.trim()}`);
        return;
      }
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
    const ap = git(this.integ, 'apply', '--3way', '--whitespace=nowarn', job.patch);
    if (ap.code) {
      resetClone(this.integ, 'HEAD');
      this.stats.conflicts++;
      this.record(job, 'conflict', (ap.err || ap.out).trim().split('\n').slice(-2).join(' '));
      if (job.conflicts++ < 1 && !this.stopping) {
        this.log(`${f.name}: patch conflicts with newer commits; it goes back in the queue`);
        this.retry.push({ f, model: job.model, attempt: job.attempt, notes: job.notes, retry: true, conflicts: job.conflicts });
      } else this.fail(null, job, 'patch conflicts twice');
      return false;
    }
    const trailers = [...this.o.trailers, `Swarm-Run: ${this.id} (${job.model})`];
    const args = ['python3', 'tools/scripts/decomp.py', 'done', f.name, '--commit',
                  '--note', job.note || 'matched by a swarm worker', ...trailers.flatMap(t => ['--trailer', t]),
                  ...(defer ? ['--defer-build'] : [])];
    const before = gitOk(this.integ, 'rev-parse', 'HEAD');
    const r = await this.dk(this.integ, args);
    fs.writeFileSync(path.join(job.dir, 'done.txt'), r.out);
    const after = gitOk(this.integ, 'rev-parse', 'HEAD');
    if (this.stopping === 'now') return false;
    if (r.code !== 0 || after === before) {
      resetClone(this.integ, before);
      const why = r.out.trim().split('\n').slice(-6).join('\n');
      // the bytes matched (try said so): what's left is usually fixable (a header, lint), once
      if (!job.gateFix && !this.stopping) {
        this.record(job, 'gate-refused', why.replace(/\n/g, ' '));
        this.log(`${f.name}: done refused it; back to a worker to fix: ${why.split('\n').pop()}`);
        this.retry.unshift({ f, model: job.model, attempt: job.attempt, retry: true, permuted: job.permuted,
                             notes: job.notes, startPatch: job.patch, gateFix: why });
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

  // --resume RUN: matches an earlier run verified with `try` but stopped before gating go straight
  // to the gate (still unmatched ones only: the queue lists those)
  resumeFrom(runId) {
    const byName = new Map(this.queue.map(f => [f.name, f]));
    let n = 0;
    for (const name of fs.existsSync(this.paths.results) ? fs.readdirSync(this.paths.results) : [])
      for (const d of fs.readdirSync(path.join(this.paths.results, name)).filter(d => d.endsWith(`-${runId}`))) {
        const dir = path.join(this.paths.results, name, d), f = byName.get(name);
        const tried = path.join(dir, 'try.txt');
        // not in the queue = matched already; done.txt alone proves nothing (a stop may cut a batch)
        if (!f || !fs.existsSync(tried) ||
            !/^MATCH /m.test(fs.readFileSync(tried, 'utf8'))) continue;
        const res = readJson(path.join(dir, 'result.json'), {});
        this.integQueue.push({ f, model: 'resumed', attempt: 1, notes: [], conflicts: 0, dir,
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
    for (const s of this.slots) if (s.job) clearTimeout(s.job.timer);
    for (const p of this.children) killTree(p.pid);
    const n = dockerKill(`${LABEL}=${this.id}`, this.o.dir);
    if (n) this.log(`killed ${n} container(s)`);
    try { resetClone(this.integ, 'HEAD'); } catch { /* best effort */ }
  }

  writeStatus(state = 'running') {
    writeJson(this.paths.status, {
      id: this.id, pid: process.pid, state, stopping: this.stopping, started: new Date(this.started).toISOString(),
      branch: this.o.branch, stats: this.stats, costUsd: +this.costUsd.toFixed(2), leases: this.leases, queued: this.queue.length,
      workers: this.slots.map(s => ({ slot: s.id, func: s.job?.f.name || null, model: s.job?.model || null, pid: s.job?.pid || null,
                                      minutes: s.job ? +((Date.now() - s.job.started) / 60e3).toFixed(1) : null })),
      permuters: this.pslots.map(s => ({ slot: s.id, func: s.job?.f.name || null })),
      permuteWaiting: this.permQueue.map(j => j.f.name),
      integrating: this.integrating ? this.integrating.map(j => j.f.name).join(', ') : null, waiting: this.integQueue.map(j => j.f.name),
    });
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
      if (this.o.resume) this.resumeFrom(String(this.o.resume));
      this.log(`run ${this.id}: ${this.o.workers} workers, ${this.o.permuters} permuters, ${this.queue.length} candidates` +
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
            const job = this.nextJob();
            if (!job) break;
            job.conflicts ||= 0;
            try {
              this.startJob(slot, job);
            } catch (e) {
              slot.job = null;
              this.fail(null, job, `could not start: ${e.message}`);
            }
          }
        const idle = !this.activeJobs().length;
        const more = this.retry.length || (this.leases < this.o.maxFunctions && this.nextJob(true));
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

function clean(o) {
  const lock = readJson(path.join(o.dir, 'lock'), null);
  if (lock && alive(lock.pid)) die('a run is active; stop it first');
  for (const d of ['workers', 'integ']) fs.rmSync(path.join(o.dir, d), { recursive: true, force: true });
  console.log(`removed the clones under ${o.dir} (logs, history and results kept)`);
}

const [cmd, o] = parseArgs(process.argv.slice(2));
o.baseGiven = process.argv.includes('--base');
if (cmd === 'run') await new Swarm(o).run();
else if (cmd === 'stop') await stop(o);
else if (cmd === 'status') status(o);
else if (cmd === 'clean') clean(o);
else console.log(fs.readFileSync(fileURLToPath(import.meta.url), 'utf8').split('\n').slice(1, 11).map(l => l.replace(/^\/\/ ?/, '')).join('\n'));
