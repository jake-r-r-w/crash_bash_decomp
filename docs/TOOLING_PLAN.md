# Tooling plan: swarm bug fixes and ideas from decomp.wtf

Written 2026-10-16 from a review of [decomp.wtf](https://github.com/jake-r-r-w/decomp.wtf)
(an MIT, now-discontinued app that runs AI agents on matching decomps) against our
`decomp.py` and `tools/swarm/swarm.mjs`. Line numbers are as of commit `18380a0`;
re-find them by the quoted code if they have moved.

Why this order: about 80% of the 857 KB left is in functions of 400 B and up,
which go to opus big mode, so big-mode spend dominates. decomp.wtf mostly makes
*small* functions cheap (batching, compact diffs, short sessions, free twin
matches). Its ideas help at the margins. The biggest wins are fixing swarm bugs
that will bite on the slot overlays, then measuring where the money goes.

Work in this order: **Part 1** (before any slot-overlay swarm run), **Part 2**
(telemetry), then **Part 3** as the numbers justify. Each item is a separate
commit after `tools/dk make check` (and `make test-tools`; see 1.0).

## Part 1: bugs to fix before the slot-overlay runs

**Done 2026-10-16** (commits ec41088..9cbd1eb). Deferred: recognising the CLI's
usage-limit result in 1.3. Runs are supervised, so today's handling (uncharged
failures, the run stops after three in a row) is enough; if a limit is ever hit,
keep that `worker.out` and match its result line then. The line numbers below
are from before those commits.

### 1.0 Swarm policy tests (do first, S)

Nothing tests `swarm.mjs`. A prototype of the suite below found bug 1.2.

- `swarm.mjs`: add `export { Swarm, reapply, parseArgs, INCLUDE_LINE };` before
  the CLI, and wrap the dispatch at the end (`:1667-1678`) in
  `if (process.env.SWARM_IMPORT !== '1') { ... }`. Use the env guard, not an
  `import.meta.url` vs `argv[1]` check (Windows path case can make that misfire
  and stop `run` from starting).
- New `tools/swarm/swarm.test.mjs` (`node:test`, no dependencies, temp `--dir`),
  run with `node --test tools/swarm/swarm.test.mjs` (an explicit file: Node 22
  refuses a directory and Windows shells don't expand globs). Cover:
  - `Swarm.better`: match beats all; paired, then blocks, then exact, then STAGE
    (the user's ranking); equal is not better.
  - Output contracts: `Swarm.score`/`regionScore` on real `STAGE` and `REGIONS`
    lines, and a test that the `decomp.py` f-strings they parse still exist
    (`STAGE` `decomp.py:553`, `REGIONS` `:1214-1216`, the permuter score line
    `:3365`, queue keys `:963-966`). A diff-output change that breaks these makes
    every big session score null and park after 2 sessions.
  - `checkPatch`, `reapply` (incl. the cases in 1.2), `nextJob` leasing,
    big-mode order and parking, the sonnet -> opus -> big escalation.
- `tools/scripts/tests/test_queue.py` for `pick_targets`, and a `func_def_regex`
  case for K&R definitions (see 1.7).
- Add `node --test tools/swarm/swarm.test.mjs` to `make test-tools` or document
  it next to it, and extend the CONTRIBUTING.md test rule to swarm policy
  changes.

### 1.1 Function names shared across slot overlays (S, highest priority)

All 14 slot overlays load at `0x800B32B4`, so 35 names (`func_800B3A38`, ...)
are shared by 72 functions, 63 unmatched (29.7 KB). The swarm passes the bare
name to every tool (`swarm.mjs:566,570,604,630,674,911,1034,1196`) and
`find_func` (`decomp.py:296-309`) dies `... is ambiguous - give SEG/NAME`. So
every one of them fails: the orchestrator's `try` rejects real matches, big
sessions score "C did not build". Checkpoints and results are also keyed by
name (`swarm.mjs:524-526,578,590,771,777-780,937-949,969,984-987,1340-1371`),
so two same-named functions share one checkpoint. History is already keyed by
`f.key`.

Fix:
- `cmd_queue` (`decomp.py:965`): add `"ref"`: the name when unique in
  functions.json, else the key (`ovl5d:800B3A38`; `find_func` accepts keys).
- `swarm.mjs`: `const ref = f.ref || f.name;` and use `ref` in every tool
  argument and prompt command; keep `f.name` where the C identifier is meant
  (prompt prose, `checkPatch`, `reapply`).
- Key checkpoint, results and permuter directories by `f.key.replace(':', '_')`
  (Windows paths reject `:`). Fall back to an existing name-keyed checkpoint
  when the name is unique, so today's main/engine checkpoints still resume.
- `find_func`: on ambiguity, pick the single hit whose INCLUDE_ASM line is gone
  (the function being worked on); accept `0x800B3A38`; on a miss print
  "did you mean" (`difflib.get_close_matches`).
- `--only` should accept keys (it already does at `swarm.mjs:443`).
- Tests: `find_func` cases in `tools/scripts/tests/`; a swarm test that two
  same-named jobs get different checkpoint dirs.

### 1.2 `reapply` drops header lines (S)

Reproduced. The fallback re-apply (`swarm.mjs:239-295`) skips any added line
that already appears anywhere in the target file (`:265`). A worker's new
`typedef struct New { s16 x; s16 y; s32 z; } New;` comes out as `{ s32 z; }`
when another struct already has `s16 x;`, and `reapply` still returns success.
A second, rarer case: added lines after a blank line that follows the replaced
INCLUDE_ASM line are inserted inside the new function body (the anchor is not
moved past the replacement).

Fix: dedupe only whole declarations (a full `typedef ... ;`/`struct {...};`
block, a prototype or an extern line keyed by its declared name, the way
`decl_name` does in decomp.py), never single member lines; move the anchor
past the inserted body. Both cases become tests in 1.0.

### 1.3 Docker lost mid-run burns the run (S)

`tools/dk` prints "Docker daemon not reachable" and exits 127 (`tools/dk:23-26`),
which isn't in the environment-error list (`swarm.mjs:33-41`, used only at
`:1207-1213`). Workers that can't build park with no patch, which goes to
`notMatched` (never counted by the 3-failures stop, `:1108`), so every function
climbs sonnet -> opus -> big until the time or cost cap, and dozens park wrongly.

Fix:
- Add tools/dk's messages and exit codes 125/126/127, and the Windows
  `PermissionError`, to the error list.
- After a session that ends with no patch, run `docker info`; if it fails,
  stop the run gracefully and record the function **uncharged**.
- Preflight (`:349-371`): also check `claude auth status`.
- A start failure (`:1463-1468`, `:1476-1481`) or a permuter exception should not
  charge an attempt.
- Recognise the CLI's usage-limit / sign-in results and stop uncharged, without
  requeueing. Capture one real usage-limit `worker.out` on the host first; don't
  guess the regex.
- Do not set the CLI's retry env vars (with the watchdog on, the usage-limit
  wait is infinite and workers hang until killed).

### 1.4 Checkpoint that no longer applies traps its function (S)

When a checkpoint can't be applied (rename, split, or a header change since),
`startJob` starts fresh and the prompt says "No session has worked on it yet"
(`swarm.mjs:672-674`), but the session is still judged against the old
checkpoint's score (`:969-978`). Two sessions that can't beat it park the
function (`bigStale` 2) and the checkpoint is dead; this only shows in
`swarm.log`.

Fix (about 30 lines): when the checkpoint didn't apply, give the worker the old
diff and its log as reference; let the first session that builds replace the
checkpoint whatever its score, and don't count it as stale; find the
INCLUDE_ASM line by function name rather than exact anchors; write the failure
to `report.md` and the run summary. (Full `decomp.py port` rebasing: 3.9.)

### 1.5 Escalation and notes lost across runs (S)

`nextJob` always leases `{model: sonnet, attempt: 1, notes: []}`
(`swarm.mjs:506-508`), even for a function an earlier run parked once, so it
goes sonnet -> opus again and ends above `maxAttempts`. Notes are recorded
(`:1079`) but never reach a later worker, including `forget`'s reason.

Fix: lease with `model = h.attempts ? escalateModel : model`,
`attempt = h.attempts + 1`, and the last 3 history notes tagged
`[earlier run]`. Mark a note stale (don't show it) when it predates the last
commit to the pipeline files (`dmpsx.py`, `aspsx_fill.py`, `project.mk`,
Makefile `PIPELINE_DEPS`): the old `$at` notes are known wrong.

### 1.6 Smaller fixes (S each)

- **Port sessions die on `next`.** Permuter-port and gate-fix sessions start
  with the earlier C applied (`swarm.mjs:785`), then are told to run
  `next --func` first (`:603-604`), which dies "is already matched"
  (`decomp.py:908-910`). Make `next` print the brief when the C is in place
  (don't consult state.json: 97 matched functions have no status there), or
  change those prompts.
- **Forbidden hint.** The 8-try hint (`decomp.py:996-997`) tells swarm workers
  to run `permute`/`block`, which they may not. Word it for both cases (check
  `SWARM_RESULT` in the environment).
- **Patterns file unfiltered.** Each worker's proposed Diff patterns row is
  appended (`swarm.mjs:888`) and shown to later workers (`:598`, `:662`); pin
  and empty-asm rows got through before. Drop rows mentioning `asm`, `__asm`,
  `register ... asm` or `--hack`; cap the tail at ~2,500 chars, same overlay
  first, no duplicates.
- **Inline asm refusals.** Neither prompt says inline asm is refused, while the
  docs suggest register pins (`MATCHING.md:193`). Add to both prompts: "Pure C
  only: no `asm`/`__asm__` of any kind and no register pins (the swarm refuses
  the patch). Use the `gte_*` macros already in `include/`. If only asm would
  match, finish \"parked\" and say why." In big mode a refused patch also throws
  away the session's C unmeasured (`:932`, `:970-978`): measure it anyway, and
  if it matches *only* with asm, list it in `report.md` as "needs a human
  `--hack`" instead of `fail()` (`func_8001AA48` was such a case).
- **Capped sessions stop runs.** A session that hits `--max-turns` or a budget
  cap ends with an error result; with no patch it goes to `fail()` (`:891`) and
  three in a row stop the run. Route capped sessions to `notMatched`.
- **Timeouts.** No timeout on the orchestrator's `try`/`regions`/`done`/`verify`
  calls (`:566,570,911,1132,1200`). Add one (e.g. 10 min) so a hung container
  can't freeze the run.
- **Cost line.** `AGENT_LOG.md` "about $0.12 each" doesn't match its totals
  ($21.72 / 45 = $0.48 a match). Fix the wording when updating the log.

### 1.7 Gate lint holes (S)

These pass both `checkPatch` (`swarm.mjs:866`) and the gate lint
(`decomp.py:1259-1282`): `__asm__ __volatile__(...)`, `__asm(...)`,
`asm __volatile__(...)`, `register x __asm("$20")`. Operand-only asm
(`asm("" ::: "memory")`) passes the gate lint (the regex at `:1262` requires a
non-empty template). And `func_def_regex` (`decomp.py:339`) misses K&R
definitions (5 matched functions, e.g. `main.c:3851`), so nothing inside them
is linted. Fix: one shared regex for every `asm` spelling (keep `checkPatch` as
a second check), handle K&R definitions, tests for each spelling. Nothing in
`src/` uses these spellings today.

### 1.8 Security: workers can run code on the host (S)

Workers may Edit/Write any path and run `Bash(tools/dk:*)` (`config.json:33-34`).
`tools/dk` is a host bash script inside the clone and mounts it read-write
(`tools/dk:78`), so a worker could rewrite `tools/dk` and run anything on the
Windows host. `useOurDk` (`swarm.mjs:227`, `:411`, `:800`) restores it only for
the orchestrator's calls. Fix: allow only `Edit(src/**)`, `Edit(include/**)`,
`Write(src/**)`, `Write(include/**)`, `Write(.swarm-result.json)` in the
allowlist, and restore `tools/dk` before every worker session too (or run the
worker's `tools/dk` from outside the clone).

### 1.9 propagate leaks includes (S)

`ensure_include` runs (`decomp.py:2602`) before `land` takes its backup
(`:1800`), so a failed copy leaves its `#include` lines. Also declare every name
the copied C uses that the target file can't see (the one open case,
ovl64:800BB52C, probably compiled silently as an implicit call to
`D_8009D73C` and mismatched: there are no `-W` flags), keep the failure stage
(`:2623` overwrites it), and print the twin's renamed C in `next`.

## Part 2: measure (S-M, before tuning anything)

No per-function cost exists, and big sessions killed at their deadline print
no JSON, so they count as $0 in the logs and in `--max-run-usd`
(`swarm.mjs:129-134,836-842,881-885,1279`). Every big-mode figure in
AGENT_LOG is a floor.

1. **Backfill from what exists (no code).** Each `results/*/*/worker.out` already
   has `num_turns`, `usage` and `duration_ms`/`duration_api_ms` for completed
   sessions (`swarm.mjs:771,829`); `swarm.log` has $, minutes and timeouts. A
   throwaway script gives $/match by size and mode, turns per session, and the
   share of wall-clock outside the model (the most any tool speed-up can save).
   Check whether `~/.claude/projects/*/` holds the worker transcripts and raise
   `cleanupPeriodDays` so they survive.
2. **Capture going forward.**
   - `config.json:31`: `--output-format stream-json --verbose`; pass
     `--session-id <uuid>` per job so a killed session's transcript can be found.
   - New `sessionUsage(file, prices)`: the result line if present, else sum
     usage deduplicated by `message.id` times a price table in `config.json`
     (scale it per run by completed sessions' reported vs estimated cost).
   - Add it into `costUsd` (fixes the cap) and write one ledger line per session
     (function key, kind, model, turns, tokens in/out/cache, $, minutes, tries,
     outcome). Count Read/Grep calls on `docs/*.md`.
   - Read `try` counts at the top of `onWorkerExit` (big mode also runs `try`
     through `measure()`).
3. **`stats`**: $/match and first-session match rate by size bucket and mode.
   Use it to set `--big-bytes` and to A/B the items below. Don't put $ in commit
   trailers (parked functions never commit).

## Part 3: ideas to implement

### 3.1 Slim the big-mode context (S, biggest token item)

`promptBig` tells opus to read `docs/MATCHING.md` (117 KB; too big for one Read,
so it pages it) (`swarm.mjs:676`) and ARCHITECTURE.md sections (`:706-711`), and
carries that through ~50 turns. The normal prompt sends workers to SKILL.md and
MATCHING.md (`:603-605`); clones load CLAUDE.md, which sends them to AGENT_LOG.

- Write the loop steps inline in both prompts (one shared constant in
  swarm.mjs, not a new markdown card): `next --func`, evidence (`similar`, the
  relatives `next` prints, grep `src/` for the same globals), STAGE order,
  owned header rule, `regions`/`vars`/`splitvars`, then "grep
  docs/MATCHING.md for the symptom (`grep -n -i WORD docs/MATCHING.md`); never
  Read it whole. Skip AGENT_LOG.md, CONTRIBUTING.md, CONVENTIONS.md".
- Replace the ARCHITECTURE.md compiler check with the one-line test (every
  global store `lui $at` + `sw`, or jump slots filled from a store, means gcc
  2.7.2), and only for `main/` functions.
- In resumed big sessions, skip the "Before matching code" checks (`:705-715`)
  that the checkpoint log shows were done.
- Never put MATCHING.md line numbers in prompts (each `fold` shifts them).
- Optional, measured by Part 2: run workers with
  `CLAUDE_CODE_DISABLE_AUTO_MEMORY=1`, `CLAUDE_CODE_DISABLE_GIT_INSTRUCTIONS=1`,
  `--disable-slash-commands`, TodoWrite/NotebookEdit in `disallowedTools`; move
  the fixed rules into `--append-system-prompt-file` so they cache across
  sessions. Only drop CLAUDE.md once its tools/dk lines are in the prompt.
- A/B on $/session and turns, not match rate (a match-rate A/B would cost
  $500+).

### 3.2 Carry the best C into retries (S)

Normal mode throws away the closest C at every escalation: the prompt says to
restore INCLUDE_ASM (`:630-632`), parks drop their edits (`:896`), the opus retry
starts from m2c, and `best_c` (`:637`) is read nowhere.

- Prompt: "If it does not match, leave your closest C in place of the
  INCLUDE_ASM line (not under `#ifndef NON_MATCHING`) if it compiles, and finish
  \"parked\"." Remove `best_c`.
- `onWorkerExit`: for a park with a patch, run `checkPatch` (errors mean "not
  kept", never `fail()`), `measure`, and keep it as the function's checkpoint if
  `Swarm.better`. Route parks through the normal `try` path so a leftover that
  actually matches reaches the gate.
- Also keep it in `runPermute` when the permuter finds nothing (`:1044-1051`):
  that is how most registers-stage near misses leave normal mode.
- `startJob`: resume normal jobs from a checkpoint too (with 1.5, across runs).
- The conflict retry (`:1191`) should pass `startPatch: job.patch`.

### 3.3 Overlay families: seed one, template the rest (S-M)

The slot overlays share code: 58 exact (fp) groups (132 functions) and 36 truly
near groups, plus 13 big shapes of 600 B+ shared across overlay families, all
unmatched. 41 of the 58 groups span overlays, so per-overlay runs rarely meet a
twin's lead.

- Step 0: `similar --survey [SEG]` to count, for each todo function, its best
  matched look-alike, using a SequenceMatcher `seq` column (Dice on 4-grams is
  too harsh on small functions). Calibrate the threshold on known template
  pairs (e.g. `func_8007CC80` -> `func_8007C338`). Fix the `similar` cache stamp
  to a hash of (key, asm, size, fp) instead of functions.json's mtime and copy
  `build/similar.json` into clones.
- Seeding: a `--seed-families` order (or a hand list via `--only`) that leases
  one member of each cross-overlay group first, then the rest.
- Template in the brief: when a matched look-alike scores high, `next` prints
  its C (renamed where `rename_syms` can, otherwise unrenamed with "use this
  function's asm names"), plus the types and owned header that C relies on.
  Record a `tmpl` flag in the queue so `stats` can measure it.
- Hold back a parked lead's exact twins instead of giving each its own ladder;
  under `--retry-failed`, lease at most one member per group per run. Key all
  group state by function key.
- Make `isBig` false for a function with a matched exact twin (it's an adapt
  job, not a big one).

### 3.4 A richer `next` brief (S)

- Print the whole compact asm (default `--asm-lines` 80 -> 400) with a hex
  offset on branch labels that lines up with `regions`; `decomp.py asm NAME
  --from 0xOFF` for the rest; render the function's own jump tables and strings
  inline.
- `m2c --from` so the cut draft (`decomp.py:3960`) isn't reprinted whole by the
  hint at `:956`.
- List callees with their prototypes; flag `void f(void)` prototypes "(check
  arity)" (126 call edges today). This is the mistake that retired `auto`.
- Later, if Part 2 shows lookup turns cost real money: a data block (globals
  with their declaration and target header per CONVENTIONS.md).

### 3.5 Diff output (S)

- `make diff`/`try`: aligned rows over the whole function built from
  `regions.align`/`score`, a one-line count by kind (named after MATCHING.md's
  "Classify the diff" lists), long runs collapsed. Keep the `DIFF` and `STAGE`
  lines byte-identical and STAGE last; fall back to `show_diff` on error. Use
  `reg_fields` (`decomp.py:1097-1102`) for register classification.
- Cap the `regions` block table (22-27 rows on big functions, re-run about once a
  minute by opus), or add `regions --changed` that prints only blocks whose
  status changed since the last call.
- Optional `try --regions` so `measure()` uses one container and one compile
  instead of two.

### 3.6 Cheap speed fixes (S, measure first with Part 2)

- `tools/dk` does five checks and starts a fresh container per call
  (`tools/dk:12-79`). Time `tools/dk true` vs `docker exec true` on the host.
  If the overhead is ~1.5 s+ per call: one labelled long-lived container per
  slot with a `DK_CONTAINER` fast path (`docker exec`), recreated by the swarm.
- `make diff` parses the Makefile twice, each walking ~3,100 `.s` files
  (`Makefile:55-58`, `decomp.py:532`): cache the flags on disk, make the `find`
  skip `asm/nonmatchings`.
- Run `regions`' two compiles (normal and `-g`) in parallel.
- Read the symbol files once per command.
- Bigger, only if timings justify it: asm/ on a Docker named volume per slot, or
  a function-only compile confirmed by the whole-file build before printing
  MATCH. Running the whole swarm inside WSL is the biggest win but reverses the
  run-on-the-Windows-host decision: the user's call.

### 3.7 Gate feedback (S)

- After a code MATCH, `try` runs the real incremental `make build/<bin>` and
  prints `LINK ok` or `LINK differs at <function or rodata label> +0xOFF`
  (`rodata_syms` already maps offsets). Keep printing MATCH and leave the swarm's
  routing alone (a separate "CODE-MATCH" status would send the job to
  `notMatched`, which drops the C). Zero-code alternative: one prompt line,
  "run `decomp.py verify` once after your first MATCH".
- When `done`'s full build fails, print target vs built disassembly at the
  first difference, so gate-fix sessions aren't blind.
- Optional, for template work: compare the target symbol of each `%lo`
  relocation in `try` (the LO16 check needs no HI16 pairing). Must report 0
  findings over all matched functions before it can block anything.

### 3.8 Permuter (S)

- `permuter_base` (`decomp.py:599-606`) runs cpp itself, so GTE macros come back
  as inline asm and the lint refuses them: keep `gte_*` and project macros as
  calls (decomp-permuter's preserve-macros support). Pin decomp-permuter in the
  Dockerfile (it is cloned unpinned).
- In-run permuter match: apply it with `--apply-best`, tidy it to project style,
  recompile to prove it, `checkPatch`, and push it to the integrator queue
  instead of spending a Claude session porting it. Fall back to today's port
  path on any failure. Count first:
  `grep -c 'permuter: score .* -> 0 (MATCH)' <repo>-swarm/report.md`.
- Later, when registers-stage checkpoints pile up: use idle permuter slots on
  them in short rotating turns (decomp.wtf: ~60 s turns, max 4 per version).

### 3.9 Later, after Part 2 numbers

- **Batch small neighbours.** Simplest form: session chaining. After a small job
  exits, start a neighbour in the same file and slot with
  `claude -p --resume <session_id>`, max 3, carrying header changes; everything
  else unchanged. Worth more in fresh overlays, where parallel workers invent the
  same structs. Pilot by alternating batched and single jobs.
- **Stall nudge.** A size-scaled "no better in N tries" line in `try`, ranking
  tries by `regions.score`, logged under `build/attempts/<key>` (cleared in
  `startJob`). Normal mode only.
- **Model tiers.** A no-code pilot (`--model haiku --escalate-model sonnet
  --big-workers 0` on 40 functions under 128 B; review the first matches by hand
  for weak types), and an A/B of `--big-model sonnet`. `bigMinutes` defaults to
  150 in config.json but runs use 27: align the default.
- **Checkpoint rebasing.** `decomp.py port NAME DIFF`: re-apply an old checkpoint
  after renames and splits (rename placeholder tokens by address, find
  INCLUDE_ASM by name, send declarations through `declare()`).
- **Mis-split scan.** `decomp.py splitcheck`: a second `addiu $sp,$sp,-N` after
  the first instruction, or unreferenced code after an unconditional jump,
  flags two merged functions (like `func_80027408`); print the `// type:func`
  line to add.
- **Types without a match.** A by-hand typing pass with
  `tools/scripts/field_survey.py` on the overlays with no struct coverage
  (ovl64, ovl65, ovl67 have enums only).

## Not worth bringing

- Autopilot that commits m2c drafts: that is the retired `decomp.py auto`.
- decomp.wtf's own permuter: weaker than decomp-permuter, which we ship.
- Its PSX compile pipeline (no maspsx), the GTE-as-handwritten-asm rule,
  collab rooms, installers, GameCube/DS linking, its SQLite store.

decomp.wtf's author discontinued it out of respect for the decomp community's
objections to AI decompiling, and asks users to work only on games they own,
not share the output, and respect each project's rules on AI contributions.
