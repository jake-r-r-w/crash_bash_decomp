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

| # | Task | Notes | Done |
|---|------|-------|------|
| 6.2 | Replace the PS1 layers | PC versions of the CD, GPU and GTE, SPU and pads; assets from the extracted files | [ ] |
| 6.3 | Libsnd's sequencer as C | Plain non-matching C (and any GTE maths needed bit for bit), checked against the linked originals | [ ] |
| 6.4 | Same game | Played side by side with retail over 4.4's route, it behaves the same | [ ] |
| 6.5 | A 64-bit build | The addresses kept in integers and the structs holding pointers, fixed in the shared C (each change passing `make check`), with a translation step for files laid over structs. Required for macOS and Android. The 32-bit build is the reference. | [ ] |
| 6.6 | macOS and Android | Builds for both, ARM included. Android also needs touch controls, packaging, and the game's data from the user's own disc. | [ ] |
