# crash_bash_decomp

Crash Bash (USA, `SCUS_945.70`) for the PlayStation, decompiled to C. It gives you two things:

- **A PC port**: a native 64-bit Windows game with widescreen, upscaling, controller support and texture packs. Not an emulator.
- **A matching decomp**: C that rebuilds the retail disc byte for byte.

You need your own copy of the game for either. None of its files are included.

## Play it on PC

1. Download the zip from the [latest release](https://github.com/jake-r-r-w/crash_bash_decomp/releases/latest) and unzip it.
2. Put your Crash Bash (USA) disc image (`.cue` with its `.bin`, a raw `.bin` or an `.iso`) next to `crashbash.exe`.
3. Run `crashbash.exe`. It copies what it needs the first time, so you won't need the image again.

Controls and texture packs are in the zip's README.

### Build the port yourself

You need [CMake](https://cmake.org/), [Ninja](https://ninja-build.org/) and [llvm-mingw](https://github.com/mstorsjo/llvm-mingw) on your `PATH`.

```sh
git clone https://github.com/jake-r-r-w/crash_bash_decomp.git
cd crash_bash_decomp/port
cmake --preset windows-clang -DBASH_FETCH_SDL3=ON
cmake --build --preset windows-clang
```

That makes `build/port/windows-clang/crashbash.exe`. More in [docs/PORT.md](docs/PORT.md).

## Rebuild the disc

You need [Docker](https://www.docker.com/) (running) and git. On Windows, use Git Bash, or `tools\dk.ps1` in PowerShell.

1. Clone the repo as above, then make a `disc` folder in it and put your Crash Bash (USA) `.bin` and `.cue` in there.
2. Add PsyQ, Sony's SDK (this downloads and checks it):

   ```sh
   tools/get_psyq.sh
   ```

3. Build it (the first time takes about 10 minutes):

   ```sh
   tools/dk make disc
   ```

`build/CrashBash.cue` is your rebuilt game, for any PlayStation emulator. `tools/dk make disc-check` checks it equals retail.

To keep game files out of your commits: `git config core.hooksPath tools/hooks`.

## Docs

| Doc | What's in it |
|---|---|
| [PORT.md](docs/PORT.md) | The PC port: its design, building and running it |
| [ARCHITECTURE.md](docs/ARCHITECTURE.md) | How the game works |
| [PLAYING.md](docs/PLAYING.md) | Playing a disc build, editing the game data |
| [ASSETS.md](docs/ASSETS.md) | The game's file formats |
| [MATCHING.md](docs/MATCHING.md) | How functions were matched |
| [PROTOTYPES.md](docs/PROTOTYPES.md) | What the prototype and demo discs gave |
| [ROADMAP-B.md](docs/ROADMAP-B.md) | What's left ([ROADMAP-A.md](docs/ROADMAP-A.md): what's done) |
| [CONTRIBUTING.md](CONTRIBUTING.md) | The rules for changes |

## Notice

A preservation and research project, not affiliated with or endorsed by Eurocom, Sony Interactive
Entertainment or Activision. Crash Bash and Crash Bandicoot are trademarks of their owners.

No game files, Sony SDK files or compilers are included. The only text from the game's own files is
four headers recovered from its archive (`include/bash/arkenoid.h`, `pogo.h`, `polar.h`, `tank.h`).
Issues and pull requests with game or SDK files, or links to them, are refused.

## Credits

Built with [splat](https://github.com/ethteck/splat), [maspsx](https://github.com/mkst/maspsx),
[old-gcc](https://github.com/decompals/old-gcc), [m2c](https://github.com/matt-kempster/m2c),
[decomp-permuter](https://github.com/simonlindholm/decomp-permuter),
[objdiff](https://github.com/encounter/objdiff), [mkpsxiso](https://github.com/Lameguy64/mkpsxiso),
GCC and binutils, the converted PsyQ 4.7 libraries
([psx.arthus.net](http://psx.arthus.net/sdk/Psy-Q/)), [CTR-tools](https://github.com/DCxDemo/CTR-tools),
[Pillow](https://python-pillow.org), [SDL3](https://libsdl.org), RetroArch and PCSX ReARMed. And Eurocom, who made Crash Bash.

## License

The project's own tooling, PC port code, build files and docs are MIT ([LICENSE](LICENSE)). `src/`, `include/` and
`port/game/` reconstruct the game and belong to its owners: nothing here grants any right to the game.
`tools/reference/` (GCC's source) and `tools/cc1/patches/` are GPL.
