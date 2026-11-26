# Crash Bash Port 1.5.2

A fix for the hero tool not starting on Windows.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over from earlier versions.

## The hero tool starts

In 1.5.0 and 1.5.1 the hero tool wouldn't start on Windows: it said libc++.dll and then libunwind.dll weren't found. It needed two
files of the compiler it was built with, which were on the machine it was built on and on nobody else's. It has everything it needs
inside it now. The Linux hero tool needed the system's C++ library, which an older Linux might have had too old, and now has its own
too. Each download is now checked before it's published for anything the programs need that a player's computer mightn't have.

The hero tool is also half the size it was, without the debug information it was built with by mistake. The game itself is
unchanged. Thank you to the player who reported it.

## Online

Everyone in an online game needs this same version: 1.5.2 and 1.5.1 can't play together, and the game says so if they try.

## How it's been tested

On each of the three, the release build plays every game mode, and Bobby loads and plays from his hero pack. On Windows and Linux it
also goes through the launcher's menus and its CONTROLS page. On Windows the hero tool was started from the download, copied away
from the computer it was built on and with nothing of the compiler's to be found, where it opened Bobby and made him for every game,
and the game played from the same copy; and the hero tool opens from the game's Mods page and adds Bobby to the game.

No Mac version. Only the USA version's data is supported.

## Licence

The port's own code is MIT. Crash Bash's code and data belong to their respective owners, and nothing here grants any right to the
game. See `LICENSE.txt` in the Windows and Linux downloads, or the repository: it includes SDL3's, zlib's, miniupnpc's, libnatpmp's,
Kenney's, Recomp Achievements', rcheevos', cJSON's and cgltf's licences, and the hero tool's Dear ImGui and Fredoka font licences.
