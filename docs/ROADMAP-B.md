# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the dashboard updates.

## Part 6 - The PC port

A native 64-bit port for Windows, Linux, macOS and Android, with modern tooling and no emulation tricks
(decided 2026-10-30). The design and the rules are in [PORT.md](PORT.md). PsyQ stays linked from Sony's
objects in the PS1 build. Every change to `src/` or `include/` has to pass `make check`.

| # | Task | Notes | Done |
|---|------|-------|------|
| 6.1 | Foundations | `port/` with CMake presets, Clang on x86-64 Windows and Linux, SDL3, CTest and CI. Headers for the PC versions of PsyQ. The game's C compiles as 64-bit, and the compiler's warnings become the list for 6.2 | [ ] |
| 6.2 | Addresses and calls | A type for addresses kept in integers, in the code and the data tables. Prototypes for every function, and the calls that rely on the caller's registers say what they pass. The truncation warnings reach zero and become errors | [ ] |
| 6.3 | Real structs | Reads through other globals, past an object's end and by byte offset become real structs, arrays and fields. Struct size checks apply to the PS1 only | [ ] |
| 6.4 | Files and memory | The pointer slots in loaded files, the mode text's addresses, the heap and pools, the primitives' 16 MB block, and the memory card's format on disk | [ ] |
| 6.5 | It runs, headless | CD on the user's own data, the GTE, pads, the card and overlays, with no window. All 33 modes play to the end, optimised, and clean under ASan and UBSan | [ ] |
| 6.6 | The renderer | SDL3's GPU API, with the PS1's VRAM as a texture and higher resolutions | [ ] |
| 6.7 | Sound | The SPU and libsnd in C, sequencer and reverb included, checked against Sony's linked code in a MIPS interpreter | [ ] |
| 6.8 | Playing it | The window, keyboard, controllers with analog sticks and rumble, saving, and a first release for Windows and Linux | [ ] |
| 6.9 | Same game | The same inputs on retail and on PC, with the game's state compared frame by frame over 4.4's route | [ ] |
| 6.10 | macOS and Android | Builds for both, ARM included. Android also needs touch controls, packaging, and the game's data from the user's own disc | [ ] |
