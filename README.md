# crash_bash_decomp

Crash Bash (USA, `SCUS_945.70`) for the PlayStation, decompiled to C. It gives you two things:

- **A native PC port**: a 64-bit Windows game. Not an emulator.
- **A matching decomp**: C that rebuilds the retail disc byte for byte.

You need your own copy of the game for either. None of its files are included.

## Native PC port

1. Download the zip from the [latest release](https://github.com/jake-r-r-w/crash_bash_decomp/releases/latest) and unzip it.
2. Put the disc image of your legal copy of Crash Bash (USA) (a `.cue` with its `.bin`, a raw `.bin` or an `.iso`) next to `crashbash.exe`, or copy `CRASHBSH.DAT` from the `CRASHBSH` folder on it there.
3. Run `crashbash.exe`.

| Feature           | Details                                                |
|-------------------|--------------------------------------------------------|
| **Game modes**    | All of them: Adventure, Battle and Tournament          |
| **Resolution**    | Any window size and render resolution                  |
| **Fullscreen**    | Or a window, whichever you like                        |
| **Vsync**         | On or off                                              |
| **Frame rate**    | 30, 60 or 120 FPS                                      |
| **Widescreen**    | Shows more of the world, with extra scenery where needed |
| **Anti-aliasing** | 2x, 4x or 8x                                           |
| **Filtering**     | Smooth textures, or the original's square texels       |
| **Upscaling**     | Textures at 2x or 4x                                   |
| **Mods**          | Textures and game files from zips, on a Mods page      |
| **Extra heroes**  | Ripper Roo and Pinstripe, from your own Crash Bandicoot (USA) |
| **Controllers**   | Analog sticks and rumble, or play on the keyboard      |
| **Saves**         | Kept in a file, with no memory card to set up          |
| **Quick start**   | Skip the logos and the loading screens                 |
| **Cheat menu**    | The original's hidden cheats, in the pause menu        |

It needs 64-bit Windows 10 or 11 and Direct3D 12. Only the USA version's data is supported, and playing with several controllers hasn't been tested.

### Build the port yourself

You need [CMake](https://cmake.org/), [Ninja](https://ninja-build.org/) and [llvm-mingw](https://github.com/mstorsjo/llvm-mingw) on your `PATH`.

```sh
git clone https://github.com/jake-r-r-w/crash_bash_decomp.git
cd crash_bash_decomp/port
cmake --preset windows-clang -DBASH_FETCH_SDL3=ON
cmake --build --preset windows-clang
```

That makes `build/port/windows-clang/crashbash.exe`. More in [docs/PORT.md](docs/PORT.md).

On Linux it's the same with the `linux-clang` preset, which makes `build/port/linux-clang/crashbash`: it needs Clang, CMake, Ninja,
zlib's headers and [SDL3's build dependencies](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md), and Vulkan to run. Or
build it in Docker with `tools/dl` ([docs/PORT.md](docs/PORT.md), Linux).

## PS1 decomp (disk 2 disk)

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

**The decomp:** [splat](https://github.com/ethteck/splat), [maspsx](https://github.com/mkst/maspsx),
[old-gcc](https://github.com/decompals/old-gcc), [m2c](https://github.com/matt-kempster/m2c),
[decomp-permuter](https://github.com/simonlindholm/decomp-permuter),
[objdiff](https://github.com/encounter/objdiff), [mkpsxiso](https://github.com/Lameguy64/mkpsxiso),
GCC and binutils, the converted PsyQ 4.7 libraries
([psx.arthus.net](http://psx.arthus.net/sdk/Psy-Q/)), [CTR-tools](https://github.com/DCxDemo/CTR-tools),
[Pillow](https://python-pillow.org), RetroArch and PCSX ReARMed.

**The port:** [SDL3](https://libsdl.org), [LLVM and Clang](https://llvm.org),
[llvm-mingw](https://github.com/mstorsjo/llvm-mingw), [CMake](https://cmake.org/),
[Ninja](https://ninja-build.org/) and [Node.js](https://nodejs.org/).

Written with [Claude](https://claude.com/claude-code) by Anthropic. And Eurocom, who made Crash Bash.

## License

The project's own tooling, PC port code, build files and docs are MIT ([LICENSE](LICENSE)). `src/`, `include/` and
`port/game/` reconstruct the game and belong to its owners: nothing here grants any right to the game.
`tools/reference/` (GCC's source) and `tools/cc1/patches/` are GPL.
