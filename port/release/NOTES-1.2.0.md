# Crash Bash Port 1.2.0

Phones that fold, and a say in where your saves and settings are kept.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over from earlier versions.

## Phones that fold (Android)

The game carries on when a folding phone is opened or closed, and fits each screen. On a big screen, a folding phone's inside one or a
tablet's, the touch buttons are the size they are on a phone, in reach of your thumbs, where they used to be twice that.

MOBILE FOLD, in the video settings on Android, gives the game one part of the screen and the buttons the rest, with nothing of the
game under your thumbs: the game above and the buttons below on a folding phone's inside screen or a phone held upright, and the
game in the middle with the buttons at its sides on a phone held sideways. It's off unless you turn it on.

A game started while the phone was asleep or locked used to crash. It waits for the screen now.

This has been tried on Android's emulator of a folding phone, not yet on a real one.

## Where your files are kept (Windows and Linux)

SETTINGS in the menu the game opens with has a DATA page, with a row each for your saves, your settings and the game's data copied
from your disc. Each can be in the USER FOLDER, where they've always been and still are unless you change it; in the GAME FOLDER,
folders called saves, settings and data beside the program, to keep everything in one place or on a USB stick; or in any folder you
pick with the row underneath. The page shows the folder each is in.

Moving one copies your files to the new place where it hasn't got them. Nothing is deleted or written over, so the old place keeps
what it had.

## Online

Everyone in an online game needs this same version: 1.2.0 and 1.1.0 can't play together, and the game says so if they try.

## How it's been tested

On each of the three, the release build plays every game mode, and the extra heroes load. On Windows and Linux it also beats all four
bosses, goes through the menus by buttons and (on Windows) by mouse, loads mods, reads disc images, plays whole online games on one
PC (23 kinds), gives exactly the same game from the same presses under every picture setting, and handles controllers coming and
going. An online game between Windows and Android's emulator played the same for all of it. Folding and MOBILE FOLD were tried in
Android's emulator of a folding phone, and the DATA page on Windows, its folder picker by hand.

No Mac version. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt` in the Windows and Linux downloads, or the repository: it includes SDL3's, zlib's,
miniupnpc's and libnatpmp's licences.
