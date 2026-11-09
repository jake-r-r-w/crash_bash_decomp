# Crash Bash Port 0.3.0

The native 64-bit Windows version of Crash Bash, now with mods, and two heroes from Crash Bandicoot who can play every game.

**This download has no game data.** You need your own copy of Crash Bash (USA). Put your disc image (a .cue with its .bin, a raw
.bin or an .iso) next to `crashbash.exe` and run it: it copies what it needs once. Or copy `CRASHBSH.DAT` from the `CRASHBSH` folder
on your disc next to it, as before. Your saves and settings carry over.

## New

- **Extra heroes.** With your own copy of Crash Bandicoot (USA) as well, Ripper Roo and Pinstripe from its boss fights join the
  character select, as they looked there and with their own laughs, and play every game. Each rides, drives and carries what the
  others do: a polar bear, a pogo stick (Roo bounces on his cane), a Ballistix car, a tank, a race cart, a fish, a mallet, a dino
  and a keg. Put that disc image next to `crashbash.exe` or in a `discs` folder beside it, and turn on EXTRA HEROES on the Game
  page under Settings. They're made from the image on your own PC each time the game starts: nothing of it is copied or kept, and
  nothing of them comes with this download. The computer players can pick them too.
- **Mods.** Put a mod's zip in the new `mods` folder and it shows up on the Mods page under Settings, where you turn each one on or
  off and choose which wins where two change the same thing. A mod can replace textures with PNGs and the game's own files, or
  patch them (BPS), so it can be shared without the game's files in it. `--dump-textures`, `--dump-files` and `--log-files` help you
  make one, and the guide is `docs/MODDING.md` in the repo. Texture packs made for 0.2.0 are on the Mods page too. None come with
  the game.

They're both off at first, so the game plays as it always did until you turn them on.

## Fixes

- In Polar Push, a hero in Cortex's, Brio's, Dingodile's or Rilla Roo's place who had pushed fell straight off at the ice's edge,
  instead of teetering there first as everyone else does. They teeter again.

## How it's been tested

The release build plays every game mode, beats all four bosses and goes through the menus. Both extra heroes play every game in an
automatic test, four of them at once. Ripper Roo has been played in every game by hand, and Pinstripe's games have been watched as
the computer played them. Mods were tested with zips of textures, of files and of patches, broken ones, and two changing the same
thing.

**Playing with several controllers hasn't been tested.** If yours misbehaves, please say.

Windows only for now. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt`.
