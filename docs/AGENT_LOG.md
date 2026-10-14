# Agent log

Read this first. It holds the current state, what to do next, open leads and
the gotchas that still bite. Rules that apply to every match belong in
[MATCHING.md](MATCHING.md) (the Diff patterns table), facts about the game in
[ARCHITECTURE.md](ARCHITECTURE.md), and repo rules in
[CONVENTIONS.md](CONVENTIONS.md): move them there rather than here.

At the end of a session, update **Current state**, **Next** and **Open
leads**, add any new gotcha, and add one short entry at the top of the
**Session log**.

## Current state (2026-10-14)

- Phase 2 of [ROADMAP.md](ROADMAP.md): matching `main.exe`. Phase 1 (types
  and names) is done.
- 780/2551 game functions matched (main.exe ~514/575, 60 left), 8 parked,
  all 564 PsyQ functions (Sony objects), 5 parked.
  `make check` OK. Live figures: `decomp.py progress` / `docs/index.html`.
- Compiler: gcc 2.8.1-psx `-O2 -G0`, maspsx `--aspsx-version=2.86` (settled;
  2.7.2-cdk matched a strict subset).
- PsyQ 4.7: all library code is Sony's objects (281, plus `2MBYTE.OBJ`), and
  their `.data` comes from the objects except `bios_1.o`'s (one value differs;
  `configs/USA/psyq/data_keep.txt`). Roadmap: the PsyQ gaps box is ticked.
  The game is built with `-fno-builtin` (it calls `abs`).
- Every type and AST id in the Oct 9 prototype's debug fragment is in
  `include/bash/` and every overlay is identified (ARCHITECTURE.md, Prototype
  debug types). The disc images are in the git-ignored `protos/`.

## Next

0. Current focus (ROADMAP.md, Current focus): `make disc` and `make
   replay` work; the matching build is frame-identical to retail. The
   harness's next step is a state-sequence mode (Phase 4; frame-exact is
   too strict for builds whose code changes size), wanted by the first
   shiftable `NON_MATCHING` build. Recordings stay short and hand-written
   (`<frames> [buttons]`, check with `replay.py run ... --shot N`).
1. More swarm runs: `swarm.mjs run --branch main --seg main/ --max-minutes
   60 --max-functions 300 --max-run-usd 80 --big-bytes 400 --workers 1
   --big-workers 3`. 400-599 B
   matches 57% in normal mode and big mode does better, hence
   `--big-bytes 400`; 17 functions of that size have never been leased
   (`func_80010C70`, `func_80010E8C`, `func_8001231C`, `func_80013D54`,
   `func_80017164`, `func_80017EE8`, `func_8001A82C`, `func_8001D6B4`,
   `func_8001E21C`, `func_8001E610`, `func_8001F604`, `func_8001FE80`,
   `func_800235BC`, `func_8002854C`, `func_80028774`, `func_80028D40`,
   `func_80029560`). The 2.7.2 functions the load fix unblocked were reset
   with `swarm.mjs forget`, so no `--retry-failed` is needed. After each
   run, the checklist at the end of tools/swarm/README.md (`stats`, parks,
   `swarm.mjs fold`, this log).
2. Split `main.c` into its translation units, between swarm runs (a split
   moves functions, so workers' patches and big-mode checkpoints for them
   stop applying). Find the boundaries first and write the evidence in
   ARCHITECTURE.md: rodata order, alignment padding, file-local statics,
   changes of compiler style (as at `0x8002D488`). Doing this before the
   rest of the matching helps it: code in the wrong unit can't match, and
   the rodata batch failures came from `main.c`'s single rodata block.
3. Keep matching `main.exe` (swarm runs, alternating with split steps). By
   hand: `decomp.py next --seg main/`, the MATCHING.md process (check the
   unit, evidence, STAGE by STAGE, Diff patterns, cc1 dump, then permuter).
   No `main.exe` function left has an identical copy (`decomp.py dups`:
   every main member of a group is matched). A function that parks still gets its prototype in its owned
   header (an ordinary commit after `make check`).
4. Name each function as it's matched when the evidence allows (345 of
   2550 are named). Unpark when a new idea comes up (see Open leads).
5. Later Phase 2 items: data to C (after the split), naming and typing
   passes, and the `CRASHBSH.DAT` extractor (independent: any time).

## Open leads

Parked (`decomp.py block`; best C under `#ifndef NON_MATCHING` in
`src/main/main.c`):
- **`func_80020C04`** (lerp on the `D_80068BD4` trig table, no callers):
  retail emits `mult` with no `mflo` and returns `a`. In 2.8.1 every dead
  product form also drops the `mult`; signed `>> 32` keeps one `mflo` (19 vs
  18). Next: cc1 `-dc`/`-dl` dumps of that form, or test whether this region
  is another compiler.
- **`func_800155FC`** (seed step `A += B; B += A + ((u32)A >> 31)`): 2
  operands off. The target ties `A + srl` to the srl's register; writing the
  srl first ties it right, but then local-alloc gives A v1 instead of v0.
  Tried 17 forms (temps, operand order, `u32` globals, store order).
  `Random` (matched, `4adea44`) has the same step and matches with the
  plain `A += B; B += A + ((u32)A >> 31);`, which here is the 2-off form:
  this function's source is written differently, not just compiled in
  another context.
- **`func_80026D08`** (rebuilds `NASFreeList` from the pool): 18 off. The
  target hoists `%hi(NASFreeList)` out of the loop (a copy into a1, so
  loop.c ran) but keeps the loop test at the top, unrotated. A `goto` loop
  gets no hoisting; `for (;;) { if (i < first) break; ...}` hoists it but
  adds a `nas + 12` induction pointer (37 insns vs 36). The `KillNAS` push
  may be a `static inline FreeNAS()`, but that alone changed nothing.
- **`func_80011E80`** (block after `heap->lowest` unless owner -1, size 0
  or flag 2): 4 operands, all in the tail. The target keeps the result in v0
  and the block in a0; the m2c result-variable form, early returns and
  clearing to NULL all add 1-2 insns. Try `static inline` helpers for either
  half (they fixed `func_80021E98`).
- **`func_8001C7FC`**: `return func_8001A0D8(...)` (s32) fixes the division
  region (the returned pseudo prefers v0, so `mfhi` lands in v1), but then v0
  is live at the shared exit and gcc leaves the first `beqz` delay slot
  empty (the target has `lui v0,0x200` there). Look for a source form where a
  pseudo prefers v0 without v0 being live on the early exit.
- **`func_800177F4`**: 24 register operands off (colour pointer in v0, target
  v1; tpage/`0x02000000` constants swapped a0/v0); control flow matches.
  Permuter's best reused a temp for `tpage & ~0x60`.

Other:
- The gcc 2.7.2 units (`main/8002d488`, `main/8002e0f0`): `aspsx_fill.py`
  fills `j`/`jr` slots and, since 2026-10-14, expands indexed symbol loads
  through the load's register as ASPSX does (that unblocked and matched
  `func_8002E0F0` and `func_8002E000`). Left: `func_8002E3D0` (436 B; big-mode
  checkpoint structure 107/111; gcc
  if-converts `v = 0; if (bit) v = arg2;`), `func_8002DE2C` (468 B; an
  extra `addu a3,v1,zero` copy in the target's first `bne` slot),
  `func_8002D4F4` (2360 B, never tried), `func_8002E25C` (parked, below).
  Swarm notes on them before 2026-10-14 blame the `$at` loads; ignore
  those. If a `jal` slot differs, extend `aspsx_fill.py` from the target.
  Whether the two units are one file is open.
- **`func_800274FC`** (parked; split out of `func_80027408`, now matched):
  registers 4/40. It's `func_800276C4`'s body plus `D_8005B698 = NULL`;
  the target loads the last `*arg0` into a0 and the global's `lui` into v1
  (so sched1 put the `lui` before the load), ours v1 then v0. Next: the
  cc1 sched1 dump.
- **`func_8002E25C`** (parked, the debug text printer): structure 68/93 under
  2.7.2. The target reloads `*str` in the Kanji and ROM-glyph paths but
  compares the first load for `' '`; ours reuses it. Find what stops 2.7.2's
  CSE there (a char local, `volatile` and an index loop didn't).
- Swarm: `--seg` is a prefix and defaults to `main/main`, so the new units
  need `--seg main/8002` (or `--only`).
- Heap: `D_8004E0F0` is a `Heap` now. Its +0x10 is the deferred-free list
  (`func_80011578` pushes, `func_80011DB4` counts down); `MemBlock.unk2` is
  the countdown. The heap functions still in asm (`func_800107C8`,
  `func_800115D8`, `func_80010C70`) need prototypes taking `Heap *`.
- `func_800C8BA0` (ovl5f) matches alone but adds 4 bytes of data/rodata to
  the overlay (offset 0x4C): its literal belongs in another file's rodata.
- Prototype debug types (ARCHITECTURE.md, Prototype debug types): every type
  in the Oct 9 fragment is in a header now, and every single-id AST
  Init/Plot is named. Use them while matching: type `NAS` fields, `FUX`
  extensions and globals with them instead of offsets, as the four Tank
  Inits (`InitMineBall`, ...) now are. 34 fields of 15 types
  whose bodies were on lost pages are word or byte arrays; retype them if code
  shows their fields.
- `func_8001A0D8` (blocks `func_8001C7FC`) stores SXY at +0x10/+0x18/+0x20
  too: use `gte_stsxy3_10`.
- Sound voices: the C walkers use `SfxVoicePool.active`; the symbol
  `SfxActiveVoices` stays only for the asm (`func_800228C4` and data). Drop it
  from `sym.main.txt` once nothing in asm uses it. `func_80022504` is "stop all" (`StopSfx` inlined in a loop),
  `func_800228C4` unlinks without stopping, `func_80022924` allocates,
  `func_80022984` inits the pool.
- Ambiguous naming leads are listed in ARCHITECTURE.md ("Functions reached
  from the AST pairs").

## Gotchas

**Tooling**
- **`make clean` (and `make verify`) delete `build/`** except
  `build/scratch/` (keep drafts and notes there), `build/emu/` and
  `build/disc-cache/`.
- **The build tracks its pipeline:** C objects depend on `dmpsx.py`,
  `aspsx_fill.py`, `project.mk` and the Makefile, and overlay linker scripts
  on their own `sym.<ovl>.txt` and `sym.engine.txt`, so editing those
  rebuilds or re-splits. `decomp.py` takes `CPPFLAGS`/`ASFLAGS` from the
  Makefile (`make print-VAR`). `try` prints `STILL-ASM` for a function whose
  INCLUDE_ASM is still in place instead of a self-match.
- **Swarm clones and splat output.** Clones get `asm/` and `linkers/` copied
  from this checkout; since 2026-10-14 they are copied again when
  `configs/USA`'s sym/rel files or yamls change (a fingerprint in the clone's
  `.git/swarm-data`). Run `make split` here after such a change, before a
  swarm run: the clones copy this checkout's `asm/`. Before the fix, a symbol
  split made every gate fail ("can't open asm/nonmatchings/.../func_X.s").
- **Toolchain image tag.** `tools/dk` and `tools/dk.ps1` tag the image
  `psx-decomp:<git hash-object of the Dockerfile, 12 chars>`, so a Dockerfile
  change builds a new image by itself; nothing to bump.
- **Replay recordings.** Menus ignore a press while they animate in: hold a
  button ~4 frames and leave a gap. Find frame counts with `replay.py run
  ... --shot N` and look at the PNGs (tools/replay/README.md).
- Splitting a function by adding a symbol (`name = 0x...; // type:func` in
  `sym.main.txt`, no `;` in the comment): run `decomp.py index`, which adds
  the new function but keeps the old one's `size` in
  `.decomp/functions.json`. Fix that by hand, or `make diff` compares
  against the old length.
- A unit whose last function ends in padding `nop`s (and rodata ending in
  zero padding) comes out short once that function is C: add a splat
  `[offset, pad]` subsegment after it (`configs/USA/main.yaml` has the two
  for `main/8002e0f0`).
- The swarm refuses any patch that adds inline asm, even a GTE macro, and a
  big-mode function then parks as "no progress" though its sessions said
  MATCH. Read the parks' `result.json` notes after a run: `func_8001AA48`
  was such a case, committed by hand with its macro (`gte_ldv0_t`) next to
  `gte_ldv0_w` in `include/bash/main.h`, the precedent for game-specific
  GTE macros.
- The swarm: start it with the Bash tool's `run_in_background`, not `&`, and
  wait for `run <id> finished` in `swarm.log` (or the pid gone, by
  `tasklist //FI "PID eq N"`; `process.kill(pid, 0)` doesn't work for it on
  Windows). Don't wait on a bare `fatal`: git errors quoted in the log
  contain it. `swarm.log` timestamps are UTC, an hour behind local time.
- Git Bash heredocs can fail outright ("unexpected EOF while looking for
  matching quote") on long scripts with backticks and quotes: write the
  script with the Write tool and run it with `tools/dk python3 build/x.py`.
- Before 2026-10-13, `try` masked every relocated word, so a `j` to the
  wrong label inside the function still "matched alone" and only the full
  build failed. Local jumps are compared now; a "matches alone, full build
  doesn't" refusal from before then is worth a fresh `try`.
- Everything runs through `tools/dk` (Docker); the host has no Python.
- In shell loops, give `tools/dk` `</dev/null` (it runs `docker -i`). But
  never with a heredoc: `tools/dk python3 - <<EOF ... </dev/null` silently
  runs nothing.
- Git Bash heredocs collapse `\\` to `\`: write Python with escapes using
  the Write tool.
- A C error does not stop the build: the Makefile pipes `cc1` into maspsx
  and `as`, so the pipeline's status is `as`'s and a partial object gets
  linked (a missed field rename showed up only as a 20-byte-short exe). After
  header changes, grep the build log for `stdin:N:` lines that are not
  warnings.
- `tryf.sh` replaces only the function, so a comment line above it in the
  draft file is added again on every run. Keep comments out of drafts.
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
- **Near-misses at the registers stage:** `swarm.mjs permute --only ...`
  before another Opus session. Checkpoints now: `func_8001795C` registers
  12/107, `func_800248A0` 5/142 (the permuter didn't move it in 25 min),
  `func_80015F60` 2/89.
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

- **2026-10-14 (tooling: regions):** new `decomp.py regions NAME`
  (tools/scripts/regions.py, tests in tools/scripts/tests): aligned,
  block-by-block score with C lines. Re-scored 22 saved swarm patches:
  it localises (`func_80028F28`: 6/7 blocks ok, STAGE said structure
  59/118) and `paired` spotted a near-structure version STAGE called a
  regression (`func_800248A0` 110/144 -> paired 140/144); register
  cascades still mislead both. Not yet in the swarm; next: an A/B of big
  sessions with and without it.
- **2026-10-14 (by hand, roadmap order):** `func_80020C04` (main) parked
  at `structure` 18 vs 19 after compiler experiments (Open leads); the
  permuter was stopped early when the session ended.
- **2026-10-14 (by hand, 1 function):** `func_800C8870` (ovl5f) matched first
  try: an `s32` grid lookup `D_800CA34C[(y >> 9) * D_800CAFF4 + (x >> 9)]`,
  the same shape as its `u8` neighbours `func_800C891C`/`func_800C8950`.
- **2026-10-14 (permuter only, ~25 min):** two parallel `decomp.py permute`
  runs (7 jobs each) from the swarm checkpoints. `func_8001795C`: registers
  17 -> 14/107 (`ti = ti & 0x1FF` inside the index; `r0 = r0 | 0x02000000`
  instead of `|=`), saved as its checkpoint. `func_800248A0` (5/142): no
  improvement. Now one command: `swarm.mjs permute --only f1,f2
  --minutes 30` (tools/swarm/README.md); its first test took
  `func_8001795C` from 14 to 12/107 in 3 minutes on 15 cores.
- **2026-10-14 (swarm, 10 min):** no match. `func_8001795C` reached the
  registers stage (17/107, checkpoint); `func_800248A0` stayed at registers
  5/142 (2 stale sessions: big mode won't lease it again; it wants a long
  permuter run or `forget`). `func_8002E3D0`'s session was hard-stopped at
  19 min and lost. Big sessions run 13-20 min, so a run shorter than ~20
  min mostly ends in a graceful wait or a hard stop that loses work.
- **2026-10-14 (swarm, 15 min):** `--big-bytes 400 --workers 1
  --big-workers 3` (with 400, 52 of 54 candidates are big: give big mode
  the workers). `func_8002E000` matched in 3.5 min after 4 earlier fails
  (the 2.7.2 load fix). Checkpoints: `func_8002E3D0` 107/111 and
  `func_8001795C` 106/108 (structure). $9.68. A first start died on
  `tools/dk`'s image tag (`git -C` with a /c/ path under Windows git.exe;
  fixed in `6dda356`).
- **2026-10-14 (tooling):** Make deps on the pipeline scripts and overlay
  sym files; `try` prints `STILL-ASM`; flags from the Makefile; image
  tagged by the Dockerfile's hash (old images deleted); `make clean` keeps
  `build/scratch`, `emu`, `disc-cache`; disc cache. Swarm: stops at the
  first environment error without charging the function; `fold` (after-run
  step 3) and `forget` (reset functions a fix unblocked: done for
  `func_8002E000`, `func_8002E3D0`, `func_8002DE2C`). `make test-tools`:
  12 unit tests of `dmpsx.py` and `aspsx_fill.py`.
- **2026-10-14 (tool fix):** `aspsx_fill.py` now expands indexed symbol
  loads through the load's own register, as ASPSX does (maspsx used `$at`),
  in the gcc 2.7.2 units. First win: `func_8002E0F0` matched by hand. The
  other functions those units blocked on it (`func_8002E000`,
  `func_8002E3D0`, `func_8002DE2C`, `func_8002D4F4`) are ordinary matching
  now; their swarm notes predate the fix. `done` now refuses a function
  whose INCLUDE_ASM is still in place (it "matched" itself: a bogus
  metadata-only commit, and the swarm's duplicate resumes).
- **2026-10-14 (swarm):** Two runs failed every gate: the clones' `asm/`
  predated the `func_800274FC` symbol split (fixed in `ed7dc7a`, Gotchas).
  Rerun with `--resume` of both: 9 matched in ~30 min (stopped early), $16;
  `--resume` gated three functions twice (metadata-only commits), now one
  attempt per function. 400-599 B matches only 57% in normal mode (12/21):
  consider `--big-bytes 400`. Parks: `func_80010000` (registers 11/86),
  `func_8002E3D0` (needed loads expanded through the dest reg in 2.7.2
  units: fixed the same day), `func_80020C04` (inline asm refused).
- **2026-10-14 (play and test docs):** `tools/replay/README.md` (packer,
  playing in RetroArch, harness, recordings, limits), linked from README,
  CONTRIBUTING (when to run `make replay`) and CLAUDE.md. RetroArch + the
  PCSX-ReARMed core installed on the host at `C:\RetroArch-Win64`.
  `replay.py --video` (MP4 with sound); memory cards pinned in `OPTIONS`.
- **2026-10-14 (replay harness):** `make replay`: `tools/replay/replay.py`
  drives the PCSX-ReARMed libretro core through ctypes (pinned in the
  toolchain image, now `psx-decomp:2`), traces each frame's image hash and
  `values.txt` (seeds, `Heroes` mode/health/score), compares two discs.
  Retail vs build: 7512 identical frames; a patched `Random` is caught at
  frame 1294. Deterministic run to run.
- **2026-10-14 (disc packer):** `make disc` / `make disc-check`
  (`tools/scripts/pack_disc.py`): dumpsxiso the retail image into
  `build/disc/`, put the built exe and overlays back, mkpsxiso it. The
  matching build packs to the retail image byte for byte (~20 s). Roadmap:
  packer ticked, a playable, testable build is now the current focus.
- **2026-10-14 (by hand):** Split `func_80027408` at `0x800274FC` (a
  `// type:func` line in `sym.main.txt`; it never returns) and matched it.
  After a split, `decomp.py index` keeps the old size: fix `size` in
  `.decomp/functions.json`. Parked `func_800274FC` and `func_8002E25C`.
- **2026-10-14:** 1-hour swarm run: 20 committed, $41. Its parks showed
  `main/8002e0f0` is gcc 2.7.2 (Makefile changed; `func_8002E618` then
  matched by hand, with `pad` subsegments for the unit's end padding).
  `func_80027408` needs a symbol split (Open leads).
- **2026-10-13 (night):** 1-hour swarm run: 28 committed, plus 2 by hand
  (`func_80017D90`: the gate's m2c `?` lint flagged a ternary, now fixed;
  `func_8001AA48`: refused for its GTE macro, see Gotchas), 1 real park
  (`func_8002E0F0`, Open leads), $34. Biggest swarm match yet:
  `func_8002B210` (680 B). `tools/dk` names containers
  (`swarm-w1-diff-func_X-n`, `DK_NAME`); patterns folded.
- **2026-10-13 (evening):** Committed `func_80025534` (600 B) from the
  swarm's gate-fix patch. `ScriptCursor` (`func_80024FA4`) is a partial
  view of `EXTanimator` (both 0x2C), so the call casts; merge the two types.
  Roadmap: a final test (clean rebuild, shifted build plays, frame-for-frame
  replay vs retail, modern GCC), with its tools placed where first needed
  (packer + replay harness Phase 2, shift build Phase 4).

- **2026-10-13:** First real big-mode run (stopped early, ~18 min): 10
  matches incl. `func_8001D894` (612 B) by the big worker in one session,
  0 conflicts, $9. `func_80025534` matched alone but failed the gate on
  rodata (Open leads). Fixed: workers based on the integration clone's HEAD
  during a batch, so a failed batch dropped the base a permuter needed (now
  the last pushed tip); `stats` counts first-session big matches as big.

- **2026-10-13:** PsyQ gaps closed. The 27 gap functions were Sony's own tiny
  4.7 objects that signatures can't tell apart (identical bytes, different
  call or global): `psyq_gaps.py` compares every object with each gap and
  checks the relocations against the exe's symbol and data addresses, and
  found one object for each. The three multi-object gap files were split per
  object, then `link-psyq` (now incremental) linked them all. `--data` cut
  the data blob around 32 objects' `.data` (bios_1's differs, kept from the
  exe). `func_80032F3C` is `abs`; the rename only matches with
  `-fno-builtin`, so the game was built with it (the whole build matches).
  The pre-commit hook ignores whole PsyQ gap files deleted by link-psyq.

- **2026-10-13:** Big functions. `try` prints a `STAGE` line (structure /
  instructions / registers + diff count). Swarm big mode: functions >=
  `--big-bytes` (600) or parked in normal mode go to big workers (opus,
  150-min sessions, $20 each) that resume from a per-function checkpoint
  (`checkpoints/<func>`), kept only when the `try` stage/score improves; parks
  after 6 sessions or 2 without progress; the permuter (60 min) only at the
  registers stage. `swarm.mjs stats`: outcomes by size (normal mode 93-97%
  under 400 B, nothing bigger tried yet). Tested with fake workers (resume,
  stale park, escalation). Docs: MATCHING.md loop refreshed (unit check,
  stages) + Big functions section; swarm README, CONTRIBUTING, README,
  CLAUDE.md and the skill updated.

- **2026-10-13:** Swarm 1 h: 58 matches, 2 parked, 0 conflicts, $25.
  Two batch builds came out 8/32 bytes long: rodata, not code (a string
  literal duplicating an `INCLUDE_RODATA` one; gcc's `.align 3` jump table);
  the gate sent both back and they matched (Diff patterns rows). Folded 54
  rows (46 after merging). Split `0x8002D488..0x8002E0F0` into
  `main/8002d488` (text and rodata), built with gcc 2.7.2-psx plus
  `aspsx_fill.py` for ASPSX's delay-slot filling; per-unit Makefile
  settings, read by `decomp.py` (`unit_cfg`). `func_8002D488` matched.

- **2026-10-13:** Swarm, 30 min, 3 workers: 43 matches, 0 conflicts (the
  ordered prototypes worked), 1 failed, $11.16. The failure,
  `func_8001251C`, "matched alone" because `try` masked the in-function `j`
  targets. `compare` now checks them (91 functions with local jumps
  re-checked, only that one was wrong), and it matched by hand with a
  top-tested `goto` loop. 29 more Diff patterns rows folded in.

- **2026-10-13:** Improvements, no swarm run. Folded 36 swarm rows into
  MATCHING.md (the swarm's `patterns.md` was renamed to
  `patterns.folded-2026-10-13.md`). `declare()` now inserts prototypes in
  address order into the run of prototypes at the end of each owned header
  (`sort_protos.py` sorted the existing runs once), so parallel matches
  conflict less. `D_8004E0F0` is typed `Heap` (21 fewer warnings), and the
  Sfx walkers are typed. The build pads PsyQ gap objects to 16 bytes, and the
  three gap files that held several objects are split, so `func_80033530`
  and `func_8003E0EC` are matched.

- **2026-10-12:** Built the swarm orchestrator on branch `swarm-orchestrator`
  (`tools/swarm/`, README there): parallel Claude workers in clones, one
  serial integrator through `done --commit`, stop switch (`stop`, `stop
  --now`, STOP file), patch checks, its own `try`, circuit breakers. Added
  `decomp.py queue` and `DK_DOCKER_ARGS` in `tools/dk`. Tested end to end
  with fake workers (replayed two past matches, hard and graceful stop,
  timeout, escalation, a cheating patch, a false match claim). Then: CLI
  installed (npm), workers use `dontAsk` and `--max-budget-usd`, run cost
  cap; `done`'s full build now runs `make -j` (149 s to 30 s, same bytes),
  so a swarm commit takes ~1 min. First real runs (1 worker, Sonnet,
  `func_80012FFC`): run 1 parked at once because `sed -i` was refused
  (the prompt now says to edit with Edit/Write and not chain commands);
  run 2 worked properly but parked after 11 min, 10 of them waiting on the
  permuter (score 805 to 300). So the permuter moved to its own pool:
  workers hand off `needs_permute`, a follow-up worker ports the result.
  `func_80012FFC` has used its 2 attempts (`--retry-failed` to retry).
  First full run (2 workers, 1 permuter, 4 functions, $0.74 notional):
  3 matched on `swarm/integration` (`func_800135E4`, `func_80013650`, and
  `func_80011878` by Opus after Sonnet + permuter: it returns the split
  block). `func_800263D8` matched but `done` refused it (struct declared in
  main.c, prototype in main.h): workers are now told types go in the owned
  header, and a gate refusal goes back to a worker once. Two Diff patterns
  rows are waiting in `crash_bash_decomp-swarm/patterns.md`.
  Swarm on main (`--branch main`, 3 workers): 14 commits in ~25 min, and
  workers verified matches far faster than the one-at-a-time gate could
  commit them. Added the batched gate (`done --defer-build` + one
  `decomp.py verify` per batch), permuter pause during gate builds,
  back-pressure (`--max-queued`) and `--resume RUN`. First batch: 5 of 6
  committed in ~4 min (~50 s each, not 6x: per-function `done` overhead
  dominates; next, skip `cmd_progress` per function in batch mode). The
  batch re-check caught `func_80015A18`, which matched on an older main but
  not today's. 11 verified matches are waiting:
  `swarm.mjs run --branch main --resume 20261012120212`.
  Then profiled the gate: `done` spent ~36 s on the dashboard
  (`sym_names()` called per matched function; `rodata_syms` re-parsing
  2800 .s files): now ~5 s (hoisted, cached in build/cache). And every
  build re-ran all 16 splat splits (splat leaves an unchanged .ld alone, so
  it stayed older than sym.main.txt); under `make -j` those ran
  concurrently and sometimes produced a short exe. The splat pass is now
  serial and touches the .ld: full build 149 s -> 8 s, 11 verifies in a
  row OK. A flaky build could only refuse a good match, never pass a bad one.
  Then: 10-min run 15 commits, 20-min run 29 commits (~85/hour), mostly
  limited by conflicts on the end of `bash/main.h`. Fixed in the swarm:
  union merge for headers in its clones, neighbours (within 8 lines) never
  leased together, and a conflicting patch is re-applied mechanically
  instead of going back to Claude. Next swarm idea, not done: `done` could
  insert prototypes in address order instead of appending (fewer header
  clashes, also for hand sessions); long term, split `main.c` into units.

- **2026-10-12:** Short follow-on run: matched `func_80013688` (wrapper of
  `func_80013034`) and `func_80011BC0` (tail-recursive walk to the next free
  heap block; the NULL return goes last). `func_80012FFC` (sum of
  `D_80050628` fields) is next in the queue, untried: start from the m2c
  draft with a local for `unkC != 0`.
- **2026-10-12:** One-hour run: 21 matches in `main.exe` (`func_8001361C`, `func_80012E28`,
  `func_8001C34C`, `func_80028158`, `func_80026FCC`, `func_80026D9C`,
  `func_8002120C`, `func_8001D5D0`, `func_8002D094`, `func_80021E60`,
  `func_80022984`, `func_8002B8F4`, `func_80016E5C`, `func_8001DD20`,
  `func_8002C060`, `func_80021E98`, `func_80011828`, `func_800138A4`,
  `func_800133C8`, `func_8002D360`, `func_8001316C`); parked
  `func_80011E80`. `GfxState.unk8C` is a `MATRIX`. Six new Diff patterns
  rows (an inlined pop helper again, increment and constant order, operand
  order, early global loads).
- **2026-10-12:** Parked `func_80026D08` (free-list rebuild, not a spawner;
  declared the pool globals `D_8005AA10/12/1C`). Matched `func_8001636C`
  by copying its matched sibling `func_800163E0`, then `func_80016450` (same
  record; the pointer must be a local or gcc folds the offsets).
  Then `func_8001C270` (`eng->kill(eng)`, called from `KillNAS`) and its
  twin `func_8001C298` (`eng->render(eng)`).
- **2026-10-12:** Matched `SpawnNASFromRecord` (504 bytes) and unparked
  `SpawnNASById` (512 bytes): both pop the free list through a
  `static inline AllocNAS()` with an early `return NULL` (an earlier inline
  attempt without the early return had failed). `decomp.py try` now takes
  `DIFF_ROWS` for longer diffs. The ternary `?:` trips the `?` lint: use an `if`.
- **2026-10-12:** Matched the rest of Tank's `D_800CB264` table:
  `func_800C8598` (add), `func_800C85D0` (clear, descending loop) and
  `func_800C84D8` (returns the slot within `dist` of x/z; local declaration
  order fixed a t0/a3 swap).
- **2026-10-12:** Matched `func_800C8560` (Tank, 56 bytes): removes a `NAS *`
  from the 8-slot table `D_800CB264` (siblings `func_800C8598` add,
  `func_800C85D0` clear, `func_800C84D8` distance test: next candidates).
- **2026-10-12:** Matched `func_8008EF9C` (engine, 56 bytes): sets `D_8009E5DC`
  from `func_8008EF70()` (a word from `D_8009EDD4` by `D_8005A676`), then the
  same `D_8005A644 = 0x8008` as `func_8008F068`, then `func_8008ECE4()`.
- **2026-10-12:** Matched `func_80095D88` (engine, 52 bytes): counts 0x24-byte
  records while the first word is >= 2; m2c's stride was in bytes. Diff
  patterns row.
- **2026-10-12:** Matched `PlotRaceFire` (ovl63, 32 bytes): `FindExtension(nas,
  0x10000000)` with `nas` passed through; `ovl63.c` now includes `main.h`.
- **2026-10-12:** Matched `func_8008F068` (engine, 28 bytes): two global
  stores; `D_8005A644` is `u16` (its 0x8008 loads with `ori`). Diff patterns row.
- **2026-10-12:** Rewrote the four m2c Tank Inits (`InitTankBossCortexTurret`,
  `InitBoatField`, `InitShellExplosion`, `InitMineBall`) with `NAS`,
  `EXTengine` and `subengine` fields; typed 383 -> 387.
- **2026-10-12:** Added the rest of the Oct 9 types: new headers `core.h`,
  `hero.h`, `shell.h` and one per game (`hub.h`, `race.h`, `chase.h`, ...),
  plus Polar's missing structs in `polar.h`; each overlay includes its game's
  header. About 100 size checks; all binaries still match.
- **2026-10-12:** Named 201 AST Init/Plot functions from the Oct 9 enums
  (game functions named: 144 -> 345). Tank's 68 match the leaked Tank
  prototypes exactly; 14 matched Tank wrappers were retyped to `NAS *` first.
- **2026-10-11:** Integrated the Oct 9 prototype types: `nas.h` now uses the
  original names (`NAStype`, `EXTengtype`, `HeroInfo`, `EXTanimator`,
  `NASTrigger` for `NASSpawn`, `_sfx_handler` for `SfxVoice`) and defines
  `EXTheromove`, `EXTbotai`, `PreMap`, `LevSpec`; `tank.h` gains the Tank enums,
  `TA_player`, `TA_Bot`, `EXTobject_TA`. All binaries still match.
- **2026-10-11:** Searched the three Hidden Palace prototypes (`protos/`,
  git-ignored). No `.SYM`/`.MAP` files or text leaks, but the Aug 23 and
  Oct 9 DATs end in SN debug type records: engine structs with offsets and all
  AST enums, which name every overlay's game. New `leak_scan.py`,
  `sym_scan.py`.

- **2026-10-11:** Copied `func_80026A28` to `func_800C9B5C` (ovl62). The two
  PsyQ-gap copies don't fit in the build (function padding, see Open leads).
- **2026-10-11:** Matched `func_8001534C` (string hash) with a 10-minute
  permuter run after the loop rewrites stalled at 9 operands: the product goes
  through a reused `s32` temp. Diff patterns row added.
- **2026-10-11:** Third hour of `next`: 18 main.exe functions (16 matches,
  two identical copies), one via the permuter (`func_80015518`). New types:
  `SeqSlot` (`D_8006557C`, was an `s32`), `OffsetTable`, `PadState`,
  `MusicEntry`, `LoaderState`; `NAS.prev30`/`next30`. Two Diff patterns.
  Left `func_8001534C` unmatched (Open leads).
- **2026-10-11:** Second hour of `next`: 14 main.exe matches (`func_800280C0`
  to `func_8001682C`), one with the permuter (`func_8001828C`). New types:
  `NodePool`/`PoolNode` (`D_80056844`), `NAS.prev14`/`next14`.
  `func_80016EF4` takes a second argument (passed to `func_8002A5A4`).
  Four Diff patterns.
- **2026-10-11:** Matched `func_80016274` (112 bytes, first diff): first entry
  of a NULL-terminated array whose `func_8001534C` value equals `arg1`+0x10.
- **2026-10-11:** One hour of `next`: 17 main.exe matches (`func_8002C488` to
  `func_8001DCD4`), most on the first diff. `func_800101E0` launches the
  Spyro 3 demo (unused local for the frame). Subengine +0x54 is `hook`,
  `VabSlot.handle`, heap free-stats globals, ovl6a `D_800BCEE8` is a
  `subengine *`. Three Diff patterns.
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
