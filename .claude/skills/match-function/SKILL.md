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
2. **Write C** in the file that `next` printed (`src ...`), replacing that
   function's `INCLUDE_ASM` line only. Rules:
   - C89: declarations at the top of blocks, `/* */` comments, `s32`/`u8`/... types.
   - Declare what the function uses in the owned header of the file that defines
     it (`bash/main.h`, `bash/engine.h`, `bash/ovlXX.h`, marked `owner:`), or in
     the original headers (`bash/pogo.h`, ...) if they already name it. `done`
     adds the function's own prototype. See docs/CONVENTIONS.md (Headers).
   - No `M2C_*` macros, no `?` types, no `asm()` with instructions.
   - Prefer struct fields over pointer arithmetic once the layout is known.
3. **Diff:** `tools/dk make diff FUNC=NAME`, then iterate. Typical fixes:
   statement order, signed/unsigned, `s16`/`s32` locals, temporaries, `do {} while`
   instead of `for`, pointer increments instead of indexing.
4. **Before the permuter.** Use `make permute` only after all of these:
   - the evidence step from docs/MATCHING.md ("Evidence first": `decomp.py dups`,
     grep matched C in `src/` for the same globals or shape, the headers in
     `include/bash/` and the leaked minigame headers);
   - the "Diff patterns" table and the checklist for this kind of diff
     (register-only, instruction order, or opcode/length), noting in your
     reasoning which items you tried;
   - for register differences, the cc1 dump ("Why does a register differ?").
   Then run `tools/dk make permute FUNC=NAME MINUTES=10`. When it finds
   something, port it by hand and add the general rule as a row in the
   "Diff patterns" table. If it still doesn't match, restore the `INCLUDE_ASM` line, keep the attempt
   under `#ifndef NON_MATCHING ... #endif`, run
   `decomp.py block NAME --note "what still differs"`, and stop. Report it as parked.
5. **Validate and commit:**
   `tools/dk python3 tools/scripts/decomp.py done NAME --commit --note "<one line: what made it match>" --trailer "Co-Authored-By: <your model attribution>"`
   If `done` refuses, fix the reason it gives (scope, lint, hack, function, or
   full build). Never commit a match with plain `git commit`, and never use
   `--no-verify`.
6. **Report** the commit hash, the function, and anything learned. Add
   anything reusable (an idiom, a struct, a compiler quirk) to the
   session's `docs/AGENT_LOG.md` entry.

One function per invocation. To do several, invoke the skill again for each.
Each function gets its own commit.
