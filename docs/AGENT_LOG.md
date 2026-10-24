# Agent log

Read this first. It holds the current state, what to do next, open leads and
the gotchas that still bite. Rules that apply to every match belong in
[MATCHING.md](MATCHING.md) (the Diff patterns table), facts about the game in
[ARCHITECTURE.md](ARCHITECTURE.md), and repo rules in
[CONVENTIONS.md](CONVENTIONS.md): move them there rather than here.

At the end of a session, update **Current state**, **Next** and **Open
leads**, add any new gotcha, and add one short entry at the top of the
**Session log**.

## Current state (2026-10-24)

- [ROADMAP.md](ROADMAP.md): Phase 1 (types and names) is done, and every
  `main.exe` function matches and `main.c` is split (2026-10-23); Phase 2's
  naming and data are left. The work now is matching the overlays (Phases 3-5) with the
  swarm.
- 2520/2537 game functions matched (94.62% of game code bytes, 2026-10-23):
  **main.exe, the engine, ovl61, ovl65, ovl66, ovl67, ovl69 and ovl6a are
  complete**; 17 left (ovl5d 2, ovl5e 1, ovl5f 2, ovl60 2, ovl62 3, ovl63 4,
  ovl64 2, ovl68 1), every one tried by the swarm at least once. All 564
  PsyQ functions (Sony objects). Three parked by hand, `func_800BE914`,
  `PlotTransmuterBeam` (one instruction off) and `func_800B4048` (Open leads)
  (`decomp.py status`; the swarm parked 5, which the dashboard counts from
  `docs/swarm.js`). Matches are C only, natural C
  first (CONVENTIONS.md, Matching rules; the "hack" class was dropped on
  2026-10-22: steering C counts as matched, inline asm is refused;
  `func_800193A8`'s register pins predate that). Every slot overlay's id word is
  its own rodata unit (2026-10-19), so first-file switches link as `switch`.
  `make check` OK. Live figures: `decomp.py progress` / `docs/index.html`.
- Dashboard (2026-10-18): `docs/index.html` is `tools/scripts/dashboard.html` plus
  the data; a live Swarm card from the git-ignored `docs/swarm.js` the swarm
  writes (tools/swarm/README.md, Live status). `progress` takes ~2.5 s warm.
- The engine overlay is seven files (`panel` and `pause` named from
  Jampack's records) and `ovl5d` three, `ovl5e`, `ovl66`, `ovl67`, `ovl6a`
  two each (ARCHITECTURE.md, Files inside the engine / the slot overlays).
  `main/main` is nine since 2026-10-23: `main` (0..135), `dbase`,
  `80017164`, `scripts`, `sfx`, `80022a3c`, `sound`, `music`, `card` (Files
  inside `main/main`, Split so far); the main segment links with
  `SUBALIGN(4)`.
- Compiler: gcc 2.8.1-psx `-O2 -G0`, maspsx `--aspsx-version=2.86` (settled;
  2.7.2-cdk matched a strict subset).
- PsyQ 4.7: all library code is Sony's objects (281, plus `2MBYTE.OBJ`), and
  their `.data` comes from the objects except `bios_1.o`'s (one value differs;
  `configs/USA/psyq/data_keep.txt`). Roadmap: the PsyQ gaps box is ticked.
  The game is built with `-fno-builtin` (it calls `abs`).
- Every type and AST id in the Oct 9 prototype's debug fragment is in
  `include/bash/` and every overlay is identified (ARCHITECTURE.md, Prototype
  debug types). The disc images are in the git-ignored `protos/`.
- Game data (2026-10-16): every `CRASHBSH.DAT` entry but the 15 code overlays
  extracts to editable files in `assets/CRASHBSH/` (textures PNG, models OBJ
  + glTF with animations, sounds WAV/VAB, music SEQ/MIDI, text, screens) and
  packs back byte for byte; `make disc-check` from a fresh extraction is
  identical (docs/ASSETS.md). ROADMAP's asset extraction box is ticked.

## Next

**The overlays' last 17 functions**, all tried by the swarm. Hand-match
them from their checkpoints, closest first (Open leads), with their
earlier compiles (`decomp.py protos`) as evidence. Pointing the swarm at
functions it had never tried (`--only` the untried list, smallest first,
no `--retry-failed`) matched 37 of 42 under 2 KB for $151 (~$4 a match),
against $7.50 for the retry runs, which kept re-trying the same hard ones
(the queue puts checkpointed functions first). After each
run, the "After a run" checklist in tools/swarm/README.md.

Recipes (`--seg overlays/NAME --branch main --max-minutes 60`):
- **Low cost** (2026-10-19, ovl5f: 78 matched for $15.96, ~$0.20 a match;
  ovl5d: 64 for $31.79, ~$0.50, its 250-599 B functions cost more; ovl65:
  50 in 38 min for $16.70, ~$0.33; ovl68: 46 in 26 min for $11.71, ~$0.25):
  `--model sonnet --escalate-model opus --big-workers 1 --big-budget-usd 8
  --max-run-usd 30`. Use it for fresh overlays.
- **Opus** (the `config.json` defaults, 5 normal + 1 big; ovl62/ovl60/ovl63
  matched 77-117 an hour for $34-47): add `--big-workers 2` while big
  functions are queued. The normal workers finish the small functions first,
  then idle.
- **Big-heavy** for an overlay's leftovers, mostly >= 600 B: `--workers 1
  --big-workers 4 --big-bytes 400`. Retry parks with it after tool upgrades
  (`decomp.py unblock`, `--retry-failed`): on 2026-10-16 it matched 12 of
  main.exe's 15 parks.

Gate failures are usually header clashes or the toolchain, not the worker's
C: the swarm now reconciles headers itself (`decomp.py reconcile`), and the
rest are fixed by hand from the worker's patch (Gotchas). A park whose
`jtbl_` names functions is a bad split (Gotchas).

Hand work, between runs:
1. The swarm's near misses (Open leads), by hand. First run `decomp.py similar` on
   any function before working it: a 1.00 twin matched elsewhere ports by
   hand (Gotchas, Tooling). For a register-priority miss, try the repeated assignment after an empty
   `do {} while (0)` (MATCHING.md, CSE's block ends): it adds refs without a
   sched barrier, which matched `PogoAI_RadialSearch` and `PlotScriptFX`.
   For a wrong cse canonical register, a dead assignment after the last use
   reorders `make_regs_eqv`'s last uses (MATCHING.md, CSE's canonical
   register). For a sched1 tie the target breaks with code it doesn't have,
   look for a pass after sched1 that deletes what you add: reload_cse_regs
   (a store-back of a just-loaded value, a repeated store) or jump2's
   cross-jump (a duplicated call, which also sets its arg registers twice
   and so takes away their birthing priority); `func_800BC274` used both.
   For a merged tail kept in the wrong place, `decomp.py jtrace NAME`
   shows which copy each exit went to and why; `decomp.py protos NAME`
   finds the function's earlier compiles on the prototype discs, and two
   successive builds show which source change made a pattern
   (`func_8007F314`, PROTOTYPES.md).
2. Name each function as it's matched when the evidence allows.
3. The `main.c` split is finished: nine files (2026-10-23). The zones
   left open (ARCHITECTURE.md, Files inside `main/main`) are closed: no
   leak names the exe's files and the code evidence is used up, so don't
   reopen them without a new source (a disc with debug records for the
   exe). `split_unit.py` and `decomp.py units [--data]` stay for the
   overlays; a split moves functions, so workers' patches and checkpoints
   for them stop applying.
4. Later Phase 2 items: data to C (after the split), naming and typing
   passes.

Swarm history in short (details: `swarm.mjs stats`, the ledger, the commits):
engine 2026-10-15..17 (big-heavy from 2026-10-16), main.exe parks
2026-10-16, ovl61/ovl67/ovl69/ovl6a 2026-10-18, ovl62/ovl60/ovl63/ovl5f
2026-10-19 (opus normal workers since then, $0.19-0.45 a match under
400 B against sonnet's ~$0.48 before; low cost ~$0.20 with the reconcile
and current tools).

## Open leads

- **The swarm's checkpoints are worth finishing by hand.** `func_800B4AB0`
  (27/1246) and `PogoAI_MoveTowardsTarget` (4/330) each matched in one
  session: apply `<repo>-swarm/checkpoints/<key>/best.diff` (`git apply
  --include=src/...`; merge its header part by hand, since the headers have
  moved on), score it, then work the rows with `sched` and the global.c
  priorities. `func_800B4AB0`'s leftovers were register choices that a
  variable's other uses decide; `PogoAI_MoveTowardsTarget`'s were an
  expression whose shape the swarm had rewritten (`(z - 3) << 9` for the
  neighbour square's `((z + 1) << 9) - 0x800`) and a sched1 barrier
  (MATCHING.md, Diff patterns). Read the checkpoint's log for what was
  tried, not for rules: its "the gate refuses do { } while (0)" predates
  2026-10-22. The closest checkpoints left (stage, differing insns):
  `Plot_PogoPadlock` registers 11/350, `Plot_PogoRipperRoo` instructions 16/656, `func_800B7508`
  registers 17/867, `func_800B5E28` instructions 21/195, `func_800B6708`
  registers 27/411. `CheckMineToBoss` has none.
- **`PlotTransmuterBeam`'s last instruction** (parked 2026-10-24 at
  structure 2310/2311, 316/317 blocks; the `decomp.py block` note lists what
  was tried). At 0xc14 the target reads `TBV->mode` once through a base in
  a1 (`lui v0; addiu a1,v0,%lo(D_8005A614); lh v1,0x34(a1)`), where any
  single read folds into `lui; lh %lo(sym+0x34)`. The target's base was
  live past the read until global alloc: passing it to `InitDelete` as a
  second argument gives these three instructions exactly, but that's no
  source (`InitScriptFX` takes one parameter) and it pushes `bx` out of a0
  in all three spawns. Look for a later use of the base that disappears
  after allocation (a no-op copy, a store reload_cse deletes). Case 1 of the
  hit loop's switch still steers: `one = 1` before the 0xB4 store gives the
  constant 1 enough lifetime for loop.c (MATCHING.md, Diff patterns, the
  t3..t6 row); natural C must give it lifetime 3, not 2, between its set and
  the `hero_last_action` store without changing the 38/18/28 store order.
  Try shapes with `try PlotTransmuterBeam --file`, or the small harness in
  `scratch/ptb/t/` (gone after a clean).
- **`func_800B4048`'s copy** (parked 2026-10-24 at instructions 10/255, 25
  of 26 blocks; the `decomp.py block` note lists what was tried). The tail
  copies `t = v` (`addu v0,v1,zero`) before the `*60` chain that loop.c
  hoists. The Oct 9 build has the same copy (with `*42` and `*72`), so the
  copy is in the source. cse2 and combine keep it only if `v` is set again
  between the copy and the chain (MATCHING.md, Diff patterns). The parked C
  does that with `v = (s32)D_8005A614; q = (u8 *)v;`, but that `lui` comes
  before the hoisted chain in the RTL. So sched1 places it inside `t`'s
  second life, local-alloc gives it v0, and `t` drops to a2 (`v` to v0).
  Look for a natural set of `v` between the copy and the chain whose insns
  land after the hoisted chain (a movable found later in the loop body, or a
  strength-reduction init), or for something that makes q's `lui` lose the
  luid tie. Try shapes with `try func_800B4048 --file`; the variant scripts
  are in `scratch/b48/`.
- **`func_800BE914`'s copies** (parked 2026-10-23 at registers 7/170; the
  `decomp.py block` note has the detail). The target copies each clamped
  speed into v0 right before its `>> 8` (`move v0,aN; sra a2,v0,8`, four
  times). No plain C shape tried keeps such a copy: combine folds an SImode
  register copy into its user unless the copy's source is set between them
  or its result is used again, and post-reload CSE forgets every value at
  the label before them. The parked C gets them from 8-aligned struct
  temps (a DImode SUBREG destination, which combine refuses), which can't
  work after the first division, where `q` holds v1. Natural C that gets
  everything else right: the parameters `pa`/`pb` reused for the x speeds,
  plain nested-if clamps, plain shifts (every t-register then matches, `e`
  in t6). The Aug 23 build has no clamps and loads each field straight
  into v0 before the shift, so the copies came in with the clamps: look
  for a clamp shape that leaves its result in a new pseudo. The game was
  C++ (PROTOTYPES.md); `cc1plus` gives the same code for this C, so only a
  C++ construct (an inline taking a reference, a small fixed-point class)
  is still untried. Test files go through `try func_800BE914 --file`.
- **Earlier compiles of the 17 left** (PROTOTYPES.md, Earlier compiles of a
  function, has the table). Each of the five biggest has a build within a
  few instructions of retail's: Oct 9's `func_800BC038` (+1 insn) and
  `func_800B87A0` (-1), Sep 14's `PlotTransmuterBeam` (-12), Oct 9's
  `func_800B77E8` (-10), the older builds' `PlotUkaPolarMask` (-20). Next:
  disassemble that copy (`decomp.py protos NAME` gives the address and the
  entry offset), diff it against retail, and see whether the change sits
  where the C still differs (done for `PlotTransmuterBeam`: the copies
  differ only away from its last diff, but a build that adds code to a loop
  shows how loop.c scored that loop; PROTOTYPES.md). `decomp.py protos
  NAME --diff DISC` prints the aligned hunks. `func_800B5E28` and `func_800B8294` are the
  same code but two `andi` masks (`0xF0` in ovl5d, `0xE0` in ovl64), so one
  match ports to the other.
- **Finish the demo discs' symbols** (PROTOTYPES.md; applied: 135 function
  and 51 global names, unit `panel`, the Jampack enums, 43 prototypes, the
  leaked types and class names, 16 global types, 13 PANEL.CPP locals).
  Left: `cam_init` and the 11 the tool can't write, the classes with no
  stand-in (`geo`,
  `geo_entity`, `joypad_info`, `tga_header`, `file`, ...), merging the
  other views of `obj_texture` (AnimEntry, TexDst2A84C) and of the camera,
  `PrintNasPos` (never referenced). The link groups place no file inside
  the engine. Full reports: `protos/<disc>/scan.md`.
- **Tune the permuter from its record.** Runs are logged since 2026-10-20
  (`decomp.py permstats`, `swarm.mjs stats`). Two ideas to try once there
  are a few dozen runs, compared on that record:
  - Registers-stage pass weights in `settings.toml` (`weight_overrides`):
    favour temp_for_expr, split_assignment, randomize_internal_type,
    reorder_decls and duplicate_assignment, the edits behind the
    permuter finds in Diff patterns.
  - A deterministic search over the Diff patterns' common fixes (copy a
    parameter to a local, s16 -> s32, split a statement, a constant in a
    local, a goto loop), generalising `splitvars`.
- **Tooling ideas not done** (from the 2026-10-16 plan):
  - Pilots, no code: model tiers (`--model haiku --escalate-model sonnet
    --big-workers 0` on ~40 functions under 128 B, check the first matches'
    types by hand; an A/B of `--big-model sonnet`), and `--chain 3`
    (alternate with single jobs, compare `normal-chained` in `stats`).
  - A by-hand typing pass with `tools/scripts/field_survey.py` on the
    overlays with no struct coverage (ovl64, ovl65, ovl67: enums only).
  - Usage-limit result: not recognised (runs are supervised); if a limit is
    ever hit, keep that `worker.out` and match its result line then.
  - Optional: `try --regions`; a `%lo` relocation-target check in `try`
    (must find 0 over all matched functions before it blocks anything);
    idle permuter slots rotating over registers-stage checkpoints (~60 s
    turns); a long-lived container per slot (`tools/dk` overhead measured
    1.2 s, under the 1.5 s that would justify it).

- **Checkpoint ranking (decided 2026-10-14): stays paired, then blocks,
  then exact.** A sum of the three fractions would have kept a closer
  `func_8002E3D0` version (11/12 blocks, registers right, paired 106/109
  vs 109/110; its C is in that session's result note), but the user
  ruled it out: it would make progress too slow. Port such a version by
  hand instead.

Other:
- The gcc 2.7.2 units (`main/8002d488`, `main/8002e0f0`) are fully
  matched. Their toolchain differences, all in the Makefile and
  `aspsx_fill.py`: ASPSX's slot filling and load expansion; `8002d488` is
  compiled with `-fno-rerun-cse-after-loop` (global arrays keep `la` +
  offsets); and `refill_from_jump` redoes one slot fill where Sony's reorg
  differs from FSF 2.7.2's (it fills a `j` from its target before the
  branches; FSF 2.7.2.3's source is in `tools/reference/gcc-2.7.2.3/`). 2.7.2-cdk and 2.8.1-psx share Sony's reorg order but
  not its codegen, so a gcc 2.7.2 game elsewhere may need the same rule.
- `main/8002e0f0` with `-fno-rerun-cse-after-loop`: `func_8002E25C` stops
  matching (structure 44/95), the other four still match. Whether both
  units had the flag (and E25C's C leans on cse2) is open.
- Heap: `D_8004E0F0` is a `_mal_area` (the `Heap` of before) now. Its +0x10 is the deferred-free list
  (`func_80011578` pushes, `func_80011DB4` counts down); `mal.unk2` is
  the countdown. `func_800115D8`'s prototype still takes `void **`, not
  `_mal_area *`; retype it when its callers allow.
- Prototype debug types (ARCHITECTURE.md, Prototype debug types): every type
  in the Oct 9 fragment is in a header now, and every single-id AST
  Init/Plot is named. Use them while matching: type `NAS` fields, `FUX`
  extensions and globals with them instead of offsets, as the four Tank
  Inits (`InitMineBall`, ...) now are. 34 fields of 15 types
  whose bodies were on lost pages are word or byte arrays; retype them if code
  shows their fields.
- Sound voices: the C walkers use `SfxVoicePool.active`; the symbol
  `SfxActiveVoices` stays only for the data asm (`asm/data/3E4BC.data.s`).
  Drop it from `sym.main.txt` once data goes to C. `func_80022504` is "stop all" (`StopSfx` inlined in a loop),
  `func_800228C4` unlinks without stopping, `func_80022924` allocates,
  `func_80022984` inits the pool.
- Ambiguous naming leads are listed in ARCHITECTURE.md ("Functions reached
  from the AST pairs").

## Gotchas

**Tooling**
- **`BUILD FAILED` with `tee: build/...c.i: Permission denied`** after
  back-to-back `try` runs is a Windows file lock on the build output, not
  the C: run it again.
- **`try`'s `structure n/total` counts differing instructions** (lower is
  better), and a shift in alignment swings it by hundreds; for a big
  function compare `regions` blocks instead. Look the symptom up in the
  Diff patterns table before digging into a pass: `func_8007F314`'s
  base-before-index order was already a row (`T *base = Array; p =
  &base[i];`).
- **`try` adds one to the function's `tries` in `.decomp/state.json`
  each run**, so a script that runs it over many variants should restore
  that file afterwards (`git checkout .decomp/state.json`).
- **Typing a byte-array global as a struct can move its loads.** Its reads
  become in-struct MEMs, and gcc 2.8's alias rule lets a scalar read at a
  fixed address pass a store through a struct pointer but not an in-struct
  one: `D_8009F644.amount` stayed below `gs->otzAdd = 0x200` in pause.c's
  `func_80082590`, where retail hoists it, so that read is
  `*(s32 *)&D_8009F644`. Build and check after retyping a global, and look
  for that pattern where one function stops matching.
- **An overlay that names a symbol its code reaches only at an offset**
  (`D_8009F658.next_process` for splat's `D_8009F65C`) doesn't link: splat's
  `<ovl>_undefined_syms_auto.txt` lists only the labels the code touches.
  Add a `PROVIDE` to `configs/USA/undefined_syms_manual.txt`.
- **A split whose rodata starts with an asm item needs 4-aligned objects.**
  An `INCLUDE_RODATA` item's `.align 2` doesn't survive the linker
  script's `SUBALIGN(2)`, so after a file whose rodata ends at 2 mod 4 the
  next one lands 2 bytes early (the whole exe shifts). psylink 4-aligned
  objects: the main segment now links with `SUBALIGN(4)` (`subalign: 4` in
  main.yaml); the overlays still use 2 (ovl5d 1), so check a split there.
- **Splitting C: K&R definitions and temporary macros.** `split_unit.py`
  takes the moved functions from `decomp.py`'s definition finder (a K&R
  definition, whose parameter lines have `;`, was missed once and its stale
  `.s` sent it back to the old unit in the index, which then dropped it),
  and copies a file-scope `#define` only if no `#undef` follows it (a
  temporary macro copied into the new file broke every local of that name).
- **Splitting a unit: its header's inline helpers go with their users.** A
  `static inline` helper in the old unit's header that calls a function now
  declared in the new unit's header sees no prototype (the headers are
  included in unit order) and gets "conflicting types"; reordering the
  includes just moves the problem when the new header needs the old one's
  types. Move each helper to the header of the file that uses it, and point
  other headers' direct `#include "bash/<old>.h"` at both new headers.
  `split_unit.py` moves the ones only the new unit uses (with the comment
  above, signatures over several lines too); a prototype's comment moves
  only if it starts its own line (a trailing one belongs to the line
  above).
- **Renaming a unit of matched C leaves the index on the old name.**
  `decomp.py index` moves a function to a segment only through its `.s`, and
  splat writes none for matched C, so after renaming a unit's `.c`, header and
  yaml paths, set the `.c` aside for one `make split` (splat then writes every
  function's `.s` into the new folder and a stub `.c`), put the real `.c`
  back, and `index`. Delete the old asm folder only after that: its stale
  `.s` files, under a segment no yaml has any more, become bogus bare-address
  entries if `index` sees them.
- **A new name an original header declares must take its types.** When a
  function gets a name the recovered headers (`polar.h`, `tank.h`, ...)
  already declare, `decomp.py rename` keeps only that declaration, and a
  definition with other types fails as "conflicting types" (rolled back).
  Give the definition the header's types first (`TestBeamChance(u32
  player)`). `proto_apply.py` lists such names before `--apply`.
- **Renaming a function breaks a `D_` alias of its address.** An overlay's
  generated symbol lists (`linkers/<ovl>_undefined_*_auto.txt`) name an
  outside address after the symbol files only, so source that took a
  function's address as data (`= D_8001C378;`, `extern u8 D_8001C378[]`)
  stops linking once the function is named, and `decomp.py rename` rolls
  back. Write the function's name there in the same change (it can't link
  as `func_<addr>` before the rename either). `proto_apply.py` lists such
  uses and refuses `--apply` until they're fixed.
- **A park can gain a matched twin later, and nothing revisits it.** When a
  function matches, the gate's propagate step tries its C on the identical
  copies; one that fails is marked `copy_failed` (and `copy_failed_stage`)
  in `.decomp/state.json`, and differing prototypes (`VECTOR *` against
  `s32 *`) are enough to fail it. `func_800BFEE0` and `func_800B7190` stayed
  unmatched for one to two days after their ovl65 twins matched. `decomp.py similar
  NAME` shows a twin as similarity 1.00: port its C by hand with the local
  types.
- **A matched function's `.s` exists only from before it was matched.**
  splat writes `asm/nonmatchings/**/NAME.s` only for functions the C file
  lacks. Until 2026-10-22 `rename` deleted the old file, losing the target
  asm (`regions`, `vars`, `similar`) and the rodata ownership behind the
  dashboard's data % (1.38% -> 1.27% over that day's renames). `rename` now
  moves the file. To re-create missing ones, split a copy of the overlay's
  yaml with `asm_path: build/asmall`, `disassemble_all: True` and the
  linker/undefined-syms paths moved under `build/asmall`, then copy the
  missing NAME.s files from `build/asmall/matchings/` (byte-identical to the
  originals); 300+ were restored this way.
- **The pre-commit hook refuses files over 2 MB** (a catch-all for game
  data) except docs/progress.json, whose history grows by a line per commit
  (exempt since 2026-10-21; it once blocked 5 swarm matches). A match the
  gate refuses at commit stays in the swarm's results/:
  `run --resume RUNID --only ...` gates it again.
- **`done --commit` in a git worktree (`.claude/worktrees/...`) fails at
  `git add`** ("not a git repository: /work/C:/..."): the worktree's `.git`
  file names a Windows path the container can't see. A worktree also lacks
  `asm/`, `linkers/`, `ext/` and `disc/`: copy them from the main checkout
  first. Re-run the gate with the main `.git` mounted (one line, from the
  worktree): `DK_DOCKER_ARGS="-v <main>\.git:/maingit -e
  GIT_DIR=/maingit/worktrees/<name> -e GIT_WORK_TREE=/work -e
  GIT_CONFIG_COUNT=2 -e GIT_CONFIG_KEY_0=core.hooksPath -e
  GIT_CONFIG_VALUE_0=/work/tools/hooks -e GIT_CONFIG_KEY_1=safe.directory -e
  GIT_CONFIG_VALUE_1=*" tools/dk python3 tools/scripts/decomp.py done ...`.
  If the failed run already matched and propagated copies, `git checkout`
  everything but your own edits first: the scope check refuses the copies.
  Propagation also skips a copy whose header still declares a stale
  prototype (`void f();` against `NAS *`): fix it and `done` that copy alone.
- **`permute --lines A-B` widens to whole statements.** A range starting
  on `if (...) {` takes the whole if/else chain (`func_800B5710` 317-324
  became 277-458). Start inside the block. A `decomp.py permute` stopped
  early records no PERMUTE-STATS line, and a `stop` of `swarm.mjs
  permute` saves nothing: copy `output-*/diff.txt` first.
- **`tools/dk python3 - <<'EOF' </dev/null` runs nothing:** the `</dev/null`
  replaces the heredoc as stdin, and Python exits quietly on an empty
  script. Write the script to a file in `scratch/` and run that.
- **After a resplit or rename, run `decomp.py index`.** Since 2026-10-19 it
  checks every entry against the symbol files and `src/`: a renamed
  function gets its symbol's name (before, 51 entries kept their old
  `func_` names and no `src`), and an entry no symbol, `INCLUDE_ASM` or C
  definition names any more (a function a resplit merged away) is dropped
  from `functions.json` and `state.json`. `decomp.py rename` also failed its build check
  on 2026-10-19 with the error hidden ("BUILD FAILED") when the same rename
  by hand built and matched; rename by hand (symbol file, sed, `make split`,
  `make check`) if it does.
- **A switch whose cases each return is cut into functions.** In a leaf
  function without a frame, every case's `return`/`break` becomes its own
  `jr $ra`, and splat starts a new function at each jump-table target after
  one (`func_800B5588`, 2026-10-19). Sign: a `jtbl_` whose words name
  functions instead of `.L` labels. Fix: give the function its full extent
  in the overlay's symbol file (`func_X = 0x...; // type:func size:0x...`,
  up to the next real function), resplit, then `index` (above).
- **A new prototype can break callers in other files.** The gate adds the
  matched function's prototype to the owned header; an already matched
  caller elsewhere that calls it with other arguments then fails to compile
  (`func_80085890`: two `func_80085890()` calls in ovl63/ovl65). Fix the
  calls (`grep -rn NAME src`), not the prototype; the full build proves the
  callers' bytes. The gate's message shows cc1's error since 2026-10-16.
- **Don't use `swarm.mjs run --dry-run` to preview a queue.** It records its
  fake parks in the swarm dir's `history.json`, which counts them as attempts
  and moves functions to big mode. If you ran one, delete the entries whose
  only notes say `dry run`.
- **`dashboard.html` holds the data placeholder once.** `progress` puts the
  data in place of every `/*DATA*/null` in the template (Python's
  `str.replace`): one in a comment doubled the page. Name it some other way.
- **A swarm match the gate refused on a compile error is usually still good.**
  The swarm runs `decomp.py reconcile` on a declaration clash first
  (2026-10-19), then a gate-fix worker with cc1's error lines; what still
  fails is left for a hand fix. Its C matched against an older tip, and other matches in the run changed the
  same header (a second typedef of one struct, a prototype with other
  parameters). Apply `results/<key>/*gatefix*/patch.diff` (or the big1 one) with
  `git apply --3way`, keep both sides of a header conflict, merge duplicate
  structs into the existing one, and call a function whose callers pass other
  arguments than its definition takes through a cast (MATCHING.md). Run
  `git update-index --refresh` first if `apply` says "does not match index".
- **Smoke-test swarm changes end to end** before a real run: a scripted
  `--worker-cmd` run against a local bare remote (`--remote <bare clone>
  --branch smoke --only KEY`) catches what the unit tests can't (clone setup,
  the queue's Python, patches through `git`); see the 2026-10-16 entries.
- **Claude Code allow rules for paths are `Edit(...)`.** They cover Write
  too; a `Write(path)` rule is ignored, so in `dontAsk` mode the write is
  refused. Check a permission change with a one-turn haiku session in a
  worker clone before a run.
- **Stale asm and `decomp.py index`.** splat rewrites `asm/nonmatchings/` only
  for functions still under `INCLUDE_ASM`; a matched function's `.s` is the
  one from when it was asm. `index` reads every `.s` there, so after a split
  the old unit's copies overwrite the new segment, and stale files carry old
  sizes (`func_80027408.s` is 404 bytes, `functions.json` says 244). After a
  split, move the old unit's folder aside, `make split`, copy the matched
  functions' `.s` into their new folders and update `functions.json`'s seg,
  src and asm for the moved functions (as the engine split did) rather than
  running `index` over everything.
- **Rodata migrated into a function.** When a matched function's .s held
  `.rodata` (splat migrated it there), the full build loses that symbol. Add
  `D_xxx = 0x...; // force_not_migration:True` to `sym.main.txt`, put
  `INCLUDE_RODATA(...)` where the function was, `extern char D_xxx[];`, and
  `make split` (`func_800235BC`). Don't run `splat split --disassemble-all`:
  it writes `asm/matchings/`, which the build assembles and fails on.
- **`make clean` (and `make verify`) delete all of `build/`** (since
  2026-10-16). Keep drafts and notes in `scratch/` (ignored); the game data
  dump is `assets/` (your edits: `dat.py extract` won't overwrite it without
  `--force`); splat's binary segments are in `asm/bin/` (`asset_path`).
  `disc/files/` must be complete for `make disc` (rerun `decomp.py extract`).
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
  contain it. `swarm.log` timestamps are UTC.
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
- `tools/dk` right after `git checkout` sometimes hits a Windows
  `PermissionError`; retry. Writing a source file (`main.c`) from a script
  in a loop can hit it too, now and then: retry the write, and check the
  file is back to its base after a variant loop (`git diff`).
- **main.exe has no target `.s` files** (it is fully matched, so splat
  writes none): `make diff`, `regions` and `vars` can't align a main.exe
  function. To rework one, `objdump -d -r` the current object as the target
  and diff a variant's disassembly against it.
- `make permute` runs in its own container: stopping the background task
  doesn't stop it. `docker stop` it before running `done`.
- Stopping `swarm.mjs permute` early (a STOP file or Ctrl+C) saves nothing,
  and its STOP file then blocks the next run until deleted. Copy the best
  `output-SCORE-N/` from `<swarm>/permuters/hN/build/permuter/NAME/` first,
  apply its `diff.txt` in that clone, measure it, and write `best.diff` and
  `best.json` (`git diff` in the clone, against its HEAD).

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
- Every inline asm spelling, register pins included, is a lint error since
  2026-10-22 (only C), in macros and inlines too; `gte_*` macros are exempt.
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

- **2026-10-24 (hand: `func_800B4048` parked):** from the swarm's
  checkpoint (instructions 10/255), applied with names and the camera,
  screen and percent-table structs. The tail's `t = v` copy is in the source
  (the Oct 9 build has it), and it survives cse2 and combine only when `v` is
  set again before the hoisted `*60` chain. The checkpoint's re-set does
  that, but its `lui` lands inside `t`'s life (Open leads). A dead early
  store keeps the copy through cse2 but not through combine. The two Diff
  patterns rows for this function are merged into one, with the rules.

- **2026-10-24 (hand: `PlotTransmuterBeam` parked):** from the swarm's
  checkpoint (2237/2311 exact) to 2309/2311, one instruction short (the
  single-read base at 0xc14, Open leads). Natural fixes: duplicated case
  tails that jump2 cross-jumps, block-local `Heroes`/trig bases,
  `h = &Heroes[j]` in the team-mate loop, `return`, one `pitch = 0x400`.
  The protos copies (Sep 14, JP) differ only elsewhere; JP bounds loop.c's
  score for the hoisted constant. Six Diff patterns rows; row 1600's
  block-local view pointer advice corrected. ARCHITECTURE.md maps the
  extension to the leak's `EXTtransmuter`. New: `decomp.py protos NAME
  --diff DISC` aligns a build's copy with retail.

- **2026-10-23 (hand: `func_800BE914` parked):** from the swarm's
  checkpoint (registers 7/170). The four `move v0,X` copies before the
  shifts are the whole problem; no natural shape, compiler (2.7.2 too)
  or pass explains them (Open leads). Found on the way: parameter reuse
  for the x speeds puts every t-register right. New tool: `decomp.py try
  NAME --file F.c` diffs a test copy of a function with its unit's
  settings in about 2 s. The checkpoint's C stays under `NON_MATCHING`.

- **2026-10-23 (hand: `PogoAI_MoveTowardsTarget` matched):** from the
  swarm's checkpoint (instructions 4/330). `az`, the next square's world z,
  is `((z + 1) << 9) - 0x800` set before the loop (combine's three-insn
  split makes the target's `sll; addiu -0x600` after CSE has passed it);
  `gx`/`gz`/`bz` stay in the loop for loop.c's threshold; arms 3/4 got
  per-arm locals. Two `do { } while (0)` stay as noted steering (global.c
  priority for `x`/`z`, a sched1 barrier on the `TargetXYZ.z` store; the
  Jampack copy has the same load order). Four Diff patterns rows.

- **2026-10-23 (hand: `func_800B4AB0` matched):** from the swarm's
  checkpoint (instructions 27/1246) and the Aug 23 build's copy, whose
  first test, `ok = (fux->status == 1)`, led to one variable for
  `nas->FUX` and the hit object. The rest were register choices that a
  variable's other uses decide: the `func_800B46B8` results in `tmp`,
  two-statement angle flips, and two steering exits back to `goto steer`
  to keep `ang`'s global.c priority between its neighbours' (three Diff
  patterns rows). 18 left, all in the slot overlays.

- **2026-10-23 (tools: `protos` covers the minigames):** it now reads each
  disc's slot overlays from its DAT and looks a function up in all of them.
  Run on the 19 functions left: the table is in PROTOTYPES.md, the lead in
  Open leads. The Oct 9 prototype has PAL retail's minigame code in 15 of
  the 19; US retail matches JP more often.

- **2026-10-23 (hand: `func_8007F314` matched, the engine complete):** from
  the user's prototype findings. The Sep 14 and Oct 9 builds' copies
  (`decomp.py protos`, new) showed retail's tail merge came in with case
  0's `0x7000` test; a ptrace tracer on cc1's jump2 (`decomp.py jtrace`,
  new) showed why (an exit right after a branch takes the first copy) and
  which shapes keep it. Then the `ActivePad()` base-first helper (a known
  Diff patterns row), Cups_Update's search loop, three statement orders
  and the mask temps: regions 197 -> 264/264. ROADMAP's engine box ticked.

- **2026-10-23 (hand: `func_8007F314` goto pass, still parked):** regions
  176 -> 197/264, the new park: the debug hook in both branches instead of
  the empty loop, a `goto` into the first `return 1` block, and case 0's
  PlaySfx as a `goto` into case 8's (not the source; leads list why). No
  `break`/`goto` mix gives the target's three tail survivors. Found the
  unit is probably `PAUSE.CPP`; `cc1plus` compiles the C identically.

- **2026-10-23 (docs tidy):** README (layout, a names-and-types section, the
  leak and split tools), CONVENTIONS (how a segment is split and named),
  CONTRIBUTING, ROADMAP (current focus, statuses), ARCHITECTURE (main.c's
  section no longer says it is one file) and PROTOTYPES (what is applied and
  what is left) brought up to date.

- **2026-10-23 (numbered types):** core.h's `._1`/`._2`/`._10` fields are
  SLONG_XY, SWORD_XY, UBYTE_RGB (Oct 9 numbers them as Jampack does);
  `._21`/`._22`/`._23`/`._37` are MATRIX, VECTOR, SVECTOR, RECT by a
  two-ahead shift (PROTOTYPES.md); all 23 fields typed, polar.h's left.

- **2026-10-23 (the last seven leaked prototypes):** typed the bodies of
  `line_intersect_zy`, `map_loaded_geo`, `render_entity_thread`,
  `update_camera_thread` (`script_control_info` laid out) and found
  natural code for the widths of `Set_Clock`, `GotoLinkAnim`,
  `map_entity_address` (MATCHING.md, two new Diff patterns). `main.h` now
  includes `core.h`.

- **2026-10-23 (leaked types and prototypes):** the shell's `SWORD_XY`
  family, `process_info`/`process_control_info`/`fade_info` (for ModeMgr,
  GameModeDef and the fade, with the engine's and overlays' globals typed),
  PANEL.CPP's globals as `ScoreInfo`/`ClockInfo`/`SWORD_XY`, the leaked
  class names on the stand-ins (`mal`, `_mal_area`, `obj_texture`,
  `obj_textureset`, ...) and an `obj_dbase` layout. `proto_types.py` had
  rewritten calls as prototypes (fixed); 36 functions now have the leak's
  parameter types (PROTOTYPES.md lists what is left).

- **2026-10-23 (main.c split into nine):** four more cuts, one commit
  each: `sfx` (332..349), `80017164` (170..), `dbase` (136..169, the dots
  code ends at 135) and `scripts` (275..331: its data block is used only
  by 275..331, the process manager ends at 274). `split_unit.py` now moves
  header inlines with multi-line signatures and leaves trailing comments
  where they are. The leaks hold no `main.exe` file names; the open zones
  are listed in ARCHITECTURE.md.

- **2026-10-23 (main.c split into five):** new `tools/scripts/split_unit.py`
  does a split end to end. `card` (549..) is the one exact cut; the
  2-option zones were settled by the gap functions' behaviour and data
  (`80022a3c` from 350, `sound` from 497, `music` from 529); the
  xa/texture zone (456/457) stays open, the 3-6 option ones untouched. The
  main segment needed `SUBALIGN(4)` (see Gotchas).

- **2026-10-23 (PAUSE.CPP placed; engine unit split):** Jampack's line table
  ends `PAUSE.CPP` right before the demo copy of `func_80083708`, and its last
  functions match retail in order, so `8007ee2c` is `pause` (functions
  74..95) and a new unit `80083708` starts at 96 (rodata `0x800ADF60`; the
  build matches). The link groups name only whole sections: no file inside
  the engine. `main.c`: the names against the cut zones (ARCHITECTURE.md,
  Files inside `main/main`) pin one cut exactly (after 548); the rest leave
  2-6 choices, so `main.c` is not split yet.

- **2026-10-23 (PANEL.CPP locals and global types):** 13 locals named from
  Jampack's register records (one C variable in the register, one recorded
  local: `vars NAME --all`); local names can't change the code (make check
  OK). `proto_types.py --globals`: 10 PANEL.CPP globals typed (`Clock_Time`
  u32, the `BigScore_*` counters u16, `left_score` char *, `pHeroHealth`
  u16 *[4]); `PanelCtrl` u32 changes the code.

- **2026-10-23 (parameter types from the C++ names):** new `proto_types.py`
  demangles the applied names (g++ 2.x: `P7NAStype`, `T0`, `N40`, ...) and
  writes the types the repo has over generic ones, then builds the batch and
  halves it on a mismatch: 21 kept, 13 dropped (49 builds). Types are fixed
  for the 21 only where the C's were generic or a different integer.

- **2026-10-23 (stubs and the "mid-symbol" globals):** `DefaultTouched`,
  `DefHero2ObjectCollision` placed by the matched code storing them as
  handlers (`proto_scan.py`: by users, then by order among identical stubs;
  `InitParticleEffect` lands on the repo's own name). `PrintNasPos` is never
  referenced: unplaceable. Of the 8 globals "inside a symbol", 2 had exact
  symbols the lookup missed (`core_process`, `GlobalNAStatus`: renamed); 6
  had no reference to their own address in retail, only to the next objects,
  so they are rejected (`globals.txt` now shows the first offset).

- **2026-10-23 (Jampack enums in `shell.h`):** `cheatflags`,
  `worldnumbers`, `levnumbers` (demo values, marked). Jampack's `IconParts`
  is Oct 9's `IconBits` reordered and would redefine its enumerators: left
  out. Check demo types against the Oct 9 headers before adding them.

- **2026-10-23 (4 more names, approved):** `SetTweakAi`, `HeroGhost`,
  `line_intersect_zy` (engine) and `TankBossCortexTurretFire` (ovl5f):
  0.78..0.88 and in link order, no gap to confirm them; the user approved.

- **2026-10-23 (unit `8007ac20` is `panel`):** `PANEL.CPP` by Jampack's
  records: `src/overlays/engine/panel.c`, `bash/panel.h`, the yaml paths;
  the index needed the unit's asm regenerated (see Gotchas). make check OK.

- **2026-10-23 (36 engine globals named):** Jampack's 33 PANEL.CPP globals
  (`PanelCtrl`, `Clock`, `Score`, `IconParts`, `CupTimer`, `pHeroHealth`,
  ...) and Euro's `map`, `camera_frame_list_size`, `camera_frame_right`
  (`engine:ADDRESS` specs, so they go in `sym.engine.txt`).

- **2026-10-23 (10 slot overlay names from Euro Demo 65):** `UpdatePAPU`,
  `PAHeroHit`, `SetPAHeroCarry` (ovl5e), `PlotTotemBit` (ovl5f), `DoGetBomb`,
  `TestBeamChance` (ovl62), `MaintainCentre` (ovl63), `num_players_render`,
  `adventure_mode_head`, `team_mode_init` (ovl69). `polar.h` (the leak)
  declares `TestBeamChance(u32)`, so its definition took `u32` first (see
  Gotchas).

- **2026-10-23 (37 engine names, PANEL.CPP and Euro):** Jampack's 23
  `PANEL.CPP` functions and Euro Demo 65's engine labels (`InitialiseNasSystem`,
  `GravitateNas`, `level_load_map`, ...; `InitHealth`/`PlotHealth` take their
  originals `Init_HealthPU`/`Plot_HealthPU`); the discs agree where both name
  one. `proto_scan.py` no longer matches `jr ra` stubs by code (every stub is
  alike) but by distance from the last match; `proto_apply.py --module slot`.

- **2026-10-23 (13 `main.exe` globals named):** the Euro Demo 65 globals
  that land on a `D_` placeholder's start: `screen_aspect`, `FirstNas`,
  `World_floor_level`, `num_sfx_banks`, `cdb`, `next_sub`, ... (`decomp.py
  rename` takes `D_<addr> NAME` pairs too).

- **2026-10-23 (globals mapped to retail):** `proto_scan.py` pairs the data
  addresses aligned instructions of strong matches build and gives each
  global a retail address (`globals.txt`): Jampack 33 (all PANEL.CPP, all on
  a `D_` start), Euro 41 (`main.exe` 33). A module's data can move apart from
  its code (Jampack: code +4, data -0x28), so the data offset is measured,
  not assumed; a vote on start hits over the code's shifts only got it wrong.
  Nothing renamed yet.

- **2026-10-23 (4 originals for our own names):** `SpawnNASById` ->
  `CreateNAS`, `SpawnNASFromRecord` -> `TriggerNas`, `AddEngine` ->
  `AddExtEngine`, `AddExtension` -> `AddExt` (Euro Demo 65's symbols),
  across 31 code files and the docs.

- **2026-10-23 (5 more `main.exe` names):** the 0.75..0.9 matches checked
  by hand (link order between confirmed renames, same gap to the next one)
  and renamed: `mal_free`, `path_get`, `get_script_header`, `new_sprim`,
  `texture_draw` (93/575 named).

- **2026-10-23 (73 `main.exe` names from Euro Demo 65):** new
  `proto_apply.py` turns a scan's matches into a checked rename list for
  `decomp.py rename --file`; the 0.9+ placeholder matches in `main.exe` are
  renamed (88/575 game functions named now). `ovl60` used `D_8001C378` for
  `func_8001C378`'s address and stopped linking (see Gotchas); fixed in the
  same commit.

- **2026-10-23 (Japanese retail scan, `protos/JP_retail/`):** no leak,
  as PAL. Every disc on the list is scanned (PROTOTYPES.md); next is
  deciding how to apply the Jampack and Euro Demo 65 names.

- **2026-10-23 (PAL retail scan, `protos/PAL_retail/`):** no leak: no
  debug records, no source text or build paths, and the engine's spare
  space is zeros and `0x01` filler.

- **2026-10-23 (PlayStation Demo Disc 1.3 scan):** its `CRASHBSH.EXE` and
  `.DAT` are Jampack's byte for byte, so the same leak; the disc was
  mastered 2000-08-30, which dates that build. The leak is in the files, so
  only a different build helps: `proto_scan.py` now prints the files'
  SHA-1 and names any scanned disc with the same files.

- **2026-10-23 (Euro Demo 65 scan):** its engine entry holds 1182 SN
  symbol table labels and 2325 types: 163 labels match retail functions
  (`main.exe` 124, engine 27, slot overlays 12), 184 are data, 811 are
  section symbols giving the link layout (`_WORLD_OVERLAY`,
  `_RULES_OVERLAY`, overlay names). `proto_scan.py` now matches labels,
  finds the slot overlays, fits per-stretch offsets (`+0x4C` from
  `0x800947F4`), and takes a lone `.bin` without a `.cue`.

- **2026-10-23 (`proto_scan.py`):** the disc scan is one command now
  (PROTOTYPES.md, Scanning a disc): finds the exe by its DAT index, the
  engine and its load address, runs sym_scan/leak_scan, matches function
  records to retail, and writes `protos/<disc>/scan.md`. `sym_scan.py`
  reads line records (and checks code addresses, which drops its junk
  block records). Every disc in `protos/` rescanned; types unchanged. New:
  the Jampack engine loads at `0x80071764`, its records 4 bytes lower (a
  link one word off), and its 1021 line records cover all of `PAUSE.CPP`;
  every leak so far is inside the engine's DAT entry.

- **2026-10-23 (Jampack Winter 2000 demo scan):** the demo's DAT holds an
  SN fragment in its engine entry's tail: the first function records (all
  30 of `PANEL.CPP`, with locals), 44 globals, 4611 types and line
  records. 23 functions mapped to retail unit `8007ac20` by masked code
  similarity; nothing renamed yet. Written up with every other disc's
  findings in the new PROTOTYPES.md.

- **2026-10-23 (hand: `func_8007F314`, still parked):** worked jump2's
  cross-jump order out against dumps: `redirect_jump` puts each threaded
  `break` at the head of the join's chain, so with the empty loop after
  the switch a tail merges into the latest surviving copy, and without it
  into the last tail (MATCHING.md, Cross-jumping). The target needs case
  0's PlaySfx tail to merge into case 8's, an existing label at the
  survivor's `j`, and both `return 1` blocks kept apart; no C shape tried
  gives that, and `try`'s structure count rose while regions fell, so the
  park keeps its C (block note).

- **2026-10-23 (hand: `func_800BFEE0` and `func_800B7190` from their ovl65
  twins, 35308bcd, 402f835a):** `func_800BFEE0`'s park (4 lines off) had an
  identical twin, `func_800B4474` (ovl65), matched 26 minutes after it
  was parked; the gate's copy failed on the differing prototypes.
  Its control flow fixed it:
  the body nested in `if (disc >= 0)`, with `return 0;` in place for the
  `t0 < 0 && t1 < 0` early-out (MATCHING.md has the jump2 rule; 35308bcd's
  note credits a cross-jump, which the jump2 dump doesn't show). `similar`
  on all 21 unmatched functions found one more twin: `func_800B7190`
  (ovl63) is ovl65's `func_800B9020`, ported with new `Ovl63Seg` types.

- **2026-10-23 (hand: the `func_800BC274` triplet matched, ce174819,
  b3a8cd4c, 52ce83eb):** the a1/a0 swap before `jal func_8001CD04`, parked
  after five passes, fixed with steering C. The call is written in both arms
  of `if (eng->unk5E)`, each copy storing `eng->sub` back through a temp. The
  store-back is the block's second memory insn but uses the load, so the load
  waits out its latency; the second call sets `$a2` twice, so the a2 set
  loses birthing priority and fills that slot in place of `addiu a1`, and the
  load then wins on potential hazard. reload_cse deletes the store and copy,
  and jump2 cross-jumps the arms back into one. The same C matched
  `func_800BAF1C` (ovl64) and `func_800C284C` (ovl5d, with `EXTobject`
  names). Found by tracing the ready lists in `-dS` dumps of variants
  compiled with cc1 directly (`scratch/bct/h.sh`); MATCHING.md has the rule.

- **2026-10-23 (hand: `PlotScriptFX` matched, 562b2203):** from the parked
  95/112. The f/cur swap: `f = 0` repeated twice behind empty do-whiles (34
  refs, outranks `cur`). Block 21: a dead `p = NULL;` after the
  interpolation makes `p` the last-mentioned of p/cur/next for cse, so the
  load of `next->frame` stays on `p` and combine can't fold `p++` into
  `next = p`; local-alloc's `optimize_reg_copy_1` then moves the load to
  `next`. `kf->frame` is read into `first` before the `||` test, and a second
  `sub->unk52 = 1` after the sy line makes that quotient tie the remainder
  in local-alloc (a0). MATCHING.md has the rules.

- **2026-10-22 (hand: `PogoAI_RadialSearch` matched, 8a76ba61):** the
  nested do-whiles gave dx/dz the refs for fp/s7 but put a sched2 barrier
  after `dx = -1/1`, so side1's store came before side0's. Each case now sets
  dx/dz, then an empty `do {} while (0);`, then the same assignment again:
  cse1 keeps the repeat (it stops at the loop-end note), cse2 makes it a
  self-copy that flow counts, combine folds the first set into it. No
  barrier is left among the stores, which then keep source order (natural
  side0..side3). Found with direct cc1 compiles of the unit's `.i` (~0.6 s
  each, `scratch/rs2/h.py`) and `-dt`/`-dc` dumps; MATCHING.md has the rule.

- **2026-10-22 (the two unblocked by dropping hacks):** `func_800B62EC`
  matched (be7c85ca) from swarm D's patch: its hero-loop dx/dz do-while
  keeps `pz` in fp (loop-weighted refs); without it the structure is off.
  `PogoAI_RadialSearch` ported from its checkpoint to 2/347 and parked:
  one store pair in a cross-jumped case tail (Open leads).

- **2026-10-22 (no more hack class):** on the user's call, matches that
  steer the compiler with C count as matched like any other; only C is
  allowed (inline asm of any kind is a lint error, in macros and inlines
  too, `gte_*` excepted) and natural C comes first. Removed `decomp.py
  hacks`, `done --hack`, the idioms/acceptance records, the dashboard's
  Hacks tile, the swarm's hack rule and pattern filter, and the (hack)
  markers in MATCHING.md; updated CONVENTIONS, ROADMAP, CONTRIBUTING,
  CLAUDE.md and the /match-function skill. Older entries below keep the
  old wording.

- **2026-10-22 (hand: `PogoWinner_Relic` without its hack, 348e2185):**
  the do-while around `if (out != 0) done = 1; else lose;` was a loop to
  loop.c, which moved the `done = 1` arm out of line, so the lose code fell
  through with no label and reload_cse turned `i = 0` into `move s1,s3`.
  Writing the test as `if (out == 0) lose; else done = 1;` (as the
  function's other two arms already were) matches plainly.

- **2026-10-22 (hand: ovl66's two hacks):** `func_800B6CD8` without its
  identical if/else arms (c585c406): one PlaySfx per case and the store
  after the switch. `func_800B4454` left flagged: plain C allocates by
  global.c priority `best` 10/124 below `z`/`x` (9/109, 9/110) and
  `found` 7/140 below `arg0` 10/188. The do-while gives each one more
  loop-weighted ref; it matches in both loops too, so it reads like a
  macro around `best = d; found = ...`. Loop shape (the sibling's
  `do { e = &T[i]; ... } while`), pointer locals and an `else if` split
  don't help. From the dumps (`-df`: flow.c counts REG_N_REFS +=
  loop_depth before combine; global.c allocno_compare): `arg0`'s 10 refs
  are 3 `Heroes[arg0]` multiplies (3 mentions each) + its copy, the same 3
  as the target, and `x`/`z` are loaded at their block's end (hazard), so
  the target's flow RTL had `found` >= 8 refs (beats `arg0` 0.160) and
  `best` >= 11 (beats `z` 0.248). The wrapper in loop 1, loop 2 or both
  matches; around the whole body it doesn't. A second `found != 0` test
  (`return found != 0`, `?:`, two ifs) gives `found` its 8th ref and `fp`
  but becomes an `sltu` where the target branches on constants. The user
  accepted it as a macro: `OVL66_SET_CLOSEST` in both loops (093716cd,
  `hacks --accept`).

- **2026-10-22 (hand: hack reworks, no permuter):** `func_800115D8`
  without its do-while (2202c6e4): `arg3` is `s32`, not `s16`. A narrow
  parameter's conversion insn sits in block 0 and sched1 sinks it to the
  first call, shortening its life so it outranks `arg1` in global-alloc.
  `func_8008A924` tried again and left as a hack: a byte temp lets
  reload_cse delete the self-store but sched2 floats `li a1,5` into the
  load slot; a wider temp keeps the order and the store (Diff patterns).

- **2026-10-22 (hand: `func_80098284` without its hack):** the do-while
  only moved `li v0,0x1E00` ahead of `li a0,0x19D` before PlaySfx. The real
  cause: `func_800976F8` never reads the `arg0` the decomp gave it, and
  passing `p` set `$a0` twice, so sched1's birthing launch (REG_N_SETS ==
  1) skipped `$a0 = 0x19D`. With `func_800976F8(void)` it matches plainly
  (53a439eb; prototype in 7caa393e and the next commit). Of 222 matched
  PlaySfx setups only this one and four with `$a0` set once are v0-first.
  Lesson for the other hacks: check the callees' asm for unused params.

- **2026-10-22 (lint helpers, swarm E/F, hand: the BC274 triplet):** the
  lint now reads the macros and static inlines a body uses and matches
  `do { } while (0)` at any depth (fc8825e9): 107 hack matches (+25).
  The swarm's prompts name the refused hacks. Swarm E (the 24 todo
  functions, all retries) was stopped after 10 min on the user's word
  (only never-swarmed functions wanted): `Tile_Update` matched
  (430e013d). Swarm F ran the last never-tried one, `func_800BAF1C`: parked
  at 2/197, the same sched1 tie as `func_800BC274`/`func_800C284C`. A
  fifth hand pass on the three (no hacks, no permuter) found a tail
  duplication that flips the tie but costs a `%hi` register (Open leads);
  all three parked by hand.

- **2026-10-22 (swarm D: every never-tried function):** run 20261022104953
  on the 42 never-tried functions (`scratch/swarmD_only.txt`; `func_800BAF1C`
  left out, a `func_800BC274`-style hack case). Started with the default 5
  normal workers idle (every function is over `--big-bytes`), restarted
  with `--workers 1 --big-workers 10`. 33 matched in 1 h 50 min, none with
  a hack, 83.10% -> 92.26%: up to 5.3 KB (`PlotBearminator`,
  `DoChooseNewAction`, `PlotPolarBoss`, `PlotN2OBall`). $265 (~$8 a match;
  the $200 cap stops new sessions but lets running ones finish). 1 parked,
  9 never started (cap). `func_800B62EC` matched only with a `do { } while
  (0)` wrapper and the gate refused it (new lint): its C is in the swarm's
  results, for a sign-off or rework. Checkpoints: `func_800B4AB0` 27/1246
  instructions; `PlotTransmuterBeam`, `func_800BC038`, `func_800B87A0`,
  `func_800B77E8`, `PlotUkaPolarMask` at structure. Next: the 9 unstarted
  plus `--resume` those checkpoints; let idle normal workers take big jobs
  (swarm.mjs).

- **2026-10-22 (hack audit + swarm C):** the lint now flags `do { } while
  (0)` wrappers in function bodies (they only add loop notes): 88 matched
  functions had them. A script unwrapped each and re-ran `try`: 9 match
  without (wrappers removed), 79 need them and are on `decomp.py hacks`
  (88 hacks in all). 57 Diff patterns rows that recommend the wrapper are
  marked (hack). One was mine (`func_8001CE00`, earlier this session).
  Swarm C on the 35 untried 2-4 KB functions, stopped by request after
  ~15 min: 4 matched (`func_800CDE8C`, `PlotRaceMissile`, `func_800B55E8`,
  `func_800B4144`, the last gated with `--resume`), none with a hack.
  83.10%. By request the 79 stay flagged except macro-shaped ones: 6
  accepted (`hacks --accept`), each wrapper a self-contained idiom (the
  `obj->player = ...; obj->will_do |= mask; return nas;` block in five
  functions of ovl5d/5e/64, and `PlotFlubberTA`'s restart-sound and
  child-engine copy); single assignments wrapped 2-3 deep were not. 82
  hacks remain, to rework when a natural form turns up. Next: rerun swarm
  C (`scratch/swarmC_only.txt`).

- **2026-10-22 (naming, second hour):** 554 -> 602 named. Leak names via
  their code (Arkenoid intersect/balls/floor/corners/collisions/pickups;
  Pogo kerbs, claim, AI, ramp, stealing, level tile helpers; Tank
  sandbag/swamp/mine-to-boss/triggers; Polar AI chooser, team-mate check,
  bombs, CheckMelted/IsPtInALLZone), `LevSpec.PostTriggers` (`D_8005AA88`)
  for the `*Triggers` stubs, and a debug string for `XZBoxToSphere` (one
  copy each in ovl62, ovl63, ovl65). More stand-ins became real types
  (`Ovl60Goal` = `EXTbotai`, `Ovl61FuxF` = `EXTobject.can_do`, raw offsets
  = `EXTobject_TA` fields), all byte-identical. Found the data-% drift on
  renames: `rename` deleted matched functions' `.s` files (fixed; 300+
  restored, data 1.27% -> 1.39%; Gotchas). What is left of the leak is
  ambiguous (Polar `Test*Chance`/`Do*` states, two Pogo planners, AIQuality
  vs Quality_Init, the `s16` bounce returns, `CountBears`); the other seven
  games have no leaked names.

- **2026-10-22 (naming from the leak, ~1.5 h):** named 345 -> 554 game
  functions from the leaked minigame headers, each from its own code
  (evidence in ARCHITECTURE.md). Sources that pin names: the engine's
  `HEROFunc` table (`0x8009DCA0`, `{SetMode, Action}` per hero action id:
  every game's state machine), `LevSpec` (`D_8005AA70`) and `HookInf`
  (`D_8009D6E8`) slots (field names from the prototype's types), anim ids
  (`HeroAnim_seqs`), AST spawn ids, and the leak's code order. Done:
  Arkenoid states, hooks, PI bounds; Tank states, hooks, SFX setters,
  sandbag maps, Moe/Joe; Pogo almost all (states, tiles, zaps, islands, AI,
  pickups, challenges); Polar hooks and a few AI helpers. Method: rewrite
  the definition and owned prototype to the leak's types
  (`scratch/leak_sig.py OVL LEAKHDR ADDR=NAME...`), `make check`, then
  `decomp.py rename` (it fails on conflicting types otherwise). Local
  stand-in structs (`Ovl61Obj`, `OvlClampPos`, `Ovl61Vel`) were
  `EXTengine` fields; unsigned params became `s32` with `(u32)` casts in
  the bounds tests (same code). Fixed two `rename` bugs (`dedupe_owned`
  dropped struct members and declarations its header's inline functions
  use). Left unnamed with notes in ARCHITECTURE.md: Pogo FizzleOut /
  DelayedFizzleOut / OnFire, the three AI planners, AIQuality vs
  Quality_Init; Arkenoid's non-PI bounce / out-of-bounds (BA/SE/NG) and
  `AR_PI_BounceOnBounds` (the `s16` return adds a sign extension); Polar's
  `Test*Chance` family (AI states unnamed). Next: make `leak_sig.py` and
  `scratch/fninfo.js` (callers/callees/arg count per function) proper
  tools; the table's other games (ovl5d, 5e, 63-68) have no leaked names.

- **2026-10-22 (hand: `func_800BE158`, parked):** worked the 2/347
  checkpoint under the updated `/match-function` rules (no hacks, permuter
  only at registers stage). Its nested do-whiles are a hack; natural C
  reaches structure 337/350 and is parked with the deciding rules (Open
  leads). A permuter run started from the structure-stage C was stopped
  under the new rule; no result used. Still 82.30%.

- **2026-10-22 (swarm C, aborted):** queued the 35 never-tried 2-4 KB
  functions (`scratch/swarmC_only.txt`, from `scratch/untried_now.txt`).
  Docker Desktop was down (start it first); the first start used the
  default `--big-workers 1` (pass `--big-workers 6`, every function here is
  big) and lost 2 sessions to the OAuth refresh collision (reset with
  `forget`). Restarted, then stopped by request after 2 min: 0 matched,
  nothing to gate. Rerun the same command for a real try.

- **2026-10-22 (swarm A, resumed):** run 20261021204521 on the same
  `--only` list after `forget` reset the 8 network failures: 27 matched,
  2 parked for $104.87 (~$3.88 a match), stopped by request with 5 queued.
  Game code 82.30% (2471/2537), `make check` OK, no new hacks. Fold: 83
  rows; dropped one that called a function through a cast to keep two
  call tails apart (a workaround, as `func_800B4444`'s was).

- **2026-10-21 (swarm A: untried functions under 2 KB):** run
  20261021184442 (`--only` 42 never-tried functions, no `--retry-failed`):
  10 matched in 40 min before the network went down; 8 failed on API
  errors (ENOTFOUND, uncharged), not on their C: func_800B7D94, func_800D0A6C, func_800C0888, func_800B9020, func_800B7244, func_800B980C, func_800B6E0C, func_800C18F0. Requeue
  them (`swarm.mjs forget --only ... --why "network"`, then the same
  `--only` list from `scratch/swarmA_only.txt`). Hand (B) in parallel:
  `func_800B4048` natural C at 247/255 (`scratch/b4048_natural.c`; left:
  `andi 0xFFFE` and one copy), `func_800B5E28` 178/195 (its `dz`/`c` reuse
  looks original; one variable per value gives 120), `func_800BE914`'s
  163/170 checkpoint is contrived (aligned struct, param reuse).

- **2026-10-21 (swarm, third big-mode retry, stopped):** the first start
  died in 30 s on an OAuth refresh collision (4 failures in a row; login
  fine), restarted with `--start-gap-seconds 30`. Run 20261021170058,
  killed by request: 11 matched for $82.57 (~$7.50 each, all fresh
  0.8-1.5 KB functions), 5 parked. Finding: 89 of the 104 left (221 KB)
  were never attempted. The queue puts checkpointed functions first, so
  `--retry-failed` runs spent their budget re-trying ~15 hard ones
  (`func_800BE914`: 11 ledger entries). Next runs: drop `--retry-failed`,
  `--only` the untried ones, smallest first.

- **2026-10-21 (hand: hack reworks, docs):** `func_8001CE00` without its
  `if (e || blk)` hack (340ce459): `e` 6 refs/21 insns lost `s0` to `blk`
  5/16 in global.c; its `func_80019094(e, ...)` call in a do-while(0)
  fixes it. Four more tried and left as hacks (Open leads). Permuter on the
  one near miss (`func_800BE914`, registers 26/170) 38 min: no gain.
  Dashboard: swarm parks are looked up by unit and name (slot overlays
  share names); Parked reads 9 with swarm.js (4 hand, 5 swarm). `hacks
  --accept NAME --why` records a hack reviewed as plausibly original (state
  `hack_ok`), off the count but listed: `func_800193A8`. The overwritten-store
  lint read `.in` as the local `in`; fixed, it flags `func_8001AF2C`. 11
  hacks. Docs: stale leads (matched functions) cut, hack kinds updated.

- **2026-10-21 (hand: `func_800B4444` without its hack):** 40ba6e7d. The
  target steps a pointer it never reads, so the original read `*p` and
  lost the read late: the `-d` dumps showed jump2 deleting an
  `if (*p)` branch whose body flow had emptied. An unused `s16` counter
  (`if (*p != NULL) c++;`, add + sign-extend: a cycle cse1 keeps and flow
  deletes) in a goto loop matches; an `s32` one dies in cse1.
  `func_800B3330` takes its `s32 *` (18402bd5), no cast.
  12 hacks left.

- **2026-10-21 (hand: `func_800BC274`, still parked):** a fourth pass
  with no hacks or permuter. Worked sched1's tie-breaks from sched.c:
  potential hazard, priority (counted from the block start: the call
  data-depends on a0, but the load only outranks `addiu a1` with an
  in-block predecessor), class and luid all need an instruction in the
  target's sched1 block that its final code lacks, and REG_EQUIV,
  reload_cse, ADDRESSOF and cross-jumping can't remove one from natural
  C. Block note updated; the next try needs a new block boundary.

- **2026-10-21 (hand: `func_800B7134` without its hack):** df4e1881 replaces
  the dead-store match (tb/trig/seq NULLed at the outer loop's bottom).
  The `-dL` dump showed the outer loop's move_movables stopping two
  candidates late; reading `D_800BE888` in the inner loop (not through `t`)
  adds two inner movables ahead of the `D_800BE838` base, so `insn_count`
  doubles twice more and the bases stay per i. No permuter. 13 hacks left.

- **2026-10-21 (swarm all overlays, second retry in big mode):** run
  20261021103227, same flags as the first (`--max-minutes 180`), ran to
  the time limit: 85 matched (80 in the run, 5 re-gated) for $232, 5
  parked; game code 74.86% (2423/2537). The last 5 matches were refused
  at commit: docs/progress.json had passed the 2 MB pre-commit limit.
  `progress` now writes one line per history point (1.3 MB, bc39f123),
  and `run --resume 20261021103227 --only ...` committed the saved
  matches with no new sessions.
  `fold`: 111 rows, 84 kept (2 described hacks, the rest duplicates of
  each other or of older rows). Those 2 matches, Roo_FindNewSquare (a dead
  load overwritten before any read) and PlotHelicopterTA (`if (i) {S} else
  {S}`), had passed the lint: it now flags both shapes (`overwritten_stores`,
  `same_branches`), which found 8 older ones; 14 hacks in all.

- **2026-10-21 (hand: MATCHING.md cleanup, three swarm parks):** merged
  21 duplicate Diff patterns rows from the last two folds (e8ba0e20).
  `func_800B4240` was already matched by the retry run. From the swarm's
  checkpoints: `func_800B3CA8` (ovl69, e33213ca) matched with its +0x32
  store as an array element (in-struct, so sched moves it past the scalar
  load into the delay slot); `func_800B9E74` (ovl5e, cecd6837) with
  `lo *= lo; if (dx * dx + dz * dz < lo)` (it also drops the checkpoint's
  `(dx = dz * dz)` trick); `func_800BFEE0` (ovl62) parked by hand at
  183/187 with C under NON_MATCHING (`decomp.py block` note has the tries).

- **2026-10-21 (swarm all overlays, retry in big mode):** run
  20261021072703, `--seg overlays/ --retry-failed --big-bytes 1 --model opus
  --workers 1 --big-workers 6 --big-budget-usd 20 --max-run-usd 250`
  (default 27 min big sessions), stopped gracefully by request: 85 matched
  in 2 h 9 min for $160.71 (ovl5f 18, ovl62 14, ovl63 12, ovl64 9, ovl68 7,
  ovl67 7, ovl60 6, ovl5d 6, ovl69 2, ovl5e 2, ovl66 1, ovl65 1), 5 parked,
  0 failed; game code 65.75% (2332/2537). The gate sent back a no-op
  `++/--` match (`func_800B406C`) and one that didn't match; the workers
  redid them. `fold`: 76 rows from matches folded unreviewed (dedup pass
  pending, with the previous run's 56); 11 on unmatched functions in
  `<swarm dir>/patterns.unmatched-2026-10-21b.md`.

- **2026-10-21 (swarm all overlays, Opus):** run 20261021061703, `--seg
  overlays/ --model opus --workers 5 --big-workers 3 --big-budget-usd 20
  --big-minutes 15 --max-run-usd 250`: 77 matched in 67 min for $77.86
  (ovl60 24, ovl5d 12, ovl68 11, ovl62 9, ovl67 7, ovl66 5, ovl64 3,
  ovl63 3, ovl5e 2, ovl5f 1); game code 59.7% (2247/2537). It ran out of
  takeable work before its 3 h cap (attempts and big-mode staleness
  limits). Failed: 2 OAuth clashes at start; `func_800B4240` refused at
  the gate (its C uses a struct field, `unk1C`, the header lacks). Before
  it, `func_800B4268` (aacb5037) matched from the swarm's refused C with a
  K&R definition (its callers pass 3 or 4 args: the original had no
  prototype). `fold`: 70 rows; 56 from matches folded (3 merged into the
  global-array row), 14 on unmatched functions kept in
  `<swarm dir>/patterns.unmatched-2026-10-21.md`. The 56 still want a
  dedup pass against older rows.

- **2026-10-20 (swarm all overlays):** run 20261020211358, `--seg overlays/
  --model sonnet --escalate-model opus --workers 5 --big-workers 2
  --big-budget-usd 8 --big-minutes 12 --max-run-usd 40`, stopped
  gracefully by request after 18 min: 10 matched for $10.26 (ovl68 5,
  ovl62 2, ovl66 2 incl. the morning's park `func_800B34F8`, ovl64 1), 2
  failed (an OAuth refresh clash at start; `func_800B4268` refused at the
  gate, Open leads). 85 functions under 600 B had never been tried (runs
  ended on budget before them); most are still queued. `fold`: 12 rows, 4
  new, 1 example added, 2 hack rows and 4 on unmatched functions to Open
  leads, 1 duplicate dropped.

- **2026-10-20 (cleanup):** merged the sched tooling (260a4660); removed
  the two side-session worktrees and their merged branches (the BB3F0
  session's scratch kept in `scratch/bb3f0/`). BC274's third pass (flags,
  810 inserted reads, prototypes, do-while(0) copies) is in its block note.

- **2026-10-20 (hand: `func_800BB3F0`, ovl5e, 684 B):** matched with no hack
  (08aa8ff7; its copies too: `func_800C159C` in it, `func_800BA098`
  8e6f2255). From the swarm's 26/171: `-dL` showed the `player*108` chain
  moving at 85 insns (29*2*2 >= insn_count). Reading `Heroes[player]`
  directly in the loop moves its la first (threshold -3 per move), which
  keeps the chain in the loop. The last `s0/s1`, `s6/s7` swaps were global.c
  priorities (`mask` 5 refs/222 can't beat `self` 4/86). The claim in a
  do-while (0) in each arm fixed them. Worktree git gotcha added.
- **2026-10-20 (tools: `sched` on parked C, tie rules):** `sched`,
  `regions` and `vars` now compile a parked function's C under
  `#ifdef NON_MATCHING` (`-DNON_MATCHING`, into `build/regions/` and
  `build/sched/`, never the build's object). `sched`'s report counts each
  block's memory insns and names the rule behind every pick: priority,
  birthing, class, potential hazard or luid, following sched.c's lazy
  re-sort (`order from T-n`). The potential-hazard model (store > load >
  ALU at 2+ memory insns) predicted all 16.9k tie groups of five units'
  dumps. `sched func_800BC274` shows block 5's 100-over-98 luid tie with
  one memory insn. A worktree needs `asm/`, `linkers/`, `ext/` and the
  small `disc/` files copied in (git-ignored) before cpp or make work.

- **2026-10-20 (hand: `func_800B7134` as a hack match):** on the user's go,
  committed the dead-store version (ba1244e6, `Hack:` line in the commit):
  198/198, the `j = 0;` before the three sets is needed (sets first, then
  `for (j = 0; ...)`: 7/198). `done`'s lint now flags a local whose last
  store in a loop body is dead (63b06bf4); no other matched function hits
  it. `decomp.py hacks` now finds `func_800B345C` too.

- **2026-10-20 (hand: ovl5e parks `func_800BC274`, `func_800B7134`):** both
  parked, C under `NON_MATCHING`. BC274 (2/197): the a0/a1 order before
  `jal func_8001CD04` is a sched1 tie that `schedule_select`'s potential
  hazard or luid decides; the target's order needs a 2nd memory insn in that
  block, which its final code doesn't have. 47k permuter iterations and every
  shape tried fail. B7134: the inner loop now matches exactly
  (`a >>= 7` in place, the angle index in its own `k`), found by compiling a
  statement-order x form grid with cc1 directly; the outer loop still hoists
  one table base (2 insns) unless the worker's dead NULL stores stay, which
  match 198/198. Added FSF gcc 2.8.1 source to `tools/reference/gcc-2.8.1/`
  (sha256 in its README.decomp.md); the passes used here have the same rules
  as 2.7.2.3. Four Diff patterns rows.

- **2026-10-20 (swarm ovl5e, run 3, 30 min big-heavy):** run
  20261020123536, `--workers 1 --big-workers 4 --big-bytes 400
  --max-minutes 20 --big-minutes 12 --worker-minutes 12 --permute-minutes 8
  --big-permute-minutes 8 --max-run-usd 30`: 3 big matched (`func_800B6D14`
  740 B, `func_800B36D4` 552 B, `func_800B6860` 1204 B) in 27 min for
  $19.01, ~$6 a match (run 2's big mode: $20.82 for one); 4 parked after
  two big sessions without progress (`func_800BC274` 2/197, `func_800B7134`
  5/198, `func_800BB3F0`, `func_800B38FC`). 9 left. `fold`: 8 rows, 1 new,
  2 into the pointer-local row, 5 on unmatched functions to Open leads.

- **2026-10-20 (swarm ovl5e, run 2):** run 20261020112410, `--model sonnet
  --escalate-model opus --workers 5 --big-workers 2 --big-budget-usd 8
  --max-run-usd 40`: 34 matched in 63 min for $41.68 (stopped at the
  budget), no parks; 12 left (3 with checkpoints, 9 big ones not reached).
  $/match: 150-399 B ~$0.28, 400-599 B $0.90, big mode $20.82 for one.
  Permuter pool (first live record): 10 runs, 9 improved, 4 matched
  (`func_800B7BE4`, `func_800B4EE8`, `func_800B64AC`, `func_800B5E44`; 3
  went to the gate without a port session), helped by scoring by address.
  `fold`: 29 rows -> 15 new, 4 into existing rows, 3 rows on unmatched
  functions kept as leads (Open leads), duplicates merged.

- **2026-10-20 (swarm ovl64):** run 20261020083954, `--model sonnet
  --escalate-model opus --workers 4 --big-workers 2 --big-budget-usd 8
  --max-run-usd 40`: 22 matched in 22 min for $8.26 (~$0.38 a match), no
  parks; stopped with `--now` (network going down), which killed 2 big
  sessions and 2 permuter runs in flight. 24 left, mostly big. `fold`: 16
  rows, the six `Heroes[arg0]` lookup rows merged into one.

- **2026-10-20 (two parks, permuter then hand):** `func_800B5710` (ovl6a,
  2836 B, c1cb924c; ovl6a complete) matched with the permuter's change
  alone: `ang = 0x400; ang = (... % 0x1000) + ang;`. The permuter scored
  that C 20, not 0: the target names fields by their own address
  (`D_8005A677`), ours `D_8005A614+0x63`, and its scorer compared names, so
  it never reported the match. Fixed (a5ac14a0: relocations compared by
  address). `func_800B7458` (ovl69, 1360 B, 2cb2b9c4): the permuter's `off`
  temp fixed the row block; the test then needed `val`, another variable
  the target keeps in v1 (global.c's a1 preference, Diff patterns).

- **2026-10-20 (swarm ovl5e, then a permuter pass):** low-cost recipe, run
  20261020073721: 16 matched in 10 min for $1.43, stopped by request (47
  left). `fold`: 6 rows down to 2 new, 3 merged into existing rows.
  Then 4 permuter runs (stopped at ~15 of 30 min), all gains in the first
  2 min but one: `func_800B5710` 300 -> 20 and `func_800B7458` 20 -> 10
  with real C (their leads); `func_800B34F8` (ovl66) -> 15 only with a
  no-op `D_800B9EF8++; D_800B9EF8--;` (that global's reference count is
  the lever); `func_800C159C` (ovl5d, registers 45/171) 295 -> 235 at
  13 min with `(*(new_var = &h))->hero_object`, a hack that says `h`'s
  allocation is the lever.

- **2026-10-20 (permuter speed and record):** `decomp.py permute` now
  prunes `base.c` to the declarations the function uses and compiles
  candidates through `tools/scripts/permuter_fast.py`, which runs the
  Python filters in-process, reads includes from /tmp and skips a no-op
  cpp. Throughput went up 1.7x to 7x. Identical code on 58 sampled
  functions across every unit (bash, fast, pruned). `base.c` also gained
  the file's own declarations; before, a function using the file's own
  externs failed to compile there (3 of 58). Every run is recorded
  (`permstats`, the swarm's `permuter.jsonl` and `stats`). MATCHING.md, Big
  functions.

- **2026-10-20 (swarm ovl66):** low-cost recipe, run 20261020061838,
  49 matched, 1 parked (`func_800B34F8`, registers 8/92, checkpoint kept)
  in 39 min for $7.10 (~$0.15 a match); stopped early by request (`stop`,
  then `--now`, so the big session on `func_800B7B48` and two permuters were
  killed). Its first run (20261020061709) died at start on an OAuth refresh
  clash even with the 10 s start gap; `--start-gap-seconds 30` worked.
  `fold`: 14 rows down to 8 (merged duplicates, dropped two hacky ones).

- **2026-10-19 (PlotBall by hand):** matched (f0a4117d, 5576 B), from the
  swarm checkpoint at structure 777/1394; ovl61 is complete. No hacks, no
  permuter: each step came from the cc1 dumps (CSE's canonical register,
  local-alloc ties, global priorities) and small test files compiled with
  cc1. The keys were `s16` locals (gcc 2.8 extends a short when it's
  read, not set), splitting `done` into an `s32` flag and an `s16`, a
  `static inline` for the player-to-side chain, and reading `D_8005A648`
  and the `GetContour` hook as aggregate members so stores stay ordered
  (that removed the checkpoint's pointer temps and gotos). Eight Diff
  patterns rows; `D_8005AA70` is the level's `LevSpec` (ARCHITECTURE.md).
- **2026-10-19 (permuter + hand, near misses):** `swarm.mjs permute --only
  ovl67:800B39C0,ovl69:800B7458`, stopped by hand after 32 of 60 min.
  `func_800B39C0` went from registers 29/145 to 3/145 (a `do { } while
  (0)` around one assignment, found in the first minute), then matched
  by hand (ca3a674c): `D_800C418C[i]` indexed in place in the tail, not
  through the loop's pointer (Diff patterns). Gated in the swarm's `h1`
  clone because the main checkout had another session's edits.
  `func_800B7458` was skipped: its checkpoint no longer applies (Open
  leads).
- **2026-10-19 (ovl68, low cost):** run 20261019171842, stopped by hand
  after 26 min: 46 matched for $11.71, no gate failures; game code passes
  50% of bytes (2024/2537 functions). Four left unmatched: two cut off by
  the stop before their C built (`func_800B5CD0`, `func_800B65B0`, retry
  next run), `func_800B6C84` (instructions 12/128, checkpoint kept) and
  `func_800B4444` (cc1 deletes its empty first loop). Fold: 19 rows, 3
  repeats dropped.
- **2026-10-19 (hand, ovl5d):** `func_800BB64C` (364 B) matched by hand
  from the swarm park (registers 2/91), no permuter: `z` masked in place
  (one death, so local) and `x` its own local loaded before the two
  multiplies (its longer life drops its local-alloc priority). Two Diff
  patterns rows.
- **2026-10-19 (hand, ovl69):** `func_800B7458` from its checkpoint
  (registers 4/340), parked at the same score: the last swap is a global.c
  register preference (Open leads). The checkpoint's header retype of
  `Ovl69Entry.unk0` broke four integer users; it stays `s32` with a cast.
- **2026-10-19 (hand, ovl65):** `func_800B4D6C` (1720 B) matched by hand
  from the swarm's checkpoint (structure 314/430), no permuter: loop 3
  as a real loop, `Heroes[i]` indexing, separate loop variables, and
  `best` reused as the done flag (four Diff patterns rows). `decomp.py
  sched` misread hex ready-list priorities (>= 0x10); fixed.
- **2026-10-19 (ovl65, low cost):** run 20261019154152, stopped by hand
  after 38 min: 50 matched for $16.70, 0 parked. The first start failed on
  "Docker is not reachable" (Docker Desktop had quit; start it and rerun).
  Both gate failures matched by hand from the workers' C: `func_800BC820`
  (prototype said void, it returns s32), `func_800B85DC` (the tree's
  `Ovl65Ext`; `D_800BDA00`/`04` retyped `NAS *`). `func_800B4D6C` (1720 B)
  keeps its big-mode checkpoint (structure 314/430). Fold: 29 rows, 5
  repeats dropped.
- **2026-10-19 (ovl5d, low cost):** run 20261019133956, 64 matched in 67
  min for $31.79 (the $30 cap stops leasing, sessions finish). Gate
  failure `func_800B6DA4` matched by hand (the tree's `EXTobject.status`,
  an s32 return: a caller's s16 prototype was wrong). Parked
  `func_800BB64C` (registers 2/91, checkpoint kept). Fold: 40 rows, 4
  dropped (repeats, a dead-value hack).
- **2026-10-19 (tidy):** `decomp.py index` repairs the index itself: 51
  renamed functions had kept their old names and no `src`; merged-away
  entries are dropped. This log's Next is current counts and recipes
  instead of run reports; dead leads and gotchas (main.exe's, now matched)
  removed; session entries before 2026-10-16 moved to git history. Swarm
  README example is the low-cost recipe; `--big-minutes` default is 27.
- **2026-10-19 (reconcile):** `decomp.py reconcile` now keeps the tree's
  declaration over a patch's only when every C file including the patched
  header also sees the other header (`headers.visible`); before, any header
  counted, which broke `func_800C8A80` once.
- **2026-10-19 (ovl5f split):** `func_800B5588`'s four switch cases had
  been split off (and swarm-matched) as functions; a `size:` in
  sym.ovl5f.txt rejoins them and the whole function matches by hand (a
  pointer per case, like ovl61's sibling).
- **2026-10-19 (ovl5f, low cost):** 78 matched for $15.96 with Sonnet
  workers; survived an internet drop via --resume; 33 pattern rows folded
  (7 repeats dropped); one park points at a bad split in ovl5f.
- **2026-10-19 (tools):** three matching aids. (1) `decomp.py reconcile`
  (headers.py): on a declaration-clash gate refusal the swarm keeps the
  tree's declarations and gates again before paying a worker. (2)
  `aspsx_compare.py` runs Sony's real ASPSX (2.56-2.86, `ext/aspsx/`) on
  every file's matched C: identical to our pipeline on all ~1,700 gcc 2.8.1
  functions; the slot fills `aspsx_fill.py` adds (frame-only epilogue, the
  2.7.2 units) are Sony's compilers', not ASPSX's (ARCHITECTURE.md,
  Toolchain). (3) `decomp.py sched NAME` prints global.c's allocation
  order and priorities and the rule that placed each insn in sched1/sched2.
- **2026-10-19 (lint):** `done` refuses a run of `x++; x--;` statements
  (no-op pair) without --hack; only `func_800B345C` has one. Its rework got
  the registers with one natural `do { } while (0)` but not a sched1 tie
  (Open leads); the committed hack stays.
- **2026-10-19 (hand + toolchain):** `func_800C0F5C` matched with plain C
  after the build learned ASPSX's jump-slot fill for gcc 2.8.1 units
  (f8824b13: `aspsx_fill.py --jumps-only` by default; of the ten slots gcc
  2.8.1 leaves to the assembler, nine follow a store and keep their nop,
  the frame-only epilogue gets filled; `make check` OK). Gate failures
  matched from the workers' C: `InitRaceSmokeBoost`, `func_800C5620`,
  `func_800C6B9C`.
- **2026-10-19 (ovl63 swarm):** 77 matched in 51 min ($34.41) with 5+2
  workers (now the default); 1 park, 1 gate failure, 1 hack to redo
  (`func_800B345C`); 28 pattern rows folded (3 repeats and the hack
  dropped).
- **2026-10-19 (ovl5f swarm):** 9 matched in a 9-min run, stopped for the
  ovl63 7-worker trial; 6 pattern rows folded.
- **2026-10-19 (hand):** `func_800C8BA0` (ovl5f, 567ce2fb): its "+4 bytes
  of rodata" lead is gone (the id-word split fixed it); it needed all five
  cases listed so gcc builds the 0..4 table.
- **2026-10-19 (gate):** `done` now refuses other functions' C edited on
  lines that don't call the match (`stray_edits`; caught the ovl60 case,
  0 hits over the last 150 match commits), and `extract_function(top=True)`
  skips an indented call line it used to take for the definition.
- **2026-10-19 (hand):** ovl60's two gate failures matched from the
  workers' C: `func_800BC184` (6ff3b7da; the header already had the slot
  field, plus a pad view) and `Roo_ChangeDirOnEdge` (b0e170e0; without its
  patch's stray edit to `func_800BE864`).
- **2026-10-19 (ovl60 swarm):** 113 matched in 60 min ($46.61, 0 parked,
  2 gate failures to retry); 40 Diff pattern rows folded.
- **2026-10-19 (ovl62 swarm):** 117 matched in 60 min ($46.59, 0 parked) on
  opus workers after the resplit; fold added 39 Diff pattern rows (10
  repeats of existing rows dropped).
- **2026-10-19 (ovl62 splits):** the first ovl62 swarm run stopped after a
  minute (5 workers hit an OAuth refresh race at once), but its one park
  found ten AST names that `5bf518f2` put in `ovl62`, each cutting a real
  function mid-code. They belong in `ovl63` (Rippled, InnerRipple) and
  `ovl6a` (PathEffect, CrystalFlash, SparkGFX); moved, ovl62 resplit (now
  186 functions), `make check` OK (ARCHITECTURE.md, Which game is which). A
  sweep for any function whose predecessor doesn't end in a jump finds no
  others.
- **2026-10-19 (log):** the three gate failures listed under Next
  (`func_800BA43C`, `func_800B3E74`, `func_800B7CE8`) were already matched
  on 2026-10-18; Next now says so.
- **2026-10-19 (hand):** `func_800B8A68` (ovl61, 3264 B) matched (fc3176ef)
  from the swarm checkpoint (144/150 blocks), without its `min + k - k`: the
  dead skill value needs a use that combine cancels after flow, now
  `(e->unk10.x + k) - (pos.x + k)` (user's choice; still a no-op term). The
  register spots fell to field-form bases, repeated case-3 bodies instead of
  gotos, `abs()` in both arms, and single-set locals. Five Diff patterns
  rows added, one extended.
- **2026-10-19 (hand):** the id word is its own rodata unit in the other 13
  slot overlays too (95e2fa09, byte-identical), and ovl61's two `&&label`
  tables are `switch` statements again through the gate: `func_800B68C8`
  (2d811ef3), `Plot_N_Gin` (1b763319). No other first file had any.
- **2026-10-19 (hand):** `func_800B488C` (ovl6a, 1032 B) matched (ee24f281)
  with the swarm's real-switch C. The +4-byte link failure was the overlay id
  word sharing `ovl6a.c`'s rodata: split into its own unit (815cad97),
  byte-identical. ARCHITECTURE.md's slot-overlay layout says why; one Diff
  patterns row extended, one added.
- **2026-10-18 (hand):** `func_800B5710` (ovl6a, 2836 B) taken from the
  swarm checkpoint (529/721 differing) to 120/121 blocks and parked on one
  delay-slot fill in the 0x80 block (Open leads). Four Diff patterns rows
  (constant held across two calls, a spill decided by live length, base-first
  `ARR[i].field`, a switch on a global read once) and three examples added.
- **2026-10-18 (hand):** the swarm's three other gate refusals fixed and
  committed: `func_800BA43C` (ovl67, 067bc83b: a third, unused bool parameter
  like ovl61's ArkHeroRad2ObjectCollision), `func_800B3E74` (ovl67, c079de93:
  conflicting prototypes merged; `func_800B4138` takes one argument but this
  caller passes two, so a cast call), `func_800B7CE8` (ovl69, 3ba0a413: its own
  types dropped for the header's `Ovl69Entry`). New gotcha on the recipe.
- **2026-10-18 (hand):** `func_800B4EAC` (ovl6a) matched (36eb852e) from the
  swarm's big-mode C, which matched but failed the gate on compile errors: its
  D_8005A614 view (a second `Ovl6aGameView`) merged into the file's own, stale
  prototypes dropped, and `func_800B7B38` (u16 return) called through an s32
  cast as the file's other caller does. `func_800B551C` turned out matched by
  the swarm (9c5b1dd8). The swarm's other gate failures (ovl67
  `func_800BA43C`, `func_800B3E74`; ovl69 `func_800B7CE8`) are likely the
  same: apply results/<key>/*gatefix*/patch.diff and fix the headers.
- **2026-10-18 (swarm):** ovl6a run 20261018184703, `--workers 3 --big-workers 2
  --permuters 2`, 87 min, $47.98: 64 matched ($0.75 each), 2 failed at the
  gate. Fold: 37 rows (a volatile and a frame-padding workaround dropped).
- **2026-10-18 (swarm):** ovl69 run 20261018173050, `--workers 3 --big-workers 1
  --permuters 2`, 75 min, $33.21: 48 matched ($0.69 each), 1 failed at the gate
  (compile error against the run's newer headers). Fold: 20 rows. Then ovl6a.
- **2026-10-18 (swarm):** ovl67 run 20261018163029, `--workers 3 --big-workers 2
  --permuters 2`, 58 min, $39.26: 82 matched, 1 parked, 2 failed at the gate
  (compile errors against the run's newer headers; see Next). Fold: 32 rows,
  3 more examples on existing rows; a volatile workaround and a shared
  prototype change dropped.
- **2026-10-18 (hand):** `func_800B9CB8` (ovl67, 1924 B) matched (e4529709) by
  porting ovl61's `func_800BE4D4`: no grab path, its own reach boxes
  (`D_800C4190`; the hero's y read once into a separate `hy` for top/bot) and
  one combined whatami-2 test ending in `KillNAS`. Three fixes after the first
  compile: `hero` assigned after the near point, `hy` apart from the loop's
  `y`, and ovl61's `-b + a` get_angle arguments kept (the plain `a - b` I
  wrote while porting loads the operands the other way).
- **2026-10-18 (hand):** `func_800BE4D4` (2816 B) matched (9fd67e11), no hack
  or permuter, from the swarm's 699/704 paired. The sin indexes' register copies
  (only here and in ovl67's `func_800B9CB8` in the whole game) are `u16` copies
  of the angles made before `abs()`, one pair per block: combine.c refuses to
  merge across the call and sched.c moves block-local copies after it. Then the
  second sin/cos copy's roles filled with the variables global.c had already
  put in the target's registers, and dz/x1 for the height test. Takes a second,
  unused parameter (its matched callers pass two). MATCHING.md: two rows.
  ovl67's `func_800B9CB8` (the same code) was ported from it the same day.
- **2026-10-18 (hand):** `func_800B85DC` matched (5b63fbe1), no register pin
  or permuter, from the swarm's 182/226 (s-reg rotation only). Worked through
  global.c's priority (floor_log2(refs) * refs / live) per pseudo from -dl:
  the target's order lim > h > arg0 > arg1 needs `arg2 *= arg2` (7 refs, not
  4) and loop 2's refs loop-weighted, i.e. its goto-loop body in
  `do { } while (0)` (a real while/for/do loop lets loop.c hoist the (s16)
  casts: 135 diffs). Plus a3 for loop 1's own hero pointer, block-scoped
  Heroes bases, and `goto ret0` for the xori test. MATCHING.md: two rows.
- **2026-10-18 (hand):** `CalcArkInfluence` matched (b46ab0f8), no hack or
  permuter, from the swarm's 146/150 (z2 in v1, three big sessions and a
  permuter run stuck there). From the -dl/-dg dumps and global.c: x2/z2
  overlapped (all shifts before the adds) and z2 inherited a v1 preference
  from its product, whose mult operand was the block-local `(s16)sz` in v1
  (set_preference, then expand_preferences). One temp `t` reused with
  `x1 += t` fixed the order; an `s32 scale` assigned `(s16)sx` then
  `(s16)sz` (dies twice: global, so no inherited preference) put t in v0.
  MATCHING.md: two rows.
- **2026-10-18 (swarm):** ovl61 run 20261018125240, `--workers 1
  --big-workers 4 --permuters 2` (31 of the 34 left were >= 600 B), 128 min,
  $88.97 (in-flight sessions run past the $80 cap): 28 matched ($3.18 each),
  0 parked; ended by the spend cap. Before it, `func_800B7518` by hand from
  the last run's dropped permuter match (worker patch + `permute-apply`).
  `bigPermuteMinutes` default 60 -> 30.
- **2026-10-18 (dashboard):** `decomp.py progress` 10.8 s -> 2.4 s (every
  `done` and each swarm batch paid it): the typed scan (a regex per matched
  function, ~8 s) is cached per source file in `build/cache/typed.json`, and
  the rodata cache key's mtimes are read once, not per segment. Same summary.
  The page moved to `tools/scripts/dashboard.html`; it gets the history packed
  (columns, per-unit changes only) and the code map: 474 KB -> 136 KB, ~160 ms
  to load. New: per-day bars, a table view, a treemap of every game function
  (click a unit to zoom), units with stacked bars and sparklines, largest left,
  recent and parked lists. The swarm writes `docs/swarm.js` (`--page`), and
  the page polls it as a script (works from `file://`). Smoke run (scripted
  worker, bare remote) showed the card follow a run live; 2 new swarm tests.
- **2026-10-18 (swarm):** ovl61 run 20261018104310, 80 min, $35.51: 55
  matched (about $0.65 each), 1 parked, stopped by hand with 35 left (see
  Next). Gotcha: `run --dry-run` writes its fake "parked: dry run" results
  to `history.json` and charges attempts; I deleted its 89 entries before
  the real run (backup `history.json.bak-dryrun` in the swarm dir).
- **2026-10-18 (hand):** `PlotScriptFX` parked at regions 95/112 blocks,
  484/520 exact, no hack or permuter (the swarm had 87/112 with a dead
  store and volatile casts). C under `#ifdef NON_MATCHING`; the
  `decomp.py block` note lists what was solved (the hoisted 1 via `case 1`
  first and two `unk52 = 1` stores that reload_cse deletes, `mode` read
  through `EngineGameView`) and what is left (block 21's keyframe copies,
  `f`/`cur` in s1/s2). New `ScriptKey` struct. MATCHING.md: three rows, and
  the loop-rotation rule corrected (`while (1) { if (..) break; }` stays
  top-tested).
- **2026-10-18 (hand):** `func_80078DD4` matched (308bf0e), no hack or
  permuter, from the swarm's 156/166 checkpoint (stuck on a b/t `a3`/`v1`
  swap). global.c's `set_preference` gives arg 4's variable `a3`, and t can
  always take `v1`, so no ordering of b and t works. Reusing `dx` for b does:
  dx is live in the other branch while that call sets `a3 = 0`, so
  `prune_preferences` drops the preference. Plus the target's b-first/t-last
  arm order. `EngineGameView` names `unk4`.
- **2026-10-18 (hand):** `func_80093854` matched (8ef8052), no hack or
  permuter, from the swarm's 66/106 checkpoint. Worked through by hand from
  global.c, loop.c, stmt.c, sched.c and local-alloc.c: its nine `s` registers
  need the outer loop's refs weighted, but a real loop hoists `%hi(D_8009FA14)`
  into a tenth. A `goto` loop with its body in `do { } while (0)` gives both
  (loop.c skips the region as phony). Then a shared scan counter (it ties `p`
  and wins on pseudo number), and block 0's `D_80052A54` index before its
  local. `EngineRec32` names `unk4`. MATCHING.md: four rules, four rows.
- **2026-10-18 (hand):** `func_80095258` matched (99f2fe3), no hack or
  permuter. The sibling `func_80094A28`'s C was right but for a t0/t1 swap
  of the parameter and the loop's hoisted `%hi(D_800A0C70)` copy. From
  global.c, flow.c, sched.c and local-alloc.c: the copy's life is doubled
  (`update_equiv_regs`, a constant), so it ranked 5/42 against the
  parameter's 5/32. A `do { } while (0)` around the counted loop (not the
  NULL test) keeps the hoisted insns at depth 2: 8 refs against 6, matched
  on the first compile. MATCHING.md: global.c rules and a Diff patterns row.
- **2026-10-18 (hand):** `func_8008C824` matched (d3e79ab) from the swarm's
  25/155, no hack, no permuter. global.c ranked the two loop givs 27/77
  against 25/76. A doubly nested `do { } while (0)` around the loop's first
  five statements gives the spark giv's store depth 4. Splitting
  `a -= Random()` makes the first insn after the notes a cost-1 `subu`.
  Diff patterns row; global.c and loop-note rules added to "Reading the
  compiler's rules".

- **2026-10-18 (hand):** `func_8007B330` matched (7233c73) from 7/307, no
  hack, no permuter. sched.c's birthing and tie rules gave per-block index
  variables plus a twice-assigned Heroes base. local-alloc's tie rule gave
  separate panel pointers, each panel's stores in `do { } while (0)`. Two
  Diff patterns rows.
- **2026-10-18 (hand):** `func_8007F314` (5772 B) taken from the swarm's
  1263/1443 to 1147/1443 and parked (Open leads). How jump2 merges the
  shared `ret = 0` tails is now worked out; still missing is the C that
  gives case 0's copy an existing label.
- **2026-10-18 (hand):** `func_800926CC` matched (01be8b2): `j = 1` inside
  `do { } while (0)` beats sched1's birthing priority and wins `j` a0. No
  parks left in the engine; 7 functions to go.
- **2026-10-18 (hand, third try):** `func_8008A924` matched without a hack
  (9787456). The target's unused `lbu` is half of `run_walk = run_walk`;
  post-reload CSE deletes the store. A `do { } while (0)` around the read
  gives the reloaded pointer loop-weighted refs, so it keeps `v0`. Two new
  Diff patterns rows.
- **2026-10-18 (hand, second try):** `func_8008A924` still parked. A `$3` pin
  matches (not committed: a workaround). A dead read gets the dying base's
  register, so the original's value was live at allocation; an if/else whose
  arms merge in jump2 drops the read too (delete_computation).
- **2026-10-18 (hand, no permuter):** `func_8008A924` (120 B) parked at
  registers 1/30. Its dead read of `run_walk` must be volatile and lands in
  `v0` (the dying base) where the target has `v1`; the block note lists what
  was tried. The permuter ran in parallel on the three register-only engine
  near misses: `func_800939FC` matched in 5 min (c7cf408); `func_8007B330`
  (registers 7/307) and `func_8008C824` (25/155) made no progress in 20 min
  before it was stopped.
- **2026-10-18:** `decomp.py hacks`: matched functions that need a register
  pin or asm barrier, with their match commit's `Hack:` reason (read from
  the source, so it can't go stale). CONVENTIONS points at it.
- **2026-10-17 (hand, no permuter):** `func_800926CC` from the swarm's 4/134 to
  1/134 and parked. The misplaced `li a0,1` was sched1's birthing priority on a
  single-set pseudo; `s8 j` avoids it, but post-reload CSE then turns the
  compare's `li a1,1` into `move a1,a0`. The target needs a HI/QI `j` set twice
  with the extra set costing nothing; the `decomp.py block` note lists what was
  proven and tried. New Diff patterns row for the birthing/SUBREG trick.
- **2026-10-17 midday (engine swarm):** runs 20261017113732 (60 min, 10
  matched, $63.10) and 20261017124451 (15 min, killed at the end of the
  session: 2 matched, $6.37; `func_8007F314` and `PlotScriptFX` sessions
  cut short). 0 parked. Folded 15 rows from matched functions.
- **2026-10-17 morning (engine swarm):** run 20261017101444, 60 min: 23
  matched (688-1580 B, $65.81), parked `func_8008C824`. Docker and the login
  were refreshed before launch; no failures. Folded 23 rows from matched
  functions (dropped 4 from unmatched ones).
- **2026-10-17 (engine swarm):** run 20261017064912, stopped at 38 min
  (internet down): 9 matched (688-948 B, $28.30), parked `func_80078DD4`
  and `func_800926CC`. The first launch failed in seconds: the OAuth token
  had expired overnight and 4 workers raced to refresh it (uncharged, but it
  writes error notes into history.json that feed later prompts; removed by
  hand). Gotcha: after a long idle, run `claude -p "reply ok"` once before
  `swarm.mjs run`. Folded 9 rows from matched functions.
- **2026-10-16 night (engine swarm):** run 20261016213103, 60 min, 1 normal
  + 4 big workers: 19 matched (incl. `InitHealth`, `PlotHealth`), 0 parked,
  $56.63. The two small engine functions left (`func_80095258`,
  `func_8008A924`: inline asm) are at max attempts, so the normal worker
  idled. Folded 18 pattern rows from matched functions; dropped rows from
  still-unmatched functions and `func_8002D4F4` duplicates.
- **2026-10-16 (docs, cleanup):** ROADMAP's "match every game function in
  the executable" ticked; README, ARCHITECTURE, MATCHING and CLAUDE.md
  updated for the 2.7.2 units' settings and `tools/reference/`. Fixed the
  2.7.2 unit's `EXTRA_CC1FLAGS` override dropping `-fno-builtin`.

- **2026-10-16 (reference):** FSF gcc 2.7.2.3's C compiler and MIPS
  target source added under `tools/reference/gcc-2.7.2.3/` (read-only,
  vendored, GPL; `README.decomp.md` maps passes to files).

- **2026-10-16 (by hand + tool, `func_8002D4F4`):** matched (710a20d);
  main.exe is complete. The last slot (state 6) was the compiler, not the
  C: read gcc-2.7.2.3 `reorg.c` (now in `tools/reference/gcc-2.7.2.3/`) and
  found FSF reorg fills a `j` after the branches, Sony's (like 2.7.2-cdk
  and 2.8.1-psx on a lab copy) before them. `aspsx_fill.py`'s new
  `refill_from_jump` redoes that order for a `beq` + `j` (07750fc, 5 tests);
  every matched function of both 2.7.2 units still matches, `make check` OK.

- **2026-10-16 (by hand, `func_8002D4F4`):** parked at 145/148 (was 144,
  length 592 -> 590). Found that the 2.7.2 unit compiles without cse2
  (`-fno-rerun-cse-after-loop`, Makefile; `CC1FLAGS` is now `=` so
  per-unit `EXTRA_CC1FLAGS` applies): array accesses through a constant
  base keep `la` + offsets, the `D_8006788D` symbol is just
  `D_8006788C[1]`, and `func_8002DE2C` matches without its always-zero
  offset. `make check` OK. Left: one reorg delay-slot choice in state 6,
  which turned out to be a compiler-build difference (Open leads). No
  permuter.

- **2026-10-16 (by hand, `func_800248A0`):** matched (03bef8c), no
  permuter. The caller passes x and y as shorts (`lh`, `sll/sra`), so the
  definition is K&R with `short x, y;`. gcc copies a0 at entry and emits the
  short conversion into s5; combine makes the test read the entry copy, which
  stays local in a0. The earlier attempts' `s16` prototype or local put the
  s5 move after the a2/a3 copies (Diff patterns row). main.exe: only
  `func_8002D4F4` is left.

- **2026-10-16 (by hand, `func_8001795C`):** matched (cfeda67), no
  permuter. Started from the sibling `func_80017EE8`'s arm form (clut
  stored in each arm, cross-jumped) on the checkpoint's setup: 6/107, a
  tie in local-alloc priority between the `face->tex` and `mc->uvs` offset
  temps (`-dl`). Setting `ts = &model->texs` early took `ts` out of
  `t = *tex`'s load-delay slot at the block end, so the tex temp dies
  sooner and wins a3 (Diff patterns row).

- **2026-10-16 (by hand, `func_800177F4`):** matched (c8e99ad), no
  permuter. The target's tail (both loads per arm, shared `andi/or` and
  `sw/sh` behind labels) is cross-jumped per-arm stores: writing the store
  in each arm makes tpage arm-local in a0, so idx's a0 preference is
  pruned and the face-first add allocates as retail. The earlier session
  had chased the preference with carrier variables (Diff patterns row).

- **2026-10-16 (tool, ASPSX conditional-branch slots):** `aspsx_fill.py`
  fills the slots of branches gcc 2.7.2 leaves to the assembler: from
  below, else from the target (or a `j`'s slot there), with an
  instruction whose register is dead on the other path (path-following
  liveness; a load's write doesn't count; a return reads everything); a
  target instruction every branch took and nothing falls into is dropped.
  Replaces `steal_jump_slot`. Every matched 2.7.2 function still matches;
  `func_8002D4F4` 126 -> 142/148 blocks, then a big-mode run (4 sessions,
  $9.63) took it to 144/148 and parked it (Open leads). Swarm fix: a big
  session that errored before any work (an OAuth refresh failure) was
  measured as "no progress" and parked the function; now an uncharged
  failure (fa40cae).

- **2026-10-16 (swarm, main.exe retry, run 20261016133425):** `decomp.py
  unblock` (new) unparked main.exe's 15 parks; big mode only, no
  permuter, 90 min / $100 cap. 12 matched in 75 min for $53.46:
  `func_8002E25C`, `func_800155FC`, `func_80010488`, `func_80026D08`,
  `func_80011E80` (refused once by the gate after a neighbour's commit,
  redone in 1.6 min), `func_80020C04`, `func_80010C70`, `func_80010E8C`,
  `func_8001A82C`, `func_80020680`, `func_80017164`, `func_80010000`.
  Re-parked: `func_800248A0`, `func_8001795C`, `func_800177F4` (1/89).
  `func_8002D4F4` to 126/148 blocks; its rest needs `aspsx_fill.py`.
  Folded 16 Diff patterns rows.

- **2026-10-16 (assets: the rest):** every DAT entry but the 15 code
  overlays is decoded. Models: all 400 (16-byte vertices were normals,
  +0x28; 27 have no meshes), texture coordinates and colours from main.c's
  func_80017EE8/func_80017D90 (UV table at 0x24 + [0x24], colours at 0x20 +
  [0x20], texture runs; image N of the set before the model), and
  animations (func_8001C0F0: keyframes as index lists into a vertex pool)
  exported as model.glb (morph targets, KHR_mesh_quantization; 259 MB).
  Music: SEQ <-> MIDI, all 57 byte for byte. Checked in a three.js viewer
  (Uka Uka talking). Re-extract from the disc + disc-check OK.

- **2026-10-16 (assets: sound packs):** `sfx.py`: all 160 packs to
  `<id>.sfx/` (bank.vh/.vb, music_N.seq, a WAV per sample) and back byte
  for byte; an edited WAV is ADPCM-encoded (55-70 dB SNR), keeping its
  loop/one-shot shape. Offset table: VB, VH, SEQs (the last runs to EOF;
  effects packs have one empty SEQ). 241 raw -> 81 (the odd models).

- **2026-10-16 (disposable build/):** `make clean` deletes all of `build/`;
  `build/scratch` -> `scratch/`; splat's `asset_path` -> `asm/bin` (the
  Makefile had been building all 992 DAT files from `assets/` into
  objects); `pack_disc` uses `disc/files` (no `build/disc-cache`);
  `decomp.py extract` also extracts `assets/` (never over edits). Checked by
  deleting build/, asm/, linkers/, assets/ and running the Quick Start.

- **2026-10-16 (assets: screens, text):** `tga.py` (5 title screens, plain
  TGA, to PNG) and `txt.py` (12 modes' text, strings in fixed slots
  because data points at them by address; four files keep a cut string's
  tail after its terminator) round-trip byte for byte; disc-check OK.
  `BASHY.` is 30 MB of zeros (filler), `SPYRO3/` the bundled demo: all of
  Crash Bash's data is in the DAT. Next: the 160 sound packs (VAB/SEQ).

- **2026-10-16 (assets: models):** `tools/scripts/mdl.py`: model files'
  meshes to OBJ (`<id>.mdl/`, geometry only) and back byte for byte, 319
  of 400 (CTR-tools' BashMesh reading; the other 81 differ); `dat.py` has
  a FORMATS table and saves index.json however decode ends. disc-check OK.
  CTR-tools' file-name list (GPL-2.0, the repo is private: the user said
  commit it) is `configs/USA/dat_names.txt`; names go into index.json.
  Next: texture coordinates (per-face arrays), sound banks.

- **2026-10-16 (assets, stage 2: textures):** `tools/scripts/tex.py`: the
  400 texture sets (type 8) to PNGs (`<id>.tex/`, true colours) and back
  byte for byte; `dat.py extract` decodes them, `make disc` packs them
  (cached in build/dat-cache/); disc-check OK; Pillow in the Docker image.
  Image->CLUT pairing and depth from CTR-tools' reading (the s16 at +12 is
  CLUT << 1 | 8-bit). Formats in the new docs/ASSETS.md; next: models (the
  odd entries), sound banks, text.

- **2026-10-16 (assets, stage 1):** `tools/scripts/dat.py` splits
  `CRASHBSH.DAT` into `assets/CRASHBSH/` (992 entries, not 993; plus
  `filler.bin`: entry padding and 12 unreferenced sectors are not zeros),
  and `make disc` builds the DAT from it (extracting first if missing);
  `disc-check` OK. Stage 2, decoding formats (textures, sounds, models), is
  not started.

- **2026-10-16 (supervised engine run):** 60 min, $41.11, 28 matched
  (`func_80085890` by hand: callers in ovl63/ovl65 passed no argument).
  Fixed: `checkPatch` read an added blank line before a context
  INCLUDE_ASM as an add (`s*` spanned lines) and refused
  `func_800939FC`'s best session; `verify_build` hid cc1's errors.
  Folded 67 pattern rows (dropped a K&R-to-dodge-callers hack and an
  unsolved row).

- **2026-10-16 (tooling plan done):** `--chain N` session chaining (off by
  default); a scripted run checked the stale-checkpoint hand-off, keeping a
  park's C and chaining. Every Part 1-3 code item is in.

- **2026-10-16 (tooling plan 3.4-3.9, paused):** `next` asm with offsets,
  jump tables, strings, callees; `try` LINK check and disassembly at the
  first difference; faster tools/dk/make/regions; permuter keeps GTE macros
  and applies in-run matches; Dockerfile pins maspsx/m2c/permuter (make
  verify OK); stall nudge; `splitcheck`; `port`.

- **2026-10-16 (tooling plan 3.2):** parks keep their closest C as the
  function's checkpoint and every attempt resumes from it. A `--worker-cmd`
  smoke run caught two 3.3 bugs that stopped runs at startup (no `build/` in
  a new clone; the queue copy couldn't import `regions`); fixed.

- **2026-10-16 (tooling plan 3.3):** `similar --survey`; `next` prints a
  matched look-alike's C (seq >= 0.90); queue `twin` field (exact / near /
  template) keeps adapt jobs out of big mode; swarm seeds cross-overlay
  identical groups and holds back a tried lead's twins.

- **2026-10-16 (tooling plan 3.1):** a worker session's git status changes
  every job and cost ~11k tokens of cache writes per session; workers now run
  without git instructions, memory, skills, TodoWrite. Prompts carry the loop
  inline. Real-session probe: the CLI ignores `Write(path)` allow rules, so
  1.8's result-file rule is now `Edit(.swarm-result.json)` (gotcha below).

- **2026-10-16 (tooling plan 3.5):** `try`/`make diff` show target and
  compiled aligned over the whole function (`regions.diff_rows`: register,
  operand, moved, missing, extra) with a `KINDS` count, identical runs cut;
  `DIFF`/`STAGE` lines unchanged. `regions` caps its table at 12 blocks
  and has `--changed`; the big-mode prompt mentions it.

- **2026-10-16 (tooling plan Part 2):** workers get `--session-id`; killed
  sessions are priced from their transcripts (prices fitted from 745 result
  lines, within 2%); `ledger.jsonl` (backfilled for 760 sessions, $453) and
  `stats` $/match by size and mode. Docs are read ~0.14 times per big
  session; cache writes are 34-49% of spend. Usage-limit detection deferred.

- **2026-10-16 (tooling plan Part 1):** all nine items, one commit each,
  with `tools/swarm/swarm.test.mjs` (node:test, 32 cases) and new
  `test_queue.py`/`test_lint.py`. Slot-overlay names go to tools as keys
  (`ovl5d:800B3A38`), checkpoints/results keyed by key; `reapply` keeps
  whole declarations; environment errors stop runs uncharged; unapplicable
  checkpoints get ported; escalation and notes carry across runs; asm-only
  matches go to report.md for a human `--hack`; gate lint covers every asm
  spelling and K&R definitions; workers are sandboxed from the host.
  propagate: `decl_name` misread function-pointer externs, which hid
  `D_8009D73C` from `ovl64:800BB52C`'s copy; fixed, and it matched
  (1021). Checked with a scripted `--worker-cmd` run on a local bare remote.

- **2026-10-16 (review, no code):** compared decomp.wtf's tools with ours;
  plan in TOOLING_PLAN.md (done, removed after 94f04c7). Found swarm bugs: shared overlay names are
  ambiguous to `find_func`, checkpoints keyed by name, `reapply` drops
  header lines (reproduced), Docker loss not detected, killed big sessions
  uncosted, workers can rewrite `tools/dk` on the host.
- **2026-10-10 .. 2026-10-15:** project setup, the overlay splits, the swarm
  and the first matching runs. Those entries are in git history:
  `git show 9c6e0800:docs/AGENT_LOG.md`.
