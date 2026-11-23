# The hero tool

Make a hero of your own for the PC port of Crash Bash from a 3D model, and play as them in every game. A custom hero plays like one
of the eight original heroes you pick (their moves, speed, ride and props), with your model, card and sounds.

Bobby, a dachshund, comes with it as an example.

## Opening it

- From the game: **Settings**, then the **Mods** page, then **HERO TOOL**.
- Or run `herotool.exe` (`herotool` on Linux), which sits next to the game's program.

It isn't on Android. A hero made on a PC plays there once its mod is copied over.

## The welcome page

- **Create a new hero**: drop a `.glb` model on it, or **Browse...** for one.
- **Edit a hero**: your heroes, each saying whether it's **In the game**. Click one to open it. Its folder button opens its files, and
  its trash deletes it from the tool and the game (you're asked first).
- At the bottom, the tool's two folders, each with a button to open it:
  - **Your heroes**, next to the tool: where **Save** keeps them. This is your heroes' source.
  - **The game's mods**: where **Add to game** puts them. This is what the game plays.

## What the model needs

- A `.glb` file (glTF 2.0) with its textures inside it, 8 at most.
- At most 1365 triangles and 4096 vertices.
- Standing and facing forward (+Z, Blender's front). Any size: the tool makes it the height you pick.
- Rigged with Mixamo, VRM, Blender or Unity bone names, or not rigged at all: then you place its joints.
- Its textures share 256 x 256 texels in the game, so keep them small.

## Making a hero

### 1. Who it's based on

The first question for a new hero. It plays as that hero: their moves in every game, their ride or prop, and their sounds where you
don't give your own. It's also made that hero's height to start with. You can change both later in **Hero Setup**; a new base
remakes every game but keeps any animations of your own.

### 2. Hero Setup

The first page (the person at the top left of the strip of games).

- **Joints**: the front and side views show your model with a dot on each joint. Drag the dots onto its joints: hips, spine,
  chest, neck, head, shoulders, elbows, wrists, knees, ankles, and a jaw, ears and a tail if it has them. Hover over a dot to see
  which joint it is.
  - Drag a box, or Ctrl-click dots, to move several at once.
  - Right-click to snap the picked dots to the grid.
  - **Mirror** moves the other side's dot too. **Grid** shows the grid.
  - Wheel to zoom, right-drag to move the view, right double-click to put it back. Ctrl+Z and Ctrl+Y undo and redo.
  - The preview beside them plays a move with the joints as they are.
- **Model**: a rigged model uses its own bones. An unrigged one has **Guess joints**, to put the dots where its shape says (it asks
  first, as it moves yours), and **Has: Jaw, Ears, Tail** for the extra bones.
- **Hero**: its **Name** in the game, an **Id** for files and online play, its **Height** (as one of the heroes, or a number of
  your own), **Based on**, and the **Author** shown in the game's mods list.
- **Card**: its picture in the character select. **Take a photo** turns the picture into a camera: drag to turn it, right-drag
  to move it, wheel to zoom, and pick a pose and frame. Beside it you see exactly what the game will show, in the character select
  and on the panel while playing. **Take photo** makes it the card. Or **Browse** for a `.png`, or **Clear** to let the game draw
  its face from the model.
- **Sounds**: a `.wav` for each of name, laugh, yelp, jump, land, thud, throw and fire. Each one you leave out is its base hero's.

### 3. Each game

The strip along the top has a page for each game (the arrow beside the hero's name hides the strip). Each page shows your hero as the game
makes it there. The first time takes a moment, while the game makes it.

- **On foot** can play its base's animations or its own. Other games can take the bear or the prop from another hero.
- **Its own animations**: change one and it becomes the hero's own. Pick a bone (click it, or on the timeline), then rotate, move
  or scale it with the gizmo (R, G, S). Each change is a key on the timeline at that frame:
  - **Fade** sets how many frames a key fades over, and how (Smooth, Bounce and so on). **Blend** flows it into the bone's next
    key instead.
  - **Stretch** off rotates bones rather than lengthening them. **X-Mirror** does each change on the other side too.
    **Onion** shows the frames before and after. **Clipping** shows where it pokes through itself.
  - **Hide** hides a bone (to tuck it into a tank's hatch, say). **Reset bone** and **Reset animation** put things back.
  - Space plays, Left and Right step a frame, Ctrl+C and Ctrl+V copy and paste a key, Delete removes one, Tab hides the bones.
    The **?** button lists the rest.
- The grid button beside the hero's name shows every game at once.

### 4. Save, add, preview

At the top right:

- **Save** keeps it in **Your heroes**, next to the tool.
- **Add to game** puts the saved hero in the game's mods folder and switches it on. It's greyed out until you've saved.
- **Preview in GAME** saves it and starts the game straight in that game, with your hero as player 1. Its arrow picks another
  game. **Esc** closes the preview.

A hero that's in the game shows up in the character select, in the row under the original heroes.

## Sharing a hero

A hero is a mod: its folder in the game's mods folder, or that folder zipped. Give it to a friend to drop in their mods folder.
To play a custom hero online, everyone needs the same one (the same files): if anyone hasn't got it, it isn't in the character
select for that game.

## More

[docs/HERO_PACKS.md](../docs/HERO_PACKS.md) has the details: what's in a hero's folder, how the game makes it, and the tool's
command line.
