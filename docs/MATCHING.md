# Matching workflow

Functions are matched by hand, by a person or an AI agent, **one function per
commit**, and every commit goes through one validation gate ([diagram](../README.md#how-functions-are-matched)). The bulk matcher
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

## GTE code

Write GTE code with Sony's macros from `inline_c.h` and `gtemac.h` (PsyQ 4.7,
included by `include/psyq.h`): `gte_ldv0(v); gte_rt(); gte_stlvl(out);` is
`lwc2 x2, nop, nop, mvmva, swc2 x3`. Read the target's cop2 instruction to pick
the macro: `rtps`, `nclip`, `avsz3`, and `mvmva sf, mx, v, cv, lm`, where mx
0/1/2 = RT/LL/LC, v 0..2 = V0..V2 (3 = IR), cv 0/1/3 = TR/BK/none
(`mvmva 1,0,0,0,0` is `gte_rt`/`gte_rtv0tr`). The macros emit placeholder
words; `tools/scripts/dmpsx.py` turns them into the real instructions after
cc1, as Sony's DMPSX did. The gate's lint doesn't count these macros as
inline asm. m2c can't draft GTE functions, so write them from the asm.

## Loops gcc rotates but the target doesn't

gcc 2.8 moves a `while`/`for` loop's exit test to the bottom when that test
jumps to the loop's own end. If the target tests at the top and jumps back
with `j` at the bottom, the source left the loop by `return` or by `goto` to
a label after it:

```c
for (;;) {
    if (--n == -1) {
        goto none; /* or return */
    }
    ...
}
```

A loop built from labels and `goto` alone doesn't match: loop.c then skips it,
so loop constants (`li $a0, -1`) aren't hoisted.

## For AI agents

- Work on one function at a time and commit it before picking the next.
- Start each session by reading [AGENT_LOG.md](AGENT_LOG.md). End it by
  appending an entry that covers what matched, what got parked and why, and
  any idioms you learned.
- Never fake a match: no whole-function `asm()`, no editing `asm/`, `build/`
  or the checksums, and no committing around the gate.
- The project skill `.claude/skills/match-function` runs this loop: `/match-function`
  matches the next function, `/match-function NAME` a specific one.
