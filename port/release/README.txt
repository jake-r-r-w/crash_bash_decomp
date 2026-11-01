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


TEXTURE PACKS

You can replace the game's textures with your own. A pack is a folder in a
folder called packs, next to crashbash.exe, full of PNGs. Choose it under
Settings, Video, Texture Pack. None come with the game.

To make one, run the game once with
    crashbash.exe --dump-textures mypack
and play: each texture is saved to mypack as it first appears, named as a pack
needs it. Edit the PNGs, at any size up to 2048 x 2048, keep the ones you've
changed, and move the folder into packs. A texture with no PNG stays as it was.
A PNG's clear parts stay clear. The same picture in other colours counts as
another texture with its own PNG, and the few textures that scroll can't be
replaced.

Packs are PNG files that the game loads, so only use ones from people you trust.


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
