# Architecture

How `SCUS_945.70` and its code overlays are laid out and how this repository
rebuilds them. The evidence behind each boundary is kept as comments next to it
in `configs/USA/main.yaml` and `configs/USA/overlays/*.yaml`.

## Disc

| File | Size | Contents |
|---|---|---|
| `SYSTEM.CNF` | 69 B | boots `SCUS_945.70` |
| `SCUS_945.70` | 422 KB | resident executable |
| `CRASHBSH/CRASHBSH.DAT` | 70 MB | data archive indexed by the exe; holds the engine module and 14 slot overlays (see Overlays) |
| `BASHY.` | 30 MB | unidentified (likely streamed audio/video) |
| `SPYRO3/SPYRO3.EXE`, `WAD.WAD`, `SPEECH.STR` | | Spyro: Year of the Dragon demo, out of scope |

## Executable memory map

PS-X EXE header at file offset `0x0`; the file body (offset `0x800`) loads at
`0x80010000`, `0x69000` bytes. Entry point `0x8002E7B0` (`__SN_ENTRY_POINT`).

| VRAM | File | Section | Owner |
|---|---|---|---|
| `0x80010000..0x8002D488` | `0x800` | `.text` | game code, `src/main/main.c` |
| `0x8002D488..0x8002E0F0` | `0x1DC88` | `.text` | game code built with gcc 2.7.2 + ASPSX, `src/main/8002d488.c` |
| `0x8002E0F0..0x8002E7A8` | `0x1E8F0` | `.text` | game code built with gcc 2.7.2 + ASPSX, `src/main/8002e0f0.c` |
| `0x8002E7A8..0x8004C56C` | `0x1EFA8` | `.text` | PsyQ 4.7: 281 Sony objects linked in place, plus `2MBYTE.OBJ` (asm) |
| `0x8004C56C..0x8004CEFC` | `0x3CD6C` | `.rodata` | game (strings, jump tables) |
| `0x8004CEFC..0x8004DCBC` | `0x3D6FC` | `.rodata` | PsyQ, per object (`.rdata` of the linked objects) |
| `0x8004DCBC..0x80079000` | `0x3E4BC` | `.data` | game data (blob pieces), with 32 PsyQ objects' `.data` linked from the objects |
| `0x80079000..` | | `.bss` | not yet measured |

The engine module loads at `0x80078C90`, over the last `0x370` bytes of the
exe's `.data` (zeros in the file).

How the boundaries were found:

- **Library start** `0x8002E7A8` (`__main`, 2MBYTE.OBJ): lowest address matched
  by the PsyQ signatures; no function above it calls below it (0 calls), the
  game calls into it 285 times.
- **`.rodata` start** `0x8004C56C`: the end of `.text`. **Game/library seam**
  `0x8004CEFC`: the unreferenced `$Id: sys.c` string of LIBGPU; the highest
  game reference is `0x8004CEEC`, the lowest library one `0x8004CF34`.
- **`main/8002d488`** (`0x8002D488..0x8002E0F0`, rodata `0x8004CE0C..0x8004CE5C`):
  every global store is `lui $at` + `sw`, and `j`/`jr` slots are filled by the
  assembler, which `main.c`'s gcc 2.8.1 never produces. gcc 2.7.2-psx through
  ASPSX does: `func_8002D488` (the region byte at `0xBFC7FF52`) matches with it.
- **`main/8002e0f0`** (`0x8002E0F0..0x8002E7A8`) is 2.7.2 + ASPSX too, not
  2.8.1 as first thought: `func_8002E0F0` has a `lui $at` store and an
  indexed load expanded through its destination register, `func_8002E25C`
  a `bne; nop` whose slot 2.8.1's reorg would fill, and `func_8002E618`
  matches only under 2.7.2. So 2.7.2 code runs from `0x8002D488` to the
  start of PsyQ; whether the two units are one file is still open.
  - Its indexed loads from a symbol (`lbu $2,D_X($3)`) are expanded through
    the load's own register (`lui $2; addu $2,$2,$3; lbu $2,%lo(D_X)($2)`),
    or through `$at` when that register is also the index: GNU as's rule,
    and ASPSX's. maspsx always uses `$at`, so `aspsx_fill.py` rewrites it.
  - `func_8002E0F0` is a fatal text screen: it resets the GPU, clears the
    screen, prints one message twice with `func_8002E25C` (the debug text
    printer; colour `0xFFFF`, then `0x8000` one pixel down-right) and calls
    `exit()`. The message comes from a table of 12-byte entries at
    `0x8006789C` indexed by `D_8006EA24` (the font mode): `+2` u8, `+4`/`+6`
    s16 x/y, `+8` the text pointer. `TextPos` has a fourth s16 at `+6`
    (set to 1 there).
- **Files inside `main/main`** (`0x80010000..0x8002D488`, 566 functions),
  evidence as of 2026-10-15. None of it places a boundary yet:
  - No padding between functions (every symbol ends where the next starts),
    so `.text` alignment marks no file end.
  - Style: all 2.8.1, no change of compiler inside the unit.
  - String literals: no duplicates across functions, and gcc 2.8 pools
    identical literals per file, so each shared literal ties its users to one
    file: `func_800129D0`, `func_80012AC4`, `func_80013034` ("too many loads
    in queue", `0x8004C83C`) and `func_8002C97C`, `func_8002D1D4`
    (`0x8004CDAC`). The second "rodata goes back" candidate of `decomp.py
    units` is this sharing, not a boundary.
  - `func_800134FC`'s two strings (`0x8004C818`, `0x8004C82C`) come before
    `0x8004C83C`, though its text comes after the three users above. gcc
    2.8.1 emits a literal at its first use (a `static inline` helper's at
    the helper's definition, tested), and the linker keeps `.text` and
    `.rodata` in the same file order, so no simple order of files explains
    it. Open.
  - File-local data: `decomp.py units --clusters` finds 25 runs of functions
    that share data no other code uses (183 of 566 functions); each run is
    one file, so boundaries fall between runs.
  - Data blocks (`decomp.py units --data`): each object's `.data` is one
    block, linked in `.text` order, so the data a run of functions uses alone
    climbs block by block along the code. It does, in one sequence:
    functions 1-55 use `0x8004E0B4..0x8004E110`, 58-73 `0x8005063C..0x80051240`,
    85-120 `0x800512A4..0x800517C0`, 101-110 `0x800527C4`, 170-225
    `0x8005683C..0x80056AC8`, 244 `0x80058AC8`, 332-347
    `0x80058F0C..0x80059614`, 354-434 `0x8005A5C0..0x8005B78C`, 438-442
    `0x800637B4`, 446-517 `0x80063A7C..0x80064478`, 513-562
    `0x800654B8..0x80065E24`, 551-559 `0x800675C8..0x80067830` (function
    numbers are positions in `main/main`). The zero-initialised region climbs
    the same way: `0x8006E9F4` (3), `0x8006E9F8..FC` (74-78), `0x8006EA04`
    (354), `0x8006EA08..14` (438-441). So the file order is the same in code
    and data, and each block marks a region of one or a few files. It still
    places no single edge: a global used by one run can be defined in another
    file, and the stretches with no private data (121-169, 226-331) could
    hold any number of files.
  - Inversions (`units --data`, last list): when run A comes before run B
    in the code but A's private data lies after B's in the same `.data`
    chunk, B..A must be one file. Short spans (<= 40 functions; a global
    used by one far function but defined elsewhere inverts against
    everything, e.g. `D_8004E0E4` used only by function 430) give eight
    probable one-file regions: 1-16, 58-120, 170-184, 199-225, 354-430,
    458-493, 497-527, 550-557. The boundaries lie in the gaps between them
    (17-57, 121-169, 185-198, 226-353, 431-457, 494-496, 528-549, 558-565),
    and a region can still hide a boundary if one of its inversions comes
    from a misplaced global (58-120 leans on `D_80050010`, used only by
    function 101).
  - Jump-table parity (`units`, first list) is the one positive signal so
    far. cc1 puts `.align 3` before every switch table, relative to its own
    object's `.rodata`, and psylink places objects only 4-aligned, so inside
    a file every table has the same address mod 8 (an odd-length table is
    followed by a zero pad word). Proof from the engine: 13 tables in a row
    at `0x800AE128..0x800AE3D0` are all 0 mod 8, back to back, each of even
    length. So a table whose parity differs from the previous one is in a
    later file. This is also why tables of matched functions landed at the
    wrong mod-8 offset in the single `main.c` (the `goto *tbl` workaround in
    MATCHING.md). In `main/main`: a file starts after function 95 and by
    370 (it agrees with the data). The new file's rodata must start at an
    item that is 0 mod 8 between the two tables: `0x8004C8A8` (used by
    193), `0x8004C8B8` (202) or `0x8004C8D0` (358). With the one-file
    regions below that leaves three separate places for the start, 121..193,
    194..199 or 226..354, and nothing yet chooses between them, so `main.c`
    stays whole. The engine has five (after 4/by 33,
    59/77, 154/208, 244/289, 320/330), and `ovl5d` two, `ovl5e`, `ovl66`,
    `ovl67`, `ovl6a` one each (`decomp.py units overlays/<ovl>/<ovl>`).
  - Calls: 84 functions are called only by code within 40 functions and
    by nothing else (no other unit, no data table), so probably `static`
    and in their callers' file; 217 are called from outside. Together with
    the shared private data and the inversions, that leaves 17 possible cut
    zones (`units --data`, last list), eight of them one or two functions
    wide: after 0, 46, 56-57, 134-135, 456-457, 495-496, 527-528, 548. All
    of this says where a boundary can't be; nothing yet proves one: no
    duplicate literal, no style change, and the only byte-identical function
    pairs (307/312, 467/468) are neighbours, so no `static` from a shared
    header shows up in two places.
  - Who uses the rest of the data (`.data` labels with their code users,
    overlays included, and the pointers in each table): the first game data
    (`0x8004DCBC..0x8004DDD0`) is tables of pointers into the first rodata
    strings (`0x8004C56C..0x8004C7D0`) and the `D_8004DDD0` level table, used
    by the engine, `ovl69` and functions 101/332-358, so the first object is a
    shared tables file. The 2.7.2 units' data (`0x8006783C..0x80067898` for
    `8002d488`, `0x8006789C..` for `8002e0f0`) follows `main/main`'s last data
    directly, as their code follows it. Pointer tables tie functions to their
    file: `D_80058B00` lists functions 276-314 and is used by 283-297.
  - `.data` interleaves game chunks with Sony's objects (`main.yaml`: game
    data at `0x3E4BC`, `0x593D4` (the 16 KB trig table `D_80068BD4`),
    `0x5E0B4`, `0x5E3AC`, ... between library `.data`), so the game objects
    were linked among the library ones, not all before them.
  - Prototype exes: none holds a source file name (only Sony's `$Id: sys.c`
    lines); `Compiler version 2.8.1 SN32 BUILD 4.0.0009` is a game string
    that retail has too. By masked code signature the Oct 9 exe has 466 of
    the 566 functions (Sep 14: 295, Aug 23: 251) in the same order; the
    offset changes at edited functions, and no block of functions moved, so
    the link order didn't change between Oct 9 and retail.
  - Names: 15 functions are named; too few to read modules from.
  - The Aug 23 prototype's debug pages name one shell source,
    `C:\crash4\Core\Shell\PSX\PSX_PAD.CPP`, and about 50 headers under
    `CORE\NASCORE`, `CORE\SHELL\INCLUDE` (`SHELL.H`, `ALLDBASE.H`),
    `GAMEENG`, `GAMESPEC` (`FILES.H`) and `LANGUAGE`: the exe is probably the
    `Core\Shell` code, but nothing maps functions to files.
- **`.data` start** `0x8004DCBC`: the first game table (pointers to the game's
  own strings) after the last LIBMCRD string; the lowest stored-to address is
  `0x8004E0B4`.

## Overlays

`CRASHBSH.DAT` has no header; the exe indexes it with a table of
`{u32 sector, u32 size}` pairs at `0x8004E110`: 992 entries, `0x000..0x3DF`
(the pair after them, sector 0 size 0x88, starts another table). Each sector is
the previous + ceil(size / 2048), except at 12 entries (`0x012`, `0x014`,
`0x028`, `0x07C`, `0x093`, `0x09F`, `0x1DA`, `0x2A6`, `0x2AE`, `0x2D7`,
`0x335`, `0x33B`), where one sector no entry points at comes first. Those
sectors and the rest of each entry's last sector are not zeros (1.1 MB in
all); `tools/scripts/dat.py` keeps them as `filler.bin`. Scanning every entry (and the whole file) for MIPS
code finds 15 code files; nothing else on the disc holds code.

| Overlay | DAT entry | Sector | Size | Loads at | Contents |
|---|---|---|---|---|---|
| `engine` | `0x3DF` | `0x8AEB` | `0x5E4B8` | `0x80078C90` | engine module, then the reserved slot image |
| `ovl5d`..`ovl6a` | `0x1E7`, `0x2CC`..`0x2D8` | | `0x6CD8`..`0x23690` | `0x800B32B4` | slot overlays, one at a time |

- **File names** (`configs/USA/dat_names.txt`, from CTR-tools): the engine is
  `overlays/gameeng.bin`, the slot overlays `overlays/modes/<mode>.bin`: crate
  (ovl5d), papu, tank, pogo, ball (ovl61), dash (ovl63), ring (ovl64), dino,
  mallet (ovl66), oxide (ovl67), kegs (ovl68), menu (ovl69), warp (ovl6a),
  polar (ovl62).
- **Load addresses** from `jal` targets: at `0x80078C90` 308 of the engine's
  internal calls land on prologues (next best 150); every slot overlay's calls
  into itself fit `0x800B32B4`. The engine's own `.data` stores
  `0x800B32B4`, and its header word 4 (`0x800D7144`) is the end of the slot image.
- **Engine layout**: 5 header words (4 entry pointers + image end), `.text`
  `0x80078CA4..0x80098594`, `.data`, `.rodata` (switch tables and strings,
  `0x800ADBD8..0x800AEC58`), a second writable block, then the slot image at
  `0x800B32B4` (file offset `0x3A624`): a stub with id `0x5C`, zeros, and from
  offset `0x54000` 40 KB of leftover build-machine memory (see below).
- **Files inside the engine** (split 2026-10-15; `src/overlays/engine/`, each
  with its owned header `bash/<unit>.h`, which `bash/engine.h` includes).
  Each start is proven by a switch-table parity change (see Files inside
  `main/main` for the rule); the exact function is the latest one the other
  evidence allows in the proven range (function numbers count from 0 in the
  engine):

  | Unit | First function | Proven range for the start | Rodata from |
  |---|---|---|---|
  | `engine` | `func_80078CA4` (0) | - | `0x800ADBD8` |
  | `8007ac20` | `func_8007AC20` (33) | 7..33: function 6's string stays before | `0x800ADC2C` |
  | `8007ee2c` | `func_8007EE2C` (74) | 60..77; fewest call ties at 74 | `0x800ADDB0`: the mode names `D_8009AF00` (used by 77, 78) points to |
  | `8008e27c` | `func_8008E27C` (208) | 176..208: function 175's strings can't straddle; no call ties from 192 | `0x800AE464` |
  | `8009317c` | `func_8009317C` (282) | 248..289: the memory-card text tables are used up to 247; no call ties 273..282 | `0x800AEBC8` |
  | `8009608c` | `func_8009608C` (330) | 321..330; no call ties from 328 | `0x800AEC34` |

  A start can still move earlier inside its range, and a unit can hide more
  starts where the parity doesn't change.
- **Files inside the slot overlays** (split 2026-10-15, the same way). A slot
  overlay's `.rodata` comes first (from `0x800B32B4`, 4 mod 8), then `.text`,
  `.data`; each later file has its own `.rodata` range in the yaml and an
  owned header `bash/<ovl>_<unit>.h` (slot overlays share one address range,
  so the overlay name keeps header names unique), which `bash/<ovl>.h`
  includes. Most of their functions are declared in the recovered original
  headers (`bash.h`, `papu.h`, ...), which stay as they are.

  | Overlay | Later files start at (function number, name) | Proven range | Rodata from |
  |---|---|---|---|
  | `ovl5d` | 59 `func_800BB64C`; 80 `func_800BFF1C` | 22..62 (fewest call ties at 59); 73..80 (none from 77) | `0x800B3734`; `0x800B3A18` |
  | `ovl5e` | 50 `func_800B87A0` | 15..50 (fewest call ties at 44..50) | `0x800B33FC` |
  | `ovl66` | 45 `func_800B6EFC` | 15..50 (fewest call ties at 44, 45) | `0x800B3364` |
  | `ovl67` | 75 `func_800BBDB8` | 36..75 | `0x800B336C` (`D_800B336C`, used by 75, 78, 129) |
  | `ovl6a` | 8 `func_800B4EAC` | 3..9 (fewest call ties at 8) | `0x800B330C` |

  `ovl5f`..`ovl65`, `ovl68` have tables but no parity change; `ovl69` has
  none. The other positive signs are absent everywhere too (checked
  2026-10-15): no overlay or engine string is stored twice, and no overlay
  function has the gcc 2.7.2 `lui $at` stores. So the remaining files inside
  each unit can't be placed yet.
- **Slot overlays** are named by their first word, an id (`0x5D`..`0x6A`).
  Layout: `.rodata` (id, strings, jump tables), `.text`, `.data`. The id
  word is its own unit: in every slot overlay the first file's jump tables
  are 8-aligned (`.align 3` from an 8-aligned rodata start) while the id sits
  at `0x800B32B4`, 4 mod 8, so it can't be in their object; the first C
  file's `.rodata` starts at `0x800B32B8`. Every slot overlay is split this
  way (`<ovl>_id`, 2026-10-19; `ovl69` has no tables and follows the same
  layout), so a real `switch` in a first file links at the retail table
  address; before the split it landed 4 bytes off, and the two matched ones
  (ovl61) had been `&&label` tables.
- **Which game is which**: the engine's AST table at `0x80098720`
  (`{id, Init, Plot}` x 0xAE) points into the slot. Groups by id high byte:
  `0x13` -> `ovl5f`, `0x14` -> `ovl60` (Pogo), `0x15` -> `ovl61` (Arkenoid),
  `0x16` -> `ovl62` (Polar), `0x17` -> `ovl63`, `0x11`/`0x02` -> `ovl5d`,
  `0x12` -> `ovl5e`, `0x18`..`0x1D` -> `ovl64`..`ovl68` (`0x1B`, `0x1C` both
  `ovl67`), `0x1F` -> `ovl6a`. The Pogo/Arkenoid/Polar ids match the leaked
  enums (`POGO_AST_zero = 0x1400`, ...); `ovl5f` is Tank (`TANK_AST_zero = 0x1300` in
  the Oct 9 prototype). The id group does not always name the overlay: five
  Polar ids point elsewhere, and splitting `ovl62` at them cut real functions
  mid-code (fixed 2026-10-19). `Rippled` and `InnerRipple` (0x161D, 0x161E)
  are in `ovl63`; `PathEffect`, `CrystalFlash` and `SparkGFX` (0x1626-0x1628)
  are in `ovl6a`, which spawns them. Before naming from the table, check that
  the address starts a function in the overlay (the one before it ends in
  `jr $ra`). Leaf functions reached only through this table are
  declared in `configs/USA/sym.<ovl>.txt` so splat splits them.
- **AST names**: the field order is fixed by the engine's spawn code
  (`SpawnNASFromRecord`): it stores `+4` at `nas+0x48` and calls it at once
  (Init), and it stores `+8` at `nas+0x4C` (Plot). Each Pogo, Arkenoid and
  Polar id is named from its leaked enum entry (id = `*_AST_zero` + position).
  The functions take the leak's own `Init`/`Plot` prototype spelling where one
  exists (`Init_PogoPickup`, `InitBall`), and `Init`/`Plot` + the enum name
  otherwise (`InitGravityObject`). Arkenoid spells four objects differently
  in its enum and its prototypes: `ArkFlash`/`_ArkCameraFlash`,
  `NGDeadWall`/`_NGArkDeadWall`, `LaserWall`/`_ArkLaserWall`,
  `ArkRepulse`/`_RepulseRing`. `AST_AR_PU` is the Arkenoid pickup: it has no Init (stub) and the leak declares only `Plot_Ark_Pickup` for it. `AST_ArkDeadWall` and `AST_NGDeadWall` share
  one Plot (`Plot_ArkDeadWall`). Some of these objects live in the engine
  rather than the slot (`FloatyScore`, `ParticleEffect`, `ScriptFX`,
  `SmokePoof`, `WeatherNas`). Every other id is named the same way from the
  Oct 9 prototype's enums (see Prototype debug types): Tank's 68 names come
  out exactly as the leaked Tank header spells its `Init`/`Plot` prototypes.
  Left unnamed because one function serves several ids: `ovl5d`'s
  `func_800BBD48` (Init of `Crate` and `PACrate`), `func_800BC038` (Plot of
  `Crate`, `CrateType`) and `func_800BE9D0` (Plot of five pickups); the
  engine's `InitScriptFX`/`PlotScriptFX` also serve `AST_SpecialFX`. The
  Plot of `PACrate`/`PACrateType` (`0x800B87A0`) is not a function start in
  `ovl5d`.

### Leaked source

A scan of every disc file for long printable runs with C keywords finds one
block: 40 KB of preprocessed headers in the engine's slot image (DAT offset
`0x45C9800`), from `c:\Crash4\GameEng\{Tank,Pogo,Arkenoid,Polar}\` and
`c:\Crash4\Core\Shell\include\alldbase.h`: about 400 prototypes, the AST
enums and the minigame structs. It does not define the engine's own types.
It stays on the disc (re-extract it with any strings tool); only names and types
derived from it are committed. No other source paths or `__FILE__` strings exist
in the exe or the overlays.

The declarations are in `include/bash/`: `tank.h`, `pogo.h` (with
`psx_pogo.h`), `arkenoid.h` and `polar.h`. Each overlay's source includes its
own header (`ovl5f`, `ovl60`, `ovl61`, `ovl62`). Differences from the leak:
- Types are written as `s32`/`u16`/... and the layout is the repo's. The
  leak's comments, preprocessor residue and the inline helper bodies
  (`CalcCurrentSector`, `KeepWithinZone2`) are not kept. Those helpers appear
  as prototypes only.
- The fragments where the leak is cut off are dropped. That covers the start of
  Tank's header and the end of Polar's.
- `CheckMineToBoss` names two parameters `PosX`. The third is left unnamed.
- `Roo_FireRocket` is declared twice with different parameters. The
  one-argument form is kept, and the other is noted in a comment until the
  definition settles it.
- The engine types the leak uses but never defines (`NAS`, `EXTengine`,
  `EXTnode`, `hero_seq`, ...) are defined in `bash/nas.h`, mostly from the
  Oct 9 prototype (see Engine types); the rest stay incomplete. Declarations that
  need one of them complete are wrapped in `#ifdef BASH_HAVE_<TYPE>`; every
  guarded type is now defined (`TA_player`, `EXTobject_TA` in `bash/tank.h`).
- `bool` is 32 bits (`s32`); see Engine types.

### Prototype debug types

Three prototype discs are archived on Hidden Palace (Aug 23, Sep 14 and
Oct 9 2000; the Oct 9 one is a PAL review build, `SCES_028.34`). Keep the
images in `protos/` (git-ignored); commit only the names and types derived
from them. None ships a `.SYM` or `.MAP` file, and none has a text leak like
retail's.

The padding at the end of two of the prototype `CRASHBSH.DAT` files holds
pages of build-machine memory with SN debug type records (the `.SYM` def
format: `u32 value`, tag `0x94`/`0x96`, class, type, size, name). They hold
only types: no functions, globals or line numbers.
- **Aug 23** (DAT `0x2AD5800`): the start of an SN `LNK` object for
  `C:\crash4\Core\Shell\PSX\PSX_PAD.CPP`, its include list (about 90 headers
  under `C:\CRASH4\{CORE,GAMEENG,GAMESPEC,LANGUAGE}`), then 784 records.
- **Oct 9** (DAT `0x54B3E00`..`0x54CAA00`): 3429 records. It defines the engine's
  own types, which the retail leak lacks: `NAStype` (typedef `NAS`),
  `EXTtype` (`EXT`), `EXTengtype` (`EXTengine`), `EXTnode`, `NASnode`,
  `subengine`, `HeroInfo`, `EXTobject`, `EXTanimator`, `EXTmotion`, the
  script, trigger and map structs, `TA_player`, and the `*_AST_types` enum of
  every minigame. Struct members carry their offsets.

`tools/scripts/sym_scan.py FILE --from OFF --c` prints the records as C. The
Oct 9 layouts agree with the ones recovered from retail code, so retail is
close to that build: `NAStype` is 0x70 bytes with `InitDelete`/`Handler` at
`0x48`/`0x4C` (our `Init`/`Plot`), `trigger` `0x50`, `Engine` `0x54`, `FUX`
`0x58`, `UserInfo` `0x5C`, `Class`/`Failure`/`Type` at `0x68`/`0x6A`/`0x6C`;
`EXTengtype` has `sub`/`last` at `0x6C`/`0x70`, `Engine` (next) `0x7C`,
`platform` `0x80`, `collision_box` `0x84`, `sfx` `0x94`. Check each type
against retail code before adopting it: the enums moved between Aug and Oct
(`RACE_AST_zero` is `0x1800` in Aug, `0x1700` in Oct).

The Oct 9 enums name every id in the retail AST table (`0x0001` is
`Null_AST` in `HERO_AST_types`), and so name each slot overlay's game:

| Ids | Enum | Overlay |
|---|---|---|
| `0x00xx` | `HERO_AST_types` | none: Init and Plot are the stub `0x80026D00` |
| `0x02xx` | `XTRAS_AST_types` (crates, pickups) | `ovl5d` and the engine (`0x0205`, `0x0207`, `0x020C`, `0x020D`) |
| `0x10xx` | `TITLES_AST_types` | `ovl69` (the entries are the stub `0x80026D00`; see below) |
| `0x11xx` | `BASH_AST_types` | `ovl5d` |
| `0x12xx` | `PAPU_AST_types` | `ovl5e` |
| `0x13xx` | `TANK_AST_types` | `ovl5f` |
| `0x14xx` | `POGO_AST_types` | `ovl60` |
| `0x15xx` | `ARKENOID_AST_types` | `ovl61` |
| `0x16xx` | `POLAR_AST_types` | `ovl62` |
| `0x17xx` | `RACE_AST_types` | `ovl63` |
| `0x18xx` | `SPIN_AST_types` | `ovl64` |
| `0x19xx` | `DINO_AST_types` | `ovl65` |
| `0x1Axx` | `BONK_AST_types` | `ovl66` |
| `0x1Bxx`, `0x1Cxx` | `N2O_AST_types`, `NOX_AST_types` | `ovl67` |
| `0x1Dxx` | `CHASE_AST_types` | `ovl68` |
| `0x1Fxx` | `HUB_AST_types` | `ovl6a` |

Tank's ids are `TANK_AST_zero = 0x1300` + position, like the other three
leaked games (`AST_Shell` is `0x1301`, ..., `AST_Torpedo` `0x1322`).

`ovl69` is the front end (Titles). It has no entries in the AST table (the
`TITLES_AST_types` ids `0x1001`/`0x1002` point at the stub), but its strings
are the title menus: game type (`ADVENTURE MODE`, `BATTLE MODE`, `TOURNAMENT`,
`OPTIONS`), player count, `NEW GAME`/`LOAD GAME`, the sound and controller
options, `CHARACTER SELECT`, the arena and level choosers and `VS BATTLE`/
`TEAM BATTLE`. Those match the prototype's Titles enums (`TITLES_INIT_MAIN_MENU`,
`TITLES_INIT_PLAYTYPE_MENU`, `TITLES_INIT_CHARACTER_SELECTION`, ...;
`OPT_ONEPLAYER`, `OPT_TEAMPLAY`, ...) and the Aug 23 include list's
`GAMEENG\TITLES` headers (`LOADGAME.H`, `TEAMMODE.H`, `TOURNSEL.H`,
`ARENASEL.H`, `CHARSEL.H`, `TITLES.H`, `GAMEFLOW.H`, `BOOTUP.H`).

Where the types are in the repo: the object model in `bash/nas.h`; Tank's in
`bash/tank.h` and the structs the Polar leak is missing at the end of
`bash/polar.h`; the rest in headers named after the original `GameEng`
directories: `bash/core.h` (scripts, maps, animation tables, memory,
geometry, particles), `hero.h`, `shell.h` (pause, panel, movies, cheat menu,
camera modes, map ids), `xtras.h` (crates, pickups), `bash.h`, `papu.h`,
`race.h`, `titles.h`, `hub.h`, `spin.h`, `dino.h`, `bonk.h`, `n2o.h` (N2O and
NOX) and `chase.h`. Each overlay includes its game's header (`engine.c`
includes `hero.h` and `shell.h`). Every struct carries the prototype's size
as a compile-time check. A member whose type the fragment never defines (an
anonymous struct from a lost page, such as `fadescreen_info.master`) is a word
or byte array of the recorded size: the word arrays keep the real types'
alignment, which `fadescreen_info` and `PedestalInfo` need for their sizes.
The fragment was printed in include order, and the headers keep that order.

### Functions reached from the AST pairs

Named from the leaked prototypes, with code evidence for each name. A matching
signature alone is not enough.

- **Pogo, Ripper Roo** (helpers that only `Plot_PogoRipperRoo` reaches):
  - `Roo_ChangeDirOnEdge()`: no arguments; clamps the Roo's grid square
    (`D_800C5BD8`/`DC`) to the 8x8 board edge and picks a new direction.
  - `Roo_FindNewSquare(eng)`: the caller tests its result; it picks a random
    direction and calls `SquareIsFreeForRoo(x, y)`.
  - `SquareIsFreeForRoo(x, y)` returns 1 off the board, and otherwise checks
    `PogoSquare.occupied` (`+0xE`) and `reserved` (`+0x10`), where 5 is the
    Roo's own reservation.
  - `Roo_Bounce(nas)` returns a bool; every 14 frames it calls
    `Roo_GoBoing(eng)`, which plays one of six random sounds at the engine's
    position.
  - `Roo_UpdateVelocities(eng)` computes velocity and heading toward the target
    square. `Roo_UpdatePosition(eng)` adds them to the position and moves the
    grid reservation.
  - `Roo_ShootAtHumans(nas)` compares each player's square with the Roo's row
    and column and fires along a match.
  - The grid at `0x800C53DC` is 8x8 `PogoSquare` with rows of 0xE0, so
    `sizeof(PogoSquare)` is 0x1C (checked in pogo.h).
- **Arkenoid.** `ArkHeroRad2ObjectRepel(player, radius)` is called by
  `Plot_RepulseRing` with `(NAS+0x60, 0x800)`. It has a twin right after it
  that differs in one way: the direction is negated in Repel and not in the
  twin. That twin is `ArkHeroRad2ObjectAttract`, the next prototype in the
  leak.
  - `ArkHeroRad2ObjectCollision(player, radius, bool)` is the function right
    before Repel, as in the leak's order. It adds the radius to a size from
    the hero's engine and compares with the distance, and its two callers
    pass 0 and 1 as the third argument.
  - After `PlotBall`, the code follows the leak's order: `CalcArkInfluence`
    (`0x800BB41C`, called by `PlotBall` with `FindExtension(nas, 0x800)`, an
    `EXTmotion`), `FreeGrabbedObject(player, bool)` (`0x800BB674`) and
    `AlignGrabBall(eng, motion, object)` (`0x800BB924`, called by `PlotBall`
    with `nas->Engine`, the 0x800 extension and `nas->FUX`).
  - `PlotBall` calls three hooks that `func_800B36CC` sets per mode:
    `D_800C1C8C` (`func_800B4880` or `func_800B4AE8`: an `s16` angle or -1,
    like the leak's `s16 AR_*_BounceOnBounds(EXTengine *, s32)`; the
    match needs the `s16` return), `D_800C1C90` (`func_800C0A74`,
    `func_800C096C`, `func_800B4D9C`: `AR_*_TestOutOfBounds(EXTengine *,
    EXTobject *, bool)`) and `D_800C1C94` (`func_800B4EFC`, `func_800C08C8`:
    `AR_*_TestLaunchBounds(EXTengine *, EXTobject *)`).
    `AR_PreInitLevel`'s mode 17 installs the `AR_PI_` movers, so its hooks
    are `AR_PI_BounceOnBounds` (`func_800B4AE8`: named in notes only; with
    the leak's `s16` return it gains a sign extension, so it keeps `s32` and
    its address name), `AR_PI_TestOutOfBounds` and `AR_PI_TestLaunchBounds`,
    and its `GetContour` is `GetArkPIContour`; the other modes use
    `GetArkContour`. The leak has only BA and PI launch tests, so the other
    one is `AR_BA_TestLaunchBounds`. The non-PI bounce and out-of-bounds
    hooks could be BA, SE or NG: left unnamed. These take the leak's
    `EXTengine *` / `EXTobject *` (the out-of-bounds test writes
    `EXTobject.player`).
  - More Arkenoid, each from its code: `GetIntersect(p0, p1, q)` (segment
    intersection in x/z; `SLONG_XYZ *` as in the leak), `CreateBall(nas)`
    (splits a ball: spawns another 0x1501 at a rotated angle and returns
    it), `FinishArkLevel` (keeps the healthiest player, knocks out the rest),
    `PickArkPlayer(p)` (a random live opponent weighted by a per-player
    table), `UpdateArkFloor` (the PI floor tilt `D_800C1C68`/`6C` that
    `GetArkPIContour` reads), `SetCornerFailure(u16)` / `HandleCornerFailure`
    (start and run a wall's failure timer; the old `arg & 0xFFFF` was the
    `u16`), `LineToCircleTest` (exact leak signature), `TestArkObject2Hero
    (eng, u16 radius)` (against every hero's collision radius) and
    `TestArkObjectObstacles(eng, obj)` (from `PlotBall`; its flag word is
    `EXTobject.can_do`). Not placed: `TestArkenoidStop` / `TestForMove`
    (`func_800B60FC` maps fire buttons to kick/red kick/grab,
    `func_800B649C` steps the walk animation: neither fits), `DangerBalls`
    (`func_800B8964` finds a ball ahead but returns a bool, not a NAS).
    `Create_ArkPickup` is the only spawner of `AST_AR_PU` (0x1502, at a
    corner); `ArkCrateMaster` (every frame from `AR_UpdateLevel`) times and
    calls it. `InitARKfloor` (from `AR_InitLevel`, clears the floor/wall
    state) sits right before `UpdateArkFloor` in code, as in the leak (the
    weakest of these). The leak's MagnaRing has no AST id in the enum: cut.
  - Hero states: the engine's `HEROFunc` table (`{SetMode, Action}` pairs)
    has 10 Arkenoid pairs at `0x8009DD80`, hero actions 0x1C-0x25, the leak's
    10 `SetArkHero*`/`ArkHero*` pairs. Each is named from its own code, not
    the leak's order (the table puts Taunt before Kick):
    0x1C Idle (breathe anim, random idle anim; every state ends back here),
    0x1D Move, 0x1E Taunt (taunt anim and sound), 0x1F Kick (raises
    `hero_attack_delay`, `ArkHeroRad2ObjectCollision(.., 0)`), 0x20 RedKick
    (the same with `.., 1` and effect 0x2007), 0x21 Grab (150-frame
    `hero_weapon_timer`, then `FreeGrabbedObject`), 0x22 Winner, 0x23 Lose
    (status bits 0x200/0x400 pick them at the end), 0x24 Die (counts
    `hero_hit` down, then 0x25), 0x25 Dead (health 0, explosion, sound 0x586).
    The anim ids fit `HeroAnim_seqs`' Arkenoid order from 0x19 (breathe,
    move, idle, taunt, kick, ...). They take `u32 player` as the leak does.
- **Tank hero states** (ovl5f, the same table, actions 0x69-0x70; anim ids
  are `HeroAnim_seqs` values): 0x69 Recoil (`tankrecoil` anim, 60 frames of
  `hero_immunity`), 0x6A Die (`tankdie`; the Do spawns the explosion, then
  0x6B), 0x6B Dead (health 0, the same engine calls as `SetArkHeroDead` /
  `ArkHeroDead`), 0x6C Idle (`tankidle`, random `hero_idle_count`), 0x6E Move
  (the Do is the 3 KB driving code), 0x6F Turn (`func_800C7E68`/`EB0` enter it
  with a +-0x80 turn speed; the Do turns for `ModeCounter` frames, then back
  to Move), 0x70 Victory (the engine calls of `SetArkHeroWinner` /
  `ArkHeroWinner`). 0x6D Minelay is by elimination: its Set only checks for
  a win and its Do is empty (the `tankminelay` anim is never set).
- **Pogo hero states** (ovl60, actions 0x39-0x3F): 0x39 Idle (`pogo_idle` /
  `pogo_quick_idle`), 0x3A Pogo (the bounce; back to Idle), 0x3B Dazed
  (`pogo_dazed`, `hero_frozen` 10), 0x3C Winner (the engine's winner calls),
  0x3D Loser, 0x3E Tied, 0x3F Tazing (`pogo_tazing`). The end states are
  entered by three tests on `hero_status`, the leak's `TestHeroPogoWinner`
  (0x200, `pogo_winner` anim), `TestHeroPogoLoser` (0x400) and
  `TestHeroPogoTied` (0x100): Arkenoid's Dead state uses the same 0x200/0x400
  bits for Winner/Lose.
- **Pogo helpers** (ovl60), each from its code: `TestHeroPogo` (moving onto a
  free square enters 0x3A), `TestHeroPogoDazed` (status 0x800 enters 0x3B),
  `TestHeroPogoShoot` (fire button: the held pickup picks `Fire_Rocket(p, 0)`,
  `Fire_Rocket(p, 1)`, `Fire_RocketFourWays` or `Fire_Tazer`, which enters
  0x3F), `TestHeroPogoShootCancel` / `HeroPogoTazingCancel` (kill the tazer
  object), `Player_RemoveReserve` (clears the player's `reserved` marks; every
  Set state calls it). `Fire_Rocket` shoots one way (d-pad or facing),
  `Fire_RocketFourWays` the four ways that stay on the board, each through
  `Fire_RocketInDir(p, type, angle, speed)`, which calls
  `Create_PogoProjectile` (6 args, spawns `AST_PogoProjectile`).
  `PlayThrowNoise` / `PlayBoingNoise` play a per-hero sound (throw, landed
  bounce). Effects by hero (`hero_name`): `AddDingoSmoke` (Dingodile, Kanga),
  `AddSomeDust` (Crash, Coco, Tiny, Kong), `AddBrioSmoke` (Brio, Cortex);
  `AddTazerRing` is the tazer state's. `Pickup_Lose(p)` drops the held pickup
  (`D_800C535C[p].unk4`), leaving the quick-idle state for pickup 5 and
  unfreezing the tiles for pickup 6; three end states called it with no
  argument (K&R), now `Pickup_Lose(arg0)` with the same code.
- **Pogo tiles** (ovl60). `PogoSquare.state` (`+0x12`): 0 a pending claim
  (owner after `+0x14` frames, old owner in `+0x16`), 1 normal (owner `+0xA`,
  -1 free), 5 TNT (120-frame fuse), 6 Nitro, 7 four-way arrow, 10 locked
  (240 frames). The setters are named by the state they write: `Tile_Set`
  (1, owner), `Tile_SetLocked` (10), `Tile_DelayedSet` (0; `ZapInDir`
  writes the same with a growing delay), `Tile_SetTnt`, `Tile_SetNitro`,
  `Tile_SetFourWay`; `Tile_Claim` sets 1 (or 10 with the freeze pickup);
  `Tile_Clear` resets a square (state 1, no owner), which the others inline.
  Three more (`func_800C2294`/`22E8`/`2338`, states 4/2/3) are the leak's
  FizzleOut/DelayedFizzleOut/OnFire in some order: left unnamed.
  `Set_Tile_Under_Player(p, bool)` runs a landing: the mode rules
  (`SetTile_PogoThree` for mode 12's arrows, `SetTile_PogoFour` for mode 13),
  then claim, or `Tile_FireTnt` / `Tile_FireNitro` on states 5/6 (both daze
  players within a square: `Player_SetDazed` sets status 0x800).
  `Tile_FireAllNitros` fires every state-6 tile. `Tile_Update(x, z)` is called
  after every change. Mode 13: `PogoFour_PlayerHasStoodOnOwnTile` frees all
  the player's tiles, `PogoFour_FreezeMyTiles` / `UnfreezeMyTiles` toggle
  state 10. `ZapFourWays` and `ZapArrow` call `ZapInDir`. Islands:
  `Tiles_CheckForIslands` builds a map of the player's tiles
  (`Tiles_MakeUpIslandMap`), floods it from the edges
  (`Tiles_FloodFillIslandMap`, recursive `Island_FloodFill`, which takes the
  leak's `s32` x/z with `(u32)` casts in its bounds test) and claims what is
  enclosed (`Tiles_CountUnflagged`).
  `Bank_Tiles` banks the player's tiles and returns the count;
  `Handle_Score` adds it with `CreateFloatyPogoScore`. `Player_SnapToTile`
  centres the hero on its square; `Player_CalcForwardSpeed(p, bool)` is the
  per-hero bounce speed. `Player_SetReserve(p, x, z)` moves the player's
  reservation; `Player_CountTiles` counts its squares (the same body as
  `PogoAI_CountMyTiles`, which only the AI calls); `Player_SetPoisoned` sets
  the slot flag that makes the dazed state spin the hero, beside
  `Player_SetDazed` in code and leak. `PogoAI_InitHero` resets a bot's
  `EXTbotai` (called from `POGO_InitHero` and `PogoInit_AllHeros`). `PlaySquareChangeNoise` / `PlaySquareLockedNoise`
  are per-player sounds for claiming and for landing on a locked square.
- **Game hook tables.** Every minigame fills two engine tables, and their slot
  names come from the prototype's types: `D_8005AA70` is a `LevSpec`
  (`LevelPreInit`, `LevelInit`, `LevelRestartInit`, `LevelRoutine`,
  `LevelExit`, `EarliestTriggers`, `PostTriggers`, `SpecTriggerFunc`,
  `OnOffWindowTest`, `GetContour`, `FinishQuick`) and `D_8009D6E8` is a
  `HookInf` (`InitHero`, ..., `CreateShadow`, `UpdateShadow`, `PreMoveHero`,
  `MoveHero`, `PostMoveHero`, ..., `HeroBot`, ..., `PowerUp`, `Touched`,
  `ChooseWinner`, `CheckBounds`). The function that fills `LevSpec` is the
  game's SetUp, and the slots name the leaked functions:
  - Pogo: `POGOSetUp`; `InitialisePOGOFirst` (pre-init, fills `HookInf`),
    `InitialisePOGO` (init and restart), `UpdatePOGO`, `FirstTriggersPOGO`,
    `GetPogoContour`; hooks `POGO_InitHero`, `PogoCreateShadow`,
    `PreMovePOGO`, `PostMovePOGO`, `PogoAI_Poll` (`HeroBot`, matching its
    own evidence), `PogoHeroChooseWinner`.
  - Arkenoid: `AR_SetUp`; `AR_PreInitLevel` (fills `HookInf`; per mode),
    `AR_InitLevel` (init and restart), `AR_UpdateLevel`,
    `AR_EarliestTriggers`; hooks `AR_InitHero`, `AR_InitShadow` /
    `AR_UpdateShadow`, `Ark_HeroBot`, and per mode `AR_PreMoveHero` /
    `AR_MoveHero` / `AR_KeepInBounds` or the `AR_PI_` three (code order
    follows the leak's).
  - Tank: `TA_EarliestTriggers`; hooks `TA_InitHero`, `TA_HeroBot`,
    `TA_KeepInBounds`, `TA_CreateShadow`, `TA_PreMoveHero`, `TA_MoveHero`.
    Also from their code: `SetHullSFX` / `SetTurretSFX` /
    `SetFireBreatherSFX` (each swaps one `TA_player` sound: `HullSFX`,
    `TurretSFX`, `FireBreatherSFX`), `ResetToBasicWeapon` (999 rounds and
    the hero's basic weapon type) and `AnimateTank` (idle anim, with a
    random `tankidle1` and sound after `IdleAnimCt`; it follows
    `TA_MoveHero` in code as in the leak). `CreateMineShrapnelBits(nas)`
    (from `PlotMine` / `PlotWaterMine`) spawns four `AST_ShrapnelBit`;
    `MissileCollideWithPickup(eng)` tests a missile against the three
    pickups in `D_800CAD44` and destroys the one it hits.
    `SetMoeJoeMode(hero, mode)` is the boss tanks' mode setter: it sets
    `hero_mode` / `hero_last_mode` and runs their own handlers for idle
    (0x6C), move (0x6E), recoil (0x69) and dead (0x6B).
    Mode 9 is the Komodo Moe and Joe tank-boss fight (players 2 and 3 go
    through `SetMoeJoeMode`); in it the mine and the lobber call
    `CheckMineToBoss(x, y, z)` with their explosion point.
    `BumpSpeedsForSwamp(EXTobject_TA *)` scales `SpeedX`/`SpeedZ` by 3/2 and
    `Counter` by 2/3 (it was raw offsets +0x10/+0x18/+0x28; now the fields,
    same code).
    `GetSandbagValueTank(x, z)` / `GetSandbagValueMiss(x, z)` read the two
    byte maps `D_800CACFC` / `D_800CACB4` (one cell per 0x200 units, row
    width `D_800CAFF4`): the first is checked ahead of a tank's facing
    (`func_800B3F44`), the second on the AI's lines of fire; they follow
    `TA_CreateShadow` in code as in the leak. With the leak's `u32` return
    the callers' `s32 (*)(s32, s32)` casts went.
  - Polar (ovl62): `InitialisePOLARFirst`, `InitialisePOLAR`, `UpdatePOLAR`;
    hooks `Polar_HeroBot` and `DoNowt` (the `CheckBounds` hook, which only
    returns 0). From the AI: `PlayerActionCount(action, player)` counts the
    bots in a state (player unused; its caller had cast it to the two-arg
    form), `TestChaseChance` picks a live target and starts a chase. The
    other `Test*Chance` functions (`func_800B9398`, `B94B8`, `B9600`,
    `B9714`, `B98AC`) each roll against a per-hero table at `D_800D4E0C` and
    start an AI state, but the states have no names yet: left unnamed.
    `FindClosetPlayer(p, &dist)` (the leak's spelling) returns the nearest
    live opponent; `IsBombAvailable()` the first bomb object in
    `D_800D5294`'s list whose extension is still free.
    `ResetMass(p)` sets the hero's `mass` (0x180 for heroes 5-8, less 0x46
    with extension 0x20000000); `TestForTeamMate(src, dst)` is true in team
    games when they are team-mates; `Player2PlayerRayTest(p, target)` casts
    the facing ray at the target through `func_800BFEE0`.
    `Polar_HeroBot` clears the pad, calls `DoChooseNewAction` (which runs
    the tests that install a state function in the FUX) and then the state
    function. Each state function starts with
    `EnsureMateNotObstructingMate(p)` (team games: when the nearest player is
    a close team-mate and the two face each other, it separates them and
    returns 1); its callers called it with no argument (old-style prototype),
    now with `player`, same code. `DoesPlayerHaveBomb(p)`: a bomb in
    `D_800D5294`'s list held by p (also inlined as `Ovl62HasBomb`).
    `XZBoxToSphere` (box of one subengine against another's sphere) is named
    by its own debug message; ovl63 and ovl65 have their own copies.
  `LevSpec.PostTriggers` (`D_8005AA88`, typed `void (*)(NASTrigger *, s32)`
  in main.h) holds the leak's per-game triggers function: `TA_Triggers`,
  `AR_Triggers` and `POLARtriggers` are all empty stubs (Pogo leaves the
  slot unset, so `POGOriggers` has no code).
  The `CheckBounds` functions take the leak's `EXTengine *`; their old local
  stand-in structs were `unk10.x/z`, `collision_box.radius/flags` and `unk1E`.
- **Pogo challenges** (ovl60). The challenge flags at `D_8005A614+0x32` are
  0x80 crystal, 0x40 relic, 0x20 gem, none trophy/normal. Evidence:
  `PogoInit_CrystalChallenge` (flag 0x80) spawns the crystals, and
  `PogoWinner_Crystal` (0x80) is the only winner that hands off, to
  `PogoWinner_CrystalOther` on modes 11/13, as the leak's pair suggests;
  `PogoInit_RelicChallenge` (0x40) sets score targets; the init functions
  sit in reverse leak order (`PogoInit_TrophyChallenge` with no flag,
  `PogoInit_GemChallenge` 0x20, ..., crystal) and the winners in leak order
  (`PogoWinner_Relic` 0x40, `PogoWinner_Gem` 0x20, `PogoWinner_Normal` the
  fallback), both giving 0x20 = gem. Also `PogoInit_Camera`,
  `PogoInit_AllHeros` (per-hero setup) and `InitPogo_Tiles` (clears the
  board). `UpdatePOGO` runs `PogoQuality_Update` (`func_800C2EE0`, every
  frame), `Update_Occupancy` (rebuilds `PogoSquare.occupied` from the heroes
  and the Roo) and `Update_Tiles` (tile timers: TNT fuse, Nitro, locks).
  `func_800B3FF0` (per-mode table, ends with `PogoQuality_Update`) is
  `PogoInit_AIQuality` or `PogoQuality_Init`: left unnamed.
- **Pogo pickups** (ovl60): `UpdatePOGO` runs `PogoPickups_Poll` and
  `PogoPowerups_Poll`, which drop `AST_PogoPickup` (0x1401) through
  `CreatePickupOnTile` and `AST_PogoPowerup` (0x1402) through
  `CreatePowerupOnTile` on a square from `FindFreeSquare` (10 random tries).
  `FindFreeSquareForMushroom` adds `SquareContainsMushroom` (a square whose
  hero holds pickup action 0xB). `PogoPadlock_IsHere(x, z)` tests the padlock
  object (`D_800C5BB0`, status >= 3), which `Create_PogoPadlock(x, z)` spawns
  (`AST_PogoPadlock`, 0x1408; at (4, 4) in mode 13); `PadlockAI_IsTileOKForPadlock(nas, x,
  z)` is its tile test (the NAS is unused). `func_800BC04C` (a square free
  of heroes, reservations, TNT/Nitro and the padlock; used by all of them)
  may be the leak's `SquareIsFreeForMushroom`: left unnamed.
- **Pogo kerbs** (ovl60, mode 11): the board's edges are four 8-entry owner
  arrays (`D_800C5B2C`, `B4C`, `B6C`, `B8C`). `PogoKerb_Init` (from the mode
  11 setup) sets the kerb timers and calls `PogoKerb_CopyData`, which copies
  pattern `D_800C5BC0` of `D_800C3C14` into them; `PogoKerb_Poll` (every
  frame from `UpdatePOGO`) runs the timer `D_800C5BC4`. Its caller passed an
  unused argument (old-style prototype); dropped, same code.
  `PogoKerb_SetMode(mode)` (from the poll) sets the kerb mode `D_800C5BD0`
  and its timers.
- **More Pogo:** `Player_RampToTile` eases the hero a quarter of the way to
  its square's centre per frame (the tazing state; `Player_SnapToTile` jumps
  there); `PogoHero_ExtraEffects` is the `PowerUp` hook (floats and spins
  the held power-up over the hero); `Tiles_PlayerIsStealing(new, old)`
  (from the projectile) hands every tile of one player to another with a
  delay; `Tile_Update`'s mode helpers are `LevelSpecific_UnclaimedTile` (the
  free-square graphic id per mode) and `LevelSpecific_ClaimedTile` (mode
  11's owned-tile neighbour mask, kerbs included).
- **Pogo AI** (ovl60). `PogoAI_Poll(p)` runs the bot (`EXTbotai`: `AIpad`,
  timers, `AImode` 1/3/4-7). Tests: `PogoAI_CanPlayerShootAnyone` (an
  opponent in the facing row or column), `...FourWays` (any row or column),
  `PogoAI_CanPlayerShockAnyone` (an opponent within
  `PogoDistanceToDestination` 0x380, which is `csqrt` of
  `PogoSquaredDistanceToDestination` >> 6), `PogoAI_IsSquareOKForPlayer(p,
  x, z, dx, dz, flags)` (6 args as in the leak). `PogoAI_ScanForClosestPickup
  (p, &x, &z)` scans with `Pickup_IsClaimableByAI`; the player-side
  `Pickup_IsClaimable` precedes it in code and leak. `PogoAI_CountMyTiles`,
  `PogoAI_CountNeighbours(x, z, p)` (including the kerbs) and
  `PogoAI_ClaimingThisTileWillEncircle` (flood-fills a trial map).
  `PogoAI_RadialSearch(p, x, z, dir, &x, &z)` has the leak's 6-arg shape; its
  off-board test is `PogoAI_RadialSearch_CheckKerbs` (the kerb arrays at
  `D_800C5B2C`/`4C`/`6C`/`8C`; `s32` x/z with `(u32)` casts, as in the leak).
  `PreMovePOGO` calls `Pickup_Claim(x, z, p)` when `Pickup_IsClaimable`
  holds; it calls `PogoAI_HeroClaimedPickup` (resets the bot's `AItimer1`
  from the quality table). `PogoAI_Poll` runs `PogoAI_MoveTowardsTarget`
  while a bot has a target (modes 3, 5-7) and `PogoAI_PrimeDirective`,
  which tries the planners and the pickup scan in order and says whether one
  set a target.
  Of the three `(p, EXTbotai *)` planners, `PogoAI_StartEncircling` decides
  through `PogoAI_ClaimingThisTileWillEncircle` (its old `Ovl60Goal` was
  `EXTbotai`: `TargetXYZ.x/z`, `AImode`); `func_800C0F08` (radial search)
  and `func_800BE6C4` (needs 9+ own tiles) are GoForMyOwn / StartEnclosing
  in some order: left unnamed.
- **Ambiguous, left unnamed:**
  - Both of the leak's `Roo_FireRocket` declarations match real functions:
    `func_800BFFF4(eng)` and `func_800C02AC(eng, dir)` both spawn
    `AST_PogoProjectile`. One of them must have another name.
  - `func_800B4260(pos, n)` (Polar) fits `CheckMelted`, `IsRayEndInZone` and
    `IsPtInALLZone`.
  - `func_800C9B5C()` counts a list linked through `+0x20`, which could be
    `CountBears`.
  - `get_angle(x, z)` returns a 12-bit angle, as the leak's inline
    `get_angle` does. That's one use, so not yet proof.
  - The rest of the reached helpers have no prototype in the leak.

## Core engine functions

The most-called functions in the exe and the engine. `get_angle` is the leak's
own name, from the inline `CalcCurrentSector` in polar.h. The others are
**descriptive names**: the leak doesn't name them, so replace them if the
originals turn up. Call counts are `jal` sites across all binaries.

| Name | Address | Calls | Evidence |
|---|---|---|---|
| `PlaySfx` | `0x80022660` | 911 | Looks the id up in the table at `D_80064508`, takes a voice from the pool at `D_800590D0+0x540`, returns it. `Roo_GoBoing` passes a random id and `&eng->unk10`. |
| `Random(n)` | `0x80015590` | 650 | Advances two seeds (`D_800517B8`, `D_800517BC`) and returns `seed % n`, or 0 for `n < 2`. |
| `FindExtension(nas, id)` | `0x800264E8` | 438 | Walks the extension chain from `NAS.Extend.Child`, matching `EXTid` (and the low byte when given). |
| `SpawnNASById(x, y, z, id, ...)` | `0x80022A3C` | 349 | Finds `id` in the AST table, then builds a `NAS` the way `SpawnNASFromRecord` does. Callers pass `AST_*` ids. |
| `SetHeroAction(player, action)` | `0x800868B4` | 310 | Stores the action at `hero+0x2C` (the previous one at `+0x4C`) and calls its handler from `D_8009DCA0`. |
| `get_angle(x, z)` | `0x8001463C` | 300 | Arctangent through the table at `D_800517C0`, giving a 12-bit angle; used as `get_angle(x, z) & 4095` as in the leak. |
| `GetEngineSlot(i)` | `0x80095E38` | 237 | `&D_800A0E78[i]` with 0x9C-byte entries (`sizeof(EXTengine)`); its sibling copies the engine template into one. |
| `KillNAS(nas)` | `0x80022FD4` | 225 | Frees the engines and extensions, unlinks from the active list (`D_8005AA08`), clears `flags`, pushes onto the free list (`D_8005AA0C`). |
| `StopSfx(voice)` | `0x800225C4` | 173 | Stops the voice (`func_8002B8F4(voice+0x2C)`) and returns it to `PlaySfx`'s pool. |
| `SetHeroSeq(player, seq)` | `0x800854C0` | 133 | Picks the per-character table from `D_8009D29C` by `hero+0x2E` (1-8, the leak's eight `hero_seq` prefixes) and sets `hero+0x56`. |
| `AddExtension(nas, size, id)` | `0x8002689C` | 75 | `AllocExtension` with no template. |
| `AddEngine(nas, id)` | `0x8002672C` | 44 | 0x9C-byte engine from a template, first one in `NAS.Engine`. |
| `AddMotion(nas)` | `0x80026540` | - | 0x34-byte `EXTmotion`, id 0x800, from the template `D_8005A9CC`; sets 0x800 in `NAS.flags`. |
| `AddObject(nas)` | `0x80026684` | - | 0x2C-byte `EXTobject`, id 0x1000, from the template `D_8005A974`; sets 0x1000 in `NAS.flags`. |
| `SpawnNASFromRecord(rec)` | `0x80022DDC` | - | Takes a `NAS` from the free list and sets it up (see Engine types). |
| `AllocExtension(owner, size, id, tmpl)` | `0x800232B0` | - | The heap allocator behind the extension calls. |
| `FreeExtension(ext)` | `0x80023404` | - | Range-checks against the heap, unlinks from the chain, frees the block. |

Also called often but not named yet, because their roles aren't proven:
`func_8008502C` (116, the `hero_seq` interpreter's 27-case switch), `func_80025A48` (94, looks up a flagged
entry through `eng->sub`), `func_80026980` (63, unlinks from a list at
`D_8005AA34`), `func_800250B0`, `func_80095BEC`, `func_80025FAC`.

## Core globals

Descriptive names (the leak doesn't name these), with types in `bash/main.h`,
`bash/engine.h` and `bash/nas.h` where they're known.

| Name | Address | Type | Evidence |
|---|---|---|---|
| `ASTTable` | `0x8005AA6C` | `ASTEntry *` | The engine's `func_8007AB8C` points it at `EngineASTTable`; the spawners index it by id with 12-byte steps. |
| `ASTCount` | `0x8005AA60` | `s32` | Set from `func_8007AC00` (which counts the entries); the bound of every `ASTTable` search. |
| `EngineASTTable` | `0x80098720` | `ASTEntry[]` | The engine's table (`{id, Init, Plot}`, see Overlays). |
| `NASActiveHead` / `NASActiveTail` | `0x8005AA04` / `0x8005AA08` | `NAS *` | Head and tail of the active list: `SpawnNASFromRecord` appends, `KillNAS` unlinks. |
| `NASFreeList` | `0x8005AA0C` | `NAS *` | Spawners pop from it, `KillNAS` pushes onto it (linked through `+0x10`). |
| `NASTemplate` | `0x8005A7C0` | `NAS` | Copied over each spawned `NAS` (0x70 bytes). |
| `EngineTemplate` | `0x8005A8D8` | `EXTengine` | Copied over each new engine by `AddEngine` and `func_80095BEC`. |
| `EnginePool` | `0x800A0E78` | `EXTengine[]` | `GetEngineSlot(i)` returns `&EnginePool[i]`. |
| `Heroes` | `0x8009D52C` | `HeroInfo[4]` | The player/hero array (see Engine types); used by 822 functions. `func_80083708` sets up all four. |
| `HeroActionTable` | `0x8009DCA0` | not typed yet | 8-byte entries; `SetHeroAction` calls entry `action`'s handler. |
| `HeroSeqTables` | `0x8009D29C` | `hero_seq **[]` | One table of sequences per character; `SetHeroSeq` indexes it by `HeroInfo.hero_name`, then by the sequence id. |
| `RandomSeedA` / `RandomSeedB` | `0x800517B8` / `0x800517BC` | `s32` | `Random`'s state; `SetRandomSeed(a, b)` (`0x800155E8`) sets both. |
| `AtanTable` | `0x800517C0` | `s16[]` | `get_angle`'s lookup table. |
| `SfxTable` | `0x80064508` | not typed yet | 2-byte entries per sound id, read by `PlaySfx`. |
| `LevelSpec` | `0x8005AA70` | the leak's `LevSpec` (declared `s32[11]`) | 11 words of level hooks, the size and order of `LevSpec`: main.exe calls `[3]` (`D_8005AA7C`), `[5]` (`D_8005AA84`) and `[6]` (`D_8005AA88`), the engine `[10]` (`FinishQuick`), and `func_80025F44` calls `[9]` (`D_8005AA94`, `GetContour`) when it's set. Arkenoid's `func_800B36CC` sets `[9]` to `func_800BFD9C` and `PlotBall` calls it. |
| `SfxVoicePool` / `SfxActiveVoices` | `0x800590D0` / `0x80059614` | not typed yet | The voice pool: free list at `+0x540`, active list at `+0x544` (`SfxActiveVoices`). |

## Engine types

The engine's object model. The definitions are in `include/bash/nas.h`, and
each size has a compile-time check. Type, tag and field names come from the
Oct 9 prototype's debug types (see Prototype debug types) wherever it defines
the type; every layout below was first recovered from retail code, and the
prototype's offsets agree with all of it. Its field types were adopted too
(the build still matches with them), except `EXTanimator.Alist`, which code
indexes as `void **`. Fields of types the fragment does not define
(`subengine`, `EXTengine`'s `xyzrf` block at `0x0C..0x60`, `_sfx_handler`)
keep code-derived names (`unkXX` where only offset and width are known).

- **Extension allocator.** `AddExtension(owner, size, id)` calls
  `AllocExtension`. That allocates `size` bytes after a 0x18-byte heap header,
  then writes `id` at `+0` (`EXTid`), the owner at `+4` (`Extend.Parent`) and
  a null at `+8` (`Extend.Child`). It appends the new extension to the owner's
  chain, which starts at `NAS.Extend.Child` and continues through each
  extension's `Extend.Child`. On failure it sets bit 0 of `NAS.Failure`. So
  every extension starts with `s32 EXTid; EXTnode Extend;` (the prototype's
  `EXTtype`, typedef `EXT`).
- **Checked against the leak.** The leak's extension structs have exactly the
  allocated size if `EXTnode` is 8 bytes and `SLONG_XYZ` is 12: `InitSmokeTrail`
  allocates 0x30 (`EXTsmoketrail`), `InitBombMaker` 0x1C (`EXTbombmaker`) and
  `InitTransmuterBeam` 0x1E0 (`EXTtransmuter`). The stores land on the leak's
  fields: `fadeuprate` at `+0xC`, `angle` at `+0x2C`, and `EXTbombmaker.timer`
  is set to 180.
- **NAS (`struct NAStype`, 0x70 bytes).**
  - `SpawnNASFromRecord(trigger)` takes one from the free list (linked through
    `Tag.Child`), copies a 0x70-byte template over it, and appends it to the
    active list (`Tag.Parent`/`Tag.Child`).
  - It sets `NASid` to 1, `Type` (the AST index) and `trigger` (the
    `NASTrigger` it was built from; `Type` there is the AST id and `Status`
    bit 0 means spawned).
  - It stores the AST entry's Init and Plot in `InitDelete`/`Handler`, calls
    `AddEngine(nas, 1)` to make the engine extension, and then calls Init.
- **EXTengine (`struct EXTengtype`, 0x9C bytes).** `AddEngine(nas, id)`
  allocates 0x9C bytes with `EXTid = id | 0x100` from a template, and sets
  `0x100` in `NAS.NASid`.
  - The first engine extension goes to `NAS.Engine`; later ones are linked
    through `EXTengine.Engine`.
  - The trigger's `TrigXYZ` is copied to `+0x10`.
  - `EXTtransmuter.it_eng` and `light_eng[]` (both `EXTengine *` in the leak)
    are filled from this same constructor, which confirms the type.
  - `sub`/`last` are the first and last of the engine's `subengine`s,
    `num_subs` their count, and `sfx` a playing sound (`_sfx_handler`, the
    repo's former `SfxVoice`) that `func_8001D174` stops when it frees them.
    `platform` pairs two engines; `KillNAS` clears it both ways.
- **subengine (0xA8 bytes).** `func_80018F3C(eng, n)` (through
  `func_8001D390`) moves `n` nodes from the free list `D_80058ACC` onto the
  engine, copying the 0xA8-byte template `D_8005A830` over each, and links them
  through `+0x5C` (next) and `+0x60` (prev). `func_8001D174` and its siblings
  call each node's `+0x58` callback with the node and return the chain to the
  free list. Pogo's grid setup (`func_800B3C44`) stores each node of such a
  chain in `PogoSquare.sub`, which the leak types `subengine *`.
- **EXTmotion (0x34 bytes, id 0x800)** and **EXTobject (0x2C bytes, id
  0x1000)** come from `AddMotion` and `AddObject` (see Core engine functions).
  The leak names them through Arkenoid: `PlotBall` passes
  `FindExtension(nas, 0x800)` to `CalcArkInfluence(EXTmotion *)`, and
  `nas->FUX` (which `InitBall` sets to `AddObject(nas)`) to the
  `AR_*_TestOutOfBounds(EXTengine *, EXTobject *, bool)` hooks and to
  `AlignGrabBall`. `FUX` holds other extension types in other objects, so it
  is an `EXT *`.
- **hero_seq** is one halfword. The sequence interpreter (`func_8008502C`)
  reads `seq[pos]` with a 2-byte stride; `0x8000 | n` is command `n` (27 of
  them, some with halfword operands; the prototype's `HEROcmds`) and other
  values are frames.
- **HeroInfo (0x6C bytes).** `func_80083708` sets up the four `Heroes`: `hero`
  from a pool at `D_8005AA1C` (AST index 3), `hero_eng = AddEngine(nas, 1)`,
  `hero_sub = hero_eng->sub`, three more engines (`AddEngine` ids 2, 3, 3:
  `hero_shadow`, `hero_powerup`, `hero_fx`), `hero_move` (0x54-byte
  `EXTheromove`), `hero_anim` (the 0x2C-byte id-0x400 `EXTanimator` from
  `func_800265DC`, which plays the current `hero_seq`; `SetHeroSeq` sets its
  `Atable`) and `hero_ai` (0x44-byte `EXTbotai`). `SetHeroAction` sets
  `hero_mode` and keeps the old one in `hero_last_mode`; `hero_name` (1-8)
  picks the character's sequence table. The prototype's sizes for the three
  extensions match the sizes code allocates.
- **bool is 32 bits.** `ArkHeroRad2ObjectCollision` and `FreeGrabbedObject`
  keep their `bool` argument in its home slot and reload it with `lw`, with no
  truncation; gcc would narrow a `char` or `short` parameter on entry.
- **Still unknown.** The layouts of `subengine`, `EXTengine.xyzrf` and
  `_sfx_handler` (the fragment names them but does not define them), and of
  the shell types (`shell_setup`, `file_group_load_info`).

## Toolchain: the assembler checked against Sony's ASPSX

The build never runs Sony's assembler: maspsx (`--aspsx-version=2.86`) rewrites
cc1's output to read like ASPSX's, GNU as assembles it, and `aspsx_fill.py`
adds slot fills retail has. On 2026-10-19 the real ASPSX (mkst/maspsx's
`aspsx` release, versions 2.56-2.86; the 16-bit ones before 2.56 don't run on
64-bit Windows) assembled the same cc1 text as our pipeline for every file's
matched C (`tools/scripts/aspsx_compare.py`; binaries in the ignored
`ext/aspsx/`):

- **gcc 2.8.1 files: identical.** Every matched function in `main.c`, the
  engine and the slot overlays (about 1,700) assembles to the same words,
  with every ASPSX version from 2.56 to 2.86, except one.
- **The exception is not ASPSX.** `func_800C0F5C`'s frame-only epilogue
  (`addiu sp,-24; jr ra; addiu sp,24` in retail): no ASPSX version fills the
  `j $31` slot; all write `addu sp; jr ra; nop`, as maspsx does. So retail's
  form comes from the game's compiler, presumably Sony's build of gcc 2.8.1
  writing this epilogue in noreorder mode, which our FSF-based cc1 doesn't.
  `aspsx_fill.py --jumps-only` gives retail's bytes and stays; only its name
  for the cause was wrong.
- **gcc 2.7.2 files (`main/8002d488`, `main/8002e0f0`): ASPSX fills fewer
  slots than retail** (32 and 16 bytes longer, every version). The fills
  `aspsx_fill.py` makes there, derived from retail, are likewise the game's
  gcc 2.7.2 build, not its assembler.
- **Consequence for matching:** where retail's code can't come from our cc1,
  suspect Sony's compiler builds (their reorg and epilogue), not the
  assembler model. maspsx's 2.86 model is right for everything we have.

## PsyQ

Version 4.7 (signature scan: 262 objects, best of all versions; `$Id` strings
dated 1998). Sony's converted libraries (`ext/psyq47/lib/*.a`) contain the
objects the game was linked with, and all of the library code is linked from
them instead of being decompiled: 281 objects, byte-identical, plus Sony's
hand-written `2MBYTE.OBJ` (`asm/main/psyq/2mbyte.s`, not in the converted
libraries). 254 were found by signature (`psyq_match.py`); the last 27 are
tiny wrappers that are byte-identical to each other apart from what they call
or which global they use (`CdSync`, `CdFlush`, `CdStatus`, ...), so the
signatures left them unnamed. `tools/scripts/psyq_gaps.py` found them by
comparing every object with each gap and checking every relocation against the
address the exe uses. `configs/USA/psyq/`:

- `objects.txt` - every object found in the exe (address, size, LIB/OBJ).
- `layout.txt` - where each object's `.data`/`.bss`/`.rdata` sat, inferred from
  its relocations; the externals each object needs, at the exe's addresses.
- `sections.ld` - the objects' `.bss` (and the `.data` of the objects in
  `data_keep.txt`) pinned `NOLOAD` at their original addresses.
- `data_keep.txt` - objects whose `.data` still comes from the exe because the
  game's copy differs from 4.7: `bios_1.o` (libcd) has one static initialised
  to 0 at `0x8006DE5C` where 4.7 has 1, so the game's libcd is a slightly
  different build. Every other object's `.data` is linked from the object
  (`decomp.py link-psyq --data` cut the data blob around them).
- `symbols.ld` - symbols psylink placed differently from the objects' own
  layout; read last so they win.

`func_80032F3C` is `abs` (`ABS_0.OBJ`; `labs` in `ABS_1.OBJ` is identical
code).

## Build pipeline

```
src/**/*.c --cpp--> .i --cc1 2.8.1 -O2 -G0--> .s --maspsx (ASPSX 2.86)--> .s --as--> build/src/**/*.c.o
asm/**/*.s (hand-written, non-function asm) --as--> build/asm/**/*.s.o
ext/psyq47/lib/*.a --ar x + psyq_objs.py fix--> build/psyq/<lib>/<obj>.o
linkers/main.ld (splat) + configs/USA/psyq/sections.ld --> build/main.full.ld
ld --> build/main.elf --objcopy--> build/main.exe   == configs/USA/checksum.sha
```

A unit can override project.mk per object (`build/<src>.o: VAR = value` in
the Makefile; `decomp.py`'s `unit_cfg` reads the same lines):
`main/8002d488` and `main/8002e0f0` use `CC_VER = 2.7.2-psx` and `ASPSX_FILL`
(`tools/scripts/aspsx_fill.py`: ASPSX's delay-slot filling for `j`/`jr` and
conditional branches, and its expansion of indexed symbol loads, plus one
delay-slot fill that Sony's gcc 2.7.2 orders differently from FSF 2.7.2's;
its docstring lists each with the function it came from). `main/8002d488` is
also built with `-fno-rerun-cse-after-loop`: without that second cse pass, a
global array read or written at several offsets keeps one `la` base, as
retail does. Every unit is built with `-fno-builtin` (`EXTRA_CC1FLAGS`): the
game calls `abs`, which gcc 2.8.1 would otherwise inline. FSF GCC 2.7.2.3's
source is in `tools/reference/gcc-2.7.2.3/`.

Functions not yet decompiled stay in the C files as `INCLUDE_ASM(...)`, which
pulls `asm/nonmatchings/<seg>/<func>.s` into the compiled object, so the build
matches at every step. splat (`make split`) regenerates `asm/` and `linkers/`
from the retail executable; it never overwrites existing `src/` files.

Overlays (registered with `decomp.py add-overlay`) get their own
`configs/USA/overlays/<ovl>.yaml`, are cut from their archive to
`disc/<ovl>.bin`, link from `linkers/<ovl>.ld` into `build/<ovl>.bin` against
the executable's symbols, and are checked by the same `checksum.sha`.
