# Swarm: parallel matching

`swarm.mjs` runs several Claude Code workers at once, each matching one function in its own
clone, and one integrator that commits every match through the validation gate
(`decomp.py done --commit`). It runs on the host with Node (no dependencies).

Every game function matched on 2026-10-26, so the queue is empty: the swarm is kept for
another game's decomp or for a batch of rework, and this page records how it was run.

```
workers (N clones)                         integrator (one clone, serial)
 reset to the integration tip               git apply --3way the patch
 claude -p: match-function steps 1-4  -->   decomp.py done NAME --commit
 orchestrator: patch checks + `try`         push to <remote>/<branch>
        |
        | stuck: "needs_permute" + closest C (the worker's slot is freed)
        v
permuter pool (M clones, no Claude): make permute on that C; an improvement sends the
function back to the front of the queue with the permuter's diff
```

## Running

```
node tools/swarm/swarm.mjs run --branch main --seg overlays/ovl5d --max-minutes 60 --max-run-usd 30 \
  --max-functions 500 --model sonnet --escalate-model opus --big-budget-usd 8
node tools/swarm/swarm.mjs status
node tools/swarm/swarm.mjs stats      # outcomes and $ per match by size (--run ID: one run), permuter runs, checkpoints
node tools/swarm/swarm.mjs stop       # graceful; --now kills (then `run --clear-stop` to start again)
```

The run above is the low-cost recipe (sonnet first, about $0.20 a match on ovl5f; ovl5d $0.50,
its 250-599 B functions cost more; ovl65 and ovl68 $0.25-0.33). Use it for fresh overlays. The
other two:

- **Opus** (the `config.json` defaults, 5 normal + 1 big; polar, ovl60 and ovl63 matched 77-117
  an hour for $34-47): add `--big-workers 2` while big functions are queued. The normal workers
  finish the small functions first, then idle.
- **Big-heavy**, for an overlay's leftovers, mostly >= 600 B: `--workers 1 --big-workers 4
  --big-bytes 400`. Retry parks with it after tool upgrades (`decomp.py unblock`,
  `--retry-failed`): on 2026-10-16 it matched 12 of main.exe's 15 parks.

Functions it had never tried are the cheapest to queue (`--only` the untried list, smallest
first, no `--retry-failed`): 37 of 42 under 2 KB for $151, about $4 a match, against $7.50 for
retry runs, which keep re-trying the same hard ones (the queue puts checkpointed functions
first).

Needs Docker and `claude` on `PATH` (logged in once with `claude auth login`). Workers run with
`--permission-mode dontAsk`. Options (defaults in `config.json`, override with `--kebab-case value`):

- `--seg` (`main/main`): a prefix of the segment (`main/` is every unit of `main.exe`,
  `overlays/engine` every engine file); `--only f1,f2` for exactly these functions.
- `--workers` (5, since 2026-10-19; was 3), `--model` (`opus`, since 2026-10-19; was `sonnet`), `--escalate-model` (`opus`): a function the first model
  doesn't match gets one more attempt with the second.
- Big mode: `--big-workers` (1), `--big-bytes` (600), `--big-model` (`opus`), `--big-minutes`
  (27), `--big-budget-usd` (20), `--big-sessions` (6), `--big-stale` (2),
  `--big-permute-minutes` (30), `--escalate-big` (`true`).
- `--permuters` (2), `--permute-minutes` (10); `--permuters 0` turns the pool off.
- `--branch` (`swarm/integration`), `--remote` (`origin`): each match is pushed as soon as the gate
  passes. `--branch main` commits straight to `main`, like a hand session.
- Limits: `--max-minutes` (240), `--max-run-usd` (50), `--max-functions` (20; set it high so time
  and cost are the limits), `--worker-budget-usd` (5), `--worker-minutes` (45).
- `--resume RUN[,RUN...]`: gate the matches a stopped run verified but never committed.
- `--start-gap-seconds` (10): the least time between two worker starts; workers that start
  together all refresh the login token at once and fail.
- `--dir` (`<repo>-swarm`): clones, logs, results and checkpoints.
- `--page` (`docs/swarm.js`): the dashboard's live status file, relative to the repo; `--page ''`
  turns it off.
- `--dry-run`; `--worker-cmd CMD` runs CMD instead of Claude (for tests). A dry run still records
  its fake parks in `history.json` as attempts: don't use one to preview a queue.

## Live status on the dashboard

While a run is on, the orchestrator writes `docs/swarm.js` in your checkout (git-ignored) and
`docs/index.html` shows it as a Swarm card at the bottom of the page: the state, time and spend against the limits, what
each slot and permuter is on, the gate's queue, this run's outcomes, the log's last lines and the
recent runs (from `swarm.log` and `ledger.jsonl`, read once per run). It's written from what the
run holds in memory (no Docker, no git) when something changes, and every 15 s as a heartbeat; a
card that hasn't heard from its run for 2 minutes says "Not responding". The page loads it as a
script, which works from `file://`: every 5 s while the run is on, every minute otherwise. When
the swarm has committed matches the page doesn't have yet, the card says so with a Reload button
(your checkout follows the branch while it's clean). GitHub Pages has no `swarm.js`, so the card
stays hidden there.

## Big mode

Functions of at least `--big-bytes`, or that park in normal mode, go to big-mode workers: opus
sessions that work block by block with `decomp.py regions` and resume from a checkpoint.

```
session n: claude (opus, up to --big-minutes), starting from the checkpoint's C
orchestrator: `try` and `regions` on the C it left -> STAGE line + region score
better than the checkpoint? -> new checkpoint (checkpoints/<key>/best.diff + best.json)
MATCH -> the gate; "needs_permute" at the registers stage -> permuter pool -> session n+1
otherwise -> session n+1, until --big-sessions, or --big-stale sessions in a row without progress
```

`checkpoints/` and `results/` are keyed by function key (`8002D4F4`, `ovl5d_800B3A38`): the 14 slot
overlays all load at `0x800B32B4`, so 35 names repeat across them. For those the workers and
`decomp.py` get the key (the queue's `ref`); `--only`, `permute --only` and `forget --only` take names
or keys. Checkpoints saved by name before 2026-10-16 still resume when the name is unique.

- Progress is the region score: more of the target paired by opcode, then more identical
  blocks, then more exact instructions. Only C that builds, passes the patch checks and beats
  the checkpoint replaces it.
- Each session starts from the checkpoint's C, and its prompt carries the earlier sessions'
  notes. Checkpoints persist across runs.
- A function that stops progressing is parked with its best C in `checkpoints/`;
  `--retry-failed` gives it a fresh allowance for one run.
- **The closest C carries over:** a normal worker that doesn't match leaves its closest C and
  parks; the orchestrator measures it (one that matches after all goes to the gate) and keeps it
  as the function's checkpoint if it is the closest yet. Every later attempt, normal or big, in any
  run, starts from that checkpoint.
- **Session chaining** (`--chain N`, off by default, a pilot): after a normal session, the next small
  function of the same file runs in the same Claude session (`--resume`), up to N in a row. The
  ledger marks those `normal-chained`; compare them with `normal` in `stats` before turning it on.
- **Families:** identical functions (one fingerprint) are one job: the first member of each group
  that spans overlays is leased first (`--seed-families`, on by default), and while a tried member
  is unmatched its twins wait (a match fills them through `done`). A function with a matched twin
  or look-alike (the queue's `twin`) is an adapt job: normal mode whatever its size, and `next`
  prints the other C.
- Use `stats` to place `--big-bytes` where normal mode's match rate drops, and its $ per match
  to compare runs (`--run ID`).

## What sessions cost

Each worker session gets a `--session-id`; when it ends, one line goes to `ledger.jsonl` in the swarm
directory (function, size, kind, model, attempt, outcome, $, tokens, turns, minutes, `try` runs,
reads and greps of `docs/*.md`). A finished session's $ is its result line's; a killed one (its
deadline, a stop) has none, so its transcript (`~/.claude/projects/<clone>/<session>.jsonl`) is
priced with `prices` in config.json ($ per million tokens: input, output, cache read, cache write),
scaled by how this run's finished sessions compare with their own transcripts priced. Both count
towards `--max-run-usd`. The prices were fitted on 2026-10-16 from 745 sessions' result lines
(estimates within 2%); refit them when the models or prices change. Sessions before 2026-10-16 were
backfilled into the ledger (`"backfilled": true`; their "matched" means `try` said MATCH).

Where the money went (760 sessions to 2026-10-16): cache writes are the biggest share (49% of
normal spend, 34% of big), then output and cache reads. Docs are hardly read (0.14 reads per big
session); tool output fills the context, and a big session's cost comes from its turns (~70, each
re-reading a growing context). Time outside the model is 44% of a normal session, 31% of a big one.
So the levers are fewer turns (better starting points, short tool output) and less fixed context
per session, not shorter docs.

## Safeguards

- **`main` only when asked** (`--branch main`); all work happens in clones, and your checkout is
  only fast-forwarded.
- **Workers can't commit or push:** file edits, `tools/dk` and read-only commands only. The
  orchestrator takes only `git diff` of `src/` and `include/`.
- **Workers can't change what runs on the host:** Edit/Write only `src/**`, `include/**` and
  `.swarm-result.json` (allow rules for paths must be `Edit(...)`: they cover Write too, and the CLI
  ignores a `Write(path)` rule); on the host only `ls`, `cat`, `head`, `tail`, `grep`, `wc`, `date` and
  `tools/dk` (git runs as `tools/dk git ...`); their containers see `.git`, `tools/` and `.claude/`
  read-only, and the clone's `tools/dk` is read-only. `--dir` can't contain spaces (the mounts go
  through `DK_DOCKER_ARGS`).
- **Environment errors stop the run uncharged:** tools/dk's Docker errors (exit 125-127), a
  `PermissionError`, a tools/dk call over its time limit, or Docker gone after a session that left
  no patch. Fix it and run again. A usage limit isn't recognised as such
  (deferred: runs are supervised); its sessions fail uncharged and three in a row stop the run.
- **Tests:** `node --test tools/swarm/swarm.test.mjs` (host Node) after any change to the policy.
- **Patch checks:** only `src/` and `include/`, under 64 KB, removes exactly the target's
  `INCLUDE_ASM` line, no inline asm; then the orchestrator runs `decomp.py try` itself.
- **The gate:** every commit is `decomp.py done --commit`, one function each; waiting matches
  are batched (`--batch-max` 6) under one full build before pushing.
- **Generated files:** clones get `asm/`, `linkers/`, `assets/` and `ext/` from your checkout,
  refreshed when `configs/USA` changes. Run `make split` here after a symbol or yaml change.
- **Conflicts:** headers merge with `merge=union` (both sides kept, so two matches can leave a duplicate declaration). When the gate refuses one on a declaration clash, the orchestrator first runs `decomp.py reconcile` (tools/scripts/headers.py: the tree's declaration wins over the patch's re-declaration of the same name, if every file that includes the header also sees it; a patch-added duplicate goes) and gates once more; only if that fails does a worker fix it, with cc1's error lines and a hint to merge the duplicate (2026-10-19: the four clashes that day were all fixed this way by hand); neighbouring functions aren't leased together;
  a conflicting patch is re-applied mechanically, else it goes back to a worker.
- **Stops:** after `--max-consecutive-failures` (3) failures in a row, an environment error, a
  failed push, or any limit above. A function is tried at most `--max-attempts` (2) times across
  runs.
- One run at a time per `--dir`. Containers are labelled `crashbash-swarm`.

## After a run

Parks are not committed. Then:

1. `swarm.mjs stats`: check match rates by size; move `--big-bytes` if needed. Its permuter
   table (from `permuter.jsonl`, one line per permuter run) shows how often a run improves
   or matches for its CPU; `decomp.py permstats <swarm dir>/permuter.jsonl` splits it by mode.
2. Review `report.md` and the parks (big-mode best C in `checkpoints/`).
3. `swarm.mjs fold`: appends the workers' proposed rows (`patterns.md`) to the Diff patterns table
   in `docs/MATCHING.md`. Review the diff, drop weak rows and duplicates, commit.
4. Near misses: `swarm.mjs permute --near --minutes 60` (CPU only; gate a match with the
   `run --resume` line it prints).
5. Add the run to `docs/AGENT_LOG.md`.

## Permuter-only sessions

```
node tools/swarm/swarm.mjs permute --only func_8001795C,func_800248A0 --minutes 30
node tools/swarm/swarm.mjs permute --near --minutes 60     # every unmatched checkpoint at registers
```

Each function gets a clone with its checkpoint applied and a permuter run on the C of its
differing blocks; a better result becomes its checkpoint, and a match prints the
`run --resume <id> --only NAME` that gates it.

When a change (a tool, a header, a split) makes old failures out of date:
`swarm.mjs forget --only f1,f2 --why "what changed" [--checkpoint]` resets their attempts and notes
(`--checkpoint` also deletes the old checkpoint). Functions parked by hand (`decomp.py block`) are
never queued: `decomp.py unblock f1 f2 --why "..."` puts them back (their park notes stay, and the
workers see them). After tool upgrades, retry them in big mode only (`--big-bytes 1 --retry-failed`):
on 2026-10-16 that matched 12 of main.exe's 15 parks in 75 min.
