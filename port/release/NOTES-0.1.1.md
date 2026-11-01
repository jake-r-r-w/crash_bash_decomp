# Crash Bash for PC 0.1.1

A patch release of the native 64-bit Windows version of Crash Bash.

**This download has no game data.** You need your own copy of Crash Bash (USA). If you're new to it, unzip it anywhere, copy `CRASHBSH.DAT`
from your copy of the game (it's in the `CRASHBSH` folder) next to `crashbash.exe`, and run it. Your 0.1.0 saves and settings carry over.

## Fixes

- Floor tiles no longer float about as the camera moves, in Space Bash and the other arenas built from them.
- The sparkles in the credits have their trails.
- Mallet Mash's hero sounds and Moe and Joe's recoil sounds in Tank Wars come from where they happen. They could be silent or in one ear.

## Under the hood

The last of the PS1's shape is gone from how the game talks to the computer: its maths, drawing, pads, sound and files are all plain
calls now. It shouldn't look or play any differently because of that.

## How it's been tested

Frames and sound were compared with the build before this work across every game mode, the logos, the hub and the pause text, and the
only differences are the fixes above. The test player still plays the whole Adventure to 200%, with its save and reload, and beats all
four bosses.

**The pads were rewritten and haven't been tried on a real controller since.** That includes analog sticks and rumble. If yours misbehaves
and 0.1.0 was fine, please say. Playing with several controllers still hasn't been tested.

Windows only for now. Only the USA version's data is supported.

## Licence

The port's own code is MIT. Crash Bash's code and data belong to their respective owners, and nothing here grants any right to the game.
See `LICENSE.txt`.
