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
| 7.4 | Widescreen | Today a wide window shows the 4:3 picture, centred, with bars at the sides. A wide `--render` size only stretches that picture: it shows no more of the world. A real wider view needs the projection, the clipping and the off-screen culling widened (`ScreenRect`, `cam_set_projection`, `bbox_visible`), the HUD and menus laid out for it, as they place things in 640-wide units, and a look at what each level's edges show once you can see past them | [ ] |
| 7.5 | High-res texture packs | Packs that replace the game's textures, found by what each texture is. The packs can't ship with the port if they're made from the game's art | [ ] |
| 7.6 | Sharper icons and UI text | They're at the original's low resolution, whatever size the game draws at. Upscale them. The upscaled art can't ship with the port, as it would be the game's: it's made on the player's machine from their own data, or comes as a pack (7.5) | [ ] |
| 7.7 | Optional modern lighting | Lighting beyond the game's own, which is colours baked into its models' vertices | [ ] |
| 7.8 | Mod support | A mod loader, with a mod manager built into the launcher (`settingsmenu.c`): list, turn on and off and order mods, over the game's archive files | [ ] |
| 7.9 | Online play | Rollback netcode. The game's fixed timing and repeatable runs make it more realistic than in most ports, and `same` already shows a run repeats exactly. It's still a big job: saving and restoring the whole game state each frame, including its heap, and keeping sound out of the replays | [ ] |
