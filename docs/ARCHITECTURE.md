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
| `0x80010000..0x8002E7A8` | `0x800` | `.text` | game code, `src/main/main.c` |
| `0x8002E7A8..0x8004C56C` | `0x1EFA8` | `.text` | PsyQ 4.7: 254 Sony objects linked in place, plus `src/main/psyq/` |
| `0x8004C56C..0x8004CEFC` | `0x3CD6C` | `.rodata` | game (strings, jump tables) |
| `0x8004CEFC..0x8004DCBC` | `0x3D6FC` | `.rodata` | PsyQ, per object (`.rdata` of the linked objects) |
| `0x8004DCBC..0x80079000` | `0x3E4BC` | `.data` | game and library data, still one blob |
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
- **`.data` start** `0x8004DCBC`: the first game table (pointers to the game's
  own strings) after the last LIBMCRD string; the lowest stored-to address is
  `0x8004E0B4`.

## Overlays

`CRASHBSH.DAT` has no header; the exe indexes it with a table of
`{u32 sector, u32 size}` pairs at `0x8004E110` (993 entries, each sector =
previous + ceil(size / 2048)). Scanning every entry (and the whole file) for MIPS
code finds 15 code files; nothing else on the disc holds code.

| Overlay | DAT entry | Sector | Size | Loads at | Contents |
|---|---|---|---|---|---|
| `engine` | `0x3DF` | `0x8AEB` | `0x5E4B8` | `0x80078C90` | engine module, then the reserved slot image |
| `ovl5d`..`ovl6a` | `0x1E7`, `0x2CC`..`0x2D8` | | `0x6CD8`..`0x23690` | `0x800B32B4` | slot overlays, one at a time |

- **Load addresses** from `jal` targets: at `0x80078C90` 308 of the engine's
  internal calls land on prologues (next best 150); every slot overlay's calls
  into itself fit `0x800B32B4`. The engine's own `.data` stores
  `0x800B32B4`, and its header word 4 (`0x800D7144`) is the end of the slot image.
- **Engine layout**: 5 header words (4 entry pointers + image end), `.text`
  `0x80078CA4..0x80098594`, `.data`, `.rodata` (only switch jump tables,
  `0x800ADBD8..0x800AEC58`), a second writable block, then the slot image at
  `0x800B32B4` (file offset `0x3A624`): a stub with id `0x5C`, zeros, and from
  offset `0x54000` 40 KB of leftover build-machine memory (see below).
- **Slot overlays** are named by their first word, an id (`0x5D`..`0x6A`).
  Layout: `.rodata` (id, strings, jump tables), `.text`, `.data`.
- **Which game is which**: the engine's AST table at `0x80098720`
  (`{id, Init, Plot}` x 0xAE) points into the slot. Groups by id high byte:
  `0x13` -> `ovl5f`, `0x14` -> `ovl60` (Pogo), `0x15` -> `ovl61` (Arkenoid),
  `0x16` -> `ovl62` (Polar), `0x17` -> `ovl63`, `0x11`/`0x02` -> `ovl5d`,
  `0x12` -> `ovl5e`, `0x18`..`0x1D` -> `ovl64`..`ovl68` (`0x1B`, `0x1C` both
  `ovl67`), `0x1F` -> `ovl6a`. The Pogo/Arkenoid/Polar ids match the leaked
  enums (`POGO_AST_zero = 0x1400`, ...); `ovl5f` is probably Tank (its enum is
  cut off in the leak). Leaf functions reached only through this table are
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
  `SmokePoof`, `WeatherNas`). Tank's pairs are not named: its enum is cut
  off, and the prototype order does not follow the table order (Pogo shows
  this), so the leak alone cannot match its ids to names; the prototype
  enums can (see Prototype debug types).

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
  `EXTnode`, `hero_seq`, ...) are defined in `bash/nas.h` as their layouts are
  recovered (see Engine types); the rest stay incomplete. Declarations that
  need one of them complete are wrapped in `#ifdef BASH_HAVE_<TYPE>`. Only
  `TA_player` is still guarded.
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

The Oct 9 enums name every id in the retail AST table except `0x0001`, and
so name each slot overlay's game:

| Ids | Enum | Overlay |
|---|---|---|
| `0x00xx` | `HERO_AST_types` | none: Init and Plot are the stub `0x80026D00` |
| `0x02xx` | `XTRAS_AST_types` (crates, pickups) | `ovl5d` and the engine (`0x0205`, `0x0207`, `0x020C`, `0x020D`) |
| `0x10xx` | `TITLES_AST_types` | none: the stub `0x80026D00` |
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
    with `nas->eng`, the 0x800 extension and `nas->unk58`).
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
| `FindExtension(nas, id)` | `0x800264E8` | 438 | Walks the extension chain from `NAS.ext`, matching `EXTid` (and the low byte when given). |
| `SpawnNASById(x, y, z, id, ...)` | `0x80022A3C` | 349 | Finds `id` in the AST table, then builds a `NAS` the way `SpawnNASFromRecord` does. Callers pass `AST_*` ids. |
| `SetHeroAction(player, action)` | `0x800868B4` | 310 | Stores the action at `hero+0x2C` (the previous one at `+0x4C`) and calls its handler from `D_8009DCA0`. |
| `get_angle(x, z)` | `0x8001463C` | 300 | Arctangent through the table at `D_800517C0`, giving a 12-bit angle; used as `get_angle(x, z) & 4095` as in the leak. |
| `GetEngineSlot(i)` | `0x80095E38` | 237 | `&D_800A0E78[i]` with 0x9C-byte entries (`sizeof(EXTengine)`); its sibling copies the engine template into one. |
| `KillNAS(nas)` | `0x80022FD4` | 225 | Frees the engines and extensions, unlinks from the active list (`D_8005AA08`), clears `flags`, pushes onto the free list (`D_8005AA0C`). |
| `StopSfx(voice)` | `0x800225C4` | 173 | Stops the voice (`func_8002B8F4(voice+0x2C)`) and returns it to `PlaySfx`'s pool. |
| `SetHeroSeq(player, seq)` | `0x800854C0` | 133 | Picks the per-character table from `D_8009D29C` by `hero+0x2E` (1-8, the leak's eight `hero_seq` prefixes) and sets `hero+0x56`. |
| `AddExtension(nas, size, id)` | `0x8002689C` | 75 | `AllocExtension` with no template. |
| `AddEngine(nas, id)` | `0x8002672C` | 44 | 0x9C-byte engine from a template, first one in `NAS.eng`. |
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
| `Heroes` | `0x8009D52C` | `Hero[4]` | The player/hero array (see Engine types); used by 822 functions. `func_80083708` sets up all four. |
| `HeroActionTable` | `0x8009DCA0` | not typed yet | 8-byte entries; `SetHeroAction` calls entry `action`'s handler. |
| `HeroSeqTables` | `0x8009D29C` | `hero_seq **[]` | One table of sequences per character; `SetHeroSeq` indexes it by `Hero.character`, then by the sequence id. |
| `RandomSeedA` / `RandomSeedB` | `0x800517B8` / `0x800517BC` | `s32` | `Random`'s state; `SetRandomSeed(a, b)` (`0x800155E8`) sets both. |
| `AtanTable` | `0x800517C0` | `s16[]` | `get_angle`'s lookup table. |
| `SfxTable` | `0x80064508` | not typed yet | 2-byte entries per sound id, read by `PlaySfx`. |
| `SfxVoicePool` / `SfxActiveVoices` | `0x800590D0` / `0x80059614` | not typed yet | The voice pool: free list at `+0x540`, active list at `+0x544` (`SfxActiveVoices`). |

## Engine types

The engine's object model, as far as code proves it. The definitions are in
`include/bash/nas.h`, and each size has a compile-time check.

- **Extension allocator.** `AddExtension(owner, size, id)` calls
  `AllocExtension`. That allocates `size` bytes after a 0x18-byte heap header,
  then writes `id` at `+0` (`EXTid`), the previous link at `+4` and a null at
  `+8`. It appends the new extension to the owner's chain, which starts at
  `NAS+0x08` and continues through each extension's `+8`. On failure it sets bit
  0 of `NAS+0x6A`. So every extension starts with `s32 EXTid; EXTnode Extend;`,
  and `EXTnode` is two pointers (8 bytes).
- **Checked against the leak.** The leak's extension structs have exactly the
  allocated size if `EXTnode` is 8 bytes and `SLONG_XYZ` is 12: `InitSmokeTrail`
  allocates 0x30 (`EXTsmoketrail`), `InitBombMaker` 0x1C (`EXTbombmaker`) and
  `InitTransmuterBeam` 0x1E0 (`EXTtransmuter`). The stores land on the leak's
  fields: `fadeuprate` at `+0xC`, `angle` at `+0x2C`, and `EXTbombmaker.timer`
  is set to 180.
- **NAS (0x70 bytes).**
  - `SpawnNASFromRecord(spawn)` takes one from the free list (linked through
    `+0x10`), copies a 0x70-byte template over it, and appends it to the active
    list (`+0x0C` prev, `+0x10` next).
  - It sets `flags` (`+0`) to 1, the AST index (`s16` at `+0x6C`) and the spawn
    record (`+0x50`).
  - It stores the AST entry's Init and Plot at `+0x48`/`+0x4C`, calls
    `AddEngine(nas, 1)` to make the engine extension, and then calls Init.
  - Field widths elsewhere come from a survey of every named Init/Plot (`a0` is
    the `NAS`).
- **EXTengine (0x9C bytes).** `AddEngine(nas, id)` allocates 0x9C bytes
  with `EXTid = id | 0x100` from a template, and sets `0x100` in `NAS.flags`.
  - The first engine extension goes to `NAS+0x54` (`eng`); later ones are
    linked through `+0x7C`.
  - The spawn record's first 12 bytes are copied to `+0x10`.
  - `EXTtransmuter.it_eng` and `light_eng[]` (both `EXTengine *` in the leak)
    are filled from this same constructor, which confirms the type.
  - `+0x6C`/`+0x70` are the first and last of the engine's `subengine`s,
    `+0x9A` their count, and `+0x94` a sound voice that `func_8001D174` stops
    when it frees them.
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
  `nas->unk58` (which `InitBall` sets to `AddObject(nas)`) to the
  `AR_*_TestOutOfBounds(EXTengine *, EXTobject *, bool)` hooks and to
  `AlignGrabBall`. `NAS+0x58` holds other extension types in other objects, so
  it stays `void *`. Field widths come from a survey of every access through
  those pointers.
- **hero_seq** is one halfword. The sequence interpreter (`func_8008502C`)
  reads `seq[pos]` with a 2-byte stride; `0x8000 | n` is command `n` (27 of
  them, some with halfword operands) and other values are frames.
- **Hero (0x6C bytes).** `func_80083708` sets up the four `Heroes`: `nas` from a
  pool at `D_8005AA1C` (AST index 3), `eng = AddEngine(nas, 1)`, `sub =
  eng->sub`, three more engines (`AddEngine` ids 2, 3, 3) at `+0x0C..+0x14`,
  0x54- and 0x44-byte extensions at `+0x18`/`+0x20`, and at `+0x1C` the
  0x2C-byte id-0x400 extension from `func_800265DC` that plays the current
  `hero_seq` (`SetHeroSeq` sets its `+0x0C`). The rest is halfwords; widths
  and signedness come from a survey of every `Heroes[i]` access (signed where
  most loads are `lh`).
- **bool is 32 bits.** `ArkHeroRad2ObjectCollision` and `FreeGrabbedObject`
  keep their `bool` argument in its home slot and reload it with `lw`, with no
  truncation; gcc would narrow a `char` or `short` parameter on entry.
- **Still unknown.** The names of most fields, which stay `unkXX`. The layouts
  of the minigame-only types `EXTbotai`, `EXTheromove`, `EXTobject_TA`,
  `NASTrigger` and `TA_player`, and of the shell types. Which leak names the
  hero's `+0x18`, `+0x1C` and `+0x20` extensions have.

## PsyQ

Version 4.7 (signature scan: 262 objects, best of all versions; `$Id` strings
dated 1998). Sony's converted libraries (`ext/psyq47/lib/*.a`) contain the
objects the game was linked with: 254 of 255 identified objects are
byte-identical, and are linked in place of their code instead of being
decompiled. `configs/USA/psyq/`:

- `objects.txt` - every object found in the exe (address, size, LIB/OBJ).
- `layout.txt` - where each object's `.data`/`.bss`/`.rdata` sat, inferred from
  its relocations; the externals each object needs, at the exe's addresses.
- `sections.ld` - the objects' data sections pinned `NOLOAD` at their original
  addresses (the data blob still supplies the bytes).
- `symbols.ld` - symbols psylink placed differently from the objects' own
  layout; read last so they win.

The few library functions not covered by an object are in
`src/main/psyq/unknown/` (named by address) and `asm/main/psyq/2mbyte.s`
(Sony's hand-written `2MBYTE.OBJ`, not in the converted libraries).

## Build pipeline

```
src/**/*.c --cpp--> .i --cc1 2.8.1 -O2 -G0--> .s --maspsx (ASPSX 2.86)--> .s --as--> build/src/**/*.c.o
asm/**/*.s (hand-written, non-function asm) --as--> build/asm/**/*.s.o
ext/psyq47/lib/*.a --ar x + psyq_objs.py fix--> build/psyq/<lib>/<obj>.o
linkers/main.ld (splat) + configs/USA/psyq/sections.ld --> build/main.full.ld
ld --> build/main.elf --objcopy--> build/main.exe   == configs/USA/checksum.sha
```

Functions not yet decompiled stay in the C files as `INCLUDE_ASM(...)`, which
pulls `asm/nonmatchings/<seg>/<func>.s` into the compiled object, so the build
matches at every step. splat (`make split`) regenerates `asm/` and `linkers/`
from the retail executable; it never overwrites existing `src/` files.

Overlays (registered with `decomp.py add-overlay`) get their own
`configs/USA/overlays/<ovl>.yaml`, are cut from their archive to
`disc/<ovl>.bin`, link from `linkers/<ovl>.ld` into `build/<ovl>.bin` against
the executable's symbols, and are checked by the same `checksum.sha`.
