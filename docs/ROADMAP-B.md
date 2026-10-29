# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the roadmap badge updates.

## Part 1 - Decisions

| # | Task | Notes | Done |
|---|------|-------|------|
| 1.1 | Memory images: may a source build write zeros where the engine's and slot's .bss hold build-machine leftovers? | The shift builds already zero the slot's and play. The exe's .bss is in its file too (ARCHITECTURE.md, The exe's .bss). | [ ] |
| 1.2 | Modern GCC build: drop it, or finish it? | Shelved 2026-10-29. Boots to a black screen: libsnd's `.bss` order differs from retail's (AGENT_LOG, Open leads). A PC port replaces PsyQ anyway. | [ ] |

## Part 2 - Typing (quality level 4)

| # | Task | Notes | Done |
|---|------|-------|------|
| 2.1 | Type the 113 untyped game functions | `decomp.py progress` | [ ] |
| 2.2 | Fold the overlays' 136 views of engine globals into one type each | `decl_dups.py --views`, `fold_views.py`. Includes the `HeroHooks` and game-state views and Phase 4's stand-ins (`PogoCell`, `BashCell`, `RaceFux*`, ...). | [ ] |
| 2.3 | The exe's 69 raw-offset functions | Mostly heap and pool block arithmetic, and `export_exe_functions`' slot offsets | [ ] |
| 2.4 | Name the `unkXX` fields | Including the file formats' (ASSETS.md, What isn't decoded) | [ ] |
| 2.5 | Structs for Spin, Dino and N2O | Enums only so far; `field_survey.py` | [ ] |
| 2.6 | The prototype types' 34 lost-page fields | Word or byte arrays now; retype where code shows the fields | [ ] |
| 2.7 | `func_800115D8` takes `_mal_area *` | Not `void **`, once its callers allow | [ ] |

## Part 3 - Names and loose ends

| # | Task | Notes | Done |
|---|------|-------|------|
| 3.1 | Natural forms of the three steering matches | `func_800B4048`, `func_800BE914`, `PlotTransmuterBeam` (AGENT_LOG, Open leads) | [ ] |
| 3.2 | The demo discs' last symbols | `cam_init`, 11 more, classes with no stand-in, `ProcessPauseUD` (PROTOTYPES.md, Left) | [ ] |
| 3.3 | The ambiguous names | ARCHITECTURE.md, Functions reached from the AST pairs | [ ] |
| 3.4 | Did both gcc 2.7.2 units use `-fno-rerun-cse-after-loop`? | `main/modchip`, `main/modchip_msg` | [ ] |
| 3.5 | Sony's 131 unnamed statics | The converted objects carry no names | [ ] |

## Part 4 - Documentation

| # | Task | Notes | Done |
|---|------|-------|------|
| 4.1 | ARCHITECTURE.md describes the game's systems | | [ ] |

## Part 5 - The final test

The decomp is done when all five pass.

| # | Test | Proves | Status | Done |
|---|------|--------|--------|------|
| 5.1 | Clean rebuild: a fresh clone, the user's disc and `make check` match every binary; the packed disc matches retail | Nothing outside the repo is needed | Passes in the working checkout; not run from a fresh clone | [ ] |
| 5.2 | Nothing hidden: no asm, `INCLUDE_RODATA`, `.incbin`, parked function or `func_`/`D_` name | Everything is source | Waits on 1.1 and Part 2 | [ ] |
| 5.3 | The fully shifted build plays every minigame through to a win | No address is hard-coded | `make shift-all` plays (2026-10-29); not every minigame to a win yet | [ ] |
| 5.4 | Same game: the shifted build, side by side with retail over the same route, behaves the same | It behaves like the original | Not run | [ ] |
| 5.5 | A modern compiler builds it and passes 5.4 | The C is correct C | Waits on 1.2 | [ ] |

## Part 6 - Release

Before making the repo public.

| # | Task | Where | Notes | Done |
|---|------|-------|-------|------|
| 6.1 | Add a licence for our own work | `LICENSE` | MIT (or CC0) for the tools, scripts, Makefile and docs; not the game's code or data. | [ ] |
| 6.2 | Say what the licence covers | `README.md` | Our tooling only. The decompiled C and data reconstruct the copyrighted game (Eurocom / Sony). `tools/reference/` and the `tools/cc1/` patches stay GPL. | [ ] |
| 6.3 | Preservation and research notice | `README.md` | No game files included; you need your own retail copy to build. | [ ] |
| 6.4 | Getting-started steps | `README.md` | Your disc in `disc/`, `tools/get_psyq.sh`, then `tools/dk make` / `make check`. | [ ] |
| 6.5 | Credits | `README.md` | splat, maspsx, m2c, decomp-permuter, objdiff, the PsyQ conversion, GCC. | [ ] |
| 6.6 | Remove the local RetroArch path | `CLAUDE.md` | A generic launch line, or a pointer to `docs/PLAYING.md`. | [ ] |
| 6.7 | Tidy the agent log | `docs/AGENT_LOG.md` | Trim the Session log, or say in the README that it's the working log. | [ ] |
| 6.8 | Commit the progress history | `.decomp/history.json` | With each docs update, so it stops showing as modified. | [ ] |
| 6.9 | Re-read the leak and prototype docs | `docs/PROTOTYPES.md`, `docs/ARCHITECTURE.md` | Describing the sources is fine; nothing may quote the leaked files at length. | [ ] |
| 6.10 | Decide on `tools/reference/` (26 MB GCC source) | `tools/reference/` | Keep it (GPL, COPYING present) or replace it with a fetch script. | [ ] |
| 6.11 | Final check | repo root | `git status` clean; `.gitignore` lists `disc/`, `protos/`, `asm/`, `assets/`, `ext/`, `build/`, `linkers/`; no binaries in the history. | [ ] |
| 6.12 | Switch the GitHub repo to public | GitHub settings | After 6.1 to 6.11. | [ ] |
| 6.13 | List the project on recomp.fyi | `.recomp.json` | At least `"game": "Crash Bash"`, `"system": "PS1"`, `"type": "decomp"`. Under 8 KB; never mention a ROM, an ISO or where to get one. Listed daily, or at once through https://recomp.fyi/add. | [ ] |

## Part 7 - Afterwards

Planned once the final test passes.

| # | Task | Notes | Done |
|---|------|-------|------|
| 7.1 | Native PC port | Assets from the extracted files; PC replacements for the CD, GPU and GTE, SPU and pads | [ ] |
| 7.2 | PsyQ as C | C that compiles to Sony's objects; the PC port replaces most of it, so decide after 7.1 | [ ] |
