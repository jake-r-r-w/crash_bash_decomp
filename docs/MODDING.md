# Making mods

A mod for the PC version is a zip of textures and files in place of the game's own. A player puts it in the `mods` folder next to
`crashbash.exe`, and it's on: the Mods page of the menu the game opens with turns mods on and off and puts them in order. This page is
for making one. How it works inside is in [PORT.md](PORT.md) (Mods).

## What's in a mod

```
My Mod.zip
  mod.json                optional: its name, author, version and description
  0a1b2c3d4e5f6789.png    a texture, named after the one it replaces
  textures/...            PNGs can be in folders of any names
  files/0A6.bin           one of the game's files, by its number
  files/0A7.bps           or a patch to one, with only what it changes
```

A zip of a folder, as zipping a folder makes it, is just as good: when everything's in one folder, that folder is the mod.

`mod.json` says what the Mods page shows:

```json
{
  "name": "Pink Crates",
  "author": "You",
  "version": "1.0",
  "description": "Every crate is pink"
}
```

Each one is optional. A mod without a name is called after its zip. The page shows them in the game's font, which has capitals, digits,
spaces and `. , ' : ! = + - ? % < >`: other letters become capitals and the rest are left out. A name of more than 18 characters is cut
short, and so is a description of more than 48.

## The demo mod

`port/mods/gift-boxes/` is a mod made as this page says, in art of its own: TNT crates wrapped up as presents, with a bow on the lid.
To try it, copy the folder into the game's `mods` folder, or zip it and put the zip there. Its PNGs are named after TNT's sides, the
sides of the 3, 2 and 1 it counts down with once it's hit, and its lid, which are the same in every crate arena, and they're four
times the game's size, so they're drawn sharper.

The game's TNT lid is the grey crate's image tinted red by the model, and a tint multiplies, so nothing on it could be gold. So the
mod has patches too: each crate arena's texture set gains an image for the lid, and its model points TNT's lid at it, without the
tint, and the lids of the 3, 2 and 1 it turns into, which are the wooden crate's image tinted purple. Nitro, the grey crate tinted
green, and the wooden crates keep their own. `node port/mods/gift_boxes.mjs` draws the PNGs and makes the patches again, against
your own `CRASHBSH.DAT` (`--data` says where it is).

## Making one as you go

Make a folder in `mods` and put what your mod has in that. A folder is a mod too, the same as its zip. The Mods page looks at the mods
again each time it's opened, so after changing a PNG, go back into the page to see it there. Start the game again to see it in a level.
Zip the folder when it's done.

## Textures

Run the game once with `--dump-textures` and a folder, from a command prompt or a shortcut:

```
crashbash.exe --dump-textures mytextures
```

and play. Each texture is saved to that folder as it first appears, as a PNG named after it. Edit the ones you want to change, at any
size up to 2048 x 2048 (a bigger one is drawn sharper), and put them in your mod with the same names. A texture with no PNG stays as
it was.

- A texel's alpha says what it is, which is all the game knows: under 64 clear, 64 to 191 semi-transparent, and over that solid.
- The same picture in other colours counts as another texture, with a name of its own.
- A scrolling texture is saved as it is at rest, and yours scrolls as it did.
- Only what's drawn gets saved, so play everything you want to change.
- Some faces of a model have no texture, only colours painted on, and no mod can change those.

## The game's files

The game's data is almost a thousand files in `CRASHBSH.DAT`, each with a number. `--dump-files` saves them all, named by their
numbers in hex, and stops:

```
crashbash.exe --dump-files myfiles
```

`--log-files` prints which ones the game reads as you play, and when, to find which a level or a menu has:

```
crashbash.exe --log-files > files.txt
```

Put a changed one in your mod's `files` folder with the same name, as `files/0A6.bin`. It can be bigger than the original: the files
after it are moved on to make room. But the game loads some files into room of a fixed size, so a much bigger file may not fit there.
The mode text files, `08D` to `098`, can be up to 16384 bytes, and the Mods page says if one's bigger. The game's code is built into the
PC version, so the code files (`1E7`, `2CC` to `2D8` and `3DF`) are left out of a mod.

What the files hold is in [ASSETS.md](ASSETS.md): texture sets, models, sound packs, mode text and title screens. The decomp's
`tools/scripts/dat.py` unpacks them into formats you can edit, and packs them back.

### Patches

A changed file is still the game's, so a mod that has one can't be shared without sharing the game. A patch can: put `files/0A6.bps`
in place of `files/0A6.bin`, and it holds only what you changed, with checksums of the game's file and of yours. The game applies it
to the player's own copy as it applies the mod's files. A patch is always to the game's own file, not to another mod's, and one made
from another version of the file is left out, with the Mods page saying so. Make one with

```
node port/mods/bps.mjs 0A6-original.bin 0A6-changed.bin files/0A6.bps
```

from the decomp's repo, which applies the patch again before it keeps it, to be sure; or with any maker of BPS patches, such as
Floating IPS. The original is the file `--dump-files` saved.

The same thing is often in more than one file: each crate arena has its own copy of the crates, and other games and the warp rooms
have some too, so a change to one arena's file only changes that arena. And some colours aren't in a texture at all: a model's faces
have colours that tint their texture, as the tops of TNT and Nitro crates are the grey crate's image tinted red and green, so those
are changed in the model's file.

## Which one wins

Where two mods have the same texture or file, the one higher on the Mods page wins. On that page, L1 and R1 (Q and W on the keyboard)
move the chosen mod up and down. New ones go at the top.

## When it changes

Textures change as soon as a mod's turned on or off. Files change when PLAY is pressed: the game reads them from then on.

## When something's wrong

The Mods page says BROKEN for a mod with nothing it can use, and why underneath: not a zip it can read, a zip in parts, a ZIP64 zip, or
no textures or files in it. A broken file in a mod that works is left out, and the game's own is used: a PNG it can't read, a file
that's encrypted or fails its check, a mode text file that's too big, or a patch that's damaged or for another file. Run as `crashbash.exe > log.txt 2>&1`, the game writes what it
found in each mod, and anything it couldn't read, to `log.txt`.

It reads zips that are stored or deflated, the usual kinds, as long as they're not encrypted.

Mods are files the game reads, so only use ones from people you trust. It checks them as it goes: a zip's numbers against its size,
each file against its size and its CRC, a patch's every count and offset, and a PNG's size before it's loaded.
