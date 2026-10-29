# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the roadmap badge updates.

## Part 4 - The final test

The decomp is done when all four pass (4.1 to 4.3 passed 2026-10-29: ROADMAP-A, Part 4). The
modern-compiler test moved to the PC port (6.1).

| # | Test | Proves | Status | Done |
|---|------|--------|--------|------|
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
| 5.11 | The toolchain image builds from scratch | `tools/docker/Dockerfile` | 4.1 reused the existing image. Build it with `--no-cache` on a machine without it, then `make check`. | [ ] |
| 5.12 | Quiet `make split`'s three splat notices | `configs/USA/sym.main.txt` | "Unable to determine a segment" for `funcEvSpError`, `funcEvSpErrorx`, `MsgFatalMetricsError` (seen in 4.1): give them a `segment:` or `absolute:True`, so a new user's first split prints no error. | [ ] |
| 5.13 | Final check | repo root | `git status` clean; `.gitignore` lists `disc/`, `protos/`, `asm/`, `assets/`, `ext/`, `build/`, `linkers/`; no binaries in the history. | [ ] |
| 5.14 | Switch the GitHub repo to public | GitHub settings | After 5.1 to 5.13. | [ ] |
| 5.15 | List the project on recomp.fyi | `.recomp.json` | At least `"game": "Crash Bash"`, `"system": "PS1"`, `"type": "decomp"`. Under 8 KB; never mention a ROM, an ISO or where to get one. Listed daily, or at once through https://recomp.fyi/add. | [ ] |

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
