# Crash Bash Port 1.4.1

A fix for mouse buttons bound to the game's controls.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over from earlier versions.

## Mouse buttons in the menus

With a control on a mouse button, the game's menus used to switch that button off, so with Square on the left click there was no way
to save over an old save. Now the game's menus only take the left and right clicks, which they use; the middle and side buttons always do
what they're bound to. On a screen with no cursor, like CONFIRM OVERWRITE, a left click on empty space does what the left button is
bound to. Thank you to the player who reported it.

## Online

Everyone in an online game needs this same version: 1.4.1 and 1.4.0 can't play together, and the game says so if they try.

## How it's been tested

On each of the three, the release build plays every game mode, and the extra heroes load. On Windows it also goes through the menus by
mouse, with a control on the left click, and the launcher's CONTROLS page by keys, buttons and mouse.

No Mac version. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt` in the Windows and Linux downloads, or the repository: it includes SDL3's, zlib's,
miniupnpc's, libnatpmp's, Kenney's, Recomp Achievements', rcheevos' and cJSON's licences.
