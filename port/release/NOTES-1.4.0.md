# Crash Bash Port 1.4.0

A fix for the crash in team games, much better logs, achievements, and RecompLauncher.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over from earlier versions.

## The crash at the end of Pogo Painter

Every game with a big team score crashed at the end of its first round: the Pogo games, Mallet Mash and Keg Kaboom, in an Adventure
with two players or a Battle in teams. That's fixed, and a two-player Adventure's Pogo Painter is now part of the tests. Thank you to
the players who sent their logs.

## Logs

Each time it runs, the game writes a log in a `logs` folder beside it, named by the date and time it started, and deletes logs older
than 14 days. A log says which version and build it is, the computer, graphics card and driver, the settings, and each level as it
starts. If the game crashes, the log says where, by the names of the game's own functions, and what it was doing then, and on Windows
a message names the log to send. If it crashes, please send the newest log with your report. Android writes the same logs.

## The mouse cursor

The mouse cursor now hides while you play and comes back in the menus, so in fullscreen with a keyboard and mouse a nudge of the mouse
no longer puts it over the game.

## Portable

To keep everything beside the game (on a USB stick, say), put an empty file called `portable.txt` next to it: the saves, the settings,
the game's data and the logs all go in folders there, and nothing is left anywhere else. The first time, what you already had is copied
over. `settings.ini` now has a line on each setting saying what it does.

## RecompLauncher

The port can now be installed with [RecompLauncher](https://recomplauncher.com/game/crash-bash-decomp), which sets it up from your disc
image, keeps it up to date, and shows its settings, keys and buttons on its own page.

## Achievements

Played from RecompLauncher, the Adventure has 59 achievements with [Recomp Achievements](https://github.com/elripalda/RecompAchievements):
a trophy for each of the 28 levels, clearing each warp room, the four bosses, 1, 10, 20 and all 28 of each award (trophies, gems,
crystals, gold and platinum relics), and 100% and 200%. With the cheat menu on they count as Casual. The game only reads its own
Adventure for them, so nothing changes how it plays, and outside RecompLauncher nothing happens at all.

## Online

Everyone in an online game needs this same version: 1.4.0 and 1.3.0 can't play together, and the game says so if they try.

## How it's been tested

On each of the three, the release build plays every game mode, and the extra heroes load. On Windows and Linux it also beats all four
bosses, goes through the menus by buttons and (on Windows) by mouse, loads mods, reads disc images, plays whole online games on one
PC, gives exactly the same game from the same presses under every picture setting, handles controllers coming and going, plays a
two-player Adventure's Pogo Painter past its first round, and sends the right achievements to a stand-in for RecompLauncher, holding
back the ones a test Adventure is one award short of. The achievements haven't yet been earned inside RecompLauncher itself.

No Mac version. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt` in the Windows and Linux downloads, or the repository: it includes SDL3's, zlib's,
miniupnpc's, libnatpmp's, Kenney's, Recomp Achievements', rcheevos' and cJSON's licences.
