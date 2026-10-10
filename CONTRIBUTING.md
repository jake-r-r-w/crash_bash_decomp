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

All commands run through `tools/dk` (Docker):

```sh
tools/dk python3 tools/scripts/decomp.py next            # target, asm listing and m2c draft
# replace its INCLUDE_ASM line in src/... with C
tools/dk make diff FUNC=func_80012345                    # build that file and diff the function
tools/dk make permute FUNC=func_80012345                 # if stuck after a few attempts
tools/dk python3 tools/scripts/decomp.py done func_80012345 --commit
```

`done` re-checks the function, rebuilds from scratch and compares the whole
executable, copies the C to identical functions elsewhere, refreshes the
dashboard and badges, and commits `src/ include/ configs/ .decomp/ docs/` as
`Match NAME (seg, N bytes)`. Never commit a match by hand: `done` is the only
path that guarantees the full build was checked.

Small functions can be batched: `next --count 4`, then `done A B C D --commit`.
`decomp.py auto --max-size 128 --commit` matches trivial functions from m2c
drafts without any manual work; run it at the start and end of a session.

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
