# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the roadmap badge updates.

## Part 1 - Typing (quality level 4)

| # | Task | Notes | Done |
|---|------|-------|------|
| 1.3 | The exe's 69 raw-offset functions | Mostly heap and pool block arithmetic, and `export_exe_functions`' slot offsets | [ ] |
| 1.4 | Name the `unkXX` fields | Including the file formats' (ASSETS.md, What isn't decoded) | [ ] |
| 1.5 | Structs for Spin, Dino and N2O | Enums only so far; `field_survey.py` | [ ] |
| 1.6 | The prototype types' 34 lost-page fields | Word or byte arrays now; retype where code shows the fields | [ ] |
| 1.7 | `func_800115D8` takes `_mal_area *` | Not `void **`, once its callers allow | [ ] |

## Part 2 - Names and loose ends

| # | Task | Notes | Done |
|---|------|-------|------|
| 2.1 | Natural forms of the three steering matches | `func_800B4048`, `func_800BE914`, `PlotTransmuterBeam` (AGENT_LOG, Open leads) | [ ] |
| 2.2 | The demo discs' last symbols | `cam_init`, 11 more, classes with no stand-in, `ProcessPauseUD` (PROTOTYPES.md, Left) | [ ] |
| 2.3 | The ambiguous names | ARCHITECTURE.md, Functions reached from the AST pairs | [ ] |
| 2.4 | Did both gcc 2.7.2 units use `-fno-rerun-cse-after-loop`? | `main/modchip`, `main/modchip_msg` | [ ] |
| 2.5 | Source builds write zeros in the engine's .bss (decided 2026-10-29) | Check the game writes each leftover word before reading it, have `shift_all.py` zero it as it does the slot, and playtest. The matching build keeps reading it from the disc. | [ ] |

## Part 3 - Documentation

| # | Task | Notes | Done |
|---|------|-------|------|
| 3.1 | ARCHITECTURE.md describes the game's systems | | [ ] |

## Part 4 - The final test

The decomp is done when all four pass. The modern-compiler test moved to the PC port (6.1).

| # | Test | Proves | Status | Done |
|---|------|--------|--------|------|
| 4.1 | Clean rebuild: a fresh clone, the user's disc and `make check` match every binary; the packed disc matches retail | Nothing outside the repo is needed | Passes in the working checkout; not run from a fresh clone | [ ] |
| 4.2 | Nothing hidden: no asm, `INCLUDE_RODATA`, `.incbin`, parked function or `func_`/`D_` name | Everything is source | Waits on Part 1 and 2.5 | [ ] |
| 4.3 | The fully shifted build plays every minigame through to a win | No address is hard-coded | `make shift-all` plays (2026-10-29); not every minigame to a win yet | [ ] |
| 4.4 | Same game: the shifted build, side by side with retail over the same route, behaves the same | It behaves like the original | Not run | [ ] |

## Part 5 - Release

Before making the repo public.

| # | Task | Where | Notes | Done |
|---|------|-------|-------|------|
| 5.1 | Add a licence for our own work | `LICENSE` | MIT (or CC0) for the tools, scripts, Makefile and docs; not the game's code or data. | [ ] |
| 5.2 | Say what the licence covers | `README.md` | Our tooling only. The decompiled C and data reconstruct the copyrighted game (Eurocom / Sony). `tools/reference/` and the `tools/cc1/` patches stay GPL. | [ ] |
| 5.3 | Preservation and research notice | `README.md` | No game files included; you need your own retail copy to build. | [ ] |
| 5.4 | Getting-started steps | `README.md` | Your disc in `disc/`, `tools/get_psyq.sh`, then `tools/dk make` / `make check`. | [ ] |
| 5.5 | Credits | `README.md` | splat, maspsx, m2c, decomp-permuter, objdiff, the PsyQ conversion, GCC. | [ ] |
| 5.6 | Remove the local RetroArch path | `CLAUDE.md` | A generic launch line, or a pointer to `docs/PLAYING.md`. | [ ] |
| 5.7 | Tidy the agent log | `docs/AGENT_LOG.md` | Trim the Session log, or say in the README that it's the working log. | [ ] |
| 5.8 | Commit the progress history | `.decomp/history.json` | With each docs update, so it stops showing as modified. | [ ] |
| 5.9 | Re-read the leak and prototype docs | `docs/PROTOTYPES.md`, `docs/ARCHITECTURE.md` | Describing the sources is fine; nothing may quote the leaked files at length. | [ ] |
| 5.10 | Decide on `tools/reference/` (26 MB GCC source) | `tools/reference/` | Keep it (GPL, COPYING present) or replace it with a fetch script. | [ ] |
| 5.11 | Final check | repo root | `git status` clean; `.gitignore` lists `disc/`, `protos/`, `asm/`, `assets/`, `ext/`, `build/`, `linkers/`; no binaries in the history. | [ ] |
| 5.12 | Switch the GitHub repo to public | GitHub settings | After 5.1 to 5.11. | [ ] |
| 5.13 | List the project on recomp.fyi | `.recomp.json` | At least `"game": "Crash Bash"`, `"system": "PS1"`, `"type": "decomp"`. Under 8 KB; never mention a ROM, an ISO or where to get one. Listed daily, or at once through https://recomp.fyi/add. | [ ] |

## Part 6 - The PC port

After the final test. PsyQ stays linked from Sony's objects in the PS1 build (decided
2026-10-29): much of it was hand-written asm, and the port replaces it. A PS1 build with a modern
compiler is dropped (decided 2026-10-29): the port compiles the same C with one and finds the
same bugs.

| # | Task | Notes | Done |
|---|------|-------|------|
| 6.1 | Build the game's C for PC with a current GCC or Clang | Proves the C is correct C, not C that only works with the old compiler. Fix what it exposes in the shared C (uninitialised reads, out-of-bounds reads, pointers kept in 32-bit numbers); the matching build must stay `OK`. | [ ] |
| 6.2 | Replace the PS1 layers | PC versions of the CD, GPU and GTE, SPU and pads; assets from the extracted files | [ ] |
| 6.3 | Libsnd's sequencer as C | Plain non-matching C (and any GTE maths needed bit for bit), checked against the linked originals | [ ] |
| 6.4 | Same game | Played side by side with retail over 4.4's route, it behaves the same | [ ] |
