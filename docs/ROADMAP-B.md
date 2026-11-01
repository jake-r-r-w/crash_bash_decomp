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
| 7.11 | Widescreen fixes, level by level | Levels stop short of what a wide view shows. Tune each by eye and by playing it, with the level's own art (`main/widescenery.c`: extra pieces, a ring of floor and wall, or a hidden piece), or leave it as it was. The levels are listed under this table | [ ] |
| 7.6 | Sharper icons and UI text | Icons upscale with UPSCALE (7.5). Text and digits don't: the scaler spoils them, so they stay as they were. Sharper text would need a font redrawn at a higher resolution, which can't come from the player's data and so would be new art. A texture pack can now carry one (7.10) | [ ] |
| 7.7 | Optional modern lighting | Lighting beyond the game's own, which is colours baked into its models' vertices | [ ] |
| 7.8 | Mod support | A mod loader, with a mod manager built into the launcher (`settingsmenu.c`): list, turn on and off and order mods, over the game's archive files. It goes in the launcher's Mods section, with the texture packs (7.10) | [ ] |
| 7.9 | Online play | Rollback netcode. The game's fixed timing and repeatable runs make it more realistic than in most ports, and `same` already shows a run repeats exactly. It's still a big job: saving and restoring the whole game state each frame, including its heap, and keeping sound out of the replays | [ ] |

### 7.11's levels

Black was measured in still frames at the start of each level, so this is where to look first, not a full list.

To fix:

- Dante's Dash: the extra trees look doubled, and flat beside the real ones

To recheck after playing:

- Beach Ball: the new sand and wall
- N.Ballism: the front corners

Black at the sides, most first:

- Crashball
- Jungle Fox
- Big Bad Fox
- Ring Ding
- Swamp Fox
- Metal Fox
- Drain Bash
- Pogo Painter
- El Pogo Loco

Less black:

- Pogo Padlock
- Sky Balls
- Space Bash
- Papu Pummel
- Desert Fox
- Bearminator
- Toxic Dash
- Warp Room 3

Other marks:

- Sky Balls: a white patch at the top
- Polar Panic: magenta at the edge
- Tilt Panic: a white block beside its crystal arena

Probably leave:

- Oxide Ride: its space is dark on purpose
- Polar Panic: its night sky
- Melt Panic: the user prefers it as it was

Not looked at yet:

- The crystal maps, which are maps of their own
- Any level past its first few seconds

