# The port

A native 64-bit port for Windows, Linux and Android, built from its own copy of the game's C. The design, the rules and how to
build, run and test it are in [docs/PORT.md](../docs/PORT.md), the steps taken are in [docs/ROADMAP-A.md](../docs/ROADMAP-A.md), and
what's next is in [docs/ROADMAP-B.md](../docs/ROADMAP-B.md).

| Path | What |
|---|---|
| `game/` | The copy of the game's C, not under this project's licence ([LICENSE](../LICENSE)), with the port's own additions to it |
| `platform/` | What the game runs on: the window and input (`sdl.c`, `pad.c`, `touch.c`), the renderer (`gfx.c`, Direct3D 12 and Vulkan), sound (`audio.c`), the disc image and archive, saves and settings, mods and texture packs, and online play (`net.c`, `portmap.c`) |
| `main.c` | The entry point and the command line's options |
| `tests/` | The play tests (`playtest.mjs`), the computer players they use, and online play's packet fuzzer (`netfuzz.c`) |
| `android/` | Android's app around the game, and its build (`build.sh`, in `tools/da`) |
| `mods/` | The BPS patch maker the mod tests use, and an example mod made with it |
| `release/` | The README a release comes with, and the first release's notes |
| `package.mjs` | Makes a release's download, for Windows, Linux or Android ([docs/PORT.md](../docs/PORT.md), A release) |
| `CMakeLists.txt`, `CMakePresets.json` | The build: the `windows-clang`, `linux-clang`, `linux-gcc` and `linux-asan` presets, and `netfuzz` |
