#include "common.h"
#include "bash/engine.h"

/* The engine's entry table, the first 0x14 bytes of its image (0x80078C90, HeapStart): the
 * exe's shell loads the engine there and runs it through this (EngineModule: shell_start_engine
 * calls init, shell_update update, shell_render render). init is RestartGame, the first boot's
 * start; heap is where the heap starts with the engine and the slot loaded. In .text, ahead of
 * the engine's code, as the original link had it. */
ModuleEntry EngineEntry = {
    RestartGame, UpdateProcess, RenderProcess, QuitProcess, SlotHeapStart,
};
