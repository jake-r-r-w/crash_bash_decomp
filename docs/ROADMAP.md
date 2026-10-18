# Roadmap

The goal is a **source-only Crash Bash**: the executable and every overlay
rebuilt byte for byte from C, data included, with no `INCLUDE_ASM` left, and
everything named, typed and split into its original files. The game's assets
(textures, sounds, models, levels) are not part of the source: a tool extracts
them from the user's own disc into editable formats, and their formats are
documented. Assets stay on the user's machine and never go in git; only the
tools and format documentation do. [The final test](#the-final-test) says
when it's done: a rebuilt disc, and a shifted build that plays the same as
retail. Phases are ordered but overlap;
matching continues throughout. Progress lives on the dashboard and
badges. Tick a box in the commit that finishes that item, and run
`tools/dk make progress` in it so the dashboard and roadmap badge update.

Preparation goes in the phase that first needs it. When later work turns up
something new to do, add it to the phase it blocks, not to an earlier one.

## Current focus: a build we can play and test

Matching continues, but the first priority is being able to run any build of
ours. `tools/dk make disc` packs `build/main.exe` and the overlays into
`build/CrashBash.bin`/`.cue`, which boots in an emulator, and `make
disc-check` confirms the matching build packs to the retail image byte for
byte, so it plays like retail by definition. Builds are tested by playing
them. The next step is the shiftable `NON_MATCHING` build (Phase 4), the
first build that isn't byte-identical.

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

## Phase 2 - `main.exe` to source-only

**Done when** the exe's game code has no `INCLUDE_ASM` or `INCLUDE_RODATA`
left, it's split into translation units at the boundaries the binary shows,
its data is C, every function and global is named and typed, and
`build/main.exe` still matches. The library is Sony's objects, code and data,
plus `2MBYTE.OBJ`. `CRASHBSH.DAT` extracts, and so do the formats whose
loaders are in the exe, each documented. The disc packer works, so later
phases can play what they finish.

- [x] A disc packer: put the built exe and overlays, with the user's other
  files, back into a disc image with the retail layout (every file at its
  original sector). Packing the matching build must give the retail image
  byte for byte. It's the only way to run any build of ours in an emulator.
  `tools/dk make disc` (`tools/scripts/pack_disc.py`) and `make disc-check`
  (`configs/USA/disc.sha`). A file may shrink but not grow; an overlay that
  outgrows its `CRASHBSH.DAT` slot needs the DAT and its index rebuilt.
- [x] Match GTE code: include Sony's GTE macros (`inline_c.h`, `gtemac.h`) in
  the game sources and match one GTE function through `done`. 27 of the exe's
  game functions use the GTE, and m2c can't draft them.
- [ ] Split `main.c` into translation units at the boundaries the binary shows
  (rodata and data order, alignment padding, file-local data), with the
  evidence for each in `docs/ARCHITECTURE.md`. The exe has no file names, so
  the units are named by what they contain. Do this before the data, so each
  file's local data moves with it, and early in the matching: code in
  the wrong unit can't match its rodata, and a split moves functions out
  from under swarm patches, so do it between swarm runs.
  Status: `decomp.py units` collects the evidence; switch-table parity
  proves one boundary in `main/main`, but not yet where (ARCHITECTURE.md,
  Files inside `main/main`), so `main.c` is still one file.
- [x] Match every game function in the executable.
- [ ] Name and type every function and global in the exe (a descriptive name
  where the original is unknown, per [CONVENTIONS.md](CONVENTIONS.md)).
  For browsing cross-references across the whole binary, try Ghidra with a
  PSX loader by hand first; an agent bridge such as REA
  (github.com/morluto/rea) adds little over that and doesn't help matching.
- [ ] Turn its rodata, data and bss into C.
- [x] Close the PsyQ gaps: every library function is linked from Sony's 4.7
  objects (281, plus the hand-written `2MBYTE.OBJ`), with their data. The one
  exception is `bios_1.o`'s `.data`, kept from the exe because the game's
  libcd differs from 4.7 in one initial value
  (`configs/USA/psyq/data_keep.txt`, ARCHITECTURE.md, PsyQ).
- [x] Asset extraction tool: read the `CRASHBSH.DAT` index (992 entries at
  `0x8004E110`) from the user's disc and extract its files to `assets/`, plus
  the formats whose loaders are in the exe, to editable files. The first half
  is done: `tools/scripts/dat.py` splits the DAT into `assets/CRASHBSH/` and
  `make disc` builds the DAT from there (2026-10-16). Formats decoded (docs/ASSETS.md): texture
  sets, 400 of them, to PNGs and back byte for byte (`tools/scripts/tex.py`); all 400 model
  files to OBJ and back, with textures, and to glTF with their animations (`mdl.py`, `gltf.py`);
  the 5 title screens to PNG (`tga.py`), the 12 modes' text to text files (`txt.py`), and the
  160 sound packs' samples to WAV, banks as VAB, music as SEQ and MIDI (`sfx.py`). Every DAT
  entry but the code overlays is a decoded folder (2026-10-16); ASSETS.md lists what is still
  bytes. Document each
  format. Formats parsed by the engine or a minigame belong to that phase.

## Phase 3 - The engine overlay

Its types are the vocabulary the minigames are written in.

**Done when** `src/overlays/engine/` has no `INCLUDE_ASM` or `INCLUDE_RODATA`,
it's split into its translation units, its data is C, everything is named and
typed, `build/engine.bin` still matches, and the formats the engine loads
extract and are documented.

- [ ] Match every function in `engine`.
- [ ] Split it into its original translation units.
  Status: split at every start the binary proves (switch-table parity;
  ARCHITECTURE.md, Files inside the engine); more files may hide inside
  those units. The slot overlays with proven starts are split the same way.
- [ ] Turn its data and jump-table rodata into C.
- [ ] Extract and document the asset formats the engine loads (models,
  animation, level data, ...).

## Phase 4 - The leaked minigames

Their functions start out named and typed. Finish one overlay completely
before starting the next. A minigame's box is ticked when its overlay is
source-only, the globals its header declares are named, its asset files
(its `file_group_*` groups) extract, and it passes its shift test: padded so
everything in it moves, it boots, plays through to a win the same way retail does. The slot overlays are the first units
that can be shifted: the only pointers into one from outside are the engine's
AST table entries (`Init`/`Plot`), which are C after Phase 3, so the engine
is relinked against the moved overlay without moving itself. `main.exe` and
the engine can't move until every overlay linking to them is source
(Phase 6).

**Done when** Tank, Pogo, Arkenoid and Polar (`ovl5f`, `ovl60`, `ovl61`,
`ovl62`) are source-only and still match. Every declaration in their
`include/bash/` headers is either defined by matched code or noted in
`docs/ARCHITECTURE.md` as not in this build. Each of their asset files
extracts, and each passes its shift test.

- [ ] A shiftable `NON_MATCHING` build: an option that pads a source-only slot
  overlay's code and data, relinks the engine's AST table against it, packs a
  disc and runs the shift test.
- [ ] Finish the slot overlays' section splits (data vs rodata), so functions
  using strings and jump tables can match.
- [x] Recover the minigame-only types (`EXTbotai`, `EXTheromove`, `TA_player`,
  `EXTobject_TA`, `NASTrigger`, ...): done from the Oct 9 prototype's debug
  types (ARCHITECTURE.md, Prototype debug types).
- [ ] Pogo (`ovl60`)
- [ ] Arkenoid (`ovl61`)
- [ ] Polar (`ovl62`)
- [x] Name Tank's AST pairs. Its enum is cut off in the leak; the Oct 9
  prototype's `TANK_AST_types` gives every id, and all 68 names match the
  leak's `Init`/`Plot` prototypes.
- [ ] Tank (`ovl5f`)

## Phase 5 - The remaining overlays

One at a time, starting with code shared or near-identical between minigames.

**Done when** every slot overlay is identified in `docs/ARCHITECTURE.md`, is
source-only, still matches, its asset files extract, and it passes its shift
test (Phase 4). A box below is ticked on the same terms.

- [x] Identify which minigame or mode each remaining overlay is: the Oct 9
  prototype's AST enums name each one's game (ARCHITECTURE.md, Prototype
  debug types). `ovl69` has no AST entries; its strings and the Titles enums
  make it the front end.
- [ ] Bash (`ovl5d`, with the crates and pickups of `XTRAS_AST_types`)
- [ ] Papu (`ovl5e`)
- [ ] Race (`ovl63`)
- [ ] Spin (`ovl64`)
- [ ] Dino (`ovl65`)
- [ ] Bonk (`ovl66`)
- [ ] N2O and NOX (`ovl67`)
- [ ] Chase (`ovl68`)
- [ ] Hub (`ovl6a`)
- [ ] Titles, the front end (`ovl69`)

## Phase 6 - Finish

**Done when** every test in [The final test](#the-final-test) passes.

- [ ] No `INCLUDE_ASM` or `INCLUDE_RODATA` anywhere, and no parked functions.
- [ ] Review every hack match (`decomp.py hacks`: register pins and empty
  `asm("")` barriers). Retry each without its hack with the tools of the
  day; keep a pin only where the evidence says it's the original source (a
  register choice normal allocation never makes), and say so in its commit.
  A hack that is only a workaround gets matched properly or parked.
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
alone does not. The tools come earlier: the disc packer in Phase 2, the shifted build in Phase 4, and every slot overlay passes a shift
test of its own when it's finished. So these tests are the last check on the whole
game, not the first time anything has been tested.

1. **Clean rebuild.** A fresh clone, the user's disc and `tools/dk make check`
   (nothing else) rebuild `main.exe` and every overlay, and each matches
   `configs/USA/checksum.sha`. The disc packer then rebuilds the disc image,
   and it matches the retail image byte for byte. *Proves: nothing outside the
   repo is needed except the user's disc.*
2. **Nothing hidden.** No `INCLUDE_ASM`, `INCLUDE_RODATA`, `.incbin` of code or
   data, parked function or `func_`/`D_` name anywhere; the only binary inputs
   are the assets the extractor reads from the disc. *Proves: everything is
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
