# Crash Bash Port 1.1.0

A deadzone for the sticks that you choose, and the mouse in every menu.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over from earlier versions.

## The sticks' deadzone

As asked for on GitHub: the game moves you only once a stick is nearly three quarters of the way out, as on the PlayStation, which
can feel stiff on a modern controller. STICK DEADZONE, at the top of the CONTROLS page under Settings in the menu the game opens with,
is ORIGINAL (the PlayStation's) or 10% to 60%. With it on that row, a box beside the page shows where your stick is, and the dot turns
green once it would move you. Online, it's your own: it changes how your stick reads, not anyone's game.

## The mouse

The menus work with the mouse now, the game's own as well as the one it opens with. Point at a row and the cursor goes to it, click to
choose it, right-click to go back, and turn the wheel to go up and down. On a setting with a value (the sound and video settings, the
cups and the difficulty), click its left half for less and its right half for more. The lines that say what a button does, like
PRESS X BUTTON TO START BATTLE, can be clicked, and so can the tournament's arrows; on the rules' pages, a click anywhere turns the page.

The mouse presses your first player's buttons, as the keyboard does, so online it works the menus you'd work yourself and nobody
else's. It doesn't yet work the warp room's doors and save computer, typing a name, or the cheat menu: use the keys or a controller
there.

## Online

Everyone in an online game needs this same version: 1.1.0 and 1.0.0 can't play together, and the game says so if they try.

## How it's been tested

On each of the three, the release build plays every game mode, and the extra heroes load. On Windows and Linux it also beats all four
bosses, goes through the menus by buttons and by mouse, loads mods, reads disc images, plays whole online games on one PC (23 kinds,
one with the host using the mouse), gives exactly the same game from the same presses under every picture setting, handles
controllers coming and going, and moves a hero with a stick inside the original deadzone once a smaller one is set. An online game
between Windows and Android's emulator played the same for all of it.

No Mac version. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt` in the Windows and Linux downloads, or the repository: it includes SDL3's, zlib's,
miniupnpc's and libnatpmp's licences.
