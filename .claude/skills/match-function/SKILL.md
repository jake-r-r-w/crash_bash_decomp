---
name: match-function
description: Hand-match one Crash Bash function to the retail bytes and commit it through the validation gate (decomp.py done). Use when asked to match, decompile or "do the next" function in this repo, or with a function name (/match-function func_800C6828).
---

# Match one function

Follow [docs/MATCHING.md](../../../docs/MATCHING.md). Everything runs through `tools/dk`;
the host has no Python. In Git Bash loops, pass `</dev/null` to `tools/dk`.

## Hard rules

- **Only C.** No inline asm of any kind: no `asm()` statements, register pins
  (`register ... asm("$N")`) or `asm("")` barriers, in the body or in a macro or
  static inline it uses. `done` refuses them. The SDK's `gte_*` macros are the
  one exception.
- **Natural C first.** Write what a 1999 programmer would plausibly have
  written, and fix diffs with types, struct layouts, prototypes, statement
  order and real code shapes found from the evidence and the compiler's rules
  (step 3). Only when step 3 finds no natural form may you use C that just
  steers a pass: a `do { } while (0)` wrapper, an extra temporary or store, a
  reordered statement. Then say in the `done --note` which one and the gcc
  rule it works around.
- **Compiler first.** Every C change for an instruction-order, register or
  tail-merge difference must be justified by the gcc rule that decides it
  (step 3). Shuffling statements until the diff moves is not analysis.
- **Permuter last, and rarely.** At most one run per function, under the
  conditions in step 4. Its output is a clue, never a fix by itself.
- **Parking is a good outcome.** When nothing matches, a clean park with a
  precise note is fine.
- **Write a script when it helps.** If a one-shot script would answer a
  question faster than reading by hand (diffing two pass dumps, tracing one
  pseudo-reg through the RTL, counting sched priorities, brute-forcing a few C
  shapes through cc1), write it and run it. One-shot scripts go in `scratch/`
  (ignored) and run through `tools/dk`. If it would help on other functions
  too, make it a proper tool before the session ends: a `decomp.py` subcommand
  or a module in `tools/scripts/`, with a test in `tools/scripts/tests/`, a
  `--help`, a line in docs/MATCHING.md where it is used, and in the CLAUDE.md
  tool list. Commit it on its own, separate from any match.

## Steps

1. **Pick.** With an argument: `tools/dk python3 tools/scripts/decomp.py next --func NAME`.
   Without one: `tools/dk python3 tools/scripts/decomp.py next`.
   Read the asm, not just the draft. m2c misses things like pass-through
   arguments (`a0` never written), and it misses signedness and struct
   offsets.
   **Check the unit:** if every global store is `lui $at` + `sw`, or jump
   delay slots are filled from a store, the code is gcc 2.7.2 + ASPSX. In a
   file built with gcc 2.8.1 it can't match: stop and report that it needs
   a split (docs/MATCHING.md, step 2). From about 600 bytes up, read "Big
   functions" in docs/MATCHING.md: it is a multi-session job.
   **Evidence first** (docs/MATCHING.md): `decomp.py dups`, `similar NAME`,
   grep matched C in `src/` for the same globals or shape, the headers in
   `include/bash/` and the leaked minigame headers.
2. **Write C** in the file that `next` printed (`src ...`), replacing that
   function's `INCLUDE_ASM` line only. Rules:
   - C89: declarations at the top of blocks, `/* */` comments, `s32`/`u8`/... types.
   - Declare what the function uses in the owned header of the file that defines
     it (`bash/main.h`, `bash/engine.h`, `bash/<overlay>_ovl.h`, marked `owner:`), or in
     the original headers (`bash/pogo.h`, ...) if they already name it. `done`
     adds the function's own prototype. See docs/CONVENTIONS.md (Headers).
   - No `M2C_*` macros, no `?` types, no inline asm.
   - Prefer struct fields over pointer arithmetic once the layout is known.
3. **Diff and reason from the compiler.** `tools/dk make diff FUNC=NAME`. The last
   line, `STAGE NAME stage n/total`, says what to fix next: `structure` (length,
   branches, loops, jump tables) first, then `instructions` (opcodes: order,
   temps, types), then `registers`. For a long diff, or an early extra
   instruction shifting everything after it, use `decomp.py regions NAME`.
   - Structure and opcode differences: types, signedness, `s16`/`s32` locals,
     loop shape (`do {} while` vs `for`), pointer increments vs indexing; check the
     "Diff patterns" table and the checklist for this kind of diff.
   - Instruction order, register choice, merged tails: before editing C,
     1. find the deciding pass: dump the function with the real cc1
        (`-dr -ds -dc -dS -dR -dl -dJ`; "Using the compiler directly") and follow
        the differing insn from pass to pass to the first one that can't give
        the target's code. Use `decomp.py sched NAME` for order ties and
        `decomp.py vars NAME` / `splitvars NAME VAR` for register swaps;
     2. open that pass's source in `tools/reference/gcc-2.8.1/` (`gcc-2.7.2.3/`
        for the 2.7.2 units) and name the function and condition that decides
        the tie (e.g. `rank_for_schedule` in sched.c, `qty_compare` in
        local-alloc.c, cross-jumping in jump.c);
     3. work it through by hand: which property of the RTL (priority, birth
        order, `reg_n_refs`, luid, the block's memory-insn count, ...) has to
        change for the target's choice to win;
     4. find a natural C change that produces that property. Try shapes in a small
        test file with `/opt/gcc/2.8.1-psx/cc1 -quiet -O2 -G0 -fno-builtin` first,
        then port the winner and re-check it in the function.
     Before each such edit, write in your reasoning the rule (file:function)
     that justifies it.
4. **Permuter: only if all of these hold.**
   - STAGE is `registers` (the permuter doesn't fix structure or opcodes);
   - step 3's analysis is done and written down, and found no natural fix;
   - it is the only run for this function: `tools/dk make permute FUNC=NAME
     MINUTES=5 ARGS=--differing`.
   Port a find only if you can explain it with a gcc rule; prefer it in
   natural form, and treat a find that only steers a pass like any steering C
   (Natural C first). One you can't explain is discarded. Otherwise park
   (step 5).
5. **Finish one of two ways.**
   - **Match:** `tools/dk python3 tools/scripts/decomp.py done NAME --commit --note "<one line: what made it match>" --trailer "Co-Authored-By: <your model attribution>"`.
     If `done` refuses, fix the reason it gives (scope, lint, function, or
     full build). Never commit a match with plain `git commit`, and never use
     `--no-verify`.
   - **Park:** keep the best C under `#ifdef NON_MATCHING`, with the
     `INCLUDE_ASM` line under `#else` (`#endif` after), and run `decomp.py block NAME --note
     "<the STAGE line>; deciding pass and rule; what was tried; next: ..."`.
6. **Report** the commit hash (or the park), the function, and anything learned.
   Put an idiom in the Diff patterns table in docs/MATCHING.md *with the gcc rule
   that explains it* (not just "permuter find"); update `docs/AGENT_LOG.md`
   (Current state, Open leads for a park, and the session's Session log entry).

One function per invocation. To do several, invoke the skill again for each.
Each function gets its own commit.
