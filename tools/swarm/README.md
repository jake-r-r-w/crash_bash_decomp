# Swarm: parallel matching

`swarm.mjs` runs several Claude Code workers at once, each matching one function in its own
clone, and one integrator that commits every match through the validation gate
(`decomp.py done --commit`). It runs on the host with Node (no dependencies).

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
node tools/swarm/swarm.mjs run --branch main --seg overlays/engine --max-minutes 60 --max-run-usd 80 \
  --max-functions 500 --workers 3 --big-workers 2 --big-bytes 400 --big-minutes 27
node tools/swarm/swarm.mjs status
node tools/swarm/swarm.mjs stats      # outcomes and $ per match by size (--run ID: one run), checkpoints
node tools/swarm/swarm.mjs stop       # graceful; --now kills (then `run --clear-stop` to start again)
```

Needs Docker and `claude` on `PATH` (logged in once with `claude auth login`). Workers run with
`--permission-mode dontAsk`. Options (defaults in `config.json`, override with `--kebab-case value`):

- `--seg` (`main/main`): a prefix of the segment (`main/` is every unit of `main.exe`,
  `overlays/engine` every engine file); `--only f1,f2` for exactly these functions.
- `--workers` (3), `--model` (`sonnet`), `--escalate-model` (`opus`): a function the first model
  doesn't match gets one more attempt with the second.
- Big mode: `--big-workers` (1), `--big-bytes` (600), `--big-model` (`opus`), `--big-minutes`
  (150), `--big-budget-usd` (20), `--big-sessions` (6), `--big-stale` (2),
  `--big-permute-minutes` (60), `--escalate-big` (`true`).
- `--permuters` (2), `--permute-minutes` (10); `--permuters 0` turns the pool off.
- `--branch` (`swarm/integration`), `--remote` (`origin`): each match is pushed as soon as the gate
  passes. `--branch main` commits straight to `main`, like a hand session.
- Limits: `--max-minutes` (240), `--max-run-usd` (50), `--max-functions` (20; set it high so time
  and cost are the limits), `--worker-budget-usd` (5), `--worker-minutes` (45).
- `--resume RUN[,RUN...]`: gate the matches a stopped run verified but never committed.
- `--dir` (`<repo>-swarm`): clones, logs, results and checkpoints.
- `--dry-run`; `--worker-cmd CMD` runs CMD instead of Claude (for tests).

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

## Safeguards

- **`main` only when asked** (`--branch main`); all work happens in clones, and your checkout is
  only fast-forwarded.
- **Workers can't commit or push:** file edits, `tools/dk` and read-only commands only. The
  orchestrator takes only `git diff` of `src/` and `include/`.
- **Workers can't change what runs on the host:** Edit/Write only `src/**`, `include/**` and
  `.swarm-result.json`; on the host only `ls`, `cat`, `head`, `tail`, `grep`, `wc`, `date` and
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
- **Conflicts:** headers merge with `merge=union`; neighbouring functions aren't leased together;
  a conflicting patch is re-applied mechanically, else it goes back to a worker.
- **Stops:** after `--max-consecutive-failures` (3) failures in a row, an environment error, a
  failed push, or any limit above. A function is tried at most `--max-attempts` (2) times across
  runs.
- One run at a time per `--dir`. Containers are labelled `crashbash-swarm`.

## After a run

Parks are not committed. Then:

1. `swarm.mjs stats`: check match rates by size; move `--big-bytes` if needed.
2. Review `report.md` and the parks (big-mode best C in `checkpoints/`).
3. `swarm.mjs fold`: appends the workers' proposed rows (`patterns.md`) to the Diff patterns table
   in `docs/MATCHING.md`. Review the diff, drop weak rows (hacks, duplicates), commit.
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
(`--checkpoint` also deletes the old checkpoint).
