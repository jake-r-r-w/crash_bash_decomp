# Roadmap A: done

Everything the roadmap has finished, phase by phase, with what each item found. The work still
open, the final test and what comes after are in [ROADMAP-B.md](ROADMAP-B.md). When an item there
is done, move it here in the commit that ticks it.

The goal (ROADMAP-B.md): a source-only Crash Bash, rebuilt byte for byte from C, named, typed and
split into its original files, with the assets extracted from the user's own disc. Phases 1 to 5
are done (2026-10-28), and so are three of Phase 6's items (2026-10-29).

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
  mode each slot overlay is (its AST enums; `titles`'s strings make it the
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
  (`func_800193A8`, `src/main/prims.c`) keeps its three register pins
  (`register s32 xy asm("$20")`, ...). They are the original's register
  variables: no pin-free C can give the target's `$18`-`$20` with `$16`/`$17`
  unused (2026-10-26; CONVENTIONS.md, Matching rules).

## Phase 3 - The Tank pilot

Take one slot overlay, Tank, all the way to source-only and
shift-tested, and build the tools for it so the other overlays only repeat
the recipe. A slot overlay is the first unit that can move: the only
pointers into one from outside are the engine's AST table entries
(`Init`/`Plot`), so once those are symbols the engine relinks against the
moved overlay without moving itself. `main.exe` and the engine can't move
until every overlay linking to them is source (Phase 6).

**Done when** `tank` has no `INCLUDE_RODATA` and no data asm, every
function and global in it is named and typed, every declaration in
`include/bash/tank.h` is defined by matched code or noted in
`docs/ARCHITECTURE.md` as not in this build, `build/tank.bin` still
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
  section order (`tank`'s is rodata, text, data). Generic, so every
  overlay can use it. Done 2026-10-26: `tools/scripts/data2c.py`
  (CONVENTIONS.md, Data as C).
- [x] Tank's rodata as C: the id word (`tank_id`) and the `INCLUDE_RODATA`
  of `D_800B32B8` (the animation name strings). Done 2026-10-26: the
  strings are `const char` arrays in `tank.c`, and the id word is
  `src/ps1/tank_id.c` (`TankOverlayId`, a rodata-only unit:
  splat takes a `.rodata` subsegment with no `c` one). No asm is left in
  Tank.
- [x] Tank's data and bss as C (`asm/data/overlays/tank/`): every pointer
  a symbol, every table typed with the struct the naming found. Done
  2026-10-26: all 9.9 KB are C in `tank.c`, the structured tables retyped
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
- [x] A shiftable `NON_MATCHING` build: an option (say `make shift OVL=tank
  PAD=N`) that pads a source-only slot overlay's code and data so every
  address in it moves, relinks the engine's AST table against it and packs a
  disc. The packer rebuilds `CRASHBSH.DAT` and its index when an entry grows
  (today a file may shrink but not grow), and the padded overlay must stay
  inside the slot (`0x800B32B4` up to `0x800D7144`, the slot image's end;
  Tank ends at `0x800CB294`). Done 2026-10-26: `make shift OVL=tank
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
  Passed 2026-10-26: `make shift OVL=tank PAD=0x40`, Desert Fox played
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
  2026-10-26: every start the binary proves was split already (`bash`,
  `papu`, `bonk`, `n2o`, `hub`, by jump-table parity); the leaked
  headers' order (`tools/scripts/leak_order.py`) and `units --data` prove no
  other. Pogo and Arkenoid read as one file each like Tank. Polar's
  functions 155..216 can't be placed until they are named (Polar's box).
- [x] Pogo. Done 2026-10-26: every function named (11 from the
  leak's order), rodata, data and id word as C and named, one file, its
  shift test passed (`make shift OVL=pogo PAD=0x40`, PLAYING.md). `pogo.h`
  is settled but `POGOSpecifics` (not in this build) and `number_of_banks`
  (no object fits it). Left for Phase 5: its game-state views
  (`Pogo*View`) and the tile grid's and AI's stand-ins (`PogoCell`,
  `PogoMove`, the leak's `PogoSquare`/`PogoAIPlayer`).
- [x] Arkenoid. Done 2026-10-26: every function named (6 from
  the leak's order), rodata, data and id word as C and named, one file,
  its shift test passed (`make shift OVL=arkenoid PAD=0x40`, PLAYING.md).
  `arkenoid.h`'s eight undefined names are not in this build
  (ARCHITECTURE.md, Arkenoid's data). Left for Phase 5: its game-state
  views and stand-in structs (`Arkenoid*`).
- [x] Polar. Done 2026-10-26: every function named (functions
  155..216 included: the hero actions 73..90 by the game's
  `HeroMode_nums`), rodata, data and id word as C and named, its shift
  test passed (`make shift OVL=polar PAD=0x40`, PLAYING.md). The leak
  can't place 155..216 (none of them is in `polar.h`), so it stays one
  unit; 26 of `polar.h`'s names are not identified (ARCHITECTURE.md,
  Polar's data). Left for Phase 5: its game-state views and stand-ins.
- [x] Bash (`bash`, with the crates and pickups of `XTRAS_AST_types`). Done
  2026-10-26: every function named from its code (no leaked header), rodata,
  data, id word and last byte as C, each later file's data in its own file
  (`xtras.c`, `bahero.c`), all 172 data objects named, the DAT entries it
  loads all decode, its shift test passed (`make shift OVL=bash PAD=0x40`,
  PLAYING.md). Mode 0x20's level is not identified. Left for Phase 5: its
  stand-in structs (`BashCell`, `T_BC038*`, the crate stats as the leak's
  `CrateVelocity`).
- [x] Papu. Done 2026-10-26: every function named (most from their Bash
  twins), rodata, data, id word and tail as C (`pahero.c`'s data in its own
  file), all 166 data objects named, the DAT entries it loads all decode, its
  shift test passed (`make shift OVL=papu PAD=0x40`, PLAYING.md) after the
  link fix for an overlay's own halfword labels (OVERLAY_RECIPE.md, The shift
  test). Left for Phase 5: its stand-ins (`PapuTile`, `PapuKey`, `Slot40`, the
  leak's `PapuFloor_info`/`PapuMinion_info`/`PapuSquare`).
- [x] Race. Done 2026-10-26: every function named from its code (about
  twenty uncalled in retail), rodata, data and id word as C (one file), all
  212 data objects named, the DAT entries it loads all decode, its shift test
  passed (`make shift OVL=race PAD=0x40`, PLAYING.md). Left for Phase 5: its
  stand-ins (`RaceFux*`, `RaceBarrelView` as the leak's `RaceBarrel`, the
  `EXTrace*` extensions).
- [x] Spin. Done 2026-10-26: every function named (many from their Bash and
  Papu twins), rodata, data and id word as C (one file), all 95 data objects
  named, the DAT entries it loads all decode, its shift test passed (`make
  shift OVL=spin PAD=0x40`, PLAYING.md). Left for Phase 5: its stand-ins
  (`SpinFux`, `SpinCell`, `SpinSlot`, `SpinGame`).
- [x] Dino. Done 2026-10-26: every function named (many from their Polar and
  Race twins), rodata, data and id word as C (one file), all 113 data objects
  named, the DAT entries it loads all decode, its shift test passed (`make
  shift OVL=dino PAD=0x40`, PLAYING.md) with shift.py's sequence-column rule.
  Left for Phase 5: its stand-ins (`DinoExt`, `DinoChase`, `DinoObj`, `DinoGame`).
- [x] Bonk. Done 2026-10-26: every function named, rodata, data and id word as
  C (`bohero.c`'s data in its own file), all 118 data objects named, the DAT
  entries it loads all decode, its shift test passed (`make shift OVL=bonk
  PAD=0x40`, PLAYING.md). Left for Phase 5: its stand-ins (`BonkEntry`,
  `BonkEnt`, `BonkSlot`, `BonkPad`).
- [x] N2O and NOX (`n2o`). Done 2026-10-26: every function named (many from
  Arkenoid's twins), rodata, data and id word as C (n2o.c's, then nox.c's
  from its script records), all 205 data objects named, the DAT entries it
  loads all decode, its shift test passed (`make shift OVL=n2o PAD=0x40`,
  PLAYING.md) once the engine named N2O's star field. Left for Phase 5:
  its stand-ins (`N2OSlot`, `N2ORec`, `N2ORound`, `BossGameView`, the
  asteroid and laser views).
- [x] Chase. Done 2026-10-26: every function named (many from their twins),
  rodata, data and id word as C (one file), all 143 data objects named, the
  DAT entries it loads all decode, its shift test passed (`make shift
  OVL=chase PAD=0x40`, PLAYING.md) with shift.py's mode-table rule. Left for
  Phase 5: its stand-ins (`CoalChainGame`, `ChaseAiState`, `ChasePad`, the
  coal and flame views).
- [x] Hub (`hub`: the warp rooms, the credits and the tournament podium).
  Done 2026-10-26: every function named, its six string blocks, data and id
  word as C (hub.c's data, then hubcore.c's from `0x800BC2D0`), all 207
  data objects named, the DAT entries it loads all decode, its shift test
  passed (`make shift OVL=hub PAD=0x40`, PLAYING.md) once the engine named
  the hub's functions and data it uses. Left for Phase 5: its stand-ins
  (`EngRec90248` for the gateways, the text-panel records, `HubGameView`).
- [x] Titles, the front end (`titles`). Done 2026-10-26: named, typed,
  rodata, data and id word as C, one file, and its shift test passed
  (`make shift OVL=titles PAD=0x40`, the route in PLAYING.md). Left for
  Phase 5: its views of the game-state struct (`Titles*View`).

## Phase 5 - The engine and `main.exe` to source-only (done 2026-10-28)

The same recipe for the two units that stay in place: the engine overlay
(its types are the vocabulary the minigames are written in) and the
executable.

**Done when** `src/overlays/engine/` and `src/main/` have no
`INCLUDE_RODATA` or data asm, both are split into their translation units,
every function and global in them is named and typed, `build/engine.bin`
and `build/main.exe` still match, and the formats the engine and the exe
load extract and are documented.

- [x] Split the engine into its original translation units (closed 2026-10-28). Split
  at every start the binary proves (switch-table parity; ARCHITECTURE.md,
  Files inside the engine), and Jampack's records name two of the files
  (`panel` is `PANEL.CPP`, `pause` `PAUSE.CPP`); more files may hide inside
  those units. Rechecked 2026-10-28: no further start is proven (ARCHITECTURE.md, Files
  inside the engine). No more evidence will turn up (every prototype is scanned), so the
  eight files are final; a file hiding inside one of them stays merged.
- [x] The engine's rodata as C: all 59 INCLUDE_RODATA blocks are `const char` arrays where
  they were (2026-10-26), and its 176 strings are named from their text.
- [x] The engine's 0x14-byte header as C (2026-10-28): `entry.c`'s `EngineEntry`, a
  `ModuleEntry` (`init` `RestartGame`, `update`, `render`, `quit`, `heap` `SlotHeapStart`)
  in `.text` ahead of the code; the exe runs it as `EngineModule` (`RunModules` runs a
  module until it hands over to the next, `CurrentModule`). No data asm is left in the
  engine; its two memory images (the .bss and the slot) are Phase 6's decision.
- [x] The engine's data as C (2026-10-27). `slot_refs.py` named the overlay symbol
  behind all 1086 slot words and `data2c.py engine` wrote the 515 objects, cut by
  file in link order (engine.c, asttable.c with the world records, panel.c from
  `countdown_eng`, pause.c, heroes.c, gameflow.c, levelload.c, camera.c). The
  0x465C bytes after the rodata are the engine's .bss, a memory image kept from
  the disc like the slot's (ARCHITECTURE.md, The engine's .bss). The engine's
  slot words take their addresses from the overlays' links, so `shift.py`'s
  engine-data rewrite is gone. Left from it: the shift retests below (playtests).
- [x] Name and type every engine function and global (closed 2026-10-28). Every engine
  function and global is named, from their code, the leaked headers (`HeroMode_nums`, the
  `process_info` slots) and the Oct 9 prototype's types; no `D_`, `func_` or address-named
  type is left. The globals have their real types: the model bank (`obj_dbase
  HeroModelBank[283]`, `enum DbaseIds`), `MainCamera` (the prototype's `camera_info_tag`),
  `MapScene` (`map_enviroment *`), the warp rooms and gateways (the prototype's `hub_info`,
  `HubTableEntry`, `hub_warp_info`, `HubGateway`), the panels (`PlaceRec` lists, which the
  text panel items and pause records were too), the cutscenes (`ScriptScene`, `MarkCtx` and
  `EngineVabCue` lists), the per-mode model loads (`ModeDbase`), the card slots
  (`CardSlot`, a saved `GameState`), the pointer tables (texts, files, sequences, the
  pause menus' `PlaceRec ****` sets). `GameInfo` is one `GameState` (main.h; each field the
  global at that address), its 23 engine views folded and read through `GAME`; the labels
  stay globals of their own (the game state decision below). Functions take typed
  parameters (the weather `NAS *`, the script emitter's `EXTscriptdata`, `threaddata` and
  `partdata`, `PlaceSub`, `camera_info_tag *`, ...). What still reads bytes keeps a comment
  saying why: a pointer the matching needs (a byte walk, a separate counter, an address
  added the other way round: InitialiseHeroes, InitGameState, PickCpuHeroes, SetTweakAi,
  RecordWin, UpdatePause, RenderScreenLabels), a file record with no source type yet (the
  map's paths and zones, the mode text header, InitScriptFX's offset to a thread: the asset
  formats item), the subengine's variable tail (the prototype's `prim_` union, not in the
  fragment: the HUD bars), or another unit's extension (Race's kart).
- [x] Type the game state (2026-10-28). `GameMode`, `GameTeams` and the other labels are
  globals of their own (the user's choice: Pogo's `Tiles_PlayerIsStealing` reloads
  `GameMode` by label in a loop, which a struct field can't give; MATCHING.md, Diff
  patterns), and the rest of `GameInfo`'s 0x18C bytes are objects around them, every
  label the code names by address defined in C: `GamePlayers` (+0x26), `GameArena`
  (+0x36), `GameTeamLeader` (+0x48), `GameHeroCpu` (+0x5C), `GameWinner` (+0x60), and
  from +0x70 the adventure's progress, `GameAdventure` (`AdventureState`, laid out as
  gameflow.h's `GameStateView`). `GameTeams` stays an array: as a scalar, Nox's
  `PlotMineSFX` loads it ahead of a store through a pointer (scalars and struct fields
  are different alias classes). Left: the views (`HubGameView` ..) and the byte-offset
  users stay; turning a constant-offset user into the global at that offset changed the
  code in every unit tried (2026-10-28), so the game read those through `GameInfo`.
- [x] Name `EngineHooks`' fields (2026-10-27), from what the games put in each slot
  (`InitHero`, `HeroModels`, `RunSpeeds`, `PreMoveHero` .. `KeepInBounds`; `unk8` is
  always 0), in `EngineHooks` and in the overlays' views of `HeroHooks` (Bonk, Chase,
  N2O, Pogo, Polar, Race, Spin). Left: merging the views into the one type.
- [x] Extract and document the asset formats the engine loads (models,
  animation, level data, ...) that Phase 2's extractor still keeps as bytes (closed 2026-10-28). Each format gets a
  C struct the code reads it through (the exe's raw file offsets, from the name-and-type item)
  and its extractor in `tools/scripts/`, documented in [ASSETS.md](ASSETS.md). Status
  (2026-10-28): texture sets done: their animated textures (`tail.bin`) come out as frame PNGs
  and tex.json fields (all 86 sets pack back), and the code reads the file through `TexFile`
  and `TexImage`. Model files: the code reads them through `ModelFile` (every header section
  named, the maps through the prototype's `map_list`/`map_map`), `MeshHeader`, `MeshVerts`,
  `ModelEntity`, `FileEntry` and `MeshAnimFrame` (2026-10-28; the exe's raw-offset functions 97 to
  69); mdl.py writes the level data to map.json (`mapdata.py`: paths, triggers and placements,
  editable; the contours, the arenas' walls and zones, too; the map directory listed); the
  scripts' threads (six types, their keys), the mode text's menus (txt.py: `PlaceRec` lists in
  menus.json) and the memory-card save (card.py) as well. Every file comes out editable; what is
  left is fields no code names yet (docs/ASSETS.md, What isn't decoded).
- [x] `main.exe`'s rodata as C: its six INCLUDE_RODATA blocks (`data2c.py main
  --strings-only`, 2026-10-26); no INCLUDE_RODATA is left in `src/`.
- [x] `main.exe`'s data and bss as C (2026-10-28): the game's .data cut by file in link
  order, the data-only Sony objects linked, `bios_1.o`'s .data as C (`src/ps1/bios_1.c`,
  the game's copy differs from 4.7), the word at `$gp` and the game's .bss in their files,
  and the file padded to its size (ARCHITECTURE.md, The exe's .bss). No data asm is left in
  the exe; the dashboard's data figure is 100%.
- [x] Name and type every function and global in the exe (closed 2026-10-28). The
  names, parameter types and struct names the demo discs leaked are applied
  ([PROTOTYPES.md](PROTOTYPES.md)); 447 more game functions are named from their code
  (the shell, allocator, loader, dbase, renderer, scripts, NAS calls, textures, sound,
  music, card, modchip), in the leak's styles (`mal_*`, `dbase_*`, `*_thread`,
  CamelCase for the NAS calls); the last ones from their callers, every game function
  named (2537 of 2537, 2026-10-28; empty ones nothing calls are `*_unused`). Then
  (2026-10-28): no placeholder name is left (four PsyQ functions by their SDK
  names, the address-named inlines by what they inline); one `obj_texture` for the texture
  entry (TexDst2A84C and AnimEntry folded); `PadState` for every unit's pads (PadStick,
  PadInput and 14 overlay pad views folded, the rumble a `PadRumble`); `DrawBuffer` for
  DrawBuffers/cdb; the script threads read `script_thread`, `script_header` and `partdata`
  fields; texture sets, dbases, load nodes, music slots, subengines and the trigger file's head
  typed where functions took bytes; the prim set pool (`PrimSet` lists), the music slot's
  `songs[5]`, the large clut list (`FreeNode`), the texture file's frame and scroll animations
  (`TexFrameAnim`, `texture_scroll`), `textureset_init` on its `obj_textureset`, the draw
  functions' parent `MATRIX` (its `t` the translation) and `dbase_texture_ptr`/`_entry`
  (`obj_texture`, `TexClut.pos`); obj_texture nests its +0x18 `info`, and the texture, clut
  and dbase views (AnimTex, TexAnimEntry, GfxTexEntry, the four clut entries, GfxModel,
  GfxTexSet) are folded. What is left raw moves to the asset formats item: 97 exe functions
  with raw offsets (399), all
  the heap's and pools' own block arithmetic and the files' self-relative layouts (the model
  file's header and mesh records, the mesh animations, the texture file, the card image, the
  script table at +0x4C, the map's path table), which want the file-format structs of the asset
  formats item, and `export_exe_functions`' 164 slot offsets; the
  131 Sony statics the converted objects carry no names for. The overlays' 43 GameInfo views are
  folded into `GameState` (now in nas.h; `tools/scripts/fold_views.py`), and 106 overlay
  functions read GameInfo by field. For browsing cross-references across the whole binary, try Ghidra with a PSX
  loader by hand.

- [x] Shift-test again (playtests): dropped (decided 2026-10-28). Phase 6's shift of everything
  at once, and the final test's shifted build, play every overlay shifted anyway. The note as it
  stood: Bash, Arkenoid and Pogo were shift-tested
  before the overlay links took splat's labels as PROVIDEs (they had 7, 6 and 1
  pinned labels); Polar before its skating sequences (`*_seq_breathe_ft` ..) were
  split out, so 32 engine words into it didn't move then; and every overlay once
  the engine's data is C and `shift.py`'s rewrite is gone.

## Phase 6 - Finish

The items done so far; what was left of the phase is Parts 1 to 5 of [ROADMAP-B.md](ROADMAP-B.md).

- [x] No `INCLUDE_ASM` or `INCLUDE_RODATA` anywhere (closed 2026-10-28): none since 2026-10-26
  (every function C, every string block C), and no data asm is left in any unit (`decomp.py
  progress`: data 100%).
- [x] Every remaining disc file (music, movies, anything not covered above)
  extracts, and the formats are documented (closed 2026-10-28): every `CRASHBSH.DAT` entry
  decodes to editable files (texture sets, models with their animations, level data and scripts,
  title screens, mode text and menus, sound packs with the music as SEQ and MIDI), and the save
  on a memory card too (`card.py`). The disc has no movies: `BASHY.` is 30 MB of zeros, and
  `SPYRO3/` is the bundled Spyro 3 demo, not Crash Bash's (docs/ASSETS.md).
- [x] Shift everything at once: pad `main.exe` and the engine too, and relink
  every overlay against their new addresses instead of the fixed ones in
  `configs/USA/sym.main.txt` and `sym.engine.txt`. This needs every overlay's
  data to be C, since a pointer left in a data blob keeps its old address.
  Done 2026-10-29: `make shift-all` (`tools/scripts/shift_all.py`) moves the exe, the engine
  and all 14 overlays at once (the engine to `0x80078D50`, the slot to `0x800B3434`), and the
  disc boots and plays (playtested: the front end, the pause screens' text, a load and a save,
  the hub, the minigames). What had kept addresses fixed: splat's name lists (now fallbacks a
  definition overrides, the rest moved by position), `HeapStart` and `SlotHeapStart`, the
  pinned PsyQ and engine .bss sections, the exe header's entry point, Sony's startup code
  (`2MBYTE.OBJ`, raw words: the tool assembles a copy that names what it uses), and the mode
  text files' pointers into the engine (docs/PLAYING.md, The fully shifted build).
- [x] Decision: the memory images (2026-10-29): source builds write zeros where the engine's .bss
  and the slot hold the build machine's leftovers; the matching build keeps reading them from the
  disc. The slot's zeros are already playtested (every shift build); the engine's .bss is
  ROADMAP-B's 2.5. The exe's .bss is zeros already (`objcopy --pad-to`).
- [x] Decision: no PS1 build with a modern compiler (2026-10-29). Every game file compiles with
  GCC 12, but the link boots to a black screen because Sony's libsnd `.bss` order differs from
  retail's, and fixing that is PS1-only work. The PC port compiles the same C with a modern
  compiler and finds the same bugs (ROADMAP-B 6.1), so the final test's modern-compiler test moved
  there. The shelved link is `scratch/modern/modern.py` (AGENT_LOG, Open leads).
- [x] Type the 113 untyped game functions (2026-10-29; ROADMAP-B's 1.1). Every game function is
  typed (`progress`: 2537 of 2537). About 48 were retyped as fields (`retype.py`), which also gave
  `ChaseAiState` its last two words and `DinoExt` its physics record at +0x94. 58 put their
  layout's byte arithmetic through named macros (`FILE_AT`, `SELF_REL`, the heap's `MAL_*`).
  Seven keep a byte form the matching needs, each marked `/* matching:` with what the typed form
  changes (CONVENTIONS.md, Quality levels).
- [x] Fold the overlays' views of engine globals into one type each (2026-10-29; ROADMAP-B's old
  1.2). Every overlay file includes `engine.h` first. The overlay headers' 218 copies of engine
  and exe declarations are gone, and the hook, pad, trig, arena-cell and jump-entry views are
  folded into `EngineHooks`, `PadState`, `TrigEntry`, `ArenaCell` and `HeroJumpEnt`. Nox uses
  pause.h's sparks (`SparkSet.prims[2]`). Names only the overlays declared moved to the defining
  file's header, and four got their real types: `PickupCounts` is `u16[30]`, `RoundWinner` a
  `u16`, `HeadOfBalls2` a `void *`, and `DirPadBits` an `s32[24]`. Three calls keep the
  original's view with a `/* matching:` cast: Chase's unprototyped `pad_rumble_big` (0xF0 as an
  int), Nox's `int`-typed `SetHeroAction` (jump2 doesn't merge it), and the `HeroesTouched` hook
  read through a cast. `decl_dups.py --views` is at zero, and its test keeps it there. The
  Makefile now deletes an object whose compile failed (`.DELETE_ON_ERROR`): before, a failed
  cc1 still left the assembler's empty object behind.
- [x] The slot overlays' stand-in structs as the recovered headers' types, where one exists
  (2026-10-29; ROADMAP-B's 1.2). Folded, each still byte-identical:
  - Pogo: `PogoCell`/`PogoMove` are `PogoSquare`/`PogoAIPlayer`;
  - Papu: `PapuCell`/`PapuKey`/`PapuStep` are `PapuSquare`/`PapuFloor_info`/`PapuMinion_info`;
  - Polar: `PolarSector` and `UkaMaskSeg` are `psector`, `ExtSmokeTrail` is `EXTsmoketrail`,
    `BombMakerExt` is `EXTbombmaker`, and `TransBeamExt` is `EXTtransmuter`;
  - Race: `RaceBarrelView` is `RaceBarrel`, and the `RaceSceneObj` its `sub` points at is a
    `subengine`.

  Not in this build (no code reads them): Polar's `bdata`/`barrierdata` and Pogo's `PogoKerb`
  and `EXTPogoShockwaveInfo`. Bash's crate stats (`T_BC038Stats`, 0x12 bytes of `s16`) aren't
  the recovered `CrateVelocity` (0x16 bytes of `u16`). The other stand-ins have no recovered
  type and keep their names. A layout-only search (`scratch/pair2.py`) finds no more: generic
  word arrays fit anything.
- [x] The exe's raw-offset functions (2026-10-29; ROADMAP-B's 1.3). `tools/scripts/raw_offsets.py`
  lists the byte-pointer reads the progress check misses: it found 32 functions and 224 lines,
  and 12 functions are left.
  - Typed: the loader's records (`ModeDbase` moved to main.h, `file_group_load_info.group` is a
    `FileGroup *`), the dbase texture loads, the draw buffers, a texture set's animation block,
    the interface sprites, the script tracks' model and camera fields, and the title screens'
    TGA header (`tga_header`). The card save's offsets are named (`CARD_SLOTS`, `CARD_FLAGS`,
    `CARD_SUM`).
  - Left, each for a reason:
    - `export_exe_functions`' table: an array indexed by the byte offsets the overlays use;
    - `prim_write_uvs`: GPU packets built byte by byte, two prim layouts in one buffer;
    - `prims_init`, `dbase_init_all` and `texture_init_mem_textureset`: pool and bank walks
      by fixed strides;
    - seven helpers nothing in this build calls, so no caller shows their argument's type:
      `script_track_at`, `find_range_30`/`_24`/`_1c`, `list_count`, `name_list_find`,
      `texsrc_init`.
- [x] Name the `unkXX` fields (2026-10-29; ROADMAP-B's 1.4). Every field the code reads is named:
  uses of `unkXX` went from 12,440 to about 1,600, and declarations from 1,471 to 428.
  - What did it: the prototype's `xyzrf` block spelt out (`XYZ`, a `RotScale` `xform`) in
    `EXTengine`, `subengine` and `GfxXform`. About 90 stand-in views folded into the core
    types (`EXTobject`, `EXTengine`, `subengine`, `EXTmotion`, `EXTheromove`, `NAS`, `RaceFux`)
    and into leak types (`EXTtransmuter`, `EXTpolarAI`, `EXTpolarbomb`, `EXTbeardeath`). About
    450 fields named from their uses.
  - The 202 fields no code touches are `padXX`.
  - The 307 still called `unkXX` are only ever stored, mostly zeros and constants at init.
    Nothing reads them, so nothing shows what they hold.
  - Tools: `field_uses.py STRUCT` (every use of a struct's fields, by cc1) and `unk_fields.py`
    (every struct's unk fields sorted into none / write / read).
- [x] Structs for Spin, Dino and N2O (2026-10-29; ROADMAP-B's 1.5). Their recovered headers only
  have enums, so their structs are ours, and since 1.2 to 1.4 they're typed and named.
  - `raw_offsets.py --src` now scans any folder. In these three it found 9 functions: Dino's
    winner choice reads `GameState` fields, its contour reads use `map_map.contour_grids` and
    the segment structs, and Nox's boss keys go through `SELF_REL`.
  - The 3 lines left are typed pointer arithmetic, not byte offsets.
  - The same scan over every overlay found more (ROADMAP-B 1.8).
- [x] The prototype types' lost-page fields (2026-10-29; ROADMAP-B's 1.6). Earlier sessions had
  retyped all but 7 of the 34. The rest are typed where the code shows them:
  - `script_key_frame` (start, len);
  - `script_key_subscript` (a `script_key_anim` without the fade);
  - the entity thread's flags and first `ObjKey`, and the camera thread's `dist`;
  - `AnimKey` folded into `script_key_anim`.

  A quaternion and its slerp table stay word arrays, since that's their shape. Polar's
  `ang[2]` stays too: nothing reads it.
- [x] The overlays' byte-pointer reads (2026-10-29; ROADMAP-B's 1.8). `raw_offsets.py --src
  src/overlays` found 84 functions (375 lines); 4 are left (5 lines), each on purpose:
  - Polar's hero states, `InitScriptFX`, the cameras, the hub's gateways, titles' menus and the
    rest read their structs by name (`MapPath` keys, `EXTsmoketrail`, `EXTscriptdata`, subengines,
    `MeshHeader`, ...). Where a field read changes the code (address operand order, signedness),
    the raw form stays, marked `/* matching: */`.
  - Left: `UpdatePowerIcons` (the engine reads each game's own extension, so it can't know its
    type), Nox's two hit tests (their callers pass the object as `s32 *`) and `HeroCopyPosStub`
    (an unused stub). The fifth, `LoadModeText`'s "header" read, turned out to be the overlay
    file's `file.size`: it is `LoadModeOverlay` now, and `ModeTextFiles` is `ModeOverlayFiles`.
- [x] `func_800115D8` takes `_mal_area *` (2026-10-29; ROADMAP-B's 1.7): `mal_alloc_owned_low(s32
  bytes, void **owner, _mal_area *heap, s32 flags)`, its ten callers passing `&MainHeap`.

## Names and loose ends (ROADMAP-B Part 2, done 2026-10-29)

- [x] The three steering matches' natural forms (2.1). They are recorded as steering, and kept:
  - `func_800B4048` and `func_800BE914`: tools/cc1 variants showed the steered tie or copy is
    all that differs. No natural C shape reaches it in this compiler (MATCHING.md, the
    `QTY_CMP_PRI` and register-copy rows).
  - `PlotTransmuterBeam`: an empty block or a redundant store in place of the dead
    `if (TB_MODE13()) c++;` both lose the base's register. A counter nobody reads is plausible
    leftover code in the original, so this one may be natural as written.
- [x] The demo discs' last symbols (2.2):
  - The C++ class `file` is the archive entry `{sector, size}`. `DatIndex` is a `file[992]`,
    the loaders take `file *`, and the games' file lists are `GroupFile` triples.
  - Six functions take the demos' prototypes.
  - Every class the demos name has its stand-in, `ProcessPauseUD` is named, and the texture
    entry is one type. PROTOTYPES.md, Left, lists each.
- [x] The ambiguous names (2.3). Each is settled by its code: Pogo's `StartEnclosing` and
  `GoForMyOwn`, the `Roo_FireRocket` overload (`Roo_FireProjectile`), `CountBears` and
  `get_angle` (ARCHITECTURE.md, Functions reached from the AST pairs).
- [x] Both gcc 2.7.2 units take `-fno-rerun-cse-after-loop` (2.4). `modchip_msg` matches with
  it, and with it `msg_print` takes its plainest form.
- [x] Source builds write zeros in the engine's .bss (2.5). `shift_all.py` zeros the image as it
  does the slot (playtested by hand 2026-10-29). The game clears the intro's two words before reading them, and fills the mode
  text buffer before reading it. The zeroed build boots and loads the mode text there.

## Documentation (ROADMAP-B Part 3, done 2026-10-29)

- [x] ARCHITECTURE.md describes the game's systems (3.1). It opens with How the game works: the
  three layers (shell, engine, slot overlay), boot and the main loop, the process chain, what a
  mode is (`ModeTypes`, world records, `LevSpec`), one frame, the NAS object system, heroes,
  drawing, maps and the camera, scripts, loading and the compacting heap, sound and music, pads,
  the game state and the memory card. The layout and evidence follow as Where everything is.
- [x] The docs brought up to date with the code:
  - 1,500 `func_` addresses in the reference docs are now names. Slot addresses that several
    overlays share were resolved by the commit that wrote each line.
  - The stale notes are rewritten: unnamed functions, unknown layouts, `asm/data`, the address
    file names, Polar's leak-order recheck, and the agent log's current state.
  - Two names were fixed on the way:
    - `LoadModeText` is `LoadModeOverlay`: it also loads the minigame overlay, and its "header"
      read is the overlay's `file.size`.
    - The BIOS region byte is `BiosRegion`, the last `D_` name in `src/`.

## Part 4 - The final test (done so far; the rest is in ROADMAP-B)

- [x] 4.1 The clean rebuild (2026-10-29). The repo was cloned fresh from GitHub at `ec9a1cdc`
  into a separate folder and run through the README's Quick Start exactly:
  - the disc and its `.cue` went in `disc/`;
  - PsyQ 4.7 went in `ext/psyq47/` (copied from an unpacked archive, every file checked against
    `configs/USA/psyq/sdk.sha256`, as `tools/get_psyq.sh` does);
  - then the hooks, `decomp.py extract` (6 min), `make split`, `make check` and `make disc-check`.

  The results:
  - All 16 binaries match, and the packed disc is identical to the retail image.
  - The clone's tree stays clean after the build, and `make test-tools` passes (357 tests).

  Two limits:
  - The Docker image was the existing one, built from the same Dockerfile (its tag is the
    Dockerfile's hash), so the image build itself wasn't rerun (ROADMAP-B 5.11).
  - `make split` prints three splat "Unable to determine a segment" notices, for
    `funcEvSpError`, `funcEvSpErrorx` and `MsgFatalMetricsError`. They are harmless: the split
    and the build are unaffected (ROADMAP-B 5.12).
- [x] 4.2 Nothing hidden (2026-10-29). `make hidden` (`tools/scripts/hidden.py`, with a test) passes.
  It checks three things:
  - no asm include, `.incbin`, inline asm, `NON_MATCHING` block or `func_`/`D_` name in `src/`
    or `include/`;
  - no parked function;
  - every object the 16 links load is built from `src/` or is one of Sony's PsyQ objects.

  On the way:
  - the exe's PS-X EXE header became C (`src/ps1/header.c`). Its entry point is a link-time
    symbol, so `shift_all.py` no longer patches it and now pads after the header;
  - `mal_update`'s old parked draft under `#else` was removed.

  The scan allows these and prints each with its reason:
  - Sony's `2MBYTE.OBJ`: SDK code the repo can't hold, cut from the user's disc;
  - the engine's two memory images: build-machine leftovers, zeros in source builds;
  - `draw_verts`' register pins: the recorded exception;
  - three game-side `gte_*` macros in the SDK's own form.
- [x] 4.3 Every minigame to a win on the fully shifted build (2026-10-29).
  `tools/playtest/playtest.mjs` (docs/PLAYING.md, Playing every minigame automatically) drives
  RetroArch through its network commands, by RAM reads and writes, with no screenshots. Each mode gets
  four CPU heroes, and the harness taps Cross through the rules pages.
  - **The run.** `make shift-all PAD=0x40` played all 33 minigame modes in one run of about 5 minutes,
    each loading in about a second.
  - **Battles.** The 29 battles were all won: the 4 Bash, 4 Fox, 4 Pogo, 4 Ball, 4 Panic and 4 Dash
    arenas, Ring Ding, Dragon Drop, Mallet Mash, Keg Kaboom and the Tie-Breaker.
  - **Bosses.** The bots can't beat the four bosses their own way (Papu's crates and minion waves,
    ...). So the harness holds the heroes alive for 150 game frames of fight, then sets
    `BossState.flags` 0x8000, the flag each boss sets when it's beaten. All four were then won through
    the game's own winner choice and win sequence. Papu Pummel and Big Bad Fox were rerun on the final
    harness; Bearminator and Oxide Ride passed the same method in the boss-only run before it. Each
    boss's own defeat moment isn't covered.
  - **Hand check.** The user watched the runs, and the games played normally.
  - **What the shifted build's boot turned up:** a stray "load this mode" flag paused one level. That
    was a harness race, not the game: the fix resets the flags after each load.
- [x] 4.4 Same game (closed 2026-10-29, by decision). The user watched the shifted build play every
  minigame during 4.3's runs and judged it to behave as retail does, so a recorded side-by-side run
  wasn't made. If it's ever wanted: `tools/playtest/playtest.mjs` can log hero positions, scores and the
  clock every N frames on both builds (each build's maps give the addresses) and compare them value
  for value.

The final test is done: 4.1 to 4.4 (2026-10-29).

## Part 5 - Release (done so far; the rest is in ROADMAP-B)

- [x] 5.1 A licence for our own work (2026-10-29). `LICENSE` is MIT, "the crash_bash_decomp
  contributors". Its second half says what it covers: the tools, the build files (Makefile,
  configs/), the tests and the docs. It also says what it doesn't cover:
  - `src/` and `include/`, which reconstruct the game (the recovered original headers included);
  - `tools/reference/` (GCC's source, GPL);
  - `tools/cc1/patches/` (changes to GCC, GPL).
- [x] 5.2 The README says what the licence covers (2026-10-29). Its License section has a table, part
  by part:
  - the tooling, build files, tests and docs are MIT;
  - `src/` and `include/` are the game's (Eurocom / Sony), and nothing grants any right to it;
  - `tools/reference/` and `tools/cc1/patches/` are GPL;
  - the third-party tools the Docker image fetches keep their own licences.
- [x] 5.3 The README's preservation and research notice (2026-10-29), right after the goal:
  - what the project is for;
  - no game, SDK or compiler files are included, and the build reads them from your own disc and SDK;
  - you need your own retail copy, whose executable the split checks against retail's SHA-1;
  - game files aren't accepted in issues or pull requests;
  - no affiliation with Eurocom, Sony or Activision.
- [x] 5.4 The README's Getting started (2026-10-29), near the top: Docker and git, clone, your disc in
  `disc/`, `tools/get_psyq.sh`, `tools/dk make disc`, then play `build/CrashBash.cue`. The rest of the
  commands are in a More commands table.
- [x] 5.5 Credits (2026-10-29): the README's Credits table. It names each tool and source with what it
  was used for:
  - splat, maspsx, old-gcc, m2c, decomp-permuter, objdiff and mkpsxiso;
  - GCC and binutils;
  - the converted PsyQ libraries;
  - CTR-tools and Pillow;
  - RetroArch and PCSX ReARMed;
  - and Eurocom.
- [x] 5.6 No local RetroArch path in CLAUDE.md (2026-10-29). It points to PLAYING.md (Playing it), which
  gives RetroArch's default Windows folder as the example, and names the `RETROARCH` /
  `RETROARCH_CORE` variables the playtest harness reads for another install.
- [x] 5.7 The agent log tidied (2026-10-29): 2,766 lines to 815.
  - The session log keeps only 2026-10-28 and 29, the 40 entries since the source became complete.
    The 234 older entries are in the git history, and their results are in ROADMAP-A.
  - The stale progress-check note was removed (`check_progress.py` is gone).
  - Current state, Next, Open leads and the Gotchas stay.
- [x] 5.8 The progress history committed (2026-10-29). `.decomp/history.json` is tracked and goes in
  with each progress update, as it has since 5.1, so the tree stays clean. CONTRIBUTING's rule now
  names it next to `docs/`.
- [x] 5.9 The leak and prototype docs re-read (2026-10-29). PROTOTYPES.md and ARCHITECTURE.md (Leaked
  source, Prototype debug types) describe the sources without quoting them:
  - paths, counts, offsets and names cited as evidence;
  - no copied struct text, comments or code;
  - the 24 mangled names in PROTOTYPES are single symbols.

  The recovered headers in `include/bash/` (`tank.h`, `pogo.h`, `arkenoid.h`, `polar.h`) hold
  declarations only: enums, structs and prototypes, with no leaked comments or helper bodies.

  The one change: the four places that said where the prototype discs are archived now just name the
  discs, so nothing points to where to get game images.
- [x] 5.10 `tools/reference/` stays (decided 2026-10-29). It's the GPL source of GCC 2.8.1 and 2.7.2.3
  (27 MB), with its COPYING files, and LICENSE and the README already say it's GPL. The matching
  docs, the match-function skill and `jtrace.py` read it. A fetch script wouldn't shrink clones,
  because the history keeps it, and the whole repo packs to 18 MB anyway.
- [x] 5.11 The toolchain image builds from scratch (2026-10-29). `docker build --no-cache --pull` of
  `tools/docker/Dockerfile`, under its own tag, took 3 min 16 s. With that image:
  - `make verify` (a clean rebuild) matched all 16 binaries;
  - `make test-tools` passed (362 tests);
  - `make disc-check` rebuilt a disc identical to retail.

  So every tool the image fetches still installs and gives the same output. That closes the one gap
  4.1 left.
- [x] 5.12 `make split` prints no errors (2026-10-29). The three names 4.1 saw were only the ones whose
  names contain "Error". The real notice was bigger: every overlay's yaml reads main.exe's and the
  engine's symbol files (for its references to them), so splat listed all of those as "Unable to
  determine a segment": 1,455 for the engine, 2,427 for Tank. They still name and link correctly.
  - Marking them `absolute:True` drops them from the undefined-symbol lists the links,
    `shift.py`, `shift_all.py` and `proto_scan.py` read, so that route was backed out.
  - Instead `tools/scripts/splat_quiet.py` (with a test) filters the Makefile's splat output. It
    drops a listed symbol only when another binary's symbol file defines it, so anything else
    still prints, and it drops the known `EngineEntry`/`HeapStart` overlap.
  - `BiosRegion` is `absolute:True`, since it's outside every segment.
  - splat is pinned to 0.50.1 in the Dockerfile, because a later splat may turn the notice into an
    error.

  A split from nothing prints no notice. The clean rebuild, the tests, `make hidden`, disc-check and
  shift-all all pass on the rebuilt image.
- [x] 5.13 The final check (2026-10-29):
  - `git status` is clean.
  - `.gitignore` lists `disc/`, `protos/`, `asm/`, `assets/`, `ext/`, `build/` and `linkers/`.
  - Across all 3,282 commits:
    - no binary file was ever committed; the largest blobs are the progress JSON files, about 2 MB;
    - no file from an ignored folder;
    - no key, token or password;
    - no username, email or local path.
  - Every author email is GitHub's noreply address.
  - The README's first line no longer says "work-in-progress".
- [x] 5.14 The GitHub repo is public (2026-10-29, switched by the user; `gh repo view` reports PUBLIC):
  https://github.com/jake-r-r-w/crash_bash_decomp
- [x] 5.15 The recomp.fyi listing (2026-10-29): `.recomp.json` in the repo root, 900 bytes, in the
  board's v1 format:
  - Crash Bash, PS1, decomp; USA `SCUS-94570`;
  - status complete, toolchain Splat, started 2026-10-10;
  - the approach, the notes, three tags and the maintainer.

  It doesn't mention a ROM, an ISO or where to get the game. The board reads it daily, or at once
  through https://recomp.fyi/add.

The release is done: 5.1 to 5.15 (2026-10-29).

## Part 6 - The PC port (done so far; the rest is in ROADMAP-B)

- [x] The boundary between the decomp and the port (2026-10-30; PORT.md).
  - `src/` and `include/` are shared and must always match.
  - The 18 PS1-only files moved to `src/ps1/` with no byte changed: `header.c`, `bios_1.c`, the two
    mod-chip files and the 14 overlay id words.
  - `port/` is new and PC-only: a CMake build, the entry point, and `sdk/` and `platform/` waiting for
    6.2. It builds with the image's GCC 13; `-DBASH_BUILD_GAME=ON` picks up the 38 shared files.
  - Checked from the built ELFs: the 14 overlays' 4,368 global names have one clash,
    `XZBoxToSphere`, so they can link into one PC program.
- [x] 6.2 The PS1 layers replaced, and the game runs on PC (2026-10-30; PORT.md). A software GPU, the
  GTE and libgte as C, libcd on `CRASHBSH.DAT`, pads, a memory card file and SDL2. All 33 minigame modes
  play to the end with computer players (`--playtest 0-32`), in about two minutes. What it showed about
  the shared C: more calls relying on the caller's registers, through function pointers; null pointers
  (`LOW_RAM`); the data's layout (`data_order.txt`), and why the game's C can't be optimised. Music and
  reverb moved to 6.3.
- [x] 6.1 The game's C builds for PC with current compilers (2026-10-30; PORT.md, What's known).
  - All 38 files compile with Clang 22 and GCC 13, and link into a 32-bit Windows exe against stub PsyQ
    functions. `port/sdk/include` has the PC headers; their layouts and macros were compared with Sony's.
  - What it exposed in the shared C, each fix with `make check` still `OK`:
    - 44 calls that relied on the caller's registers, now `PASSED`;
    - `PolarAIFlags[6]`, read past a 4-word array, so the array is now 7 words;
    - a parameter redeclared in `N2OCameraFollow`;
    - `LevelUnused`, defined in both the exe and the engine;
    - `VramAreas` and `HeroSide`, labels inside larger arrays, now arrays of their own.
