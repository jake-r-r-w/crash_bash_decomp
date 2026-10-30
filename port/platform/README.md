# The host layer

What the PC versions of PsyQ (`../sdk`) run on: `platform.h`, then `sdl.c` for a window, sound and input,
or `no_host.c` in a build without SDL. Nothing PS1-specific, and nothing the game's C calls directly.
