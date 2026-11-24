# Crash Bash Port 1.5.1

A fix for the demo starting when you press PLAY.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over from earlier versions.

## PLAY after a while on the launcher

With the launcher open for a while, clicking PLAY with the mouse went straight into the demo, the game playing one of its games by
itself, instead of the main menu. The game counts how long nobody has pressed a button, and the launcher's mouse clicks didn't count
as presses, so by the time PLAY was clicked it thought nobody had touched it for long enough. Now PLAY starts that count again, and
the main menu shows its demo only after you've left it alone there, as the game always has. Thank you to the player who reported it.

## Online

Everyone in an online game needs this same version: 1.5.1 and 1.5.0 can't play together, and the game says so if they try.

## How it's been tested

On each of the three, the release build plays every game mode, and Bobby loads and plays from his hero pack. On Windows and Linux it
also goes through the launcher's menus by buttons and the CONTROLS page by keys, buttons and mouse, including PLAY clicked after 40
seconds on the launcher, which now opens the main menu. On Windows it also goes through the menus by mouse.

No Mac version. Only the USA version's data is supported.

## Licence

The port's own code is MIT. Crash Bash's code and data belong to their respective owners, and nothing here grants any right to the
game. See `LICENSE.txt` in the Windows and Linux downloads, or the repository: it includes SDL3's, zlib's, miniupnpc's, libnatpmp's,
Kenney's, Recomp Achievements', rcheevos', cJSON's and cgltf's licences, and the hero tool's Dear ImGui and Fredoka font licences.
