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
# replace its INCLUDE_ASM line in src/... with C
tools/dk make diff FUNC=func_80012345                    # build that file and diff the function
tools/dk make permute FUNC=func_80012345                 # if stuck after ~10 rounds
tools/dk python3 tools/scripts/decomp.py done func_80012345 --commit --note "what made it match"
```

`done` is the gate. It checks scope (only this function's `INCLUDE_ASM` is
replaced), lint (no m2c leftovers, hacks declared with `--hack`), the function
and a clean full build of every target. Only then does it record and commit
`Match NAME (seg, N bytes)`. The pre-commit hook refuses matches committed any
other way. The bulk matcher `decomp.py auto` is retired.

Functions that won't match go to `decomp.py block NAME --note "..."`.

## Other changes

Renames: `decomp.py rename OLD NEW` (checks the build itself). Headers,
config and file splits: ordinary commits after `tools/dk make verify` prints
`OK`. If a change affects progress, run `tools/dk make progress` and include
`docs/` in the commit.

## Session log

Agents and contributors append a short entry to
[docs/AGENT_LOG.md](docs/AGENT_LOG.md) at the end of each working session:
what changed, what was learned, and what to do next.
