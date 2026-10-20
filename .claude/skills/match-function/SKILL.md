---
name: match-function
description: Hand-match one Crash Bash function to the retail bytes and commit it through the validation gate (decomp.py done). Use when asked to match, decompile or "do the next" function in this repo, or with a function name (/match-function func_800C6828).
---

# Match one function

Follow [docs/MATCHING.md](../../../docs/MATCHING.md). Everything runs through `tools/dk`;
the host has no Python. In Git Bash loops, pass `</dev/null` to `tools/dk`.

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
2. **Write C** in the file that `next` printed (`src ...`), replacing that
   function's `INCLUDE_ASM` line only. Rules:
   - C89: declarations at the top of blocks, `/* */` comments, `s32`/`u8`/... types.
   - Declare what the function uses in the owned header of the file that defines
     it (`bash/main.h`, `bash/engine.h`, `bash/ovlXX.h`, marked `owner:`), or in
     the original headers (`bash/pogo.h`, ...) if they already name it. `done`
     adds the function's own prototype. See docs/CONVENTIONS.md (Headers).
   - No `M2C_*` macros, no `?` types, no `asm()` with instructions.
   - Prefer struct fields over pointer arithmetic once the layout is known.
3. **Diff:** `tools/dk make diff FUNC=NAME`, then iterate. The last line,
   `STAGE NAME stage n/total`, says what to fix next: `structure` (length,
   branches, loops, jump tables) first, then `instructions` (opcodes: order,
   temps, types), then `registers`. Typical fixes: statement order,
   signed/unsigned, `s16`/`s32` locals, temporaries, `do {} while` instead of
   `for`, pointer increments instead of indexing. When the diff is long, or
   an early extra instruction shifts everything after it, use
   `tools/dk python3 tools/scripts/decomp.py regions NAME` instead: it aligns
   the code, lists the differing blocks with their C lines and shows the
   first one side by side; fix that block, re-run.
4. **Before the permuter.** Use `make permute` only after all of these:
   - the evidence step from docs/MATCHING.md ("Evidence first": `decomp.py dups` and `similar NAME`,
     grep matched C in `src/` for the same globals or shape, the headers in
     `include/bash/` and the leaked minigame headers);
   - the "Diff patterns" table and the checklist for this kind of diff
     (register-only, instruction order, or opcode/length), noting in your
     reasoning which items you tried;
   - for register differences, `decomp.py vars NAME` (the variable behind each
     swap; it points to `decomp.py splitvars NAME VAR` when one variable wants
     different registers in different blocks), then the cc1 dump ("Why does a
     register differ?").
   - for a tie (two instructions in the other order, two values in each
     other's registers, a tail merged into the wrong copy): the deciding
     pass's trace (`-dS`, `-dR`, `-dl`, `-dJ`) and that pass's source in
     `tools/reference/gcc-2.8.1/` ("Reading the compiler's rules" in
     docs/MATCHING.md). Find the tie in the trace, read the function that
     decides it, work it through by hand, and change the C so the tie goes the
     target's way.
   - the compiler itself ("Using the compiler directly" in docs/MATCHING.md):
     dump every pass for the function (`-dr -ds -dc` too, for expand, cse and
     combine), follow the differing instruction from pass to pass to the
     first one that can't give the target's code, and try C shapes in a small
     test file compiled with `/opt/gcc/2.8.1-psx/cc1 -quiet -O2 -G0
     -fno-builtin` (seconds per shape). Then port the winner and re-check it
     in the function.
   - the `registers` stage: the permuter doesn't fix structure.
   Then run `tools/dk make permute FUNC=NAME MINUTES=10` (for a big function add
   `ARGS=--differing`: only the C of the blocks that differ). When it finds
   something, port it by hand and add the general rule as a row in the
   "Diff patterns" table. If it still doesn't match, restore the `INCLUDE_ASM` line, keep the attempt
   under `#ifndef NON_MATCHING ... #endif`, run
   `decomp.py block NAME --note "<the STAGE line>; what still differs; next: ..."`, and stop.
   Report it as parked; the next session starts from that C.
5. **Validate and commit:**
   `tools/dk python3 tools/scripts/decomp.py done NAME --commit --note "<one line: what made it match>" --trailer "Co-Authored-By: <your model attribution>"`
   If `done` refuses, fix the reason it gives (scope, lint, hack, function, or
   full build). Never commit a match with plain `git commit`, and never use
   `--no-verify`.
6. **Report** the commit hash, the function, and anything learned. Put an
   idiom or compiler quirk in the Diff patterns table in docs/MATCHING.md;
   update `docs/AGENT_LOG.md` (Current state, Open leads for a park, and the
   session's Session log entry).

One function per invocation. To do several, invoke the skill again for each.
Each function gets its own commit.
