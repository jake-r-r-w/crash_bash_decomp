# Crash Bash Port 1.3.0

Button prompts for the controller in your hands, and mouse buttons for the game's buttons.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over from earlier versions.

## Button prompts

The game's pictures of its buttons now show what you're playing with: an Xbox, PlayStation, Nintendo or Steam Deck controller's
buttons, or your keys, everywhere the game draws them, in its menus and on every game's rules pages, the d-pad, L1 and R1 too. Pick
up a controller and they change to its buttons; press a key and they change to your keys. They follow player 1, and the keys and
buttons you've chosen on the CONTROLS page, so a button you've moved shows where it is now.

PROMPTS, on the CONTROLS page under STICK DEADZONE, is AUTO, which follows what player 1 pressed last, or always one of ORIGINAL (the
game's own PlayStation pictures), XBOX, PLAYSTATION, NINTENDO, STEAM DECK or KEYBOARD. The CONTROLS page now names the game's buttons
in words, with their pictures beside them. On Android's touch buttons the game's own pictures stay.

The pictures are Kenney's Input Prompts (www.kenney.nl), which are free for anyone to use: thank you, Kenney.

## Mouse buttons

On the CONTROLS page, choose a row and click a mouse button instead of pressing a key: the left, right or middle button, or either
side button, plays that button of the game. In menus a click still chooses and a right click still goes back, so a mouse button you've
given a game button only plays it outside them.

## Online

Everyone in an online game needs this same version: 1.3.0 and 1.2.0 can't play together, and the game says so if they try. Each
player sees the prompts for their own controller.

## How it's been tested

On each of the three, the release build plays every game mode, and the extra heroes load. On Windows and Linux it also beats all four
bosses, goes through the menus by buttons and (on Windows) by mouse, loads mods, reads disc images, plays whole online games on one
PC, gives exactly the same game from the same presses under every picture setting, and handles controllers coming and going. Every
game's rules pages were checked with the new prompts, and a mouse button given to a game button was checked to play it in a game.
The prompts have been tried with pretend controllers and on screen, not yet with every real controller.

No Mac version. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt` in the Windows and Linux downloads, or the repository: it includes SDL3's, zlib's,
miniupnpc's, libnatpmp's and Kenney's licences.
