# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the dashboard updates.

## Part 6 - The PC port

A native 64-bit PC port for Windows and Linux: no emulation, no hacks, on a modern stack (decided
2026-10-30). The design and the rules are in [PORT.md](PORT.md). PsyQ stays linked from Sony's
objects in the PS1 build. Every change to `src/` or `include/` has to pass `make check`.

| # | Task | Notes | Done |
|---|------|-------|------|
| 6.1 | Foundations | `port/` with CMake presets, Clang on x86-64 Windows and Linux, SDL3, CTest and CI. Headers for the PC versions of PsyQ. The game's C compiles as 64-bit, and the compiler's warnings become the list for 6.2 | [ ] |
| 6.2 | Addresses and calls | A type for addresses kept in integers, in the code and the data tables. Prototypes for every function, and the calls that rely on the caller's registers say what they pass. The truncation warnings reach zero and become errors | [ ] |
| 6.3 | Real structs | Reads through other globals, past an object's end and by byte offset become real structs, arrays and fields. Struct size checks apply to the PS1 only | [ ] |
| 6.4 | Files and memory | The pointer slots in loaded files, the mode text's addresses, the heap and pools, and the save format on disk | [ ] |
| 6.5 | It runs, headless | Files from the user's own data, the game's maths, pads, saves and overlays, with no window. All 33 modes play to the end, optimised, and clean under ASan and UBSan | [ ] |
| 6.6 | A native renderer | Models, maps, sprites and text drawn directly through SDL3's GPU API, replacing the PS1 drawing code: real 3D, real textures and any resolution, with no PS1 drawing commands or VRAM | [ ] |
| 6.7 | Native sound | The game's sound banks and music played by its own mixer and sequencer on SDL3, with reverb. Note timing, pitch and volume checked against Sony's linked code | [ ] |
| 6.8 | Playing it | The window, keyboard, controllers with analog sticks and rumble, saving, and a first release for Windows and Linux | [ ] |
| 6.9 | Same game | The same inputs on retail and on PC, with the game's state compared frame by frame over 4.4's route. The game plays the same; only the picture and sound quality differ | [ ] |
