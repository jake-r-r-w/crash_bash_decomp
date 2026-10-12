# Swarm: parallel matching

`swarm.mjs` runs several Claude Code workers at once, each matching one function in its own
clone, and a single integrator that commits every match through the validation gate
(`decomp.py done --commit`). It runs on the host with Node (no dependencies), because it starts
`claude` and `tools/dk`.

```
workers (N clones)                         integrator (one clone, serial)
 reset to the integration tip               git apply --3way the patch
 claude -p: match-function steps 1-4  -->   decomp.py done NAME --commit
 orchestrator: patch checks + `try`         push to the swarm branch
```

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
```

Needs Docker and `claude` on `PATH` (`npm i -g @anthropic-ai/claude-code`, then log in once with
`claude auth login`; `--claude-bin` for another path). Workers run with `--permission-mode
dontAsk`: anything outside the allowed tools is denied, never prompted. Options (defaults in
`config.json`, override any with `--kebab-case value`):

- `--workers` (3), `--seg` (`main/main`), `--only f1,f2` (exactly these functions)
- `--branch` (`swarm/integration`): where matches are committed, created from `--base` (`main`)
  the first time. Merge it into `main` yourself after review.
- `--model` (`sonnet`), `--escalate-model` (`opus`): a function the first model doesn't match gets
  one more attempt with the second, carrying the first attempt's notes.
- `--dir` (`<repo>-swarm`, next to the repo): clones, logs and results.
- `--push-origin`: also push the swarm branch to GitHub.
- `--dry-run`: everything but Claude (workers report "parked"). `--worker-cmd CMD`: run CMD in
  the worker clone instead of Claude (env `SWARM_FUNC`, `SWARM_SRC`, `SWARM_RESULT`), for tests.

## Safeguards

- **Never on `main`:** refuses `--branch main` (unless `--allow-main`) and a branch checked out
  in your repo. Your checkout is never touched: all work happens in clones under `--dir`.
- **Workers can't commit or push:** the allowed tools are file edits, `tools/dk` and read-only
  commands; `git commit/push/reset/clean`, `rm`, web and sub-agents are denied, and each clone's
  push URL is disabled. The orchestrator takes only `git diff` of `src/` and `include/`.
- **Patch checks:** only `src/` and `include/`, under 64 KB, removes exactly the target's
  `INCLUDE_ASM` line and adds none, no inline asm. Then the orchestrator runs `decomp.py try`
  itself; a worker's claim of a match is not trusted.
- **The gate is unchanged:** every commit is made by `decomp.py done --commit` (scope, lint,
  function, full build), one function per commit, in one integrator at a time.
- **No duplicate work:** a function is leased to one worker; identical copies (same fingerprint)
  are never leased together. `history.json` stops a function being tried more than
  `--max-attempts` (2) across runs (`--retry-failed` to override).
- **Circuit breakers:** stops after `--max-consecutive-failures` (3) refused patches, gate
  failures or conflicts in a row, after `--max-minutes` (240), after `--max-functions` (20)
  leases, or if a push fails. Each worker is killed after `--worker-minutes` (45).
- **Cost caps:** each worker session is capped by `--worker-budget-usd` (5, passed to
  `claude --max-budget-usd`); the run stops when the workers' total reaches `--max-run-usd` (50).
  A session that fails to start (not logged in, API down) counts as a failure, so three in a
  row stop the run.
- **One run at a time** per `--dir` (lock file with the PID).
- **CPU:** each worker's containers get `--cpus` (cores / workers).

## Output

In `--dir`: `swarm.log`, `run.json` (live state, read by `status`), `report.md` (one line per
attempt), `patterns.md` (Diff patterns rows workers proposed, fed to later workers in the same
run), `results/<func>/attempt*/` (prompt, worker output, patch, `try` and `done` output).

Parks are not committed. Review `report.md`, fold good rows from `patterns.md` into
`docs/MATCHING.md`, and add the session to `docs/AGENT_LOG.md`, as after any session.
