# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the dashboard updates.

## Part 6 - The PC port

After the final test. PsyQ stays linked from Sony's objects in the PS1 build (decided
2026-10-29): much of it was hand-written asm, and the port replaces it. A PS1 build with a modern
compiler is dropped (decided 2026-10-29): the port compiles the same C with one and finds the
same bugs. The layout and the rules are in [PORT.md](PORT.md). The port is 32-bit first (decided
2026-10-30): pointers fit the game's `s32`s, so it can run before those change. The targets are
Windows, Linux, macOS and Android.

The port uses modern tooling (decided 2026-10-30): current libraries and native approaches, not
emulator-style layers. Today's 32-bit build with its software GPU is the reference: it stays until 6.5
passes, each replacement is compared against it, and then the old version is deleted. The port stays in
C, and `src/` and `include/` still have to pass `make check`.

| # | Task | Notes | Done |
|---|------|-------|------|
| 6.3 | SDL3 | `port/platform` moves from SDL2 to SDL3: the window, sound and input | [ ] |
| 6.4 | Libsnd's sequencer as C | Plain non-matching C, checked against the linked originals: music, with libsnd's own volumes and pitches, and the SPU's reverb | [ ] |
| 6.5 | Same game | Played side by side with retail over 4.4's route, it behaves the same | [ ] |
| 6.6 | A 64-bit build, with no PS1 layout | The addresses kept in integers and the structs holding pointers, fixed in the shared C (each change passing `make check`), with a translation step for files laid over structs. Real structs replace reads past a global's size, so `data_order.txt`, the 16 MB limit and the null-page handler go. Then the game's C can be optimised instead of `-O0`, with ASan and UBSan builds. Required for macOS and Android. | [ ] |
| 6.7 | Tests and CI | CTest, and CI that builds the port on all four systems and plays the minigames | [ ] |
| 6.8 | A real GPU renderer | SDL3's GPU API (Vulkan, Direct3D 12, Metal), with higher resolutions, in place of the software GPU. The game's C still writes PS1 drawing commands, because it has to match retail, so the renderer translates them, and has to handle the VRAM tricks such as animated textures | [ ] |
| 6.9 | macOS and Android | Builds for both, ARM included. Android also needs touch controls, packaging, and the game's data from the user's own disc. | [ ] |
