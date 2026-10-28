# Before making the repo public

| # | Task | Where | Notes | Done |
|---|------|-------|-------|------|
| 1 | Add a licence for our own work | `LICENSE` | MIT (or CC0) for the tools, scripts, Makefile and docs. It does not cover the game's code or data. | [ ] |
| 2 | Say what the licence covers | `README.md` | The licence covers our tooling only. The decompiled C and data reconstruct the copyrighted game (Eurocom / Sony). `tools/reference/` and `tools/cc1/` patches stay GPL. | [ ] |
| 3 | Preservation and research notice | `README.md` | The project is for preservation and research. It contains no game files, and you need your own retail copy of Crash Bash to build it. | [ ] |
| 4 | Getting-started steps | `README.md` | Put your own disc in `disc/`, run `tools/get_psyq.sh`, then build with `tools/dk make` / `make check`. | [ ] |
| 5 | Credits | `README.md` | splat, maspsx, m2c, decomp-permuter, objdiff, the PsyQ conversion, GCC. | [ ] |
| 6 | Remove the local RetroArch path | `CLAUDE.md` | Replace `C:/RetroArch-Win64/...` with a generic launch line or a pointer to `docs/PLAYING.md`. | [ ] |
| 7 | Tidy the agent log | `docs/AGENT_LOG.md` | Trim the Session log to recent entries, or note in the README that it is the working log. | [ ] |
| 8 | Commit the progress history | `.decomp/history.json` | Commit it with each docs update so it stops showing as modified. | [ ] |
| 9 | Re-read the leak and prototype docs | `docs/PROTOTYPES.md`, `docs/ARCHITECTURE.md` (Leaked source) | Describing the sources is fine; make sure nothing quotes the leaked files at length. | [ ] |
| 10 | Decide on `tools/reference/` (26 MB GCC source) | `tools/reference/` | Keep it (GPL, its COPYING files are present) or replace it with a fetch script for smaller clones. | [ ] |
| 11 | Final check | repo root | Run `git status` for nothing untracked; check that `.gitignore` still lists `disc/`, `protos/`, `asm/`, `assets/`, `ext/`, `build/`, `linkers/`; check the history again for binaries. | [ ] |
| 12 | Switch the GitHub repo to public | GitHub settings | After 1 to 11. | [ ] |
| 13 | List the project on recomp.fyi | `.recomp.json` (repo root) | Add a `.recomp.json` with at least `"game": "Crash Bash"`, `"system": "PS1"`, `"type": "decomp"` (optional: `status`, `toolchain`, `notes`, `links`, ...). Keep it under 8 KB and never mention a ROM, an ISO or where to get one, or it is refused. The board reads it daily; to be listed at once, submit the repo through the manifest form at https://recomp.fyi/add. | [ ] |
