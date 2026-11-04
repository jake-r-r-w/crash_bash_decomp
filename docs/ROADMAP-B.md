# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the dashboard updates.

Parts 1 to 6 are done, the PC port's own last (2026-11-01; ROADMAP-A.md). A Linux build stays in the port's plan (PORT.md), but isn't built, at the
user's request.

## Part 7 - More for the PC port

What the port could have beyond the game as it was. None of it may reach the decomp: it's all in `port/` (PORT.md, The rules). In order: the
quick wins for every player first, then the look, each building on the last, then the two big jobs. Mods can use what the packs need, and online play comes
last as it's the largest and any change to the game's state affects it.

Where it makes sense, each is optional: a setting in the launcher to turn it on or off, so people can play with the original or the new. That's 7.2 to 7.6, and each mod in 7.8. Turned off, the game looks and plays as it did. Easier setup and online play aren't toggles: one's a step in setting up and the other's a way to play.

| # | Task | Notes | Done |
|---|------|-------|------|
| 7.12 | Extra heroes | Bosses and Ripper Roo as new hero slots, at the user's word (2026-11-03), a launcher toggle that's off by default: each with a card of its own in the character select, its own model, name and icon, playing every game. Inside, each borrows the number of an original hero nobody picked that match (at most four of the eight are taken), so the game's tables by hero (speeds, AI, sounds, which model) all still work and no two players clash. A hero's moves are sequences that pick animations by their place in the model and frames by number, so each extra hero's are translated from its base hero's: animation by name (Roo's RUN is his JUMP), frames scaled to its own lengths, timing and loops kept | [ ] |
| 7.12.1 | The slot | The toggle (EXTRA HEROES, the launcher's GAME page); a ninth card, the first of a third row of the extra heroes' (gold, under the red row); a pick, the computer's too, and a free base hero when the game starts; the model in the base's place (`HeroHooks.HeroModels`) and loaded; the frames drawn as its own; its icon and portrait. The card, its row and the portraits approved by the user (2026-11-04); its name in text isn't done | [ ] |
| 7.12.3 | Every other game | Crate 2, Pogo, the two Polar, Ball, Tank, Kegs, Mallet, Dino, Race and Splash: a vehicle or mount kept with the extra hero in or on it, held things where its hands are. Polar Push's are in for Roo (2026-11-04, not yet played by the user): a model of each hero's kind (on foot, riding a bear), Roo sat on Crash's bear from the player's archive and moving as Crash does on it, as heavy and quick as Crash, with his own sounds for all of his base's | [ ] |
| 7.12.4 | The bosses | Papu Papu (7 animations), the Bearminator (6), Big Bad Fox (5), Oxide (a still model in his hovercraft) | [ ] |
| 7.12.5 | Icons and results | New art where the game has none (Roo, the fox), the podium and win screens | [ ] |
| 7.12.6 | Adventure | The warp rooms, teams and saves: later, if at all | [ ] |
| 7.12.7 | Bonus heroes from other discs | At the user's idea (2026-11-03): with the player's own copy of another Crash game beside the program or in a `discs` folder, an extra hero looks as it did there, its model and animations converted on their machine and never kept (`main/bonusheroes.c`). Ripper Roo from Crash Bandicoot (USA) is in, with his hero card from his boss fight on the panel; that disc's other bosses (Pinstripe, Koala Kong, Papu Papu) could follow. Each hero is a file of its own (`main/heroes/`), Roo's the template the others follow: docs/EXTRA_HEROES.md goes through it | [ ] |
| 7.9 | Online play | Rollback netcode. The game's fixed timing and repeatable runs make it more realistic than in most ports, and `same` already shows a run repeats exactly. It's still a big job: saving and restoring the whole game state each frame, including its heap, and keeping sound out of the replays | [ ] |

