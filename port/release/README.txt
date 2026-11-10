CRASH BASH PORT - version @VERSION@

A native 64-bit @SYSTEM@ version of Crash Bash, built from the decompiled source
at https://github.com/jake-r-r-w/crash_bash_decomp. It isn't an emulator.

This download has no game data. You need your own copy of Crash Bash (USA).


GETTING STARTED

1. Put your disc image (a .cue with its .bin, a raw .bin or an .iso) in the same
   folder as @PROGRAM@.
2. Run @PROGRAM@. It copies what it needs the first time, so you won't need
   the image again.

If there's no image there, it asks you where yours is. Or copy CRASHBSH.DAT from
the CRASHBSH folder on your copy of the game next to @PROGRAM@.

@NEEDS@


CONTROLS

A controller just works, with its sticks and rumble. If the stick has to go
too far before you move, set STICK DEADZONE on the CONTROLS page, under
Settings in the menu the game opens with: ORIGINAL is the PlayStation's, nearly
three quarters of the way out, and 10% to 60% are less. The box beside it shows
where your stick is, green once it would move you. On the keyboard:

  Arrows        Move
  Z             Cross
  X             Circle
  A             Square
  S             Triangle
  Enter         Start
  Q / W         L1 / R1
  1 / 2         L2 / R2


ONLINE

ONLINE, in the menu the game opens with, plays the game with others on their own
machines, up to four players in all. Everyone needs the same version of this
program and their own copy of the game.

  HOST GAME     Opens a game, and shows who's joined. Press START when they're
                all there. The first time, Windows asks whether to let the game
                through its firewall: say yes.
  JOIN GAME     A game hosted on your own network shows up by itself. For one
                anywhere else, choose CODE and type the code the host reads out
                (eight letters and digits, as 94FM-4B2B), then press Start.

To let players elsewhere in, the game asks the host's router to open a port for
it, and shows the join code once it has. A code is the host's internet address
in another form, so give it only to people you'd give that to.

Some routers won't (the setting is called UPnP, and can be off), and some
internet providers share one address between many homes, as phone networks do.
The game says so if it meets either. Then someone else can host (you can still
join); or the host can open UDP port 47000 on the router by hand, and the others
type the host's internet address where the code goes; or everyone can use a
virtual network.

In an online game each connected player has a small tag over their picture at
the top of the screen: which player they are (YOU on yours), and a dot that is
green while their connection keeps up, orange when the game has had to wait for
it, and red when it has stopped answering. The pause menu's sound options, and
its video options, are your own there: changing them changes them for you only.

A virtual network puts everyone's machines on one network over the internet,
whatever their own is like: Hamachi, Radmin VPN and ZeroTier are three, free for
a few players. Each player installs the same one and joins the same network in
it. Then host as usual, and the game shows up in the others' JOIN GAME by
itself. If it doesn't (Tailscale is one that won't), type the host's address on
that network where the code goes: the host's screen shows it. When Windows asks
about its firewall, allow the game on public networks too, as that's what it
takes these for.

Each player's own machine runs the whole game, and only the button presses
travel, each used a moment late so it reaches everyone in time. The last online
game's presses are kept beside your saves, in last-online-game.txt: if a game
stops because the players' games are no longer the same, that file is what
shows why. Player 1, the host, drives the menus. Picture settings are each
player's own. Cheats, the two skips and the save are the host's; extra heroes
and mods that change the game's files are off online.


SETTINGS

The menu the game opens with has its settings, and the pause menu has the
Video ones too. The new looks are all off at first, so the game looks as it
always did until you turn them on:

  WIDESCREEN    A wide window shows more of the world, with each level's own
                scenery carried on at the sides
  ANTIALIAS     Smooths the edges of things
  TEXTURES      SMOOTH blends the textures' pixels
  UPSCALE       Rounds off textures on your PC, from your own data
  ICONS         CRISP sharpens the hero and boss pictures
  FRAME RATE    30 is the original, 60 and 120 are smoother


EXTRA HEROES

With your own copy of Crash Bandicoot (USA) too, Ripper Roo and Pinstripe from
its boss fights can play, as they looked there, with their own laughs. Put that
disc image (a .cue with its .bin, a raw .bin or an .iso) in the same folder as
@PROGRAM@, or in a folder called discs next to it, and turn on EXTRA HEROES
on the Game page, under Settings in the menu the game opens with. Each has a
card of his own in the character select, in a third row under the others, and
the computer players can pick them too.

They're made from the image each time the game starts, so keep it there: nothing
of it is copied or kept. Roo's made for every game: Crate Crush and the others on
foot, Polar Push, where he rides a polar bear as the others do, Pogo, where he
bounces on the cane he has as its boss, Ballistix, where he drives his car with
his head, Tank, where he has Dingodile's flamethrower, Crash Dash, in a red race
cart of his own, or on the water track riding a red fish, holding on with his
tongue, the mallet game, where he swings his mallet with it in his mouth, the
gem game, riding a red dino, and the kegs game, with his keg balanced on his
head. Pinstripe's made for every game too, doing as Crash does: on foot, his
tommy gun out as he attacks and as he wins, riding Crash's polar bear, bouncing
on Crash's pogo stick, in Crash's Ballistix car and tank, in a gunmetal race
cart of his own, riding a gunmetal dolphin and dino, swinging Crash's mallet and
carrying a keg under his arm. Both stand on the tournament's podium as themselves.


MODS

Mods change the game's textures, and its files too. To add one, put its zip in
the mods folder next to @PROGRAM@, with no need to unzip it. It shows up on
the Mods page, under Settings in the menu the game opens with, and it's on.
OPEN MODS FOLDER at the top of that page opens the folder.

On the Mods page, Cross turns the chosen mod on or off, and L1 and R1 (Q and W)
move it up and down: where two mods change the same thing, the higher one wins.
Textures change at once, and files when you press PLAY. None come with the game.

Texture packs made for earlier versions, folders of PNGs in the packs folder,
are on the Mods page too, off until you turn them on.

Mods are files the game loads, so only use ones from people you trust.


MAKING MODS

A mod is a zip, or a folder in the mods folder while you make it, of PNGs
named after the textures they replace and of the game's own files in a folder
called files. A file called mod.json can give its name, author, version and
description:

    { "name": "Pink Crates", "author": "You", "version": "1.0",
      "description": "Every crate is pink" }

For textures, run the game once with
    @RUN@ --dump-textures mytextures
and play: each texture is saved to mytextures as it first appears, named as a
mod needs it. Edit the PNGs, at any size up to 2048 x 2048, and put the ones
you've changed in your mod. A texture with no PNG stays as it was. A PNG's clear
parts stay clear. The same picture in other colours counts as another texture
with its own PNG. Only what's seen gets saved, so play everything you want to
change. Some faces of a model have no texture, only colours painted on them, and
no mod can change those.

For the game's files,
    @RUN@ --dump-files myfiles
saves them all, named by number, and
    @RUN@ --log-files > files.txt
lists which ones the game reads as you play. Put a changed one in your mod's
files folder with the same name. Or, to share your mod without the game's own
files in it, put a patch there instead, NNN.bps, made with any maker of BPS
patches: it holds only what you changed. The guide is docs/MODDING.md at the
address above.


YOUR SAVES AND SETTINGS

@SAVES@
There's no memory card to set up: save at the computer in a warp room, as in
the game.


KNOWN LIMITS

- Only the USA version's data is supported.
- The whole Adventure has been played through to 200% by a test player, but that
  player needed weakened opponents for many of the challenges, so the hardest
  ones have had less testing at full strength.
- Playing with several controllers hasn't been tested.
- Online play has been tested between copies on one PC, between Windows and
  Linux, and with Android's emulator, but not yet between two homes over the
  internet.


LICENCE

See LICENSE.txt. Crash Bash's code and data belong to their respective owners,
and nothing here grants any right to the game.
