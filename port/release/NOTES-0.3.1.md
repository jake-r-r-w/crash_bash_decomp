# Crash Bash for PC 0.3.1

Fixes, so the game plays and looks as it does on the PlayStation, and the extra heroes on the tournament's podium.

**This download has no game data.** You need your own copy of Crash Bash (USA). Put your disc image (a .cue with its .bin, a raw
.bin or an .iso) next to `crashbash.exe` and run it: it copies what it needs once. Or copy `CRASHBSH.DAT` from the `CRASHBSH` folder
on your disc next to it. Your saves and settings carry over.

## Fixes

- **Mallet Mash** speeds up in stages again as it should. The PC version jumped from the first stage straight to the fourth.
- **The pause menu** has its yellow frame round the box again.
- **The tournament's podium:** the heroes no longer vanish while they cheer or sulk, and the panel of each player's face and points
  shows along the bottom, as it does on the PlayStation.
- **Ripper Roo and Pinstripe on the podium** stand there as themselves, cheering and sulking in their own ways. Before, they stood
  there as the heroes they share a slot with.
- A number of places where the PC version read the wrong memory now read what the PlayStation does: the Tank games' arena edges, the
  Polar Push shadows and its transmuter beam, the Ballistix computer players' aim, and Toxic Dash's glob.

## How it's been tested

Every game mode, the menus, the boss fights and both extra heroes were run under a memory checker that catches any read or write
past the end of the game's data, and none is left. The release build plays every game mode, beats all four bosses and goes through
the menus, and the extra heroes play every game, the podium too.

**Playing with several controllers hasn't been tested.** If yours misbehaves, please say.

Windows only for now. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt`.
