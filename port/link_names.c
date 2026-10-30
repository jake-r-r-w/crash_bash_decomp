/* Names the PS1 build gets from its link, as plain addresses with no C object behind them (configs/USA).
 * Here they need real memory. */

/* The heap and the overlay slot: from the slot (0x800B32B4 on the PS1) to 0x801FF000. On the PS1
 * HeapStart is lower, with the engine's image between it and the slot; here the engine is linked in,
 * so HeapStart is the slot (RAM_TOP, SlotOverlayId, SlotHeapStart in include/bash/). */
unsigned char HeapStart[0x14BD4C] __attribute__((section(".bss")));

/* The engine's .bss after its rodata (ARCHITECTURE.md, The engine's .bss). */
int IntroScriptHandle;
int IntroDbase;
unsigned char ModeTextBuffer[0x4654] __attribute__((section(".bss")));

/* What LOW_RAM (include/common.h) gives a null pointer: the PS1's low memory. */
unsigned char PcLowRam[0x10000] __attribute__((section(".bss")));

/* A word past the end of every slot overlay, where Papu's hero code keeps a sound handle. */
void *PapuStraySfx;

/* An instruction inside PsyQ's FntLoad, which nascalls.c patches. Nothing runs it on PC. */
int FntLoadPatch;
