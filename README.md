# crash_bash_decomp

A matching decompilation of Crash Bash (USA, `SCUS_945.70`) for the PlayStation. Every function and
all the data are C that rebuilds the retail disc byte for byte, and the game's files come out of
your disc as editable files.

## Notice

A preservation and research project, not affiliated with or endorsed by Eurocom, Sony Interactive
Entertainment or Activision. Crash Bash and Crash Bandicoot are trademarks of their owners.

- **No game files are included**, and no Sony SDK files or compilers. The only text from the game's
  own files is four headers recovered from its archive (`include/bash/arkenoid.h`, `pogo.h`,
  `polar.h`, `tank.h`).
- **You need your own retail copy** of the game to build anything.
- **Don't share game files here**: issues and pull requests with game or SDK files, or links to
  them, are refused.

## Getting started

You need [Docker](https://www.docker.com/) (running) and git.

1. **Get the code**

   ```sh
   git clone https://github.com/jake-r-r-w/crash_bash_decomp.git
   cd crash_bash_decomp
   ```

2. **Add your game.** Make a `disc` folder here and put your Crash Bash (USA) `.bin` and `.cue` in it.

3. **Add PsyQ** (Sony's SDK; downloads and checks it):

   ```sh
   tools/get_psyq.sh
   ```

4. **Build your disc**

   ```sh
   tools/dk make disc
   ```

That's it: `build/CrashBash.cue` is your rebuilt game. Play it in any PlayStation emulator
([docs/PLAYING.md](docs/PLAYING.md)). The first build takes about 10 minutes, because it unpacks
your disc first. On Windows, run the commands in Git Bash, or use `tools\dk.ps1` in PowerShell.

## Commands

Run them as `tools/dk make <target>`:

| Target | What it does |
|---|---|
| `disc` | Build a playable disc image |
| `check` | Build, and compare every binary with retail |
| `disc-check` | Build the disc, and check it equals the retail disc |
| `verify` | Clean rebuild, then check |
| `progress` | Regenerate the dashboard (`docs/index.html`) |
| `test-tools` | Run the tools' tests |

To keep game files out of your commits: `git config core.hooksPath tools/hooks`.

## Documentation

| Doc | What's in it |
|---|---|
| [ARCHITECTURE.md](docs/ARCHITECTURE.md) | How the game works, and how the binaries are laid out |
| [PLAYING.md](docs/PLAYING.md) | Playing a build, editing the game data, the shifted builds |
| [ASSETS.md](docs/ASSETS.md) | The game's file formats |
| [MATCHING.md](docs/MATCHING.md) | How functions were matched, and how to rework one |
| [PROTOTYPES.md](docs/PROTOTYPES.md) | What the prototype and demo discs gave |
| [PORT.md](docs/PORT.md) | The plan for a native 64-bit PC port, and the rules that keep it apart from the decomp |
| [ROADMAP-B.md](docs/ROADMAP-B.md) | What's left ([ROADMAP-A.md](docs/ROADMAP-A.md): what's done) |
| [CONTRIBUTING.md](CONTRIBUTING.md) | The rules for changes |

## Credits

Built with [splat](https://github.com/ethteck/splat), [maspsx](https://github.com/mkst/maspsx),
[old-gcc](https://github.com/decompals/old-gcc), [m2c](https://github.com/matt-kempster/m2c),
[decomp-permuter](https://github.com/simonlindholm/decomp-permuter),
[objdiff](https://github.com/encounter/objdiff), [mkpsxiso](https://github.com/Lameguy64/mkpsxiso),
GCC and binutils, the converted PsyQ 4.7 libraries
([psx.arthus.net](http://psx.arthus.net/sdk/Psy-Q/)), [CTR-tools](https://github.com/DCxDemo/CTR-tools),
[Pillow](https://python-pillow.org), RetroArch and PCSX ReARMed. And Eurocom, who made Crash Bash.

## License

The project's own tooling, PC port code, build files and docs are MIT ([LICENSE](LICENSE)). `src/`, `include/` and
`port/game/` reconstruct the game and belong to its owners: nothing here grants any right to the game.
`tools/reference/` (GCC's source) and `tools/cc1/patches/` are GPL.
