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

Where it makes sense, each is optional: a setting in the launcher to turn it on or off, so people can play with the original or the new. That's 7.2 to 7.7, and each mod in 7.8. Turned off, the game looks and plays as it did. Easier setup and online play aren't toggles: one's a step in setting up and the other's a way to play.

| # | Task | Notes | Done |
|---|------|-------|------|
| 7.12 | Polar Push's snowball | First, before the rest of Part 7. The beam turns you into a big snowball, and the snowball doesn't show correctly. Seen by the user, probably in the level where Uka melts the map (Melt Panic, going by the name). Not yet reproduced, and not known to be to do with widescreen | [ ] |
| 7.11 | Widescreen fixes, level by level | Levels stop short of what a wide view shows. Tune each by eye and by playing it, with the level's own art (`main/widescenery.c`: extra pieces, a ring of floor and wall, or a hidden piece), or leave it as it was, as Melt Panic is by the user's choice. The rows below are in the order to look at them. Black was measured in still frames at the start of each level, so they're where to look first, not a full list | [ ] |
| 7.11.1 | Dante's Dash | The extra trees look doubled, and flat beside the real ones | [ ] |
| 7.11.2 | Beach Ball | Recheck by playing: the new sand and wall | [ ] |
| 7.11.3 | N.Ballism | Recheck by playing: the front corners | [ ] |
| 7.11.4 | Crashball | Black at the sides | [ ] |
| 7.11.5 | Jungle Fox | Black at the sides | [ ] |
| 7.11.6 | Big Bad Fox | Black at the sides | [ ] |
| 7.11.7 | Ring Ding | Black at the sides | [ ] |
| 7.11.8 | Swamp Fox | Black at the sides | [ ] |
| 7.11.9 | Metal Fox | Black at the sides | [ ] |
| 7.11.10 | Drain Bash | Black at the sides | [ ] |
| 7.11.11 | Pogo Painter | Black at the sides | [ ] |
| 7.11.12 | El Pogo Loco | Black at the sides | [ ] |
| 7.11.13 | Pogo Padlock | Some black at the sides | [ ] |
| 7.11.14 | Sky Balls | Some black at the sides, and a white patch at the top | [ ] |
| 7.11.15 | Space Bash | Some black at the sides | [ ] |
| 7.11.16 | Papu Pummel | Some black at the sides | [ ] |
| 7.11.17 | Desert Fox | Some black at the sides | [ ] |
| 7.11.18 | Bearminator | Some black at the sides | [ ] |
| 7.11.19 | Toxic Dash | Some black at the sides | [ ] |
| 7.11.20 | Warp Room 3 | Some black at the sides | [ ] |
| 7.11.21 | Polar Panic | Magenta at the edge. Its night sky is probably fine as it is | [ ] |
| 7.11.22 | Tilt Panic | A white block beside its crystal arena | [ ] |
| 7.11.23 | Oxide Ride | Space is dark on purpose: probably leave | [ ] |
| 7.11.24 | Crystal maps | Not looked at yet: they're maps of their own | [ ] |
| 7.11.25 | Later in each level | Not looked at yet: all this is from the first few seconds | [ ] |
| 7.6 | Sharper icons and UI text | Icons upscale with UPSCALE (7.5). Text and digits don't: the scaler spoils them, so they stay as they were. Sharper text would need a font redrawn at a higher resolution, which can't come from the player's data and so would be new art. A texture pack can now carry one (7.10) | [ ] |
| 7.7 | Optional modern lighting | Lighting beyond the game's own, which is colours baked into its models' vertices | [ ] |
| 7.8 | Mod support | A mod loader, with a mod manager built into the launcher (`settingsmenu.c`): list, turn on and off and order mods, over the game's archive files. It goes in the launcher's Mods section, with the texture packs (7.10) | [ ] |
| 7.9 | Online play | Rollback netcode. The game's fixed timing and repeatable runs make it more realistic than in most ports, and `same` already shows a run repeats exactly. It's still a big job: saving and restoring the whole game state each frame, including its heap, and keeping sound out of the replays | [ ] |

