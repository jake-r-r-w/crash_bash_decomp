# The PC port

A native 64-bit PC port of Crash Bash, built from this source on the best tech stack for a game that plays: no
emulation and no hacks. It plays every mode. This file is the design, the steps taken are in [ROADMAP-A.md](ROADMAP-A.md), Part 6, and what's next is in [ROADMAP-B.md](ROADMAP-B.md), Part 7.

A first attempt ran the game as a 32-bit program on the PS1's memory layout, with a software PS1 GPU. It was
deleted on 2026-10-30, and this plan starts again without any of that. Its code is in git
at `7115b3b8`, as a reference only. What it taught is at the end of this file.

## The rules

1. **The port has its own copy of the game's C** (decided 2026-10-30): `port/game/`, taken once from `src/main/`,
   `src/overlays/` and `include/`. It doesn't have to match anything, so it can be refactored freely.
2. **The decomp is left alone.** `src/`, `include/` and the Makefile don't change for the port, and `make check`
   stays the proof that the source is the game. The Makefile never sees `port/`.
3. **Everything PC-only goes in `port/`**, and the port never builds from `src/` or `include/`. A fix found in one
   copy goes to the other by hand, if it applies.
4. **The port is tested by playing it.** Disc to disc is byte matching; this isn't. The user plays it and says what's wrong.
5. **No Sony code in `port/`.** The game calls plain functions (`platform/`, `bash/draw.h`), not PsyQ's.
6. **No emulation and no hacks.** No PS1 GPU, VRAM or SPU model, no fixed low addresses, no linker lists that
   recreate the PS1's layout, no crash handlers that patch up bad accesses, no `-O0` to hide undefined
   behaviour. If the C relies on the PS1, the C gets fixed.

## Tooling

| What | Choice |
|---|---|
| Language | C. The game's C has to stay C to match, and `port/` stays with it |
| Build | CMake with presets, and Ninja |
| Compilers | Clang on both systems, GCC as a second check on Linux |
| Window, sound, input | SDL3 |
| Drawing | SDL3's GPU API: Direct3D 12 on Windows, Vulkan on Linux and Android |
| Checking | CTest, ASan and UBSan builds, optimised release builds |

There are no GitHub workflows: all of this runs by hand, on a machine with the game's data. Playing the
game in tests needs that data, which can't go on a public server.

## The design

**The copy.** `port/game/` starts as the decomp's C and gets cleaned up: code written only to steer the old compiler goes
back to natural C, K&R calls get prototypes, and `src/ps1/` stays out. The names and types recovered in the decomp
stay.

**Addresses in integers.** The game keeps addresses in `s32`s (about 430 casts, and more in data tables). They become
real pointers. The compiler's truncation warnings are the to-do list, and they end up as errors.

**Real structs.** The game reads some globals through others: `GameState` over `GameInfo` and the names after
it, arrays that run on into the next object, about 400 casts to byte pointers. These only work on the PS1's
layout, and they stop an optimiser being safe, so each becomes a real struct, array or field.

**Files aren't memory.** The game lays structs straight over the files it loads, and even carves heap block headers out of a
loaded file's bytes, so those headers stay 0x18 bytes and a file's 4-byte pointer slots stay 4 bytes. Self-relative offsets
(`SELF_REL`, `FILE_AT`) work anywhere. A slot that needs a pointer is a `ref32`, an offset from the heap arena: the arena is
static, so every heap and static address the game holds is within 2 GB of it. Mode text's absolute addresses are translated
when it loads (`modetext.c`), into real records and pointer tables. The saves are the port's own files (`platform/saves.c`), with no memory card: `GameState` has no
pointers, and a compile-time check holds it at the PS1's 0x18C bytes.

**What touches the hardware is small.** Of about 116,000 lines of game C, the drawing commands, VRAM calls and sound
calls come to a few hundred lines. Most of the projecting and the primitive code are in one file,
`src/main/prims.c`. The game's logic never sees the hardware, so the port replaces that layer, and the game above
it is unchanged.

**Overlays.** They all link into the program. A loaded overlay's data has to go back to its starting values,
so each overlay's data goes in a section of its own, saved at startup and restored on load. Their global
names have one clash, `XZBoxToSphere`. How a section's ends are found is each system's linker's own
(`platform/ovlsection.h`): Windows sorts a section's `$a`, `$b` and `$c` parts by name, so marks go either side;
Linux's linkers don't (the marks ended up side by side, around nothing, and the warp room stuck the second time it
loaded), but mark a section named as a C identifier with `__start_` and `__stop_` symbols; the Mac's give
`section$start$` and `section$end$` ones. GCC has no pragma to put a file's data in a section, so a GCC build links
but stops at startup.

**Null pointers.** The PS1 has RAM at address 0, so the game's null reads and writes just work there. Each one
gets a real check, found by running under the sanitizers.

**Calls that rely on the caller's registers.** On the PS1, `f()` can hand on the caller's own `a0` without
saying so. With real prototypes, these calls just pass the argument.

**The game's maths.** The fixed-point maths in the game's logic stays, because it decides how the game plays:
camera, collision and sound positions come out the same. It's plain C functions (`platform/maths.c`), with no registers,
and the same integer rules as the PS1.

**Drawing.** The port doesn't write PS1 drawing commands. The game calls `bash/draw.h` directly (`main/draw.c`), which sends vertices to the
GPU with real 3D transforms, depth and perspective-correct textures. A place in the frame's order is just a number (a layer), and a model keeps
its faces' look as plain `DrawFace` records. Widescreen leaves the game's own 512-wide screen and logic alone: the renderer shows more past each side of it (`gfx_view_extra`), and the game's culling (`clipX0`, `clipX1`) knows about it. The player panel keeps the place the 4:3 screen gives it, so the heroes' pictures stay in the middle, at the user's word: it used to go out to the sides. A menu's panel that reaches a side of the 4:3 screen ran off the screen there, as the menus' title bars and the hero select's bars do, so in a wide view it carries on past that side too (`wide_panel_sides` in `main/prims.c`), and its end never shows. It goes by where the panel is drawn, as some screens place their panels from the middle and the hero select from the left. A full-screen picture whose edge is one colour all the way down, as the Universal logo's white is, has that edge carried on across the sides (`draw_back`), just in front of the black a viewport puts there; one with more at its edge keeps black sides. Anti-aliasing, filtering, widescreen and upscaling are settings, off by default, and with them off the frames are what they always were. Upscaling (`platform/upscale.c`) is a texture made bigger as it's first drawn, by a Scale2x rule with near colours counted as the same: not AI, it adds no detail, and it never blends, so a texel's clear, semi or opaque alpha survives. No art ships: it's all from the player's own data. Widescreen shows further than some levels were built to be seen. `main/widescenery.c` has a table of extra scenery for those: copies of the level's own map pieces, moved and turned, which only a wide view draws, behind everything (`draw_behind`). The port ships positions, no art. Copies looked doubled in Dante's Dash, so it has a ring instead, a curved wall of the level's own panorama. Beach Ball's backdrop is carried on past its ends along its own curve, in its own tiles and colours (`BeachWall`), and up past the screen's top. Its sand (`main/widesand.c`) is one surface, a grid made as the level loads from its real floor's faces: the floor's own values under the floor and its edge's just past it, so there's no line where they meet, then the smoothest thing out to a height given for each column of the wall, with bumps and patches of light at the size the floor's own have. The level's kelp and coral stand on that, in the level's own draw order. Its tiles mirror from each cell to the next, as the level's own floor does, or their joins show. The real floor stops short of the wall at both back corners, where the wall would paint over the sand, so the foot of each end of the wall goes with the backdrop instead (`draw_face_list`), in real depth with the sand. The backdrop's depth has its own near (`DEPTH_NEAR_BEHIND`): with a near of 1, a float depth's steps are hundreds of units apart at a far wall, which shows as saw teeth where two surfaces cross. N.Ballism's round wall is carried on round the arena in mirror-image copies of its own last stretch, end to end (`BallismRoundRight`, `BallismRoundLeft`; `draw_mirrored`, as a mirror image's faces wind the other way), with a black plate under the floor, whose see-through texels would show them. Dante's Dash copies its swamp grass on round the lava. N.Ballism's front radars are edge on to this camera, so a wide view shows what was never meant to be seen. They're in the level's main mesh, painted before their bases and rings, which cover them, so in a wide view they all go in real depth (`draw_ballism_radars`). They stop at the back of their rims with nothing there, so they get a back, as an old television has, made of rows out from the rim in the radar's own colours (`BallismBackRows`). While a radar's firing the game swaps its rings for a lit model, with a shaft of light down the spike. Its shaft is only lit on the side the back radars show the camera, and the front ones are turned copies, so a wide view turns it a third of the way round the spike (`draw_ballism_shaft`). The front radars' rings are flat, and this view is all but edge on to them, so they'd only be needles, and a wide view makes them solid from the same hexagons, idle or lit (`draw_ballism_ring`). The flat floor under the bases goes with the backdrop, as in the level's order it sorts over them in places. Ring Ding's brick wall carries on round the sides of the tower, along the turn its ends make and then straight towards the camera, in its own tile and the colours of its ends (`draw_ring_ding_wall`), starting a little behind the wall's own ends so no crack shows where they meet. It has a copy of the grey gear under its platform on the left on the right too, in the level's own order so the platform stays in front. A shadow over its turning right cog, made by taking light away from what's drawn before it, is left out of a wide view: the cog's faces come before or after it as each one turns, so they flickered in and out of it. Pogo Painter's garden has a wall only at the back, with stone blocks for sides and nothing past them, so the wall carries on down each side from its ends, straight towards the camera, in its own tiles and row colours, with a row more on top, as the view looks out over the blocks almost level at the top of the screen (`draw_pogo_painter_backdrop`). Its grass carries on out to the walls' foot (widesand.c), and a little past the front, so the strip of black along the bottom is grass too in a wide view. Its crystal version is another map, and isn't changed. El Pogo Loco's far wall of mesas and its sky carry on at the sides as copies of themselves, side by side, as each one's two ends are alike. Its sand carries on out past the forecourt and the road at its edges (widesand.c), dipping away to the far wall's foot, so the wall's own band shows above it as in the middle. Its sand's edges are shaded dark beside the asphalt, so the new sand has a warm shade of its own a little way out (`WideSand.colour`). Far out its tile's detail fades away to its plainest texel (`WideSand.plain_far`; `draw_texture_plain` in `main/draw.c` finds the texel nearest the tile's average colour), as a textured floor seen almost side on, at the sides of a very wide view, is only stripes when its texels are sampled sharply. Its crystal version is another map, and isn't changed either. Sky Balls' wall of sky and the fields under it carry on at the sides as their mirror images across each end of the sky, a little inside it, so the sky's shading meets its own and the level's own covers the join, and beyond those the level's own moved twice as far (`WideExtra.mirror`, drawn with `draw_mirrored`). The fields' far edge is the sky's foot, the same points, so the copies of both have to move as one, or a dotted crack shows along the horizon. A copy is drawn whatever its bounding box says (the game's own flag 0x1000000, `DRAW_UNCULLED`), as the box's test goes by a sphere round its middle, and the near fields moved out to the side have their middle off the screen while their near end is on it. Papu Pummel's sunset is a wall on part of a circle, seen from above, and its far hills reach round it as far: both carry on round that circle, as their mirror images across each end of the sunset, a mirror line through the circle's middle so they stay on it, and beyond those the level's own turned on round. Desert Fox's arena is on an island of sand, with a panorama of far dunes and sky on part of a circle round it, far off, and nothing between them. Its sand is carried on out to just behind the panorama's foot all round (widesand.c), from all of its ground, of many tiles (`WIDE_SAND_ANY_TILE`), in its plain sand at the size the ground has it (`WideSand.tile_cells`). The panorama's two ends are each other's mirror image across the line through its circle's middle, so its copies turned by the span between them carry it on round, its tiles going on in their order, as they loop. Drawn as quads of their own they met it with a dotted crack and a slightly different shade, so they're copies of the level's own mesh, turned a little less than the span so it covers the join. The Bearminator's arena is in a bay of sea in a box of ice floes, with the mountains round the back of it and a wall of sky behind them, and a wide view sees out past the floes at the sides. The sky's two ends are each other's mirror image across the line through the arena's middle, so the sea, the mountains and the sky, turned round the arena by the span between those ends, carry it all on round at the sides, the sea at the mountains' foot as at the back. A face missing from the ice cliff of the floe on the right, out where a 4:3 view doesn't reach, is drawn again from its neighbours' tile and colours (`RING_IN_ORDER`, in the level's own order, with the floes' own place in it). The sniper's binoculars are a model that covers a 4:3 view, black round their two eyes, so a wide view is black past them at the sides too (`wide_scenery_cover_sides`, in their place in the order, under the HUD). Toxic Dash's room is round, with its wall round the back half and a floor of toxic waste round the track, open at the front for the camera, which is outside it, so a wide view looks out past the wall's two ends. The wall is carried on from each end straight towards the camera, as Pogo Painter's is (`SideWall`, now for any level: its rows, tiles and colours), in its own tiles in their turn and the colours of its rows, and the waste on out to the new walls' foot (widesand.c), as bright as it is where it lights the wall. The waste is a flipbook, a new frame of its tile each frame, and the sand has it too. Polar Panic's sea is only as wide as a 4:3 view sees, with steps at its sides, under a black night sky, and the misty band along its horizon stops at the sea's ends. The sea is carried on out at the sides and behind the band (widesand.c), shading on out to the band's foot as it does at the back, its far texels fading to its plain one; the band goes on as its mirror image across each end. Its eight stars are spread only as wide as a 4:3 view sees, so the sky past them was plain black: copies of them all, moved a little more than they spread each way, carry them on (mirror images would pair off across the line between, there being so few). At 1920x1000 the sea already reaches the sides, and only a copied star shows at the left edge; at 21:9 and wider the sea and stars both carry on. Tilt Panic's sea is Polar Panic's again by day, with a gradient sky wall at the back whose two ends are alike: the sea is carried on the same way (`draw_sea_backdrop`, for both), shading out to the haze at the sky's foot, and the sky goes on as its mirror image across each end. At 1920x1000 it shows when the round ends and the camera moves over to the right. Oxide's fight is in two parts. The first is a flight through asteroids over a purple haze, a backdrop model the game draws the same in front of the camera every frame (a scene mark, `DrawSceneMarks`), which stops a little past where a 4:3 view does: it's carried on at each side in the colours of its edge, fading out (`wide_scenery_mark`). The second is an arena in his ship under a camera that doesn't move, its towers round the back out of the dark: copies of the towers, without the floor (`WideExtra.faces`, the faces a copy keeps), stand on out at each side. A crystal challenge plays on a map of the level's own (its level record's other variant, `levelRecAlt`, by the 0x80 level flag); gem and relic challenges play on the normal map, so they have its fixes. Eleven of the 28 crystal maps are carried on too, at the user's choice: where a crystal map shares its normal map's scenery (Desert Fox, Ring Ding, N.Ballism's front radars) it gets the same fix, in its own colours where they differ (`BallismRadarLook`, Ring Ding's wall shade), and the rest get the same kinds of fix as the normal maps: a backdrop's mirror images across its ends (Jungle Bash, Pogo Painter, Beach Ball, Sky Balls, Polar and Tilt Panic), a turned copy (Dot Dash, the Bearminator's sky), copies of a wall panel (Pogo Padlock) and a floor carried on (widesand.c). Tilt Panic's crystal map's white block was the level's white clear colour showing under the foot of its storm dome, where its sea and horizon stopped. Jungle Fox's canopy carries on into the top corners with copies of its own leaf clusters, out past the floor's edge, where the floor can't paint over them; the rest of its dark is its own, a lit clearing that fades into the night. Crashball has nothing added, as its black is its own: a lit arena in the dark, in 4:3 too, with its far ends painted black. Its crowd's camera flashes (`Plot_ArkCameraFlash`) stay out of sight in a wide view, at the user's word, as they looked like blue sparks: they still run, so the game's random numbers and objects are as they were. But a corner of its right pipe's end was left lit past where a 4:3 view stops, so a wide view has the mirror image of the left one's end there (`draw_crashball_pipe_end`). Its comment says how to place more. A level that fills all of a 4:3 screen doesn't clear what's behind it, so past its sides old pictures stayed, and blended colours built up: `draw_viewport` starts a wide view's margins black, at the very back of the frame. Filtered (SMOOTH), the art's tile cuts would show as lines, as its tiles don't quite meet and square texels were hiding that. So `draw.c` notes which tile edges land on the same place on the screen (`note_tile_edges`), in the frame's faces, whichever mesh they're from, and `relax_edge` in `gfx.c` eases both sides of a join to the colour between them over their last 4 texels. SHARP isn't touched. Text and the panel's digits are never upscaled or filtered (`is_text` in `main/draw.c`): the user tried it, and the scaler spoils their small hand-placed pixels. ICONS: CRISP (`settings.icons`, off by default) makes the heroes' and bosses' icons sharp instead. They're 64x32, shown at about 5 screen pixels a texel at 4K, already sampled sharp: their fuzziness is the art's own soft edges. So `upscale_texels_sharp` in `platform/upscale.c` enlarges them 4x smoothly (Catmull-Rom), then takes away a blur of that (an unsharp mask), with the outline where the smooth enlargement is half covered, and a square corner, as a portrait's, kept square. Each texel keeps the kind of the one it's in: clear, semi-transparent or opaque. It's its own texture (`gfx_texture_icon`, an id of its own), and only for a flat sprite (`draw_face_quad`): the panel's frames for each hero (`IconImage`), the hero select's portraits, and a boss's own icon, which `SetPanelHeroIconB` marks (`draw_mark_icon`); a set's marks go when it's loaded again or released. A texture pack's art wins over it. The game's own lettering stays: a fan font in the Crash style was tried over the main menu, and the user preferred the original. A mod's textures (`platform/texpack.c`, from `platform/mods.c`) are PNGs, each named after the 64-bit hash of the pixels and palette it replaces: the port ships none, and they can replace text too, the one way to sharper letters. A replacement's alpha is read as the game's clear, semi or opaque. Scrolling textures are named and saved as they are at rest, and the PNG scrolls as they did. Colours painted on a model's vertices aren't textures, so a mod can't change them. Textures go from the game's indexed files to
GPU textures at load, and palettes and animated or scrolling textures use GPU resources, not a VRAM allocator. Blend
modes, fades and draw order are kept, so it looks like the game, sharper. The few effects that draw into VRAM
become render targets.

**Sound.** The sound banks and music are game assets, so they're played directly (`platform/audio.c`): ADPCM samples decoded by a
native mixer, and the SEQ music by a native sequencer. The sound device asks for samples on its own thread and the music is timed by
them, so its speed doesn't depend on the frame rate. Effects and music have 24 voices each. Reverb is a delay network with the game's own presets, fed by the tones marked for it. Checked by
ear and with `--wav`, not against Sony's code in an interpreter as first planned.

**Mods.** A mod (`platform/mods.c`; how to make one is in [MODDING.md](MODDING.md)) is a zip in a `mods` folder beside the program or in
the user's folder, or a folder there while it's being made: PNGs in place of textures, and files in place of the archive's, `files/NNN.bin`
by the entry's hex number. A zip is read where it is (`platform/zipread.c`: stored or deflated, by zlib, with every number checked against
the zip's size and every file against its CRC), so nothing is unpacked. The launcher's Mods page lists them, new ones first and on, to turn
on and off and put in order: where two have the same texture or file, the higher one wins. It looks again while it's open, so a zip put in
the folder shows up in a second. Textures change as they're chosen (the renderer starts its atlas again when `texpack_generation` changes),
and files at PLAY, for what the game reads from then on. A file can be bigger than the one it replaces: `platform/archive.c` lays the
archive out again, moving every later file on and growing its group's size, and finds each sector of a read where it is now. The code
entries are left out, as the program has its own, and a mode text file can't be bigger than its buffer (`mode_text_room`). A mod's file
can be a BPS patch to the game's own instead (`files/NNN.bps`, `platform/bps.c`), with only what it changes and the CRCs of both, so a
mod that changes the game's files needn't have them; one that isn't for the player's copy is left out. `port/mods/bps.mjs` makes them. Texture packs
from before mods, folders in `packs/`, are mods too, and the setting that chose one turns that one on. With no mods, the archive's tables
are the game's own and a read is a plain read. Test runs look in no mods folder but `--mods-dir`. The demo mod, `port/mods/gift-boxes/`,
is new art and patches (`port/mods/gift_boxes.mjs` makes it), as a mod made of the game's own files, even recoloured, can't be in git.

**Keeping it the same game.** The user plays each stage next to retail and says what's wrong. All 33 modes also play to the
end by themselves on every build, optimised and under ASan and UBSan, which catches crashes and bad memory but not a
game that plays differently.

## Building

It needs CMake, Ninja, a 64-bit Clang and SDL3. On Windows that's [llvm-mingw](https://github.com/mstorsjo/llvm-mingw).
From `port/`, with `CMAKE_PREFIX_PATH` pointing at SDL3 (or `-DBASH_FETCH_SDL3=ON` to download and build it):

```sh
cmake --preset windows-clang
cmake --build --preset windows-clang
ctest --preset windows-clang
```

The presets are `windows-clang`, `linux-clang`, `linux-gcc` and `linux-asan`, and the output is under `build/port/`.
`port/` holds `game/` (the copy of the game's C), `platform/` (the host: window, renderer, sound, maths, files), `tests/` (the play tests) and `main.c`.

The game's C isn't in the default build until it compiles. To see what's left:

```sh
cmake --build --preset windows-clang --target game -- -k 0
```

### Linux

Linux builds in a container, `tools/dl` (Docker, as `tools/dk` is for the decomp; `tools/docker/port-linux/Dockerfile`): Debian 12,
for its glibc (a program needs the glibc it was built with or newer, and this one's 2.36 makes a release that needs 2.34), with Clang
and GCC, SDL3 built in as a static library, glslang and SPIRV-Cross, and for the play tests a virtual screen (Xvfb) and Mesa's software
Vulkan (lavapipe). From the repo's root:

```sh
tools/dl sh -c "cd port && cmake --preset linux-clang && cmake --build --preset linux-clang"
tools/dl sh -c 'xvfb-run -a -s "-screen 0 1920x1080x24" node port/tests/playtest.mjs modes'
```

(xvfb-run through `sh`: run straight by `tools/dl` it can wait for ever for its screen.)

The play tests take `build/port/linux-clang/crashbash` there by default. Linux draws with Vulkan, whose shaders are GLSL compiled ahead
to SPIR-V (`port/platform/shaders/`): a change to the HLSL in `gfx.c` gets made in `gfx.vert` or `gfx.frag` too, and
`tools/dl sh port/platform/shaders/make.sh` makes the headers `gfx.c` includes again. The container's pictures are lavapipe's, a pixel
here and there from Direct3D's, so `same` keeps a baseline per system.

### The Mac

Not built, and off the roadmap: that needs a Mac, as Apple's SDK is licensed for Apple's own computers only, and the user has none
(2026-11-10). The code stays ready for it as far as it can be from here: the renderer runs on Metal there (`gfx.c`, `GPU_DRIVER`), with the MSL that SPIRV-Cross makes from the Vulkan shaders
(`make.sh`, so all three systems draw with one shader source; the Linux build compiles the Metal path too, but can't run it), and the
`macos-clang` preset. On a Mac with Xcode's command-line tools, CMake and Ninja (SDL3 from Homebrew, or fetched):

```sh
cd port && cmake --preset macos-clang -DBASH_FETCH_SDL3=ON && cmake --build --preset macos-clang
```

What's left for it: building and playing it there, then a release (one program for Apple silicon and Intel, signed if it's to open
without macOS's warning) and the README's Mac parts in `package.mjs`.

### Android

Android builds in a container of its own, `tools/da` (`tools/docker/port-android/Dockerfile`: Debian 12, Google's SDK command-line tools,
then with them a platform to build against, the build tools and the NDK, a JDK, and SDL3 built with the NDK for arm64 and x86_64).
`tools/da sh port/android/build.sh` makes `build/port/android/crashbash.apk` with the SDK's own tools, no Gradle: the game as
`libmain.so` beside SDL's `libSDL3.so` for each processor, SDL's Java side with the port's activity (`port/android/java/`, which runs the
game's `main` with the arguments a launch gives it), the manifest and icon, signed with a debug key unless `$ANDROID_KEYSTORE` names
another.

What's Android's own:
- It draws with Vulkan, as Linux does, and turns with the phone: the picture fits either way round.
- Unless told not to, SDL asks a Vulkan device for four features many phones haven't got, and turns down one without them: the first
  real phone said "Crash Bash needs a graphics card with Vulkan" (2026-11-10). The renderer turns off the three it doesn't use (clip
  distances, indirect draws' first instance, anisotropic filtering) everywhere, and if no device has the fourth, depth clamping, it
  takes one without and clips depth instead (gfx.c, `make_device`; `--depth-clip` draws that way on any device). Every mode's shots
  are the same either way on Windows and Linux. With still no device, the message and `log.txt` name each GPU and what it lacks of what
  SDL still needs (`vulkan_lacks`).
- The program's folder (`platform_base_path`) is the app's in shared storage, `Android/data/io.github.jakerrw.crashbash/files/`, which a
  PC reaches over USB: `CRASHBSH.DAT`, a disc image, `discs/` and `mods/` go there, and the game writes its output to `log.txt` there.
  The game makes `discs/` and `mods/` itself at startup: Android doesn't let an app look in a folder something else made in its own
  storage (one adb made was "No such directory" to it), though it can read the files in it. Saves and settings are in the app's private
  storage.
- The first run asks for the disc image with Android's file chooser: the .bin, or an .iso (a .cue's .bin can't be found from it there).
- WIDESCREEN is on by default, so a phone held sideways is filled (the user's word, 2026-11-09).
- On-screen buttons (`platform/touch.c`), drawn over the picture on the window itself, so never in a test run's shots: a d-pad, the face
  buttons, the shoulders, Select and Start, thin and see-through as phones' games have them (the user's word: "they should not get in
  the way of the game"), fading to under half after 4 seconds untouched. What a finger presses is bigger than what's drawn. Upright
  they sit below the picture. They show from the start, go when a controller or keyboard is used, and come back at a touch (anywhere, a
  PC's touch screen too).
- The back button is Triangle.
- The game ends its process at once (`_exit`): an `exit()` crashed in Android's UI thread on a lock it had torn down, and Android may
  start the app again in a process it kept, with all of the game's state as it was.

Testing is in Android's emulator on Windows: the SDK in `%LOCALAPPDATA%\Android\Sdk`, which needs Windows Hypervisor Platform turned
on, and its virtual device `crashbash` (a 2400x1080 phone, the AOSP Android 15 x86_64 image), started with `-no-metrics`. Then:

```sh
adb install -r build/port/android/crashbash.apk
adb push CRASHBSH.DAT /sdcard/Android/data/io.github.jakerrw.crashbash/files/
adb shell am start -n io.github.jakerrw.crashbash/.CrashBashActivity --esa args --mode,1,--frames,900,--shot,600,/sdcard/Android/data/io.github.jakerrw.crashbash/files/m1.png
```

In Git Bash `MSYS_NO_PATHCONV=1` has to be set, or it turns the `/sdcard` paths into Windows ones.

A release is signed with the release key, which signs every Android release after it too, as Android only installs an update over an
app signed by the same key. It lives outside the repo (on the user's PC, `.android-keys/crash-bash/` in their home folder: `release.jks`,
alias `crashbash`, and its password in `release.pass`; its certificate's SHA-256 begins `8DE18A0A`), and `tools/da` mounts it only when
asked: `CRASH_BASH_ANDROID_KEY=FOLDER tools/da node port/package.mjs --android` makes `crash-bash-port-VERSION-android.apk` in
`build/port/release/`, checks each processor's `libmain.so` as the other downloads' programs are checked, and says which certificate
signed it. A test build is signed with a debug key instead, so going from one to the other in the emulator needs an uninstall first.

The play tests run there too, with `--android` (`node port/tests/playtest.mjs modes --android`): one run at a time, each launched by adb
with its arguments, its shots fetched from the app's folder and its result read from its `log.txt` and Android's crash log, as an app has
no exit code to give. The emulator can run without a window for them (`-no-window`), out of the way.

Phones are arm64, where a function returning a small type (u8, u16, bool) needn't clear the rest of its register, and the emulator is
x86_64, so it can't show a call through a pointer of the wrong type. The game's C turns off Clang's function-pointer warnings, so a
check builds it for arm64 with them on (`tools/da sh port/android/check-fnptrs.sh`): the calls that read such a result
go through wrappers of the right type now (the grab hooks, which returned a pointer read as an s32, and two of Polar's AI tests, a bool
read as an s32), and what's left differs only in arguments of the same size, or results nobody reads.

## A release

`node port/package.mjs`, with `TC` set to the toolchain, builds `build/port/release/crash-bash-port-VERSION-windows-x64.zip` (up to
0.3.2 they were `crash-bash-pc-...`; renamed at the user's word, 2026-11-09, the published ones left as they are): the program
with no console window and no symbols, its README (`port/release/README.txt`) and the licences. The version is the `project()` one in
`port/CMakeLists.txt`. It fails if the program holds a sample of the game's archive or this machine's paths, and it publishes nothing. Try
the zip with `playtest.mjs ... --exe`, and unzipped by itself, with and without `CRASHBSH.DAT`. The release's notes are in `port/release/`,
`NOTES-VERSION.md`, and a GitHub release is called "Crash Bash Port VERSION", as its notes' heading (the user's name, 2026-11-09; the
first six were "Crash Bash for PC", and were renamed): `gh release create vVERSION ZIP TAR.GZ --title "Crash Bash Port VERSION"
--notes-file port/release/NOTES-VERSION.md`.

**Icons.** The window's icon is the PS1 game's memory card icon, the first of its two 16x16 frames in the font's textures, made from the
player's own data as the game starts (`game/main/saveicon.c`) and scaled pixel for pixel to the size the system asks for (`host_set_icon`).
What a download carries itself, the Windows program's icon (`platform/crashbash.rc`) and Android's launcher icon, is a drawn one
(`port/release/icons.ps1` makes both), as the save icon is the game's art (the user's word, 2026-11-09).

`tools/dl node port/package.mjs` builds the Linux one, `crash-bash-port-VERSION-linux-x64.tar.gz` (preset `linux-release`): the program
with SDL3 and zlib in it, so it needs only what every Linux has, as SDL opens the system's own window, sound and Vulkan libraries as it
runs. Its README says the oldest glibc it runs on, from what the program imports. The README is one for both, with `@NAME@` parts that
`package.mjs` fills for each system.

## Running it

Run `build/port/windows-clang/crashbash.exe` from the repo's root, where it finds the game's data in `build/disc/`. It also looks for `CRASHBSH.DAT` in the current folder, next to the program and in the user's folder, where a disc image's copy goes the first time it runs (`platform/discimage.c`): an image next to the program or in the current folder is used with no questions, and otherwise it asks. A settings
menu opens first, one of the game's own menus (`game/overlays/titles/settingsmenu.c`), and what's chosen is kept in the user's folder (`platform/settings.c`).
Keys at first: arrows, Z = Cross, X = Circle, A = Square, S = Triangle, Enter = Start, Right Shift = Select, Q and W = L1 and R1, 1 and 2 = L2 and R2.
SETTINGS has a CONTROLS page to change them, and a controller's buttons (the same for every controller), at the user's word
(2026-11-10; as simple as it can be, also theirs): a row for each of the game's buttons, showing what plays it on what the player's
using, its key or, once a controller's had the last press, its controller button (`host_using_gamepad`), as modern games' prompts
follow the controller. A row, Cross, then whichever's pressed first, a key or a controller's button, is its new one; one another button
had swaps with it, so each has one; Esc, or a few seconds with nothing, keeps it; RESET TO DEFAULTS puts them all back. They're kept in
the settings file by SDL's names (`key_cross=Z`, `pad_cross=a`; `platform/controls.c`). The face buttons show as the font's pictures of
them (`\xFC` Cross, `\xFD` Circle, `\xFF` Square, `\xFE` Triangle), and so do a PlayStation controller's. Two columns, keys and
buttons, were tried first and dropped by the user: the game's letters aren't one width, so a second column needs text records of its
own, and slot 0's 32 are all taken. `playtest.mjs controls` checks it with pretend keys.

The menu is PLAY, SETTINGS and QUIT, with the settings in categories; a new setting is a row in its category's table and a case in
`change_setting` and `show_value`. The main menu's EXIT comes back to it.

**Controllers.** Each controller is a player's, as each was plugged into a port on the PlayStation: player 1 has the keyboard, the
first controller and on a phone the touch buttons, and each other controller is the next player, a third or fourth making the game
see a multitap (`pads_read`). The game's own players menus offer as many players as there are controllers in a row from player 1, as
the PlayStation's did. A controller pulled out leaves its player with none and nobody else's changes; plugged in again it's that
player's again, and a new one takes the first player with none (`platform/sdl.c`, by each controller's GUID, name and serial). With two
or more, PLAY opens the players screen first (the user's idea, 2026-11-10): each controller's name in its player's row, moved up or
down by pressing up or down on that controller, its row lit and the controller buzzed so its player sees which it is; START plays and
Triangle goes back (`settingsmenu.c`, `controllers.h`). `playtest.mjs pads` checks all of it with pretend controllers.

For testing, any run with `--frames N` ignores the saved settings and skips the menu, and its window opens on a second monitor if there is one:

| Option | What it does |
|---|---|
| `--image FILE` | copy the game's data from this disc image first, to `--data`'s path if it's given, and otherwise the user's folder |
| `--antialias N`, `--smooth`, `--widescreen`, `--upscale N`, `--sharp-icons` | those settings, for this run (the last is ICONS: CRISP) |
| `--level-flags HEX` | with `--mode`, a challenge's flags as a door would set them, as a one-player game: `80` is a crystal's, on the level's other map. It opens on the rules, so press through them; `--bot` plays it |
| `--mods-dir DIR`, `--mod NAME` | look for mods only in DIR, and turn one on (its zip's or folder's name), for this run |
| `--texture-pack DIR` | a folder of PNGs as a mod that's on, a path or a name under `packs/`, for this run |
| `--dump-textures DIR` | save each texture as it's first drawn, named as a mod needs it |
| `--dump-files DIR` | save the archive's files, all but its code, named as a mod's `files/` has them, and stop |
| `--log-files` | print which archive files each read is of |
| `--frames N` | stop after N vblanks |
| `--skip-intro`, `--skip-loading` | the menu's two skips, for this run |
| `--fps 30\|60\|120` | the frame rate, for this run |
| `--turbo` | don't wait for the clock, to reach late moments quickly |
| `--pretend-pads N` | plug in N pretend controllers (SDL's virtual gamepads, "Pretend pad 1" on), and use no real ones; up to four can come and go |
| `--pad-press P N BUTTONS` | hold pretend controller P's buttons (1 to 4; hex, `platform.h`) from vblank N for a few vblanks |
| `--pad-unplug P N`, `--pad-plug P N` | pull pretend controller P out, or plug it in, at vblank N |
| `--key-press N KEY` | hold a pretend key (SDL's name: `J`, `Escape`, `Right Shift`) from vblank N for a few vblanks, through the CONTROLS page's keys as a real one is |
| `--depth-clip` | draw as a device without depth clamping does (some phones): a point outside the depth range is clipped, not drawn at its edge (gfx.c, `make_device`) |
| `--draw-every N` | draw one frame in N, and any a shot needs, so a long test runs as fast as the game's logic can |
| `--heroes A B C D` | with `--mode`, the four players' heroes, 1 to 8 (0: the demo's own): 1 Crash, 2 Coco, 3 Cortex, 4 Brio, 5 Tiny, 6 Koala Kong, 7 Dingodile, 8 Rilla Roo |
| `--mode N` | go from the main menu straight to game mode N, played by the computer, with TESTING on screen and no time limit. It's the game's demo, whose pause bit stops what waits on a pause; Polar Push's tilt doesn't wait on that bit (the game never demos its tilting levels). Mode 43, the tournament's podium, is as at a tournament's end: Crash, Coco, Cortex and Brio (or extra heroes in their places), player 1 first |
| `--tile I N` | put this window in slot I of a grid of N on the screen, to watch several test runs at once |
| `--wav FILE` | record a test run's sound (test runs are silent otherwise) |
| `--sound` | play a test run's sound, for one that's being watched |
| `--game-clock-sound` | mix the sound a vblank at a time by the game's clock, as online play does (Online play, below), not as the sound device asks |
| `--fingerprints N` | print the game's fingerprint every N steps (`game/main/fingerprint.c`; Online play, below) |
| `--host [ADDRESS:]PORT`, `--join ADDRESS:PORT` | host an online game, or join one (Online play, below); `--net-joiners N` is how many machines the host waits for (1) |
| `--net-delay N`, `--net-patience MS` | an online game's delay in steps, and how long the host waits for a machine that's stopped answering |
| `--net-lag MS`, `--net-loss N`, `--net-differ STEP`, `--net-vanish STEP`, `--net-stall STEP MS` | tests of online play: late and lost packets, a machine that disagrees, vanishes or stalls |
| `--net-record FILE`, `--replay FILE` | keep an online game's presses, step by step, and play them again with nobody else |
| `--macro-until N` | `--macro`'s pattern stops at vblank N |
| `--then` | what follows is for the game's next start (an online game from the launcher starts it again): presses, shots, `--frames` and `--turbo` |
| `--restart-on-play` | the launcher's PLAY starts the game again from its beginning, as an online game's START does |
| `--online-local`, `--online-port N` | the launcher's online games are this PC's alone, and on that port |
| `--online-pretend-router open\|refused\|none\|shared` | hosting asks no router for a port, and is told that |
| `--check-router` | ask the router for a port as HOST GAME does, a step at a time, saying what each step found, and close it again: for when hosting says the router won't |
| `--make-code ADDRESS N`, `--check-codes` | print the join code for an address and port N of the eight; check codes are made and read right |
| `--controllers none|N`, `--no-sound` | for several copies of the game on one PC: this copy uses no controller, or only the Nth found (from 1), read even when its window isn't in front; and plays no sound |
| `--shot N FILE` | save vblank N as a PNG |
| `--press N BUTTONS` | hold pad 1's buttons (hex, `platform.h`) from vblank N for a moment |
| `--no-launcher` | start with the saved settings, without the menu |
| `--launcher` | open the menu in a test run too (drive it with `--press`) |
| `--window W H`, `--render W H` | the window's size and what the game draws at, for this run (`--render 0 0`: match the window). A size the screen can't hold, title bar and all, opens maximised |
| `--cheat-menu` | the cheat menu on, for this run |
| `--macro N HEX:VBLANKS,...` | a repeating pattern of buttons on pad 1 from vblank N. A test run logs a boss fight's health |
| `--stall N FILE` | stops a boss fight that's made no progress in N vblanks, with a shot in FILE; the log, written as it goes, says where it stuck |
| `--aim` | a test run's player for the boss fights (`port/tests/bossbot.c`): it looks where things are and only presses the pad's buttons |
| `--bot`, `--cheats HEX` | the game's own AI plays pad 1 and its fight's state is logged; cheat flags kept on (`shell.h`, `cheatflags`: 2 is no death) |
| `--boss-beat N` | marks the boss beaten at vblank N, to test what follows. A test run also prints each change of game mode |
| `--stick N LX LY` | a test's analog pad with motors on pad 1: its left stick at these (hex, centre 80) from vblank N for a few vblanks. A test run prints the rumble it asks for |
| `--no-window`, `--data`, `--saves` | no window, and where the game's data and the saves are (a test run keeps no saves without `--saves`) |

**Play tests.** `port/tests/playtest.mjs` plays the real game through its pad, by Node on the host, with your own game data, and says what passed:

```sh
node port/tests/playtest.mjs bosses
node port/tests/playtest.mjs modes
node port/tests/playtest.mjs adventure
node port/tests/playtest.mjs same
node port/tests/playtest.mjs menu
node port/tests/playtest.mjs mods
node port/tests/playtest.mjs heroes
node port/tests/playtest.mjs hero roo
node port/tests/playtest.mjs identical
```

`bosses` beats every boss from a real Adventure, by each fight's own rules, and back to the warp room; `--watch` shows them at normal speed with sound, and `--hero roo` plays them as an extra hero (`--extra-hero 0 roo`). `modes` runs every game mode, played by the
computer, with `--asan` for the AddressSanitizer build. `adventure` plays a new Adventure with no cheat flags, door by door, until there's nothing left to win, saves at the computer, loads that save in a new run, and passes at 200%. A challenge it keeps losing gets weaker computer heroes and in the end ones that stand still, and the log says which. `--resume` carries on from its last save, and `challenges` plays any challenge alone. `menu` goes from the title screen into a game by the pad alone. `packs` and `mods` check textures and files from mods by the pictures of one mode: zips stored and deflated, a zipped folder, which of two wins, broken zips, and archive files at the same size, bigger and moving the rest. `shots FROM STEP COUNT` makes a contact sheet of one run. `heroes` checks in a few seconds that the extra heroes load: one short game
with them as the players, each made from its disc and drawn as itself. That's all a release or a change elsewhere checks of them (the
user's word, 2026-11-09). `hero ID`, for work on that hero, plays it in every game, four of it at once, and checks from the log that each game draws it in a model of its own of that game's kind, with a contact sheet of them (docs/EXTRA_HEROES.md, the checklist); `same --hero ID` compares its pictures and sound with a baseline of its own. `identical` checks that the game repeats itself exactly, for online play (Online play, below): the same presses under each picture setting, by the game's fingerprint after every step, in about two minutes, with `--all` for every game mode too and `--against FILE` for another system's fingerprints. `online` plays whole online games with every machine a copy of the game on this PC (Online play, below). What they leave is in
`build/port/playtest/`.

**Frame rates.** The game's logic runs at about 30 frames a second (2 vblanks a frame) and stays there, because its movement is per frame. At 60 and 120 the renderer keeps the game's last two frames and draws steps between them, so what's shown is one game frame behind. Triangles are matched by name: `draw_owner` says which object is drawing, and a face number or the order within that object does the rest (`gfx_mark`). What has no owner, like some 2D, just appears. A triangle that jumps more than a quarter of the screen isn't moved. The window keeps being maximised as `maximized=1` in the settings, not as its size, and the menu's WINDOW SIZE goes through the sizes that fit on the screen, then MAXIMISED (`step_window`). With vsync on, a picture past the screen's rate would wait for the screen and hold up the game's clock, which ran the game at half speed at 120 on a 60 Hz screen, so well above its rate a share of the pictures is left out (`picture_fits`).

**Depth.** The game sorts by bucket, as a PS1 must. Solid parts of `draw_object`'s models also test real depth against each other, so a moving model's faces don't swap. The map, effects and anything semi-transparent or with a layer of its own stay in the game's order alone.

## Online play

Roadmap 7.9, begun 2026-11-10. The user's choices: one player hosts and the others join with a join code, with no servers of our own,
as plug and play as it can be. The first version is the simple kind: every machine runs the whole game, only presses travel, and each
press is used a few of the game's steps late (it steps 30 times a second, whatever the FPS), so that it reaches everyone in time.
Rollback can come later.

That only works if the game repeats itself exactly from the same presses, on every machine, whatever that machine's own settings.
Step 1 was making sure of it, and it does (`playtest.mjs identical`).

**The fingerprint** (`game/main/fingerprint.c`). After every step (`shell_update`), the game's own values are folded into a number:
the random numbers' state, the game's record (`GameState`) and frame counts, each hero's numbers, every object in play (which, what
kind, its state, where), the heap's free room, the pads as the game read them, and which voices are playing. Not raw memory, whose
addresses differ between machines: nothing in it is a pointer, and what's in the heap goes in by its place in the heap. A test run
prints it with `--fingerprints N`, a line of parts, so the first part to differ says where to look; `all` is every step's so far,
which is what machines will compare online.

**What stays each player's own**, as it doesn't change the game: FPS, widescreen, bigger textures, smoothing, anti-aliasing, sharp
icons, the window's and the picture's size, and whether it runs in real time or behind. `identical` plays the same presses from two
pretend controllers through the menus into a Battle under each, and its `--all` plays every game mode under all of them at once, in
real time with the sound played: every step's fingerprint matches. Windows's and Linux's match each other too (`--against`).

**What has to be the same for everyone**, as it does change the game (the test says what each does): the cheat menu
(`GameState.cheats`), SKIP LOADING and SKIP INTRO (the steps a load or the logos take), EXTRA HEROES (a ninth card, and what's
loaded) and mods that change the game's files. Online these are the host's. Mods and extra heroes are files, and an extra hero comes
from each player's own disc, so they're to be compared at joining, not sent.

Two things had to change for it:

- **Sound by the game's clock** (`settings.h`, `game_clock_sound`; `audio.c`, `snd_vblank`). The game reads back which voices are
  still playing (`audio_voice_states`, read by `sfx_update_voice`), and a new sound takes the first free voice. Offline the sound
  device's thread mixes, at the sound card's pace, so a voice ends a step sooner on one machine than another: the test's run mixed
  that way parts from the others within a thousand steps. Online the game mixes a vblank's worth at each vblank, as a test's
  recording (`--wav`) always has, and the device plays from a queue of it (`sdl.c`, `host_audio_queue`): about 50 ms deep, held
  there by playing a touch faster or slower (up to 1%), as the game's clock and the sound card's never quite agree. Offline play
  is as it was. A silent test run mixes nothing at all, so its voices never end: fingerprints are only comparable between runs that
  mix (`--game-clock-sound` or `--wav`).
- **The draw heap** (`common.h`, `DRAW_HEAP_BYTES`; `prims.c`, `face_prim_alloc`). The PS1 game allocates each mesh's faces from its
  one heap as the mesh is first drawn, and frees them when it hasn't been for a while. A wide view draws more, so the heap was a
  different shape with widescreen on, which the fingerprint caught. Those faces have a heap of their own now, the last 16 MB of the
  arena, and the game's heap is the same whatever is drawn. Frames and sound didn't change (`same`).

### ONLINE in the launcher (step 4)

The launcher's top menu is PLAY, ONLINE, SETTINGS and QUIT (`game/overlays/titles/settingsmenu.c`), and ONLINE has HOST GAME and
JOIN GAME, in the game's own menus, as the players screen is:

- **HOST GAME** opens a game on port 47000 (`NET_PORT`) and lists who's here: this PC's players, and each machine that's joined, by
  its address and ping. Under it is the address for the others to join at, this machine's on its network. START (or Cross) begins
  the game once someone's joined; Triangle stops hosting. The first time, Windows asks whether to let the game through its
  firewall, and it has to be allowed.
- **JOIN GAME** lists the games being hosted on this network, which show by themselves: every second it asks the whole network who's
  hosting (`T_FIND`, a broadcast), and each host with its lobby open answers. Cross joins one. Under them is ADDRESS, for a host
  that isn't on this network or didn't show: digits and dots from the keyboard, or with a controller up and down for the last
  character, right for another and left to take one off. It starts with this machine's own address as far as its last dot, as the
  host's is most likely beside it. Joined, it waits for the host's START.
- What goes wrong is said under the menu: nobody hosting at that address, another version of the game, different game data, a full
  game, the host gone.

**The start.** A game played on from the launcher differs the longer the launcher was open: it's one of the game's own menus, and
the random numbers step in it too (`shell_update`). So START starts the game again from its very beginning on every machine, in the
same window (`platform_online_start`, `platform_restart` in `main.c`):

- All of the game's own data is in a section (`ovl_game`: main's and the engine's; each minigame overlay already had its own),
  saved as the program starts and put back (`platform/overlays.c`, `overlays_restart`), and the heap arena, which is the platform's
  and outside that data (`arena.h`), is emptied. That's everything the game has: its C has no other state.
- What the platform keeps for the game is put back as it was at the start: the sound (`audio_restart`: nothing open or playing, the
  mix's counts at nought), the clock's vblank counts, the pads' motors, and the textures it has seen. The sound device changes to
  being fed by the game's clock, and back afterwards (`host_audio_by_clock`).
- Then the game's first function is called again (a `longjmp` back to `main`).

`playtest.mjs identical` checks it's the same game a fresh start gives, step for step, however long the launcher was open
(`--restart-on-play` makes the launcher's PLAY restart so). The same sections and arena are what a snapshot of the whole game
would be made of, for rollback later.

**The end.** The main menu's EXIT, in an online game, takes each machine back to its own launcher, offline: the game starts again
from its beginning once more, with the player's own settings, saves and mods back (`platform_online_stop`). So does the host
leaving or stopping answering, a mismatch, or falling too far behind, and the launcher says which under its menu. A game from the
command line ends the program instead.

**The ping** that sets the delay is timed as the host presses START, for a quarter of a second in which the host and the joiners
do nothing else (`time_pings`): in the lobby each side only looks at what's come once a vblank, which would add up to two of them
to every ping and a step to the delay.

**A test's presses** are for one start of the game or another: those after `--then` on the command line are for the next start,
whose vblanks count from 0 again. `--turbo` after `--then` is flat out from that start on, so a test can go through the launcher
in real time, as finding and joining need, and then play fast. `--online-local` keeps the launcher's games to this PC (hosted at
127.0.0.1 and looked for there, so Windows doesn't ask about the firewall), and `--online-port N` puts them on another port.

### Join codes (step 3)

For a game between homes. A home's router lets nothing in from outside that wasn't asked for from inside, so a game hosted at
home can't be joined from another home until the router passes a port on to the host's machine.

**The router's port** (`platform/portmap.c`). HOST GAME asks the router for it, by UPnP (most home routers, where it's turned
on) and, failing that, NAT-PMP (Apple's and some others), through miniupnpc and libnatpmp: small BSD-licensed libraries, fetched
at one version each and checked by hash as the build is set up (`CMakeLists.txt`), and built into the program. It asks on a
thread of its own, as finding the router takes a couple of seconds and the launcher is still drawing. The port outside is the
first free of eight in a row from 47000, for two hours, so a game that ends badly doesn't leave it open for good; it's closed
when the game's over or the host backs out. The router says what its own address is, and that's the one the others join at.

**The code** (`net_code_make`, `net_code_read`). The host's address and which of the eight ports are 35 bits, and with 5 more
to catch a slip they're eight characters of 32, written `XXXX-XXXX`: the digits and the letters without I, L, O and U, so none
is read as another (an O typed is taken as 0, an I or L as 1). Every one-character slip is caught. The bits are stirred first,
so the codes of addresses that start alike don't. The host's screen shows it once the port's open, and JOIN GAME's CODE row
takes it, typed, or picked with a controller (up and down for the last character, right for another, left to take one off). While
it's typed a key is a character and not the button it's set to: START joins and Esc goes back. That row takes an address too. A
code is the host's address in another form, so it's for people the host would give that to.

**When it can't work**, the host's screen says which, and what can be done, a page of each in turn:

- Nothing answered: there's no router that takes such requests, as on a phone's hotspot or a shared network, or it's turned off
  on it. Players on the host's network can still join, at the address shown; the host can still join a game someone else hosts;
  or everyone can use a virtual network such as Tailscale.
- A router answered and wouldn't: the same, and UDP port 47000 can be opened on it by hand (the others then type the host's
  address as the world sees it).
- The router's own address isn't one the world can reach (a private one): the provider shares one address between homes, as
  phone networks and some broadband do, or there's a second router beyond this one. No port opened here would help, so someone
  else has to host, or everyone uses a virtual network.

**No servers** is the user's choice, made at the start and again (2026-11-10) once they'd met the case it costs: other games' codes
work from any network because everything goes through servers their makers run or rent, and here a host on a phone's hotspot or
a shared address can't be reached at all. A relay of our own would fix that and was turned down; such a player joins, and someone
else hosts.

**Virtual networks** (Hamachi, Radmin VPN, ZeroTier, Tailscale) are the way round a network that can't host: each player installs
one and joins the same network in it, and their machines are then on one network as far as the game can tell, through that
tool's own servers. The host hosts as ever; nothing is asked of any router that matters. JOIN GAME asks who's hosting on every
network the machine is on, each adapter's by name (`everyone_on_each`), not only the main one's, so a game on a virtual network
shows by itself like one at home: tried here with a game hosted only on a Hyper-V adapter's address, which was found, joined and
played. That needs the tool to carry broadcasts, which Hamachi, Radmin VPN and ZeroTier do; Tailscale doesn't, and there the
host's address in it is typed in the CODE row. Not tried with any of those tools themselves. Windows treats such a network as a
public one, so its firewall question has to be allowed for public networks too.

**For tests** no router is ever asked: `--online-local` games don't, and `--online-pretend-router open|refused|none|shared` gives
that answer after a moment. `--make-code ADDRESS N` prints the code for an address and port N of the eight, and `--check-codes`
(a `ctest`) makes and reads codes and tries every slip in some.

### By address, from the command line (step 2)

`platform/net.c`, plain UDP sockets. One machine hosts and the others join it by its address:

```sh
crashbash --host 47000 --net-joiners 2      # waits for two machines to join, then starts
crashbash --join 192.168.1.20:47000         # on each of the others
```

The launcher's ONLINE does the same with screens (above); this way is for tests, and the window stays black till everyone's there,
with what's happening in the program's output. `--host ADDRESS:PORT` hosts on one address only (`127.0.0.1` for games on one PC,
which Windows doesn't ask about its firewall for).

**On one PC.** `node port/tests/playtest.mjs online --play` starts two copies of the game (or `3` or `4`) side by side, each at its
launcher with the player's own settings, to host in one and join in the others: the first has the keyboard and the sound, and each
other a controller (`--controllers`), so with the first window in front the keyboard is one player and a controller another, both
at once; another copy's window, clicked, takes the keyboard. Their games are this PC's alone (`--online-local`). `--delay STEPS`,
`--lag MS` and `--loss N` are for feeling what a game over the internet would be like. The host keeps the game's presses in
`build/port/playtest/online-play/game.txt`.

**Players.** Each machine brings its own: the keyboard's, and one for each controller after the first, as offline. The host's are
the first players, then each machine's in the order they joined, up to four. Player 1 drives the menus, as on the PlayStation.

**Steps.** A step is one read of the pads by the game (`pads_update`, 30 a second; `platform_pad_step`). At each, a machine takes
its own pads as they are, all of what the game reads of a pad (`pad_poll`: whether it's there, its buttons, its sticks if it has
any, whether it rumbles), and sends them for the step DELAY steps on. The host puts every machine's together into that step's
record, the four pads as the game will read them, and sends it to everyone; a machine plays a step once it has its record, and
waits if it must. So the host decides what each step's presses were, and nobody can disagree. A machine only ever talks to the host,
so the host's is the one address anyone needs. Every packet carries all that the other side hasn't said it has, up to 32 steps, so
a lost one is made up by the next; and what's arrived is looked at every vblank, not only when the game wants it.

**The delay** is set once, by the host, from the longest ping to a joiner (`start_host`: the ping and a fifth, 25 ms, and a step
over), and is the same for everyone, the host too: 1 step on one PC or a home network, about 3 at 60 ms there and back.
`--net-delay N` sets it by hand.

**The start.** Every machine starts the game at its very beginning, as a test run does, and the first step can't be played till
everyone's first pads are in, so they start together. Before it, a joiner is checked (the same version of the program, the same
game data, by its size and first part) and sent the host's four save slots, which it plays with in memory, its own files left alone
(`saves_in_memory`), and the host's settings that change the game, for this game only (`settings_borrow`: the player's own are still
what's saved). Extra heroes and mods' files are off for everyone online for now, as there's no telling yet that everyone has them;
mods' textures are each player's own.

**Checking.** Once a second the machines swap the game's fingerprint, and if a joiner's and the host's differ the game stops on
every machine and says so, as from then on they'd be playing different games.

**A machine that stops answering.** The host waits 5 seconds for it (`--net-patience MS`), then plays on without waiting: that
machine's pads are as they last were, with nothing held, so its hero stands there. (Pulled out, the game would pause for the
controller to come back, as it does offline, and everyone would wait on a machine that may never.) If the machine was only stalled,
as when its window's dragged, it plays the steps it missed flat out, and once its pads arrive in time again the host waits for it as
before. A machine more than a minute behind is told it's out. A joiner that leaves says so, and the others play on at once. If the
host leaves or stops answering (10 seconds), the game ends for everyone: a joiner plays out the steps the host had sent, then says
so.

**Keeping close.** Each machine plays by its own clock. A joiner a whole step behind the host plays on without waiting for its
clock till it's caught up (`net_behind`), as its pads would arrive that much later and the host would wait for them.

**For tests.** `--net-lag MS` and `--net-loss N` make this machine's packets late and lose N in a hundred; `--net-record FILE` keeps
a game's records, with its saves and settings, and `--replay FILE` plays them again with nobody else, which has to give the same
game; `--net-differ STEP` makes this machine's fingerprints wrong from a step, `--net-vanish STEP` ends it without a word, and
`--net-stall STEP MS` stops it for a while. `playtest.mjs online` plays whole games with every machine a copy on this PC: two and
four machines from the menus through a Battle's rounds and back to the title by the pause menu, flat out with packets late and
lost, matched step for step by fingerprint; a recorded game played again; a machine that disagrees; and in real time, late and lost
packets, a machine that vanishes and one that stalls and is back.

The steps: 1, staying identical (done); 2, online by address from the command line (done on one PC and between Windows and Linux);
4, ONLINE in the launcher (done before 3, at the user's word: hosting and joining need nothing but the launcher, on one network);
3, join codes (written, and tested with a pretend router: no real router has been asked for a port yet, which is the user's to
try, hosting at home with a friend joining from elsewhere). Not in the first version: rollback, joining a game already running,
spectators, hosting from behind a shared address, IPv6 codes, chat.

## What a pointer's size breaks

Giving a struct a real pointer makes it bigger, so anything that assumed the old size is now wrong, and the compiler can't
always see it. `node tools/port/views.mjs` finds every struct read as another by Clang's AST and checks it against the layouts
at 32 and 64 bits. Most of what it still reports is casts back to the real type, mistyped locals, or code that 6.4 and 6.6 replace. Two aren't proven: `script_control_info` and `HubSparkSet` lie over the end of a `subengine`, so check them when they first run.

- Byte strides: `* 0x38` for a model bank entry, `<< 4` for a `BattleLevel`, offsets like `[0x6C / 4]`. Use the array or field.
- Views: a pad written `u8 pad0[0x74]` is the PS1's. Write it `u8 pad0[offsetof(subengine, entity)]`, which holds at any size.
- Word views of a struct, such as `st[0x12]` and `hero[40]`. Use the field, or `EXT_W(ext, i)` for an extension of plain words.
- Fixed sizes: `AllocExtension` and `AddExt` now take `sizeof`, and its heap aligns to 8 and has a header of `EXT_HEADER` bytes. The
  sizes the game asks for its heaps are still the PS1's, so 6.4 has to scale them.
- Tables split across two labels: the PS1 read on from one to the next (`DefaultArenaDef`, `TitlesBackdropAtable`). Make them one array.
- Words that were next to each other: `ScreenLabelState` and the four after it are one array now. `p[1]` from a single global reads whatever Clang put next.
- Views over an `EXTobject` with `sizeof(EXT) + 8`: its two pointers count for more now. Name the field with `offsetof`.
- Views of one extension that place their fields by the PS1's offsets past a pointer of the real struct: a Polar Push hero's
  (`PolarAi`, the prototype's `EXTpolardata`) has two at 0xA0 and 0xA4, so `Ext20`'s push effect at 0xA4 lay over
  `PolarHitExt`'s fall flag at 0xA8, and a hero who'd pushed with an effect (Cortex, Brio, Dingodile, Rilla Roo) fell straight
  off the ice ever after. The views' pads are `offsetof(PolarAi, ...)` now, with a `_Static_assert` each. `views.mjs` doesn't
  see these: `FindExtension` returns `void *`.
- Hooks: a function stored in `subengine.hook` takes a view of the subengine, which the view check can't see through the pointer (`PosDst`, `TextNAS`, `ModelObj`).
- A view indexed as an array has to be the struct's size: `PanelPrim` has a size check now.
- A slot that holds a number in some uses and an address in others is an `iptr` (`NAS.UserInfo`). Otherwise it gets its
  real pointer type.

## What the first attempt taught

Each of these cost time to find, and they all still apply:

- **Clang keeps data in first-use order, not source order**, and has no `-fno-toplevel-reorder`. Anything that
  relies on neighbouring globals breaks, which is why the real structs come first.
- **An optimiser believes a global's declared size.** `GameInfo` is declared as 4 bytes, and Clang deleted a write to
  it because a later write further on "covered" the object. Dead-store elimination is enough to do it.
- **Pass-through arguments are everywhere.** 44 direct calls, plus calls through function pointers:
  `HeroActionTable[].fn`, `HeroHooks.HeroSetIdle`, `UpdateShadow`, `HeroFacing`, `HeroKickCollision`, Tank's
  `SetMoeJoe*`.
- **Known null accesses.** `SetHeroIdle` and the games' own versions (`SetTankHeroIdle`, `SetHeroPogoIdle`,
  `SetDinoHeroIdle`, `SetChaseHeroIdle`, `SetNoxHeroFly`), which run before a hero has `hero_move`;
  `HubPreMoveHero`; `InitPolarBomb`; `InitBashHomer` and `InitLoadingScreen`, past the last subengine; the debug
  font's `ScreenRect` at boot.
- **Known overruns.** `PolarAIFlags` is 7 words, not 4. `VramAreas` and `HeroSide` are labels inside larger arrays.
  `LevelUnused` is defined in both the exe and the engine. `N2OCameraFollow` redeclares its parameter.
  `N2OSpawnFirstMissile` spawns at whatever is on the stack.
- **The game writes to a `const` string**: the completion figures, into `CardSaveTitleFormat`.
- **`SsInit` has to close every sound bank**: the game never closes its music banks.
- **The tables can be computed**, exactly as a retail exe has them: `rcossin_tbl` is `round(4096 sin)` and `round(4096
  cos)` over 4096 steps; `VectorNormal`'s is `floor(4096 sqrt(64 / (i + 64)))` for 192 entries; libsnd's pitch tables
  are `floor(0x8000 * 2^(i / 12))` and `floor(0x8000 * 2^(i / 1536))`.
- **GTE opcodes decide `sf`.** `VectorNormal`'s GPF is `0x190003D`, with `sf` clear, so it doesn't shift by 12.
  Getting this wrong made every model posed by a script key vanish.
- **libsnd.** A tick's timing, volumes and voice allocation were read from the linked code and can be read again:
  `_SsSeqPlay`, `_SsGetSeqData`, `_SsVmKeyOn`, `_SsVmKeyOnNow`, `_SsVmSetVol`, `_SsVmAlloc`, `SsPitchFromNote`.
  The music uses notes, program changes, pitch bends, controllers 6, 7, 10, 98 and 99, and the end marker.
  99 picks what 6 means: 16 the reverb's depth, 20 a loop's start (127: for ever). 99 being 30 ends the loop, with no 6.
- **Reverb is the song's.** The game sets the depth to 0 at every music load, and the song then sets it: read from the real game's RAM,
  `_svm_rattr` holds mode 2 and depth 0x1428 (20 of 127) once a song plays.
- **`SsSetVoiceMask` matters.** It's the voices the effects still hold: one whose sample ended stays theirs until they let it go. Giving it
  out sooner means the old effect's clean-up stops the new one.
- **The sound wandered between slow and fast** in a real window, in the first port, which mixed once a vblank. The mix now runs when the
  device asks for it.
- **A tone's sample**: sample n is from the end of n - 1 to the end of n in the bank's size table; the first port played n - 1.
