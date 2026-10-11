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
- 444/2550 game functions matched (main.exe 189/574), 4 parked. `make check`
  OK. Live figures: `decomp.py progress` / `docs/index.html`.
- Compiler: gcc 2.8.1-psx `-O2 -G0`, maspsx `--aspsx-version=2.86` (settled;
  2.7.2-cdk matched a strict subset).
- PsyQ 4.7: Sony's objects are linked in place of the library; the gaps are
  in `src/main/psyq/unknown/`.

## Next

1. The core API list is done (all matched or parked with prototypes).
   Next: the 9 functions with identical copies (`decomp.py dups`).
   A function that parks still gets its prototype in `bash/main.h` (an
   ordinary commit after `make check`), as `FindExtension` did.
2. Keep matching `main.exe`, one function per commit:
   `decomp.py next --seg main/main` (least work first: near-miss drafts from
   the old `auto`, weighted by identical copies, then size). Use the
   MATCHING.md process: evidence, Diff patterns, checklist, cc1 dump, then
   permuter.
3. Name each function as it's matched when the evidence allows (only 144 of
   2550 are named).
4. Unpark when a new idea comes up (see Open leads).
5. Later Phase 2 items: split `main.c` into units, data to C, the PsyQ gaps,
   the `CRASHBSH.DAT` extractor.

## Open leads

Parked (`decomp.py block`; best C under `#ifndef NON_MATCHING` in
`src/main/main.c`):
- **`func_800155FC`** (seed step `A += B; B += A + ((u32)A >> 31)`): 2
  operands off. The target ties `A + srl` to the srl's register; writing the
  srl first ties it right, but then local-alloc gives A v1 instead of v0.
  Tried 17 forms (temps, operand order, `u32` globals, store order).
  `Random` (matched, `4adea44`) has the same step and matches with the
  plain `A += B; B += A + ((u32)A >> 31);`, which here is the 2-off form:
  this function's source is written differently, not just compiled in
  another context.
- **`SpawnNASById`**: 4 operands. The free-list pop's next pointer is in s0
  (reusing `nas`, as the permuter found) where the target uses a1; a fresh
  temp, reusing `eng`, or a `static inline AllocNAS` all give 34 lines.
  `SpawnNASFromRecord` has the same three sequences: match it next and
  compare. Prototype is in `bash/main.h`.
- **`func_8001C7FC`**: `return func_8001A0D8(...)` (s32) fixes the division
  region (the returned pseudo prefers v0, so `mfhi` lands in v1), but then v0
  is live at the shared exit and gcc leaves the first `beqz` delay slot
  empty (the target has `lui v0,0x200` there). Look for a source form where a
  pseudo prefers v0 without v0 being live on the early exit.
- **`func_800177F4`**: 24 register operands off (colour pointer in v0, target
  v1; tpage/`0x02000000` constants swapped a0/v0); control flow matches.
  Permuter's best reused a temp for `tpage & ~0x60`.

Other:
- `D_8004E0F0` is the main `Heap` (`bash/main.h`) but is still declared
  `u8[]`, so `func_800110A8`'s callers warn. Retype it together with the
  `u8 *` heap parameters of `func_80011DB4` and `func_800112A4`.
- `func_800C8BA0` (ovl5f) matches alone but adds 4 bytes of data/rodata to
  the overlay (offset 0x4C): its literal belongs in another file's rodata.
- Tank (`ovl5f`, probably): find its AST ids another way (strings, the
  overlay's own tables) before naming anything.
- `func_8001A0D8` (blocks `func_8001C7FC`) stores SXY at +0x10/+0x18/+0x20
  too: use `gte_stsxy3_10`.
- Sound voices: `SfxActiveVoices` (`0x80059614`) is `SfxVoicePool.active`
  (+0x544). Drop the separate symbol and retype the matched voice walkers
  (`func_800227C0`, `func_800227FC`, `func_80022838`, `func_800228A4`) with
  `SfxVoice`. `func_80022504` is "stop all" (`StopSfx` inlined in a loop),
  `func_800228C4` unlinks without stopping, `func_80022924` allocates,
  `func_80022984` inits the pool.
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
  FUNC`), `done`, repeat. `done` also applies the same C to byte-identical
  copies in the file and commits them together: skip those (apply_one asserts).
- `done`'s scope check compares names: commit renames before matching.
- `make diff` shows at most 40 rows. For the whole diff, or to swap a draft
  in and diff in one step: `tools/dk python3 disc/scratch/fulldiff.py FUNC`,
  `sh disc/scratch/tryf.sh FUNC FILE.c [LINES]` (main.c only).
- Giving a core function its prototype can break old `auto` matches that
  call it with no arguments (m2c dropped a pass-through `a0`). Pass what
  the argument register holds at the call; the code doesn't change
  (`KillNAS((NAS *)arg0)` in six overlay Init functions).
- A new prototype can also change a matched caller's code through the
  type it assigns to (`D_800C5BB0` had to become `NAS *`). Run `make check`
  after adding one.
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

**Progress check:** `tools/dk python3 tools/scripts/check_progress.py` recounts
every unit from the committed sources, prints functions % and bytes % side by
side (the dashboard's headline is bytes), and checks `docs/progress.json`.

**Scripts** (`disc/scratch/`, not committed): `callsites.py OVL` (what each
call passes, whether the result is used), `callgraph.py spawns`,
`nas_fields.py ARGREG FUNC...` (field offsets through a pointer argument).
`tools/scripts/field_survey.py` (committed) counts loads/stores per struct
offset. Most-called functions:
`grep -rhoE "jal +func_800[0-9A-F]{5}" asm/nonmatchings | sort | uniq -c | sort -rn`.

## Session log

Newest first, a few lines each. Details are in the commits.

- **2026-10-11:** Matched `func_80016AD4` (116 bytes, first diff): unloads
  what `func_80016C18` loads; `func_80012F04` takes one argument. Unparked
  `FindExtension`: a label inside the first `return` block, `goto` from the
  second (Diff patterns row updated).
- **2026-10-11:** Matched nine 32-byte wrappers like `func_800C6828` (eight
  in ovl5f, `func_800BCA18` in ovl65): pass `a0` through with a constant.
  `SetHeroSeq`/`SetHeroAction` prototypes (`Hero *`) in `bash/engine.h`.
- **2026-10-11:** Matched `func_800110A8` (508 bytes): the heap's free
  (merge with free neighbours, sorted insert). New `MemBlock`/`Heap` types.
  Finishes the core API list. Two new Diff patterns.
- **2026-10-11:** Matched `func_80025FAC` (156 bytes, first diff): selects
  sequence `n` of an `EXTanim` (`seq = seqs[n]`) and resets its state by kind.
- **2026-10-11:** Matched `func_8002C44C` (60 bytes) with `func_8001D604`'s
  C; `D_8006557C` is an `s32`, not an array.
- **2026-10-11:** Matched `func_800250B0` (248 bytes): runs a NAS's id-0x400
  `EXTanim` (new type) with `func_800251A8`/`func_80025534` by kind. Callees
  take `nas` through; ten overlay callers now pass `nas` (headers too).
- **2026-10-11:** Matched `func_80026A54` (132 bytes) with `func_80026980`'s C
  on the +0x28 list (`D_8005AA24`), first diff.
- **2026-10-11:** Matched `func_80026980` (132 bytes): unlinks a NAS from the
  list at +0x38/+0x3C headed by `D_8005AA34`. `NAS` gains `prev28`/`next28`
  and `prev38`/`next38`. Two new Diff patterns (head pointer local, `a || b`
  outer test).
- **2026-10-11:** Matched `func_80025A48` (156 bytes): finds the engine's
  `FlagRecord` with a flag (its own at +0x84, then the ones after a
  `PointList`'s points). New `FlagRecord` type, `PointList.nRecs`. Block
  layout came from joining the NULL checks with `&&` (Diff patterns).
- **2026-10-11:** Matched `get_angle` (752 bytes, `s32 get_angle(s32 x, s32 z)`,
  0x1000 per turn) on the second diff: the draft only had one quadrant's
  branch inverted; writing the `x >= z` case first there fixed it.
- **2026-10-11:** Started the core API (most-called unmatched functions,
  ~3,000 call sites; plan in Next): matched `Random` on the first diff,
  parked `FindExtension` (branch layout) but added its prototype; matched
  `StopSfx` on the first diff with new `SfxVoice`/`SfxPool` types, and
  `PlaySfx` (352 bytes; allocation written out in place, one final return),
  and `KillNAS` (388 bytes; `NASSpawn` typed, six callers fixed). Parked
  `SpawnNASById` at 4 operands with its prototype. Dashboard now shows
  function % beside byte %; new `check_progress.py`.
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
