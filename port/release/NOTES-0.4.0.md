# Crash Bash Port 0.4.0

An Android version, with on-screen touch buttons, beside the Windows and Linux ones. On every system, the window's icon is now
Crash Bash's own PlayStation save icon.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. Your saves and settings carry over.

## Android

`crash-bash-port-0.4.0-android.apk`: open it on your phone to install it (Android asks you to allow installing apps from wherever you
opened it). It needs Android 8.0 or newer and Vulkan, which phones from about 2018 on have.

- The first time, it asks for your disc image: choose the .bin (or an .iso) in Android's file chooser, and it's copied once. Or copy
  `CRASHBSH.DAT` from the disc to `Android/data/io.github.jakerrw.crashbash/files` on the phone, over USB from a PC.
- Held sideways, the game fills the screen (WIDESCREEN is on from the start, and can be turned off in Settings). Held upright, it fits
  the top and the buttons sit below it.
- The touch buttons are see-through, over the game, and fade when you leave them alone. A Bluetooth or USB controller works too, and
  hides them until you touch the screen again.
- The back button is Triangle, the game's own way back.
- Ripper Roo and Pinstripe: put your Crash Bandicoot (USA) disc image in `Android/data/io.github.jakerrw.crashbash/files/discs` over USB,
  and turn on EXTRA HEROES on the Game page, under Settings.

**It's only been played in Android's emulator, not yet on a real phone.** If it misbehaves on yours, please say.

## Also new

- The window's icon is Crash Bash's PlayStation save icon, made from your own game data each time it starts, so it's not in the
  download. The Windows program has an icon of its own too.
- A touch screen on a PC gets the same buttons as a phone, from the first touch.

## How it's been tested

On each of the three, the release build plays every game mode. On Windows and Linux it also beats all four bosses, goes through the
menus, loads mods and reads disc images, and the extra heroes load. On Android, in its emulator, the first run takes a disc image from
Android's file chooser, the touch buttons work either way round, and the extra heroes load.

**Playing with several controllers hasn't been tested.** If yours misbehaves, please say.

No Mac version yet. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt` in the Windows and Linux downloads, or the repository.
