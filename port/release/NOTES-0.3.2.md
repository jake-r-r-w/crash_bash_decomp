# Crash Bash Port 0.3.2

A Linux version, alongside the Windows one. On Windows the game plays as in 0.3.1.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). Put your disc image (a .cue with its .bin, a raw
.bin or an .iso) next to the program (`crashbash.exe` on Windows, `crashbash` on Linux) and run it: it copies what it needs once. Or
copy `CRASHBSH.DAT` from the `CRASHBSH` folder on your disc next to it. Your saves and settings carry over.

## Linux

`crash-bash-pc-0.3.2-linux-x64.tar.gz`: unpack it and run `crashbash` in its folder. It needs 64-bit Linux from about 2021 on (glibc
2.34 or newer: Ubuntu 22.04, Debian 12, Fedora 35, Mint 21 and later) and a graphics card with Vulkan. Everything else it needs is in
the program. Your saves and settings are kept in `~/.local/share/CrashBash/CrashBash`.

To ask you for your disc image it needs a file chooser (your desktop's, or zenity). Without one it says so, and you can put the image
next to the program instead.

Everything the Windows version has is there: widescreen, anti-aliasing, the frame rates, upscaling, mods, and Ripper Roo and Pinstripe
from your own Crash Bandicoot (USA).

## Fixes

- A .cue sheet that names its .bin in other capitals than the file has (`CRASHBASH.BIN` for `CrashBash.bin`) works on Linux, as it
  always did on Windows.

## How it's been tested

The Linux version plays every game mode, beats all four bosses, goes through the menus, reads disc images and loads mods, and the
extra heroes play every game, all on a Linux system without a graphics card, drawing in software. It plays the same game as the
Windows version: the bosses fall on the same frame, and a long stretch of Adventure goes exactly the same way.

**The Linux version hasn't been played on a real Linux PC with a graphics card yet.** If it misbehaves on yours, please say.

**Playing with several controllers hasn't been tested.** If yours misbehaves, please say.

No Mac version yet. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt`.
