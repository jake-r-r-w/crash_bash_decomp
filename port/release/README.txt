CRASH BASH for PC - version @VERSION@

A native 64-bit Windows version of Crash Bash, built from the decompiled source
at https://github.com/jake-r-r-w/crash_bash_decomp. It isn't an emulator.

This download has no game data. You need your own copy of Crash Bash (USA).


GETTING STARTED

1. Put your disc image (a .cue with its .bin, a raw .bin or an .iso) in the same
   folder as crashbash.exe.
2. Run crashbash.exe. It copies what it needs the first time, so you won't need
   the image again.

If there's no image there, it asks you where yours is. Or copy CRASHBSH.DAT from
the CRASHBSH folder on your copy of the game next to crashbash.exe.

It needs 64-bit Windows 10 or 11 and a graphics card with Direct3D 12. Windows
may warn about an unrecognised program, as this one isn't signed.


CONTROLS

A controller just works, with its sticks and rumble. On the keyboard:

  Arrows        Move
  Z             Cross
  X             Circle
  A             Square
  S             Triangle
  Enter         Start
  Q / W         L1 / R1
  1 / 2         L2 / R2


SETTINGS

The menu the game opens with has its settings, and the pause menu has the
Video ones too. The new looks are all off at first, so the game looks as it
always did until you turn them on:

  WIDESCREEN    A wide window shows more of the world, with each level's own
                scenery carried on at the sides
  ANTIALIAS     Smooths the edges of things
  TEXTURES      SMOOTH blends the textures' pixels
  UPSCALE       Rounds off textures on your PC, from your own data
  ICONS         CRISP sharpens the hero and boss pictures
  FRAME RATE    30 is the original, 60 and 120 are smoother


EXTRA HEROES

With your own copy of Crash Bandicoot (USA) too, Ripper Roo and Pinstripe from
its boss fights can play, as they looked there, with their own laughs. Put that
disc image (a .cue with its .bin, a raw .bin or an .iso) in the same folder as
crashbash.exe, or in a folder called discs next to it, and turn on EXTRA HEROES
on the Game page, under Settings in the menu the game opens with. Each has a
card of his own in the character select, in a third row under the others, and
the computer players can pick them too.

They're made from the image each time the game starts, so keep it there: nothing
of it is copied or kept. Roo's made for every game: Crate Crush and the others on
foot, Polar Push, where he rides a polar bear as the others do, Pogo, where he
bounces on the cane he has as its boss, Ballistix, where he drives his car with
his head, Tank, where he has Dingodile's flamethrower, Crash Dash, in a red race
cart of his own, or on the water track riding a red fish, holding on with his
tongue, the mallet game, where he swings his mallet with it in his mouth, the
gem game, riding a red dino, and the kegs game, with his keg balanced on his
head. Pinstripe's made for every game too, doing as Crash does: on foot, his
tommy gun out as he attacks and as he wins, riding Crash's polar bear, bouncing
on Crash's pogo stick, in Crash's Ballistix car and tank, in a gunmetal race
cart of his own, riding a gunmetal dolphin and dino, swinging Crash's mallet and
carrying a keg under his arm. Both stand on the tournament's podium as themselves.


MODS

Mods change the game's textures, and its files too. To add one, put its zip in
the mods folder next to crashbash.exe, with no need to unzip it. It shows up on
the Mods page, under Settings in the menu the game opens with, and it's on.
OPEN MODS FOLDER at the top of that page opens the folder.

On the Mods page, Cross turns the chosen mod on or off, and L1 and R1 (Q and W)
move it up and down: where two mods change the same thing, the higher one wins.
Textures change at once, and files when you press PLAY. None come with the game.

Texture packs made for earlier versions, folders of PNGs in the packs folder,
are on the Mods page too, off until you turn them on.

Mods are files the game loads, so only use ones from people you trust.


MAKING MODS

A mod is a zip, or a folder in the mods folder while you make it, of PNGs
named after the textures they replace and of the game's own files in a folder
called files. A file called mod.json can give its name, author, version and
description:

    { "name": "Pink Crates", "author": "You", "version": "1.0",
      "description": "Every crate is pink" }

For textures, run the game once with
    crashbash.exe --dump-textures mytextures
and play: each texture is saved to mytextures as it first appears, named as a
mod needs it. Edit the PNGs, at any size up to 2048 x 2048, and put the ones
you've changed in your mod. A texture with no PNG stays as it was. A PNG's clear
parts stay clear. The same picture in other colours counts as another texture
with its own PNG. Only what's seen gets saved, so play everything you want to
change. Some faces of a model have no texture, only colours painted on them, and
no mod can change those.

For the game's files,
    crashbash.exe --dump-files myfiles
saves them all, named by number, and
    crashbash.exe --log-files > files.txt
lists which ones the game reads as you play. Put a changed one in your mod's
files folder with the same name. Or, to share your mod without the game's own
files in it, put a patch there instead, NNN.bps, made with any maker of BPS
patches: it holds only what you changed. The guide is docs/MODDING.md at the
address above.


YOUR SAVES AND SETTINGS

They're kept in %APPDATA%\CrashBash\CrashBash. There's no memory card to set up:
save at the computer in a warp room, as in the game.


KNOWN LIMITS

- Only the USA version's data is supported.
- The whole Adventure has been played through to 200% by a test player, but that
  player needed weakened opponents for many of the challenges, so the hardest
  ones have had less testing at full strength.
- Playing with several controllers hasn't been tested.


LICENCE

See LICENSE.txt. Crash Bash's code and data belong to their respective owners,
and nothing here grants any right to the game.
