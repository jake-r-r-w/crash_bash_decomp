/* Names the PS1 build gets from its link, as plain addresses with no C object behind them (configs/USA).
 * Here they need real memory. How the heap and the slot work on PC is ROADMAP-B 6.2. */

/* The heap: from the exe's end to 0x801FF000 (ARCHITECTURE.md). */
unsigned char HeapStart[0x186370];
unsigned char SlotOverlayId[4];
unsigned char SlotHeapStart[4];

/* The engine's .bss after its rodata (ARCHITECTURE.md, The engine's .bss). */
int IntroScriptHandle;
int IntroDbase;
unsigned char ModeTextBuffer[0x4654];

/* A word past the end of every slot overlay, where Papu's hero code keeps a sound handle. */
void *PapuStraySfx;

/* An instruction inside PsyQ's FntLoad, which nascalls.c patches. Nothing runs it on PC. */
int FntLoadPatch;
