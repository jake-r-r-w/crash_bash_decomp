# The port's copy of the game's C

Taken once from `src/main/`, `src/overlays/` and `include/` at commit cd3368b4, and refactored freely from there. Unlike
the decomp, it doesn't have to match retail's bytes, but it has to play the same (docs/PORT.md). It reconstructs the
game, so like `src/` and `include/` it isn't under this project's licence (LICENSE).

The port's own additions to it, each file saying what it does:

| File | What |
|---|---|
| `overlays/titles/settingsmenu.c` | The launcher: PLAY, ONLINE (hosting and joining), SETTINGS and QUIT, in the game's own menus |
| `main/draw.c` | The game's drawing, handed to the renderer (`platform/gfx.c`) |
| `main/videomenu.c` | The video options, for every menu that shows them |
| `main/widescenery.c`, `main/widesand.c` | Widescreen: each level's own scenery carried on at the sides |
| `main/extraheroes.c`, `main/bonusheroes.c`, `main/heroes/` | The extra heroes, made from the player's Crash Bandicoot disc (docs/EXTRA_HEROES.md) |
| `main/fingerprint.c` | The game's fingerprint after every step, which online play's machines compare |
| `main/online.c` | What's different online: each machine's own sound options, and the players' connection tags |
| `main/card.c` | The save slots as files, and `card_check`, which checks a save from another machine before the game has it |
| `main/saveicon.c` | The window's icon, from the memory card's |
