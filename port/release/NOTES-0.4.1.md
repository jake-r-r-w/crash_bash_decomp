# Crash Bash Port 0.4.1

A fix for Android phones that 0.4.0 wouldn't start on, saying "Crash Bash needs a graphics card with Vulkan". On Windows and Linux the
game plays and looks as in 0.4.0.

**These downloads have no game data.** You need your own copy of Crash Bash (USA). On Windows and Linux, put your disc image (a .cue
with its .bin, a raw .bin or an .iso) next to the program and run it: it copies what it needs once. On Android it asks for the disc
image the first time. Your saves and settings carry over.

## Fixes

- **Android:** the game starts on phones whose graphics haven't got a few Vulkan features it never needed, which 0.4.0 asked for
  anyway. Open `crash-bash-port-0.4.1-android.apk` on your phone and Android installs it over 0.4.0, keeping your game data, so it
  doesn't ask for the disc image again.
- If a phone still can't start it, the message now names the phone's graphics and what they lack. Please say what it says.

## How it's been tested

It's been played on a real Android phone now, as well as in Android's emulator. On each of the three, the release build plays every
game mode, and the extra heroes load. On Windows and Linux it also beats all four bosses, goes through the menus, loads mods and reads
disc images. Every game mode draws exactly the same, pixel for pixel, as on a phone without the feature most phones lack (depth
clamping).

**Playing with several controllers hasn't been tested.** If yours misbehaves, please say.

No Mac version yet. Only the USA versions' data is supported, for Crash Bash and for Crash Bandicoot.

## Licence

The port's own code is MIT. Crash Bash's and Crash Bandicoot's code and data belong to their respective owners, and nothing here
grants any right to the games. See `LICENSE.txt` in the Windows and Linux downloads, or the repository.
