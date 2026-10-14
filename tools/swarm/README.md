# Swarm: parallel matching

`swarm.mjs` runs several Claude Code workers at once, each matching one function in its own
clone, and a single integrator that commits every match through the validation gate
(`decomp.py done --commit`). It runs on the host with Node (no dependencies), because it starts
`claude` and `tools/dk`.

```
workers (N clones)                         integrator (one clone, serial)
 reset to the integration tip               git apply --3way the patch
 claude -p: match-function steps 1-4  -->   decomp.py done NAME --commit
 orchestrator: patch checks + `try`         push to <remote>/<branch>
        |
        | stuck: "needs_permute" + closest C (the worker's slot is freed)
        v
permuter pool (M clones, no Claude)
 make permute on that C; if the score improves, the function goes back to the
 front of the queue and the next worker starts from that C plus the permuter's diff
```

Workers can't run the permuter themselves: a 10-minute permuter run would hold a Claude slot
idle. Each function gets one permuter run.

## Big mode

Normal workers get one short attempt per model. That matches 93-97% of functions under 400
bytes but only about half from 400 B up, so big ones go to big-mode workers (`b1`, ...):
opus sessions that work block by block with `decomp.py regions` and resume from a checkpoint.
Most functions up to ~1200 B match in one session of 2-15 minutes (13 of 15 on 2026-10-14);
the rest take several, each building on the best C so far:

```
big function (>= --big-bytes, or parked in normal mode)
 session n: claude (opus, up to --big-minutes), starting from the checkpoint's C
 orchestrator: `try` and `regions` on whatever C it left -> STAGE line + region score
 better than the checkpoint? -> new checkpoint (checkpoints/<func>/best.diff + best.json)
 MATCH -> the gate, as usual
 "needs_permute" at the registers stage -> permuter pool (--big-permute-minutes) -> session n+1
 otherwise -> session n+1, until --big-sessions or --big-stale sessions in a row without progress
```

- **Which functions:** at least `--big-bytes` (600) bytes, or any function that parks in normal
  mode after its escalation attempt (`--escalate-big`, on). Normal workers never take a big one.
  `--big-workers 0` turns big mode off (every function goes to normal workers, as before).
- **Progress** is the region score of `decomp.py regions`: more of the target paired by opcode
  (structure), then more identical blocks, then more exact instructions; on a tie, or against an
  old checkpoint without a region score, the `STAGE` line of `decomp.py try` (a later stage, then
  fewer diffs). `STAGE` counts positions, so one session can lose blocks at the same count as
  another gained them (the 2026-10-14 A/B, `func_80019A60`). Only C that builds, passes the
  patch checks and beats the checkpoint replaces it, so a bad session can't lose earlier work. A
  session killed at `--big-minutes` is measured the same way.
- **Resuming:** each session starts from the checkpoint's C (re-applied mechanically if `main`
  has moved), and its prompt carries the checkpoint's log: each session's stage, note and its
  "next" suggestion. Checkpoints persist across runs in `--dir`.
- **When it stops:** after `--big-sessions` (6) sessions, or `--big-stale` (2) sessions in a row
  without progress, the function is parked and its best C stays in `checkpoints/`.
  `--retry-failed` gives it more.
- **The regions prompt** (the default, `--regions all`): big sessions work block by block with
  `decomp.py regions` (fix the first differing block, re-run). `--regions f1,f2` limits it to
  those functions and gives the rest the older `STAGE` prompt, for an A/B (`--regions none`: no
  function). After every big session the orchestrator writes `ab.json` (arm, start, status,
  score) and `regions.txt` (the region score of the session's C) to its results folder.
- **Declarations:** a worker may fix its function's own prototype or extern anywhere in `src/`
  and `include/` (the gate's full build catches a change that breaks another match). A
  `#define` named after the function is refused: it hides a type change from the callers.
- **Choosing `--big-bytes`:** `node tools/swarm/swarm.mjs stats` prints every function's outcome
  by size across runs (matched in normal or big mode, parked, attempts, big sessions). Put the
  threshold where normal mode's match rate drops; escalation catches the smaller functions that
  turn out hard anyway.

## Turning it off

| How | Effect |
|---|---|
| `node tools/swarm/swarm.mjs stop` | Graceful: no new functions; running workers finish and their matches are integrated, then it exits. |
| `node tools/swarm/swarm.mjs stop --now` | Kills every worker process and swarm container within seconds; uncommitted work is dropped. If the orchestrator doesn't respond in 15 s it is killed too, and the `STOP` file stays so nothing restarts until `run --clear-stop`. |
| Ctrl+C in its terminal | Once: graceful. Twice: hard stop. |
| Create `<dir>/STOP` (contents `now` for a hard stop) | The same as `stop`, by hand; the orchestrator checks it every 2 seconds. |

Containers are labelled `crashbash-swarm`, so `docker ps --filter label=crashbash-swarm` shows
them and `docker kill $(docker ps -q --filter label=crashbash-swarm)` stops them by hand.

## Running

```
node tools/swarm/swarm.mjs run --workers 3 --max-functions 10
node tools/swarm/swarm.mjs status
node tools/swarm/swarm.mjs stats      # outcomes by size, big-mode checkpoints
```

Needs Docker and `claude` on `PATH` (`npm i -g @anthropic-ai/claude-code`, then log in once with
`claude auth login`; `--claude-bin` for another path). Workers run with `--permission-mode
dontAsk`: anything outside the allowed tools is denied, never prompted. Options (defaults in
`config.json`, override any with `--kebab-case value`):

- `--workers` (3), `--seg` (`main/main`), `--only f1,f2` (exactly these functions). `--seg` is a
  prefix of the segment: `main/` takes every game unit of `main.exe`, including the gcc 2.7.2
  one (`main/8002d488`).
- Big mode: `--big-workers` (1), `--big-bytes` (600), `--big-model` (`opus`), `--big-minutes`
  (150), `--big-budget-usd` (20, per session), `--big-sessions` (6), `--big-stale` (2),
  `--big-permute-minutes` (60), `--escalate-big` (`true`). See [Big mode](#big-mode).
- `--permuters` (2), `--permute-minutes` (10): the permuter pool; `--permuters 0` turns it off
  (workers then just park).
- `--branch` (`swarm/integration`), `--remote` (`origin`): each match is pushed to
  `<remote>/<branch>` as soon as the gate passes (the branch is created from `--base`, `main`, if
  it's new). The default is a review branch; `--branch main` commits straight to GitHub's `main`
  like a hand session, with the gate as the only check. If the branch moves while the gate runs
  (a hand session pushed), the swarm redoes the gate on the new tip. Your checkout fast-forwards
  when it's on that branch with no uncommitted changes; otherwise `git pull`.
- `--model` (`sonnet`), `--escalate-model` (`opus`): a function the first model doesn't match gets
  one more attempt with the second, carrying the first attempt's notes.
- `--dir` (`<repo>-swarm`, next to the repo): clones, logs and results.
- `--resume RUN[,RUN...]`: after a stop, send the matches those runs verified with `try` but never committed
  straight to the gate instead of matching them again (the run id is in `swarm.log`). Matches
  that sat a long time may no longer fit `main`; the gate refuses those and a worker fixes them.
- `--dry-run`: everything but Claude (workers report "parked"). `--worker-cmd CMD`: run CMD in
  the worker clone instead of Claude (env `SWARM_FUNC`, `SWARM_SRC`, `SWARM_RESULT`), for tests.

## Safeguards

- **`main` only when asked:** by default matches go to a review branch; `main` needs `--branch
  main` on the command line. All work happens in clones under `--dir`; your checkout is only
  ever fast-forwarded, never edited.
- **Workers can't commit or push:** the allowed tools are file edits, `tools/dk` and read-only
  commands; `git commit/push/reset/clean`, `rm`, web and sub-agents are denied, and each clone's
  push URL is disabled. The orchestrator takes only `git diff` of `src/` and `include/`.
- **Patch checks:** only `src/` and `include/`, under 64 KB, removes exactly the target's
  `INCLUDE_ASM` line and adds none, no inline asm. Then the orchestrator runs `decomp.py try`
  itself; a worker's claim of a match is not trusted.
- **The gate, batched:** every commit is made by `decomp.py done --commit`, one function per
  commit, in one integrator. Matches waiting for the gate go through together (up to
  `--batch-max`, 6): each gets `done --defer-build` (scope, lint, function check, its own commit),
  then one `decomp.py verify` checks the batch's final state and only then is the batch pushed.
  If that build fails, nothing is pushed and each function goes through the full `done` alone.
  Batching only happens when the branch's `decomp.py` has `--defer-build`.
- **Fresh generated files:** each clone gets `asm/`, `linkers/`, `assets/` and `ext/` copied from
  your checkout, and `asm/` and `linkers/` are copied again whenever `configs/USA`'s sym/rel
  files or yamls change (a fingerprint in the clone's `.git/swarm-data`). So after a symbol
  split, run `make split` in your checkout before the run: stale clones failed every gate with
  "can't open asm/nonmatchings/.../func_X.s".
- **Gate builds get the CPU:** running permuter containers are paused (`docker pause`) while
  the gate works and resumed after.
- **Conflicts:** headers merge with `merge=union` in the swarm's clones (every match appends its
  prototype to the end of `bash/main.h`). Two functions within 8 lines of each other in a file
  are never leased at once. If a patch still conflicts, the orchestrator re-applies the worker's
  C mechanically (its INCLUDE_ASM line becomes the C, added lines go after the same anchor) and
  the gate checks it; only a patch it can't re-apply goes back to a worker.
- **Back-pressure:** no new functions are leased while more than `--max-queued` (3) matches wait
  for the gate, so workers don't run ahead into conflicts.
- **No duplicate work:** a function is leased to one worker; identical copies (same fingerprint)
  are never leased together. `history.json` stops a function being tried more than
  `--max-attempts` (2) across runs (`--retry-failed` to override).
- **Environment errors stop the run:** gate output that says the environment is broken, not
  the function (`can't open asm/... for reading`, `No rule to make target`, Docker unreachable,
  disk full; `INFRA_ERRORS` in `swarm.mjs`), stops the run at the first one. The function is not
  charged an attempt, and `run --resume RUN` gates its verified match once it's fixed.
- **Circuit breakers:** stops after `--max-consecutive-failures` (3) refused patches, gate
  failures or conflicts in a row, after `--max-minutes` (240), after `--max-functions` (20)
  leases, or if a push fails. Each worker is killed after `--worker-minutes` (45), a big-mode
  session after `--big-minutes` (150).
- **Cost caps:** each worker session is capped by `--worker-budget-usd` (5, passed to
  `claude --max-budget-usd`), a big-mode session by `--big-budget-usd` (20); the run stops when
  the workers' total reaches `--max-run-usd` (50).
  A session that fails to start (not logged in, API down) counts as a failure, so three in a
  row stop the run.
- **One run at a time** per `--dir` (lock file with the PID).
- **CPU:** with a permuter pool, workers share half the cores and permuters the other half
  (Docker `--cpus`); without one, workers get them all. A stop cuts running permuter runs short
  (nobody would port their result); the worker's C stays in `results/`.

## Output

In `--dir`: `swarm.log`, `run.json` (live state, read by `status`), `report.md` (one line per
attempt), `patterns.md` (Diff patterns rows workers proposed, fed to later workers in the same
run), `results/<func>/attempt*/` and `big*/` (prompt, worker output, patch, `try` and `done`
output), `checkpoints/<func>/` (big mode's best C so far: `best.diff`, and `best.json` with its
stage, diff count and the session log).

Parks are not committed. After a run:

1. `node tools/swarm/swarm.mjs stats`: check the match rates by size, and move `--big-bytes`
   if normal mode is failing below it or matching well above it.
2. Review `report.md` and the parks; a big-mode park's best C is in `checkpoints/`.
3. `node tools/swarm/swarm.mjs fold`: appends the rows of `patterns.md` to the Diff patterns
   table in `docs/MATCHING.md` (pipes inside code escaped, rows already there skipped) and
   archives it as `patterns.folded-<date>.md`. Review `git diff docs/MATCHING.md`, drop weak
   rows, commit.
4. Near misses: `node tools/swarm/swarm.mjs permute --near --minutes 60` gives every unmatched
   checkpoint at the registers stage a permuter run on its differing blocks (CPU only; can run
   overnight). Gate a match with the `run --resume` line it prints.
5. Add the run to `docs/AGENT_LOG.md` (Current state, Next, Session log), as after any session.

## Permuter-only sessions

For functions at the registers stage, the permuter often beats another model session, and it
costs nothing but CPU:

```
node tools/swarm/swarm.mjs permute --only func_8001795C,func_800248A0 --minutes 30
node tools/swarm/swarm.mjs permute --near --minutes 60     # every unmatched checkpoint at registers
```

`--near` picks the near misses itself: every checkpoint at the registers stage whose function is
still `INCLUDE_ASM` on `origin/main`, fewest diffs first (at most 16), so an overnight run needs no
list. Each run only randomizes the C of the blocks that differ (`decomp.py permute --differing`).

Each function gets a clone of your checkout's HEAD (`permuters/h1`, ...) with its big-mode
checkpoint applied, then `decomp.py permute --apply-best` (the permuter's best version goes into
the source only if `try` improves). The cores are split between the functions (`--jobs` per
function to override). A better result is saved as the function's checkpoint, so the next big
session or permute run starts from it; a match also goes to `results/`, and the log prints the
`run --resume <id> --only NAME` that gates it. Functions with no checkpoint and no C are skipped.
Ctrl+C or `stop --now` kills it (nothing is saved). It refuses to start while a run is active.

When a fix (a tool change, a header, a split) makes earlier failures out of date, reset those
functions so runs lease them again and no worker is told the old blocker:
`node tools/swarm/swarm.mjs forget --only f1,f2 --why "what changed" [--checkpoint]`. It sets
their attempts and big-mode sessions to 0 and replaces their notes with the reason (kept in
`history.json`); `--checkpoint` also deletes a big-mode checkpoint built on the old state.
`--retry-failed` does the same for every failure at once, for one run.
