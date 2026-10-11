# Agent log

Newest first. Each session appends: what changed, what was learned, what's next.

## 2026-10-11 - func_80015780 unparked

**State at end of session:** `done` full build OK; 404/2550 game functions
matched (main.exe 158/574), 0 parked.

**Done**
- Matched `func_80015780` (`ba9c5b0`). The source walks the list with an
  index: `p = &pts[i]` is the pointer it loads through, and `pt++` is the
  separate pointer it returns.

**Learned / gotchas**
- An instruction stuck after a hoisted loop constant comes from source
  order. A pointer init that the target places *after* the constant
  (`li a0,-1` then `move s0,s1`) is a strength-reduced giv. loop.c emits
  giv inits after the moved invariants. Write it as `&base[i]` with an `i`
  counter.
- Reduced giv increments go where the counter's `i++` is. Put `i++` before
  or after the other pointer's `++` to set the order of the two `addiu`s.
- Whether loop.c hoists a constant depends on the loop's size
  (`threshold * savings * lifetime >= insn_count`). Writing `&pts[i]` twice
  cost two address computations and the `-1` stayed in the loop. Computing
  it once into a local (`p = &pts[i]`) brought it back out.

**GTE batch (same day):** matched `func_8001D048` (4-vertex `rtps` into a
`POLY_GT4`, `addPrim`), `func_8002AEE0` (finish a VAB transfer, then GTE
setup), `func_8001C974`, `func_8001C690`, and `func_80015C1C` (points to
`TILE_1`s). That's 409/2550. Parked `func_800177F4` and `func_8001C7FC`;
the `block` notes say what still differs.
- New types: `GfxState.ot`/`otzAdd`/`otLen`/`farColor`/`dp`, `GfxModel`,
  `RelTable`, `VabFile`/`VabSlot`, `PointSprites`/`PointSpriteSet`.
- `gte_ldv0_w(xy, z)` in `bash/main.h`: the game loads vector 0 with
  `lw` + `mtc2`, which no Sony macro does. Load the arguments into locals
  before a store macro when the target's `lw`s come first.
- `lw t4/t5/t6` + `ctc2` comes from Sony's `gte_SetRotMatrix` and its siblings.
  Pass one `MATRIX *` local to both `gte_SetRotMatrix` and
  `gte_SetTransMatrix`, so the pointer is computed once.
- `addiu a0,a0,0x6C` and then fields at small offsets means the code took a
  pointer to a sub-struct (`&ps->set`).
- The words of a POLY can be updated whole (`*(u32 *)&poly->r0 |= 0x02000000`
  for semi-transparency, not `setSemiTrans`).

**Next**
1. Unpark `func_8001C7FC` (scheduling only) and `func_800177F4` (registers).
2. More of `main.exe` (`/match-function`). Of the GTE functions only
   the larger ones are left.

## 2026-10-10 - GTE functions

**State at end of session:** `make verify` -> 16 targets OK; 403/2550 game
functions matched (main.exe 157/574), 1 parked.

**Done**
- Matched five GTE functions through `done`: `func_8002B760` and
  `func_8002B6D0` (rtps of a position relative to a `View`, through the
  `GteScratch` at `0x800569CC`), `func_8001C008` and `func_8001C0F0` (vertex
  interpolation with `gte_intpl` over a `MorphModel` run list; the second
  picks vertices through two index lists), and `func_8001CD04` (loads a
  `GteSetup` from `func_8001965C` into the GTE: `gte_SetGeomOffset`,
  `gte_SetTransVector`, `gte_SetRotMatrix`).
- New types in `bash/main.h`: `View`, `GteScratch`, `MorphModel`,
  `GfxState` (placeholder for the 161-reference global at `0x80056998`; only
  the fields `func_8001CD04` touches), `GteSetup`, `FlagPoint`/`PointList`.
  `EXTengine` gained `unk3C` (split from `unk38`) and `unk5E` is `u16`.
- Parked `func_80015780` (first flagged point of a subengine, transformed
  with `gte_rt`): 2 instructions off, best C under `NON_MATCHING`.
- `docs/MATCHING.md`: when gcc rotates a loop and the target doesn't.

**Learned / gotchas**
- splat marks functions with cop2 moves as `/* Handwritten function */`; they
  are ordinary C with Sony's macros (`gte_ldsv`, `gte_ldfcdir`, `gte_stsv`,
  `gte_SetGeomOffset`, ...).
- gcc 2.8 rotates a loop (moves its exit test to the bottom) only when the
  test jumps to the loop's own end label. A target with the test at the top
  and `j` back at the bottom came from an exit by `return` or `goto` past
  the loop; that keeps loop.c's hoisting, unlike a hand-made goto loop.
- `gte_intpl_b()` (no nops) when the target has other instructions right
  before `intpl`.
- Two pointers stepping in lockstep (one loaded through, one returned) were
  two variables in the source.
- Bash heredocs collapse `\\n` in Python: write generator scripts with the
  Write tool (bit me twice this session).
- `make permute` runs in its own container; stopping the background task
  doesn't stop it: `docker stop` it before running `done`.

**Next**
1. `func_80015780`: the hoisted `li -1` is scheduled after the pointer copy;
   try declaring the locals in a different order or moving the copy.
2. Remaining GTE functions in the exe: `func_8001D048`, `func_8002AEE0`,
   `func_8001C974`, `func_800177F4`, `func_8001C690`, `func_8001C7FC`,
   `func_80015C1C`, then the larger ones.

## 2026-10-10 - Phase 2: GTE code

**State at end of session:** `make verify` -> 16 targets OK; 398/2550 game
functions matched (main.exe 152/574). First Phase 2 item ticked.

**Done**
- `include/psyq.h` includes Sony's `inline_c.h` and `gtemac.h` (PsyQ 4.7,
  unmodified). Their GTE commands are DMPSX placeholder words
  (`.word (index << 6) | 0x3f`); `tools/scripts/dmpsx.py` rewrites them to
  cop2 instructions inside `#APP` blocks, between cc1 and maspsx, in the
  Makefile pipelines and `decomp.py`'s (permuter, probes). Unknown
  placeholders stop the build. Every cop2 command in the game's asm is in
  the table.
- `obj_func` now runs `objdump -z`: without it, runs of zero words (the GTE's
  two nops) were printed as `...` and dropped from `make diff`, so a correct
  object looked 2 instructions short.
- Matched `func_80015758` (`gte_ldv0`, `gte_rt`, `gte_stlvl`; an entry of
  the engine API table that `func_8001492C` fills, at `+0x194`).
- `docs/MATCHING.md` has a GTE section (how to read `mvmva` fields into a
  macro name).

**Learned / gotchas**
- The table's `ll`/`lc`/`sqr` entries (lm bit, BK for `lc`) come from the
  command encodings, not from a retail match; only `rtps`, `rtpt`, `rt`,
  `dpcs`, `intpl`, `nclip`, `avsz3` are used by game code in the asm. If one
  of the others ever mismatches by a bit, check the table first.
- `gte_rt` and `gte_rtv0tr` are the same instruction.
- Grepping asm for `4A`/`4B` opcode bytes also hits ASCII in data ("MARK").

**Next**
1. The other GTE functions (`func_8002B760`, `func_8002B6D0` are
   `rtps` + `gte_stlvl` + `gte_stsxy`; `func_80015C1C` is marked
   handwritten by splat, check whether it's C with `gte_SetRotMatrix`).
2. Keep matching `main.exe` (`/match-function`).

## 2026-10-10 - Phase 1 exit check

**State at end of session:** `make verify` -> 16 targets OK; 397/2550 game
functions matched (main.exe 151/574).

**Done**
- Tested Phase 1's exit condition ("matching `main.exe` no longer stalls on
  missing names or types") two ways:
  - m2c-drafted all 427 unmatched `main.exe` functions with the headers as
    context. 4 failed: 3 because callbacks were declared as data
    (`extern u8 func_X[]`, now `void func_X();`, also in `ovl60.h`), 1
    because m2c can't follow a tail branch to its own label
    (`func_80011BC0`, a loop). 29 more contain GTE instructions m2c doesn't
    know (`ctc2`/`mtc2`/`lwc2`); that's a GTE-macro matching task, not a
    missing type.
  - Hand-matched four through `done`, each on the first diff: `AddMotion`,
    `func_8001D3B0` (chains an array of subengines), `AddEngine`, and
    `func_80025BBC` (a median-sized one, chosen by size, not by type). All
    used the Phase 1 types unchanged. The only header work was declaring the
    globals and callees each one uses, as CONVENTIONS.md expects.
- `func_8001D390` retyped from `void(void)` to `s32 (EXTengine *, s32)`:
  `AddEngine` calls it with arguments and tests the result.

**Learned / gotchas**
- 185 `D_` globals that unmatched `main.exe` code uses are still undeclared.
  They get declared (and typed) by the match that first uses them; it isn't a
  blocker.
- `decomp.py next` picks the smallest function first, so it's not a fair
  sample; pick by size percentile for checks like this one.
- `func_80032F3C` is libc `abs`/`labs` (identical objects, so it's unnamed);
  `func_80025BBC` calls it three times and drops the result.
- Matched-but-mistyped functions (`void f(void)` pass-throughs written by the
  old `auto`) are the likeliest remaining type stalls: retype them when a
  caller needs it.

**Next**
1. Phase 2: keep matching `main.exe` (`/match-function`).
2. The GTE functions: check `include/gte_macros.inc` covers what the 29
   need before starting them.

## 2026-10-10 - engine types, hero struct and bool (Phase 1 done)

**State at end of session:** `make verify` -> 16 targets OK; 144 game
functions named, 280/393 matched functions typed.

**Done**
- `bash/nas.h` now defines `Hero` (0x6C; `Heroes[4]` in `engine.h`),
  `subengine` (0xA8), `EXTmotion` (0x34, id 0x800), `EXTobject` (0x2C, id
  0x1000) and `hero_seq` (one halfword). `EXTengine` gained `sub`/`subLast`
  (`+0x6C`/`+0x70`), `unk94` and `subCount` (`+0x9A`). Evidence is in
  `docs/ARCHITECTURE.md` (Engine types).
- `bool` is 32 bits: the callees of two of the leak's bool parameters reload
  them with `lw` and don't narrow them.
- `BASH_HAVE_HERO_SEQ` is defined, so the leak's `*_seq_*` externs now compile
  in. Only `BASH_HAVE_TA_PLAYER` is still guarded.
- Named `AddMotion`, `AddObject` and four Arkenoid functions from the leak's
  order after `PlotBall`/before `ArkHeroRad2ObjectRepel`:
  `ArkHeroRad2ObjectCollision`, `CalcArkInfluence`, `FreeGrabbedObject`,
  `AlignGrabBall`.
- Typed the four matched subengine-release functions (`func_8001D174`,
  `func_8001D220`, `func_8001D2B4`, `func_8001D334`) with the new structs; the
  build still matches, which checks those offsets.
- `tools/scripts/field_survey.py`: counts every load/store through a struct
  pointer (by array symbol, function return or loaded field), giving each
  offset's widths and signedness. That's where the field types come from.
- Roadmap: last Phase 1 item ticked. `NASTrigger`, `EXTbotai`, `EXTheromove`
  and `TA_player` are used only by the minigame overlays and stay incomplete
  until those are worked on.

**Learned / gotchas**
- `make verify` wipes `build/`, including the full disassembly that
  `make objdiff-config` writes to `build/objdiff/asm`. Re-run objdiff-config
  before surveying matched functions too; `asm/` alone lacks them.
- `NAS+0x58` is a per-object pointer: `EXTobject` for Arkenoid balls, other
  extension types elsewhere. Leave it `void *`.
- The leak's prototype order follows the code order inside a file
  (Arkenoid twice now), so neighbours of a named function are good candidates.

**Next**
1. Phase 2: match `main.exe` game functions, one per session
   (`/match-function`, `decomp.py next`). Use the new types (`Hero`,
   `subengine`, ...) in place of m2c offsets.
2. Name the hero's `+0x18`/`+0x1C`/`+0x20` extensions and `func_8008502C`
   (the `hero_seq` interpreter) once their leak names are proven.

## 2026-10-10 - objdiff-config fixed

**State at end of session:** `make verify` -> 16 targets OK; `make
objdiff-config` exits 0 (34 units); `make report` -> functions 393/2575.

**Done**
- `make objdiff-config` runs splat a second time per config with
  `configs/USA/objdiff.yaml` merged in (`splat split <cfg> objdiff.yaml`):
  `disassemble_all`, with asm, assets, linker script, undefined-symbol files and
  cache all under `build/objdiff/`, and `matchings_path: nonmatchings` so
  matched functions land next to the rest. `asm/`, `src/` and `linkers/` are
  untouched.
- `objdiff_config.py` now reads each unit's functions (name and address) from
  those asm files instead of from `functions.json` keys and names. That fixes
  both failures: overlay keys like `engine:80078CA4` are no longer parsed, and
  renamed functions (`SetRandomSeed`, still `func_800155E8` in
  `functions.json`) get the right file. Target `.text` sizes equal the base
  objects' sizes (checked for main, engine, ovl5d).

**Learned / gotchas**
- splat 0.50 merges extra yaml files given after the config (dicts merge,
  scalars override, lists append), so per-run overrides need no copy of the yaml.
- `functions.json` names can lag `sym.*.txt` renames; don't trust them for file
  paths.
- Target objects also carry `NAME.NON_MATCHING` markers for matched functions.
  The report's matched count is still right (393), so they don't get in the way.

**Next**
1. Unchanged from the previous entry (hero struct, remaining engine types,
   `bool`).


## 2026-10-10 - PsyQ headers in the game sources

**State at end of session:** `make verify` -> 16 targets OK.

**Done**
- `include/psyq.h` includes Sony's PsyQ 4.7 headers (libc, `libapi`, `libetc`,
  `libgte`, `libgpu`, `libcd`, `libspu`, `libsnd`, `libpad`, `libmcrd`), and
  `common.h` includes it, so every library call now has Sony's prototype. Before,
  they were implicit declarations. The game calls 125 library functions.
- Fixed the call sites the prototypes flagged, all codegen-neutral:
  `D_8005B640` is a `RECT`, `D_8005B698` a `RECT *` (two rects), `D_8006EA14`
  a `CdlLOC` (with `CdlSetloc`/`CdlSeekL`), and `D_800643F8` the saved tick
  callback `void (*)()`. Roadmap item ticked.
- m2c drafts now see the SDK too (`decomp.py m2c` context includes it).

**Learned / gotchas**
- Sony's `memory.h` declares `memcpy` K&R-style; gcc's builtin prototype still
  warns on the untyped `func_800162E4`. That clears when the function is typed.
- `make objdiff-config` is broken (separate from this change): overlay keys
  (`engine:ADDR`) break `objdiff_config.py`, and the target units need asm for
  already-matched functions, which splat doesn't write. Flagged as its own task.

**Next**
1. The hero struct (0x6C, at `Heroes`), the other engine types `main.exe` uses,
   and `bool`. That finishes Phase 1.


## 2026-10-10 - core globals

**State at end of session:** `make verify` -> 16 targets OK; 138 game functions
named, 276/393 matched functions typed.

**Done**
- Named 18 core globals, with evidence in `docs/ARCHITECTURE.md` ("Core
  globals"): the AST table and count, the NAS lists and template, the engine
  template and pool, the hero array and its tables, the random seeds, the atan
  table and the sound tables. `ASTEntry` (12 bytes) is in `bash/nas.h`.
  `ASTTable`, the NAS lists and `EnginePool` have real types, and the matched
  C that used them now uses fields (`GetEngineSlot`, `func_80095A04`, and two
  `ovl6a` functions use `eng->unk10.y`). `SetRandomSeed` named. Roadmap item
  ticked.
- Fixed a slip from the owned-headers migration: overlays included other
  overlays' headers wherever a placeholder name coincided. Each overlay now
  includes only its own header; the few same-named asm functions its C calls
  are declared there.

**Learned / gotchas**
- `decomp.py rename` prints `ERROR: unknown function OVL:ADDR` for data
  symbols given as `OVL:ADDR`. That's a lookup attempt; the rename still works.

**Next**
1. PsyQ headers in the game sources.
2. The hero struct (0x6C, at `Heroes`), the other engine types `main.exe` uses,
   and `bool`.


## 2026-10-10 - core engine function names

**State at end of session:** `make verify` -> 16 targets OK; 137 game functions named.

**Done**
- Named 15 core functions with code evidence (`docs/ARCHITECTURE.md`, "Core
  engine functions"): `PlaySfx`/`StopSfx`, `Random`, `get_angle` (the leak's
  name), `SpawnNASById`/`SpawnNASFromRecord`/`KillNAS`, the extension calls
  (`AllocExtension`, `AddExtension`, `AddEngine`, `FindExtension`,
  `FreeExtension`), `SetHeroAction`, `SetHeroSeq` and `GetEngineSlot`. All but
  `get_angle` are descriptive names. Roadmap item ticked.

**Learned / gotchas**
- The player/hero array `D_8009D52C` has 0x6C-byte entries; `hero+0x2E` is the
  character (1-8).
- `D_800A0E78` is a pool of 0x9C-byte engines.
- Most-called list: `grep -rhoE "jal +func_800[0-9A-F]{5}" asm/nonmatchings | sort | uniq -c | sort -rn`.

**Next**
1. Core globals: the AST table (`D_8005AA6C`, count `D_8005AA60`), the NAS
   lists (`D_8005AA04`/`08`/`0C`), the hero array (`D_8009D52C`), the random
   seeds and the engine pool.
2. PsyQ headers in the game sources; then the hero struct and `bool`.


## 2026-10-10 - owned headers

**State at end of session:** `make verify` -> 16 targets OK.

**Done**
- `include/externs.h` and `include/prototypes.h` are gone. Their declarations
  are now in one owned header per source file: `bash/main.h`, `bash/engine.h`,
  `bash/ovl5d.h`..`bash/ovl6a.h`, each marked `owner: <src>`. Functions go to
  the file that defines them. Data goes by address: the exe's to `main.h`, the
  engine's to `engine.h`, and slot-overlay data to each overlay that uses it.
  Names the leak-derived headers declare are not repeated. Sources include only
  the headers they use. Roadmap item ticked.
- The engine types header moved from `bash/engine.h` to `bash/nas.h` (the name
  `engine.h` is now the engine overlay's owned header).
- `decomp.py`:
  - `declare()` adds declarations to a file's owned header (it's used by
    `land()`, so by `done`'s duplicate propagation and by legacy `auto`).
  - `done` declares the matched function's prototype.
  - `rename` rewrites a slot overlay's owned headers and drops an owned
    declaration once the name is declared in an original header.
  - Propagated copies no longer include another file's owned header.
  - `write_prototypes`/`add_externs` are removed.

**Learned / gotchas**
- Renaming a function to a leak-declared name and back loses its owned
  declaration (the dedupe runs on the way in). Re-add it by hand if that
  happens.

**Next**
1. Phase 1: finish the overlay section splits, and identify the overlays.


## 2026-10-10 - functions reached from the AST pairs

**State at end of session:** `make verify` -> 16 targets OK; 122 game functions named.

**Done**
- Named 10 helpers from the leaked prototypes, each with code evidence: eight
  Ripper Roo functions in Pogo, and Arkenoid's
  `ArkHeroRad2ObjectRepel`/`Attract`. `PogoSquare` is size-checked at 0x1C.
  Evidence and the ambiguous leads are in `docs/ARCHITECTURE.md` ("Functions
  reached from the AST pairs"). Roadmap item ticked.

**Learned / gotchas**
- `func_80022A3C(x, y, z, id, ...)` spawns an object by AST id.
  `func_800264E8(nas, id)` finds an extension by `EXTid`. A constant AST id
  passed to the first is the best "this function creates X" evidence.
- Fingerprint scripts: `disc/scratch/callsites.py OVL` (what each call passes,
  and whether the result is used) and `disc/scratch/callgraph.py spawns`.

**Next**
1. Phase 1: retire `include/externs.h`/`prototypes.h`, finish the overlay
   section splits, and identify the overlays.
2. Follow the ambiguous leads once more code is matched (see ARCHITECTURE).


## 2026-10-10 - core engine types

**State at end of session:** `make verify` -> 16 targets OK.

**Done**
- `include/bash/engine.h` defines `SLONG_XYZ`, `EXTnode`, `NAS` (0x70) and
  `EXTengine` (0x9C), with `BASH_SIZE_CHECK` compile-time sizes. Unknown members
  are `unkXX` with explicit padding. `BASH_HAVE_EXTNODE` is defined, so the
  leak's `EXT*` structs in pogo.h/polar.h are live. polar.h checks
  `EXTsmoketrail`, `EXTbombmaker` and `EXTtransmuter` against their allocation
  sizes. Evidence is in `docs/ARCHITECTURE.md` (Engine types).
- The roadmap item is split: core layouts ticked, the remaining types open.

**Learned / gotchas**
- `func_8002689C(nas, size, id)` allocates an extension and
  `func_8002672C(nas, id)` an engine. Their call sites give extension sizes
  directly, which is the fastest check on a struct layout.
- Pogo doesn't call `func_8002689C` directly, so `EXTPogoShockwaveInfo` is
  unchecked.
- Survey script: `disc/scratch/nas_fields.py ARGREG FUNC...` lists field
  offsets and widths accessed through a pointer argument.

**Next**
1. Name the functions the AST pairs reach (phase 1).
2. Remaining engine types: start with `EXTengine+0x6C`'s target (`EXTobject`?).


## 2026-10-10 - leaked headers into include/bash/

**State at end of session:** `make verify` -> 16 targets OK.

**Done**
- `include/bash/{tank,pogo,arkenoid,polar}.h`: the leak's enums, structs,
  prototypes and externs, rewritten in repo types and style. `bash/engine.h`
  holds the engine types the leak uses but never defines, as incomplete structs.
  Each of `ovl5f`/`ovl60`/`ovl61`/`ovl62` includes its header. Details are in
  `docs/ARCHITECTURE.md` (Leaked source). Roadmap item ticked.
- The already matched Init/Plot functions now take `NAS *`, as the headers
  declare. Their code is unchanged.
- `PlotAR_PU` renamed to `Plot_Ark_Pickup` (`e6cdf38`).

**Learned / gotchas**
- The headers were generated by a local script (`disc/scratch/leak_to_headers.py`,
  not committed: it reads the leak). Edit the headers directly from now on.
- `make verify` prints splat "Unable to determine a segment" errors for
  `funcEvSpError`/`funcEvSpErrorx`. They're harmless (the build matches),
  but they're noise worth fixing in the symbol files.
- `bool` is a guess (`s32`). The first matched function that takes or returns
  one settles it.

**Next**
1. Engine core types: recover `EXTnode` and `NAS` from field accesses, then
   define them in `bash/engine.h` and drop the matching `BASH_HAVE_*` guards.
2. Name the functions the AST pairs reach, using the prototypes now in
   `include/bash/`.


## 2026-10-10 - AST Init/Plot names

**State at end of session:** full build matches; 112 game functions named.

**Done**
- Named the AST Init/Plot pairs for Pogo (`ovl60`), Arkenoid (`ovl61`), Polar
  (`ovl62`) and the engine-side objects they use: 112 functions (renames in
  this commit; method and evidence in `docs/ARCHITECTURE.md`, Overlays). The
  roadmap item is ticked. Tank is split out as its own unticked item.
- Fixed `decomp.py rename` for slot overlays: since the restructure it looked
  for sources under `src/<ovl>/`, not `src/overlays/<ovl>/`.
- The pre-commit hook counts `INCLUDE_ASM` lines, so renames pass.

**Learned / gotchas**
- AST entry: `{id, Init, Plot}`, with `0x80026D00` as the empty stub.
  Pointers below `0x800B32B4` are engine functions.
- `decomp.py done`'s scope check compares names: commit renames *before*
  matching, or it reports the renamed functions as extra matches.

**Next**
1. Turn the leaked headers into `include/bash/` (roadmap phase 1).
2. Tank: look for another way to get its ids (strings, the overlay's own
   tables) before naming anything.


## 2026-10-10 - auto-matching pass, compiler settled

**State at end of session:** `tools/dk make verify` -> 16 targets OK. Game code
1.89% (19,248 / 1,019,036 bytes, 391 / 2,550 functions); was 97 functions.

**Done**
- Compiler tie settled: **2.8.1-psx -O2 -G0** (kept in `configs/USA/project.mk`).
  Same pool, `auto --max-size 128` (200 functions, the default `--limit`),
  no commit, reverted between runs: 2.7.2-cdk matched 71, 2.8.1-psx matched 128.
  All 71 from 2.7.2-cdk are among the 2.8.1 ones (57 extra, none the other way).
- Auto run over every function (`--max-size 1000000 --limit 100000`), committed in
  `346b9dc` (its title says 77, but it also holds the 128 from the 2.8.1 tie run)
  and `693366a` (89). Remaining failures in `.decomp/state.json`: 814 types,
  716 compile, 628 diff, 16 m2c, 7 in-file, 1 full-build.
- `decomp.py` `land()`: when the full build fails, it now bisects the batch and
  leaves out only the functions that break it (tagged `full-build`), instead of
  reverting everything.

**Learned / gotchas**
- `auto` defaults to `--limit 200`: pass `--limit 100000` for a full sweep.
- A function can match on its own and still break the binary: `func_800C8BA0`
  (ovl5f) adds 4 bytes of data/rodata (ovl5f offset 0x4C). It needs its literal
  or constant placed as in the original (likely rodata belongs to another file).

- Then switched to **hand matching only** (`010aed4`): `decomp.py done NAME`
  is a validation gate (scope: one function; lint: no m2c leftovers, hacks need
  `--hack`; function; full build), the pre-commit hook refuses matches made
  outside it, and `auto` is retired (`--legacy`). See `docs/MATCHING.md` and
  the `/match-function` skill. First match through it: `func_800C6828` (ovl5f,
  `d3386d9`; m2c dropped the pass-through `a0`).
- README: states the goal (a source-only Crash Bash: all C, no INCLUDE_ASM,
  byte-identical; PsyQ from Sony objects) and has a Mermaid diagram of the
  matching loop and gate ("How functions are matched").

- Added `docs/ROADMAP.md`: six phases (foundations: types and names; main.exe;
  engine; leaked minigames; other overlays; finish) with exit criteria.

**Next** (phase 1 of `docs/ROADMAP.md`)
1. `/match-function`, one function per commit. `next` still shows the old
   auto drafts for the 628 near-misses, which are good starting points.
2. Many failures need types: turn the leaked Pogo/Arkenoid/Polar headers
   (`disc/leaked_source.txt`) into `include/bash/` headers.
3. Fix `func_800C8BA0` by hand (see above).

## 2026-10-10 - overlays and leaked-source scan

**State at end of session:** `tools/dk make verify` -> 16 targets OK
(`build/main.exe`, `build/engine.bin`, `build/ovl5d.bin`..`build/ovl6a.bin`).
Game code is now 1,019,036 bytes / 2,550 functions (was ~125 KB / 574): the exe
was about an eighth of the game.

**Done**
- Found the `CRASHBSH.DAT` index in the exe: `{u32 sector, u32 size}` x 993 at
  `0x8004E110`. Code scan of every entry and of the whole DAT/`BASHY.`: 15 code
  files, nothing else.
- Registered them (`add-overlay`, yamls in `configs/USA/overlays/`, sources in
  `src/overlays/<ovl>/`): `engine` (entry 0x3DF, loads at `0x80078C90`) and 14
  slot overlays at `0x800B32B4`, named by their id word (`ovl5d`..`ovl6a`).
  Facts and evidence are in `docs/ARCHITECTURE.md` (Overlays) and the yaml
  comments.
- Engine split into header (textbin), `.text`, `.data`, `.rodata` (jump tables
  only), `sdata` (second writable block), plus a separate `engine_slot` segment
  for the slot image it reserves (stub id 0x5C + the leaked text).
- AST table (`0x80098720`) maps id groups to overlays (Pogo `ovl60`, Arkenoid
  `ovl61`, Polar `ovl62`, probably Tank `ovl5f`); its 10 pointers that fell
  inside ovl62 functions are now function symbols in `sym.ovl62.txt`.
- Leak scan: one block only (40 KB headers, DAT `0x45C9800`); no other paths or
  `__FILE__` strings anywhere. Local copy `disc/leaked_source.txt`.

**Learned / gotchas**
- splat only writes `.s` files for functions an existing C file `INCLUDE_ASM`s,
  and never rewrites `src/`. After adding function symbols to an overlay that
  has no C yet, delete its scaffold `.c` and re-split. And `make check` won't
  notice: `.c.o` doesn't depend on the included `.s` files, so a stale object
  still matches. Use `make verify` (clean) after any re-split.
- Packed segments: `align: 4` is needed on *every* segment in a file, since
  splat aligns each segment's end (`__romPos = ALIGN(..., 16)` by default).
- `make clean` wipes `build/`: keep scratch scripts in `disc/scratch/`.

**Next**
1. Slot overlays: split `.data` vs trailing rodata the same way (store
   evidence), and look for more pointer-only leaf functions (data tables of
   function pointers, as the AST table did for ovl62).
2. Turn the leaked headers into `include/` headers for Pogo/Arkenoid/Polar
   (structs, prototypes in id order), name the AST Init/Plot pairs.
3. Start matching: `decomp.py auto --max-size 128 --commit` (now across 2,550
   functions), then break the compiler tie (2.7.2-cdk vs 2.8.1) with
   `auto --max-size 48` under each.

## 2026-10-10 - project setup and restructure

**State at end of session:** `tools/dk make verify` -> `build/main.exe: OK`.
All game functions are still `INCLUDE_ASM`, except 17 empty functions that splat
writes as C itself. PsyQ is 99.3% covered by Sony's linked objects. No overlays
yet.

**Done**
- Scaffolded the project from the `psx-matching-decomp` skill. Disc copied from
  the user's `disk/` (kept, gitignored) to `disc/CrashBash.{bin,cue}`.
- Split `SCUS_945.70` with splat: 1140 functions, 574 of them game code.
- Compiler: probe tied four ways on 10 leaf functions (2.7.2-cdk / 2.8.1-psx,
  -O1/-O2). Chose **2.8.1-psx -O2 -G0** because an earlier Crash Bash attempt
  recorded 2.8.1 matching 14/16 vs 2.7.2-cdk 5/16. Not yet confirmed in
  this repo: re-test with `auto --max-size 48` under each compiler once a few
  dozen functions match. maspsx `--aspsx-version=2.86` (PsyQ 4.7).
- PsyQ 4.7 identified (`$Id: sys.c` 1998/01/12), library start `0x8002E7A8`,
  432 functions named, one file per object, then 254 Sony objects linked in
  place (`configs/USA/psyq/`).
- `.rodata` split from `.data` (game rodata, library rodata, data); the
  boundaries and evidence are in `docs/ARCHITECTURE.md` and the yaml.
- objdiff support (`make objdiff-config`, `make report`); objdiff v3.8.2 CLI
  and GUI downloaded to `ext/objdiff/`.
- Restructured to mirror khasinski/parasite-eve-decomp (README sections,
  `configs/USA/`, `linkers/`, `src/main/...`, `tools/scripts/`, docs, badges),
  with the local dashboard in place of decomp.dev.

**Learned / gotchas**
- `decomp.py index` kept stale `src`/`seg` paths after files moved; fixed so a
  missing `src` file is re-resolved. If files are moved again, check
  `make report` and `decomp.py progress` agree with the figures before the move.
- Git Bash heredocs passed through the Bash tool collapse `\\` to `\`: write
  Python helpers with the Write tool, not heredocs, when they contain escapes.
- Host has no Python; run every script through `tools/dk`.
- `tools/dk` in shell loops needs `</dev/null` (it runs `docker -i`).
- Paths differ from the skill's docs: the skill says `tools/decomp.py`,
  `config/`, `config/symbol_addrs.txt`; here they are `tools/scripts/decomp.py`,
  `configs/USA/`, `configs/USA/sym.main.txt`, exe target `build/main.exe`,
  linker scripts in `linkers/`.
- README badges point at `raw.githubusercontent.com/jake-r-r-w/crash_bash_decomp`;
  no git remote exists yet. Fix the URL if the repo is published elsewhere.

**Next**
1. Overlays (skill setup step 3d): find code in `CRASHBSH/CRASHBSH.DAT`
   (engine module + minigames), its index table in the exe, load addresses;
   `decomp.py add-overlay`, one yaml per overlay in `configs/USA/overlays/`,
   then split rodata/data per overlay. Most of the game's code is there.
2. Look for leaked source text in the disc files (C keywords, `typedef`,
   `# 12 "c:\...`); an earlier attempt found ~40 KB of minigame headers inside
   the engine overlay.
3. Start matching: `decomp.py auto --max-size 128 --commit`, then one function
   per session with `next` / `make diff` / `done --commit`.
