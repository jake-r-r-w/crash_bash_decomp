# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the dashboard updates.

Parts 1 to 6 are done, the PC port's own last (2026-11-01; ROADMAP-A.md). A Linux build stays in the port's plan (PORT.md), but isn't built, at the
user's request.

## Part 7 - More for the PC port

What the port could have beyond the game as it was. None of it may reach the decomp: it's all in `port/` (PORT.md, The rules).

| # | Task | Notes | Done |
|---|------|-------|------|
| 7.1 | Widescreen | Today a wide window shows the 4:3 picture, centred, with bars at the sides. A wide `--render` size only stretches that picture: it shows no more of the world. A real wider view needs the projection, the clipping and the off-screen culling widened (`ScreenRect`, `cam_set_projection`, `bbox_visible`), the HUD and menus laid out for it, as they place things in 640-wide units, and a look at what each level's edges show once you can see past them | [ ] |
| 7.2 | Anti-aliasing | A setting for it. The game draws to its own picture with no multisampling today, so edges are as sharp as the resolution | [ ] |
| 7.3 | Texture filtering | A choice of filtering. Textures are sampled nearest-pixel today, from one shared atlas, so filtering needs gaps around each texture or it picks up its neighbours. The original look stays the default | [ ] |
| 7.4 | High-res texture packs | Packs that replace the game's textures, found by what each texture is. The packs can't ship with the port if they're made from the game's art | [ ] |
| 7.5 | Optional modern lighting | A setting for lighting beyond the game's own, which is colours baked into its models' vertices. The original look stays the default | [ ] |
| 7.6 | Online play | Rollback netcode. The game's fixed timing and repeatable runs make it more realistic than in most ports, and `same` already shows a run repeats exactly. It's still a big job: saving and restoring the whole game state each frame, including its heap, and keeping sound out of the replays | [ ] |
| 7.7 | Easier setup | Pull `CRASHBSH.DAT` from the player's own disc image in the launcher, as well as copying it by hand | [ ] |
| 7.8 | Mod support | A mod loader, with a mod manager built into the launcher (`settingsmenu.c`): list, turn on and off and order mods, over the game's archive files | [ ] |
| 7.9 | Sharper icons and UI text | They're at the original's low resolution, whatever size the game draws at. Upscale them, with a setting to play with the original or the upscaled ones. The upscaled art can't ship with the port, as it would be the game's: it's made on the player's machine from their own data, or comes as a pack (7.4) | [ ] |
