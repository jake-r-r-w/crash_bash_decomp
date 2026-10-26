# Roadmap

The goal is a **source-only Crash Bash**: the executable and every overlay
rebuilt byte for byte from C, data included, with no `INCLUDE_ASM` or
`INCLUDE_RODATA` left, and everything named, typed and split into its
original files. The game's assets (textures, sounds, models, levels) are not
part of the source: a tool extracts them from the user's own disc into
editable formats, and their formats are documented. Assets stay on the
user's machine and never go in git; only the tools and format documentation
do. [The final test](#the-final-test) says when it's done: a rebuilt disc,
and a shifted build that plays the same as retail. Progress lives on the
dashboard and badges. Tick a box in the commit that finishes that item, and
run `tools/dk make progress` in it so the dashboard and roadmap badge
update.

Preparation goes in the phase that first needs it. When later work turns up
something new to do, add it to the phase it blocks, not to an earlier one.

## Current focus: every slot overlay the same way

Every game function matches (2026-10-26), so what is left is what makes the
source source-only: data as C, names and types, and the proof that the code
works as code, a shifted build that still plays. Phase 3 did all of it for
one overlay, Tank (`ovl5f`), end to end, and built the tooling on the way
(finished 2026-10-26). Phase 4 now repeats it overlay by overlay with the
recipe in [OVERLAY_RECIPE.md](OVERLAY_RECIPE.md), the other leaked
minigames first, and Phase 5 does the same for the engine and `main.exe`.

Any build of ours can be played: `tools/dk make disc` packs `build/main.exe`
and the overlays into `build/CrashBash.bin`/`.cue`, which boots in an
emulator, and `make disc-check` confirms the matching build packs to the
retail image byte for byte (docs/PLAYING.md).

Matches are C only (no inline asm; the SDK's `gte_*` macros excepted), and
natural C comes first: C that only steers the compiler is used when nothing
natural matches (docs/CONVENTIONS.md, Matching rules). A rework of a matched
function (a rename, a retype, a natural form) is an ordinary commit once
`make check` is OK.

## Phase 1 - Foundations: types and names

Build the names and types that matching `main.exe` depends on.

**Done when** matching `main.exe` functions no longer stalls on missing names
or types.

- [x] Turn the leaked minigame headers (Tank, Pogo, Arkenoid, Polar) into
  `include/bash/` headers: structs, enums and prototypes. Commit only the names
  and types, never the leaked text.
- [x] Name the AST Init/Plot pairs that the leaked enums prove (Pogo,
  Arkenoid, Polar).
- [x] Name the functions the named AST pairs reach, using the prototypes in
  `include/bash/`.
- [x] Recover the core engine layouts: `NAS`, `EXTnode`, `EXTengine` and
  `SLONG_XYZ` (sizes and proven fields).
- [x] Retire `include/externs.h` and `include/prototypes.h` into owned headers.
- [x] Name the core engine functions every match calls (spawn by AST id,
  extension and engine allocation, extension lookup, random, sound, angle, and
  the most-called exe and engine functions), each with code evidence.
- [x] Name the core globals: the AST table, the NAS lists and the player/hero
  array.
- [x] Include Sony's PsyQ headers in the game sources, so library calls get
  real prototypes.
- [x] Recover the layouts of the engine types `main.exe`'s code uses (from
  `EXTobject`, `EXTmotion`, `NASTrigger`, `subengine`, `hero_seq`, ...) and the
  player/hero struct indexed from `D_8009D52C`, and settle the size of `bool`.
  Naming `unkXX` fields is ongoing work in every phase, not part of this item.
  `NASTrigger` and the other minigame-only types (`EXTbotai`, `EXTheromove`,
  `TA_player`) are only used in the overlays, so they're recovered with them.

## Phase 2 - Matching

Every game function becomes C that compiles to the retail bytes, with the
tools that needs: the disc packer, the asset extractor and the library.

**Done when** every game function in `main.exe`, the engine and the slot
overlays matches in C only (no inline asm but the SDK's `gte_*` macros and
`draw_verts`'s original register pins), the
library is Sony's objects, `CRASHBSH.DAT` extracts, and the disc packer
rebuilds the retail image.

- [x] A disc packer: put the built exe and overlays, with the user's other
  files, back into a disc image with the retail layout (every file at its
  original sector). Packing the matching build must give the retail image
  byte for byte. It's the only way to run any build of ours in an emulator.
  `tools/dk make disc` (`tools/scripts/pack_disc.py`) and `make disc-check`
  (`configs/USA/disc.sha`).
- [x] Match GTE code: include Sony's GTE macros (`inline_c.h`, `gtemac.h`) in
  the game sources and match one GTE function through `done`. 27 of the exe's
  game functions use the GTE, and m2c can't draft them.
- [x] Split `main.c` into translation units at the boundaries the binary shows
  (rodata and data order, alignment padding, file-local data), with the
  evidence for each in `docs/ARCHITECTURE.md`. Done 2026-10-23: `main/main`
  is nine files, cut where the evidence settles it (`decomp.py units`, the
  data blocks, what the functions on either side do; `split_unit.py` does a
  split end to end). No leak names the exe's files and the code evidence is
  used up (ARCHITECTURE.md, Files inside `main/main`).
- [x] Close the PsyQ gaps: every library function is linked from Sony's 4.7
  objects (281, plus the hand-written `2MBYTE.OBJ`), with their data. The one
  exception is `bios_1.o`'s `.data`, kept from the exe because the game's
  libcd differs from 4.7 in one initial value
  (`configs/USA/psyq/data_keep.txt`, ARCHITECTURE.md, PsyQ).
- [x] Asset extraction tool: `tools/scripts/dat.py` splits `CRASHBSH.DAT`
  into `assets/CRASHBSH/` and `make disc` builds the DAT from there
  (2026-10-16). Every entry but the code overlays is a decoded folder:
  texture sets to PNG (`tex.py`), models to OBJ and glTF with their
  animations (`mdl.py`, `gltf.py`), title screens (`tga.py`), text
  (`txt.py`), sounds to WAV, VAB, SEQ and MIDI (`sfx.py`), each documented in
  docs/ASSETS.md, which lists what is still bytes.
- [x] Recover the minigame-only types (`EXTbotai`, `EXTheromove`, `TA_player`,
  `EXTobject_TA`, `NASTrigger`, ...) from the Oct 9 prototype's debug types
  (ARCHITECTURE.md, Prototype debug types), and identify which minigame or
  mode each slot overlay is (its AST enums; `ovl69`'s strings make it the
  front end).
- [x] Name Tank's AST pairs: the Oct 9 prototype's `TANK_AST_types` gives
  every id, and all 68 names match the leak's `Init`/`Plot` prototypes.
- [x] Match every game function in the executable (2026-10-23).
- [x] Match every function in `engine` (2026-10-23; the pause menu,
  `func_8007F314`, last, from its earlier compiles on the prototype discs:
  PROTOTYPES.md, Earlier compiles of a function).
- [x] Match every function in the slot overlays (2026-10-26; the last three
  with cc1 analysis variants, `tools/cc1/`).
- [x] Only C everywhere, with one recorded exception: `draw_verts`
  (`func_800193A8`, `src/main/80017164.c`) keeps its three register pins
  (`register s32 xy asm("$20")`, ...). They are the original's register
  variables: no pin-free C can give the target's `$18`-`$20` with `$16`/`$17`
  unused (2026-10-26; CONVENTIONS.md, Matching rules).

## Phase 3 - The Tank pilot

Take one slot overlay, Tank (`ovl5f`), all the way to source-only and
shift-tested, and build the tools for it so the other overlays only repeat
the recipe. A slot overlay is the first unit that can move: the only
pointers into one from outside are the engine's AST table entries
(`Init`/`Plot`), so once those are symbols the engine relinks against the
moved overlay without moving itself. `main.exe` and the engine can't move
until every overlay linking to them is source (Phase 6).

**Done when** `ovl5f` has no `INCLUDE_RODATA` and no data asm, every
function and global in it is named and typed, every declaration in
`include/bash/tank.h` is defined by matched code or noted in
`docs/ARCHITECTURE.md` as not in this build, `build/ovl5f.bin` still
matches, its asset files extract, and it passes its shift test: padded so
everything in it moves, it boots and plays through to a win the same way
retail does. The recipe is written up for Phase 4.

- [x] Name and type every Tank function and global (a descriptive name
  where the original is unknown, per [CONVENTIONS.md](CONVENTIONS.md)),
  from the leaked `tank.h`, the Oct 9 debug types and code evidence. Settle
  each `tank.h` declaration: defined by matched code, or noted as not in
  this build. First, because the data's types come from the code that reads
  it: naming a function shows what each table it reads is, and names those
  globals too. Done 2026-10-26: every function and global (the seven bss
  words nothing reads on any disc are `TankUnused*`). The leak's
  data is defined: `TANK_PREMAP` (with its `WWInf`/`TrigLookUp` pieces),
  `tank_shell_info`, the five `file_group_TANK*` and the 72
  `*_seq_tank*` hero sequences; `shell_setup` and `file_group_load_info`
  are complete types now. `TANKSpecifics` is not in Tank's data.
- [x] Split Tank into its files where the evidence settles it, after the
  naming and before its data goes to C. Done 2026-10-26: Tank is one file.
  `decomp.py units --data` now reads the slot overlays (its initialised data
  ties functions 1..123), and `cc1plus`'s emission order settles the rest:
  the out-of-line inline functions close a file in reverse definition
  order, and against the leak's order Tank has one such tail, 87..180
  (ARCHITECTURE.md, Files inside the slot overlays).
- [x] Data to C tooling, once the names and types say what the tables are:
  turn a splat rodata or data block into C
  initialisers (strings, tables, structs from the headers, pointers as
  symbols, never as numbers) that rebuild byte for byte, in the unit's
  section order (`ovl5f`'s is rodata, text, data). Generic, so every
  overlay can use it. Done 2026-10-26: `tools/scripts/data2c.py`
  (CONVENTIONS.md, Data as C).
- [x] Tank's rodata as C: the id word (`ovl5f_id`) and the `INCLUDE_RODATA`
  of `D_800B32B8` (the animation name strings). Done 2026-10-26: the
  strings are `const char` arrays in `ovl5f.c`, and the id word is
  `src/overlays/ovl5f/ovl5f_id.c` (`TankOverlayId`, a rodata-only unit:
  splat takes a `.rodata` subsegment with no `c` one). No asm is left in
  Tank.
- [x] Tank's data and bss as C (`asm/data/overlays/ovl5f/`): every pointer
  a symbol, every table typed with the struct the naming found. Done
  2026-10-26: all 9.9 KB are C in `ovl5f.c`, the structured tables retyped
  with the naming (`PreMap`, `WWInf`, `hero_seq`, `file_group_load_info`,
  `shell_setup`, `u16` AI tables); lists of ids and frames stay `s32[]`.
- [x] Tank's asset files extract: list the `CRASHBSH.DAT` entries its
  `file_group_TANK*` groups load and check each decodes (docs/ASSETS.md); a
  format still kept as bytes gets decoded here. Done 2026-10-26: every DAT
  entry but the 15 code overlays decodes (400 texture sets, 400 models,
  160 sound banks, 12 texts, 5 screens), so all Tank loads does; its 66
  named entries sit in 16 of the exe's file groups (the mode texts, the
  arena sounds and music, the arena and object models, the eight
  characters' tanks).
- [x] The engine's AST table (`EngineASTTable`, `0x80098720`) as C with
  symbols: its slot entries were numbers (`.word 0x800B...`), so a moved
  overlay would still be called at its old addresses. Done 2026-10-26:
  `src/overlays/engine/asttable.c`, all 174 entries by name with the AST
  enums' ids; the engine's link takes the slot functions' addresses from
  the overlays' own links (`tools/scripts/slot_syms.py`, a Makefile step).
- [x] A shiftable `NON_MATCHING` build: an option (say `make shift OVL=ovl5f
  PAD=N`) that pads a source-only slot overlay's code and data so every
  address in it moves, relinks the engine's AST table against it and packs a
  disc. The packer rebuilds `CRASHBSH.DAT` and its index when an entry grows
  (today a file may shrink but not grow), and the padded overlay must stay
  inside the slot (`0x800B32B4` up to `0x800D7144`, the slot image's end;
  Tank ends at `0x800CB294`). Done 2026-10-26: `make shift OVL=ovl5f
  PAD=0x40` (`tools/scripts/shift.py`, docs/PLAYING.md). It relinks from the
  matching objects with padding in the linker script, so the C needs no
  `NON_MATCHING` code. Until the engine's data is C (Phase 5), it rewrites
  the engine data words that point at Tank (19 of them) by name.
- [x] The engine's slot image in the `NON_MATCHING` build: the id word (`0x5C`)
  and zeros in place of the 147 KB the matching build reads from the disc
  (ARCHITECTURE.md, The engine's slot image). The shift test runs with it.
- [x] Tank's shift test: the padded build boots and Tank plays through to a
  win, the same way as on the retail disc. Write the route down in
  docs/PLAYING.md, so the test can be repeated and later automated.
  Passed 2026-10-26: `make shift OVL=ovl5f PAD=0x40`, Desert Fox played
  through to a win, everything as on retail (route: PLAYING.md, The shift
  test route). It relies on `shift.py`'s stopgap for the engine's asm data
  (the per-character hero sequence tables, the hero mode table, the
  per-world record), until Phase 5 makes that data C.
- [x] Settle `tank.h`: every declaration is defined by a built unit but
  `TANKSpecifics`, which is not in this build (`TA_SetUp` fills the exe's
  `LevSpec` at `0x8005AA70` field by field instead; ARCHITECTURE.md, Tank's
  arenas and data). Done 2026-10-26; a stale alias of ours (`D_800CAD50`, at
  `TA_players`) is gone.
- [x] Words nothing reads, checked on the other discs: Tank's seven
  never-read bss words were mapped into each prototype and demo disc's Tank
  through `TA_InitLevel`, and no build reads them, so they are
  `TankUnused*` (2026-10-26).
- [x] The dashboard measures what the pilot works on: its map has four
  tabs (Game code, Functions named, Data in C, Data named), a C file's data
  is cut at its objects, a switch's jump table shows as its function's
  (`PlotTotemBit switch table`), and the engine's slot image is counted
  apart from the data. Done 2026-10-26; Tank shows all done in every tab.
- [x] What the shift test needed: a completed save card in place of the
  cheat word, and a way to trace a crash when the emulator goes down
  (RetroArch's network commands, the BIOS's saved registers; PLAYING.md).
  Done 2026-10-26.
- [x] Write the recipe up (data to C, naming, the AST table, the shift
  test) with what the pilot learned, so each Phase 4 overlay follows it.
  Done 2026-10-26: [OVERLAY_RECIPE.md](OVERLAY_RECIPE.md).

## Phase 4 - Every slot overlay the same way

One at a time, with Phase 3's recipe ([OVERLAY_RECIPE.md](OVERLAY_RECIPE.md)): the other leaked minigames first,
since their headers name most of them, then the rest, starting with code
shared or near-identical between minigames.

**Done when** every slot overlay is source-only, named and typed, still
matches, its asset files extract and it passes its shift test, on Phase 3's
terms. A box below is ticked on the same terms. Where a leaked header covers
an overlay (Pogo, Arkenoid, Polar), every declaration in it is either
defined by matched code or noted in `docs/ARCHITECTURE.md` as not in this
build.

- [x] Finish the slot overlays' file splits where the binary proves a start
  (ARCHITECTURE.md, Files inside the slot overlays), each before that
  overlay's data goes to C, since a file's local data moves with it. Done
  2026-10-26: every start the binary proves was split already (`ovl5d`,
  `ovl5e`, `ovl66`, `ovl67`, `ovl6a`, by jump-table parity); the leaked
  headers' order (`tools/scripts/leak_order.py`) and `units --data` prove no
  other. Pogo and Arkenoid read as one file each like Tank. Polar's
  functions 155..216 can't be placed until they are named (Polar's box).
- [ ] Pogo (`ovl60`)
- [ ] Arkenoid (`ovl61`)
- [ ] Polar (`ovl62`): name functions 155..216 first, then run
  `leak_order.py` again to place them (one file or a second).
- [ ] Bash (`ovl5d`, with the crates and pickups of `XTRAS_AST_types`)
- [ ] Papu (`ovl5e`)
- [ ] Race (`ovl63`)
- [ ] Spin (`ovl64`)
- [ ] Dino (`ovl65`)
- [ ] Bonk (`ovl66`)
- [ ] N2O and NOX (`ovl67`)
- [ ] Chase (`ovl68`)
- [ ] Hub (`ovl6a`)
- [ ] Titles, the front end (`ovl69`): named, typed, rodata and data as C
  and its shift build made (2026-10-26); left: the shift test.

## Phase 5 - The engine and `main.exe` to source-only

The same recipe for the two units that stay in place: the engine overlay
(its types are the vocabulary the minigames are written in) and the
executable.

**Done when** `src/overlays/engine/` and `src/main/` have no
`INCLUDE_RODATA` or data asm, both are split into their translation units,
every function and global in them is named and typed, `build/engine.bin`
and `build/main.exe` still match, and the formats the engine and the exe
load extract and are documented.

- [ ] Split the engine into its original translation units. Status: split
  at every start the binary proves (switch-table parity; ARCHITECTURE.md,
  Files inside the engine), and Jampack's records name two of the files
  (`panel` is `PANEL.CPP`, `pause` `PAUSE.CPP`); more files may hide inside
  those units.
- [ ] The engine's data and rodata as C (its AST table is done in Phase 3).
  With it, `shift.py`'s stopgap goes: the engine's per-game table, the hero
  mode tables and the level tables point into the slot overlays, and as C
  they take their addresses from the overlays' links like the AST table.
- [ ] Name and type every engine function and global.
- [ ] Extract and document the asset formats the engine loads (models,
  animation, level data, ...) that Phase 2's extractor still keeps as bytes.
- [ ] `main.exe`'s rodata, data and bss as C, each file's local data with
  its unit.
- [ ] Name and type every function and global in the exe. Status: the
  names, parameter types and struct names the demo discs leaked are applied
  ([PROTOTYPES.md](PROTOTYPES.md)); the rest is named from behaviour. For
  browsing cross-references across the whole binary, try Ghidra with a PSX
  loader by hand.

## Phase 6 - Finish

**Done when** every test in [The final test](#the-final-test) passes.

- [ ] No `INCLUDE_ASM` or `INCLUDE_RODATA` anywhere (no function has been
  asm or parked since 2026-10-26; the rodata is left).
- [ ] Every remaining disc file (music, movies, anything not covered above)
  extracts, and the formats are documented.
- [ ] Shift everything at once: pad `main.exe` and the engine too, and relink
  every overlay against their new addresses instead of the fixed ones in
  `configs/USA/sym.main.txt` and `sym.engine.txt`. This needs every overlay's
  data to be C, since a pointer left in a data blob keeps its old address.
- [ ] A build with a current GCC for MIPS (`NON_MATCHING`).
- [ ] Every file at quality level 4 ([CONVENTIONS.md](CONVENTIONS.md)), with no
  placeholder names.
- [ ] `docs/ARCHITECTURE.md` describes the game's systems.

## The final test

The decomp is done when all of these pass. Each proves something a byte match
alone does not. The tools come earlier: the disc packer in Phase 2, the shifted build in Phase 3 (the Tank
pilot), and every slot overlay passes a shift test of its own when it's finished. So these tests are the last check on the whole
game, not the first time anything has been tested.

1. **Clean rebuild.** A fresh clone, the user's disc and `tools/dk make check`
   (nothing else) rebuild `main.exe` and every overlay, and each matches
   `configs/USA/checksum.sha`. The disc packer then rebuilds the disc image,
   and it matches the retail image byte for byte. *Proves: nothing outside the
   repo is needed except the user's disc.*
2. **Nothing hidden.** No `INCLUDE_ASM`, `INCLUDE_RODATA`, `.incbin` of code or
   data, parked function or `func_`/`D_` name anywhere; the only binary inputs
   are the assets the extractor reads from the disc, and the engine's slot
   image, which the matching build reads from the disc too (it is the build
   machine's memory, not source: ARCHITECTURE.md). *Proves: everything is
   source.*
3. **Shifted build plays.** The fully shifted `NON_MATCHING` build boots in an
   emulator and plays: the front end, a save and a load, the hub, and every
   minigame through to a win. *Proves: no address is hard-coded; the code works
   as code, not as bytes that happen to land in the right place.*
4. **Same game.** Played side by side with the retail disc over the same
   route (front end, a save and a load, the hub, each minigame), the shifted
   build behaves the same. *Proves: the rebuilt game behaves like the
   original, not just roughly like it.* Keep the retail disc layout: load
   times depend on where files sit on the CD.
5. **A modern compiler.** The source builds with a current GCC for MIPS
   (warnings allowed, `NON_MATCHING`) and passes test 4. *Proves: the C is
   correct C, not C that only works because of the old compiler's quirks;
   this is also the first step of the PC port.*

## Afterwards - decided once the source is complete

These are not phases yet. Each gets its own plan when the final test passes.

- **Native PC port.** Build the C for PC, with assets loaded from the extracted
  files instead of the CD. That means replacing the PS1 hardware layers (CD,
  GPU and GTE, SPU sound, pads) with PC equivalents.
- **The library as C.** Sony's PsyQ code is linked from Sony's own objects.
  Rewriting it as C that compiles to the same objects would make the whole
  game source; the PC port replaces most of it anyway, so decide after that.
