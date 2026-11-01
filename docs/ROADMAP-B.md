# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the dashboard updates.

## Part 6 - The PC port

A native 64-bit PC port: no emulation, no hacks, on the best tech stack for a game that plays (decided 2026-10-30). The
disc to disc work is byte matching; this isn't. The port has its own copy of the game's C and is tested by the user
playing it. The decomp stays as it is. The design and the rules are in [PORT.md](PORT.md).

| # | Task | Notes | Done |
|---|------|-------|------|
| 6.8 | Fully native | Take out the PS1-shaped layers: the game's C calls plain maths instead of GTE operations (`maths.c`), draws directly instead of building GPU packets (`bash/draw.h`), and uses the port's own input, sound and file APIs instead of Sony's library names. Sound keeps the hardware's interpolation and envelopes, as they're how the game sounds | [ ] |
