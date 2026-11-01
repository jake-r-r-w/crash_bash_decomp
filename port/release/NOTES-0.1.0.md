# Crash Bash for PC 0.1.0

The first release of the native 64-bit Windows version of Crash Bash, built from this repo's decompiled source. It isn't an
emulator: the game's C runs natively, drawing with Direct3D 12 through SDL3.

**This download has no game data.** You need your own copy of Crash Bash (USA).

## Getting started

1. Unzip it anywhere.
2. Copy `CRASHBSH.DAT` from your copy of the game (it's in the `CRASHBSH` folder) next to `crashbash.exe`.
3. Run `crashbash.exe`.

It needs 64-bit Windows 10 or 11 and a graphics card with Direct3D 12. Windows may warn about an unrecognised program, as it isn't signed.

## What's in it

- Every game mode, the whole Adventure, Battle and Tournament
- Any window size and resolution, fullscreen, 30, 60 or 120 frames a second
- Controllers, with analog sticks and rumble, and the keyboard
- Saving at the computer, with no memory card to set up

## How it's been tested

A test player plays the whole Adventure to 200%, with no cheat flags, saves it and loads it back, and all four bosses are beaten by
real play. It isn't a strong player: 70 of the 140 challenges were won at full strength, and the rest against weakened or
motionless opponents, so the hardest ones have had less testing at full strength. A person has played it too, with a controller.
Battle and Tournament have only had a quick check with one player: **playing with several controllers hasn't been tested.**

Windows only for now. Linux is planned. Only the USA version's data is supported.

## Licence

The port's own code is MIT. Crash Bash's code and data belong to their respective owners, and nothing here grants any right to the game.
See `LICENSE.txt`.
