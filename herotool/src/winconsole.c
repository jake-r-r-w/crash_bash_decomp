/* The tool's terminal on Windows. It's a window program there, with no console window of its own (the user's word, 2026-11-23: "when
 * it opens a command window opens with it can that not happen?"). Started from a terminal with its output going nowhere (a command typed
 * there, herotool import ...), it says what it does in that terminal; its output sent to a file or a pipe (a script's, a test's) goes
 * there as it always did. Elsewhere nothing to do. */
#include "kit.h"

#ifdef _WIN32
#include <stdio.h>
#include <windows.h>

void kit_terminal(void) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    if ((out == NULL || out == INVALID_HANDLE_VALUE || GetFileType(out) == FILE_TYPE_UNKNOWN) && AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
}
#else
void kit_terminal(void) {
}
#endif
