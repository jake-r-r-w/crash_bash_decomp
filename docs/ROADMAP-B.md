# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the dashboard updates.

## Part 6 - The PC port

A native 64-bit PC port for Windows and Linux: no emulation, no hacks, on a modern stack (decided
2026-10-30). The port has its own copy of the game's C and doesn't have to match retail's bytes, but it has to
play the same. The decomp stays as it is. The design and the rules are in [PORT.md](PORT.md).

| # | Task | Notes | Done |
|---|------|-------|------|
| 6.1 | Foundations | `port/game/`, the copy of the game's C. `port/` with CMake presets, Clang on x86-64 Windows and Linux, SDL3, CTest and CI. The game's C compiles as 64-bit, and the compiler's warnings become the list for 6.3 | [ ] |
| 6.2 | Retail recordings | Scripted inputs played on retail in an emulator, with the game's state recorded frame by frame over every mode. Kept on the machine with the disc, never in git. This is the port's safety net, so it comes before any refactoring | [ ] |
| 6.3 | Addresses and calls | Addresses kept in integers become real pointers, every function gets a prototype, and calls that relied on the caller's registers pass their arguments. The truncation warnings reach zero and become errors | [ ] |
| 6.4 | Real structs | Reads through other globals, past an object's end and by byte offset become real structs, arrays and fields. Code written only to steer the old compiler goes back to natural C | [ ] |
| 6.5 | Files and memory | The pointer slots in loaded files, the mode text's addresses, the heap and pools, and the save format on disk | [ ] |
| 6.6 | It runs, headless | Files from the user's own data, the game's maths, pads, saves and overlays, with no window. All 33 modes play to the end, optimised, clean under ASan and UBSan, and matching the retail recordings | [ ] |
| 6.7 | A native renderer | Models, maps, sprites and text drawn directly through SDL3's GPU API, replacing the PS1 drawing code: real 3D, real textures and any resolution, with no PS1 drawing commands or VRAM | [ ] |
| 6.8 | Native sound | The game's sound banks and music played by its own mixer and sequencer on SDL3, with reverb. Note timing, pitch and volume checked against Sony's linked code | [ ] |
| 6.9 | Playing it | The window, keyboard, controllers with analog sticks and rumble, and saving. Played side by side with retail over 4.4's route, then a first release for Windows and Linux | [ ] |
