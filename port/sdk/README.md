# PC versions of PsyQ

The game's C calls PsyQ by name (`DrawOTag`, `LoadImage`, `SsSeqPlay`, `CdRead`, `PadSetAct`, ...).
This folder gives it the same names on PC: `include/` holds the headers (`libgpu.h`, `libgte.h`,
`libsnd.h`, `libcd.h`, `libpad.h`, ...), which the PC build finds where the PS1 build finds
`ext/psyq47/include`, and the `.c` files here implement them on top of `../platform`. For now
`stubs.c` holds every function, doing nothing.

Write these from what the game needs and from Sony's documented behaviour. Don't copy Sony's files:
they can't be in this repository.
