# Roadmap B: to do

The goal: a source-only Crash Bash, every binary rebuilt byte for byte from named, typed C split
into its original files, and the assets extracted from the user's own disc (never in git).
The work already done is in [ROADMAP-A.md](ROADMAP-A.md).

When a task is done, tick it (`[x]`), move its row to ROADMAP-A.md, and run
`tools/dk make progress` so the dashboard updates.

Parts 1 to 6 are done, the PC port's own last (2026-11-01; ROADMAP-A.md). The port was Windows only at first, at the user's request; Linux
(7.13) and Android (7.14) followed, and the Mac is off the roadmap, as the user has none (2026-11-10).

## Part 7 - More for the PC port

What the port could have beyond the game as it was. None of it may reach the decomp: it's all in `port/` (PORT.md, The rules). In order: the
quick wins for every player first, then the look, each building on the last, then the two big jobs. Mods can use what the packs need, and online play comes
last as it's the largest and any change to the game's state affects it.

Where it makes sense, each is optional: a setting in the launcher to turn it on or off, so people can play with the original or the new. That's 7.2 to 7.6, and each mod in 7.8. Turned off, the game looks and plays as it did. Easier setup and online play aren't toggles: one's a step in setting up and the other's a way to play.

| # | Task | Notes | Done |
|---|------|-------|------|
| 7.12.6 | Extra heroes in Adventure | The warp rooms, teams and saves with Ripper Roo or Pinstripe: later, if at all | [ ] |
| 7.9 | Online play | One player hosts and the others join with a join code, with no servers of our own (the user's choices, 2026-11-10). Every machine runs the whole game and only presses travel, each used a few steps late; rollback can come later on the same pieces (a bigger job: saving and restoring the whole game state each frame, including its heap). In four steps (PORT.md, Online play): 1, staying identical, done (2026-11-10): the same presses give the same game under every picture setting, by the game's fingerprint after every step (`playtest.mjs identical`), with sound mixed by the game's clock and what's drawn kept out of the game's heap; 2, online by address from the command line, done (2026-11-10: `--host`, `--join`, `platform/net.c`, `playtest.mjs online`), with two real machines on a network still to try; 4, ONLINE in the launcher, done before 3 at the user's word (2026-11-10): HOST GAME and JOIN GAME in the game's own menus, games on the same network found by themselves, and the game started again from its beginning on every machine; 3, join codes, written (2026-11-10): the router's port opened by the game (UPnP and NAT-PMP), a code of eight letters and digits for the host's address, and plain messages when hosting can't work, tested with a pretend router. Released in 1.0.0 (2026-11-10). What's left is the user's: a game between two machines on different networks. Couch play came first, at the user's word (2026-11-10), as online players would come in as controllers 2 to 4: checked with pretend controllers (`playtest.mjs pads`), with a players screen after PLAY, not yet tried by the user with real ones | [ ] |

| 7.15 | Phones that fold | The Galaxy Z Fold7 and other phones that fold (the user's word, 2026-11-11): one with a tall, narrow screen outside (the Fold7's 6.5" 2520x1080) and a nearly square one inside (8", 2184x1968), and Android moving the app from one to the other as it's opened or closed. In four steps (PORT.md, Android, Phones that fold). 1, opening and closing it mid-game: tried in the emulator's Pixel Fold (2026-11-11, the virtual device `crashbash_fold`): the game keeps running and draws at each screen's size, whether Android resizes its surface or takes it away and gives another; one stuck black screen, where SDL never asked for a new surface, is recovered from now (`gfx.c`, `reclaim_window`), though it didn't come up again to prove it. Left: a minigame and a menu through a fold on a real one, and Samsung's split screen and pop-up windows. 2, the buttons (`platform/touch.c`), done in the emulator (2026-11-11): sized by a phone's short side (420 dp) however big the screen, so the inside screen's are a phone's size and not twice it, and a screen nearer square than 4:3 is laid out as one held sideways, the shoulder buttons a phone's height above the bottom; for the user to try. 3, the inside screen's camera and the fold's crease, by the safe area as now: not looked at, as the emulator's phone has neither. 4, half open, as a laptop (Flex mode), the picture above the fold and the buttons below it, if the user wants it: Android gives the fold's place through Jetpack's WindowManager, a library the build (no Gradle) would have to take in, or its angle alone by `TYPE_HINGE_ANGLE`. Then a real one | [ ] |
