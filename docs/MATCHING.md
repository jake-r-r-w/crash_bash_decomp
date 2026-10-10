# Matching workflow

Functions are matched by hand, by a person or an AI agent, **one function per
commit**, and every commit goes through one validation gate. The bulk matcher
(`decomp.py auto`) is retired. Its earlier drafts still show up in `next` as a
starting point.

## The loop

All commands run through `tools/dk` (Docker).

1. **Pick:** `tools/dk python3 tools/scripts/decomp.py next`
   prints the target, its callers, the asm and a draft (m2c, or an earlier
   compiling draft). `next --func NAME` picks a specific one, `--seg REGEX`
   restricts to a segment (`overlays/ovl62`, ...).
2. **Write C:** replace the function's `INCLUDE_ASM` line in its source file
   with C. Use real types from `include/` (add structs there, see
   [CONVENTIONS.md](CONVENTIONS.md)). Don't leave m2c leftovers.
3. **Diff:** `tools/dk make diff FUNC=NAME` builds that file and shows
   only the differing instructions. Iterate on steps 2 and 3.
4. **Stuck?** After about 10 diff rounds with no progress, try
   `tools/dk make permute FUNC=NAME MINUTES=10`, then take what it found
   by hand. Still stuck: restore the `INCLUDE_ASM` line, keep the best
   attempt under `#ifndef NON_MATCHING`, and run
   `decomp.py block NAME --note "what differs"`. Then move on.
5. **Validate and commit:**
   `tools/dk python3 tools/scripts/decomp.py done NAME --commit --note "what made it match"`

## The validation gate (`done`)

`done` records nothing, and commits nothing, unless every check passes:

| Check | Fails when |
|---|---|
| Scope | the working tree replaces any other function's `INCLUDE_ASM` (one function per commit) |
| Lint | the function still contains `M2C_*` macros, m2c's `?` type, or inline asm with instructions |
| Hacks | a register pin or an `asm("")` barrier is used without `--hack "why"` |
| Function | the function's compiled words differ from the target |
| Full build | a clean rebuild of every target (exe and overlays) is not byte-identical |

Then it records the match, copies the same C to identical functions elsewhere
(they're listed in the commit body), refreshes the dashboard, and commits
`src/ include/ configs/ .decomp/ docs/` as `Match NAME (seg, N bytes)` with a
`Validated-by:` trailer.

**Enforcement:** the pre-commit hook (`tools/hooks/pre-commit`, enable it with
`git config core.hooksPath tools/hooks`) refuses any commit that replaces an
`INCLUDE_ASM` line unless the commit came from `done`.

## Commits that aren't matches

Renames (`decomp.py rename`), header moves, typing an already matched function,
and file splits are ordinary small commits made after `tools/dk make verify`
prints `OK` for every target. Keep each one to a single purpose.

## For AI agents

- Work on one function at a time and commit it before picking the next.
- Start each session by reading [AGENT_LOG.md](AGENT_LOG.md). End it by
  appending an entry that covers what matched, what got parked and why, and
  any idioms you learned.
- Never fake a match: no whole-function `asm()`, no editing `asm/`, `build/`
  or the checksums, and no committing around the gate.
- The project skill `.claude/skills/match-function` runs this loop: `/match-function`
  matches the next function, `/match-function NAME` a specific one.
