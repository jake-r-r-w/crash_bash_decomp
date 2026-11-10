# Crash Bash Port 1.0.0

Online play: up to four players, each on their own machine, with no servers in between. And couch play for up to four on one machine,
with every key and controller button yours to choose.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over from earlier versions.

## Online play

ONLINE, in the menu the game opens with, has HOST GAME and JOIN GAME. Everyone needs this same version and their own copy of the game.

- **On the same network**, a game someone hosts shows up in JOIN GAME by itself.
- **Anywhere else**, the game asks the host's router to open a port for it (UPnP or NAT-PMP), and the host's screen shows an
  eight-character join code, as `94FM-4B2B`. The others choose CODE in JOIN GAME and type it, on the keyboard or with a controller.
- **Where that can't work**, as on a phone's hotspot, with UPnP turned off, or where a provider shares one address between homes, the
  host's screen says so. Then someone else can host, or everyone can join the same virtual network (Hamachi, Radmin VPN, ZeroTier or
  Tailscale): the game shows up in JOIN GAME by itself there, and the host's screen shows the address to join at if it doesn't.
- The first time, Windows asks whether to let the game through its firewall. Say yes, and for public networks too if you use a virtual
  network, as Windows takes those for public ones.

Every machine runs the whole game, and only the button presses travel. The host, player 1, drives the menus, and the host's saves and
settings (the cheat menu and the two skips) are everyone's for that game; extra heroes, and mods that change the game's files, are off
online. Each player's picture
settings, and the sound options in the pause menu, stay their own.

In a game, each connected player has a small tag over their picture: which player they are, YOU on yours, and a dot that's green
while their connection keeps up, orange when the game has had to wait for it, and red when they've stopped answering. The machines
check every second that they're still playing the same game, and stop it if not. The last online game's presses are kept beside your
saves, in `last-online-game.txt`: if a game ever stops like that, that file shows why, so please send it along when you say.

A join code is the host's internet address in another form, so give it only to people you'd give that to. Anyone who can reach the
port while a game is open can join it, and nothing is encrypted. What arrives is checked, and so are the host's saves before anyone
else's game loads them.

**Online play has been tested between copies on one PC, between Windows and Linux, and between Windows and Android's emulator, but not
yet between two homes over the internet, nor on a real phone.** If it misbehaves, please say what happened.

## Couch play

- With two or more controllers, PLAY goes through a players screen first: press up or down on a controller to move it to another player.
- Battle, Tournament and Adventure offer as many players as there are controllers (Adventure takes two).
- A controller pulled out and plugged back in keeps its player.
- CONTROLS, under Settings in the menu the game opens with, sets every game button's key and controller button. A key or button that
  another had swaps with it, and RESET TO DEFAULTS puts them all back.

**Playing with several real controllers hasn't been tested.** If yours misbehave, please say.

## How it's been tested

On each of the three, the release build plays every game mode, and the extra heroes load. On Windows and Linux it also beats all four
bosses, goes through the menus, loads mods, reads disc images, plays whole online games on one PC (22 kinds, through the menus and from
the command line, with packets late and lost, machines that vanish and stall, and join codes), gives exactly the same game from the
same presses under every picture setting, and handles controllers coming and going. An online game between Windows and Android's
emulator played the same for all of it. The network code was fuzzed with millions of broken packets.

No Mac version. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt` in the Windows and Linux downloads, or the repository: it includes SDL3's, zlib's,
miniupnpc's and libnatpmp's licences.
