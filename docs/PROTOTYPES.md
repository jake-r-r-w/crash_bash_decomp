# Prototype and demo discs

What each disc other than our retail target has given the decomp. The images
live in `protos/` (git-ignored); only the names and types derived from them
are committed.

Each separate build of the game's files is a fresh chance of a leak: the
build tools padded files with whatever was in the build machine's memory,
and that memory sometimes held source text or pieces of the SN linker's
`.SYM` output. Every leak found so far, retail's included, sits inside the
engine module's `CRASHBSH.DAT` entry. The leak is in the file, so a disc
that carries the same build as one already scanned (the same `CRASHBSH.EXE`
and `.DAT`) has the same leak; the scan reports that.

## Scanning a disc

1. Extract the archive on the host with 7-Zip into its own folder,
   `protos/<disc>/` (the image, with its `.cue` if it has one).
2. Run the scan; it dumps the image to `files/` first if there is no
   `files/` folder yet (`make build` must have run, for the retail maps):

   ```bash
   tools/dk python3 -I tools/scripts/proto_scan.py protos/<disc>/
   ```

3. Read `protos/<disc>/scan.md` and add the disc to the tables below.

`proto_scan.py` finds the Crash Bash exe by its DAT index, the engine module,
the slot overlays and their load addresses, and runs `sym_scan.py` (debug
records) and `leak_scan.py` (source text) over the exe and the DAT. Each
function record, and each label that lands on a function start, is matched
to the retail function (exe, engine or any slot overlay) with the most
similar code. Records can come from a link a few bytes off the shipped code,
as a whole or from some function on; the tool fits those offsets and reports
them. Globals (data labels, and the debug records' extern and static
variables) get retail addresses from the matched functions: every address an
aligned instruction pair builds (`lui` plus `addiu` or a load/store) pairs a
demo address with a retail one. A global referenced at its own start takes
that retail address (`start`); one referenced only in its fields or
elements takes their most common offset (`inside`). A module's data can
move apart from its code between the two links, so its offset is measured:
the one that puts the most globals exactly on referenced addresses. It
writes everything next to the disc:

| File | What |
|---|---|
| `scan.md` | the report: the files' SHA-1 (and any disc already scanned with the same files), where each record run is, source files, link groups, functions matched to retail, data labels, globals, text leaks |
| `sym.txt` | every record, with its file offset |
| `types.h` | the types as C |
| `lines.txt` | the line table, `address file:line` |
| `sections.txt` | the linker's section symbols |
| `globals.txt` | each global mapped to retail: module, address, name, the retail symbol there, evidence |
| `engine_<id>.bin` | the engine module cut from the DAT |
| `leaks/` | each source-text hit |

## Applying names

`proto_apply.py` turns a scan's matches into renames for one module and
hands them to `decomp.py rename --file`, which rolls back if the build stops
matching:

```bash
tools/dk python3 -I tools/scripts/proto_apply.py protos/<disc>/scan.md --module main
```

Without `--apply` it only lists. A C++ symbol gives its name without the
mangled parameters (`mal_free__FPvP9_mal_area` is `mal_free`). Matches of
0.9 and up become renames (`--min`); 0.75 up to that are listed to check by
hand. Left out, with the reason: PsyQ functions (Sony's names stay), C++
class members and operators, overloads (one C name for two functions), a
name the build already uses, two symbols on one function, and a function
the repo has already named (unless `--replace`). The renames are kept in
`protos/<disc>/renames_<module>.txt`.

## At a glance

| Disc | Build | Leak | What it gave |
|---|---|---|---|
| Retail `SCUS_945.70` | release | 40 KB of preprocessed headers (engine entry `+0x54000`) | ~400 prototypes, the AST enums and the Tank/Pogo/Arkenoid/Polar structs (`bash/tank.h`, `pogo.h`, `arkenoid.h`, `polar.h`) |
| Aug 23 2000 prototype (Hidden Palace) | early | engine entry `+0x4E000`: an SN object for `PSX_PAD.CPP`, its ~90-header include list, 788 type records | source directory and header names (`Core\Shell`, `GameEng\Titles`, ...) |
| Sep 14 2000 prototype (Hidden Palace) | early | none | exe comparison only |
| Oct 9 2000 prototype (Hidden Palace; PAL review build `SCES_028.34`) | close to retail | engine entry `+0x40002`: 3447 type records | the engine's own types (`NAS`, `EXTengine`, `subengine`, `HeroInfo`, ...), every AST enum: names for 201 AST Init/Plot functions; the `include/bash/` game headers |
| Jampack Winter 2000 demo (`SCUS_946.24`) | older than Oct 9 | engine entry `+0x2BE15`: 4611 type records, **30 function records**, 44 globals, 1021 line records | unit `8007ac20` is `PANEL.CPP`; 23 function names mapped (not applied yet) |
| PlayStation Demo Disc 1.3 (`SCUS-94618`, mastered 2000-08-30) | the Jampack build | the same as Jampack: the same `CRASHBSH.EXE` and `.DAT`, byte for byte | nothing new; dates the Jampack build to 2000-08-30 or earlier |
| PAL retail (`SCES-02834`, mastered 2000-10-23) | release | none: the engine's spare space is zeros and `0x01` filler, no leftover memory | nothing |
| Japanese retail, Crash Bandicoot Carnival (`SCPS-10140`, mastered 2000-11-16) | release | none: as PAL; the extra `B320I.EXE`/`B321I.EXE` only play the two `BANJ/*.IKI` videos | nothing |
| Euro Demo 65 (`SCED-02639`, PAL; also sold as Official PlayStation Magazine UK Demo 65) | near the demos | engine entry `+0x332CB`: **1182 symbol table labels**, 2325 type records | 163 function names mapped across `main.exe`, the engine and 6 slot overlays; 184 global names; the link layout and overlay names (not applied yet) |

The prototype exes have no source names, but by masked code signature they
hold the retail `main.exe` functions in retail order (Oct 9: 466 of 566,
Sep 14: 295, Aug 23: 251), so the link order didn't change before release.

## What kind of information came from where

| Information | Retail | Aug 23 | Oct 9 | Jampack | Euro 65 |
|---|---|---|---|---|---|
| Function prototypes (Tank, Pogo, Arkenoid, Polar) | yes | | | | |
| Minigame structs and AST enums | yes | some | yes | yes | |
| Engine types (`NAS`, `EXTengine`, ...) | | | yes | | |
| Source file and header paths | 4 game dirs | yes | | `PANEL.CPP`, `PAUSE.CPP` | |
| Function names with addresses | | | | 30 (`PANEL.CPP`) | 371 labels; 163 matched |
| Parameter types (mangled names) | | | | 30 | 140 |
| Local variables and their registers | | | | yes | |
| Global variable names | | | | 44, with types and sizes | 184 |
| Source line numbers | | | | `PAUSE.CPP`'s line table; each `PANEL.CPP` function's first line | |
| Link layout (groups, overlays) | | | | | yes |
| New enums | | | | `cheatflags`, `levnumbers`, `worldnumbers`, `IconParts` | level map enums (Tank, Pogo, ...) |

Details for the retail, Aug 23 and Oct 9 leaks are in
[ARCHITECTURE.md](ARCHITECTURE.md) (Leaked source, Prototype debug types).

## Jampack Winter 2000 demo

`protos/Jampack_Winter_2000/scan.md` has the full report.

**The disc.** A US multi-game demo; Crash Bash is `CRASHBSH/CRASHBSH.EXE`
(load `0x80018000`) and a 41 MB `CRASHBSH.DAT`, both dated 2000-10-09. No
`.SYM`, `.MAP` or source text anywhere on the disc. PlayStation Demo Disc
1.3, mastered 2000-08-30, carries the same two files byte for byte, so the
build is from late August 2000 at the latest. That fits its age: the AST ids
are numbered before Race existed
(`BEAR_AST_types` at `0x1700`, `SPIN` `0x1900`, `DINO` `0x1A00`, ...; retail
has `RACE` at `0x1700`), and `TA_player`/`CH_player` are smaller (`0x9C`/
`0x48` against Oct 9's `0xA4`/`0x54`).

**Where the fragment is.** The exe's DAT index is at `0x8004AF84` (486
entries). The engine module is entry `0x1E5` (sector `0x4E28`, size
`0x4F158`), loaded at `0x80071764`: its header pointers and jal targets land
on function starts there. The records sit in that entry at `+0x2BE15`
(DAT `0x0273FE15..0x0275DFE5`). Their addresses are all 4 bytes below the
shipped code: they come from a link of the same code one word lower (the
exe a word shorter, say), not from the shipped build. `proto_scan.py` finds
such a shift itself. The addresses below are the records'.

**What it holds.**
- 30 function records, all `C:\crash4\GameEng\Nas\PANEL.CPP` (the in-game
  panel: clock, scores, health, power and cup icons). Each has its mangled
  name (parameter types: `MakeSubBar(subengine *, short, short, uint, uint)`),
  frame size, first line, end, and its blocks' locals with storage class and
  register (`Set_Clock`: `i` in `$t3`, `n` in `$v1`).
- 44 `PANEL.CPP` globals with addresses, types and sizes: `PanelTab`,
  `Clock`, `Score`, `BigScore1`/`2`, `IconParts`, `CupTimer`, `CupMode`,
  `pHeroHealth`, the icon position tables, ...
- 4611 type records (163 names; 116 in Oct 9's). New: `cheatflags`
  (`cheat_mode_no_death`, `cheat_mode_freeze_time`, ...), `levnumbers`
  (every level, including `LEV_HUBDEMO`, `LEV_E3_HUB0`/`1`, `LEV_GFXTEST`),
  `worldnumbers`, `IconParts`, the demo-only `BEAR_AST_types`, and PsyQ SDK
  structs. Of the 35 types both define, 31 have the same size as Oct 9's.
- 1021 line records, all `C:\crash4\GameEng\Nas\PAUSE.CPP`, from
  `0x80076318` (right after `Init_Panel`) to `0x8007A07C`: the whole file's
  code, but no function records for it. `PANEL.CPP` has no line records.

**PANEL.CPP in retail.** All 23 below are renamed (with Euro Demo 65's
engine names: `renames_engine_all.txt`). `Setup_panel` is `func_8007AC20`, the first function
of the engine unit `8007ac20`, so that unit is `PANEL.CPP`. All 23 retail
functions are already matched; the names are for readability.

| Demo name | Retail | Similarity |
|---|---|---|
| `Setup_panel__Fv` | `func_8007AC20` | 0.82 |
| `InitBigScore__Fv` | `func_8007B120` | 0.95 |
| `UpdateBigScore__Fv` | `func_8007B330` | 0.99 |
| `Set_Clock__FUsUs` | `func_8007B7FC` | 1.00 |
| `Draw_Clock__Fv` | `func_8007B8D4` | 1.00 |
| `InitHealthIcons__Fv` | `func_8007C338` | 1.00 |
| `ClearHealthIcons__Fv` | `func_8007C514` | 1.00 |
| `InitBossHealthIcons__Fv` | `func_8007C6EC` | 1.00 |
| `UpdateHealthIcons__Fv` | `func_8007CA70` | 1.00 |
| `InitPowerIcons__Fv` | `func_8007CC80` | 1.00 |
| `UpdatePowerIcons__Fv` | `func_8007CE50` | 0.78 |
| `InitScore3Icons__Fv` | `func_8007CFAC` | 1.00 (three identical; by order) |
| `InitScore2Icons__Fv` | `func_8007D034` | 1.00 (by order) |
| `InitScore1Icons__Fv` | `func_8007D0BC` | 1.00 (by order) |
| `InitSharedScoreIcons__Fv` | `func_8007D144` | 1.00 |
| `UpdateScoreIcons__Fv` | `func_8007D4D0` | 1.00 |
| `Cups_Update__Fv` | `func_8007D9DC` | 0.92 |
| `StillTottingUpScore__Fv` | `func_8007E894` | 1.00 |
| `MakeSubBar__FP9subenginessUiUi` | `func_8007E8D0` | 1.00 |
| `Clock_Off__Fv` | `func_8007EB04` | 1.00 |
| `Init_Clock__Fiii` | `func_8007EB1C` | 1.00 |
| `InitCountdown__Fv` | `func_8007EB68` | 0.95 |
| `Init_Panel__Fv` | `func_8007EC48` | 0.98 |

Not placed (the code changed after the demo): `InitIcons`, `InitSplatIcons`
and `InitBorderIcons` are probably `func_8007BE84`, `func_8007BFA8` and
`func_8007C170` by position (similarity 0.51); `InitBossIcon`, `FetchWinner`,
`Cups_Init` and `Draw_Panel` have no good match. Retail-only functions in the
unit: `func_8007AF3C`, `C830`, `D208`, `D2F4`, `D5EC`, `D6D0`, `D850`,
`E7A4`, `E7C8`, `E7F4`, `E874`, `E93C`, `E9FC`, `EAC0`, `ED0C`. Euro Demo 65
names `E7C8`: `CalcElapsedTime`.

The last functions in the file (from `Cups_Init` on in the demo) come in
reverse line order; the ones placed in retail keep that order.

**PAUSE.CPP.** The demo's pause code differs a lot from retail (best
similarity 0.40). Its largest function (`0x1238` bytes) is closest to
`func_8007F314`, the engine pause menu (0.28), so `PAUSE.CPP` is probably the
unit `8007ee2c`, maybe from `func_8007ED0C`. Not proven.

**Still to do.**
- Apply the signatures and the locals to the 23 renamed functions, and
  rename the unit `panel`.
- Globals: the 33 mapped (of 44; every one referenced at its start and on a
  retail `D_` start) are renamed with Euro's engine ones
  (`renames_engine_globals.txt`: `PanelCtrl` is `D_80099174`, `CupTimer`
  `D_8009AE3C`, `HealthIconPos` `D_80099250`, ...). The shipped data sits
  0x28 below the records, though the code is 4 above. Their types (sizes,
  arrays) are still to apply.
- Add the new enums to `include/bash/` (`shell.h` for the cheat flags and
  level/world numbers).
- Place `PAUSE.CPP` in retail from its line table (statement boundaries
  against retail's code).

## Euro Demo 65

`protos/Euro_Demo_65/scan.md` has the full report, with every label and its
match.

**The disc.** A PAL multi-game demo, `SCED-02639`, dated 2000-10-09; Crash
Bash is `CRASHBSH/CRASH.EXE` (load `0x80018000`) and a 44 MB
`CRASHBSH.DAT`. No `.SYM`, `.MAP` or source text on the disc.

**Where the fragment is.** The exe's DAT index is at `0x8004CA38` (630
entries). The engine is entry `0x275`, loaded at `0x8007A154` (the
fragment's own `_engine_start_obj` says the same); 13 slot overlays (entries
`0x174..0x180`) load at `0x800B0CCC`. The records sit in the engine entry at
`+0x332CB` (DAT `0x2A272CB..0x2A4C22F`). Their addresses fit the shipped exe
and the first part of the engine as they are; from `0x800947F4` on and in
the slot overlays they are `0x4C` below the
shipped code, so some function before there changed size between the two
links.

**What it holds.**
- 1182 labels: the SN symbol table, a name and an address each, no types.
  - 371 functions and globals: 140 with C++ mangled names (so their
    parameter types), the rest C names (PsyQ functions and globals) and
    game globals.
  - 811 section symbols (`_<group>[_<section>]_org/orgend/obj/objend/size`),
    which give the link layout: `_core` is the exe, `_engine_start` to
    `_engine_end` the engine module (`0x8007A154..0x800D2B60`), and two
    overlay slots inside it: `_WORLD_OVERLAY` (`0x800B0C84`) for the games,
    `BASH`, `PAPU`, `TANK`, `POGO`, `ARKENOID`, `POLAR`, `RACE`, `SPIN`,
    `DINO`, `BONK`, `N2O`, `KABOOM`, `HUB` and `TITLES` (the AST enums'
    names, with `KABOOM`, Keg Kaboom, for the enums' `CHASE`), and
    `_RULES_OVERLAY` (`0x800AD3C0`) for each game's rules text in each
    language, `_<game>_<lang>` (`_TA_GE` is Tank in German; EN, FR, GE, IT,
    SP, and one `_BA_JP`).
- 2325 type records, 151 names; about half are new to us, nearly all map
  enums of single levels (`*_TA_MAPS` for each Tank arena, `PG_A_MAPS` ...
  for Pogo, `MJ_*`, `TBOSS_*`, ...), plus `AIactionTypesTA`, `geo_entity`
  and `sub_shape_info`.

**Matched to retail.** 163 labels match a retail function at 0.75 or more
(149 at 0.9 or more):

| Module | Matched (0.9+ / 0.75+) | Examples |
|---|---|---|
| `main.exe` | 116 / 124 | `mal_update`, `mal_free`, ... (the allocator), `init_file`, `load_file_permanent`, `set_joypad`, `cam_init`, `interp`, `BSwapEndian`, `map_entity_address`, `init_dots`, `dbase_asload_with_anims_texture_callback`, `get_script_header`, `texture_draw`, PsyQ functions |
| engine | 22 / 27 | `InitialiseNasSystem` (`func_80078CA4`, the engine's first function), `GravitateNas`, `DoAmbience`, `CalcElapsedTime` (`func_8007E7C8`, in `PANEL.CPP`), `DefHero2ObjectCollision`, `HeroGhost`, `PrintNasPos` |
| slot overlays | 11 / 12 | `UpdatePAPU`, `PAHeroHit`, `SetPAHeroCarry` (`ovl5e`), `DoGetBomb`, `TestBeamChance` (`ovl62`), `PlotTotemBit`, `TankBossCortexTurretFire` (`ovl5f`), `MaintainCentre` (`ovl63`), `team_mode_init`, `adventure_mode_head` (`ovl69`) |

Checks: the `PANEL.CPP` names agree with the Jampack mapping
(`InitBossHealthIcons` is `func_8007C6EC` in both), and names the repo got
from the leaks agree (`InitSmokePoof`, `PlotSmokePoof`; `TA_HeroBot` and
`InitialisePOLARFirst` are the best, if weak, matches of their labels). Some
names the repo gave itself ("Name 15 core engine functions") had their
originals here: `SpawnNASById` was `CreateNAS`, `SpawnNASFromRecord`
`TriggerNas`, `AddEngine` `AddExtEngine`, `AddExtension` `AddExt` (all four
renamed), and `InitHealth`/`PlotHealth` are `Init_HealthPU`/`Plot_HealthPU`
(engine, not yet).

**Applied.** 78 `main.exe` placeholder functions are renamed: the 73
matches of 0.9 and up (`renames_main.txt`; `mal_update`, `init_file`,
`cam_init`, `dbase_*`, `map_*`, `music_*`, ...) and 5 of 0.75..0.9 checked
by hand (`renames_main_checked.txt`: `mal_free`, `path_get`,
`get_script_header`, `new_sprim`, `texture_draw`; each sits in link order
between confirmed renames, and four are the same distance from the next
one in both builds). One needed a source fix first: `ovl60` named
`0x8001C378` as data (`D_8001C378`), so it now uses the function's name.
The 4 names the repo gave itself are replaced by the originals
(`renames_main_replace.txt`: `CreateNAS`, `TriggerNas`, `AddExtEngine`,
`AddExt`).

**Still to do.**
- Engine names done: 37 with Jampack's (`renames_engine_all.txt`); left are
  `SetTweakAi` (0.84), `HeroGhost` (0.86) and `line_intersect_zy` (0.78), in
  link order but with no gap to confirm them, and four stubs no place
  confirms (`DefaultTouched`, `DefHero2ObjectCollision`, `PrintNasPos`,
  `InitParticleEffect`). Slot overlay names next.
- The parameter types from the C++ names, one prototype at a time.
- The rest of the globals (`globals.txt`, 41 mapped). `main.exe`'s 13 that
  land on a retail `D_` symbol's start are renamed (`renames_main_globals.txt`:
  `screen_aspect`, `FirstNas`, `World_floor_level`, `num_sfx_banks`, `cdb`,
  ...); 5 sit inside one (`core_process`, `map_frame_counter`,
  `GlobalNAStatus`, `GemRules`, `ASTString`) and need the symbol split first;
  15 are PsyQ data (Sony's names stay). The engine's are renamed with
  Jampack's (36 in all: also `map`, `camera_frame_list_size`,
  `camera_frame_right`); `default_shell_info` and the two overlay ones
  (`PolarAITeeterChance`, `RACEQualityFactor4t`) sit inside a symbol and wait
  with the 5 above. The two globals both demos map agree
  (`HealthIconPos`, `BigScore_teamA`).
- The link groups give file boundaries in the engine (`_core_end`,
  `_engine_start`, ...); check them against the unit split in
  ARCHITECTURE.md.

## Discs not scanned yet

None known. The PAL and Japanese releases are clean, so the best chances
are other pre-release builds: prototypes, or demo discs carrying a Crash
Bash build not seen yet (the scan says when a disc's files match one
already scanned).
