# Agent log

Read this first. It holds the current state, what to do next, open leads and
the gotchas that still bite. Rules that apply to every match belong in
[MATCHING.md](MATCHING.md) (the Diff patterns table), facts about the game in
[ARCHITECTURE.md](ARCHITECTURE.md), and repo rules in
[CONVENTIONS.md](CONVENTIONS.md): move them there rather than here.

At the end of a session, update **Current state**, **Next** and **Open
leads**, add any new gotcha, and add one short entry at the top of the
**Session log**.

## Current state (2026-10-11)

- Phase 2 of [ROADMAP.md](ROADMAP.md): matching `main.exe`. Phase 1 (types
  and names) is done.
- 421/2550 game functions matched (main.exe 175/574), 3 parked. `make check`
  OK. Live figures: `decomp.py progress` / `docs/index.html`.
- Compiler: gcc 2.8.1-psx `-O2 -G0`, maspsx `--aspsx-version=2.86` (settled;
  2.7.2-cdk matched a strict subset).
- PsyQ 4.7: Sony's objects are linked in place of the library; the gaps are
  in `src/main/psyq/unknown/`.

## Next

1. Keep matching `main.exe`, one function per commit:
   `decomp.py next --seg main/main` (not always the smallest). Use the
   MATCHING.md process: evidence, Diff patterns, checklist, cc1 dump, then
   permuter.
2. Name each function as it's matched when the evidence allows (only 144 of
   2550 are named).
3. Unpark when a new idea comes up (see Open leads).
4. Later Phase 2 items: split `main.c` into units, data to C, the PsyQ gaps,
   the `CRASHBSH.DAT` extractor.

## Open leads

Parked (`decomp.py block`; best C under `#ifndef NON_MATCHING` in
`src/main/main.c`):
- **`func_800155FC`** (seed step `A += B; B += A + ((u32)A >> 31)`): 2
  operands off. The target ties `A + srl` to the srl's register; writing the
  srl first ties it right, but then local-alloc gives A v1 instead of v0.
  Tried 17 forms (temps, operand order, `u32` globals, store order).
  `Random` (0x80015590) is the same step plus `divu` (unsigned `%`): match
  them together.
- **`func_8001C7FC`**: `return func_8001A0D8(...)` (s32) fixes the division
  region (the returned pseudo prefers v0, so `mfhi` lands in v1), but then v0
  is live at the shared exit and gcc leaves the first `beqz` delay slot
  empty (the target has `lui v0,0x200` there). Look for a source form where a
  pseudo prefers v0 without v0 being live on the early exit.
- **`func_800177F4`**: 24 register operands off (colour pointer in v0, target
  v1; tpage/`0x02000000` constants swapped a0/v0); control flow matches.
  Permuter's best reused a temp for `tpage & ~0x60`.

Other:
- `func_800C8BA0` (ovl5f) matches alone but adds 4 bytes of data/rodata to
  the overlay (offset 0x4C): its literal belongs in another file's rodata.
- Tank (`ovl5f`, probably): find its AST ids another way (strings, the
  overlay's own tables) before naming anything.
- `func_8001A0D8` (blocks `func_8001C7FC`) stores SXY at +0x10/+0x18/+0x20
  too: use `gte_stsxy3_10`.
- Ambiguous naming leads are listed in ARCHITECTURE.md ("Functions reached
  from the AST pairs").

## Gotchas

**Tooling**
- Everything runs through `tools/dk` (Docker); the host has no Python.
- In shell loops, give `tools/dk` `</dev/null` (it runs `docker -i`). But
  never with a heredoc: `tools/dk python3 - <<EOF ... </dev/null` silently
  runs nothing.
- Git Bash heredocs collapse `\\` to `\`: write Python with escapes using
  the Write tool.
- `make clean`/`make verify` wipe `build/` (including `build/objdiff/asm`; rerun
  `make objdiff-config`). Keep scratch scripts in `disc/scratch/`.
- `tools/dk` right after `git checkout` sometimes hits a Windows
  `PermissionError`; retry.
- `make permute` runs in its own container: stopping the background task
  doesn't stop it. `docker stop` it before running `done`.

**decomp.py**
- `done` refuses when another function in the file is also un-`INCLUDE_ASM`ed.
  For a batch: keep the full file in `disc/scratch/`, `git checkout` the
  source, apply one function (`disc/scratch/apply_one.py FULL SRC SEGDIR
  FUNC`), `done`, repeat.
- `done`'s scope check compares names: commit renames before matching.
- `make diff` shows at most 40 rows. For the whole diff, or to swap a draft
  in and diff in one step: `tools/dk python3 disc/scratch/fulldiff.py FUNC`,
  `sh disc/scratch/tryf.sh FUNC FILE.c [LINES]` (main.c only).
- Register pins pass the lint as hacks (`--hack`) since `ef8fdc9`; before,
  `asm("$20")` was misread as inline asm.
- Renaming a function to a leak-declared name and back loses its owned
  declaration; re-add it by hand.
- `rename` prints `ERROR: unknown function OVL:ADDR` for data symbols; the
  rename still works.
- `functions.json` names can lag `sym.*.txt` renames; don't trust them for
  file paths.

**splat and the build**
- splat only writes `.s` files for functions a C file `INCLUDE_ASM`s, and never
  rewrites `src/`. After adding symbols to an overlay with no C yet, delete
  its scaffold `.c` and re-split. `make check` won't notice a stale object
  (`.c.o` doesn't depend on the `.s`): use `make verify` after any re-split.
- Packed segments need `align: 4` on every segment in the file.
- `make verify`'s splat "Unable to determine a segment" errors for
  `funcEvSpError`/`funcEvSpErrorx` are harmless.

**Matching and types**
- 185-odd `D_` globals used by unmatched exe code are still undeclared: the
  match that first uses one declares and types it in the owned header.
- Matched-but-mistyped functions (`void f(void)` from the old `auto`) are the
  likeliest type stalls: retype them when a caller needs it.
- splat marks functions with cop2 moves as `/* Handwritten function */`; they
  are ordinary C with Sony's GTE macros. m2c can't draft them.
- `bool` is 32 bits. `NAS+0x58` is a per-object pointer (`EXTobject` for
  Arkenoid balls, other types elsewhere): leave it `void *`.
- The leak's prototype order follows code order inside a file, so the
  neighbours of a named function are good naming candidates.
- AST entry: `{id, Init, Plot}`, `0x80026D00` is the empty stub; pointers
  below `0x800B32B4` are engine functions.
- The GTE table's `ll`/`lc`/`sqr` entries come from encodings, not a retail
  match: if one mismatches by a bit, check `tools/scripts/dmpsx.py` first.
- Grepping asm for `4A`/`4B` opcode bytes also hits ASCII in data ("MARK").

**Scripts** (`disc/scratch/`, not committed): `callsites.py OVL` (what each
call passes, whether the result is used), `callgraph.py spawns`,
`nas_fields.py ARGREG FUNC...` (field offsets through a pointer argument).
`tools/scripts/field_survey.py` (committed) counts loads/stores per struct
offset. Most-called functions:
`grep -rhoE "jal +func_800[0-9A-F]{5}" asm/nonmatchings | sort | uniq -c | sort -rn`.

## Session log

Newest first, a few lines each. Details are in the commits.

- **2026-10-11:** Matched `func_800193A8` (692 bytes, GTE triangle strips;
  register pins, `--hack`) through the new process; six Diff patterns rows.
  Fixed the gate lint so pins count as hacks.
- **2026-10-11:** MATCHING.md gained the evidence step, the Diff patterns
  table, per-kind checklists and the cc1 dump recipe; the skill's step 4 now
  requires them before the permuter. Parked `func_800155FC`. Matched
  `func_8002AABC` (sound init) on the first diff.
- **2026-10-11:** Matched 10 small exe functions (pool free-lists, bit
  set/clear loops, `AddExtension`, list length, ...). Root-caused
  `func_8001C7FC` with cc1 dumps (still parked). Dashboard: progress over time
  per unit. Unparked `func_80015780`; GTE batch (5 functions).
- **2026-10-10:** Phase 2 started: Sony's GTE macros in the sources
  (`dmpsx.py`), first GTE matches. Phase 1 finished: engine types (`NAS`,
  `EXTengine`, `Hero`, ...), core names and globals, owned headers per source
  file, PsyQ headers, leaked headers in `include/bash/`, AST Init/Plot names,
  objdiff config fixed.
- **2026-10-10:** Hand matching only (`done` gate, pre-commit hook; `auto`
  retired) after an auto pass to 391 functions. Overlays found and split
  (`engine` + 14 slot overlays at `0x800B32B4`). Project set up from the
  `psx-matching-decomp` skill.
