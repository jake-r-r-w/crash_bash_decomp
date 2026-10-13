# Contributing

## Ground rules

- **No game or SDK data in git.** Disc images, extracted files, the
  executable, `asm/`, `assets/`, `linkers/`, `build/` and `ext/` stay local.
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

Functions are matched by hand (by a person or an AI agent), **one function per
commit**, through the validation gate described in
[docs/MATCHING.md](docs/MATCHING.md). All commands run through `tools/dk`:

```sh
tools/dk python3 tools/scripts/decomp.py next            # target, asm listing and draft
# check the asm's style fits the file's compiler (docs/MATCHING.md, step 2)
# replace its INCLUDE_ASM line in src/... with C
tools/dk make diff FUNC=func_80012345                    # diff; last line: STAGE structure/instructions/registers
tools/dk make permute FUNC=func_80012345                 # only at the registers stage, if stuck
tools/dk python3 tools/scripts/decomp.py done func_80012345 --commit --note "what made it match"
```

`done` is the gate. It checks scope (only this function's `INCLUDE_ASM` is
replaced), lint (no m2c leftovers, hacks declared with `--hack`), the function
and a clean full build of every target. Only then does it record and commit
`Match NAME (seg, N bytes)`. The pre-commit hook refuses matches committed any
other way. The bulk matcher `decomp.py auto` is retired.

Functions of about 600 bytes or more take several sessions; see Big
functions in [docs/MATCHING.md](docs/MATCHING.md).

The swarm ([tools/swarm](tools/swarm/README.md)) matches several functions at
once and commits through the same gate. It batches: `done --defer-build` per
function (still one commit each), then one `decomp.py verify` for the batch,
and nothing is pushed unless that passes. Big functions go to its big-mode
workers, which resume from the best C so far each session.

Functions that won't match go to `decomp.py block NAME --note "..."`.

## Other changes

Renames: `decomp.py rename OLD NEW` (checks the build itself). Headers,
config and file splits: ordinary commits after `tools/dk make verify` prints
`OK`. If a change affects progress, run `tools/dk make progress` and include
`docs/` in the commit.

## Session log

At the end of each working session, agents and contributors update
[docs/AGENT_LOG.md](docs/AGENT_LOG.md): its current state, next steps, open
leads and gotchas, plus a short entry at the top of its session log.
Matching idioms go in the Diff patterns table in
[docs/MATCHING.md](docs/MATCHING.md), facts about the game in
[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).
