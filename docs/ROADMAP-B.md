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
| 6.5 | It plays | The game linked and booting from the user's own data, drawn through SDL3's GPU API with no PS1 drawing commands or VRAM, on keyboard and controller. From here the user plays it and reports what's wrong, and the fixes follow | [ ] |
| 6.6 | Native sound | The game's sound banks and music played by its own mixer and sequencer on SDL3, with reverb | [ ] |
| 6.7 | Finishing it | Saving, any resolution, analog sticks and rumble, then a first release | [ ] |
