# Contributing

## Ground rules

- **No game or SDK data in git.** Disc images, extracted files, the
  executable, `asm/`, `assets/`, `linkers/`, `build/` and `ext/` stay local
  (so do your notes in `scratch/`).
  The pre-commit hook (`tools/hooks/pre-commit`, enabled with
  `git config core.hooksPath tools/hooks`) refuses PS-X EXE files, game data
  paths and anything over 2 MB.
- **Every commit keeps the build matching.** Run `tools/dk make check` (or
  `make verify` after config, header or file-layout changes) and commit only
  when it prints `OK`.
- **Don't decompile PsyQ.** Sony's objects are linked in place; library code
  is not a matching target.
- Follow [docs/CONVENTIONS.md](docs/CONVENTIONS.md) for names, headers and
  placement.

## Matching a function

Every game function matches (2026-10-26), so this is now the route for
reworking one (a natural form for a steering match) and the record of how
the matching was done. What is left is in [docs/ROADMAP-B.md](docs/ROADMAP-B.md) (done work in
[ROADMAP-A.md](docs/ROADMAP-A.md)). Functions were matched by hand (by a person or an AI agent), **one function per
commit**, through the validation gate described in
[docs/MATCHING.md](docs/MATCHING.md). All commands run through `tools/dk`:

```sh
tools/dk python3 tools/scripts/decomp.py next            # target, asm listing and draft
# check the asm's style fits the file's compiler (docs/MATCHING.md, step 2)
# replace its INCLUDE_ASM line in src/... with C
tools/dk make diff FUNC=NAME                             # diff; last line: STAGE structure/instructions/registers
tools/dk make permute FUNC=NAME                          # only at the registers stage, if stuck
tools/dk python3 tools/scripts/decomp.py done NAME --commit --note "what made it match"
```

`done` is the gate. It checks scope (this function's `INCLUDE_ASM` is
replaced by C, no other one is, and other functions' C changes only
where it calls this one), lint (no m2c leftovers, no inline asm: only C), the function
and a clean full build of every target. Only then does it record and commit
`Match NAME (seg, N bytes)`. The pre-commit hook refuses matches committed any
other way. The old bulk matcher (`decomp.py auto`) is removed.

Functions of about 600 bytes or more take several sessions; see Big
functions in [docs/MATCHING.md](docs/MATCHING.md).

The swarm ([tools/swarm](tools/swarm/README.md)) matches several functions at
once and commits through the same gate. It batches: `done --defer-build` per
function (still one commit each), then one `decomp.py verify` for the batch,
and nothing is pushed unless that passes. Big functions go to its big-mode
workers, which resume from the best C so far each session.

Functions that won't match go to `decomp.py block NAME --note "..."`.
Rewriting an already matched function (a rename, a retype, a natural form) is
an ordinary commit once `tools/dk make check` prints `OK`.

## Other changes

Renames: `decomp.py rename OLD NEW` (checks the build itself); names and
types from a prototype or demo disc go through `proto_apply.py` and
`proto_types.py` ([docs/PROTOTYPES.md](docs/PROTOTYPES.md)), and a file
split through `split_unit.py` (CONVENTIONS.md, Source layout). A change to an
asm filter (`tools/scripts/dmpsx.py`, `aspsx_fill.py`) needs `tools/dk make
test-tools` to pass and a case in `tools/scripts/tests/` for the new rule. A change to
the swarm's policy (`tools/swarm/swarm.mjs`: leasing, scoring, escalation, patch
checks) needs `node --test tools/swarm/swarm.test.mjs` (host Node) to pass and a
case there. Headers,
config and file splits: ordinary commits after `tools/dk make verify` prints
`OK`. If a change affects progress, run `tools/dk make progress` and include
`docs/` and `.decomp/history.json` (the dashboard's history, one entry per run) in the commit.

A change to a game data tool (`tools/scripts/dat.py`, `tex.py`, `mdl.py`,
`gltf.py`, `sfx.py`, `tga.py`, `txt.py`, `pack_disc.py`) needs `tools/dk make
test-tools` to pass, a case in `tools/scripts/tests/`, and a fresh extraction
to rebuild the retail disc: delete `assets/CRASHBSH/` (or `dat.py extract
--force`), then `tools/dk make disc-check`. Formats go in
[docs/ASSETS.md](docs/ASSETS.md).

## Testing a build in an emulator

A match needs no emulator: `make check` proves the build is the retail bytes,
so it plays like retail. For a build that differs on purpose, `tools/dk make
disc` gives a disc image to play yourself; how to run it is in
[docs/PLAYING.md](docs/PLAYING.md).

## Session log

At the end of each working session, agents and contributors update
[docs/AGENT_LOG.md](docs/AGENT_LOG.md): its current state, next steps, open
leads and gotchas, plus a short entry at the top of its session log.
Matching idioms go in the Diff patterns table in
[docs/MATCHING.md](docs/MATCHING.md), facts about the game in
[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).
