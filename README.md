# crash_bash_decomp

Crash Bash (USA, `SCUS_945.70`) for the PlayStation, decompiled to C. It gives you two things:

- **A native port**: a 64-bit game for Windows, Linux and Android, with widescreen, mods, extra heroes and online play. Not an emulator.
- **A matching decomp**: C that rebuilds the retail disc byte for byte.

You need your own copy of the game for either. None of its files are included.

## Native port

1. Download the zip for Windows or the `.tar.gz` for Linux from the [latest release](https://github.com/jake-r-r-w/crash_bash_decomp/releases/latest) and unpack it.
2. Put the disc image of your legal copy of Crash Bash (USA) (a `.cue` with its `.bin`, a raw `.bin` or an `.iso`) next to `crashbash.exe` (`crashbash` on Linux), or copy `CRASHBSH.DAT` from the `CRASHBSH` folder on it there.
3. Run `crashbash.exe` (`crashbash` on Linux).

On Android, install the `.apk` from the same release, and the first time it asks for your disc image (the `.bin`, or an `.iso`). It has on-screen touch buttons, or takes a controller.

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
| **Icons**         | Crisp hero and boss pictures, or the original's        |
| **Mods**          | Textures, game files and patches from zips, on a Mods page |
| **Extra heroes**  | Ripper Roo and Pinstripe, from your own Crash Bandicoot (USA), in every game |
| **Online play**   | Up to four players on their own machines, with no servers: by your network, a join code or a virtual network |
| **Controllers**   | Up to four players on one machine, with a screen to put the controllers in order; analog sticks, with a deadzone of your choosing, and rumble, or the keyboard; every key and button can be changed |
| **Touch**         | On Android, faint buttons over the picture, or a controller |
| **Saves**         | Kept in a file, with no memory card to set up          |
| **Quick start**   | Skip the logos and the loading screens                 |
| **Cheat menu**    | The original's hidden cheats, in the pause menu        |

It needs 64-bit Windows 10 or 11 and Direct3D 12, 64-bit Linux with glibc 2.34 or newer (Ubuntu 22.04, Debian 12, Fedora 35 and later) and Vulkan, or Android 8.0 or newer with Vulkan. The Linux version hasn't been played on a real Linux PC with a graphics card yet, only in software. There's no Mac version. Only the USA version's data is supported. Playing with several controllers at once, and online play between two homes, haven't been tested by people yet.

### Playing online

ONLINE, in the menu the game opens with, has HOST GAME and JOIN GAME. Everyone needs the same version and their own copy of the game.

- **On the same network**, a hosted game shows up in JOIN GAME by itself.
- **Anywhere else**, the game asks the host's router to open a port (UPnP or NAT-PMP), and the host's screen shows an eight-character join code for the others to type in.
- **Where that can't work**, as on a phone's hotspot or with UPnP turned off, everyone can join the same virtual network (Hamachi, Radmin VPN, ZeroTier or Tailscale). The host's screen shows the address to join at there.

Every machine runs the whole game and only the button presses travel, so nothing goes through a server. Each player has a small tag over their picture, its dot green, orange or red with their connection, and the sound and picture settings stay each player's own. The release's `README.txt` says more, and [docs/PORT.md](docs/PORT.md) (Online play) has how it works.

### Build the port yourself

You need [CMake](https://cmake.org/), [Ninja](https://ninja-build.org/) and [llvm-mingw](https://github.com/mstorsjo/llvm-mingw) on your `PATH`.

```sh
git clone https://github.com/jake-r-r-w/crash_bash_decomp.git
cd crash_bash_decomp/port
cmake --preset windows-clang -DBASH_FETCH_SDL3=ON
cmake --build --preset windows-clang
```

That makes `build/port/windows-clang/crashbash.exe`. Configuring it downloads miniupnpc and libnatpmp, which online play uses, at one
version each checked by hash (and SDL3, with `-DBASH_FETCH_SDL3=ON`). More in [docs/PORT.md](docs/PORT.md).

On Linux it's the same with the `linux-clang` preset, which makes `build/port/linux-clang/crashbash`: it needs Clang, CMake, Ninja,
zlib's headers and [SDL3's build dependencies](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md), and Vulkan to run. Or
build it in Docker with `tools/dl` ([docs/PORT.md](docs/PORT.md), Linux).

Android's `.apk` builds in Docker too: `tools/da sh port/android/build.sh` ([docs/PORT.md](docs/PORT.md), Android).

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
| [PORT.md](docs/PORT.md) | The port: its design, building, running and testing it, and online play |
| [MODDING.md](docs/MODDING.md) | Making mods for the port |
| [EXTRA_HEROES.md](docs/EXTRA_HEROES.md) | The port's extra heroes, and adding one |
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

**The port:** [SDL3](https://libsdl.org), [zlib](https://zlib.net),
[miniupnpc and libnatpmp](https://github.com/miniupnp/miniupnp), [LLVM and Clang](https://llvm.org) (and its libFuzzer),
[llvm-mingw](https://github.com/mstorsjo/llvm-mingw), [CMake](https://cmake.org/), [Ninja](https://ninja-build.org/),
[Node.js](https://nodejs.org/), the Android SDK and NDK, and [Mesa](https://www.mesa3d.org/)'s software Vulkan for testing on Linux.

Written with [Claude](https://claude.com/claude-code) by Anthropic. And Eurocom, who made Crash Bash.

## License

The project's own tooling, PC port code, build files and docs are MIT ([LICENSE](LICENSE)). `src/`, `include/` and
`port/game/` reconstruct the game and belong to its owners: nothing here grants any right to the game.
`tools/reference/` (GCC's source) and `tools/cc1/patches/` are GPL.
