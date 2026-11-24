# Crash Bash Port 1.5.0

Heroes of your own, made from a 3D model, and a hero tool to make them, in place of the old extra heroes from the Crash Bandicoot
disc. And logs that say more when something goes wrong.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over from earlier versions.

## Custom heroes

A hero of your own can play every game, made from a 3D model (glTF). Each one is a mod, a hero pack: turn it on on the Mods page,
under Settings, and it has a card in the character select beside the eight, and the computer players can pick it too. It plays like
the hero it's based on, any of the eight, and rides, carries and holds what the others do in every game: the polar bear, the pogo
stick, the Ballistix car, the tank, the race cart, the dolphin, the dino, the mallet and the kegs. It has its own picture in the
character select and on the panel, and can have its own sounds and moves.

## The hero tool

The hero tool is in a `herotool` folder beside the game, and HERO TOOL on the Mods page opens it (Windows and Linux). Give it a model
as a .glb, a standing two-legged character of up to 1365 triangles, rigged or not, and pick who it's based on. It fits the model to the
game's bones, or you drag dots onto its joints, and shows it in every game at once, exactly as the game will draw it. Its moves can be
adjusted on a timeline, with keys that blend, a mirror, onion skin and a finder for where the model clips through itself. Give it a
name, a card (a photo of it, or a picture) and sounds if you like. Save keeps the hero in the tool's own `heroes` folder, Add to game
puts it in the game's mods and turns it on, and Preview starts the game with it straight away.

Bobby, a dachshund, comes with the hero tool as an example. He isn't in the game until you add him: open him in the tool and press
Add to game. A hero made on a PC plays on Android once its mod is copied into the game's `mods` folder there.

## The old extra heroes are gone

Ripper Roo and Pinstripe, the two extra heroes made from your own Crash Bandicoot (USA) disc, have been removed, and the game no
longer reads that disc. Custom heroes replace them: any model can be a hero now, not only two from one disc, and the hero tool makes
them. The EXTRA HEROES switch on the Game page is gone too: a hero mod that's on is all it takes. A Crash Bandicoot disc image in the
`discs` folder isn't used any more, and can be deleted.

## Online

Custom heroes play online when every player has the same hero pack: the game compares them, and leaves out any that someone hasn't.
Everyone in an online game needs this same version: 1.5.0 and 1.4.1 can't play together, and the game says so if they try.

## Logs

A log now lists every setting (as the game starts, and again when PLAY or an online game starts it), each controller as it's plugged
in, by its name, kind and connection, and what player 1 is playing with. In an online game it also says which system and controllers
each of the other machines has. If an online game stops because the games differ, please send the logs from every machine in it, and
`last-online-game.txt` from your saves folder: it can play the game again to find out why.

## How it's been tested

On each of the three, the release build plays every game mode, Bobby loads and plays from his hero pack, and an online game is played
between the PC and Android's emulator. On Windows and Linux it also beats all four bosses, goes through the menus by buttons and (on
Windows) by mouse, loads mods, reads disc images, plays whole online games on one PC, with a hero pack too, gives exactly the same game
from the same presses under every picture setting, handles controllers coming and going, and sends the right achievements to a
stand-in for RecompLauncher. On Windows, HERO TOOL on the Mods page opens the tool from its folder, and the tool's Add to game puts
Bobby in the game's mods and turns him on. The hero tool has been used to make and adjust heroes on Windows; on Linux it has only been
built.

No Mac version. Only the USA version's data is supported.

## Licence

The port's own code is MIT. Crash Bash's code and data belong to their respective owners, and nothing here grants any right to the
game. See `LICENSE.txt` in the Windows and Linux downloads, or the repository: it includes SDL3's, zlib's, miniupnpc's, libnatpmp's,
Kenney's, Recomp Achievements', rcheevos', cJSON's and cgltf's licences, and the hero tool's Dear ImGui and Fredoka font licences.
