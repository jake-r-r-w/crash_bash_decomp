# The PC port

A native 64-bit port for Windows and Linux, built from its own copy of the game's C. The design, the rules and how to
build it are in [docs/PORT.md](../docs/PORT.md), and the steps are in [docs/ROADMAP-B.md](../docs/ROADMAP-B.md).

| Path | What |
|---|---|
| `game/` | The copy of the game's C, not under this project's licence ([LICENSE](../LICENSE)) |
| `psyq/` | PsyQ's names, so the game compiles: scaffolding, which goes as they're replaced |
| `main.c` | The entry point |
