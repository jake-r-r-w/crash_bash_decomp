# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the dashboard updates.

Parts 1 to 6 are done, the PC port's own last (2026-11-01; ROADMAP-A.md). The port was Windows only at first, at the user's request; Linux
and the Mac are 7.13.

## Part 7 - More for the PC port

What the port could have beyond the game as it was. None of it may reach the decomp: it's all in `port/` (PORT.md, The rules). In order: the
quick wins for every player first, then the look, each building on the last, then the two big jobs. Mods can use what the packs need, and online play comes
last as it's the largest and any change to the game's state affects it.

Where it makes sense, each is optional: a setting in the launcher to turn it on or off, so people can play with the original or the new. That's 7.2 to 7.6, and each mod in 7.8. Turned off, the game looks and plays as it did. Easier setup and online play aren't toggles: one's a step in setting up and the other's a way to play.

| # | Task | Notes | Done |
|---|------|-------|------|
| 7.12 | Extra heroes | Bosses and Ripper Roo as new hero slots, at the user's word (2026-11-03), a launcher toggle that's off by default: each with a card of its own in the character select, its own model, name and icon, playing every game. Inside, each borrows the number of an original hero nobody picked that match (at most four of the eight are taken), so the game's tables by hero (speeds, AI, sounds, which model) all still work and no two players clash. A hero's moves are sequences that pick animations by their place in the model and frames by number, so each extra hero's are translated from its base hero's: animation by name (Roo's RUN is his JUMP), frames scaled to its own lengths, timing and loops kept | [ ] |
| 7.12.4 | The bosses | Papu Papu (7 animations), the Bearminator (6), Big Bad Fox (5), Oxide (a still model in his hovercraft) | [ ] |
| 7.12.5 | Icons and results | New art where the game has none (Roo, the fox), the podium and win screens. Roo and Pinstripe have their games' own cards; the win screens are right with them; on the podium each stood as the hero whose number it borrows, so each has a podium model of its own (2026-11-08, EXTRA_HEROES.md 4, step 14), not yet played by the user. The fox's art goes with the bosses (7.12.4) | [ ] |
| 7.12.6 | Adventure | The warp rooms, teams and saves: later, if at all | [ ] |
| 7.12.7 | Bonus heroes from other discs | At the user's idea (2026-11-03): with the player's own copy of another Crash game beside the program or in a `discs` folder, an extra hero looks as it did there, its model and animations converted on their machine and never kept (`main/bonusheroes.c`). Ripper Roo from Crash Bandicoot (USA) is in, with his hero card from his boss fight on the panel; that disc's other bosses (Pinstripe, Koala Kong, Papu Papu) could follow. Each hero is a file of its own (`main/heroes/`), Roo's the template the others follow: docs/EXTRA_HEROES.md goes through it. Adding one is a line in `include/bash/herolist.h` and its file, with a checklist and a check of it in every game (7.12.8, done). Pinstripe, at the user's word (2026-11-05): a model for every game, one game at a time as Roo's were, each approved by the user (2026-11-08) | [ ] |
| 7.13 | Linux and the Mac | At the user's word (2026-11-08), Linux first. Linux builds and plays (PORT.md, Linux): in a container (`tools/dl`), on Vulkan, with the modes, bosses, menu, hero and disc image checks passing there on a virtual screen and software Vulkan, the bosses won on the same vblanks as on Windows and the sound the same, and `package.mjs` makes its release (glibc 2.34 or newer), published in v0.3.2 (2026-11-09). Not yet played on a real Linux desktop. The Mac's code is ready as far as it can be without one (Metal, PORT.md, The Mac); building it needs a Mac, the route the user's call | [ ] |
| 7.14 | Android | At the user's word (2026-11-09), with touch buttons in the first version and tested in Android's emulator on Windows (PORT.md, Android). It builds (`tools/da`) and plays in the emulator, in either orientation, filling a sideways phone with its touch buttons faint over it: every mode passes there (`playtest.mjs modes --android`), and the first run takes a disc image from Android's file chooser. Released in v0.4.0 (2026-11-09), signed with its release key. On the first real phone, the user's (2026-11-10), it wouldn't start: SDL's Vulkan asks for features many phones lack, and the renderer now turns off the ones it doesn't use. With that it works on their phone (a test build; the fix isn't in a release yet) | [ ] |
| 7.9 | Online play | Rollback netcode. The game's fixed timing and repeatable runs make it more realistic than in most ports, and `same` already shows a run repeats exactly. It's still a big job: saving and restoring the whole game state each frame, including its heap, and keeping sound out of the replays | [ ] |

