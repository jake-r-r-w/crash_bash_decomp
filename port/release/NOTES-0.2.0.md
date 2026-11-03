# Crash Bash for PC 0.2.0

The native 64-bit Windows version of Crash Bash, now with widescreen and other new looks, all optional.

**This download has no game data.** You need your own copy of Crash Bash (USA). Put your disc image (a .cue with its .bin, a raw
.bin or an .iso) next to `crashbash.exe` and run it: it copies what it needs once. Or copy `CRASHBSH.DAT` from the `CRASHBSH` folder
on your disc next to it, as before. Your saves and settings carry over.

## New

- **Widescreen.** A wide window shows more of the world. Every level was gone through so the sides aren't black: its own scenery
  carries on past where it used to stop, made from your copy of the game, with nothing added from anywhere else. That's every
  minigame and boss, both parts of the Oxide fight, the menus and warp rooms, and eleven of the crystal challenge arenas. Where a
  level was dark at its edges to begin with, it stays dark.
- **Your disc image can be the game's data.** Point it at a .cue, .bin or .iso once, or just put the image next to the program.
- **Anti-aliasing** (2x, 4x or 8x) and **smooth textures**.
- **Upscaling** (2x or 4x): textures rounded off on your own PC, with no AI and nothing downloaded.
- **Crisp icons**: sharper hero and boss pictures.
- **Texture packs**: put a folder of PNGs in `packs` and choose it under Settings, Mods. `crashbash.exe --dump-textures mypack`
  saves the game's own textures as a place to start. None come with the game.

They're all off at first, so the game looks as it always did until you turn them on. The settings are in the menu the game opens
with, and the Video ones are in the pause menu too.

## Fixes

- In Polar Push, a hero who picks up a snowball no longer vanishes, leaving only their reflection.
- 120 frames a second on a 60 Hz screen no longer plays in slow motion.
- A maximised window opens maximised again, rather than screen-sized and partly under the taskbar, and WINDOW SIZE can take it out
  of being maximised.

## How it's been tested

With the new settings off, frames were compared with the build before each change, and only the fixes above change anything.
Each level's widescreen look was checked by eye at several shapes, from 16:9 to 32:9. The release build plays every game mode, beats
all four bosses and goes through the menus.

**Analog sticks and rumble still haven't been tried on a real controller since the pads were rewritten in 0.1.1,** and playing with
several controllers hasn't been tested. If yours misbehaves, please say.

Windows only for now. Only the USA version's data is supported.

## Licence

The port's own code is MIT. Crash Bash's code and data belong to their respective owners, and nothing here grants any right to the game.
See `LICENSE.txt`.
