# Prototype and demo discs

What each disc other than our retail target has given the decomp. The images
live in `protos/` (git-ignored); only the names and types derived from them
are committed.

Each separately mastered disc is a fresh chance of a leak: the build tools
padded files with whatever was in the build machine's memory, and that
memory sometimes held source text or pieces of the SN linker's `.SYM`
output. Every leak found so far, retail's included, sits inside the engine
module's `CRASHBSH.DAT` entry.

## Scanning a disc

1. Extract the archive on the host with 7-Zip into its own folder,
   `protos/<disc>/` (the image and its `.cue`).
2. Run the scan; it dumps the image to `files/` first if there is no
   `files/` folder yet (`make build` must have run, for the retail maps):

   ```bash
   tools/dk python3 -I tools/scripts/proto_scan.py protos/<disc>/
   ```

3. Read `protos/<disc>/scan.md` and add the disc to the tables below.

`proto_scan.py` finds the Crash Bash exe by its DAT index, the engine module
and its load address, runs `sym_scan.py` (debug records) and `leak_scan.py`
(source text) over the exe and the DAT, and matches each function record to
the retail function with the most similar code. It writes everything next
to the disc:

| File | What |
|---|---|
| `scan.md` | the report: where each record run is, source files, functions matched to retail, globals, text leaks |
| `sym.txt` | every record, with its file offset |
| `types.h` | the types as C |
| `lines.txt` | the line table, `address file:line` |
| `engine_<id>.bin` | the engine module cut from the DAT |
| `leaks/` | each source-text hit |

## At a glance

| Disc | Build | Leak | What it gave |
|---|---|---|---|
| Retail `SCUS_945.70` | release | 40 KB of preprocessed headers (engine entry `+0x54000`) | ~400 prototypes, the AST enums and the Tank/Pogo/Arkenoid/Polar structs (`bash/tank.h`, `pogo.h`, `arkenoid.h`, `polar.h`) |
| Aug 23 2000 prototype (Hidden Palace) | early | engine entry `+0x4E000`: an SN object for `PSX_PAD.CPP`, its ~90-header include list, 788 type records | source directory and header names (`Core\Shell`, `GameEng\Titles`, ...) |
| Sep 14 2000 prototype (Hidden Palace) | early | none | exe comparison only |
| Oct 9 2000 prototype (Hidden Palace; PAL review build `SCES_028.34`) | close to retail | engine entry `+0x40002`: 3447 type records | the engine's own types (`NAS`, `EXTengine`, `subengine`, `HeroInfo`, ...), every AST enum: names for 201 AST Init/Plot functions; the `include/bash/` game headers |
| Jampack Winter 2000 demo (`SCUS_946.24`) | older than Oct 9 | engine entry `+0x2BE15`: 4611 type records, **30 function records**, 44 globals, 1021 line records | unit `8007ac20` is `PANEL.CPP`; 23 function names mapped (not applied yet) |

The prototype exes have no source names, but by masked code signature they
hold the retail `main.exe` functions in retail order (Oct 9: 466 of 566,
Sep 14: 295, Aug 23: 251), so the link order didn't change before release.

## What kind of information came from where

| Information | Retail | Aug 23 | Oct 9 | Jampack |
|---|---|---|---|---|
| Function prototypes (Tank, Pogo, Arkenoid, Polar) | yes | | | |
| Minigame structs and AST enums | yes | some | yes | yes |
| Engine types (`NAS`, `EXTengine`, ...) | | | yes | |
| Source file and header paths | 4 game dirs | yes | | `PANEL.CPP`, `PAUSE.CPP` |
| Function names with addresses | | | | 30 (`PANEL.CPP`) |
| Parameter types (mangled names) | | | | 30 |
| Local variables and their registers | | | | yes |
| Global variable names, types and sizes | | | | 44 |
| Source line numbers | | | | `PAUSE.CPP`'s line table; each `PANEL.CPP` function's first line |
| New enums | | | | `cheatflags`, `levnumbers`, `worldnumbers`, `IconParts` |

Details for the retail, Aug 23 and Oct 9 leaks are in
[ARCHITECTURE.md](ARCHITECTURE.md) (Leaked source, Prototype debug types).

## Jampack Winter 2000 demo

`protos/Jampack_Winter_2000/scan.md` has the full report.

**The disc.** A US multi-game demo; Crash Bash is `CRASHBSH/CRASHBSH.EXE`
(load `0x80018000`) and a 41 MB `CRASHBSH.DAT`, both dated 2000-10-09. No
`.SYM`, `.MAP` or source text anywhere on the disc. The code is older than
the Oct 9 prototype even so: the AST ids are numbered before Race existed
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

**PANEL.CPP in retail.** `Setup_panel` is `func_8007AC20`, the first function
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
`E7A4`, `E7C8`, `E7F4`, `E874`, `E93C`, `E9FC`, `EAC0`, `ED0C`.

The last functions in the file (from `Cups_Init` on in the demo) come in
reverse line order; the ones placed in retail keep that order.

**PAUSE.CPP.** The demo's pause code differs a lot from retail (best
similarity 0.40). Its largest function (`0x1238` bytes) is closest to
`func_8007F314`, the engine pause menu (0.28), so `PAUSE.CPP` is probably the
unit `8007ee2c`, maybe from `func_8007ED0C`. Not proven.

**Still to do.**
- Apply the 23 names, the signatures and the locals to `8007ac20.c` and its
  header, and rename the unit `panel`.
- Map the 44 globals to retail addresses through the data references of the
  matched pairs.
- Add the new enums to `include/bash/` (`shell.h` for the cheat flags and
  level/world numbers).
- Place `PAUSE.CPP` in retail from its line table (statement boundaries
  against retail's code).

## Discs not scanned yet

| Disc | Why it might help |
|---|---|
| PlayStation Demo Disc v1.3 (US) | earliest public build (E3 2000 hubs); another link's leftovers |
| Official PlayStation Magazine UK Demo 65 | separate PAL mastering |
| PAL retail (`SCES-02834`) | separate mastering; a diff shows PAL-only code |
| Japanese retail (Crash Bandicoot Carnival) | later, separate mastering; localization changes |
